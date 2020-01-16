//__________________PINS & VARAIBLES DECRARATION________________________


#define btnEnter 4 //pin to which the button is connected gpio4 it will trigger in case of simple emergence
int sender=0,contact=0; // it is like flag, to now the state of emergency

float Long=30.064366,Lat=-1.958082,d1,d2,d3; // this are variables to store cordinates and distance
String online_link, hospital,acc_type;
String Car_ID="RAD 103L X";

//________ HOSPITALS location________________
float hospital_1_lat=-1.9559;
float hospital_1_long=30.0605;

float hospital_2_lat=-1.9439;
float hospital_2_long=30.0952;

float hospital_3_lat=-1.9884;
float hospital_3_long=30.2171;

void nearest_hospital(){
d1= (hospital_1_lat - Lat)*(hospital_1_lat - Lat) + (hospital_1_long - Long)*(hospital_1_long - Long);
d2= (hospital_2_lat - Lat)*(hospital_2_lat - Lat) + (hospital_2_long - Long)*(hospital_2_long - Long);
d3= (hospital_3_lat - Lat)*(hospital_3_lat - Lat) + (hospital_3_long - Long)*(hospital_3_long - Long);
d1=(sqrt(d1))*1000;
d2=(sqrt(d2))*1000;
d3=(sqrt(d3))*1000;
}

//wifi library
#include <ESP8266WiFi.h>
//_______-_______CODES FROM MPU6050_____________
#include <Wire.h>//allows us to use I2C COMMUNICATION PROTOCAL

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = 12;//clock line
const uint8_t sda = 14;// data line

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

 double Ax, Ay, Az, T, Gx, Gy, Gz ,tX,tY,tZ;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

//___________GPS CODES___________________
#include <TinyGPS++.h> //gps library
#include <SoftwareSerial.h>// to enable us to use gpio pins as transmiter and receiver

static const int RXPin = 5, TXPin = 0; //RX=5, TX=0
static const uint32_t GPSBaud = 9600;//gps and node mcu communication speed

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);



String apiKey = "37XSWFWCY4NTRS2W";     //  Enter your Write API key from ThingSpeak
const char* ssid =  "project";     // Enter your WiFi Network's SSID
const char* pass =  "12345678"; // Enter your WiFi Network's Password



const char* server = "api.thingspeak.com";
WiFiClient client;


 //__________________THE SETUP FUNCTION_______________________
void setup() 
{
Serial.begin(115200);                   //starting the serial communication
    delay(10);
//CONNECTING TO THE WIFI
WiFi.begin(ssid, pass);
delay(100);
 
      Serial.println("");
      Serial.println("***GSM network connected***");

     
       ss.begin(GPSBaud);//SOFTWARE SERIAL FOR GPS
       delay(100);
 Serial.println("    SYSTEM WORKING PROPERY");      
       
//_________acc

Wire.begin(sda, scl);
delay(100);
MPU6050_Init();
    
}
 
void loop() 
{
//________accerlometer________
Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;


  // This sketch displays information every time a new sentence is correctly encoded

  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){ 
      Lat = gps.location.lat();
      Long = gps.location.lng();
      yield();
    }
  }

//____creating google map link__________
  online_link=" ";
  online_link += F("http://maps.google.com/?q=");
  online_link += String(gps.location.lat(), 6);
  online_link += F(",");
  online_link += String(gps.location.lng(), 6);

 
conversion();
nearest_hospital();

if(tZ<30)
{
sender=sender+1;  
acc_type="Car Dive";
}

 if(digitalRead(btnEnter)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(btnEnter)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(btnEnter)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(btnEnter)==HIGH);  //Confirm button release
sender=sender+1;
acc_type="Captain Request";
    }
  }
       
  if(sender==1){  
Serial.print("     Ax: "); Serial.print(tX);
Serial.print("    Ay: "); Serial.print(tY);
Serial.print("    Az: "); Serial.println(tZ);
Serial.print("Latitude= "); 
Serial.print(Lat, 6);
Serial.print("    Longitude= "); 
Serial.println(Long, 6);
Serial.println(online_link);
Serial.print("    hospital 1= "); 
Serial.print(d1, 6);
Serial.print("Km"); 
Serial.print("    hospital 2= "); 
Serial.print(d2, 6);
Serial.print("Km"); 
Serial.print("    hospital 3= "); 
Serial.print(d3, 6);
Serial.println("Km"); 
check_the_nearest_hospital();
sendSMS();
//push_data_to_cloud();
delay(100);
sender=0;//clear flag

    }
}


void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);//addressing the register we want to access
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}


// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}

void conversion()
{
  if(Ax<0){
  Ax=Ax*(-1);
  }
 if(Ay<0){
  Ay=Ay*(-1);
  }
 if(Az<0){
  Az=Az*(-1);
  }

tX=Ax*100;
tY=Ay*100;
tZ=Az*100;
}

void sendSMS() {
  while(Serial.available()&&sender<4)
  {
  // AT command to set Serial to SMS mode
  Serial.println("AT+CMGF=1"); 
  delay(100);
Serial.println("running...");
  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  if(contact==1){
  Serial.println("AT+CMGS=\"0787284715\""); 
  delay(5000);
  }
    if(contact==2){
  Serial.println("AT+CMGS=\"0787284715\""); 
  delay(5000);
  }
    if(contact==3){
  Serial.println("AT+CMGS=\"0787284715\""); 
  delay(5000);
  }
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  
  Serial.println("There is car accident, please help!"); 
  Serial.println("Here is the accident location: ");
  Serial.println(online_link);
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  Serial.println((char)26); 
  delay(100);
 sender=sender+1;
  //break;
  }
  Serial.println();
  // Give module time to send SMS
  delay(10000); 
  Serial.println("done...message sent");
  contact=0;
}

void check_the_nearest_hospital(){
  if(d1<d2 && d1<d3){
    contact=contact+1;
    Serial.println("hospital one is closer");
    hospital="CHUK";
    Serial.println(hospital);
  }
  else if(d2<d1 && d2<d3){
    contact=contact+2;
    Serial.println("hospital two is closer");
    hospital="KING FAISAL";
    Serial.println(hospital);
  }
  else{
    contact=contact+3;
    Serial.println("hospital three is closer");
    hospital="MASAKA hospital";
    Serial.println(hospital);
  }
}
