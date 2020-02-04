#include <Arduino.h>
#include "Configuration.h"
#include "JustWifiConfig.h"
#include "WebServer.h"
#include "Mqtt.h"
#include "Sensors.h"

void setup() {
    Serial.begin(115200);
    
    config.load();
    setupJustWifi();
    setupWebserverAsync();
    setupMQTT();
    
    sensors.setup();
}

void loop() {
    if (config.updated()) {
        // Update Services
        refreshMDNS(config.lastNodeId);
        reloadWiFiConfig();
        setupMQTT();
    }

    loopJustWifi();
    loopMQTT();

    sensors.loop();
}
