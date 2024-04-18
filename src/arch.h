#pragma once

#define MG_ARCH_CUSTOM 0        // User creates its own mongoose_config.h
#define MG_ARCH_UNIX 1          // Linux, BSD, Mac, ...
#define MG_ARCH_WIN32 2         // Windows
#define MG_ARCH_ESP32 3         // ESP32
#define MG_ARCH_ESP8266 4       // ESP8266
#define MG_ARCH_FREERTOS 5      // FreeRTOS
#define MG_ARCH_AZURERTOS 6     // MS Azure RTOS
#define MG_ARCH_ZEPHYR 7        // Zephyr RTOS
#define MG_ARCH_NEWLIB 8        // Bare metal ARM
#define MG_ARCH_CMSIS_RTOS1 9   // CMSIS-RTOS API v1 (Keil RTX)
#define MG_ARCH_TIRTOS 10       // Texas Semi TI-RTOS
#define MG_ARCH_RP2040 11       // Raspberry Pi RP2040
#define MG_ARCH_ARMCC 12        // Keil MDK-Core with Configuration Wizard
#define MG_ARCH_CMSIS_RTOS2 13  // CMSIS-RTOS API v2 (Keil RTX5, FreeRTOS)
#define MG_ARCH_RTTHREAD 14     // RT-Thread RTOS

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
#error "MG_ARCH is not specified and we couldn't guess it. Define MG_ARCH=... in your compiler"
#endif

// http://esr.ibiblio.org/?p=5095
#define MG_BIG_ENDIAN (*(uint16_t *) "\0\xff" < 0x100)

#include "arch_esp32.h"
#include "arch_esp8266.h"
#include "arch_freertos.h"
#include "arch_newlib.h"
#include "arch_rtx.h"
#include "arch_unix.h"
#include "arch_win32.h"
#include "arch_zephyr.h"

#include "net_ft.h"
#include "net_lwip.h"
#include "net_rl.h"
