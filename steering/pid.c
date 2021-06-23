/*
 * pid.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Lenovo PC
 */
#include "stdint.h"


int sensor_Reading =0;
int control_action =0;
volatile int set__point =0;


/* constants for PID */
const float Kp =4.2;
const float Ki = 0.0005;//0.001;
const float Kd = 0.5;


int integral=0  ,last_error =0 ,derivative =0;

int  pid_loop(int target , int current_postion )
{
   //current_postion = read_;
   int error = target -current_postion;
   integral = integral +error ;
   derivative =error - last_error ;
   int pwm =(Kp*error) + (Ki*integral) + (Kd*derivative);


   if(pwm >99)
   { pwm =99;}
   else if (pwm <-99) {pwm =-99;}

   last_error =error ;
   return pwm ;
}


uint16_t change(uint16_t input)
{
 /*
  *  output  max = 0      min = 15900
  *  input   max =15900   min = 0
  */
    if (input > 15900)
    {
        return 0;
    }
    else if(input <2)
    {
        return 15900;
    }
    else
    {
        return (15900 -input );
    }
}
uint16_t percentage_change(uint8_t input)
{
   float x = (float)input ;
   x=x/100;
   return change(x *16000);
}

void PID_task_t(void)
{
   sensor_Reading = ecoder_read();
   control_action = pid_loop(set__point ,sensor_Reading);
   UARTprintf("%d \n",sensor_Reading);
   int c =0;

   if(control_action >0)
   {
      motor_stop();
      c= percentage_change(control_action);
      Turn_Left(c);
   }
   else
   {
     motor_stop();
     c= percentage_change(-control_action);
     Turn_Right(c);
   }

}

extern void task_set_point_change(void)
{
   set__point =set__point -100;
}
