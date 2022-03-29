# Include HTTP library's source and header path variables.
include( "${CMAKE_CURRENT_LIST_DIR}/coreHTTP/httpFilePaths.cmake" )

idf_component_register(
    SRCS
        "${HTTP_SOURCES}"
        "${HTTP_THIRD_PARTY_SOURCES}"
        "${CMAKE_CURRENT_LIST_DIR}/port/network_transport/network_transport.c"
    INCLUDE_DIRS
        "${HTTP_INCLUDE_PUBLIC_DIRS}"
        "${CMAKE_CURRENT_LIST_DIR}/../common/logging/"
        "${CMAKE_CURRENT_LIST_DIR}/port/network_transport"
        "config"
        "."
    REQUIRES
        esp-tls
)

set_source_files_properties(
    "${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/core_http_client.c"
    PROPERTIES COMPILE_FLAGS -Wno-stringop-truncation
)