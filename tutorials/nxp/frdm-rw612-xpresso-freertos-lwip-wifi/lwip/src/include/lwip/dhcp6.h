/**
 * @file
 *
 * DHCPv6 client: IPv6 address autoconfiguration as per
 * RFC 3315 (stateful DHCPv6) and
 * RFC 3736 (stateless DHCPv6).
 */

/*
 * Copyright (c) 2018 Simon Goldschmidt
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Simon Goldschmidt <goldsimon@gmx.de>
 */

#ifndef LWIP_HDR_IP6_DHCP6_H
#define LWIP_HDR_IP6_DHCP6_H

#include "lwip/opt.h"

#if LWIP_IPV6_DHCP6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/err.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/** period (in milliseconds) of the application calling dhcp6_tmr() */
#define DHCP6_TIMER_MSECS   500

#if LWIP_IPV6_DHCP6_STATEFUL
void dhcp6_set_client_id_duid(struct netif *netif, const void *duid, u16_t duid_len);
#endif

#if LWIP_IPV6_DHCP6_PD
struct dhcp6_delegated_prefix
{
  ip6_addr_t prefix;
  u8_t prefix_len;
  u32_t prefix_pref; /* Prefix preferred lifetime [s] */
  u32_t prefix_valid; /* Prefix valid lifetime [s] */
};

typedef void (*dhcp6_pd_callback_fn)(struct netif * netif, const struct dhcp6_delegated_prefix * prefix, u8_t valid);
void dhcp6_register_pd_callback(struct netif *netif, dhcp6_pd_callback_fn callback);
const struct dhcp6_delegated_prefix * dhcp6_get_delegated_prefix(struct netif * netif);
#endif

struct dhcp6
{
  /** transaction identifier of last sent request */
  u32_t xid;
  /** track PCB allocation state */
  u8_t pcb_allocated;
  /** current DHCPv6 state machine state */
  u8_t state;
  /** State after releasing of address is done */
  u8_t state_after_release;
  /** retries of current request */
  u8_t tries;
  /** #ticks with period DHCP6_TIMER_MSECS for request timeout */
  u16_t request_timeout_cntr;
  u16_t request_timeout;

  /** upper bound of request timeout in #ticks */
  u16_t max_request_timeout;

#if LWIP_IPV6_DHCP6_STATEFUL
  /** Maximal count of retransmissions */
  u8_t max_retrans_cnt;
    /** Maximal duration of retransmissions */
  u16_t max_retrans_dur;

  u32_t trans_started_at;
  void *server_id; /* Dynamically allocated mem with server id including its option header */
  u8_t server_id_len; /* Length of server id including its option header */

  const void *cli_id_usr_duid; /* Client ID DUID supplied by user */
  u16_t cli_id_usr_duid_len; /* Lenght of client ID DUID supplied by user */

  ip6_addr_t addr;
  s8_t  addr_idx;
  u32_t addr_pref;
  u32_t addr_valid;
  u32_t addr_t1;
  u32_t addr_t2;

  u8_t addr_by_slaac;

#endif /* LWIP_IPV6_DHCP6_STATEFUL */

#if LWIP_IPV6_DHCP6_PD
  struct dhcp6_delegated_prefix prefix;
  u32_t ia_pd_t1;
  u32_t ia_pd_t2;

  dhcp6_pd_callback_fn pd_callback;
#endif /* LWIP_IPV6_DHCP6_PD  */
};

void dhcp6_set_struct(struct netif *netif, struct dhcp6 *dhcp6);
/** Remove a struct dhcp6 previously set to the netif using dhcp6_set_struct() */
#define dhcp6_remove_struct(netif) netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6, NULL)
void dhcp6_cleanup(struct netif *netif);

err_t dhcp6_enable(struct netif *netif);
void dhcp6_disable(struct netif *netif);

void dhcp6_tmr(void);

void dhcp6_nd6_ra_trigger(struct netif *netif, u8_t managed_addr_config, u8_t other_config);

#if LWIP_DHCP6_GET_NTP_SRV
/** This function must exist, in other to add offered NTP servers to
 * the NTP (or SNTP) engine.
 * See LWIP_DHCP6_MAX_NTP_SERVERS */
extern void dhcp6_set_ntp_servers(u8_t num_ntp_servers, const ip_addr_t* ntp_server_addrs);
#endif /* LWIP_DHCP6_GET_NTP_SRV */

#define netif_dhcp6_data(netif) ((struct dhcp6*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6))

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6_DHCP6 */

#endif /* LWIP_HDR_IP6_DHCP6_H */
