# coreMQTT

# This gives MQTT_INCLUDE_PUBLIC_DIRS, MQTT_SOURCES, and MQTT_SERIALIZER_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/coreMQTT/mqttFilePaths.cmake)

set(COREMQTT_INCLUDE_DIRS
    ${MQTT_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/config
)

set(COREMQTT_SRCS
    ${MQTT_SOURCES}
    ${MQTT_SERIALIZER_SOURCES}
)

# coreMQTT-Agent

# This gives MQTT_AGENT_INCLUDE_PUBLIC_DIRS and MQTT_AGENT_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/coreMQTT-Agent/mqttAgentFilePaths.cmake)

set(COREMQTT_AGENT_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port/freertos_agent_message.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port/freertos_command_pool.c
)

set(COREMQTT_AGENT_INCLUDE_DIRS
    ${MQTT_AGENT_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port
)

set(COREMQTT_AGENT_SRCS
    ${MQTT_AGENT_SOURCES}
    ${COREMQTT_AGENT_PORT_SRCS}
)

# Network transport port

if(CONFIG_AWS_IOT_USE_NETWORK_TRANSPORT_PORT)

    set(NETWORK_TRANSPORT_PORT_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/port/network_transport/network_transport.c
    )

    set(NETWORK_TRANSPORT_PORT_INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/port/network_transport
    )

    set(NETWORK_TRANSPORT_PORT_REQUIRES 
        esp-tls
    )

else()

    set(NETWORK_TRANSPORT_PORT_SRCS
    )

    set(NETWORK_TRANSPORT_PORT_INCLUDE_DIRS
    )

    set(NETWORK_TRANSPORT_PORT_REQUIRES 
    )

endif()

set(NETWORK_TRANSPORT_PORT_REQUIRES 
    esp-tls
)

# corePKCS11

# This gives PKCS_INCLUDE_PUBLIC_DIRS, and PKCS_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/libraries/corePKCS11/corePKCS11/pkcsFilePaths.cmake)

set(COREPKCS_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/corePKCS11/port
)

set(COREPKCS_INCLUDE_DIRS
    ${PKCS_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/corePKCS11/config
    ${COREPKCS_PORT_INCLUDE_DIRS}
)

set(COREPKCS_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/corePKCS11/port/core_pkcs11_pal.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/corePKCS11/port/iot_crypto.c
)

set(COREPKCS_SRCS
    ${PKCS_SOURCES}
    ${COREPKCS_PORT_SRCS}
)

set(COREPKCS_REQUIRES
    mbedtls
    nvs_flash
    log
    bootloader_support
)

# OTA

# This gives OTA_INCLUDE_PUBLIC_DIRS, and OTA_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/ota-for-aws-iot-embedded-sdk/otaFilePaths.cmake)

set(AWS_OTA_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/port
)

set(AWS_OTA_INCLUDE_DIRS
    ${OTA_INCLUDE_PUBLIC_DIRS}
    ${OTA_INCLUDE_PRIVATE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/config
    ${AWS_OTA_PORT_INCLUDE_DIRS}
)

set(AWS_OTA_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/port/aws_esp_ota_ops.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/port/ota_pal.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/ota-for-aws-iot-embedded-sdk/port/ota_os_freertos.c
)

set(AWS_OTA_SRCS
    ${OTA_SOURCES}
    ${OTA_MQTT_SOURCES}
    ${OTA_HTTP_SOURCES}
    ${AWS_OTA_PORT_SRCS}
)

set(AWS_OTA_REQUIRES
    esp_common
    spi_flash
    bootloader_support
    efuse
    log
    app_update
)

idf_component_register(
    SRCS
        ${COREMQTT_SRCS}
        ${COREMQTT_AGENT_SRCS}
        ${NETWORK_TRANSPORT_PORT_SRCS}
        ${COREPKCS_SRCS}
        ${AWS_OTA_SRCS}
    INCLUDE_DIRS
        ${COREMQTT_INCLUDE_DIRS}
        ${COREMQTT_AGENT_INCLUDE_DIRS}
        ${NETWORK_TRANSPORT_PORT_INCLUDE_DIRS}
        ${COREPKCS_INCLUDE_DIRS}
        ${AWS_OTA_INCLUDE_DIRS}
    REQUIRES
        ${NETWORK_TRANSPORT_PORT_REQUIRES}
        ${COREPKCS_REQUIRES}
        ${AWS_OTA_REQUIRES}
)