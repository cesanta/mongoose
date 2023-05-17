// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// See https://mongoose.ws/tutorials/json-rpc-over-websocket/

#include "mongoose.h"

static const char *s_listen_on = "ws://localhost:8000";
static const char *s_web_root = "web_root";
static struct mg_rpc *s_rpc_head = NULL;

static void rpc_sum(struct mg_rpc_req *r) {
  double a = 0.0, b = 0.0;
  mg_json_get_num(r->frame, "$.params[0]", &a);
  mg_json_get_num(r->frame, "$.params[1]", &b);
  mg_rpc_ok(r, "%g", a + b);
}

static void rpc_mul(struct mg_rpc_req *r) {
  double a = 0.0, b = 0.0;
  mg_json_get_num(r->frame, "$.params[0]", &a);
  mg_json_get_num(r->frame, "$.params[1]", &b);
  mg_rpc_ok(r, "%g", a * b);
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_WS_OPEN) {
    c->data[0] = 'W';  // Mark this connection as an established WS client
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/websocket")) {
      // Upgrade to websocket. From now on, a connection is a full-duplex
      // Websocket connection, which will receive MG_EV_WS_MSG events.
      mg_ws_upgrade(c, hm, NULL);
    } else {
      // Serve static files
      struct mg_http_serve_opts opts = {.root_dir = s_web_root};
      mg_http_serve_dir(c, ev_data, &opts);
    }
  } else if (ev == MG_EV_WS_MSG) {
    // Got websocket frame. Received data is wm->data
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    struct mg_iobuf io = {0, 0, 0, 512};
    struct mg_rpc_req r = {&s_rpc_head, 0, mg_pfn_iobuf, &io, 0, wm->data};
    mg_rpc_process(&r);
    if (io.buf) mg_ws_send(c, (char *) io.buf, io.len, WEBSOCKET_OP_TEXT);
    mg_iobuf_free(&io);
  }
  (void) fn_data;
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  // Broadcast message to all connected websocket clients.
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    if (c->data[0] != 'W') continue;
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "{%m:%m,%m:[%d,%d,%d]}",
                 MG_ESC("method"), MG_ESC("notification1"), MG_ESC("params"), 1,
                 2, 3);
  }
}

int main(void) {
  struct mg_mgr mgr;        // Event manager
  mg_mgr_init(&mgr);        // Init event manager
  mg_log_set(MG_LL_DEBUG);  // Set log level
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT, timer_fn, &mgr);  // Init timer

  // Configure JSON-RPC functions we're going to handle
  mg_rpc_add(&s_rpc_head, mg_str("sum"), rpc_sum, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("mul"), rpc_mul, NULL);
  mg_rpc_add(&s_rpc_head, mg_str("rpc.list"), mg_rpc_list, &s_rpc_head);

  printf("Starting WS listener on %s/websocket\n", s_listen_on);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);                            // Deallocate event manager
  mg_rpc_del(&s_rpc_head, NULL);                // Deallocate RPC handlers
  return 0;
}
