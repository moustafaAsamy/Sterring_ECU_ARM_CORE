/*
 * pid.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Lenovo PC
 */


#include "stdint.h"

/* constants for PID */
const float Kp = 5;
const float Ki = 0.047;
const float Kd = 0.5;
const int Set_Point = 353;


int32_t MainDuty = 0;                   // Main Duty:
int32_t d_lastError_main = 0;                //
int32_t i_sum_main = 0;                // Accumulated error sum
int32_t PWM_last_main = 0;              //


#define DUTYSCALER              1000
#define PWM_MIN                 5
#define PWM_MAX                 95

// PID controller for the main motor
uint32_t Maincontroller(int32_t target, int32_t current)
{
    // Scales the values up by a constant so for example 1000 * 0.01 can be 1010 instead of 1 * 0.01 getting 1.01
    // because decimals are inacurate and even small changes from rounding could be a problem
    target = target ;
    current = current ;
    int32_t i_Max = 10000 ;
    int32_t i_Min = 0;

//    float Kp = 0.7;
//    float Ki = 0.01;
//    float Kd = 0.1;

    int32_t error = target - current;

    // Proportional: The error times the proportional coefficent (Kp)
    int32_t P = error * Kp;

    // Add the current Error to the error sum
    i_sum_main += error  ;

    // Limit the summed error to between i_max and i_min
    if (i_sum_main > i_Max) i_sum_main = i_Max;
    else if (i_sum_main < i_Min) i_sum_main = i_Min;

    // Integral: Multiply the sum by the integral coefficent (Ki)
    int32_t I = Ki * i_sum_main;

    // Derivative: Calculate change in error between now and last time through the controller
    // then multiply by the differential coefficent (Kd)
    int32_t D = Kd * (d_lastError_main - error);

    // Store error to be used to calculate the change next time
    d_lastError_main = error;

    // Combine the proportional, intergral and dirrivetave components and then scales back down.
    //      (looking at it again im not sure why this isn't just "P + I + D" as the previous
    //      duty cycle shouldn't matter, I'll test changing this)
    int32_t PWM_Duty = (P + I + D)  ;

    // Limit the duty cycle to between 95 and 5
    if (PWM_Duty > 95) PWM_Duty = PWM_MAX;
    else if (PWM_Duty < 5) PWM_Duty = PWM_MIN;

    PWM_last_main = PWM_Duty;
    return PWM_Duty;
}

int integral=0  ,last_error =0 ,derivative =0;

int  pid_loop(int target , int current_postion )
{
//current_postion = read_;
int error = target -current_postion;
integral = integral +error ;
//if (integral > 10000)
//{
//    integral = 10000;
//
//}
//else if (integral <-10000)
//{
//    integral =-10000;
//}
derivative =error - last_error ;
int pwm =(Kp*error) + (Ki*integral) + (Kd*derivative);


if(pwm >1590)
{pwm =1590;}
else if (pwm <-1590) {pwm =-1590;}

last_error =error ;
return pwm ;
//if (pwm >0) motorcw(pwm)
//
//elseif (pwm <0)motorcc(-pwm)
//
//else()
//{
//    motor(stop)
//}
}
