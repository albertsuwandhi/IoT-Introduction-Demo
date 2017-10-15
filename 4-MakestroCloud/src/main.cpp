/*
Send BMP180 readings to Makestro cloud and visualize them to FreeBoard
Control on board LED using FreeBoard Button Widget
Written by : Albert Suwandhi
Date : 16 Sept 2017
*/


#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPectro.h>
#include "PubSubClient.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>

// Update these with values suitable for your poject
#define LEDPin 15
#define FREQ 5000 //Publish every 3 seconds
#define SEALEVEL_PRESS 101500
#define LED_ON HIGH
#define LED_OFF LOW

const char* ssid =  "SSID";
const char* password =  "PASS";
const char* mqtt_server = "cloud.makestro.com";
const char* server_username = "USER"; //your username
const char* server_password = "KEY"; //your key
const char* data_topic = "TOPIC "; //your topic
const char* control_topic = "TOPIC "; //your topic

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BMP085 bmp;
ESPectro board(ESPectro_V3);
int value;
float T; //Temperature
int P,A; //Pressure, Alti
String tString = "";
long last=0;
long now=0;
long currentTime = 0, lastTime = 0;
//bool needtoPublish = false;
bool buttonState = false;

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
//void intHandler();

void setup() {
  board.turnOffAllNeopixel();
  pinMode(LEDPin, OUTPUT); // Initialize the LEDPin pin as an output
  Serial.begin(115200);
  if (!bmp.begin()){
    Serial.println("Could not find a valid BMP180 sensor");
    while(1){}
  }    
  Serial.println("BMP180 Sensor Initialized");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char json [100];
  //String json;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    json[i]= (char)payload[i];
  }
  //String json = String((char*)payload).substring.(0,length);
  Serial.println();
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  else{
    Serial.println("Parsing Success");
  }
  // Fetch values.
  if (root.containsKey("LED")){
  value = root["LED"];
  Serial.print("Value is : ");
  Serial.println(value);
  }
  else {
  Serial.println("Invalid JSON");
  }
  if (value == 1) {
    digitalWrite(LEDPin, HIGH);   
  } else {
    digitalWrite(LEDPin, LOW);  
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    
    if (client.connect("ESPectroCore",server_username, server_password)) {
      Serial.println("connected");
      client.subscribe(control_topic);
    } else {
      Serial.print("failed, retry count =");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
    
  }
}
void loop() {
    currentTime = millis();
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
  //Push data to Cloud every FREQ seconds!
    now=millis();
    if (now-last>FREQ){
          T = bmp.readTemperature();
          P = bmp.readPressure();
          A = bmp.readAltitude(SEALEVEL_PRESS);
          Serial.println(A);
          tString = "{\"T\":";
          tString = tString + String(T)+",";
          tString = tString + "\"P\":";
          tString = tString + String(P)+",";
          tString = tString + "\"A\":";
          tString = tString + String(A);
          tString = tString + "}";
          Serial.println(tString);
          client.publish(data_topic, tString.c_str());
          last=millis();
          Serial.println("Published BMP180 sensor readings");
}
}
