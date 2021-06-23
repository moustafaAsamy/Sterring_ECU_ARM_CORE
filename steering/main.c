

#include "motor.h"
#include "encoder.h"
#include "stdint.h"
#include "interface/lwip_int.h"
#include "application.h"


int main()
{
    ecoder_init();
    motor_init();

    Remote_ip_addr.addr=htonl(Remote_ip_addr.addr);
    ECU_int(&g_sNetIF, 0 ,  &ip_addr, &net_mask, &gw_addr);

    uint32_t i =0;
    for(i =0;i<65000;i++)
    {

    }

    Udp_ptr_steering_encoder_reading = udp_new();

    Udp_ptr_steering_set_point       = udp_new();
    err_t result1 =  udp_bind   (  Udp_ptr_steering_set_point  , &ip_addr       ,   port_local_steering_set_point     );
    err_t result2 =  udp_connect(  Udp_ptr_steering_set_point  , &Remote_ip_addr,   port_remote_steering_set_point    );
    Udp_ptr_steering_set_point->recv = UDP_recv_steering_set_point ;                                      ;

    Udp_ptr_steering_encoder_reading = udp_new();
    err_t result3 =  udp_bind   (  Udp_ptr_steering_encoder_reading  , &ip_addr       ,   port_local_steering_encoder_reading     );
    err_t result4 =  udp_connect(  Udp_ptr_steering_encoder_reading  , &Remote_ip_addr,   port_remote_steering_encoder_reading    );
    Udp_ptr_steering_encoder_reading->recv = NULL ;

    timer_start();
    while(1)
    {

    }
}







