#include "dns.h"
#include "event.h"
#include "log.h"
#include "net.h"
#include "printf.h"
#include "str.h"
#include "timer.h"
#include "tls.h"
#include "url.h"
#include "util.h"

#if MG_ENABLE_SOCKET

#ifndef closesocket
#define closesocket(x) close(x)
#endif

#define FD(c_) ((MG_SOCKET_TYPE) (size_t) (c_)->fd)
#define S2PTR(s_) ((void *) (size_t) (s_))

#ifndef MSG_NONBLOCKING
#define MSG_NONBLOCKING 0
#endif

#ifndef AF_INET6
#define AF_INET6 10
#endif

#ifndef MG_SOCK_ERR
#define MG_SOCK_ERR(errcode) ((errcode) < 0 ? errno : 0)
#endif

#ifndef MG_SOCK_INTR
#define MG_SOCK_INTR(fd) (fd == MG_INVALID_SOCKET && MG_SOCK_ERR(-1) == EINTR)
#endif

#ifndef MG_SOCK_PENDING
#define MG_SOCK_PENDING(errcode) \
  (((errcode) < 0) && (errno == EINPROGRESS || errno == EWOULDBLOCK))
#endif

#ifndef MG_SOCK_RESET
#define MG_SOCK_RESET(errcode) \
  (((errcode) < 0) && (errno == EPIPE || errno == ECONNRESET))
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
  memcpy(&usa->sin.sin_addr, a->ip, sizeof(uint32_t));
#if MG_ENABLE_IPV6
  if (a->is_ip6) {
    usa->sin.sin_family = AF_INET6;
    usa->sin6.sin6_port = a->port;
    usa->sin6.sin6_scope_id = a->scope_id;
    memcpy(&usa->sin6.sin6_addr, a->ip, sizeof(a->ip));
    len = sizeof(usa->sin6);
  }
#endif
  return len;
}

static void tomgaddr(union usa *usa, struct mg_addr *a, bool is_ip6) {
  a->is_ip6 = is_ip6;
  a->port = usa->sin.sin_port;
  memcpy(&a->ip, &usa->sin.sin_addr, sizeof(uint32_t));
#if MG_ENABLE_IPV6
  if (is_ip6) {
    memcpy(a->ip, &usa->sin6.sin6_addr, sizeof(a->ip));
    a->port = usa->sin6.sin6_port;
    a->scope_id = (uint8_t) usa->sin6.sin6_scope_id;
  }
#endif
}

static void setlocaddr(MG_SOCKET_TYPE fd, struct mg_addr *addr) {
  union usa usa;
  socklen_t n = sizeof(usa);
  if (getsockname(fd, &usa.sa, &n) == 0) {
    tomgaddr(&usa, addr, n != sizeof(usa.sin));
  }
}

static void iolog(struct mg_connection *c, char *buf, long n, bool r) {
  if (n == MG_IO_WAIT) {
    // Do nothing
  } else if (n <= 0) {
    c->is_closing = 1;  // Termination. Don't call mg_error(): #1529
  } else if (n > 0) {
    if (c->is_hexdumping) {
      union usa usa;
      socklen_t slen = sizeof(usa.sin);
      if (getsockname(FD(c), &usa.sa, &slen) < 0) (void) 0;  // Ignore result
      MG_INFO(("\n-- %lu %M %s %M %ld", c->id, mg_print_ip_port, &c->loc,
               r ? "<-" : "->", mg_print_ip_port, &c->rem, n));

      mg_hexdump(buf, (size_t) n);
    }
    if (r) {
      c->recv.len += (size_t) n;
      mg_call(c, MG_EV_READ, &n);
    } else {
      mg_iobuf_del(&c->send, 0, (size_t) n);
      // if (c->send.len == 0) mg_iobuf_resize(&c->send, 0);
      if (c->send.len == 0) {
        MG_EPOLL_MOD(c, 0);
      }
      mg_call(c, MG_EV_WRITE, &n);
    }
  }
}

long mg_io_send(struct mg_connection *c, const void *buf, size_t len) {
  long n;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = sendto(FD(c), (char *) buf, len, 0, &usa.sa, slen);
    if (n > 0) setlocaddr(FD(c), &c->loc);
  } else {
    n = send(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  if (MG_SOCK_PENDING(n)) return MG_IO_WAIT;
  if (MG_SOCK_RESET(n)) return MG_IO_RESET;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

bool mg_send(struct mg_connection *c, const void *buf, size_t len) {
  if (c->is_udp) {
    long n = mg_io_send(c, buf, len);
    MG_DEBUG(("%lu %ld %d:%d %ld err %d", c->id, c->fd, (int) c->send.len,
              (int) c->recv.len, n, MG_SOCK_ERR(n)));
    iolog(c, (char *) buf, n, false);
    return n > 0;
  } else {
    return mg_iobuf_add(&c->send, c->send.len, buf, len);
  }
}

static void mg_set_non_blocking_mode(MG_SOCKET_TYPE fd) {
#if defined(MG_CUSTOM_NONBLOCK)
  MG_CUSTOM_NONBLOCK(fd);
#elif MG_ARCH == MG_ARCH_WIN32 && MG_ENABLE_WINSOCK
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ENABLE_RL
  unsigned long on = 1;
  ioctlsocket(fd, FIONBIO, &on);
#elif MG_ENABLE_FREERTOS_TCP
  const BaseType_t off = 0;
  if (setsockopt(fd, 0, FREERTOS_SO_RCVTIMEO, &off, sizeof(off)) != 0) (void) 0;
  if (setsockopt(fd, 0, FREERTOS_SO_SNDTIMEO, &off, sizeof(off)) != 0) (void) 0;
#elif MG_ENABLE_LWIP
  lwip_fcntl(fd, F_SETFL, O_NONBLOCK);
#elif MG_ARCH == MG_ARCH_AZURERTOS
  fcntl(fd, F_SETFL, O_NONBLOCK);
#elif MG_ARCH == MG_ARCH_TIRTOS
  int val = 0;
  setsockopt(fd, SOL_SOCKET, SO_BLOCKING, &val, sizeof(val));
  // SPRU524J section 3.3.3 page 63, SO_SNDLOWAT
  int sz = sizeof(val);
  getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &sz);
  val /= 2;  // set send low-water mark at half send buffer size
  setsockopt(fd, SOL_SOCKET, SO_SNDLOWAT, &val, sizeof(val));
#else
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);  // Non-blocking mode
  fcntl(fd, F_SETFD, FD_CLOEXEC);                          // Set close-on-exec
#endif
}

bool mg_open_listener(struct mg_connection *c, const char *url) {
  MG_SOCKET_TYPE fd = MG_INVALID_SOCKET;
  bool success = false;
  c->loc.port = mg_htons(mg_url_port(url));
  if (!mg_aton(mg_url_host(url), &c->loc)) {
    MG_ERROR(("invalid listening URL: %s", url));
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->loc, &usa);
    int rc, on = 1, af = c->loc.is_ip6 ? AF_INET6 : AF_INET;
    int type = strncmp(url, "udp:", 4) == 0 ? SOCK_DGRAM : SOCK_STREAM;
    int proto = type == SOCK_DGRAM ? IPPROTO_UDP : IPPROTO_TCP;
    (void) on;

    if ((fd = socket(af, type, proto)) == MG_INVALID_SOCKET) {
      MG_ERROR(("socket: %d", MG_SOCK_ERR(-1)));
#if defined(SO_EXCLUSIVEADDRUSE)
    } else if ((rc = setsockopt(fd, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
                                (char *) &on, sizeof(on))) != 0) {
      // "Using SO_REUSEADDR and SO_EXCLUSIVEADDRUSE"
      MG_ERROR(("setsockopt(SO_EXCLUSIVEADDRUSE): %d %d", on, MG_SOCK_ERR(rc)));
#elif defined(SO_REUSEADDR) && (!defined(LWIP_SOCKET) || SO_REUSE)
    } else if ((rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &on,
                                sizeof(on))) != 0) {
      // 1. SO_REUSEADDR semantics on UNIX and Windows is different.  On
      // Windows, SO_REUSEADDR allows to bind a socket to a port without error
      // even if the port is already open by another program. This is not the
      // behavior SO_REUSEADDR was designed for, and leads to hard-to-track
      // failure scenarios.
      //
      // 2. For LWIP, SO_REUSEADDR should be explicitly enabled by defining
      // SO_REUSE = 1 in lwipopts.h, otherwise the code below will compile but
      // won't work! (setsockopt will return EINVAL)
      MG_ERROR(("setsockopt(SO_REUSEADDR): %d", MG_SOCK_ERR(rc)));
#endif
#if MG_IPV6_V6ONLY
      // Bind only to the V6 address, not V4 address on this port
    } else if (c->loc.is_ip6 &&
               (rc = setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &on,
                                sizeof(on))) != 0) {
      // See #2089. Allow to bind v4 and v6 sockets on the same port
      MG_ERROR(("setsockopt(IPV6_V6ONLY): %d", MG_SOCK_ERR(rc)));
#endif
    } else if ((rc = bind(fd, &usa.sa, slen)) != 0) {
      MG_ERROR(("bind: %d", MG_SOCK_ERR(rc)));
    } else if ((type == SOCK_STREAM &&
                (rc = listen(fd, MG_SOCK_LISTEN_BACKLOG_SIZE)) != 0)) {
      // NOTE(lsm): FreeRTOS uses backlog value as a connection limit
      // In case port was set to 0, get the real port number
      MG_ERROR(("listen: %d", MG_SOCK_ERR(rc)));
    } else {
      setlocaddr(fd, &c->loc);
      mg_set_non_blocking_mode(fd);
      c->fd = S2PTR(fd);
      MG_EPOLL_ADD(c);
      success = true;
    }
  }
  if (success == false && fd != MG_INVALID_SOCKET) closesocket(fd);
  return success;
}

long mg_io_recv(struct mg_connection *c, void *buf, size_t len) {
  long n = 0;
  if (c->is_udp) {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    n = recvfrom(FD(c), (char *) buf, len, 0, &usa.sa, &slen);
    if (n > 0) tomgaddr(&usa, &c->rem, slen != sizeof(usa.sin));
  } else {
    n = recv(FD(c), (char *) buf, len, MSG_NONBLOCKING);
  }
  if (MG_SOCK_PENDING(n)) return MG_IO_WAIT;
  if (MG_SOCK_RESET(n)) return MG_IO_RESET;
  if (n <= 0) return MG_IO_ERR;
  return n;
}

// NOTE(lsm): do only one iteration of reads, cause some systems
// (e.g. FreeRTOS stack) return 0 instead of -1/EWOULDBLOCK when no data
static void read_conn(struct mg_connection *c) {
  long n = -1;
  if (c->recv.len >= MG_MAX_RECV_SIZE) {
    mg_error(c, "max_recv_buf_size reached");
  } else if (c->recv.size <= c->recv.len &&
             !mg_iobuf_resize(&c->recv, c->recv.size + MG_IO_SIZE)) {
    mg_error(c, "oom");
  } else {
    char *buf = (char *) &c->recv.buf[c->recv.len];
    size_t len = c->recv.size - c->recv.len;
    n = c->is_tls ? mg_tls_recv(c, buf, len) : mg_io_recv(c, buf, len);
    MG_DEBUG(("%lu %ld snd %ld/%ld rcv %ld/%ld n=%ld err=%d", c->id, c->fd,
              (long) c->send.len, (long) c->send.size, (long) c->recv.len,
              (long) c->recv.size, n, MG_SOCK_ERR(n)));
    iolog(c, buf, n, true);
  }
}

static void write_conn(struct mg_connection *c) {
  char *buf = (char *) c->send.buf;
  size_t len = c->send.len;
  long n = c->is_tls ? mg_tls_send(c, buf, len) : mg_io_send(c, buf, len);
  MG_DEBUG(("%lu %ld snd %ld/%ld rcv %ld/%ld n=%ld err=%d", c->id, c->fd,
            (long) c->send.len, (long) c->send.size, (long) c->recv.len,
            (long) c->recv.size, n, MG_SOCK_ERR(n)));
  iolog(c, buf, n, false);
}

static void close_conn(struct mg_connection *c) {
  if (FD(c) != MG_INVALID_SOCKET) {
#if MG_ENABLE_EPOLL
    epoll_ctl(c->mgr->epoll_fd, EPOLL_CTL_DEL, FD(c), NULL);
#endif
    closesocket(FD(c));
#if MG_ENABLE_FREERTOS_TCP
    FreeRTOS_FD_CLR(c->fd, c->mgr->ss, eSELECT_ALL);
#endif
  }
  mg_close_conn(c);
}

static void connect_conn(struct mg_connection *c) {
  union usa usa;
  socklen_t n = sizeof(usa);
  // Use getpeername() to test whether we have connected
  if (getpeername(FD(c), &usa.sa, &n) == 0) {
    c->is_connecting = 0;
    mg_call(c, MG_EV_CONNECT, NULL);
    MG_EPOLL_MOD(c, 0);
    if (c->is_tls_hs) mg_tls_handshake(c);
  } else {
    mg_error(c, "socket error");
  }
}

static void setsockopts(struct mg_connection *c) {
#if MG_ENABLE_FREERTOS_TCP || MG_ARCH == MG_ARCH_AZURERTOS || \
    MG_ARCH == MG_ARCH_TIRTOS
  (void) c;
#else
  int on = 1;
#if !defined(SOL_TCP)
#define SOL_TCP IPPROTO_TCP
#endif
  if (setsockopt(FD(c), SOL_TCP, TCP_NODELAY, (char *) &on, sizeof(on)) != 0)
    (void) 0;
  if (setsockopt(FD(c), SOL_SOCKET, SO_KEEPALIVE, (char *) &on, sizeof(on)) !=
      0)
    (void) 0;
#endif
}

void mg_connect_resolved(struct mg_connection *c) {
  int type = c->is_udp ? SOCK_DGRAM : SOCK_STREAM;
  int rc, af = c->rem.is_ip6 ? AF_INET6 : AF_INET;  // c->rem has resolved IP
  c->fd = S2PTR(socket(af, type, 0));               // Create outbound socket
  c->is_resolving = 0;                              // Clear resolving flag
  if (FD(c) == MG_INVALID_SOCKET) {
    mg_error(c, "socket(): %d", MG_SOCK_ERR(-1));
  } else if (c->is_udp) {
    MG_EPOLL_ADD(c);
#if MG_ARCH == MG_ARCH_TIRTOS
    union usa usa;  // TI-RTOS NDK requires binding to receive on UDP sockets
    socklen_t slen = tousa(&c->loc, &usa);
    if ((rc = bind(c->fd, &usa.sa, slen)) != 0)
      MG_ERROR(("bind: %d", MG_SOCK_ERR(rc)));
#endif
    mg_call(c, MG_EV_RESOLVE, NULL);
    mg_call(c, MG_EV_CONNECT, NULL);
  } else {
    union usa usa;
    socklen_t slen = tousa(&c->rem, &usa);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    MG_EPOLL_ADD(c);
    mg_call(c, MG_EV_RESOLVE, NULL);
    rc = connect(FD(c), &usa.sa, slen);  // Attempt to connect
    if (rc == 0) {                       // Success
      mg_call(c, MG_EV_CONNECT, NULL);   // Send MG_EV_CONNECT to the user
    } else if (MG_SOCK_PENDING(rc)) {    // Need to wait for TCP handshake
      MG_DEBUG(("%lu %ld -> %M pend", c->id, c->fd, mg_print_ip_port, &c->rem));
      c->is_connecting = 1;
    } else {
      mg_error(c, "connect: %d", MG_SOCK_ERR(rc));
    }
  }
}

static MG_SOCKET_TYPE raccept(MG_SOCKET_TYPE sock, union usa *usa,
                              socklen_t *len) {
  MG_SOCKET_TYPE fd = MG_INVALID_SOCKET;
  do {
    memset(usa, 0, sizeof(*usa));
    fd = accept(sock, &usa->sa, len);
  } while (MG_SOCK_INTR(fd));
  return fd;
}

static void accept_conn(struct mg_mgr *mgr, struct mg_connection *lsn) {
  struct mg_connection *c = NULL;
  union usa usa;
  socklen_t sa_len = sizeof(usa);
  MG_SOCKET_TYPE fd = raccept(FD(lsn), &usa, &sa_len);
  if (fd == MG_INVALID_SOCKET) {
#if MG_ARCH == MG_ARCH_AZURERTOS || defined(__ECOS)
    // AzureRTOS, in non-block socket mode can mark listening socket readable
    // even it is not. See comment for 'select' func implementation in
    // nx_bsd.c That's not an error, just should try later
    if (errno != EAGAIN)
#endif
      MG_ERROR(("%lu accept failed, errno %d", lsn->id, MG_SOCK_ERR(-1)));
#if (MG_ARCH != MG_ARCH_WIN32) && !MG_ENABLE_FREERTOS_TCP && \
    (MG_ARCH != MG_ARCH_TIRTOS) && !MG_ENABLE_POLL && !MG_ENABLE_EPOLL
  } else if ((long) fd >= FD_SETSIZE) {
    MG_ERROR(("%ld > %ld", (long) fd, (long) FD_SETSIZE));
    closesocket(fd);
#endif
  } else if ((c = mg_alloc_conn(mgr)) == NULL) {
    MG_ERROR(("%lu OOM", lsn->id));
    closesocket(fd);
  } else {
    tomgaddr(&usa, &c->rem, sa_len != sizeof(usa.sin));
    LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
    c->fd = S2PTR(fd);
    MG_EPOLL_ADD(c);
    mg_set_non_blocking_mode(FD(c));
    setsockopts(c);
    c->is_accepted = 1;
    c->is_hexdumping = lsn->is_hexdumping;
    c->loc = lsn->loc;
    c->pfn = lsn->pfn;
    c->pfn_data = lsn->pfn_data;
    c->fn = lsn->fn;
    c->fn_data = lsn->fn_data;
    MG_DEBUG(("%lu %ld accepted %M -> %M", c->id, c->fd, mg_print_ip_port,
              &c->rem, mg_print_ip_port, &c->loc));
    mg_call(c, MG_EV_OPEN, NULL);
    mg_call(c, MG_EV_ACCEPT, NULL);
  }
}

static bool can_read(const struct mg_connection *c) {
  return c->is_full == false;
}

static bool can_write(const struct mg_connection *c) {
  return c->is_connecting || (c->send.len > 0 && c->is_tls_hs == 0);
}

static bool skip_iotest(const struct mg_connection *c) {
  return (c->is_closing || c->is_resolving || FD(c) == MG_INVALID_SOCKET) ||
         (can_read(c) == false && can_write(c) == false);
}

static void mg_iotest(struct mg_mgr *mgr, int ms) {
#if MG_ENABLE_FREERTOS_TCP
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) continue;
    if (can_read(c))
      FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_READ | eSELECT_EXCEPT);
    if (can_write(c)) FreeRTOS_FD_SET(c->fd, mgr->ss, eSELECT_WRITE);
  }
  FreeRTOS_select(mgr->ss, pdMS_TO_TICKS(ms));
  for (c = mgr->conns; c != NULL; c = c->next) {
    EventBits_t bits = FreeRTOS_FD_ISSET(c->fd, mgr->ss);
    c->is_readable = bits & (eSELECT_READ | eSELECT_EXCEPT) ? 1U : 0;
    c->is_writable = bits & eSELECT_WRITE ? 1U : 0;
    if (c->fd != MG_INVALID_SOCKET)
      FreeRTOS_FD_CLR(c->fd, mgr->ss,
                      eSELECT_READ | eSELECT_EXCEPT | eSELECT_WRITE);
  }
#elif MG_ENABLE_EPOLL
  size_t max = 1;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (mg_tls_pending(c) > 0) ms = 1, c->is_readable = 1;
    if (can_write(c)) MG_EPOLL_MOD(c, 1);
    max++;
  }
  struct epoll_event *evs = (struct epoll_event *) alloca(max * sizeof(evs[0]));
  int n = epoll_wait(mgr->epoll_fd, evs, (int) max, ms);
  for (int i = 0; i < n; i++) {
    struct mg_connection *c = (struct mg_connection *) evs[i].data.ptr;
    if (evs[i].events & EPOLLERR) {
      mg_error(c, "socket error");
    } else if (c->is_readable == 0) {
      bool rd = evs[i].events & (EPOLLIN | EPOLLHUP);
      bool wr = evs[i].events & EPOLLOUT;
      c->is_readable = can_read(c) && rd ? 1U : 0;
      c->is_writable = can_write(c) && wr ? 1U : 0;
    }
  }
  (void) skip_iotest;
#elif MG_ENABLE_POLL
  nfds_t n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) n++;
  struct pollfd *fds = (struct pollfd *) alloca(n * sizeof(fds[0]));
  memset(fds, 0, n * sizeof(fds[0]));
  n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) {
      // Socket not valid, ignore
    } else if (mg_tls_pending(c) > 0) {
      ms = 1;  // Don't wait if TLS is ready
    } else {
      fds[n].fd = FD(c);
      if (can_read(c)) fds[n].events |= POLLIN;
      if (can_write(c)) fds[n].events |= POLLOUT;
      n++;
    }
  }

  // MG_INFO(("poll n=%d ms=%d", (int) n, ms));
  if (poll(fds, n, ms) < 0) {
#if MG_ARCH == MG_ARCH_WIN32
    if (n == 0) Sleep(ms);  // On Windows, poll fails if no sockets
#endif
    memset(fds, 0, n * sizeof(fds[0]));
  }
  n = 0;
  for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next) {
    if (skip_iotest(c)) {
      // Socket not valid, ignore
    } else if (mg_tls_pending(c) > 0) {
      c->is_readable = 1;
    } else {
      if (fds[n].revents & POLLERR) {
        mg_error(c, "socket error");
      } else {
        c->is_readable =
            (unsigned) (fds[n].revents & (POLLIN | POLLHUP) ? 1 : 0);
        c->is_writable = (unsigned) (fds[n].revents & POLLOUT ? 1 : 0);
      }
      n++;
    }
  }
#else
  struct timeval tv = {ms / 1000, (ms % 1000) * 1000}, tv_zero = {0, 0}, *tvp;
  struct mg_connection *c;
  fd_set rset, wset, eset;
  MG_SOCKET_TYPE maxfd = 0;
  int rc;

  FD_ZERO(&rset);
  FD_ZERO(&wset);
  FD_ZERO(&eset);
  tvp = ms < 0 ? NULL : &tv;
  for (c = mgr->conns; c != NULL; c = c->next) {
    c->is_readable = c->is_writable = 0;
    if (skip_iotest(c)) continue;
    FD_SET(FD(c), &eset);
    if (can_read(c)) FD_SET(FD(c), &rset);
    if (can_write(c)) FD_SET(FD(c), &wset);
    if (mg_tls_pending(c) > 0) tvp = &tv_zero;
    if (FD(c) > maxfd) maxfd = FD(c);
  }

  if ((rc = select((int) maxfd + 1, &rset, &wset, &eset, tvp)) < 0) {
#if MG_ARCH == MG_ARCH_WIN32
    if (maxfd == 0) Sleep(ms);  // On Windows, select fails if no sockets
#else
    MG_ERROR(("select: %d %d", rc, MG_SOCK_ERR(rc)));
#endif
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);
  }

  for (c = mgr->conns; c != NULL; c = c->next) {
    if (FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &eset)) {
      mg_error(c, "socket error");
    } else {
      c->is_readable = FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &rset);
      c->is_writable = FD(c) != MG_INVALID_SOCKET && FD_ISSET(FD(c), &wset);
      if (mg_tls_pending(c) > 0) c->is_readable = 1;
    }
  }
#endif
}

void mg_mgr_poll(struct mg_mgr *mgr, int ms) {
  struct mg_connection *c, *tmp;
  uint64_t now;

  mg_iotest(mgr, ms);
  now = mg_millis();
  mg_timer_poll(&mgr->timers, now);

  for (c = mgr->conns; c != NULL; c = tmp) {
    bool is_resp = c->is_resp;
    tmp = c->next;
    mg_call(c, MG_EV_POLL, &now);
    if (is_resp && !c->is_resp) {
      long n = 0;
      mg_call(c, MG_EV_READ, &n);
    }
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
