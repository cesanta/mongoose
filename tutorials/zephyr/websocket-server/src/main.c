// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"
#include <zephyr/net/net_mgmt.h>

static const char *s_web_dir = "/";
static const char *s_ws_addr = "ws://0.0.0.0:8000";
static const char *s_wss_addr = "wss://0.0.0.0:8443";

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   /api/rest - respond with JSON string {"result": 123}
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  } else if (ev == MG_EV_ACCEPT && c->is_tls) {
    struct mg_tls_opts opts = {.cert = (char *) s_ssl_cert, .key = (char *) s_ssl_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/websocket"), NULL)) {
      // Upgrade to websocket. From now on, a connection is a full-duplex
      // Websocket connection, which will receive MG_EV_WS_MSG events.
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/rest"), NULL)) {
      // Serve REST response
      mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Got websocket frame. Received data is wm->data. Echo it back!
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    MG_INFO(("Got wm: %p, data: %p, %d = %*.s", wm, wm->data.buf, wm->data.len, wm->data.len, wm->data.buf));
    mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_TEXT);
  }
}

// Zephyr: Define a semaphore and network management callback to be able to wait
// until our IP address is ready. The main function will start and block on this
// semaphore until this event handler releases it when the network is ready
K_SEM_DEFINE(run, 0, 1);

static void zeh(struct net_mgmt_event_callback *cb,
// https://docs.zephyrproject.org/latest/releases/migration-guide-4.2.html#networking
#if ZEPHYR_VERSION_CODE < 0x40200
                uint32_t mgmt_event,
#else
                uint64_t mgmt_event,
#endif
                struct net_if *iface) {
  if (mgmt_event == NET_EVENT_L4_CONNECTED) k_sem_give(&run);
}

int main(int argc, char *argv[]) {
  // Zephyr: Register the network management callback and block on the semaphore
  struct net_mgmt_event_callback ncb;
  net_mgmt_init_event_callback(&ncb, zeh, NET_EVENT_L4_CONNECTED);
  net_mgmt_add_event_callback(&ncb);
  k_sem_take(&run, K_FOREVER);

  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);
  mg_http_listen(&mgr, s_ws_addr, fn, NULL);
  mg_http_listen(&mgr, s_wss_addr, fn, NULL);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_ws_addr));
  MG_INFO(("Listening on     : %s", s_wss_addr));
  MG_INFO(("Web root         : [%s]", s_web_dir));

  uint64_t sntp_timer = 0;
  for (;;) {
    uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 24 * 3600 * 1000;
    if (mg_timer_expired(&sntp_timer, period, mg_millis())) {
      mg_sntp_connect(&mgr, NULL, NULL, NULL);
    }
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
  return 0;
}
