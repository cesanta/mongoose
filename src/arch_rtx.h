#pragma once

#if MG_ARCH == MG_ARCH_RTX

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <rl_net.h>

#define MG_ENABLE_CUSTOM_MILLIS 1
typedef int socklen_t;
#define closesocket(x) closesocket(x)
#define mkdir(a, b) (-1)
#define EWOULDBLOCK BSD_EWOULDBLOCK
#define EAGAIN BSD_EWOULDBLOCK
#define EINPROGRESS BSD_EWOULDBLOCK
#define EINTR BSD_EWOULDBLOCK
#define ECONNRESET BSD_ECONNRESET
#define EPIPE BSD_ECONNRESET
#define TCP_NODELAY SO_KEEPALIVE

#endif
