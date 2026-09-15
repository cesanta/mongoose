#pragma once
#include "hal.h"

// See https://mongoose.ws/docs/getting-started/build-options/
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_TLS MG_TLS_BUILTIN
#define MG_OTA MG_OTA_STM32H7

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_DRIVER_STM32H 1

// Place Ethernet driver's buffers into the DMA-accessible RAM, see link.ld
#define MG_ETH_RAM __attribute__((section(".eth_ram")))

// Construct MAC address from the MCU unique ID
#define MGUID ((uint32_t *) UID_BASE)
#define MG_SET_MAC_ADDRESS(mac)      \
  do {                               \
    mac[0] = 2;                      \
    mac[1] = MGUID[0] & 255;         \
    mac[2] = (MGUID[0] >> 10) & 255; \
    mac[3] = (MGUID[0] >> 19) & 255; \
    mac[4] = MGUID[1] & 255;         \
    mac[5] = MGUID[2] & 255;         \
  } while (0)

// Crash report support. Health record lives in the .mg_health region, see link.ld
#define MG_HEALTH_RAM __attribute__((section(".mg_health")))

// mdash.net device management service support
#define MG_ENABLE_MDASH 1
#define MG_MDASH_KEY "u8eyWvBmkc6gw9991z1SQ8Lg"  // Set this - mDash device token
