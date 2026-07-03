#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

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

#include "mongoose.c"

#define WAIT_MS 5000

static size_t tap_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t received = read(*(int *) ifp->driver_data, buf, len);
  usleep(1);
  if (received < 0) return 0;
  return (size_t) received;
}

static size_t tap_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t res = write(*(int *) ifp->driver_data, buf, len);
  if (res < 0) {
    MG_ERROR(("tap_tx failed: %d", errno));
    return 0;
  }
  return (size_t) res;
}

static bool tap_poll(struct mg_tcpip_if *ifp, bool s1) {
  return s1 && ifp->driver_data ? true : false;
}

int main(int argc, char *argv[]) {
  int res = EXIT_SUCCESS;
  const char *iface = "tap0";
  const char *mac = "02:00:01:02:03:78";
#ifndef __OpenBSD__
  const char *tuntap_device = "/dev/net/tun";
#else
  const char *tuntap_device = "/dev/tap0";
#endif
  uint16_t vlan_id = 3;
  int fd, i;
  struct ifreq ifr;
  struct mg_mgr mgr;
  struct mg_tcpip_driver driver;
  struct mg_tcpip_if mif;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
      iface = argv[++i];
    } else if (strcmp(argv[i], "-mac") == 0 && i + 1 < argc) {
      mac = argv[++i];
    } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
      mg_log_set(atoi(argv[++i]));
    } else if (strcmp(argv[i], "-vlan") == 0 && i + 1 < argc) {
      vlan_id = (uint16_t) atoi(argv[++i]);
    } else {
      MG_ERROR(("unknown option %s", argv[i]));
      return EXIT_FAILURE;
    }
  }

  fd = open(tuntap_device, O_RDWR);
  if (fd < 0) {
    MG_ERROR(("Failed to open TAP device: %d", errno));
    return EXIT_FAILURE;
  }

  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
#ifndef __OpenBSD__
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    close(fd);
    return EXIT_FAILURE;
  }
#else
  ifr.ifr_flags = (short) (IFF_UP | IFF_BROADCAST | IFF_MULTICAST);
  if (ioctl(fd, TUNSIFMODE, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    close(fd);
    return EXIT_FAILURE;
  }
#endif
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

  memset(&driver, 0, sizeof(driver));
  driver.tx = tap_tx;
  driver.rx = tap_rx;
  driver.poll = tap_poll;

  memset(&mif, 0, sizeof(mif));
  mif.driver = &driver;
  mif.driver_data = &fd;
#if MG_ENABLE_IPV6
  mif.ip6[0] = mg_htonll(0x100000000000000);
  mif.prefix[0] = 1;
  mif.prefix_len = 64;
  mif.gw6[0] = mg_htonll(0x100000000000000);
  mif.gw6_ready = true;
#endif
  mif.l2type = MG_TCPIP_L2_PPPoE;
  mif.l2data.eth.vlan_id = vlan_id;
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);

  mg_mgr_init(&mgr);
  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Opened TAP interface %s, VLAN %u", iface, vlan_id));

  for (i = 0; i < WAIT_MS / 10 && (mif.state != MG_TCPIP_STATE_READY
#if MG_ENABLE_IPV6
                                   || mif.state6 != MG_TCPIP_STATE_READY
#endif
                                  ); i++) {
    mg_mgr_poll(&mgr, 10);
    usleep(10000);
  }

  if (mif.state != MG_TCPIP_STATE_READY) {
    MG_ERROR(("IP FAILED %u", mif.state));
    res = EXIT_FAILURE;
  }
#if MG_ENABLE_IPV6
  if (mif.state6 != MG_TCPIP_STATE_READY) {
    MG_ERROR(("IPv6 FAILED %u", mif.state6));
    res = EXIT_FAILURE;
  }
#endif

  mg_mgr_free(&mgr);
  close(fd);
  return res;
}
