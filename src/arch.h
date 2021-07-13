#pragma once

#define MG_ARCH_CUSTOM 0
#define MG_ARCH_UNIX 1
#define MG_ARCH_WIN32 2
#define MG_ARCH_ESP32 3
#define MG_ARCH_ESP8266 4
#define MG_ARCH_FREERTOS_TCP 5
#define MG_ARCH_FREERTOS_LWIP 6

#if !defined(MG_ARCH)
#if defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(FREERTOS_IP_H)
#define MG_ARCH MG_ARCH_FREERTOS_TCP
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it."
#endif
#endif  // !defined(MG_ARCH)

#if !defined(PRINTF_LIKE)
#if defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
#define PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#else
#define PRINTF_LIKE(f, a)
#endif
#endif

#if MG_ARCH == MG_ARCH_CUSTOM
#include <mongoose_custom.h>
#endif

#include "arch_esp32.h"
#include "arch_esp8266.h"
#include "arch_freertos_lwip.h"
#include "arch_freertos_tcp.h"
#include "arch_unix.h"
#include "arch_win32.h"
