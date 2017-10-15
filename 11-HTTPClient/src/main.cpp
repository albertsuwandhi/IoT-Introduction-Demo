/*
ESP8266 HTTP CLient 
Make a HTTP GET Request to Server and send its uptime and IP Address
Run server.js to activate the server
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
 
const char* host = "192.168.1.33"; //replace with your HTTP Server IP
const int  port = 8080;
const int   interval = 5000;
unsigned long previousMillis = millis();
 
ESP8266WiFiMulti wifiMulti;
HTTPClient http;
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  // We start by connecting to a WiFi network
  wifiMulti.addAP("ssid1", "psk1");
  wifiMulti.addAP("ssid2", "psk2");
 
  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.print(WiFi.localIP());
  }
  Serial.setDebugOutput(true);
  
}
 
int value = 0;
 
void loop() {
  unsigned long currentMillis = millis();
 
  if ( currentMillis - previousMillis > interval ) {
    previousMillis = currentMillis;
 
    if(wifiMulti.run() != WL_CONNECTED) {
      Serial.println("Not Connected Yet");
      ESP.restart();
    } else {  
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    
      String url = "/watchdog?command=watchdog&uptime=";
      url += String(millis());
      url += "&ip=";
      url += WiFi.localIP().toString();
      
      Serial.print("connecting to ");
      Serial.println(host);
      Serial.print("Requesting URL: ");
      Serial.println(url);
    
      http.begin(host,port,url);
      int httpCode = http.GET();
      if (httpCode) {
        if (httpCode == 200) {
          String payload = http.getString();
          Serial.println(payload);
        }
      }
      Serial.println("closing connection");
      http.end();
      WiFi.disconnect();
    }
  }
}