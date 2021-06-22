/*
 * buttons.c
 *
 *  Created on: Oct 21, 2019
 *      Author: lenovo
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/rom_map.h"
#include <driverlib/interrupt.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "led.h"
#include "driverlib/sysctl.h"

#define BUTTONS_GPIO_PERIPH     SYSCTL_PERIPH_GPIOF
#define BUTTONS_GPIO_BASE       GPIO_PORTF_BASE
#define NUM_BUTTONS             2
#define LEFT_BUTTON             GPIO_PIN_4
#define RIGHT_BUTTON            GPIO_PIN_0
#define ALL_BUTTONS             (LEFT_BUTTON)

extern volatile uint8_t stop_flag;
 uint8_t g_ui8ButtonStates = ALL_BUTTONS;
 extern void task_set_point_change(void);
 volatile uint8_t right =0;
extern void ButtonsInit(void)
{

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIODirModeSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_DIR_MODE_IN);
    MAP_GPIOPadConfigSet(BUTTONS_GPIO_BASE, ALL_BUTTONS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    MAP_IntEnable(46);
    MAP_GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS, GPIO_FALLING_EDGE);
    GPIOIntClear(GPIO_PORTF_BASE, ALL_BUTTONS);
    GPIOIntEnable(GPIO_PORTF_BASE, ALL_BUTTONS);
    g_ui8ButtonStates = GPIOPinRead(BUTTONS_GPIO_BASE, ALL_BUTTONS);
}


extern void buttons_handler(void)
{
    uint8_t pin_mask = GPIOIntStatus(GPIO_PORTF_BASE, true) & 0xFF;
    GPIOIntClear(GPIO_PORTF_BASE, pin_mask);
    MAP_GPIOPinWrite(GPIO_PORTF_BASE,8, 8);
   // TcpIp_Close(0, 0);
    led_on(green);
    task_set_point_change();
    if (right ==1)
        {
        right =0;
        }
    else
    {
        right =1 ;
    }
    stop_flag =1;

}


