#include <Arduino.h>
#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

#define SSID "B415"
#define PASSWORD "appletv415"
#define LINE_TOKEN "YOUR_LINE_NOTIFY"

#define DHTPIN D4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastNotificationTime = 0;
const unsigned long notificationInterval = 120000; // 2 minutes in milliseconds

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWifi connected\nIP");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN);

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  unsigned long currentTime = millis();

  if (humidity > 90) {
    if (currentTime - lastNotificationTime >= notificationInterval) {
      String message = "\nความชื้นเกินกำหนด: " + String(humidity) + "% \nอุณหภูมิ: " + String(temperature) + "°C";
      LINE.notify(message);
      lastNotificationTime = currentTime;
    }
  }

  delay(1000);
}
