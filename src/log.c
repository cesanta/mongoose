#include "log.h"
#include "util.h"

static const char *s_spec = "2";

static void logc(unsigned char c) {
  MG_PUTCHAR(c);
}

static void logs(const char *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) logc(((unsigned char *) buf)[i]);
}

void mg_log_set(const char *spec) {
  MG_DEBUG(("Setting log level to %s", spec));
  s_spec = spec;
}

bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  // static unsigned long seq;
  int max = MG_LL_INFO;
  struct mg_str k, v, s = mg_str(s_spec);
  const char *p = strrchr(file, '/');

  if (p == NULL) p = strrchr(file, '\\');
  p = p == NULL ? file : p + 1;

  while (mg_commalist(&s, &k, &v)) {
    if (v.len == 0) max = atoi(k.ptr);
    if (v.len > 0 && strncmp(p, k.ptr, k.len) == 0) max = atoi(v.ptr);
  }

  if (level <= max) {
    char buf[41];
    size_t n = mg_snprintf(buf, sizeof(buf), "%llx %d %s:%d:%s", mg_millis(),
                           level, p, line, fname);
    if (n > sizeof(buf) - 2) n = sizeof(buf) - 2;
    while (n < sizeof(buf)) buf[n++] = ' ';
    logs(buf, n - 1);
    return true;
  } else {
    return false;
  }
}

void mg_log(const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  size_t len;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  logs(buf, len);
  logc((unsigned char) '\n');
  if (buf != mem) free(buf);
}

static unsigned char nibble(unsigned c) {
  return (unsigned char) (c < 10 ? c + '0' : c + 'W');
}

#define ISPRINT(x) ((x) >= ' ' && (x) <= '~')
void mg_hexdump(const void *buf, size_t len) {
  const unsigned char *p = (const unsigned char *) buf;
  unsigned char ascii[16], alen = 0;
  size_t i;
  for (i = 0; i < len; i++) {
    if ((i % 16) == 0) {
      // Print buffered ascii chars
      if (i > 0) logs("  ", 2), logs((char *) ascii, 16), logc('\n'), alen = 0;
      // Print hex address, then \t
      logc(nibble((i >> 12) & 15)), logc(nibble((i >> 8) & 15)),
          logc(nibble((i >> 4) & 15)), logc('0'), logs("   ", 3);
    }
    logc(nibble(p[i] >> 4)), logc(nibble(p[i] & 15));  // Two nibbles, e.g. c5
    logc(' ');                                         // Space after hex number
    ascii[alen++] = ISPRINT(p[i]) ? p[i] : '.';        // Add to the ascii buf
  }
  while (alen < 16) logs("   ", 3), ascii[alen++] = ' ';
  logs("  ", 2), logs((char *) ascii, 16), logc('\n');
}
