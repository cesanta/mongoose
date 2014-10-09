// Copyright (c) 2014 Cesanta Software
// All rights reserved
//
// This example demostrates how to send arbitrary files to the client.

#include "mongoose.h"

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_REQUEST:
      mg_send_file(conn, "send_file.c", NULL);  // Also could be a dir, or CGI
      return MG_MORE; // It is important to return MG_MORE after mg_send_file!
    case MG_AUTH: return MG_TRUE;
    default: return MG_FALSE;
  }
}

int main(void) {
  struct mg_server *server = mg_create_server(NULL, ev_handler);
  mg_set_option(server, "listening_port", "8080");

  printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
  for (;;) mg_poll_server(server, 1000);
  mg_destroy_server(&server);

  return 0;
}
