include( "${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/otaFilePaths.cmake" )

idf_component_register(
    SRCS
        "${OTA_SOURCES}"
        "${OTA_MQTT_SOURCES}"
        "${OTA_HTTP_SOURCES}"
        "port/ota_pal.c"
        "port/ota_os_freertos.c"
        "port/aws_esp_ota_ops.c"
        "port/iot_crypto.c"
    INCLUDE_DIRS
        "${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/source/portable/os/"
        "${OTA_INCLUDE_PUBLIC_DIRS}"
        "${CMAKE_CURRENT_LIST_DIR}/../common/logging/"
        "config"
        "port"
        "."
    PRIV_INCLUDE_DIRS
        "${OTA_INCLUDE_PRIVATE_DIRS}"
    REQUIRES
        spi_flash
        mbedtls
        bootloader_support
        app_update
        corePKCS11
)

set_source_files_properties(
"${CMAKE_CURRENT_LIST_DIR}/ota-for-aws-iot-embedded-sdk/source/ota.c"
PROPERTIES COMPILE_FLAGS -Wno-format)