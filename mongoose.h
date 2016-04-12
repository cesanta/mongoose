/*
 * Copyright (c) 2004-2013 Sergey Lyubka
 * Copyright (c) 2013-2015 Cesanta Software Limited
 * All rights reserved
 *
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#ifndef CS_MONGOOSE_SRC_COMMON_H_
#define CS_MONGOOSE_SRC_COMMON_H_

#define MG_VERSION "6.4"

/* Local tweaks, applied before any of Mongoose's own headers. */
#ifdef MG_LOCALS
#include <mg_locals.h>
#endif

#if defined(MG_ENABLE_DEBUG) && !defined(CS_ENABLE_DEBUG)
#define CS_ENABLE_DEBUG
#endif
#if defined(MG_DISABLE_STDIO) && !defined(CS_DISABLE_STDIO)
#define CS_DISABLE_STDIO
#elif defined(CS_DISABLE_STDIO) && !defined(MG_DISABLE_STDIO)
#define MG_DISABLE_STDIO
#endif

/* All of the below features depend on filesystem access, disable them. */
#ifdef MG_DISABLE_FILESYSTEM
#ifndef MG_DISABLE_DAV
#define MG_DISABLE_DAV
#endif
#ifndef MG_DISABLE_CGI
#define MG_DISABLE_CGI
#endif
#ifndef MG_DISABLE_DIRECTORY_LISTING
#define MG_DISABLE_DIRECTORY_LISTING
#endif
#ifndef MG_DISABLE_DAV
#define MG_DISABLE_DAV
#endif
#endif /* MG_DISABLE_FILESYSTEM */

#ifdef MG_NO_BSD_SOCKETS
#ifndef MG_DISABLE_SYNC_RESOLVER
#define MG_DISABLE_SYNC_RESOLVER
#endif
#ifndef MG_DISABLE_SOCKETPAIR
#define MG_DISABLE_SOCKETPAIR
#endif
#endif /* MG_NO_BSD_SOCKETS */


#endif /* CS_MONGOOSE_SRC_COMMON_H_ */
#ifndef CS_COMMON_PLATFORM_H_
#define CS_COMMON_PLATFORM_H_

/*
 * For the "custom" platform, includes and dependencies can be
 * provided through mg_locals.h.
 */
#define CS_P_CUSTOM 0
#define CS_P_UNIX 1
#define CS_P_WINDOWS 2
#define CS_P_ESP_LWIP 3
#define CS_P_CC3200 4
#define CS_P_MSP432 5

/* If not specified explicitly, we guess platform by defines. */
#ifndef CS_PLATFORM

#if defined(TARGET_IS_MSP432P4XX) || defined(__MSP432P401R__)

#define CS_PLATFORM CS_P_MSP432
#elif defined(cc3200)
#define CS_PLATFORM CS_P_CC3200
#elif defined(__unix__) || defined(__APPLE__)
#define CS_PLATFORM CS_P_UNIX
#elif defined(_WIN32)
#define CS_PLATFORM CS_P_WINDOWS
#endif

#ifndef CS_PLATFORM
#error "CS_PLATFORM is not specified and we couldn't guess it."
#endif

#endif /* !defined(CS_PLATFORM) */


/* Common stuff */

#ifdef __GNUC__
#define NORETURN __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define NOINLINE __attribute__((noinline))
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define NORETURN
#define UNUSED
#define NOINLINE
#define WARN_UNUSED_RESULT
#endif /* __GNUC__ */

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#endif

#endif /* CS_COMMON_PLATFORM_H_ */
#ifndef CS_COMMON_PLATFORMS_PLATFORM_WINDOWS_H_
#define CS_COMMON_PLATFORMS_PLATFORM_WINDOWS_H_
#if CS_PLATFORM == CS_P_WINDOWS

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
#ifdef _MSC_VER
#pragma warning(disable : 4127) /* FD_SET() emits warning, disable it */
#pragma warning(disable : 4204) /* missing c99 support */
#endif

#include <assert.h>
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#define random() rand()
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib") /* Linking with winsock library */
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>

#ifndef EINPROGRESS
#define EINPROGRESS WSAEINPROGRESS
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
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
#define popen(x, y) _popen((x), (y))
#define pclose(x) _pclose(x)
#define rmdir _rmdir
#if defined(_MSC_VER) && _MSC_VER >= 1400
#define fseeko(x, y, z) _fseeki64((x), (y), (z))
#else
#define fseeko(x, y, z) fseek((x), (y), (z))
#endif
#define random() rand()
typedef int socklen_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
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
#define SIZE_T_FMT "Iu"
#ifdef __MINGW32__
typedef struct stat cs_stat_t;
#else
typedef struct _stati64 cs_stat_t;
#endif
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(x) (((x) &_S_IFMT) == _S_IFREG)
#endif
#define DIRSEP '\\'

/* POSIX opendir/closedir/readdir API for Windows. */
struct dirent {
  char d_name[MAX_PATH];
};

typedef struct DIR {
  HANDLE handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;

DIR *opendir(const char *name);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);

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

#endif /* CS_PLATFORM == CS_P_WINDOWS */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_WINDOWS_H_ */
#ifndef CS_COMMON_PLATFORMS_PLATFORM_UNIX_H_
#define CS_COMMON_PLATFORMS_PLATFORM_UNIX_H_
#if CS_PLATFORM == CS_P_UNIX

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

/* <inttypes.h> wants this for C++ */
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

/* C++ wants that for INT64_MAX */
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

/* Enable fseeko() and ftello() functions */
#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif

/* Enable 64-bit file offsets */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * osx correctly avoids defining strtoll when compiling in strict ansi mode.
 * We require strtoll, and if your embedded pre-c99 compiler lacks one, please
 * implement a shim.
 */
#if !(defined(__DARWIN_C_LEVEL) && __DARWIN_C_LEVEL >= 200809L)
long long strtoll(const char *, char **, int);
#endif

typedef int sock_t;
#define INVALID_SOCKET (-1)
#define SIZE_T_FMT "zu"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

#ifndef va_copy
#ifdef __va_copy
#define va_copy __va_copy
#else
#define va_copy(x, y) (x) = (y)
#endif
#endif

#define closesocket(x) close(x)

#ifndef MG_MAX_HTTP_REQUEST_SIZE
#define MG_MAX_HTTP_REQUEST_SIZE 8192
#endif

#ifndef MG_MAX_HTTP_SEND_MBUF
#define MG_MAX_HTTP_SEND_MBUF 4096
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 40
#endif

#endif /* CS_PLATFORM == CS_P_UNIX */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_UNIX_H_ */
#ifndef CS_COMMON_PLATFORMS_PLATFORM_ESP_LWIP_H_
#define CS_COMMON_PLATFORMS_PLATFORM_ESP_LWIP_H_
#if CS_PLATFORM == CS_P_ESP_LWIP

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <lwip/err.h>
#include <lwip/ip_addr.h>
#include <lwip/inet.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

#ifndef LWIP_PROVIDE_ERRNO
#include <errno.h>
#endif

#define LWIP_TIMEVAL_PRIVATE 0

#if LWIP_SOCKET
#include <lwip/sockets.h>
#define SOMAXCONN 10
#else
/* We really need the definitions from sockets.h. */
#undef LWIP_SOCKET
#define LWIP_SOCKET 1
#include <lwip/sockets.h>
#undef LWIP_SOCKET
#define LWIP_SOCKET 0
#endif

typedef int sock_t;
#define INVALID_SOCKET (-1)
#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

unsigned long os_random(void);
#define random os_random

#endif /* CS_PLATFORM == CS_P_ESP_LWIP */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_ESP_LWIP_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_
#define CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_
#if CS_PLATFORM == CS_P_CC3200

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef __TI_COMPILER_VERSION__
#include <fcntl.h>
#include <sys/time.h>
#endif

#define MG_SOCKET_SIMPLELINK 1
#define MG_DISABLE_SOCKETPAIR 1
#define MG_DISABLE_SYNC_RESOLVER 1
#define MG_DISABLE_POPEN 1
#define MG_DISABLE_CGI 1
/* Only SPIFFS supports directories, SLFS does not. */
#ifndef CC3200_FS_SPIFFS
#define MG_DISABLE_DAV 1
#define MG_DISABLE_DIRECTORY_LISTING 1
#endif


typedef int sock_t;
#define INVALID_SOCKET (-1)
#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

#define fileno(x) -1

/* Some functions we implement for Mongoose. */

#ifdef __TI_COMPILER_VERSION__
struct SlTimeval_t;
#define timeval SlTimeval_t
int gettimeofday(struct timeval *t, void *tz);
#endif

long int random(void);

/* TI's libc does not have stat & friends, add them. */
#ifdef __TI_COMPILER_VERSION__

#include <file.h>

typedef unsigned int mode_t;
typedef size_t _off_t;
typedef long ssize_t;

struct stat {
  int st_ino;
  mode_t st_mode;
  int st_nlink;
  time_t st_mtime;
  off_t st_size;
};

int _stat(const char *pathname, struct stat *st);
#define stat(a, b) _stat(a, b)

#define __S_IFMT 0170000

#define __S_IFDIR 0040000
#define __S_IFCHR 0020000
#define __S_IFREG 0100000

#define __S_ISTYPE(mode, mask) (((mode) &__S_IFMT) == (mask))

#define S_IFDIR __S_IFDIR
#define S_IFCHR __S_IFCHR
#define S_IFREG __S_IFREG
#define S_ISDIR(mode) __S_ISTYPE((mode), __S_IFDIR)
#define S_ISREG(mode) __S_ISTYPE((mode), __S_IFREG)

/* As of 5.2.7, TI compiler does not support va_copy() yet. */
#define va_copy(apc, ap) ((apc) = (ap))

#endif /* __TI_COMPILER_VERSION__ */

#ifdef CC3200_FS_SPIFFS
#include <common/spiffs/spiffs.h>

typedef struct {
  spiffs_DIR dh;
  struct spiffs_dirent de;
} DIR;

#define d_name name
#define dirent spiffs_dirent

DIR *opendir(const char *dir_name);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);
#endif /* CC3200_FS_SPIFFS */

#ifdef CC3200_FS_SLFS
#define MG_FS_SLFS
#endif

#endif /* CS_PLATFORM == CS_P_CC3200 */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_CC3200_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_PLATFORM_MSP432_H_
#define CS_COMMON_PLATFORMS_PLATFORM_MSP432_H_
#if CS_PLATFORM == CS_P_MSP432

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef __TI_COMPILER_VERSION__
#include <fcntl.h>
#include <sys/time.h>
#endif

#define MG_SOCKET_SIMPLELINK 1
#define MG_DISABLE_SOCKETPAIR 1
#define MG_DISABLE_SYNC_RESOLVER 1
#define MG_DISABLE_POPEN 1
#define MG_DISABLE_CGI 1
#define MG_DISABLE_DAV 1
#define MG_DISABLE_DIRECTORY_LISTING 1


typedef int sock_t;
#define INVALID_SOCKET (-1)
#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

#define fileno(x) -1

/* Some functions we implement for Mongoose. */

#ifdef __TI_COMPILER_VERSION__
struct SlTimeval_t;
#define timeval SlTimeval_t
int gettimeofday(struct timeval *t, void *tz);
#endif

long int random(void);

/* TI's libc does not have stat & friends, add them. */
#ifdef __TI_COMPILER_VERSION__

#include <file.h>

typedef unsigned int mode_t;
typedef size_t _off_t;
typedef long ssize_t;

struct stat {
  int st_ino;
  mode_t st_mode;
  int st_nlink;
  time_t st_mtime;
  off_t st_size;
};

int _stat(const char *pathname, struct stat *st);
#define stat(a, b) _stat(a, b)

#define __S_IFMT 0170000

#define __S_IFDIR 0040000
#define __S_IFCHR 0020000
#define __S_IFREG 0100000

#define __S_ISTYPE(mode, mask) (((mode) &__S_IFMT) == (mask))

#define S_IFDIR __S_IFDIR
#define S_IFCHR __S_IFCHR
#define S_IFREG __S_IFREG
#define S_ISDIR(mode) __S_ISTYPE((mode), __S_IFDIR)
#define S_ISREG(mode) __S_ISTYPE((mode), __S_IFREG)

/* As of 5.2.7, TI compiler does not support va_copy() yet. */
#define va_copy(apc, ap) ((apc) = (ap))

#endif /* __TI_COMPILER_VERSION__ */

#endif /* CS_PLATFORM == CS_P_MSP432 */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_MSP432_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_
#define CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_

/* If simplelink.h is already included, all bets are off. */
#if defined(MG_SOCKET_SIMPLELINK) && !defined(__SIMPLELINK_H__)

#include <stdbool.h>

#ifndef __TI_COMPILER_VERSION__
#undef __CONCAT
#undef FD_CLR
#undef FD_ISSET
#undef FD_SET
#undef FD_SETSIZE
#undef FD_ZERO
#undef fd_set
#endif

/* We want to disable SL_INC_STD_BSD_API_NAMING, so we include user.h ourselves
 * and undef it. */
#define PROVISIONING_API_H_
#include <simplelink/user.h>
#undef PROVISIONING_API_H_
#undef SL_INC_STD_BSD_API_NAMING

#include <simplelink/include/simplelink.h>

/* Now define only the subset of the BSD API that we use.
 * Notably, close(), read() and write() are not defined. */
#define AF_INET SL_AF_INET

#define socklen_t SlSocklen_t
#define sockaddr SlSockAddr_t
#define sockaddr_in SlSockAddrIn_t
#define in_addr SlInAddr_t

#define SOCK_STREAM SL_SOCK_STREAM
#define SOCK_DGRAM SL_SOCK_DGRAM

#define FD_SET SL_FD_SET
#define FD_CLR SL_FD_CLR
#define FD_ISSET SL_FD_ISSET
#define FD_ZERO SL_FD_ZERO
#define fd_set SlFdSet_t

#define htonl sl_Htonl
#define ntohl sl_Ntohl
#define htons sl_Htons
#define ntohs sl_Ntohs

#define accept sl_Accept
#define closesocket sl_Close
#define bind sl_Bind
#define connect sl_Connect
#define listen sl_Listen
#define recv sl_Recv
#define recvfrom sl_RecvFrom
#define send sl_Send
#define sendto sl_SendTo
#define socket sl_Socket

#define select(nfds, rfds, wfds, efds, tout) \
  sl_Select((nfds), (rfds), (wfds), (efds), (struct SlTimeval_t *)(tout))

#ifndef EACCES
#define EACCES SL_EACCES
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT SL_EAFNOSUPPORT
#endif
#ifndef EAGAIN
#define EAGAIN SL_EAGAIN
#endif
#ifndef EBADF
#define EBADF SL_EBADF
#endif
#ifndef EINVAL
#define EINVAL SL_EINVAL
#endif
#ifndef ENOMEM
#define ENOMEM SL_ENOMEM
#endif
#ifndef EWOULDBLOCK
#define EWOULDBLOCK SL_EWOULDBLOCK
#endif

#define SOMAXCONN 8

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
char *inet_ntoa(struct in_addr in);
int inet_pton(int af, const char *src, void *dst);

struct mg_mgr;

typedef void (*mg_init_cb)(struct mg_mgr *mgr);
bool mg_start_task(int priority, int stack_size, mg_init_cb mg_init);

void mg_run_in_task(void (*cb)(struct mg_mgr *mgr, void *arg), void *cb_arg);

int sl_fs_init();

#endif /* defined(MG_SOCKET_SIMPLELINK) && !defined(__SIMPLELINK_H__) */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_CS_SIMPLELINK_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_CS_DBG_H_
#define CS_COMMON_CS_DBG_H_

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

void cs_log_set_level(enum cs_log_level level);

#ifndef CS_DISABLE_STDIO

#include <stdio.h>

void cs_log_set_file(FILE *file);

extern enum cs_log_level cs_log_level;
void cs_log_print_prefix(const char *func);
void cs_log_printf(const char *fmt, ...);

#define LOG(l, x)                  \
  if (cs_log_level >= l) {         \
    cs_log_print_prefix(__func__); \
    cs_log_printf x;               \
  }

#ifndef CS_NDEBUG

#define DBG(x)                            \
  if (cs_log_level >= LL_VERBOSE_DEBUG) { \
    cs_log_print_prefix(__func__);        \
    cs_log_printf x;                      \
  }

#else /* NDEBUG */

#define DBG(x)

#endif

#else /* CS_DISABLE_STDIO */

#define LOG(l, x)
#define DBG(x)

#endif

#endif /* CS_COMMON_CS_DBG_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_CS_TIME_H_
#define CS_COMMON_CS_TIME_H_

/* Sub-second granularity time(). */
double cs_time();

#endif /* CS_COMMON_CS_TIME_H_ */
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === Memory Buffers
 *
 * Mbufs are mutable/growing memory buffers, like C++ strings.
 * Mbuf can append data to the end of a buffer, or insert data into arbitrary
 * position in the middle of a buffer. The buffer grows automatically when
 * needed.
 */

#ifndef CS_COMMON_MBUF_H_
#define CS_COMMON_MBUF_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>

#ifndef MBUF_SIZE_MULTIPLIER
#define MBUF_SIZE_MULTIPLIER 1.5
#endif

/* Memory buffer descriptor */
struct mbuf {
  char *buf;   /* Buffer pointer */
  size_t len;  /* Data length. Data is located between offset 0 and len. */
  size_t size; /* Buffer size allocated by realloc(1). Must be >= len */
};

/*
 * Initialize an Mbuf.
 * `initial_capacity` specifies the initial capacity of the mbuf.
 */
void mbuf_init(struct mbuf *, size_t initial_capacity);

/* Free the space allocated for the mbuffer and resets the mbuf structure. */
void mbuf_free(struct mbuf *);

/*
 * Appends data to the Mbuf.
 *
 * Return the number of bytes appended, or 0 if out of memory.
 */
size_t mbuf_append(struct mbuf *, const void *data, size_t data_size);

/*
 * Insert data at a specified offset in the Mbuf.
 *
 * Existing data will be shifted forwards and the buffer will
 * be grown if necessary.
 * Return the number of bytes inserted.
 */
size_t mbuf_insert(struct mbuf *, size_t, const void *, size_t);

/* Remove `data_size` bytes from the beginning of the buffer. */
void mbuf_remove(struct mbuf *, size_t data_size);

/*
 * Resize an Mbuf.
 *
 * If `new_size` is smaller than buffer's `len`, the
 * resize is not performed.
 */
void mbuf_resize(struct mbuf *, size_t new_size);

/* Shrink an Mbuf by resizing its `size` to `len`. */
void mbuf_trim(struct mbuf *);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CS_COMMON_MBUF_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_SHA1_H_
#define CS_COMMON_SHA1_H_

#ifndef DISABLE_SHA1


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} cs_sha1_ctx;

void cs_sha1_init(cs_sha1_ctx *);
void cs_sha1_update(cs_sha1_ctx *, const unsigned char *data, uint32_t len);
void cs_sha1_final(unsigned char digest[20], cs_sha1_ctx *);
void cs_hmac_sha1(const unsigned char *key, size_t key_len,
                  const unsigned char *text, size_t text_len,
                  unsigned char out[20]);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DISABLE_SHA1 */

#endif /* CS_COMMON_SHA1_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_MD5_H_
#define CS_COMMON_MD5_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct MD5Context {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} MD5_CTX;

void MD5_Init(MD5_CTX *c);
void MD5_Update(MD5_CTX *c, const unsigned char *data, size_t len);
void MD5_Final(unsigned char *md, MD5_CTX *c);

/*
 * Return stringified MD5 hash for NULL terminated list of strings.
 * Example:
 *
 *    char buf[33];
 *    cs_md5(buf, "foo", "bar", NULL);
 */
char *cs_md5(char buf[33], ...);

/*
 * Stringify binary data. Output buffer size must be 2 * size_of_input + 1
 * because each byte of input takes 2 bytes in string representation
 * plus 1 byte for the terminating \0 character.
 */
void cs_to_hex(char *to, const unsigned char *p, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_MD5_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_BASE64_H_
#define CS_COMMON_BASE64_H_

#ifndef DISABLE_BASE64

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*cs_base64_putc_t)(char, void *);

struct cs_base64_ctx {
  /* cannot call it putc because it's a macro on some environments */
  cs_base64_putc_t b64_putc;
  unsigned char chunk[3];
  int chunk_size;
  void *user_data;
};

void cs_base64_init(struct cs_base64_ctx *ctx, cs_base64_putc_t putc,
                    void *user_data);
void cs_base64_update(struct cs_base64_ctx *ctx, const char *str, size_t len);
void cs_base64_finish(struct cs_base64_ctx *ctx);

void cs_base64_encode(const unsigned char *src, int src_len, char *dst);
void cs_fprint_base64(FILE *f, const unsigned char *src, int src_len);
int cs_base64_decode(const unsigned char *s, int len, char *dst);

#ifdef __cplusplus
}
#endif

#endif /* DISABLE_BASE64 */

#endif /* CS_COMMON_BASE64_H_ */
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_STR_UTIL_H_
#define CS_COMMON_STR_UTIL_H_

#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t c_strnlen(const char *s, size_t maxlen);
int c_snprintf(char *buf, size_t buf_size, const char *format, ...);
int c_vsnprintf(char *buf, size_t buf_size, const char *format, va_list ap);
/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
const char *c_strnstr(const char *s, const char *find, size_t slen);

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_STR_UTIL_H_ */
/*
 * Copyright (c) 2004-2013 Sergey Lyubka <valenok@gmail.com>
 * Copyright (c) 2013 Cesanta Software Limited
 * All rights reserved
 *
 * This library is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http: *www.gnu.org/licenses/>.
 *
 * You are free to use this library under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this library under a commercial
 * license, as set out in <http://cesanta.com/products.html>.
 */

#ifndef CS_MONGOOSE_DEPS_FROZEN_FROZEN_H_
#define CS_MONGOOSE_DEPS_FROZEN_FROZEN_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdarg.h>

enum json_type {
  JSON_TYPE_EOF = 0, /* End of parsed tokens marker */
  JSON_TYPE_STRING = 1,
  JSON_TYPE_NUMBER = 2,
  JSON_TYPE_OBJECT = 3,
  JSON_TYPE_TRUE = 4,
  JSON_TYPE_FALSE = 5,
  JSON_TYPE_NULL = 6,
  JSON_TYPE_ARRAY = 7
};

struct json_token {
  const char *ptr;     /* Points to the beginning of the token */
  int len;             /* Token length */
  int num_desc;        /* For arrays and object, total number of descendants */
  enum json_type type; /* Type of the token, possible values above */
};

/* Error codes */
#define JSON_STRING_INVALID -1
#define JSON_STRING_INCOMPLETE -2
#define JSON_TOKEN_ARRAY_TOO_SMALL -3

int parse_json(const char *json_string, int json_string_length,
               struct json_token *tokens_array, int size_of_tokens_array);
struct json_token *parse_json2(const char *json_string, int string_length);
struct json_token *find_json_token(struct json_token *toks, const char *path);

int json_emit_long(char *buf, int buf_len, long value);
int json_emit_double(char *buf, int buf_len, double value);
int json_emit_quoted_str(char *buf, int buf_len, const char *str, int len);
int json_emit_unquoted_str(char *buf, int buf_len, const char *str, int len);
int json_emit(char *buf, int buf_len, const char *fmt, ...);
int json_emit_va(char *buf, int buf_len, const char *fmt, va_list);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_DEPS_FROZEN_FROZEN_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_CS_DIRENT_H_
#define CS_COMMON_CS_DIRENT_H_

#ifdef CS_ENABLE_SPIFFS

#include <spiffs.h>

typedef struct {
  spiffs_DIR dh;
  struct spiffs_dirent de;
} DIR;

#define d_name name
#define dirent spiffs_dirent

int rmdir(const char *path);
int mkdir(const char *path, mode_t mode);

#endif

#if defined(_WIN32) || defined(CS_ENABLE_SPIFFS)
DIR *opendir(const char *dir_name);
int closedir(DIR *dir);
struct dirent *readdir(DIR *dir);
#endif

#endif /* CS_COMMON_CS_DIRENT_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/*
 * === Core: TCP/UDP/SSL
 *
 * NOTE: Mongoose manager is single threaded. It does not protect
 * its data structures by mutexes, therefore all functions that are dealing
 * with particular event manager should be called from the same thread,
 * with exception of `mg_broadcast()` function. It is fine to have different
 * event managers handled by different threads.
 */

#ifndef CS_MONGOOSE_SRC_NET_H_
#define CS_MONGOOSE_SRC_NET_H_

#ifdef MG_ENABLE_JAVASCRIPT
#define EXCLUDE_COMMON
#include <v7.h>
#endif


#ifdef MG_ENABLE_SSL
#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <openssl/ssl.h>
#else
typedef void *SSL;
typedef void *SSL_CTX;
#endif

#ifndef MG_VPRINTF_BUFFER_SIZE
#define MG_VPRINTF_BUFFER_SIZE 100
#endif

#ifdef MG_USE_READ_WRITE
#define MG_RECV_FUNC(s, b, l, f) read(s, b, l)
#define MG_SEND_FUNC(s, b, l, f) write(s, b, l)
#else
#define MG_RECV_FUNC(s, b, l, f) recv(s, b, l, f)
#define MG_SEND_FUNC(s, b, l, f) send(s, b, l, f)
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

union socket_address {
  struct sockaddr sa;
  struct sockaddr_in sin;
#ifdef MG_ENABLE_IPV6
  struct sockaddr_in6 sin6;
#else
  struct sockaddr sin6;
#endif
};

/* Describes chunk of memory */
struct mg_str {
  const char *p; /* Memory chunk pointer */
  size_t len;    /* Memory chunk length */
};

struct mg_connection;

/*
 * Callback function (event handler) prototype, must be defined by user.
 * Mongoose calls event handler, passing events defined below.
 */
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev, void *);

/* Events. Meaning of event parameter (evp) is given in the comment. */
#define MG_EV_POLL 0    /* Sent to each connection on each mg_mgr_poll() call */
#define MG_EV_ACCEPT 1  /* New connection accepted. union socket_address * */
#define MG_EV_CONNECT 2 /* connect() succeeded or failed. int *  */
#define MG_EV_RECV 3    /* Data has benn received. int *num_bytes */
#define MG_EV_SEND 4    /* Data has been written to a socket. int *num_bytes */
#define MG_EV_CLOSE 5   /* Connection is closed. NULL */
#define MG_EV_TIMER 6   /* now >= conn->ev_timer_time. double * */

/*
 * Mongoose event manager.
 */
struct mg_mgr {
  struct mg_connection *active_connections;
  const char *hexdump_file; /* Debug hexdump file path */
#ifndef MG_DISABLE_SOCKETPAIR
  sock_t ctl[2]; /* Socketpair for mg_wakeup() */
#endif
  void *user_data; /* User data */
  void *mgr_data;  /* Implementation-specific event manager's data. */
#ifdef MG_ENABLE_JAVASCRIPT
  struct v7 *v7;
#endif
};

/*
 * Mongoose connection.
 */
struct mg_connection {
  struct mg_connection *next, *prev; /* mg_mgr::active_connections linkage */
  struct mg_connection *listener;    /* Set only for accept()-ed connections */
  struct mg_mgr *mgr;                /* Pointer to containing manager */

  sock_t sock; /* Socket to the remote peer */
  int err;
  union socket_address sa; /* Remote peer address */
  size_t recv_mbuf_limit;  /* Max size of recv buffer */
  struct mbuf recv_mbuf;   /* Received data */
  struct mbuf send_mbuf;   /* Data scheduled for sending */
  SSL *ssl;
  SSL_CTX *ssl_ctx;
  time_t last_io_time;              /* Timestamp of the last socket IO */
  double ev_timer_time;             /* Timestamp of the future MG_EV_TIMER */
  mg_event_handler_t proto_handler; /* Protocol-specific event handler */
  void *proto_data;                 /* Protocol-specific data */
  void (*proto_data_destructor)(void *proto_data);
  mg_event_handler_t handler; /* Event handler function */
  void *user_data;            /* User-specific data */
  union {
    void *v;
    /*
     * the C standard is fussy about fitting function pointers into
     * void pointers, since some archs might have fat pointers for functions.
     */
    mg_event_handler_t f;
  } priv_1;       /* Used by mg_enable_multithreading() */
  void *priv_2;   /* Used by mg_enable_multithreading() */
  void *mgr_data; /* Implementation-specific event manager's data. */
  unsigned long flags;
/* Flags set by Mongoose */
#define MG_F_LISTENING (1 << 0)          /* This connection is listening */
#define MG_F_UDP (1 << 1)                /* This connection is UDP */
#define MG_F_RESOLVING (1 << 2)          /* Waiting for async resolver */
#define MG_F_CONNECTING (1 << 3)         /* connect() call in progress */
#define MG_F_SSL_HANDSHAKE_DONE (1 << 4) /* SSL specific */
#define MG_F_WANT_READ (1 << 5)          /* SSL specific */
#define MG_F_WANT_WRITE (1 << 6)         /* SSL specific */
#define MG_F_IS_WEBSOCKET (1 << 7)       /* Websocket specific */

/* Flags that are settable by user */
#define MG_F_SEND_AND_CLOSE (1 << 10)      /* Push remaining data and close  */
#define MG_F_CLOSE_IMMEDIATELY (1 << 11)   /* Disconnect */
#define MG_F_WEBSOCKET_NO_DEFRAG (1 << 12) /* Websocket specific */
#define MG_F_DELETE_CHUNK (1 << 13)        /* HTTP specific */

#define MG_F_USER_1 (1 << 20) /* Flags left for application */
#define MG_F_USER_2 (1 << 21)
#define MG_F_USER_3 (1 << 22)
#define MG_F_USER_4 (1 << 23)
#define MG_F_USER_5 (1 << 24)
#define MG_F_USER_6 (1 << 25)
};

/*
 * Initialize Mongoose manager. Side effect: ignores SIGPIPE signal.
 * `mgr->user_data` field will be initialized with `user_data` parameter.
 * That is an arbitrary pointer, where user code can associate some data
 * with the particular Mongoose manager. For example, a C++ wrapper class
 * could be written, in which case `user_data` can hold a pointer to the
 * class instance.
 */
void mg_mgr_init(struct mg_mgr *mgr, void *user_data);

/*
 * De-initializes Mongoose manager.
 *
 * Close and deallocate all active connections.
 */
void mg_mgr_free(struct mg_mgr *);

/*
 * This function performs the actual IO, and must be called in a loop
 * (an event loop). Returns the current timestamp.
 * `milli` is the maximum number of milliseconds to sleep.
 * `mg_mgr_poll()` checks all connection for IO readiness. If at least one
 * of the connections is IO-ready, `mg_mgr_poll()` triggers respective
 * event handlers and returns.
 */
time_t mg_mgr_poll(struct mg_mgr *, int milli);

#ifndef MG_DISABLE_SOCKETPAIR
/*
 * Pass a message of a given length to all connections.
 *
 * Must be called from a thread that does NOT call `mg_mgr_poll()`.
 * Note that `mg_broadcast()` is the only function
 * that can be, and must be, called from a different (non-IO) thread.
 *
 * `func` callback function will be called by the IO thread for each
 * connection. When called, event would be `MG_EV_POLL`, and message will
 * be passed as `ev_data` pointer. Maximum message size is capped
 * by `MG_CTL_MSG_MESSAGE_SIZE` which is set to 8192 bytes.
 */
void mg_broadcast(struct mg_mgr *, mg_event_handler_t func, void *, size_t);
#endif

/*
 * Iterate over all active connections.
 *
 * Returns next connection from the list
 * of active connections, or `NULL` if there is no more connections. Below
 * is the iteration idiom:
 *
 * ```c
 * for (c = mg_next(srv, NULL); c != NULL; c = mg_next(srv, c)) {
 *   // Do something with connection `c`
 * }
 * ```
 */
struct mg_connection *mg_next(struct mg_mgr *, struct mg_connection *);

/*
 * Optional parameters to `mg_add_sock_opt()`.
 *
 * `flags` is an initial `struct mg_connection::flags` bitmask to set,
 * see `MG_F_*` flags definitions.
 */
struct mg_add_sock_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Initial connection flags */
  const char **error_string; /* Placeholder for the error string */
};

/*
 * Create a connection, associate it with the given socket and event handler,
 * and add it to the manager.
 *
 * For more options see the `mg_add_sock_opt` variant.
 */
struct mg_connection *mg_add_sock(struct mg_mgr *, sock_t, mg_event_handler_t);

/*
 * Create a connection, associate it with the given socket and event handler,
 * and add to the manager.
 *
 * See the `mg_add_sock_opts` structure for a description of the options.
 */
struct mg_connection *mg_add_sock_opt(struct mg_mgr *, sock_t,
                                      mg_event_handler_t,
                                      struct mg_add_sock_opts);

/*
 * Optional parameters to `mg_bind_opt()`.
 *
 * `flags` is an initial `struct mg_connection::flags` bitmask to set,
 * see `MG_F_*` flags definitions.
 */
struct mg_bind_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Extra connection flags */
  const char **error_string; /* Placeholder for the error string */
#ifdef MG_ENABLE_SSL
  /* SSL settings. */
  const char *ssl_cert;    /* Server certificate to present to clients */
  const char *ssl_ca_cert; /* Verify client certificates with this CA bundle */
#endif
};

/*
 * Create listening connection.
 *
 * See `mg_bind_opt` for full documentation.
 */
struct mg_connection *mg_bind(struct mg_mgr *, const char *,
                              mg_event_handler_t);
/*
 * Create listening connection.
 *
 * `address` parameter tells which address to bind to. It's format is the same
 * as for the `mg_connect()` call, where `HOST` part is optional. `address`
 * can be just a port number, e.g. `:8000`. To bind to a specific interface,
 * an IP address can be specified, e.g. `1.2.3.4:8000`. By default, a TCP
 * connection is created. To create UDP connection, prepend `udp://` prefix,
 * e.g. `udp://:8000`. To summarize, `address` paramer has following format:
 * `[PROTO://][IP_ADDRESS]:PORT`, where `PROTO` could be `tcp` or `udp`.
 *
 * See the `mg_bind_opts` structure for a description of the optional
 * parameters.
 *
 * Return a new listening connection, or `NULL` on error.
 * NOTE: Connection remains owned by the manager, do not free().
 */
struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                  mg_event_handler_t handler,
                                  struct mg_bind_opts opts);

/* Optional parameters to `mg_connect_opt()` */
struct mg_connect_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Extra connection flags */
  const char **error_string; /* Placeholder for the error string */
#ifdef MG_ENABLE_SSL
  /* SSL settings. */
  const char *ssl_cert;    /* Client certificate to present to the server */
  const char *ssl_ca_cert; /* Verify server certificate using this CA bundle */

  /*
   * Server name verification. If ssl_ca_cert is set and the certificate has
   * passed verification, its subject will be verified against this string.
   * By default (if ssl_server_name is NULL) hostname part of the address will
   * be used. Wildcard matching is supported. A special value of "*" disables
   * name verification.
   */
  const char *ssl_server_name;
#endif
};

/*
 * Connect to a remote host.
 *
 * See `mg_connect_opt()` for full documentation.
 */
struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                 mg_event_handler_t handler);

/*
 * Connect to a remote host.
 *
 * `address` format is `[PROTO://]HOST:PORT`. `PROTO` could be `tcp` or `udp`.
 * `HOST` could be an IP address,
 * IPv6 address (if Mongoose is compiled with `-DMG_ENABLE_IPV6`), or a host
 * name. If `HOST` is a name, Mongoose will resolve it asynchronously. Examples
 * of valid addresses: `google.com:80`, `udp://1.2.3.4:53`, `10.0.0.1:443`,
 * `[::1]:80`
 *
 * See the `mg_connect_opts` structure for a description of the optional
 * parameters.
 *
 * Returns a new outbound connection, or `NULL` on error.
 *
 * NOTE: Connection remains owned by the manager, do not free().
 *
 * NOTE: To enable IPv6 addresses, `-DMG_ENABLE_IPV6` should be specified
 * in the compilation flags.
 *
 * NOTE: New connection will receive `MG_EV_CONNECT` as it's first event
 * which will report connect success status.
 * If asynchronous resolution fail, or `connect()` syscall fail for whatever
 * reason (e.g. with `ECONNREFUSED` or `ENETUNREACH`), then `MG_EV_CONNECT`
 * event report failure. Code example below:
 *
 * ```c
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   int connect_status;
 *
 *   switch (ev) {
 *     case MG_EV_CONNECT:
 *       connect_status = * (int *) ev_data;
 *       if (connect_status == 0) {
 *         // Success
 *       } else  {
 *         // Error
 *         printf("connect() error: %s\n", strerror(connect_status));
 *       }
 *       break;
 *     ...
 *   }
 * }
 *
 *   ...
 *   mg_connect(mgr, "my_site.com:80", ev_handler);
 * ```
 */
struct mg_connection *mg_connect_opt(struct mg_mgr *mgr, const char *address,
                                     mg_event_handler_t handler,
                                     struct mg_connect_opts opts);

/*
 * Enable SSL for a given connection.
 * `cert` is a server certificate file name for a listening connection,
 * or a client certificate file name for an outgoing connection.
 * Certificate files must be in PEM format. Server certificate file
 * must contain a certificate, concatenated with a private key, optionally
 * concatenated with parameters.
 * `ca_cert` is a CA certificate, or NULL if peer verification is not
 * required.
 * Return: NULL on success, or error message on error.
 */
const char *mg_set_ssl(struct mg_connection *nc, const char *cert,
                       const char *ca_cert);

/*
 * Send data to the connection.
 *
 * Note that sending functions do not actually push data to the socket.
 * They just append data to the output buffer. MG_EV_SEND will be delivered when
 * the data has actually been pushed out.
 */
void mg_send(struct mg_connection *, const void *buf, int len);

/* Enables format string warnings for mg_printf */
#if defined(__GNUC__)
__attribute__((format(printf, 2, 3)))
#endif
/* don't separate from mg_printf declaration */

/*
 * Send `printf`-style formatted data to the connection.
 *
 * See `mg_send` for more details on send semantics.
 */
int mg_printf(struct mg_connection *, const char *fmt, ...);

/* Same as `mg_printf()`, but takes `va_list ap` as an argument. */
int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);

/*
 * Create a socket pair.
 * `sock_type` can be either `SOCK_STREAM` or `SOCK_DGRAM`.
 * Return 0 on failure, 1 on success.
 */
int mg_socketpair(sock_t[2], int sock_type);

/*
 * Convert domain name into IP address.
 *
 * This is a utility function. If compilation flags have
 * `-DMG_ENABLE_GETADDRINFO`, then `getaddrinfo()` call is used for name
 * resolution. Otherwise, `gethostbyname()` is used.
 *
 * CAUTION: this function can block.
 * Return 1 on success, 0 on failure.
 */
#ifndef MG_DISABLE_SYNC_RESOLVER
int mg_resolve(const char *domain_name, char *ip_addr_buf, size_t buf_len);
#endif

/*
 * Verify given IP address against the ACL.
 *
 * `remote_ip` - an IPv4 address to check, in host byte order
 * `acl` - a comma separated list of IP subnets: `x.x.x.x/x` or `x.x.x.x`.
 * Each subnet is
 * prepended by either a - or a + sign. A plus sign means allow, where a
 * minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
 * this means to deny only that single IP address.
 * Subnet masks may vary from 0 to 32, inclusive. The default setting
 * is to allow all accesses. On each request the full list is traversed,
 * and the last match wins. Example:
 *
 * `-0.0.0.0/0,+192.168/16` - deny all acccesses, only allow 192.168/16 subnet
 *
 * To learn more about subnet masks, see the
 * link:https://en.wikipedia.org/wiki/Subnetwork[Wikipedia page on Subnetwork]
 *
 * Return -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
 */
int mg_check_ip_acl(const char *acl, uint32_t remote_ip);

/*
 * Enable multi-threaded handling for the given listening connection `nc`.
 * For each accepted connection, Mongoose will create a separate thread
 * and run event handler in that thread. Thus, if an event hanler is doing
 * a blocking call or some long computation, that will not slow down
 * other connections.
 */
void mg_enable_multithreading(struct mg_connection *nc);

#ifdef MG_ENABLE_JAVASCRIPT
/*
 * Enable server-side JavaScript scripting.
 * Requires `-DMG_ENABLE_JAVASCRIPT` compilation flag, and V7 engine sources.
 * v7 instance must not be destroyed during manager's lifetime.
 * Return V7 error.
 */
enum v7_err mg_enable_javascript(struct mg_mgr *m, struct v7 *v7,
                                 const char *init_js_file_name);
#endif

/*
 * Schedule MG_EV_TIMER event to be delivered at `timestamp` time.
 * `timestamp` is a UNIX time (a number of seconds since Epoch). It is
 * `double` instead of `time_t` to allow for sub-second precision.
 * Return the old timer value.
 *
 * Example: set connect timeout to 1.5 seconds:
 *
 * ```
 *  c = mg_connect(&mgr, "cesanta.com", ev_handler);
 *  mg_set_timer(c, mg_time() + 1.5);
 *  ...
 *
 *  void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
 *  switch (ev) {
 *    case MG_EV_CONNECT:
 *      mg_set_timer(c, 0);  // Clear connect timer
 *      break;
 *    case MG_EV_TIMER:
 *      log("Connect timeout");
 *      c->flags |= MG_F_CLOSE_IMMEDIATELY;
 *      break;
 * ```
 */
double mg_set_timer(struct mg_connection *c, double timestamp);

/*
 * A sub-second precision version of time().
 */
double mg_time();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_NET_H_ */
/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_NET_IF_H_
#define CS_MONGOOSE_SRC_NET_IF_H_

/*
 * Internal async networking core interface.
 * Consists of calls made by the core, which should not block,
 * and callbacks back into the core ("..._cb").
 * Callbacks may (will) cause methods to be invoked from within,
 * but methods are not allowed to invoke callbacks inline.
 *
 * Implementation must ensure that only one callback is invoked at any time.
 */

/* Request that a TCP connection is made to the specified address. */
void mg_if_connect_tcp(struct mg_connection *nc,
                       const union socket_address *sa);
/* Open a UDP socket. Doesn't actually connect anything. */
void mg_if_connect_udp(struct mg_connection *nc);
/* Callback invoked by connect methods. err = 0 -> ok, != 0 -> error. */
void mg_if_connect_cb(struct mg_connection *nc, int err);

/* Set up a listening TCP socket on a given address. rv = 0 -> ok. */
int mg_if_listen_tcp(struct mg_connection *nc, union socket_address *sa);

/*
 * Deliver a new TCP connection. Returns NULL in case on error (unable to
 * create connection, in which case interface state should be discarded.
 * This is phase 1 of the two-phase process - MG_EV_ACCEPT will be delivered
 * when mg_if_accept_tcp_cb is invoked.
 */
struct mg_connection *mg_if_accept_new_conn(struct mg_connection *lc);
void mg_if_accept_tcp_cb(struct mg_connection *nc, union socket_address *sa,
                         size_t sa_len);

/* Request that a "listening" UDP socket be created. */
int mg_if_listen_udp(struct mg_connection *nc, union socket_address *sa);

/* Send functions for TCP and UDP. Sent data is copied before return. */
void mg_if_tcp_send(struct mg_connection *nc, const void *buf, size_t len);
void mg_if_udp_send(struct mg_connection *nc, const void *buf, size_t len);
/* Callback that reports that data has been put on the wire. */
void mg_if_sent_cb(struct mg_connection *nc, int num_sent);

/*
 * Receive callback.
 * buf must be heap-allocated and ownership is transferred to the core.
 * Core will acknowledge consumption by calling mg_if_recved.
 */
void mg_if_recv_tcp_cb(struct mg_connection *nc, void *buf, int len);
void mg_if_recv_udp_cb(struct mg_connection *nc, void *buf, int len,
                       union socket_address *sa, size_t sa_len);
void mg_if_recved(struct mg_connection *nc, size_t len);

/* Deliver a POLL event to the connection. */
void mg_if_poll(struct mg_connection *nc, time_t now);

/* Deliver a TIMER event to the connection. */
void mg_if_timer(struct mg_connection *c, double now);

/* Perform interface-related connection initialization. Return 1 on success. */
int mg_if_create_conn(struct mg_connection *nc);

/* Perform interface-related cleanup on connection before destruction. */
void mg_if_destroy_conn(struct mg_connection *nc);

void mg_close_conn(struct mg_connection *nc);

/* Put connection's address into *sa, local (remote = 0) or remote. */
void mg_if_get_conn_addr(struct mg_connection *nc, int remote,
                         union socket_address *sa);

/* Associate a socket to a connection. */
void mg_sock_set(struct mg_connection *nc, sock_t sock);

#endif /* CS_MONGOOSE_SRC_NET_IF_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === URI
 */

#ifndef CS_MONGOOSE_SRC_URI_H_
#define CS_MONGOOSE_SRC_URI_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Parses an URI and fills string chunks with locations of the respective
 * uri components within the input uri string. NULL pointers will be
 * ignored.
 *
 * General syntax:
 *
 *     [scheme://[user_info@]]host[:port][/path][?query][#fragment]
 *
 * Example:
 *
 *     foo.com:80
 *     tcp://foo.com:1234
 *     http://foo.com:80/bar?baz=1
 *     https://user:pw@foo.com:443/blah
 *
 * `path` will include the leading slash. `query` won't include the leading `?`.
 * `host` can contain embedded colons if surrounded by square brackets in order
 * to support IPv6 literal addresses.
 *
 *
 * Returns 0 on success, -1 on error.
 */
int mg_parse_uri(struct mg_str uri, struct mg_str *scheme,
                 struct mg_str *user_info, struct mg_str *host,
                 unsigned int *port, struct mg_str *path, struct mg_str *query,
                 struct mg_str *fragment);

int mg_normalize_uri_path(const struct mg_str *in, struct mg_str *out);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_URI_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === Utilities
 */

#ifndef CS_MONGOOSE_SRC_UTIL_H_
#define CS_MONGOOSE_SRC_UTIL_H_

#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MAX_PATH_SIZE
#define MAX_PATH_SIZE 500
#endif

/*
 * Fetch substring from input string `s`, `end` into `v`.
 * Skips initial delimiter characters. Records first non-delimiter character
 * as the beginning of substring `v`. Then scans the rest of the string
 * until a delimiter character or end-of-string is found.
 * `delimiters` is a 0-terminated string containing delimiter characters.
 * Either one of `delimiters` or `end_string` terminates the search.
 * Return an `s` pointer, advanced forward where parsing stopped.
 */
const char *mg_skip(const char *s, const char *end_string,
                    const char *delimiters, struct mg_str *v);

/*
 * Cross-platform version of `strncasecmp()`.
 */
int mg_ncasecmp(const char *s1, const char *s2, size_t len);

/*
 * Cross-platform version of `strcasecmp()`.
 */
int mg_casecmp(const char *s1, const char *s2);

/*
 * Cross-platform version of `strcmp()` where where first string is
 * specified by `struct mg_str`.
 */
int mg_vcmp(const struct mg_str *str2, const char *str1);

/*
 * Cross-platform version of `strncasecmp()` where first string is
 * specified by `struct mg_str`.
 */
int mg_vcasecmp(const struct mg_str *str2, const char *str1);

/*
 * Decode base64-encoded string `s`, `len` into the destination `dst`.
 * Destination has to have enough space to hold decoded buffer.
 * Decoding stops either when all string has been decoded, or invalid
 * character appeared.
 * Destination is '\0'-terminated.
 * Return number of decoded characters. On success, that should be equal to
 * `len`. On error (invalid character) the return value is smaller then `len`.
 */
int mg_base64_decode(const unsigned char *s, int len, char *dst);

/*
 * Base64-encode chunk of memory `src`, `src_len` into the destination `dst`.
 * Destination has to have enough space to hold encoded buffer.
 * Destination is '\0'-terminated.
 */
void mg_base64_encode(const unsigned char *src, int src_len, char *dst);

#ifndef MG_DISABLE_FILESYSTEM
/*
 * Perform a 64-bit `stat()` call against given file.
 *
 * `path` should be UTF8 encoded.
 *
 * Return value is the same as for `stat()` syscall.
 */
int mg_stat(const char *path, cs_stat_t *st);

/*
 * Open the given file and return a file stream.
 *
 * `path` and `mode` should be UTF8 encoded.
 *
 * Return value is the same as for the `fopen()` call.
 */
FILE *mg_fopen(const char *path, const char *mode);

/*
 * Open the given file and return a file stream.
 *
 * `path` should be UTF8 encoded.
 *
 * Return value is the same as for the `open()` syscall.
 */
int mg_open(const char *path, int flag, int mode);
#endif /* MG_DISABLE_FILESYSTEM */

#if defined(_WIN32) && !defined(MG_ENABLE_THREADS)
#define MG_ENABLE_THREADS
#endif

#ifdef MG_ENABLE_THREADS
/*
 * Start a new detached thread.
 * Arguments and semantic is the same as pthead's `pthread_create()`.
 * `thread_func` is a thread function, `thread_func_param` is a parameter
 * that is passed to the thread function.
 */
void *mg_start_thread(void *(*thread_func)(void *), void *thread_func_param);
#endif

void mg_set_close_on_exec(sock_t);

#define MG_SOCK_STRINGIFY_IP 1
#define MG_SOCK_STRINGIFY_PORT 2
#define MG_SOCK_STRINGIFY_REMOTE 4
/*
 * Convert connection's local or remote address into string.
 *
 * The `flags` parameter is a bit mask that controls the behavior,
 * see `MG_SOCK_STRINGIFY_*` definitions.
 *
 * - MG_SOCK_STRINGIFY_IP - print IP address
 * - MG_SOCK_STRINGIFY_PORT - print port number
 * - MG_SOCK_STRINGIFY_REMOTE - print remote peer's IP/port, not local address
 *
 * If both port number and IP address are printed, they are separated by `:`.
 * If compiled with `-DMG_ENABLE_IPV6`, IPv6 addresses are supported.
 */
void mg_conn_addr_to_str(struct mg_connection *nc, char *buf, size_t len,
                         int flags);
#ifndef MG_DISABLE_SOCKET_IF /* Legacy interface. */
void mg_sock_to_str(sock_t sock, char *buf, size_t len, int flags);
#endif

/*
 * Convert socket's address into string.
 *
 * `flags` is MG_SOCK_STRINGIFY_IP and/or MG_SOCK_STRINGIFY_PORT.
 */
void mg_sock_addr_to_str(const union socket_address *sa, char *buf, size_t len,
                         int flags);

/*
 * Generates human-readable hexdump of memory chunk.
 *
 * Takes a memory buffer `buf` of length `len` and creates a hex dump of that
 * buffer in `dst`. Generated output is a-la hexdump(1).
 * Return length of generated string, excluding terminating `\0`. If returned
 * length is bigger than `dst_len`, overflow bytes are discarded.
 */
int mg_hexdump(const void *buf, int len, char *dst, int dst_len);

/*
 * Generates human-readable hexdump of the data sent or received by connection.
 * `path` is a file name where hexdump should be written. `num_bytes` is
 * a number of bytes sent/received. `ev` is one of the `MG_*` events sent to
 * an event handler. This function is supposed to be called from the
 * event handler.
 */
void mg_hexdump_connection(struct mg_connection *nc, const char *path,
                           const void *buf, int num_bytes, int ev);
/*
 * Print message to buffer. If buffer is large enough to hold the message,
 * return buffer. If buffer is to small, allocate large enough buffer on heap,
 * and return allocated buffer.
 * This is a supposed use case:
 *
 *    char buf[5], *p = buf;
 *    p = mg_avprintf(&p, sizeof(buf), "%s", "hi there");
 *    use_p_somehow(p);
 *    if (p != buf) {
 *      free(p);
 *    }
 *
 * The purpose of this is to avoid malloc-ing if generated strings are small.
 */
int mg_avprintf(char **buf, size_t size, const char *fmt, va_list ap);

/*
 * Return true if target platform is big endian.
 */
int mg_is_big_endian(void);

/*
 * A helper function for traversing a comma separated list of values.
 * It returns a list pointer shifted to the next value, or NULL if the end
 * of the list found.
 * Value is stored in val vector. If value has form "x=y", then eq_val
 * vector is initialized to point to the "y" part, and val vector length
 * is adjusted to point only to "x".
 * If list is just a comma separated list of entries, like "aa,bb,cc" then
 * `eq_val` will contain zero-length string.
 *
 * The purpose of this function is to parse comma separated string without
 * any copying/memory allocation.
 */
const char *mg_next_comma_list_entry(const char *list, struct mg_str *val,
                                     struct mg_str *eq_val);

/*
 * Match 0-terminated string (mg_match_prefix) or string with given length
 * mg_match_prefix_n against a glob pattern.
 * Match is case-insensitive. Return number of bytes matched, or -1 if no match.
 */
int mg_match_prefix(const char *pattern, int pattern_len, const char *str);
int mg_match_prefix_n(const struct mg_str pattern, const struct mg_str str);

/*
 * A helper function for creating mg_str struct from plain C string.
 * `NULL` is allowed and becomes `{NULL, 0}`.
 */
struct mg_str mg_mk_str(const char *s);

/* Macro for initializing mg_str. */
#define MG_MK_STR(str_literal) \
  { str_literal, sizeof(str_literal) - 1 }

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_UTIL_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === HTTP + Websocket
 */

#ifndef CS_MONGOOSE_SRC_HTTP_H_
#define CS_MONGOOSE_SRC_HTTP_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 20
#endif

#ifndef MG_MAX_HTTP_REQUEST_SIZE
#define MG_MAX_HTTP_REQUEST_SIZE 1024
#endif

#ifndef MG_MAX_PATH
#ifdef PATH_MAX
#define MG_MAX_PATH PATH_MAX
#else
#define MG_MAX_PATH 256
#endif
#endif

#ifndef MG_MAX_HTTP_SEND_MBUF
#define MG_MAX_HTTP_SEND_MBUF 1024
#endif

#ifndef MG_WEBSOCKET_PING_INTERVAL_SECONDS
#define MG_WEBSOCKET_PING_INTERVAL_SECONDS 5
#endif

#ifndef MG_CGI_ENVIRONMENT_SIZE
#define MG_CGI_ENVIRONMENT_SIZE 8192
#endif

#ifndef MG_MAX_CGI_ENVIR_VARS
#define MG_MAX_CGI_ENVIR_VARS 64
#endif

#ifndef MG_ENV_EXPORT_TO_CGI
#define MG_ENV_EXPORT_TO_CGI "MONGOOSE_CGI"
#endif

/* HTTP message */
struct http_message {
  struct mg_str message; /* Whole message: request line + headers + body */

  /* HTTP Request line (or HTTP response line) */
  struct mg_str method; /* "GET" */
  struct mg_str uri;    /* "/my_file.html" */
  struct mg_str proto;  /* "HTTP/1.1" -- for both request and response */

  /* For responses, code and response status message are set */
  int resp_code;
  struct mg_str resp_status_msg;

  /*
   * Query-string part of the URI. For example, for HTTP request
   *    GET /foo/bar?param1=val1&param2=val2
   *    |    uri    |     query_string     |
   *
   * Note that question mark character doesn't belong neither to the uri,
   * nor to the query_string
   */
  struct mg_str query_string;

  /* Headers */
  struct mg_str header_names[MG_MAX_HTTP_HEADERS];
  struct mg_str header_values[MG_MAX_HTTP_HEADERS];

  /* Message body */
  struct mg_str body; /* Zero-length for requests with no body */
};

/* WebSocket message */
struct websocket_message {
  unsigned char *data;
  size_t size;
  unsigned char flags;
};

/* HTTP multipart part */
struct mg_http_multipart_part {
  const char *file_name;
  const char *var_name;
  struct mg_str data;
  int status; /* <0 on error */
  void *user_data;
};

/* HTTP and websocket events. void *ev_data is described in a comment. */
#define MG_EV_HTTP_REQUEST 100 /* struct http_message * */
#define MG_EV_HTTP_REPLY 101   /* struct http_message * */
#define MG_EV_HTTP_CHUNK 102   /* struct http_message * */
#define MG_EV_SSI_CALL 105     /* char * */

#define MG_EV_WEBSOCKET_HANDSHAKE_REQUEST 111 /* NULL */
#define MG_EV_WEBSOCKET_HANDSHAKE_DONE 112    /* NULL */
#define MG_EV_WEBSOCKET_FRAME 113             /* struct websocket_message * */
#define MG_EV_WEBSOCKET_CONTROL_FRAME 114     /* struct websocket_message * */

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART
#define MG_EV_HTTP_MULTIPART_REQUEST 121 /* struct http_message */
#define MG_EV_HTTP_PART_BEGIN 122        /* struct mg_http_multipart_part */
#define MG_EV_HTTP_PART_DATA 123         /* struct mg_http_multipart_part */
#define MG_EV_HTTP_PART_END 124          /* struct mg_http_multipart_part */
#endif

/*
 * Attach built-in HTTP event handler to the given connection.
 * User-defined event handler will receive following extra events:
 *
 * - MG_EV_HTTP_REQUEST: HTTP request has arrived. Parsed HTTP request
 *  is passed as
 *   `struct http_message` through the handler's `void *ev_data` pointer.
 * - MG_EV_HTTP_MULTIPART_REQUEST: A multipart POST request has received.
 *   This event is sent before body is parsed. After this user
 *   should expect a sequence of MG_EV_HTTP_PART_BEGIN/DATA/END requests.
 *   This is also the last time when headers and other request fields are
 *   accessible.
 * - MG_EV_HTTP_REPLY: HTTP reply has arrived. Parsed HTTP reply is passed as
 *   `struct http_message` through the handler's `void *ev_data` pointer.
 * - MG_EV_HTTP_CHUNK: HTTP chunked-encoding chunk has arrived.
 *   Parsed HTTP reply is passed as `struct http_message` through the
 *   handler's `void *ev_data` pointer. `http_message::body` would contain
 *   incomplete, reassembled HTTP body.
 *   It will grow with every new chunk arrived, and
 *   potentially can consume a lot of memory. An event handler may process
 *   the body as chunks are coming, and signal Mongoose to delete processed
 *   body by setting `MG_F_DELETE_CHUNK` in `mg_connection::flags`. When
 *   the last zero chunk is received,
 *   Mongoose sends `MG_EV_HTTP_REPLY` event with
 *   full reassembled body (if handler did not signal to delete chunks) or
 *   with empty body (if handler did signal to delete chunks).
 * - MG_EV_WEBSOCKET_HANDSHAKE_REQUEST: server has received websocket handshake
 *   request. `ev_data` contains parsed HTTP request.
 * - MG_EV_WEBSOCKET_HANDSHAKE_DONE: server has completed Websocket handshake.
 *   `ev_data` is `NULL`.
 * - MG_EV_WEBSOCKET_FRAME: new websocket frame has arrived. `ev_data` is
 *   `struct websocket_message *`
 * - MG_EV_HTTP_PART_BEGIN: new part of multipart message is started,
 *   extra parameters are passed in mg_http_multipart_part
 * - MG_EV_HTTP_PART_DATA: new portion of data from multiparted message
 *   no additional headers are available, only data and data size
 * - MG_EV_HTTP_PART_END: final boundary received, analogue to maybe used to
 *   find the end of packet
 *   Note: Mongoose should be compiled with MG_ENABLE_HTTP_STREAMING_MULTIPART
 *   to enable MG_EV_HTTP_MULTIPART_REQUEST, MG_EV_HTTP_REQUEST_END,
 *   MG_EV_HTTP_REQUEST_CANCEL, MG_EV_HTTP_PART_BEGIN, MG_EV_HTTP_PART_DATA,
 *   MG_EV_HTTP_PART_END constants
 */
void mg_set_protocol_http_websocket(struct mg_connection *nc);

/*
 * Send websocket handshake to the server.
 *
 * `nc` must be a valid connection, connected to a server. `uri` is an URI
 * to fetch, extra_headers` is extra HTTP headers to send or `NULL`.
 *
 * This function is intended to be used by websocket client.
 *
 * Note that the Host header is mandatory in HTTP/1.1 and must be
 * included in `extra_headers`. `mg_send_websocket_handshake2` offers
 * a better API for that.
 *
 * Deprecated in favour of `mg_send_websocket_handshake2`
 */
void mg_send_websocket_handshake(struct mg_connection *nc, const char *uri,
                                 const char *extra_headers);

/*
 * Send websocket handshake to the server.
 *
 * `nc` must be a valid connection, connected to a server. `uri` is an URI
 * to fetch, `host` goes into the `Host` header, `protocol` goes into the
 * `Sec-WebSocket-Proto` header (NULL to omit), extra_headers` is extra HTTP
 * headers to send or `NULL`.
 *
 * This function is intended to be used by websocket client.
 */
void mg_send_websocket_handshake2(struct mg_connection *nc, const char *path,
                                  const char *host, const char *protocol,
                                  const char *extra_headers);

/*
 * Helper function that creates an outbound WebSocket connection.
 *
 * `url` is a URL to connect to. It must be properly URL-encoded, e.g. have
 * no spaces, etc. By default, `mg_connect_ws()` sends Connection and
 * Host headers. `extra_headers` is an extra HTTP headers to send, e.g.
 * `"User-Agent: my-app\r\n"`.
 * If `protocol` is not NULL, then a `Sec-WebSocket-Protocol` header is sent.
 *
 * Examples:
 *
 * ```c
 *   nc1 = mg_connect_ws(mgr, ev_handler_1, "ws://echo.websocket.org", NULL,
 *                       NULL);
 *   nc2 = mg_connect_ws(mgr, ev_handler_1, "wss://echo.websocket.org", NULL,
 *                       NULL);
 *   nc3 = mg_connect_ws(mgr, ev_handler_1, "ws://api.cesanta.com",
 *                       "clubby.cesanta.com", NULL);
 * ```
 */
struct mg_connection *mg_connect_ws(struct mg_mgr *mgr,
                                    mg_event_handler_t event_handler,
                                    const char *url, const char *protocol,
                                    const char *extra_headers);

/*
 * Helper function that creates an outbound WebSocket connection
 *
 * Mostly identical to mg_connect_ws, but allows to provide extra parameters
 * (for example, SSL parameters
 */
struct mg_connection *mg_connect_ws_opt(struct mg_mgr *mgr,
                                        mg_event_handler_t ev_handler,
                                        struct mg_connect_opts opts,
                                        const char *url, const char *protocol,
                                        const char *extra_headers);

/*
 * Send websocket frame to the remote end.
 *
 * `op_and_flags` specifies frame's type, one of:
 *
 * - WEBSOCKET_OP_CONTINUE
 * - WEBSOCKET_OP_TEXT
 * - WEBSOCKET_OP_BINARY
 * - WEBSOCKET_OP_CLOSE
 * - WEBSOCKET_OP_PING
 * - WEBSOCKET_OP_PONG
 *
 * Orred with one of the flags:
 *
 * - WEBSOCKET_DONT_FIN: Don't set the FIN flag on the frame to be sent.
 *
 * `data` and `data_len` contain frame data.
 */
void mg_send_websocket_frame(struct mg_connection *nc, int op_and_flags,
                             const void *data, size_t data_len);

/*
 * Send multiple websocket frames.
 *
 * Like `mg_send_websocket_frame()`, but composes a frame from multiple buffers.
 */
void mg_send_websocket_framev(struct mg_connection *nc, int op_and_flags,
                              const struct mg_str *strings, int num_strings);

/*
 * Send websocket frame to the remote end.
 *
 * Like `mg_send_websocket_frame()`, but allows to create formatted message
 * with `printf()`-like semantics.
 */
void mg_printf_websocket_frame(struct mg_connection *nc, int op_and_flags,
                               const char *fmt, ...);

/*
 * Send buffer `buf` of size `len` to the client using chunked HTTP encoding.
 * This function first sends buffer size as hex number + newline, then
 * buffer itself, then newline. For example,
 *   `mg_send_http_chunk(nc, "foo", 3)` whill append `3\r\nfoo\r\n` string to
 * the `nc->send_mbuf` output IO buffer.
 *
 * NOTE: HTTP header "Transfer-Encoding: chunked" should be sent prior to
 * using this function.
 *
 * NOTE: do not forget to send empty chunk at the end of the response,
 * to tell the client that everything was sent. Example:
 *
 * ```
 *   mg_printf_http_chunk(nc, "%s", "my response!");
 *   mg_send_http_chunk(nc, "", 0); // Tell the client we're finished
 * ```
 */
void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len);

/*
 * Send printf-formatted HTTP chunk.
 * Functionality is similar to `mg_send_http_chunk()`.
 */
void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...);

/*
 * Send response status line.
 * If `extra_headers` is not NULL, then `extra_headers` are also sent
 * after the reponse line. `extra_headers` must NOT end end with new line.
 * Example:
 *
 *      mg_send_response_line(nc, 200, "Access-Control-Allow-Origin: *");
 *
 * Will result in:
 *
 *      HTTP/1.1 200 OK\r\n
 *      Access-Control-Allow-Origin: *\r\n
 */
void mg_send_response_line(struct mg_connection *c, int status_code,
                           const char *extra_headers);

/*
 * Send response line and headers.
 * This function sends response line with the `status_code`, and automatically
 * sends one header: either "Content-Length", or "Transfer-Encoding".
 * If `content_length` is negative, then "Transfer-Encoding: chunked" header
 * is sent, otherwise, "Content-Length" header is sent.
 *
 * NOTE: If `Transfer-Encoding` is `chunked`, then message body must be sent
 * using `mg_send_http_chunk()` or `mg_printf_http_chunk()` functions.
 * Otherwise, `mg_send()` or `mg_printf()` must be used.
 * Extra headers could be set through `extra_headers` - and note `extra_headers`
 * must NOT be terminated by a new line.
 */
void mg_send_head(struct mg_connection *n, int status_code,
                  int64_t content_length, const char *extra_headers);

/*
 * Send printf-formatted HTTP chunk, escaping HTML tags.
 */
void mg_printf_html_escape(struct mg_connection *nc, const char *fmt, ...);

/* Websocket opcodes, from http://tools.ietf.org/html/rfc6455 */
#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10

/*
 * If set causes the FIN flag to not be set on outbound
 * frames. This enables sending multiple fragments of a single
 * logical message.
 *
 * The WebSocket protocol mandates that if the FIN flag of a data
 * frame is not set, the next frame must be a WEBSOCKET_OP_CONTINUE.
 * The last frame must have the FIN bit set.
 *
 * Note that mongoose will automatically defragment incoming messages,
 * so this flag is used only on outbound messages.
 */
#define WEBSOCKET_DONT_FIN 0x100

/*
 * Parse a HTTP message.
 *
 * `is_req` should be set to 1 if parsing request, 0 if reply.
 *
 * Return number of bytes parsed. If HTTP message is
 * incomplete, `0` is returned. On parse error, negative number is returned.
 */
int mg_parse_http(const char *s, int n, struct http_message *hm, int is_req);

/*
 * Search and return header `name` in parsed HTTP message `hm`.
 * If header is not found, NULL is returned. Example:
 *
 *     struct mg_str *host_hdr = mg_get_http_header(hm, "Host");
 */
struct mg_str *mg_get_http_header(struct http_message *hm, const char *name);

/*
 * Parse HTTP header `hdr`. Find variable `var_name` and store it's value
 * in the buffer `buf`, `buf_size`. Return 0 if variable not found, non-zero
 * otherwise.
 *
 * This function is supposed to parse
 * cookies, authentication headers, etcetera. Example (error handling omitted):
 *
 *     char user[20];
 *     struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
 *     mg_http_parse_header(hdr, "username", user, sizeof(user));
 *
 * Return length of the variable's value. If buffer is not large enough,
 * or variable not found, 0 is returned.
 */
int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                         size_t buf_size);

/*
 * Parse buffer `buf`, `buf_len` that contains multipart form data chunks.
 * Store chunk name in a `var_name`, `var_name_len` buffer.
 * If a chunk is an uploaded file, then `file_name`, `file_name_len` is
 * filled with an uploaded file name. `chunk`, `chunk_len`
 * points to the chunk data.
 *
 * Return: number of bytes to skip to the next chunk, or 0 if there are
 *         no more chunks.
 *
 * Usage example:
 *
 * ```c
 *    static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *      switch(ev) {
 *        case MG_EV_HTTP_REQUEST: {
 *          struct http_message *hm = (struct http_message *) ev_data;
 *          char var_name[100], file_name[100];
 *          const char *chunk;
 *          size_t chunk_len, n1, n2;
 *
 *          n1 = n2 = 0;
 *          while ((n2 = mg_parse_multipart(hm->body.p + n1,
 *                                          hm->body.len - n1,
 *                                          var_name, sizeof(var_name),
 *                                          file_name, sizeof(file_name),
 *                                          &chunk, &chunk_len)) > 0) {
 *            printf("var: %s, file_name: %s, size: %d, chunk: [%.*s]\n",
 *                   var_name, file_name, (int) chunk_len,
 *                   (int) chunk_len, chunk);
 *            n1 += n2;
 *          }
 *        }
 *        break;
 * ```
 */
size_t mg_parse_multipart(const char *buf, size_t buf_len, char *var_name,
                          size_t var_name_len, char *file_name,
                          size_t file_name_len, const char **chunk,
                          size_t *chunk_len);

/*
 * Fetch an HTTP form variable.
 *
 * Fetch a variable `name` from a `buf` into a buffer specified by
 * `dst`, `dst_len`. Destination is always zero-terminated. Return length
 * of a fetched variable. If not found, 0 is returned. `buf` must be
 * valid url-encoded buffer. If destination is too small, `-1` is returned.
 */
int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len);

/*
 * Decode URL-encoded string.
 *
 * Source string is specified by (`src`, `src_len`), and destination is
 * (`dst`, `dst_len`). If `is_form_url_encoded` is non-zero, then
 * `+` character is decoded as a blank space character. This function
 * guarantees to `\0`-terminate the destination. If destination is too small,
 * then source string is partially decoded and `-1` is returned. Otherwise,
 * a length of decoded string is returned, not counting final `\0`.
 */
int mg_url_decode(const char *src, int src_len, char *dst, int dst_len,
                  int is_form_url_encoded);

/* Create Digest authentication header for client request. */
int mg_http_create_digest_auth_header(char *buf, size_t buf_len,
                                      const char *method, const char *uri,
                                      const char *auth_domain, const char *user,
                                      const char *passwd);

/*
 * Helper function that creates outbound HTTP connection.
 *
 * `url` is a URL to fetch. It must be properly URL-encoded, e.g. have
 * no spaces, etc. By default, `mg_connect_http()` sends Connection and
 * Host headers. `extra_headers` is an extra HTTP headers to send, e.g.
 * `"User-Agent: my-app\r\n"`.
 * If `post_data` is NULL, then GET request is created. Otherwise, POST request
 * is created with the specified POST data. Note that if the data being posted
 * is a form submission, the `Content-Type` header should be set accordingly
 * (see example below).
 *
 * Examples:
 *
 * ```c
 *   nc1 = mg_connect_http(mgr, ev_handler_1, "http://www.google.com", NULL,
 *                         NULL);
 *   nc2 = mg_connect_http(mgr, ev_handler_1, "https://github.com", NULL, NULL);
 *   nc3 = mg_connect_http(
 *       mgr, ev_handler_1, "my_server:8000/form_submit/",
 *       "Content-Type: application/x-www-form-urlencoded\r\n",
 *       "var_1=value_1&var_2=value_2");
 * ```
 */
struct mg_connection *mg_connect_http(struct mg_mgr *mgr,
                                      mg_event_handler_t event_handler,
                                      const char *url,
                                      const char *extra_headers,
                                      const char *post_data);

/*
 * Helper function that creates outbound HTTP connection.
 *
 * Mostly identical to mg_connect_http, but allows to provide extra parameters
 * (for example, SSL parameters
 */
struct mg_connection *mg_connect_http_opt(struct mg_mgr *mgr,
                                          mg_event_handler_t ev_handler,
                                          struct mg_connect_opts opts,
                                          const char *url,
                                          const char *extra_headers,
                                          const char *post_data);
/*
 * This structure defines how `mg_serve_http()` works.
 * Best practice is to set only required settings, and leave the rest as NULL.
 */
struct mg_serve_http_opts {
  /* Path to web root directory */
  const char *document_root;

  /* List of index files. Default is "" */
  const char *index_files;

  /*
   * Leave as NULL to disable authentication.
   * To enable directory protection with authentication, set this to ".htpasswd"
   * Then, creating ".htpasswd" file in any directory automatically protects
   * it with digest authentication.
   * Use `mongoose` web server binary, or `htdigest` Apache utility to
   * create/manipulate passwords file.
   * Make sure `auth_domain` is set to a valid domain name.
   */
  const char *per_directory_auth_file;

  /* Authorization domain (domain name of this web server) */
  const char *auth_domain;

  /*
   * Leave as NULL to disable authentication.
   * Normally, only selected directories in the document root are protected.
   * If absolutely every access to the web server needs to be authenticated,
   * regardless of the URI, set this option to the path to the passwords file.
   * Format of that file is the same as ".htpasswd" file. Make sure that file
   * is located outside document root to prevent people fetching it.
   */
  const char *global_auth_file;

  /* Set to "no" to disable directory listing. Enabled by default. */
  const char *enable_directory_listing;

  /* SSI files pattern. If not set, "**.shtml$|**.shtm$" is used. */
  const char *ssi_pattern;

  /* IP ACL. By default, NULL, meaning all IPs are allowed to connect */
  const char *ip_acl;

  /* URL rewrites.
   *
   * Comma-separated list of `uri_pattern=file_or_directory_path` rewrites.
   * When HTTP request is received, Mongoose constructs a file name from the
   * requested URI by combining `document_root` and the URI. However, if the
   * rewrite option is used and `uri_pattern` matches requested URI, then
   * `document_root` is ignored. Instead, `file_or_directory_path` is used,
   * which should be a full path name or a path relative to the web server's
   * current working directory. Note that `uri_pattern`, as all Mongoose
   * patterns, is a prefix pattern.
   *
   * If uri_pattern starts with `@` symbol, then Mongoose compares it with the
   * HOST header of the request. If they are equal, Mongoose sets document root
   * to `file_or_directory_path`, implementing virtual hosts support.
   * Example: `@foo.com=/document/root/for/foo.com`
   *
   * If `uri_pattern` starts with `%` symbol, then Mongoose compares it with
   * the listening port. If they match, then Mongoose issues a 301 redirect.
   * For example, to redirect all HTTP requests to the
   * HTTPS port, do `%80=https://my.site.com`. Note that the request URI is
   * automatically appended to the redirect location.
   */
  const char *url_rewrites;

  /* DAV document root. If NULL, DAV requests are going to fail. */
  const char *dav_document_root;

  /*
   * DAV passwords file. If NULL, DAV requests are going to fail.
   * If passwords file is set to "-", then DAV auth is disabled.
   */
  const char *dav_auth_file;

  /* Glob pattern for the files to hide. */
  const char *hidden_file_pattern;

  /* Set to non-NULL to enable CGI, e.g. **.cgi$|**.php$" */
  const char *cgi_file_pattern;

  /* If not NULL, ignore CGI script hashbang and use this interpreter */
  const char *cgi_interpreter;

  /*
   * Comma-separated list of Content-Type overrides for path suffixes, e.g.
   * ".txt=text/plain; charset=utf-8,.c=text/plain"
   */
  const char *custom_mime_types;

  /*
   * Extra HTTP headers to add to each server response.
   * Example: to enable CORS, set this to "Access-Control-Allow-Origin: *".
   */
  const char *extra_headers;
};

/*
 * Serve given HTTP request according to the `options`.
 *
 * Example code snippet:
 *
 * ```c
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   struct http_message *hm = (struct http_message *) ev_data;
 *   struct mg_serve_http_opts opts = { .document_root = "/var/www" };  // C99
 *
 *   switch (ev) {
 *     case MG_EV_HTTP_REQUEST:
 *       mg_serve_http(nc, hm, opts);
 *       break;
 *     default:
 *       break;
 *   }
 * }
 * ```
 */
void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts);

/*
 * Register callback for specified http endpoint
 * Note: if callback is registered it is called instead of
 * callback provided in mg_bind
 *
 * Example code snippet:
 *
 * ```c
 * static void handle_hello1(struct mg_connection *nc, int ev, void *ev_data) {
 *   (void) ev; (void) ev_data;
 *   mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello1]");
 *  nc->flags |= MG_F_SEND_AND_CLOSE;
 * }
 *
 * static void handle_hello1(struct mg_connection *nc, int ev, void *ev_data) {
 *  (void) ev; (void) ev_data;
 *   mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello2]");
 *  nc->flags |= MG_F_SEND_AND_CLOSE;
 * }
 *
 * void init() {
 *   nc = mg_bind(&mgr, local_addr, cb1);
 *   mg_register_http_endpoint(nc, "/hello1", handle_hello1);
 *   mg_register_http_endpoint(nc, "/hello1/hello2", handle_hello2);
 * }
 * ```
 */
void mg_register_http_endpoint(struct mg_connection *nc, const char *uri_path,
                               mg_event_handler_t handler);

#ifdef MG_ENABLE_HTTP_STREAMING_MULTIPART

/* Callback prototype for `mg_file_upload_handler()`. */
typedef struct mg_str (*mg_fu_fname_fn)(struct mg_connection *nc,
                                        struct mg_str fname);

/*
 * File upload handler.
 * This handler can be used to implement file uploads with minimum code.
 * This handler will process MG_EV_HTTP_PART_* events and store file data into
 * a local file.
 * `local_name_fn` will be invoked with whatever name was provided by the client
 * and will expect the name of the local file to open. Return value of NULL will
 * abort file upload (client will get a "403 Forbidden" response). If non-null,
 * the returned string must be heap-allocated and will be freed by the caller.
 * Exception: it is ok to return the same string verbatim.
 *
 * Example:
 *
 * ```c
 * struct mg_str upload_fname(struct mg_connection *nc, struct mg_str fname) {
 *   // Just return the same filename. Do not actually do this except in test!
 *   // fname is user-controlled and needs to be sanitized.
 *   return fname;
 * }
 * void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   switch (ev) {
 *     ...
 *     case MG_EV_HTTP_PART_BEGIN:
 *     case MG_EV_HTTP_PART_DATA:
 *     case MG_EV_HTTP_PART_END:
 *       mg_file_upload_handler(nc, ev, ev_data, upload_fname);
 *       break;
 *   }
 * }
 * ```
 */
void mg_file_upload_handler(struct mg_connection *nc, int ev, void *ev_data,
                            mg_fu_fname_fn local_name_fn);
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_HTTP_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === JSON-RPC
 */

#ifndef CS_MONGOOSE_SRC_JSON_RPC_H_
#define CS_MONGOOSE_SRC_JSON_RPC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* JSON-RPC request */
struct mg_rpc_request {
  struct json_token *message; /* Whole RPC message */
  struct json_token *id;      /* Message ID */
  struct json_token *method;  /* Method name */
  struct json_token *params;  /* Method params */
};

/* JSON-RPC response */
struct mg_rpc_reply {
  struct json_token *message; /* Whole RPC message */
  struct json_token *id;      /* Message ID */
  struct json_token *result;  /* Remote call result */
};

/* JSON-RPC error */
struct mg_rpc_error {
  struct json_token *message;       /* Whole RPC message */
  struct json_token *id;            /* Message ID */
  struct json_token *error_code;    /* error.code */
  struct json_token *error_message; /* error.message */
  struct json_token *error_data;    /* error.data, can be NULL */
};

/*
 * Parse JSON-RPC reply contained in `buf`, `len` into JSON tokens array
 * `toks`, `max_toks`. If buffer contains valid reply, `reply` structure is
 * populated. The result of RPC call is located in `reply.result`. On error,
 * `error` structure is populated. Returns: the result of calling
 * `parse_json(buf, len, toks, max_toks)`:
 *
 * On success, an offset inside `json_string` is returned
 * where parsing has finished. On failure, a negative number is
 * returned, one of:
 *
 * - `#define JSON_STRING_INVALID           -1`
 * - `#define JSON_STRING_INCOMPLETE        -2`
 * - `#define JSON_TOKEN_ARRAY_TOO_SMALL    -3`
 */
int mg_rpc_parse_reply(const char *buf, int len, struct json_token *toks,
                       int max_toks, struct mg_rpc_reply *,
                       struct mg_rpc_error *);

/*
 * Create JSON-RPC request in a given buffer.
 *
 * Return length of the request, which
 * can be larger then `len` that indicates an overflow.
 * `params_fmt` format string should conform to `json_emit()` API,
 * see https://github.com/cesanta/frozen
 */
int mg_rpc_create_request(char *buf, int len, const char *method,
                          const char *id, const char *params_fmt, ...);

/*
 * Create JSON-RPC reply in a given buffer.
 *
 * Return length of the reply, which
 * can be larger then `len` that indicates an overflow.
 * `result_fmt` format string should conform to `json_emit()` API,
 * see https://github.com/cesanta/frozen
 */
int mg_rpc_create_reply(char *buf, int len, const struct mg_rpc_request *req,
                        const char *result_fmt, ...);

/*
 * Create JSON-RPC error reply in a given buffer.
 *
 * Return length of the error, which
 * can be larger then `len` that indicates an overflow.
 * `fmt` format string should conform to `json_emit()` API,
 * see https://github.com/cesanta/frozen
 */
int mg_rpc_create_error(char *buf, int len, struct mg_rpc_request *req,
                        int code, const char *message, const char *fmt, ...);

/* JSON-RPC standard error codes */
#define JSON_RPC_PARSE_ERROR (-32700)
#define JSON_RPC_INVALID_REQUEST_ERROR (-32600)
#define JSON_RPC_METHOD_NOT_FOUND_ERROR (-32601)
#define JSON_RPC_INVALID_PARAMS_ERROR (-32602)
#define JSON_RPC_INTERNAL_ERROR (-32603)
#define JSON_RPC_SERVER_ERROR (-32000)

/*
 * Create JSON-RPC error in a given buffer.
 *
 * Return length of the error, which
 * can be larger then `len` that indicates an overflow. See
 * JSON_RPC_*_ERROR definitions for standard error values:
 *
 * - `#define JSON_RPC_PARSE_ERROR (-32700)`
 * - `#define JSON_RPC_INVALID_REQUEST_ERROR (-32600)`
 * - `#define JSON_RPC_METHOD_NOT_FOUND_ERROR (-32601)`
 * - `#define JSON_RPC_INVALID_PARAMS_ERROR (-32602)`
 * - `#define JSON_RPC_INTERNAL_ERROR (-32603)`
 * - `#define JSON_RPC_SERVER_ERROR (-32000)`
 */
int mg_rpc_create_std_error(char *buf, int len, struct mg_rpc_request *req,
                            int code);

typedef int (*mg_rpc_handler_t)(char *buf, int len, struct mg_rpc_request *req);

/*
 * Dispatches a JSON-RPC request.
 *
 * Parses JSON-RPC request contained in `buf`, `len`.
 * Then, dispatches the request to the correct handler method.
 * Valid method names should be specified in NULL
 * terminated array `methods`, and corresponding handlers in `handlers`.
 * Result is put in `dst`, `dst_len`. Return: length of the result, which
 * can be larger then `dst_len` that indicates an overflow.
 * Overflown bytes are not written to the buffer.
 * If method is not found, an error is automatically generated.
 */
int mg_rpc_dispatch(const char *buf, int, char *dst, int dst_len,
                    const char **methods, mg_rpc_handler_t *handlers);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_JSON_RPC_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/*
 * === MQTT
 */

#ifndef CS_MONGOOSE_SRC_MQTT_H_
#define CS_MONGOOSE_SRC_MQTT_H_


struct mg_mqtt_message {
  int cmd;
  struct mg_str payload;
  int qos;
  uint8_t connack_ret_code; /* connack */
  uint16_t message_id;      /* puback */
  char *topic;
};

struct mg_mqtt_topic_expression {
  const char *topic;
  uint8_t qos;
};

struct mg_send_mqtt_handshake_opts {
  unsigned char flags; /* connection flags */
  uint16_t keep_alive;
  const char *will_topic;
  const char *will_message;
  const char *user_name;
  const char *password;
};

/* Message types */
#define MG_MQTT_CMD_CONNECT 1
#define MG_MQTT_CMD_CONNACK 2
#define MG_MQTT_CMD_PUBLISH 3
#define MG_MQTT_CMD_PUBACK 4
#define MG_MQTT_CMD_PUBREC 5
#define MG_MQTT_CMD_PUBREL 6
#define MG_MQTT_CMD_PUBCOMP 7
#define MG_MQTT_CMD_SUBSCRIBE 8
#define MG_MQTT_CMD_SUBACK 9
#define MG_MQTT_CMD_UNSUBSCRIBE 10
#define MG_MQTT_CMD_UNSUBACK 11
#define MG_MQTT_CMD_PINGREQ 12
#define MG_MQTT_CMD_PINGRESP 13
#define MG_MQTT_CMD_DISCONNECT 14

/* MQTT event types */
#define MG_MQTT_EVENT_BASE 200
#define MG_EV_MQTT_CONNECT (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_CONNECT)
#define MG_EV_MQTT_CONNACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_CONNACK)
#define MG_EV_MQTT_PUBLISH (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBLISH)
#define MG_EV_MQTT_PUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBACK)
#define MG_EV_MQTT_PUBREC (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBREC)
#define MG_EV_MQTT_PUBREL (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBREL)
#define MG_EV_MQTT_PUBCOMP (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PUBCOMP)
#define MG_EV_MQTT_SUBSCRIBE (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_SUBSCRIBE)
#define MG_EV_MQTT_SUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_SUBACK)
#define MG_EV_MQTT_UNSUBSCRIBE (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_UNSUBSCRIBE)
#define MG_EV_MQTT_UNSUBACK (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_UNSUBACK)
#define MG_EV_MQTT_PINGREQ (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PINGREQ)
#define MG_EV_MQTT_PINGRESP (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_PINGRESP)
#define MG_EV_MQTT_DISCONNECT (MG_MQTT_EVENT_BASE + MG_MQTT_CMD_DISCONNECT)

/* Message flags */
#define MG_MQTT_RETAIN 0x1
#define MG_MQTT_DUP 0x4
#define MG_MQTT_QOS(qos) ((qos) << 1)
#define MG_MQTT_GET_QOS(flags) (((flags) &0x6) >> 1)
#define MG_MQTT_SET_QOS(flags, qos) (flags) = ((flags) & ~0x6) | ((qos) << 1)

/* Connection flags */
#define MG_MQTT_CLEAN_SESSION 0x02
#define MG_MQTT_HAS_WILL 0x04
#define MG_MQTT_WILL_RETAIN 0x20
#define MG_MQTT_HAS_PASSWORD 0x40
#define MG_MQTT_HAS_USER_NAME 0x80
#define MG_MQTT_GET_WILL_QOS(flags) (((flags) &0x18) >> 3)
#define MG_MQTT_SET_WILL_QOS(flags, qos) \
  (flags) = ((flags) & ~0x18) | ((qos) << 3)

/* CONNACK return codes */
#define MG_EV_MQTT_CONNACK_ACCEPTED 0
#define MG_EV_MQTT_CONNACK_UNACCEPTABLE_VERSION 1
#define MG_EV_MQTT_CONNACK_IDENTIFIER_REJECTED 2
#define MG_EV_MQTT_CONNACK_SERVER_UNAVAILABLE 3
#define MG_EV_MQTT_CONNACK_BAD_AUTH 4
#define MG_EV_MQTT_CONNACK_NOT_AUTHORIZED 5

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * Attach built-in MQTT event handler to the given connection.
 *
 * The user-defined event handler will receive following extra events:
 *
 * - MG_EV_MQTT_CONNACK
 * - MG_EV_MQTT_PUBLISH
 * - MG_EV_MQTT_PUBACK
 * - MG_EV_MQTT_PUBREC
 * - MG_EV_MQTT_PUBREL
 * - MG_EV_MQTT_PUBCOMP
 * - MG_EV_MQTT_SUBACK
 */
void mg_set_protocol_mqtt(struct mg_connection *nc);

/* Send MQTT handshake. */
void mg_send_mqtt_handshake(struct mg_connection *nc, const char *client_id);

/* Send MQTT handshake with optional parameters. */
void mg_send_mqtt_handshake_opt(struct mg_connection *nc, const char *client_id,
                                struct mg_send_mqtt_handshake_opts);

/* Publish a message to a given topic. */
void mg_mqtt_publish(struct mg_connection *nc, const char *topic,
                     uint16_t message_id, int flags, const void *data,
                     size_t len);

/* Subscribe to a bunch of topics. */
void mg_mqtt_subscribe(struct mg_connection *nc,
                       const struct mg_mqtt_topic_expression *topics,
                       size_t topics_len, uint16_t message_id);

/* Unsubscribe from a bunch of topics. */
void mg_mqtt_unsubscribe(struct mg_connection *nc, char **topics,
                         size_t topics_len, uint16_t message_id);

/* Send a DISCONNECT command. */
void mg_mqtt_disconnect(struct mg_connection *nc);

/* Send a CONNACK command with a given `return_code`. */
void mg_mqtt_connack(struct mg_connection *nc, uint8_t return_code);

/* Send a PUBACK command with a given `message_id`. */
void mg_mqtt_puback(struct mg_connection *nc, uint16_t message_id);

/* Send a PUBREC command with a given `message_id`. */
void mg_mqtt_pubrec(struct mg_connection *nc, uint16_t message_id);

/* Send a PUBREL command with a given `message_id`. */
void mg_mqtt_pubrel(struct mg_connection *nc, uint16_t message_id);

/* Send a PUBCOMP command with a given `message_id`. */
void mg_mqtt_pubcomp(struct mg_connection *nc, uint16_t message_id);

/*
 * Send a SUBACK command with a given `message_id`
 * and a sequence of granted QoSs.
 */
void mg_mqtt_suback(struct mg_connection *nc, uint8_t *qoss, size_t qoss_len,
                    uint16_t message_id);

/* Send a UNSUBACK command with a given `message_id`. */
void mg_mqtt_unsuback(struct mg_connection *nc, uint16_t message_id);

/* Send a PINGREQ command. */
void mg_mqtt_ping(struct mg_connection *nc);

/* Send a PINGRESP command. */
void mg_mqtt_pong(struct mg_connection *nc);

/*
 * Extract the next topic expression from a SUBSCRIBE command payload.
 *
 * Topic expression name will point to a string in the payload buffer.
 * Return the pos of the next topic expression or -1 when the list
 * of topics is exhausted.
 */
int mg_mqtt_next_subscribe_topic(struct mg_mqtt_message *msg,
                                 struct mg_str *topic, uint8_t *qos, int pos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_MQTT_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/*
 * === MQTT Broker
 */

#ifndef CS_MONGOOSE_SRC_MQTT_BROKER_H_
#define CS_MONGOOSE_SRC_MQTT_BROKER_H_

#ifdef MG_ENABLE_MQTT_BROKER


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_MQTT_MAX_SESSION_SUBSCRIPTIONS 512;

struct mg_mqtt_broker;

/* MQTT session (Broker side). */
struct mg_mqtt_session {
  struct mg_mqtt_broker *brk;          /* Broker */
  struct mg_mqtt_session *next, *prev; /* mg_mqtt_broker::sessions linkage */
  struct mg_connection *nc;            /* Connection with the client */
  size_t num_subscriptions;            /* Size of `subscriptions` array */
  struct mg_mqtt_topic_expression *subscriptions;
  void *user_data; /* User data */
};

/* MQTT broker. */
struct mg_mqtt_broker {
  struct mg_mqtt_session *sessions; /* Session list */
  void *user_data;                  /* User data */
};

/* Initialize a MQTT broker. */
void mg_mqtt_broker_init(struct mg_mqtt_broker *brk, void *user_data);

/*
 * Process a MQTT broker message.
 *
 * Listening connection expects a pointer to an initialized `mg_mqtt_broker`
 * structure in the `user_data` field.
 *
 * Basic usage:
 *
 * ```c
 * mg_mqtt_broker_init(&brk, NULL);
 *
 * if ((nc = mg_bind(&mgr, address, mg_mqtt_broker)) == NULL) {
 *   // fail;
 * }
 * nc->user_data = &brk;
 * ```
 *
 * New incoming connections will receive a `mg_mqtt_session` structure
 * in the connection `user_data`. The original `user_data` will be stored
 * in the `user_data` field of the session structure. This allows the user
 * handler to store user data before `mg_mqtt_broker` creates the session.
 *
 * Since only the MG_EV_ACCEPT message is processed by the listening socket,
 * for most events the `user_data` will thus point to a `mg_mqtt_session`.
 */
void mg_mqtt_broker(struct mg_connection *brk, int ev, void *data);

/*
 * Iterate over all mqtt sessions connections. Example:
 *
 * ```c
 * struct mg_mqtt_session *s;
 * for (s = mg_mqtt_next(brk, NULL); s != NULL; s = mg_mqtt_next(brk, s)) {
 *   // Do something
 * }
 * ```
 */
struct mg_mqtt_session *mg_mqtt_next(struct mg_mqtt_broker *brk,
                                     struct mg_mqtt_session *s);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_MQTT_BROKER */
#endif /* CS_MONGOOSE_SRC_MQTT_BROKER_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === DNS
 */

#ifndef CS_MONGOOSE_SRC_DNS_H_
#define CS_MONGOOSE_SRC_DNS_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_DNS_A_RECORD 0x01     /* Lookup IP address */
#define MG_DNS_CNAME_RECORD 0x05 /* Lookup CNAME */
#define MG_DNS_AAAA_RECORD 0x1c  /* Lookup IPv6 address */
#define MG_DNS_MX_RECORD 0x0f    /* Lookup mail server for domain */

#define MG_MAX_DNS_QUESTIONS 32
#define MG_MAX_DNS_ANSWERS 32

#define MG_DNS_MESSAGE 100 /* High-level DNS message event */

enum mg_dns_resource_record_kind {
  MG_DNS_INVALID_RECORD = 0,
  MG_DNS_QUESTION,
  MG_DNS_ANSWER
};

/* DNS resource record. */
struct mg_dns_resource_record {
  struct mg_str name; /* buffer with compressed name */
  int rtype;
  int rclass;
  int ttl;
  enum mg_dns_resource_record_kind kind;
  struct mg_str rdata; /* protocol data (can be a compressed name) */
};

/* DNS message (request and response). */
struct mg_dns_message {
  struct mg_str pkt; /* packet body */
  uint16_t flags;
  uint16_t transaction_id;
  int num_questions;
  int num_answers;
  struct mg_dns_resource_record questions[MG_MAX_DNS_QUESTIONS];
  struct mg_dns_resource_record answers[MG_MAX_DNS_ANSWERS];
};

struct mg_dns_resource_record *mg_dns_next_record(
    struct mg_dns_message *msg, int query, struct mg_dns_resource_record *prev);

/*
 * Parse the record data from a DNS resource record.
 *
 *  - A:     struct in_addr *ina
 *  - AAAA:  struct in6_addr *ina
 *  - CNAME: char buffer
 *
 * Returns -1 on error.
 *
 * TODO(mkm): MX
 */
int mg_dns_parse_record_data(struct mg_dns_message *msg,
                             struct mg_dns_resource_record *rr, void *data,
                             size_t data_len);

/*
 * Send a DNS query to the remote end.
 */
void mg_send_dns_query(struct mg_connection *nc, const char *name,
                       int query_type);

/*
 * Insert a DNS header to an IO buffer.
 *
 * Return number of bytes inserted.
 */
int mg_dns_insert_header(struct mbuf *io, size_t pos,
                         struct mg_dns_message *msg);

/*
 * Append already encoded body from an existing message.
 *
 * This is useful when generating a DNS reply message which includes
 * all question records.
 *
 * Return number of appened bytes.
 */
int mg_dns_copy_body(struct mbuf *io, struct mg_dns_message *msg);

/*
 * Encode and append a DNS resource record to an IO buffer.
 *
 * The record metadata is taken from the `rr` parameter, while the name and data
 * are taken from the parameters, encoded in the appropriate format depending on
 * record type, and stored in the IO buffer. The encoded values might contain
 * offsets within the IO buffer. It's thus important that the IO buffer doesn't
 * get trimmed while a sequence of records are encoded while preparing a DNS
 *reply.
 *
 * This function doesn't update the `name` and `rdata` pointers in the `rr`
 *struct
 * because they might be invalidated as soon as the IO buffer grows again.
 *
 * Return the number of bytes appened or -1 in case of error.
 */
int mg_dns_encode_record(struct mbuf *io, struct mg_dns_resource_record *rr,
                         const char *name, size_t nlen, const void *rdata,
                         size_t rlen);

/* Low-level: parses a DNS response. */
int mg_parse_dns(const char *buf, int len, struct mg_dns_message *msg);

/*
 * Uncompress a DNS compressed name.
 *
 * The containing dns message is required because the compressed encoding
 * and reference suffixes present elsewhere in the packet.
 *
 * If name is less than `dst_len` characters long, the remainder
 * of `dst` is terminated with `\0' characters. Otherwise, `dst` is not
 *terminated.
 *
 * If `dst_len` is 0 `dst` can be NULL.
 * Return the uncompressed name length.
 */
size_t mg_dns_uncompress_name(struct mg_dns_message *msg, struct mg_str *name,
                              char *dst, int dst_len);

/*
 * Attach built-in DNS event handler to the given listening connection.
 *
 * DNS event handler parses incoming UDP packets, treating them as DNS
 * requests. If incoming packet gets successfully parsed by the DNS event
 * handler, a user event handler will receive `MG_DNS_REQUEST` event, with
 * `ev_data` pointing to the parsed `struct mg_dns_message`.
 *
 * See
 * [captive_dns_server](https://github.com/cesanta/mongoose/tree/master/examples/captive_dns_server)
 * example on how to handle DNS request and send DNS reply.
 */
void mg_set_protocol_dns(struct mg_connection *nc);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_DNS_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === DNS server
 *
 * Disabled by default; enable with `-DMG_ENABLE_DNS_SERVER`.
 */

#ifndef CS_MONGOOSE_SRC_DNS_SERVER_H_
#define CS_MONGOOSE_SRC_DNS_SERVER_H_

#ifdef MG_ENABLE_DNS_SERVER


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_DNS_SERVER_DEFAULT_TTL 3600

struct mg_dns_reply {
  struct mg_dns_message *msg;
  struct mbuf *io;
  size_t start;
};

/*
 * Create a DNS reply.
 *
 * The reply will be based on an existing query message `msg`.
 * The query body will be appended to the output buffer.
 * "reply + recursion allowed" will be added to the message flags and
 * message's num_answers will be set to 0.
 *
 * Answer records can be appended with `mg_dns_send_reply` or by lower
 * level function defined in the DNS API.
 *
 * In order to send the reply use `mg_dns_send_reply`.
 * It's possible to use a connection's send buffer as reply buffers,
 * and it will work for both UDP and TCP connections.
 *
 * Example:
 *
 * ```c
 * reply = mg_dns_create_reply(&nc->send_mbuf, msg);
 * for (i = 0; i < msg->num_questions; i++) {
 *   rr = &msg->questions[i];
 *   if (rr->rtype == MG_DNS_A_RECORD) {
 *     mg_dns_reply_record(&reply, rr, 3600, &dummy_ip_addr, 4);
 *   }
 * }
 * mg_dns_send_reply(nc, &reply);
 * ```
 */
struct mg_dns_reply mg_dns_create_reply(struct mbuf *io,
                                        struct mg_dns_message *msg);

/*
 * Append a DNS reply record to the IO buffer and to the DNS message.
 *
 * The message num_answers field will be incremented. It's caller's duty
 * to ensure num_answers is propertly initialized.
 *
 * Returns -1 on error.
 */
int mg_dns_reply_record(struct mg_dns_reply *reply,
                        struct mg_dns_resource_record *question,
                        const char *name, int rtype, int ttl, const void *rdata,
                        size_t rdata_len);

/*
 * Send a DNS reply through a connection.
 *
 * The DNS data is stored in an IO buffer pointed by reply structure in `r`.
 * This function mutates the content of that buffer in order to ensure that
 * the DNS header reflects size and flags of the mssage, that might have been
 * updated either with `mg_dns_reply_record` or by direct manipulation of
 * `r->message`.
 *
 * Once sent, the IO buffer will be trimmed unless the reply IO buffer
 * is the connection's send buffer and the connection is not in UDP mode.
 */
void mg_dns_send_reply(struct mg_connection *nc, struct mg_dns_reply *r);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_DNS_SERVER */
#endif /* CS_MONGOOSE_SRC_DNS_SERVER_H_ */
/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

/*
 * === Asynchronouns DNS resolver
 */

#ifndef CS_MONGOOSE_SRC_RESOLV_H_
#define CS_MONGOOSE_SRC_RESOLV_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum mg_resolve_err {
  MG_RESOLVE_OK = 0,
  MG_RESOLVE_NO_ANSWERS = 1,
  MG_RESOLVE_EXCEEDED_RETRY_COUNT = 2,
  MG_RESOLVE_TIMEOUT = 3
};

typedef void (*mg_resolve_callback_t)(struct mg_dns_message *dns_message,
                                      void *user_data, enum mg_resolve_err);

/* Options for `mg_resolve_async_opt`. */
struct mg_resolve_async_opts {
  const char *nameserver_url;
  int max_retries;    /* defaults to 2 if zero */
  int timeout;        /* in seconds; defaults to 5 if zero */
  int accept_literal; /* pseudo-resolve literal ipv4 and ipv6 addrs */
  int only_literal;   /* only resolves literal addrs; sync cb invocation */
  struct mg_connection **dns_conn; /* return DNS connection */
};

/* See `mg_resolve_async_opt()` */
int mg_resolve_async(struct mg_mgr *mgr, const char *name, int query,
                     mg_resolve_callback_t cb, void *data);

/*
 * Resolved a DNS name asynchronously.
 *
 * Upon successful resolution, the user callback will be invoked
 * with the full DNS response message and a pointer to the user's
 * context `data`.
 *
 * In case of timeout while performing the resolution the callback
 * will receive a NULL `msg`.
 *
 * The DNS answers can be extracted with `mg_next_record` and
 * `mg_dns_parse_record_data`:
 *
 * [source,c]
 * ----
 * struct in_addr ina;
 * struct mg_dns_resource_record *rr = mg_next_record(msg, MG_DNS_A_RECORD,
 *   NULL);
 * mg_dns_parse_record_data(msg, rr, &ina, sizeof(ina));
 * ----
 */
int mg_resolve_async_opt(struct mg_mgr *mgr, const char *name, int query,
                         mg_resolve_callback_t cb, void *data,
                         struct mg_resolve_async_opts opts);

/*
 * Resolve a name from `/etc/hosts`.
 *
 * Returns 0 on success, -1 on failure.
 */
int mg_resolve_from_hosts_file(const char *host, union socket_address *usa);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CS_MONGOOSE_SRC_RESOLV_H_ */
/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

/*
 * === CoAP
 *
 * CoAP message format:
 *
 * ```
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * |Ver| T | TKL | Code | Message ID | Token (if any, TKL bytes) ...
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * | Options (if any) ...            |1 1 1 1 1 1 1 1| Payload (if any) ...
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 * ```
 */

#ifndef CS_MONGOOSE_SRC_COAP_H_
#define CS_MONGOOSE_SRC_COAP_H_

#ifdef MG_ENABLE_COAP

#define MG_COAP_MSG_TYPE_FIELD 0x2
#define MG_COAP_CODE_CLASS_FIELD 0x4
#define MG_COAP_CODE_DETAIL_FIELD 0x8
#define MG_COAP_MSG_ID_FIELD 0x10
#define MG_COAP_TOKEN_FIELD 0x20
#define MG_COAP_OPTIOMG_FIELD 0x40
#define MG_COAP_PAYLOAD_FIELD 0x80

#define MG_COAP_ERROR 0x10000
#define MG_COAP_FORMAT_ERROR (MG_COAP_ERROR | 0x20000)
#define MG_COAP_IGNORE (MG_COAP_ERROR | 0x40000)
#define MG_COAP_NOT_ENOUGH_DATA (MG_COAP_ERROR | 0x80000)
#define MG_COAP_NETWORK_ERROR (MG_COAP_ERROR | 0x100000)

#define MG_COAP_MSG_CON 0
#define MG_COAP_MSG_NOC 1
#define MG_COAP_MSG_ACK 2
#define MG_COAP_MSG_RST 3
#define MG_COAP_MSG_MAX 3

#define MG_COAP_CODECLASS_REQUEST 0
#define MG_COAP_CODECLASS_RESP_OK 2
#define MG_COAP_CODECLASS_CLIENT_ERR 4
#define MG_COAP_CODECLASS_SRV_ERR 5

#define MG_COAP_EVENT_BASE 300
#define MG_EV_COAP_CON (MG_COAP_EVENT_BASE + MG_COAP_MSG_CON)
#define MG_EV_COAP_NOC (MG_COAP_EVENT_BASE + MG_COAP_MSG_NOC)
#define MG_EV_COAP_ACK (MG_COAP_EVENT_BASE + MG_COAP_MSG_ACK)
#define MG_EV_COAP_RST (MG_COAP_EVENT_BASE + MG_COAP_MSG_RST)

/*
 * CoAP options.
 * Use mg_coap_add_option and mg_coap_free_options
 * for creation and destruction.
 */
struct mg_coap_option {
  struct mg_coap_option *next;
  uint32_t number;
  struct mg_str value;
};

/* CoAP message. See RFC 7252 for details. */
struct mg_coap_message {
  uint32_t flags;
  uint8_t msg_type;
  uint8_t code_class;
  uint8_t code_detail;
  uint16_t msg_id;
  struct mg_str token;
  struct mg_coap_option *options;
  struct mg_str payload;
  struct mg_coap_option *optiomg_tail;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Set CoAP protocol handler - trigger CoAP specific events */
int mg_set_protocol_coap(struct mg_connection *nc);

/*
 * Add new option to mg_coap_message structure.
 * Returns pointer to the newly created option.
 */
struct mg_coap_option *mg_coap_add_option(struct mg_coap_message *cm,
                                          uint32_t number, char *value,
                                          size_t len);

/*
 * Free the memory allocated for options,
 * if cm paramater doesn't contain any option does nothing.
 */
void mg_coap_free_options(struct mg_coap_message *cm);

/*
 * Compose CoAP message from `mg_coap_message`
 * and send it into `nc` connection.
 * Return 0 on success. On error, it is a bitmask:
 *
 * - `#define MG_COAP_ERROR 0x10000`
 * - `#define MG_COAP_FORMAT_ERROR (MG_COAP_ERROR | 0x20000)`
 * - `#define MG_COAP_IGNORE (MG_COAP_ERROR | 0x40000)`
 * - `#define MG_COAP_NOT_ENOUGH_DATA (MG_COAP_ERROR | 0x80000)`
 * - `#define MG_COAP_NETWORK_ERROR (MG_COAP_ERROR | 0x100000)`
 */
uint32_t mg_coap_send_message(struct mg_connection *nc,
                              struct mg_coap_message *cm);

/*
 * Compose CoAP acknowledgement from `mg_coap_message`
 * and send it into `nc` connection.
 * Return value: see `mg_coap_send_message()`
 */
uint32_t mg_coap_send_ack(struct mg_connection *nc, uint16_t msg_id);

/*
 * Parse COAP message and fills mg_coap_message and returns cm->flags.
 * This is a helper function.
 *
 * NOTE: usually CoAP work over UDP, so lack of data means format error,
 * but in theory it is possible to use CoAP over TCP (according to RFC)
 *
 * The caller have to check results and treat COAP_NOT_ENOUGH_DATA according to
 * underlying protocol:
 *
 * - in case of UDP COAP_NOT_ENOUGH_DATA means COAP_FORMAT_ERROR,
 * - in case of TCP client can try to receive more data
 *
 * Return value: see `mg_coap_send_message()`
 */
uint32_t mg_coap_parse(struct mbuf *io, struct mg_coap_message *cm);

/*
 * Composes CoAP message from mg_coap_message structure.
 * This is a helper function.
 * Return value: see `mg_coap_send_message()`
 */
uint32_t mg_coap_compose(struct mg_coap_message *cm, struct mbuf *io);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_COAP */

#endif /* CS_MONGOOSE_SRC_COAP_H_ */
