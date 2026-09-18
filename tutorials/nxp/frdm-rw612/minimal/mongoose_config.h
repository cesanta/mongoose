#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_RW612 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_OTA MG_OTA_RW612

// #define MG_DRIVER_MDC_CR 51   // RMII MDC clock divider
// #define MG_DRIVER_MDC_HOLDTIME 3 // RMII MDC Holdtime (valid values [0-7])
// #define MG_TCPIP_PHY_ADDR 2  // PHY address

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// Set custom MAC address. By default, it is randomly generated
// Using a build-time constant:
// #define MG_SET_MAC_ADDRESS(mac) do { mac[0] = 2; mac[1] = 3; mac[2] = 4; mac[3] = 5; mac[4] = 6; mac[5] = 7; } while (0)
//
// Using custom function:
// extern void my_function(unsigned char *mac);
// #define MG_SET_MAC_ADDRESS(mac) my_function(mac)
