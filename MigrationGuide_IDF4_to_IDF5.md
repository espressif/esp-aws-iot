# Migration Guide from IDF 4 to IDF 5 for esp-aws-iot

This guide provides detailed steps and examples for migrating applications using functions in `aws_iot_mqtt_client.c` to the new coreMQTT component. Follow the instructions below to ensure a smooth transition.

## 1. Update Dependencies

Ensure that your project is using the latest version of the coreMQTT library. Update your project's dependencies to include coreMQTT.

## 2. Update Configuration

Update your project's configuration to use coreMQTT. This may involve updating your project's `sdkconfig` file or other configuration files.

## 3. Update Code

### 3.1 Replace `aws_iot_mqtt_client.c` Functions

Replace the functions in `aws_iot_mqtt_client.c` with their equivalents in coreMQTT. Below are some examples of common function replacements:

**Old Code Snippet**:
```c
aws_iot_mqtt_init(&mqttClient, &mqttInitParams);
aws_iot_mqtt_connect(&mqttClient, &connectParams);
aws_iot_mqtt_subscribe(&mqttClient, topic, topicLen, qos, messageHandler, pData);
aws_iot_mqtt_publish(&mqttClient, topic, topicLen, &params);
aws_iot_mqtt_disconnect(&mqttClient);
```

**New Code Snippet**:
```c
MQTT_Init(&mqttContext, &networkContext, &mqttCallbacks, &mqttBuffer);
MQTT_Connect(&mqttContext, &connectInfo, &willInfo, timeoutMs, &sessionPresent);
MQTT_Subscribe(&mqttContext, &subscriptionList, subscriptionCount, packetId);
MQTT_Publish(&mqttContext, &publishInfo, packetId);
MQTT_Disconnect(&mqttContext);
```

### 3.2 Update Network Context

Update the `NetworkContext` struct to match the new coreMQTT requirements. Below is an example of the changes:

**Old Code Snippet**:
```c
NetworkContext_t networkContext;
networkContext.pParams = &tlsParams;
```

**New Code Snippet**:
```c
NetworkContext_t networkContext;
networkContext.pParams = &transportParams;
```

### 3.3 Update Callback Functions

Update any callback functions to match the new coreMQTT function signatures. Below is an example of the changes:

**Old Code Snippet**:
```c
void messageHandler(AWS_IoT_Client *pClient, char *topicName, uint16_t topicNameLen, IoT_Publish_Message_Params *params, void *pData) {
    // Handle message
}
```

**New Code Snippet**:
```c
void eventCallback(MQTTContext_t *pContext, MQTTPacketInfo_t *pPacketInfo, MQTTDeserializedInfo_t *pDeserializedInfo) {
    // Handle event
}
```

## 4. Test and Validate

After making the necessary changes, thoroughly test your application to ensure that it works correctly with the new coreMQTT component. Validate that all functionality is working as expected.

## 5. Update Documentation

Update any relevant documentation to reflect the changes made during the migration process. This may include updating README files, user guides, or other documentation.

By following these steps, you can successfully migrate your application from IDF 4 to IDF 5, transitioning from `aws_iot_mqtt_client.c` to the new coreMQTT component.
