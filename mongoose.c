// Copyright (c) 2004-2013 Sergey Lyubka
// Copyright (c) 2013-2021 Cesanta Software Limited
// All rights reserved
//
// This software is dual-licensed: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. For the terms of this
// license, see <http://www.gnu.org/licenses/>.
//
// You are free to use this software under the terms of the GNU General
// Public License, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// Alternatively, you can license this software under a commercial
// license, as set out in <https://www.cesanta.com/license>.

#include "mongoose.h"

#ifdef MG_ENABLE_LINES
#line 1 "src/private.h"
#endif
void mg_connect_resolved(struct mg_connection *);

#if MG_ARCH == MG_ARCH_FREERTOS
static inline void *mg_calloc(int cnt, size_t size) {
  void *p = pvPortMalloc(size);
  if (p != NULL) memset(p, 0, size);
  return p;
}
#define calloc(a, b) mg_calloc((a), (b))
#define malloc(a) pvPortMalloc(a)
#define free(a) vPortFree(a)
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/base64.c"
#endif
#include <string.h>

static int mg_b64idx(int c) {
  if (c < 26) {
    return c + 'A';
  } else if (c < 52) {
    return c - 26 + 'a';
  } else if (c < 62) {
    return c - 52 + '0';
  } else {
    return c == 62 ? '+' : '/';
  }
}

static int mg_b64rev(int c) {
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
  } else if (c == '=') {
    return 64;
  } else {
    return -1;
  }
}

int mg_base64_update(unsigned char ch, char *to, int n) {
  unsigned char rem = (n & 3) % 3;
  if (rem == 0) {
    to[n] = mg_b64idx(ch >> 2);
    to[++n] = (ch & 3) << 4;
  } else if (rem == 1) {
    to[n] = mg_b64idx(to[n] | (ch >> 4));
    to[++n] = (ch & 15) << 2;
  } else {
    to[n] = mg_b64idx(to[n] | (ch >> 6));
    to[++n] = mg_b64idx(ch & 63);
    n++;
  }
  return n;
}

int mg_base64_final(char *to, int n) {
  int saved = n;
  // printf("---[%.*s]\n", n, to);
  if (n & 3) n = mg_base64_update(0, to, n);
  if ((saved & 3) == 2) n--;
  // printf("    %d[%.*s]\n", n, n, to);
  while (n & 3) to[n++] = '=';
  to[n] = '\0';
  return n;
}

int mg_base64_encode(const unsigned char *p, int n, char *to) {
  int i, len = 0;
  for (i = 0; i < n; i++) len = mg_base64_update(p[i], to, len);
  len = mg_base64_final(to, len);
  return len;
}

int mg_base64_decode(const char *src, int n, char *dst) {
  const char *end = src + n;
  int len = 0;
  while (src + 3 < end) {
    int a = mg_b64rev(src[0]), b = mg_b64rev(src[1]), c = mg_b64rev(src[2]),
        d = mg_b64rev(src[3]);
    if (a == 64 || a < 0 || b == 64 || b < 0 || c < 0 || d < 0) return 0;
    dst[len++] = (a << 2) | (b >> 4);
    if (src[2] != '=') {
      dst[len++] = (b << 4) | (c >> 2);
      if (src[3] != '=') dst[len++] = (c << 6) | d;
    }
    src += 4;
  }
  dst[len] = '\0';
  return len;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/dns.c"
#endif







struct dns_data {
  struct dns_data *next;
  struct mg_connection *c;
  unsigned long expire;
  uint16_t txnid;
};

static struct dns_data *s_reqs;  // Active DNS requests

static void mg_sendnsreq(struct mg_connection *, struct mg_str *, int,
                         struct mg_dns *, bool);

static void mg_dns_free(struct dns_data *d) {
  LIST_DELETE(struct dns_data, &s_reqs, d);
  free(d);
}

void mg_resolve_cancel(struct mg_connection *c) {
  struct dns_data *tmp, *d;
  for (d = s_reqs; d != NULL; d = tmp) {
    tmp = d->next;
    if (d->c == c) mg_dns_free(d);
  }
}

static size_t mg_dns_parse_name_depth(const uint8_t *s, size_t len, size_t ofs,
                                      char *to, size_t tolen, int depth) {
  size_t i = 0, j = 0;
  if (tolen > 0) to[0] = '\0';
  if (depth > 5) return 0;
  while (ofs + i + 1 < len) {
    size_t n = s[ofs + i];
    if (n == 0) {
      i++;
      break;
    }
    if (n & 0xc0) {
      size_t ptr = (((n & 0x3f) << 8) | s[ofs + i + 1]);  // 12 is hdr len
      if (ptr + 1 < len && (s[ptr] & 0xc0) == 0 &&
          mg_dns_parse_name_depth(s, len, ptr, to, tolen, depth + 1) == 0)
        return 0;
      i += 2;
      break;
    }
    if (ofs + i + n + 1 >= len) return 0;
    if (j > 0) {
      if (j < tolen) to[j] = '.';
      j++;
    }
    if (j + n < tolen) memcpy(&to[j], &s[ofs + i + 1], n);
    j += n;
    i += n + 1;
    if (j < tolen) to[j] = '\0';  // Zero-terminate this chunk
  }
  if (tolen > 0) to[tolen - 1] = '\0';  // Make sure make sure it is nul-term
  return i;
}

size_t mg_dns_parse_name(const uint8_t *s, size_t n, size_t ofs, char *dst,
                         size_t dstlen) {
  return mg_dns_parse_name_depth(s, n, ofs, dst, dstlen, 0);
}

size_t mg_dns_parse_rr(const uint8_t *buf, size_t len, size_t ofs,
                       bool is_question, struct mg_dns_rr *rr) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  const uint8_t *s = buf + ofs, *e = &buf[len];

  memset(rr, 0, sizeof(*rr));
  if (len < sizeof(*h)) return 0;  // Too small, headers dont fit
  if (len > 512) return 0;         //  Too large, we don't expect that
  if (s >= e) return 0;            //  Overflow

  if ((rr->nlen = (uint16_t) mg_dns_parse_name(buf, len, ofs, NULL, 0)) == 0)
    return 0;
  s += rr->nlen + 4;
  if (s > e) return 0;
  rr->atype = ((uint16_t) s[-4] << 8) | s[-3];
  rr->aclass = ((uint16_t) s[-2] << 8) | s[-1];
  if (is_question) return rr->nlen + 4;

  s += 6;
  if (s > e) return 0;
  rr->alen = ((uint16_t) s[-2] << 8) | s[-1];
  if (s + rr->alen > e) return 0;
  return rr->nlen + rr->alen + 10;
}

bool mg_dns_parse(const uint8_t *buf, size_t len, struct mg_dns_message *dm) {
  const struct mg_dns_header *h = (struct mg_dns_header *) buf;
  struct mg_dns_rr rr;
  size_t i, n, ofs = sizeof(*h);
  memset(dm, 0, sizeof(*dm));

  if (len < sizeof(*h)) return 0;                // Too small, headers dont fit
  if (mg_ntohs(h->num_questions) > 1) return 0;  // Sanity
  if (mg_ntohs(h->num_answers) > 10) return 0;   // Sanity
  dm->txnid = mg_ntohs(h->txnid);

  for (i = 0; i < mg_ntohs(h->num_questions); i++) {
    if ((n = mg_dns_parse_rr(buf, len, ofs, true, &rr)) == 0) return false;
    // LOG(LL_INFO, ("Q %zu %zu", ofs, n));
    ofs += n;
  }
  for (i = 0; i < mg_ntohs(h->num_answers); i++) {
    // LOG(LL_INFO, ("A -- %zu %zu %s", ofs, n, dm->name));
    if ((n = mg_dns_parse_rr(buf, len, ofs, false, &rr)) == 0) return false;
    mg_dns_parse_name(buf, len, ofs, dm->name, sizeof(dm->name));
    ofs += n;

    if (rr.alen == 4 && rr.atype == 1 && rr.aclass == 1) {
      dm->addr.is_ip6 = false;
      memcpy(&dm->addr.ip, &buf[ofs - 4], 4);
      dm->resolved = true;
      break;  // Return success
    } else if (rr.alen == 16 && rr.atype == 28 && rr.aclass == 1) {
      dm->addr.is_ip6 = true;
      memcpy(&dm->addr.ip6, &buf[ofs - 16], 16);
      dm->resolved = true;
      break;  // Return success
    }
  }
  return true;
}

static void dns_cb(struct mg_connection *c, int ev, void *ev_data,
                   void *fn_data) {
  struct dns_data *d, *tmp;
  if (ev == MG_EV_POLL) {
    unsigned long now = *(unsigned long *) ev_data;
    for (d = s_reqs; d != NULL; d = tmp) {
      tmp = d->next;
      // LOG(LL_DEBUG, ("%lu %lu dns poll", d->expire, now));
      if (now > d->expire) mg_error(d->c, "DNS timeout");
    }
  } else if (ev == MG_EV_READ) {
    struct mg_dns_message dm;
    int resolved = 0;
    if (mg_dns_parse(c->recv.buf, c->recv.len, &dm) == false) {
      char *s = mg_hexdump(c->recv.buf, c->recv.len);
      LOG(LL_ERROR, ("Unexpected DNS response:\n%s\n", s));
      free(s);
    } else {
      LOG(LL_VERBOSE_DEBUG, ("%s %d", dm.name, dm.resolved));
      for (d = s_reqs; d != NULL; d = tmp) {
        tmp = d->next;
        // LOG(LL_INFO, ("d %p %hu %hu", d, d->txnid, dm.txnid));
        if (dm.txnid != d->txnid) continue;
        if (d->c->is_resolving) {
          d->c->is_resolving = 0;
          if (dm.resolved) {
#if MG_ENABLE_LOG
            char buf[100];
#endif
            dm.addr.port = d->c->peer.port;  // Save port
            d->c->peer = dm.addr;            // Copy resolved address
            LOG(LL_DEBUG, ("%lu %s resolved to %s", d->c->id, dm.name,
                           mg_ntoa(&d->c->peer, buf, sizeof(buf))));
            mg_connect_resolved(d->c);
#if MG_ENABLE_IPV6
          } else if (dm.addr.is_ip6 == false && dm.name[0] != '\0') {
            struct mg_str x = mg_str(dm.name);
            mg_sendnsreq(d->c, &x, c->mgr->dnstimeout, &c->mgr->dns6, true);
#endif
          } else {
            mg_error(d->c, "%s DNS lookup failed", dm.name);
          }
        } else {
          LOG(LL_ERROR, ("%lu already resolved", d->c->id));
        }
        mg_dns_free(d);
        resolved = 1;
      }
    }
    if (!resolved) LOG(LL_ERROR, ("stray DNS reply"));
    c->recv.len = 0;
  } else if (ev == MG_EV_CLOSE) {
    for (d = s_reqs; d != NULL; d = tmp) {
      tmp = d->next;
      mg_dns_free(d);
    }
  }
  (void) fn_data;
}

void mg_dns_send(struct mg_connection *c, const struct mg_str *name,
                 uint16_t txnid, bool ipv6) {
  struct {
    struct mg_dns_header header;
    uint8_t data[256];
  } pkt;
  size_t i, n;
  memset(&pkt, 0, sizeof(pkt));
  pkt.header.txnid = mg_htons(txnid);
  pkt.header.flags = mg_htons(0x100);
  pkt.header.num_questions = mg_htons(1);
  for (i = n = 0; i < sizeof(pkt.data) - 5; i++) {
    if (name->ptr[i] == '.' || i >= name->len) {
      pkt.data[n] = (uint8_t)(i - n);
      memcpy(&pkt.data[n + 1], name->ptr + n, i - n);
      n = i + 1;
    }
    if (i >= name->len) break;
  }
  memcpy(&pkt.data[n], "\x00\x00\x01\x00\x01", 5);  // A query
  n += 5;
  if (ipv6) pkt.data[n - 3] = 0x1c;  // AAAA query
  // memcpy(&pkt.data[n], "\xc0\x0c\x00\x1c\x00\x01", 6);  // AAAA query
  // n += 6;
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#if 0
  // Immediately after A query, send AAAA query. Whatever reply comes first,
  // we'll use it. Note: we cannot send two queries in a single packet.
  // https://stackoverflow.com/questions/4082081/requesting-a-and-aaaa-records-in-single-dns-query
  pkt.data[n - 3] = 0x1c;  // AAAA query
  mg_send(c, &pkt, sizeof(pkt.header) + n);
#endif
}

static void mg_sendnsreq(struct mg_connection *c, struct mg_str *name, int ms,
                         struct mg_dns *dnsc, bool ipv6) {
  struct dns_data *d = NULL;
  if (dnsc->url == NULL) {
    mg_error(c, "DNS server URL is NULL. Call mg_mgr_init()");
  } else if (dnsc->c == NULL) {
    dnsc->c = mg_connect(c->mgr, dnsc->url, NULL, NULL);
    if (dnsc->c != NULL) {
      dnsc->c->pfn = dns_cb;
      snprintf(dnsc->c->label, sizeof(dnsc->c->label), "%s", "DNS");
      // dnsc->c->is_hexdumping = 1;
    }
  }
  if (dnsc->c == NULL) {
    mg_error(c, "resolver");
  } else if ((d = (struct dns_data *) calloc(1, sizeof(*d))) == NULL) {
    mg_error(c, "resolve OOM");
  } else {
    d->txnid = s_reqs ? s_reqs->txnid + 1 : 1;
    d->next = s_reqs;
    s_reqs = d;
    d->expire = mg_millis() + ms;
    d->c = c;
    c->is_resolving = 1;
    LOG(LL_VERBOSE_DEBUG, ("%lu resolving %.*s, txnid %hu", c->id,
                           (int) name->len, name->ptr, d->txnid));
    mg_dns_send(dnsc->c, name, d->txnid, ipv6);
  }
}

void mg_resolve(struct mg_connection *c, struct mg_str *name, int ms) {
  if (mg_aton(*name, &c->peer)) {
    // name is an IP address, do not fire name resolution
    mg_connect_resolved(c);
  } else {
    // name is not an IP, send DNS resolution request
    mg_sendnsreq(c, name, ms, &c->mgr->dns4, false);
  }
}

#ifdef MG_ENABLE_LINES
#line 1 "src/event.c"
#endif





void mg_call(struct mg_connection *c, int ev, void *ev_data) {
  if (c->pfn != NULL) c->pfn(c, ev, ev_data, c->pfn_data);
  if (c->fn != NULL) c->fn(c, ev, ev_data, c->fn_data);
}

void mg_error(struct mg_connection *c, const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  va_start(ap, fmt);
  mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  LOG(LL_ERROR, ("%lu %s", c->id, buf));
  mg_call(c, MG_EV_ERROR, buf);
  if (buf != mem) free(buf);
  c->is_closing = 1;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/http.c"
#endif











// Multipart POST example:
// https://gist.github.com/cpq/b8dd247571e6ee9c54ef7e8dfcfecf48

void mg_http_bauth(struct mg_connection *c, const char *user,
                   const char *pass) {
  struct mg_str u = mg_str(user), p = mg_str(pass);
  size_t need = c->send.len + 36 + (u.len + p.len) * 2;
  if (c->send.size < need) mg_iobuf_resize(&c->send, need);
  if (c->send.size >= need) {
    size_t i, n = 0;
    char *buf = (char *) &c->send.buf[c->send.len + 21];
    memcpy(&buf[-21], "Authorization: Basic ", 21);  // DON'T use mg_send!
    for (i = 0; i < u.len; i++) n = mg_base64_update(u.ptr[i], buf, n);
    if (p.len > 0) {
      n = mg_base64_update(':', buf, n);
      for (i = 0; i < p.len; i++) n = mg_base64_update(p.ptr[i], buf, n);
    }
    n = mg_base64_final(buf, n);
    c->send.len += 21 + n + 2;
    memcpy(&c->send.buf[c->send.len - 2], "\r\n", 2);
  } else {
    LOG(LL_ERROR, ("%lu %s cannot resize iobuf %d->%d ", c->id, c->label,
                   (int) c->send.size, (int) need));
  }
}

int mg_http_get_var(const struct mg_str *buf, const char *name, char *dst,
                    int dst_len) {
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
  v->len = s - v->ptr;
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
  hm->head.len = req_len;
  hm->chunk.ptr = end;
  hm->message.len = hm->body.len = (size_t) ~0;  // Set body length to infinite

  // Parse request line
  s = skip(s, end, " ", &hm->method);
  s = skip(s, end, " ", &hm->uri);
  s = skip(s, end, "\r\n", &hm->proto);

  // Sanity check
  if (hm->method.len == 0 || hm->uri.len == 0 || hm->proto.len == 0) return -1;

  // If URI contains '?' character, setup query string
  if ((qs = (const char *) memchr(hm->uri.ptr, '?', hm->uri.len)) != NULL) {
    hm->query.ptr = qs + 1;
    hm->query.len = &hm->uri.ptr[hm->uri.len] - (qs + 1);
    hm->uri.len = qs - hm->uri.ptr;
  }

  mg_http_parse_headers(s, end, hm->headers,
                        sizeof(hm->headers) / sizeof(hm->headers[0]));
  if ((cl = mg_http_get_header(hm, "Content-Length")) != NULL) {
    hm->body.len = (size_t) mg_to64(*cl);
    hm->message.len = req_len + hm->body.len;
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
    hm->message.len = req_len;
  }

  // The 204 (No content) responses also have 0 body length
  if (hm->body.len == (size_t) ~0 && is_response &&
      mg_vcasecmp(&hm->uri, "204") == 0) {
    hm->body.len = 0;
    hm->message.len = req_len;
  }

  return req_len;
}

static void mg_http_vprintf_chunk(struct mg_connection *c, const char *fmt,
                                  va_list ap) {
  char mem[256], *buf = mem;
  int len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  mg_printf(c, "%X\r\n", len);
  mg_send(c, buf, len);
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
  mg_printf(c, "%X\r\n", len);
  mg_send(c, buf, len);
  mg_send(c, "\r\n", 2);
}

void mg_http_reply(struct mg_connection *c, int code, const char *headers,
                   const char *fmt, ...) {
  char mem[100], *buf = mem;
  va_list ap;
  int len;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  mg_printf(c, "HTTP/1.1 %d OK\r\n%sContent-Length: %d\r\n\r\n", code,
            headers == NULL ? "" : headers, len);
  mg_send(c, buf, len);
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
      return hm->body.len;
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
    mg_printf(c,
              "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n"
              "Etag: %s\r\nContent-Length: " MG_INT64_FMT "\r\n%s\r\n",
              mime, etag, (int64_t) st.st_size, hdrs ? hdrs : "");
    if (mg_vcasecmp(&hm->method, "HEAD") == 0) {
      fclose(fp);
    } else {
      c->pfn = static_cb;
      c->pfn_data = fp;
    }
  }
}

#if MG_ARCH == MG_ARCH_ESP32 || MG_ARCH == MG_ARCH_ESP8266 || \
    MG_ARCH == MG_ARCH_FREERTOS
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
#if MG_ARCH == MG_ARCH_FREERTOS
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
  } else if ((d = (DIR *) malloc(sizeof(*d))) == NULL) {
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
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&stp->st_mtime));
  n = mg_url_encode(name, strlen(name), path, sizeof(path));
  mg_printf(c,
            "  <tr><td><a href=\"%.*s%s\">%s%s</a></td>"
            "<td>%s</td><td>%s</td></tr>\n",
            n, path, slash, name, slash, mod, size);
}

static void listdir(struct mg_connection *c, struct mg_http_message *hm,
                    char *dir) {
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
    mg_printf(c, "%s\r\n",
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html; charset=utf-8\r\n"
              "Content-Length:         \r\n");
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
    n = snprintf(tmp, sizeof(tmp), "%lu", (unsigned long) (c->send.len - off));
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
        strncpy(p, "index.shtml", &t2[sizeof(t2)] - p - 2);
        t2[sizeof(t2) - 1] = '\0';
        fp = mg_fopen(t2, "r");
      }
#endif
      if (is_index && fp == NULL) {
#if MG_ENABLE_DIRECTORY_LISTING
        listdir(c, hm, t2);
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
        mg_http_serve_file(c, hm, t2, guess_content_type(t2), NULL);
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

int mg_url_encode(const char *s, size_t sl, char *buf, size_t len) {
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

void mg_http_creds(struct mg_http_message *hm, char *user, int userlen,
                   char *pass, int passlen) {
  struct mg_str *v = mg_http_get_header(hm, "Authorization");
  user[0] = pass[0] = '\0';
  if (v != NULL && v->len > 6 && memcmp(v->ptr, "Basic ", 6) == 0) {
    char buf[256];
    int n = mg_base64_decode(v->ptr + 6, (int) v->len - 6, buf);
    const char *p = (const char *) memchr(buf, ':', n);
    if (p != NULL) {
      snprintf(user, userlen, "%.*s", (int) (p - buf), buf);
      snprintf(pass, passlen, "%.*s", n - (int) (p - buf) - 1, p + 1);
    }
  } else if (v != NULL && v->len > 7 && memcmp(v->ptr, "Bearer ", 7) == 0) {
    snprintf(pass, passlen, "%.*s", (int) v->len - 7, v->ptr + 7);
  } else if ((v = mg_http_get_header(hm, "Cookie")) != NULL) {
    size_t i;
    for (i = 0; i < v->len - 13; i++) {
      if (memcmp(&v->ptr[i], "access_token=", 13) == 0) {
        const char *p2 = v->ptr + i + 13, *p3 = p2;
        while (p2 < &v->ptr[v->len] && p2[0] != ';' && p2[0] != ' ') p2++;
        snprintf(pass, passlen, "%.*s", (int) (p2 - p3), p3);
        break;
      }
    }
  } else {
    mg_http_get_var(&hm->query, "access_token", pass, passlen);
  }
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
                       int reqlen) {
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
        char *buf = (char *) &c->recv.buf[reqlen];
        size_t memo = c->recv.len;
        size_t cl = get_chunk_length(&buf[off], memo - reqlen - off, &ll);
        size_t n = cl < ll + 2 ? 0 : cl - ll - 2;
        if (cl <= 5) break;
        memmove(buf + bl, buf + off + ll, n);
        bl += n;
        off += cl;
      }
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
    size_t n = end - (char *) c->recv.buf;
    if (c->recv.len > n) memmove((char *) ch.ptr, end, c->recv.len - n);
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
        hm.body.len = hm.message.len - (hm.body.ptr - hm.message.ptr);
      } else if (is_chunked) {
        walkchunks(c, &hm, n);
      }
      // LOG(LL_INFO,
      //("---->%d %d\n%.*s", n, is_chunked, (int) c->recv.len, c->recv.buf));
      if (n < 0 && ev == MG_EV_READ) {
        LOG(LL_ERROR, ("%lu HTTP parse error", c->id));
        c->is_closing = 1;
        break;
      } else if (n > 0 && (size_t) c->recv.len >= hm.message.len) {
        mg_call(c, MG_EV_HTTP_MSG, &hm);
        mg_iobuf_delete(&c->recv, hm.message.len);
      } else {
        if (n > 0 && !is_chunked) {
          hm.chunk = mg_str_n((char *) &c->recv.buf[n], c->recv.len - n);
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

#ifdef MG_ENABLE_LINES
#line 1 "src/iobuf.c"
#endif



#include <string.h>

int mg_iobuf_resize(struct mg_iobuf *io, size_t new_size) {
  int ok = 1;
  if (new_size == 0) {
    free(io->buf);
    io->buf = NULL;
    io->len = io->size = 0;
  } else if (new_size != io->size) {
    // NOTE(lsm): do not use realloc here. Use malloc/free only, to ease the
    // porting to some obscure platforms like FreeRTOS
    void *p = malloc(new_size);
    if (p != NULL) {
      memcpy(p, io->buf, io->size < new_size ? io->size : new_size);
      free(io->buf);
      io->buf = (unsigned char *) p;
      io->size = new_size;
    } else {
      ok = 0;
      LOG(LL_ERROR,
          ("%lu->%lu", (unsigned long) io->size, (unsigned long) new_size));
    }
  }
  return ok;
}

int mg_iobuf_init(struct mg_iobuf *io, size_t size) {
  return mg_iobuf_resize(io, size);
}

size_t mg_iobuf_append(struct mg_iobuf *io, const void *buf, size_t len,
                       size_t chunk_size) {
  size_t new_size = io->len + len + chunk_size;
  new_size -= new_size % chunk_size;
  if (new_size != io->size) mg_iobuf_resize(io, new_size);
  if (new_size != io->size) len = 0;  // Realloc failure, append nothing
  if (buf != NULL) memmove(io->buf + io->len, buf, len);
  io->len += len;
  return len;
}

size_t mg_iobuf_delete(struct mg_iobuf *io, size_t len) {
  if (len > io->len) len = 0;
  memmove(io->buf, io->buf + len, io->len - len);
  io->len -= len;
  return len;
}

void mg_iobuf_free(struct mg_iobuf *io) {
  mg_iobuf_resize(io, 0);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/log.c"
#endif



#if MG_ENABLE_MGOS
#else
#if MG_ENABLE_LOG
static void mg_log_stdout(const void *buf, int len, void *userdata) {
  (void) userdata;
  fwrite(buf, 1, len, stdout);
}

static const char *s_spec = "2";
static void (*s_fn)(const void *, int, void *) = mg_log_stdout;
static void *s_fn_param = NULL;

void mg_log_set(const char *spec) {
  LOG(LL_DEBUG, ("Setting log level to %s", spec));
  s_spec = spec;
}

bool mg_log_prefix(int level, const char *file, int line, const char *fname) {
  // static unsigned long seq;
  int max = LL_INFO;
  struct mg_str k, v, s = mg_str(s_spec);
  const char *p = strrchr(file, '/');
  p = p == NULL ? file : p + 1;

  if (s_fn == NULL) return false;
  while (mg_next_comma_entry(&s, &k, &v)) {
    if (v.len == 0) max = atoi(k.ptr);
    if (v.len > 0 && strncmp(p, k.ptr, k.len) == 0) max = atoi(v.ptr);
  }

  if (level <= max) {
    char timebuf[21], buf[50] = "";
    time_t t = time(NULL);
    struct tm *tm = gmtime(&t);
    int n, tag;
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);
    tag = level == LL_ERROR ? 'E' : level == LL_INFO ? 'I' : ' ';
    n = snprintf(buf, sizeof(buf), "%s  %c %s:%d:%s", timebuf, tag, p, line,
                 fname);
    if (n < 0 || n > (int) sizeof(buf) - 2) n = sizeof(buf) - 2;
    while (n < (int) sizeof(buf) - 1) buf[n++] = ' ';
    s_fn(buf, sizeof(buf) - 1, s_fn_param);
    return true;
  } else {
    return false;
  }
}

void mg_log(const char *fmt, ...) {
  char mem[256], *buf = mem;
  va_list ap;
  int len = 0;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);
  s_fn(buf, len, s_fn_param);
  s_fn("\n", 1, s_fn_param);
  if (buf != mem) free(buf);
}

void mg_log_set_callback(void (*fn)(const void *, int, void *), void *param) {
  s_fn = fn;
  s_fn_param = param;
}
#endif
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/lwip.c"
#endif







#if MG_ENABLE_LWIP
#include <lwip/tcp.h>
#include <lwip/udp.h>

static void tcp_error_cb(void *arg, err_t err) {
  struct mg_connection *c = (struct mg_connection *) arg;
  mg_error(c, "%p err %ld", c->fd, err);
}

static err_t connect_cb(void *arg, struct tcp_pcb *pcb, err_t err) {
  struct mg_connection *c = (struct mg_connection *) arg;
  LOG(LL_DEBUG, ("err %ld, arg %p, pcb %p", err, arg, pcb));
  c->is_connecting = 0;
  if (err != ERR_OK) mg_error(c, "%p err %d", c->fd, err);
  return err;
}

static err_t tcp_recv_cb(void *arg, struct tcp_pcb *pcb, struct pbuf *p,
                         err_t err) {
  struct mg_connection *c = (struct mg_connection *) arg;
  LOG(LL_DEBUG,
      ("err %ld, pbuf %p/%d, io.len %d, io.size %d", err, p,
       p == NULL ? 0 : (int) p->len, (int) c->recv.len, (int) c->recv.size));
  if (err == ERR_OK && p != NULL) {
#if 0
    if (s->io.size < s->io.len + p->len) {
      char *buf = realloc(s->io.buf, s->io.len + p->len);
      if (buf != NULL) {
        s->io.buf = buf;
        s->io.size = s->io.len + p->len;
      } else {
        return ERR_MEM;
      }
    }
    // MLOG(LL_DEBUG, " --> cpy, %p %p", s->io.buf, p->payload);
    memcpy(s->io.buf + s->io.len, p->payload, p->len);
    s->io.len += p->len;
#endif
    tcp_recved(pcb, p->len);
    pbuf_free(p);
    return err;
  } else {
    // rmsock(s);
    return ERR_ABRT;
  }
}

static void udp_recv_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                        const ip_addr_t *addr, uint16_t port) {
  LOG(LL_DEBUG,
      ("%p %p pbuf %p/%d port %hu", arg, pcb, p, p == NULL ? 0 : p->len, port));
}

static err_t tcp_sent_cb(void *arg, struct tcp_pcb *pcb, uint16_t len) {
  LOG(LL_DEBUG, ("%p %d", pcb, (int) len));
  return ERR_OK;
}

#if 0
static int ll_write(struct mg_connection *c, const void *buf, int len,
                    int *fail) {
  int n = c->is_tls ? mg_tls_send(c, buf, len, fail)
                    : mg_sock_send(c, buf, len, fail);
  LOG(*fail ? LL_ERROR : LL_VERBOSE_DEBUG,
      ("%p %c%c%c %d/%d %d", c->fd, c->is_tls ? 'T' : 't',
       c->is_udp ? 'U' : 'u', c->is_connecting ? 'C' : 'c', n, len,
       MG_SOCK_ERRNO));
  if (n > 0 && c->is_hexdumping) mg_hexdump(c, "->", buf, n);
  return n;
}
#endif

int mg_send(struct mg_connection *c, const void *buf, size_t len) {
  if (c->is_udp) {
    struct udp_pcb *pcb = (struct udp_pcb *) c->fd;
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
    if (p != NULL) {
      // p->payload = (void *) buf;
      memcpy(p->payload, buf, len);
      err_t err = udp_send(pcb, p);
      pbuf_free(p);
      LOG(LL_DEBUG,
          ("%lu UDP %d bytes -> %x:%hu, err %ld", c->id, (int) len,
           (unsigned) *(uint32_t *) &pcb->remote_ip, pcb->remote_port, err));
      if (err != ERR_OK) mg_error(c, "%p err %d", c->fd, err);
    } else {
      mg_error(c, "%p pbuf OOM", c->fd);
    }
  } else if (c->is_connecting) {
    mg_iobuf_append(&c->send, buf, len, MG_IO_SIZE);
  } else {
  }
#if 0
  int fail, n = c->is_udp
                    ? ll_write(c, buf, (SOCKET) len, &fail)
                    : mg_iobuf_append(&c->send, buf, len, MG_IO_SIZE);
  return n;
#endif
  return 0;
}

static struct mg_connection *mg_mkconn(const char *url) {
  struct mg_connection *c = (struct mg_connection *) calloc(1, sizeof(*c));
  typedef void *(*new_t)(void);
  int is_udp = strncmp(url, "udp:", 4) == 0;
  if (c == NULL) {
    LOG(LL_ERROR, ("%s %s", url, "OOM"));
  } else if ((c->fd = (is_udp ? (new_t) udp_new : (new_t) tcp_new)()) == NULL) {
    LOG(LL_ERROR, ("%s new", url));
    free(c);
    c = NULL;
  } else {
    c->is_udp = is_udp;
  }
  return c;
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_mkconn(url);
  struct mg_str host = mg_url_host(url);
  if (c == NULL) return c;
  c->next = mgr->conns;
  mgr->conns = c;
  // mg_set_non_blocking_mode((SOCKET) c->fd);
  c->is_client = 1;
  c->peer.port = mg_htons(mg_url_port(url));
  c->fn = fn;
  c->fn_data = fn_data;
  c->is_hexdumping = 1;
  if (c->is_udp) {
    udp_bind(c->fd, IP_ADDR_ANY, 0);
    udp_recv(c->fd, udp_recv_cb, c);
  } else {
    tcp_arg(c->fd, c);
    tcp_err(c->fd, tcp_error_cb);
    tcp_sent(c->fd, tcp_sent_cb);
    tcp_recv(c->fd, tcp_recv_cb);
    tcp_bind(c->fd, IP_ADDR_ANY, 0);
    tcp_nagle_disable((struct tcp_pcb *) c->fd);
  }
  LOG(LL_DEBUG, ("%lu -> %s %s", c->id, url, c->is_udp ? "UDP" : "TCP"));
  mg_resolve(mgr, c, &host, mgr->dnstimeout);
  return c;
}

void mg_connect_resolved(struct mg_connection *c) {
  char buf[40];
  ip_addr_t ipaddr;
  memcpy(&ipaddr, &c->peer.ip, sizeof(ipaddr));
  mg_call(c, MG_EV_RESOLVE, NULL);
  LOG(LL_DEBUG, ("%lu resolved to %s", c->id, mg_straddr(c, buf, sizeof(buf))));
  err_t err = c->is_udp ? udp_connect((struct udp_pcb *) c->fd, &ipaddr,
                                      mg_ntohs(c->peer.port))
                        : tcp_connect((struct tcp_pcb *) c->fd, &ipaddr,
                                      mg_ntohs(c->peer.port), connect_cb);
  if (c->is_udp) c->is_connecting = 0;
  if (err != ERR_OK) mg_error(c, "%p failed, err %d", c->fd, err);
}

static err_t accept_cb(void *arg, struct tcp_pcb *pcb, err_t e) {
  LOG(LL_DEBUG, ("%p err %ld, pcb %p", arg, e, pcb));
  return ERR_OK;
}

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_mkconn(url);
  struct mg_str host = mg_url_host(url);
  uint16_t port = mg_url_port(url);
  uint32_t ipaddr;
  err_t err;
  if (c == NULL) return c;
  mg_aton(host.ptr, &ipaddr);
  if (!mg_vcasecmp(&host, "localhost")) ipaddr = mg_htonl(0x7f000001);
  if ((err = tcp_bind(c->fd, (ip_addr_t *) &ipaddr, port)) != ERR_OK) {
    mg_error(c, "%p tcp_bind(%x:%hu) -> %ld", c->fd, ipaddr, port, err);
  } else {
    tcp_listen(c->fd);
    tcp_accept(c->fd, accept_cb);
  }
  return c;
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  LOG(LL_DEBUG, ("%p %d", mgr, ms));
  mg_usleep(200 * 1000);
  mg_timer_poll(mg_millis());
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/md5.c"
#endif
#include <string.h>


#if MG_ENABLE_MD5
#if !defined(BYTE_ORDER) && defined(__BYTE_ORDER)
#define BYTE_ORDER __BYTE_ORDER
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif /* LITTLE_ENDIAN */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN __LITTLE_ENDIAN
#endif /* BIG_ENDIAN */
#endif /* BYTE_ORDER */

static void mg_byte_reverse(unsigned char *buf, unsigned longs) {
/* Forrest: MD5 expect LITTLE_ENDIAN, swap if BIG_ENDIAN */
#if BYTE_ORDER == BIG_ENDIAN
  do {
    uint32_t t = (uint32_t)((unsigned) buf[3] << 8 | buf[2]) << 16 |
                 ((unsigned) buf[1] << 8 | buf[0]);
    *(uint32_t *) buf = t;
    buf += 4;
  } while (--longs);
#else
  (void) buf;
  (void) longs;
#endif
}

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f, w, x, y, z, data, s) \
  (w += f(x, y, z) + data, w = w << s | w >> (32 - s), w += x)

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void mg_md5_init(mg_md5_ctx *ctx) {
  ctx->buf[0] = 0x67452301;
  ctx->buf[1] = 0xefcdab89;
  ctx->buf[2] = 0x98badcfe;
  ctx->buf[3] = 0x10325476;

  ctx->bits[0] = 0;
  ctx->bits[1] = 0;
}

static void mg_md5_transform(uint32_t buf[4], uint32_t const in[16]) {
  uint32_t a, b, c, d;

  a = buf[0];
  b = buf[1];
  c = buf[2];
  d = buf[3];

  MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

void mg_md5_update(mg_md5_ctx *ctx, const unsigned char *buf, size_t len) {
  uint32_t t;

  t = ctx->bits[0];
  if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t) ctx->bits[1]++;
  ctx->bits[1] += (uint32_t) len >> 29;

  t = (t >> 3) & 0x3f;

  if (t) {
    unsigned char *p = (unsigned char *) ctx->in + t;

    t = 64 - t;
    if (len < t) {
      memcpy(p, buf, len);
      return;
    }
    memcpy(p, buf, t);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    buf += t;
    len -= t;
  }

  while (len >= 64) {
    memcpy(ctx->in, buf, 64);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    buf += 64;
    len -= 64;
  }

  memcpy(ctx->in, buf, len);
}

void mg_md5_final(mg_md5_ctx *ctx, unsigned char digest[16]) {
  unsigned count;
  unsigned char *p;
  uint32_t *a;

  count = (ctx->bits[0] >> 3) & 0x3F;

  p = ctx->in + count;
  *p++ = 0x80;
  count = 64 - 1 - count;
  if (count < 8) {
    memset(p, 0, count);
    mg_byte_reverse(ctx->in, 16);
    mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
    memset(ctx->in, 0, 56);
  } else {
    memset(p, 0, count - 8);
  }
  mg_byte_reverse(ctx->in, 14);

  a = (uint32_t *) ctx->in;
  a[14] = ctx->bits[0];
  a[15] = ctx->bits[1];

  mg_md5_transform(ctx->buf, (uint32_t *) ctx->in);
  mg_byte_reverse((unsigned char *) ctx->buf, 4);
  memcpy(digest, ctx->buf, 16);
  memset((char *) ctx, 0, sizeof(*ctx));
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/mqtt.c"
#endif









#define MQTT_CLEAN_SESSION 0x02
#define MQTT_HAS_WILL 0x04
#define MQTT_WILL_RETAIN 0x20
#define MQTT_HAS_PASSWORD 0x40
#define MQTT_HAS_USER_NAME 0x80
#define MQTT_GET_WILL_QOS(flags) (((flags) &0x18) >> 3)
#define MQTT_SET_WILL_QOS(flags, qos) (flags) = ((flags) & ~0x18) | ((qos) << 3)

enum { MQTT_OK, MQTT_INCOMPLETE, MQTT_MALFORMED };

void mg_mqtt_send_header(struct mg_connection *c, uint8_t cmd, uint8_t flags,
                         uint32_t len) {
  uint8_t buf[1 + sizeof(len)], *vlen = &buf[1];
  buf[0] = (cmd << 4) | flags;
  do {
    *vlen = len % 0x80;
    len /= 0x80;
    if (len > 0) *vlen |= 0x80;
    vlen++;
  } while (len > 0 && vlen < &buf[sizeof(buf)]);
  mg_send(c, buf, vlen - buf);
}

static void mg_send_u16(struct mg_connection *c, uint16_t value) {
  mg_send(c, &value, sizeof(value));
}

static void mqtt_login(struct mg_connection *c, const char *url,
                       struct mg_mqtt_opts *opts) {
  uint32_t total_len = 7 + 1 + 2 + 2;
  uint16_t flags = (opts->qos & 3) << 3;
  struct mg_str user = mg_url_user(url);
  struct mg_str pass = mg_url_pass(url);

  if (user.len > 0) {
    total_len += 2 + (uint32_t) user.len;
    flags |= MQTT_HAS_USER_NAME;
  }
  if (pass.len > 0) {
    total_len += 2 + (uint32_t) pass.len;
    flags |= MQTT_HAS_PASSWORD;
  }
  if (opts->will_topic.len > 0 && opts->will_message.len > 0) {
    total_len +=
        4 + (uint32_t) opts->will_topic.len + (uint32_t) opts->will_message.len;
    flags |= MQTT_HAS_WILL;
  }
  if (opts->clean || opts->client_id.len == 0) flags |= MQTT_CLEAN_SESSION;
  if (opts->will_retain) flags |= MQTT_WILL_RETAIN;
  total_len += (uint32_t) opts->client_id.len;

  mg_mqtt_send_header(c, MQTT_CMD_CONNECT, 0, total_len);
  mg_send(c, "\00\04MQTT\04", 7);
  mg_send(c, &flags, 1);
  // keepalive == 0 means "do not disconnect us!"
  mg_send_u16(c, mg_htons((uint16_t) opts->keepalive));
  mg_send_u16(c, mg_htons((uint16_t) opts->client_id.len));
  mg_send(c, opts->client_id.ptr, opts->client_id.len);
  if (flags & MQTT_HAS_WILL) {
    mg_send_u16(c, mg_htons((uint16_t) opts->will_topic.len));
    mg_send(c, opts->will_topic.ptr, opts->will_topic.len);
    mg_send_u16(c, mg_htons((uint16_t) opts->will_message.len));
    mg_send(c, opts->will_message.ptr, opts->will_message.len);
  }
  if (user.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) user.len));
    mg_send(c, user.ptr, user.len);
  }
  if (pass.len > 0) {
    mg_send_u16(c, mg_htons((uint16_t) pass.len));
    mg_send(c, pass.ptr, pass.len);
  }
}

void mg_mqtt_pub(struct mg_connection *c, struct mg_str *topic,
                 struct mg_str *data) {
  uint8_t flags = MQTT_QOS(1);
  uint32_t total_len = 2 + (uint32_t) topic->len + (uint32_t) data->len;
  LOG(LL_DEBUG, ("%lu [%.*s] -> [%.*s]", c->id, (int) topic->len,
                 (char *) topic->ptr, (int) data->len, (char *) data->ptr));
  if (MQTT_GET_QOS(flags) > 0) total_len += 2;
  mg_mqtt_send_header(c, MQTT_CMD_PUBLISH, flags, total_len);
  mg_send_u16(c, mg_htons((uint16_t) topic->len));
  mg_send(c, topic->ptr, topic->len);
  if (MQTT_GET_QOS(flags) > 0) {
    static uint16_t s_id;
    if (++s_id == 0) s_id++;
    mg_send_u16(c, mg_htons(s_id));
  }
  mg_send(c, data->ptr, data->len);
}

void mg_mqtt_sub(struct mg_connection *c, struct mg_str *topic) {
  static uint16_t s_id;
  uint8_t qos = 1;
  uint32_t total_len = 2 + (uint32_t) topic->len + 2 + 1;
  mg_mqtt_send_header(c, MQTT_CMD_SUBSCRIBE, (uint8_t) MQTT_QOS(qos),
                      total_len);
  if (++s_id == 0) ++s_id;
  mg_send_u16(c, mg_htons(s_id));
  mg_send_u16(c, mg_htons((uint16_t) topic->len));
  mg_send(c, topic->ptr, topic->len);
  mg_send(c, &qos, sizeof(qos));
}

int mg_mqtt_parse(const uint8_t *buf, size_t len, struct mg_mqtt_message *m) {
  uint8_t lc = 0, *p, *end;
  uint32_t n = 0, len_len = 0;

  memset(m, 0, sizeof(*m));
  m->dgram.ptr = (char *) buf;
  if (len < 2) return MQTT_INCOMPLETE;
  m->cmd = buf[0] >> 4;
  m->qos = (buf[0] >> 1) & 3;

  n = len_len = 0;
  p = (uint8_t *) buf + 1;
  while ((size_t)(p - buf) < len) {
    lc = *((uint8_t *) p++);
    n += (lc & 0x7f) << 7 * len_len;
    len_len++;
    if (!(lc & 0x80)) break;
    if (len_len >= 4) return MQTT_MALFORMED;
  }
  end = p + n;
  if (lc & 0x80 || end > buf + len) return MQTT_INCOMPLETE;
  m->dgram.len = end - buf;

  switch (m->cmd) {
    case MQTT_CMD_CONNACK:
      if (end - p < 2) return MQTT_MALFORMED;
      m->ack = p[1];
      break;
    case MQTT_CMD_PUBACK:
    case MQTT_CMD_PUBREC:
    case MQTT_CMD_PUBREL:
    case MQTT_CMD_PUBCOMP:
    case MQTT_CMD_SUBACK:
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (p[0] << 8) | p[1];
      break;
    case MQTT_CMD_SUBSCRIBE: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->id = (p[0] << 8) | p[1];
      p += 2;
      break;
    }
    case MQTT_CMD_PUBLISH: {
      if (p + 2 > end) return MQTT_MALFORMED;
      m->topic.len = (p[0] << 8) | p[1];
      m->topic.ptr = (char *) p + 2;
      p += 2 + m->topic.len;
      if (p > end) return MQTT_MALFORMED;
      if (m->qos > 0) {
        if (p + 2 > end) return MQTT_MALFORMED;
        m->id = (p[0] << 8) | p[1];
        p += 2;
      }
      if (p > end) return MQTT_MALFORMED;
      m->data.ptr = (char *) p;
      m->data.len = end - p;
      break;
    }
    default:
      break;
  }
  return MQTT_OK;
}

static int mg_mqtt_next_topic(struct mg_mqtt_message *msg, struct mg_str *topic,
                              uint8_t *qos, int pos) {
  unsigned char *buf = (unsigned char *) msg->dgram.ptr + pos;
  int new_pos;
  if ((size_t) pos >= msg->dgram.len) return -1;

  topic->len = buf[0] << 8 | buf[1];
  topic->ptr = (char *) buf + 2;
  new_pos = pos + 2 + topic->len + (qos == NULL ? 0 : 1);
  if ((size_t) new_pos > msg->dgram.len) return -1;
  if (qos != NULL) *qos = buf[2 + topic->len];
  return new_pos;
}

int mg_mqtt_next_sub(struct mg_mqtt_message *msg, struct mg_str *topic,
                     uint8_t *qos, int pos) {
  uint8_t tmp;
  return mg_mqtt_next_topic(msg, topic, qos == NULL ? &tmp : qos, pos);
}

int mg_mqtt_next_unsub(struct mg_mqtt_message *msg, struct mg_str *topic,
                       int pos) {
  return mg_mqtt_next_topic(msg, topic, NULL, pos);
}

static void mqtt_cb(struct mg_connection *c, int ev, void *ev_data,
                    void *fn_data) {
  if (ev == MG_EV_READ) {
    for (;;) {
      struct mg_mqtt_message mm;
      int rc = mg_mqtt_parse(c->recv.buf, c->recv.len, &mm);
      if (rc == MQTT_MALFORMED) {
        LOG(LL_ERROR, ("%lu MQTT malformed message", c->id));
        c->is_closing = 1;
        break;
      } else if (rc == MQTT_OK) {
        LOG(LL_VERBOSE_DEBUG,
            ("%p MQTT CMD %d len %d [%.*s]", c->fd, mm.cmd, (int) mm.dgram.len,
             (int) mm.data.len, mm.data.ptr));
        switch (mm.cmd) {
          case MQTT_CMD_CONNACK:
            mg_call(c, MG_EV_MQTT_OPEN, &mm.ack);
            if (mm.ack == 0) {
              LOG(LL_INFO, ("%lu Connected", c->id));
            } else {
              LOG(LL_ERROR, ("%lu MQTT auth failed, code %d", c->id, mm.ack));
              c->is_closing = 1;
            }
            break;
          case MQTT_CMD_PUBLISH: {
            LOG(LL_DEBUG, ("%lu [%.*s] -> [%.*s]", c->id, (int) mm.topic.len,
                           mm.topic.ptr, (int) mm.data.len, mm.data.ptr));
            mg_call(c, MG_EV_MQTT_MSG, &mm);
            break;
          }
        }
        mg_call(c, MG_EV_MQTT_CMD, &mm);
        mg_iobuf_delete(&c->recv, mm.dgram.len);
      } else {
        break;
      }
    }
  }
  (void) ev_data;
  (void) fn_data;
}

void mg_mqtt_ping(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGREQ, 0, 0);
}

void mg_mqtt_pong(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_PINGRESP, 0, 0);
}

void mg_mqtt_disconnect(struct mg_connection *nc) {
  mg_mqtt_send_header(nc, MQTT_CMD_DISCONNECT, 0, 0);
}

struct mg_connection *mg_mqtt_connect(struct mg_mgr *mgr, const char *url,
                                      struct mg_mqtt_opts *opts,
                                      mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    struct mg_mqtt_opts empty;
    memset(&empty, 0, sizeof(empty));
    mqtt_login(c, url, opts == NULL ? &empty : opts);
    c->pfn = mqtt_cb;
  }
  return c;
}

struct mg_connection *mg_mqtt_listen(struct mg_mgr *mgr, const char *url,
                                     mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = mg_listen(mgr, url, fn, fn_data);
  if (c != NULL) c->pfn = mqtt_cb, c->pfn_data = mgr;
  return c;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/net.c"
#endif




int mg_vprintf(struct mg_connection *c, const char *fmt, va_list ap) {
  char mem[256], *buf = mem;
  int len = mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  len = mg_send(c, buf, len);
  if (buf != mem) free(buf);
  return len;
}

int mg_printf(struct mg_connection *c, const char *fmt, ...) {
  int len = 0;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(c, fmt, ap);
  va_end(ap);
  return len;
}

char *mg_straddr(struct mg_connection *c, char *buf, size_t len) {
  char tmp[100];
  const char *fmt = c->peer.is_ip6 ? "[%s]:%d" : "%s:%d";
  mg_ntoa(&c->peer, tmp, sizeof(tmp));
  snprintf(buf, len, fmt, tmp, (int) mg_ntohs(c->peer.port));
  return buf;
}

char *mg_ntoa(const struct mg_addr *addr, char *buf, size_t len) {
  if (addr->is_ip6) {
    uint16_t *p = (uint16_t *) addr->ip6;
    snprintf(buf, len, "%x:%x:%x:%x:%x:%x:%x:%x", mg_htons(p[0]),
             mg_htons(p[1]), mg_htons(p[2]), mg_htons(p[3]), mg_htons(p[4]),
             mg_htons(p[5]), mg_htons(p[6]), mg_htons(p[7]));
  } else {
    uint8_t p[4];
    memcpy(p, &addr->ip, sizeof(p));
    snprintf(buf, len, "%d.%d.%d.%d", (int) p[0], (int) p[1], (int) p[2],
             (int) p[3]);
  }
  return buf;
}

static bool mg_atonl(struct mg_str str, struct mg_addr *addr) {
  if (mg_vcasecmp(&str, "localhost") != 0) return false;
  addr->ip = mg_htonl(0x7f000001);
  addr->is_ip6 = false;
  return true;
}

static bool mg_aton4(struct mg_str str, struct mg_addr *addr) {
  uint8_t data[4] = {0, 0, 0, 0};
  size_t i, num_dots = 0;
  for (i = 0; i < str.len; i++) {
    if (str.ptr[i] >= '0' && str.ptr[i] <= '9') {
      int octet = data[num_dots] * 10 + (str.ptr[i] - '0');
      if (octet > 255) return false;
      data[num_dots] = octet;
    } else if (str.ptr[i] == '.') {
      if (num_dots >= 3 || i == 0 || str.ptr[i - 1] == '.') return false;
      num_dots++;
    } else {
      return false;
    }
  }
  if (num_dots != 3 || str.ptr[i - 1] == '.') return false;
  memcpy(&addr->ip, data, sizeof(data));
  addr->is_ip6 = false;
  return true;
}

static bool mg_aton6(struct mg_str str, struct mg_addr *addr) {
  size_t i, j = 0, n = 0, dc = 42;
  for (i = 0; i < str.len; i++) {
    if ((str.ptr[i] >= '0' && str.ptr[i] <= '9') ||
        (str.ptr[i] >= 'a' && str.ptr[i] <= 'f') ||
        (str.ptr[i] >= 'A' && str.ptr[i] <= 'F')) {
      unsigned long val;
      if (i > j + 3) return false;
      // LOG(LL_DEBUG, ("%zu %zu [%.*s]", i, j, (int) (i - j + 1),
      // &str.ptr[j]));
      val = mg_unhexn(&str.ptr[j], i - j + 1);
      addr->ip6[n] = (uint8_t)((val >> 8) & 255);
      addr->ip6[n + 1] = (uint8_t)(val & 255);
    } else if (str.ptr[i] == ':') {
      j = i + 1;
      if (i > 0 && str.ptr[i - 1] == ':') {
        dc = n;  // Double colon
        if (i > 1 && str.ptr[i - 2] == ':') return false;
      } else if (i > 0) {
        n += 2;
      }
      if (n > 14) return false;
      addr->ip6[n] = addr->ip6[n + 1] = 0;  // For trailing ::
    } else {
      return false;
    }
  }
  if (n < 14 && dc == 42) return false;
  if (n < 14) {
    memmove(&addr->ip6[dc + (14 - n)], &addr->ip6[dc], n - dc + 2);
    memset(&addr->ip6[dc], 0, 14 - n);
  }
  addr->is_ip6 = true;
  return true;
}

bool mg_aton(struct mg_str str, struct mg_addr *addr) {
  // LOG(LL_INFO, ("[%.*s]", (int) str.len, str.ptr));
  return mg_atonl(str, addr) || mg_aton4(str, addr) || mg_aton6(str, addr);
}

void mg_mgr_free(struct mg_mgr *mgr) {
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) c->is_closing = 1;
  mg_mgr_poll(mgr, 0);
#if MG_ARCH == MG_ARCH_FREERTOS
  FreeRTOS_DeleteSocketSet(mgr->ss);
#endif
  LOG(LL_INFO, ("All connections closed"));
}

void mg_mgr_init(struct mg_mgr *mgr) {
#ifdef _WIN32
  WSADATA data;
  WSAStartup(MAKEWORD(2, 2), &data);
#elif MG_ARCH == MG_ARCH_FREERTOS
  mgr->ss = FreeRTOS_CreateSocketSet();
#elif defined(__unix) || defined(__unix__) || defined(__APPLE__)
  // Ignore SIGPIPE signal, so if client cancels the request, it
  // won't kill the whole process.
  signal(SIGPIPE, SIG_IGN);
#endif
  memset(mgr, 0, sizeof(*mgr));
  mgr->dnstimeout = 3000;
  mgr->dns4.url = "udp://8.8.8.8:53";
  mgr->dns6.url = "udp://[2001:4860:4860::8888]:53";
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sha1.c"
#endif
/* Copyright(c) By Steve Reid <steve@edmweb.com> */
/* 100% Public Domain */

#include <string.h>

/*
 * clang with std=-c99 uses __LITTLE_ENDIAN, by default
 * while for ex, RTOS gcc - LITTLE_ENDIAN, by default
 * it depends on __USE_BSD, but let's have everything
 */
#if !defined(BYTE_ORDER) && defined(__BYTE_ORDER)
#define BYTE_ORDER __BYTE_ORDER
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif /* LITTLE_ENDIAN */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN __LITTLE_ENDIAN
#endif /* BIG_ENDIAN */
#endif /* BYTE_ORDER */

union char64long16 {
  unsigned char c[64];
  uint32_t l[16];
};

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

static uint32_t blk0(union char64long16 *block, int i) {
/* Forrest: SHA expect BIG_ENDIAN, swap if LITTLE_ENDIAN */
#if BYTE_ORDER == LITTLE_ENDIAN
  block->l[i] =
      (rol(block->l[i], 24) & 0xFF00FF00) | (rol(block->l[i], 8) & 0x00FF00FF);
#endif
  return block->l[i];
}

/* Avoid redefine warning (ARM /usr/include/sys/ucontext.h define R0~R4) */
#undef blk
#undef R0
#undef R1
#undef R2
#undef R3
#undef R4

#define blk(i)                                                               \
  (block->l[i & 15] = rol(block->l[(i + 13) & 15] ^ block->l[(i + 8) & 15] ^ \
                              block->l[(i + 2) & 15] ^ block->l[i & 15],     \
                          1))
#define R0(v, w, x, y, z, i)                                          \
  z += ((w & (x ^ y)) ^ y) + blk0(block, i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R1(v, w, x, y, z, i)                                  \
  z += ((w & (x ^ y)) ^ y) + blk(i) + 0x5A827999 + rol(v, 5); \
  w = rol(w, 30);
#define R2(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0x6ED9EBA1 + rol(v, 5); \
  w = rol(w, 30);
#define R3(v, w, x, y, z, i)                                        \
  z += (((w | x) & y) | (w & x)) + blk(i) + 0x8F1BBCDC + rol(v, 5); \
  w = rol(w, 30);
#define R4(v, w, x, y, z, i)                          \
  z += (w ^ x ^ y) + blk(i) + 0xCA62C1D6 + rol(v, 5); \
  w = rol(w, 30);

void mg_sha1_transform(uint32_t state[5], const unsigned char buffer[64]) {
  uint32_t a, b, c, d, e;
  union char64long16 block[1];

  memcpy(block, buffer, 64);
  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  R0(a, b, c, d, e, 0);
  R0(e, a, b, c, d, 1);
  R0(d, e, a, b, c, 2);
  R0(c, d, e, a, b, 3);
  R0(b, c, d, e, a, 4);
  R0(a, b, c, d, e, 5);
  R0(e, a, b, c, d, 6);
  R0(d, e, a, b, c, 7);
  R0(c, d, e, a, b, 8);
  R0(b, c, d, e, a, 9);
  R0(a, b, c, d, e, 10);
  R0(e, a, b, c, d, 11);
  R0(d, e, a, b, c, 12);
  R0(c, d, e, a, b, 13);
  R0(b, c, d, e, a, 14);
  R0(a, b, c, d, e, 15);
  R1(e, a, b, c, d, 16);
  R1(d, e, a, b, c, 17);
  R1(c, d, e, a, b, 18);
  R1(b, c, d, e, a, 19);
  R2(a, b, c, d, e, 20);
  R2(e, a, b, c, d, 21);
  R2(d, e, a, b, c, 22);
  R2(c, d, e, a, b, 23);
  R2(b, c, d, e, a, 24);
  R2(a, b, c, d, e, 25);
  R2(e, a, b, c, d, 26);
  R2(d, e, a, b, c, 27);
  R2(c, d, e, a, b, 28);
  R2(b, c, d, e, a, 29);
  R2(a, b, c, d, e, 30);
  R2(e, a, b, c, d, 31);
  R2(d, e, a, b, c, 32);
  R2(c, d, e, a, b, 33);
  R2(b, c, d, e, a, 34);
  R2(a, b, c, d, e, 35);
  R2(e, a, b, c, d, 36);
  R2(d, e, a, b, c, 37);
  R2(c, d, e, a, b, 38);
  R2(b, c, d, e, a, 39);
  R3(a, b, c, d, e, 40);
  R3(e, a, b, c, d, 41);
  R3(d, e, a, b, c, 42);
  R3(c, d, e, a, b, 43);
  R3(b, c, d, e, a, 44);
  R3(a, b, c, d, e, 45);
  R3(e, a, b, c, d, 46);
  R3(d, e, a, b, c, 47);
  R3(c, d, e, a, b, 48);
  R3(b, c, d, e, a, 49);
  R3(a, b, c, d, e, 50);
  R3(e, a, b, c, d, 51);
  R3(d, e, a, b, c, 52);
  R3(c, d, e, a, b, 53);
  R3(b, c, d, e, a, 54);
  R3(a, b, c, d, e, 55);
  R3(e, a, b, c, d, 56);
  R3(d, e, a, b, c, 57);
  R3(c, d, e, a, b, 58);
  R3(b, c, d, e, a, 59);
  R4(a, b, c, d, e, 60);
  R4(e, a, b, c, d, 61);
  R4(d, e, a, b, c, 62);
  R4(c, d, e, a, b, 63);
  R4(b, c, d, e, a, 64);
  R4(a, b, c, d, e, 65);
  R4(e, a, b, c, d, 66);
  R4(d, e, a, b, c, 67);
  R4(c, d, e, a, b, 68);
  R4(b, c, d, e, a, 69);
  R4(a, b, c, d, e, 70);
  R4(e, a, b, c, d, 71);
  R4(d, e, a, b, c, 72);
  R4(c, d, e, a, b, 73);
  R4(b, c, d, e, a, 74);
  R4(a, b, c, d, e, 75);
  R4(e, a, b, c, d, 76);
  R4(d, e, a, b, c, 77);
  R4(c, d, e, a, b, 78);
  R4(b, c, d, e, a, 79);
  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  /* Erase working structures. The order of operations is important,
   * used to ensure that compiler doesn't optimize those out. */
  memset(block, 0, sizeof(block));
  a = b = c = d = e = 0;
  (void) a;
  (void) b;
  (void) c;
  (void) d;
  (void) e;
}

void mg_sha1_init(mg_sha1_ctx *context) {
  context->state[0] = 0x67452301;
  context->state[1] = 0xEFCDAB89;
  context->state[2] = 0x98BADCFE;
  context->state[3] = 0x10325476;
  context->state[4] = 0xC3D2E1F0;
  context->count[0] = context->count[1] = 0;
}

void mg_sha1_update(mg_sha1_ctx *context, const unsigned char *data,
                    size_t len) {
  size_t i, j;

  j = context->count[0];
  if ((context->count[0] += (uint32_t) len << 3) < j) context->count[1]++;
  context->count[1] += (uint32_t)(len >> 29);
  j = (j >> 3) & 63;
  if ((j + len) > 63) {
    memcpy(&context->buffer[j], data, (i = 64 - j));
    mg_sha1_transform(context->state, context->buffer);
    for (; i + 63 < len; i += 64) {
      mg_sha1_transform(context->state, &data[i]);
    }
    j = 0;
  } else
    i = 0;
  memcpy(&context->buffer[j], &data[i], len - i);
}

void mg_sha1_final(unsigned char digest[20], mg_sha1_ctx *context) {
  unsigned i;
  unsigned char finalcount[8], c;

  for (i = 0; i < 8; i++) {
    finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >>
                                      ((3 - (i & 3)) * 8)) &
                                     255);
  }
  c = 0200;
  mg_sha1_update(context, &c, 1);
  while ((context->count[0] & 504) != 448) {
    c = 0000;
    mg_sha1_update(context, &c, 1);
  }
  mg_sha1_update(context, finalcount, 8);
  for (i = 0; i < 20; i++) {
    digest[i] =
        (unsigned char) ((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
  }
  memset(context, '\0', sizeof(*context));
  memset(&finalcount, '\0', sizeof(finalcount));
}

void mg_hmac_sha1(const unsigned char *key, size_t keylen,
                  const unsigned char *data, size_t datalen,
                  unsigned char out[20]) {
  mg_sha1_ctx ctx;
  unsigned char buf1[64], buf2[64], tmp_key[20], i;

  if (keylen > sizeof(buf1)) {
    mg_sha1_init(&ctx);
    mg_sha1_update(&ctx, key, keylen);
    mg_sha1_final(tmp_key, &ctx);
    key = tmp_key;
    keylen = sizeof(tmp_key);
  }

  memset(buf1, 0, sizeof(buf1));
  memset(buf2, 0, sizeof(buf2));
  memcpy(buf1, key, keylen);
  memcpy(buf2, key, keylen);

  for (i = 0; i < sizeof(buf1); i++) {
    buf1[i] ^= 0x36;
    buf2[i] ^= 0x5c;
  }

  mg_sha1_init(&ctx);
  mg_sha1_update(&ctx, buf1, sizeof(buf1));
  mg_sha1_update(&ctx, data, datalen);
  mg_sha1_final(out, &ctx);

  mg_sha1_init(&ctx);
  mg_sha1_update(&ctx, buf2, sizeof(buf2));
  mg_sha1_update(&ctx, out, 20);
  mg_sha1_final(out, &ctx);
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sntp.c"
#endif






#define SNTP_INTERVAL_SEC (3600)
#define SNTP_TIME_OFFSET 2208988800

static unsigned long s_sntmp_next;

int mg_sntp_parse(const unsigned char *buf, size_t len, struct timeval *tv) {
  int mode = len > 0 ? buf[0] & 7 : 0, res = -1;
  if (len < 48) {
    LOG(LL_ERROR, ("%s", "corrupt packet"));
  } else if ((buf[0] & 0x38) >> 3 != 4) {
    LOG(LL_ERROR, ("%s", "wrong version"));
  } else if (mode != 4 && mode != 5) {
    LOG(LL_ERROR, ("%s", "not a server reply"));
  } else if (buf[1] == 0) {
    LOG(LL_ERROR, ("%s", "server sent a kiss of death"));
  } else {
    uint32_t *data = (uint32_t *) &buf[40];
    tv->tv_sec = mg_ntohl(data[0]) - SNTP_TIME_OFFSET;
    tv->tv_usec = mg_ntohl(data[1]);
    s_sntmp_next = tv->tv_sec + SNTP_INTERVAL_SEC;
    res = 0;
  }
  return res;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
  if (ev == MG_EV_READ) {
    struct timeval tv = {0, 0};
    if (mg_sntp_parse(c->recv.buf, c->recv.len, &tv) == 0) {
      mg_call(c, MG_EV_SNTP_TIME, &tv);
      LOG(LL_DEBUG, ("%u.%u, next at %lu", (unsigned) tv.tv_sec,
                     (unsigned) tv.tv_usec, s_sntmp_next));
    }
    c->recv.len = 0;  // Clear receive buffer
  } else if (ev == MG_EV_RESOLVE) {
    mg_sntp_send(c, time(NULL));
  } else if (ev == MG_EV_CLOSE) {
    // mg_fn_del(c, sntp_cb);
  }
  (void) fnd;
  (void) evd;
}

void mg_sntp_send(struct mg_connection *c, unsigned long utc) {
  if (c->is_resolving) {
    LOG(LL_ERROR, ("%lu wait until resolved", c->id));
  } else if (utc > s_sntmp_next) {
    uint8_t buf[48] = {0};
    s_sntmp_next = utc + SNTP_INTERVAL_SEC;
    buf[0] = (3 << 6) | (4 << 3) | 3;
    mg_send(c, buf, sizeof(buf));
    LOG(LL_DEBUG,
        ("%p request sent, ct %lu, next at %lu", c->fd, utc, s_sntmp_next));
  }
}

struct mg_connection *mg_sntp_connect(struct mg_mgr *mgr, const char *url,
                                      mg_event_handler_t fn, void *fnd) {
  struct mg_connection *c = NULL;
  if (url == NULL) url = "udp://time.google.com:123";
  if ((c = mg_connect(mgr, url, fn, fnd)) != NULL) c->pfn = sntp_cb;
  return c;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/sock.c"
#endif











#if MG_ENABLE_SOCKET
#if defined(_WIN32)
#define MG_SOCK_ERRNO WSAGetLastError()
#define FD(C_) ((SOCKET)(C_)->fd)
#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif
#elif MG_ARCH == MG_ARCH_FREERTOS
#define MG_SOCK_ERRNO errno
typedef Socket_t SOCKET;
#define FD(C_) ((long) (C_)->fd)
#define INVALID_SOCKET FREERTOS_INVALID_SOCKET
#else
#define MG_SOCK_ERRNO errno
#define closesocket(x) close(x)
#define INVALID_SOCKET (-1)
typedef int SOCKET;
#define FD(C_) ((SOCKET)(long) (C_)->fd)
#endif

#ifndef MSG_NONBLOCKING
#define MSG_NONBLOCKING 0
#endif

union usa {
  struct sockaddr sa;
  struct sockaddr_in sin;
#if MG_ENABLE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

static union usa tousa(struct mg_addr *a) {
  union usa usa;
  memset(&usa, 0, sizeof(usa));
  usa.sin.sin_family = AF_INET;
  usa.sin.sin_port = a->port;
  *(uint32_t *) &usa.sin.sin_addr = a->ip;
#if MG_ENABLE_IPV6
  if (a->is_ip6) {
    usa.sin.sin_family = AF_INET6;
    usa.sin6.sin6_port = a->port;
    memcpy(&usa.sin6.sin6_addr, a->ip6, sizeof(a->ip6));
  }
#endif
  return usa;
}

static int mg_sock_failed(void) {
  int err = MG_SOCK_ERRNO;
  return err != EINPROGRESS && err != EWOULDBLOCK
#ifndef WINCE
         && err != EAGAIN && err != EINTR
#endif
#ifdef _WIN32
         && err != WSAEINTR && err != WSAEWOULDBLOCK
#endif
      ;
}

static struct mg_connection *alloc_conn(struct mg_mgr *mgr, int is_client,
                                        SOCKET fd) {
  struct mg_connection *c = (struct mg_connection *) calloc(1, sizeof(*c));
  if (c != NULL) {
    c->is_client = is_client;
    c->fd = (void *) (long) fd;
    c->mgr = mgr;
    c->id = ++mgr->nextid;
  }
  return c;
}

static int mg_sock_recv(struct mg_connection *c, void *buf, int len,
                        int *fail) {
  int n = 0;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = sizeof(usa.sin);
#if MG_ENABLE_IPV6
    if (c->peer.is_ip6) slen = sizeof(usa.sin6);
#endif
    n = recvfrom(FD(c), (char *) buf, len, 0, &usa.sa, &slen);
    if (n > 0) {
      if (c->peer.is_ip6) {
#if MG_ENABLE_IPV6
        memcpy(c->peer.ip6, &usa.sin6.sin6_addr, sizeof(c->peer.ip6));
        c->peer.port = usa.sin6.sin6_port;
#endif
      } else {
        c->peer.ip = *(uint32_t *) &usa.sin.sin_addr;
        c->peer.port = usa.sin.sin_port;
      }
    }
  } else {
    n = recv(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  *fail = (n == 0) || (n < 0 && mg_sock_failed());
  return n;
}

static int mg_sock_send(struct mg_connection *c, const void *buf, int len,
                        int *fail) {
  int n = 0;
  if (c->is_udp) {
    union usa usa = tousa(&c->peer);
    socklen_t slen = sizeof(usa.sin);
#if MG_ENABLE_IPV6
    if (c->peer.is_ip6) slen = sizeof(usa.sin6);
#endif
    n = sendto(FD(c), (char *) buf, len, 0, &usa.sa, slen);
  } else {
    n = send(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  *fail = (n == 0) || (n < 0 && mg_sock_failed());
  return n;
}

static int ll_read(struct mg_connection *c, void *buf, int len, int *fail) {
  int n = c->is_tls ? mg_tls_recv(c, buf, len, fail)
                    : mg_sock_recv(c, buf, len, fail);
  LOG(*fail ? LL_DEBUG : LL_VERBOSE_DEBUG,
      ("%lu %c%c%c %d/%d %d %d", c->id, c->is_tls ? 'T' : 't',
       c->is_udp ? 'U' : 'u', c->is_connecting ? 'C' : 'c', n, len,
       MG_SOCK_ERRNO, *fail));
  if (n > 0 && c->is_hexdumping) {
    char *s = mg_hexdump(buf, n);
    LOG(LL_INFO, ("\n-- %lu %s %s %d\n%s--", c->id, c->label, "<-", n, s));
    free(s);
  }
  return n;
}

static int ll_write(struct mg_connection *c, const void *buf, int len,
                    int *fail) {
  int n = c->is_tls ? mg_tls_send(c, buf, len, fail)
                    : mg_sock_send(c, buf, len, fail);
  LOG(*fail ? LL_ERROR : LL_VERBOSE_DEBUG,
      ("%lu %c%c%c %d/%d %d", c->id, c->is_tls ? 'T' : 't',
       c->is_udp ? 'U' : 'u', c->is_connecting ? 'C' : 'c', n, len,
       MG_SOCK_ERRNO));
  if (n > 0 && c->is_hexdumping) {
    char *s = mg_hexdump(buf, len);
    LOG(LL_INFO, ("\n-- %lu %s %s %d\n%s--", c->id, c->label, "->", len, s));
    free(s);
  }
  return n;
}

int mg_send(struct mg_connection *c, const void *buf, size_t len) {
  int fail, n = c->is_udp
                    ? ll_write(c, buf, (SOCKET) len, &fail)
                    : (int) mg_iobuf_append(&c->send, buf, len, MG_IO_SIZE);
  if (len > 0 && n == 0) fail = 1;
  return n;
}

static void mg_set_non_blocking_mode(SOCKET fd) {
#ifdef _WIN32
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ARCH == MG_ARCH_FREERTOS
  const BaseType_t off = 0;
  setsockopt(fd, 0, FREERTOS_SO_RCVTIMEO, &off, sizeof(off));
  setsockopt(fd, 0, FREERTOS_SO_SNDTIMEO, &off, sizeof(off));
#else
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
#endif
}

SOCKET mg_open_listener(const char *url) {
  struct mg_addr addr;
  SOCKET fd = INVALID_SOCKET;

  memset(&addr, 0, sizeof(addr));
  addr.port = mg_htons(mg_url_port(url));
  if (!mg_aton(mg_url_host(url), &addr)) {
    LOG(LL_ERROR, ("invalid listening URL: %s", url));
  } else {
    union usa usa = tousa(&addr);
    int on = 1, af = AF_INET;
    int type = strncmp(url, "udp:", 4) == 0 ? SOCK_DGRAM : SOCK_STREAM;
    int proto = type == SOCK_DGRAM ? IPPROTO_UDP : IPPROTO_TCP;
    socklen_t slen = sizeof(usa.sin);
#if MG_ENABLE_IPV6
    if (addr.is_ip6) af = AF_INET6, slen = sizeof(usa.sin6);
#endif

    if ((fd = socket(af, type, proto)) != INVALID_SOCKET &&
#if !defined(_WIN32) || !defined(SO_EXCLUSIVEADDRUSE)
        // SO_RESUSEADDR is not enabled on Windows because the semantics of
        // SO_REUSEADDR on UNIX and Windows is different. On Windows,
        // SO_REUSEADDR allows to bind a socket to a port without error even if
        // the port is already open by another program. This is not the behavior
        // SO_REUSEADDR was designed for, and leads to hard-to-track failure
        // scenarios. Therefore, SO_REUSEADDR was disabled on Windows unless
        // SO_EXCLUSIVEADDRUSE is supported and set on a socket.
        !setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) &&
#endif
#if defined(_WIN32) && defined(SO_EXCLUSIVEADDRUSE) && !defined(WINCE)
        // "Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE"
        //! setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *) &on, sizeof(on))
        //! &&
        !setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *) &on,
                    sizeof(on)) &&
#endif
        bind(fd, &usa.sa, slen) == 0 &&
        // NOTE(lsm): FreeRTOS uses backlog value as a connection limit
        (type == SOCK_DGRAM || listen(fd, 128) == 0)) {
      mg_set_non_blocking_mode(fd);
    } else if (fd != INVALID_SOCKET) {
      LOG(LL_ERROR, ("Failed to listen on %s, errno %d", url, MG_SOCK_ERRNO));
      closesocket(fd);
      fd = INVALID_SOCKET;
    }
  }

  return fd;
}

static void read_conn(struct mg_connection *c,
                      int (*fn)(struct mg_connection *, void *, int, int *)) {
  unsigned char *buf;
  int rc, len, fail;

  // NOTE(lsm): do only one iteration of reads, cause some systems
  // (e.g. FreeRTOS stack) return 0 instead of -1/EWOULDBLOCK when no data
  if (c->recv.size - c->recv.len < MG_IO_SIZE &&
      c->recv.size < MG_MAX_RECV_BUF_SIZE &&
      !mg_iobuf_resize(&c->recv, c->recv.size + MG_IO_SIZE)) {
    c->is_closing = 1;
  }
  buf = c->recv.buf + c->recv.len;
  len = (int) (c->recv.size - c->recv.len);
  rc = fn(c, buf, len, &fail);
  if (rc > 0) {
    struct mg_str evd = mg_str_n((char *) buf, rc);
    c->recv.len += rc;
    mg_call(c, MG_EV_READ, &evd);
  } else {
    if (fail) c->is_closing = 1;
  }
}

static int write_conn(struct mg_connection *c) {
  int fail, rc = ll_write(c, c->send.buf, (SOCKET) c->send.len, &fail);
  if (rc > 0) {
    mg_iobuf_delete(&c->send, rc);
    if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
    mg_call(c, MG_EV_WRITE, &rc);
  } else if (fail) {
    c->is_closing = 1;
  }
  return rc;
}

static void close_conn(struct mg_connection *c) {
  // Unlink this connection from the list
  LIST_DELETE(struct mg_connection, &c->mgr->conns, c);
  mg_resolve_cancel(c);
  if (c == c->mgr->dns4.c) c->mgr->dns4.c = NULL;
  if (c == c->mgr->dns6.c) c->mgr->dns6.c = NULL;
  mg_call(c, MG_EV_CLOSE, NULL);
  // while (c->callbacks != NULL) mg_fn_del(c, c->callbacks->fn);
  LOG(LL_DEBUG, ("%lu closed", c->id));
  if (FD(c) != INVALID_SOCKET) {
    closesocket(FD(c));
#if MG_ARCH == MG_ARCH_FREERTOS
    FreeRTOS_FD_CLR(c->fd, c->mgr->ss, eSELECT_ALL);
#endif
  }
  mg_tls_free(c);
  free(c->recv.buf);
  free(c->send.buf);
  memset(c, 0, sizeof(*c));
  free(c);
}

static void setsockopts(struct mg_connection *c) {
#if MG_ARCH == MG_ARCH_FREERTOS
  FreeRTOS_FD_SET(c->fd, c->mgr->ss, eSELECT_READ | eSELECT_EXCEPT);
#else
  int on = 1;
#if !defined(SOL_TCP)
#define SOL_TCP IPPROTO_TCP
#endif
  setsockopt(FD(c), SOL_TCP, TCP_NODELAY, (char *) &on, sizeof(on));
#if defined(TCP_QUICKACK)
  setsockopt(FD(c), SOL_TCP, TCP_QUICKACK, (char *) &on, sizeof(on));
#endif
  setsockopt(FD(c), SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof(on));
#if ESP32 || ESP8266 || defined(__linux__)
  int idle = 60;
  setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
#endif
#if !defined(_WIN32) && !defined(__QNX__)
  {
    int cnt = 3, intvl = 20;
    setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt));
    setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
  }
#endif
#endif
}

void mg_connect_resolved(struct mg_connection *c) {
  char buf[40];
  int type = c->is_udp ? SOCK_DGRAM : SOCK_STREAM;
  int af = AF_INET;
#if MG_ENABLE_IPV6
  if (c->peer.is_ip6) af = AF_INET6;
#endif
  mg_straddr(c, buf, sizeof(buf));
  c->fd = (void *) (long) socket(af, type, 0);
  if (FD(c) == INVALID_SOCKET) {
    mg_error(c, "socket(): %d", MG_SOCK_ERRNO);
    return;
  }

  mg_set_non_blocking_mode(FD(c));
  mg_call(c, MG_EV_RESOLVE, NULL);
  if (type == SOCK_STREAM) {
    union usa usa = tousa(&c->peer);
    socklen_t slen =
#if MG_ENABLE_IPV6
        c->peer.is_ip6 ? sizeof(usa.sin6) :
#endif
                       sizeof(usa.sin);
    int rc = connect(FD(c), &usa.sa, slen);
    int fail = rc < 0 && mg_sock_failed() ? MG_SOCK_ERRNO : 0;
    if (fail) {
      mg_error(c, "connect: %d", MG_SOCK_ERRNO);
    } else {
      setsockopts(c);
    }
    if (rc < 0) c->is_connecting = 1;
  }
}

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *url,
                                 mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  if ((c = alloc_conn(mgr, 1, INVALID_SOCKET)) == NULL) {
    LOG(LL_ERROR, ("OOM"));
  } else {
    struct mg_str host = mg_url_host(url);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->is_udp = (strncmp(url, "udp:", 4) == 0);
    c->peer.port = mg_htons(mg_url_port(url));
    c->fn = fn;
    c->fn_data = fn_data;
    LOG(LL_DEBUG, ("%lu -> %s", c->id, url));
    mg_resolve(c, &host, mgr->dnstimeout);
  }
  return c;
}

static void accept_conn(struct mg_mgr *mgr, struct mg_connection *lsn) {
  struct mg_connection *c = NULL;
  union usa usa;
  socklen_t sa_len = sizeof(usa);
  SOCKET fd = accept(FD(lsn), &usa.sa, &sa_len);
  if (fd == INVALID_SOCKET) {
    LOG(LL_ERROR, ("%lu accept failed, errno %d", lsn->id, MG_SOCK_ERRNO));
#if !defined(_WIN32)
  } else if (fd >= FD_SETSIZE) {
    LOG(LL_ERROR, ("%ld > %ld", (long) fd, (long) FD_SETSIZE));
    closesocket(fd);
#endif
  } else if ((c = alloc_conn(mgr, 0, fd)) == NULL) {
    LOG(LL_ERROR, ("%lu OOM", lsn->id));
    closesocket(fd);
  } else {
    char buf[40];
    c->peer.port = usa.sin.sin_port;
    memcpy(&c->peer.ip, &usa.sin.sin_addr, sizeof(c->peer.ip));
#if MG_ENABLE_IPV6
    if (sa_len == sizeof(usa.sin6)) {
      memcpy(c->peer.ip6, &usa.sin6.sin6_addr, sizeof(c->peer.ip6));
      c->peer.port = usa.sin6.sin6_port;
      c->peer.is_ip6 = 1;
    }
#endif
    mg_straddr(c, buf, sizeof(buf));
    LOG(LL_DEBUG, ("%lu accepted %s", c->id, buf));
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->is_accepted = 1;
    c->is_hexdumping = lsn->is_hexdumping;
    c->pfn = lsn->pfn;
    c->pfn_data = lsn->pfn_data;
    c->fn = lsn->fn;
    c->fn_data = lsn->fn_data;
    mg_call(c, MG_EV_ACCEPT, NULL);
  }
}

#if MG_ENABLE_SOCKETPAIR
bool mg_socketpair(int *s1, int *s2) {
#ifdef MG_ENABLE_NATIVE_SOCKETPAIR
  // For some reason, native socketpair() call fails on Macos
  // Enable this codepath only when MG_ENABLE_NATIVE_SOCKETPAIR is defined
  int sp[2], ret = 0;
  if (socketpair(AF_INET, SOCK_DGRAM, IPPROTO_UDP, sp) == 0) {
    *s1 = sp[0], *s2 = sp[1], ret = 1;
  }
  LOG(LL_INFO, ("errno %d", errno));
  return ret;
#else
  union usa sa, sa2;
  SOCKET sp[2] = {INVALID_SOCKET, INVALID_SOCKET};
  socklen_t len = sizeof(sa.sin);
  int ret = 0;

  (void) memset(&sa, 0, sizeof(sa));
  sa.sin.sin_family = AF_INET;
  sa.sin.sin_addr.s_addr = htonl(0x7f000001); /* 127.0.0.1 */
  sa2 = sa;

  if ((sp[0] = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET &&
      (sp[1] = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET &&
      bind(sp[0], &sa.sa, len) == 0 && bind(sp[1], &sa2.sa, len) == 0 &&
      getsockname(sp[0], &sa.sa, &len) == 0 &&
      getsockname(sp[1], &sa2.sa, &len) == 0 &&
      connect(sp[0], &sa2.sa, len) == 0 && connect(sp[1], &sa.sa, len) == 0) {
    mg_set_non_blocking_mode(sp[1]);
    *s1 = sp[0];
    *s2 = sp[1];
    ret = 1;
  } else {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
  }

  return ret;
#endif
}
#endif

struct mg_connection *mg_listen(struct mg_mgr *mgr, const char *url,
                                mg_event_handler_t fn, void *fn_data) {
  struct mg_connection *c = NULL;
  int is_udp = strncmp(url, "udp:", 4) == 0;
  SOCKET fd = mg_open_listener(url);
  if (fd == INVALID_SOCKET) {
  } else if ((c = alloc_conn(mgr, 0, fd)) == NULL) {
    LOG(LL_ERROR, ("OOM %s", url));
    closesocket(fd);
  } else {
    c->fd = (void *) (long) fd;
    c->is_listening = 1;
    c->is_udp = is_udp;
    setsockopts(c);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fn = fn;
    c->fn_data = fn_data;
    LOG(LL_INFO, ("%lu accepting on %s", c->id, url));
  }
  return c;
}

static void mg_iotest(struct mg_mgr *mgr, int ms) {
#if MG_ARCH == MG_ARCH_FREERTOS
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    FreeRTOS_FD_CLR(c->fd, mgr->ss, eSELECT_WRITE);
    if (c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0))
      FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_WRITE);
  }
  FreeRTOS_select(mgr->ss, pdMS_TO_TICKS(ms));
  for (c = mgr->conns; c != NULL; c = c->next) {
    EventBits_t bits = FreeRTOS_FD_ISSET(c->fd, mgr->ss);
    c->is_readable = bits & (eSELECT_READ | eSELECT_EXCEPT) ? 1 : 0;
    c->is_writable = bits & eSELECT_WRITE ? 1 : 0;
  }
#else
  struct timeval tv = {ms / 1000, (ms % 1000) * 1000};
  struct mg_connection *c;
  fd_set rset, wset;
  SOCKET maxfd = 0;
  int rc;

  FD_ZERO(&rset);
  FD_ZERO(&wset);

  for (c = mgr->conns; c != NULL; c = c->next) {
    // c->is_writable = 0;
    // TLS might have stuff buffered, so dig everything
    // c->is_readable = c->is_tls && c->is_readable ? 1 : 0;
    if (c->is_closing || c->is_resolving || FD(c) == INVALID_SOCKET) continue;
    FD_SET(FD(c), &rset);
    if (FD(c) > maxfd) maxfd = FD(c);
    if (c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0))
      FD_SET(FD(c), &wset);
  }

  if ((rc = select(maxfd + 1, &rset, &wset, NULL, &tv)) < 0) {
    LOG(LL_DEBUG, ("select: %d %d", rc, MG_SOCK_ERRNO));
    FD_ZERO(&rset);
    FD_ZERO(&wset);
  }

  for (c = mgr->conns; c != NULL; c = c->next) {
    // TLS might have stuff buffered, so dig everything
    c->is_readable = c->is_tls && c->is_readable
                         ? 1
                         : FD(c) != INVALID_SOCKET && FD_ISSET(FD(c), &rset);
    c->is_writable = FD(c) != INVALID_SOCKET && FD_ISSET(FD(c), &wset);
  }
#endif
}

static void connect_conn(struct mg_connection *c) {
  int rc = 0;
  socklen_t len = sizeof(rc);
  if (getsockopt(FD(c), SOL_SOCKET, SO_ERROR, (char *) &rc, &len)) rc = 1;
  if (rc == EAGAIN || rc == EWOULDBLOCK) rc = 0;
  c->is_connecting = 0;
  if (rc) {
    char buf[40];
    mg_error(c, "error connecting to %s", mg_straddr(c, buf, sizeof(buf)));
  } else {
    if (c->is_tls_hs) mg_tls_handshake(c);
    mg_call(c, MG_EV_CONNECT, NULL);
  }
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  unsigned long now;

  mg_iotest(mgr, ms);
  now = mg_millis();
  mg_timer_poll(now);

  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    LOG(LL_VERBOSE_DEBUG,
        ("%lu %c%c %c%c%c%c%c", c->id, c->is_readable ? 'r' : '-',
         c->is_writable ? 'w' : '-', c->is_tls ? 'T' : 't',
         c->is_connecting ? 'C' : 'c', c->is_tls_hs ? 'H' : 'h',
         c->is_resolving ? 'R' : 'r', c->is_closing ? 'C' : 'c'));
    if (c->is_resolving || c->is_closing) {
      // Do nothing
    } else if (c->is_listening && c->is_udp == 0) {
      if (c->is_readable) accept_conn(mgr, c);
    } else if (c->is_connecting) {
      if (c->is_readable || c->is_writable) connect_conn(c);
    } else if (c->is_tls_hs) {
      if ((c->is_readable || c->is_writable)) mg_tls_handshake(c);
    } else {
      if (c->is_readable) read_conn(c, ll_read);
      if (c->is_writable) write_conn(c);
    }

    if (c->is_draining && c->send.len == 0) c->is_closing = 1;
    if (c->is_closing) close_conn(c);
  }
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/ssi.c"
#endif




#ifndef MG_MAX_SSI_DEPTH
#define MG_MAX_SSI_DEPTH 5
#endif

#if MG_ENABLE_SSI
static char *mg_ssi(const char *path, const char *root, int depth) {
  struct mg_iobuf b = {NULL, 0, 0};
  FILE *fp = mg_fopen(path, "rb");
  if (fp != NULL) {
    char buf[BUFSIZ], arg[sizeof(buf)];
    int ch, intag = 0;
    size_t len = 0, align = MG_IO_SIZE;
    while ((ch = fgetc(fp)) != EOF) {
      if (intag && ch == '>' && buf[len - 1] == '-' && buf[len - 2] == '-') {
        buf[len++] = ch & 0xff;
        if (sscanf(buf, "<!--#include file=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX], *p = (char *) path + strlen(path), *data;
          while (p > path && p[-1] != MG_DIRSEP && p[-1] != '/') p--;
          snprintf(tmp, sizeof(tmp), "%.*s%s", (int) (p - path), path, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_append(&b, data, strlen(data), align);
            free(data);
          } else {
            LOG(LL_ERROR, ("%s: file=%s error or too deep", path, arg));
          }
        } else if (sscanf(buf, "<!--#include virtual=\"%[^\"]", arg)) {
          char tmp[MG_PATH_MAX], *data;
          snprintf(tmp, sizeof(tmp), "%s%s", root, arg);
          if (depth < MG_MAX_SSI_DEPTH &&
              (data = mg_ssi(tmp, root, depth + 1)) != NULL) {
            mg_iobuf_append(&b, data, strlen(data), align);
            free(data);
          } else {
            LOG(LL_ERROR, ("%s: virtual=%s error or too deep", path, arg));
          }
        } else {
          // Unknown SSI tag
          LOG(LL_INFO, ("Unknown SSI tag: %.*s", (int) len, buf));
          mg_iobuf_append(&b, buf, len, align);
        }
        intag = 0;
        len = 0;
      } else if (ch == '<') {
        intag = 1;
        if (len > 0) mg_iobuf_append(&b, buf, len, align);
        len = 0;
        buf[len++] = ch & 0xff;
      } else if (intag) {
        if (len == 5 && strncmp(buf, "<!--#", 5) != 0) {
          intag = 0;
        } else if (len >= sizeof(buf) - 2) {
          LOG(LL_ERROR, ("%s: SSI tag is too large", path));
          len = 0;
        }
        buf[len++] = ch & 0xff;
      } else {
        buf[len++] = ch & 0xff;
        if (len >= sizeof(buf)) {
          mg_iobuf_append(&b, buf, len, align);
          len = 0;
        }
      }
    }
    if (len > 0) mg_iobuf_append(&b, buf, len, align);
    if (b.len > 0) mg_iobuf_append(&b, "", 1, align);  // nul-terminate
    fclose(fp);
  }
  (void) depth;
  (void) root;
  return (char *) b.buf;
}

void mg_http_serve_ssi(struct mg_connection *c, const char *root,
                       const char *fullpath) {
  char *data = mg_ssi(fullpath, root, 0);
  mg_http_reply(c, 200, "", "%s", data == NULL ? "" : data);
  free(data);
}
#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/str.c"
#endif

#include <stdlib.h>

struct mg_str mg_str(const char *s) {
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
    char *sc = (char *) malloc(s.len + 1);
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

#ifdef MG_ENABLE_LINES
#line 1 "src/timer.c"
#endif
// Copyright (c) Cesanta Software Limited
// All rights reserved




struct mg_timer *g_timers;

void mg_timer_init(struct mg_timer *t, int ms, int flags, void (*fn)(void *),
                   void *arg) {
  struct mg_timer tmp = {ms, flags, fn, arg, 0UL, g_timers};
  *t = tmp;
  g_timers = t;
  if (flags & MG_TIMER_RUN_NOW) fn(arg);
}

void mg_timer_free(struct mg_timer *t) {
  struct mg_timer **head = &g_timers;
  while (*head && *head != t) head = &(*head)->next;
  if (*head) *head = t->next;
}

void mg_timer_poll(unsigned long now_ms) {
  // If time goes back (wrapped around), reset timers
  struct mg_timer *t, *tmp;
  static unsigned long oldnow;  // Timestamp in a previous invocation
  if (oldnow > now_ms) {        // If it is wrapped, reset timers
    for (t = g_timers; t != NULL; t = t->next) t->expire = 0;
  }
  oldnow = now_ms;

  for (t = g_timers; t != NULL; t = tmp) {
    tmp = t->next;
    if (t->expire == 0) t->expire = now_ms + t->period_ms;
    if (t->expire > now_ms) continue;
    t->fn(t->arg);
    // Try to tick timers with the given period as accurate as possible,
    // even if this polling function is called with some random period.
    t->expire = now_ms - t->expire > (unsigned long) t->period_ms
                    ? now_ms + t->period_ms
                    : t->expire + t->period_ms;
    if (!(t->flags & MG_TIMER_REPEAT)) mg_timer_free(t);
  }
}

#ifdef MG_ENABLE_LINES
#line 1 "src/tls.c"
#endif


#if MG_ENABLE_MBEDTLS  ///////////////////////////////////////// MBEDTLS




#include <mbedtls/debug.h>
#include <mbedtls/ssl.h>

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif
#endif

// Different versions have those in different files, so declare here
EXTERN_C int mbedtls_net_recv(void *, unsigned char *, size_t);
EXTERN_C int mbedtls_net_send(void *, const unsigned char *, size_t);

struct mg_tls {
  char *cafile;             // CA certificate path
  mbedtls_x509_crt ca;      // Parsed CA certificate
  mbedtls_x509_crt cert;    // Parsed certificate
  mbedtls_ssl_context ssl;  // SSL/TLS context
  mbedtls_ssl_config conf;  // SSL-TLS config
  mbedtls_pk_context pk;    // Private key context
};

int mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  mbedtls_ssl_set_bio(&tls->ssl, &c->fd, mbedtls_net_send, mbedtls_net_recv, 0);
  rc = mbedtls_ssl_handshake(&tls->ssl);
  if (rc == 0) {  // Success
    LOG(LL_DEBUG, ("%lu success", c->id));
    c->is_tls_hs = 0;
  } else if (rc == MBEDTLS_ERR_SSL_WANT_READ ||
             rc == MBEDTLS_ERR_SSL_WANT_WRITE) {  // Still pending
    LOG(LL_VERBOSE_DEBUG, ("%lu pending, %d%d %d (-%#x)", c->id,
                           c->is_connecting, c->is_tls_hs, rc, -rc));
  } else {
    mg_error(c, "TLS handshake: -%#x", -rc);  // Error
  }
  return rc == 0;
}

static int mbed_rng(void *ctx, unsigned char *buf, size_t len) {
  mg_random(buf, len);
  (void) ctx;
  return 0;
}

static void debug_cb(void *c, int lev, const char *s, int n, const char *s2) {
  n = strlen(s2) - 1;
  LOG(LL_VERBOSE_DEBUG, ("%p %.*s", ((struct mg_connection *) c)->fd, n, s2));
  (void) s;
  (void) c;
  (void) lev;
}

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  int rc = 0;
  const char *ca = opts->ca == NULL     ? "-"
                   : opts->ca[0] == '-' ? "(emb)"
                                        : opts->ca;
  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }
  LOG(LL_DEBUG, ("%lu Setting TLS, CA: %s, cert: %s, key: %s", c->id, ca,
                 opts->cert == NULL ? "null" : opts->cert,
                 opts->certkey == NULL ? "null" : opts->certkey));
  mbedtls_ssl_init(&tls->ssl);
  mbedtls_ssl_config_init(&tls->conf);
  mbedtls_ssl_conf_dbg(&tls->conf, debug_cb, c);
#if !defined(ESP_PLATFORM)
  mbedtls_debug_set_threshold(5);
#endif
  if ((rc = mbedtls_ssl_config_defaults(
           &tls->conf,
           c->is_client ? MBEDTLS_SSL_IS_CLIENT : MBEDTLS_SSL_IS_SERVER,
           MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) {
    mg_error(c, "tls defaults %#x", -rc);
    goto fail;
  }
  mbedtls_ssl_conf_rng(&tls->conf, mbed_rng, c);
  if (opts->ca == NULL || strcmp(opts->ca, "*") == 0) {
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_NONE);
  }
  if (opts->ca != NULL && opts->ca[0] != '\0') {
#if defined(MBEDTLS_X509_CA_CHAIN_ON_DISK)
    tls->cafile = strdup(opts->ca);
    rc = mbedtls_ssl_conf_ca_chain_file(&tls->conf, tls->cafile, NULL);
    if (rc != 0) {
      mg_error(c, "parse on-disk chain(%s) err %#x", ca, -rc);
      goto fail;
    }
#else
    mbedtls_x509_crt_init(&tls->ca);
    rc = opts->ca[0] == '-'
             ? mbedtls_x509_crt_parse(&tls->ca, (uint8_t *) opts->ca,
                                      strlen(opts->ca) + 1)
             : mbedtls_x509_crt_parse_file(&tls->ca, opts->ca);
    if (rc != 0) {
      mg_error(c, "parse(%s) err %#x", ca, -rc);
      goto fail;
    }
    mbedtls_ssl_conf_ca_chain(&tls->conf, &tls->ca, NULL);
#endif
    if (opts->srvname.len > 0) {
      char buf[opts->srvname.len + 1];
      sprintf(buf, "%.*s", (int) opts->srvname.len, opts->srvname.ptr);
      mbedtls_ssl_set_hostname(&tls->ssl, buf);
    }
    mbedtls_ssl_conf_authmode(&tls->conf, MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) key = opts->cert;
    mbedtls_x509_crt_init(&tls->cert);
    mbedtls_pk_init(&tls->pk);
    if ((rc = mbedtls_x509_crt_parse_file(&tls->cert, opts->cert)) != 0) {
      mg_error(c, "parse(%s) err %#x", opts->cert, -rc);
      goto fail;
    }
    if ((rc = mbedtls_pk_parse_keyfile(&tls->pk, key, NULL)) != 0) {
      mg_error(c, "tls key(%s) %#x", key, -rc);
      goto fail;
    }
    rc = mbedtls_ssl_conf_own_cert(&tls->conf, &tls->cert, &tls->pk);
    if (rc != 0) {
      mg_error(c, "own cert %#x", -rc);
      goto fail;
    }
  }
  if ((rc = mbedtls_ssl_setup(&tls->ssl, &tls->conf)) != 0) {
    mg_error(c, "setup err %#x", -rc);
    goto fail;
  }
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  return 1;
fail:
  c->is_closing = 1;
  free(tls);
  return 0;
}

int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = mbedtls_ssl_read(&tls->ssl, (unsigned char *) buf, len);
  *fail = (n == 0) || (n < 0 && n != MBEDTLS_ERR_SSL_WANT_READ);
  return n;
}

int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = mbedtls_ssl_write(&tls->ssl, (unsigned char *) buf, len);
  *fail = (n == 0) || (n < 0 && n != MBEDTLS_ERR_SSL_WANT_WRITE);
  return n;
}

int mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return 0;
  free(tls->cafile);
  mbedtls_x509_crt_free(&tls->ca);
  mbedtls_x509_crt_free(&tls->cert);
  mbedtls_ssl_free(&tls->ssl);
  mbedtls_pk_free(&tls->pk);
  mbedtls_ssl_config_free(&tls->conf);
  free(tls);
  return 1;
}
#elif MG_ENABLE_OPENSSL  ///////////////////////////////////////// OPENSSL

#include <openssl/err.h>
#include <openssl/ssl.h>

struct mg_tls {
  SSL_CTX *ctx;
  SSL *ssl;
};

static int mg_tls_err(struct mg_tls *tls, int res) {
  int err = SSL_get_error(tls->ssl, res);
  // We've just fetched the last error from the queue.
  // Now we need to clear the error queue. If we do not, then the following
  // can happen (actually reported):
  //  - A new connection is accept()-ed with cert error (e.g. self-signed cert)
  //  - Since all accept()-ed connections share listener's context,
  //  - *ALL* SSL accepted connection report read error on the next poll cycle.
  //    Thus a single errored connection can close all the rest, unrelated ones.
  // Clearing the error keeps the shared SSL_CTX in an OK state.
  ERR_clear_error();
  if (err == SSL_ERROR_WANT_READ) return 0;
  if (err == SSL_ERROR_WANT_WRITE) return 0;
  return err;
}

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  struct mg_tls *tls = (struct mg_tls *) calloc(1, sizeof(*tls));
  const char *id = "mongoose";
  static unsigned char s_initialised = 0;
  int rc;

  if (tls == NULL) {
    mg_error(c, "TLS OOM");
    goto fail;
  }

  if (!s_initialised) {
    SSL_library_init();
    s_initialised++;
  }
  LOG(LL_DEBUG, ("%lu Setting TLS, CA: %s, cert: %s, key: %s", c->id,
                 opts->ca == NULL ? "null" : opts->ca,
                 opts->cert == NULL ? "null" : opts->cert,
                 opts->certkey == NULL ? "null" : opts->certkey));
  tls->ctx = c->is_client ? SSL_CTX_new(SSLv23_client_method())
                          : SSL_CTX_new(SSLv23_server_method());
  if ((tls->ssl = SSL_new(tls->ctx)) == NULL) {
    mg_error(c, "SSL_new");
    goto fail;
  }
  SSL_set_session_id_context(tls->ssl, (const uint8_t *) id, strlen(id));
  // Disable deprecated protocols
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv2);
  SSL_set_options(tls->ssl, SSL_OP_NO_SSLv3);
  SSL_set_options(tls->ssl, SSL_OP_NO_TLSv1);
#ifdef MG_ENABLE_OPENSSL_NO_COMPRESSION
  SSL_set_options(tls->ssl, SSL_OP_NO_COMPRESSION);
#endif
#ifdef MG_ENABLE_OPENSSL_CIPHER_SERVER_PREFERENCE
  SSL_set_options(tls->ssl, SSL_OP_CIPHER_SERVER_PREFERENCE);
#endif

  if (opts->ca != NULL && opts->ca[0] != '\0') {
    SSL_set_verify(tls->ssl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
                   NULL);
    if ((rc = SSL_CTX_load_verify_locations(tls->ctx, opts->ca, NULL)) != 1) {
      mg_error(c, "parse(%s): err %d", opts->ca, mg_tls_err(tls, rc));
      goto fail;
    }
  }
  if (opts->cert != NULL && opts->cert[0] != '\0') {
    const char *key = opts->certkey;
    if (key == NULL) key = opts->cert;
    if ((rc = SSL_use_certificate_file(tls->ssl, opts->cert, 1)) != 1) {
      mg_error(c, "Invalid SSL cert, err %d", mg_tls_err(tls, rc));
      goto fail;
    } else if ((rc = SSL_use_PrivateKey_file(tls->ssl, key, 1)) != 1) {
      mg_error(c, "Invalid SSL key, err %d", mg_tls_err(tls, rc));
      goto fail;
#if OPENSSL_VERSION_NUMBER > 0x10002000L
    } else if ((rc = SSL_use_certificate_chain_file(tls->ssl, opts->cert)) !=
               1) {
      mg_error(c, "Invalid CA, err %d", mg_tls_err(tls, rc));
      goto fail;
#endif
    } else {
      SSL_set_mode(tls->ssl, SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER);
#if OPENSSL_VERSION_NUMBER > 0x10002000L
      SSL_set_ecdh_auto(tls->ssl, 1);
#endif
    }
  }
  if (opts->ciphers != NULL) SSL_set_cipher_list(tls->ssl, opts->ciphers);
  if (opts->srvname.len > 0) {
    char buf[opts->srvname.len + 1];
    sprintf(buf, "%.*s", (int) opts->srvname.len, opts->srvname.ptr);
    SSL_set_tlsext_host_name(tls->ssl, buf);
  }
  c->tls = tls;
  c->is_tls = 1;
  c->is_tls_hs = 1;
  if (c->is_client && c->is_resolving == 0 && c->is_connecting == 0) {
    mg_tls_handshake(c);
  }
  c->is_hexdumping = 1;
  LOG(LL_DEBUG, ("%lu SSL %s OK", c->id, c->is_accepted ? "accept" : "client"));
  return 1;
fail:
  c->is_closing = 1;
  free(tls);
  return 0;
}

int mg_tls_handshake(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int rc;
  SSL_set_fd(tls->ssl, (long) c->fd);
  rc = c->is_client ? SSL_connect(tls->ssl) : SSL_accept(tls->ssl);
  if (rc == 1) {
    LOG(LL_DEBUG, ("%lu success", c->id));
    c->is_tls_hs = 0;
    return 1;
  } else {
    int code;
    ERR_print_errors_fp(stderr);
    code = mg_tls_err(tls, rc);
    if (code != 0) mg_error(c, "tls hs: rc %d, err %d", rc, code);
    return 0;
  }
}

int mg_tls_free(struct mg_connection *c) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  if (tls == NULL) return 0;
  SSL_free(tls->ssl);
  SSL_CTX_free(tls->ctx);
  free(tls);
  return 1;
}

int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_read(tls->ssl, buf, len);
  *fail = (n == 0) || (n < 0 && mg_tls_err(tls, n) != 0);
  return n;
}

int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  struct mg_tls *tls = (struct mg_tls *) c->tls;
  int n = SSL_write(tls->ssl, buf, len);
  *fail = (n == 0) || (n < 0 && mg_tls_err(tls, n) != 0);
  return n;
}

#else  //////////////////////////////////////////   NO TLS

int mg_tls_init(struct mg_connection *c, struct mg_tls_opts *opts) {
  (void) opts;
  mg_error(c, "TLS is not enabled");
  return 0;
}
int mg_tls_handshake(struct mg_connection *c) {
  return c != NULL;
}
int mg_tls_free(struct mg_connection *c) {
  return c != NULL;
}
int mg_tls_recv(struct mg_connection *c, void *buf, size_t len, int *fail) {
  *fail = 1;
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}
int mg_tls_send(struct mg_connection *c, const void *buf, size_t len,
                int *fail) {
  *fail = 1;
  return c == NULL || buf == NULL || len == 0 ? 0 : -1;
}

#endif

#ifdef MG_ENABLE_LINES
#line 1 "src/url.c"
#endif
#include <stdlib.h>


struct url {
  int key, user, pass, host, port, uri, end;
};

int mg_url_is_ssl(const char *url) {
  return strncmp(url, "wss:", 4) == 0 || strncmp(url, "https:", 6) == 0 ||
         strncmp(url, "mqtts:", 6) == 0 || strncmp(url, "ssl:", 4) == 0 ||
         strncmp(url, "tls:", 4) == 0;
}

static struct url urlparse(const char *url) {
  int i;
  struct url u;
  memset(&u, 0, sizeof(u));
  for (i = 0; url[i] != '\0'; i++) {
    if (i > 0 && url[i - 1] == '/' && url[i] == '/') {
      u.host = i + 1;
      u.port = 0;
    } else if (url[i] == ']') {
      u.port = 0;  // IPv6 URLs, like http://[::1]/bar
    } else if (url[i] == ':') {
      u.port = i + 1;
    } else if (url[i] == '@') {
      u.user = u.host;
      u.pass = u.port;
      u.host = i + 1;
      u.port = 0;
    } else if (u.host && !u.uri && url[i] == '/') {
      u.uri = i;
    }
  }
  u.end = i;
#if 0
  printf("[%s] %d %d %d %d %d\n", url, u.user, u.pass, u.host, u.port, u.uri);
#endif
  return u;
}

struct mg_str mg_url_host(const char *url) {
  struct url u = urlparse(url);
  int n =
      u.port ? u.port - u.host - 1 : u.uri ? u.uri - u.host : u.end - u.host;
  struct mg_str s = mg_str_n(url + u.host, n);
  if (s.len > 2 && s.ptr[0] == '[' && s.ptr[s.len - 1] == ']') {
    s.len -= 2;
    s.ptr++;
  }
  return s;
}

const char *mg_url_uri(const char *url) {
  struct url u = urlparse(url);
  return u.uri ? url + u.uri : "/";
}

unsigned short mg_url_port(const char *url) {
  struct url u = urlparse(url);
  unsigned short port = 0;
  if (memcmp(url, "http:", 5) == 0 || memcmp(url, "ws:", 3) == 0) port = 80;
  if (memcmp(url, "wss:", 4) == 0 || memcmp(url, "https:", 6) == 0) port = 443;
  if (memcmp(url, "mqtt:", 5) == 0) port = 1883;
  if (memcmp(url, "mqtts:", 6) == 0) port = 8883;
  if (u.port) port = atoi(url + u.port);
  return port;
}

struct mg_str mg_url_user(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str("");
  if (u.user && (u.pass || u.host)) {
    int n = u.pass ? u.pass - u.user - 1 : u.host - u.user - 1;
    s = mg_str_n(url + u.user, n);
  }
  return s;
}

struct mg_str mg_url_pass(const char *url) {
  struct url u = urlparse(url);
  struct mg_str s = mg_str("");
  if (u.pass && u.host) {
    int n = u.host - u.pass - 1;
    s = mg_str_n(url + u.pass, n);
  }
  return s;
}

#ifdef MG_ENABLE_LINES
#line 1 "src/util.c"
#endif



#if MG_ENABLE_FS
int mg_stat(const char *path, mg_stat_t *st) {
#ifdef _WIN32
  wchar_t tmp[MG_PATH_MAX];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
  return _wstati64(tmp, st);
#else
  return stat(path, st);
#endif
}

FILE *mg_fopen(const char *path, const char *mode) {
#ifdef _WIN32
  wchar_t b1[MG_PATH_MAX], b2[10];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, b1, sizeof(b1) / sizeof(b1[0]));
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, b2, sizeof(b2) / sizeof(b2[0]));
  return _wfopen(b1, b2);
#else
  return fopen(path, mode);
#endif
}

int64_t mg_file_size(const char *path) {
#if MG_ARCH == MG_ARCH_FREERTOS
  struct FF_STAT st;
  return ff_stat(path, &st) == 0 ? st.st_size : 0;
#else
  mg_stat_t st;
  return mg_stat(path, &st) == 0 ? st.st_size : 0;
#endif
}

char *mg_file_read(const char *path) {
  FILE *fp;
  char *data = NULL;
  size_t size = (size_t) mg_file_size(path);
  if ((fp = mg_fopen(path, "rb")) != NULL) {
    data = (char *) malloc(size + 1);
    if (data != NULL) {
      if (fread(data, 1, size, fp) != size) {
        free(data);
        data = NULL;
      } else {
        data[size] = '\0';
      }
    }
    fclose(fp);
  }
  return data;
}

bool mg_file_write(const char *path, const void *buf, size_t len) {
  bool result = false;
  FILE *fp;
  char tmp[MG_PATH_MAX];
  snprintf(tmp, sizeof(tmp), "%s.%d", path, rand());
  fp = mg_fopen(tmp, "wb");
  if (fp != NULL) {
    result = fwrite(buf, 1, len, fp) == len;
    fclose(fp);
    if (result) {
      remove(path);
      rename(tmp, path);
    } else {
      remove(tmp);
    }
  }
  return result;
}

bool mg_file_printf(const char *path, const char *fmt, ...) {
  char tmp[256], *buf = tmp;
  bool result;
  size_t len;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vasprintf(&buf, sizeof(tmp), fmt, ap);
  va_end(ap);
  result = mg_file_write(path, buf, len);
  if (buf != tmp) free(buf);
  return result;
}
#endif

void mg_random(void *buf, size_t len) {
  bool done = false;
#if MG_ENABLE_FS
  FILE *fp = mg_fopen("/dev/urandom", "rb");
  if (fp != NULL) {
    fread(buf, 1, len, fp);
    fclose(fp);
    done = true;
  }
#endif
  if (!done) {
    // Fallback to a pseudo random gen
    size_t i;
    for (i = 0; i < len; i++) ((unsigned char *) buf)[i] = rand() % 0xff;
  }
}

bool mg_globmatch(const char *s1, int n1, const char *s2, int n2) {
  int i = 0, j = 0, ni = 0, nj = 0;
  while (i < n1 || j < n2) {
    if (i < n1 && j < n2 && (s1[i] == '?' || s2[j] == s1[i])) {
      i++, j++;
    } else if (i < n1 && (s1[i] == '*' || s1[i] == '#')) {
      ni = i, nj = j + 1, i++;
    } else if (nj > 0 && nj <= n2 && (s1[i - 1] == '#' || s2[j] != '/')) {
      i = ni, j = nj;
    } else {
      return false;
    }
  }
  return true;
}

static int mg_nextcommaentry(const char *s, int slen, int ofs, int *koff,
                             int *klen, int *voff, int *vlen) {
  int kvlen, kl;
  for (kvlen = 0; ofs + kvlen < slen && s[ofs + kvlen] != ',';) kvlen++;
  for (kl = 0; kl < kvlen && s[ofs + kl] != '=';) kl++;
  if (koff != NULL) *koff = ofs;
  if (klen != NULL) *klen = kl;
  if (voff != NULL) *voff = kl < kvlen ? ofs + kl + 1 : 0;
  if (vlen != NULL) *vlen = kl < kvlen ? kvlen - kl - 1 : 0;
  return ofs >= slen ? slen : ofs + kvlen + 1;
}

bool mg_next_comma_entry(struct mg_str *s, struct mg_str *k, struct mg_str *v) {
  int koff, klen, voff, vlen;
  int off = mg_nextcommaentry(s->ptr, s->len, 0, &koff, &klen, &voff, &vlen);
  if (k != NULL) *k = mg_str_n(s->ptr + koff, klen);
  if (v != NULL) *v = mg_str_n(s->ptr + voff, vlen);
  *s = mg_str_n(s->ptr + off, s->len - off);
  return off > 0;
}

uint32_t mg_ntohl(uint32_t net) {
  uint8_t data[4] = {0, 0, 0, 0};
  memcpy(&data, &net, sizeof(data));
  return ((uint32_t) data[3] << 0) | ((uint32_t) data[2] << 8) |
         ((uint32_t) data[1] << 16) | ((uint32_t) data[0] << 24);
}

uint16_t mg_ntohs(uint16_t net) {
  uint8_t data[2] = {0, 0};
  memcpy(&data, &net, sizeof(data));
  return ((uint16_t) data[1] << 0) | ((uint32_t) data[0] << 8);
}

char *mg_hexdump(const void *buf, size_t len) {
  const unsigned char *p = (const unsigned char *) buf;
  size_t i, idx, n = 0, ofs = 0, dlen = len * 5 + 100;
  char ascii[17] = "", *dst = (char *) malloc(dlen);
  if (dst == NULL) return dst;
  for (i = 0; i < len; i++) {
    idx = i % 16;
    if (idx == 0) {
      if (i > 0 && dlen > n) n += snprintf(dst + n, dlen - n, "  %s\n", ascii);
      if (dlen > n) n += snprintf(dst + n, dlen - n, "%04x ", (int) (i + ofs));
    }
    if (dlen < n) break;
    n += snprintf(dst + n, dlen - n, " %02x", p[i]);
    ascii[idx] = p[i] < 0x20 || p[i] > 0x7e ? '.' : p[i];
    ascii[idx + 1] = '\0';
  }
  while (i++ % 16) {
    if (n < dlen) n += snprintf(dst + n, dlen - n, "%s", "   ");
  }
  if (n < dlen) n += snprintf(dst + n, dlen - n, "  %s\n", ascii);
  if (n > dlen - 1) n = dlen - 1;
  dst[n] = '\0';
  return dst;
}

char *mg_hex(const void *buf, int len, char *to) {
  const unsigned char *p = (const unsigned char *) buf;
  static const char *hex = "0123456789abcdef";
  int i = 0;
  for (; len--; p++) {
    to[i++] = hex[p[0] >> 4];
    to[i++] = hex[p[0] & 0x0f];
  }
  to[i] = '\0';
  return to;
}

unsigned long mg_unhexn(const char *s, int len) {
  unsigned long i = 0, v = 0;
  for (i = 0; i < (unsigned long) len; i++) {
    int c = s[i];
    if (i > 0) v <<= 4;
    v |= (c >= '0' && c <= '9') ? c - '0'
                                : (c >= 'A' && c <= 'F') ? c - '7' : c - 'W';
  }
  return v;
}

void mg_unhex(const char *buf, int len, unsigned char *to) {
  int i;
  for (i = 0; i < len; i += 2) {
    to[i >> 1] = (unsigned char) mg_unhexn(&buf[i], 2);
  }
}

int mg_vasprintf(char **buf, size_t size, const char *fmt, va_list ap) {
  va_list ap_copy;
  int len;

  va_copy(ap_copy, ap);
  len = vsnprintf(*buf, size, fmt, ap_copy);
  va_end(ap_copy);

  if (len < 0) {
    // eCos and Windows are not standard-compliant and return -1 when
    // the buffer is too small. Keep allocating larger buffers until we
    // succeed or out of memory.
    // LCOV_EXCL_START
    *buf = NULL;
    while (len < 0) {
      free(*buf);
      if (size == 0) size = 5;
      size *= 2;
      if ((*buf = (char *) malloc(size)) == NULL) {
        len = -1;
        break;
      }
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, size - 1, fmt, ap_copy);
      va_end(ap_copy);
    }
    // Microsoft version of vsnprintf() is not always null-terminated, so put
    // the terminator manually
    if (*buf != NULL) (*buf)[len] = 0;
    // LCOV_EXCL_STOP
  } else if (len >= (int) size) {
    /// Standard-compliant code path. Allocate a buffer that is large enough
    if ((*buf = (char *) malloc(len + 1)) == NULL) {
      len = -1;  // LCOV_EXCL_LINE
    } else {     // LCOV_EXCL_LINE
      va_copy(ap_copy, ap);
      len = vsnprintf(*buf, len + 1, fmt, ap_copy);
      va_end(ap_copy);
    }
  }

  return len;
}

int mg_asprintf(char **buf, size_t size, const char *fmt, ...) {
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = mg_vasprintf(buf, size, fmt, ap);
  va_end(ap);
  return ret;
}

int64_t mg_to64(struct mg_str str) {
  int64_t result = 0, neg = 1;
  size_t i = 0;
  while (i < str.len && (str.ptr[i] == ' ' || str.ptr[i] == '\t')) i++;
  if (i < str.len && str.ptr[i] == '-') neg = -1, i++;
  while (i < str.len && str.ptr[i] >= '0' && str.ptr[i] <= '9') {
    result *= 10;
    result += (str.ptr[i] - '0');
    i++;
  }
  return result * neg;
}

uint32_t mg_crc32(uint32_t crc, const char *buf, size_t len) {
  int i;
  crc = ~crc;
  while (len--) {
    crc ^= *(unsigned char *) buf++;
    for (i = 0; i < 8; i++) crc = crc & 1 ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
  }
  return ~crc;
}

double mg_time(void) {
#if MG_ARCH == MG_ARCH_WIN32
  SYSTEMTIME sysnow;
  FILETIME ftime;
  GetLocalTime(&sysnow);
  SystemTimeToFileTime(&sysnow, &ftime);
  /*
   * 1. VC 6.0 doesn't support conversion uint64 -> double, so, using int64
   * This should not cause a problems in this (21th) century
   * 2. Windows FILETIME is a number of 100-nanosecond intervals since January
   * 1, 1601 while time_t is a number of _seconds_ since January 1, 1970 UTC,
   * thus, we need to convert to seconds and adjust amount (subtract 11644473600
   * seconds)
   */
  return (double) (((int64_t) ftime.dwLowDateTime +
                    ((int64_t) ftime.dwHighDateTime << 32)) /
                   10000000.0) -
         11644473600;
#else
  struct timeval tv;
  if (gettimeofday(&tv, NULL /* tz */) != 0) return 0;
  return (double) tv.tv_sec + (((double) tv.tv_usec) / 1000000.0);
#endif /* _WIN32 */
}

void mg_usleep(unsigned long usecs) {
#if MG_ARCH == MG_ARCH_WIN32
  Sleep(usecs / 1000);
#elif MG_ARCH == MG_ARCH_ESP8266
  ets_delay_us(usecs);
#else
  usleep(usecs);
#endif
}

unsigned long mg_millis(void) {
#if MG_ARCH == MG_ARCH_WIN32
  return GetTickCount();
#elif MG_ARCH == MG_ARCH_ESP32
  return esp_timer_get_time() / 1000;
#elif MG_ARCH == MG_ARCH_ESP8266
  // return system_get_time() / 1000;
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#elif MG_ARCH == MG_ARCH_FREERTOS
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
#else
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
#endif
}

#ifdef MG_ENABLE_LINES
#line 1 "src/ws.c"
#endif










struct ws_msg {
  uint8_t flags;
  size_t header_len;
  size_t data_len;
};

static void ws_handshake(struct mg_connection *c, const char *key,
                         size_t key_len, const char *fmt, va_list ap) {
  const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  unsigned char sha[20], b64_sha[30];
  char mem[128], *buf = mem;

  mg_sha1_ctx sha_ctx;
  mg_sha1_init(&sha_ctx);
  mg_sha1_update(&sha_ctx, (unsigned char *) key, key_len);
  mg_sha1_update(&sha_ctx, (unsigned char *) magic, 36);
  mg_sha1_final(sha, &sha_ctx);
  mg_base64_encode(sha, sizeof(sha), (char *) b64_sha);
  buf[0] = '\0';
  if (fmt != NULL) mg_vasprintf(&buf, sizeof(mem), fmt, ap);
  mg_printf(c,
            "HTTP/1.1 101 Switching Protocols\r\n"
            "Upgrade: websocket\r\n"
            "Connection: Upgrade\r\n"
            "Sec-WebSocket-Accept: %s\r\n"
            "%s\r\n",
            b64_sha, buf);
  if (buf != mem) free(buf);
}

static size_t ws_process(uint8_t *buf, size_t len, struct ws_msg *msg) {
  size_t i, n = 0, mask_len = 0;
  memset(msg, 0, sizeof(*msg));
  if (len >= 2) {
    n = buf[1] & 0x7f;
    mask_len = buf[1] & WEBSOCKET_FLAGS_MASK_FIN ? 4 : 0;
    msg->flags = *(unsigned char *) buf;
    if (n < 126 && len >= mask_len) {
      msg->data_len = n;
      msg->header_len = 2 + mask_len;
    } else if (n == 126 && len >= 4 + mask_len) {
      msg->header_len = 4 + mask_len;
      msg->data_len = mg_ntohs(*(uint16_t *) &buf[2]);
    } else if (len >= 10 + mask_len) {
      msg->header_len = 10 + mask_len;
      msg->data_len =
          (int) (((uint64_t) mg_ntohl(*(uint32_t *) &buf[2])) << 32) +
          mg_ntohl(*(uint32_t *) &buf[6]);
    }
  }
  if (msg->header_len + msg->data_len > len) return 0;
  if (mask_len > 0) {
    uint8_t *p = buf + msg->header_len, *m = p - mask_len;
    for (i = 0; i < msg->data_len; i++) p[i] ^= m[i & 3];
  }
  return msg->header_len + msg->data_len;
}

size_t mg_ws_send(struct mg_connection *c, const char *buf, size_t len,
                  int op) {
  unsigned char header[10], mask[4];
  size_t i, header_len = 0;
  header[0] = (uint8_t)(op | WEBSOCKET_FLAGS_MASK_FIN);
  if (len < 126) {
    header[1] = (unsigned char) len;
    header_len = 2;
  } else if (len < 65536) {
    uint16_t tmp = mg_htons((uint16_t) len);
    header[1] = 126;
    memcpy(&header[2], &tmp, sizeof(tmp));
    header_len = 4;
  } else {
    uint32_t tmp;
    header[1] = 127;
    tmp = mg_htonl((uint32_t)((uint64_t) len >> 32));
    memcpy(&header[2], &tmp, sizeof(tmp));
    tmp = mg_htonl((uint32_t)(len & 0xffffffff));
    memcpy(&header[6], &tmp, sizeof(tmp));
    header_len = 10;
  }
  if (c->is_client) header[1] |= 1 << 7;  // Set masking flag
  mg_send(c, header, header_len);
  if (c->is_client) {
    mg_random(mask, sizeof(mask));
    mg_send(c, mask, sizeof(mask));
    header_len += sizeof(mask);
  }
  LOG(LL_VERBOSE_DEBUG, ("WS out: %d [%.*s]", (int) len, (int) len, buf));
  mg_send(c, buf, len);
  if (c->is_client && c->send.buf != NULL) {
    uint8_t *p = c->send.buf + c->send.len - len;
    for (i = 0; i < len; i++) p[i] ^= mask[i & 3];
  }
  return header_len + len;
}

static void mg_ws_cb(struct mg_connection *c, int ev, void *ev_data,
                     void *fn_data) {
  struct ws_msg msg;

  if (ev == MG_EV_READ) {
    if (!c->is_websocket && c->is_client) {
      int n = mg_http_get_request_len(c->recv.buf, c->recv.len);
      if (n < 0) {
        c->is_closing = 1;  // Some just, not an HTTP request
      } else if (n > 0) {
        if (n < 15 || memcmp(c->recv.buf + 9, "101", 3) != 0) {
          LOG(LL_ERROR,
              ("%lu WS handshake error: %.*s", c->id, 15, c->recv.buf));
          c->is_closing = 1;
        } else {
          struct mg_http_message hm;
          mg_http_parse((char *) c->recv.buf, c->recv.len, &hm);
          c->is_websocket = 1;
          mg_call(c, MG_EV_WS_OPEN, &hm);
        }
        mg_iobuf_delete(&c->recv, n);
      } else {
        return;  // A request is not yet received
      }
    }

    while (ws_process(c->recv.buf, c->recv.len, &msg) > 0) {
      char *s = (char *) c->recv.buf + msg.header_len;
      struct mg_ws_message m = {{s, msg.data_len}, msg.flags};
      switch (msg.flags & WEBSOCKET_FLAGS_MASK_OP) {
        case WEBSOCKET_OP_PING:
          LOG(LL_DEBUG, ("%s", "WS PONG"));
          mg_ws_send(c, s, msg.data_len, WEBSOCKET_OP_PONG);
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_PONG:
          mg_call(c, MG_EV_WS_CTL, &m);
          break;
        case WEBSOCKET_OP_CLOSE:
          LOG(LL_ERROR, ("%lu Got WS CLOSE", c->id));
          mg_call(c, MG_EV_WS_CTL, &m);
          c->is_closing = 1;
          return;
        default: {
          mg_call(c, MG_EV_WS_MSG, &m);
          break;
        }
      }
      mg_iobuf_delete(&c->recv, msg.header_len + msg.data_len);
    }
  }
  (void) fn_data;
  (void) ev_data;
}

struct mg_connection *mg_ws_connect(struct mg_mgr *mgr, const char *url,
                                    mg_event_handler_t fn, void *fn_data,
                                    const char *fmt, ...) {
  struct mg_connection *c = mg_connect(mgr, url, fn, fn_data);
  if (c != NULL) {
    char nonce[16], key[30], mem1[128], mem2[256], *buf1 = mem1, *buf2 = mem2;
    struct mg_str host = mg_url_host(url);
    int n1 = 0, n2 = 0;
    if (fmt != NULL) {
      va_list ap;
      va_start(ap, fmt);
      n1 = mg_vasprintf(&buf1, sizeof(mem1), fmt, ap);
      va_end(ap);
    }
    // Send handshake request
    // mg_url_host(url, host, sizeof(host));
    mg_random(nonce, sizeof(nonce));
    mg_base64_encode((unsigned char *) nonce, sizeof(nonce), key);
    // LOG(LL_DEBUG, "%p [%s]", uri, uri == NULL ? "???" : uri);
    n2 = mg_asprintf(&buf2, sizeof(mem2),
                     "GET %s HTTP/1.1\r\n"
                     "Upgrade: websocket\r\n"
                     "Host: %.*s\r\n"
                     "Connection: Upgrade\r\n"
                     "%.*s"
                     "Sec-WebSocket-Version: 13\r\n"
                     "Sec-WebSocket-Key: %s\r\n"
                     "\r\n",
                     mg_url_uri(url), (int) host.len, host.ptr, n1, buf1, key);
    mg_send(c, buf2, n2);
    if (buf1 != mem1) free(buf1);
    if (buf2 != mem2) free(buf2);
    c->pfn = mg_ws_cb;
    c->fn_data = fn_data;
  }
  return c;
}

void mg_ws_upgrade(struct mg_connection *c, struct mg_http_message *hm,
                   const char *fmt, ...) {
  struct mg_str *wskey = mg_http_get_header(hm, "Sec-WebSocket-Key");
  c->pfn = mg_ws_cb;
  if (wskey != NULL) {
    va_list ap;
    va_start(ap, fmt);
    ws_handshake(c, wskey->ptr, wskey->len, fmt, ap);
    va_end(ap);
  }
  c->is_websocket = 1;
}
