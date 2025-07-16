#pragma once

#define MG_ARCH MG_ARCH_ARMGCC     // Generic ARM GCC
#define MG_TLS MG_TLS_BUILTIN      // MG_TLS_NONE, MG_TLS_MBED, MG_TLS_OPENSSL
#define MG_ENABLE_TCPIP 1          // Enable built-in TCP/IP stack

// #define MG_ENABLE_DRIVER_STM32F 1
// #define MG_ENABLE_DRIVER_STM32H 1
#error "Choose network driver in mongoose_config.h"

// OTA implementation
// #define MG_OTA_STM32F 1
// #define MG_OTA_STM32H5 1
// #define MG_OTA_STM32H7 1

// IP and MAC configuration
// ------------------------
// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)
//
// Construct MAC address from UUID
#include "main.h"
#define MGUID ((uint32_t *) UID_BASE)  // Unique 96-bit chip ID
#define MG_SET_MAC_ADDRESS(mac)      \
  do {                               \
    mac[0] = 2;                      \
    mac[1] = MGUID[0] & 255;         \
    mac[2] = (MGUID[0] >> 10) & 255; \
    mac[3] = (MGUID[0] >> 19) & 255; \
    mac[4] = MGUID[1] & 255;         \
    mac[5] = MGUID[2] & 255;         \
  } while (0)

// #define MG_ENABLE_CUSTOM_CALLOC 1 // Let user define mg_calloc() and mg_free()
// #define MG_ENABLE_CUSTOM_RANDOM 1 // Let user define mg_random()
// #define MG_ENABLE_CUSTOM_MILLIS 1 // Let user define mg_millis()

// Driver options
// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 4
// #define MG_TCPIP_PHY_ADDR 0  // PHY address
