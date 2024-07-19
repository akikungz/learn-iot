// #include <Arduino.h>
// #include <DHT.h>

// #define DHTPIN 13

// const int LED_GREEN = 2;
// const int LED_YELLOW = 14;
// const int LED_RED = 12;

// DHT dht(DHTPIN, DHT22);

// float temp = 0;

// void setup() {
//     Serial.begin(9600);

//     dht.begin();

//     pinMode(LED_GREEN, OUTPUT);
//     pinMode(LED_YELLOW, OUTPUT);
//     pinMode(LED_RED, OUTPUT);
// }

// void loop () {
//     float t = dht.readTemperature();

//     if (isnan(t)) {
//         Serial.println("Failed to read from DHT sensor!");
//         return;
//     }

//     if (temp == 0) {
//         temp = t;
//     } else {
//         Serial.print("Before: ");
//         Serial.println(temp);

//         Serial.print("After: ");
//         Serial.println(t);

//         if (t > temp) {
//             digitalWrite(LED_GREEN, HIGH);
//             delay(1000);
//             digitalWrite(LED_YELLOW, HIGH);
//             delay(1000);
//             digitalWrite(LED_RED, HIGH);
//         } else if (t < temp) {
//             digitalWrite(LED_RED, HIGH);
//             delay(1000);
//             digitalWrite(LED_YELLOW, HIGH);
//             delay(1000);
//             digitalWrite(LED_GREEN, HIGH);
//         }
//     }

//     delay(2000);
//     digitalWrite(LED_GREEN, LOW);
//     digitalWrite(LED_YELLOW, LOW);
//     digitalWrite(LED_RED, LOW);

//     temp = t;
// }
