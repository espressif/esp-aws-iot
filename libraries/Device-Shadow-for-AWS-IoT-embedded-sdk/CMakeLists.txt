# Include Device Shadow library's source and header path variables.

include( "${CMAKE_CURRENT_LIST_DIR}/Device-Shadow-for-AWS-IoT-embedded-sdk/shadowFilePaths.cmake" )

idf_component_register(
    SRCS
        "${SHADOW_SOURCES}"
    INCLUDE_DIRS
        "${SHADOW_INCLUDE_PUBLIC_DIRS}"
        "config"
        "."
)
