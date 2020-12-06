// Copyright (c) 2018-2020 Cesanta Software Limited
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "mjson.h"

#if !defined(_MSC_VER) || _MSC_VER >= 1700
#else
#define va_copy(x, y) (x) = (y)
#define snprintf _snprintf
#define alloca _alloca
#endif

static int mjson_esc(int c, int esc) {
  const char *p, *esc1 = "\b\f\n\r\t\\\"", *esc2 = "bfnrt\\\"";
  for (p = esc ? esc1 : esc2; *p != '\0'; p++) {
    if (*p == c) return esc ? esc2[p - esc1] : esc1[p - esc2];
  }
  return 0;
}

static int mjson_pass_string(const char *s, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (s[i] == '\\' && i + 1 < len && mjson_esc(s[i + 1], 1)) {
      i++;
    } else if (s[i] == '\0') {
      return MJSON_ERROR_INVALID_INPUT;
    } else if (s[i] == '"') {
      return i;
    }
  }
  return MJSON_ERROR_INVALID_INPUT;
}

int mjson(const char *s, int len, mjson_cb_t cb, void *ud) {
  enum { S_VALUE, S_KEY, S_COLON, S_COMMA_OR_EOO } expecting = S_VALUE;
  unsigned char nesting[MJSON_MAX_DEPTH];
  int i, depth = 0;
#define MJSONCALL(ev) \
  if (cb != NULL && cb(ev, s, start, i - start + 1, ud)) return i + 1;

// In the ascii table, the distance between `[` and `]` is 2.
// Ditto for `{` and `}`. Hence +2 in the code below.
#define MJSONEOO()                                                     \
  do {                                                                 \
    if (c != nesting[depth - 1] + 2) return MJSON_ERROR_INVALID_INPUT; \
    depth--;                                                           \
    if (depth == 0) {                                                  \
      MJSONCALL(tok);                                                  \
      return i + 1;                                                    \
    }                                                                  \
  } while (0)

  for (i = 0; i < len; i++) {
    int start = i;
    unsigned char c = ((unsigned char *) s)[i];
    int tok = c;
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
    // printf("- %c [%.*s] %d %d\n", c, i, s, depth, expecting);
    switch (expecting) {
      case S_VALUE:
        if (c == '{') {
          if (depth >= (int) sizeof(nesting)) return MJSON_ERROR_TOO_DEEP;
          nesting[depth++] = c;
          expecting = S_KEY;
          break;
        } else if (c == '[') {
          if (depth >= (int) sizeof(nesting)) return MJSON_ERROR_TOO_DEEP;
          nesting[depth++] = c;
          break;
        } else if (c == ']' && depth > 0) {  // Empty array
          MJSONEOO();
        } else if (c == 't' && i + 3 < len && memcmp(&s[i], "true", 4) == 0) {
          i += 3;
          tok = MJSON_TOK_TRUE;
        } else if (c == 'n' && i + 3 < len && memcmp(&s[i], "null", 4) == 0) {
          i += 3;
          tok = MJSON_TOK_NULL;
        } else if (c == 'f' && i + 4 < len && memcmp(&s[i], "false", 5) == 0) {
          i += 4;
          tok = MJSON_TOK_FALSE;
        } else if (c == '-' || ((c >= '0' && c <= '9'))) {
          char *end = NULL;
          strtod(&s[i], &end);
          if (end != NULL) i += end - &s[i] - 1;
          tok = MJSON_TOK_NUMBER;
        } else if (c == '"') {
          int n = mjson_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          i += n + 1;
          tok = MJSON_TOK_STRING;
        } else {
          return MJSON_ERROR_INVALID_INPUT;
        }
        if (depth == 0) {
          MJSONCALL(tok);
          return i + 1;
        }
        expecting = S_COMMA_OR_EOO;
        break;

      case S_KEY:
        if (c == '"') {
          int n = mjson_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          i += n + 1;
          tok = MJSON_TOK_KEY;
          expecting = S_COLON;
        } else if (c == '}') {  // Empty object
          MJSONEOO();
          expecting = S_COMMA_OR_EOO;
        } else {
          return MJSON_ERROR_INVALID_INPUT;
        }
        break;

      case S_COLON:
        if (c == ':') {
          expecting = S_VALUE;
        } else {
          return MJSON_ERROR_INVALID_INPUT;
        }
        break;

      case S_COMMA_OR_EOO:
        if (depth <= 0) return MJSON_ERROR_INVALID_INPUT;
        if (c == ',') {
          expecting = (nesting[depth - 1] == '{') ? S_KEY : S_VALUE;
        } else if (c == ']' || c == '}') {
          MJSONEOO();
        } else {
          return MJSON_ERROR_INVALID_INPUT;
        }
        break;
    }
    MJSONCALL(tok);
  }
  return MJSON_ERROR_INVALID_INPUT;
}

struct msjon_get_data {
  const char *path;     // Lookup json path
  int pos;              // Current path index
  int d1;               // Current depth of traversal
  int d2;               // Expected depth of traversal
  int i1;               // Index in an array
  int i2;               // Expected index in an array
  int obj;              // If the value is array/object, offset where it starts
  const char **tokptr;  // Destination
  int *toklen;          // Destination length
  int tok;              // Returned token
};

static int mjson_plen(const char *s) {
  int i = 0;
  while (s[i] != '\0' && s[i] != '.' && s[i] != '[') i++;
  return i;
}

static int mjson_get_cb(int tok, const char *s, int off, int len, void *ud) {
  struct msjon_get_data *data = (struct msjon_get_data *) ud;
  // printf("--> %2x %2d %2d %2d %2d\t'%s'\t'%.*s'\t\t'%.*s'\n", tok, data->d1,
  // data->d2, data->i1, data->i2, data->path + data->pos, off, s, len,
  // s + off);
  if (data->tok != MJSON_TOK_INVALID) return 1;  // Found

  if (tok == '{') {
    if (!data->path[data->pos] && data->d1 == data->d2) data->obj = off;
    data->d1++;
  } else if (tok == '[') {
    if (data->d1 == data->d2 && data->path[data->pos] == '[') {
      data->i1 = 0;
      data->i2 = (int) strtod(&data->path[data->pos + 1], NULL);
      if (data->i1 == data->i2) {
        data->d2++;
        data->pos += 3;
      }
    }
    if (!data->path[data->pos] && data->d1 == data->d2) data->obj = off;
    data->d1++;
  } else if (tok == ',') {
    if (data->d1 == data->d2 + 1) {
      data->i1++;
      if (data->i1 == data->i2) {
        while (data->path[data->pos] != ']') data->pos++;
        data->pos++;
        data->d2++;
      }
    }
  } else if (tok == MJSON_TOK_KEY && data->d1 == data->d2 + 1 &&
             data->path[data->pos] == '.' && s[off] == '"' &&
             s[off + len - 1] == '"' &&
             mjson_plen(&data->path[data->pos + 1]) == len - 2 &&
             !memcmp(s + off + 1, &data->path[data->pos + 1], len - 2)) {
    data->d2++;
    data->pos += len - 1;
  } else if (tok == MJSON_TOK_KEY && data->d1 == data->d2) {
    return 1;  // Exhausted path, not found
  } else if (tok == '}' || tok == ']') {
    data->d1--;
    // data->d2--;
    if (!data->path[data->pos] && data->d1 == data->d2 && data->obj != -1) {
      data->tok = tok - 2;
      if (data->tokptr) *data->tokptr = s + data->obj;
      if (data->toklen) *data->toklen = off - data->obj + 1;
      return 1;
    }
  } else if (MJSON_TOK_IS_VALUE(tok)) {
    // printf("TOK --> %d\n", tok);
    if (data->d1 == data->d2 && !data->path[data->pos]) {
      data->tok = tok;
      if (data->tokptr) *data->tokptr = s + off;
      if (data->toklen) *data->toklen = len;
      return 1;
    }
  }
  return 0;
}

enum mjson_tok mjson_find(const char *s, int len, const char *jp,
                          const char **tokptr, int *toklen) {
  struct msjon_get_data data = {jp, 1,  0,      0,      0,
                                0,  -1, tokptr, toklen, MJSON_TOK_INVALID};
  if (jp[0] != '$') return MJSON_TOK_INVALID;
  if (mjson(s, len, mjson_get_cb, &data) < 0) return MJSON_TOK_INVALID;
  return (enum mjson_tok) data.tok;
}

int mjson_get_number(const char *s, int len, const char *path, double *v) {
  const char *p;
  int tok, n;
  if ((tok = mjson_find(s, len, path, &p, &n)) == MJSON_TOK_NUMBER) {
    if (v != NULL) *v = strtod(p, NULL);
  }
  return tok == MJSON_TOK_NUMBER ? 1 : 0;
}

int mjson_get_bool(const char *s, int len, const char *path, int *v) {
  int tok = mjson_find(s, len, path, NULL, NULL);
  if (tok == MJSON_TOK_TRUE && v != NULL) *v = 1;
  if (tok == MJSON_TOK_FALSE && v != NULL) *v = 0;
  return tok == MJSON_TOK_TRUE || tok == MJSON_TOK_FALSE ? 1 : 0;
}

static int mjson_unescape(const char *s, int len, char *to, int n) {
  int i, j;
  for (i = 0, j = 0; i < len && j < n; i++, j++) {
    if (s[i] == '\\' && i + 1 < len) {
      int c = mjson_esc(s[i + 1], 0);
      if (c == 0) return -1;
      to[j] = c;
      i++;
    } else {
      to[j] = s[i];
    }
  }
  if (j >= n) return -1;
  if (n > 0) to[j] = '\0';
  return j;
}

int mjson_get_string(const char *s, int len, const char *path, char *to,
                     int n) {
  const char *p;
  int sz;
  if (mjson_find(s, len, path, &p, &sz) != MJSON_TOK_STRING) return -1;
  return mjson_unescape(p + 1, sz - 2, to, n);
}

int mjson_get_hex(const char *s, int len, const char *x, char *to, int n) {
  const char *p;
  int i, j, sz;
  if (mjson_find(s, len, x, &p, &sz) != MJSON_TOK_STRING) return -1;
  for (i = j = 0; i < sz - 3 && j < n; i += 2, j++) {
#define HEXTOI(x) (x >= '0' && x <= '9' ? x - '0' : x - 'W')
    unsigned char a = *(const unsigned char *) (p + i + 1);
    unsigned char b = *(const unsigned char *) (p + i + 2);
    ((unsigned char *) to)[j] = (HEXTOI(a) << 4) | HEXTOI(b);
  }
  if (j < n) to[j] = '\0';
  return j;
}

#if MJSON_ENABLE_BASE64
static int mjson_base64rev(int c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A';
  } else if (c >= 'a' && c <= 'z') {
    return c + 26 - 'a';
  } else if (c >= '0' && c <= '9') {
    return c + 52 - '0';
  } else if (c == '+') {
    return 62;
  } else if (c == '/') {
    return 63;
  } else {
    return 64;
  }
}

int mjson_base64_dec(const char *src, int n, char *dst, int dlen) {
  const char *end = src + n;
  int len = 0;
  while (src + 3 < end && len < dlen) {
    int a = mjson_base64rev(src[0]), b = mjson_base64rev(src[1]),
        c = mjson_base64rev(src[2]), d = mjson_base64rev(src[3]);
    dst[len++] = (a << 2) | (b >> 4);
    if (src[2] != '=' && len < dlen) {
      dst[len++] = (b << 4) | (c >> 2);
      if (src[3] != '=' && len < dlen) {
        dst[len++] = (c << 6) | d;
      }
    }
    src += 4;
  }
  if (len < dlen) dst[len] = '\0';
  return len;
}

int mjson_get_base64(const char *s, int len, const char *path, char *to,
                     int n) {
  const char *p;
  int sz;
  if (mjson_find(s, len, path, &p, &sz) != MJSON_TOK_STRING) return 0;
  return mjson_base64_dec(p + 1, sz - 2, to, n);
}
#endif  // MJSON_ENABLE_BASE64

#if MJSON_ENABLE_NEXT
struct nextdata {
  int off, len, depth, t, vo, arrayindex;
  int *koff, *klen, *voff, *vlen, *vtype;
};

static int next_cb(int tok, const char *s, int off, int len, void *ud) {
  struct nextdata *d = (struct nextdata *) ud;
  // int i;
  switch (tok) {
    case '{':
    case '[':
      if (d->depth == 0 && tok == '[') d->arrayindex = 0;
      if (d->depth == 1 && off > d->off) {
        d->vo = off;
        d->t = tok == '{' ? MJSON_TOK_OBJECT : MJSON_TOK_ARRAY;
        if (d->voff) *d->voff = off;
        if (d->vtype) *d->vtype = d->t;
      }
      d->depth++;
      break;
    case '}':
    case ']':
      d->depth--;
      if (d->depth == 1 && d->vo) {
        d->len = off + len;
        if (d->vlen) *d->vlen = d->len - d->vo;
        if (d->arrayindex >= 0) {
          if (d->koff) *d->koff = d->arrayindex;  // koff holds array index
          if (d->klen) *d->klen = 0;              // klen holds 0
        }
        return 1;
      }
      if (d->depth == 1 && d->arrayindex >= 0) d->arrayindex++;
      break;
    case ',':
    case ':':
      break;
    case MJSON_TOK_KEY:
      if (d->depth == 1 && d->off < off) {
        if (d->koff) *d->koff = off;  // And report back to the user
        if (d->klen) *d->klen = len;  // If we have to
      }
      break;
    default:
      if (d->depth != 1) break;
      // If we're iterating over the array
      if (off > d->off) {
        d->len = off + len;
        if (d->vlen) *d->vlen = len;    // value length
        if (d->voff) *d->voff = off;    // value offset
        if (d->vtype) *d->vtype = tok;  // value type
        if (d->arrayindex >= 0) {
          if (d->koff) *d->koff = d->arrayindex;  // koff holds array index
          if (d->klen) *d->klen = 0;              // klen holds 0
        }
        return 1;
      }
      if (d->arrayindex >= 0) d->arrayindex++;
      break;
  }
  (void) s;
  return 0;
}

int mjson_next(const char *s, int n, int off, int *koff, int *klen, int *voff,
               int *vlen, int *vtype) {
  struct nextdata d = {off, 0, 0, 0, 0, -1, koff, klen, voff, vlen, vtype};
  mjson(s, n, next_cb, &d);
  return d.len;
}
#endif

#if MJSON_ENABLE_PRINT
int mjson_print_fixed_buf(const char *ptr, int len, void *fndata) {
  struct mjson_fixedbuf *fb = (struct mjson_fixedbuf *) fndata;
  int i, left = fb->size - 1 - fb->len;
  if (left < len) len = left;
  for (i = 0; i < len; i++) fb->ptr[fb->len + i] = ptr[i];
  fb->len += len;
  fb->ptr[fb->len] = '\0';
  return len;
}

int mjson_print_dynamic_buf(const char *ptr, int len, void *fndata) {
  char *s, *buf = *(char **) fndata;
  int curlen = buf == NULL ? 0 : strlen(buf);
  if ((s = (char *) realloc(buf, curlen + len + 1)) == NULL) {
    return 0;
  } else {
    memcpy(s + curlen, ptr, len);
    s[curlen + len] = '\0';
    *(char **) fndata = s;
    return len;
  }
}

int mjson_print_null(const char *ptr, int len, void *userdata) {
  (void) ptr;
  (void) userdata;
  return len;
}

int mjson_print_file(const char *ptr, int len, void *userdata) {
  return fwrite(ptr, 1, len, (FILE *) userdata);
}

int mjson_print_buf(mjson_print_fn_t fn, void *fndata, const char *buf,
                    int len) {
  return fn(buf, len, fndata);
}

int mjson_print_int(mjson_print_fn_t fn, void *fndata, int value,
                    int is_signed) {
  char buf[20];
  int len = snprintf(buf, sizeof(buf), is_signed ? "%d" : "%u", value);
  return fn(buf, len, fndata);
}

int mjson_print_long(mjson_print_fn_t fn, void *fndata, long value,
                     int is_signed) {
  char buf[20];
  const char *fmt = (is_signed ? "%ld" : "%lu");
  int len = snprintf(buf, sizeof(buf), fmt, value);
  return fn(buf, len, fndata);
}

int mjson_print_dbl(mjson_print_fn_t fn, void *fndata, double d,
                    const char *fmt) {
  char buf[40];
  int n = snprintf(buf, sizeof(buf), fmt, d);
  return fn(buf, n, fndata);
}

int mjson_print_str(mjson_print_fn_t fn, void *fndata, const char *s, int len) {
  int i, n = fn("\"", 1, fndata);
  for (i = 0; i < len; i++) {
    char c = mjson_esc(s[i], 1);
    if (c) {
      n += fn("\\", 1, fndata);
      n += fn(&c, 1, fndata);
    } else {
      n += fn(&s[i], 1, fndata);
    }
  }
  return n + fn("\"", 1, fndata);
}

#if MJSON_ENABLE_BASE64
int mjson_print_b64(mjson_print_fn_t fn, void *fndata, const unsigned char *s,
                    int n) {
  const char *t =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i, len = fn("\"", 1, fndata);
  for (i = 0; i < n; i += 3) {
    int a = s[i], b = i + 1 < n ? s[i + 1] : 0, c = i + 2 < n ? s[i + 2] : 0;
    char buf[4] = {t[a >> 2], t[(a & 3) << 4 | (b >> 4)], '=', '='};
    if (i + 1 < n) buf[2] = t[(b & 15) << 2 | (c >> 6)];
    if (i + 2 < n) buf[3] = t[c & 63];
    len += fn(buf, sizeof(buf), fndata);
  }
  return len + fn("\"", 1, fndata);
}
#endif /* MJSON_ENABLE_BASE64 */

int mjson_vprintf(mjson_print_fn_t fn, void *fndata, const char *fmt,
                  va_list xap) {
  int i = 0, n = 0;
  va_list ap;
  va_copy(ap, xap);
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      char fc = fmt[++i];
      int is_long = 0;
      if (fc == 'l') {
        is_long = 1;
        fc = fmt[i + 1];
      }
      if (fc == 'Q') {
        char *buf = va_arg(ap, char *);
        n += mjson_print_str(fn, fndata, buf ? buf : "", buf ? strlen(buf) : 0);
      } else if (strncmp(&fmt[i], ".*Q", 3) == 0) {
        int len = va_arg(ap, int);
        char *buf = va_arg(ap, char *);
        n += mjson_print_str(fn, fndata, buf, len);
        i += 2;
      } else if (fc == 'd' || fc == 'u') {
        int is_signed = (fc == 'd');
        if (is_long) {
          long val = va_arg(ap, long);
          n += mjson_print_long(fn, fndata, val, is_signed);
          i++;
        } else {
          int val = va_arg(ap, int);
          n += mjson_print_int(fn, fndata, val, is_signed);
        }
      } else if (fc == 'B') {
        const char *s = va_arg(ap, int) ? "true" : "false";
        n += mjson_print_buf(fn, fndata, s, strlen(s));
      } else if (fc == 's') {
        char *buf = va_arg(ap, char *);
        n += mjson_print_buf(fn, fndata, buf, strlen(buf));
      } else if (strncmp(&fmt[i], ".*s", 3) == 0) {
        int len = va_arg(ap, int);
        char *buf = va_arg(ap, char *);
        n += mjson_print_buf(fn, fndata, buf, len);
        i += 2;
      } else if (fc == 'g') {
        n += mjson_print_dbl(fn, fndata, va_arg(ap, double), "%g");
      } else if (fc == 'f') {
        n += mjson_print_dbl(fn, fndata, va_arg(ap, double), "%f");
#if MJSON_ENABLE_BASE64
      } else if (fc == 'V') {
        int len = va_arg(ap, int);
        const char *buf = va_arg(ap, const char *);
        n += mjson_print_b64(fn, fndata, (unsigned char *) buf, len);
#endif
      } else if (fc == 'H') {
        const char *hex = "0123456789abcdef";
        int i, len = va_arg(ap, int);
        const unsigned char *p = va_arg(ap, const unsigned char *);
        n += fn("\"", 1, fndata);
        for (i = 0; i < len; i++) {
          n += fn(&hex[(p[i] >> 4) & 15], 1, fndata);
          n += fn(&hex[p[i] & 15], 1, fndata);
        }
        n += fn("\"", 1, fndata);
      } else if (fc == 'M') {
        mjson_vprint_fn_t vfn = va_arg(ap, mjson_vprint_fn_t);
        n += vfn(fn, fndata, &ap);
      }
      i++;
    } else {
      n += mjson_print_buf(fn, fndata, &fmt[i++], 1);
    }
  }
  va_end(xap);
  va_end(ap);
  return n;
}

int mjson_printf(mjson_print_fn_t fn, void *fndata, const char *fmt, ...) {
  va_list ap;
  int len;
  va_start(ap, fmt);
  len = mjson_vprintf(fn, fndata, fmt, ap);
  va_end(ap);
  return len;
}
#endif /* MJSON_ENABLE_PRINT */

#if MJSON_IMPLEMENT_STRTOD
static inline int is_digit(int c) {
  return c >= '0' && c <= '9';
}

/* NOTE: strtod() implementation by Yasuhiro Matsumoto. */
double strtod(const char *str, char **end) {
  double d = 0.0;
  int sign = 1, n = 0;
  const char *p = str, *a = str;

  /* decimal part */
  if (*p == '-') {
    sign = -1;
    ++p;
  } else if (*p == '+')
    ++p;
  if (is_digit(*p)) {
    d = (double) (*p++ - '0');
    while (*p && is_digit(*p)) {
      d = d * 10.0 + (double) (*p - '0');
      ++p;
      ++n;
    }
    a = p;
  } else if (*p != '.')
    goto done;
  d *= sign;

  /* fraction part */
  if (*p == '.') {
    double f = 0.0;
    double base = 0.1;
    ++p;

    if (is_digit(*p)) {
      while (*p && is_digit(*p)) {
        f += base * (*p - '0');
        base /= 10.0;
        ++p;
        ++n;
      }
    }
    d += f * sign;
    a = p;
  }

  /* exponential part */
  if ((*p == 'E') || (*p == 'e')) {
    int e = 0;
    ++p;

    sign = 1;
    if (*p == '-') {
      sign = -1;
      ++p;
    } else if (*p == '+')
      ++p;

    if (is_digit(*p)) {
      while (*p == '0') ++p;
      e = (int) (*p++ - '0');
      while (*p && is_digit(*p)) {
        e = e * 10 + (int) (*p - '0');
        ++p;
      }
      e *= sign;
    } else if (!is_digit(*(a - 1))) {
      a = str;
      goto done;
    } else if (*p == 0)
      goto done;

    if (d == 2.2250738585072011 && e == -308) {
      d = 0.0;
      a = p;
      goto done;
    }
    if (d == 2.2250738585072012 && e <= -308) {
      d *= 1.0e-308;
      a = p;
      goto done;
    }
    {
      int i;
      for (i = 0; i < 10; i++) d *= 10;
    }
    a = p;
  } else if (p > str && !is_digit(*(p - 1))) {
    a = str;
    goto done;
  }

done:
  if (end) *end = (char *) a;
  return d;
}
#endif

#if MJSON_ENABLE_MERGE
int mjson_merge(const char *s, int n, const char *s2, int n2,
                mjson_print_fn_t fn, void *userdata) {
  int koff, klen, voff, vlen, t, t2, k, off = 0, len = 0, comma = 0;
  if (n < 2) return len;
  len += fn("{", 1, userdata);
  while ((off = mjson_next(s, n, off, &koff, &klen, &voff, &vlen, &t)) != 0) {
#if !defined(_MSC_VER) || _MSC_VER >= 1700
    char path[klen + 1];
#else
    char *path = (char *) alloca(klen + 1);
#endif
    const char *val;
    memcpy(path, "$.", 2);
    memcpy(path + 2, s + koff + 1, klen - 2);
    path[klen] = '\0';
    if ((t2 = mjson_find(s2, n2, path, &val, &k)) != MJSON_TOK_INVALID) {
      if (t2 == MJSON_TOK_NULL) continue;  // null deletes the key
    } else {
      val = s + voff;  // Key is not found in the update. Copy the old value.
    }
    if (comma) len += fn(",", 1, userdata);
    len += fn(s + koff, klen, userdata);
    len += fn(":", 1, userdata);
    if (t == MJSON_TOK_OBJECT && t2 == MJSON_TOK_OBJECT) {
      len += mjson_merge(s + voff, vlen, val, k, fn, userdata);
    } else {
      if (t2 != MJSON_TOK_INVALID) vlen = k;
      len += fn(val, vlen, userdata);
    }
    comma = 1;
  }
  // Add missing keys
  off = 0;
  while ((off = mjson_next(s2, n2, off, &koff, &klen, &voff, &vlen, &t)) != 0) {
#if !defined(_MSC_VER) || _MSC_VER >= 1700
    char path[klen + 1];
#else
    char *path = (char *) alloca(klen + 1);
#endif
    const char *val;
    if (t == MJSON_TOK_NULL) continue;
    memcpy(path, "$.", 2);
    memcpy(path + 2, s2 + koff + 1, klen - 2);
    path[klen] = '\0';
    if (mjson_find(s, n, path, &val, &vlen) != MJSON_TOK_INVALID) continue;
    if (comma) len += fn(",", 1, userdata);
    len += fn(s2 + koff, klen, userdata);
    len += fn(":", 1, userdata);
    len += fn(s2 + voff, vlen, userdata);
    comma = 1;
  }
  len += fn("}", 1, userdata);
  return len;
}
#endif  // MJSON_ENABLE_MERGE

#if MJSON_ENABLE_PRETTY
struct prettydata {
  int level;
  int len;
  int prev;
  const char *pad;
  int padlen;
  mjson_print_fn_t fn;
  void *userdata;
};

static int pretty_cb(int ev, const char *s, int off, int len, void *ud) {
  struct prettydata *d = (struct prettydata *) ud;
  int i;
  switch (ev) {
    case '{':
    case '[':
      d->level++;
      d->len += d->fn(s + off, len, d->userdata);
      break;
    case '}':
    case ']':
      d->level--;
      if (d->prev != '[' && d->prev != '{' && d->padlen > 0) {
        d->len += d->fn("\n", 1, d->userdata);
        for (i = 0; i < d->level; i++)
          d->len += d->fn(d->pad, d->padlen, d->userdata);
      }
      d->len += d->fn(s + off, len, d->userdata);
      break;
    case ',':
      d->len += d->fn(s + off, len, d->userdata);
      if (d->padlen > 0) {
        d->len += d->fn("\n", 1, d->userdata);
        for (i = 0; i < d->level; i++)
          d->len += d->fn(d->pad, d->padlen, d->userdata);
      }
      break;
    case ':':
      d->len += d->fn(s + off, len, d->userdata);
      if (d->padlen > 0) d->len += d->fn(" ", 1, d->userdata);
      break;
    case MJSON_TOK_KEY:
      if (d->prev == '{' && d->padlen > 0) {
        d->len += d->fn("\n", 1, d->userdata);
        for (i = 0; i < d->level; i++)
          d->len += d->fn(d->pad, d->padlen, d->userdata);
      }
      d->len += d->fn(s + off, len, d->userdata);
      break;
    default:
      if (d->prev == '[' && d->padlen > 0) {
        d->len += d->fn("\n", 1, d->userdata);
        for (i = 0; i < d->level; i++)
          d->len += d->fn(d->pad, d->padlen, d->userdata);
      }
      d->len += d->fn(s + off, len, d->userdata);
      break;
  }
  d->prev = ev;
  return 0;
}

int mjson_pretty(const char *s, int n, const char *pad, mjson_print_fn_t fn,
                 void *userdata) {
  struct prettydata d = {0, 0, 0, pad, strlen(pad), fn, userdata};
  if (mjson(s, n, pretty_cb, &d) < 0) return -1;
  return d.len;
}
#endif  // MJSON_ENABLE_PRETTY

#if MJSON_ENABLE_RPC
struct jsonrpc_ctx jsonrpc_default_context;
struct jsonrpc_userdata {
  mjson_print_fn_t fn;
  void *fndata;
};

int mjson_globmatch(const char *s1, int n1, const char *s2, int n2) {
  int i = 0, j = 0, ni = 0, nj = 0;
  while (i < n1 || j < n2) {
    if (i < n1 && j < n2 && (s1[i] == '?' || s2[j] == s1[i])) {
      i++, j++;
    } else if (i < n1 && (s1[i] == '*' || s1[i] == '#')) {
      ni = i, nj = j + 1, i++;
    } else if (nj > 0 && nj <= n2 && (s1[i - 1] == '#' || s2[j] != '/')) {
      i = ni, j = nj;
    } else {
      return 0;
    }
  }
  return 1;
}

static int jsonrpc_printer(const char *buf, int len, void *userdata) {
  struct jsonrpc_userdata *u = (struct jsonrpc_userdata *) userdata;
  return u->fn(buf, len, u->fndata);
}

void jsonrpc_return_errorv(struct jsonrpc_request *r, int code,
                           const char *message, const char *data_fmt,
                           va_list ap) {
  if (r->id_len == 0) return;
  mjson_printf(r->fn, r->fndata,
               "{\"id\":%.*s,\"error\":{\"code\":%d,\"message\":%Q", r->id_len,
               r->id, code, message == NULL ? "" : message);
  if (data_fmt != NULL) {
  	mjson_printf(r->fn, r->fndata, ",\"data\":");
    mjson_vprintf(r->fn, r->fndata, data_fmt, ap);
  }
  mjson_printf(r->fn, r->fndata, "}}\n");
}

void jsonrpc_return_error(struct jsonrpc_request *r, int code,
                          const char *message, const char *data_fmt, ...) {
  va_list ap;
  va_start(ap, data_fmt);
  jsonrpc_return_errorv(r, code, message, data_fmt, ap);
  va_end(ap);
}

void jsonrpc_return_successv(struct jsonrpc_request *r, const char *result_fmt,
                             va_list ap) {
  if (r->id_len == 0) return;
  mjson_printf(r->fn, r->fndata, "{\"id\":%.*s,\"result\":", r->id_len, r->id);
  if (result_fmt != NULL) {
    mjson_vprintf(r->fn, r->fndata, result_fmt, ap);
  } else {
    mjson_printf(r->fn, r->fndata, "%s", "null");
  }
  mjson_printf(r->fn, r->fndata, "}\n");
}

void jsonrpc_return_success(struct jsonrpc_request *r, const char *result_fmt,
                            ...) {
  va_list ap;
  va_start(ap, result_fmt);
  jsonrpc_return_successv(r, result_fmt, ap);
  va_end(ap);
}

void jsonrpc_ctx_process(struct jsonrpc_ctx *ctx, const char *req, int req_sz,
                         mjson_print_fn_t fn, void *fndata) {
  const char *result = NULL, *error = NULL;
  int result_sz = 0, error_sz = 0;
  struct jsonrpc_method *m = NULL;
  struct jsonrpc_userdata d;
  struct jsonrpc_request r = {req, req_sz,           0,    0,   0, 0, 0,
                              0,   &jsonrpc_printer, NULL, NULL};

  d.fn = fn;
  d.fndata = fndata;
  r.fndata = &d;

  // Is is a response frame?
  mjson_find(req, req_sz, "$.result", &result, &result_sz);
  if (result == NULL) mjson_find(req, req_sz, "$.error", &error, &error_sz);
  if (result_sz > 0 || error_sz > 0) {
    if (ctx->response_cb != NULL) ctx->response_cb(req, req_sz, ctx->userdata);
    return;
  }

  // Method must exist and must be a string
  if (mjson_find(req, req_sz, "$.method", &r.method, &r.method_len) !=
      MJSON_TOK_STRING) {
    mjson_printf(fn, fndata, "{\"error\":{\"code\":-32700,\"message\":%.*Q}}\n",
                 req_sz, req);
    ctx->in_len = 0;
    return;
  }

  // id and params are optional
  mjson_find(req, req_sz, "$.id", &r.id, &r.id_len);
  mjson_find(req, req_sz, "$.params", &r.params, &r.params_len);

  for (m = ctx->methods; m != NULL; m = m->next) {
    if (mjson_globmatch(m->method, m->method_sz, r.method + 1,
                        r.method_len - 2) > 0) {
      if (r.params == NULL) r.params = "";
      r.userdata = m->cbdata;
      m->cb(&r);
      break;
    }
  }
  if (m == NULL) {
    jsonrpc_return_error(&r, JSONRPC_ERROR_NOT_FOUND, "method not found", NULL);
  }
}

static int jsonrpc_print_methods(mjson_print_fn_t fn, void *fndata,
                                 va_list *ap) {
  struct jsonrpc_ctx *ctx = va_arg(*ap, struct jsonrpc_ctx *);
  struct jsonrpc_method *m;
  int len = 0;
  for (m = ctx->methods; m != NULL; m = m->next) {
    if (m != ctx->methods) len += mjson_print_buf(fn, fndata, ",", 1);
    len += mjson_print_str(fn, fndata, m->method, strlen(m->method));
  }
  return len;
}

static void rpclist(struct jsonrpc_request *r) {
  jsonrpc_return_success(r, "[%M]", jsonrpc_print_methods, r->userdata);
}

void jsonrpc_ctx_init(struct jsonrpc_ctx *ctx, mjson_print_fn_t response_cb,
                      void *userdata) {
  ctx->response_cb = response_cb;
  ctx->userdata = userdata;
  jsonrpc_ctx_export(ctx, MJSON_RPC_LIST_NAME, rpclist, ctx);
}

void jsonrpc_ctx_process_byte(struct jsonrpc_ctx *ctx, unsigned char ch,
                              mjson_print_fn_t fn, void *p) {
  if (ctx->in_len >= (int) sizeof(ctx->in)) ctx->in_len = 0;  // Overflow
  if (ch == '\n') {  // If new line, parse frame
    if (ctx->in_len > 1) jsonrpc_ctx_process(ctx, ctx->in, ctx->in_len, fn, p);
    ctx->in_len = 0;
  } else {
    ctx->in[ctx->in_len] = ch;  // Append to the buffer
    ctx->in_len++;
  }
}

void jsonrpc_init(mjson_print_fn_t response_cb, void *userdata) {
  jsonrpc_ctx_init(&jsonrpc_default_context, response_cb, userdata);
}
#endif  // MJSON_ENABLE_RPC
