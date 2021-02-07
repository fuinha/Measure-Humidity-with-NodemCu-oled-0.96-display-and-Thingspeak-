#include <ESP8266WiFi.h> // esp8226 librarry
#include "DHTesp.h" //  dht11 library for ESP8226
#include <Wire.h> //librarry for lcd dosplay 
#include <Adafruit_GFX.h> //oled 0.96 library
#include <Adafruit_SSD1306.h>// oled 0.96 library
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET  -1    // RESET PIN
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dht;
String apiKey = "XXXXXXXXXXX";     //   API key from ThingSpeak
 
const char *ssid =  "XXXXXXXX";     // ssid naam
const char *pass =  "XXXXXXXXXXXXX"; //ssid wachtwoord
const char* server = "api.thingspeak.com";
 
#define DHTPIN 0         //DHT11 sensor pin
 

 
WiFiClient client;
 
void setup() 
{
       Serial.begin(9600);
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
        display.clearDisplay();
       delay(10);
dht.setup(D0, DHTesp::DHT11);

 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 

{    display.clearDisplay();
  
      float humidity = dht.getHumidity(); //variable voor humidity
      
             
 
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field6=";
                             postStr += String(humidity);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

                             // Serialprint
                             Serial.print(" degrees Celcius, Humidity: ");
                             Serial.print(humidity);
                             Serial.println("%. Send to Thingspeak.");

                             //oled 0.96 print

                             display.setTextSize(1);
                             display.setCursor(0,0);
                             display.print("Temperature: ");
                             display.setTextSize(2);
                             display.setCursor(0,10);
                             display.print(humidity);
                             display.print(" % ");
                             
                        }
          client.stop();
 

  
  // elke 10 minuten wordt de data geüpdatet
  delay(600000);
}
