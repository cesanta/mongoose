#include "log.h"
#include "util.h"

#if MG_ENABLE_LOG
static void mg_log_stdout(const void *buf, size_t len, void *userdata) {
  (void) userdata, (void) buf, (void) len;
#if MG_ENABLE_FILE
  fwrite(buf, 1, len, stdout);
#endif
}

static const char *s_spec = "2";
static void (*s_fn)(const void *, size_t, void *) = mg_log_stdout;
static void *s_fn_param = NULL;

void mg_log_set(const char *spec) {
  MG_DEBUG(("Setting log level to %s", spec));
  s_spec = spec;
}

bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  // static unsigned long seq;
  int max = MG_LL_INFO;
  struct mg_str k, v, s = mg_str(s_spec);
  const char *p = strrchr(file, '/');

  if (s_fn == NULL) return false;

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
    buf[sizeof(buf) - 1] = '\0';
    s_fn(buf, n - 1, s_fn_param);
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
  s_fn(buf, len, s_fn_param);
  s_fn("\n", 1, s_fn_param);
  if (buf != mem) free(buf);
}

void mg_log_set_callback(void (*fn)(const void *, size_t, void *), void *fnd) {
  s_fn = fn;
  s_fn_param = fnd;
}
#endif
