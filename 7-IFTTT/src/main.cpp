#include <ESP8266WiFi.h>
#include <ESPectro.h>
#define Switch_Pin  0     // the number of the pushbutton pin
#define LED_Pin     15     // the number of the LED pin

ESPectro board(ESPectro_V3);

const char ssid[] = "SSID"; //your SSID
const char password[] = "PASS"; //your PASSWORD

// IFTTT setup
const char *host = "maker.ifttt.com";
const char *Maker_Event = "EVENT"; //replace with youe event name
const char *Your_Key = "1234567890"; //replace with your key
int val1 = 100;
int val2 = 200;
int val3 = 300;

// Hardware setup
const int buttonPin = Switch_Pin;     // the number of the pushbutton pin
const int ledPin = LED_Pin ;          // the number of the LED pin

//------- prototype function ---------
void send_event(const char *event);

void setup()
{
  board.turnOffAllNeopixel();
  Serial.begin(115200);
  delay(200);
  //ESP.wdtDisable();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  /*********** Connect to WiFi network ***********/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.scanNetworks(bool async, optional bool show_hidden)

  while (WiFi.waitForConnectResult() != WL_CONNECTED) // check connection with router
  {
    Serial.println("Connection Router Failed! Rebooting...");
    delay(500);
    ESP.restart();
  }

  Serial.println("");
  Serial.println("WiFi Connected. ");
  Serial.print("IP = ");
  Serial.println(WiFi.localIP());
  delay(1000);
  // Set your pin modes
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // turn off LED
  //send_event(Maker_Event);
}

void loop()
{
/* 
  while(1){
  digitalWrite(ledPin, HIGH);
  ESP.wdtFeed();
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  }
*/

  int buttonState  = digitalRead(buttonPin);
  delay(250);
  if (buttonState == LOW)
  {
    Serial.print("Button is pressed!");
    send_event(Maker_Event);
  }
}

void send_event(const char *event)
{
  // set the LED on while we are sending the event
  digitalWrite(ledPin, HIGH);

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  // We now create a URL for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += Your_Key;
  //Value
  url += "/?value1=";
  url += String(val1);
  url += "&";
  url += "value2=";
  url += String(val2);
  url += "&";
  url += "value3=";
  url += String(val3);
  //url += "&"+"value3:"+"val3";


  Serial.print("Requesting URL: ");
  Serial.println(url);


  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  // Read all the lines of the reply from server and print them to Serial,
  // the connection will close when the server has sent all the data.
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    else
    {
      // No data yet, wait a bit
      delay(50);
    };
  }

  // All done
  Serial.println();
  Serial.println("closing connection");

  client.stop();
  // Finished sending the message, turn off the LED
  digitalWrite(ledPin, LOW);
}
