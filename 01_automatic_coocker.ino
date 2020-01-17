#include <SPI.h>
#include <LiquidCrystal.h>
#include <RtcDS3234.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8


const int rs = A0, en = A1, d4 = A2, d5 =A3 , d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const uint8_t DS3234_CS_PIN = 10;
RtcDS3234<SPIClass> Rtc(SPI, DS3234_CS_PIN);

#define btnSetting 9
#define btnHour 6
#define btnMinute 5
#define relayPin 4
#define ledPin 3


char user_on_set_values[4];
char user_off_set_values[4];

char manuel_time_to_compare[5];
int h1on=0,h2on=0,m1on=0,m2on=0;
int h1off=0,h2off=0,m1off=0,m2off=0;
int count=0,comparison=5;

int h1on_address=0,h2on_address=1,m1on_address=2,m2on_address=3;
int h1off_address=4,h2off_address=5,m1off_address=6,m2off_address=7;
int comparisonStore_address=8;

void setup() {
  pinMode(relayPin,OUTPUT);
  pinMode(ledPin,OUTPUT);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" AUTO COOCKER ");
 
 
    h1on=EEPROM.read(h1on_address);
    h2on=EEPROM.read(h2on_address);
    m1on=EEPROM.read(m1on_address);
    m2on=EEPROM.read(m2on_address);

    h1off=EEPROM.read(h1off_address);
    h2off=EEPROM.read(h2off_address);
    m1off=EEPROM.read(m1off_address);
    m2off=EEPROM.read(m2off_address);

    comparison=EEPROM.read(comparisonStore_address);



    user_on_set_values[0]= h1on;
    user_on_set_values[1]= h2on;
    user_on_set_values[2]= m1on;
    user_on_set_values[3]= m2on;

    user_off_set_values[0]= h1off;
    user_off_set_values[1]= h2off;
    user_off_set_values[2]= m1off;
    user_off_set_values[3]= m2off;




  
    Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(2000);
    while (!Serial);
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);
    SPI.begin();
    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    // never assume the Rtc was last configured by you, so
    // just clear them to your needed state
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3234SquareWavePin_ModeNone); 



}







void loop() {
gsm_fct();
  seting_function();
 lcd.clear();
// int p=compareStrings(user_on_set_values,manuel_time_to_compare);
//Serial.println(manuel_time_to_compare);
//Serial.println(String(user_on_set_values));
//Serial.print("_________ONcomparisoon______");
//Serial.println(p);
// int pp=compareStrings(manuel_time_to_compare,user_off_set_values);
//Serial.print("_________OFFcomparisoon______");
//Serial.println(pp);


compare_ON_Time();
compare_OFF_Time();
Conclusion();

    if (!Rtc.IsDateTimeValid()) 
    {
        
        Serial.println("RTC lost confidence in the DateTime!");
    }

    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();

  RtcTemperature temp = Rtc.GetTemperature();
 // temp.Print(Serial);
  //  Serial.println("C");

    delay(1000); // ten seconds

}











#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char my_dates[12],my_time[10],time_to_compare[8];
    snprintf_P(my_dates, 
            countof(my_dates),
            PSTR("%02u/%02u/%04u"),
            dt.Month(),
            dt.Day(),
            dt.Year());
snprintf_P(my_time, 
            countof(my_time),
            PSTR("%02u:%02u:%02u"),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
snprintf_P(manuel_time_to_compare, 
            countof(manuel_time_to_compare),
            PSTR("%02u%02u"),
            dt.Hour(),
            dt.Minute());
snprintf_P(time_to_compare, 
            countof(time_to_compare),
            PSTR("%02u:%02u"),
            dt.Hour(),
            dt.Minute());
          //  time_to_comparee=time_to_compare;
    Serial.println(time_to_compare);
    //Serial.println(time_to_comparee);
   // Serial.println(my_time_to_ring[3]);
    lcd.setCursor(1,0);
    lcd.print(my_time);lcd.print(" |");
    
    lcd.setCursor(11,0);
    lcd.print(h1on);;lcd.print(h2on);lcd.print(":");
    lcd.print(m1on);lcd.print(m2on);

    
    
    lcd.setCursor(0,1);
    lcd.print(my_dates);lcd.print("|");

    
    lcd.setCursor(11,1);
    lcd.print(h1off);;lcd.print(h2off);lcd.print(":");
    lcd.print(m1off);lcd.print(m2off);


    
    Serial.println(my_dates);

   
}













void seting_function(){
      if(digitalRead(btnSetting)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnSetting)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnSetting)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnSetting)==HIGH);  //Confirm button release
while(count<8){
  increasing_time();
  moving_cursor();
  saving_settings();
  lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("ON");
    lcd.setCursor(11,0);
    lcd.print("OFF");
    //lcd.setCursor(14,0);
   
    lcd.setCursor(2,1);
    lcd.print(h1on);;lcd.print(h2on);lcd.print(":");
    lcd.print(m1on);lcd.print(m2on);
    lcd.setCursor(10,1);
    lcd.print(h1off);;lcd.print(h2off);lcd.print(":");
    lcd.print(m1off);lcd.print(m2off);
  delay(100);
}
    }
    count=0;
  }
}




void moving_cursor(){
   if(digitalRead(btnSetting)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnSetting)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnSetting)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnSetting)==HIGH);  //Confirm button release
 count=count+1;
 if(count==8){
  count=0;
 }
}
    }
   
  }



void increasing_time(){
  if(digitalRead(btnHour)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnHour)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnHour)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnHour)==HIGH);  //Confirm button release



//__________on time settings____________
if(count==0){
  //lcd.setCursor(4,1);lcd.blink();
h1on=h1on+1;
if(h1on==3){
  h1on=0;
}
}
if(count==1){
h2on=h2on+1;
if(h2on==10){
  h2on=0;
}
if(h2on==4 && h1on==2){
  h2on=0;
}
}

if(count==2){
m1on=m1on+1;
if(m1on==6){
  m1on=0;
}
}

if(count==3){
m2on=m2on+1;
if(m2on==10){
  m2on=0;
}
}



//__________off time settings____________
if(count==4){
  //lcd.setCursor(4,1);lcd.blink();
h1off=h1off+1;
if(h1off==3){
  h1off=0;
}
}
if(count==5){
h2off=h2off+1;
if(h2off==10){
  h2off=0;
}
if(h2off==4 && h1off==2){
  h2off=0;
}
}

if(count==6){
m1off=m1off+1;
if(m1off==6){
  m1off=0;
}
}

if(count==7){
m2off=m2off+1;
if(m2off==10){
  m2off=0;
}
}

    }
  }
}




void saving_settings(){
  if(digitalRead(btnMinute)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnMinute)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnMinute)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnMinute)==HIGH);  //Confirm button release

lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Set Saved");

    user_on_set_values[0]= h1on;
    user_on_set_values[1]= h2on;
    user_on_set_values[2]= m1on;
    user_on_set_values[3]= m2on;

    user_off_set_values[0]= h1off;
    user_off_set_values[1]= h2off;
    user_off_set_values[2]= m1off;
    user_off_set_values[3]= m2off;

    EEPROMWrite16bits(h1on_address,h1on);
    EEPROMWrite16bits(h2on_address,h2on);
    EEPROMWrite16bits(m1on_address,m1on);
    EEPROMWrite16bits(m2on_address,m2on);

    EEPROMWrite16bits(h1off_address,h1off);
    EEPROMWrite16bits(h2off_address,h2off);
    EEPROMWrite16bits(m1off_address,m1off);
    EEPROMWrite16bits(m2off_address,m2off);

    


    
    delay(2000);
    
}
   
    count=9;
    }
  }











 //___________GSM FUNCTION____________-

 void gsm_fct(){

  mySerial.println("AT"); //Handshaking with SIM900
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  
    while(mySerial.available())
  {
    if(mySerial.find("ON")){
      comparison=0;
    Serial.println("hello rwanda");
    activateActuator();
    lcd.clear();
    lcd.print("Activation");
    delay(500);}
    if(mySerial.find("OFF")){
      comparison=1;
    Serial.println("bye rwanda");
    lcd.clear();
    lcd.print("Disactivation");
    disactivateActuator();
    delay(500);}
    }
}







int compareStrings(String str1, String str2){
  int str1Length = str1.length();
  int str2Length = str2.length();

  if(str1Length == str2Length){
    if (str1[0] - '0'== str2[0] - '0' && str1[1] - '0'== str2[1] - '0' 
  && str1[2] - '0'== str2[2] - '0' && str1[3] - '0'== str2[3] - '0'){
      return 1;
      
    }
    else return -1;
  }
 else
    return -1;
 }



void compare_ON_Time(){
  int c1,c2,c3,c4;
  if (int(manuel_time_to_compare[1])== int(user_on_set_values[1])){
    Serial.print("time1111111111111111 comparison::::::::::::::::");
          Serial.println("matches");
  }
  if (manuel_time_to_compare[0]- '0' == user_on_set_values[0] && manuel_time_to_compare[1]- '0' == user_on_set_values[1] 
  && manuel_time_to_compare[2]- '0' == user_on_set_values[2] && manuel_time_to_compare[3]- '0' == user_on_set_values[3]){
      comparison=0;
      EEPROMWrite16bits(comparisonStore_address, comparison);
     
    }
 Serial.print("returned value..... ");Serial.print(comparison);  Serial.println(".....ok");  
}





 
void compare_OFF_Time(){
  int c1,c2,c3,c4;
  if (int(manuel_time_to_compare[1])== int(user_on_set_values[1])){
    Serial.print("time1111111111111111 comparison::::::::::::::::");
          Serial.println("matches");
  }
  if (manuel_time_to_compare[0]- '0' == user_off_set_values[0] && manuel_time_to_compare[1]- '0' == user_off_set_values[1] 
  && manuel_time_to_compare[2]- '0' == user_off_set_values[2] && manuel_time_to_compare[3]- '0' == user_off_set_values[3]){
      comparison=1;
      EEPROMWrite16bits(comparisonStore_address, comparison);
     
    }
 Serial.print("returned value..... ");Serial.print(comparison);  Serial.println(".....ok");  
}




void activateActuator(){
  digitalWrite(relayPin,HIGH);
  digitalWrite(ledPin,HIGH);
}

void disactivateActuator(){
  digitalWrite(relayPin,LOW);
  digitalWrite(ledPin,LOW);
}


void Conclusion(){
  if(comparison==0){
    activateActuator();
  }
   if(comparison==1){
    disactivateActuator();
  }
}







void EEPROMWrite16bits(int address, int value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> two = Least significant byte
     // byte two = (value & 0xFF);
     // byte one = ((value >> 8) & 0xFF);
      

      //Write the 4 bytes into the eeprom memory.
     // EEPROM.write(address, two);
     // EEPROM.write(address + 1, one);

      EEPROM.write(address, value);
      
      }

uint16_t EEPROMRead16bits(int address)
      {
      //Read the 4 bytes from the eeprom memory.
      uint16_t two = EEPROM.read(address);
      uint16_t one = EEPROM.read(address + 1);
      

      //Return the recomposed long by using bitshift.
      return ((two << 0) & 0xFF) + ((one << 8)& 0xFFFF) ;
      }
