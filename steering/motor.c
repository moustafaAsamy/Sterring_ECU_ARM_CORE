
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"


void motor_init(void)
{
     MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
     MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
     //MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
     //MAP_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_7);
     MAP_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_2);
     MAP_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_4);
     GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
     GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
     GPIOPinConfigure(GPIO_PB1_T2CCP1);
     GPIOPinConfigure(GPIO_PB0_T2CCP0);
     GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_1);
     GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_0);
     TimerConfigure(TIMER2_BASE,    TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM|TIMER_CFG_A_PWM);  //X

     TimerLoadSet(TIMER2_BASE, TIMER_B, 16000);
     TimerMatchSet(TIMER2_BASE, TIMER_B,  15900   );
     TimerEnable(TIMER2_BASE, TIMER_B);

     TimerLoadSet(TIMER2_BASE, TIMER_A, 16000);
     TimerMatchSet(TIMER2_BASE, TIMER_A,   15900);
     TimerEnable(TIMER2_BASE, TIMER_A);
}

void Turn_Right(uint16_t control)
{
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
    TimerMatchSet(TIMER2_BASE, TIMER_A,  15990);
    TimerDisable(TIMER2_BASE, TIMER_A);
    TimerMatchSet(TIMER2_BASE, TIMER_B,  control);
    TimerEnable(TIMER2_BASE, TIMER_B);
}

void Turn_Left(uint16_t control )
{
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_PIN_4);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
    TimerMatchSet(TIMER2_BASE, TIMER_B,  15990);
    TimerDisable(TIMER2_BASE, TIMER_B);
    TimerMatchSet(TIMER2_BASE, TIMER_A,   control);
    TimerEnable(TIMER2_BASE, TIMER_A);
}

void motor_stop(void)
{
    TimerDisable(TIMER2_BASE, TIMER_B);
       TimerDisable(TIMER2_BASE, TIMER_A);
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);

//    TimerDisable(TIMER2_BASE, TIMER_B);
//    TimerDisable(TIMER2_BASE, TIMER_A);

}
