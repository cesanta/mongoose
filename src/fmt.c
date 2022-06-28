#include "iobuf.h"
#include "str.h"

size_t mg_rprintf(void (*out)(char, void *), void *ptr, const char *fmt, ...) {
  size_t len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vrprintf(out, ptr, fmt, &ap);
  va_end(ap);
  return len;
}

static void mg_putchar_iobuf(char ch, void *param) {
  struct mg_iobuf *io = (struct mg_iobuf *) param;
  if (io->len < io->size) io->buf[io->len++] = (uint8_t) ch;
}

void mg_putchar_realloc(char ch, void *param) {
  char *s, *buf = *(char **) param;
  size_t len = buf == NULL ? 0 : strlen(buf), chunksize = 256;
  size_t new_size = len + 1 + 1 + chunksize;
  new_size -= new_size % chunksize;
  if ((s = (char *) realloc(buf, new_size)) != NULL) {
    s[len] = ch;
    s[len + 1] = '\0';
    *(char **) param = s;
  }
}

size_t mg_vsnprintf(char *buf, size_t len, const char *fmt, va_list *ap) {
  struct mg_iobuf io = {(uint8_t *) buf, len, 0};
  size_t n = mg_vrprintf(mg_putchar_iobuf, &io, fmt, ap);
  if (n < len) buf[n] = '\0';
  return n;
}

static bool is_digit(int c) {
  return c >= '0' && c <= '9';
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

static size_t scpy(void (*out)(char, void *), void *ptr, char *buf,
                   size_t len) {
  size_t i = 0;
  while (i < len && buf[i] != '\0') out(buf[i++], ptr);
  return i;
}

static char mg_esc(int c, bool esc) {
  const char *p, *esc1 = "\b\f\n\r\t\\\"", *esc2 = "bfnrt\\\"";
  for (p = esc ? esc1 : esc2; *p != '\0'; p++) {
    if (*p == c) return esc ? esc2[p - esc1] : esc1[p - esc2];
  }
  return 0;
}

static char mg_escape(int c) {
  return mg_esc(c, true);
}

static size_t qcpy(void (*out)(char, void *), void *ptr, char *buf,
                   size_t len) {
  size_t i = 0, extra = 2;
  out('"', ptr);
  for (i = 0; i < len && buf[i] != '\0'; i++) {
    char c = mg_escape(buf[i]);
    if (c) {
      out('\\', ptr), out(c, ptr), extra++;
    } else {
      out(buf[i], ptr);
    }
  }
  out('"', ptr);
  return i + extra;
}

static size_t bcpy(void (*out)(char, void *), void *ptr, uint8_t *buf,
                   size_t len) {
  size_t i, n = 0;
  const char *t =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  out('"', ptr), n++;
  for (i = 0; i < len; i += 3) {
    uint8_t c1 = buf[i], c2 = i + 1 < len ? buf[i + 1] : 0,
            c3 = i + 2 < len ? buf[i + 2] : 0;
    char tmp[4] = {t[c1 >> 2], t[(c1 & 3) << 4 | (c2 >> 4)], '=', '='};
    if (i + 1 < len) tmp[2] = t[(c2 & 15) << 2 | (c3 >> 6)];
    if (i + 2 < len) tmp[3] = t[c3 & 63];
    n += scpy(out, ptr, tmp, sizeof(tmp));
  }
  out('"', ptr), n++;
  return n;
}

size_t mg_vrprintf(void (*out)(char, void *), void *param, const char *fmt,
                   va_list *ap) {
  size_t i = 0, n = 0;
  while (fmt[i] != '\0') {
    if (fmt[i] == '%') {
      size_t j, k, x = 0, is_long = 0, w = 0 /* width */, pr = ~0U /* prec */;
      char pad = ' ', minus = 0, c = fmt[++i];
      if (c == '#') x++, c = fmt[++i];
      if (c == '-') minus++, c = fmt[++i];
      if (c == '0') pad = '0', c = fmt[++i];
      while (is_digit(c)) w *= 10, w += (size_t) (c - '0'), c = fmt[++i];
      if (c == '.') {
        c = fmt[++i];
        if (c == '*') {
          pr = (size_t) va_arg(*ap, int);
          c = fmt[++i];
        } else {
          pr = 0;
          while (is_digit(c)) pr *= 10, pr += (size_t) (c - '0'), c = fmt[++i];
        }
      }
      while (c == 'h') c = fmt[++i];  // Treat h and hh as int
      if (c == 'l') {
        is_long++, c = fmt[++i];
        if (c == 'l') is_long++, c = fmt[++i];
      }
      if (c == 'p') x = 1, is_long = 1;
      if (c == 'd' || c == 'u' || c == 'x' || c == 'X' || c == 'p' ||
          c == 'g') {
        bool s = (c == 'd'), h = (c == 'x' || c == 'X' || c == 'p');
        char tmp[40];
        size_t xl = x ? 2 : 0;
        if (c == 'g' || c == 'f') {
          double v = va_arg(*ap, double);
          if (pr == ~0U) pr = 6;
          k = mg_dtoa(tmp, sizeof(tmp), v, (int) pr);
        } else if (is_long == 2) {
          int64_t v = va_arg(*ap, int64_t);
          k = mg_lld(tmp, v, s, h);
        } else if (is_long == 1) {
          long v = va_arg(*ap, long);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned long) v, s, h);
        } else {
          int v = va_arg(*ap, int);
          k = mg_lld(tmp, s ? (int64_t) v : (int64_t) (unsigned) v, s, h);
        }
        for (j = 0; j < xl && w > 0; j++) w--;
        for (j = 0; pad == ' ' && !minus && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
        n += scpy(out, param, (char *) "0x", xl);
        for (j = 0; pad == '0' && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
        n += scpy(out, param, tmp, k);
        for (j = 0; pad == ' ' && minus && k < w && j + k < w; j++)
          n += scpy(out, param, &pad, 1);
      } else if (c == 'M') {
        mg_pm_t f = va_arg(*ap, mg_pm_t);
        n += f(out, param, ap);
      } else if (c == 'c') {
        int ch = va_arg(*ap, int);
        out((char) ch, param);
        n++;
      } else if (c == 'H') {
        // Print hex-encoded double-quoted string
        size_t bl = (size_t) va_arg(*ap, int);
        uint8_t *p = va_arg(*ap, uint8_t *), dquote = '"';
        const char *hex = "0123456789abcdef";
        n += scpy(out, param, (char *) &dquote, 1);
        for (j = 0; j < bl; j++) {
          n += scpy(out, param, (char *) &hex[(p[j] >> 4) & 15], 1);
          n += scpy(out, param, (char *) &hex[p[j] & 15], 1);
        }
        n += scpy(out, param, (char *) &dquote, 1);
      } else if (c == 'V') {
        // Print base64-encoded double-quoted string
        size_t len = (size_t) va_arg(*ap, int);
        uint8_t *buf = va_arg(*ap, uint8_t *);
        n += bcpy(out, param, buf, len);
      } else if (c == 's' || c == 'Q') {
        char *p = va_arg(*ap, char *);
        size_t (*f)(void (*)(char, void *), void *, char *, size_t) =
            c == 's' ? scpy : qcpy;
        if (pr == ~0U) pr = p == NULL ? 0 : strlen(p);
        for (j = 0; !minus && pr < w && j + pr < w; j++)
          n += f(out, param, &pad, 1);
        n += f(out, param, p, pr);
        for (j = 0; minus && pr < w && j + pr < w; j++)
          n += f(out, param, &pad, 1);
      } else if (c == '%') {
        out('%', param);
        n++;
      } else {
        out('%', param);
        out(c, param);
        n += 2;
      }
      i++;
    } else {
      out(fmt[i], param), n++, i++;
    }
  }
  return n;
}
