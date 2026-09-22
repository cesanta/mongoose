// Copyright (c) 2020-2024 Cesanta Software Limited
// All rights reserved
//
// Streaming upload example. Demonstrates how to use mg_http_stream_body()
// to save a large file without buffering it fully in memory.
// The file is saved as /tmp/a.txt.
//
// curl http://localhost:8000/fs/a.txt --data-binary @large_file.txt

#include "mongoose.h"

#define UPLOAD_DIR "/tmp"

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

static bool upload(struct mg_http_message *hm, struct mg_str *data, void **p) {
  struct mg_fs *fs = &mg_fs_posix;
  if (hm != NULL) {
    char path[MG_PATH_MAX];
    MG_INFO(("UPLOAD BEGIN, URI %.*s ", hm->uri.len, hm->uri.buf));
    mg_snprintf(path, sizeof(path), "%s/%.*s", UPLOAD_DIR, hm->uri.len - 4,
                hm->uri.buf + 4);
    *p = NULL;
    if (mg_path_is_sane(mg_str(path))) {
      fs->rm(path);                     // Delete file if it exists
      *p = fs->op(path, MG_FS_WRITE);  // Open file for writing
    }
  } else if (data != NULL) {
    MG_INFO(("UPLOAD DATA, %zu bytes", data->len));
    if (*p) fs->wr(*p, data->buf, data->len);  // Write to file
  } else {
    MG_INFO(("UPLOAD END %p", p));
    if (*p) fs->cl(*p), *p = NULL;
  }
  return true;  // signal success
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->is_tls) {
    struct mg_tls_opts opts = {.cert = mg_str(s_tls_cert),
                               .key = mg_str(s_tls_key)};
    mg_tls_init(c, &opts);
  } else if (ev == MG_EV_HTTP_HDRS) {
    // Handle upload requests to /fs/*
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/fs/*"), NULL)) {
      mg_http_stream_body(c, hm, upload, NULL);
    }
  } else if (ev == MG_EV_HTTP_MSG && c->pfn != NULL) {
    struct mg_http_serve_opts opts = {.root_dir = "web_root"};
    mg_http_serve_dir(c, ev_data, &opts);
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);
  mg_http_listen(&mgr, "https://0.0.0.0:8443", fn, "TLS!");

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
