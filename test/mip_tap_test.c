#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_TCPIP_DRIVER_INIT 0

#define MIPTAPTEST_USING_DHCP 1

#define FETCH_BUF_SIZE (16 * 1024)

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

#ifdef MQTT_LOCALHOST
#define MQTT_URL "mqtt://127.0.0.1:1883"
#else
#define MQTT_URL "mqtt://broker.hivemq.com:1883"
#endif
#if MG_TLS == MG_TLS_BUILTIN
#define MQTTS_URL "mqtts://mongoose.ws:8883"  // test requires TLS 1.3
#define MQTTS_CA mg_str(s_ca_cert)
static const char *s_ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "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\n"
    "-----END CERTIFICATE-----\n";
#elif MG_TLS
#ifdef MQTT_LOCALHOST
// we'll generate MQTTS_URL
#define MQTTS_CA mg_str(s_ca_cert)
static const char *s_ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIBFTCBvAIJAMNTFtpfcq8NMAoGCCqGSM49BAMCMBMxETAPBgNVBAMMCE1vbmdv\n"
    "b3NlMB4XDTI0MDUwNzE0MzczNloXDTM0MDUwNTE0MzczNlowEzERMA8GA1UEAwwI\n"
    "TW9uZ29vc2UwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASuP+86T/rOWnGpEVhl\n"
    "fxYZ+pjMbCmDZ+vdnP0rjoxudwRMRQCv5slRlDK7Lxue761sdvqxWr0Ma6TFGTNg\n"
    "epsRMAoGCCqGSM49BAMCA0gAMEUCIQCwb2CxuAKm51s81S6BIoy1IcandXSohnqs\n"
    "us64BAA7QgIgGGtUrpkgFSS0oPBlCUG6YPHFVw42vTfpTC0ySwAS0M4=\n"
    "-----END CERTIFICATE-----\n";
#else
#define MQTTS_URL "mqtts://broker.hivemq.com:8883"
#define MQTTS_CA mg_unpacked("/data/ca.pem")
#endif // MQTT_LOCALHOST
#endif

static char *host_ip;

static int s_num_tests = 0;

#ifdef NO_SLEEP_ABORT
#define ABORT() abort()
#else
#define ABORT()                       \
  sleep(2); /* 2s, GH print reason */ \
  abort();
#endif

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      fflush(stdout);                                           \
      ABORT();                                                  \
    }                                                           \
  } while (0)

static struct mg_http_message gethm(const char *buf) {
  struct mg_http_message hm;
  memset(&hm, 0, sizeof(hm));
  mg_http_parse(buf, strlen(buf), &hm);
  return hm;
}

static int cmpbody(const char *buf, const char *str) {
  struct mg_str s = mg_str(str);
  struct mg_http_message hm = gethm(buf);
  size_t len = strlen(buf);
  if (hm.body.len > len) hm.body.len = len - (size_t) (hm.body.buf - buf);
  return mg_strcmp(hm.body, s);
}

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

static bool tap_poll(struct mg_tcpip_if *ifp, bool s1) {
  return s1 && ifp->driver_data ? true : false;
}

static void eh1(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_tls_opts *topts = (struct mg_tls_opts *) c->fn_data;
  if (ev == MG_EV_ACCEPT && topts != NULL) mg_tls_init(c, topts);
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_DEBUG(("[%.*s %.*s] message len %d", (int) hm->method.len,
              hm->method.buf, (int) hm->uri.len, hm->uri.buf,
              (int) hm->message.len));
    if (mg_match(hm->uri, mg_str("/foo/*"), NULL)) {
      mg_http_reply(c, 200, "", "uri: %.*s", hm->uri.len - 5, hm->uri.buf + 5);
    } else if (mg_match(hm->uri, mg_str("/ws"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/body"), NULL)) {
      mg_http_reply(c, 200, "", "%.*s", (int) hm->body.len, hm->body.buf);
    } else {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = "./data";
      mg_http_serve_dir(c, hm, &sopts);
    }
  } else if (ev == MG_EV_WS_OPEN) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ASSERT(mg_strcmp(hm->uri, mg_str("/ws")) == 0);
    mg_ws_send(c, "opened", 6, WEBSOCKET_OP_BINARY);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_BINARY);
  }
}
struct fetch_data {
  char *buf;
  const char *url;
  int code, closed;
};

static void fcb(struct mg_connection *c, int ev, void *ev_data) {
  struct fetch_data *fd = (struct fetch_data *) c->fn_data;
  if (ev == MG_EV_CONNECT) {
    MG_DEBUG(("CONNECT"));
    if (mg_url_is_ssl(fd->url)) {
      struct mg_tls_opts opts;
      memset(&opts, 0, sizeof(opts));  // read CA from packed_fs
      if (host_ip != NULL && strstr(fd->url, host_ip) != NULL) {
        MG_DEBUG(("Local connection, using self-signed certificates"));
        opts.name = mg_str_s("localhost");
        opts.ca = mg_unpacked("/certs/ca.crt");
      } else {
        opts.name = mg_url_host(fd->url);
        opts.ca = mg_unpacked("/data/ca.pem");
#if MG_TLS == MG_TLS_BUILTIN
        // our TLS does not search for the proper CA in a bundle
        opts.ca = mg_file_read(&mg_fs_posix, "data/e5.crt");
#endif
      }
      mg_tls_init(c, &opts);
#if MG_TLS == MG_TLS_BUILTIN
      mg_free((void *) opts.ca.buf);
#endif
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    snprintf(fd->buf, FETCH_BUF_SIZE, "%.*s", (int) hm->message.len,
             hm->message.buf);
    fd->code = atoi(hm->uri.buf);
    fd->closed = 1;
    c->is_closing = 1;
    MG_DEBUG(("CODE: %d, MSG: %.*s", fd->code, (int) hm->message.len,
              hm->message.buf));
    (void) c;
  } else if (ev == MG_EV_CLOSE) {
    MG_DEBUG(("CLOSE"));
    fd->closed = 1;
  } else if (ev == MG_EV_READ) {
    long bytes = *(long *) ev_data;
    MG_DEBUG(("READ %d: %.*s", (int) bytes, (int) bytes, c->recv.buf));
  }
}

static int fetch(struct mg_mgr *mgr, char *buf, const char *url,
                 const char *fmt, ...) {
  struct fetch_data fd = {buf, url, 0, 0};
  int i;
  struct mg_connection *c = NULL;
  va_list ap;
  mg_mgr_poll(mgr, 0);  // update ifp->now to avoid ARP lookup using an old
                        // timestamp (from an ancient call in other test)
  c = mg_http_connect(mgr, url, fcb, &fd);
  ASSERT(c != NULL);
  va_start(ap, fmt);
  mg_vprintf(c, fmt, &ap);
  va_end(ap);
  buf[0] = '\0';
  // - TLS: multiple (small) records: allow enough loops so mg_mgr_poll can
  // process buffered records when no more frames are coming in
  for (i = 0; i < 500 && buf[0] == '\0' && !fd.closed; i++) {
    mg_mgr_poll(mgr, 0);
    usleep(5000);  // 5 ms. Slow down poll loop to ensure packet transit, but
                   // allow enough loops to get the ARP response, otherwise,
                   // given enough traffic, the timer expires before we get a
                   // chance to see the response
  }
  if (!fd.closed) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
  return fd.code;
}

static void test_http_client(struct mg_mgr *mgr) {
  char buf[FETCH_BUF_SIZE];
  int rc = 0;
  const bool ipv6 = 0;
#if MG_TLS
  if (ipv6) {
    rc = fetch(mgr, buf, "https://ipv6.google.com",
               "GET / HTTP/1.0\r\nHost: ipv6.google.com\r\n\r\n");
  } else {
    rc = fetch(mgr, buf, "https://cesanta.com",
               "GET /robots.txt HTTP/1.0\r\nHost: cesanta.com\r\n\r\n");
  }
  ASSERT(rc == 200);  // OK
#else
  if (ipv6) {
    rc = fetch(mgr, buf, "http://ipv6.google.com",
               "GET / HTTP/1.0\r\nHost: ipv6.google.com\r\n\r\n");
  } else {
    rc = fetch(mgr, buf, "http://cesanta.com",
               "GET /robots.txt HTTP/1.0\r\nHost: cesanta.com\r\n\r\n");
  }
  ASSERT(rc == 301);  // OK: Permanently moved (HTTP->HTTPS redirect)

#endif
}

static struct mg_connection *s_conn;
static char s_topic[16];

struct mqtt_data {
  char *url;
  bool passed;
};

static void mqtt_fn(struct mg_connection *c, int ev, void *ev_data) {
  struct mqtt_data *data = (struct mqtt_data *) c->fn_data;
  if (ev == MG_EV_CONNECT) {
    MG_DEBUG(("CONNECT"));
#if MG_TLS
    struct mg_tls_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.ca = MQTTS_CA;
    opts.name = mg_url_host(data->url);
    mg_tls_init(c, &opts);
#endif
  } else if (ev == MG_EV_MQTT_OPEN) {
    MG_DEBUG(("MQTT CONNECT"));
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
    MG_DEBUG(("TOPIC: %.*s, MSG: %.*s", (int) mm->topic.len, mm->topic.buf,
              mm->data.len > 10 ? 10 : (int) mm->data.len, mm->data.buf));
    ASSERT(mm->topic.len == strlen(s_topic) &&
           strncmp(mm->topic.buf, s_topic, mm->topic.len) == 0);
    if (mm->data.len == 2 && strncmp(mm->data.buf, "hi", 2) == 0) {
      struct mg_mqtt_opts pub_opts;
      memset(&pub_opts, 0, sizeof(pub_opts));
      pub_opts.topic = mm->topic;
      // send more than 1 record, content is not relevant
      pub_opts.message = mg_str_n((char *)(size_t) mqtt_fn, 21098);
      pub_opts.qos = 1, pub_opts.retain = false;
      mg_mqtt_pub(c, &pub_opts);
    } else if (mm->data.len == 8 && strncmp(mm->data.buf, "farewell", 8) == 0) {
      // close on farewell
      MG_INFO(("%lu CLOSING", c->id));
      mg_mqtt_disconnect(c, NULL);
      data->passed = true;
    } else if (mm->data.len == 21098) {
      struct mg_mqtt_opts pub_opts;
      ASSERT(memcmp((const char *) (size_t) mqtt_fn, mm->data.buf, 21098) == 0);
      // send farewell after receiving big data
      memset(&pub_opts, 0, sizeof(pub_opts));
      pub_opts.topic = mm->topic;
      pub_opts.message = mg_str("farewell");
      pub_opts.qos = 1, pub_opts.retain = false;
      mg_mqtt_pub(c, &pub_opts);
    }
  } else if (ev == MG_EV_CLOSE) {
    MG_DEBUG(("CLOSE"));
    s_conn = NULL;
  } else if (ev == MG_EV_ERROR) {
    MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
  }
}

static void test_mqtt_connsubpub(struct mg_mgr *mgr) {
  struct mqtt_data data;
  struct mg_mqtt_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.clean = true, opts.version = 4;
  data.passed = false;
#if defined(MQTT_LOCALHOST) && MG_TLS != MG_TLS_BUILTIN
  if (host_ip == NULL) {
    printf("\nMQTT_LOCALHOST defined but no HOST_IP provided, skipping MQTTS tests\n");
    return;
  }
  printf("HOST_IP: %s\n", host_ip);
#endif
#if MG_TLS
#if defined(MQTT_LOCALHOST) && MG_TLS != MG_TLS_BUILTIN
  data.url = mg_mprintf("mqtts://%s:8883", host_ip);
#else
  data.url = strdup(MQTTS_URL);
#endif
#else
#ifdef MQTT_LOCALHOST
  data.url = mg_mprintf("mqtt://%s:1883", host_ip);
#else
  data.url = strdup(MQTT_URL);
#endif
#endif
  s_conn = mg_mqtt_connect(mgr, data.url, &opts, mqtt_fn, &data);
  ASSERT(s_conn != NULL);
  for (int i = 0; i < 1000 && s_conn != NULL && !s_conn->is_closing; i++) {
    mg_mgr_poll(mgr, 0);
    usleep(10000);  // 10 ms. Slow down poll loop to ensure packets transit
  }
  ASSERT(data.passed);
  mg_mgr_poll(mgr, 0);
  free(data.url);
}

#include <pthread.h>
static void *poll_thread(void *p) {
  struct mg_mgr *mgr = (struct mg_mgr *) p;
  int i;
  for (i = 0; i < 300; i++) {
    mg_mgr_poll(mgr, 0);
    usleep(10000);  // 10 ms. Slow down poll loop to ensure packet transit
  }
  return NULL;
}

static void test_http_server(struct mg_mgr *mgr) {
  struct mg_connection *c;
  char *cmd;
  pthread_t thread_id = (pthread_t) 0;
#if MG_TLS
  struct mg_tls_opts opts;
  memset(&opts, 0, sizeof(opts));
  // opts.ca = mg_str(s_tls_ca);
  opts.cert = mg_unpacked("/certs/server.crt");
  opts.key = mg_unpacked("/certs/server.key");
  c = mg_http_listen(mgr, "https://0.0.0.0:12347", eh1, &opts);
  cmd = mg_mprintf("./mip_curl.sh --insecure https://%M:12347", mg_print_ip4,
                   &mgr->ifp->ip);
#else
  c = mg_http_listen(mgr, "http://0.0.0.0:12347", eh1, NULL);
  cmd =
      mg_mprintf("./mip_curl.sh http://%M:12347", mg_print_ip4, &mgr->ifp->ip);
#endif
  ASSERT(c != NULL);
  pthread_create(&thread_id, NULL, poll_thread,
                 mgr);  // simpler this way, no concurrency anyway
  MG_DEBUG(("CURL"));
  ASSERT(system(cmd) == 0);  // wait for curl
  MG_DEBUG(("MONGOOSE"));
  pthread_join(thread_id, NULL);  // wait for Mongoose
  MG_DEBUG(("DONE"));
  free(cmd);
}

static void test_tls(struct mg_mgr *mgr) {
#if MG_TLS
  char *url;
  char buf[FETCH_BUF_SIZE];  // make sure it can hold Makefile
  struct mg_str data = mg_unpacked("/Makefile");
  if (host_ip == NULL) {
    printf("\nNo HOST_IP provided, skipping TLS tests\n");
    return;
  }
  printf("HOST_IP: %s\n", host_ip);
  // - POST a large file, make sure we drain TLS buffers and read all: done at
  // server test, using curl as POSTing client
  // - Fire patched server, test multiple TLS records per TCP segment handling
  url = mg_mprintf("https://%s:8443", host_ip);  // for historic reasons
  ASSERT(system("tls_multirec/server -d tls_multirec &") == 0);
  sleep(1);
  ASSERT(fetch(mgr, buf, url, "GET /thefile HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, data.buf) == 0);  // "thefile" links to Makefile
  ASSERT(system("killall tls_multirec/server") == 0);
  free(url);
#else
  (void) cmpbody("", "");
  (void) mgr;
#endif
}

int main(void) {
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

  host_ip = getenv("HOST_IP");

  // Events
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
  usleep(500000);  // 500 ms
  test_http_client(&mgr);
  usleep(500000);  // 500 ms
  test_http_server(&mgr);
  usleep(500000);  // 500 ms
  test_tls(&mgr);
  usleep(500000);  // 500 ms
  test_mqtt_connsubpub(&mgr);
  usleep(500000);  // 500 ms

  printf("SUCCESS. Total tests: %d\n", s_num_tests);

  // Clear
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);  // Deconstruction OK
  close(fd);
  return 0;
}
