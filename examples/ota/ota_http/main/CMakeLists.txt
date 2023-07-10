set(COMPONENT_SRCS 
	"app_main.c"
	"ota_demo_core_http.c"
	"http_demo_url_utils.c"
	"mqtt_subscription_manager.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${PROJECT_DIR}/../../../libraries/common/logging/"
	"${PROJECT_DIR}/../../../libraries/common/posix_compat/"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )