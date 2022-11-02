#pragma once

#define MG_ARCH_CUSTOM 0
#define MG_ARCH_UNIX 1
#define MG_ARCH_WIN32 2
#define MG_ARCH_ESP32 3
#define MG_ARCH_ESP8266 4
#define MG_ARCH_FREERTOS_TCP 5
#define MG_ARCH_FREERTOS_LWIP 6
#define MG_ARCH_AZURERTOS 7
#define MG_ARCH_RTX_LWIP 8
#define MG_ARCH_ZEPHYR 9
#define MG_ARCH_NEWLIB 10
#define MG_ARCH_RTX 11
#define MG_ARCH_TIRTOS 12
#define MG_ARCH_RP2040 13

#if !defined(MG_ARCH)
#if defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(__ZEPHYR__)
#define MG_ARCH MG_ARCH_ZEPHYR
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(FREERTOS_IP_H)
#define MG_ARCH MG_ARCH_FREERTOS_TCP
#elif defined(AZURE_RTOS_THREADX)
#define MG_ARCH MG_ARCH_AZURERTOS
#elif defined(PICO_TARGET_NAME)
#define MG_ARCH MG_ARCH_RP2040
#endif

#if !defined(MG_ARCH) || (MG_ARCH == MG_ARCH_CUSTOM)
#include "mongoose_custom.h"  // keep this include
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it. Set -D MG_ARCH=..."
#endif
#endif  // !defined(MG_ARCH)

// http://esr.ibiblio.org/?p=5095
#define MG_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#include "arch_esp32.h"
#include "arch_esp8266.h"
#include "arch_freertos_lwip.h"
#include "arch_freertos_tcp.h"
#include "arch_newlib.h"
#include "arch_rtx.h"
#include "arch_rtx_lwip.h"
#include "arch_unix.h"
#include "arch_win32.h"
#include "arch_zephyr.h"
