/*
ESP8266 Telegram Bot
*/
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266TelegramBOT.h>
#include <ESPectro.h>
#include <Adafruit_BMP085.h>

#define LED_PIN 15
#define botName "BOTNAME"
#define botUserName "BOTUSERNAME"
#define botToken "TOKEN" //replace with your bot token
#define adminID "12345678" //your ID

static char SSID[] = "SSID";
static char password[] =  "PASS";
static char hostname[] = "XXXXXXXX"; 
float temperature;
ESPectro board(ESPectro_V3);
Adafruit_BMP085 bmp;
TelegramBOT bot(botToken, botName, botUserName);

void setup() 
{
    board.turnOffAllNeopixel();
    pinMode(LED_PIN, OUTPUT);
	board.turnOffLED();
    Serial.begin(115200);
    if (!bmp.begin()){
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while(1){}
    }    
    Serial.println("BMP180 Sensor Initialized");
	Serial.println("Connecting to " + String(SSID) + " ");
    WiFi.begin(SSID, password);
    // Wait until it is connected to the network
	while (WiFi.status() != WL_CONNECTED) 
	{
	    delay(500);
	    Serial.print(".");
	}
	Serial.println("\n\nWiFi connected.");  
	Serial.print("  IP address: " + String(WiFi.localIP()) + "\n");
	Serial.print("  Host name:  " + String(hostname) + "\n");
	delay(3000);
	// Start the bot and send a message to admin, so we know that ESP8266 has booted.
	bot.begin();
    bot.sendMessage(adminID, "I am online now!", "");
    bot.sendMessage(adminID, "Please enter /info ", "");
}

void loop() 
{
	// React on an incomming message:
	/* 	
		GENERAL
			bot.message[0][0] ... number of received messages
			bot.message[0][1] ... code of last message
		MESSAGE
			bot.message[i][0] ... update ID
			bot.message[i][1] ... user ID
			bot.message[i][2] ... first name
			bot.message[i][3] ... last name
			bot.message[i][4] ... user ID
			bot.message[i][5] ... message
	*/
	bot.getUpdates(bot.message[0][1]);
	for (int i = 1; i < bot.message[0][0].toInt() + 1; i++)
	{
		// Only the admin is allowed to talk to the bot
		if (bot.message[i][4] == adminID)
		{
			// extract the message
			bot.message[i][5] = bot.message[i][5].substring(1, bot.message[i][5].length());
			if (bot.message[i][5] == "info")
			{
				bot.sendMessage(bot.message[i][4], "Hello, I am your Bot", "");
				bot.sendMessage(bot.message[i][4], "Please type /help for commands", "");
            }
            else if ((bot.message[i][5] == "help"))
			{
                bot.sendMessage(bot.message[i][4], " /info - get info about this bot" , "");
                bot.sendMessage(bot.message[i][4], " /help - list all available commands" , "");
                bot.sendMessage(bot.message[i][4], " /turnonled - turn on on board LED" , "");
                bot.sendMessage(bot.message[i][4], " /turnoffled - turn of on board LED" , "");
                bot.sendMessage(bot.message[i][4], " /gettemperature - get the current temperature" , "");
                bot.sendMessage(bot.message[i][4], " /getuptime - get the system uptime" , "");
            }
            else if ((bot.message[i][5] == "turnonled"))
			{
				board.turnOnLED();
				bot.sendMessage(bot.message[i][4], "LED has been turned on", "");
            }
            else if ((bot.message[i][5] == "turnoffled"))
			{
				board.turnOffLED();
				bot.sendMessage(bot.message[i][4], "LED has been turned off", "");
            }
            else if ((bot.message[i][5] == "gettemperature"))
			{
                temperature = bmp.readTemperature();
				bot.sendMessage(bot.message[i][4], "Temperature :  " + String(temperature) + " C", "");
			}
			else if ((bot.message[i][5] == "getuptime"))
			{
				unsigned long runtime = millis() / 1000;
				bot.sendMessage(bot.message[i][4], "Uptime: " + String(runtime) + "sec.", "");
			}
			else 
			{
				bot.sendMessage(bot.message[i][4], "Command unknown.", "");
			}
		}
		// If any other person sends something to your bot, let them know they are not allowed to do so!!
		else 
		{
			String senderID = bot.message[i][4];
			bot.sendMessage(bot.message[i][4], "Access denied. Your ID: " + senderID, "");
			//
		}
	}
	bot.message[0][0] = "";
	delay(500);
}
