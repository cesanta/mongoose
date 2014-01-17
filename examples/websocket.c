#include <string.h>
#include "mongoose.h"

extern const char *find_embedded_file(const char *, size_t *);

static int iterate_callback(struct mg_connection *c) {
  if (c->is_websocket) {
    char buf[20];
    int len = snprintf(buf, sizeof(buf), "%d", * (int *) c->connection_param);
    mg_websocket_write(c, 1, buf, len);
  }
  return 1;
}

static int index_html(struct mg_connection *conn) {
  size_t index_size;
  const char *index_html = find_embedded_file("websocket.html", &index_size);

  if (conn->is_websocket) {
    // This handler is called for each incoming websocket frame, one or more
    // times for connection lifetime.
    // Echo websocket data back to the client.
    mg_websocket_write(conn, 1, conn->content, conn->content_len);
    return conn->content_len == 4 && !memcmp(conn->content, "exit", 4);
  } else {
    mg_send_header(conn, "Content-Type", "text/html");
    mg_send_data(conn, index_html, index_size);
    return 1;
  }
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL);
  unsigned int current_timer = 0, last_timer = 0;

  mg_set_option(server, "listening_port", "8080");
  mg_add_uri_handler(server, "/", index_html);

  printf("Started on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    current_timer = mg_poll_server(server, 100);
    if (current_timer - last_timer > 1) {
      last_timer = current_timer;
      mg_iterate_over_connections(server, iterate_callback, &current_timer);
    }
  }

  mg_destroy_server(&server);
  return 0;
}
