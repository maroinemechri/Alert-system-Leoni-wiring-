/*********
 * Alert System
 * 
 * Made By Marwene MECHRI at MakerLab
 * 05/05/2021
*********/


// Import required libraries
#ifdef  ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif


#define Button 4


String Post2 = "";
String Post1 = "";


// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 100;

  // Replace with your network credentials
const char* ssid     = "MakerLab";
const char* password = "MakerLab2020";

  // Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDSPost1() {
  char *state;
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  //  sensors.requestTemperatures(); 
  float tempC = digitalRead(Button);

  if(tempC == 0) {
    Serial.println("pressed Button");
    state="Alert";
    
  } else {
    Serial.print("not pressed Button ");
    Serial.println(tempC); 
    state="OK";
  }
  return state;
}

String readDSPost2() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  // sensors.requestTemperatures(); 
  float tempF = 0;
  char *state ="Not Work";
  if(int(tempF) == -196){
    Serial.println("Second Node doesn't Work");
    return state;
  } else {
    Serial.print("Second Node doesn't Work");
    Serial.println(tempF);
  }
  return state;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 3rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2> Alert Sytem</h2>
  <p>

    <span class="ds-labels">Post1 :  </span> 
    <span id="Post1">%Post1 : %</span>

  </p>
  <p>

    <span class="ds-labels">Post2 : </span>
    <span id="Post2">%Post2 :%</span>
   <p>  
    <span class="ds-labels">Post3 : </span>
    <span id="Post3">%Post2 :%</span>
</p>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Post1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Post1", true);
  xhttp.send();
}, 1000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Post2").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Post2", true);
  xhttp.send();
}, 700) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "Post1"){
    return Post1;
  }
  else if(var == "Post2"){
    return Post2;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  pinMode(Button,INPUT_PULLUP);
  Serial.begin(115200);
 // Serial.println();
  
  
  // Start up the DS18B20 library
//  sensors.begin();

  Post1 = readDSPost1();
  Post2 = readDSPost2();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Post1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", Post1.c_str());
  });
  server.on("/Post2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", Post2.c_str());
  });
  // Start server
  server.begin();
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
    Post1 = readDSPost1();
    Post2 = readDSPost2();
    lastTime = millis();
  }  
}
