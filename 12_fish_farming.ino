#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial 
#include <SPI.h>;
#include <BlynkSimpleEsp8266.h>;
#include <SimpleTimer.h>; 
#define DS18B20 13  //DS18B20 is connected to GPIO Pin 13 correspond to D7
int relayInput = 4; // the input to the relay pin D2  (GPIO4)
const int buzzer = D6;
#define cooler  14
#define LED1 D1 // Led indication normal temperature in NodeMCU at pin GPIO5 (D1)
#define LED2 D0 // Led indication normal temperature in NodeMCU at pin GPIO16 (D0)
#include <Wire.h>                    // Include Wire library (required for I2C devices)
#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
 LiquidCrystal_I2C lcd(0x27, 20, 4);  // Configure LiquidCrystal_I2C library with 0x27 address, 20 columns and 4 rows

#define button1    3                 // Button B1 is connected to GPIO3 (RX)
#define button2    1                 // Button B2 is connected to GPIO1 (TX)

String apiKey = "EUSSYTP7ZJ5XAO1F";     //  Enter your Write API key from ThingSpeak
//blynk key for elie
//"8746300d4eac4c5bb0cc814d8cd2f02b",
char auth[] = "fadcd288842e4288b673868b515f97e6";   // code from blynk app
const char* ssid =  "Thegiants_Electronics";     // Enter your WiFi Network's SSID
const char* pass =  "Thegiants2019$"; // Enter your WiFi Network's Password
const char* server = "api.thingspeak.com";
float temp;
int pHSense = A0; 
OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);
WiFiClient client;
SimpleTimer timer;
void sendSensor()
{
  sensor.requestTemperatures();
      temp = sensor.getTempCByIndex(0);
}
 
void setup() 
{
 Serial.begin(9600);
 pinMode(button1, INPUT);
  pinMode(button2, INPUT);
 delay(1000);
 pinMode(buzzer, OUTPUT);
 pinMode(LED1, OUTPUT); // set the digital pin as output
 pinMode(LED2, OUTPUT); // set the digital pin as output
  pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT
  Wire.begin(0, 2);      // Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
  //lcd.begin(20, 4);                  
 lcd.backlight();                   // Turn backlight ON
 lcd.print("SMART FISH FARMING");
   lcd.setCursor(0, 1);
  lcd.print("   done by"); 
   lcd.setCursor(0, 2);
  lcd.print("CLAIRE, JOSELYNE"); 
  lcd.setCursor(0, 3);
  lcd.print("  and SAFARI");
 delay(2000);
 lcd.clear();
 Blynk.begin(auth, ssid, pass);
 sensor.begin();
 // Setup a function to be called every second
 timer.setInterval(1000L, sendSensor);
 
 Serial.print("Connecting to: ");
  lcd.print("CONNECTING TO NETWORK");
  lcd.setCursor(0, 1);
  lcd.print("...............");
  lcd.setCursor(0, 2);
 Serial.println(ssid);
 WiFi.begin(ssid, pass);
 while (WiFi.status() != WL_CONNECTED) 
     {
            delay(100);
            Serial.print("*");
     }
      Serial.println("");
      Serial.println("WiFi connected");
      lcd.print("WiFi connected");
      delay(1000);
      lcd.clear();
 }

/*RTC INTERFACE*/
char Time[]     = "TIME:  :  :  ";
char h,m,s,hh,mm,ss;
char d,dd,mt,mtt,y,yy;
char Calendar[] = "DATE:  /  /20  ";
byte i, second, minute, hour, date, month, year;
 
void DS3231_display(){
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion
  Time[12]     = second % 10 + 48;
  Time[11]     = second / 10 + 48;
  Time[9]      = minute % 10 + 48;
  Time[8]      = minute / 10 + 48;
  Time[6]      = hour   % 10 + 48;
  Time[5]      = hour   / 10 + 48;
  Calendar[14] = year   % 10 + 48;
  Calendar[13] = year   / 10 + 48;
  Calendar[9]  = month  % 10 + 48;
  Calendar[8]  = month  / 10 + 48;
  Calendar[6]  = date   % 10 + 48;
  Calendar[5]  = date   / 10 + 48;

h=Time[5] ;
hh=Time[6] ;
m=Time[8];
mm=Time[9];
s=Time[11];
ss=Time[12];

d=Calendar[5];
dd=Calendar[6];
mt=Calendar[8];
mtt=Calendar[9];
y=Calendar[13];
yy=Calendar[14];

//  lcd.setCursor(0, 0);
//  lcd.print(Time);                               // Display time
//  lcd.setCursor(0, 1);
//  lcd.print(Calendar);                           // Display calendar
lcd.setCursor(5, 0);
lcd.print("__SENSOR__");
lcd.setCursor(0, 0);
lcd.print("Date");
lcd.print("|");
lcd.setCursor(0, 1);
lcd.print(" ");lcd.print(d);lcd.print(dd);
lcd.print(" |");
lcd.setCursor(0, 2);
lcd.print(" ");lcd.print(mt);lcd.print(mtt);
lcd.print(" |");
lcd.setCursor(0, 3);
lcd.print("20");lcd.print(y);lcd.print(yy);
lcd.print("|");

lcd.setCursor(15, 0);
lcd.print("|");
lcd.print("Time");
lcd.setCursor(15, 1);
lcd.print("|");
lcd.print(" ");
lcd.print(h);lcd.print(hh);
lcd.setCursor(15, 2);
lcd.print("|");
lcd.print(" ");
lcd.print(m);lcd.print(mm);
lcd.setCursor(15, 3);
lcd.print("|");
lcd.print(" ");
lcd.print(s);lcd.print(ss);
}
 // button to set time and date 
void blink_parameter(){
  byte j = 0;
  while(j < 10 && digitalRead(button1) && digitalRead(button2)){
    j++;
    delay(25);
  }
}
byte edit(byte x, byte y, byte parameter){
  char text[3];
  while(!digitalRead(button1));                  // Wait until button1 released
  while(true){
    while(!digitalRead(button2)){                // If button2 is pressed
      parameter++;
      if(i == 0 && parameter > 23)               // If hours > 23 ==> hours = 0
        parameter = 0;
      if(i == 1 && parameter > 59)               // If minutes > 59 ==> minutes = 0
        parameter = 0;
      if(i == 2 && parameter > 31)               // If date > 31 ==> date = 1
        parameter = 1;
      if(i == 3 && parameter > 12)               // If month > 12 ==> month = 1
        parameter = 1;
      if(i == 4 && parameter > 99)               // If year > 99 ==> year = 0
        parameter = 0;
      sprintf(text,"%02u", parameter);
      lcd.setCursor(x, y);
      lcd.print(text);
      delay(200);                                // Wait 200ms
    }
    lcd.setCursor(x, y);
    lcd.print("  ");                             // Display two spaces
    blink_parameter();
    sprintf(text,"%02u", parameter);
    lcd.setCursor(x, y);
    lcd.print(text);
    blink_parameter();
    if(!digitalRead(button1)){                   // If button1 is pressed
      i++;                                       // Increament 'i' for the next parameter
      return parameter;                          // Return parameter value and exit
    }
  }
}

 
void loop() 
{
  int measuringVal = analogRead(pHSense);
    Serial.println("Measuring Raw Value > ");
    Serial.print(measuringVal);
     double vltValue = 5/1024.0 * measuringVal;
    Serial.print("Voltage Value > ");
    Serial.print(vltValue, 3);
 
    float P0 = 7 + ((2.5 - vltValue) / 0.18);
    Serial.print("");
    Serial.print("pH Value ");
    Serial.print(P0, 3);
    Serial.print("");
    //delay(1000);

if(!digitalRead(button1)){                           // If button1 is pressed
    i = 0;
    hour   = edit(5, 0, hour);
    minute = edit(8, 0, minute);
    date   = edit(5, 1, date);
    month  = edit(8, 1, month);
    year   = edit(13, 1, year);
 
    // Convert decimal to BCD
    minute = ((minute / 10) << 4) + (minute % 10);
    hour = ((hour / 10) << 4) + (hour % 10);
    date = ((date / 10) << 4) + (date % 10);
    month = ((month / 10) << 4) + (month % 10);
    year = ((year / 10) << 4) + (year % 10);
    // End conversion
 
    // Write data to DS3231 RTC
    Wire.beginTransmission(0x68);               // Start I2C protocol with DS3231 address
    Wire.write(0);                              // Send register address
    Wire.write(0);                              // Reset sesonds and start oscillator
    Wire.write(minute);                         // Write minute
    Wire.write(hour);                           // Write hour
    Wire.write(1);                              // Write day (not used)
    Wire.write(date);                           // Write date
    Wire.write(month);                          // Write month
    Wire.write(year);                           // Write year
    Wire.endTransmission();                     // Stop transmission and release the I2C bus
    delay(200);                                 // Wait 200ms
   }
 
   Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  Wire.read();                                  // Read day from register 3 (not used)
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
 
  DS3231_display();                             // Diaplay time & calendar
 
  delay(50);                                    // Wait 50ms
  
      sensor.requestTemperatures();
      temp = sensor.getTempCByIndex(0);
      String state;
      if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
      {  
       String sendData = apiKey+"&field1="+String(temp)+"\r\n\r\n"; 
       
       //Serial.println(sendData);

       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(sendData.length());
       client.print("\n\n");
       client.print(sendData);

       Serial.print("Temperature: ");
       Serial.print(temp);
       Serial.println("deg C. Connecting to Thingspeak..");
       Serial.println("14CORE | pH Sense");

        
        if (temp >= 28 && temp <=30)
        {
          digitalWrite(LED2, HIGH);
          digitalWrite(LED1, LOW);
           digitalWrite(buzzer, LOW);
        state = "Normal";
        }
      if(temp < 28){
        digitalWrite(LED2, LOW);
        digitalWrite(LED1, HIGH);
         digitalWrite(buzzer, HIGH);
        state = "   Low";}
      if(temp > 30){
        state = "  High";
        digitalWrite(LED2, LOW);
        digitalWrite(LED1, HIGH);
         digitalWrite(buzzer, HIGH);
        }
        
        // You can send any value at any time.
 // Please don't send more that 10 values per second.
 Blynk.virtualWrite(V5, pHSense); // Humidity for gauge
 Blynk.virtualWrite(V6, temp); // Temperature for gauge
 Blynk.virtualWrite(V7, pHSense); // Humidity for graph
 Blynk.virtualWrite(V8, state); // Temperature for graph
  delay(1000);
      Blynk.run(); // Initiates Blynk
 timer.run(); // Initiates SimpleTimer
      }
      
      client.stop();

      Serial.println("Sending....");
  
lcd.setCursor(6, 2);
lcd.print("T=");
lcd.print(temp,2);
lcd.print("C");
lcd.setCursor(6, 3);
//lcd.print("PH=");
//lcd.print(pHSense,1);
lcd.print(state);
//lcd.print("%");

actuators();
 //delay(15000);
}

//function describing HEATER AND COOLERS
void actuators()
{ if (temp<28)
      {digitalWrite(relayInput, HIGH); // turn relay on
       //digitalWrite(LED, LOW);
      digitalWrite(14,LOW);   // COOLER PIN LOW
      Serial.print("Temperature is Below the normal ");
      Serial.print("TURN HEATER ON ");
        }
      if (temp>=28 && temp<=30)
      {digitalWrite(relayInput, LOW); // turn relay off
       //digitalWrite(LED, HIGH);
      Serial.print("Temperature is normal ");
     }  
       if (temp>30)
       {digitalWrite(14,HIGH); //COOLER PIN HIGH
       // digitalWrite(LED, LOW);
        Serial.print("Temperature is above the normal ");
        Serial.print("TURN COOLER ON ");
        } 
  }

