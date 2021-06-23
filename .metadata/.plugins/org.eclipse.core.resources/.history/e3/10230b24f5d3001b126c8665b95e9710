


#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
//#include "lwip_int.h"
#include "config.h"
extern void task2(void);
#include "led.h"
extern void lwIPServiceTimers(void);
extern void TCP_sent_buffer(void);
extern void lwIPTimer(uint32_t ui32TimeMS) ;
extern int send_udp(const uint8_t* data, int length);
extern uint8_t buffer[100];
extern  void task_t(void);
extern void  timer_start(void)
{
    SysTickPeriodSet(systic_rate);
    IntMasterEnable();
    SysTickIntEnable();
    SysTickEnable();
    int x =SysCtlClockGet();
}

volatile uint32_t leds_timer = 0;   //Required every 1000 ms
volatile uint32_t TCp_counter = 0;   //Required every 500 ms 50 *10
volatile uint32_t UDp_counter = 0;   //Required every 600 ms
volatile uint32_t TCP_timer =   0;   //Required every 500 ms
volatile uint32_t main_function = 0;   //Required every 100 ms
uint32_t g_ui32_APP_Timer = 0;
#define ui32TickMS 10
extern volatile int set__point ;
void SysTickIntHandler(void)
{
#if NO_SYS
    lwIPTimer(ui32TickMS);
    lwIPServiceTimers();
    g_ui32_APP_Timer =  g_ui32_APP_Timer + ui32TickMS;
    UDp_counter =  UDp_counter + ui32TickMS;
    main_function ++;  //main function trigger
    leds_timer++;
#if ECU1
    if(g_ui32_APP_Timer >= 1500)
    {
       // app_task_tx_tcp();
        //send(buffer,100);
        g_ui32_APP_Timer=0;
    }
        if  (UDp_counter == 3000 )
           {
               UDp_counter =0;
    //           app_task_tx_udp();
           }
#endif
        if (main_function >= 10 )
            {
                main_function =0;
                 task_t();
                //task2();
                //send_udp(buffer,4);
               // TcpIp_MainFunction();
            }
            if ((leds_timer == 300  ) || (leds_timer > 300  ))
                {
                leds_timer=0;
                 led_off();
               // set__point = set__point +250;
                }

#endif
}
