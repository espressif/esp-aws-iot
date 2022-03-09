# This gives OTA_INCLUDE_PUBLIC_DIRS, and OTA_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/otaFilePaths.cmake)

set(AWS_OTA_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/port
)

set(AWS_OTA_INCLUDE_DIRS
    ${OTA_INCLUDE_PUBLIC_DIRS}
    ${OTA_INCLUDE_OS_FREERTOS_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${AWS_OTA_PORT_INCLUDE_DIRS}
)

set(AWS_OTA_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/port/aws_esp_ota_ops.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ota_pal.c
)

set(AWS_OTA_SRCS
    ${OTA_SOURCES}
    ${OTA_OS_FREERTOS_SOURCES}
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
        ${AWS_OTA_SRCS}
    INCLUDE_DIRS
        ${AWS_OTA_INCLUDE_DIRS}
    REQUIRES
        ${AWS_OTA_REQUIRES}
)