#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,7,6,5,4,3);
RTC_DS1307 RTC;

const int soil = 0; 
const int pump = 2;
char h,m;
DateTime now;

void setup() {
  pinMode(pump, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  lcd.begin(16, 2);
  lcd.clear();
  
if (! RTC.isrunning())
{
Serial.println("RTC is NOT running!");
RTC.adjust(DateTime("_DATE_ , _TIME_"));
}
}

void loop() {
now = RTC.now();
lcd.clear();
h=now.hour();m=now.minute();
lcd.print("TIME ");lcd.print(now.hour());
lcd.print(":");lcd.print(now.minute());
float moisture = analogRead(soil);
lcd.setCursor(0,1);
lcd.print("Moisture: ");lcd.print(moisture);

pump_control(moisture);
//delay(100);


}

void pump_control(float moisture){
  if(moisture<512 && (0<=now.hour()<=6 || 18<=now.hour()<=23)){
    digitalWrite(pump,HIGH);
  }
  else{
    digitalWrite(pump,LOW);
  }

}
