#include "base64.h"
#include "fmt.h"
#include "json.h"

static const char *escapeseq(int esc) {
  return esc ? "\b\f\n\r\t\\\"" : "bfnrt\\\"";
}

static char json_esc(int c, int esc) {
  const char *p, *esc1 = escapeseq(esc), *esc2 = escapeseq(!esc);
  for (p = esc1; *p != '\0'; p++) {
    if (*p == c) return esc2[p - esc1];
  }
  return 0;
}

static int mg_pass_string(const char *s, int len) {
  int i;
  for (i = 0; i < len; i++) {
    if (s[i] == '\\' && i + 1 < len && json_esc(s[i + 1], 1)) {
      i++;
    } else if (s[i] == '\0') {
      return MG_JSON_INVALID;
    } else if (s[i] == '"') {
      return i;
    }
  }
  return MG_JSON_INVALID;
}

static double mg_atod(const char *p, int len, int *numlen) {
  double d = 0.0;
  int i = 0, sign = 1;

  // Sign
  if (i < len && *p == '-') {
    sign = -1, i++;
  } else if (i < len && *p == '+') {
    i++;
  }

  // Decimal
  for (; i < len && p[i] >= '0' && p[i] <= '9'; i++) {
    d *= 10.0;
    d += p[i] - '0';
  }
  d *= sign;

  // Fractional
  if (i < len && p[i] == '.') {
    double frac = 0.0, base = 0.1;
    i++;
    for (; i < len && p[i] >= '0' && p[i] <= '9'; i++) {
      frac += base * (p[i] - '0');
      base /= 10.0;
    }
    d += frac * sign;
  }

  // Exponential
  if (i < len && (p[i] == 'e' || p[i] == 'E')) {
    int j, exp = 0, minus = 0;
    i++;
    if (i < len && p[i] == '-') minus = 1, i++;
    if (i < len && p[i] == '+') i++;
    while (i < len && p[i] >= '0' && p[i] <= '9' && exp < 308)
      exp = exp * 10 + (p[i++] - '0');
    if (minus) exp = -exp;
    for (j = 0; j < exp; j++) d *= 10.0;
    for (j = 0; j < -exp; j++) d /= 10.0;
  }

  if (numlen != NULL) *numlen = i;
  return d;
}

// Iterate over object or array elements
size_t mg_json_next(struct mg_str obj, size_t ofs, struct mg_str *key,
                    struct mg_str *val) {
  if (ofs >= obj.len) {
    ofs = 0;  // Out of boundaries, stop scanning
  } else if (obj.len < 2 || (*obj.ptr != '{' && *obj.ptr != '[')) {
    ofs = 0;  // Not an array or object, stop
  } else {
    struct mg_str sub = mg_str_n(obj.ptr + ofs, obj.len - ofs);
    if (ofs == 0) ofs++, sub.ptr++, sub.len--;
    if (*obj.ptr == '[') {  // Iterate over an array
      int n = 0, o = mg_json_get(sub, "$", &n);
      if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
        ofs = 0;  // Error parsing key, stop scanning
      } else {
        if (key) *key = mg_str_n(NULL, 0);
        if (val) *val = mg_str_n(sub.ptr + o, (size_t) n);
        ofs = (size_t) (&sub.ptr[o + n] - obj.ptr);
      }
    } else {  // Iterate over an object
      int n = 0, o = mg_json_get(sub, "$", &n);
      if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
        ofs = 0;  // Error parsing key, stop scanning
      } else {
        if (key) *key = mg_str_n(sub.ptr + o, (size_t) n);
        sub.ptr += o + n, sub.len -= (size_t) (o + n);
        while (sub.len > 0 && *sub.ptr != ':') sub.len--, sub.ptr++;
        if (sub.len > 0 && *sub.ptr == ':') sub.len--, sub.ptr++;
        n = 0, o = mg_json_get(sub, "$", &n);
        if (n < 0 || o < 0 || (size_t) (o + n) > sub.len) {
          ofs = 0;  // Error parsing value, stop scanning
        } else {
          if (val) *val = mg_str_n(sub.ptr + o, (size_t) n);
          ofs = (size_t) (&sub.ptr[o + n] - obj.ptr);
        }
      }
    }
    //MG_INFO(("SUB ofs %u %.*s", ofs, sub.len, sub.ptr));
    while (ofs && ofs < obj.len &&
           (obj.ptr[ofs] == ' ' || obj.ptr[ofs] == '\t' ||
            obj.ptr[ofs] == '\n' || obj.ptr[ofs] == '\r')) {
      ofs++;
    }
    if (ofs && ofs < obj.len && obj.ptr[ofs] == ',') ofs++;
    if (ofs > obj.len) ofs = 0;
  }
  return ofs;
}

int mg_json_get(struct mg_str json, const char *path, int *toklen) {
  const char *s = json.ptr;
  int len = (int) json.len;
  enum { S_VALUE, S_KEY, S_COLON, S_COMMA_OR_EOO } expecting = S_VALUE;
  unsigned char nesting[MG_JSON_MAX_DEPTH];
  int i = 0;             // Current offset in `s`
  int j = 0;             // Offset in `s` we're looking for (return value)
  int depth = 0;         // Current depth (nesting level)
  int ed = 0;            // Expected depth
  int pos = 1;           // Current position in `path`
  int ci = -1, ei = -1;  // Current and expected index in array

  if (toklen) *toklen = 0;
  if (path[0] != '$') return MG_JSON_INVALID;

#define MG_CHECKRET(x)                                  \
  do {                                                  \
    if (depth == ed && path[pos] == '\0' && ci == ei) { \
      if (toklen) *toklen = i - j + 1;                  \
      return j;                                         \
    }                                                   \
  } while (0)

// In the ascii table, the distance between `[` and `]` is 2.
// Ditto for `{` and `}`. Hence +2 in the code below.
#define MG_EOO(x)                                            \
  do {                                                       \
    if (depth == ed && ci != ei) return MG_JSON_NOT_FOUND;   \
    if (c != nesting[depth - 1] + 2) return MG_JSON_INVALID; \
    depth--;                                                 \
    MG_CHECKRET(x);                                          \
  } while (0)

  for (i = 0; i < len; i++) {
    unsigned char c = ((unsigned char *) s)[i];
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r') continue;
    switch (expecting) {
      case S_VALUE:
        // p("V %s [%.*s] %d %d %d %d\n", path, pos, path, depth, ed, ci, ei);
        if (depth == ed) j = i;
        if (c == '{') {
          if (depth >= (int) sizeof(nesting)) return MG_JSON_TOO_DEEP;
          if (depth == ed && path[pos] == '.' && ci == ei) {
            // If we start the object, reset array indices
            ed++, pos++, ci = ei = -1;
          }
          nesting[depth++] = c;
          expecting = S_KEY;
          break;
        } else if (c == '[') {
          if (depth >= (int) sizeof(nesting)) return MG_JSON_TOO_DEEP;
          if (depth == ed && path[pos] == '[' && ei == ci) {
            ed++, pos++, ci = 0;
            for (ei = 0; path[pos] != ']' && path[pos] != '\0'; pos++) {
              ei *= 10;
              ei += path[pos] - '0';
            }
            if (path[pos] != 0) pos++;
          }
          nesting[depth++] = c;
          break;
        } else if (c == ']' && depth > 0) {  // Empty array
          MG_EOO(']');
        } else if (c == 't' && i + 3 < len && memcmp(&s[i], "true", 4) == 0) {
          i += 3;
        } else if (c == 'n' && i + 3 < len && memcmp(&s[i], "null", 4) == 0) {
          i += 3;
        } else if (c == 'f' && i + 4 < len && memcmp(&s[i], "false", 5) == 0) {
          i += 4;
        } else if (c == '-' || ((c >= '0' && c <= '9'))) {
          int numlen = 0;
          mg_atod(&s[i], len - i, &numlen);
          i += numlen - 1;
        } else if (c == '"') {
          int n = mg_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          i += n + 1;
        } else {
          return MG_JSON_INVALID;
        }
        MG_CHECKRET('V');
        if (depth == ed && ei >= 0) ci++;
        expecting = S_COMMA_OR_EOO;
        break;

      case S_KEY:
        if (c == '"') {
          int n = mg_pass_string(&s[i + 1], len - i - 1);
          if (n < 0) return n;
          if (i + 1 + n >= len) return MG_JSON_NOT_FOUND;
          if (depth < ed) return MG_JSON_NOT_FOUND;
          if (depth == ed && path[pos - 1] != '.') return MG_JSON_NOT_FOUND;
          // printf("K %s [%.*s] [%.*s] %d %d %d %d %d\n", path, pos, path, n,
          //        &s[i + 1], n, depth, ed, ci, ei);
          //  NOTE(cpq): in the check sequence below is important.
          //  strncmp() must go first: it fails fast if the remaining length
          //  of the path is smaller than `n`.
          if (depth == ed && path[pos - 1] == '.' &&
              strncmp(&s[i + 1], &path[pos], (size_t) n) == 0 &&
              (path[pos + n] == '\0' || path[pos + n] == '.' ||
               path[pos + n] == '[')) {
            pos += n;
          }
          i += n + 1;
          expecting = S_COLON;
        } else if (c == '}') {  // Empty object
          MG_EOO('}');
          expecting = S_COMMA_OR_EOO;
          if (depth == ed && ei >= 0) ci++;
        } else {
          return MG_JSON_INVALID;
        }
        break;

      case S_COLON:
        if (c == ':') {
          expecting = S_VALUE;
        } else {
          return MG_JSON_INVALID;
        }
        break;

      case S_COMMA_OR_EOO:
        if (depth <= 0) {
          return MG_JSON_INVALID;
        } else if (c == ',') {
          expecting = (nesting[depth - 1] == '{') ? S_KEY : S_VALUE;
        } else if (c == ']' || c == '}') {
          if (depth == ed && c == '}' && path[pos - 1] == '.')
            return MG_JSON_NOT_FOUND;
          if (depth == ed && c == ']' && path[pos - 1] == ',')
            return MG_JSON_NOT_FOUND;
          MG_EOO('O');
          if (depth == ed && ei >= 0) ci++;
        } else {
          return MG_JSON_INVALID;
        }
        break;
    }
  }
  return MG_JSON_NOT_FOUND;
}

bool mg_json_get_num(struct mg_str json, const char *path, double *v) {
  int n, toklen, found = 0;
  if ((n = mg_json_get(json, path, &toklen)) >= 0 &&
      (json.ptr[n] == '-' || (json.ptr[n] >= '0' && json.ptr[n] <= '9'))) {
    if (v != NULL) *v = mg_atod(json.ptr + n, toklen, NULL);
    found = 1;
  }
  return found;
}

bool mg_json_get_bool(struct mg_str json, const char *path, bool *v) {
  int found = 0, off = mg_json_get(json, path, NULL);
  if (off >= 0 && (json.ptr[off] == 't' || json.ptr[off] == 'f')) {
    if (v != NULL) *v = json.ptr[off] == 't';
    found = 1;
  }
  return found;
}

bool mg_json_unescape(struct mg_str s, char *to, size_t n) {
  size_t i, j;
  for (i = 0, j = 0; i < s.len && j < n; i++, j++) {
    if (s.ptr[i] == '\\' && i + 5 < s.len && s.ptr[i + 1] == 'u') {
      //  \uXXXX escape. We could process a simple one-byte chars
      // \u00xx from the ASCII range. More complex chars would require
      // dragging in a UTF8 library, which is too much for us
      if (s.ptr[i + 2] != '0' || s.ptr[i + 3] != '0') return false;  // Give up
      ((unsigned char *) to)[j] = (unsigned char) mg_unhexn(s.ptr + i + 4, 2);

      i += 5;
    } else if (s.ptr[i] == '\\' && i + 1 < s.len) {
      char c = json_esc(s.ptr[i + 1], 0);
      if (c == 0) return false;
      to[j] = c;
      i++;
    } else {
      to[j] = s.ptr[i];
    }
  }
  if (j >= n) return false;
  if (n > 0) to[j] = '\0';
  return true;
}

char *mg_json_get_str(struct mg_str json, const char *path) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && len > 1 && json.ptr[off] == '"') {
    if ((result = (char *) calloc(1, (size_t) len)) != NULL &&
        !mg_json_unescape(mg_str_n(json.ptr + off + 1, (size_t) (len - 2)),
                          result, (size_t) len)) {
      free(result);
      result = NULL;
    }
  }
  return result;
}

char *mg_json_get_b64(struct mg_str json, const char *path, int *slen) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && json.ptr[off] == '"' && len > 1 &&
      (result = (char *) calloc(1, (size_t) len)) != NULL) {
    size_t k = mg_base64_decode(json.ptr + off + 1, (size_t) (len - 2), result,
                                (size_t) len);
    if (slen != NULL) *slen = (int) k;
  }
  return result;
}

char *mg_json_get_hex(struct mg_str json, const char *path, int *slen) {
  char *result = NULL;
  int len = 0, off = mg_json_get(json, path, &len);
  if (off >= 0 && json.ptr[off] == '"' && len > 1 &&
      (result = (char *) calloc(1, (size_t) len / 2)) != NULL) {
    mg_unhex(json.ptr + off + 1, (size_t) (len - 2), (uint8_t *) result);
    result[len / 2 - 1] = '\0';
    if (slen != NULL) *slen = len / 2 - 1;
  }
  return result;
}

long mg_json_get_long(struct mg_str json, const char *path, long dflt) {
  double dv;
  long result = dflt;
  if (mg_json_get_num(json, path, &dv)) result = (long) dv;
  return result;
}
