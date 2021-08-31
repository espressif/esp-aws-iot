# Amazon Web Services IoT Thing Shadow Example

This is an adaptation of the [AWS IoT C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C) "shadow_demo_main" example for ESP-IDF.

# Provisioning/Configuration

See the README.md in the parent directory for information about configuring the AWS IoT examples.

Few additional steps to be carried out:  

## Find & Set AWS Endpoint Hostname

Your AWS IoT account has a unique endpoint hostname to connect to. To find it, open the AWS IoT Console and click the "Settings" button on the bottom left side. The endpoint hostname is shown under the "Custom Endpoint" heading on this page.

Run `idf.py menuconfig`. Under `Example Configuration`, set the `MQTT Broker Endpoint` to the host name.

## Set Thing Name

Run `idf.py menuconfig`. Under `Example Configuration`, set the `MQTT Client Identifier` to the thing name.

The Thing Name should match a Thing that you created while following the Getting Started guide (to check the Things you have registered, go to the AWS IoT console web interface, click Registry and then click Things).

## (Optional) Locally Check The Root Certificate

The Root CA certificate provides a root-of-trust when the ESP32 connects to AWS IoT. We have supplied the root CA certificate already (in PEM format) in the file `main/certs/root_cert_auth.pem`.

If you want to locally verify that this Root CA certificate hasn't changed, you can run the following command against your AWS MQTT Host:

```
openssl s_client -showcerts -connect hostname:8883 < /dev/null
```

(Replace hostname with your AWS MQTT endpoint host.) The Root CA certificate is the last certificate in the list of certificates printed. You can copy-paste this in place of the existing `root_cert_auth.pem` file.


# Monitoring Thing Status

After flashing the example to your ESP32, it should connect to Amazon and start updating the example Thing's shadow.

In the ESP32's serial output, you should see the logs every couple of seconds.

```
[INFO] [ShadowDemo] [EstablishMqttSession:683] MQTT connection successfully established with broker.
[INFO] [ShadowDemo] [EstablishMqttSession:703] An MQTT session with broker is re-established. Resending unacked publishes.
[INFO] [ShadowDemo] [SubscribeToTopic:795] SUBSCRIBE topic $aws/things/thingname/shadow/delete/accepted to broker.
[INFO] [ShadowDemo] [HandleOtherIncomingPacket:505] MQTT_PACKET_TYPE_SUBACK.
[INFO] [ShadowDemo] [SubscribeToTopic:795] SUBSCRIBE topic $aws/things/thingname/shadow/delete/rejected to broker.
[INFO] [ShadowDemo] [HandleOtherIncomingPacket:505] MQTT_PACKET_TYPE_SUBACK.
[INFO] [ShadowDemo] [PublishToTopic:908] Published payload: 
[INFO] [ShadowDemo] [PublishToTopic:936] PUBLISH sent for topic $aws/things/thingname/shadow/delete to broker with packet ID 3.
[INFO] [ShadowDemo] [HandleOtherIncomingPacket:526] PUBACK received for packet id 3.
[INFO] [ShadowDemo] [cleanupOutgoingPublishWithPacketID:490] Cleaned up outgoing publish packet with packet id 3.
[INFO] [SHADOW] [eventCallback:579] pPublishInfo->pTopicName:$aws/things/thingname/shadow/delete/accepted.

```

You can monitor the Thing status from the AWS IoT console web interface:

* On the left-hand toolbar, click Registry and then click Things.
* Click on the "Thing" you set up for the example.
* Click on the "Shadow" sidebar link. You should see the Thing Shadow updating regularly.
* Click on the "Activity" sidebar link to see all Thing Shadow updates in a list. You can examine each update individually.

# Troubleshooting

If you're having problems with the AWS IoT connection itself, check the Troubleshooting section of the README in the parent directory.

* If your Thing is connecting and appears to be successfully updating, but you don't see any updates in the AWS IoT console, then check that the `MQTT Client Identifier` in the Example Configuration under menuconfig matches exactly the thing name in AWS IoT console (including case).
