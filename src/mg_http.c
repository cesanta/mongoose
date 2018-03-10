/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_HTTP

#include "common/cs_md5.h"
#include "mg_internal.h"
#include "mg_util.h"

/* altbuf {{{ */

/*
 * Alternate buffer: fills the client-provided buffer with data; and if it's
 * not large enough, allocates another buffer (via mbuf), similar to asprintf.
 */
struct altbuf {
  struct mbuf m;
  char *user_buf;
  size_t len;
  size_t user_buf_size;
};

/*
 * Initializes altbuf; `buf`, `buf_size` is the client-provided buffer.
 */
MG_INTERNAL void altbuf_init(struct altbuf *ab, char *buf, size_t buf_size) {
  mbuf_init(&ab->m, 0);
  ab->user_buf = buf;
  ab->user_buf_size = buf_size;
  ab->len = 0;
}

/*
 * Appends a single char to the altbuf.
 */
MG_INTERNAL void altbuf_append(struct altbuf *ab, char c) {
  if (ab->len < ab->user_buf_size) {
    /* The data fits into the original buffer */
    ab->user_buf[ab->len++] = c;
  } else {
    /* The data can't fit into the original buffer, so write it to mbuf.  */

    /*
     * First of all, see if that's the first byte which overflows the original
     * buffer: if so, copy the existing data from there to a newly allocated
     * mbuf.
     */
    if (ab->len > 0 && ab->m.len == 0) {
      mbuf_append(&ab->m, ab->user_buf, ab->len);
    }

    mbuf_append(&ab->m, &c, 1);
    ab->len = ab->m.len;
  }
}

/*
 * Resets any data previously appended to altbuf.
 */
MG_INTERNAL void altbuf_reset(struct altbuf *ab) {
  mbuf_free(&ab->m);
  ab->len = 0;
}

/*
 * Returns whether the additional buffer was allocated (and thus the data
 * is in the mbuf, not the client-provided buffer)
 */
MG_INTERNAL int altbuf_reallocated(struct altbuf *ab) {
  return ab->len > ab->user_buf_size;
}

/*
 * Returns the actual buffer with data, either the client-provided or a newly
 * allocated one. If `trim` is non-zero, mbuf-backed buffer is trimmed first.
 */
MG_INTERNAL char *altbuf_get_buf(struct altbuf *ab, int trim) {
  if (altbuf_reallocated(ab)) {
    if (trim) {
      mbuf_trim(&ab->m);
    }
    return ab->m.buf;
  } else {
    return ab->user_buf;
  }
}

/* }}} */

static const char *mg_version_header = "Mongoose/" MG_VERSION;

enum mg_http_proto_data_type { DATA_NONE, DATA_FILE, DATA_PUT };

struct mg_http_proto_data_file {
  FILE *fp;      /* Opened file. */
  int64_t cl;    /* Content-Length. How many bytes to send. */
  int64_t sent;  /* How many bytes have been already sent. */
  int keepalive; /* Keep connection open after sending. */
  enum mg_http_proto_data_type type;
};

#if MG_ENABLE_HTTP_CGI
struct mg_http_proto_data_cgi {
  struct mg_connection *cgi_nc;
};
#endif

struct mg_http_proto_data_chuncked {
  int64_t body_len; /* How many bytes of chunked body was reassembled. */
};

struct mg_http_endpoint {
  struct mg_http_endpoint *next;
  struct mg_str uri_pattern; /* owned */
  char *auth_domain;         /* owned */
  char *auth_file;           /* owned */

  mg_event_handler_t handler;
#if MG_ENABLE_CALLBACK_USERDATA
  void *user_data;
#endif
};

enum mg_http_multipart_stream_state {
  MPS_BEGIN,
  MPS_WAITING_FOR_BOUNDARY,
  MPS_WAITING_FOR_CHUNK,
  MPS_GOT_CHUNK,
  MPS_GOT_BOUNDARY,
  MPS_FINALIZE,
  MPS_FINISHED
};

struct mg_http_multipart_stream {
  const char *boundary;
  int boundary_len;
  const char *var_name;
  const char *file_name;
  void *user_data;
  int prev_io_len;
  enum mg_http_multipart_stream_state state;
  int processing_part;
};

struct mg_reverse_proxy_data {
  struct mg_connection *linked_conn;
};

struct mg_ws_proto_data {
  /*
   * Defragmented size of the frame so far.
   *
   * First byte of nc->recv_mbuf.buf is an op, the rest of the data is
   * defragmented data.
   */
  size_t reass_len;
};

struct mg_http_proto_data {
#if MG_ENABLE_FILESYSTEM
  struct mg_http_proto_data_file file;
#endif
#if MG_ENABLE_HTTP_CGI
  struct mg_http_proto_data_cgi cgi;
#endif
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
  struct mg_http_multipart_stream mp_stream;
#endif
#if MG_ENABLE_HTTP_WEBSOCKET
  struct mg_ws_proto_data ws_data;
#endif
  struct mg_http_proto_data_chuncked chunk;
  struct mg_http_endpoint *endpoints;
  mg_event_handler_t endpoint_handler;
  struct mg_reverse_proxy_data reverse_proxy_data;
  size_t rcvd; /* How many bytes we have received. */
};

static void mg_http_conn_destructor(void *proto_data);
struct mg_connection *mg_connect_http_base(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    struct mg_connect_opts opts, const char *scheme1, const char *scheme2,
    const char *scheme_ssl1, const char *scheme_ssl2, const char *url,
    struct mg_str *path, struct mg_str *user_info, struct mg_str *host);

static struct mg_http_proto_data *mg_http_get_proto_data(
    struct mg_connection *c) {
  if (c->proto_data == NULL) {
    c->proto_data = MG_CALLOC(1, sizeof(struct mg_http_proto_data));
    c->proto_data_destructor = mg_http_conn_destructor;
  }

  return (struct mg_http_proto_data *) c->proto_data;
}

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_free_proto_data_mp_stream(
    struct mg_http_multipart_stream *mp) {
  MG_FREE((void *) mp->boundary);
  MG_FREE((void *) mp->var_name);
  MG_FREE((void *) mp->file_name);
  memset(mp, 0, sizeof(*mp));
}
#endif

#if MG_ENABLE_FILESYSTEM
static void mg_http_free_proto_data_file(struct mg_http_proto_data_file *d) {
  if (d != NULL) {
    if (d->fp != NULL) {
      fclose(d->fp);
    }
    memset(d, 0, sizeof(struct mg_http_proto_data_file));
  }
}
#endif

static void mg_http_free_proto_data_endpoints(struct mg_http_endpoint **ep) {
  struct mg_http_endpoint *current = *ep;

  while (current != NULL) {
    struct mg_http_endpoint *tmp = current->next;
    MG_FREE((void *) current->uri_pattern.p);
    MG_FREE((void *) current->auth_domain);
    MG_FREE((void *) current->auth_file);
    MG_FREE(current);
    current = tmp;
  }

  ep = NULL;
}

static void mg_http_free_reverse_proxy_data(struct mg_reverse_proxy_data *rpd) {
  if (rpd->linked_conn != NULL) {
    /*
     * Connection has linked one, we have to unlink & close it
     * since _this_ connection is going to die and
     * it doesn't make sense to keep another one
     */
    struct mg_http_proto_data *pd = mg_http_get_proto_data(rpd->linked_conn);
    if (pd->reverse_proxy_data.linked_conn != NULL) {
      pd->reverse_proxy_data.linked_conn->flags |= MG_F_SEND_AND_CLOSE;
      pd->reverse_proxy_data.linked_conn = NULL;
    }
    rpd->linked_conn = NULL;
  }
}

static void mg_http_conn_destructor(void *proto_data) {
  struct mg_http_proto_data *pd = (struct mg_http_proto_data *) proto_data;
#if MG_ENABLE_FILESYSTEM
  mg_http_free_proto_data_file(&pd->file);
#endif
#if MG_ENABLE_HTTP_CGI
  mg_http_free_proto_data_cgi(&pd->cgi);
#endif
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
  mg_http_free_proto_data_mp_stream(&pd->mp_stream);
#endif
  mg_http_free_proto_data_endpoints(&pd->endpoints);
  mg_http_free_reverse_proxy_data(&pd->reverse_proxy_data);
  MG_FREE(proto_data);
}

#if MG_ENABLE_FILESYSTEM

#define MIME_ENTRY(_ext, _type) \
  { _ext, sizeof(_ext) - 1, _type }
static const struct {
  const char *extension;
  size_t ext_len;
  const char *mime_type;
} mg_static_builtin_mime_types[] = {
    MIME_ENTRY("html", "text/html"),
    MIME_ENTRY("html", "text/html"),
    MIME_ENTRY("htm", "text/html"),
    MIME_ENTRY("shtm", "text/html"),
    MIME_ENTRY("shtml", "text/html"),
    MIME_ENTRY("css", "text/css"),
    MIME_ENTRY("js", "application/x-javascript"),
    MIME_ENTRY("ico", "image/x-icon"),
    MIME_ENTRY("gif", "image/gif"),
    MIME_ENTRY("jpg", "image/jpeg"),
    MIME_ENTRY("jpeg", "image/jpeg"),
    MIME_ENTRY("png", "image/png"),
    MIME_ENTRY("svg", "image/svg+xml"),
    MIME_ENTRY("txt", "text/plain"),
    MIME_ENTRY("torrent", "application/x-bittorrent"),
    MIME_ENTRY("wav", "audio/x-wav"),
    MIME_ENTRY("mp3", "audio/x-mp3"),
    MIME_ENTRY("mid", "audio/mid"),
    MIME_ENTRY("m3u", "audio/x-mpegurl"),
    MIME_ENTRY("ogg", "application/ogg"),
    MIME_ENTRY("ram", "audio/x-pn-realaudio"),
    MIME_ENTRY("xml", "text/xml"),
    MIME_ENTRY("ttf", "application/x-font-ttf"),
    MIME_ENTRY("json", "application/json"),
    MIME_ENTRY("xslt", "application/xml"),
    MIME_ENTRY("xsl", "application/xml"),
    MIME_ENTRY("ra", "audio/x-pn-realaudio"),
    MIME_ENTRY("doc", "application/msword"),
    MIME_ENTRY("exe", "application/octet-stream"),
    MIME_ENTRY("zip", "application/x-zip-compressed"),
    MIME_ENTRY("xls", "application/excel"),
    MIME_ENTRY("tgz", "application/x-tar-gz"),
    MIME_ENTRY("tar", "application/x-tar"),
    MIME_ENTRY("gz", "application/x-gunzip"),
    MIME_ENTRY("arj", "application/x-arj-compressed"),
    MIME_ENTRY("rar", "application/x-rar-compressed"),
    MIME_ENTRY("rtf", "application/rtf"),
    MIME_ENTRY("pdf", "application/pdf"),
    MIME_ENTRY("swf", "application/x-shockwave-flash"),
    MIME_ENTRY("mpg", "video/mpeg"),
    MIME_ENTRY("webm", "video/webm"),
    MIME_ENTRY("mpeg", "video/mpeg"),
    MIME_ENTRY("mov", "video/quicktime"),
    MIME_ENTRY("mp4", "video/mp4"),
    MIME_ENTRY("m4v", "video/x-m4v"),
    MIME_ENTRY("asf", "video/x-ms-asf"),
    MIME_ENTRY("avi", "video/x-msvideo"),
    MIME_ENTRY("bmp", "image/bmp"),
    {NULL, 0, NULL}};

static struct mg_str mg_get_mime_type(const char *path, const char *dflt,
                                      const struct mg_serve_http_opts *opts) {
  const char *ext, *overrides;
  size_t i, path_len;
  struct mg_str r, k, v;

  path_len = strlen(path);

  overrides = opts->custom_mime_types;
  while ((overrides = mg_next_comma_list_entry(overrides, &k, &v)) != NULL) {
    ext = path + (path_len - k.len);
    if (path_len > k.len && mg_vcasecmp(&k, ext) == 0) {
      return v;
    }
  }

  for (i = 0; mg_static_builtin_mime_types[i].extension != NULL; i++) {
    ext = path + (path_len - mg_static_builtin_mime_types[i].ext_len);
    if (path_len > mg_static_builtin_mime_types[i].ext_len && ext[-1] == '.' &&
        mg_casecmp(ext, mg_static_builtin_mime_types[i].extension) == 0) {
      r.p = mg_static_builtin_mime_types[i].mime_type;
      r.len = strlen(r.p);
      return r;
    }
  }

  r.p = dflt;
  r.len = strlen(r.p);
  return r;
}
#endif

/*
 * Check whether full request is buffered. Return:
 *   -1  if request is malformed
 *    0  if request is not yet fully buffered
 *   >0  actual request length, including last \r\n\r\n
 */
static int mg_http_get_request_len(const char *s, int buf_len) {
  const unsigned char *buf = (unsigned char *) s;
  int i;

  for (i = 0; i < buf_len; i++) {
    if (!isprint(buf[i]) && buf[i] != '\r' && buf[i] != '\n' && buf[i] < 128) {
      return -1;
    } else if (buf[i] == '\n' && i + 1 < buf_len && buf[i + 1] == '\n') {
      return i + 2;
    } else if (buf[i] == '\n' && i + 2 < buf_len && buf[i + 1] == '\r' &&
               buf[i + 2] == '\n') {
      return i + 3;
    }
  }

  return 0;
}

static const char *mg_http_parse_headers(const char *s, const char *end,
                                         int len, struct http_message *req) {
  int i = 0;
  while (i < (int) ARRAY_SIZE(req->header_names) - 1) {
    struct mg_str *k = &req->header_names[i], *v = &req->header_values[i];

    s = mg_skip(s, end, ": ", k);
    s = mg_skip(s, end, "\r\n", v);

    while (v->len > 0 && v->p[v->len - 1] == ' ') {
      v->len--; /* Trim trailing spaces in header value */
    }

    /*
     * If header value is empty - skip it and go to next (if any).
     * NOTE: Do not add it to headers_values because such addition changes API
     * behaviour
     */
    if (k->len != 0 && v->len == 0) {
      continue;
    }

    if (k->len == 0 || v->len == 0) {
      k->p = v->p = NULL;
      k->len = v->len = 0;
      break;
    }

    if (!mg_ncasecmp(k->p, "Content-Length", 14)) {
      req->body.len = (size_t) to64(v->p);
      req->message.len = len + req->body.len;
    }

    i++;
  }

  return s;
}

int mg_parse_http(const char *s, int n, struct http_message *hm, int is_req) {
  const char *end, *qs;
  int len = mg_http_get_request_len(s, n);

  if (len <= 0) return len;

  memset(hm, 0, sizeof(*hm));
  hm->message.p = s;
  hm->body.p = s + len;
  hm->message.len = hm->body.len = (size_t) ~0;
  end = s + len;

  /* Request is fully buffered. Skip leading whitespaces. */
  while (s < end && isspace(*(unsigned char *) s)) s++;

  if (is_req) {
    /* Parse request line: method, URI, proto */
    s = mg_skip(s, end, " ", &hm->method);
    s = mg_skip(s, end, " ", &hm->uri);
    s = mg_skip(s, end, "\r\n", &hm->proto);
    if (hm->uri.p <= hm->method.p || hm->proto.p <= hm->uri.p) return -1;

    /* If URI contains '?' character, initialize query_string */
    if ((qs = (char *) memchr(hm->uri.p, '?', hm->uri.len)) != NULL) {
      hm->query_string.p = qs + 1;
      hm->query_string.len = &hm->uri.p[hm->uri.len] - (qs + 1);
      hm->uri.len = qs - hm->uri.p;
    }
  } else {
    s = mg_skip(s, end, " ", &hm->proto);
    if (end - s < 4 || s[3] != ' ') return -1;
    hm->resp_code = atoi(s);
    if (hm->resp_code < 100 || hm->resp_code >= 600) return -1;
    s += 4;
    s = mg_skip(s, end, "\r\n", &hm->resp_status_msg);
  }

  s = mg_http_parse_headers(s, end, len, hm);

  /*
   * mg_parse_http() is used to parse both HTTP requests and HTTP
   * responses. If HTTP response does not have Content-Length set, then
   * body is read until socket is closed, i.e. body.len is infinite (~0).
   *
   * For HTTP requests though, according to
   * http://tools.ietf.org/html/rfc7231#section-8.1.3,
   * only POST and PUT methods have defined body semantics.
   * Therefore, if Content-Length is not specified and methods are
   * not one of PUT or POST, set body length to 0.
   *
   * So,
   * if it is HTTP request, and Content-Length is not set,
   * and method is not (PUT or POST) then reset body length to zero.
   */
  if (hm->body.len == (size_t) ~0 && is_req &&
      mg_vcasecmp(&hm->method, "PUT") != 0 &&
      mg_vcasecmp(&hm->method, "POST") != 0) {
    hm->body.len = 0;
    hm->message.len = len;
  }

  return len;
}

struct mg_str *mg_get_http_header(struct http_message *hm, const char *name) {
  size_t i, len = strlen(name);

  for (i = 0; hm->header_names[i].len > 0; i++) {
    struct mg_str *h = &hm->header_names[i], *v = &hm->header_values[i];
    if (h->p != NULL && h->len == len && !mg_ncasecmp(h->p, name, len))
      return v;
  }

  return NULL;
}

#if MG_ENABLE_FILESYSTEM
static void mg_http_transfer_file_data(struct mg_connection *nc) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  char buf[MG_MAX_HTTP_SEND_MBUF];
  size_t n = 0, to_read = 0, left = (size_t)(pd->file.cl - pd->file.sent);

  if (pd->file.type == DATA_FILE) {
    struct mbuf *io = &nc->send_mbuf;
    if (io->len >= MG_MAX_HTTP_SEND_MBUF) {
      to_read = 0;
    } else {
      to_read = MG_MAX_HTTP_SEND_MBUF - io->len;
    }
    if (to_read > left) {
      to_read = left;
    }
    if (to_read > 0) {
      n = mg_fread(buf, 1, to_read, pd->file.fp);
      if (n > 0) {
        mg_send(nc, buf, n);
        pd->file.sent += n;
        DBG(("%p sent %d (total %d)", nc, (int) n, (int) pd->file.sent));
      }
    } else {
      /* Rate-limited */
    }
    if (pd->file.sent >= pd->file.cl) {
      LOG(LL_DEBUG, ("%p done, %d bytes", nc, (int) pd->file.sent));
      if (!pd->file.keepalive) nc->flags |= MG_F_SEND_AND_CLOSE;
      mg_http_free_proto_data_file(&pd->file);
    }
  } else if (pd->file.type == DATA_PUT) {
    struct mbuf *io = &nc->recv_mbuf;
    size_t to_write = left <= 0 ? 0 : left < io->len ? (size_t) left : io->len;
    size_t n = mg_fwrite(io->buf, 1, to_write, pd->file.fp);
    if (n > 0) {
      mbuf_remove(io, n);
      pd->file.sent += n;
    }
    if (n == 0 || pd->file.sent >= pd->file.cl) {
      if (!pd->file.keepalive) nc->flags |= MG_F_SEND_AND_CLOSE;
      mg_http_free_proto_data_file(&pd->file);
    }
  }
#if MG_ENABLE_HTTP_CGI
  else if (pd->cgi.cgi_nc != NULL) {
    /* This is POST data that needs to be forwarded to the CGI process */
    if (pd->cgi.cgi_nc != NULL) {
      mg_forward(nc, pd->cgi.cgi_nc);
    } else {
      nc->flags |= MG_F_SEND_AND_CLOSE;
    }
  }
#endif
}
#endif /* MG_ENABLE_FILESYSTEM */

/*
 * Parse chunked-encoded buffer. Return 0 if the buffer is not encoded, or
 * if it's incomplete. If the chunk is fully buffered, return total number of
 * bytes in a chunk, and store data in `data`, `data_len`.
 */
static size_t mg_http_parse_chunk(char *buf, size_t len, char **chunk_data,
                                  size_t *chunk_len) {
  unsigned char *s = (unsigned char *) buf;
  size_t n = 0; /* scanned chunk length */
  size_t i = 0; /* index in s */

  /* Scan chunk length. That should be a hexadecimal number. */
  while (i < len && isxdigit(s[i])) {
    n *= 16;
    n += (s[i] >= '0' && s[i] <= '9') ? s[i] - '0' : tolower(s[i]) - 'a' + 10;
    i++;
  }

  /* Skip new line */
  if (i == 0 || i + 2 > len || s[i] != '\r' || s[i + 1] != '\n') {
    return 0;
  }
  i += 2;

  /* Record where the data is */
  *chunk_data = (char *) s + i;
  *chunk_len = n;

  /* Skip data */
  i += n;

  /* Skip new line */
  if (i == 0 || i + 2 > len || s[i] != '\r' || s[i + 1] != '\n') {
    return 0;
  }
  return i + 2;
}

MG_INTERNAL size_t mg_handle_chunked(struct mg_connection *nc,
                                     struct http_message *hm, char *buf,
                                     size_t blen) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  char *data;
  size_t i, n, data_len, body_len, zero_chunk_received = 0;
  /* Find out piece of received data that is not yet reassembled */
  body_len = (size_t) pd->chunk.body_len;
  assert(blen >= body_len);

  /* Traverse all fully buffered chunks */
  for (i = body_len;
       (n = mg_http_parse_chunk(buf + i, blen - i, &data, &data_len)) > 0;
       i += n) {
    /* Collapse chunk data to the rest of HTTP body */
    memmove(buf + body_len, data, data_len);
    body_len += data_len;
    hm->body.len = body_len;

    if (data_len == 0) {
      zero_chunk_received = 1;
      i += n;
      break;
    }
  }

  if (i > body_len) {
    /* Shift unparsed content to the parsed body */
    assert(i <= blen);
    memmove(buf + body_len, buf + i, blen - i);
    memset(buf + body_len + blen - i, 0, i - body_len);
    nc->recv_mbuf.len -= i - body_len;
    pd->chunk.body_len = body_len;

    /* Send MG_EV_HTTP_CHUNK event */
    nc->flags &= ~MG_F_DELETE_CHUNK;
    mg_call(nc, nc->handler, nc->user_data, MG_EV_HTTP_CHUNK, hm);

    /* Delete processed data if user set MG_F_DELETE_CHUNK flag */
    if (nc->flags & MG_F_DELETE_CHUNK) {
      memset(buf, 0, body_len);
      memmove(buf, buf + body_len, blen - i);
      nc->recv_mbuf.len -= body_len;
      hm->body.len = 0;
      pd->chunk.body_len = 0;
    }

    if (zero_chunk_received) {
      /* Total message size is len(body) + len(headers) */
      hm->message.len =
          (size_t) pd->chunk.body_len + blen - i + (hm->body.p - hm->message.p);
    }
  }

  return body_len;
}

struct mg_http_endpoint *mg_http_get_endpoint_handler(struct mg_connection *nc,
                                                      struct mg_str *uri_path) {
  struct mg_http_proto_data *pd;
  struct mg_http_endpoint *ret = NULL;
  int matched, matched_max = 0;
  struct mg_http_endpoint *ep;

  if (nc == NULL) {
    return NULL;
  }

  pd = mg_http_get_proto_data(nc);

  ep = pd->endpoints;
  while (ep != NULL) {
    if ((matched = mg_match_prefix_n(ep->uri_pattern, *uri_path)) > 0) {
      if (matched > matched_max) {
        /* Looking for the longest suitable handler */
        ret = ep;
        matched_max = matched;
      }
    }

    ep = ep->next;
  }

  return ret;
}

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_multipart_continue(struct mg_connection *nc);

static void mg_http_multipart_begin(struct mg_connection *nc,
                                    struct http_message *hm, int req_len);

#endif

static void mg_http_call_endpoint_handler(struct mg_connection *nc, int ev,
                                          struct http_message *hm);

static void deliver_chunk(struct mg_connection *c, struct http_message *hm,
                          int req_len) {
  /* Incomplete message received. Send MG_EV_HTTP_CHUNK event */
  hm->body.len = c->recv_mbuf.len - req_len;
  c->flags &= ~MG_F_DELETE_CHUNK;
  mg_call(c, c->handler, c->user_data, MG_EV_HTTP_CHUNK, hm);
  /* Delete processed data if user set MG_F_DELETE_CHUNK flag */
  if (c->flags & MG_F_DELETE_CHUNK) c->recv_mbuf.len = req_len;
}

/*
 * lx106 compiler has a bug (TODO(mkm) report and insert tracking bug here)
 * If a big structure is declared in a big function, lx106 gcc will make it
 * even bigger (round up to 4k, from 700 bytes of actual size).
 */
#ifdef __xtensa__
static void mg_http_handler2(struct mg_connection *nc, int ev,
                             void *ev_data MG_UD_ARG(void *user_data),
                             struct http_message *hm) __attribute__((noinline));

void mg_http_handler(struct mg_connection *nc, int ev,
                     void *ev_data MG_UD_ARG(void *user_data)) {
  struct http_message hm;
  mg_http_handler2(nc, ev, ev_data MG_UD_ARG(user_data), &hm);
}

static void mg_http_handler2(struct mg_connection *nc, int ev,
                             void *ev_data MG_UD_ARG(void *user_data),
                             struct http_message *hm) {
#else  /* !__XTENSA__ */
void mg_http_handler(struct mg_connection *nc, int ev,
                     void *ev_data MG_UD_ARG(void *user_data)) {
  struct http_message shm, *hm = &shm;
#endif /* __XTENSA__ */
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mbuf *io = &nc->recv_mbuf;
  int req_len;
  const int is_req = (nc->listener != NULL);
#if MG_ENABLE_HTTP_WEBSOCKET
  struct mg_str *vec;
#endif
  if (ev == MG_EV_CLOSE) {
#if MG_ENABLE_HTTP_CGI
    /* Close associated CGI forwarder connection */
    if (pd->cgi.cgi_nc != NULL) {
      pd->cgi.cgi_nc->user_data = NULL;
      pd->cgi.cgi_nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    }
#endif
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (pd->mp_stream.boundary != NULL) {
      /*
       * Multipart message is in progress, but connection is closed.
       * Finish part and request with an error flag.
       */
      struct mg_http_multipart_part mp;
      memset(&mp, 0, sizeof(mp));
      mp.status = -1;
      mp.var_name = pd->mp_stream.var_name;
      mp.file_name = pd->mp_stream.file_name;
      mg_call(nc, (pd->endpoint_handler ? pd->endpoint_handler : nc->handler),
              nc->user_data, MG_EV_HTTP_PART_END, &mp);
      mp.var_name = NULL;
      mp.file_name = NULL;
      mg_call(nc, (pd->endpoint_handler ? pd->endpoint_handler : nc->handler),
              nc->user_data, MG_EV_HTTP_MULTIPART_REQUEST_END, &mp);
    } else
#endif
        if (io->len > 0 &&
            (req_len = mg_parse_http(io->buf, io->len, hm, is_req)) > 0) {
      /*
      * For HTTP messages without Content-Length, always send HTTP message
      * before MG_EV_CLOSE message.
      */
      int ev2 = is_req ? MG_EV_HTTP_REQUEST : MG_EV_HTTP_REPLY;
      hm->message.len = io->len;
      hm->body.len = io->buf + io->len - hm->body.p;
      deliver_chunk(nc, hm, req_len);
      mg_http_call_endpoint_handler(nc, ev2, hm);
    }
    pd->rcvd = 0;
  }

#if MG_ENABLE_FILESYSTEM
  if (pd->file.fp != NULL) {
    mg_http_transfer_file_data(nc);
  }
#endif

  mg_call(nc, nc->handler, nc->user_data, ev, ev_data);

  if (ev == MG_EV_RECV) {
    struct mg_str *s;
    pd->rcvd += *(int *) ev_data;

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (pd->mp_stream.boundary != NULL) {
      mg_http_multipart_continue(nc);
      return;
    }
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

    req_len = mg_parse_http(io->buf, io->len, hm, is_req);

    if (req_len > 0 &&
        (s = mg_get_http_header(hm, "Transfer-Encoding")) != NULL &&
        mg_vcasecmp(s, "chunked") == 0) {
      mg_handle_chunked(nc, hm, io->buf + req_len, io->len - req_len);
    }

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
    if (req_len > 0 && (s = mg_get_http_header(hm, "Content-Type")) != NULL &&
        s->len >= 9 && strncmp(s->p, "multipart", 9) == 0) {
      mg_http_multipart_begin(nc, hm, req_len);
      mg_http_multipart_continue(nc);
      return;
    }
#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

    /* TODO(alashkin): refactor this ifelseifelseifelseifelse */
    if ((req_len < 0 ||
         (req_len == 0 && io->len >= MG_MAX_HTTP_REQUEST_SIZE))) {
      DBG(("invalid request"));
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
    } else if (req_len == 0) {
      /* Do nothing, request is not yet fully buffered */
    }
#if MG_ENABLE_HTTP_WEBSOCKET
    else if (nc->listener == NULL &&
             mg_get_http_header(hm, "Sec-WebSocket-Accept")) {
      /* We're websocket client, got handshake response from server. */
      /* TODO(lsm): check the validity of accept Sec-WebSocket-Accept */
      mbuf_remove(io, req_len);
      nc->proto_handler = mg_ws_handler;
      nc->flags |= MG_F_IS_WEBSOCKET;
      mg_call(nc, nc->handler, nc->user_data, MG_EV_WEBSOCKET_HANDSHAKE_DONE,
              NULL);
      mg_ws_handler(nc, MG_EV_RECV, ev_data MG_UD_ARG(user_data));
    } else if (nc->listener != NULL &&
               (vec = mg_get_http_header(hm, "Sec-WebSocket-Key")) != NULL) {
      struct mg_http_endpoint *ep;

      /* This is a websocket request. Switch protocol handlers. */
      mbuf_remove(io, req_len);
      nc->proto_handler = mg_ws_handler;
      nc->flags |= MG_F_IS_WEBSOCKET;

      /*
       * If we have a handler set up with mg_register_http_endpoint(),
       * deliver subsequent websocket events to this handler after the
       * protocol switch.
       */
      ep = mg_http_get_endpoint_handler(nc->listener, &hm->uri);
      if (ep != NULL) {
        nc->handler = ep->handler;
#if MG_ENABLE_CALLBACK_USERDATA
        nc->user_data = ep->user_data;
#endif
      }

      /* Send handshake */
      mg_call(nc, nc->handler, nc->user_data, MG_EV_WEBSOCKET_HANDSHAKE_REQUEST,
              hm);
      if (!(nc->flags & (MG_F_CLOSE_IMMEDIATELY | MG_F_SEND_AND_CLOSE))) {
        if (nc->send_mbuf.len == 0) {
          mg_ws_handshake(nc, vec, hm);
        }
        mg_call(nc, nc->handler, nc->user_data, MG_EV_WEBSOCKET_HANDSHAKE_DONE,
                NULL);
        mg_ws_handler(nc, MG_EV_RECV, ev_data MG_UD_ARG(user_data));
      }
    }
#endif /* MG_ENABLE_HTTP_WEBSOCKET */
    else if (hm->message.len > pd->rcvd) {
      /* Not yet received all HTTP body, deliver MG_EV_HTTP_CHUNK */
      deliver_chunk(nc, hm, req_len);
      if (nc->recv_mbuf_limit > 0 && nc->recv_mbuf.len >= nc->recv_mbuf_limit) {
        LOG(LL_ERROR, ("%p recv buffer (%lu bytes) exceeds the limit "
                       "%lu bytes, and not drained, closing",
                       nc, (unsigned long) nc->recv_mbuf.len,
                       (unsigned long) nc->recv_mbuf_limit));
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
    } else {
      /* We did receive all HTTP body. */
      int trigger_ev = nc->listener ? MG_EV_HTTP_REQUEST : MG_EV_HTTP_REPLY;
      char addr[32];
      mg_sock_addr_to_str(&nc->sa, addr, sizeof(addr),
                          MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
      DBG(("%p %s %.*s %.*s", nc, addr, (int) hm->method.len, hm->method.p,
           (int) hm->uri.len, hm->uri.p));
      deliver_chunk(nc, hm, req_len);
      /* Whole HTTP message is fully buffered, call event handler */
      mg_http_call_endpoint_handler(nc, trigger_ev, hm);
      mbuf_remove(io, hm->message.len);
      pd->rcvd = 0;
    }
  }
}

static size_t mg_get_line_len(const char *buf, size_t buf_len) {
  size_t len = 0;
  while (len < buf_len && buf[len] != '\n') len++;
  return len == buf_len ? 0 : len + 1;
}

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
static void mg_http_multipart_begin(struct mg_connection *nc,
                                    struct http_message *hm, int req_len) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  struct mg_str *ct;
  struct mbuf *io = &nc->recv_mbuf;

  char boundary_buf[100];
  char *boundary = boundary_buf;
  int boundary_len;

  ct = mg_get_http_header(hm, "Content-Type");
  if (ct == NULL) {
    /* We need more data - or it isn't multipart mesage */
    goto exit_mp;
  }

  /* Content-type should start with "multipart" */
  if (ct->len < 9 || strncmp(ct->p, "multipart", 9) != 0) {
    goto exit_mp;
  }

  boundary_len =
      mg_http_parse_header2(ct, "boundary", &boundary, sizeof(boundary_buf));
  if (boundary_len == 0) {
    /*
     * Content type is multipart, but there is no boundary,
     * probably malformed request
     */
    nc->flags = MG_F_CLOSE_IMMEDIATELY;
    DBG(("invalid request"));
    goto exit_mp;
  }

  /* If we reach this place - that is multipart request */

  if (pd->mp_stream.boundary != NULL) {
    /*
     * Another streaming request was in progress,
     * looks like protocol error
     */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else {
    struct mg_http_endpoint *ep = NULL;
    pd->mp_stream.state = MPS_BEGIN;
    pd->mp_stream.boundary = strdup(boundary);
    pd->mp_stream.boundary_len = strlen(boundary);
    pd->mp_stream.var_name = pd->mp_stream.file_name = NULL;
    pd->endpoint_handler = nc->handler;

    ep = mg_http_get_endpoint_handler(nc->listener, &hm->uri);
    if (ep != NULL) {
      pd->endpoint_handler = ep->handler;
    }

    mg_http_call_endpoint_handler(nc, MG_EV_HTTP_MULTIPART_REQUEST, hm);

    mbuf_remove(io, req_len);
  }
exit_mp:
  if (boundary != boundary_buf) MG_FREE(boundary);
}

#define CONTENT_DISPOSITION "Content-Disposition: "

static void mg_http_multipart_call_handler(struct mg_connection *c, int ev,
                                           const char *data, size_t data_len) {
  struct mg_http_multipart_part mp;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  memset(&mp, 0, sizeof(mp));

  mp.var_name = pd->mp_stream.var_name;
  mp.file_name = pd->mp_stream.file_name;
  mp.user_data = pd->mp_stream.user_data;
  mp.data.p = data;
  mp.data.len = data_len;
  mg_call(c, pd->endpoint_handler, c->user_data, ev, &mp);
  pd->mp_stream.user_data = mp.user_data;
}

static int mg_http_multipart_got_chunk(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  struct mbuf *io = &c->recv_mbuf;

  mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_DATA, io->buf,
                                 pd->mp_stream.prev_io_len);
  mbuf_remove(io, pd->mp_stream.prev_io_len);
  pd->mp_stream.prev_io_len = 0;
  pd->mp_stream.state = MPS_WAITING_FOR_CHUNK;

  return 0;
}

static int mg_http_multipart_finalize(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);

  mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_END, NULL, 0);
  MG_FREE((void *) pd->mp_stream.file_name);
  pd->mp_stream.file_name = NULL;
  MG_FREE((void *) pd->mp_stream.var_name);
  pd->mp_stream.var_name = NULL;
  mg_http_multipart_call_handler(c, MG_EV_HTTP_MULTIPART_REQUEST_END, NULL, 0);
  mg_http_free_proto_data_mp_stream(&pd->mp_stream);
  pd->mp_stream.state = MPS_FINISHED;

  return 1;
}

static int mg_http_multipart_wait_for_boundary(struct mg_connection *c) {
  const char *boundary;
  struct mbuf *io = &c->recv_mbuf;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);

  if (pd->mp_stream.boundary == NULL) {
    pd->mp_stream.state = MPS_FINALIZE;
    DBG(("Invalid request: boundary not initialized"));
    return 0;
  }

  if ((int) io->len < pd->mp_stream.boundary_len + 2) {
    return 0;
  }

  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  if (boundary != NULL) {
    const char *boundary_end = (boundary + pd->mp_stream.boundary_len);
    if (io->len - (boundary_end - io->buf) < 4) {
      return 0;
    }
    if (strncmp(boundary_end, "--\r\n", 4) == 0) {
      pd->mp_stream.state = MPS_FINALIZE;
      mbuf_remove(io, (boundary_end - io->buf) + 4);
    } else {
      pd->mp_stream.state = MPS_GOT_BOUNDARY;
    }
  } else {
    return 0;
  }

  return 1;
}

static void mg_http_parse_header_internal(struct mg_str *hdr,
                                          const char *var_name,
                                          struct altbuf *ab);

static int mg_http_multipart_process_boundary(struct mg_connection *c) {
  int data_size;
  const char *boundary, *block_begin;
  struct mbuf *io = &c->recv_mbuf;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  struct altbuf ab_file_name, ab_var_name;
  int line_len;
  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  block_begin = boundary + pd->mp_stream.boundary_len + 2;
  data_size = io->len - (block_begin - io->buf);

  altbuf_init(&ab_file_name, NULL, 0);
  altbuf_init(&ab_var_name, NULL, 0);

  while (data_size > 0 &&
         (line_len = mg_get_line_len(block_begin, data_size)) != 0) {
    if (line_len > (int) sizeof(CONTENT_DISPOSITION) &&
        mg_ncasecmp(block_begin, CONTENT_DISPOSITION,
                    sizeof(CONTENT_DISPOSITION) - 1) == 0) {
      struct mg_str header;

      header.p = block_begin + sizeof(CONTENT_DISPOSITION) - 1;
      header.len = line_len - sizeof(CONTENT_DISPOSITION) - 1;

      altbuf_reset(&ab_var_name);
      mg_http_parse_header_internal(&header, "name", &ab_var_name);

      altbuf_reset(&ab_file_name);
      mg_http_parse_header_internal(&header, "filename", &ab_file_name);

      block_begin += line_len;
      data_size -= line_len;

      continue;
    }

    if (line_len == 2 && mg_ncasecmp(block_begin, "\r\n", 2) == 0) {
      mbuf_remove(io, block_begin - io->buf + 2);

      if (pd->mp_stream.processing_part != 0) {
        mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_END, NULL, 0);
      }

      /* Reserve 2 bytes for "\r\n" in file_name and var_name */
      altbuf_append(&ab_file_name, '\0');
      altbuf_append(&ab_file_name, '\0');
      altbuf_append(&ab_var_name, '\0');
      altbuf_append(&ab_var_name, '\0');

      MG_FREE((void *) pd->mp_stream.file_name);
      pd->mp_stream.file_name = altbuf_get_buf(&ab_file_name, 1 /* trim */);
      MG_FREE((void *) pd->mp_stream.var_name);
      pd->mp_stream.var_name = altbuf_get_buf(&ab_var_name, 1 /* trim */);

      mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_BEGIN, NULL, 0);
      pd->mp_stream.state = MPS_WAITING_FOR_CHUNK;
      pd->mp_stream.processing_part++;
      return 1;
    }

    block_begin += line_len;
  }

  pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;

  altbuf_reset(&ab_var_name);
  altbuf_reset(&ab_file_name);

  return 0;
}

static int mg_http_multipart_continue_wait_for_chunk(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  struct mbuf *io = &c->recv_mbuf;

  const char *boundary;
  if ((int) io->len < pd->mp_stream.boundary_len + 6 /* \r\n, --, -- */) {
    return 0;
  }

  boundary = c_strnstr(io->buf, pd->mp_stream.boundary, io->len);
  if (boundary == NULL && pd->mp_stream.prev_io_len == 0) {
    pd->mp_stream.prev_io_len = io->len;
    return 0;
  } else if (boundary == NULL &&
             (int) io->len >
                 pd->mp_stream.prev_io_len + pd->mp_stream.boundary_len + 4) {
    pd->mp_stream.state = MPS_GOT_CHUNK;
    return 1;
  } else if (boundary != NULL) {
    int data_size = (boundary - io->buf - 4);
    mg_http_multipart_call_handler(c, MG_EV_HTTP_PART_DATA, io->buf, data_size);
    mbuf_remove(io, (boundary - io->buf));
    pd->mp_stream.prev_io_len = 0;
    pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;
    return 1;
  } else {
    return 0;
  }
}

static void mg_http_multipart_continue(struct mg_connection *c) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(c);
  while (1) {
    switch (pd->mp_stream.state) {
      case MPS_BEGIN: {
        pd->mp_stream.state = MPS_WAITING_FOR_BOUNDARY;
        break;
      }
      case MPS_WAITING_FOR_BOUNDARY: {
        if (mg_http_multipart_wait_for_boundary(c) == 0) {
          return;
        }
        break;
      }
      case MPS_GOT_BOUNDARY: {
        if (mg_http_multipart_process_boundary(c) == 0) {
          return;
        }
        break;
      }
      case MPS_WAITING_FOR_CHUNK: {
        if (mg_http_multipart_continue_wait_for_chunk(c) == 0) {
          return;
        }
        break;
      }
      case MPS_GOT_CHUNK: {
        if (mg_http_multipart_got_chunk(c) == 0) {
          return;
        }
        break;
      }
      case MPS_FINALIZE: {
        if (mg_http_multipart_finalize(c) == 0) {
          return;
        }
        break;
      }
      case MPS_FINISHED: {
        mbuf_remove(&c->recv_mbuf, c->recv_mbuf.len);
        return;
      }
    }
  }
}

struct file_upload_state {
  char *lfn;
  size_t num_recd;
  FILE *fp;
};

#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */

void mg_set_protocol_http_websocket(struct mg_connection *nc) {
  nc->proto_handler = mg_http_handler;
}

const char *mg_status_message(int status_code) {
  switch (status_code) {
    case 206:
      return "Partial Content";
    case 301:
      return "Moved";
    case 302:
      return "Found";
    case 400:
      return "Bad Request";
    case 401:
      return "Unauthorized";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 416:
      return "Requested Range Not Satisfiable";
    case 418:
      return "I'm a teapot";
    case 500:
      return "Internal Server Error";
    case 502:
      return "Bad Gateway";
    case 503:
      return "Service Unavailable";

#if MG_ENABLE_EXTRA_ERRORS_DESC
    case 100:
      return "Continue";
    case 101:
      return "Switching Protocols";
    case 102:
      return "Processing";
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 202:
      return "Accepted";
    case 203:
      return "Non-Authoritative Information";
    case 204:
      return "No Content";
    case 205:
      return "Reset Content";
    case 207:
      return "Multi-Status";
    case 208:
      return "Already Reported";
    case 226:
      return "IM Used";
    case 300:
      return "Multiple Choices";
    case 303:
      return "See Other";
    case 304:
      return "Not Modified";
    case 305:
      return "Use Proxy";
    case 306:
      return "Switch Proxy";
    case 307:
      return "Temporary Redirect";
    case 308:
      return "Permanent Redirect";
    case 402:
      return "Payment Required";
    case 405:
      return "Method Not Allowed";
    case 406:
      return "Not Acceptable";
    case 407:
      return "Proxy Authentication Required";
    case 408:
      return "Request Timeout";
    case 409:
      return "Conflict";
    case 410:
      return "Gone";
    case 411:
      return "Length Required";
    case 412:
      return "Precondition Failed";
    case 413:
      return "Payload Too Large";
    case 414:
      return "URI Too Long";
    case 415:
      return "Unsupported Media Type";
    case 417:
      return "Expectation Failed";
    case 422:
      return "Unprocessable Entity";
    case 423:
      return "Locked";
    case 424:
      return "Failed Dependency";
    case 426:
      return "Upgrade Required";
    case 428:
      return "Precondition Required";
    case 429:
      return "Too Many Requests";
    case 431:
      return "Request Header Fields Too Large";
    case 451:
      return "Unavailable For Legal Reasons";
    case 501:
      return "Not Implemented";
    case 504:
      return "Gateway Timeout";
    case 505:
      return "HTTP Version Not Supported";
    case 506:
      return "Variant Also Negotiates";
    case 507:
      return "Insufficient Storage";
    case 508:
      return "Loop Detected";
    case 510:
      return "Not Extended";
    case 511:
      return "Network Authentication Required";
#endif /* MG_ENABLE_EXTRA_ERRORS_DESC */

    default:
      return "OK";
  }
}

void mg_send_response_line_s(struct mg_connection *nc, int status_code,
                             const struct mg_str extra_headers) {
  mg_printf(nc, "HTTP/1.1 %d %s\r\nServer: %s\r\n", status_code,
            mg_status_message(status_code), mg_version_header);
  if (extra_headers.len > 0) {
    mg_printf(nc, "%.*s\r\n", (int) extra_headers.len, extra_headers.p);
  }
}

void mg_send_response_line(struct mg_connection *nc, int status_code,
                           const char *extra_headers) {
  mg_send_response_line_s(nc, status_code, mg_mk_str(extra_headers));
}

void mg_http_send_redirect(struct mg_connection *nc, int status_code,
                           const struct mg_str location,
                           const struct mg_str extra_headers) {
  char bbody[100], *pbody = bbody;
  int bl = mg_asprintf(&pbody, sizeof(bbody),
                       "<p>Moved <a href='%.*s'>here</a>.\r\n",
                       (int) location.len, location.p);
  char bhead[150], *phead = bhead;
  mg_asprintf(&phead, sizeof(bhead),
              "Location: %.*s\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: %d\r\n"
              "Cache-Control: no-cache\r\n"
              "%.*s%s",
              (int) location.len, location.p, bl, (int) extra_headers.len,
              extra_headers.p, (extra_headers.len > 0 ? "\r\n" : ""));
  mg_send_response_line(nc, status_code, phead);
  if (phead != bhead) MG_FREE(phead);
  mg_send(nc, pbody, bl);
  if (pbody != bbody) MG_FREE(pbody);
}

void mg_send_head(struct mg_connection *c, int status_code,
                  int64_t content_length, const char *extra_headers) {
  mg_send_response_line(c, status_code, extra_headers);
  if (content_length < 0) {
    mg_printf(c, "%s", "Transfer-Encoding: chunked\r\n");
  } else {
    mg_printf(c, "Content-Length: %" INT64_FMT "\r\n", content_length);
  }
  mg_send(c, "\r\n", 2);
}

void mg_http_send_error(struct mg_connection *nc, int code,
                        const char *reason) {
  if (!reason) reason = mg_status_message(code);
  LOG(LL_DEBUG, ("%p %d %s", nc, code, reason));
  mg_send_head(nc, code, strlen(reason),
               "Content-Type: text/plain\r\nConnection: close");
  mg_send(nc, reason, strlen(reason));
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

#if MG_ENABLE_FILESYSTEM
static void mg_http_construct_etag(char *buf, size_t buf_len,
                                   const cs_stat_t *st) {
  snprintf(buf, buf_len, "\"%lx.%" INT64_FMT "\"", (unsigned long) st->st_mtime,
           (int64_t) st->st_size);
}

#ifndef WINCE
static void mg_gmt_time_string(char *buf, size_t buf_len, time_t *t) {
  strftime(buf, buf_len, "%a, %d %b %Y %H:%M:%S GMT", gmtime(t));
}
#else
/* Look wince_lib.c for WindowsCE implementation */
static void mg_gmt_time_string(char *buf, size_t buf_len, time_t *t);
#endif

static int mg_http_parse_range_header(const struct mg_str *header, int64_t *a,
                                      int64_t *b) {
  /*
   * There is no snscanf. Headers are not guaranteed to be NUL-terminated,
   * so we have this. Ugh.
   */
  int result;
  char *p = (char *) MG_MALLOC(header->len + 1);
  if (p == NULL) return 0;
  memcpy(p, header->p, header->len);
  p[header->len] = '\0';
  result = sscanf(p, "bytes=%" INT64_FMT "-%" INT64_FMT, a, b);
  MG_FREE(p);
  return result;
}

void mg_http_serve_file(struct mg_connection *nc, struct http_message *hm,
                        const char *path, const struct mg_str mime_type,
                        const struct mg_str extra_headers) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  cs_stat_t st;
  LOG(LL_DEBUG, ("%p [%s] %.*s", nc, path, (int) mime_type.len, mime_type.p));
  if (mg_stat(path, &st) != 0 || (pd->file.fp = mg_fopen(path, "rb")) == NULL) {
    int code, err = mg_get_errno();
    switch (err) {
      case EACCES:
        code = 403;
        break;
      case ENOENT:
        code = 404;
        break;
      default:
        code = 500;
    };
    mg_http_send_error(nc, code, "Open failed");
  } else {
    char etag[50], current_time[50], last_modified[50], range[70];
    time_t t = (time_t) mg_time();
    int64_t r1 = 0, r2 = 0, cl = st.st_size;
    struct mg_str *range_hdr = mg_get_http_header(hm, "Range");
    int n, status_code = 200;

    /* Handle Range header */
    range[0] = '\0';
    if (range_hdr != NULL &&
        (n = mg_http_parse_range_header(range_hdr, &r1, &r2)) > 0 && r1 >= 0 &&
        r2 >= 0) {
      /* If range is specified like "400-", set second limit to content len */
      if (n == 1) {
        r2 = cl - 1;
      }
      if (r1 > r2 || r2 >= cl) {
        status_code = 416;
        cl = 0;
        snprintf(range, sizeof(range),
                 "Content-Range: bytes */%" INT64_FMT "\r\n",
                 (int64_t) st.st_size);
      } else {
        status_code = 206;
        cl = r2 - r1 + 1;
        snprintf(range, sizeof(range), "Content-Range: bytes %" INT64_FMT
                                       "-%" INT64_FMT "/%" INT64_FMT "\r\n",
                 r1, r1 + cl - 1, (int64_t) st.st_size);
#if _FILE_OFFSET_BITS == 64 || _POSIX_C_SOURCE >= 200112L || \
    _XOPEN_SOURCE >= 600
        fseeko(pd->file.fp, r1, SEEK_SET);
#else
        fseek(pd->file.fp, (long) r1, SEEK_SET);
#endif
      }
    }

#if !MG_DISABLE_HTTP_KEEP_ALIVE
    {
      struct mg_str *conn_hdr = mg_get_http_header(hm, "Connection");
      if (conn_hdr != NULL) {
        pd->file.keepalive = (mg_vcasecmp(conn_hdr, "keep-alive") == 0);
      } else {
        pd->file.keepalive = (mg_vcmp(&hm->proto, "HTTP/1.1") == 0);
      }
    }
#endif

    mg_http_construct_etag(etag, sizeof(etag), &st);
    mg_gmt_time_string(current_time, sizeof(current_time), &t);
    mg_gmt_time_string(last_modified, sizeof(last_modified), &st.st_mtime);
    /*
     * Content length casted to size_t because:
     * 1) that's the maximum buffer size anyway
     * 2) ESP8266 RTOS SDK newlib vprintf cannot contain a 64bit arg at non-last
     *    position
     * TODO(mkm): fix ESP8266 RTOS SDK
     */
    mg_send_response_line_s(nc, status_code, extra_headers);
    mg_printf(nc,
              "Date: %s\r\n"
              "Last-Modified: %s\r\n"
              "Accept-Ranges: bytes\r\n"
              "Content-Type: %.*s\r\n"
              "Connection: %s\r\n"
              "Content-Length: %" SIZE_T_FMT
              "\r\n"
              "%sEtag: %s\r\n\r\n",
              current_time, last_modified, (int) mime_type.len, mime_type.p,
              (pd->file.keepalive ? "keep-alive" : "close"), (size_t) cl, range,
              etag);

    pd->file.cl = cl;
    pd->file.type = DATA_FILE;
    mg_http_transfer_file_data(nc);
  }
}

static void mg_http_serve_file2(struct mg_connection *nc, const char *path,
                                struct http_message *hm,
                                struct mg_serve_http_opts *opts) {
#if MG_ENABLE_HTTP_SSI
  if (mg_match_prefix(opts->ssi_pattern, strlen(opts->ssi_pattern), path) > 0) {
    mg_handle_ssi_request(nc, hm, path, opts);
    return;
  }
#endif
  mg_http_serve_file(nc, hm, path, mg_get_mime_type(path, "text/plain", opts),
                     mg_mk_str(opts->extra_headers));
}

#endif

int mg_url_decode(const char *src, int src_len, char *dst, int dst_len,
                  int is_form_url_encoded) {
  int i, j, a, b;
#define HEXTOI(x) (isdigit(x) ? x - '0' : x - 'W')

  for (i = j = 0; i < src_len && j < dst_len - 1; i++, j++) {
    if (src[i] == '%') {
      if (i < src_len - 2 && isxdigit(*(const unsigned char *) (src + i + 1)) &&
          isxdigit(*(const unsigned char *) (src + i + 2))) {
        a = tolower(*(const unsigned char *) (src + i + 1));
        b = tolower(*(const unsigned char *) (src + i + 2));
        dst[j] = (char) ((HEXTOI(a) << 4) | HEXTOI(b));
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

  dst[j] = '\0'; /* Null-terminate the destination */

  return i >= src_len ? j : -1;
}

int mg_get_http_var(const struct mg_str *buf, const char *name, char *dst,
                    size_t dst_len) {
  const char *p, *e, *s;
  size_t name_len;
  int len;

  /*
   * According to the documentation function returns negative
   * value in case of error. For debug purposes it returns:
   * -1 - src is wrong (NUUL)
   * -2 - dst is wrong (NULL)
   * -3 - failed to decode url or dst is to small
   * -4 - name does not exist
   */
  if (dst == NULL || dst_len == 0) {
    len = -2;
  } else if (buf->p == NULL || name == NULL || buf->len == 0) {
    len = -1;
    dst[0] = '\0';
  } else {
    name_len = strlen(name);
    e = buf->p + buf->len;
    len = -4;
    dst[0] = '\0';

    for (p = buf->p; p + name_len < e; p++) {
      if ((p == buf->p || p[-1] == '&') && p[name_len] == '=' &&
          !mg_ncasecmp(name, p, name_len)) {
        p += name_len + 1;
        s = (const char *) memchr(p, '&', (size_t)(e - p));
        if (s == NULL) {
          s = e;
        }
        len = mg_url_decode(p, (size_t)(s - p), dst, dst_len, 1);
        /* -1 means: failed to decode or dst is too small */
        if (len == -1) {
          len = -3;
        }
        break;
      }
    }
  }

  return len;
}

void mg_send_http_chunk(struct mg_connection *nc, const char *buf, size_t len) {
  char chunk_size[50];
  int n;

  n = snprintf(chunk_size, sizeof(chunk_size), "%lX\r\n", (unsigned long) len);
  mg_send(nc, chunk_size, n);
  mg_send(nc, buf, len);
  mg_send(nc, "\r\n", 2);
}

void mg_printf_http_chunk(struct mg_connection *nc, const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int len;
  va_list ap;

  va_start(ap, fmt);
  len = mg_avprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);

  if (len >= 0) {
    mg_send_http_chunk(nc, buf, len);
  }

  /* LCOV_EXCL_START */
  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
  /* LCOV_EXCL_STOP */
}

void mg_printf_html_escape(struct mg_connection *nc, const char *fmt, ...) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int i, j, len;
  va_list ap;

  va_start(ap, fmt);
  len = mg_avprintf(&buf, sizeof(mem), fmt, ap);
  va_end(ap);

  if (len >= 0) {
    for (i = j = 0; i < len; i++) {
      if (buf[i] == '<' || buf[i] == '>') {
        mg_send(nc, buf + j, i - j);
        mg_send(nc, buf[i] == '<' ? "&lt;" : "&gt;", 4);
        j = i + 1;
      }
    }
    mg_send(nc, buf + j, i - j);
  }

  /* LCOV_EXCL_START */
  if (buf != mem && buf != NULL) {
    MG_FREE(buf);
  }
  /* LCOV_EXCL_STOP */
}

static void mg_http_parse_header_internal(struct mg_str *hdr,
                                          const char *var_name,
                                          struct altbuf *ab) {
  int ch = ' ', ch1 = ',', n = strlen(var_name);
  const char *p, *end = hdr ? hdr->p + hdr->len : NULL, *s = NULL;

  /* Find where variable starts */
  for (s = hdr->p; s != NULL && s + n < end; s++) {
    if ((s == hdr->p || s[-1] == ch || s[-1] == ch1 || s[-1] == ';') &&
        s[n] == '=' && !strncmp(s, var_name, n))
      break;
  }

  if (s != NULL && &s[n + 1] < end) {
    s += n + 1;
    if (*s == '"' || *s == '\'') {
      ch = ch1 = *s++;
    }
    p = s;
    while (p < end && p[0] != ch && p[0] != ch1) {
      if (ch != ' ' && p[0] == '\\' && p[1] == ch) p++;
      altbuf_append(ab, *p++);
    }

    if (ch != ' ' && *p != ch) {
      altbuf_reset(ab);
    }
  }

  /* If there is some data, append a NUL. */
  if (ab->len > 0) {
    altbuf_append(ab, '\0');
  }
}

int mg_http_parse_header2(struct mg_str *hdr, const char *var_name, char **buf,
                          size_t buf_size) {
  struct altbuf ab;
  altbuf_init(&ab, *buf, buf_size);
  if (hdr == NULL) return 0;
  if (*buf != NULL && buf_size > 0) *buf[0] = '\0';

  mg_http_parse_header_internal(hdr, var_name, &ab);

  /*
   * Get a (trimmed) buffer, and return a len without a NUL byte which might
   * have been added.
   */
  *buf = altbuf_get_buf(&ab, 1 /* trim */);
  return ab.len > 0 ? ab.len - 1 : 0;
}

int mg_http_parse_header(struct mg_str *hdr, const char *var_name, char *buf,
                         size_t buf_size) {
  char *buf2 = buf;

  int len = mg_http_parse_header2(hdr, var_name, &buf2, buf_size);

  if (buf2 != buf) {
    /* Buffer was not enough and was reallocated: free it and just return 0 */
    MG_FREE(buf2);
    return 0;
  }

  return len;
}

int mg_get_http_basic_auth(struct http_message *hm, char *user, size_t user_len,
                           char *pass, size_t pass_len) {
  struct mg_str *hdr = mg_get_http_header(hm, "Authorization");
  if (hdr == NULL) return -1;
  return mg_parse_http_basic_auth(hdr, user, user_len, pass, pass_len);
}

int mg_parse_http_basic_auth(struct mg_str *hdr, char *user, size_t user_len,
                             char *pass, size_t pass_len) {
  char *buf = NULL;
  char fmt[64];
  int res = 0;

  if (mg_strncmp(*hdr, mg_mk_str("Basic "), 6) != 0) return -1;

  buf = (char *) MG_MALLOC(hdr->len);
  cs_base64_decode((unsigned char *) hdr->p + 6, hdr->len, buf, NULL);

  /* e.g. "%123[^:]:%321[^\n]" */
  snprintf(fmt, sizeof(fmt), "%%%" SIZE_T_FMT "[^:]:%%%" SIZE_T_FMT "[^\n]",
           user_len - 1, pass_len - 1);
  if (sscanf(buf, fmt, user, pass) == 0) {
    res = -1;
  }

  MG_FREE(buf);
  return res;
}

#if MG_ENABLE_FILESYSTEM
static int mg_is_file_hidden(const char *path,
                             const struct mg_serve_http_opts *opts,
                             int exclude_specials) {
  const char *p1 = opts->per_directory_auth_file;
  const char *p2 = opts->hidden_file_pattern;

  /* Strip directory path from the file name */
  const char *pdir = strrchr(path, DIRSEP);
  if (pdir != NULL) {
    path = pdir + 1;
  }

  return (exclude_specials && (!strcmp(path, ".") || !strcmp(path, ".."))) ||
         (p1 != NULL && mg_match_prefix(p1, strlen(p1), path) == strlen(p1)) ||
         (p2 != NULL && mg_match_prefix(p2, strlen(p2), path) > 0);
}

#if !MG_DISABLE_HTTP_DIGEST_AUTH

#ifndef MG_EXT_MD5
void mg_hash_md5_v(size_t num_msgs, const uint8_t *msgs[],
                   const size_t *msg_lens, uint8_t *digest) {
  size_t i;
  cs_md5_ctx md5_ctx;
  cs_md5_init(&md5_ctx);
  for (i = 0; i < num_msgs; i++) {
    cs_md5_update(&md5_ctx, msgs[i], msg_lens[i]);
  }
  cs_md5_final(digest, &md5_ctx);
}
#else
extern void mg_hash_md5_v(size_t num_msgs, const uint8_t *msgs[],
                          const size_t *msg_lens, uint8_t *digest);
#endif

void cs_md5(char buf[33], ...) {
  unsigned char hash[16];
  const uint8_t *msgs[20], *p;
  size_t msg_lens[20];
  size_t num_msgs = 0;
  va_list ap;

  va_start(ap, buf);
  while ((p = va_arg(ap, const unsigned char *) ) != NULL) {
    msgs[num_msgs] = p;
    msg_lens[num_msgs] = va_arg(ap, size_t);
    num_msgs++;
  }
  va_end(ap);

  mg_hash_md5_v(num_msgs, msgs, msg_lens, hash);
  cs_to_hex(buf, hash, sizeof(hash));
}

static void mg_mkmd5resp(const char *method, size_t method_len, const char *uri,
                         size_t uri_len, const char *ha1, size_t ha1_len,
                         const char *nonce, size_t nonce_len, const char *nc,
                         size_t nc_len, const char *cnonce, size_t cnonce_len,
                         const char *qop, size_t qop_len, char *resp) {
  static const char colon[] = ":";
  static const size_t one = 1;
  char ha2[33];
  cs_md5(ha2, method, method_len, colon, one, uri, uri_len, NULL);
  cs_md5(resp, ha1, ha1_len, colon, one, nonce, nonce_len, colon, one, nc,
         nc_len, colon, one, cnonce, cnonce_len, colon, one, qop, qop_len,
         colon, one, ha2, sizeof(ha2) - 1, NULL);
}

int mg_http_create_digest_auth_header(char *buf, size_t buf_len,
                                      const char *method, const char *uri,
                                      const char *auth_domain, const char *user,
                                      const char *passwd, const char *nonce) {
  static const char colon[] = ":", qop[] = "auth";
  static const size_t one = 1;
  char ha1[33], resp[33], cnonce[40];

  snprintf(cnonce, sizeof(cnonce), "%lx", (unsigned long) mg_time());
  cs_md5(ha1, user, (size_t) strlen(user), colon, one, auth_domain,
         (size_t) strlen(auth_domain), colon, one, passwd,
         (size_t) strlen(passwd), NULL);
  mg_mkmd5resp(method, strlen(method), uri, strlen(uri), ha1, sizeof(ha1) - 1,
               nonce, strlen(nonce), "1", one, cnonce, strlen(cnonce), qop,
               sizeof(qop) - 1, resp);
  return snprintf(buf, buf_len,
                  "Authorization: Digest username=\"%s\","
                  "realm=\"%s\",uri=\"%s\",qop=%s,nc=1,cnonce=%s,"
                  "nonce=%s,response=%s\r\n",
                  user, auth_domain, uri, qop, cnonce, nonce, resp);
}

/*
 * Check for authentication timeout.
 * Clients send time stamp encoded in nonce. Make sure it is not too old,
 * to prevent replay attacks.
 * Assumption: nonce is a hexadecimal number of seconds since 1970.
 */
static int mg_check_nonce(const char *nonce) {
  unsigned long now = (unsigned long) mg_time();
  unsigned long val = (unsigned long) strtoul(nonce, NULL, 16);
  return (now >= val) && (now - val < 60 * 60);
}

int mg_http_check_digest_auth(struct http_message *hm, const char *auth_domain,
                              FILE *fp) {
  int ret = 0;
  struct mg_str *hdr;
  char username_buf[50], cnonce_buf[64], response_buf[40], uri_buf[200],
      qop_buf[20], nc_buf[20], nonce_buf[16];

  char *username = username_buf, *cnonce = cnonce_buf, *response = response_buf,
       *uri = uri_buf, *qop = qop_buf, *nc = nc_buf, *nonce = nonce_buf;

  /* Parse "Authorization:" header, fail fast on parse error */
  if (hm == NULL || fp == NULL ||
      (hdr = mg_get_http_header(hm, "Authorization")) == NULL ||
      mg_http_parse_header2(hdr, "username", &username, sizeof(username_buf)) ==
          0 ||
      mg_http_parse_header2(hdr, "cnonce", &cnonce, sizeof(cnonce_buf)) == 0 ||
      mg_http_parse_header2(hdr, "response", &response, sizeof(response_buf)) ==
          0 ||
      mg_http_parse_header2(hdr, "uri", &uri, sizeof(uri_buf)) == 0 ||
      mg_http_parse_header2(hdr, "qop", &qop, sizeof(qop_buf)) == 0 ||
      mg_http_parse_header2(hdr, "nc", &nc, sizeof(nc_buf)) == 0 ||
      mg_http_parse_header2(hdr, "nonce", &nonce, sizeof(nonce_buf)) == 0 ||
      mg_check_nonce(nonce) == 0) {
    ret = 0;
    goto clean;
  }

  /* NOTE(lsm): due to a bug in MSIE, we do not compare URIs */

  ret = mg_check_digest_auth(
      hm->method,
      mg_mk_str_n(
          hm->uri.p,
          hm->uri.len + (hm->query_string.len ? hm->query_string.len + 1 : 0)),
      mg_mk_str(username), mg_mk_str(cnonce), mg_mk_str(response),
      mg_mk_str(qop), mg_mk_str(nc), mg_mk_str(nonce), mg_mk_str(auth_domain),
      fp);

clean:
  if (username != username_buf) MG_FREE(username);
  if (cnonce != cnonce_buf) MG_FREE(cnonce);
  if (response != response_buf) MG_FREE(response);
  if (uri != uri_buf) MG_FREE(uri);
  if (qop != qop_buf) MG_FREE(qop);
  if (nc != nc_buf) MG_FREE(nc);
  if (nonce != nonce_buf) MG_FREE(nonce);

  return ret;
}

int mg_check_digest_auth(struct mg_str method, struct mg_str uri,
                         struct mg_str username, struct mg_str cnonce,
                         struct mg_str response, struct mg_str qop,
                         struct mg_str nc, struct mg_str nonce,
                         struct mg_str auth_domain, FILE *fp) {
  char buf[128], f_user[sizeof(buf)], f_ha1[sizeof(buf)], f_domain[sizeof(buf)];
  char expected_response[33];

  /*
   * Read passwords file line by line. If should have htdigest format,
   * i.e. each line should be a colon-separated sequence:
   * USER_NAME:DOMAIN_NAME:HA1_HASH_OF_USER_DOMAIN_AND_PASSWORD
   */
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    if (sscanf(buf, "%[^:]:%[^:]:%s", f_user, f_domain, f_ha1) == 3 &&
        mg_vcmp(&username, f_user) == 0 &&
        mg_vcmp(&auth_domain, f_domain) == 0) {
      /* Username and domain matched, check the password */
      mg_mkmd5resp(method.p, method.len, uri.p, uri.len, f_ha1, strlen(f_ha1),
                   nonce.p, nonce.len, nc.p, nc.len, cnonce.p, cnonce.len,
                   qop.p, qop.len, expected_response);
      LOG(LL_DEBUG,
          ("%.*s %s %.*s %s", (int) username.len, username.p, f_domain,
           (int) response.len, response.p, expected_response));
      return mg_ncasecmp(response.p, expected_response, response.len) == 0;
    }
  }

  /* None of the entries in the passwords file matched - return failure */
  return 0;
}

int mg_http_is_authorized(struct http_message *hm, struct mg_str path,
                          const char *domain, const char *passwords_file,
                          int flags) {
  char buf[MG_MAX_PATH];
  const char *p;
  FILE *fp;
  int authorized = 1;

  if (domain != NULL && passwords_file != NULL) {
    if (flags & MG_AUTH_FLAG_IS_GLOBAL_PASS_FILE) {
      fp = mg_fopen(passwords_file, "r");
    } else if (flags & MG_AUTH_FLAG_IS_DIRECTORY) {
      snprintf(buf, sizeof(buf), "%.*s%c%s", (int) path.len, path.p, DIRSEP,
               passwords_file);
      fp = mg_fopen(buf, "r");
    } else {
      p = strrchr(path.p, DIRSEP);
      if (p == NULL) p = path.p;
      snprintf(buf, sizeof(buf), "%.*s%c%s", (int) (p - path.p), path.p, DIRSEP,
               passwords_file);
      fp = mg_fopen(buf, "r");
    }

    if (fp != NULL) {
      authorized = mg_http_check_digest_auth(hm, domain, fp);
      fclose(fp);
    } else if (!(flags & MG_AUTH_FLAG_ALLOW_MISSING_FILE)) {
      authorized = 0;
    }
  }

  LOG(LL_DEBUG, ("%.*s %s %x %d", (int) path.len, path.p,
                 passwords_file ? passwords_file : "", flags, authorized));
  return authorized;
}
#else
int mg_http_is_authorized(struct http_message *hm, const struct mg_str path,
                          const char *domain, const char *passwords_file,
                          int flags) {
  (void) hm;
  (void) path;
  (void) domain;
  (void) passwords_file;
  (void) flags;
  return 1;
}
#endif

#if MG_ENABLE_DIRECTORY_LISTING
static void mg_escape(const char *src, char *dst, size_t dst_len) {
  size_t n = 0;
  while (*src != '\0' && n + 5 < dst_len) {
    unsigned char ch = *(unsigned char *) src++;
    if (ch == '<') {
      n += snprintf(dst + n, dst_len - n, "%s", "&lt;");
    } else {
      dst[n++] = ch;
    }
  }
  dst[n] = '\0';
}

static void mg_print_dir_entry(struct mg_connection *nc, const char *file_name,
                               cs_stat_t *stp) {
  char size[64], mod[64], path[MG_MAX_PATH];
  int64_t fsize = stp->st_size;
  int is_dir = S_ISDIR(stp->st_mode);
  const char *slash = is_dir ? "/" : "";
  struct mg_str href;

  if (is_dir) {
    snprintf(size, sizeof(size), "%s", "[DIRECTORY]");
  } else {
    /*
     * We use (double) cast below because MSVC 6 compiler cannot
     * convert unsigned __int64 to double.
     */
    if (fsize < 1024) {
      snprintf(size, sizeof(size), "%d", (int) fsize);
    } else if (fsize < 0x100000) {
      snprintf(size, sizeof(size), "%.1fk", (double) fsize / 1024.0);
    } else if (fsize < 0x40000000) {
      snprintf(size, sizeof(size), "%.1fM", (double) fsize / 1048576);
    } else {
      snprintf(size, sizeof(size), "%.1fG", (double) fsize / 1073741824);
    }
  }
  strftime(mod, sizeof(mod), "%d-%b-%Y %H:%M", localtime(&stp->st_mtime));
  mg_escape(file_name, path, sizeof(path));
  href = mg_url_encode(mg_mk_str(file_name));
  mg_printf_http_chunk(nc,
                       "<tr><td><a href=\"%s%s\">%s%s</a></td>"
                       "<td>%s</td><td name=%" INT64_FMT ">%s</td></tr>\n",
                       href.p, slash, path, slash, mod, is_dir ? -1 : fsize,
                       size);
  free((void *) href.p);
}

static void mg_scan_directory(struct mg_connection *nc, const char *dir,
                              const struct mg_serve_http_opts *opts,
                              void (*func)(struct mg_connection *, const char *,
                                           cs_stat_t *)) {
  char path[MG_MAX_PATH];
  cs_stat_t st;
  struct dirent *dp;
  DIR *dirp;

  LOG(LL_DEBUG, ("%p [%s]", nc, dir));
  if ((dirp = (opendir(dir))) != NULL) {
    while ((dp = readdir(dirp)) != NULL) {
      /* Do not show current dir and hidden files */
      if (mg_is_file_hidden((const char *) dp->d_name, opts, 1)) {
        continue;
      }
      snprintf(path, sizeof(path), "%s/%s", dir, dp->d_name);
      if (mg_stat(path, &st) == 0) {
        func(nc, (const char *) dp->d_name, &st);
      }
    }
    closedir(dirp);
  } else {
    LOG(LL_DEBUG, ("%p opendir(%s) -> %d", nc, dir, mg_get_errno()));
  }
}

static void mg_send_directory_listing(struct mg_connection *nc, const char *dir,
                                      struct http_message *hm,
                                      struct mg_serve_http_opts *opts) {
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

  mg_send_response_line(nc, 200, opts->extra_headers);
  mg_printf(nc, "%s: %s\r\n%s: %s\r\n\r\n", "Transfer-Encoding", "chunked",
            "Content-Type", "text/html; charset=utf-8");

  mg_printf_http_chunk(
      nc,
      "<html><head><title>Index of %.*s</title>%s%s"
      "<style>th,td {text-align: left; padding-right: 1em; "
      "font-family: monospace; }</style></head>\n"
      "<body><h1>Index of %.*s</h1>\n<table cellpadding=0><thead>"
      "<tr><th><a href=# rel=0>Name</a></th><th>"
      "<a href=# rel=1>Modified</a</th>"
      "<th><a href=# rel=2>Size</a></th></tr>"
      "<tr><td colspan=3><hr></td></tr>\n"
      "</thead>\n"
      "<tbody id=tb>",
      (int) hm->uri.len, hm->uri.p, sort_js_code, sort_js_code2,
      (int) hm->uri.len, hm->uri.p);
  mg_scan_directory(nc, dir, opts, mg_print_dir_entry);
  mg_printf_http_chunk(nc,
                       "</tbody><tr><td colspan=3><hr></td></tr>\n"
                       "</table>\n"
                       "<address>%s</address>\n"
                       "</body></html>",
                       mg_version_header);
  mg_send_http_chunk(nc, "", 0);
  /* TODO(rojer): Remove when cesanta/dev/issues/197 is fixed. */
  nc->flags |= MG_F_SEND_AND_CLOSE;
}
#endif /* MG_ENABLE_DIRECTORY_LISTING */

/*
 * Given a directory path, find one of the files specified in the
 * comma-separated list of index files `list`.
 * First found index file wins. If an index file is found, then gets
 * appended to the `path`, stat-ed, and result of `stat()` passed to `stp`.
 * If index file is not found, then `path` and `stp` remain unchanged.
 */
MG_INTERNAL void mg_find_index_file(const char *path, const char *list,
                                    char **index_file, cs_stat_t *stp) {
  struct mg_str vec;
  size_t path_len = strlen(path);
  int found = 0;
  *index_file = NULL;

  /* Traverse index files list. For each entry, append it to the given */
  /* path and see if the file exists. If it exists, break the loop */
  while ((list = mg_next_comma_list_entry(list, &vec, NULL)) != NULL) {
    cs_stat_t st;
    size_t len = path_len + 1 + vec.len + 1;
    *index_file = (char *) MG_REALLOC(*index_file, len);
    if (*index_file == NULL) break;
    snprintf(*index_file, len, "%s%c%.*s", path, DIRSEP, (int) vec.len, vec.p);

    /* Does it exist? Is it a file? */
    if (mg_stat(*index_file, &st) == 0 && S_ISREG(st.st_mode)) {
      /* Yes it does, break the loop */
      *stp = st;
      found = 1;
      break;
    }
  }
  if (!found) {
    MG_FREE(*index_file);
    *index_file = NULL;
  }
  LOG(LL_DEBUG, ("[%s] [%s]", path, (*index_file ? *index_file : "")));
}

#if MG_ENABLE_HTTP_URL_REWRITES
static int mg_http_send_port_based_redirect(
    struct mg_connection *c, struct http_message *hm,
    const struct mg_serve_http_opts *opts) {
  const char *rewrites = opts->url_rewrites;
  struct mg_str a, b;
  char local_port[20] = {'%'};

  mg_conn_addr_to_str(c, local_port + 1, sizeof(local_port) - 1,
                      MG_SOCK_STRINGIFY_PORT);

  while ((rewrites = mg_next_comma_list_entry(rewrites, &a, &b)) != NULL) {
    if (mg_vcmp(&a, local_port) == 0) {
      mg_send_response_line(c, 301, NULL);
      mg_printf(c, "Content-Length: 0\r\nLocation: %.*s%.*s\r\n\r\n",
                (int) b.len, b.p, (int) (hm->proto.p - hm->uri.p - 1),
                hm->uri.p);
      return 1;
    }
  }

  return 0;
}

static void mg_reverse_proxy_handler(struct mg_connection *nc, int ev,
                                     void *ev_data MG_UD_ARG(void *user_data)) {
  struct http_message *hm = (struct http_message *) ev_data;
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);

  if (pd == NULL || pd->reverse_proxy_data.linked_conn == NULL) {
    DBG(("%p: upstream closed", nc));
    return;
  }

  switch (ev) {
    case MG_EV_CONNECT:
      if (*(int *) ev_data != 0) {
        mg_http_send_error(pd->reverse_proxy_data.linked_conn, 502, NULL);
      }
      break;
    /* TODO(mkm): handle streaming */
    case MG_EV_HTTP_REPLY:
      mg_send(pd->reverse_proxy_data.linked_conn, hm->message.p,
              hm->message.len);
      pd->reverse_proxy_data.linked_conn->flags |= MG_F_SEND_AND_CLOSE;
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      break;
    case MG_EV_CLOSE:
      pd->reverse_proxy_data.linked_conn->flags |= MG_F_SEND_AND_CLOSE;
      break;
  }

#if MG_ENABLE_CALLBACK_USERDATA
  (void) user_data;
#endif
}

void mg_http_reverse_proxy(struct mg_connection *nc,
                           const struct http_message *hm, struct mg_str mount,
                           struct mg_str upstream) {
  struct mg_connection *be;
  char burl[256], *purl = burl;
  int i;
  const char *error;
  struct mg_connect_opts opts;
  struct mg_str path = MG_NULL_STR, user_info = MG_NULL_STR, host = MG_NULL_STR;
  memset(&opts, 0, sizeof(opts));
  opts.error_string = &error;

  mg_asprintf(&purl, sizeof(burl), "%.*s%.*s", (int) upstream.len, upstream.p,
              (int) (hm->uri.len - mount.len), hm->uri.p + mount.len);

  be = mg_connect_http_base(nc->mgr, MG_CB(mg_reverse_proxy_handler, NULL),
                            opts, "http", NULL, "https", NULL, purl, &path,
                            &user_info, &host);
  LOG(LL_DEBUG, ("Proxying %.*s to %s (rule: %.*s)", (int) hm->uri.len,
                 hm->uri.p, purl, (int) mount.len, mount.p));

  if (be == NULL) {
    LOG(LL_ERROR, ("Error connecting to %s: %s", purl, error));
    mg_http_send_error(nc, 502, NULL);
    goto cleanup;
  }

  /* link connections to each other, they must live and die together */
  mg_http_get_proto_data(be)->reverse_proxy_data.linked_conn = nc;
  mg_http_get_proto_data(nc)->reverse_proxy_data.linked_conn = be;

  /* send request upstream */
  mg_printf(be, "%.*s %.*s HTTP/1.1\r\n", (int) hm->method.len, hm->method.p,
            (int) path.len, path.p);

  mg_printf(be, "Host: %.*s\r\n", (int) host.len, host.p);
  for (i = 0; i < MG_MAX_HTTP_HEADERS && hm->header_names[i].len > 0; i++) {
    struct mg_str hn = hm->header_names[i];
    struct mg_str hv = hm->header_values[i];

    /* we rewrite the host header */
    if (mg_vcasecmp(&hn, "Host") == 0) continue;
    /*
     * Don't pass chunked transfer encoding to the client because hm->body is
     * already dechunked when we arrive here.
     */
    if (mg_vcasecmp(&hn, "Transfer-encoding") == 0 &&
        mg_vcasecmp(&hv, "chunked") == 0) {
      mg_printf(be, "Content-Length: %" SIZE_T_FMT "\r\n", hm->body.len);
      continue;
    }
    /* We don't support proxying Expect: 100-continue. */
    if (mg_vcasecmp(&hn, "Expect") == 0 &&
        mg_vcasecmp(&hv, "100-continue") == 0) {
      continue;
    }

    mg_printf(be, "%.*s: %.*s\r\n", (int) hn.len, hn.p, (int) hv.len, hv.p);
  }

  mg_send(be, "\r\n", 2);
  mg_send(be, hm->body.p, hm->body.len);

cleanup:
  if (purl != burl) MG_FREE(purl);
}

static int mg_http_handle_forwarding(struct mg_connection *nc,
                                     struct http_message *hm,
                                     const struct mg_serve_http_opts *opts) {
  const char *rewrites = opts->url_rewrites;
  struct mg_str a, b;
  struct mg_str p1 = MG_MK_STR("http://"), p2 = MG_MK_STR("https://");

  while ((rewrites = mg_next_comma_list_entry(rewrites, &a, &b)) != NULL) {
    if (mg_strncmp(a, hm->uri, a.len) == 0) {
      if (mg_strncmp(b, p1, p1.len) == 0 || mg_strncmp(b, p2, p2.len) == 0) {
        mg_http_reverse_proxy(nc, hm, a, b);
        return 1;
      }
    }
  }

  return 0;
}
#endif /* MG_ENABLE_FILESYSTEM */

MG_INTERNAL int mg_uri_to_local_path(struct http_message *hm,
                                     const struct mg_serve_http_opts *opts,
                                     char **local_path,
                                     struct mg_str *remainder) {
  int ok = 1;
  const char *cp = hm->uri.p, *cp_end = hm->uri.p + hm->uri.len;
  struct mg_str root = {NULL, 0};
  const char *file_uri_start = cp;
  *local_path = NULL;
  remainder->p = NULL;
  remainder->len = 0;

  { /* 1. Determine which root to use. */

#if MG_ENABLE_HTTP_URL_REWRITES
    const char *rewrites = opts->url_rewrites;
#else
    const char *rewrites = "";
#endif
    struct mg_str *hh = mg_get_http_header(hm, "Host");
    struct mg_str a, b;
    /* Check rewrites first. */
    while ((rewrites = mg_next_comma_list_entry(rewrites, &a, &b)) != NULL) {
      if (a.len > 1 && a.p[0] == '@') {
        /* Host rewrite. */
        if (hh != NULL && hh->len == a.len - 1 &&
            mg_ncasecmp(a.p + 1, hh->p, a.len - 1) == 0) {
          root = b;
          break;
        }
      } else {
        /* Regular rewrite, URI=directory */
        size_t match_len = mg_match_prefix_n(a, hm->uri);
        if (match_len > 0) {
          file_uri_start = hm->uri.p + match_len;
          if (*file_uri_start == '/' || file_uri_start == cp_end) {
            /* Match ended at component boundary, ok. */
          } else if (*(file_uri_start - 1) == '/') {
            /* Pattern ends with '/', backtrack. */
            file_uri_start--;
          } else {
            /* No match: must fall on the component boundary. */
            continue;
          }
          root = b;
          break;
        }
      }
    }
    /* If no rewrite rules matched, use DAV or regular document root. */
    if (root.p == NULL) {
#if MG_ENABLE_HTTP_WEBDAV
      if (opts->dav_document_root != NULL && mg_is_dav_request(&hm->method)) {
        root.p = opts->dav_document_root;
        root.len = strlen(opts->dav_document_root);
      } else
#endif
      {
        root.p = opts->document_root;
        root.len = strlen(opts->document_root);
      }
    }
    assert(root.p != NULL && root.len > 0);
  }

  { /* 2. Find where in the canonical URI path the local path ends. */
    const char *u = file_uri_start + 1;
    char *lp = (char *) MG_MALLOC(root.len + hm->uri.len + 1);
    char *lp_end = lp + root.len + hm->uri.len + 1;
    char *p = lp, *ps;
    int exists = 1;
    if (lp == NULL) {
      ok = 0;
      goto out;
    }
    memcpy(p, root.p, root.len);
    p += root.len;
    if (*(p - 1) == DIRSEP) p--;
    *p = '\0';
    ps = p;

    /* Chop off URI path components one by one and build local path. */
    while (u <= cp_end) {
      const char *next = u;
      struct mg_str component;
      if (exists) {
        cs_stat_t st;
        exists = (mg_stat(lp, &st) == 0);
        if (exists && S_ISREG(st.st_mode)) {
          /* We found the terminal, the rest of the URI (if any) is path_info.
           */
          if (*(u - 1) == '/') u--;
          break;
        }
      }
      if (u >= cp_end) break;
      parse_uri_component((const char **) &next, cp_end, "/", &component);
      if (component.len > 0) {
        int len;
        memmove(p + 1, component.p, component.len);
        len = mg_url_decode(p + 1, component.len, p + 1, lp_end - p - 1, 0);
        if (len <= 0) {
          ok = 0;
          break;
        }
        component.p = p + 1;
        component.len = len;
        if (mg_vcmp(&component, ".") == 0) {
          /* Yum. */
        } else if (mg_vcmp(&component, "..") == 0) {
          while (p > ps && *p != DIRSEP) p--;
          *p = '\0';
        } else {
          size_t i;
#ifdef _WIN32
          /* On Windows, make sure it's valid Unicode (no funny stuff). */
          wchar_t buf[MG_MAX_PATH * 2];
          if (to_wchar(component.p, buf, MG_MAX_PATH) == 0) {
            DBG(("[%.*s] smells funny", (int) component.len, component.p));
            ok = 0;
            break;
          }
#endif
          *p++ = DIRSEP;
          /* No NULs and DIRSEPs in the component (percent-encoded). */
          for (i = 0; i < component.len; i++, p++) {
            if (*p == '\0' || *p == DIRSEP
#ifdef _WIN32
                /* On Windows, "/" is also accepted, so check for that too. */
                ||
                *p == '/'
#endif
                ) {
              ok = 0;
              break;
            }
          }
        }
      }
      u = next;
    }
    if (ok) {
      *local_path = lp;
      if (u > cp_end) u = cp_end;
      remainder->p = u;
      remainder->len = cp_end - u;
    } else {
      MG_FREE(lp);
    }
  }

out:
  LOG(LL_DEBUG,
      ("'%.*s' -> '%s' + '%.*s'", (int) hm->uri.len, hm->uri.p,
       *local_path ? *local_path : "", (int) remainder->len, remainder->p));
  return ok;
}

static int mg_get_month_index(const char *s) {
  static const char *month_names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  size_t i;

  for (i = 0; i < ARRAY_SIZE(month_names); i++)
    if (!strcmp(s, month_names[i])) return (int) i;

  return -1;
}

static int mg_num_leap_years(int year) {
  return year / 4 - year / 100 + year / 400;
}

/* Parse UTC date-time string, and return the corresponding time_t value. */
MG_INTERNAL time_t mg_parse_date_string(const char *datetime) {
  static const unsigned short days_before_month[] = {
      0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  char month_str[32];
  int second, minute, hour, day, month, year, leap_days, days;
  time_t result = (time_t) 0;

  if (((sscanf(datetime, "%d/%3s/%d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6) ||
       (sscanf(datetime, "%d %3s %d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6) ||
       (sscanf(datetime, "%*3s, %d %3s %d %d:%d:%d", &day, month_str, &year,
               &hour, &minute, &second) == 6) ||
       (sscanf(datetime, "%d-%3s-%d %d:%d:%d", &day, month_str, &year, &hour,
               &minute, &second) == 6)) &&
      year > 1970 && (month = mg_get_month_index(month_str)) != -1) {
    leap_days = mg_num_leap_years(year) - mg_num_leap_years(1970);
    year -= 1970;
    days = year * 365 + days_before_month[month] + (day - 1) + leap_days;
    result = days * 24 * 3600 + hour * 3600 + minute * 60 + second;
  }

  return result;
}

MG_INTERNAL int mg_is_not_modified(struct http_message *hm, cs_stat_t *st) {
  struct mg_str *hdr;
  if ((hdr = mg_get_http_header(hm, "If-None-Match")) != NULL) {
    char etag[64];
    mg_http_construct_etag(etag, sizeof(etag), st);
    return mg_vcasecmp(hdr, etag) == 0;
  } else if ((hdr = mg_get_http_header(hm, "If-Modified-Since")) != NULL) {
    return st->st_mtime <= mg_parse_date_string(hdr->p);
  } else {
    return 0;
  }
}

void mg_http_send_digest_auth_request(struct mg_connection *c,
                                      const char *domain) {
  mg_printf(c,
            "HTTP/1.1 401 Unauthorized\r\n"
            "WWW-Authenticate: Digest qop=\"auth\", "
            "realm=\"%s\", nonce=\"%lx\"\r\n"
            "Content-Length: 0\r\n\r\n",
            domain, (unsigned long) mg_time());
}

static void mg_http_send_options(struct mg_connection *nc) {
  mg_printf(nc, "%s",
            "HTTP/1.1 200 OK\r\nAllow: GET, POST, HEAD, CONNECT, OPTIONS"
#if MG_ENABLE_HTTP_WEBDAV
            ", MKCOL, PUT, DELETE, PROPFIND, MOVE\r\nDAV: 1,2"
#endif
            "\r\n\r\n");
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

static int mg_is_creation_request(const struct http_message *hm) {
  return mg_vcmp(&hm->method, "MKCOL") == 0 || mg_vcmp(&hm->method, "PUT") == 0;
}

MG_INTERNAL void mg_send_http_file(struct mg_connection *nc, char *path,
                                   const struct mg_str *path_info,
                                   struct http_message *hm,
                                   struct mg_serve_http_opts *opts) {
  int exists, is_directory, is_cgi;
#if MG_ENABLE_HTTP_WEBDAV
  int is_dav = mg_is_dav_request(&hm->method);
#else
  int is_dav = 0;
#endif
  char *index_file = NULL;
  cs_stat_t st;

  exists = (mg_stat(path, &st) == 0);
  is_directory = exists && S_ISDIR(st.st_mode);

  if (is_directory)
    mg_find_index_file(path, opts->index_files, &index_file, &st);

  is_cgi =
      (mg_match_prefix(opts->cgi_file_pattern, strlen(opts->cgi_file_pattern),
                       index_file ? index_file : path) > 0);

  LOG(LL_DEBUG,
      ("%p %.*s [%s] exists=%d is_dir=%d is_dav=%d is_cgi=%d index=%s", nc,
       (int) hm->method.len, hm->method.p, path, exists, is_directory, is_dav,
       is_cgi, index_file ? index_file : ""));

  if (is_directory && hm->uri.p[hm->uri.len - 1] != '/' && !is_dav) {
    mg_printf(nc,
              "HTTP/1.1 301 Moved\r\nLocation: %.*s/\r\n"
              "Content-Length: 0\r\n\r\n",
              (int) hm->uri.len, hm->uri.p);
    MG_FREE(index_file);
    return;
  }

  /* If we have path_info, the only way to handle it is CGI. */
  if (path_info->len > 0 && !is_cgi) {
    mg_http_send_error(nc, 501, NULL);
    MG_FREE(index_file);
    return;
  }

  if (is_dav && opts->dav_document_root == NULL) {
    mg_http_send_error(nc, 501, NULL);
  } else if (!mg_http_is_authorized(
                 hm, mg_mk_str(path), opts->auth_domain, opts->global_auth_file,
                 ((is_directory ? MG_AUTH_FLAG_IS_DIRECTORY : 0) |
                  MG_AUTH_FLAG_IS_GLOBAL_PASS_FILE |
                  MG_AUTH_FLAG_ALLOW_MISSING_FILE)) ||
             !mg_http_is_authorized(
                 hm, mg_mk_str(path), opts->auth_domain,
                 opts->per_directory_auth_file,
                 ((is_directory ? MG_AUTH_FLAG_IS_DIRECTORY : 0) |
                  MG_AUTH_FLAG_ALLOW_MISSING_FILE))) {
    mg_http_send_digest_auth_request(nc, opts->auth_domain);
  } else if (is_cgi) {
#if MG_ENABLE_HTTP_CGI
    mg_handle_cgi(nc, index_file ? index_file : path, path_info, hm, opts);
#else
    mg_http_send_error(nc, 501, NULL);
#endif /* MG_ENABLE_HTTP_CGI */
  } else if ((!exists ||
              mg_is_file_hidden(path, opts, 0 /* specials are ok */)) &&
             !mg_is_creation_request(hm)) {
    mg_http_send_error(nc, 404, NULL);
#if MG_ENABLE_HTTP_WEBDAV
  } else if (!mg_vcmp(&hm->method, "PROPFIND")) {
    mg_handle_propfind(nc, path, &st, hm, opts);
#if !MG_DISABLE_DAV_AUTH
  } else if (is_dav &&
             (opts->dav_auth_file == NULL ||
              (strcmp(opts->dav_auth_file, "-") != 0 &&
               !mg_http_is_authorized(
                   hm, mg_mk_str(path), opts->auth_domain, opts->dav_auth_file,
                   ((is_directory ? MG_AUTH_FLAG_IS_DIRECTORY : 0) |
                    MG_AUTH_FLAG_IS_GLOBAL_PASS_FILE |
                    MG_AUTH_FLAG_ALLOW_MISSING_FILE))))) {
    mg_http_send_digest_auth_request(nc, opts->auth_domain);
#endif
  } else if (!mg_vcmp(&hm->method, "MKCOL")) {
    mg_handle_mkcol(nc, path, hm);
  } else if (!mg_vcmp(&hm->method, "DELETE")) {
    mg_handle_delete(nc, opts, path);
  } else if (!mg_vcmp(&hm->method, "PUT")) {
    mg_handle_put(nc, path, hm);
  } else if (!mg_vcmp(&hm->method, "MOVE")) {
    mg_handle_move(nc, opts, path, hm);
#if MG_ENABLE_FAKE_DAVLOCK
  } else if (!mg_vcmp(&hm->method, "LOCK")) {
    mg_handle_lock(nc, path);
#endif
#endif /* MG_ENABLE_HTTP_WEBDAV */
  } else if (!mg_vcmp(&hm->method, "OPTIONS")) {
    mg_http_send_options(nc);
  } else if (is_directory && index_file == NULL) {
#if MG_ENABLE_DIRECTORY_LISTING
    if (strcmp(opts->enable_directory_listing, "yes") == 0) {
      mg_send_directory_listing(nc, path, hm, opts);
    } else {
      mg_http_send_error(nc, 403, NULL);
    }
#else
    mg_http_send_error(nc, 501, NULL);
#endif
  } else if (mg_is_not_modified(hm, &st)) {
    mg_http_send_error(nc, 304, "Not Modified");
  } else {
    mg_http_serve_file2(nc, index_file ? index_file : path, hm, opts);
  }
  MG_FREE(index_file);
}

void mg_serve_http(struct mg_connection *nc, struct http_message *hm,
                   struct mg_serve_http_opts opts) {
  char *path = NULL;
  struct mg_str *hdr, path_info;
  uint32_t remote_ip = ntohl(*(uint32_t *) &nc->sa.sin.sin_addr);

  if (mg_check_ip_acl(opts.ip_acl, remote_ip) != 1) {
    /* Not allowed to connect */
    mg_http_send_error(nc, 403, NULL);
    nc->flags |= MG_F_SEND_AND_CLOSE;
    return;
  }

#if MG_ENABLE_HTTP_URL_REWRITES
  if (mg_http_handle_forwarding(nc, hm, &opts)) {
    return;
  }

  if (mg_http_send_port_based_redirect(nc, hm, &opts)) {
    return;
  }
#endif

  if (opts.document_root == NULL) {
    opts.document_root = ".";
  }
  if (opts.per_directory_auth_file == NULL) {
    opts.per_directory_auth_file = ".htpasswd";
  }
  if (opts.enable_directory_listing == NULL) {
    opts.enable_directory_listing = "yes";
  }
  if (opts.cgi_file_pattern == NULL) {
    opts.cgi_file_pattern = "**.cgi$|**.php$";
  }
  if (opts.ssi_pattern == NULL) {
    opts.ssi_pattern = "**.shtml$|**.shtm$";
  }
  if (opts.index_files == NULL) {
    opts.index_files = "index.html,index.htm,index.shtml,index.cgi,index.php";
  }
  /* Normalize path - resolve "." and ".." (in-place). */
  if (!mg_normalize_uri_path(&hm->uri, &hm->uri)) {
    mg_http_send_error(nc, 400, NULL);
    return;
  }
  if (mg_uri_to_local_path(hm, &opts, &path, &path_info) == 0) {
    mg_http_send_error(nc, 404, NULL);
    return;
  }
  mg_send_http_file(nc, path, &path_info, hm, &opts);

  MG_FREE(path);
  path = NULL;

  /* Close connection for non-keep-alive requests */
  if (mg_vcmp(&hm->proto, "HTTP/1.1") != 0 ||
      ((hdr = mg_get_http_header(hm, "Connection")) != NULL &&
       mg_vcmp(hdr, "keep-alive") != 0)) {
#if 0
    nc->flags |= MG_F_SEND_AND_CLOSE;
#endif
  }
}

#if MG_ENABLE_HTTP_STREAMING_MULTIPART
void mg_file_upload_handler(struct mg_connection *nc, int ev, void *ev_data,
                            mg_fu_fname_fn local_name_fn
                                MG_UD_ARG(void *user_data)) {
  switch (ev) {
    case MG_EV_HTTP_PART_BEGIN: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) MG_CALLOC(1, sizeof(*fus));
      struct mg_str lfn = local_name_fn(nc, mg_mk_str(mp->file_name));
      mp->user_data = NULL;
      if (lfn.p == NULL || lfn.len == 0) {
        LOG(LL_ERROR, ("%p Not allowed to upload %s", nc, mp->file_name));
        mg_printf(nc,
                  "HTTP/1.1 403 Not Allowed\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n"
                  "Not allowed to upload %s\r\n",
                  mp->file_name);
        nc->flags |= MG_F_SEND_AND_CLOSE;
        return;
      }
      fus->lfn = (char *) MG_MALLOC(lfn.len + 1);
      memcpy(fus->lfn, lfn.p, lfn.len);
      fus->lfn[lfn.len] = '\0';
      if (lfn.p != mp->file_name) MG_FREE((char *) lfn.p);
      LOG(LL_DEBUG,
          ("%p Receiving file %s -> %s", nc, mp->file_name, fus->lfn));
      fus->fp = mg_fopen(fus->lfn, "w");
      if (fus->fp == NULL) {
        mg_printf(nc,
                  "HTTP/1.1 500 Internal Server Error\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n");
        LOG(LL_ERROR, ("Failed to open %s: %d\n", fus->lfn, mg_get_errno()));
        mg_printf(nc, "Failed to open %s: %d\n", fus->lfn, mg_get_errno());
        /* Do not close the connection just yet, discard remainder of the data.
         * This is because at the time of writing some browsers (Chrome) fail to
         * render response before all the data is sent. */
      }
      mp->user_data = (void *) fus;
      break;
    }
    case MG_EV_HTTP_PART_DATA: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) mp->user_data;
      if (fus == NULL || fus->fp == NULL) break;
      if (mg_fwrite(mp->data.p, 1, mp->data.len, fus->fp) != mp->data.len) {
        LOG(LL_ERROR, ("Failed to write to %s: %d, wrote %d", fus->lfn,
                       mg_get_errno(), (int) fus->num_recd));
        if (mg_get_errno() == ENOSPC
#ifdef SPIFFS_ERR_FULL
            || mg_get_errno() == SPIFFS_ERR_FULL
#endif
            ) {
          mg_printf(nc,
                    "HTTP/1.1 413 Payload Too Large\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n\r\n");
          mg_printf(nc, "Failed to write to %s: no space left; wrote %d\r\n",
                    fus->lfn, (int) fus->num_recd);
        } else {
          mg_printf(nc,
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n\r\n");
          mg_printf(nc, "Failed to write to %s: %d, wrote %d", mp->file_name,
                    mg_get_errno(), (int) fus->num_recd);
        }
        fclose(fus->fp);
        remove(fus->lfn);
        fus->fp = NULL;
        /* Do not close the connection just yet, discard remainder of the data.
         * This is because at the time of writing some browsers (Chrome) fail to
         * render response before all the data is sent. */
        return;
      }
      fus->num_recd += mp->data.len;
      LOG(LL_DEBUG, ("%p rec'd %d bytes, %d total", nc, (int) mp->data.len,
                     (int) fus->num_recd));
      break;
    }
    case MG_EV_HTTP_PART_END: {
      struct mg_http_multipart_part *mp =
          (struct mg_http_multipart_part *) ev_data;
      struct file_upload_state *fus =
          (struct file_upload_state *) mp->user_data;
      if (fus == NULL) break;
      if (mp->status >= 0 && fus->fp != NULL) {
        LOG(LL_DEBUG, ("%p Uploaded %s (%s), %d bytes", nc, mp->file_name,
                       fus->lfn, (int) fus->num_recd));
        mg_printf(nc,
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n"
                  "Connection: close\r\n\r\n"
                  "Ok, %s - %d bytes.\r\n",
                  mp->file_name, (int) fus->num_recd);
      } else {
        LOG(LL_ERROR, ("Failed to store %s (%s)", mp->file_name, fus->lfn));
        /*
         * mp->status < 0 means connection was terminated, so no reason to send
         * HTTP reply
         */
      }
      if (fus->fp != NULL) fclose(fus->fp);
      MG_FREE(fus->lfn);
      MG_FREE(fus);
      mp->user_data = NULL;
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    }
  }

#if MG_ENABLE_CALLBACK_USERDATA
  (void) user_data;
#endif
}

#endif /* MG_ENABLE_HTTP_STREAMING_MULTIPART */
#endif /* MG_ENABLE_FILESYSTEM */

struct mg_connection *mg_connect_http_base(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    struct mg_connect_opts opts, const char *scheme1, const char *scheme2,
    const char *scheme_ssl1, const char *scheme_ssl2, const char *url,
    struct mg_str *path, struct mg_str *user_info, struct mg_str *host) {
  struct mg_connection *nc = NULL;
  unsigned int port_i = 0;
  int use_ssl = 0;
  struct mg_str scheme, query, fragment;
  char conn_addr_buf[2];
  char *conn_addr = conn_addr_buf;

  if (mg_parse_uri(mg_mk_str(url), &scheme, user_info, host, &port_i, path,
                   &query, &fragment) != 0) {
    MG_SET_PTRPTR(opts.error_string, "cannot parse url");
    goto out;
  }

  /* If query is present, do not strip it. Pass to the caller. */
  if (query.len > 0) path->len += query.len + 1;

  if (scheme.len == 0 || mg_vcmp(&scheme, scheme1) == 0 ||
      (scheme2 != NULL && mg_vcmp(&scheme, scheme2) == 0)) {
    use_ssl = 0;
    if (port_i == 0) port_i = 80;
  } else if (mg_vcmp(&scheme, scheme_ssl1) == 0 ||
             (scheme2 != NULL && mg_vcmp(&scheme, scheme_ssl2) == 0)) {
    use_ssl = 1;
    if (port_i == 0) port_i = 443;
  } else {
    goto out;
  }

  mg_asprintf(&conn_addr, sizeof(conn_addr_buf), "tcp://%.*s:%u",
              (int) host->len, host->p, port_i);
  if (conn_addr == NULL) goto out;

  LOG(LL_DEBUG, ("%s use_ssl? %d %s", url, use_ssl, conn_addr));
  if (use_ssl) {
#if MG_ENABLE_SSL
    /*
     * Schema requires SSL, but no SSL parameters were provided in opts.
     * In order to maintain backward compatibility, use a faux-SSL with no
     * verification.
     */
    if (opts.ssl_ca_cert == NULL) {
      opts.ssl_ca_cert = "*";
    }
#else
    MG_SET_PTRPTR(opts.error_string, "ssl is disabled");
    goto out;
#endif
  }

  if ((nc = mg_connect_opt(mgr, conn_addr, MG_CB(ev_handler, user_data),
                           opts)) != NULL) {
    mg_set_protocol_http_websocket(nc);
  }

out:
  if (conn_addr != NULL && conn_addr != conn_addr_buf) MG_FREE(conn_addr);
  return nc;
}

struct mg_connection *mg_connect_http_opt(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    struct mg_connect_opts opts, const char *url, const char *extra_headers,
    const char *post_data) {
  struct mg_str user = MG_NULL_STR, null_str = MG_NULL_STR;
  struct mg_str host = MG_NULL_STR, path = MG_NULL_STR;
  struct mbuf auth;
  struct mg_connection *nc =
      mg_connect_http_base(mgr, MG_CB(ev_handler, user_data), opts, "http",
                           NULL, "https", NULL, url, &path, &user, &host);

  if (nc == NULL) {
    return NULL;
  }

  mbuf_init(&auth, 0);
  if (user.len > 0) {
    mg_basic_auth_header(user, null_str, &auth);
  }

  if (post_data == NULL) post_data = "";
  if (extra_headers == NULL) extra_headers = "";
  if (path.len == 0) path = mg_mk_str("/");
  if (host.len == 0) host = mg_mk_str("");

  mg_printf(nc, "%s %.*s HTTP/1.1\r\nHost: %.*s\r\nContent-Length: %" SIZE_T_FMT
                "\r\n%.*s%s\r\n%s",
            (post_data[0] == '\0' ? "GET" : "POST"), (int) path.len, path.p,
            (int) (path.p - host.p), host.p, strlen(post_data), (int) auth.len,
            (auth.buf == NULL ? "" : auth.buf), extra_headers, post_data);

  mbuf_free(&auth);
  return nc;
}

struct mg_connection *mg_connect_http(
    struct mg_mgr *mgr, MG_CB(mg_event_handler_t ev_handler, void *user_data),
    const char *url, const char *extra_headers, const char *post_data) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_http_opt(mgr, MG_CB(ev_handler, user_data), opts, url,
                             extra_headers, post_data);
}

size_t mg_parse_multipart(const char *buf, size_t buf_len, char *var_name,
                          size_t var_name_len, char *file_name,
                          size_t file_name_len, const char **data,
                          size_t *data_len) {
  static const char cd[] = "Content-Disposition: ";
  size_t hl, bl, n, ll, pos, cdl = sizeof(cd) - 1;
  int shl;

  if (buf == NULL || buf_len <= 0) return 0;
  if ((shl = mg_http_get_request_len(buf, buf_len)) <= 0) return 0;
  hl = shl;
  if (buf[0] != '-' || buf[1] != '-' || buf[2] == '\n') return 0;

  /* Get boundary length */
  bl = mg_get_line_len(buf, buf_len);

  /* Loop through headers, fetch variable name and file name */
  var_name[0] = file_name[0] = '\0';
  for (n = bl; (ll = mg_get_line_len(buf + n, hl - n)) > 0; n += ll) {
    if (mg_ncasecmp(cd, buf + n, cdl) == 0) {
      struct mg_str header;
      header.p = buf + n + cdl;
      header.len = ll - (cdl + 2);
      {
        char *var_name2 = var_name;
        mg_http_parse_header2(&header, "name", &var_name2, var_name_len);
        /* TODO: handle reallocated buffer correctly */
        if (var_name2 != var_name) {
          MG_FREE(var_name2);
          var_name[0] = '\0';
        }
      }
      {
        char *file_name2 = file_name;
        mg_http_parse_header2(&header, "filename", &file_name2, file_name_len);
        /* TODO: handle reallocated buffer correctly */
        if (file_name2 != file_name) {
          MG_FREE(file_name2);
          file_name[0] = '\0';
        }
      }
    }
  }

  /* Scan through the body, search for terminating boundary */
  for (pos = hl; pos + (bl - 2) < buf_len; pos++) {
    if (buf[pos] == '-' && !strncmp(buf, &buf[pos], bl - 2)) {
      if (data_len != NULL) *data_len = (pos - 2) - hl;
      if (data != NULL) *data = buf + hl;
      return pos;
    }
  }

  return 0;
}

void mg_register_http_endpoint_opt(struct mg_connection *nc,
                                   const char *uri_path,
                                   mg_event_handler_t handler,
                                   struct mg_http_endpoint_opts opts) {
  struct mg_http_proto_data *pd = NULL;
  struct mg_http_endpoint *new_ep = NULL;

  if (nc == NULL) return;
  new_ep = (struct mg_http_endpoint *) MG_CALLOC(1, sizeof(*new_ep));
  if (new_ep == NULL) return;

  pd = mg_http_get_proto_data(nc);
  new_ep->uri_pattern = mg_strdup(mg_mk_str(uri_path));
  if (opts.auth_domain != NULL && opts.auth_file != NULL) {
    new_ep->auth_domain = strdup(opts.auth_domain);
    new_ep->auth_file = strdup(opts.auth_file);
  }
  new_ep->handler = handler;
#if MG_ENABLE_CALLBACK_USERDATA
  new_ep->user_data = opts.user_data;
#endif
  new_ep->next = pd->endpoints;
  pd->endpoints = new_ep;
}

static void mg_http_call_endpoint_handler(struct mg_connection *nc, int ev,
                                          struct http_message *hm) {
  struct mg_http_proto_data *pd = mg_http_get_proto_data(nc);
  void *user_data = nc->user_data;

  if (ev == MG_EV_HTTP_REQUEST
#if MG_ENABLE_HTTP_STREAMING_MULTIPART
      || ev == MG_EV_HTTP_MULTIPART_REQUEST
#endif
      ) {
    struct mg_http_endpoint *ep =
        mg_http_get_endpoint_handler(nc->listener, &hm->uri);
    if (ep != NULL) {
#if MG_ENABLE_FILESYSTEM && !MG_DISABLE_HTTP_DIGEST_AUTH
      if (!mg_http_is_authorized(hm, hm->uri, ep->auth_domain, ep->auth_file,
                                 MG_AUTH_FLAG_IS_GLOBAL_PASS_FILE)) {
        mg_http_send_digest_auth_request(nc, ep->auth_domain);
        return;
      }
#endif
      pd->endpoint_handler = ep->handler;
#if MG_ENABLE_CALLBACK_USERDATA
      user_data = ep->user_data;
#endif
    }
  }
  mg_call(nc, pd->endpoint_handler ? pd->endpoint_handler : nc->handler,
          user_data, ev, hm);
}

void mg_register_http_endpoint(struct mg_connection *nc, const char *uri_path,
                               MG_CB(mg_event_handler_t handler,
                                     void *user_data)) {
  struct mg_http_endpoint_opts opts;
  memset(&opts, 0, sizeof(opts));
#if MG_ENABLE_CALLBACK_USERDATA
  opts.user_data = user_data;
#endif
  mg_register_http_endpoint_opt(nc, uri_path, handler, opts);
}

#endif /* MG_ENABLE_HTTP */
