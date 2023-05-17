// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// Example Websocket server with timers. This is a simple Websocket echo
// server, which sends a message to all connected clients periodically,
// using timer API.

#include "mongoose.h"

static const char *s_listen_on = "http://localhost:8000";
static const char *s_web_root = "web_root";

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/api/watch")) {
      mg_ws_upgrade(c, hm, NULL);  // Upgrade HTTP to Websocket
      c->data[0] = 'W';            // Set some unique mark on the connection
    } else {
      struct mg_http_serve_opts opts = {.root_dir = s_web_root};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  }
  (void) fn_data;
}

// Push to all watchers
static void push(struct mg_mgr *mgr, const char *name, const void *data) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->data[0] != 'W') continue;
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m,%m:%m}", MG_ESC("name"),
                 MG_ESC(name), MG_ESC("data"), MG_ESC(data));
  }
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  char buf[50];
  mg_snprintf(buf, sizeof(buf), "[%lu, %d]", (unsigned long) time(NULL),
              10 + (int) ((double) rand() * 10 / RAND_MAX));
  push(mgr, "metrics", buf);
}

int main(void) {
  struct mg_mgr mgr;        // Event manager
  mg_mgr_init(&mgr);        // Initialise event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, 2000, MG_TIMER_REPEAT, timer_fn, &mgr);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 500);              // Infinite event loop
  mg_mgr_free(&mgr);                            // Free manager resources
  return 0;
}
