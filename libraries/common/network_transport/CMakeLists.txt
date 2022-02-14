idf_component_register(
    SRCS
        "tls_freertos.c"
    INCLUDE_DIRS
        "."
    REQUIRES
        tcp_transport
)
