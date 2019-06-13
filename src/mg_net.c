/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 *
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#include "common/cs_time.h"
#include "mg_dns.h"
#include "mg_internal.h"
#include "mg_resolv.h"
#include "mg_util.h"

#define MG_MAX_HOST_LEN 200

#ifndef MG_TCP_IO_SIZE
#define MG_TCP_IO_SIZE 1460
#endif
#ifndef MG_UDP_IO_SIZE
#define MG_UDP_IO_SIZE 1460
#endif

#define MG_COPY_COMMON_CONNECTION_OPTIONS(dst, src) \
  memcpy(dst, src, sizeof(*dst));

/* Which flags can be pre-set by the user at connection creation time. */
#define _MG_ALLOWED_CONNECT_FLAGS_MASK                                   \
  (MG_F_USER_1 | MG_F_USER_2 | MG_F_USER_3 | MG_F_USER_4 | MG_F_USER_5 | \
   MG_F_USER_6 | MG_F_WEBSOCKET_NO_DEFRAG | MG_F_ENABLE_BROADCAST)
/* Which flags should be modifiable by user's callbacks. */
#define _MG_CALLBACK_MODIFIABLE_FLAGS_MASK                               \
  (MG_F_USER_1 | MG_F_USER_2 | MG_F_USER_3 | MG_F_USER_4 | MG_F_USER_5 | \
   MG_F_USER_6 | MG_F_WEBSOCKET_NO_DEFRAG | MG_F_SEND_AND_CLOSE |        \
   MG_F_CLOSE_IMMEDIATELY | MG_F_IS_WEBSOCKET | MG_F_DELETE_CHUNK)

#ifndef intptr_t
#define intptr_t long
#endif

MG_INTERNAL void mg_add_conn(struct mg_mgr *mgr, struct mg_connection *c) {
  DBG(("%p %p", mgr, c));
  c->mgr = mgr;
  c->next = mgr->active_connections;
  mgr->active_connections = c;
  c->prev = NULL;
  if (c->next != NULL) c->next->prev = c;
  if (c->sock != INVALID_SOCKET) {
    c->iface->vtable->add_conn(c);
  }
}

MG_INTERNAL void mg_remove_conn(struct mg_connection *conn) {
  if (conn->prev == NULL) conn->mgr->active_connections = conn->next;
  if (conn->prev) conn->prev->next = conn->next;
  if (conn->next) conn->next->prev = conn->prev;
  conn->prev = conn->next = NULL;
  conn->iface->vtable->remove_conn(conn);
}

MG_INTERNAL void mg_call(struct mg_connection *nc,
                         mg_event_handler_t ev_handler, void *user_data, int ev,
                         void *ev_data) {
  if (ev_handler == NULL) {
    /*
     * If protocol handler is specified, call it. Otherwise, call user-specified
     * event handler.
     */
    ev_handler = nc->proto_handler ? nc->proto_handler : nc->handler;
  }
  if (ev != MG_EV_POLL) {
    DBG(("%p %s ev=%d ev_data=%p flags=0x%lx rmbl=%d smbl=%d", nc,
         ev_handler == nc->handler ? "user" : "proto", ev, ev_data, nc->flags,
         (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
  }

#if !defined(NO_LIBC) && MG_ENABLE_HEXDUMP
  if (nc->mgr->hexdump_file != NULL && ev != MG_EV_POLL && ev != MG_EV_RECV &&
      ev != MG_EV_SEND /* handled separately */) {
    mg_hexdump_connection(nc, nc->mgr->hexdump_file, NULL, 0, ev);
  }
#endif
  if (ev_handler != NULL) {
    unsigned long flags_before = nc->flags;
    ev_handler(nc, ev, ev_data MG_UD_ARG(user_data));
    /* Prevent user handler from fiddling with system flags. */
    if (ev_handler == nc->handler && nc->flags != flags_before) {
      nc->flags = (flags_before & ~_MG_CALLBACK_MODIFIABLE_FLAGS_MASK) |
                  (nc->flags & _MG_CALLBACK_MODIFIABLE_FLAGS_MASK);
    }
  }
  if (ev != MG_EV_POLL) nc->mgr->num_calls++;
  if (ev != MG_EV_POLL) {
    DBG(("%p after %s flags=0x%lx rmbl=%d smbl=%d", nc,
         ev_handler == nc->handler ? "user" : "proto", nc->flags,
         (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
  }
#if !MG_ENABLE_CALLBACK_USERDATA
  (void) user_data;
#endif
}

MG_INTERNAL void mg_timer(struct mg_connection *c, double now) {
  if (c->ev_timer_time > 0 && now >= c->ev_timer_time) {
    double old_value = c->ev_timer_time;
    c->ev_timer_time = 0;
    mg_call(c, NULL, c->user_data, MG_EV_TIMER, &old_value);
  }
}

MG_INTERNAL size_t recv_avail_size(struct mg_connection *conn, size_t max) {
  size_t avail;
  if (conn->recv_mbuf_limit < conn->recv_mbuf.len) return 0;
  avail = conn->recv_mbuf_limit - conn->recv_mbuf.len;
  return avail > max ? max : avail;
}

static int mg_do_recv(struct mg_connection *nc);

int mg_if_poll(struct mg_connection *nc, double now) {
  if (nc->flags & MG_F_CLOSE_IMMEDIATELY) {
    mg_close_conn(nc);
    return 0;
  } else if (nc->flags & MG_F_SEND_AND_CLOSE) {
    if (nc->send_mbuf.len == 0) {
      nc->flags |= MG_F_CLOSE_IMMEDIATELY;
      mg_close_conn(nc);
      return 0;
    }
  } else if (nc->flags & MG_F_RECV_AND_CLOSE) {
    mg_close_conn(nc);
    return 0;
  }
#if MG_ENABLE_SSL
  if ((nc->flags & (MG_F_SSL | MG_F_LISTENING | MG_F_CONNECTING)) == MG_F_SSL) {
    /* SSL library may have data to be delivered to the app in its buffers,
     * drain them. */
    int recved = 0;
    do {
      if (nc->flags & (MG_F_WANT_READ | MG_F_WANT_WRITE)) break;
      if (recv_avail_size(nc, MG_TCP_IO_SIZE) <= 0) break;
      recved = mg_do_recv(nc);
    } while (recved > 0);
  }
#endif /* MG_ENABLE_SSL */
  mg_timer(nc, now);
  {
    time_t now_t = (time_t) now;
    mg_call(nc, NULL, nc->user_data, MG_EV_POLL, &now_t);
  }
  return 1;
}

void mg_destroy_conn(struct mg_connection *conn, int destroy_if) {
  if (conn->sock != INVALID_SOCKET) { /* Don't print timer-only conns */
    LOG(LL_DEBUG, ("%p 0x%lx %d", conn, conn->flags, destroy_if));
  }
  if (destroy_if) conn->iface->vtable->destroy_conn(conn);
  if (conn->proto_data != NULL && conn->proto_data_destructor != NULL) {
    conn->proto_data_destructor(conn->proto_data);
  }
#if MG_ENABLE_SSL
  mg_ssl_if_conn_free(conn);
#endif
  mbuf_free(&conn->recv_mbuf);
  mbuf_free(&conn->send_mbuf);

  memset(conn, 0, sizeof(*conn));
  MG_FREE(conn);
}

void mg_close_conn(struct mg_connection *conn) {
  /* See if there's any remaining data to deliver. Skip if user completely
   * throttled the connection there will be no progress anyway. */
  if (conn->sock != INVALID_SOCKET && mg_do_recv(conn) == -2) {
    /* Receive is throttled, wait. */
    conn->flags |= MG_F_RECV_AND_CLOSE;
    return;
  }
#if MG_ENABLE_SSL
  if (conn->flags & MG_F_SSL_HANDSHAKE_DONE) {
    mg_ssl_if_conn_close_notify(conn);
  }
#endif
  /*
   * Clearly mark the connection as going away (if not already).
   * Some net_if impls (LwIP) need this for cleanly handling half-dead conns.
   */
  conn->flags |= MG_F_CLOSE_IMMEDIATELY;
  mg_remove_conn(conn);
  conn->iface->vtable->destroy_conn(conn);
  mg_call(conn, NULL, conn->user_data, MG_EV_CLOSE, NULL);
  mg_destroy_conn(conn, 0 /* destroy_if */);
}

void mg_mgr_init(struct mg_mgr *m, void *user_data) {
  struct mg_mgr_init_opts opts;
  memset(&opts, 0, sizeof(opts));
  mg_mgr_init_opt(m, user_data, opts);
}

void mg_mgr_init_opt(struct mg_mgr *m, void *user_data,
                     struct mg_mgr_init_opts opts) {
  memset(m, 0, sizeof(*m));
#if MG_ENABLE_BROADCAST
  m->ctl[0] = m->ctl[1] = INVALID_SOCKET;
#endif
  m->user_data = user_data;

#ifdef _WIN32
  {
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
  }
#elif defined(__unix__)
  /* Ignore SIGPIPE signal, so if client cancels the request, it
   * won't kill the whole process. */
  signal(SIGPIPE, SIG_IGN);
#endif

  {
    int i;
    if (opts.num_ifaces == 0) {
      opts.num_ifaces = mg_num_ifaces;
      opts.ifaces = mg_ifaces;
    }
    if (opts.main_iface != NULL) {
      opts.ifaces[MG_MAIN_IFACE] = opts.main_iface;
    }
    m->num_ifaces = opts.num_ifaces;
    m->ifaces =
        (struct mg_iface **) MG_MALLOC(sizeof(*m->ifaces) * opts.num_ifaces);
    for (i = 0; i < opts.num_ifaces; i++) {
      m->ifaces[i] = mg_if_create_iface(opts.ifaces[i], m);
      m->ifaces[i]->vtable->init(m->ifaces[i]);
    }
  }
  if (opts.nameserver != NULL) {
    m->nameserver = strdup(opts.nameserver);
  }
  DBG(("=================================="));
  DBG(("init mgr=%p", m));
#if MG_ENABLE_SSL
  {
    static int init_done;
    if (!init_done) {
      mg_ssl_if_init();
      init_done++;
    }
  }
#endif
}

void mg_mgr_free(struct mg_mgr *m) {
  struct mg_connection *conn, *tmp_conn;

  DBG(("%p", m));
  if (m == NULL) return;
  /* Do one last poll, see https://github.com/cesanta/mongoose/issues/286 */
  mg_mgr_poll(m, 0);

#if MG_ENABLE_BROADCAST
  if (m->ctl[0] != INVALID_SOCKET) closesocket(m->ctl[0]);
  if (m->ctl[1] != INVALID_SOCKET) closesocket(m->ctl[1]);
  m->ctl[0] = m->ctl[1] = INVALID_SOCKET;
#endif

  for (conn = m->active_connections; conn != NULL; conn = tmp_conn) {
    tmp_conn = conn->next;
    conn->flags |= MG_F_CLOSE_IMMEDIATELY;
    mg_close_conn(conn);
  }

  {
    int i;
    for (i = 0; i < m->num_ifaces; i++) {
      m->ifaces[i]->vtable->free(m->ifaces[i]);
      MG_FREE(m->ifaces[i]);
    }
    MG_FREE(m->ifaces);
  }

  MG_FREE((char *) m->nameserver);
}

int mg_mgr_poll(struct mg_mgr *m, int timeout_ms) {
  int i, num_calls_before = m->num_calls;

  for (i = 0; i < m->num_ifaces; i++) {
    m->ifaces[i]->vtable->poll(m->ifaces[i], timeout_ms);
  }

  return (m->num_calls - num_calls_before);
}

int mg_vprintf(struct mg_connection *nc, const char *fmt, va_list ap) {
  char mem[MG_VPRINTF_BUFFER_SIZE], *buf = mem;
  int len;

  if ((len = mg_avprintf(&buf, sizeof(mem), fmt, ap)) > 0) {
    mg_send(nc, buf, len);
  }
  if (buf != mem && buf != NULL) {
    MG_FREE(buf); /* LCOV_EXCL_LINE */
  }               /* LCOV_EXCL_LINE */

  return len;
}

int mg_printf(struct mg_connection *conn, const char *fmt, ...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  len = mg_vprintf(conn, fmt, ap);
  va_end(ap);
  return len;
}

#if MG_ENABLE_SYNC_RESOLVER
/* TODO(lsm): use non-blocking resolver */
static int mg_resolve2(const char *host, struct in_addr *ina) {
#if MG_ENABLE_GETADDRINFO
  int rv = 0;
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_in *h = NULL;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(host, NULL, NULL, &servinfo)) != 0) {
    DBG(("getaddrinfo(%s) failed: %s", host, strerror(mg_get_errno())));
    return 0;
  }
  for (p = servinfo; p != NULL; p = p->ai_next) {
    memcpy(&h, &p->ai_addr, sizeof(h));
    memcpy(ina, &h->sin_addr, sizeof(*ina));
  }
  freeaddrinfo(servinfo);
  return 1;
#else
  struct hostent *he;
  if ((he = gethostbyname(host)) == NULL) {
    DBG(("gethostbyname(%s) failed: %s", host, strerror(mg_get_errno())));
  } else {
    memcpy(ina, he->h_addr_list[0], sizeof(*ina));
    return 1;
  }
  return 0;
#endif /* MG_ENABLE_GETADDRINFO */
}

int mg_resolve(const char *host, char *buf, size_t n) {
  struct in_addr ad;
  return mg_resolve2(host, &ad) ? snprintf(buf, n, "%s", inet_ntoa(ad)) : 0;
}
#endif /* MG_ENABLE_SYNC_RESOLVER */

MG_INTERNAL struct mg_connection *mg_create_connection_base(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts) {
  struct mg_connection *conn;

  if ((conn = (struct mg_connection *) MG_CALLOC(1, sizeof(*conn))) != NULL) {
    conn->sock = INVALID_SOCKET;
    conn->handler = callback;
    conn->mgr = mgr;
    conn->last_io_time = (time_t) mg_time();
    conn->iface =
        (opts.iface != NULL ? opts.iface : mgr->ifaces[MG_MAIN_IFACE]);
    conn->flags = opts.flags & _MG_ALLOWED_CONNECT_FLAGS_MASK;
    conn->user_data = opts.user_data;
    /*
     * SIZE_MAX is defined as a long long constant in
     * system headers on some platforms and so it
     * doesn't compile with pedantic ansi flags.
     */
    conn->recv_mbuf_limit = ~0;
  } else {
    MG_SET_PTRPTR(opts.error_string, "failed to create connection");
  }

  return conn;
}

MG_INTERNAL struct mg_connection *mg_create_connection(
    struct mg_mgr *mgr, mg_event_handler_t callback,
    struct mg_add_sock_opts opts) {
  struct mg_connection *conn = mg_create_connection_base(mgr, callback, opts);

  if (conn != NULL && !conn->iface->vtable->create_conn(conn)) {
    MG_FREE(conn);
    conn = NULL;
  }
  if (conn == NULL) {
    MG_SET_PTRPTR(opts.error_string, "failed to init connection");
  }

  return conn;
}

/*
 * Address format: [PROTO://][HOST]:PORT
 *
 * HOST could be IPv4/IPv6 address or a host name.
 * `host` is a destination buffer to hold parsed HOST part. Should be at least
 * MG_MAX_HOST_LEN bytes long.
 * `proto` is a returned socket type, either SOCK_STREAM or SOCK_DGRAM
 *
 * Return:
 *   -1   on parse error
 *    0   if HOST needs DNS lookup
 *   >0   length of the address string
 */
MG_INTERNAL int mg_parse_address(const char *str, union socket_address *sa,
                                 int *proto, char *host, size_t host_len) {
  unsigned int a, b, c, d, port = 0;
  int ch, len = 0;
#if MG_ENABLE_IPV6
  char buf[100];
#endif

  /*
   * MacOS needs that. If we do not zero it, subsequent bind() will fail.
   * Also, all-zeroes in the socket address means binding to all addresses
   * for both IPv4 and IPv6 (INADDR_ANY and IN6ADDR_ANY_INIT).
   */
  memset(sa, 0, sizeof(*sa));
  sa->sin.sin_family = AF_INET;

  *proto = SOCK_STREAM;

  if (strncmp(str, "udp://", 6) == 0) {
    str += 6;
    *proto = SOCK_DGRAM;
  } else if (strncmp(str, "tcp://", 6) == 0) {
    str += 6;
  }

  if (sscanf(str, "%u.%u.%u.%u:%u%n", &a, &b, &c, &d, &port, &len) == 5) {
    /* Bind to a specific IPv4 address, e.g. 192.168.1.5:8080 */
    sa->sin.sin_addr.s_addr =
        htonl(((uint32_t) a << 24) | ((uint32_t) b << 16) | c << 8 | d);
    sa->sin.sin_port = htons((uint16_t) port);
#if MG_ENABLE_IPV6
  } else if (sscanf(str, "[%99[^]]]:%u%n", buf, &port, &len) == 2 &&
             inet_pton(AF_INET6, buf, &sa->sin6.sin6_addr)) {
    /* IPv6 address, e.g. [3ffe:2a00:100:7031::1]:8080 */
    sa->sin6.sin6_family = AF_INET6;
    sa->sin.sin_port = htons((uint16_t) port);
#endif
#if MG_ENABLE_ASYNC_RESOLVER
  } else if (strlen(str) < host_len &&
             sscanf(str, "%[^ :]:%u%n", host, &port, &len) == 2) {
    sa->sin.sin_port = htons((uint16_t) port);
    if (mg_resolve_from_hosts_file(host, sa) != 0) {
      /*
       * if resolving from hosts file failed and the host
       * we are trying to resolve is `localhost` - we should
       * try to resolve it using `gethostbyname` and do not try
       * to resolve it via DNS server if gethostbyname has failed too
       */
      if (mg_ncasecmp(host, "localhost", 9) != 0) {
        return 0;
      }

#if MG_ENABLE_SYNC_RESOLVER
      if (!mg_resolve2(host, &sa->sin.sin_addr)) {
        return -1;
      }
#else
      return -1;
#endif
    }
#endif
  } else if (sscanf(str, ":%u%n", &port, &len) == 1 ||
             sscanf(str, "%u%n", &port, &len) == 1) {
    /* If only port is specified, bind to IPv4, INADDR_ANY */
    sa->sin.sin_port = htons((uint16_t) port);
  } else {
    return -1;
  }

  /* Required for MG_ENABLE_ASYNC_RESOLVER=0 */
  (void) host;
  (void) host_len;

  ch = str[len]; /* Character that follows the address */
  return port < 0xffffUL && (ch == '\0' || ch == ',' || isspace(ch)) ? len : -1;
}

#if MG_ENABLE_SSL
MG_INTERNAL void mg_ssl_handshake(struct mg_connection *nc) {
  int err = 0;
  int server_side = (nc->listener != NULL);
  enum mg_ssl_if_result res;
  if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) return;
  res = mg_ssl_if_handshake(nc);

  if (res == MG_SSL_OK) {
    nc->flags |= MG_F_SSL_HANDSHAKE_DONE;
    nc->flags &= ~(MG_F_WANT_READ | MG_F_WANT_WRITE);
    if (server_side) {
      mg_call(nc, NULL, nc->user_data, MG_EV_ACCEPT, &nc->sa);
    } else {
      mg_call(nc, NULL, nc->user_data, MG_EV_CONNECT, &err);
    }
  } else if (res == MG_SSL_WANT_READ) {
    nc->flags |= MG_F_WANT_READ;
  } else if (res == MG_SSL_WANT_WRITE) {
    nc->flags |= MG_F_WANT_WRITE;
  } else {
    if (!server_side) {
      err = res;
      mg_call(nc, NULL, nc->user_data, MG_EV_CONNECT, &err);
    }
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}
#endif /* MG_ENABLE_SSL */

struct mg_connection *mg_if_accept_new_conn(struct mg_connection *lc) {
  struct mg_add_sock_opts opts;
  struct mg_connection *nc;
  memset(&opts, 0, sizeof(opts));
  nc = mg_create_connection(lc->mgr, lc->handler, opts);
  if (nc == NULL) return NULL;
  nc->listener = lc;
  nc->proto_handler = lc->proto_handler;
  nc->user_data = lc->user_data;
  nc->recv_mbuf_limit = lc->recv_mbuf_limit;
  nc->iface = lc->iface;
  if (lc->flags & MG_F_SSL) nc->flags |= MG_F_SSL;
  mg_add_conn(nc->mgr, nc);
  LOG(LL_DEBUG, ("%p %p %d %d", lc, nc, nc->sock, (int) nc->flags));
  return nc;
}

void mg_if_accept_tcp_cb(struct mg_connection *nc, union socket_address *sa,
                         size_t sa_len) {
  LOG(LL_DEBUG, ("%p %s://%s:%hu", nc, (nc->flags & MG_F_UDP ? "udp" : "tcp"),
                 inet_ntoa(sa->sin.sin_addr), ntohs(sa->sin.sin_port)));
  nc->sa = *sa;
#if MG_ENABLE_SSL
  if (nc->listener->flags & MG_F_SSL) {
    nc->flags |= MG_F_SSL;
    if (mg_ssl_if_conn_accept(nc, nc->listener) == MG_SSL_OK) {
      mg_ssl_handshake(nc);
    } else {
      mg_close_conn(nc);
    }
  } else
#endif
  {
    mg_call(nc, NULL, nc->user_data, MG_EV_ACCEPT, &nc->sa);
  }
  (void) sa_len;
}

void mg_send(struct mg_connection *nc, const void *buf, int len) {
  nc->last_io_time = (time_t) mg_time();
  mbuf_append(&nc->send_mbuf, buf, len);
}

static int mg_recv_tcp(struct mg_connection *nc, char *buf, size_t len);
static int mg_recv_udp(struct mg_connection *nc, char *buf, size_t len);

static int mg_do_recv(struct mg_connection *nc) {
  int res = 0;
  char *buf = NULL;
  size_t len = (nc->flags & MG_F_UDP ? MG_UDP_IO_SIZE : MG_TCP_IO_SIZE);
  if ((nc->flags & (MG_F_CLOSE_IMMEDIATELY | MG_F_CONNECTING)) ||
      ((nc->flags & MG_F_LISTENING) && !(nc->flags & MG_F_UDP))) {
    return -1;
  }
  do {
    len = recv_avail_size(nc, len);
    if (len == 0) {
      res = -2;
      break;
    }
    if (nc->recv_mbuf.size < nc->recv_mbuf.len + len) {
      mbuf_resize(&nc->recv_mbuf, nc->recv_mbuf.len + len);
    }
    buf = nc->recv_mbuf.buf + nc->recv_mbuf.len;
    len = nc->recv_mbuf.size - nc->recv_mbuf.len;
    if (nc->flags & MG_F_UDP) {
      res = mg_recv_udp(nc, buf, len);
    } else {
      res = mg_recv_tcp(nc, buf, len);
    }
  } while (res > 0 && !(nc->flags & (MG_F_CLOSE_IMMEDIATELY | MG_F_UDP)));
  return res;
}

void mg_if_can_recv_cb(struct mg_connection *nc) {
  mg_do_recv(nc);
}

static int mg_recv_tcp(struct mg_connection *nc, char *buf, size_t len) {
  int n = 0;
#if MG_ENABLE_SSL
  if (nc->flags & MG_F_SSL) {
    if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
      n = mg_ssl_if_read(nc, buf, len);
      DBG(("%p <- %d bytes (SSL)", nc, n));
      if (n < 0) {
        if (n == MG_SSL_WANT_READ) {
          nc->flags |= MG_F_WANT_READ;
          n = 0;
        } else {
          nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
      } else if (n > 0) {
        nc->flags &= ~MG_F_WANT_READ;
      }
    } else {
      mg_ssl_handshake(nc);
    }
  } else
#endif
  {
    n = nc->iface->vtable->tcp_recv(nc, buf, len);
    DBG(("%p <- %d bytes", nc, n));
  }
  if (n > 0) {
    nc->recv_mbuf.len += n;
    nc->last_io_time = (time_t) mg_time();
#if !defined(NO_LIBC) && MG_ENABLE_HEXDUMP
    if (nc->mgr && nc->mgr->hexdump_file != NULL) {
      mg_hexdump_connection(nc, nc->mgr->hexdump_file, buf, n, MG_EV_RECV);
    }
#endif
    mbuf_trim(&nc->recv_mbuf);
    mg_call(nc, NULL, nc->user_data, MG_EV_RECV, &n);
  } else if (n < 0) {
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
  mbuf_trim(&nc->recv_mbuf);
  return n;
}

static int mg_recv_udp(struct mg_connection *nc, char *buf, size_t len) {
  int n = 0;
  struct mg_connection *lc = nc;
  union socket_address sa;
  size_t sa_len = sizeof(sa);
  n = nc->iface->vtable->udp_recv(lc, buf, len, &sa, &sa_len);
  if (n < 0) {
    lc->flags |= MG_F_CLOSE_IMMEDIATELY;
    goto out;
  }
  if (nc->flags & MG_F_LISTENING) {
    /*
     * Do we have an existing connection for this source?
     * This is very inefficient for long connection lists.
     */
    lc = nc;
    for (nc = mg_next(lc->mgr, NULL); nc != NULL; nc = mg_next(lc->mgr, nc)) {
      if (memcmp(&nc->sa.sa, &sa.sa, sa_len) == 0 && nc->listener == lc) {
        break;
      }
    }
    if (nc == NULL) {
      struct mg_add_sock_opts opts;
      memset(&opts, 0, sizeof(opts));
      /* Create fake connection w/out sock initialization */
      nc = mg_create_connection_base(lc->mgr, lc->handler, opts);
      if (nc != NULL) {
        nc->sock = lc->sock;
        nc->listener = lc;
        nc->sa = sa;
        nc->proto_handler = lc->proto_handler;
        nc->user_data = lc->user_data;
        nc->recv_mbuf_limit = lc->recv_mbuf_limit;
        nc->flags = MG_F_UDP;
        /*
         * Long-lived UDP "connections" i.e. interactions that involve more
         * than one request and response are rare, most are transactional:
         * response is sent and the "connection" is closed. Or - should be.
         * But users (including ourselves) tend to forget about that part,
         * because UDP is connectionless and one does not think about
         * processing a UDP request as handling a connection that needs to be
         * closed. Thus, we begin with SEND_AND_CLOSE flag set, which should
         * be a reasonable default for most use cases, but it is possible to
         * turn it off the connection should be kept alive after processing.
         */
        nc->flags |= MG_F_SEND_AND_CLOSE;
        mg_add_conn(lc->mgr, nc);
        mg_call(nc, NULL, nc->user_data, MG_EV_ACCEPT, &nc->sa);
      }
    }
  }
  if (nc != NULL) {
    DBG(("%p <- %d bytes from %s:%d", nc, n, inet_ntoa(nc->sa.sin.sin_addr),
         ntohs(nc->sa.sin.sin_port)));
    if (nc == lc) {
      nc->recv_mbuf.len += n;
    } else {
      mbuf_append(&nc->recv_mbuf, buf, n);
    }
    mbuf_trim(&lc->recv_mbuf);
    lc->last_io_time = nc->last_io_time = (time_t) mg_time();
#if !defined(NO_LIBC) && MG_ENABLE_HEXDUMP
    if (nc->mgr && nc->mgr->hexdump_file != NULL) {
      mg_hexdump_connection(nc, nc->mgr->hexdump_file, buf, n, MG_EV_RECV);
    }
#endif
    if (n != 0) {
      mg_call(nc, NULL, nc->user_data, MG_EV_RECV, &n);
    }
  }

out:
  mbuf_free(&lc->recv_mbuf);
  return n;
}

void mg_if_can_send_cb(struct mg_connection *nc) {
  int n = 0;
  const char *buf = nc->send_mbuf.buf;
  size_t len = nc->send_mbuf.len;

  if (nc->flags & (MG_F_CLOSE_IMMEDIATELY | MG_F_CONNECTING)) {
    return;
  }
  if (!(nc->flags & MG_F_UDP)) {
    if (nc->flags & MG_F_LISTENING) return;
    if (len > MG_TCP_IO_SIZE) len = MG_TCP_IO_SIZE;
  }
#if MG_ENABLE_SSL
  if (nc->flags & MG_F_SSL) {
    if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
      if (len > 0) {
        n = mg_ssl_if_write(nc, buf, len);
        DBG(("%p -> %d bytes (SSL)", nc, n));
      }
      if (n < 0) {
        if (n == MG_SSL_WANT_WRITE) {
          nc->flags |= MG_F_WANT_WRITE;
          n = 0;
        } else {
          nc->flags |= MG_F_CLOSE_IMMEDIATELY;
        }
      } else {
        nc->flags &= ~MG_F_WANT_WRITE;
      }
    } else {
      mg_ssl_handshake(nc);
    }
  } else
#endif
      if (len > 0) {
    if (nc->flags & MG_F_UDP) {
      n = nc->iface->vtable->udp_send(nc, buf, len);
    } else {
      n = nc->iface->vtable->tcp_send(nc, buf, len);
    }
    DBG(("%p -> %d bytes", nc, n));
  }

#if !defined(NO_LIBC) && MG_ENABLE_HEXDUMP
  if (n > 0 && nc->mgr && nc->mgr->hexdump_file != NULL) {
    mg_hexdump_connection(nc, nc->mgr->hexdump_file, buf, n, MG_EV_SEND);
  }
#endif
  if (n < 0) {
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  } else if (n > 0) {
    nc->last_io_time = (time_t) mg_time();
    mbuf_remove(&nc->send_mbuf, n);
    mbuf_trim(&nc->send_mbuf);
  }
  if (n != 0) mg_call(nc, NULL, nc->user_data, MG_EV_SEND, &n);
}

/*
 * Schedules an async connect for a resolved address and proto.
 * Called from two places: `mg_connect_opt()` and from async resolver.
 * When called from the async resolver, it must trigger `MG_EV_CONNECT` event
 * with a failure flag to indicate connection failure.
 */
MG_INTERNAL struct mg_connection *mg_do_connect(struct mg_connection *nc,
                                                int proto,
                                                union socket_address *sa) {
  LOG(LL_DEBUG, ("%p %s://%s:%hu", nc, proto == SOCK_DGRAM ? "udp" : "tcp",
                 inet_ntoa(sa->sin.sin_addr), ntohs(sa->sin.sin_port)));

  nc->flags |= MG_F_CONNECTING;
  if (proto == SOCK_DGRAM) {
    nc->iface->vtable->connect_udp(nc);
  } else {
    nc->iface->vtable->connect_tcp(nc, sa);
  }
  mg_add_conn(nc->mgr, nc);
  return nc;
}

void mg_if_connect_cb(struct mg_connection *nc, int err) {
  LOG(LL_DEBUG,
      ("%p %s://%s:%hu -> %d", nc, (nc->flags & MG_F_UDP ? "udp" : "tcp"),
       inet_ntoa(nc->sa.sin.sin_addr), ntohs(nc->sa.sin.sin_port), err));
  nc->flags &= ~MG_F_CONNECTING;
  if (err != 0) {
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
#if MG_ENABLE_SSL
  if (err == 0 && (nc->flags & MG_F_SSL)) {
    mg_ssl_handshake(nc);
  } else
#endif
  {
    mg_call(nc, NULL, nc->user_data, MG_EV_CONNECT, &err);
  }
}

#if MG_ENABLE_ASYNC_RESOLVER
/*
 * Callback for the async resolver on mg_connect_opt() call.
 * Main task of this function is to trigger MG_EV_CONNECT event with
 *    either failure (and dealloc the connection)
 *    or success (and proceed with connect()
 */
static void resolve_cb(struct mg_dns_message *msg, void *data,
                       enum mg_resolve_err e) {
  struct mg_connection *nc = (struct mg_connection *) data;
  int i;
  int failure = -1;

  nc->flags &= ~MG_F_RESOLVING;
  if (msg != NULL) {
    /*
     * Take the first DNS A answer and run...
     */
    for (i = 0; i < msg->num_answers; i++) {
      if (msg->answers[i].rtype == MG_DNS_A_RECORD) {
        /*
         * Async resolver guarantees that there is at least one answer.
         * TODO(lsm): handle IPv6 answers too
         */
        mg_dns_parse_record_data(msg, &msg->answers[i], &nc->sa.sin.sin_addr,
                                 4);
        mg_do_connect(nc, nc->flags & MG_F_UDP ? SOCK_DGRAM : SOCK_STREAM,
                      &nc->sa);
        return;
      }
    }
  }

  if (e == MG_RESOLVE_TIMEOUT) {
    double now = mg_time();
    mg_call(nc, NULL, nc->user_data, MG_EV_TIMER, &now);
  }

  /*
   * If we get there was no MG_DNS_A_RECORD in the answer
   */
  mg_call(nc, NULL, nc->user_data, MG_EV_CONNECT, &failure);
  mg_call(nc, NULL, nc->user_data, MG_EV_CLOSE, NULL);
  mg_destroy_conn(nc, 1 /* destroy_if */);
}
#endif

struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                 MG_CB(mg_event_handler_t callback,
                                       void *user_data)) {
  struct mg_connect_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_connect_opt(mgr, address, MG_CB(callback, user_data), opts);
}

void mg_ev_handler_empty(struct mg_connection *c, int ev,
                         void *ev_data MG_UD_ARG(void *user_data)) {
  (void) c;
  (void) ev;
  (void) ev_data;
#if MG_ENABLE_CALLBACK_USERDATA
  (void) user_data;
#endif
}

struct mg_connection *mg_connect_opt(struct mg_mgr *mgr, const char *address,
                                     MG_CB(mg_event_handler_t callback,
                                           void *user_data),
                                     struct mg_connect_opts opts) {
  struct mg_connection *nc = NULL;
  int proto, rc;
  struct mg_add_sock_opts add_sock_opts;
  char host[MG_MAX_HOST_LEN];

  MG_COPY_COMMON_CONNECTION_OPTIONS(&add_sock_opts, &opts);

  if (callback == NULL) callback = mg_ev_handler_empty;

  if ((nc = mg_create_connection(mgr, callback, add_sock_opts)) == NULL) {
    return NULL;
  }

  if ((rc = mg_parse_address(address, &nc->sa, &proto, host, sizeof(host))) <
      0) {
    /* Address is malformed */
    MG_SET_PTRPTR(opts.error_string, "cannot parse address");
    mg_destroy_conn(nc, 1 /* destroy_if */);
    return NULL;
  }

  nc->flags |= opts.flags & _MG_ALLOWED_CONNECT_FLAGS_MASK;
  nc->flags |= (proto == SOCK_DGRAM) ? MG_F_UDP : 0;
#if MG_ENABLE_CALLBACK_USERDATA
  nc->user_data = user_data;
#else
  nc->user_data = opts.user_data;
#endif

#if MG_ENABLE_SSL
  LOG(LL_DEBUG,
      ("%p %s %s,%s,%s", nc, address, (opts.ssl_cert ? opts.ssl_cert : "-"),
       (opts.ssl_key ? opts.ssl_key : "-"),
       (opts.ssl_ca_cert ? opts.ssl_ca_cert : "-")));

  if (opts.ssl_cert != NULL || opts.ssl_ca_cert != NULL ||
      opts.ssl_psk_identity != NULL) {
    const char *err_msg = NULL;
    struct mg_ssl_if_conn_params params;
    if (nc->flags & MG_F_UDP) {
      MG_SET_PTRPTR(opts.error_string, "SSL for UDP is not supported");
      mg_destroy_conn(nc, 1 /* destroy_if */);
      return NULL;
    }
    memset(&params, 0, sizeof(params));
    params.cert = opts.ssl_cert;
    params.key = opts.ssl_key;
    params.ca_cert = opts.ssl_ca_cert;
    params.cipher_suites = opts.ssl_cipher_suites;
    params.psk_identity = opts.ssl_psk_identity;
    params.psk_key = opts.ssl_psk_key;
    if (opts.ssl_ca_cert != NULL) {
      if (opts.ssl_server_name != NULL) {
        if (strcmp(opts.ssl_server_name, "*") != 0) {
          params.server_name = opts.ssl_server_name;
        }
      } else if (rc == 0) { /* If it's a DNS name, use host. */
        params.server_name = host;
      }
    }
    if (mg_ssl_if_conn_init(nc, &params, &err_msg) != MG_SSL_OK) {
      MG_SET_PTRPTR(opts.error_string, err_msg);
      mg_destroy_conn(nc, 1 /* destroy_if */);
      return NULL;
    }
    nc->flags |= MG_F_SSL;
  }
#endif /* MG_ENABLE_SSL */

  if (rc == 0) {
#if MG_ENABLE_ASYNC_RESOLVER
    /*
     * DNS resolution is required for host.
     * mg_parse_address() fills port in nc->sa, which we pass to resolve_cb()
     */
    struct mg_connection *dns_conn = NULL;
    struct mg_resolve_async_opts o;
    memset(&o, 0, sizeof(o));
    o.dns_conn = &dns_conn;
    o.nameserver = opts.nameserver;
    if (mg_resolve_async_opt(nc->mgr, host, MG_DNS_A_RECORD, resolve_cb, nc,
                             o) != 0) {
      MG_SET_PTRPTR(opts.error_string, "cannot schedule DNS lookup");
      mg_destroy_conn(nc, 1 /* destroy_if */);
      return NULL;
    }
    nc->priv_2 = dns_conn;
    nc->flags |= MG_F_RESOLVING;
    return nc;
#else
    MG_SET_PTRPTR(opts.error_string, "Resolver is disabled");
    mg_destroy_conn(nc, 1 /* destroy_if */);
    return NULL;
#endif
  } else {
    /* Address is parsed and resolved to IP. proceed with connect() */
    return mg_do_connect(nc, proto, &nc->sa);
  }
}

struct mg_connection *mg_bind(struct mg_mgr *srv, const char *address,
                              MG_CB(mg_event_handler_t event_handler,
                                    void *user_data)) {
  struct mg_bind_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_bind_opt(srv, address, MG_CB(event_handler, user_data), opts);
}

struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                  MG_CB(mg_event_handler_t callback,
                                        void *user_data),
                                  struct mg_bind_opts opts) {
  union socket_address sa;
  struct mg_connection *nc = NULL;
  int proto, rc;
  struct mg_add_sock_opts add_sock_opts;
  char host[MG_MAX_HOST_LEN];

#if MG_ENABLE_CALLBACK_USERDATA
  opts.user_data = user_data;
#endif

  if (callback == NULL) callback = mg_ev_handler_empty;

  MG_COPY_COMMON_CONNECTION_OPTIONS(&add_sock_opts, &opts);

  if (mg_parse_address(address, &sa, &proto, host, sizeof(host)) <= 0) {
    MG_SET_PTRPTR(opts.error_string, "cannot parse address");
    return NULL;
  }

  nc = mg_create_connection(mgr, callback, add_sock_opts);
  if (nc == NULL) {
    return NULL;
  }

  nc->sa = sa;
  nc->flags |= MG_F_LISTENING;
  if (proto == SOCK_DGRAM) nc->flags |= MG_F_UDP;

#if MG_ENABLE_SSL
  DBG(("%p %s %s,%s,%s", nc, address, (opts.ssl_cert ? opts.ssl_cert : "-"),
       (opts.ssl_key ? opts.ssl_key : "-"),
       (opts.ssl_ca_cert ? opts.ssl_ca_cert : "-")));

  if (opts.ssl_cert != NULL || opts.ssl_ca_cert != NULL) {
    const char *err_msg = NULL;
    struct mg_ssl_if_conn_params params;
    if (nc->flags & MG_F_UDP) {
      MG_SET_PTRPTR(opts.error_string, "SSL for UDP is not supported");
      mg_destroy_conn(nc, 1 /* destroy_if */);
      return NULL;
    }
    memset(&params, 0, sizeof(params));
    params.cert = opts.ssl_cert;
    params.key = opts.ssl_key;
    params.ca_cert = opts.ssl_ca_cert;
    params.cipher_suites = opts.ssl_cipher_suites;
    if (mg_ssl_if_conn_init(nc, &params, &err_msg) != MG_SSL_OK) {
      MG_SET_PTRPTR(opts.error_string, err_msg);
      mg_destroy_conn(nc, 1 /* destroy_if */);
      return NULL;
    }
    nc->flags |= MG_F_SSL;
  }
#endif /* MG_ENABLE_SSL */

  if (nc->flags & MG_F_UDP) {
    rc = nc->iface->vtable->listen_udp(nc, &nc->sa);
  } else {
    rc = nc->iface->vtable->listen_tcp(nc, &nc->sa);
  }
  if (rc != 0) {
    DBG(("Failed to open listener: %d", rc));
    MG_SET_PTRPTR(opts.error_string, "failed to open listener");
    mg_destroy_conn(nc, 1 /* destroy_if */);
    return NULL;
  }
  mg_add_conn(nc->mgr, nc);

  return nc;
}

struct mg_connection *mg_next(struct mg_mgr *s, struct mg_connection *conn) {
  return conn == NULL ? s->active_connections : conn->next;
}

#if MG_ENABLE_BROADCAST
void mg_broadcast(struct mg_mgr *mgr, mg_event_handler_t cb, void *data,
                  size_t len) {
  struct ctl_msg ctl_msg;

  /*
   * Mongoose manager has a socketpair, `struct mg_mgr::ctl`,
   * where `mg_broadcast()` pushes the message.
   * `mg_mgr_poll()` wakes up, reads a message from the socket pair, and calls
   * specified callback for each connection. Thus the callback function executes
   * in event manager thread.
   */
  if (mgr->ctl[0] != INVALID_SOCKET && data != NULL &&
      len < sizeof(ctl_msg.message)) {
    size_t dummy;

    ctl_msg.callback = cb;
    memcpy(ctl_msg.message, data, len);
    dummy = MG_SEND_FUNC(mgr->ctl[0], (char *) &ctl_msg,
                         offsetof(struct ctl_msg, message) + len, 0);
    dummy = MG_RECV_FUNC(mgr->ctl[0], (char *) &len, 1, 0);
    (void) dummy; /* https://gcc.gnu.org/bugzilla/show_bug.cgi?id=25509 */
  }
}
#endif /* MG_ENABLE_BROADCAST */

static int isbyte(int n) {
  return n >= 0 && n <= 255;
}

static int parse_net(const char *spec, uint32_t *net, uint32_t *mask) {
  int n, a, b, c, d, slash = 32, len = 0;

  if ((sscanf(spec, "%d.%d.%d.%d/%d%n", &a, &b, &c, &d, &slash, &n) == 5 ||
       sscanf(spec, "%d.%d.%d.%d%n", &a, &b, &c, &d, &n) == 4) &&
      isbyte(a) && isbyte(b) && isbyte(c) && isbyte(d) && slash >= 0 &&
      slash < 33) {
    len = n;
    *net =
        ((uint32_t) a << 24) | ((uint32_t) b << 16) | ((uint32_t) c << 8) | d;
    *mask = slash ? 0xffffffffU << (32 - slash) : 0;
  }

  return len;
}

int mg_check_ip_acl(const char *acl, uint32_t remote_ip) {
  int allowed, flag;
  uint32_t net, mask;
  struct mg_str vec;

  /* If any ACL is set, deny by default */
  allowed = (acl == NULL || *acl == '\0') ? '+' : '-';

  while ((acl = mg_next_comma_list_entry(acl, &vec, NULL)) != NULL) {
    flag = vec.p[0];
    if ((flag != '+' && flag != '-') ||
        parse_net(&vec.p[1], &net, &mask) == 0) {
      return -1;
    }

    if (net == (remote_ip & mask)) {
      allowed = flag;
    }
  }

  DBG(("%08x %c", (unsigned int) remote_ip, allowed));
  return allowed == '+';
}

/* Move data from one connection to another */
void mg_forward(struct mg_connection *from, struct mg_connection *to) {
  mg_send(to, from->recv_mbuf.buf, from->recv_mbuf.len);
  mbuf_remove(&from->recv_mbuf, from->recv_mbuf.len);
}

double mg_set_timer(struct mg_connection *c, double timestamp) {
  double result = c->ev_timer_time;
  c->ev_timer_time = timestamp;
  /*
   * If this connection is resolving, it's not in the list of active
   * connections, so not processed yet. It has a DNS resolver connection
   * linked to it. Set up a timer for the DNS connection.
   */
  DBG(("%p %p %d -> %lu", c, c->priv_2, (c->flags & MG_F_RESOLVING ? 1 : 0),
       (unsigned long) timestamp));
  if ((c->flags & MG_F_RESOLVING) && c->priv_2 != NULL) {
    mg_set_timer((struct mg_connection *) c->priv_2, timestamp);
  }
  return result;
}

void mg_sock_set(struct mg_connection *nc, sock_t sock) {
  if (sock != INVALID_SOCKET) {
    nc->iface->vtable->sock_set(nc, sock);
  }
}

void mg_if_get_conn_addr(struct mg_connection *nc, int remote,
                         union socket_address *sa) {
  nc->iface->vtable->get_conn_addr(nc, remote, sa);
}

struct mg_connection *mg_add_sock_opt(struct mg_mgr *s, sock_t sock,
                                      MG_CB(mg_event_handler_t callback,
                                            void *user_data),
                                      struct mg_add_sock_opts opts) {
#if MG_ENABLE_CALLBACK_USERDATA
  opts.user_data = user_data;
#endif

  struct mg_connection *nc = mg_create_connection_base(s, callback, opts);
  if (nc != NULL) {
    mg_sock_set(nc, sock);
    mg_add_conn(nc->mgr, nc);
  }
  return nc;
}

struct mg_connection *mg_add_sock(struct mg_mgr *s, sock_t sock,
                                  MG_CB(mg_event_handler_t callback,
                                        void *user_data)) {
  struct mg_add_sock_opts opts;
  memset(&opts, 0, sizeof(opts));
  return mg_add_sock_opt(s, sock, MG_CB(callback, user_data), opts);
}

double mg_time(void) {
  return cs_time();
}
