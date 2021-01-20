#pragma once

#ifndef MG_ENABLE_LWIP
#define MG_ENABLE_LWIP 0
#endif

#if MG_ENABLE_LWIP
#define MG_ENABLE_SOCKET 0
#elif !defined(MG_ENABLE_SOCKET)
#define MG_ENABLE_SOCKET 1
#endif

#ifndef MG_ENABLE_MBEDTLS
#define MG_ENABLE_MBEDTLS 0
#endif

#ifndef MG_ENABLE_OPENSSL
#define MG_ENABLE_OPENSSL 0
#endif

#ifndef MG_ENABLE_FS
#define MG_ENABLE_FS 1
#endif

#ifndef MG_ENABLE_SSI
#define MG_ENABLE_SSI 0
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#ifndef MG_ENABLE_LOG
#define MG_ENABLE_LOG 1
#endif

#ifndef MG_ENABLE_MGOS
#define MG_ENABLE_MGOS 0
#endif

#ifndef MG_ENABLE_MD5
#define MG_ENABLE_MD5 0
#endif

#ifndef MG_ENABLE_DIRECTORY_LISTING
#define MG_ENABLE_DIRECTORY_LISTING 0
#endif

#ifndef MG_ENABLE_HTTP_DEBUG_ENDPOINT
#define MG_ENABLE_HTTP_DEBUG_ENDPOINT 0
#endif

#ifndef MG_ENABLE_SOCKETPAIR
#define MG_ENABLE_SOCKETPAIR 0
#endif

// Granularity of the send/recv IO buffer growth
#ifndef MG_IO_SIZE
#define MG_IO_SIZE 512
#endif

// Maximum size of the recv IO buffer
#ifndef MG_MAX_RECV_BUF_SIZE
#define MG_MAX_RECV_BUF_SIZE (3 * 1024 * 1024)
#endif

#ifndef MG_MAX_HTTP_HEADERS
#define MG_MAX_HTTP_HEADERS 40
#endif
