#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/semphr.h"
#include <string.h>
#include "esp_log.h"
#include "esp_tls.h"
#include "sys/socket.h"
#include "network_transport.h"
#include "sdkconfig.h"

TlsTransportStatus_t xTlsConnect( NetworkContext_t* pxNetworkContext )
{
    TlsTransportStatus_t xResult = TLS_TRANSPORT_CONNECT_FAILURE;

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
        .timeout_ms = 2000,
        .non_block = false,
    };

    if( xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY) == pdTRUE )
    {
        int lConnectResult = -1;
        esp_tls_t * pxTls = esp_tls_init();

        if( pxTls != NULL )
        {
            pxNetworkContext->pxTls = pxTls;

            lConnectResult = esp_tls_conn_new_sync( pxNetworkContext->pcHostname,
                strlen( pxNetworkContext->pcHostname ),
                pxNetworkContext->xPort,
                &xEspTlsConfig, pxTls );

            if( lConnectResult == 1 )
            {

                int lSockFd = -1;
                if( esp_tls_get_conn_sockfd(pxNetworkContext->pxTls, &lSockFd) == ESP_OK)
                {
                    int flags = fcntl(lSockFd, F_GETFL);

                    if( fcntl(lSockFd, F_SETFL, flags | O_NONBLOCK ) != -1)
                    {
                        xResult = TLS_TRANSPORT_SUCCESS;
                    }
                }
            }

            if( xResult != TLS_TRANSPORT_SUCCESS )
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
    BaseType_t xResult;

    if( xSemaphoreTake(pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY ) == pdTRUE )
    {
        if( pxNetworkContext->pxTls == NULL )
        {
            xResult = TLS_TRANSPORT_SUCCESS;
        }
        else if(esp_tls_conn_destroy(pxNetworkContext->pxTls ) == 0)
        {
            xResult = TLS_TRANSPORT_SUCCESS;
        }
        else
        {
            xResult = TLS_TRANSPORT_DISCONNECT_FAILURE;
        }

        ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore );
    }
    else
    {
        xResult = TLS_TRANSPORT_DISCONNECT_FAILURE;
    }

    return xResult;
}

int32_t espTlsTransportSend( NetworkContext_t* pxNetworkContext,
                             const void* pvData, size_t uxDataLen)
{

    int lSockFd = -1;
    int32_t lBytesSent = -1;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        TimeOut_t xTimeout;
        TickType_t xTicksToWait = pdMS_TO_TICKS(10);

        vTaskSetTimeOutState( &xTimeout );

        if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, xTicksToWait ) == pdTRUE )
        {
            esp_err_t xError = esp_tls_get_conn_sockfd( pxNetworkContext->pxTls, &lSockFd );
            if( xError == ESP_OK)
            {
                struct timeval timeout = { .tv_usec = 10000, .tv_sec = 0 };
                lBytesSent = 0;
                do
                {
                    fd_set write_fds;
                    fd_set error_fds;
                    int lSelectResult = -1;

                    FD_ZERO( &write_fds );
                    FD_SET( lSockFd, &write_fds );

                    FD_ZERO( &error_fds );
                    FD_SET( lSockFd, &error_fds );

                    lSelectResult = select( lSockFd + 1, NULL, &write_fds, &error_fds, &timeout );

                    if( lSelectResult < 0 )
                    {
                        lBytesSent = -1;
                        break;
                    }
                    else if( lSelectResult > 0 )
                    {
                        ssize_t lResult = esp_tls_conn_write( pxNetworkContext->pxTls, pvData, uxDataLen );

                        if( lResult > 0 )
                        {
                            lBytesSent += ( int32_t ) lResult;
                        }
                        else if( ( lResult != MBEDTLS_ERR_SSL_WANT_WRITE ) &&
                                ( lResult != MBEDTLS_ERR_SSL_WANT_READ ) )
                        {
                            lBytesSent = lResult;
                        }
                        else
                        {
                            /* Empty when lResult == 0 */
                        }

                        if( ( lBytesSent < 0 ) ||
                            ( lBytesSent == uxDataLen ) )
                        {
                            break;
                        }
                    }
                    else
                    {
                        /* Empty when lSelectResult == 0 */
                    }
                }
                while( xTaskCheckForTimeOut( &xTimeout, &xTicksToWait ) == pdFALSE );
            }
            xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);
        }
    }

    return lBytesSent;
}

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
                             void* pvData, size_t uxDataLen)
{
    int32_t lBytesRead;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY ) == pdTRUE )
        {
            ssize_t lResult = esp_tls_conn_read( pxNetworkContext->pxTls, pvData, ( size_t ) uxDataLen );

            if( lResult > 0 )
            {
                lBytesRead = ( int32_t ) lResult;
            }
            else if( ( lResult == ESP_TLS_ERR_SSL_WANT_READ ) ||
                     ( lResult == ESP_TLS_ERR_SSL_WANT_WRITE ) )
            {
                lBytesRead = 0;
            }
            else
            {
                lBytesRead = -1;
            }

            ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore);
        }
    }
    else
    {
        lBytesRead = -1;
    }

    return lBytesRead;
}
