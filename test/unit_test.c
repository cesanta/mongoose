#include "mongoose.h"

#include "float.h"  // For DBL_EPSILON and HUGE_VAL
#include "math.h"

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

#define FETCH_BUF_SIZE (256 * 1024)

// Important: we use different port numbers for the Windows bug workaround. See
// https://support.microsoft.com/en-ae/help/3039044/error-10013-wsaeacces-is-returned-when-a-second-bind-to-a-excluded-por

static void test_match(void) {
  ASSERT(mg_match(mg_str_n("", 0), mg_str_n("", 0), NULL) == 1);
  ASSERT(mg_match(mg_str_n("a", 1), mg_str_n("*", 1), NULL) == 1);
  ASSERT(mg_match(mg_str_n("ab", 2), mg_str_n("*", 1), NULL) == 1);
  ASSERT(mg_match(mg_str_n("a", 1), mg_str_n("", 0), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/foo", 4), mg_str_n("/", 1), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/x/bar", 6), mg_str_n("/*/foo", 6), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/x/foo", 6), mg_str_n("/*/foo", 6), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/x/foox", 7), mg_str_n("/*/foo", 6), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/x/foox", 7), mg_str_n("/*/foo*", 7), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/abc", 4), mg_str_n("/*", 2), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/ab/", 4), mg_str_n("/*", 2), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/", 1), mg_str_n("/*", 2), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/x/2", 4), mg_str_n("/x/*", 4), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/x/2/foo", 8), mg_str_n("/x/*", 4), NULL) == 0);
  ASSERT(mg_match(mg_str_n("/x/2/foo", 8), mg_str_n("/x/*/*", 6), NULL) == 1);
  ASSERT(mg_match(mg_str_n("///", 3), mg_str_n("#", 1), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/api/foo", 8), mg_str_n("/api/*", 6), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/api/log/static", 15), mg_str_n("/api/*", 6),
                  NULL) == 0);
  ASSERT(mg_match(mg_str_n("/api/log/static", 15), mg_str_n("/api/#", 6),
                  NULL) == 1);
  ASSERT(mg_match(mg_str_n("/ssi/index.shtml", 16), mg_str_n("#.shtml", 7),
                  NULL) == 1);
  ASSERT(mg_match(mg_str_n(".c", 2), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("ab", 2), mg_str_n("abc", 3), NULL) == 0);
  ASSERT(mg_match(mg_str_n("a.c", 3), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("..c", 3), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("/.c", 3), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("//a.c", 5), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("x/a.c", 5), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("./a.c", 5), mg_str_n("#.c", 3), NULL) == 1);
  ASSERT(mg_match(mg_str_n("./ssi/index.shtml", 17), mg_str_n("#.shtml", 7),
                  NULL) == 1);
  ASSERT(mg_match(mg_str_n("caabba", 6), mg_str_n("#aa#bb#", 7), NULL) == 1);
  ASSERT(mg_match(mg_str_n("caabxa", 6), mg_str_n("#aa#bb#", 7), NULL) == 0);
  ASSERT(mg_match(mg_str_n("a__b_c", 6), mg_str_n("a*b*c", 5), NULL) == 1);

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

    ASSERT(mg_match(mg_str("a#c"), mg_str("?#"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("a")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("#c")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);

    ASSERT(mg_match(mg_str("a*c"), mg_str("?*"), caps) == true);
    ASSERT(mg_strcmp(caps[0], mg_str("a")) == 0);
    ASSERT(mg_strcmp(caps[1], mg_str("*c")) == 0);
    ASSERT(mg_strcmp(caps[2], mg_str("")) == 0);
  }
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
  return mg_strcmp(s1, mg_str(s2)) == 0;
}
static bool is_space(int c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}
static struct mg_str strstrip(struct mg_str s) {
  while (s.len > 0 && is_space((int) *s.buf)) s.buf++, s.len--;
  while (s.len > 0 && is_space((int) *(s.buf + s.len - 1))) s.len--;
  return s;
}
static const char *mgstrstr(const struct mg_str haystack,
                            const struct mg_str needle) {
  size_t i;
  if (needle.len > haystack.len) return NULL;
  if (needle.len == 0) return haystack.buf;
  for (i = 0; i <= haystack.len - needle.len; i++) {
    if (memcmp(haystack.buf + i, needle.buf, needle.len) == 0) {
      return haystack.buf + i;
    }
  }
  return NULL;
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
  ASSERT(vcmp(mg_url_user("p://foo/q?mail=a@b.c"), ""));
  ASSERT(vcmp(mg_url_pass("p://foo/q?mail=a@b.c"), ""));

  // URI
  ASSERT(strcmp(mg_url_uri("p://foo"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo/"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/"), "/") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/abc"), "/abc") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo:12/a/b/c"), "/a/b/c") == 0);
  ASSERT(strcmp(mg_url_uri("p://[::1]:12/a/b/c"), "/a/b/c") == 0);
  ASSERT(strcmp(mg_url_uri("p://[ab::1]:12/a/b/c"), "/a/b/c") == 0);
  ASSERT(strcmp(mg_url_uri("p://foo/q?mail=a@b.c"), "/q?mail=a@b.c") == 0);
}

static void test_base64(void) {
  char buf[128];

  ASSERT(mg_base64_encode((uint8_t *) "", 0, buf, sizeof(buf)) == 0);
  ASSERT(strcmp(buf, "") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "x", 1, buf, sizeof(buf)) == 4);
  ASSERT(strcmp(buf, "eA==") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "xyz", 3, buf, sizeof(buf)) == 4);
  ASSERT(strcmp(buf, "eHl6") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "abcdef", 6, buf, sizeof(buf)) == 8);
  ASSERT(strcmp(buf, "YWJjZGVm") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "ы", 2, buf, sizeof(buf)) == 4);
  ASSERT(strcmp(buf, "0Ys=") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "xy", 3, buf, sizeof(buf)) == 4);
  ASSERT(strcmp(buf, "eHkA") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "test", 4, buf, sizeof(buf)) == 8);
  ASSERT(strcmp(buf, "dGVzdA==") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "abcde", 5, buf, sizeof(buf)) == 8);
  ASSERT(strcmp(buf, "YWJjZGU=") == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 0) == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 1) == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 2) == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 3) == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 4) == 0);
  ASSERT(mg_base64_encode((uint8_t *) "a", 1, buf, 5) == 4);

  ASSERT(mg_base64_decode("кю", 4, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("A", 1, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("A=", 2, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("AA=", 3, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf, sizeof(buf)) == 2);
  ASSERT(mg_base64_decode("AAAA====", 8, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("AAAA----", 8, buf, sizeof(buf)) == 0);
  ASSERT(mg_base64_decode("Q2VzYW50YQ==", 12, buf, sizeof(buf)) == 7);
  ASSERT(strcmp(buf, "Cesanta") == 0);

  ASSERT(mg_base64_decode("AAA=", 4, buf, 0) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf, 1) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf, 2) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf, 3) == 0);
  ASSERT(mg_base64_decode("AAA=", 4, buf, 4) == 2);
}

static void test_iobuf(void) {
  struct mg_iobuf io = {0, 0, 0, 10};
  ASSERT(io.buf == NULL && io.size == 0 && io.len == 0);
  mg_iobuf_resize(&io, 1);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 0);
  ASSERT(memcmp(io.buf, "\x00", 1) == 0);
  mg_iobuf_add(&io, 3, "hi", 2);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 5);
  ASSERT(memcmp(io.buf, "\x00\x00\x00hi", 5) == 0);
  mg_iobuf_add(&io, io.len, "!", 1);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 6);
  ASSERT(memcmp(io.buf, "\x00\x00\x00hi!", 6) == 0);
  mg_iobuf_add(&io, 0, "x", 1);
  ASSERT(memcmp(io.buf, "x\x00\x00\x00hi!", 7) == 0);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 7);
  mg_iobuf_del(&io, 1, 3);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 4);
  ASSERT(memcmp(io.buf, "xhi!", 3) == 0);
  mg_iobuf_del(&io, 10, 100);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 4);
  ASSERT(memcmp(io.buf, "xhi!", io.len) == 0);
  mg_iobuf_add(&io, io.len, "123456", 6);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 10);
  mg_iobuf_add(&io, io.len, "a", 1);
  ASSERT(io.buf != NULL && io.size == 20 && io.len == 11);
  ASSERT(memcmp(io.buf, "xhi!123456a", io.len) == 0);
  mg_iobuf_free(&io);
}

static void sntp_cb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_SNTP_TIME) {
    int64_t received = *(int64_t *) ev_data;
    *(int64_t *) c->fn_data = received;
    MG_DEBUG(("got time: %lld", received));
#if MG_ARCH == MG_ARCH_UNIX
    struct timeval tv = {0, 0};
    gettimeofday(&tv, 0);
    int64_t ms = (int64_t) tv.tv_sec * 1000 + tv.tv_usec / 1000;
    int64_t diff = ms > received ? ms - received : received - ms;
    MG_DEBUG(("diff: %lld", diff));
    // ASSERT(diff < 100);
#endif
  } else if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  }
  (void) c;
}

static bool test_sntp_server(const char *url) {
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
  mg_mgr_free(&mgr);
  return ms > 0;
}

static void test_sntp(void) {
  bool result;
  const unsigned char bad[] =
      "\x55\x02\x00\xeb\x00\x00\x00\x1e\x00\x00\x07\xb6\x3e\xc9\xd6\xa2"
      "\xdb\xde\xea\x30\x91\x86\xb7\x10\xdb\xde\xed\x98\x00\x00\x00\xde"
      "\xdb\xde\xed\x99\x0a\xe2\xc7\x96\xdb\xde\xed\x99\x0a\xe4\x6b\xda";

  ASSERT(mg_sntp_parse(bad, sizeof(bad)) < 0);
  ASSERT(mg_sntp_parse(NULL, 0) == -1);
  // NOTE(): historical NTP port blockage issue; expect at least one to be
  // reachable and work. https://github.com/actions/runner-images/issues/5615
  result = test_sntp_server("udp://time.apple.com:123") ||
           test_sntp_server("udp://time.windows.com:123") ||
           test_sntp_server(NULL);
#if defined(NO_SNTP_CHECK)
  (void) result;
#else
  ASSERT(result);
#endif
}

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
    "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
    "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
    "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
    "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
    "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
    "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
    "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
    "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
    "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
    "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
    "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
    "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
    "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
    "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
    "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
    "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
    "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
    "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
    "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
    "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
    "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
    "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
    "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
    "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
    "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
    "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
    "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
    "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
    "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
    "-----END CERTIFICATE-----\n";
#elif MG_TLS
#ifdef MQTT_LOCALHOST
#define MQTTS_URL "mqtts://127.0.0.1:8883"
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

struct mqtt_data {
  char *topic;
  char *msg;
  size_t topicsize;
  size_t msgsize;
  int flags;
};
#define flags_subscribed (1 << 0)
#define flags_published (1 << 1)
#define flags_received (1 << 2)
#define flags_released (1 << 3)
#define flags_completed (1 << 4)

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data) {
  struct mqtt_data *test_data = (struct mqtt_data *) c->fn_data;
  char *buf = test_data->msg;
#if MG_TLS
  if (c->is_tls && ev == MG_EV_CONNECT) {
    struct mg_tls_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.ca = MQTTS_CA;
#if defined( MQTT_LOCALHOST) && MG_TLS != MG_TLS_BUILTIN
    MG_ERROR(("Hostname not tested"));
#else
    opts.name = mg_url_host(MQTTS_URL);
#endif
    mg_tls_init(c, &opts);
  } else
#endif
      if (ev == MG_EV_MQTT_OPEN) {
    buf[0] = *(int *) ev_data == 0 ? 'X' : 'Y';
  } else if (ev == MG_EV_CLOSE) {
    buf[0] = 0;
  } else if (ev == MG_EV_MQTT_CMD) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    if (mm->cmd == MQTT_CMD_SUBACK) {
      test_data->flags = flags_subscribed;
    } else if (mm->cmd == MQTT_CMD_PUBACK) {  // here we assume the broker
      test_data->flags = flags_published;     // reported no errors,
    } else if (mm->cmd == MQTT_CMD_PUBREC) {  // either no var header or
      test_data->flags |= flags_received;     // reason code 0x00
    } else if (mm->cmd == MQTT_CMD_PUBREL) {
      test_data->flags |= flags_released;
    } else if (mm->cmd == MQTT_CMD_PUBCOMP) {
      test_data->flags |= flags_completed;
    }
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    snprintf(test_data->topic, test_data->topicsize, "%.*s",
             (int) mm->topic.len, mm->topic.buf);
    snprintf(buf + 1, test_data->msgsize - 2, "%.*s", (int) mm->data.len,
             mm->data.buf);

    if (mm->cmd == MQTT_CMD_PUBLISH && c->is_mqtt5) {
      size_t pos = 0, i = 0, j = 0;
      struct mg_mqtt_prop prop;

      for (i = 0; i < 5 ; i++) {
        ASSERT((pos = mg_mqtt_next_prop(mm, &prop, pos)) > 0);
        if (prop.id == MQTT_PROP_MESSAGE_EXPIRY_INTERVAL) {
          ASSERT(prop.iv == 10);
          j += 1;
        } else if (prop.id == MQTT_PROP_PAYLOAD_FORMAT_INDICATOR) {
          j += 2;
          continue;
        } else if (prop.id == MQTT_PROP_CONTENT_TYPE) {
          ASSERT(strncmp(prop.val.buf, "test_content_val_2", prop.val.len) == 0 && prop.val.len == strlen("test_content_val_2"));
          j += 4;
        } else if (prop.id == MQTT_PROP_USER_PROPERTY) {
          if (strncmp(prop.key.buf, "test_key_1", prop.key.len) == 0 && prop.key.len == strlen("test_key_1")) {
            ASSERT(strncmp(prop.val.buf, "test_value_1", prop.val.len) == 0 && prop.val.len == strlen("test_value_1"));
            j += 8;
          } else if (strncmp(prop.key.buf, "test_key_2", prop.key.len) == 0 && prop.key.len == strlen("test_key_2")) {
            ASSERT(strncmp(prop.val.buf, "test_value_2", prop.val.len) == 0 && prop.val.len == strlen("test_value_2"));
            j += 16;
          } else {
            ASSERT(0);
          }
        }
      }
      ASSERT((pos = mg_mqtt_next_prop(mm, &prop, pos)) == 0);
      ASSERT(j == 31);
    }
  }
  (void) c;
}

static void construct_props(struct mg_mqtt_prop *props) {
  props[0].id = MQTT_PROP_MESSAGE_EXPIRY_INTERVAL;
  props[0].iv = 10;

  props[1].id = MQTT_PROP_USER_PROPERTY;
  props[1].key = mg_str("test_key_1");
  props[1].val = mg_str("test_value_1");

  props[2].id = MQTT_PROP_USER_PROPERTY;
  props[2].key = mg_str("test_key_2");
  props[2].val = mg_str("test_value_2");

  props[3].id = MQTT_PROP_CONTENT_TYPE;
  props[3].val = mg_str("test_content_val_2");

  props[4].id = MQTT_PROP_PAYLOAD_FORMAT_INDICATOR;
  props[4].iv = 1;
}

static void test_mqtt_base(void);
static void test_mqtt_base(void) {
  char buf[10] = {0};  // we won't use it
  struct mqtt_data test_data = {buf, buf, 10, 10, 0};
  struct mg_mgr mgr;
  struct mg_connection *c;
  const char *url = MQTT_URL;
  int i;
  mg_mgr_init(&mgr);

  // Ping the client
  c = mg_mqtt_connect(&mgr, url, NULL, mqtt_cb, &test_data);
  mg_mqtt_ping(c);
  for (i = 0; i < 300 && !(c->is_client && !c->is_connecting); i++)
    mg_mgr_poll(&mgr, 10);
  ASSERT(c->is_client && !c->is_connecting);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void check_mqtt_message(struct mg_mqtt_opts *opts,
                               struct mqtt_data *data, bool enforce) {
  if (opts->topic.len != strlen(data->topic) ||
      strcmp(opts->topic.buf, data->topic)) {
    MG_INFO(("TOPIC[%s]", data->topic));
    if (enforce) ASSERT(0);
  }
  if (*data->msg != 'X' || opts->message.len != (strlen(&data->msg[1])) ||
      strcmp(opts->message.buf, &data->msg[1])) {
    MG_INFO(("MSG[%s]", data->msg));
    if (enforce) ASSERT(0);
  }
}

// generate a random string ending in three digits taken from current time
static struct mg_str genstring(char *t, unsigned int sz) {
  mg_random_str(t, sz - 3);
  snprintf(t + sz - 4, 4, "%03u", (unsigned int) mg_millis() % 1000);
  return mg_str(t);
}

static void test_mqtt_basic(void) {
  char tbuf[16], mbuf[50] = {0}, topic[16];
  struct mqtt_data test_data = {tbuf, mbuf, 16, 50, 0};
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_mqtt_opts opts;
#if MG_TLS
  const char *url = MQTTS_URL;
#else
  const char *url = MQTT_URL;
#endif
  int i, retries;

  // Connect with empty client ID, no options, ergo MQTT = 3.1.1
  mg_mgr_init(&mgr);
  c = mg_mqtt_connect(&mgr, url, NULL, mqtt_cb, &test_data);
  for (i = 0; i < 300 && mbuf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (mbuf[0] != 'X') MG_INFO(("[%s]", mbuf));
  ASSERT(mbuf[0] == 'X');
  ASSERT(test_data.flags == 0);

  // Subscribe with QoS1
  opts.topic = genstring(topic, sizeof(topic));
  opts.qos = 1;
  mg_mqtt_sub(c, &opts);
  for (i = 0; i < 500 && test_data.flags == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.flags == flags_subscribed);
  test_data.flags = 0;

  // Publish with QoS0 to subscribed topic and check reception
  // keep former opts.topic
  opts.message = mg_str("hi0"), opts.qos = 0, opts.retain = false;
  mg_mqtt_pub(c, &opts);
  for (i = 0; i < 500 && mbuf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(!(test_data.flags & flags_published));  // No PUBACK for QoS0
  check_mqtt_message(&opts, &test_data, false);  // We may not get the msg
  memset(mbuf + 1, 0, sizeof(mbuf) - 1);
  test_data.flags = 0;

  // Publish with QoS1 to subscribed topic and check reception
  // keep former opts.topic
  opts.message = mg_str("hi1"), opts.qos = 1, opts.retain = false,
  opts.retransmit_id = 0;
  retries = 0;  // don't do retries for test speed
  do {          // retry on failure after an expected timeout
    opts.retransmit_id = mg_mqtt_pub(c, &opts);  // save id for possible resend
    for (i = 0; i < 500 && test_data.flags == 0; i++) mg_mgr_poll(&mgr, 10);
  } while (test_data.flags == 0 && retries--);
  ASSERT(test_data.flags == flags_published);
  for (i = 0; i < 500 && mbuf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  check_mqtt_message(&opts, &test_data, true);
  memset(mbuf + 1, 0, sizeof(mbuf) - 1);
  test_data.flags = 0;
  opts.retransmit_id = 0;

#if MG_TLS
  // send more than 1 record, content is not relevant
  {
    static char somedata[21098];
    mg_random(somedata, sizeof(somedata));
    opts.message = mg_str_n(somedata, sizeof(somedata));
  }
  opts.qos = 1, opts.retain = false, opts.retransmit_id = 0;
  mg_mqtt_pub(c, &opts);
  tbuf[0] = 0;
  for (i = 0; i < 1000 && test_data.flags == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.flags == flags_published);
  for (i = 0; i < 1000 && tbuf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(tbuf[0] != 0);  // just check we were able to send and receive
  memset(mbuf + 1, 0, sizeof(mbuf) - 1);
  test_data.flags = 0;
  opts.retransmit_id = 0;
#endif

  // Clean Disconnect !
  mg_mqtt_disconnect(c, NULL);
  for (i = 0; i < 10 && mbuf[0] != 0; i++) mg_mgr_poll(&mgr, 10);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_mqtt_ver(uint8_t mqtt_version) {
  char tbuf[16], mbuf[50] = {0}, client_id[16], topic[16];
  struct mqtt_data test_data = {tbuf, mbuf, 16, 50, 0};
  struct mg_mgr mgr;
  struct mg_connection *c;
  struct mg_mqtt_opts opts;
  struct mg_mqtt_prop properties[5];
  const char *url = MQTT_URL;
  int i, retries;

  MG_DEBUG(("ver: %u", mqtt_version));
  // Connect with options: version, clean session, last will, keepalive
  // time. Don't set retain, some runners are not random
  test_data.flags = 0;
  memset(mbuf, 0, sizeof(mbuf));
  memset(&opts, 0, sizeof(opts));
  mg_mgr_init(&mgr);

  opts.clean = true, opts.qos = 1, opts.retain = false, opts.keepalive = 20;
  opts.version = mqtt_version;
  opts.topic = genstring(topic, sizeof(topic));
  opts.message = mg_str("mg_will_messsage");
  opts.client_id = genstring(client_id, sizeof(client_id));
  c = mg_mqtt_connect(&mgr, url, &opts, mqtt_cb, &test_data);
  for (i = 0; i < 500 && mbuf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  if (mbuf[0] != 'X') MG_INFO(("[%s]", mbuf));
  ASSERT(mbuf[0] == 'X');
  ASSERT(test_data.flags == 0);

  // Subscribe with QoS2 (reception downgrades to published QoS)
  opts.topic = genstring(topic, sizeof(topic));
  opts.qos = 2;
  mg_mqtt_sub(c, &opts);
  for (i = 0; i < 500 && test_data.flags == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.flags == flags_subscribed);
  test_data.flags = 0;

  // Publish with QoS1 to subscribed topic and check reception
  // keep former opts.topic
  opts.message = mg_str("hi1"), opts.qos = 1, opts.retain = false;
  if (mqtt_version == 5) {
    opts.props = properties;
    opts.num_props = 5;
    construct_props(properties);
  }
  retries = 0;  // don't do retries for test speed
  do {          // retry on failure after an expected timeout
    opts.retransmit_id = mg_mqtt_pub(c, &opts);  // save id for possible resend
    for (i = 0; i < 500 && test_data.flags == 0; i++) mg_mgr_poll(&mgr, 10);
  } while (test_data.flags == 0 && retries--);
  ASSERT(test_data.flags == flags_published);
  for (i = 0; i < 500 && mbuf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  check_mqtt_message(&opts, &test_data, true);
  memset(mbuf + 1, 0, sizeof(mbuf) - 1);
  test_data.flags = 0;
  opts.retransmit_id = 0;

  // Publish with QoS2 to subscribed topic and check (simultaneous) reception
  // keep former opts.topic
  opts.message = mg_str("hi2"), opts.qos = 2, opts.retain = false;
  if (mqtt_version == 5) {
    opts.props = properties;
    opts.num_props = 5;
    construct_props(properties);
  }
  retries = 0;  // don't do retries for test speed
  do {          // retry on failure after an expected timeout
    opts.retransmit_id = mg_mqtt_pub(c, &opts);  // save id for possible resend
    for (i = 0; i < 500 && !(test_data.flags & flags_received); i++)
      mg_mgr_poll(&mgr, 10);
  } while (!(test_data.flags & flags_received) && retries--);
  ASSERT(test_data.flags & flags_received);
  test_data.flags &= ~flags_received;
  opts.retransmit_id = 0;
  // Mongoose sent PUBREL, wait for PUBCOMP
  for (i = 0; i < 500 && !(test_data.flags & flags_completed); i++)
    mg_mgr_poll(&mgr, 10);
  // TODO(): retry sending PUBREL on failure after an expected timeout
  // or broker sends PUBREC again
  ASSERT(test_data.flags & flags_completed);
  for (i = 0; i < 500 && mbuf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  check_mqtt_message(&opts, &test_data, true);
  for (i = 0; i < 500 && !(test_data.flags & flags_released); i++)
    mg_mgr_poll(&mgr, 10);
  ASSERT(test_data.flags & flags_released);  // Mongoose sent PUBCOMP
  memset(mbuf + 1, 0, sizeof(mbuf) - 1);
  test_data.flags = 0;

  // dirty disconnect
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_mqtt(void) {
  test_mqtt_base();
#ifdef NO_MQTT_TESTS
  MG_ERROR(("MQTT tests skipped on request"));
  (void) test_mqtt_basic, (void) test_mqtt_ver;
#else
  test_mqtt_basic();
  test_mqtt_ver(4);
  test_mqtt_ver(5);
#endif
}

static void eh1(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_tls_opts *topts = (struct mg_tls_opts *) c->fn_data;
  if (ev == MG_EV_ACCEPT && topts != NULL) mg_tls_init(c, topts);
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("[%.*s %.*s] message len %d", (int) hm->method.len, hm->method.buf,
             (int) hm->uri.len, hm->uri.buf, (int) hm->message.len));
    if (mg_match(hm->uri, mg_str("/foo/*"), NULL)) {
      mg_http_reply(c, 200, "", "uri: %.*s", hm->uri.len - 5, hm->uri.buf + 5);
    } else if (mg_match(hm->uri, mg_str("/ws"), NULL)) {
      mg_ws_upgrade(c, hm, NULL);
    } else if (mg_match(hm->uri, mg_str("/body"), NULL)) {
      mg_http_reply(c, 200, "", "%.*s", (int) hm->body.len, hm->body.buf);
    } else if (mg_match(hm->uri, mg_str("/bar"), NULL)) {
      mg_http_reply(c, 404, "", "not found");
    } else if (mg_match(hm->uri, mg_str("/no_reason"), NULL)) {
      mg_printf(c, "%s", "HTTP/1.0 200\r\nContent-Length: 2\r\n\r\nok");
    } else if (mg_match(hm->uri, mg_str("/badroot"), NULL)) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = "/BAAADDD!";
      mg_http_serve_dir(c, hm, &sopts);
    } else if (mg_match(hm->uri, mg_str("/creds"), NULL)) {
      char user[100], pass[100];
      mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
      mg_http_reply(c, 200, "", "[%s]:[%s]", user, pass);
    } else if (mg_match(hm->uri, mg_str("/upload"), NULL)) {
      mg_http_upload(c, hm, &mg_fs_posix, ".", 99999);
      c->is_hexdumping = 1;
    } else if (mg_match(hm->uri, mg_str("/dirtest/"), NULL)) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = ".";
      sopts.extra_headers = "A: B\r\nE: F\r\n";
      mg_http_serve_dir(c, hm, &sopts);
    } else if (mg_match(hm->uri, mg_str("/servefile"), NULL)) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.mime_types = "foo=a/b,txt=c/d";
      mg_http_serve_file(c, hm, "data/a.txt", &sopts);
    } else if (mg_match(hm->uri, mg_str("/servefile2"), NULL)) {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.mime_types = "*=a/b,txt=c/d";
      mg_http_serve_file(c, hm, "data/a.txt", &sopts);
    } else {
      struct mg_http_serve_opts sopts;
      memset(&sopts, 0, sizeof(sopts));
      sopts.root_dir = "./data";
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
    mg_ws_send(c, wm->data.buf, wm->data.len, WEBSOCKET_OP_BINARY);
  }
}

struct fetch_data {
  char *buf;
  int code, closed;
};

static void fcb(struct mg_connection *c, int ev, void *ev_data) {
  struct fetch_data *fd = (struct fetch_data *) c->fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    snprintf(fd->buf, FETCH_BUF_SIZE, "%.*s", (int) hm->message.len,
             hm->message.buf);
    fd->code = atoi(hm->uri.buf);
    fd->closed = 1;
    c->is_closing = 1;
    (void) c;
  } else if (ev == MG_EV_CLOSE) {
    fd->closed = 1;
  }
}

static int fetch(struct mg_mgr *mgr, char *buf, const char *url,
                 const char *fmt, ...) {
  struct fetch_data fd = {buf, 0, 0};
  int i;
  struct mg_connection *c = NULL;
  va_list ap;
  c = mg_http_connect(mgr, url, fcb, &fd);
  ASSERT(c != NULL);
  if (c != NULL && mg_url_is_ssl(url)) {
    struct mg_tls_opts opts;
    memset(&opts, 0, sizeof(opts));  // read CA from packed_fs
    opts.name = mg_url_host(url);
    opts.ca = mg_unpacked("/data/ca.pem");
    if (strstr(url, "localhost") != NULL) {
      // Local connection, use self-signed certificates
      opts.ca = mg_unpacked("/certs/ca.crt");
      // opts.cert = mg_str(s_tls_cert);
      // opts.key = mg_str(s_tls_key);
    }
    mg_tls_init(c, &opts);
  }
  // c->is_hexdumping = 1;
  va_start(ap, fmt);
  mg_vprintf(c, fmt, &ap);
  va_end(ap);
  buf[0] = '\0';
  for (i = 0; i < 100 && buf[0] == '\0'; i++) mg_mgr_poll(mgr, 1);
  if (!fd.closed) c->is_closing = 1;
  mg_mgr_poll(mgr, 1);
  return fd.code;
}

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

static bool cmpheader(const char *buf, const char *name, const char *value) {
  struct mg_http_message hm = gethm(buf);
  struct mg_str *h = mg_http_get_header(&hm, name);
  return h != NULL && mg_strcmp(*h, mg_str(value)) == 0;
}

static void wcb(struct mg_connection *c, int ev, void *ev_data) {
  int *p = (int *) c->fn_data;
  if (ev == MG_EV_WS_OPEN) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_str *wsproto = mg_http_get_header(hm, "Sec-WebSocket-Protocol");
    ASSERT(wsproto != NULL);
    mg_ws_send(c, "hello", 0, 0);
    mg_ws_printf(c, WEBSOCKET_OP_BINARY, "%.3s", "boo!!!!");
    mg_ws_printf(c, WEBSOCKET_OP_BINARY, "%s", "foobar");
    mg_ws_send(c, "", 0, WEBSOCKET_OP_PING);
    p[0] += 100;
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (mgstrstr(wm->data, mg_str("foobar")))
      mg_ws_send(c, "", 0, WEBSOCKET_OP_CLOSE);
    if (mgstrstr(wm->data, mg_str("boo"))) p[0] += 2;
    if (mgstrstr(wm->data, mg_str("foobar"))) p[0] += 3;
  } else if (ev == MG_EV_CLOSE) {
    p[0] += 10;
  }
}

static void ew2(struct mg_connection *c, int ev, void *ev_data) {
  size_t size = 65 * 1024 + 737;
  if (ev == MG_EV_WS_OPEN) {
    char *msg = (char *) calloc(1, size + 1);
    memset(msg, 'A', size);
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", msg);
    free(msg);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    if (wm->data.len == 6) {
      // Ignore the "opened" message from server
    } else {
      size_t ok = 1, i;
      ASSERT(wm->data.len == size);
      for (i = 0; i < size; i++) {
        if (wm->data.buf[i] != 'A') ok = 0;
      }
      ASSERT(ok == 1);
      *(int *) c->fn_data = 1;
    }
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

  // Test large WS frames, over 64k
  done = 0;
  mg_ws_connect(&mgr, url, ew2, &done, NULL);
  for (i = 0; i < 1000 && done == 0; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(done == 1);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void eh9(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ERROR) {
    ASSERT(!strcmp((char *) ev_data, "socket error"));
    *(int *) c->fn_data = 7;
  }
  (void) c;
}

struct fpr_data {
  char *buf;
  int len, reqs, closed;
};

static void fprcb(struct mg_connection *c, int ev, void *ev_data) {
  struct fpr_data *fd = (struct fpr_data *) c->fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    int code = atoi(hm->uri.buf);
    if (code == 200) {
      snprintf(fd->buf + fd->len, FETCH_BUF_SIZE - (unsigned int) fd->len,
               "%.*s", (int) hm->message.len, hm->message.buf);
      fd->len += (int) hm->message.len;
      ++fd->reqs;
      if (fd->reqs == 2) {
        fd->closed = 1;
        c->is_closing = 1;
      }
    } else {  // allow testing for other codes and catching wrong responses
      MG_INFO(("reqs: %d, code: %d", fd->reqs, code));
      fd->reqs += code;
    }
    (void) c;
  } else if (ev == MG_EV_CLOSE) {
    fd->closed = 1;
  }
}

static int fpr(struct mg_mgr *mgr, char *buf, const char *url, const char *fmt,
               ...) {
  struct fpr_data fd = {buf, 0, 0, 0};
  int i;
  struct mg_connection *c = NULL;
  va_list ap;
  c = mg_http_connect(mgr, url, fprcb, &fd);
  ASSERT(c != NULL);
  va_start(ap, fmt);
  mg_vprintf(c, fmt, &ap);
  va_end(ap);
  buf[0] = '\0';
  for (i = 0; i < 500 && !fd.closed; i++) mg_mgr_poll(mgr, 1);
  if (!fd.closed) c->is_closing = 1;
  mg_mgr_poll(mgr, 1);
  return fd.reqs;
}

static void test_http_server(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12346";
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh1, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  ASSERT(cmpheader(buf, "C", "D"));

  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\nA:\tB\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  ASSERT(cmpheader(buf, "A", "B") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /%%61.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);

  // Invalid header: failure
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\nA B\n\n") == 0);

  // Pipelined requests
  ASSERT(fpr(&mgr, buf, url,
             "GET /foo/bar HTTP/1.1\n\nGET /foo/foobar HTTP/1.1\n\n") == 2);
  // Pipelined requests with file requests other than the last one (see #2796)
  ASSERT(fpr(&mgr, buf, url,
             "GET /a.txt HTTP/1.1\n\nGET /a.txt HTTP/1.1\n\n") == 2);
  /*ASSERT(fpr(&mgr, buf, url,
             "HEAD /a.txt HTTP/1.1\n\nGET /a.txt HTTP/1.1\n\n") == 2);*/
  // Connection: close
  ASSERT(fpr(&mgr, buf, url,
             "GET /foo/bar HTTP/1.1\nConnection: close\n\nGET /foo/foobar "
             "HTTP/1.1\n\n") == 1);
  ASSERT(cmpbody(buf, "uri: bar") == 0);

  // Responses with missing reason phrase must also work
  ASSERT(fetch(&mgr, buf, url, "GET /no_reason HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "ok") == 0);

  // Fetch file with unicode chars in filename
  ASSERT(fetch(&mgr, buf, url, "GET /київ.txt HTTP/1.0\n\n") == 200);
  MG_INFO(("%s", buf));
  ASSERT(cmpbody(buf, "є\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /../fuzz.c HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url, "GET /.%%2e/fuzz.c HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url, "GET /.%%2e%%2ffuzz.c HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url, "GET /..%%2f%%20fuzz.c HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url, "GET /..%%2ffuzz.c%%20 HTTP/1.0\n\n") == 400);

  ASSERT(fetch(&mgr, buf, url, "GET /dredir HTTP/1.0\n\n") == 301);
  ASSERT(cmpheader(buf, "Location", "/dredir/"));

  ASSERT(fetch(&mgr, buf, url, "GET /dredir/ HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hi\n") == 0);

  ASSERT(fetch(&mgr, buf, url,
               "GET /dredirgz/ HTTP/1.0\n"
               "Accept-Encoding: gzip\n\n") == 200);
  ASSERT(cmpheader(buf, "Content-Type", "text/html; charset=utf-8"));
  ASSERT(cmpheader(buf, "Content-Encoding", "gzip"));

  ASSERT(fetch(&mgr, buf, url, "GET /gzip.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hi\n") == 0);
  ASSERT(gethm(buf).body.len == 3);
  ASSERT(cmpheader(buf, "Content-Encoding", "gzip") == false);

  ASSERT(fetch(&mgr, buf, url,
               "GET /gzip.txt HTTP/1.0\n"
               "Accept-Encoding: foo,gzip\n\n") == 200);
  mg_hexdump(buf, strlen(buf));
  ASSERT(cmpheader(buf, "Content-Encoding", "gzip") == true);
  ASSERT(gethm(buf).body.len == 23);

  ASSERT(fetch(&mgr, buf, url, "GET /..ddot HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url, "GET /..ddot/ HTTP/1.0\n\n") == 400);
  ASSERT(fetch(&mgr, buf, url,
               "GET /a.txt HTTP/1.0\n"
               "Content-Length: -123\n\n") == 0);
  ASSERT(fetch(&mgr, buf, url,
               "POST /a.txt HTTP/1.0\n"
               "Content-Length: -123\n\n") == 0);
  ASSERT(fetch(&mgr, buf, url,
               "GET /a.txt HTTP/1.0\n"
               "Content-Length: 19000000000000000000\n\n") == 0);
  ASSERT(fetch(&mgr, buf, url,
               "POST /a.txt HTTP/1.0\n"
               "Content-Length: 19000000000000000000\n\n") == 0);
  ASSERT(fetch(&mgr, buf, url,
               "GET /a.txt HTTP/1.0\n"
               ":\n"  // truncated header
               "Content-Length: 1\n\n") == 0);

  {
    extern char *mg_http_etag(char *, size_t, size_t, time_t);
    struct mg_http_message hm;
    char etag[100];
    size_t size = 0;
    time_t mtime = 0;
    ASSERT(mg_fs_posix.st("./data/a.txt", &size, &mtime) != 0);
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
    struct mg_str *s;
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT((s = mg_http_get_header(&hm, "Content-Type")) != NULL);
    ASSERT(mg_strcmp(*s, mg_str("c/d")) == 0);
  }

  ASSERT(fetch(&mgr, buf, url, "GET /servefile2 HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);
  {
    struct mg_http_message hm;
    struct mg_str *s;
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT((s = mg_http_get_header(&hm, "Content-Type")) != NULL);
    ASSERT(mg_strcmp(*s, mg_str("a/b")) == 0);
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
    struct mg_str data = mg_file_read(&mg_fs_posix, "./data/ca.pem");
    ASSERT(fetch(&mgr, buf, url, "GET /ca.pem HTTP/1.0\r\n\n") == 200);
    ASSERT(cmpbody(buf, data.buf) == 0);
    mg_free((void *) data.buf);
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
    // Give it a couple of seconds, see #1605
    for (i = 0; i < 200 && errored == 0; i++) mg_mgr_poll(&mgr, 10);
    MG_INFO(("errored: %d, expected: 7", errored));
    ASSERT(errored == 7);
  }

  // Directory listing
  fetch(&mgr, buf, url, "GET /dirtest/ HTTP/1.0\n\n");
  ASSERT(fetch(&mgr, buf, url, "GET /dirtest/ HTTP/1.0\n\n") == 200);
  ASSERT(mgstrstr(mg_str(buf), mg_str(">Index of /dirtest/<")) != NULL);
  ASSERT(mgstrstr(mg_str(buf), mg_str(">fuzz.c<")) != NULL);
  ASSERT(cmpheader(buf, "A", "B"));
  ASSERT(!cmpheader(buf, "C", "D"));
  ASSERT(cmpheader(buf, "E", "F"));

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
    struct mg_str s;
    remove("uploaded.txt");
    s = mg_file_read(&mg_fs_posix, "uploaded.txt");
    ASSERT(s.buf == NULL);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload HTTP/1.0\n"
                 "Content-Length: 1\n\nx") == 400);

    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?file=uploaded.txt HTTP/1.0\r\n"
                 "Content-Length: 5\r\n"
                 "\r\nhello") == 200);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?file=uploaded.txt&offset=5 HTTP/1.0\r\n"
                 "Content-Length: 6\r\n"
                 "\r\n\nworld") == 200);
    s = mg_file_read(&mg_fs_posix, "uploaded.txt");
    ASSERT(s.buf != NULL);
    ASSERT(strcmp(s.buf, "hello\nworld") == 0);
    mg_free((void *) s.buf);
    remove("uploaded.txt");
  }

  {
    // Test upload directory traversal
    struct mg_str s;
    remove("uploaded.txt");
    s = mg_file_read(&mg_fs_posix, "uploaded.txt");
    ASSERT(s.buf == NULL);
    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?file=../uploaded.txt HTTP/1.0\r\n"
                 "Content-Length: 5\r\n"
                 "\r\nhello") == 400);
    s = mg_file_read(&mg_fs_posix, "uploaded.txt");
    ASSERT(s.buf == NULL);
  }

  // HEAD request
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(fetch(&mgr, buf, url, "HEAD /a.txt HTTP/1.0\n\n") == 200);

  // Pre-compressed files
  {
    struct mg_http_message hm;
    ASSERT(fetch(&mgr, buf, url,
                 "HEAD /hello.txt HTTP/1.0\n"
                 "Accept-Encoding: gzip\n\n") == 200);
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

  // #2552, reject chunks with no length
  ASSERT(fetch(&mgr, buf, url,
               "POST / HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n\r\n"
               "1\r\n"
               "Z\r\n"
               "?\r\n"
               "\r\n") == 0);
  ASSERT(fetch(&mgr, buf, url,
               "POST / HTTP/1.1\r\n"
               "Transfer-Encoding: chunked\r\n\r\n"
               "1\r\n"
               "Z\r\n"
               "\r\n"
               "\r\n") == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void h4(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    MG_INFO(("[%.*s %.*s] message len %d", (int) hm->method.len, hm->method.buf,
             (int) hm->uri.len, hm->uri.buf, (int) hm->message.len));
    if (mg_match(hm->uri, mg_str("/a/#"), NULL)) {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "/a=./data";
      opts.page404 = "./data/404.html";  // existing 404 page
      mg_http_serve_dir(c, hm, &opts);
    } else if (mg_match(hm->uri, mg_str("/b/#"), NULL)) {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "/b=./data";
      opts.page404 = "./data/nooooo.html";  // non-existing 404 page
      mg_http_serve_dir(c, hm, &opts);
    } else {  // null 404 page
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
      opts.root_dir = "./data";
      mg_http_serve_dir(c, hm, &opts);
    }
  }
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
  {  // txt requested, existent 404 is html
    struct mg_http_message hm;
    struct mg_str *cl;
    mg_http_parse(buf, strlen(buf), &hm);
    cl = mg_http_get_header(&hm, "Content-Type");
    ASSERT(cl != NULL);
    if (cl->len > 9) cl->len = 9;  // restrict to text/html len max
    ASSERT(mg_strcmp(*cl, mg_str("text/html")) == 0);
  }
  ASSERT(fetch(&mgr, buf, url, "GET /b/xx.txt HTTP/1.0\n\n") == 404);
  ASSERT(cmpbody(buf, "Not found\n") == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_tls(void) {
//  return;
#if MG_TLS
  struct mg_mgr mgr;
  struct mg_connection *c;
  const char *url = "https://localhost:12347";
  char buf[FETCH_BUF_SIZE];
  struct mg_tls_opts opts;
  struct mg_str data = mg_unpacked("/Makefile");
  char bigdata[FETCH_BUF_SIZE - 256];  // leave extra room
  struct mg_str bd;
  ASSERT(data.buf != NULL && data.len > 0);
  memset(&opts, 0, sizeof(opts));
  // opts.ca = mg_str(s_tls_ca);
  opts.cert = mg_unpacked("/certs/server.crt");
  opts.key = mg_unpacked("/certs/server.key");
  mg_mgr_init(&mgr);
  c = mg_http_listen(&mgr, url, eh1, &opts);
  ASSERT(c != NULL);
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  // MG_INFO(("%s", buf));
  ASSERT(cmpbody(buf, "hello\n") == 0);
  // POST a large file, several MSS but less than max TLS record length
  // make sure we drain TLS buffers and read all, #2619
  ASSERT(data.len > 3100 && data.len < 16384);  // pick another file on failure
  ASSERT(fetch(&mgr, buf, url,
               "POST /body HTTP/1.0\n"
               "Content-Length: %lu\n\n"
               "%s",
               data.len, data.buf) == 200);
  // /body returns data back, so verify contents (both server and client send
  // and receive the whole file
  ASSERT(cmpbody(buf, data.buf) == 0);
  // repeat with a really large "file", several times max TLS record length
  bd = genstring(bigdata, sizeof(bigdata));
  ASSERT(fetch(&mgr, buf, url,
               "POST /body HTTP/1.0\n"
               "Content-Length: %lu\n\n"
               "%s",
               bd.len, bd.buf) == 200);
  ASSERT(cmpbody(buf, bd.buf) == 0);
#if MG_TLS == MG_TLS_BUILTIN && defined(__linux__)
  // fire patched server, test multiple TLS records per TCP segment handling
  // skip other TLS stacks to avoid "bad client hello", we are 1.3 only
  if (access("tls_multirec/server", X_OK) == 0) {
    ASSERT(system("tls_multirec/server -d tls_multirec &") == 0);
    sleep(1);
    // fetch() needs to loop enough times in order to process all TLS records;
    // otherwise it will end with 200 and shorter file contents
    ASSERT(fetch(&mgr, buf, "https://localhost:8443",
                 "GET /thefile HTTP/1.0\n\n") == 200);
    ASSERT(cmpbody(buf, data.buf) == 0);  // "thefile" links to Makefile
    ASSERT(system("killall tls_multirec/server") == 0);
  } else {
    MG_ERROR(("SKIPPED TLS MULTIPLE RECORDS TEST, tls_multirec/server NOT PRESENT"));
  }
#else
  printf(
      "\n Skipping multiple TLS records per TCP segment handling test, server "
      "is 1.3 only; re-enable when other stacks can be easily configured for "
      "1.3\n");
#endif
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
#endif
}

static void f3(struct mg_connection *c, int ev, void *ev_data) {
  int *ok = (int *) c->fn_data;
  // MG_INFO(("%d", ev));
  if (ev == MG_EV_CONNECT) {
    // c->is_hexdumping = 1;
    ASSERT((c->loc.ip[0] != 0));  // Make sure that c->loc address is populated
    mg_printf(c, "GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",
              c->rem.is_ip6 ? "" : "/robots.txt",
              c->rem.is_ip6 ? "ipv6.google.com" : "cesanta.com");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // MG_INFO(("-->[%.*s]", (int) hm->message.len, hm->message.buf));
    // ASSERT(vcmp(hm->method, "HTTP/1.1"));
    // ASSERT(vcmp(hm->uri, "301"));
    *ok = mg_http_status(hm);
    c->is_closing = 1;
  } else if (ev == MG_EV_CLOSE) {
    if (*ok == 0) *ok = 888;
  } else if (ev == MG_EV_ERROR) {
    if (*ok == 0) *ok = 777;
  }
}

static void test_http_client(void) {
  struct mg_mgr mgr;
  struct mg_connection *c = NULL;
  const char *url = "http://cesanta.com";
  int i, ok = 0;
  struct mg_tls_opts opts;
  memset(&opts, 0, sizeof(opts));
  opts.ca = mg_unpacked("/data/ca.pem");
  opts.name = mg_url_host(url);
  ASSERT(opts.ca.len > 0);
  ASSERT(opts.name.len > 0);
  mg_mgr_init(&mgr);
  c = mg_http_connect(&mgr, url, f3, &ok);
  ASSERT(c != NULL);
  for (i = 0; i < 1000 && ok <= 0; i++) mg_mgr_poll(&mgr, 1);
  MG_INFO(("OK: %d", ok));
  ASSERT(ok == 301 || ok == 200);
  mg_mgr_poll(&mgr, 0);
  ok = 0;
#if MG_TLS
  url = "https://cesanta.com";
  opts.name = mg_url_host(url);
#if MG_TLS == MG_TLS_BUILTIN
  // our TLS does not search for the proper CA in a bundle
  opts.ca = mg_file_read(&mg_fs_posix, "data/e5.crt");
#endif
  c = mg_http_connect(&mgr, url, f3, &ok);
  ASSERT(c != NULL);
  mg_tls_init(c, &opts);
  for (i = 0; i < 1500 && ok <= 0; i++) mg_mgr_poll(&mgr, 1);
  MG_INFO(("OK: %d", ok));
  ASSERT(ok == 200);
  mg_mgr_poll(&mgr, 1);

  // Make host validationfail
  c = mg_http_connect(&mgr, url, f3, &ok);
  ASSERT(c != NULL);
  opts.name = mg_str("dummy");  // Set some invalid hostname value
  mg_tls_init(c, &opts);
  ok = 0;
  for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  MG_INFO(("OK: %d", ok));
  ASSERT(ok == 777);
  mg_mgr_poll(&mgr, 1);
  opts.name = mg_url_host(url);
#if MG_TLS == MG_TLS_BUILTIN
  mg_free((void *) opts.ca.buf);
#endif

  // Test empty CA
  // Disable mbedTLS: https://github.com/Mbed-TLS/mbedtls/issues/7075
#if MG_TLS != MG_TLS_MBED
  opts.ca = mg_str("");
  c = mg_http_connect(&mgr, url, f3, &ok);
  mg_tls_init(c, &opts);
  ok = 0;
  for (i = 0; i < 1000 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  MG_INFO(("OK: %d", ok));
  ASSERT(ok == 200);
  mg_mgr_poll(&mgr, 1);
#endif
#endif

#if MG_ENABLE_IPV6
  ok = 0;
  // ipv6.google.com does not have IPv4 address, only IPv6, therefore
  // it is guaranteed to hit IPv6 resolution path.
  c = mg_http_connect(&mgr, "http://ipv6.google.com", f3, &ok);
  for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
  MG_INFO(("OK: %d", ok));
  ASSERT(ok == 200);
#endif

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f4(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_printf(c, "HTTP/1.0 200 OK\n\n%.*s/%s", (int) hm->uri.len, hm->uri.buf,
              "abcdef");
    strcat((char *) c->fn_data, "m");
    c->is_draining = 1;
  } else if (ev == MG_EV_CLOSE) {
    strcat((char *) c->fn_data, "c");
  }
}

static void f4c(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CONNECT) {
    mg_printf(c, "GET /foo/bar HTTP/1.0\n\n");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    ASSERT(mg_strcmp(hm->body, mg_str("/foo/bar/abcdef")) == 0);
    strcat((char *) c->fn_data, "m");
  } else if (ev == MG_EV_CLOSE) {
    strcat((char *) c->fn_data, "c");
  }
}

static void f41(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_printf(c, "HTTP/1.0 200 OK\n\n%.*s/%s", (int) hm->uri.len, hm->uri.buf,
              "abcdef");
  }
}

static void test_http_no_content_length(void) {
  char buf1[10] = {0}, buf2[10] = {0};
  char buf[100];
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12348";
  const char *url2 = "http://127.0.0.1:12349";
  int i;
  const char *post_req =
      "POST / HTTP/1.1\r\nContent-Type:"
      "b/a\r\nContent-Length: 15\r\n\r\n"
      "{\"key\": \"value\"}";
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, f4, (void *) buf1);
  mg_http_connect(&mgr, url, f4c, (void *) buf2);
  for (i = 0; i < 1000 && strchr(buf2, 'c') == NULL; i++) mg_mgr_poll(&mgr, 10);
  MG_INFO(("[%s] [%s]", buf1, buf2));
  ASSERT(strcmp(buf1, "mc") == 0);
  ASSERT(strcmp(buf2, "mc") == 0);
  mg_mgr_free(&mgr);
  // 12348 is in TIME_WAIT, use another port
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url2, f41, (void *) NULL);
  ASSERT(fetch(&mgr, buf, url2, "POST / HTTP/1.1\r\n\r\n") == 411);
  ASSERT(fetch(&mgr, buf, url2, "HTTP/1.1 200\r\n\r\n") == 411);
  ASSERT(fetch(&mgr, buf, url2, "HTTP/1.1 100\r\n\r\n") != 411);
  ASSERT(fetch(&mgr, buf, url2, "HTTP/1.1 304\r\n\r\n") != 411);
  ASSERT(fetch(&mgr, buf, url2, "HTTP/1.1 305\r\n\r\n") == 411);
  ASSERT(fetch(&mgr, buf, url2, post_req) != 411);
  // Check it is processed only once (see #2811)
  ASSERT(fpr(&mgr, buf, url2, "POST / HTTP/1.1\r\n\r\n") == 411);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f5(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    mg_http_reply(c, 200, "", "%.*s", (int) hm->uri.len, hm->uri.buf);
    (*(int *) c->fn_data)++;
  }
}

static void f6(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    (*(int *) c->fn_data)++;
    (void) ev_data;
  }
}

static void test_http_pipeline(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12377";
  struct mg_connection *c;
  int i, ok = 0, ok2 = 0;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, f5, (void *) &ok);
  c = mg_http_connect(&mgr, url, f6, &ok2);
  mg_printf(c, "POST / HTTP/1.0\nContent-Length: 5\n\n12345GET / HTTP/1.0\n\n");
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(ok == 2);
  ASSERT(ok2 == 2);
  // Fire a valid, then invalid request, see #2592. First one should serve
  ok = ok2 = 0;
  c = mg_http_connect(&mgr, url, f6, (void *) &ok2);
  mg_printf(c, "GET / HTTP/1.1\n\nInvalid\n\n");
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(ok == 1);
  ASSERT(ok2 == 1);
  // MG_INFO(("-----> [%d] [%d]", ok, ok2));
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
    const char *s = "GET / \r\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == 0);
  }

  {
    const char *s = "GET / invalid\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == -1);
  }

  {
    const char *s = "GET /blah HTTP/1.0\r\nFoo:  bar  \r\n\r\n";
    size_t idx, len = strlen(s);
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) len);
    ASSERT(vcmp(req.headers[0].name, "Foo"));
    ASSERT(vcmp(req.headers[0].value, "bar"));
    ASSERT(req.headers[1].name.len == 0);
    ASSERT(req.headers[1].name.buf == NULL);
    ASSERT(req.query.len == 0);
    ASSERT(req.message.len == len);
    ASSERT(req.body.len == 0);
    ASSERT(vcmp(req.method, "GET"));
    ASSERT(vcmp(req.uri, "/blah"));
    ASSERT(vcmp(req.proto, "HTTP/1.0"));
    for (idx = 0; idx < len; idx++) ASSERT(mg_http_parse(s, idx, &req) == 0);
  }

  {
    const char *s = "get b HTTP/1.1\nb: t\nv:vv\n\n xx";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
  }

  {
    const char *s = "get b HTTP/1.1\nb: t\nv:\n\n xx";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
  }

  {
    const char *s = "get b HTTP/1.1\nb: t\nv v\n\n xx";
    ASSERT(mg_http_parse(s, strlen(s), &req) == -1);
  }

  {
    const char *s = "get b HTTP/1.1\nb: t\n : aa\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == -1);
  }

  {
    const char *s = "get b HTTP/1.1\nz:  k \nb: t\nv:k\n\n xx";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
    ASSERT(req.headers[3].name.len == 0);
    ASSERT(vcmp(req.headers[0].name, "z"));
    ASSERT(vcmp(req.headers[0].value, "k"));
    ASSERT(vcmp(req.headers[1].name, "b"));
    ASSERT(vcmp(req.headers[1].value, "t"));
    ASSERT(vcmp(req.headers[2].name, "v"));
    ASSERT(vcmp(req.headers[2].value, "k"));
    ASSERT(req.body.len == 0);
  }

  // #2292: fail on stray \r inside the headers
  ASSERT(mg_http_parse("a є\n\n", 6, &req) == 6);
  ASSERT(mg_http_parse("a b\n\n", 5, &req) == 5);
  ASSERT(mg_http_parse("a b\na:\n\n", 8, &req) > 0);
  ASSERT(mg_http_parse("a b\na:\r\n\n", 9, &req) > 0);
  ASSERT(mg_http_parse("a b\n\ra:\r\n\n", 10, &req) == -1);
  ASSERT(mg_http_parse("a b\na:\r1\n\n", 10, &req) == -1);
  ASSERT(mg_http_parse("a b\na: \r1\n\n", 11, &req) == -1);
  ASSERT(mg_http_parse("a b\na: \rb:\n\n", 12, &req) == -1);
  ASSERT(mg_http_parse("a b\na: \nb:\n\n", 12, &req) > 0);

  {
    const char *s = "ґєт /слеш HTTP/1.0\nмісто:  кіїв \n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(req.body.len == 0);
    ASSERT(req.headers[1].name.len == 0);
    ASSERT(vcmp(req.headers[0].name, "місто"));
    ASSERT(vcmp(req.headers[0].value, "кіїв"));
    ASSERT((v = mg_http_get_header(&req, "місто")) != NULL);
    ASSERT(vcmp(req.method, "ґєт"));
    ASSERT(vcmp(req.uri, "/слеш"));
    ASSERT(vcmp(req.proto, "HTTP/1.0"));
  }

  {
    const char *s =
        "a b HTTP/1.0\r\nContent-Length: 21 \r\nb: t\r\nv:v\r\n\r\nabc";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s) - 3);
    ASSERT(req.body.len == 21);
    ASSERT(req.message.len == 21 - 3 + strlen(s));
    ASSERT(mg_http_get_header(&req, "foo") == NULL);
    ASSERT((v = mg_http_get_header(&req, "contENT-Length")) != NULL);
    ASSERT(vcmp(*v, "21"));
    ASSERT((v = mg_http_get_header(&req, "B")) != NULL);
    ASSERT(vcmp(*v, "t"));
  }

  {
    const char *s = "GET /foo?a=b&c=d HTTP/1.0\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT(vcmp(req.uri, "/foo"));
    ASSERT(vcmp(req.query, "a=b&c=d"));
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
    ASSERT(vcmp(req.method, "HTTP/1.0"));
    ASSERT(vcmp(req.uri, "200"));
    ASSERT(vcmp(req.proto, "OK"));
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
    ASSERT(vcmp(*v, "127.0.0.1:18888"));
    ASSERT((v = mg_http_get_header(&req, "Cookie")) != NULL);
    ASSERT(v->len == 0);
    ASSERT((v = mg_http_get_header(&req, "X-PlayID")) != NULL);
    ASSERT(vcmp(*v, "45455"));
    ASSERT((v = mg_http_get_header(&req, "Range")) != NULL);
    ASSERT(vcmp(*v, "0-1"));
  }

  {
    static const char *s =
        "a b HTTP/1.0\na:1\nb:2\nc:3\nd:4\ne:5\nf:6\ng:7\nh:8\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT((v = mg_http_get_header(&req, "e")) != NULL);
    ASSERT(vcmp(*v, "5"));
    ASSERT((v = mg_http_get_header(&req, "g")) != NULL);
    ASSERT(vcmp(*v, "7"));
    ASSERT((v = mg_http_get_header(&req, "h")) == NULL);  // MG_MAX_HTTP_HEADERS
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
    const char *s = "a b HTTP/1.0\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b\nc:d\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a\nb:b\nc:c\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) < 0);
    s = "a b\nc: \xc0\n\n";  // Invalid UTF in the header value: accept
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    ASSERT((v = mg_http_get_header(&hm, "c")) != NULL);
    ASSERT(vcmp(*v, "\xc0"));
    s = "a b\n\xc0: 2\n\n";  // Invalid UTF in the header name: do NOT accept
    ASSERT(mg_http_parse(s, strlen(s), &hm) == -1);
  }

  {
    struct mg_http_message hm;
    const char *s;
    s = "a b HTTP/1.0\nd:e\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b HTTP/1.0\nd: e\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b HTTP/1.0\nd:\te\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b HTTP/1.0\nd:\t e\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    s = "a b HTTP/1.0\nd: \te\t \n\n";
    ASSERT(mg_http_parse(s, strlen(s), &hm) == (int) strlen(s));
    ASSERT(mg_strcmp(hm.headers[0].value, mg_str("e")) == 0);
  }
}

static void ehr(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.root_dir = "./data";
    mg_http_serve_dir(c, hm, &opts);
  }
}

static void test_http_range(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12350";
  struct mg_http_message hm;
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, ehr, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /range.txt HTTP/1.0\n\n") == 200);
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  ASSERT(hm.body.len == 312);

  fetch(&mgr, buf, url, "%s", "GET /range.txt HTTP/1.0\nRange: bytes=5-10\n\n");
  ASSERT(mg_http_parse(buf, strlen(buf), &hm) > 0);
  printf("%s", buf);
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

  // Start a timer, then shift system time a long time back and long time forth
  v1 = 0;
  mg_timer_init(&head, &t1, 5, MG_TIMER_REPEAT, f1, &v1);
  mg_timer_poll(&head, 0);
  ASSERT(v1 == 0);

  // Shift a long time forth, make sure it ticks
  mg_timer_poll(&head, 100);
  ASSERT(v1 == 1);
  mg_timer_poll(&head, 101);
  ASSERT(v1 == 1);
  mg_timer_poll(&head, 102);
  ASSERT(v1 == 1);
  mg_timer_poll(&head, 103);
  ASSERT(v1 == 1);
  mg_timer_poll(&head, 104);
  ASSERT(v1 == 1);
  mg_timer_poll(&head, 105);
  ASSERT(v1 == 2);

  // Shift a long time back, make sure it ticks
  mg_timer_poll(&head, 50);
  ASSERT(v1 == 2);
  mg_timer_poll(&head, 60);
  ASSERT(v1 == 3);

  mg_timer_free(&head, &t1);
  ASSERT(head == NULL);

  // Test proper timer deallocation, see #1539
  {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_timer_add(&mgr, 1, MG_TIMER_REPEAT, f1, NULL);
    ASSERT(mgr.timers != NULL);
    mg_mgr_free(&mgr);
    ASSERT(mgr.timers == NULL);
    ASSERT(mgr.conns == NULL);
  }

  // Test that non-repeating called timers are deleted, see #2768
  {
    struct mg_mgr mgr;
    int arg = 0;
    mg_mgr_init(&mgr);
    mg_timer_add(&mgr, 0, MG_TIMER_ONCE, f1, &arg);
    ASSERT(mgr.timers != NULL);
    mg_mgr_poll(&mgr, 10);
    ASSERT(arg == 1);
    ASSERT(mgr.timers == NULL);
    mg_mgr_free(&mgr);
    ASSERT(mgr.timers == NULL);
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

static size_t pf1(void (*out)(char, void *), void *ptr, va_list *ap) {
  int a = va_arg(*ap, int);
  int b = va_arg(*ap, int);
  return mg_xprintf(out, ptr, "%d", a + b);
}

static size_t pf2(void (*out)(char, void *), void *ptr, va_list *ap) {
  int cnt = va_arg(*ap, int);
  size_t n = 0;
  while (cnt-- > 0) n += mg_xprintf(out, ptr, "%d", cnt);
  return n;
}

static bool chkdbl(struct mg_str s, double val) {
  double d, tolerance = 1e-14;
  return mg_json_get_num(s, "$", &d) && fabs(val - d) < tolerance;
}

static void test_str(void) {
  {
    struct mg_str s = mg_strdup(mg_str("a"));
    ASSERT(mg_strcmp(s, mg_str("a")) == 0);
    mg_free((void *) s.buf);
  }

  {
    const char *s;
    struct mg_str a = mg_str("hello"), b = mg_str("a"), c = mg_str(NULL);
    ASSERT((s = mgstrstr(a, b)) == NULL);
    ASSERT((s = mgstrstr(a, c)) != NULL);
    ASSERT(s == a.buf);
  }

  ASSERT(mg_strcmp(mg_str(""), mg_str(NULL)) == 0);
  ASSERT(mg_strcmp(mg_str("a"), mg_str("b")) < 0);
  ASSERT(mg_strcmp(mg_str("b"), mg_str("a")) > 0);
  ASSERT(mg_strcmp(mg_str("hi"), strstrip(mg_str(" \thi\r\n"))) == 0);

  ASSERT(sccmp("", "", 0));
  ASSERT(sccmp("", "1", -49));
  ASSERT(sccmp("a", "A", 0));
  ASSERT(sccmp("a1", "A", 49));
  ASSERT(sccmp("a", "A1", -49));

  ASSERT(mg_strcasecmp(mg_str(""), mg_str(NULL)) == 0);
  ASSERT(mg_strcasecmp(mg_str("a"), mg_str("B")) < 0);
  ASSERT(mg_strcasecmp(mg_str("b"), mg_str("A")) > 0);
  ASSERT(mg_strcasecmp(mg_str("hi"), mg_str("HI")) == 0);

  {
    ASSERT(chkdbl(mg_str_n("1.23", 3), 1.2));
    ASSERT(chkdbl(mg_str("1.23 "), 1.23));
    ASSERT(chkdbl(mg_str("-0.01 "), -0.01));
    ASSERT(chkdbl(mg_str("-0.5e2"), -50.0));
    ASSERT(chkdbl(mg_str("123e-3"), 0.123));
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
    char buf[100], *p = NULL;
    struct mg_iobuf io = {0, 0, 0, 16};
    const char *expected;

    expected = "\"\"";
    mg_snprintf(buf, sizeof(buf), "%m", mg_print_esc, 0, "");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "";
    mg_snprintf(buf, 1, "%s", "abc");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "a";
    mg_snprintf(buf, 2, "%s", "abc");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"hi, \\\"\"";
    mg_snprintf(buf, sizeof(buf), "\"hi, %M\"", mg_print_esc, 0, "\"");
    MG_INFO(("[%s] [%s]", buf, expected));
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"a'b\"";
    mg_snprintf(buf, sizeof(buf), "%m", mg_print_esc, 0, "a'b");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"a\\b\\n\\f\\r\\t\\\"\"";
    mg_snprintf(buf, sizeof(buf), "%m", mg_print_esc, 0, "a\b\n\f\r\t\"");
    ASSERT(strcmp(buf, expected) == 0);

    expected = "\"abc\"";
    mg_snprintf(buf, sizeof(buf), "%m", mg_print_esc, 3, "abcdef");
    ASSERT(strcmp(buf, expected) == 0);

    p = mg_mprintf("[%s,%M,%s]", "null", pf1, 2, 3, "hi");
    ASSERT(strcmp(p, "[null,5,hi]") == 0);
    mg_free(p);

    p = mg_mprintf("[%M,%d]", pf2, 10, 7);
    ASSERT(strcmp(p, "[9876543210,7]") == 0);
    mg_free(p);

    mg_xprintf(mg_pfn_iobuf, &io, "[%M", pf2, 10);
    mg_xprintf(mg_pfn_iobuf, &io, ",");
    mg_xprintf(mg_pfn_iobuf, &io, "%d]", 7);
    ASSERT(strcmp((char *) io.buf, "[9876543210,7]") == 0);
    mg_iobuf_free(&io);
  }

  {
#if MG_ARCH == MG_ARCH_WIN32
    bool is_windows = true;
#else
    bool is_windows = false;
#endif

#define DBLWIDTH(a, b) a, b
#define TESTDOUBLE(fmt_, num_, res_)                             \
  do {                                                           \
    char t1[40] = "", t2[40] = "";                               \
    const char *N = #num_;                                       \
    mg_snprintf(t1, sizeof(t1), fmt_, num_);                     \
    snprintf(t2, sizeof(t2), fmt_, num_);                        \
    printf("[%s,%s] : [%s] [%s] [%s]\n", fmt_, N, res_, t2, t1); \
    ASSERT(strcmp(t1, res_) == 0);                               \
    if (!is_windows) ASSERT(strcmp(t1, t2) == 0);                \
  } while (0)

#define TESTDOUBLE_NOHOSTCHECK(fmt_, num_, res_)                 \
  do {                                                           \
    char t1[40] = "", t2[40] = "";                               \
    const char *N = #num_;                                       \
    mg_snprintf(t1, sizeof(t1), fmt_, num_);                     \
    snprintf(t2, sizeof(t2), fmt_, num_);                        \
    printf("[%s,%s] : [%s] [%s] [%s]\n", fmt_, N, res_, t2, t1); \
    ASSERT(strcmp(t1, res_) == 0);                               \
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
    TESTDOUBLE("%f", 1000000.0, "1000000.000000");
    TESTDOUBLE("%g", 10000000.0, "1e+07");
    TESTDOUBLE("%f", 10000000.0, "10000000.000000");
    TESTDOUBLE("%g", 100000001.0, "1e+08");
    TESTDOUBLE("%g", 0.1, "0.1");
    TESTDOUBLE("%g", 0.01, "0.01");
    TESTDOUBLE("%g", 0.001, "0.001");
    TESTDOUBLE("%g", 0.0001, "0.0001");
    TESTDOUBLE_NOHOSTCHECK("%g", 0.00001, "0.00001");  // "1e-05"
    TESTDOUBLE("%g", 0.000001, "1e-06");
    TESTDOUBLE("%g", -0.0001, "-0.0001");
    TESTDOUBLE_NOHOSTCHECK("%g", -0.00001, "-0.00001");  // "-1e-05"
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
    TESTDOUBLE("%g", 0.14, "0.14");
    TESTDOUBLE("%f", 0.14, "0.140000");
    TESTDOUBLE("%.*f", DBLWIDTH(4, 0.14), "0.1400");
    TESTDOUBLE("%.*f", DBLWIDTH(3, 0.14), "0.140");
    TESTDOUBLE("%.*f", DBLWIDTH(2, 0.14), "0.14");
    TESTDOUBLE("%.*f", DBLWIDTH(2, 25.14), "25.14");
    TESTDOUBLE("%.*f", DBLWIDTH(1, 0.14), "0.1");
    TESTDOUBLE("%.*f", DBLWIDTH(1, 0.19), "0.2");
    TESTDOUBLE("%.*f", DBLWIDTH(1, 0.16), "0.2");
    TESTDOUBLE_NOHOSTCHECK("%.1f", 0.15, "0.2");
    TESTDOUBLE("%.3f", 123.123456789, "123.123");
    TESTDOUBLE("%.4f", 123.123456789, "123.1235");
    TESTDOUBLE("%.5f", 123.1234567, "123.12346");
    TESTDOUBLE_NOHOSTCHECK("%.*f", DBLWIDTH(1, 0.15), "0.2");
    TESTDOUBLE("%.5f", 123.12345, "123.12345");
    TESTDOUBLE("%.4f", 789.01234, "789.0123");
    TESTDOUBLE("%2.3f", 1.23, "1.230");

    TESTDOUBLE("%.1f", 1.5, "1.5");

    TESTDOUBLE("%.3f", 500.0, "500.000");
    TESTDOUBLE("%.1f", 1.15, "1.1");
    TESTDOUBLE("%.1f", 0.155, "0.2");
    TESTDOUBLE("%.1f", 1.155, "1.2");
    TESTDOUBLE("%.1f", 0.155, "0.2");
    TESTDOUBLE("%.2f", 0.015, "0.01");
    TESTDOUBLE("%.2f", 0.0015, "0.00");
    TESTDOUBLE("%.1f", 0.155, "0.2");

    TESTDOUBLE("%.3f", 13.12505, "13.125");
#if MG_ARCH == MG_ARCH_WIN32 && defined(_MSC_VER) && _MSC_VER < 1700
    // TODO(): for some reason we round down in VC98; skip
#else
    TESTDOUBLE("%.3f", 15.1255, "15.126");
    TESTDOUBLE("%.3f", 19.1255, "19.125");
#endif
    TESTDOUBLE("%.4f", 100.15, "100.1500");
    TESTDOUBLE("%.2f", 5.55, "5.55");

#ifndef _WIN32
    TESTDOUBLE("%g", (double) INFINITY, "inf");
    TESTDOUBLE("%g", (double) -INFINITY, "-inf");
    TESTDOUBLE("%g", (double) NAN, "nan");
#else
    TESTDOUBLE("%g", HUGE_VAL, "inf");
    TESTDOUBLE("%g", -HUGE_VAL, "-inf");
#endif
  }

  {
    const char *expected = "[\"MA==\",\"MAo=\",\"MAr+\",\"MAr+Zw==\"]";
    char tmp[100], s[] = "0\n\xfeg";
    ASSERT(mg_snprintf(tmp, sizeof(tmp), "[%m,%m,%m,%m]", mg_print_base64, 1, s,
                       mg_print_base64, 2, s, mg_print_base64, 3, s,
                       mg_print_base64, 4, s) == 33);
    ASSERT(strcmp(tmp, expected) == 0);
  }

  {
    const char *expected = "\"002001200220616263\"";
    char tmp[100], s[] = "\x00 \x01 \x02 abc";
    ASSERT(mg_snprintf(tmp, sizeof(tmp), "%m", mg_print_hex, 9, s) == 20);
    ASSERT(strcmp(tmp, expected) == 0);
  }

  {
    char tmp[3];
    ASSERT(mg_snprintf(tmp, sizeof(tmp), "%s", "0123456789") == 10);
    ASSERT(strcmp(tmp, "01") == 0);
    ASSERT(tmp[2] == '\0');
  }

  {
    char buf[100];
    struct mg_addr a;
    uint32_t addr = mg_htonl(0x2000001);
    memcpy(a.ip, &addr, sizeof(uint32_t));
    a.port = mg_htons(3);
    a.is_ip6 = false;

    ASSERT(mg_snprintf(buf, sizeof(buf), "%M %d", mg_print_ip, &a, 7) == 9);
    ASSERT(strcmp(buf, "2.0.0.1 7") == 0);
    ASSERT(mg_snprintf(buf, sizeof(buf), "%M %d", mg_print_ip_port, &a, 7) ==
           11);
    ASSERT(strcmp(buf, "2.0.0.1:3 7") == 0);

    memset(a.ip, 0, sizeof(a.ip));
    a.ip[0] = 1, a.ip[1] = 100, a.ip[2] = 33;
    a.is_ip6 = true;
    ASSERT(mg_snprintf(buf, sizeof(buf), "%M %d", mg_print_ip, &a, 7) == 24);
    ASSERT(strcmp(buf, "[164:2100:0:0:0:0:0:0] 7") == 0);
    ASSERT(mg_snprintf(buf, sizeof(buf), "%M %d", mg_print_ip_port, &a, 7) ==
           26);
    ASSERT(strcmp(buf, "[164:2100:0:0:0:0:0:0]:3 7") == 0);
  }

  ASSERT(mg_path_is_sane(mg_str(".")) == true);
  ASSERT(mg_path_is_sane(mg_str("")) == true);
  ASSERT(mg_path_is_sane(mg_str("a.b")) == true);
  ASSERT(mg_path_is_sane(mg_str("a..b")) == true);
  ASSERT(mg_path_is_sane(mg_str(".a")) == true);
  ASSERT(mg_path_is_sane(mg_str(".a.")) == true);
  ASSERT(mg_path_is_sane(mg_str("./")) == true);
  ASSERT(mg_path_is_sane(mg_str("a..")) == true);
  ASSERT(mg_path_is_sane(mg_str("././a..")) == true);
  ASSERT(mg_path_is_sane(mg_str("..")) == false);
  ASSERT(mg_path_is_sane(mg_str("../")) == false);
  ASSERT(mg_path_is_sane(mg_str("./..")) == false);
  ASSERT(mg_path_is_sane(mg_str("a/../")) == false);
  ASSERT(mg_path_is_sane(mg_str("a/../b")) == false);
  ASSERT(mg_path_is_sane(mg_str_n("a/..", 2)) == true);
  ASSERT(mg_path_is_sane(mg_str_n("a/../b", 2)) == true);
}

static void fn1(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ERROR) {
    mg_free(*(char **) c->fn_data);  // See #2263
    *(char **) c->fn_data = mg_mprintf("%s", (char *) ev_data);
  }
  (void) c;
}

static void test_dns_error(const char *dns_server_url, const char *errstr) {
  // Test timeout
  struct mg_mgr mgr;
  char *buf = NULL;
  int i;
  mg_mgr_init(&mgr);
  mgr.dns4.url = dns_server_url;
  mgr.dnstimeout = 10;
  MG_DEBUG(("opening dummy DNS listener @ [%s]...", dns_server_url));
  mg_listen(&mgr, mgr.dns4.url, NULL, NULL);  // Just discard our queries
  mg_http_connect(&mgr, "http://google.com", fn1, &buf);
  for (i = 0; i < 50 && buf == NULL; i++) mg_mgr_poll(&mgr, 1);
  mg_mgr_free(&mgr);
  // MG_DEBUG(("buf: [%s] [%s]", buf, errstr));
  ASSERT(buf != NULL && strcmp(buf, errstr) == 0);
  mg_free(buf);
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

  {
    // DNS Query for domain abc.local
    // 0000   00 00 00 00 00 01 00 00 00 00 00 00 03 61 62 63   .............abc
    // 0010   05 6c 6f 63 61 6c 00 00 01 00 01                  .local.....
    uint8_t d[] = {
        0x00, 0x00,                          // txid: 0
        0x00, 0x00,                          // flags: 0 (Query flag = 0)
        0x00, 0x01,                          // numQuestions: 1
        0x00, 0x00,                          // numAnswers: 1
        0x00, 0x00, 0x00, 0x00,              // additional RRs
        0x03, 0x61, 0x62, 0x63,              // "abc"
        0x05, 0x6c, 0x6f, 0x63, 0x61, 0x6c,  // "local"
        0x00, 0x00, 0x01, 0x00, 0x01         // domain end, type, class
    };
    memset(&dm, 0, sizeof(dm));
    ASSERT(mg_dns_parse(d, sizeof(d), &dm) == 1);
    ASSERT(dm.resolved == false);
    ASSERT(strcmp(dm.name, "abc.local") == 0);
  }

  test_dns_error("udp://127.0.0.1:12345", "DNS timeout");
  test_dns_error("", "resolver");
  test_dns_error("tcp://0.0.0.0:0", "DNS error");
}

static void test_util(void) {
  const char *e;
  char buf[100], *s;
  struct mg_addr a;
  uint32_t ipv4;
  uint16_t port;
  struct mg_str data;
  memset(&a, 0xa5, sizeof(a));
  ASSERT(mg_file_printf(&mg_fs_posix, "data.txt", "%s", "hi") == true);
  // if (system("ls -l") != 0) (void) 0;
  data = mg_file_read(&mg_fs_posix, "data.txt");
  ASSERT(data.buf != NULL);
  ASSERT(strcmp(data.buf, "hi") == 0);
  mg_free((void *) data.buf);
  remove("data.txt");
  ASSERT(mg_aton(mg_str("0"), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0."), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0.256"), &a) == false);
  ASSERT(mg_aton(mg_str("0.0.0.-1"), &a) == false);
  ASSERT(mg_aton(mg_str("127.0.0.1"), &a) == true);
  ASSERT(a.is_ip6 == false);
  memcpy(&ipv4, a.ip, sizeof(ipv4));
  ASSERT(ipv4 == mg_htonl(0x7f000001));
  ASSERT(mg_ntohl(ipv4) == 0x7f000001);
  MG_STORE_BE32(&ipv4, 0x5678abcd);
  ASSERT(((uint8_t *) &ipv4)[0] == 0x56 && ((uint8_t *) &ipv4)[1] == 0x78 &&
         ((uint8_t *) &ipv4)[2] == 0xab && ((uint8_t *) &ipv4)[3] == 0xcd);
  ASSERT(MG_LOAD_BE32(&ipv4) == 0x5678abcd);
  MG_STORE_BE16(&port, 0x1234);
  ASSERT(((uint8_t *) &port)[0] == 0x12 && ((uint8_t *) &port)[1] == 0x34);
  ASSERT(MG_LOAD_BE16(&port) == 0x1234);
  ASSERT(port == mg_htons(0x1234));
  ASSERT(mg_ntohs(port) == 0x1234);
  MG_STORE_BE24(&ipv4, 0xef2345);
  ASSERT(((uint8_t *) &ipv4)[0] == 0xef && ((uint8_t *) &ipv4)[1] == 0x23 &&
         ((uint8_t *) &ipv4)[2] == 0x45);
  ASSERT(MG_LOAD_BE24(&ipv4) == 0xef2345);

  memset(a.ip, 0xa5, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("1:2:3:4:5:6:7:8"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00\x06\x00\x07\x00\x08";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xa5, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("1:2::3"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x00\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x03";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("1::1"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("::fFff:1.2.3.4"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\x01\x02\x03\x04";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("::1"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(a.scope_id == 0);
  e = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("1::"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("2001:4860:4860::8888"), &a) == true);
  ASSERT(a.is_ip6 == true);
  e = "\x20\x01\x48\x60\x48\x60\x00\x00\x00\x00\x00\x00\x00\x00\x88\x88";
  ASSERT(memcmp(a.ip, e, sizeof(a.ip)) == 0);

  ASSERT(mg_url_decode("a=%", 3, buf, sizeof(buf), 0) < 0);
  ASSERT(mg_url_decode("&&&a=%", 6, buf, sizeof(buf), 0) < 0);
  ASSERT(mg_url_decode("a=%1", 4, buf, sizeof(buf), 0) < 0);
  ASSERT(mg_url_decode("a=%12", 5, buf, sizeof(buf), 0) == 3 && buf[2] == 0x12);
  ASSERT(mg_url_decode("a=%123", 6, buf, sizeof(buf), 0) == 4 &&
         buf[2] == 0x12 && buf[3] == '3');

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("::1%1"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(a.scope_id == 1);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("abcd::aabb:ccdd%17"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(a.scope_id == 17);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("::1%17"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(a.scope_id == 17);

  memset(a.ip, 0xaa, sizeof(a.ip));
  ASSERT(mg_aton(mg_str("::1%255"), &a) == true);
  ASSERT(a.is_ip6 == true);
  ASSERT(a.scope_id == 255);

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
    s = mg_mprintf("%3d", 123);
    ASSERT(strcmp(s, "123") == 0);
    mg_free(s);
  }

  {
    extern bool mg_to_size_t(struct mg_str, size_t *);
    size_t val, max = (size_t) -1;
    ASSERT(mg_to_size_t(mg_str("0"), &val) && val == 0);
    ASSERT(mg_to_size_t(mg_str("123"), &val) && val == 123);
    ASSERT(mg_to_size_t(mg_str(" 123 \t"), &val) && val == 123);
    ASSERT(mg_to_size_t(mg_str(""), &val) == false);
    ASSERT(mg_to_size_t(mg_str(" 123x"), &val) == false);
    ASSERT(mg_to_size_t(mg_str("-"), &val) == false);
    mg_snprintf(buf, sizeof(buf), sizeof(max) == 8 ? "%llu" : "%lu", max);
    ASSERT(mg_to_size_t(mg_str(buf), &val) && val == max);
  }

  {
    uint64_t val, max = (uint64_t) -1;
    ASSERT(mg_str_to_num(mg_str("0"), 10, &val, sizeof(uint64_t)) && val == 0);
    ASSERT(mg_str_to_num(mg_str("123"), 10, &val, sizeof(uint64_t)) &&
           val == 123);
    ASSERT(mg_str_to_num(mg_str(" 123"), 10, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("123 "), 10, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str(""), 10, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str(" 123x"), 10, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("-"), 10, &val, sizeof(uint64_t)) == false);
    mg_snprintf(buf, sizeof(buf), "%llu", max);
    ASSERT(mg_str_to_num(mg_str(buf), 10, &val, sizeof(uint64_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("0"), 2, &val, sizeof(uint64_t)) && val == 0);
    ASSERT(mg_str_to_num(mg_str("1"), 2, &val, sizeof(uint64_t)) && val == 1);
    ASSERT(mg_str_to_num(mg_str("0123"), 2, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("123"), 2, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("01111011"), 2, &val, sizeof(uint64_t)) &&
           val == 123);
    ASSERT(mg_str_to_num(mg_str("1111111111111111111111111111111111111111111111"
                                "111111111111111111"),
                         2, &val, sizeof(uint64_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("0"), 16, &val, sizeof(uint64_t)) && val == 0);
    ASSERT(mg_str_to_num(mg_str("123"), 16, &val, sizeof(uint64_t)) &&
           val == 0x123);
    ASSERT(mg_str_to_num(mg_str("def"), 16, &val, sizeof(uint64_t)) &&
           val == 0xdef);
    ASSERT(mg_str_to_num(mg_str("defg"), 16, &val, sizeof(uint64_t)) == false);
    mg_snprintf(buf, sizeof(buf), "%llx", max);
    ASSERT(mg_str_to_num(mg_str(buf), 16, &val, sizeof(uint64_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("0x123"), 0, &val, sizeof(uint64_t)) &&
           val == 0x123);
    ASSERT(mg_str_to_num(mg_str("0b123"), 0, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("0c123"), 0, &val, sizeof(uint64_t)) == false);
    ASSERT(mg_str_to_num(mg_str("0b101"), 0, &val, sizeof(uint64_t)) &&
           val == 5);
    ASSERT(mg_str_to_num(mg_str("0123"), 0, &val, sizeof(uint64_t)) &&
           val == 123);
  }
  {
    uint32_t val, max = (uint32_t) -1;
    ASSERT(mg_str_to_num(mg_str("123"), 10, &val, sizeof(uint32_t)) &&
           val == 123);
    mg_snprintf(buf, sizeof(buf), "%lu", (unsigned long) max);
    ASSERT(strcmp(buf, "4294967295") == 0);
    ASSERT(mg_str_to_num(mg_str(buf), 10, &val, sizeof(uint32_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("01111011"), 2, &val, sizeof(uint32_t)) &&
           val == 123);
    ASSERT(mg_str_to_num(mg_str("11111111111111111111111111111111"), 2, &val,
                         sizeof(uint32_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("0"), 16, &val, sizeof(uint32_t)) && val == 0);
    ASSERT(mg_str_to_num(mg_str("123"), 16, &val, sizeof(uint32_t)) &&
           val == 0x123);
    mg_snprintf(buf, sizeof(buf), "%lx", max);
    ASSERT(mg_str_to_num(mg_str(buf), 16, &val, sizeof(uint32_t)) &&
           val == max);
  }
  {
    uint16_t val, max = (uint16_t) -1;
    ASSERT(mg_str_to_num(mg_str("123"), 10, &val, sizeof(uint16_t)) &&
           val == 123);
    mg_snprintf(buf, sizeof(buf), "%u", max);
    ASSERT(mg_str_to_num(mg_str(buf), 10, &val, sizeof(uint16_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("01111011"), 2, &val, sizeof(uint16_t)) &&
           val == 123);
    ASSERT(
        mg_str_to_num(mg_str("1111111111111111"), 2, &val, sizeof(uint16_t)) &&
        val == max);
    ASSERT(mg_str_to_num(mg_str("123"), 16, &val, sizeof(uint16_t)) &&
           val == 0x123);
    mg_snprintf(buf, sizeof(buf), "%x", max);
    ASSERT(mg_str_to_num(mg_str(buf), 16, &val, sizeof(uint16_t)) &&
           val == max);
  }
  {
    uint8_t val, max = (uint8_t) -1;
    ASSERT(mg_str_to_num(mg_str("123"), 10, &val, sizeof(uint8_t)) &&
           val == 123);
    mg_snprintf(buf, sizeof(buf), "%u", max);
    ASSERT(mg_str_to_num(mg_str(buf), 10, &val, sizeof(uint8_t)) && val == max);
    ASSERT(mg_str_to_num(mg_str("01111011"), 2, &val, sizeof(uint8_t)) &&
           val == 123);
    ASSERT(mg_str_to_num(mg_str("11111111"), 2, &val, sizeof(uint8_t)) &&
           val == max);
    ASSERT(mg_str_to_num(mg_str("12"), 16, &val, sizeof(uint8_t)) &&
           val == 0x12);
    mg_snprintf(buf, sizeof(buf), "%x", max);
    ASSERT(mg_str_to_num(mg_str(buf), 16, &val, sizeof(uint8_t)) && val == max);
  }

  {
    size_t i;
    memset(buf, ' ', sizeof(buf));
    mg_random_str(buf, 0);
    ASSERT(buf[0] == ' ');
    mg_random_str(buf, 1);
    ASSERT(buf[0] == '\0');
    ASSERT(buf[1] == ' ');
    mg_random_str(buf, sizeof(buf));
    ASSERT(buf[sizeof(buf) - 1] == '\0');
    for (i = 0; i < sizeof(buf) - 1; i++) ASSERT(isalnum((uint8_t) buf[i]));
  }
}

static void test_crc32(void) {
  //  echo -n aaa | cksum -o3
  ASSERT(mg_crc32(0, 0, 0) == 0);
  ASSERT(mg_crc32(0, "a", 1) == 3904355907);
  ASSERT(mg_crc32(0, "abc", 3) == 891568578);
  ASSERT(mg_crc32(mg_crc32(0, "ab", 2), "c", 1) == 891568578);
}

static void us(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;
  if (ev == MG_EV_HTTP_MSG && mg_match(hm->uri, mg_str("/upload"), NULL)) {
    MG_DEBUG(("Got all %lu bytes!", (unsigned long) hm->body.len));
    MG_DEBUG(("Query string: [%.*s]", (int) hm->query.len, hm->query.buf));
    // MG_DEBUG(("Body:\n%.*s", (int) hm->body.len, hm->body.buf));
    mg_http_reply(c, 200, "", "ok (%d %.*s)\n", (int) hm->body.len,
                  (int) hm->body.len, hm->body.buf);
  } else if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "ok\n");
  }
}

static void uc(struct mg_connection *c, int ev, void *ev_data) {
  const char **s = (const char **) c->fn_data;
  if (ev == MG_EV_OPEN) {
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    mg_printf(c,
              "POST /upload HTTP/1.0\r\n"
              "Transfer-Encoding: chunked\r\n\r\n");
    mg_http_printf_chunk(c, "%s", "foo\n");
    mg_http_printf_chunk(c, "%s", "bar\n");
    mg_http_printf_chunk(c, "");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // MG_INFO(("---> [%s] [%.*s]", *s, hm->body.len, hm->body.buf));
    ASSERT(mg_strcmp(hm->body, mg_str(*s)) == 0);
    *s = NULL;
  }
}

static void test_http_upload(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12352";
  int i;
  const char *s = "ok (8 foo\nbar\n)\n";

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, us, NULL);
  mg_http_connect(&mgr, url, uc, (void *) &s);
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 5);
  ASSERT(s == NULL);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

#define LONG_CHUNK "chunk with length taking up more than two hex digits"

static void eX(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    c->data[0] = 1;
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_POLL && c->data[0] != 0) {
    c->data[0]++;
    if (c->data[0] == 10) mg_http_printf_chunk(c, "a");
    if (c->data[0] == 20) {
      mg_http_printf_chunk(c, "b");
      mg_http_printf_chunk(c, "c");
    }
    if (c->data[0] == 30) {
      mg_http_printf_chunk(c, "d");
      mg_http_printf_chunk(c, "");
      c->data[0] = 0;
    }
  }
  (void) ev_data;
}

static void eY(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 4\r\n\r\n");
    c->data[0] = 1;
  } else if (ev == MG_EV_POLL && c->data[0] != 0) {
    c->data[0]++;
    if (c->data[0] == 10) mg_send(c, "a", 1);
    if (c->data[0] == 12) mg_send(c, "bc", 2);
    if (c->data[0] == 30) mg_send(c, "d", 1), c->is_resp = 0, c->data[0] = 0;
  }
  (void) ev_data;
}

static void eZ(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    mg_http_reply(c, 200, "", "abcd");
  }
  (void) ev_data;
}

// Do not delete chunks as they arrive
static void eh4(struct mg_connection *c, int ev, void *ev_data) {
  uint32_t *crc = (uint32_t *) c->fn_data;
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    *crc = mg_crc32(*crc, hm->body.buf, hm->body.len);
    MG_INFO(("%lu M [%.*s]", c->id, (int) hm->body.len, hm->body.buf));
  }
}

static void test_http_chunked_case(mg_event_handler_t s, mg_event_handler_t c,
                                   int req_count, const char *expected) {
  char url[100];
  struct mg_mgr mgr;
  uint32_t i, crc = 0, expected_crc = mg_crc32(0, expected, strlen(expected));
  struct mg_connection *conn;
  static uint16_t port = 32344;  // To prevent bind errors on Windows
  mg_snprintf(url, sizeof(url), "http://127.0.0.1:%d", port++);
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, s, NULL);
  conn = mg_http_connect(&mgr, url, c, &crc);
  while (conn != NULL && req_count-- > 0) {
    mg_printf(conn, "GET / HTTP/1.0\n\n");
  }
  for (i = 0; i < 100 && crc != expected_crc; i++) {
    mg_mgr_poll(&mgr, 1);
  }
  // MG_INFO(("-------- %d [%s]", i, expected));
  ASSERT(i < 100);
  ASSERT(crc == expected_crc);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void test_http_chunked(void) {
  // Non-chunked encoding
  test_http_chunked_case(eY, eh4, 1, "abcd");  // Chunks not deleted
  test_http_chunked_case(eY, eh4, 2, "abcdabcd");
  test_http_chunked_case(eZ, eh4, 1, "abcd");  // Not deleted
  test_http_chunked_case(eZ, eh4, 2, "abcdabcd");

  // Chunked encoding
  test_http_chunked_case(eX, eh4, 1, "abcd");  // Chunks not deleted
  test_http_chunked_case(eX, eh4, 2, "abcdabcd");
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
static void eh8(struct mg_connection *c, int ev, void *ev_data) {
  struct stream_status *status = (struct stream_status *) c->fn_data;
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
static void eh10(struct mg_connection *c, int ev, void *ev_data) {
  if (c->recv.len >= MG_MAX_RECV_SIZE && ev == MG_EV_READ) c->is_full = true;

  eh8(c, ev, ev_data);

  if (c->recv.len < MG_MAX_RECV_SIZE && ev == MG_EV_POLL) c->is_full = false;
}

// Send buffer larger than MG_MAX_RECV_SIZE to server
static void eh11(struct mg_connection *c, int ev, void *ev_data) {
  struct stream_status *status = (struct stream_status *) c->fn_data;
  if (ev == MG_EV_CONNECT) {
    size_t len = MG_MAX_RECV_SIZE * 2;
    struct mg_iobuf buf = {NULL, 0, 0, 0};
    mg_iobuf_init(&buf, len, 0);
    mg_random(buf.buf, buf.size);
    buf.len = buf.size;
    mg_send(c, buf.buf, buf.len);
    status->sent = buf.len;
    status->send_crc = mg_crc32(0, (const char *) buf.buf, buf.len);
    mg_iobuf_free(&buf);
  }
  (void) ev_data;
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
  // MG_INFO(("--> [%.*s]", (int) part.body.len, part.body.buf));
  ASSERT(mg_strcmp(part.body, mg_str("abc\r\ndef")) == 0);
  ASSERT(part.filename.len == 0);
  ASSERT((ofs = mg_http_next_multipart(mg_str(s), ofs, &part)) > 0);
  ASSERT(mg_strcmp(part.name, mg_str("foo")) == 0);
  // MG_INFO(("--> [%.*s]", (int) part.filename.len, part.filename.buf));
  ASSERT(mg_strcmp(part.filename, mg_str("a b.txt")) == 0);
  ASSERT(mg_strcmp(part.body, mg_str("hello world\r\n")) == 0);
  ASSERT(mg_http_next_multipart(mg_str(s), ofs, &part) == 0);
}

static void eh7(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts sopts;
    memset(&sopts, 0, sizeof(sopts));
    sopts.root_dir = "/";
    sopts.fs = &mg_fs_packed;
    mg_http_serve_dir(c, hm, &sopts);
  }
}

static void test_packed(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12351";
  char buf[FETCH_BUF_SIZE];
  struct mg_str data = mg_file_read(&mg_fs_posix, "Makefile");
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh7, NULL);

  // Load top level file directly
  // fetch(&mgr, buf, url, "GET /Makefile HTTP/1.0\n\n");
  // printf("---> %s\n", buf);
  ASSERT(fetch(&mgr, buf, url, "GET /Makefile HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, data.buf) == 0);
  mg_free((void *) data.buf);

  // Load file deeper in the FS tree directly
  data = mg_file_read(&mg_fs_posix, "data/ssi.h");
  ASSERT(fetch(&mgr, buf, url, "GET /data/ssi.h HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, data.buf) == 0);
  mg_free((void *) data.buf);

  // List root dir
  ASSERT(fetch(&mgr, buf, url, "GET / HTTP/1.0\n\n") == 200);
  // printf("--------\n%s\n", buf);

  // List nested dir
  ASSERT(fetch(&mgr, buf, url, "GET /data HTTP/1.0\n\n") == 301);
  ASSERT(fetch(&mgr, buf, url, "GET /data/ HTTP/1.0\n\n") == 200);
  // printf("--------\n%s\n", buf);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

#if (MG_ENABLE_SOCKET == 0)
int send(int sock, const void *buf, size_t len, int flags);
int send(int sock, const void *buf, size_t len, int flags) {
  (void) sock, (void) buf, (void) len, (void) flags;
  return -1;
}
#endif

static void u1(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_CONNECT) {
    ((int *) c->fn_data)[0] += 1;
    mg_send(c, "hi", 2);
  } else if (ev == MG_EV_WRITE) {
    ((int *) c->fn_data)[0] += 100;
  } else if (ev == MG_EV_READ) {
    ((int *) c->fn_data)[0] += 10;
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
  struct mg_addr ip = {{1, 2, 3, 4}, 0, 0, false};  // 1.2.3.4
  ASSERT(mg_check_ip_acl(mg_str(NULL), &ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str(""), &ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("invalid"), &ip) == -1);
  ASSERT(mg_check_ip_acl(mg_str("+hi"), &ip) == -2);
  ASSERT(mg_check_ip_acl(mg_str("+//"), &ip) == -2);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0"), &ip) == 0);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.0.0.0/8"), &ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.2.3.4"), &ip) == 1);
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0,+1.0.0.0/16"), &ip) == 0);
  ip.is_ip6 = true;
  ASSERT(mg_check_ip_acl(mg_str("-0.0.0.0/0"), &ip) ==
         -1);  // not yet supported
}

static void w3(struct mg_connection *c, int ev, void *ev_data) {
  // MG_INFO(("ev %d", ev));
  if (ev == MG_EV_WS_OPEN) {
    char buf[8192];
    memset(buf, 'A', sizeof(buf));
    mg_ws_send(c, "hi there!", 9, WEBSOCKET_OP_TEXT);
    mg_ws_printf(c, WEBSOCKET_OP_TEXT, "%s", "hi there2!");
    mg_printf(c, "%s", "boo");
    mg_ws_wrap(c, 3, WEBSOCKET_OP_TEXT);
    mg_ws_send(c, buf, sizeof(buf), WEBSOCKET_OP_TEXT);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    ASSERT(mg_strcmp(wm->data, mg_str("lebowski")) == 0);
    ((int *) c->fn_data)[0]++;
  } else if (ev == MG_EV_CLOSE) {
    ((int *) c->fn_data)[0] += 10;
  }
}

static void w2(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_str msg = mg_str_n("lebowski", 8);
  if (ev == MG_EV_HTTP_MSG) {
    mg_ws_upgrade(c, (struct mg_http_message *) ev_data, NULL);
  } else if (ev == MG_EV_WS_OPEN) {
    mg_ws_send(c, "x", 1, WEBSOCKET_OP_PONG);
  } else if (ev == MG_EV_POLL && c->is_websocket) {
    size_t ofs, n = (size_t) c->fn_data;
    if (n < msg.len) {
      // Send "msg" char by char using fragmented frames
      // mg_ws_send() sets the FIN flag in the WS header. Clean it
      // to send fragmented packet. Insert PONG messages between frames
      uint8_t op = n == 0 ? WEBSOCKET_OP_TEXT : WEBSOCKET_OP_CONTINUE;
      mg_ws_send(c, ":->", 3, WEBSOCKET_OP_PING);
      ofs = c->send.len;
      mg_ws_send(c, &msg.buf[n], 1, op);
      if (n < msg.len - 1) c->send.buf[ofs] = op;  // Clear FIN flag
      c->fn_data = (void *) (n + 1);               // Point to the next char
    } else {
      mg_ws_send(c, "", 0, WEBSOCKET_OP_CLOSE);
    }
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    MG_INFO(("Got WS, %lu", wm->data.len));
    // mg_hexdump(wm->data.buf, wm->data.len);
    if (wm->data.len == 9) {
      ASSERT(mg_strcmp(wm->data, mg_str("hi there!")) == 0);
    } else if (wm->data.len == 10) {
      ASSERT(mg_strcmp(wm->data, mg_str("hi there2!")) == 0);
    } else if (wm->data.len == 3) {
      ASSERT(mg_strcmp(wm->data, mg_str("boo")) == 0);
    } else {
      MG_INFO(("%lu", wm->data.len));
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

static void h7(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct mg_http_serve_opts opts;
    memset(&opts, 0, sizeof(opts));
    opts.root_dir = "./data,/foo=./dirtest";
    mg_http_serve_dir(c, hm, &opts);
  }
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

static void json_scan(struct mg_str json, int depth) {
  int i, n = 0, o = mg_json_get(json, "$", &n);
  for (i = 0; i < depth; i++) printf("  ");
  printf("%.*s\n", n, json.buf + o);
  if (json.buf[o] == '{' || json.buf[o] == '[') {  // Iterate over elems
    struct mg_str key, val, sub = mg_str_n(json.buf + o, (size_t) n);
    size_t ofs = 0;
    while ((ofs = mg_json_next(sub, ofs, &key, &val)) > 0) {
      for (i = 0; i < depth; i++) printf("  ");
      printf("KEY: %.*s VAL: %.*s\n", (int) key.len, key.buf, (int) val.len,
             val.buf);
      if (*val.buf == '[' || *val.buf == '{') json_scan(val, depth + 1);
    }
  }
}

static void test_json(void) {
  const char *s1 = "{\"a\":{},\"b\":7,\"c\":[[],2]}";
  const char *s2 = "{\"a\":{\"b1\":{}},\"c\":7,\"d\":{\"b2\":{}}}";
  int n;
  struct mg_str json;

  ASSERT(mg_json_get(mg_str_n(" true ", 6), "", &n) == MG_JSON_INVALID);
  ASSERT(mg_json_get(mg_str_n(" true ", 6), "$", &n) == 1 && n == 4);
  ASSERT(mg_json_get(mg_str_n("null ", 5), "$", &n) == 0 && n == 4);
  json = mg_str("  \"hi\\nthere\"");
  ASSERT(mg_json_get(json, "$", &n) == 2 && n == 11);
  ASSERT(mg_json_get(mg_str_n(" { } ", 5), "$", &n) == 1);
  ASSERT(mg_json_get(mg_str_n(" [[]]", 5), "$", &n) == 1);
  ASSERT(mg_json_get(mg_str_n(" [ ]  ", 5), "$", &n) == 1);

  ASSERT(mg_json_get(mg_str_n("[1,2]", 5), "$", &n) == 0 && n == 5);
  ASSERT(mg_json_get(mg_str_n("[1,2]", 5), "$[0]", &n) == 1 && n == 1);
  ASSERT(mg_json_get(mg_str_n("[1,2]", 5), "$[1]", &n) == 3 && n == 1);
  ASSERT(mg_json_get(mg_str_n("[1,2]", 5), "$[3]", &n) == MG_JSON_NOT_FOUND);

  json = mg_str("{\"a\":[]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 2);
  json = mg_str("{\"a\":[1,2]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 5);
  json = mg_str("{\"a\":[1,[1]]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 7);
  json = mg_str("{\"a\":[[]]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 4);
  json = mg_str("{\"a\":[[1,2]]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 7);
  json = mg_str("{\"a\":{}}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 2);
  json = mg_str("{\"a\":{\"a\":{}}}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 8);
  json = mg_str("{\"a\":{\"a\":[]}}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 8);

  json = mg_str("[[1,[2,3]],4]");
  ASSERT(mg_json_get(json, "$", &n) == 0 && n == 13);
  ASSERT(mg_json_get(json, "$[0]", &n) == 1 && n == 9);
  ASSERT(mg_json_get(json, "$[1]", &n) == 11);
  ASSERT(mg_json_get(json, "$[1]", &n) == 11 && n == 1);
  ASSERT(mg_json_get(json, "$[2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$[0][0]", &n) == 2 && n == 1);
  ASSERT(mg_json_get(json, "$[0][1]", &n) == 4 && n == 5);
  ASSERT(mg_json_get(json, "$[0][2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$[0][1][0]", &n) == 5 && n == 1);
  ASSERT(mg_json_get(json, "$[0][1][1]", &n) == 7 && n == 1);

  json = mg_str("[[1,2],3]");
  ASSERT(mg_json_get(json, "$", &n) == 0 && n == 9);
  ASSERT(mg_json_get(json, "$[0][0]", &n) == 2 && n == 1);
  ASSERT(mg_json_get(json, "$[0][1]", &n) == 4 && n == 1);
  ASSERT(mg_json_get(json, "$[0][2]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$[1][0]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$[1]", &n) == 7 && n == 1);
  ASSERT(mg_json_get(json, "$[1][0]", &n) == MG_JSON_NOT_FOUND);

  ASSERT(mg_json_get(json, "$", &n) == 0 && n == 9);
  ASSERT(mg_json_get(json, "$[1][0]", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$[0][1]", &n) == 4 && n == 1);

  json = mg_str(s1);
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 2);
  ASSERT(mg_json_get(json, "$.b", &n) == 12 && n == 1);
  ASSERT(mg_json_get(json, "$.c", &n) == 18 && n == 6);
  ASSERT(mg_json_get(json, "$.c[0]", &n) == 19 && n == 2);
  ASSERT(mg_json_get(json, "$.c[1]", &n) == 22 && n == 1);
  ASSERT(mg_json_get(json, "$.c[3]", &n) == MG_JSON_NOT_FOUND);

  json = mg_str(s2);
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 9);
  ASSERT(mg_json_get(json, "$.a.b1", &n) == 11 && n == 2);
  ASSERT(mg_json_get(json, "$.a.b2", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$.a.b", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$.a1", &n) == MG_JSON_NOT_FOUND);
  ASSERT(mg_json_get(json, "$.c", &n) == 19 && n == 1);

  {
    char to[4], expect[4] = {0, 0, 0, 0};
    memset(to, 0, sizeof(to));
    ASSERT(mg_json_unescape(mg_str("\\u0000"), to, 4) &&
           memcmp(to, expect, 4) == 0);
    to[0] = 0;
    expect[0] = (char) 0xff;
    ASSERT(mg_json_unescape(mg_str("\\u00ff"), to, 4) &&
           memcmp(to, expect, 4) == 0);
    ASSERT(!mg_json_unescape(mg_str("\\u0100"), to, 4));
    ASSERT(!mg_json_unescape(mg_str("\\u1000"), to, 4));
  }

  {
    double d = 0;
    bool b = false;
    int len;
    char *str = NULL;

    json = mg_str("{\"a\":\"b\"}");
    str = mg_json_get_str(json, "$.a");
    ASSERT(str != NULL);
    // printf("---> [%s]\n", str);
    ASSERT(strcmp(str, "b") == 0);
    mg_free(str);

    json = mg_str("{\"a\": \"hi\\nthere\",\"b\": [12345, true]}");
    str = mg_json_get_str(json, "$.a");

    ASSERT(str != NULL);
    ASSERT(strcmp(str, "hi\nthere") == 0);
    mg_free(str);

    ASSERT(mg_json_get_long(json, "$.foo", -42) == -42);
    ASSERT(mg_json_get_long(json, "$.b[0]", -42) == 12345);

    ASSERT(mg_json_get_num(json, "$.a", &d) == false);
    ASSERT(mg_json_get_num(json, "$.c", &d) == false);
    ASSERT(mg_json_get_num(json, "$.b[0]", &d) == true);
    ASSERT(d == 12345);

    ASSERT(mg_json_get_bool(json, "$.b", &b) == false);
    ASSERT(mg_json_get_bool(json, "$.b[0]", &b) == false);
    ASSERT(mg_json_get_bool(json, "$.b[1]", &b) == true);
    ASSERT(b == true);
    ASSERT(mg_json_get(json, "$.b[2]", &len) < 0);

    json = mg_str("[\"YWJj\", \"0100026869\"]");
    ASSERT((str = mg_json_get_b64(json, "$[0]", &len)) != NULL);
    ASSERT(len == 3 && memcmp(str, "abc", (size_t) len) == 0);
    mg_free(str);
    ASSERT((str = mg_json_get_hex(json, "$[1]", &len)) != NULL);
    ASSERT(len == 5 && memcmp(str, "\x01\x00\x02hi", (size_t) len) == 0);
    mg_free(str);

    json = mg_str("{\"a\":[1,2,3], \"ab\": 2}");
    ASSERT(mg_json_get_long(json, "$.a[0]", -42) == 1);
    ASSERT(mg_json_get_long(json, "$.ab", -42) == 2);
    ASSERT(mg_json_get_long(json, "$.ac", -42) == -42);

    json = mg_str("{\"a\":[],\"b\":[1,2]}");
    ASSERT(mg_json_get_long(json, "$.a[0]", -42) == -42);
    ASSERT(mg_json_get_long(json, "$.b[0]", -42) == 1);
    ASSERT(mg_json_get_long(json, "$.b[1]", -42) == 2);
    ASSERT(mg_json_get_long(json, "$.b[2]", -42) == -42);
    json = mg_str("[{\"a\":1,\"b\":2},{\"a\":3, \"b\":4}]");
    ASSERT(mg_json_get_long(json, "$[0].a", -42) == 1);
    ASSERT(mg_json_get_long(json, "$[0].b", -42) == 2);
    ASSERT(mg_json_get_long(json, "$[1].a", -42) == 3);
    ASSERT(mg_json_get_long(json, "$[1].b", -42) == 4);
    ASSERT(mg_json_get_long(json, "$[2].a", -42) == -42);

    json = mg_str("{\"a\":[1],\"b\":[2,3]}");
    ASSERT(mg_json_get_long(json, "$.a[0]", -42) == 1);
    ASSERT(mg_json_get_long(json, "$.a[1]", -42) == -42);

    json = mg_str("{\"a\":[1,[2,3], 4]}");
    ASSERT(mg_json_get_long(json, "$.a[0]", -42) == 1);
    ASSERT(mg_json_get_long(json, "$.a[1][0]", -42) == 2);
    ASSERT(mg_json_get_long(json, "$.a[1][1]", -42) == 3);
    ASSERT(mg_json_get_long(json, "$.a[1][2]", -42) == -42);
    ASSERT(mg_json_get_long(json, "$.a[2]", -42) == 4);
    ASSERT(mg_json_get_long(json, "$.a[3]", -42) == -42);
  }

  json = mg_str("{\"a\":[],\"b\":[1,2]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 2);
  ASSERT(mg_json_get(json, "$.a[0]", &n) < 0 && n == 0);

  json = mg_str("{\"a\":{},\"b\":[1,2]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 2);
  ASSERT(mg_json_get(json, "$.a[0]", &n) < 0 && n == 0);

  json = mg_str("{\"a\":true,\"b\":[1,2]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 4);
  ASSERT(mg_json_get(json, "$.a[0]", &n) < 0 && n == 0);

  json = mg_str("{\"a\":1,\"b\":[1,2]}");
  ASSERT(mg_json_get(json, "$.a", &n) == 5 && n == 1);
  ASSERT(mg_json_get(json, "$.a[0]", &n) < 0 && n == 0);

  ASSERT(mg_json_get_long(mg_str("[0, 42]"), "$[1]", 0) == 42);
  ASSERT(mg_json_get_long(mg_str("[[], 42]"), "$[1]", 0) == 42);
  ASSERT(mg_json_get_long(mg_str("[{}, 42]"), "$[1]", 0) == 42);

  json = mg_str("[{\"a\":1},{\"a\":2}]");
  ASSERT(mg_json_get_long(json, "$[0]", -1) == -1);
  ASSERT(mg_json_get_long(json, "$[0].a", -1) == 1);
  ASSERT(mg_json_get_long(json, "$[1].a", -1) == 2);
  ASSERT(mg_json_get_long(json, "$[2].a", -1) == -1);

  json = mg_str("[{\"a1\":1},{\"a\":2}]");
  ASSERT(mg_json_get_long(json, "$[0]", -1) == -1);
  ASSERT(mg_json_get_long(json, "$[0].a", -1) == -1);
  ASSERT(mg_json_get_long(json, "$[1].a", -1) == 2);
  ASSERT(mg_json_get_long(json, "$[2].a", -1) == -1);

  // mg_json_next()
  json = mg_str("[1,true,{\"a\":[3],\"b\":42}]");
  json_scan(json, 0);
  {
    struct mg_str k, v, sub = mg_str_n(json.buf + 8, json.len - 8);
    const char *a = "\"a\"", *b = "\"b\"";
    ASSERT(mg_json_next(sub, 0, &k, &v) == 9);
    ASSERT(vcmp(k, a));
    ASSERT(vcmp(v, "[3]"));
    ASSERT(mg_json_next(sub, 9, &k, &v) == 15);
    ASSERT(vcmp(k, b));
    ASSERT(vcmp(v, "42"));
    ASSERT(mg_json_next(sub, 15, &k, &v) == 0);
  }

  {
    struct mg_str expected = mg_str("\"b:c\""), val;
    json = mg_str("{\"a\":\"b:c\"}");
    val = mg_json_get_tok(json, "$.a");
    ASSERT(mg_strcmp(val, expected) == 0);
  }
}

static void resp_rpc(struct mg_rpc_req *r) {
  int len = 0, off = mg_json_get(r->frame, "$.result", &len);
  mg_xprintf(r->pfn, r->pfn_data, "%.*s", len, &r->frame.buf[off]);
}

static void test_rpc(void) {
  struct mg_rpc *head = NULL;
  struct mg_iobuf io = {0, 0, 0, 256};
  struct mg_rpc_req req = {&head, 0, mg_pfn_iobuf, &io, 0, {0, 0}};
  mg_rpc_add(&head, mg_str("rpc.list"), mg_rpc_list, NULL);

  {
    req.frame = mg_str("{\"method\":\"rpc.list\"}");
    mg_rpc_process(&req);
    ASSERT(io.buf == NULL);
  }

  {
    const char *resp = "{\"id\":1,\"result\":[\"rpc.list\"]}";
    req.frame = mg_str("{\"id\": 1,\"method\":\"rpc.list\"}");
    mg_rpc_process(&req);
    ASSERT(strcmp((char *) io.buf, resp) == 0);
    mg_iobuf_free(&io);
  }

  {
    const char *resp =
        "{\"id\":true,\"error\":{\"code\":-32601,\"message\":\"foo not "
        "found\"}}";
    req.frame = mg_str("{\"id\": true,\"method\":\"foo\"}");
    mg_rpc_process(&req);
    // MG_INFO(("-> %s", io.buf));
    ASSERT(strcmp((char *) io.buf, resp) == 0);
    mg_iobuf_free(&io);
  }

  {
    const char *resp =
        "{\"id\":true,\"error\":{\"code\":-32601,\"message\":\"foo not "
        "found\"}}";
    req.frame = mg_str("{\"id\": true,\"method\":\"foo\"}");
    req.head = NULL;
    mg_rpc_process(&req);
    // MG_INFO(("-> %s", io.buf));
    ASSERT(strcmp((char *) io.buf, resp) == 0);
    mg_iobuf_free(&io);
    req.head = &head;
  }

  {
    const char *resp = "{\"error\":{\"code\":-32700,\"message\":\"haha\"}}";
    req.frame = mg_str("haha");
    mg_rpc_process(&req);
    // MG_INFO(("-> %s", io.buf));
    ASSERT(strcmp((char *) io.buf, resp) == 0);
    mg_iobuf_free(&io);
  }

  {
    const char *resp =
        "{\"id\":1,\"error\":{\"code\":-32601,\"message\":\" not found\"}}";
    req.frame = mg_str("{\"id\":1,\"result\":123}");
    mg_rpc_process(&req);
    // MG_INFO(("-> %s", io.buf));
    ASSERT(strcmp((char *) io.buf, resp) == 0);
    mg_iobuf_free(&io);
  }

  {
    req.frame = mg_str("{\"id\":1,\"result\":123}");
    mg_rpc_add(&head, mg_str(""), resp_rpc, NULL);
    mg_rpc_process(&req);
    MG_INFO(("-> %s", io.buf));
    ASSERT(strcmp((char *) io.buf, "123") == 0);
    mg_iobuf_free(&io);
  }

  mg_rpc_del(&head, NULL);
  ASSERT(head == NULL);
}

static void ph(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_POLL) ++(*(int *) c->fn_data);
  (void) c, (void) ev_data;
}

static void test_poll(void) {
  int count = 0, i;
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, "http://127.0.0.1:42346", ph,
                 &count);  // To prevent bind errors on Windows
  for (i = 0; i < 10; i++) mg_mgr_poll(&mgr, 0);
  ASSERT(count == 10);
  mg_mgr_free(&mgr);
}

#define NMESSAGES 99999
static uint32_t s_qcrc = 0;
static int s_out, s_in;
static void producer(void *param) {
  struct mg_queue *q = (struct mg_queue *) param;
  char tmp[64 * 1024], *buf;
  size_t len, ofs = sizeof(tmp);
  for (s_out = 0; s_out < NMESSAGES; s_out++) {
    if (ofs >= sizeof(tmp)) mg_random(tmp, sizeof(tmp)), ofs = 0;
    len = ((uint8_t *) tmp)[ofs] % 55U + 1U;
    if (ofs + len > sizeof(tmp)) len = sizeof(tmp) - ofs;
    while ((mg_queue_book(q, &buf, len)) < len) (void) 0;
    memcpy(buf, &tmp[ofs], len);
    s_qcrc = mg_crc32(s_qcrc, buf, len);
    ofs += len;
#if 0
    fprintf(stderr, "-->prod %3d  %8x  %-3lu %zu/%zu/%lu\n", s_out, s_qcrc, len, q->tail,
           q->head, buf - q->buf);
#endif
    mg_queue_add(q, len);
  }
}

static uint32_t consumer(struct mg_queue *q) {
  uint32_t crc = 0;
  for (s_in = 0; s_in < NMESSAGES; s_in++) {
    char *buf;
    size_t len;
    while ((len = mg_queue_next(q, &buf)) == 0) (void) 0;
    crc = mg_crc32(crc, buf, len);
#if 0
    fprintf(stderr, "-->cons %3u  %8x  %-3lu %zu/%zu/%lu\n", s_in, crc, len, q->tail,
           q->head, buf - q->buf);
#endif
    mg_queue_del(q, len);
  }
  return crc;
}

#if MG_ARCH == MG_ARCH_WIN32
static void start_thread(void (*f)(void *), void *p) {
  _beginthread((void(__cdecl *)(void *)) f, 0, p);
}
#elif MG_ARCH == MG_ARCH_UNIX
#include <pthread.h>
static void start_thread(void (*f)(void *), void *p) {
  union {
    void (*f1)(void *);
    void *(*f2)(void *);
  } u = {f};
  pthread_t thread_id = (pthread_t) 0;
  pthread_attr_t attr;
  (void) pthread_attr_init(&attr);
  (void) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&thread_id, &attr, u.f2, p);
  pthread_attr_destroy(&attr);
}
#else
static void start_thread(void (*f)(void *), void *p) {
  (void) f, (void) p;
}
#endif

static void test_queue(void) {
  char buf[512];
  struct mg_queue queue;
  uint32_t crc;
  memset(buf, 0x55, sizeof(buf));
  mg_queue_init(&queue, buf, sizeof(buf));
  start_thread(producer, &queue);  // Start producer in a separate thread
  crc = consumer(&queue);          // Consumer eats data in this thread
  MG_INFO(("CRC1 %8x", s_qcrc));   // Show CRCs
  MG_INFO(("CRC2 %8x", crc));
  ASSERT(s_qcrc == crc);
}

static void test_md5_str(const char *string,
                         const unsigned char *expected_hash) {
  mg_md5_ctx ctx;
  unsigned char digest[16];
  mg_md5_init(&ctx);
  mg_md5_update(&ctx, (unsigned char *) string, strlen(string));
  mg_md5_final(&ctx, digest);
  ASSERT((memcmp(digest, expected_hash, 16) == 0));
}

static void test_md5(void) {
  const unsigned char expected_hash_1[] = {0xe5, 0x45, 0x14, 0x96, 0xe1, 0x1d,
                                           0x7d, 0xa9, 0x62, 0x9f, 0xe0, 0x64,
                                           0xcb, 0x3d, 0x2b, 0x54};
  const unsigned char expected_hash_2[] = {0x99, 0x33, 0xf6, 0x4d, 0x7a, 0xb5,
                                           0x0b, 0x0f, 0xf4, 0x35, 0xdc, 0x61,
                                           0x1d, 0xef, 0x20, 0xff};
  const unsigned char expected_hash_3[] = {0xf7, 0x94, 0xc3, 0xa4, 0x56, 0x6d,
                                           0xc1, 0x10, 0x95, 0xfc, 0x56, 0x87,
                                           0xf8, 0xb1, 0x69, 0xf2};
  test_md5_str("#&*%$DHFH(0x12345)^&*(^!@$%^^&&*", expected_hash_1);
  test_md5_str("1298**&^%DHKSHFLS)(*)&^^%$#!!!!", expected_hash_2);
  test_md5_str(")_)+_)!&^*%$#>>>{}}}{{{][[[[]]]", expected_hash_3);
}

static void test_sha1_str(const char *string,
                          const unsigned char *expected_hash) {
  mg_sha1_ctx ctx;
  unsigned char digest[20];
  mg_sha1_init(&ctx);
  mg_sha1_update(&ctx, (unsigned char *) string, strlen(string));
  mg_sha1_final(digest, &ctx);
  ASSERT((memcmp(digest, expected_hash, 20) == 0));
}

static void test_sha1(void) {
  const unsigned char expected_hash_1[] = {
      0x02, 0xaf, 0x27, 0x00, 0xf7, 0xba, 0xb5, 0xf5, 0xf3, 0x69,
      0xd8, 0x80, 0x01, 0x0d, 0x6a, 0x28, 0x31, 0x63, 0x1f, 0x92};
  const unsigned char expected_hash_2[] = {
      0xaa, 0xe4, 0x39, 0xe8, 0xb4, 0x72, 0x47, 0xe5, 0x1a, 0x6d,
      0x82, 0x25, 0x5e, 0x9f, 0x32, 0xd9, 0x93, 0x0a, 0x5f, 0xfb};
  const unsigned char expected_hash_3[] = {
      0xa0, 0xdd, 0xd2, 0xa1, 0x52, 0xdf, 0xa9, 0xb8, 0x7e, 0x73,
      0x32, 0x6a, 0x31, 0x28, 0xe9, 0x6d, 0x3a, 0x90, 0x82, 0x58};
  test_sha1_str("#&*%$DHFH(0x12345)^&*(^!@$%^^&&*", expected_hash_1);
  test_sha1_str("1298**&^%DHKSHFLS)(*)&^^%$#!!!!", expected_hash_2);
  test_sha1_str(")_)+_)!&^*%$#>>>{}}}{{{][[[[]]]", expected_hash_3);
}

static void test_sha256_str(const char *string,
                            const unsigned char *expected_hash) {
  unsigned char digest[32];
  mg_sha256(digest, (unsigned char *) string, strlen(string));
  ASSERT((memcmp(digest, expected_hash, 32) == 0));
}

static void test_sha256(void) {
  const unsigned char expected_hash_1[] = {
      0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4,
      0xc8, 0x99, 0x6f, 0xb9, 0x24, 0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b,
      0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55};
  const unsigned char expected_hash_2[] = {
      0xbc, 0x07, 0x32, 0x21, 0x17, 0x8e, 0x81, 0xbd, 0x2f, 0x67, 0x13,
      0x3a, 0xca, 0xb4, 0x07, 0xad, 0x5b, 0x61, 0x8b, 0x33, 0xd2, 0x95,
      0x9e, 0x94, 0x45, 0x45, 0xdc, 0x24, 0x99, 0x0a, 0xff, 0x92};
  const unsigned char expected_hash_3[] = {
      0x1b, 0x65, 0x3e, 0xda, 0x9a, 0x2a, 0x24, 0x55, 0xa3, 0x56, 0x38,
      0x08, 0xf4, 0xf7, 0xc5, 0xa6, 0xc5, 0x2d, 0x2c, 0xb1, 0x71, 0xe5,
      0x90, 0x4c, 0x83, 0x9c, 0x77, 0x92, 0x51, 0xa2, 0x84, 0x4a};
  test_sha256_str("", expected_hash_1);
  test_sha256_str(
      "#&*%$DHFH(0x12345)^&*(^!@$%^^&&*1298**&^%DHKSHFLS)(*)&^^%$#!!!!",
      expected_hash_2);
  test_sha256_str(")_)+_)!&^*%$#>>>{}}}{{{][[[[]]]", expected_hash_3);
}

static void test_sha384_str(const char *string,
                            const unsigned char *expected_hash) {
  unsigned char digest[48];
  mg_sha384(digest, (unsigned char *) string, strlen(string));
  ASSERT((memcmp(digest, expected_hash, 48) == 0));
}

static void test_sha384(void) {
  const unsigned char expected_hash_1[] = {
      0x38, 0xb0, 0x60, 0xa7, 0x51, 0xac, 0x96, 0x38, 0x4c, 0xd9, 0x32, 0x7e,
      0xb1, 0xb1, 0xe3, 0x6a, 0x21, 0xfd, 0xb7, 0x11, 0x14, 0xbe, 0x07, 0x43,
      0x4c, 0x0c, 0xc7, 0xbf, 0x63, 0xf6, 0xe1, 0xda, 0x27, 0x4e, 0xde, 0xbf,
      0xe7, 0x6f, 0x65, 0xfb, 0xd5, 0x1a, 0xd2, 0xf1, 0x48, 0x98, 0xb9, 0x5b};
  const unsigned char expected_hash_2[] = {
      0x77, 0xe7, 0x0a, 0x31, 0xe5, 0xcd, 0x68, 0xa4, 0xc5, 0xb3, 0x70, 0x55,
      0x38, 0xd0, 0x90, 0xb0, 0xcd, 0xb6, 0xf4, 0x1c, 0x2e, 0xe6, 0xf4, 0xdd,
      0xf6, 0xb4, 0xfc, 0x97, 0x01, 0x79, 0x3c, 0x89, 0x82, 0x3b, 0x13, 0xa2,
      0x48, 0xa7, 0xfe, 0xd2, 0xd0, 0xc4, 0xbf, 0xed, 0x85, 0xb6, 0x20, 0xc7};
  const unsigned char expected_hash_3[] = {
      0x45, 0xa1, 0xc6, 0x4d, 0x99, 0x29, 0x42, 0x87, 0x49, 0x46, 0x73, 0x3c,
      0x3b, 0xc8, 0xbc, 0x9c, 0x43, 0x10, 0x75, 0x23, 0x89, 0x22, 0x04, 0x41,
      0xcd, 0xa3, 0x34, 0xeb, 0x97, 0x9f, 0x2a, 0xbf, 0x17, 0x94, 0x38, 0x72,
      0x6b, 0xd8, 0x8e, 0xcc, 0xb5, 0x50, 0xc6, 0x5b, 0x35, 0x1f, 0x91, 0x90};
  test_sha384_str("", expected_hash_1);
  test_sha384_str(
      "#&*%$DHFH(0x12345)^&*(^!@$%^^&&*1298**&^%DHKSHFLS)(*)&^^%$#!!!!",
      expected_hash_2);
  test_sha384_str(")_)+_)!&^*%$#>>>{}}}{{{][[[[]]]", expected_hash_3);
}

static void test_split(void) {
  struct mg_str a, b, s;

  s = mg_str("");
  ASSERT(mg_span(s, &a, &s, '.') == false);
  ASSERT(mg_span(s, &a, NULL, '.') == false);
  ASSERT(mg_span(s, NULL, &b, '.') == false);
  ASSERT(mg_span(s, NULL, NULL, '.') == false);
  ASSERT(mg_span(s, &a, &b, '.') == false);

  s = mg_str("aa.bb.cc"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, &b, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("aa")) == 0);
  ASSERT(mg_strcmp(b, mg_str("bb.cc")) == 0);
  ASSERT(mg_strcmp(s, mg_str("aa.bb.cc")) == 0);

  s = mg_str("aa.bb.cc"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, NULL, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("aa")) == 0);
  ASSERT(mg_strcmp(s, mg_str("aa.bb.cc")) == 0);

  s = mg_str("aa.bb.cc"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, NULL, &b, '.') == true);
  ASSERT(mg_strcmp(b, mg_str("bb.cc")) == 0);
  ASSERT(mg_strcmp(s, mg_str("aa.bb.cc")) == 0);

  s = mg_str("aa.bb.cc"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, NULL, NULL, '.') == true);
  ASSERT(mg_strcmp(s, mg_str("aa.bb.cc")) == 0);

  s = mg_str("aa.bb.cc"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, &s, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("aa")) == 0);
  ASSERT(mg_strcmp(s, mg_str("bb.cc")) == 0);

  s = mg_str(".aa"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, &b, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("")) == 0);
  ASSERT(mg_strcmp(b, mg_str("aa")) == 0);

  s = mg_str("aa."), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, &b, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("aa")) == 0);
  ASSERT(mg_strcmp(b, mg_str("")) == 0);

  s = mg_str("aa"), a = mg_str_n(NULL, 0), b = mg_str_n(NULL, 0);
  ASSERT(mg_span(s, &a, &b, '.') == true);
  ASSERT(mg_strcmp(a, mg_str("aa")) == 0);
  ASSERT(mg_strcmp(b, mg_str("")) == 0);
}

static void test_x25519(void) {
#if MG_TLS == MG_TLS_BUILTIN
  uint8_t key[X25519_BYTES];
  uint8_t buf[X25519_BYTES];
  char tmp[100];
  size_t i;
  for (i = 0; i < sizeof(key); i++) key[i] = (uint8_t) i;
  for (i = 0; i < sizeof(buf); i++) buf[i] = 0;
  mg_tls_x25519(buf, key, X25519_BASE_POINT, 1);
  mg_snprintf(tmp, sizeof(tmp), "%M", mg_print_hex, sizeof(buf), buf);
  MG_INFO(("%s", tmp));
  ASSERT(mg_strcmp(mg_str("8f40c5adb6"), mg_str_n(tmp, 10)) == 0);
#endif
}

static void test_rsa(void) {
#if MG_TLS == MG_TLS_BUILTIN
  const unsigned char mod[] = {
      0x00, 0xba, 0xee, 0x3b, 0x0b, 0x89, 0x58, 0xa6, 0x19, 0x0d, 0x4c, 0x89,
      0x1a, 0x85, 0x9a, 0xf4, 0x55, 0xc2, 0xdd, 0x0d, 0xd4, 0x4a, 0xf5, 0xed,
      0xda, 0x28, 0x55, 0x2f, 0x64, 0x46, 0x21, 0x9f, 0x46, 0x5c, 0xfa, 0x37,
      0x88, 0x11, 0xdf, 0xcb, 0x51, 0x73, 0x42, 0x3d, 0x5e, 0x50, 0xde, 0x11,
      0x30, 0x61, 0x04, 0x59, 0xd0, 0xf4, 0x57, 0xed, 0x13, 0x90, 0x32, 0xc5,
      0x3f, 0xe6, 0x66, 0xfc, 0x2a, 0x12, 0xa3, 0x1f, 0xd1, 0x77, 0x21, 0x65,
      0xdf, 0x9a, 0xcf, 0x04, 0x05, 0xc3, 0x1c, 0xf8, 0x79, 0xb5, 0xf5, 0x97,
      0x68, 0x98, 0x2e, 0x96, 0x85, 0x3f, 0xee, 0x71, 0x91, 0xc1, 0x54, 0x71,
      0x9a, 0x80, 0x1f, 0xbe, 0x21, 0xd9, 0xc1, 0x80, 0x9b, 0xd0, 0x5d, 0xb3,
      0x76, 0x3e, 0xcc, 0x14, 0x3d, 0xec, 0xb7, 0x18, 0x74, 0xfb, 0xc4, 0x0e,
      0x56, 0x8d, 0x3d, 0x78, 0xe6, 0xca, 0xcd, 0x9d, 0xc6, 0x20, 0x5a, 0xeb,
      0x9b, 0xc8, 0x19, 0x5e, 0xeb, 0x80, 0xd2, 0xb2, 0xfe, 0x88, 0x15, 0x5c,
      0x7c, 0x6b, 0x26, 0xe0, 0x43, 0xda, 0xa4, 0x07, 0x85, 0x73, 0xc4, 0x80,
      0x28, 0xcb, 0xda, 0x18, 0x56, 0x37, 0x91, 0xd6, 0x41, 0xa1, 0x0b, 0xa2,
      0x77, 0xd0, 0x62, 0x31, 0xc7, 0xc2, 0x67, 0x6d, 0x75, 0x08, 0x80, 0xe7,
      0xb6, 0xbe, 0xc2, 0x25, 0xc9, 0xe0, 0x2c, 0x02, 0xbf, 0x39, 0x61, 0x7e,
      0x32, 0xa4, 0xc9, 0xe7, 0x91, 0xe3, 0xa0, 0xcd, 0x94, 0x24, 0xbf, 0x8c,
      0xeb, 0x47, 0x76, 0x53, 0x85, 0xb3, 0xb7, 0x31, 0x80, 0x3c, 0x77, 0x10,
      0x69, 0xc3, 0x04, 0xd1, 0x60, 0x4c, 0x74, 0xda, 0x15, 0x18, 0x0b, 0x20,
      0x6f, 0xb3, 0x03, 0x58, 0x4a, 0xfc, 0xd1, 0xd2, 0xcf, 0x37, 0x15, 0x0a,
      0x63, 0xc8, 0xe9, 0xd5, 0x7d, 0xd5, 0xf2, 0x90, 0x78, 0x53, 0x49, 0xa9,
      0xc5, 0x25, 0x65, 0x5c, 0x01};
  const unsigned char exp[] = {1, 0, 1};  // 65537
  const unsigned char sig[] = {
      0x1e, 0xb1, 0x6a, 0xcb, 0x39, 0x63, 0x12, 0xed, 0x85, 0x62, 0x4b, 0x85,
      0x47, 0x25, 0x67, 0xbd, 0xbd, 0x0e, 0xaa, 0x73, 0x34, 0x5f, 0x07, 0x2b,
      0xbb, 0x4f, 0xf5, 0x21, 0x88, 0xb1, 0x04, 0x2c, 0xbb, 0x52, 0x72, 0x64,
      0x89, 0x45, 0x50, 0x41, 0x73, 0xca, 0xda, 0x97, 0xae, 0x81, 0x89, 0x4f,
      0x83, 0x8d, 0x48, 0x65, 0x63, 0xe7, 0x82, 0x03, 0xd2, 0x40, 0x07, 0x1c,
      0x86, 0x58, 0xd5, 0xac, 0x89, 0xb1, 0xca, 0x5c, 0xde, 0x21, 0x06, 0x88,
      0x88, 0x0c, 0xe1, 0x20, 0xc0, 0xdf, 0xf1, 0x92, 0x9b, 0xb8, 0xa5, 0xeb,
      0x6d, 0x89, 0xcc, 0x5c, 0x5c, 0x24, 0x3e, 0x9b, 0x3c, 0x35, 0x32, 0xa5,
      0x04, 0x9e, 0x8c, 0x49, 0x01, 0xee, 0xbf, 0x1f, 0x2c, 0xb0, 0x52, 0xa8,
      0xab, 0x79, 0x11, 0xcf, 0xb5, 0x5a, 0x16, 0xa1, 0xee, 0x21, 0x6a, 0x5a,
      0x2b, 0x14, 0xae, 0x32, 0x3c, 0xa2, 0x6c, 0xa2, 0x40, 0x0c, 0xcb, 0x9e,
      0x8f, 0x69, 0xab, 0xd7, 0xf3, 0xd8, 0xd1, 0xfb, 0x2d, 0xfa, 0xa9, 0x13,
      0x09, 0xbf, 0xa7, 0xca, 0xc8, 0x90, 0x74, 0x23, 0x7b, 0x3e, 0xdd, 0x81,
      0x32, 0xa7, 0x88, 0x42, 0x56, 0x8a, 0xcb, 0xe8, 0x8f, 0xef, 0x06, 0x9f,
      0x39, 0x7e, 0x8e, 0x24, 0x07, 0xb3, 0xae, 0x7e, 0x13, 0x6b, 0xf2, 0xf8,
      0x35, 0xe4, 0x16, 0x3e, 0xae, 0xf2, 0x55, 0x79, 0x10, 0x39, 0xfa, 0x70,
      0x3a, 0x1b, 0x02, 0xb3, 0x2b, 0x1d, 0x44, 0xac, 0x30, 0x81, 0xd3, 0x11,
      0xdd, 0x34, 0x1e, 0xcd, 0x26, 0xf5, 0x89, 0xc6, 0x55, 0x23, 0x17, 0x09,
      0xd2, 0xc1, 0xdc, 0x49, 0xf9, 0x99, 0x36, 0x2b, 0xf5, 0xae, 0x42, 0x5c,
      0xb7, 0x80, 0xda, 0x32, 0x69, 0x28, 0xa3, 0xee, 0xb9, 0xd4, 0x90, 0xa6,
      0xab, 0x34, 0x17, 0x5e, 0xa0, 0xd6, 0xc1, 0x54, 0xc6, 0x9c, 0x58, 0x3a,
      0xaf, 0xbf, 0xdc, 0x64};
  unsigned char v[256];  // 2048 bits
  mg_rsa_mod_pow(mod, sizeof(mod), exp, sizeof(exp), sig, sizeof(sig), v,
                 sizeof(v));
  ASSERT(v[sizeof(v) - 1] == 0xbc);
#endif
}

static void test_crypto(void) {
  test_md5();
  test_sha1();
  test_sha256();
  test_sha384();
  test_x25519();
  test_rsa();
}

int main(void) {
  const char *debug_level = getenv("V");
  if (debug_level == NULL) debug_level = "3";
  mg_log_set(atoi(debug_level));

  test_crypto();
  test_split();
  test_json();
  test_queue();
  test_rpc();
  test_str();
  test_match();
  test_get_header_var();
  test_http_parse();
  test_rewrites();
  test_check_ip_acl();
  test_udp();
  test_packed();
  test_crc32();
  test_multipart();
  test_invalid_listen_addr();
  test_http_chunked();
  test_http_upload();
  test_http_stream_buffer();
  test_util();
  test_dns();
  test_timer();
  test_url();
  test_iobuf();
  test_base64();
  test_http_get_var();
  test_tls();
  test_ws();
  test_ws_fragmentation();
  test_http_server();
  test_http_404();
  test_http_no_content_length();
  test_http_pipeline();
  test_http_range();
#ifndef LOCALHOST_ONLY
  test_sntp();
  test_mqtt();  // sorry, MQTT_LOCALHOST is also skipped 
  test_http_client();
#else
  (void) test_sntp, (void) test_mqtt, (void) test_http_client;
#endif
  test_poll();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);

  return EXIT_SUCCESS;
}
