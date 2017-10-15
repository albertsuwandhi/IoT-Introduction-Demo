/*
Post Temperature, Humidity and Up Time to ThingSpeak
Written by : Albert Suwandhi
Date       : 16 Sept 2017
*/

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_BMP085.h>
#include <ESPectro.h>

char ssid[] = "SSID";    //  your network SSID (name) 
char pass[] =  "PASS";   //  your password
float Temperature;
long Pressure;
float Altitude;
//ThingSpeak Channel and API Key
unsigned long myChannelNumber = 123456789;  //your channel number
const char * myWriteAPIKey = "12345678"; //your key
WiFiClient client;
Adafruit_BMP085 bmp;
ESPectro board(ESPectro_V3);

void setup(){
    board.turnOffAllNeopixel();
    Serial.begin(115200);
    WiFi.begin(ssid,pass);
    long time=millis();
    if (!bmp.begin()){
        Serial.println("Could not find a valid BMP180 sensor");
        while(1){}
    }    
    Serial.println("BMP180 Sensor Initialized");
    while(WiFi.status()!=WL_CONNECTED&&millis()-time<5000){
        delay(1000);
        Serial.print(".");
    }
    if(WiFi.status()!=WL_CONNECTED){
        Serial.println("Failed to connect to WiFi");
        return;

    }
    Serial.println("WiFi Connected");
    Serial.println(WiFi.localIP());
    ThingSpeak.begin(client);
}

void loop(){
    Temperature = bmp.readTemperature();
    Pressure = bmp.readPressure();
    Altitude = bmp.readAltitude(101500);
    int Minutes = millis()/60000;
    ThingSpeak.setField(1,Minutes); 
    ThingSpeak.setField(2,Temperature);
    ThingSpeak.setField(3,Pressure);
    ThingSpeak.setField(4,Altitude);
    // Write all the fields to ThingSpeak
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    delay(20000); // ThingSpeak will only accept updates every 15 seconds
}





