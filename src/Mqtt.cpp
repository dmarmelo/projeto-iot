#include "Mqtt.h"

static WiFiClient espClient;
static PubSubClient mqttClient(espClient);

boolean reconnect() {
    if (WiFi.status() != WL_CONNECTED)
        return false;

    mqttClient.connect(String(ESP.getChipId()).c_str(), "", "");

    return mqttConnected();
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
    if (mqttConnected()) {
        mqttClient.disconnect();
    }
    mqttClient.setServer("bifrost.local", 1883);
    mqttClient.setCallback(callbackMqtt);
}

void loopMqtt() {
    if (WiFi.status() != WL_CONNECTED)
        return;

    static unsigned long lastReconnectAttempt = millis();
    if (!mqttConnected()) {
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
    else
    {
        mqttClient.loop();
    }
}

void publishOnMqtt(const char *topic, const char *payload, bool retain) {
    if (!mqttConnected()) {
        return;
    }

    static unsigned int retries = 0;
    while (!mqttClient.publish(topic, payload, retain) && retries < 3) {
        retries++;
    }
    retries = 0;
}

bool mqttConnected() {
    return mqttClient.connected();
}

void subscribeOnMqtt(const char *topic) {
    if (!mqttConnected()) {
        return;
    }
    mqttClient.subscribe(topic);
}

void unsubscribeOnMqtt(const char *topic) {
    if (mqttConnected()) {
        mqttClient.unsubscribe(topic);
    }
}
