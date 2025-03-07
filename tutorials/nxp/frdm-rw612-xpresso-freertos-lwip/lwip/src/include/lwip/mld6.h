/**
 * @file
 *
 * Multicast listener discovery v2 for IPv6. Aims to be compliant with RFC 3810.
 * No support for Source Specific Multicast.
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
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
 * Author: Ivan Delamer <delamer@inicotech.com>
 *
 *
 * Please coordinate changes and requests with Ivan Delamer
 * <delamer@inicotech.com>
 */

#ifndef LWIP_HDR_MLD6_H
#define LWIP_HDR_MLD6_H

#include "lwip/opt.h"

#if LWIP_IPV6_MLD && LWIP_IPV6  /* don't build if not configured for use in lwipopts.h */

#include "lwip/pbuf.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

/** MLD group */
struct mld_group {
  /** next link */
  struct mld_group *next;
  /** next link for list of pending change reports */
  struct mld_group *to_report_next;
  /** multicast address */
  ip6_addr_t group_address;
  /** counter of simultaneous uses */
  u8_t use;
  /** count of change reports to send */
  u8_t chg_reps_to_send;
};

/** MLD data per netif */
struct mld_data {
    /** linked list of subscribed groups */
    struct mld_group *groups;
    /** linked list of groups to report */
    struct mld_group *groups_to_report;
    /** robustness variable (how many times each change shoud be sent) default 2 */
    u8_t robustness;
    /** Ticks remaining to send next change report */
    u32_t chg_report_tmr;
    /** Time to response to query. Default 10 s  max possible value 8 387 584 s */
    u32_t max_response_delay;
    /** Ticks remainig to send next general report */
    u32_t general_report_tmr;
};

#define MLD6_TMR_INTERVAL              100 /* Milliseconds */

err_t  mld6_stop(struct netif *netif);
void   mld6_report_groups(struct netif *netif);
void   mld6_tmr(void);
struct mld_group *mld6_lookfor_group(struct netif *ifp, const ip6_addr_t *addr);
void   mld6_input(struct pbuf *p, struct netif *inp);
err_t  mld6_joingroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr);
err_t  mld6_joingroup_netif(struct netif *netif, const ip6_addr_t *groupaddr);
err_t  mld6_leavegroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr);
err_t  mld6_leavegroup_netif(struct netif *netif, const ip6_addr_t *groupaddr);

/** @ingroup mld6
 * Get list head of MLD6 groups for netif.
 * Note: The allnodes group IP is NOT in the list, since it must always
 * be received for correct IPv6 operation.
 * @see @ref netif_set_mld_mac_filter()
 */
#define netif_mld6_data(netif) ((struct mld_data *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_MLD6))

#ifdef __cplusplus
}
#endif

#endif /* LWIP_IPV6_MLD && LWIP_IPV6 */

#endif /* LWIP_HDR_MLD6_H */
