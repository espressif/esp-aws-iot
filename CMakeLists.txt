# coreMQTT

# This gives MQTT_INCLUDE_PUBLIC_DIRS, MQTT_SOURCES, and MQTT_SERIALIZER_SOURCES
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

# This gives MQTT_AGENT_INCLUDE_PUBLIC_DIRS and MQTT_AGENT_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/coreMQTT-Agent/mqttAgentFilePaths.cmake)

set(COREMQTT_AGENT_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port/freertos_agent_message.c
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port/freertos_command_pool.c
)

set(COREMQTT_AGENT_INCLUDE_DIRS
    ${MQTT_AGENT_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT-Agent/port
)

set(COREMQTT_AGENT_SRCS
    ${MQTT_AGENT_SOURCES}
    ${COREMQTT_AGENT_PORT_SRCS}
)

# Network transport port

if(CONFIG_AWS_IOT_USE_NETWORK_TRANSPORT_PORT)

    set(NETWORK_TRANSPORT_PORT_SRCS
        ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/port/network_transport/network_transport.c
    )

    set(NETWORK_TRANSPORT_PORT_INCLUDE_DIRS
        ${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT/port/network_transport
    )

    set(NETWORK_TRANSPORT_PORT_REQUIRES 
        esp-tls
    )

else()

    set(NETWORK_TRANSPORT_PORT_SRCS
    )

    set(NETWORK_TRANSPORT_PORT_INCLUDE_DIRS
    )

    set(NETWORK_TRANSPORT_PORT_REQUIRES 
    )

endif()

set(NETWORK_TRANSPORT_PORT_REQUIRES 
        esp-tls
)

idf_component_register(
    SRCS
        ${COREMQTT_SRCS}
        ${COREMQTT_AGENT_SRCS}
        ${NETWORK_TRANSPORT_PORT_SRCS}
    INCLUDE_DIRS
        ${COREMQTT_INCLUDE_DIRS}
        ${COREMQTT_AGENT_INCLUDE_DIRS}
        ${NETWORK_TRANSPORT_PORT_INCLUDE_DIRS}
    REQUIRES
        ${NETWORK_TRANSPORT_PORT_REQUIRES}
)
