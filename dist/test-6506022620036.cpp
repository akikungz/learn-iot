#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>

#include <ESP8266WiFi.h>

#include <DHT.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ThingSpeak.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ssid = "B415";
const char *password = "appletv415";

WiFiClient client;

struct ThingSpeakConfig {
    const char *api_key = "18UCK96F4OIJHODP";
    const unsigned long channel_id = 2640513;
} thingspeak_config;

// Sensor
struct Sensor {
    const int trigger = 5;
    const int echo = 4;
    const int dht = 2;
} sensor;

DHT dht(sensor.dht, DHT22);

struct Target {
    const int warming_light = 14;
    const int alert_light = 12;
    const int cooling_fan = 13;
} target;

struct Memory {
    float temperature;
    float humidity;
    float level;
} memory;

bool dhtStep () {
    memory.temperature = dht.readTemperature();
    memory.humidity = dht.readHumidity();

    if (isnan(memory.temperature) || isnan(memory.humidity)) {
        Serial.println("Failed to read data from DHT22");
        return false;
    }

    return true;
}

bool ultraSonicStep () {
    digitalWrite(sensor.trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(sensor.trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor.trigger, LOW);

    float duration = pulseIn(sensor.echo, HIGH);
    memory.level = 100 - (duration * 0.034 / 2);

    return true;
}

bool targetStep () {
    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setTextSize(1); 
    OLED.setCursor(0, 0);

    if (memory.temperature < 20 || memory.temperature > 25) {
        digitalWrite(target.alert_light, HIGH);

        if (memory.temperature > 25) digitalWrite(target.cooling_fan, HIGH);
        if (memory.temperature < 20) digitalWrite(target.warming_light, HIGH);
    } else {
        digitalWrite(target.cooling_fan, LOW);
        digitalWrite(target.warming_light, LOW);
    }

    if (memory.humidity < 60 || memory.humidity > 75) digitalWrite(target.alert_light, HIGH);

    if (memory.level < 5) digitalWrite(target.alert_light, HIGH);

    OLED.print("Temperature: ");
    OLED.print(memory.temperature);
    OLED.println(" Celsius");
    
    Serial.print("Temperature: ");
    Serial.print(memory.temperature);
    Serial.println(" Celsius");

    OLED.print("Humidity: ");
    OLED.print(memory.humidity);
    OLED.println("%");

    Serial.print("Humidity: ");
    Serial.print(memory.humidity);
    Serial.println("%");

    OLED.print("Water Level: ");
    OLED.print(memory.level);
    OLED.println(" cm");

    Serial.print("Water Level: ");
    Serial.print(memory.level);
    Serial.println(" cm");

    OLED.display();
    return true;
}

bool thinkSpeakStep () {
    ThingSpeak.setField(1, memory.temperature);
    ThingSpeak.setField(2, memory.humidity);
    ThingSpeak.setField(3, memory.level);

    int status = ThingSpeak.writeFields(thingspeak_config.channel_id, thingspeak_config.api_key);

    Serial.print("Status: ");
    Serial.println(status);

    if (status == 200) {
        Serial.println("Data sent to ThingSpeak");
        return true;
    } else {
        Serial.println("Failed to send data to ThingSpeak");
        return false;
    }
}

void setup () {
    Serial.begin(9600);

    ThingSpeak.begin(client);

    if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
    } else {
        OLED.clearDisplay();
        OLED.display();
        Serial.println("OLED Ready!");
    }

    dht.begin();
    pinMode(sensor.dht, INPUT);

    pinMode(sensor.trigger, OUTPUT);
    pinMode(sensor.echo, INPUT);

    pinMode(target.alert_light, OUTPUT);
    pinMode(target.cooling_fan, OUTPUT);
    pinMode(target.warming_light, OUTPUT);
}

void loop () {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        Serial.print("Connecting to Wifi");
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
        Serial.println("");
        Serial.println("Connected to WiFi");
    }

    if (!dhtStep()) return;
    if (!ultraSonicStep()) return;
    if (!targetStep()) return;
    if (!thinkSpeakStep()) delay(1000);
    delay(1000);
}