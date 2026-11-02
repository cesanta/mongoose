// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved

#include "mongoose.h"

struct udp_server {
  int fd;
  bool pending;
  struct sockaddr_in peer;
  socklen_t peerlen;
  size_t len;
  char buf[128];
};

static struct udp_server s_server = {.fd = -1};

static int udp_nonblock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  return flags >= 0 && fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
}

int udp_init(uint16_t port) {
  struct sockaddr_in sa = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {.s_addr = INADDR_ANY},
  };
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (fd < 0 || !udp_nonblock(fd) ||
      bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    MG_ERROR(("BSD UDP init: %d", errno));
    if (fd >= 0) close(fd);
    return 0;
  }
  s_server.fd = fd;
  MG_INFO(("BSD UDP echo server on port %hu", port));
  return 1;
}

void udp_poll(void) {
  struct udp_server *s = &s_server;
  struct pollfd p;
  ssize_t n;

  if (s->fd < 0) return;
  p.fd = s->fd, p.events = s->pending ? POLLOUT : POLLIN, p.revents = 0;
  if (poll(&p, 1, 0) <= 0) return;
  if (s->pending) {
    n = sendto(s->fd, s->buf, s->len, 0, (struct sockaddr *) &s->peer,
               s->peerlen);
    if (n == (ssize_t) s->len) {
      s->pending = false;
    } else if (n < 0 && errno != EAGAIN) {
      MG_ERROR(("BSD UDP send: %d", errno));
      s->pending = false;
    }
  } else {
    s->peerlen = sizeof(s->peer);
    n = recvfrom(s->fd, s->buf, sizeof(s->buf), 0,
                 (struct sockaddr *) &s->peer, &s->peerlen);
    if (n >= 0) {
      s->len = (size_t) n, s->pending = true;
    } else if (errno != EAGAIN) {
      MG_ERROR(("BSD UDP recv: %d", errno));
    }
  }
}
