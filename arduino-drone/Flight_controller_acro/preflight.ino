


  void startup(){

  pinMode(CH1_PIN, INPUT);                                                        //Set DDR (Data Direction Register) as INPUT
  pinMode(CH2_PIN, INPUT);                                                        //Set DDR (Data Direction Register) as INPUT
  pinMode(CH3_PIN, INPUT);                                                        //Set DDR (Data Direction Register) as INPUT
  pinMode(CH4_PIN, INPUT);                                                        //Set DDR (Data Direction Register) as INPUT

  pinMode(MOTOR_1_PIN, OUTPUT);                                                   //Set DDR (Data Direction Register) as OUTPUT --> same as DDRB = B00001111 but this one is not so readble 
  pinMode(MOTOR_2_PIN, OUTPUT);                                                   //Set DDR (Data Direction Register) as OUTPUT --> same as DDRB = B00001111 but this one is not so readble 
  pinMode(MOTOR_3_PIN, OUTPUT);                                                   //Set DDR (Data Direction Register) as OUTPUT --> same as DDRB = B00001111 but this one is not so readble 
  pinMode(MOTOR_4_PIN, OUTPUT);                                                   //Set DDR (Data Direction Register) as OUTPUT --> same as DDRB = B00001111 but this one is not so readble 

  pinMode(ALARM_LED, OUTPUT);                                                     
  digitalWrite(ALARM_LED, LOW);
  
}








  void interrupt_registers_startup(){
    
    //Pin Change Interrupt Control Register and PCIEx are the individual bits of PCICR so in these lines we are setting 1 on the most significant bit PCIE2 according to the doc to enable scanning 
    //over PCMSK2 on the PCMSK corrisponding to the bit setted to 1   
    PCICR |= (1 << PCIE2);    //enable PCMSK2 as reported here  --> https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328 
    
    /* Following registers table reported in the datasheet we enable pin change interrupt using Pin Change Mask Register singol bits */
    PCMSK2 |= (1 << PCINT20);  //Triggers pin D4 on pin state change (from 0 to 1 or from 1 to 0)  -->  Pitch       channel
    PCMSK2 |= (1 << PCINT21);  //Triggers pin D5 on pin state change (from 0 to 1 or from 1 to 0)  -->  Roll        channel                                          
    PCMSK2 |= (1 << PCINT22);  //Triggers pin D6 on pin state change (from 0 to 1 or from 1 to 0)  -->  Yaw         channel                                          
    PCMSK2 |= (1 << PCINT23);  //Triggers pin D7 on pin state change (from 0 to 1 or from 1 to 0)  -->  Throttle    channel
    
}









  void motors_startup(){
  
    for(i = 0; i<500; i++){ 
      PORTB = B00001111;
      delayMicroseconds(1000);
      PORTB = B00000000;
      delayMicroseconds(3000);
    } 
    
  }











  void check_gyro_coherence(){
    
    switch( MAX_GYRO_VALUE ){
      case 250:
        if(GYRO_PRESCALER != 131.0 || MIN_GYRO_VALUE !=  -250){ show_alarm(); };
        break;
      case 500:
        if(GYRO_PRESCALER !=  65.5 || MIN_GYRO_VALUE !=  -500){ show_alarm(); };
        break;
      case 1000:
        if(GYRO_PRESCALER !=  32.8 || MIN_GYRO_VALUE != -1000){ show_alarm(); };
        break;      
      case 2000:
        if(GYRO_PRESCALER !=  16.4 || MIN_GYRO_VALUE  != -2000){ show_alarm(); };
        break;
      default:
        break;  
    }
    
  }
  
  
  
  
  
  
  
  bool security_check(){
  
    if( NOT_ARMED_PWM_PULSE < 1010 || NOT_ARMED_PWM_PULSE > 1050 ){ return true; }
    
    if( MIN_SPIN < 1050 || MIN_SPIN > 1150 ){ return true; }
    if( MAX_SPIN != 2000 ){ return true; }
    if( MAX_THROTTLE < 1700 || MAX_THROTTLE > 1900 ){ return true; }
  
    if( FLIP_GYRO_X_DATA_SIGN != -1 && FLIP_GYRO_X_DATA_SIGN != 1 ){ return true; }
    if( FLIP_GYRO_Y_DATA_SIGN != -1 && FLIP_GYRO_Y_DATA_SIGN != 1 ){ return true; }  
    if( FLIP_GYRO_Z_DATA_SIGN != -1 && FLIP_GYRO_Z_DATA_SIGN != 1 ){ return true; }
    
    if( SW_PITCH_REVERSE != -1 && SW_PITCH_REVERSE != 1) { return true; }
    if( SW_ROLL_REVERSE  != -1 && SW_ROLL_REVERSE  != 1) { return true; }
    if( SW_YAW_REVERSE   != -1 && SW_YAW_REVERSE   != 1) { return true; }
  
    if( ( PWM_MAX - DEADBAND_UPPER_BOUND ) / SETPOINT_PITCH_SPEED_RATE > MAX_GYRO_VALUE ){ return true; }
    if( ( PWM_MAX - DEADBAND_UPPER_BOUND ) / SETPOINT_ROLL_SPEED_RATE  > MAX_GYRO_VALUE ){ return true; } 
    if( ( PWM_MAX - DEADBAND_UPPER_BOUND ) / SETPOINT_YAW_SPEED_RATE   > MAX_GYRO_VALUE ){ return true; }     
  
    if( THROTTLE_LOWER_THRESHOLD != 1040 ){ return true; }
  
    
    if( SETPOINT_PITCH_SPEED_RATE <= 0 || SETPOINT_ROLL_SPEED_RATE  <= 0 || SETPOINT_YAW_SPEED_RATE <= 0 ){ return true; }
  
    return false;
    
  }
