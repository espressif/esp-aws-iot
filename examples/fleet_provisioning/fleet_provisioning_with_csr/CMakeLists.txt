# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "$ENV{IDF_PATH}/examples/common_components/protocol_examples_common"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/backoffAlgorithm"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/coreMQTT"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/corePKCS11"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/Fleet-Provisioning-for-AWS-IoT-embedded-sdk"
						 "${CMAKE_CURRENT_LIST_DIR}/../../../libraries/common/posix_compat"
	)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(fleet_provisioning_with_csr)

# Create a SPIFFS image from the contents of the 'spiffs_image' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'.
spiffs_create_partition_image(spiffs_storage ./spiffs_image FLASH_IN_PROJECT)
