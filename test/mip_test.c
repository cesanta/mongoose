#define MG_ENABLE_SOCKET 0
#define MG_ENABLE_LINES 1
#define MG_ENABLE_MIP 1
#define MG_ENABLE_PACKED_FS 0

#include <assert.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>
#include "mongoose.c"

#include "driver_mock.c"

static int s_num_tests = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

// *****************************************************************************

static void test_queue(void) {
  static uint8_t
      buf[sizeof(size_t) + sizeof(uint16_t) + 3];  // fit 1 element but not 2
  uint16_t val = 1234;
  static struct queue q = {buf, sizeof(buf), 0, 0};

  // Write to an empty queue, and read back
  assert(q_avail(&q) == 0);
  assert(q_write(&q, &val, sizeof(val)) == true);
  assert(q_avail(&q) == sizeof(val));
  assert(q.head > q.tail);
  // Only one element may fit
  assert(q_write(&q, &val, sizeof(val)) == false);
  val = 0;
  assert(q_read(&q, &val) == sizeof(val));
  assert(val == 1234);
  assert(q_avail(&q) == 0);

  // Second write - wrap over the buffer boundary
  assert(q_write(&q, &val, sizeof(val)) == true);
  assert(q_avail(&q) == sizeof(val));
  assert(q.head < q.tail);
  // Only one element may fit
  assert(q_write(&q, &val, sizeof(val)) == false);
  val = 0;
  assert(q_read(&q, &val) == sizeof(val));
  assert(val == 1234);
  assert(q_avail(&q) == 0);
}

// *****************************************************************************

static void test_statechange(void) {
  uint8_t tx[1540];
  struct mip_if iface;
  memset(&iface, 0, sizeof(iface));
  iface.ip = mg_htonl(0x01020304);
  iface.state = MIP_STATE_READY;
  iface.tx.buf = tx, iface.tx.len = sizeof(tx);
  iface.driver = &mip_driver_mock;
  onstatechange(&iface);
}

// *****************************************************************************
// Implement tap_rx() tap_tx() tap_up()

static size_t tap_rx(void *buf, size_t len, void *userdata) {
  ssize_t received = read((int) userdata, buf, len); // Original
  usleep(1);  // This is to avoid 100% CPU
  if (received < 0) return 0;
  return (size_t) received;
}

static size_t tap_tx(const void *buf, size_t len, void *userdata) {
  ssize_t res = write((int) userdata, buf, len); // Original
  if (res < 0) {
    MG_ERROR(("tap_tx failed: %d", errno));
    return 0;
  }
  return (size_t) res;
}

static bool tap_up(void *userdata) {
  return userdata ? true : false;
}

// *****************************************************************************
// HTTP fetch

static void f_http_fetch(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int *fn_data_int_ptr = (int *) fn_data;
  // MG_INFO(("%d", ev));
  if (ev == MG_EV_CONNECT) {
    // c->is_hexdumping = 1;
    mg_printf(c, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",
              c->rem.is_ip6 ? "" : "/robots.txt",
              c->rem.is_ip6 ? "ipv6.google.com" : "cesanta.com");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // MG_INFO(("-->[%.*s]", (int) hm->message.len, hm->message.ptr));
    // ASSERT(mg_vcmp(&hm->method, "HTTP/1.1") == 0);
    // ASSERT(mg_vcmp(&hm->uri, "301") == 0);
    *fn_data_int_ptr = mg_http_status(hm);
  } else if (ev == MG_EV_CLOSE) {
    if (*fn_data_int_ptr == 0) *fn_data_int_ptr = 888;
  } else if (ev == MG_EV_ERROR) {
    if (*fn_data_int_ptr == 0) *fn_data_int_ptr = 777;
  }
}

static void test_http_fetch(void) {
  const char *iface = "tap0";             // Network iface
  const char *mac = "00:00:01:02:03:78";  // MAC address
  int fd = open("/dev/net/tun", O_RDWR);  // Open network interface
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, iface, IFNAMSIZ);
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, (void *) &ifr) < 0) {
    MG_ERROR(("Failed to setup TAP interface: %s", ifr.ifr_name));
    abort(); // return EXIT_FAILURE;
  }
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode

  MG_INFO(("Opened TAP interface: %s", iface));

  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  struct mip_cfg c = {.ip = 0, .mask = 0, .gw = 0};
  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &c.mac[0], &c.mac[1], &c.mac[2],
         &c.mac[3], &c.mac[4], &c.mac[5]);

  struct mip_driver driver = {.tx = tap_tx, .up = tap_up, .rx = tap_rx};
  mip_init(&mgr, &c, &driver, (void *) fd);
  MG_INFO(("Init done, starting main loop"));

  int ok = 0;
  struct mg_connection *conn;

  int pc = 100; // Wait sufficiently to acquire HTTP lease
  while ((pc--)>0) {
    mg_mgr_poll(&mgr, 100);
    usleep(10000); // 10 ms
  }

  conn = mg_http_connect(&mgr, "http://cesanta.com", f_http_fetch, &ok);
  ASSERT(conn != NULL);		// Assertion on initialisation
  for (int i = 0; i < 500 && ok <= 0; i++) {
    mg_mgr_poll(&mgr, 100);
    usleep(10000); // 10 ms. Slow down poll loop to ensure packets transit
  }
  ASSERT(ok == 301);		  // Assertion on DNS lookup
  conn->is_closing = 1;
  mg_mgr_poll(&mgr, 0);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL); // Assertion on deconstruction
}

// *****************************************************************************

int main(void) {
  test_queue();
  test_statechange();
  test_http_fetch();
  printf("SUCCESS\n");
  return 0;
}
