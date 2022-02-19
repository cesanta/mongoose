#pragma once

#if MG_ARCH == MG_ARCH_FREERTOS_LWIP

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(__GNUC__)
#include <sys/stat.h>
#include <sys/time.h>
#else
struct timeval {
  time_t tv_sec;
  long tv_usec;
};
#endif

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/sockets.h>

#if LWIP_SOCKET != 1
// Sockets support disabled in LWIP by default
#error Set LWIP_SOCKET variable to 1 (in lwipopts.h)
#endif

// Re-route calloc/free to the FreeRTOS's functions, don't use stdlib
static inline void *mg_calloc(int cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size);
  return p;
}
#define calloc(a, b) mg_calloc((a), (b))
#define free(a) vPortFree(a)
#define malloc(a) pvPortMalloc(a)

#define mkdir(a, b) (-1)

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

#ifndef MG_PATH_MAX
#define MG_PATH_MAX 128
#endif

#endif  // MG_ARCH == MG_ARCH_FREERTOS_LWIP
