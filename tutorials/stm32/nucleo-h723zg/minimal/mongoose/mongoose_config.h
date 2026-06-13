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
#define MG_ENABLE_PACKED_FS 1
#define MG_ETH_RAM __attribute__((section(".eth_ram")))

// #define MG_DRIVER_MDC_CR 4   // RMII MDC clock divider, from 0 to 5
// #define MG_TCPIP_PHY_ADDR 0  // PHY address

// For static IP configuration, define MG_TCPIP_{IP,MASK,GW}
// By default, those are set to zero, meaning that DHCP is used
//
// #define MG_TCPIP_IP MG_IPV4(192, 168, 0, 10)     // IP
// #define MG_TCPIP_GW MG_IPV4(192, 168, 0, 1)      // Gateway
// #define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Netmask

// OTA rollback timer via IWDG1. PR=7 → /512 on H723 (RM0468 §54.4.2).
// Fixed 10 s hang-detection window: RLR = 10 * 32000 / 512 = 625.
// RM0468 §54.4 sequence: 0xCCCC first (starts LSI), then 0x5555 to unlock,
// write PR+RLR, wait SR=0 (PVU|RVU), then 0xAAAA to reload the counter.
// mg_ota_poll() feeds the watchdog every 500ms while in MG_OTA_TESTING state.
#define MG_OTA_ROLLBACK_TIMER_START()                                           \
  do {                                                                          \
    IWDG1->KR = 0xCCCCU;                               /* start/LSI     */    \
    IWDG1->KR = 0x5555U;                               /* unlock        */    \
    IWDG1->PR = 7U;                                    /* /512          */    \
    IWDG1->RLR = 625U;                                 /* 10 s          */    \
    while (IWDG1->SR & (IWDG_SR_PVU | IWDG_SR_RVU)) (void) 0; /* wait SR=0 */ \
    IWDG1->KR = 0xAAAAU;                               /* reload        */    \
  } while (0)
#define MG_OTA_ROLLBACK_TIMER_FEED() (IWDG1->KR = 0xAAAAU)

// OTA state in RTC backup register 0 (survives all resets, clears on POR).
// Requires hal_backup_domain_init() (called from hal_clock_init) to have run.
#define MG_OTA_STATE_GET()    (RTC->BKP0R)
#define MG_OTA_STATE_SET(v)   (RTC->BKP0R = (uint32_t) (v))

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
