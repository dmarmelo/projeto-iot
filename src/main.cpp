#include <Arduino.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>

//#include "WiFiManagerConfig.h"
#include "Configuration.h"
#include "JustWifiConfig.h"
#include "Mqtt.h"

AsyncWebServer server(80);
DNSServer dns;

void handleRoot(AsyncWebServerRequest *request) {
    Serial.println("Request " + request->host() + "-" + request->url());
    //request->send(200, "application/json", "{\"result\":\"OK\"}");
    request->send(200, "text/html", "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>HEHE</title></head><body><h1>Hello World!<h1></body></html>");
}

void setup() {
    Serial.begin(115200);
    Serial.println("Heeloo");

    //setupWiFiManager(&server, &dns);
    config.load();
    setupJustWifi();
    setupMQTT();

    pinMode(A0, INPUT);

    /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"result\":\"OK\"}");
    });*/
    server.on("/", handleRoot);

    server.begin();
}

void loop() {
    //loopWiFiManager();
    loopJustWifi();
    loopMqtt();


    // Small delay to give some breath
    delay(10);
}
