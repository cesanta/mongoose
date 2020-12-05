#pragma once

#define WEBSOCKET_OP_CONTINUE 0
#define WEBSOCKET_OP_TEXT 1
#define WEBSOCKET_OP_BINARY 2
#define WEBSOCKET_OP_CLOSE 8
#define WEBSOCKET_OP_PING 9
#define WEBSOCKET_OP_PONG 10

#define WEBSOCKET_FLAGS_MASK_FIN 128
#define WEBSOCKET_FLAGS_MASK_OP 15

#include "http.h"

struct mg_ws_message {
  struct mg_str data;
  uint8_t flags;  // Websocket message flags
};

struct mg_connection *mg_ws_connect(struct mg_mgr *, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...);
void mg_ws_upgrade(struct mg_connection *, struct mg_http_message *);
size_t mg_ws_send(struct mg_connection *, const char *buf, size_t len, int op);
