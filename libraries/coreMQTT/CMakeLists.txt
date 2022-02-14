# Include MQTT library's source and header path variables.
include( "${CMAKE_CURRENT_LIST_DIR}/coreMQTT/mqttFilePaths.cmake" )

idf_component_register(
    SRCS
        "${MQTT_SOURCES}"
        "${MQTT_SERIALIZER_SOURCES}"
    INCLUDE_DIRS
        "${MQTT_INCLUDE_PUBLIC_DIRS}"
        "${CMAKE_CURRENT_LIST_DIR}/../common/logging/"
        "config"
        "."
    REQUIRES
        network_transport
)
