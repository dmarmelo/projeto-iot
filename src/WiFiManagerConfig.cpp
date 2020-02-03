#include "WiFiManagerConfig.h"

void setupWiFiManager(AsyncWebServer *server, DNSServer *dns) {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wifiManager(server, dns);

    //reset saved settings
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setTimeout(180);

    WiFi.hostname("projeto-iot-teste");

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
}

void loopWiFiManager() {
    MDNS.update();
}
