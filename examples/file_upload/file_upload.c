// Copyright (c) 2004-2012 Sergey Lyubka
// This file is a part of mongoose project, http://github.com/valenok/mongoose

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

static int send_index_page(struct mg_connection *conn) {
  const char *data;
  int data_len, n1, n2;
  char var_name[100], file_name[100];

  mg_printf_data(conn, "%s",
                 "<html><body>Upload example."
                 "<form method=\"POST\" action=\"/handle_post_request\" "
                 "  enctype=\"multipart/form-data\">"
                 "<input type=\"file\" name=\"file1\" /> <br/>"
                 "<input type=\"file\" name=\"file2\" /> <br/>"
                 "<input type=\"submit\" value=\"Upload\" />"
                 "</form>");

  n1 = n2 = 0;
  while ((n2 = mg_parse_multipart(conn->content + n1, conn->content_len - n1,
                                  var_name, sizeof(var_name), file_name,
                                  sizeof(file_name), &data, &data_len)) > 0) {
    mg_printf_data(conn, "var: %s, file_name: %s, size: %d bytes<br>",
                   var_name, file_name, data_len);
    n1 += n2;
  }

  mg_printf_data(conn, "%s", "</body></html>");

  return MG_TRUE;
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH:     return MG_TRUE;
    case MG_REQUEST:  return send_index_page(conn);
    default:          return MG_FALSE;
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
