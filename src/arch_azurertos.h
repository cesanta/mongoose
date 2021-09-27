#pragma once

#if MG_ARCH == MG_ARCH_AZURERTOS

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
 
#include <tx_api.h>
#include <fx_api.h>

#include <tx_port.h>
#include <nx_port.h>
#include <nx_api.h>
#include <nx_bsd.h>

#ifdef __REDLIB__
#define va_copy(d,s)__builtin_va_copy(d,s)
#endif

#define PATH_MAX FX_MAXIMUM_PATH
#define MG_DIRSEP '\\'

#define socklen_t int
#define closesocket(x) soc_close(x)
#define gmtime_r(a, b) gmtime(a)
#define MG_INT64_FMT "%lld"

static __inline struct tm *localtime_r(time_t *t, struct tm *tm) {
  (void) tm;
  return localtime(t);
}

#undef FOPEN_MAX

#endif
