// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

#define WIFI_NAME "YOUR_WIFI_NETWORK"   // SET THIS!
#define WIFI_PASS "YOUR_WIFI_PASSWORD"  // SET THIS!

// Access point setup
#define ENABLE_AP false                  // Change to true to enable
#define WIFI_AP_NAME "YOUR_AP_NETWORK"   // SET THIS!
#define WIFI_AP_PASS "YOUR_AP_PASSWORD"  // SET THIS!

// mif user states
enum { AP, SCANNING, STOPPING_AP, CONNECTING, READY };
static unsigned int state;

static void mif_fn(struct mg_tcpip_if *ifp, int ev, void *ev_data) {
  // TODO(): should we include this inside ifp ? add an fn_data ?
  if (ev == MG_TCPIP_EV_STATE_CHANGE) {
    MG_INFO(("State change: %u", *(uint8_t *) ev_data));
  }
  switch (state) {
    case AP:  // we are in AP mode, wait for a user connection to trigger a scan
              // or a connection to a network
      if (ev == MG_TCPIP_EV_STATE_CHANGE &&
          *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("Access Point READY !"));

        // simulate user request to scan for networks
        bool res = mg_wifi_scan();
        MG_INFO(("Starting scan: %s", res ? "OK" : "FAIL"));
        if (res) state = SCANNING;
      }
      break;
    case SCANNING:
      if (ev == MG_TCPIP_EV_WIFI_SCAN_RESULT) {
        struct mg_wifi_scan_bss_data *bss =
            (struct mg_wifi_scan_bss_data *) ev_data;
        MG_INFO(("BSS: %.*s (%u) (%M) %d dBm %u", bss->SSID.len, bss->SSID.buf,
                 bss->channel, mg_print_mac, bss->BSSID, (int) bss->RSSI,
                 bss->security));
      } else if (ev == MG_TCPIP_EV_WIFI_SCAN_END) {
        struct mg_tcpip_driver_pico_w_data *d =
            (struct mg_tcpip_driver_pico_w_data *) ifp->driver_data;
        MG_INFO(("Wi-Fi scan finished"));

        // simulate user selection of a network (1/2: stop AP)
        bool res = mg_wifi_ap_stop();
        MG_INFO(("Manually stopping AP: %s", res ? "OK" : "FAIL"));
        if (res) state = STOPPING_AP;
        // else we have a hw/fw problem
      }
      break;
    case STOPPING_AP:
      if (ev == MG_TCPIP_EV_STATE_CHANGE &&
          *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_wifi_data *wifi =
            &((struct mg_tcpip_driver_pico_w_data *) ifp->driver_data)->wifi;
        wifi->apmode = false;

        // simulate user selection of a network (2/2: actual connect)
        bool res = mg_wifi_connect(wifi);
        MG_INFO(("Manually connecting: %s", res ? "OK" : "FAIL"));
        if (res) {
          state = CONNECTING;
        }  // else manually start AP as below
      }
      break;
    case CONNECTING:
      if (ev == MG_TCPIP_EV_STATE_CHANGE &&
          *(uint8_t *) ev_data == MG_TCPIP_STATE_READY) {
        MG_INFO(("READY!"));
        state = READY;

        // simulate user code disconnection and go back to AP mode (1/2:
        // disconnect)
        bool res = mg_wifi_disconnect();
        MG_INFO(("Manually disconnecting: %s", res ? "OK" : "FAIL"));
      } else if (ev == MG_TCPIP_EV_WIFI_CONNECT_ERR) {
        MG_ERROR(("Wi-Fi connect failed"));
        // manually start AP as below
      }
      break;
    case READY:
      // go back to AP mode after a disconnection (simulation 2/2), you could
      // retry
      if (ev == MG_TCPIP_EV_STATE_CHANGE &&
          *(uint8_t *) ev_data == MG_TCPIP_STATE_DOWN) {
        struct mg_wifi_data *wifi =
            &((struct mg_tcpip_driver_pico_w_data *) ifp->driver_data)->wifi;
        bool res = mg_wifi_ap_start(wifi);
        MG_INFO(("Disconnected"));
        MG_INFO(("Manually starting AP: %s", res ? "OK" : "FAIL"));
        if (res) {
          state = AP;
          wifi->apmode = true;
        }
      }
      break;
  }
}

void wifi_setconfig(void *ifp, void *driver_data) {
  struct mg_tcpip_driver_pico_w_data *d =
      (struct mg_tcpip_driver_pico_w_data *) driver_data;
  struct mg_wifi_data *wifi = &d->wifi;
  wifi->ssid = WIFI_NAME;
  wifi->pass = WIFI_PASS;
  wifi->apssid = WIFI_AP_NAME;
  wifi->appass = WIFI_AP_PASS;
  wifi->apip = MG_IPV4(192, 168, 111, 1);
  wifi->apmask = MG_IPV4(255, 255, 255, 0);
  wifi->security = 0;
  wifi->apsecurity = 0;
  wifi->apchannel = 10;
  wifi->apmode = ENABLE_AP;

  ((struct mg_tcpip_if *) ifp)->fn = mif_fn;
}
