// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static void websocket_ready_handler(struct mg_connection *conn) {
  static const char *message = "server ready";
  mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, message, strlen(message));
}

// Arguments:
//   flags: first byte of websocket frame, see websocket RFC,
//          http://tools.ietf.org/html/rfc6455, section 5.2
//   data, data_len: payload data. Mask, if any, is already applied.
static int websocket_data_handler(struct mg_connection *conn, int flags,
                                  char *data, size_t data_len) {
  (void) flags; // Unused
  mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, data, data_len);

  // Returning zero means stoping websocket conversation.
  // Close the conversation if client has sent us "exit" string.
  return memcmp(data, "exit", 4);
}

int main(void) {
  struct mg_context *ctx;
  struct mg_callbacks callbacks;
  const char *options[] = {
    "listening_ports", "8080",
    "document_root", "websocket_html_root",
    NULL
  };

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.websocket_ready = websocket_ready_handler;
  callbacks.websocket_data = websocket_data_handler;
  ctx = mg_start(&callbacks, NULL, options);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
