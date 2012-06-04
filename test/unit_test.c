#include "mongoose.c"

static void test_should_keep_alive(void) {
  struct mg_connection conn;
  struct mg_context ctx;
  char req1[] = "GET / HTTP/1.1\r\n\r\n";
  char req2[] = "GET / HTTP/1.0\r\n\r\n";
  char req3[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
  char req4[] = "GET / HTTP/1.1\r\nConnection: keep-alive\r\n\r\n";

  memset(&conn, 0, sizeof(conn));
  conn.ctx = &ctx;
  parse_http_request(req1, &conn.request_info);

  ctx.config[ENABLE_KEEP_ALIVE] = "no";
  assert(should_keep_alive(&conn) == 0);

  ctx.config[ENABLE_KEEP_ALIVE] = "yes";
  assert(should_keep_alive(&conn) == 1);

  conn.must_close = 1;
  assert(should_keep_alive(&conn) == 0);

  conn.must_close = 0;
  parse_http_request(req2, &conn.request_info);
  assert(should_keep_alive(&conn) == 0);

  parse_http_request(req3, &conn.request_info);
  assert(should_keep_alive(&conn) == 0);

  parse_http_request(req4, &conn.request_info);
  assert(should_keep_alive(&conn) == 1);

  conn.request_info.status_code = 401;
  //assert(should_keep_alive(&conn) == 0);

  conn.request_info.status_code = 200;
  conn.must_close = 1;
  assert(should_keep_alive(&conn) == 0);
}

static void test_match_prefix(void) {
  assert(match_prefix("/a/", 3, "/a/b/c") == 3);
  assert(match_prefix("/a/", 3, "/ab/c") == -1);
  assert(match_prefix("/*/", 3, "/ab/c") == 4);
  assert(match_prefix("**", 2, "/a/b/c") == 6);
  assert(match_prefix("/*", 2, "/a/b/c") == 2);
  assert(match_prefix("*/*", 3, "/a/b/c") == 2);
  assert(match_prefix("**/", 3, "/a/b/c") == 5);
  assert(match_prefix("**.foo|**.bar", 13, "a.bar") == 5);
  assert(match_prefix("a|b|cd", 6, "cdef") == 2);
  assert(match_prefix("a|b|c?", 6, "cdef") == 2);
  assert(match_prefix("a|?|cd", 6, "cdef") == 1);
  assert(match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi") == -1);
  assert(match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi") == 12);
  assert(match_prefix("**/", 3, "/a/b/c") == 5);
  assert(match_prefix("**/$", 4, "/a/b/c") == -1);
  assert(match_prefix("**/$", 4, "/a/b/") == 5);
  assert(match_prefix("$", 1, "") == 0);
  assert(match_prefix("$", 1, "x") == -1);
  assert(match_prefix("*$", 2, "x") == 1);
  assert(match_prefix("/$", 2, "/") == 1);
  assert(match_prefix("**/$", 4, "/a/b/c") == -1);
  assert(match_prefix("**/$", 4, "/a/b/") == 5);
  assert(match_prefix("*", 1, "/hello/") == 0);
  assert(match_prefix("**.a$|**.b$", 11, "/a/b.b/") == -1);
  assert(match_prefix("**.a$|**.b$", 11, "/a/b.b") == 6);
  assert(match_prefix("**.a$|**.b$", 11, "/a/b.a") == 6);
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
#if defined(_WIN32)
    {"/\\", "/"},
#else
    {"/\\", "/\\"},
#endif
    {"/a\\", "/a\\"},
  };
  size_t i;

  for (i = 0; i < ARRAY_SIZE(data); i++) {
    //printf("[%s] -> [%s]\n", data[i].before, data[i].after);
    remove_double_dots_and_double_slashes(data[i].before);
    assert(strcmp(data[i].before, data[i].after) == 0);
  }
}

int main(void) {
  test_match_prefix();
  test_remove_double_dots();
  test_should_keep_alive();
  return 0;
}
