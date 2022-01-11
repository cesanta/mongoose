#pragma once

#if MG_ARCH == MG_ARCH_AZURERTOS

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <fx_api.h>
#include <tx_api.h>

#include <nx_api.h>
#include <nx_bsd.h>
#include <nx_port.h>
#include <tx_port.h>

#ifdef __REDLIB__
#define va_copy(d, s) __builtin_va_copy(d, s)
#endif

#define PATH_MAX FX_MAXIMUM_PATH
#define MG_DIRSEP '\\'

#define socklen_t int
#define closesocket(x) soc_close(x)
#define gmtime_r(a, b) gmtime(a)
#define MG_INT64_FMT "%lld"

static __inline struct tm *localtime_r(const time_t *t, struct tm *tm) {
  struct tm *x = localtime(t);
  *tm = *x;
  return tm;
}

#undef FOPEN_MAX

#endif
