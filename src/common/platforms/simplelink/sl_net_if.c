/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/platforms/simplelink/sl_net_if.h"

#if MG_ENABLE_NET_IF_SIMPLELINK

#include "mongoose/src/internal.h"
#include "mongoose/src/util.h"

#define MG_TCP_RECV_BUFFER_SIZE 1024
#define MG_UDP_RECV_BUFFER_SIZE 1500

static sock_t mg_open_listening_socket(struct mg_connection *nc,
                                       union socket_address *sa, int type,
                                       int proto);

static void mg_set_non_blocking_mode(sock_t sock) {
  SlSockNonblocking_t opt;
#if SL_MAJOR_VERSION_NUM < 2
  opt.NonblockingEnabled = 1;
#else
  opt.NonBlockingEnabled = 1;
#endif
  sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &opt, sizeof(opt));
}

static int mg_is_error(int n) {
  return (n < 0 && n != SL_ERROR_BSD_EALREADY && n != SL_ERROR_BSD_EAGAIN);
}

static void mg_sl_if_connect_tcp(struct mg_connection *nc,
                                 const union socket_address *sa) {
  int proto = 0;
#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_SIMPLELINK
  if (nc->flags & MG_F_SSL) proto = SL_SEC_SOCKET;
#endif
  sock_t sock = sl_Socket(AF_INET, SOCK_STREAM, proto);
  if (sock < 0) {
    nc->err = sock;
    goto out;
  }
  mg_sock_set(nc, sock);
#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_SIMPLELINK
  nc->err = sl_set_ssl_opts(sock, nc);
  if (nc->err != 0) goto out;
#endif
  nc->err = sl_Connect(sock, &sa->sa, sizeof(sa->sin));
out:
  DBG(("%p to %s:%d sock %d %d err %d", nc, inet_ntoa(sa->sin.sin_addr),
       ntohs(sa->sin.sin_port), nc->sock, proto, nc->err));
}

static void mg_sl_if_connect_udp(struct mg_connection *nc) {
  sock_t sock = sl_Socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    nc->err = sock;
    return;
  }
  mg_sock_set(nc, sock);
  nc->err = 0;
}

static int mg_sl_if_listen_tcp(struct mg_connection *nc,
                               union socket_address *sa) {
  int proto = 0;
  if (nc->flags & MG_F_SSL) proto = SL_SEC_SOCKET;
  sock_t sock = mg_open_listening_socket(nc, sa, SOCK_STREAM, proto);
  if (sock < 0) return sock;
  mg_sock_set(nc, sock);
  return 0;
}

static int mg_sl_if_listen_udp(struct mg_connection *nc,
                               union socket_address *sa) {
  sock_t sock = mg_open_listening_socket(nc, sa, SOCK_DGRAM, 0);
  if (sock == INVALID_SOCKET) return (errno ? errno : 1);
  mg_sock_set(nc, sock);
  return 0;
}

static int mg_sl_if_tcp_send(struct mg_connection *nc, const void *buf,
                             size_t len) {
  int n = (int) sl_Send(nc->sock, buf, len, 0);
  if (n < 0 && !mg_is_error(n)) n = 0;
  return n;
}

static int mg_sl_if_udp_send(struct mg_connection *nc, const void *buf,
                             size_t len) {
  int n = sl_SendTo(nc->sock, buf, len, 0, &nc->sa.sa, sizeof(nc->sa.sin));
  if (n < 0 && !mg_is_error(n)) n = 0;
  return n;
}

static int mg_sl_if_tcp_recv(struct mg_connection *nc, void *buf, size_t len) {
  int n = sl_Recv(nc->sock, buf, len, 0);
  if (n == 0) {
    /* Orderly shutdown of the socket, try flushing output. */
    nc->flags |= MG_F_SEND_AND_CLOSE;
  } else if (n < 0 && !mg_is_error(n)) {
    n = 0;
  }
  return n;
}

static int mg_sl_if_udp_recv(struct mg_connection *nc, void *buf, size_t len,
                             union socket_address *sa, size_t *sa_len) {
  SlSocklen_t sa_len_t = *sa_len;
  int n = sl_RecvFrom(nc->sock, buf, MG_UDP_RECV_BUFFER_SIZE, 0,
                      (SlSockAddr_t *) sa, &sa_len_t);
  *sa_len = sa_len_t;
  if (n < 0 && !mg_is_error(n)) n = 0;
  return n;
}

static int mg_sl_if_create_conn(struct mg_connection *nc) {
  (void) nc;
  return 1;
}

void mg_sl_if_destroy_conn(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) return;
  /* For UDP, only close outgoing sockets or listeners. */
  if (!(nc->flags & MG_F_UDP) || nc->listener == NULL) {
    sl_Close(nc->sock);
  }
  nc->sock = INVALID_SOCKET;
}

static int mg_accept_conn(struct mg_connection *lc) {
  struct mg_connection *nc;
  union socket_address sa;
  socklen_t sa_len = sizeof(sa);
  sock_t sock = sl_Accept(lc->sock, &sa.sa, &sa_len);
  if (sock < 0) {
    DBG(("%p: failed to accept: %d", lc, sock));
    return 0;
  }
  nc = mg_if_accept_new_conn(lc);
  if (nc == NULL) {
    sl_Close(sock);
    return 0;
  }
  DBG(("%p conn from %s:%d", nc, inet_ntoa(sa.sin.sin_addr),
       ntohs(sa.sin.sin_port)));
  mg_sock_set(nc, sock);
  mg_if_accept_tcp_cb(nc, &sa, sa_len);
  return 1;
}

/* 'sa' must be an initialized address to bind to */
static sock_t mg_open_listening_socket(struct mg_connection *nc,
                                       union socket_address *sa, int type,
                                       int proto) {
  int r;
  socklen_t sa_len =
      (sa->sa.sa_family == AF_INET) ? sizeof(sa->sin) : sizeof(sa->sin6);
  sock_t sock = sl_Socket(sa->sa.sa_family, type, proto);
  if (sock < 0) return sock;
#if MG_ENABLE_SSL && MG_SSL_IF == MG_SSL_IF_SIMPLELINK
  if ((r = sl_set_ssl_opts(sock, nc)) < 0) goto clean;
#endif
  if ((r = sl_Bind(sock, &sa->sa, sa_len)) < 0) goto clean;
  if (type != SOCK_DGRAM) {
    if ((r = sl_Listen(sock, SOMAXCONN)) < 0) goto clean;
  }
  mg_set_non_blocking_mode(sock);
clean:
  if (r < 0) {
    sl_Close(sock);
    sock = r;
  }
  return sock;
}

#define _MG_F_FD_CAN_READ 1
#define _MG_F_FD_CAN_WRITE 1 << 1
#define _MG_F_FD_ERROR 1 << 2

void mg_mgr_handle_conn(struct mg_connection *nc, int fd_flags, double now) {
  DBG(("%p fd=%d fd_flags=%d nc_flags=0x%lx rmbl=%d smbl=%d", nc, nc->sock,
       fd_flags, nc->flags, (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));

  if (!mg_if_poll(nc, now)) return;

  if (nc->flags & MG_F_CONNECTING) {
    if ((nc->flags & MG_F_UDP) || nc->err != SL_ERROR_BSD_EALREADY) {
      mg_if_connect_cb(nc, nc->err);
    } else {
      /* In SimpleLink, to get status of non-blocking connect() we need to wait
       * until socket is writable and repeat the call to sl_Connect again,
       * which will now return the real status. */
      if (fd_flags & _MG_F_FD_CAN_WRITE) {
        nc->err = sl_Connect(nc->sock, &nc->sa.sa, sizeof(nc->sa.sin));
        DBG(("%p conn res=%d", nc, nc->err));
        if (nc->err == SL_ERROR_BSD_ESECSNOVERIFY ||
            /* TODO(rojer): Provide API to set the date for verification. */
            nc->err == SL_ERROR_BSD_ESECDATEERROR
#if SL_MAJOR_VERSION_NUM >= 2
            /* Per SWRU455, this error does not mean verification failed,
             * it only means that the cert used is not present in the trusted
             * root CA catalog. Which is perfectly fine. */
            ||
            nc->err == SL_ERROR_BSD_ESECUNKNOWNROOTCA
#endif
            ) {
          nc->err = 0;
        }
        mg_if_connect_cb(nc, nc->err);
      }
    }
    /* Ignore read/write in further processing, we've handled it. */
    fd_flags &= ~(_MG_F_FD_CAN_READ | _MG_F_FD_CAN_WRITE);
  }

  if (fd_flags & _MG_F_FD_CAN_READ) {
    if (nc->flags & MG_F_UDP) {
      mg_if_can_recv_cb(nc);
    } else {
      if (nc->flags & MG_F_LISTENING) {
        mg_accept_conn(nc);
      } else {
        mg_if_can_recv_cb(nc);
      }
    }
  }

  if (fd_flags & _MG_F_FD_CAN_WRITE) {
    mg_if_can_send_cb(nc);
  }

  DBG(("%p after fd=%d nc_flags=0x%lx rmbl=%d smbl=%d", nc, nc->sock, nc->flags,
       (int) nc->recv_mbuf.len, (int) nc->send_mbuf.len));
}

/* Associate a socket to a connection. */
void mg_sl_if_sock_set(struct mg_connection *nc, sock_t sock) {
  mg_set_non_blocking_mode(sock);
  nc->sock = sock;
  DBG(("%p %d", nc, sock));
}

void mg_sl_if_init(struct mg_iface *iface) {
  (void) iface;
  DBG(("%p using sl_Select()", iface->mgr));
}

void mg_sl_if_free(struct mg_iface *iface) {
  (void) iface;
}

void mg_sl_if_add_conn(struct mg_connection *nc) {
  (void) nc;
}

void mg_sl_if_remove_conn(struct mg_connection *nc) {
  (void) nc;
}

time_t mg_sl_if_poll(struct mg_iface *iface, int timeout_ms) {
  struct mg_mgr *mgr = iface->mgr;
  double now = mg_time();
  double min_timer;
  struct mg_connection *nc, *tmp;
  struct SlTimeval_t tv;
  SlFdSet_t read_set, write_set, err_set;
  sock_t max_fd = INVALID_SOCKET;
  int num_fds, num_ev = 0, num_timers = 0;

  SL_SOCKET_FD_ZERO(&read_set);
  SL_SOCKET_FD_ZERO(&write_set);
  SL_SOCKET_FD_ZERO(&err_set);

  /*
   * Note: it is ok to have connections with sock == INVALID_SOCKET in the list,
   * e.g. timer-only "connections".
   */
  min_timer = 0;
  for (nc = mgr->active_connections, num_fds = 0; nc != NULL; nc = tmp) {
    tmp = nc->next;

    if (nc->sock != INVALID_SOCKET) {
      num_fds++;

      if (!(nc->flags & MG_F_WANT_WRITE) &&
          nc->recv_mbuf.len < nc->recv_mbuf_limit &&
          (!(nc->flags & MG_F_UDP) || nc->listener == NULL)) {
        SL_SOCKET_FD_SET(nc->sock, &read_set);
        if (max_fd == INVALID_SOCKET || nc->sock > max_fd) max_fd = nc->sock;
      }

      if (((nc->flags & MG_F_CONNECTING) && !(nc->flags & MG_F_WANT_READ)) ||
          (nc->send_mbuf.len > 0 && !(nc->flags & MG_F_CONNECTING))) {
        SL_SOCKET_FD_SET(nc->sock, &write_set);
        SL_SOCKET_FD_SET(nc->sock, &err_set);
        if (max_fd == INVALID_SOCKET || nc->sock > max_fd) max_fd = nc->sock;
      }
    }

    if (nc->ev_timer_time > 0) {
      if (num_timers == 0 || nc->ev_timer_time < min_timer) {
        min_timer = nc->ev_timer_time;
      }
      num_timers++;
    }
  }

  /*
   * If there is a timer to be fired earlier than the requested timeout,
   * adjust the timeout.
   */
  if (num_timers > 0) {
    double timer_timeout_ms = (min_timer - mg_time()) * 1000 + 1 /* rounding */;
    if (timer_timeout_ms < timeout_ms) {
      timeout_ms = timer_timeout_ms;
    }
  }
  if (timeout_ms < 0) timeout_ms = 0;

  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (timeout_ms % 1000) * 1000;

  if (num_fds > 0) {
    num_ev = sl_Select((int) max_fd + 1, &read_set, &write_set, &err_set, &tv);
  }

  now = mg_time();
  DBG(("sl_Select @ %ld num_ev=%d of %d, timeout=%d", (long) now, num_ev,
       num_fds, timeout_ms));

  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    int fd_flags = 0;
    if (nc->sock != INVALID_SOCKET) {
      if (num_ev > 0) {
        fd_flags =
            (SL_SOCKET_FD_ISSET(nc->sock, &read_set) &&
                     (!(nc->flags & MG_F_UDP) || nc->listener == NULL)
                 ? _MG_F_FD_CAN_READ
                 : 0) |
            (SL_SOCKET_FD_ISSET(nc->sock, &write_set) ? _MG_F_FD_CAN_WRITE
                                                      : 0) |
            (SL_SOCKET_FD_ISSET(nc->sock, &err_set) ? _MG_F_FD_ERROR : 0);
      }
      /* SimpleLink does not report UDP sockets as writable. */
      if (nc->flags & MG_F_UDP && nc->send_mbuf.len > 0) {
        fd_flags |= _MG_F_FD_CAN_WRITE;
      }
    }
    tmp = nc->next;
    mg_mgr_handle_conn(nc, fd_flags, now);
  }

  return now;
}

void mg_sl_if_get_conn_addr(struct mg_connection *nc, int remote,
                            union socket_address *sa) {
  /* SimpleLink does not provide a way to get socket's peer address after
   * accept or connect. Address should have been preserved in the connection,
   * so we do our best here by using it. */
  if (remote) memcpy(sa, &nc->sa, sizeof(*sa));
}

void sl_restart_cb(struct mg_mgr *mgr) {
  /*
   * SimpleLink has been restarted, meaning all sockets have been invalidated.
   * We try our best - we'll restart the listeners, but for outgoing
   * connections we have no option but to terminate.
   */
  struct mg_connection *nc;
  for (nc = mg_next(mgr, NULL); nc != NULL; nc = mg_next(mgr, nc)) {
    if (nc->sock == INVALID_SOCKET) continue; /* Could be a timer */
    if (nc->flags & MG_F_LISTENING) {
      DBG(("restarting %p %s:%d", nc, inet_ntoa(nc->sa.sin.sin_addr),
           ntohs(nc->sa.sin.sin_port)));
      int res = (nc->flags & MG_F_UDP ? mg_sl_if_listen_udp(nc, &nc->sa)
                                      : mg_sl_if_listen_tcp(nc, &nc->sa));
      if (res == 0) continue;
      /* Well, we tried and failed. Fall through to closing. */
    }
    nc->sock = INVALID_SOCKET;
    DBG(("terminating %p %s:%d", nc, inet_ntoa(nc->sa.sin.sin_addr),
         ntohs(nc->sa.sin.sin_port)));
    /* TODO(rojer): Outgoing UDP? */
    nc->flags |= MG_F_CLOSE_IMMEDIATELY;
  }
}

/* clang-format off */
#define MG_SL_IFACE_VTABLE                                              \
  {                                                                     \
    mg_sl_if_init,                                                      \
    mg_sl_if_free,                                                      \
    mg_sl_if_add_conn,                                                  \
    mg_sl_if_remove_conn,                                               \
    mg_sl_if_poll,                                                      \
    mg_sl_if_listen_tcp,                                                \
    mg_sl_if_listen_udp,                                                \
    mg_sl_if_connect_tcp,                                               \
    mg_sl_if_connect_udp,                                               \
    mg_sl_if_tcp_send,                                                  \
    mg_sl_if_udp_send,                                                  \
    mg_sl_if_tcp_recv,                                                  \
    mg_sl_if_udp_recv,                                                  \
    mg_sl_if_create_conn,                                               \
    mg_sl_if_destroy_conn,                                              \
    mg_sl_if_sock_set,                                                  \
    mg_sl_if_get_conn_addr,                                             \
  }
/* clang-format on */

const struct mg_iface_vtable mg_simplelink_iface_vtable = MG_SL_IFACE_VTABLE;
#if MG_NET_IF == MG_NET_IF_SIMPLELINK
const struct mg_iface_vtable mg_default_iface_vtable = MG_SL_IFACE_VTABLE;
#endif

#endif /* MG_ENABLE_NET_IF_SIMPLELINK */
