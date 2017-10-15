/*
Simple Blynk Demo 2
Control on Board NeoPixel by using slider and ZERGBA
Send String from App terminal to device - Device will print it to serial port
Written by : Albert Suwandhi
Date       : 14 September 2017
*/

#include <ESPectro.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define pixelno0 0
#define pixelno1 1
#define RGB0 V1
#define RED1 V2
#define GREEN1 V3
#define BLUE1 V4
#define TERMINAL V5

ESPectro board(ESPectro_V3);

long red0,green0,blue0,neopixelcolor0=0;
long red1,green1,blue1, neopixelcolor1=0;
char auth[]= "XXXXXXXXXX";  //your key
const char* ssid="SSID"; //your SSID
const char* password="PASS"; //your password

WidgetTerminal terminal(TERMINAL);

BLYNK_WRITE(RGB0){
  red0=param[0].asInt();
  green0=param[1].asInt();
  blue0=param[2].asInt();
  neopixelcolor0=(red0<<16)|(green0<<8)|(blue0);
  board.turnOnNeopixel(HtmlColor(neopixelcolor0),pixelno0);
}

BLYNK_WRITE(RED1){
  red1=param[0].asInt();
  neopixelcolor1=neopixelcolor1&0x00FFFF;
  neopixelcolor1=neopixelcolor1|(red1<<16);
  board.turnOnNeopixel(HtmlColor(neopixelcolor1),pixelno1);
}

BLYNK_WRITE(GREEN1){
  green1=param[0].asInt();
  neopixelcolor1=neopixelcolor1&0xFF00FF;
  neopixelcolor1=neopixelcolor1|(green1<<8);
  board.turnOnNeopixel(HtmlColor(neopixelcolor1),pixelno1);
}

BLYNK_WRITE(BLUE1){
  blue1=param[0].asInt();
  neopixelcolor1=neopixelcolor1&0xFFFF00;
  neopixelcolor1=neopixelcolor1|blue1;
  board.turnOnNeopixel(HtmlColor(neopixelcolor1),pixelno1);
}

BLYNK_WRITE(TERMINAL){
  Serial.println(param[0].asString());
  terminal.print("You typed : ");
  terminal.println(param[0].asString());
  terminal.flush();
}

void setup(){
board.turnOffAllNeopixel();
Blynk.begin(auth,ssid,password);
while(!Blynk.connected()){}
Serial.begin(115200);
while(!Serial){}
Serial.print("IP Address : ");
Serial.println(WiFi.localIP());
terminal.println("Please type your message");
terminal.flush();
}

void loop(){
  Blynk.run();
}