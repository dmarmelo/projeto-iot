#include "Configuration.h"

Configuration::Configuration() {

}

Configuration::~Configuration() {

}

void Configuration::writeToFile(File &file) {
    file.write((uint8_t *)nodeId, sizeof(nodeId));
    file.write((uint8_t *)mqttIp, sizeof(mqttIp));
    file.write((uint8_t *)&mqttPort, sizeof(mqttPort));
    file.write((uint8_t *)mqttUsername, sizeof(mqttUsername));
    file.write((uint8_t *)mqttPassword, sizeof(mqttPassword));
    file.write((uint8_t *)mqttTopic, sizeof(mqttTopic));
    file.write((uint8_t *)wifiSSID, sizeof(wifiSSID));
    file.write((uint8_t *)wifiPassword, sizeof(wifiPassword));
    file.write((uint8_t *)building, sizeof(building));
    file.write((uint8_t *)room, sizeof(room));
}

void Configuration::readFromFile(File &file) {
    file.read((uint8_t *)nodeId, sizeof(nodeId));
    file.read((uint8_t *)mqttIp, sizeof(mqttIp));
    file.read((uint8_t *)&mqttPort, sizeof(mqttPort));
    file.read((uint8_t *)mqttUsername, sizeof(mqttUsername));
    file.read((uint8_t *)mqttPassword, sizeof(mqttPassword));
    file.read((uint8_t *)mqttTopic, sizeof(mqttTopic));
    file.read((uint8_t *)wifiSSID, sizeof(wifiSSID));
    file.read((uint8_t *)wifiPassword, sizeof(wifiPassword));
    file.read((uint8_t *)building, sizeof(building));
    file.read((uint8_t *)room, sizeof(room));
}

void Configuration::load() {
    if (!SPIFFS.begin()) {
        Serial.println("Config File storage can't start");
    }

    if (!SPIFFS.exists("config.bin")) {
        Serial.println("Default config loaded.");
        strlcpy(nodeId, String(ESP.getChipId()).c_str(), sizeof(nodeId));
        mqttPort = 1883;
        Serial.println("Config loaded.");
    }
    else {
        File file = SPIFFS.open(CONFIGURATION_FILE, "r+");
        readFromFile(file);
        file.close();
        Serial.println("Stored config loaded.");
    }
    SPIFFS.end();
}

void Configuration::save() {
    if (!SPIFFS.begin()) {
        Serial.println("Config File storage can't start");
    }

    File file = SPIFFS.open(CONFIGURATION_FILE, "w+");
    writeToFile(file);
    file.close();
    SPIFFS.end();
    
    Serial.println("Config stored.");
}

size_t Configuration::serializeToJson(Print &output) {
    const size_t CAPACITY = JSON_OBJECT_SIZE(10) + sizeof(Configuration);
    StaticJsonDocument<CAPACITY> doc;
    doc["nodeId"] = nodeId;
    doc["mqttIp"] = mqttIp;
    doc["mqttPort"] = mqttPort;
    doc["mqttUser"] = mqttUser;
    doc["mqttPassword"] = mqttPassword;
    doc["mqttTopic"] = mqttTopic;
    doc["wifiSSID"] = wifiSSID;
    doc["wifiPassword"] = wifiPassword;
    doc["building"] = building;
    doc["room"] = room;

    return serializeJson(doc, output);
}

void Configuration::updateFromJson(JsonObject doc) {
    String n_name = doc["nodeId"] | String(ESP.getChipId());
    strlcpy(nodeId, n_name.c_str(), sizeof(nodeId));
    strlcpy(mqttIp, doc["mqttIp"] | "", sizeof(mqttIp));
    mqttPort = doc["mqttPort"] | 1883;
    strlcpy(mqttUsername, doc["mqttUser"] | "", sizeof(mqttUser));
    strlcpy(mqttPassword, doc["mqttPassword"] | "", sizeof(mqttPassword));
    strlcpy(mqttTopic, doc["mqttTopic"] | "", sizeof(mqttTopic));
    strlcpy(wifiSSID, doc["wifiSSID"] | "", sizeof(wifiSSID));
    strlcpy(wifiPassword, doc["wifiPassword"] | "", sizeof(wifiPassword));
    strlcpy(building, doc["building"] | "", sizeof(building));
    strlcpy(room, doc["room"] | "", sizeof(room));

    // TODO reload services
    
}

Configuration config;