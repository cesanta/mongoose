#pragma once

#if MG_ARCH == MG_ARCH_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef enum { false = 0, true = 1 } bool;
#else
#include <stdbool.h>
#include <stdint.h>
#include <ws2tcpip.h>
#endif

#include <winsock2.h>

// Protect from calls like std::snprintf in app code
// See https://github.com/cesanta/mongoose/issues/1047
#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strdup(x) _strdup(x)
#endif

typedef unsigned suseconds_t;
typedef int socklen_t;
#define MG_DIRSEP '\\'
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#define realpath(a, b) _fullpath((b), (a), MG_PATH_MAX)
#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(x, y) (x) = (y)
#endif
#endif
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#define MG_INT64_FMT "%I64d"

// https://lgtm.com/rules/2154840805/ -gmtime, localtime, ctime and asctime
static __inline struct tm *gmtime_r(time_t *t, struct tm *tm) {
  (void) tm;
  return gmtime(t);
}

static __inline struct tm *localtime_r(time_t *t, struct tm *tm) {
  (void) tm;
  return localtime(t);
}

#endif
