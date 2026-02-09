// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static int s_debug_level = MG_LL_DEBUG;
static const char *s_http_addr = "http://0.0.0.0:8000";
static const char *s_https_addr = "https://0.0.0.0:8443";

// Self signed certificates, see
// https://github.com/cesanta/mongoose/blob/master/test/certs/generate.sh
static const char *s_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBMTCB2aADAgECAgkAluqkgeuV/zUwCgYIKoZIzj0EAwIwEzERMA8GA1UEAwwI\n"
    "TW9uZ29vc2UwHhcNMjQwNTA3MTQzNzM2WhcNMzQwNTA1MTQzNzM2WjARMQ8wDQYD\n"
    "VQQDDAZzZXJ2ZXIwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASo3oEiG+BuTt5y\n"
    "ZRyfwNr0C+SP+4M0RG2pYkb2v+ivbpfi72NHkmXiF/kbHXtgmSrn/PeTqiA8M+mg\n"
    "BhYjDX+zoxgwFjAUBgNVHREEDTALgglsb2NhbGhvc3QwCgYIKoZIzj0EAwIDRwAw\n"
    "RAIgTXW9MITQSwzqbNTxUUdt9DcB+8pPUTbWZpiXcA26GMYCIBiYw+DSFMLHmkHF\n"
    "+5U3NXW3gVCLN9ntD5DAx8LTG8sB\n"
    "-----END CERTIFICATE-----\n";

static const char *s_tls_key =
    "-----BEGIN EC PRIVATE KEY-----\n"
    "MHcCAQEEIAVdo8UAScxG7jiuNY2UZESNX/KPH8qJ0u0gOMMsAzYWoAoGCCqGSM49\n"
    "AwEHoUQDQgAEqN6BIhvgbk7ecmUcn8Da9Avkj/uDNERtqWJG9r/or26X4u9jR5Jl\n"
    "4hf5Gx17YJkq5/z3k6ogPDPpoAYWIw1/sw==\n"
    "-----END EC PRIVATE KEY-----\n";

// HTTPS listener serves requests
static void https_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->is_tls) {
    struct mg_tls_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.cert = mg_str(s_tls_cert);
    opts.key = mg_str(s_tls_key);
    mg_tls_init(c, &opts);
  }

  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/api/hello"), NULL)) {
      mg_http_reply(c, 200, "", "{%m:%d}\n", MG_ESC("status"), 1);
    } else {
      mg_http_reply(c, 200, "", "Hi! Curent time is %llu", mg_now());
    }
  }
}

// HTTP listener sends redirects to HTTPS
static void http_ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char buf[256];
    mg_snprintf(buf, sizeof(buf), "Location: https://%M:%u%.*s\r\n",
                mg_print_ip, &c->loc, mg_url_port(s_https_addr), hm->uri.len,
                hm->uri.buf);
    // If you want to redirect to a name, you must provide a redirection URL:
    // - static const char *s_https_redirect_addr = "https://yourname:port"
    // - mg_snprintf(buf, sizeof(buf), "Location: %s%.*s\r\n",
    //               s_https_redirect_addr, hm->uri.len, hm->uri.buf);
    mg_http_reply(c, 302, buf, "%s", buf);  // 302 redirect
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_log_set(s_debug_level);
  mg_mgr_init(&mgr);

  if (mg_http_listen(&mgr, s_http_addr, http_ev_handler, NULL) == NULL) {
    exit(EXIT_FAILURE);
  }

  if (mg_http_listen(&mgr, s_https_addr, https_ev_handler, NULL) == NULL) {
    exit(EXIT_FAILURE);
  }

  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("HTTP listener    : %s", s_http_addr));
  MG_INFO(("HTTPS listener   : %s", s_https_addr));
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);
  return 0;
}
