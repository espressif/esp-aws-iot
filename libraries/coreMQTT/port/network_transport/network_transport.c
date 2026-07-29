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

#define TAG "network_transport"

Timeouts_t timeouts = { .connectionTimeoutMs = 4000, .sendTimeoutMs = 10000, .recvTimeoutMs = 2000 };

void vTlsSetConnectTimeout( uint16_t connectionTimeoutMs )
{
    timeouts.connectionTimeoutMs = connectionTimeoutMs;
}

void vTlsSetSendTimeout( uint16_t sendTimeoutMs )
{
    timeouts.sendTimeoutMs = sendTimeoutMs;
}

void vTlsSetRecvTimeout( uint16_t recvTimeoutMs )
{
    timeouts.recvTimeoutMs = recvTimeoutMs;
}

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
#if NETWORK_TRANSPORT_HAS_KEY_CONFIG
        .client_key = pxNetworkContext->client_key,
#elif ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(6, 0, 0)
        .use_secure_element = pxNetworkContext->use_secure_element,
#endif /* ESP-IDF 6.x without the unified key interface backport:
          esp-tls has no secure element support at all. */
        .ds_data = pxNetworkContext->ds_data,
        .clientkey_buf = ( const unsigned char* )( pxNetworkContext->pcClientKey ),
        .clientkey_bytes = pxNetworkContext->pcClientKeySize,
        .timeout_ms = timeouts.connectionTimeoutMs,
        .non_block = false,
    };

    if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, portMAX_DELAY ) == pdTRUE )
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
                if( esp_tls_get_conn_sockfd( pxNetworkContext->pxTls, &lSockFd ) == ESP_OK )
                {
                    int flags = fcntl( lSockFd, F_GETFL );

                    if( fcntl( lSockFd, F_SETFL, flags | O_NONBLOCK ) != -1 )
                    {
                        xResult = TLS_TRANSPORT_SUCCESS;
                    }
                }
            }

            if( xResult != TLS_TRANSPORT_SUCCESS )
            {
                esp_tls_conn_destroy( pxNetworkContext->pxTls );
                pxNetworkContext->pxTls = NULL;
            } else 
            {
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
        else if( esp_tls_conn_destroy(pxNetworkContext->pxTls ) == 0)
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
                             const void* pvData, size_t uxDataLen )
{
    int32_t lBytesSent = -1;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        TimeOut_t xTimeout;
        vTaskSetTimeOutState( &xTimeout );

        struct timeval timeout = { .tv_usec = timeouts.sendTimeoutMs * 1000, .tv_sec = 0 };
        TickType_t xTicksToWait = pdMS_TO_TICKS( timeouts.sendTimeoutMs );
        TickType_t start_tick = xTaskGetTickCount();

        if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, xTicksToWait ) == pdTRUE )
        {
            int lSockFd = -1;
            esp_err_t xError = esp_tls_get_conn_sockfd( pxNetworkContext->pxTls, &lSockFd );
            if( xError == ESP_OK )
            {
                unsigned char * pucData = ( unsigned char * ) pvData;
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

                    suseconds_t elapsed_time_usec = ( xTaskGetTickCount() - start_tick ) * portTICK_PERIOD_MS * 1000;
                    timeout.tv_usec = ( timeout.tv_usec - elapsed_time_usec >= 0 ) ? timeout.tv_usec - elapsed_time_usec : 0;
                    lSelectResult = select( lSockFd + 1, NULL, &write_fds, &error_fds, &timeout );

                    if( lSelectResult < 0 )
                    {
                        lBytesSent = -1;
                        ESP_LOGE( TAG, "Error during call to select." );
                    }
                    else if( ( lSelectResult > 0 ) && ( FD_ISSET( lSockFd, &write_fds ) != 0 ) )
                    {
                        ssize_t lResult = esp_tls_conn_write( pxNetworkContext->pxTls,
                                                        &( pucData[lBytesSent] ),
                                                     uxDataLen - lBytesSent );

                        if( lResult >= 0 )
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
                            /* Empty when MBEDTLS_ERR_SSL_WANT_READ || MBEDTLS_ERR_SSL_WANT_WRITE */
                        }
                    }
                    else
                    {
                        /* Empty when lSelectResult == 0 */
                    }
                }
                while( ( xTaskCheckForTimeOut( &xTimeout, &xTicksToWait ) == pdFALSE ) &&
                       ( lBytesSent < uxDataLen ) &&
                       ( lBytesSent >= 0 ) );
            }
            xSemaphoreGive(pxNetworkContext->xTlsContextSemaphore);
        }
    }

    return lBytesSent;
}

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
                             void* pvData, size_t uxDataLen )
{
    int32_t lBytesRead = -1;

    if( ( pvData != NULL ) &&
        ( uxDataLen > 0 ) &&
        ( pxNetworkContext != NULL ) &&
        ( pxNetworkContext->pxTls != NULL ) )
    {
        TimeOut_t xTimeout;
        vTaskSetTimeOutState( &xTimeout );

        TickType_t xTicksToWait = pdMS_TO_TICKS( timeouts.recvTimeoutMs );
        struct timeval timeout = {.tv_usec = timeouts.recvTimeoutMs * 1000, .tv_sec = 0};
        TickType_t start_tick = xTaskGetTickCount();

        if( xSemaphoreTake( pxNetworkContext->xTlsContextSemaphore, xTicksToWait ) == pdTRUE )
        {
            int lSockFd = -1;
            fd_set read_fds;
            fd_set error_fds;

            lBytesRead = 0;

            esp_tls_get_conn_sockfd( pxNetworkContext->pxTls, &lSockFd );
            FD_ZERO( &read_fds );
            FD_SET( lSockFd, &read_fds );
            FD_ZERO( &error_fds );
            FD_SET( lSockFd, &error_fds );

            do
            {
                // Fetch any buffered data
                ssize_t lResult = esp_tls_conn_read( pxNetworkContext->pxTls,
                                                pvData,
                                            ( size_t ) uxDataLen );

                if( lResult > 0 )
                {
                    lBytesRead = ( int32_t ) lResult;
                }
                else if( ( lResult == MBEDTLS_ERR_SSL_WANT_WRITE ) ||
                         ( lResult == MBEDTLS_ERR_SSL_WANT_READ ) )
                {
                    suseconds_t elapsed_time_usec = ( xTaskGetTickCount() - start_tick ) * portTICK_PERIOD_MS * 1000;
                    timeout.tv_usec = ( timeout.tv_usec - elapsed_time_usec >= 0 ) ? timeout.tv_usec - elapsed_time_usec : 0;

                    int lSelectResult = select( lSockFd + 1, &read_fds, NULL, &error_fds, &timeout );
                    if ( ( lSelectResult < 0 ) || FD_ISSET( lSockFd, &error_fds ) ) {
                        ESP_LOGE( TAG, "Error reading the message" );
                        lBytesRead = lResult = -1;
                    }
                }
                else if( lResult == 0 )
                {
                    ESP_LOGE( TAG, "Connection closed" );
                    lBytesRead = -1;
                }
                else
                {
                    ESP_LOGE( TAG, "Error reading: %d", ( int ) lResult );
                    lBytesRead = ( int32_t ) lResult;
                }
            }
            while ( ( xTaskCheckForTimeOut( &xTimeout, &xTicksToWait ) == pdFALSE ) &&
                    ( lBytesRead == 0 ) );


            ( void ) xSemaphoreGive( pxNetworkContext->xTlsContextSemaphore );
        }
    }
    return lBytesRead;
}
