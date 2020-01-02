


//______________________SOLAR_POWERED_AUTO_IRRIGATION_SYSTEM________________________________



#include <LiquidCrystal.h>         // LCD LIBRARY
LiquidCrystal lcd(2,3,4,5,6,7);   //rs,e,d4,d5,d6,d7  LCD PINS DECLARATION
int sensor_pin = A0 ;            // SENSOR PINDECLARATION
int ph_pin = A1 ;
int output_value ;              // VARIABLE DECLARATION
int output_value2 ; 
int valve_pin = 8 ;            // RELAY PIN DECLARATION
int pump_controller = 9;


#define echoPin 10 // Echo Pin
#define trigPin 11 // Trigger Pin


int maximumRange =200; // Maximum range needed in cm
int minimumRange =30; // Minimum range needed in cm
long duration, distance; // Duration used to calculate distance


//_____________________________________________________________________________


void setup ( ) {
 Serial.begin(9600);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);

   // THE HOME SCREEN OR THE FIRST SCREEN
  lcd.begin (16,2);
  lcd.setCursor(0,0);
  lcd.print (" SOLAR POWERED");
  lcd.setCursor(0,1);
  lcd.print ("AUTO IRRIGATION");
  delay(500);


  // THE SECOND SCREEN
  lcd.clear();
  lcd.print("READING SENSOR");
  delay(500);
  lcd.setCursor(0,1);
  lcd.print("wait........");
  delay(1000);


 // notification();
   pinMode (valve_pin, OUTPUT); // MAKING THE RELAY PIN OUTPUT
   pinMode(pump_controller,OUTPUT);


 } 


//___________________________________________________________________________



void loop ( ) {

//THE ULTRASONIC_SENSOR___DETECTING THE DISTANCE

 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 distance = duration/58.2;
Serial.println(distance);

// THE LOGIC FOR FINDING WHETHER THE TANK IS ALMOST FULL

if (distance <= maximumRange&&distance>minimumRange){
 
 digitalWrite(pump_controller, HIGH); 

 }
 else {
 
 digitalWrite(pump_controller,LOW ); 
 
 }
 

  
// READINGN THE ANALOG VALUSES FROM THE SENSORS

  output_value = analogRead (sensor_pin);                   //READINT THE MOISTURE SENSOR
  output_value = map (output_value , 1023 , 0 , 0 , 100);  // MAPING THE SENSED VALUE INTO 0-100

 output_value2 = analogRead (ph_pin);
 output_value2 = map (output_value , 1023 , 0 , 14 , 0);  // MAPING THE SENSED VALUE INTO 0-14






  lcd.clear ( ); // CLEAR THE LCD
  lcd.setCursor (0, 0);
  lcd.print ("moisture");
  lcd.print (output_value);
  lcd.setCursor (12,0);
  lcd.print (" PH");
  lcd.print (output_value2);
  lcd.setCursor (0, 1);
  




  if (output_value < 40)

  {
  lcd.print (" Valve: ON ");
  valve_ON_controller(); // FUNCTION CALL
   }  



  else

   { 
    lcd.print (" Valve: OFF ");
    valve_OFF_controller(); // FUNCTION CALL 
   } 

  delay(1000);// THIS DELAY IS NECESSARY JUST BEFORE NEXT READ OF THE SENSORS

  }






  void valve_ON_controller()
  {
 digitalWrite (valve_pin , HIGH);    //Relay operates on opposite signal

  delay (500); 
  }





    void valve_OFF_controller()
  {
 digitalWrite (valve_pin , LOW);    //Relay operates on opposite signal

  delay (500); 
  }




