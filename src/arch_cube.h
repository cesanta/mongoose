#pragma once

#if MG_ARCH == MG_ARCH_CUBE

#include <ctype.h>
#include <errno.h>
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

#ifndef MG_ENABLE_SOCKET
#define MG_ENABLE_SOCKET 0
#endif

#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 1  // Enable built-in TCP/IP stack
#endif

#if MG_ENABLE_TCPIP && !defined(MG_ENABLE_DRIVER_STM32F) && !defined(MG_ENABLE_DRIVER_STM32H)
#if defined(STM32F1) || defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
#define MG_ENABLE_DRIVER_STM32F 1
#elif defined(STM32H5) || defined(STM32H7)
#define MG_ENABLE_DRIVER_STM32H 1
#else
#error Select a driver in mongoose_config.h
#endif
#endif

#ifndef MG_TLS
#define MG_TLS MG_TLS_BUILTIN
#endif

#if !defined(MG_OTA) && defined(STM32F1) || defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
#define MG_OTA MG_OTA_STM32F
#elif !defined(MG_OTA) && defined(STM32H5)
#define MG_OTA MG_OTA_STM32H5
#elif !defined(MG_OTA) && defined(STM32H7)
#define MG_OTA MG_OTA_STM32H7
#endif
// use HAL-defined execute-in-ram section
#define MG_IRAM __attribute__((section(".RamFunc")))

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
