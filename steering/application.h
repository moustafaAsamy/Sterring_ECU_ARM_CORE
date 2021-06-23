/*
 * application.h
 *
 *  Created on: Jun 23, 2021
 *      Author: Lenovo PC
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "interface/lwip_int.h"


void UDP_recv_steering_set_point(void *arg, struct udp_pcb *pcb, struct pbuf *p,ip_addr_t *addr, u16_t port);
void send_udp_steering_encoder_reading(void);

extern ip_addr_t ip_addr           ;
extern ip_addr_t Remote_ip_addr    ;
extern ip_addr_t net_mask          ;
extern ip_addr_t gw_addr           ;


extern u16_t port_local_steering_set_point          ;
extern u16_t port_local_steering_encoder_reading    ;
extern u16_t port_remote_steering_set_point         ;
extern u16_t port_remote_steering_encoder_reading   ;


struct udp_pcb * Udp_ptr_steering_set_point  ;
struct udp_pcb * Udp_ptr_steering_encoder_reading  ;

extern  struct netif g_sNetIF;


#endif /* APPLICATION_H_ */
