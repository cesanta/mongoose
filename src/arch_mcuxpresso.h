#pragma once

#if MG_ARCH == MG_ARCH_MCUXPRESSO
#define _POSIX_TIMERS

#include <ctype.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// MCUXpresso-generated header, includes the SoC peripheral definitions.
// NOTE: use angle brackets to prevent amalgamator ditching it
#include <fsl_device_registers.h>

#ifndef MG_PATH_MAX
#define MG_PATH_MAX 100
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 0
#endif

#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 1  // Enable built-in TCP/IP stack
#endif

#if MG_ENABLE_TCPIP && !defined(MG_ENABLE_DRIVER_IMXRT10) && \
    !defined(MG_ENABLE_DRIVER_IMXRT11) && !defined(MG_ENABLE_DRIVER_NETC)
#if defined(CPU_MIMXRT1021DAG5A) || defined(CPU_MIMXRT1024DAG5A) ||         \
    defined(CPU_MIMXRT1042XJM5B) || defined(CPU_MIMXRT1052DVL6B) ||         \
    defined(CPU_MIMXRT1062DVL6B) || defined(CPU_MIMXRT1064DVL6B) ||         \
    defined(CPU_MIMXRT1062DVL6A) || defined(CPU_MIMXRT1064DVL6A) ||         \
    defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) || \
    defined(CPU_MIMXRT1062DVL6B_cm7) || defined(_MIMXRT1021_H_) ||          \
    defined(_MIMXRT1024_H_) || defined(_MIMXRT1042_H_) ||                   \
    defined(_MIMXRT1052_H_) || defined(_MIMXRT1062_H_) ||                   \
    defined(_MIMXRT1064_H_)
#define MG_ENABLE_DRIVER_IMXRT10 1
#elif defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1176DVMAA_cm4) || \
    defined(_MIMXRT1176_cm7_H_) || defined(_MIMXRT1176_cm4_H_) ||             \
    defined(_MIMXRT1176_H_)
#define MG_ENABLE_DRIVER_IMXRT11 1
#elif defined(CPU_MIMXRT1186CVJ8C_cm33) || defined(MIMXRT1186_cm33_H_)
#define MG_ENABLE_DRIVER_NETC 1
#else
#error Select an Ethernet driver in mongoose_config.h
#endif
#endif

#ifndef MG_TLS
#define MG_TLS MG_TLS_BUILTIN
#endif

#if !defined(MG_OTA) &&                                              \
    (defined(CPU_MIMXRT1021DAG5A) || defined(CPU_MIMXRT1024DAG5A) || \
     defined(_MIMXRT1021_H_) || defined(_MIMXRT1024_H_))
#define MG_OTA MG_OTA_RT1020
#elif !defined(MG_OTA) && \
    (defined(CPU_MIMXRT1052DVL6B) || defined(_MIMXRT1052_H_))
#define MG_OTA MG_OTA_RT1050
#elif !defined(MG_OTA) &&                                                    \
    (defined(CPU_MIMXRT1062DVL6B) || defined(CPU_MIMXRT1062DVL6A) ||         \
     defined(CPU_MIMXRT1042XJM5B) || defined(_MIMXRT1042_H_) ||              \
     defined(CPU_MIMXRT1062DVMAA_cm7) || defined(CPU_MIMXRT1062DVL6A_cm7) || \
     defined(CPU_MIMXRT1062DVL6B_cm7) || defined(_MIMXRT1062_H_))
#define MG_OTA MG_OTA_RT1060
#elif !defined(MG_OTA) &&                                            \
    (defined(CPU_MIMXRT1064DVL6B) || defined(CPU_MIMXRT1064DVL6A) || \
     defined(_MIMXRT1064_H_))
#define MG_OTA MG_OTA_RT1064
#elif !defined(MG_OTA) &&                                                    \
    (defined(CPU_MIMXRT1176DVMAA_cm7) || defined(CPU_MIMXRT1176DVMAA_cm4) || \
     defined(_MIMXRT1176_cm7_H_) || defined(_MIMXRT1176_cm4_H_) ||           \
     defined(_MIMXRT1176_H_))
#define MG_OTA MG_OTA_RT1170
#elif !defined(MG_OTA) && \
    (defined(CPU_MIMXRT1186CVJ8C_cm33) || defined(MIMXRT1186_cm33_H_))
#define MG_OTA MG_OTA_RT1180
#endif

#ifndef MG_IRAM
#if defined(CPU_MIMXRT1186CVJ8C_cm33) || defined(MIMXRT1186_cm33_H_)
#define MG_IRAM \
  __attribute__((noinline, section(".ramfunc.$SRAM_ITC_cm33")))
#else
#define MG_IRAM __attribute__((noinline, section(".data_RAM2")))
#endif
#endif

#ifndef MG_ETH_RAM
#if defined(CPU_MIMXRT1186CVJ8C_cm33) || defined(MIMXRT1186_cm33_H_)
#define MG_ETH_RAM __attribute__((section(".bss.$ETH_RAM")))
#endif
#endif

#ifndef MG_SET_MAC_ADDRESS
#if defined(MG_ENABLE_DRIVER_NETC) && MG_ENABLE_DRIVER_NETC
// No OCOTP_FUSES present on RT1186 to generate the UUID
#define MG_SET_MAC_ADDRESS(mac) (void) (mac)
#else
#if defined(MG_ENABLE_DRIVER_IMXRT11) && MG_ENABLE_DRIVER_IMXRT11
#define MG_OCOTP_FUSES ((volatile uint32_t *) 0x40cac900)
#else
#define MG_OCOTP_FUSES ((volatile uint32_t *) 0x401f4410)
#endif
#define MG_SET_MAC_ADDRESS(mac)                                             \
  do {                                                                      \
    mac[0] = 2;                                                             \
    mac[1] = (MG_OCOTP_FUSES[0] >> 0) & 255;                                \
    mac[2] = (MG_OCOTP_FUSES[0] >> 10) & 255;                               \
    mac[3] = ((MG_OCOTP_FUSES[0] >> 19) ^ (MG_OCOTP_FUSES[4] >> 19)) & 255; \
    mac[4] = (MG_OCOTP_FUSES[4] >> 10) & 255;                               \
    mac[5] = (MG_OCOTP_FUSES[4] >> 0) & 255;                                \
  } while (0)
#endif
#endif

#ifndef MG_IMXRT_WDOG1_TIMEOUT_MS
#define MG_IMXRT_WDOG1_TIMEOUT_MS 10000U
#endif

#if defined(CPU_MIMXRT1186CVJ8C_cm33) || defined(MIMXRT1186_cm33_H_)

#define MG_IMXRT_RTWDOG1_UNLOCK()                                \
  do {                                                           \
    if ((RTWDOG1->CS & RTWDOG_CS_CMD32EN_MASK) != 0U) {          \
      RTWDOG1->CNT = RTWDOG_UPDATE_KEY;                          \
    } else {                                                     \
      RTWDOG1->CNT = RTWDOG_UPDATE_KEY & 0xffffU;                \
      RTWDOG1->CNT = RTWDOG_UPDATE_KEY >> 16;                    \
    }                                                            \
    while ((RTWDOG1->CS & RTWDOG_CS_ULK_MASK) == 0U) (void) 0;  \
  } while (0)

#define MG_IMXRT_RTWDOG1_FEED()                                  \
  do {                                                           \
    uint32_t primask_ = __get_PRIMASK();                         \
    __disable_irq();                                             \
    if ((RTWDOG1->CS & RTWDOG_CS_CMD32EN_MASK) != 0U) {          \
      RTWDOG1->CNT = RTWDOG_REFRESH_KEY;                         \
    } else {                                                     \
      RTWDOG1->CNT = RTWDOG_REFRESH_KEY & 0xffffU;               \
      RTWDOG1->CNT = RTWDOG_REFRESH_KEY >> 16;                   \
    }                                                            \
    if (primask_ == 0U) __enable_irq();                          \
  } while (0)

#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START()                                      \
  do {                                                                     \
    uint32_t primask_ = __get_PRIMASK();                                   \
    SRC_GENERAL_REG->SRMASK &= ~SRC_GENERAL_SRMASK_WDOG1_MASK_MASK;        \
    __disable_irq();                                                       \
    MG_IMXRT_RTWDOG1_UNLOCK();                                             \
    RTWDOG1->WIN = 0U;                                                     \
    RTWDOG1->TOVAL = (MG_IMXRT_WDOG1_TIMEOUT_MS + 7U) / 8U;                \
    RTWDOG1->CS = RTWDOG_CS_EN_MASK | RTWDOG_CS_UPDATE_MASK |              \
                  RTWDOG_CS_CLK(1U) | RTWDOG_CS_PRES_MASK |                \
                  RTWDOG_CS_CMD32EN_MASK;                                  \
    while ((RTWDOG1->CS & RTWDOG_CS_RCS_MASK) == 0U) (void) 0;             \
    if (primask_ == 0U) __enable_irq();                                    \
  } while (0)
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() MG_IMXRT_RTWDOG1_FEED()
#endif

// GPR5 survives RTWDOG global resets and is not reserved by the boot ROM.
#ifndef MG_OTA_STATE_GET
#define MG_OTA_STATE_GET() SRC_GENERAL_REG->GPR[5]
#endif

#ifndef MG_OTA_STATE_SET
#define MG_OTA_STATE_SET(v) (SRC_GENERAL_REG->GPR[5] = (uint32_t) (v))
#endif

#else

#define MG_IMXRT_WDOG1_FEED() \
  do {                        \
    WDOG1->WSR = 0x5555;      \
    WDOG1->WSR = 0xaaaa;      \
  } while (0)

#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START()                                     \
  do {                                                                    \
    uint16_t wt_ = (uint16_t) ((MG_IMXRT_WDOG1_TIMEOUT_MS / 500) - 1);    \
    if ((WDOG1->WCR & WDOG_WCR_WDE_MASK) == 0) {                          \
      WDOG1->WMCR = 0;                                                    \
      WDOG1->WCR = (uint16_t) (WDOG_WCR_WDZST_MASK | WDOG_WCR_WDBG_MASK | \
                               WDOG_WCR_WDE_MASK | WDOG_WCR_WDT_MASK |    \
                               WDOG_WCR_SRS_MASK | WDOG_WCR_WDW_MASK |    \
                               WDOG_WCR_WT(wt_));                         \
    }                                                                     \
    MG_IMXRT_WDOG1_FEED();                                                \
  } while (0)
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() MG_IMXRT_WDOG1_FEED()
#endif

#ifndef MG_OTA_STATE_GET
#define MG_OTA_STATE_GET() SNVS->LPGPR[0]
#endif

#ifndef MG_OTA_STATE_SET
#define MG_OTA_STATE_SET(v)          \
  do {                               \
    if (SNVS->LPSR & (1u << 3)) {    \
      SNVS->LPLVDR = 0x41736166u;    \
      SNVS->LPSR = 1u << 3;          \
    }                                \
    SNVS->LPGPR[0] = (uint32_t) (v); \
  } while (0)
#endif

#endif

#endif
