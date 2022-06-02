#pragma once

#ifndef MG_ENABLE_MIP
#define MG_ENABLE_MIP 0
#endif

#ifndef MG_ENABLE_POLL
#define MG_ENABLE_POLL 0
#endif

#ifndef MG_ENABLE_FATFS
#define MG_ENABLE_FATFS 0
#endif

#ifndef MG_ENABLE_SOCKET
#define MG_ENABLE_SOCKET 1
#endif

#ifndef MG_ENABLE_MBEDTLS
#define MG_ENABLE_MBEDTLS 0
#endif

#ifndef MG_ENABLE_OPENSSL
#define MG_ENABLE_OPENSSL 0
#endif

#ifndef MG_ENABLE_CUSTOM_TLS
#define MG_ENABLE_CUSTOM_TLS 0
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 0
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

// Granularity of the send/recv IO buffer growth
#ifndef MG_IO_SIZE
#define MG_IO_SIZE 2048
#endif

// Maximum size of the recv IO buffer
#ifndef MG_MAX_RECV_SIZE
#define MG_MAX_RECV_SIZE (3 * 1024 * 1024)
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 40
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
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif

#ifndef MG_DIRSEP
#define MG_DIRSEP '/'
#endif

#ifndef MG_ENABLE_FILE
#if defined(FOPEN_MAX)
#define MG_ENABLE_FILE 1
#else
#define MG_ENABLE_FILE 0
#endif
#endif

#ifndef MG_PUTCHAR
#define MG_PUTCHAR(x) putchar(x)
#endif
