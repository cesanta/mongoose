#include "bsd.h"
#include "dns.h"
#include <stdarg.h>

#if MG_ENABLE_BSD_SOCKETS && MG_ENABLE_FREERTOS
#include <queue.h>

static QueueHandle_t s_cmd_q;
static struct mg_bsd_cmd *s_waiters;

static bool bsd_qsend(QueueHandle_t q, const void *item, TickType_t ticks) {
  if (xQueueSend(q, item, ticks) == pdTRUE) return true;
  MG_ERROR(("queue send failed %p", q));
  return false;
}
#endif

#if MG_ENABLE_BSD_SOCKETS
static struct mg_mgr *s_mgr;

#ifndef MG_BSD_FD_BASE
#define MG_BSD_FD_BASE 11
#endif
#ifndef MG_BSD_MAX_SOCKETS
#define MG_BSD_MAX_SOCKETS 64
#endif
#ifndef MG_BSD_ACCEPT_MS
#define MG_BSD_ACCEPT_MS 3000
#endif

// clang-format off
enum bsd_op {
  BSD_OP_NONE, BSD_OP_SOCKET, BSD_OP_BIND, BSD_OP_LISTEN, BSD_OP_FCNTL,
  BSD_OP_ACCEPT, BSD_OP_CONNECT, BSD_OP_RECV, BSD_OP_SEND, BSD_OP_SENDTO,
  BSD_OP_RECVFROM, BSD_OP_CLOSE, BSD_OP_POLL, BSD_OP_SELECT,
  BSD_OP_GETHOSTBYNAME,
};

// A BSD API request from a user task. Gets sent via the RTOS queue, and
// executed by the network task (mg_bsd_poll). Baremetal code calls directly
struct mg_bsd_cmd {
  int result;                // return value
  int err;                   // errno
  enum bsd_op op;
  bool started;              // operation initiated (connect/resolve)
  void *caller;              // task to notify when done
  struct mg_bsd_cmd *next;   // waiter list link
  union {                    // op-specific arguments
    struct { int domain, type, proto; } op_socket;
    struct { int fd, backlog; } op_listen;
    struct { int fd, cmd, arg; } op_fcntl;
    struct { int fd; } op_close;
    struct { int fd; const struct sockaddr *addr; socklen_t len; } op_bind;
    struct { int fd; const struct sockaddr *addr; socklen_t len; } op_connect;
    struct { int fd; struct sockaddr *addr; socklen_t *addrlen; } op_accept;
    struct { int fd; void *buf; size_t len; int flags; } op_recv;
    struct { int fd; const void *buf; size_t len; int flags; } op_send;
    struct { int fd; const void *buf; size_t len; int flags; const struct sockaddr *addr; socklen_t addrlen; } op_sendto;
    struct { int fd; void *buf; size_t len; int flags; struct sockaddr *src; socklen_t *addrlen; } op_recvfrom;
    struct { struct pollfd *fds; nfds_t nfds; uint64_t deadline; } op_poll;
    struct { int nfds; fd_set *r, *w, *e; uint64_t deadline; } op_select;
    struct { const char *name; struct hostent *result; } op_gethostbyname;
  };
};
#define BSD_CMD_EMPTY {0, 0, BSD_OP_NONE, 0, 0, 0, {{0, 0, 0}}}
// clang-format on

// The BSD socket outlives its Mongoose connection until the caller closes fd.
struct mg_bsd_sock {
  int fd, err, backlog;
  uint64_t accept_expire;
  struct sockaddr_in addr;
  struct mg_connection *c;
  struct mg_bsd_sock *listener;
  bool used, bound, closed, nonblock, is_udp, orphan;  // closed: c was closed
};

static struct mg_bsd_sock s_socks[MG_BSD_MAX_SOCKETS > 0 ?
                                  MG_BSD_MAX_SOCKETS : 1];

static void bsd_ev(struct mg_connection *, int, void *);

// BSD-owned fd registry
static struct mg_bsd_sock *bsd_get(int fd) {
  int offset;
  struct mg_bsd_sock *s;

  if (fd < MG_BSD_FD_BASE) return NULL;
  offset = fd - MG_BSD_FD_BASE;
  if (offset >= MG_BSD_MAX_SOCKETS) return NULL;
  s = &s_socks[offset];
  if (!s->used || s->orphan) return NULL;
  return s;
}

static void bsd_attach(struct mg_bsd_sock *s, struct mg_connection *c) {
  s->c = c;
  c->fn = bsd_ev;
  c->fn_data = s;
  c->is_bsd = 1;
  c->is_udp = s->is_udp;
}

static struct mg_connection *bsd_new_conn(struct mg_bsd_sock *s) {
  struct mg_connection *c = mg_alloc_conn(s_mgr);
  if (c != NULL) {
    bsd_attach(s, c);
    LIST_ADD_HEAD(struct mg_connection, &s_mgr->conns, c);
  }
  return c;
}

#if MG_ENABLE_FREERTOS
// Complete operations blocked on c before Mongoose destroys it.
static void bsd_finish_waiters(struct mg_bsd_sock *s, bool error) {
  struct mg_bsd_cmd **p = &s_waiters;

  if (s->orphan) return;  // No BSD socket owner yet

  while (*p != NULL) {
    struct mg_bsd_cmd *cmd = *p;
    int fd = -1;
    bool read = cmd->op == BSD_OP_RECV || cmd->op == BSD_OP_RECVFROM;
    bool send = cmd->op == BSD_OP_SEND || cmd->op == BSD_OP_SENDTO;

    switch (cmd->op) {  // clang-format off
      case BSD_OP_ACCEPT: fd = cmd->op_accept.fd; break;
      case BSD_OP_CONNECT: fd = cmd->op_connect.fd; break;
      case BSD_OP_RECV: fd = cmd->op_recv.fd; break;
      case BSD_OP_SEND: fd = cmd->op_send.fd; break;
      case BSD_OP_SENDTO: fd = cmd->op_sendto.fd; break;
      case BSD_OP_RECVFROM: fd = cmd->op_recvfrom.fd; break;
      default: break;
    }  // clang-format on
    if (fd != s->fd) {
      p = &cmd->next;
      continue;
    }

    *p = cmd->next;
    if (!error && s->c != NULL && s->c->is_draining && read) {
      cmd->result = 0;  // Orderly EOF
    } else {
      cmd->result = -1;
      cmd->err = send ? EPIPE : EIO;  // Mongoose error / forced close
    }
    xTaskNotifyGive((TaskHandle_t) cmd->caller);
  }
}
#endif

static void bsd_ev(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_bsd_sock *s = (struct mg_bsd_sock *) c->fn_data;
  if (s == NULL) return;

  if (ev == MG_EV_ACCEPT) {
    struct mg_bsd_sock *child = NULL, *t;
    int count = 0;
    for (t = s_socks; t < s_socks + MG_BSD_MAX_SOCKETS; t++) {
      if (!t->used && child == NULL) child = t;  // Find a free BSD socket slot,
      if (t->orphan && t->listener == s) count++;  // and count pending accepts
    }
    if (count >= s->backlog) {
      c->fn_data = NULL;
      mg_error(c, "accept backlog full");
    } else if (child == NULL) {
      c->fn_data = NULL;
      mg_error(c, "accept rejected");
    } else {
      memset(child, 0, sizeof(*child));  // Claim slot
      child->used = true;
      child->c = c;
      child->listener = s;
      child->accept_expire = mg_millis() + MG_BSD_ACCEPT_MS;
      child->is_udp = s->is_udp;
      child->orphan = true;
      c->fn_data = child;
      c->is_bsd = 1;
      c->is_udp = child->is_udp;
    }
  } else if (ev == MG_EV_ERROR) {
    if (s->err == 0) s->err = EIO;
#if MG_ENABLE_FREERTOS
    bsd_finish_waiters(s, true);
#endif
  } else if (ev == MG_EV_CLOSE) {
#if MG_ENABLE_FREERTOS
    bsd_finish_waiters(s, false);
#endif
    s->c = NULL;
    s->closed = true;
    c->fn_data = NULL;
    if (s->orphan) memset(s, 0, sizeof(*s)); // release
  } else if (ev == MG_EV_POLL && s->orphan &&
             mg_millis() > s->accept_expire) {
    mg_error(c, "accept timeout");
  }
  (void) ev_data;
}

#if MG_ENABLE_FREERTOS
#ifndef MG_WAKEUP_QUEUE_DEPTH
#define MG_WAKEUP_QUEUE_DEPTH 4
#endif

struct wumsg {
  unsigned long id;
  size_t len;
  uint8_t data[];
};

static void wufn(struct mg_connection *c, int ev, void *ev_data) {
  QueueHandle_t q = (QueueHandle_t) c->mgr->pipe.q;
  if (ev == MG_EV_POLL) {
    struct wumsg *m;
    if (xQueueReceive(q, &m, 0) == pdTRUE) {
      struct mg_connection *t;
      for (t = c->mgr->conns; t != NULL; t = t->next) {
        if (t->id == m->id) {
          struct mg_str data = mg_str_n((char *) m->data, m->len);
          mg_call(t, MG_EV_WAKEUP, &data);
          break;
        }
      }
      free(m);
    }
  } else if (ev == MG_EV_CLOSE) {
    struct wumsg *m;
    while (xQueueReceive(q, &m, 0) == pdTRUE) free(m);
    vQueueDelete(q);
    c->mgr->pipe.q = NULL;
  }
  (void) ev_data;
}

bool mg_wakeup_init(struct mg_mgr *mgr) {
  struct mg_connection *c;
  if (mgr->pipe.q != NULL) return true;
  mgr->pipe.q = xQueueCreate(MG_WAKEUP_QUEUE_DEPTH, sizeof(void *));
  if (mgr->pipe.q == NULL) {
    MG_ERROR(("Cannot create queue"));
    return false;
  }
  c = mg_alloc_conn(mgr);
  if (c == NULL) {
    vQueueDelete((QueueHandle_t) mgr->pipe.q);
    mgr->pipe.q = NULL;
    return false;
  }
  c->fd = (void *) (size_t) MG_INVALID_SOCKET;
  c->fn = wufn;
  LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
  MG_DEBUG(("%lu queue %p", c->id, mgr->pipe.q));
  mg_call(c, MG_EV_OPEN, NULL);
  return true;
}

bool mg_wakeup(struct mg_mgr *mgr, unsigned long conn_id, const void *buf,
               size_t len) {
  struct wumsg *m;
  if (mgr->pipe.q == NULL || conn_id == 0) return false;
  m = (struct wumsg *) calloc(1, sizeof(*m) + len);
  if (m == NULL) {
    MG_ERROR(("OOM"));
    return false;
  }
  m->id = conn_id;
  m->len = len;
  memcpy(m->data, buf, len);
  if (!bsd_qsend((QueueHandle_t) mgr->pipe.q, &m, 0)) {
    free(m);
    return false;
  }
  return true;
}
#endif

enum { BSD_READ = 1, BSD_WRITE = 2, BSD_ERROR = 4 };

static int bsd_ready(struct mg_bsd_sock *s) {
  struct mg_bsd_sock *t;
  int ready = 0;

  if (s->err != 0) return BSD_READ | BSD_ERROR;
  if (s->c == NULL) return s->closed ? BSD_READ | BSD_ERROR : 0;
  if (s->c->is_listening) {
    for (t = s_socks; t < s_socks + MG_BSD_MAX_SOCKETS; t++) {
      if (t->orphan && t->listener == s) return BSD_READ;
    }
    return 0;
  }
  if (s->c->recv.len > 0 || s->c->is_draining) ready |= BSD_READ;
  if (!s->is_udp && !s->c->is_connecting && !s->c->is_closing &&
      !s->c->is_draining)
    ready |= BSD_WRITE;  // Established TCP socket
  return ready;
}

static void bsd_close(struct mg_bsd_sock *s) {
  struct mg_bsd_sock *t;

#if MG_ENABLE_FREERTOS
  bsd_finish_waiters(s, true);
#endif
  for (t = s_socks; t < s_socks + MG_BSD_MAX_SOCKETS; t++) {
    if (t->orphan && t->listener == s) {
      if (t->c != NULL) {
        t->c->fn_data = NULL;
        mg_close_conn(t->c);
        t->c = NULL;
      }
      memset(t, 0, sizeof(*t));  // Release orphan
    }
  }
  if (s->c != NULL) {
    s->c->fn_data = NULL;
    mg_close_conn(s->c);
    s->c = NULL;
  }
  memset(s, 0, sizeof(*s));  // Release socket
}

static char s_hostent_name[64];
static uint32_t s_hostent_addr;
static char *s_hostent_addr_list[2] = {(char *) &s_hostent_addr, NULL};
static struct hostent s_hostent = {s_hostent_name, NULL, AF_INET, 4,
                                   s_hostent_addr_list};
static volatile int s_resolve_done;

static void bsd_resolve_fn(struct mg_connection *c, int ev, void *ev_data) {
  (void) ev_data;
  if (ev == MG_EV_RESOLVE) {
    s_hostent_addr = c->rem.addr.ip4;
    s_resolve_done = 1;
    c->is_closing = 1;
  } else if (ev == MG_EV_ERROR) {
    s_resolve_done = -1;
  }
}

// Execute a command. Runs in the network task (mg_bsd_poll) on FreeRTOS, or
// in the caller otherwise. Returns true when done, false when the operation
// must wait for the network task and be re-checked later.
static bool bsd_run(struct mg_bsd_cmd *cmd) {
  struct mg_connection *c;
  struct mg_bsd_sock *s;
  switch (cmd->op) {
    case BSD_OP_NONE:
      cmd->result = -1;
      return true;
    case BSD_OP_SOCKET:
      if (s_mgr == NULL || cmd->op_socket.domain != AF_INET ||
          (cmd->op_socket.type != SOCK_STREAM &&
           cmd->op_socket.type != SOCK_DGRAM)) {
        cmd->result = -1, cmd->err = EPERM;
      } else {
        for (s = s_socks; s < s_socks + MG_BSD_MAX_SOCKETS; s++) {
          if (!s->used) break;  // Find a free BSD socket registry slot
        }
        if (s == s_socks + MG_BSD_MAX_SOCKETS) {
          cmd->result = -1, cmd->err = EMFILE;
        } else {
          memset(s, 0, sizeof(*s));  // Claim slot
          s->used = true;
          s->is_udp = cmd->op_socket.type == SOCK_DGRAM;
          s->fd = MG_BSD_FD_BASE + (int) (s - s_socks);
          cmd->result = s->fd;
        }
      }
      return true;
    case BSD_OP_BIND:
      if ((s = bsd_get(cmd->op_bind.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else {
        const struct sockaddr_in *sa =
            (const struct sockaddr_in *) cmd->op_bind.addr;
        memcpy(&s->addr, sa, sizeof(s->addr));
        s->bound = true;
        if (!s->is_udp) {
          cmd->result = 0;
        } else if (s->c == NULL && (c = bsd_new_conn(s)) == NULL) {
          cmd->result = -1, cmd->err = ENOMEM;
        } else {
          c = s->c;
          c->loc.port = s->addr.sin_port;
          c->loc.addr.ip4 = s->addr.sin_addr.s_addr;
          c->loc.is_ip6 = false;
          cmd->result = 0;
        }
      }
      return true;
    case BSD_OP_LISTEN: {
      char url[32];

      s = bsd_get(cmd->op_listen.fd);
      if (s == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else if (!s->bound || s->is_udp || cmd->op_listen.backlog < 1) {
        cmd->result = -1, cmd->err = EINVAL;
      } else if (s->c != NULL && !s->c->is_listening) {
        cmd->result = -1, cmd->err = EINVAL;
      } else if (s->c == NULL) {
        mg_snprintf(url, sizeof(url), "tcp://%M:%hu", mg_print_ip4,
                    &s->addr.sin_addr.s_addr, mg_ntohs(s->addr.sin_port));
        if ((c = mg_listen(s_mgr, url, bsd_ev, s)) == NULL) {
          cmd->result = -1, cmd->err = ENOMEM;
        } else {
          bsd_attach(s, c);
          s->backlog = cmd->op_listen.backlog;
          cmd->result = 0;
        }
      } else {
        s->backlog = cmd->op_listen.backlog;
        cmd->result = 0;
      }
      return true;
    }
    case BSD_OP_FCNTL:
      if ((s = bsd_get(cmd->op_fcntl.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else if (cmd->op_fcntl.cmd == F_GETFL) {
        cmd->result = s->nonblock ? O_NONBLOCK : 0;
      } else if (cmd->op_fcntl.cmd == F_SETFL) {
        s->nonblock = (cmd->op_fcntl.arg & O_NONBLOCK) != 0;
        cmd->result = 0;
      } else {
        cmd->result = -1, cmd->err = EINVAL;
      }
      return true;
    case BSD_OP_ACCEPT: {
      int i;
      struct mg_bsd_sock *child;

      s = bsd_get(cmd->op_accept.fd);
      if (s == NULL || s->c == NULL || !s->c->is_listening) {
        cmd->result = -1, cmd->err = EINVAL;
        return true;
      }
      for (i = 0, child = s_socks; i < MG_BSD_MAX_SOCKETS; i++, child++) {
        if (child->orphan && child->listener == s) {
          c = child->c;
          child->orphan = false;
          child->listener = NULL;
          child->fd = MG_BSD_FD_BASE + i;
          cmd->result = child->fd;
          if (cmd->op_accept.addr != NULL && cmd->op_accept.addrlen != NULL) {
            struct sockaddr_in *sa = (struct sockaddr_in *) cmd->op_accept.addr;
            sa->sin_family = AF_INET;
            sa->sin_port = c->rem.port;
            sa->sin_addr.s_addr = c->rem.addr.ip4;
            *cmd->op_accept.addrlen = sizeof(*sa);
          }
          return true;
        }
      }
      if (s->nonblock) {
        cmd->result = -1, cmd->err = EAGAIN;
        return true;
      }
#if MG_ENABLE_FREERTOS
      return false;  // No connection yet, wait
#else
      cmd->result = -1, cmd->err = EAGAIN;
      return true;
#endif
    }
    case BSD_OP_CONNECT:
      if ((s = bsd_get(cmd->op_connect.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
        return true;
      }
      if (!cmd->started && s->c == NULL) {
        if (s->closed || (c = bsd_new_conn(s)) == NULL) {
          cmd->result = -1, cmd->err = s->closed ? EIO : ENOMEM;
          return true;
        }
      }
      if ((c = s->c) == NULL) {
        cmd->result = -1, cmd->err = s->err ? s->err : EIO;
        return true;
      }
      if (!cmd->started) {
        const struct sockaddr_in *sa =
            (const struct sockaddr_in *) cmd->op_connect.addr;
        c->rem.port = sa->sin_port;
        c->rem.addr.ip4 = sa->sin_addr.s_addr;
        c->rem.is_ip6 = false;
        c->is_client = 1;
        mg_connect_resolved(c);
        cmd->started = true;
      }
      if (c->is_closing) {
        cmd->result = -1, cmd->err = ECONNREFUSED;
        return true;
      }
      if (!c->is_connecting) {
        cmd->result = 0;
        return true;
      }
      if (s->nonblock) {
        cmd->result = -1, cmd->err = EINPROGRESS;
        return true;
      }
#if MG_ENABLE_FREERTOS
      return false;  // Still connecting, wait
#else
      cmd->result = -1, cmd->err = EINPROGRESS;
      return true;
#endif
    case BSD_OP_RECV:
      if ((s = bsd_get(cmd->op_recv.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
        return true;
      }
      if ((c = s->c) == NULL) {
        cmd->result = s->closed && s->err == 0 ? 0 : -1;
        cmd->err = s->closed ? s->err : ENOTCONN;
        return true;
      }
      if (c->recv.len > 0) {
        size_t n = cmd->op_recv.len;
        if (c->recv.len < n) n = c->recv.len;
        memcpy(cmd->op_recv.buf, c->recv.buf, n);
        if (!(cmd->op_recv.flags & MSG_PEEK)) mg_iobuf_del(&c->recv, 0, n);
        cmd->result = (int) n;
        return true;
      }
      if (s->err != 0) {
        cmd->result = -1, cmd->err = s->err;
        return true;
      }
      if (c->is_draining) {  // Orderly shutdown by peer
        cmd->result = 0;
        return true;
      }
      if (!s->nonblock && !(cmd->op_recv.flags & MSG_DONTWAIT)) {
#if MG_ENABLE_FREERTOS
        return false;  // No data yet, wait
#endif
      }
      cmd->result = -1, cmd->err = EAGAIN;
      return true;
    case BSD_OP_SEND:
      if ((s = bsd_get(cmd->op_send.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else if ((c = s->c) == NULL || s->err != 0 || c->is_closing ||
                 c->is_draining) {
        cmd->result = -1, cmd->err = EPIPE;
      } else if (mg_send(c, cmd->op_send.buf, cmd->op_send.len)) {
        cmd->result = (int) cmd->op_send.len;
      } else if (s->nonblock || (cmd->op_send.flags & MSG_DONTWAIT)) {
        cmd->result = -1, cmd->err = EAGAIN;
      } else {
#if MG_ENABLE_FREERTOS
        return false;  // No send buffer space yet, wait
#else
        cmd->result = -1, cmd->err = EAGAIN;
#endif
      }
      return true;
    case BSD_OP_SENDTO:
      if ((s = bsd_get(cmd->op_sendto.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else if ((c = s->c) == NULL || s->err != 0 || c->is_closing ||
                 c->is_draining) {
        cmd->result = -1, cmd->err = EPIPE;
      } else {
        const struct sockaddr_in *sa =
            (const struct sockaddr_in *) cmd->op_sendto.addr;
        c->rem.port = sa->sin_port;
        c->rem.addr.ip4 = sa->sin_addr.s_addr;
        c->rem.is_ip6 = false;
        if (mg_send(c, cmd->op_sendto.buf, cmd->op_sendto.len)) {
          cmd->result = (int) cmd->op_sendto.len;
        } else if (s->nonblock || (cmd->op_sendto.flags & MSG_DONTWAIT)) {
          cmd->result = -1, cmd->err = EAGAIN;
        } else {
#if MG_ENABLE_FREERTOS
          return false;  // No send buffer space yet, wait
#else
          cmd->result = -1, cmd->err = EAGAIN;
#endif
        }
      }
      return true;
    case BSD_OP_RECVFROM:
      if ((s = bsd_get(cmd->op_recvfrom.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
        return true;
      }
      if ((c = s->c) == NULL) {
        cmd->result = s->closed && s->err == 0 ? 0 : -1;
        cmd->err = s->closed ? s->err : ENOTCONN;
        return true;
      }
      if (c->recv.len > 0) {
        size_t n = cmd->op_recvfrom.len;
        if (c->recv.len < n) n = c->recv.len;
        memcpy(cmd->op_recvfrom.buf, c->recv.buf, n);
        if (!(cmd->op_recvfrom.flags & MSG_PEEK)) mg_iobuf_del(&c->recv, 0, n);
        if (cmd->op_recvfrom.src != NULL && cmd->op_recvfrom.addrlen != NULL) {
          struct sockaddr_in *sa = (struct sockaddr_in *) cmd->op_recvfrom.src;
          sa->sin_family = AF_INET;
          sa->sin_port = c->rem.port;
          sa->sin_addr.s_addr = c->rem.addr.ip4;
          *cmd->op_recvfrom.addrlen = sizeof(*sa);
        }
        cmd->result = (int) n;
        return true;
      }
      if (s->err != 0) {
        cmd->result = -1, cmd->err = s->err;
        return true;
      }
      if (!s->nonblock && !(cmd->op_recvfrom.flags & MSG_DONTWAIT)) {
#if MG_ENABLE_FREERTOS
        return false;  // No datagram yet, wait
#endif
      }
      cmd->result = -1, cmd->err = EAGAIN;
      return true;
    case BSD_OP_CLOSE:
      if ((s = bsd_get(cmd->op_close.fd)) == NULL) {
        cmd->result = -1, cmd->err = EBADF;
      } else {
        bsd_close(s);
        cmd->result = 0;
      }
      return true;
    case BSD_OP_POLL: {
      struct pollfd *p;
      int i, n = 0, ready;

      for (i = 0, p = cmd->op_poll.fds; i < (int) cmd->op_poll.nfds;
           i++, p++) {
        p->revents = 0;
        if (p->fd < 0) continue;
        if ((s = bsd_get(p->fd)) == NULL) {
          p->revents = POLLNVAL;
        } else {
          ready = bsd_ready(s);
          if ((p->events & POLLIN) && (ready & BSD_READ))
            p->revents |= POLLIN;
          if ((p->events & POLLOUT) && (ready & BSD_WRITE))
            p->revents |= POLLOUT;
          if (s->err != 0) p->revents |= POLLERR;
          if (s->closed) p->revents |= POLLHUP;
        }
        if (p->revents != 0) n++;
      }
#if MG_ENABLE_FREERTOS
      if (n == 0 && mg_millis() < cmd->op_poll.deadline)
        return false;  // Keep caller pollfd[] intact while waiting
#endif
      cmd->result = n;
      return true;
    }
    case BSD_OP_SELECT: {
      fd_set rr, ww, ee;
      fd_set *r = cmd->op_select.r;
      fd_set *w = cmd->op_select.w;
      fd_set *e = cmd->op_select.e;
      int fd, n = 0, want_read, want_write, want_error, ready;
#if MG_ENABLE_BSD_PROTOTYPES
      int i;
      __fd_mask bits;
      int bitsize = (int) (sizeof(bits) * 8);
#endif

      if (cmd->op_select.nfds < 0 || cmd->op_select.nfds > FD_SETSIZE) {
        cmd->result = -1, cmd->err = EINVAL;
        return true;
      }
      FD_ZERO(&rr);
      FD_ZERO(&ww);
      FD_ZERO(&ee);
#if MG_ENABLE_BSD_PROTOTYPES
      for (i = 0; i < (cmd->op_select.nfds + bitsize - 1) / bitsize; i++) {
        bits = 0;
        if (r != NULL) bits |= r->__fds_bits[i];
        if (w != NULL) bits |= w->__fds_bits[i];
        if (e != NULL) bits |= e->__fds_bits[i];
        while (bits != 0) {
          fd = i * bitsize + (int) __builtin_ctzl(bits);
          bits &= bits - 1;
          if (fd >= cmd->op_select.nfds) continue;
          want_read = r != NULL && FD_ISSET(fd, r);
          want_write = w != NULL && FD_ISSET(fd, w);
          want_error = e != NULL && FD_ISSET(fd, e);
          if ((s = bsd_get(fd)) == NULL) goto err;
          ready = bsd_ready(s);
          if (want_read && (ready & BSD_READ)) FD_SET(fd, &rr);
          if (want_write && (ready & BSD_WRITE)) FD_SET(fd, &ww);
          if (want_error && (ready & BSD_ERROR)) FD_SET(fd, &ee);
          if ((want_read && (ready & BSD_READ)) ||
              (want_write && (ready & BSD_WRITE)) ||
              (want_error && (ready & BSD_ERROR)))
            n++;
        }
      }
#else
      for (fd = 0; fd < cmd->op_select.nfds; fd++) {
        want_read = r != NULL && FD_ISSET(fd, r);
        want_write = w != NULL && FD_ISSET(fd, w);
        want_error = e != NULL && FD_ISSET(fd, e);
        if (!want_read && !want_write && !want_error) continue;
        if ((s = bsd_get(fd)) == NULL) goto err;
        ready = bsd_ready(s);
        if (want_read && (ready & BSD_READ)) FD_SET(fd, &rr);
        if (want_write && (ready & BSD_WRITE)) FD_SET(fd, &ww);
        if (want_error && (ready & BSD_ERROR)) FD_SET(fd, &ee);
        if ((want_read && (ready & BSD_READ)) ||
            (want_write && (ready & BSD_WRITE)) ||
            (want_error && (ready & BSD_ERROR)))
          n++;
      }
#endif
#if MG_ENABLE_FREERTOS
      if (n == 0 && mg_millis() < cmd->op_select.deadline)
        return false;  // Keep fd sets while waiting
#endif
      if (r != NULL) *r = rr;
      if (w != NULL) *w = ww;
      if (e != NULL) *e = ee;
      cmd->result = n;
      return true;

err:
      cmd->result = -1, cmd->err = EBADF;
      return true;
    }
    case BSD_OP_GETHOSTBYNAME:
      if (!cmd->started) {
        struct mg_connection *r = mg_alloc_conn(s_mgr);
        if (r == NULL) {
          cmd->op_gethostbyname.result = NULL;
          return true;
        }
        r->fn = bsd_resolve_fn;
        LIST_ADD_HEAD(struct mg_connection, &s_mgr->conns, r);
        mg_call(r, MG_EV_OPEN, NULL);
        s_resolve_done = 0;
        mg_resolve(r, cmd->op_gethostbyname.name);
        cmd->started = true;
#if MG_ENABLE_FREERTOS
        return false;  // Wait for DNS response
#endif
      }
      if (s_resolve_done != 0) {
        if (s_resolve_done < 0) {
          cmd->op_gethostbyname.result = NULL;
        } else {
          mg_snprintf(s_hostent_name, sizeof(s_hostent_name), "%s",
                      cmd->op_gethostbyname.name);
          cmd->op_gethostbyname.result = &s_hostent;
        }
        return true;
      }
#if MG_ENABLE_FREERTOS
      return false;  // Still resolving
#else
      cmd->op_gethostbyname.result = NULL;
      return true;
#endif
  }
  return true;
}

static void bsd_dispatch(struct mg_bsd_cmd *cmd) {
#if MG_ENABLE_FREERTOS
  if (s_cmd_q == NULL) {
    cmd->result = -1, cmd->err = EPERM;
  } else {
    cmd->caller = xTaskGetCurrentTaskHandle(); 
    if (!bsd_qsend(s_cmd_q, &cmd, portMAX_DELAY)) {
      cmd->result = -1, cmd->err = EIO;
    } else {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
  }
#else
  bsd_run(cmd);
#endif
}

int socket(int domain, int type, int proto) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_SOCKET, cmd.op_socket.domain = domain,
  cmd.op_socket.type = type, cmd.op_socket.proto = proto;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int bind(int fd, const struct sockaddr *addr, socklen_t len) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_BIND, cmd.op_bind.fd = fd, cmd.op_bind.addr = addr,
  cmd.op_bind.len = len;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int listen(int fd, int backlog) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_LISTEN, cmd.op_listen.fd = fd,
  cmd.op_listen.backlog = backlog;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int fcntl(int fd, int fcmd, ...) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  int arg = 0;

  if (fcmd == F_SETFL) {
    va_list ap;

    va_start(ap, fcmd);
    arg = va_arg(ap, int);
    va_end(ap);
  }
  cmd.op = BSD_OP_FCNTL, cmd.op_fcntl.fd = fd, cmd.op_fcntl.cmd = fcmd,
  cmd.op_fcntl.arg = arg;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int accept(int fd, struct sockaddr *addr, socklen_t *addrlen) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_ACCEPT, cmd.op_accept.fd = fd, cmd.op_accept.addr = addr,
  cmd.op_accept.addrlen = addrlen;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int connect(int fd, const struct sockaddr *addr, socklen_t len) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_CONNECT, cmd.op_connect.fd = fd, cmd.op_connect.addr = addr,
  cmd.op_connect.len = len;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

ssize_t recv(int fd, void *buf, size_t len, int flags) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_RECV, cmd.op_recv.fd = fd, cmd.op_recv.buf = buf,
  cmd.op_recv.len = len, cmd.op_recv.flags = flags;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

ssize_t send(int fd, const void *buf, size_t len, int flags) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_SEND, cmd.op_send.fd = fd, cmd.op_send.buf = (void *) buf,
  cmd.op_send.len = len, cmd.op_send.flags = flags;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

ssize_t sendto(int fd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest, socklen_t addrlen) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_SENDTO, cmd.op_sendto.fd = fd,
  cmd.op_sendto.buf = (void *) buf, cmd.op_sendto.len = len,
  cmd.op_sendto.flags = flags, cmd.op_sendto.addr = dest,
  cmd.op_sendto.addrlen = addrlen;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

ssize_t recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *src,
                 socklen_t *addrlen) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_RECVFROM, cmd.op_recvfrom.fd = fd, cmd.op_recvfrom.buf = buf,
  cmd.op_recvfrom.len = len, cmd.op_recvfrom.flags = flags,
  cmd.op_recvfrom.src = src, cmd.op_recvfrom.addrlen = addrlen;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int close(int fd) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_CLOSE, cmd.op_close.fd = fd;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  uint64_t deadline = timeout < 0 ? (uint64_t) ~0
                                  : mg_millis() + (uint64_t) timeout;

  cmd.op = BSD_OP_POLL, cmd.op_poll.fds = fds, cmd.op_poll.nfds = nfds,
  cmd.op_poll.deadline = deadline;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

int select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tv) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  uint64_t deadline = tv == NULL ? (uint64_t) ~0
                                 : mg_millis() + (uint64_t) tv->tv_sec * 1000 +
                                       tv->tv_usec / 1000;
  cmd.op = BSD_OP_SELECT, cmd.op_select.nfds = nfds, cmd.op_select.r = r,
  cmd.op_select.w = w, cmd.op_select.e = e, cmd.op_select.deadline = deadline;
  bsd_dispatch(&cmd);
  errno = cmd.err;
  return cmd.result;
}

struct hostent *gethostbyname(const char *name) {
  struct mg_bsd_cmd cmd = BSD_CMD_EMPTY;
  cmd.op = BSD_OP_GETHOSTBYNAME, cmd.op_gethostbyname.name = name;
  bsd_dispatch(&cmd);
  return cmd.op_gethostbyname.result;
}

#endif  // MG_ENABLE_BSD_SOCKETS

#if MG_ENABLE_BSD_SOCKETS
void mg_bsd_init(struct mg_mgr *mgr) {
  s_mgr = mgr;
#if MG_ENABLE_BSD_SOCKETS && MG_ENABLE_FREERTOS
  s_cmd_q = xQueueCreate(16, sizeof(struct mg_bsd_cmd *));
#endif
}

void mg_bsd_poll(struct mg_mgr *mgr) {
  (void) mgr;
#if MG_ENABLE_BSD_SOCKETS && MG_ENABLE_FREERTOS
  struct mg_bsd_cmd **p, *cmd;
  while (xQueueReceive(s_cmd_q, &cmd, 0) == pdTRUE) {
    if (bsd_run(cmd)) {
      xTaskNotifyGive((TaskHandle_t) cmd->caller);
    } else {
      cmd->next = s_waiters;
      s_waiters = cmd;
    }
  }
  p = &s_waiters;
  while (*p != NULL) {
    struct mg_bsd_cmd *w = *p;
    if (bsd_run(w)) {
      *p = w->next;
      xTaskNotifyGive((TaskHandle_t) w->caller);
    } else {
      p = &w->next;
    }
  }
  taskYIELD();
#endif
}
#endif
