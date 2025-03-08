/**
 * @file
 *
 * @defgroup dhcp6 DHCPv6
 * @ingroup ip6
 * DHCPv6 client: IPv6 address autoconfiguration as per
 * RFC 8415 (stateful DHCPv6 with prefix delegation) and
 * RFC 3736 (stateless DHCPv6).
 *
 * Limitations and features you should know about:
 *  - Solicitation is sent only on router advertisement reception
 *  - Client always picks first advertisement and continues with that server.
 *  - Rapid commit is not supported
 *  - Client is not keeping list of multiple potential servers so rebinding is
 *    not supported. Client will transit to solicitation state instead.
 *  - When server replies to renew with address or prefix with zero valid time
 *    client goes to solicitation state.
 *
 * dhcp6_enable() enables DHCPv6 for a netif <br>
 * dhcp6_disable() disable DHCPv6 for a netif
 *
 * define LWIP_IPV6_DHCP6_STATEFUL to 1 to support stateful client (Can give you an address using DHCP6)
 * define LWIP_IPV6_DHCP6_PD to 1 to request prefix form your router.
 *
 * When enabled, requests are only issued after receipt of RA with the
 * corresponding bits set.
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

#include "lwip/opt.h"

#if LWIP_IPV6 && LWIP_IPV6_DHCP6 /* don't build if not configured for use in lwipopts.h */

#include "lwip/dhcp6.h"
#include "lwip/prot/dhcp6.h"
#include "lwip/prot/iana.h"
#include "lwip/def.h"
#include "lwip/udp.h"
#include "lwip/dns.h"

#include <string.h>
#include <lwip/dhcp.h>
#include <lwip/sys.h>

#ifdef LWIP_HOOK_FILENAME
#include LWIP_HOOK_FILENAME
#endif

#if defined(LWIP_DHCP6_ORO_OPTS_CUSTOM) && !defined(LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM)
#warning "Custom option set in LWIP_DHCP6_ORO_OPTS_CUSTOM but missing definition of LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM to parse it"
#endif
#if !defined(LWIP_DHCP6_ORO_OPTS_CUSTOM) && defined(LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM)
#warning "LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM to parse registered to parse custom option but no option set in LWIP_DHCP6_ORO_OPTS_CUSTOM"
#endif

#ifdef LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM
extern void LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM(const struct netif * netif, u16_t type, u8_t state, const struct pbuf * p, u16_t offset, u16_t opt_len);
#endif


#if LWIP_DNS && LWIP_DHCP6_MAX_DNS_SERVERS
#if DNS_MAX_SERVERS > LWIP_DHCP6_MAX_DNS_SERVERS
#define LWIP_DHCP6_PROVIDE_DNS_SERVERS LWIP_DHCP6_MAX_DNS_SERVERS
#else
#define LWIP_DHCP6_PROVIDE_DNS_SERVERS DNS_MAX_SERVERS
#endif
#else
#define LWIP_DHCP6_PROVIDE_DNS_SERVERS 0
#endif

#define dhcp6_sec_to_ticks(sec) ((sec*1000)/DHCP6_TIMER_MSECS)

const ip_addr_t dhcp6_All_DHCP6_Relay_Agents_and_Servers = IPADDR6_INIT_HOST(0xFF020000, 0, 0, 0x00010002);
const ip_addr_t dhcp6_All_DHCP6_Servers = IPADDR6_INIT_HOST(0xFF020000, 0, 0, 0x00010003);

static struct udp_pcb *dhcp6_pcb;
static u8_t dhcp6_pcb_refcount;

static const u16_t requested_options[] = {
#if LWIP_DHCP6_PROVIDE_DNS_SERVERS
    DHCP6_OPTION_DNS_SERVERS,
#endif
#if LWIP_DHCP6_GET_NTP_SRV
    DHCP6_OPTION_SNTP_SERVERS,
#endif
#ifdef LWIP_DHCP6_ORO_OPTS_CUSTOM
  LWIP_DHCP6_ORO_OPTS_CUSTOM,
#endif
    0U /* end of list */
};


static void dhcp6_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
static void dhcp6_information_request(struct netif *netif, struct dhcp6 *dhcp6);
#if LWIP_IPV6_DHCP6_STATEFUL
static void dhcp6_solicitation(struct netif *netif, struct dhcp6 *dhcp6);
static void dhcp6_request(struct netif *netif, struct dhcp6 *dhcp6);
static void dhcp6_renew(struct netif *netif, struct dhcp6 *dhcp6);
static void dhcp6_rebind(struct netif *netif, struct dhcp6 *dhcp6);
static void dhcp6_release(struct netif *netif, struct dhcp6 *dhcp6);
static void set_addr_not_advertised(struct dhcp6 *dhcp6);
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

/** Ensure DHCP PCB is allocated and bound */
static err_t
dhcp6_inc_pcb_refcount(void)
{
  if (dhcp6_pcb_refcount == 0) {
    LWIP_ASSERT("dhcp6_inc_pcb_refcount(): memory leak", dhcp6_pcb == NULL);

    /* allocate UDP PCB */
    dhcp6_pcb = udp_new_ip6();

    if (dhcp6_pcb == NULL) {
      return ERR_MEM;
    }

    ip_set_option(dhcp6_pcb, SOF_BROADCAST);

    /* set up local and remote port for the pcb -> listen on all interfaces on all src/dest IPs */
    udp_bind(dhcp6_pcb, IP6_ADDR_ANY, DHCP6_CLIENT_PORT);
    udp_recv(dhcp6_pcb, dhcp6_recv, NULL);
  }

  dhcp6_pcb_refcount++;

  return ERR_OK;
}

/** Free DHCP PCB if the last netif stops using it */
static void
dhcp6_dec_pcb_refcount(void)
{
  LWIP_ASSERT("dhcp6_pcb_refcount(): refcount error", (dhcp6_pcb_refcount > 0));
  dhcp6_pcb_refcount--;

  if (dhcp6_pcb_refcount == 0) {
    udp_remove(dhcp6_pcb);
    dhcp6_pcb = NULL;
  }
}

/**
 * @ingroup dhcp6
 * Set a statically allocated struct dhcp6 to work with.
 * Using this prevents dhcp6_start to allocate it using mem_malloc.
 *
 * @param netif the netif for which to set the struct dhcp
 * @param dhcp6 (uninitialised) dhcp6 struct allocated by the application
 */
void
dhcp6_set_struct(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("dhcp6 != NULL", dhcp6 != NULL);
  LWIP_ASSERT("netif already has a struct dhcp6 set", netif_dhcp6_data(netif) == NULL);

  /* clear data structure */
  memset(dhcp6, 0, sizeof(struct dhcp6));
#if LWIP_IPV6_DHCP6_STATEFUL
  dhcp6->addr_idx = -1;
#endif
  netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6, dhcp6);
}

static struct dhcp6*
dhcp6_get_struct(struct netif *netif, const char *dbg_requester)
{
  struct dhcp6 *dhcp6 = netif_dhcp6_data(netif);
  if (dhcp6 == NULL) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("%s: mallocing new DHCPv6 client\n", dbg_requester));
    dhcp6 = (struct dhcp6 *)mem_malloc(sizeof(struct dhcp6));
    if (dhcp6 == NULL) {
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
                  ("%s: could not allocate dhcp6\n", dbg_requester));
      return NULL;
    }
    /* clear data structure, this implies DHCP6_STATE_OFF */
    memset(dhcp6, 0, sizeof(struct dhcp6));
#if LWIP_IPV6_DHCP6_STATEFUL
    dhcp6->addr_idx = -1;
#endif
    /* store this dhcp6 client in the netif */
    netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6, dhcp6);
  } else {
    /* already has DHCP6 client attached */
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("%s: using existing DHCPv6 client\n", dbg_requester));
  }

  if (!dhcp6->pcb_allocated) {
    if (dhcp6_inc_pcb_refcount() != ERR_OK) { /* ensure DHCP6 PCB is allocated */
      mem_free(dhcp6);
      netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6, NULL);
      return NULL;
    }
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("%s: dhcp6 successfully allocated\n", dbg_requester));
    dhcp6->pcb_allocated = 1;
  }
  return dhcp6;
}

static void
dhcp6_free_struct(struct netif *netif, const char *dbg_requester) {
  struct dhcp6 *dhcp6 = netif_dhcp6_data(netif);
  if (dhcp6 == NULL) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("%s: DHCPv6 client already freed\n", dbg_requester));
  } else {
#if LWIP_IPV6_DHCP6_STATEFUL
    if(dhcp6->server_id != NULL) {
      mem_free(dhcp6->server_id);
    }
#endif /* LWIP_IPV6_DHCP6_STATEFUL */
    mem_free(dhcp6);
    netif_set_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP6, NULL);
    dhcp6_dec_pcb_refcount();
  }
}

/*
 * Set the DHCPv6 state
 * If the state changed, reset the number of tries and set retransmission params
 * according RFC.
 */
static void
dhcp6_set_state(struct netif *netif, struct dhcp6 *dhcp6, u8_t new_state, const char *dbg_caller)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp6_set_state: %d -> %d (caller: %s)\n",
    dhcp6->state, new_state, dbg_caller));
  if (new_state != dhcp6->state) {
    dhcp6->state = new_state;
    dhcp6->tries = 0;
    dhcp6->request_timeout = 0;
#if LWIP_IPV6_DHCP6_STATEFUL
    dhcp6->max_request_timeout = 0;
    dhcp6->max_retrans_cnt = 0;
    dhcp6->max_retrans_dur = 0;
#endif

    switch (dhcp6->state) {

      case DHCP6_STATE_OFF:
        dhcp6_free_struct(netif, dbg_caller);
        /* dhcp6 struct doesn't exist here anymore. */
        return;

      case DHCP6_STATE_IDLE:
        break;

      case DHCP6_STATE_SL_REQUESTING_CONFIG:
        dhcp6_information_request(netif, dhcp6);
        dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_INF_TIMEOUT);
        dhcp6->max_request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_INF_MAX_RT);
        break;

#if LWIP_IPV6_DHCP6_STATEFUL
      case DHCP6_STATE_SF_INIT_DELAY:
        /* something random between 0 and SOL_MAX delay Fixme: randomize*/
        dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_SOL_MAX_DELAY)/2;
        break;

      case DHCP6_STATE_SF_SOLICITING:
        /* Set initial IA NA T1 and T2 preference to no preference */
        set_addr_not_advertised(dhcp6);
        dhcp6->addr_t1 = 0;
        dhcp6->addr_t2 = 0;
        dhcp6_solicitation(netif, dhcp6);
        dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_SOL_TIMEOUT);
        dhcp6->max_request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_SOL_MAX_RT);
        break;

      case DHCP6_STATE_SF_REQUESTING:
        dhcp6_request(netif, dhcp6);
        dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REQ_TIMEOUT);
        dhcp6->max_request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REQ_MAX_RT);
        dhcp6->max_retrans_cnt = DHCP6_PAR_REQ_MAX_RC;
        break;

      case DHCP6_STATE_SF_BOUND:
        dhcp6->request_timeout = 0;
        break;

      case DHCP6_STATE_SF_RENEWING:
          dhcp6_renew(netif, dhcp6);
          dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REN_TIMEOUT);
          dhcp6->max_request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REN_MAX_RT);
          break;

      case DHCP6_STATE_SF_REBINDING:
          dhcp6_rebind(netif, dhcp6);
          dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REB_TIMEOUT);
          dhcp6->max_request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REB_MAX_RT);
          break;

      case DHCP6_STATE_SF_RELEASING:
          dhcp6_release(netif, dhcp6);
          dhcp6->request_timeout = dhcp6_sec_to_ticks(DHCP6_PAR_REL_TIMEOUT);
          dhcp6->max_retrans_cnt = DHCP6_PAR_REL_MAX_RC;
          break;

#endif /* LWIP_IPV6_DHCP6_STATEFUL */
      default:
          LWIP_ASSERT("Unhandled state at dhcp6_set_state", 0);
    } /* switch */
    dhcp6->request_timeout_cntr = dhcp6->request_timeout;
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_set_state: setting request timeout to %"U16_F" ms\n",
                                                dhcp6->request_timeout * DHCP6_TIMER_MSECS));
  } /* if state changed */
}

/**
 * @ingroup dhcp6
 * Enable DHCPv6 on this netif
 * Requests are sent on receipt of an RA message.
 *
 * A struct dhcp6 will be allocated for this netif if not
 * set via @ref dhcp6_set_struct before.
 */
err_t
dhcp6_enable(struct netif *netif)
{
  struct dhcp6 *dhcp6;

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp6_enable_stateful(netif=%p) %c%c%"U16_F"\n",
              (void *)netif, netif->name[0], netif->name[1], (u16_t)netif->num));
  LWIP_ASSERT_CORE_LOCKED();

  dhcp6 = dhcp6_get_struct(netif, "dhcp6_enable_stateful");
  if (dhcp6 == NULL) {
    return ERR_MEM;
  }
  if (dhcp6->state == DHCP6_STATE_OFF) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp6_enable: DHCPv6 enabled\n"));
    dhcp6_set_state(netif, dhcp6, DHCP6_STATE_IDLE, "dhcp6_enable");
  } else if (dhcp6->state == DHCP6_STATE_OFF) {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp6_enable: DHCPv6 is in releasing process. Be enabled when done.\n"));
    dhcp6->state_after_release = DHCP6_STATE_IDLE;
    return ERR_INPROGRESS;
  } else {
    LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp6_enable: DHCPv6 is not disabled\n"));
    return ERR_ALREADY;
  }

  return ERR_OK;
}

/**
 * @ingroup dhcp6
 * Disable DHCPv6 on specified netif and dealocates it't memory.
 */
void
dhcp6_disable(struct netif *netif)
{
  struct dhcp6 *dhcp6;

  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_STATE, ("dhcp6_disable(netif=%p) %c%c%"U16_F"\n",
              (void *)netif, netif->name[0], netif->name[1], (u16_t)netif->num));

  dhcp6 = netif_dhcp6_data(netif);
  if (dhcp6 != NULL) {
    switch (dhcp6->state) {
      case DHCP6_STATE_OFF:
          break;
      case DHCP6_STATE_IDLE:
      case DHCP6_STATE_SL_REQUESTING_CONFIG:
#if LWIP_IPV6_DHCP6_STATEFUL
      case DHCP6_STATE_SF_INIT_DELAY:
      case DHCP6_STATE_SF_SOLICITING:
      case DHCP6_STATE_SF_REQUESTING:
#endif
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_OFF, "disable");
          break;

#if LWIP_IPV6_DHCP6_STATEFUL
      case DHCP6_STATE_SF_BOUND:
      case DHCP6_STATE_SF_RENEWING:
      case DHCP6_STATE_SF_REBINDING:
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_RELEASING, "disable");
          break;

      case DHCP6_STATE_SF_RELEASING:
          break;

#endif /* LWIP_IPV6_DHCP6_STATEFUL */
      default:
          LWIP_ASSERT("Unhandled state at dhcp6_set_state", 0);
    }
    dhcp6->state_after_release = DHCP6_STATE_OFF;
  }
}

#if LWIP_IPV6_DHCP6_STATEFUL
/**
 * Allows you to set a reference to custom DUID of client identifier.
 *
 * DUID format is described in RFC 8415 section 11.
 *
 * By default lwIP will send DUID Based on Link-Layer Address (DUID-LL).
 *
 * @param netif
 * @param duid      Ptr to DUID data or NULL to set default DUID mentioned above.
 * @param duid_len  Length of DUID
 */
void dhcp6_set_client_id_duid(struct netif *netif, const void *duid, u16_t duid_len)
{
  struct dhcp6 *dhcp6;
  LWIP_ASSERT("netif is NULL", netif != NULL);
  LWIP_ASSERT_CORE_LOCKED();
  dhcp6 = netif_dhcp6_data(netif);
  LWIP_ASSERT("Enable dhcp6 first", dhcp6 != NULL);
  LWIP_ASSERT("Duid must be null or longer than 0", ( (duid != NULL) && (duid_len > 0) ) || (duid == NULL) );
  if (dhcp6 != NULL) {
    dhcp6->cli_id_usr_duid = duid;
    dhcp6->cli_id_usr_duid_len = duid_len;
  }
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

#if LWIP_IPV6_DHCP6_PD
/**
 * Register function which will be called on change of delegated prefix.
 * @param netif
 * @param callback function
 */
void
dhcp6_register_pd_callback(struct netif *netif, dhcp6_pd_callback_fn callback)
{
  struct dhcp6 *dhcp6;
  LWIP_ASSERT("netif is NULL", netif != NULL);
  LWIP_ASSERT_CORE_LOCKED();
  dhcp6 = netif_dhcp6_data(netif);
  LWIP_ASSERT("Enable dhcp6 first", dhcp6 != NULL);
  if (dhcp6 != NULL) {
    dhcp6->pd_callback = callback;
  }
}

/**
 * Register function which will be called on change of delegated prefix.
 * @param netif
 * @returns pointer to internal @ref struct dhcp6_delegated_prefix or NULL if client is disabled.
 *
 */
const struct dhcp6_delegated_prefix *
dhcp6_get_delegated_prefix(struct netif *netif)
{
  struct dhcp6 *dhcp6;
  LWIP_ASSERT("netif is NULL", netif != NULL);
  LWIP_ASSERT_CORE_LOCKED();
  dhcp6 = netif_dhcp6_data(netif);

  if (dhcp6 != NULL) {
    return &dhcp6->prefix;
  }
  return NULL;
}

/* Return true if we have the prefix stored by dhcp6_parse_ia_pd. Doesn't checks valid time.*/
static u8_t
is_prefix_advertised(const struct dhcp6 *dhcp6)
{
  return (dhcp6->prefix.prefix_len > 0);
}

/* Sets prefix to be "not delegated". is_prefix_advertised() will return false.*/
static void
set_prefix_not_delegated(struct dhcp6 *dhcp6)
{
  dhcp6->prefix.prefix_len = 0U;
}
#endif /* LWIP_IPV6_DHCP6_PD */

#if LWIP_IPV6_DHCP6_STATEFUL
static u8_t
is_addr_advertised(const struct dhcp6 *dhcp6)
{
  return !(ip6_addr_isany(&dhcp6->addr));
}

/* Sets address to be "not advertised". is_addr_advertised() will then return false. */
static void
set_addr_not_advertised(struct dhcp6 *dhcp6)
{
  ip6_addr_set_any(&dhcp6->addr);
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

/**
 * Create a DHCPv6 request, fill in common headers
 *
 * @param netif the netif under DHCPv6 control
 * @param dhcp6 dhcp6 control struct
 * @param message_type message type of the request
 * @param opt_len_alloc option length to allocate
 * @param options_out_len option length on exit
 * @return a pbuf for the message
 */
static struct pbuf *
dhcp6_create_msg(struct netif *netif, struct dhcp6 *dhcp6, u8_t message_type,
                 u16_t opt_len_alloc, u16_t *options_out_len)
{
  struct pbuf *p_out;
  struct dhcp6_msg *msg_out;

  LWIP_ERROR("dhcp6_create_msg: netif != NULL", (netif != NULL), return NULL;);
  LWIP_ERROR("dhcp6_create_msg: dhcp6 != NULL", (dhcp6 != NULL), return NULL;);
  p_out = pbuf_alloc(PBUF_TRANSPORT, sizeof(struct dhcp6_msg) + opt_len_alloc, PBUF_RAM);
  if (p_out == NULL) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
                ("dhcp6_create_msg: could not allocate pbuf\n"));
    return NULL;
  }
  LWIP_ASSERT("dhcp6_create_msg: check that first pbuf can hold struct dhcp6_msg",
              (p_out->len >= sizeof(struct dhcp6_msg) + opt_len_alloc));
  
  /* reuse transaction identifier in retransmissions */
  if (dhcp6->tries == 0) {
    dhcp6->xid = LWIP_RAND() & 0xFFFFFF;
#if LWIP_IPV6_DHCP6_STATEFUL
    dhcp6->trans_started_at = sys_now();
#endif
  }

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE,
              (" transaction id xid(%"X32_F")\n", dhcp6->xid));

  msg_out = (struct dhcp6_msg *)p_out->payload;
  memset(msg_out, 0, sizeof(struct dhcp6_msg) + opt_len_alloc);

  msg_out->msgtype = message_type;
  msg_out->transaction_id[0] = (u8_t)(dhcp6->xid >> 16);
  msg_out->transaction_id[1] = (u8_t)(dhcp6->xid >> 8);
  msg_out->transaction_id[2] = (u8_t)dhcp6->xid;
  *options_out_len = 0;

  return p_out;
}

#if LWIP_IPV6_DHCP6_PD
static u16_t
dhcp6_option_byte(u16_t options_out_len, u8_t *options, u8_t value)
{
  options[options_out_len++] = value;
  return options_out_len;
}
#endif /* LWIP_IPV6_DHCP6_PD */

static u16_t
dhcp6_option_short(u16_t options_out_len, u8_t *options, u16_t value)
{
  options[options_out_len++] = (u8_t)((value & 0xff00U) >> 8);
  options[options_out_len++] = (u8_t) (value & 0x00ffU);
  return options_out_len;
}

#if LWIP_IPV6_DHCP6_STATEFUL
static u16_t
dhcp6_option_unsigned(u16_t options_out_len, u8_t *options, u32_t value)
{
  options[options_out_len++] = (u8_t)((value & 0xff000000U) >> 24);
  options[options_out_len++] = (u8_t)((value & 0x00ff0000U) >> 16);
  options[options_out_len++] = (u8_t)((value & 0x0000ff00U) >>  8);
  options[options_out_len++] = (u8_t) (value & 0x000000ffU);
  return options_out_len;
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

static u16_t
dhcp6_option_optionrequest_size(void)
{
  const u16_t header_size = 4U;
  const u16_t option_size = 2U;

  u8_t num_requested_options = 0;
  while (requested_options[num_requested_options] != 0) {
    num_requested_options++;
  }

  return header_size + (option_size * num_requested_options);
}

static u16_t
dhcp6_option_optionrequest(u16_t options_out_len, u8_t *options, u16_t max_len)
{
  size_t i;
  u16_t ret;
  const u16_t header_size = 4U;

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt option request: "));

  LWIP_ASSERT("dhcp6_option_optionrequest: packet too small",
    options_out_len + dhcp6_option_optionrequest_size() <= max_len);
  LWIP_UNUSED_ARG(max_len);

  ret = dhcp6_option_short(options_out_len, options, DHCP6_OPTION_ORO);
  ret = dhcp6_option_short(ret, options, dhcp6_option_optionrequest_size() - header_size);
  for (i = 0; requested_options[i] != 0; i++) {
    ret = dhcp6_option_short(ret, options, requested_options[i]);
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("%"U16_F", ", requested_options[i]));
  }

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("\n"));

  return ret;
}

#if LWIP_IPV6_DHCP6_STATEFUL
static u16_t
dhcp6_option_ia_na_size(struct dhcp6 *dhcp6, u8_t include_addr)
{
  if (include_addr && (!is_addr_advertised(dhcp6))) {
    /* We don't have address to add */
    return 0;

  } else {
    const u16_t option_iaaddr_len = 28;
    const u16_t ia_na_len = 16 + ((include_addr) ? option_iaaddr_len : 0);

    return ia_na_len;
  }
}

/**
 * Add Identity association for Non temporary address request option into msg
 */
static u16_t
dhcp6_option_ia_na(struct netif *netif, struct dhcp6 *dhcp6, u8_t include_addr,  u16_t options_out_len, u8_t *options, u16_t max_len)
{
  if (include_addr && (!is_addr_advertised(dhcp6))) {
    /* We don't have address to add */
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt IA_NA: Not adding IA_NA because srv didn't offered any\n"));
    return options_out_len;

  } else {
    u16_t ret;
    const u16_t ia_na_len = dhcp6_option_ia_na_size(dhcp6, include_addr);

    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt IA_NA\n"));

    LWIP_ASSERT("dhcp6_option_ia_na: packet too small", options_out_len + ia_na_len <= max_len);
    LWIP_UNUSED_ARG(max_len);

    ret = dhcp6_option_short(options_out_len, options, DHCP6_OPTION_IA_NA);
    ret = dhcp6_option_short(ret, options, ia_na_len - 4); /* exclude option header */

    memcpy(&options[ret], &netif->hwaddr[1], 4); /* IAID */
    ret += 4;

    /* 0 means no client preference for those values */
    ret = dhcp6_option_unsigned(ret, options, dhcp6->addr_t1);
    ret = dhcp6_option_unsigned(ret, options, dhcp6->addr_t2);

    if (include_addr) {
      const u16_t option_iaaddr_len = 24;

      ret = dhcp6_option_short(ret, options, DHCP6_OPTION_IAADDR);
      ret = dhcp6_option_short(ret, options, option_iaaddr_len);

      memcpy(&options[ret], dhcp6->addr.addr, sizeof(dhcp6->addr.addr));
      ret += sizeof(dhcp6->addr.addr);

      ret = dhcp6_option_unsigned(ret, options, dhcp6->addr_pref); /* preferred time */
      ret = dhcp6_option_unsigned(ret, options, dhcp6->addr_valid); /* valid time */
    }

    return ret;
  }
}
#endif  /* LWIP_IPV6_DHCP6_STATEFUL */

#if LWIP_IPV6_DHCP6_PD
static u16_t
dhcp6_option_ia_pd_size(const struct dhcp6 *dhcp6, u8_t include_prefix)
{
  if (include_prefix && (!is_prefix_advertised(dhcp6))) {
    /* We don't have any prefix to add */
    return 0;
  } else {
    const u16_t ia_prefix_len = 25 + 4;
    const u16_t ia_pd_len = 16 + ((include_prefix) ? ia_prefix_len : 0);

    return ia_pd_len;
  }
}

/**
 * Add Identity association for prefix delegation option into msg
 */
static u16_t
dhcp6_option_ia_pd(struct netif *netif, struct dhcp6 *dhcp6, u8_t include_prefix, u16_t options_out_len, u8_t *options, u16_t max_len)
{
  if (include_prefix && (!is_prefix_advertised(dhcp6))) {
    /* We don't have any prefix to add */
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt IA_PD: Not adding IA_PD because srv didn't offered any\n"));
    return options_out_len;

  } else {
    u16_t ret;

    const u16_t ia_prefix_len = 25;
    const u16_t ia_pd_len = dhcp6_option_ia_pd_size(dhcp6, include_prefix);

    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt option IA_PD\n"));

    LWIP_ASSERT("dhcp6_option_ia_pd: packet too small.",
                options_out_len + ia_pd_len <= max_len);
    LWIP_UNUSED_ARG(max_len);


    ret = dhcp6_option_short(options_out_len, options, DHCP6_OPTION_IA_PD);
    ret = dhcp6_option_short(ret, options, ia_pd_len - 4);

    memcpy(&options[ret], &netif->hwaddr[1], 4); /* IAID */
    ret += 4;

    ret = dhcp6_option_unsigned(ret, options, include_prefix ? dhcp6->ia_pd_t1 : 0); /* T1 */
    ret = dhcp6_option_unsigned(ret, options, include_prefix ? dhcp6->ia_pd_t2 : 0); /* T2 */

    if (include_prefix) {
      ret = dhcp6_option_short(ret, options, DHCP6_OPTION_IAPREFIX);
      ret = dhcp6_option_short(ret, options, ia_prefix_len);
      ret = dhcp6_option_unsigned(ret, options, dhcp6->prefix.prefix_pref); /* preferred time */
      ret = dhcp6_option_unsigned(ret, options, dhcp6->prefix.prefix_valid); /* valid time */
      ret = dhcp6_option_byte(ret, options, dhcp6->prefix.prefix_len);

      memcpy(&options[ret], dhcp6->prefix.prefix.addr, sizeof(dhcp6->prefix.prefix.addr));
      ret += sizeof(dhcp6->prefix.prefix.addr);
    }

    return ret;
  }
}
#endif /* LWIP_IPV6_DHCP6_PD */

#if LWIP_IPV6_DHCP6_STATEFUL
static u16_t
dhcp6_option_client_identifier_size(struct netif *netif, struct dhcp6 *dhcp6)
{
  const u16_t duid_ll_len = 4 + netif->hwaddr_len;
  const u16_t clientid_len = 4 + ( (dhcp6->cli_id_usr_duid != NULL) ? dhcp6->cli_id_usr_duid_len : duid_ll_len );
  return clientid_len;
}

static u16_t
dhcp6_option_client_identifier(struct netif *netif, struct dhcp6 *dhcp6, u16_t options_out_len, u8_t *options, u16_t max_len)
{
  u16_t ret;

  const u16_t clientid_len = dhcp6_option_client_identifier_size(netif, dhcp6);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt client identifier: (Using hw addr as DUID)\n"));

  LWIP_ASSERT("dhcp6_option_client_identifier: packet too small",
                (options_out_len + clientid_len) <= max_len);
  LWIP_UNUSED_ARG(max_len);

  ret = dhcp6_option_short(options_out_len, options, DHCP6_OPTION_CLIENTID);
  ret = dhcp6_option_short(ret, options, clientid_len - 4);

  if (dhcp6->cli_id_usr_duid != NULL) {
    memcpy(&options[ret], dhcp6->cli_id_usr_duid, dhcp6->cli_id_usr_duid_len);
    ret += dhcp6->cli_id_usr_duid_len;
  } else {
  /*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |               3               |    hardware type (16 bits)    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   .                                                               .
   .             link-layer address (variable length)              .
   .                                                               .
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   */

    ret = dhcp6_option_short(ret, options, DHCP6_DUID_LL);
    ret = dhcp6_option_short(ret, options, LWIP_IANA_HWTYPE_ETHERNET);
    memcpy(&options[ret], netif->hwaddr, 6);
    ret += 6;
  }

  return ret;
}


static u16_t
dhcp6_option_server_identifier_size(struct dhcp6 *dhcp6)
{
  return dhcp6->server_id_len;
}

static u16_t
dhcp6_option_server_identifier(struct dhcp6 *dhcp6,const u16_t options_out_len, u8_t *options, u16_t max_len)
{
  const u16_t len = dhcp6_option_server_identifier_size(dhcp6);

  LWIP_ASSERT("dhcp6_option_server_identifier: dhcp6->server_id != NULL", dhcp6->server_id != NULL);
  LWIP_ASSERT("dhcp6_option_server_identifier: packet too small",
              options_out_len + len <= max_len);
  LWIP_UNUSED_ARG(max_len);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt server ID\n"));

  memcpy(&options[options_out_len], dhcp6->server_id, len);

  return options_out_len + len;
}


static u16_t
dhcp6_option_elapsed_size(void)
{
  return 6U;
}

static u16_t
dhcp6_option_elapsed(struct dhcp6 *dhcp6, u16_t options_out_len, u8_t *options, u16_t max_len)
{
  u16_t ret;
  const u32_t elapsed_32 = (sys_now() - dhcp6->trans_started_at) / 10;
  const u16_t elapsed_16 = (elapsed_32 >= 0xffff) ? 0xffff : (u16_t)elapsed_32;

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" Add opt elapsed: %"U16_F"0 ms\n", elapsed_16));

  LWIP_ASSERT("dhcp6_option_elapsed: packet too small",
              (options_out_len + dhcp6_option_elapsed_size()) <= max_len);
  LWIP_UNUSED_ARG(max_len);

  ret = dhcp6_option_short(options_out_len, options, DHCP6_OPTION_ELAPSED_TIME);
  ret = dhcp6_option_short(ret, options, dhcp6_option_elapsed_size() - 4);
  ret = dhcp6_option_short(ret, options, elapsed_16);

  return ret;
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

/* All options are added, shrink the pbuf to the required size */
static void
dhcp6_msg_finalize(struct dhcp6 *dhcp6, u16_t options_out_len, struct pbuf *p_out)
{
  /* shrink the pbuf to the actual content length */
  pbuf_realloc(p_out, (u16_t)(sizeof(struct dhcp6_msg) + options_out_len));

  if (dhcp6->tries < 255) {
    dhcp6->tries++;
  }

}

#if LWIP_IPV6_DHCP6_STATEFUL
/**
 * Compose and send statefull message
 * @param dhcp6
 * @param message_type dhcp6 message type
 * @param fill_addr_and_prefix  true: If addr/prefix is present in dhcp6 struct ia_na/ia_pd with addr/prefix will be
 *                                    appended to msg. If not present ia_na/ia_pd will not be added to msg.
 *                              false: ia_pd/ia_na without prefix/address will be appended to msg.
 * @param fill_srvid  If true copies server id into
 */
static void
dhcp6_send_stateful_msg(struct netif *netif, struct dhcp6 *dhcp6, u8_t message_type,
                        u8_t fill_addr_and_prefix, u8_t fill_srvid)
{
  struct pbuf *p_out;
  u16_t options_out_len;
  u16_t size = dhcp6_option_client_identifier_size(netif, dhcp6) + dhcp6_option_optionrequest_size() +
               dhcp6_option_elapsed_size();

#if LWIP_IPV6_DHCP6_PD
    size += dhcp6_option_ia_pd_size(dhcp6, fill_addr_and_prefix);
#endif

  if (!dhcp6->addr_by_slaac) {
    size += dhcp6_option_ia_na_size(dhcp6, fill_addr_and_prefix);
  }
  if (fill_srvid) {
    size += dhcp6_option_server_identifier_size(dhcp6);
  }

  /* create and initialize the DHCP message header */
  p_out = dhcp6_create_msg(netif, dhcp6, message_type, size, &options_out_len);
  if (p_out != NULL) {
    err_t err;
    struct dhcp6_msg *msg_out = (struct dhcp6_msg *)p_out->payload;
    u8_t *options = (u8_t *)(msg_out + 1);
    const u16_t options_max_len = size;

    if (!dhcp6->addr_by_slaac) {
      options_out_len = dhcp6_option_ia_na(netif, dhcp6, fill_addr_and_prefix,
                                           options_out_len, options, options_max_len);
    }

    options_out_len = dhcp6_option_client_identifier(netif, dhcp6, options_out_len, options, options_max_len);
    if (fill_srvid) {
      options_out_len = dhcp6_option_server_identifier(dhcp6, options_out_len, options, options_max_len);
    }
    options_out_len = dhcp6_option_optionrequest(options_out_len, options, options_max_len);
    options_out_len = dhcp6_option_elapsed(dhcp6, options_out_len, options, options_max_len);
#if LWIP_IPV6_DHCP6_PD
    options_out_len = dhcp6_option_ia_pd(netif, dhcp6, fill_addr_and_prefix, options_out_len, options, options_max_len);
#endif
    dhcp6_msg_finalize(dhcp6, options_out_len, p_out);

    err = udp_sendto_if (dhcp6_pcb, p_out, &dhcp6_All_DHCP6_Relay_Agents_and_Servers, DHCP6_SERVER_PORT, netif);
    pbuf_free(p_out);
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_send_stateful_msg: sent, udp_sendto_if returned %d\n", (int)err));
    LWIP_UNUSED_ARG(err);
  } else {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS, ("dhcp6_send_stateful_msg: could not allocate DHCP6 request\n"));
  }
}

static void
dhcp6_solicitation(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_solicitation: preparing msg\n"));
  dhcp6_send_stateful_msg(netif, dhcp6, DHCP6_SOLICIT, 0, 0);
}

static void
dhcp6_request(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_request: preparing msg\n"));
  dhcp6_send_stateful_msg(netif, dhcp6, DHCP6_REQUEST, 1, 1);
}

static void
dhcp6_renew(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_renew: preparing msg\n"));
  dhcp6_send_stateful_msg(netif, dhcp6, DHCP6_RENEW, 1, 1);
}

static void
dhcp6_rebind(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_rebind: preparing msg\n"));
  dhcp6_send_stateful_msg(netif, dhcp6, DHCP6_RENEW, 1, 0);
}

static void
dhcp6_release(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_release: preparing msg\n"));
  dhcp6_send_stateful_msg(netif, dhcp6, DHCP6_RELEASE, 1, 1);
}
#endif /* LWIP_IPV6_DHCP6_STATEFULL */

#if LWIP_IPV6_DHCP6_STATELESS
/**
 * Send information request (stateless)
 */
static void
dhcp6_information_request(struct netif *netif, struct dhcp6 *dhcp6)
{
  struct pbuf *p_out;
  u16_t options_out_len;
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_information_request: preparing msg\n"));
  /* create and initialize the DHCP message header */
  p_out = dhcp6_create_msg(netif, dhcp6, DHCP6_INFOREQUEST, dhcp6_option_optionrequest_size(), &options_out_len);
  if (p_out != NULL) {
    err_t err;
    struct dhcp6_msg *msg_out = (struct dhcp6_msg *)p_out->payload;
    u8_t *options = (u8_t *)(msg_out + 1);

    options_out_len = dhcp6_option_optionrequest(options_out_len, options, p_out->len);

    dhcp6_msg_finalize(dhcp6, options_out_len, p_out);

    err = udp_sendto_if (dhcp6_pcb, p_out, &dhcp6_All_DHCP6_Relay_Agents_and_Servers, DHCP6_SERVER_PORT, netif);
    pbuf_free(p_out);
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_information_request: sent with return code %d\n", (int)err));
    LWIP_UNUSED_ARG(err);
  } else {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS, ("dhcp6_information_request: could not allocate DHCP6 request\n"));
  }
}

#endif /* LWIP_IPV6_DHCP6_STATELESS */

/** This function is called from nd6 module when an RA message is received
 * It triggers DHCPv6 requests (if enabled).
 *
 * @param managed_addr_config Managed Address Config Flag tells the host if it should use stateful DHCPv6
 * @param other_config        Other Config Flag tells the host that there is other information the router can provide.
 */
void
dhcp6_nd6_ra_trigger(struct netif *netif, u8_t managed_addr_config, u8_t other_config)
{
  struct dhcp6 *dhcp6;
#if !LWIP_IPV6_DHCP6_STATEFUL
  LWIP_UNUSED_ARG(managed_addr_config);
#endif

  LWIP_ASSERT("netif != NULL", netif != NULL);
  dhcp6 = netif_dhcp6_data(netif);

  if (dhcp6 != NULL) {

    switch (dhcp6->state) {
      case DHCP6_STATE_IDLE:
#if LWIP_IPV6_DHCP6_STATEFUL
        dhcp6->addr_by_slaac = !managed_addr_config;

        if (managed_addr_config) {
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_INIT_DELAY, "ra_trigger M flag");
          break;
        }
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

        if (other_config) {
#if LWIP_IPV6_DHCP6_PD
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_INIT_DELAY, "ra_trigger O flag");
#else
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SL_REQUESTING_CONFIG, "ra_trigger O flag");
#endif
          break;
        }
        break;
      case DHCP6_STATE_OFF:
      case DHCP6_STATE_SL_REQUESTING_CONFIG:
#if LWIP_IPV6_DHCP6_STATEFUL
      case DHCP6_STATE_SF_INIT_DELAY:
      case DHCP6_STATE_SF_SOLICITING:
      case DHCP6_STATE_SF_REQUESTING:
      case DHCP6_STATE_SF_BOUND:
      case DHCP6_STATE_SF_RENEWING:
      case DHCP6_STATE_SF_REBINDING:
      case DHCP6_STATE_SF_RELEASING:
#endif /* LWIP_IPV6_DHCP6_STATEFUL */
        break;

      default:
        LWIP_ASSERT("Unhandled state at dhcp6_nd6_ra_trigger", 0);
    } /* switch */
  }

}
 /* copies option into bufer and retrns its len or -1 and sets offset param to beginning of option data (after header) */
static s16_t dhcp6_find_option(const struct pbuf *p, u16_t option_code, u16_t *offset)
{
  const u16_t opt_hdr_len = 4;

  while (((*offset) + opt_hdr_len) <= p->tot_len) {
    const u8_t hdr_len = sizeof(struct dhcp6_option_header);
    u8_t hdr_buf[sizeof(struct dhcp6_option_header)];

    struct dhcp6_option_header *hdr =
            (struct dhcp6_option_header *) pbuf_get_contiguous(p, hdr_buf, hdr_len, hdr_len, *offset);

    if (hdr != NULL) {
      u16_t next_offset;
      const u16_t found_opt_code = lwip_ntohs(hdr->option_code);
      const u16_t found_opt_len = lwip_ntohs(hdr->option_len);

      *offset += hdr_len;
      next_offset = *offset + found_opt_len;

      if (next_offset > p->tot_len) {
        return -1; /* bad packet */
      }

      if (found_opt_code == option_code) {
        return found_opt_len;
      }

      *offset = next_offset;
    } else {
      return -1;
    }
  }
  return -1;
}

#if LWIP_IPV6_DHCP6_STATEFUL
/* stores server id in to dhcp6 struct return 1 on success */
static u8_t
dhcp6_parse_and_store_serverid(struct pbuf *p, u16_t offset, struct dhcp6 *dhcp6)
{
  void *ptr;
  /* store option including its header.  */
  const s16_t server_id_len = dhcp6_find_option(p, DHCP6_OPTION_SERVERID, &offset) + 4;
  offset -= 4;

  if (server_id_len < 0) {
    return 0;
  }

  if (dhcp6->server_id != NULL) {
    mem_free(dhcp6->server_id);
  }
  dhcp6->server_id = mem_malloc(server_id_len);

  if (dhcp6->server_id == NULL) {
    return 0;
  }

  ptr = pbuf_get_contiguous(p, dhcp6->server_id, server_id_len, server_id_len, offset);
  if (ptr == NULL) {
    return 0;
  } else if (ptr != dhcp6->server_id) {
    memcpy(dhcp6->server_id, ptr, server_id_len);
    dhcp6->server_id_len = server_id_len;
  }

  return 1;
}

/* returns 0 if received and  */
static u8_t
dhcp6_parse_and_check_serverid_different(struct pbuf *p, u16_t offset, struct dhcp6 *dhcp6)
{
  u16_t are_different;
  /* store option including its header.  */
  const s16_t server_id_len = dhcp6_find_option(p, DHCP6_OPTION_SERVERID, &offset) + 4;
  offset -= 4;

  if (server_id_len < 0) {
    return 2;
  } else if (server_id_len != dhcp6->server_id_len) {
    return 3;
  }

  LWIP_ASSERT("DHCP6: Check of server id requested but we haven't stored any", dhcp6->server_id != NULL);

  are_different = pbuf_memcmp(p, offset, dhcp6->server_id, server_id_len);

  return are_different;
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

#if LWIP_DHCP6_PROVIDE_DNS_SERVERS
/* Parses list of dns servers and sets them into lwIP */
static u8_t
dhcp6_parse_dns_servers(const struct netif *netif, const struct pbuf *p, u16_t offset)
{
  uint8_t i;
  ip_addr_t addr;
  s16_t dns_opt_len = dhcp6_find_option(p, DHCP6_OPTION_DNS_SERVERS, &offset);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" dhcp6_parse_dns_servers:"));

  if (dns_opt_len < 16 ) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" got none\n"));
    return 0;
  }

  ip_addr_set_zero_ip6(&addr);

  for(i=0; dns_opt_len >= 16 && i < LWIP_DHCP6_MAX_DNS_SERVERS; i++) {
    uint16_t n = pbuf_copy_partial(p, &addr.u_addr.ip6.addr, 16, offset);
    if (n == 16) {
      ip6_addr_assign_zone(&addr.u_addr.ip6, IP6_UNKNOWN, netif);
      dns_setserver(i, &addr);
      offset += 16;
      dns_opt_len -= 16;
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" %s,", ipaddr_ntoa(&addr)));
    } else {
      break;
    }
  }
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("\n"));

  /* Reset remaining slots */
  for (; i < LWIP_DHCP6_MAX_DNS_SERVERS; i++) {
      dns_setserver(i, NULL);
  }

  return 1;
}
#endif /* LWIP_DHCP6_PROVIDE_DNS_SERVERS */

#if LWIP_DHCP6_GET_NTP_SRV
/* stores server id in to dhcp6 struct return 1 on success */
static u8_t
dhcp6_parse_sntp_servers(const struct netif * netif, const struct pbuf *p, u16_t offset)
{
  uint8_t num = 0;
  ip_addr_t addrs[LWIP_DHCP6_MAX_SNTP_SERVERS];
  s16_t sntp_opt_len = dhcp6_find_option(p, DHCP6_OPTION_SNTP_SERVERS, &offset);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" dhcp6_parse_sntp_servers:"));
  if (sntp_opt_len < 16) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" got none\n"));
  }

  for (; sntp_opt_len >= 16 && num < LWIP_DHCP6_MAX_SNTP_SERVERS; num++) {
    ip_addr_set_zero_ip6(&addrs[num]);
    if (pbuf_copy_partial(p, &addrs[num].u_addr.ip6.addr, 16, offset) == 16) {
      ip6_addr_assign_zone(&addrs[num].u_addr.ip6, IP6_UNKNOWN, netif);
      offset += 16;
      sntp_opt_len -= 16;
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" %s,", ipaddr_ntoa(&addrs[num])));
    } else {
      break;
    }
  }

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("\n"));

  dhcp6_set_ntp_servers(num, addrs);

  return (num > 0) ? 1 : 0;
}
#endif /* LWIP_DHCP6_GET_NTP_SRV */

#ifdef LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM
static void
dhcp6_parse_oro_opts_custom(const struct netif * netif, const struct dhcp6 * dhcp6, const struct pbuf *p, u16_t offset)
{
  u8_t i = 0;
  u16_t opt_offset = offset;

  /* Ignore opts already handled by dhcp client */
#if LWIP_DHCP6_PROVIDE_DNS_SERVERS
  i++;
#endif
#if LWIP_DHCP6_GET_NTP_SRV
  i++;
#endif

  for (; requested_options[i] != 0; i++) {
    s16_t opt_len = dhcp6_find_option(p, requested_options[i], &opt_offset);
    if (opt_len >= 0) {
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" dhcp6_parse_oro_opts_custom: got %"U16_F"\n", requested_options[i]));
      LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM(netif, requested_options[i], dhcp6->state, p, opt_offset, opt_len);
    }
  }
}
#endif /* LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM */

#if LWIP_IPV6_DHCP6_STATEFUL
static u8_t
dhcp6_parse_ia_na(struct pbuf *p, u16_t offset, struct dhcp6 *dhcp6)
{
  const u16_t ia_len = dhcp6_find_option(p, DHCP6_OPTION_IA_NA, &offset);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" dhcp6_parse_ia_na:"));

  if (ia_len >= (sizeof(struct dhcp6_option_ia) + sizeof(struct dhcp6_option_ia_address) + 4)) {
    struct dhcp6_option_ia buf;
    const struct dhcp6_option_ia *opt_ia =
            (const struct dhcp6_option_ia *)pbuf_get_contiguous(p, &buf, sizeof(buf), sizeof(buf), offset);
    if (opt_ia != NULL) {
      u16_t ia_opt_offset = offset + sizeof(struct dhcp6_option_ia); /* Address option is inside ia option. */
      const u16_t ia_addr_opt_len = dhcp6_find_option(p, DHCP6_OPTION_IAADDR, &ia_opt_offset);

      if (ia_addr_opt_len >= sizeof(struct dhcp6_option_ia_address)) {
        struct dhcp6_option_ia_address a_buf;
        const struct dhcp6_option_ia_address *opt_ia_addr =
                (struct dhcp6_option_ia_address *)pbuf_get_contiguous(
                        p, &a_buf, sizeof(a_buf), sizeof(a_buf), ia_opt_offset);

        if (opt_ia_addr != NULL) {
          dhcp6->addr_t1 = lwip_ntohl(opt_ia->t1);
          dhcp6->addr_t2 = lwip_ntohl(opt_ia->t2);

          memcpy(&dhcp6->addr.addr, &opt_ia_addr->address, sizeof(opt_ia_addr->address));
          dhcp6->addr_pref = lwip_ntohl(opt_ia_addr->preferred);
          dhcp6->addr_valid = lwip_ntohl(opt_ia_addr->valid);

          LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" %s t1=%"U32_F" t2=%"U32_F" prf=%"U32_F" val=%"U32_F"\n",
                      ip6addr_ntoa(&dhcp6->addr), dhcp6->addr_t1, dhcp6->addr_t2, dhcp6->addr_pref, dhcp6->addr_valid));
          return 1;
        }
      }
    }
  }
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" got nothing\n"));
  return 0;
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

#if LWIP_IPV6_DHCP6_PD

/* stores server id in to dhcp6 struct */
static u8_t
dhcp6_parse_ia_pd(struct pbuf *p, u16_t offset, struct dhcp6 *dhcp6)
{
  const s16_t ia_len = dhcp6_find_option(p, DHCP6_OPTION_IA_PD, &offset);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" dhcp6_parse_ia_pd:"));

  if (ia_len >= (s16_t)(sizeof(struct dhcp6_option_ia) + sizeof(struct dhcp6_option_ia_pd) + 4)) {
    struct dhcp6_option_ia buf;
    const struct dhcp6_option_ia *opt_ia =
            (struct dhcp6_option_ia *)pbuf_get_contiguous(p, &buf, sizeof(buf), sizeof(buf), offset);

    u16_t ia_opt_offset = offset + sizeof(struct dhcp6_option_ia); /* Address option is inside ia option. */
    const s16_t ia_addr_opt_len = dhcp6_find_option(p, DHCP6_OPTION_IAPREFIX, &ia_opt_offset);

    if (ia_addr_opt_len >= (s16_t)sizeof(struct dhcp6_option_ia_pd)) {
      struct dhcp6_option_ia_pd p_buf;
      const struct dhcp6_option_ia_pd *opt_ia_pref =
              (const struct dhcp6_option_ia_pd *) pbuf_get_contiguous(
                      p, &p_buf, sizeof(p_buf), sizeof(p_buf), ia_opt_offset);

      dhcp6->ia_pd_t1 = lwip_ntohl(opt_ia->t1);
      dhcp6->ia_pd_t2 = lwip_ntohl(opt_ia->t2);

      memcpy(&dhcp6->prefix.prefix.addr, &opt_ia_pref->prefix, sizeof(opt_ia_pref->prefix));
      dhcp6->prefix.prefix_len = opt_ia_pref->prefix_length;
      dhcp6->prefix.prefix_pref = lwip_ntohl(opt_ia_pref->preferred);
      dhcp6->prefix.prefix_valid = lwip_ntohl(opt_ia_pref->valid);

      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" %s/%"U16_F" t1=%"U32_F" t2=%"U32_F" prf=%"U32_F" val=%"U32_F"\n",
              ip6addr_ntoa(&dhcp6->prefix.prefix), (u16_t)dhcp6->prefix.prefix_len,
              dhcp6->ia_pd_t1, dhcp6->ia_pd_t1, dhcp6->prefix.prefix_pref, dhcp6->prefix.prefix_valid));

      return 1;
    }
  }
  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, (" got nothing\n"));
  return 0;
}
#endif /* LWIP_IPV6_DHCP6_PD */

static void
dhcp6_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
  struct netif *netif = ip_current_input_netif();
  struct dhcp6 *dhcp6 = netif_dhcp6_data(netif);
  struct dhcp6_msg *reply_msg = (struct dhcp6_msg *)p->payload;
  u8_t msg_type;
  u32_t xid;
  const u16_t opt_offset = sizeof(struct dhcp6_msg);

  LWIP_UNUSED_ARG(arg);

  /* Caught DHCPv6 message from netif that does not have DHCPv6 enabled? -> not interested */
  if ((dhcp6 == NULL) || (dhcp6->pcb_allocated == 0)) {
    goto free_pbuf_and_return;
  }

  LWIP_ERROR("invalid server address type", IP_IS_V6(addr), goto free_pbuf_and_return;);

  LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE,("dhcp6_recv(pbuf=%p len=%"U16_F", tot_len=%"U16_F") from DHCPv6 srv %s\n",
                                            (void *)p, p->len, p->tot_len, ipaddr_ntoa(addr)));
  /* prevent warnings about unused arguments */
  LWIP_UNUSED_ARG(pcb);
  LWIP_UNUSED_ARG(addr);
  LWIP_UNUSED_ARG(port);

  if (p->len < sizeof(struct dhcp6_msg)) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING, ("DHCPv6 reply message or pbuf too short\n"));
    goto free_pbuf_and_return;
  }

  /* match transaction ID against what we expected */
  xid = reply_msg->transaction_id[0] << 16;
  xid |= reply_msg->transaction_id[1] << 8;
  xid |= reply_msg->transaction_id[2];
  if (xid != dhcp6->xid) {
    LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                ("transaction id mismatch reply_msg->xid(%"X32_F")!= dhcp6->xid(%"X32_F")\n", xid, dhcp6->xid));
    goto free_pbuf_and_return;
  }

  /* read DHCP message type */
  msg_type = reply_msg->msgtype;
  /* message type is DHCP6 REPLY? */
  switch (msg_type) {
    case DHCP6_REPLY:
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("DHCP6_REPLY received\n"));
#if LWIP_IPV6_DHCP6_STATELESS
      /* in info-requesting state? */
      switch (dhcp6->state) {
        case DHCP6_STATE_SL_REQUESTING_CONFIG:
#if LWIP_DHCP6_PROVIDE_DNS_SERVERS
          dhcp6_parse_dns_servers(netif, p, opt_offset);
#endif
#if LWIP_DHCP6_GET_NTP_SRV
          dhcp6_parse_sntp_servers(netif, p, opt_offset);
#endif /* LWIP_DHCP6_PROVIDE_DNS_SERVERS */
#ifdef LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM
          dhcp6_parse_oro_opts_custom(netif, dhcp6, p, opt_offset);
#endif
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_IDLE, "dhcp6_recv");
          break;
#endif /* LWIP_IPV6_DHCP6_STATELESS */
#if LWIP_IPV6_DHCP6_STATEFUL
        case DHCP6_STATE_SF_RENEWING:
        case DHCP6_STATE_SF_REQUESTING: {
          u8_t success;
          u8_t refused_by_srv = 0;
          if (dhcp6_parse_and_check_serverid_different(p, opt_offset, dhcp6)) {
            LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                        ("DHCPv6 reply has different server ID. Ignoring.\n"));
            break; /* reply is not for us */
          }
#if LWIP_IPV6_DHCP6_PD
          success = dhcp6_parse_ia_pd(p, opt_offset, dhcp6);
          if (!success) {
            set_prefix_not_delegated(dhcp6);
            LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("Using DHCPv6 advertisement without prefix.\n"));
          } else {
            refused_by_srv = (dhcp6->prefix.prefix_valid == 0);
            if (refused_by_srv) {
              LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("DHCPv6 srv revoked delegated prefix (valid time is zero).\n"));
            }
          }
#endif /* LWIP_IPV6_DHCP6_PD */

#if LWIP_DHCP6_PROVIDE_DNS_SERVERS
          dhcp6_parse_dns_servers(netif, p, opt_offset);
#endif
#if LWIP_DHCP6_GET_NTP_SRV
          dhcp6_parse_sntp_servers(netif, p, opt_offset);
#endif
#ifdef LWIP_HOOK_DHCP6_PARSE_ORO_OPTS_CUSTOM
          dhcp6_parse_oro_opts_custom(netif, dhcp6, p, opt_offset);
#endif

          if (!dhcp6->addr_by_slaac) {
            success = dhcp6_parse_ia_na(p, opt_offset, dhcp6);
            if (!success) {
              set_addr_not_advertised(dhcp6);
            } else {
              LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("Using DHCPv6 advertisement without address\n"));
              netif_add_ip6_address(netif, &dhcp6->addr, &dhcp6->addr_idx);
              if (dhcp6->addr_idx >= 0) {
                netif_ip6_addr_set_pref_life(netif, dhcp6->addr_idx, dhcp6->addr_pref);
                netif_ip6_addr_set_valid_life(netif, dhcp6->addr_idx, dhcp6->addr_valid);
                if (dhcp6->addr_valid <= 0) {
                  /* Server is refusing our renew request. */
                  refused_by_srv = 1;
                  netif_ip6_addr_set_state(netif, dhcp6->addr_idx, IP6_ADDR_INVALID);
                } else if (ip6_addr_isvalid(netif_ip6_addr_state(netif, 1))) {
                  netif_ip6_addr_set_state(netif, dhcp6->addr_idx, IP6_ADDR_PREFERRED);
                } else {
                  netif_ip6_addr_set_state(netif, dhcp6->addr_idx, IP6_ADDR_TENTATIVE);
                }
              } else {
                LWIP_ASSERT("Not enough space for IPv6 add from DHCP.", dhcp6->addr_idx < 0);
              }
            }
          } else {
            set_addr_not_advertised(dhcp6);
          }
#if LWIP_IPV6_DHCP6_PD
          if (dhcp6->pd_callback != NULL) {
            dhcp6->pd_callback(netif, &dhcp6->prefix, (dhcp6->prefix.prefix_valid > 0));
          }

          /*dhcp6_set_state(netif, dhcp6, valid ? DHCP6_STATE_SF_BOUND, "dhcp6_recv"); toto nejak vykut - potrebujeme se dostat do soliciting pokud jakykoliv valid time = 0 a
          a zahlasit to callbackem o prefixu.*/

#endif /* LWIP_IPV6_DHCP6_PD */
          dhcp6_set_state(netif, dhcp6, refused_by_srv ? DHCP6_STATE_SF_SOLICITING : DHCP6_STATE_SF_BOUND, "dhcp6_recv");
          break;
        } /* case DHCP6_STATE_SF_REQUESTING */

        case DHCP6_STATE_SF_RELEASING:
          if (dhcp6_parse_and_check_serverid_different(p, opt_offset, dhcp6)) {
            LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_WARNING,
                        ("Ignoring reply with different server ID.\n"));
            break; /* reply is not for us */
          }
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_OFF, "dhcp6_recv");

#endif /* LWIP_IPV6_DHCP6_STATEFUL */

        default:
          break;
      } /* switch (dhcp6->state) */

      break;

#if LWIP_IPV6_DHCP6_STATEFUL
    case DHCP6_ADVERTISE:
      if (dhcp6->state == DHCP6_STATE_SF_SOLICITING) {
        /* TODO handle rapid commit option, handle preference option, seek for the best server. */
        int success = dhcp6_parse_and_store_serverid(p, opt_offset, dhcp6);
        if (!success) {
          LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_LEVEL_WARNING, ("DHCP6_ADVERTISE without server id\n"));
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_SOLICITING, "dhcp6_recv");
          break;
        }

#if LWIP_IPV6_DHCP6_PD
        success = dhcp6_parse_ia_pd(p, opt_offset, dhcp6);
        if (!success) {
          set_prefix_not_delegated(dhcp6);
          LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("Using dhcp6 advertisement without prefix\n"));
        }
#endif /* LWIP_IPV6_DHCP6_PD */

        if (!dhcp6->addr_by_slaac) {
          success = dhcp6_parse_ia_na(p, opt_offset, dhcp6);
          if (!success) {
            set_addr_not_advertised(dhcp6);
            LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("Using dhcp6 advertisement without address\n"));
          }
        } else {
          set_addr_not_advertised(dhcp6);
        }
        dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_REQUESTING, "dhcp6_recv");
      }
      break;
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

    default:
      break;

  } /* switch (msg_type) */

free_pbuf_and_return:
  pbuf_free(p);
}

static void
dhcp6_update_retrans_timeout(struct dhcp6 *dhcp6)
{
  dhcp6->request_timeout *= 2;
  if (dhcp6->request_timeout > dhcp6->max_request_timeout) {
    dhcp6->request_timeout = dhcp6->max_request_timeout;
  }
  dhcp6->request_timeout_cntr = dhcp6->request_timeout;
}

/**
 * A DHCPv6 request has timed out.
 *
 * The timer that was started with the DHCPv6 request has
 * timed out, indicating no response was received in time.
 */
static void
dhcp6_timeout(struct netif *netif, struct dhcp6 *dhcp6)
{
  LWIP_DEBUGF(DHCP_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout()\n"));

  LWIP_UNUSED_ARG(netif);
  LWIP_UNUSED_ARG(dhcp6);

  switch (dhcp6->state) {

    case DHCP6_STATE_OFF:
    case DHCP6_STATE_IDLE:
      break;
    case DHCP6_STATE_SL_REQUESTING_CONFIG:
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout: retrying information request\n"));
      dhcp6_information_request(netif, dhcp6);
      break;
#if LWIP_IPV6_DHCP6_STATEFUL
    case DHCP6_STATE_SF_INIT_DELAY:
      dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_SOLICITING, "dhcp6_timeout from INIT_DELAY");
      break;
    case DHCP6_STATE_SF_SOLICITING:
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout: retrying solicitation\n"));
      dhcp6_solicitation(netif, dhcp6);
      break;
    case DHCP6_STATE_SF_REQUESTING:
      if (dhcp6->tries >= dhcp6->max_retrans_cnt) {
        dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_SOLICITING, "dhcp6_timeout from REQUESTING");
      } else {
        LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout: retrying request\n"));
        dhcp6_request(netif, dhcp6);
      }
      break;

    case DHCP6_STATE_SF_BOUND:
      break;

    case DHCP6_STATE_SF_RENEWING:
      LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout: retrying renew request\n"));
      dhcp6_renew(netif, dhcp6);
      break;
    case DHCP6_STATE_SF_REBINDING:
      dhcp6_rebind(netif, dhcp6);
      break;
    case DHCP6_STATE_SF_RELEASING:
      if (dhcp6->tries >= dhcp6->max_retrans_cnt) {
        dhcp6_set_state(netif, dhcp6, DHCP6_STATE_OFF, "dhcp6_timeout from RELEASING");
      } else {
        LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_timeout: retrying release request\n"));
        dhcp6_release(netif, dhcp6);
      }
      break;
#endif /* LWIP_IPV6_DHCP6_STATEFUL */
    default:
      LWIP_ASSERT("Unhandled state in dhcp6_timeout", 0);
  }

  dhcp6_update_retrans_timeout(dhcp6);

/*#endif  LWIP_IPV6_DHCP6_STATELESS */
}

#if LWIP_IPV6_DHCP6_STATEFUL
static void
dhcp6_1s_tick(struct netif * netif, struct dhcp6 *dhcp6) {

  switch (dhcp6->state) {
    case DHCP6_STATE_SF_BOUND:

#if LWIP_IPV6_DHCP6_PD
      if (!is_prefix_advertised(dhcp6)) {
        /* nothing to do */
      }
      else if (dhcp6->prefix.prefix_pref == 1) {
        dhcp6->prefix.prefix_pref = 0;
        /* Valid time of prefix expired */
        dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_RENEWING, "dhcp6_1s_tick");
        break;
      } else if (dhcp6->prefix.prefix_pref > 0) {
        dhcp6->prefix.prefix_pref--;
      }
#endif /* LWIP_IPV6_DHCP6_PD */

      if (is_addr_advertised(dhcp6)) {
        if (dhcp6->addr_t1 == 1) {
          dhcp6->addr_t1 = 0;
          /* Valid time of prefix expired */
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_RENEWING, "dhcp6_1s_tick");
          break;
        } else if (dhcp6->addr_t1 > 0) {
          dhcp6->addr_t1--;
        }

        if (netif_ip6_addr_pref_life(netif, dhcp6->addr_idx) == 0) {
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_RENEWING, "dhcp6_1s_tick");
          break;
        }
      }

      /* Intentional fall through */

    case DHCP6_STATE_SF_RENEWING:
#if LWIP_IPV6_DHCP6_PD
      if (!is_prefix_advertised(dhcp6)) {
        /* nothing to do */
      }
      else if (dhcp6->prefix.prefix_valid == 1) {
        dhcp6->prefix.prefix_valid = 0;
        /* Valid time of prefix expired */
        if (dhcp6->pd_callback != NULL) {
          dhcp6->pd_callback(netif, &dhcp6->prefix, 0);
        }
        dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_SOLICITING, "dhcp6_1s_tick");
        break;
      } else if (dhcp6->prefix.prefix_valid > 0) {
        dhcp6->prefix.prefix_valid--;
      }
#endif /* LWIP_IPV6_DHCP6_PD */

      if (is_addr_advertised(dhcp6)) {
        if (dhcp6->addr_t2 == 1) {
          dhcp6->addr_t2 = 0;
          /* Valid time of prefix expired */
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_RENEWING, "dhcp6_1s_tick");
          break;
        } else if (dhcp6->addr_t2 > 0) {
          dhcp6->addr_t2--;
        }

        if (netif_ip6_addr_valid_life(netif, dhcp6->addr_idx) == 0) {
          dhcp6_set_state(netif, dhcp6, DHCP6_STATE_SF_SOLICITING, "dhcp6_1s_tick");
          break;
        }
      }
      break;

    default:
      break;
  }
}
#endif /* LWIP_IPV6_DHCP6_STATEFUL */


/**
 * DHCPv6 timeout handling (this function must be called every 500ms,
 * see @ref DHCP6_TIMER_MSECS).
 *
 * A DHCPv6 server is expected to respond within a short period of time.
 * This timer checks whether an outstanding DHCPv6 request is timed out.
 */
void
dhcp6_tmr(void)
{
  struct netif *netif;

#if LWIP_IPV6_DHCP6_STATEFUL
  u8_t second_elapsed = 0;
  static u16_t ticks_ms = 0;
  if (ticks_ms >= 1000) {
    ticks_ms -= 1000;
    second_elapsed = 1;
  }
  ticks_ms += DHCP6_TIMER_MSECS;
#endif /* LWIP_IPV6_DHCP6_STATEFUL */

  /* loop through netif's */
  NETIF_FOREACH(netif) {
    struct dhcp6 *dhcp6 = netif_dhcp6_data(netif);
    /* only act on DHCPv6 configured interfaces */
    if (dhcp6 != NULL) {
#if LWIP_IPV6_DHCP6_STATEFUL
      if (second_elapsed) {
        dhcp6_1s_tick(netif, dhcp6);
      }
#endif

      /* timer is active (non-zero), and is about to trigger now */
      if (dhcp6->request_timeout_cntr > 1) {
        dhcp6->request_timeout_cntr--;
      } else if (dhcp6->request_timeout_cntr == 1) {
        dhcp6->request_timeout_cntr--;
        /* { dhcp6->request_timeout == 0 } */
        LWIP_DEBUGF(DHCP6_DEBUG | LWIP_DBG_TRACE, ("dhcp6_tmr: request timeout\n"));
        /* this client's request timeout triggered */
        dhcp6_timeout(netif, dhcp6);
      }
    }
  } /* NETIF_FOREACH() */
}


#endif /* LWIP_IPV6 && LWIP_IPV6_DHCP6 */
