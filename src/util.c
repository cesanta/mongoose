#include "util.h"

size_t mg_file_size(const char *path) {
#if MG_ARCH == MG_ARCH_FREERTOS
  struct FF_STAT st;
  return ff_stat(path, &st) == 0 ? st.st_size : 0;
#else
  struct stat st;
  return stat(path, &st) == 0 ? st.st_size : 0;
#endif
}

char *mg_file_read(const char *path) {
  FILE *fp;
  char *data = NULL;
  size_t size = mg_file_size(path);
  if ((fp = fopen(path, "rb")) != NULL) {
    data = (char *) malloc(size + 1);
    if (data != NULL) {
      if (fread(data, 1, size, fp) != size) {
        free(data);
        data = NULL;
      } else {
        data[size] = '\0';
      }
    }
    fclose(fp);
  }
  return data;
}

int mg_file_write(const char *path, const char *fmt, ...) {
  char tmp[100];
  int written = 0;
  FILE *fp;
  snprintf(tmp, sizeof(tmp), "%s.%d", path, rand());
  fp = fopen(tmp, "wb");
  if (fp != NULL) {
    va_list ap;
    va_start(ap, fmt);
    written = vfprintf(fp, fmt, ap);
    va_end(ap);
    fclose(fp);
    remove(path);
    rename(tmp, path);
  }
  return written;
}

void mg_random(void *buf, size_t len) {
  FILE *fp = fopen("/dev/urandom", "rb");
  size_t i, n = 0;
  if (fp != NULL) n = fread(buf, 1, len, fp);
  if (fp == NULL || n <= 0) {
    for (i = 0; i < len; i++) ((unsigned char *) buf)[i] = rand() % 0xff;
  }
  if (fp != NULL) fclose(fp);
}

bool mg_globmatch(const char *s1, int n1, const char *s2, int n2) {
  int i = 0, j = 0, ni = 0, nj = 0;
  while (i < n1 || j < n2) {
    if (i < n1 && j < n2 && (s1[i] == '?' || s2[j] == s1[i])) {
      i++, j++;
    } else if (i < n1 && (s1[i] == '*' || s1[i] == '#')) {
      ni = i, nj = j + 1, i++;
    } else if (nj > 0 && nj <= n2 && (s1[i - 1] == '#' || s2[j] != '/')) {
      i = ni, j = nj;
    } else {
      return false;
    }
  }
  return true;
}

static int mg_nextcommaentry(const char *s, int slen, int ofs, int *koff,
                             int *klen, int *voff, int *vlen) {
  int kvlen, kl;
  for (kvlen = 0; ofs + kvlen < slen && s[ofs + kvlen] != ',';) kvlen++;
  for (kl = 0; kl < kvlen && s[ofs + kl] != '=';) kl++;
  if (koff != NULL) *koff = ofs;
  if (klen != NULL) *klen = kl;
  if (voff != NULL) *voff = kl < kvlen ? ofs + kl + 1 : 0;
  if (vlen != NULL) *vlen = kl < kvlen ? kvlen - kl - 1 : 0;
  return ofs >= slen ? slen : ofs + kvlen + 1;
}

bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v) {
  int koff, klen, voff, vlen;
  int off = mg_nextcommaentry(s->ptr, s->len, 0, &koff, &klen, &voff, &vlen);
  if (k != NULL) *k = mg_str_n(s->ptr + koff, klen);
  if (v != NULL) *v = mg_str_n(s->ptr + voff, vlen);
  *s = mg_str_n(s->ptr + off, s->len - off);
  return off > 0;
}

uint32_t mg_ntohl(uint32_t net) {
  uint8_t data[4] = {0, 0, 0, 0};
  memcpy(&data, &net, sizeof(data));
  return ((uint32_t) data[3] << 0) | ((uint32_t) data[2] << 8) |
         ((uint32_t) data[1] << 16) | ((uint32_t) data[0] << 24);
}

uint16_t mg_ntohs(uint16_t net) {
  uint8_t data[2] = {0, 0};
  memcpy(&data, &net, sizeof(data));
  return ((uint16_t) data[1] << 0) | ((uint32_t) data[0] << 8);
}

char *mg_hexdump(const void *buf, size_t len) {
  const unsigned char *p = (const unsigned char *) buf;
  size_t i, idx, n = 0, ofs = 0, dlen = len * 5 + 100;
  char ascii[17] = "", *dst = (char *) malloc(dlen);
  if (dst == NULL) return dst;
  for (i = 0; i < len; i++) {
    idx = i % 16;
    if (idx == 0) {
      if (i > 0 && dlen > n) n += snprintf(dst + n, dlen - n, "  %s\n", ascii);
      if (dlen > n) n += snprintf(dst + n, dlen - n, "%04x ", (int) (i + ofs));
    }
    if (dlen < n) break;
    n += snprintf(dst + n, dlen - n, " %02x", p[i]);
    ascii[idx] = p[i] < 0x20 || p[i] > 0x7e ? '.' : p[i];
    ascii[idx + 1] = '\0';
  }
  while (i++ % 16) {
    if (n < dlen) n += snprintf(dst + n, dlen - n, "%s", "   ");
  }
  if (n < dlen) n += snprintf(dst + n, dlen - n, "  %s\n", ascii);
  if (n > dlen - 1) n = dlen - 1;
  dst[n] = '\0';
  return dst;
}

char *mg_hex(const void *buf, int len, char *to) {
  const unsigned char *p = (const unsigned char *) buf;
  static const char *hex = "0123456789abcdef";
  int i = 0;
  for (; len--; p++) {
    to[i++] = hex[p[0] >> 4];
    to[i++] = hex[p[0] & 0x0f];
  }
  to[i] = '\0';
  return to;
}

unsigned long mg_unhexn(const char *s, int len) {
  unsigned long i = 0, v = 0;
  for (i = 0; i < (unsigned long) len; i++) {
    int c = s[i];
    if (i > 0) v <<= 4;
    v |= (c >= '0' && c <= '9') ? c - '0'
                                : (c >= 'A' && c <= 'F') ? c - '7' : c - 'W';
  }
  return v;
}

void mg_unhex(const char *buf, int len, unsigned char *to) {
  int i;
  for (i = 0; i < len; i += 2) {
    to[i >> 1] = (unsigned char) mg_unhexn(&buf[i], 2);
  }
}

int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  int len;

  va_copy(ap_copy, ap);
  len = vsnprintf(*buf, size, fmt, ap_copy);
  va_end(ap_copy);

  if (len < 0) {
    // eCos and Windows are not standard-compliant and return -1 when
    // the buffer is too small. Keep allocating larger buffers until we
    // succeed or out of memory.
    // LCOV_EXCL_START
    *buf = NULL;
    while (len < 0) {
      free(*buf);
      if (size == 0) size = 5;
      size *= 2;
      if ((*buf = (char *) malloc(size)) == NULL) {
        len = -1;
        break;
      }
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, size - 1, fmt, ap_copy);
      va_end(ap_copy);
    }
    // Microsoft version of vsnprintf() is not always null-terminated, so put
    // the terminator manually
    if (*buf != NULL) (*buf)[len] = 0;
    // LCOV_EXCL_STOP
  } else if (len >= (int) size) {
    /// Standard-compliant code path. Allocate a buffer that is large enough
    if ((*buf = (char *) malloc(len + 1)) == NULL) {
      len = -1;  // LCOV_EXCL_LINE
    } else {     // LCOV_EXCL_LINE
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, len + 1, fmt, ap_copy);
      va_end(ap_copy);
    }
  }

  return len;
}

int mg_asprintf(char **buf, size_t size, const char *fmt, ...) {
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = mg_vasprintf(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

int64_t mg_to64(struct mg_str str) {
  int64_t result = 0, neg = 1;
  size_t i = 0;
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (i < str.len && str.ptr[i] == '-') neg = -1, i++;
  while (i < str.len && str.ptr[i] >= '0' && str.ptr[i] <= '9') {
    result *= 10;
    result += (str.ptr[i] - '0');
    i++;
  }
  return result * neg;
}

double mg_time(void) {
#if MG_ARCH == MG_ARCH_WIN32
  SYSTEMTIME sysnow;
  FILETIME ftime;
  GetLocalTime(&sysnow);
  SystemTimeToFileTime(&sysnow, &ftime);
  /*
   * 1. VC 6.0 doesn't support conversion uint64 -> double, so, using int64
   * This should not cause a problems in this (21th) century
   * 2. Windows FILETIME is a number of 100-nanosecond intervals since January
   * 1, 1601 while time_t is a number of _seconds_ since January 1, 1970 UTC,
   * thus, we need to convert to seconds and adjust amount (subtract 11644473600
   * seconds)
   */
  return (double) (((int64_t) ftime.dwLowDateTime +
                    ((int64_t) ftime.dwHighDateTime << 32)) /
                   10000000.0) -
         11644473600;
#else
  struct timeval tv;
  if (gettimeofday(&tv, NULL /* tz */) != 0) return 0;
  return (double) tv.tv_sec + (((double) tv.tv_usec) / 1000000.0);
#endif /* _WIN32 */
}

void mg_usleep(unsigned long usecs) {
#if MG_ARCH == MG_ARCH_WIN32
  Sleep(usecs / 1000);
#elif MG_ARCH == MG_ARCH_ESP8266
  ets_delay_us(usecs);
#else
  usleep(usecs);
#endif
}

unsigned long mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_ESP32
  return esp_timer_get_time() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266
  // return system_get_time() / 1000;
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_FREERTOS
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#else
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}
