#ifndef ESP_TLS_TRANSPORT_H
#define ESP_TLS_TRANSPORT_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "transport_interface.h"
#include "esp_tls.h"

typedef enum TlsTransportStatus
{
    TLS_TRANSPORT_SUCCESS = 0,              /**< Function successfully completed. */
                                            /**< -1 is reserved for ESP_FAIL */
    TLS_TRANSPORT_INVALID_PARAMETER = -2,   /**< At least one parameter was invalid. */
    TLS_TRANSPORT_INSUFFICIENT_MEMORY = -3, /**< Insufficient memory required to establish connection. */
    TLS_TRANSPORT_INVALID_CREDENTIALS = -4, /**< Provided credentials were invalid. */
    TLS_TRANSPORT_HANDSHAKE_FAILED = -5,    /**< Performing TLS handshake with server failed. */
    TLS_TRANSPORT_INTERNAL_ERROR = -6,      /**< A call to a system API resulted in an internal error. */
    TLS_TRANSPORT_CONNECT_FAILURE = -7,     /**< Initial connection to the server failed. */
    TLS_TRANSPORT_DISCONNECT_FAILURE = -8   /**< Failed to disconnect from server. */
} TlsTransportStatus_t;

struct NetworkContext
{
    SemaphoreHandle_t xTlsContextSemaphore;
    esp_tls_t* pxTls;
    const char *pcHostname;          /**< @brief Server host name. */
    int xPort;                       /**< @brief Server port in host-order. */
    const char *pcServerRootCAPem;   /**< @brief String representing a trusted server root certificate. */
    const char *pcClientCertPem;     /**< @brief String representing the client certificate. */
    const char *pcClientKeyPem;      /**< @brief String representing the client certificate's private key. */
    bool use_secure_element;         /**< @brief Boolean representing the use of secure element
                                                 for the TLS connection. */
    void *ds_data;                   /**< @brief Pointer for digital signature peripheral context */

    /**
    * @brief To use ALPN, set this to a NULL-terminated list of supported
    * protocols in decreasing order of preference.
    *
    * See [this link]
    * (https://aws.amazon.com/blogs/iot/mqtt-with-tls-client-authentication-on-port-443-why-it-is-useful-and-how-it-works/)
    * for more information.
    */
    const char ** pAlpnProtos;

    /**
    * @brief Disable server name indication (SNI) for a TLS session.
    */
    BaseType_t disableSni;
};

TlsTransportStatus_t xTlsConnect(NetworkContext_t* pxNetworkContext );

TlsTransportStatus_t xTlsDisconnect( NetworkContext_t* pxNetworkContext );

int32_t espTlsTransportSend( NetworkContext_t* pxNetworkContext,
    const void* pvData, size_t uxDataLen );

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
    void* pvData, size_t uxDataLen );

#endif /* ESP_TLS_TRANSPORT_H */