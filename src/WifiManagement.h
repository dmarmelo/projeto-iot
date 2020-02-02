#ifndef WIFI_MANAGEMENT
#define WIFI_MANAGEMENT

#include <Arduino.h>

class WifiManagement {
private:
    
public:
    WifiManagement();
    ~WifiManagement();

    void setup();
    void loop();
    void reloadConfig();
    void refreshMDNS(const char* lastName);
    size_t systemJSONStatus(Print &output);
    
};

#endif