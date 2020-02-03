#include "JustWifiConfig.h"

unsigned long connectedOn = 0ul;

String getApName() {
  return "DmEsIoT-" + String(ESP.getChipId());
}

void setupJustWifi() {
    // Set WIFI hostname (otherwise it would be ESP-XXXXXX)
    // TODO ir buscar às configurações
    jw.setHostname("justwifi");

    // Callbacks
    jw.subscribe(infoCallback);
    jw.subscribe(mdnsCallback);
    jw.subscribe(captivePortalCallback);

    // AP mode only as fallback
    // TODO ir buscar às configurações
    jw.setSoftAP(getApName().c_str());
    jw.enableAP(false);
    jw.enableAPFallback(true);

    // Enable STA mode (connecting to a router)
    jw.enableSTA(true);

    jw.addNetwork("Pidgeon", "pombopombo");

    Serial.println("[WIFI] Connecting Wifi...");

    reloadWiFiConfig();
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

}

// -----------------------------------------------------------------------------
// mDNS
// -----------------------------------------------------------------------------
void mdnsCallback(justwifi_messages_t code, char *parameter) {
    if (code == MESSAGE_CONNECTED) {
        refreshMDNS(getApName().c_str());
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
    MDNS.removeService(lastName, "DmEsIoT", "tcp");
    MDNS.close();
    String hostname = String(WiFi.hostname());
    if (MDNS.begin(hostname, INADDR_ANY, 10)) {
        MDNS.addService("http", "tcp", 80);
        MDNS.addServiceTxt("DmEsIoT", "tcp", "hardwareId", String(ESP.getChipId()).c_str());
    }
    else {
        Serial.printf("MDNS Error");
    }
}

size_t systemJSONStatus(Print &output) {
    const size_t CAPACITY = JSON_OBJECT_SIZE(13) + 400;
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object["wifiIp"] = WiFi.localIP().toString();
    object["wifiAPSSID"] = jw.getAPSSID();
    object["wifiSSID"] = WiFi.SSID();
    object["status"] = jw.connected();
    object["signal"] = WiFi.RSSI();
    object["wifiMask"] = WiFi.subnetMask().toString();
    object["wifiGw"] = WiFi.gatewayIP().toString();
    object["mac"] = WiFi.softAPmacAddress();
    object["channel"] = WiFi.channel();
    object["mode"] = (int)WiFi.getMode();
    //object["mqttConnected"] = mqttConnected();
    object["freeHeap"] = String(ESP.getFreeHeap());
    return serializeJson(doc, output);
}
