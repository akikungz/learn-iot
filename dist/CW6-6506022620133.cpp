#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int trig = D7;
int echo = D8;

float duration;
int distance;
float height;
int door_Height = 200;

void setup() 
{
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("KMUTNB Start!!!");
    OLED.clearDisplay();
    OLED.display();
  }
}

void loop() 
{
  digitalWrite(trig, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration / 2) / 29.1;
  height = door_Height - distance;

  float heightInches = height * 0.393701;
  int feet = int(heightInches) / 12;
  int inches = int(heightInches) % 12;
  float heightCm = feet * 30.48 + inches * 2.54;

  Serial.print("Feet and inches: ");
  Serial.print(feet);
  Serial.print("' ");
  Serial.print(inches);
  Serial.println("\"");
  Serial.print("Centimeters: ");
  Serial.print(heightCm, 2);
  Serial.println(" cm");

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  
 
  OLED.setTextSize(2); 
  OLED.setCursor(0, 0);
  OLED.print(feet);
  OLED.print("' ");
  OLED.print(inches);
  OLED.println("\"");

  
  OLED.setTextSize(2); 
  OLED.setCursor(0, 32);
  //OLED.print("Cm: ");
  OLED.print(heightCm, 2);
  OLED.println(" cm");

  OLED.display();

  delay(1000); 
}