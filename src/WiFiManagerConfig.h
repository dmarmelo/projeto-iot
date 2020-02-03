#ifndef WIFI_MANAGER_CONFIG_H
#define WIFI_MANAGER_CONFIG_H

#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>            //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

void setupWiFiManager(AsyncWebServer * server, DNSServer *dns);
void loopWiFiManager();

#endif