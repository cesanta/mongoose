#pragma once

#if MG_ARCH == MG_ARCH_FREERTOS

#include <ctype.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // rand(), strtol(), atoi()
#include <string.h>
#if defined(__ARMCC_VERSION)
#define mode_t size_t
#include <time.h>
#else
#include <sys/stat.h>
#endif

#include <FreeRTOS.h>
#include <task.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

#define calloc(a, b) mg_calloc(a, b)
#define free(a) vPortFree(a)
#define malloc(a) pvPortMalloc(a)
#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)

// Re-route calloc/free to the FreeRTOS's functions, don't use stdlib
static inline void *mg_calloc(size_t cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size * cnt);
  return p;
}

#define mkdir(a, b) mg_mkdir(a, b)
static inline int mg_mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

#endif  // MG_ARCH == MG_ARCH_FREERTOS
