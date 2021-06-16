#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Keypad.h>
#include <EEPROM.h>

const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Variables
int mot_min = 90;   //min servo angle  (set yours)
int mot_max = 180; //Max servo angle   (set yours)
static int character = 0;
static int activated =0;
static char Received = 'c';
static char customKey ;
char Str[16] = {' ', ' ', ' ', ' ', ' ', ' ', '-', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' '};  

  
 static  int pres_state = 0;
  static int prev_state = 0;
  static int button_moves_counter=0;
  static int button_presses_counter = 0 ;
  static int state = 0;
  static int button_pressed =0;
  static int button_high_pulse_triggered;


  static int lock_state =0;
  static int counter = 0;
//Pins
Servo myservo1;
Servo myservo2;
Servo myservo3;
int button1 = 13; 


//Keypad config
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte  rowPins[ROWS] = {6,7,8}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void setup() {

  
  
  lcd.begin(16, 2);
 Serial.begin(9600);
  myservo1.attach(10); //password controlled door
 myservo2.attach(9);//button controlled door
// myservo3.attach(11); //buttom for bluetooth
  
  pinMode(button1,INPUT);
  //Init the screen and print the first text
  if(EEPROMRead16bits(0) == 0)
  set_default_password();
  //put the servo in the close position first
  myservo1.write(0);  
  myservo2.write(0);  
//  myservo3.write(0);

}

void loop() {
  ///////////////Android OPEN/CLOSE/////////  
  if(Serial.available()>0)
 { 
    
    char Received = Serial.read();

    if (Received == 'c')
    {
      myservo3.write(mot_min);
      activated = 0;
      character=0; 
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);  
    }
    if (Received == 'o')
    {
      myservo3.write(mot_max);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  ANDROID OPEN");
      activated = 2;
     
      
      lcd.clear();    
      lcd.setCursor(4,0);
      lcd.print("WELLCOME");
      

      lcd.clear();    
      lcd.setCursor(3,0);
      lcd.print("DOOR  OPEN");
      delay(500);
    }
    
    
    
 }
 //////////////////////////////////////////////////////////////////////////////////////
  lcd.setCursor(0,0);
  lcd.print("1.LOCK/OPEN");
  lcd.setCursor(0,1);
  lcd.print("2.RESET PASSWORD");
  home_screen_options();

   int temp_counter = read_the_button();
    if(counter != temp_counter){
    if(temp_counter % 2 != 0){
      //open the door
  turn_the_motor_0(2);  
    }
   else if(temp_counter % 2 ==0){
  //close the door
  turn_the_motor_90(2); 
}
    }
    counter = temp_counter;
}

void btn_door_controlled(int counter){
   
  
Serial.println(counter % 2);

}
void lock_or_open(){
  long password;
  lcd_init();
  lcd.print("    PASSWORD");
  lcd.blink();
  lcd.setCursor(0,1);
  lcd.print("        ");

  Serial.print("lock_state:");
  Serial.println(lock_state);
  
  password = enter_password(1);
  Serial.print("value in password:");
  Serial.println(password);
if(password == get_password()){
  if (lock_state == 0){
   turn_the_motor_90(1); 
  }
  
  else if (lock_state ==1){
    turn_the_motor_0(1);
  }
  password = 0;
  }
else if(password != get_password()){
  lcd_init(); 
lcd.print("     WRONG");
lcd.setCursor(0,1);
lcd.print("   PASSWORD");
delay(500);
lcd_init();
}
}
void home_screen_options(){
   char key = customKeypad.getKey();
  
  if (key){
    Serial.println(key);
    switch(key){
      case '1': 
        lock_or_open();  
        break;
       case '2':
       edit_password();
        break;
       default:
        break;
    }
    
  }
}

void close_door(int servo_number)
{
  if(servo_number == 1)
  myservo1.write(mot_min);

  if(servo_number == 2)
  myservo2.write(mot_min);

  if(servo_number == 3)
  myservo3.write(mot_min);
  
      activated = 0;
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';   
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);  
}
void open_door(int servo_number){
  if(servo_number == 1)
  myservo1.write(mot_max);

  if(servo_number == 2)
  myservo2.write(mot_max);

  if(servo_number == 3)
  myservo3.write(mot_max);
  
      activated = 0;
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';   
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);  
}

void input_password(){
      customKey = customKeypad.getKey(); //this function reads the presed key
  
  if (customKey){
//BY DEFAULT character holds 0
if (character ==0)
    {  
    Serial.println(customKey);

    Str[6]= customKey;   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    PASSWORD");    
    lcd.setCursor(0,1);
    lcd.print(Str);
   
    }
  //KEEP FIRST CHARACTER IN THE str[7]

    if (character ==1)
    {  
    Serial.println(customKey);
    Str[7]= customKey;   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    PASSWORD");    
    lcd.setCursor(0,1);
    lcd.print(Str);
   
    }
  //KEEP SECOND CHARACTER IN THE str[8]

    if (character ==2)
    {  
    Serial.println(customKey);
    Str[8]= customKey;   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    PASSWORD");    
    lcd.setCursor(0,1);
    lcd.print(Str);
   
    }
  //KEEP THIRD CHARACTER IN THE str[9]

    if (character ==3)
    {  
    Serial.println(customKey);
    Str[9]= customKey;   
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    PASSWORD");    
    lcd.setCursor(0,1);
    lcd.print(Str);
   
    }
//IF FORTH CHARACTER READ THEN MAKE SET ACTIVATE FLAG TO 1...to check the pswd entered
    if (character ==4)
    {  
    Serial.println(customKey);
    Str[10]= customKey;
    activated=1;
   
    }
    character=character+1;
    delay(100);
  } 

  
}

void check_password_entered(){
  //CHECKING THE PASSWORD ENTERED
  //PRESS A to open the door
  //PRESS B to close the door
    if (activated == 1)
    {
/*Change your password below!!! 
Change each of Str[6], Str[7], Str[8], Str[9]*/
  //PRESS A to open the door

    if(Str[10]='A' && character==5 && Str[6]=='1' && Str[7]=='4' && Str[8]=='7' && Str[9]=='7' )
    {
      myservo1.write(mot_max);
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("ACCEPTED");
      activated = 2;
      
      lcd.clear();    
      lcd.setCursor(4,0);
      lcd.print("WELLCOME");
      delay(500);
      lcd.setCursor(2,1);
      lcd.print("ELECTRONOOBS");
      delay(1000);

      lcd.clear();    
      lcd.setCursor(3,0);
      lcd.print("DOOR  OPEN");
      lcd.setCursor(2,1);
      lcd.print("ELECTRONOOBS");
      
    }
    else
    {
      lcd.clear();    
      lcd.setCursor(1,0);
      lcd.print("PASSWORD ERROR");
      lcd.setCursor(3,1);
      lcd.print("TRY  AGAIN");
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';
      activated = 0;
      lcd.clear();    
      lcd.setCursor(4,0);
      lcd.print("PASSWORD");
      lcd.setCursor(0,1);
      lcd.print(Str);   
    }
  }
   
   //PRESS B to close the door

  if (activated == 2)
    {
    if(customKey == 'B' )
    {
      myservo1.write(mot_min);
      activated = 0;
      character=0;
      Str[6]= '-';
      Str[7]= '*'; 
      Str[8]= '*'; 
      Str[9]= '*';
      Str[10]= ' ';   
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    PASSWORD");    
      lcd.setCursor(0,1);
      lcd.print(Str);
     
    }
  }  
}

void reading_android_app(){

  Serial.print("serial.available():");
    Serial.println(Serial.available());
 Serial.println(Received);
  if (Received == 'c')
  close_door(1);
  if (Received == 'o')
  open_door(1);
}
//============================================================================================
int read_the_button()
{

    
    pres_state = digitalRead(button1);
    delay(10);
    
    if(pres_state == prev_state){
      button_pressed = 0;
    }
    if(pres_state != prev_state){
      button_pressed = 1;
    }
    if ((button_pressed == 1) && (prev_state == 0)){
      button_moves_counter++;
       
       button_high_pulse_triggered = button_moves_counter%2;
    if(button_high_pulse_triggered == 0){
      button_presses_counter++;
    }
      state = 1;
    }
    if ((button_pressed == 1) && (prev_state == 1)){
      button_moves_counter++;
      state = 0;

       button_high_pulse_triggered = button_moves_counter%2;
    if(button_high_pulse_triggered == 0){
      button_presses_counter++;
    }
    
    }
    if (state == 1){
    Serial.println(button_presses_counter);
      
    }
    if (state == 0){
   }

    prev_state = pres_state;
    return button_presses_counter;
    
}
//===========================================================================================
//============================================================================================
void set_default_password(){
  long default_password = 1111;
  EEPROMWrite16bits(0,default_password);
}

void set_password(long new_password){
  EEPROMWrite16bits(0,new_password);
}

long get_password(){
long  password = EEPROMRead16bits(0);
 return password; 
}

long enter_password(int row){
  int i;//for  for loop iteration
  char key;//get input character from the keypad
  long temp=0;
  char lastKey;

  i=0;

  //allow password with  less than 10 digits
   while(i<10)//for(i=0;i<10;i++)
  {
    
    lcd.setCursor(i,row);//i for the cols and j for the rows
BACK:
    char   key ='x';//trick the keypad... to let key be x
          while(key=='x')
            
                 key = customKeypad.getKey();//let it be null
            
            while(key==NULL)//while key will be still NULL
            {
              
                 key = customKeypad.getKey();//try to find a typed non NULL value
              
            }
       
           //if the input character is an alphabet then return
           if((key=='B')||(key=='C')||(key=='D'))
           goto BACK;
                         
            if(key=='*')//if ENTER IS PRESSED
            {
              
            break;
            }
            //delete key
            if(key=='#')//if DELETE key IS PRESSED
            {
              lcd.clear();
              lcd.setCursor(0,0);
               
               lcd.print("    PASSWORD");
              lcd.setCursor(0,1);
              
              
              temp=temp/10;//return  back one column
              
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
           lcd.print(lastKey);
            temp=temp*10 +(key-'0');//to prevent the last null value on the strings
            
          i++;
            
         
       
  } 
    lcd.noBlink();//remove cursor blinking
           return temp;
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
 void lcd_init(){
  lcd.clear();
  lcd.setCursor(0,0); 
 }

 void edit_password(){
  long old_password;
  long new_password;
  
  lcd_init();
    
    lcd.print("OLDER PASSWORD:");
    lcd.setCursor(0,1);
    lcd.blink();
    old_password = enter_password(1);
    
    if( old_password == EEPROMRead16bits(0)){
      lcd_init();
    
    lcd.print("NEW PASSWORD:");
    lcd.setCursor(0,1);
    lcd.blink();
    new_password = enter_password(1);
    EEPROMWrite16bits(0,new_password);
    lcd_init();
    lcd.print("   SAVED");
    lcd.setCursor(0,1);
    lcd.print("SUCCESSFULLY!");
    delay(1000);
    }
    else{
      lcd_init();
      lcd.print("WRONG PASSWORD");
      delay(1000);
    }
 }

 //==========================================================================================
//============================MOTOR CONTROL================================================
void turn_the_motor_90(int servo_number){
  for(int motor_direction=0;motor_direction<=90;motor_direction++){
    if(servo_number == 1)
     myservo1.write(motor_direction); 
    else if (servo_number == 2)
    myservo2.write(motor_direction);
//     Serial.println(motor_direction);
     delay(20);
     }
     lock_state = 1;
     return lock_state;
}
void turn_the_motor_0(int servo_number){
  for(int motor_direction=90;motor_direction>=0;motor_direction--){
    if(servo_number == 1)
     myservo1.write(motor_direction); 
    else if (servo_number == 2)
     myservo2.write(motor_direction); 
//     Serial.println(motor_direction);
     delay(20);
     }
     lock_state = 0;
     return lock_state;
}


//==========================================================================================
