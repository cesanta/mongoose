// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// This example shows how we can poll an external HTTP server periodically, and
// make sure that not more than one instance of the active request in flight.
// Each request has a configured timeout.

#include "mongoose.h"

static const char *s_url = "https://mongoose.ws/tls/api/ca";
static const char *s_post_data = "hi";
static const uint64_t s_connect_timeout_ms = 5500;  // milliseconds
static struct mg_connection *s_conn = NULL;

// TLS certificate, obtained using https://mongoose.ws/tls/
#define TLS_CA                                                         \
  "-----BEGIN CERTIFICATE-----\n"                                      \
  "MIIEVjCCAj6gAwIBAgIQY5WTY8JOcIJxWRi/w9ftVjANBgkqhkiG9w0BAQsFADBP\n" \
  "MQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFy\n" \
  "Y2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMTAeFw0yNDAzMTMwMDAwMDBa\n" \
  "Fw0yNzAzMTIyMzU5NTlaMDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBF\n" \
  "bmNyeXB0MQswCQYDVQQDEwJFODB2MBAGByqGSM49AgEGBSuBBAAiA2IABNFl8l7c\n" \
  "S7QMApzSsvru6WyrOq44ofTUOTIzxULUzDMMNMchIJBwXOhiLxxxs0LXeb5GDcHb\n" \
  "R6EToMffgSZjO9SNHfY9gjMy9vQr5/WWOrQTZxh7az6NSNnq3u2ubT6HTKOB+DCB\n" \
  "9TAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMB\n" \
  "MBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFI8NE6L2Ln7RUGwzGDhdWY4j\n" \
  "cpHKMB8GA1UdIwQYMBaAFHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEB\n" \
  "BCYwJDAiBggrBgEFBQcwAoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzATBgNVHSAE\n" \
  "DDAKMAgGBmeBDAECATAnBgNVHR8EIDAeMBygGqAYhhZodHRwOi8veDEuYy5sZW5j\n" \
  "ci5vcmcvMA0GCSqGSIb3DQEBCwUAA4ICAQBnE0hGINKsCYWi0Xx1ygxD5qihEjZ0\n" \
  "RI3tTZz1wuATH3ZwYPIp97kWEayanD1j0cDhIYzy4CkDo2jB8D5t0a6zZWzlr98d\n" \
  "AQFNh8uKJkIHdLShy+nUyeZxc5bNeMp1Lu0gSzE4McqfmNMvIpeiwWSYO9w82Ob8\n" \
  "otvXcO2JUYi3svHIWRm3+707DUbL51XMcY2iZdlCq4Wa9nbuk3WTU4gr6LY8MzVA\n" \
  "aDQG2+4U3eJ6qUF10bBnR1uuVyDYs9RhrwucRVnfuDj29CMLTsplM5f5wSV5hUpm\n" \
  "Uwp/vV7M4w4aGunt74koX71n4EdagCsL/Yk5+mAQU0+tue0JOfAV/R6t1k+Xk9s2\n" \
  "HMQFeoxppfzAVC04FdG9M+AC2JWxmFSt6BCuh3CEey3fE52Qrj9YM75rtvIjsm/1\n" \
  "Hl+u//Wqxnu1ZQ4jpa+VpuZiGOlWrqSP9eogdOhCGisnyewWJwRQOqK16wiGyZeR\n" \
  "xs/Bekw65vwSIaVkBruPiTfMOo0Zh4gVa8/qJgMbJbyrwwG97z/PRgmLKCDl8z3d\n" \
  "tA0Z7qq7fta0Gl24uyuB05dqI5J1LvAzKuWdIjT1tP8qCoxSE/xpix8hX2dt3h+/\n" \
  "jujUgFPFZ0EVZ0xSyBNRF3MboGZnYXFUxpNjTWPKpagDHJQmqrAcDmWJnMsFY3jS\n" \
  "u1igv3OefnWjSQ==\n"                                                 \
  "-----END CERTIFICATE-----"

static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  uint64_t *start_time = (uint64_t *) c->data;  // Stored start time
  bool *success = (bool *) (start_time + 1);    // Stored success flag
  if (ev == MG_EV_OPEN) {
    *start_time = mg_millis();
    MG_DEBUG(("%lu Connection started, time: %llu", c->id, *start_time));
  } else if (ev == MG_EV_POLL) {
    if (*start_time + s_connect_timeout_ms < mg_millis()) {
      mg_error(c, "Connect timeout");
    }
  } else if (ev == MG_EV_CONNECT) {
    // Connected to server. Extract host name from URL
    struct mg_str host = mg_url_host(s_url);
    if (c->is_tls) {
      struct mg_tls_opts opts = {};
      opts.ca = mg_str(TLS_CA);
      opts.name = host;
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
              s_post_data ? "POST" : "GET", mg_url_uri(s_url), host.len,
              host.buf, content_length);
    mg_send(c, s_post_data, content_length);
  } else if (ev == MG_EV_HTTP_MSG) {
    // Response is received. Print it
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("%lu Got response %lu bytes %lu", c->id, hm->body.len, c->rtls.len));
    *success = true;
    c->is_closing = 1;
  } else if (ev == MG_EV_ERROR) {
    MG_ERROR(("%lu Connection error: %s", c->id, ev_data));
  } else if (ev == MG_EV_CLOSE) {
    MG_DEBUG(("%lu Connection closed %lu", c->id, mg_millis() - *start_time));
    if (*success == false) MG_ERROR(("%lu NO RESPONSE %lu", c->id, c->rtls.len));
    if (c->is_draining == 0) s_conn = NULL;
  }
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  // If connection is already there, do nothing. Otherwise, start it
  if (s_conn != NULL) {
    MG_DEBUG(("Connection is already active..."));
  } else {
    s_conn = mg_http_connect(mgr, s_url, http_ev_handler, NULL);
  }
}

int main(void) {
  struct mg_mgr mgr;
  mg_log_set(MG_LL_INFO);
  mg_mgr_init(&mgr);

  // Attempt to start a POST request frequently
  mg_timer_add(&mgr, 50, MG_TIMER_REPEAT, timer_fn, &mgr);

  for (;;) mg_mgr_poll(&mgr, 10);
  mg_mgr_free(&mgr);

  return 0;
}
