#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"

#define CONFIGURATION_FILE "config.bin"

class Configuration {
private:
    bool _updated = false;

    void writeToFile(File &file);
    void readFromFile(File &file);
    
public:
    Configuration();
    ~Configuration();

    char nodeId[32];
    char lastNodeId[32];

    char mqttIp[40];
    int mqttPort;
    char mqttUser[32];
    char mqttPassword[24];
    char mqttTopic[32];

    char wifiSSID[32];
    char wifiPassword[24];

    char building[32];
    char room[32];

    void load();
    void save();
    size_t serializeToJson(Print &output);
    void updateFromJson(JsonObject doc);

    bool updated();

};

extern Configuration config;

#endif // CONFIG_H