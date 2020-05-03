



  
  void motors_synch(){
  
    while(micros() - prevMotorWrite < 4000){};
     
  }
  




  
  void show_alarm(){
    
    digitalWrite(ALARM_LED, HIGH);
    delay(500);
    digitalWrite(ALARM_LED, LOW);
    delay(500);
       
  }
  
