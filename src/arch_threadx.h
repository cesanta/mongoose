#pragma once

#if MG_ARCH == MG_ARCH_THREADX

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Do not include time.h and stdlib.h, since they conflict with nxd_bsd.h
// extern time_t time(time_t *);
#include <nxd_bsd.h>

#define MG_DIRSEP '\\'
#undef FOPEN_MAX

#ifndef MG_PATH_MAX
#define MG_PATH_MAX 32
#endif

#ifndef MG_SOCK_LISTEN_BACKLOG_SIZE
#define MG_SOCK_LISTEN_BACKLOG_SIZE 3
#endif

#ifndef MG_ENABLE_IPV6
#define MG_ENABLE_IPV6 0
#endif

#define socklen_t int
#define closesocket(x) soc_close(x)

// In order to enable BSD support in NetxDuo, do the following (assuming Cube):
// 1. Add nxd_bsd.h and nxd_bsd.c to the repo:
//     https://github.com/eclipse-threadx/netxduo/blob/v6.1.12_rel/addons/BSD/nxd_bsd.c
//     https://github.com/eclipse-threadx/netxduo/blob/v6.1.12_rel/addons/BSD/nxd_bsd.h
// 2. Add to tx_user.h
//     #define TX_THREAD_USER_EXTENSION int bsd_errno;
// 3. Add to nx_user.h
//     #define NX_ENABLE_EXTENDED_NOTIFY_SUPPORT
// 4. Add __CCRX__ build preprocessor constant
//   Project -> Properties -> C/C++ -> Settings -> MCU Compiler -> Preprocessor

#endif
