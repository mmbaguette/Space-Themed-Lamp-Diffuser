/*
This program makes the ESP join a Wi-Fi network with a given SSID (WiFi name), password, and hosts a website at a given URL in the serial monitor,
which lets you turn a connected LED on and off.
*/

#include <ESP8266WiFi.h>

const char* ssid = "My Phone HotSpot";     // Your Wi-Fi Name
const char* password = "WIFI PASSWORD"; // Wi-Fi Password
const int LED = D1;   // LED connected to given pin

WiFiServer server(80); //port number 80 (for http)

void setup() {
  Serial.begin(115200); // Default Baudrate
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.print("Connecting to the Network");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("Server started");
  server.begin();

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

  Serial.println("Waiting for new client");

  while (!client.available()) {
    //Serial.print(".");
    delay(100);

  }
  
  String request = client.readStringUntil('\r');
  Serial.println("Connection found!");
  Serial.println(request);
  client.flush();

  int value = LOW;

  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED, HIGH); // Turn LED ON
    value = HIGH;
  }

  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED, LOW); // Turn LED OFF
    value = LOW;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("CONTROL LED: ");

  if (value == HIGH) {
    client.print("ON");
  } else {
    client.print("OFF");
  }

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
  client.println("</html>");

  delay(1);

  Serial.println("Client disconnected");
  Serial.println("");
}
