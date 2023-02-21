// Copyright (c) 2021 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

// HTTP request handler function. It implements the following endpoints:
//   /upload - prints all submitted form elements
//   all other URI - serves web_root/ directory
//
// /////////////////           IMPORTANT        //////////////////////////
//
// Mongoose has a limit on input buffer, which also limits maximum upload size.
// It is controlled by the MG_MAX_RECV_SIZE constant, which is set by
// default to (3 * 1024 * 1024), i.e. 3 megabytes.
// Use -DMG_MAX_BUF_SIZE=NEW_LIMIT to override it.
//
// Also, consider changing -DMG_IO_SIZE=SOME_BIG_VALUE to increase IO buffer
// increment when reading data.
static void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("New request to: [%.*s], body size: %lu", (int) hm->uri.len,
             hm->uri.ptr, (unsigned long) hm->body.len));
    if (mg_http_match_uri(hm, "/upload")) {
      struct mg_http_part part;
      size_t ofs = 0;
      while ((ofs = mg_http_next_multipart(hm->body, ofs, &part)) > 0) {
        MG_INFO(("Chunk name: [%.*s] filename: [%.*s] length: %lu bytes",
                 (int) part.name.len, part.name.ptr, (int) part.filename.len,
                 part.filename.ptr, (unsigned long) part.body.len));
      }
      mg_http_reply(c, 200, "", "Thank you!");
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "web_root"};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;

  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_http_listen(&mgr, "http://localhost:8000", cb, NULL);

  for (;;) mg_mgr_poll(&mgr, 50);
  mg_mgr_free(&mgr);

  return 0;
}
