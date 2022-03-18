#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_tls.h"
#include "network_transport.h"
#include "sdkconfig.h"

#define CONFIG_GRI_USE_DS_PERIPHERAL 0

TlsTransportStatus_t xTlsConnect( NetworkContext_t* pxNetworkContext )
{
    TlsTransportStatus_t xRet = TLS_TRANSPORT_SUCCESS;

    esp_tls_cfg_t xEspTlsConfig = {
        .cacert_buf = (const unsigned char*) ( pxNetworkContext->pcServerRootCAPem ),
        .cacert_bytes = strlen( pxNetworkContext->pcServerRootCAPem ) + 1,
        .clientcert_buf = (const unsigned char*) ( pxNetworkContext->pcClientCertPem ),
        .clientcert_bytes = strlen( pxNetworkContext->pcClientCertPem ) + 1,
#if CONFIG_GRI_USE_DS_PERIPHERAL
        .ds_data = pxNetworkContext->ds_data,
#else
        .ds_data = NULL,
        .clientkey_buf = ( const unsigned char* )( pxNetworkContext->pcClientKeyPem ),
        .clientkey_bytes = strlen( pxNetworkContext->pcClientKeyPem ) + 1,
#endif /* CONFIG_USE_DS_PERIPHERAL */
        .timeout_ms = 500,
    };

    esp_tls_t* pxTls = esp_tls_init();

    xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY);
    pxNetworkContext->pxTls = pxTls;

    if (esp_tls_conn_new_sync( pxNetworkContext->pcHostname, 
            strlen( pxNetworkContext->pcHostname ), 
            pxNetworkContext->xPort, 
            &xEspTlsConfig, pxTls) <= 0)
    {
        xRet = TLS_TRANSPORT_CONNECT_FAILURE;
    }

    xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);

    return xRet;
}

TlsTransportStatus_t xTlsDisconnect( NetworkContext_t* pxNetworkContext )
{
    BaseType_t xRet = TLS_TRANSPORT_SUCCESS;

    xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY);
    if (pxNetworkContext->pxTls != NULL && 
        esp_tls_conn_destroy(pxNetworkContext->pxTls) < 0)
    {
        xRet = TLS_TRANSPORT_DISCONNECT_FAILURE;
    }

    pxNetworkContext->pxTls = NULL;
    xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);

    return xRet;
}

int32_t espTlsTransportSend(NetworkContext_t* pxNetworkContext,
    const void* pvData, size_t uxDataLen)
{
    int32_t lBytesSent = 0;

    if(pxNetworkContext != NULL && pxNetworkContext->pxTls != NULL)
    {
        xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY);
        lBytesSent = esp_tls_conn_write(pxNetworkContext->pxTls, pvData, uxDataLen);
        xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);
    }
    else
    {
        lBytesSent = -1;
    }

    return lBytesSent;
}

int32_t espTlsTransportRecv(NetworkContext_t* pxNetworkContext,
    void* pvData, size_t uxDataLen)
{
    int32_t lBytesRead = 0;

    if(pxNetworkContext != NULL && pxNetworkContext->pxTls != NULL)
    {
        xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY);
        lBytesRead = esp_tls_conn_read(pxNetworkContext->pxTls, pvData, uxDataLen);
        xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);
    }
    else
    {
        lBytesRead = -1;
    }

    /* Temporary fix for underlying mbedTLS want read error... connection is still fine. not sure why esp_tls_conn_read returns this error */
    if(lBytesRead == -0x6900)
    {
        lBytesRead = 0;
    }

    return lBytesRead;
}