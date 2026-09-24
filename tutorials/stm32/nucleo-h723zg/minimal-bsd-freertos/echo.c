// Copyright (c) 2026 Cesanta Software Limited
// All rights reserved
//
// BSD socket tasks: TCP echo, UDP echo, and an HTTP client.

#include <FreeRTOS.h>
#include <task.h>

#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>

#define PRIO (configMAX_PRIORITIES - 1)

// Connect to mongoose.ws:80, fetch "/", log response length.
static void web_client_task(void *args) {
  taskYIELD();
  struct hostent *h = gethostbyname("mongoose.ws");
  if (!h) {
    printf("DNS failed\n");
    vTaskDelete(NULL);
    return;
  }
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {.sin_family = AF_INET, .sin_port = htons(80)};
  memcpy(&sa.sin_addr, h->h_addr, 4);
  if (connect(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    printf("connect failed\n");
    close(fd);
    vTaskDelete(NULL);
    return;
  }
  const char *req = "GET / HTTP/1.0\r\nHost: mongoose.ws\r\n\r\n";
  send(fd, req, strlen(req), 0);
  size_t total = 0;
  char buf[512];
  ssize_t n;
  while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) total += (size_t) n;
  printf("mongoose.ws response: %lu bytes\n", (unsigned long) total);
  close(fd);
  vTaskDelete(NULL);
  (void) args;
}

// Client connection: socket fd + linked-list pointer
struct peer {
  int fd;
  struct peer *next;
};

// TCP echo: multiplex clients with select() over a linked list of sockets
static void tcp_echo_task(void *args) {
  int lfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in sa = {
      .sin_family = AF_INET,
      .sin_port = htons(1234),
      .sin_addr = {.s_addr = INADDR_ANY},
  };
  struct peer *head = NULL;
  bind(lfd, (struct sockaddr *) &sa, sizeof(sa));
  listen(lfd, 5);
  printf("BSD TCP echo server on :1234\n");
  for (;;) {
    fd_set rfds;
    int maxfd = lfd;
    struct peer *c, **link;
    FD_ZERO(&rfds);
    FD_SET(lfd, &rfds);
    for (c = head; c != NULL; c = c->next) {
      FD_SET(c->fd, &rfds);
      if (c->fd > maxfd) maxfd = c->fd;
    }
    if (select(maxfd + 1, &rfds, NULL, NULL, NULL) <= 0) continue;
    if (FD_ISSET(lfd, &rfds)) {
      int fd = accept(lfd, NULL, NULL);
      if (fd >= 0) {
        struct peer *nc = (struct peer *) calloc(1, sizeof(*nc));
        if (nc != NULL) {
          nc->fd = fd;
          nc->next = head;
          head = nc;
        } else {
          close(fd);
        }
      }
    }
    for (link = &head; (c = *link) != NULL;) {
      if (FD_ISSET(c->fd, &rfds)) {
        char buf[512];
        ssize_t n = recv(c->fd, buf, sizeof(buf), 0);
        if (n > 0) {
          send(c->fd, buf, (size_t) n, 0);
          link = &c->next;
        } else {
          *link = c->next;  // EOF or error: drop the client
          close(c->fd);
          free(c);
        }
      } else {
        link = &c->next;
      }
    }
  }
  (void) args;
}

// Echo UDP datagrams back to the sender
static void udp_echo_task(void *args) {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in sa = {
      .sin_family = AF_INET,
      .sin_port = htons(1235),
      .sin_addr = {.s_addr = INADDR_ANY},
  };
  char buf[512];
  if (fd < 0 || bind(fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
    printf("udp bind failed\n");
    vTaskDelete(NULL);
    return;
  }
  printf("BSD UDP echo server on :1235\n");
  for (;;) {
    struct sockaddr_in peer;
    socklen_t peerlen = sizeof(peer);
    ssize_t n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &peer,
                         &peerlen);
    if (n > 0) sendto(fd, buf, (size_t) n, 0, (struct sockaddr *) &peer, peerlen);
  }
  (void) args;
}

void echo_start(void) {
  xTaskCreate(tcp_echo_task, "tcp_echo", 1024, NULL, PRIO, 0);
  xTaskCreate(udp_echo_task, "udp_echo", 256, NULL, PRIO, 0);
  xTaskCreate(web_client_task, "web_client", 256, NULL, PRIO, 0);
}
