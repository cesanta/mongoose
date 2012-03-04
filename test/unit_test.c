#include "mongoose.c"

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
  return 0;
}
