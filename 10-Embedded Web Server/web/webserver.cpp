#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESPectro.h>

#define ledPin 15
#define buttonPin 0
#define ledOn HIGH
#define ledOff LOW
#define interval 5000

ESPectro board(ESPectro_V3);
String mainPage = "";
String buttonPage = "";
long lastmillis=0;
ESP8266WebServer server(80);

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  board.turnOffAllNeopixel();
  mainPage += "<h1>ESP8266 Web Server</h1><p>On Board LED <a href=\"ledon\"><button>ON</button></a>&nbsp;<a href=\"ledoff\"><button>OFF</button></a></p>";
  mainPage += "<p>Button State <a href=\"button\"><button>READ</button></a></p>";
  pinMode(ledPin, OUTPUT); 
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, ledOff);
  Serial.begin(115200);
 
  WiFi.mode(WIFI_AP);
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("ESP8266", "Medan2017") ? "Ready" : "Failed!");
  Serial.println("Please connect to IP: " + WiFi.softAPIP()); 
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }


  server.on("/", [](){
    server.send(200, "text/html", mainPage);
  });

  server.on("/ledon", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(ledPin, ledOn);
    delay(100);
  });

  server.on("/ledoff", [](){
    server.send(200, "text/html", mainPage);
    digitalWrite(ledPin, ledOff);
    delay(100);
  });

  server.on("/button", [](){
    buttonPage = "";
    buttonPage += "Button State is : " + String(digitalRead(buttonPin));
    server.send(200, "text/html", buttonPage);
    delay(100);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  if(millis()-lastmillis > interval){
      Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
      lastmillis=millis();
  }
  
}