#ifndef SHADOW_CONFIG_H
#define SHADOW_CONFIG_H

#include "sdkconfig.h"
#define EXTRACT_ARGS( ... ) __VA_ARGS__
#define STRIP_PARENS( X ) X
#define REMOVE_PARENS( X ) STRIP_PARENS( EXTRACT_ARGS X )

/* Logging configurations */
#if CONFIG_FLEET_PROVISIONING_LOG_ERROR || CONFIG_FLEET_PROVISIONING_LOG_WARN || CONFIG_FLEET_PROVISIONING_LOG_INFO || CONFIG_FLEET_PROVISIONING_LOG_DEBUG

    /* Set logging level for the Device SHADOW component to highest level,
     * so any defined logging level below is printed. */
    #ifdef LOG_LOCAL_LEVEL
        #undef LOG_LOCAL_LEVEL
    #endif
    #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
    #include "esp_log.h"

    /* Change LIBRARY_LOG_NAME to "FleetProvisioning" if defined somewhere else. */
    #ifdef LIBRARY_LOG_NAME
        #undef LIBRARY_LOG_NAME
    #endif
    #define LIBRARY_LOG_NAME "FleetProvisioning"

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
#if CONFIG_FLEET_PROVISIONING_LOG_ERROR
    #define LogError( message, ... ) ESP_LOGE( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#endif

#if CONFIG_FLEET_PROVISIONING_LOG_WARN
    #define LogWarn( message, ... ) ESP_LOGW( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#endif

#if CONFIG_FLEET_PROVISIONING_LOG_INFO
    #define LogInfo( message, ... ) ESP_LOGI( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#endif

#if CONFIG_FLEET_PROVISIONING_LOG_DEBUG
    #define LogDebug( message, ... ) ESP_LOGD( LIBRARY_LOG_NAME, REMOVE_PARENS( message ), ##__VA_ARGS__ )
#endif

#endif /* SHADOW_CONFIG_H */