#include <EEPROM.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(29,31,33,35,37,39,41,43,45,47);//lcd in 8 bit mode
//rs,e,d0 to d7

//#include <Wire.h>
#include "RTClib.h"//for rtc clock
RTC_DS3231 rtc;
RTC_Millis rtc1;//to take time from software based on millis()and timer

#include <avr/pgmspace.h>

#include <SD.h>
#include <Keypad.h>

#include <Adafruit_Fingerprint.h>
//#include <LiquidCrystal_PCF8574.h>

#define chipSelect 53
//LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
//defining the keypad symbols
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D',}
};
byte rowPins[ROWS] = {32,34,36,38}; //connect to the row pinouts of the kpd
byte colPins[COLS]= {40,42,30,28}; //connect to the column pinouts of the kpd

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 





//fingerprint to serial communication
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,11);//Tx,Rx pins of mega, GREEN,YELLOW



Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void retakerReg();
int getFingerprintEnroll(int ,int ,int );
int attendance();
//void writingToExcel();
void deleteAllPrints();
long EEPROMReadlong(int address);
void EEPROMWritelong(int address, long value);//int stores 16 bits
void regOptions();
void lecturerReg();
void studentReg();
void retakerReg();
void homeScreen();
void printToXl();
void attendanceToXl(long);

//declaring excel variables
static int inc=3;//increments as number of students on excelsheet is increasing
static char incChar2='C';//keep track of the column of the sessions
 static int session=1;//count number of sessions
 
//static int no=1;//for student number in excel
//static int inc=2;//for the regNo location incrent in excel
  //char str[30];
 // char str2[30];
 // char str1[30];
 // char str3[30];
 // char str4[30];
 // char str5[30];
 // char incChar='D';//for character incrementation
 //  int s=1;//count number of sessions


static long address;
int userCount;

static int lectCounter=0;//has not to exceed 29 (0-30)
static int lessonCounter=0;
static int retakerLessons=0;

static int studCounter=0;//has not to exceed 29 (0-30)

static int retakerCounter=0;

int lectFinger;

int arrCode[10];//array to keep every input data in its array
char key;

//for fingerprint to eeprom

  int i;//for iteration

 //for the rtc to print the exact day in words
 char daysOfTheWeek[7][12] = {"Sund", "Mond", "Tues", "Wedn", "Thur", "Frid", "Satu"};

    
//creating  file pointer
File myfiles1;
File myfiles2;

//for fingerprint to eeprom
static int id=0;//takes all the users'fingerprint


static int lectLoc=0;//lecturer's eeprom position counter
static int lectLoc1=300;
static int lectLoc2=400;
static int lectLoc3=20;//starting location of lecturers'fingerprints 

static int studLoc=30;//students'eeprom position counter
static int studLoc1=500;//location starting for students registration numbers
static int studLoc2=60;//location starting for students fingerprints ID's
static int studLoc3=90;

static int retLoc=120;//retakers'eeprom position counter
static int retLoc1=700;
static int retLoc2=190;
static int retLoc3=900;
static int retLoc4=150;//for the retakers'fingerprint IDs ++++

//adding EEPROM to hold the fingerprints

//student's records

struct students{
        
     
        PROGMEM int studID[30];//take the number of students registered consecutively
        PROGMEM long regNo[30];
        PROGMEM int prints[30];//will take the value of the fingerprint scanned
        }stud;

//lecturer's records
struct lecturer{
        // is the max number of students
       
        PROGMEM long lessonID[10];
        PROGMEM long lectCode[10];//consecutive numbers of the registered lectures
        PROGMEM int prints[10];//will take the value of the fingerprint scanned
        //PROGMEM int lectArray[30];//let the max number of users be 30
        }lect;

//retaker's records

struct retaker{
        
        PROGMEM long lessonID[10];
        PROGMEM int studID[30];//take the number of students registered consecutively
        PROGMEM long regNo[30];
        PROGMEM int prints[30];//will take the value of the fingerprint scanned
        }ret;


void setup() {

  
 
/*
 
 //////////////INITIALIZING THE DEVICE WITH ALL THE SAVED DATA.....HIBERNATING/////////////////
int arrLoc,memLoc;
//****************************WRITING TO LECTURER'S RECORD
//**writing lessonID[10] 0-9
arrLoc=0;
for(memLoc=0;memLoc<10;memLoc++)
{
  
    lect.lessonID[arrLoc]=EEPROM.read(memLoc);//printing data stored by the user from the EEPROM to array...
    arrLoc++;
  
}
//**writing lectCode[10] 10-19
arrLoc=0;
for(memLoc=10;memLoc<20;memLoc++)
{
  
    lect.lectCode[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}
//**writing prints   20-29
arrLoc=0;
for(memLoc=20;memLoc<30;memLoc++)
{
  
    lect.prints[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
  Serial.println("before");
}



//WRITING TO STUDENT RECORDS

//**writing regNo[30]  500-619 EEPROM address locations
arrLoc=0;
for(memLoc=500;memLoc<619;memLoc=+4)//4 locations corresponding to 32 bits location ....long datatype
{
  //reading 32 bits EEPROM address
    stud.regNo[arrLoc]=//EEPROMReadlong(memLoc);//EEPROM.read(memLoc);
  arrLoc++;
  Serial.println("after");
}

//*** writing studID[30] from  60-89 to its specific arrays
arrLoc=0;
for(memLoc=60;memLoc<90;memLoc++)
{
  stud.studID[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}

//**writing prints[30] 90-119
arrLoc=0;

Serial.println("stud.prints[arrLoc]=EEPROM.read(memLoc)");
for(memLoc=90;memLoc<120;memLoc++)
{

  stud.prints[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}



//WRITING TO RETAKER RECORDS

//**writing regNo[30]  120-149
arrLoc=0;
for(memLoc=120;memLoc<150;memLoc++)
{
  
    ret.regNo[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}

//*** writing studID[30] from 150-179
arrLoc=0;
for(memLoc=150;memLoc<180;memLoc++)
{
  
    ret.studID[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}

//*writing lessonID[10] 180-189
arrLoc=0;
for(memLoc=180;memLoc<190;memLoc++)
{
  
    ret.lessonID[arrLoc]=EEPROM.read(memLoc);
  arrLoc++;
}


//**writing prints[30]  190-219
arrLoc=0;
for(memLoc=190;memLoc<220;memLoc++)
{
  
    ret.prints[arrLoc]=EEPROM.read(memLoc);
 arrLoc++;
}

////////////////////////////////////////////////////////////////////////////////////////////
*/
/*
 //READ VALUES FROM THE EEPROM TO KNOW WHERE THE DEVICE STOPPED WHILE REGISTERING
  //TO AVOID THE STUD,RECT,RET DATA MIXING.

  id=EEPROM.read(233);//takes the last fingerprint template id registered
  lectCounter=EEPROM.read(250);
  
  if(EEPROM.read(220)>0)//TO PREVENT THE LOSS OF THE INITIAL VALUES,(initially if no value in the address will assign zero to variable which is a harm
  lectLoc=EEPROM.read(220);
  
  if(EEPROM.read(221)>0)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  lectLoc1=EEPROM.read(221);

  
  if(EEPROM.read(222)>10)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  lectLoc2=EEPROM.read(222);

  
  if(EEPROM.read(223)>20)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  lectLoc3=EEPROM.read(223);

  
  studCounter=EEPROM.read(251);

  
  if(EEPROM.read(224)>3)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  studLoc=EEPROM.read(224);

  
  if(EEPROM.read(225)>30)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  studLoc1=EEPROM.read(225);

  if(EEPROM.read(226)>60)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  studLoc2=EEPROM.read(226);
  
  if(EEPROM.read(227)>90)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  studLoc3=EEPROM.read(227);
  
  retakerCounter=EEPROM.read(252);
  
  if(EEPROM.read(228)>120)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc=EEPROM.read( 228);
  
  if(EEPROM.read(202)>120)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc1=EEPROM.read(202);
  
  if(EEPROM.read(230)>150)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc2=EEPROM.read(230);
  
  if(EEPROM.read(231)>180)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc3=EEPROM.read(231);
  
  if(EEPROM.read(232)>190)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc4=EEPROM.read(232);
*/
  
  //READING ALL DATA SAVED IN THE EEPROM WRITING THEM TO THE ARRAYS
  
  
  //Wire.begin();
  //Wire.beginTransmission(0x27);
//  lcd.setBacklight(50);//255






  lcd.begin(20,4);
  //lcd.clear();
  Serial.begin(9600);
  delay(10);

 //clears all data on activeSheet of excel if available
  Serial.println("CLEARSHEET");

  //set SD card baud rate
    
  //SD.begin(CS pin);
  pinMode(chipSelect,OUTPUT); //declare the CS as an OUTPUT
  //SD.begin(chipSelect);
  
  if(!SD.begin(chipSelect))// 53 is chipselect on arduino mega
  {
    Serial.println("SD card not working");
  }
  else
  Serial.println("SD card  working");
  
  //CHECKING THE RTC MODULE ON THE BOARD
  if(rtc.begin())
  {
  Serial.println("RTC working");
  // following line sets the RTC to the date & time this sketch was compiled
    rtc1.begin(DateTime(F(__DATE__), F(__TIME__)));
   //or set  January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  /*
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  */
  // set the data rate for the sensor serial port
  finger.begin(57600);
  //-----------------------FINGERPRINT SCANNER PRESENCE TEST------------------------
  if (finger.verifyPassword()) {
    ////Serial.println("Found fingerprint sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("FINGERPRINT "));
    lcd.setCursor(0,1);
    lcd.print(F(" WORKING"));
    Serial.println(" fingerprint working");

    //if working then empty the fingerprints to restart over
   //finger.emptyDatabase();
    delay(1000);
  } else {
    ////Serial.println("Did not find fingerprint sensor :(");
    lcd.print(F("FINGERPRINT"));
    lcd.print(F(" NOT FOUND!"));
    delay(2000);
    while (1) { delay(1); }
  }
  //--------------------------------------------------------------------------------------
//---------------------------RESTORE LECTURER'S FINGERPRINTS ID FROM EEPROM------------------
//restore the lectCounter value
if(EEPROM.read(250)>0)//if there is a value other than zero in storage location of lectCounter
lectCounter=EEPROM.read(250);
Serial.print("lectCounter:");
Serial.println(lectCounter);

//restore the last location of the fingerprints stored
if(EEPROM.read(223)>20)
lectLoc3=EEPROM.read(223);
Serial.print("lectLoc3:");
Serial.println(lectLoc3);
//restore the lecturer's finger ID's to theis respective arrays
int y=20;
for(int z=0;z<lectCounter;z++)
{
  while(y<lectLoc3)
  {
    lect.prints[z]=EEPROM.read(y);
    Serial.print("the lecturer's finger ID:");
    Serial.println(lect.prints[z]);
    break;
  }
  y++;
}
//------------------------------------------------------------------------------------------
 
 //---------------------------------------------------------------------------------
 //----------------------RESTORE STUDENTS REG.No FROM EEPROM-------------------------------
 if(EEPROM.read(233)>0);
 id=EEPROM.read(233);//takes the last fingerprint template id registered
 
 if(EEPROMRead16bits(200)>500)//TO PREVENT THE LOSS OF THE INITIAL VALUES
 studLoc1=EEPROMRead16bits(200);

if(EEPROM.read(251)>0)
  studCounter=EEPROM.read(251);
 // Serial.print("value in studCounter:");
 // Serial.println(studCounter);
 // Serial.print("value in Studloc1:");
 // Serial.println(studLoc1);
  int r=500;
    
   
   for(int z=0;z<studCounter;z++)//index counter for the students registration numbers
      { 
        while(r<=studLoc1)
        {
          stud.regNo[z]=EEPROMReadlong(r);
          Serial.print("studCounter value:");
          Serial.println(studCounter);
          Serial.print("studLoc1");
          Serial.println(studLoc1);
          Serial.print("r values:");
          Serial.println(r);
          Serial.println(stud.regNo[z]);
         
          break;
        }
        r=r+4;
    }
//------------------------------------------------------------------------------------------
//----------------------RESTORE STUDENTS PRINTS FROM EEPROM-------------------------------
 
 if(EEPROM.read(226)>60)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  studLoc2=EEPROM.read(226);
  
 Serial.print("value in studloc2:");
          Serial.println(studLoc2);
 Serial.print("value in studCounter");
 Serial.println(studCounter);

          int r2=60;
 for(int z=0;z<studCounter;z++)//index counter for the students 
      {
       
        while(r2<=studLoc2)
        {
          
          stud.prints[z]=EEPROM.read(r2);
          Serial.print("value in stud.prints[z]:");
          Serial.println(stud.prints[z]);
        
          //Serial.println(r2);
          break;
          
        }
      
        r2++;
       
    }
//----------------------------------------------------------------------------------------

//-----------------------RESTORE RETAKERS' REG.No----------------------------------------
if(EEPROMRead16bits(202)>700)//TO PREVENT THE LOSS OF THE INITIAL VALUES
 retLoc1=EEPROMRead16bits(202);
 Serial.println(EEPROMRead16bits(202));
 Serial.print("retLoc1");
 Serial.println(retLoc1);
if(EEPROM.read(252)>0)
  retakerCounter=EEPROM.read(252);
  r=700;
 Serial.print("retakerCounter value:");
 Serial.println(retakerCounter);   
   
   for(int z=0;z<retakerCounter;z++)//index counter for the retaker's registration numbers
      { 
        while(r<=retLoc1)
        {
          ret.regNo[z]=EEPROMReadlong(r);
          Serial.print("retakerCounter value:");
          Serial.println(retakerCounter);
          Serial.print("retLoc1");
          Serial.println(retLoc1);
          Serial.print("r values:");
          Serial.println(r);
          Serial.println(ret.regNo[z]);
         
          break;
        }
        r=r+4;
    }
//------------------------------------------------------------------------------------------
//-------------------RESTORING THE RETAKER'S PRINTS FROM EEPROM MEMORY-----------------------
if(EEPROM.read(232)>150)//TO PREVENT THE LOSS OF THE INITIAL VALUES
  retLoc4=EEPROM.read(232);
  r2=150;
 for(int z=0;z<retakerCounter;z++)//index counter for the students 
      {
       
        while(r2<=retLoc4)
        {
          
          ret.prints[z]=EEPROM.read(r2);
          Serial.print("retaker's fingerprint IDs:");
          Serial.println(ret.prints[z]);
         
        
          //Serial.println(r2);
          break;
          
        }
      
        r2++;
       
    }
//----------------------------------------------------------------------------------------


  

homeScreen();
}

void loop()
{

  
  //REGISTER OR ATTEND
   key = keypad.getKey();
   
    switch(key)
  {
    case '1'://for registration

          regOptions();//REGISTERING OPTIONS
          
         
            break;
      case '2'://for attendance
            lcd.clear();
            lcd.setCursor(2,0);
            //lcd.print(F("no attend"));
            attendance();
            break;
      case 'D'://format device
          deleteAllPrints();
          
          break;
    default: 
          break;  
  } 
}

void homeScreen()
    {
       lcd.clear();
       lcd.setCursor(0,1);  
       lcd.print(F("1.REGISTER"));
       lcd.setCursor(0,2);
       lcd.print(F("2.ATTEND"));

       //PRINTING THE IMMEDIATE TIME ON THE HOMESCREEN
  DateTime now = rtc1.now();
  lcd.setCursor(15,0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.setCursor(18,1);
  lcd.print(now.day(), DEC);
  lcd.setCursor(18,2);
  lcd.print(now.month(), DEC);
  lcd.setCursor(15,3);
  lcd.print(now.year(), DEC);
  
    }

void regOptions(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("REGISTER AS:"));
  
  lcd.setCursor(0,1);
  lcd.print(F("1.LECTURER"));
  
  lcd.setCursor(0,2);
  lcd.print(F("2.STUDENT"));

  lcd.setCursor(0,3);
  lcd.print(F("3.RETAKER"));

  key ='x';//trick the keypad... to let key be x
          
          while(key=='x'){
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }
           
   switch(key){
       case '1'://lecturer's registration
        lecturerReg();
    
    
    break;
    case '2'://student registration
        studentReg();
        //writingToExcel();
        
      break;
    case '3'://retaker's registration
        retakerReg();
      break;
    /*  
    case 'A'://return to home
    abort();
    break;*/
    case '#'://get back to homeScreen()
       homeScreen();
      break;
    default:
       break;
    
  }
          }
}


void lecturerReg()
{
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("lesson code:"));
    lcd.setCursor(0,1);
    lect.lessonID[lessonCounter]=codeInput(1,1);//input codes not more than 10 digits on the second row
    ////Serial.println("lect.lessonID");
    ////Serial.println(lect.lessonID[lessonCounter]);

    
    //if enter is pressed
    lcd.setCursor(0,2);
    lcd.print(F("lecturer's code:"));
    lcd.setCursor(0,3);
    lect.lectCode[lectCounter]=codeInput(3,2);//input lecturer's code on the fourth line
   
    //if enter is pressed again
    
    //FINGERPRINT ENROLL
    getFingerprintEnroll(1,0,0);//(int lectbit,int studbit,int retakerbit)

    
    
    //delay(500);
     // homeScreen();
    
    }

   void studentReg()
  {
    lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("Reg. No:"));
    lcd.setCursor(0,1);
    stud.regNo[studCounter]=codeInput(1,3);//input codes not more than 10 digits on the second row
    
    //-----------------------writing entered registration number to EEPROM----------------
    EEPROMWritelong(studLoc1,stud.regNo[studCounter]);
    Serial.println("Reg.No written to EEPROM");
    Serial.println(EEPROMReadlong(studLoc1));
    
    //jump other 3 spaces for the next registration number
    studLoc1=studLoc1+4;
    Serial.print("value of incremented studLoc1:");
    Serial.println(studLoc1);
    //---------store the incremented value
     EEPROMWrite16bits(200,studLoc1);//as studLoc1 is a 16bits value
    //EEPROM.write(225,studLoc1);
    Serial.print("studLoc1 EEPROM VALUE:");
    Serial.println(EEPROMRead16bits(200));
    
    ////-----------------------------------------------------------------------------------
    /*
     Serial.println(F("checking the sd card"));
    //--------------------store students reg no in the SD Card file-----------------------
   myfiles1=SD.open("RegFile.txt",FILE_WRITE);
   if(myfiles1)//if file well opened
   {
    Serial.println(F("studRegFile well opened"));
    myfiles1.println(stud.regNo[studCounter]);
   }
   else if(!myfiles1)
   {
   Serial.println("file not opening");
   }
    myfiles1.close();
   //------------------------------------------------------------------------------------- 
    */
    //if enter is pressed
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("FingerPrint:"));
    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,1,0);//(int lectbit,int studbit,int retakerbit)
    

    
    
    //if enter is pressed again go to home screen
     // enterToHomeScreen();
  }

  void enterToHomeScreen()//check if ENTER and output the input characters 
{

              char   key ='x';//trick the keypad... to let key be x
   
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }
            if(key=='*')//if ENTER pressed
            {
              homeScreen();
            } 
}



long codeInput(int row,int who )//will return the double codeInput
{
  
  int i;//for  for loop iteration
  char key;//get input character from the keypad
  long temp=0;
  char lastKey;

  i=0;
   while(i<10)//for(i=0;i<10;i++)
  {
    
    lcd.setCursor(i,row);//i for the cols and j for the rows
BACK:
    char   key ='x';//trick the keypad... to let key be x
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
              
            }
           //if the input character is an alphabet then return
           if((key=='A')||(key=='B')||(key=='C')||(key=='D'))
           goto BACK;
                         
            if(key=='*')//if ENTER IS PRESSED
            {
              
            break;
            }
            //delete key
            if(key=='#')//if DELETE key IS PRESSED
            {
              //if @ lesson code...1
              if(who==1)
              {
                lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(F("lesson code:"));
              lcd.setCursor(0,1);
              }
              //if @lecturer's code....2
              if(who==2)
              {
                lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(F("lesson code:"));
              lcd.setCursor(0,1);

              //PRINT THE LESSON CODE RECENTLY ENTERED BY THE USER
              lcd.print(lect.lessonID[lessonCounter]);
              lcd.setCursor(0,2);
              lcd.print(F("lecturer's code:"));
              lcd.setCursor(0,3);
              }
              //if @ student Reg.No:....3
              if(who==3)
              {
                lcd.clear();
              lcd.setCursor(0,0);
               
               lcd.print(F("Reg. No:"));;
              lcd.setCursor(0,1);
              }
              //if @ retaker lesson code:....4
              if(who==4)
              {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(F("lesson code:"));
                lcd.setCursor(0,1);
              }
              //if @ retaker Reg. No:....5
              if(who==5)
              {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(F("lesson code:"));
                //PRINT THE LESSON CODE RECENTLY ENTERED BY THE USER
                lcd.setCursor(0,1);
                lcd.print(ret.lessonID[retakerLessons]);
                 lcd.setCursor(0,2);
                lcd.print(F("Reg. No:"));
                lcd.setCursor(0,3);
                
              }
              
              
              temp=temp/10;//return  back one column
              ////Serial.println("new temp value:");
              ////Serial.println(temp);
              if(lastKey=='0' || lastKey=='1' || lastKey=='2'|| lastKey=='3'|| lastKey=='4')
              {
                if(temp>=0)//to avoid going to negative numbers
                lcd.print((double)temp,0);
              }

              if(lastKey=='5' || lastKey=='6' || lastKey=='7'|| lastKey=='8'|| lastKey=='9')
              {
                temp=temp-1;//125=12 not 13; 456=45 not 46 AFTER DELETE
                lcd.print((double)temp,0);
              }
              i--;
             continue;
            }
            lastKey=key;//to prevent 1225=123,126=13 while deleting
           lcd.print(key);
            temp=temp*10 +(key-'0');//to prevent the last null value on the strings
            
          i++;
            
         
       
  } 
    ////Serial.println("the value in temp:");
           ////Serial.println(temp);
           return temp;
}


//---------------------------FINGER CHECK -------------------------------------------------------
int attendance()
{
   //------------------------------------SCANNING THE FINGER----------------------------
 int p=1;
 int startAttend;
  lcd.clear();
  //block for the time display during attendance
  DateTime now = rtc1.now();//fetch the data read from the rtc

    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.setCursor(10,0);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    
    
  lcd.setCursor(0,1);
  lcd.print(F("press finger:"));
  lcd.setCursor(0,2);
 
  while(p!=FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  return -1;
  //-------------------------------SEARCHING THE CORRESPONDING FINGER TEMPLATE-----------

  p = finger.fingerFastSearch();
  
  if (p != FINGERPRINT_OK)// if finger not found
  {
    delay(10);
   //lcd.clear();
   lcd.print(F("Finger Not Found"));
   lcd.setCursor(0,3);
   lcd.print(F("TRY AGAIN"));
   delay(1000);
   homeScreen();
  return -1;
  }
 
  for(int i=0;i<=id;i++)//check from all users fingerprint IDs
  {
    //--------------------check if print belongs to the lecturer-------
    //----------------------if not NO CLASS---------------------------
    Serial.println("fingerID detected:");
    Serial.println(finger.fingerID);
    if(lect.prints[i]==finger.fingerID)// && (stud.prints[i]!=finger.fingerID) && (ret.prints[i]!=finger.fingerID))
    {
      //START ATTENDANCE
      lectFinger=finger.fingerID;//variable to hold the ID of who started attendance
     startAttend=1;//lectFinger scans
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      Serial.println(F("ATTENDANCE STARTED"));


    //---------------------------WRITE THE REG.No TO EXCEL--------------------------------
    printToXl();
    //-------------------------------------------------------------------------------------
      
      lcd.setCursor(0,1);

      //WAITING FOR THE STUDENTS AND RETAKERS TO ATTEND OR THE SAME LECTURER TO CLOSE
      
      //infinite loop to allow students/retakers attendance to stop only if the same
      //lecturer pressed the fingerprint again
      
      while(startAttend==1)//if lecturer's finger was once scanned
      {
        //-----------------------------------PRESS THE FINGER----------
        delay(1000);//enough delay to prevents finger multiscanning
        p=1;
 
  BLOCK:
  lcd.clear();

  /////////////////////////////////////////////////////////////////////////////////////
  //DATE AND TIME
  DateTime now = rtc1.now();//fetch the data read from the rtc

    
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
   ////////////////////////////////////////////////////////////////////////////////////////// 
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
  lcd.print(F("press finger:"));
  Serial.println(F("press finger:"));
  //lcd.setCursor(0,1);
  
 
  while(p!=FINGERPRINT_OK)
  {
    p = finger.getImage();
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  return -1;
   //-----------------------CHECK THE MATCHING FINGER TEMPLATE IN THE DATABASE-----
        
  p = finger.fingerFastSearch();
  
  if (p != FINGERPRINT_OK)// if finger not found
  {
   lcd.clear();
   lcd.print(F("Finger Not Found"));
   Serial.println(F("Finger Not Found"));
   
   lcd.setCursor(0,1);
   lcd.print(F("TRY AGAIN"));
   Serial.println(F("TRY AGAIN"));
   delay(1000);
   //homeScreen();
 // return -1;
 goto BLOCK;//if finger is not known then return to the BLOCK to read the next finger
  }
  //----------------------------------------------------------------------------------
        //---------------IF IS OF STUDENT OR A RETAKER THEN HAS ATTENDED SUCCESSFULLY----

        //PRINT THE FINGERID DETECTED WHILE ATTENDING
        Serial.print("fingerID while attending:");
        Serial.println(finger.fingerID);
        Serial.print("student prints:");
        //Serial.println(stud.prints[1]);
        for(int j=0;j<=studCounter;j++)//check from all users fingerprint IDs
  {
        Serial.println(stud.prints[j]);
  }
  
  for(int j=0;j<=id;j++)//check from all users fingerprint IDs
  {
    //if the fingerID found belongs to that of student
    if((stud.prints[j]==finger.fingerID)&&(lect.prints[j]!=finger.fingerID))
    {
      Serial.println("student id:");
      Serial.println(stud.prints[j]);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
      //OUTPUT THE STUDENT REGISTRATION NO FROM THE EEPROM
      lcd.print(stud.regNo[j]);
      //lcd.print(//EEPROMReadlong(500+((j+1)*4)));
      //Serial.println(EEPROMReadlong(500+((j+1)*4)));
        
      lcd.setCursor(0,2);
      Serial.println("student attended");
      lcd.print(F("ATTENDED"));
      //---------------------------------------pass the regNo number to excel-----------------
      if(Serial)//if device is connected to Serial port send data
      attendanceToXl(stud.regNo[j]);
      //---------------------------------------------------------------------------------------

      //delay(10);
      break;//if student's fingerprint found then break
    }
    
    //if the fingerID found belongs to that of retaker
    else if((ret.prints[j]==finger.fingerID)&&(lect.prints[j]!=finger.fingerID))
    {
      Serial.println("retaker id:");
      Serial.println(ret.prints[j]);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(F("ATTENDANCE STARTED"));
      lcd.setCursor(0,1);
      lcd.print(ret.regNo[j]);
     lcd.setCursor(0,2);
      Serial.println("retaker attended");
      lcd.print(F("ATTENDED"));
      //---------------------------------------pass the regNo number to excel-----------------
      attendanceToXl(ret.regNo[j]);
   //--------------------------------------------------------------------------   
    
      //delay(1000);
      break;//if retaker's fingerprint found then break
    }

      //IF IS OF THE SAME LECTURER THEN ATTENDANCE CLOSED....break;
      
    else if(lect.prints[j]==finger.fingerID)//if the fingerprint is of one of the lecturer
      {
        
        Serial.println("lecturer id:");
      Serial.println(lect.prints[j]);
     
      lcd.clear();  
      lcd.setCursor(0,1);
      if(lect.prints[j]==lectFinger)//if the fingerprint is of the lecturer who started the attendance
      {
        lcd.print(F("ATTENDANCE CLOSED"));
       //Serial.println(F("ATTENDANCE CLOSED"));
       startAttend=0;
      delay(1000);
      break;
      }
      else// if is the different lecturer
      lcd.print("DENIED!");
      delay(500);
      break;
      }
      
       
      }
      
      }
        if(startAttend==0)
       break;
      }

   
    }


  //IF FINGER NOT BELONG TO A LECTURER
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("NO CLASS"));
    Serial.println("the finger id detected:");
    Serial.println(finger.fingerID);

    
    delay(1000);
    homeScreen();
 
}





//---------------------------FINGER ENROL-------------------------------------------------------
int getFingerprintEnroll(int lectbit,int studbit,int retakerbit)
{

  
  int address;//to return the address of the id(either having the lecturerer's address or student address)
  lcd.clear();
  lcd.setCursor(4,0);
  //------------------ANALYSE THE FINGERPRINT FIRST-----------------------------------
  int p = -1;
  lcd.print(F("PLACE FINGER:"));
  delay(1000);

  

  //the loop will stop iteration if FINGERPRINT_OK
  while (p != FINGERPRINT_OK)
  {
    //SCANNING THE FINGER
    p = finger.getImage();
    //delay(5000);
    switch (p)
    {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      //lcd.clear();
      ////printId(userCount);
      lcd.setCursor(0,1);
      lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No Finger");
      //lcd.clear();
     // //printId(userCount);
       lcd.setCursor(0,1);
       lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("No Finger"));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
      //lcd.clear();
     // //printId(userCount);
       lcd.setCursor(0,1);
       lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Comm Error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Imaging error");
      //lcd.clear();
     // //printId(userCount);
       lcd.setCursor(0,1);
       lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Imaging Error"));
      break;
    default:
      //Serial.println("Unknown error");
      // lcd.clear();
      ////printId(userCount);
       lcd.setCursor(0,1);
       lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Unknown Error"));
      //break;
    }
  }
  //IF FINGER SCANNING IS SUCCESSFULLY DONE THEN
  
  //CONVERTING THE FINGERPRINT IMAGE
  
  p = finger.image2Tz(1);//put 1st template in slot location 1 for verification
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image converted");
      //lcd.clear();
      //printId(userCount);
      
       lcd.setCursor(0,1);
       lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Image too messy");
       //lcd.clear();
       //printId(userCount);
        
      return p;//p is -1
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Communication error");
            //lcd.clear();
            //printId(userCount);
             lcd.setCursor(0,1);
             lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Comm Error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
           // lcd.clear();
           //printId(userCount);
             lcd.setCursor(0,1);
      lcd.print(F("Feature Not Found"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
                 // lcd.clear();
                 //printId(userCount);
                  lcd.setCursor(0,1);
                  lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Feature Not Found"));
      return p;
    default:
      //Serial.println("Unknown error");
                  //lcd.clear();
                  //printId(userCount);
                  lcd.setCursor(0,1);
                  lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Unknown Error"));
      return p;
  }
  //DO A double CHECK
  
  //Serial.println("Remove finger");
  //lcd.clear();
  //printId(userCount);
   lcd.setCursor(0,1);
   lcd.print("               ");
      lcd.setCursor(0,1);
  lcd.print(F("Remove Finger"));
  delay(500);
  p = 0;
  
  //if no finger detected then it will wait till the finger is pressed again
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
//Serial.println("ID "); 
  //////Serial.println(id);
  p = -1;
  //---------------------PLACE THE SAME FINGER--------------------------------------
  //Serial.println("Place same finger again");
   //lcd.clear();
   //printId(userCount);
    lcd.setCursor(0,1);
    lcd.print("               ");
      lcd.setCursor(0,1);
      lcd.print(F("Place again"));
      //lcd.setCursor(0,1);
      //lcd.print(F("   Again"));
   
   //wait for getting finger to scan
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      
    
      break;
    case FINGERPRINT_IMAGEFAIL:
      ////Serial.println("Imaging error");
      
   
      break;
    default:
      ////Serial.println("Unknown error");
      
   
      return;
    }
  }
  // OK success!
  //convert the scanned finger-> (2)
  p = finger.image2Tz(2);//place template in the location 2 for verification
  switch (p) {
    case FINGERPRINT_OK:
      ////Serial.println("Image converted");
     
      break;
    case FINGERPRINT_IMAGEMESS:
      ////Serial.println("Image too messy");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ////Serial.println("Communication error");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ////Serial.println("Could not find fingerprint features");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    enterToHomeScreen();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ////Serial.println("Could not find fingerprint features");
      lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      return p;
    default: 
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
      ////Serial.println("Unknown error");
      return p;
  }
  // OK converted!
  
  //take a two print feature template and create a model
  ////Serial.println("Creating model for #");  
  ////Serial.println(id);
  
  p = finger.createModel();//model created
  if (p == FINGERPRINT_OK) { 
    //////Serial.println("Prints matched!");
    lcd.print(F("matched"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    ////Serial.println("mismatch");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  } else {
    ////Serial.println("Unknown error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    delay(500);
    homeScreen();
    return p;
  }
  ////Serial.println("ID "); 
  ////Serial.println(id);

  //store the model of the id
  p = finger.storeModel(id);//store calculated model for later matching
  if (p == FINGERPRINT_OK) {
    //-------------------------FINGER IS STORED-----------------------------------------
    ////Serial.println("REGISTERED");
   // lcd.clear();
   //printId(userCount);
    lcd.setCursor(0,1);
    lcd.print(F("REGISTERED!"));
    

    
    
    
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ////Serial.println("Communication error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION) {
    ////Serial.println("Could not store in that location");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  } 
  else if (p == FINGERPRINT_FLASHERR) {
    ////Serial.println("Error writing to flash");
    
    return p;
  }
  else {
    ////Serial.println("Unknown error");
    lcd.print(F("               "));
    lcd.print(F("TRY AGAIN"));
    
    return p;
  }
 //--------------------WHOSE FINGERPRINT IS THIS? LECTURER OR STUDENT---------------------
  //if enrolling a lecturer...
  if(lectbit==1 && studbit==0 && retakerbit==0)
  {

    lect.prints[lectCounter]=id;//save the finger//printId of the lecturer
 
 //------------------------------STORING THE LECTURER'S FINGERPRINT ID TO EEPROM-----
     EEPROM.write(lectLoc3,lect.prints[lectCounter]);
     Serial.print("Registered lecturer's fingerprint iD:");
     Serial.println(EEPROM.read(lectLoc3));
     lectLoc3++;
     //-----------STORE THE VALUE OF THE INCREMENTED lectLoc3 in EEPROM--------------
     EEPROM.write(223,lectLoc3);
 //----------------------------------------------------------------------------------
 
   lectCounter++;
    //UPDATE lectCounter VARIABLE IN THE EEPROM ADDRESS
    EEPROM.update(250,lectCounter);
   
    userCount=lectCounter;
    
    
    id++;//increase the number of overall registered users(lecturer,student,retaker)
    EEPROM.update(233,id);//save to EEPROM
   
  }

//---------------------------IF ENROLLING A STUDENT---------------------------------------------------
  if(lectbit==0 && studbit==1 && retakerbit==0)
  {
    stud.studID[studCounter]=studCounter;
    stud.prints[studCounter]=id;//save the finger//printId of a student

//---------------------STORING THE FINGERPRINT ID IN THE EEPROM------------------------
    Serial.println("fingerprint value kept in  stud.prints[j] ");
        Serial.println(stud.prints[studCounter]);
        EEPROM.write(studLoc2,stud.prints[studCounter]);
        
        Serial.println("value in EEPROM:");
        Serial.println(EEPROM.read(studLoc2));
        studLoc2++;
        Serial.println("---------->value in studeLoc2");
        Serial.println(studLoc2);
        EEPROM.write(226,studLoc2);//store the incremented value in EEPROM
        
//--------------------------------------------------------------------------------------



  
     studCounter++;

    //UPDATE studCounter VARIABLE IN THE EEPROM ADDRESS
    EEPROM.update(251,studCounter);
    userCount=studCounter;
    
    id++;// increase the number of overall registered users(lecturer,student,retaker)
    EEPROM.update(233,id);//save to EEPROM
    
  }

  //if enrolling a retaker...
  if(lectbit==0 && studbit==0 && retakerbit==1)
  {
    ret.studID[retakerCounter]=retakerCounter;
    ret.prints[retakerCounter]=id;//save the finger//printId of a student
//-------------------STORING RETAKER'S FINGERPRINT ID IN THE EEPROM------------------------ 
  EEPROM.write(retLoc4,ret.prints[retakerCounter]);
   
   //store the value of retLoc4 to EEPROM memory
   EEPROM.write(232,retLoc4);
   Serial.print("value kept in retLoc4:");
   Serial.println(EEPROM.read(232));
   retLoc4++;
retakerCounter++;
//UPDATE retakerCounter VARIABLE IN THE EEPROM ADDRESS
    EEPROM.update(252,retakerCounter);
//-----------------------------------------------------------------------------------------

  
    userCount=retakerCounter;
    
    id++;// increase the number of overall registered users(lecturer,student,retaker)
     EEPROM.update(233,id);//save to EEPROM
  }

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("No:"));
/*
  if(userCount == 29)//if reached the max users
  {
    lcd.clear();
    lcd.print(F("users full"));

    //return an error
    return -1;
  }
  */
  //id=userCount;//each id corresponds to the user

  //userCount++;//increment for the next to register have next ID
  
  lcd.print(userCount);//print the user's ID
  lcd.setCursor(0,1);

  delay(2000);
    homeScreen();
  
}

//---------------------------------------------------------------------------------------

//---------------------NUMBER OF ALL USERS---UNUSED-----------------------------------
void printId(int id)//output the user's number
{
        lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("No:"));
  //writing the value kept in the eeprom
  lcd.print(id);
}

void retakerReg()
{
  lcd.clear();
    lcd.setCursor(0,0);
    
    lcd.print(F("lesson code:"));
    //lcd.setCursor(0,1);
    ret.lessonID[retakerLessons]=codeInput(1,4);//input codes not more than 10 digits on the second row
 
  
    //if enter is pressed
     lcd.setCursor(0,2);
    lcd.print(F("Reg. No:"));
   // lcd.setCursor(0,3);
    ret.regNo[retakerCounter]=codeInput(3,5);//input codes not more than 10 digits on the second row

 //-----------------STORE ENTERED REGISTRATION NUMBER TO EEPROM---------------------------
  EEPROMWritelong(retLoc1,ret.regNo[retakerCounter]);
    Serial.println("retaker's Reg.No written to EEPROM");
    Serial.println(EEPROMReadlong(retLoc1));
    
    //jump other 3 spaces for the next registration number
    retLoc1=retLoc1+4;
    Serial.print("value of incremented retLoc1:");
    Serial.println(retLoc1);
    //---------store the incremented value
     EEPROMWrite16bits(202,retLoc1);//as retLoc1 is a 16bits value
    
    Serial.print("retLoc1 EEPROM VALUE:");
    Serial.println(EEPROMRead16bits(202));
    
    ////-----------------------------------------------------------------------------------
    
 
    //FINGERPRINT ENROLL
    getFingerprintEnroll(0,0,1);//(int lectbit,int studbit,int retakerbit)
    
}
/*
void //readingStudNo()
{
  //--------------------store students  in the SD Card file-----------------------
   myfiles2=SD.open("NOFile.txt");
   Serial.println(F("studIdFile data"));
   while(myfiles2.available())//if file are available
   {
    
    Serial.write(myfiles2.read());
   }
    myfiles2.close();
   //------------------------------------------------------------------------------------- 
  
}
/*
//-------------------------------- WRITING FILE TO EXCEL-------------------------
void writingToExcel()
{ 
  //CLEARING EXCEL DATA LEFT FROM THE PREV SHEET AND START @ THE 1st LINE
  //Serial.println("CLEARDATA");
  Serial.println("CLEARSHEET");
  //CREATE FIRST ROW
  Serial.println("LABEL,NO,Reg.No");
  
  
  //-------------------------------OPENING A FILE TO READ FROM------------------
  myfiles2=SD.open("NOFile.txt");
  myfiles1=SD.open("RegFile.txt");
  
   Serial.println(F("studIdFile data"));
   while((myfiles1.available()) && (myfiles2.available()))//if file are available
   {
    //ENABLING  WRITING DATA TO FIRST COLUMN
    Serial.print("DATA,");
    Serial.write(myfiles2.read());
    Serial.print(",");
    Serial.write(myfiles1.read()); 
    
    Serial.println(); //put something for the file to be read into excel
    delay(100);
   }
    myfiles1.close();
     myfiles2.close();


    //------------------------- WRITING REGISTRAION NUMBERS---------------------
}*/

void deleteAllPrints()
{
   //delete all fingerprint templates registered in the sensor
   finger.emptyDatabase();  
   
   
   //delete the EEPROM data
   lcd.clear();
          lcd.setCursor(5,0);
          lcd.print("FORMATING");
   for(int z=0;z<1024;z++)//EEPROM IS OF 1K MEMORY
   {
    EEPROM.write(z,0);//clear all the EEPROM locations by writing zero to all positions
   }
   
          for(int lcdrows=1;lcdrows<4;lcdrows++)//rows
          {
            
            for(int lcdcols=0;lcdcols<20;lcdcols++)//columns
            {
            
             
              lcd.setCursor(lcdcols,lcdrows);
              lcd.print(".");
              delay(100);
             
            }
            
          }
  /*
   //MUST KEEP IN EEPROM
   userCount=0;
    lectLoc=0;//lecturer's eeprom position counter
    lectLoc1=0;
    lectLoc2=10;
   lectLoc3=20;

  studLoc=30;//students'eeprom position counter
  studLoc1=30;
  studLoc2=60;
   studLoc3=90;

   retLoc=120;//retakers'eeprom position counter
   retLoc1=120;
   retLoc2=150;
   retLoc3=180;
   retLoc4=190;
   //THEN GO HOME 
  */
 lcd.clear();
 lcd.setCursor(0,1);
 lcd.print("*******RESET*******");

}

//in between interrupt to homeScreen
void toHome()
{

char   key ='x';//trick the keypad... to let key be x
          while(key=='x')
            
            key=keypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              key=keypad.getKey();//try to find a typed non NULL value
            }

            if(key=='A')//if return to home btn is pressed 
            {
              
            return -1;
            }
}

//---------------------------EEPROM Read Write Long functions------------------------------
//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
      }
 //This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(int address)
      {
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
      }

void EEPROMWrite16bits(int address, int value)
      {
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> two = Least significant byte
      byte two = (value & 0xFF);
      byte one = ((value >> 8) & 0xFF);
      

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, two);
      EEPROM.write(address + 1, one);
      
      }

uint16_t EEPROMRead16bits(int address)
      {
      //Read the 4 bytes from the eeprom memory.
      uint16_t two = EEPROM.read(address);
      uint16_t one = EEPROM.read(address + 1);
      

      //Return the recomposed long by using bitshift.
      return ((two << 0) & 0xFF) + ((one << 8)& 0xFFFF) ;
      }
  
/*
//--------------------------------------------EXCEL--------------------------
  void valtoXl(long xlVal)//xlVal will pass the long values as regNo
  {
  //  Serial.println("CLEARSHEET");
  //CREATE FIRST ROW
//Serial.println("LABEL,NO,Reg.No");


 //---------------------------WRITING TO EXCEL--------------------------------------
  //--------------------------PRINTS 20 SESSIONS--------------------------------------
for(s=1;s<=20;s++)//20 number of sessions
{
  sprintf(str4,"CELL,SET,%c1,",incChar);//date are on the second line
  sprintf(str5,"SESSION%d",s);
  Serial.print(str4);//command to write in a col of row1
  Serial.println(str5);//print the session number to excel
  delay(500);
  //s++;//encrement the number of sessions
  incChar++;//increment to the next col
}
//-----------------------------------------------------------------------------------------




 

  //-----------------CHECK THE PRESSED FINGER IF IS ITS FIRST PRESS-------------------------
  for(int Brows=inc;Brows>=1;Brows--)
   {
   
    sprintf(str3,"CELL,GET,B%d",Brows);//create a string for the data direction
    Serial.println(str3);//Serial.println("CELL,GET,B%d",Brows);
    
    long valFromXl=Serial.readStringUntil(10).toInt();//reading string till its end and convert it
    if(xlVal==valFromXl)
    goto END;
    
   }
   //--------------------------------------------------------------------------------------
  
  ////////////PRINT OUT NO/////////////////////////////////////////////////////
                    Serial.println("CELL,SET,A1,No");
                    sprintf(str2,"CELL,SET,A%d,",inc);//create a string for the data direction
  Serial.print(str2);
  Serial.println(no);//student number reached
  
  no++ ;//number of students will increment for each student or retaker
  
  //////////PRINT OUT REG.No//////////////////////////////////////////////////////////
  sprintf(str,"CELL,SET,B%d,",inc);//create a string for the data direction
  Serial.print(str);
  inc++;//goes to the next row
  Serial.println(xlVal);//display REG No
  END:
  return 0;
  }*/


//------------------------PRINT OUT ALL REGISTERED STUDENTS TO EXCEL----------------------
void printToXl()
{
  inc=3;//counts from the second row of col A and B 
   char strA[30];//for holding the changes on the code for stud.regNo
  char strB[30];//for ret.regNo
  char strC[30];//for no
  int no=1;//counts the numbers from 1 respectively
  //sessionsNum represents number of cols
  static int incChar='C';
  //variables for DATE at each session
  char str3[30];
 
   
   char str4[30];
  char str5[30];
  static int sessionsNum=1;//count number of attendance done to open another session

  //Serial.println("LABEL,REG.No");
  //  Serial.println("CLEARSHEET"); // clears starting at row 1
  
  delay(10);
  Serial.println("CELL,SET,B1,REG.No");
  
  
for(int a=0;a<studCounter;a++)
  {
////////////PRINT OUT NO////////////////////////////////////////////////////////
   Serial.println("CELL,SET,A1,No");
  sprintf(strC,"CELL,SET,A%d,",inc);//create a string for the data direction
  Serial.print(strC);
  Serial.println(no);//student number reached
////////////PRINT THE REG.No ON THE NEXT COL////////////////////////////////////////////////
     sprintf(strA,"CELL,SET,B%d,",inc);//create a string for the data direction
  Serial.print(strA);
  Serial.println(stud.regNo[a]);//DATA to be displayed
   delay(10);
   inc++;
   no++;
  }

  for(int b=0;b<retakerCounter;b++)
  {
    ////////////PRINT OUT NO////////////////////////////////////////////////////////
   Serial.println("CELL,SET,A1,No");
  sprintf(strC,"CELL,SET,A%d,",inc);//create a string for the data direction
  Serial.print(strC);
  Serial.println(no);//student number reached
////////////PRINT THE REG.No ON THE NEXT COL////////////////////////////////////////////////
     sprintf(strB,"CELL,SET,B%d,",inc);//create a string for the data direction
  Serial.print(strB);
   
   
  //Serial.print("DATA");//DATA has to match the LABEL
  
 
  Serial.println(ret.regNo[b]);//DATA to be displayed
   delay(10);
   inc++;
   no++;
  }
//-------------------WRITING SESSIONS AT THE TOP-----------------------------------

  sprintf(str4,"CELL,SET,%c1,",incChar);//date are on the second line
  sprintf(str5,"SESSION%d",session);
  Serial.print(str4);//command to write in a col of row1
  Serial.println(str5);//print the session number to excel
  delay(10);
  incChar++;//increment to the next col
  session++;
///////////////////////////////////////////////////////////////////////////////////////////

 //----------------------PRINT THE DATE THE SESSION IS DUE--------------------------------
 sprintf(str3,"CELL,SET,%c2,",incChar2);//date are on the second line
  Serial.print(str3);
  Serial.println("DATE");
  delay(10);
  incChar2++;
  
}
//-----------------------------passing the Reg.No to excel for attendance records-------------
void attendanceToXl(long valToXl)
{
  char str7[30];
  char str6[30];

  delay (10);
    //inc 
  for(int Brows=inc;Brows>=1;Brows--)//inc helps navigate all the rows of B column
   {
   
    sprintf(str7,"CELL,GET,B%d",Brows);//create a string for the data direction
    Serial.println(str7);//goes to the B column of row Brows to read the value contained
    
    long valFromXl=Serial.readStringUntil(10).toInt();//reading string till its end(10 meaning in ASCII) and convert it
    
    //if the value pointed by B,Brows correspons to that of the pressed finger then √ attended
    if(valToXl==valFromXl)
    {
      //check today's session column and the row of valFromXl
      if(valToXl==valFromXl)
    {
      //check today's session column and the row of valFromXl
      
      sprintf(str6,"CELL,SET,%c%d,",incChar2-1,Brows);//date are on the second line
  Serial.print(str6);
  Serial.write("A");
  Serial.println();
  delay(10);
  break;
      
      
    }
      
    }
    
  
}
 
}
