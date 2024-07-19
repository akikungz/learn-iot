#include <Arduino.h>

int count = 0;

const int LED_GREEN = 2;
const int LED_YELLOW = 14;
const int LED_RED = 12;

void turnOnLED(int led) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);

    digitalWrite(led, HIGH);
}

int countFunc (int *count) {
    *count++;

    Serial.print("count = ");
    Serial.println(*count);

    if (*count % 16 == 0) {
        *count = 0;
    }

    return *count;
}

void setup() {
    Serial.begin(9600);

    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}

void loop() {
    countFunc(&count);

    if (count == 1) {
        turnOnLED(LED_RED);
    } else if (count == 12) {
        turnOnLED(LED_GREEN);
    } else if (count == 15) {
        turnOnLED(LED_YELLOW);
    }

    delay(1000);
}