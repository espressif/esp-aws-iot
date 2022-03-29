set(COMPONENT_SRCS
    "app_main.c"
    "http_demo_mutual_auth.c"
    "http_demo_utils.c"
    )

set(COMPONENT_ADD_INCLUDEDIRS
    "."
    "${CMAKE_CURRENT_LIST_DIR}"
    )

idf_component_register(SRCS "${COMPONENT_SRCS}"
                       INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
                      )