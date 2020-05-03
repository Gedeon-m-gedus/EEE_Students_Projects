
////////////////////////////////////////////AR-01 EC///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/* Author          : Edoardo Cittadini                                                       */
/* Target platform : Arduino UNO - Arduino MEGA2560 - Arduino NANO - Arduino micro           */
/* Last update     : 02-05-2019                                                              */
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                           //
//                                            ^                                              //
//                                            |                                              //
//           4 --> speedVec[MOTOR_4]    X           X    <-- 2 speedVec[MOTOR_2]             //
//                                         -     -                                           //
//                                            -                                              //
//                                         -     -                                           //
//           3 --> speedVec[MOTOR_3]    X           X    <-- 1 speedVec[MOTOR_1]             //
//                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////

  
  #include <Wire.h>


  
  #define MOTORS_NUM                        4 //number of motors (quad --> 4) --> possible extention to hexacopter in future 

  #define CH1_PIN                           4 //radio channel 1 --- PITCH 
  #define CH2_PIN                           5 //radio channel 2 --- ROLL 
  #define CH3_PIN                           7 //radio channel 3 --- THROTTLE
  #define CH4_PIN                           6 //radio channel 4 --- YAW

  #define MOTOR_1_PIN                       8 //rear right motor
  #define MOTOR_2_PIN                       9 //front right motor
  #define MOTOR_3_PIN                      10 //rear left motor
  #define MOTOR_4_PIN                      11 //front left motor

  #define SW_PITCH_REVERSE                  1 //channel inverter (for example if pitching forward the quad goes backward)  
  #define SW_ROLL_REVERSE                   1 //channel inverter               
  #define SW_YAW_REVERSE                    1 //channel inverter


  #define PWM_MAX                        2000 //DO NOT MODIFY defines the maximum PWM pulse width
      
  #define MPU_6050                       0x68 //DO NOT MODIFY slave address of IMU MPU_6050 over I2C 
  #define WAKE_UP_REG                    0x6B //DO NOT MODIFY wake-up register of the unit 
  #define TEMPERATURE_REG                0x41 //DO NOT MODIFY temperature value first register
  #define GYRO_CONFIG_REG                0x1B //DO NOT MODIFY register that allows to modify gyro configurations
  #define DPS_FULL_SCALE_500             0x08 //DO NOT MODIFY gyro dps mask
  #define LOW_PASS_FILTER_REG            0x1A //DO NOT MODIFY register used to enable am hardware low-pass filter buil in the MPU-6050
  #define GYRO_DATA_REG                  0x43 //DO NOT MODIFY first data register of the requested burst
  #define CALIBRATION_SAMPLES            2000 //number of iterations that will be performed in order to find calibration values
  #define FLIP_GYRO_X_DATA_SIGN             1 //inverter of gyro motion (it must agree with the model given in the doc)
  #define FLIP_GYRO_Y_DATA_SIGN             1 //inverter of gyro motion (it must agree with the model given in the doc)
  #define FLIP_GYRO_Z_DATA_SIGN            -1 //inverter of gyro motion (it must agree with the model given in the doc)
  #define GYRO_PRESCALER                 65.5 //divisor to obtain the true data from raw data for a gyro configuration of 500 dps
  #define MAX_GYRO_VALUE                  500 //valued expressed in degrees per second... from datasheet the possible values are { +- 250, 500, 1000, 2000 } 
  #define MIN_GYRO_VALUE                 -500 //valued expressed in degrees per second... from datasheet the possible values are { +- 250, 500, 1000, 2000 } 

  #define DEADBAND_UPPER_BOUND           1508 //upper bound of the middle position where the radio signal is evaluated as 0 dps
  #define DEADBAND_LOWER_BOUND           1492 //lower bound of the middle position where the radio signal is evaluated as 0 dps

  
  #define SETPOINT_PITCH_SPEED_RATE       1.5 // ex for value 3   -->   (500 - 8) / 3 = 164 deg/sec
  #define SETPOINT_ROLL_SPEED_RATE        1.5 // ex for value 3   -->   (500 - 8) / 3 = 164 deg/sec
  #define SETPOINT_YAW_SPEED_RATE         3.0 // ex for value 4   -->   (500 - 8) / 4 = 125 deg/sec
  
  #define PID_UPPER_BOUND                 400 //DO NOT MODIFY best tested parameters for maximum PID positive output
  #define PID_LOWER_BOUND                -400 //DO NOT MODIFY best tested parameters for maximum PID negative output
  
  #define MAX_THROTTLE                   1800 //ceil given to the PID... in this case PID has 200 free microseconds to make corrections when the quad is at its max throttle. Allowed values [1700; 1900]
  #define THROTTLE_LOWER_THRESHOLD       1040 //DO NOT MODIFY under this value arm/disarm commands are enabled
  #define NOT_ARMED_PWM_PULSE            1040 //it is the value given to the ESC in order to not beep and at the same time to not make motors spin. Allowed values [1010; 1050]
  #define MIN_SPIN                       1060 //you have to test for which values your motors start their motion... Allowed values for security reasons [1050; 1150] 
  #define MAX_SPIN                       2000 //maxium value that final pulse can reach



//////////////////////////////////////PID CONSTANTS///////////////////////////////////////
                                                                                        //
  float kp_pitch = 2.00;                                                                //
  float ki_pitch = 0.02;                                                                //
  float kd_pitch = 12.0;                                                                //
                                                                                        //
  float kp_roll  = 2.00;                                                                //
  float ki_roll  = 0.02;                                                                //
  float kd_roll  = 12.0;                                                                //    
                                                                                        //
  float kp_yaw   = 2.00;                                                                //
  float ki_yaw   = 0.02;                                                                //
  float kd_yaw   = 0.00;                                                                //     good parameter choice allow to not have derivative on yaw because Z axis already has enought friction with propellers spin                                               
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////VARIABILI PID ED ERRORI///////////////////////////////////
                                                                                        //
                                                                                        //
  float pid_p_x=0;                                                                      //
  float pid_i_x=0;                                                                      //
  float pid_d_x=0;                                                                      //
                                                                                        //
  float pid_p_y=0;                                                                      //
  float pid_i_y=0;                                                                      //
  float pid_d_y=0;                                                                      //
                                                                                        //
  float pid_p_z=0;                                                                      //
  float pid_i_z=0;                                                                      //
  float pid_d_z=0;                                                                      //
                                                                                        //
  int16_t gyro_roll_pid_input = 0, gyro_pitch_pid_input = 0, gyro_yaw_pid_input = 0;    //
  int16_t error_x = 0, error_y = 0, error_z = 0;                                        //
  int16_t previous_error_x = 0, previous_error_y = 0, previous_error_z = 0;             //
  float total_pid_x = 0, total_pid_y = 0, total_pid_z = 0;                              //
  int16_t desired_angle_rate_x = 0, desired_angle_rate_y = 0, desired_angle_rate_z = 0; //
  uint16_t speedVec[MOTORS_NUM] = {0, 0, 0, 0};                                         //
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////                                                                                     
                                                                                      
//////////////////////////////RADIOCOMANDO ED INTERRUPTS//////////////////////////////////
                                                                                        //
  volatile unsigned long counter_1, counter_2, counter_3, counter_4, current_count;     //      
  volatile uint8_t  CH1_LS, CH2_LS,CH3_LS,CH4_LS;                                       //      
  volatile uint16_t radioChannelCommands[4];                                            //     
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////IMU E GIROSCOPIO///////////////////////////////////////
                                                                                        //
  int16_t Gyr_rawX = 0, Gyr_rawY = 0, Gyr_rawZ = 0;                                     //
  int16_t Gyro_raw_error_x = 0, Gyro_raw_error_y = 0, Gyro_raw_error_z = 0;             //
  float gyro_angle_x = 0.0, gyro_angle_y = 0.0, gyro_angle_z = 0.0;                     //
//////////////////////////////////////////////////////////////////////////////////////////                                                                                     
  
//////////////////////////////////VARIABILI GENERICHE/////////////////////////////////////  
                                                                                        //
  uint16_t i = 0;                                                                       //                                                         
  uint16_t throttle = 0;                                                                //
  uint8_t armed = 0 ;                                                                   //
  bool firstSpin = true;                                                                //
  unsigned currTime = 0;                                                                //
  uint16_t time_to_spin = 0;                                                            //
  enum motors{ MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4 };                                    //
  enum signals{ PITCH, ROLL, THROTTLE, YAW };                                           //
                                                                                        //
  enum debug_pars{ GYRO_ANGLES,                                                         //
                   TOTAL_ANGLES,                                                        //
                   DESIRED_ANGLES,                                                      //
                   PID_ERROR,                                                           //
                   PID_OUTPUT,                                                          //
                   RADIO_COMMANDS,                                                      //
                   MOTORS_FINAL_PULSE,                                                  //
                   PID_COMPONENTS,                                                      //
                   TO_EXEL_GRID,                                                        //
                   ELAPSED_DISTANCE,                                                    //
                   TEMPERATURE                                                          //
                   };                                                                   //
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////  

//////////////////////////////////////DATI SENSORI////////////////////////////////////////  
                                                                                        //
  unsigned long duration = 0;                                                           //
  uint8_t elapsed_distance = 0;                                                         //
  int16_t temperature = 0;                                                              //
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////  
                  
///////////////////////////////////SCHED VARIABLES////////////////////////////////////////
                                                                                        //
  unsigned long prevMotorWrite = 0;                                                     //
                                                                                        //
//////////////////////////////////////////////////////////////////////////////////////////



void setup() {
  
  interrupt_registers_startup();
  startup();
  
  while( security_check() ){ show_alarm(); };
  
  initialize_imu();
  check_gyro_coherence();
  calibrate_imu();
  motors_startup();

}




void loop(){
  
  check_for_motor_arm();
  read_imu();
  map_radio_channels_to_angle_rates();
  calculate_pid();
  motors_synch();
  write_motor_pulse();
  
}
