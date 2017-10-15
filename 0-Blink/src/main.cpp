/*
Hello World
Blink LED connected to GPIO15
Written by : Albert Suwandhi
Date       : 14 September 2017
*/

#define ESPECTRO 1
#if ESPECTRO
#include <ESPectro.h>
ESPectro board(ESPectro_V3);
#endif
#include <Arduino.h>

#define LED_PIN 15
#define TIME 250

void setup(){
    pinMode(LED_PIN,OUTPUT);
    digitalWrite(LED_PIN,LOW);
    #if ESPECTRO
    board.turnOffAllNeopixel();
    #endif
}

void loop(){
    digitalWrite(LED_PIN,HIGH);
    //board.turnOnLED();
    delay(TIME);
    digitalWrite(LED_PIN,LOW);
    //board.turnOffLED();
    delay(TIME);
}