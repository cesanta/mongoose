// Copyright (c) 2014 Cesanta Software
// All rights reserved
//
// This example demostrates how to connect to the remote Web server,
// download data, process it and send back a reply.

#include <signal.h>
#include <stdlib.h>

#include "mongoose.h"

static int s_received_signal = 0;
static struct mg_server *s_server = NULL;
static const char *s_remote_addr = "glosbe.com:80";

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_received_signal = sig_num;
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  struct mg_connection *client, *orig;

  switch (ev) {
    case MG_AUTH:
      return MG_TRUE;

    case MG_CONNECT:
      // Send request to the remote host.
      // TODO(lsm): handle connect error here.
      mg_printf(conn, "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n",
                "/gapi/translate?from=eng&dest=fra&format=json&phrase=cat",
                s_remote_addr);
      return MG_TRUE;

    case MG_REPLY:
      // Send reply to the original connection
      orig = (struct mg_connection *) conn->connection_param;
      mg_send_status(orig, conn->status_code);
      mg_send_header(orig, "Content-Type", "text/plain");
      mg_send_data(orig, conn->content, conn->content_len);
      mg_send_data(orig, "", 0);  // Last chunk: mark the end of reply

      // Disconnect connections
      orig->connection_param = NULL;
      conn->connection_param = NULL;
      return MG_TRUE;

    case MG_REQUEST:
      if ((client = mg_connect(s_server, s_remote_addr)) != NULL) {
        // Interconnect requests
        client->connection_param = conn;
        conn->connection_param = client;
        return MG_MORE;
      } else {
        mg_printf_data(conn, "%s", "cannot send API request");
        return MG_TRUE;
      }

    default:
      return MG_FALSE;
  }
}

int main(void) {
  s_server = mg_create_server(NULL, ev_handler);

  mg_set_option(s_server, "listening_port", "8080");

  // Setup signal handlers
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);

  printf("Listening on port %s\n", mg_get_option(s_server, "listening_port"));
  while (s_received_signal == 0) {
    mg_poll_server(s_server, 1000);
  }
  mg_destroy_server(&s_server);
  printf("Existing on signal %d\n", s_received_signal);

  return EXIT_SUCCESS;
}
