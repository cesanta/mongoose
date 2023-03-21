#pragma once

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

#define MG_SOCK_INTR(fd) 0

#define socklen_t int
#endif
