// Copyright (c) 2020-2024 Cesanta Software Limited
// All rights reserved
//
// Streaming upload example. Demonstrates how to use MG_EV_READ events
// to save a large file without buffering it fully in memory.
//
// curl http://localhost:8000/upload?name=a.txt --data-binary @large_file.txt

#include "mongoose.h"

static void handle_uploads(struct mg_connection *c, int ev, void *ev_data) {
  size_t *data = (size_t *) c->data;

  // Catch /upload requests early, without buffering whole body:
  if (ev == MG_EV_HTTP_HDRS) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_match(hm->uri, mg_str("/upload"), NULL)) {
      // When we receive MG_EV_HTTP_HDRS event, that means we've received all
      // HTTP headers but not necessarily full HTTP body. We save HTTP body
      // length in data[0]:
      // data[0] contains expected number of bytes
      // data[1] contains received number of bytes
      data[0] = hm->body.len;  // Store number of bytes we expect
      mg_iobuf_del(&c->recv, 0, hm->head.len);  // Delete HTTP headers
      c->pfn = NULL;  // Silence HTTP protocol handler, we'll use MG_EV_READ
    }
  }

  // Catch uploaded file data for both MG_EV_READ and MG_EV_HTTP_HDRS
  if (data[0] > 0 && c->recv.len > 0) {
    data[1] += c->recv.len;
    // MG_DEBUG(("Got chunk len %lu, %lu total", c->recv.len, data[1]));
    c->recv.len = 0;  // Delete received data
    if (data[1] >= data[0]) {
      // Uploaded everything. Send response back
      MG_INFO(("Uploaded %lu bytes", data[1]));
      mg_http_reply(c, 200, NULL, "%lu ok\n", data[1]);
      c->is_draining = 1;  // Close us when response gets sent
    }
  }
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
  handle_uploads(c, ev, ev_data);

  // Non-upload requests, we serve normally
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_serve_opts opts = {.root_dir = "web_root"};
    mg_http_serve_dir(c, ev_data, &opts);
  }
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", fn, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
