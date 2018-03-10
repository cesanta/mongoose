/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "mg_internal.h"
#include "mg_uri.h"

/*
 * scan string until encountering one of `seps`, keeping track of component
 * boundaries in `res`.
 *
 * `p` will point to the char after the separator or it will be `end`.
 */
static void parse_uri_component(const char **p, const char *end,
                                const char *seps, struct mg_str *res) {
  const char *q;
  res->p = *p;
  for (; *p < end; (*p)++) {
    for (q = seps; *q != '\0'; q++) {
      if (**p == *q) break;
    }
    if (*q != '\0') break;
  }
  res->len = (*p) - res->p;
  if (*p < end) (*p)++;
}

int mg_parse_uri(const struct mg_str uri, struct mg_str *scheme,
                 struct mg_str *user_info, struct mg_str *host,
                 unsigned int *port, struct mg_str *path, struct mg_str *query,
                 struct mg_str *fragment) {
  struct mg_str rscheme = {0, 0}, ruser_info = {0, 0}, rhost = {0, 0},
                rpath = {0, 0}, rquery = {0, 0}, rfragment = {0, 0};
  unsigned int rport = 0;
  enum {
    P_START,
    P_SCHEME_OR_PORT,
    P_USER_INFO,
    P_HOST,
    P_PORT,
    P_REST
  } state = P_START;

  const char *p = uri.p, *end = p + uri.len;
  while (p < end) {
    switch (state) {
      case P_START:
        /*
         * expecting on of:
         * - `scheme://xxxx`
         * - `xxxx:port`
         * - `[a:b:c]:port`
         * - `xxxx/path`
         */
        if (*p == '[') {
          state = P_HOST;
          break;
        }
        for (; p < end; p++) {
          if (*p == ':') {
            state = P_SCHEME_OR_PORT;
            break;
          } else if (*p == '/') {
            state = P_REST;
            break;
          }
        }
        if (state == P_START || state == P_REST) {
          rhost.p = uri.p;
          rhost.len = p - uri.p;
        }
        break;
      case P_SCHEME_OR_PORT:
        if (end - p >= 3 && strncmp(p, "://", 3) == 0) {
          rscheme.p = uri.p;
          rscheme.len = p - uri.p;
          state = P_USER_INFO;
          p += 3;
        } else {
          rhost.p = uri.p;
          rhost.len = p - uri.p;
          state = P_PORT;
        }
        break;
      case P_USER_INFO:
        ruser_info.p = p;
        for (; p < end; p++) {
          if (*p == '@' || *p == '[' || *p == '/') {
            break;
          }
        }
        if (p == end || *p == '/' || *p == '[') {
          /* backtrack and parse as host */
          p = ruser_info.p;
        }
        ruser_info.len = p - ruser_info.p;
        state = P_HOST;
        break;
      case P_HOST:
        if (*p == '@') p++;
        rhost.p = p;
        if (*p == '[') {
          int found = 0;
          for (; !found && p < end; p++) {
            found = (*p == ']');
          }
          if (!found) return -1;
        } else {
          for (; p < end; p++) {
            if (*p == ':' || *p == '/') break;
          }
        }
        rhost.len = p - rhost.p;
        if (p < end) {
          if (*p == ':') {
            state = P_PORT;
            break;
          } else if (*p == '/') {
            state = P_REST;
            break;
          }
        }
        break;
      case P_PORT:
        p++;
        for (; p < end; p++) {
          if (*p == '/') {
            state = P_REST;
            break;
          }
          rport *= 10;
          rport += *p - '0';
        }
        break;
      case P_REST:
        /* `p` points to separator. `path` includes the separator */
        parse_uri_component(&p, end, "?#", &rpath);
        if (p < end && *(p - 1) == '?') {
          parse_uri_component(&p, end, "#", &rquery);
        }
        parse_uri_component(&p, end, "", &rfragment);
        break;
    }
  }

  if (scheme != 0) *scheme = rscheme;
  if (user_info != 0) *user_info = ruser_info;
  if (host != 0) *host = rhost;
  if (port != 0) *port = rport;
  if (path != 0) *path = rpath;
  if (query != 0) *query = rquery;
  if (fragment != 0) *fragment = rfragment;

  return 0;
}

/* Normalize the URI path. Remove/resolve "." and "..". */
int mg_normalize_uri_path(const struct mg_str *in, struct mg_str *out) {
  const char *s = in->p, *se = s + in->len;
  char *cp = (char *) out->p, *d;

  if (in->len == 0 || *s != '/') {
    out->len = 0;
    return 0;
  }

  d = cp;

  while (s < se) {
    const char *next = s;
    struct mg_str component;
    parse_uri_component(&next, se, "/", &component);
    if (mg_vcmp(&component, ".") == 0) {
      /* Yum. */
    } else if (mg_vcmp(&component, "..") == 0) {
      /* Backtrack to previous slash. */
      if (d > cp + 1 && *(d - 1) == '/') d--;
      while (d > cp && *(d - 1) != '/') d--;
    } else {
      memmove(d, s, next - s);
      d += next - s;
    }
    s = next;
  }
  if (d == cp) *d++ = '/';

  out->p = cp;
  out->len = d - cp;
  return 1;
}

int mg_assemble_uri(const struct mg_str *scheme, const struct mg_str *user_info,
                    const struct mg_str *host, unsigned int port,
                    const struct mg_str *path, const struct mg_str *query,
                    const struct mg_str *fragment, int normalize_path,
                    struct mg_str *uri) {
  int result = -1;
  struct mbuf out;
  mbuf_init(&out, 0);

  if (scheme != NULL && scheme->len > 0) {
    mbuf_append(&out, scheme->p, scheme->len);
    mbuf_append(&out, "://", 3);
  }

  if (user_info != NULL && user_info->len > 0) {
    mbuf_append(&out, user_info->p, user_info->len);
    mbuf_append(&out, "@", 1);
  }

  if (host != NULL && host->len > 0) {
    mbuf_append(&out, host->p, host->len);
  }

  if (port != 0) {
    char port_str[20];
    int port_str_len = sprintf(port_str, ":%u", port);
    mbuf_append(&out, port_str, port_str_len);
  }

  if (path != NULL && path->len > 0) {
    if (normalize_path) {
      struct mg_str npath = mg_strdup(*path);
      if (npath.len != path->len) goto out;
      if (!mg_normalize_uri_path(path, &npath)) {
        free((void *) npath.p);
        goto out;
      }
      mbuf_append(&out, npath.p, npath.len);
      free((void *) npath.p);
    } else {
      mbuf_append(&out, path->p, path->len);
    }
  } else if (normalize_path) {
    mbuf_append(&out, "/", 1);
  }

  if (query != NULL && query->len > 0) {
    mbuf_append(&out, "?", 1);
    mbuf_append(&out, query->p, query->len);
  }

  if (fragment != NULL && fragment->len > 0) {
    mbuf_append(&out, "#", 1);
    mbuf_append(&out, fragment->p, fragment->len);
  }

  result = 0;

out:
  if (result == 0) {
    uri->p = out.buf;
    uri->len = out.len;
  } else {
    mbuf_free(&out);
    uri->p = NULL;
    uri->len = 0;
  }
  return result;
}
