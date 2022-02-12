// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// See https://mongoose.ws/tutorials/json-rpc-over-websocket/

#include "mjson.h"
#include "mongoose.h"

static const char *s_listen_on = "ws://localhost:8000";
static const char *s_web_root = "web_root";

static void sum(struct jsonrpc_request *r) {
  double a = 0.0, b = 0.0;
  mjson_get_number(r->params, r->params_len, "$[0]", &a);
  mjson_get_number(r->params, r->params_len, "$[1]", &b);
  jsonrpc_return_success(r, "%g", a + b);
}

static void multiply(struct jsonrpc_request *r) {
  double a = 0.0, b = 0.0;
  mjson_get_number(r->params, r->params_len, "$[0]", &a);
  mjson_get_number(r->params, r->params_len, "$[1]", &b);
  jsonrpc_return_success(r, "%g", a * b);
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_WS_OPEN) {
    c->label[0] = 'W';  // Mark this connection as an established WS client
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
    struct mg_str req = wm->data;
    char *response = NULL;
    jsonrpc_process(req.ptr, req.len, mjson_print_dynamic_buf, &response, NULL);
    mg_ws_send(c, response, strlen(response), WEBSOCKET_OP_TEXT);
    MG_INFO(("[%.*s] -> [%s]", (int) req.len, req.ptr, response));
    free(response);
  }
  (void) fn_data;
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  // Broadcast "hi" message to all connected websocket clients.
  // Traverse over all connections
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    // Send JSON-RPC notifications to marked connections
    const char *msg = "{\"method\":\"hiya!!\",\"params\":[1,2,3]}";
    if (c->label[0] == 'W') mg_ws_send(c, msg, strlen(msg), WEBSOCKET_OP_TEXT);
  }
}

int main(void) {
  struct mg_mgr mgr;   // Event manager
  struct mg_timer t1;  // Timer

  mg_mgr_init(&mgr);  // Init event manager
  mg_timer_init(&t1, 5000, MG_TIMER_REPEAT, timer_fn, &mgr);  // Init timer

  jsonrpc_init(NULL, NULL);         // Init JSON-RPC instance
  jsonrpc_export("sum", sum);       // And export a couple
  jsonrpc_export("mul", multiply);  // of RPC functions

  printf("Starting WS listener on %s/websocket\n", s_listen_on);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_timer_free(&t1);                           // Free timer resources
  mg_mgr_free(&mgr);                            // Deallocate event manager
  return 0;
}
