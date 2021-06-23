#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "config.h"
#include "led.h"

extern void lwIPServiceTimers(void);
extern void lwIPTimer(uint32_t ui32TimeMS) ;
extern void PID_task_t(void);
extern void send_udp_steering_encoder_reading(void) ;


volatile uint32_t leds_timer = 0;   //Required every 1000 ms
volatile uint32_t steering_encoder_reading_timer = 0;   //Required every 600 ms
volatile uint32_t main_function = 0;   //Required every 100 ms

#define ui32TickMS 10

extern void  timer_start(void)
{
    SysTickPeriodSet(systic_rate);
    IntMasterEnable();
    SysTickIntEnable();
    SysTickEnable();
    int x =SysCtlClockGet();
}


void SysTickIntHandler(void)
{
#if NO_SYS
    lwIPTimer(ui32TickMS);
    lwIPServiceTimers();
    main_function ++;  //main function trigger
    leds_timer++;
    steering_encoder_reading_timer++;
    if (main_function >= 10 )
       {
                // 100 ms
                main_function =0;
                PID_task_t();
       }
    if ((leds_timer == 300  ) || (leds_timer > 300  ))
        {
           leds_timer=0;
           led_off();
        }
    if (steering_encoder_reading_timer >= 35 )
       {
                // 350 ms
          steering_encoder_reading_timer =0;
          send_udp_steering_encoder_reading();
       }

#endif
}
