#define _POSIX_C_SOURCE 200809L

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <poll.h>
#include <unistd.h>

#include "shm_queue.h"

static volatile sig_atomic_t s_signo;

static void signal_handler(int signo) {
  s_signo = signo;
}

static int open_udp_socket(void) {
  const uint16_t port = 0xAA55;      // Local UDP port
  const uint16_t tap_port = 0x55AA;  // Host port_tap_bridge UDP port
  struct sockaddr_in port_addr, tap_addr;
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&port_addr, 0, sizeof(port_addr));
  port_addr.sin_family = AF_INET;
  port_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  port_addr.sin_port = htons(port);
  if (bind(fd, (struct sockaddr *) &port_addr, sizeof(port_addr)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }
  printf("Opened UDP socket: 127.0.0.1:%u (0x%04x)\n", (unsigned) port,
         (unsigned) port);

  memset(&tap_addr, 0, sizeof(tap_addr));
  tap_addr.sin_family = AF_INET;
  tap_addr.sin_addr.s_addr = htonl(0x7f000001U);  // 127.0.0.1
  tap_addr.sin_port = htons(tap_port);
  if (connect(fd, (struct sockaddr *) &tap_addr, sizeof(tap_addr)) < 0) {
    perror("connect");
    close(fd);
    return -1;
  }
  printf("Connected to TAP UDP socket: 127.0.0.1:%u (0x%04x)\n",
         (unsigned) tap_port, (unsigned) tap_port);

  return fd;
}

static struct shm_link *open_shm(int *shmfd) {
  struct shm_link *shm;

  shm_unlink(SHM_NAME);
  *shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
  if (*shmfd < 0) {
    perror("shm_open");
    return NULL;
  }
  if (ftruncate(*shmfd, (off_t) sizeof(*shm)) != 0) {
    perror("ftruncate");
    close(*shmfd);
    shm_unlink(SHM_NAME);
    return NULL;
  }
  shm = (struct shm_link *) mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE,
                                 MAP_SHARED, *shmfd, 0);
  if (shm == MAP_FAILED) {
    perror("mmap");
    close(*shmfd);
    shm_unlink(SHM_NAME);
    return NULL;
  }

  memset(shm, 0, sizeof(*shm));
  printf("Opened shared memory: %s\n", SHM_NAME);

  return shm;
}

int main(void) {
  int sockfd, shmfd = -1;
  struct shm_link *shm;

  sockfd = open_udp_socket();
  if (sockfd < 0) return EXIT_FAILURE;
  shm = open_shm(&shmfd);
  if (shm == NULL) {
    close(sockfd);
    return EXIT_FAILURE;
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  while (s_signo == 0) {
    struct pollfd pfd = {sockfd, (short) (POLLIN | POLLPRI), 0};
    int result = poll(&pfd, 1, 1);
    if (result < 0 && errno != EINTR) {
      perror("poll");
      break;
    }
    if (result > 0 && (pfd.revents & (POLLIN | POLLPRI))) {
      uint8_t buf[FRAME_SIZE];
      ssize_t nread = recv(sockfd, buf, sizeof(buf), 0);
      if (nread > 0 &&
          !shm_queue_push(&shm->from_tap, buf, (size_t) nread)) {
        printf("FRAME LOST: port -> SHM\n");
      }
    }
    {
      uint8_t buf[FRAME_SIZE];
      size_t n = shm_queue_pop(&shm->to_tap, buf, sizeof(buf));
      if (n > 0) {
        if (send(sockfd, buf, n, 0) < 0) perror("send");
      }
    }
  }

  munmap(shm, sizeof(*shm));
  close(shmfd);
  shm_unlink(SHM_NAME);
  close(sockfd);
  return EXIT_SUCCESS;
}
