#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Configuration.h"


bool MQTTConnected();

void setupMQTT();
void loopMQTT();

void publishOnMqtt(const char *topic, const char *payload, bool retain);
void subscribeOnMqtt(const char *topic);
void unsubscribeOnMqtt(const char *topic);

#endif // MQTT_H