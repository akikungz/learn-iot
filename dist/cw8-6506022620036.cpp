#include <Arduino.h>

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#include <AXWIFI.h>

// Wifi settings
const char *ssid = "AKI-LAPTOP";
const char *password = "AkiLolicon@1";

// ThingSpeak settings
const char *api_key = "SJ8FC123JPUFQDNB";
const unsigned long channel_id = 2614714;
const int time_interval = 15;

// Initialize the dht sensor
const int dht_pin = 2;
DHT dht(dht_pin, DHT22);

WiFiClient client;

void serial_print_data(float temp, float hum) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
}

void think_speak(float temp, float hum) {
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);

    int status = ThingSpeak.writeFields(channel_id, api_key);

    Serial.print("Status: ");
    Serial.println(status);

    if (status == 200) {
        Serial.println("Data sent to ThingSpeak");
    } else {
        Serial.println("Failed to send data to ThingSpeak");
    }

    delay(time_interval * 1000);
}

void setup() {
    Serial.begin(9600);

    ThingSpeak.begin(client);

    dht.begin();
    pinMode(dht_pin, INPUT);

    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.println("Connecting to WiFi..");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }
        Serial.println("Connected to WiFi");
    }

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT sensor");
        delay(1000);
        return;
    }

    serial_print_data(temp, hum);
    think_speak(temp, hum);
}