#include <Arduino.h>

#include <AXWIFI.h>

const int btn = 12;

const int red_led = 14;
const int green_led = 2;
const int yellow_led = 16;

int count = 0;
int state = 0;

void led_state (int state) {
    digitalWrite(green_led, state);
    digitalWrite(yellow_led, state);
    digitalWrite(red_led, state);
}

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

    if (btnState == LOW && state != LOW) {
        count++;
        state = LOW;
    } else if (state == LOW && btnState == HIGH) {
        state = HIGH;
    }

    if (count % 2 == 0) {
        led_state(LOW);
    } else {
        led_state(HIGH);
    }
}