#pragma once

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
