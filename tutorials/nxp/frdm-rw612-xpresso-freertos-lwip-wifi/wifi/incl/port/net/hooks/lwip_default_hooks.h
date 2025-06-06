/*
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/*!\file lwip_default_hooks.h
 *\brief This file provides interfaces for lwip hook.
*/

#ifndef _LWIP_DEFAULT_HOOKS_H_
#define _LWIP_DEFAULT_HOOKS_H_
#include <osa.h>
#if defined(SDK_OS_FREE_RTOS)

#include "lwip/priv/tcp_priv.h"
#include "lwip/tcp.h"
#include "lwip/pbuf.h"

/**To find the valid net
*\param[in] src  the source ipv4 address
*\param[in] dest the destination ipv4 address
*\return the match net if find, otherwise return NULL.
*/
struct netif *lwip_hook_ip4_route_src(const ip4_addr_t *src, const ip4_addr_t *dest);

#define LWIP_HOOK_TCP_OUT_ADD_TCPOPTS(p, hdr, pcb, opts) lwip_hook_tcp_out_add_tcpopts(p, hdr, pcb, opts)
u32_t *lwip_hook_tcp_out_add_tcpopts(struct pbuf *p, struct tcp_hdr *hdr, const struct tcp_pcb *pcb, u32_t *opts);

#elif __ZEPHYR__

#endif

#endif
