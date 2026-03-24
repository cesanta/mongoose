#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_PICOSDK
#define MG_OTA MG_OTA_PICOSDK

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_PICO_W 1
#define MG_ENABLE_PACKED_FS 1

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// MAC address is read from OTP by the driver

// Set your Wi-Fi configuration
/* Using a build-time constant:
#define MG_SET_WIFI_CONFIG(data) do { 					                \
        ((struct mg_tcpip_driver_pico_w_data *)data)->ssid = "this"; 	\
        ((struct mg_tcpip_driver_pico_w_data *)data)->pass = "that"; 	\
        } while (0)
*/
// Using a custom function:
extern void main_setconfig(void *data);
#define MG_SET_WIFI_CONFIG(data) main_setconfig(data)
