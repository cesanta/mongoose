#include "dns.h"
#include "event.h"
#include "log.h"
#include "net.h"
#include "private.h"
#include "str.h"
#include "timer.h"
#include "tls.h"
#include "url.h"
#include "util.h"

#if MG_ENABLE_SOCKET
#if defined(_WIN32) && MG_ENABLE_WINSOCK
#define MG_SOCK_ERRNO WSAGetLastError()
#ifndef SO_EXCLUSIVEADDRUSE
#define SO_EXCLUSIVEADDRUSE ((int) (~SO_REUSEADDR))
#endif
#elif MG_ARCH == MG_ARCH_FREERTOS_TCP
#define MG_SOCK_ERRNO errno
typedef Socket_t SOCKET;
#define INVALID_SOCKET FREERTOS_INVALID_SOCKET
#else
#define MG_SOCK_ERRNO errno
#ifndef closesocket
#define closesocket(x) close(x)
#endif
#define INVALID_SOCKET (-1)
typedef int SOCKET;
#endif

union _su {
  SOCKET s;
  void *ptr;
};
#define FD(c_) ptr2sock((c_)->fd)

static SOCKET ptr2sock(void *ptr) {
  union _su u = {0};
  u.ptr = ptr;
  return u.s;
}

static void *sock2ptr(SOCKET s) {
  union _su u = {s};
  return u.ptr;
}

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

static bool mg_sock_would_block(void) {
  int err = MG_SOCK_ERRNO;
  return err == EINPROGRESS || err == EWOULDBLOCK
#ifndef WINCE
         || err == EAGAIN || err == EINTR
#endif
#if defined(_WIN32) && MG_ENABLE_WINSOCK
         || err == WSAEINTR || err == WSAEWOULDBLOCK
#endif
      ;
}

static struct mg_connection *alloc_conn(struct mg_mgr *mgr, bool is_client,
                                        SOCKET fd) {
  struct mg_connection *c = (struct mg_connection *) calloc(1, sizeof(*c));
  if (c != NULL) {
    c->is_client = is_client;
    c->fd = sock2ptr(fd);
    c->mgr = mgr;
    c->id = ++mgr->nextid;
  }
  return c;
}

static long mg_sock_send(struct mg_connection *c, const void *buf, size_t len) {
  long n = 0;
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
  return n == 0 ? -1 : n < 0 && mg_sock_would_block() ? 0 : n;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  return c->is_udp ? mg_sock_send(c, buf, len) > 0
                   : mg_iobuf_append(&c->send, buf, len, MG_IO_SIZE);
}

static void mg_set_non_blocking_mode(SOCKET fd) {
#if defined(_WIN32) && MG_ENABLE_WINSOCK
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ARCH == MG_ARCH_FREERTOS_TCP
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
      closesocket(fd);
      fd = INVALID_SOCKET;
    }
  }
  if (fd == INVALID_SOCKET) {
    LOG(LL_ERROR, ("Failed to listen on %s, errno %d", url, MG_SOCK_ERRNO));
  }

  return fd;
}

static long mg_sock_recv(struct mg_connection *c, void *buf, size_t len) {
  long n = 0;
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
  return n == 0 ? -1 : n < 0 && mg_sock_would_block() ? 0 : n;
}

// NOTE(lsm): do only one iteration of reads, cause some systems
// (e.g. FreeRTOS stack) return 0 instead of -1/EWOULDBLOCK when no data
static void read_conn(struct mg_connection *c) {
  if (c->recv.len >= MG_MAX_RECV_BUF_SIZE) {
    mg_error(c, "max_recv_buf_size reached");
  } else if (c->recv.size - c->recv.len < MG_IO_SIZE &&
             !mg_iobuf_resize(&c->recv, c->recv.size + MG_IO_SIZE)) {
    mg_error(c, "oom");
  } else {
    char *buf = (char *) &c->recv.buf[c->recv.len];
    size_t len = c->recv.size - c->recv.len;
    long n = c->is_tls ? mg_tls_recv(c, buf, len) : mg_sock_recv(c, buf, len);
    LOG(n > 0 ? LL_VERBOSE_DEBUG : LL_DEBUG,
        ("%-3lu %d%d%d%d%d%d%d%d%d%d%d%d%d%d %7ld %ld/%ld err %d", c->id,
         c->is_listening, c->is_client, c->is_accepted, c->is_resolving,
         c->is_connecting, c->is_tls, c->is_tls_hs, c->is_udp, c->is_websocket,
         c->is_hexdumping, c->is_draining, c->is_closing, c->is_readable,
         c->is_writable, (long) c->recv.len, n, (long) len, MG_SOCK_ERRNO));
    if (n == 0) {
      // Do nothing
    } else if (n < 0) {
      c->is_closing = 1;  // Error, or normal termination
    } else if (n > 0) {
      struct mg_str evd = mg_str_n(buf, (size_t) n);
      if (c->is_hexdumping) {
        char *s = mg_hexdump(buf, (size_t) n);
        LOG(LL_INFO, ("\n-- %lu %s %s %ld\n%s", c->id, c->label, "<-", n, s));
        free(s);
      }
      c->recv.len += (size_t) n;
      mg_call(c, MG_EV_READ, &evd);
    }
  }
}

static void write_conn(struct mg_connection *c) {
  char *buf = (char *) c->send.buf;
  size_t len = c->send.len;
  long n = c->is_tls ? mg_tls_send(c, buf, len) : mg_sock_send(c, buf, len);

  LOG(n > 0 ? LL_VERBOSE_DEBUG : LL_DEBUG,
      ("%-3lu %d%d%d%d%d%d%d%d%d%d%d%d%d%d %7ld %ld err %d", c->id,
       c->is_listening, c->is_client, c->is_accepted, c->is_resolving,
       c->is_connecting, c->is_tls, c->is_tls_hs, c->is_udp, c->is_websocket,
       c->is_hexdumping, c->is_draining, c->is_closing, c->is_readable,
       c->is_writable, (long) c->send.len, n, MG_SOCK_ERRNO));

  if (n == 0) {
    // Do nothing
  } else if (n < 0) {
    c->is_closing = 1;  // Error, or normal termination
  } else if (n > 0) {
    // Hexdump before deleting
    if (c->is_hexdumping) {
      char *s = mg_hexdump(buf, (size_t) n);
      LOG(LL_INFO, ("\n-- %lu %s %s %ld\n%s", c->id, c->label, "<-", n, s));
      free(s);
    }
    mg_iobuf_delete(&c->send, (size_t) n);
    if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
    mg_call(c, MG_EV_WRITE, &n);
    // if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
  }
}

static void close_conn(struct mg_connection *c) {
  // Unlink this connection from the list
  mg_resolve_cancel(c);
  LIST_DELETE(struct mg_connection, &c->mgr->conns, c);
  if (c == c->mgr->dns4.c) c->mgr->dns4.c = NULL;
  if (c == c->mgr->dns6.c) c->mgr->dns6.c = NULL;
  mg_call(c, MG_EV_CLOSE, NULL);
  // while (c->callbacks != NULL) mg_fn_del(c, c->callbacks->fn);
  LOG(LL_DEBUG, ("%lu closed", c->id));
  if (FD(c) != INVALID_SOCKET) {
    closesocket(FD(c));
#if MG_ARCH == MG_ARCH_FREERTOS_TCP
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
#if MG_ARCH == MG_ARCH_FREERTOS_TCP
  (void) c;
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
  int rc, af = AF_INET;
#if MG_ENABLE_IPV6
  if (c->peer.is_ip6) af = AF_INET6;
#endif
  mg_straddr(c, buf, sizeof(buf));
  c->fd = sock2ptr(socket(af, type, 0));
  if (FD(c) == INVALID_SOCKET) {
    mg_error(c, "socket(): %d", MG_SOCK_ERRNO);
    return;
  }

  mg_set_non_blocking_mode(FD(c));
  mg_call(c, MG_EV_RESOLVE, NULL);
  if (type == SOCK_STREAM) {
    union usa usa = tousa(&c->peer);
    socklen_t slen = sizeof(usa.sin);
#if MG_ENABLE_IPV6
    if (c->peer.is_ip6) slen = sizeof(usa.sin6);
#endif
    if ((rc = connect(FD(c), &usa.sa, slen)) == 0 || mg_sock_would_block()) {
      setsockopts(c);
      if (rc != 0) c->is_connecting = 1;
    } else {
      mg_error(c, "connect: %d", MG_SOCK_ERRNO);
    }
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
#if (!defined(_WIN32) && (MG_ARCH != MG_ARCH_FREERTOS_TCP))
  } else if ((long) fd >= FD_SETSIZE) {
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
  bool is_udp = strncmp(url, "udp:", 4) == 0;
  SOCKET fd = mg_open_listener(url);
  if (fd == INVALID_SOCKET) {
  } else if ((c = alloc_conn(mgr, 0, fd)) == NULL) {
    LOG(LL_ERROR, ("OOM %s", url));
    closesocket(fd);
  } else {
    c->fd = sock2ptr(fd);
    c->is_listening = 1;
    c->is_udp = is_udp;
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fn = fn;
    c->fn_data = fn_data;
    LOG(LL_INFO, ("%lu accepting on %s", c->id, url));
  }
  return c;
}

static void mg_iotest(struct mg_mgr *mgr, int ms) {
#if MG_ARCH == MG_ARCH_FREERTOS_TCP
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    if (c->is_closing || c->is_resolving || FD(c) == INVALID_SOCKET) continue;
    FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_READ | eSELECT_EXCEPT);
    if (c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0))
      FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_WRITE);
  }
  FreeRTOS_select(mgr->ss, pdMS_TO_TICKS(ms));
  for (c = mgr->conns; c != NULL; c = c->next) {
    EventBits_t bits = FreeRTOS_FD_ISSET(c->fd, mgr->ss);
    c->is_readable = bits & (eSELECT_READ | eSELECT_EXCEPT) ? 1 : 0;
    c->is_writable = bits & eSELECT_WRITE ? 1 : 0;
    FreeRTOS_FD_CLR(c->fd, mgr->ss,
                    eSELECT_READ | eSELECT_EXCEPT | eSELECT_WRITE);
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
#if MG_ARCH != MG_ARCH_FREERTOS_TCP
  socklen_t len = sizeof(rc);
  if (getsockopt(FD(c), SOL_SOCKET, SO_ERROR, (char *) &rc, &len)) rc = 1;
#endif
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
      if (c->is_readable) read_conn(c);
      if (c->is_writable) write_conn(c);
    }

    if (c->is_draining && c->send.len == 0) c->is_closing = 1;
    if (c->is_closing) close_conn(c);
  }
}
#endif
