// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static void *callback(enum mg_event event, struct mg_connection *conn) {
  if (event == MG_WEBSOCKET_READY) {
    static const char *hello = "hello from mongoose! waiting for message ...";
    char frame[2];

    // Prepare websocket frame.
    frame[0] = 0x81;            // text frame
    frame[1] = strlen(hello);   // length is < 126

    // Write frame and a text message
    mg_write(conn, frame, sizeof(frame));
    mg_write(conn, hello, strlen(hello));
    return "";
  } else if (event == MG_WEBSOCKET_MESSAGE) {
    unsigned char buf[500], reply[500];
    int len, msg_len, i, mask_len, xor;

    // Read message from the client and echo it back
    if ((len = mg_read(conn, buf, sizeof(buf))) > 8) {
      msg_len = buf[1] & 127;
      mask_len = (buf[1] & 128) ? 4 : 0;
      if (msg_len < 126) {
        reply[0] = 0x81;  // text, FIN set
        reply[1] = msg_len;
        for (i = 0; i < msg_len; i++) {
          xor = mask_len == 0 ? 0 : buf[2 + (i % 4)];
          reply[i + 2] = buf[i + 2 + mask_len] ^ xor;
        }
        mg_write(conn, reply, 2 + msg_len);
      }
    }

    return ""; // Return non-NULL: stop websocket conversation
  } else {
    return NULL;
  }
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {
    "listening_ports", "8080",
    "document_root", "websocket_html_root",
    NULL
  };

  ctx = mg_start(&callback, NULL, options);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
