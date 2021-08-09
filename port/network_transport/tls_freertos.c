/*
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file tls_freertos.c
 * @brief TLS transport interface implementations. This implementation uses
 * mbedTLS.
 */

/* Standard includes. */
#include <string.h>

/* FreeRTOS includes. */
#include "freertos/FreeRTOS.h"

/* TLS transport header. */
#include "tls_freertos.h"

#include "esp_log.h"

static const char *TAG = "tls_freertos";

#define TLS_DEFAULT_PORT 443
/*-----------------------------------------------------------*/

TlsTransportStatus_t TLS_FreeRTOS_Connect( NetworkContext_t * pNetworkContext,
                                           const char * pHostName,
                                           uint16_t port,
                                           const NetworkCredentials_t * pNetworkCredentials,
                                           uint32_t receiveTimeoutMs,
                                           uint32_t sendTimeoutMs )
{
    esp_err_t ret;
    TlsTransportStatus_t returnStatus = TLS_TRANSPORT_SUCCESS;

    if( ( pNetworkContext == NULL ) ||
        ( pHostName == NULL ) ||
        ( pNetworkCredentials == NULL ) )
    {
        ESP_LOGE(TAG, "Invalid input parameter(s): Arguments cannot be NULL. pNetworkContext=%p, "
                    "pHostName=%p, pNetworkCredentials=%p.",
                    pNetworkContext,
                    pHostName,
                    pNetworkCredentials);
        return TLS_TRANSPORT_INVALID_PARAMETER;
    }
    
    pNetworkContext->transport = esp_transport_ssl_init();
    pNetworkContext->transport_list = esp_transport_list_init();
    ret = esp_transport_set_default_port(pNetworkContext->transport, TLS_DEFAULT_PORT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set default port for transport (%d)!", ret);
    }
    ret = esp_transport_list_add(pNetworkContext->transport_list, pNetworkContext->transport, "ssl");
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add transport to list (%d)!", ret);
    }
    pNetworkContext->receiveTimeoutMs = receiveTimeoutMs;
    pNetworkContext->sendTimeoutMs = sendTimeoutMs;
    if (pNetworkCredentials->pAlpnProtos) {
        esp_transport_ssl_set_alpn_protocol(pNetworkContext->transport, pNetworkCredentials->pAlpnProtos);
    }

    if (pNetworkCredentials->disableSni) {
        esp_transport_ssl_skip_common_name_check(pNetworkContext->transport);
    }

    if (pNetworkCredentials->pRootCa) {
        esp_transport_ssl_set_cert_data_der(pNetworkContext->transport, (const char *)pNetworkCredentials->pRootCa, pNetworkCredentials->rootCaSize);
    }

    if (pNetworkCredentials->pClientCert) {
        esp_transport_ssl_set_client_cert_data_der(pNetworkContext->transport, (const char *)pNetworkCredentials->pClientCert, pNetworkCredentials->clientCertSize);
    }

    if (pNetworkCredentials->pPrivateKey) {
        esp_transport_ssl_set_client_key_data_der(pNetworkContext->transport, (const char *)pNetworkCredentials->pPrivateKey, pNetworkCredentials->privateKeySize);
    }

    if (esp_transport_connect(pNetworkContext->transport, pHostName, port, receiveTimeoutMs) < 0) {
        returnStatus = TLS_TRANSPORT_CONNECT_FAILURE;
    } else {
        returnStatus = TLS_TRANSPORT_SUCCESS;
    }

    /* Clean up on failure. */
    if( returnStatus != TLS_TRANSPORT_SUCCESS )
    {
        if( pNetworkContext != NULL )
        {
            esp_transport_close( pNetworkContext->transport );
            esp_transport_destroy( pNetworkContext->transport );
        }
    }
    else
    {
        ESP_LOGI(TAG, "(Network connection %p) Connection to %s:%d established.",
                   pNetworkContext,
                   pHostName, port);
    }

    return returnStatus;
}
/*-----------------------------------------------------------*/

void TLS_FreeRTOS_Disconnect( NetworkContext_t * pNetworkContext )
{
    if (( pNetworkContext == NULL ) ) {
        ESP_LOGE(TAG, "Invalid input parameter(s): Arguments cannot be NULL. pNetworkContext=%p.", pNetworkContext);
        return;
    }

    /* Attempting to terminate TLS connection. */
    esp_transport_close( pNetworkContext->transport );

    /* Free TLS contexts. */
    esp_transport_destroy( pNetworkContext->transport );
}
/*-----------------------------------------------------------*/

int32_t TLS_FreeRTOS_recv( NetworkContext_t * pNetworkContext,
                           void * pBuffer,
                           size_t bytesToRecv )
{
    int32_t tlsStatus = 0;

    if (( pNetworkContext == NULL ) || 
        ( pBuffer == NULL) || 
        ( bytesToRecv == 0) ) {
        ESP_LOGE(TAG, "Invalid input parameter(s): Arguments cannot be NULL. pNetworkContext=%p, "
                "pBuffer=%p, bytesToRecv=%d.", pNetworkContext, pBuffer, bytesToRecv );
        return TLS_TRANSPORT_INVALID_PARAMETER;
    }

    tlsStatus = esp_transport_read(pNetworkContext->transport, pBuffer, bytesToRecv, pNetworkContext->receiveTimeoutMs);
    if (tlsStatus < 0) {
        ESP_LOGE(TAG, "Reading failed, errno= %d", errno);
        return ESP_FAIL;
    }

    return tlsStatus;
}
/*-----------------------------------------------------------*/

int32_t TLS_FreeRTOS_send( NetworkContext_t * pNetworkContext,
                           const void * pBuffer,
                           size_t bytesToSend )
{
    int32_t tlsStatus = 0;

    if (( pNetworkContext == NULL ) || 
        ( pBuffer == NULL) || 
        ( bytesToSend == 0) ) {
        ESP_LOGE(TAG, "Invalid input parameter(s): Arguments cannot be NULL. pNetworkContext=%p, "
                "pBuffer=%p, bytesToSend=%d.", pNetworkContext, pBuffer, bytesToSend );
        return TLS_TRANSPORT_INVALID_PARAMETER;
    }

    tlsStatus = esp_transport_write(pNetworkContext->transport, pBuffer, bytesToSend, pNetworkContext->sendTimeoutMs);
    if (tlsStatus < 0) {
        ESP_LOGE(TAG, "Writing failed, errno= %d", errno);
        return ESP_FAIL;
    }

    return tlsStatus;
}
/*-----------------------------------------------------------*/
