#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <espnow.h>

#include <AXWIFI.h>

const int LED_1 = 14;
const int LED_2 = 13;
const int LED_3 = 12;

struct Message {
    bool SW_1;
    bool SW_2;
} message;

void onDataRecv (uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    memcpy(&message, incomingData, sizeof(message));
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("SW_1: ");
    Serial.println(message.SW_1);
    Serial.print("SW_2: ");
    Serial.println(message.SW_2);

    // Control the LED
    digitalWrite(LED_1, (message.SW_1 && !message.SW_2) ? HIGH : LOW);
    digitalWrite(LED_2, (!message.SW_1 && message.SW_2) ? HIGH : LOW);
    digitalWrite(LED_3, (message.SW_1 && message.SW_2) ? HIGH : LOW);
}

void setup() {
    Serial.begin(9600);

    // Show the MAC address of the ESP8266
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    // Define the LED pins as output
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);

    // Set the initial state of the LEDs to off
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);

    // init axwifi
    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);

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
    // Do nothing
}