#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define DATA_PORT 1234
#define CONTROL_PORT 1235
#define SEND_PORT 1236
#define CONTROL_PEER_PORT 14000
#define PEER_A_PORT 14001
#define PEER_B_PORT 14002
#define HELPER_PORT 14003

static int open_peer(unsigned port) {
  struct sockaddr_in sa;
  struct timeval timeout = {1, 0};
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  sa.sin_port = htons((uint16_t) port);
  if (fd < 0 || bind(fd, (struct sockaddr *) &sa, sizeof(sa)) != 0 ||
      setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0)
    return -1;
  return fd;
}

static int make_peer(struct sockaddr_in *sa, const char *ip, unsigned port) {
  memset(sa, 0, sizeof(*sa));
  sa->sin_family = AF_INET;
  sa->sin_port = htons((uint16_t) port);
  return inet_pton(AF_INET, ip, &sa->sin_addr) == 1 ? 0 : -1;
}

static int send_packet(int fd, const struct sockaddr_in *sa, const char *buf,
                       size_t len) {
  return sendto(fd, buf, len, 0, (const struct sockaddr *) sa, sizeof(*sa)) ==
                 (ssize_t) len
             ? 0
             : -1;
}

static int wait_go(int fd, const char *step) {
  char buf[8];
  int n = recv(fd, buf, sizeof(buf), 0);
  if (n == 2 && memcmp(buf, "go", 2) == 0) return 0;
  printf("peer: %s failed: %d\n", step, n);
  return -1;
}

static void stale_packet(void) {
  printf("TRACE stale MAC: Server received packet despite wrong L2 address\n");
}

static int wait_stale_go(int fd) {
  char buf[8];
  int n, stale = 0;

  for (;;) {
    n = recv(fd, buf, sizeof(buf), 0);
    if (n == 5 && memcmp(buf, "stale", 5) == 0) {
      stale = 1;
      stale_packet();
    } else if (n == 2 && memcmp(buf, "go", 2) == 0) {
      if (stale) return 0;
      n = recv(fd, buf, sizeof(buf), 0);
      if (n == 5 && memcmp(buf, "stale", 5) == 0)
        stale_packet();
      else
        printf("FAIL stale MAC: expected packet, got none\n");
      return 0;
    } else {
      printf("peer: stale MAC handoff failed: %d\n", n);
      return -1;
    }
  }
}

static int wait_connect(int fd, struct sockaddr_in *peer) {
  char buf[8];
  socklen_t len = sizeof(*peer);
  return recv(fd, buf, sizeof(buf), 0) == 7 && memcmp(buf, "connect", 7) == 0 &&
                 recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) peer, &len) == 5 &&
                 memcmp(buf, "hello", 5) == 0
             ? 0
             : -1;
}

static int wait_helper(int fd, struct sockaddr_in *peer) {
  char buf[8];
  socklen_t len = sizeof(*peer);
  ssize_t n;

  while ((n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) peer,
                       &len)) < 0)
    len = sizeof(*peer);
  return n == 5 && memcmp(buf, "ready", 5) == 0 ? 0 : -1;
}

static int run_helper(const char *server_ip, const char *target_ip) {
  struct sockaddr_in server, target;
  char buf[32];
  unsigned port;
  int fd, n;

  if (make_peer(&server, server_ip, HELPER_PORT) != 0 ||
      make_peer(&target, target_ip, 0) != 0 || (fd = open_peer(0)) < 0)
    return 1;
  for (;;) {
    if (send_packet(fd, &server, "ready", 5) != 0) break;
    n = recv(fd, buf, sizeof(buf) - 1, 0);
    if (n <= 0) continue;  // Retry ready until the Server receives it
    buf[n] = '\0';
    if (sscanf(buf, "intrude %u", &port) == 1 && port < 65536) {
      target.sin_port = htons((uint16_t) port);
      if (send_packet(fd, &target, "intruder", 8) != 0) break;
    } else if (strcmp(buf, "prime") == 0) {
      target.sin_port = htons(SEND_PORT);
      if (send_packet(fd, &target, "prime", 5) != 0) break;
      return 0;
    } else if (strcmp(buf, "reply") == 0) {
      printf("FAIL connected peer: helper received reply\n");
    }
  }
  close(fd);
  return 1;
}

static int run_peer(const char *target_ip, int remote) {
  struct sockaddr_in data, control, out, peer, helper_peer;
  char buf[32], cmd[32];
  int a, b, ctl, helper = -1;
  ssize_t n;

  if (make_peer(&data, target_ip, DATA_PORT) != 0) return 1;
  control = data, out = data;
  control.sin_port = htons(CONTROL_PORT);
  out.sin_port = htons(SEND_PORT);
  ctl = open_peer(CONTROL_PEER_PORT);
  a = open_peer(PEER_A_PORT);
  b = open_peer(PEER_B_PORT);
  if (remote) helper = open_peer(HELPER_PORT);
  if (ctl < 0 || a < 0 || b < 0 || (remote && helper < 0)) return 1;
  if (remote && wait_helper(helper, &helper_peer) != 0) return 1;
  if (remote) printf("peer: helper ready\n");

  if (send_packet(ctl, &control, "ready", 5) != 0 ||
      wait_go(ctl, "initial go") != 0)
    return 1;
  if (send_packet(a, &data, "one", 3) != 0 ||
      send_packet(b, &data, "two", 3) != 0 ||
      wait_go(ctl, "two datagrams") != 0)
    return 1;
  if (send_packet(b, &data, "ABCDE", 5) != 0 ||
      wait_go(ctl, "short datagram") != 0)
    return 1;
  if (send_packet(b, &data, "", 0) != 0 || wait_connect(ctl, &peer) != 0) {
    printf("peer: connect handshake failed\n");
    return 1;
  }
  if (remote) {
    snprintf(cmd, sizeof(cmd), "intrude %u", (unsigned) ntohs(peer.sin_port));
    if (send_packet(helper, &helper_peer, cmd, strlen(cmd)) != 0) return 1;
  } else if (send_packet(b, &peer, "intruder", 8) != 0) {
    return 1;
  }
  n = recv(ctl, buf, sizeof(buf), 0);
  printf("%s reply: expected control, got %s: intruder replaced peer\n",
         n < 0 ? "FAIL" : "PASS", n < 0 ? "none" : "control");
  if (!remote) {
    n = recv(b, buf, sizeof(buf), 0);
    printf("%s reply: intruder got %.*s\n", n > 0 ? "FAIL" : "PASS",
           (int) (n > 0 ? n : 0), buf);
  }
  if (send_packet(ctl, &control, "done", 4) != 0 ||
      wait_go(ctl, "first sendto") != 0 ||
      send_packet(ctl, &control, "sendlost", 8) != 0)
    return 1;

  n = recv(ctl, buf, sizeof(buf), 0);
  printf("%s sendto: expected packet, got %s\n", n < 0 ? "FAIL" : "PASS",
         n < 0 ? "none" : "packet");
  if (send_packet(ctl, &control, "miss", 4) != 0 ||
      wait_go(ctl, "prime") != 0)
    return 1;
  if (remote) {
    if (send_packet(helper, &helper_peer, "prime", 5) != 0 ||
        wait_stale_go(ctl) != 0)
      return 1;
  } else {
    if (send_packet(ctl, &out, "prime", 5) != 0) return 1;
    n = recv(ctl, buf, sizeof(buf), 0);
    printf("sendto after RX: %.*s\n", (int) (n > 0 ? n : 0), buf);
    n = recv(ctl, buf, sizeof(buf), 0);
    printf("%s send: expected none, got %.*s\n", n > 0 ? "FAIL" : "PASS",
           (int) (n > 0 ? n : 0), buf);
  }
  close(a);
  close(b);
  close(ctl);
  if (helper >= 0) close(helper);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 2) return run_peer(argv[1], 0);
  if (argc == 3 && strcmp(argv[1], "--remote") == 0)
    return run_peer(argv[2], 1);
  if (argc == 4 && strcmp(argv[1], "--helper") == 0)
    return run_helper(argv[2], argv[3]);
  return 1;
}
