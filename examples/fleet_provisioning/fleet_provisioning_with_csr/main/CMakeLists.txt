set(COMPONENT_SRCS
	"app_main.c"
	"fleet_provisioning_with_csr_demo.c"
	"mqtt_operations.c"
	"fleet_provisioning_serializer.c"
	"pkcs11_operations.c"
	"mbedtls_pkcs11_posix.c"
	)

set(COMPONENT_ADD_INCLUDEDIRS
	"."
	"${CMAKE_CURRENT_LIST_DIR}"
	)

idf_component_register(SRCS "${COMPONENT_SRCS}"
					   INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
					  )