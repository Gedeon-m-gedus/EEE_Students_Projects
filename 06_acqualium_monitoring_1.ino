#include <Wire.h>                    // Include Wire library (required for I2C devices)
#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
#include <DallasTemperature.h>
#include <OneWire.h>
#include <ESP8266WiFi.h>

#define DS18B20 4 //temp sensor is connectedto d2 gpio 4
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensorValue,count=0;float turbidity_value,temp;
const int buzzer = D5, redLED = D6, yellowLED = D7, greenLED = D8;


String apiKey = "1NQ2LNRGKSQRGUYM";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "Thegiants_Electronics";     // Enter your WiFi Network's SSID
const char* pass =  "Thegiants2019$"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
WiFiClient client;

OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);

void setup() {
 Serial.begin(115200);
 Wire.begin(0, 2);      // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
 lcd.begin(16, 2);
 sensor.begin();                  
 lcd.backlight(); 
 
 pinMode(buzzer,OUTPUT);
 pinMode(redLED,OUTPUT);
 pinMode(yellowLED,OUTPUT);
 pinMode(greenLED,OUTPUT);
 
 test_displays();

 WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            lcd.clear();
            Serial.print("*");
            lcd.print("No connectivity!");
            delay(1000);
     }
      Serial.println("");
      Serial.println("***WiFi connected***");
      lcd.clear();
      Serial.print("*");
      lcd.print("Network conected");
      delay(2000);
      
}


void loop() {lcd.clear();
  sensorValue = analogRead(A0);
  turbidity_value = sensorValue * (100.0 / 1024.0);
  sensor.requestTemperatures();
  temp = sensor.getTempCByIndex(0);
  
  Serial.println ("Sensor Output (%):");
  Serial.println (turbidity_value);
  Serial.println ("temperature:");
  Serial.println(temp);
  Serial.println(count);
   lcd.print("Turbidity:");
   lcd.print(turbidity_value);
   lcd.print("%");
   lcd.setCursor(0,1);
   lcd.print("Temperature:");
   lcd.print(temp);
   delay(1000);
   count=count+1;
   push_data_to_cloud();
}

void test_displays(){
 lcd.clear();
 lcd.print("Testing module..");
 digitalWrite(redLED,HIGH);
 digitalWrite(yellowLED,HIGH);
 digitalWrite(greenLED,HIGH);
 lcd.setCursor(0,1);
 lcd.print(".."); delay(200);
 lcd.print(".."); delay(200);
 lcd.print(".."); delay(200);
 lcd.print("."); delay(800);
 lcd.print("."); delay(50);
 lcd.print(".."); delay(100);
 lcd.clear();
 digitalWrite(redLED,LOW);
 digitalWrite(yellowLED,LOW);
 digitalWrite(greenLED,LOW);
}


void push_data_to_cloud(){
  if (client.connect(server,80) && count>5)   //   "184.106.153.149" or api.thingspeak.com
                      {  
                             count=0;
                             lcd.clear();
                             lcd.print("Sending data...");
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temp);
                             postStr +="&field2=";
                             postStr += String(turbidity_value);
                             postStr += String(" ");
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
                               // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
                             delay(10000);
                        }
          client.stop();
 
          Serial.println("Waiting...");
}

