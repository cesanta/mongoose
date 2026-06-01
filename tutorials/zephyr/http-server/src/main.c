// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"
#include <zephyr/net/net_mgmt.h>

static const char *s_web_dir = "/";
static const char *s_http_addr = "http://0.0.0.0:8000";
static const char *s_https_addr = "https://0.0.0.0:8443";

// Event handler for the listening HTTP/HTTPS connection.
static void wcb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->is_tls) {
    struct mg_tls_opts opts = {.cert = (char *) s_ssl_cert, .key = (char *) s_ssl_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    MG_INFO(("%.*s %.*s %ld", (int) hm->method.len, hm->method.buf,
             (int) hm->uri.len, hm->uri.buf, (long) hm->body.len));
    if (mg_match(hm->uri, mg_str("/api/#"), NULL)) {  // REST API requests
      // Print some statistics about currently established connections
      mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
      mg_http_printf_chunk(c, "ID PROTO TYPE      LOCAL           REMOTE\n");
      for (struct mg_connection *t = c->mgr->conns; t != NULL; t = t->next) {
        mg_http_printf_chunk(c, "%-3lu %4s %s %M %M\n", t->id,
                             t->is_udp ? "UDP" : "TCP",
                             t->is_listening  ? "LISTENING"
                             : t->is_accepted ? "ACCEPTED "
                                              : "CONNECTED",
                             mg_print_ip, &t->loc, mg_print_ip, &t->rem);
      }
      mg_http_printf_chunk(c, "");  // Don't forget the last empty chunk
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_web_dir};  // Serve
      mg_http_serve_dir(c, hm, &opts);                           // static files
    }
  } else if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
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
  mg_http_listen(&mgr, s_http_addr, wcb, NULL);
  mg_http_listen(&mgr, s_https_addr, wcb, NULL);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_http_addr));
  MG_INFO(("Listening on     : %s", s_https_addr));
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
