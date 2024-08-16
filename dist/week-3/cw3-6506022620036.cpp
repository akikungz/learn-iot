#include <Arduino.h>

#include <AXWIFI.h>

const int btn = 12;

const int red_led = 14;
const int green_led = 2;
const int yellow_led = 16;

int count = 0;

void setup() {
    Serial.begin(9600);
    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);

    pinMode(btn, INPUT);
    pinMode(red_led, OUTPUT);
    pinMode(green_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);
}

void loop() {
    int btnState = digitalRead(btn);

    if (btnState == LOW) {
        count++;
        Serial.print("Count: ");
        Serial.println(count);
    }

    if (count == 10) {
        digitalWrite(green_led, HIGH);
    } else if (count == 20) {
        digitalWrite(yellow_led, HIGH);
    } else if (count == 30) {
        digitalWrite(red_led, HIGH);
    } else if (count == 40) {
        digitalWrite(green_led, HIGH);
        digitalWrite(yellow_led, HIGH);
        digitalWrite(red_led, HIGH);
        delay(1000);
        count = 0;
    } else {
        digitalWrite(green_led, LOW);
        digitalWrite(yellow_led, LOW);
        digitalWrite(red_led, LOW);
    }

    delay(100);
}