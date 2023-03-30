#pragma once

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
