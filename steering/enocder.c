/*
 * enocder.c
 *
 *  Created on: Jun 16, 2021
 *      Author: Lenovo PC
 */

#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "encoder.h"

int turn = 0;
volatile long counter = 0;
uint16_t  dec_position = 0;
int old_read = 0;

uint16_t test_encoder = 0;
extern int ecoder_read(void)
{

    uint8_t c= GPIOPinRead(GPIO_PORTC_BASE, 0Xf0);
    int32_t bb= GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_6);
    int32_t a = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3);

    uint16_t encoder = 0;
    uint16_t b =0;
    uint8_t pin_state[10]={0};

    b= !( GPIO_PIN_3 && (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3))); //encoder_pin_0
    pin_state[0]=b;
    b=  !(GPIO_PIN_2 && GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2));//encoder_pin_1
    pin_state[1]=b;
    b =  !(GPIO_PIN_3 && GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3));//encoder_pin_2
    pin_state[2]=b;
    b =  !(GPIO_PIN_2 && GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2));//encoder_pin_3
    pin_state[3]=b;
    b = !(GPIO_PIN_7 &&  GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_7));//encoder_pin_4
    pin_state[4]=b;
    b=   !(GPIO_PIN_6 && GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_6));//encoder_pin_5
    pin_state[5]=b;
    b =  !(GPIO_PIN_7 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7));//encoder_pin_6
    pin_state[6]=b;
    b =  !(GPIO_PIN_6 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6));//encoder_pin_7
    pin_state[7]=b;
    b =  !(GPIO_PIN_5 &&  GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5));//encoder_pin_8
    pin_state[8]=b;
    b =  !(GPIO_PIN_4 && GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4));//encoder_pin_9
    pin_state[9]=b;
    dec_position = pin_state[9];
    int i =0;
     for (   i = 8; i >= 0; i--)
         {
         dec_position = (dec_position << 1) | (pin_state[i] ^ (dec_position & 0x1));
         }
     test_encoder =dec_position + test_encoder;

     if ((old_read>=900 && old_read<=1023) && (dec_position >= 0 && dec_position <=100) ) turn++; //359 or number of tickes per rev
         if ((old_read>=0 && old_read <=100)  && (dec_position>=900 && dec_position <= 1023)) turn--;
         counter= dec_position-512 + 1024*turn;

         //counter = counter-891 +1024+2000;
//         if(counter > right_max )
//         {
//             counter = right_max;
//         }
//         else if(counter <left_max )
//         {
//             counter = left_max;
//         }
         //counter = counter-891 +1024;
         //wheel_angle = counter/float(1024*4.5)*60;//counter *total steering angle/total wheel angle
         old_read= dec_position;
       //UARTprintf(" mo_reading = %d  \n", counter);
    return  counter;
}

extern void ecoder_init(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIODirModeSet(GPIO_PORTC_BASE, 0Xf0, GPIO_DIR_MODE_IN);
    GPIODirModeSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_6, GPIO_DIR_MODE_IN);
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_DIR_MODE_IN);

    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, 0XF0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7|GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

}
