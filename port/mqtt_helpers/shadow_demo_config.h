/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 */

#ifndef DEMO_CONFIG_H
#define DEMO_CONFIG_H

/* FreeRTOS config include. */
#include "freertos/FreeRTOSConfig.h"

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Include logging header files and define logging macros in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define the LIBRARY_LOG_NAME and LIBRARY_LOG_LEVEL macros depending on
 * the logging configuration for DEMO.
 * 3. Include the header file "logging_stack.h", if logging is enabled for DEMO.
 */

#include "logging_levels.h"

/* Logging configuration for the Demo. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME    "ShadowDemo"
#endif

#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_DEBUG
#endif

/* Map the SdkLog macro to the logging function to enable logging
 * on Windows simulator. */
#define SdkLog( message )    printf message

#include "logging_stack.h"

/************ End of logging configuration ****************/

/**
 * @brief The MQTT client identifier used in this example.  Each client identifier
 * must be unique; so edit as required to ensure that no two clients connecting to
 * the same broker use the same client identifier.
 *
 *!!! Please note a #defined constant is used for convenience of demonstration
 *!!! only.  Production devices can use something unique to the device that can
 *!!! be read by software, such as a production serial number, instead of a
 *!!! hard coded constant.
 *
 * #define democonfigCLIENT_IDENTIFIER				"insert here."
 */
#define democonfigCLIENT_IDENTIFIER       CONFIG_MQTT_CLIENT_IDENTIFIER

/**
 * @brief Endpoint of the MQTT broker to connect to.
 *
 * This demo application can be run with any MQTT broker, that supports server
 * authentication.
 *
 * @note If you would like to setup an MQTT broker for running this demo,
 * please see `mqtt_broker_setup.txt`.
 *
 * #define democonfigMQTT_BROKER_ENDPOINT    "...insert here..."
 */
#define democonfigMQTT_BROKER_ENDPOINT    CONFIG_MQTT_BROKER_ENDPOINT

/**
 * @brief The port to use for the demo.
 *
 * In general, port 8883 is for secured MQTT connections.
 *
 * @note Port 443 requires use of the ALPN TLS extension with the ALPN protocol
 * name. When using port 8883, ALPN is not required.
 *
 * #define democonfigMQTT_BROKER_PORT    ( insert here )
 */
#define democonfigMQTT_BROKER_PORT    CONFIG_MQTT_BROKER_PORT

/**
 * @brief Server's root CA certificate.
 *
 * For AWS IoT MQTT broker, this certificate is used to identify the AWS IoT
 * server and is publicly available. Refer to the AWS documentation available
 * in the link below.
 * https://docs.aws.amazon.com/iot/latest/developerguide/server-authentication.html#server-authentication-certs
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define democonfigROOT_CA_PEM    "...insert here..."
 */

/**
 * @brief Client certificate.
 *
 * For AWS IoT MQTT broker, refer to the AWS documentation below for details
 * regarding client authentication.
 * https://docs.aws.amazon.com/iot/latest/developerguide/client-authentication.html
 *
 * @note This certificate should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----\n"\
 * "...base64 data...\n"\
 * "-----END CERTIFICATE-----\n"
 *
 * #define democonfigCLIENT_CERTIFICATE_PEM    "...insert here..."
 */

/**
 * @brief Client's private key.
 *
 *!!! Please note pasting a key into the header file in this manner is for
 *!!! convenience of demonstration only and should not be done in production.
 *!!! Never paste a production private key here!.  Production devices should
 *!!! store keys securely, such as within a secure element.  Additionally,
 *!!! we provide the corePKCS library that further enhances security by
 *!!! enabling securely stored keys to be used without exposing them to
 *!!! software.
 *
 * For AWS IoT MQTT broker, refer to the AWS documentation below for details
 * regarding clientauthentication.
 * https://docs.aws.amazon.com/iot/latest/developerguide/client-authentication.html
 *
 * @note This private key should be PEM-encoded.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----\n"\
 * "...base64 data...\n"\
 * "-----END RSA PRIVATE KEY-----\n"
 *
 * #define democonfigCLIENT_PRIVATE_KEY_PEM    "...insert here..."
 */

/**
 * @brief An option to disable Server Name Indication.
 *
 * @note When using a local Mosquitto server setup, SNI needs to be disabled
 * for an MQTT broker that only has an IP address but no hostname. However,
 * SNI should be enabled whenever possible.
 */
#define democonfigDISABLE_SNI            CONFIG_MQTT_OPTION_SNI

/**
 * @brief Configuration that indicates if the demo connection is made to the AWS IoT Core MQTT broker.
 *
 * If username/password based authentication is used, the demo will use appropriate TLS ALPN and
 * SNI configurations as required for the Custom Authentication feature of AWS IoT.
 * For more information, refer to the following documentation:
 * https://docs.aws.amazon.com/iot/latest/developerguide/custom-auth.html#custom-auth-mqtt
 *
 * #define democonfigUSE_AWS_IOT_CORE_BROKER    ( 1 )
 */

/**
 * @brief The username value for authenticating client to the MQTT broker when
 * username/password based client authentication is used.
 *
 * For AWS IoT MQTT broker, refer to the AWS IoT documentation below for
 * details regarding client authentication with a username and password.
 * https://docs.aws.amazon.com/iot/latest/developerguide/custom-authentication.html
 * An authorizer setup needs to be done, as mentioned in the above link, to use
 * username/password based client authentication.
 *
 * #define democonfigCLIENT_USERNAME    "...insert here..."
 */

/**
 * @brief The password value for authenticating client to the MQTT broker when
 * username/password based client authentication is used.
 *
 * For AWS IoT MQTT broker, refer to the AWS IoT documentation below for
 * details regarding client authentication with a username and password.
 * https://docs.aws.amazon.com/iot/latest/developerguide/custom-authentication.html
 * An authorizer setup needs to be done, as mentioned in the above link, to use
 * username/password based client authentication.
 *
 * #define democonfigCLIENT_PASSWORD    "...insert here..."
 */

/**
 * @brief The name of the operating system that the application is running on.
 * The current value is given as an example. Please update for your specific
 * operating system.
 */
#define OS_NAME                   "FreeRTOS"

/**
 * @brief The version of the operating system that the application is running
 * on. The current value is given as an example. Please update for your specific
 * operating system version.
 */
#define OS_VERSION                tskKERNEL_VERSION_NUMBER

/**
 * @brief The name of the hardware platform the application is running on. The
 * current value is given as an example. Please update for your specific
 * hardware platform.
 */
#define HARDWARE_PLATFORM_NAME    CONFIG_HARDWARE_PLATFORM_NAME

/**
 * @brief The name of the MQTT library used and its version, following an "@"
 * symbol.
 */
#define MQTT_LIB                  "core-mqtt@1.0.0"

/**
 * @brief Set the stack size of the main demo task.
 *
 * In the Windows port, this stack only holds a structure. The actual
 * stack is created by an operating system thread.
 */
#define DEMO_STACKSIZE         CONFIG_MQTT_EXAMPLE_STACKSIZE

/**
 * @brief Size of the network buffer for MQTT packets.
 */
#define NETWORK_BUFFER_SIZE    CONFIG_MQTT_NETWORK_BUFFER_SIZE

/**
 * @brief Predefined thing name.
 *
 * This is the example predefine thing name and could be compiled in ROM code.
 */
#define THING_NAME           CONFIG_MQTT_CLIENT_IDENTIFIER

/**
 * @brief The length of #THING_NAME.
 */
#define THING_NAME_LENGTH    ( ( uint16_t ) ( sizeof( THING_NAME ) - 1 ) )

/**
 * @brief Predefined shadow name.
 *
 * Defaults to unnamed "Classic" shadow. Change to a custom string to use a named shadow.
 */
#ifndef SHADOW_NAME
    #define SHADOW_NAME    SHADOW_NAME_CLASSIC
#endif

/**
 * @brief The length of #SHADOW_NAME.
 */
#define SHADOW_NAME_LENGTH    ( ( uint16_t ) ( sizeof( SHADOW_NAME ) - 1 ) )

#endif /* DEMO_CONFIG_H */
