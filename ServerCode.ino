/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/



/* add LIBRARY*/
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
// Import required libraries
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// Replaces placeholder with LED state value
String processor(const String& var){
  return "ok";}
ESP8266WiFiMulti WiFiMulti;
String payload="1";

// Replace with your network credentials
const char* ssid     = "Leoni";
const char* password = "123456789";

// Your IP address with path or Domain name with URL path 
const char* serverNamebutton1 = "http://192.168.4.1/Button1";

const int buttonPin = 4;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
    pinMode(buttonPin, INPUT_PULLUP);

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
    
      Serial.print("Setting AP (Access Point)…");  

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Leoni", "123456789");
  while((WiFiMulti.run() != WL_CONNECTED)) { 
    delay(500);
    Serial.print(".");
  }
    Serial.println("");
  Serial.println("Connected to WiFi");
  // Print ESP32 Local IP Address*
   
   Serial.print("Local IP Address");
  
   Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html", String(), false, processor);});
    
  // Route to load style.css file
server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/style.css", "text/css");});
  
server.on("/Leoni_image.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/Leoni_image.jpg", "image/jpg");});

server.on("/iset_sousse_image.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/iset_sousse_image.jpg", "image/jpg");});


  // Send a GET request to <ESP_IP>/state
  server.on("/T-270", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain",payload);//
  });

  // Start server
  server.begin();

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://192.168.4.1/Button1")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          payload = http.getString();
          if((payload=="1")&&(digitalRead(buttonPin)==0))
          {
            payload="2";
            }
         if((payload=="0")&&(digitalRead(buttonPin)==0))
          {
            payload="3";
            }
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  delay(100);
}
