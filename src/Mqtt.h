#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void setupMQTT();
void loopMqtt();

bool mqttConnected();

void publishOnMqtt(const char *topic, const char *payload, bool retain);
void subscribeOnMqtt(const char *topic);
void unsubscribeOnMqtt(const char *topic);


#endif // MQTT_H