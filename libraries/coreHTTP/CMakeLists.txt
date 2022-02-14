# Include HTTP library's source and header path variables.
include( "${CMAKE_CURRENT_LIST_DIR}/coreHTTP/httpFilePaths.cmake" )

idf_component_register(
    SRCS
        "${HTTP_SOURCES}"
        "${HTTP_THIRD_PARTY_SOURCES}"
    INCLUDE_DIRS
        "${HTTP_INCLUDE_PUBLIC_DIRS}"
        "${CMAKE_CURRENT_LIST_DIR}/../common/logging/"
        "config"
        "."
    REQUIRES
        network_transport
)

set_source_files_properties(
    "${CMAKE_CURRENT_LIST_DIR}/coreHTTP/source/core_http_client.c"
    PROPERTIES COMPILE_FLAGS -Wno-stringop-truncation
)