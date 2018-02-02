#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ccgi.h>

extern char **environ;

#ifdef _WIN32
#define EOL "\n"
#else
#define EOL "\r\n"
#endif

int alpha_cmp(const void *a, const void *b) {
  return strcmp(*(const char **) a, *(const char **) b);
}

int main(void) {
  puts("Content-Type: text/html" EOL "Status: 201 Created" EOL EOL);

  puts("<pre>" EOL "<h1>Environment</h1>" EOL);
  {
    const char *sorted_env[500];
    size_t i, num_env;

    for (num_env = 0; environ[num_env] != 0; num_env++) {
      sorted_env[num_env] = environ[num_env];
    }
    qsort(sorted_env, num_env, sizeof(const char *), alpha_cmp);

    for (i = 0; i < num_env; i++) {
      printf("E: %s" EOL, sorted_env[i]);
    }
  }
  puts(EOL "<h1>Query string</h1>" EOL);
  {
    const char *k;
    CGI_varlist *vl = CGI_get_query(NULL);

    for (k = CGI_first_name(vl); k != NULL; k = CGI_next_name(vl)) {
      printf("Q: %s=%s" EOL, k, CGI_lookup(vl, k));
    }

    CGI_free_varlist(vl);
  }
  puts(EOL "<h1>Form variables</h1>" EOL);
  {
    const char *k;
    CGI_varlist *vl = CGI_get_post(NULL, NULL);

    for (k = CGI_first_name(vl); k != NULL; k = CGI_next_name(vl)) {
      printf("P: %s=%s" EOL, k, CGI_lookup(vl, k));
    }

    CGI_free_varlist(vl);
  }
  puts(EOL "</pre>" EOL);
  return 0;
}

/* Some functions for libccgi that are missing on Windows (VC6). */
#ifdef _WIN32
static int lowercase(const char *s) {
  return tolower(*(const unsigned char *) s);
}

int strncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;

  if (len > 0) do {
      diff = lowercase(s1++) - lowercase(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);

  return diff;
}

int strcasecmp(const char *s1, const char *s2) {
  return strncasecmp(s1, s2, (size_t) ~0);
}

int mkstemp(char *template) {
  return -1; /* Not used by us. */
}
#endif
