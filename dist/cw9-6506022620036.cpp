#include <Arduino.h>
#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "B415";
const char* password = "appletv415";

// LED pin definitions
const int RED_LED = D4;   
const int GREEN_LED = D5; 
const int YELLOW_LED = D0;  
WiFiServer server(80);

void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  // Initialize all LEDs to be off
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  // Connect to Wi-Fi
  Serial.print("Connecting to the Network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  server.begin(); // Start the server
  Serial.println("Server started");

  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New client connected");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println("Request: " + request);
  client.flush();

  // Control the LEDs based on the request
  if (request.indexOf("/RED=ON") != -1) {
    digitalWrite(RED_LED, HIGH); // Turn RED LED ON
  }
  if (request.indexOf("/RED=OFF") != -1) {
    digitalWrite(RED_LED, LOW); // Turn RED LED OFF
  }
  if (request.indexOf("/GREEN=ON") != -1) {
    digitalWrite(GREEN_LED, HIGH); // Turn GREEN LED ON
  }
  if (request.indexOf("/GREEN=OFF") != -1) {
    digitalWrite(GREEN_LED, LOW); // Turn GREEN LED OFF
  }
  if (request.indexOf("/BLUE=ON") != -1) {
    digitalWrite(YELLOW_LED, HIGH); // Turn YELLOW LED ON
  }
  if (request.indexOf("/BLUE=OFF") != -1) {
    digitalWrite(YELLOW_LED, LOW); // Turn YELLOW LED OFF
  }

  // Create the HTML page with CSS styling
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
  // CSS styling for the page
  client.println("<style>");
  client.println("body { font-family: Arial, sans-serif; background-color: #f0f0f0; margin: 0; padding: 0; text-align: center; }");
  client.println("h1 { color: #333; }");
  client.println("button { background-color: #00FF00; #; border: none; color: white; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; }");
  client.println("button2 { background-color: #FF0000; border: none; color: white; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; }");
  client.println("button:hover { background-color: #45a049; }");
  client.println("a { text-decoration: none; }");
  client.println(".status { margin: 20px; font-size: 18px; }");
  client.println(".container { margin: 20px; padding: 20px; background-color: #fff; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); display: inline-block; }");
  client.println(".image { margin: 20px; }");
  client.println("</style>");

  client.println("<body>");
  client.println("<h1>LED Control Panel</h1>");
  client.println("<div class='container'>");

  client.print("<div class='status'>RED LED: ");
  client.print(digitalRead(RED_LED) == LOW ? "ON" : "OFF");
  client.println("</div>");
  client.println("<a href=\"/RED=ON\"><button>Turn RED ON</button></a>");
  client.println("<a href=\"/RED=OFF\"><button2>Turn RED OFF</button></a><br><br>");
  
  client.print("<div class='status'>GREEN LED: ");
  client.print(digitalRead(GREEN_LED) == LOW ? "ON" : "OFF");
  client.println("</div>");
  client.println("<a href=\"/GREEN=ON\"><button>Turn GREEN ON</button></a>");
  client.println("<a href=\"/GREEN=OFF\"><button2>Turn GREEN OFF</button></a><br><br>");

  client.print("<div class='status'>BLUE LED: ");
  client.print(digitalRead(YELLOW_LED) == LOW ? "ON" : "OFF");
  client.println("</div>");
  client.println("<a href=\"/BLUE=ON\"><button>Turn BLUE ON</button></a>");
  client.println("<a href=\"/BLUE=OFF\"><button2>Turn BLUE OFF</button></a><br><br>");
  
  client.println("</div>");

  // Move the image to the bottom
  client.println("<div class='image'>");
  client.println("<img src=\"https://scontent-bkk1-2.xx.fbcdn.net/v/t39.30808-6/277530061_1651949845153059_1882076327746208940_n.jpg?_nc_cat=105&ccb=1-7&_nc_sid=6ee11a&_nc_ohc=rGEVSaChWRAQ7kNvgHQfRqq&_nc_ht=scontent-bkk1-2.xx&_nc_gid=AC74t0wmNm41KPKJy7GK0Sy&oh=00_AYDsHcwk8YrKiA3Ser25_oEJX1eiHuFVOSUn09VoFxXa2A&oe=6713CEFB\" alt=\"Image\" width=\"400\">");
  client.println("</div>");
  client.println("<h1>Chaiyasit Mintakorn</h1>");
  client.println("</body>");
  client.println("</html>");
  
  delay(1);
  Serial.println("Client disconnected");
  Serial.println();
}
