// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Streaming upload example. Demonstrates how to use MG_EV_READ events
// to get large payload in smaller chunks. To test, use curl utility:
//
// curl http://localhost:8000/upload?name=a.txt --data-binary @large_file.txt

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /upload - Saves the next file chunk
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    // Parse the incoming data ourselves. If we can parse the request,
    // store two size_t variables in the c->data: expected len and recv len.
    size_t *data = (size_t *) c->data;
    if (data[0]) {  // Already parsed, simply print received data
      data[1] += c->recv.len;
      MG_INFO(("Got chunk len %lu, %lu total", c->recv.len, data[1]));
      c->recv.len = 0;  // And cleanup the receive buffer. Streaming!
      if (data[1] >= data[0]) mg_http_reply(c, 200, "", "ok\n");
    } else {
      struct mg_http_message hm;
      int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      if (n < 0) mg_error(c, "Bad response");
      if (n > 0) {
        if (mg_http_match_uri(&hm, "/upload")) {
          MG_INFO(("Got chunk len %lu", c->recv.len - n));
          data[0] = hm.body.len;
          data[1] = c->recv.len - n;
          if (data[1] >= data[0]) mg_http_reply(c, 200, "", "ok\n");
        } else {
          struct mg_http_serve_opts opts = {.root_dir = "web_root"};
          mg_http_serve_dir(c, &hm, &opts);
        }
      }
    }
  }
  (void) fn_data, (void) ev_data;
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set debug log level
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
