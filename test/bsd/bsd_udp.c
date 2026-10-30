#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 1235

int main(int argc, char *argv[]) {
  struct sockaddr_in sa;
  struct timeval timeout;
  char buf[8];
  ssize_t n;
  int fd, rogue;

  if (argc != 2) return 1;
  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(PORT);
  if (inet_pton(AF_INET, argv[1], &sa.sin_addr) != 1) return 1;
  timeout.tv_sec = 5, timeout.tv_usec = 0;
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
      setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0 ||
      connect(fd, (struct sockaddr *) &sa, sizeof(sa)) != 0 ||
      send(fd, "hello", 5, 0) != 5 || recv(fd, buf, sizeof(buf), 0) != 5 ||
      memcmp(buf, "ready", 5) != 0)
    goto fail;
  if ((rogue = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
      setsockopt(rogue, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) !=
          0 ||
      send(fd, "one", 3, 0) != 3 ||
      sendto(rogue, "two2", 4, 0, (struct sockaddr *) &sa, sizeof(sa)) != 4 ||
      recv(fd, buf, sizeof(buf), 0) != 4 || memcmp(buf, "peer", 4) != 0 ||
      recv(rogue, buf, sizeof(buf), 0) != 5 || memcmp(buf, "rogue", 5) != 0 ||
      send(fd, "truncate", 8, 0) != 8 || send(fd, "peek", 4, 0) != 4 ||
      recv(fd, buf, sizeof(buf), 0) != 4 || memcmp(buf, "more", 4) != 0 ||
      recv(fd, buf, sizeof(buf), 0) != 3 || memcmp(buf, "one", 3) != 0 ||
      recv(fd, buf, sizeof(buf), 0) != 4 || memcmp(buf, "two2", 4) != 0 ||
      recv(fd, buf, sizeof(buf), 0) != 0 ||
      sendto(rogue, "rogue", 5, 0, (struct sockaddr *) &sa, sizeof(sa)) != 5 ||
      recv(fd, buf, sizeof(buf), 0) != 7 || memcmp(buf, "checked", 7) != 0 ||
      send(fd, "echo", 4, 0) != 4 || recv(fd, buf, sizeof(buf), 0) != 4 ||
      memcmp(buf, "echo", 4) != 0)
    goto fail_close_rogue;
  close(rogue);
  close(fd);
  return 0;

fail_close_rogue:
  close(rogue);
fail:
  close(fd);
  return 1;
}
