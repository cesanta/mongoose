// Copyright (c) 2013-2014 Cesanta Software Limited
// $Date: 2014-09-09 17:07:55 UTC $

#include <string.h>
#include <time.h>
#include "mongoose.h"

static void push_message(struct mg_server *server, time_t current_time) {
  struct mg_connection *c;
  char buf[20];
  int len = sprintf(buf, "%lu", (unsigned long) current_time);

  // Iterate over all connections, and push current time message to websocket ones.
  for (c = mg_next(server, NULL); c != NULL; c = mg_next(server, c)) {
    if (c->is_websocket) {
      mg_websocket_write(c, 1, buf, len);
    }
  }
}

static int send_reply(struct mg_connection *conn) {
  if (conn->is_websocket) {
    // This handler is called for each incoming websocket frame, one or more
    // times for connection lifetime.
    // Echo websocket data back to the client.
    mg_websocket_write(conn, 1, conn->content, conn->content_len);
    return conn->content_len == 4 && !memcmp(conn->content, "exit", 4) ?
      MG_FALSE : MG_TRUE;
  } else {
    mg_send_file(conn, "index.html", NULL);
    return MG_MORE;
  }
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST: return send_reply(conn);
    default: return MG_FALSE;
  }
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL, ev_handler);
  time_t current_timer = 0, last_timer = time(NULL);

  mg_set_option(server, "listening_port", "8080");

  printf("Started on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 100);
    current_timer = time(NULL);
    if (current_timer - last_timer > 0) {
      last_timer = current_timer;
      push_message(server, current_timer);
    }
  }

  mg_destroy_server(&server);
  return 0;
}
