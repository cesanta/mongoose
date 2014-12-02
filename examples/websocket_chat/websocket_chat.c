// Copyright (c) 2013-2014 Cesanta Software Limited
// $Date: 2014-09-09 17:07:55 UTC $

#include <string.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include "mongoose.h"

static int s_signal_received = 0;
static struct mg_server *s_server = NULL;

// Data associated with each websocket connection
struct conn_data {
  int room;
};

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);  // Reinstantiate signal handler
  s_signal_received = sig_num;
}

static void handle_websocket_message(struct mg_connection *conn) {
  struct conn_data *d = (struct conn_data *) conn->connection_param;
  struct mg_connection *c;

  printf("[%.*s]\n", (int) conn->content_len, conn->content);
  if (conn->content_len > 5 && !memcmp(conn->content, "join ", 5)) {
    // Client joined new room
    d->room = conn->content[5];
  } else if (conn->content_len > 4 && !memcmp(conn->content, "msg ", 4) &&
             d->room != 0 && d->room != '?') {
    // Client has sent a message. Push this message to all clients
    // that are subscribed to the same room as client
    for (c = mg_next(s_server, NULL); c != NULL; c = mg_next(s_server, c)) {
      struct conn_data *d2 = (struct conn_data *) c->connection_param;
      if (!c->is_websocket || d2->room != d->room) continue;
      mg_websocket_printf(c, WEBSOCKET_OPCODE_TEXT, "msg %c %p %.*s",
                          (char) d->room, conn,
                          conn->content_len - 4, conn->content + 4);
    }
  }
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_REQUEST:
      if (conn->is_websocket) {
        handle_websocket_message(conn);
        return MG_TRUE;
      } else {
        mg_send_file(conn, "index.html", NULL);  // Return MG_MORE after!
        return MG_MORE;
      }
    case MG_WS_CONNECT:
      // New websocket connection. Send connection ID back to the client.
      conn->connection_param = calloc(1, sizeof(struct conn_data));
      mg_websocket_printf(conn, WEBSOCKET_OPCODE_TEXT, "id %p", conn);
      return MG_FALSE;
    case MG_CLOSE:
      free(conn->connection_param);
      return MG_TRUE;
    case MG_AUTH:
      return MG_TRUE;
    default:
      return MG_FALSE;
  }
}

int main(void) {
  s_server = mg_create_server(NULL, ev_handler);
  mg_set_option(s_server, "listening_port", "8080");

  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  printf("Started on port %s\n", mg_get_option(s_server, "listening_port"));
  while (s_signal_received == 0) {
    mg_poll_server(s_server, 100);
  }
  mg_destroy_server(&s_server);
  return 0;
}
