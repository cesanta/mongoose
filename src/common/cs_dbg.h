/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CS_COMMON_CS_DBG_H_
#define CS_COMMON_CS_DBG_H_

#include "common/platform.h"

#if CS_ENABLE_STDIO
#include <stdio.h>
#endif

#ifndef CS_ENABLE_DEBUG
#define CS_ENABLE_DEBUG 0
#endif

#ifndef CS_LOG_ENABLE_TS_DIFF
#define CS_LOG_ENABLE_TS_DIFF 0
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Log level; `LL_INFO` is the default. Use `cs_log_set_level()` to change it.
 */
enum cs_log_level {
  LL_NONE = -1,
  LL_ERROR = 0,
  LL_WARN = 1,
  LL_INFO = 2,
  LL_DEBUG = 3,
  LL_VERBOSE_DEBUG = 4,

  _LL_MIN = -2,
  _LL_MAX = 5,
};

/*
 * Set max log level to print; messages with the level above the given one will
 * not be printed.
 */
void cs_log_set_level(enum cs_log_level level);

/*
 * Set log filter. NULL (a default) logs everything.
 * Otherwise, function name and file name will be tested against the given
 * pattern, and only matching messages will be printed.
 *
 * For the pattern syntax, refer to `mg_match_prefix()` in `str_util.h`.
 *
 * Example:
 * ```c
 * void foo(void) {
 *   LOG(LL_INFO, ("hello from foo"));
 * }
 *
 * void bar(void) {
 *   LOG(LL_INFO, ("hello from bar"));
 * }
 *
 * void test(void) {
 *   cs_log_set_filter(NULL);
 *   foo();
 *   bar();
 *
 *   cs_log_set_filter("f*");
 *   foo();
 *   bar(); // Will NOT print anything
 *
 *   cs_log_set_filter("bar");
 *   foo(); // Will NOT print anything
 *   bar();
 * }
 * ```
 */
void cs_log_set_filter(const char *pattern);

/*
 * Helper function which prints message prefix with the given `level`, function
 * name `func` and `filename`. If message should be printed (accordingly to the
 * current log level and filter), prints the prefix and returns 1, otherwise
 * returns 0.
 *
 * Clients should typically just use `LOG()` macro.
 */
int cs_log_print_prefix(enum cs_log_level level, const char *func,
                        const char *filename);

extern enum cs_log_level cs_log_threshold;

#if CS_ENABLE_STDIO

/*
 * Set file to write logs into. If `NULL`, logs go to `stderr`.
 */
void cs_log_set_file(FILE *file);

/*
 * Prints log to the current log file, appends "\n" in the end and flushes the
 * stream.
 */
void cs_log_printf(const char *fmt, ...) PRINTF_LIKE(1, 2);

#if CS_ENABLE_STDIO

/*
 * Format and print message `x` with the given level `l`. Example:
 *
 * ```c
 * LOG(LL_INFO, ("my info message: %d", 123));
 * LOG(LL_DEBUG, ("my debug message: %d", 123));
 * ```
 */
#define LOG(l, x)                                                    \
  do {                                                               \
    if (cs_log_print_prefix(l, __func__, __FILE__)) cs_log_printf x; \
  } while (0)

#else

#define LOG(l, x) ((void) l)

#endif

#ifndef CS_NDEBUG

/*
 * Shortcut for `LOG(LL_VERBOSE_DEBUG, (...))`
 */
#define DBG(x) LOG(LL_VERBOSE_DEBUG, x)

#else /* NDEBUG */

#define DBG(x)

#endif

#else /* CS_ENABLE_STDIO */

#define LOG(l, x)
#define DBG(x)

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_CS_DBG_H_ */
