#define MIPTAPTEST_USING_DHCP 1

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

#include "mip_x_test.c"


// MIP TUNTAP driver
static size_t tap_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t received = read(*(int *) ifp->driver_data, buf, len);
  usleep(1);  // This is to avoid 100% CPU
  if (received < 0) return 0;
  return (size_t) received;
}

// ignore IPv6, testing segment loss under IPv4 is enough
static bool is_tcp_segment(const void *buf, size_t len) {
  const struct eth *eth = (const struct eth *) buf;
  const struct ip *ip;
  const struct tcp *tcp;
  size_t hlen;
  if (len < sizeof(*eth)) return false;
  if (eth->type != mg_htons(0x800)) return false;
  ip = (const struct ip *) (eth + 1);
  if (len < sizeof(*eth) + sizeof(*ip)) return false;
  hlen = (size_t) (ip->ver & 15) * 4;
  if ((ip->ver >> 4) != 4 || ip->proto != 6 || hlen < sizeof(*ip) ||
      len < sizeof(*eth) + hlen + sizeof(*tcp))
    return false;
  tcp = (const struct tcp *) ((const char *) ip + hlen);
  return (tcp->flags & (TH_SYN | TH_FIN)) == 0;
}

static size_t tap_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  uint32_t random;
  if (ifp->enable_tcp_retransmit && is_tcp_segment(buf, len)) {  // introduce a 30% segment loss
    mg_random(&random, sizeof(random));
    if (random % 10 < 3) {
      MG_DEBUG(("dropping TCP segment"));
      return len;
    }
  }
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


int main(void) {
  bool result;
  const char *debug_level = getenv("V");
  // Setup interface
  const char *iface = "tap0";             // Network iface
  const char *mac = "02:00:01:02:03:78";  // MAC address
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
    ABORT();  // return EXIT_FAILURE;
  }
#else
  ifr.ifr_flags = (short) (IFF_UP | IFF_BROADCAST | IFF_MULTICAST);
  if (ioctl(fd, TUNSIFMODE, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    ABORT();  // return EXIT_FAILURE;
  }
#endif
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode

  MG_INFO(("Opened TAP interface: %s", iface));
  usleep(200000);  // 200 ms

  if (debug_level == NULL) debug_level = "3";
  mg_log_set(atoi(debug_level));

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  // MIP driver
  struct mg_tcpip_driver driver;
  memset(&driver, 0, sizeof(driver));

  driver.tx = tap_tx;
  driver.poll = tap_poll;
  driver.rx = tap_rx;

  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));

  mif.driver = &driver;
  mif.driver_data = &fd;

#if MIPTAPTEST_USING_DHCP == 1
#else
  mif.ip = mg_htonl(MG_U32(192, 168, 32, 2));  // Triggering a network failure
  mif.mask = mg_htonl(MG_U32(255, 255, 255, 0));
  mif.gw = mg_htonl(MG_U32(192, 168, 32, 1));
#endif

  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);

  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));
  usleep(200000);  // 200 ms

  // Stack initialization, Network configuration (DHCP lease, ...)
#if MIPTAPTEST_USING_DHCP == 0
  MG_INFO(("MIF configuration: Static IP"));
  ASSERT(mif.ip != 0);     // Check we have a satic IP assigned
  mg_mgr_poll(&mgr, 100);  // For initialisation
#else
  MG_INFO(("MIF configuration: DHCP"));
  ASSERT(!mif.ip);  // Check we are set for DHCP
  int pc = 500;     // Timeout on DHCP lease 500 ~ approx 5s (typical delay <1s)
  while (((pc--) > 0) && !mif.ip) {
    mg_mgr_poll(&mgr, 100);
    usleep(10000);  // 10 ms
  }
  if (!mif.ip) MG_ERROR(("No ip assigned (DHCP lease may have failed).\n"));
  ASSERT(mif.ip);  // We have an IP (lease or static)
#endif
  while (mif.state != MG_TCPIP_STATE_READY) {
    mg_mgr_poll(&mgr, 100);
    usleep(10000);  // 10 ms
  }

  // RUN TESTS
  mif.enable_tcp_retransmit = getenv("RTX") != NULL;
  result = mip_x_test(&mgr);
  close(fd);
  if (!result) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
