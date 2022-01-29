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
  return tolower(*(const unsigned char *) s);
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

struct mg_str mg_strstrip(struct mg_str s) {
  while (s.len > 0 && isspace((int) *s.ptr)) s.ptr++, s.len--;
  while (s.len > 0 && isspace((int) *(s.ptr + s.len - 1))) s.len--;
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
