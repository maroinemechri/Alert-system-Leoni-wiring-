/*
marwene mechri
*/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>


// Set your access point network credentials
const char* ssid = "Leoni";
const char* password = "123456789";
const int buttonPin = 4;
//Your IP address or domain name with URL path




// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/Button1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(digitalRead(buttonPin)).c_str());
  });


  // Start server
  server.begin();
}
 
void loop(){
  
}
