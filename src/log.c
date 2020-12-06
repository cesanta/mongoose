#include "log.h"
#include "util.h"

#if MG_ENABLE_MGOS
#else
#if MG_ENABLE_LOG
static void mg_log_stdout(const void *buf, int len, void *userdata) {
  (void) userdata;
  fwrite(buf, 1, len, stdout);
}

static const char *s_spec = "2";
static void (*s_fn)(const void *, int, void *) = mg_log_stdout;
static void *s_fn_param = NULL;

void mg_log_set(const char *spec) {
  LOG(LL_INFO, ("Setting log level to %s", spec));
  s_spec = spec;
}

bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  // static unsigned long seq;
  int max = LL_INFO;
  struct mg_str k, v, s = mg_str(s_spec);
  const char *p = strrchr(file, '/');
  p = p == NULL ? file : p + 1;

  if (s_fn == NULL) return false;
  while (mg_next_comma_entry(&s, &k, &v)) {
    if (v.len == 0) max = atoi(k.ptr);
    if (v.len > 0 && strncmp(p, k.ptr, k.len) == 0) max = atoi(v.ptr);
  }

  if (level <= max) {
    char timebuf[21], buf[50] = "";
    time_t t = time(NULL);
    struct tm *tm = gmtime(&t);
    int n, tag;
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);
    tag = level == LL_ERROR ? 'E' : level == LL_INFO ? 'I' : ' ';
    n = snprintf(buf, sizeof(buf), "%s  %c %s:%d:%s", timebuf, tag, p, line,
                 fname);
    if (n < 0 || n > (int) sizeof(buf) - 2) n = sizeof(buf) - 2;
    while (n < (int) sizeof(buf) - 1) buf[n++] = ' ';
    s_fn(buf, sizeof(buf) - 1, s_fn_param);
    return true;
  } else {
    return false;
  }
}

void mg_log(const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  int len = 0;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  s_fn(buf, len, s_fn_param);
  s_fn("\n", 1, s_fn_param);
  if (buf != mem) free(buf);
}

void mg_log_set_callback(void (*fn)(const void *, int, void *), void *param) {
  s_fn = fn;
  s_fn_param = param;
}
#endif
#endif
