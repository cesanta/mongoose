#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <signal.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#define FRAME_SIZE 1540
#define FRAME_QUEUE_DEPTH 8
#define SHM_NAME "/mongoose-shm"

struct shm_frame {
  size_t len;
  uint8_t buf[FRAME_SIZE];
};

struct shm_queue {
  sem_t full;
  sem_t empty;
  size_t head;
  size_t tail;
  struct shm_frame frames[FRAME_QUEUE_DEPTH];
};

struct shm_link {
  struct shm_queue to_tap;
  struct shm_queue from_tap;
};

static volatile sig_atomic_t s_signo;
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
  if (shmfd < 0) return EXIT_FAILURE;
  if (ftruncate(shmfd, sizeof(*shm)) != 0) return EXIT_FAILURE;
  shm = (struct shm_link *) mmap(NULL, sizeof(*shm), PROT_READ | PROT_WRITE,
                                 MAP_SHARED, shmfd, 0);
  if (shm == MAP_FAILED) return EXIT_FAILURE;

  memset(shm, 0, sizeof(*shm));
  sem_init(&shm->to_tap.full, 1, 0);
  sem_init(&shm->to_tap.empty, 1, FRAME_QUEUE_DEPTH);
  sem_init(&shm->from_tap.full, 1, 0);
  sem_init(&shm->from_tap.empty, 1, FRAME_QUEUE_DEPTH);
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
          struct shm_queue *q = &shm->from_tap;
          ssize_t nread;
          if (sem_trywait(&q->empty) == 0) {
            struct shm_frame *f = &q->frames[q->tail];
            nread = read(fd, f->buf, sizeof(f->buf));
//          printf("Got at TAP: %d bytes\n", (int) nread);
            if (nread > 0) {
              f->len = (size_t) nread;
              q->tail = (q->tail + 1) % FRAME_QUEUE_DEPTH;
              sem_post(&q->full);
//            printf("Sent to SHM\n");
            } else {
              sem_post(&q->empty);
            }
          }
        }
				break;
			}
      if (sem_trywait(&shm->to_tap.full) == 0) {
        struct shm_queue *q = &shm->to_tap;
        struct shm_frame *f = &q->frames[q->head];
//      printf("Got at SHM: %d bytes\n", (int) f->len);
        write(fd, f->buf, f->len);
        q->head = (q->head + 1) % FRAME_QUEUE_DEPTH;
        sem_post(&q->empty);
//      printf("Sent to TAP\n");
      }
		} while (result >= 0 && s_signo == 0);
    if (result < 0)
      printf("looks like we got an error here... should I quit ? Waiting for CTRL-C\n");
	}
  sem_destroy(&shm->from_tap.empty);
  sem_destroy(&shm->from_tap.full);
  sem_destroy(&shm->to_tap.empty);
  sem_destroy(&shm->to_tap.full);
  munmap(shm, sizeof(*shm));
  close(shmfd);
  shm_unlink(SHM_NAME);
  close(fd);
}
