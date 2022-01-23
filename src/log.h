#pragma once

#include "arch.h"
#include "config.h"

enum { LL_NONE, LL_ERROR, LL_INFO, LL_DEBUG, LL_VERBOSE_DEBUG };
void mg_log(const char *fmt, ...) PRINTF_LIKE(1, 2);
bool mg_log_prefix(int ll, const char *file, int line, const char *fname);
void mg_log_set(const char *spec);
void mg_log_set_callback(void (*fn)(const void *, size_t, void *), void *param);

// Let the compiler always see the log invocation in order to check parameters
// For MG_ENABLE_LOG=0 case, the call will be optimised out, anyway

#if MG_ENABLE_LOG

#define LOG(level, args)                                                   \
  do {                                                                     \
    if (mg_log_prefix((level), __FILE__, __LINE__, __func__)) mg_log args; \
  } while (0)

#else

#define LOG(level, args) \
  do {                   \
    (void) level;        \
    if (0) mg_log args;  \
  } while (0)

#endif
