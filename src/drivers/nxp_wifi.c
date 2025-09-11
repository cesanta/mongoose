#if MG_ENABLE_TCPIP && defined(MG_ENABLE_DRIVER_NXP_WIFI) && \
    MG_ENABLE_DRIVER_NXP_WIFI

#include "drivers/nxp_wifi.h"

bool __attribute__((weak)) netif_init(struct mg_tcpip_if *ifp) {
  (void) ifp;
  MG_ERROR(("Please link wifi/port/net contents"));
  return false;
}
size_t __attribute__((weak))
netif_tx(const void *bfr, size_t len, struct mg_tcpip_if *ifp) {
  (void) bfr;
  (void) len;
  netif_init(ifp);
  return 0;
}
bool __attribute__((weak)) netif_connect(struct mg_wifi_data *wifi) {
  (void) wifi;
  return netif_init(NULL);
}
bool __attribute__((weak))
netif_poll(struct mg_tcpip_if *ifp, bool s1, mg_tcpip_event_handler_t evcb) {
  (void) ifp;
  (void) s1;
  (void) evcb;
  return false;
}

static struct mg_tcpip_if *s_ifp;
static uint32_t s_ip, s_mask;

static void wifi_cb(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  struct mg_wifi_data *wifi =
      &((struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data)->wifi;
  if (wifi->apmode && ev == MG_TCPIP_EV_ST_CHG &&
      *(uint8_t *) ev_data == MG_TCPIP_STATE_UP) {
    MG_DEBUG(("Access Point started"));
    s_ip = ifp->ip, ifp->ip = wifi->apip;
    s_mask = ifp->mask, ifp->mask = wifi->apmask;
    ifp->enable_dhcp_client = false;
    ifp->enable_dhcp_server = true;
  }
}

static bool nxp_wifi_init(struct mg_tcpip_if *ifp) {
  struct mg_wifi_data *wifi =
      &((struct mg_tcpip_driver_nxp_wifi_data *) ifp->driver_data)->wifi;
  s_ifp = ifp;
  s_ip = ifp->ip;
  s_mask = ifp->mask;
  ifp->pfn = wifi_cb;
  if (!netif_init(ifp)) return false;
  if (wifi->apmode) {
    return mg_wifi_ap_start(wifi);
  } else if (wifi->ssid != NULL && wifi->pass != NULL) {
    return mg_wifi_connect(wifi);
  }
  return true;
}

bool nxp_wifi_poll(struct mg_tcpip_if *ifp, bool s1) {
  return netif_poll(ifp, s1, mg_tcpip_call);
}

struct mg_tcpip_driver mg_tcpip_driver_nxp_wifi = {nxp_wifi_init, netif_tx,
                                                   NULL, nxp_wifi_poll};

bool mg_wifi_connect(struct mg_wifi_data *wifi) {
  s_ifp->ip = s_ip;
  s_ifp->mask = s_mask;
  if (s_ifp->ip == 0) s_ifp->enable_dhcp_client = true;
  s_ifp->enable_dhcp_server = false;
  return netif_connect(wifi);
}

bool __attribute__((weak)) mg_wifi_scan(void) {
  return netif_init(NULL);
}
bool __attribute__((weak)) mg_wifi_disconnect(void) {
  return netif_init(NULL);
}
bool __attribute__((weak)) mg_wifi_ap_start(struct mg_wifi_data *wifi) {
  (void) wifi;
  return netif_init(NULL);
}
bool __attribute__((weak)) mg_wifi_ap_stop(void) {
  return netif_init(NULL);
}

#endif
