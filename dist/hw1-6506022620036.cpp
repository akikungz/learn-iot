#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <DHT.h>

#define DHT_PIN D4
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHTTYPE);

const char* ssid = "B415";
const char* password = "appletv415";
const char* SERVER_NAME = "http://chaiyasit.atwebpages.com/sensordata.php";
String PROJECT_API_KEY = "chaiyasitmysqldht";

unsigned long lastMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Wire.begin();

  connectToWiFi();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - lastMillis > interval) {
      uploadTemperature();
      lastMillis = millis();
    }
  } else {
    Serial.println("WiFi Disconnected");
    connectToWiFi(); 
  }
  delay(1000);
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.println("...");

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {  
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void uploadTemperature() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read DHT sensor");
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  String temperature = String(t, 2);
  String humidity = String(h, 2);

  Serial.println("\n---------------------------------------------------------------------------");
  String temperature_data = "api_key=" + PROJECT_API_KEY + "&temperature=" + temperature + "&humidity=" + humidity;
  Serial.print("temperature_data: ");
  Serial.println(temperature_data);

  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_NAME);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(temperature_data);

  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
  } else {
    Serial.print("Error in HTTP request: ");
    Serial.println(http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}
