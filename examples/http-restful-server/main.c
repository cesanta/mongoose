// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

static const char *s_listen_on = "http://localhost:8000";
static const char *s_web_directory = ".";

// This RESTful server implements the following endpoints:
//   /api/f1 - respond with JSON string {"result": 123}
//   /api/f2/:id - wildcard example, respond with JSON string {"result": "URI"}
//   any other URI serves static files from s_web_directory
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/f1")) {
      mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);  // Serve REST
    } else if (mg_http_match_uri(hm, "/api/f2/*")) {
      mg_http_reply(c, 200, "", "{\"result\": \"%.*s\"}\n", (int) hm->uri.len,
                    hm->uri.ptr);
    } else {
      mg_http_serve_dir(c, ev_data, s_web_directory);  // Serve static files
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                            // Event manager
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
