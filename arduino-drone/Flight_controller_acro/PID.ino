







    
  void reset_pid(){
    
        pid_i_x = 0;                                    
        pid_i_y = 0;                                      
        pid_i_z = 0;  
  
        previous_error_x = 0;                                         
        previous_error_y = 0;                                      
        previous_error_z = 0;                 
  }









  void calculate_pid(){
  
  error_x = gyro_pitch_pid_input - desired_angle_rate_x;                   //input for rate/acro mode is the difference between gyro and radio normalized pulse 
  error_y = gyro_roll_pid_input  - desired_angle_rate_y;                   //input for rate/acro mode is the difference between gyro and radio normalized pulse 
  error_z = gyro_yaw_pid_input   - desired_angle_rate_z;                   //input for rate/acro mode is the difference between gyro and radio normalized pulse 
    
  pid_p_x = kp_pitch * error_x;                                            //proportional value of PID controller for X
  pid_p_y = kp_roll  * error_y;                                            //proportional value of PID controller for Y
  pid_p_z = kp_yaw   * error_z;                                            //proportional value of PID controller for Z

  pid_i_x = pid_i_x + (ki_pitch * error_x);                                //integral value of PID controller for X
  pid_i_y = pid_i_y + (ki_roll  * error_y);                                //integral value of PID controller for Y
  pid_i_z = pid_i_z + (ki_yaw   * error_z);                                //integral value of PID controller for Z   

  if(pid_i_x > PID_UPPER_BOUND){ pid_i_x = PID_UPPER_BOUND; }              //avoid extreme output of integral component 
  if(pid_i_x < PID_LOWER_BOUND){ pid_i_x = PID_LOWER_BOUND; }              //avoid extreme output of integral component 
  if(pid_i_y > PID_UPPER_BOUND){ pid_i_y = PID_UPPER_BOUND; }              //avoid extreme output of integral component 
  if(pid_i_y < PID_LOWER_BOUND){ pid_i_y = PID_LOWER_BOUND; }              //avoid extreme output of integral component 
  if(pid_i_z > PID_UPPER_BOUND){ pid_i_z = PID_UPPER_BOUND; }              //avoid extreme output of integral component 
  if(pid_i_z < PID_LOWER_BOUND){ pid_i_z = PID_LOWER_BOUND; }              //avoid extreme output of integral component        
                
  pid_d_x = kd_pitch * (error_x - previous_error_x);                       //derivative component acts on the speed of the error 
  pid_d_y = kd_roll  * (error_y - previous_error_y);                       //derivative component acts on the speed of the error    
  pid_d_z = kd_yaw   * (error_z - previous_error_z);                       //derivative component acts on the speed of the error  

  total_pid_x = pid_p_x + pid_i_x + pid_d_x;                               //final PID for X axis
  total_pid_y = pid_p_y + pid_i_y + pid_d_y;                               //final PID for Y axis
  total_pid_z = pid_p_z + pid_i_z + pid_d_z;                               //final PID for Z axis

  if( total_pid_x < PID_LOWER_BOUND ){ total_pid_x = PID_LOWER_BOUND; }    //value normalization 
  if( total_pid_x > PID_UPPER_BOUND ){ total_pid_x = PID_UPPER_BOUND; }    //value normalization 
  if( total_pid_y < PID_LOWER_BOUND ){ total_pid_y = PID_LOWER_BOUND; }    //value normalization 
  if( total_pid_y > PID_UPPER_BOUND ){ total_pid_y = PID_UPPER_BOUND; }    //value normalization 
  if( total_pid_z < PID_LOWER_BOUND ){ total_pid_z = PID_LOWER_BOUND; }    //value normalization 
  if( total_pid_z > PID_UPPER_BOUND ){ total_pid_z = PID_UPPER_BOUND; }    //value normalization 


  throttle = radioChannelCommands[THROTTLE];                               //assign throttle value to a variable in order to not disable interrupts

  if(    throttle > MAX_THROTTLE    ){ throttle = MAX_THROTTLE;  }         //ceil for PID controller at maximum throttle  

  speedVec[MOTOR_1] = throttle + total_pid_y + total_pid_x + total_pid_z;  //rear  right motor PWM    
  speedVec[MOTOR_2] = throttle + total_pid_y - total_pid_x - total_pid_z;  //front right motor PWM    
  speedVec[MOTOR_3] = throttle - total_pid_y + total_pid_x - total_pid_z;  //rear   left motor PWM  
  speedVec[MOTOR_4] = throttle - total_pid_y - total_pid_x + total_pid_z;  //front  left motor PWM  


  for(i = 0; i < MOTORS_NUM; i++){                                         //speed array normalization 
    if(speedVec[i] < MIN_SPIN){ speedVec[i] = MIN_SPIN; }                  //if the value is less than the minimum
    if(speedVec[i] > MAX_SPIN){ speedVec[i] = MAX_SPIN; }                  //if the value is greater than the maximum
  }

  
  previous_error_x = error_x;                                              //actual error become previous error for the next cycle
  previous_error_y = error_y;                                              //actual error become previous error for the next cycle
  previous_error_z = error_z;                                              //actual error become previous error for the next cycle
  
}
