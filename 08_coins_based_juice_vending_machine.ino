
//______________________________JUICE VENDING MACHINE____________________________________
//_______________________________FINAL YEAR PROJECT______________________________________
//______________________BY REMY MANANIZEYE____AND___FRANK RUTAPFA________________________
//_________________ACCADENIC__YEAR_2018_JUNE_____GISHARI_POLYTECHNIQUE___________________

//___________________________               _____________________________________________

/*
 __________________________________KEY_NOTES_____________________________________
 This system accepts only coin of a 100frw
 the system will give the juice which corresponds to the amount of money inserted
 after iserting the coin you press the button in order to make juice start flowing

 for the tecnical issues:
          -the red wire of coin aceptor is connected to 12v
          -the black wire of coin aceptor is connected to the ground
          -the white wire of coin aceptor is connected to digital pin 2
          -the pushbutton is connected to pin 4
          -the LCD is connnected to pin 5 up to 10
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(5,6,7,8,9,10);   //rs,e,d4,d5,d6,d7  LCD PINS DECLARATION

//PINS AND CONSTANTSDECLARATION
const byte coinPin = 2;
volatile int coinsValue = 0;
int ledpin=11;
int buzzer=12;            
int btnpin=4;              


void setup()
{  
   Serial.begin(9600);
   pinMode(coinPin, INPUT); //SET THE INPUT PIN_coin
   pinMode(ledpin,OUTPUT);//SET THE OUTPUT PIN_juice-pump
   pinMode(btnpin,INPUT); //SET THE INPUT PIN_button
   pinMode(buzzer,OUTPUT);//SET THE OUTPUT PIN_buzzer
 
  
  //  THE FIRST SCREEN
  lcd.begin (16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print (" JUICE VENDING");
  lcd.setCursor(0,1);
  lcd.print ("   MACHINE");
  delay(2000);
  lcd.clear();
}

void loop()
{         
  
  //THE HOME SCREEN OF THE MACHINE
          lcd.clear();
          lcd.print("  WELCOME");
          lcd.setCursor(0,1);
          lcd.print ("INSERT THE COIN");

          
    static unsigned long prevCoinMillis = 0;
    static float coinValue = 0;     
    bool coinInserted = false;
    unsigned long currentMillis = millis();

    //LET CHECK THE PULSE DURATION AND READ THE COIN PIN, THEN COUNT THE COINS
    if((currentMillis - prevCoinMillis >= 50) && (digitalRead(coinPin) == HIGH))
    {
        prevCoinMillis = currentMillis;
        coinInserted = true;
        coinValue += 0.05;
    }



    if(coinInserted == true)
    {
      coinsValue = coinsValue + 100; 
          Serial.print("AYOWINJIJE: ");
          Serial.print(coinsValue);
          Serial.println("frw");

          //PRINTING IN THE SCREEN
          lcd.clear();
          lcd.print("AMOUNT: ");
          lcd.print(coinsValue);
          lcd.println("frw");
          lcd.setCursor(0,1);
          lcd.print ("PRESS-or-ADDCOIN");
    }



    // CLIENT HAS TO PRESS A BUTTON FOR THE JUICE TO START COMING
     if(digitalRead(btnpin)==LOW)                //DETECTION OF BUTTON INTERFACE TO LOW
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnpin)==LOW)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnpin)==LOW);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnpin)==LOW);  //Confirm button release

     lcd.clear();
     lcd.print("TAKE YOUR JUICE");
     delay(500);
     digitalWrite(buzzer,HIGH);
     delay(500);
     digitalWrite(buzzer,LOW);
     delay(500);
     digitalWrite(buzzer,HIGH);
     delay(500);
     digitalWrite(buzzer,LOW);
     
      //CALCURATING THE TIME THAT THE JUICE PUMP WILL BE ON DEPENDING ON NUMBER OF COINS
    
     for(int i=coinsValue;i>=0; i--)
     {
          //PRINTING IN THE SCREEN
          lcd.clear();
          lcd.print("REMAINS :");
          lcd.print(i);
          lcd.println("frw");
          lcd.setCursor(0,1);
          lcd.print ("_THROWING JUICE_");
        digitalWrite(ledpin,HIGH);         //ACTIVATING THE JUICE PUMP
        delay(30);                        //THE TIME OF GIVING JUICE
      }
     
     digitalWrite(ledpin,LOW);         //STOPPING THE JUICE PUMP
     coinsValue=0;                    //STARTING THE COINS COUNTER
      lcd.clear();
      lcd.print("__THANK YOU!!!__");// THANKS OF THE MACHINE
      delay(1000);
      }
   }


}


