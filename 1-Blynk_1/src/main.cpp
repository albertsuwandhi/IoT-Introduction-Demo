/*
Simple Blynk Demo 1
Read GPIO0 and send status to Blynk Apps
Control LED connected to GPIO15 from Apps using LEDWidget
Written by : Albert Suwandhi
Date       : 14 September 2017
*/

#define ESPECTRO 1
#if ESPECTRO
#include <ESPectro.h>
#endif
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
#define BUTTON_PIN 0
#define LED_PIN 15
#define LED_VIRTUAL V1

#if ESPECTRO
ESPectro board(ESPectro_V3);
#endif

char auth[] = "AUTH"; //your key
char ssid[] = "SSID"; //your SSID
char pass[] = "PASS"; //your password
char previousButton0 = 0;
long lastread=0;

WidgetLED ledWidget(LED_VIRTUAL);

void setup()
{
  #if ESPECTRO 
  board.turnOffAllNeopixel();
  #endif
  Serial.begin(115200);
  pinMode(BUTTON_PIN,INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  // Read State Changes
  char currentButton0 = digitalRead(BUTTON_PIN);
  if (currentButton0 != previousButton0) {
  if (currentButton0 == 0){
      ledWidget.on();
  }else{
      ledWidget.off();
  }
  previousButton0 = currentButton0;
  }
}
