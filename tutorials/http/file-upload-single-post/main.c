// Copyright (c) 2020-2024 Cesanta Software Limited
// All rights reserved
//
// Streaming upload example. Demonstrates how to use MG_EV_READ events
// to save a large file without buffering it fully in memory.
//
// curl http://localhost:8000/upload?name=a.txt --data-binary @large_file.txt

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

struct upload_state {
  size_t expected;  // POST data length, bytes
  size_t received;  // Already received bytes
  void *fp;         // Opened uploaded file
};

static void handle_uploads(struct mg_connection *c, int ev, void *ev_data) {
  struct upload_state *us = (struct upload_state *) c->data;
  struct mg_fs *fs = &mg_fs_posix;

  // Catch /upload requests early, without buffering whole body
  // When we receive MG_EV_HTTP_HDRS event, that means we've received all
  // HTTP headers but not necessarily full HTTP body
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/upload/*"), NULL)) {
      char path[MG_PATH_MAX];
      mg_snprintf(path, sizeof(path), "%s/%.*s", UPLOAD_DIR, hm->uri.len - 8,
                  hm->uri.buf + 8);
      us->expected = hm->body.len;  // Store number of bytes we expect
      mg_iobuf_del(&c->recv, 0, hm->head.len);  // Delete HTTP headers
      c->pfn = NULL;  // Silence HTTP protocol handler, we'll use MG_EV_READ
      if (mg_path_is_sane(mg_str(path))) {
        fs->rm(path);                        // Delete file if it exists
        us->fp = fs->op(path, MG_FS_WRITE);  // Open file for writing
      }
    }
  }

  // Catch uploaded file data for both MG_EV_READ and MG_EV_HTTP_HDRS
  if (us->expected > 0 && c->recv.len > 0) {
    us->received += c->recv.len;
    if (us->fp) fs->wr(us->fp, c->recv.buf, c->recv.len);  // Write to file
    c->recv.len = 0;  // Delete received data
    if (us->received >= us->expected) {
      // Uploaded everything. Send response back
      MG_INFO(("Uploaded %lu bytes", us->received));
      mg_http_reply(c, 200, NULL, "%lu ok\n", us->received);
      if (us->fp) fs->cl(us->fp);  // Close file
      memset(us, 0, sizeof(*us));  // Cleanup upload state
      c->is_draining = 1;          // Close connection when response gets sent
    }
  }
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT && c->fn_data != NULL) {
    struct mg_tls_opts opts = {.cert = mg_str(s_tls_cert),
                               .key = mg_str(s_tls_key)};
    mg_tls_init(c, &opts);
  }

  handle_uploads(c, ev, ev_data);

  // Non-upload requests, we serve normally
  // NOTE: handle_uploads() may delete request and reset c->pfn
  if (ev == MG_EV_HTTP_MSG && c->pfn != NULL) {
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
