// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"
#include <zephyr/net/net_mgmt.h>

static const char *s_url = "https://example.org/";
static const char *s_post_data = NULL;     // POST data
static const int64_t s_timeout_ms = 1500;  // Connect timeout in milliseconds
struct mg_mgr s_mgr;
static int s_connected = 0;
static bool done = false;

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_OPEN) {
    // Connection created. Store connect expiration time in c->data
    *(int64_t *) c->data = mg_millis() + s_timeout_ms;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *(int64_t *) c->data &&
        (c->is_connecting || c->is_resolving)) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);

    // If s_url is https://, init TLS client connection
    if (c->is_tls) {
      struct mg_tls_opts opts = {.ca = s_ca, .name = host};
      mg_tls_init(c, &opts);
    }

    // Send request
    int content_length = s_post_data ? strlen(s_post_data) : 0;
    mg_printf(c,
              "%s %s HTTP/1.0\r\n"
              "Host: %.*s\r\n"
              "Content-Type: octet-stream\r\n"
              "Content-Length: %d\r\n"
              "\r\n",
              s_post_data ? "POST" : "GET", mg_url_uri(s_url), (int) host.len,
              host.buf, content_length);
    mg_send(c, s_post_data, content_length);
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("%.*s", (int) hm->message.len, hm->message.buf));
    c->is_draining = 1;        // Tell mongoose to close this connection
    *(bool *) c->fn_data = true;  // Tell event loop to stop
  } else if (ev == MG_EV_ERROR) {
    *(bool *) c->fn_data = true;  // Error, tell event loop to stop
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

  mg_log_set(MG_LL_DEBUG);
  mg_mgr_init(&s_mgr);

  MG_INFO(("Mongoose version : v%s", MG_VERSION));

  uint64_t sntp_timer = 0;
  while (!done) {
    uint64_t period = mg_boot_timestamp_ms == 0 ? 1000 : 24 * 3600 * 1000;
    if (mg_timer_expired(&sntp_timer, period, mg_millis())) {
      mg_sntp_connect(&s_mgr, NULL, NULL, NULL);
    }
    // Initiate the HTTPS request once we have a valid time (needed for TLS)
    if (mg_boot_timestamp_ms > 0 && !s_connected) {
      MG_INFO(("Connecting to    : [%s]", s_url));
      mg_http_connect(&s_mgr, s_url, fn, &done);
      s_connected = 1;
    }
    mg_mgr_poll(&s_mgr, 1000);
  }
  mg_mgr_free(&s_mgr);

  return 0;
}
