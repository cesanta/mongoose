#include "str.h"

struct mg_str mg_str_s(const char *s) {
  struct mg_str str = {(char *) s, s == NULL ? 0 : strlen(s)};
  return str;
}

struct mg_str mg_str_n(const char *s, size_t n) {
  struct mg_str str = {(char *) s, n};
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
  int r = strncmp(s1->buf, s2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

int mg_vcasecmp(const struct mg_str *str1, const char *str2) {
  size_t n2 = strlen(str2), n1 = str1->len;
  int r = mg_ncasecmp(str1->buf, str2, (n1 < n2) ? n1 : n2);
  if (r == 0) return (int) (n1 - n2);
  return r;
}

struct mg_str mg_strdup(const struct mg_str s) {
  struct mg_str r = {NULL, 0};
  if (s.len > 0 && s.buf != NULL) {
    char *sc = (char *) calloc(1, s.len + 1);
    if (sc != NULL) {
      memcpy(sc, s.buf, s.len);
      sc[s.len] = '\0';
      r.buf = sc;
      r.len = s.len;
    }
  }
  return r;
}

int mg_strcmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    int c1 = str1.buf[i];
    int c2 = str2.buf[i];
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
  if (needle.len == 0) return haystack.buf;
  for (i = 0; i <= haystack.len - needle.len; i++) {
    if (memcmp(haystack.buf + i, needle.buf, needle.len) == 0) {
      return haystack.buf + i;
    }
  }
  return NULL;
}

static bool is_space(int c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

struct mg_str mg_strstrip(struct mg_str s) {
  while (s.len > 0 && is_space((int) *s.buf)) s.buf++, s.len--;
  while (s.len > 0 && is_space((int) *(s.buf + s.len - 1))) s.len--;
  return s;
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

bool mg_globmatch(const char *s1, size_t n1, const char *s2, size_t n2) {
  return mg_match(mg_str_n(s2, n2), mg_str_n(s1, n1), NULL);
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

char *mg_hex(const void *buf, size_t len, char *to) {
  const unsigned char *p = (const unsigned char *) buf;
  const char *hex = "0123456789abcdef";
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

bool mg_path_is_sane(const struct mg_str path) {
  const char *s = path.buf;
  size_t n = path.len;
  if (path.buf[0] == '.' && path.buf[1] == '.') return false;  // Starts with ..
  for (; s[0] != '\0' && n > 0; s++, n--) {
    if ((s[0] == '/' || s[0] == '\\') && n >= 2) {   // Subdir?
      if (s[1] == '.' && s[2] == '.') return false;  // Starts with ..
    }
  }
  return true;
}
