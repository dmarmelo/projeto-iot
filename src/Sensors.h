#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
//#include <dht_nonblocking.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include "Configuration.h"
#include "Mqtt.h"

#define LDR_PIN A0

#define DHT_PIN D2
// dht_nonblocking
//#define DHT_TYPE DHT_TYPE_11
//#define DHT_TYPE DHT_TYPE_21
//#define DHT_TYPE DHT_TYPE_22

// Adafruit DHT
#define DHT_TYPE DHT11   // DHT 11
//#define DHT_TYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHT_TYPE DHT21   // DHT 21 (AM2301)

#define READING_INTERVAL 10000

class Sensors {
private:
    //DHT_nonblocking *_dht;
    DHT *_dht;
public:
    Sensors();
    ~Sensors();

    void setup();
    void loop();
};

extern Sensors sensors;

#endif //SENSORS_H
