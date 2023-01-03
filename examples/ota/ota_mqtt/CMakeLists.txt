# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/ota-for-aws-iot-embedded-sdk"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/coreMQTT"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/corePKCS11"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/coreJSON"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/backoffAlgorithm"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/common/posix_compat"
   )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ota_mqtt)

target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/root_cert_auth.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.crt" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/client.key" TEXT)
target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/certs/aws_codesign.crt" TEXT)