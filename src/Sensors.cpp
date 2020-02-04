#include "Sensors.h"

unsigned long lastRead = 0ul;

Sensors::Sensors() {

}

Sensors::~Sensors() {

}

void Sensors::setup() {
    //_dht = new DHT_nonblocking(DHT_PIN, DHT_TYPE);
    _dht = new DHT(DHT_PIN, DHT_TYPE);
    pinMode(LDR_PIN, INPUT);
}

void Sensors::loop() {
    if (lastRead + READING_INTERVAL < millis()) {
        // Read DHT
        /*float temperature;
        float humidity;
        if (_dht->measure(&temperature, &humidity) == false) {
            Serial.println("DHT reading failed");
            return;
        }*/

        // Reading temperature or humidity takes about 250 milliseconds!
		// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
		float humidity = _dht->readHumidity();
		// Read temperature as Celsius (the default)
		float temperature = _dht->readTemperature();

		// Check if any reads failed and exit early (to try again).
		if (isnan(humidity) || isnan(temperature)) {
			Serial.println(F("Failed to read from DHT sensor!"));
			return;
		}
        Serial.print("DHT-> ");
        Serial.print(F("Humidity: "));
		Serial.print(humidity);
		Serial.print(F("%  Temperature: "));
		Serial.print(temperature);
		Serial.println(F("°C"));

        // Read LDR
        int ldrRaw = analogRead(LDR_PIN);
        Serial.print("LDR-> ");
        Serial.print(ldrRaw);
		Serial.println(F("Lmn"));

        // Send to MQTT

        const size_t capacity = JSON_OBJECT_SIZE(5) + sizeof(config.room) + sizeof(config.building);
        DynamicJsonDocument doc(capacity);

        JsonObject fields = doc.createNestedObject("fields");
        fields["temperature"] = temperature;
        fields["humidity"] = humidity;
        fields["illuminance"] = ldrRaw;

        JsonObject tags = doc.createNestedObject("tags");
        tags["building"] = config.building;
        tags["room"] = config.room;

		// serialize the object and send the result to Serial
		String output;
		serializeJson(doc, output);
		//Serial.println(output);
		publishOnMqtt(config.mqttTopic, output.c_str(), false);

        lastRead = millis();
    }
}

Sensors sensors;
