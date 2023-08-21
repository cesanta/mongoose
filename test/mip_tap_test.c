#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_SOCKET 0
#define MG_USING_DHCP 1
#define MG_ENABLE_PACKED_FS 0
#define MG_ENABLE_LINES 1

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

// MIP TUNTAP driver
static size_t tap_rx(void *buf, size_t len, struct mg_tcpip_if *ifp) {
  ssize_t received = read(*(int *) ifp->driver_data, buf, len);
  usleep(1);  // This is to avoid 100% CPU
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

static bool tap_up(struct mg_tcpip_if *ifp) {
  return ifp->driver_data ? true : false;
}

// HTTP fetches IOs
struct Post_reply {
  char *post;                            // HTTP POST data
  void *http_response;                   // Server response(s)
  unsigned int http_responses_received;  // Number responses received
};

char *fetch(struct mg_mgr *mgr, const char *url, const char *post_data);
static void f_http_fetch_query(struct mg_connection *c, int ev, void *ev_data,
                               void *fn_data);
int get_response_code(char *);  // Returns HTTP status code from full char* msg

static void f_http_fetch_query(struct mg_connection *c, int ev, void *ev_data,
                               void *fn_data) {
  static char *http_response = 0;
  static bool http_response_allocated =
      0;  // So that we will update out parameter
  unsigned int http_responses_received = 0;
  struct Post_reply *post_reply_l;
  post_reply_l = (struct Post_reply *) fn_data;

  if (ev == MG_EV_CONNECT) {
    mg_printf(c, post_reply_l->post);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    http_responses_received++;
    if (!http_response_allocated) {
      http_response = (char *) mg_strdup(hm->message).ptr;
      http_response_allocated = 1;
    }
    if (http_responses_received > 0) {
      post_reply_l->http_response = http_response;
      post_reply_l->http_responses_received = http_responses_received;
    }
  }
}

// Fetch utility returns message from fetch(..., URL, POST)
char *fetch(struct mg_mgr *mgr, const char *url, const char *fn_data) {
  struct Post_reply post_reply;
  {
    post_reply.post = (char *) fn_data;
    post_reply.http_response = 0;
    post_reply.http_responses_received = 0;
  }
  struct mg_connection *conn;
  conn = mg_http_connect(mgr, url, f_http_fetch_query, &post_reply);
  ASSERT(conn != NULL);  // Assertion on initialisation
  for (int i = 0; i < 500 && !post_reply.http_responses_received; i++) {
    mg_mgr_poll(mgr, 100);
    usleep(10000);  // 10 ms. Slow down poll loop to ensure packets transit
  }

  if (mgr->conns != 0) {
    conn->is_closing = 1;
    mg_mgr_poll(mgr, 0);
  }

  mg_mgr_poll(mgr, 0);
  if (!post_reply.http_responses_received) {
    return 0;
  } else {
    return (char *) post_reply.http_response;
  }
}

// Returns server's HTTP response code
int get_response_code(char *http_msg_raw) {
  int http_status = 0;
  struct mg_http_message http_msg_parsed;
  if (mg_http_parse(http_msg_raw, strlen(http_msg_raw), &http_msg_parsed)) {
    http_status = mg_http_status(&http_msg_parsed);
  } else {
    printf("Error: mg_http_parse()\n");
    ASSERT(http_status != 0);  // Couldn't parse.
  }
  return http_status;
}

static void test_http_fetch(struct mg_mgr *mgr) {
  char *http_feedback = (char *) "";
  const bool ipv6 = 0;
  if (ipv6) {
    http_feedback = fetch(mgr, "ipv6.google.com",
                          "GET/ HTTP/1.0\r\nHost: ipv6.google.com\r\n\r\n");
  } else {
    http_feedback =
        fetch(mgr, "http://cesanta.com",
              "GET //robots.txt HTTP/1.0\r\nHost: cesanta.com\r\n\r\n");
  }

  ASSERT(http_feedback != NULL &&
         *http_feedback != '\0');  // HTTP response received ?

  int http_status = get_response_code(http_feedback);
  // printf("Server response HTTP status code: %d\n",http_status);
  ASSERT(http_status != 0);
  ASSERT(http_status == 301);  // OK: Permanently moved (HTTP->HTTPS redirect)

  if (http_feedback) {
    free(http_feedback);
    http_feedback = 0;
  }
}

static struct mg_connection *s_conn;
static char s_topic[16];

static void mqtt_fn(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_MQTT_OPEN) {
    // MQTT connect is successful
    struct mg_mqtt_opts sub_opts;
    memset(&sub_opts, 0, sizeof(sub_opts));
    sub_opts.topic = mg_str(mg_random_str(s_topic, sizeof(s_topic)));
    sub_opts.qos = 1;
    mg_mqtt_sub(c, &sub_opts);
    struct mg_mqtt_opts pub_opts;
    memset(&pub_opts, 0, sizeof(pub_opts));
    pub_opts.topic = sub_opts.topic;
    pub_opts.message = mg_str("hi");
    pub_opts.qos = 1, pub_opts.retain = false;
    mg_mqtt_pub(c, &pub_opts);
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->topic.len != strlen(s_topic) || strcmp(mm->topic.ptr, s_topic))
      ASSERT(0);
    if (mm->data.len != 2 || strcmp(mm->data.ptr, "hi")) ASSERT(0);
    mg_mqtt_disconnect(c, NULL);
    *(bool *) fn_data = true;
  } else if (ev == MG_EV_CLOSE) {
    s_conn = NULL;
  }
}

static void test_mqtt_connsubpub(struct mg_mgr *mgr) {
  const char *url = "mqtt://broker.hivemq.com:1883";
  bool passed = false;
  struct mg_mqtt_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.clean = true, opts.version = 4;
  s_conn = mg_mqtt_connect(mgr, url, &opts, mqtt_fn, &passed);
  ASSERT(s_conn != NULL);
  for (int i = 0; i < 500 && s_conn != NULL && !s_conn->is_closing; i++) {
    mg_mgr_poll(mgr, 0);
    usleep(10000);  // 10 ms. Slow down poll loop to ensure packets transit
  }
  ASSERT(passed);
  mg_mgr_poll(mgr, 0);
}

int main(void) {
  // Setup interface
  const char *iface = "tap0";             // Network iface
  const char *mac = "00:00:01:02:03:78";  // MAC address
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

  // Events
  struct mg_mgr mgr;  // Event manager
  mg_mgr_init(&mgr);  // Initialise event manager

  // MIP driver
  struct mg_tcpip_driver driver;
  memset(&driver, 0, sizeof(driver));

  driver.tx = tap_tx;
  driver.up = tap_up;
  driver.rx = tap_rx;

  struct mg_tcpip_if mif;
  memset(&mif, 0, sizeof(mif));

  mif.driver = &driver;
  mif.driver_data = &fd;

#if MG_USING_DHCP == 1
#else
  mif.ip = mg_htonl(MG_U32(192, 168, 32, 2));  // Triggering a network failure
  mif.mask = mg_htonl(MG_U32(255, 255, 255, 0));
  mif.gw = mg_htonl(MG_U32(192, 168, 32, 1));
#endif

  sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &mif.mac[0], &mif.mac[1],
         &mif.mac[2], &mif.mac[3], &mif.mac[4], &mif.mac[5]);

  mg_tcpip_init(&mgr, &mif);
  MG_INFO(("Init done, starting main loop"));

  // Stack initialization, Network configuration (DHCP lease, ...)
#if MG_USING_DHCP == 0
  MG_INFO(("MIF configuration: Static IP"));
  ASSERT(mif.ip != 0);     // Check we have a satic IP assigned
  mg_mgr_poll(&mgr, 100);  // For initialisation
#else
  MG_INFO(("MIF configuration: DHCP"));
  ASSERT(!mif.ip);  // Check we are set for DHCP
  int pc = 500;     // Timout on DHCP lease 500 ~ approx 5s (typical delay <1s)
  while (((pc--) > 0) && !mif.ip) {
    mg_mgr_poll(&mgr, 100);
    usleep(10000);  // 10 ms
  }
  if (!mif.ip) MG_ERROR(("No ip assigned (DHCP lease may have failed).\n"));
  ASSERT(mif.ip);  // We have an IP (lease or static)
#endif

  // RUN TESTS
  test_http_fetch(&mgr);
  test_mqtt_connsubpub(&mgr);
  printf("SUCCESS. Total tests: %d\n", s_num_tests);

  // Clear
  mg_mgr_free(&mgr);
  mg_tcpip_free(&mif);        // Release after mg_mgr
  ASSERT(mgr.conns == NULL);  // Deconstruction OK
  close(fd);
  return 0;
}
