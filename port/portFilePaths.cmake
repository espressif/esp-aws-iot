set( PORT_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/network_transport/tls_freertos.c"
    "${CMAKE_CURRENT_LIST_DIR}/posix-compat/clock_esp.c"
    )

set( PORT_INCLUDE_PUBLIC_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/logging"
    "${CMAKE_CURRENT_LIST_DIR}/network_transport"
    "${CMAKE_CURRENT_LIST_DIR}/posix-compat"
    "${CMAKE_CURRENT_LIST_DIR}/../aws-iot-device-sdk-embedded-C/platform/include"
    )
