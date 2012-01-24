#include "mongoose.c"

int main(void) {
  assert(match_prefix("/a/", 3, "/a/b/c") == 3);
  assert(match_prefix("/a/", 3, "/ab/c") == 0);
  assert(match_prefix("/*/", 3, "/ab/c") == 4);
  assert(match_prefix("**", 2, "/a/b/c") == 6);
  assert(match_prefix("/*", 2, "/a/b/c") == 2);
  assert(match_prefix("*/*", 3, "/a/b/c") == 2);
  assert(match_prefix("**/", 3, "/a/b/c") == 5);
  assert(match_prefix("**.foo|**.bar", 13, "a.bar") == 5);
  assert(match_prefix("a|b|cd", 6, "cdef") == 2);
  assert(match_prefix("a|b|c?", 6, "cdef") == 2);
  assert(match_prefix("a|?|cd", 6, "cdef") == 1);
  assert(match_prefix("/a/**.cgi", 9, "/foo/bar/x.cgi") == 0);
  assert(match_prefix("/a/**.cgi", 9, "/a/bar/x.cgi") == 12);

  return 0;
}
