/////
////
/////**
//// * main.c
//// */
////#include "ros_lib\ros\msg.h"
////#include "ros_lib\ros\Publisher.h"
////#include "ros_lib\ros\msg.h"
////#include "ros_lib\ros\ros.h"
////#include "ros_lib/std_msgs/String.h"
////#include "ros_lib\ros\node_handle.h"
////
////
////
////
////
////
////
////
////
#include "ports/config.h"
#include "interface/lwip_int.h"
////
////
////
////
////
#if ECU2
#include "interface/lwip_int.h"
#include "string.h"
#include "ros_ethernet/EthernetClient.h"

extern void inti(void);
ip_addr_t ip_addr ;
ip_addr_t Remote_ip_addr ={0xc0a8016a} ;
ip_addr_t net_mask = { 0xc0a80000};
ip_addr_t  gw_addr = { 0xc0a82526};
extern struct udp_pcb * Udp_ptr  ;
extern uint8_t buffer[100];
u16_t port_1 =9007 ;
u16_t port_remote =9807;



extern void ECU_int(struct netif *netif, uint8_t controller_id ,  ip_addr_t *ipaddr, ip_addr_t *netmask,ip_addr_t *gw);
extern void send_udp(const uint8_t* data, int length) ;
extern uint8_t volatile connection_established ;


int main()
{
    ip_addr.addr = 0xc0a8017b;
    //ip_addr.addr = 0xc0a80172;remote
    Remote_ip_addr.addr=htonl(Remote_ip_addr.addr);
   ECU_int(&g_sNetIF, 0 ,  &ip_addr, &net_mask, &gw_addr);
    memset(buffer,8,100);
    Udp_ptr = udp_new();
    err_t result1 =  udp_bind(Udp_ptr , &ip_addr ,port_1);
    err_t result2 =  udp_connect(Udp_ptr, &Remote_ip_addr,port_remote);
    Udp_ptr->recv = UDP_recv;

    while(1)
    {

    }
}
#endif

////
////
////
////
////
////
//////lwIPTimer
////
//////gpio_comm_init
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
////
//////extern void send(const uint8_t* data, int length);
//////extern uint8_t buffer[100];
////
////
//// //_c_int00 data
///////ros_lib/std_msgs String.h
//////  HardwareSerial  Serial;
////
//////unsigned long millis();
//////ros::Publisher c;
//////class Msg;
//////ros::NodeHandle nh;
//////std_msgs::String str_msg;
//////ros::Publisher chatter("chatter", &str_msg);
//////char hello[13] = "hello world!";
//////int main(void)
//////{
//////	 nh.initNode();
//////       nh.advertise(chatter);
//////
//////
//////
//////
//////
//////while(1)
//////{
////////    extern bool configured_ ;
//////
//////  str_msg.data = hello;
//////  chatter.publish( &str_msg );
//////  nh.spinOnce();
////////  //delay(1000);
//////}
//////millis
////
//////}
//////HardwareSerial
////
//////Serial
//////ros::normalizeSecNSec
//// ///HardwareSerial::begin
////
//////ros::normalizeSecNSec
////
////
////unsigned long millis()
////{
////
////}
////
////
////#include <driverlib/rom_map.h>
////#include <stdbool.h>
////   #include <stdint.h>
////   // TivaC specific includes
////   extern "C"
////   {
////     #include <driverlib/sysctl.h>
////     #include <driverlib/gpio.h>
////   }
////   // ROS includes
//////   #include <ros.h>
////  // #include <std_msgs/String.h>
////
////   // ROS nodehandle
////   ros::NodeHandle nh;
////
////   std_msgs::String str_msg;
////   ros::Publisher chatter("chatter", &str_msg);
////   char hello[13] = "Hello world!";
////
//////   int main(void)
//////   {
////     // TivaC application specific code
////     //FPUEnable();
////     //MAP_FPULazyStackingEnable();
////     // TivaC system clock configuration. Set to 80MHz.
////   // MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
////
////     // ROS nodehandle initialization and topic registration
//////     nh.initNode();
//////     nh.advertise(chatter);
////
//////     while (1)
//////     {
//////       // Publish message to be transmitted.
//////       str_msg.data = hello;
//////       chatter.publish(&str_msg);
//////
//////       // Handle all communications and callbacks.
//////       nh.spinOnce();
//////
//////       // Delay for a bit.
//////       nh.getHardware()->delay(100);
//////     }
//// //   }
////
////
//////#include <stdbool.h>
//////   2 #include <stdint.h>
//////   3 // TivaC specific includes
//////   4 extern "C"
//////   5 {
//////   6   #include <driverlib/sysctl.h>
//////   7   #include <driverlib/gpio.h>
//////   8   #include "buttons.h"
//////   9 }
//////  10 // ROS includes
//////  11 #include <ros.h>
//////  12 // Custom ROS message
//////  13 #include "rosserial_tivac_tutorials/Buttons.h"
//////  14
//////  15 // ROS nodehandle
//////  16 ros::NodeHandle nh;
//////  17
//////  18 rosserial_tivac_tutorials::Buttons button_msg;
//////  19 ros::Publisher button_publisher("button_state", &button_msg);
//////  20
//////  21 int main(void)
//////  22 {
//////  23   // TivaC application specific code
//////  24   MAP_FPUEnable();
//////  25   MAP_FPULazyStackingEnable();
//////  26   // TivaC system clock configuration. Set to 80MHz.
//////  27   MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
//////  28
//////  29   uint8_t button_debounced_delta;
//////  30   uint8_t button_raw_state;
//////  31   ButtonsInit();
//////  32
//////  33   // ROS nodehandle initialization and topic registration
//////  34   nh.initNode();
//////  35   nh.advertise(button_publisher);
//////  36
//////  37   while (1)
//////  38   {
//////  39     uint8_t button_debounced_state = ButtonsPoll(&button_debounced_delta, &button_raw_state);
//////  40     // Publish message to be transmitted.
//////  41     button_msg.sw1.data = button_debounced_state & LEFT_BUTTON;
//////  42     button_msg.sw2.data = button_debounced_state & RIGHT_BUTTON;
//////  43     button_publisher.publish(&button_msg);
//////  44
//////  45     // Handle all communications and callbacks.
//////  46     nh.spinOnce();
//////  47
//////  48     // Delay for a bit.
//////  49     nh.getHardware()->delay(100);
//////  50   }
//////  51 }
//
//
//
//
////#include "ros_ethernet/Tivac_Udp_Hardware.h"
//#include "ros_lib/ros/ros.h"
//#include "ros_lib/std_msgs/String.h"
////#include "interface/lwip_int.h"
////extern ip_addr_t  ipaddr,gw,netmask;
//
//ros::NodeHandle nh;
//std_msgs::String str_msg;
//ros::Publisher chatter("chatter", &str_msg);
//char hello[13] = "hello world!";
//extern void task2(void);
//extern volatile uint32_t main_function ;
//int main (void)
//
//{
//
//
//
//      nh.initNode();
//      nh.advertise(chatter);
//
//      str_msg.data = hello;
//      chatter.publish( &str_msg );
//      while (1)
//      {
//
//          if ((main_function > 70 ) ||(main_function < 90))
//                      {
//                         chatter.publish( &str_msg );
//      //    nh.spinOnce();
//      }
//
//      }
//
//
//}
//
//
//void task2(void)
//{
//    chatter.publish( &str_msg );
//}
