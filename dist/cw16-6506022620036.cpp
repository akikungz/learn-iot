#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"

#define DHTTYPE DHT22 
const int DHTPin = D4;
#define ON_Board_LED 2 // On Board LED

const char* ssid = "B415"; // WiFi SSID
const char* password = "appletv415"; // WiFi Password

const char* host = "script.google.com";
const int httpsPort = 443;

DHT dht(DHTPin, DHTTYPE);
WiFiClientSecure client;

long now = millis();
long lastMeasure = 0;
String GAS_ID = "1z8iscJywAXhy2DsmPIWJtsb0RUO0rGGaOHBkA0rjX0-tCSukQrg2MD__"; // Google Apps Script ID 

void setup() {
  Serial.begin(115200);
  delay(500);
  dht.begin();
  WiFi.begin(ssid, password); // Connect to WiFi
  Serial.println("");
  pinMode(ON_Board_LED, OUTPUT); // On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); // Turn off LED
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }

  digitalWrite(ON_Board_LED, HIGH); 
  Serial.println("");
  Serial.print("Successfully connected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
}

void loop() {
  now = millis();
  if (now - lastMeasure > 3000) {
    lastMeasure = now;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\tTemperature: ");
    Serial.print(t);
    Serial.println(" *C");

    sendData(t, h);
  }
}

void sendData(float temperature, float humidity) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?temp=" + String(temperature) + "&humi=" + String(humidity);
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Data sent successfully!");
  } else {
    Serial.println("Failed to send data");
  }
  Serial.print("reply was: ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
}
