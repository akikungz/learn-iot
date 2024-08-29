#include <Arduino.h>

const int analogInPin = A0;

void setup() {
  Serial.begin(9600);

  pinMode(D4, OUTPUT);
}

void loop() {
  int out = analogRead(analogInPin);

  Serial.print("Analog input = ");
  Serial.println(out);

  if (out > 500) {
    digitalWrite(D4, HIGH);
  } else {
    digitalWrite(D4, LOW);
  }

  delay(10);
}
