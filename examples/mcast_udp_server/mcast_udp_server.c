// Copyright (c) 2016 Cesanta Software Limited
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
// This demo shows how to configure a mongoose UDP connection to receive
// multicast traffic.

#include <netinet/in.h>

#include "mongoose.h"

#define MCAST_GROUP "224.0.22.1"
#define DEFAULT_PORT "1234"

static void ev_handler(struct mg_connection *nc, int ev, void *p) {
  struct mbuf *io = &nc->recv_mbuf;
  (void) p;
  switch (ev) {
    case MG_EV_POLL: {
      const char *peer;
      peer = inet_ntoa(nc->sa.sin.sin_addr);
      printf("nc->sa: %s %d\n", peer, ntohs(nc->sa.sin.sin_port));

      break;
    }
    case MG_EV_RECV:
      printf("Received (%zu bytes): '%.*s'\n", io->len, (int) io->len, io->buf);
      mbuf_remove(io, io->len);
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    default:
      break;
  }
}

void usage(char **argv) {
  fprintf(stderr, "%s: -i address_of_mcast_interface [ -g %s ] [ -p %s ]\n",
          argv[0], MCAST_GROUP, DEFAULT_PORT);
  exit(1);
}

int main(int argc, char **argv) {
  struct mg_mgr mgr;
  const char *port = "1234";
  struct mg_connection *nc;
  struct ip_mreq group;
  const char *interface = NULL;
  const char *mcast_group = MCAST_GROUP;
  int i;

  /* Parse command line arguments */
  for (i = 1; i < argc; i++) {
    // IP address of the interface where to join a multicast group.
    if (strcmp(argv[i], "-i") == 0) {
      interface = argv[++i];
    } else if (strcmp(argv[i], "-g") == 0) {
      mcast_group = argv[++i];
    } else if (strcmp(argv[i], "-p") == 0) {
      port = argv[++i];
    }
  }

  if (interface == NULL) {
    usage(argv);
  }

  mg_mgr_init(&mgr, NULL);
  {
    char listen[256];
    snprintf(listen, sizeof(listen), "udp://%s", port);
    nc = mg_bind(&mgr, listen, ev_handler);
  }

  if (nc == NULL) {
    perror("cannot bind\n");
    exit(1);
  }

  group.imr_multiaddr.s_addr = inet_addr(mcast_group);
  group.imr_interface.s_addr = inet_addr(interface);
  if (setsockopt(nc->sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &group,
                 sizeof(group)) < 0) {
    perror("Adding multicast group error");
    exit(1);
  }

  printf("Starting mcast server on port %s listening to group %s\n", port,
         mcast_group);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}
