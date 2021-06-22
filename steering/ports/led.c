/*
 * led.c
 *
 *  Created on: Oct 23, 2019
 *      Author: lenovo
 */


#include "led.h"



void led_on(uint8_t led)
{
    MAP_GPIOPinWrite(GPIO_PORTF_BASE,led, led);
    //MAP_GPIOPinWrite(GPIO_PORTF_BASE,~led, ~led);
}
void led_off(void)
{
    MAP_GPIOPinWrite(GPIO_PORTF_BASE,all, 0);
}


void led_init(void)
{
    GPIODirModeSet(GPIO_PORTF_BASE, all, GPIO_DIR_MODE_OUT);
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, all, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
