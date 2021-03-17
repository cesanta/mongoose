// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /upload - prints all submitted form elements
//   all other URI - serves web_root/ directory
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    LOG(LL_INFO, ("New request to: [%.*s]", (int) hm->uri.len, hm->uri.ptr));
    if (mg_http_match_uri(hm, "/upload")) {
      struct mg_http_part part;
      size_t ofs = 0;
      while ((ofs = mg_http_next_multipart(hm->body, ofs, &part)) > 0) {
        LOG(LL_INFO,
            ("Name: [%.*s] Filename: [%.*s] Body: [%.*s]", (int) part.name.len,
             part.name.ptr, (int) part.filename.len, part.filename.ptr,
             (int) part.body.len, part.body.ptr));
      }
      mg_http_reply(c, 200, "", "Thank you!");
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
}

int main(void) {
  struct mg_mgr mgr;
  struct mg_timer t1;

  mg_mgr_init(&mgr);
  mg_log_set("3");
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
