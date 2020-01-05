//MAN I TRIED MY BEST TO MAKE YOUR PROGRAM COMPILE
//AND I HOPE IT WILL WORK FINE, BUT BEFORE U REMOVE THOSE SERIAL FIRST RUNS THE WHOLE PROJECT
// NEXT YOU HAVE TO REDUCE ALL THE DELAY ALMOS TO ZERO SO THAT IT RESPONDS ON REAL TIME
//BUT DO IT AFTER CHEKING IT RESPONSE WIT ALL THE DELAYS
// BEST WISHES TO YOU ALL GUYS   CLAUDE_GEDEON-...............

#include <EEPROM.h>
#include <Stepper.h>


int x = 233;
int y = 433;
int z = 433;
Stepper myStepper1(x, 12,9,10,11);
Stepper myStepper2(y, 6,8,7,5); 
Stepper myStepper3(z, 13,4,3,2);
  int detector1Pin = A1;
  int detector1Value = 0;
  int detector2Pin = A2;
  int detector2Value = 0;
  int detector2Threshold = 0;
  int lightValue = 0;
  int sensorPin1 = A0;
   int sensorPin2 = A4;
  
  // EEPROM ADRESS
  int address0 = 0;
  int address1 = 0;
  int address2 = 0;


  
  int counterpills = 0;
  int valknob1 = 0;
  int valknob2 = 0;   int ledStrip = A0;                // select the input pin for the LED strip
  int laser = A3;                   // select the input pin for the laser   int sensorPin1 = A6;              // select the input pin for the potentiometer 1
//  int sensorPin2 = A7;              // select the input pin for the potentiometer 2
  int sensorValue1 = 0;             // variable to store the value coming from the sensor 1
  int sensorValue2 = 0;             // variable to store the value coming from the sensor 2
  
  int i = 0;
  int level = 0;
   void setup() {
  
  // initialize the serial BOUD RATE
  Serial.begin(9600);

  myStepper1.setSpeed(60);          //Set rotation speeds of each stepper.
  myStepper2.setSpeed(60);
  myStepper3.setSpeed(60);

  pinMode(ledStrip, OUTPUT);        //Declare led strip and laser as output
  pinMode(laser, OUTPUT);
  detector1Value = analogRead (detector1Pin);       //Read the starting photoresistor value in order to detect when the cover is opened.
}




void loop()
{  
  
  // Check if the upper cover is opened 
  lightValue = analogRead (detector1Pin);
  detector2Value = analogRead (detector2Pin);
  
  Serial.print ("Cover ");
  Serial.println (lightValue);
  
  if (lightValue >= detector1Value + 100){        //If the cover is opened start the scheduling function and restart the pills counter.
    int counter = 0;
    
    for (int i = 1; i <= 2; i++){                 //Choosing the level that is going to be filled, we have 2 levels so the loop is going to be done twice
      level = i;
      int address0 = (level*10);                  //Adress 0 stores the nuber of pills, every level has it's own (10 for level 1,20 for level 2, 30 for level 3...)
      
      counterpills = EEPROM.read(address0);       //Read the adress for every level to see how many pills are already stored
      
      Serial.print(address0);
      Serial.print("\t");
      Serial.print("pills number ");
      Serial.print(counterpills);
      Serial.print("\t");       if (counterpills == 0){                     //If the level checked is empty, we select it as the level we will fill and break the "for loop".
        Serial.print("level choosen ");
        Serial.println(level);
        break;        
      } else {                                    //If there are pills stored we discard it as we can't mix pills.
        Serial.print("Floor ");
        Serial.print(level);
        Serial.println(" is full");
        level = 0;
      }
    }
    if (level != 0) {                             //When one of the levels is empty we proceed to fill it.
      if (level == 1){                            //If the level choosen is the first one we rotate the selector to guide pills to it. 
        myStepper1.step(-2*x);
        delay (500);
        myStepper2.step (y);                      // Every level has 15 compartments so we rotate 24 degreees each time a pill is inserted.
        delay (500);
      }
      if (level == 2){                            //We do the same for level 2
        delay (3000);
        myStepper1.step(-x);
        delay (3000);
        myStepper3.step(z);
        delay (500);
      }
  
      digitalWrite(laser, HIGH);                            //We light the laser and detect the photoresistor value
      delay(2000);
      detector2Threshold = analogRead(detector2Pin);
        
      while (counter <= 15){                                //We repeat the loop 15 times, one for pill.
        detector2Value = analogRead(detector2Pin);          //We read the photoresistor value and light the led strip up to say the user he can introduce the pills
        digitalWrite(ledStrip, HIGH);                       
        
        Serial.print ("        PHTR value ");               
        Serial.print (detector2Value);
        Serial.print ("      counter       ");
        Serial.println (counter);
        
        if (detector2Value <= detector2Threshold - 30){     //If the value is lower than the starting one means a pill has interrupted the laser 
          digitalWrite(ledStrip, LOW);                      //We turn off the led strip and add one pill to the counter
          counter++;
          delay (1500);
          Serial.print ("Rotating level ... please wait");
  
          if (level == 1){
            myStepper2.step (y);
          }
          if (level == 2){            
            myStepper3.step (z);
          }
          delay (1500);
        }
        
        lightValue = analogRead (detector1Pin);              //We check the photorresistor on the cover again to see if the cover was closed.
  
        Serial.print ("lightvalue ");
        Serial.print (lightValue);
        Serial.print ("      threshold ");
        Serial.print (detector1Value + 100);
      
        if (lightValue <= detector1Value + 130){             //If the cover was closed we wait to double check if it was and avoid errors
          delay (5000);
          lightValue = analogRead (detector1Pin);
          if (lightValue <= detector1Value + 130){  
            digitalWrite(ledStrip, LOW);                    //If we confirm it is closed we turn off the led and exit the loop.
            break;
          }
        }
      }
  
      Serial.println ("You have finished inserting pills");
      delay (3000);
      Serial.println ("Rotating selector back to it's starting position... please wait");
  
      if (level == 1){                                      //We move back the selector to it's initial position.
        myStepper1.step(2*x);
        delay (1500);
      }
  
      if (level == 2){
        myStepper1.step(x);
        delay (1500);        
      }
  
      sensorValue1 = analogRead(sensorPin1);                  // read the value from the dial 1 and give a value depending on it's position.
      if (0 <= sensorValue1 && sensorValue1 <= 252){
       valknob1 = 6;
      } if (253 <= sensorValue1 && sensorValue1 <= 505){
       valknob1 = 8;
      } if (506 <= sensorValue1 && sensorValue1 <= 757){
       valknob1 = 12;
      } if (758 <= sensorValue1 && sensorValue1 <= 1024){
       valknob1 = 24;
      }
             
      sensorValue2 = analogRead(sensorPin2);                   //We do the same for dial 2
      if (0 <= sensorValue2 && sensorValue2 <= 112){
       valknob2 = 8;
      } if (113 <= sensorValue2 && sensorValue2 <= 224){
       valknob2 = 10;
      } if (225 <= sensorValue2 && sensorValue2 <= 336){
       valknob2 = 12;
      } if (337 <= sensorValue2 && sensorValue2 <= 448){
       valknob2 = 14;
      } if (449 <= sensorValue2 && sensorValue2 <= 561){
       valknob2 = 16;
      } if (562 <= sensorValue2 && sensorValue2 <= 673){
       valknob2 = 18;
      } if (674 <= sensorValue2 && sensorValue2 <= 785){
       valknob2 = 20;
      } if (786 <= sensorValue2 && sensorValue2 <= 897){
       valknob2 = 22;
      } if (898 <= sensorValue2 && sensorValue2 <= 1023){
       valknob2 = 24;
      } 
  
      counterpills = counter;
  
      address0 = (level*10);                                    //We declare wich adress in the EEPROM will be used to store the data.
      address1 = (level*10)+1;
      address2 = (level*10)+2;
     
      Serial.println (valknob1);
      Serial.println (valknob2);
      Serial.println (counterpills);
  
      EEPROM.write(address0, counterpills);                     //And we will write the EEPROM with tha data received from both dials and the pills counter.
      EEPROM.write(address1, valknob1);
      EEPROM.write(address2, valknob2); 
      
      delay (10000);
  
      int k = EEPROM.read(address0);                            //We read the values to check in the serial monitor that they have been recorded right.
      int l = EEPROM.read(address1);
      int m = EEPROM.read(address2);
  
      Serial.println (k);
      Serial.println (l);
      Serial.println (m);   
  
      delay (1000);
    }
  }
  delay (500);
}



