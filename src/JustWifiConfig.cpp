#include "JustWifiConfig.h"

unsigned long connectedOn = 0ul;

String getApName() {
  return "ProjIoT-" + String(config.nodeId);
}

void setupJustWifi() {
    // Set WIFI hostname (otherwise it would be ESP-XXXXXX)
    jw.setHostname(config.nodeId);

    // Callbacks
    if (connectedOn == 0ul) {
        jw.subscribe(infoCallback);
        jw.subscribe(mdnsCallback);
        jw.subscribe(captivePortalCallback);
    }

    // AP mode only as fallback
    jw.setSoftAP(getApName().c_str());
    jw.enableAP(false);
    jw.enableAPFallback(true);

    // Enable STA mode (connecting to a router)
    jw.enableSTA(true);

    // Clean and add network from configuration
    jw.cleanNetworks();
    jw.addNetwork(config.wifiSSID, config.wifiPassword);

    Serial.println("[WIFI] Connecting Wifi...");
}

void loopJustWifi() {
    if ((WiFi.getMode() & WIFI_AP) && WiFi.isConnected() && connectedOn + 60000 < millis()) {
        jw.enableAP(false);
    }
    // This call takes care of it all
    jw.loop();
    dnsServer.processNextRequest();
    MDNS.update();
}

void reloadWiFiConfig() {
    jw.disconnect();
    setupJustWifi();
}

// -----------------------------------------------------------------------------
// mDNS
// -----------------------------------------------------------------------------
void mdnsCallback(justwifi_messages_t code, char *parameter) {
    if (code == MESSAGE_CONNECTED) {
        connectedOn = millis();
        refreshMDNS(config.nodeId);
    }
}

// -----------------------------------------------------------------------------
// Captive portal
// -----------------------------------------------------------------------------
DNSServer dnsServer;

void captivePortalCallback(justwifi_messages_t code, char *parameter) {
    if (code == MESSAGE_ACCESSPOINT_CREATED) {
        if (!WiFi.isConnected()) {
            Serial.println("Starting DNS Server");
            dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
            dnsServer.start(53, "*", WiFi.softAPIP());
        }
    }

    if (code == MESSAGE_CONNECTED) {
        Serial.println("Stopping DNS Server");
        dnsServer.stop();
    }
}

void refreshMDNS(const char* lastName) {
    MDNS.removeService(lastName, "ProjIoT", "tcp");
    MDNS.close();
    String hostname = String(WiFi.hostname());
    if (MDNS.begin(hostname, INADDR_ANY, 10)) {
        MDNS.addService("http", "tcp", 80);
        MDNS.addServiceTxt("ProjIoT", "tcp", "hardwareId", String(ESP.getChipId()).c_str());
    }
    else {
        Serial.printf("MDNS Error");
    }
}
