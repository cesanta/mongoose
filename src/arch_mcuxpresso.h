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
    !defined(MG_ENABLE_DRIVER_IMXRT11)
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
#endif

#ifndef MG_IRAM
#define MG_IRAM __attribute__((noinline, section(".data_RAM2")))
#endif

#ifndef MG_SET_MAC_ADDRESS
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

#ifndef MG_IMXRT_WDOG1_TIMEOUT_MS
#define MG_IMXRT_WDOG1_TIMEOUT_MS 10000
#endif

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
