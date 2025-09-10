/** @file net.c
 *
 *  @brief  This file provides network porting code
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

// Modified to work with Mongoose built-in TCP/IP by Cesanta Software Ltd.

#include <inttypes.h>
#include <osa.h>
#include <wlan.h>
#include <wm_net.h>
#include "netif_decl.h"
#include "wifi.h"

#include <wmlog.h>
#define net_e(...) wmlog_e("net", ##__VA_ARGS__)

#if CONFIG_NET_DEBUG
#define net_d(...) wmlog("net", ##__VA_ARGS__)
#else
#define net_d(...)
#endif /* ! CONFIG_NET_DEBUG */

typedef struct {
  struct netif netif;
} interface_t;

static interface_t s_sta;
static interface_t s_ap;

static bool net_wlan_init_done;

// in netif-specific netif_mongoose.c
extern void handle_data_packet(const t_u8 interface, const t_u8 *rcvdata, const t_u16 datalen);
extern void handle_amsdu_data_packet(t_u8 interface, t_u8 *rcvdata, t_u16 datalen);
extern void handle_deliver_packet_above(t_void *rxpd, t_u8 interface, t_void *lwip_pbuf);
extern bool wrapper_net_is_ip_or_ipv6(const t_u8 *buffer);

void net_wlan_set_mac_address(unsigned char *stamac, unsigned char *uapmac) {
  if (stamac != NULL) memcpy(s_sta.netif.mac, stamac, sizeof(s_sta.netif.mac));
  if (uapmac != NULL) memcpy(s_ap.netif.mac, uapmac, sizeof(s_sta.netif.mac));
}

int net_wlan_init(void) {
#ifdef RW610
  (void) wifi_register_data_input_callback(&handle_data_packet);
  (void) wifi_register_amsdu_data_input_callback(&handle_amsdu_data_packet);
  (void) wifi_register_deliver_packet_above_callback(
      &handle_deliver_packet_above);
  (void) wifi_register_wrapper_net_is_ip_or_ipv6_callback(
      &wrapper_net_is_ip_or_ipv6);
#endif
  if (!net_wlan_init_done) {

#ifndef RW610
    (void) wifi_register_data_input_callback(&handle_data_packet);
    (void) wifi_register_amsdu_data_input_callback(&handle_amsdu_data_packet);
    (void) wifi_register_deliver_packet_above_callback(
        &handle_deliver_packet_above);
    (void) wifi_register_wrapper_net_is_ip_or_ipv6_callback(
        &wrapper_net_is_ip_or_ipv6);
#endif
    net_wlan_init_done = 1;
  }

  (void) wlan_wlcmgr_send_msg(WIFI_EVENT_NET_INTERFACE_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
  return WM_SUCCESS;
}

struct netif *net_get_sta_interface(void) {
  return &s_sta.netif;
}

struct netif *net_get_uap_interface(void) {
  return &s_ap.netif;
}

int net_get_if_name_netif(char *pif_name, struct netif *iface) {
  if (iface == &s_sta.netif) strcpy(pif_name, "STA");
  if (iface == &s_ap.netif) strcpy(pif_name, "AP");
  return WM_SUCCESS;
}

int net_wlan_deinit(void) {
  return WM_SUCCESS;
}

void net_stop_dhcp_timer(void) {
}

void *net_get_sta_handle(void) {
  return &s_sta;
}

void *net_get_uap_handle(void) {
  return &s_ap;
}

int net_alloc_client_data_id() {
  return -1;
}

void net_interface_up(void *intrfc_handle) {
(void) intrfc_handle;
}

void net_interface_down(void *intrfc_handle) {
(void) intrfc_handle;
}

void net_interface_dhcp_stop(void *intrfc_handle) {
(void) intrfc_handle;
}

void net_interface_dhcp_cleanup(void *intrfc_handle) {
(void) intrfc_handle;
}

int net_configure_address(struct net_ip_config *addr, void *intrfc_handle) {
  interface_t *if_handle = (interface_t *)intrfc_handle;
  // A Mongoose environment does not need this nor works this way
  net_d("Deliberately ignoring request to configure interface %s (with %s)",
        (if_handle == &s_sta) ? "STA" : "AP", (addr->ipv4.addr_type == NET_ADDR_TYPE_DHCP) ? "DHCP client" : "Static IP");
  if (if_handle == &s_sta) {
    wlan_wlcmgr_send_msg(WIFI_EVENT_NET_STA_ADDR_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
    // Keep wlan in its realm and fool it to return we are connected
    if (addr->ipv4.addr_type == NET_ADDR_TYPE_DHCP)
      wlan_wlcmgr_send_msg(WIFI_EVENT_NET_DHCP_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
  } else if (if_handle == &s_ap) {
    (void) wlan_wlcmgr_send_msg(WIFI_EVENT_UAP_NET_ADDR_CONFIG, WIFI_EVENT_REASON_SUCCESS, NULL);
  } else {
    // Do nothing
  }
  return WM_SUCCESS;
}

int net_get_if_addr(struct net_ip_config *addr, void *intrfc_handle) {
  interface_t *if_handle = (interface_t *)intrfc_handle;
  extern void netif_fillstaddr(unsigned *, unsigned *, unsigned *, unsigned *); // type is NXP's struct type
  extern void netif_fillapaddr(unsigned *, unsigned *, unsigned *, unsigned *); // type is NXP's struct type
  if (if_handle == &s_sta) netif_fillstaddr(&addr->ipv4.address, &addr->ipv4.netmask, &addr->ipv4.gw, &addr->ipv4.dns1);
  if (if_handle == &s_ap)  netif_fillapaddr(&addr->ipv4.address, &addr->ipv4.netmask, &addr->ipv4.gw, &addr->ipv4.dns1);
  addr->ipv4.dns2 = addr->ipv4.dns1;
  return WM_SUCCESS;
}

void net_configure_dns(struct net_ip_config *ip, unsigned int role) {
  // A Mongoose environment does not need this nor works this way
  net_d("Deliberately ignoring request to configure DNS for role %u", role);
}
