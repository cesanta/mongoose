#include <stdio.h>
#include <string.h>
#include "mongoose.h"


static int index_html(struct mg_connection *conn) {
  FILE *fp = (FILE *) conn->connection_param;
  char buf[200];
  size_t n = 0;

  if (fp == NULL) {
    fp = fopen("../mongoose.c", "r");
    conn->connection_param = (void *) fp;
  }

  if (fp != NULL) {
    n = fread(buf, 1, sizeof(buf), fp);
    mg_send_data(conn, buf, n);

    if (n < sizeof(buf) || conn->wsbits != 0) {
      fclose(fp);
      conn->connection_param = NULL;
    }
  }

  return n < sizeof(buf) ? MG_REQUEST_PROCESSED : MG_REQUEST_CALL_AGAIN;
}

int main(void) {
  struct mg_server *server;

  // Create and configure the server
  server = mg_create_server(NULL);
  mg_set_option(server, "listening_port", "8080");
  mg_set_request_handler(server, index_html);

  // Serve request. Hit Ctrl-C to terminate the program
  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) {
    mg_poll_server(server, 1000);
  }

  // Cleanup, and free server instance
  mg_destroy_server(&server);

  return 0;
}
