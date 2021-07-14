#
# Component Makefile
#

set (includes)
set (srcs)
set (priv_includes)

list(APPEND includes
    "aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/source/include/"
    "aws-iot-device-sdk-embedded-C/platform/include/"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/source/include"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/source/interface"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreJSON/source/include"
    "aws-iot-device-sdk-embedded-C/libraries/aws/device-shadow-for-aws-iot-embedded-sdk/source/include"
    "port/include"
    "port/logging"
    "port/network_transport"
    "port/mqtt_helpers"
    )

list(APPEND srcs
    "aws-iot-device-sdk-embedded-C/libraries/standard/backoffAlgorithm/source/backoff_algorithm.c"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/source/core_mqtt_serializer.c"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/source/core_mqtt_state.c"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreMQTT/source/core_mqtt.c"
    "aws-iot-device-sdk-embedded-C/libraries/standard/coreJSON/source/core_json.c"
    "aws-iot-device-sdk-embedded-C/libraries/aws/device-shadow-for-aws-iot-embedded-sdk/source/shadow.c"
    "aws-iot-device-sdk-embedded-C/platform/posix/clock_posix.c"
    "port/network_transport/tls_freertos.c"
    "port/mqtt_helpers/mqtt_demo_helpers.c"
    )

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS ${includes}
                    PRIV_INCLUDE_DIRS ${priv_includes}
                    REQUIRES lwip nghttp mbedtls tcp_transport)
