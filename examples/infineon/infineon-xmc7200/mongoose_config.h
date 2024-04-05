#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_NEWLIB

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_DRIVER_XMC7 1
#define MG_IO_SIZE 256
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)

// Set custom MAC address. By default, it is randomly generated
// Using a build-time constant:
// #define MG_SET_MAC_ADDRESS(mac) do { uint8_t buf_[6] = {2,3,4,5,6,7}; memmove(mac, buf_, sizeof(buf_)); } while (0)
//
// Using custom function:
// extern void my_function(unsigned char *mac);
// #define MG_SET_MAC_ADDRESS(mac) my_function(mac)
