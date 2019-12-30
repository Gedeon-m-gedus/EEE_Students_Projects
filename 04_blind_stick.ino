//__________VARIABLES DECLARATION____________
#define trigPin1 5
#define echoPin1 4
#define trigPin2 2
#define echoPin2 3
int buzzer = 9;
int vibrator = 6;
long duration1, distance1, UltraSensor1;
long duration2, distance2, UltraSensor2;
float Long=30.4239396,Lat=-1.9068961;
String online_link;

#include <TinyGPS++.h>
#include <SoftwareSerial.h>


TinyGPSPlus gps;

// Configure software serial port
SoftwareSerial SIM900(7, 8); //rx tx

void setup() {

pinMode(trigPin1, OUTPUT);
pinMode(echoPin1, INPUT);
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(buzzer, OUTPUT);
pinMode(vibrator, OUTPUT);

  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  Serial.begin(19200);
  SIM900.begin(19200);
  // Give time to your GSM shield log on to network
  delay(200);   
 }

void loop() { 
receiveMessage();
SonarSensor(trigPin1, echoPin1);
SonarSensor(trigPin2, echoPin2);
draw_conclusion();
get_gps_data();
creating_link_to_map();
}

//______my defined functions__________
void get_gps_data(){
  while (Serial.available() > 0){ 
    gps.encode(Serial.read());
    if (gps.location.isUpdated()){
      Lat=gps.location.lat();
      Long=gps.location.lng();
    }
 }
}

void receiveMessage()
{
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  if(SIM900.find("urihe")){
    sendSMS();
    }

  }
void sendSMS() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
Serial.println("runing");
  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"0788200036\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println(online_link); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000); 
 
}

void creating_link_to_map(){
  //____creating google map link__________
  online_link="";
  online_link += F("http://maps.google.com/?q=");
  online_link += String(Lat, 6);
  online_link += F(",");
  online_link += String(Long, 6);
}

void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin1, LOW);
delayMicroseconds(2);
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distance1 = (duration1/2) / 29.1;
delay(100);

digitalWrite(trigPin2, LOW);
delayMicroseconds(2);
digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distance2 = (duration2/2) / 29.1;
delay(100);
}

void draw_conclusion(){
  if (distance1>25)
{
  Serial.println("sensor 1 detected");
  digitalWrite(buzzer, HIGH);
  }
 else
 {
  Serial.println("No object sensor 1");
  digitalWrite(buzzer, LOW);
 }
 if (distance2<60)
{  Serial.println("sensor 2 detected");
  digitalWrite(vibrator, HIGH);
  }
 else
 {
  Serial.println("No object sensor 2");
  digitalWrite(vibrator, LOW);
 }
}

