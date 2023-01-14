# This gives PKCS_INCLUDE_PUBLIC_DIRS, and PKCS_SOURCES
include(${CMAKE_CURRENT_LIST_DIR}/corePKCS11/pkcsFilePaths.cmake)

set(COREPKCS11_LOCATION "${CMAKE_CURRENT_LIST_DIR}/corePKCS11")

set(CORE_PKCS11_3RDPARTY_LOCATION "${COREPKCS11_LOCATION}/source/dependency/3rdparty")

set(COREPKCS_PORT_INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/port
    ${CMAKE_CURRENT_LIST_DIR}/../common/logging/
)

set(COREPKCS_INCLUDE_DIRS
    ${PKCS_INCLUDE_PUBLIC_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/config
    ${COREPKCS_PORT_INCLUDE_DIRS}
)

set(COREPKCS_PORT_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/port/core_pkcs11_pal.c
    ${CMAKE_CURRENT_LIST_DIR}/port/iot_crypto.c
)

set(COREPKCS_SRCS
    ${PKCS_SOURCES}
    ${COREPKCS_PORT_SRCS}
)

set(COREPKCS_REQUIRES
    mbedtls
    nvs_flash
    log
    bootloader_support
    spi_flash
)

idf_component_register(
    SRCS
        ${COREPKCS_SRCS}
        "${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils/mbedtls_utils.c"
    INCLUDE_DIRS
        ${COREPKCS_INCLUDE_DIRS}
        ${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils
    PRIV_INCLUDE_DIRS
        
    REQUIRES
        ${COREPKCS_REQUIRES}
)
