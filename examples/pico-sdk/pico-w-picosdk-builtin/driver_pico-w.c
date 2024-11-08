// Copyright (c) 2024 Cesanta Software Limited
// All rights reserved

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/unique_id.h"
#include "cyw43.h"
#include "cyw43_country.h"

#include "mongoose.h"
#include "driver_pico-w.h"


static struct mg_tcpip_if *s_ifp;

static bool mg_tcpip_driver_pico_w_init(struct mg_tcpip_if *ifp) {
  struct mg_tcpip_driver_pico_w_data *d = (struct mg_tcpip_driver_pico_w_data *) ifp->driver_data;
  s_ifp = ifp;
  // initialize WiFi chip and connect to network
  cyw43_init(&cyw43_state);
  cyw43_wifi_set_up(&cyw43_state, CYW43_ITF_STA, true, CYW43_COUNTRY_WORLDWIDE);
  cyw43_wifi_join(&cyw43_state, strlen(d->ssid), d->ssid, strlen(d->pass), d->pass, CYW43_AUTH_WPA2_AES_PSK, NULL, CYW43_ITF_STA);
  cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA, ifp->mac);
  return true;
}

static size_t mg_tcpip_driver_pico_w_tx(const void *buf, size_t len,
                                             struct mg_tcpip_if *ifp) {
  return cyw43_send_ethernet(&cyw43_state, CYW43_ITF_STA, len, buf, false) ? 0 : len;
  (void) ifp;
}

static bool mg_tcpip_driver_pico_w_up(struct mg_tcpip_if *ifp) {
  return cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_JOIN ? 1 : 0;
}

void driver_pico_w_poll(void) {
  cyw43_poll();
}

struct mg_tcpip_driver mg_tcpip_driver_pico_w = {
  mg_tcpip_driver_pico_w_init,
  mg_tcpip_driver_pico_w_tx,
  NULL,
  mg_tcpip_driver_pico_w_up,
};

void cyw43_cb_tcpip_init(cyw43_t *self, int itf) {}
void cyw43_cb_tcpip_deinit(cyw43_t *self, int itf) {}
void cyw43_cb_tcpip_set_link_up(cyw43_t *self, int itf) {}
void cyw43_cb_tcpip_set_link_down(cyw43_t *self, int itf) {}

// Called once per outstanding frame during a call to cyw43_poll
void cyw43_cb_process_ethernet(void *cb_data, int itf, size_t len, const uint8_t *buf) {
  if (itf != CYW43_ITF_STA) return;
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  (void) cb_data;
}

// Return mac address
void cyw43_hal_get_mac(__unused int idx, uint8_t buf[6]) {
  memcpy(buf, cyw43_state.mac, 6);
}

// Generate a mac address if one is not set in otp
void cyw43_hal_generate_laa_mac(__unused int idx, uint8_t buf[6]) {
  pico_unique_board_id_t board_id;
  MG_DEBUG(("No MAC in cyw43 OTP, generated from board id"));
  pico_get_unique_board_id(&board_id);
  memcpy(buf, &board_id.id[2], 6);
  buf[0] &= (uint8_t)~0x1; // unicast
  buf[0] |= 0x2; // locally administered
}

// there's life beyond lwIP
void pbuf_copy_partial(void){(void)0;}
