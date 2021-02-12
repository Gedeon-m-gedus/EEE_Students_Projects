// sudo chmod a+rw /dev/ttyACM0

// ________________LIBLARIES__________________
#include <Wire.h> //  For I2C communication
#include <LiquidCrystal_I2C.h> // Liquid crystal Display

LiquidCrystal_I2C lcd(0x27,20,4); // Defining the address line and screen size








// _____________PINS DECLARATION_______________
const int relayPin = 7, buzzer = 6, battery = A2, button = A3;






// VARIABLES DECLARATION_______________
bool mode = true;  // Mode flag true=charging, false=usage mode
int batLevel = 0; // this will be percentile battery capacity
float batVolts = 0; // This will be the battery capatity in volts
double volt = 0.0;
int counter = 0, loopDelay=500;






  
void setup() {
Serial.begin(9600);
  
  lcd.init();                      // initialize the lcd 
  
  // Print a welcome message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Battery Charger");
  lcd.setCursor(0,1);
  lcd.print("&Level Indicator");


  pinMode(relayPin,OUTPUT);
  pinMode(buzzer,OUTPUT);
  
  digitalWrite(relayPin, LOW);
  digitalWrite(buzzer, LOW);
  
  digitalWrite(relayPin, HIGH);
  delay(1000);
  batVolts = analogRead(battery);
  batVolts = map(batVolts,0,1023,0,5);
  Serial.print(batVolts);
  delay(3000);
  while (batVolts<1){
    batVolts = analogRead(battery);
    batVolts = map(batVolts,0,1023,0,5);
    Serial.print(batVolts);
    lcd.clear();
    lcd.print("   NO BATTERY");
    lcd.setCursor(4,1);
    lcd.print("DETECTED");
    delay(300);
  }
batLevel = map(batVolts, 0, 4, 0, 200);
WELCOME();
digitalWrite(relayPin, LOW);
 
  
}













void loop() {
counter = counter + 1;
if (counter == 100){
 counter = 0;
 Serial.println("___________________UPDATING THE BATT LEVEL_________");
updateBatteryLevel();
}    

//___________FLIPPING THE MODE WITH A BUTTON________________  
 if(digitalRead(button)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(button)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(button)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(button)==HIGH);  //Confirm button release
//button's task here
mode = not mode;
      }
}







//___________________CHARGING MODE_____________________
if (mode){
  Serial.println("CHARGING MODE");
  displayScreen(mode, batLevel);
  batterylevel(15,1,volt);
  volt = volt + 0.7;
  if (volt>5){
    volt = 0.0;
  }

  
}








//__________________BATTERY USAGE MODE_________________
else{
  Serial.println("BATTERY USAGE MODE");
  displayScreen(mode, batLevel);
  batterylevel(15,1,batVolts);
}


delay(loopDelay);
}

//________________________DISPLAY FUNCTION________________________
bool displayScreen(bool charging, int v){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Batt Level:");lcd.print(v);lcd.print("%");
  if (charging==true){
    lcd.setCursor(0,1);lcd.print("Charging");
  }
  else{
    lcd.setCursor(0,1);lcd.print("In Usage mode");
  }
}


void updateBatteryLevel(){
    digitalWrite(relayPin, HIGH);
    delay(200);
    batVolts = analogRead(battery);
    batVolts = map(batVolts,0,1023,0,4);
    if (mode){
      batLevel = batLevel + 1;
    }
    else{
      batLevel = batLevel - 1;
    }
    // map(batVolts, 0, 4, 0, 100);
    digitalWrite(relayPin, LOW);

}

//____________FUNCTION FOR DISPLAY CUSTOM CHARACTER OF A BATTERY_________

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
  //https://www.hackster.io/thearduinoworld/arduino-addons-battery-level-indicator-756b76
}







//____________HOMESCREEN FUNCTION_________________
void WELCOME(){
    lcd.clear();
    lcd.print("BATTERY FOUND");
    lcd.setCursor(0,1);
    lcd.print("....");
    delay(500);
    lcd.print("....");
    delay(100);
    lcd.print("....");
    delay(300);
    lcd.print("....");
    delay(1000);
    lcd.print("..");
    delay(300);
    lcd.print("....");
    delay(1000);
    lcd.print("...");
    lcd.clear();
    
    lcd.print("CHECKING MODULES");
    lcd.setCursor(0,1);
    lcd.print("....");
    delay(500);
    lcd.print("..");
    delay(500);
    lcd.print("..");
    delay(300);
    lcd.print("...");
    delay(1000);
    lcd.print("..");
    delay(300);
    lcd.print("..");
    delay(1000);
    lcd.print("...");
    delay(1000);
    lcd.print("...");
    lcd.clear();
    lcd.print(" SYTEM WORKING");
    delay(2000);
}
