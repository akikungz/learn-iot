#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <DHT.h>

#define SW1_PIN D1
#define DHTPIN D4
#define DHTTYPE DHT22

uint8_t broadcastAddress[] = {0x68, 0xC6, 0x3A, 0xAC, 0x54, 0xB8};

typedef struct struct_message {
  bool SW_1;
  float temperature;
  float humidity;
} struct_message;

struct_message myData;

DHT dht(DHTPIN, DHTTYPE);

unsigned long lasTime = 0;
unsigned long timeDelay = 50;

void OnDataRecv(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(SW1_PIN, INPUT_PULLUP);

  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataRecv);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
  if ((millis() - lasTime) > timeDelay) {
    lasTime = millis();
    myData.SW_1 = digitalRead(SW1_PIN) == LOW;

    if (myData.SW_1) {
      myData.temperature = dht.readTemperature();
      myData.humidity = dht.readHumidity();

      if (isnan(myData.temperature) || isnan(myData.humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
      }

      Serial.print("Temperature: ");
      Serial.print(myData.temperature);
      Serial.print(" *C, Humidity: ");
      Serial.print(myData.humidity);
      Serial.println(" %");

      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    }
  }
}
