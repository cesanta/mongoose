#if MG_ENABLE_TCPIP && MG_ARCH == MG_ARCH_PICOSDK && \
    defined(MG_ENABLE_DRIVER_PICO_W) && MG_ENABLE_DRIVER_PICO_W

#include "drivers/pico-w.h"
#include "net_builtin.h"
#include "pico/stdlib.h"

static struct mg_tcpip_if *s_ifp;
static uint32_t s_ip, s_mask;
static bool s_aplink = false, s_scanning = false;
static bool s_stalink = false, s_connecting = false;

static void wifi_cb(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  struct mg_wifi_data *wifi =
      &((struct mg_tcpip_driver_pico_w_data *) ifp->driver_data)->wifi;
  if (wifi->apmode && ev == MG_TCPIP_EV_ST_CHG &&
      *(uint8_t *) ev_data == MG_TCPIP_STATE_UP) {
    MG_DEBUG(("Access Point started"));
    s_ip = ifp->ip, ifp->ip = wifi->apip;
    s_mask = ifp->mask, ifp->mask = wifi->apmask;
    ifp->enable_dhcp_client = false;
    ifp->enable_dhcp_server = true;
  }
}

static bool mg_tcpip_driver_pico_w_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_pico_w_data *d =
      (struct mg_tcpip_driver_pico_w_data *) ifp->driver_data;
  struct mg_wifi_data *wifi = &d->wifi;
  s_ifp = ifp;
  s_ip = ifp->ip;
  s_mask = ifp->mask;
  ifp->pfn = wifi_cb;
  if (cyw43_arch_init() != 0)
    return false;  // initialize async_context and WiFi chip
  if (wifi->apmode && wifi->apssid != NULL) {
    if (!mg_wifi_ap_start(wifi)) return false;
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA, ifp->mac);  // same MAC
  } else {
    cyw43_arch_enable_sta_mode();
    cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA, ifp->mac);
    if (wifi->ssid != NULL) {
      return mg_wifi_connect(wifi);
    } else {
      cyw43_arch_disable_sta_mode();
    }
  }
  return true;
}

static size_t mg_tcpip_driver_pico_w_tx(const void *buf, size_t len,
                                        struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_pico_w_data *d =
      (struct mg_tcpip_driver_pico_w_data *) ifp->driver_data;
  return cyw43_send_ethernet(&cyw43_state,
                             d->wifi.apmode ? CYW43_ITF_AP : CYW43_ITF_STA, len,
                             buf, false) == 0
             ? len
             : 0;
}

static bool mg_tcpip_driver_pico_w_poll(struct mg_tcpip_if *ifp, bool s1) {
  cyw43_arch_poll();  // not necessary, except when IRQs are disabled (OTA)
  if (s_scanning && !cyw43_wifi_scan_active(&cyw43_state)) {
    MG_VERBOSE(("scan complete"));
    s_scanning = 0;
    mg_tcpip_call(ifp, MG_TCPIP_EV_WIFI_SCAN_END, NULL);
  }
  if (ifp->update_mac_hash_table) {
    // first call to _poll() is after _init(), so this is safe
    cyw43_wifi_update_multicast_filter(&cyw43_state, (uint8_t *) mcast_addr,
                                       true);
    ifp->update_mac_hash_table = false;
  }
  if (!s1) return false;
  struct mg_tcpip_driver_pico_w_data *d =
      (struct mg_tcpip_driver_pico_w_data *) ifp->driver_data;
  if (d->wifi.apmode) return s_aplink;
  int sdkstate = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
  MG_VERBOSE(("conn: %c state: %d", s_connecting ? '1' : '0', sdkstate));
  if (sdkstate < 0 && s_connecting) {
    mg_tcpip_call(ifp, MG_TCPIP_EV_WIFI_CONNECT_ERR, &sdkstate);
    s_connecting = false;
  }
  return s_stalink;
}

struct mg_tcpip_driver mg_tcpip_driver_pico_w = {
    mg_tcpip_driver_pico_w_init,
    mg_tcpip_driver_pico_w_tx,
    NULL,
    mg_tcpip_driver_pico_w_poll,
};

// Called once per outstanding frame by async_context
void cyw43_cb_process_ethernet(void *cb_data, int itf, size_t len,
                               const uint8_t *buf) {
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  (void) cb_data;
}

// Called by async_context
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf) {
  if (itf == CYW43_ITF_AP) {
    s_aplink = true;
  } else {
    s_stalink = true;
    s_connecting = false;
  }
}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf) {
  if (itf == CYW43_ITF_AP) {
    s_aplink = false;
  } else {
    s_stalink = false;
    // SDK calls this before we check status, don't clear s_connecting here
  }
}

// there's life beyond lwIP
void pbuf_copy_partial(void) {
  (void) 0;
}

static int result_cb(void *arg, const cyw43_ev_scan_result_t *data) {
  struct mg_wifi_scan_bss_data bss;
  bss.SSID = mg_str_n(data->ssid, data->ssid_len);
  bss.BSSID = (char *) data->bssid;
  bss.RSSI = (int8_t) data->rssi;
  bss.has_n = 0;  // SDK ignores this
  bss.channel = (uint8_t) data->channel;
  bss.band = MG_WIFI_BAND_2G;
  // SDK-internal dependency, 2.1.0
  bss.security = data->auth_mode & MG_BIT(0) ? MG_WIFI_SECURITY_WEP
                                             : MG_WIFI_SECURITY_OPEN;
  if (data->auth_mode & MG_BIT(1)) bss.security |= MG_WIFI_SECURITY_WPA;
  if (data->auth_mode & MG_BIT(2)) bss.security |= MG_WIFI_SECURITY_WPA2;
  MG_VERBOSE(("BSS: %.*s (%u) (%M) %d dBm %u", bss.SSID.len, bss.SSID.buf,
              bss.channel, mg_print_mac, bss.BSSID, (int) bss.RSSI,
              bss.security));
  mg_tcpip_call(s_ifp, MG_TCPIP_EV_WIFI_SCAN_RESULT, &bss);
  return 0;
}

bool mg_wifi_scan(void) {
  cyw43_wifi_scan_options_t opts;
  memset(&opts, 0, sizeof(opts));
  bool res = (cyw43_wifi_scan(&cyw43_state, &opts, NULL, result_cb) == 0);
  if (res) s_scanning = true;
  return res;
}

bool mg_wifi_connect(struct mg_wifi_data *wifi) {
  s_ifp->ip = s_ip;
  s_ifp->mask = s_mask;
  if (s_ifp->ip == 0) s_ifp->enable_dhcp_client = true;
  s_ifp->enable_dhcp_server = false;
  cyw43_arch_enable_sta_mode();
  MG_DEBUG(("Connecting to '%s'", wifi->ssid));
  int res = cyw43_arch_wifi_connect_async(wifi->ssid, wifi->pass,
                                          CYW43_AUTH_WPA2_AES_PSK);
  MG_VERBOSE(("res: %d", res));
  if (res == 0) s_connecting = true;
  return (res == 0);
}

bool mg_wifi_disconnect(void) {
  cyw43_arch_disable_sta_mode();
  s_connecting = false;
  return true;
}

bool mg_wifi_ap_start(struct mg_wifi_data *wifi) {
  MG_DEBUG(("Starting AP '%s' (%u)", wifi->apssid, wifi->apchannel));
  cyw43_wifi_ap_set_channel(&cyw43_state, wifi->apchannel);
  cyw43_arch_enable_ap_mode(wifi->apssid, wifi->appass,
                            CYW43_AUTH_WPA2_AES_PSK);
  return true;
}

bool mg_wifi_ap_stop(void) {
  cyw43_arch_disable_ap_mode();
  return true;
}

#endif
