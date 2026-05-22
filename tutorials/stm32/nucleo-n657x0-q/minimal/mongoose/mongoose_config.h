#pragma once

// See https://mongoose.ws/docs/getting-started/build-options/
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN
// #define MG_OTA MG_OTA_STM32H7

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_STM32N 1
#define MG_ENABLE_PACKED_FS 1

// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 5
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)     // IP
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)      // Gateway
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Netmask

// Construct MAC address from the MCU unique ID
#define MGUID ((uint32_t *) 0x56009014UL)  // Unique 96-bit chip ID
#define MG_SET_MAC_ADDRESS(mac)      \
  do {                               \
    mac[0] = 2;                      \
    mac[1] = MGUID[0] & 255;         \
    mac[2] = (MGUID[0] >> 10) & 255; \
    mac[3] = (MGUID[0] >> 19) & 255; \
    mac[4] = MGUID[1] & 255;         \
    mac[5] = MGUID[2] & 255;         \
  } while (0)
