

#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include "stdint.h"
void InitConsole(void);
extern int  pid_loop(int target , int current_postion );
uint8_t encoder_reading_buffer[100]={0} ;
uint8_t set_point_buffer[10] ={0};
extern void  timer_start(void);
int sensor_Reading =0;
int control_action =0;
 int set__point =0;
int main()
{
    ecoder_init();
    motor_init();
    InitConsole();
    uint32_t i =0;
    for(i =0;i<65000;i++)
    {

    }
    set__point =-700;
    timer_start();
     // Turn_Left(0); //CWW
    //Turn_Right(15000);//CW
    while(1)
    {

//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
////        motor_stop();
////         Turn_Left(500);
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
////        motor_stop();
////         Turn_Left(500);
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
//        for(i =0;i<655000;i++)
//            {
//
//            }
       // set__point = 1000;
//        motor_stop();
       //Turn_Right(100);
    }
}

extern  void task_t(void)
{
    static c = 200;
 sensor_Reading = ecoder_read();
 control_action = pid_loop(set__point ,sensor_Reading);
 UARTprintf("%d \n",sensor_Reading);
 if(control_action >0)
 {
      Turn_Left( (16000- control_action)*100  );
 }
 else
 {
     control_action = -control_action;
      Turn_Right(  (16000-  control_action)*35  );
 }
 c = c +1;

}
