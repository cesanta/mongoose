// Copyright (c) 2004-2013 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Unit test for the mongoose web server. Tests embedded API.

#define USE_WEBSOCKET
#define USE_LUA

#ifndef _WIN32
#define __cdecl
#define USE_IPV6
#endif

// USE_* definitions must be made before #include "mongoose.c" !

#include "mongoose.c"

static int s_total_tests = 0;
static int s_failed_tests = 0;

#define FAIL(str, line) do {                     \
  printf("Fail on line %d: [%s]\n", line, str);   \
  s_failed_tests++; \
} while (0)

#define ASSERT(expr) do { \
  s_total_tests++; \
  if (!(expr)) FAIL(#expr, __LINE__); \
} while (0)

#define HTTP_PORT "56789"
#define HTTPS_PORT "56790"
#define HTTP_PORT2 "56791"
#define LISTENING_ADDR          \
  "127.0.0.1:" HTTP_PORT "r"    \
  ",127.0.0.1:" HTTPS_PORT "s"  \
  ",127.0.0.1:" HTTP_PORT2

static void test_parse_http_message() {
  struct mg_request_info ri;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "BLAH / HTTP/1.1\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nBah\r\n";
  char req4[] = "GET / HTTP/1.1\r\nA: foo bar\r\nB: bar\r\nbaz\r\n\r\n";
  char req5[] = "GET / HTTP/1.1\r\n\r\n";
  char req6[] = "G";
  char req7[] = " blah ";
  char req8[] = " HTTP/1.1 200 OK \n\n";
  char req9[] = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";

  ASSERT(parse_http_message(req9, sizeof(req9), &ri) == sizeof(req9) - 1);
  ASSERT(ri.num_headers == 1);

  ASSERT(parse_http_message(req1, sizeof(req1), &ri) == sizeof(req1) - 1);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
  ASSERT(ri.num_headers == 0);

  ASSERT(parse_http_message(req2, sizeof(req2), &ri) == -1);
  ASSERT(parse_http_message(req3, sizeof(req3), &ri) == 0);
  ASSERT(parse_http_message(req6, sizeof(req6), &ri) == 0);
  ASSERT(parse_http_message(req7, sizeof(req7), &ri) == 0);
  ASSERT(parse_http_message("", 0, &ri) == 0);
  ASSERT(parse_http_message(req8, sizeof(req8), &ri) == sizeof(req8) - 1);

  // TODO(lsm): Fix this. Header value may span multiple lines.
  ASSERT(parse_http_message(req4, sizeof(req4), &ri) == sizeof(req4) - 1);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
  ASSERT(ri.num_headers == 3);
  ASSERT(strcmp(ri.http_headers[0].name, "A") == 0);
  ASSERT(strcmp(ri.http_headers[0].value, "foo bar") == 0);
  ASSERT(strcmp(ri.http_headers[1].name, "B") == 0);
  ASSERT(strcmp(ri.http_headers[1].value, "bar") == 0);
  ASSERT(strcmp(ri.http_headers[2].name, "baz\r\n\r") == 0);
  ASSERT(strcmp(ri.http_headers[2].value, "") == 0);

  ASSERT(parse_http_message(req5, sizeof(req5), &ri) == sizeof(req5) - 1);
  ASSERT(strcmp(ri.request_method, "GET") == 0);
  ASSERT(strcmp(ri.http_version, "1.1") == 0);
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
  ASSERT(parse_http_message(req1, sizeof(req1), &conn.request_info) ==
         sizeof(req1) - 1);

  ctx.config[ENABLE_KEEP_ALIVE] = "no";
  ASSERT(should_keep_alive(&conn) == 0);

  ctx.config[ENABLE_KEEP_ALIVE] = "yes";
  ASSERT(should_keep_alive(&conn) == 1);

  conn.must_close = 1;
  ASSERT(should_keep_alive(&conn) == 0);

  conn.must_close = 0;
  parse_http_message(req2, sizeof(req2), &conn.request_info);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_message(req3, sizeof(req3), &conn.request_info);
  ASSERT(should_keep_alive(&conn) == 0);

  parse_http_message(req4, sizeof(req4), &conn.request_info);
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
  ASSERT(match_prefix("**.a$|**.b$", 11, "/a/B.A") == 6);
  ASSERT(match_prefix("**o$", 4, "HELLO") == 5);
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
    remove_double_dots_and_double_slashes(data[i].before);
    ASSERT(strcmp(data[i].before, data[i].after) == 0);
  }
}

static char *read_file(const char *path, int *size) {
  FILE *fp;
  struct stat st;
  char *data = NULL;
  if ((fp = fopen(path, "rb")) != NULL && !fstat(fileno(fp), &st)) {
    *size = (int) st.st_size;
    ASSERT((data = malloc(*size)) != NULL);
    ASSERT(fread(data, 1, *size, fp) == (size_t) *size);
    fclose(fp);
  }
  return data;
}

static const char *fetch_data = "hello world!\n";
static const char *inmemory_file_data = "hi there";
static const char *upload_filename = "upload_test.txt";
static const char *upload_filename2 = "upload_test2.txt";
static const char *upload_ok_message = "upload successful";

static const char *open_file_cb(const struct mg_connection *conn,
                                const char *path, size_t *size) {
  (void) conn;
  if (!strcmp(path, "./blah")) {
    *size = strlen(inmemory_file_data);
    return inmemory_file_data;
  }
  return NULL;
}

static void upload_cb(struct mg_connection *conn, const char *path) {
  const struct mg_request_info *ri = mg_get_request_info(conn);
  char *p1, *p2;
  int len1, len2;

  if (atoi(ri->query_string) == 1) {
    ASSERT(!strcmp(path, "./upload_test.txt"));
    ASSERT((p1 = read_file("mongoose.c", &len1)) != NULL);
    ASSERT((p2 = read_file(path, &len2)) != NULL);
    ASSERT(len1 == len2);
    ASSERT(memcmp(p1, p2, len1) == 0);
    free(p1), free(p2);
    remove(upload_filename);
  } else if (atoi(ri->query_string) == 2) {
    if (!strcmp(path, "./upload_test.txt")) {
      ASSERT((p1 = read_file("mongoose.h", &len1)) != NULL);
      ASSERT((p2 = read_file(path, &len2)) != NULL);
      ASSERT(len1 == len2);
      ASSERT(memcmp(p1, p2, len1) == 0);
      free(p1), free(p2);
      remove(upload_filename);
    } else if (!strcmp(path, "./upload_test2.txt")) {
      ASSERT((p1 = read_file("README.md", &len1)) != NULL);
      ASSERT((p2 = read_file(path, &len2)) != NULL);
      ASSERT(len1 == len2);
      ASSERT(memcmp(p1, p2, len1) == 0);
      free(p1), free(p2);
      remove(upload_filename);
    } else {
      ASSERT(0);
    }
  } else {
    ASSERT(0);
  }

  mg_printf(conn, "HTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n%s",
            (int) strlen(upload_ok_message), upload_ok_message);
}

static int begin_request_handler_cb(struct mg_connection *conn) {
  const struct mg_request_info *ri = mg_get_request_info(conn);

  if (!strcmp(ri->uri, "/data")) {
    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/plain\r\n\r\n"
              "%s", fetch_data);
    close_connection(conn);
    return 1;
  }

  if (!strcmp(ri->uri, "/upload")) {
    ASSERT(ri->query_string != NULL);
    ASSERT(mg_upload(conn, ".") == atoi(ri->query_string));
  }

  return 0;
}

static int log_message_cb(const struct mg_connection *conn, const char *msg) {
  (void) conn;
  printf("%s\n", msg);
  return 0;
}

static const struct mg_callbacks CALLBACKS = {
  &begin_request_handler_cb, NULL, &log_message_cb, NULL, NULL, NULL, NULL,
  &open_file_cb, NULL, &upload_cb, NULL
};

static const char *OPTIONS[] = {
  "document_root", ".",
  "listening_ports", LISTENING_ADDR,
  "enable_keep_alive", "yes",
  "ssl_certificate", "build/ssl_cert.pem",
  NULL,
};

static char *read_conn(struct mg_connection *conn, int *size) {
  char buf[100], *data = NULL;
  int len;
  *size = 0;
  while ((len = mg_read(conn, buf, sizeof(buf))) > 0) {
    *size += len;
    ASSERT((data = realloc(data, *size)) != NULL);
    memcpy(data + *size - len, buf, len);
  }
  return data;
}

static void test_mg_download(void) {
  char *p1, *p2, ebuf[100];
  int len1, len2, port = atoi(HTTPS_PORT);
  struct mg_connection *conn;
  struct mg_context *ctx;

  ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);

  ASSERT(mg_download(NULL, port, 0, ebuf, sizeof(ebuf), "%s", "") == NULL);
  ASSERT(mg_download("localhost", 0, 0, ebuf, sizeof(ebuf), "%s", "") == NULL);
  ASSERT(mg_download("localhost", port, 1, ebuf, sizeof(ebuf),
                     "%s", "") == NULL);

  // Fetch nonexistent file, should see 404
  ASSERT((conn = mg_download("localhost", port, 1, ebuf, sizeof(ebuf), "%s",
                             "GET /gimbec HTTP/1.0\r\n\r\n")) != NULL);
  ASSERT(strcmp(conn->request_info.uri, "404") == 0);
  mg_close_connection(conn);

  ASSERT((conn = mg_download("google.com", 443, 1, ebuf, sizeof(ebuf), "%s",
                             "GET / HTTP/1.0\r\n\r\n")) != NULL);
  mg_close_connection(conn);

  // Fetch mongoose.c, should succeed
  ASSERT((conn = mg_download("localhost", port, 1, ebuf, sizeof(ebuf), "%s",
                             "GET /mongoose.c HTTP/1.0\r\n\r\n")) != NULL);
  ASSERT(!strcmp(conn->request_info.uri, "200"));
  ASSERT((p1 = read_conn(conn, &len1)) != NULL);
  ASSERT((p2 = read_file("mongoose.c", &len2)) != NULL);
  ASSERT(len1 == len2);
  ASSERT(memcmp(p1, p2, len1) == 0);
  free(p1), free(p2);
  mg_close_connection(conn);


  // Fetch in-memory file, should succeed.
  ASSERT((conn = mg_download("localhost", port, 1, ebuf, sizeof(ebuf), "%s",
                             "GET /blah HTTP/1.1\r\n\r\n")) != NULL);
  ASSERT((p1 = read_conn(conn, &len1)) != NULL);
  ASSERT(len1 == (int) strlen(inmemory_file_data));
  ASSERT(memcmp(p1, inmemory_file_data, len1) == 0);
  free(p1);
  mg_close_connection(conn);

  // Fetch in-memory data with no Content-Length, should succeed.
  ASSERT((conn = mg_download("localhost", port, 1, ebuf, sizeof(ebuf), "%s",
                             "GET /data HTTP/1.1\r\n\r\n")) != NULL);
  ASSERT((p1 = read_conn(conn, &len1)) != NULL);
  ASSERT(len1 == (int) strlen(fetch_data));
  ASSERT(memcmp(p1, fetch_data, len1) == 0);
  free(p1);
  mg_close_connection(conn);

  // Test SSL redirect, IP address
  ASSERT((conn = mg_download("localhost", atoi(HTTP_PORT), 0,
                             ebuf, sizeof(ebuf), "%s",
                             "GET /foo HTTP/1.1\r\n\r\n")) != NULL);
  ASSERT(strcmp(conn->request_info.uri, "302") == 0);
  ASSERT(strcmp(mg_get_header(conn, "Location"),
                "https://127.0.0.1:" HTTPS_PORT "/foo") == 0);
  mg_close_connection(conn);

  // Test SSL redirect, Host:
  ASSERT((conn = mg_download("localhost", atoi(HTTP_PORT), 0,
                             ebuf, sizeof(ebuf), "%s",
                             "GET /foo HTTP/1.1\r\nHost: a.b:77\n\n")) != NULL);
  ASSERT(strcmp(conn->request_info.uri, "302") == 0);
  ASSERT(strcmp(mg_get_header(conn, "Location"),
                "https://a.b:" HTTPS_PORT "/foo") == 0);
  mg_close_connection(conn);

  mg_stop(ctx);
}

static int alloc_printf(char **buf, size_t size, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  return alloc_vprintf(buf, size, fmt, ap);
}

static void test_mg_upload(void) {
  static const char *boundary = "OOO___MY_BOUNDARY___OOO";
  struct mg_context *ctx;
  struct mg_connection *conn;
  char ebuf[100], buf[20], *file_data, *file2_data, *post_data;
  int file_len, file2_len, post_data_len;

  ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);

  // Upload one file
  ASSERT((file_data = read_file("mongoose.c", &file_len)) != NULL);
  post_data = NULL;
  post_data_len = alloc_printf(&post_data, 0,
                                       "--%s\r\n"
                                       "Content-Disposition: form-data; "
                                       "name=\"file\"; "
                                       "filename=\"%s\"\r\n\r\n"
                                       "%.*s\r\n"
                                       "--%s--\r\n",
                                       boundary, upload_filename,
                                       file_len, file_data, boundary);
  ASSERT(post_data_len > 0);
  ASSERT((conn = mg_download("localhost", atoi(HTTPS_PORT), 1,
                             ebuf, sizeof(ebuf),
                             "POST /upload?1 HTTP/1.1\r\n"
                             "Content-Length: %d\r\n"
                             "Content-Type: multipart/form-data; "
                             "boundary=%s\r\n\r\n"
                             "%.*s", post_data_len, boundary,
                             post_data_len, post_data)) != NULL);
  free(file_data), free(post_data);
  ASSERT(mg_read(conn, buf, sizeof(buf)) == (int) strlen(upload_ok_message));
  ASSERT(memcmp(buf, upload_ok_message, strlen(upload_ok_message)) == 0);
  mg_close_connection(conn);

  // Upload two files
  ASSERT((file_data = read_file("mongoose.h", &file_len)) != NULL);
  ASSERT((file2_data = read_file("README.md", &file2_len)) != NULL);
  post_data = NULL;
  post_data_len = alloc_printf(&post_data, 0,
                               // First file
                               "--%s\r\n"
                               "Content-Disposition: form-data; "
                               "name=\"file\"; "
                               "filename=\"%s\"\r\n\r\n"
                               "%.*s\r\n"

                               // Second file
                               "--%s\r\n"
                               "Content-Disposition: form-data; "
                               "name=\"file\"; "
                               "filename=\"%s\"\r\n\r\n"
                               "%.*s\r\n"

                               // Final boundary
                               "--%s--\r\n",
                               boundary, upload_filename,
                               file_len, file_data,
                               boundary, upload_filename2,
                               file2_len, file2_data,
                               boundary);
  ASSERT(post_data_len > 0);
  ASSERT((conn = mg_download("localhost", atoi(HTTPS_PORT), 1,
                             ebuf, sizeof(ebuf),
                             "POST /upload?2 HTTP/1.1\r\n"
                             "Content-Length: %d\r\n"
                             "Content-Type: multipart/form-data; "
                             "boundary=%s\r\n\r\n"
                             "%.*s", post_data_len, boundary,
                             post_data_len, post_data)) != NULL);
  free(file_data), free(file2_data), free(post_data);
  ASSERT(mg_read(conn, buf, sizeof(buf)) == (int) strlen(upload_ok_message));
  ASSERT(memcmp(buf, upload_ok_message, strlen(upload_ok_message)) == 0);
  mg_close_connection(conn);

  mg_stop(ctx);
}

static void test_base64_encode(void) {
  const char *in[] = {"a", "ab", "abc", "abcd", NULL};
  const char *out[] = {"YQ==", "YWI=", "YWJj", "YWJjZA=="};
  char buf[100];
  int i;

  for (i = 0; in[i] != NULL; i++) {
    base64_encode((unsigned char *) in[i], strlen(in[i]), buf);
    ASSERT(!strcmp(buf, out[i]));
  }
}

static void test_mg_get_var(void) {
  static const char *post[] = {
    "a=1&&b=2&d&=&c=3%20&e=",
    "q=&st=2012%2F11%2F13+17%3A05&et=&team_id=",
    NULL
  };
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
  ASSERT(mg_get_var(post[0], strlen(post[0]), "c", buf, 2) == -2);

  ASSERT(mg_get_var(post[0], strlen(post[0]), "x", NULL, 10) == -2);
  ASSERT(mg_get_var(post[0], strlen(post[0]), "x", buf, 0) == -2);
  ASSERT(mg_get_var(post[1], strlen(post[1]), "st", buf, 16) == -2);
  ASSERT(mg_get_var(post[1], strlen(post[1]), "st", buf, 17) == 16);
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

#ifdef USE_LUA
static void check_lua_expr(lua_State *L, const char *expr, const char *value) {
  const char *v, *var_name = "myVar";
  char buf[100];

  snprintf(buf, sizeof(buf), "%s = %s", var_name, expr);
  (void) luaL_dostring(L, buf);
  lua_getglobal(L, var_name);
  v = lua_tostring(L, -1);
  ASSERT((value == NULL && v == NULL) ||
         (value != NULL && v != NULL && !strcmp(value, v)));
}

static void test_lua(void) {
  static struct mg_connection conn;
  static struct mg_context ctx;

  char http_request[] = "POST /foo/bar HTTP/1.1\r\n"
      "Content-Length: 12\r\n"
      "Connection: close\r\n\r\nhello world!";
  lua_State *L = luaL_newstate();

  conn.ctx = &ctx;
  conn.buf = http_request;
  conn.buf_size = conn.data_len = strlen(http_request);
  conn.request_len = parse_http_message(conn.buf, conn.data_len,
                                        &conn.request_info);
  conn.content_len = conn.data_len - conn.request_len;

  prepare_lua_environment(&conn, L);
  ASSERT(lua_gettop(L) == 0);

  check_lua_expr(L, "'hi'", "hi");
  check_lua_expr(L, "mg.request_info.request_method", "POST");
  check_lua_expr(L, "mg.request_info.uri", "/foo/bar");
  check_lua_expr(L, "mg.request_info.num_headers", "2");
  check_lua_expr(L, "mg.request_info.remote_ip", "0");
  check_lua_expr(L, "mg.request_info.http_headers['Content-Length']", "12");
  check_lua_expr(L, "mg.request_info.http_headers['Connection']", "close");
  (void) luaL_dostring(L, "post = mg.read()");
  check_lua_expr(L, "# post", "12");
  check_lua_expr(L, "post", "hello world!");
  lua_close(L);
}
#endif

static void test_mg_stat(void) {
  static struct mg_context ctx;
  struct file file = STRUCT_FILE_INITIALIZER;
  ASSERT(!mg_stat(fc(&ctx), " does not exist ", &file));
}

static void test_skip_quoted(void) {
  char x[] = "a=1, b=2  c='hi \' there'", *s = x, *p;

  p = skip_quoted(&s, ", ", ", ", 0);
  ASSERT(p != NULL && !strcmp(p, "a=1"));

  p = skip_quoted(&s, ", ", ", ", 0);
  ASSERT(p != NULL && !strcmp(p, "b=2"));

  // TODO(lsm): fix this
#if 0
  p = skip_quoted(&s, "'", ", ", '\\');
  p = skip_quoted(&s, "'", ", ", '\\');
  printf("[%s]\n", p);
  ASSERT(p != NULL && !strcmp(p, "hi ' there"));
#endif
}

static void test_alloc_vprintf(void) {
  char buf[MG_BUF_LEN], *p = buf;

  ASSERT(alloc_printf(&p, sizeof(buf), "%s", "hi") == 2);
  ASSERT(p == buf);
  ASSERT(alloc_printf(&p, sizeof(buf), "%s", "") == 0);
  ASSERT(alloc_printf(&p, sizeof(buf), "") == 0);

  // Pass small buffer, make sure alloc_printf allocates
  ASSERT(alloc_printf(&p, 1, "%s", "hello") == 5);
  ASSERT(p != buf);
  free(p);
}

static void test_request_replies(void) {
  char ebuf[100];
  int i, port = atoi(HTTPS_PORT);
  struct mg_connection *conn;
  struct mg_context *ctx;
  static struct { const char *request, *reply_regex; } tests[] = {
    {
      "GET test/hello.txt HTTP/1.0\r\nRange: bytes=3-5\r\n\r\n",
      "^HTTP/1.1 206 Partial Content"
    },
    {NULL, NULL},
  };

  ASSERT((ctx = mg_start(&CALLBACKS, NULL, OPTIONS)) != NULL);
  for (i = 0; tests[i].request != NULL; i++) {
    ASSERT((conn = mg_download("localhost", port, 1, ebuf, sizeof(ebuf), "%s",
                               tests[i].request)) != NULL);
    mg_close_connection(conn);
  }
  mg_stop(ctx);
}

static int api_callback(struct mg_connection *conn) {
  struct mg_request_info *ri = mg_get_request_info(conn);
  char post_data[100] = "";

  ASSERT(ri->user_data == (void *) 123);
  ASSERT(ri->num_headers == 2);
  ASSERT(strcmp(mg_get_header(conn, "host"), "blah.com") == 0);
  ASSERT(mg_read(conn, post_data, sizeof(post_data)) == 3);
  ASSERT(memcmp(post_data, "b=1", 3) == 0);
  ASSERT(ri->query_string != NULL);
  ASSERT(ri->remote_ip > 0);
  ASSERT(ri->remote_port > 0);
  ASSERT(strcmp(ri->http_version, "1.0") == 0);

  mg_printf(conn, "HTTP/1.0 200 OK\r\n\r\n");
  return 1;
}

static void test_api_calls(void) {
  char ebuf[100];
  struct mg_callbacks callbacks;
  struct mg_connection *conn;
  struct mg_context *ctx;
  static const char *request = "POST /?a=%20&b=&c=xx HTTP/1.0\r\n"
    "Host:  blah.com\n"     // More spaces before
    "content-length: 3\r\n" // Lower case header name
    "\r\nb=123456";         // Content size > content-length, test for mg_read()

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = api_callback;
  ASSERT((ctx = mg_start(&callbacks, (void *) 123, OPTIONS)) != NULL);
  ASSERT((conn = mg_download("localhost", atoi(HTTPS_PORT), 1,
                             ebuf, sizeof(ebuf), "%s", request)) != NULL);
  mg_close_connection(conn);
  mg_stop(ctx);
}

static void test_url_decode(void) {
  char buf[100];

  ASSERT(mg_url_decode("foo", 3, buf, 3, 0) == -1);  // No space for \0
  ASSERT(mg_url_decode("foo", 3, buf, 4, 0) == 3);
  ASSERT(strcmp(buf, "foo") == 0);

  ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 0) == 2);
  ASSERT(strcmp(buf, "a+") == 0);

  ASSERT(mg_url_decode("a+", 2, buf, sizeof(buf), 1) == 2);
  ASSERT(strcmp(buf, "a ") == 0);

  ASSERT(mg_url_decode("%61", 1, buf, sizeof(buf), 1) == 1);
  ASSERT(strcmp(buf, "%") == 0);

  ASSERT(mg_url_decode("%61", 2, buf, sizeof(buf), 1) == 2);
  ASSERT(strcmp(buf, "%6") == 0);

  ASSERT(mg_url_decode("%61", 3, buf, sizeof(buf), 1) == 1);
  ASSERT(strcmp(buf, "a") == 0);
}

static void test_mg_strcasestr(void) {
  static const char *big1 = "abcdef";
  ASSERT(mg_strcasestr("Y", "X") == NULL);
  ASSERT(mg_strcasestr("Y", "y") != NULL);
  ASSERT(mg_strcasestr(big1, "X") == NULL);
  ASSERT(mg_strcasestr(big1, "CD") == big1 + 2);
  ASSERT(mg_strcasestr("aa", "AAB") == NULL);
}

static void test_mg_get_cookie(void) {
  char buf[20];

  ASSERT(mg_get_cookie("", "foo", NULL, sizeof(buf)) == -2);
  ASSERT(mg_get_cookie("", "foo", buf, 0) == -2);
  ASSERT(mg_get_cookie("", "foo", buf, sizeof(buf)) == -1);
  ASSERT(mg_get_cookie("", NULL, buf, sizeof(buf)) == -1);
  ASSERT(mg_get_cookie("a=1; b=2; c; d", "a", buf, sizeof(buf)) == 1);
  ASSERT(strcmp(buf, "1") == 0);
  ASSERT(mg_get_cookie("a=1; b=2; c; d", "b", buf, sizeof(buf)) == 1);
  ASSERT(strcmp(buf, "2") == 0);
  ASSERT(mg_get_cookie("a=1; b=123", "b", buf, sizeof(buf)) == 3);
  ASSERT(strcmp(buf, "123") == 0);
  ASSERT(mg_get_cookie("a=1; b=2; c; d", "c", buf, sizeof(buf)) == -1);
}

static void test_strtoll(void) {
  ASSERT(strtoll("0", NULL, 10) == 0);
  ASSERT(strtoll("123", NULL, 10) == 123);
  ASSERT(strtoll("-34", NULL, 10) == -34);
  ASSERT(strtoll("3566626116", NULL, 10) == 3566626116);
}

static void test_parse_port_string(void) {
  static const char *valid[] = {
    "1", "1s", "1r", "1.2.3.4:1", "1.2.3.4:1s", "1.2.3.4:1r",
#if defined(USE_IPV6)
    "[::1]:123", "[3ffe:2a00:100:7031::1]:900",
#endif
    NULL
  };
  static const char *invalid[] = {
    "0", "99999", "1k", "1.2.3", "1.2.3.4:", "1.2.3.4:2p",
    NULL
  };
  struct socket so;
  struct vec vec;
  int i;

  for (i = 0; valid[i] != NULL; i++) {
    vec.ptr = valid[i];
    vec.len = strlen(vec.ptr);
    ASSERT(parse_port_string(&vec, &so) != 0);
  }

  for (i = 0; invalid[i] != NULL; i++) {
    vec.ptr = invalid[i];
    vec.len = strlen(vec.ptr);
    ASSERT(parse_port_string(&vec, &so) == 0);
  }
}

int __cdecl main(void) {
  test_parse_port_string();
  test_mg_strcasestr();
  test_alloc_vprintf();
  test_base64_encode();
  test_match_prefix();
  test_remove_double_dots();
  test_should_keep_alive();
  test_parse_http_message();
  test_mg_download();
  test_mg_get_var();
  test_set_throttle();
  test_next_option();
  test_mg_stat();
  test_skip_quoted();
  test_mg_upload();
  test_request_replies();
  test_api_calls();
  test_url_decode();
  test_mg_get_cookie();
  test_strtoll();
#ifdef USE_LUA
  test_lua();
#endif

  printf("TOTAL TESTS: %d, FAILED: %d\n", s_total_tests, s_failed_tests);

  return s_failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
