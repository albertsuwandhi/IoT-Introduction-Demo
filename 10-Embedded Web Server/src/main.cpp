/*
Simple ESP8266 Web Server
*/

#define ESPECTRO

#ifdef ESPECTRO
#include <ESPectro.h>
ESPectro board(ESPectro_V3);
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>
// Replace with your network credentials
#define WIFISSID  "SSID"
#define WIFIPASS  "PASS"
unsigned long startTime = millis();
Adafruit_BMP085 bmp;
ESP8266WebServer server(80);
String webPage = "";

void connectWifi() {
   Serial.println("Connecting to access point");
   WiFi.disconnect();
   if (WiFi.getMode() != WIFI_STA) {
      WiFi.mode(WIFI_STA);
   }
   WiFi.begin (WIFISSID, WIFIPASS );
   //Give ESP 5 seconds to connect to AP
   startTime = millis();
   while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
     delay(1000);
     Serial.print(".");
   }
   Serial.println("");
   // Check connection
   if (WiFi.status() == WL_CONNECTED) {
      Serial.print("WiFi connected; IP address: ");
      Serial.println(WiFi.localIP());
   } else {
      Serial.print("WiFi connect failed to ssid: ");
      Serial.println(WIFISSID);
   }
}

void setup(){  
    #ifdef ESPECTRO
    board.turnOffAllNeopixel();
    #endif
    Serial.begin(115200);
    if (!bmp.begin()){
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while(1){}
    }    
    Serial.println("BMP180 Sensor Initialized");
    //delay(500);
    connectWifi(); //make connection
    Serial.println("");

// Web Server Response
    server.on("/", [](){
        webPage="<H1>ESP8266 Web Server</H1>";
        webPage+="Ready!!!";
        server.send(200, "text/html", webPage);
        ESP.getFreeHeap();
    });

    server.on("/temp", [](){
    // Read temperature
        float t = bmp.readTemperature();
        server.send(200, "text/plain", (String)t);
        ESP.getFreeHeap();
    });
  
    server.on("/press", [](){
        long int p = bmp.readPressure();
        server.send(200, "text/plain", (String)p);
        ESP.getFreeHeap();
    });

    server.begin();
    Serial.println("Embedded HTTP server started");
}
 
void loop(){
  server.handleClient();
}