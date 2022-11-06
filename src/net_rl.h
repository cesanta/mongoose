#pragma once

#if defined(MG_ENABLE_RL) && MG_ENABLE_RL
#include <rl_net.h>

#define MG_ENABLE_CUSTOM_MILLIS 1
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
