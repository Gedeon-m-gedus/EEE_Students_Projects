#include <SPI.h>
#include <SD.h>
#define btnEnter 11
int my_buzzer=10;
String str1 = "Look Here";
String str2 = "Look Here";
  int g;
  int comparison=50;
  
char str[9];
char strr[9];

char school_alarm_time[6];
char real_time_data[6];
  

File myFile;
char filedata;
String my_data[127],time_val="";
int i=0,p=0,k=0;
String my_time_to_ring[19],time_to_comparee;

const uint8_t DS3234_CS_PIN = 53;
byte bell_character[8] = { B00000,
B00000,
B00100,
B01110,
B01110,
B01110,
B11111,
B00100 };
int b=0;
#include <LiquidCrystal.h>
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <SPI.h>
#include <RtcDS3234.h>

RtcDS3234<SPIClass> Rtc(SPI, DS3234_CS_PIN);

void setup () 
{

pinMode(my_buzzer,OUTPUT);
pinMode(9,OUTPUT);
digitalWrite(9,HIGH);
  lcd.clear();
  lcd.begin(16, 2);
  lcd.print(" SCHOOL BELL ");
  lcd.createChar(b, bell_character);
  delay(2000);
    Serial.begin(115200);
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


//__________________________________________
  pinMode(8, OUTPUT);
  if (!SD.begin(8)) {
    Serial.println("Initialization failed!");
    return;
  }
myFile = SD.open("dataa.csv");
if(myFile) {
    
    //Serial.println();
    
    while (myFile.available()) {
      
      filedata = myFile.read();
      //Serial.print(filedata);
      
        my_data[i]=filedata;
        
        i=i+1;
}
myFile.close();

}

get_time_var();



//____________________________________________

digitalWrite(9,LOW);
}

void loop () {


str1.toCharArray(str, 9);
str2.toCharArray(strr, 9);
  int m = strcmp(str, strr);
  Serial.println(m);
  
  lcd.clear();
lcd.write(b);
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
   k=0;
   while (k<19){
     
time_to_comparee.toCharArray(real_time_data,6);
my_time_to_ring[k].toCharArray(school_alarm_time,6);  
  g = strcmp(real_time_data, school_alarm_time);
  Serial.print("real_time_data: ");
  Serial.println(real_time_data);
  Serial.print("school_alarm_time: ");
  Serial.println(school_alarm_time);
  if(g==0){
    comparison=0;
    k=20;
    delay(100);
  }
  else{
    comparison=10;
  }

  k=k+1;
  }
  read_button();
    if(comparison==0){
   Serial.println("wow runing"); 
   lcd.clear();
   lcd.print("Ringing || ");lcd.print(time_to_comparee);
   digitalWrite(my_buzzer,HIGH);
   delay(3000);
   comparison=10;
  }
  else if(comparison !=0)
  digitalWrite(my_buzzer,LOW);       

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
snprintf_P(time_to_compare, 
            countof(time_to_compare),
            PSTR("%02u:%02u"),
            dt.Hour(),
            dt.Minute());
            time_to_comparee=time_to_compare;
    //Serial.println(time_to_compare);
    //Serial.println(time_to_comparee);
   // Serial.println(my_time_to_ring[3]);
    lcd.setCursor(4,0);
    lcd.print(my_time);
    lcd.setCursor(14,0);
    
    //lcd.print(a);
    lcd.setCursor(3,1);
    lcd.print(my_dates);
    Serial.println(my_dates);
   
}



void get_time_var(){
  for(int j=0; j<127; j++){
//Serial.print(my_data[j]);
time_val=time_val+my_data[j];
//delay(100);
if (my_data[j] == "\n"){
  my_time_to_ring[p]=time_val;
  
  time_val="";
       Serial.print("alarm time");
       Serial.print(my_time_to_ring[p]);
       p=p+1;
      
      }
}
}

void read_button(){
  if(digitalRead(btnEnter)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnEnter)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnEnter)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnEnter)==HIGH);  //Confirm button release
comparison=0;
    }
  }
}

