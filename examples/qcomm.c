#include <unistd.h>
#include <string.h>
#include "core.h"

static void iterate_callback(struct mg_connection *c, void *param) {
  if (c->is_websocket) {
    char buf[20];
    int len = snprintf(buf, sizeof(buf), "%d", * (int *) param);
    mg_websocket_write(c, 1, buf, len);
  }
}

// This thread sends heartbeats to all websocket connections with 1s interval.
// The heartbeat message is simply an iteration counter.
static void *timer_thread(void *param) {
  struct mg_server *server = (struct mg_server *) param;
  int i;

  for (i = 0; i < 9999999; i++) {
    sleep(1);
    mg_iterate_over_connections(server, iterate_callback, &i);
  }

  return NULL;
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

  mg_set_option(server, "listening_port", "8080");
  mg_set_option(server, "document_root", argc > 1 ? argv[1] : ".");
  mg_add_uri_handler(server, "/ws", handler);
  mg_start_thread(timer_thread, server);

  printf("Started on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 3000);
  }

  mg_destroy_server(&server);
  return 0;
}
