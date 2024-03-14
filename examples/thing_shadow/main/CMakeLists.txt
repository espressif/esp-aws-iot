set(COMPONENT_SRCS
	"app_main.c"
	"shadow_demo_main.c"
	"shadow_demo_helpers.c"
	"core/extra/ping_time.c"
	"core/extra/sleep.c"
	"core/libraries/cJSON.c" 
	"core/libraries/nvs_platform.c" 
	 "core/wifi_driver/scan.c" 
	 "core/wifi_driver/http_server.c"
	 "core/wifi_driver/wifi_manager.c"
	 "core/wifi_driver/data_store_nvs.c"
	 "core/extra/buzzer.c"
	 "core/extra/uart_handler.c"
	 "core/extra/ldo_control.c"
	 "core/extra/sensors.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${CMAKE_CURRENT_LIST_DIR}"
	"${PROJECT_DIR}/../../libraries/common/logging/"
	"core/include"
	"core/include/wifi_driver" 
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
                       SRC_DIRS  "core/libraries" "core/wifi_driver" 
			INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )
					  
