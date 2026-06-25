// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2025 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see http://www.gnu.org/licenses/
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in https://www.mongoose.ws/licensing/
//
// SPDX-License-Identifier: GPL-2.0-only or commercial

#ifndef MONGOOSE_H
#define MONGOOSE_H

#define MG_VERSION "7.22"

#ifdef __cplusplus
extern "C" {
#endif


#define MG_ARCH_CUSTOM 0        // User creates its own mongoose_config.h
#define MG_ARCH_UNIX 1          // Linux, BSD, Mac, ...
#define MG_ARCH_WIN32 2         // Windows
#define MG_ARCH_ESP32 3         // ESP32
#define MG_ARCH_ESP8266 4       // ESP8266

#define MG_ARCH_FREERTOS 5      // FreeRTOS <-- DEPRECATED !!!

#define MG_ARCH_THREADX 6       // Eclipse ThreadX (former MS Azure RTOS)
#define MG_ARCH_ZEPHYR 7        // Zephyr RTOS
#define MG_ARCH_ARMGCC 8        // Plain ARM GCC
#define MG_ARCH_CMSIS_RTOS1 9   // CMSIS-RTOS API v1 (Keil RTX)
#define MG_ARCH_TIRTOS 10       // Texas Semi TI-RTOS
#define MG_ARCH_PICOSDK 11      // Raspberry Pi Pico-SDK (RP2040, RP2350)
#define MG_ARCH_ARMCC 12        // Keil MDK-Core with Configuration Wizard
#define MG_ARCH_CMSIS_RTOS2 13  // CMSIS-RTOS API v2 (Keil RTX5, FreeRTOS)
#define MG_ARCH_RTTHREAD 14     // RT-Thread RTOS
#define MG_ARCH_ARMCGT 15       // Texas Semi ARM-CGT
#define MG_ARCH_CUBE 16	        // STM32Cube environment

#define MG_ARCH_NEWLIB MG_ARCH_ARMGCC  // Alias, deprecate in 2025

#if !defined(MG_ARCH)
#if defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#endif
#endif  // !defined(MG_ARCH)

#if !defined(MG_ARCH) || (MG_ARCH == MG_ARCH_CUSTOM)
#include "mongoose_config.h"  // keep this include
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it. Define MG_ARCH=... in mongoose_config.h"
#elif MG_ARCH == MG_ARCH_FREERTOS
#error "MG_ARCH_FREERTOS has been deprecated, set MG_ARCH=your_build_environment and MG_ENABLE_FREERTOS=1 instead"
#undef MG_ARCH	// avoid errors piling up, provide a clean environment so the error above is seen
#define MG_ARCH MG_ARCH_ARMGCC
#define MG_ENABLE_FREERTOS 1
#endif

// http://esr.ibiblio.org/?p=5095
#define MG_BIG_ENDIAN (*(uint16_t *) "\0\xff" < 0x100)


















#if MG_ARCH == MG_ARCH_ARMCGT

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MG_PATH_MAX 100
#define MG_ENABLE_DIRLIST 0

#endif


#if MG_ARCH == MG_ARCH_ARMGCC
#define _POSIX_TIMERS

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

#define MG_PATH_MAX 100
#define MG_ENABLE_DIRLIST 0

#endif


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
#define MG_OTA_STATE_GET()                                           \
  (RCC->APB3ENR |= RCC_APB3ENR_RTCAPBEN, TAMP->BKP0R)
#define MG_OTA_STATE_SET(v)                                           \
  (RCC->APB3ENR |= RCC_APB3ENR_RTCAPBEN, MG_OTA_DBP_ENABLE(),        \
   (TAMP->BKP0R = (uint32_t) (v)), TAMP->BKP0R)
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


#if MG_ARCH == MG_ARCH_ESP32

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <esp_ota_ops.h>   // Use angle brackets to avoid
#include <esp_timer.h>     // amalgamation ditching them
#include <esp_system.h>
#include <esp_mac.h>

#define MG_PATH_MAX 128

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#endif


#if MG_ARCH == MG_ARCH_ESP8266

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <esp_system.h>

#define MG_PATH_MAX 128

#endif


#if MG_ARCH == MG_ARCH_PICOSDK
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
#include <pico/rand.h>
#include <pico/unique_id.h>
int mkdir(const char *, mode_t);

#if MG_OTA == MG_OTA_PICOSDK
#include <hardware/flash.h>
#include <hardware/watchdog.h>
#include <hardware/structs/watchdog.h>
#include <pico/bootrom.h>

#ifndef MG_OTA_WATCHDOG_MS
#if PICO_RP2040
#define MG_OTA_WATCHDOG_MS 8000U // max delay on RP2040 is 8.3 seconds
#else
#define MG_OTA_WATCHDOG_MS 10000U // max delay on RP2350 is 16 seconds
#endif
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START() watchdog_enable(MG_OTA_WATCHDOG_MS, false)
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED() watchdog_update()
#endif

#define MG_OTA_STATE_GET() ((uint32_t) watchdog_hw->scratch[0])
#define MG_OTA_STATE_SET(v) \
  (watchdog_hw->scratch[0] = (uint32_t) (v), (uint32_t) watchdog_hw->scratch[0])

#if PICO_RP2040
#define MG_OTA_ROLLBACK()                                      \
  do {                                                         \
    mg_flash->swap_fn();                                       \
    *(volatile unsigned long *) 0xe000ed0c = 0x5fa0004;         \
  } while (0)
#else
#define MG_OTA_ROLLBACK()                                      \
  do {                                                         \
    mg_flash->swap_fn();                                       \
    ((rom_reboot_fn) rom_func_lookup(ROM_FUNC_REBOOT))(         \
        BOOT_TYPE_NORMAL | 0x100, 1, 0, 0);                    \
  } while (0)
#endif
#endif

#endif


#if MG_ARCH == MG_ARCH_RTTHREAD

#include <rtthread.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 1460
#endif

#endif // MG_ARCH == MG_ARCH_RTTHREAD


#if MG_ARCH == MG_ARCH_ARMCC || MG_ARCH == MG_ARCH_CMSIS_RTOS1 || \
    MG_ARCH == MG_ARCH_CMSIS_RTOS2

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <time.h>
#if MG_ARCH == MG_ARCH_CMSIS_RTOS1
#include "cmsis_os.h"  // keep this include
// https://developer.arm.com/documentation/ka003821/latest
extern uint32_t rt_time_get(void);
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS2
#include "cmsis_os2.h"  // keep this include
#endif

#define strdup(s) ((char *) mg_strdup(mg_str(s)).buf)

#if defined(__ARMCC_VERSION)
#define mode_t size_t
#if !defined(MG_ENABLE_POSIX_FS) || !MG_ENABLE_POSIX_FS
#else
#define mkdir(a, b) mg_mkdir(a, b)
static inline int mg_mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}
#endif
#endif

#if (MG_ARCH == MG_ARCH_CMSIS_RTOS1 || MG_ARCH == MG_ARCH_CMSIS_RTOS2) &&     \
    !defined MG_ENABLE_RL && (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP) && \
    (!defined(MG_ENABLE_TCPIP) || !MG_ENABLE_TCPIP)
#define MG_ENABLE_RL 1
#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif
#endif

#endif


#if MG_ARCH == MG_ARCH_THREADX

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Do not include time.h and stdlib.h, since they conflict with nxd_bsd.h
// extern time_t time(time_t *);
#include <nxd_bsd.h>

#define MG_DIRSEP '\\'
#undef FOPEN_MAX

#ifndef MG_PATH_MAX
#define MG_PATH_MAX 32
#endif

#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#define socklen_t int
#define closesocket(x) soc_close(x)

// In order to enable BSD support in NetxDuo, do the following (assuming Cube):
// 1. Add nxd_bsd.h and nxd_bsd.c to the repo:
//     https://github.com/eclipse-threadx/netxduo/blob/v6.1.12_rel/addons/BSD/nxd_bsd.c
//     https://github.com/eclipse-threadx/netxduo/blob/v6.1.12_rel/addons/BSD/nxd_bsd.h
// 2. Add to tx_user.h
//     #define TX_THREAD_USER_EXTENSION int bsd_errno;
// 3. Add to nx_user.h
//     #define NX_ENABLE_EXTENDED_NOTIFY_SUPPORT
// 4. Add __CCRX__ build preprocessor constant
//   Project -> Properties -> C/C++ -> Settings -> MCU Compiler -> Preprocessor

#endif


#if MG_ARCH == MG_ARCH_TIRTOS

#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <serrno.h>
#include <sys/socket.h>

#include <ti/sysbios/knl/Clock.h>

#endif


#if MG_ARCH == MG_ARCH_UNIX

#define _DARWIN_UNLIMITED_SELECT 1  // No limit on file descriptors

#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if !defined(MG_ENABLE_EPOLL) && defined(__linux__)
#define MG_ENABLE_EPOLL 1
#elif !defined(MG_ENABLE_POLL)
#define MG_ENABLE_POLL 1
#endif

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MG_ENABLE_EPOLL) && MG_ENABLE_EPOLL
#include <sys/epoll.h>
#elif defined(MG_ENABLE_POLL) && MG_ENABLE_POLL
#include <poll.h>
#else
#include <sys/select.h>
#endif

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 16384
#endif

#endif


#if MG_ARCH == MG_ARCH_WIN32

// Avoid name clashing; (macro expansion producing 'defined' has undefined
// behaviour). See config.h for user options
#ifndef MG_ENABLE_WINSOCK
#if (!defined(MG_ENABLE_TCPIP) || !MG_ENABLE_TCPIP) && \
    (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP) &&   \
    (!defined(MG_ENABLE_FREERTOS_TCP) || !MG_ENABLE_FREERTOS_TCP)
#define MG_ENABLE_WINSOCK 1
#else
#define MG_ENABLE_WINSOCK 0
#endif
#endif

#ifndef _CRT_RAND_S
#define _CRT_RAND_S
#endif

#ifndef _WIN32_WINNT
#if defined(_MSC_VER) && _MSC_VER < 1700
#define _WIN32_WINNT 0x0400 // Let vc98 pick up wincrypt.h
#else
#define _WIN32_WINNT 0x0600
#endif
#endif
#ifndef WINVER
#define WINVER _WIN32_WINNT
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <ctype.h>
#include <direct.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <winsock2.h>       // fix missing macros and types

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef enum { false = 0, true = 1 } bool;
#define inline __inline
#else
#include <stdbool.h>
#include <stdint.h>
#if MG_ENABLE_WINSOCK
#include <ws2tcpip.h>
#endif
#endif

#include <process.h>
#include <winerror.h>

// For mg_random()
#if defined(_MSC_VER) && _MSC_VER < 1700
#include <wincrypt.h>
#pragma comment(lib, "advapi32.lib")
#endif

#if defined(_MSC_VER) && _MSC_VER <= 1200
  #ifndef IPPROTO_IP
    #define IPPROTO_IP 0
  #endif

  #ifndef IP_ADD_MEMBERSHIP
    struct ip_mreq {
        struct in_addr imr_multiaddr;
        struct in_addr imr_interface;
    };
    #define IP_ADD_MEMBERSHIP  12
  #endif
#endif

// Protect from calls like std::snprintf in app code
// See https://github.com/cesanta/mongoose/issues/1047
#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#ifndef strdup  // For MSVC with _DEBUG, see #1359
#define strdup(x) _strdup(x)
#endif
#endif

#if defined(MG_ENABLE_POLL) && MG_ENABLE_POLL && (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP)
typedef unsigned long nfds_t; // see #3388
#endif

#if defined(_MSC_VER)
#if MG_ENABLE_WINSOCK
#pragma comment(lib, "ws2_32.lib")
#endif
#ifndef alloca
#define alloca(a) _alloca(a)
#endif
#endif

#define MG_DIRSEP '\\'

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#if MG_ENABLE_WINSOCK

#define MG_INVALID_SOCKET INVALID_SOCKET
#define MG_SOCKET_TYPE SOCKET
#define poll(a, b, c) WSAPoll((a), (b), (c))
#define closesocket(x) closesocket(x)
typedef int socklen_t;

#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? WSAGetLastError() : 0)

#define MG_SOCK_PENDING(errcode)                                            \
  (((errcode) < 0) &&                                                       \
   (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEINPROGRESS || \
    WSAGetLastError() == WSAEWOULDBLOCK))

#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (WSAGetLastError() == WSAECONNRESET))

#endif  // MG_ENABLE_WINSOCK

#define realpath(a, b) _fullpath((b), (a), MG_PATH_MAX)
#define sleep(x) Sleep((x) *1000)
#define mkdir(a, b) _mkdir(a)
#define rmdir(a) _rmdir(a)
#define timegm(x) _mkgmtime(x)

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef SIGPIPE
#define SIGPIPE 0
#endif

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 1
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 16384
#endif

#endif


#if MG_ARCH == MG_ARCH_ZEPHYR

#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
// #include <zephyr/posix/dirent.h>
#include <zephyr/posix/fcntl.h>
#include <zephyr/posix/sys/select.h>
#include <zephyr/random/random.h>
#include <zephyr/version.h>

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MG_PUTCHAR(x) printk("%c", x)
#ifndef strdup
#define strdup(s) ((char *) mg_strdup(mg_str(s)).buf)
#endif
#define strerror(x) zsock_gai_strerror(x)

#ifndef FD_CLOEXEC
#define FD_CLOEXEC 0
#endif

#ifndef F_SETFD
#define F_SETFD 0
#endif

#define MG_ENABLE_SSI 0

int rand(void);
int sscanf(const char *, const char *, ...);

#endif


#if defined(MG_ENABLE_FREERTOS) && MG_ENABLE_FREERTOS

#include <FreeRTOS.h>
#include <task.h>

#define MG_ENABLE_CUSTOM_CALLOC 1

static inline void mg_free(void *ptr) {
  vPortFree(ptr);
}

// Re-route calloc/free to FreeRTOS functions, don't use stdlib
static inline void *mg_calloc(size_t cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size * cnt);
  return p;
}

#endif  // MG_ENABLE_FREERTOS


#if defined(MG_ENABLE_FREERTOS_TCP) && MG_ENABLE_FREERTOS_TCP

#include <limits.h>
#include <list.h>

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>

#define MG_SOCKET_TYPE Socket_t
#define MG_INVALID_SOCKET FREERTOS_INVALID_SOCKET

// Why FreeRTOS-TCP did not implement a clean BSD API, but its own thing
// with FreeRTOS_ prefix, is beyond me
#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define SO_BROADCAST 0
#define SO_ERROR 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? (errcode) : 0)

#define MG_SOCK_PENDING(errcode)                 \
  ((errcode) == -pdFREERTOS_ERRNO_EWOULDBLOCK || \
   (errcode) == -pdFREERTOS_ERRNO_EISCONN ||     \
   (errcode) == -pdFREERTOS_ERRNO_EINPROGRESS || \
   (errcode) == -pdFREERTOS_ERRNO_EAGAIN)

#define MG_SOCK_RESET(errcode) ((errcode) == -pdFREERTOS_ERRNO_ENOTCONN)

// actually only if optional timeout is enabled
#define MG_SOCK_INTR(fd) (fd == NULL)

#define sockaddr_in freertos_sockaddr
#define sockaddr freertos_sockaddr
#if ipFR_TCP_VERSION_MAJOR >= 4
#define sin_addr sin_address.ulIP_IPv4
#endif
#define accept(a, b, c) FreeRTOS_accept((a), (b), (c))
#define connect(a, b, c) FreeRTOS_connect((a), (b), (c))
#define bind(a, b, c) FreeRTOS_bind((a), (b), (c))
#define listen(a, b) FreeRTOS_listen((a), (b))
#define socket(a, b, c) FreeRTOS_socket((a), (b), (c))
#define send(a, b, c, d) FreeRTOS_send((a), (b), (c), (d))
#define recv(a, b, c, d) FreeRTOS_recv((a), (b), (c), (d))
#define setsockopt(a, b, c, d, e) FreeRTOS_setsockopt((a), (b), (c), (d), (e))
#define sendto(a, b, c, d, e, f) FreeRTOS_sendto((a), (b), (c), (d), (e), (f))
#define recvfrom(a, b, c, d, e, f) \
  FreeRTOS_recvfrom((a), (b), (c), (d), (e), (f))
#define closesocket(x) FreeRTOS_closesocket(x)
#define gethostbyname(x) FreeRTOS_gethostbyname(x)
#define getsockname(a, b, c) mg_getsockname((a), (b), (c))
#define getpeername(a, b, c) mg_getpeername((a), (b), (c))

static inline int mg_getsockname(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return -1;
}

static inline int mg_getpeername(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
#endif


#if defined(MG_ENABLE_LWIP) && MG_ENABLE_LWIP

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#include <sys/stat.h>
#endif

struct timeval;

#include <lwip/sockets.h>

#if !LWIP_TIMEVAL_PRIVATE
#if defined(__GNUC__) && !defined(__ARMCC_VERSION) // armclang sets both
#include <sys/time.h>
#else
struct timeval {
  time_t tv_sec;
  long tv_usec;
};
#endif
#endif

#if LWIP_SOCKET != 1
// Sockets support disabled in LWIP by default
#error Set LWIP_SOCKET variable to 1 (in lwipopts.h)
#endif
#endif


#if defined(MG_ENABLE_RL) && MG_ENABLE_RL
#include <rl_net.h>

#define closesocket(x) closesocket(x)

#define TCP_NODELAY SO_KEEPALIVE

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? (errcode) : 0)

#define MG_SOCK_PENDING(errcode)                                \
  ((errcode) == BSD_EWOULDBLOCK || (errcode) == BSD_EALREADY || \
   (errcode) == BSD_EINPROGRESS)

#define MG_SOCK_RESET(errcode) \
  ((errcode) == BSD_ECONNABORTED || (errcode) == BSD_ECONNRESET)

// In blocking mode, which is enabled by default, accept() waits for a
// connection request. In non blocking mode, you must call accept()
// again if the error code BSD_EWOULDBLOCK is returned.
#define MG_SOCK_INTR(fd) (fd == BSD_EWOULDBLOCK)

#define socklen_t int
#endif


#ifndef MG_ENABLE_LOG
#define MG_ENABLE_LOG 1
#endif

#ifndef MG_ENABLE_CUSTOM_CALLOC
#define MG_ENABLE_CUSTOM_CALLOC 0
#endif

#ifndef MG_ENABLE_CUSTOM_LOG
#define MG_ENABLE_CUSTOM_LOG 0  // Let user define their own MG_LOG
#endif

#ifndef MG_ENABLE_FREERTOS
#define MG_ENABLE_FREERTOS 0  // FreeRTOS RTOS
#endif

#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 0  // Mongoose built-in network stack
#endif

#ifndef MG_ENABLE_BSD_SOCKETS
#define MG_ENABLE_BSD_SOCKETS 0  // BSD API support for built-in stack
#endif

#ifndef MG_ENABLE_BSD_PROTOTYPES
#define MG_ENABLE_BSD_PROTOTYPES 1
#endif

#ifndef MG_ENABLE_LWIP
#define MG_ENABLE_LWIP 0  // lwIP network stack
#endif

#ifndef MG_ENABLE_FREERTOS_TCP
#define MG_ENABLE_FREERTOS_TCP 0  // Amazon FreeRTOS-TCP network stack
#endif

#ifndef MG_ENABLE_RL
#define MG_ENABLE_RL 0  // ARM MDK network stack
#endif

#ifndef MG_ENABLE_SOCKET
#define MG_ENABLE_SOCKET !MG_ENABLE_TCPIP
#endif

#ifndef MG_ENABLE_POLL
#define MG_ENABLE_POLL 0
#endif

#ifndef MG_ENABLE_EPOLL
#define MG_ENABLE_EPOLL 0
#endif

#ifndef MG_ENABLE_FATFS
#define MG_ENABLE_FATFS 0
#endif

#ifndef MG_ENABLE_LFS
#define MG_ENABLE_LFS 0
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_IPV6_V6ONLY
#define MG_IPV6_V6ONLY 0  // IPv6 socket binds only to V6, not V4 address
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 1
#endif

// Set MG_ENABLE_WINSOCK=0 for Win32 builds with other external IP stack not
// mentioned in arch_win32.h
#ifndef MG_ENABLE_WINSOCK
#define MG_ENABLE_WINSOCK 1
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 0
#endif

#ifndef MG_ENABLE_CUSTOM_RANDOM
#define MG_ENABLE_CUSTOM_RANDOM 0
#endif

#ifndef MG_ENABLE_CUSTOM_MILLIS
#define MG_ENABLE_CUSTOM_MILLIS 0
#endif

#ifndef MG_ENABLE_ASSERT
#define MG_ENABLE_ASSERT 0
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512  // Granularity of the send/recv IO buffer growth
#endif

#ifndef MG_MAX_RECV_SIZE
#define MG_MAX_RECV_SIZE (3UL * 1024UL * 1024UL)  // Maximum recv IO buffer size
#endif

#ifndef MG_DATA_SIZE
#define MG_DATA_SIZE 32  // struct mg_connection :: data size
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 30
#endif

#ifndef MG_HTTP_INDEX
#define MG_HTTP_INDEX "index.html"
#endif

#ifndef MG_PATH_MAX
#ifdef PATH_MAX
#define MG_PATH_MAX PATH_MAX
#else
#define MG_PATH_MAX 128
#endif
#endif

#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 128
#endif

#ifndef MG_DIRSEP
#define MG_DIRSEP '/'
#endif

#ifndef MG_ENABLE_POSIX_FS
#define MG_ENABLE_POSIX_FS 0
#endif

#ifndef MG_INVALID_SOCKET
#define MG_INVALID_SOCKET (-1)
#endif

#ifndef MG_SOCKET_TYPE
#define MG_SOCKET_TYPE int
#endif

#ifndef MG_SOCKET_ERRNO
#define MG_SOCKET_ERRNO errno
#endif

#if MG_ENABLE_EPOLL
#define MG_EPOLL_ADD(c)                                                    \
  do {                                                                     \
    struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};          \
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_ADD, (int) (size_t) c->fd, &ev); \
  } while (0)
#define MG_EPOLL_MOD(c, wr)                                                \
  do {                                                                     \
    struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};          \
    if (wr) ev.events |= EPOLLOUT;                                         \
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_MOD, (int) (size_t) c->fd, &ev); \
  } while (0)
#else
#define MG_EPOLL_ADD(c)
#define MG_EPOLL_MOD(c, wr)
#endif

#ifndef MG_ENABLE_PROFILE
#define MG_ENABLE_PROFILE 0
#endif

#ifndef MG_ENABLE_TCPIP_DRIVER_INIT    // mg_mgr_init() will also initialize
#define MG_ENABLE_TCPIP_DRIVER_INIT 1  // enabled built-in driver for
#endif                                 // Mongoose built-in network stack

#ifndef MG_TCPIP_IP                      // e.g. MG_IPV4(192, 168, 0, 223)
#define MG_TCPIP_IP MG_IPV4(0, 0, 0, 0)  // Default is 0.0.0.0 (DHCP)
#endif

#ifndef MG_TCPIP_MASK
#define MG_TCPIP_MASK MG_IPV4(0, 0, 0, 0)  // Default is 0.0.0.0 (DHCP)
#endif

#ifndef MG_TCPIP_GW
#define MG_TCPIP_GW MG_IPV4(0, 0, 0, 0)  // Default is 0.0.0.0 (DHCP)
#endif

#if MG_ENABLE_IPV6

#ifndef MG_TCPIP_GLOBAL
#define MG_TCPIP_GLOBAL MG_IPV6(0, 0, 0, 0, 0, 0, 0, 0)
#endif

#ifndef MG_TCPIP_IPV6_LINKLOCAL
#define MG_TCPIP_IPV6_LINKLOCAL MG_IPV6(0, 0, 0, 0, 0, 0, 0, 0)
#endif

#ifndef MG_TCPIP_PREFIX_LEN
#define MG_TCPIP_PREFIX_LEN 0
#endif

#ifndef MG_TCPIP_GW6
#define MG_TCPIP_GW6 MG_IPV6(0, 0, 0, 0, 0, 0, 0, 0)
#endif

#endif

#ifndef MG_SET_MAC_ADDRESS
#define MG_SET_MAC_ADDRESS(mac)
#endif

#ifndef MG_TCPIP_DHCPNAME_SIZE
#define MG_TCPIP_DHCPNAME_SIZE 18  // struct mg_tcpip_if :: dhcp_name size
#endif

#ifndef MG_SET_WIFI_CONFIG
#define MG_SET_WIFI_CONFIG(ifp, driver_data)
#endif

#ifndef MG_ENABLE_TCPIP_PRINT_DEBUG_STATS
#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 0
#endif

#ifndef MG_ENABLE_CHACHA20
#define MG_ENABLE_CHACHA20 1  // When set to 0, GCM is used. For MG_TLS_BUILTIN
#endif




// Macros to record timestamped events that happens with a connection.
// They are saved into a c->prof IO buffer, each event is a name and a 32-bit
// timestamp in milliseconds since connection init time.
//
// Test (run in two separate terminals):
//   make -C tutorials/http/http-server/ CFLAGS_EXTRA=-DMG_ENABLE_PROFILE=1
//   curl localhost:8000
// Output:
//   1ea1f1e7 2 net.c:150:mg_close_conn      3 profile:                                                            
//   1ea1f1e8 2 net.c:150:mg_close_conn      1ea1f1e6 init                                                         
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_OPEN
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_ACCEPT 
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_READ
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_HTTP_MSG
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_WRITE
//   1ea1f1e8 2 net.c:150:mg_close_conn          1 EV_CLOSE
//
// Usage:
//   Enable profiling by setting MG_ENABLE_PROFILE=1
//   Invoke MG_PROF_ADD(c, "MY_EVENT_1") in the places you'd like to measure

#if MG_ENABLE_PROFILE
struct mg_profitem {
  const char *name;    // Event name
  uint32_t timestamp;  // Milliseconds since connection creation (MG_EV_OPEN)
};

#define MG_PROFILE_ALLOC_GRANULARITY 256  // Can save 32 items wih to realloc

// Adding a profile item to the c->prof. Must be as fast as possible.
// Reallocation of the c->prof iobuf is not desirable here, that's why we
// pre-allocate c->prof with MG_PROFILE_ALLOC_GRANULARITY.
// This macro just inits and copies 8 bytes, and calls mg_millis(),
// which should be fast enough.
#define MG_PROF_ADD(c, name_)                                             \
  do {                                                                    \
    struct mg_iobuf *io = &c->prof;                                       \
    uint32_t inittime = ((struct mg_profitem *) io->buf)->timestamp;      \
    struct mg_profitem item = {name_, (uint32_t) mg_millis() - inittime}; \
    mg_iobuf_add(io, io->len, &item, sizeof(item));                       \
  } while (0)

// Initialising profile for a new connection. Not time sensitive
#define MG_PROF_INIT(c)                                          \
  do {                                                           \
    struct mg_profitem first = {"init", (uint32_t) mg_millis()}; \
    mg_iobuf_init(&(c)->prof, 0, MG_PROFILE_ALLOC_GRANULARITY);  \
    mg_iobuf_add(&c->prof, c->prof.len, &first, sizeof(first));  \
  } while (0)

#define MG_PROF_FREE(c) mg_iobuf_free(&(c)->prof)

// Dumping the profile. Not time sensitive
#define MG_PROF_DUMP(c)                                            \
  do {                                                             \
    struct mg_iobuf *io = &c->prof;                                \
    struct mg_profitem *p = (struct mg_profitem *) io->buf;        \
    struct mg_profitem *e = &p[io->len / sizeof(*p)];              \
    MG_INFO(("%lu profile:", c->id));                              \
    while (p < e) {                                                \
      MG_INFO(("%5lx %s", (unsigned long) p->timestamp, p->name)); \
      p++;                                                         \
    }                                                              \
  } while (0)

#else
#define MG_PROF_INIT(c)
#define MG_PROF_FREE(c)
#define MG_PROF_ADD(c, name)
#define MG_PROF_DUMP(c)
#endif




// A non-owning view of an arbitrary byte buffer. Not NUL-terminated in general.
// Use mg_str() or mg_str_n() to construct; use mg_strdup() for an owned copy.
struct mg_str {
  char *buf;   // Pointer to the data (not necessarily NUL-terminated)
  size_t len;  // Number of bytes
};

// Using macro to avoid shadowing the C++ struct constructor, see #1298
#define mg_str(s) mg_str_s(s)

// Creates an mg_str from a NUL-terminated C string. Computes length via
// strlen. Returns {NULL, 0} when s is NULL.
struct mg_str mg_str(const char *s);

// Creates an mg_str from a pointer and explicit byte count. s need not be
// NUL-terminated. Does not copy the data.
struct mg_str mg_str_n(const char *s, size_t n);

// Case-insensitive strcmp for NUL-terminated C strings.
// Returns 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
int mg_casecmp(const char *s1, const char *s2);

// Compares two mg_str by content and length. Returns 0 if identical,
// -1 if str1 < str2, 1 if str1 > str2. Length is used as a tiebreaker
// when one string is a prefix of the other.
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);

// Case-insensitive mg_strcmp.
int mg_strcasecmp(const struct mg_str str1, const struct mg_str str2);

// Returns a heap-allocated, NUL-terminated copy of s.
// Returns {NULL, 0} on OOM or if s.buf is NULL or s.len is 0.
// Caller must mg_free(result.buf) when done.
struct mg_str mg_strdup(const struct mg_str s);

// Matches a string against a glob pattern.
//
// Returns:
//   True if the whole str matches pattern.
// Example:
//   if (mg_match(hm->uri, mg_str("/api/#"), NULL)) { ... }
// Full examples:
//   tutorials/http/http-server, tutorials/http/link-checker,
//   tutorials/websocket/websocket-server
// Related APIs:
//   mg_str(), mg_str_n(), mg_span(), mg_strcmp(), mg_strcasecmp()
// Notes:
//   Pattern wildcards: ? matches one character, * matches any sequence except
//   '/', and # matches any sequence including '/'. If caps is not NULL, each
//   wildcard capture is stored as a zero-copy mg_str slice into str. Pass
//   caps=NULL when captures are not needed. The caps array size should be
//   at least the number of wildcards in a pattern plus one. The last cap
//   is initialised to an empty string.
//   Attention: the pattern must consume the whole str in order to match.
bool mg_match(struct mg_str str, struct mg_str pattern, struct mg_str *caps);

// Splits s at the first occurrence of sep. Sets *a to the part before sep
// and *b to the part after it (sep is not included in either). Both slices
// are zero-copy views into s. a or b may be NULL if that half is not needed.
// If sep is not found, *a is the whole string and *b is empty.
// Returns false if s is empty or s.buf is NULL.
// Example - scan comma-separated key=value pairs:
// ```c
//   struct mg_str entry, key, val;
//   struct mg_str s = mg_str("a=333,b=777,hello");
//
//   while (mg_span(s, &entry, &s, ',')) {
//     mg_span(entry, &key, &val, '=');
//     printf("key: %.*s, val: %.*s\n", (int) key.len, key.buf, (int) val.len, val.buf);
//   }
// ```
bool mg_span(struct mg_str s, struct mg_str *a, struct mg_str *b, char sep);

// Parses str as an unsigned integer into the val_len-byte value at *val.
// val_len must be 1, 2, 4, or 8 (for uint8_t, uint16_t, uint32_t, uint64_t).
// base must be 2, 10, or 16; pass 0 to auto-detect from a "0b" or "0x" prefix.
// The entire string must consist of valid digits with no trailing characters.
// Returns false on overflow, invalid input, unsupported base, or unsupported val_len.
bool mg_str_to_num(struct mg_str str, int base, void *val, size_t val_len);




// Lock-free, single-producer / single-consumer (SPSC) message queue.
// Safe to write from an ISR and read from the main loop without locking,
// as long as only one context writes and one context reads at a time.
// Each message is stored with an internal 32-bit length prefix; the buf
// pointers returned by mg_queue_book() and mg_queue_next() already skip it.
//
// Producer pattern:
//   char buf[1024];
//   if (mg_queue_book(&q, &buf, len) >= len) {
//     memcpy(buf, data, len);
//     mg_queue_add(&q, len);
//   }
//
// Consumer pattern:
//   char *buf;
//   size_t len = mg_queue_next(&q, &buf);
//   if (len > 0) { process(buf, len); mg_queue_del(&q, len); }

struct mg_queue {
  char *buf;            // Caller-supplied backing buffer
  size_t size;          // Size of buf in bytes
  volatile size_t tail; // Read offset; updated by the consumer
  volatile size_t head; // Write offset; updated by the producer
};

// Initialises q to use the caller-supplied buf/size. buf must remain valid
// for the lifetime of the queue.
void mg_queue_init(struct mg_queue *q, char *buf, size_t size);

// Reserves space for a message of len bytes. Sets *buf to the write location.
// Returns the number of bytes available (>= len means the reservation
// succeeded). Does NOT commit the message; call mg_queue_add() after writing.
size_t mg_queue_book(struct mg_queue *q, char **buf, size_t len);

// Commits the most recently reserved message of len bytes. Must be called
// with the same len passed to the preceding mg_queue_book() call, after
// the data has been written into the buffer it returned.
void mg_queue_add(struct mg_queue *q, size_t len);

// Returns the length of the oldest unconsumed message and sets *buf to its
// start. Returns 0 if the queue is empty. Does NOT remove the message;
// call mg_queue_del() after processing to advance the read position.
size_t mg_queue_next(struct mg_queue *q, char **buf);

// Removes the oldest message of len bytes. Must be called with the same len
// returned by the preceding mg_queue_next() call.
void mg_queue_del(struct mg_queue *q, size_t len);




// Character output function. Called once per output character by mg_xprintf/mg_vxprintf.
// arg is an arbitrary user pointer passed through from the caller.
typedef void (*mg_pfn_t)(char, void *arg);

// Custom %M/%m printer. Reads its own arguments from ap and writes via fn.
// Returns the number of characters printed (excluding the quotes added by %m).
typedef size_t (*mg_pm_t)(mg_pfn_t fn, void *arg, va_list *);

// Formats fmt with printf-like specifiers, calling fn for each output character.
// Returns the total number of characters printed. Supported specifiers:
//  - %c - expect char
//  - %f, %g - expect double
//  - %hhd, %hd, %d, %ld, %lld - for char, short, int, long, int64_t
//  - %hhu, %hu, %u, %lu, %llu - same but for unsigned variants
//  - %hhx, %hx, %x, %lx, %llx - same, unsigned and hex output
//  - %p - expects any pointer, prints 0x..... hex value
//  - %s - expects char *
//  - %% - prints the % character itself
//  - %X.Y - optional width and precision modifiers (e.g.: %1.2d)
//  - %.* - optional precision modifier, expected as int argument (e.g.: %.*d)
//  - %M - (EXTENSION) expects a pointer to a custom print function and its arguments
//  - %m - (EXTENSION) exactly like %M but double-quotes the output
//  - %M/%m (custom mg_pm_t printer; %m additionally wraps the output in quotes).
//
// There are higher level wrappers around these functions that use
// predefined output functions, and can print to char buffer, or connection
// send iobuffer, etc etc. - for example, mg_snrptinf(). The %M/%m non-standard
// specifier allows to use custom formtter. There are several pre-defined
// formatters:
//   - mg_print_base64 - prints a buffer as a base64-encoded string
//   - mg_print_esc - prints a JSON-escaped string
//   - mg_print_hex - prints a buffer as a hex string
//   - mg_print_ip - prints an IP address in a struct mg_str
//   - mg_print_ip_port - prints IP address and port in a struct mg_str
//   - mg_print_ip4 - prints an IPv4 address
//   - mg_print_ip6 - prints an IPv6 address
//   - mg_print_mac - prints a MAC address
//
// Usage example:
//
// ```c
//   struct a { int a, b; };
//
//   size_t print_a(mg_pfn_t fn, void *arg, va_list *ap) {
//     struct a *p = va_arg(*ap, struct a *);
//     return mg_xprintf(fn, arg, "{%m:%d}", MG_ESC("a"), p->a);
//   }
//
//   struct a temp = { 42, 43 };
//   char buf[100];
//
//   mg_snprintf(buf, sizeof(buf), "%M", print_a, &temp);    // {"a":42}
// ```
size_t mg_vxprintf(mg_pfn_t fn, void *arg, const char *fmt, va_list *);
size_t mg_xprintf(mg_pfn_t fn, void *arg, const char *fmt, ...);







// Formats data into a caller-supplied buffer.
//
// Returns:
//   Number of bytes that would be written if buf were large enough, like
//   snprintf().
// Example:
//   char buf[20];
//   mg_snprintf(buf, sizeof(buf), "{%m:%d}", MG_ESC("status"), 1);
// Full examples:
//   tutorials/http/link-checker, tutorials/http/redirect-to-https,
//   tutorials/mqtt/mqtt-dashboard/device
// Related APIs:
//   mg_xprintf(), mg_printf(), mg_mprintf(), MG_ESC
// Notes:
//   These functions are just wrappers around mg_xprintf().
//   Call with buf=NULL and len=0 to measure. NUL-terminates if the result fits
//   (return value < len). Supports mg_xprintf specifiers, including custom
//   %M/%m printers. Use MG_ESC when printing JSON strings.
size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap);
size_t mg_snprintf(char *buf, size_t len, const char *fmt, ...);

// Formats into a heap-allocated NUL-terminated string. Caller must mg_free() it.
char *mg_vmprintf(const char *fmt, va_list *ap);
char *mg_mprintf(const char *fmt, ...);

// Formats into a mg_queue slot. Returns bytes written, or 0 if no space.
size_t mg_queue_printf(struct mg_queue *, const char *fmt, ...);

// Built-in %M/%m printer functions. Each reads its argument(s) from ap.
size_t mg_print_base64(mg_pfn_t, void *arg, va_list *ap);   // expects: const void *buf, size_t len
size_t mg_print_esc(mg_pfn_t, void *arg, va_list *ap);      // expects: int len, const char *str -- use MG_ESC()
size_t mg_print_hex(mg_pfn_t, void *arg, va_list *ap);      // expects: const void *buf, size_t len
size_t mg_print_ip(mg_pfn_t, void *arg, va_list *ap);       // expects: const struct mg_addr *
size_t mg_print_ip_port(mg_pfn_t, void *arg, va_list *ap);  // expects: const struct mg_addr *
size_t mg_print_ip4(mg_pfn_t, void *arg, va_list *ap);      // expects: uint32_t *ipv4
size_t mg_print_ip6(mg_pfn_t, void *arg, va_list *ap);      // expects: uint8_t[16] ipv6
size_t mg_print_mac(mg_pfn_t, void *arg, va_list *ap);      // expects: uint8_t[6] mac
size_t mg_print_ieee64(mg_pfn_t, void *arg, va_list *ap);   // expects: uint64_t
size_t mg_print_l2addr(mg_pfn_t, void *arg, va_list *ap);   // expects: uint8_t l2, uint8_t[n] n-byte l2-dependent address
size_t mg_print_html_esc(mg_pfn_t, void *arg, va_list *ap); // expects: int len, const char *str -- use MG_ESC()


// Output functions for use as the fn argument to mg_xprintf/mg_vxprintf.
void mg_pfn_iobuf(char ch, void *param);           // param: struct mg_iobuf * (resizes as needed)
void mg_pfn_iobuf_noresize(char ch, void *param);  // param: struct mg_iobuf * (fixed buffer, no resize)
void mg_pfn_stdout(char c, void *param);           // param: ignored, writes to stdout

// Expands to the three arguments for %m to JSON-escape a string literal.
// Example: mg_snprintf(buf, len, "%m", MG_ESC("hello")) -> "\"hello\""
#define MG_ESC(str) mg_print_esc, 0, (str)
// Logging
//
// Mongoose provides a set of functions and macros for logging. The application can
// use these functions for its own purposes as well as the rest of Mongoose API.







// Log levels
enum { MG_LL_NONE, MG_LL_ERROR, MG_LL_INFO, MG_LL_DEBUG, MG_LL_VERBOSE };

// Set Mongoose logging level. Example: `mg_log_set(MG_LL_INFO);`
extern int mg_log_level;  // Current log level, one of MG_LL_*
#define mg_log_set(level_) mg_log_level = (level_)

void mg_log(const char *fmt, ...);
void mg_log_prefix(int ll, const char *file, int line, const char *fname);

// Log a hex dump of binary data `buf`, `len`.
void mg_hexdump(const void *buf, size_t len);

// Set log printer function which prints one byte. Example:
//
// ```c
// static void print_char(char ch, void *param) {
//   hal_uart_write_char(param, ch);
// }
// ...
// mg_log_set_fn(print_char, USART3);
// ```
void mg_log_set_fn(mg_pfn_t fn, void *param);

#if MG_ENABLE_LOG
#if !defined(_MSC_VER) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#define MG___FUNC__ ""
#else
#define MG___FUNC__ __func__  // introduced in C99
#endif
#define MG_LOG(level, args)                                    \
  do {                                                         \
    if ((level) <= mg_log_level) {                             \
      mg_log_prefix((level), __FILE__, __LINE__, MG___FUNC__); \
      mg_log args;                                             \
    }                                                          \
  } while (0)
#else
#define MG_LOG(level, args) \
  do {                      \
    if (0) mg_log args;     \
  } while (0)
#endif

// Logging macros. Note: the argument is exactly like for mg_snprintf(),
// and it should be enclosed in double parenthesis. Example:
// ```c
// MG_INFO(("Conn %lu, recv buf: %.*s", c->id, c->recv.len, c->recv.buf));
// ```
#define MG_ERROR(args) MG_LOG(MG_LL_ERROR, args)
#define MG_INFO(args) MG_LOG(MG_LL_INFO, args)
#define MG_DEBUG(args) MG_LOG(MG_LL_DEBUG, args)
#define MG_VERBOSE(args) MG_LOG(MG_LL_VERBOSE, args)




struct mg_timer {
  uint64_t period_ms;          // Timer period in milliseconds
  uint64_t expire;             // Expiration timestamp in milliseconds
  unsigned flags;              // Possible flags values below
#define MG_TIMER_ONCE 0        // Call function once
#define MG_TIMER_REPEAT 1      // Call function periodically
#define MG_TIMER_RUN_NOW 2     // Call immediately when timer is set
#define MG_TIMER_CALLED 4      // Timer function was called at least once
#define MG_TIMER_AUTODELETE 8  // mg_free() timer when done
  void (*fn)(void *);          // Function to call
  void *arg;                   // Function argument
  struct mg_timer *next;       // Linkage
};

void mg_timer_init(struct mg_timer **head, struct mg_timer *timer,
                   uint64_t milliseconds, unsigned flags, void (*fn)(void *),
                   void *arg);
void mg_timer_free(struct mg_timer **head, struct mg_timer *);
void mg_timer_poll(struct mg_timer **head, uint64_t new_ms);





// Flags returned by mg_fs.st() and passed to mg_fs.open().
enum { MG_FS_READ = 1, MG_FS_WRITE = 2, MG_FS_DIR = 4, MG_FS_EXCL = 8 };

// Filesystem abstraction. Implement all function pointers to plug in a custom
// filesystem. Short UNIX-style names are used deliberately to avoid conflicts
// with libraries that define macros for stat(), read(), write(), etc.
struct mg_fs {
  int (*st)(const char *path, size_t *size, time_t *mtime);  // Stat: return MG_FS_* flags; size/mtime may be NULL
  void (*ls)(const char *path, void (*fn)(const char *, void *), void *);  // List dir: call fn(name, arg) per entry
  void *(*op)(const char *path, int flags);             // Open file; return opaque fd, or NULL on error
  void (*cl)(void *fd);                                 // Close file
  size_t (*rd)(void *fd, void *buf, size_t len);        // Read up to len bytes; return bytes read
  size_t (*wr)(void *fd, const void *buf, size_t len);  // Write len bytes; return bytes written
  size_t (*sk)(void *fd, size_t offset);                // Seek to offset from start; return new position
  bool (*mv)(const char *from, const char *to);         // Rename/move file
  bool (*rm)(const char *path);                         // Delete file
  bool (*mkd)(const char *path);                        // Create directory
};

extern struct mg_fs mg_fs_posix;   // POSIX filesystem (open/close/read/write/seek)
extern struct mg_fs mg_fs_packed;  // Read-only packed filesystem (see tutorials/core/embedded-filesystem)
extern struct mg_fs mg_fs_fat;     // FAT filesystem

// Open file descriptor returned by mg_fs_open(). Bundles the raw fd with its fs.
struct mg_fd {
  void *fd;          // Opaque file handle from fs->op()
  struct mg_fs *fs;  // Filesystem that owns fd
};

// Opens path with the given MG_FS_* flags. Returns NULL on error.
struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags);

// Closes fd and frees the mg_fd struct. Safe to call with NULL.
void mg_fs_close(struct mg_fd *fd);

// Sequential directory iterator. Call repeatedly with the same buf/len;
// each call fills buf with the next entry name. Returns false when done.
bool mg_fs_ls(struct mg_fs *fs, const char *path, char *buf, size_t len);

// Reads the entire file into a heap-allocated buffer. The returned mg_str is
// NUL-terminated. Caller must mg_free(result.buf). Returns {NULL,0} on error.
struct mg_str mg_file_read(struct mg_fs *fs, const char *path);

// Atomically writes buf/len to path (via a temp file + rename). Returns false on error.
bool mg_file_write(struct mg_fs *fs, const char *path, const void *buf, size_t len);

// Formats a string and atomically writes it to path. Returns false on error.
bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...);


// A file stored in memory (flash or RAM), used by mg_fs_packed.
struct mg_mem_file {
  const char *path;          // Virtual file path, e.g. "/web_root/index.html"
  const unsigned char *data; // Pointer to file content
  size_t size;               // Content size in bytes
  time_t mtime;              // Modification time (may be 0)
};

// Pointer to a NULL-path-terminated array of in-memory files for mg_fs_packed.
// Override to serve a custom set of files:
//   static const struct mg_mem_file my_files[] = {
//     {"/index.html", data, sizeof(data), 0},
//     {NULL, NULL, 0, 0},
//   };
//   mg_mem_files = my_files;
extern const struct mg_mem_file *mg_mem_files;

// Looks up path in the packed file array. Returns an mg_str pointing directly
// into the stored data (zero-copy). Returns {NULL,0} if not found.
struct mg_str mg_unpacked(const char *path);

// Array of packed files generated by the "pack" utility into packed_fs.c.
// Activate by assigning: mg_mem_files = mg_packed_files;
extern const struct mg_mem_file mg_packed_files[];








#if MG_ENABLE_ASSERT
#include <assert.h>
#elif !defined(assert)
#define assert(x)
#endif

// Wrappers around calloc/free. Override by defining MG_ENABLE_CUSTOM_CALLOC=1
// and providing your own implementations.
void *mg_calloc(size_t count, size_t size);
void mg_free(void *ptr);

// Zeroes len bytes at buf using a volatile write loop that the compiler cannot
// elide. Safe to call with buf=NULL. Use instead of memset() for clearing
// sensitive data (keys, passwords).
void mg_bzero(volatile unsigned char *buf, size_t len);

// Fills buf with len cryptographically random bytes. Uses the best available
// hardware or OS source (hardware RNG, /dev/urandom, CryptGenRandom, etc.).
// Falls back to rand() with an error log if no strong source is available.
// Returns true when a strong source was used, false on fallback to rand().
// Override by defining MG_ENABLE_CUSTOM_RANDOM=1 and providing your own impl.
bool mg_random(void *buf, size_t len);

// Fills buf with len-1 random alphanumeric characters ([a-zA-Z0-9]) and
// NUL-terminates. Returns buf.
char *mg_random_str(char *buf, size_t len);

// Computes CRC32 (polynomial 0xEDB88320) over buf/len. Pass crc=0 to start
// a new checksum; pass the result of a prior call to extend over more data.
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);

// Returns true if path is safe to serve from the filesystem. Rejects paths
// that start with '~' or '..', or contain a '/../' component, to prevent
// directory traversal attacks.
bool mg_path_is_sane(const struct mg_str path);

// Busy-waits for at least ms milliseconds using mg_millis(). Blocks the
// calling context; avoid in event handlers.
void mg_delayms(unsigned int ms);

// Packs four byte values into a uint32_t in big-endian order.
// MG_U32(1, 2, 3, 4) == 0x01020304
#define MG_U32(a, b, c, d)                                         \
  (((uint32_t) ((a) &255) << 24) | ((uint32_t) ((b) &255) << 16) | \
   ((uint32_t) ((c) &255) << 8) | (uint32_t) ((d) &255))

// Constructs an IPv4 address in network byte order from four decimal octets.
// Usage: uint32_t ip = MG_IPV4(192, 168, 1, 1);
#define MG_IPV4(a, b, c, d) mg_htonl(MG_U32(a, b, c, d))

// Expands to a brace-enclosed byte initialiser for a 16-byte IPv6 address.
// Arguments are the eight 16-bit groups in the address, e.g.:
//   uint8_t ip6[16] = MG_IPV6(0x2001, 0xdb8, 0, 0, 0, 0, 0, 1);
#define MG_IPV6(a, b, c, d, e, f, g ,h) \
  { (uint8_t)((a)>>8),(uint8_t)(a), \
    (uint8_t)((b)>>8),(uint8_t)(b), \
    (uint8_t)((c)>>8),(uint8_t)(c), \
    (uint8_t)((d)>>8),(uint8_t)(d), \
    (uint8_t)((e)>>8),(uint8_t)(e), \
    (uint8_t)((f)>>8),(uint8_t)(f), \
    (uint8_t)((g)>>8),(uint8_t)(g), \
    (uint8_t)((h)>>8),(uint8_t)(h) }

// For printing IPv4 addresses: printf("%d.%d.%d.%d\n", MG_IPADDR_PARTS(&ip))
#define MG_U8P(ADDR) ((uint8_t *) (ADDR))
#define MG_IPADDR_PARTS(ADDR) \
  MG_U8P(ADDR)[0], MG_U8P(ADDR)[1], MG_U8P(ADDR)[2], MG_U8P(ADDR)[3]

// Read an unaligned big-endian value from byte pointer p into a native integer.
// Safe on architectures that forbid unaligned access (e.g. Cortex-M0).
#define MG_LOAD_BE16(p) \
  ((uint16_t) (((uint16_t) MG_U8P(p)[0] << 8U) | MG_U8P(p)[1]))
#define MG_LOAD_BE24(p)                           \
  ((uint32_t) (((uint32_t) MG_U8P(p)[0] << 16U) | \
               ((uint32_t) MG_U8P(p)[1] << 8U) | MG_U8P(p)[2]))
#define MG_LOAD_BE32(p)                           \
  ((uint32_t) (((uint32_t) MG_U8P(p)[0] << 24U) | \
               ((uint32_t) MG_U8P(p)[1] << 16U) | \
               ((uint32_t) MG_U8P(p)[2] << 8U) | MG_U8P(p)[3]))
#define MG_LOAD_BE64(p)                           \
  ((uint64_t) (((uint64_t) MG_U8P(p)[0] << 56U) | \
               ((uint64_t) MG_U8P(p)[1] << 48U) | \
               ((uint64_t) MG_U8P(p)[2] << 40U) | \
               ((uint64_t) MG_U8P(p)[3] << 32U) | \
               ((uint64_t) MG_U8P(p)[4] << 24U) | \
               ((uint64_t) MG_U8P(p)[5] << 16U) | \
               ((uint64_t) MG_U8P(p)[6] << 8U) | MG_U8P(p)[7]))

// Write a native integer to byte pointer p in big-endian byte order.
// Safe on architectures that forbid unaligned access.
#define MG_STORE_BE16(p, n)               \
  do {                                    \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 8U); \
    MG_U8P(p)[1] = (uint8_t) (n);         \
  } while (0)
#define MG_STORE_BE24(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[2] = (uint8_t) (n);          \
  } while (0)
#define MG_STORE_BE32(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 24U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[2] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[3] = (uint8_t) (n);          \
  } while (0)
#define MG_STORE_BE64(p, n)                \
  do {                                     \
    MG_U8P(p)[0] = (uint8_t) ((n) >> 56U); \
    MG_U8P(p)[1] = (uint8_t) ((n) >> 48U); \
    MG_U8P(p)[2] = (uint8_t) ((n) >> 40U); \
    MG_U8P(p)[3] = (uint8_t) ((n) >> 32U); \
    MG_U8P(p)[4] = (uint8_t) ((n) >> 24U); \
    MG_U8P(p)[5] = (uint8_t) ((n) >> 16U); \
    MG_U8P(p)[6] = (uint8_t) ((n) >> 8U);  \
    MG_U8P(p)[7] = (uint8_t) (n);          \
  } while (0)

// Network / host byte-order conversion (big-endian <-> native).
// mg_htons/mg_htonl/mg_htonll are aliases for the same operation (symmetric).
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint64_t mg_ntohll(uint64_t net);
#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)
#define mg_htonll(x) mg_ntohll(x)

// Memory-mapped register access: reads/writes a volatile uint32_t at address x.
#define MG_REG(x) ((volatile uint32_t *) (x))[0]

// Produces a uint32_t with bit x set. x must be 0-31.
#define MG_BIT(x) (((uint32_t) 1U) << (x))

// Clears bits in CLRMASK and sets bits in SETMASK in register R atomically
// (read-modify-write). Example: MG_SET_BITS(MG_REG(addr), 0xF, 0x3);
#define MG_SET_BITS(R, CLRMASK, SETMASK) (R) = ((R) & ~(CLRMASK)) | (SETMASK)

// Round x up / down to the nearest multiple of a. Returns x when a is 0.
#define MG_ROUND_UP(x, a) ((a) == 0 ? (x) : ((((x) + (a) -1) / (a)) * (a)))
#define MG_ROUND_DOWN(x, a) ((a) == 0 ? (x) : (((x) / (a)) * (a)))

#if defined(__GNUC__) && defined(__arm__)
#ifdef __ZEPHYR__
#define MG_ARM_DISABLE_IRQ() __asm__ __volatile__("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() __asm__ __volatile__("cpsie i" : : : "memory")
#else
#define MG_ARM_DISABLE_IRQ() asm volatile("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() asm volatile("cpsie i" : : : "memory")
#endif // !ZEPHYR
#elif defined(__CCRH__)
#define MG_RH850_DISABLE_IRQ() __DI()
#define MG_RH850_ENABLE_IRQ() __EI()
#else
#define MG_ARM_DISABLE_IRQ()
#define MG_ARM_ENABLE_IRQ()
#endif

#if defined(__CC_ARM)
#define MG_DSB() __dsb(0xf)
#elif defined(__ARMCC_VERSION)
#define MG_DSB() __builtin_arm_dsb(0xf)
#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)
#ifdef __ZEPHYR__
#define MG_DSB() __asm__("DSB 0xf")
#else
#define MG_DSB() asm("DSB 0xf")
#endif // !ZEPHYR
#elif defined(__ICCARM__)
#define MG_DSB() __iar_builtin_DSB()
#else
#define MG_DSB()
#endif

struct mg_addr;
int mg_check_ip_acl(struct mg_str acl, struct mg_addr *remote_ip);

// Linked list management macros
#define LIST_ADD_HEAD(type_, head_, elem_) \
  do {                                     \
    (elem_)->next = (*head_);              \
    *(head_) = (elem_);                    \
  } while (0)

#define LIST_ADD_TAIL(type_, head_, elem_) \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != NULL) h = &(*h)->next;    \
    *h = (elem_);                          \
  } while (0)

#define LIST_DELETE(type_, head_, elem_)   \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != (elem_)) h = &(*h)->next; \
    *h = (elem_)->next;                    \
  } while (0)



// Returns the port from url. Uses scheme defaults when no port is explicit:
// http/ws=80, https/wss=443, mqtt=1883, mqtts=8883. Returns 0 if unknown.
unsigned short mg_url_port(const char *url);

// Returns true if url uses a TLS scheme (https, wss, mqtts, ssl, tls, tcps).
bool mg_url_is_ssl(const char *url);

// Returns the hostname from url as mg_str, without port or brackets.
// E.g. "http://foo.com:8080/x" -> "foo.com". Not NUL-terminated.
struct mg_str mg_url_host(const char *url);

// Returns the username from url (user:pass@host form) as mg_str.
// Returns an empty mg_str if no credentials are present. Not NUL-terminated.
struct mg_str mg_url_user(const char *url);

// Returns the password from url (user:pass@host form) as mg_str.
// Returns an empty mg_str if no credentials are present. Not NUL-terminated.
struct mg_str mg_url_pass(const char *url);

// Returns a pointer to the URI path in url (e.g. "/api/v1").
// Returns "/" if the url has no path. Points into the original url string.
const char *mg_url_uri(const char *url);




// Dynamically-sized I/O buffer. len <= size always holds.
struct mg_iobuf {
  unsigned char *buf;  // Heap-allocated data buffer
  size_t size;         // Allocated capacity (rounded up to align)
  size_t len;          // Number of bytes currently stored
  size_t align;        // Allocation granularity; capacity is always a multiple of this
};

// Initializes io to empty, then allocates size bytes with the given alignment.
// Returns false on allocation failure.
bool mg_iobuf_init(struct mg_iobuf *io, size_t size, size_t align);

// Resizes the buffer to new_size (rounded up to io->align). new_size=0 frees
// the buffer. Uses mg_calloc+mg_free (not realloc) so old memory is zeroed on release.
// Returns false on allocation failure.
bool mg_iobuf_resize(struct mg_iobuf *io, size_t new_size);

// Frees the buffer and zeroes the struct. Equivalent to mg_iobuf_resize(io, 0).
void mg_iobuf_free(struct mg_iobuf *io);

// Inserts len bytes from buf at offset ofs, shifting existing data right.
// Pass buf=NULL to reserve space without writing. Returns bytes inserted, 0 on OOM.
size_t mg_iobuf_add(struct mg_iobuf *io, size_t ofs, const void *buf, size_t len);

// Removes len bytes at ofs, shifting remaining data left. Clamps to available data.
// Returns bytes actually removed.
size_t mg_iobuf_del(struct mg_iobuf *io, size_t ofs, size_t len);


size_t mg_base64_update(unsigned char input_byte, char *buf, size_t len);
size_t mg_base64_final(char *buf, size_t len);
size_t mg_base64_encode(const unsigned char *p, size_t n, char *buf, size_t);
size_t mg_base64_decode(const char *src, size_t n, char *dst, size_t);




typedef struct {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} mg_md5_ctx;

void mg_md5_init(mg_md5_ctx *c);
void mg_md5_update(mg_md5_ctx *c, const unsigned char *data, size_t len);
void mg_md5_final(mg_md5_ctx *c, unsigned char[16]);




typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} mg_sha1_ctx;

void mg_sha1_init(mg_sha1_ctx *);
void mg_sha1_update(mg_sha1_ctx *, const unsigned char *data, size_t len);
void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *);
// https://github.com/B-Con/crypto-algorithms
// Author:     Brad Conte (brad AT bradconte.com)
// Disclaimer: This code is presented "as is" without any guarantees.
// Details:    Defines the API for the corresponding SHA1 implementation.
// Copyright:  public domain





typedef struct {
  uint32_t state[8];
  uint64_t bits;
  uint32_t len;
  unsigned char buffer[64];
} mg_sha256_ctx;


void mg_sha256_init(mg_sha256_ctx *);
void mg_sha256_update(mg_sha256_ctx *, const unsigned char *data, size_t len);
void mg_sha256_final(unsigned char digest[32], mg_sha256_ctx *);
void mg_sha256(uint8_t dst[32], uint8_t *data, size_t datasz);
void mg_hmac_sha256(uint8_t dst[32], uint8_t *key, size_t keysz, uint8_t *data,
                    size_t datasz);

typedef struct {
    uint64_t state[8];
    uint8_t buffer[128];
    uint64_t bitlen[2];
    uint32_t datalen;
} mg_sha384_ctx;
void mg_sha384_init(mg_sha384_ctx *ctx);
void mg_sha384_update(mg_sha384_ctx *ctx, const uint8_t *data, size_t len);
void mg_sha384_final(uint8_t digest[48], mg_sha384_ctx *ctx);
void mg_sha384(uint8_t dst[48], uint8_t *data, size_t datasz);



struct mg_connection;

// User-supplied event handler. ev is enum mg_event; ev_data type depends on
// the event (see enum below). c->fn_data is the user pointer passed to
// mg_listen(), mg_connect(), mg_*_listen(), mg_*_connect().
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data);

// Fires ev on connection c, invoking c->fn and c->pfn with ev_data.
void mg_call(struct mg_connection *c, int ev, void *ev_data);

// Sets c->is_closing and fires MG_EV_ERROR with a printf-formatted message.
void mg_error(struct mg_connection *c, const char *fmt, ...);

// Event codes passed to mg_event_handler_t. Each entry shows the type of
// ev_data cast to use inside the handler, e.g.: char *msg = (char *) ev_data;
enum mg_event {
  // Event          Meaning                         ev_data type
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_OPEN,       // Connection created           NULL
  MG_EV_POLL,       // mg_mgr_poll iteration        uint64_t *uptime_millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_TLS_HS,     // TLS handshake succeeded      NULL
  MG_EV_READ,       // Data received from socket    long *bytes_read
  MG_EV_WRITE,      // Data written to socket       long *bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_HDRS,  // HTTP headers                 struct mg_http_message *
  MG_EV_HTTP_MSG,   // Full HTTP request/response   struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           uint64_t *epoch_millis
  MG_EV_WAKEUP,     // mg_wakeup() data received    struct mg_str *data
  MG_EV_MDNS_REQ,   // mDNS request                 struct mg_mdns_req *
  MG_EV_MDNS_RESP,  // mDNS response                struct mg_mdns_resp *
  MG_EV_MODBUS_REQ, // Modbus TCP request            struct mg_modbus_req *
  MG_EV_USER        // Starting ID for user events
};










// DNS server configuration used by struct mg_mgr.
struct mg_dns {
  const char *url;          // DNS server URL, e.g. "udp://8.8.8.8:53"
  struct mg_connection *c;  // Active DNS connection; NULL when idle
};

// Network address: IPv4 or IPv6 address plus port. All values in network byte order.
struct mg_addr {
  union {      // IP address bytes in network byte order
    uint8_t ip[16];   // Raw 16-byte buffer; IPv4 uses the first 4 bytes
    uint32_t ip4;     // IPv4 address alias (same storage as ip[0..3])
    uint64_t ip6[2];  // IPv6 address as two 64-bit words
  } addr;
  uint16_t port;     // TCP or UDP port in network byte order
  uint8_t scope_id;  // IPv6 scope ID (from the %N suffix in the address)
  bool is_ip6;       // True when this address holds an IPv6 address
};

union mg_pipe {
  MG_SOCKET_TYPE fd;
  void *q;
};

// Central event manager. Zero-initialise with mg_mgr_init() before use.
struct mg_mgr {
  struct mg_connection *conns;  // Linked list of all open connections
  struct mg_dns dns4;           // IPv4 DNS server (default: 8.8.8.8:53)
  struct mg_dns dns6;           // IPv6 DNS server (default: [2001:4860:4860::8888]:53)
  struct mg_connection *mdns;   // mDNS listener connection, or NULL
  int dnstimeout;               // DNS resolve timeout in ms (default: 3000)
  bool use_dns6;                // If true, prefer DNS6 for hostname resolution
  unsigned long nextid;         // Auto-incrementing counter for connection IDs
  void *userdata;               // Arbitrary user pointer
  void *tls_ctx;                // Shared TLS context for all TLS connections
  uint16_t mqtt_id;             // Packet ID counter for MQTT pub/sub
  void *active_dns_requests;    // Pending DNS queries (internal)
  void *active_mdns_requests;   // Pending mDNS resolver queries (internal)
  struct mg_timer *timers;      // Linked list of active timers
  int epoll_fd;                 // epoll file descriptor; -1 when unused (MG_EPOLL_ENABLE=1)
  struct mg_tcpip_if *ifp;      // Builtin TCP/IP stack: network interface pointer
  size_t extraconnsize;         // Builtin TCP/IP stack: extra bytes allocated per connection
  union mg_pipe pipe;           // Socketpair write-end / queue, used by mg_wakeup()
#if MG_ENABLE_FREERTOS_TCP
  SocketSet_t ss;               // FreeRTOS-TCP socket set
#endif
};

// A single network connection. Do not allocate directly; use mg_connect() or
// mg_listen(). Each connection has an event handler fn called for every event.
struct mg_connection {
  struct mg_connection *next;     // Next connection in mgr->conns linked list
  struct mg_mgr *mgr;             // Manager that owns this connection
  struct mg_addr loc;             // Local address (IP + port)
  struct mg_addr rem;             // Remote address (IP + port)
  void *fd;                       // OS socket handle or LWIP PCB pointer
  unsigned long id;               // Unique, auto-incrementing connection ID
  struct mg_iobuf recv;           // Received data; consume in MG_EV_READ handler
  struct mg_iobuf send;           // Data to send; append with mg_send() / mg_printf()
  struct mg_iobuf prof;           // Profiling data (MG_ENABLE_PROFILE only)
  struct mg_iobuf rtls;           // TLS only: encrypted data buffered before decryption
  mg_event_handler_t fn;          // User event handler
  void *fn_data;                  // User-supplied argument passed to fn
  mg_event_handler_t pfn;         // Protocol-level handler (set by protocol modules)
  void *pfn_data;                 // Protocol-level handler argument
  char data[MG_DATA_SIZE];        // Scratch space for protocol state; freely readable
  void *tls;                      // TLS state (internal)
  unsigned is_listening : 1;      // Listening connection; accepts inbound connections
  unsigned is_client : 1;         // Outbound connection created by mg_connect()
  unsigned is_accepted : 1;       // Inbound connection accepted from a listener
  unsigned is_resolving : 1;      // DNS resolution is in progress
  unsigned is_arplooking : 1;     // ARP resolution is in progress (builtin TCP/IP)
  unsigned is_connecting : 1;     // Non-blocking TCP connect is in progress
  unsigned is_tls : 1;            // TLS connection
  unsigned is_tls_hs : 1;         // TLS handshake is in progress
  unsigned is_udp : 1;            // UDP connection (not TCP)
  unsigned is_websocket : 1;      // WebSocket connection
  unsigned is_mqtt5 : 1;          // MQTT5 connection (set by mg_mqtt_login)
  unsigned is_hexdumping : 1;     // Log hex dump of all in/out traffic
  unsigned is_draining : 1;       // Flush send buffer, then close
  unsigned is_closing : 1;        // Close and free immediately on next poll
  unsigned is_full : 1;           // Pause incoming reads until cleared
  unsigned is_tls_throttled : 1;  // TLS write was throttled; retry pending
  unsigned is_resp : 1;           // HTTP: response is still being generated
  unsigned is_readable : 1;       // Socket is ready to read (epoll/select)
  unsigned is_writable : 1;       // Socket is ready to write (epoll/select)
};

// Runs one iteration of the event loop.
//
// Example:
//   while (keep_running) mg_mgr_poll(&mgr, 50);
// Full examples:
//   tutorials/http/http-server, tutorials/mqtt/mqtt-client,
//   tutorials/websocket/websocket-server, tutorials/core/timers
// Related APIs:
//   mg_mgr_init(), mg_mgr_free(), mg_timer_add(), mg_wakeup()
// Notes:
//   Waits up to ms milliseconds for I/O events; use ms=0 to return
//   immediately. Calls event handlers for ready connections and fires expired
//   timers. Call repeatedly from the main loop or a dedicated network task.
void mg_mgr_poll(struct mg_mgr *, int ms);

// Initialises an event manager before use.
//
// Example:
//   struct mg_mgr mgr;
//   mg_mgr_init(&mgr);
// Full examples:
//   tutorials/http/http-server, tutorials/mqtt/mqtt-client,
//   tutorials/websocket/websocket-server, tutorials/core/embedded-filesystem
// Related APIs:
//   mg_mgr_poll(), mg_mgr_free(), mg_http_listen(), mg_connect()
// Notes:
//   Sets safe defaults such as DNS servers, DNS timeout, epoll/SIGPIPE setup,
//   and TLS context. Call before creating connections, listeners, or timers.
//   Set mgr.userdata after this call if your application needs it.
//   On embedded systems, if the built-in TCP/IP stack is enabled
//   (MG_ENABLE_TCPIP=1) and one built-in driver is selected, e.g.
//   MG_ENABLE_DRIVER_STM32H=1, mg_mgr_init() also calls mg_tcpip_init().
//   If no driver is selected, automatic driver init is disabled, or multiple
//   interfaces are intended, call mg_tcpip_init() separately.
void mg_mgr_init(struct mg_mgr *);

// Closes all connections, frees all timers, and releases the TLS context.
// Safe to call on a partially initialised mgr.
void mg_mgr_free(struct mg_mgr *);

// Creates a listening connection on url (e.g. "tcp://0.0.0.0:8080").
// Fires MG_EV_OPEN on the listener, then MG_EV_ACCEPT for each new client.
// Returns NULL on error.
struct mg_connection *mg_listen(struct mg_mgr *, const char *url,
                                mg_event_handler_t fn, void *fn_data);

// Opens an outbound connection to url (e.g. "tcp://example.com:80").
// DNS resolution is asynchronous. Fires MG_EV_RESOLVE, then MG_EV_CONNECT
// when the connection is established (or MG_EV_ERROR on failure).
// Returns NULL on error (OOM, network down, null url).
struct mg_connection *mg_connect(struct mg_mgr *, const char *url,
                                 mg_event_handler_t fn, void *fn_data);

// Wraps an existing OS file descriptor in a Mongoose connection.
// The connection is added to mgr and fires MG_EV_OPEN immediately.
// Useful for integrating pre-opened sockets (e.g. stdin, accept() fd).
struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
                                mg_event_handler_t fn, void *fn_data);

// Called internally after DNS resolution completes to create the OS socket
// and initiate the TCP/IP connect. Normally not called by user code.
void mg_connect_resolved(struct mg_connection *);

// Appends buf/len to c->send. Returns true on success, false on OOM
// (which also fires MG_EV_ERROR and schedules the connection for close).
// Data is sent asynchronously by the next mg_mgr_poll() call.
bool mg_send(struct mg_connection *, const void *, size_t);

// Formats data and appends it to a connection send buffer.
//
// Returns:
//   Number of bytes appended, or 0 on OOM.
// Example:
//   mg_printf(c, "GET / HTTP/1.0\r\nHost: %.*s\r\n\r\n",
//             (int) host.len, host.buf);
// Full examples:
//   tutorials/http/http-client, tutorials/http/http-proxy-client,
//   tutorials/http/http-restful-server
// Related APIs:
//   mg_send(), mg_snprintf(), mg_http_reply(), mg_ws_printf()
// Notes:
//   Data is sent asynchronously by a later mg_mgr_poll() call. Supports
//   mg_xprintf specifiers, including custom %M/%m printers. See src/fmt.h for
//   the full specifier list.
size_t mg_printf(struct mg_connection *, const char *fmt, ...);
size_t mg_vprintf(struct mg_connection *, const char *fmt, va_list *ap);

// Parses an IP address string str into addr. Handles "localhost", plain IPv4
// (e.g. "1.2.3.4"), IPv6 with optional brackets and scope ID, and
// IPv4-mapped IPv6 (::ffff:1.2.3.4). Does NOT parse the port.
// Returns true on success; addr->is_ip6 and addr->addr are set.
bool mg_aton(struct mg_str str, struct mg_addr *addr);

// Low-level functions for integrating with custom network stacks.
// Allocates a new connection struct with mgr->extraconnsize extra bytes.
// Returns NULL on OOM.
struct mg_connection *mg_alloc_conn(struct mg_mgr *);

// Fires MG_EV_CLOSE, frees all buffers (recv/send/rtls), removes c from
// mgr->conns, and calls mg_free(c). Do not use c after this call.
void mg_close_conn(struct mg_connection *c);

// Creates and binds the OS socket for a listening connection c at url.
// Called internally by mg_listen(); expose for custom network stack use.
// Returns false on error.
bool mg_open_listener(struct mg_connection *c, const char *url);

// Thread-safe wakeup: fires MG_EV_WAKEUP on the connection whose id matches,
// with ev_data pointing to an mg_str containing buf/len.
// Requires mg_wakeup_init() to have been called first.
// Returns false if the pipe is not initialised or conn_id is 0.
// Safe to call from any thread or interrupt context.
bool mg_wakeup(struct mg_mgr *, unsigned long id, const void *buf, size_t len);

// Initialises the internal socketpair used by mg_wakeup(). Call once after
// mg_mgr_init() if you intend to wake the event loop from another thread.
// Returns false on failure (e.g. socketpair creation failed).
bool mg_wakeup_init(struct mg_mgr *);

// Deprecated API, do not expose. Use mg_timer_expired instead
struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg);

// Like mg_connect() but also sets a protocol-level handler pfn/pfn_data.
// Used internally by protocol modules (MQTT, WebSocket, etc.).
struct mg_connection *mg_connect_svc(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data,
                                     mg_event_handler_t pfn, void *pfn_data);

// Restores c->rem from a saved address buffer written by the caller, then
// maps the IP back through the builtin TCP/IP stack. Used after a UDP
// multicast send to restore the connection's original remote address.
void mg_multicast_restore(struct mg_connection *c, uint8_t *from);








// A single HTTP header name/value pair.
struct mg_http_header {
  struct mg_str name;   // Header name, e.g. "Content-Type"
  struct mg_str value;  // Header value, e.g. "text/html"
};

// Parsed HTTP request or response.
// Passed as ev_data in MG_EV_HTTP_MSG and MG_EV_HTTP_HDRS.
// For requests:  method="GET", uri="/path", proto="a=1&b=2", proto="HTTP/1.1"
// For responses: method="HTTP/1.0", uri="200", query="",  proto="OK"
struct mg_http_message {
  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Parsed headers array
  struct mg_str body;     // Request or response body
  struct mg_str head;     // Raw bytes: request/status line + headers, no body
  struct mg_str message;  // Raw bytes: head + body
};

// Options for mg_http_serve_dir() and mg_http_serve_file().
struct mg_http_serve_opts {
  const char *root_dir;       // Root directory for serving files, e.g. "/web"
  const char *ssi_pattern;    // Pattern for Server-Side Include files, e.g. "#.shtml". NULL disables SSI
  const char *extra_headers;  // Extra response headers, e.g. "A: b\r\nC: d\r\n". NULL for none
  const char *mime_types;     // Additional MIME types: "ext1=type1,ext2=type2". NULL for defaults only
  const char *page404;        // Path to a custom 404 page, e.g. "/404.html". NULL for built-in
  struct mg_fs *fs;           // Filesystem to use. NULL defaults to POSIX
};

// A single part of a multipart/form-data body, filled by mg_http_next_multipart().
struct mg_http_part {
  struct mg_str name;      // Form field name, from Content-Disposition: name="..."
  struct mg_str filename;  // Original filename for file uploads, empty for plain fields
  struct mg_str body;      // Part contents, points into the original message buffer
};

// Parses HTTP request or response in buffer s/len into hm.
// Returns number of bytes consumed, 0 if incomplete, -1 on error.
int mg_http_parse(const char *s, size_t len, struct mg_http_message *);

// Returns the byte length of HTTP headers in buf (i.e. offset of the body).
// Returns 0 if headers are incomplete, -1 on error.
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);

// Sends one chunk in HTTP chunked transfer encoding using printf-style fmt.
// Call with empty fmt ("") to send the terminating zero-length chunk.
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);

// Sends one chunk in HTTP chunked transfer encoding from a raw buffer.
// Call with len=0 to send the terminating zero-length chunk.
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);

// Creates an HTTP server on url, e.g. "http://0.0.0.0:8000".
//
// Returns:
//   Listening connection, or NULL on error.
// Example:
//   mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL);
// Full examples:
//   tutorials/http/http-server, tutorials/http/*
// Related APIs:
//   mg_http_reply(), mg_http_serve_dir(), mg_match()
// Notes:
//   Call mg_mgr_poll() in the main loop. The user-supplied fn event handler
//   receives normal connection events. It also receives MG_EV_HTTP_HDRS when
//   headers are received and MG_EV_HTTP_MSG when the full request is received.
//   ev_data for both HTTP events is struct mg_http_message *.
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);

// Opens an HTTP client connection to url, e.g. "http://example.org".
//
// Returns:
//   Client connection, or NULL on error.
// Example:
//   mg_http_connect(&mgr, "http://example.org", fn, NULL);
// Full examples:
//   tutorials/http/http-client, tutorials/http/huge-response
// Related APIs:
//   mg_printf(), mg_http_status(), mg_http_get_header()
// Notes:
//   In the user-supplied fn event handler, send the HTTP request on
//   MG_EV_CONNECT. The handler receives normal connection events. It also
//   receives MG_EV_HTTP_HDRS when headers are received and MG_EV_HTTP_MSG when
//   the full response is received. ev_data for both HTTP events is struct
//   mg_http_message *.
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);

// Serves static files from a directory.
//
// Example:
//   struct mg_http_serve_opts o = {.root_dir = "web_root", .fs = &mg_fs_posix};
//   mg_http_serve_dir(c, hm, &o);
// Full examples:
//   tutorials/http/http-server, tutorials/core/embedded-filesystem
// Related APIs:
//   mg_http_listen(), mg_http_serve_file(), mg_http_reply(), mg_match()
// Notes:
//   Call from an MG_EV_HTTP_MSG handler. The uri in hm is mapped under
//   opts->root_dir. Directory listing depends on MG_ENABLE_DIRLIST; SSI uses
//   opts->ssi_pattern when configured.
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *);

// Serves a single file at path. Call from an MG_EV_HTTP_MSG handler.
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, const struct mg_http_serve_opts *);

// Sends a complete HTTP response with Content-Length.
//
// Example:
//   mg_http_reply(c, 200, "Content-Type: application/json\r\n",
//                 "{%m:%d}", MG_ESC("temperature"), 123);
// Full examples:
//   tutorials/http/http-server, tutorials/http/http-restful-server,
//   tutorials/http/device-dashboard
// Related APIs:
//   mg_http_listen(), mg_printf(), mg_http_printf_chunk(), MG_ESC
// Notes:
//   headers must end with "\r\n"; pass "" or NULL for no extra headers.
//   body_fmt is printf-style and supports %M/%m custom printers. Use MG_ESC
//   when printing JSON strings.
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
                   const char *body_fmt, ...);

// Looks up a request header by name (case-insensitive).
// Returns a pointer to the mg_str value, or NULL if not found.
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);

// Extracts a named variable from a query string or form-encoded body (buf).
// Returns the raw (URL-encoded) value as mg_str, or an empty mg_str if not found.
// Not NUL-terminated. Use mg_url_decode() to get a decoded string.
struct mg_str mg_http_var(struct mg_str buf, struct mg_str name);

// URL-decodes variable name from query string or form body into a NUL-terminated buffer.
// Returns the length of the decoded value, or negative if not found / buffer too small.
int mg_http_get_var(const struct mg_str *, const char *name, char *, size_t);

// URL-decodes s/n into to/to_len. Set form=1 to also decode '+' as space.
// Returns number of bytes written, or -1 if to buffer is too small.
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);

// URL-encodes s/n into buf/len. Returns number of bytes written.
size_t mg_url_encode(const char *s, size_t n, char *buf, size_t len);

// Extracts HTTP credentials into caller-supplied buffers.
// For Basic auth: fills user (buf1) and password (buf2).
// For Bearer auth: fills buf1 with "" and buf2 with the token.
void mg_http_creds(struct mg_http_message *, char *, size_t, char *, size_t);

// Deprecated API, do not expose
long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_fs *fs, const char *dir, size_t max_size);

// Streams the raw request body into dir/name via fs. Non-blocking: installs an
// internal handler and returns immediately. fn is called with NULL on success
// or an error string on failure.
void mg_http_start_upload(struct mg_connection *c, struct mg_http_message *hm,
                          struct mg_str name, struct mg_str dir,
                          struct mg_fs *fs,
                          void (*fn)(struct mg_connection *, const char *));

// Starts an OTA firmware update from an HTTP POST upload.
// Calls mg_ota_begin/write/end internally; fn is called with NULL on success,
// or an error string on failure.
void mg_http_start_ota(struct mg_connection *c, struct mg_http_message *hm,
                       void (*fn)(struct mg_connection *, const char *));

// Sends a 401 Unauthorized response with a Basic Auth WWW-Authenticate challenge.
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);

// Extracts a named parameter from a header value string s.
// E.g. for s="multipart/form-data; boundary=abc", v="boundary" returns "abc".
struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v);

// Iterates over parts of a multipart/form-data body.
//
// Returns:
//   Offset for the next call, or 0 when there are no more parts.
// Example:
//   size_t ofs = 0;
//   struct mg_http_part part;
//   while ((ofs = mg_http_next_multipart(hm->body, ofs, &part)) > 0) { ... }
// Full examples:
//   tutorials/http/file-upload-html-form, tutorials/http/http-server
// Related APIs:
//   mg_http_listen(), mg_http_start_upload()
// Notes:
//   Call from an MG_EV_HTTP_MSG handler after the full request body is
//   received. part.name, part.filename, and part.body are zero-copy slices into
//   hm->body and are not NUL-terminated.
size_t mg_http_next_multipart(struct mg_str, size_t, struct mg_http_part *);

// Returns the HTTP status code from a parsed response message (e.g. 200, 404).
int mg_http_status(const struct mg_http_message *hm);


void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath);
// TLS / Security


// Available TLS backend libraries
#define MG_TLS_NONE 0     // No TLS support
#define MG_TLS_MBED 1     // mbedTLS
#define MG_TLS_OPENSSL 2  // OpenSSL
#define MG_TLS_WOLFSSL 5  // WolfSSL (based on OpenSSL)
#define MG_TLS_BUILTIN 3  // Built-in
#define MG_TLS_CUSTOM 4   // Custom implementation

#ifndef MG_TLS
// mongoose_config.h setting. Set MG_TLS to one of the MG_TLS_* values above to
// select a TLS backend. Defaults to MG_TLS_NONE (no TLS).
#define MG_TLS MG_TLS_NONE
#endif





// TLS options structure passed to mg_tls_init(). All cert/key fields accept
// PEM strings or DER binary.
//
// One-way TLS:
// server sets `cert` + `key`,
// client sets `ca` + optionally `name` for hostname verification.
//
// Two-way (mutual) TLS: both sides set `ca` + `cert` + `key`.
//
// - `ca`: CA certificate. Verifies the peer's certificate.
//   Set on clients to authenticate the server. Set on servers to require
//   and verify a client certificate. If empty, peer is not verified.
// - `cert`: Our certificate. Required on servers. Also set on clients for
//   mutual TLS.
// - `key`: Our private key. May equal `cert` when PEM bundles both.
// - `name`: Server name for SNI and hostname verification. Set on clients.
//   Empty disables hostname verification.
// - `skip_verification`: Skip certificate and hostname verification.
//   Useful during development; do not use in production.
struct mg_tls_opts {
  struct mg_str ca;       // CA certificate, PEM or DER
  struct mg_str cert;     // Our certificate, PEM or DER
  struct mg_str key;      // Our private key, PEM or DER
  struct mg_str name;     // Server name for SNI + hostname verification
  bool skip_verification;  // Skip certificate and hostname verification
};

// Initialises TLS on a connection.
//
// Example:
// ```c
// // Server: one-way TLS
// if (ev == MG_EV_ACCEPT) {
//   struct mg_tls_opts opts = {.cert = mg_str(s_tls_cert),
//                              .key  = mg_str(s_tls_key)};
//   mg_tls_init(c, &opts);
// }
//
// // Client: verify server certificate and hostname
// if (ev == MG_EV_CONNECT) {
//   struct mg_tls_opts opts = {.ca   = mg_str(s_tls_ca),
//                              .name = mg_str("hostname")};
//   mg_tls_init(c, &opts);
// }
// ```
// Full examples:
//   tutorials/http/http-server, tutorials/http/http-client,
//   tutorials/mqtt/mqtt-client-aws-iot, tutorials/websocket/websocket-client
// Related APIs:
//   mg_http_listen(), mg_http_connect(), mg_ws_connect(), mg_mqtt_connect()
// Notes:
//   Call from the user-supplied event handler on MG_EV_ACCEPT for servers or
//   MG_EV_CONNECT for clients, before application data is sent. Servers usually
//   set cert and key. Clients usually set ca and name; name enables SNI and
//   hostname verification.
void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts);

// Private API, do not expose
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_flush(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);
void mg_tls_ctx_init(struct mg_mgr *);
void mg_tls_ctx_free(struct mg_mgr *);
#define MG_IS_DER(buf) (((uint8_t *) (buf))[0] == 0x30)  // DER begins with 0x30

// Low-level IO primitives used by TLS layer
enum { MG_IO_ERR = -1, MG_IO_WAIT = -2 };
long mg_io_send(struct mg_connection *c, const void *buf, size_t len);
long mg_io_recv(struct mg_connection *c, void *buf, size_t len);
#ifndef TLS_X15519_H
#define TLS_X15519_H



#define X25519_BYTES 32
extern const uint8_t X25519_BASE_POINT[X25519_BYTES];

int mg_tls_x25519(uint8_t out[X25519_BYTES], const uint8_t scalar[X25519_BYTES],
                  const uint8_t x1[X25519_BYTES], int clamp);


#endif /* TLS_X15519_H */
/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of AES-GCM authenticated
 * encryption. The focus of this work was correctness & accuracy. It is written
 * in straight 'C' without any particular focus upon optimization or speed. It
 * should be endian (memory byte order) neutral since the few places that care
 * are handled explicitly.
 *
 * This implementation of AES-GCM was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
 *         http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ \
 *         gcm/gcm-revised-spec.pdf
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/
#ifndef TLS_AES128_H
#define TLS_AES128_H

/******************************************************************************
 *  AES_CONTEXT : cipher context / holds inter-call data
 ******************************************************************************/
typedef struct {
  int mode;          // 1 for Encryption, 0 for Decryption
  int rounds;        // keysize-based rounds count
  uint32_t *rk;      // pointer to current round key
  uint32_t buf[68];  // key expansion buffer
} aes_context;


#define GCM_AUTH_FAILURE 0x55555555  // authentication failure

/******************************************************************************
 *  GCM_CONTEXT : MUST be called once before ANY use of this library
 ******************************************************************************/
int mg_gcm_initialize(void);

//
//  aes-gcm.h
//  MKo
//
//  Created by Markus Kosmal on 20/11/14.
//
//
int mg_aes_gcm_encrypt(unsigned char *output, const unsigned char *input,
                       size_t input_length, const unsigned char *key,
                       const size_t key_len, const unsigned char *iv,
                       const size_t iv_len, unsigned char *aead,
                       size_t aead_len, unsigned char *tag,
                       const size_t tag_len);

int mg_aes_gcm_decrypt(unsigned char *output, const unsigned char *input,
                       size_t input_length, const unsigned char *key,
                       const size_t key_len, const unsigned char *iv,
                       const size_t iv_len, unsigned char *aead,
                       size_t aead_len, const unsigned char *tag,
                       const size_t tag_len);

#endif /* TLS_AES128_H */

// End of aes128 PD



/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_H_
#define _UECC_H_

/* Platform selection options.
If MG_UECC_PLATFORM is not defined, the code will try to guess it based on
compiler macros. Possible values for MG_UECC_PLATFORM are defined below: */
#define mg_uecc_arch_other 0
#define mg_uecc_x86 1
#define mg_uecc_x86_64 2
#define mg_uecc_arm 3
#define mg_uecc_arm_thumb 4
#define mg_uecc_arm_thumb2 5
#define mg_uecc_arm64 6
#define mg_uecc_avr 7

/* If desired, you can define MG_UECC_WORD_SIZE as appropriate for your platform
(1, 4, or 8 bytes). If MG_UECC_WORD_SIZE is not explicitly defined then it will
be automatically set based on your platform. */

/* Optimization level; trade speed for code size.
   Larger values produce code that is faster but larger.
   Currently supported values are 0 - 4; 0 is unusably slow for most
   applications. Optimization level 4 currently only has an effect ARM platforms
   where more than one curve is enabled. */
#ifndef MG_UECC_OPTIMIZATION_LEVEL
#define MG_UECC_OPTIMIZATION_LEVEL 2
#endif

/* MG_UECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a
specific function to be used for (scalar) squaring instead of the generic
multiplication function. This can make things faster somewhat faster, but
increases the code size. */
#ifndef MG_UECC_SQUARE_FUNC
#define MG_UECC_SQUARE_FUNC 0
#endif

/* MG_UECC_VLI_NATIVE_LITTLE_ENDIAN - If enabled (defined as nonzero), this will
switch to native little-endian format for *all* arrays passed in and out of the
public API. This includes public and private keys, shared secrets, signatures
and message hashes. Using this switch reduces the amount of call stack memory
used by uECC, since less intermediate translations are required. Note that this
will *only* work on native little-endian processors and it will treat the
uint8_t arrays passed into the public API as word arrays, therefore requiring
the provided byte arrays to be word aligned on architectures that do not support
unaligned accesses. IMPORTANT: Keys and signatures generated with
MG_UECC_VLI_NATIVE_LITTLE_ENDIAN=1 are incompatible with keys and signatures
generated with MG_UECC_VLI_NATIVE_LITTLE_ENDIAN=0; all parties must use the same
endianness. */
#ifndef MG_UECC_VLI_NATIVE_LITTLE_ENDIAN
#define MG_UECC_VLI_NATIVE_LITTLE_ENDIAN 0
#endif

/* Curve support selection. Set to 0 to remove that curve. */
#ifndef MG_UECC_SUPPORTS_secp160r1
#define MG_UECC_SUPPORTS_secp160r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp192r1
#define MG_UECC_SUPPORTS_secp192r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp224r1
#define MG_UECC_SUPPORTS_secp224r1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp256r1
#define MG_UECC_SUPPORTS_secp256r1 1
#endif
#ifndef MG_UECC_SUPPORTS_secp256k1
#define MG_UECC_SUPPORTS_secp256k1 0
#endif
#ifndef MG_UECC_SUPPORTS_secp384r1
#define MG_UECC_SUPPORTS_secp384r1 1
#endif

/* Specifies whether compressed point format is supported.
   Set to 0 to disable point compression/decompression functions. */
#ifndef MG_UECC_SUPPORT_COMPRESSED_POINT
#define MG_UECC_SUPPORT_COMPRESSED_POINT 1
#endif

struct MG_UECC_Curve_t;
typedef const struct MG_UECC_Curve_t *MG_UECC_Curve;

#ifdef __cplusplus
extern "C" {
#endif

#if MG_UECC_SUPPORTS_secp160r1
MG_UECC_Curve mg_uecc_secp160r1(void);
#endif
#if MG_UECC_SUPPORTS_secp192r1
MG_UECC_Curve mg_uecc_secp192r1(void);
#endif
#if MG_UECC_SUPPORTS_secp224r1
MG_UECC_Curve mg_uecc_secp224r1(void);
#endif
#if MG_UECC_SUPPORTS_secp256r1
MG_UECC_Curve mg_uecc_secp256r1(void);
#endif
#if MG_UECC_SUPPORTS_secp256k1
MG_UECC_Curve mg_uecc_secp256k1(void);
#endif
#if MG_UECC_SUPPORTS_secp384r1
MG_UECC_Curve mg_uecc_secp384r1(void);
#endif

/* MG_UECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1
if 'dest' was filled with random data, or 0 if the random data could not be
generated. The filled-in values should be either truly random, or from a
cryptographically-secure PRNG.

A correctly functioning RNG function must be set (using mg_uecc_set_rng())
before calling mg_uecc_make_key() or mg_uecc_sign().

Setting a correctly functioning RNG function improves the resistance to
side-channel attacks for mg_uecc_shared_secret() and
mg_uecc_sign_deterministic().

A correct RNG function is set by default when building for Windows, Linux, or OS
X. If you are building on another POSIX-compliant system that supports
/dev/random or /dev/urandom, you can define MG_UECC_POSIX to use the predefined
RNG. For embedded platforms there is no predefined RNG function; you must
provide your own.
*/
typedef int (*MG_UECC_RNG_Function)(uint8_t *dest, unsigned size);

/* mg_uecc_set_rng() function.
Set the function that will be used to generate random bytes. The RNG function
should return 1 if the random data was generated, or 0 if the random data could
not be generated.

On platforms where there is no predefined RNG function (eg embedded platforms),
this must be called before mg_uecc_make_key() or mg_uecc_sign() are used.

Inputs:
    rng_function - The function that will be used to generate random bytes.
*/
void mg_uecc_set_rng(MG_UECC_RNG_Function rng_function);

/* mg_uecc_get_rng() function.

Returns the function that will be used to generate random bytes.
*/
MG_UECC_RNG_Function mg_uecc_get_rng(void);

/* mg_uecc_curve_private_key_size() function.

Returns the size of a private key for the curve in bytes.
*/
int mg_uecc_curve_private_key_size(MG_UECC_Curve curve);

/* mg_uecc_curve_public_key_size() function.

Returns the size of a public key for the curve in bytes.
*/
int mg_uecc_curve_public_key_size(MG_UECC_Curve curve);

/* mg_uecc_make_key() function.
Create a public/private key pair.

Outputs:
    public_key  - Will be filled in with the public key. Must be at least 2 *
the curve size (in bytes) long. For example, if the curve is secp256r1,
public_key must be 64 bytes long. private_key - Will be filled in with the
private key. Must be as long as the curve order; this is typically the same as
the curve size, except for secp160r1. For example, if the curve is secp256r1,
private_key must be 32 bytes long.

                  For secp160r1, private_key must be 21 bytes long! Note that
the first byte will almost always be 0 (there is about a 1 in 2^80 chance of it
being non-zero).

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int mg_uecc_make_key(uint8_t *public_key, uint8_t *private_key,
                     MG_UECC_Curve curve);

/* mg_uecc_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key. If
the public key is not from a trusted source and has not been previously
verified, you should verify it first using mg_uecc_valid_public_key(). Note: It
is recommended that you hash the result of mg_uecc_shared_secret() before using
it for symmetric encryption or HMAC.

Inputs:
    public_key  - The public key of the remote party.
    private_key - Your private key.

Outputs:
    secret - Will be filled in with the shared secret value. Must be the same
size as the curve size; for example, if the curve is secp256r1, secret must be
32 bytes long.

Returns 1 if the shared secret was generated successfully, 0 if an error
occurred.
*/
int mg_uecc_shared_secret(const uint8_t *public_key, const uint8_t *private_key,
                          uint8_t *secret, MG_UECC_Curve curve);

#if MG_UECC_SUPPORT_COMPRESSED_POINT
/* mg_uecc_compress() function.
Compress a public key.

Inputs:
    public_key - The public key to compress.

Outputs:
    compressed - Will be filled in with the compressed public key. Must be at
least (curve size + 1) bytes long; for example, if the curve is secp256r1,
                 compressed must be 33 bytes long.
*/
void mg_uecc_compress(const uint8_t *public_key, uint8_t *compressed,
                      MG_UECC_Curve curve);

/* mg_uecc_decompress() function.
Decompress a compressed public key.

Inputs:
    compressed - The compressed public key.

Outputs:
    public_key - Will be filled in with the decompressed public key.
*/
void mg_uecc_decompress(const uint8_t *compressed, uint8_t *public_key,
                        MG_UECC_Curve curve);
#endif /* MG_UECC_SUPPORT_COMPRESSED_POINT */

/* mg_uecc_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any
other uECC functions. However, you may wish to avoid spending CPU time computing
a shared secret or verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int mg_uecc_valid_public_key(const uint8_t *public_key, MG_UECC_Curve curve);

/* mg_uecc_compute_public_key() function.
Compute the corresponding public key for a private key.

Inputs:
    private_key - The private key to compute the public key for

Outputs:
    public_key - Will be filled in with the corresponding public key

Returns 1 if the key was computed successfully, 0 if an error occurred.
*/
int mg_uecc_compute_public_key(const uint8_t *private_key, uint8_t *public_key,
                               MG_UECC_Curve curve);

/* mg_uecc_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it in to this function along with your private key.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.

Outputs:
    signature - Will be filled in with the signature value. Must be at least 2 *
curve size long. For example, if the curve is secp256r1, signature must be 64
bytes long.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int mg_uecc_sign(const uint8_t *private_key, const uint8_t *message_hash,
                 unsigned hash_size, uint8_t *signature, MG_UECC_Curve curve);

/* MG_UECC_HashContext structure.
This is used to pass in an arbitrary hash function to
mg_uecc_sign_deterministic(). The structure will be used for multiple hash
computations; each time a new hash is computed, init_hash() will be called,
followed by one or more calls to update_hash(), and finally a call to
finish_hash() to produce the resulting hash.

The intention is that you will create a structure that includes
MG_UECC_HashContext followed by any hash-specific data. For example:

typedef struct SHA256_HashContext {
    MG_UECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

void init_SHA256(MG_UECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Init(&context->ctx);
}

void update_SHA256(MG_UECC_HashContext *base,
                   const uint8_t *message,
                   unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Update(&context->ctx, message, message_size);
}

void finish_SHA256(MG_UECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Final(hash_result, &context->ctx);
}

... when signing ...
{
    uint8_t tmp[32 + 32 + 64];
    SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64,
32, tmp}}; mg_uecc_sign_deterministic(key, message_hash, &ctx.uECC, signature);
}
*/
typedef struct MG_UECC_HashContext {
  void (*init_hash)(const struct MG_UECC_HashContext *context);
  void (*update_hash)(const struct MG_UECC_HashContext *context,
                      const uint8_t *message, unsigned message_size);
  void (*finish_hash)(const struct MG_UECC_HashContext *context,
                      uint8_t *hash_result);
  unsigned
      block_size; /* Hash function block size in bytes, eg 64 for SHA-256. */
  unsigned
      result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
  uint8_t *tmp;    /* Must point to a buffer of at least (2 * result_size +
                      block_size) bytes. */
} MG_UECC_HashContext;

/* mg_uecc_sign_deterministic() function.
Generate an ECDSA signature for a given hash value, using a deterministic
algorithm (see RFC 6979). You do not need to set the RNG using mg_uecc_set_rng()
before calling this function; however, if the RNG is defined it will improve
resistance to side-channel attacks.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it to this function along with your private key and a hash context. Note
that the message_hash does not need to be computed with the same hash function
used by hash_context.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.
    hash_context - A hash context to use.

Outputs:
    signature - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int mg_uecc_sign_deterministic(const uint8_t *private_key,
                               const uint8_t *message_hash, unsigned hash_size,
                               const MG_UECC_HashContext *hash_context,
                               uint8_t *signature, MG_UECC_Curve curve);

/* mg_uecc_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature
values (r and s).

Inputs:
    public_key   - The signer's public key.
    message_hash - The hash of the signed data.
    hash_size    - The size of message_hash in bytes.
    signature    - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/
int mg_uecc_verify(const uint8_t *public_key, const uint8_t *message_hash,
                   unsigned hash_size, const uint8_t *signature,
                   MG_UECC_Curve curve);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_TYPES_H_
#define _UECC_TYPES_H_

#ifndef MG_UECC_PLATFORM
#if defined(__AVR__) && __AVR__
#define MG_UECC_PLATFORM mg_uecc_avr
#elif defined(__thumb2__) || \
    defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
#define MG_UECC_PLATFORM mg_uecc_arm_thumb2
#elif defined(__thumb__)
#define MG_UECC_PLATFORM mg_uecc_arm_thumb
#elif defined(__arm__) || defined(_M_ARM)
#define MG_UECC_PLATFORM mg_uecc_arm
#elif defined(__aarch64__)
#define MG_UECC_PLATFORM mg_uecc_arm64
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || \
    defined(__I86__)
#define MG_UECC_PLATFORM mg_uecc_x86
#elif defined(__amd64__) || defined(_M_X64)
#define MG_UECC_PLATFORM mg_uecc_x86_64
#else
#define MG_UECC_PLATFORM mg_uecc_arch_other
#endif
#endif

#ifndef MG_UECC_ARM_USE_UMAAL
#if (MG_UECC_PLATFORM == mg_uecc_arm) && (__ARM_ARCH >= 6)
#define MG_UECC_ARM_USE_UMAAL 1
#elif (MG_UECC_PLATFORM == mg_uecc_arm_thumb2) && (__ARM_ARCH >= 6) && \
    (!defined(__ARM_ARCH_7M__) || !__ARM_ARCH_7M__)
#define MG_UECC_ARM_USE_UMAAL 1
#else
#define MG_UECC_ARM_USE_UMAAL 0
#endif
#endif

#ifndef MG_UECC_WORD_SIZE
#if MG_UECC_PLATFORM == mg_uecc_avr
#define MG_UECC_WORD_SIZE 1
#elif (MG_UECC_PLATFORM == mg_uecc_x86_64 || MG_UECC_PLATFORM == mg_uecc_arm64)
#define MG_UECC_WORD_SIZE 8
#else
#define MG_UECC_WORD_SIZE 4
#endif
#endif

#if (MG_UECC_WORD_SIZE != 1) && (MG_UECC_WORD_SIZE != 4) && \
    (MG_UECC_WORD_SIZE != 8)
#error "Unsupported value for MG_UECC_WORD_SIZE"
#endif

#if ((MG_UECC_PLATFORM == mg_uecc_avr) && (MG_UECC_WORD_SIZE != 1))
#pragma message("MG_UECC_WORD_SIZE must be 1 for AVR")
#undef MG_UECC_WORD_SIZE
#define MG_UECC_WORD_SIZE 1
#endif

#if ((MG_UECC_PLATFORM == mg_uecc_arm ||         \
      MG_UECC_PLATFORM == mg_uecc_arm_thumb ||   \
      MG_UECC_PLATFORM == mg_uecc_arm_thumb2) && \
     (MG_UECC_WORD_SIZE != 4))
#pragma message("MG_UECC_WORD_SIZE must be 4 for ARM")
#undef MG_UECC_WORD_SIZE
#define MG_UECC_WORD_SIZE 4
#endif

typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
typedef int8_t cmpresult_t;

#if (MG_UECC_WORD_SIZE == 1)

typedef uint8_t mg_uecc_word_t;
typedef uint16_t mg_uecc_dword_t;

#define HIGH_BIT_SET 0x80
#define MG_UECC_WORD_BITS 8
#define MG_UECC_WORD_BITS_SHIFT 3
#define MG_UECC_WORD_BITS_MASK 0x07

#elif (MG_UECC_WORD_SIZE == 4)

typedef uint32_t mg_uecc_word_t;
typedef uint64_t mg_uecc_dword_t;

#define HIGH_BIT_SET 0x80000000
#define MG_UECC_WORD_BITS 32
#define MG_UECC_WORD_BITS_SHIFT 5
#define MG_UECC_WORD_BITS_MASK 0x01F

#elif (MG_UECC_WORD_SIZE == 8)

typedef uint64_t mg_uecc_word_t;

#define HIGH_BIT_SET 0x8000000000000000U
#define MG_UECC_WORD_BITS 64
#define MG_UECC_WORD_BITS_SHIFT 6
#define MG_UECC_WORD_BITS_MASK 0x03F

#endif /* MG_UECC_WORD_SIZE */

#endif /* _UECC_TYPES_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_VLI_H_
#define _UECC_VLI_H_

// 
// 

/* Functions for raw large-integer manipulation. These are only available
   if uECC.c is compiled with MG_UECC_ENABLE_VLI_API defined to 1. */
#ifndef MG_UECC_ENABLE_VLI_API
#define MG_UECC_ENABLE_VLI_API 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if MG_UECC_ENABLE_VLI_API

void mg_uecc_vli_clear(mg_uecc_word_t *vli, wordcount_t num_words);

/* Constant-time comparison to zero - secure way to compare long integers */
/* Returns 1 if vli == 0, 0 otherwise. */
mg_uecc_word_t mg_uecc_vli_isZero(const mg_uecc_word_t *vli,
                                  wordcount_t num_words);

/* Returns nonzero if bit 'bit' of vli is set. */
mg_uecc_word_t mg_uecc_vli_testBit(const mg_uecc_word_t *vli, bitcount_t bit);

/* Counts the number of bits required to represent vli. */
bitcount_t mg_uecc_vli_numBits(const mg_uecc_word_t *vli,
                               const wordcount_t max_words);

/* Sets dest = src. */
void mg_uecc_vli_set(mg_uecc_word_t *dest, const mg_uecc_word_t *src,
                     wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns one if left == right, zero otherwise */
mg_uecc_word_t mg_uecc_vli_equal(const mg_uecc_word_t *left,
                                 const mg_uecc_word_t *right,
                                 wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns sign of left - right, in constant time. */
cmpresult_t mg_uecc_vli_cmp(const mg_uecc_word_t *left,
                            const mg_uecc_word_t *right, wordcount_t num_words);

/* Computes vli = vli >> 1. */
void mg_uecc_vli_rshift1(mg_uecc_word_t *vli, wordcount_t num_words);

/* Computes result = left + right, returning carry. Can modify in place. */
mg_uecc_word_t mg_uecc_vli_add(mg_uecc_word_t *result,
                               const mg_uecc_word_t *left,
                               const mg_uecc_word_t *right,
                               wordcount_t num_words);

/* Computes result = left - right, returning borrow. Can modify in place. */
mg_uecc_word_t mg_uecc_vli_sub(mg_uecc_word_t *result,
                               const mg_uecc_word_t *left,
                               const mg_uecc_word_t *right,
                               wordcount_t num_words);

/* Computes result = left * right. Result must be 2 * num_words long. */
void mg_uecc_vli_mult(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                      const mg_uecc_word_t *right, wordcount_t num_words);

/* Computes result = left^2. Result must be 2 * num_words long. */
void mg_uecc_vli_square(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        wordcount_t num_words);

/* Computes result = (left + right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void mg_uecc_vli_modAdd(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                        wordcount_t num_words);

/* Computes result = (left - right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void mg_uecc_vli_modSub(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                        const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                        wordcount_t num_words);

/* Computes result = product % mod, where product is 2N words long.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_mmod(mg_uecc_word_t *result, mg_uecc_word_t *product,
                      const mg_uecc_word_t *mod, wordcount_t num_words);

/* Calculates result = product (mod curve->p), where product is up to
   2 * curve->num_words long. */
void mg_uecc_vli_mmod_fast(mg_uecc_word_t *result, mg_uecc_word_t *product,
                           MG_UECC_Curve curve);

/* Computes result = (left * right) % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_modMult(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                         const mg_uecc_word_t *right, const mg_uecc_word_t *mod,
                         wordcount_t num_words);

/* Computes result = (left * right) % curve->p. */
void mg_uecc_vli_modMult_fast(mg_uecc_word_t *result,
                              const mg_uecc_word_t *left,
                              const mg_uecc_word_t *right, MG_UECC_Curve curve);

/* Computes result = left^2 % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void mg_uecc_vli_modSquare(mg_uecc_word_t *result, const mg_uecc_word_t *left,
                           const mg_uecc_word_t *mod, wordcount_t num_words);

/* Computes result = left^2 % curve->p. */
void mg_uecc_vli_modSquare_fast(mg_uecc_word_t *result,
                                const mg_uecc_word_t *left,
                                MG_UECC_Curve curve);

/* Computes result = (1 / input) % mod.*/
void mg_uecc_vli_modInv(mg_uecc_word_t *result, const mg_uecc_word_t *input,
                        const mg_uecc_word_t *mod, wordcount_t num_words);

#if MG_UECC_SUPPORT_COMPRESSED_POINT
/* Calculates a = sqrt(a) (mod curve->p) */
void mg_uecc_vli_mod_sqrt(mg_uecc_word_t *a, MG_UECC_Curve curve);
#endif

/* Converts an integer in uECC native format to big-endian bytes. */
void mg_uecc_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
                               const mg_uecc_word_t *native);
/* Converts big-endian bytes to an integer in uECC native format. */
void mg_uecc_vli_bytesToNative(mg_uecc_word_t *native, const uint8_t *bytes,
                               int num_bytes);

unsigned mg_uecc_curve_num_words(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_bytes(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_bits(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_words(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_bytes(MG_UECC_Curve curve);
unsigned mg_uecc_curve_num_n_bits(MG_UECC_Curve curve);

const mg_uecc_word_t *mg_uecc_curve_p(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_n(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_G(MG_UECC_Curve curve);
const mg_uecc_word_t *mg_uecc_curve_b(MG_UECC_Curve curve);

int mg_uecc_valid_point(const mg_uecc_word_t *point, MG_UECC_Curve curve);

/* Multiplies a point by a scalar. Points are represented by the X coordinate
   followed by the Y coordinate in the same array, both coordinates are
   curve->num_words long. Note that scalar must be curve->num_n_words long (NOT
   curve->num_words). */
void mg_uecc_point_mult(mg_uecc_word_t *result, const mg_uecc_word_t *point,
                        const mg_uecc_word_t *scalar, MG_UECC_Curve curve);

/* Generates a random integer in the range 0 < random < top.
   Both random and top have num_words words. */
int mg_uecc_generate_random_int(mg_uecc_word_t *random,
                                const mg_uecc_word_t *top,
                                wordcount_t num_words);

#endif /* MG_UECC_ENABLE_VLI_API */

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_VLI_H_ */

// End of uecc BSD-2
// portable8439 v1.0.1
// Source: https://github.com/DavyLandman/portable8439
// Licensed under CC0-1.0
// Contains poly1305-donna e6ad6e091d30d7f4ec2d4f978be1fcfcbce72781 (Public
// Domain)





#if MG_TLS == MG_TLS_BUILTIN
#ifndef __PORTABLE_8439_H
#define __PORTABLE_8439_H
#if defined(__cplusplus)
extern "C" {
#endif

// provide your own decl specificier like -DPORTABLE_8439_DECL=ICACHE_RAM_ATTR
#ifndef PORTABLE_8439_DECL
#define PORTABLE_8439_DECL
#endif

/*
 This library implements RFC 8439 a.k.a. ChaCha20-Poly1305 AEAD

 You can use this library to avoid attackers mutating or reusing your
 encrypted messages. This does assume you never reuse a nonce+key pair and,
 if possible, carefully pick your associated data.
*/

/* Make sure we are either nested in C++ or running in a C99+ compiler
#if !defined(__cplusplus) && !defined(_MSC_VER) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#error "C99 or newer required"
#endif */

// #if CHAR_BIT > 8
// #    error "Systems without native octals not suppoted"
// #endif

#if defined(_MSC_VER) || defined(__cplusplus)
// add restrict support is possible
#if (defined(_MSC_VER) && _MSC_VER >= 1900) || defined(__clang__) || \
    defined(__GNUC__)
#define restrict __restrict
#else
#define restrict
#endif
#endif

#define RFC_8439_TAG_SIZE (16)
#define RFC_8439_KEY_SIZE (32)
#define RFC_8439_NONCE_SIZE (12)

/*
    Encrypt/Seal plain text bytes into a cipher text that can only be
    decrypted by knowing the key, nonce and associated data.

    input:
        - key: RFC_8439_KEY_SIZE bytes that all parties have agreed
            upon beforehand
        - nonce: RFC_8439_NONCE_SIZE bytes that should never be repeated
            for the same key. A counter or a pseudo-random value are fine.
        - ad: associated data to include with calculating the tag of the
            cipher text. Can be null for empty.
        - plain_text: data to be encrypted, pointer + size should not overlap
            with cipher_text pointer

    output:
        - cipher_text: encrypted plain_text with a tag appended. Make sure to
            allocate at least plain_text_size + RFC_8439_TAG_SIZE

    returns:
        - size of bytes written to cipher_text, can be -1 if overlapping
            pointers are passed for plain_text and cipher_text
*/
PORTABLE_8439_DECL size_t mg_chacha20_poly1305_encrypt(
    uint8_t *restrict cipher_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE], const uint8_t *restrict ad,
    size_t ad_size, const uint8_t *restrict plain_text, size_t plain_text_size);

/*
    Decrypt/unseal cipher text given the right key, nonce, and additional data.

    input:
        - key: RFC_8439_KEY_SIZE bytes that all parties have agreed
            upon beforehand
        - nonce: RFC_8439_NONCE_SIZE bytes that should never be repeated for
            the same key. A counter or a pseudo-random value are fine.
        - ad: associated data to include with calculating the tag of the
            cipher text. Can be null for empty.
        - cipher_text: encrypted message.

    output:
        - plain_text: data to be encrypted, pointer + size should not overlap
            with cipher_text pointer, leave at least enough room for
            cipher_text_size - RFC_8439_TAG_SIZE

    returns:
        - size of bytes written to plain_text, -1 signals either:
            - incorrect key/nonce/ad
            - corrupted cipher_text
            - overlapping pointers are passed for plain_text and cipher_text
*/
PORTABLE_8439_DECL size_t mg_chacha20_poly1305_decrypt(
    uint8_t *restrict plain_text, const uint8_t key[RFC_8439_KEY_SIZE],
    const uint8_t nonce[RFC_8439_NONCE_SIZE],
    const uint8_t *restrict ad, size_t ad_size,
    const uint8_t *restrict cipher_text, size_t cipher_text_size);
#if defined(__cplusplus)
}
#endif
#endif
#endif
#ifndef TLS_RSA_H
#define TLS_RSA_H


int mg_rsa_mod_pow(const uint8_t *mod, size_t modsz, const uint8_t *exp, size_t expsz, const uint8_t *msg, size_t msgsz, uint8_t *out, size_t outsz);
int mg_rsa_crt_sign(const uint8_t *em, size_t em_len,
                    const uint8_t *dP, size_t dP_len,
                    const uint8_t *dQ, size_t dQ_len,
                    const uint8_t *p, size_t p_len,
                    const uint8_t *q, size_t q_len,
                    const uint8_t *qInv, size_t qInv_len,
                    uint8_t *signature, size_t sig_len);
bool mg_rsa_verify(const uint8_t *em, size_t nlen, const uint8_t *mhash);
#endif // TLS_RSA_H







#if MG_TLS == MG_TLS_MBED
#include <mbedtls/debug.h>
#include <mbedtls/error.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_ticket.h>

struct mg_tls_ctx {
  int dummy;
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context tickets;
#endif
};

struct mg_tls {
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_pk_context pk;    // Private key context
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context ticket;  // Session tickets context
#endif
  // https://github.com/Mbed-TLS/mbedtls/blob/3b3c652d/include/mbedtls/ssl.h#L5071C18-L5076C29
  unsigned char *throttled_buf;  // see #3074
  size_t throttled_len;
};
#endif


#if MG_TLS == MG_TLS_OPENSSL || MG_TLS == MG_TLS_WOLFSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  BIO_METHOD *bm;
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif


// WebSocket frame opcodes, used in mg_ws_send() op parameter and
// in the low 4 bits of mg_ws_message.flags.
#define WEBSOCKET_OP_CONTINUE 0  // Continuation frame (fragmented message)
#define WEBSOCKET_OP_TEXT 1      // Text (UTF-8) data frame
#define WEBSOCKET_OP_BINARY 2    // Binary data frame
#define WEBSOCKET_OP_CLOSE 8     // Connection close control frame
#define WEBSOCKET_OP_PING 9      // Ping control frame (auto-replied with PONG)
#define WEBSOCKET_OP_PONG 10     // Pong control frame



// WebSocket message delivered as ev_data in MG_EV_WS_MSG and MG_EV_WS_CTL.
// data is a zero-copy slice into the receive buffer; valid until the next
// mg_mgr_poll() call.
struct mg_ws_message {
  struct mg_str data;  // Message payload
  uint8_t flags;       // First byte of the WS frame header: FIN (bit7) | opcode (bits3:0)
};

// Opens an outbound WebSocket connection.
//
// Returns:
//   Client connection, or NULL on error.
// Example:
//   mg_ws_connect(&mgr, "ws://localhost:8000/websocket", fn, NULL, NULL);
// Full examples:
//   tutorials/websocket/websocket-client, tutorials/mqtt/mqtt-over-ws-client
// Related APIs:
//   mg_ws_send(), mg_ws_printf(), mg_tls_init(), mg_ws_upgrade()
// Notes:
//   url may use ws:// or wss://. Sends the HTTP Upgrade request immediately.
//   fmt is a printf-style string for extra HTTP request headers; each header
//   must end with "\r\n". The user-supplied fn event handler receives
//   MG_EV_WS_OPEN on handshake success, MG_EV_WS_MSG for messages, and
//   MG_EV_WS_CTL for control frames.
struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);

// Upgrades a server-side HTTP connection to WebSocket.
//
// Example:
//   if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
//     mg_ws_upgrade(c, hm, NULL);
//   }
// Full examples:
//   tutorials/websocket/websocket-server,
//   tutorials/websocket/json-rpc-over-websocket, tutorials/core/timers
// Related APIs:
//   mg_http_listen(), mg_match(), mg_ws_send(), mg_ws_printf()
// Notes:
//   Call from an MG_EV_HTTP_MSG handler and pass that event's hm. fmt is a
//   printf-style string for extra response headers; each header must end with
//   "\r\n". Fires MG_EV_WS_OPEN immediately on success. Sends HTTP 426 and
//   drains the connection if the request lacks Sec-WebSocket-Key.
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
                   const char *fmt, ...);

// Sends one WebSocket frame.
//
// Returns:
//   Number of bytes appended to c->send; 0 if the frame header cannot be
//   appended.
// Example:
//   mg_ws_send(c, "hello", 5, WEBSOCKET_OP_TEXT);
// Full examples:
//   tutorials/websocket/websocket-server, tutorials/websocket/websocket-client,
//   tutorials/core/timers
// Related APIs:
//   mg_ws_connect(), mg_ws_upgrade(), mg_ws_printf(), mg_ws_wrap()
// Notes:
//   op is one of WEBSOCKET_OP_*. Client connections are automatically masked
//   per RFC 6455. On OOM, the return value can be smaller than header + len.
//   Data is appended to c->send and sent by a later mg_mgr_poll() call.
size_t mg_ws_send(struct mg_connection *, const void *buf, size_t len, int op);

// Wraps the last len bytes already in c->send with a WebSocket frame header
// and opcode op. Used internally by mg_ws_printf(); call it directly when you
// have written data into c->send manually and need to frame it.
// Returns c->send.len (total buffer size after the operation).
size_t mg_ws_wrap(struct mg_connection *, size_t len, int op);

// Formats a WebSocket message using printf-style fmt and sends it with
// opcode op (WEBSOCKET_OP_TEXT or WEBSOCKET_OP_BINARY).
// Returns the number of payload bytes written, or 0 on OOM.
size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...);
size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
                     va_list *);
// Time / SNTP functions





// Return milliseconds since boot (uptime). Never goes backwards.
// On RTOS targets (FreeRTOS, ThreadX, Zephyr, etc.) uses the RTOS tick count.
// On STM32/Cube uses HAL_GetTick(). On Arduino uses millis().
// On Linux/macOS uses CLOCK_MONOTONIC_RAW or CLOCK_MONOTONIC; falls back to
// CLOCK_REALTIME if neither is available.
uint64_t mg_millis(void);

// Boot timestamp in ms since Epoch. 0 until the first successful SNTP sync;
// updated automatically by mg_sntp_connect() on each successful response.
extern uint64_t mg_boot_timestamp_ms;

// Return milliseconds since Epoch: mg_millis() + mg_boot_timestamp_ms.
// Until a successful SNTP request completes, this is identical to mg_millis().
uint64_t mg_now(void);

// Checks whether a periodic deadline has expired.
//
// Returns:
//   True when the timer has fired; false otherwise.
// Example:
//   uint64_t timer = 0;
//   if (mg_timer_expired(&timer, 1000, mg_millis())) {
//     MG_INFO(("every second"));
//   }
// Full examples:
//   tutorials/http/http-server/arduino/teensy41-http,
//   tutorials/mqtt/mqtt-client
// Related APIs:
//   mg_millis(), mg_now()
// Notes:
//   Initialise *expiration to 0 before first use. On expiry, this function
//   advances *expiration by period and handles time wrap-around.
bool mg_timer_expired(uint64_t *expiration, uint64_t period, uint64_t now);

// Connect to an SNTP server and send a time request.
// `url` defaults to "udp://time.google.com:123" when NULL.
// On success the internal boot timestamp is updated so that mg_now() returns
// the correct wall-clock time, and MG_EV_SNTP_TIME is fired on `fn` (if not
// NULL) with ev_data pointing to a uint64_t containing the epoch in milliseconds.
// Pass fn == NULL and fn_data == NULL for a fire-and-forget sync that only
// updates mg_boot_timestamp_ms. Typical polling usage:
//
// ```c
// uint64_t timer = 0;
// for (;;) {
//   uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 3600 * 1000;
//   if (mg_timer_expired(&timer, period, mg_millis())) {
//     mg_sntp_connect(&mgr, NULL, NULL, NULL);
//   }
//   mg_mgr_poll(&mgr, 1000);
// }
// ```
struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data);

// Private API, do not expose
void mg_sntp_request(struct mg_connection *c);
int64_t mg_sntp_parse(const unsigned char *buf, size_t len);





// MQTT packet type codes. The received command is in mg_mqtt_message.cmd.
#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14
#define MQTT_CMD_AUTH 15

// MQTT5 property IDs. Used in mg_mqtt_prop.id.
#define MQTT_PROP_PAYLOAD_FORMAT_INDICATOR 0x01
#define MQTT_PROP_MESSAGE_EXPIRY_INTERVAL 0x02
#define MQTT_PROP_CONTENT_TYPE 0x03
#define MQTT_PROP_RESPONSE_TOPIC 0x08
#define MQTT_PROP_CORRELATION_DATA 0x09
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER 0x0B
#define MQTT_PROP_SESSION_EXPIRY_INTERVAL 0x11
#define MQTT_PROP_ASSIGNED_CLIENT_IDENTIFIER 0x12
#define MQTT_PROP_SERVER_KEEP_ALIVE 0x13
#define MQTT_PROP_AUTHENTICATION_METHOD 0x15
#define MQTT_PROP_AUTHENTICATION_DATA 0x16
#define MQTT_PROP_REQUEST_PROBLEM_INFORMATION 0x17
#define MQTT_PROP_WILL_DELAY_INTERVAL 0x18
#define MQTT_PROP_REQUEST_RESPONSE_INFORMATION 0x19
#define MQTT_PROP_RESPONSE_INFORMATION 0x1A
#define MQTT_PROP_SERVER_REFERENCE 0x1C
#define MQTT_PROP_REASON_STRING 0x1F
#define MQTT_PROP_RECEIVE_MAXIMUM 0x21
#define MQTT_PROP_TOPIC_ALIAS_MAXIMUM 0x22
#define MQTT_PROP_TOPIC_ALIAS 0x23
#define MQTT_PROP_MAXIMUM_QOS 0x24
#define MQTT_PROP_RETAIN_AVAILABLE 0x25
#define MQTT_PROP_USER_PROPERTY 0x26
#define MQTT_PROP_MAXIMUM_PACKET_SIZE 0x27
#define MQTT_PROP_WILDCARD_SUBSCRIPTION_AVAILABLE 0x28
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE 0x29
#define MQTT_PROP_SHARED_SUBSCRIPTION_AVAILABLE 0x2A

// Property value types used internally to encode/decode MQTT5 properties.
enum {
  MQTT_PROP_TYPE_BYTE,
  MQTT_PROP_TYPE_STRING,
  MQTT_PROP_TYPE_STRING_PAIR,
  MQTT_PROP_TYPE_BINARY_DATA,
  MQTT_PROP_TYPE_VARIABLE_INT,
  MQTT_PROP_TYPE_INT,
  MQTT_PROP_TYPE_SHORT
};

// Return codes from mg_mqtt_parse().
enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

// A single MQTT5 property. Use in mg_mqtt_opts.props / mg_mqtt_opts.will_props
// arrays, and iterate received properties with mg_mqtt_next_prop().
struct mg_mqtt_prop {
  uint8_t id;         // Property ID, one of MQTT_PROP_*
  uint32_t iv;        // Integer value for BYTE, SHORT, INT, VARIABLE_INT types
  struct mg_str key;  // Property key string; only set for USER_PROPERTY pairs
  struct mg_str val;  // String/binary value; set for STRING, BINARY_DATA, and USER_PROPERTY
};

// Options passed to mg_mqtt_connect(), mg_mqtt_pub(), mg_mqtt_sub(),
// mg_mqtt_unsub(), and mg_mqtt_disconnect(). Zero-initialise and set only
// the fields relevant to the operation being called.
struct mg_mqtt_opts {
  struct mg_str user;               // CONNECT: username; empty = omit
  struct mg_str pass;               // CONNECT: password; empty = omit
  struct mg_str client_id;          // CONNECT: client ID; empty = auto-generate
  struct mg_str topic;              // CONNECT: will topic; PUBLISH/SUBSCRIBE/UNSUBSCRIBE: topic
  struct mg_str message;            // CONNECT: will payload; PUBLISH: message payload
  uint8_t qos;                      // QoS level: 0, 1, or 2
  uint8_t version;                  // MQTT version: 4 (3.1.1) or 5; 0 defaults to 4
  uint16_t keepalive;               // CONNECT: keepalive interval in seconds; 0 = no timeout
  uint16_t retransmit_id;           // PUBLISH: packet ID for retransmit; init to 0 for new message
  bool retain;                      // PUBLISH/CONNECT will: set the retain flag
  bool clean;                       // CONNECT: start a clean session
  struct mg_mqtt_prop *props;       // MQTT5: properties array; NULL if none
  size_t num_props;                 // MQTT5: number of entries in props
  struct mg_mqtt_prop *will_props;  // MQTT5 CONNECT: will properties; NULL if none
  size_t num_will_props;            // MQTT5: number of entries in will_props
};

// Received MQTT message. Passed as ev_data for MG_EV_MQTT_MSG (PUBLISH),
// MG_EV_MQTT_CMD (all packets), and MG_EV_MQTT_OPEN (CONNACK).
// topic, data, and dgram are zero-copy slices into the receive buffer; they
// are only valid until the next mg_mgr_poll() call.
struct mg_mqtt_message {
  struct mg_str topic;  // PUBLISH: message topic; slice into dgram
  struct mg_str data;   // PUBLISH: message payload; slice into dgram
  struct mg_str dgram;  // Complete raw MQTT packet, including fixed header
  uint16_t id;          // Packet ID; set for QoS>0 PUBLISH, PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK
  uint8_t cmd;          // Packet type, one of MQTT_CMD_*
  uint8_t qos;          // QoS level extracted from the PUBLISH fixed header
  uint8_t ack;          // CONNACK return code; 0 = success
  size_t props_start;   // MQTT5: byte offset from dgram.buf to start of properties
  size_t props_size;    // MQTT5: byte length of the properties section
};

// Opens an MQTT client connection and sends a CONNECT packet.
//
// Returns:
//   Client connection, or NULL on error.
// Example:
//   struct mg_mqtt_opts opts = {.client_id = mg_str("device1")};
//   mg_mqtt_connect(&mgr, "mqtt://broker:1883", &opts, fn, NULL);
// Full examples:
//   tutorials/mqtt/mqtt-client, tutorials/mqtt/mqtt-client-aws-iot,
//   tutorials/mqtt/ota-over-mqtt
// Related APIs:
//   mg_mqtt_pub(), mg_mqtt_sub(), mg_tls_init(), mg_mgr_poll()
// Notes:
//   opts may be NULL to use MQTT 3.1.1 defaults with no auth and an auto client
//   ID. The user-supplied fn event handler receives normal connection events,
//   MG_EV_MQTT_OPEN when CONNACK is received, MG_EV_MQTT_MSG for incoming
//   PUBLISH packets, and MG_EV_MQTT_CMD for every received MQTT packet.
struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      const struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);

// Creates an MQTT server listener on url.
// Fires MG_EV_MQTT_CMD for each complete packet received (ev_data: struct mg_mqtt_message *).
// Returns NULL on error.
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data);

// Sends a CONNECT packet on an already-open connection.
// Called automatically by mg_mqtt_connect(); exposed for advanced use such
// as re-authenticating on an existing connection.
void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Sends an MQTT PUBLISH packet.
//
// Returns:
//   0 for QoS 0; assigned packet ID for QoS 1 or 2.
// Example:
//   struct mg_mqtt_opts opts = {
//       .topic = mg_str("device/status"),
//       .message = mg_str("{\"ok\":true}"),
//       .qos = 1,
//   };
//   uint16_t id = mg_mqtt_pub(c, &opts);
// Full examples:
//   tutorials/mqtt/mqtt-client, tutorials/mqtt/mqtt-client-aws-iot,
//   tutorials/mqtt/ota-over-mqtt
// Related APIs:
//   mg_mqtt_connect(), mg_mqtt_sub(), mg_mqtt_send_header()
// Notes:
//   On success, opts.topic and opts.message are copied into c->send before the
//   function returns. To retransmit a QoS message, set opts.retransmit_id to the
//   packet ID returned by the previous call; use 0 for a new message.
uint16_t mg_mqtt_pub(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Sends an MQTT SUBSCRIBE packet.
//
// Example:
//   struct mg_mqtt_opts opts = {.topic = mg_str("device/rx"), .qos = 1};
//   mg_mqtt_sub(c, &opts);
// Full examples:
//   tutorials/mqtt/mqtt-client, tutorials/mqtt/mqtt-client-aws-iot,
//   tutorials/mqtt/ota-over-mqtt
// Related APIs:
//   mg_mqtt_connect(), mg_mqtt_pub(), mg_mqtt_unsub()
// Notes:
//   Send after MG_EV_MQTT_OPEN reports a successful CONNACK. Incoming PUBLISH
//   messages are delivered to the user-supplied fn event handler as
//   MG_EV_MQTT_MSG with ev_data pointing to struct mg_mqtt_message.
void mg_mqtt_sub(struct mg_connection *, const struct mg_mqtt_opts *opts);

// Sends an UNSUBSCRIBE packet for opts.topic.
void mg_mqtt_unsub(struct mg_connection *c, const struct mg_mqtt_opts *opts);

// Parses a raw MQTT packet in buf/len. version must be 4 (MQTT 3.1.1) or 5.
// On success, fills *m; topic, data, and dgram slice into buf (zero-copy).
// Returns MQTT_OK, MQTT_INCOMPLETE (need more data), or MQTT_MALFORMED.
int mg_mqtt_parse(const uint8_t *, size_t, uint8_t, struct mg_mqtt_message *);

// Low-level helper: sends the MQTT fixed header (command byte and
// variable-length remaining-length field). Call before writing the PDU body.
// cmd: MQTT_CMD_*; flags: low 4 bits of the first byte; len: remaining length.
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags,
                         uint32_t len);

// Sends a PINGREQ packet.
void mg_mqtt_ping(struct mg_connection *);

// Sends a PINGRESP packet.
void mg_mqtt_pong(struct mg_connection *);

// Sends a DISCONNECT packet. For MQTT5, opts.props can carry reason
// properties. opts may be NULL for a plain disconnect with no properties.
void mg_mqtt_disconnect(struct mg_connection *, const struct mg_mqtt_opts *);

// Iterates MQTT5 properties in a received message. Start with ofs=0; pass
// the returned value back on each successive call. Returns 0 when done.
// Fills *prop with each property; key and val are zero-copy slices into msg->dgram.
size_t mg_mqtt_next_prop(struct mg_mqtt_message *, struct mg_mqtt_prop *,
                         size_t ofs);





// DNS record types, used in mg_dns_rr.atype and mg_mdns_query().
#define MG_DNS_RTYPE_A 1     // IPv4 address (4 bytes)
#define MG_DNS_RTYPE_PTR 12  // Domain name pointer, used for reverse lookups and service discovery
#define MG_DNS_RTYPE_TXT 16  // Arbitrary text, used by DNS-SD for service metadata
#define MG_DNS_RTYPE_AAAA 28 // IPv6 address (16 bytes)
#define MG_DNS_RTYPE_SRV 33  // Service location: priority, weight, port, target hostname

// Mongoose sends DNS queries that contain only one question:
// either A (IPv4) or AAAA (IPv6) address lookup.
// Therefore, we expect zero or one answer.
// If `resolved` is true, then `addr` contains resolved IPv4 or IPV6 address.
struct mg_dns_message {
  uint16_t txnid;       // Transaction ID
  bool resolved;        // Resolve successful, addr is set
  struct mg_addr addr;  // Resolved address
  char name[256];       // Host name
};

// DNS wire-format header (RFC 1035). All fields are in network byte order;
// use mg_ntohs() to read them.
struct mg_dns_header {
  uint16_t txnid;           // Transaction ID, matched between query and response
  uint16_t flags;           // Flags: bit15=QR (0=query,1=response), bit10=AA, bit8=RD
  uint16_t num_questions;   // Number of question records following the header
  uint16_t num_answers;     // Number of answer records in the response
  uint16_t num_authority_prs;  // Number of authority records
  uint16_t num_other_prs;      // Number of additional records
};

// Parsed DNS resource record fields, extracted from the wire format.
struct mg_dns_rr {
  uint16_t nlen;    // Byte length of the encoded name in the wire format
  uint16_t atype;   // Record type: MG_DNS_RTYPE_A=1, AAAA=28, PTR=12, SRV=33, TXT=16
  uint16_t aclass;  // Record class: 1=INET; mDNS queries: bit15=QU (unicast request)
  uint16_t alen;    // Byte length of the RDATA section (e.g. 4 for A, 16 for AAAA)
};

// DNS-SD service description. Used by the user to describe a service to advertise.
struct mg_dnssd_record {
  struct mg_str srvcproto;  // Service and protocol label, e.g. "_http._tcp"
  struct mg_str txt;        // TXT record contents, verbatim
  uint16_t port;            // Port number for the SRV record
};

// ev_data for MG_EV_MDNS_REQ. The handler must set is_resp=true and fill r
// (and optionally respname/addr) to send a response; leave is_resp=false to ignore.
struct mg_mdns_req {
  struct mg_dns_rr *rr;         // Parsed resource record from the incoming query
  struct mg_dnssd_record *r;    // User-supplied service record to include in the response
  struct mg_str reqname;        // Queried hostname, without the .local suffix
  struct mg_str respname;       // Hostname to use in response; defaults to fn_data if empty
  struct mg_addr *addr;         // IP address for A record; uses local interface if NULL
  bool is_listing;  // True if this is a service-discovery listing (_services._dns-sd._udp)
  bool is_resp;     // Set to true in the handler to trigger a response
  bool is_unicast;  // True if the client requested a unicast (QU) response
};

// ev_data for MG_EV_MDNS_RESP, carrying the resolved address from an mDNS response.
struct mg_mdns_resp {
  struct mg_dns_rr *rr;  // Resource record from the response
  struct mg_str name;    // Resolved hostname, without the .local suffix
  struct mg_addr addr;   // Resolved IP address
};

// Parses a DNS query or response from buf/len into dm. Returns true on success.
// On a response, sets dm->resolved=true and fills dm->addr if an A/AAAA record
// is found. Always fills dm->txnid and dm->name. Rejects packets larger than 512 bytes.
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm);

// Parses one DNS record at offset ofs in buf/len into rr. Set is_question=true
// for question records (no RDATA), false for answer records. Returns the number
// of bytes consumed (use to advance ofs), or 0 on error.
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr);

// Creates an mDNS listener on UDP multicast 224.0.0.251:5353. Registers it as
// the manager's mDNS resolver (mgr->mdns). Fires MG_EV_MDNS_REQ for incoming
// queries and MG_EV_MDNS_RESP for incoming responses. Returns NULL on error.
struct mg_connection *mg_mdns_listen(struct mg_mgr *mgr, mg_event_handler_t fn,
                                     void *fn_data);

// Sends an mDNS query on connection c (from mg_mdns_listen()) for name, with
// record type rtype (e.g. MG_DNS_RTYPE_A). Returns false on send error.
bool mg_mdns_query(struct mg_connection *c, const char *name, unsigned int rtype);


// Private API, do not expose or call in the user app
void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);




// Modbus TCP function codes passed in mg_modbus_req.func.
#define MG_MODBUS_FUNC_READ_COILS 1             // Read 1-2000 discrete output coils
#define MG_MODBUS_FUNC_READ_DISCRETE_INPUTS 2   // Read 1-2000 discrete input coils
#define MG_MODBUS_FUNC_READ_HOLDING_REGISTERS 3 // Read 1-125 holding registers
#define MG_MODBUS_FUNC_READ_INPUT_REGISTERS 4   // Read 1-125 input registers
#define MG_MODBUS_FUNC_WRITE_SINGLE_COIL 5      // Write one output coil
#define MG_MODBUS_FUNC_WRITE_SINGLE_REGISTER 6  // Write one holding register
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_COILS 15  // Write 1-1968 output coils
#define MG_MODBUS_FUNC_WRITE_MULTIPLE_REGISTERS 16 // Write 1-123 holding registers

// Standard Modbus exception codes. Set mg_modbus_req.error to one of these
// in the MG_EV_MODBUS_REQ handler to return an exception response to the client.
#define MG_MODBUS_ERR_NONE 0             // No error; send a normal response
#define MG_MODBUS_ERR_ILLEGAL_FUNCTION 1 // Function code not supported
#define MG_MODBUS_ERR_ILLEGAL_ADDRESS 2  // Starting address or range out of bounds
#define MG_MODBUS_ERR_ILLEGAL_VALUE 3    // Data value not accepted
#define MG_MODBUS_ERR_DEVICE_FAILURE 4   // Unrecoverable device error

// Modbus request passed as ev_data to the MG_EV_MODBUS_REQ event handler.
//
// For READ functions (READ_COILS, READ_DISCRETE_INPUTS, READ_HOLDING_REGISTERS,
// READ_INPUT_REGISTERS): u.bits / u.regs points to a zeroed, heap-allocated
// array of len entries. The handler fills in the values to return.
//
// For WRITE functions: u.bits / u.regs is pre-populated from the client
// request. len=1 for single-write functions.
//
// Do NOT free u.bits or u.regs - the library frees them after the handler
// returns and the response has been sent.
//
// Set error to a non-zero MG_MODBUS_ERR_* code to send an exception response
// instead of a data response. The response is sent automatically on return.
struct mg_modbus_req {
  uint8_t func;   // Function code, one of MG_MODBUS_FUNC_*
  uint8_t error;  // Exception code; set to MG_MODBUS_ERR_* to return an error
  uint16_t addr;  // Starting coil/register address from the request
  union {
    bool *bits;      // Coil values; used by coil function codes
    uint16_t *regs;  // Register values; used by register function codes
  } u;
  uint16_t len;  // Number of entries in u.bits or u.regs
};

// Creates a Modbus TCP listener on url. Fires MG_EV_MODBUS_REQ for each
// complete request; ev_data is struct mg_modbus_req *. Returns the listening
// connection, or NULL on error.
struct mg_connection *mg_modbus_listen(struct mg_mgr *mgr, const char *url,
                                       mg_event_handler_t fn, void *fn_data);





// Maximum JSON nesting depth. mg_json_get() returns MG_JSON_TOO_DEEP if
// the path traverses more than this many nested objects/arrays.
#ifndef MG_JSON_MAX_DEPTH
#define MG_JSON_MAX_DEPTH 30
#endif

// Error codes returned by mg_json_get(). Successful returns are >= 0.
enum { MG_JSON_TOO_DEEP = -1, MG_JSON_INVALID = -2, MG_JSON_NOT_FOUND = -3 };

// Low-level token locator. Finds the JSON token at path inside json and
// returns its byte offset in json.buf. Sets *toklen to the token length.
// toklen may be NULL if only a presence check is needed.
// path must start with "$" (root). Use ".key" for object fields and "[N]"
// for array indices. Example: "$.items[0].name"
// Returns MG_JSON_NOT_FOUND, MG_JSON_INVALID, or MG_JSON_TOO_DEEP on error.
int mg_json_get(struct mg_str json, const char *path, int *toklen);

// Returns the raw JSON token at path.
//
// Returns:
//   Zero-copy mg_str slice into json.buf, or {NULL, 0} if not found or invalid.
// Example:
//   struct mg_str tok = mg_json_get_tok(body, "$.device.name");
// Full examples:
//   src/dash.c
// Related APIs:
//   mg_json_get(), mg_json_get_str(), mg_json_unescape()
// Notes:
//   The returned token is not NUL-terminated. String tokens include the
//   surrounding double quotes and escape sequences; use mg_json_unescape() or
//   mg_json_get_str() when you need decoded string content.
struct mg_str mg_json_get_tok(struct mg_str json, const char *path);

// Parses a numeric JSON value at path into *v.
// Returns true and sets *v on success. Returns false if not found or not a
// number. v may be NULL to check existence only.
bool mg_json_get_num(struct mg_str json, const char *path, double *v);

// Parses a boolean JSON value at path into *v.
// Returns true and sets *v on success. Returns false if not found or not a
// boolean literal. v may be NULL to check existence only.
bool mg_json_get_bool(struct mg_str json, const char *path, bool *v);

// Returns a JSON number at path as a long, or dflt if not found or not a
// number. Use mg_json_get_num() when you need to distinguish missing from 0.
long mg_json_get_long(struct mg_str json, const char *path, long dflt);

// Returns a heap-allocated, NUL-terminated, JSON-unescaped copy of the string
// at path. Caller must mg_free() the result. Returns NULL if not found or the
// token is not a JSON string.
char *mg_json_get_str(struct mg_str json, const char *path);

// Decodes a hex-encoded JSON string at path into a heap-allocated byte array.
// Sets *len to the decoded byte count. Caller must mg_free() the result.
// Returns NULL if not found or not a string.
char *mg_json_get_hex(struct mg_str json, const char *path, int *len);

// Decodes a base64-encoded JSON string at path into a heap-allocated byte
// array. Sets *len to the decoded byte count. Caller must mg_free() the result.
// Returns NULL if not found or not a string.
char *mg_json_get_b64(struct mg_str json, const char *path, int *len);

// Writes a JSON-unescaped string value at path into a caller-supplied buffer.
//
// Returns:
//   Number of bytes written excluding the NUL, or 0 on error, not found,
//   non-string token, too-small buffer, or an empty string.
// Example:
//   char name[32];
//   mg_json_unescape(body, "$.device.name", name, sizeof(name));
// Full examples:
//   src/dash.c, src/ota.c
// Related APIs:
//   mg_json_get_tok(), mg_json_get_str(), mg_json_get_num()
// Notes:
//   On success, NUL-terminates when n > 0. The destination buffer is owned by
//   the caller. Only string tokens are unescaped; other token types return 0.
size_t mg_json_unescape(struct mg_str json, const char *path, char *, size_t);

// Sequential iterator over a JSON object or array. Start with ofs=0.
// Each call fills *key and *val with the next entry and returns the new ofs
// to pass on the next call. Returns 0 when there are no more entries.
// For arrays, *key is set to {NULL, 0}. For objects, *key is the raw
// quoted key (e.g. "\"name\""). *val is the raw unparsed token.
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val);




// In-flight JSON-RPC request. Populate and pass to mg_rpc_process() to
// dispatch, or receive one pre-filled in a handler registered with mg_rpc_add().
struct mg_rpc_req {
  struct mg_rpc **head;  // Head of the handler list; allows handlers to call other RPCs
  struct mg_rpc *rpc;    // Handler that matched this request; set by mg_rpc_process()
  mg_pfn_t pfn;          // Output function for the response (e.g. mg_pfn_iobuf)
  void *pfn_data;        // Argument passed to pfn (e.g. pointer to an mg_iobuf)
  void *req_data;        // Arbitrary caller data, not touched by the RPC layer
  struct mg_str frame;   // Raw JSON-RPC frame, e.g. {"id":1,"method":"add","params":[1,2]}
};

// A registered JSON-RPC method handler. Managed by mg_rpc_add/del; do not
// allocate directly. method is a glob pattern matched via mg_match().
struct mg_rpc {
  struct mg_rpc *next;              // Next handler in the linked list
  struct mg_str method;             // Method name glob pattern (heap-allocated copy)
  void (*fn)(struct mg_rpc_req *);  // Handler function called on a match
  void *fn_data;                    // Passed to fn as r->rpc->fn_data
};

// Allocates a handler entry and prepends it to *head. method_pattern is
// copied (mg_strdup). Glob patterns are supported, e.g. "get/*".
// To receive JSON-RPC responses ($.result / $.error), register with method=""
void mg_rpc_add(struct mg_rpc **head, struct mg_str method_pattern,
                void (*handler)(struct mg_rpc_req *), void *handler_data);

// Removes and frees all handlers in *head whose fn matches handler.
// Pass handler=NULL to remove every handler in the list.
void mg_rpc_del(struct mg_rpc **head, void (*handler)(struct mg_rpc_req *));

// Dispatches r->frame to the matching handler. Looks up "$.method" and calls
// the first handler whose pattern matches. If "$.result" or "$.error" is found
// instead (i.e. r->frame is a JSON-RPC response), dispatches to the "" handler.
// Sends a -32601 "not found" error if no handler matches, or -32700 on parse failure.
void mg_rpc_process(struct mg_rpc_req *);

// Sends a JSON-RPC success response: {"id":<id>,"result":<fmt>}.
// fmt is a printf-style format string written to r->pfn; NULL sends "null".
// Does nothing if the request frame has no "id" field (notification).
void mg_rpc_ok(struct mg_rpc_req *, const char *fmt, ...);
void mg_rpc_vok(struct mg_rpc_req *, const char *fmt, va_list *ap);

// Sends a JSON-RPC error response: {"id":<id>,"error":{"code":<code>,"message":<fmt>}}.
// fmt is a printf-style format string; NULL sends "null" as the message.
// The "id" field is omitted if the request had none; the error is always sent.
void mg_rpc_err(struct mg_rpc_req *, int code, const char *fmt, ...);
void mg_rpc_verr(struct mg_rpc_req *, int code, const char *fmt, va_list *);

// Built-in handler: responds with a JSON array of all registered method names.
// Register it as a method handler, e.g. mg_rpc_add(&head, mg_str("rpc.list"), mg_rpc_list, NULL);
void mg_rpc_list(struct mg_rpc_req *r);









#ifndef MG_DASH_MAX_USER_NAME
#define MG_DASH_MAX_USER_NAME 32
#endif

#ifndef MG_HTTP_ADDR
#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
#define MG_HTTP_ADDR "http://0.0.0.0:8000"
#define MG_HTTPS_ADDR "https://0.0.0.0:8443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:8502"
#else
#define MG_HTTP_ADDR "http://0.0.0.0:80"
#define MG_HTTPS_ADDR "http://0.0.0.0:443"
#define MG_MODBUS_ADDR "tcp://0.0.0.0:502"
#endif
#endif

#ifndef MG_DASH_MAX_SEND_BUF_SIZE
#define MG_DASH_MAX_SEND_BUF_SIZE (MG_IO_SIZE * 5)
#endif

// C type of a value bound to a dashboard field, see struct mg_field
enum mg_val_type {
  MG_VAL_INT,     // int
  MG_VAL_UINT64,  // uint64_t
  MG_VAL_BOOL,    // bool
  MG_VAL_DBL,     // double
  MG_VAL_STR,     // Zero-terminated string
  MG_VAL_RAW,     // Pre-formatted JSON value, copied verbatim
};

// Dashboard field: binds a JSON key to a C variable for read and/or write
struct mg_field {
  const char *name;       // JSON key
  enum mg_val_type type;  // C type of the value, see enum mg_val_type
  void *value;            // Pointer to the bound variable
  size_t value_size;  // sizeof(*value) for type checking; 0 makes it read-only
};

// Operation passed to a struct mg_field_set read/write/delete/add callback.
// MG_DASH_ADD: fields hold the values of a new element to append; *index is
// not set. Return true to accept and persist it, false to reject (e.g. a cap
// was reached)
enum mg_dash_op { MG_DASH_READ, MG_DASH_WRITE, MG_DASH_DELETE, MG_DASH_ADD };

struct mg_dash;

// Authenticated (or guest) dashboard user
struct mg_dash_user {
  struct mg_dash_user *next;         // Next user in the list
  char name[MG_DASH_MAX_USER_NAME];  // Username
  char token[21];        // Session token, sent to the client as a cookie
  int level;             // Access level, see struct mg_dash::authenticate
  uint64_t expire;       // Session expiration time, see mg_millis()
  struct mg_dash *dash;  // Dashboard this user belongs to
};

// Named group of related dashboard fields, e.g. "settings" or "metrics"
struct mg_field_set {
  const char *name;         // Set name, used as a JSON key
  struct mg_field *fields;  // Zero-terminated array of fields
  bool (*fn)(enum mg_dash_op op,
             struct mg_dash_user *u);  // Read/write/delete/add callback
  int *index;  // Non-NULL: array set. *index is set before fn(READ) call;
               // fn sets *index = -1 to signal end of iteration.
               // Size query: framework sets *index = -1 before fn(READ); fn
               // sets *index = total size
  bool (*get_dir)(const struct mg_dash_user *u, char *buf,
                  size_t len);  // Non-NULL: file array
  struct mg_field_set *next;    // Next set in the list
};

// Custom URI handler, registered with MG_DASH_REGISTER_CUSTOM_HANDLER()
struct mg_dash_custom_handler {
  struct mg_dash_custom_handler *next;  // Next handler in the list
  struct mg_str uri_pattern;            // URI to match, see mg_match()
  mg_event_handler_t handler;           // Event handler function
  void *handler_data;                   // Opaque data for the handler
};

// Dashboard instance: fields, users and handlers served over HTTP and WS
struct mg_dash {
  struct mg_field_set *sets;  // Field sets, see MG_DASH_ADD_FIELD_SET()
  struct mg_dash_custom_handler *custom_handlers;  // Custom URI handlers
  // Validates pass, fills in user, returns access level (<= 0: failure).
  // NULL: no login required, all clients get guest access
  int (*authenticate)(char *user, size_t userlen, const char *pass);
  int session_auto_expiration_seconds;  // Session lifetime, 0: 1 hour default
  struct mg_dash_user *guest;           // Used when authenticate == NULL
  struct mg_fs *upload_fs;  // FS for uploads/listing, default &mg_fs_posix
};

#define MG_DASH_ADD_FIELD_SET(dash_, set_) \
  do {                                     \
    (set_)->next = (dash_)->sets;          \
    (dash_)->sets = (set_);                \
  } while (0)

#define MG_DASH_REGISTER_CUSTOM_HANDLER(dash_, uri_, fn_, data_) \
  do {                                                           \
    static struct mg_dash_custom_handler ch_;                    \
    ch_.next = (dash_)->custom_handlers;                         \
    ch_.uri_pattern = mg_str(uri_);                              \
    ch_.handler = (fn_);                                         \
    ch_.handler_data = (data_);                                  \
    (dash_)->custom_handlers = &ch_;                             \
  } while (0)

// Dashboard event handler. Pass to mg_http_listen() as the fn argument
void mg_dash_ev_handler(struct mg_connection *c, int ev, void *ev_data);

// Notify WebSocket clients that a field set changed: broadcasts the new
// array size if *set->index < 0, or a single element update otherwise
void mg_dash_send_change(struct mg_mgr *mgr, struct mg_field_set *);

// Default reader for file-backed array sets (set->get_dir != NULL)
// Usage example - this is a complete implemenation of the file manager:
// ```c
// static struct file {
//   int index;
//   char name[64];
//   size_t size;
//   uint64_t checksum;
// } s_file;
// static struct mg_field_set set_files;
//
// static bool get_dir(const struct mg_dash_user *u, char *buf, size_t len) {
//   (void) u;
//   mkdir("/tmp/dashboard", 0755);
//   mg_snprintf(buf, len, "%s", "/tmp/dashboard");
//   return true;
// }
//
// static struct mg_field fields_files[] = {
//     {"name", MG_VAL_STR, s_file.name, sizeof(s_file.name)},
//     {"size", MG_VAL_UINT64, &s_file.size, 0},
//     {"checksum", MG_VAL_UINT64, &s_file.checksum, sizeof(s_file.checksum)},
//     {NULL, MG_VAL_INT, NULL, 0},
// };
//
// static bool files_fn(enum mg_dash_op op, struct mg_dash_user *u) {
//   if (op == MG_DASH_WRITE) return s_uploads_enabled && u->level >= 7;
//   if (op != MG_DASH_READ) return false;
//   if (!mg_dash_dir_read(&set_files, u)) return false;
//   s_file.checksum = 0;  // Mock to show how to set custom fields
//   return true;
// }
//
// static struct mg_field_set set_files = {"files", fields_files, files_fn, &s_file.index, get_dir, NULL};
// ```
bool mg_dash_dir_read(struct mg_field_set *set, struct mg_dash_user *u);

// Helper forward declarations for Mongoose CMSIS pack modules
extern struct mg_mgr g_mgr;
extern void mg_dash_init(struct mg_mgr *);
extern void mg_dash_poll(struct mg_mgr *);
extern void mg_mqtt_init(struct mg_mgr *);
extern void mg_mqtt_poll(struct mg_mgr *);
// Firmware OTA updates
//
// This is a low-level OTA API, not intended to be called directly by users.
// Users are provided with a higher-level API:
// - defining MG_OTA_URL enables HTTP pull-based updates
// - mg_http_start_ota() enables push-based updates
//
// However, it is possible to use the mg_ota_* API directly. Below is the
// example code of a function that is called repeatedly. It expects the
// last chink of size 0, which marks the end of the OTA process
//
// ```c
// static void ota(size_t total, size_t offset, void *buf, size_t len) {
//   bool ok = false;
//   if (ofs == 0 && (ok = mg_ota_begin(total)) == false) {
//     mg_ota_end();
//   } else if (len > 0 && (ok = mg_ota_write(buf, len)) == false) {
//     mg_ota_end();
//   } else if (len == 0) {}
//     ok = mg_ota_end();
//   }
//   return ok;
// }
// ```






// Supported OTA targets
#define MG_OTA_NONE 0               // No OTA support
#define MG_OTA_STM32H5 1            // STM32 H5
#define MG_OTA_STM32H7 2            // STM32 H7
#define MG_OTA_STM32H7_DUAL_CORE 3  // STM32 H7 dual core
#define MG_OTA_STM32F 4             // STM32 F7/F4/F2
#define MG_OTA_CH32V307 100         // WCH CH32V307
#define MG_OTA_U2A 200              // Renesas U2A16, U2A8, U2A6
#define MG_OTA_RT1020 300           // IMXRT1020
#define MG_OTA_RT1050 301           // IMXRT1050
#define MG_OTA_RT1060 302           // IMXRT1060
#define MG_OTA_RT1064 303           // IMXRT1064
#define MG_OTA_RT1170 304           // IMXRT1170
#define MG_OTA_MCXN 310             // MCXN947
#define MG_OTA_RW612 320            // FRDM-RW612
#define MG_OTA_FLASH 900            // OTA via internal flash
#define MG_OTA_ESP32 910            // ESP32 OTA implementation
#define MG_OTA_PICOSDK 920          // RP2040/2350 using Pico-SDK hardware_flash
#define MG_OTA_CUSTOM 1000          // Custom implementation

// OTA target. Settable in mongoose_config.h
#ifndef MG_OTA
#define MG_OTA MG_OTA_NONE
#else
#ifndef MG_IRAM
#if defined(__GNUC__)
// Places code block in .iram ELF section. A linker script can place it in RAM
#define MG_IRAM __attribute__((noinline, section(".iram")))
#else
#define MG_IRAM
#endif  // compiler
#endif  // IRAM
#endif  // OTA

// Starts the OTA process. Called once at the beginning of a firmware update.
bool mg_ota_begin(size_t new_firmware_size);

// Writes the next firmware chunk. Called repeatedly until all data is written.
bool mg_ota_write(const void *buf, size_t len);

// Ends the OTA process and commits the written firmware.
bool mg_ota_end(void);

// MG_OTA_ROLLBACK: swap firmware banks and reset. On flash-based OTA, the
// default calls swap_fn() (no-op on single-bank) then resets. Override in
// mongoose_config.h for platforms without mg_flash (ESP32, PicoSDK, etc.).
#ifndef MG_OTA_ROLLBACK
#define MG_OTA_ROLLBACK() \
  do {                    \
    mg_flash->swap_fn();  \
    NVIC_SystemReset();   \
  } while (0)
#endif

// Arm a hardware watchdog. mg_ota_poll() feeds it while in MG_OTA_TESTING state;
// if the event loop hangs, it fires, resets the device, and MG_OTA_FAILED
// triggers MG_OTA_ROLLBACK() on next boot. Timeout is arch-defined.
#ifndef MG_OTA_ROLLBACK_TIMER_START
#define MG_OTA_ROLLBACK_TIMER_START()
#endif

#ifndef MG_OTA_ROLLBACK_TIMER_FEED
#define MG_OTA_ROLLBACK_TIMER_FEED()
#endif

// OTA state values for MG_OTA_STATE_GET / MG_OTA_STATE_SET.
// MG_OTA_CONFIRMED:  firmware is committed, normal operation.
// MG_OTA_TESTING:    new firmware booted for the first time; IWDG is armed.
//                    If the device reboots while in this state, it transitions
//                    to MG_OTA_FAILED and rolls back on the next boot.
// MG_OTA_FAILED:     previous boot did not commit in time; rollback on next boot.
// MG_OTA_ROLLEDBACK: device rolled back to previous firmware. Sent to the OTA
//                    server as boot=3 on every poll until a new OTA succeeds,
//                    which overwrites this state with MG_OTA_TESTING.
enum {
  MG_OTA_CONFIRMED = 0,
  MG_OTA_TESTING = 1,
  MG_OTA_FAILED = 2,
  MG_OTA_ROLLEDBACK = 3
};

// Persistent OTA state storage. Override in mongoose_config.h to persist state across resets.
#ifndef MG_OTA_STATE_GET
#define MG_OTA_STATE_GET() 0
#endif
#ifndef MG_OTA_STATE_SET
#define MG_OTA_STATE_SET(val) (void) (val)
#endif

// Evaluation window for new firmware in seconds. mg_ota_poll() feeds the
// rollback watchdog while in MG_OTA_TESTING state and within this window;
// when it expires, auto-commits if the OTA server confirmed the same version,
// otherwise stops feeding and the watchdog triggers a rollback.
#ifndef MG_OTA_ROLLBACK_TIMEOUT_SECONDS
#define MG_OTA_ROLLBACK_TIMEOUT_SECONDS 120
#endif

// Call once at boot, after clock and backup-domain init (e.g. after HAL_Init()
// + SystemClock_Config() on CubeMX projects, or after hal_clock_init() on
// Mongoose tutorials). The backup domain must be write-accessible so the state
// transition TESTING → FAILED persists across resets.
// Arms the rollback watchdog if new firmware is in TESTING state, rolls back
// immediately if FAILED. mg_ota_poll() feeds the watchdog and auto-commits
// when the server confirms the same version within the window.
// boot (TESTING) → set FAILED → arm IWDG → run firmware
//  ├── server ok + window expires → set CONFIRMED → IWDG resets cleanly
//  └── no server ok + window expires → stop feeding → IWDG fires → FAILED → rollback
#define MG_OTA_BOOT_CHECK()                                        \
  do {                                                             \
    if (MG_OTA_STATE_GET() == MG_OTA_FAILED) {                     \
      MG_OTA_STATE_SET(MG_OTA_ROLLEDBACK);                         \
      MG_INFO(("Commit deadline expired, rolling back"));          \
      MG_OTA_ROLLBACK();                                           \
    } else if (MG_OTA_STATE_GET() == MG_OTA_TESTING) {             \
      MG_OTA_STATE_SET(MG_OTA_FAILED);                             \
      MG_INFO(("New firmware: commit within %u sec or rolls back", \
               (unsigned) MG_OTA_ROLLBACK_TIMEOUT_SECONDS));       \
      MG_OTA_ROLLBACK_TIMER_START();                               \
    }                                                              \
  } while (0)

// Pull-based OTA over HTTP. Fetches metadata_url, which must return a JSON
// object like: { "version": "1.2.3", "url": "FIRMWARE_URL", "size": 324645 }
// If the server version differs from MG_OTA_FIRMWARE_VERSION, downloads
// FIRMWARE_URL and performs the OTA update. fn is called on every outcome:
// NULL on successful flash, "Same version" when already up to date, or an
// error string on failure. Pass NULL to use the default handler which logs
// failures via MG_ERROR.
void mg_ota_url_check(struct mg_mgr *mgr, const char *metadata_url,
                      void (*fn)(const char *error_message));

// Firmware info URL for mg_ota_poll() which calls mg_ota_url_check().
// Example:  "http://mongoose.ws/ota/u/0/ota.json". See http://mongoose.ws/ota/
// Settable in mongoose_config.h
#ifndef MG_OTA_URL
#define MG_OTA_URL NULL
#endif

// OTA status callback function for mg_ota_poll()
#ifndef MG_OTA_STATUS_FN
#define MG_OTA_STATUS_FN NULL
#endif

// Firmware version for mg_ota_poll(). Settable in mongoose_config.h
#ifndef MG_OTA_FIRMWARE_VERSION
#define MG_OTA_FIRMWARE_VERSION "1.0.0"
#endif

// Maximum version string length
#ifndef MG_OTA_MAX_VERSION_LEN
#define MG_OTA_MAX_VERSION_LEN 64
#endif

// How often mg_ota_poll() checks for a firmware update, in seconds
#ifndef MG_OTA_PULL_INTERVAL_SECONDS
#define MG_OTA_PULL_INTERVAL_SECONDS 60
#endif

// Set to 1 to define a custom mg_ota_device_id(). Settable in mongoose_config.h
#ifndef MG_ENABLE_CUSTOM_DEVICE_ID
#define MG_ENABLE_CUSTOM_DEVICE_ID 0
#endif

// Populates buf with a unique device ID string.
// E.g. on STM32, uses the 96-bit MCU UID converted to a hex string.
void mg_ota_device_id(char *buf, size_t len);

// Checks for a firmware update over HTTP. Called automatically by mg_mgr_poll()
// when MG_OTA_URL is set in mongoose_config.h. Do not call directly.
void mg_ota_poll(struct mg_mgr *);




#if (MG_OTA != MG_OTA_NONE && MG_OTA != MG_OTA_CUSTOM) || MG_ENABLE_LFS

struct mg_flash {
  void *start;    // Address at which flash starts
  size_t size;    // Flash size
  size_t secsz;   // Sector size
  size_t align;   // Write alignment
  bool (*write_fn)(void *, const void *, size_t);  // Write function
  bool (*swap_fn)(void);                           // Swap partitions
};

extern struct mg_flash *mg_flash;
bool mg_lfs_init(size_t size);

bool mg_ota_flash_begin(size_t new_firmware_size, struct mg_flash *flash);
bool mg_ota_flash_write(const void *buf, size_t len, struct mg_flash *flash);
bool mg_ota_flash_end(struct mg_flash *flash);

#endif






struct mg_wifi_data {
  char *ssid, *pass;      // STA mode, SSID to connect to
  char *apssid, *appass;  // AP mode, our SSID
  uint32_t apip, apmask;  // AP mode, our IP address and mask
  uint8_t security;       // STA mode, TBD
  uint8_t apsecurity;     // AP mode, TBD
  uint8_t apchannel;      // AP mode, channel to use
  bool apmode;  // start in AP mode; 'false' -> connect to 'ssid' != NULL
};

struct mg_wifi_scan_bss_data {
  struct mg_str SSID;
  char *BSSID;
  int16_t RSSI;
  uint8_t security;
#define MG_WIFI_SECURITY_OPEN 0
#define MG_WIFI_SECURITY_WEP MG_BIT(0)
#define MG_WIFI_SECURITY_WPA MG_BIT(1)
#define MG_WIFI_SECURITY_WPA2 MG_BIT(2)
#define MG_WIFI_SECURITY_WPA3 MG_BIT(3)
#define MG_WIFI_SECURITY_WPA_ENTERPRISE MG_BIT(4)
#define MG_WIFI_SECURITY_WPA2_ENTERPRISE MG_BIT(5)
#define MG_WIFI_SECURITY_WPA3_ENTERPRISE MG_BIT(6)
  uint8_t channel;
  unsigned band : 2;
#define MG_WIFI_BAND_2G 0
#define MG_WIFI_BAND_5G 1
  unsigned has_n : 1;
  unsigned has_ac : 1;
  unsigned has_ax : 1;
};

bool mg_wifi_scan(void);
bool mg_wifi_connect(struct mg_wifi_data *);
bool mg_wifi_disconnect(void);
bool mg_wifi_ap_start(struct mg_wifi_data *);
bool mg_wifi_ap_stop(void);





#if MG_ENABLE_TCPIP

// no config defaults to 0 => Ethernet
enum mg_l2type { MG_TCPIP_L2_ETH = 0, MG_TCPIP_L2_PPP, MG_TCPIP_L2_PPPoE };

#if defined(__DCC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct mg_l2addr {
  union {
    uint8_t mac[6];   // ieee48
    uint64_t ieee64;  // PPP ifcid
    uint16_t ieee16;
  } addr;
};

// L2 using L2 (PPPoE) must inherit the lower layer first, so they overlap
struct eth_data {
  uint16_t vlan_id;
};
struct pppoe_data {  // (struct eth_data *) &pppoe_data = pppoedata.eth
  struct eth_data eth;
};

union mg_l2data {
  struct eth_data eth;
  struct pppoe_data pppoe;
};

#if defined(__DCC__)
#pragma pack(0)
#else
#pragma pack(pop)
#endif


enum mg_l2proto {
  MG_TCPIP_L2PROTO_IPV4 = 0,
  MG_TCPIP_L2PROTO_IPV6,
  MG_TCPIP_L2PROTO_ARP,
  MG_TCPIP_L2PROTO_PPPoE_DISC,
  MG_TCPIP_L2PROTO_PPPoE_SESS
};
enum mg_l2addrtype {
  MG_TCPIP_L2ADDR_BCAST,
  MG_TCPIP_L2ADDR_MCAST,
  MG_TCPIP_L2ADDR_MCAST6
};

#endif








#if MG_ENABLE_TCPIP

// Mongoose TCP/IP network interface
struct mg_tcpip_if;

// Hardware driver interface. Implement all function pointers to plug in a
// new network device. Polling drivers set rx; interrupt/DMA drivers set
// rx=NULL and push frames via mg_tcpip_qwrite() from their ISR instead.
struct mg_tcpip_driver {
  bool (*init)(struct mg_tcpip_if *);                        // One-time hardware init; return false on failure
  size_t (*tx)(const void *, size_t, struct mg_tcpip_if *);  // Transmit one raw frame; return bytes sent
  size_t (*rx)(void *buf, size_t len, struct mg_tcpip_if *); // Read one raw frame; return frame size, 0 if none. NULL for interrupt-driven drivers
  bool (*poll)(struct mg_tcpip_if *, bool);                  // Check link status; second arg is true once per second. Return true=up, false=down
};

// Event handler called by the TCP/IP stack for network-level events.
// ev is one of MG_TCPIP_EV_*; ev_data type is documented per event below.
typedef void (*mg_tcpip_event_handler_t)(struct mg_tcpip_if *ifp, int ev,
                                         void *ev_data);

// clang-format off
// Events delivered to mg_tcpip_if.fn (user) and mg_tcpip_if.pfn (driver).
// Each entry shows the ev_data type to cast inside the handler.
enum {
  MG_TCPIP_EV_STATE_CHANGE,      // IPv4 link state changed       uint8_t *(&ifp->state)
  MG_TCPIP_EV_DHCP_DNS,          // DHCP assigned a DNS server    uint32_t *ipaddr
  MG_TCPIP_EV_DHCP_SNTP,         // DHCP assigned an SNTP server  uint32_t *ipaddr
  MG_TCPIP_EV_ARP,               // Raw ARP frame received        struct mg_str *
  MG_TCPIP_EV_TIMER_1S,          // 1-second periodic tick        NULL
  MG_TCPIP_EV_WIFI_SCAN_RESULT,  // Wi-Fi BSS scan entry          struct mg_wifi_scan_bss_data *
  MG_TCPIP_EV_WIFI_SCAN_END,     // Wi-Fi scan finished           NULL
  MG_TCPIP_EV_WIFI_CONNECT_ERR,  // Wi-Fi connection failed       driver and chip specific
  MG_TCPIP_EV_DRIVER,            // Driver-defined event          driver specific
  MG_TCPIP_EV_STATE6_CHANGE,     // IPv6 link state changed       uint8_t *(&ifp->state6)
  MG_TCPIP_EV_USER               // Starting ID for user-defined events
};
// clang-format on

// Network interface. Zero-initialise, then set the fields marked "user-set"
// before calling mg_tcpip_init(). Fields in the "internal state" section
// below are managed by the stack; read them freely but do not modify them.
struct mg_tcpip_if {
  // User-set configuration
  uint8_t mac[sizeof(struct mg_l2addr)];  // MAC address; must be set to a valid address
  uint32_t ip, mask, gw;                  // Static IPv4 address, netmask, gateway; 0 = use DHCP
  struct mg_str tx;                       // TX frame buffer (managed by the stack)
  bool enable_dhcp_client;                // Enable DHCP client; auto-set if ip==0
  bool enable_dhcp_server;                // Enable DHCP server
  bool enable_get_gateway;                // DHCP server offers itself as the default gateway
  bool enable_req_dns;                    // DHCP client requests a DNS server address
  bool enable_req_sntp;                   // DHCP client requests an SNTP server address
  bool enable_fcs_check;                  // Verify and strip FCS from received frames
  bool enable_mac_check;                  // Drop frames not addressed to this MAC
  bool update_mac_hash_table;             // Signal driver to refresh MAC multicast hash table
  bool is_ip_changed;                     // Set by stack when IP changes; triggers connection restart
  struct mg_tcpip_driver *driver;         // Hardware driver; must be set before mg_tcpip_init()
  void *driver_data;                      // Passed to all driver functions as ifp->driver_data
  mg_tcpip_event_handler_t pfn;           // Driver-level event handler; called before fn
  mg_tcpip_event_handler_t fn;            // User event handler for MG_TCPIP_EV_* events
  struct mg_mgr *mgr;                     // Mongoose event manager; set by mg_tcpip_init()
  struct mg_queue recv_queue;             // Incoming frame queue; pre-size by setting recv_queue.size
  char dhcp_name[MG_TCPIP_DHCPNAME_SIZE]; // Hostname sent in DHCP requests; defaults to "mip"
  uint16_t mtu;                           // IP MTU (max payload size at the IP layer)
  uint16_t framesize;                     // Maximum L2 frame size in bytes
  uint16_t l2mtu;                 	  // L2 frame payload, default net MTU

#if MG_ENABLE_IPV6
  uint64_t ip6ll[2], ip6[2];  // IPv6 link-local and global addresses
  uint8_t prefix[8];          // Global prefix bytes
  uint8_t prefix_len;         // Global prefix length in bits
  uint64_t gw6[2];            // IPv6 default gateway
  bool enable_slaac;          // Enable SLAAC (stateless address autoconfiguration)
  bool enable_dhcp6_client;   // Enable DHCPv6 client (not yet fully implemented)
  bool is_ip6_changed;        // Set by stack when IPv6 address changes
#endif

  // Internal state - read-only for users, managed by the stack
  uint8_t gwmac[sizeof(struct mg_l2addr)];  // Gateway hardware address (resolved via ARP)
  enum mg_l2type l2type;                    // Layer-2 type: Ethernet, PPP, etc. (see l2.h)
  union mg_l2data l2data;                   // Layer-2 config and state (see l2.h)
  char *dns4_url;                           // Active DNS server URL (set from DHCP or config)
  uint64_t now;                             // Current time in ms (updated each mg_mgr_poll)
  uint64_t timer_1000ms;                    // Timestamp of last 1-second tick
  uint64_t lease_expire;                    // DHCP lease expiry time in ms
  uint16_t eport;                           // Next ephemeral port to assign
  volatile uint32_t ndrop;                  // Frames received but dropped (queue full)
  volatile uint32_t nrecv;                  // Total frames received
  volatile uint32_t nsent;                  // Total frames transmitted
  volatile uint32_t nerr;                   // Driver-reported transmit errors
  uint8_t state;                            // Current IPv4 link/IP state (MG_TCPIP_STATE_*)
#define MG_TCPIP_STATE_DOWN 0       // Physical link is down
#define MG_TCPIP_STATE_LINK_UP 1    // Driver reports physical link is up
#define MG_TCPIP_STATE_UP 2         // L2 is ready (e.g. PPP negotiated)
#define MG_TCPIP_STATE_REQ 3        // DHCP REQUESTING: waiting for DHCP offer
#define MG_TCPIP_STATE_IP 4         // IP address assigned; resolving gateway MAC
#define MG_TCPIP_STATE_READY 5      // Fully operational; gateway MAC resolved
  bool gw_ready;                    // Gateway MAC address has been resolved via ARP

#if MG_ENABLE_IPV6
  uint8_t gw6mac[sizeof(struct mg_l2addr)];  // IPv6 gateway hardware address
  uint8_t state6;                            // Current IPv6 link/IP state (MG_TCPIP_STATE_*)
  bool gw6_ready;                            // IPv6 gateway MAC address has been resolved
#endif
};

// Initialises the TCP/IP stack. Calls driver->init(), allocates the TX frame
// buffer (ifp->framesize bytes) and the RX queue. If ifp->ip == 0 and the
// L2 type is not PPP, DHCP client is enabled automatically.
// Must be called once after populating ifp->driver, ifp->mac, and any
// static IP/mask/gw values.
void mg_tcpip_init(struct mg_mgr *, struct mg_tcpip_if *);

// Frees the TX buffer and RX queue allocated by mg_tcpip_init().
void mg_tcpip_free(struct mg_tcpip_if *);

// Pushes a received raw frame into ifp->recv_queue. Used by interrupt-driven
// or DMA-based drivers (those with rx=NULL) to hand frames to the stack from
// an ISR or DMA completion callback. Drops the frame and increments ndrop if
// the queue is full.
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp);

// Sends a broadcast ARP who-has request for ip. mac may be NULL for a plain
// broadcast, or a non-NULL hint to set the target MAC field in the request.
void mg_tcpip_arp_request(struct mg_tcpip_if *ifp, uint32_t ip, uint8_t *mac);

// Built-in hardware drivers. Assign one to mg_tcpip_if.driver before calling
// mg_tcpip_init(). Each driver also requires a corresponding driver_data
// struct; see the matching driver header for details.
extern struct mg_tcpip_driver mg_tcpip_driver_stm32f;
extern struct mg_tcpip_driver mg_tcpip_driver_w5500;
extern struct mg_tcpip_driver mg_tcpip_driver_w5100;
extern struct mg_tcpip_driver mg_tcpip_driver_tm4c;
extern struct mg_tcpip_driver mg_tcpip_driver_tms570;
extern struct mg_tcpip_driver mg_tcpip_driver_stm32h;
extern struct mg_tcpip_driver mg_tcpip_driver_imxrt;
extern struct mg_tcpip_driver mg_tcpip_driver_same54;
extern struct mg_tcpip_driver mg_tcpip_driver_cmsis;
extern struct mg_tcpip_driver mg_tcpip_driver_ra;
extern struct mg_tcpip_driver mg_tcpip_driver_xmc;
extern struct mg_tcpip_driver mg_tcpip_driver_xmc7;
extern struct mg_tcpip_driver mg_tcpip_driver_pico_w;
extern struct mg_tcpip_driver mg_tcpip_driver_rw612;
extern struct mg_tcpip_driver mg_tcpip_driver_cyw;
extern struct mg_tcpip_driver mg_tcpip_driver_nxp_wifi;
extern struct mg_tcpip_driver mg_tcpip_driver_st67w6;
extern struct mg_tcpip_driver mg_tcpip_driver_atcmd;

// SPI bus abstraction for SPI-attached network chips (e.g. W5500).
// Populate and assign to the driver's driver_data field.
struct mg_tcpip_spi {
  void *spi;                                                   // Opaque SPI bus handle passed to all callbacks
  void (*begin)(void *);                                       // Assert chip-select (start of transaction)
  void (*end)(void *);                                         // Deassert chip-select (end of transaction)
  void (*txn)(void *, uint8_t *write, uint8_t *read, size_t); // Full-duplex transfer: write and read len bytes simultaneously
};

// Alignment attributes for DMA-capable Ethernet descriptor buffers.
// Use these on driver RX/TX descriptor arrays where the hardware requires
// specific alignment.
#ifndef MG_8BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_8BYTE_ALIGNED __attribute__((aligned((8U))))
#else
#define MG_8BYTE_ALIGNED
#endif  // compiler
#endif  // 8BYTE_ALIGNED

#ifndef MG_16BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_16BYTE_ALIGNED __attribute__((aligned((16U))))
#else
#define MG_16BYTE_ALIGNED
#endif  // compiler
#endif  // 16BYTE_ALIGNED

#ifndef MG_32BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_32BYTE_ALIGNED __attribute__((aligned((32U))))
#else
#define MG_32BYTE_ALIGNED
#endif  // compiler
#endif  // 32BYTE_ALIGNED

#ifndef MG_64BYTE_ALIGNED
#if defined(__GNUC__)
#define MG_64BYTE_ALIGNED __attribute__((aligned((64U))))
#else
#define MG_64BYTE_ALIGNED
#endif  // compiler
#endif  // 64BYTE_ALIGNED

// Memory section attribute for Ethernet frame buffers. Define to
// __attribute__((section(".eth_ram"))) or similar if the Ethernet DMA
// requires buffers in a specific RAM region.
#ifndef MG_ETH_RAM
#define MG_ETH_RAM
#endif

#endif





#if MG_ENABLE_BSD_SOCKETS

#ifndef MG_ENABLE_BSD_PROTOTYPES
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/un.h>
#else
#include <errno.h>
typedef unsigned int socklen_t;
typedef uint32_t in_addr_t;
struct in_addr { in_addr_t s_addr; };
struct in6_addr { uint8_t s6_addr[16]; };
struct sockaddr { uint16_t sa_family; char sa_data[14]; };
struct sockaddr_in {
  uint16_t sin_family;
  uint16_t sin_port;
  struct in_addr sin_addr;
  char sin_zero[8];
};
struct sockaddr_in6 {
  uint16_t sin6_family;
  uint16_t sin6_port;
  uint32_t sin6_flowinfo;
  struct in6_addr sin6_addr;
  uint32_t sin6_scope_id;
};
struct sockaddr_storage { uint16_t ss_family; char __ss_pad[126]; };
struct hostent {
  char *h_name;
  char **h_aliases;
  int h_addrtype;
  int h_length;
  char **h_addr_list;
};
#define h_addr h_addr_list[0]
struct addrinfo {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  socklen_t ai_addrlen;
  struct sockaddr *ai_addr;
  char *ai_canonname;
  struct addrinfo *ai_next;
};
struct pollfd { int fd; short events; short revents; };
#define AF_INET 2
#define AF_INET6 10
#define AF_UNSPEC 0
#define PF_INET AF_INET
#define PF_INET6 AF_INET6
#define PF_UNSPEC AF_UNSPEC
#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define INADDR_ANY 0
#define INADDR_LOOPBACK 0x7f000001
#define SOL_SOCKET 0xffff
#define SO_REUSEADDR 2
#define SO_REUSEPORT 15
#define SO_KEEPALIVE 9
#define SO_ERROR 4
#define SO_BROADCAST 6
#define SO_RCVBUF 8
#define SO_SNDBUF 7
#define TCP_NODELAY 1
#define MSG_DONTWAIT 0x40
#define MSG_NOSIGNAL 0x4000
#define MSG_PEEK 0x02
#define F_GETFL 3
#define F_SETFL 4
#define O_NONBLOCK 0x0004
#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#define AI_PASSIVE 0x0001
#define NI_MAXHOST 1025
#define NI_MAXSERV 32
#ifndef EAGAIN
#define EAGAIN 11
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif
#ifndef EINPROGRESS
#define EINPROGRESS 36
#endif
#ifndef ENOTCONN
#define ENOTCONN 107
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED 111
#endif
#ifndef ECONNRESET
#define ECONNRESET 104
#endif
#ifndef EADDRINUSE
#define EADDRINUSE 98
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif
#define POLLIN 0x001
#define POLLOUT 0x004
#define POLLERR 0x008
#define POLLHUP 0x010
#define POLLNVAL 0x020
uint16_t htons(uint16_t);
uint16_t ntohs(uint16_t);
uint32_t htonl(uint32_t);
uint32_t ntohl(uint32_t);
#endif  // MG_ENABLE_BSD_PROTOTYPES

#define closesocket(a) close(a)

#ifndef MG_BSD_BACKLOG
#define MG_BSD_BACKLOG 5
#endif

#ifndef MG_BSD_CONNECT_TIMEOUT_MS
#define MG_BSD_CONNECT_TIMEOUT_MS 10000
#endif

// Mongoose-specific API
void mg_bsd_init(void);  // must be called before socket()
void mg_bsd_poll(struct mg_mgr *);  // process pending BSD commands

// Transport backend: implement these OR define MG_ENABLE_FREERTOS for the built-in backend
void *mg_bsd_transport_new(int domain, int type, int proto);
void  mg_bsd_transport_free(void *t);
int   mg_bsd_transport_listen(void *t, const struct sockaddr_in *addr);
void *mg_bsd_transport_accept(void *t, struct sockaddr_in *peer, bool nonblock);
ssize_t mg_bsd_transport_recv(void *t, void *buf, size_t len, bool nonblock);
ssize_t mg_bsd_transport_send(void *t, const void *buf, size_t len, bool nonblock);
int   mg_bsd_transport_connect(void *t, const struct sockaddr_in *addr, bool nonblock);
void  mg_bsd_transport_close(void *t);

// Standard BSD socket API
int socket(int, int, int);
int bind(int, const struct sockaddr *, socklen_t);
int listen(int, int);
int accept(int, struct sockaddr *, socklen_t *);
int connect(int, const struct sockaddr *, socklen_t);
ssize_t send(int, const void *, size_t, int);
ssize_t recv(int, void *, size_t, int);
ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t);
ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *, socklen_t *);
int close(int);
int shutdown(int, int);
int fcntl(int, int, int);
int setsockopt(int, int, int, const void *, socklen_t);
int getsockopt(int, int, int, void *, socklen_t *);
int getsockname(int, struct sockaddr *, socklen_t *);
int getpeername(int, struct sockaddr *, socklen_t *);
int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int poll(struct pollfd *, unsigned int, int);
struct hostent *gethostbyname(const char *);
int getaddrinfo(const char *, const char *, const struct addrinfo *, struct addrinfo **);
void freeaddrinfo(struct addrinfo *);
int inet_pton(int, const char *, void *);
const char *inet_ntop(int, const void *, char *, socklen_t);
in_addr_t inet_addr(const char *);
char *inet_ntoa(struct in_addr);

#endif  // MG_ENABLE_BSD_SOCKETS


struct mg_tcpip_driver_atcmd_data {
  void *usart;                  // Opaque UART/SPI descriptor
  void (*reset)(void *);        // Modem hardware reset
  void (*tx)(void *, uint8_t);  // USART transmit single byte
  int (*rx)(void *);            // USART receive single byte
  const char **script;          // List of AT commands and expected replies
  int script_index;             // Index of the current AT command in the list
  uint64_t deadline;            // AT command deadline in ms
  bool no_byte_stuff;           // Do not perform byte stuffing/unstuffing
  bool link;                    // internal use: done with AT processing
  bool unstuffing;              // internal use: unstuffing state
};


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CMSIS) && MG_ENABLE_DRIVER_CMSIS

#include "Driver_ETH_MAC.h"  // keep this include
#include "Driver_ETH_PHY.h"  // keep this include

#endif


#if MG_ENABLE_TCPIP &&                                          \
    ((defined(MG_ENABLE_DRIVER_CYW) && MG_ENABLE_DRIVER_CYW) || \
     (defined(MG_ENABLE_DRIVER_CYW_SDIO) && MG_ENABLE_DRIVER_CYW_SDIO))

struct mg_tcpip_driver_cyw_firmware {
  const uint8_t *code_addr;
  size_t code_len;
  const uint8_t *nvram_addr;
  size_t nvram_len;
  const uint8_t *clm_addr;
  size_t clm_len;
};

struct mg_tcpip_driver_cyw_data {
  struct mg_wifi_data wifi;
  void *bus;
  struct mg_tcpip_driver_cyw_firmware *fw;
  bool hs;  // use chip "high-speed" mode; otherwise SPI CPOL0 CPHA0 (DS 4.2.3 Table 6)
};

#define MG_TCPIP_DRIVER_INIT(mgr)                              \
  do {                                                         \
    static struct mg_tcpip_driver_cyw_data driver_data_;       \
    static struct mg_tcpip_if mif_;                            \
    MG_SET_WIFI_CONFIG(&mif_, &driver_data_);                  \
    mif_.ip = MG_TCPIP_IP;                                     \
    mif_.mask = MG_TCPIP_MASK;                                 \
    mif_.gw = MG_TCPIP_GW;                                     \
    mif_.driver = &mg_tcpip_driver_cyw;                        \
    mif_.driver_data = &driver_data_;                          \
    mif_.recv_queue.size = 8192;                               \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */    \
    mg_tcpip_init(mgr, &mif_);                                 \
    MG_INFO(("Driver: cyw, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && \
  (defined(MG_ENABLE_DRIVER_IMXRT10) && MG_ENABLE_DRIVER_IMXRT10) || \
  (defined(MG_ENABLE_DRIVER_IMXRT11) && MG_ENABLE_DRIVER_IMXRT11) || \
  (defined(MG_ENABLE_DRIVER_MCXE) && MG_ENABLE_DRIVER_MCXE)

struct mg_tcpip_driver_imxrt_data {
  // MDC clock divider. MDC clock is derived from IPS Bus clock (ipg_clk),
  // must not exceed 2.5MHz. Configuration for clock range 2.36~2.50 MHz
  // 37.5.1.8.2, Table 37-46 : f = ipg_clk / (2(mdc_cr + 1))
  //    ipg_clk       mdc_cr VALUE
  //    --------------------------
  //                  -1  <-- TODO() tell driver to guess the value
  //    25 MHz         4
  //    33 MHz         6
  //    40 MHz         7
  //    50 MHz         9
  //    66 MHz        13
  int mdc_cr;  // Valid values: -1 to 63

  uint8_t phy_addr;  // PHY address
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 2
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 24
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                \
  do {                                                           \
    static struct mg_tcpip_driver_imxrt_data driver_data_;       \
    static struct mg_tcpip_if mif_;                              \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                      \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                   \
    mif_.ip = MG_TCPIP_IP;                                       \
    mif_.mask = MG_TCPIP_MASK;                                   \
    mif_.gw = MG_TCPIP_GW;                                       \
    mif_.driver = &mg_tcpip_driver_imxrt;                        \
    mif_.driver_data = &driver_data_;                            \
    MG_SET_MAC_ADDRESS(mif_.mac);                                \
    mg_tcpip_init(mgr, &mif_);                                   \
    MG_INFO(("Driver: imxrt, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP  && \
    defined(MG_ENABLE_DRIVER_NXP_WIFI) && MG_ENABLE_DRIVER_NXP_WIFI


struct mg_tcpip_driver_nxp_wifi_data {
  struct mg_wifi_data wifi;
};


#define MG_TCPIP_DRIVER_INIT(mgr)                                   \
  do {                                                              \
    static struct mg_tcpip_driver_nxp_wifi_data driver_data_;       \
    static struct mg_tcpip_if mif_;                                 \
    MG_SET_WIFI_CONFIG(&mif_, &driver_data_);                       \
    mif_.ip = MG_TCPIP_IP;                                          \
    mif_.mask = MG_TCPIP_MASK;                                      \
    mif_.gw = MG_TCPIP_GW;                                          \
    mif_.driver = &mg_tcpip_driver_nxp_wifi;                        \
    mif_.driver_data = &driver_data_;                               \
    mif_.recv_queue.size = 8192;                                    \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */         \
    mg_tcpip_init(mgr, &mif_);                                      \
    MG_INFO(("Driver: nxp wifi, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif




struct mg_phy {
  uint16_t (*read_reg)(uint8_t addr, uint8_t reg);
  void (*write_reg)(uint8_t addr, uint8_t reg, uint16_t value);
};

// PHY configuration settings, bitmask
enum {
  // Set if PHY LEDs are connected to ground
  MG_PHY_LEDS_ACTIVE_HIGH = (1 << 0),
  // Set when PHY clocks MAC. Otherwise, MAC clocks PHY
  MG_PHY_CLOCKS_MAC = (1 << 1)
};

enum { MG_PHY_SPEED_10M, MG_PHY_SPEED_100M, MG_PHY_SPEED_1000M };

void mg_phy_init(struct mg_phy *, uint8_t addr, uint8_t config);
bool mg_phy_up(struct mg_phy *, uint8_t addr, bool *full_duplex,
               uint8_t *speed);


#if MG_ENABLE_TCPIP && MG_ARCH == MG_ARCH_PICOSDK && \
    defined(MG_ENABLE_DRIVER_PICO_W) && MG_ENABLE_DRIVER_PICO_W

#include "cyw43.h"              // keep this include
#include "pico/cyw43_arch.h"    // keep this include
#include "pico/unique_id.h"     // keep this include

struct mg_tcpip_driver_pico_w_data {
  struct mg_wifi_data wifi;
};

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_pico_w_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_WIFI_CONFIG(&mif_, &driver_data_);                     \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_pico_w;                        \
    mif_.driver_data = &driver_data_;                             \
    mif_.recv_queue.size = 8192;                                  \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */       \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: pico-w, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && \
  (defined(MG_ENABLE_DRIVER_RA6) && MG_ENABLE_DRIVER_RA6) || \
  (defined(MG_ENABLE_DRIVER_RA8) && MG_ENABLE_DRIVER_RA8)

struct mg_tcpip_driver_ra_data {
  // MDC clock "divider". MDC clock is software generated,
  uint32_t clock;    // core clock frequency in Hz
  uint16_t irqno;    // IRQn, R_ICU->IELSR[irqno]
  uint8_t phy_addr;  // PHY address
};

#ifndef MG_DRIVER_CLK_FREQ
#define MG_DRIVER_CLK_FREQ 100000000UL
#endif

#ifndef MG_DRIVER_IRQ_NO
#define MG_DRIVER_IRQ_NO 0
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                             \
  do {                                                        \
    static struct mg_tcpip_driver_ra_data driver_data_;       \
    static struct mg_tcpip_if mif_;                           \
    driver_data_.clock = MG_DRIVER_CLK_FREQ;                  \
    driver_data_.irqno = MG_DRIVER_IRQ_NO;                    \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                \
    mif_.ip = MG_TCPIP_IP;                                    \
    mif_.mask = MG_TCPIP_MASK;                                \
    mif_.gw = MG_TCPIP_GW;                                    \
    mif_.driver = &mg_tcpip_driver_ra;                        \
    mif_.driver_data = &driver_data_;                         \
    MG_SET_MAC_ADDRESS(mif_.mac);                             \
    mg_tcpip_init(mgr, &mif_);                                \
    MG_INFO(("Driver: ra, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_RW612) && MG_ENABLE_DRIVER_RW612

struct mg_tcpip_driver_rw612_data {
  // 38.1.8 MII Speed Control Register (MSCR)
  // MDC clock frequency must not exceed 2.5 MHz and is calculated as follows:
  // MDC_freq = P_clock / ((mdc_cr + 1) * 2), where P_clock is the
  // peripheral bus clock.
  // IEEE802.3 clause 22 defines a minimum of 10 ns for the hold time on the
  // MDIO output. Depending on the host bus frequency, the setting may need
  // to be increased.
  int mdc_cr;
  int mdc_holdtime; // Valid values: [0-7]
  uint8_t phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 2
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 51
#endif

#ifndef MG_DRIVER_MDC_HOLDTIME
#define MG_DRIVER_MDC_HOLDTIME 3
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_rw612_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_rw612;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: rw612, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_SAME54) && \
    MG_ENABLE_DRIVER_SAME54

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 5
#endif

#ifndef MG_DRIVER_PHY_ADDR
#define MG_DRIVER_PHY_ADDR 0
#endif

struct mg_tcpip_driver_same54_data {
  int mdc_cr;
  uint8_t phy_addr;
};

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_same54_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_DRIVER_PHY_ADDR;                   \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_same54;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: same54, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && \
    (defined(MG_ENABLE_DRIVER_CYW_SDIO) && MG_ENABLE_DRIVER_CYW_SDIO)

// Specific chip/card driver --> SDIO driver --> HAL --> SDIO hw controller

// API with HAL for hardware controller
// - Provide a function to init the controller (external)
// - Provide these functions:
struct mg_tcpip_sdio {
  void *sdio;                    // Opaque SDIO bus descriptor
  void (*cfg)(void *, uint8_t);  // select operating parameters
  // SDIO transaction: send cmd with a possible 1-byte read or write
  bool (*txn)(void *, uint8_t cmd, uint32_t arg, uint32_t *r);
  // SDIO extended transaction: write or read len bytes, using blksz blocks
  bool (*xfr)(void *, bool write, uint32_t arg, uint16_t blksz, uint32_t *,
              uint32_t len, uint32_t *r);
};

// API with driver (e.g.: cyw.c)
// Once the hardware controller has been initialized:
// - Init card: selects the card, sets F0 block size, sets bus width and speed
bool mg_sdio_init(struct mg_tcpip_sdio *sdio);
// - Enable other possible functions (F1 to F7)
bool mg_sdio_enable_f(struct mg_tcpip_sdio *sdio, unsigned int f);
// - Wait for them to be ready
bool mg_sdio_waitready_f(struct mg_tcpip_sdio *sdio, unsigned int f);
// - Set their transfer block length
bool mg_sdio_set_blksz(struct mg_tcpip_sdio *sdio, unsigned int f,
                       uint16_t blksz);
// - Transfer data to/from a function (abstracts from transaction type)
// - Requesting a read transfer > blocksize means block transfer will be used.
// - Drivers must have room to accomodate a whole block transfer, see sdio.c
// - Transfers of > 1 byte --> (uint32_t *) data. 1-byte --> (uint8_t *) data
bool mg_sdio_transfer(struct mg_tcpip_sdio *sdio, bool write, unsigned int f,
                      uint32_t addr, void *data, uint32_t len);

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_ST67W6) && \
    MG_ENABLE_DRIVER_ST67W6

struct mg_tcpip_driver_st67w6_data {
  struct mg_wifi_data wifi;
  void *spi;
  bool (*is_ready)(void);     // return state of module RDY pin
  struct mg_queue send_queue; // decouple tx calls from module polls
};

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_st67w6_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    MG_SET_WIFI_CONFIG(&mif_, &driver_data_);                     \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_st67w6;                        \
    mif_.driver_data = &driver_data_;                             \
    mif_.recv_queue.size = 8192;                                  \
    mif_.mac[0] = 2; /* MAC read from OTP at driver init */       \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: st67w6, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_STM32F) && \
    MG_ENABLE_DRIVER_STM32F

struct mg_tcpip_driver_stm32f_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-216 MHz   HCLK/102       4  <-- value for Nucleo-F* on max speed
  //    216-310 MHz   HCLK/124       5
  //    110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5

  uint8_t phy_addr;  // PHY address
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 4
#endif

#if MG_ARCH == MG_ARCH_CUBE && !defined(MG_ENABLE_ETH_IRQ)
#define MG_ENABLE_ETH_IRQ() NVIC_EnableIRQ(ETH_IRQn)
#elif !defined(MG_ENABLE_ETH_IRQ)
#define MG_ENABLE_ETH_IRQ()
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_stm32f_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_stm32f;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    MG_ENABLE_ETH_IRQ();                                          \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: stm32f, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP
#if !defined(MG_ENABLE_DRIVER_STM32H)
#define MG_ENABLE_DRIVER_STM32H 0
#endif
#if !defined(MG_ENABLE_DRIVER_MCXN)
#define MG_ENABLE_DRIVER_MCXN 0
#endif
#if !defined(MG_ENABLE_DRIVER_STM32N)
#define MG_ENABLE_DRIVER_STM32N 0
#endif
#if MG_ENABLE_DRIVER_STM32H || MG_ENABLE_DRIVER_MCXN || MG_ENABLE_DRIVER_STM32N

struct mg_tcpip_driver_stm32h_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-250 MHz   HCLK/102       4  <-- value for max speed HSI
  //    250-300 MHz   HCLK/124       5  <-- value for Nucleo-H* on CSI
  //    300-500 MHz   HCLK/204       6
  //    500-800 MHz   HCLK/324       7
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5

  uint8_t phy_addr;  // PHY address
  uint8_t phy_conf;  // PHY config
};

#ifndef MG_TCPIP_PHY_CONF
#define MG_TCPIP_PHY_CONF MG_PHY_CLOCKS_MAC
#endif

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 4
#endif

#if MG_ENABLE_DRIVER_STM32H && MG_ARCH == MG_ARCH_CUBE && !defined(MG_ENABLE_ETH_IRQ)
#define MG_ENABLE_ETH_IRQ() NVIC_EnableIRQ(ETH_IRQn)
#elif MG_ENABLE_DRIVER_STM32N && MG_ARCH == MG_ARCH_CUBE && !defined(MG_ENABLE_ETH_IRQ)
#define MG_ENABLE_ETH_IRQ() NVIC_EnableIRQ(ETH1_IRQn)
#elif !defined(MG_ENABLE_ETH_IRQ)
#define MG_ENABLE_ETH_IRQ()
#endif

#if MG_ENABLE_IPV6
#define MG_IPV6_INIT(mif)                                         \
  do {                                                            \
    memcpy(mif.ip6ll, (uint8_t[16]) MG_TCPIP_IPV6_LINKLOCAL, 16);     \
    memcpy(mif.ip6, (uint8_t[16]) MG_TCPIP_GLOBAL, 16);           \
    memcpy(mif.gw6, (uint8_t[16]) MG_TCPIP_GW6, 16);              \
    mif.prefix_len = MG_TCPIP_PREFIX_LEN;                        \
  } while(0)
#else
#define MG_IPV6_INIT(mif)
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_stm32h_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    driver_data_.phy_conf = MG_TCPIP_PHY_CONF;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_stm32h;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    MG_IPV6_INIT(mif_);                                           \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_ENABLE_ETH_IRQ();                                          \
    MG_INFO(("Driver: stm32h, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TM4C) && MG_ENABLE_DRIVER_TM4C

struct mg_tcpip_driver_tm4c_data {
  // MDC clock divider. MDC clock is derived from SYSCLK, must not exceed 2.5MHz
  //    SYSCLK range   DIVIDER   mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    SYSCLK/42      0
  //    100-150 MHz   SYSCLK/62      1  <-- value for EK-TM4C129* on max speed
  //    20-35 MHz     SYSCLK/16      2
  //    35-60 MHz     SYSCLK/26      3
  //    0x4-0xF Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3
};

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 1
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                               \
  do {                                                          \
    static struct mg_tcpip_driver_tm4c_data driver_data_;       \
    static struct mg_tcpip_if mif_;                             \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                     \
    mif_.ip = MG_TCPIP_IP;                                      \
    mif_.mask = MG_TCPIP_MASK;                                  \
    mif_.gw = MG_TCPIP_GW;                                      \
    mif_.driver = &mg_tcpip_driver_tm4c;                        \
    mif_.driver_data = &driver_data_;                           \
    MG_SET_MAC_ADDRESS(mif_.mac);                               \
    mg_tcpip_init(mgr, &mif_);                                  \
    MG_INFO(("Driver: tm4c, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_TMS570) && MG_ENABLE_DRIVER_TMS570
struct mg_tcpip_driver_tms570_data {
  int mdc_cr;
  int phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 1
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                               \
  do {                                                          \
    static struct mg_tcpip_driver_tms570_data driver_data_;     \
    static struct mg_tcpip_if mif_;                             \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                     \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                  \
    mif_.ip = MG_TCPIP_IP;                                      \
    mif_.mask = MG_TCPIP_MASK;                                  \
    mif_.gw = MG_TCPIP_GW;                                      \
    mif_.driver = &mg_tcpip_driver_tms570;                      \
    mif_.driver_data = &driver_data_;                           \
    MG_SET_MAC_ADDRESS(mif_.mac);                               \
    mg_tcpip_init(mgr, &mif_);                                  \
    MG_INFO(("Driver: tms570, MAC: %M", mg_print_mac, mif_.mac));\
  } while (0)
#endif



#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_XMC) && MG_ENABLE_DRIVER_XMC

struct mg_tcpip_driver_xmc_data {
  // 13.2.8.1 Station Management Functions
  // MDC clock divider (). MDC clock is derived from ETH MAC clock
  // It must not exceed 2.5MHz
  // ETH Clock range  DIVIDER       mdc_cr VALUE
  // --------------------------------------------
  //                                     -1  <-- tell driver to guess the value
  // 60-100 MHz       ETH Clock/42        0
  // 100-150 MHz      ETH Clock/62        1
  // 20-35 MHz        ETH Clock/16        2
  // 35-60 MHz        ETH Clock/26        3
  // 150-250 MHz      ETH Clock/102       4
  // 250-300 MHz      ETH Clock/124       5
  // 110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5
  uint8_t phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 4
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_xmc_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_xmc;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: xmc, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_XMC7) && MG_ENABLE_DRIVER_XMC7

struct mg_tcpip_driver_xmc7_data {
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5
  uint8_t phy_addr;
};

#ifndef MG_TCPIP_PHY_ADDR
#define MG_TCPIP_PHY_ADDR 0
#endif

#ifndef MG_DRIVER_MDC_CR
#define MG_DRIVER_MDC_CR 3
#endif

#define MG_TCPIP_DRIVER_INIT(mgr)                                 \
  do {                                                            \
    static struct mg_tcpip_driver_xmc7_data driver_data_;       \
    static struct mg_tcpip_if mif_;                               \
    driver_data_.mdc_cr = MG_DRIVER_MDC_CR;                       \
    driver_data_.phy_addr = MG_TCPIP_PHY_ADDR;                    \
    mif_.ip = MG_TCPIP_IP;                                        \
    mif_.mask = MG_TCPIP_MASK;                                    \
    mif_.gw = MG_TCPIP_GW;                                        \
    mif_.driver = &mg_tcpip_driver_xmc7;                        \
    mif_.driver_data = &driver_data_;                             \
    MG_SET_MAC_ADDRESS(mif_.mac);                                 \
    mg_tcpip_init(mgr, &mif_);                                    \
    MG_INFO(("Driver: xmc7, MAC: %M", mg_print_mac, mif_.mac)); \
  } while (0)

#endif


#ifdef __cplusplus
}
#endif
#endif  // MONGOOSE_H
