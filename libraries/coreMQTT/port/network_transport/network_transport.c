#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/semphr.h"
#include <string.h>
#include "esp_log.h"
#include "esp_tls.h"
#include "network_transport.h"
#include "sdkconfig.h"

TlsTransportStatus_t xTlsConnect( NetworkContext_t* pxNetworkContext )
{
    TlsTransportStatus_t xResult = TLS_TRANSPORT_CONNECT_FAILURE;
    TimeOut_t xTimeout;

    TickType_t xTicksToWait = pxNetworkContext->xTimeout;

    esp_tls_cfg_t xEspTlsConfig = {
        .cacert_buf = (const unsigned char*) ( pxNetworkContext->pcServerRootCA ),
        .cacert_bytes = pxNetworkContext->pcServerRootCASize,
        .clientcert_buf = (const unsigned char*) ( pxNetworkContext->pcClientCert ),
        .clientcert_bytes = pxNetworkContext->pcClientCertSize,
        .skip_common_name = pxNetworkContext->disableSni,
        .alpn_protos = pxNetworkContext->pAlpnProtos,
        .use_secure_element = pxNetworkContext->use_secure_element,
        .ds_data = pxNetworkContext->ds_data,
        .clientkey_buf = ( const unsigned char* )( pxNetworkContext->pcClientKey ),
        .clientkey_bytes = pxNetworkContext->pcClientKeySize,
        .timeout_ms = 1000,
    };

    vTaskSetTimeOutState( &xTimeout );

    if( xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY) == pdTRUE )
    {
        int lConnectResult = -1;
        esp_tls_t * pxTls = esp_tls_init();

        if( pxTls != NULL )
        {
            pxNetworkContext->pxTls = pxTls;

            do
            {
                lConnectResult = esp_tls_conn_new_async( pxNetworkContext->pcHostname,
                    strlen( pxNetworkContext->pcHostname ),
                    pxNetworkContext->xPort,
                    &xEspTlsConfig, pxTls );

                if( lConnectResult != 0 )
                {
                    break;
                }

                vTaskDelay( pdMS_TO_TICKS( 10 ) );
            }
            while( xTaskCheckForTimeOut( &xTimeout, &xTicksToWait ) == pdFALSE );

            if( lConnectResult == 1 )
            {
                xResult = TLS_TRANSPORT_SUCCESS;
            }
            else
            {
                esp_tls_conn_destroy( pxNetworkContext->pxTls );
                pxNetworkContext->pxTls = NULL;
            }
        }
        ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore );
    }

    return xResult;
}

TlsTransportStatus_t xTlsDisconnect( NetworkContext_t* pxNetworkContext )
{
    BaseType_t xResult = TLS_TRANSPORT_DISCONNECT_FAILURE;

    if( xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY ) == pdTRUE )
    {
        if( ( pxNetworkContext->pxTls != NULL ) &&
            ( esp_tls_conn_destroy(pxNetworkContext->pxTls ) != 0 ) )
        {
            xResult = TLS_TRANSPORT_DISCONNECT_FAILURE;
        }
        else
        {
            xResult = TLS_TRANSPORT_SUCCESS;
            pxNetworkContext->pxTls = NULL;
        }

        ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore );
    }

    return xResult;
}

int32_t espTlsTransportSend( NetworkContext_t* pxNetworkContext,
                             const void* pvData, size_t uxDataLen)
{

    int lSockFd = -1;
    fd_set write_fds;
    fd_set errorSet;

    struct timeval timeout = { .tv_usec = 10000, .tv_sec = 0 };

    int32_t lBytesSent = -1;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        TimeOut_t xTimeout;
        TickType_t xTicksToWait = pxNetworkContext->xTimeout;

        vTaskSetTimeOutState( &xTimeout );

        if( xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, xTicksToWait) == pdTRUE )
        {
            lBytesSent = 0;
            do
            {
                FD_ZERO(&write_fds);
                FD_SET(lSockFd, &write_fds);

                FD_ZERO(&errorSet);
                FD_SET(lSockFd, &errorSet);

                select(lSockFd + 1, NULL, &write_fds, &errorSet, &timeout);

                ssize_t lResult = esp_tls_conn_write(pxNetworkContext->pxTls, pvData, uxDataLen);

                if( lResult > 0 )
                {
                    lBytesSent += ( int32_t ) lResult;
                }
                else if( ( lResult != MBEDTLS_ERR_SSL_WANT_WRITE ) &&
                         ( lResult != MBEDTLS_ERR_SSL_WANT_READ ) )
                {
                    lBytesSent = lResult;
                }

                if( ( lBytesSent < 0 ) ||
                    ( lBytesSent == uxDataLen ) )
                {
                    break;
                }

            }
            while( xTaskCheckForTimeOut( &xTimeout, &xTicksToWait ) == pdFALSE );

            xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);
        }
    }

    return lBytesSent;
}

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
                             void* pvData, size_t uxDataLen)
{
    int32_t lBytesRead = -1;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY ) == pdTRUE )
        {
            lBytesRead = 0;

            ssize_t lResult = esp_tls_conn_read( pxNetworkContext->pxTls, pvData, ( size_t ) uxDataLen );

            if( lResult > 0 )
            {
                lBytesRead = ( int32_t ) lResult;
            }
            else if( ( lResult != ESP_TLS_ERR_SSL_WANT_READ ) &&
                    ( lResult != ESP_TLS_ERR_SSL_WANT_WRITE ) )
            {
                lBytesRead = -1;
            }
            else
            {
                /* Empty Else */
            }

            ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore);
        }
    }

    return lBytesRead;
}
