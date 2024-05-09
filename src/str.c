#include "str.h"

struct mg_str mg_str_s(const char *s) {
  struct mg_str str = {(char *) s, s == NULL ? 0 : strlen(s)};
  return str;
}

struct mg_str mg_str_n(const char *s, size_t n) {
  struct mg_str str = {(char *) s, n};
  return str;
}

static char tolc(char c) {
  return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int mg_casecmp(const char *s1, const char *s2) {
  int diff = 0;
  do {
    char c = tolc(*s1++), d = tolc(*s2++);
    diff = c - d;
  } while (diff == 0 && s1[-1] != '\0');
  return diff;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    char c1 = str1.buf[i];
    char c2 = str2.buf[i];
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    i++;
  }
  if (i < str1.len) return 1;
  if (i < str2.len) return -1;
  return 0;
}

int mg_strcasecmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    char c1 = tolc(str1.buf[i]);
    char c2 = tolc(str2.buf[i]);
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
    i++;
  }
  if (i < str1.len) return 1;
  if (i < str2.len) return -1;
  return 0;
}

bool mg_match(struct mg_str s, struct mg_str p, struct mg_str *caps) {
  size_t i = 0, j = 0, ni = 0, nj = 0;
  if (caps) caps->buf = NULL, caps->len = 0;
  while (i < p.len || j < s.len) {
    if (i < p.len && j < s.len && (p.buf[i] == '?' || s.buf[j] == p.buf[i])) {
      if (caps == NULL) {
      } else if (p.buf[i] == '?') {
        caps->buf = &s.buf[j], caps->len = 1;     // Finalize `?` cap
        caps++, caps->buf = NULL, caps->len = 0;  // Init next cap
      } else if (caps->buf != NULL && caps->len == 0) {
        caps->len = (size_t) (&s.buf[j] - caps->buf);  // Finalize current cap
        caps++, caps->len = 0, caps->buf = NULL;       // Init next cap
      }
      i++, j++;
    } else if (i < p.len && (p.buf[i] == '*' || p.buf[i] == '#')) {
      if (caps && !caps->buf) caps->len = 0, caps->buf = &s.buf[j];  // Init cap
      ni = i++, nj = j + 1;
    } else if (nj > 0 && nj <= s.len && (p.buf[ni] == '#' || s.buf[j] != '/')) {
      i = ni, j = nj;
      if (caps && caps->buf == NULL && caps->len == 0) {
        caps--, caps->len = 0;  // Restart previous cap
      }
    } else {
      return false;
    }
  }
  if (caps && caps->buf && caps->len == 0) {
    caps->len = (size_t) (&s.buf[j] - caps->buf);
  }
  return true;
}

bool mg_span(struct mg_str s, struct mg_str *a, struct mg_str *b, char sep) {
  if (s.len == 0 || s.buf == NULL) {
    return false;  // Empty string, nothing to span - fail
  } else {
    size_t len = 0;
    while (len < s.len && s.buf[len] != sep) len++;  // Find separator
    if (a) *a = mg_str_n(s.buf, len);                // Init a
    if (b) *b = mg_str_n(s.buf + len, s.len - len);  // Init b
    if (b && len < s.len) b->buf++, b->len--;        // Skip separator
    return true;
  }
}

uint8_t mg_toi(char c, int base) {
  return (c >= '0' && c <= '9') ? (uint8_t) (c - '0')
         : base == 16           ? (c >= 'A' && c <= 'F')   ? (uint8_t) (c - '7')
                                  : (c >= 'a' && c <= 'f') ? (uint8_t) (c - 'W')
                                                           : (uint8_t) ~0
                                : (uint8_t) ~0;
}

unsigned long mg_unhexn(const char *s, size_t len) {
  unsigned long i = 0, v = 0;
  for (i = 0; i < len; i++) v <<= 4, v |= mg_toi(((char *) s)[i], 16);
  return v;
}

void mg_unhex(const char *buf, size_t len, unsigned char *to) {
  size_t i;
  for (i = 0; i < len; i += 2) {
    to[i >> 1] = (unsigned char) mg_unhexn(&buf[i], 2);
  }
}
