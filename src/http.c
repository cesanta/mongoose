#include "http.h"
#include "arch.h"
#include "base64.h"
#include "log.h"
#include "net.h"
#include "ssi.h"
#include "util.h"
#include "version.h"
#include "ws.h"

// Multipart POST example:
// --xyz
// Content-Disposition: form-data; name="val"
//
// abcdef
// --xyz
// Content-Disposition: form-data; name="foo"; filename="a.txt"
// Content-Type: text/plain
//
// hello world
//
// --xyz--
size_t mg_http_next_multipart(struct mg_str body, size_t ofs,
                              struct mg_http_part *part) {
  struct mg_str cd = mg_str_n("Content-Disposition", 19);
  const char *s = body.ptr;
  size_t b = ofs, h1, h2, b1, b2, max = body.len;

  // Init part params
  if (part != NULL) part->name = part->filename = part->body = mg_str_n(0, 0);

  // Skip boundary
  while (b + 2 < max && s[b] != '\r' && s[b + 1] != '\n') b++;
  if (b <= ofs || b + 2 >= max) return 0;
  // MG_INFO(("B: %zu %zu [%.*s]", ofs, b - ofs, (int) (b - ofs), s));

  // Skip headers
  h1 = h2 = b + 2;
  for (;;) {
    while (h2 + 2 < max && s[h2] != '\r' && s[h2 + 1] != '\n') h2++;
    if (h2 == h1) break;
    if (h2 + 2 >= max) return 0;
    // MG_INFO(("Header: [%.*s]", (int) (h2 - h1), &s[h1]));
    if (part != NULL && h1 + cd.len + 2 < h2 && s[h1 + cd.len] == ':' &&
        mg_ncasecmp(&s[h1], cd.ptr, cd.len) == 0) {
      struct mg_str v = mg_str_n(&s[h1 + cd.len + 2], h2 - (h1 + cd.len + 2));
      part->name = mg_http_get_header_var(v, mg_str_n("name", 4));
      part->filename = mg_http_get_header_var(v, mg_str_n("filename", 8));
    }
    h1 = h2 = h2 + 2;
  }
  b1 = b2 = h2 + 2;
  while (b2 + 2 + (b - ofs) + 2 < max && !(s[b2] == '\r' && s[b2 + 1] == '\n' &&
                                           memcmp(&s[b2 + 2], s, b - ofs) == 0))
    b2++;

  if (b2 + 2 >= max) return 0;
  if (part != NULL) part->body = mg_str_n(&s[b1], b2 - b1);
  // MG_INFO(("Body: [%.*s]", (int) (b2 - b1), &s[b1]));
  return b2 + 2;
}

void mg_http_bauth(struct mg_connection *c, const char *user,
                   const char *pass) {
  struct mg_str u = mg_str(user), p = mg_str(pass);
  size_t need = c->send.len + 36 + (u.len + p.len) * 2;
  if (c->send.size < need) mg_iobuf_resize(&c->send, need);
  if (c->send.size >= need) {
    int i, n = 0;
    char *buf = (char *) &c->send.buf[c->send.len + 21];
    memcpy(&buf[-21], "Authorization: Basic ", 21);  // DON'T use mg_send!
    for (i = 0; i < (int) u.len; i++) {
      n = mg_base64_update(((unsigned char *) u.ptr)[i], buf, n);
    }
    if (p.len > 0) {
      n = mg_base64_update(':', buf, n);
      for (i = 0; i < (int) p.len; i++) {
        n = mg_base64_update(((unsigned char *) p.ptr)[i], buf, n);
      }
    }
    n = mg_base64_final(buf, n);
    c->send.len += 21 + (size_t) n + 2;
    memcpy(&c->send.buf[c->send.len - 2], "\r\n", 2);
  } else {
    MG_ERROR(("%lu %s cannot resize iobuf %d->%d ", c->id, c->label,
              (int) c->send.size, (int) need));
  }
}

int mg_http_get_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len) {
  const char *p, *e, *s;
  size_t name_len;
  int len;

  if (dst == NULL || dst_len == 0) {
    len = -2;  // Bad destination
  } else if (buf->ptr == NULL || name == NULL || buf->len == 0) {
    len = -1;  // Bad source
    dst[0] = '\0';
  } else {
    name_len = strlen(name);
    e = buf->ptr + buf->len;
    len = -4;  // Name does not exist
    dst[0] = '\0';
    for (p = buf->ptr; p + name_len < e; p++) {
      if ((p == buf->ptr || p[-1] == '&') && p[name_len] == '=' &&
          !mg_ncasecmp(name, p, name_len)) {
        p += name_len + 1;
        s = (const char *) memchr(p, '&', (size_t) (e - p));
        if (s == NULL) s = e;
        len = mg_url_decode(p, (size_t) (s - p), dst, dst_len, 1);
        if (len < 0) len = -3;  // Failed to decode
        break;
      }
    }
  }
  return len;
}

static bool isx(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

int mg_url_decode(const char *src, size_t src_len, char *dst, size_t dst_len,
                  int is_form_url_encoded) {
  size_t i, j;
  for (i = j = 0; i < src_len && j + 1 < dst_len; i++, j++) {
    if (src[i] == '%') {
      // Use `i + 2 < src_len`, not `i < src_len - 2`, note small src_len
      if (i + 2 < src_len && isx(src[i + 1]) && isx(src[i + 2])) {
        mg_unhex(src + i + 1, 2, (uint8_t *) &dst[j]);
        i += 2;
      } else {
        return -1;
      }
    } else if (is_form_url_encoded && src[i] == '+') {
      dst[j] = ' ';
    } else {
      dst[j] = src[i];
    }
  }
  if (j < dst_len) dst[j] = '\0';  // Null-terminate the destination
  return i >= src_len && j < dst_len ? (int) j : -1;
}

static bool isok(uint8_t c) {
  return c == '\n' || c == '\r' || c >= ' ';
}

int mg_http_get_request_len(const unsigned char *buf, size_t buf_len) {
  size_t i;
  for (i = 0; i < buf_len; i++) {
    if (!isok(buf[i])) return -1;
    if ((i > 0 && buf[i] == '\n' && buf[i - 1] == '\n') ||
        (i > 3 && buf[i] == '\n' && buf[i - 1] == '\r' && buf[i - 2] == '\n'))
      return (int) i + 1;
  }
  return 0;
}

static const char *skip(const char *s, const char *e, const char *d,
                        struct mg_str *v) {
  v->ptr = s;
  while (s < e && *s != '\n' && strchr(d, *s) == NULL) s++;
  v->len = (size_t) (s - v->ptr);
  while (s < e && strchr(d, *s) != NULL) s++;
  return s;
}

struct mg_str *mg_http_get_header(struct mg_http_message *h, const char *name) {
  size_t i, n = strlen(name), max = sizeof(h->headers) / sizeof(h->headers[0]);
  for (i = 0; i < max && h->headers[i].name.len > 0; i++) {
    struct mg_str *k = &h->headers[i].name, *v = &h->headers[i].value;
    if (n == k->len && mg_ncasecmp(k->ptr, name, n) == 0) return v;
  }
  return NULL;
}

static void mg_http_parse_headers(const char *s, const char *end,
                                  struct mg_http_header *h, int max_headers) {
  int i;
  for (i = 0; i < max_headers; i++) {
    struct mg_str k, v, tmp;
    const char *he = skip(s, end, "\n", &tmp);
    s = skip(s, he, ": \r\n", &k);
    s = skip(s, he, "\r\n", &v);
    if (k.len == tmp.len) continue;
    while (v.len > 0 && v.ptr[v.len - 1] == ' ') v.len--;  // Trim spaces
    if (k.len == 0) break;
    // MG_INFO(("--HH [%.*s] [%.*s] [%.*s]", (int) tmp.len - 1, tmp.ptr,
    //(int) k.len, k.ptr, (int) v.len, v.ptr));
    h[i].name = k;
    h[i].value = v;
  }
}

int mg_http_parse(const char *s, size_t len, struct mg_http_message *hm) {
  int is_response, req_len = mg_http_get_request_len((unsigned char *) s, len);
  const char *end = s + req_len, *qs;
  struct mg_str *cl;

  memset(hm, 0, sizeof(*hm));
  if (req_len <= 0) return req_len;

  hm->message.ptr = hm->head.ptr = s;
  hm->body.ptr = end;
  hm->head.len = (size_t) req_len;
  hm->chunk.ptr = end;
  hm->message.len = hm->body.len = (size_t) ~0;  // Set body length to infinite

  // Parse request line
  s = skip(s, end, " ", &hm->method);
  s = skip(s, end, " ", &hm->uri);
  s = skip(s, end, "\r\n", &hm->proto);

  // Sanity check. Allow protocol/reason to be empty
  if (hm->method.len == 0 || hm->uri.len == 0) return -1;

  // If URI contains '?' character, setup query string
  if ((qs = (const char *) memchr(hm->uri.ptr, '?', hm->uri.len)) != NULL) {
    hm->query.ptr = qs + 1;
    hm->query.len = (size_t) (&hm->uri.ptr[hm->uri.len] - (qs + 1));
    hm->uri.len = (size_t) (qs - hm->uri.ptr);
  }

  mg_http_parse_headers(s, end, hm->headers,
                        sizeof(hm->headers) / sizeof(hm->headers[0]));
  if ((cl = mg_http_get_header(hm, "Content-Length")) != NULL) {
    hm->body.len = (size_t) mg_to64(*cl);
    hm->message.len = (size_t) req_len + hm->body.len;
  }

  // mg_http_parse() is used to parse both HTTP requests and HTTP
  // responses. If HTTP response does not have Content-Length set, then
  // body is read until socket is closed, i.e. body.len is infinite (~0).
  //
  // For HTTP requests though, according to
  // http://tools.ietf.org/html/rfc7231#section-8.1.3,
  // only POST and PUT methods have defined body semantics.
  // Therefore, if Content-Length is not specified and methods are
  // not one of PUT or POST, set body length to 0.
  //
  // So, if it is HTTP request, and Content-Length is not set,
  // and method is not (PUT or POST) then reset body length to zero.
  is_response = mg_ncasecmp(hm->method.ptr, "HTTP/", 5) == 0;
  if (hm->body.len == (size_t) ~0 && !is_response &&
      mg_vcasecmp(&hm->method, "PUT") != 0 &&
      mg_vcasecmp(&hm->method, "POST") != 0) {
    hm->body.len = 0;
    hm->message.len = (size_t) req_len;
  }

  // The 204 (No content) responses also have 0 body length
  if (hm->body.len == (size_t) ~0 && is_response &&
      mg_vcasecmp(&hm->uri, "204") == 0) {
    hm->body.len = 0;
    hm->message.len = (size_t) req_len;
  }

  return req_len;
}

static void mg_http_vprintf_chunk(struct mg_connection *c, const char *fmt,
                                  va_list ap) {
  char mem[256], *buf = mem;
  size_t len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  mg_printf(c, "%lx\r\n", (unsigned long) len);
  mg_send(c, buf, len > 0 ? (size_t) len : 0);
  mg_send(c, "\r\n", 2);
  if (buf != mem) free(buf);
}

void mg_http_printf_chunk(struct mg_connection *c, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_http_vprintf_chunk(c, fmt, ap);
  va_end(ap);
}

void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len) {
  mg_printf(c, "%lx\r\n", (unsigned long) len);
  mg_send(c, buf, len);
  mg_send(c, "\r\n", 2);
}

// clang-format off
static const char *mg_http_status_code_str(int status_code) {
  switch (status_code) {
    case 100: return "Continue";
    case 201: return "Created";
    case 202: return "Accepted";
    case 204: return "No Content";
    case 206: return "Partial Content";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 304: return "Not Modified";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 418: return "I'm a teapot";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    default: return "OK";
  }
}
// clang-format on

void mg_http_reply(struct mg_connection *c, int code, const char *headers,
                   const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  size_t len;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  mg_printf(c, "HTTP/1.1 %d %s\r\n%sContent-Length: %d\r\n\r\n", code,
            mg_http_status_code_str(code), headers == NULL ? "" : headers, len);
  mg_send(c, buf, len > 0 ? len : 0);
  if (buf != mem) free(buf);
}

static void http_cb(struct mg_connection *, int, void *, void *);
static void restore_http_cb(struct mg_connection *c) {
  mg_fs_close((struct mg_fd *) c->pfn_data);
  c->pfn_data = NULL;
  c->pfn = http_cb;
}

char *mg_http_etag(char *buf, size_t len, size_t size, time_t mtime);
char *mg_http_etag(char *buf, size_t len, size_t size, time_t mtime) {
  mg_snprintf(buf, len, "\"%lld.%lld\"", (int64_t) mtime, (int64_t) size);
  return buf;
}

static void static_cb(struct mg_connection *c, int ev, void *ev_data,
                      void *fn_data) {
  if (ev == MG_EV_WRITE || ev == MG_EV_POLL) {
    struct mg_fd *fd = (struct mg_fd *) fn_data;
    // Read to send IO buffer directly, avoid extra on-stack buffer
    size_t n, max = MG_IO_SIZE, space, *cl = (size_t *) c->label;
    if (c->send.size < max) mg_iobuf_resize(&c->send, max);
    if (c->send.len >= c->send.size) return;  // Rate limit
    if ((space = c->send.size - c->send.len) > *cl) space = *cl;
    n = fd->fs->rd(fd->fd, c->send.buf + c->send.len, space);
    c->send.len += n;
    *cl -= n;
    if (n == 0) restore_http_cb(c);
  } else if (ev == MG_EV_CLOSE) {
    restore_http_cb(c);
  }
  (void) ev_data;
}

static struct mg_str guess_content_type(struct mg_str path, const char *extra) {
  struct mg_str k, v, s = mg_str(extra);
  size_t i = 0;

  // clang-format off
  struct mg_str tab[] = {
      MG_C_STR("html"), MG_C_STR("text/html; charset=utf-8"),
      MG_C_STR("htm"), MG_C_STR("text/html; charset=utf-8"),
      MG_C_STR("css"), MG_C_STR("text/css; charset=utf-8"),
      MG_C_STR("js"), MG_C_STR("text/javascript; charset=utf-8"),
      MG_C_STR("gif"), MG_C_STR("image/gif"),
      MG_C_STR("png"), MG_C_STR("image/png"),
      MG_C_STR("jpg"), MG_C_STR("image/jpeg"),
      MG_C_STR("jpeg"), MG_C_STR("image/jpeg"),
      MG_C_STR("woff"), MG_C_STR("font/woff"),
      MG_C_STR("ttf"), MG_C_STR("font/ttf"),
      MG_C_STR("svg"), MG_C_STR("image/svg+xml"),
      MG_C_STR("txt"), MG_C_STR("text/plain; charset=utf-8"),
      MG_C_STR("avi"), MG_C_STR("video/x-msvideo"),
      MG_C_STR("csv"), MG_C_STR("text/csv"),
      MG_C_STR("doc"), MG_C_STR("application/msword"),
      MG_C_STR("exe"), MG_C_STR("application/octet-stream"),
      MG_C_STR("gz"), MG_C_STR("application/gzip"),
      MG_C_STR("ico"), MG_C_STR("image/x-icon"),
      MG_C_STR("json"), MG_C_STR("application/json"),
      MG_C_STR("mov"), MG_C_STR("video/quicktime"),
      MG_C_STR("mp3"), MG_C_STR("audio/mpeg"),
      MG_C_STR("mp4"), MG_C_STR("video/mp4"),
      MG_C_STR("mpeg"), MG_C_STR("video/mpeg"),
      MG_C_STR("pdf"), MG_C_STR("application/pdf"),
      MG_C_STR("shtml"), MG_C_STR("text/html; charset=utf-8"),
      MG_C_STR("tgz"), MG_C_STR("application/tar-gz"),
      MG_C_STR("wav"), MG_C_STR("audio/wav"),
      MG_C_STR("webp"), MG_C_STR("image/webp"),
      MG_C_STR("zip"), MG_C_STR("application/zip"),
      MG_C_STR("3gp"), MG_C_STR("video/3gpp"),
      {0, 0},
  };
  // clang-format on

  // Shrink path to its extension only
  while (i < path.len && path.ptr[path.len - i - 1] != '.') i++;
  path.ptr += path.len - i;
  path.len = i;

  // Process user-provided mime type overrides, if any
  while (mg_commalist(&s, &k, &v)) {
    if (mg_strcmp(path, k) == 0) return v;
  }

  // Process built-in mime types
  for (i = 0; tab[i].ptr != NULL; i += 2) {
    if (mg_strcmp(path, tab[i]) == 0) return tab[i + 1];
  }

  return mg_str("text/plain; charset=utf-8");
}

static int getrange(struct mg_str *s, int64_t *a, int64_t *b) {
  size_t i, numparsed = 0;
  // MG_INFO(("%.*s", (int) s->len, s->ptr));
  for (i = 0; i + 6 < s->len; i++) {
    if (memcmp(&s->ptr[i], "bytes=", 6) == 0) {
      struct mg_str p = mg_str_n(s->ptr + i + 6, s->len - i - 6);
      if (p.len > 0 && p.ptr[0] >= '0' && p.ptr[0] <= '9') numparsed++;
      *a = mg_to64(p);
      // MG_INFO(("PPP [%.*s] %d", (int) p.len, p.ptr, numparsed));
      while (p.len && p.ptr[0] >= '0' && p.ptr[0] <= '9') p.ptr++, p.len--;
      if (p.len && p.ptr[0] == '-') p.ptr++, p.len--;
      *b = mg_to64(p);
      if (p.len > 0 && p.ptr[0] >= '0' && p.ptr[0] <= '9') numparsed++;
      // MG_INFO(("PPP [%.*s] %d", (int) p.len, p.ptr, numparsed));
      break;
    }
  }
  return (int) numparsed;
}

void mg_http_serve_file(struct mg_connection *c, struct mg_http_message *hm,
                        const char *path,
                        const struct mg_http_serve_opts *opts) {
  char etag[64], tmp[MG_PATH_MAX];
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct mg_fd *fd = path == NULL ? NULL : mg_fs_open(fs, path, MG_FS_READ);
  size_t size = 0;
  time_t mtime = 0;
  struct mg_str *inm = NULL;
  struct mg_str mime = guess_content_type(mg_str(path), opts->mime_types);
  bool gzip = false;

  // If file does not exist, we try to open file PATH.gz - and if such
  // pre-compressed .gz file exists, serve it with the Content-Encoding: gzip
  // Note - we ignore Accept-Encoding, cause we don't have a choice
  if (fd == NULL) {
    MG_DEBUG(("NULL [%s]", path));
    mg_snprintf(tmp, sizeof(tmp), "%s.gz", path);
    if ((fd = mg_fs_open(fs, tmp, MG_FS_READ)) != NULL) {
      gzip = true;
      path = tmp;
    } else if (opts->page404 != NULL) {
      // No precompressed file, serve 404
      fd = mg_fs_open(fs, opts->page404, MG_FS_READ);
      mime = guess_content_type(mg_str(path), opts->mime_types);
      path = opts->page404;
    }
  }

  if (fd == NULL || fs->st(path, &size, &mtime) == 0) {
    mg_http_reply(c, 404, opts->extra_headers, "Not found\n");
    mg_fs_close(fd);
    // NOTE: mg_http_etag() call should go first!
  } else if (mg_http_etag(etag, sizeof(etag), size, mtime) != NULL &&
             (inm = mg_http_get_header(hm, "If-None-Match")) != NULL &&
             mg_vcasecmp(inm, etag) == 0) {
    mg_fs_close(fd);
    mg_printf(c, "HTTP/1.1 304 Not Modified\r\n%sContent-Length: 0\r\n\r\n",
              opts->extra_headers ? opts->extra_headers : "");
  } else {
    int n, status = 200;
    char range[100] = "";
    int64_t r1 = 0, r2 = 0, cl = (int64_t) size;

    // Handle Range header
    struct mg_str *rh = mg_http_get_header(hm, "Range");
    if (rh != NULL && (n = getrange(rh, &r1, &r2)) > 0 && r1 >= 0 && r2 >= 0) {
      // If range is specified like "400-", set second limit to content len
      if (n == 1) r2 = cl - 1;
      if (r1 > r2 || r2 >= cl) {
        status = 416;
        cl = 0;
        mg_snprintf(range, sizeof(range), "Content-Range: bytes */%lld\r\n",
                    (int64_t) size);
      } else {
        status = 206;
        cl = r2 - r1 + 1;
        mg_snprintf(range, sizeof(range),
                    "Content-Range: bytes %lld-%lld/%lld\r\n", r1, r1 + cl - 1,
                    (int64_t) size);
        fs->sk(fd->fd, (size_t) r1);
      }
    }
    mg_printf(c,
              "HTTP/1.1 %d %s\r\n"
              "Content-Type: %.*s\r\n"
              "Etag: %s\r\n"
              "Content-Length: %llu\r\n"
              "%s%s%s\r\n",
              status, mg_http_status_code_str(status), (int) mime.len, mime.ptr,
              etag, cl, gzip ? "Content-Encoding: gzip\r\n" : "", range,
              opts->extra_headers ? opts->extra_headers : "");
    if (mg_vcasecmp(&hm->method, "HEAD") == 0) {
      c->is_draining = 1;
      mg_fs_close(fd);
    } else {
      c->pfn = static_cb;
      c->pfn_data = fd;
      *(size_t *) c->label = (size_t) cl;  // Track to-be-sent content length
    }
  }
}

struct printdirentrydata {
  struct mg_connection *c;
  struct mg_http_message *hm;
  const struct mg_http_serve_opts *opts;
  const char *dir;
};

static void printdirentry(const char *name, void *userdata) {
  struct printdirentrydata *d = (struct printdirentrydata *) userdata;
  struct mg_fs *fs = d->opts->fs == NULL ? &mg_fs_posix : d->opts->fs;
  size_t size = 0;
  time_t t = 0;
  char path[MG_PATH_MAX], sz[40], mod[40];
  int flags, n = 0;

  // MG_DEBUG(("[%s] [%s]", d->dir, name));
  if (mg_snprintf(path, sizeof(path), "%s%c%s", d->dir, '/', name) >
      sizeof(path)) {
    MG_ERROR(("%s truncated", name));
  } else if ((flags = fs->st(path, &size, &t)) == 0) {
    MG_ERROR(("%lu stat(%s): %d", d->c->id, path, errno));
  } else {
    const char *slash = flags & MG_FS_DIR ? "/" : "";
    if (flags & MG_FS_DIR) {
      mg_snprintf(sz, sizeof(sz), "%s", "[DIR]");
    } else {
      mg_snprintf(sz, sizeof(sz), "%lld", (uint64_t) size);
    }
    mg_snprintf(mod, sizeof(mod), "%ld", (unsigned long) t);
    n = (int) mg_url_encode(name, strlen(name), path, sizeof(path));
    mg_printf(d->c,
              "  <tr><td><a href=\"%.*s%s\">%s%s</a></td>"
              "<td name=%lu>%s</td><td name=%lld>%s</td></tr>\n",
              n, path, slash, name, slash, (unsigned long) t, mod,
              flags & MG_FS_DIR ? (int64_t) -1 : (int64_t) size, sz);
  }
}

static void listdir(struct mg_connection *c, struct mg_http_message *hm,
                    const struct mg_http_serve_opts *opts, char *dir) {
  const char *sort_js_code =
      "<script>function srt(tb, sc, so, d) {"
      "var tr = Array.prototype.slice.call(tb.rows, 0),"
      "tr = tr.sort(function (a, b) { var c1 = a.cells[sc], c2 = b.cells[sc],"
      "n1 = c1.getAttribute('name'), n2 = c2.getAttribute('name'), "
      "t1 = a.cells[2].getAttribute('name'), "
      "t2 = b.cells[2].getAttribute('name'); "
      "return so * (t1 < 0 && t2 >= 0 ? -1 : t2 < 0 && t1 >= 0 ? 1 : "
      "n1 ? parseInt(n2) - parseInt(n1) : "
      "c1.textContent.trim().localeCompare(c2.textContent.trim())); });";
  const char *sort_js_code2 =
      "for (var i = 0; i < tr.length; i++) tb.appendChild(tr[i]); "
      "if (!d) window.location.hash = ('sc=' + sc + '&so=' + so); "
      "};"
      "window.onload = function() {"
      "var tb = document.getElementById('tb');"
      "var m = /sc=([012]).so=(1|-1)/.exec(window.location.hash) || [0, 2, 1];"
      "var sc = m[1], so = m[2]; document.onclick = function(ev) { "
      "var c = ev.target.rel; if (c) {if (c == sc) so *= -1; srt(tb, c, so); "
      "sc = c; ev.preventDefault();}};"
      "srt(tb, sc, so, true);"
      "}"
      "</script>";
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct printdirentrydata d = {c, hm, opts, dir};
  char tmp[10], buf[MG_PATH_MAX];
  size_t off, n;
  int len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 0);
  struct mg_str uri = len > 0 ? mg_str_n(buf, (size_t) len) : hm->uri;

  mg_printf(c,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=utf-8\r\n"
            "%s"
            "Content-Length:         \r\n\r\n",
            opts->extra_headers == NULL ? "" : opts->extra_headers);
  off = c->send.len;  // Start of body
  mg_printf(c,
            "<!DOCTYPE html><html><head><title>Index of %.*s</title>%s%s"
            "<style>th,td {text-align: left; padding-right: 1em; "
            "font-family: monospace; }</style></head>"
            "<body><h1>Index of %.*s</h1><table cellpadding=\"0\"><thead>"
            "<tr><th><a href=\"#\" rel=\"0\">Name</a></th><th>"
            "<a href=\"#\" rel=\"1\">Modified</a></th>"
            "<th><a href=\"#\" rel=\"2\">Size</a></th></tr>"
            "<tr><td colspan=\"3\"><hr></td></tr>"
            "</thead>"
            "<tbody id=\"tb\">\n",
            (int) uri.len, uri.ptr, sort_js_code, sort_js_code2, (int) uri.len,
            uri.ptr);
  mg_printf(c, "%s",
            "  <tr><td><a href=\"..\">..</a></td>"
            "<td name=-1></td><td name=-1>[DIR]</td></tr>\n");

  fs->ls(dir, printdirentry, &d);
  mg_printf(c,
            "</tbody><tfoot><tr><td colspan=\"3\"><hr></td></tr></tfoot>"
            "</table><address>Mongoose v.%s</address></body></html>\n",
            MG_VERSION);
  n = mg_snprintf(tmp, sizeof(tmp), "%lu", (unsigned long) (c->send.len - off));
  if (n > sizeof(tmp)) n = 0;
  memcpy(c->send.buf + off - 12, tmp, n);  // Set content length
}

static void remove_double_dots(char *s) {
  char *p = s;
  while (*s != '\0') {
    *p++ = *s++;
    if (s[-1] == '/' || s[-1] == '\\') {
      while (s[0] != '\0') {
        if (s[0] == '/' || s[0] == '\\') {
          s++;
        } else if (s[0] == '.' && s[1] == '.' &&
                   (s[2] == '/' || s[2] == '\\')) {
          s += 2;
        } else {
          break;
        }
      }
    }
  }
  *p = '\0';
}

// Resolve requested file into `path` and return its fs->st() result
static int uri_to_path2(struct mg_connection *c, struct mg_http_message *hm,
                        struct mg_fs *fs, struct mg_str url, struct mg_str dir,
                        char *path, size_t path_size) {
  int flags, tmp;
  // Append URI to the root_dir, and sanitize it
  size_t n = mg_snprintf(path, path_size, "%.*s", (int) dir.len, dir.ptr);
  if (n > path_size) n = path_size;
  path[path_size - 1] = '\0';
  if (n + 2 < path_size) path[n++] = '/', path[n] = '\0';
  mg_url_decode(hm->uri.ptr + url.len, hm->uri.len - url.len, path + n,
                path_size - n, 0);
  path[path_size - 1] = '\0';  // Double-check
  remove_double_dots(path);
  n = strlen(path);
  while (n > 1 && path[n - 1] == '/') path[--n] = 0;  // Trim trailing slashes
  flags = mg_vcmp(&hm->uri, "/") == 0 ? MG_FS_DIR : fs->st(path, NULL, NULL);
  MG_VERBOSE(("%lu %.*s -> %s %d", c->id, (int) hm->uri.len, hm->uri.ptr, path,
              flags));
  if (flags == 0) {
    // Do nothing - let's caller decide
  } else if ((flags & MG_FS_DIR) && hm->uri.len > 0 &&
             hm->uri.ptr[hm->uri.len - 1] != '/') {
    mg_printf(c,
              "HTTP/1.1 301 Moved\r\n"
              "Location: %.*s/\r\n"
              "Content-Length: 0\r\n"
              "\r\n",
              (int) hm->uri.len, hm->uri.ptr);
    flags = -1;
  } else if (flags & MG_FS_DIR) {
    if (((mg_snprintf(path + n, path_size - n, "/" MG_HTTP_INDEX) > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0) ||
         (mg_snprintf(path + n, path_size - n, "/index.shtml") > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0))) {
      flags = tmp;
    } else {
      path[n] = '\0';  // Remove appended index file name
    }
  }
  return flags;
}

static int uri_to_path(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts, char *path,
                       size_t path_size) {
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct mg_str k, v, s = mg_str(opts->root_dir), u = {0, 0}, p = {0, 0};
  while (mg_commalist(&s, &k, &v)) {
    if (v.len == 0) v = k, k = mg_str("/");
    if (hm->uri.len < k.len) continue;
    if (mg_strcmp(k, mg_str_n(hm->uri.ptr, k.len)) != 0) continue;
    u = k, p = v;
  }
  return uri_to_path2(c, hm, fs, u, p, path, path_size);
}

void mg_http_serve_dir(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts) {
  char path[MG_PATH_MAX] = "";
  const char *sp = opts->ssi_pattern;
  int flags = uri_to_path(c, hm, opts, path, sizeof(path));
  if (flags < 0) {
    // Do nothing: the response has already been sent by uri_to_path()
  } else if (flags & MG_FS_DIR) {
    listdir(c, hm, opts, path);
  } else if (flags && sp != NULL &&
             mg_globmatch(sp, strlen(sp), path, strlen(path))) {
    mg_http_serve_ssi(c, opts->root_dir, path);
  } else {
    mg_http_serve_file(c, hm, path, opts);
  }
}

static bool mg_is_url_safe(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') || c == '.' || c == '_' || c == '-' || c == '~';
}

size_t mg_url_encode(const char *s, size_t sl, char *buf, size_t len) {
  size_t i, n = 0;
  for (i = 0; i < sl; i++) {
    int c = *(unsigned char *) &s[i];
    if (n + 4 >= len) return 0;
    if (mg_is_url_safe(c)) {
      buf[n++] = s[i];
    } else {
      buf[n++] = '%';
      mg_hex(&s[i], 1, &buf[n]);
      n += 2;
    }
  }
  return n;
}

void mg_http_creds(struct mg_http_message *hm, char *user, size_t userlen,
                   char *pass, size_t passlen) {
  struct mg_str *v = mg_http_get_header(hm, "Authorization");
  user[0] = pass[0] = '\0';
  if (v != NULL && v->len > 6 && memcmp(v->ptr, "Basic ", 6) == 0) {
    char buf[256];
    int n = mg_base64_decode(v->ptr + 6, (int) v->len - 6, buf);
    const char *p = (const char *) memchr(buf, ':', n > 0 ? (size_t) n : 0);
    if (p != NULL) {
      mg_snprintf(user, userlen, "%.*s", (int) (p - buf), buf);
      mg_snprintf(pass, passlen, "%.*s", n - (int) (p - buf) - 1, p + 1);
    }
  } else if (v != NULL && v->len > 7 && memcmp(v->ptr, "Bearer ", 7) == 0) {
    mg_snprintf(pass, passlen, "%.*s", (int) v->len - 7, v->ptr + 7);
  } else if ((v = mg_http_get_header(hm, "Cookie")) != NULL) {
    struct mg_str t = mg_http_get_header_var(*v, mg_str_n("access_token", 12));
    if (t.len > 0) mg_snprintf(pass, passlen, "%.*s", (int) t.len, t.ptr);
  } else {
    mg_http_get_var(&hm->query, "access_token", pass, passlen);
  }
}

static struct mg_str stripquotes(struct mg_str s) {
  return s.len > 1 && s.ptr[0] == '"' && s.ptr[s.len - 1] == '"'
             ? mg_str_n(s.ptr + 1, s.len - 2)
             : s;
}

struct mg_str mg_http_get_header_var(struct mg_str s, struct mg_str v) {
  size_t i;
  for (i = 0; v.len > 0 && i + v.len + 2 < s.len; i++) {
    if (s.ptr[i + v.len] == '=' && memcmp(&s.ptr[i], v.ptr, v.len) == 0) {
      const char *p = &s.ptr[i + v.len + 1], *b = p, *x = &s.ptr[s.len];
      int q = p < x && *p == '"' ? 1 : 0;
      while (p < x &&
             (q ? p == b || *p != '"' : *p != ';' && *p != ' ' && *p != ','))
        p++;
      // MG_INFO(("[%.*s] [%.*s] [%.*s]", (int) s.len, s.ptr, (int) v.len,
      // v.ptr, (int) (p - b), b));
      return stripquotes(mg_str_n(b, (size_t) (p - b + q)));
    }
  }
  return mg_str_n(NULL, 0);
}

bool mg_http_match_uri(const struct mg_http_message *hm, const char *glob) {
  return mg_match(hm->uri, mg_str(glob), NULL);
}

static size_t get_chunk_length(const char *buf, size_t len, size_t *ll) {
  size_t i = 0, n;
  while (i < len && buf[i] != '\r' && i != '\n') i++;
  n = mg_unhexn((char *) buf, i);
  while (i < len && (buf[i] == '\r' || i == '\n')) i++;
  // MG_INFO(("len %zu i %zu n %zu ", len, i, n));
  if (ll != NULL) *ll = i + 1;
  if (i < len && i + n + 2 < len) return i + n + 3;
  return 0;
}

// Walk through all chunks in the chunked body. For each chunk, fire
// an MG_EV_HTTP_CHUNK event.
static bool walkchunks(struct mg_connection *c, struct mg_http_message *hm,
                       size_t reqlen) {
  size_t off = 0, bl, ll;
  while (off + reqlen < c->recv.len) {
    char *buf = (char *) &c->recv.buf[reqlen];
    size_t memo = c->recv.len;
    size_t cl = get_chunk_length(&buf[off], memo - reqlen - off, &ll);
    // MG_INFO(("len %zu off %zu cl %zu ll %zu", len, off, cl, ll));
    if (cl == 0) break;
    hm->chunk = mg_str_n(&buf[off + ll], cl < ll + 2 ? 0 : cl - ll - 2);
    mg_call(c, MG_EV_HTTP_CHUNK, hm);
    // Increase offset only if user has not deleted this chunk
    if (memo == c->recv.len) off += cl;
    if (cl <= 5) {
      // Zero chunk - last one. Prepare body - cut off chunk lengths
      if (memo != c->recv.len) return true;  // Tell caller to cleanup
      off = bl = 0;
      while (off + reqlen < c->recv.len) {
        char *buf2 = (char *) &c->recv.buf[reqlen];
        size_t memo2 = c->recv.len;
        size_t cl2 = get_chunk_length(&buf2[off], memo2 - reqlen - off, &ll);
        size_t n = cl2 < ll + 2 ? 0 : cl2 - ll - 2;
        memmove(buf2 + bl, buf2 + off + ll, n);
        bl += n;
        off += cl2;
        if (cl2 <= 5) break;
      }
      // MG_INFO(("BL->%d del %d off %d", (int) bl, (int) del, (int) off));
      c->recv.len -= off - bl;
      // Set message length to indicate we've received
      // everything, to fire MG_EV_HTTP_MSG
      hm->message.len = bl + reqlen;
      hm->body.len = bl;
      // If user was deleting chunks, send nothing
      break;
    }
  }
  return false;
}

static bool mg_is_chunked(struct mg_http_message *hm) {
  const char *needle = "chunked";
  struct mg_str *te = mg_http_get_header(hm, "Transfer-Encoding");
  return te != NULL && mg_vcasecmp(te, needle) == 0;
}

void mg_http_delete_chunk(struct mg_connection *c, struct mg_http_message *hm) {
  struct mg_str ch = hm->chunk;
  const char *end = (char *) &c->recv.buf[c->recv.len], *ce;
  bool chunked = mg_is_chunked(hm);
  if (chunked) {
    ch.len += 4, ch.ptr -= 2;  // \r\n before and after the chunk
    while (ch.ptr > hm->body.ptr && *ch.ptr != '\n') ch.ptr--, ch.len++;
  }
  ce = &ch.ptr[ch.len];
  if (ce < end) memmove((void *) ch.ptr, ce, (size_t) (end - ce));
  c->recv.len -= ch.len;
  if (c->pfn_data != NULL) c->pfn_data = (char *) c->pfn_data - ch.len;
}

int mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                   struct mg_fs *fs, const char *dir) {
  char offset[40] = "", name[200] = "", path[256];
  int res = 0;
  mg_http_get_var(&hm->query, "offset", offset, sizeof(offset));
  mg_http_get_var(&hm->query, "name", name, sizeof(name));
  if (name[0] == '\0') {
    mg_http_reply(c, 400, "", "%s", "name required");
    res = -1;
  } else if (hm->body.len == 0) {
    mg_http_reply(c, 200, "", "%d", res);  // Nothing to write
  } else {
    struct mg_fd *fd;
    long oft = strtol(offset, NULL, 0);
    mg_snprintf(path, sizeof(path), "%s%c%s", dir, MG_DIRSEP, name);
    remove_double_dots(path);
    MG_DEBUG(("%d bytes @ %ld [%s]", (int) hm->body.len, oft, path));
    if (oft == 0) fs->rm(path);
    if ((fd = mg_fs_open(fs, path, MG_FS_WRITE)) == NULL) {
      mg_http_reply(c, 400, "", "open(%s): %d", path, errno);
      res = -2;
    } else {
      res = (int) fs->wr(fd->fd, hm->body.ptr, hm->body.len);
      mg_fs_close(fd);
      mg_http_reply(c, 200, "", "%d", res);
    }
  }
  return res;
}

int mg_http_status(const struct mg_http_message *hm) {
  return atoi(hm->uri.ptr);
}

static void http_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ || ev == MG_EV_CLOSE) {
    struct mg_http_message hm;
    while (c->recv.buf != NULL && c->recv.len > 0) {
      int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      bool is_chunked = n > 0 && mg_is_chunked(&hm);
      if (ev == MG_EV_CLOSE) {
        hm.message.len = c->recv.len;
        hm.body.len = hm.message.len - (size_t) (hm.body.ptr - hm.message.ptr);
      } else if (is_chunked && n > 0 && walkchunks(c, &hm, (size_t) n)) {
        // walkchunks told us to cleanup the request
        if (n > (int) c->recv.len) n = (int) c->recv.len;
        mg_iobuf_del(&c->recv, 0, (size_t) n);
        break;
      }
      // MG_INFO(("---->%d %d\n%.*s", n, is_chunked, (int) c->recv.len,
      // c->recv.buf));
      if (n < 0 && ev == MG_EV_READ) {
        mg_error(c, "HTTP parse:\n%.*s", (int) c->recv.len, c->recv.buf);
        break;
      } else if (n > 0 && (size_t) c->recv.len >= hm.message.len) {
        mg_call(c, MG_EV_HTTP_MSG, &hm);
        mg_iobuf_del(&c->recv, 0, hm.message.len);
      } else {
        if (n > 0 && !is_chunked) {
          hm.chunk =
              mg_str_n((char *) &c->recv.buf[n], c->recv.len - (size_t) n);
          // Store remaining body length in c->pfn_data
          if (c->pfn_data == NULL)
            c->pfn_data = (void *) (hm.message.len - (size_t) n);
          mg_call(c, MG_EV_HTTP_CHUNK, &hm);
          if (c->pfn_data == NULL) {
            hm.chunk.len = 0;                   // Last chunk!
            mg_call(c, MG_EV_HTTP_CHUNK, &hm);  // Lest user know
            memmove(c->recv.buf, c->recv.buf + n, c->recv.len - (size_t) n);
            c->recv.len -= (size_t) n;
          }
        }
        break;
      }
    }
  }
  (void) fnd;
  (void) evd;
}

struct mg_connection *mg_http_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = http_cb;
  return c;
}

struct mg_connection *mg_http_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = http_cb;
  return c;
}
