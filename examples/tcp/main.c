// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_lsn = "tcp://localhost:8765";   // Listening address
static const char *s_conn = "tcp://localhost:8765";  // Connect to address

// client resources
static struct c_res_s {
  int i;
  struct mg_connection *c;
} c_res;

// Self signed certificates
// https://mongoose.ws/documentation/tutorials/tls/#self-signed-certificates
static const char *s_tls_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBqjCCAU+gAwIBAgIUESoOPGqMhf9uarzblVFwzrQweMcwCgYIKoZIzj0EAwIw\n"
    "RDELMAkGA1UEBhMCSUUxDzANBgNVBAcMBkR1YmxpbjEQMA4GA1UECgwHQ2VzYW50\n"
    "YTESMBAGA1UEAwwJVGVzdCBSb290MCAXDTIwMDUwOTIxNTE0NFoYDzIwNTAwNTA5\n"
    "MjE1MTQ0WjBEMQswCQYDVQQGEwJJRTEPMA0GA1UEBwwGRHVibGluMRAwDgYDVQQK\n"
    "DAdDZXNhbnRhMRIwEAYDVQQDDAlUZXN0IFJvb3QwWTATBgcqhkjOPQIBBggqhkjO\n"
    "PQMBBwNCAAQsq9ECZiSW1xI+CVBP8VDuUehVA166sR2YsnJ5J6gbMQ1dUCH/QvLa\n"
    "dBdeU7JlQcH8hN5KEbmM9BnZxMor6ussox0wGzAMBgNVHRMEBTADAQH/MAsGA1Ud\n"
    "DwQEAwIBrjAKBggqhkjOPQQDAgNJADBGAiEAnHFsAIwGQQyRL81B04dH6d86Iq0l\n"
    "fL8OKzndegxOaB0CIQCPwSIwEGFdURDqCC0CY2dnMrUGY5ZXu3hHCojZGS7zvg==\n"
    "-----END CERTIFICATE-----\n";

static const char *s_tls_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBhzCCASygAwIBAgIUbnMoVd8TtWH1T09dANkK2LU6IUswCgYIKoZIzj0EAwIw\n"
    "RDELMAkGA1UEBhMCSUUxDzANBgNVBAcMBkR1YmxpbjEQMA4GA1UECgwHQ2VzYW50\n"
    "YTESMBAGA1UEAwwJVGVzdCBSb290MB4XDTIwMDUwOTIxNTE0OVoXDTMwMDUwOTIx\n"
    "NTE0OVowETEPMA0GA1UEAwwGc2VydmVyMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcD\n"
    "QgAEkuBGnInDN6l06zVVQ1VcrOvH5FDu9MC6FwJc2e201P8hEpq0Q/SJS2nkbSuW\n"
    "H/wBTTBaeXN2uhlBzMUWK790KKMvMC0wCQYDVR0TBAIwADALBgNVHQ8EBAMCA6gw\n"
    "EwYDVR0lBAwwCgYIKwYBBQUHAwEwCgYIKoZIzj0EAwIDSQAwRgIhAPo6xx7LjCdZ\n"
    "QY133XvLjAgVFrlucOZHONFVQuDXZsjwAiEAzHBNligA08c5U3SySYcnkhurGg50\n"
    "BllCI0eYQ9ggp/o=\n"
    "-----END CERTIFICATE-----\n";

static const char *s_tls_key =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQglNni0t9Dg9icgG8w\n"
    "kbfxWSS+TuNgbtNybIQXcm3NHpmhRANCAASS4EacicM3qXTrNVVDVVys68fkUO70\n"
    "wLoXAlzZ7bTU/yESmrRD9IlLaeRtK5Yf/AFNMFp5c3a6GUHMxRYrv3Qo\n"
    "-----END PRIVATE KEY-----\n";

// CLIENT event handler
static void cfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int *i = &((struct c_res_s *) fn_data)->i;
  if (ev == MG_EV_OPEN) {
    MG_INFO(("CLIENT has been initialized"));
  } else if (ev == MG_EV_CONNECT) {
    MG_INFO(("CLIENT connected"));
#if MG_TLS
    struct mg_str host = mg_url_host(s_conn);
    mg_tls_init(c, host);
#endif
    *i = 1;  // do something
  } else if (ev == MG_EV_READ) {
    struct mg_iobuf *r = &c->recv;
    MG_INFO(("CLIENT got data: %.*s", r->len, r->buf));
    r->len = 0;  // Tell Mongoose we've consumed data
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("CLIENT disconnected"));
    // signal we are done
    ((struct c_res_s *) fn_data)->c = NULL;
  } else if (ev == MG_EV_ERROR) {
    MG_INFO(("CLIENT error: %s", (char *) ev_data));
  } else if (ev == MG_EV_POLL && *i != 0) {
    switch ((*i)++) {
      case 50:  // 50 x 100ms = 5s
        mg_send(c, "Hi, there", 9);
        MG_INFO(("CLIENT sent data"));
        break;
      case 100:  // another 5s
        // send any possible outstanding data and close the connection
        c->is_draining = 1;
        break;
    }
  }
}

// SERVER event handler
static void sfn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN && c->is_listening == 1) {
    MG_INFO(("SERVER is listening"));
  } else if (ev == MG_EV_ACCEPT) {
    MG_INFO(("SERVER accepted a connection"));
#if MG_TLS
    mg_tls_init(c, mg_str(""));
#endif
  } else if (ev == MG_EV_READ) {
    struct mg_iobuf *r = &c->recv;
    MG_INFO(("SERVER got data: %.*s", r->len, r->buf));
    mg_send(c, r->buf, r->len);  // echo it back
    r->len = 0;                  // Tell Mongoose we've consumed data
  } else if (ev == MG_EV_CLOSE) {
    MG_INFO(("SERVER disconnected"));
  } else if (ev == MG_EV_ERROR) {
    MG_INFO(("SERVER error: %s", (char *) ev_data));
  }
  (void) fn_data;
}

// Timer function - recreate client connection if it is closed
static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  if (c_res.c == NULL) {
    // connect
    c_res.i = 0;
    c_res.c = mg_connect(mgr, s_conn, cfn, &c_res);
    if (c_res.c == NULL)
      MG_INFO(("CLIENT cant' open a connection"));
    else
      MG_INFO(("CLIENT is connecting"));
  }
}

int main(void) {
  struct mg_mgr mgr;  // Event manager
  struct mg_connection *c;

  mg_log_set(MG_LL_INFO);  // Set log level
  mg_mgr_init(&mgr);       // Initialize event manager

  struct mg_tls_opts opts = {.client_ca = mg_str(s_tls_ca),
                             .server_cert = mg_str(s_tls_cert),
                             .server_key = mg_str(s_tls_key)};
  mg_tls_ctx_init(&mgr, &opts);

  mg_timer_add(&mgr, 15000, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, timer_fn, &mgr);
  c = mg_listen(&mgr, s_lsn, sfn, NULL);  // Create server connection
  if (c == NULL) {
    MG_INFO(("SERVER cant' open a connection"));
    return 0;
  }
  while (true)
    mg_mgr_poll(&mgr, 100);  // Infinite event loop, blocks for upto 100ms
                             // unless there is network activity
  mg_mgr_free(&mgr);         // Free resources
  return 0;
}
