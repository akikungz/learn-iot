#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

const int relay_pin = 2;
int state = LOW;

void setup () {
    pinMode(relay_pin, OUTPUT);
}

void loop () {
    digitalWrite(relay_pin, state);
    delay(500);
    state = state == LOW ? HIGH : LOW;
}
