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

static int websocket_data_handler(struct mg_connection *conn) {
  unsigned char buf[200], reply[200];
  int n, i, mask_len, xor, msg_len, len;

  // Read message from the client.
  // Accept only small (<126 bytes) messages.
  len = 0;
  msg_len = mask_len = 0;
  for (;;) {
    if ((n = mg_read(conn, buf + len, sizeof(buf) - len)) <= 0) {
      return 0;  // Read error, close websocket
    }
    len += n;
    if (len >= 2) {
      msg_len = buf[1] & 127;
      mask_len = (buf[1] & 128) ? 4 : 0;
      if (msg_len > 125) {
        return 0; // Message is too long, close websocket
      }
      // If we've buffered the whole message, exit the loop
      if (len >= 2 + mask_len + msg_len) {
        break;
      }
    }
  }

  // Prepare frame
  reply[0] = 0x81;  // text, FIN set
  reply[1] = msg_len;

  // Copy message from request to reply, applying the mask if required.
  for (i = 0; i < msg_len; i++) {
    xor = mask_len == 0 ? 0 : buf[2 + (i % 4)];
    reply[i + 2] = buf[i + 2 + mask_len] ^ xor;
  }

  // Echo the message back to the client
  mg_write(conn, reply, 2 + msg_len);

  // Returnint zero means stoping websocket conversation.
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
