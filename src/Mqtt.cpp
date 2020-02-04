#include "Mqtt.h"

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

bool MQTTConnected() {
    return mqttClient.connected();
}

boolean reconnect() {
    if (WiFi.status() != WL_CONNECTED || strlen(config.mqttIp) == 0)
        return false;

    mqttClient.connect(config.mqttIp, config.mqttUser, config.mqttPassword);

    return MQTTConnected();
}

void callbackMqtt(char *topic, byte *payload, unsigned int length) {
    Serial.println("MQTT Message received");
    char *payload_as_string = (char *)malloc(length + 1);
    memcpy(payload_as_string, (char *)payload, length);
    payload_as_string[length] = 0;

    Serial.printf("MQTT Topic: %s\n", topic);
    Serial.printf("MQTT Payload: %s\n", payload_as_string);

    // TODO Call processing functions
    //processMqttAction(topic, payload_as_string);

    free(payload_as_string);
}

void setupMQTT() {
    if (MQTTConnected()) {
        mqttClient.disconnect();
    }
    mqttClient.setServer(config.mqttIp, config.mqttPort);
    mqttClient.setCallback(callbackMqtt);
}

void loopMQTT() {
    if (WiFi.status() != WL_CONNECTED || strlen(config.mqttIp) == 0)
        return;

    static unsigned long lastReconnectAttempt = millis();
    if (!MQTTConnected()) {
        long now = millis();
        if (now - lastReconnectAttempt > 8000) {
            Serial.print("Attempting MQTT connection...");
            lastReconnectAttempt = now;
            if (reconnect()) {
                Serial.println("MQTT connected!");
                lastReconnectAttempt = 0;
            }
        }
    }
    else {
        mqttClient.loop();
    }
}

void publishOnMqtt(const char *topic, const char *payload, bool retain) {
    if (!MQTTConnected()) {
        return;
    }

    static unsigned int retries = 0;
    while (!mqttClient.publish(topic, payload, retain) && retries < 3) {
        retries++;
    }
    retries = 0;
}

void subscribeOnMqtt(const char *topic) {
    if (MQTTConnected()) {
        mqttClient.subscribe(topic);
    }
}

void unsubscribeOnMqtt(const char *topic) {
    if (MQTTConnected()) {
        mqttClient.unsubscribe(topic);
    }
}
