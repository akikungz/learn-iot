#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <espnow.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <AXWIFI.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int SW_1 = 16;

bool cleared = false;

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

struct Message {
    bool SW_1 = false;
    float temperature;
    float humidity;
} message;

void onDataRecv (uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    memcpy(&message, incomingData, sizeof(message));

    Serial.print("Bytes received: ");
    Serial.println(len);
}

void setup() {
    Serial.begin(9600);

    // Button init
    pinMode(SW_1, INPUT_PULLUP);

    // Show the MAC address of the ESP8266
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    // init axwifi
    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);

    // Oled init
    if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    } else {
        Serial.println(F("SSD1306 allocation success"));
    }

    OLED.clearDisplay();
    OLED.setTextSize(1);
    OLED.setTextColor(WHITE);
    OLED.setCursor(0, 0);
    OLED.print("Press SW_1 to show the data");
    OLED.display();
    cleared = true;

    // WiFi mode STA
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(onDataRecv);
}

void loop() {
    // Button state
    if (digitalRead(SW_1) == LOW) {
        // Set OLED display
        OLED.clearDisplay();
        OLED.setTextSize(1);
        OLED.setTextColor(WHITE);
        OLED.setCursor(0, 0);
        OLED.print("Temperature: ");
        OLED.print(message.temperature);
        OLED.println(" C");

        OLED.setCursor(0, 10);
        OLED.print("Humidity: ");
        OLED.println(message.humidity);
        OLED.println(" %");
        OLED.display();
        cleared = false;

        // Print to serial monitor
        Serial.println("SW_1: Clicked");

        Serial.print("Temperature: ");
        Serial.print(message.temperature);
        Serial.println(" C");

        Serial.print("Humidity: ");
        Serial.print(message.humidity);
        Serial.println(" %");
        
        delay(1000);
    }
}