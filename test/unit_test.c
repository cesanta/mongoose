#include "mongoose.c"

int main(void) {
  assert(match_prefix("/a/", 3, "/a/b/c") == 3);
  assert(match_prefix("/a/", 3, "/ab/c") == 0);
  assert(match_prefix("/*/", 3, "/ab/c") == 4);
  assert(match_prefix("**", 2, "/a/b/c") == 6);
  assert(match_prefix("/*", 2, "/a/b/c") == 2);
  assert(match_prefix("*/*", 3, "/a/b/c") == 2);
  assert(match_prefix("**/", 3, "/a/b/c") == 5);

  return 0;
}
