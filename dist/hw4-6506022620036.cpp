#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <Servo.h>

#include <time.h>

const char *ssid = "B415";
const char *password = "";

// ntp server
const char *ntpServer = "pool.ntp.org";

// Timezone offset
const long gmtOffset_sec = 3600 * 7;

Servo servo;

int servoPin = D4;

void setup()
{
    Serial.begin(115200);
    servo.attach(servoPin);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    configTime(gmtOffset_sec, 0, ntpServer);
}

void loop()
{
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);;

    delay(1000);

    // Check time if it's 08:00 AM and 06:00 PM
    if (timeinfo->tm_hour == 8 && timeinfo->tm_min == 0)
    {
        servo.write(180);
        delay(1000);
    }
    else if (timeinfo->tm_hour == 18 && timeinfo->tm_min == 0)
    {
        servo.write(180);
        delay(1000);
    } else {
        servo.write(0);
    }

    // Check time to check temperature is 06:00 AM, 12:00 PM, 06:00 PM and send to wokwi
    if (timeinfo->tm_hour == 6 && timeinfo->tm_min == 0)
    {
        Serial.print("Temperature: ");
        Serial.println(random(20, 40));
    }
    else if (timeinfo->tm_hour == 12 && timeinfo->tm_min == 0)
    {
        Serial.print("Temperature: ");
        Serial.println(random(20, 40));
    }
    else if (timeinfo->tm_hour == 18 && timeinfo->tm_min == 0)
    {
        Serial.print("Temperature: ");
        Serial.println(random(20, 40));
    }
}