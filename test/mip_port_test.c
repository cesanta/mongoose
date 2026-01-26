#define MIPPORTTEST_USING_DHCP 1

#include <sys/socket.h>
#include <sys/ioctl.h>

#include "mip_x_test.c"

struct port_driver_data {
  int sockfd;
  struct sockaddr_in port_addr;
  struct sockaddr_in tap_addr;
};

// MIP Socket driver
static size_t sock_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t received = recv(((struct port_driver_data *)ifp->driver_data)->sockfd, buf, len, 0); 
  usleep(1);  // This is to avoid 100% CPU
  if (received < 0) return 0;
  return (size_t) received;
}

static size_t sock_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t res = send(((struct port_driver_data *)ifp->driver_data)->sockfd, buf, len, 0); 
  if (res < 0) {
    MG_ERROR(("sock_tx failed: %d", errno));
    return 0;
  }
  return (size_t) res;
}

static bool sock_poll(struct mg_tcpip_if *ifp, bool s1) {
  return s1 && ifp->driver_data ? true : false;
}


int main(void) {
  bool result;
  const char *debug_level = getenv("V");
  // Setup interface
  const uint16_t port = 0xAA55;           // UDP port
  const char *mac = "02:00:01:02:03:78";  // MAC address

	struct port_driver_data pdd;
  pdd.sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	pdd.port_addr.sin_family = AF_INET;
	pdd.port_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	pdd.port_addr.sin_port = htons(port);
	if (bind(pdd.sockfd, (struct sockaddr *) &pdd.port_addr, sizeof(pdd.port_addr)) < 0) return EXIT_FAILURE; 
  printf("Opened UDP socket\n");

	pdd.tap_addr.sin_family = AF_INET;
	pdd.tap_addr.sin_addr.s_addr =  htonl(0x7f000001U);  // 127.0.0.1
	pdd.tap_addr.sin_port = htons(0x55AA);
	if (connect(pdd.sockfd, (struct sockaddr *) &pdd.tap_addr, sizeof(pdd.tap_addr)) < 0) return EXIT_FAILURE; 
  printf("Connected to TAP UDP socket\n");

  usleep(200000);  // 200 ms

  if (debug_level == NULL) debug_level = "3";
  mg_log_set(atoi(debug_level));

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  // MIP driver
  struct mg_tcpip_driver driver;
  memset(&driver, 0, sizeof(driver));

  driver.tx = sock_tx;
  driver.poll = sock_poll;
  driver.rx = sock_rx;

  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));

  mif.driver = &driver;
  mif.driver_data = &pdd;

#if MIPPORTTEST_USING_DHCP == 1
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
#if MIPPORTTEST_USING_DHCP == 0
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
  result = mip_x_test(&mgr);
  close(pdd.sockfd);
  if (!result) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
