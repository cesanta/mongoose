#pragma once

#if MG_ARCH == MG_ARCH_CUBE

#include <ctype.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Cube-generated header, includes ST Cube HAL
// NOTE: use angle brackets to prevent amalgamator ditching it
#include <main.h>

#ifndef MG_PATH_MAX
#define MG_PATH_MAX 100
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 0
#endif


#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 1  // Enable built-in TCP/IP stack
#endif

#if MG_ENABLE_TCPIP && !defined(MG_ENABLE_DRIVER_STM32F) && \
    !defined(MG_ENABLE_DRIVER_STM32H) && !defined(MG_ENABLE_DRIVER_STM32N) && \
    !defined(MG_ENABLE_DRIVER_CYW) && !defined(MG_ENABLE_DRIVER_CYW_SDIO) && \
    !defined(MG_ENABLE_DRIVER_ST67W6)
#if defined(STM32F1) || defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
#define MG_ENABLE_DRIVER_STM32F 1
#elif defined(STM32H5) || defined(STM32H7)
#define MG_ENABLE_DRIVER_STM32H 1
#elif defined(STM32N6)
#define MG_ENABLE_DRIVER_STM32N 1
#else
#error Select a driver in mongoose_config.h
#endif
#endif

#ifndef MG_TLS
#define MG_TLS MG_TLS_BUILTIN
#endif

#if !defined(MG_OTA) && defined(STM32F1) || defined(STM32F2) || \
    defined(STM32F4) || defined(STM32F7)
#define MG_OTA MG_OTA_STM32F
#elif !defined(MG_OTA) && defined(STM32H5)
#define MG_OTA MG_OTA_STM32H5
#elif !defined(MG_OTA) && defined(STM32H7)
#define MG_OTA MG_OTA_STM32H7
#endif

// OTA rollback timer via IWDG. Fixed 10 s hang-detection window (prescaler
// /1024, LSI ~32 kHz: reload = 10 * 32000 / 1024 = 312). mg_ota_poll() feeds
// the watchdog every 500 ms while in MG_OTA_TESTING state; if the event loop
// hangs for 10 s, the IWDG fires and the device rolls back.
// STM32H7 names the peripheral IWDG1; all others use IWDG.
// The total evaluation period is set by MG_OTA_ROLLBACK_TIMEOUT_SECONDS.
// STM32H7 names the watchdog peripheral IWDG1; all other families use IWDG.
#if defined(IWDG1) && !defined(IWDG)
#define MG_IWDG IWDG1
#else
#define MG_IWDG IWDG
#endif
// PR=6 → /256 prescaler, RLR=1250 → ~10 s at LSI 32 kHz.
// Works on F4/F7/H5/H7; uses direct register writes so it is safe before HAL_Init().
#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START()                                          \
  do {                                                                         \
    MG_IWDG->KR = 0xCCCCU;                              /* start + LSI  */    \
    MG_IWDG->KR = 0x5555U;                              /* unlock PR/RLR */   \
    MG_IWDG->PR = 6U;                                   /* /256         */    \
    MG_IWDG->RLR = 1250U;                               /* 10 s         */    \
    while (MG_IWDG->SR & (IWDG_SR_PVU | IWDG_SR_RVU)) (void) 0;              \
    MG_IWDG->KR = 0xAAAAU;                              /* reload       */    \
  } while (0)
#endif

// Feed (reload) the IWDG. Called from mg_ota_poll() every 500 ms.
#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() (MG_IWDG->KR = 0xAAAAU)
#endif

// OTA state in a backup register that survives warm resets but clears on POR.
// STATE_GET reads work without any setup. STATE_SET enables the RTC APB clock
// and unlocks backup domain write access via direct register writes, so it is
// safe to call from any context (before or after HAL_Init).
// H5 (TAMP->BKP0R), H7 (RTC->BKP0R), F4/F7 (RTC->BKP0R).
#ifndef MG_OTA_STATE_GET
#if defined(PWR_DBPCR_DBP)
#define MG_OTA_DBP_ENABLE() (PWR->DBPCR |= PWR_DBPCR_DBP)
#elif defined(PWR_CR1_DBP)
#define MG_OTA_DBP_ENABLE() (PWR->CR1 |= PWR_CR1_DBP)
#else
#define MG_OTA_DBP_ENABLE() (PWR->CR |= PWR_CR_DBP)
#endif

#if defined(TAMP)
#define MG_OTA_STATE_GET() (TAMP->BKP0R)
#define MG_OTA_STATE_SET(v)                                           \
  (RCC->APB3ENR |= RCC_APB3ENR_RTCAPBEN, MG_OTA_DBP_ENABLE(),        \
   (TAMP->BKP0R = (uint32_t) (v)))
#elif defined(RCC_APB4ENR_RTCAPBEN)
#define MG_OTA_STATE_GET() (RTC->BKP0R)
#define MG_OTA_STATE_SET(v)                                           \
  (RCC->APB4ENR |= RCC_APB4ENR_RTCAPBEN, MG_OTA_DBP_ENABLE(),        \
   (RTC->BKP0R = (uint32_t) (v)))
#else
#define MG_OTA_STATE_GET() (RTC->BKP0R)
#define MG_OTA_STATE_SET(v)                                           \
  (RCC->APB1ENR |= RCC_APB1ENR_PWREN, MG_OTA_DBP_ENABLE(),           \
   (RTC->BKP0R = (uint32_t) (v)))
#endif
#endif

// use HAL-defined execute-in-ram section
#ifndef MG_IRAM
#define MG_IRAM __attribute__((section(".RamFunc")))
#endif

#ifndef MG_ETH_RAM
#define MG_ETH_RAM __attribute__((section(".eth_ram")))
#endif

#ifndef HAL_ICACHE_MODULE_ENABLED
#define HAL_ICACHE_IsEnabled() 0
#define HAL_ICACHE_Enable() (void) 0
#define HAL_ICACHE_Disable() (void) 0
#endif

#ifndef MG_SET_MAC_ADDRESS
// Construct MAC address from UUID
#define MGUID ((uint32_t *) UID_BASE)  // Unique 96-bit chip ID
#define MG_SET_MAC_ADDRESS(mac)                                   \
  do {                                                            \
    int icache_enabled_ = HAL_ICACHE_IsEnabled();                 \
    if (icache_enabled_) HAL_ICACHE_Disable();                    \
    mac[0] = 42;                                                  \
    mac[1] = ((MGUID[0] >> 0) & 255) ^ ((MGUID[2] >> 19) & 255);  \
    mac[2] = ((MGUID[0] >> 10) & 255) ^ ((MGUID[1] >> 10) & 255); \
    mac[3] = (MGUID[0] >> 19) & 255;                              \
    mac[4] = ((MGUID[1] >> 0) & 255) ^ ((MGUID[2] >> 10) & 255);  \
    mac[5] = ((MGUID[2] >> 0) & 255) ^ ((MGUID[1] >> 19) & 255);  \
    if (icache_enabled_) HAL_ICACHE_Enable();                     \
  } while (0)
#endif

#endif
