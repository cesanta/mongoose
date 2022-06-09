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

int mg_json_get(const char *s, int len, const char *path, int *toklen) {
  enum { S_VALUE, S_KEY, S_COLON, S_COMMA_OR_EOO } expecting = S_VALUE;
  unsigned char nesting[MG_JSON_MAX_DEPTH];
  int i, j = 0, depth = 0;
  int pos = 1;           // Current position in path
  int ed = 0;            // Expected depth
  int ci = -1, ei = -1;  // Current and expected index in array

  if (path[0] != '$') return MG_JSON_INVALID;

#if 0
#define MG_DBGP(x)                                                             \
  do {                                                                         \
    printf("%c %.*s j=%d i=%d pos=%d depth=%d ed=%d ci=%d ei=%d\n", x, len, s, \
           j, i, pos, depth, ed, ci, ei);                                      \
  } while (0)
#else
#define MG_DBGP(x)
#endif

#define MG_CHECKRET(x)                                  \
  do {                                                  \
    MG_DBGP(x);                                         \
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
    MG_DBGP('-');
    switch (expecting) {
      case S_VALUE:
        if (depth == ed) j = i;
        if (c == '{') {
          if (depth >= (int) sizeof(nesting)) return MG_JSON_TOO_DEEP;
          if (depth == ed && path[pos] == '.') ed++, pos++;
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
          // printf("K[%.*s] %d %d\n", n, &s[i + 1], depth, ed);
          if (depth == ed && path[pos - 1] == '.' &&
              memcmp(&s[i + 1], &path[pos], (size_t) n) == 0) {
            pos += n;
          }
          i += n + 1;
          expecting = S_COLON;
        } else if (c == '}') {  // Empty object
          MG_EOO('}');
          expecting = S_COMMA_OR_EOO;
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
          // MG_CHECKRET('C');
        } else if (c == ']' || c == '}') {
          MG_EOO('O');
          if (depth == ed && ei >= 0) ci++;
        } else {
          return MG_JSON_INVALID;
        }
        break;
    }
    MG_DBGP('E');
  }
  return MG_JSON_NOT_FOUND;
}

bool mg_json_get_num(struct mg_str json, const char *path, double *v) {
  int n, toklen, found = 0;
  if ((n = mg_json_get(json.ptr, (int) json.len, path, &toklen)) >= 0 &&
      (json.ptr[n] == '-' || (json.ptr[n] >= '0' && json.ptr[n] <= '9'))) {
    if (v != NULL) *v = mg_atod(json.ptr + n, toklen, NULL);
    found = 1;
  }
  return found;
}

bool mg_json_get_bool(struct mg_str json, const char *path, bool *v) {
  int n, toklen, found = 0;
  if ((n = mg_json_get(json.ptr, (int) json.len, path, &toklen)) >= 0 &&
      (json.ptr[n] == 't' || json.ptr[n] == 'f')) {
    if (v != NULL) *v = json.ptr[n] == 't';
    found = 1;
  }
  return found;
}

static bool json_unescape(const char *s, size_t len, char *to, size_t n) {
  size_t i, j;
  for (i = 0, j = 0; i < len && j < n; i++, j++) {
    if (s[i] == '\\' && i + 5 < len && s[i + 1] == 'u') {
      //  \uXXXX escape. We could process a simple one-byte chars
      // \u00xx from the ASCII range. More complex chars would require
      // dragging in a UTF8 library, which is too much for us
      if (s[i + 2] != '0' || s[i + 3] != '0') return false;  // Give up
      ((unsigned char *) to)[j] = (unsigned char) mg_unhexn(s + i + 4, 2);

      i += 5;
    } else if (s[i] == '\\' && i + 1 < len) {
      char c = json_esc(s[i + 1], 0);
      if (c == 0) return false;
      to[j] = c;
      i++;
    } else {
      to[j] = s[i];
    }
  }
  if (j >= n) return false;
  if (n > 0) to[j] = '\0';
  return true;
}

char *mg_json_get_str(struct mg_str json, const char *path) {
  int n, toklen;
  char *result = NULL;
  if ((n = mg_json_get(json.ptr, (int) json.len, path, &toklen)) >= 0 &&
      json.ptr[n] == '"') {
    if ((result = (char *) calloc(1, (size_t) toklen)) != NULL &&
        !json_unescape(json.ptr + n + 1, (size_t) (toklen - 2), result,
                       (size_t) toklen)) {
      free(result);
      result = NULL;
    }
  }
  return result;
}
