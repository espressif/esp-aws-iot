# Include required library's source and header path variables.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/aws/ota-for-aws-iot-embedded-sdk/otaFilePaths.cmake )
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/mqttFilePaths.cmake )
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP/httpFilePaths.cmake )

# Include backoffAlgorithm library file path configuration.
include( ${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/backoffAlgorithmFilePaths.cmake )

# Set path to corePKCS11 and it's third party libraries.
set(COREPKCS11_LOCATION "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/corePKCS11")
set(3RDPARTY_LOCATION "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/source/dependency/3rdparty")
set(CORE_PKCS11_3RDPARTY_LOCATION "${COREPKCS11_LOCATION}/source/dependency/3rdparty")

# Include PKCS #11 library's source and header path variables.
include( ${COREPKCS11_LOCATION}/pkcsFilePaths.cmake )

include( ${PROJECT_DIR}/../../../port/portFilePaths.cmake )

set(COMPONENT_SRCS 
	"app_main.c"
	"ota_demo_core_http.c"
	"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/ota/common/src/mqtt_subscription_manager.c"
	"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/ota/common/src/http_demo_utils.c"
	"${OTA_SOURCES}"
	"${OTA_MQTT_SOURCES}"
	"${OTA_HTTP_SOURCES}"
	"${MQTT_SOURCES}"
	"${MQTT_SERIALIZER_SOURCES}"
	"${BACKOFF_ALGORITHM_SOURCES}"
	"${HTTP_SOURCES}"
	"${HTTP_THIRD_PARTY_SOURCES}"
	"${PORT_SOURCES}"
	"${PORT_OTA_SOURCES}"
	"${PKCS_SOURCES}"
	"${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils/mbedtls_utils.c"
    "${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils/mbedtls_error.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/ota/common/include"
	"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/aws/ota-for-aws-iot-embedded-sdk/source/portable/os/"
	"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/demos/http/common/include"
	"${CMAKE_CURRENT_LIST_DIR}"
	"${LOGGING_INCLUDE_DIRS}"
	"${BACKOFF_ALGORITHM_INCLUDE_PUBLIC_DIRS}"
	"${OTA_INCLUDE_PUBLIC_DIRS}"
	"${OTA_INCLUDE_PRIVATE_DIRS}"
	"${MQTT_INCLUDE_PUBLIC_DIRS}"
	"${HTTP_INCLUDE_PUBLIC_DIRS}"
	"${PORT_INCLUDE_PUBLIC_DIRS}"
	"${PORT_OTA_INCLUDE_PUBLIC_DIRS}"
	"${PKCS_INCLUDE_PUBLIC_DIRS}"
	"${CORE_PKCS11_3RDPARTY_LOCATION}/pkcs11"
	"${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )

set_source_files_properties(
"${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/aws/ota-for-aws-iot-embedded-sdk/source/ota.c"
PROPERTIES COMPILE_FLAGS -Wno-format)

set_source_files_properties(
    "${PROJECT_DIR}/../../../aws-iot-device-sdk-embedded-C/libraries/standard/coreHTTP/source/core_http_client.c"
    PROPERTIES COMPILE_FLAGS -Wno-stringop-truncation
)