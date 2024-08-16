#include <Arduino.h>

#include <AXWIFI.h>

const int trigger = 5;
const int echo = 4;

long duration, distance;

void setup() {
    Serial.begin(9600);
    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);

    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
}

void loop() {
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    duration = pulseIn(echo, HIGH);
    
    // Sensor at height 200 cm
    distance = 200 - (duration * 0.034 / 2);
    Serial.print("Height: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Distance in feet and inches
    float feet = distance / 30.48;
    float inches = (feet - int(feet)) * 12;
    Serial.print("Height: ");
    Serial.print(int(feet));
    Serial.print("\" ");
    Serial.print(int(inches));
    Serial.println("'");

    // Serial.println("Hello, World!");
    delay(1000);
}