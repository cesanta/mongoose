// Copyright (c) 2014 Cesanta Software
// All rights reserved
//
// This example demostrates how to use array get variables using mg_get_n_var
// $Date: 2014-09-09 22:20:23 UTC $

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
    {
      mg_printf_data(conn, "Hello! Requested URI is [%s] ", conn->uri);
      char buffer[1024];
      int i, ret;
      for(i=0; (ret = mg_get_var_n(conn, "foo[]", buffer, 1024, i)) > 0; i++)
        mg_printf_data(conn, "\nfoo[%d] = %s", i, buffer);

      return MG_TRUE;
    }
    default: return MG_FALSE;
  }
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
