// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

static int event_handler(struct mg_event *event) {

  if (event->type == MG_REQUEST_BEGIN) {
    const char *version_header = mg_get_header(event->conn,
                                               "Sec-WebSocket-Version");

    if (version_header != NULL) {
      // Websocket request, process it
      if (strcmp(version_header, "13") != 0) {
        mg_printf(event->conn, "%s", "HTTP/1.1 426 Upgrade Required\r\n\r\n");
      } else {
        static const char *server_ready_message = "server ready";
        char *data;
        int bits, len, must_exit = 0;

        // Handshake, and send initial server message
        mg_websocket_handshake(event->conn);
        mg_websocket_write(event->conn, WEBSOCKET_OPCODE_TEXT,
                           server_ready_message, strlen(server_ready_message));

        // Read messages sent by client. Echo them back.
        while (must_exit == 0 &&
               (len = mg_websocket_read(event->conn, &bits, &data)) > 0) {

          printf("got message: [%.*s]\n", len, data);
          mg_websocket_write(event->conn, WEBSOCKET_OPCODE_TEXT, data, len);

          // If the message is "exit", close the connection, exit the loop
          if (memcmp(data, "exit", 4) == 0) {
            mg_websocket_write(event->conn,
                               WEBSOCKET_OPCODE_CONNECTION_CLOSE, "", 0);
            must_exit = 1;
          }

          // It's our responsibility to free allocated message
          free(data);
        }
      }
      return 1;
    }
  }

  return 0;
}

int main(void) {
  struct mg_context *ctx;
  const char *options[] = {
    "listening_ports", "8080",
    "document_root", "websocket_html_root",
    NULL
  };

  ctx = mg_start(options, &event_handler, NULL);
  getchar();  // Wait until user hits "enter"
  mg_stop(ctx);

  return 0;
}
