#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#include <Wire.h>                    // Include Wire library (required for I2C devices)
#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial mySerial(13, 15);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);



const char* ssid =  "gedeon";     // replace with your wifi ssid and wpa2 key
const char* password =  "gedeon1234";
const char* host = "192.168.43.94";

WiFiClient client;

uint8_t id;

int addr = 0;//eeprom memory address

const int led = 16; //GPIO5
int b=12,bb=14;
int stud_id;
int course_id=213;
int g=1,gg=2,count=0,m=0;

void setup()  
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  //while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nATENDANCE DEVICE");

Wire.begin(2, 0); // d3 and d4 Initialize I2C LCD module (SDA = GPIO0, SCL = GPIO2)
 lcd.begin(16, 2);
lcd.backlight();
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("atendance device");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(100); 
    lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sensor not found");}
  }


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



g=EEPROM.read(addr);
Serial.println("______________memory location ");
Serial.println(g);
Serial.println("______________memory location ");

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()                     // run over and over again
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Atendance device");
  lcd.setCursor(0,1);
 // lcd.print("  Press finger");
  
  startRegistration();
  startClass();
  delay(200);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    lcd.setCursor(0,1);
  lcd.print("   Press finger");
  delay(200);
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK){
    lcd.clear();
    lcd.setCursor(0,0);
  lcd.print("   Try again");
  delay(1000);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK){
    lcd.clear();
    lcd.setCursor(0,0);
  lcd.print(" Not registered");
  delay(1000);
    return -1;
  }
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

stud_id=finger.fingerID;
lcd.clear();
lcd.setCursor(0,0);
  lcd.print("Student:");lcd.print(stud_id);
  lcd.print(" Attend");
  lcd.setCursor(0,1);
  lcd.print("Module: IT");lcd.print(course_id);
  
  delay(4000);

  if(finger.fingerID==1 && finger.confidence>100){
    digitalWrite(led, HIGH);
    Serial.print(" LED ON");
  }
  if(finger.fingerID==2 && finger.confidence>100){
    digitalWrite(led, LOW);
    Serial.print(" LED OFF");
  }
 pushDataToDatabase(); 
  return finger.fingerID; 
}

void registerStudent(uint16_t code){
  uint8_t imageCaptionStatus;
  uint8_t imageToTemplateStatus;
  uint8_t modelCreationStatus;
  uint8_t modelStoringStatus;

 lcd.clear();
 lcd.setCursor(0,0);
 lcd.println("PRESS FINGER");
 Serial.println("PRESS FINGER");
while(imageCaptionStatus = finger.getImage());
delay(100);
if(imageCaptionStatus == FINGERPRINT_OK){
    imageToTemplateStatus = finger.image2Tz(1);
    if(imageCaptionStatus == FINGERPRINT_OK){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("REMOVE FINGER");
      Serial.println("REMOVE FINGER");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PRESS AGAIN");
      Serial.println("PRESS AGAIN");
      while(imageCaptionStatus = finger.getImage());
      if(imageCaptionStatus == FINGERPRINT_OK){
        imageToTemplateStatus = finger.image2Tz(2);
        if(imageToTemplateStatus == FINGERPRINT_OK){
          modelCreationStatus = finger.createModel();
          if(modelCreationStatus == FINGERPRINT_OK){
            modelStoringStatus = finger.storeModel(code);
            if(modelStoringStatus == FINGERPRINT_OK){
              Serial.println("Registered!!");
              //EEPROM.write(STUDENTS_MEMORY_STARTS + students_counter,code);
             // students_counter++;
             // EEPROM.write(0,students_counter);
              Serial.println("Registered with success");
              lcd.clear();
              lcd.print("Registered!!"); 
              delay(1000);
            }
          }
        }
      }
      else {
        Serial.println("Retry!");
        lcd.clear();
        lcd.print("Retry"); 
        delay(1000);}
    }
  }
}

void pushDataToDatabase(){
Serial.print("connecting to ");
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connecting to ");
  lcd.setCursor(0,1);
  lcd.print(host);
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    lcd.setCursor(0,0);
  lcd.print("Connection fail");
    return;
  }
  
  // We now create a URI for the request where there are the php codes that sends them in database
  String url = "/Student-Attendance-System/recordAttendance.php?";
  url += "stud_id=";
  url += stud_id;
  url += "&course_id=";
  url += course_id;

  
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
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data pushed");

   delay(2000);
   //stud_id=stud_id+1;
}


void startRegistration(){
if(digitalRead(b)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(b)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(b)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(b)==HIGH);  //Confirm button release
lcd.clear();
lcd.print("Registration Mod");delay(1000);
//finger.emptyDatabase();
while (g<gg){
  formatOptions();
 registerStudent(g);

  g=g+1;
}
gg=gg+1,
  delay(1000);
}
 
}
    }



void startClass(){
if(digitalRead(bb)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(bb)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(bb)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(bb)==HIGH);  //Confirm button release
while(count<4){
  //increasing_time();
  moving_cursor();
 // saving_settings();
  lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Select Module");
    lcd.setCursor(2,1);
    lcd.print("IT");lcd.print(course_id);
    delay(300);
    Save();
   // startRegistration()
}
count=0;
}
 
}
    }


  void moving_cursor(){
if(digitalRead(bb)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(bb)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(bb)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(bb)==HIGH);  //Confirm button release

course_id=course_id+5;
count=count+1;
 if(count==4){
  count=0;
  course_id=213;
 }

}
 
}
    }

    void Save(){
if(digitalRead(b)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(b)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(b)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(b)==HIGH);  //Confirm button release
lcd.clear();
lcd.print("Attending Mode");delay(1000);
count=10;
while(m==0){
  getFingerprintIDez();
  exitSave();
}


}
 
}
m=0;
    }



    void exitSave(){
if(digitalRead(b)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(b)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(b)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(b)==HIGH);  //Confirm button release
lcd.clear();
lcd.print("Attenance closed");delay(2000);
//count=10;
m=2;
  
}

}

    }



    void formatOptions(){
if(digitalRead(b)==HIGH)                //DETECTION OF BUTTON INTERFACE TO HIGH
  {   
      delay(10);                               //DELAY FOR ELIMANATING EDGE JITTER(bounce)
      if(digitalRead(b)==HIGH)            //CONFIRM BUTTON IS PRESSED
      {     
        while(digitalRead(b)==HIGH);    //WAIT FOR KEY INTERFACE TO HIGH
        delay(10);                         //DELAY FOR ELIMANATING EDGE JITTER
        while(digitalRead(b)==HIGH);  //Confirm button release
lcd.clear();
lcd.print("Registration Mod");delay(1000);
finger.emptyDatabase();

}
 
}
    }
