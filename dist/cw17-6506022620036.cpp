#include <Arduino.h>
#include <Wire.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <BH1750.h>  // Include BH1750 library
#include <time.h>    // For NTP time

// Pin and constants
#define DHTPIN D4     // Pin connected to the DHT22 data pin
#define DHTTYPE DHT22 // DHT 22 (AM2302)
#define LED_PIN D7    // Pin connected to the LED

// Firebase credentials
char FIREBASE_AUTH[] = "";  // Your Firebase Web API Key
char FIREBASE_HOST[] = ""; // Your Firebase Host URL

// WiFi credentials
char WIFI_SSID[] = "";     // Your WiFi SSID
char WIFI_PASSWORD[] = ""; // Your WiFi password

// Firebase and DHT setup
FirebaseData firebaseData;
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
BH1750 lightSensor;        // Initialize BH1750 sensor

// Static variable for log count
static int logCount = 1;

void setup() {
  Serial.begin(115200);
  
  // Start WiFi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
  
  // Start Firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Start DHT sensor
  dht.begin();

  // Start light sensor
  Wire.begin(); // Start I2C
  lightSensor.begin(); // Initialize BH1750

  // Start NTP to get the time (UTC+7 for Thailand)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for NTP time sync...");
  while (!time(nullptr)) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nTime synced!");

  // Set up LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Ensure LED is initially off
}

String getFormattedTime() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  
  char buffer[20];
  strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", timeinfo);
  
  return String(buffer);
}

void loop() {
  // Get temperature and humidity from DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Get light level from BH1750
  float lightLevel = lightSensor.readLightLevel();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity) || isnan(lightLevel)) {
    Serial.println("Failed to read from sensors!");
  } else {
    // Get the current timestamp as a formatted string
    String timestamp;

    if (time(nullptr) == 0) {
      Serial.println("NTP time not set, using default time.");
      timestamp = "1970/01/01 00:00:00"; // Default time
    } else {
      timestamp = getFormattedTime(); // Get formatted time
    }

    // Create a new log entry using logCount with zero padding
    String logPath;
    if (logCount < 10) {
      logPath = "/logs/log_00" + String(logCount); // log_001, log_002
    } else if (logCount < 100) {
      logPath = "/logs/log_0" + String(logCount); // log_010, log_011
    } else {
      logPath = "/logs/log_" + String(logCount); // log_100
    }
    logCount++;  // Increment logCount for the next entry

    // Toggle LED status
    static bool ledState = false;  // Start with LED OFF
    ledState = !ledState;  // Toggle the state
    digitalWrite(LED_PIN, ledState ? HIGH : LOW); // Set LED state

    // Log LED status to Firebase
    Firebase.setString(firebaseData, logPath + "/led", ledState ? "ON" : "OFF"); // Log current state

    // Send temperature, humidity, light level, and timestamp to Firebase
    Firebase.setFloat(firebaseData, logPath + "/temperature", temperature);
    Firebase.setFloat(firebaseData, logPath + "/humidity", humidity);
    Firebase.setFloat(firebaseData, logPath + "/light", lightLevel);
    Firebase.setString(firebaseData, logPath + "/time", timestamp);

    // Print values to Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    Serial.print("Light Level: ");
    Serial.print(lightLevel);
    Serial.println(" lx");
    Serial.print("LED Status: ");
    Serial.println(ledState ? "ON" : "OFF"); // Print current LED status
    Serial.print("Timestamp: ");
    Serial.println(timestamp);

    // Wait for 5 seconds before next loop
    delay(5000);
  }
}
