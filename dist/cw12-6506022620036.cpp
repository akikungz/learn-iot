#include <Arduino.h>
// #include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#include <Wire.h>
#include <SPI.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include <AXWIFI.h>

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6_96nfpnK"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "y-OA-VMt9g0r-8ocR9y7Dh8GvlnhY4TV"

#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;

void myTimerEvent()
{
    // You can send any value at any time.
    // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V2, millis() / 1000);
}

BLYNK_CONNECTED()
{
    // Change Web Link Button message to "Congratulations!"
    Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
    Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
    Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

struct WiFiConfig {
    const char *ssid = "B415";
    const char *password = "appletv415";
} wifi;

void BH1750_Init(int address) {
    Wire.beginTransmission(address);
    Wire.write(0x10); // 1 [lux] resolution
    Wire.endTransmission();
}

int BH1750_Read(int address, byte *buff) {
    int i = 0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    Serial.print("Wire.available(): ");
    Serial.println(Wire.available());
    while(Wire.available()) {
        buff[i] = Wire.read();
        i++;
    }
    Wire.endTransmission();
    return i;
}

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int BH1750_address = 0x23;
byte buff[2];

void setup() {
    Serial.begin(9600);

    // OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    BH1750_Init(BH1750_address);
    
    if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
    }

    OLED.clearDisplay();
    OLED.display();

    // Wifi connection
    WiFi.begin(wifi.ssid, wifi.password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi...");
    }

    // Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, wifi.ssid, wifi.password);
    timer.setInterval(1000L, myTimerEvent);

    // AxWifi
    // ax.begin();
    // ax.SledTypeRGB();
    // ax.SledShow(0, 0, 0, 0);
    // ax.SledShow(1, 0, 0, 0);
}

void loop() {
    uint16_t val = 0;
    Serial.println("Reading light sensor...");

    Blynk.run();
    timer.run();

    BH1750_Init(BH1750_address);
    delay(200);
    if (2 == BH1750_Read(BH1750_address, buff)) {
        val = (((unsigned int)buff[0] << 8) | (unsigned int)buff[1]) / 1.2;
        Serial.print("Light: ");
        Serial.print(val);
        Serial.println(" lx");

        Blynk.virtualWrite(V0, val);

        OLED.clearDisplay();
        OLED.setTextSize(1);
        OLED.setTextColor(WHITE);
        OLED.setCursor(0, 0);
        OLED.print("Light: ");
        OLED.print(val);
        OLED.println(" lx");
        OLED.display();
    }
    
    delay(1000);
}