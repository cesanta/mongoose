#include "arch.h"
#include "base64.h"
#include "fmt.h"
#include "http.h"
#include "json.h"
#include "log.h"
#include "net.h"
#include "printf.h"
#include "ssi.h"
#include "util.h"
#include "version.h"
#include "ws.h"

bool mg_to_size_t(struct mg_str str, size_t *val);
bool mg_to_size_t(struct mg_str str, size_t *val) {
  size_t i = 0, max = (size_t) -1, max2 = max / 10, result = 0, ndigits = 0;
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (i < str.len && str.ptr[i] == '-') return false;
  while (i < str.len && str.ptr[i] >= '0' && str.ptr[i] <= '9') {
    size_t digit = (size_t) (str.ptr[i] - '0');
    if (result > max2) return false;  // Overflow
    result *= 10;
    if (result > max - digit) return false;  // Overflow
    result += digit;
    i++, ndigits++;
  }
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (ndigits == 0) return false;  // #2322: Content-Length = 1 * DIGIT
  if (i != str.len) return false;  // Ditto
  *val = (size_t) result;
  return true;
}

// Chunk deletion marker is the MSB in the "processed" counter
#define MG_DMARK ((size_t) 1 << (sizeof(size_t) * 8 - 1))

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
    size_t i, n = 0;
    char *buf = (char *) &c->send.buf[c->send.len];
    memcpy(buf, "Authorization: Basic ", 21);  // DON'T use mg_send!
    for (i = 0; i < u.len; i++) {
      n = mg_base64_update(((unsigned char *) u.ptr)[i], buf + 21, n);
    }
    if (p.len > 0) {
      n = mg_base64_update(':', buf + 21, n);
      for (i = 0; i < p.len; i++) {
        n = mg_base64_update(((unsigned char *) p.ptr)[i], buf + 21, n);
      }
    }
    n = mg_base64_final(buf + 21, n);
    c->send.len += 21 + (size_t) n + 2;
    memcpy(&c->send.buf[c->send.len - 2], "\r\n", 2);
  } else {
    MG_ERROR(("%lu oom %d->%d ", c->id, (int) c->send.size, (int) need));
  }
}

struct mg_str mg_http_var(struct mg_str buf, struct mg_str name) {
  struct mg_str k, v, result = mg_str_n(NULL, 0);
  while (mg_split(&buf, &k, &v, '&')) {
    if (name.len == k.len && mg_ncasecmp(name.ptr, k.ptr, k.len) == 0) {
      result = v;
      break;
    }
  }
  return result;
}

int mg_http_get_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len) {
  int len;
  if (dst == NULL || dst_len == 0) {
    len = -2;  // Bad destination
  } else if (buf->ptr == NULL || name == NULL || buf->len == 0) {
    len = -1;  // Bad source
    dst[0] = '\0';
  } else {
    struct mg_str v = mg_http_var(*buf, mg_str(name));
    if (v.ptr == NULL) {
      len = -4;  // Name does not exist
    } else {
      len = mg_url_decode(v.ptr, v.len, dst, dst_len, 1);
      if (len < 0) len = -3;  // Failed to decode
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
struct mg_str *mg_http_get_header(struct mg_http_message *h, const char *name) {
  size_t i, n = strlen(name), max = sizeof(h->headers) / sizeof(h->headers[0]);
  for (i = 0; i < max && h->headers[i].name.len > 0; i++) {
    struct mg_str *k = &h->headers[i].name, *v = &h->headers[i].value;
    if (n == k->len && mg_ncasecmp(k->ptr, name, n) == 0) return v;
  }
  return NULL;
}

// Get character length. Used to parse method, URI, headers
static size_t clen(const char *s) {
  uint8_t c = *(uint8_t *) s;
  if (c > ' ' && c < '~') return 1;  // Usual ascii printed char
  if ((c & 0xe0) == 0xc0) return 2;  // 2-byte UTF8
  if ((c & 0xf0) == 0xe0) return 3;  // 3-byte UTF8
  if ((c & 0xf8) == 0xf0) return 4;  // 4-byte UTF8
  return 0;
}

// Skip until the newline. Return advanced `s`, or NULL on error
static const char *skiptorn(const char *s, const char *end, struct mg_str *v) {
  v->ptr = s;
  while (s < end && s[0] != '\n' && s[0] != '\r') s++, v->len++;  // To newline
  if (s >= end || (s[0] == '\r' && s[1] != '\n')) return NULL;    // Stray \r
  if (s < end && s[0] == '\r') s++;                               // Skip \r
  if (s >= end || *s++ != '\n') return NULL;                      // Skip \n
  return s;
}

static bool mg_http_parse_headers(const char *s, const char *end,
                                  struct mg_http_header *h, size_t max_hdrs) {
  size_t i, n;
  for (i = 0; i < max_hdrs; i++) {
    struct mg_str k = {NULL, 0}, v = {NULL, 0};
    if (s >= end) return false;
    if (s[0] == '\n' || (s[0] == '\r' && s[1] == '\n')) break;
    k.ptr = s;
    while (s < end && s[0] != ':' && (n = clen(s)) > 0) s += n, k.len += n;
    if (k.len == 0) return false;               // Empty name
    if (s >= end || *s++ != ':') return false;  // Invalid, not followed by :
    while (s < end && s[0] == ' ') s++;         // Skip spaces
    if ((s = skiptorn(s, end, &v)) == NULL) return false;
    while (v.len > 0 && v.ptr[v.len - 1] == ' ') v.len--;  // Trim spaces
    // MG_INFO(("--HH [%.*s] [%.*s]", (int) k.len, k.ptr, (int) v.len, v.ptr));
    h[i].name = k, h[i].value = v;  // Success. Assign values
  }
  return true;
}

int mg_http_parse(const char *s, size_t len, struct mg_http_message *hm) {
  int is_response, req_len = mg_http_get_request_len((unsigned char *) s, len);
  const char *end = s == NULL ? NULL : s + req_len, *qs;  // Cannot add to NULL
  struct mg_str *cl;
  size_t n;

  memset(hm, 0, sizeof(*hm));
  if (req_len <= 0) return req_len;

  hm->message.ptr = hm->head.ptr = s;
  hm->body.ptr = end;
  hm->head.len = (size_t) req_len;
  hm->message.len = hm->body.len = (size_t) -1;  // Set body length to infinite

  // Parse request line
  hm->method.ptr = s;
  while (s < end && (n = clen(s)) > 0) s += n, hm->method.len += n;
  while (s < end && s[0] == ' ') s++;  // Skip spaces
  hm->uri.ptr = s;
  while (s < end && (n = clen(s)) > 0) s += n, hm->uri.len += n;
  while (s < end && s[0] == ' ') s++;  // Skip spaces
  if ((s = skiptorn(s, end, &hm->proto)) == NULL) return false;

  // If URI contains '?' character, setup query string
  if ((qs = (const char *) memchr(hm->uri.ptr, '?', hm->uri.len)) != NULL) {
    hm->query.ptr = qs + 1;
    hm->query.len = (size_t) (&hm->uri.ptr[hm->uri.len] - (qs + 1));
    hm->uri.len = (size_t) (qs - hm->uri.ptr);
  }

  // Sanity check. Allow protocol/reason to be empty
  // Do this check after hm->method.len and hm->uri.len are finalised
  if (hm->method.len == 0 || hm->uri.len == 0) return -1;

  if (!mg_http_parse_headers(s, end, hm->headers,
                             sizeof(hm->headers) / sizeof(hm->headers[0])))
    return -1;  // error when parsing
  if ((cl = mg_http_get_header(hm, "Content-Length")) != NULL) {
    if (mg_to_size_t(*cl, &hm->body.len) == false) return -1;
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
  if (hm->message.len < (size_t) req_len) return -1;  // Overflow protection

  return req_len;
}

static void mg_http_vprintf_chunk(struct mg_connection *c, const char *fmt,
                                  va_list *ap) {
  size_t len = c->send.len;
  mg_send(c, "        \r\n", 10);
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, ap);
  if (c->send.len >= len + 10) {
    mg_snprintf((char *) c->send.buf + len, 9, "%08lx", c->send.len - len - 10);
    c->send.buf[len + 8] = '\r';
    if (c->send.len == len + 10) c->is_resp = 0;  // Last chunk, reset marker
  }
  mg_send(c, "\r\n", 2);
}

void mg_http_printf_chunk(struct mg_connection *c, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  mg_http_vprintf_chunk(c, fmt, &ap);
  va_end(ap);
}

void mg_http_write_chunk(struct mg_connection *c, const char *buf, size_t len) {
  mg_printf(c, "%lx\r\n", (unsigned long) len);
  mg_send(c, buf, len);
  mg_send(c, "\r\n", 2);
  if (len == 0) c->is_resp = 0;
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
    default: return "";
  }
}
// clang-format on

void mg_http_reply(struct mg_connection *c, int code, const char *headers,
                   const char *fmt, ...) {
  va_list ap;
  size_t len;
  mg_printf(c, "HTTP/1.1 %d %s\r\n%sContent-Length:            \r\n\r\n", code,
            mg_http_status_code_str(code), headers == NULL ? "" : headers);
  len = c->send.len;
  va_start(ap, fmt);
  mg_vxprintf(mg_pfn_iobuf, &c->send, fmt, &ap);
  va_end(ap);
  if (c->send.len > 16) {
    size_t n = mg_snprintf((char *) &c->send.buf[len - 15], 11, "%-10lu",
                           (unsigned long) (c->send.len - len));
    c->send.buf[len - 15 + n] = ' ';  // Change ending 0 to space
  }
  c->is_resp = 0;
}

static void http_cb(struct mg_connection *, int, void *, void *);
static void restore_http_cb(struct mg_connection *c) {
  mg_fs_close((struct mg_fd *) c->pfn_data);
  c->pfn_data = NULL;
  c->pfn = http_cb;
  c->is_resp = 0;
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
    size_t n, max = MG_IO_SIZE, space;
    size_t *cl = (size_t *) &c->data[(sizeof(c->data) - sizeof(size_t)) /
                                     sizeof(size_t) * sizeof(size_t)];
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

// Known mime types. Keep it outside guess_content_type() function, since
// some environments don't like it defined there.
// clang-format off
static struct mg_str s_known_types[] = {
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

static struct mg_str guess_content_type(struct mg_str path, const char *extra) {
  struct mg_str k, v, s = mg_str(extra);
  size_t i = 0;

  // Shrink path to its extension only
  while (i < path.len && path.ptr[path.len - i - 1] != '.') i++;
  path.ptr += path.len - i;
  path.len = i;

  // Process user-provided mime type overrides, if any
  while (mg_commalist(&s, &k, &v)) {
    if (mg_strcmp(path, k) == 0) return v;
  }

  // Process built-in mime types
  for (i = 0; s_known_types[i].ptr != NULL; i += 2) {
    if (mg_strcmp(path, s_known_types[i]) == 0) return s_known_types[i + 1];
  }

  return mg_str("text/plain; charset=utf-8");
}

static int getrange(struct mg_str *s, size_t *a, size_t *b) {
  size_t i, numparsed = 0;
  for (i = 0; i + 6 < s->len; i++) {
    struct mg_str k, v = mg_str_n(s->ptr + i + 6, s->len - i - 6);
    if (memcmp(&s->ptr[i], "bytes=", 6) != 0) continue;
    if (mg_split(&v, &k, NULL, '-')) {
      if (mg_to_size_t(k, a)) numparsed++;
      if (v.len > 0 && mg_to_size_t(v, b)) numparsed++;
    } else {
      if (mg_to_size_t(v, a)) numparsed++;
    }
    break;
  }
  return (int) numparsed;
}

void mg_http_serve_file(struct mg_connection *c, struct mg_http_message *hm,
                        const char *path,
                        const struct mg_http_serve_opts *opts) {
  char etag[64], tmp[MG_PATH_MAX];
  struct mg_fs *fs = opts->fs == NULL ? &mg_fs_posix : opts->fs;
  struct mg_fd *fd = NULL;
  size_t size = 0;
  time_t mtime = 0;
  struct mg_str *inm = NULL;
  struct mg_str mime = guess_content_type(mg_str(path), opts->mime_types);
  bool gzip = false;

  if (path != NULL) {
    // If a browser sends us "Accept-Encoding: gzip", try to open .gz first
    struct mg_str *ae = mg_http_get_header(hm, "Accept-Encoding");
    if (ae != NULL && mg_strstr(*ae, mg_str("gzip")) != NULL) {
      mg_snprintf(tmp, sizeof(tmp), "%s.gz", path);
      fd = mg_fs_open(fs, tmp, MG_FS_READ);
      if (fd != NULL) gzip = true, path = tmp;
    }
    // No luck opening .gz? Open what we've told to open
    if (fd == NULL) fd = mg_fs_open(fs, path, MG_FS_READ);
  }

  // Failed to open, and page404 is configured? Open it, then
  if (fd == NULL && opts->page404 != NULL) {
    fd = mg_fs_open(fs, opts->page404, MG_FS_READ);
    mime = guess_content_type(mg_str(path), opts->mime_types);
    path = opts->page404;
  }

  if (fd == NULL || fs->st(path, &size, &mtime) == 0) {
    mg_http_reply(c, 404, opts->extra_headers, "Not found\n");
    mg_fs_close(fd);
    // NOTE: mg_http_etag() call should go first!
  } else if (mg_http_etag(etag, sizeof(etag), size, mtime) != NULL &&
             (inm = mg_http_get_header(hm, "If-None-Match")) != NULL &&
             mg_vcasecmp(inm, etag) == 0) {
    mg_fs_close(fd);
    mg_http_reply(c, 304, opts->extra_headers, "");
  } else {
    int n, status = 200;
    char range[100];
    size_t r1 = 0, r2 = 0, cl = size;

    // Handle Range header
    struct mg_str *rh = mg_http_get_header(hm, "Range");
    range[0] = '\0';
    if (rh != NULL && (n = getrange(rh, &r1, &r2)) > 0) {
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
                    "Content-Range: bytes %llu-%llu/%llu\r\n", (uint64_t) r1,
                    (uint64_t) (r1 + cl - 1), (uint64_t) size);
        fs->sk(fd->fd, r1);
      }
    }
    mg_printf(c,
              "HTTP/1.1 %d %s\r\n"
              "Content-Type: %.*s\r\n"
              "Etag: %s\r\n"
              "Content-Length: %llu\r\n"
              "%s%s%s\r\n",
              status, mg_http_status_code_str(status), (int) mime.len, mime.ptr,
              etag, (uint64_t) cl, gzip ? "Content-Encoding: gzip\r\n" : "",
              range, opts->extra_headers ? opts->extra_headers : "");
    if (mg_vcasecmp(&hm->method, "HEAD") == 0) {
      c->is_draining = 1;
      c->is_resp = 0;
      mg_fs_close(fd);
    } else {
      // Track to-be-sent content length at the end of c->data, aligned
      size_t *clp = (size_t *) &c->data[(sizeof(c->data) - sizeof(size_t)) /
                                        sizeof(size_t) * sizeof(size_t)];
      c->pfn = static_cb;
      c->pfn_data = fd;
      *clp = cl;
    }
  }
}

struct printdirentrydata {
  struct mg_connection *c;
  struct mg_http_message *hm;
  const struct mg_http_serve_opts *opts;
  const char *dir;
};

#if MG_ENABLE_DIRLIST
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
#if defined(MG_HTTP_DIRLIST_TIME_FMT)
    {
      char time_str[40];
      struct tm *time_info = localtime(&t);
      strftime(time_str, sizeof time_str, "%Y/%m/%d %H:%M:%S", time_info);
      mg_snprintf(mod, sizeof(mod), "%s", time_str);
    }
#else
    mg_snprintf(mod, sizeof(mod), "%lu", (unsigned long) t);
#endif
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
  c->is_resp = 0;                          // Mark response end
}
#endif

// Resolve requested file into `path` and return its fs->st() result
static int uri_to_path2(struct mg_connection *c, struct mg_http_message *hm,
                        struct mg_fs *fs, struct mg_str url, struct mg_str dir,
                        char *path, size_t path_size) {
  int flags, tmp;
  // Append URI to the root_dir, and sanitize it
  size_t n = mg_snprintf(path, path_size, "%.*s", (int) dir.len, dir.ptr);
  if (n + 2 >= path_size) {
    mg_http_reply(c, 400, "", "Exceeded path size");
    return -1;
  }
  path[path_size - 1] = '\0';
  // Terminate root dir with slash
  if (n > 0 && path[n - 1] != '/') path[n++] = '/', path[n] = '\0';
  if (url.len < hm->uri.len) {
    mg_url_decode(hm->uri.ptr + url.len, hm->uri.len - url.len, path + n,
                  path_size - n, 0);
  }
  path[path_size - 1] = '\0';  // Double-check
  if (!mg_path_is_sane(path)) {
    mg_http_reply(c, 400, "", "Invalid path");
    return -1;
  }
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
    c->is_resp = 0;
    flags = -1;
  } else if (flags & MG_FS_DIR) {
    if (((mg_snprintf(path + n, path_size - n, "/" MG_HTTP_INDEX) > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0) ||
         (mg_snprintf(path + n, path_size - n, "/index.shtml") > 0 &&
          (tmp = fs->st(path, NULL, NULL)) != 0))) {
      flags = tmp;
    } else if ((mg_snprintf(path + n, path_size - n, "/" MG_HTTP_INDEX ".gz") >
                    0 &&
                (tmp = fs->st(path, NULL, NULL)) !=
                    0)) {  // check for gzipped index
      flags = tmp;
      path[n + 1 + strlen(MG_HTTP_INDEX)] =
          '\0';  // Remove appended .gz in index file name
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
    if (v.len == 0) v = k, k = mg_str("/"), u = k, p = v;
    if (hm->uri.len < k.len) continue;
    if (mg_strcmp(k, mg_str_n(hm->uri.ptr, k.len)) != 0) continue;
    u = k, p = v;
  }
  return uri_to_path2(c, hm, fs, u, p, path, path_size);
}

void mg_http_serve_dir(struct mg_connection *c, struct mg_http_message *hm,
                       const struct mg_http_serve_opts *opts) {
  char path[MG_PATH_MAX];
  const char *sp = opts->ssi_pattern;
  int flags = uri_to_path(c, hm, opts, path, sizeof(path));
  if (flags < 0) {
    // Do nothing: the response has already been sent by uri_to_path()
  } else if (flags & MG_FS_DIR) {
#if MG_ENABLE_DIRLIST
    listdir(c, hm, opts, path);
#else
    mg_http_reply(c, 403, "", "Forbidden\n");
#endif
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
  if (len > 0 && n < len - 1) buf[n] = '\0';  // Null-terminate the destination
  if (len > 0) buf[len - 1] = '\0';           // Always.
  return n;
}

void mg_http_creds(struct mg_http_message *hm, char *user, size_t userlen,
                   char *pass, size_t passlen) {
  struct mg_str *v = mg_http_get_header(hm, "Authorization");
  user[0] = pass[0] = '\0';
  if (v != NULL && v->len > 6 && memcmp(v->ptr, "Basic ", 6) == 0) {
    char buf[256];
    size_t n = mg_base64_decode(v->ptr + 6, v->len - 6, buf, sizeof(buf));
    const char *p = (const char *) memchr(buf, ':', n > 0 ? n : 0);
    if (p != NULL) {
      mg_snprintf(user, userlen, "%.*s", p - buf, buf);
      mg_snprintf(pass, passlen, "%.*s", n - (size_t) (p - buf) - 1, p + 1);
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

long mg_http_upload(struct mg_connection *c, struct mg_http_message *hm,
                    struct mg_fs *fs, const char *path, size_t max_size) {
  char buf[20] = "0";
  long res = 0, offset;
  mg_http_get_var(&hm->query, "offset", buf, sizeof(buf));
  offset = strtol(buf, NULL, 0);
  if (hm->body.len == 0) {
    mg_http_reply(c, 200, "", "%ld", res);  // Nothing to write
  } else {
    struct mg_fd *fd;
    size_t current_size = 0;
    MG_DEBUG(("%s -> %d bytes @ %ld", path, (int) hm->body.len, offset));
    if (offset == 0) fs->rm(path);  // If offset if 0, truncate file
    fs->st(path, &current_size, NULL);
    if (offset < 0) {
      mg_http_reply(c, 400, "", "offset required");
      res = -1;
    } else if (offset > 0 && current_size != (size_t) offset) {
      mg_http_reply(c, 400, "", "%s: offset mismatch", path);
      res = -2;
    } else if ((size_t) offset + hm->body.len > max_size) {
      mg_http_reply(c, 400, "", "%s: over max size of %lu", path,
                    (unsigned long) max_size);
      res = -3;
    } else if ((fd = mg_fs_open(fs, path, MG_FS_WRITE)) == NULL) {
      mg_http_reply(c, 400, "", "open(%s): %d", path, errno);
      res = -4;
    } else {
      res = offset + (long) fs->wr(fd->fd, hm->body.ptr, hm->body.len);
      mg_fs_close(fd);
      mg_http_reply(c, 200, "", "%ld", res);
    }
  }
  return res;
}

int mg_http_status(const struct mg_http_message *hm) {
  return atoi(hm->uri.ptr);
}

static bool is_hex_digit(int c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

static int skip_chunk(const char *buf, int len, int *pl, int *dl) {
  int i = 0, n = 0;
  if (len < 3) return 0;
  while (i < len && is_hex_digit(buf[i])) i++;
  if (len < i + 1 || buf[i] != '\r' || buf[i + 1] != '\n') return -1;  // Error
  n = (int) mg_unhexn(buf, (size_t) i);  // Decode hex length
  if (n < 0) return -1;                  // Error
  if (len < i + n + 4) return 0;         // Chunk not yet fully buffered
  if (buf[i + n + 2] != '\r' || buf[i + n + 3] != '\n') return -1;  // Error
  *pl = i + 2, *dl = n;
  return i + 2 + n + 2;
}

static bool is_chunked(struct mg_http_message *hm) {
  const char *needle = "chunked";
  struct mg_str *te = mg_http_get_header(hm, "Transfer-Encoding");
  return te != NULL && mg_vcasecmp(te, needle) == 0;
}

static void http_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ || ev == MG_EV_CLOSE) {
    struct mg_http_message hm;
    size_t ofs = 0;  // Parsing offset

    while (c->is_resp == 0 && ofs < c->recv.len) {
      const char *buf = (char *) c->recv.buf + ofs;
      int n = mg_http_parse(buf, c->recv.len - ofs, &hm);
      if (n < 0) {
        mg_error(c, "HTTP parse");
        return;
      }
      if (n == 0) break;        // Request is not buffered yet
      if (ev == MG_EV_CLOSE) {  // If client did not set Content-Length
        hm.message.len = c->recv.len - ofs;  // and closes now, deliver MSG
        hm.body.len = hm.message.len - (size_t) (hm.body.ptr - hm.message.ptr);
      }

      if (is_chunked(&hm)) {
        // For chunked data, strip off prefixes and suffixes from chunks
        // and relocate them right after the headers, then report a message
        char *s = (char *) c->recv.buf + ofs + n;
        int o = 0, pl, dl, cl, len = (int) (c->recv.len - ofs - (size_t) n);

        // Find zero-length chunk (the end of the body)
        while ((cl = skip_chunk(s + o, len - o, &pl, &dl)) > 0 && dl) o += cl;
        if (cl == 0) break;  // No zero-len chunk, buffer more data
        if (cl < 0) {
          mg_error(c, "Invalid chunk");
          break;
        }

        // Zero chunk found. Second pass: strip + relocate
        o = 0, hm.body.len = 0, hm.message.len = (size_t) n;
        while ((cl = skip_chunk(s + o, len - o, &pl, &dl)) > 0) {
          memmove(s + hm.body.len, s + o + pl, (size_t) dl);
          o += cl, hm.body.len += (size_t) dl, hm.message.len += (size_t) dl;
          if (dl == 0) break;
        }
        ofs += (size_t) (n + o);
      } else { // Normal, non-chunked data
        size_t len = c->recv.len - ofs - (size_t) n;
        if (hm.body.len > len) break;  // Buffer more data
        ofs += (size_t) n + hm.body.len;
      }

      if (c->is_accepted) c->is_resp = 1;  // Start generating response
      mg_call(c, MG_EV_HTTP_MSG, &hm);     // User handler can clear is_resp
    }
    if (ofs > 0) mg_iobuf_del(&c->recv, 0, ofs);  // Delete processed data
  }
  (void) evd, (void) fnd;
}

static void mg_hfn(struct mg_connection *c, int ev, void *ev_data, void *fnd) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/quit")) {
      mg_http_reply(c, 200, "", "ok\n");
      c->is_draining = 1;
      c->data[0] = 'X';
    } else if (mg_http_match_uri(hm, "/debug")) {
      int level = (int) mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
      mg_log_set(level);
      mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
    } else {
      mg_http_reply(c, 200, "", "hi\n");
    }
  } else if (ev == MG_EV_CLOSE) {
    if (c->data[0] == 'X') *(bool *) fnd = true;
  }
}

void mg_hello(const char *url) {
  struct mg_mgr mgr;
  bool done = false;
  mg_mgr_init(&mgr);
  if (mg_http_listen(&mgr, url, mg_hfn, &done) == NULL) done = true;
  while (done == false) mg_mgr_poll(&mgr, 100);
  mg_mgr_free(&mgr);
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
