/*
 * interface.c
 *
 *  Created on: Apr 27, 2021
 *      Author: Lenovo PC
 */
#include <stdint.h>
#include <stdbool.h>
#include "utils/lwiplib.h"
#include "ports/enc28j60.h"
#include "lwip_int.h"
#include <stdint.h>
#include "../ports/config.h"
#include "../ports/timer.h"
#include "../ports/led.h"
#include "../src/include/lwip/udp.h"
#include "string.h"
#include "../src/include/lwip/def.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "stdint.h"
 struct netif g_sNetIF;

 void InitConsole(void);
 extern err_t enc_low_level_output(struct netif *netif, struct pbuf *p );
 extern err_t EthIf_ProvideTxBuffer(struct netif *netif, struct pbuf *BufPtr);
// err_t TCP_connection_establshe(void *pvArg, struct tcp_pcb *psPcb, err_t iErr);
 extern void ButtonsInit(void);
 struct tcp_pcb * psTCP ;
 uint8_t Udp_buffer_RX [1800]={0};
 uint8_t Udp_buffer_TX [1800]={0};
 uint8_t TCP_buffer_RX [1800]={0};
 uint8_t TCP_buffer_TX [1800]={0};

 uint8_t volatile connection_established =0;
 extern uint8_t buffer_socket_1[100] ;
 extern uint8_t buffers_sizes[10] ;


 extern uint8_t encoder_reading_buffer[100]  ;
 extern uint8_t set_point_buffer[10];

 extern struct netif g_sNetIF;
 extern struct udp_pcb * Udp_ptr_Set_point  = NULL;
 extern struct udp_pcb * Udp_ptr_encoder_reading  = NULL;

   uint8_t buffer[100]={5};
 #define g_ui32SysClock_t          16000000
 #define SYSTEM_TICK_MS          10
 #define SYSTEM_TICK_S           100

 //*****************************************************************************
 //
 // The local time for the lwIP Library Abstraction layer, used to support the
 // Host and lwIP periodic callback functions.
 //
 //*****************************************************************************
 #if NO_SYS
 uint32_t g_ui32LocalTimer = 0;
 #endif
 //*****************************************************************************
 //
 // The local time when the TCP timer was last serviced.
 //
 //*****************************************************************************
 #if NO_SYS
 static uint32_t g_ui32TCPTimer = 0;
 #endif

 //*****************************************************************************
 //
 // The local time when the HOST timer was last serviced.
 //
 //*****************************************************************************
 #if NO_SYS && HOST_TMR_INTERVAL
 static uint32_t g_ui32HostTimer = 0;
 #endif

 //*****************************************************************************
 //
 // The local time when the ARP timer was last serviced.
 //
 //*****************************************************************************
 #if NO_SYS && LWIP_ARP
 static uint32_t g_ui32ARPTimer = 0;
 #endif
#if NO_SYS && LWIP_DNS
static uint32_t g_ui32DNSTimer = 0;
#endif
//*****************************************************************************
//
// The default IP address acquisition mode.
//
//*****************************************************************************
static uint32_t g_ui32IPMode = IPADDR_USE_STATIC;

//*****************************************************************************
//
// The IP address to be used.  This is used during the initialization of the
// stack and when the interface configuration is changed.
//
//*****************************************************************************
//static uint32_t g_ui32IPAddr;
 uint32_t g_ui32IPAddr;
//*****************************************************************************
//
// The netmask to be used.  This is used during the initialization of the stack
// and when the interface configuration is changed.
//
//*****************************************************************************
//static uint32_t g_ui32NetMask;
  uint32_t g_ui32NetMask;

//*****************************************************************************
//
// The gateway address to be used.  This is used during the initialization of
// the stack and when the interface configuration is changed.
//
//*****************************************************************************
//static uint32_t g_ui32GWAddr;
  uint32_t g_ui32GWAddr;

//*****************************************************************************
//
// Completes the initialization of lwIP.  This is directly called when not
// using a RTOS and provided as a callback to the TCP/IP thread when using a
// RTOS.
//
//*****************************************************************************
void
lwIPPrivateInit(struct netif *netif, uint8_t controller_id ,  ip_addr_t *ip_addr, ip_addr_t *net_mask,ip_addr_t *gw_addr)
{
//    struct ip_addr ip_addr;
//    struct ip_addr net_mask;
//    struct ip_addr gw_addr;

    //
    // If not using a RTOS, initialize the lwIP stack.
    //
#if NO_SYS
    lwip_init();
#endif

    //
    // If using a RTOS, create a queue (to be used as a semaphore) to signal
    // the Ethernet interrupt task from the Ethernet interrupt handler.
    //
#if !NO_SYS
#if RTOS_FREERTOS
    g_pInterrupt = xQueueCreate(1, sizeof(void *));
#endif
#endif

    //
    // If using a RTOS, create the Ethernet interrupt task.
    //
#if !NO_SYS
#if RTOS_FREERTOS
    xTaskCreate(lwIPInterruptTask, (signed portCHAR *)"eth_int",
                STACKSIZE_LWIPINTTASK, 0, tskIDLE_PRIORITY + 1,
                0);
#endif
#endif

    //
    // Setup the network address values.
    //
    if(g_ui32IPMode == IPADDR_USE_STATIC)
    {
        ip_addr->addr = htonl(ip_addr->addr);
        net_mask->addr = htonl(net_mask->addr);
        gw_addr->addr = htonl(gw_addr->addr);
    }
    else
    {
        ip_addr->addr = 0;
        net_mask->addr = 0;
        gw_addr->addr = 0;
    }
    //
    // Create, configure and add the Ethernet controller interface with
    // default settings.  ip_input should be used to send packets directly to
    // the stack when not using a RTOS and tcpip_input should be used to send
    // packets to the TCP/IP thread's queue when using a RTOS.
    //
#if NO_SYS
    netif_add(netif, ip_addr, net_mask, gw_addr, NULL, NULL,ip_input);
    netif->flags = (netif->flags ) | (NETIF_FLAG_ETHARP);
#else
    netif_add(&g_sNetIF, &ip_addr, &net_mask, &gw_addr, NULL, tivaif_init,
              tcpip_input);
#endif
    netif_set_default(netif);
    //
    // Bring the interface up.
    //
    netif_set_up(netif);
    //
    // Setup a timeout for the host timer callback function if using a RTOS.
    //
#if !NO_SYS && HOST_TMR_INTERVAL
    sys_timeout(HOST_TMR_INTERVAL, lwIPPrivateHostTimer, NULL);
#endif

    //
    // Setup a timeout for the link detect callback function if using a RTOS.
    //
#if !NO_SYS && (LWIP_AUTOIP || LWIP_DHCP)
    sys_timeout(LINK_TMR_INTERVAL, lwIPPrivateLinkTimer, NULL);
#endif
}



//*****************************************************************************
//
// This function services all of the lwIP periodic timers, including TCP and
// Host timers.  This should be called from the lwIP context, which may be
// the Ethernet interrupt (in the case of a non-RTOS system) or the lwIP
// thread, in the event that an RTOS is used.
//
//*****************************************************************************
#if NO_SYS
void lwIPServiceTimers(void)
{
    //
    // Service the host timer.
    //
#if HOST_TMR_INTERVAL
    if((g_ui32LocalTimer - g_ui32HostTimer) >= HOST_TMR_INTERVAL)
    {
        g_ui32HostTimer = g_ui32LocalTimer;
        //lwIPHostTimerHandler();
    }
#endif

    //
    // Service the ARP timer.
    //
#if LWIP_ARP
    if((g_ui32LocalTimer - g_ui32ARPTimer) >= ARP_TMR_INTERVAL)
    {
        g_ui32ARPTimer = g_ui32LocalTimer;
        etharp_tmr();
    }
#endif

    //
    // Service the TCP timer.
    //
#if LWIP_TCP
    if((g_ui32LocalTimer - g_ui32TCPTimer) >= TCP_TMR_INTERVAL)
    {
        g_ui32TCPTimer = g_ui32LocalTimer;
        tcp_tmr();
    }
#endif

    //
    // Service the AutoIP timer.
    //
#if LWIP_AUTOIP
    if((g_ui32LocalTimer - g_ui32AutoIPTimer) >= AUTOIP_TMR_INTERVAL)
    {
        g_ui32AutoIPTimer = g_ui32LocalTimer;
        autoip_tmr();
    }
#endif

    //
    // Service the DCHP Coarse Timer.
    //
#if LWIP_DHCP
    if((g_ui32LocalTimer - g_ui32DHCPCoarseTimer) >= DHCP_COARSE_TIMER_MSECS)
    {
        g_ui32DHCPCoarseTimer = g_ui32LocalTimer;
        dhcp_coarse_tmr();
    }
#endif

    //
    // Service the DCHP Fine Timer.
    //
#if LWIP_DHCP
    if((g_ui32LocalTimer - g_ui32DHCPFineTimer) >= DHCP_FINE_TIMER_MSECS)
    {
        g_ui32DHCPFineTimer = g_ui32LocalTimer;
        dhcp_fine_tmr();
    }
#endif

    //
    // Service the IP Reassembly Timer
    //
#if IP_REASSEMBLY
    if((g_ui32LocalTimer - g_ui32IPReassemblyTimer) >= IP_TMR_INTERVAL)
    {
        g_ui32IPReassemblyTimer = g_ui32LocalTimer;
        ip_reass_tmr();
    }
#endif

    //
    // Service the IGMP Timer
    //
#if LWIP_IGMP
    if((g_ui32LocalTimer - g_ui32IGMPTimer) >= IGMP_TMR_INTERVAL)
    {
        g_ui32IGMPTimer = g_ui32LocalTimer;
        igmp_tmr();
    }
#endif

    //
    // Service the DNS Timer
    //
#if LWIP_DNS
    if((g_ui32LocalTimer - g_ui32DNSTimer) >= DNS_TMR_INTERVAL)
    {
        g_ui32DNSTimer = g_ui32LocalTimer;
        dns_tmr();
    }
#endif

    //
    // Service the link timer.
    //
#if LWIP_AUTOIP || LWIP_DHCP
    if((g_ui32LocalTimer - g_ui32LinkTimer) >= LINK_TMR_INTERVAL)
    {
        g_ui32LinkTimer = g_ui32LocalTimer;
        lwIPLinkDetect();
    }
#endif
}
#endif


//*****************************************************************************
//
//! Handles periodic timer events for the lwIP TCP/IP stack.
//!
//! \param ui32TimeMS is the incremental time for this periodic interrupt.
//!
//! This function will update the local timer by the value in \e ui32TimeMS.
//! If the system is configured for use without an RTOS, an Ethernet interrupt
//! will be triggered to allow the lwIP periodic timers to be serviced in the
//! Ethernet interrupt.
//!
//! \return None.
//
//*****************************************************************************
#if NO_SYS
void
lwIPTimer(uint32_t ui32TimeMS)
{
    //
    // Increment the lwIP Ethernet timer.
    //
    g_ui32LocalTimer += ui32TimeMS;

}
#endif


void ECU_int(struct netif *netif, uint8_t controller_id ,  ip_addr_t *ipaddr, ip_addr_t *netmask,ip_addr_t *gw)
{
        gpio_comm_init();
        spi_init();
        //ButtonsInit();
        enc_init(mac);
        led_on(yellow);
        timer_start();
        //netif->ctr_ID =controller_id;
        netif->hwaddr[0] = mac[0];
        netif->hwaddr[1] = mac[1];
        netif->hwaddr[2] = mac[2];
        netif->hwaddr[3] = mac[3];
        netif->hwaddr[4] = mac[4];
        netif->hwaddr[5] = mac[5];
        netif->output = etharp_output;
        netif->linkoutput = EthIf_ProvideTxBuffer;
        LWIP_stack_int(netif, controller_id ,  ipaddr, netmask,gw);
        InitConsole();
        UARTprintf("55");
        led_off();
        led_on(yellow);
}

void SimpleDelay_2(void)
{
    SysCtlDelay(16000000 / 3);
}




void UDP_recv_set_point(void *arg, struct udp_pcb *pcb, struct pbuf *p,ip_addr_t *addr, u16_t port)
{
    led_off();
#if ECU1
    led_on(red);
#endif
#if ECU2
    led_on(blue);
#endif
    memcpy( set_point_buffer  , (u8_t *)p->payload,   p->len);
    set_point_buffer[0]=p->len;
    pbuf_free(p);           // Free the memory space allocated for this receive.
    p =NULL;                // to avoid dangling pointer
    connection_established =1;
}

void UDP_recv_encoder_reading(void *arg, struct udp_pcb *pcb, struct pbuf *p,ip_addr_t *addr, u16_t port)
{
    led_off();
#if ECU1
    led_on(red);
#endif
#if ECU2
    led_on(blue);
#endif
    memcpy( encoder_reading_buffer  , (u8_t *)p->payload,   p->len);
    encoder_reading_buffer[0]=p->len;
    pbuf_free(p);           // Free the memory space allocated for this receive.
    p =NULL;                // to avoid dangling pointer
    connection_established =1;
}



void LWIP_stack_int(struct netif *netif, uint8_t controller_id ,  ip_addr_t *ipaddr, ip_addr_t *netmask,ip_addr_t *gw)
{
    lwIPPrivateInit(netif, controller_id , ipaddr, netmask,gw);
}

void
InitConsole(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Enable UART0 so that we can configure the clock.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Select the alternate (UART) function for these pins.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}


//void UDP_CONNECT( TcpIp_SocketIdType SocketId  , TcpIp_SockAddrType * RemoteAddrPtr_udp)
//{
//
//udp_connect( (struct udp_pcb *) sockets_list[SocketId].ptr_connection  , (ip_addr_t *) (&RemoteAddrPtr_udp->TcpIp_SockAddrInetType_t.addr) ,  RemoteAddrPtr_udp->TcpIp_SockAddrInetType_t.port );
//
//}

void send_udp_encoder_reading(const uint8_t* data, int length)
{
    //if (connection_established ==1)
    //{
         struct pbuf *p;
         if (   (p= pbuf_alloc(PBUF_udp, length,PBUF_RAM) )== NULL) { return ERR_MEM;}  /* not enough space */           // Length of data only and payload point to the start of data
         //static int c =5;
         memset(data,0xaa,1);
         memset(data+1,0xbb,1);
         memset(data+2,0xcc,1);
         memset(data+3,0xdd,1);
         //uint32_t x = 0xffaabbcc;
         memcpy(p->payload,(u8_t*)data ,length);
         err_t n= udp_send(Udp_ptr_encoder_reading, p);
         pbuf_free(p);
  //  }

}

void send_udp_set_point(const uint8_t* data, int length)
{
    //if (connection_established ==1)
    //{
         struct pbuf *p;
         if (   (p= pbuf_alloc(PBUF_udp, length,PBUF_RAM) )== NULL) { return ERR_MEM;}  /* not enough space */           // Length of data only and payload point to the start of data
         //static int c =5;
         memset(data,0xaa,1);
         memset(data+1,0xbb,1);
         memset(data+2,0xcc,1);
         memset(data+3,0xdd,1);
         //uint32_t x = 0xffaabbcc;
         memcpy(p->payload,(u8_t*)data ,length);
         err_t n= udp_send(Udp_ptr_Set_point, p);
         pbuf_free(p);
  //  }

}

void inti(void)
{

}
