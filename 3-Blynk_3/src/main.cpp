#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ESPectro.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define BLYNK_PRINT Serial
#define INTERVAL 2000 //every 2 seconds
#define MILLIS V1
#define ADC V2
#define TEMP V3 
#define HUMI V4
#define ALTI V5
#define ADCLimit 500
#define NOTIFICATION 1
#define SEALEVEL_PRESS 101500

ESPectro board(ESPectro_V3);
Adafruit_BMP085 bmp;
char auth[] = "AUTH"; //your key
char ssid[] = "SSID"; //your SSID
char pass[] = "PASS"; //your password
long lastread=0;

BLYNK_READ(MILLIS)
{
Blynk.virtualWrite(MILLIS, millis() / 1000);
}

void setup()
{
  board.turnOffAllNeopixel();
  Serial.begin(115200);
  
  if (!bmp.begin()){
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while(1){}
  }    
  Serial.println("BMP180 Sensor Initialized");
  //delay(1000);
  Blynk.begin(auth, ssid, pass);
  while(!Blynk.connected()){
  Serial.print(".");
  }
}

void loop()
{
  Blynk.run();

  //Push Sensor Readings
  if (millis()-lastread>INTERVAL){
     lastread=millis();
     int ADCValue=board.readAnalog();
     Serial.print("ADC Value is : ");
     Serial.println(ADCValue);
     Blynk.virtualWrite(ADC, ADCValue);
//     #ifdef NOTIFICATION
     if (ADCValue>ADCLimit){
         Blynk.tweet("ADC current value is " + String(ADCValue) + " and it is above limit #IoT");
         Blynk.notify("It is very bright!!!");
     }

//	 #endif
	//BMP180
	int T = bmp.readTemperature();
	int H = bmp.readPressure();
	int A = bmp.readAltitude(SEALEVEL_PRESS);
	//Write to Blynk - Push
	Blynk.virtualWrite(TEMP,T);
	Blynk.virtualWrite(HUMI,H);
	Blynk.virtualWrite(ALTI,A);

	//Print to Serial Port
	Serial.print("Temperature = ");
	Serial.print(T);
	Serial.println(" *C");

	Serial.print("Pressure = ");
	Serial.print(H);
	Serial.println(" Pa");

	Serial.print("Altitude = ");
	Serial.print(A);
	Serial.println(" meters");
    
  }
  
}

