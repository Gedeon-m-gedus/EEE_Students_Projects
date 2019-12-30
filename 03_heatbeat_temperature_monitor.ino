
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
int heart_beat,heart_beat_pin=A0 ;
const int rs = D0, en = D1, d4 = D2, d5 = D3, d6 = D4, d7 = D5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define DS18B20 13   


float temp;
 
OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);
 
void setup() 
{ sensor.begin();
  pinMode(heart_beat_pin,INPUT);
lcd.clear();
 lcd.begin(16, 2);
 Serial.begin(9600);
lcd.print("HEART BEAT SENSOR");
Serial.print("in the set up");
delay(2000);
 
       
 
}
 

void loop() {
heartbeatdata();
tepmerature_sensor();
lcd.clear();
Serial.print("running........");
lcd.setCursor(0,0);
lcd.print("temperature:");
lcd.print(temp);
lcd.setCursor(1,1);
lcd.print("heart beat: ");
lcd.print(heart_beat);
delay(200);
}

void heartbeatdata(){
 heart_beat=analogRead(heart_beat_pin); 

}

void tepmerature_sensor() 
{
  
      sensor.requestTemperatures();
      temp = sensor.getTempCByIndex(0);

       Serial.print(temp);
      
}
