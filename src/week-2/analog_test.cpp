#include <Arduino.h>

void setup () {
    Serial.begin(9600);

    pinMode(A0, INPUT);
    pinMode(2, OUTPUT);

    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

void loop () {
    int sensorValue = analogRead(A0);
    int ledValue = map(sensorValue, 0, 1023, 0, 255);

    analogWrite(2, ledValue);

    Serial.print("sensorValue = ");
    Serial.print(sensorValue);
    Serial.print("\t");
    Serial.print("ledValue = ");
    Serial.println(ledValue);

    delay(500);
}