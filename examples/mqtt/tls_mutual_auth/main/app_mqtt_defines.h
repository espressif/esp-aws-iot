/**
 * @brief Structure to keep the MQTT publish packets until an ack is received
 * for QoS1 publishes.
 */
typedef struct PublishPackets
{
    /**
     * @brief Packet identifier of the publish packet.
     */
    uint16_t packetId;

    /**
     * @brief Publish info of the publish packet.
     */
    MQTTPublishInfo_t pubInfo;
} PublishPackets_t;

typedef enum
{
    ESTABLISH_BROKER_TLS_CONNC,
    ESTABLISH_MQTT_CONNC,
    ACTION_ON_PEND_PUBLISHES,
    SUBSCRIBE_TO_TOPICS,
    PUBLISH_AND_RECV_FROM_CLOUD
}APP_COMMUNICATION_STATES;

typedef struct sAppCloudMsg
{
    PublishPackets_t PublishPacket;
}AppCloudMsg_t;
