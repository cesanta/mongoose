#include <unistd.h>
#include <string.h>

#include "mongoose.h"

static void iterate_callback(struct mg_connection *c, void *param) {
  if (c->is_websocket) {
    char buf[20];
    int len = snprintf(buf, sizeof(buf), "%d", * (int *) param);
    mg_websocket_write(c, 1, buf, len);
  }
}

// This handler is called for each incoming websocket frame, one or more
// times for connection lifetime.
static int handler(struct mg_connection *conn) {
  static const char oops[] = "HTTP/1.0 200 OK\r\n\r\nwebsocket data expected\n";

  if (!conn->is_websocket) {
    mg_write(conn, oops, sizeof(oops) - 1);
    return 1;
  }

  mg_websocket_write(conn, 1, conn->content, conn->content_len);

  return conn->content_len == 4 && !memcmp(conn->content, "exit", 4);
}

int main(int argc, char *argv[]) {
  struct mg_server *server = mg_create_server(NULL);
  unsigned int current_timer = 0, last_timer = 0;

  mg_set_option(server, "listening_port", "8080");
  mg_set_option(server, "document_root", argc > 1 ? argv[1] : ".");
  mg_add_uri_handler(server, "/ws", handler);

  printf("Started on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    current_timer = mg_poll_server(server, 1);
    if (current_timer - last_timer > 4) {
      last_timer = current_timer;
      mg_iterate_over_connections(server, iterate_callback, &current_timer);
    }
  }

  mg_destroy_server(&server);
  return 0;
}
