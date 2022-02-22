#include "str.h"
#include <stdlib.h>

struct mg_str mg_str_s(const char *s) {
  struct mg_str str = {s, s == NULL ? 0 : strlen(s)};
  return str;
}

struct mg_str mg_str_n(const char *s, size_t n) {
  struct mg_str str = {s, n};
  return str;
}

int mg_lower(const char *s) {
  int c = *s;
  if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
  return c;
}

int mg_ncasecmp(const char *s1, const char *s2, size_t len) {
  int diff = 0;
  if (len > 0) do {
      diff = mg_lower(s1++) - mg_lower(s2++);
    } while (diff == 0 && s1[-1] != '\0' && --len > 0);
  return diff;
}

int mg_casecmp(const char *s1, const char *s2) {
  return mg_ncasecmp(s1, s2, (size_t) ~0);
}

int mg_vcmp(const struct mg_str *s1, const char *s2) {
  size_t n2 = strlen(s2), n1 = s1->len;
  int r = strncmp(s1->ptr, s2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

int mg_vcasecmp(const struct mg_str *str1, const char *str2) {
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = mg_ncasecmp(str1->ptr, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

struct mg_str mg_strdup(const struct mg_str s) {
  struct mg_str r = {NULL, 0};
  if (s.len > 0 && s.ptr != NULL) {
    char *sc = (char *) calloc(1, s.len + 1);
    if (sc != NULL) {
      memcpy(sc, s.ptr, s.len);
      sc[s.len] = '\0';
      r.ptr = sc;
      r.len = s.len;
    }
  }
  return r;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    int c1 = str1.ptr[i];
    int c2 = str2.ptr[i];
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    i++;
  }
  if (i < str1.len) return 1;
  if (i < str2.len) return -1;
  return 0;
}

const char *mg_strstr(const struct mg_str haystack,
                      const struct mg_str needle) {
  size_t i;
  if (needle.len > haystack.len) return NULL;
  for (i = 0; i <= haystack.len - needle.len; i++) {
    if (memcmp(haystack.ptr + i, needle.ptr, needle.len) == 0) {
      return haystack.ptr + i;
    }
  }
  return NULL;
}

static bool is_digit(int c) {
  return c >= '0' && c <= '9';
}

static bool is_space(int c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

struct mg_str mg_strstrip(struct mg_str s) {
  while (s.len > 0 && is_space((int) *s.ptr)) s.ptr++, s.len--;
  while (s.len > 0 && is_space((int) *(s.ptr + s.len - 1))) s.len--;
  return s;
}

bool mg_match(struct mg_str s, struct mg_str p, struct mg_str *caps) {
  size_t i = 0, j = 0, ni = 0, nj = 0;
  if (caps) caps->ptr = NULL, caps->len = 0;
  while (i < p.len || j < s.len) {
    if (i < p.len && j < s.len && (p.ptr[i] == '?' || s.ptr[j] == p.ptr[i])) {
      if (caps == NULL) {
      } else if (p.ptr[i] == '?') {
        caps->ptr = &s.ptr[j], caps->len = 1;     // Finalize `?` cap
        caps++, caps->ptr = NULL, caps->len = 0;  // Init next cap
      } else if (caps->ptr != NULL && caps->len == 0) {
        caps->len = (size_t) (&s.ptr[j] - caps->ptr);  // Finalize current cap
        caps++, caps->len = 0, caps->ptr = NULL;       // Init next cap
      }
      i++, j++;
    } else if (i < p.len && (p.ptr[i] == '*' || p.ptr[i] == '#')) {
      if (caps && !caps->ptr) caps->len = 0, caps->ptr = &s.ptr[j];  // Init cap
      ni = i++, nj = j + 1;
    } else if (nj > 0 && nj <= s.len && (p.ptr[ni] == '#' || s.ptr[j] != '/')) {
      i = ni, j = nj;
      if (caps && caps->ptr == NULL && caps->len == 0) {
        caps--, caps->len = 0;  // Restart previous cap
      }
    } else {
      return false;
    }
  }
  if (caps && caps->ptr && caps->len == 0) {
    caps->len = (size_t) (&s.ptr[j] - caps->ptr);
  }
  return true;
}

bool mg_globmatch(const char *s1, size_t n1, const char *s2, size_t n2) {
  return mg_match(mg_str_n(s2, n2), mg_str_n(s1, n1), NULL);
}

static size_t mg_nce(const char *s, size_t n, size_t ofs, size_t *koff,
                     size_t *klen, size_t *voff, size_t *vlen) {
  size_t kvlen, kl;
  for (kvlen = 0; ofs + kvlen < n && s[ofs + kvlen] != ',';) kvlen++;
  for (kl = 0; kl < kvlen && s[ofs + kl] != '=';) kl++;
  if (koff != NULL) *koff = ofs;
  if (klen != NULL) *klen = kl;
  if (voff != NULL) *voff = kl < kvlen ? ofs + kl + 1 : 0;
  if (vlen != NULL) *vlen = kl < kvlen ? kvlen - kl - 1 : 0;
  ofs += kvlen + 1;
  return ofs > n ? n : ofs;
}

bool mg_commalist(struct mg_str *s, struct mg_str *k, struct mg_str *v) {
  size_t koff = 0, klen = 0, voff = 0, vlen = 0, off = 0;
  if (s->ptr == NULL || s->len == 0) return 0;
  off = mg_nce(s->ptr, s->len, 0, &koff, &klen, &voff, &vlen);
  if (k != NULL) *k = mg_str_n(s->ptr + koff, klen);
  if (v != NULL) *v = mg_str_n(s->ptr + voff, vlen);
  *s = mg_str_n(s->ptr + off, s->len - off);
  return off > 0;
}

size_t mg_snprintf(char *buf, size_t len, const char *fmt, ...) {
  va_list ap;
  size_t n;
  va_start(ap, fmt);
  n = mg_vsnprintf(buf, len, fmt, ap);
  va_end(ap);
  return n;
}

char *mg_hexdump(const void *buf, size_t len) {
  const unsigned char *p = (const unsigned char *) buf;
  size_t i, idx, n = 0, ofs = 0, dlen = len * 5 + 100;
  char ascii[17] = "", *dst = (char *) calloc(1, dlen);
  if (dst == NULL) return dst;
  for (i = 0; i < len; i++) {
    idx = i % 16;
    if (idx == 0) {
      if (i > 0 && dlen > n)
        n += mg_snprintf(dst + n, dlen - n, "  %s\n", ascii);
      if (dlen > n)
        n += mg_snprintf(dst + n, dlen - n, "%04x ", (int) (i + ofs));
    }
    if (dlen < n) break;
    n += mg_snprintf(dst + n, dlen - n, " %02x", p[i]);
    ascii[idx] = (char) (p[i] < 0x20 || p[i] > 0x7e ? '.' : p[i]);
    ascii[idx + 1] = '\0';
  }
  while (i++ % 16) {
    if (n < dlen) n += mg_snprintf(dst + n, dlen - n, "%s", "   ");
  }
  if (n < dlen) n += mg_snprintf(dst + n, dlen - n, "  %s\n", ascii);
  if (n > dlen - 1) n = dlen - 1;
  dst[n] = '\0';
  return dst;
}

char *mg_hex(const void *buf, size_t len, char *to) {
  const unsigned char *p = (const unsigned char *) buf;
  static const char *hex = "0123456789abcdef";
  size_t i = 0;
  for (; len--; p++) {
    to[i++] = hex[p[0] >> 4];
    to[i++] = hex[p[0] & 0x0f];
  }
  to[i] = '\0';
  return to;
}

static unsigned char mg_unhex_nimble(unsigned char c) {
  return (c >= '0' && c <= '9')   ? (unsigned char) (c - '0')
         : (c >= 'A' && c <= 'F') ? (unsigned char) (c - '7')
                                  : (unsigned char) (c - 'W');
}

unsigned long mg_unhexn(const char *s, size_t len) {
  unsigned long i = 0, v = 0;
  for (i = 0; i < len; i++) v <<= 4, v |= mg_unhex_nimble(((uint8_t *) s)[i]);
  return v;
}

void mg_unhex(const char *buf, size_t len, unsigned char *to) {
  size_t i;
  for (i = 0; i < len; i += 2) {
    to[i >> 1] = (unsigned char) mg_unhexn(&buf[i], 2);
  }
}

size_t mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  size_t len;

  va_copy(ap_copy, ap);
  len = mg_vsnprintf(*buf, size, fmt, ap_copy);
  va_end(ap_copy);

  if (len >= size) {
    //  Allocate a buffer that is large enough
    if ((*buf = (char *) calloc(1, len + 1)) == NULL) {
      len = 0;
    } else {
      va_copy(ap_copy, ap);
      len = mg_vsnprintf(*buf, len + 1, fmt, ap_copy);
      va_end(ap_copy);
    }
  }

  return len;
}

size_t mg_asprintf(char **buf, size_t size, const char *fmt, ...) {
  size_t ret;
  va_list ap;
  va_start(ap, fmt);
  ret = mg_vasprintf(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

int64_t mg_to64(struct mg_str str) {
  int64_t result = 0, neg = 1, max = 922337203685477570 /* INT64_MAX/10-10 */;
  size_t i = 0;
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (i < str.len && str.ptr[i] == '-') neg = -1, i++;
  while (i < str.len && str.ptr[i] >= '0' && str.ptr[i] <= '9') {
    if (result > max) return 0;
    result *= 10;
    result += (str.ptr[i] - '0');
    i++;
  }
  return result * neg;
}

size_t mg_lld(char *buf, int64_t val, bool is_signed, bool is_hex) {
  const char *letters = "0123456789abcdef";
  uint64_t v = (uint64_t) val;
  size_t s = 0, n, i;
  if (is_signed && val < 0) buf[s++] = '-', v = (uint64_t) (-val);
  // This loop prints a number in reverse order. I guess this is because we
  // write numbers from right to left: least significant digit comes last.
  // Maybe because we use Arabic numbers, and Arabs write RTL?
  if (is_hex) {
    for (n = 0; v; v >>= 4) buf[s + n++] = letters[v & 15];
  } else {
    for (n = 0; v; v /= 10) buf[s + n++] = letters[v % 10];
  }
  // Reverse a string
  for (i = 0; i < n / 2; i++) {
    char t = buf[s + i];
    buf[s + i] = buf[s + n - i - 1], buf[s + n - i - 1] = t;
  }
  if (val == 0) buf[n++] = '0';  // Handle special case
  return n + s;
}

static size_t mg_copys(char *buf, size_t len, size_t n, char *p, size_t k) {
  size_t j = 0;
  for (j = 0; j < k && p[j]; j++)
    if (j + n < len) buf[n + j] = p[j];
  return j;
}

size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list ap) {
  size_t i = 0, n = 0;
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      size_t j, k, x = 0, is_long = 0, w = 0 /* width */, pr = 0 /* prec */;
      char pad = ' ', minus = 0, c = fmt[++i];
      if (c == '#') x++, c = fmt[++i];
      if (c == '-') minus++, c = fmt[++i];
      if (c == '0') pad = '0', c = fmt[++i];
      while (is_digit(c)) w *= 10, w += (size_t) (c - '0'), c = fmt[++i];
      if (c == '.') {
        c = fmt[++i];
        if (c == '*') {
          pr = (size_t) va_arg(ap, int);
          c = fmt[++i];
        } else {
          while (is_digit(c)) pr *= 10, pr += (size_t) (c - '0'), c = fmt[++i];
        }
      }
      while (c == 'h') c = fmt[++i];  // Treat h and hh as int
      if (c == 'l') {
        is_long++, c = fmt[++i];
        if (c == 'l') is_long++, c = fmt[++i];
      }
      if (c == 'p') x = 1, is_long = 1;
      if (c == 'd' || c == 'u' || c == 'x' || c == 'X' || c == 'p') {
        bool s = (c == 'd'), h = (c == 'x' || c == 'X' || c == 'p');
        char tmp[30];
        size_t xl = x ? 2 : 0;
        if (is_long == 2) {
          int64_t v = va_arg(ap, int64_t);
          k = mg_lld(tmp, v, s, h);
        } else if (is_long == 1) {
          long v = va_arg(ap, long);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned long) v, s, h);
        } else {
          int v = va_arg(ap, int);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned) v, s, h);
        }
        for (j = 0; j < xl && w > 0; j++) w--;
        for (j = 0; pad == ' ' && !minus && k < w && j + k < w; j++)
          n += mg_copys(buf, len, n, &pad, 1);
        n += mg_copys(buf, len, n, (char *) "0x", xl);
        for (j = 0; pad == '0' && k < w && j + k < w; j++)
          n += mg_copys(buf, len, n, &pad, 1);
        n += mg_copys(buf, len, n, tmp, k);
        for (j = 0; pad == ' ' && minus && k < w && j + k < w; j++)
          n += mg_copys(buf, len, n, &pad, 1);
      } else if (c == 'c') {
        int p = va_arg(ap, int);
        if (n < len) buf[n] = (char) p;
        n++;
      } else if (c == 's') {
        char *p = va_arg(ap, char *);
        if (pr == 0) pr = p == NULL ? 0 : strlen(p);
        for (j = 0; !minus && pr < w && j + pr < w; j++)
          n += mg_copys(buf, len, n, &pad, 1);
        n += mg_copys(buf, len, n, p, pr);
        for (j = 0; minus && pr < w && j + pr < w; j++)
          n += mg_copys(buf, len, n, &pad, 1);
      } else if (c == '%') {
        if (n < len) buf[n] = '%';
        n++;
      } else {
        if (n < len) buf[n] = '%';
        n++;
        if (n < len) buf[n] = c;
        n++;
      }
      i++;
    } else {
      if (n < len) buf[n] = fmt[i];
      n++, i++;
    }
  }
  if (n < len) buf[n] = '\0';
  return n;
}
