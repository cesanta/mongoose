#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <signal.h>

#include <linux/if.h>
#include <linux/if_tun.h>
#include "shm_queue.h"

static volatile sig_atomic_t s_signo;
static int s_trace_udp;

static void trace_udp(const uint8_t *p, size_t len) {
  if (!s_trace_udp || len < 34 || p[12] != 8 || p[13] != 0 || p[23] != 17)
    return;
  printf("UDP IP dst %u.%u.%u.%u -> Ethernet dst %02x:%02x:%02x:%02x:%02x:%02x\n",
         p[30], p[31], p[32], p[33], p[0], p[1], p[2], p[3], p[4], p[5]);
}

void signal_handler(int signo) {
  s_signo = signo;
}

int main(void) {
  // Setup interface
  const char *iface = "tap0";             // Network iface
  const char *tuntap_device = "/dev/net/tun";
  int fd = open(tuntap_device, O_RDWR);
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
    printf("Failed to setup TAP interface: %s", ifr.ifr_name);
    return EXIT_FAILURE;
  }
  printf("Opened TAP interface: %s\n", iface);

  int shmfd;
  struct shm_link *shm;

  shm_unlink(SHM_NAME);
  shmfd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
  if (shmfd < 0) {
    printf("shm_open failed: %d\n", errno);
    return EXIT_FAILURE;
  }
  if (ftruncate(shmfd, sizeof(*shm)) != 0) {
    printf("ftruncate failed: %d\n", errno);
    return EXIT_FAILURE;
  }
  shm = (struct shm_link *) mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE,
                                 MAP_SHARED, shmfd, 0);
  if (shm == MAP_FAILED) {
    printf("mmap failed: %d\n", errno);
    return EXIT_FAILURE;
  }

  memset(shm, 0, sizeof(*shm));
  s_trace_udp = getenv("TRACE_UDP") != NULL;
  printf("Opened shared memory: %s\n", SHM_NAME);
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  while (s_signo == 0) {
    struct pollfd pfd[1];
		int result;
		do {
			pfd[0].fd = fd;
			pfd[0].events = POLLIN | POLLPRI;		// will wait for input available (read ready), use POLLOUT for write

			// only read ready shown here, for write ready use the 3rd arg, and the 4th for exceptions
			result = poll(pfd, 1, 1);		// 1 pfd, TIMEOUT in milliseconds
			switch(result) {
			case -1: 	// error
				break;
			case 0: 	// timeout (if a timeout struct was passed to select)
				break;
			default: 	// the number of fd/sockets with data available (see pfd.revents for each one);
        if (pfd[0].revents & (POLLIN | POLLPRI)) {
          uint8_t buf[FRAME_SIZE];
          ssize_t nread = read(fd, buf, sizeof(buf));
//          printf("Got at TAP: %d bytes\n", (int) nread);
          if (nread > 0) {
            if (!shm_queue_push(&shm->from_tap, buf, (size_t) nread)) {
              printf("FRAME LOST: TAP -> SHM\n");
            }
//            printf("Sent to SHM\n");
          }
        }
				break;
			}
      {
        uint8_t buf[FRAME_SIZE];
        size_t n = shm_queue_pop(&shm->to_tap, buf, sizeof(buf));
        if (n > 0) {
          trace_udp(buf, n);
//      printf("Got at SHM: %d bytes\n", (int) f->len);
          write(fd, buf, n);
//      printf("Sent to TAP\n");
        }
      }
		} while (result >= 0 && s_signo == 0);
    if (result < 0)
      printf("looks like we got an error here... should I quit ? Waiting for CTRL-C\n");
	}
  munmap(shm, sizeof(*shm));
  close(shmfd);
  shm_unlink(SHM_NAME);
  close(fd);
}
