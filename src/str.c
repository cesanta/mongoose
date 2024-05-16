#include "str.h"

struct mg_str mg_str_s(const char *s) {
  struct mg_str str = {(char *) s, s == NULL ? 0 : strlen(s)};
  return str;
}

struct mg_str mg_str_n(const char *s, size_t n) {
  struct mg_str str = {(char *) s, n};
  return str;
}

static int mg_tolc(char c) {
  return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

int mg_casecmp(const char *s1, const char *s2) {
  int diff = 0;
  do {
    int c = mg_tolc(*s1++), d = mg_tolc(*s2++);
    diff = c - d;
  } while (diff == 0 && s1[-1] != '\0');
  return diff;
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

int mg_strcasecmp(const struct mg_str str1, const struct mg_str str2) {
  size_t i = 0;
  while (i < str1.len && i < str2.len) {
    int c1 = mg_tolc(str1.buf[i]);
    int c2 = mg_tolc(str2.buf[i]);
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

bool mg_str_to_num(struct mg_str str, int base, void *val, size_t val_len) {
  size_t i = 0, ndigits = 0;
  uint64_t max = val_len == sizeof(uint8_t)   ? 0xFF
                 : val_len == sizeof(uint16_t) ? 0xFFFF
                 : val_len == sizeof(uint32_t) ? 0xFFFFFFFF
                                : (uint64_t) ~0;
  uint64_t result = 0;
  if (max == (uint64_t) ~0 && val_len != sizeof(uint64_t)) return false;
  if (base == 0 && str.len >= 2) {
    if (str.buf[i] == '0') {
      i++;
      base = str.buf[i] == 'b' ? 2 : str.buf[i] == 'x' ? 16 : 10;
      if (base != 10) ++i;
    } else {
      base = 10;
    }
  }
  switch (base) {
    case 2:
      while (i < str.len && (str.buf[i] == '0' || str.buf[i] == '1')) {
        uint64_t digit = (uint64_t) (str.buf[i] - '0');
        if (result > max/2) return false;  // Overflow
        result *= 2;
        if (result > max - digit) return false;  // Overflow
        result += digit;
        i++, ndigits++;
      }
      break;
    case 10:
      while (i < str.len && str.buf[i] >= '0' && str.buf[i] <= '9') {
        uint64_t digit = (uint64_t) (str.buf[i] - '0');
        if (result > max/10) return false;  // Overflow
        result *= 10;
        if (result > max - digit) return false;  // Overflow
        result += digit;
        i++, ndigits++;
    }
      break;
    case 16:
      while (i < str.len) {
        char c = str.buf[i];
        uint64_t digit = (c >= '0' && c <= '9')   ? (uint64_t) (c - '0')
                         : (c >= 'A' && c <= 'F') ? (uint64_t) (c - '7')
                         : (c >= 'a' && c <= 'f') ? (uint64_t) (c - 'W')
                                                  : (uint64_t) ~0;
        if (digit == (uint64_t) ~0) break;
        if (result > max/16) return false;  // Overflow
        result *= 16;
        if (result > max - digit) return false;  // Overflow
        result += digit;
        i++, ndigits++;
      }
      break;
    default:
      return false;
  }
  if (ndigits == 0) return false;
  if (i != str.len) return false;
  if (val_len == 1) {
    *((uint8_t *) val) = (uint8_t) result;
  } else if (val_len == 2) {
    *((uint16_t *) val) = (uint16_t) result;
  } else if (val_len == 4) {
    *((uint32_t *) val) = (uint32_t) result;
  } else {
    *((uint64_t *) val) = (uint64_t) result;
  }
  return true;
}
