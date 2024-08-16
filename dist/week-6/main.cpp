#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <AXWIFI.h>

// Wifi settings
const char *ssid = "AKI-LAPTOP";
const char *password = "AkiLolicon@1";

// LED settings
const int led_red = 13;
const int led_yellow = 12;
const int led_green = 14;

// Web server
ESP8266WebServer server(80);

void serial_print_data(float temp, float hum) {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" *C");

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
}

void setup() {
    Serial.begin(9600);

    // Connect to wifi
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1);
    }
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    server.begin();
    Serial.println("Server started");

    ax.begin();
    ax.SledTypeRGB();
    ax.SledShow(0, 0, 0, 0);
    ax.SledShow(1, 0, 0, 0);
    ax.beep();

    pinMode(led_red, OUTPUT);
    pinMode(led_yellow, OUTPUT);
    pinMode(led_green, OUTPUT);
}

void loop () {
    // Handle the web server
    server.handleClient();

    // Send http response to client
    server.send(200, "text/plain", "Hello from ESP8266!");

    
}