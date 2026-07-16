#ifndef ESP_TLS_TRANSPORT_H
#define ESP_TLS_TRANSPORT_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

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
    const char *pcServerRootCA;      /**< @brief Trusted server root certificate bytes.
                                                 If NULL and CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
                                                 is enabled, the ESP-IDF certificate bundle is
                                                 used instead. */
    uint32_t pcServerRootCASize;     /**< @brief Number of trusted server root certificate bytes. */
    const char *pcClientCert;        /**< @brief Client certificate bytes. */
    uint32_t pcClientCertSize;       /**< @brief Number of client certificate bytes. */
    const char *pcClientKey;         /**< @brief Client certificate's private key bytes. */
    uint32_t pcClientKeySize;        /**< @brief Number of client certificate's private key bytes. */
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

/**
 * @brief Structure to configure timeouts for individual TLS operations in milliseconds resolution.
 * Note that it uses FreeRTOS software timer internally and hence minimum resolution is the tick duration.
 * These timeouts can be specified before any receive or send operation. Timout of 0 will act as a non-blocking mode.
 *
 * Defaults are:
 * Connection timeout - 4 seconds
 * Send timeout - 10 seconds
 * Receive timeout - 2 seconds
 */
typedef struct Timeouts
{
    uint16_t connectionTimeoutMs;
    uint16_t sendTimeoutMs;
    uint16_t recvTimeoutMs;
} Timeouts_t;

TlsTransportStatus_t xTlsConnect(NetworkContext_t* pxNetworkContext );

TlsTransportStatus_t xTlsDisconnect( NetworkContext_t* pxNetworkContext );

int32_t espTlsTransportSend( NetworkContext_t* pxNetworkContext,
    const void* pvData, size_t uxDataLen );

int32_t espTlsTransportRecv( NetworkContext_t* pxNetworkContext,
    void* pvData, size_t uxDataLen );

void vTlsSetConnectTimeout( uint16_t connectionTimeoutMs );

void vTlsSetSendTimeout( uint16_t sendTimeoutMs );

void vTlsSetRecvTimeout( uint16_t recvTimeoutMs );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ESP_TLS_TRANSPORT_H */
