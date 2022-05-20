// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "certs.h"
#include "mongoose.h"

static const char *s_debug_level = "3";
static time_t s_boot_timestamp = 0;
static struct mg_connection *s_sntp_conn = NULL;
static const char *s_url = "https://example.org/";
static const char *s_post_data = NULL;     // POST data
static const int64_t s_timeout_ms = 1500;  // Connect timeout in milliseconds
struct mg_mgr s_mgr;
static int s_connected = 0;

// Print HTTP response and signal that we're done
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    // Connection created. Store connect expiration time in c->label
    *(int64_t *) c->label = mg_millis() + s_timeout_ms;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *(int64_t *) c->label &&
        (c->is_connecting || c->is_resolving)) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);

    // If s_url is https://, tell client connection to use TLS
    if (mg_url_is_ssl(s_url)) {
      struct mg_tls_opts opts = {.ca = s_ca, .srvname = host};
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
              host.ptr, content_length);
    mg_send(c, s_post_data, content_length);
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("%.*s", (int) hm->message.len, hm->message.ptr));
    c->is_closing = 1;         // Tell mongoose to close this connection
    *(bool *) fn_data = true;  // Tell event loop to stop
  } else if (ev == MG_EV_ERROR) {
    *(bool *) fn_data = true;  // Error, tell event loop to stop
  }
}

// We have no valid system time(), and we need it for TLS. Implement it
time_t time(time_t *tp) {
  time_t t = s_boot_timestamp + k_uptime_get() / 1000;
  if (tp != NULL) *tp = t;
  return t;
}

// SNTP callback. Modifies s_boot_timestamp, to make time() correct
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t t = *(int64_t *) ev_data;
    MG_INFO(("Got SNTP time: %lld ms from epoch", t));
    s_boot_timestamp = (time_t) ((t - mg_millis()) / 1000);

    // We need correct time in order to get HTTPs working, therefore,
    // making https request from SMTP callback
    if (!s_connected) {
      MG_INFO(("Connecting to    : [%s]", s_url));
      mg_http_connect(&s_mgr, s_url, fn, NULL);  // Create client connection
      s_connected = 1;
    }
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

int main(int argc, char *argv[]) {
  mg_log_set(s_debug_level);

  mg_mgr_init(&s_mgr);
  mg_timer_add(&s_mgr, 5000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn,
               &s_mgr);

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));

  for (;;) mg_mgr_poll(&s_mgr, 1000);
  mg_mgr_free(&s_mgr);

  return 0;
}
