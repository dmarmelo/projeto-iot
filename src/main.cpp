#include <Arduino.h>
#include <ESP8266mDNS.h>
//#include <JustWifi.h>
//#include "JustWifiUtils.h"

#include <ESPAsyncWebServer.h>            //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

AsyncWebServer server(80);
DNSServer dns;

/*void mdnsCallback(justwifi_messages_t code, char *parameter) {
  if (code == MESSAGE_CONNECTED) {
    Serial.println("Setting up mdns");
    MDNS.removeService("projeto-iot-teste", "projeto-iot-teste", "tcp");
    MDNS.close();
    if (MDNS.begin("projeto-iot-teste", INADDR_ANY, 10)) {
      MDNS.addService("http", "tcp", 80);
    }
  }
}*/

void setup() {
  Serial.begin(115200);
  Serial.println("Heelo");

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  AsyncWiFiManager wifiManager(&server,&dns);
  WiFi.hostname("projeto-iot-teste");

  //reset saved settings
  //wifiManager.resetSettings();

  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("projeto-iot");
  //or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  if (!MDNS.begin("projeto-iot-teste")) {
    Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);

  /*delay(2000);
  Serial.println();
  Serial.println();

  // -------------------------------------------------------------------------

  // Set WIFI hostname (otherwise it would be ESP-XXXXXX)
  jw.setHostname("justwifi");

  // Callbacks
  jw.subscribe(infoCallback);
  jw.subscribe(mdnsCallback);

  // -------------------------------------------------------------------------

  // AP mode only as fallback
  //jw.setSoftAP("iot-teste");
  jw.enableAP(false);
  jw.enableAPFallback(true);

  // -------------------------------------------------------------------------

  // Enable STA mode (connecting to a router)
  jw.enableSTA(true);

  // Clean existing network configuration
  //jw.cleanNetworks();
  jw.addNetwork("Pidgeon", "pombopombo");
  // -------------------------------------------------------------------------

  Serial.println("[WIFI] Connecting Wifi...");*/



  pinMode(A0, INPUT);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"result\":\"OK\"}");
  });

  server.begin();
}

void loop() {
  // This call takes care of it all
  //jw.loop();
  MDNS.update();
  // Small delay to give some breath
  delay(10);
}