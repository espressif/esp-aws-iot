# Include MQTT library's source and header path variables.
include( ${PROJECT_DIR}/../../aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/mqttFilePaths.cmake )

# Include backoffAlgorithm library file path configuration.
include( ${PROJECT_DIR}/../../aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/backoffAlgorithmFilePaths.cmake )

# Include Shadow library's source and header path variables.
include( ${PROJECT_DIR}/../../aws-iot-device-sdk-embedded-C/libraries/aws/device-shadow-for-aws-iot-embedded-sdk/shadowFilePaths.cmake )

# Include JSON library's source and header path variables.
include( ${PROJECT_DIR}/../../aws-iot-device-sdk-embedded-C/libraries/standard/coreJSON/jsonFilePaths.cmake )

include( ${PROJECT_DIR}/../../port/portFilePaths.cmake )

set(COMPONENT_SRCS
	"app_main.c"
	"shadow_demo_main.c"
	"shadow_demo_helpers.c"
	"${MQTT_SOURCES}"
	"${MQTT_SERIALIZER_SOURCES}"
	"${BACKOFF_ALGORITHM_SOURCES}"
	"${SHADOW_SOURCES}"
	"${JSON_SOURCES}"
	"${PORT_SOURCES}"
	)

set(COMPONENT_ADD_INCLUDEDIRS "."
	"${LOGGING_INCLUDE_DIRS}"
	"${MQTT_INCLUDE_PUBLIC_DIRS}"
	"${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS}"
	"${SHADOW_INCLUDE_PUBLIC_DIRS}"
	"${CMAKE_CURRENT_LIST_DIR}"
	"${JSON_INCLUDE_PUBLIC_DIRS}"
	"${PORT_INCLUDE_PUBLIC_DIRS}"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )
