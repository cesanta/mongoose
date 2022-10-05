// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"

static const char *s_web_dir = "/";
static const char *s_ws_addr = "ws://0.0.0.0:8000";
static const char *s_wss_addr = "wss://0.0.0.0:8443";
static time_t s_boot_timestamp = 0;
static struct mg_connection *s_sntp_conn = NULL;

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   /api/rest - respond with JSON string {"result": 123}
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  } else if (ev == MG_EV_ACCEPT && fn_data != NULL) {
    struct mg_tls_opts opts = {.cert = s_ssl_cert, .certkey = s_ssl_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/websocket")) {
      // Upgrade to websocket. From now on, a connection is a full-duplex
      // Websocket connection, which will receive MG_EV_WS_MSG events.
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/rest")) {
      // Serve REST response
      mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Got websocket frame. Received data is wm->data. Echo it back!
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    MG_INFO(("Got wm: %p, data: %p, %d = %*.s", wm, wm->data.ptr, wm->data.len, wm->data.len, wm->data.ptr));
    mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
  }
  (void) fn_data;
}

// example system time()-like function
time_t ourtime(time_t *tp) {
  time_t t = s_boot_timestamp + mg_millis() / 1000;
  if (tp != NULL) *tp = t;
  return t;
}

// SNTP callback. Modifies s_boot_timestamp, to make ourtime() correct
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t t = *(int64_t *) ev_data;
    MG_INFO(("Got SNTP time: %lld ms from epoch", t));
    s_boot_timestamp = (time_t) ((t - mg_millis()) / 1000);
  } else if (ev == MG_EV_CLOSE) {
    s_sntp_conn = NULL;
  }
}

// Periodic timer syncs time via SNTP
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (s_sntp_conn == NULL) s_sntp_conn = mg_sntp_connect(mgr, NULL, sfn, NULL);
  if (s_boot_timestamp < 9999) mg_sntp_request(s_sntp_conn);
}

// Zephyr: Define a semaphore and network management callback to be able to wait
// until our IP address is ready. The main function will start and block on this
// semaphore until this event handler releases it when the network is ready
K_SEM_DEFINE(run, 0, 1);

static void zeh(struct net_mgmt_event_callback *cb, uint32_t mgmt_event,
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
  mg_http_listen(&mgr, s_wss_addr, fn, &mgr);
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_ws_addr));
  MG_INFO(("Listening on     : %s", s_wss_addr));
  MG_INFO(("Web root         : [%s]", s_web_dir));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
