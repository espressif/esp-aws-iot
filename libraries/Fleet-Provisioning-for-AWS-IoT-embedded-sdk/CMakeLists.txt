# Include Fleet Provisioning library's source and header path variables.

include( "${CMAKE_CURRENT_LIST_DIR}/Fleet-Provisioning-for-AWS-IoT-embedded-sdk/fleetprovisioningFilePaths.cmake" )

idf_component_register(
    SRCS
        "${FLEET_PROVISIONING_SOURCES}"
    INCLUDE_DIRS
        "${FLEET_PROVISIONING_INCLUDE_PUBLIC_DIRS}"
        "config"
        "."
)
