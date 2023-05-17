// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// REST basics example
// It implements the following endpoints:
//    /api/f1 - respond with a simple mock result
//    /api/sum - respond with the result of adding two numbers
//    any other URI serves static files from s_root_dir
// Results are JSON strings

#include "mongoose.h"

static const char *s_http_addr = "http://localhost:8000";  // HTTP port
static const char *s_root_dir = "web_root";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/f1")) {
      mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m:%d}\n",
                    MG_ESC("result"), 123);
    } else if (mg_http_match_uri(hm, "/api/sum")) {
      // Attempt to fetch a JSON array from the body, hm->body
      struct mg_str json = hm->body;
      double num1, num2;
      if (mg_json_get_num(json, "$[0]", &num1) &&
          mg_json_get_num(json, "$[1]", &num2)) {
        // Success! create a JSON response
        mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{%m:%g}\n",
                      MG_ESC("result"), num1 + num2);
      }
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_root_dir};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

int main(void) {
  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_DEBUG);                      // Set log level
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_http_addr, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);
  return 0;
}
