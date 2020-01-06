

//ACCELEROMETER FOR SENDING DATA TO CLOUD BY GEDEON


//_____________________ LIBRARY_____________________

#include <ESP8266WiFi.h>




 //_________API KEY ANT NETWORK CONNECTION____________
//String apiKey = "S4AG5L61CY5OFI90";           //  Enter your Write API key from ThingSpeak
//const char* ssid =  "GEDEON_wifi";     // replace with your wifi ssid and wpa2 key
const char* password =  "Thegiants2019$";
const char *ssid =  "Thegiants_Electronics";     // replace with your wifi ssid and wpa2 key
//const char *pass =  "Thegiants2019$";

const char* host = "192.168.8.112";


//__________________PINS & VARAIBLES DECRARATION________________________

#define LED A0
#define btnEnter 4
WiFiClient client;
int sender=0,contact=0;
int ds=200;
float Long,Lat;
float d1,d2,d3;

String online_link;
//________ offices location________________
float office_1_lat=-1.958869;
float office_1_long=30.062613;

float office_2_lat=-1.958123;
float office_2_long=30.062758;

float office_3_lat=-1.958990;
float office_3_long=30.062129;

void nearest_office(){
d1= (office_1_lat - Lat)*(office_1_lat - Lat) + (office_1_long - Long)*(office_1_long - Long);
d2= (office_2_lat - Lat)*(office_2_lat - Lat) + (office_2_long - Long)*(office_2_long - Long);
d3= (office_3_lat - Lat)*(office_3_lat - Lat) + (office_3_long - Long)*(office_3_long - Long);
d1=(sqrt(d1))*1000;
d2=(sqrt(d2))*1000;
d3=(sqrt(d3))*1000;
}

void check_the_nearest_office(){
  if(d1<d2 && d1<d3){
    contact=contact+1;
    Serial.println("Office one is closer");
  }
  else if(d2<d1 && d2<d3){
    contact=contact+2;
    Serial.println("Office two is closer");
  }
  else{
    contact=contact+3;
    Serial.println("Office three is closer");
  }
}

//_______-_______CODES FROM ACC_____________
#include <Wire.h>

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = 12;
const uint8_t sda = 14;

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
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 5, TXPin = 0; //RX=3, TX=4
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);



 //__________________THE SETUP FUNCTION_______________________
void setup() 
{
       Serial.begin(115200);                   //starting the serial communication
       delay(10);
       pinMode(LED, OUTPUT);    // LED pin as output.
       ss.begin(GPSBaud);//SOFTWARE SERIAL FOR GPS



       //_________acc
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(sda, scl);
  MPU6050_Init();
 Serial.println("Connecting to ");
       Serial.println(ssid);
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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


  // This sketch displays information every time a new sentence is correctly encoded.
 while (ss.available() > 0){
    
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6);
      Lat=gps.location.lat();
      Long=gps.location.lng();
    }
  
 }

 //____creating google map link__________
  online_link=" ";
  online_link += F("http://maps.google.com/?q=");
  online_link += String(gps.location.lat(), 6);
  online_link += F(",");
  online_link += String(gps.location.lng(), 6);
  Serial.println(online_link);



/*
  
  Serial.print("Ax: "); Serial.print(Ax);
  Serial.print(" Ay: "); Serial.print(Ay);
  Serial.print(" Az: "); Serial.print(Az);
  Serial.print(" T: "); Serial.print(T);
  Serial.print(" Gx: "); Serial.print(Gx);
  Serial.print(" Gy: "); Serial.print(Gy);
  Serial.print(" Gz: "); Serial.println(Gz);
*/

conversion();

led();

Serial.print("     Ax: "); Serial.print(tX);
  Serial.print("    Ay: "); Serial.print(tY);
  Serial.print("    Az: "); Serial.println(tZ);
  Serial.print("    T: "); Serial.print(T);
  delay(100);
//_________________________

if(tZ<30)
{
sender=sender+1;  
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
    }
  }



       
  if(sender>0)
  {
                  /*===============================================================================*/


  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request where there are the php codes that sends them in database
  String url = "/admin/send.php?";
  url += "Long=";
  url += Long;
  url += "&Lat=";
  url += Lat;
  url += "&ds=";
  url += ds;
  url += "&T=";
  url += online_link;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
    digitalWrite(LED, HIGH); 
  delay(2000);
  sender=0;
    digitalWrite(LED, LOW);   
  }

}



void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
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

void led(){
  if (tX<30){
digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
                       // wait for a second
 }
 else{
  digitalWrite(LED_BUILTIN, LOW);}
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
