# This gives MQTT_INCLUDE_PUBLIC_DIRS, MQTT_SOURCES, and MQTT_SERIALIZER_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/coreMQTT/mqttFilePaths.cmake)

set(COREMQTT_INCLUDE_DIRS
    ${MQTT_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}/../common/logging/
)

set(COREMQTT_SRCS
    ${MQTT_SOURCES}
    ${MQTT_SERIALIZER_SOURCES}
)

set(COREMQTT_REQUIRES
    esp-tls
    network_transport
)

idf_component_register(
    SRCS
        ${COREMQTT_SRCS}
    INCLUDE_DIRS
        ${COREMQTT_INCLUDE_DIRS}
    REQUIRES
        ${COREMQTT_REQUIRES}
)

set_source_files_properties(
"${CMAKE_CURRENT_LIST_DIR}/coreMQTT/source/core_mqtt.c"
PROPERTIES COMPILE_FLAGS -Wno-format)
