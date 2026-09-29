// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// This is an implementation of the simple TCP echo server which supports
// multiple concurrent connections, using BSD sockets API.

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>

struct echo_server {
  int sock;
  struct echo_client {
    struct echo_client *next;
    int sock;
    size_t len, sent;
    char buf[128];
  } *clients;
};

static struct echo_server s_server = {-1, NULL};

static int echo_nonblock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  return flags >= 0 && fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
}

static void echo_remove(struct echo_client **p) {
  struct echo_client *c = *p;
  *p = c->next;
  close(c->sock);
  free(c);
}

int echo_init(uint16_t port) {
  struct sockaddr_in sa = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr = {.s_addr = INADDR_ANY},
  };
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd < 0 || !echo_nonblock(fd) ||
      bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0 || listen(fd, 3) < 0) {
    printf("BSD echo init: %d\n", errno);
    if (fd >= 0) close(fd);
    return 0;
  }
  s_server.sock = fd;
  printf("BSD echo server on port %hu\n", port);
  return 1;
}

void echo_poll(void) {
  struct echo_server *s = &s_server;
  struct echo_client *c, **p;
  int maxfd, n;
  fd_set r, w;

  if (s->sock < 0) return;
  maxfd = s->sock;
  FD_ZERO(&r);
  FD_ZERO(&w);
  FD_SET(s->sock, &r);
  for (c = s->clients; c != NULL; c = c->next) {
    if (c->len == c->sent)
      FD_SET(c->sock, &r);
    else
      FD_SET(c->sock, &w);
    if (c->sock > maxfd) maxfd = c->sock;
  }

  if (select(maxfd + 1, &r, &w, NULL, NULL) <= 0) return;

  if (FD_ISSET(s->sock, &r)) {
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    int fd = accept(s->sock, (struct sockaddr *) &sa, &len);
    if (fd >= 0) {
      c = (struct echo_client *) calloc(1, sizeof(*c));
      if (c == NULL || !echo_nonblock(fd)) {
        printf("BSD echo accept: %d\n", c == NULL ? ENOMEM : errno);
        close(fd);
        free(c);
      } else {
        c->sock = fd;
        c->next = s->clients;
        s->clients = c;
      }
    }
  }

  p = &s->clients;
  while (*p != NULL) {
    c = *p;
    if (c->sent < c->len && FD_ISSET(c->sock, &w)) {
      n = (int) send(c->sock, c->buf + c->sent, c->len - c->sent, 0);
      if (n > 0) c->sent += (size_t) n;
      if (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        echo_remove(p);
        continue;
      }
    }
    if (c->sent == c->len && FD_ISSET(c->sock, &r)) {
      n = (int) recv(c->sock, c->buf, sizeof(c->buf), 0);
      if (n > 0) c->len = (size_t) n, c->sent = 0;
      if (n == 0 || (n < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
        echo_remove(p);
        continue;
      }
    }
    if (c->sent == c->len) c->len = c->sent = 0;
    p = &c->next;
  }
}
