/*
 * application.c
 *
 *  Created on: Jun 23, 2021
 *      Author: Lenovo PC
 */

#include "application.h"
#include "ports/led.h"


ip_addr_t ip_addr =         { 0xc0a8017b};
ip_addr_t Remote_ip_addr =  { 0xc0a8016a} ;
ip_addr_t net_mask =        { 0xc0a80000} ;
ip_addr_t gw_addr  =        { 0xc0a82526} ;


u16_t port_local_steering_set_point          = 9001 ;
u16_t port_local_steering_encoder_reading    = 9002 ;
u16_t port_remote_steering_set_point         = 9801 ;
u16_t port_remote_steering_encoder_reading   = 9802 ;



extern int sensor_Reading ;
extern volatile int set__point ;

void UDP_recv_steering_set_point(void *arg, struct udp_pcb *pcb, struct pbuf *p,ip_addr_t *addr, u16_t port)
{
    led_off();
    led_on(blue);
    memcpy( (void*)set__point  , (u8_t *)p->payload ,   p->len);
    pbuf_free(p);                                       // Free the memory space allocated for this receive.
    p =NULL;                                            // to avoid dangling pointer
}


void send_udp_steering_encoder_reading(void)
{
         struct pbuf *p;
         if (   (p= pbuf_alloc(PBUF_udp, 2,PBUF_RAM) )== NULL) { return ERR_MEM;}  /* not enough space */           // Length of data only and payload point to the start of data
         memcpy(p->payload,(void *)sensor_Reading ,2);
         err_t n= udp_send(Udp_ptr_steering_encoder_reading, p);
         pbuf_free(p);
}



