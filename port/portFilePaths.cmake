set( PORT_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/network_transport/tls_freertos.c"
    "${CMAKE_CURRENT_LIST_DIR}/posix-compat/clock_esp.c"
    "${CMAKE_CURRENT_LIST_DIR}/posix-compat/semaphore.c"
    )

set( PORT_OTA_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/ota/ota_os_freertos.c"
    "${CMAKE_CURRENT_LIST_DIR}/ota/aws_esp_ota_ops.c"
    "${CMAKE_CURRENT_LIST_DIR}/ota/ota_pal.c"
    "${CMAKE_CURRENT_LIST_DIR}/crypto/iot_crypto.c"
    "${CMAKE_CURRENT_LIST_DIR}/pkcs11/core_pkcs11_pal.c"
    )

set( PORT_INCLUDE_PUBLIC_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/logging"
    "${CMAKE_CURRENT_LIST_DIR}/network_transport"
    "${CMAKE_CURRENT_LIST_DIR}/posix-compat"
    "${CMAKE_CURRENT_LIST_DIR}/../aws-iot-device-sdk-embedded-C/platform/include"
    )

set( PORT_OTA_INCLUDE_PUBLIC_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/crypto"
    "${CMAKE_CURRENT_LIST_DIR}/ota"
    )
