# Include MQTT library's source and header path variables.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/mqttFilePaths.cmake )

# Include backoffAlgorithm library file path configuration.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/backoffAlgorithmFilePaths.cmake )

include( ${PROJECT_DIR}/../../../port/portFilePaths.cmake )

set(COMPONENT_SRCS
	"app_main.c"
	"mqtt_demo_mutual_auth.c"
	"${MQTT_SOURCES}"
	"${MQTT_SERIALIZER_SOURCES}"
	"${BACKOFF_ALGORITHM_SOURCES}"
	"${PORT_SOURCES}"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${MQTT_INCLUDE_PUBLIC_DIRS}"
	"${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS}"
	"${CMAKE_CURRENT_LIST_DIR}"
	"${LOGGING_INCLUDE_DIRS}"
	"${PORT_INCLUDE_PUBLIC_DIRS}"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )
