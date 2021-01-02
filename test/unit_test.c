#include "mongoose.h"

static int s_num_tests = 0;

#define ASSERT(expr)                                            \
  do {                                                          \
    s_num_tests++;                                              \
    if (!(expr)) {                                              \
      printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
      exit(EXIT_FAILURE);                                       \
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
}

static void test_commalist(void) {
  struct mg_str k, v, s1 = mg_str(""), s2 = mg_str("a"), s3 = mg_str("a,b");
  struct mg_str s4 = mg_str("a=123"), s5 = mg_str("a,b=123");
  ASSERT(mg_next_comma_entry(&s1, &k, &v) == false);

  ASSERT(mg_next_comma_entry(&s2, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_next_comma_entry(&s2, &k, &v) == false);

  ASSERT(mg_next_comma_entry(&s3, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_next_comma_entry(&s3, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "b") == 0);
  ASSERT(mg_next_comma_entry(&s3, &k, &v) == false);

  ASSERT(mg_next_comma_entry(&s4, &k, &v) == true);
  ASSERT(mg_vcmp(&k, "a") == 0 && mg_vcmp(&v, "123") == 0);
  ASSERT(mg_next_comma_entry(&s4, &k, &v) == false);
  ASSERT(mg_next_comma_entry(&s4, &k, &v) == false);

  ASSERT(mg_next_comma_entry(&s5, &k, &v) == true);
  ASSERT(v.len == 0 && mg_vcmp(&k, "a") == 0);
  ASSERT(mg_next_comma_entry(&s5, &k, &v) == true);
  ASSERT(mg_vcmp(&k, "b") == 0 && mg_vcmp(&v, "123") == 0);
  ASSERT(mg_next_comma_entry(&s4, &k, &v) == false);
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
  // LOG(LL_INFO, ("->%.*s<->%s<- %d %d %d", (int) s1.len, s1.ptr, s2,
  //(int) s1.len, strncmp(s1.ptr, s2, s1.len), mg_vcmp(&s1, s2)));
  return mg_vcmp(&s1, s2) == 0;
}

static void test_url(void) {
  // Host
  ASSERT(vcmp(mg_url_host("foo"), "foo"));
  ASSERT(vcmp(mg_url_host("//foo"), "foo"));
  ASSERT(vcmp(mg_url_host("foo:1234"), "foo"));
  ASSERT(vcmp(mg_url_host("//foo:1234"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/"), "foo"));
  ASSERT(vcmp(mg_url_host("p://foo/x"), "foo"));
  ASSERT(vcmp(mg_url_host("p://bar:1234"), "bar"));
  ASSERT(vcmp(mg_url_host("p://bar:1234/"), "bar"));
  ASSERT(vcmp(mg_url_host("p://bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u@bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u:p@bar:1234/a"), "bar"));
  ASSERT(vcmp(mg_url_host("p://u:p@[::1]:1234/a"), "::1"));
  ASSERT(vcmp(mg_url_host("p://u:p@[1:2::3]:1234/a"), "1:2::3"));

  // Port
  ASSERT(mg_url_port("foo:1234") == 1234);
  ASSERT(mg_url_port("x://foo:1234") == 1234);
  ASSERT(mg_url_port("x://foo:1234/") == 1234);
  ASSERT(mg_url_port("x://foo:1234/xx") == 1234);
  ASSERT(mg_url_port("x://foo:1234") == 1234);
  ASSERT(mg_url_port("p://bar:1234/a") == 1234);
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
  mg_iobuf_append(&io, "hi", 2, 10);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 2);
  ASSERT(memcmp(io.buf, "hi", 2) == 0);
  mg_iobuf_append(&io, "!", 1, 10);
  ASSERT(io.buf != NULL && io.size == 10 && io.len == 3);
  ASSERT(memcmp(io.buf, "hi!", 3) == 0);
  free(io.buf);
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_SNTP_TIME) {
    *(struct timeval *) fnd = *(struct timeval *) evd;
  }
  (void) c;
}

static void test_sntp(void) {
  struct timeval tv = {0, 0};
  struct mg_mgr mgr;
  struct mg_connection *c = NULL;
  int i;

  mg_mgr_init(&mgr);
  c = mg_sntp_connect(&mgr, NULL, sntp_cb, &tv);
  ASSERT(c != NULL);
  ASSERT(c->is_udp == 1);
  mg_sntp_send(c, (unsigned long) time(NULL));
  for (i = 0; i < 70 && tv.tv_sec == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(tv.tv_sec > 0);
  mg_mgr_free(&mgr);

  {
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
    struct timeval tv = {0, 0};
    struct tm *tm;
    time_t t;
    ASSERT(mg_sntp_parse(sntp_good, sizeof(sntp_good), &tv) == 0);
    t = tv.tv_sec;
    tm = gmtime(&t);
    ASSERT(tm->tm_year == 116);
    ASSERT(tm->tm_mon == 10);
    ASSERT(tm->tm_mday == 22);
    ASSERT(tm->tm_hour == 16);
    ASSERT(tm->tm_min == 15);
    ASSERT(tm->tm_sec == 21);
    ASSERT(mg_sntp_parse(bad_good, sizeof(bad_good), &tv) == -1);
  }

  ASSERT(mg_sntp_parse(NULL, 0, &tv) == -1);
}

static void mqtt_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  char *buf = (char *) fnd;
  if (ev == MG_EV_MQTT_OPEN) {
    buf[0] = *(int *) evd == 0 ? 'X' : 'Y';
  } else if (ev == MG_EV_MQTT_MSG) {
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) evd;
    sprintf(buf + 1, "%.*s/%.*s", (int) mm->topic.len, mm->topic.ptr,
            (int) mm->data.len, mm->data.ptr);
  }
  (void) c;
}

static void test_mqtt(void) {
  char buf[50] = {0};
  struct mg_mgr mgr;
  struct mg_str topic = mg_str("x/f12"), data = mg_str("hi");
  struct mg_connection *c;
  struct mg_mqtt_opts opts;
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
  c = mg_mqtt_connect(&mgr, url, NULL, mqtt_cb, buf);
  for (i = 0; i < 100 && buf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(buf[0] == 'X');
  mg_mqtt_sub(c, &topic);
  mg_mqtt_pub(c, &topic, &data);
  for (i = 0; i < 100 && buf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  // LOG(LL_INFO, ("[%s]", buf));
  ASSERT(strcmp(buf, "Xx/f12/hi") == 0);

  // Set params
  memset(buf, 0, sizeof(buf));
  memset(&opts, 0, sizeof(opts));
  opts.qos = 1;
  opts.clean = true;
  opts.will_retain = true;
  opts.keepalive = 20;
  opts.client_id = mg_str("mg_client");
  opts.will_topic = mg_str("mg_will_topic");
  opts.will_message = mg_str("mg_will_messsage");
  c = mg_mqtt_connect(&mgr, url, &opts, mqtt_cb, buf);
  for (i = 0; i < 100 && buf[0] == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(buf[0] == 'X');
  mg_mqtt_sub(c, &topic);
  mg_mqtt_pub(c, &topic, &data);
  for (i = 0; i < 100 && buf[1] == 0; i++) mg_mgr_poll(&mgr, 10);
  ASSERT(strcmp(buf, "Xx/f12/hi") == 0);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void eh1(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  struct mg_tls_opts *opts = (struct mg_tls_opts *) fn_data;
  if (ev == MG_EV_ACCEPT && opts != NULL) mg_tls_init(c, opts);
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    LOG(LL_INFO,
        ("[%.*s %.*s] message len %d", (int) hm->method.len, hm->method.ptr,
         (int) hm->uri.len, hm->uri.ptr, (int) hm->message.len));
    if (mg_http_match_uri(hm, "/foo/*")) {
      mg_http_reply(c, 200, "", "uri: %.*s", hm->uri.len - 5, hm->uri.ptr + 5);
    } else if (mg_http_match_uri(hm, "/ws")) {
      mg_ws_upgrade(c, hm);
    } else if (mg_http_match_uri(hm, "/body")) {
      mg_http_reply(c, 200, "", "%.*s", (int) hm->body.len, hm->body.ptr);
    } else if (mg_http_match_uri(hm, "/bar")) {
      mg_http_reply(c, 404, "", "not found");
    } else if (mg_http_match_uri(hm, "/badroot")) {
      struct mg_http_serve_opts opts = {"/BAAADDD!", NULL};
      mg_http_serve_dir(c, hm, &opts);
    } else if (mg_http_match_uri(hm, "/creds")) {
      char user[100], pass[100];
      mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
      mg_http_reply(c, 200, "", "[%s]:[%s]", user, pass);
    } else if (mg_http_match_uri(hm, "/upload")) {
      mg_http_upload(c, hm, ".");
    } else if (mg_http_match_uri(hm, "/test/")) {
      struct mg_http_serve_opts opts = {".", NULL};
      mg_http_serve_dir(c, hm, &opts);
    } else {
      struct mg_http_serve_opts opts = {"./test/data", "#.shtml"};
      mg_http_serve_dir(c, hm, &opts);
    }
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_BINARY);
  }
}

static void wcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_WS_OPEN) {
    mg_ws_send(c, "boo", 3, WEBSOCKET_OP_BINARY);
    mg_ws_send(c, "", 0, WEBSOCKET_OP_PING);
  } else if (ev == MG_EV_WS_MSG) {
    struct mg_ws_message *wm = (struct mg_ws_message *) ev_data;
    ASSERT(mg_strstr(wm->data, mg_str("boo")));
    mg_ws_send(c, "", 0, WEBSOCKET_OP_CLOSE);  // Ask server to close
    *(int *) fn_data = 1;
  } else if (ev == MG_EV_CLOSE) {
    *(int *) fn_data = 2;
  }
}

static void test_ws(void) {
  struct mg_mgr mgr;
  int i, done = 0;

  mg_mgr_init(&mgr);
  ASSERT(mg_http_listen(&mgr, "ws://LOCALHOST:12345", eh1, NULL) != NULL);
  mg_ws_connect(&mgr, "ws://localhost:12345/ws", wcb, &done, "%s", "");
  for (i = 0; i < 20; i++) mg_mgr_poll(&mgr, 1);
  ASSERT(done == 2);

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

struct fetch_data {
  char *buf;
  int code, closed;
};

static void fcb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct fetch_data *fd = (struct fetch_data *) fn_data;
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
    memset(&opts, 0, sizeof(opts));
    opts.ca = "./test/data/ca.pem";
    if (strstr(url, "127.0.0.1") != NULL) {
      // Local connection, use self-signed certificates
      opts.ca = "./test/data/ss_ca.pem";
      opts.cert = "./test/data/ss_client.pem";
    }
    mg_tls_init(c, &opts);
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
  mg_http_parse(buf, strlen(buf), &hm);
  return strncmp(hm.body.ptr, str, hm.body.len);
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
  struct stat st;

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh1, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(cmpbody(buf, "hello\n") == 0);

  {
    extern char *mg_http_etag(char *, size_t, struct stat *);
    char etag[100];
    ASSERT(stat("./test/data/a.txt", &st) == 0);
    ASSERT(mg_http_etag(etag, sizeof(etag), &st) == etag);
    ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\nIf-None-Match: %s\n\n",
                 etag) == 304);
  }

  ASSERT(fetch(&mgr, buf, url, "GET /foo/1 HTTP/1.0\r\n\n") == 200);
  // LOG(LL_INFO, ("%d %.*s", (int) hm.len, (int) hm.len, hm.buf));
  ASSERT(cmpbody(buf, "uri: 1") == 0);

  ASSERT(fetch(&mgr, buf, url, "%s",
               "POST /body HTTP/1.1\r\n"
               "Content-Length: 4\r\n\r\nkuku") == 200);
  ASSERT(cmpbody(buf, "kuku") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /ssi HTTP/1.1\r\n\r\n") == 200);
  ASSERT(cmpbody(buf,
                 "this is index\n"
                 "this is nested\n\n"
                 "this is f1\n\n\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n"
                 "recurse\n\n") == 0);

  ASSERT(fetch(&mgr, buf, url, "GET /badroot HTTP/1.0\r\n\n") == 400);
#if MG_ARCH == MG_ARCH_WIN32
  ASSERT(cmpbody(buf, "Bad web root [Z:\\BAAADDD!]\n") == 0);
#else
  // LOG(LL_INFO, ("--> [%s]", buf));
  ASSERT(cmpbody(buf, "Bad web root [/BAAADDD!]\n") == 0);
#endif

  {
    char *data = mg_file_read("./test/data/ca.pem");
    ASSERT(fetch(&mgr, buf, url, "GET /ca.pem HTTP/1.0\r\n\n") == 200);
    ASSERT(cmpbody(data, buf) == 0);
    free(data);
  }

  {
    // Test mime type
    struct mg_http_message hm;
    ASSERT(fetch(&mgr, buf, url, "GET /empty.js HTTP/1.0\r\n\n") == 200);
    mg_http_parse(buf, strlen(buf), &hm);
    ASSERT(mg_http_get_header(&hm, "Content-Type") != NULL);
    ASSERT(mg_strcmp(*mg_http_get_header(&hm, "Content-Type"),
                     mg_str("text/javascript")) == 0);
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
    ASSERT((p = mg_file_read("uploaded.txt")) == NULL);

    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload HTTP/1.0\n"
                 "Content-Length: 1\n\nx") == 400);

    ASSERT(fetch(&mgr, buf, url,
                 "POST /upload?name=uploaded.txt HTTP/1.0\r\n"
                 "Content-Length: 5\r\n"
                 "\r\nhello") == 200);
    ASSERT((p = mg_file_read("uploaded.txt")) != NULL);
    ASSERT(strcmp(p, "hello") == 0);
    free(p);
    remove("uploaded.txt");
  }

  // HEAD request
  ASSERT(fetch(&mgr, buf, url, "GET /a.txt HTTP/1.0\n\n") == 200);
  ASSERT(fetch(&mgr, buf, url, "HEAD /a.txt HTTP/1.0\n\n") == 200);

#if MG_ENABLE_IPV6
  {
    const char *url6 = "http://[::1]:12346";
    ASSERT(mg_http_listen(&mgr, url6, eh1, NULL) != NULL);
    ASSERT(fetch(&mgr, buf, url6, "GET /a.txt HTTP/1.0\n\n") == 200);
    ASSERT(cmpbody(buf, "hello\n") == 0);
  }
#endif

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
  // LOG(LL_INFO, ("%s", buf));
  ASSERT(cmpbody(buf, "hello\n") == 0);
  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
#endif
}

static void f3(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  int *ok = (int *) fn_data;
  // LOG(LL_INFO, ("%d", ev));
  if (ev == MG_EV_CONNECT) {
    // c->is_hexdumping = 1;
    mg_printf(c, "GET / HTTP/1.0\r\nHost: %s\r\n\r\n",
              c->peer.is_ip6 ? "ipv6.google.com" : "cesanta.com");
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    // LOG(LL_INFO, ("-->[%.*s]", (int) hm->message.len, hm->message.ptr));
    // ASSERT(mg_vcmp(&hm->method, "HTTP/1.1") == 0);
    // ASSERT(mg_vcmp(&hm->uri, "301") == 0);
    *ok = atoi(hm->uri.ptr);
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
    struct mg_tls_opts opts = {.ca = "./test/data/ca.pem"};
    c = mg_http_connect(&mgr, "https://cesanta.com", f3, &ok);
    ASSERT(c != NULL);
    mg_tls_init(c, &opts);
    for (i = 0; i < 500 && ok <= 0; i++) mg_mgr_poll(&mgr, 10);
    ASSERT(ok == 200);
  }
#endif

#if MG_ENABLE_IPV6
  ok = 0;
  // ipv6.google.com does not have IPv4 address, only IPv6, therefore
  // it is guaranteed to hit IPv6 resolution path.
  c = mg_http_connect(&mgr, "http://ipv6.google.com", f3, &ok);
  ASSERT(c != NULL);
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
              fn_data);
    c->is_draining = 1;
  }
}

static void test_http_no_content_length(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12348";
  char buf[FETCH_BUF_SIZE];
  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, f4, (void *) "baz");
  ASSERT(fetch(&mgr, buf, url, "GET /foo/bar HTTP/1.0\r\n\n") == 200);
  ASSERT(cmpbody(buf, "/foo/bar/baz") == 0);
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
  // LOG(LL_INFO, ("-----> [%d]", ok));
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
    static const char *s = "a b c\na:1\nb:2\nc:3\nd:4\ne:5\nf:6\n\n";
    ASSERT(mg_http_parse(s, strlen(s), &req) == (int) strlen(s));
    ASSERT((v = mg_http_get_header(&req, "e")) != NULL);
    ASSERT(mg_vcmp(v, "5") == 0);
    ASSERT((v = mg_http_get_header(&req, "f")) == NULL);
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
    const char *req = "GET /foo?bar=baz HTTP/1.0\n\n ";
    ASSERT(mg_http_parse(req, strlen(req), &hm) == (int) strlen(req) - 1);
    ASSERT(mg_strcmp(hm.uri, mg_str("/foo")) == 0);
    ASSERT(mg_strcmp(hm.query, mg_str("bar=baz")) == 0);
  }
}

static void test_http_range(void) {
  struct mg_mgr mgr;
  const char *url = "http://127.0.0.1:12349";
  struct mg_http_message hm;
  char buf[FETCH_BUF_SIZE];

  mg_mgr_init(&mgr);
  mg_http_listen(&mgr, url, eh1, NULL);

  ASSERT(fetch(&mgr, buf, url, "GET /range.txt HTTP/1.0\n\n") == 200);
  mg_http_parse(buf, strlen(buf), &hm);
  ASSERT(hm.body.len == 312);
  // ASSERT(strlen(buf) == 312);

#if 0
  ASSERT(fetch(&mgr, buf, url, "%s",
               "GET /data/range.txt HTTP/1.0\nRange: bytes=5-10\n\n") == 206);
  ASSERT(strcmp(buf, "\r\n of co") == 0);
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 206 Partial Content");
  ASSERT(strstr(buf, "Content-Length: 6\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes 5-10/312\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 8, "\r\n of co");

  /* Fetch till EOF */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=300-\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 206 Partial Content");
  ASSERT(strstr(buf, "Content-Length: 12\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes 300-311/312\r\n") != 0);
  ASSERT_STREQ(buf + strlen(buf) - 14, "\r\nis disease.\n");

  /* Fetch past EOF, must trigger 416 response */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=1000-\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 416");
  ASSERT(strstr(buf, "Content-Length: 0\r\n") != 0);
  ASSERT(strstr(buf, "Content-Range: bytes */312\r\n") != 0);

  /* Request range past EOF, must trigger 416 response */
  fetch_http(buf, "%s", "GET /data/range.txt HTTP/1.0\nRange: bytes=0-312\n\n");
  ASSERT_STREQ_NZ(buf, "HTTP/1.1 416");
#endif

  mg_mgr_free(&mgr);
  ASSERT(mgr.conns == NULL);
}

static void f1(void *arg) {
  (*(int *) arg)++;
}

static void test_timer(void) {
  int v1 = 0, v2 = 0, v3 = 0;
  struct mg_timer t1, t2, t3;

  LOG(LL_INFO, ("g_timers: %p", g_timers));
  ASSERT(g_timers == NULL);

  mg_timer_init(&t1, 5, MG_TIMER_REPEAT, f1, &v1);
  mg_timer_init(&t2, 15, 0, f1, &v2);
  mg_timer_init(&t3, 10, MG_TIMER_REPEAT | MG_TIMER_RUN_NOW, f1, &v3);

  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == &t2);

  mg_timer_poll(0);
  mg_timer_poll(1);
  ASSERT(v1 == 0);
  ASSERT(v2 == 0);
  ASSERT(v3 == 1);

  mg_timer_poll(5);
  ASSERT(v1 == 1);
  ASSERT(v2 == 0);
  ASSERT(v3 == 1);

  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == &t2);

  // Simulate long delay - timers must invalidate expiration times
  mg_timer_poll(100);
  ASSERT(v1 == 2);
  ASSERT(v2 == 1);
  ASSERT(v3 == 2);

  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == &t1);  // t2 should be removed
  ASSERT(g_timers->next->next == NULL);

  mg_timer_poll(107);
  ASSERT(v1 == 3);
  ASSERT(v2 == 1);
  ASSERT(v3 == 2);

  mg_timer_poll(114);
  ASSERT(v1 == 4);
  ASSERT(v2 == 1);
  ASSERT(v3 == 3);

  mg_timer_poll(115);
  ASSERT(v1 == 5);
  ASSERT(v2 == 1);
  ASSERT(v3 == 3);

  mg_timer_init(&t2, 3, 0, f1, &v2);
  ASSERT(g_timers == &t2);
  ASSERT(g_timers->next == &t3);
  ASSERT(g_timers->next->next == &t1);
  ASSERT(g_timers->next->next->next == NULL);

  mg_timer_poll(120);
  ASSERT(v1 == 6);
  ASSERT(v2 == 1);
  ASSERT(v3 == 4);

  mg_timer_poll(125);
  ASSERT(v1 == 7);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  // Test millisecond counter wrap - when time goes back.
  mg_timer_poll(0);
  ASSERT(v1 == 7);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == &t1);
  ASSERT(g_timers->next->next == NULL);

  mg_timer_poll(7);
  ASSERT(v1 == 8);
  ASSERT(v2 == 2);
  ASSERT(v3 == 4);

  mg_timer_poll(11);
  ASSERT(v1 == 9);
  ASSERT(v2 == 2);
  ASSERT(v3 == 5);

  mg_timer_free(&t1);
  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == NULL);

  mg_timer_free(&t2);
  ASSERT(g_timers == &t3);
  ASSERT(g_timers->next == NULL);

  mg_timer_free(&t3);
  ASSERT(g_timers == NULL);
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

#if 0
  {
    char *data = mg_file_read("dns.bin");
    ASSERT(data != NULL);
    ASSERT(mg_dns_parse((uint8_t *) data, mg_file_size("dns.bin"), &dm) == 0);
    free(data);
  }
#endif
}

static void test_util(void) {
  char buf[100], *s = mg_hexdump("abc", 3);
  struct mg_addr a;
  ASSERT(s != NULL);
  free(s);
  memset(&a, 0, sizeof(a));
  ASSERT(mg_file_write("data.txt", "%s", "hi") == 2);
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
}

int main(void) {
  mg_log_set("3");
  test_util();
  test_sntp();
  test_dns();
  test_str();
  test_timer();
  test_http_range();
  test_url();
  test_iobuf();
  test_commalist();
  test_base64();
  test_globmatch();
  test_http_get_var();
  test_tls();
  test_ws();
  test_http_parse();
  test_http_server();
  test_http_client();
  test_http_no_content_length();
  test_http_pipeline();
  test_mqtt();
  printf("SUCCESS. Total tests: %d\n", s_num_tests);
  return EXIT_SUCCESS;
}
