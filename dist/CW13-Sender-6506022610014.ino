#include <ESP8266WiFi.h>
#include <espnow.h>

#define SW1_PIN D1
#define SW2_PIN D2

uint8_t broadcastAddress[] = {0x68, 0xC6, 0x3A, 0xAC, 0x54, 0xB8};

typedef struct struct_message {
  bool SW_1;
  bool SW_2;

} struct_message;

struct_message myData;

unsigned long lasTime = 0;
unsigned long timeDelay = 50;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.println("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
 }

void setup() {
  Serial.begin(115200);

  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
 

  myData.SW_1 = digitalRead(SW1_PIN) == LOW;
  myData.SW_2 = digitalRead(SW2_PIN) == LOW;

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != 0){
      Serial.println("Error initializing ESP-NOW");
      return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_send_cb(OnDataSent);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop() {
    if ((millis() - lasTime) > timeDelay){
      lasTime = millis();
      myData.SW_1 = digitalRead(SW1_PIN) == LOW;
      myData.SW_2 = digitalRead(SW2_PIN) == LOW;
      
     
      esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    lasTime = millis();
    }
}
