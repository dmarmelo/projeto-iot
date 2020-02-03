#include "WebServer.h"

AsyncWebServer server(80);

void setupWebserverAsync() {
    // HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html, sizeof(index_html));
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=600");
        request->send(response);
    });

    // JS
    server.on("/js/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", index_js, sizeof(index_js));
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=600");
        request->send(response);
    });

    server.on("/js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", jquery_min_js, sizeof(jquery_min_js));
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=600");
        request->send(response);
    });

    // CSS
    server.on("/css/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", bootstrap_min_css, sizeof(bootstrap_min_css));
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Cache-Control", "max-age=600");
        request->send(response);
    });

    //-------- API ---------
    // Get configuration
    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        config.serializeToJson(*response);
        request->send(response);
    });

    // Save Configuration
    server.addHandler(new AsyncCallbackJsonWebHandler("/save-config", [](AsyncWebServerRequest *request, JsonVariant json) {
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        config.updateFromJson(json);
        config.save();
        // TODO trigger wifi reload
        config.serializeToJson(*response);
        request->send(response);
    }));

    server.onNotFound([](AsyncWebServerRequest *request) {
        //CORS
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        }
        else {
            request->send(404);
        }
    });

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), F("POST, PUT, GET"));
    DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("Content-Type, Origin, Referer, User-Agent"));

    server.begin();
}
