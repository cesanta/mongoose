// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// example using MIP and a TUN/TAP interface

#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include "mongoose.h"

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

static size_t tap_tx(const void *buf, size_t len, void *userdata) {
  ssize_t res = write(*(int*) userdata, buf, len);
  if (res < 0) {
    MG_ERROR(("tap_tx failed: %d", errno));
    return 0;
  }
  return (size_t) res;
}

static bool tap_up(void *userdata) {
  return userdata ? true : false;
}

static size_t tap_rx(void *buf, size_t len, void *userdata) {
  ssize_t received = read(*(int *) userdata, buf, len);
  usleep(1);  // This is to avoid 100% CPU
  if (received < 0) return 0;
  return (size_t) received;
}

int main(int argc, char *argv[]) {
  const char *iface = "tap0";             // Network iface
  const char *mac = "02:00:01:02:03:77";  // MAC address

  // Parse options
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      iface = argv[++i];
    } else if (strcmp(argv[i], "-mac") == 0 && i + 1 < argc) {
      mac = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
      mg_log_set(atoi(argv[++i]));
    } else {
      MG_ERROR(("unknown option %s", argv[i]));
      return EXIT_FAILURE;
    }
  }

  // Open network interface
  int fd = open("/dev/net/tun", O_RDWR);
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    return EXIT_FAILURE;
  }
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode

  MG_INFO(("Opened TAP interface: %s", iface));
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  struct mip_driver driver = {.tx = tap_tx, .up = tap_up, .rx = tap_rx};
  struct mip_if mif = {.use_dhcp = true, .driver = &driver, .driver_data = &fd};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);
  mip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  extern void device_dashboard_fn(struct mg_connection *, int, void *, void *);
  mg_http_listen(&mgr, "http://0.0.0.0:8000", device_dashboard_fn, &mgr);

  while (s_signo == 0) mg_mgr_poll(&mgr, 100);  // Infinite event loop

  mg_mgr_free(&mgr);
  close(fd);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
