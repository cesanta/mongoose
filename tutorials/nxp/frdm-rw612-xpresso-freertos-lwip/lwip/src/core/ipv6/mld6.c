/**
 * @file
 * Multicast listener discovery
 *
 * @defgroup mld6 MLD6
 * @ingroup ip6
 * Multicast listener discovery v2 for IPv6. Aims to be compliant with RFC 3810.
 * No support for Source Specific Multicast.<br>
 * Note: The allnodes (ff01::1, ff02::1) group is assumed be received by your
 * netif since it must always be received for correct IPv6 operation (e.g. SLAAC).
 * Ensure the netif filters are configured accordingly!<br>
 * The netif flags also need NETIF_FLAG_MLD6 flag set to enable MLD6 on a
 * netif ("netif->flags |= NETIF_FLAG_MLD6;").<br>
 * To be called from TCPIP thread.
 */

/*
 * Copyright (c) 2010 Inico Technologies Ltd.
 * Copyright 2017, 2023 NXP
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

/* Based on igmp.c implementation of igmp v2 protocol */

#include "lwip/opt.h"

#if LWIP_IPV6 && LWIP_IPV6_MLD  /* don't build if not configured for use in lwipopts.h */

#include "lwip/mld6.h"
#include "lwip/prot/mld6.h"
#include "lwip/icmp6.h"
#include "lwip/ip6.h"
#include "lwip/ip6_addr.h"
#include "lwip/ip.h"
#include "lwip/inet_chksum.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "lwip/memp.h"
#include "lwip/stats.h"

#include <string.h>


/*
 * MLD constants
 */
/* Hop limit in reports */
#define MLD6_HL                    1

/* Default Maximum Response Delay [s] */
#define MLD6_DEFAULT_MRD           10U

/* Default value of Robustness variable */
#define MLD6_DEFAULT_ROBUSTNESS    2U

/* Unsolicited Report Interval */
#define MLD6_UNSOL_REPORT_INTERVAL 1000U

static const ip_addr_t all_MLDv2_capable_routers = IPADDR6_INIT_HOST(0xFF020000, 0, 0, 0x00000016);

enum mld_filter_mode {
    MODE_IS_INCLUDE = 1,
    MODE_IS_EXCLUDE = 2,
    MODE_CHANGE_TO_INCLUDE = 3,
    MODE_CHANGE_TO_EXCLUDE = 4
};

/* Forward declarations. */
static struct mld_group *mld6_new_group(struct netif *netif, const ip6_addr_t *addr);
static void mld6_report_change(struct netif *netif, struct mld_group *group);
static void mld6_set_max_response_delay(struct mld_data *mld, u16_t max_response_code);
static void mld6_set_general_report_tmr(struct mld_data *mld);
static void mld6_send(struct netif *netif, struct mld_group *group, u8_t send_change_report);

static struct
mld_data *mld6_get_data(struct netif *netif) {
  return netif_mld6_data(netif);
}

static struct
mld_data *mld6_get_or_alloc_data(struct netif *netif) {
  struct mld_data *data = netif_mld6_data(netif);

  if (data == NULL) {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("Mallocing new MLDv6 struct for %%%u\n", netif_get_index(netif)));
    data = (struct mld_data *)mem_malloc(sizeof(struct mld_data));
    LWIP_ASSERT("Allocation of mld data failed", data != NULL);
    if (data) {
      netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_MLD6, data);
      data->groups = NULL;
      data->groups_to_report = NULL;
      data->general_report_tmr = 0;
      data->general_report_tmr = 0;
      data->max_response_delay = MLD6_DEFAULT_MRD;
      data->robustness = MLD6_DEFAULT_ROBUSTNESS;
    }
  }
  return data;
}

/**
 * Stop MLD processing on interface
 *
 * @param netif network interface on which stop MLD processing
 */
err_t
mld6_stop(struct netif *netif)
{
  if (netif_mld6_data(netif)) {
    struct mld_data *mld = mld6_get_data(netif);
    struct mld_group *group;

    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("mld6_stop(%%%u)\n", netif_get_index((netif))));

    if (mld == NULL) {
      return ERR_OK;
    }

    netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_MLD6, NULL);

    /* First free groups which are already left, so they aren't in mld->groups list and don't have set mac filter but
     * have pending leave change report. Those are in mld->groups_to_report and have use = 0 */
    for (group = mld->groups_to_report; group != NULL;) {
      struct mld_group *next = group->to_report_next;

      if (group->use == 0) {
        memp_free(MEMP_MLD6_GROUP, group);
      }

      group = next;
    }

    /* remove MAC filter and free all joined groups */
    for (group = mld->groups; group != NULL; ) {
      struct mld_group *next = group->next; /* avoid use-after-free below */

      /* disable the group at the MAC level */
      if (netif->mld_mac_filter != NULL) {
        netif->mld_mac_filter(netif, &(group->group_address), NETIF_DEL_MAC_FILTER);
      }

      /* free group */
      memp_free(MEMP_MLD6_GROUP, group);

      /* move to "next" */
      group = next;
    }

    mem_free(mld);

  }
  return ERR_OK;
}

/**
 * Report MLD memberships change for all subscribed groups.
 *
 * This is called on up/down, addr change...
 *
 * @param netif network interface on which report MLD memberships
 */
void
mld6_report_groups(struct netif *netif)
{
  struct mld_data *mld = mld6_get_data(netif);

  LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("mld6_report_groups(%%%u)\n", netif_get_index((netif))));

  if (mld != NULL) {
    struct mld_group *g;

    for (g = mld->groups; g != NULL; g = g->next) {
      mld6_report_change(netif, g);
    }
  }
}

/**
 * Search for a group that is joined on a netif
 *
 * @param netif the network interface for which to look
 * @param addr the group ipv6 address to search for
 * @return a struct mld_group* if the group has been found,
 *         NULL if the group wasn't found.
 */
struct mld_group *
mld6_lookfor_group(struct netif *netif, const ip6_addr_t *addr)
{
  struct mld_data *mld = mld6_get_data(netif);

  if (mld != NULL) {
    struct mld_group *group = mld->groups;

    while (group != NULL) {
      if (ip6_addr_eq(&(group->group_address), addr)) {
        return group;
      }
      group = group->next;
    }
  }

  return NULL;
}

/**
 * create a new group
 *
 * @param netif the network interface for which to create
 * @param addr the new group ipv6
 * @return a struct mld_group*,
 *         NULL on memory error.
 */
static struct mld_group *
mld6_new_group(struct netif *netif, const ip6_addr_t *addr)
{
  struct mld_group *group;
  struct mld_data *mld = mld6_get_or_alloc_data(netif);

  /* Try to find group in list to report */
  for (group = mld->groups_to_report; group != NULL; group = group->to_report_next) {
    if ((group->use == 0) && ip6_addr_eq(&(group->group_address), addr)) {
      break;
    }
  }

  if (group == NULL) {
    /* group not found - allocate new */
    group = (struct mld_group *)memp_malloc(MEMP_MLD6_GROUP);

    if (group == NULL) {
	  return NULL;
    }

    ip6_addr_set(&(group->group_address), addr);
    group->to_report_next = NULL;
    group->chg_reps_to_send = 0;
  }

  group->use  = 1;
  group->next = mld->groups;
  mld->groups = group;

  return group;
}

/**
 * Process an input MLD query message. Called by icmp6_input.
 *
 * @param p the mld packet, p->payload pointing to the icmpv6 header
 * @param inp the netif on which this packet was received
 */
void
mld6_input(struct pbuf *p, struct netif *inp) {
  const struct mld_header_query *mld_hdr;
  struct mld_data *mld = mld6_get_data(inp);

  MLD6_STATS_INC(mld6.recv);

  LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("Got MLD query from %x:%x:%x:%x:%x:%x:%x:%x %%%u\n",
          IP6_ADDR_BLOCK1(ip6_current_src_addr()),
          IP6_ADDR_BLOCK2(ip6_current_src_addr()),
          IP6_ADDR_BLOCK3(ip6_current_src_addr()),
          IP6_ADDR_BLOCK4(ip6_current_src_addr()),
          IP6_ADDR_BLOCK5(ip6_current_src_addr()),
          IP6_ADDR_BLOCK6(ip6_current_src_addr()),
          IP6_ADDR_BLOCK7(ip6_current_src_addr()),
          IP6_ADDR_BLOCK8(ip6_current_src_addr()),
          netif_get_index(inp)
  ));

  if (mld == NULL) {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tDropping - No join called on this if so far\n"));
    pbuf_free(p);
    return;
  }

  /* Check that mld header fits in packet. */
  if (p->len < sizeof(struct mld_header_query)) {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\t Dropping, too short.\n"));
    pbuf_free(p);
    MLD6_STATS_INC(mld6.lenerr);
    MLD6_STATS_INC(mld6.drop);
    return;
  }

  mld_hdr = (struct mld_header_query *) p->payload;

  /* Src addr must be link local or any */
  if (!(ip6_addr_islinklocal(ip6_current_src_addr()) || ip6_addr_isany(ip6_current_src_addr())) ) {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tDropping, src addr is not link local nor any.\n"));
    MLD6_STATS_INC(mld6.drop);
    pbuf_free(p);
    return;
  }

  /* Hop limit must be 1 */
  if ((IP6H_HOPLIM(ip6_current_header()) != 1)) {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tDropping, hop limit != 1\n"));
    MLD6_STATS_INC(mld6.drop);
    pbuf_free(p);
    return;
  }

  /** @todo we should check existence of hop-by-hop options but this is processed in icmp6_input */

  mld6_set_max_response_delay(mld, mld_hdr->max_resp_code);
  if ((mld_hdr->s_qrv & 0x7) > 0) {
    /* If non-zero, the QRV field contains the [Robustness Variable] value
       used by the Querier.  If the Querier's [Robustness Variable] exceeds
       7 (the maximum value of the QRV field), the QRV field is set to zero. */

    mld->robustness = (mld_hdr->s_qrv & 0x7);
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tRobustness set to %u.\n", mld->robustness));
  }

  /* Is it general query? */
  if (ip6_addr_isany(&mld_hdr->multicast_address)) {
    MLD6_STATS_INC(mld6.rx_general);
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tgeneral query.\n"));
    mld6_set_general_report_tmr(mld);
  } else {
    /* Query for specific group */
    ip6_addr_t addr;
    struct mld_group *group;
    MLD6_STATS_INC(mld6.rx_group);
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tquery for %x:%x:%x:%x:%x:%x:%x:%x\n",
          IP6_ADDR_BLOCK1(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK2(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK2(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK3(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK4(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK5(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK6(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK7(&mld_hdr->multicast_address),
          IP6_ADDR_BLOCK8(&mld_hdr->multicast_address)
          ));

    ip6_addr_copy_from_packed(addr, mld_hdr->multicast_address);

    /* Not using IP6_MULTICAST because we can not be sure what address we get in request */
    ip6_addr_assign_zone(&addr, IP6_UNKNOWN, inp);

    if (mld_hdr->number_of_sources != 0) {
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\t %u src addrs in query. Ignoring them. SSM is not supported.\n",
                                                mld_hdr->number_of_sources));
      (void)0;
    }

    group = mld6_lookfor_group(inp, &addr);
    if (group != NULL) {
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tsending response.\n"));
      mld6_send(inp, group, 0);
    } else {
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\twe are not listening to this addr.\n"));
      (void)0;
    }

  }

  pbuf_free(p);
}

/**
 * @ingroup mld6
 * Join a group on one or all network interfaces.
 *
 * If the group is to be joined on all interfaces, the given group address must
 * not have a zone set (i.e., it must have its zone index set to IP6_NO_ZONE).
 * If the group is to be joined on one particular interface, the given group
 * address may or may not have a zone set.
 *
 * @param srcaddr ipv6 address (zoned) of the network interface which should
 *                join a new group. If IP6_ADDR_ANY6, join on all netifs
 * @param groupaddr the ipv6 address of the group to join (possibly but not
 *                  necessarily zoned)
 * @return ERR_OK if group was joined on the netif(s), an err_t otherwise
 */
err_t
mld6_joingroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr)
{
  err_t         err = ERR_VAL; /* no matching interface */
  struct netif *netif;

  LWIP_ASSERT_CORE_LOCKED();

  /* loop through netif's */
  NETIF_FOREACH(netif) {
    /* Should we join this interface ? */
    if (ip6_addr_isany(srcaddr) ||
        netif_get_ip6_addr_match(netif, srcaddr) >= 0) {
      err = mld6_joingroup_netif(netif, groupaddr);
      if (err != ERR_OK) {
        return err;
      }
    }
  }

  return err;
}

/**
 * @ingroup mld6
 * Join a group on a network interface.
 *
 * @param netif the network interface which should join a new group.
 * @param groupaddr the ipv6 address of the group to join (possibly but not
 *                  necessarily zoned)
 * @return ERR_OK if group was joined on the netif, an err_t otherwise
 */
err_t
mld6_joingroup_netif(struct netif *netif, const ip6_addr_t *groupaddr)
{
  struct mld_group *group;
#if LWIP_IPV6_SCOPES
  ip6_addr_t ip6addr;

  /* If the address has a particular scope but no zone set, use the netif to
   * set one now. Within the mld6 module, all addresses are properly zoned. */
  if (ip6_addr_lacks_zone(groupaddr, IP6_MULTICAST)) {
    ip6_addr_set(&ip6addr, groupaddr);
    ip6_addr_assign_zone(&ip6addr, IP6_MULTICAST, netif);
    groupaddr = &ip6addr;
  }
  IP6_ADDR_ZONECHECK_NETIF(groupaddr, netif);
#endif /* LWIP_IPV6_SCOPES */

  LWIP_ASSERT_CORE_LOCKED();

  /* find group or create a new one if not found */
  group = mld6_lookfor_group(netif, groupaddr);

  if (group == NULL) {
    /* Joining a new group. Create a new group entry. */
    group = mld6_new_group(netif, groupaddr);
    if (group == NULL) {
      return ERR_MEM;
    }

    /* Activate this address on the MAC layer. */
    if (netif->mld_mac_filter != NULL) {
      netif->mld_mac_filter(netif, groupaddr, NETIF_ADD_MAC_FILTER);
    }

    /* Report our membership. */
    MLD6_STATS_INC(mld6.tx_report);
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("mld6_joingroup_netif(%x:%x:%x:%x:%x:%x:%x:%x %%%u)\n",
            IP6_ADDR_BLOCK1(&group->group_address),
            IP6_ADDR_BLOCK2(&group->group_address),
            IP6_ADDR_BLOCK3(&group->group_address),
            IP6_ADDR_BLOCK4(&group->group_address),
            IP6_ADDR_BLOCK5(&group->group_address),
            IP6_ADDR_BLOCK6(&group->group_address),
            IP6_ADDR_BLOCK7(&group->group_address),
            IP6_ADDR_BLOCK8(&group->group_address),
            netif_get_index(netif)
    ));
    mld6_report_change(netif, group);
  } else {
    /* Just increment group use */
    group->use++;
  }

  return ERR_OK;
}

/**
 * @ingroup mld6
 * Leave a group on a network interface.
 *
 * Zoning of address follows the same rules as @ref mld6_joingroup.
 *
 * @param srcaddr ipv6 address (zoned) of the network interface which should
 *                leave the group. If IP6_ADDR_ANY6, leave on all netifs
 * @param groupaddr the ipv6 address of the group to leave (possibly, but not
 *                  necessarily zoned)
 * @return ERR_OK if group was left on the netif(s), an err_t otherwise
 */
err_t
mld6_leavegroup(const ip6_addr_t *srcaddr, const ip6_addr_t *groupaddr)
{
  err_t         err = ERR_VAL; /* no matching interface */
  struct netif *netif;

  LWIP_ASSERT_CORE_LOCKED();

  /* loop through netif's */
  NETIF_FOREACH(netif) {
    /* Should we leave this interface ? */
    if (ip6_addr_isany(srcaddr) ||
        netif_get_ip6_addr_match(netif, srcaddr) >= 0) {
      err_t res = mld6_leavegroup_netif(netif, groupaddr);
      if (err != ERR_OK) {
        /* Store this result if we have not yet gotten a success */
        err = res;
      }
    }
  }

  return err;
}

/**
 * @ingroup mld6
 * Leave a group on a network interface.
 *
 * @param netif the network interface which should leave the group.
 * @param groupaddr the ipv6 address of the group to leave (possibly, but not
 *                  necessarily zoned)
 * @return ERR_OK if group was left on the netif, an err_t otherwise
 */
err_t
mld6_leavegroup_netif(struct netif *netif, const ip6_addr_t *groupaddr)
{
  struct mld_group *group;
#if LWIP_IPV6_SCOPES
  ip6_addr_t ip6addr;

  if (ip6_addr_lacks_zone(groupaddr, IP6_MULTICAST)) {
    ip6_addr_set(&ip6addr, groupaddr);
    ip6_addr_assign_zone(&ip6addr, IP6_MULTICAST, netif);
    groupaddr = &ip6addr;
  }
  IP6_ADDR_ZONECHECK_NETIF(groupaddr, netif);
#endif /* LWIP_IPV6_SCOPES */

  LWIP_ASSERT_CORE_LOCKED();

  /* find group */
  group = mld6_lookfor_group(netif, groupaddr);

  if (group != NULL) {
    /* Decrement group use */
    group->use--;

    /* Leave if there is no other use of the group */
    if (group->use == 0) {
      struct mld_data *mld = mld6_get_data(netif); /* lookfor_grp will return NULL if mld is NULL */
      struct mld_group *prev = NULL;
      struct mld_group *g = NULL;

      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("mld6_leavegroup_netif(%x:%x:%x:%x:%x:%x:%x:%x %%%u)\n",
              IP6_ADDR_BLOCK1(&group->group_address),
              IP6_ADDR_BLOCK2(&group->group_address),
              IP6_ADDR_BLOCK3(&group->group_address),
              IP6_ADDR_BLOCK4(&group->group_address),
              IP6_ADDR_BLOCK5(&group->group_address),
              IP6_ADDR_BLOCK6(&group->group_address),
              IP6_ADDR_BLOCK7(&group->group_address),
              IP6_ADDR_BLOCK8(&group->group_address),
              netif_get_index(netif)
      ));
      mld6_report_change(netif, group);

      group->use = 0;
      /* Remove the group from the list */
      for (g = mld->groups; g != group; g = g->next) {
        prev = g;
      }

      if (prev == NULL) {
        mld->groups = group->next;
      } else {
        prev->next = group->next;
      }

      MLD6_STATS_INC(mld6.tx_leave);

      /* Disable the group at the MAC level */
      if (netif->mld_mac_filter != NULL) {
        netif->mld_mac_filter(netif, groupaddr, NETIF_DEL_MAC_FILTER);
      }
    }

    /* Left group */
    return ERR_OK;
  }

  /* Group not found */
  return ERR_VAL;
}

/**
 * Sets change report send time to interval from MLD6_TMR_INTERVAL to 1 s
 *
 * @param mld mld data from netif
 */
static void
mld6_set_change_report_tmr(struct mld_data *mld) {
  LWIP_ASSERT("mld->groups_to_report != NULL", mld->groups_to_report != NULL);
  LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("set_change_report_tmr: "));
  if (mld->chg_report_tmr == 0) {
    /* Set time if not running */
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("set\n"));
    mld->chg_report_tmr = 1 + (LWIP_RAND() % ((MLD6_UNSOL_REPORT_INTERVAL / MLD6_TMR_INTERVAL) - 1));
  } else {
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("already running\n"));
  }
}

/**
 * Sets maximum response delay as specified in rfc3810 section 5.1.3
 * @code
 * If Maximum Response Code < 32768,
 *    Maximum Response Delay = Maximum Response Code
 *
 * If Maximum Response Code >=32768, Maximum Response Code represents a
 * floating-point value as follows:
 *
 *     0 1 2 3 4 5 6 7 8 9 A B C D E F
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |1| exp |          mant         |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Maximum Response Delay = (mant | 0x1000) << (exp+3)
 * @endcode
 *
 * @param mld  mld data from netif
 * @param max_response_code maximum response code (in network endian)
 */
static void
mld6_set_max_response_delay(struct mld_data *mld, u16_t max_response_code) {
  u16_t mrc_h = ntohs(max_response_code);
  if (mrc_h < 32768u) {
    mld->max_response_delay = mrc_h;
  } else {
    u16_t mant = mrc_h & 0x0fffU;
    u16_t exp = (mrc_h >> 12) & 0x07U;

    mld->max_response_delay = (mant | 0x1000) << (exp+3);
  }
}

/**
 * Sets general report send time to interval from MLD6_TMR_INTERVAL to maximum response delay time from report.
 *
 * @param mld mld data from netif
 */
static void
mld6_set_general_report_tmr(struct mld_data *mld) {
  if (mld->general_report_tmr == 0) {
    /* Timer not running - set time */
    mld->general_report_tmr = 1 + (LWIP_RAND() % ((mld->max_response_delay / MLD6_TMR_INTERVAL) - 1));
  } else {
    /* Crop timer if new maximum is smaller than current remaining time */
    const u16_t mrd_ticks = mld->max_response_delay / MLD6_TMR_INTERVAL;
    if (mld->general_report_tmr > mrd_ticks) {
      mld->general_report_tmr = mrd_ticks;
    }
  }
}


/**
 * Periodic timer for mld processing. Must be called every
 * MLD6_TMR_INTERVAL milliseconds (100).
 *
 * Sends change report if scheduled.
 */
void
mld6_tmr(void) {
  struct netif *netif;

  NETIF_FOREACH(netif) {
    struct mld_data *mld = mld6_get_data(netif);
    if (mld != NULL) {
      if (mld->chg_report_tmr > 1) {
        mld->chg_report_tmr--;
      } else if (mld->chg_report_tmr == 1) {
        /* send change report */
        mld->chg_report_tmr = 0;

        mld6_send(netif, NULL, 1);

        if (mld->groups_to_report != NULL) {
          /* There is still some groups to send set timer again */
          mld6_set_change_report_tmr(mld);
        }
      }

      if (mld->general_report_tmr > 1) {
        mld->general_report_tmr--;
      } else if (mld->general_report_tmr == 1) {
        /* send general report */
        mld->general_report_tmr = 0;
        mld6_send(netif, NULL, 0);
      }
    }
  } /* NETIF_FOREACH */
}

/**
 * Puts group into into list of changed groups and sends first report.
 *
 * Be sure to set use variable of group prior calling this.
 *
 */
static void
mld6_report_change(struct netif *netif, struct mld_group *group)
{
  struct mld_group *g;
  struct mld_data *mld = mld6_get_data(netif);

  for (g = mld->groups_to_report; (g != NULL) && (g != group); g = g->to_report_next) {
  }

  if (g != group) {
    /* We are not in list add us*/
    group->to_report_next = mld->groups_to_report;
    mld->groups_to_report = group;
  }

  group->chg_reps_to_send = mld->robustness;

  mld6_set_change_report_tmr(mld);
}

/**
 * Fills the Multicast address record into packet.
 *
 * Filter mode means which particular source addresses we want to receive. This implementation
 * always sends empty list of sources so EXCLUDE (no source) means "receive everything" and
 * INCLUDE (no source) means "receive nothing" (leave group).
 *
 * @param group        group to fill in
 * @param addr_record  pointer to pbuf data.
 * @param filter_mode  desired filter modes defined in @see enum mld_filter_mode.
 */
static void
mld6_group_to_packet(const struct mld_group *group, struct mld_addr_record *addr_record, enum mld_filter_mode filter_mode)
{
  LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\tAdding grp %x:%x:%x:%x:%x:%x:%x:%x mode %s_%s\n",
          IP6_ADDR_BLOCK1(&group->group_address),
          IP6_ADDR_BLOCK2(&group->group_address),
          IP6_ADDR_BLOCK3(&group->group_address),
          IP6_ADDR_BLOCK4(&group->group_address),
          IP6_ADDR_BLOCK5(&group->group_address),
          IP6_ADDR_BLOCK6(&group->group_address),
          IP6_ADDR_BLOCK7(&group->group_address),
          IP6_ADDR_BLOCK8(&group->group_address),
          ((filter_mode-1) & 2) ? "TO" : "IS",
          ((filter_mode-1) & 1) ? "EXCL" : "INCL"
  ));

  addr_record->type = filter_mode;
  addr_record->aux_data_len = 0;
  addr_record->number_of_sources = 0; /* We don't support SSM now */
  ip6_addr_copy_to_packed(addr_record->multicast_address, group->group_address);
}

/**
 * Send a MLD message (report or done).
 *
 * An IPv6 hop-by-hop options header with a router alert option
 * is prepended.
 *
 * @param group single group to report or NULL to send change report or reply to general query
 * @param send_change_report True - sends change report, False - sends reply to general query.
 */
static void
mld6_send(struct netif *netif, struct mld_group *group, u8_t send_change_report)
{
  u8_t groups_to_send = 0;
  struct mld_group *g;
  const u16_t max_groups_in_pkt = (netif->mtu6 - (PBUF_IP + MLD6_HBH_HLEN + sizeof(struct mld_header_reply))) /
                                  sizeof(struct mld_addr_record);
  struct mld_data *mld = mld6_get_data(netif);

  if (group != NULL) {
    groups_to_send = 1;
  } else {
    if (send_change_report) {
      for (g = mld->groups_to_report; g != NULL; g = g->to_report_next) {
        groups_to_send++;
      }
    } else {
      /* Send general report */
      for (g = mld->groups; g != NULL; g = g->next) {
        groups_to_send++;
      }
    }
  }

  while (groups_to_send > 0) {
    struct mld_header_reply *mld_hdr;
    struct mld_addr_record *mld_addr;
    struct pbuf *p;
    const ip6_addr_t *src_addr;

    const u8_t groups_in_cur_pkt = LWIP_MIN(groups_to_send, max_groups_in_pkt);
    const u16_t pbuf_len = sizeof(struct mld_header_reply) + MLD6_HBH_HLEN +
                           (groups_in_cur_pkt * sizeof(struct mld_addr_record));

    groups_to_send -= groups_in_cur_pkt;


    /* Allocate a packet. Size is MLD header + IPv6 Hop-by-hop options header. */
    p = pbuf_alloc(PBUF_IP, pbuf_len, PBUF_RAM);

    if (p == NULL) {
      MLD6_STATS_INC(mld6.memerr);
      return;
    }

    /* Move to make room for Hop-by-hop options header. */
    if (pbuf_remove_header(p, MLD6_HBH_HLEN)) {
      pbuf_free(p);
      MLD6_STATS_INC(mld6.lenerr);
      return;
    }

    /* Select our source address. */
    if (!ip6_addr_isvalid(netif_ip6_addr_state(netif, 0))) {
      /* This is a special case, when we are performing duplicate address detection.
       * We must join the multicast group, but we don't have a valid address yet. */
      src_addr = IP6_ADDR_ANY6;
    } else {
      /* Use link-local address as source address. */
      src_addr = netif_ip6_addr(netif, 0);
    }

    /* MLD message header pointer. */
    mld_hdr = (struct mld_header_reply *) p->payload;

    /* Set fields. */
    mld_hdr->type = ICMP6_TYPE_ML2R;
    mld_hdr->reserved = 0;
    mld_hdr->chksum = 0;
    mld_hdr->reserved2 = 0;
    mld_hdr->nr_address_records = htons(groups_in_cur_pkt);

    mld_addr = (struct mld_addr_record *) &mld_hdr[1];

    if (group != NULL) {
      /* reply to single group query */
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("Preparing single grp report:\n"));
      mld6_group_to_packet(group, mld_addr, MODE_IS_EXCLUDE);
    } else if (send_change_report) {
      struct mld_group *prev = NULL;
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("Preparing change report (%u grps):\n", groups_in_cur_pkt));

      for (g = mld->groups_to_report; g != NULL; ) {
        struct mld_group *next = g->to_report_next;
        enum mld_filter_mode mode = (g->use == 0) ? MODE_CHANGE_TO_INCLUDE : MODE_CHANGE_TO_EXCLUDE;

        mld6_group_to_packet(g, mld_addr, mode);
        mld_addr++;

        g->chg_reps_to_send--;
        if (g->chg_reps_to_send <= 0) {
          /* All resends done - de-schedule */

          LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\t(descheduling)\n", mld->chg_report_tmr));

          if (prev == NULL) {
            mld->groups_to_report = g->to_report_next;
          } else {
            prev->to_report_next = g->to_report_next;
          }

          if (g->use == 0) {
            memp_free(MEMP_MLD6_GROUP, g);
          }
          /* On removal previous group remains the same. */
          g = prev;
        }

        prev = g;
        g = next;
      }
    } else {
      /* Send general report */
      LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("Preparing general report (%u grps):\n", groups_in_cur_pkt));
      for (g = mld->groups; g != NULL; g = g->next) {
        mld6_group_to_packet(g, mld_addr, MODE_IS_EXCLUDE);
        mld_addr++;
      }
    }

#if CHECKSUM_GEN_ICMP6
    IF__NETIF_CHECKSUM_ENABLED(netif, NETIF_CHECKSUM_GEN_ICMP6) {
      mld_hdr->chksum = ip6_chksum_pseudo(p, IP6_NEXTH_ICMP6, p->len,
                                          src_addr, ip_2_ip6(&all_MLDv2_capable_routers));
    }
#endif /* CHECKSUM_GEN_ICMP6 */

    /* Add hop-by-hop headers options: router alert with MLD value. */
    ip6_options_add_hbh_ra(p, IP6_NEXTH_ICMP6, IP6_ROUTER_ALERT_VALUE_MLD);

    /* Send the packet out. */
    MLD6_STATS_INC(mld6.xmit);
    ip6_output_if(p, (ip6_addr_isany(src_addr)) ? NULL : src_addr, ip_2_ip6(&all_MLDv2_capable_routers),
                  MLD6_HL, 0, IP6_NEXTH_HOPBYHOP, netif);
    LWIP_DEBUGF(MLD6_DEBUG | LWIP_DBG_TRACE, ("\t\t Report sent from netif %u\n", netif_get_index(netif)));
    pbuf_free(p);
  } /* while (groups_to_send > 0) */
}

#endif /* LWIP_IPV6 */
