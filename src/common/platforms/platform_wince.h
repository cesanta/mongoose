#ifndef CS_COMMON_PLATFORMS_PLATFORM_WINCE_H_
#define CS_COMMON_PLATFORMS_PLATFORM_WINCE_H_

#if CS_PLATFORM == CS_P_WINCE

/*
 * MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
 * MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
 * MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
 * MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
 * MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
 * MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
 * MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
 * MSVC++ 7.0  _MSC_VER == 1300
 * MSVC++ 6.0  _MSC_VER == 1200
 * MSVC++ 5.0  _MSC_VER == 1100
 */
#pragma warning(disable : 4127) /* FD_SET() emits warning, disable it */
#pragma warning(disable : 4204) /* missing c99 support */

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "ws2.lib") /* Linking with WinCE winsock library */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define strdup _strdup

#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#ifndef EAGAIN
#define EAGAIN EWOULDBLOCK
#endif

#ifndef __func__
#define STRX(x) #x
#define STR(x) STRX(x)
#define __func__ __FILE__ ":" STR(__LINE__)
#endif

#define snprintf _snprintf
#define fileno _fileno
#define vsnprintf _vsnprintf
#define sleep(x) Sleep((x) *1000)
#define to64(x) _atoi64(x)
#define rmdir _rmdir

#if defined(_MSC_VER) && _MSC_VER >= 1400
#define fseeko(x, y, z) _fseeki64((x), (y), (z))
#else
#define fseeko(x, y, z) fseek((x), (y), (z))
#endif

typedef int socklen_t;

#if _MSC_VER >= 1700
#include <stdint.h>
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif

typedef SOCKET sock_t;
typedef uint32_t in_addr_t;

#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 4294967295
#endif

#ifndef pid_t
#define pid_t HANDLE
#endif

#define INT64_FMT "I64d"
#define INT64_X_FMT "I64x"
/* TODO(alashkin): check if this is correct */
#define SIZE_T_FMT "u"

#define DIRSEP '\\'
#define CS_DEFINE_DIRENT

#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(x, y) (x) = (y)
#endif
#endif

#ifndef MG_MAX_HTTP_REQUEST_SIZE
#define MG_MAX_HTTP_REQUEST_SIZE 8192
#endif

#ifndef MG_MAX_HTTP_SEND_MBUF
#define MG_MAX_HTTP_SEND_MBUF 4096
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 40
#endif

#ifndef CS_ENABLE_STDIO
#define CS_ENABLE_STDIO 1
#endif

#define abort() DebugBreak();

#ifndef BUFSIZ
#define BUFSIZ 4096
#endif
/*
 * Explicitly disabling MG_ENABLE_THREADS for WinCE
 * because they are enabled for _WIN32 by default
 */
#ifndef MG_ENABLE_THREADS
#define MG_ENABLE_THREADS 0
#endif

#ifndef MG_ENABLE_FILESYSTEM
#define MG_ENABLE_FILESYSTEM 1
#endif

#ifndef MG_NET_IF
#define MG_NET_IF MG_NET_IF_SOCKET
#endif

typedef struct _stati64 {
  uint32_t st_mtime;
  uint32_t st_size;
  uint32_t st_mode;
} cs_stat_t;

/*
 * WinCE 6.0 has a lot of useful definitions in ATL (not windows.h) headers
 * use #ifdefs to avoid conflicts
 */

#ifndef ENOENT
#define ENOENT ERROR_PATH_NOT_FOUND
#endif

#ifndef EACCES
#define EACCES ERROR_ACCESS_DENIED
#endif

#ifndef ENOMEM
#define ENOMEM ERROR_NOT_ENOUGH_MEMORY
#endif

#ifndef _UINTPTR_T_DEFINED
typedef unsigned int *uintptr_t;
#endif

#define _S_IFREG 2
#define _S_IFDIR 4

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFDIR) != 0)
#endif

#ifndef S_ISREG
#define S_ISREG(x) (((x) &_S_IFREG) != 0)
#endif

int open(const char *filename, int oflag, int pmode);
int _wstati64(const wchar_t *path, cs_stat_t *st);
const char *strerror();

#endif /* CS_PLATFORM == CS_P_WINCE */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_WINCE_H_ */
