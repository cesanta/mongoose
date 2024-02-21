// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2024 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see http://www.gnu.org/licenses/
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in https://www.mongoose.ws/licensing/
//
// SPDX-License-Identifier: GPL-2.0-only or commercial

#ifndef MONGOOSE_H
#define MONGOOSE_H

#define MG_VERSION "7.13"

#ifdef __cplusplus
extern "C" {
#endif


#define MG_ARCH_CUSTOM 0        // User creates its own mongoose_custom.h
#define MG_ARCH_UNIX 1          // Linux, BSD, Mac, ...
#define MG_ARCH_WIN32 2         // Windows
#define MG_ARCH_ESP32 3         // ESP32
#define MG_ARCH_ESP8266 4       // ESP8266
#define MG_ARCH_FREERTOS 5      // FreeRTOS
#define MG_ARCH_AZURERTOS 6     // MS Azure RTOS
#define MG_ARCH_ZEPHYR 7        // Zephyr RTOS
#define MG_ARCH_NEWLIB 8        // Bare metal ARM
#define MG_ARCH_CMSIS_RTOS1 9   // CMSIS-RTOS API v1 (Keil RTX)
#define MG_ARCH_TIRTOS 10       // Texas Semi TI-RTOS
#define MG_ARCH_RP2040 11       // Raspberry Pi RP2040
#define MG_ARCH_ARMCC 12        // Keil MDK-Core with Configuration Wizard
#define MG_ARCH_CMSIS_RTOS2 13  // CMSIS-RTOS API v2 (Keil RTX5, FreeRTOS)
#define MG_ARCH_RTTHREAD 14     // RT-Thread RTOS

#if !defined(MG_ARCH)
#if defined(__unix__) || defined(__APPLE__)
#define MG_ARCH MG_ARCH_UNIX
#elif defined(_WIN32)
#define MG_ARCH MG_ARCH_WIN32
#elif defined(ICACHE_FLASH) || defined(ICACHE_RAM_ATTR)
#define MG_ARCH MG_ARCH_ESP8266
#elif defined(__ZEPHYR__)
#define MG_ARCH MG_ARCH_ZEPHYR
#elif defined(ESP_PLATFORM)
#define MG_ARCH MG_ARCH_ESP32
#elif defined(FREERTOS_IP_H)
#define MG_ARCH MG_ARCH_FREERTOS
#define MG_ENABLE_FREERTOS_TCP 1
#elif defined(AZURE_RTOS_THREADX)
#define MG_ARCH MG_ARCH_AZURERTOS
#elif defined(PICO_TARGET_NAME)
#define MG_ARCH MG_ARCH_RP2040
#elif defined(__RTTHREAD__)
#define MG_ARCH MG_ARCH_RTTHREAD
#endif
#endif  // !defined(MG_ARCH)

#if !defined(MG_ARCH) || (MG_ARCH == MG_ARCH_CUSTOM)
#include "mongoose_custom.h"  // keep this include
#endif

#if !defined(MG_ARCH)
#error "MG_ARCH is not specified and we couldn't guess it. Set -D MG_ARCH=..."
#endif

// http://esr.ibiblio.org/?p=5095
#define MG_BIG_ENDIAN (*(uint16_t *) "\0\xff" < 0x100)















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

#define PATH_MAX FX_MAXIMUM_PATH
#define MG_DIRSEP '\\'

#define socklen_t int
#define closesocket(x) soc_close(x)

#undef FOPEN_MAX

#endif


#if MG_ARCH == MG_ARCH_ESP32

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <esp_timer.h>

#define MG_PATH_MAX 128

#endif


#if MG_ARCH == MG_ARCH_ESP8266

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#include <esp_system.h>

#define MG_PATH_MAX 128

#endif


#if MG_ARCH == MG_ARCH_FREERTOS

#include <ctype.h>
#if !defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP
#include <errno.h>
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // rand(), strtol(), atoi()
#include <string.h>
#if defined(__ARMCC_VERSION)
#define mode_t size_t
#include <time.h>
#include <alloca.h>
#else
#include <sys/stat.h>
#endif

#include <FreeRTOS.h>
#include <task.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

#define calloc(a, b) mg_calloc(a, b)
#define free(a) vPortFree(a)
#define malloc(a) pvPortMalloc(a)
#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)

// Re-route calloc/free to the FreeRTOS's functions, don't use stdlib
static inline void *mg_calloc(size_t cnt, size_t size) {
  void *p = pvPortMalloc(cnt * size);
  if (p != NULL) memset(p, 0, size * cnt);
  return p;
}

#define mkdir(a, b) mg_mkdir(a, b)
static inline int mg_mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}

#endif  // MG_ARCH == MG_ARCH_FREERTOS


#if MG_ARCH == MG_ARCH_NEWLIB
#define _POSIX_TIMERS

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MG_PATH_MAX 100
#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_DIRLIST 0

#endif


#if MG_ARCH == MG_ARCH_RP2040
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pico/stdlib.h>
int mkdir(const char *, mode_t);
#endif


#if MG_ARCH == MG_ARCH_RTTHREAD

#include <rtthread.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 1460
#endif

#endif // MG_ARCH == MG_ARCH_RTTHREAD


#if MG_ARCH == MG_ARCH_ARMCC || MG_ARCH == MG_ARCH_CMSIS_RTOS1 || \
    MG_ARCH == MG_ARCH_CMSIS_RTOS2

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>
#include <string.h>
#include <time.h>
#if MG_ARCH == MG_ARCH_CMSIS_RTOS1
#include "cmsis_os.h"  // keep this include
// https://developer.arm.com/documentation/ka003821/latest
extern uint32_t rt_time_get(void);
#elif MG_ARCH == MG_ARCH_CMSIS_RTOS2
#include "cmsis_os2.h"  // keep this include
#endif

#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)

#if defined(__ARMCC_VERSION)
#define mode_t size_t
#define mkdir(a, b) mg_mkdir(a, b)
static inline int mg_mkdir(const char *path, mode_t mode) {
  (void) path, (void) mode;
  return -1;
}
#endif

#if (MG_ARCH == MG_ARCH_CMSIS_RTOS1 || MG_ARCH == MG_ARCH_CMSIS_RTOS2) &&     \
    !defined MG_ENABLE_RL && (!defined(MG_ENABLE_LWIP) || !MG_ENABLE_LWIP) && \
    (!defined(MG_ENABLE_TCPIP) || !MG_ENABLE_TCPIP)
#define MG_ENABLE_RL 1
#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif
#endif

#endif


#if MG_ARCH == MG_ARCH_TIRTOS

#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <serrno.h>
#include <sys/socket.h>

#include <ti/sysbios/knl/Clock.h>

#endif


#if MG_ARCH == MG_ARCH_UNIX

#define _DARWIN_UNLIMITED_SELECT 1  // No limit on file descriptors

#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if !defined(MG_ENABLE_EPOLL) && defined(__linux__)
#define MG_ENABLE_EPOLL 1
#elif !defined(MG_ENABLE_POLL)
#define MG_ENABLE_POLL 1
#endif

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(MG_ENABLE_EPOLL) && MG_ENABLE_EPOLL
#include <sys/epoll.h>
#elif defined(MG_ENABLE_POLL) && MG_ENABLE_POLL
#include <poll.h>
#else
#include <sys/select.h>
#endif

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#endif


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
#include <direct.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
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

#include <process.h>
#include <winerror.h>
#include <winsock2.h>

// Protect from calls like std::snprintf in app code
// See https://github.com/cesanta/mongoose/issues/1047
#ifndef __cplusplus
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#ifndef strdup  // For MSVC with _DEBUG, see #1359
#define strdup(x) _strdup(x)
#endif
#endif

#define MG_INVALID_SOCKET INVALID_SOCKET
#define MG_SOCKET_TYPE SOCKET
typedef unsigned long nfds_t;
#if defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#ifndef alloca
#define alloca(a) _alloca(a)
#endif
#endif
#define poll(a, b, c) WSAPoll((a), (b), (c))
#define closesocket(x) closesocket(x)

typedef int socklen_t;
#define MG_DIRSEP '\\'

#ifndef MG_PATH_MAX
#define MG_PATH_MAX FILENAME_MAX
#endif

#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? WSAGetLastError() : 0)

#define MG_SOCK_PENDING(errcode)                                            \
  (((errcode) < 0) &&                                                       \
   (WSAGetLastError() == WSAEINTR || WSAGetLastError() == WSAEINPROGRESS || \
    WSAGetLastError() == WSAEWOULDBLOCK))

#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (WSAGetLastError() == WSAECONNRESET))

#define realpath(a, b) _fullpath((b), (a), MG_PATH_MAX)
#define sleep(x) Sleep((x) *1000)
#define mkdir(a, b) _mkdir(a)
#define timegm(x) _mkgmtime(x)

#ifndef S_ISDIR
#define S_ISDIR(x) (((x) &_S_IFMT) == _S_IFDIR)
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 1
#endif

#ifndef SIGPIPE
#define SIGPIPE 0
#endif

#endif


#if MG_ARCH == MG_ARCH_ZEPHYR

#include <zephyr/kernel.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <zephyr/net/socket.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MG_PUTCHAR(x) printk("%c", x)
#ifndef strdup
#define strdup(s) ((char *) mg_strdup(mg_str(s)).ptr)
#endif
#define strerror(x) zsock_gai_strerror(x)

#ifndef FD_CLOEXEC
#define FD_CLOEXEC 0
#endif

#ifndef F_SETFD
#define F_SETFD 0
#endif

#define MG_ENABLE_SSI 0

int rand(void);
int sscanf(const char *, const char *, ...);

#endif


#if defined(MG_ENABLE_FREERTOS_TCP) && MG_ENABLE_FREERTOS_TCP

#include <limits.h>
#include <list.h>

#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <FreeRTOS_errno_TCP.h>  // contents to be moved and file removed, some day

#define MG_SOCKET_TYPE Socket_t
#define MG_INVALID_SOCKET FREERTOS_INVALID_SOCKET

// Why FreeRTOS-TCP did not implement a clean BSD API, but its own thing
// with FreeRTOS_ prefix, is beyond me
#define IPPROTO_TCP FREERTOS_IPPROTO_TCP
#define IPPROTO_UDP FREERTOS_IPPROTO_UDP
#define AF_INET FREERTOS_AF_INET
#define SOCK_STREAM FREERTOS_SOCK_STREAM
#define SOCK_DGRAM FREERTOS_SOCK_DGRAM
#define SO_BROADCAST 0
#define SO_ERROR 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? (errcode) : 0)

#define MG_SOCK_PENDING(errcode)                 \
  ((errcode) == -pdFREERTOS_ERRNO_EWOULDBLOCK || \
   (errcode) == -pdFREERTOS_ERRNO_EISCONN ||     \
   (errcode) == -pdFREERTOS_ERRNO_EINPROGRESS || \
   (errcode) == -pdFREERTOS_ERRNO_EAGAIN)

#define MG_SOCK_RESET(errcode) ((errcode) == -pdFREERTOS_ERRNO_ENOTCONN)

// actually only if optional timeout is enabled
#define MG_SOCK_INTR(fd) (fd == NULL)

#define sockaddr_in freertos_sockaddr
#define sockaddr freertos_sockaddr
#define accept(a, b, c) FreeRTOS_accept((a), (b), (c))
#define connect(a, b, c) FreeRTOS_connect((a), (b), (c))
#define bind(a, b, c) FreeRTOS_bind((a), (b), (c))
#define listen(a, b) FreeRTOS_listen((a), (b))
#define socket(a, b, c) FreeRTOS_socket((a), (b), (c))
#define send(a, b, c, d) FreeRTOS_send((a), (b), (c), (d))
#define recv(a, b, c, d) FreeRTOS_recv((a), (b), (c), (d))
#define setsockopt(a, b, c, d, e) FreeRTOS_setsockopt((a), (b), (c), (d), (e))
#define sendto(a, b, c, d, e, f) FreeRTOS_sendto((a), (b), (c), (d), (e), (f))
#define recvfrom(a, b, c, d, e, f) \
  FreeRTOS_recvfrom((a), (b), (c), (d), (e), (f))
#define closesocket(x) FreeRTOS_closesocket(x)
#define gethostbyname(x) FreeRTOS_gethostbyname(x)
#define getsockname(a, b, c) mg_getsockname((a), (b), (c))
#define getpeername(a, b, c) mg_getpeername((a), (b), (c))

static inline int mg_getsockname(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return -1;
}

static inline int mg_getpeername(MG_SOCKET_TYPE fd, void *buf, socklen_t *len) {
  (void) fd, (void) buf, (void) len;
  return 0;
}
#endif


#if defined(MG_ENABLE_LWIP) && MG_ENABLE_LWIP

#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#include <sys/stat.h>
#endif

struct timeval;

#include <lwip/sockets.h>

#if !LWIP_TIMEVAL_PRIVATE
#if defined(__GNUC__) && !defined(__ARMCC_VERSION) // armclang sets both
#include <sys/time.h>
#else
struct timeval {
  time_t tv_sec;
  long tv_usec;
};
#endif
#endif

#if LWIP_SOCKET != 1
// Sockets support disabled in LWIP by default
#error Set LWIP_SOCKET variable to 1 (in lwipopts.h)
#endif
#endif


#if defined(MG_ENABLE_RL) && MG_ENABLE_RL
#include <rl_net.h>

#define closesocket(x) closesocket(x)

#define TCP_NODELAY SO_KEEPALIVE

#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? (errcode) : 0)

#define MG_SOCK_PENDING(errcode)                                \
  ((errcode) == BSD_EWOULDBLOCK || (errcode) == BSD_EALREADY || \
   (errcode) == BSD_EINPROGRESS)

#define MG_SOCK_RESET(errcode) \
  ((errcode) == BSD_ECONNABORTED || (errcode) == BSD_ECONNRESET)

// In blocking mode, which is enabled by default, accept() waits for a
// connection request. In non blocking mode, you must call accept()
// again if the error code BSD_EWOULDBLOCK is returned.
#define MG_SOCK_INTR(fd) (fd == BSD_EWOULDBLOCK)

#define socklen_t int
#endif


#ifndef MG_ENABLE_LOG
#define MG_ENABLE_LOG 1
#endif

#ifndef MG_ENABLE_CUSTOM_LOG
#define MG_ENABLE_CUSTOM_LOG 0  // Let user define their own MG_LOG
#endif

#ifndef MG_ENABLE_TCPIP
#define MG_ENABLE_TCPIP 0  // Mongoose built-in network stack
#endif

#ifndef MG_ENABLE_LWIP
#define MG_ENABLE_LWIP 0  // lWIP network stack
#endif

#ifndef MG_ENABLE_FREERTOS_TCP
#define MG_ENABLE_FREERTOS_TCP 0  // Amazon FreeRTOS-TCP network stack
#endif

#ifndef MG_ENABLE_RL
#define MG_ENABLE_RL 0  // ARM MDK network stack
#endif

#ifndef MG_ENABLE_SOCKET
#define MG_ENABLE_SOCKET !MG_ENABLE_TCPIP
#endif

#ifndef MG_ENABLE_POLL
#define MG_ENABLE_POLL 0
#endif

#ifndef MG_ENABLE_EPOLL
#define MG_ENABLE_EPOLL 0
#endif

#ifndef MG_ENABLE_FATFS
#define MG_ENABLE_FATFS 0
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_IPV6_V6ONLY
#define MG_IPV6_V6ONLY 0  // IPv6 socket binds only to V6, not V4 address
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 1
#endif

// Set MG_ENABLE_WINSOCK=0 for Win32 builds with external IP stack (like LWIP)
#ifndef MG_ENABLE_WINSOCK
#define MG_ENABLE_WINSOCK 1
#endif

#ifndef MG_ENABLE_DIRLIST
#define MG_ENABLE_DIRLIST 0
#endif

#ifndef MG_ENABLE_CUSTOM_RANDOM
#define MG_ENABLE_CUSTOM_RANDOM 0
#endif

#ifndef MG_ENABLE_CUSTOM_MILLIS
#define MG_ENABLE_CUSTOM_MILLIS 0
#endif

#ifndef MG_ENABLE_PACKED_FS
#define MG_ENABLE_PACKED_FS 0
#endif

#ifndef MG_ENABLE_ASSERT
#define MG_ENABLE_ASSERT 0
#endif

#ifndef MG_IO_SIZE
#define MG_IO_SIZE 2048  // Granularity of the send/recv IO buffer growth
#endif

#ifndef MG_MAX_RECV_SIZE
#define MG_MAX_RECV_SIZE (3UL * 1024UL * 1024UL)  // Maximum recv IO buffer size
#endif

#ifndef MG_DATA_SIZE
#define MG_DATA_SIZE 32  // struct mg_connection :: data size
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 30
#endif

#ifndef MG_HTTP_INDEX
#define MG_HTTP_INDEX "index.html"
#endif

#ifndef MG_PATH_MAX
#ifdef PATH_MAX
#define MG_PATH_MAX PATH_MAX
#else
#define MG_PATH_MAX 128
#endif
#endif

#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 128
#endif

#ifndef MG_DIRSEP
#define MG_DIRSEP '/'
#endif

#ifndef MG_ENABLE_POSIX_FS
#if defined(FOPEN_MAX)
#define MG_ENABLE_POSIX_FS 1
#else
#define MG_ENABLE_POSIX_FS 0
#endif
#endif

#ifndef MG_INVALID_SOCKET
#define MG_INVALID_SOCKET (-1)
#endif

#ifndef MG_SOCKET_TYPE
#define MG_SOCKET_TYPE int
#endif

#ifndef MG_SOCKET_ERRNO
#define MG_SOCKET_ERRNO errno
#endif

#if MG_ENABLE_EPOLL
#define MG_EPOLL_ADD(c)                                                    \
  do {                                                                     \
    struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};          \
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_ADD, (int) (size_t) c->fd, &ev); \
  } while (0)
#define MG_EPOLL_MOD(c, wr)                                                \
  do {                                                                     \
    struct epoll_event ev = {EPOLLIN | EPOLLERR | EPOLLHUP, {c}};          \
    if (wr) ev.events |= EPOLLOUT;                                         \
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_MOD, (int) (size_t) c->fd, &ev); \
  } while (0)
#else
#define MG_EPOLL_ADD(c)
#define MG_EPOLL_MOD(c, wr)
#endif

#ifndef MG_ENABLE_PROFILE
#define MG_ENABLE_PROFILE 0
#endif




struct mg_str {
  const char *ptr;  // Pointer to string data
  size_t len;       // String len
};

#define MG_C_STR(a) \
  { (a), sizeof(a) - 1 }

// Using macro to avoid shadowing C++ struct constructor, see #1298
#define mg_str(s) mg_str_s(s)

struct mg_str mg_str(const char *s);
struct mg_str mg_str_n(const char *s, size_t n);
int mg_lower(const char *s);
int mg_ncasecmp(const char *s1, const char *s2, size_t len);
int mg_casecmp(const char *s1, const char *s2);
int mg_vcmp(const struct mg_str *s1, const char *s2);
int mg_vcasecmp(const struct mg_str *str1, const char *str2);
int mg_strcmp(const struct mg_str str1, const struct mg_str str2);
struct mg_str mg_strstrip(struct mg_str s);
struct mg_str mg_strdup(const struct mg_str s);
const char *mg_strstr(const struct mg_str haystack, const struct mg_str needle);
bool mg_match(struct mg_str str, struct mg_str pattern, struct mg_str *caps);
bool mg_globmatch(const char *pattern, size_t plen, const char *s, size_t n);
bool mg_span(struct mg_str s, struct mg_str *a, struct mg_str *b, char delim);
char *mg_hex(const void *buf, size_t len, char *dst);
void mg_unhex(const char *buf, size_t len, unsigned char *to);
unsigned long mg_unhexn(const char *s, size_t len);
bool mg_path_is_sane(const char *path);




// Single producer, single consumer non-blocking queue

struct mg_queue {
  char *buf;
  size_t size;
  volatile size_t tail;
  volatile size_t head;
};

void mg_queue_init(struct mg_queue *, char *, size_t);        // Init queue
size_t mg_queue_book(struct mg_queue *, char **buf, size_t);  // Reserve space
void mg_queue_add(struct mg_queue *, size_t);                 // Add new message
size_t mg_queue_next(struct mg_queue *, char **);  // Get oldest message
void mg_queue_del(struct mg_queue *, size_t);      // Delete oldest message




typedef void (*mg_pfn_t)(char, void *);                  // Output function
typedef size_t (*mg_pm_t)(mg_pfn_t, void *, va_list *);  // %M printer

size_t mg_vxprintf(void (*)(char, void *), void *, const char *fmt, va_list *);
size_t mg_xprintf(void (*fn)(char, void *), void *, const char *fmt, ...);






// Convenience wrappers around mg_xprintf
size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap);
size_t mg_snprintf(char *, size_t, const char *fmt, ...);
char *mg_vmprintf(const char *fmt, va_list *ap);
char *mg_mprintf(const char *fmt, ...);
size_t mg_queue_vprintf(struct mg_queue *, const char *fmt, va_list *);
size_t mg_queue_printf(struct mg_queue *, const char *fmt, ...);

// %M print helper functions
size_t mg_print_base64(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_esc(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_hex(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip_port(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip4(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_ip6(void (*out)(char, void *), void *arg, va_list *ap);
size_t mg_print_mac(void (*out)(char, void *), void *arg, va_list *ap);

// Various output functions
void mg_pfn_iobuf(char ch, void *param);  // param: struct mg_iobuf *
void mg_pfn_stdout(char c, void *param);  // param: ignored

// A helper macro for printing JSON: mg_snprintf(buf, len, "%m", MG_ESC("hi"))
#define MG_ESC(str) mg_print_esc, 0, (str)






enum { MG_LL_NONE, MG_LL_ERROR, MG_LL_INFO, MG_LL_DEBUG, MG_LL_VERBOSE };
extern int mg_log_level;  // Current log level, one of MG_LL_*

void mg_log(const char *fmt, ...);
void mg_log_prefix(int ll, const char *file, int line, const char *fname);
// bool mg_log2(int ll, const char *file, int line, const char *fmt, ...);
void mg_hexdump(const void *buf, size_t len);
void mg_log_set_fn(mg_pfn_t fn, void *param);

#define mg_log_set(level_) mg_log_level = (level_)

#if MG_ENABLE_LOG
#define MG_LOG(level, args)                                 \
  do {                                                      \
    if ((level) <= mg_log_level) {                          \
      mg_log_prefix((level), __FILE__, __LINE__, __func__); \
      mg_log args;                                          \
    }                                                       \
  } while (0)
#else
#define MG_LOG(level, args) \
  do {                      \
    if (0) mg_log args;     \
  } while (0)
#endif

#define MG_ERROR(args) MG_LOG(MG_LL_ERROR, args)
#define MG_INFO(args) MG_LOG(MG_LL_INFO, args)
#define MG_DEBUG(args) MG_LOG(MG_LL_DEBUG, args)
#define MG_VERBOSE(args) MG_LOG(MG_LL_VERBOSE, args)




struct mg_timer {
  unsigned long id;         // Timer ID
  uint64_t period_ms;       // Timer period in milliseconds
  uint64_t expire;          // Expiration timestamp in milliseconds
  unsigned flags;           // Possible flags values below
#define MG_TIMER_ONCE 0     // Call function once
#define MG_TIMER_REPEAT 1   // Call function periodically
#define MG_TIMER_RUN_NOW 2  // Call immediately when timer is set
  void (*fn)(void *);       // Function to call
  void *arg;                // Function argument
  struct mg_timer *next;    // Linkage
};

void mg_timer_init(struct mg_timer **head, struct mg_timer *timer,
                   uint64_t milliseconds, unsigned flags, void (*fn)(void *),
                   void *arg);
void mg_timer_free(struct mg_timer **head, struct mg_timer *);
void mg_timer_poll(struct mg_timer **head, uint64_t new_ms);
bool mg_timer_expired(uint64_t *expiration, uint64_t period, uint64_t now);





enum { MG_FS_READ = 1, MG_FS_WRITE = 2, MG_FS_DIR = 4 };

// Filesystem API functions
// st() returns MG_FS_* flags and populates file size and modification time
// ls() calls fn() for every directory entry, allowing to list a directory
//
// NOTE: UNIX-style shorthand names for the API functions are deliberately
// chosen to avoid conflicts with some libraries that make macros for e.g.
// stat(), write(), read() calls.
struct mg_fs {
  int (*st)(const char *path, size_t *size, time_t *mtime);  // stat file
  void (*ls)(const char *path, void (*fn)(const char *, void *),
             void *);  // List directory entries: call fn(file_name, fn_data)
                       // for each directory entry
  void *(*op)(const char *path, int flags);             // Open file
  void (*cl)(void *fd);                                 // Close file
  size_t (*rd)(void *fd, void *buf, size_t len);        // Read file
  size_t (*wr)(void *fd, const void *buf, size_t len);  // Write file
  size_t (*sk)(void *fd, size_t offset);                // Set file position
  bool (*mv)(const char *from, const char *to);         // Rename file
  bool (*rm)(const char *path);                         // Delete file
  bool (*mkd)(const char *path);                        // Create directory
};

extern struct mg_fs mg_fs_posix;   // POSIX open/close/read/write/seek
extern struct mg_fs mg_fs_packed;  // Packed FS, see examples/device-dashboard
extern struct mg_fs mg_fs_fat;     // FAT FS

// File descriptor
struct mg_fd {
  void *fd;
  struct mg_fs *fs;
};

struct mg_fd *mg_fs_open(struct mg_fs *fs, const char *path, int flags);
void mg_fs_close(struct mg_fd *fd);
bool mg_fs_ls(struct mg_fs *fs, const char *path, char *buf, size_t len);
struct mg_str mg_file_read(struct mg_fs *fs, const char *path);
bool mg_file_write(struct mg_fs *fs, const char *path, const void *, size_t);
bool mg_file_printf(struct mg_fs *fs, const char *path, const char *fmt, ...);

// Packed API
const char *mg_unpack(const char *path, size_t *size, time_t *mtime);
const char *mg_unlist(size_t no);             // Get no'th packed filename
struct mg_str mg_unpacked(const char *path);  // Packed file as mg_str







#if MG_ENABLE_ASSERT
#include <assert.h>
#elif !defined(assert)
#define assert(x)
#endif

void mg_bzero(volatile unsigned char *buf, size_t len);
void mg_random(void *buf, size_t len);
char *mg_random_str(char *buf, size_t len);
uint16_t mg_ntohs(uint16_t net);
uint32_t mg_ntohl(uint32_t net);
uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len);
uint64_t mg_millis(void);  // Return milliseconds since boot
uint64_t mg_now(void);     // Return milliseconds since Epoch

#define mg_htons(x) mg_ntohs(x)
#define mg_htonl(x) mg_ntohl(x)

#define MG_U32(a, b, c, d)                                         \
  (((uint32_t) ((a) &255) << 24) | ((uint32_t) ((b) &255) << 16) | \
   ((uint32_t) ((c) &255) << 8) | (uint32_t) ((d) &255))

// For printing IPv4 addresses: printf("%d.%d.%d.%d\n", MG_IPADDR_PARTS(&ip))
#define MG_U8P(ADDR) ((uint8_t *) (ADDR))
#define MG_IPADDR_PARTS(ADDR) \
  MG_U8P(ADDR)[0], MG_U8P(ADDR)[1], MG_U8P(ADDR)[2], MG_U8P(ADDR)[3]

#define MG_REG(x) ((volatile uint32_t *) (x))[0]
#define MG_BIT(x) (((uint32_t) 1U) << (x))
#define MG_SET_BITS(R, CLRMASK, SETMASK) (R) = ((R) & ~(CLRMASK)) | (SETMASK)

#define MG_ROUND_UP(x, a) ((a) == 0 ? (x) : ((((x) + (a) -1) / (a)) * (a)))
#define MG_ROUND_DOWN(x, a) ((a) == 0 ? (x) : (((x) / (a)) * (a)))

#ifdef __GNUC__
#define MG_ARM_DISABLE_IRQ() asm volatile("cpsid i" : : : "memory")
#define MG_ARM_ENABLE_IRQ() asm volatile("cpsie i" : : : "memory")
#else
#define MG_ARM_DISABLE_IRQ()
#define MG_ARM_ENABLE_IRQ()
#endif

#if defined(__CC_ARM)
#define MG_DSB() __dsb(0xf)
#elif defined(__ARMCC_VERSION)
#define MG_DSB() __builtin_arm_dsb(0xf)
#elif defined(__GNUC__) && defined(__arm__) && defined(__thumb__)
#define MG_DSB() asm("DSB 0xf")
#elif defined(__ICCARM__)
#define MG_DSB() __iar_builtin_DSB()
#else
#define MG_DSB()
#endif

struct mg_addr;
int mg_check_ip_acl(struct mg_str acl, struct mg_addr *remote_ip);

// Linked list management macros
#define LIST_ADD_HEAD(type_, head_, elem_) \
  do {                                     \
    (elem_)->next = (*head_);              \
    *(head_) = (elem_);                    \
  } while (0)

#define LIST_ADD_TAIL(type_, head_, elem_) \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != NULL) h = &(*h)->next;    \
    *h = (elem_);                          \
  } while (0)

#define LIST_DELETE(type_, head_, elem_)   \
  do {                                     \
    type_ **h = head_;                     \
    while (*h != (elem_)) h = &(*h)->next; \
    *h = (elem_)->next;                    \
  } while (0)



unsigned short mg_url_port(const char *url);
int mg_url_is_ssl(const char *url);
struct mg_str mg_url_host(const char *url);
struct mg_str mg_url_user(const char *url);
struct mg_str mg_url_pass(const char *url);
const char *mg_url_uri(const char *url);




struct mg_iobuf {
  unsigned char *buf;  // Pointer to stored data
  size_t size;         // Total size available
  size_t len;          // Current number of bytes
  size_t align;        // Alignment during allocation
};

int mg_iobuf_init(struct mg_iobuf *, size_t, size_t);
int mg_iobuf_resize(struct mg_iobuf *, size_t);
void mg_iobuf_free(struct mg_iobuf *);
size_t mg_iobuf_add(struct mg_iobuf *, size_t, const void *, size_t);
size_t mg_iobuf_del(struct mg_iobuf *, size_t ofs, size_t len);


size_t mg_base64_update(unsigned char input_byte, char *buf, size_t len);
size_t mg_base64_final(char *buf, size_t len);
size_t mg_base64_encode(const unsigned char *p, size_t n, char *buf, size_t);
size_t mg_base64_decode(const char *src, size_t n, char *dst, size_t);




typedef struct {
  uint32_t buf[4];
  uint32_t bits[2];
  unsigned char in[64];
} mg_md5_ctx;

void mg_md5_init(mg_md5_ctx *c);
void mg_md5_update(mg_md5_ctx *c, const unsigned char *data, size_t len);
void mg_md5_final(mg_md5_ctx *c, unsigned char[16]);




typedef struct {
  uint32_t state[5];
  uint32_t count[2];
  unsigned char buffer[64];
} mg_sha1_ctx;

void mg_sha1_init(mg_sha1_ctx *);
void mg_sha1_update(mg_sha1_ctx *, const unsigned char *data, size_t len);
void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *);




typedef struct {
  uint32_t state[8];
  uint64_t bits;
  uint32_t len;
  unsigned char buffer[64];
} mg_sha256_ctx;

void mg_sha256_init(mg_sha256_ctx *);
void mg_sha256_update(mg_sha256_ctx *, const unsigned char *data, size_t len);
void mg_sha256_final(unsigned char digest[32], mg_sha256_ctx *);
void mg_hmac_sha256(uint8_t dst[32], uint8_t *key, size_t keysz, uint8_t *data,
                    size_t datasz);
/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of the AES Rijndael
 * 128-bit block cipher designed by Vincent Rijmen and Joan Daemen. The focus
 * of this work was correctness & accuracy.  It is written in 'C' without any
 * particular focus upon optimization or speed. It should be endian (memory
 * byte order) neutral since the few places that care are handled explicitly.
 *
 * This implementation of Rijndael was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/archive/aes/rijndael/wsdindex.html
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/

#ifndef AES_HEADER
#define AES_HEADER

/******************************************************************************/
#define AES_DECRYPTION 1  // whether AES decryption is supported
/******************************************************************************/

#define ENCRYPT 1  // specify whether we're encrypting
#define DECRYPT 0  // or decrypting



typedef unsigned char uchar;  // add some convienent shorter types
typedef unsigned int uint;

/******************************************************************************
 *  AES_INIT_KEYGEN_TABLES : MUST be called once before any AES use
 ******************************************************************************/
void aes_init_keygen_tables(void);

/******************************************************************************
 *  AES_CONTEXT : cipher context / holds inter-call data
 ******************************************************************************/
typedef struct {
  int mode;          // 1 for Encryption, 0 for Decryption
  int rounds;        // keysize-based rounds count
  uint32_t *rk;      // pointer to current round key
  uint32_t buf[68];  // key expansion buffer
} aes_context;

/******************************************************************************
 *  AES_SETKEY : called to expand the key for encryption or decryption
 ******************************************************************************/
int aes_setkey(aes_context *ctx,  // pointer to context
               int mode,          // 1 or 0 for Encrypt/Decrypt
               const uchar *key,  // AES input key
               uint keysize);     // size in bytes (must be 16, 24, 32 for
                                  // 128, 192 or 256-bit keys respectively)
                                  // returns 0 for success

/******************************************************************************
 *  AES_CIPHER : called to encrypt or decrypt ONE 128-bit block of data
 ******************************************************************************/
int aes_cipher(aes_context *ctx,       // pointer to context
               const uchar input[16],  // 128-bit block to en/decipher
               uchar output[16]);      // 128-bit output result block
                                       // returns 0 for success

#endif /* AES_HEADER */
/******************************************************************************
 *
 * THIS SOURCE CODE IS HEREBY PLACED INTO THE PUBLIC DOMAIN FOR THE GOOD OF ALL
 *
 * This is a simple and straightforward implementation of AES-GCM authenticated
 * encryption. The focus of this work was correctness & accuracy. It is written
 * in straight 'C' without any particular focus upon optimization or speed. It
 * should be endian (memory byte order) neutral since the few places that care
 * are handled explicitly.
 *
 * This implementation of AES-GCM was created by Steven M. Gibson of GRC.com.
 *
 * It is intended for general purpose use, but was written in support of GRC's
 * reference implementation of the SQRL (Secure Quick Reliable Login) client.
 *
 * See:    http://csrc.nist.gov/publications/nistpubs/800-38D/SP-800-38D.pdf
 *         http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/ \
 *         gcm/gcm-revised-spec.pdf
 *
 * NO COPYRIGHT IS CLAIMED IN THIS WORK, HOWEVER, NEITHER IS ANY WARRANTY MADE
 * REGARDING ITS FITNESS FOR ANY PARTICULAR PURPOSE. USE IT AT YOUR OWN RISK.
 *
 *******************************************************************************/
#ifndef GCM_HEADER
#define GCM_HEADER


#define GCM_AUTH_FAILURE 0x55555555  // authentication failure

/******************************************************************************
 *  GCM_CONTEXT : GCM context / holds keytables, instance data, and AES ctx
 ******************************************************************************/
typedef struct {
  int mode;             // cipher direction: encrypt/decrypt
  uint64_t len;         // cipher data length processed so far
  uint64_t add_len;     // total add data length
  uint64_t HL[16];      // precalculated lo-half HTable
  uint64_t HH[16];      // precalculated hi-half HTable
  uchar base_ectr[16];  // first counter-mode cipher output for tag
  uchar y[16];          // the current cipher-input IV|Counter value
  uchar buf[16];        // buf working value
  aes_context aes_ctx;  // cipher context used
} gcm_context;

/******************************************************************************
 *  GCM_CONTEXT : MUST be called once before ANY use of this library
 ******************************************************************************/
int gcm_initialize(void);

/******************************************************************************
 *  GCM_SETKEY : sets the GCM (and AES) keying material for use
 ******************************************************************************/
int gcm_setkey(gcm_context *ctx,   // caller-provided context ptr
               const uchar *key,   // pointer to cipher key
               const uint keysize  // size in bytes (must be 16, 24, 32 for
                                   // 128, 192 or 256-bit keys respectively)
);                                 // returns 0 for success

/******************************************************************************
 *
 *  GCM_CRYPT_AND_TAG
 *
 *  This either encrypts or decrypts the user-provided data and, either
 *  way, generates an authentication tag of the requested length. It must be
 *  called with a GCM context whose key has already been set with GCM_SETKEY.
 *
 *  The user would typically call this explicitly to ENCRYPT a buffer of data
 *  and optional associated data, and produce its an authentication tag.
 *
 *  To reverse the process the user would typically call the companion
 *  GCM_AUTH_DECRYPT function to decrypt data and verify a user-provided
 *  authentication tag.  The GCM_AUTH_DECRYPT function calls this function
 *  to perform its decryption and tag generation, which it then compares.
 *
 ******************************************************************************/
int gcm_crypt_and_tag(
    gcm_context *ctx,    // gcm context with key already setup
    int mode,            // cipher direction: ENCRYPT (1) or DECRYPT (0)
    const uchar *iv,     // pointer to the 12-byte initialization vector
    size_t iv_len,       // byte length if the IV. should always be 12
    const uchar *add,    // pointer to the non-ciphered additional data
    size_t add_len,      // byte length of the additional AEAD data
    const uchar *input,  // pointer to the cipher data source
    uchar *output,       // pointer to the cipher data destination
    size_t length,       // byte length of the cipher data
    uchar *tag,          // pointer to the tag to be generated
    size_t tag_len);     // byte length of the tag to be generated

/******************************************************************************
 *
 *  GCM_AUTH_DECRYPT
 *
 *  This DECRYPTS a user-provided data buffer with optional associated data.
 *  It then verifies a user-supplied authentication tag against the tag just
 *  re-created during decryption to verify that the data has not been altered.
 *
 *  This function calls GCM_CRYPT_AND_TAG (above) to perform the decryption
 *  and authentication tag generation.
 *
 ******************************************************************************/
int gcm_auth_decrypt(
    gcm_context *ctx,    // gcm context with key already setup
    const uchar *iv,     // pointer to the 12-byte initialization vector
    size_t iv_len,       // byte length if the IV. should always be 12
    const uchar *add,    // pointer to the non-ciphered additional data
    size_t add_len,      // byte length of the additional AEAD data
    const uchar *input,  // pointer to the cipher data source
    uchar *output,       // pointer to the cipher data destination
    size_t length,       // byte length of the cipher data
    const uchar *tag,    // pointer to the tag to be authenticated
    size_t tag_len);     // byte length of the tag <= 16

/******************************************************************************
 *
 *  GCM_START
 *
 *  Given a user-provided GCM context, this initializes it, sets the encryption
 *  mode, and preprocesses the initialization vector and additional AEAD data.
 *
 ******************************************************************************/
int gcm_start(
    gcm_context *ctx,  // pointer to user-provided GCM context
    int mode,          // ENCRYPT (1) or DECRYPT (0)
    const uchar *iv,   // pointer to initialization vector
    size_t iv_len,     // IV length in bytes (should == 12)
    const uchar *add,  // pointer to additional AEAD data (NULL if none)
    size_t add_len);   // length of additional AEAD data (bytes)

/******************************************************************************
 *
 *  GCM_UPDATE
 *
 *  This is called once or more to process bulk plaintext or ciphertext data.
 *  We give this some number of bytes of input and it returns the same number
 *  of output bytes. If called multiple times (which is fine) all but the final
 *  invocation MUST be called with length mod 16 == 0. (Only the final call can
 *  have a partial block length of < 128 bits.)
 *
 ******************************************************************************/
int gcm_update(gcm_context *ctx,    // pointer to user-provided GCM context
               size_t length,       // length, in bytes, of data to process
               const uchar *input,  // pointer to source data
               uchar *output);      // pointer to destination data

/******************************************************************************
 *
 *  GCM_FINISH
 *
 *  This is called once after all calls to GCM_UPDATE to finalize the GCM.
 *  It performs the final GHASH to produce the resulting authentication TAG.
 *
 ******************************************************************************/
int gcm_finish(gcm_context *ctx,  // pointer to user-provided GCM context
               uchar *tag,        // ptr to tag buffer - NULL if tag_len = 0
               size_t tag_len);   // length, in bytes, of the tag-receiving buf

/******************************************************************************
 *
 *  GCM_ZERO_CTX
 *
 *  The GCM context contains both the GCM context and the AES context.
 *  This includes keying and key-related material which is security-
 *  sensitive, so it MUST be zeroed after use. This function does that.
 *
 ******************************************************************************/
void gcm_zero_ctx(gcm_context *ctx);

#endif /* GCM_HEADER */
//
//  aes-gcm.h
//  MKo
//
//  Created by Markus Kosmal on 20/11/14.
//
//

#ifndef mko_aes_gcm_h
#define mko_aes_gcm_h

int aes_gcm_encrypt(unsigned char *output, const unsigned char *input,
                    size_t input_length, const unsigned char *key,
                    const size_t key_len, const unsigned char *iv,
                    const size_t iv_len, unsigned char *aead, size_t aead_len,
                    unsigned char *tag, const size_t tag_len);

int aes_gcm_decrypt(unsigned char *output, const unsigned char *input,
                    size_t input_length, const unsigned char *key,
                    const size_t key_len, const unsigned char *iv,
                    const size_t iv_len);

#endif

// End of aes128 PD



#define uECC_SUPPORTS_secp256r1 1
/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_H_
#define _UECC_H_

/* Platform selection options.
If uECC_PLATFORM is not defined, the code will try to guess it based on compiler
macros. Possible values for uECC_PLATFORM are defined below: */
#define uECC_arch_other 0
#define uECC_x86 1
#define uECC_x86_64 2
#define uECC_arm 3
#define uECC_arm_thumb 4
#define uECC_arm_thumb2 5
#define uECC_arm64 6
#define uECC_avr 7

/* If desired, you can define uECC_WORD_SIZE as appropriate for your platform
(1, 4, or 8 bytes). If uECC_WORD_SIZE is not explicitly defined then it will be
automatically set based on your platform. */

/* Optimization level; trade speed for code size.
   Larger values produce code that is faster but larger.
   Currently supported values are 0 - 4; 0 is unusably slow for most
   applications. Optimization level 4 currently only has an effect ARM platforms
   where more than one curve is enabled. */
#ifndef uECC_OPTIMIZATION_LEVEL
#define uECC_OPTIMIZATION_LEVEL 2
#endif

/* uECC_SQUARE_FUNC - If enabled (defined as nonzero), this will cause a
specific function to be used for (scalar) squaring instead of the generic
multiplication function. This can make things faster somewhat faster, but
increases the code size. */
#ifndef uECC_SQUARE_FUNC
#define uECC_SQUARE_FUNC 0
#endif

/* uECC_VLI_NATIVE_LITTLE_ENDIAN - If enabled (defined as nonzero), this will
switch to native little-endian format for *all* arrays passed in and out of the
public API. This includes public and private keys, shared secrets, signatures
and message hashes. Using this switch reduces the amount of call stack memory
used by uECC, since less intermediate translations are required. Note that this
will *only* work on native little-endian processors and it will treat the
uint8_t arrays passed into the public API as word arrays, therefore requiring
the provided byte arrays to be word aligned on architectures that do not support
unaligned accesses. IMPORTANT: Keys and signatures generated with
uECC_VLI_NATIVE_LITTLE_ENDIAN=1 are incompatible with keys and signatures
generated with uECC_VLI_NATIVE_LITTLE_ENDIAN=0; all parties must use the same
endianness. */
#ifndef uECC_VLI_NATIVE_LITTLE_ENDIAN
#define uECC_VLI_NATIVE_LITTLE_ENDIAN 0
#endif

/* Curve support selection. Set to 0 to remove that curve. */
#ifndef uECC_SUPPORTS_secp160r1
#define uECC_SUPPORTS_secp160r1 0
#endif
#ifndef uECC_SUPPORTS_secp192r1
#define uECC_SUPPORTS_secp192r1 0
#endif
#ifndef uECC_SUPPORTS_secp224r1
#define uECC_SUPPORTS_secp224r1 0
#endif
#ifndef uECC_SUPPORTS_secp256r1
#define uECC_SUPPORTS_secp256r1 1
#endif
#ifndef uECC_SUPPORTS_secp256k1
#define uECC_SUPPORTS_secp256k1 0
#endif

/* Specifies whether compressed point format is supported.
   Set to 0 to disable point compression/decompression functions. */
#ifndef uECC_SUPPORT_COMPRESSED_POINT
#define uECC_SUPPORT_COMPRESSED_POINT 1
#endif

struct uECC_Curve_t;
typedef const struct uECC_Curve_t *uECC_Curve;

#ifdef __cplusplus
extern "C" {
#endif

#if uECC_SUPPORTS_secp160r1
uECC_Curve uECC_secp160r1(void);
#endif
#if uECC_SUPPORTS_secp192r1
uECC_Curve uECC_secp192r1(void);
#endif
#if uECC_SUPPORTS_secp224r1
uECC_Curve uECC_secp224r1(void);
#endif
#if uECC_SUPPORTS_secp256r1
uECC_Curve uECC_secp256r1(void);
#endif
#if uECC_SUPPORTS_secp256k1
uECC_Curve uECC_secp256k1(void);
#endif

/* uECC_RNG_Function type
The RNG function should fill 'size' random bytes into 'dest'. It should return 1
if 'dest' was filled with random data, or 0 if the random data could not be
generated. The filled-in values should be either truly random, or from a
cryptographically-secure PRNG.

A correctly functioning RNG function must be set (using uECC_set_rng()) before
calling uECC_make_key() or uECC_sign().

Setting a correctly functioning RNG function improves the resistance to
side-channel attacks for uECC_shared_secret() and uECC_sign_deterministic().

A correct RNG function is set by default when building for Windows, Linux, or OS
X. If you are building on another POSIX-compliant system that supports
/dev/random or /dev/urandom, you can define uECC_POSIX to use the predefined
RNG. For embedded platforms there is no predefined RNG function; you must
provide your own.
*/
typedef int (*uECC_RNG_Function)(uint8_t *dest, unsigned size);

/* uECC_set_rng() function.
Set the function that will be used to generate random bytes. The RNG function
should return 1 if the random data was generated, or 0 if the random data could
not be generated.

On platforms where there is no predefined RNG function (eg embedded platforms),
this must be called before uECC_make_key() or uECC_sign() are used.

Inputs:
    rng_function - The function that will be used to generate random bytes.
*/
void uECC_set_rng(uECC_RNG_Function rng_function);

/* uECC_get_rng() function.

Returns the function that will be used to generate random bytes.
*/
uECC_RNG_Function uECC_get_rng(void);

/* uECC_curve_private_key_size() function.

Returns the size of a private key for the curve in bytes.
*/
int uECC_curve_private_key_size(uECC_Curve curve);

/* uECC_curve_public_key_size() function.

Returns the size of a public key for the curve in bytes.
*/
int uECC_curve_public_key_size(uECC_Curve curve);

/* uECC_make_key() function.
Create a public/private key pair.

Outputs:
    public_key  - Will be filled in with the public key. Must be at least 2 *
the curve size (in bytes) long. For example, if the curve is secp256r1,
public_key must be 64 bytes long. private_key - Will be filled in with the
private key. Must be as long as the curve order; this is typically the same as
the curve size, except for secp160r1. For example, if the curve is secp256r1,
private_key must be 32 bytes long.

                  For secp160r1, private_key must be 21 bytes long! Note that
the first byte will almost always be 0 (there is about a 1 in 2^80 chance of it
being non-zero).

Returns 1 if the key pair was generated successfully, 0 if an error occurred.
*/
int uECC_make_key(uint8_t *public_key, uint8_t *private_key, uECC_Curve curve);

/* uECC_shared_secret() function.
Compute a shared secret given your secret key and someone else's public key. If
the public key is not from a trusted source and has not been previously
verified, you should verify it first using uECC_valid_public_key(). Note: It is
recommended that you hash the result of uECC_shared_secret() before using it for
symmetric encryption or HMAC.

Inputs:
    public_key  - The public key of the remote party.
    private_key - Your private key.

Outputs:
    secret - Will be filled in with the shared secret value. Must be the same
size as the curve size; for example, if the curve is secp256r1, secret must be
32 bytes long.

Returns 1 if the shared secret was generated successfully, 0 if an error
occurred.
*/
int uECC_shared_secret(const uint8_t *public_key, const uint8_t *private_key,
                       uint8_t *secret, uECC_Curve curve);

#if uECC_SUPPORT_COMPRESSED_POINT
/* uECC_compress() function.
Compress a public key.

Inputs:
    public_key - The public key to compress.

Outputs:
    compressed - Will be filled in with the compressed public key. Must be at
least (curve size + 1) bytes long; for example, if the curve is secp256r1,
                 compressed must be 33 bytes long.
*/
void uECC_compress(const uint8_t *public_key, uint8_t *compressed,
                   uECC_Curve curve);

/* uECC_decompress() function.
Decompress a compressed public key.

Inputs:
    compressed - The compressed public key.

Outputs:
    public_key - Will be filled in with the decompressed public key.
*/
void uECC_decompress(const uint8_t *compressed, uint8_t *public_key,
                     uECC_Curve curve);
#endif /* uECC_SUPPORT_COMPRESSED_POINT */

/* uECC_valid_public_key() function.
Check to see if a public key is valid.

Note that you are not required to check for a valid public key before using any
other uECC functions. However, you may wish to avoid spending CPU time computing
a shared secret or verifying a signature using an invalid public key.

Inputs:
    public_key - The public key to check.

Returns 1 if the public key is valid, 0 if it is invalid.
*/
int uECC_valid_public_key(const uint8_t *public_key, uECC_Curve curve);

/* uECC_compute_public_key() function.
Compute the corresponding public key for a private key.

Inputs:
    private_key - The private key to compute the public key for

Outputs:
    public_key - Will be filled in with the corresponding public key

Returns 1 if the key was computed successfully, 0 if an error occurred.
*/
int uECC_compute_public_key(const uint8_t *private_key, uint8_t *public_key,
                            uECC_Curve curve);

/* uECC_sign() function.
Generate an ECDSA signature for a given hash value.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it in to this function along with your private key.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.

Outputs:
    signature - Will be filled in with the signature value. Must be at least 2 *
curve size long. For example, if the curve is secp256r1, signature must be 64
bytes long.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign(const uint8_t *private_key, const uint8_t *message_hash,
              unsigned hash_size, uint8_t *signature, uECC_Curve curve);

/* uECC_HashContext structure.
This is used to pass in an arbitrary hash function to uECC_sign_deterministic().
The structure will be used for multiple hash computations; each time a new hash
is computed, init_hash() will be called, followed by one or more calls to
update_hash(), and finally a call to finish_hash() to produce the resulting
hash.

The intention is that you will create a structure that includes uECC_HashContext
followed by any hash-specific data. For example:

typedef struct SHA256_HashContext {
    uECC_HashContext uECC;
    SHA256_CTX ctx;
} SHA256_HashContext;

void init_SHA256(uECC_HashContext *base) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Init(&context->ctx);
}

void update_SHA256(uECC_HashContext *base,
                   const uint8_t *message,
                   unsigned message_size) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Update(&context->ctx, message, message_size);
}

void finish_SHA256(uECC_HashContext *base, uint8_t *hash_result) {
    SHA256_HashContext *context = (SHA256_HashContext *)base;
    SHA256_Final(hash_result, &context->ctx);
}

... when signing ...
{
    uint8_t tmp[32 + 32 + 64];
    SHA256_HashContext ctx = {{&init_SHA256, &update_SHA256, &finish_SHA256, 64,
32, tmp}}; uECC_sign_deterministic(key, message_hash, &ctx.uECC, signature);
}
*/
typedef struct uECC_HashContext {
  void (*init_hash)(const struct uECC_HashContext *context);
  void (*update_hash)(const struct uECC_HashContext *context,
                      const uint8_t *message, unsigned message_size);
  void (*finish_hash)(const struct uECC_HashContext *context,
                      uint8_t *hash_result);
  unsigned
      block_size; /* Hash function block size in bytes, eg 64 for SHA-256. */
  unsigned
      result_size; /* Hash function result size in bytes, eg 32 for SHA-256. */
  uint8_t *tmp;    /* Must point to a buffer of at least (2 * result_size +
                      block_size) bytes. */
} uECC_HashContext;

/* uECC_sign_deterministic() function.
Generate an ECDSA signature for a given hash value, using a deterministic
algorithm (see RFC 6979). You do not need to set the RNG using uECC_set_rng()
before calling this function; however, if the RNG is defined it will improve
resistance to side-channel attacks.

Usage: Compute a hash of the data you wish to sign (SHA-2 is recommended) and
pass it to this function along with your private key and a hash context. Note
that the message_hash does not need to be computed with the same hash function
used by hash_context.

Inputs:
    private_key  - Your private key.
    message_hash - The hash of the message to sign.
    hash_size    - The size of message_hash in bytes.
    hash_context - A hash context to use.

Outputs:
    signature - Will be filled in with the signature value.

Returns 1 if the signature generated successfully, 0 if an error occurred.
*/
int uECC_sign_deterministic(const uint8_t *private_key,
                            const uint8_t *message_hash, unsigned hash_size,
                            const uECC_HashContext *hash_context,
                            uint8_t *signature, uECC_Curve curve);

/* uECC_verify() function.
Verify an ECDSA signature.

Usage: Compute the hash of the signed data using the same hash as the signer and
pass it to this function along with the signer's public key and the signature
values (r and s).

Inputs:
    public_key   - The signer's public key.
    message_hash - The hash of the signed data.
    hash_size    - The size of message_hash in bytes.
    signature    - The signature value.

Returns 1 if the signature is valid, 0 if it is invalid.
*/
int uECC_verify(const uint8_t *public_key, const uint8_t *message_hash,
                unsigned hash_size, const uint8_t *signature, uECC_Curve curve);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_VLI_H_
#define _UECC_VLI_H_

//
//

/* Functions for raw large-integer manipulation. These are only available
   if uECC.c is compiled with uECC_ENABLE_VLI_API defined to 1. */
#ifndef uECC_ENABLE_VLI_API
#define uECC_ENABLE_VLI_API 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if uECC_ENABLE_VLI_API

void uECC_vli_clear(uECC_word_t *vli, wordcount_t num_words);

/* Constant-time comparison to zero - secure way to compare long integers */
/* Returns 1 if vli == 0, 0 otherwise. */
uECC_word_t uECC_vli_isZero(const uECC_word_t *vli, wordcount_t num_words);

/* Returns nonzero if bit 'bit' of vli is set. */
uECC_word_t uECC_vli_testBit(const uECC_word_t *vli, bitcount_t bit);

/* Counts the number of bits required to represent vli. */
bitcount_t uECC_vli_numBits(const uECC_word_t *vli,
                            const wordcount_t max_words);

/* Sets dest = src. */
void uECC_vli_set(uECC_word_t *dest, const uECC_word_t *src,
                  wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns one if left == right, zero otherwise */
uECC_word_t uECC_vli_equal(const uECC_word_t *left, const uECC_word_t *right,
                           wordcount_t num_words);

/* Constant-time comparison function - secure way to compare long integers */
/* Returns sign of left - right, in constant time. */
cmpresult_t uECC_vli_cmp(const uECC_word_t *left, const uECC_word_t *right,
                         wordcount_t num_words);

/* Computes vli = vli >> 1. */
void uECC_vli_rshift1(uECC_word_t *vli, wordcount_t num_words);

/* Computes result = left + right, returning carry. Can modify in place. */
uECC_word_t uECC_vli_add(uECC_word_t *result, const uECC_word_t *left,
                         const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left - right, returning borrow. Can modify in place. */
uECC_word_t uECC_vli_sub(uECC_word_t *result, const uECC_word_t *left,
                         const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left * right. Result must be 2 * num_words long. */
void uECC_vli_mult(uECC_word_t *result, const uECC_word_t *left,
                   const uECC_word_t *right, wordcount_t num_words);

/* Computes result = left^2. Result must be 2 * num_words long. */
void uECC_vli_square(uECC_word_t *result, const uECC_word_t *left,
                     wordcount_t num_words);

/* Computes result = (left + right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void uECC_vli_modAdd(uECC_word_t *result, const uECC_word_t *left,
                     const uECC_word_t *right, const uECC_word_t *mod,
                     wordcount_t num_words);

/* Computes result = (left - right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap
   mod. */
void uECC_vli_modSub(uECC_word_t *result, const uECC_word_t *left,
                     const uECC_word_t *right, const uECC_word_t *mod,
                     wordcount_t num_words);

/* Computes result = product % mod, where product is 2N words long.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_mmod(uECC_word_t *result, uECC_word_t *product,
                   const uECC_word_t *mod, wordcount_t num_words);

/* Calculates result = product (mod curve->p), where product is up to
   2 * curve->num_words long. */
void uECC_vli_mmod_fast(uECC_word_t *result, uECC_word_t *product,
                        uECC_Curve curve);

/* Computes result = (left * right) % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_modMult(uECC_word_t *result, const uECC_word_t *left,
                      const uECC_word_t *right, const uECC_word_t *mod,
                      wordcount_t num_words);

/* Computes result = (left * right) % curve->p. */
void uECC_vli_modMult_fast(uECC_word_t *result, const uECC_word_t *left,
                           const uECC_word_t *right, uECC_Curve curve);

/* Computes result = left^2 % mod.
   Currently only designed to work for mod == curve->p or curve_n. */
void uECC_vli_modSquare(uECC_word_t *result, const uECC_word_t *left,
                        const uECC_word_t *mod, wordcount_t num_words);

/* Computes result = left^2 % curve->p. */
void uECC_vli_modSquare_fast(uECC_word_t *result, const uECC_word_t *left,
                             uECC_Curve curve);

/* Computes result = (1 / input) % mod.*/
void uECC_vli_modInv(uECC_word_t *result, const uECC_word_t *input,
                     const uECC_word_t *mod, wordcount_t num_words);

#if uECC_SUPPORT_COMPRESSED_POINT
/* Calculates a = sqrt(a) (mod curve->p) */
void uECC_vli_mod_sqrt(uECC_word_t *a, uECC_Curve curve);
#endif

/* Converts an integer in uECC native format to big-endian bytes. */
void uECC_vli_nativeToBytes(uint8_t *bytes, int num_bytes,
                            const uECC_word_t *native);
/* Converts big-endian bytes to an integer in uECC native format. */
void uECC_vli_bytesToNative(uECC_word_t *native, const uint8_t *bytes,
                            int num_bytes);

unsigned uECC_curve_num_words(uECC_Curve curve);
unsigned uECC_curve_num_bytes(uECC_Curve curve);
unsigned uECC_curve_num_bits(uECC_Curve curve);
unsigned uECC_curve_num_n_words(uECC_Curve curve);
unsigned uECC_curve_num_n_bytes(uECC_Curve curve);
unsigned uECC_curve_num_n_bits(uECC_Curve curve);

const uECC_word_t *uECC_curve_p(uECC_Curve curve);
const uECC_word_t *uECC_curve_n(uECC_Curve curve);
const uECC_word_t *uECC_curve_G(uECC_Curve curve);
const uECC_word_t *uECC_curve_b(uECC_Curve curve);

int uECC_valid_point(const uECC_word_t *point, uECC_Curve curve);

/* Multiplies a point by a scalar. Points are represented by the X coordinate
   followed by the Y coordinate in the same array, both coordinates are
   curve->num_words long. Note that scalar must be curve->num_n_words long (NOT
   curve->num_words). */
void uECC_point_mult(uECC_word_t *result, const uECC_word_t *point,
                     const uECC_word_t *scalar, uECC_Curve curve);

/* Generates a random integer in the range 0 < random < top.
   Both random and top have num_words words. */
int uECC_generate_random_int(uECC_word_t *random, const uECC_word_t *top,
                             wordcount_t num_words);

#endif /* uECC_ENABLE_VLI_API */

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* _UECC_VLI_H_ */

/* Copyright 2015, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifndef _UECC_TYPES_H_
#define _UECC_TYPES_H_

#ifndef uECC_PLATFORM
#if defined(__AVR__) && __AVR__
#define uECC_PLATFORM uECC_avr
#elif defined(__thumb2__) || \
    defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
#define uECC_PLATFORM uECC_arm_thumb2
#elif defined(__thumb__)
#define uECC_PLATFORM uECC_arm_thumb
#elif defined(__arm__) || defined(_M_ARM)
#define uECC_PLATFORM uECC_arm
#elif defined(__aarch64__)
#define uECC_PLATFORM uECC_arm64
#elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || \
    defined(__I86__)
#define uECC_PLATFORM uECC_x86
#elif defined(__amd64__) || defined(_M_X64)
#define uECC_PLATFORM uECC_x86_64
#else
#define uECC_PLATFORM uECC_arch_other
#endif
#endif

#ifndef uECC_ARM_USE_UMAAL
#if (uECC_PLATFORM == uECC_arm) && (__ARM_ARCH >= 6)
#define uECC_ARM_USE_UMAAL 1
#elif (uECC_PLATFORM == uECC_arm_thumb2) && (__ARM_ARCH >= 6) && \
    (!defined(__ARM_ARCH_7M__) || !__ARM_ARCH_7M__)
#define uECC_ARM_USE_UMAAL 1
#else
#define uECC_ARM_USE_UMAAL 0
#endif
#endif

#ifndef uECC_WORD_SIZE
#if uECC_PLATFORM == uECC_avr
#define uECC_WORD_SIZE 1
#elif (uECC_PLATFORM == uECC_x86_64 || uECC_PLATFORM == uECC_arm64)
#define uECC_WORD_SIZE 8
#else
#define uECC_WORD_SIZE 4
#endif
#endif

#if (uECC_WORD_SIZE != 1) && (uECC_WORD_SIZE != 4) && (uECC_WORD_SIZE != 8)
#error "Unsupported value for uECC_WORD_SIZE"
#endif

#if ((uECC_PLATFORM == uECC_avr) && (uECC_WORD_SIZE != 1))
#pragma message("uECC_WORD_SIZE must be 1 for AVR")
#undef uECC_WORD_SIZE
#define uECC_WORD_SIZE 1
#endif

#if ((uECC_PLATFORM == uECC_arm || uECC_PLATFORM == uECC_arm_thumb || \
      uECC_PLATFORM == uECC_arm_thumb2) &&                            \
     (uECC_WORD_SIZE != 4))
#pragma message("uECC_WORD_SIZE must be 4 for ARM")
#undef uECC_WORD_SIZE
#define uECC_WORD_SIZE 4
#endif

typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
typedef int8_t cmpresult_t;

#if (uECC_WORD_SIZE == 1)

typedef uint8_t uECC_word_t;
typedef uint16_t uECC_dword_t;

#define HIGH_BIT_SET 0x80
#define uECC_WORD_BITS 8
#define uECC_WORD_BITS_SHIFT 3
#define uECC_WORD_BITS_MASK 0x07

#elif (uECC_WORD_SIZE == 4)

typedef uint32_t uECC_word_t;
typedef uint64_t uECC_dword_t;

#define HIGH_BIT_SET 0x80000000
#define uECC_WORD_BITS 32
#define uECC_WORD_BITS_SHIFT 5
#define uECC_WORD_BITS_MASK 0x01F

#elif (uECC_WORD_SIZE == 8)

typedef uint64_t uECC_word_t;

#define HIGH_BIT_SET 0x8000000000000000U
#define uECC_WORD_BITS 64
#define uECC_WORD_BITS_SHIFT 6
#define uECC_WORD_BITS_MASK 0x03F

#endif /* uECC_WORD_SIZE */

#endif /* _UECC_TYPES_H_ */
// End of uecc BSD-2


struct mg_connection;
typedef void (*mg_event_handler_t)(struct mg_connection *, int ev,
                                   void *ev_data);
void mg_call(struct mg_connection *c, int ev, void *ev_data);
void mg_error(struct mg_connection *c, const char *fmt, ...);

enum {
  MG_EV_ERROR,      // Error                        char *error_message
  MG_EV_OPEN,       // Connection created           NULL
  MG_EV_POLL,       // mg_mgr_poll iteration        uint64_t *uptime_millis
  MG_EV_RESOLVE,    // Host name is resolved        NULL
  MG_EV_CONNECT,    // Connection established       NULL
  MG_EV_ACCEPT,     // Connection accepted          NULL
  MG_EV_TLS_HS,     // TLS handshake succeeded      NULL
  MG_EV_READ,       // Data received from socket    long *bytes_read
  MG_EV_WRITE,      // Data written to socket       long *bytes_written
  MG_EV_CLOSE,      // Connection closed            NULL
  MG_EV_HTTP_MSG,   // HTTP request/response        struct mg_http_message *
  MG_EV_WS_OPEN,    // Websocket handshake done     struct mg_http_message *
  MG_EV_WS_MSG,     // Websocket msg, text or bin   struct mg_ws_message *
  MG_EV_WS_CTL,     // Websocket control msg        struct mg_ws_message *
  MG_EV_MQTT_CMD,   // MQTT low-level command       struct mg_mqtt_message *
  MG_EV_MQTT_MSG,   // MQTT PUBLISH received        struct mg_mqtt_message *
  MG_EV_MQTT_OPEN,  // MQTT CONNACK received        int *connack_status_code
  MG_EV_SNTP_TIME,  // SNTP time received           uint64_t *epoch_millis
  MG_EV_WAKEUP,     // mg_wakeup() data received    struct mg_str *data
  MG_EV_USER        // Starting ID for user events
};









struct mg_dns {
  const char *url;          // DNS server URL
  struct mg_connection *c;  // DNS server connection
};

struct mg_addr {
  uint8_t ip[16];    // Holds IPv4 or IPv6 address, in network byte order
  uint16_t port;     // TCP or UDP port in network byte order
  uint8_t scope_id;  // IPv6 scope ID
  bool is_ip6;       // True when address is IPv6 address
};

struct mg_mgr {
  struct mg_connection *conns;  // List of active connections
  struct mg_dns dns4;           // DNS for IPv4
  struct mg_dns dns6;           // DNS for IPv6
  int dnstimeout;               // DNS resolve timeout in milliseconds
  bool use_dns6;                // Use DNS6 server by default, see #1532
  unsigned long nextid;         // Next connection ID
  unsigned long timerid;        // Next timer ID
  void *userdata;               // Arbitrary user data pointer
  void *tls_ctx;                // TLS context shared by all TLS sessions
  uint16_t mqtt_id;             // MQTT IDs for pub/sub
  void *active_dns_requests;    // DNS requests in progress
  struct mg_timer *timers;      // Active timers
  int epoll_fd;                 // Used when MG_EPOLL_ENABLE=1
  void *priv;                   // Used by the MIP stack
  size_t extraconnsize;         // Used by the MIP stack
  MG_SOCKET_TYPE pipe;          // Socketpair end for mg_wakeup()
#if MG_ENABLE_FREERTOS_TCP
  SocketSet_t ss;  // NOTE(lsm): referenced from socket struct
#endif
};

struct mg_connection {
  struct mg_connection *next;  // Linkage in struct mg_mgr :: connections
  struct mg_mgr *mgr;          // Our container
  struct mg_addr loc;          // Local address
  struct mg_addr rem;          // Remote address
  void *fd;                    // Connected socket, or LWIP data
  unsigned long id;            // Auto-incrementing unique connection ID
  struct mg_iobuf recv;        // Incoming data
  struct mg_iobuf send;        // Outgoing data
  struct mg_iobuf prof;        // Profile data enabled by MG_ENABLE_PROFILE
  struct mg_iobuf rtls;        // TLS only. Incoming encrypted data
  mg_event_handler_t fn;       // User-specified event handler function
  void *fn_data;               // User-specified function parameter
  mg_event_handler_t pfn;      // Protocol-specific handler function
  void *pfn_data;              // Protocol-specific function parameter
  char data[MG_DATA_SIZE];     // Arbitrary connection data
  void *tls;                   // TLS specific data
  unsigned is_listening : 1;   // Listening connection
  unsigned is_client : 1;      // Outbound (client) connection
  unsigned is_accepted : 1;    // Accepted (server) connection
  unsigned is_resolving : 1;   // Non-blocking DNS resolution is in progress
  unsigned is_arplooking : 1;  // Non-blocking ARP resolution is in progress
  unsigned is_connecting : 1;  // Non-blocking connect is in progress
  unsigned is_tls : 1;         // TLS-enabled connection
  unsigned is_tls_hs : 1;      // TLS handshake is in progress
  unsigned is_udp : 1;         // UDP connection
  unsigned is_websocket : 1;   // WebSocket connection
  unsigned is_mqtt5 : 1;       // For MQTT connection, v5 indicator
  unsigned is_hexdumping : 1;  // Hexdump in/out traffic
  unsigned is_draining : 1;    // Send remaining data, then close and free
  unsigned is_closing : 1;     // Close and free the connection immediately
  unsigned is_full : 1;        // Stop reads, until cleared
  unsigned is_resp : 1;        // Response is still being generated
  unsigned is_readable : 1;    // Connection is ready to read
  unsigned is_writable : 1;    // Connection is ready to write
};

void mg_mgr_poll(struct mg_mgr *, int ms);
void mg_mgr_init(struct mg_mgr *);
void mg_mgr_free(struct mg_mgr *);

struct mg_connection *mg_listen(struct mg_mgr *, const char *url,
                                mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_connect(struct mg_mgr *, const char *url,
                                 mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_wrapfd(struct mg_mgr *mgr, int fd,
                                mg_event_handler_t fn, void *fn_data);
void mg_connect_resolved(struct mg_connection *);
bool mg_send(struct mg_connection *, const void *, size_t);
size_t mg_printf(struct mg_connection *, const char *fmt, ...);
size_t mg_vprintf(struct mg_connection *, const char *fmt, va_list *ap);
bool mg_aton(struct mg_str str, struct mg_addr *addr);

// These functions are used to integrate with custom network stacks
struct mg_connection *mg_alloc_conn(struct mg_mgr *);
void mg_close_conn(struct mg_connection *c);
bool mg_open_listener(struct mg_connection *c, const char *url);

// Utility functions
bool mg_wakeup(struct mg_mgr *, unsigned long id, const void *buf, size_t len);
bool mg_wakeup_init(struct mg_mgr *);
struct mg_timer *mg_timer_add(struct mg_mgr *mgr, uint64_t milliseconds,
                              unsigned flags, void (*fn)(void *), void *arg);








struct mg_http_header {
  struct mg_str name;   // Header name
  struct mg_str value;  // Header value
};

struct mg_http_message {
  struct mg_str method, uri, query, proto;             // Request/response line
  struct mg_http_header headers[MG_MAX_HTTP_HEADERS];  // Headers
  struct mg_str body;                                  // Body
  struct mg_str head;                                  // Request + headers
  struct mg_str message;  // Request + headers + body
};

// Parameter for mg_http_serve_dir()
struct mg_http_serve_opts {
  const char *root_dir;       // Web root directory, must be non-NULL
  const char *ssi_pattern;    // SSI file name pattern, e.g. #.shtml
  const char *extra_headers;  // Extra HTTP headers to add in responses
  const char *mime_types;     // Extra mime types, ext1=type1,ext2=type2,..
  const char *page404;        // Path to the 404 page, or NULL by default
  struct mg_fs *fs;           // Filesystem implementation. Use NULL for POSIX
};

// Parameter for mg_http_next_multipart
struct mg_http_part {
  struct mg_str name;      // Form field name
  struct mg_str filename;  // Filename for file uploads
  struct mg_str body;      // Part contents
};

int mg_http_parse(const char *s, size_t len, struct mg_http_message *);
int mg_http_get_request_len(const unsigned char *buf, size_t buf_len);
void mg_http_printf_chunk(struct mg_connection *cnn, const char *fmt, ...);
void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len);
void mg_http_delete_chunk(struct mg_connection *c, struct mg_http_message *hm);
struct mg_connection *mg_http_listen(struct mg_mgr *, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_http_connect(struct mg_mgr *, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_http_serve_dir(struct mg_connection *, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *);
void mg_http_serve_file(struct mg_connection *, struct mg_http_message *hm,
                        const char *path, const struct mg_http_serve_opts *);
void mg_http_reply(struct mg_connection *, int status_code, const char *headers,
                   const char *body_fmt, ...);
struct mg_str *mg_http_get_header(struct mg_http_message *, const char *name);
struct mg_str mg_http_var(struct mg_str buf, struct mg_str name);
int mg_http_get_var(const struct mg_str *, const char *name, char *, size_t);
int mg_url_decode(const char *s, size_t n, char *to, size_t to_len, int form);
size_t mg_url_encode(const char *s, size_t n, char *buf, size_t len);
void mg_http_creds(struct mg_http_message *, char *, size_t, char *, size_t);
bool mg_http_match_uri(const struct mg_http_message *, const char *glob);
long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_fs *fs, const char *dir, size_t max_size);
void mg_http_bauth(struct mg_connection *, const char *user, const char *pass);
struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v);
size_t mg_http_next_multipart(struct mg_str, size_t, struct mg_http_part *);
int mg_http_status(const struct mg_http_message *hm);
void mg_hello(const char *url);


void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath);


#define MG_TLS_NONE 0     // No TLS support
#define MG_TLS_MBED 1     // mbedTLS
#define MG_TLS_OPENSSL 2  // OpenSSL
#define MG_TLS_BUILTIN 3  // Built-in
#define MG_TLS_CUSTOM 4   // Custom implementation

#ifndef MG_TLS
#define MG_TLS MG_TLS_NONE
#endif





struct mg_tls_opts {
  struct mg_str ca;    // PEM or DER
  struct mg_str cert;  // PEM or DER
  struct mg_str key;   // PEM or DER
  struct mg_str name;  // If not empty, enable host name verification
};

void mg_tls_init(struct mg_connection *, const struct mg_tls_opts *opts);
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);

// Private
void mg_tls_ctx_init(struct mg_mgr *);
void mg_tls_ctx_free(struct mg_mgr *);

// Low-level IO primives used by TLS layer
enum { MG_IO_ERR = -1, MG_IO_WAIT = -2, MG_IO_RESET = -3 };
long mg_io_send(struct mg_connection *c, const void *buf, size_t len);
long mg_io_recv(struct mg_connection *c, void *buf, size_t len);







#if MG_TLS == MG_TLS_MBED
#include <mbedtls/debug.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ssl_ticket.h>

struct mg_tls_ctx {
  int dummy;
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context tickets;
#endif
};

struct mg_tls {
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_pk_context pk;    // Private key context
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
#ifdef MBEDTLS_SSL_SESSION_TICKETS
  mbedtls_ssl_ticket_context ticket;  // Session tickets context
#endif
};
#endif


#if MG_TLS == MG_TLS_OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  BIO_METHOD *bm;
  SSL_CTX *ctx;
  SSL *ssl;
};
#endif


#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10



struct mg_ws_message {
  struct mg_str data;  // Websocket message data
  uint8_t flags;       // Websocket message flags
};

struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *,
                   const char *fmt, ...);
size_t mg_ws_send(struct mg_connection *, const void *buf, size_t len, int op);
size_t mg_ws_wrap(struct mg_connection *, size_t len, int op);
size_t mg_ws_printf(struct mg_connection *c, int op, const char *fmt, ...);
size_t mg_ws_vprintf(struct mg_connection *c, int op, const char *fmt,
                     va_list *);




struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data);
void mg_sntp_request(struct mg_connection *c);
int64_t mg_sntp_parse(const unsigned char *buf, size_t len);





#define MQTT_CMD_CONNECT 1
#define MQTT_CMD_CONNACK 2
#define MQTT_CMD_PUBLISH 3
#define MQTT_CMD_PUBACK 4
#define MQTT_CMD_PUBREC 5
#define MQTT_CMD_PUBREL 6
#define MQTT_CMD_PUBCOMP 7
#define MQTT_CMD_SUBSCRIBE 8
#define MQTT_CMD_SUBACK 9
#define MQTT_CMD_UNSUBSCRIBE 10
#define MQTT_CMD_UNSUBACK 11
#define MQTT_CMD_PINGREQ 12
#define MQTT_CMD_PINGRESP 13
#define MQTT_CMD_DISCONNECT 14
#define MQTT_CMD_AUTH 15

#define MQTT_PROP_PAYLOAD_FORMAT_INDICATOR 0x01
#define MQTT_PROP_MESSAGE_EXPIRY_INTERVAL 0x02
#define MQTT_PROP_CONTENT_TYPE 0x03
#define MQTT_PROP_RESPONSE_TOPIC 0x08
#define MQTT_PROP_CORRELATION_DATA 0x09
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER 0x0B
#define MQTT_PROP_SESSION_EXPIRY_INTERVAL 0x11
#define MQTT_PROP_ASSIGNED_CLIENT_IDENTIFIER 0x12
#define MQTT_PROP_SERVER_KEEP_ALIVE 0x13
#define MQTT_PROP_AUTHENTICATION_METHOD 0x15
#define MQTT_PROP_AUTHENTICATION_DATA 0x16
#define MQTT_PROP_REQUEST_PROBLEM_INFORMATION 0x17
#define MQTT_PROP_WILL_DELAY_INTERVAL 0x18
#define MQTT_PROP_REQUEST_RESPONSE_INFORMATION 0x19
#define MQTT_PROP_RESPONSE_INFORMATION 0x1A
#define MQTT_PROP_SERVER_REFERENCE 0x1C
#define MQTT_PROP_REASON_STRING 0x1F
#define MQTT_PROP_RECEIVE_MAXIMUM 0x21
#define MQTT_PROP_TOPIC_ALIAS_MAXIMUM 0x22
#define MQTT_PROP_TOPIC_ALIAS 0x23
#define MQTT_PROP_MAXIMUM_QOS 0x24
#define MQTT_PROP_RETAIN_AVAILABLE 0x25
#define MQTT_PROP_USER_PROPERTY 0x26
#define MQTT_PROP_MAXIMUM_PACKET_SIZE 0x27
#define MQTT_PROP_WILDCARD_SUBSCRIPTION_AVAILABLE 0x28
#define MQTT_PROP_SUBSCRIPTION_IDENTIFIER_AVAILABLE 0x29
#define MQTT_PROP_SHARED_SUBSCRIPTION_AVAILABLE 0x2A

enum {
  MQTT_PROP_TYPE_BYTE,
  MQTT_PROP_TYPE_STRING,
  MQTT_PROP_TYPE_STRING_PAIR,
  MQTT_PROP_TYPE_BINARY_DATA,
  MQTT_PROP_TYPE_VARIABLE_INT,
  MQTT_PROP_TYPE_INT,
  MQTT_PROP_TYPE_SHORT
};

enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

struct mg_mqtt_prop {
  uint8_t id;         // Enumerated at MQTT5 Reference
  uint32_t iv;        // Integer value for 8-, 16-, 32-bit integers types
  struct mg_str key;  // Non-NULL only for user property type
  struct mg_str val;  // Non-NULL only for UTF-8 types and user properties
};

struct mg_mqtt_opts {
  struct mg_str user;               // Username, can be empty
  struct mg_str pass;               // Password, can be empty
  struct mg_str client_id;          // Client ID
  struct mg_str topic;              // message/subscription topic
  struct mg_str message;            // message content
  uint8_t qos;                      // message quality of service
  uint8_t version;                  // Can be 4 (3.1.1), or 5. If 0, assume 4
  uint16_t keepalive;               // Keep-alive timer in seconds
  bool retain;                      // Retain flag
  bool clean;                       // Clean session flag
  struct mg_mqtt_prop *props;       // MQTT5 props array
  size_t num_props;                 // number of props
  struct mg_mqtt_prop *will_props;  // Valid only for CONNECT packet (MQTT5)
  size_t num_will_props;            // Number of will props
};

struct mg_mqtt_message {
  struct mg_str topic;  // Parsed topic for PUBLISH
  struct mg_str data;   // Parsed message for PUBLISH
  struct mg_str dgram;  // Whole MQTT packet, including headers
  uint16_t id;          // For PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, PUBLISH
  uint8_t cmd;          // MQTT command, one of MQTT_CMD_*
  uint8_t qos;          // Quality of service
  uint8_t ack;          // CONNACK return code, 0 = success
  size_t props_start;   // Offset to the start of the properties (MQTT5)
  size_t props_size;    // Length of the properties
};

struct mg_connection *mg_mqtt_connect(struct mg_mgr *, const char *url,
                                      const struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data);
struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data);
void mg_mqtt_login(struct mg_connection *c, const struct mg_mqtt_opts *opts);
void mg_mqtt_pub(struct mg_connection *c, const struct mg_mqtt_opts *opts);
void mg_mqtt_sub(struct mg_connection *, const struct mg_mqtt_opts *opts);
int mg_mqtt_parse(const uint8_t *, size_t, uint8_t, struct mg_mqtt_message *);
void mg_mqtt_send_header(struct mg_connection *, uint8_t cmd, uint8_t flags,
                         uint32_t len);
void mg_mqtt_ping(struct mg_connection *);
void mg_mqtt_pong(struct mg_connection *);
void mg_mqtt_disconnect(struct mg_connection *, const struct mg_mqtt_opts *);
size_t mg_mqtt_next_prop(struct mg_mqtt_message *, struct mg_mqtt_prop *,
                         size_t ofs);





// Mongoose sends DNS queries that contain only one question:
// either A (IPv4) or AAAA (IPv6) address lookup.
// Therefore, we expect zero or one answer.
// If `resolved` is true, then `addr` contains resolved IPv4 or IPV6 address.
struct mg_dns_message {
  uint16_t txnid;       // Transaction ID
  bool resolved;        // Resolve successful, addr is set
  struct mg_addr addr;  // Resolved address
  char name[256];       // Host name
};

struct mg_dns_header {
  uint16_t txnid;  // Transaction ID
  uint16_t flags;
  uint16_t num_questions;
  uint16_t num_answers;
  uint16_t num_authority_prs;
  uint16_t num_other_prs;
};

// DNS resource record
struct mg_dns_rr {
  uint16_t nlen;    // Name or pointer length
  uint16_t atype;   // Address type
  uint16_t aclass;  // Address class
  uint16_t alen;    // Address length
};

void mg_resolve(struct mg_connection *, const char *url);
void mg_resolve_cancel(struct mg_connection *);
bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *);
size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *);





#ifndef MG_JSON_MAX_DEPTH
#define MG_JSON_MAX_DEPTH 30
#endif

// Error return values - negative. Successful returns are >= 0
enum { MG_JSON_TOO_DEEP = -1, MG_JSON_INVALID = -2, MG_JSON_NOT_FOUND = -3 };
int mg_json_get(struct mg_str json, const char *path, int *toklen);

struct mg_str mg_json_get_tok(struct mg_str json, const char *path);
bool mg_json_get_num(struct mg_str json, const char *path, double *v);
bool mg_json_get_bool(struct mg_str json, const char *path, bool *v);
long mg_json_get_long(struct mg_str json, const char *path, long dflt);
char *mg_json_get_str(struct mg_str json, const char *path);
char *mg_json_get_hex(struct mg_str json, const char *path, int *len);
char *mg_json_get_b64(struct mg_str json, const char *path, int *len);

bool mg_json_unescape(struct mg_str str, char *buf, size_t len);
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val);




// JSON-RPC request descriptor
struct mg_rpc_req {
  struct mg_rpc **head;  // RPC handlers list head
  struct mg_rpc *rpc;    // RPC handler being called
  mg_pfn_t pfn;          // Response printing function
  void *pfn_data;        // Response printing function data
  void *req_data;        // Arbitrary request data
  struct mg_str frame;   // Request, e.g. {"id":1,"method":"add","params":[1,2]}
};

// JSON-RPC method handler
struct mg_rpc {
  struct mg_rpc *next;              // Next in list
  struct mg_str method;             // Method pattern
  void (*fn)(struct mg_rpc_req *);  // Handler function
  void *fn_data;                    // Handler function argument
};

void mg_rpc_add(struct mg_rpc **head, struct mg_str method_pattern,
                void (*handler)(struct mg_rpc_req *), void *handler_data);
void mg_rpc_del(struct mg_rpc **head, void (*handler)(struct mg_rpc_req *));
void mg_rpc_process(struct mg_rpc_req *);

// Helper functions to print result or error frame
void mg_rpc_ok(struct mg_rpc_req *, const char *fmt, ...);
void mg_rpc_vok(struct mg_rpc_req *, const char *fmt, va_list *ap);
void mg_rpc_err(struct mg_rpc_req *, int code, const char *fmt, ...);
void mg_rpc_verr(struct mg_rpc_req *, int code, const char *fmt, va_list *);
void mg_rpc_list(struct mg_rpc_req *r);
// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved





#define MG_OTA_NONE 0      // No OTA support
#define MG_OTA_FLASH 1     // OTA via an internal flash
#define MG_OTA_CUSTOM 100  // Custom implementation

#ifndef MG_OTA
#define MG_OTA MG_OTA_NONE
#endif

#if defined(__GNUC__) && !defined(__APPLE__)
#define MG_IRAM __attribute__((section(".iram")))
#else
#define MG_IRAM
#endif

// Firmware update API
bool mg_ota_begin(size_t new_firmware_size);     // Start writing
bool mg_ota_write(const void *buf, size_t len);  // Write chunk, aligned to 1k
bool mg_ota_end(void);                           // Stop writing

enum {
  MG_OTA_UNAVAILABLE = 0,  // No OTA information is present
  MG_OTA_FIRST_BOOT = 1,   // Device booting the first time after the OTA
  MG_OTA_UNCOMMITTED = 2,  // Ditto, but marking us for the rollback
  MG_OTA_COMMITTED = 3     // The firmware is good
};
enum { MG_FIRMWARE_CURRENT = 0, MG_FIRMWARE_PREVIOUS = 1 };

int mg_ota_status(int firmware);          // Return firmware status MG_OTA_*
uint32_t mg_ota_crc32(int firmware);      // Return firmware checksum
uint32_t mg_ota_timestamp(int firmware);  // Firmware timestamp, UNIX UTC epoch
size_t mg_ota_size(int firmware);         // Firmware size

bool mg_ota_commit(void);        // Commit current firmware
bool mg_ota_rollback(void);      // Rollback to the previous firmware
MG_IRAM void mg_ota_boot(void);  // Bootloader function
// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved





#define MG_DEVICE_NONE 0        // Dummy system
#define MG_DEVICE_STM32H5 1     // STM32 H5
#define MG_DEVICE_STM32H7 2     // STM32 H7
#define MG_DEVICE_RT1020 3      // IMXRT1020
#define MG_DEVICE_RT1060 4      // IMXRT1060
#define MG_DEVICE_CH32V307 100  // WCH CH32V307
#define MG_DEVICE_CUSTOM 1000   // Custom implementation

#ifndef MG_DEVICE
#define MG_DEVICE MG_DEVICE_NONE
#endif

// Flash information
void *mg_flash_start(void);         // Return flash start address
size_t mg_flash_size(void);         // Return flash size
size_t mg_flash_sector_size(void);  // Return flash sector size
size_t mg_flash_write_align(void);  // Return flash write align, minimum 4
int mg_flash_bank(void);            // 0: not dual bank, 1: bank1, 2: bank2

// Write, erase, swap bank
bool mg_flash_write(void *addr, const void *buf, size_t len);
bool mg_flash_erase(void *sector);
bool mg_flash_swap_bank(void);

// Convenience functions to store data on a flash sector with wear levelling
// If `sector` is NULL, then the last sector of flash is used
bool mg_flash_load(void *sector, uint32_t key, void *buf, size_t len);
bool mg_flash_save(void *sector, uint32_t key, const void *buf, size_t len);

void mg_device_reset(void);  // Reboot device immediately






#if defined(MG_ENABLE_TCPIP) && MG_ENABLE_TCPIP
struct mg_tcpip_if;  // Mongoose TCP/IP network interface

struct mg_tcpip_driver {
  bool (*init)(struct mg_tcpip_if *);                         // Init driver
  size_t (*tx)(const void *, size_t, struct mg_tcpip_if *);   // Transmit frame
  size_t (*rx)(void *buf, size_t len, struct mg_tcpip_if *);  // Receive frame
  bool (*up)(struct mg_tcpip_if *);                           // Up/down status
};

// Network interface
struct mg_tcpip_if {
  uint8_t mac[6];                  // MAC address. Must be set to a valid MAC
  uint32_t ip, mask, gw;           // IP address, mask, default gateway
  struct mg_str tx;                // Output (TX) buffer
  bool enable_dhcp_client;         // Enable DCHP client
  bool enable_dhcp_server;         // Enable DCHP server
  bool enable_get_gateway;         // DCHP server sets client as gateway
  bool enable_crc32_check;         // Do a CRC check on RX frames and strip it
  bool enable_mac_check;           // Do a MAC check on RX frames
  struct mg_tcpip_driver *driver;  // Low level driver
  void *driver_data;               // Driver-specific data
  struct mg_mgr *mgr;              // Mongoose event manager
  struct mg_queue recv_queue;      // Receive queue
  uint16_t mtu;                    // Interface MTU
#define MG_TCPIP_MTU_DEFAULT 1500

  // Internal state, user can use it but should not change it
  uint8_t gwmac[6];             // Router's MAC
  uint64_t now;                 // Current time
  uint64_t timer_1000ms;        // 1000 ms timer: for DHCP and link state
  uint64_t lease_expire;        // Lease expiration time, in ms
  uint16_t eport;               // Next ephemeral port
  volatile uint32_t ndrop;      // Number of received, but dropped frames
  volatile uint32_t nrecv;      // Number of received frames
  volatile uint32_t nsent;      // Number of transmitted frames
  volatile uint32_t nerr;       // Number of driver errors
  uint8_t state;                // Current state
#define MG_TCPIP_STATE_DOWN 0   // Interface is down
#define MG_TCPIP_STATE_UP 1     // Interface is up
#define MG_TCPIP_STATE_REQ 2    // Interface is up and has requested an IP
#define MG_TCPIP_STATE_READY 3  // Interface is up and has an IP assigned
};

void mg_tcpip_init(struct mg_mgr *, struct mg_tcpip_if *);
void mg_tcpip_free(struct mg_tcpip_if *);
void mg_tcpip_qwrite(void *buf, size_t len, struct mg_tcpip_if *ifp);

extern struct mg_tcpip_driver mg_tcpip_driver_stm32f;
extern struct mg_tcpip_driver mg_tcpip_driver_w5500;
extern struct mg_tcpip_driver mg_tcpip_driver_tm4c;
extern struct mg_tcpip_driver mg_tcpip_driver_stm32h;
extern struct mg_tcpip_driver mg_tcpip_driver_imxrt;
extern struct mg_tcpip_driver mg_tcpip_driver_same54;
extern struct mg_tcpip_driver mg_tcpip_driver_cmsis;

// Drivers that require SPI, can use this SPI abstraction
struct mg_tcpip_spi {
  void *spi;                        // Opaque SPI bus descriptor
  void (*begin)(void *);            // SPI begin: slave select low
  void (*end)(void *);              // SPI end: slave select high
  uint8_t (*txn)(void *, uint8_t);  // SPI transaction: write 1 byte, read reply
};
#endif



// Macros to record timestamped events that happens with a connection.
// They are saved into a c->prof IO buffer, each event is a name and a 32-bit
// timestamp in milliseconds since connection init time.
//
// Test (run in two separate terminals):
//   make -C examples/http-server/ CFLAGS_EXTRA=-DMG_ENABLE_PROFILE=1
//   curl localhost:8000
// Output:
//   1ea1f1e7 2 net.c:150:mg_close_conn      3 profile:                                                            
//   1ea1f1e8 2 net.c:150:mg_close_conn      1ea1f1e6 init                                                         
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_OPEN
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_ACCEPT 
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_READ
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_HTTP_MSG
//   1ea1f1e8 2 net.c:150:mg_close_conn          0 EV_WRITE
//   1ea1f1e8 2 net.c:150:mg_close_conn          1 EV_CLOSE
//
// Usage:
//   Enable profiling by setting MG_ENABLE_PROFILE=1
//   Invoke MG_PROF_ADD(c, "MY_EVENT_1") in the places you'd like to measure

#if MG_ENABLE_PROFILE
struct mg_profitem {
  const char *name;    // Event name
  uint32_t timestamp;  // Milliseconds since connection creation (MG_EV_OPEN)
};

#define MG_PROFILE_ALLOC_GRANULARITY 256  // Can save 32 items wih to realloc

// Adding a profile item to the c->prof. Must be as fast as possible.
// Reallocation of the c->prof iobuf is not desirable here, that's why we
// pre-allocate c->prof with MG_PROFILE_ALLOC_GRANULARITY.
// This macro just inits and copies 8 bytes, and calls mg_millis(),
// which should be fast enough.
#define MG_PROF_ADD(c, name_)                                             \
  do {                                                                    \
    struct mg_iobuf *io = &c->prof;                                       \
    uint32_t inittime = ((struct mg_profitem *) io->buf)->timestamp;      \
    struct mg_profitem item = {name_, (uint32_t) mg_millis() - inittime}; \
    mg_iobuf_add(io, io->len, &item, sizeof(item));                       \
  } while (0)

// Initialising profile for a new connection. Not time sensitive
#define MG_PROF_INIT(c)                                          \
  do {                                                           \
    struct mg_profitem first = {"init", (uint32_t) mg_millis()}; \
    mg_iobuf_init(&(c)->prof, 0, MG_PROFILE_ALLOC_GRANULARITY);  \
    mg_iobuf_add(&c->prof, c->prof.len, &first, sizeof(first));  \
  } while (0)

#define MG_PROF_FREE(c) mg_iobuf_free(&(c)->prof)

// Dumping the profile. Not time sensitive
#define MG_PROF_DUMP(c)                                            \
  do {                                                             \
    struct mg_iobuf *io = &c->prof;                                \
    struct mg_profitem *p = (struct mg_profitem *) io->buf;        \
    struct mg_profitem *e = &p[io->len / sizeof(*p)];              \
    MG_INFO(("%lu profile:", c->id));                              \
    while (p < e) {                                                \
      MG_INFO(("%5lx %s", (unsigned long) p->timestamp, p->name)); \
      p++;                                                         \
    }                                                              \
  } while (0)

#else
#define MG_PROF_INIT(c)
#define MG_PROF_FREE(c)
#define MG_PROF_ADD(c, name)
#define MG_PROF_DUMP(c)
#endif


#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_CMSIS) && MG_ENABLE_DRIVER_CMSIS

#include "Driver_ETH_MAC.h"  // keep this include
#include "Driver_ETH_PHY.h"  // keep this include

#endif


struct mg_tcpip_driver_imxrt_data {
  // MDC clock divider. MDC clock is derived from IPS Bus clock (ipg_clk),
  // must not exceed 2.5MHz. Configuration for clock range 2.36~2.50 MHz
  // 37.5.1.8.2, Table 37-46 : f = ipg_clk / (2(mdc_cr + 1))
  //    ipg_clk       mdc_cr VALUE
  //    --------------------------
  //                  -1  <-- TODO() tell driver to guess the value
  //    25 MHz         4
  //    33 MHz         6
  //    40 MHz         7
  //    50 MHz         9
  //    66 MHz        13
  int mdc_cr;  // Valid values: -1 to 63

  uint8_t phy_addr;  // PHY address
};


struct mg_tcpip_driver_same54_data {
    int mdc_cr;
};


struct mg_tcpip_driver_stm32f_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-216 MHz   HCLK/102       4  <-- value for Nucleo-F* on max speed
  //    216-310 MHz   HCLK/124       5
  //    110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5

  uint8_t phy_addr;  // PHY address
};


struct mg_tcpip_driver_stm32h_data {
  // MDC clock divider. MDC clock is derived from HCLK, must not exceed 2.5MHz
  //    HCLK range    DIVIDER    mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    HCLK/42        0
  //    100-150 MHz   HCLK/62        1
  //    20-35 MHz     HCLK/16        2
  //    35-60 MHz     HCLK/26        3
  //    150-250 MHz   HCLK/102       4  <-- value for Nucleo-H* on max speed driven by HSI
  //    250-300 MHz   HCLK/124       5  <-- value for Nucleo-H* on max speed driven by CSI
  //    110, 111 Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3, 4, 5
};


struct mg_tcpip_driver_tm4c_data {
  // MDC clock divider. MDC clock is derived from SYSCLK, must not exceed 2.5MHz
  //    SYSCLK range   DIVIDER   mdc_cr VALUE
  //    -------------------------------------
  //                                -1  <-- tell driver to guess the value
  //    60-100 MHz    SYSCLK/42      0
  //    100-150 MHz   SYSCLK/62      1  <-- value for EK-TM4C129* on max speed
  //    20-35 MHz     SYSCLK/16      2
  //    35-60 MHz     SYSCLK/26      3
  //    0x4-0xF Reserved
  int mdc_cr;  // Valid values: -1, 0, 1, 2, 3
};

#ifdef __cplusplus
}
#endif
#endif  // MONGOOSE_H
