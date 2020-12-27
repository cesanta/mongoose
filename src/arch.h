#pragma once

#define MG_ARCH_CUSTOM 0
#define MG_ARCH_UNIX 1
#define MG_ARCH_WIN32 2
#define MG_ARCH_ESP32 3
#define MG_ARCH_ESP8266 4
#define MG_ARCH_CC3100 5
#define MG_ARCH_CC3200 6
#define MG_ARCH_CC3220 7
#define MG_ARCH_MSP432 8
#define MG_ARCH_TM4C129 9
#define MG_ARCH_MBED 10
#define MG_ARCH_WINCE 11
#define MG_ARCH_NXP_LPC 12
#define MG_ARCH_NXP_KINETIS 13
#define MG_ARCH_NRF51 14
#define MG_ARCH_NRF52 15
#define MG_ARCH_PIC32 16
#define MG_ARCH_RS14100 17
#define MG_ARCH_STM32 18
#define MG_ARCH_FREERTOS 19

#if !defined(MG_ARCH)
#if defined(TARGET_IS_MSP432P4XX) || defined(__MSP432P401R__)
#define MG_ARCH MG_ARCH_MSP432
#elif defined(cc3200) || defined(TARGET_IS_CC3200)
#define MG_ARCH MG_ARCH_CC3200
#elif defined(cc3220) || defined(TARGET_IS_CC3220)
#define MG_ARCH MG_ARCH_CC3220
#elif defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(WINCE)
#define MG_ARCH MG_ARCH_WINCE
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(__MBED__)
#define MG_ARCH MG_ARCH_MBED
#elif defined(__USE_LPCOPEN)
#define MG_ARCH MG_ARCH_NXP_LPC
#elif defined(FRDM_K64F) || defined(FREEDOM)
#define MG_ARCH MG_ARCH_NXP_KINETIS
#elif defined(PIC32)
#define MG_ARCH MG_ARCH_PIC32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(TARGET_IS_TM4C129_RA0) || defined(TARGET_IS_TM4C129_RA1) || \
    defined(TARGET_IS_TM4C129_RA2)
#define MG_ARCH MG_ARCH_TM4C129
#elif defined(RS14100)
#define MG_ARCH MG_ARCH_RS14100
#elif defined(STM32)
#define MG_ARCH MG_ARCH_STM32
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it."
#endif
#endif  // !defined(MG_ARCH)

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Standard C headers
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if MG_ARCH == MG_ARCH_CUSTOM
#include <mongoose_custom.h>
#endif

#include "arch_esp32.h"
#include "arch_esp8266.h"
#include "arch_freertos.h"
#include "arch_unix.h"
#include "arch_win32.h"

#if !defined(PRINTF_LIKE)
#if defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
#define PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#else
#define PRINTF_LIKE(f, a)
#endif
#endif
