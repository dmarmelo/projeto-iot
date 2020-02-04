#ifndef JUST_WIFI_CONFIG_H
#define JUST_WIFI_CONFIG_H

#include <Arduino.h>
#include <JustWifi.h>
#include "JustWifiUtils.h"
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include "Configuration.h"

extern unsigned long connectedOn;
extern DNSServer dnsServer;

String getApName();

void setupJustWifi();
void loopJustWifi();
void reloadWiFiConfig();

void mdnsCallback(justwifi_messages_t code, char *parameter);
void captivePortalCallback(justwifi_messages_t code, char *parameter);
void refreshMDNS(const char* lastName);

size_t systemJSONStatus(Print &output);

#endif // JUST_WIFI_CONFIG_H