// #include <Arduino.h>
// #include <DHT.h>

// #define ledRED 5
// #define ledYELLOW 6
// #define ledGREEN 7

// #define DHTPIN 8 
// #define DHTTYPE DHT22
// DHT dht(DHTPIN, DHTTYPE);

// float before_temp = 0;

// void runLED (int input) {
//   if (input == 1) {
//     digitalWrite(ledGREEN, HIGH);
//     delay(500);
//     digitalWrite(ledYELLOW, HIGH);
//     delay(500);
//     digitalWrite(ledRED, HIGH);
//   } else if (input == -1) {
//     digitalWrite(ledRED, HIGH);
//     delay(500);
//     digitalWrite(ledYELLOW, HIGH);
//     delay(500);
//     digitalWrite(ledGREEN, HIGH);
//   }

//   delay(4000);
//   digitalWrite(ledRED, LOW);
//   digitalWrite(ledYELLOW, LOW);
//   digitalWrite(ledGREEN, LOW);
//   delay(1000);
// }

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(ledRED, OUTPUT);
//   pinMode(ledYELLOW, OUTPUT);
//   pinMode(ledGREEN, OUTPUT);
//   Serial.begin(9600);
//   Serial.println(F("DHTxx test!"));

//   dht.begin();
// }

// void loop() {

//   float temp = dht.readTemperature();

//   if (isnan(temp)) {
//     Serial.println(F("Failed to read from DHT sensor!"));
//     return;
//   }

//   if (temp == 0) {
//     before_temp = temp;
//     return;
//   } else {
//     if (temp > before_temp) {
//       runLED(1);
//       return;
//     } else if (temp < before_temp) {
//       runLED(-1);
//       return;
//     } else {
//       runLED(0);
//       return;
//     }
//   }

// }
