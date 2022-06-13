#include "float.h"  // For DBL_EPSILON and HUGE_VAL
#include "math.h"
#include "mongoose.h"

static int s_num_tests = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      abort();                                                  \
    }                                                           \
  } while (0)

#define FETCH_BUF_SIZE (256 * 1024)

// Important: we use different port numbers for the Windows bug workaround. See
// https://support.microsoft.com/en-ae/help/3039044/error-10013-wsaeacces-is-returned-when-a-second-bind-to-a-excluded-por

static void test_globmatch(void) {
  ASSERT(mg_globmatch("", 0, "", 0) == 1);
  ASSERT(mg_globmatch("*", 1, "a", 1) == 1);
  ASSERT(mg_globmatch("*", 1, "ab", 2) == 1);
  ASSERT(mg_globmatch("", 0, "a", 1) == 0);
  ASSERT(mg_globmatch("/", 1, "/foo", 4) == 0);
  ASSERT(mg_globmatch("/*/foo", 6, "/x/bar", 6) == 0);
  ASSERT(mg_globmatch("/*/foo", 6, "/x/foo", 6) == 1);
  ASSERT(mg_globmatch("/*/foo", 6, "/x/foox", 7) == 0);
  ASSERT(mg_globmatch("/*/foo*", 7, "/x/foox", 7) == 1);
  ASSERT(mg_globmatch("/*", 2, "/abc", 4) == 1);
  ASSERT(mg_globmatch("/*", 2, "/ab/", 4) == 0);
  ASSERT(mg_globmatch("/*", 2, "/", 1) == 1);
  ASSERT(mg_globmatch("/x/*", 4, "/x/2", 4) == 1);
  ASSERT(mg_globmatch("/x/*", 4, "/x/2/foo", 8) == 0);
  ASSERT(mg_globmatch("/x/*/*", 6, "/x/2/foo", 8) == 1);
  ASSERT(mg_globmatch("#", 1, "///", 3) == 1);
  ASSERT(mg_globmatch("/api/*", 6, "/api/foo", 8) == 1);
  ASSERT(mg_globmatch("/api/*", 6, "/api/log/static", 15) == 0);
  ASSERT(mg_globmatch("/api/#", 6, "/api/log/static", 15) == 1);
  ASSERT(mg_globmatch("#.shtml", 7, "/ssi/index.shtml", 16) == 1);
  ASSERT(mg_globmatch("#.c", 3, ".c", 2) == 1);
  ASSERT(mg_globmatch("abc", 3, "ab", 2) == 0);
  ASSERT(mg_globmatch("#.c", 3, "a.c", 3) == 1);
  ASSERT(mg_globmatch("#.c", 3, "..c", 3) == 1);
  ASSERT(mg_globmatch("#.c", 3, "/.c", 3) == 1);
  ASSERT(mg_globmatch("#.c", 3, "//a.c", 5) == 1);
  ASSERT(mg_globmatch("#.c", 3, "x/a.c", 5) == 1);
  ASSERT(mg_globmatch("#.c", 3, "./a.c", 5) == 1);
  ASSERT(mg_globmatch("#.shtml", 7, "./ssi/index.shtml", 17) == 1);
  ASSERT(mg_globmatch("#aa#bb#", 7, "caabba", 6) == 1);
  ASSERT(mg_globmatch("#aa#bb#", 7, "caabxa", 6) == 0);
  ASSERT(mg_globmatch("a*b*c", 5, "a__b_c", 6) == 1);

  {
    struct mg_str caps[3];
    ASSERT(mg_match(mg_str("//a.c"), mg_str("#.c"), NULL) == true);
    ASSERT(mg_match(mg_str("a"), mg_str("#"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("a")) == 0);
    ASSERT(mg_match(mg_str("//a.c"), mg_str("#.c"), caps) == true);
    ASSERT(mg_match(mg_str("a_b_c_"), mg_str("a*b*c"), caps) == false);
    ASSERT(mg_match(mg_str("a__b_c"), mg_str("a*b*c"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("__")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("_")) == 0);
    ASSERT(mg_match(mg_str("a_b_c__c"), mg_str("a*b*c"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("_")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("_c__")) == 0);
    ASSERT(mg_match(mg_str("a_xb_.c__c"), mg_str("a*b*c"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("_x")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("_.c__")) == 0);
    ASSERT(mg_match(mg_str("a"), mg_str("#a"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("")) == 0);

    ASSERT(mg_match(mg_str(".aa..b...b"), mg_str("#a#b"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str(".")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("a..b...")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);

    ASSERT(mg_match(mg_str("/foo/bar"), mg_str("/*/*"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("foo")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("bar")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);

    ASSERT(mg_match(mg_str("/foo/"), mg_str("/*/*"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("foo")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);

    ASSERT(mg_match(mg_str("abc"), mg_str("?#"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("a")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("bc")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);
  }
}

static void test_commalist(void) {
  struct mg_str k, v, s1 = mg_str(""), s2 = mg_str("a"), s3 = mg_str("a,b");
  struct mg_str s4 = mg_str("a=123"), s5 = mg_str("a,b=123");
  ASSERT(mg_commalist(&s1, &k, &v) == false);

  ASSERT(mg_commalist(&s2, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_commalist(&s2, &k, &v) == false);

  ASSERT(mg_commalist(&s3, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_commalist(&s3, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "b") == 0);
  ASSERT(mg_commalist(&s3, &k, &v) == false);

  ASSERT(mg_commalist(&s4, &k, &v) == true);
  ASSERT(mg_vcmp(&k, "a") == 0 && mg_vcmp(&v, "123") == 0);
  ASSERT(mg_commalist(&s4, &k, &v) == false);
  ASSERT(mg_commalist(&s4, &k, &v) == false);

  ASSERT(mg_commalist(&s5, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_commalist(&s5, &k, &v) == true);
  ASSERT(mg_vcmp(&k, "b") == 0 && mg_vcmp(&v, "123") == 0);
  ASSERT(mg_commalist(&s4, &k, &v) == false);
}

static void test_http_get_var(void) {
  char buf[256];
  struct mg_str body;
  body = mg_str("key1=value1&key2=value2&key3=value%203&key4=value+4");
  ASSERT(mg_http_get_var(&body, "key1", buf, sizeof(buf)) == 6);
  ASSERT(strcmp(buf, "value1") == 0);
  ASSERT(mg_http_get_var(&body, "KEY1", buf, sizeof(buf)) == 6);
  ASSERT(strcmp(buf, "value1") == 0);
  ASSERT(mg_http_get_var(&body, "key2", buf, sizeof(buf)) == 6);
  ASSERT(strcmp(buf, "value2") == 0);
  ASSERT(mg_http_get_var(&body, "key3", buf, sizeof(buf)) == 7);
  ASSERT(strcmp(buf, "value 3") == 0);
  ASSERT(mg_http_get_var(&body, "key4", buf, sizeof(buf)) == 7);
  ASSERT(strcmp(buf, "value 4") == 0);

  ASSERT(mg_http_get_var(&body, "key", buf, sizeof(buf)) == -4);
  ASSERT(mg_http_get_var(&body, "key1", NULL, sizeof(buf)) == -2);
  ASSERT(mg_http_get_var(&body, "key1", buf, 0) == -2);
  ASSERT(mg_http_get_var(&body, NULL, buf, sizeof(buf)) == -1);
  ASSERT(mg_http_get_var(&body, "key1", buf, 1) == -3);

  body = mg_str("key=broken%2");
  ASSERT(mg_http_get_var(&body, "key", buf, sizeof(buf)) == -3);

  body = mg_str("key=broken%2x");
  ASSERT(mg_http_get_var(&body, "key", buf, sizeof(buf)) == -3);
  ASSERT(mg_http_get_var(&body, "inexistent", buf, sizeof(buf)) == -4);
  body = mg_str("key=%");
  ASSERT(mg_http_get_var(&body, "key", buf, sizeof(buf)) == -3);
  body = mg_str("&&&kEy=%");
  ASSERT(mg_http_get_var(&body, "kEy", buf, sizeof(buf)) == -3);
}

static int vcmp(struct mg_str s1, const char *s2) {
  // MG_INFO(("->%.*s<->%s<- %d %d %d", (int) s1.len, s1.ptr, s2,
  //(int) s1.len, strncmp(s1.ptr, s2, s1.len), mg_vcmp(&s1, s2)));
  return mg_vcmp(&s1, s2) == 0;
}

static void test_url(void) {
  // Host
  ASSERT(vcmp(mg_url_host("foo"), "foo"));
  ASSERT(vcmp(mg_url_host("//foo"), "foo"));
  ASSERT(vcmp(mg_url_host("foo:1234"), "foo"));
  ASSERT(vcmp(mg_url_host(":1234"), ""));
  ASSERT(vcmp(mg_url_host("//foo:1234"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/x"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/x/"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/x//"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo//x"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo///x"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo///x//"), "foo"));
  ASSERT(vcmp(mg_url_host("p://bar:1234"), "bar"));
  ASSERT(vcmp(mg_url_host("p://bar:1234/"), "bar"));
  ASSERT(vcmp(mg_url_host("p://bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u@bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u:p@bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u:p@[::1]:1234/a"), "[::1]"));
  ASSERT(vcmp(mg_url_host("p://u:p@[1:2::3]:1234/a"), "[1:2::3]"));
  ASSERT(vcmp(mg_url_host("p://foo/x:y/z"), "foo"));

  // Port
  ASSERT(mg_url_port("foo:1234") == 1234);
  ASSERT(mg_url_port(":1234") == 1234);
  ASSERT(mg_url_port("x://foo:1234") == 1234);
  ASSERT(mg_url_port("x://foo:1234/") == 1234);
  ASSERT(mg_url_port("x://foo:1234/xx") == 1234);
  ASSERT(mg_url_port("x://foo:1234") == 1234);
  ASSERT(mg_url_port("p://bar:1234/a") == 1234);
  ASSERT(mg_url_port("p://bar:1234/a:b") == 1234);
  ASSERT(mg_url_port("http://bar") == 80);
  ASSERT(mg_url_port("http://localhost:1234") == 1234);
  ASSERT(mg_url_port("https://bar") == 443);
  ASSERT(mg_url_port("wss://bar") == 443);
  ASSERT(mg_url_port("wss://u:p@bar") == 443);
  ASSERT(mg_url_port("wss://u:p@bar:123") == 123);
  ASSERT(mg_url_port("wss://u:p@bar:123/") == 123);
  ASSERT(mg_url_port("wss://u:p@bar:123/abc") == 123);
  ASSERT(mg_url_port("http://u:p@[::1]/abc") == 80);
  ASSERT(mg_url_port("http://u:p@[::1]:2121/abc") == 2121);
  ASSERT(mg_url_port("http://u:p@[::1]:2121/abc/cd:ef") == 2121);

  // User / pass
  ASSERT(vcmp(mg_url_user("p://foo"), ""));
  ASSERT(vcmp(mg_url_pass("p://foo"), ""));
  ASSERT(vcmp(mg_url_user("p://:@foo"), ""));
  ASSERT(vcmp(mg_url_pass("p://:@foo"), ""));
  ASSERT(vcmp(mg_url_user("p://u@foo"), "u"));
  ASSERT(vcmp(mg_url_pass("p://u@foo"), ""));
  ASSERT(vcmp(mg_url_user("p://u:@foo"), "u"));
  ASSERT(vcmp(mg_url_pass("p://u:@foo"), ""));
  ASSERT(vcmp(mg_url_user("p://:p@foo"), ""));
  ASSERT(vcmp(mg_url_pass("p://:p@foo"), "p"));
  ASSERT(vcmp(mg_url_user("p://u:p@foo"), "u"));
  ASSERT(vcmp(mg_url_pass("p://u:p@foo"), "p"));
  ASSERT(vcmp(mg_url_pass("p://u:p@foo//a@b"), "p"));

  // URI
  ASSERT(strcmp(mg_url_uri("p://foo"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo/"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/abc"), "/abc") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/a/b/c"), "/a/b/c") == 0);
  ASSERT(strcmp(mg_url_uri("p://[::1]:12/a/b/c"), "/a/b/c") == 0);
  ASSERT(strcmp(mg_url_uri("p://[ab::1]:12/a/b/c"), "/a/b/c") == 0);
}

static void test_base64(void) {
  char buf[128];

  ASSERT(mg_base64_encode((uint8_t *) "", 0, buf) == 0);
  ASSERT(strcmp(buf, "") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "x", 1, buf) == 4);
  ASSERT(strcmp(buf, "eA==") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "xyz", 3, buf) == 4);
  ASSERT(strcmp(buf, "eHl6") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "abcdef", 6, buf) == 8);
  ASSERT(strcmp(buf, "YWJjZGVm") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "ы", 2, buf) == 4);
  ASSERT(strcmp(buf, "0Ys=") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "xy", 3, buf) == 4);
  ASSERT(strcmp(buf, "eHkA") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "test", 4, buf) == 8);
  ASSERT(strcmp(buf, "dGVzdA==") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "abcde", 5, buf) == 8);
  ASSERT(strcmp(buf, "YWJjZGU=") == 0);

  ASSERT(mg_base64_decode("кю", 4, buf) == 0);
  ASSERT(mg_base64_decode("A", 1, buf) == 0);
  ASSERT(mg_base64_decode("A=", 2, buf) == 0);
  ASSERT(mg_base64_decode("AA=", 3, buf) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf) == 2);
  ASSERT(mg_base64_decode("AAAA====", 8, buf) == 0);
  ASSERT(mg_base64_decode("AAAA----", 8, buf) == 0);
  ASSERT(mg_base64_decode("Q2VzYW50YQ==", 12, buf) == 7);
  ASSERT(strcmp(buf, "Cesanta") == 0);
}

static void test_iobuf(void) {
  struct mg_iobuf io = {0, 0, 0};
  ASSERT(io.buf == NULL && io.size == 0 && io.len == 0);
  mg_iobuf_resize(&io, 1);
  ASSERT(io.buf != NULL && io.size == 1 && io.len == 0);
  ASSERT(memcmp(io.buf, "\x00", 1) == 0);
  mg_iobuf_add(&io, 3, "hi", 2, 10);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 5);
  ASSERT(memcmp(io.buf, "\x00\x00\x00hi", 5) == 0);
  mg_iobuf_add(&io, io.len, "!", 1, 10);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 6);
  ASSERT(memcmp(io.buf, "\x00\x00\x00hi!", 6) == 0);
  mg_iobuf_add(&io, 0, "x", 1, 10);
  ASSERT(memcmp(io.buf, "x\x00\x00\x00hi!", 7) == 0);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 7);
  mg_iobuf_del(&io, 1, 3);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 4);
  ASSERT(memcmp(io.buf, "xhi!", 3) == 0);
  mg_iobuf_del(&io, 10, 100);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 4);
  ASSERT(memcmp(io.buf, "xhi!", 3) == 0);
  free(io.buf);
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_SNTP_TIME) {
    *(int64_t *) fnd = *(int64_t *) evd;
    MG_DEBUG(("got time: %lld", *(int64_t *) evd));
  } else if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  }
  (void) c;
}

static void test_sntp_server(const char *url) {
  int64_t ms = 0;
  struct mg_mgr mgr;
  struct mg_connection *c = NULL;
  int i;

  mg_mgr_init(&mgr);
  c = mg_sntp_connect(&mgr, url, sntp_cb, &ms);
  ASSERT(c != NULL);
  ASSERT(c->is_udp == 1);
  for (i = 0; i < 60 && ms == 0; i++) mg_mgr_poll(&mgr, 50);
  MG_DEBUG(("server: %s, ms: %lld", url ? url : "(default)", ms));
  ASSERT(ms > 0);
  mg_mgr_free(&mgr);
}

static void test_sntp(void) {
  test_sntp_server("udp://time.windows.com:123");
  test_sntp_server(NULL);

  {
    int64_t ms;
    const unsigned char sntp_good[] =
        "\x24\x02\x00\xeb\x00\x00\x00\x1e\x00\x00\x07\xb6\x3e"
        "\xc9\xd6\xa2\xdb\xde\xea\x30\x91\x86\xb7\x10\xdb\xde"
        "\xed\x98\x00\x00\x00\xde\xdb\xde\xed\x99\x0a\xe2\xc7"
        "\x96\xdb\xde\xed\x99\x0a\xe4\x6b\xda";
    const unsigned char bad_good[] =
        "\x55\x02\x00\xeb\x00\x00\x00\x1e\x00\x00\x07\xb6\x3e"
        "\xc9\xd6\xa2\xdb\xde\xea\x30\x91\x86\xb7\x10\xdb\xde"
        "\xed\x98\x00\x00\x00\xde\xdb\xde\xed\x99\x0a\xe2\xc7"
        "\x96\xdb\xde\xed\x99\x0a\xe4\x6b\xda";
    ASSERT((ms = mg_sntp_parse(sntp_good, sizeof(sntp_good))) > 0);
#if MG_ARCH == MG_ARCH_UNIX
    time_t t = (time_t) (ms / 1000);
    struct tm tm;
    gmtime_r(&t, &tm);
    ASSERT(tm.tm_year == 116);
    ASSERT(tm.tm_mon == 10);
    ASSERT(tm.tm_mday == 22);
    ASSERT(tm.tm_hour == 16);
    ASSERT(tm.tm_min == 15);
    ASSERT(tm.tm_sec == 21);
#endif
    ASSERT(mg_sntp_parse(bad_good, sizeof(bad_good)) < 0);
  }

  ASSERT(mg_sntp_parse(NULL, 0) == -1);
}

struct mqtt_data {
  char *buf;
  int subscribed;
  int published;
};

static void mqtt_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  struct mqtt_data *test_data = (struct mqtt_data *) fnd;
  char *buf = test_data->buf;
  if (ev == MG_EV_MQTT_OPEN) {
    buf[0] = *(int *) evd == 0 ? 'X' : 'Y';
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) evd;
    if (mm->cmd == MQTT_CMD_SUBACK) {
      test_data->subscribed = 1;
    }
    if (mm->cmd == MQTT_CMD_PUBACK) {
      test_data->published = 1;
    }
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) evd;
    sprintf(buf + 1, "%.*s/%.*s", (int) mm->topic.len, mm->topic.ptr,
            (int) mm->data.len, mm->data.ptr);
  }
  (void) c;
}

static void test_mqtt(void) {
  char buf[50] = {0};
  struct mqtt_data test_data = {buf, 0, 0};
  struct mg_mgr mgr;
  struct mg_str topic = mg_str("x/f12"), data = mg_str("hi");
  struct mg_connection *c;
  struct mg_mqtt_opts opts;
  char rnd[9], client_id[16];
  // const char *url = "mqtt://mqtt.eclipse.org:1883";
  const char *url = "mqtt://broker.hivemq.com:1883";
  int i;
  mg_mgr_init(&mgr);

  {
    uint8_t bad[] = " \xff\xff\xff\xff ";
    struct mg_mqtt_message mm;
    mg_mqtt_parse(bad, sizeof(bad), &mm);
  }

  // Connect with empty client ID
  c = mg_mqtt_connect(&mgr, url, NULL, mqtt_cb, &test_data);
  for (i = 0; i < 300 && buf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (buf[0] != 'X') MG_INFO(("[%s]", buf));
  ASSERT(buf[0] == 'X');
  ASSERT(test_data.subscribed == 0);
  mg_mqtt_sub(c, topic, 1);
  for (i = 0; i < 500 && test_data.subscribed == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.subscribed == 1);
  ASSERT(test_data.published == 0);
  mg_mqtt_pub(c, topic, data, 1, false);
  for (i = 0; i < 500 && test_data.published == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.published == 1);
  for (i = 0; i < 500 && buf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (strcmp(buf, "Xx/f12/hi") != 0) MG_INFO(("[%s]", buf));
  ASSERT(strcmp(buf, "Xx/f12/hi") == 0);

  // Set params
  test_data.subscribed = 0;
  test_data.published = 0;
  memset(buf, 0, sizeof(buf));
  memset(&opts, 0, sizeof(opts));
  opts.clean = true;
  opts.will_qos = 1;
  opts.will_retain = true;
  opts.keepalive = 20;
  mg_random(rnd, sizeof(rnd));
  mg_base64_encode((unsigned char *) rnd, sizeof(rnd), client_id);
  client_id[sizeof(client_id) - 1] = '\0';
  opts.client_id = mg_str(client_id);
  opts.will_topic = mg_str("mg_will_topic");
  opts.will_message = mg_str("mg_will_messsage");
  c = mg_mqtt_connect(&mgr, url, &opts, mqtt_cb, &test_data);
  for (i = 0; i < 300 && buf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (buf[0] != 'X') MG_INFO(("[%s]", buf));
  ASSERT(buf[0] == 'X');
  ASSERT(test_data.subscribed == 0);
  mg_mqtt_sub(c, topic, 1);
  for (i = 0; i < 500 && test_data.subscribed == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.subscribed == 1);
  ASSERT(test_data.published == 0);
  mg_mqtt_pub(c, topic, data, 1, false);
  for (i = 0; i < 500 && test_data.published == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.published == 1);
  for (i = 0; i < 500 && buf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (strcmp(buf, "Xx/f12/hi") != 0) MG_INFO(("[%s]", buf));
  ASSERT(strcmp(buf, "Xx/f12/hi") == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void eh1(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_tls_opts *topts = (struct mg_tls_opts *) fn_data;
  if (ev == MG_EV_ACCEPT && topts != NULL) mg_tls_init(c, topts);
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("[%.*s %.*s] message len %d", (int) hm->method.len, hm->method.ptr,
             (int) hm->uri.len, hm->uri.ptr, (int) hm->message.len));
    if (mg_http_match_uri(hm, "/foo/*")) {
      mg_http_reply(c, 200, "", "uri: %.*s", hm->uri.len - 5, hm->uri.ptr + 5);
    } else if (mg_http_match_uri(hm, "/ws")) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_http_match_uri(hm, "/body")) {
      mg_http_reply(c, 200, "", "%.*s", (int) hm->body.len, hm->body.ptr);
    } else if (mg_http_match_uri(hm, "/bar")) {
      mg_http_reply(c, 404, "", "not found");
    } else if (mg_http_match_uri(hm, "/no_reason")) {
      mg_printf(c, "%s", "HTTP/1.0 200\r\nContent-Length: 2\r\n\r\nok");
    } else if (mg_http_match_uri(hm, "/badroot")) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = "/BAAADDD!";
      mg_http_serve_dir(c, hm, &sopts);
    } else if (mg_http_match_uri(hm, "/creds")) {
      char user[100], pass[100];
      mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
      mg_http_reply(c, 200, "", "[%s]:[%s]", user, pass);
    } else if (mg_http_match_uri(hm, "/upload")) {
      mg_http_upload(c, hm, &mg_fs_posix, ".");
    } else if (mg_http_match_uri(hm, "/test/")) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = ".";
      sopts.extra_headers = "A: B\r\nC: D\r\n";
      mg_http_serve_dir(c, hm, &sopts);
    } else if (mg_http_match_uri(hm, "/servefile")) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.mime_types = "foo=a/b,txt=c/d";
      mg_http_serve_file(c, hm, "test/data/a.txt", &sopts);
    } else {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = "./test/data";
      sopts.ssi_pattern = "#.shtml";
      sopts.extra_headers = "C: D\r\n";
      mg_http_serve_dir(c, hm, &sopts);
    }
  } else if (ev == MG_EV_WS_OPEN) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ASSERT(mg_strcmp(hm->uri, mg_str("/ws")) == 0);
    mg_ws_send(c, "opened", 6, WEBSOCKET_OP_BINARY);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_BINARY);
  }
}

struct fetch_data {
  char *buf;
  int code, closed;
};

static void fcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct fetch_data *fd = (struct fetch_data *) fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    snprintf(fd->buf, FETCH_BUF_SIZE, "%.*s", (int) hm->message.len,
             hm->message.ptr);
    fd->code = atoi(hm->uri.ptr);
    fd->closed = 1;
    c->is_closing = 1;
    (void) c;
  }
}

static int fetch(struct mg_mgr *mgr, char *buf, const char *url,
                 const char *fmt, ...) {
  struct fetch_data fd = {buf, 0, 0};
  int i;
  struct mg_connection *c = mg_http_connect(mgr, url, fcb, &fd);
  va_list ap;
  ASSERT(c != NULL);
  if (mg_url_is_ssl(url)) {
    struct mg_tls_opts opts;
    struct mg_str host = mg_url_host(url);
    memset(&opts, 0, sizeof(opts));
    opts.ca = "./test/data/ca.pem";
    if (strstr(url, "127.0.0.1") != NULL) {
      // Local connection, use self-signed certificates
      opts.ca = "./test/data/ss_ca.pem";
      opts.cert = "./test/data/ss_client.pem";
    } else {
      opts.srvname = host;
    }
    mg_tls_init(c, &opts);
    if (c->tls == NULL) fd.closed = 1;
    // c->is_hexdumping = 1;
  }
  va_start(ap, fmt);
  mg_vprintf(c, fmt, ap);
  va_end(ap);
  buf[0] = '\0';
  for (i = 0; i < 250 && buf[0] == '\0'; i++) mg_mgr_poll(mgr, 1);
  if (!fd.closed) c->is_closing = 1;
  mg_mgr_poll(mgr, 1);
  return fd.code;
}

static int cmpbody(const char *buf, const char *str) {
  struct mg_http_message hm;
  struct mg_str s = mg_str(str);
  size_t len = strlen(buf);
  mg_http_parse(buf, len, &hm);
  if (hm.body.len > len) hm.body.len = len - (size_t) (hm.body.ptr - buf);
  return mg_strcmp(hm.body, s);
}

static bool cmpheader(const char *buf, const char *name, const char *value) {
  struct mg_http_message hm;
  struct mg_str *h;
  size_t len = strlen(buf);
  mg_http_parse(buf, len, &hm);
  h = mg_http_get_header(&hm, name);
  return h != NULL && mg_strcmp(*h, mg_str(value)) == 0;
}

static void wcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int *p = (int *) fn_data;
  if (ev == MG_EV_WS_OPEN) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_str *wsproto = mg_http_get_header(hm, "Sec-WebSocket-Protocol");
    ASSERT(wsproto != NULL);
    mg_ws_send(c, "boo", 3, WEBSOCKET_OP_BINARY);
    mg_ws_send(c, "foobar", 6, WEBSOCKET_OP_BINARY);
    mg_ws_send(c, "", 0, WEBSOCKET_OP_PING);
    p[0] += 100;
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (mg_strstr(wm->data, mg_str("foobar")))
      mg_ws_send(c, "", 0, WEBSOCKET_OP_CLOSE);
    if (mg_strstr(wm->data, mg_str("boo"))) p[0] += 2;
    if (mg_strstr(wm->data, mg_str("foobar"))) p[0] += 3;
  } else if (ev == MG_EV_CLOSE) {
    p[0] += 10;
  }
}

static void test_ws(void) {
  char buf[FETCH_BUF_SIZE];
  const char *url = "ws://LOCALHOST:12343/ws";
  struct mg_mgr mgr;
  int i, done = 0;

  mg_mgr_init(&mgr);
  ASSERT(mg_http_listen(&mgr, url, eh1, NULL) != NULL);
  mg_ws_connect(&mgr, url, wcb, &done, "%s", "Sec-WebSocket-Protocol: meh\r\n");
  for (i = 0; i < 30; i++) mg_mgr_poll(&mgr, 1);
  // MG_INFO(("--> %d", done));
  ASSERT(done == 115);

  // Test that non-WS requests fail
  ASSERT(fetch(&mgr, buf, url, "GET /ws HTTP/1.0\r\n\n") == 426);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void eh9(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ERROR) {
    ASSERT(!strcmp((char *) ev_data, "error connecting to 127.0.0.1:55117"));
    *(int *) fn_data = 7;
  }
  (void) c;
}

static void test_http_server(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12346";
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh1, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /%%61.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);

  // Responses with missing reason phrase must also work
  ASSERT(fetch(&mgr, buf, url, "GET /no_reason HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "ok") == 0);

  // Fetch file with unicode chars in filename
  ASSERT(fetch(&mgr, buf, url, "GET /київ.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "є\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /../fuzz.c HTTP/1.0\n\n") == 404);
  ASSERT(fetch(&mgr, buf, url, "GET /.%%2e/fuzz.c HTTP/1.0\n\n") == 404);
  ASSERT(fetch(&mgr, buf, url, "GET /.%%2e%%2ffuzz.c HTTP/1.0\n\n") == 404);
  ASSERT(fetch(&mgr, buf, url, "GET /..%%2f%%20fuzz.c HTTP/1.0\n\n") == 404);
  ASSERT(fetch(&mgr, buf, url, "GET /..%%2ffuzz.c%%20 HTTP/1.0\n\n") == 404);

  ASSERT(fetch(&mgr, buf, url, "GET /dredir HTTP/1.0\n\n") == 301);
  ASSERT(cmpheader(buf, "Location", "/dredir/"));

  ASSERT(fetch(&mgr, buf, url, "GET /dredir/ HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hi\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /..ddot HTTP/1.0\n\n") == 301);
  ASSERT(fetch(&mgr, buf, url, "GET /..ddot/ HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hi\n") == 0);

  {
    extern char *mg_http_etag(char *, size_t, size_t, time_t);
    struct mg_http_message hm;
    char etag[100];
    size_t size = 0;
    time_t mtime = 0;
    ASSERT(mg_fs_posix.st("./test/data/a.txt", &size, &mtime) != 0);
    ASSERT(mg_http_etag(etag, sizeof(etag), size, mtime) == etag);
    ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\nIf-None-Match: %s\n\n",
                 etag) == 304);
    ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
    MG_INFO(("%s", buf));
    ASSERT(mg_http_get_header(&hm, "C") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "C"), mg_str("D")) == 0);
  }

  // Text mime type override
  ASSERT(fetch(&mgr, buf, url, "GET /servefile HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  {
    struct mg_http_message hm;
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_http_get_header(&hm, "Content-Type") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Type"), mg_str("c/d")) ==
           0);
  }

  ASSERT(fetch(&mgr, buf, url, "GET /foo/1 HTTP/1.0\r\n\n") == 200);
  // MG_INFO(("%d %.*s", (int) hm.len, (int) hm.len, hm.buf));
  ASSERT(cmpbody(buf, "uri: 1") == 0);

  ASSERT(fetch(&mgr, buf, url, "%s",
               "POST /body HTTP/1.1\r\n"
               "Content-Length: 4\r\n\r\nkuku") == 200);
  ASSERT(cmpbody(buf, "kuku") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /ssi HTTP/1.1\r\n\r\n") == 301);
  ASSERT(fetch(&mgr, buf, url, "GET /ssi/ HTTP/1.1\r\n\r\n") == 200);
  ASSERT(cmpbody(buf,
                 "this is index\n"
                 "this is nested\n\n"
                 "this is f1\n\n\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n") == 0);
  {
    struct mg_http_message hm;
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_http_get_header(&hm, "Content-Length") != NULL);
    ASSERT(mg_http_get_header(&hm, "Content-Type") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Type"),
                     mg_str("text/html; charset=utf-8")) == 0);
  }

  ASSERT(fetch(&mgr, buf, url, "GET /badroot HTTP/1.0\r\n\n") == 404);
  // ASSERT(cmpbody(buf, "Invalid web root [/BAAADDD!]\n") == 0);

  {
    char *data = mg_file_read(&mg_fs_posix, "./test/data/ca.pem", NULL);
    ASSERT(fetch(&mgr, buf, url, "GET /ca.pem HTTP/1.0\r\n\n") == 200);
    ASSERT(cmpbody(buf, data) == 0);
    free(data);
  }

  {
    // Test mime type
    struct mg_http_message hm;
    ASSERT(fetch(&mgr, buf, url, "GET /empty.js HTTP/1.0\r\n\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_http_get_header(&hm, "Content-Type") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Type"),
                     mg_str("text/javascript; charset=utf-8")) == 0);
  }

  {
    // Test connection refused
    int i, errored = 0;
    mg_connect(&mgr, "tcp://127.0.0.1:55117", eh9, &errored);
    for (i = 0; i < 10 && errored == 0; i++) mg_mgr_poll(&mgr, 1);
    ASSERT(errored == 7);
  }

  // Directory listing
  fetch(&mgr, buf, url, "GET /test/ HTTP/1.0\n\n");
  ASSERT(fetch(&mgr, buf, url, "GET /test/ HTTP/1.0\n\n") == 200);
  ASSERT(mg_strstr(mg_str(buf), mg_str(">Index of /test/<")) != NULL);
  ASSERT(mg_strstr(mg_str(buf), mg_str(">fuzz.c<")) != NULL);

  {
    // Credentials
    struct mg_http_message hm;
    ASSERT(fetch(&mgr, buf, url, "%s",
                 "GET /creds?access_token=x HTTP/1.0\r\n\r\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_strcmp(hm.body, mg_str("[]:[x]")) == 0);

    ASSERT(fetch(&mgr, buf, url, "%s",
                 "GET /creds HTTP/1.0\r\n"
                 "Authorization: Bearer x\r\n\r\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_strcmp(hm.body, mg_str("[]:[x]")) == 0);

    ASSERT(fetch(&mgr, buf, url, "%s",
                 "GET /creds HTTP/1.0\r\n"
                 "Authorization: Basic Zm9vOmJhcg==\r\n\r\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_strcmp(hm.body, mg_str("[foo]:[bar]")) == 0);

    ASSERT(fetch(&mgr, buf, url, "%s",
                 "GET /creds HTTP/1.0\r\n"
                 "Cookie: blah; access_token=hello\r\n\r\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_strcmp(hm.body, mg_str("[]:[hello]")) == 0);
  }

  {
    // Test upload
    char *p;
    remove("uploaded.txt");
    ASSERT((p = mg_file_read(&mg_fs_posix, "uploaded.txt", NULL)) == NULL);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload HTTP/1.0\n"
                 "Content-Length: 1\n\nx") == 400);

    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?name=uploaded.txt HTTP/1.0\r\n"
                 "Content-Length: 5\r\n"
                 "\r\nhello") == 200);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?name=uploaded.txt&offset=5 HTTP/1.0\r\n"
                 "Content-Length: 6\r\n"
                 "\r\n\nworld") == 200);
    ASSERT((p = mg_file_read(&mg_fs_posix, "uploaded.txt", NULL)) != NULL);
    ASSERT(strcmp(p, "hello\nworld") == 0);
    free(p);
    remove("uploaded.txt");
  }

  {
    // Test upload directory traversal
    char *p;
    remove("uploaded.txt");
    ASSERT((p = mg_file_read(&mg_fs_posix, "uploaded.txt", NULL)) == NULL);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?name=../uploaded.txt HTTP/1.0\r\n"
                 "Content-Length: 5\r\n"
                 "\r\nhello") == 200);
    ASSERT((p = mg_file_read(&mg_fs_posix, "uploaded.txt", NULL)) != NULL);
    ASSERT(strcmp(p, "hello") == 0);
    free(p);
    remove("uploaded.txt");
  }

  // HEAD request
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(fetch(&mgr, buf, url, "HEAD /a.txt HTTP/1.0\n\n") == 200);

  // Pre-compressed files
  {
    struct mg_http_message hm;
    ASSERT(fetch(&mgr, buf, url, "HEAD /hello.txt HTTP/1.0\n\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_http_get_header(&hm, "Content-Encoding") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Encoding"),
                     mg_str("gzip")) == 0);
  }

#if MG_ENABLE_IPV6
  {
    const char *url6 = "http://[::1]:12366";
    ASSERT(mg_http_listen(&mgr, url6, eh1, NULL) != NULL);
    ASSERT(fetch(&mgr, buf, url6, "GET /a.txt HTTP/1.0\n\n") == 200);
    ASSERT(cmpbody(buf, "hello\n") == 0);
  }
#endif

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void h4(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("[%.*s %.*s] message len %d", (int) hm->method.len, hm->method.ptr,
             (int) hm->uri.len, hm->uri.ptr, (int) hm->message.len));
    if (mg_http_match_uri(hm, "/a/#")) {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "/a=./test/data";
      opts.page404 = "./test/data/404.html";  // existing 404 page
      mg_http_serve_dir(c, hm, &opts);
    } else if (mg_http_match_uri(hm, "/b/#")) {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "/b=./test/data";
      opts.page404 = "./test/data/nooooo.html";  // non-existing 404 page
      mg_http_serve_dir(c, hm, &opts);
    } else {  // null 404 page
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "./test/data";
      mg_http_serve_dir(c, hm, &opts);
    }
  }
  (void) fn_data;
}

static void test_http_404(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:22343";
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, h4, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /a/a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /b/a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /xx.txt HTTP/1.0\n\n") == 404);
  ASSERT(cmpbody(buf, "Not found\n") == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /a/xx.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "boo\n") == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /b/xx.txt HTTP/1.0\n\n") == 404);
  ASSERT(cmpbody(buf, "Not found\n") == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_tls(void) {
#if MG_ENABLE_MBEDTLS || MG_ENABLE_OPENSSL
  struct mg_tls_opts opts = {.ca = "./test/data/ss_ca.pem",
                             .cert = "./test/data/ss_server.pem",
                             .certkey = "./test/data/ss_server.pem"};
  struct mg_mgr mgr;
  struct mg_connection *c;
  const char *url = "https://127.0.0.1:12347";
  char buf[FETCH_BUF_SIZE];
  mg_mgr_init(&mgr);
  c = mg_http_listen(&mgr, url, eh1, (void *) &opts);
  ASSERT(c != NULL);
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  // MG_INFO(("%s", buf));
  ASSERT(cmpbody(buf, "hello\n") == 0);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
#endif
}

static void f3(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int *ok = (int *) fn_data;
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
    *ok = mg_http_status(hm);
  } else if (ev == MG_EV_CLOSE) {
    if (*ok == 0) *ok = 888;
  } else if (ev == MG_EV_ERROR) {
    if (*ok == 0) *ok = 777;
  }
}

static void test_http_client(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  int i, ok = 0;
  mg_mgr_init(&mgr);
  c = mg_http_connect(&mgr, "http://cesanta.com", f3, &ok);
  ASSERT(c != NULL);
  for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(ok == 301);
  c->is_closing = 1;
  mg_mgr_poll(&mgr, 0);
  ok = 0;
#if MG_ENABLE_MBEDTLS || MG_ENABLE_OPENSSL
  {
    const char *url = "https://cesanta.com";
    struct mg_str host = mg_url_host(url);
    struct mg_tls_opts opts = {.ca = "./test/data/ca.pem", .srvname = host};
    c = mg_http_connect(&mgr, url, f3, &ok);
    ASSERT(c != NULL);
    mg_tls_init(c, &opts);
    for (i = 0; i < 1500 && ok <= 0; i++) mg_mgr_poll(&mgr, 1000);
    ASSERT(ok == 200);
    c->is_closing = 1;
    mg_mgr_poll(&mgr, 1);

    // Test failed host validation
    ok = 0;
    opts.srvname = mg_str("dummy");
    c = mg_http_connect(&mgr, url, f3, &ok);
    ASSERT(c != NULL);
    mg_tls_init(c, &opts);
    for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
    ASSERT(ok == 777);
    mg_mgr_poll(&mgr, 1);
  }
#endif

#if MG_ENABLE_IPV6
  ok = 0;
  // ipv6.google.com does not have IPv4 address, only IPv6, therefore
  // it is guaranteed to hit IPv6 resolution path.
  c = mg_http_connect(&mgr, "http://ipv6.google.com", f3, &ok);
  for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(ok == 200);
#endif

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f4(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_printf(c, "HTTP/1.0 200 OK\n\n%.*s/%s", (int) hm->uri.len, hm->uri.ptr,
              "abcdef");
    strcat((char *) fn_data, "m");
    c->is_draining = 1;
  } else if (ev == MG_EV_HTTP_CHUNK) {
    strcat((char *) fn_data, "f");
  } else if (ev == MG_EV_CLOSE) {
    strcat((char *) fn_data, "c");
  }
}

static void f4c(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET /foo/bar HTTP/1.0\n\n");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ASSERT(mg_strcmp(hm->body, mg_str("/foo/bar/abcdef")) == 0);
    strcat((char *) fn_data, "m");
  } else if (ev == MG_EV_HTTP_CHUNK) {
    strcat((char *) fn_data, "f");
  } else if (ev == MG_EV_CLOSE) {
    strcat((char *) fn_data, "c");
  }
}

static void test_http_no_content_length(void) {
  char buf1[10] = {0}, buf2[10] = {0};
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12348";
  int i;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, f4, (void *) buf1);
  mg_http_connect(&mgr, url, f4c, (void *) buf2);
  for (i = 0; i < 100 && strchr(buf2, 'c') == NULL; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(strcmp(buf1, "mc") == 0);
  ASSERT(strcmp(buf2, "fcm") == 0);  // See #1475
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f5(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_printf(c, "HTTP/1.0 200 OK\n\n%.*s", (int) hm->uri.len, hm->uri.ptr);
    (*(int *) fn_data)++;
  }
}

static void test_http_pipeline(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12377";
  struct mg_connection *c;
  int i, ok = 0;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, f5, (void *) &ok);
  c = mg_http_connect(&mgr, url, NULL, NULL);
  mg_printf(c, "POST / HTTP/1.0\nContent-Length: 5\n\n12345GET / HTTP/1.0\n\n");
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 1);
  // MG_INFO(("-----> [%d]", ok));
  ASSERT(ok == 2);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_http_parse(void) {
  struct mg_str *v;
  struct mg_http_message req;

  {
    const char *s = "GET / HTTP/1.0\n\n";
    ASSERT(mg_http_parse("\b23", 3, &req) == -1);
    ASSERT(mg_http_parse("get\n\n", 5, &req) == -1);
    ASSERT(mg_http_parse(s, strlen(s) - 1, &req) == 0);
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(req.message.len == strlen(s));
    ASSERT(req.body.len == 0);
  }

  {
    const char *s = "GET /blah HTTP/1.0\r\nFoo:  bar  \r\n\r\n";
    size_t idx, len = strlen(s);
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) len);
    ASSERT(mg_vcmp(&req.headers[0].name, "Foo") == 0);
    ASSERT(mg_vcmp(&req.headers[0].value, "bar") == 0);
    ASSERT(req.headers[1].name.len == 0);
    ASSERT(req.headers[1].name.ptr == NULL);
    ASSERT(req.query.len == 0);
    ASSERT(req.message.len == len);
    ASSERT(req.body.len == 0);
    for (idx = 0; idx < len; idx++) ASSERT(mg_http_parse(s, idx, &req) == 0);
  }

  {
    static const char *s = "get b c\nz :  k \nb: t\nvvv\n\n xx";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
    ASSERT(req.headers[2].name.len == 0);
    ASSERT(mg_vcmp(&req.headers[0].value, "k") == 0);
    ASSERT(mg_vcmp(&req.headers[1].value, "t") == 0);
    ASSERT(req.body.len == 0);
  }

  {
    const char *s = "a b c\r\nContent-Length: 21 \r\nb: t\r\nvvv\r\n\r\nabc";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
    ASSERT(req.body.len == 21);
    ASSERT(req.message.len == 21 - 3 + strlen(s));
    ASSERT(mg_http_get_header(&req, "foo") == NULL);
    ASSERT((v = mg_http_get_header(&req, "contENT-Length")) != NULL);
    ASSERT(mg_vcmp(v, "21") == 0);
    ASSERT((v = mg_http_get_header(&req, "B")) != NULL);
    ASSERT(mg_vcmp(v, "t") == 0);
  }

  {
    const char *s = "GET /foo?a=b&c=d HTTP/1.0\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(mg_vcmp(&req.uri, "/foo") == 0);
    ASSERT(mg_vcmp(&req.query, "a=b&c=d") == 0);
  }

  {
    const char *s = "POST /x HTTP/1.0\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(req.body.len == (size_t) ~0);
  }

  {
    const char *s = "WOHOO /x HTTP/1.0\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(req.body.len == 0);
  }

  {
    const char *s = "HTTP/1.0 200 OK\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(mg_vcmp(&req.method, "HTTP/1.0") == 0);
    ASSERT(mg_vcmp(&req.uri, "200") == 0);
    ASSERT(mg_vcmp(&req.proto, "OK") == 0);
    ASSERT(req.body.len == (size_t) ~0);
  }

  {
    static const char *s = "HTTP/1.0 999 OMGWTFBBQ\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
  }

  {
    const char *s =
        "GET / HTTP/1.0\r\nhost:127.0.0.1:18888\r\nCookie:\r\nX-PlayID: "
        "45455\r\nRange:  0-1 \r\n\r\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT((v = mg_http_get_header(&req, "Host")) != NULL);
    ASSERT(mg_vcmp(v, "127.0.0.1:18888") == 0);
    ASSERT((v = mg_http_get_header(&req, "Cookie")) != NULL);
    ASSERT(v->len == 0);
    ASSERT((v = mg_http_get_header(&req, "X-PlayID")) != NULL);
    ASSERT(mg_vcmp(v, "45455") == 0);
    ASSERT((v = mg_http_get_header(&req, "Range")) != NULL);
    ASSERT(mg_vcmp(v, "0-1") == 0);
  }

  {
    static const char *s = "a b c\na:1\nb:2\nc:3\nd:4\ne:5\nf:6\ng:7\nh:8\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT((v = mg_http_get_header(&req, "e")) != NULL);
    ASSERT(mg_vcmp(v, "5") == 0);
    ASSERT((v = mg_http_get_header(&req, "h")) == NULL);
  }

  {
    struct mg_connection c;
    struct mg_str s,
        res = mg_str("GET /\r\nAuthorization: Basic Zm9vOmJhcg==\r\n\r\n");
    memset(&c, 0, sizeof(c));
    mg_printf(&c, "%s", "GET /\r\n");
    mg_http_bauth(&c, "foo", "bar");
    mg_printf(&c, "%s", "\r\n");
    s = mg_str_n((char *) c.send.buf, c.send.len);
    ASSERT(mg_strcmp(s, res) == 0);
    mg_iobuf_free(&c.send);
  }

  {
    struct mg_http_message hm;
    const char *s = "GET /foo?bar=baz HTTP/1.0\n\n ";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s) - 1);
    ASSERT(mg_strcmp(hm.uri, mg_str("/foo")) == 0);
    ASSERT(mg_strcmp(hm.query, mg_str("bar=baz")) == 0);
  }

  {
    struct mg_http_message hm;
    const char *s = "a b c\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b\nc\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a\nb\nc\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) < 0);
  }
}

static void ehr(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.root_dir = "./test/data";
    mg_http_serve_dir(c, hm, &opts);
  }
  (void) fn_data;
}

static void test_http_range(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12349";
  struct mg_http_message hm;
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, ehr, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /range.txt HTTP/1.0\n\n") == 200);
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(hm.body.len == 312);

  fetch(&mgr, buf, url, "%s", "GET /range.txt HTTP/1.0\nRange: bytes=5-10\n\n");
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(mg_strcmp(hm.uri, mg_str("206")) == 0);
  ASSERT(mg_strcmp(hm.proto, mg_str("Partial Content")) == 0);
  ASSERT(mg_strcmp(hm.body, mg_str(" of co")) == 0);
  ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Range"),
                   mg_str("bytes 5-10/312")) == 0);

  // Fetch till EOF
  fetch(&mgr, buf, url, "%s", "GET /range.txt HTTP/1.0\nRange: bytes=300-\n\n");
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(mg_strcmp(hm.uri, mg_str("206")) == 0);
  ASSERT(mg_strcmp(hm.body, mg_str("is disease.\n")) == 0);
  // MG_INFO(("----%d\n[%s]", (int) hm.body.len, buf));

  // Fetch past EOF, must trigger 416 response
  fetch(&mgr, buf, url, "%s", "GET /range.txt HTTP/1.0\nRange: bytes=999-\n\n");
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(mg_strcmp(hm.uri, mg_str("416")) == 0);
  ASSERT(hm.body.len == 0);
  ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Range"),
                   mg_str("bytes */312")) == 0);

  fetch(&mgr, buf, url, "%s",
        "GET /range.txt HTTP/1.0\nRange: bytes=0-312\n\n");
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(mg_strcmp(hm.uri, mg_str("416")) == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f1(void *arg) {
  (*(int *) arg)++;
}

static void test_timer(void) {
  int v1 = 0, v2 = 0, v3 = 0;
  struct mg_timer t1, t2, t3, *head = NULL;

  mg_timer_init(&head, &t1, 5, MG_TIMER_REPEAT, f1, &v1);
  mg_timer_init(&head, &t2, 15, MG_TIMER_ONCE, f1, &v2);
  mg_timer_init(&head, &t3, 10, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, f1, &v3);

  ASSERT(head == &t3);
  ASSERT(head->next == &t2);

  mg_timer_poll(&head, 0);
  ASSERT(v1 == 0);
  ASSERT(v2 == 0);
  ASSERT(v3 == 1);

  mg_timer_poll(&head, 1);
  ASSERT(v1 == 0);
  ASSERT(v2 == 0);
  ASSERT(v3 == 1);

  mg_timer_poll(&head, 5);
  ASSERT(v1 == 1);
  ASSERT(v2 == 0);
  ASSERT(v3 == 1);

  // Simulate long delay - timers must invalidate expiration times
  mg_timer_poll(&head, 100);
  ASSERT(v1 == 2);
  ASSERT(v2 == 1);
  ASSERT(v3 == 2);

  mg_timer_poll(&head, 107);
  ASSERT(v1 == 3);
  ASSERT(v2 == 1);
  ASSERT(v3 == 2);

  mg_timer_poll(&head, 114);
  ASSERT(v1 == 4);
  ASSERT(v2 == 1);
  ASSERT(v3 == 3);

  mg_timer_poll(&head, 115);
  ASSERT(v1 == 5);
  ASSERT(v2 == 1);
  ASSERT(v3 == 3);

  mg_timer_free(&head, &t2);
  mg_timer_init(&head, &t2, 3, 0, f1, &v2);
  ASSERT(head == &t2);
  ASSERT(head->next == &t3);
  ASSERT(head->next->next == &t1);
  ASSERT(head->next->next->next == NULL);

  mg_timer_poll(&head, 120);
  ASSERT(v1 == 6);
  ASSERT(v2 == 1);
  ASSERT(v3 == 4);

  mg_timer_poll(&head, 125);
  ASSERT(v1 == 7);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  // Test millisecond counter wrap - when time goes back.
  mg_timer_poll(&head, 0);
  ASSERT(v1 == 7);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  mg_timer_poll(&head, 7);
  ASSERT(v1 == 8);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  mg_timer_poll(&head, 11);
  ASSERT(v1 == 9);
  ASSERT(v2 == 2);
  ASSERT(v3 == 5);

  mg_timer_free(&head, &t1);
  ASSERT(head == &t2);
  ASSERT(head->next == &t3);
  ASSERT(head->next->next == NULL);

  mg_timer_free(&head, &t2);
  ASSERT(head == &t3);
  ASSERT(head->next == NULL);

  mg_timer_free(&head, &t3);
  ASSERT(head == NULL);

  // Test proper timer deallocation, see #1539
  {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_timer_add(&mgr, 1, MG_TIMER_REPEAT, f1, NULL);
    mg_mgr_free(&mgr);
    ASSERT(mgr.conns == NULL);
  }
}

static bool sn(const char *fmt, ...) {
  char buf[100], tmp[1] = {0}, buf2[sizeof(buf)];
  size_t n, n2, n1;
  va_list ap;
  bool result;
  va_start(ap, fmt);
  n = (size_t) vsnprintf(buf2, sizeof(buf2), fmt, ap);
  va_end(ap);
  va_start(ap, fmt);
  n1 = mg_vsnprintf(buf, sizeof(buf), fmt, &ap);
  va_end(ap);
  va_start(ap, fmt);
  n2 = mg_vsnprintf(tmp, 0, fmt, &ap);
  va_end(ap);
  result = n1 == n2 && n1 == n && strcmp(buf, buf2) == 0;
  if (!result)
    MG_ERROR(("[%s] -> [%s] != [%s] %d %d %d\n", fmt, buf, buf2, (int) n1,
              (int) n2, (int) n));
  return result;
}

static bool sccmp(const char *s1, const char *s2, int expected) {
  int n1 = mg_casecmp(s1, s2);
  // MG_INFO(("[%s] [%s] %d %d", s1, s2, n1, expected));
  return n1 == expected;
}

static size_t pf1(char *buf, size_t len, va_list *ap) {
  int a = va_arg(*ap, int);
  int b = va_arg(*ap, int);
  return mg_snprintf(buf, len, "%d", a + b);
}

static size_t pf2(char *buf, size_t len, va_list *ap) {
  int cnt = va_arg(*ap, int);
  size_t n = 0;
  while (cnt-- > 0)
    n += mg_snprintf(buf ? buf + n : 0, n < len ? len - n : 0, "%d", cnt);
  return n;
}

static void test_str(void) {
  struct mg_str s = mg_strdup(mg_str("a"));
  ASSERT(mg_strcmp(s, mg_str("a")) == 0);
  free((void *) s.ptr);
  ASSERT(mg_strcmp(mg_str(""), mg_str(NULL)) == 0);
  ASSERT(mg_strcmp(mg_str("a"), mg_str("b")) < 0);
  ASSERT(mg_strcmp(mg_str("b"), mg_str("a")) > 0);
  ASSERT(mg_strstr(mg_str("abc"), mg_str("d")) == NULL);
  ASSERT(mg_strstr(mg_str("abc"), mg_str("b")) != NULL);
  ASSERT(mg_strcmp(mg_str("hi"), mg_strstrip(mg_str(" \thi\r\n"))) == 0);

  ASSERT(sccmp("", "", 0));
  ASSERT(sccmp("", "1", -49));
  ASSERT(sccmp("a", "A", 0));
  ASSERT(sccmp("a1", "A", 49));
  ASSERT(sccmp("a", "A1", -49));

  {
    int n;
    double tolerance = 1e-14;
    ASSERT(mg_atod("1.23", 4, &n) == 1.23 && n == 4);
    ASSERT(mg_atod("1.23", 3, &n) == 1.2 && n == 3);
    ASSERT(mg_atod("1.23", 2, &n) == 1 && n == 2);
    ASSERT(mg_atod("1.23 ", 5, &n) - 1.23 < tolerance && n == 4);
    ASSERT(mg_atod("-0.01 ", 6, &n) + 0.01 < tolerance);
    ASSERT(mg_atod("-0.5e2", 6, &n) + 50 < tolerance);
    ASSERT(mg_atod("123e-3", 6, &n) - 0.123 < tolerance);
  }

  ASSERT(sn("%d", 0));
  ASSERT(sn("%d", 1));
  ASSERT(sn("%d", -1));
  ASSERT(sn("%.*s", 0, "ab"));
  ASSERT(sn("%.*s", 1, "ab"));
  ASSERT(sn("%.1s", "ab"));
  ASSERT(sn("%.99s", "a"));
  ASSERT(sn("%11s", "a"));
  ASSERT(sn("%s", "a\0b"));
  ASSERT(sn("%2s", "a"));
  ASSERT(sn("%.*s", 3, "a\0b"));
  ASSERT(sn("%d", 7));
  ASSERT(sn("%d", 123));
#if MG_ARCH == MG_ARCH_UNIX
  ASSERT(sn("%lld", (uint64_t) 0xffffffffff));
  ASSERT(sn("%lld", (uint64_t) -1));
  ASSERT(sn("%llu", (uint64_t) -1));
  ASSERT(sn("%llx", (uint64_t) 0xffffffffff));
  ASSERT(sn("%p", (void *) (size_t) 7));
#endif
  ASSERT(sn("%lx", (unsigned long) 0x6204d754));
  ASSERT(sn("ab"));
  ASSERT(sn("%dx", 1));
  ASSERT(sn("%sx", "a"));
  ASSERT(sn("%cx", 32));
  ASSERT(sn("%x", 15));
  ASSERT(sn("%2x", 15));
  ASSERT(sn("%02x", 15));
  ASSERT(sn("%hx:%hhx", (short) 1, (char) 2));
  ASSERT(sn("%hx:%hhx", (short) 1, (char) 2));
  ASSERT(sn("%%"));
  ASSERT(sn("%x", 15));
  ASSERT(sn("%#x", 15));
  ASSERT(sn("%#6x", 15));
  ASSERT(sn("%#06x", 15));
  ASSERT(sn("%#-6x", 15));
  ASSERT(sn("%-2s!", "a"));
  ASSERT(sn("%s %s", "a", "b"));
  ASSERT(sn("%s %s", "a", "b"));
  ASSERT(sn("ab%dc", 123));
  ASSERT(sn("%s ", "a"));
  ASSERT(sn("%s %s", "a", "b"));
  ASSERT(sn("%2s %s", "a", "b"));

  // Non-standard formatting
  {
    char buf[100], *p;
    const char *expected;

    expected = "\"\"";
    mg_snprintf(buf, sizeof(buf), "%Q", "");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"a'b\"";
    mg_snprintf(buf, sizeof(buf), "%Q", "a'b");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"a\\b\\n\\f\\r\\t\\\"\"";
    mg_snprintf(buf, sizeof(buf), "%Q", "a\b\n\f\r\t\"");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"abc\"";
    mg_snprintf(buf, sizeof(buf), "%.*Q", 3, "abcdef");
    ASSERT(strcmp(buf, expected) == 0);

    p = mg_mprintf("[%s,%M,%s]", "null", pf1, 2, 3, "hi");
    // printf("-> %s\n", p);
    ASSERT(strcmp(p, "[null,5,hi]") == 0);
    free(p);

    p = mg_mprintf("[%M,%d]", pf2, 10, 7);
    // printf("-> %s\n", p);
    ASSERT(strcmp(p, "[9876543210,7]") == 0);
    free(p);
  }

  {
    char tmp[40];
#define DBLWIDTH(a, b) a, b
#define TESTDOUBLE(fmt_, num_, res_)                                          \
  do {                                                                        \
    const char *N = #num_;                                                    \
    size_t n = mg_snprintf(tmp, sizeof(tmp), fmt_, num_);                     \
    if (0) printf("[%s] [%s] -> [%s] [%.*s]\n", fmt_, N, res_, (int) n, tmp); \
    ASSERT(n == strlen(res_));                                                \
    ASSERT(strcmp(tmp, res_) == 0);                                           \
  } while (0)

    TESTDOUBLE("%g", 0.0, "0");
    TESTDOUBLE("%g", 0.123, "0.123");
    TESTDOUBLE("%g", 0.00123, "0.00123");
    TESTDOUBLE("%g", 0.123456333, "0.123456");
    TESTDOUBLE("%g", 123.0, "123");
    TESTDOUBLE("%g", 11.5454, "11.5454");
    TESTDOUBLE("%g", 11.0001, "11.0001");
    TESTDOUBLE("%g", 0.999, "0.999");
    TESTDOUBLE("%g", 0.999999, "0.999999");
    TESTDOUBLE("%g", 0.9999999, "1");
    TESTDOUBLE("%g", 10.9, "10.9");
    TESTDOUBLE("%g", 10.01, "10.01");
    TESTDOUBLE("%g", 1.0, "1");
    TESTDOUBLE("%g", 10.0, "10");
    TESTDOUBLE("%g", 100.0, "100");
    TESTDOUBLE("%g", 1000.0, "1000");
    TESTDOUBLE("%g", 10000.0, "10000");
    TESTDOUBLE("%g", 100000.0, "100000");
    TESTDOUBLE("%g", 1000000.0, "1e+06");
    TESTDOUBLE("%g", 10000000.0, "1e+07");
    TESTDOUBLE("%g", 100000001.0, "1e+08");
    TESTDOUBLE("%g", 10.5454, "10.5454");
    TESTDOUBLE("%g", 999999.0, "999999");
    TESTDOUBLE("%g", 9999999.0, "1e+07");
    TESTDOUBLE("%g", 44556677.0, "4.45567e+07");
    TESTDOUBLE("%g", 1234567.2, "1.23457e+06");
    TESTDOUBLE("%g", -987.65432, "-987.654");
    TESTDOUBLE("%g", 0.0000000001, "1e-10");
    TESTDOUBLE("%g", 2.34567e-57, "2.34567e-57");
    TESTDOUBLE("%.*g", DBLWIDTH(7, 9999999.0), "9999999");
    TESTDOUBLE("%.*g", DBLWIDTH(10, 0.123456333), "0.123456333");
    TESTDOUBLE("%g", 123.456222, "123.456");
    TESTDOUBLE("%.*g", DBLWIDTH(10, 123.456222), "123.456222");
    TESTDOUBLE("%g", 600.1234, "600.123");
    TESTDOUBLE("%g", -600.1234, "-600.123");
    TESTDOUBLE("%g", 599.1234, "599.123");
    TESTDOUBLE("%g", -599.1234, "-599.123");

#ifndef _WIN32
    TESTDOUBLE("%g", (double) INFINITY, "inf");
    TESTDOUBLE("%g", (double) -INFINITY, "-inf");
    TESTDOUBLE("%g", (double) NAN, "nan");
#else
    TESTDOUBLE("%g", HUGE_VAL, "inf");
    TESTDOUBLE("%g", -HUGE_VAL, "-inf");
#endif
  }
}

static void fn1(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_ERROR) sprintf((char *) fn_data, "%s", (char *) ev_data);
  (void) c;
}

static void test_dns_error(const char *dns_server_url, const char *errstr) {
  // Test timeout
  struct mg_mgr mgr;
  char buf[100] = "";
  int i;
  mg_mgr_init(&mgr);
  mgr.dns4.url = dns_server_url;
  mgr.dnstimeout = 10;
  MG_DEBUG(("opening dummy DNS listener @ [%s]...", dns_server_url));
  mg_listen(&mgr, mgr.dns4.url, NULL, NULL);  // Just discard our queries
  mg_http_connect(&mgr, "http://google.com", fn1, buf);
  for (i = 0; i < 50 && buf[0] == '\0'; i++) mg_mgr_poll(&mgr, 1);
  mg_mgr_free(&mgr);
  // MG_DEBUG(("buf: [%s] [%s]", buf, errstr));
  ASSERT(strcmp(buf, errstr) == 0);
}

static void test_dns(void) {
  struct mg_dns_message dm;
  //       txid  flags numQ  numA  numAP numOP
  // 0000  00 01 81 80 00 01 00 01 00 00 00 00 07 63 65 73  .............ces
  // 0010  61 6e 74 61 03 63 6f 6d 00 00 01 00 01 c0 0c 00  anta.com........
  // 0020  01 00 01 00 00 02 57 00 04 94 fb 36 ec           ......W....6.
  uint8_t data[] = {0,    1,    0x81, 0x80, 0,    1,    0,    1,    0,
                    0,    0,    0,    7,    0x63, 0x65, 0x73, 0x61, 0x6e,
                    0x74, 0x61, 0x03, 0x63, 0x6f, 0x6d, 0,    0,    1,
                    0,    1,    0xc0, 0x0c, 0,    1,    0,    1,    0,
                    0,    2,    0x57, 0,    4,    0x94, 0xfb, 0x36, 0xec};
  ASSERT(mg_dns_parse(NULL, 0, &dm) == 0);
  ASSERT(mg_dns_parse(data, sizeof(data), &dm) == 1);
  ASSERT(strcmp(dm.name, "cesanta.com") == 0);
  data[30] = 29;  // Point a pointer to itself
  memset(&dm, 0, sizeof(dm));
  ASSERT(mg_dns_parse(data, sizeof(data), &dm) == 1);
  ASSERT(strcmp(dm.name, "") == 0);

  {
    // 0000  00 01 81 80 00 01 00 04 00 00 00 00 05 79 61 68  .............yah
    // 0010  6f 6f 05 63 31 31 32 36 03 63 6f 6d 00 00 01 00  oo.c1126.com....
    // 0020  01 c0 0c 00 05 00 01 00 00 0d 34 00 0c 03 77 77  ..........4...ww
    // 0030  77 05 79 61 68 6f 6f c0 18 c0 2d 00 05 00 01 00  w.yahoo...-.....
    // 0040  00 00 01 00 14 0b 6e 65 77 2d 66 70 2d 73 68 65  ......new-fp-she
    // 0050  64 03 77 67 31 01 62 c0 31 c0 45 00 01 00 01 00  d.wg1.b.1.E.....
    // 0060  00 00 0a 00 04 57 f8 64 d8 c0 45 00 01 00 01 00  .....W.d..E.....
    // 0070  00 00 0a 00 04 57 f8 64 d7                       .....W.d.
    uint8_t d[] = {
        0x00, 0x01, 0x81, 0x80, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
        0x00, 0x05, 0x79, 0x61, 0x68, 0x6f, 0x6f, 0x05, 0x63, 0x31, 0x31,
        0x32, 0x36, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01,
        0xc0, 0x0c, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x0d, 0x34, 0x00,
        0x0c, 0x03, 0x77, 0x77, 0x77, 0x05, 0x79, 0x61, 0x68, 0x6f, 0x6f,
        0xc0, 0x18, 0xc0, 0x2d, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x14, 0x0b, 0x6e, 0x65, 0x77, 0x2d, 0x66, 0x70, 0x2d,
        0x73, 0x68, 0x65, 0x64, 0x03, 0x77, 0x67, 0x31, 0x01, 0x62, 0xc0,
        0x31, 0xc0, 0x45, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0a,
        0x00, 0x04, 0x57, 0xf8, 0x64, 0xd8, 0xc0, 0x45, 0x00, 0x01, 0x00,
        0x01, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x04, 0x57, 0xf8, 0x64, 0xd7,
    };
    ASSERT(mg_dns_parse(d, sizeof(d), &dm) == 1);
    // MG_INFO(("[%s]", dm.name));
    ASSERT(strcmp(dm.name, "new-fp-shed.wg1.b.yahoo.com") == 0);
  }

  test_dns_error("udp://127.0.0.1:12345", "DNS timeout");
  test_dns_error("", "resolver");
  test_dns_error("tcp://0.0.0.0:0", "DNS error");
}

static void test_util(void) {
  char buf[100], *p, *s;
  struct mg_addr a;
  memset(&a, 0, sizeof(a));
  ASSERT(mg_file_printf(&mg_fs_posix, "data.txt", "%s", "hi") == true);
  // if (system("ls -l") != 0) (void) 0;
  ASSERT((p = mg_file_read(&mg_fs_posix, "data.txt", NULL)) != NULL);
  ASSERT(strcmp(p, "hi") == 0);
  free(p);
  remove("data.txt");
  ASSERT(mg_aton(mg_str("0"), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0."), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0.256"), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0.-1"), &a) == false);
  ASSERT(mg_aton(mg_str("127.0.0.1"), &a) == true);
  ASSERT(a.is_ip6 == false);
  ASSERT(a.ip == 0x100007f);
  ASSERT(strcmp(mg_ntoa(&a, buf, sizeof(buf)), "127.0.0.1") == 0);

  ASSERT(mg_aton(mg_str("1:2:3:4:5:6:7:8"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(
      memcmp(a.ip6,
             "\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08",
             sizeof(a.ip6)) == 0);

  memset(a.ip6, 0xaa, sizeof(a.ip6));
  ASSERT(mg_aton(mg_str("1::1"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(
      memcmp(a.ip6,
             "\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01",
             sizeof(a.ip6)) == 0);

  memset(a.ip6, 0xaa, sizeof(a.ip6));
  ASSERT(mg_aton(mg_str("::fFff:1.2.3.4"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(memcmp(a.ip6,
                "\x00\x00\x00\x00\x00\x00\x00\x00"
                "\x00\x00\xff\xff\x01\x02\x03\x04",
                sizeof(a.ip6)) == 0);

  memset(a.ip6, 0xaa, sizeof(a.ip6));
  ASSERT(mg_aton(mg_str("::1"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(
      memcmp(a.ip6,
             "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01",
             sizeof(a.ip6)) == 0);

  memset(a.ip6, 0xaa, sizeof(a.ip6));
  ASSERT(mg_aton(mg_str("1::"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(
      memcmp(a.ip6,
             "\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
             sizeof(a.ip6)) == 0);

  memset(a.ip6, 0xaa, sizeof(a.ip6));
  ASSERT(mg_aton(mg_str("2001:4860:4860::8888"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(
      memcmp(a.ip6,
             "\x20\x01\x48\x60\x48\x60\x00\x00\x00\x00\x00\x00\x00\x00\x88\x88",
             sizeof(a.ip6)) == 0);

  ASSERT(strcmp(mg_hex("abc", 3, buf), "616263") == 0);
  ASSERT(mg_url_decode("a=%", 3, buf, sizeof(buf), 0) < 0);
  ASSERT(mg_url_decode("&&&a=%", 6, buf, sizeof(buf), 0) < 0);

  {
    size_t n;
    ASSERT((n = mg_url_encode("", 0, buf, sizeof(buf))) == 0);
    ASSERT((n = mg_url_encode("a", 1, buf, 0)) == 0);
    ASSERT((n = mg_url_encode("a", 1, buf, sizeof(buf))) == 1);
    ASSERT(strncmp(buf, "a", n) == 0);
    ASSERT((n = mg_url_encode("._-~", 4, buf, sizeof(buf))) == 4);
    ASSERT(strncmp(buf, "._-~", n) == 0);
    ASSERT((n = mg_url_encode("a@%>", 4, buf, sizeof(buf))) == 10);
    ASSERT(strncmp(buf, "a%40%25%3e", n) == 0);
    ASSERT((n = mg_url_encode("a@b.c", 5, buf, sizeof(buf))) == 7);
    ASSERT(strncmp(buf, "a%40b.c", n) == 0);
  }

  {
    s = buf;
    mg_asprintf(&s, sizeof(buf), "%3d", 123);
    ASSERT(s == buf);
    ASSERT(strcmp(buf, "123") == 0);
    mg_asprintf(&s, sizeof(buf), "%.*s", 7, "a%40b.c");
    ASSERT(s == buf);
    ASSERT(strcmp(buf, "a%40b.c") == 0);
  }

  ASSERT(mg_to64(mg_str("-9223372036854775809")) == 0);
  ASSERT(mg_to64(mg_str("9223372036854775800")) == 0);
  ASSERT(mg_to64(mg_str("9223372036854775700")) > 0);
  ASSERT(mg_tou64(mg_str("0")) == 0);
  ASSERT(mg_tou64(mg_str("123")) == 123);
  ASSERT(mg_tou64(mg_str("")) == 0);
  ASSERT(mg_tou64(mg_str("-")) == 0);
  ASSERT(mg_tou64(mg_str("18446744073709551615")) == 18446744073709551615U);
}

static void test_crc32(void) {
  //  echo -n aaa | cksum -o3
  ASSERT(mg_crc32(0, 0, 0) == 0);
  ASSERT(mg_crc32(0, "a", 1) == 3904355907);
  ASSERT(mg_crc32(0, "abc", 3) == 891568578);
  ASSERT(mg_crc32(mg_crc32(0, "ab", 2), "c", 1) == 891568578);
}

static void us(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int del = *(int *) fn_data;
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;
  if (ev == MG_EV_HTTP_CHUNK && mg_http_match_uri(hm, "/upload")) {
    MG_DEBUG(("Got chunk len %lu", (unsigned long) hm->chunk.len));
    MG_DEBUG(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
    // MG_DEBUG(("Chunk data:\n%.*s", (int) hm->chunk.len, hm->chunk.ptr));
    if (del) {
      mg_http_delete_chunk(c, hm);
      if (hm->chunk.len == 0) {
        MG_DEBUG(("Last chunk received, sending response"));
        mg_http_reply(c, 200, "", "ok (chunked)\n");
      }
    }
  } else if (ev == MG_EV_HTTP_MSG && mg_http_match_uri(hm, "/upload")) {
    MG_DEBUG(("Got all %lu bytes!", (unsigned long) hm->body.len));
    MG_DEBUG(("Query string: [%.*s]", (int) hm->query.len, hm->query.ptr));
    // MG_DEBUG(("Body:\n%.*s", (int) hm->body.len, hm->body.ptr));
    mg_http_reply(c, 200, "", "ok (%d %.*s)\n", (int) hm->body.len,
                  (int) hm->body.len, hm->body.ptr);
  } else if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "ok\n");
  }
  (void) fn_data;
}

static void uc(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  const char **s = (const char **) fn_data;
  if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c,
              "POST /upload HTTP/1.0\r\n"
              "Transfer-Encoding: chunked\r\n\r\n");
    mg_http_printf_chunk(c, "%s", "foo\n");
    mg_http_printf_chunk(c, "%s", "bar\n");
    mg_http_printf_chunk(c, "");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ASSERT(mg_strcmp(hm->body, mg_str(*s)) == 0);
    *s = NULL;
  }
  (void) fn_data;
}

static void test_http_upload(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12352";
  int i, del = 1;
  const char *s1 = "ok (chunked)\n";
  const char *s2 = "ok (8 foo\nbar\n)\n";

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, us, (void *) &del);

  mg_http_connect(&mgr, url, uc, (void *) &s1);
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 5);
  ASSERT(s1 == NULL);

  del = 0;
  mg_http_connect(&mgr, url, uc, (void *) &s2);
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 5);
  ASSERT(s2 == NULL);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

#define LONG_CHUNK "chunk with length taking up more than two hex digits"

// Streaming server event handler.
// Send one chunk immediately, then drain, then send two chunks in one go
static void eh2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    mg_http_printf_chunk(c, LONG_CHUNK);
    c->label[0] = 1;
  } else if (ev == MG_EV_POLL) {
    if (c->label[0] > 0 && c->send.len == 0) c->label[0]++;
    if (c->label[0] > 10 && c->label[0] != 'x') {
      mg_http_printf_chunk(c, "chunk 1");
      mg_http_printf_chunk(c, "chunk 2");
      mg_http_printf_chunk(c, "");
      c->label[0] = 'x';
    }
  }
  (void) ev_data;
  (void) fn_data;
}

// Non-streaming client event handler. Make sure we've got full body
static void eh3(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET / HTTP/1.0\n\n");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // MG_INFO(("----> [%.*s]", (int) hm->body.len, hm->body.ptr));
    c->is_closing = 1;
    *(uint32_t *) fn_data = mg_crc32(0, hm->body.ptr, hm->body.len);
  }
}

// Streaming client event handler. Make sure we've got all chunks
static void eh4(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  uint32_t *crc = (uint32_t *) c->label;
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET / HTTP/1.0\n\n");
  } else if (ev == MG_EV_HTTP_CHUNK) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    *crc = mg_crc32(*crc, hm->chunk.ptr, hm->chunk.len);
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    *crc = mg_crc32(*crc, hm->body.ptr, hm->body.len);
    // MG_INFO(("MSG [%.*s]", (int) hm->body.len, hm->body.ptr));
    c->is_closing = 1;
    *(uint32_t *) fn_data = *crc;
  }
  (void) ev_data;
}

// Streaming client event handler. Delete chunks as they arrive
static void eh5(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  uint32_t *crc = (uint32_t *) c->label;
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET / HTTP/1.0\n\n");
  } else if (ev == MG_EV_HTTP_CHUNK) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    *crc = mg_crc32(*crc, hm->chunk.ptr, hm->chunk.len);
    // MG_INFO(("CHUNK [%.*s]", (int) hm->chunk.len, hm->chunk.ptr));
    mg_http_delete_chunk(c, hm);
    if (hm->chunk.len == 0) {
      *(uint32_t *) fn_data = *crc;
      c->is_closing = 1;
    }
  }
  (void) ev_data;
}

static void test_http_chunked(void) {
  struct mg_mgr mgr;
  const char *data, *url = "http://127.0.0.1:12344";
  uint32_t i, done = 0;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh2, NULL);

  mg_http_connect(&mgr, url, eh3, &done);
  for (i = 0; i < 50 && done == 0; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(i < 50);
  data = LONG_CHUNK "chunk 1chunk 2";
  ASSERT(done == mg_crc32(0, data, strlen(data)));

  done = 0;
  mg_http_connect(&mgr, url, eh4, &done);
  for (i = 0; i < 50 && done == 0; i++) mg_mgr_poll(&mgr, 1);
  data = LONG_CHUNK LONG_CHUNK "chunk 1chunk 2" LONG_CHUNK "chunk 1chunk 2";
  ASSERT(done == mg_crc32(0, data, strlen(data)));

  done = 0;
  mg_http_connect(&mgr, url, eh5, &done);
  for (i = 0; i < 50 && done == 0; i++) mg_mgr_poll(&mgr, 1);
  data = LONG_CHUNK "chunk 1chunk 2";
  ASSERT(done == mg_crc32(0, data, strlen(data)));

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_invalid_listen_addr(void) {
  struct mg_mgr mgr;
  struct mg_connection *c;
  mg_mgr_init(&mgr);
  c = mg_http_listen(&mgr, "invalid:31:14", eh1, NULL);
  ASSERT(c == NULL);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

struct stream_status {
  uint32_t polls;
  size_t sent;
  size_t received;
  uint32_t send_crc;
  uint32_t recv_crc;
};

// Consume recv buffer after letting it reach MG_MAX_RECV_SIZE
static void eh8(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct stream_status *status = (struct stream_status *) fn_data;
  if (c->is_listening) return;

  ASSERT(c->recv.len <= MG_MAX_RECV_SIZE);

  if (ev == MG_EV_ACCEPT) {
    // Optimize recv buffer size near max to speed up test
    mg_iobuf_resize(&c->recv, MG_MAX_RECV_SIZE - MG_IO_SIZE);
    status->received = 0;
    status->recv_crc = 0;
  }

  if (ev == MG_EV_CLOSE) {
    ASSERT(status->received == status->sent);
  }

  // Let buffer fill up and start consuming after 10 full buffer poll events
  if (status->polls >= 10 && ev == MG_EV_POLL) {
    // consume at most a third of MG_MAX_RECV_SIZE on each poll
    size_t consume;
    if (MG_MAX_RECV_SIZE / 3 >= c->recv.len)
      consume = c->recv.len;
    else
      consume = MG_MAX_RECV_SIZE / 3;
    status->received += consume;
    status->recv_crc =
        mg_crc32(status->recv_crc, (const char *) c->recv.buf, consume);
    mg_iobuf_del(&c->recv, 0, consume);
  }

  // count polls with full buffer to ensure c->is_full prevents reads
  if (ev == MG_EV_POLL && c->recv.len == MG_MAX_RECV_SIZE) status->polls += 1;
  (void) ev_data;
}

// Toggle c->is_full to prevent max_recv_buf_size reached read errors
static void eh10(struct mg_connection *c, int ev, void *ev_data,
                 void *fn_data) {
  if (c->recv.len >= MG_MAX_RECV_SIZE && ev == MG_EV_READ) c->is_full = true;

  eh8(c, ev, ev_data, fn_data);

  if (c->recv.len < MG_MAX_RECV_SIZE && ev == MG_EV_POLL) c->is_full = false;
}

// Send buffer larger than MG_MAX_RECV_SIZE to server
static void eh11(struct mg_connection *c, int ev, void *ev_data,
                 void *fn_data) {
  struct stream_status *status = (struct stream_status *) fn_data;
  if (ev == MG_EV_CONNECT) {
    size_t len = MG_MAX_RECV_SIZE * 2;
    struct mg_iobuf buf = {NULL, 0, 0};
    mg_iobuf_init(&buf, len);
    mg_random(buf.buf, buf.size);
    buf.len = buf.size;
    mg_send(c, buf.buf, buf.len);
    status->sent = buf.len;
    status->send_crc = mg_crc32(0, (const char *) buf.buf, buf.len);
    mg_iobuf_free(&buf);
  }
  (void) ev_data;
  (void) fn_data;
}

static void test_http_stream_buffer(void) {
  struct mg_mgr mgr;
  const char *url = "tcp://127.0.0.1:12344";
  uint32_t i;
  struct stream_status status;
  mg_mgr_init(&mgr);
  mg_listen(&mgr, url, eh10, &status);

  status.polls = 0;
  mg_connect(&mgr, url, eh11, &status);
  for (i = 0; i < (MG_MAX_RECV_SIZE / MG_IO_SIZE) * 50; i++) {
    mg_mgr_poll(&mgr, 1);
    if (status.polls >= 10 && status.sent == status.received) break;
  }
  ASSERT(status.sent == status.received);
  ASSERT(status.send_crc == status.recv_crc);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_multipart(void) {
  struct mg_http_part part;
  size_t ofs;
  const char *s =
      "--xyz\r\n"
      "Content-Disposition: form-data; name=\"val\"\r\n"
      "\r\n"
      "abc\r\ndef\r\n"
      "--xyz\r\n"
      "Content-Disposition: form-data; name=\"foo\"; filename=\"a b.txt\"\r\n"
      "Content-Type: text/plain\r\n"
      "\r\n"
      "hello world\r\n"
      "\r\n"
      "--xyz--\r\n";
  ASSERT(mg_http_next_multipart(mg_str(""), 0, NULL) == 0);
  ASSERT((ofs = mg_http_next_multipart(mg_str(s), 0, &part)) > 0);
  ASSERT(mg_strcmp(part.name, mg_str("val")) == 0);
  // MG_INFO(("--> [%.*s]", (int) part.body.len, part.body.ptr));
  ASSERT(mg_strcmp(part.body, mg_str("abc\r\ndef")) == 0);
  ASSERT(part.filename.len == 0);
  ASSERT((ofs = mg_http_next_multipart(mg_str(s), ofs, &part)) > 0);
  ASSERT(mg_strcmp(part.name, mg_str("foo")) == 0);
  // MG_INFO(("--> [%.*s]", (int) part.filename.len, part.filename.ptr));
  ASSERT(mg_strcmp(part.filename, mg_str("a b.txt")) == 0);
  ASSERT(mg_strcmp(part.body, mg_str("hello world\r\n")) == 0);
  ASSERT(mg_http_next_multipart(mg_str(s), ofs, &part) == 0);
}

static void eh7(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts sopts;
    memset(&sopts, 0, sizeof(sopts));
    sopts.root_dir = "/";
    sopts.fs = &mg_fs_packed;
    mg_http_serve_dir(c, hm, &sopts);
  }
  (void) fn_data;
}

static void test_packed(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12351";
  char buf[FETCH_BUF_SIZE],
      *data = mg_file_read(&mg_fs_posix, "Makefile", NULL);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh7, NULL);

  // Load top level file directly
  fetch(&mgr, buf, url, "GET /Makefile HTTP/1.0\n\n");
  // printf("---> %s\n", buf);
  ASSERT(fetch(&mgr, buf, url, "GET /Makefile HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, data) == 0);
  free(data);

  // Load file deeper in the FS tree directly
  data = mg_file_read(&mg_fs_posix, "src/ssi.h", NULL);
  ASSERT(fetch(&mgr, buf, url, "GET /src/ssi.h HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, data) == 0);
  free(data);

  // List root dir
  ASSERT(fetch(&mgr, buf, url, "GET / HTTP/1.0\n\n") == 200);
  // printf("--------\n%s\n", buf);

  // List nested dir
  ASSERT(fetch(&mgr, buf, url, "GET /test HTTP/1.0\n\n") == 301);
  ASSERT(fetch(&mgr, buf, url, "GET /test/ HTTP/1.0\n\n") == 200);
  // printf("--------\n%s\n", buf);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void eh6(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) *(int *) fn_data = 1;
  (void) c, (void) ev_data;
}

#if defined(__arm__) || defined(__riscv)
int send(int sock, const void *buf, size_t len, int flags);
int send(int sock, const void *buf, size_t len, int flags) {
  (void) sock, (void) buf, (void) len, (void) flags;
  return -1;
}
#endif

static void test_pipe_proto(bool is_udp) {
  struct mg_mgr mgr;
  int i, sock, done = 0;
  mg_mgr_init(&mgr);
  ASSERT((sock = mg_mkpipe(&mgr, eh6, (void *) &done, is_udp)) >= 0);
  ASSERT(send(sock, "hi", 2, 0) == 2);
  for (i = 0; i < 10 && done == 0; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(done == 1);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_pipe(void) {
  test_pipe_proto(true);
  test_pipe_proto(false);
}

static void u1(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_CONNECT) {
    ((int *) fn_data)[0] += 1;
    mg_send(c, "hi", 2);
  } else if (ev == MG_EV_WRITE) {
    ((int *) fn_data)[0] += 100;
  } else if (ev == MG_EV_READ) {
    ((int *) fn_data)[0] += 10;
    mg_iobuf_free(&c->recv);
  }
  (void) ev_data;
}

static void test_udp(void) {
  struct mg_mgr mgr;
  const char *url = "udp://127.0.0.1:12353";
  int i, done = 0;
  mg_mgr_init(&mgr);
  mg_listen(&mgr, url, u1, (void *) &done);
  mg_connect(&mgr, url, u1, (void *) &done);
  for (i = 0; i < 5; i++) mg_mgr_poll(&mgr, 1);
  // MG_INFO(("%d", done));
  ASSERT(done == 111);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_check_ip_acl(void) {
  uint32_t ip = mg_htonl(0x01020304);
  ASSERT(mg_check_ip_acl(mg_str(NULL), ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str(""), ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("invalid"), ip) == -1);
  ASSERT(mg_check_ip_acl(mg_str("+hi"), ip) == -2);
  ASSERT(mg_check_ip_acl(mg_str("+//"), ip) == -2);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0"), ip) == 0);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.0.0.0/8"), ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.2.3.4"), ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.0.0.0/16"), ip) == 0);
}

static void w3(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  // MG_INFO(("ev %d", ev));
  if (ev == MG_EV_WS_OPEN) {
    char buf[8192];
    memset(buf, 'A', sizeof(buf));
    mg_ws_send(c, "hi there!", 9, WEBSOCKET_OP_TEXT);
    mg_printf(c, "%s", "boo");
    mg_ws_wrap(c, 3, WEBSOCKET_OP_TEXT),
        mg_ws_send(c, buf, sizeof(buf), WEBSOCKET_OP_TEXT);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    ASSERT(mg_strcmp(wm->data, mg_str("lebowski")) == 0);
    ((int *) fn_data)[0]++;
  } else if (ev == MG_EV_CLOSE) {
    ((int *) fn_data)[0] += 10;
  }
}

static void w2(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_str msg = mg_str_n("lebowski", 8);
  if (ev == MG_EV_HTTP_MSG) {
    mg_ws_upgrade(c, (struct mg_http_message *) ev_data, NULL);
  } else if (ev == MG_EV_WS_OPEN) {
    mg_ws_send(c, "x", 1, WEBSOCKET_OP_PONG);
  } else if (ev == MG_EV_POLL && c->is_websocket) {
    size_t ofs, n = (size_t) fn_data;
    if (n < msg.len) {
      // Send "msg" char by char using fragmented frames
      // mg_ws_send() sets the FIN flag in the WS header. Clean it
      // to send fragmented packet. Insert PONG messages between frames
      uint8_t op = n == 0 ? WEBSOCKET_OP_TEXT : WEBSOCKET_OP_CONTINUE;
      mg_ws_send(c, ":->", 3, WEBSOCKET_OP_PING);
      ofs = c->send.len;
      mg_ws_send(c, &msg.ptr[n], 1, op);
      if (n < msg.len - 1) c->send.buf[ofs] = op;  // Clear FIN flag
      c->fn_data = (void *) (n + 1);               // Point to the next char
    } else {
      mg_ws_send(c, "", 0, WEBSOCKET_OP_CLOSE);
    }
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (wm->data.len == 9) {
      ASSERT(mg_strcmp(wm->data, mg_str("hi there!")) == 0);
    } else if (wm->data.len == 3) {
      ASSERT(mg_strcmp(wm->data, mg_str("boo")) == 0);
    } else {
      ASSERT(wm->data.len == 8192);
    }
  }
}

static void test_ws_fragmentation(void) {
  const char *url = "ws://localhost:12357/ws";
  struct mg_mgr mgr;
  int i, done = 0;

  mg_mgr_init(&mgr);
  ASSERT(mg_http_listen(&mgr, url, w2, NULL) != NULL);
  mg_ws_connect(&mgr, url, w3, &done, "%s", "Sec-WebSocket-Protocol: echo\r\n");
  for (i = 0; i < 25; i++) mg_mgr_poll(&mgr, 1);
  // MG_INFO(("--> %d", done));
  ASSERT(done == 11);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void h7(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.root_dir = "./test/data,/foo=./src";
    mg_http_serve_dir(c, hm, &opts);
  }
  (void) fn_data;
}

static void test_rewrites(void) {
  char buf[FETCH_BUF_SIZE];
  const char *url = "http://LOCALHOST:12358";
  const char *expected = "#define MG_VERSION \"" MG_VERSION "\"\n";
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  ASSERT(mg_http_listen(&mgr, url, h7, NULL) != NULL);
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /foo/version.h HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, expected) == 0);
  ASSERT(fetch(&mgr, buf, url, "GET /foo HTTP/1.0\n\n") == 301);
  ASSERT(fetch(&mgr, buf, url, "GET /foo/ HTTP/1.0\n\n") == 200);
  // printf("-->[%s]\n", buf);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_get_header_var(void) {
  struct mg_str empty = mg_str(""), bar = mg_str("bar"), baz = mg_str("baz");
  struct mg_str header = mg_str("Digest foo=\"bar\", blah,boo=baz, x=\"yy\"");
  struct mg_str yy = mg_str("yy");
  // struct mg_str x = mg_http_get_header_var(header, mg_str("x"));
  // MG_INFO(("--> [%d] [%d]", (int) x.len, yy.len));
  ASSERT(mg_strcmp(empty, mg_http_get_header_var(empty, empty)) == 0);
  ASSERT(mg_strcmp(empty, mg_http_get_header_var(header, empty)) == 0);
  ASSERT(mg_strcmp(empty, mg_http_get_header_var(header, mg_str("fooo"))) == 0);
  ASSERT(mg_strcmp(empty, mg_http_get_header_var(header, mg_str("fo"))) == 0);
  ASSERT(mg_strcmp(empty, mg_http_get_header_var(header, mg_str("blah"))) == 0);
  ASSERT(mg_strcmp(bar, mg_http_get_header_var(header, mg_str("foo"))) == 0);
  ASSERT(mg_strcmp(baz, mg_http_get_header_var(header, mg_str("boo"))) == 0);
  ASSERT(mg_strcmp(yy, mg_http_get_header_var(header, mg_str("x"))) == 0);
}

static void test_json(void) {
  const char *s;
  const char *s1 = "{\"a\":{},\"b\":7,\"c\":[[],2]}";
  const char *s2 = "{\"a\":{\"b1\":{}},\"c\":7}";
  int n, n1 = (int) strlen(s1), n2 = (int) strlen(s2);

  ASSERT(mg_json_get(" true ", 6, "", &n) == MG_JSON_INVALID);
  ASSERT(mg_json_get(" true ", 6, "$", &n) == 1 && n == 4);
  ASSERT(mg_json_get("null ", 5, "$", &n) == 0 && n == 4);
  s = "  \"hi\\nthere\"";
  ASSERT(mg_json_get(s, (int) strlen(s), "$", &n) == 2 && n == 11);
  ASSERT(mg_json_get(" { } ", 5, "$", &n) == 1);
  ASSERT(mg_json_get(" [[]]", 5, "$", &n) == 1);
  ASSERT(mg_json_get(" [ ]  ", 5, "$", &n) == 1);

  ASSERT(mg_json_get("[1,2]", 5, "$", &n) == 0 && n == 5);
  ASSERT(mg_json_get("[1,2]", 5, "$[0]", &n) == 1 && n == 1);
  ASSERT(mg_json_get("[1,2]", 5, "$[1]", &n) == 3 && n == 1);
  ASSERT(mg_json_get("[1,2]", 5, "$[3]", &n) == MG_JSON_NOT_FOUND);

  s = "{\"a\":[]}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":[1,2]}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":[1,[1]]}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":[[]]}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":[[1,2]]}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":{}}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":{\"a\":{}}}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);
  s = "{\"a\":{\"a\":[]}}";
  ASSERT(mg_json_get(s, (int) strlen(s), "$.a", &n) == 5);

  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$", &n) == 0 && n == 13);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0]", &n) == 1 && n == 9);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[1]", &n) == 11);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[1]", &n) == 11 && n == 1);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0][0]", &n) == 2 && n == 1);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0][1]", &n) == 4 && n == 5);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0][2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0][1][0]", &n) == 5 && n == 1);
  ASSERT(mg_json_get("[[1,[2,3]],4]", 13, "$[0][1][1]", &n) == 7 && n == 1);

  ASSERT(mg_json_get("[[1,2],3]", 9, "$", &n) == 0 && n == 9);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[0][0]", &n) == 2 && n == 1);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[0][1]", &n) == 4 && n == 1);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[0][2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[1][0]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[1]", &n) == 7 && n == 1);
  ASSERT(mg_json_get("[[1,2],3]", 9, "$[1][0]", &n) == MG_JSON_NOT_FOUND);

  ASSERT(mg_json_get("[1,[2,3]]", 9, "$", &n) == 0 && n == 9);
  ASSERT(mg_json_get("[1,[2,3]]", 9, "$[0][1]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get("[1,[2,3]]", 9, "$[1][0]", &n) == 4 && n == 1);

  ASSERT(mg_json_get(s1, n1, "$.a", &n) == 5 && n == 2);
  ASSERT(mg_json_get(s1, n1, "$.b", &n) == 12 && n == 1);
  ASSERT(mg_json_get(s1, n1, "$.c", &n) == 18 && n == 6);
  ASSERT(mg_json_get(s1, n1, "$.c[0]", &n) == 19 && n == 2);
  ASSERT(mg_json_get(s1, n1, "$.c[1]", &n) == 22 && n == 1);
  ASSERT(mg_json_get(s1, n1, "$.c[3]", &n) == MG_JSON_NOT_FOUND);

  ASSERT(mg_json_get(s2, n2, "$.a", &n) == 5 && n == 9);
  ASSERT(mg_json_get(s2, n2, "$.a.b1", &n) == 11 && n == 2);
  ASSERT(mg_json_get(s2, n2, "$.a.b2", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(s2, n2, "$.a.b", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(s2, n2, "$.a1", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(s2, n2, "$.c", &n) == 19 && n == 1);

  {
    double d = 0;
    bool b = false;
    const char *json = "{\"a\": \"hi\\nthere\",\"b\": [12345, true]}";
    char *str = mg_json_get_str(mg_str(json), "$.a");

    ASSERT(str != NULL);
    ASSERT(strcmp(str, "hi\nthere") == 0);
    free(str);

    ASSERT(mg_json_get_num(mg_str(json), "$.a", &d) == false);
    ASSERT(mg_json_get_num(mg_str(json), "$.c", &d) == false);
    ASSERT(mg_json_get_num(mg_str(json), "$.b[0]", &d) == true);
    ASSERT(d == 12345);

    ASSERT(mg_json_get_bool(mg_str(json), "$.b", &b) == false);
    ASSERT(mg_json_get_bool(mg_str(json), "$.b[0]", &b) == false);
    ASSERT(mg_json_get_bool(mg_str(json), "$.b[1]", &b) == true);
    ASSERT(b == true);
  }
}

int main(void) {
  const char *debug_level = getenv("V");
  if (debug_level == NULL) debug_level = "3";
  mg_log_set(debug_level);

  test_json();
  test_str();
  test_globmatch();
  test_get_header_var();
  test_rewrites();
  test_check_ip_acl();
  test_udp();
  test_pipe();
  test_packed();
  test_crc32();
  test_multipart();
  test_invalid_listen_addr();
  test_http_chunked();
  test_http_upload();
  test_http_stream_buffer();
  test_http_parse();
  test_util();
  test_sntp();
  test_dns();
  test_timer();
  test_url();
  test_iobuf();
  test_commalist();
  test_base64();
  test_http_get_var();
  test_tls();
  test_ws();
  test_ws_fragmentation();
  test_http_client();
  test_http_server();
  test_http_404();
  test_http_no_content_length();
  test_http_pipeline();
  test_http_range();
  test_mqtt();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
