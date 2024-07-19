#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 13

DHT dht(DHTPIN, DHT22);

void setup() {
    Serial.begin(9600);

    dht.begin();
}

void loop() {
    delay(2000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float H = dht.computeHeatIndex();

    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Heat Index: ");
    Serial.print(H);
}