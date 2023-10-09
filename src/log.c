#include "log.h"
#include "printf.h"
#include "str.h"
#include "util.h"

static int s_level = MG_LL_INFO;
static mg_pfn_t s_log_func = mg_pfn_stdout;
static void *s_log_func_param = NULL;

void mg_log_set_fn(mg_pfn_t fn, void *param) {
  s_log_func = fn;
  s_log_func_param = param;
}

static void logc(unsigned char c) {
  s_log_func((char) c, s_log_func_param);
}

static void logs(const char *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) logc(((unsigned char *) buf)[i]);
}

void mg_log_set(int log_level) {
  MG_DEBUG(("Setting log level to %d", log_level));
  s_level = log_level;
}

#if MG_ENABLE_CUSTOM_LOG
// Let user define their own mg_log_prefix() and mg_log()
#else
bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  if (level <= s_level) {
    const char *p = strrchr(file, '/');
    char buf[41];
    size_t n;
    if (p == NULL) p = strrchr(file, '\\');
    n = mg_snprintf(buf, sizeof(buf), "%-6llx %d %s:%d:%s", mg_millis(), level,
                    p == NULL ? file : p + 1, line, fname);
    if (n > sizeof(buf) - 2) n = sizeof(buf) - 2;
    while (n < sizeof(buf)) buf[n++] = ' ';
    logs(buf, n - 1);
    return true;
  } else {
    return false;
  }
}

void mg_log(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_vxprintf(s_log_func, s_log_func_param, fmt, &ap);
  va_end(ap);
  logs("\r\n", 2);
}
#endif

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
