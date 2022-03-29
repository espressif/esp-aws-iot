# Amazon Web Services IoT MQTT (TLS Mutual Authentication) Example

This is an adaptation of the [AWS IoT C SDK](https://github.com/aws/aws-iot-device-sdk-embedded-C) "mqtt_demo_mutual_auth" example for ESP-IDF.

# Provisioning/Configuration

See the README.md in the parent directory for information about configuring the AWS IoT examples.

Few additional steps to be carried out:  

## Find & Set AWS Endpoint Hostname

Your AWS IoT account has a unique endpoint hostname to connect to. To find it, open the AWS IoT Console and click the "Settings" button on the bottom left side. The endpoint hostname is shown under the "Custom Endpoint" heading on this page.

Run `idf.py menuconfig`. Under `Example Configuration`, set the `MQTT Broker Endpoint` to the host name.

## Set Client ID

Run `idf.py menuconfig`. Under `Example Configuration`, set the `MQTT Client ID` to a unique value.

The Client ID is used in the MQTT protocol used to send messages to/from AWS IoT. AWS IoT requires that each connected device within a single AWS account uses a unique Client ID. Other than this restriction, the Client ID can be any value that you like. The example default should be fine if you're only connecting one ESP32 at a time.

In a production IoT app this ID would be set dynamically, but for these examples it is compiled in via menuconfig.

## (Optional) Locally Check The Root Certificate

The Root CA certificate provides a root-of-trust when the ESP32 connects to AWS IoT. We have supplied the root CA certificate already (in PEM format) in the file `main/certs/root_cert_auth.pem`.

If you want to locally verify that this Root CA certificate hasn't changed, you can run the following command against your AWS MQTT Host:

```
openssl s_client -showcerts -connect hostname:8883 < /dev/null
```

(Replace hostname with your AWS MQTT endpoint host.) The Root CA certificate is the last certificate in the list of certificates printed. You can copy-paste this in place of the existing `root_cert_auth.pem` file.

# Console

After flashing the example to your ESP32, it should connect to Amazon and start subscriping and publishing to example/topic.

In the ESP32's serial output, you should see the logs every couple of seconds.

```
[INFO] [MQTT_DEMO] [subscribePublishLoop:1342] Creating an MQTT connection to a2hokgmhoqmk0-ats.iot.us-east-1.amazonaws.com.
[INFO] [MQTT_DEMO] [establishMqttSession:1111] MQTT connection successfully established with broker.

[INFO] [MQTT_DEMO] [subscribePublishLoop:1374] A clean MQTT connection is established. Cleaning up all the stored outgoing publishes.

[INFO] [MQTT_DEMO] [subscribePublishLoop:1392] Subscribing to the MQTT topic thingname/example/topic.
[INFO] [MQTT_DEMO] [subscribeToTopic:1175] SUBSCRIBE sent for topic thingname/example/topic to broker.

[INFO] [MQTT_DEMO] [eventCallback:998] Subscribed to the topic thingname/example/topic. with maximum QoS 1.

[INFO] [MQTT_DEMO] [subscribePublishLoop:1436] Sending Publish to the MQTT topic thingname/example/topic.
[INFO] [MQTT_DEMO] [publishToTopic:1273] PUBLISH sent for topic thingname/example/topic to broker with packet ID 2.
```
# Troubleshooting

If you're having problems with the AWS IoT connection itself, check the Troubleshooting section of the README in the parent directory.
