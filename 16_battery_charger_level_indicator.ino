// sudo chmod a+rw /dev/ttyACM0
#include <LiquidCrystal.h>
//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
const int analogPin = A0;   // the pin that the battery is attached to
int battMeasurementPercent, batMeasuredVoltage;
int counter= 0;
const int relayPin = 11;

// chenge this values
int loopDelay = 500; 
const int timeToCheckBatt = 1;

int checkDel = (timeToCheckBatt * 60000)/loopDelay;

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);


int buttonPressedValue=5,screenFlag=0;
float V,I,P,E;

int portAunit=0,portAtens=0,portBunit=0,portBtens=0;
int p; //variable to know the presed button
double volt = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(relayPin,OUTPUT);
  digitalWrite(relayPin, LOW);

lcd.begin(16, 2);


}

void loop() {

if (counter==checkDel || counter == 0){
  Serial.println("check");
  counter = 0;
digitalWrite(relayPin, LOW);
delay(100);
battMeasurementPercent = measureBatteryPercent();
batMeasuredVoltage = measureBatteryValue();
}

//Serial.print(battMeasurementPercent);Serial.print(" and ");
//Serial.println(batMeasuredVoltage);

bool activity = displayScreen(true ,61);
if (activity==true){
batterylevel(15,1,volt);
volt = volt + 0.7;
if (volt>5){
  volt = 0.0;
}
}
else{
batterylevel(15,1,volt); 
}


counter = counter + 1; 
delay(loopDelay);

}

int measureBatteryPercent(){
    int sensorReading = analogRead(analogPin); 
  int value = map(sensorReading, 0, 1023, 0, 100);
  return value;
}

int measureBatteryValue(){
    int sensorReading = analogRead(analogPin);
  int value = map(sensorReading,0,1023,0,5); 
  return value;
}


bool displayScreen(bool charging, int v){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Batt Level:");lcd.print(v);lcd.print("%");
  if (charging==true){
    lcd.setCursor(0,1);lcd.print("Charging");
  }
  else{
    lcd.setCursor(0,1);lcd.print("NOT Charging");
  }
  return charging;
}







void batterylevel(int xpos,int ypos,double curvolt)
{


  if(curvolt >= 5)
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B10101,
    B10001,
    B11011,
    B11011,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt > 4.2 )
  {
    byte batlevel[8] = {
    B01110,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 4.2)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 3.5)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 2.8)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 2.1)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <= 1.4)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
  if(curvolt <=0.7)
  {
    byte batlevel[8] = {
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    };
    lcd.createChar(0 , batlevel);
    lcd.setCursor(xpos,ypos);
    lcd.write(byte(0));
  }
}
