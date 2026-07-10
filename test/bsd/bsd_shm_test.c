#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "mongoose.h"
#include "shm_queue.h"

#ifndef O_RDWR
#define O_RDWR 02
#endif

struct shm_driver_data {
  const char *name;
  FILE *fp;
  struct shm_link *shm;
};

static struct shm_driver_data shm_data = {
    SHM_NAME,
    NULL,
    NULL,
};

static bool shm_init(struct mg_tcpip_if *ifp) {
  struct shm_driver_data *d = (struct shm_driver_data *) ifp->driver_data;
  int fd;

  fd = shm_open(d->name, O_RDWR, 0600);
  if (fd < 0) {
    printf("shm_open failed: %d\n", errno);
    return false;
  }
  d->fp = fdopen(fd, "r+");
  if (d->fp == NULL) {
    printf("fdopen failed: %d\n", errno);
    return false;
  }

  d->shm = (struct shm_link *) mmap(NULL, sizeof(*d->shm),
                                    PROT_READ | PROT_WRITE, MAP_SHARED,
                                    fileno(d->fp), 0);
  if (d->shm == MAP_FAILED) {
    printf("mmap failed: %d\n", errno);
    fclose(d->fp);
    d->fp = NULL;
    d->shm = NULL;
    return false;
  }

  return true;
}

static size_t shm_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct shm_driver_data *d = (struct shm_driver_data *) ifp->driver_data;

  if (!shm_queue_push(&d->shm->to_tap, buf, len)) {
    printf("\nFRAME LOST\n");
    return 0;
  }
  return len;
}

static size_t shm_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  struct shm_driver_data *d = (struct shm_driver_data *) ifp->driver_data;
  return shm_queue_pop(&d->shm->from_tap, buf, len);
}

static bool shm_poll(struct mg_tcpip_if *ifp, bool s1) {
  return s1 && ifp->driver_data ? true : false;
}

static struct mg_tcpip_driver shm_driver = {
    shm_init,
    shm_tx,
    shm_rx,
    shm_poll,
};


bool mg_random(void *buf, size_t len) {
  bool success = false;
  unsigned char *p = (unsigned char *) buf;
  FILE *fp = fopen("/dev/urandom", "rb");
  if (fp != NULL) {
    if (fread(buf, 1, len, fp) == len) success = true;
    fclose(fp);
  }
  // If everything above did not work, fallback to a pseudo random generator
  if (success == false) {
    printf("\nWeak RNG: using rand()\n");
    while (len--) *p++ = (unsigned char) (rand() & 255);
  }
  return success;
}


#include "bsd_x_test.c"


int main(void) {
  bool result;
  const char *debug_level = getenv("V");
  // Setup interface
  const char *mac = "02:00:01:02:03:78";  // MAC address

  printf("Opening SHM interface\n");
  usleep(200000);  // 200 ms

  result = bsd_x_test(&shm_driver, &shm_data, mac, debug_level);
  if (shm_data.shm != NULL) munmap(shm_data.shm, sizeof(*shm_data.shm));
  if (shm_data.fp != NULL) fclose(shm_data.fp);
  if (!result) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
