/*
* Copyright (c) 2015 Cesanta Software Limited
* All rights reserved
*/

#include <ctype.h>
#include <sys/time.h>
#include <stdint.h>

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "esp_common.h"

/* Makes fprintf(stdout) and stderr work. */
_ssize_t _write_r(struct _reent *r, int fd, void *buf, size_t len) {
  if (fd == 1 || fd == 2) {
    size_t i;
    for (i = 0; i < len; i++) {
      os_putc(((char *) buf)[i]);
    }
    return len;
  }
  return -1;
}

/*
 * You'll need to implement _open_r and friends if you want file operations. See
 * https://github.com/cesanta/mongoose-os/blob/master/fw/platforms/esp8266/user/esp_fs.c
 * for SPIFFS-based implementation.
 */

void abort(void) __attribute__((no_instrument_function));
void abort(void) {
  /* cause an unaligned access exception, that will drop you into gdb */
  *(int *) 1 = 1;
  while (1)
    ; /* avoid gcc warning because stdlib abort() has noreturn attribute */
}

void _exit(int status) {
  abort();
}

/*
 * This will prevent counter wrap if time is read regularly.
 * At least Mongoose poll queries time, so we're covered.
 */
int _gettimeofday_r(struct _reent *r, struct timeval *tp, void *tzp) {
  static uint32_t prev_time = 0;
  static uint32_t num_overflows = 0;
  uint32_t time = system_get_time();
  uint64_t time64 = time;
  if (prev_time > 0 && time < prev_time) num_overflows++;
  time64 += (((uint64_t) num_overflows) * (1ULL << 32));
  tp->tv_sec = time64 / 1000000ULL;
  tp->tv_usec = time64 % 1000000ULL;
  prev_time = time;
  return 0;
  (void) r;
  (void) tzp;
}
