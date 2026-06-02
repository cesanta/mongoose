// Logging
//
// Mongoose provides a set of functions and macros for logging. The application can
// use these functions for its own purposes as well as the rest of Mongoose API.

#pragma once

#include "arch.h"
#include "config.h"
#include "fmt.h"

// Log levels
enum { MG_LL_NONE, MG_LL_ERROR, MG_LL_INFO, MG_LL_DEBUG, MG_LL_VERBOSE };

// Set Mongoose logging level. Example: `mg_log_set(MG_LL_INFO);`
extern int mg_log_level;  // Current log level, one of MG_LL_*
#define mg_log_set(level_) mg_log_level = (level_)

void mg_log(const char *fmt, ...);
void mg_log_prefix(int ll, const char *file, int line, const char *fname);

// Log a hex dump of binary data `buf`, `len`.
void mg_hexdump(const void *buf, size_t len);

// Set log printer function which prints one byte. Example:
//
// ```c
// static void print_char(char ch, void *param) {
//   hal_uart_write_char(param, ch);
// }
// ...
// mg_log_set_fn(print_char, USART3);
// ```
void mg_log_set_fn(mg_pfn_t fn, void *param);

#if MG_ENABLE_LOG
#if !defined(_MSC_VER) && \
    (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#define MG___FUNC__ ""
#else
#define MG___FUNC__ __func__  // introduced in C99
#endif
#define MG_LOG(level, args)                                    \
  do {                                                         \
    if ((level) <= mg_log_level) {                             \
      mg_log_prefix((level), __FILE__, __LINE__, MG___FUNC__); \
      mg_log args;                                             \
    }                                                          \
  } while (0)
#else
#define MG_LOG(level, args) \
  do {                      \
    if (0) mg_log args;     \
  } while (0)
#endif

// Logging macros. Note: the argument is exactly like for mg_snprintf(),
// and it should be enclosed in double parenthesis. Example:
// ```c
// MG_INFO(("Conn %lu, recv buf: %.*s", c->id, c->recv.len, c->recv.buf));
// ```
#define MG_ERROR(args) MG_LOG(MG_LL_ERROR, args)
#define MG_INFO(args) MG_LOG(MG_LL_INFO, args)
#define MG_DEBUG(args) MG_LOG(MG_LL_DEBUG, args)
#define MG_VERBOSE(args) MG_LOG(MG_LL_VERBOSE, args)
