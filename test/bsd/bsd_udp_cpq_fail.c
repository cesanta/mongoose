// Demonstrates UDP behavior with the CPQ patch. Run this task with
// bsd_udp_cpq_peer against a Mongoose interface whose address it receives.
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "mongoose.h"

#include "FreeRTOS.h"
#include "task.h"

#define DATA_PORT 1234
#define CONTROL_PORT 1235
#define SEND_PORT 1236
#define PEER_A_PORT 14001

static char s_big[1600];

extern volatile bool s_cpq_done;
extern volatile bool s_cpq_ready;
extern bool s_cpq_remote;

void bsd_udp_cpq_fail_task(void *args);

static int udp_bind(unsigned port) {
  struct sockaddr_in sa;
  int fd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons((uint16_t) port);
  if (fd < 0 || bind(fd, (struct sockaddr *) &sa, sizeof(sa)) != 0) {
    printf("cannot bind UDP port %u: %d\n", port, errno);
    return -1;
  }
  return fd;
}

static void go(int fd) {
  if (send(fd, "go", 2, 0) != 2) printf("control send failed: %d\n", errno);
}

static void wait_for_packets(void) {
  vTaskDelay(pdMS_TO_TICKS(100));  // Let the manager collect peer datagrams
}

void bsd_udp_cpq_fail_task(void *args) {
  struct sockaddr_in control_peer, peer, local, other;
  socklen_t peerlen, locallen;
  char buf[16];
  int data = -1, control = -1, out = -1, fd = -1, connected = -1;
  ssize_t n;

  (void) args;
  data = udp_bind(DATA_PORT);
  control = udp_bind(CONTROL_PORT);
  out = udp_bind(SEND_PORT);
  if (data < 0 || control < 0 || out < 0) goto done;

  s_cpq_ready = true;
  printf("CPQ: control socket waiting\n");
  // The peer identifies itself on the control socket, which learns its MAC.
  peerlen = sizeof(control_peer);
  n = recvfrom(control, buf, sizeof(buf), 0,
               (struct sockaddr *) &control_peer, &peerlen);
  if (n != 5 || memcmp(buf, "ready", 5) != 0) goto done;
  printf("CPQ: peer ready\n");

  fd = udp_bind(1237);
  n = fd < 0 ? -1 : send(fd, "x", 1, 0);
  printf("%s send: expected EDESTADDRREQ, got %d: no peer check\n",
         n < 0 && errno == EDESTADDRREQ ? "PASS" : "FAIL", (int) n);
  if (fd >= 0) close(fd);

  fd = udp_bind(1238);
  n = fd < 0 ? -1 : connect(fd, (struct sockaddr *) &control_peer, peerlen);
  locallen = sizeof(local);
  if (n == 0 && getsockname(fd, (struct sockaddr *) &local, &locallen) == 0) {
    printf("%s bind: expected 1238, got %u: connect replaced bind\n",
           mg_ntohs(local.sin_port) == 1238 ? "PASS" : "FAIL",
           (unsigned) mg_ntohs(local.sin_port));
  } else {
    printf("FAIL bind: connect failed errno %d\n", errno);
  }
  if (fd >= 0) close(fd);

  fd = udp_bind(1239);
  other = control_peer, other.sin_port = htons(14998);
  n = fd < 0 ? -1 : sendto(fd, s_big, sizeof(s_big), 0,
                            (struct sockaddr *) &other, sizeof(other));
  printf("%s oversize: expected EMSGSIZE, got %d: later truncated\n",
         n < 0 && errno == EMSGSIZE ? "PASS" : "FAIL", (int) n);
  if (fd >= 0) close(fd);

  // The patch keeps the first nonempty datagram and drops the second.
  go(control);
  wait_for_packets();
  peerlen = sizeof(peer);
  n = recvfrom(data, buf, sizeof(buf), 0, (struct sockaddr *) &peer, &peerlen);
  if (n == 3 && memcmp(buf, "one", 3) == 0 &&
      mg_ntohs(peer.sin_port) == PEER_A_PORT &&
      recvfrom(data, buf, sizeof(buf), MSG_DONTWAIT, NULL, NULL) < 0 &&
      errno == EAGAIN) {
    printf("PASS drop: kept one, dropped two\n");
  } else {
    printf("FAIL drop: expected one then EAGAIN, got %d \"%.*s\"\n", (int) n,
           (int) (n > 0 ? n : 0), buf);
  }

  // A short UDP receive must discard the rest of that datagram.
  go(control);
  wait_for_packets();
  n = recvfrom(data, buf, 2, 0, NULL, NULL);
  n = recvfrom(data, buf, sizeof(buf), MSG_DONTWAIT, NULL, NULL);
  printf("%s short: expected EAGAIN, got %d \"%.*s\": tail retained\n",
         n < 0 && errno == EAGAIN ? "PASS" : "FAIL", (int) n,
         (int) (n > 0 ? n : 0), buf);

  // A zero-length datagram is data and recvfrom() must return zero for it.
  go(control);
  wait_for_packets();
  n = recvfrom(data, buf, sizeof(buf), MSG_DONTWAIT, NULL, NULL);
  printf("%s empty: expected 0, got %d errno %d: no marker\n",
         n == 0 ? "PASS" : "FAIL", (int) n, errno);

  // An unbound connected socket must reject another sender's datagram.
  if (send(control, "connect", 7, 0) != 7 ||
      (connected = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
      connect(connected, (struct sockaddr *) &control_peer,
              sizeof(control_peer)) != 0 ||
      send(connected, "hello", 5, 0) != 5) {
    goto done;
  }
  wait_for_packets();
  n = recv(connected, buf, sizeof(buf), MSG_DONTWAIT);
  printf("%s peer: expected EAGAIN, got %d \"%.*s\": no peer filter\n",
         n < 0 && errno == EAGAIN ? "PASS" : "FAIL", (int) n,
         (int) (n > 0 ? n : 0), buf);
  n = send(connected, "reply", 5, 0);
  printf("reply: sent %d bytes; peer reports its destination below\n", (int) n);
  if (recv(control, buf, sizeof(buf), 0) < 0) goto done;  // Peer checked reply
  other = control_peer, other.sin_port = htons(14999);
  n = sendto(connected, "x", 1, 0, (struct sockaddr *) &other, sizeof(other));
  printf("%s sendto: expected EISCONN, got %d: connected socket accepted it\n",
         n < 0 && errno == EISCONN ? "PASS" : "FAIL", (int) n);

  // sendto() reports success before this socket has learned a destination MAC.
  go(control);
  if (recv(control, buf, sizeof(buf), 0) < 0) goto done;
  n = sendto(out, "lost", 4, 0, (struct sockaddr *) &control_peer,
             sizeof(control_peer));
  printf("%s sendto: expected delivery, got %d: no resolution\n",
         n == 4 ? "FAIL" : "PASS", (int) n);
  if (recv(control, buf, sizeof(buf), 0) < 0) goto done;  // Peer reports miss

  // sendto() changes c->rem, so send() incorrectly succeeds on this socket.
  go(control);
  if (recvfrom(out, buf, sizeof(buf), 0, NULL, NULL) < 0) goto done;
  n = sendto(out, s_cpq_remote ? "stale" : "sendto", s_cpq_remote ? 5 : 6,
             0, (struct sockaddr *) &control_peer, sizeof(control_peer));
  printf("sendto after RX: got %d\n", (int) n);
  if (s_cpq_remote) go(control);  // Server checks its packet used helper's MAC
  n = send(out, "send", 4, 0);
  printf("%s send: expected EDESTADDRREQ, got %d: sendto set peer\n",
         n < 0 && errno == EDESTADDRREQ ? "PASS" : "FAIL", (int) n);

done:
  if (data >= 0) close(data);
  if (control >= 0) close(control);
  if (out >= 0) close(out);
  if (connected >= 0) close(connected);
  s_cpq_done = true;
  vTaskDelete(NULL);
}
