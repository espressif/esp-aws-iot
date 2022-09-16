# This gives OTA_INCLUDE_PUBLIC_DIRS, and OTA_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/otaFilePaths.cmake)

set(AWS_OTA_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/port
    ${CMAKE_CURRENT_LIST_DIR}/../common/logging/
)

set(AWS_OTA_INCLUDE_DIRS
    ${OTA_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${AWS_OTA_PORT_INCLUDE_DIRS}
)

set(AWS_OTA_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/port/aws_esp_ota_ops.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ota_pal.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ota_os_freertos.c
)

set(AWS_OTA_SRCS
    ${OTA_SOURCES}
    ${OTA_MQTT_SOURCES}
    ${OTA_HTTP_SOURCES}
    ${AWS_OTA_PORT_SRCS}
)

set(AWS_OTA_REQUIRES
    esp_rom
    mbedtls
    corePKCS11
    coreJSON
    esp_common
    spi_flash
    bootloader_support
    efuse
    log
    app_update
    cbor
)

idf_component_register(
    SRCS
        ${AWS_OTA_SRCS}
    INCLUDE_DIRS
        ${AWS_OTA_INCLUDE_DIRS}
    PRIV_INCLUDE_DIRS
        ${OTA_INCLUDE_PRIVATE_DIRS}
    REQUIRES
        ${AWS_OTA_REQUIRES}
)

set_source_files_properties(
"${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/source/ota.c"
PROPERTIES COMPILE_FLAGS -Wno-format)
