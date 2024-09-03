#include <Arduino.h>

#include <DHT.h>

// Define the DHT sensor type and pin
#define DHTPIN D7     // Pin connected to the DHT22 data pin
#define DHTTYPE DHT22 // DHT 22 (AM2302)

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Define RGB LED pins
int R = D0;
int G = D4;
int B = D5;

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Set RGB LED pins as outputs
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
}

void loop() {
  // Read humidity from the DHT22 sensor
  float humidity = dht.readHumidity();

  // Check if the reading is valid
  if (isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (humidity > 70) { // High humidity
    setColor(0, 0, 255);  // Blue
  } else if (humidity > 40) { // Medium humidity
    setColor(0, 255, 0);  // Green
  } else { // Low humidity
    setColor(255, 0, 0);  // Red
  }

  // Wait before taking another reading
  delay(2000);
}

// Function to set the RGB LED color
void setColor(int red, int green, int blue) {
  analogWrite(R, red);
  analogWrite(G, green);
  analogWrite(B, blue);
}