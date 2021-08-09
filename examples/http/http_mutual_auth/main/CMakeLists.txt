# Include HTTP library's source and header path variables.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP/httpFilePaths.cmake )

# Include backoffAlgorithm library file path configuration.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/backoffAlgorithmFilePaths.cmake )

include( ${PROJECT_DIR}/../../../port/portFilePaths.cmake )

set(COMPONENT_SRCS
    "app_main.c"
    "http_demo_mutual_auth.c"
    "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/http/common/src/http_demo_utils.c"
    "${HTTP_SOURCES}"
    "${HTTP_THIRD_PARTY_SOURCES}"
    "${BACKOFF_ALGORITHM_SOURCES}"
    "${PORT_SOURCES}"
    )

set(COMPONENT_ADD_INCLUDEDIRS
    "."
    "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/http/common/include"
    "${HTTP_INCLUDE_PUBLIC_DIRS}"
    "${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS}"
    "${CMAKE_CURRENT_LIST_DIR}"
    "${LOGGING_INCLUDE_DIRS}"
    "${PORT_INCLUDE_PUBLIC_DIRS}"
    )

idf_component_register(SRCS "${COMPONENT_SRCS}"
                       INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
                      )

set_source_files_properties(
    "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP/source/core_http_client.c"
    PROPERTIES COMPILE_FLAGS -Wno-stringop-truncation
)