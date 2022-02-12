#pragma once

#include "arch.h"
#include "config.h"

enum { MG_LL_NONE, MG_LL_ERROR, MG_LL_INFO, MG_LL_DEBUG, MG_LL_VERBOSE };
void mg_log(const char *fmt, ...) PRINTF_LIKE(1, 2);
bool mg_log_prefix(int ll, const char *file, int line, const char *fname);
void mg_log_set(const char *spec);
void mg_log_set_callback(void (*fn)(const void *, size_t, void *), void *param);

// Let the compiler always see the log invocation in order to check parameters
// For MG_ENABLE_LOG=0 case, the call will be optimised out, anyway

#if MG_ENABLE_LOG

#define MG_LOG(level, args)                                                \
  do {                                                                     \
    if (mg_log_prefix((level), __FILE__, __LINE__, __func__)) mg_log args; \
  } while (0)

#else

#define MG_LOG(level, args) \
  do {                      \
    (void) level;           \
    if (0) mg_log args;     \
  } while (0)

#endif

#define MG_ERROR(args) MG_LOG(MG_LL_ERROR, args)
#define MG_INFO(args) MG_LOG(MG_LL_INFO, args)
#define MG_DEBUG(args) MG_LOG(MG_LL_DEBUG, args)
#define MG_VERBOSE(args) MG_LOG(MG_LL_VERBOSE, args)
