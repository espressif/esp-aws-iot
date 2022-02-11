#ifndef ESP_TLS_TRANSPORT_H
#define ESP_TLS_TRANSPORT_H

#include "transport_interface.h"
#include "esp_tls.h"

typedef enum TlsTransportStatus
{
    TLS_TRANSPORT_SUCCESS = 0,         /**< Function successfully completed. */
    TLS_TRANSPORT_INVALID_PARAMETER,   /**< At least one parameter was invalid. */
    TLS_TRANSPORT_INSUFFICIENT_MEMORY, /**< Insufficient memory required to establish connection. */
    TLS_TRANSPORT_INVALID_CREDENTIALS, /**< Provided credentials were invalid. */
    TLS_TRANSPORT_HANDSHAKE_FAILED,    /**< Performing TLS handshake with server failed. */
    TLS_TRANSPORT_INTERNAL_ERROR,      /**< A call to a system API resulted in an internal error. */
    TLS_TRANSPORT_CONNECT_FAILURE,     /**< Initial connection to the server failed. */
    TLS_TRANSPORT_DISCONNECT_FAILURE
} TlsTransportStatus_t;

struct NetworkContext
{
    esp_tls_t* pxTls;
    const char *pcHostname;
    int xPort;
    const char *pcServerRootCAPem;
    const char *pcClientCertPem;
    const char *pcClientKeyPem;
    void *ds_data;
};

TlsTransportStatus_t xTlsConnect(NetworkContext_t* pxNetworkContext );

TlsTransportStatus_t xTlsDisconnect( NetworkContext_t* pxNetworkContext );

int32_t espTlsTransportSend( NetworkContext_t* pxNetworkContext,
    const void* pvData, size_t uxDataLen );

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
    void* pvData, size_t uxDataLen );

#endif /* ESP_TLS_TRANSPORT_H */