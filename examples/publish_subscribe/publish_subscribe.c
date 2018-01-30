// Copyright (c) 2014 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <https://www.cesanta.com/license>.
//
// $Date: 2014-09-28 05:04:41 UTC $

#include "mongoose.h"

static void *stdin_thread(void *param) {
  int ch, sock = *(int *) param;
  // Forward all types characters to the socketpair
  while ((ch = getchar()) != EOF) {
    unsigned char c = (unsigned char) ch;
    if (send(sock, (const char *) &c, 1, 0) < 0) {
      fprintf(stderr, "Failed to send byte to the socket");
    }
  }
  return NULL;
}

static void server_handler(struct mg_connection *nc, int ev, void *p) {
  (void) p;
  if (ev == MG_EV_RECV) {
    // Push received message to all ncections
    struct mbuf *io = &nc->recv_mbuf;
    struct mg_connection *c;

    for (c = mg_next(nc->mgr, NULL); c != NULL; c = mg_next(nc->mgr, c)) {
      if (!(c->flags & MG_F_USER_2)) continue;  // Skip non-client connections
      mg_send(c, io->buf, io->len);
    }
    mbuf_remove(io, io->len);
  } else if (ev == MG_EV_ACCEPT) {
    char addr[32];
    mg_sock_addr_to_str(p, addr, sizeof(addr),
                        MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
    printf("New client connected from %s\n", addr);
  }
}

static void client_handler(struct mg_connection *conn, int ev, void *p) {
  struct mbuf *io = &conn->recv_mbuf;
  (void) p;

  if (ev == MG_EV_CONNECT) {
    if (conn->flags & MG_F_CLOSE_IMMEDIATELY) {
      printf("%s\n", "Error connecting to server!");
      exit(EXIT_FAILURE);
    }
    printf("%s\n", "Connected to server. Type a message and press enter.");
  } else if (ev == MG_EV_RECV) {
    if (conn->flags & MG_F_USER_1) {
      // Received data from the stdin, forward it to the server
      struct mg_connection *c = (struct mg_connection *) conn->user_data;
      mg_send(c, io->buf, io->len);
      mbuf_remove(io, io->len);
    } else {
      // Received data from server connection, print it
      fwrite(io->buf, io->len, 1, stdout);
      mbuf_remove(io, io->len);
    }
  } else if (ev == MG_EV_CLOSE) {
    // Connection has closed, most probably cause server has stopped
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <port> <client|server>\n", argv[0]);
    exit(EXIT_FAILURE);
  } else if (strcmp(argv[2], "client") == 0) {
    sock_t fds[2];
    struct mg_connection *ioconn, *server_conn;

    mg_mgr_init(&mgr, NULL);

    // Connect to the pubsub server
    server_conn = mg_connect(&mgr, argv[1], client_handler);
    if (server_conn == NULL) {
      fprintf(stderr, "Cannot connect to port %s\n", argv[1]);
      exit(EXIT_FAILURE);
    }
    server_conn->flags |= MG_F_USER_2;  // Mark this as a client connection

    // Create a socketpair and give one end to the thread that reads stdin
    mg_socketpair(fds, SOCK_STREAM);
    mg_start_thread(stdin_thread, &fds[1]);

    // The other end of a pair goes inside the server
    ioconn = mg_add_sock(&mgr, fds[0], client_handler);
    ioconn->flags |= MG_F_USER_1;  // Mark this so we know this is a stdin
    ioconn->user_data = server_conn;

  } else {
    // Server code path
    mg_mgr_init(&mgr, NULL);
    mg_bind(&mgr, argv[1], server_handler);
    printf("Starting pubsub server on port %s\n", argv[1]);
  }

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return EXIT_SUCCESS;
}
