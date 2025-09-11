// Portions of this taken from NXP examples, copyright 2008-2023 NXP,
// BSD-3-Clause licensed

#include <netif_decl.h>
#include "mongoose.h"
#include "wlan_bt_fw.h"
#define STA_NWKLBL "wifi"  // wlan ids interfaces with text labels...
#define AP_NWKLBL "uap-network" // hardcoded in wlan.c, do NOT change

#if CONFIG_IPV6
#error "CONFIG_IPV6 is not supported, disable it"
#endif

#if FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER
#error FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER is not supported, disable it
#endif

#if CONFIG_TX_RX_ZERO_COPY
#error CONFIG_TX_RX_ZERO_COPY is not supported, disable it
#endif

uint16_t g_data_nf_last;
uint16_t g_data_snr_last;

#if CONFIG_WPS2
static void (*wps_rx_callback)(const t_u8 *buf, size_t len);
#endif

void handle_data_packet(const t_u8 ifc, const t_u8 *data, const t_u16 len);
void handle_amsdu_data_packet(t_u8 ifc, t_u8 *data, t_u16 len);
void handle_deliver_packet_above(t_void *rxpd, t_u8 ifc, t_void *priv_pbuf);
bool wrapper_net_is_ip_or_ipv6(const t_u8 *buffer);

#if MGMT_RX
static int (*rx_mgmt_callback)(const enum wlan_bss_type bss_type,
                               const wifi_mgmt_frame_t *frame,
                               const size_t len);
void rx_mgmt_register_callback(
    int (*rx_mgmt_cb_fn)(const enum wlan_bss_type bss_type,
                         const wifi_mgmt_frame_t *frame, const size_t len)) {
  rx_mgmt_callback = rx_mgmt_cb_fn;
}
void rx_mgmt_deregister_callback() {
  rx_mgmt_callback = NULL;
}
#endif

#define ETH_PKT_SIZE 1540  // Max frame size

// Receive path
// - net.c registers these handlers:
//   - Received data --> handle_data_packet()
//   - AMSDU data --> handle_amsdu_data_packet()
//   - deliver_packet_above_callback --> handle_deliver_packet_above
// - This generates the following receive data flow:
//   - Rx Frame --> handle_data_packet() --> process_data_packet() --> wifi libs
//   internal processing -->
//     --> handle_deliver_packet_above() --> deliver_packet_above()
//     --> handle_amsdu_data_packet() --> deliver_packet_above()
// - Wifi libs internal processing assumes some kind of lwip_pbuf-like packet
// buffer scheme; so, it calls back some functions to operate on that, and
// returns that to the two handlers shown above
// - process_data_packet() is expected to filter and respond to some management
// frames, store Rx Frame data on that buffering scheme, and handle it to
// another section of the vendor libs.
// - deliver_packet_above() finally delivers to Mongoose TCP/IP stack or back to
// the vendor stack for final EAPOL/WPS2 processing.

// THESE FUNCTIONS GET CALLED FROM HAL_ImuMainCpu13 TASK CONTEXT

static void process_data_packet(const uint8_t *data, const uint16_t len);
static void deliver_packet_above(struct pb *buf, int ifc);
static struct pb *topb(uint8_t *data, uint16_t len);

void handle_data_packet(const t_u8 ifc, const t_u8 *data, const t_u16 len) {
  if (ifc < 2)
    process_data_packet((const uint8_t *) data, (const uint16_t) len);
}

void handle_amsdu_data_packet(t_u8 ifc, t_u8 *data, t_u16 len) {
  if (ifc >= 2) return;
  // handled data will be destroyed when we return + libs use FreeRTOS tasks
  struct pb *buf = topb((uint8_t *) data, (uint16_t) len);
  if (buf != NULL) deliver_packet_above(buf, ifc);
}

void handle_deliver_packet_above(t_void *rxpd, t_u8 ifc, t_void *priv_pbuf) {
  if (ifc >= 2) return;
  deliver_packet_above((struct pb *) priv_pbuf, ifc);
  (void) rxpd;
}

// This one gets called from wifi_low_level_output(), in Mongoose context
bool wrapper_net_is_ip_or_ipv6(const t_u8 *buffer) {
  return net_is_ip_or_ipv6(buffer);
}

#pragma pack(push, 1)
// in network order
struct eth_hdr {  // TODO(scaprile) reuse 'eth' in net_builtin.c
  uint8_t dest[6];
  uint8_t src[6];
  uint16_t type;
};

struct llc_hdr {
  uint8_t dsap, ssap, ctrl, oui[3];
  uint16_t proto;
};
#pragma pack(pop)

// Entry point for data frames, wrap or put in a custom buffer is necessary,
// then handle back to wifi libs for processing
//  ------------- ------ ----------
// | INTF_HEADER | RxPD | Rx frame |
//  ------------- ------ ----------
// Take into account the possibility that the code is being compiled with the
// wpa_supplicant library and support for mobile wireless extensions
static void process_data_packet(const uint8_t *data, const uint16_t len) {
  RxPD *rxpd = (RxPD *) (data + INTF_HEADER_LEN);
  mlan_bss_type ifc = (mlan_bss_type) (rxpd->bss_type);
  uint16_t header_type;
#if !CONFIG_WPA_SUPP
#if (CONFIG_11K) || (CONFIG_11V) || (CONFIG_1AS)
  wlan_mgmt_pkt *pmgmt_pkt_hdr = MNULL;
  wlan_802_11_header *pieee_pkt_hdr = MNULL;
  t_u16 sub_type = 0;
  t_u8 category = 0;
#endif
#endif
  uint8_t *payload;
  uint16_t payload_len = 0;
  struct pb *buf = NULL;

  if (rxpd->rx_pkt_type == PKT_TYPE_AMSDU) {
    Eth803Hdr_t *h = (Eth803Hdr_t *) ((t_u8 *) rxpd + rxpd->rx_pkt_offset);
    if (memcmp(mlan_adap->priv[ifc]->curr_addr, h->dest_addr,
               MLAN_MAC_ADDR_LENGTH) != 0 &&
        ((h->dest_addr[0] & 0x01) == 0))
      return;  // If the AMSDU frame is unicast and is not for us, drop it
  }

  if (ifc == MLAN_BSS_TYPE_STA || ifc == MLAN_BSS_TYPE_UAP) {
    g_data_nf_last = rxpd->nf;
    g_data_snr_last = rxpd->snr;
  }

#if !CONFIG_WPA_SUPP
#if (CONFIG_11K) || (CONFIG_11V) || (CONFIG_1AS)
  if (rxpd->rx_pkt_type == PKT_TYPE_MGMT_FRAME && ifc == MLAN_BSS_TYPE_STA) {
    pmgmt_pkt_hdr =
        (wlan_mgmt_pkt *) (void *) ((t_u8 *) rxpd + rxpd->rx_pkt_offset);
    pieee_pkt_hdr = (wlan_802_11_header *) (void *) &pmgmt_pkt_hdr->wlan_header;
    sub_type = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(pieee_pkt_hdr->frm_ctl);
    category = *((t_u8 *) pieee_pkt_hdr + sizeof(wlan_802_11_header));
    if (sub_type == (t_u16) SUBTYPE_ACTION &&
        (category != (t_u8) IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC &&
         category != (t_u8) IEEE_MGMT_ACTION_CATEGORY_WNM &&
         category != (t_u8) IEEE_MGMT_ACTION_CATEGORY_UNPROTECT_WNM))
      return;
    payload = (uint8_t *) rxpd;
    payload_len = len - INTF_HEADER_LEN;
  } else
#endif
#endif
  {
    payload = (uint8_t *) rxpd + rxpd->rx_pkt_offset;
    payload_len = rxpd->rx_pkt_length;
  }

  // handled data will be destroyed when we return + libs use FreeRTOS tasks
  buf = topb(payload, payload_len);
  if (buf == NULL) return;

#if !CONFIG_WPA_SUPP
  if (rxpd->rx_pkt_type == PKT_TYPE_MGMT_FRAME) {
    // Bypass management frames about Add Block Ack Request or Add Block Ack
    // Response
    if (wlan_bypass_802dot11_mgmt_pkt(buf->data) == WM_SUCCESS) {
      free(buf);
      return;
    }
#if (CONFIG_11K) || (CONFIG_11V) || (CONFIG_1AS)
    if (sub_type == (t_u16) SUBTYPE_ACTION && ifc == MLAN_BSS_TYPE_STA) {
      if (wifi_event_completion(WIFI_EVENT_MGMT_FRAME,
                                WIFI_EVENT_REASON_SUCCESS, buf) != WM_SUCCESS)
        free(buf);
      return;
    }
#endif
    // MGMT_RX: rx_mgmt_callback() is never called in the examples; dropped
  }
#endif

  struct eth_hdr *ethhdr = (struct eth_hdr *) buf->data;
  header_type = mg_ntohs(ethhdr->type);

  uint8_t llcoui[6] = {0xaa, 0xaa, 0x03, 0x00, 0x00, 0x00};  // 802.3 frame ?
  if (memcmp(buf->data + sizeof(Eth803Hdr_t), llcoui, 6) == 0) {
    struct llc_hdr *h = (struct llc_hdr *) (buf->data + sizeof(Eth803Hdr_t));
    header_type = mg_ntohs(h->proto);
    if (rxpd->rx_pkt_type != PKT_TYPE_AMSDU) {  // convert to Ethernet
      ethhdr->type = h->proto;  // Rest of Eth header = 802.3 header
      buf->len -= sizeof(*h);
      (void) memmove(buf->data + sizeof(*ethhdr),
                     buf->data + sizeof(*ethhdr) + sizeof(*h),
                     buf->len - sizeof(*ethhdr));
    }
  }
  switch (header_type) {
    case 0x0800:  // IPv4
#if CONFIG_IPV6
    case 0x86dd:  // IPv6
#endif
    case 0x0806:  // Unicast ARP also needs to be reordered
      if (ifc == MLAN_BSS_TYPE_STA || ifc == MLAN_BSS_TYPE_UAP) {
        if (wrapper_wlan_handle_rx_packet(len, rxpd, buf, payload) !=
            WM_SUCCESS)
          free(buf);
      } else {
        wrapper_wlan_update_uap_rxrate_info(rxpd);
        deliver_packet_above(buf, ifc);
      }
      break;
    case 0x888E:  // EAPOL
      deliver_packet_above(buf, ifc);
      break;
    default:
#if CONFIG_NET_MONITOR
      if (rxpd->rx_pkt_type == PKT_TYPE_802DOT11 && get_monitor_flag()) {
        wifi_frame_t *frame = (wifi_frame_t *) buf->data;
        if (frame->frame_type == BEACON_FRAME ||
            frame->frame_type == DATA_FRAME ||
            frame->frame_type == AUTH_FRAME ||
            frame->frame_type == PROBE_REQ_FRAME ||
            frame->frame_type == QOS_DATA_FRAME) {
          user_recv_monitor_data(data);
        }
      }
#endif
      free(buf);
      break;
  }
}

static struct mg_tcpip_if *s_ifp;

// Final processing stage, deliver to TCP/IP stack or to wlan libs
static void deliver_packet_above(struct pb *buf, int ifc) {
  struct eth_hdr *ethhdr = (struct eth_hdr *) buf->data;
  w_pkt_d("Data RX: Driver=>Kernel, if %d, len %d", ifc, buf->len);
  if (mg_ntohs(ethhdr->type == 0x888E)) {
#if CONFIG_WPS2
    if (wps_rx_callback != NULL) wps_rx_callback(buf->data, buf->len);
#endif
  } else if (ifc < 2) {
    mg_tcpip_qwrite(buf->data, buf->len, s_ifp);
  }
  free(buf);
}

#if CONFIG_WPS2
void wps_register_rx_callback(void (*h)(const t_u8 *buf, const size_t len)) {
  wps_rx_callback = h;
}

void wps_deregister_rx_callback() {
  wps_rx_callback = NULL;
}
#endif

static struct pb *topb(uint8_t *data, uint16_t len) {
  struct pb *buf = (struct pb *) malloc(ETH_PKT_SIZE + sizeof(buf->len));
  if (buf == NULL) {
    MG_ERROR(("OOM"));
    return buf;
  }
  memcpy(buf->data, data, len);
  buf->len = len;
  return buf;
}

// Utility and link status functions, CALLED FROM (yet) ANOTHER TASK CONTEXT

// need to make sure Mongoose will not write to these vars
void netif_fillstaddr(unsigned *addr, unsigned *mask, unsigned *gw,
                      unsigned *dns) {  // type is NXP's struct type
}
void netif_fillapaddr(unsigned *addr, unsigned *mask, unsigned *gw,
                      unsigned *dns) {  // type is NXP's struct type
}

// Transmit path
// - Mongoose calls netif_tx(), which tries to make it to the wlan libs
//   - If what seems to be a priority queuing scheme (WMM) is in place, it then
//   tries calling wifi_add_to_bypassq(). This function expects a networking
//   stack proprietary buffer, so it will call back those functions mentioned at
//   the receive path docs above. If that fails, it tries several times to get a
//   vendor proprietary packet buffer, crafts data into it, and handles it to
//   wifi_low_level_output().
//   - Otherwise (no WMM), it tries several times to get another type of vendor
//   proprietary packet buffer, crafts data into it, and handles it to
//   wifi_low_level_output().

// These functions are called by Mongoose task (except when noted)

size_t netif_tx(const void *bfr, size_t len, struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_nxp_wifi_data *d = (struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data;
  t_u8 ifc = (d->wifi.apmode) ? MLAN_BSS_TYPE_UAP : MLAN_BSS_TYPE_STA;
  int ret;
  t_u8 *buf = NULL;
  t_u16 pktlen;
  uint32_t buflen;

#if CONFIG_WMM
  t_u8 tid = 0;
  int retry = 0;
  t_u8 ra[MLAN_MAC_ADDR_LENGTH] = {0};
  bool is_tx_pause = false;
  extern int retry_attempts;

  t_u32 pkt_prio = wifi_wmm_get_pkt_prio((void *) bfr, &tid);
  if (pkt_prio == -WM_FAIL) return 0;

  if (wifi_tx_status == WIFI_DATA_BLOCK) {
    wifi_tx_block_cnt++;
    return len;
  }

  if (wifi_add_to_bypassq(ifc, (void *) bfr, len) == WM_SUCCESS) return len;

  wifi_wmm_da_to_ra((void *) bfr, ra);
  retry = retry_attempts;
  while (retry--) {
    buf = wifi_wmm_get_outbuf_enh(&buflen, (mlan_wmm_ac_e) pkt_prio, ifc, ra,
                                  &is_tx_pause);
    if (buf != NULL) break;
    if (buf == NULL && retry <= 0) {
      wifi_wmm_drop_retried_drop(ifc);
      mlan_adap->priv[ifc]->tx_overrun_cnt++;
      return 0;
    }
    if (is_tx_pause) OSA_TimeDelay(1);
    send_wifi_driver_tx_data_event(ifc);
  }
  pktlen = sizeof(TxPD) + INTF_HEADER_LEN + sizeof(mlan_linked_list);
  memset(buf, 0x00, pktlen);
  memcpy(buf + pktlen, bfr, len);
  pktlen += len;
  ret = wifi_low_level_output(ifc, buf, pktlen, pkt_prio, tid);
#else
  buf = wifi_get_outbuf(&buflen);
  if (buf == NULL) {
    mlan_adap->priv[ifc]->tx_overrun_cnt++;
    return 0;
  }
  pktlen = sizeof(TxPD) + INTF_HEADER_LEN;
  memset(buf, 0x00, pktlen);
  memcpy(buf + pktlen, bfr, len);
  pktlen += len;
  ret = wifi_low_level_output(ifc, buf, pktlen);
#endif

  if (ret != WM_SUCCESS) {
    if (ret == -WM_E_NOMEM) mlan_adap->priv[ifc]->tx_overrun_cnt++;
    return 0;
  }
  return len;
}


static volatile bool s_stalink = false, s_aplink = false, s_ok = false,
                     s_fail = false;
static bool s_scanning = false, s_connecting = false;


// THIS FUNCTION GETS CALLED FROM wlcmgr_task TASK CONTEXT
// - even though a callback is passed to the init function, other parts of the libs rely on an external with this name... so we reuse the same function
int wlan_event_callback(enum wlan_event_reason reason, void *data) {
  (void) data;
  switch (reason) {
    case WLAN_REASON_INITIALIZED:
      s_ok = true;
      break;
    case WLAN_REASON_INITIALIZATION_FAILED:
      s_fail = true;
      break;
    case WLAN_REASON_SUCCESS:
      s_ok = true;
      s_stalink = true;
      break;
    case WLAN_REASON_CONNECT_FAILED:
    case WLAN_REASON_NETWORK_NOT_FOUND:
    case WLAN_REASON_NETWORK_AUTH_FAILED:
      s_fail = true;
      s_stalink = false;
      break;
    case WLAN_REASON_USER_DISCONNECT:
    case WLAN_REASON_LINK_LOST:
      s_stalink = false;
      break;
    case WLAN_REASON_UAP_SUCCESS:
      s_ok = true;
      s_aplink = true;
      break;
    case WLAN_REASON_UAP_START_FAILED:
    case WLAN_REASON_UAP_STOP_FAILED:
      s_fail = true;
      break;
    case WLAN_REASON_UAP_STOPPED:
      s_ok = true;
      s_aplink = false;
      break;
    default:
      break;
  }
  return WM_SUCCESS;
}


bool netif_init(struct mg_tcpip_if *ifp) {
  s_ifp = ifp;
  s_stalink = false;
  s_aplink = false;
  if (wlan_init(wlan_fw_bin, wlan_fw_bin_len) != WM_SUCCESS) return false;
  if (wlan_start(&wlan_event_callback) != WM_SUCCESS) return false;
  while (!s_ok) {
    if (s_fail) return false;
    OSA_TimeDelay(1);
  }
  wlan_get_mac_address(ifp->mac);  // same for STA and AP
  wlan_config_mef(MEF_TYPE_MULTICAST, 3);
  return true;
}


static volatile uint8_t s_scan_results = 0;

static void handle_scan_result(unsigned int, mg_tcpip_event_handler_t);

bool netif_poll(struct mg_tcpip_if *ifp, bool s1, mg_tcpip_event_handler_t evcb) {
  if (s_scanning && s_scan_results > 0) {
    // serve scan results one by one, to be fair to other connections; result
    // table should stay valid until next scan
    handle_scan_result((unsigned int) --s_scan_results, evcb);
    if (s_scan_results == 0) {
      MG_VERBOSE(("scan complete"));
      s_scanning = false;
      evcb(ifp, MG_TCPIP_EV_WIFI_SCAN_END, NULL);
    }
  }
  if (!s1) return false;
  struct mg_tcpip_driver_nxp_wifi_data *d = (struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data;
  if (d->wifi.apmode) return s_aplink;
  MG_VERBOSE(("conn: %c ok/fail: %c/%c", s_connecting ? '1' : '0',
              s_ok ? '1' : '0', s_fail ? '1' : '0'));
  if (s_connecting && (s_ok || s_fail)) {
    if (s_fail) evcb(ifp, MG_TCPIP_EV_WIFI_CONNECT_ERR, NULL);
    s_connecting = false;
  }
  return s_stalink;
}


static void handle_scan_result(unsigned int i, mg_tcpip_event_handler_t evcb) {
  struct wlan_scan_result r;
  if (wlan_get_scan_result((uint32_t) i, &r) != WM_SUCCESS) return;
  struct mg_wifi_scan_bss_data bss;
  unsigned char bssid[6];
  bss.SSID = mg_str_n(r.ssid, r.ssid_len);
  mg_mprintf(bssid, "%M", print_mac, r.bssid);
  bss.BSSID = bssid;
  bss.RSSI = (int8_t) -r.rssi;
  bss.has_n = r.dot11n;  // TODO(scaprile): add 11ac, 11ax, etc
  bss.channel = r.channel;
  bss.band = r.channel <= MAX_CHANNELS_BG ? MG_WIFI_BAND_2G : MG_WIFI_BAND_5G;
  bss.security = r.wep ? MG_WIFI_SECURITY_WEP : MG_WIFI_SECURITY_OPEN;
  if (r.wpa) bss.security |= MG_WIFI_SECURITY_WPA;
  if (r.wpa2) bss.security |= MG_WIFI_SECURITY_WPA2;
  // TODO(scaprile): add WPA3, enterprise
  MG_VERBOSE(("BSS: %.*s (%u) (%M) %d dBm %u", bss.SSID.len, bss.SSID.buf,
              bss.channel, mg_print_mac, bss.BSSID, (int) bss.RSSI,
              bss.security));
  evcb(s_ifp, MG_TCPIP_EV_WIFI_SCAN_RESULT, &bss);
}

// THIS FUNCTION GETS CALLED FROM wlcmgr_task TASK CONTEXT
static int get_scan(unsigned int count) {
  s_scan_results = (uint8_t) count;
  return WM_SUCCESS;
}

bool mg_wifi_scan(void) {
  s_scan_results = 0;
  s_scanning = (wlan_scan(&get_scan) == WM_SUCCESS);
  return s_scanning;
}


bool netif_connect(struct mg_wifi_data *wifi) {
  struct wlan_network sta;
  MG_DEBUG(("Connecting to '%s'", wifi->ssid));
  memset(&sta, 0, sizeof(sta));
  sta.security.type = WLAN_SECURITY_WILDCARD;
  sta.security.mfpc = true;
  sta.security.mfpr = true;
  sta.security.password_len = strlen(wifi->pass);
  strcpy(sta.security.password, wifi->pass);
  sta.security.psk_len = sta.security.password_len;
  strcpy(sta.security.psk, wifi->pass);
  strcpy(sta.name, STA_NWKLBL);
  strcpy(sta.ssid, wifi->ssid);
  sta.ip.ipv4.addr_type = ADDR_TYPE_DHCP;
  sta.ssid_specific = 1;
  if (wlan_add_network(&sta) != WM_SUCCESS) return false;
  s_connecting = false;
  s_ok = s_fail = false;
  if (wlan_connect(STA_NWKLBL) == WM_SUCCESS) {
    s_connecting = true;
  } else {
    enum wlan_connection_state s = WLAN_DISCONNECTED;
    if (wlan_get_connection_state(&s) == WM_SUCCESS && s != WLAN_DISCONNECTED)
      wlan_disconnect();
    wlan_remove_network(STA_NWKLBL);
  }
  return s_connecting;
}

bool mg_wifi_disconnect(void) {
  enum wlan_connection_state s = WLAN_DISCONNECTED;
  return (wlan_get_connection_state(&s) == WM_SUCCESS &&
          s != WLAN_DISCONNECTED && wlan_disconnect() == WM_SUCCESS &&
          wlan_remove_network(STA_NWKLBL) == WM_SUCCESS);
}

bool mg_wifi_ap_start(struct mg_wifi_data *wifi) {
  struct wlan_network ap;
  MG_DEBUG(("Starting AP '%s' (%u)", wifi->apssid, wifi->apchannel));
  wlan_initialize_uap_network(&ap);
  strcpy(ap.ssid, wifi->apssid);
  ap.channel = wifi->apchannel;
  ap.security.type = WLAN_SECURITY_WPA2;
  ap.security.psk_len = strlen(wifi->appass);
  strcpy(ap.security.psk, wifi->appass);
  wlan_add_network(&ap); // error return values won't help, more resilient
  s_ok = s_fail = false;
  if (wlan_start_network(ap.name) == WM_SUCCESS) {
    while (!s_fail) {
      if (s_ok) return true;
      OSA_TimeDelay(1);
    }
  }
  wlan_remove_network(AP_NWKLBL); // error return values won't help
  return false;
}

bool mg_wifi_ap_stop(void) {
  s_ok = s_fail = false;
  if (wlan_stop_network(AP_NWKLBL) != WM_SUCCESS) return false;
  while (!s_ok) {
    if (s_fail) return false;
    OSA_TimeDelay(1);
  }
  wlan_remove_network(AP_NWKLBL); // error return values won't help
  return true;
}
