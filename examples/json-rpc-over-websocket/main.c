// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// See https://mongoose.ws/tutorials/json-rpc-over-websocket/

#include "mongoose.h"

static const char *s_listen_on = "ws://localhost:8000";
static const char *s_web_root = "web_root";

// RPC functions. Take string params, return (allocated) string result

static char *sum(struct mg_str params) {
  double a = 0.0, b = 0.0;
  mg_json_get_num(params, "$[0]", &a);
  mg_json_get_num(params, "$[1]", &b);
  return mg_mprintf("%g", a + b);
}

static char *multiply(struct mg_str params) {
  double a = 0.0, b = 0.0;
  mg_json_get_num(params, "$[0]", &a);
  mg_json_get_num(params, "$[1]", &b);
  return mg_mprintf("%g", a * b);
}

static void process_json_message(struct mg_connection *c, struct mg_str frame) {
  struct mg_str params = mg_str(""), id = mg_str("");
  int params_off = 0, params_len = 0, id_off = 0, id_len = 0;
  char *response = NULL;

  // Parse websocket message, which should be a JSON-RPC frame like this:
  // { "id": 3, "method": "sum", "params": [1,2] }
  char *method = mg_json_get_str(frame, "$.method");
  id_off = mg_json_get(frame.ptr, (int) frame.len, "$.id", &id_len);
  params_off = mg_json_get(frame.ptr, (int) frame.len, "$.params", &params_len);
  params = mg_str_n(frame.ptr + params_off, params_len);
  id = mg_str_n(frame.ptr + id_off, id_len);

  if (method == NULL || id_off < 0 || params_off < 0) {
    // Invalid frame. Return error and include this frame as error message
    response = mg_mprintf("{%Q:{%Q:%d,%Q:%.*Q}", "error", "code", -32700,
                          "message", (int) frame.len, frame.ptr);
  } else if (strcmp(method, "sum") == 0) {
    char *result = sum(params);
    response = mg_mprintf("{%Q:%.*s, %Q:%s}", "id", (int) id.len, id.ptr,
                          "result", result);
    free(result);
  } else if (strcmp(method, "mul") == 0) {
    char *result = multiply(params);
    response = mg_mprintf("{%Q:%.*s, %Q:%s}", "id", (int) id.len, id.ptr,
                          "result", result);
    free(result);
  } else {
    response =
        mg_mprintf("{%Q:%.*s, %Q:{%Q:%d,%Q:%Q}", "id", (int) id.len, id.ptr,
                   "error", "code", -32601, "message", "Method not found");
  }

  // Send the response back to the client
  if (response) {
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", response);
    MG_INFO(("[%.*s] -> [%s]", (int) frame.len, frame.ptr, response));
  }

  free(method);
  free(response);
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
    process_json_message(c, wm->data);
  }
  (void) fn_data;
}

static void timer_fn(void *arg) {
  struct mg_mgr *mgr = (struct mg_mgr *) arg;
  char *msg = mg_mprintf("{%Q:%Q,%Q:[%d,%d,%d]}", "method", "notification1",
                         "params", 1, 2, 3);
  // Broadcast message to all connected websocket clients.
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    // Send JSON-RPC notifications to marked connections
    if (c->label[0] == 'W') mg_ws_send(c, msg, strlen(msg), WEBSOCKET_OP_TEXT);
  }
  free(msg);
}

int main(void) {
  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Init event manager
  mg_log_set("3");
  mg_timer_add(&mgr, 5000, MG_TIMER_REPEAT, timer_fn, &mgr);  // Init timer
  printf("Starting WS listener on %s/websocket\n", s_listen_on);
  mg_http_listen(&mgr, s_listen_on, fn, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);             // Infinite event loop
  mg_mgr_free(&mgr);                            // Deallocate event manager
  return 0;
}
