#pragma once

#if MG_ARCH == MG_ARCH_ESP8266

#include <dirent.h>
#include <stdbool.h>
#include <sys/time.h>
#include <user_interface.h>

#define MG_DIRSEP '/'
#ifndef PATH_MAX
#define PATH_MAX 128
#endif
#define MG_ENABLE_LWIP 1
#define LWIP_TIMEVAL_PRIVATE 0  // struct timeval is defined in sys/time.h

#if LWIP_SOCKET
#include <lwip/sockets.h>
#else
#undef LWIP_SOCKET
#define LWIP_SOCKET 1
#include <lwip/sockets.h>  // Source sockaddr_in anyway
#undef LWIP_SOCKET
#define LWIP_SOCKET 0
#endif

#endif
