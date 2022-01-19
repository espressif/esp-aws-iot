# coreMQTT
include(${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/coreMQTT/mqttFilePaths.cmake)

set(COREMQTT_INCLUDE_DIRS
    ${MQTT_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/config
)

set(COREMQTT_SRCS
    ${MQTT_SOURCES}
    ${MQTT_SERIALIZER_SOURCES}
)

# coreMQTT-Agent
include(${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/coreMQTT-Agent/mqttAgentFilePaths.cmake)

set(COREMQTT_AGENT_INCLUDE_DIRS
    ${MQTT_AGENT_INCLUDE_PUBLIC_DIRS}
)

set(COREMQTT_AGENT_SRCS
    ${MQTT_AGENT_SOURCES}
)
    
idf_component_register(
    SRCS
        ${COREMQTT_SRCS}
        ${COREMQTT_AGENT_SRCS}
    INCLUDE_DIRS
        ${COREMQTT_INCLUDE_DIRS}
        ${COREMQTT_AGENT_INCLUDE_DIRS}
)
