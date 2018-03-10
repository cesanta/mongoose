/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#include "common/cs_dbg.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common/cs_time.h"
#include "common/str_util.h"

enum cs_log_level cs_log_threshold WEAK =
#if CS_ENABLE_DEBUG
    LL_VERBOSE_DEBUG;
#else
    LL_ERROR;
#endif

static char *s_filter_pattern = NULL;
static size_t s_filter_pattern_len;

void cs_log_set_filter(const char *pattern) WEAK;

#if CS_ENABLE_STDIO

FILE *cs_log_file WEAK = NULL;

#if CS_LOG_ENABLE_TS_DIFF
double cs_log_ts WEAK;
#endif

enum cs_log_level cs_log_cur_msg_level WEAK = LL_NONE;

void cs_log_set_filter(const char *pattern) {
  free(s_filter_pattern);
  if (pattern != NULL) {
    s_filter_pattern = strdup(pattern);
    s_filter_pattern_len = strlen(pattern);
  } else {
    s_filter_pattern = NULL;
    s_filter_pattern_len = 0;
  }
}

int cs_log_print_prefix(enum cs_log_level, const char *, const char *) WEAK;
int cs_log_print_prefix(enum cs_log_level level, const char *func,
                        const char *filename) {
  char prefix[21];

  if (level > cs_log_threshold) return 0;
  if (s_filter_pattern != NULL &&
      mg_match_prefix(s_filter_pattern, s_filter_pattern_len, func) == 0 &&
      mg_match_prefix(s_filter_pattern, s_filter_pattern_len, filename) == 0) {
    return 0;
  }

  strncpy(prefix, func, 20);
  prefix[20] = '\0';
  if (cs_log_file == NULL) cs_log_file = stderr;
  cs_log_cur_msg_level = level;
  fprintf(cs_log_file, "%-20s ", prefix);
#if CS_LOG_ENABLE_TS_DIFF
  {
    double now = cs_time();
    fprintf(cs_log_file, "%7u ", (unsigned int) ((now - cs_log_ts) * 1000000));
    cs_log_ts = now;
  }
#endif
  return 1;
}

void cs_log_printf(const char *fmt, ...) WEAK;
void cs_log_printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(cs_log_file, fmt, ap);
  va_end(ap);
  fputc('\n', cs_log_file);
  fflush(cs_log_file);
  cs_log_cur_msg_level = LL_NONE;
}

void cs_log_set_file(FILE *file) WEAK;
void cs_log_set_file(FILE *file) {
  cs_log_file = file;
}

#else

void cs_log_set_filter(const char *pattern) {
  (void) pattern;
}

#endif /* CS_ENABLE_STDIO */

void cs_log_set_level(enum cs_log_level level) WEAK;
void cs_log_set_level(enum cs_log_level level) {
  cs_log_threshold = level;
#if CS_LOG_ENABLE_TS_DIFF && CS_ENABLE_STDIO
  cs_log_ts = cs_time();
#endif
}
