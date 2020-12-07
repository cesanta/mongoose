#pragma once

#if MG_ARCH == MG_ARCH_WIN32

#if defined(_MSC_VER) && _MSC_VER < 1700
#define __func__ ""
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int bool;
enum { false = 0, true = 1 };
#else
#include <stdbool.h>
#include <stdint.h>
#endif

#include <winsock2.h>
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strdup(x) _strdup(x)
#define sleep(x) Sleep((x) *1000)
typedef int socklen_t;
#define PATH_MAX MAX_PATH
#define MG_DIRSEP '\\'
#define inline __inline
#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#define realpath(a, b) _fullpath((b), (a), 512)
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

#endif
