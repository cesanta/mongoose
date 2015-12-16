/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "../../mongoose.h"
#include <sys/queue.h>

#define MAX_IDLE_CONNS 5
#define CONN_IDLE_TIMEOUT 30

struct http_backend;

struct be_conn {
  struct http_backend *be;
  struct mg_connection *nc;
  time_t idle_deadline;

  STAILQ_ENTRY(be_conn) conns;
};

STAILQ_HEAD(be_conn_list_head, be_conn);
struct http_backend {
  const char *vhost;      /* NULL if any host */
  const char *uri_prefix; /* URI prefix, e.g. "/api/v1/", "/static/" */
  const char *uri_prefix_replacement; /* if not NULL, will replace uri_prefix in
                                         requests to backends */
  const char *host_port;              /* Backend address */
  int redirect;                       /* if true redirect instead of proxy */
  int usage_counter; /* Number of times this backend was chosen */

  struct be_conn_list_head conns;
  int num_conns;
};

struct peer {
  struct mg_connection *nc;
  int64_t body_len;  /* Size of the HTTP body to forward */
  int64_t body_sent; /* Number of bytes already forwarded */
  struct {
    /* Headers have been sent, no more headers. */
    unsigned int headers_sent : 1;
    unsigned int keep_alive : 1;
  } flags;
};

struct conn_data {
  struct be_conn *be_conn; /* Chosen backend */
  struct peer client;      /* Client peer */
  struct peer backend;     /* Backend peer */
  time_t last_activity;
};

static const char *s_error_500 = "HTTP/1.1 500 Failed\r\n";
static const char *s_content_len_0 = "Content-Length: 0\r\n";
static const char *s_connection_close = "Connection: close\r\n";
static const char *s_http_port = "8000";
static struct http_backend s_vhost_backends[100], s_default_backends[100];
static int s_num_vhost_backends = 0, s_num_default_backends = 0;
static int s_sig_num = 0;
static int s_backend_keepalive = 0;
static FILE *s_log_file = NULL;
#ifdef MG_ENABLE_SSL
const char *s_ssl_cert = NULL;
#endif

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
static void write_log(const char *fmt, ...);

static void signal_handler(int sig_num) {
  signal(sig_num, signal_handler);
  s_sig_num = sig_num;
}

static void send_http_err(struct mg_connection *nc, const char *err_line) {
  mg_printf(nc, "%s%s%s\r\n", err_line, s_content_len_0, s_connection_close);
}

static void respond_with_error(struct conn_data *conn, const char *err_line) {
  struct mg_connection *nc = conn->client.nc;
  int headers_sent = conn->client.flags.headers_sent;
#ifdef DEBUG
  write_log("conn=%p nc=%p respond_with_error %d\n", conn, nc, headers_sent);
#endif
  if (nc == NULL) return;
  if (!headers_sent) {
    send_http_err(nc, err_line);
    conn->client.flags.headers_sent = 1;
  }
  nc->flags |= MG_F_SEND_AND_CLOSE;
}

static int has_prefix(const struct mg_str *uri, const char *prefix) {
  size_t prefix_len = strlen(prefix);
  return uri->len >= prefix_len && memcmp(uri->p, prefix, prefix_len) == 0;
}

static int matches_vhost(const struct mg_str *host, const char *vhost) {
  size_t vhost_len;
  if (vhost == NULL) {
    return 1;
  }
  vhost_len = strlen(vhost);
  return host->len == vhost_len && memcmp(host->p, vhost, vhost_len) == 0;
}

static void write_log(const char *fmt, ...) {
  va_list ap;
  if (s_log_file != NULL) {
    va_start(ap, fmt);
    vfprintf(s_log_file, fmt, ap);
    fflush(s_log_file);
    va_end(ap);
  }
}

static struct http_backend *choose_backend_from_list(
    struct http_message *hm, struct http_backend *backends, int num_backends) {
  int i;
  struct mg_str vhost = {"", 0};
  const struct mg_str *host = mg_get_http_header(hm, "host");
  if (host != NULL) vhost = *host;

  const char *vhost_end = vhost.p;

  while (vhost_end < vhost.p + vhost.len && *vhost_end != ':') {
    vhost_end++;
  }
  vhost.len = vhost_end - vhost.p;

  struct http_backend *chosen = NULL;
  for (i = 0; i < num_backends; i++) {
    struct http_backend *be = &backends[i];
    if (has_prefix(&hm->uri, be->uri_prefix) &&
        matches_vhost(&vhost, be->vhost) &&
        (chosen == NULL ||
         /* Prefer most specific URI prefixes */
         strlen(be->uri_prefix) > strlen(chosen->uri_prefix) ||
         /* Among prefixes of the same length chose the least used. */
         (strlen(be->uri_prefix) == strlen(chosen->uri_prefix) &&
          be->usage_counter < chosen->usage_counter))) {
      chosen = be;
    }
  }

  return chosen;
}

static struct http_backend *choose_backend(struct http_message *hm) {
  struct http_backend *chosen =
      choose_backend_from_list(hm, s_vhost_backends, s_num_vhost_backends);

  /* Nothing was chosen for this vhost, look for vhost == NULL backends. */
  if (chosen == NULL) {
    chosen = choose_backend_from_list(hm, s_default_backends,
                                      s_num_default_backends);
  }

  if (chosen != NULL) chosen->usage_counter++;

  return chosen;
}

static void forward_body(struct peer *src, struct peer *dst) {
  struct mbuf *src_io = &src->nc->recv_mbuf;
  if (src->body_sent < src->body_len) {
    size_t to_send = src->body_len - src->body_sent;
    if (src_io->len < to_send) {
      to_send = src_io->len;
    }
    mg_send(dst->nc, src_io->buf, to_send);
    src->body_sent += to_send;
    mbuf_remove(src_io, to_send);
  }
#ifdef DEBUG
  write_log("forward_body %p (ka=%d) -> %p sent %d of %d\n", src->nc,
            src->flags.keep_alive, dst->nc, src->body_sent, src->body_len);
#endif
}

static void forward(struct conn_data *conn, struct http_message *hm,
                    struct peer *src_peer, struct peer *dst_peer) {
  struct mg_connection *src = src_peer->nc;
  struct mg_connection *dst = dst_peer->nc;
  struct mbuf *io = &src->recv_mbuf;
  int i;
  int is_request = (src_peer == &conn->client);
  src_peer->body_len = hm->body.len;
  struct http_backend *be = conn->be_conn->be;

  if (is_request) {
    /* Write rewritten request line. */
    size_t trim_len = strlen(be->uri_prefix);
    mg_printf(dst, "%.*s%s%.*s\r\n", (int) (hm->uri.p - io->buf), io->buf,
              be->uri_prefix_replacement,
              (int) (hm->proto.p + hm->proto.len - (hm->uri.p + trim_len)),
              hm->uri.p + trim_len);
  } else {
    /* Reply line goes without modification */
    mg_printf(dst, "%.*s %d %.*s\r\n", (int) hm->proto.len, hm->proto.p,
              (int) hm->resp_code, (int) hm->resp_status_msg.len,
              hm->resp_status_msg.p);
  }

  /* Headers. */
  for (i = 0; i < MG_MAX_HTTP_HEADERS && hm->header_names[i].len > 0; i++) {
    struct mg_str hn = hm->header_names[i];
    struct mg_str hv = hm->header_values[i];
#ifdef MG_ENABLE_SSL
    /*
     * If we terminate SSL and backend redirects to local HTTP port,
     * strip protocol to let client use HTTPS.
     * TODO(lsm): web page content may also contain local HTTP references,
     * they need to be rewritten too.
     */
    if (mg_vcasecmp(&hn, "Location") == 0 && s_ssl_cert != NULL) {
      size_t hlen = strlen(be->host_port);
      const char *hp = be->host_port, *p = memchr(hp, ':', hlen);

      if (p == NULL) {
        p = hp + hlen;
      }

      if (mg_ncasecmp(hv.p, "http://", 7) == 0 &&
          mg_ncasecmp(hv.p + 7, hp, (p - hp)) == 0) {
        mg_printf(dst, "Location: %.*s\r\n", (int) (hv.len - (7 + (p - hp))),
                  hv.p + 7 + (p - hp));
        continue;
      }
    }
#endif

    /* We always rewrite the connection header depending on the settings. */
    if (mg_vcasecmp(&hn, "Connection") == 0) continue;

    /* Don't pass chunked transfer encoding to the client */
    if (mg_vcasecmp(&hn, "Transfer-encoding") == 0 &&
        mg_vcasecmp(&hv, "chunked") == 0) {
      continue;
    }

    mg_printf(dst, "%.*s: %.*s\r\n", (int) hn.len, hn.p, (int) hv.len, hv.p);
  }

  /* Emit the connection header. */
  const char *connection_mode = "close";
  if (dst_peer == &conn->backend) {
    if (s_backend_keepalive) connection_mode = "keep-alive";
  } else {
    if (conn->client.flags.keep_alive) connection_mode = "keep-alive";
  }
  mg_printf(dst, "Connection: %s\r\n", connection_mode);

  mg_printf(dst, "%s", "\r\n");

  mbuf_remove(io, hm->body.p - hm->message.p); /* We've forwarded headers */
  dst_peer->flags.headers_sent = 1;

  forward_body(src_peer, dst_peer);
}

struct be_conn *get_conn(struct http_backend *be) {
  if (STAILQ_EMPTY(&be->conns)) return NULL;
  struct be_conn *result = STAILQ_FIRST(&be->conns);
  STAILQ_REMOVE_HEAD(&be->conns, conns);
  be->num_conns--;
  return result;
}

/*
 * choose_backend parses incoming HTTP request and routes it to the appropriate
 * backend. It assumes that clients don't do HTTP pipelining, handling only
 * one request request for each connection. To give a hint to backend about
 * this it inserts "Connection: close" header into each forwarded request.
 */
static int connect_backend(struct conn_data *conn, struct http_message *hm) {
  struct mg_connection *nc = conn->client.nc;
  struct http_backend *be = choose_backend(hm);

  write_log("%.*s %.*s backend=%s\n", (int) hm->method.len, hm->method.p,
            (int) hm->uri.len, hm->uri.p, be ? be->host_port : "not defined");

  if (be == NULL) return 0;
  if (be->redirect != 0) {
    mg_printf(nc, "HTTP/1.1 302 Found\r\nLocation: %s\r\n\r\n", be->host_port);
    return 1;
  }
  struct be_conn *bec = get_conn(be);
  if (bec != NULL) {
    bec->nc->handler = ev_handler;
#ifdef DEBUG
    write_log("conn=%p to %p (%s) reusing bec=%p\n", conn, be, be->host_port,
              bec);
#endif
  } else {
    bec = malloc(sizeof(*conn->be_conn));
    memset(bec, 0, sizeof(*bec));
    bec->nc = mg_connect(nc->mgr, be->host_port, ev_handler);
#ifdef DEBUG
    write_log("conn=%p new conn to %p (%s) bec=%p\n", conn, be, be->host_port,
              bec);
#endif
    if (bec->nc == NULL) {
      free(bec);
      write_log("Connection to [%s] failed\n", be->host_port);
      return 0;
    }
  }
  bec->be = be;
  conn->be_conn = bec;
  conn->backend.nc = bec->nc;
  conn->backend.nc->user_data = conn;
  mg_set_protocol_http_websocket(conn->backend.nc);
  return 1;
}

static int is_keep_alive(struct http_message *hm) {
  const struct mg_str *connection_header = mg_get_http_header(hm, "Connection");
  if (connection_header == NULL) {
    /* HTTP/1.1 connections are keep-alive by default. */
    if (mg_vcasecmp(&hm->proto, "HTTP/1.1") != 0) return 0;
  } else if (mg_vcasecmp(connection_header, "keep-alive") != 0) {
    return 0;
  }
  // We must also have Content-Length.
  return mg_get_http_header(hm, "Content-Length") != NULL;
}

static void idle_backend_handler(struct mg_connection *nc, int ev,
                                 void *ev_data) {
  (void) ev_data; /* Unused. */
  struct be_conn *bec = nc->user_data;
  const time_t now = time(NULL);
#ifdef DEBUG
  write_log("%d idle bec=%p nc=%p ev=%d deadline=%d\n", now, bec, nc, ev,
            bec->idle_deadline);
#endif
  switch (ev) {
    case MG_EV_POLL: {
      if (bec->idle_deadline > 0 && now > bec->idle_deadline) {
#ifdef DEBUG
        write_log("bec=%p nc=%p closing due to idleness\n", bec, bec->nc);
#endif
        bec->nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      break;
    }

    case MG_EV_CLOSE: {
#ifdef DEBUG
      write_log("bec=%p closed\n", bec);
#endif
      if (bec->idle_deadline > 0) {
        STAILQ_REMOVE(&bec->be->conns, bec, be_conn, conns);
      }
      free(bec);
      break;
    }
  }
}

void release_backend(struct conn_data *conn) {
  /* Disassociate the backend, put back on the pool. */
  struct be_conn *bec = conn->be_conn;
  conn->be_conn = NULL;
  if (bec->nc == NULL) {
    free(bec);
    memset(&conn->backend, 0, sizeof(conn->backend));
    return;
  }
  struct http_backend *be = bec->be;
  bec->nc->user_data = bec;
  bec->nc->handler = idle_backend_handler;
  if (conn->backend.flags.keep_alive) {
    bec->idle_deadline = time(NULL) + CONN_IDLE_TIMEOUT;
    STAILQ_INSERT_TAIL(&be->conns, bec, conns);
#ifdef DEBUG
    write_log("bec=%p becoming idle\n", bec);
#endif
    be->num_conns++;
    while (be->num_conns > MAX_IDLE_CONNS) {
      bec = STAILQ_FIRST(&be->conns);
      STAILQ_REMOVE_HEAD(&be->conns, conns);
      be->num_conns--;
      bec->idle_deadline = 0;
      bec->nc->flags = MG_F_CLOSE_IMMEDIATELY;
#ifdef DEBUG
      write_log("bec=%p evicted\n", bec);
#endif
    }
  } else {
    bec->idle_deadline = 0;
    bec->nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
  memset(&conn->backend, 0, sizeof(conn->backend));
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct conn_data *conn = (struct conn_data *) nc->user_data;
  const time_t now = time(NULL);
#ifdef DEBUG
  write_log("%d conn=%p nc=%p ev=%d ev_data=%p bec=%p bec_nc=%p\n", now, conn,
            nc, ev, ev_data, conn != NULL ? conn->be_conn : NULL,
            conn != NULL && conn->be_conn != NULL ? conn->be_conn->nc : NULL);
#endif

  if (conn == NULL) {
    if (ev == MG_EV_ACCEPT) {
      conn = calloc(1, sizeof(*conn));
      if (conn == NULL) {
        send_http_err(nc, s_error_500);
      } else {
        memset(conn, 0, sizeof(*conn));
        nc->user_data = conn;
        conn->client.nc = nc;
        conn->client.body_len = -1;
        conn->backend.body_len = -1;
        conn->last_activity = now;
      }
      return;
    } else {
      if (ev != MG_EV_POLL) {
        nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      }
      return;
    }
  }

  if (ev != MG_EV_POLL) conn->last_activity = now;

  switch (ev) {
    case MG_EV_HTTP_REQUEST: { /* From client */
      assert(conn != NULL);
      assert(conn->be_conn == NULL);
      struct http_message *hm = (struct http_message *) ev_data;
      conn->client.flags.keep_alive = is_keep_alive(hm);

      if (!connect_backend(conn, hm)) {
        respond_with_error(conn, s_error_500);
        break;
      }

      if (conn->backend.nc == NULL) {
        /* This is a redirect, we're done. */
        conn->client.nc->flags |= MG_F_SEND_AND_CLOSE;
        break;
      }

      forward(conn, hm, &conn->client, &conn->backend);
      break;
    }

    case MG_EV_CONNECT: { /* To backend */
      assert(conn != NULL);
      assert(conn->be_conn != NULL);
      int status = *(int *) ev_data;
      if (status != 0) {
        write_log("Error connecting to %s: %d (%s)\n",
                  conn->be_conn->be->host_port, status, strerror(status));
        /* TODO(lsm): mark backend as defunct, try it later on */
        respond_with_error(conn, s_error_500);
        conn->be_conn->nc = NULL;
        release_backend(conn);
        break;
      }
      break;
    }

    case MG_EV_HTTP_REPLY: { /* From backend */
      assert(conn != NULL);
      struct http_message *hm = (struct http_message *) ev_data;
      conn->backend.flags.keep_alive = s_backend_keepalive && is_keep_alive(hm);
      forward(conn, hm, &conn->backend, &conn->client);
      release_backend(conn);
      if (!conn->client.flags.keep_alive) {
        conn->client.nc->flags |= MG_F_SEND_AND_CLOSE;
      } else {
#ifdef DEBUG
        write_log("conn=%p remains open\n", conn);
#endif
      }
      break;
    }

    case MG_EV_POLL: {
      assert(conn != NULL);
      if (now - conn->last_activity > CONN_IDLE_TIMEOUT &&
          conn->backend.nc == NULL /* not waiting for backend */) {
#ifdef DEBUG
        write_log("conn=%p has been idle for too long\n", conn);
        conn->client.nc->flags |= MG_F_SEND_AND_CLOSE;
#endif
      }
      break;
    }

    case MG_EV_CLOSE: {
      assert(conn != NULL);
      if (nc == conn->client.nc) {
#ifdef DEBUG
        write_log("conn=%p nc=%p client closed, body_sent=%d\n", conn, nc,
                  conn->backend.body_sent);
#endif
        conn->client.nc = NULL;
        if (conn->backend.nc != NULL) {
          conn->backend.nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
      } else if (nc == conn->backend.nc) {
#ifdef DEBUG
        write_log("conn=%p nc=%p backend closed\n", conn, nc);
#endif
        conn->backend.nc = NULL;
        if (conn->client.nc != NULL &&
            (conn->backend.body_len < 0 ||
             conn->backend.body_sent < conn->backend.body_len)) {
          write_log("Backend %s disconnected.\n", conn->be_conn->be->host_port);
          respond_with_error(conn, s_error_500);
        }
      }
      if (conn->client.nc == NULL && conn->backend.nc == NULL) {
        free(conn);
      }
      break;
    }
  }
}

static void print_usage_and_exit(const char *prog_name) {
  fprintf(stderr,
          "Usage: %s [-D debug_dump_file] [-p http_port] [-l log] [-k]"
#if MG_ENABLE_SSL
          "[-s ssl_cert] "
#endif
          "<[-r] [-v vhost] -b uri_prefix[=replacement] host_port> ... \n",
          prog_name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  int i, redirect = 0;
  const char *vhost = NULL;

  mg_mgr_init(&mgr, NULL);

  /* Parse command line arguments */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-D") == 0) {
      mgr.hexdump_file = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-k") == 0) {
      s_backend_keepalive = 1;
    } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
      if (strcmp(argv[i + 1], "-") == 0) {
        s_log_file = stdout;
      } else {
        s_log_file = fopen(argv[i + 1], "a");
        if (s_log_file == NULL) {
          perror("fopen");
          exit(EXIT_FAILURE);
        }
      }
      i++;
    } else if (strcmp(argv[i], "-p") == 0) {
      s_http_port = argv[i + 1];
      i++;
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      redirect = 1;
    } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
      if (strcmp(argv[i + 1], "") == 0) {
        vhost = NULL;
      } else {
        vhost = argv[i + 1];
      }
      i++;
    } else if (strcmp(argv[i], "-b") == 0 && i + 2 < argc) {
      struct http_backend *be =
          vhost != NULL ? &s_vhost_backends[s_num_vhost_backends++]
                        : &s_default_backends[s_num_default_backends++];
      STAILQ_INIT(&be->conns);
      char *r = NULL;
      be->vhost = vhost;
      be->uri_prefix = argv[i + 1];
      be->host_port = argv[i + 2];
      be->redirect = redirect;
      be->uri_prefix_replacement = be->uri_prefix;
      if ((r = strchr(be->uri_prefix, '=')) != NULL) {
        *r = '\0';
        be->uri_prefix_replacement = r + 1;
      }
      printf(
          "Adding backend for %s%s : %s "
          "[redirect=%d,prefix_replacement=%s]\n",
          be->vhost == NULL ? "" : be->vhost, be->uri_prefix, be->host_port,
          be->redirect, be->uri_prefix_replacement);
      vhost = NULL;
      redirect = 0;
      i += 2;
#ifdef MG_ENABLE_SSL
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      s_ssl_cert = argv[++i];
#endif
    } else {
      print_usage_and_exit(argv[0]);
    }
  }

  /* Open listening socket */
  if ((nc = mg_bind(&mgr, s_http_port, ev_handler)) == NULL) {
    fprintf(stderr, "mg_bind(%s) failed\n", s_http_port);
    exit(EXIT_FAILURE);
  }

#if MG_ENABLE_SSL
  if (s_ssl_cert != NULL) {
    const char *err_str = mg_set_ssl(nc, s_ssl_cert, NULL);
    if (err_str != NULL) {
      fprintf(stderr, "Error loading SSL cert: %s\n", err_str);
      exit(1);
    }
  }
#endif
  mg_set_protocol_http_websocket(nc);

  if (s_num_vhost_backends + s_num_default_backends == 0) {
    print_usage_and_exit(argv[0]);
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  /* Run event loop until signal is received */
  printf("Starting LB on port %s\n", s_http_port);
  while (s_sig_num == 0) {
    mg_mgr_poll(&mgr, 1000);
  }

  /* Cleanup */
  mg_mgr_free(&mgr);

  printf("Exiting on signal %d\n", s_sig_num);

  return EXIT_SUCCESS;
}
