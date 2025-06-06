/** @file lwip_default_hook.c
 *
 *  @brief  This file provides lwip porting code
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <osa.h>

#if defined(SDK_OS_FREE_RTOS)

#include "lwip/tcpip.h"
#include "lwip/prot/tcp.h"
#include "lwip/tcp.h"
#include "lwip_default_hooks.h"
#include "wlan.h"

struct netif *lwip_hook_ip4_route_src(const ip4_addr_t *src, const ip4_addr_t *dest)
{
    struct netif *netif = NULL;

    if (src == NULL)
    {
        return NULL;
    }

    /* iterate through netifs */
    NETIF_FOREACH(netif)
    {
        /* is the netif up, does it have a link and a valid address? */
        if (netif_is_up(netif) && netif_is_link_up(netif) && !ip4_addr_isany_val(*netif_ip4_addr(netif)))
        {
            /*netif ip4 address matches bind_address*/
            if (ip4_addr_eq(src, netif_ip4_addr(netif)))
            {
                return netif;
            }
        }
    }

    return NULL;
}

#if CONFIG_CLOUD_KEEP_ALIVE
u32_t *lwip_hook_tcp_out_add_tcpopts(struct pbuf *p, struct tcp_hdr *hdr, const struct tcp_pcb *pcb, u32_t *opts)
{
    t_u16 source_port;
    t_u16 destination_port;
    t_u32 seq_number;
    t_u32 ack_number;

    source_port      = hdr->src;
    destination_port = hdr->dest;
    t_u32 hdr_len    = TCPH_HDRLEN_BYTES(hdr);
    seq_number       = ntohl(hdr->seqno) + p->tot_len - hdr_len;
    /* sequence number of the keep alive packet = the expected sequence number of next packet -1 */
    seq_number = seq_number - 1;
    seq_number = ntohl(seq_number);
    ack_number = hdr->ackno;

    wlan_save_cloud_keep_alive_params(NULL, source_port, destination_port, seq_number, ack_number, 0);

    return opts;
}
#else
u32_t *lwip_hook_tcp_out_add_tcpopts(struct pbuf *p, struct tcp_hdr *hdr, const struct tcp_pcb *pcb, u32_t *opts)
{
	return opts;
}
#endif
#endif

