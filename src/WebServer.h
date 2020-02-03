#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include "Configuration.h"
#include "StaticHtml.h"
#include "StaticJs.h"
#include "StaticCss.h"

void setupWebserverAsync();

#endif // WEB_SERVER_H