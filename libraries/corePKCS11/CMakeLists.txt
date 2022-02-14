# Set path to corePKCS11 and it's third party libraries.

set(COREPKCS11_LOCATION "${CMAKE_CURRENT_LIST_DIR}/corePKCS11")

set(CORE_PKCS11_3RDPARTY_LOCATION "${COREPKCS11_LOCATION}/source/dependency/3rdparty")

include( ${COREPKCS11_LOCATION}/pkcsFilePaths.cmake )

idf_component_register(
    SRCS
        "${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils/mbedtls_utils.c"
        # "${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils/mbedtls_error.c"
        "${PKCS_SOURCES}"
        "${COREPKCS11_LOCATION}/source/portable/os/posix/core_pkcs11_pal.c"
        "${COREPKCS11_LOCATION}/source/portable/os/core_pkcs11_pal_utils.c"
    INCLUDE_DIRS
        "${COREPKCS11_LOCATION}/source/include"
        "${CMAKE_CURRENT_LIST_DIR}/../common/logging/"
        "${CORE_PKCS11_3RDPARTY_LOCATION}/pkcs11"
        "config"
        "."
    PRIV_INCLUDE_DIRS
        "${COREPKCS11_LOCATION}/source/portable/os"
        "${CORE_PKCS11_3RDPARTY_LOCATION}/mbedtls_utils"
    REQUIRES
        mbedtls
        
)

set_source_files_properties(
"${COREPKCS11_LOCATION}/source/portable/os/posix/core_pkcs11_pal.c"
PROPERTIES COMPILE_FLAGS -Wno-format)