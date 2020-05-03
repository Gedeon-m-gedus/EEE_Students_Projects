


void initialize_imu() {

  Wire.begin();                                                                   //comunication start
  Wire.beginTransmission(MPU_6050);                                               //I need to comunicate with the IMPU
  Wire.write(WAKE_UP_REG);                                                        //wake-up the unit
  Wire.write(0x00);                                                               //write 0 for reset
  Wire.endTransmission(true);                                                     //comunication end

  Wire.beginTransmission(MPU_6050);
  Wire.write(GYRO_CONFIG_REG);                                                    //we want to write to the register GYRO_CONFIG addressed at 1B hex
  Wire.write(DPS_FULL_SCALE_500);                                                 //8 bit register setted 00001000 in order to have 500dps full scale as reported in datasheet
  Wire.endTransmission();                                                         //comunication end

  Wire.beginTransmission(MPU_6050);
  Wire.write(GYRO_CONFIG_REG);                                                    //we want to read 0x1B register
  Wire.endTransmission();
  Wire.requestFrom(MPU_6050, 1);                                                  //ask for a byte

  while (Wire.available() < 1) {
    show_alarm();  //wait until the byte is received
  }

  if (Wire.read() != DPS_FULL_SCALE_500) {                                        //consistency check
    while (1) {
      show_alarm();  //something went wrong so it is forbidden to the flight controller to start motors
    }
  }

  Wire.beginTransmission(MPU_6050);
  Wire.write(LOW_PASS_FILTER_REG);                                                //we want to write to the register GYRO_CONFIG addressed at 1A hex
  Wire.write(0x03);                                                               //8 bit register setted 00000011 in order to have the Digital Low Pass Filter at ~43Hz
  Wire.endTransmission();                                                        

}








void calibrate_imu() {

  for (i = 0; i < CALIBRATION_SAMPLES; i++) {
    Wire.beginTransmission(MPU_6050);                                             //begin comunication with MPU-6050
    Wire.write(GYRO_DATA_REG);                                                    //first gyro data register
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_6050, 6, true);                                          //request for 6 registers

    Gyr_rawX = Wire.read() << 8 | Wire.read();                                    //gyro 16 bit X axis value
    Gyr_rawY = Wire.read() << 8 | Wire.read();                                    //gyro 16 bit Y axis value
    Gyr_rawZ = Wire.read() << 8 | Wire.read();                                    //gyro 16 bit Z axis value

    Gyro_raw_error_x =   Gyro_raw_error_x + (Gyr_rawX / GYRO_PRESCALER);          //cumulative value for the error on X axis
    Gyro_raw_error_y =   Gyro_raw_error_y + (Gyr_rawY / GYRO_PRESCALER);          //cumulative value for the error on Y axis
    Gyro_raw_error_z =   Gyro_raw_error_z + (Gyr_rawZ / GYRO_PRESCALER);          //cumulative value for the error on Z axis

    if (i == CALIBRATION_SAMPLES - 1) {
      Gyro_raw_error_x = Gyro_raw_error_x / CALIBRATION_SAMPLES;                  //final gyro X axis correcting offset value
      Gyro_raw_error_y = Gyro_raw_error_y / CALIBRATION_SAMPLES;                  //final gyro Y axis correcting offset value
      Gyro_raw_error_z = Gyro_raw_error_z / CALIBRATION_SAMPLES;                  //final gyro Z axis correcting offset value

      while (micros() - prevMotorWrite < 4000) {};
      write_motor_pulse(); //keep motor pulse to not make ESCs beep 
      prevMotorWrite = micros();
    }
  }

}






void read_imu() {

  Wire.beginTransmission(MPU_6050);
  Wire.write(GYRO_DATA_REG);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_6050, 6, true);

  Gyr_rawX = Wire.read() << 8 | Wire.read();
  Gyr_rawY = Wire.read() << 8 | Wire.read();
  Gyr_rawZ = Wire.read() << 8 | Wire.read();

  gyro_angle_x = ( (Gyr_rawX / GYRO_PRESCALER) - Gyro_raw_error_x ) * FLIP_GYRO_X_DATA_SIGN;   /*---X---*/
  gyro_angle_y = ( (Gyr_rawY / GYRO_PRESCALER) - Gyro_raw_error_y ) * FLIP_GYRO_Y_DATA_SIGN;   /*---Y---*/
  gyro_angle_z = ( (Gyr_rawZ / GYRO_PRESCALER) - Gyro_raw_error_z ) * FLIP_GYRO_Z_DATA_SIGN;   /*---Z---*/


  if ( gyro_angle_x >= MIN_GYRO_VALUE && gyro_angle_x <= MAX_GYRO_VALUE ) {
    gyro_pitch_pid_input = (gyro_pitch_pid_input * 0.7) + (gyro_angle_x * 0.3);
  }
  if ( gyro_angle_y >= MIN_GYRO_VALUE && gyro_angle_y <= MAX_GYRO_VALUE ) {
    gyro_roll_pid_input  = (gyro_roll_pid_input  * 0.7) + (gyro_angle_y * 0.3);
  }
  if ( gyro_angle_z >= MIN_GYRO_VALUE && gyro_angle_z <= MAX_GYRO_VALUE ) {
    gyro_yaw_pid_input   = (gyro_yaw_pid_input   * 0.7) + (gyro_angle_z * 0.3);
  }

}
