#ifndef COREMQTT_CONFIG_H
#define COREMQTT_CONFIG_H

#include "sdkconfig.h"
#define EXTRACT_ARGS( ... ) __VA_ARGS__
#define STRIP_PARENS( X ) X
#define REMOVE_PARENS( X ) STRIP_PARENS( EXTRACT_ARGS X )

/* Logging configurations */
#if CONFIG_CORE_MQTT_LOG_ERROR || CONFIG_CORE_MQTT_LOG_WARN || CONFIG_CORE_MQTT_LOG_INFO || CONFIG_CORE_MQTT_LOG_DEBUG

    /* Set logging level for the coreMQTT and coreMQTT-Agent components to highest level,
     * so any defined logging level below is printed. */
    #ifdef LOG_LOCAL_LEVEL
        #undef LOG_LOCAL_LEVEL
    #endif
    #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
    #include "esp_log.h"

    /* Change LIBRARY_LOG_NAME to "coreMQTT" if defined somewhere else. */
    #ifdef LIBRARY_LOG_NAME
        #undef LIBRARY_LOG_NAME
    #endif
    #define LIBRARY_LOG_NAME "coreMQTT"

#endif

/* Undefine logging macros if they were defined somewhere else like another AWS/FreeRTOS library. */
#ifdef LogError
    #undef LogError
#endif

#ifdef LogWarn
    #undef LogWarn
#endif

#ifdef LogInfo
    #undef LogInfo
#endif

#ifdef LogDebug
    #undef LogDebug
#endif

/* Define logging macros based on configurations in sdkconfig.h. */
#if CONFIG_CORE_MQTT_LOG_ERROR
    #define LogError( message, ... ) ESP_LOGE( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#else
    #define LogError( message, ... )
#endif

#if CONFIG_CORE_MQTT_LOG_WARN
    #define LogWarn( message, ... ) ESP_LOGW( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#else
    #define LogWarn( message, ... )
#endif

#if CONFIG_CORE_MQTT_LOG_INFO
    #define LogInfo( message, ... ) ESP_LOGI( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#else
    #define LogInfo( message, ... )
#endif

#if CONFIG_CORE_MQTT_LOG_DEBUG
    #define LogDebug( message, ... ) ESP_LOGD( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#else
    #define LogDebug( message, ... )
#endif

/* coreMQTT configurations */
#define MQTT_STATE_ARRAY_MAX_COUNT CONFIG_MQTT_STATE_ARRAY_MAX_COUNT
#define MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT CONFIG_MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT
#define MQTT_PINGRESP_TIMEOUT_MS CONFIG_MQTT_PINGRESP_TIMEOUT_MS
#define MQTT_RECV_POLLING_TIMEOUT_MS CONFIG_MQTT_RECV_POLLING_TIMEOUT_MS
#define MQTT_SEND_TIMEOUT_MS CONFIG_MQTT_SEND_TIMEOUT_MS

/* coreMQTT-Agent configurations */
#define MQTT_AGENT_MAX_OUTSTANDING_ACKS CONFIG_MQTT_AGENT_MAX_OUTSTANDING_ACKS
#define MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME CONFIG_MQTT_AGENT_MAX_EVENT_QUEUE_WAIT_TIME

#endif /* COREMQTT_CONFIG_H */