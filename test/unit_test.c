#define USE_WEBSOCKET
#include "mongoose.c"

#define FATAL(str, line) do {                     \
  printf("Fail on line %d: [%s]\n", line, str);   \
  abort();                                        \
} while (0)
#define ASSERT(expr) do { if (!(expr)) FATAL(#expr, __LINE__); } while (0)

static void test_parse_http_request() {
  struct mg_request_info ri;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
  char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";

  ASSERT(parse_http_request(req1, sizeof(req1), &ri) == sizeof(req1) - 1);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
  ASSERT(ri.num_headers == 0);

  ASSERT(parse_http_request(req2, sizeof(req2), &ri) == -1);
  ASSERT(parse_http_request(req3, sizeof(req3), &ri) == -1);

  // TODO(lsm): Fix this. Header value may span multiple lines.
  ASSERT(parse_http_request(req4, sizeof(req4), &ri) == sizeof(req4) - 1);
  ASSERT(ri.num_headers == 3);
  ASSERT(strcmp(ri.http_headers[0].name, "A") == 0);
  ASSERT(strcmp(ri.http_headers[0].value, "foo bar") == 0);
  ASSERT(strcmp(ri.http_headers[1].name, "B") == 0);
  ASSERT(strcmp(ri.http_headers[1].value, "bar") == 0);
  ASSERT(strcmp(ri.http_headers[2].name, "baz\r\n\r") == 0);
  ASSERT(strcmp(ri.http_headers[2].value, "") == 0);

  // TODO(lsm): add more tests. 
}

static void test_should_keep_alive(void) {
  struct mg_connection conn;
  struct mg_context ctx;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "GET / HTTP/1.0\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
  char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";

  memset(&conn, 0, sizeof(conn));
  conn.ctx = &ctx;
  parse_http_request(req1, sizeof(req1), &conn.request_info);

  ctx.config[ENABLE_KEEP_ALIVE] = "no";
  ASSERT(should_keep_alive(&conn) == 0);

  ctx.config[ENABLE_KEEP_ALIVE] = "yes";
  ASSERT(should_keep_alive(&conn) == 1);

  conn.must_close = 1;
  ASSERT(should_keep_alive(&conn) == 0);

  conn.must_close = 0;
  parse_http_request(req2, sizeof(req2), &conn.request_info);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_request(req3, sizeof(req3), &conn.request_info);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_request(req4, sizeof(req4), &conn.request_info);
  ASSERT(should_keep_alive(&conn) == 1);

  conn.status_code = 401;
  ASSERT(should_keep_alive(&conn) == 0);

  conn.status_code = 200;
  conn.must_close = 1;
  ASSERT(should_keep_alive(&conn) == 0);
}

static void test_match_prefix(void) {
  ASSERT(match_prefix("/api", 4, "/api") == 4);
  ASSERT(match_prefix("/a/", 3, "/a/b/c") == 3);
  ASSERT(match_prefix("/a/", 3, "/ab/c") == -1);
  ASSERT(match_prefix("/*/", 3, "/ab/c") == 4);
  ASSERT(match_prefix("**", 2, "/a/b/c") == 6);
  ASSERT(match_prefix("/*", 2, "/a/b/c") == 2);
  ASSERT(match_prefix("*/*", 3, "/a/b/c") == 2);
  ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
  ASSERT(match_prefix("**.foo|**.bar", 13, "a.bar") == 5);
  ASSERT(match_prefix("a|b|cd", 6, "cdef") == 2);
  ASSERT(match_prefix("a|b|c?", 6, "cdef") == 2);
  ASSERT(match_prefix("a|?|cd", 6, "cdef") == 1);
  ASSERT(match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi") == -1);
  ASSERT(match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi") == 12);
  ASSERT(match_prefix("**/", 3, "/a/b/c") == 5);
  ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
  ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
  ASSERT(match_prefix("$", 1, "") == 0);
  ASSERT(match_prefix("$", 1, "x") == -1);
  ASSERT(match_prefix("*$", 2, "x") == 1);
  ASSERT(match_prefix("/$", 2, "/") == 1);
  ASSERT(match_prefix("**/$", 4, "/a/b/c") == -1);
  ASSERT(match_prefix("**/$", 4, "/a/b/") == 5);
  ASSERT(match_prefix("*", 1, "/hello/") == 0);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b/") == -1);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.b") == 6);
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/b.a") == 6);
}

static void test_remove_double_dots() {
  struct { char before[20], after[20]; } data[] = {
    {"////a", "/a"},
    {"/.....", "/."},
    {"/......", "/"},
    {"...", "..."},
    {"/...///", "/./"},
    {"/a...///", "/a.../"},
    {"/.x", "/.x"},
    {"/\\", "/"},
    {"/a\\", "/a\\"},
    {"/a\\\\...", "/a\\."},
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(data); i++) {
    printf("[%s] -> [%s]\n", data[i].before, data[i].after);
    remove_double_dots_and_double_slashes(data[i].before);
    ASSERT(strcmp(data[i].before, data[i].after) == 0);
  }
}

static const char *fetch_data = "hello world!\n";
static void *event_handler(enum mg_event event,
                           struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  if (event == MG_NEW_REQUEST && !strcmp(request_info->uri, "/data")) {
    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Length: %d\r\n"
              "Content-Type: text/plain\r\n\r\n"
              "%s", (int) strlen(fetch_data), fetch_data);
    return "";
  } else if (event == MG_EVENT_LOG) {
    printf("%s\n", mg_get_log_message(conn));
  }
  
  return NULL;
}

static void test_mg_fetch(void) {
  static const char *options[] = {
    "document_root", ".",
    "listening_ports", "33796",
    NULL,
  };
  char buf[2000], buf2[2000];
  int length;
  struct mg_context *ctx;
  struct mg_request_info ri;
  const char *tmp_file = "temporary_file_name_for_unit_test.txt";
  struct mgstat st;
  FILE *fp;

  ASSERT((ctx = mg_start(event_handler, NULL, options)) != NULL);

  // Failed fetch, pass invalid URL
  ASSERT(mg_fetch(ctx, "localhost", tmp_file, buf, sizeof(buf), &ri) == NULL);
  ASSERT(mg_fetch(ctx, "localhost:33796", tmp_file,
                  buf, sizeof(buf), &ri) == NULL);
  ASSERT(mg_fetch(ctx, "http://$$$.$$$", tmp_file,
                  buf, sizeof(buf), &ri) == NULL);

  // Failed fetch, pass invalid file name
  ASSERT(mg_fetch(ctx, "http://localhost:33796/data",
                  "/this/file/must/not/exist/ever",
                  buf, sizeof(buf), &ri) == NULL);

  // Successful fetch
  ASSERT((fp = mg_fetch(ctx, "http://localhost:33796/data",
                        tmp_file, buf, sizeof(buf), &ri)) != NULL);
  ASSERT(ri.num_headers == 2);
  ASSERT(!strcmp(ri.request_method, "HTTP/1.1"));
  ASSERT(!strcmp(ri.uri, "200"));
  ASSERT(!strcmp(ri.http_version, "OK"));
  ASSERT((length = ftell(fp)) == (int) strlen(fetch_data));
  fseek(fp, 0, SEEK_SET);
  ASSERT(fread(buf2, 1, length, fp) == (size_t) length);
  ASSERT(memcmp(buf2, fetch_data, length) == 0);
  fclose(fp);

  // Fetch big file, mongoose.c
  ASSERT((fp = mg_fetch(ctx, "http://localhost:33796/mongoose.c",
                        tmp_file, buf, sizeof(buf), &ri)) != NULL);
  ASSERT(mg_stat("mongoose.c", &st) == 0);
  ASSERT(st.size == ftell(fp));
  ASSERT(!strcmp(ri.request_method, "HTTP/1.1"));

  remove(tmp_file);
  mg_stop(ctx);
}

static void test_base64_encode(void) {
  const char *in[] = {"a", "ab", "abc", "abcd", NULL};
  const char *out[] = {"YQ==", "YWI=", "YWJj", "YWJjZA=="};
  char buf[100];
  int i;

  for (i = 0; in[i] != NULL; i++) {
    base64_encode((unsigned char *) in[i], strlen(in[i]), buf);
    printf("[%s] [%s]\n", out[i], buf);
    ASSERT(!strcmp(buf, out[i]));
  }
}

static void test_mg_get_var(void) {
  static const char *post[] = {"a=1&&b=2&d&=&c=3%20&e=", NULL};
  char buf[20];
  
  ASSERT(mg_get_var(post[0], strlen(post[0]), "a", buf, sizeof(buf)) == 1);
  ASSERT(buf[0] == '1' && buf[1] == '\0');
  ASSERT(mg_get_var(post[0], strlen(post[0]), "b", buf, sizeof(buf)) == 1);
  ASSERT(buf[0] == '2' && buf[1] == '\0');
  ASSERT(mg_get_var(post[0], strlen(post[0]), "c", buf, sizeof(buf)) == 2);  
  ASSERT(buf[0] == '3' && buf[1] == ' ' && buf[2] == '\0');
  ASSERT(mg_get_var(post[0], strlen(post[0]), "e", buf, sizeof(buf)) == 0);
  ASSERT(buf[0] == '\0');

  ASSERT(mg_get_var(post[0], strlen(post[0]), "d", buf, sizeof(buf)) == -1);  
  ASSERT(mg_get_var(post[0], strlen(post[0]), "c", buf, 2) == -1);  

  ASSERT(mg_get_var(post[0], strlen(post[0]), "x", NULL, 10) == -2);
  ASSERT(mg_get_var(post[0], strlen(post[0]), "x", buf, 0) == -2);
}

static void test_set_throttle(void) {
  ASSERT(set_throttle(NULL, 0x0a000001, "/") == 0);
  ASSERT(set_throttle("10.0.0.0/8=20", 0x0a000001, "/") == 20);
  ASSERT(set_throttle("10.0.0.0/8=0.5k", 0x0a000001, "/") == 512);
  ASSERT(set_throttle("10.0.0.0/8=17m", 0x0a000001, "/") == 1048576 * 17);
  ASSERT(set_throttle("10.0.0.0/8=1x", 0x0a000001, "/") == 0);
  ASSERT(set_throttle("10.0.0.0/8=5,0.0.0.0/0=10", 0x0a000001, "/") == 10);
  ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0a000001, "/index") == 5);
  ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0a000001, "/foo/x") == 7);
  ASSERT(set_throttle("10.0.0.0/8=5,/foo/**=7", 0x0b000001, "/foxo/x") == 0);
  ASSERT(set_throttle("10.0.0.0/8=5,*=1", 0x0b000001, "/foxo/x") == 1);
}

static void test_next_option(void) {
  const char *p, *list = "x/8,/y**=1;2k,z";
  struct vec a, b;
  int i;

  ASSERT(next_option(NULL, &a, &b) == NULL);
  for (i = 0, p = list; (p = next_option(p, &a, &b)) != NULL; i++) {
    ASSERT(i != 0 || (a.ptr == list && a.len == 3 && b.len == 0));
    ASSERT(i != 1 || (a.ptr == list + 4 && a.len == 4 && b.ptr == list + 9 &&
                      b.len == 4));

    ASSERT(i != 2 || (a.ptr == list + 14 && a.len == 1 && b.len == 0));
  }
}

int main(void) {
  test_base64_encode();
  test_match_prefix();
  test_remove_double_dots();
  test_should_keep_alive();
  test_parse_http_request();
  test_mg_fetch();
  test_mg_get_var();
  test_set_throttle();
  test_next_option();
  return 0;
}
