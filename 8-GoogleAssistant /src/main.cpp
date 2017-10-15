/*
Simple Google Assistant Demo
Integrate with IFTTT : Google Assistant and WebHook - HTTP GET with Blynk API
Turn on/off on board LED (GPIO 15) using command
Written by : Albert Suwandhi
Date       : 14 September 2017
*/

#include <ESPectro.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

ESPectro board(ESPectro_V3);
char auth[]= "1234567890"; //your key
const char* ssid= "SSID"; //SSID
const char* password= "PASS"; //Password

void setup(){
    board.turnOffAllNeopixel();
    Blynk.begin(auth,ssid,password);
    while(!Blynk.connected()){}
    Serial.begin(115200);
    while(!Serial){}
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
}

void loop(){
  //Use web Hook from IFTTT : HTTP GET http://blynk-cloud.com/auth_token/update/pin?value=value 
  //If the command from Google Assistant matches the predefined commands
  Blynk.run();
}