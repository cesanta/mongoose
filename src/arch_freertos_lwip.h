#pragma once

#if MG_ARCH == MG_ARCH_FREERTOS_LWIP

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__GNUC__)
#include <sys/time.h>
#else
typedef long suseconds_t;
struct timeval {
  time_t tv_sec;
  suseconds_t tv_usec;
};
#endif

#if MG_ENABLE_FS
#include <sys/stat.h>
#endif

#include <FreeRTOS.h>
#include <task.h>

#include <lwip/sockets.h>

#define MG_INT64_FMT "%lld"
#define MG_DIRSEP '/'

// Re-route calloc/free to the FreeRTOS's functions, don't use stdlib
static inline void *mg_calloc(int cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size);
  return p;
}
#define calloc(a, b) mg_calloc((a), (b))
#define free(a) vPortFree(a)
#define malloc(a) pvPortMalloc(a)

#define gmtime_r(a, b) gmtime(a)

#endif  // MG_ARCH == MG_ARCH_FREERTOS_LWIP
