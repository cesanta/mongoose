#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  int result = MG_FALSE;

  if (ev == MG_REQUEST) {
    mg_printf_data(conn, "Hello! Requested URI is [%s]", conn->uri);
    result = MG_TRUE;
  } else if (ev == MG_AUTH) {
    result = MG_TRUE;
  }

  return result;
}

int main(void) {
  struct mg_server *server;

  // Create and configure the server
  server = mg_create_server(NULL, ev_handler);
  mg_set_option(server, "listening_port", "8080");

  // Serve request. Hit Ctrl-C to terminate the program
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }

  // Cleanup, and free server instance
  mg_destroy_server(&server);

  return 0;
}
