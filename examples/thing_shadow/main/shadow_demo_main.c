    /*
    * AWS IoT Device SDK for Embedded C 202103.00
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
    */

    /**
     * @file shadow_demo_main.c
     *
     * @brief Demo for showing how to use the Device Shadow library's API. This version
     * of Device Shadow API provide macros and helper functions for assembling MQTT topics
     * strings, and for determining whether an incoming MQTT message is related to a
     * device shadow. The shadow can be either the classic shadow or a named shadow. Change
     * #SHADOW_NAME to select the shadow. The Device Shadow library does not depend on a MQTT library,
     * therefore the code for MQTT connections are placed in another file (shadow_demo_helpers.c)
     * to make it easy to read the code using Device Shadow library.
     *
     * This example assumes there is a powerOn state in the device shadow. It does the
     * following operations:
     * 1. Establish a MQTT connection by using the helper functions in shadow_demo_helpers.c.
     * 2. Assemble strings for the MQTT topics of device shadow, by using macros defined by the Device Shadow library.
     * 3. Subscribe to those MQTT topics by using helper functions in shadow_demo_helpers.c.
     * 4. Publish a desired state of powerOn by using helper functions in shadow_demo_helpers.c.  That will cause
     * a delta message to be sent to device.
     * 5. Handle incoming MQTT messages in eventCallback, determine whether the message is related to the device
     * shadow by using a function defined by the Device Shadow library (Shadow_MatchTopicString). If the message is a
     * device shadow delta message, set a flag for the main function to know, then the main function will publish
     * a second message to update the reported state of powerOn.
     * 6. Handle incoming message again in eventCallback. If the message is from update/accepted, verify that it
     * has the same clientToken as previously published in the update message. That will mark the end of the demo.
     */

    /* Standard includes. */
    #include <assert.h>
    #include <stdlib.h>
    #include <string.h>

    /* POSIX includes. */
    #include <unistd.h>
    #include <inttypes.h>

    #include "esp_sleep.h"

    /* shadow demo helpers header. */
    #include "shadow_demo_helpers.h"

    /* Shadow config include. */
    #include "shadow_config.h"

    /* SHADOW API header. */
    #include "shadow.h"

    /* JSON API header. */
    #include "core_json.h"

    /* Clock for timer. */
    #include "clock.h"
    #include "main.h"
    #include "ping_time.h"
    #include <cJSON.h>
    #include "buzzer.h"
    #include "uart_handler.h"
    #include <iconv.h>
    #include "ldo_control.h"

    /**
     * @brief Format string representing a Shadow document with a "desired" state.
     *
     * The real json document will look like this:
     * {
     *   "state": {
     *     "desired": {
     *       "powerOn": 1
     *     }
     *   },
     *   "clientToken": "021909"
     * }
     *
     * Note the client token, which is optional for all Shadow updates. The client
     * token must be unique at any given time, but may be reused once the update is
     * completed. For this demo, a timestamp is used for a client token.
     */
    #define SHADOW_DESIRED_JSON     \
        "{"                         \
        "\"state\":{"               \
        "\"desired\":{"             \
        "\"powerOn\":%01d"          \
        "}"                         \
        "},"                        \
        "\"clientToken\":\"%06lu\"" \
        "}"

    /**
     * @brief The expected size of #SHADOW_DESIRED_JSON.
     *
     * Because all the format specifiers in #SHADOW_DESIRED_JSON include a length,
     * its full actual size is known by pre-calculation, here's the formula why
     * the length need to minus 3:
     * 1. The length of "%01d" is 4.
     * 2. The length of %06lu is 5.
     * 3. The actual length we will use in case 1. is 1 ( for the state of powerOn ).
     * 4. The actual length we will use in case 2. is 6 ( for the clientToken length ).
     * 5. Thus the additional size 3 = 4 + 5 - 1 - 6 + 1 (termination character).
     *
     * In your own application, you could calculate the size of the json doc in this way.
     */
    #define SHADOW_DESIRED_JSON_LENGTH (sizeof(SHADOW_DESIRED_JSON) - 3)

    #define PAYLOAD_PROB "$aws/things/" CONFIG_MQTT_CLIENT_IDENTIFIER "/shadow/name/payload-prod/update"

    #define MAX_UART_DATA_LENGTH 128


    /**
     * @brief Format string representing a Shadow document with a "reported" state.
     *
     * The real json document will look like this:
     * {
     *   "state": {
     *     "reported": {
     *       "powerOn": 1
     *     }
     *   },
     *   "clientToken": "021909"
     * }
     *
     * Note the client token, which is required for all Shadow updates. The client
     * token must be unique at any given time, but may be reused once the update is
     * completed. For this demo, a timestamp is used for a client token.
     */
    #define SHADOW_REPORTED_JSON    \
        "{"                         \
        "\"state\":{"               \
        "\"reported\":{"            \
        "\"powerOn\":%01d"          \
        "}"                         \
        "},"                        \
        "\"clientToken\":\"%06lu\"" \
        "}"

    /**
     * @brief The expected size of #SHADOW_REPORTED_JSON.
     *
     * Because all the format specifiers in #SHADOW_REPORTED_JSON include a length,
     * its full size is known at compile-time by pre-calculation. Users could refer to
     * the way how to calculate the actual length in #SHADOW_DESIRED_JSON_LENGTH.
     */
    #define SHADOW_REPORTED_JSON_LENGTH (sizeof(SHADOW_REPORTED_JSON) - 3)

    /**
     * @brief The maximum number of times to run the loop in this demo.
     *
     * @note The demo loop is attempted to re-run only if it fails in an iteration.
     * Once the demo loop succeeds in an iteration, the demo exits successfully.
     */
    #ifndef SHADOW_MAX_DEMO_LOOP_COUNT
    #define SHADOW_MAX_DEMO_LOOP_COUNT (3)
    #endif

    /**
     * @brief Time in seconds to wait between retries of the demo loop if
     * demo loop fails.
     */
    #define DELAY_BETWEEN_DEMO_RETRY_ITERATIONS_S (5)

    /**
     * @brief JSON key for response code that indicates the type of error in
     * the error document received on topic `/delete/rejected`.
     */
    #define SHADOW_DELETE_REJECTED_ERROR_CODE_KEY "code"

    /**
     * @brief Length of #SHADOW_DELETE_REJECTED_ERROR_CODE_KEY
     */
    #define SHADOW_DELETE_REJECTED_ERROR_CODE_KEY_LENGTH ((uint16_t)(sizeof(SHADOW_DELETE_REJECTED_ERROR_CODE_KEY) - 1))

    /*-----------------------------------------------------------*/

    /**
     * @brief The simulated device current power on state.
     */
    static uint32_t currentPowerOnState = 0;

    /**
     * @brief The flag to indicate the device current power on state changed.
     */
    static bool stateChanged = false;

    /**
     * @brief When we send an update to the device shadow, and if we care about
     * the response from cloud (accepted/rejected), remember the clientToken and
     * use it to match with the response.
     */
    static uint32_t clientToken = 0U;

    /**
     * @brief Indicator that an error occurred during the MQTT event callback. If an
     * error occurred during the MQTT event callback, then the demo has failed.
     */
    static bool eventCallbackError = false;

    /**
     * @brief Status of the response of Shadow delete operation from AWS IoT
     * message broker.
     */
    static bool deleteResponseReceived = false;

    /**
     * @brief Status of the Shadow delete operation.
     *
     * The Shadow delete status will be updated by the incoming publishes on the
     * MQTT topics for delete acknowledgement from AWS IoT message broker
     * (accepted/rejected). Shadow document is considered to be deleted if an
     * incoming publish is received on `/delete/accepted` topic or an incoming
     * publish is received on `/delete/rejected` topic with error code 404. Code 404
     * indicates that the Shadow document does not exist for the Thing yet.
     */
    static bool shadowDeleted = false;

    /*-----------------------------------------------------------*/

    /**
     * @brief This example uses the MQTT library of the AWS IoT Device SDK for
     * Embedded C. This is the prototype of the callback function defined by
     * that library. It will be invoked whenever the MQTT library receives an
     * incoming message.
     *
     * @param[in] pMqttContext MQTT context pointer.
     * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
     * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
     */
    static void eventCallback(MQTTContext_t *pMqttContext,
                            MQTTPacketInfo_t *pPacketInfo,
                            MQTTDeserializedInfo_t *pDeserializedInfo);

    /**
     * @brief Process payload from /update/delta topic.
     *
     * This handler examines the version number and the powerOn state. If powerOn
     * state has changed, it sets a flag for the main function to take further actions.
     *
     * @param[in] pPublishInfo Deserialized publish info pointer for the incoming
     * packet.
     */
    static void updateDeltaHandler(MQTTPublishInfo_t *pPublishInfo);

    /**
     * @brief Process payload from /update/accepted topic.
     *
     * This handler examines the accepted message that carries the same clientToken
     * as sent before.
     *
     * @param[in] pPublishInfo Deserialized publish info pointer for the incoming
     * packet.
     */
    static void updateAcceptedHandler(MQTTPublishInfo_t *pPublishInfo);

    /**
     * @brief Process payload from `/delete/rejected` topic.
     *
     * This handler examines the rejected message to look for the reject reason code.
     * If the reject reason code is `404`, an attempt was made to delete a shadow
     * document which was not present yet. This is considered to be success for this
     * demo application.
     *
     * @param[in] pPublishInfo Deserialized publish info pointer for the incoming
     * packet.
     */
    static void deleteRejectedHandler(MQTTPublishInfo_t *pPublishInfo);

    /*-----------------------------------------------------------*/

    static void deleteRejectedHandler(MQTTPublishInfo_t *pPublishInfo)
    {
    }

    /*-----------------------------------------------------------*/

    static void updateDeltaHandler(MQTTPublishInfo_t *pPublishInfo)
    {
        static uint32_t currentVersion = 0; /* Remember the latestVersion # we've ever received */
        uint32_t version = 0U;
        uint32_t newState = 0U;
        char *outValue = NULL;
        uint32_t outValueLength = 0U;
        JSONStatus_t result = JSONSuccess;

        assert(pPublishInfo != NULL);
        assert(pPublishInfo->pPayload != NULL);

        LogInfo(("/update/delta json payload:%s.", (const char *)pPublishInfo->pPayload));
    }

    /*-----------------------------------------------------------*/
    static void updateAcceptedHandler(MQTTPublishInfo_t *pPublishInfo)
    {
        char *outValue = NULL;
        uint32_t outValueLength = 0U;
        uint32_t receivedToken = 0U;
        JSONStatus_t result = JSONSuccess;

        assert(pPublishInfo != NULL);
        assert(pPublishInfo->pPayload != NULL);

        //** TODO
    }

    /*-----------------------------------------------------------*/

    /* This is the callback function invoked by the MQTT stack when it receives
    * incoming messages. This function demonstrates how to use the Shadow_MatchTopicString
    * function to determine whether the incoming message is a device shadow message
    * or not. If it is, it handles the message depending on the message type.
    */
    static void eventCallback(MQTTContext_t *pMqttContext,
                            MQTTPacketInfo_t *pPacketInfo,
                            MQTTDeserializedInfo_t *pDeserializedInfo)
    {
        printf("in callback\n");
        ShadowMessageType_t messageType = ShadowMessageTypeMaxNum;
        const char *pThingName = NULL;
        uint8_t thingNameLength = 0U;
        const char *pShadowName = NULL;
        uint8_t shadowNameLength = 0U;
        uint16_t packetIdentifier;

        (void)pMqttContext;

        assert(pDeserializedInfo != NULL);
        assert(pMqttContext != NULL);
        assert(pPacketInfo != NULL);

        packetIdentifier = pDeserializedInfo->packetIdentifier;

        /* Handle incoming publish. The lower 4 bits of the publish packet
        * type is used for the dup, QoS, and retain flags. Hence masking
        * out the lower bits to check if the packet is publish. */
        if ((pPacketInfo->type & 0xF0U) == MQTT_PACKET_TYPE_PUBLISH)
        {
            assert(pDeserializedInfo->pPublishInfo != NULL);

            /* Let the Device Shadow library tell us whether this is a device shadow message. */
            if (SHADOW_SUCCESS == Shadow_MatchTopicString(pDeserializedInfo->pPublishInfo->pTopicName,
                                                        pDeserializedInfo->pPublishInfo->topicNameLength,
                                                        &messageType,
                                                        &pThingName,
                                                        &thingNameLength,
                                                        &pShadowName,
                                                        &shadowNameLength))
            {
                /* Upon successful return, the messageType has been filled in. */
                if (messageType == ShadowMessageTypeGetAccepted)
                {
                    /* Handler function to process payload. */
                    LogInfo(("pPublishInfo->pTopicName:%s.", pDeserializedInfo->pPublishInfo->pTopicName));
                    updateAcceptedHandler(pDeserializedInfo->pPublishInfo);
                }
                else if (messageType == ShadowMessageTypeUpdateDelta)
                {
                    //  LogInfo(("pPublishInfo->pTopicName:%s.", pDeserializedInfo->pPublishInfo->pTopicName));
                    LogInfo(("payload:%s.", (const char *)pDeserializedInfo->pPublishInfo->pPayload));
                    updateDeltaHandler(pDeserializedInfo->pPublishInfo);
                }
                else
                {
                    LogInfo(("Other message type:%d !!", messageType));
                    LogInfo(("payload:%s.", (const char *)pDeserializedInfo->pPublishInfo->pPayload));
                }
            }
            else
            {
                LogError(("Shadow_MatchTopicString parse failed:%s !!", (const char *)pDeserializedInfo->pPublishInfo->pTopicName));
                eventCallbackError = true;
            }
        }
        else
        {
            HandleOtherIncomingPacket(pPacketInfo, packetIdentifier);
        }
    }

    /*-----------------------------------------------------------*/
    /**
     * @brief
     *
     * @param param
     * @return true
     * @return false
     */
    bool start_mqtt(void *param)
    {
        int returnStatus = EXIT_SUCCESS;

        returnStatus = EstablishMqttSession(eventCallback);

        if (returnStatus == EXIT_FAILURE)
        {
            /* Log error to indicate connection failure. */
            LogError(("Failed to connect to MQTT broker."));
            return false;
        }
        return true;
    }
    /**
     * @brief
     *
     */

    bool clean_mqtt(void *param)
    {
        printf("Clear MQTT \n");
        DisconnectMqttSession();
        return true;
    }
    /**
     * @brief
     *
     */
    bool is_mqtt_connected(void *param)
    {
        if (mqttContext.connectStatus == MQTTConnected)
        {
            return true;
        }
        return false;
    }
    /**
     * @brief
     *
     */

    
    void get_shadow_configration(void)
    {
        uint16_t returnStatus = 0;
        static char updateDocument[SHADOW_REPORTED_JSON_LENGTH + 1] = {0};
        /* First of all, try to delete any Shadow document in the cloud.
        * Try to subscribe to `/delete/accepted` and `/delete/rejected` topics. */
        returnStatus = SubscribeToTopic(SHADOW_TOPIC_STR_GET_ACC(THING_NAME, SHADOW_NAME),
                                        SHADOW_TOPIC_LEN_GET_ACC(THING_NAME_LENGTH, SHADOW_NAME_LENGTH));

        if (returnStatus == EXIT_SUCCESS)
        {
            /* Publish to Shadow `delete` topic to attempt to delete the
            * Shadow document if exists. */
            returnStatus = PublishToTopic(SHADOW_TOPIC_STR_GET(THING_NAME, SHADOW_NAME),
                                        SHADOW_TOPIC_LEN_GET(THING_NAME_LENGTH, SHADOW_NAME_LENGTH),
                                        updateDocument,
                                        0U);
            Sleep(1);
            returnStatus = UnsubscribeFromTopic(SHADOW_TOPIC_STR_GET_ACC(THING_NAME, SHADOW_NAME),
                                                SHADOW_TOPIC_LEN_GET_ACC(THING_NAME_LENGTH, SHADOW_NAME_LENGTH));

            returnStatus = SubscribeToTopic(SHADOW_TOPIC_STR_UPDATE_DELTA(THING_NAME, SHADOW_NAME),
                                            SHADOW_TOPIC_LEN_UPDATE_DELTA(THING_NAME_LENGTH, SHADOW_NAME_LENGTH));
        }
    }
    /**
     * @brief
     *
     * @param payload
     */
    uint8_t publish_payload_prob(char *payload)
    {
        int status = 0;
        if (mqttContext.connectStatus == MQTTConnected)
        {
            status = PublishToTopic(PAYLOAD_PROB,
                                    strlen(PAYLOAD_PROB),
                                    payload,
                                    strlen(payload));
            cleanupOutgoingPublishes();
        }
        uint8_t ret = (status == EXIT_SUCCESS) ? 1 : 0;

        return ret;
    }
    /**
     * @brief
     *
     */
    void init_classic_shadow()
    {
        uint8_t tries = 0;

        while (1)
        {
            tries++;
            if (start_mqtt(NULL))
            {
                get_shadow_configration();
                break;
            }
            else
            {
                if (tries == 6)
                {
                    esp_restart();
                }
            }
            Sleep(4);
        }
    }
    /**
     * @brief Entry point of shadow demo.
     *
     * This main function demonstrates how to use the macros provided by the
     * Device Shadow library to assemble strings for the MQTT topics defined
     * by AWS IoT Device Shadow. Named shadow topic strings differ from unnamed
     * ("Classic") topic strings as indicated by the tokens within square brackets.
     *
     * The main function uses these macros for topics to subscribe to:
     * - SHADOW_TOPIC_STR_UPDATE_DELTA for "$aws/things/thingName/shadow[/name/shadowname]/update/delta"
     * - SHADOW_TOPIC_STR_UPDATE_ACC for "$aws/things/thingName/shadow[/name/shadowname]/update/accepted"
     * - SHADOW_TOPIC_STR_UPDATE_REJ for "$aws/things/thingName/shadow[/name/shadowname]/update/rejected"
     *
     * It also uses these macros for topics to publish to:
     * - SHADOW_TOPIC_STR_DELETE for "$aws/things/thingName/shadow[/name/shadowname]/delete"
     * - SHADOW_TOPIC_STR_UPDATE for "$aws/things/thingName/shadow[/name/shadowname]/update"
     *
     * The helper functions this demo uses for MQTT operations have internal
     * loops to process incoming messages. Those are not the focus of this demo
     * and therefore, are placed in a separate file shadow_demo_helpers.c.
     */


void extractValues(const char* input, char* phValue, char* conductivityValue, size_t bufferSize)
{
    char* token;
    char* context;

    // Extract the value between the first and second hash (pH value)
    token = strtok_r((char*)input, "#", &context);
    if (token != NULL) {
        token = strtok_r(NULL, "#", &context);
        if (token != NULL) {
            strncpy(phValue, token, bufferSize - 1);
            phValue[bufferSize - 1] = '\0';
        }
    }

    // Extract the value between the second and third hash (conductivity value)
    if (token != NULL) {
        token = strtok_r(NULL, "#", &context);
        if (token != NULL) {
            strncpy(conductivityValue, token, bufferSize - 1);
            conductivityValue[bufferSize - 1] = '\0';
        }
    }
}

void createPayload(char** test_payload) {
    char phValue[5];  // Buffer size should accommodate the expected value length + 1 for null termination
    char conductivityValue[5];

    char uart_data[MAX_UART_DATA_LENGTH];
    char tmprawdata[MAX_UART_DATA_LENGTH];

    get_uart_data(uart_data, MAX_UART_DATA_LENGTH);

    // rx_task(rawdata);
    // strcpy(uart_data, "31gpd01&WF#0733#0236#000#0#0000.0#0000.0#00000#00000#000#000#00000#00000#0000.0#0000.0#00000#00000#valuerend"); // Copy data to rxdata
    
    strcpy(tmprawdata, uart_data); // Copy data to rxdata
    extractValues(tmprawdata, phValue, conductivityValue, sizeof(phValue));

    cJSON *root, *reported, *report;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "state",  reported = cJSON_CreateObject());
    cJSON_AddItemToObject(reported, "reported", report = cJSON_CreateObject());
    cJSON_AddItemToObject(report, "ts", cJSON_CreateNumber(GetStandardTime()));
    cJSON_AddItemToObject(report, "device_id", cJSON_CreateString(CONFIG_MQTT_CLIENT_IDENTIFIER));
    //cJSON_AddItemToObject(report, "data_packet_nr", cJSON_CreateNumber(5));
    cJSON_AddItemToObject(report, "ph", cJSON_CreateString(phValue));
    cJSON_AddItemToObject(report, "conductivity", cJSON_CreateString(conductivityValue));
    cJSON_AddItemToObject(report, "cpu_temp", cJSON_CreateNumber(45));
    cJSON_AddItemToObject(report, "rawdat", cJSON_CreateString(uart_data));
    cJSON_AddItemToObject(report, "rssi", cJSON_CreateNumber(-50));
    cJSON_AddItemToObject(report, "reset_reasons", cJSON_CreateNumber(4));

    /* print everything */
    *test_payload = cJSON_Print(root);

    /* free all objects under root and root itself */
    cJSON_Delete(root);
}


    int aws_shadow_main(int argc,
                        char **argv)
    {
        char* test_payload = NULL;
        bool disconnect_occur = false;

        init_classic_shadow();

        while (1)
        {
            ldo_on();
            createPayload(&test_payload); //* get data and prepare payload

            if (wifi_sta && disconnect_occur)
            {
                buzzer_play_error_tune();
                disconnect_occur = false;
                Sleep(2);
                clean_mqtt(NULL);
                init_classic_shadow();
            }
            else if (wifi_sta)
            {

                //** if wifi connected

                if (is_mqtt_connected(NULL))
                {

                    if (publish_payload_prob(test_payload))
                    {
                        buzzer_play_heartbeat();
                        feed_watchdog = true;
                        //Sleep(device_config.publish_interval*1000*1000);
                        ldo_off();
                        Sleep(30);
                        // esp_sleep_enable_timer_wakeup(device_config.publish_interval*1000*1000);
                        // esp_deep_sleep_start();
                    }
                    else
                    {
                        feed_watchdog = false;
                    }
                }
                else
                {
                    buzzer_play_error_tune();
                    clean_mqtt(NULL);
                    init_classic_shadow();
                }
            }
            else
            {
                buzzer_play_error_tune();
                disconnect_occur = true;
            }
            Sleep(5);
        }
    }