#include <string.h>
#include <time.h>
#include "mongoose.h"

#ifdef _WIN32
#define snprintf _snprintf
#endif

extern const char *find_embedded_file(const char *, size_t *);

static int iterate_callback(struct mg_connection *c, enum mg_event ev) {
  if (ev == MG_POLL && c->is_websocket) {
    char buf[20];
    int len = snprintf(buf, sizeof(buf), "%lu",
     (unsigned long) * (time_t *) c->callback_param);
    mg_websocket_write(c, 1, buf, len);
  }
  return MG_TRUE;
}

static int send_reply(struct mg_connection *conn) {
  size_t index_size;
  const char *index_html = find_embedded_file("websocket.html", &index_size);

  if (conn->is_websocket) {
    // This handler is called for each incoming websocket frame, one or more
    // times for connection lifetime.
    // Echo websocket data back to the client.
    mg_websocket_write(conn, 1, conn->content, conn->content_len);
    return conn->content_len == 4 && !memcmp(conn->content, "exit", 4) ?
      MG_FALSE : MG_TRUE;
  } else {
    mg_send_header(conn, "Content-Type", "text/html");
    mg_send_data(conn, index_html, index_size);
    return MG_TRUE;
  }
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  if (ev == MG_REQUEST) {
    return send_reply(conn);
  } else if (ev == MG_AUTH) {
    return MG_TRUE;
  } else {
    return MG_FALSE;
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
      mg_iterate_over_connections(server, iterate_callback, &current_timer);
    }
  }

  mg_destroy_server(&server);
  return 0;
}
