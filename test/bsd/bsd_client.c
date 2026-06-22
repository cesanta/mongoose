#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1234
#define NUM_THREADS 12
#define DATA_SIZE (4 * 1024)

static int read_full(int fd, void *buf, size_t len) {
  char *p = (char *) buf;
  while (len > 0) {
    ssize_t n = recv(fd, p, len, 0);
    if (n <= 0) return -1;
    p += n;
    len -= (size_t) n;
  }
  return 0;
}

static int write_full(int fd, const void *buf, size_t len) {
  const char *p = (const char *) buf;
  while (len > 0) {
    ssize_t n = send(fd, p, len, 0);
    if (n <= 0) return -1;
    p += n;
    len -= (size_t) n;
  }
  return 0;
}

static void fill_text(char *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) buf[i] = (char) (' ' + random() % 95);
}

static void *worker(void *arg) {
  const char *host = (const char *) arg;
  char tx[DATA_SIZE], rx[DATA_SIZE];
  struct sockaddr_in sa;
  unsigned port = 0;
  int fd;

  fill_text(tx, sizeof(tx));
  memset(rx, 0, sizeof(rx));

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) goto fail;

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(PORT);
  if (inet_pton(AF_INET, host, &sa.sin_addr) != 1) goto fail_close;
  printf("%d CONNECTING\n", fd);
  if (connect(fd, (struct sockaddr *) &sa, sizeof(sa)) != 0) goto fail_close;
  {
    socklen_t len = sizeof(sa);
    if (getsockname(fd, (struct sockaddr *) &sa, &len) != 0) goto fail_close;
    port = (unsigned) ntohs(sa.sin_port);
  }
  printf("%d CONNECTED port: %u\n", fd, port);
  printf("%d WRITING\n", fd);
  if (write_full(fd, tx, sizeof(tx)) != 0) goto fail_close;
  printf("%d READING\n", fd);
  if (read_full(fd, rx, sizeof(rx)) != 0) goto fail_close;
  if (memcmp(tx, rx, sizeof(tx)) != 0) goto fail_close;

  printf("%d OK-CLOSING\n", fd);
  close(fd);
  return NULL;

fail_close:
  printf("%d ERROR-CLOSING\n", fd);
  close(fd);
fail:
  exit(1);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int i;

  if (argc != 2) {
    fprintf(stderr, "usage: %s IP\n", argv[0]);
    return 1;
  }

  srandom(1);

  for (i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, worker, argv[1]) != 0) return 1;
  }

  for (i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) return 1;
  }

  return 0;
}
