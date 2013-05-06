// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static void websocket_ready_handler(struct mg_connection *conn) {
  unsigned char buf[40];
  buf[0] = 0x81;
  buf[1] = snprintf((char *) buf + 2, sizeof(buf) - 2, "%s", "server ready");
  mg_write(conn, buf, 2 + buf[1]);
}

// Arguments:
//   flags: first byte of websocket frame, see websocket RFC,
//          http://tools.ietf.org/html/rfc6455, section 5.2
//   data, data_len: payload data. Mask, if any, is already applied.
static int websocket_data_handler(struct mg_connection *conn, int flags,
                                  char *data, size_t data_len) {
  unsigned char reply[200];
  size_t i;

  (void) flags;

  printf("rcv: [%.*s]\n", (int) data_len, data);

  // Truncate echoed message, to simplify output code.
  if (data_len > 125) {
    data_len = 125;
  }

  // Prepare frame
  reply[0] = 0x81;  // text, FIN set
  reply[1] = data_len;

  // Copy message from request to reply, applying the mask if required.
  for (i = 0; i < data_len; i++) {
    reply[i + 2] = data[i];
  }

  // Echo the message back to the client
  mg_write(conn, reply, 2 + data_len);

  // Returning zero means stoping websocket conversation.
  // Close the conversation if client has sent us "exit" string.
  return memcmp(reply + 2, "exit", 4);
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
