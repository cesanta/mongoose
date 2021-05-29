#include "http.h"
#include "arch.h"
#include "base64.h"
#include "log.h"
#include "net.h"
#include "private.h"
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
  // LOG(LL_INFO, ("B: %zu %zu [%.*s]", ofs, b - ofs, (int) (b - ofs), s));

  // Skip headers
  h1 = h2 = b + 2;
  for (;;) {
    while (h2 + 2 < max && s[h2] != '\r' && s[h2 + 1] != '\n') h2++;
    if (h2 == h1) break;
    if (h2 + 2 >= max) return 0;
    // LOG(LL_INFO, ("Header: [%.*s]", (int) (h2 - h1), &s[h1]));
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
  // LOG(LL_INFO, ("Body: [%.*s]", (int) (b2 - b1), &s[b1]));
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
    LOG(LL_ERROR, ("%lu %s cannot resize iobuf %d->%d ", c->id, c->label,
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
        s = (const char *) memchr(p, '&', (size_t)(e - p));
        if (s == NULL) s = e;
        len = mg_url_decode(p, (size_t)(s - p), dst, dst_len, 1);
        if (len < 0) len = -3;  // Failed to decode
        break;
      }
    }
  }
  return len;
}

int mg_url_decode(const char *src, size_t src_len, char *dst, size_t dst_len,
                  int is_form_url_encoded) {
  size_t i, j;
  for (i = j = 0; i < src_len && j + 1 < dst_len; i++, j++) {
    if (src[i] == '%') {
      // Use `i + 2 < src_len`, not `i < src_len - 2`, note small src_len
      if (i + 2 < src_len && isxdigit(*(const unsigned char *) (src + i + 1)) &&
          isxdigit(*(const unsigned char *) (src + i + 2))) {
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

int mg_http_get_request_len(const unsigned char *buf, size_t buf_len) {
  size_t i;
  for (i = 0; i < buf_len; i++) {
    if (!isprint(buf[i]) && buf[i] != '\r' && buf[i] != '\n' && buf[i] < 128)
      return -1;
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
  v->len = (size_t)(s - v->ptr);
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

void mg_http_parse_headers(const char *s, const char *end,
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
    // LOG(LL_INFO, ("--HH [%.*s] [%.*s] [%.*s]", (int) tmp.len - 1, tmp.ptr,
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
    hm->query.len = (size_t)(&hm->uri.ptr[hm->uri.len] - (qs + 1));
    hm->uri.len = (size_t)(qs - hm->uri.ptr);
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
  int len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  mg_printf(c, "%X\r\n", len);
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
  mg_printf(c, "%lX\r\n", (unsigned long) len);
  mg_send(c, buf, len);
  mg_send(c, "\r\n", 2);
}

// clang-format off
static const char *mg_http_status_code_str(int status_code) {
  switch (status_code) {
    case 100: return "Continue";
    case 101: return "Switching Protocols";
    case 102: return "Processing";
    case 200: return "OK";
    case 201: return "Created";
    case 202: return "Accepted";
    case 203: return "Non-authoritative Information";
    case 204: return "No Content";
    case 205: return "Reset Content";
    case 206: return "Partial Content";
    case 207: return "Multi-Status";
    case 208: return "Already Reported";
    case 226: return "IM Used";
    case 300: return "Multiple Choices";
    case 301: return "Moved Permanently";
    case 302: return "Found";
    case 303: return "See Other";
    case 304: return "Not Modified";
    case 305: return "Use Proxy";
    case 307: return "Temporary Redirect";
    case 308: return "Permanent Redirect";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 402: return "Payment Required";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 406: return "Not Acceptable";
    case 407: return "Proxy Authentication Required";
    case 408: return "Request Timeout";
    case 409: return "Conflict";
    case 410: return "Gone";
    case 411: return "Length Required";
    case 412: return "Precondition Failed";
    case 413: return "Payload Too Large";
    case 414: return "Request-URI Too Long";
    case 415: return "Unsupported Media Type";
    case 416: return "Requested Range Not Satisfiable";
    case 417: return "Expectation Failed";
    case 418: return "I'm a teapot";
    case 421: return "Misdirected Request";
    case 422: return "Unprocessable Entity";
    case 423: return "Locked";
    case 424: return "Failed Dependency";
    case 426: return "Upgrade Required";
    case 428: return "Precondition Required";
    case 429: return "Too Many Requests";
    case 431: return "Request Header Fields Too Large";
    case 444: return "Connection Closed Without Response";
    case 451: return "Unavailable For Legal Reasons";
    case 499: return "Client Closed Request";
    case 500: return "Internal Server Error";
    case 501: return "Not Implemented";
    case 502: return "Bad Gateway";
    case 503: return "Service Unavailable";
    case 504: return "Gateway Timeout";
    case 505: return "HTTP Version Not Supported";
    case 506: return "Variant Also Negotiates";
    case 507: return "Insufficient Storage";
    case 508: return "Loop Detected";
    case 510: return "Not Extended";
    case 511: return "Network Authentication Required";
    case 599: return "Network Connect Timeout Error";
    default: return "OK";
  }
}
// clang-format on

void mg_http_reply(struct mg_connection *c, int code, const char *headers,
                   const char *fmt, ...) {
  char mem[100], *buf = mem;
  va_list ap;
  int len;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  mg_printf(c, "HTTP/1.1 %d %s\r\n%sContent-Length: %d\r\n\r\n", code,
            mg_http_status_code_str(code), headers == NULL ? "" : headers, len);
  mg_send(c, buf, len > 0 ? (size_t) len : 0);
  if (buf != mem) free(buf);
}

#if MG_ENABLE_FS
static void http_cb(struct mg_connection *, int, void *, void *);
static void restore_http_cb(struct mg_connection *c) {
  if (c->pfn_data != NULL) fclose((FILE *) c->pfn_data);
  c->pfn_data = NULL;
  c->pfn = http_cb;
}

char *mg_http_etag(char *buf, size_t len, mg_stat_t *st) {
  snprintf(buf, len, "\"%lx." MG_INT64_FMT "\"", (unsigned long) st->st_mtime,
           (int64_t) st->st_size);
  return buf;
}

int mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                   const char *dir) {
  char offset[40] = "", name[200] = "", path[256];
  mg_http_get_var(&hm->query, "offset", offset, sizeof(offset));
  mg_http_get_var(&hm->query, "name", name, sizeof(name));
  if (name[0] == '\0') {
    mg_http_reply(c, 400, "", "%s", "name required");
    return -1;
  } else {
    FILE *fp;
    size_t oft = strtoul(offset, NULL, 0);
    snprintf(path, sizeof(path), "%s%c%s", dir, MG_DIRSEP, name);
    LOG(LL_DEBUG,
        ("%p %d bytes @ %d [%s]", c->fd, (int) hm->body.len, (int) oft, name));
    if ((fp = mg_fopen(path, oft == 0 ? "wb" : "ab")) == NULL) {
      mg_http_reply(c, 400, "", "fopen(%s): %d", name, errno);
      return -2;
    } else {
      fwrite(hm->body.ptr, 1, hm->body.len, fp);
      fclose(fp);
      mg_http_reply(c, 200, "", "");
      return (int) hm->body.len;
    }
  }
}

static void static_cb(struct mg_connection *c, int ev, void *ev_data,
                      void *fn_data) {
  if (ev == MG_EV_WRITE || ev == MG_EV_POLL) {
    FILE *fp = (FILE *) fn_data;
    // Read to send IO buffer directly, avoid extra on-stack buffer
    size_t n, max = 2 * MG_IO_SIZE;
    if (c->send.size < max) mg_iobuf_resize(&c->send, max);
    if (c->send.len >= c->send.size) return;  // Rate limit
    n = fread(c->send.buf + c->send.len, 1, c->send.size - c->send.len, fp);
    if (n > 0) c->send.len += n;
    if (c->send.len < c->send.size) restore_http_cb(c);
  } else if (ev == MG_EV_CLOSE) {
    restore_http_cb(c);
  }
  (void) ev_data;
}

static const char *guess_content_type(const char *filename) {
  size_t n = strlen(filename);
#define MIME_ENTRY(_ext, _type) \
  { _ext, sizeof(_ext) - 1, _type }
  const struct {
    const char *ext;
    size_t ext_len;
    const char *type;
  } * t, types[] = {
             MIME_ENTRY("html", "text/html; charset=utf-8"),
             MIME_ENTRY("htm", "text/html; charset=utf-8"),
             MIME_ENTRY("shtml", "text/html; charset=utf-8"),
             MIME_ENTRY("css", "text/css"),
             MIME_ENTRY("js", "text/javascript"),
             MIME_ENTRY("mjs", "text/javascript"),
             MIME_ENTRY("json", "application/json"),
             MIME_ENTRY("ico", "image/x-icon"),
             MIME_ENTRY("gif", "image/gif"),
             MIME_ENTRY("jpg", "image/jpeg"),
             MIME_ENTRY("jpeg", "image/jpeg"),
             MIME_ENTRY("png", "image/png"),
             MIME_ENTRY("svg", "image/svg+xml"),
             MIME_ENTRY("txt", "text/plain; charset=utf-8"),
             MIME_ENTRY("wav", "audio/wav"),
             MIME_ENTRY("mp3", "audio/mpeg"),
             MIME_ENTRY("mid", "audio/mid"),
             MIME_ENTRY("ogg", "application/ogg"),
             MIME_ENTRY("xml", "application/xml"),
             MIME_ENTRY("ttf", "font/ttf"),
             MIME_ENTRY("json", "application/json"),
             MIME_ENTRY("xsl", "application/xml"),
             MIME_ENTRY("doc", "application/msword"),
             MIME_ENTRY("exe", "application/octet-stream"),
             MIME_ENTRY("zip", "application/zip"),
             MIME_ENTRY("xls", "application/excel"),
             MIME_ENTRY("tgz", "application/tar-gz"),
             MIME_ENTRY("tar", "application/tar"),
             MIME_ENTRY("gz", "application/gzip"),
             MIME_ENTRY("rar", "application/rar"),
             MIME_ENTRY("rtf", "application/rtf"),
             MIME_ENTRY("pdf", "application/pdf"),
             MIME_ENTRY("mpg", "video/mpeg"),
             MIME_ENTRY("webm", "video/webm"),
             MIME_ENTRY("mpeg", "video/mpeg"),
             MIME_ENTRY("mov", "video/quicktime"),
             MIME_ENTRY("mp4", "video/mp4"),
             MIME_ENTRY("avi", "video/x-msvideo"),
             MIME_ENTRY("csv", "text/csv"),
             MIME_ENTRY("bmp", "image/bmp"),
             MIME_ENTRY("bin", "application/octet-stream"),
             MIME_ENTRY("wasm", "application/wasm"),
             {NULL, 0, NULL},
         };

  for (t = types; t->ext != NULL; t++) {
    if (n < t->ext_len + 2) continue;
    if (mg_ncasecmp(t->ext, &filename[n - t->ext_len], t->ext_len)) continue;
    return t->type;
  }
  return "text/plain; charset=utf-8";
}

static int getrange(struct mg_str *s, int64_t *a, int64_t *b) {
  size_t i, numparsed = 0;
  LOG(LL_INFO, ("%.*s", (int) s->len, s->ptr));
  for (i = 0; i + 6 < s->len; i++) {
    if (memcmp(&s->ptr[i], "bytes=", 6) == 0) {
      struct mg_str p = mg_str_n(s->ptr + i + 6, s->len - i - 6);
      if (p.len > 0 && p.ptr[0] >= '0' && p.ptr[0] <= '9') numparsed++;
      *a = mg_to64(p);
      // LOG(LL_INFO, ("PPP [%.*s] %d", (int) p.len, p.ptr, numparsed));
      while (p.len && p.ptr[0] >= '0' && p.ptr[0] <= '9') p.ptr++, p.len--;
      if (p.len && p.ptr[0] == '-') p.ptr++, p.len--;
      *b = mg_to64(p);
      if (p.len > 0 && p.ptr[0] >= '0' && p.ptr[0] <= '9') numparsed++;
      // LOG(LL_INFO, ("PPP [%.*s] %d", (int) p.len, p.ptr, numparsed));
      break;
    }
  }
  return (int) numparsed;
}

void mg_http_serve_file(struct mg_connection *c, struct mg_http_message *hm,
                        const char *path, const char *mime, const char *hdrs) {
  struct mg_str *inm = mg_http_get_header(hm, "If-None-Match");
  mg_stat_t st;
  char etag[64];
  FILE *fp = mg_fopen(path, "rb");
  if (fp == NULL || mg_stat(path, &st) != 0 ||
      mg_http_etag(etag, sizeof(etag), &st) != etag) {
    LOG(LL_DEBUG,
        ("404 [%.*s] [%s] %p", (int) hm->uri.len, hm->uri.ptr, path, fp));
    mg_http_reply(c, 404, "", "%s", "Not found\n");
    if (fp != NULL) fclose(fp);
  } else if (inm != NULL && mg_vcasecmp(inm, etag) == 0) {
    fclose(fp);
    mg_printf(c, "HTTP/1.1 304 Not Modified\r\nContent-Length: 0\r\n\r\n");
  } else {
    int n, status = 200;
    char range[100] = "";
    int64_t r1 = 0, r2 = 0, cl = st.st_size;

    // Handle Range header
    struct mg_str *rh = mg_http_get_header(hm, "Range");
    if (rh != NULL && (n = getrange(rh, &r1, &r2)) > 0 && r1 >= 0 && r2 >= 0) {
      // If range is specified like "400-", set second limit to content len
      if (n == 1) r2 = cl - 1;
      if (r1 > r2 || r2 >= cl) {
        status = 416;
        cl = 0;
        snprintf(range, sizeof(range),
                 "Content-Range: bytes */" MG_INT64_FMT "\r\n",
                 (int64_t) st.st_size);
      } else {
        status = 206;
        cl = r2 - r1 + 1;
        snprintf(range, sizeof(range),
                 "Content-Range: bytes " MG_INT64_FMT "-" MG_INT64_FMT
                 "/" MG_INT64_FMT "\r\n",
                 r1, r1 + cl - 1, (int64_t) st.st_size);
#if _FILE_OFFSET_BITS == 64 || _POSIX_C_SOURCE >= 200112L || \
    _XOPEN_SOURCE >= 600
        fseeko(fp, (off_t) r1, SEEK_SET);
#else
        fseek(fp, (long) r1, SEEK_SET);
#endif
      }
    }

    mg_printf(c,
              "HTTP/1.1 %d %s\r\nContent-Type: %s\r\n"
              "Etag: %s\r\nContent-Length: " MG_INT64_FMT "\r\n%s%s\r\n",
              status, mg_http_status_code_str(status), mime, etag, cl, range,
              hdrs ? hdrs : "");
    if (mg_vcasecmp(&hm->method, "HEAD") == 0) {
      fclose(fp);
    } else {
      c->pfn = static_cb;
      c->pfn_data = fp;
    }
  }
}

#if MG_ARCH == MG_ARCH_ESP32 || MG_ARCH == MG_ARCH_ESP8266 || \
    MG_ARCH == MG_ARCH_FREERTOS_TCP
char *realpath(const char *src, char *dst) {
  int len = strlen(src);
  if (len > MG_PATH_MAX - 1) len = MG_PATH_MAX - 1;
  strncpy(dst, src, len);
  dst[len] = '\0';
  LOG(LL_DEBUG, ("[%s] -> [%s]", src, dst));
  return dst;
}
#endif

// Allow user to override this function
bool mg_is_dir(const char *path) WEAK;
bool mg_is_dir(const char *path) {
#if MG_ARCH == MG_ARCH_FREERTOS_TCP && defined(MG_ENABLE_FF)
  struct FF_STAT st;
  return (ff_stat(path, &st) == 0) && (st.st_mode & FF_IFDIR);
#else
  mg_stat_t st;
  return mg_stat(path, &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

#if MG_ENABLE_DIRECTORY_LISTING

#ifdef _WIN32
struct dirent {
  char d_name[MAX_PATH];
};

typedef struct win32_dir {
  HANDLE handle;
  WIN32_FIND_DATAW info;
  struct dirent result;
} DIR;

int gettimeofday(struct timeval *tv, void *tz) {
  FILETIME ft;
  unsigned __int64 tmpres = 0;

  if (tv != NULL) {
    GetSystemTimeAsFileTime(&ft);
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
    tmpres /= 10;  // convert into microseconds
    tmpres -= (int64_t) 11644473600000000;
    tv->tv_sec = (long) (tmpres / 1000000UL);
    tv->tv_usec = (long) (tmpres % 1000000UL);
  }
  (void) tz;
  return 0;
}

static int to_wchar(const char *path, wchar_t *wbuf, size_t wbuf_len) {
  int ret;
  char buf[MAX_PATH * 2], buf2[MAX_PATH * 2], *p;
  strncpy(buf, path, sizeof(buf));
  buf[sizeof(buf) - 1] = '\0';
  // Trim trailing slashes. Leave backslash for paths like "X:\"
  p = buf + strlen(buf) - 1;
  while (p > buf && p[-1] != ':' && (p[0] == '\\' || p[0] == '/')) *p-- = '\0';
  memset(wbuf, 0, wbuf_len * sizeof(wchar_t));
  ret = MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, (int) wbuf_len);
  // Convert back to Unicode. If doubly-converted string does not match the
  // original, something is fishy, reject.
  WideCharToMultiByte(CP_UTF8, 0, wbuf, (int) wbuf_len, buf2, sizeof(buf2),
                      NULL, NULL);
  if (strcmp(buf, buf2) != 0) {
    wbuf[0] = L'\0';
    ret = 0;
  }
  return ret;
}

DIR *opendir(const char *name) {
  DIR *d = NULL;
  wchar_t wpath[MAX_PATH];
  DWORD attrs;

  if (name == NULL) {
    SetLastError(ERROR_BAD_ARGUMENTS);
  } else if ((d = (DIR *) calloc(1, sizeof(*d))) == NULL) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
  } else {
    to_wchar(name, wpath, sizeof(wpath) / sizeof(wpath[0]));
    attrs = GetFileAttributesW(wpath);
    if (attrs != 0Xffffffff && (attrs & FILE_ATTRIBUTE_DIRECTORY)) {
      (void) wcscat(wpath, L"\\*");
      d->handle = FindFirstFileW(wpath, &d->info);
      d->result.d_name[0] = '\0';
    } else {
      free(d);
      d = NULL;
    }
  }
  return d;
}

int closedir(DIR *d) {
  int result = 0;
  if (d != NULL) {
    if (d->handle != INVALID_HANDLE_VALUE)
      result = FindClose(d->handle) ? 0 : -1;
    free(d);
  } else {
    result = -1;
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}

struct dirent *readdir(DIR *d) {
  struct dirent *result = NULL;
  if (d != NULL) {
    memset(&d->result, 0, sizeof(d->result));
    if (d->handle != INVALID_HANDLE_VALUE) {
      result = &d->result;
      WideCharToMultiByte(CP_UTF8, 0, d->info.cFileName, -1, result->d_name,
                          sizeof(result->d_name), NULL, NULL);
      if (!FindNextFileW(d->handle, &d->info)) {
        FindClose(d->handle);
        d->handle = INVALID_HANDLE_VALUE;
      }
    } else {
      SetLastError(ERROR_FILE_NOT_FOUND);
    }
  } else {
    SetLastError(ERROR_BAD_ARGUMENTS);
  }
  return result;
}
#endif

static void printdirentry(struct mg_connection *c, const char *name,
                          mg_stat_t *stp) {
  char size[64], mod[64], path[MG_PATH_MAX];
  int is_dir = S_ISDIR(stp->st_mode), n = 0;
  const char *slash = is_dir ? "/" : "";
  struct tm t;

  if (is_dir) {
    snprintf(size, sizeof(size), "%s", "[DIR]");
  } else {
    if (stp->st_size < 1024) {
      snprintf(size, sizeof(size), "%d", (int) stp->st_size);
    } else if (stp->st_size < 0x100000) {
      snprintf(size, sizeof(size), "%.1fk", (double) stp->st_size / 1024.0);
    } else if (stp->st_size < 0x40000000) {
      snprintf(size, sizeof(size), "%.1fM", (double) stp->st_size / 1048576);
    } else {
      snprintf(size, sizeof(size), "%.1fG", (double) stp->st_size / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime_r(&stp->st_mtime, &t));
  n = (int) mg_url_encode(name, strlen(name), path, sizeof(path));
  mg_printf(c,
            "  <tr><td><a href=\"%.*s%s\">%s%s</a></td>"
            "<td>%s</td><td>%s</td></tr>\n",
            n, path, slash, name, slash, mod, size);
}

static void listdir(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_http_serve_opts *opts, char *dir) {
  char path[MG_PATH_MAX], *p = &dir[strlen(dir) - 1], tmp[10];
  struct dirent *dp;
  DIR *dirp;
  static const char *sort_js_code =
      "<script>function srt(tb, sc, so, d) {"
      "var tr = Array.prototype.slice.call(tb.rows, 0),"
      "tr = tr.sort(function (a, b) { var c1 = a.cells[sc], c2 = b.cells[sc],"
      "n1 = c1.getAttribute('name'), n2 = c2.getAttribute('name'), "
      "t1 = a.cells[2].getAttribute('name'), "
      "t2 = b.cells[2].getAttribute('name'); "
      "return so * (t1 < 0 && t2 >= 0 ? -1 : t2 < 0 && t1 >= 0 ? 1 : "
      "n1 ? parseInt(n2) - parseInt(n1) : "
      "c1.textContent.trim().localeCompare(c2.textContent.trim())); });";
  static const char *sort_js_code2 =
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

  while (p > dir && *p != '/') *p-- = '\0';
  if ((dirp = (opendir(dir))) != NULL) {
    size_t off, n;
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
              (int) hm->uri.len, hm->uri.ptr, sort_js_code, sort_js_code2,
              (int) hm->uri.len, hm->uri.ptr);

    while ((dp = readdir(dirp)) != NULL) {
      mg_stat_t st;
      const char *sep = dp->d_name[0] == MG_DIRSEP ? "/" : "";
      // Do not show current dir and hidden files
      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) continue;
      // SPIFFS can report "/foo.txt" in the dp->d_name
      if (snprintf(path, sizeof(path), "%s%s%s", dir, sep, dp->d_name) < 0) {
        LOG(LL_ERROR, ("%s truncated", dp->d_name));
      } else if (mg_stat(path, &st) != 0) {
        LOG(LL_ERROR, ("%lu stat(%s): %d", c->id, path, errno));
      } else {
        printdirentry(c, dp->d_name, &st);
      }
    }
    closedir(dirp);
    mg_printf(c,
              "</tbody><tfoot><tr><td colspan=\"3\"><hr></td></tr></tfoot>"
              "</table><address>Mongoose v.%s</address></body></html>\n",
              MG_VERSION);
    n = (size_t) snprintf(tmp, sizeof(tmp), "%lu",
                          (unsigned long) (c->send.len - off));
    if (n > sizeof(tmp)) n = 0;
    memcpy(c->send.buf + off - 10, tmp, n);  // Set content length
  } else {
    mg_http_reply(c, 400, "", "Cannot open dir");
    LOG(LL_ERROR, ("%lu opendir(%s) -> %d", c->id, dir, errno));
  }
}
#endif

void mg_http_serve_dir(struct mg_connection *c, struct mg_http_message *hm,
                       struct mg_http_serve_opts *opts) {
  char t1[MG_PATH_MAX], t2[sizeof(t1)];
  t1[0] = t2[0] = '\0';

  if (realpath(opts->root_dir, t1) == NULL) {
    LOG(LL_ERROR, ("realpath(%s): %d", opts->root_dir, errno));
    mg_http_reply(c, 400, "", "Bad web root [%s]\n", opts->root_dir);
  } else if (!mg_is_dir(t1)) {
    mg_http_reply(c, 400, "", "Invalid web root [%s]\n", t1);
  } else {
    // NOTE(lsm): Xilinx snprintf does not 0-terminate the detination for
    // the %.*s specifier, if the length is zero. Make sure hm->uri.len > 0
    bool is_index = false;
    size_t n1 = strlen(t1), n2;

    mg_url_decode(hm->uri.ptr, hm->uri.len, t1 + n1, sizeof(t1) - n1, 0);
    t1[sizeof(t1) - 1] = '\0';
    n2 = strlen(t1);
    while (n2 > 0 && t1[n2 - 1] == '/') t1[--n2] = 0;

    if (realpath(t1, t2) == NULL) {
      LOG(LL_ERROR, ("realpath(%s): %d", t1, errno));
      mg_http_reply(c, 404, "", "Not found [%.*s]\n", (int) hm->uri.len,
                    hm->uri.ptr);
      return;
    }

    if (mg_is_dir(t2)) {
      strncat(t2, "/index.html", sizeof(t2) - strlen(t2) - 1);
      t2[sizeof(t2) - 1] = '\0';
      is_index = true;
    }

    if (strlen(t2) < n1 || memcmp(t1, t2, n1) != 0) {
      // Requested file is located outside root directory, fail
      mg_http_reply(c, 404, "", "Invalid URI [%.*s]\n", (int) hm->uri.len,
                    hm->uri.ptr);
    } else {
      FILE *fp = mg_fopen(t2, "r");
#if MG_ENABLE_SSI
      if (is_index && fp == NULL) {
        char *p = t2 + strlen(t2);
        while (p > t2 && p[-1] != '/') p--;
        strncpy(p, "index.shtml", (size_t)(&t2[sizeof(t2)] - p - 2));
        t2[sizeof(t2) - 1] = '\0';
        fp = mg_fopen(t2, "r");
      }
#endif
      if (is_index && fp == NULL) {
#if MG_ENABLE_DIRECTORY_LISTING
        listdir(c, hm, opts, t2);
#else
        mg_http_reply(c, 403, "", "%s", "Directory listing not supported");
#endif
#if MG_ENABLE_SSI
      } else if (opts->ssi_pattern != NULL &&
                 mg_globmatch(opts->ssi_pattern, strlen(opts->ssi_pattern), t2,
                              strlen(t2))) {
        t1[n1] = '\0';
        mg_http_serve_ssi(c, t1, t2);
#endif
      } else {
        mg_http_serve_file(c, hm, t2, guess_content_type(t2),
                           opts->extra_headers);
      }
      if (fp != NULL) fclose(fp);
    }
  }
}
#endif

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
      snprintf(user, userlen, "%.*s", (int) (p - buf), buf);
      snprintf(pass, passlen, "%.*s", n - (int) (p - buf) - 1, p + 1);
    }
  } else if (v != NULL && v->len > 7 && memcmp(v->ptr, "Bearer ", 7) == 0) {
    snprintf(pass, passlen, "%.*s", (int) v->len - 7, v->ptr + 7);
  } else if ((v = mg_http_get_header(hm, "Cookie")) != NULL) {
    struct mg_str t = mg_http_get_header_var(*v, mg_str_n("access_token", 12));
    if (t.len > 0) snprintf(pass, passlen, "%.*s", (int) t.len, t.ptr);
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
  for (i = 0; i + v.len + 2 < s.len; i++) {
    if (s.ptr[i + v.len] == '=' && memcmp(&s.ptr[i], v.ptr, v.len) == 0) {
      const char *p = &s.ptr[i + v.len + 1], *b = p, *x = &s.ptr[s.len];
      int q = p < x && *p == '"' ? 1 : 0;
      while (p < x && (q ? p == b || *p != '"' : *p != ';' && *p != ' ')) p++;
      // LOG(LL_INFO, ("[%.*s] [%.*s] [%.*s]", (int) s.len, s.ptr, (int) v.len,
      // v.ptr, (int) (p - b), b));
      return stripquotes(mg_str_n(b, (size_t)(p - b + q)));
    }
  }
  return mg_str_n(NULL, 0);
}

bool mg_http_match_uri(const struct mg_http_message *hm, const char *glob) {
  return mg_globmatch(glob, strlen(glob), hm->uri.ptr, hm->uri.len);
}

static size_t get_chunk_length(const char *buf, size_t len, size_t *ll) {
  size_t i = 0, n;
  while (i < len && buf[i] != '\r' && i != '\n') i++;
  n = mg_unhexn((char *) buf, i);
  while (i < len && (buf[i] == '\r' || i == '\n')) i++;
  // LOG(LL_INFO, ("len %zu i %zu n %zu ", len, i, n));
  if (ll != NULL) *ll = i + 1;
  if (i < len && i + n + 2 < len) return i + n + 3;
  return 0;
}

// Walk through all chunks in the chunked body. For each chunk, fire
// an MG_EV_HTTP_CHUNK event.
static void walkchunks(struct mg_connection *c, struct mg_http_message *hm,
                       size_t reqlen) {
  size_t off = 0, bl, ll;
  while (off + reqlen < c->recv.len) {
    char *buf = (char *) &c->recv.buf[reqlen];
    size_t memo = c->recv.len;
    size_t cl = get_chunk_length(&buf[off], memo - reqlen - off, &ll);
    // LOG(LL_INFO, ("len %zu off %zu cl %zu ll %zu", len, off, cl, ll));
    if (cl == 0) break;
    hm->chunk = mg_str_n(&buf[off + ll], cl < ll + 2 ? 0 : cl - ll - 2);
    mg_call(c, MG_EV_HTTP_CHUNK, hm);
    // Increase offset only if user has not deleted this chunk
    if (memo == c->recv.len) off += cl;
    if (cl <= 5) {
      // Zero chunk - last one. Prepare body - cut off chunk lengths
      off = bl = 0;
      while (off + reqlen < c->recv.len) {
        char *buf2 = (char *) &c->recv.buf[reqlen];
        size_t memo2 = c->recv.len;
        size_t cl2 = get_chunk_length(&buf2[off], memo2 - reqlen - off, &ll);
        size_t n = cl < ll + 2 ? 0 : cl2 - ll - 2;
        memmove(buf2 + bl, buf2 + off + ll, n);
        bl += n;
        off += cl2;
        if (cl2 <= 5) break;
      }
      // LOG(LL_INFO, ("BL->%d del %d off %d", (int) bl, (int) del, (int) off));
      c->recv.len -= off - bl;
      // Set message length to indicate we've received
      // everything, to fire MG_EV_HTTP_MSG
      hm->message.len = bl + reqlen;
      hm->body.len = bl;
      break;
    }
  }
}

static bool mg_is_chunked(struct mg_http_message *hm) {
  struct mg_str needle = mg_str_n("chunked", 7);
  struct mg_str *te = mg_http_get_header(hm, "Transfer-Encoding");
  return te != NULL && mg_strstr(*te, needle) != NULL;
}

void mg_http_delete_chunk(struct mg_connection *c, struct mg_http_message *hm) {
  struct mg_str ch = hm->chunk;
  if (mg_is_chunked(hm)) {
    ch.len += 4;  // \r\n before and after the chunk
    ch.ptr -= 2;
    while (ch.ptr > hm->body.ptr && *ch.ptr != '\n') ch.ptr--, ch.len++;
  }
  {
    const char *end = &ch.ptr[ch.len];
    size_t n = (size_t)(end - (char *) c->recv.buf);
    if (c->recv.len > n) {
      memmove((char *) ch.ptr, end, (size_t)(c->recv.len - n));
    }
    // LOG(LL_INFO, ("DELETING CHUNK: %zu %zu %zu\n%.*s", c->recv.len, n,
    // ch.len, (int) ch.len, ch.ptr));
  }
  c->recv.len -= ch.len;
}

static void http_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ || ev == MG_EV_CLOSE) {
    struct mg_http_message hm;
    for (;;) {
      int n = mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
      bool is_chunked = n > 0 && mg_is_chunked(&hm);
      if (ev == MG_EV_CLOSE) {
        hm.message.len = c->recv.len;
        hm.body.len = hm.message.len - (size_t)(hm.body.ptr - hm.message.ptr);
      } else if (is_chunked && n > 0) {
        walkchunks(c, &hm, (size_t) n);
      }
      // LOG(LL_INFO,
      //("---->%d %d\n%.*s", n, is_chunked, (int) c->recv.len, c->recv.buf));
      if (n < 0 && ev == MG_EV_READ) {
        mg_error(c, "HTTP parse:\n%.*s", (int) c->recv.len, c->recv.buf);
        break;
      } else if (n > 0 && (size_t) c->recv.len >= hm.message.len) {
        mg_call(c, MG_EV_HTTP_MSG, &hm);
        mg_iobuf_delete(&c->recv, hm.message.len);
      } else {
        if (n > 0 && !is_chunked) {
          hm.chunk =
              mg_str_n((char *) &c->recv.buf[n], c->recv.len - (size_t) n);
          mg_call(c, MG_EV_HTTP_CHUNK, &hm);
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
