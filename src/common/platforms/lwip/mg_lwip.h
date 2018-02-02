/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_LWIP_MG_LWIP_H_
#define CS_COMMON_PLATFORMS_LWIP_MG_LWIP_H_

#ifndef MG_LWIP
#define MG_LWIP 0
#endif

#if MG_LWIP

/*
 * When compiling for nRF5x chips with arm-none-eabi-gcc, it has BYTE_ORDER
 * already defined, so in order to avoid warnings in lwip, we have to undefine
 * it.
 *
 * TODO: Check if in the future versions of nRF5 SDK that changes.
 *       Current version of nRF51 SDK: 0.8.0
 *                          nRF5 SDK:  0.9.0
 */
#if CS_PLATFORM == CS_P_NRF51 || CS_PLATFORM == CS_P_NRF52
#undef BYTE_ORDER
#endif

#include <lwip/opt.h>
#include <lwip/err.h>
#include <lwip/ip_addr.h>
#include <lwip/inet.h>
#include <lwip/netdb.h>
#include <lwip/dns.h>

#ifndef LWIP_PROVIDE_ERRNO
#include <errno.h>
#endif

#if LWIP_SOCKET
#include <lwip/sockets.h>
#else
/* We really need the definitions from sockets.h. */
#undef LWIP_SOCKET
#define LWIP_SOCKET 1
#include <lwip/sockets.h>
#undef LWIP_SOCKET
#define LWIP_SOCKET 0
#endif

#define INVALID_SOCKET (-1)
#define SOMAXCONN 10
typedef int sock_t;

#if MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL
struct mg_mgr;
struct mg_connection;
uint32_t mg_lwip_get_poll_delay_ms(struct mg_mgr *mgr);
void mg_lwip_set_keepalive_params(struct mg_connection *nc, int idle,
                                  int interval, int count);
#endif

/* For older version of LWIP */
#ifndef ipX_2_ip
#define ipX_2_ip(x) (x)
#endif

#endif /* MG_LWIP */

#endif /* CS_COMMON_PLATFORMS_LWIP_MG_LWIP_H_ */
