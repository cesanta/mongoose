// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"

static const char *s_web_dir = "/";
static const char *s_http_addr = "http://0.0.0.0:8000";
static const char *s_https_addr = "https://0.0.0.0:8443";
static time_t s_boot_timestamp = 0;
static struct mg_connection *s_sntp_conn = NULL;

// Event handler for the listening HTTP/HTTPS connection.
static void wcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ACCEPT && fn_data != NULL) {
    struct mg_tls_opts opts = {.cert = s_ssl_cert, .certkey = s_ssl_key};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = ev_data;
    MG_INFO(("%.*s %.*s %ld", (int) hm->method.len, hm->method.ptr,
             (int) hm->uri.len, hm->uri.ptr, (long) hm->body.len));
    if (mg_http_match_uri(hm, "/api/#")) {  // REST API requests
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

  mg_log_set(MG_LL_DEBUG);

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, s_http_addr, wcb, NULL);
  mg_http_listen(&mgr, s_https_addr, wcb, &mgr);

  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", s_http_addr));
  MG_INFO(("Listening on     : %s", s_https_addr));
  MG_INFO(("Web root         : [%s]", s_web_dir));
  for (;;) mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  return 0;
}
