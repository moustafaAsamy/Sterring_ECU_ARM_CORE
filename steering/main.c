

#include "motor.h"
#include "pid.h"
#include "encoder.h"
#include "stdint.h"
#include "ADC.h"
#include "driverlib/sysctl.h"
void InitConsole(void);
extern void UARTprintf(const char *pcString, ...);
extern int  pid_loop(int target , int current_postion );
uint8_t encoder_reading_buffer[100]={0} ;
uint8_t set_point_buffer[10] ={0};
extern void  timer_start(void);
int sensor_Reading =0;
int control_action =0;
volatile int set__point =0;
volatile int get_ratio = 0;
extern void ButtonsInit(void);
void led_init(void);
uint16_t percentage_change(uint8_t input);
uint16_t change(uint16_t input);
volatile uint8_t stop_flag =0;
int c =0;
int main()
{
    SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                      SYSCTL_XTAL_16MHZ);
    ButtonsInit();
    led_init();
    ecoder_init();
    motor_init();
    InitConsole();
    ADC_INIT();

    uint32_t i =0;
    for(i =0;i<65000;i++)
    {

    }
    set__point =400;
    timer_start();
     // Turn_Left(0); //CWW
    //Turn_Right(15000);//CW
//    int c =change(set__point);
//    Turn_Right(c);
    while(1)
    {
        //Turn_Right(  (1000-  control_action)   );

//        get_ratio =ADC_get();
//        if (stop_flag== 1)
//        {
//            stop_flag = 0;
//             motor_stop();
//            SysCtlDelay(SysCtlClockGet() / 12);
//        }
//        if(get_ratio >0)
//        {
//
//           c= percentage_change(get_ratio);
//            Turn_Left(c);
//        }
//        else
//        {
//            // c= percentage_change(-get_ratio);
//            c= percentage_change(-get_ratio);
//             Turn_Right(c);
//             //Turn_Right(get_ratio);
//        }
//       // Turn_Left(1000-(get_ratio*10));
//         SysCtlDelay(SysCtlClockGet() / 12);
//       // Turn_Right(set__point);
////          c =percentage_change(set__point);
////        Turn_Right(c);
     }
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

extern  void task_t(void)
{
 sensor_Reading = ecoder_read();
 control_action = pid_loop(set__point ,sensor_Reading);
 UARTprintf("%d \n",sensor_Reading);

 if(control_action >0)
 {
      motor_stop();
      c= percentage_change(control_action);
      Turn_Left(c);

 }
 else
 {
     motor_stop();
    // control_action = -control_action;
     c= percentage_change(-control_action);
    Turn_Right(c);

 }
 //        if(get_ratio >0)
 //        {
 //
 //           c= percentage_change(get_ratio);
 //            Turn_Left(c);
 //        }
 //        else
 //        {
 //            // c= percentage_change(-get_ratio);
 //            c= percentage_change(-get_ratio);
 //             Turn_Right(c);
 //             //Turn_Right(get_ratio);
 //        }

}


extern void task_set_point_change(void)
{
    set__point =set__point -100;
  //control_action = control_action +100;
}

