#include <Arduino.h>
#include "Configuration.h"
//#include "WiFiManagerConfig.h"
#include "JustWifiConfig.h"
#include "WebServer.h"
#include "Mqtt.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Heeloo");

    //setupWiFiManager(&server, &dns);
    config.load();
    setupJustWifi();
    setupWebserverAsync();
    setupMQTT();

    pinMode(A0, INPUT);
}

void loop() {
    //loopWiFiManager();
    loopJustWifi();
    loopMqtt();


    // Small delay to give some breath
    delay(10);
}
