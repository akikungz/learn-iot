#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include <DHT.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

struct WifiConfig
{
    const char *ssid;
    const char *password;
};

struct MqttConfig
{
    const char *server;
    const char *clientId;
};

const WifiConfig wifiConfig = {
    "B415",
    "appletv415"};

const MqttConfig mqttConfig = {
    "10.110.6.150",
    "esp8266",
};

WiFiClient wifiClient;

PubSubClient mqttClient(wifiClient);

DHT dht(14, DHT22);

void setup()
{
    Serial.begin(9600);

    Wire.begin();
    SPI.begin();

    dht.begin();

    WiFi.begin(wifiConfig.ssid, wifiConfig.password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    mqttClient.setServer(mqttConfig.server, 1883);
    mqttClient.setCallback([](char *topic, byte *payload, unsigned int length) {
        // check if the topic is "node-red/led"
        if (strcmp(topic, "node-red/led") == 0)
        {
            if (payload[0] == '1')
            {
                digitalWrite(13, HIGH);
            }
            else
            {
                digitalWrite(13, LOW);
            }
        }
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        for (int i = 0; i < length; i++)
        {
            Serial.print((char)payload[i]);
        }
        Serial.println();
    });

    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (mqttClient.connect(mqttConfig.clientId))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(mqttClient.state());
            delay(2000);
        }
    }

    mqttClient.subscribe("node-red/led");
    pinMode(13, OUTPUT);
}

void loop()
{
    mqttClient.loop();

    if (!mqttClient.connected())
    {
        mqttClient.connect(mqttConfig.clientId);
    }

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    mqttClient.publish("esp8266/temperature", String(temperature).c_str());
    mqttClient.publish("esp8266/humidity", String(humidity).c_str());

    // mqttClient.publish("esp8266", "hello from esp8266");
    delay(1000);
}
