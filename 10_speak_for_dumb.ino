// include the library code:
#include <LiquidCrystal.h>


#include "SD.h"
#define SD_ChipSelectPin 8
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int mylanguage=1;
int language=0;
int m,n,g=1;
int k=1;
int i=1;
int btnPlus=A0; 
int btnMinus=A2; 
int btnEnter=A1;

void setup() {
   Serial.begin(9600);
   tmrpcm.speakerPin = 9;
   if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
lcd.print("NO MEMORY CARD!!");


//while(1)
//{
//  delay(20);
//}

}
  // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
  // Print a message to the LCD.
   lcd.print("   SPEAKING");
   lcd.setCursor(0,1);
   lcd.print("   FOR DUMB");
   delay(3000);

//selecting language
while(language<1)
{
  m=1;
    lcd.clear();
  lcd.print ("> ENGLISH");
 
  lcd.setCursor(0,1);
  lcd.print ("> KINYARWANDA   "); 

  
   if(digitalRead(btnEnter)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnEnter)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnEnter)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnEnter)==HIGH);  //Confirm button release
 if(g==2)
 {
  g=0;
 }
//button's task here
g=g+1;

//  Serial.println(i);
      }
   } 

if(g==1){
//  n=1;
  lcd.setCursor(0,0);
  lcd.blink();
}

if(g==2){
 // n=2;
  lcd.setCursor(0,1);
  lcd.blink();
  //g=0;
}

//accepting language
if(digitalRead(btnPlus)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
  
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnPlus)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnPlus)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnPlus)==HIGH);  //Confirm button release
 
 language=2;
//button's task here
if(g==1)
{
  mylanguage=0;
}
if(g==2)
{
  mylanguage=1;
}


      }
  }



   delay(300);
}

   
   lcd.clear();
   lcd.print ("MEDIA CARD FOUND");
    delay(1500);
   lcd.clear();
   lcd.print ("PRESS SELECTION");
   lcd.setCursor(0,1);
   lcd.print ("      KEY"); 
   tmrpcm.setVolume(6);
}


void loop() {
  
while(mylanguage==1)
{
    if(digitalRead(btnMinus)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnMinus)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnMinus)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnMinus)==HIGH);  //Confirm button release


k=1;


if(i==1)
{
  m=1;
    lcd.clear();
  lcd.print ("> Ndashaka amazi");
 
  lcd.setCursor(0,1);
  lcd.print ("> Ndumva ndwaye "); 
}
if(i==2)
{
  m=2;
    lcd.clear();
  lcd.print ("> Ndaribwa munda");
 
  lcd.setCursor(0,1);
  lcd.print ("> Nshaka toilete"); 
}
if(i==3)
{
  m=3;
    lcd.clear();
  lcd.print ("> Nshaka kuryama");
 
  lcd.setCursor(0,1);
  lcd.print ("> Ndumva naniwe "); 
}
if(i==4)
{
  m=4;
   lcd.clear();
  lcd.print ("> Mfite isereri ");
 
  lcd.setCursor(0,1);
  lcd.print ("> Ndwaye Umutwe ");  
}
if(i==5)
{
  m=5;
    lcd.clear();
  lcd.print ("> Nshaka kurya");
 
  lcd.setCursor(0,1);
  lcd.print ("> mpa ibinini   "); 
  
}

if(i==6)
{
 // m=5;
    lcd.clear();
  lcd.print (" PRESS TO GO TO");
 
  lcd.setCursor(0,1);
  lcd.print ("  FIRST PAGE   "); 
  i=0;
}

  //lcd.print ("ml");
//button's task here
i=i+1;
  Serial.println(i);
  
      }
   }









     if(digitalRead(btnEnter)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnEnter)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnEnter)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnEnter)==HIGH);  //Confirm button release
 
//button's task here

if(k==1&&i>1){
  n=1;
  lcd.setCursor(0,0);
  lcd.blink();
}

if(k==2&&i>1){
  n=2;
  lcd.setCursor(0,1);
  lcd.blink();
  k=0;
}

k=k+1;
  Serial.println(i);
      }
   } 



if(digitalRead(btnPlus)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
  
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnPlus)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnPlus)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnPlus)==HIGH);  //Confirm button release
 
//button's task here
//i=i-1;
//k=k-1;
if(m==1&&n==1)
{
      lcd.clear();
  lcd.print (" Ndashaka amazi");
 
  lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("1.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}
if(m==1&&n==2)
{
  lcd.clear();
    lcd.print (" Ndumva ndwaye ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("2.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}

if(m==2&&n==1)
{
  lcd.clear();
   lcd.print (" Ndaribwa munda");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("3.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

  
}
if(m==2&&n==2)
{
  lcd.clear();
    lcd.print (" Nshaka toilete"); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("4.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}

if(m==3&&n==1)
{
  lcd.clear();
    lcd.print (" Nshaka kuryama");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("5.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
 
}
if(m==3&&n==2)
{
  lcd.clear();
    lcd.print (" Ndumva naniwe "); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
  tmrpcm.play("6.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}

if(m==4&&n==1)
{
  lcd.clear();
    lcd.print (" Mfite isereri ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("7.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}
if(m==4&&n==2)
{
 lcd.clear();
   lcd.print (" Ndwaye Umutwe ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("8.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500); 

}

if(m==5&&n==1)
{
  lcd.clear();
    lcd.print (" Nshaka kurya");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("9.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}
if(m==5&&n==2)
{
 lcd.clear();
   lcd.print (" mpa ibinini   "); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("10.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
 
}


i=1;
k=1;
//lcd.clear();
   lcd.clear();
     lcd.print ("PRESS SELECTION");

  lcd.setCursor(0,1);

  lcd.print ("      KEY"); 
m=0;
n=0;

      }
  }


  
  delay(300);
 
}



//------------------------------------------------------------------------------------------------
//================================================================================================
//---------------ENGLISH VERVION-----------------------------------------------------------------
//===============================================================================================

while(mylanguage==0)
{
      if(digitalRead(btnMinus)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnMinus)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnMinus)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnMinus)==HIGH);  //Confirm button release


k=1;


if(i==1)
{
  m=1;
    lcd.clear();
  lcd.print ("> I want water");
 
  lcd.setCursor(0,1);
  lcd.print ("> Am sick       "); 
}
if(i==2)
{
  m=2;
    lcd.clear();
  lcd.print ("> 've stomachache");
 
  lcd.setCursor(0,1);
  lcd.print ("> I want toilete"); 
}
if(i==3)
{
  m=3;
    lcd.clear();
  lcd.print ("> I want to bed");
 
  lcd.setCursor(0,1);
  lcd.print ("> Am tired      "); 
}
if(i==4)
{
  m=4;
   lcd.clear();
  lcd.print ("> I have fever   ");
 
  lcd.setCursor(0,1);
  lcd.print ("> i 've headache");  
}
if(i==5)
{
  m=5;
    lcd.clear();
  lcd.print ("> Am hungry  ");
 
  lcd.setCursor(0,1);
  lcd.print ("> I want doctor "); 
  
}

if(i==6)
{
 // m=5;
    lcd.clear();
  lcd.print (" PRESS TO GO TO");
 
  lcd.setCursor(0,1);
  lcd.print ("  FIRST PAGE   "); 
  i=0;
}

  //lcd.print ("ml");
//button's task here
i=i+1;
  Serial.println(i);
  
      }
   }









     if(digitalRead(btnEnter)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnEnter)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnEnter)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnEnter)==HIGH);  //Confirm button release
 
//button's task here

if(k==1&&i>1){
  n=1;
  lcd.setCursor(0,0);
  lcd.blink();
}

if(k==2&&i>1){
  n=2;
  lcd.setCursor(0,1);
  lcd.blink();
  k=0;
}

k=k+1;
  Serial.println(i);
      }
   } 



if(digitalRead(btnPlus)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
  
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnPlus)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnPlus)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnPlus)==HIGH);  //Confirm button release
 
//button's task here
//i=i-1;
//k=k-1;
if(m==1&&n==1)
{
      lcd.clear();
  lcd.print (" I want water");
 
  lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("01.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}
if(m==1&&n==2)
{
  lcd.clear();
    lcd.print (" Am sick ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("02.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}

if(m==2&&n==1)
{
  lcd.clear();
   lcd.print (" 've stomachache");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("03.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

  
}
if(m==2&&n==2)
{
  lcd.clear();
    lcd.print (" I want toillete"); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
   tmrpcm.play("04.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
  
}

if(m==3&&n==1)
{
  lcd.clear();
    lcd.print (" I want to bed");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("05.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
 
}
if(m==3&&n==2)
{
  lcd.clear();
    lcd.print (" I feel tired "); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
  tmrpcm.play("06.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}

if(m==4&&n==1)
{
  lcd.clear();
    lcd.print (" I have fever ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("07.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}
if(m==4&&n==2)
{
 lcd.clear();
   lcd.print (" I 've headache ");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("08.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500); 

}

if(m==5&&n==1)
{
  lcd.clear();
    lcd.print (" Am hungry");
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("09.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);

}
if(m==5&&n==2)
{
 lcd.clear();
   lcd.print (" I want doctor"); 
    lcd.setCursor(0,1);
  lcd.print ("PLAYING");
     tmrpcm.play("010.wav");
   lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (".");  
   delay(500);
      lcd.print (">");  
   delay(500);
 
}


i=1;
k=1;
//lcd.clear();
   lcd.clear();
     lcd.print ("PRESS SELECTION");

  lcd.setCursor(0,1);

  lcd.print ("      KEY"); 
m=0;
n=0;

      }
  }


  delay(300);
}


}

