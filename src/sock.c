#include "dns.h"
#include "event.h"
#include "log.h"
#include "net.h"
#include "str.h"
#include "timer.h"
#include "tls.h"
#include "url.h"
#include "util.h"

#if MG_ENABLE_SOCKET
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
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

#define FD(c_) ((SOCKET) (size_t) (c_)->fd)
#define S2PTR(s_) ((void *) (size_t) (s_))

#ifndef MSG_NONBLOCKING
#define MSG_NONBLOCKING 0
#endif

#ifndef AF_INET6
#define AF_INET6 10
#endif

union usa {
  struct sockaddr sa;
  struct sockaddr_in sin;
#if MG_ENABLE_IPV6
  struct sockaddr_in6 sin6;
#endif
};

static socklen_t tousa(struct mg_addr *a, union usa *usa) {
  socklen_t len = sizeof(usa->sin);
  memset(usa, 0, sizeof(*usa));
  usa->sin.sin_family = AF_INET;
  usa->sin.sin_port = a->port;
  *(uint32_t *) &usa->sin.sin_addr = a->ip;
#if MG_ENABLE_IPV6
  if (a->is_ip6) {
    usa->sin.sin_family = AF_INET6;
    usa->sin6.sin6_port = a->port;
    memcpy(&usa->sin6.sin6_addr, a->ip6, sizeof(a->ip6));
    len = sizeof(usa->sin6);
  }
#endif
  return len;
}

static void tomgaddr(union usa *usa, struct mg_addr *a, bool is_ip6) {
  a->is_ip6 = is_ip6;
  a->port = usa->sin.sin_port;
  memcpy(&a->ip, &usa->sin.sin_addr, sizeof(a->ip));
#if MG_ENABLE_IPV6
  if (is_ip6) {
    memcpy(a->ip6, &usa->sin6.sin6_addr, sizeof(a->ip6));
    a->port = usa->sin6.sin6_port;
  }
#endif
}

bool mg_sock_would_block(void);
bool mg_sock_would_block(void) {
  int err = MG_SOCK_ERRNO;
  return err == EINPROGRESS || err == EWOULDBLOCK
#ifndef WINCE
         || err == EAGAIN || err == EINTR
#endif
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
         || err == WSAEINTR || err == WSAEWOULDBLOCK
#endif
      ;
}

static void iolog(struct mg_connection *c, char *buf, long n, bool r) {
  int log_level = n > 0 ? MG_LL_VERBOSE : MG_LL_DEBUG;
  char flags[] = {(char) ('0' + c->is_listening),
                  (char) ('0' + c->is_client),
                  (char) ('0' + c->is_accepted),
                  (char) ('0' + c->is_resolving),
                  (char) ('0' + c->is_connecting),
                  (char) ('0' + c->is_tls),
                  (char) ('0' + c->is_tls_hs),
                  (char) ('0' + c->is_udp),
                  (char) ('0' + c->is_websocket),
                  (char) ('0' + c->is_hexdumping),
                  (char) ('0' + c->is_draining),
                  (char) ('0' + c->is_closing),
                  (char) ('0' + c->is_readable),
                  (char) ('0' + c->is_writable),
                  '\0'};
  MG_LOG(log_level,
         ("%lu %s %d:%d %ld err %d (%s)", c->id, flags, (int) c->send.len,
          (int) c->recv.len, n, MG_SOCK_ERRNO, strerror(errno)));
  if (n == 0) {
    // Do nothing
  } else if (n < 0) {
    c->is_closing = 1;  // Error, or normal termination
  } else if (n > 0) {
    if (c->is_hexdumping) {
      union usa usa;
      char t1[50] = "", t2[50] = "";
      socklen_t slen = sizeof(usa.sin);
      char *s = mg_hexdump(buf, (size_t) n);
      struct mg_addr a;
      memset(&usa, 0, sizeof(usa));
      memset(&a, 0, sizeof(a));
      if (getsockname(FD(c), &usa.sa, &slen) < 0) (void) 0;  // Ignore result
      tomgaddr(&usa, &a, c->rem.is_ip6);
      MG_INFO(("\n-- %lu %s %s %s %s %ld\n%s", c->id,
               mg_straddr(&a, t1, sizeof(t1)), r ? "<-" : "->",
               mg_straddr(&c->rem, t2, sizeof(t2)), c->label, n, s));
      free(s);
      (void) t1, (void) t2;  // Silence warnings for MG_ENABLE_LOG=0
    }
    if (r) {
      struct mg_str evd = mg_str_n(buf, (size_t) n);
      c->recv.len += (size_t) n;
      mg_call(c, MG_EV_READ, &evd);
    } else {
      mg_iobuf_del(&c->send, 0, (size_t) n);
      // if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
      mg_call(c, MG_EV_WRITE, &n);
    }
  }
}

static long mg_sock_send(struct mg_connection *c, const void *buf, size_t len) {
  long n;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = sendto(FD(c), (char *) buf, len, 0, &usa.sa, slen);
  } else {
    n = send(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  return n == 0 ? -1 : n < 0 && mg_sock_would_block() ? 0 : n;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  if (c->is_udp) {
    long n = mg_sock_send(c, buf, len);
    iolog(c, (char *) buf, n, false);
    return n > 0;
  } else {
    return mg_iobuf_add(&c->send, c->send.len, buf, len, MG_IO_SIZE);
  }
}

static void mg_set_non_blocking_mode(SOCKET fd) {
#if MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ARCH == MG_ARCH_FREERTOS_TCP
  const BaseType_t off = 0;
  if (setsockopt(fd, 0, FREERTOS_SO_RCVTIMEO, &off, sizeof(off)) != 0) (void) 0;
  if (setsockopt(fd, 0, FREERTOS_SO_SNDTIMEO, &off, sizeof(off)) != 0) (void) 0;
#elif MG_ARCH == MG_ARCH_FREERTOS_LWIP || MG_ARCH == MG_ARCH_RTX_LWIP
  lwip_fcntl(fd, F_SETFL, O_NONBLOCK);
#elif MG_ARCH == MG_ARCH_AZURERTOS
  fcntl(fd, F_SETFL, O_NONBLOCK);
#else
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode
  fcntl(fd, F_SETFD, FD_CLOEXEC);                          // Set close-on-exec
#endif
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  SOCKET fd = INVALID_SOCKET;
  int s_err = 0;  // Memoized socket error, in case closesocket() overrides it
  c->loc.port = mg_htons(mg_url_port(url));
  if (!mg_aton(mg_url_host(url), &c->loc)) {
    MG_ERROR(("invalid listening URL: %s", url));
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->loc, &usa);
    int on = 1, af = c->loc.is_ip6 ? AF_INET6 : AF_INET;
    int type = strncmp(url, "udp:", 4) == 0 ? SOCK_DGRAM : SOCK_STREAM;
    int proto = type == SOCK_DGRAM ? IPPROTO_UDP : IPPROTO_TCP;
    (void) on;

    if ((fd = socket(af, type, proto)) != INVALID_SOCKET &&
#if (MG_ARCH != MG_ARCH_WIN32 || !defined(SO_EXCLUSIVEADDRUSE)) && \
    (!defined(LWIP_SOCKET) || (defined(LWIP_SOCKET) && SO_REUSE == 1))
        // 1. SO_RESUSEADDR is not enabled on Windows because the semantics of
        //    SO_REUSEADDR on UNIX and Windows is different. On Windows,
        //    SO_REUSEADDR allows to bind a socket to a port without error even
        //    if the port is already open by another program. This is not the
        //    behavior SO_REUSEADDR was designed for, and leads to hard-to-track
        //    failure scenarios. Therefore, SO_REUSEADDR was disabled on Windows
        //    unless SO_EXCLUSIVEADDRUSE is supported and set on a socket.
        // 2. In case of LWIP, SO_REUSEADDR should be explicitly enabled, by
        // defining
        //    SO_REUSE (in lwipopts.h), otherwise the code below will compile
        //    but won't work! (setsockopt will return EINVAL)
        !setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on)) &&
#endif
#if MG_ARCH == MG_ARCH_WIN32 && defined(SO_EXCLUSIVEADDRUSE) && !defined(WINCE)
        // "Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE"
        //! setsockopt(fd, SOL_SOCKET, SO_BROADCAST, (char *) &on, sizeof(on))
        //! &&
        !setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *) &on,
                    sizeof(on)) &&
#endif
        bind(fd, &usa.sa, slen) == 0 &&
        // NOTE(lsm): FreeRTOS uses backlog value as a connection limit
        (type == SOCK_DGRAM || listen(fd, MG_SOCK_LISTEN_BACKLOG_SIZE) == 0)) {
      // In case port was set to 0, get the real port number
      if (getsockname(fd, &usa.sa, &slen) == 0) {
        c->loc.port = usa.sin.sin_port;
#if MG_ENABLE_IPV6
        if (c->loc.is_ip6) c->loc.port = usa.sin6.sin6_port;
#endif
      }
      mg_set_non_blocking_mode(fd);
    } else if (fd != INVALID_SOCKET) {
      s_err = MG_SOCK_ERRNO;
      closesocket(fd);
      fd = INVALID_SOCKET;
    }
  }
  c->fd = S2PTR(fd);
  if (fd == INVALID_SOCKET) {
    if (s_err == 0) s_err = MG_SOCK_ERRNO;
    MG_ERROR(("failed %s, errno %d (%s)", url, s_err, strerror(s_err)));
  }
  return fd != INVALID_SOCKET;
}

static long mg_sock_recv(struct mg_connection *c, void *buf, size_t len) {
  long n = 0;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = recvfrom(FD(c), (char *) buf, len, 0, &usa.sa, &slen);
    if (n > 0) tomgaddr(&usa, &c->rem, slen != sizeof(usa.sin));
  } else {
    n = recv(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  return n == 0 ? -1 : n < 0 && mg_sock_would_block() ? 0 : n;
}

// NOTE(lsm): do only one iteration of reads, cause some systems
// (e.g. FreeRTOS stack) return 0 instead of -1/EWOULDBLOCK when no data
static long read_conn(struct mg_connection *c) {
  long n = -1;
  if (c->recv.len >= MG_MAX_RECV_BUF_SIZE) {
    mg_error(c, "max_recv_buf_size reached");
  } else if (c->recv.size - c->recv.len < MG_IO_SIZE &&
             !mg_iobuf_resize(&c->recv, c->recv.size + MG_IO_SIZE)) {
    mg_error(c, "oom");
  } else {
    char *buf = (char *) &c->recv.buf[c->recv.len];
    size_t len = c->recv.size - c->recv.len;
    n = c->is_tls ? mg_tls_recv(c, buf, len) : mg_sock_recv(c, buf, len);
    iolog(c, buf, n, true);
  }
  return n;
}

static void write_conn(struct mg_connection *c) {
  char *buf = (char *) c->send.buf;
  size_t len = c->send.len;
  long n = c->is_tls ? mg_tls_send(c, buf, len) : mg_sock_send(c, buf, len);
  iolog(c, buf, n, false);
}

static void close_conn(struct mg_connection *c) {
  if (FD(c) != INVALID_SOCKET) {
    closesocket(FD(c));
#if MG_ARCH == MG_ARCH_FREERTOS_TCP
    FreeRTOS_FD_CLR(c->fd, c->mgr->ss, eSELECT_ALL);
#endif
    c->fd = NULL;
  }
  mg_close_conn(c);
}

static void setsockopts(struct mg_connection *c) {
#if MG_ARCH == MG_ARCH_FREERTOS_TCP || MG_ARCH == MG_ARCH_AZURERTOS
  (void) c;
#else
  int on = 1;
#if !defined(SOL_TCP)
#define SOL_TCP IPPROTO_TCP
#endif
  if (setsockopt(FD(c), SOL_TCP, TCP_NODELAY, (char *) &on, sizeof(on)) != 0)
    (void) 0;
#if defined(TCP_QUICKACK)
  if (setsockopt(FD(c), SOL_TCP, TCP_QUICKACK, (char *) &on, sizeof(on)) != 0)
    (void) 0;
#endif
  if (setsockopt(FD(c), SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof(on)) !=
      0)
    (void) 0;
#if (defined(ESP32) && ESP32) || (defined(ESP8266) && ESP8266) || \
    defined(__linux__)
  int idle = 60;
  if (setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle)) != 0)
    (void) 0;
#endif
#if MG_ARCH != MG_ARCH_WIN32 && !defined(__QNX__)
  {
    int cnt = 3, intvl = 20;
    if (setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPCNT, &cnt, sizeof(cnt)) != 0)
      (void) 0;
    if (setsockopt(FD(c), IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl)) !=
        0)
      (void) 0;
  }
#endif
#endif
}

void mg_connect_resolved(struct mg_connection *c) {
  // char buf[40];
  int type = c->is_udp ? SOCK_DGRAM : SOCK_STREAM;
  int rc, af = c->rem.is_ip6 ? AF_INET6 : AF_INET;
  // mg_straddr(&c->rem, buf, sizeof(buf));
  c->fd = S2PTR(socket(af, type, 0));
  c->is_resolving = 0;
  if (FD(c) == INVALID_SOCKET) {
    mg_error(c, "socket(): %d", MG_SOCK_ERRNO);
  } else if (c->is_udp) {
    mg_call(c, MG_EV_RESOLVE, NULL);
    mg_call(c, MG_EV_CONNECT, NULL);
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    mg_call(c, MG_EV_RESOLVE, NULL);
    if ((rc = connect(FD(c), &usa.sa, slen)) == 0) {
      mg_call(c, MG_EV_CONNECT, NULL);
    } else if (mg_sock_would_block()) {
      c->is_connecting = 1;
    } else {
      mg_error(c, "connect: %d", MG_SOCK_ERRNO);
    }
  }
}

static void accept_conn(struct mg_mgr *mgr, struct mg_connection *lsn) {
  struct mg_connection *c = NULL;
  union usa usa;
  socklen_t sa_len = sizeof(usa);
  SOCKET fd = accept(FD(lsn), &usa.sa, &sa_len);
  if (fd == INVALID_SOCKET) {
#if MG_ARCH == MG_ARCH_AZURERTOS
    // AzureRTOS, in non-block socket mode can mark listening socket readable
    // even it is not. See comment for 'select' func implementation in nx_bsd.c
    // That's not an error, just should try later
    if (MG_SOCK_ERRNO != EAGAIN)
#endif
      MG_ERROR(("%lu accept failed, errno %d", lsn->id, MG_SOCK_ERRNO));
#if ((MG_ARCH != MG_ARCH_WIN32) && (MG_ARCH != MG_ARCH_FREERTOS_TCP))
  } else if ((long) fd >= FD_SETSIZE) {
    MG_ERROR(("%ld > %ld", (long) fd, (long) FD_SETSIZE));
    closesocket(fd);
#endif
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("%lu OOM", lsn->id));
    closesocket(fd);
  } else {
    char buf[40];
    tomgaddr(&usa, &c->rem, sa_len != sizeof(usa.sin));
    mg_straddr(&c->rem, buf, sizeof(buf));
    MG_DEBUG(("%lu accepted %s", c->id, buf));
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fd = S2PTR(fd);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    c->is_accepted = 1;
    c->is_hexdumping = lsn->is_hexdumping;
    c->pfn = lsn->pfn;
    c->loc = lsn->loc;
    c->pfn_data = lsn->pfn_data;
    c->fn = lsn->fn;
    c->fn_data = lsn->fn_data;
    mg_call(c, MG_EV_OPEN, NULL);
    mg_call(c, MG_EV_ACCEPT, NULL);
  }
}

static bool mg_socketpair(SOCKET sp[2], union usa usa[2]) {
  socklen_t n = sizeof(usa[0].sin);
  bool result = false;

  (void) memset(&usa[0], 0, sizeof(usa[0]));
  usa[0].sin.sin_family = AF_INET;
  *(uint32_t *) &usa->sin.sin_addr = mg_htonl(0x7f000001);  // 127.0.0.1
  usa[1] = usa[0];

  if ((sp[0] = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET &&
      (sp[1] = socket(AF_INET, SOCK_DGRAM, 0)) != INVALID_SOCKET &&
      bind(sp[0], &usa[0].sa, n) == 0 && bind(sp[1], &usa[1].sa, n) == 0 &&
      getsockname(sp[0], &usa[0].sa, &n) == 0 &&
      getsockname(sp[1], &usa[1].sa, &n) == 0 &&
      connect(sp[0], &usa[1].sa, n) == 0 &&
      connect(sp[1], &usa[0].sa, n) == 0) {
    mg_set_non_blocking_mode(sp[1]);  // Set close-on-exec
    result = true;
  } else {
    if (sp[0] != INVALID_SOCKET) closesocket(sp[0]);
    if (sp[1] != INVALID_SOCKET) closesocket(sp[1]);
    sp[0] = sp[1] = INVALID_SOCKET;
  }

  return result;
}

void mg_mgr_wakeup(struct mg_connection *c, const void *buf, size_t len) {
  if (buf == NULL || len == 0) buf = (void *) "", len = 1;
  if ((size_t) send((SOCKET) (size_t) c->pfn_data, (const char *) buf, len,
                    MSG_NONBLOCKING) != len)
    (void) 0;
}

static void pf1(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_READ) {
    mg_iobuf_free(&c->recv);
  } else if (ev == MG_EV_CLOSE) {
    closesocket((SOCKET) (size_t) c->pfn_data);
  }
  (void) ev_data, (void) fn_data;
}

struct mg_connection *mg_mkpipe(struct mg_mgr *mgr, mg_event_handler_t fn,
                                void *fn_data) {
  union usa usa[2];
  SOCKET sp[2] = {INVALID_SOCKET, INVALID_SOCKET};
  struct mg_connection *c = NULL;
  if (!mg_socketpair(sp, usa)) {
    MG_ERROR(("Cannot create socket pair"));
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    closesocket(sp[0]);
    closesocket(sp[1]);
    MG_ERROR(("OOM"));
  } else {
    MG_DEBUG(("pipe %lu", (unsigned long) sp[0]));
    tomgaddr(&usa[0], &c->rem, false);
    c->fd = S2PTR(sp[1]);
    c->is_udp = 1;
    c->pfn = pf1;
    c->pfn_data = (void *) (size_t) sp[0];
    c->fn = fn;
    c->fn_data = fn_data;
    mg_call(c, MG_EV_OPEN, NULL);
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
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
    if (c->is_closing || c->is_resolving || FD(c) == INVALID_SOCKET) continue;
    FD_SET(FD(c), &rset);
    if (FD(c) > maxfd) maxfd = FD(c);
    if (c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0))
      FD_SET(FD(c), &wset);
  }

  if ((rc = select((int) maxfd + 1, &rset, &wset, NULL, &tv)) < 0) {
    MG_DEBUG(("select: %d %d", rc, MG_SOCK_ERRNO));
    FD_ZERO(&rset);
    FD_ZERO(&wset);
  }

  for (c = mgr->conns; c != NULL; c = c->next) {
    // TLS might have stuff buffered, so dig everything
    c->is_readable = FD(c) != INVALID_SOCKET && FD_ISSET(FD(c), &rset);
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
    char buf[50];
    mg_error(c, "error connecting to %s",
             mg_straddr(&c->rem, buf, sizeof(buf)));
  } else {
    if (c->is_tls_hs) mg_tls_handshake(c);
    mg_call(c, MG_EV_CONNECT, NULL);
  }
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  int64_t now;

  mg_iotest(mgr, ms);
  now = mg_millis();
  mg_timer_poll(now);

  for (c = mgr->conns; c != NULL; c = tmp) {
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    MG_VERBOSE(("%lu %c%c %c%c%c%c%c", c->id, c->is_readable ? 'r' : '-',
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
