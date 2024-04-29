// Copyright (c) 2022 Cesanta Software Limited
// All rights reserved
//
// example using built-in TCP/IP stack and TUN/TAP interface

#include <sys/socket.h>
#ifndef __OpenBSD__
#include <linux/if.h>
#include <linux/if_tun.h>
#else
#include <net/if.h>
#include <net/if_tun.h>
#include <net/if_types.h>
#endif
#include <sys/ioctl.h>

#include "mongoose.h"
#include "net.h"

static int s_signo;
void signal_handler(int signo) {
  s_signo = signo;
}

static size_t tap_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t res = write(*(int *) ifp->driver_data, buf, len);
  if (res < 0) {
    MG_ERROR(("tap_tx failed: %d", errno));
    return 0;
  }
  return (size_t) res;
}

static bool tap_up(struct mg_tcpip_if *ifp) {
  return ifp->driver_data ? true : false;
}

static size_t tap_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t received = read(*(int *) ifp->driver_data, buf, len);
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
#ifndef __OpenBSD__
  const char *tuntap_device = "/dev/net/tun";
#else
  const char *tuntap_device = "/dev/tap0";
#endif
  int fd = open(tuntap_device, O_RDWR);
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
#ifndef __OpenBSD__
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    abort();  // return EXIT_FAILURE;
  }
#else
  ifr.ifr_flags = (short) (IFF_UP | IFF_BROADCAST | IFF_MULTICAST);
  if (ioctl(fd, TUNSIFMODE, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    abort();  // return EXIT_FAILURE;
  }
#endif
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode

  MG_INFO(("Opened TAP interface: %s", iface));
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  struct mg_tcpip_driver driver = {.tx = tap_tx, .up = tap_up, .rx = tap_rx};
  struct mg_tcpip_if mif = {.driver = &driver, .driver_data = &fd};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);
  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  // Start infinite event loop
  MG_INFO(("Mongoose version : v%s", MG_VERSION));
  MG_INFO(("Listening on     : %s", HTTP_URL));
  MG_INFO(("Listening on     : %s", HTTPS_URL));

  web_init(&mgr);
  while (s_signo == 0) mg_mgr_poll(&mgr, 100);  // Infinite event loop

  mg_mgr_free(&mgr);
  close(fd);
  printf("Exiting on signal %d\n", s_signo);

  return 0;
}
