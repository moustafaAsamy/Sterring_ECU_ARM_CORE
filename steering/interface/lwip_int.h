/*
 * lwip_int.h
 *
 *  Created on: Sep 29, 2019
 *      Author: lenovo
 */

#ifndef LWIP_LWIP_INT_H_
#define LWIP_LWIP_INT_H_

#include "utils/lwiplib.h"


extern struct netif   g_sNetIF ;
void ECU_int(struct netif *netif, u8_t controller_id ,  ip_addr_t *ipaddr, ip_addr_t *netmask,ip_addr_t *gw);
void InitConsole(void);
extern void UARTprintf(const char *pcString, ...);



#endif /* LWIP_LWIP_INT_H_ */
