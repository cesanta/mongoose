#pragma once

#include "arch.h"
#include "config.h"

#if MG_ENABLE_MGOS
#include <common/cs_dbg.h>
#else
#if MG_ENABLE_LOG
#define LOG(level, args)                                                   \
  do {                                                                     \
    if (mg_log_prefix((level), __FILE__, __LINE__, __func__)) mg_log args; \
  } while (0)
enum { LL_NONE, LL_ERROR, LL_INFO, LL_DEBUG, LL_VERBOSE_DEBUG };
bool mg_log_prefix(int ll, const char *file, int line, const char *fname);
void mg_log(const char *fmt, ...) PRINTF_LIKE(1, 2);
void mg_log_set(const char *spec);
void mg_log_set_callback(void (*fn)(const void *, int, void *), void *param);
#else
#define LOG(level, args)
#endif
#endif
