#pragma once

// Full list of options is at https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_ARMGCC  // Build env: ARM GCC + Newlib C library
#define MG_TLS MG_TLS_BUILTIN   // MG_TLS_NONE, MG_TLS_MBED, MG_TLS_OPENSSL, MG_TLS_CUSTOM
#define MG_ENABLE_TCPIP 1       // Enable built-in TCP/IP stack

#define MG_ENABLE_CUSTOM_MILLIS 1     // Let user implement mg_millis()
// #define MG_ENABLE_CUSTOM_RANDOM 1  // Let user implement mg_random()
// #define MG_ENABLE_PACKED_FS 1      // Enable packed filesystem, mg_packed_fs()

// Choose one of the following drivers:
// -----------------------------------
// #define MG_ENABLE_DRIVER_CMSIS 1
// #define MG_ENABLE_DRIVER_CYW 1
// #define MG_ENABLE_DRIVER_IMXRT10 1
// #define MG_ENABLE_DRIVER_IMXRT11 1
// #define MG_ENABLE_DRIVER_MCXN 1
// #define MG_ENABLE_DRIVER_PICO 1
// #define MG_ENABLE_DRIVER_RA6 1
// #define MG_ENABLE_DRIVER_RA8 1
// #define MG_ENABLE_DRIVER_RW612 1
// #define MG_ENABLE_DRIVER_SAME54 1
// #define MG_ENABLE_DRIVER_STM32F 1
// #define MG_ENABLE_DRIVER_STM32H 1
// #define MG_ENABLE_DRIVER_TM4C 1
// #define MG_ENABLE_DRIVER_TMS570 1
// #define MG_ENABLE_DRIVER_XMC 1
// #define MG_ENABLE_DRIVER_XMC7 1
#error "Choose network driver in mongoose_config.h"

// Driver options:
// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 4
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// Choose OTA implementation:
// --------------------------
// #define MG_OTA_CH32V307 1
// #define MG_OTA_CUSTOM 1
// #define MG_OTA_ESP32 1
// #define MG_OTA_FLASH 1
// #define MG_OTA_FRDM 1
// #define MG_OTA_MCXN 1
// #define MG_OTA_NONE 1
// #define MG_OTA_PICOSDK 1
// #define MG_OTA_RT1020 1
// #define MG_OTA_RT1050 1
// #define MG_OTA_RT1060 1
// #define MG_OTA_RT1064 1
// #define MG_OTA_RT1170 1
// #define MG_OTA_STM32F 1
// #define MG_OTA_STM32H5 1
// #define MG_OTA_STM32H7 1
// #define MG_OTA_U2A 1
#error "Choose OTA implementation in mongoose_config.h"

// IP and MAC configuration
// ------------------------
// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)
//
// Construct MAC address
#define MG_SET_MAC_ADDRESS(mac) \
  do {                          \
    mac[0] = 2;                 \
    mac[1] = 3;                 \
    mac[2] = 4;                 \
    mac[3] = 5;                 \
    mac[4] = 6;                 \
    mac[5] = 0xef;              \
  } while (0)
