#include "bsd.h"

#if MG_ENABLE_BSD_SOCKETS

struct mg_bsd_sock {
  void *t;                  // opaque transport handle
  int fd;
  int domain, type, proto;
  bool nonblock;
  struct sockaddr_in addr;  // bind address
  struct sockaddr_in peer;  // peer address (after accept/connect)
  struct mg_bsd_sock *next;
};

#define MG_BSD_FD_BASE 17777

// static struct mg_mgr *s_mgr;
static struct mg_bsd_sock *s_socks;

static struct mg_bsd_sock *get(int fd) {
  if (fd < MG_BSD_FD_BASE) return NULL;
  for (struct mg_bsd_sock *s = s_socks; s; s = s->next)
    if (s->fd == fd) return s;
  return NULL;
}

static int alloc_sock(struct mg_bsd_sock *s) {
  for (int fd = MG_BSD_FD_BASE; ; fd++) {
    if (get(fd) == NULL) { s->fd = fd; break; }
  }
  s->next = s_socks;
  s_socks = s;
  return s->fd;
}

static void release_sock(int fd) {
  struct mg_bsd_sock **p = &s_socks;
  while (*p && (*p)->fd != fd) p = &(*p)->next;
  if (*p) *p = (*p)->next;
}


int socket(int domain, int type, int proto) {
  struct mg_bsd_sock *s = (struct mg_bsd_sock *) calloc(1, sizeof(*s));
  if (!s) { errno = ENOMEM; return -1; }
  s->t = mg_bsd_transport_new(domain, type, proto);
  if (!s->t || alloc_sock(s) < 0) { free(s); errno = ENOMEM; return -1; }
  s->domain = domain; s->type = type; s->proto = proto;
  return s->fd;
}

int bind(int fd, const struct sockaddr *addr, socklen_t len) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  memcpy(&s->addr, addr, len < sizeof(s->addr) ? len : sizeof(s->addr));
  return 0;
}

int listen(int fd, int backlog) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  (void) backlog;
  return mg_bsd_transport_listen(s->t, &s->addr);
}

int accept(int fd, struct sockaddr *addr, socklen_t *addrlen) {
  struct mg_bsd_sock *ls = get(fd);
  if (!ls) return -1;
  struct sockaddr_in peer = {0};
  void *t = mg_bsd_transport_accept(ls->t, &peer, ls->nonblock);
  if (!t) { if (ls->nonblock) errno = EAGAIN; return -1; }
  struct mg_bsd_sock *ns = (struct mg_bsd_sock *) calloc(1, sizeof(*ns));
  if (!ns || alloc_sock(ns) < 0) { mg_bsd_transport_free(t); free(ns); errno = ENOMEM; return -1; }
  ns->t = t; ns->domain = ls->domain; ns->type = ls->type; ns->peer = peer;
  if (addr && addrlen) {
    size_t sz = sizeof(peer) < *addrlen ? sizeof(peer) : *addrlen;
    memcpy(addr, &peer, sz);
    *addrlen = (socklen_t) sizeof(peer);
  }
  return ns->fd;
}

int connect(int fd, const struct sockaddr *addr, socklen_t len) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  (void) len;
  return mg_bsd_transport_connect(s->t, (const struct sockaddr_in *) addr, s->nonblock);
}

ssize_t send(int fd, const void *buf, size_t len, int flags) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  return mg_bsd_transport_send(s->t, buf, len, s->nonblock || (flags & MSG_DONTWAIT));
}

ssize_t recv(int fd, void *buf, size_t len, int flags) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  return mg_bsd_transport_recv(s->t, buf, len, s->nonblock || (flags & MSG_DONTWAIT));
}

ssize_t sendto(int fd, const void *buf, size_t len, int flags,
                      const struct sockaddr *dest, socklen_t addrlen) {
  (void) dest; (void) addrlen;
  return send(fd, buf, len, flags);
}

ssize_t recvfrom(int fd, void *buf, size_t len, int flags,
                        struct sockaddr *src, socklen_t *addrlen) {
  ssize_t n = recv(fd, buf, len, flags);
  if (n > 0 && src && addrlen) {
    struct mg_bsd_sock *s = get(fd);
    if (s) {
      size_t sz = sizeof(s->peer) < *addrlen ? sizeof(s->peer) : *addrlen;
      memcpy(src, &s->peer, sz);
      *addrlen = (socklen_t) sizeof(s->peer);
    }
  }
  return n;
}

ssize_t write(int fd, const void *buf, size_t len) { return send(fd, buf, len, 0); }
ssize_t read(int fd, void *buf, size_t len) { return recv(fd, buf, len, 0); }

int close(int fd) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  mg_bsd_transport_close(s->t);
  release_sock(fd);
  free(s);
  return 0;
}

int shutdown(int fd, int how) { (void) how; return close(fd); }

int fcntl(int fd, int cmd, int arg) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  if (cmd == F_GETFL) return s->nonblock ? O_NONBLOCK : 0;
  if (cmd == F_SETFL) { s->nonblock = (arg & O_NONBLOCK) != 0; return 0; }
  return -1;
}

int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen) {
  (void) fd; (void) level; (void) optname; (void) optval; (void) optlen;
  return 0;
}

int getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen) {
  (void) fd; (void) level; (void) optname;
  if (optval && optlen && *optlen >= sizeof(int)) { *(int *) optval = 0; *optlen = sizeof(int); }
  return 0;
}

int getsockname(int fd, struct sockaddr *addr, socklen_t *addrlen) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) return -1;
  size_t sz = sizeof(s->addr) < *addrlen ? sizeof(s->addr) : *addrlen;
  memcpy(addr, &s->addr, sz);
  *addrlen = (socklen_t) sz;
  return 0;
}

int getpeername(int fd, struct sockaddr *addr, socklen_t *addrlen) {
  struct mg_bsd_sock *s = get(fd);
  if (!s) { errno = ENOTCONN; return -1; }
  size_t sz = sizeof(s->peer) < *addrlen ? sizeof(s->peer) : *addrlen;
  memcpy(addr, &s->peer, sz);
  *addrlen = (socklen_t) sz;
  return 0;
}

// select/poll: not implemented for queue-based backend
int select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tv) {
  (void) nfds; (void) r; (void) w; (void) e; (void) tv;
  return 0;
}

int poll(struct pollfd *fds, unsigned int nfds, int timeout) {
  (void) fds; (void) nfds; (void) timeout;
  return 0;
}

// DNS stubs (overridden in the FreeRTOS backend below)
#if !MG_ENABLE_FREERTOS
struct hostent *gethostbyname(const char *name) { (void) name; return NULL; }
int getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints, struct addrinfo **res) {
  (void) node; (void) service; (void) hints; (void) res;
  return -1;
}
void freeaddrinfo(struct addrinfo *res) { (void) res; }
#endif

int inet_pton(int af, const char *src, void *dst) {
  struct mg_addr a = {0};
  if (af == AF_INET && mg_aton(mg_str_s(src), &a)) { memcpy(dst, &a.addr.ip4, 4); return 1; }
  return 0;
}

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size) {
  if (af == AF_INET && size >= 16) {
    const uint8_t *ip = (const uint8_t *) src;
    snprintf(dst, size, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return dst;
  }
  return NULL;
}

in_addr_t inet_addr(const char *cp) {
  struct mg_addr a = {0};
  return mg_aton(mg_str_s(cp), &a) ? a.addr.ip4 : (in_addr_t) -1;
}

static char s_ntoa_buf[16];
char *inet_ntoa(struct in_addr in) {
  const uint8_t *ip = (const uint8_t *) &in.s_addr;
  snprintf(s_ntoa_buf, sizeof(s_ntoa_buf), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  return s_ntoa_buf;
}

#ifdef MG_ENABLE_BSD_PROTOTYPES
uint16_t htons(uint16_t n) { return mg_htons(n); }
uint16_t ntohs(uint16_t n) { return mg_htons(n); }
uint32_t htonl(uint32_t n) { return mg_htonl(n); }
uint32_t ntohl(uint32_t n) { return mg_htonl(n); }
#endif

// ============================================================
// FreeRTOS + Mongoose transport backend
// ============================================================
#if MG_ENABLE_FREERTOS

#include <queue.h>

#ifndef MG_BSD_CHUNK_SIZE
#define MG_BSD_CHUNK_SIZE 256
#endif
#ifndef MG_BSD_Q_DEPTH
#define MG_BSD_Q_DEPTH 4
#endif

struct mg_bsd_chunk { uint8_t data[MG_BSD_CHUNK_SIZE]; uint16_t len; };

struct mg_xport {
  struct mg_connection *c;  // Mongoose connection, task1-only
  QueueHandle_t recv_q;     // task1 writes on MG_EV_READ, task2 reads in recv()
  QueueHandle_t send_q;     // task2 writes in send(), task1 drains on MG_EV_POLL
  QueueHandle_t accept_q;   // task1 writes on MG_EV_ACCEPT, task2 reads in accept()
  struct sockaddr_in peer;
  bool closed;
  TaskHandle_t connect_waiter;  // task blocked in connect(), woken by MG_EV_CONNECT
  int *connect_result;          // where to store 0/−1 connect outcome
};

enum mg_bsd_cmd_op { BSD_CMD_LISTEN, BSD_CMD_CLOSE, BSD_CMD_CONNECT, BSD_CMD_RESOLVE };
struct mg_bsd_cmd {
  enum mg_bsd_cmd_op type;
  struct mg_xport *x;
  char url[64];
  TaskHandle_t caller;
  int *result;
};

static QueueHandle_t s_cmd_q;

// Single-slot DNS resolve state (not reentrant, sufficient for demos)
static struct { struct mg_addr addr; bool done, error; TaskHandle_t caller; } s_resolve;

static void resolve_cb(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_RESOLVE) { s_resolve.addr = c->rem; s_resolve.done = true; c->is_closing = 1; }
  else if ((ev == MG_EV_ERROR || ev == MG_EV_CLOSE) && !s_resolve.done) s_resolve.error = true;
  if ((s_resolve.done || s_resolve.error) && s_resolve.caller) {
    TaskHandle_t h = s_resolve.caller;
    s_resolve.caller = NULL;  // prevent double-notify on subsequent MG_EV_CLOSE
    xTaskNotifyGive(h);
  }
  (void) ev_data;
}

// Allocate transport for an accepted connection (recv+send queues only)
static struct mg_xport *xport_alloc(void) {
  struct mg_xport *x = (struct mg_xport *) calloc(1, sizeof(*x));
  if (!x) return NULL;
  x->recv_q = xQueueCreate(MG_BSD_Q_DEPTH, sizeof(struct mg_bsd_chunk));
  x->send_q = xQueueCreate(MG_BSD_Q_DEPTH, sizeof(struct mg_bsd_chunk));
  if (!x->recv_q || !x->send_q) { mg_bsd_transport_free(x); return NULL; }
  return x;
}

static void xport_ev(struct mg_connection *c, int ev, void *ev_data) {
  struct mg_xport *x = (struct mg_xport *) c->fn_data;
  if (!x) return;

  if (ev == MG_EV_ACCEPT) {
    // c is the new accepted connection; x is the listening transport
    struct mg_xport *nx = xport_alloc();
    if (!nx) { c->is_closing = 1; return; }
    nx->c = c;
    nx->peer.sin_family = AF_INET;
    nx->peer.sin_port = c->rem.port;
    memcpy(&nx->peer.sin_addr, &c->rem.addr.ip4, 4);
    c->fn_data = nx;
    xQueueSend(x->accept_q, &nx, 0);
  } else if (ev == MG_EV_READ && x->recv_q) {
    // Drain c->recv into recv_q in fixed-size chunks; task1 owns c->recv
    size_t off = 0;
    while (off < c->recv.len) {
      struct mg_bsd_chunk chunk;
      size_t n = c->recv.len - off;
      if (n > MG_BSD_CHUNK_SIZE) n = MG_BSD_CHUNK_SIZE;
      memcpy(chunk.data, c->recv.buf + off, n);
      chunk.len = (uint16_t) n;
      xQueueSend(x->recv_q, &chunk, portMAX_DELAY);
      off += n;
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  } else if (ev == MG_EV_POLL && x->send_q) {
    // Drain send_q → mg_send(); task1 owns c
    struct mg_bsd_chunk chunk;
    while (xQueueReceive(x->send_q, &chunk, 0) == pdTRUE)
      mg_send(c, chunk.data, chunk.len);
  } else if (ev == MG_EV_CONNECT) {
    // Outgoing connection established: wake the task blocked in connect()
    if (x->connect_waiter) {
      if (x->connect_result) *x->connect_result = 0;
      TaskHandle_t h = x->connect_waiter;
      x->connect_waiter = NULL; x->connect_result = NULL;
      xTaskNotifyGive(h);
    }
  } else if (ev == MG_EV_CLOSE) {
    x->c = NULL; x->closed = true; c->fn_data = NULL;
    // If connect() is still waiting, signal failure
    if (x->connect_waiter) {
      if (x->connect_result) *x->connect_result = -1;
      TaskHandle_t h = x->connect_waiter;
      x->connect_waiter = NULL; x->connect_result = NULL;
      xTaskNotifyGive(h);
    }
    if (x->recv_q) { struct mg_bsd_chunk eof = {.len = 0}; xQueueSend(x->recv_q, &eof, 0); }
    if (x->accept_q) { struct mg_xport *nil = NULL; xQueueSend(x->accept_q, &nil, 0); }
  }
  (void) ev_data;
}

void mg_bsd_init(void) {
  s_cmd_q = xQueueCreate(8, sizeof(struct mg_bsd_cmd));
}

void mg_bsd_poll(struct mg_mgr *mgr) {
  struct mg_bsd_cmd cmd;
  if (s_cmd_q == NULL) return;
  while (xQueueReceive(s_cmd_q, &cmd, 0) == pdTRUE) {
    bool notify = true;
    if (cmd.type == BSD_CMD_LISTEN) {
      struct mg_connection *c = mg_listen(mgr, cmd.url, xport_ev, cmd.x);
      cmd.x->c = c;
      *cmd.result = c ? 0 : -1;
    } else if (cmd.type == BSD_CMD_CLOSE) {
      if (cmd.x->c) {
        cmd.x->c->fn_data = NULL;
        cmd.x->c->is_draining = 1;
      }
      *cmd.result = 0;
    } else if (cmd.type == BSD_CMD_CONNECT) {
      cmd.x->connect_waiter = cmd.caller;
      cmd.x->connect_result = cmd.result;
      struct mg_connection *c = mg_connect(mgr, cmd.url, xport_ev, cmd.x);
      cmd.x->c = c;
      if (!c) { *cmd.result = -1; cmd.x->connect_waiter = NULL; cmd.x->connect_result = NULL; }
      else notify = false;  // xport_ev notifies when connected or on error
    } else if (cmd.type == BSD_CMD_RESOLVE) {
      s_resolve.done = s_resolve.error = false;
      s_resolve.caller = cmd.caller;
      char url[80];
      snprintf(url, sizeof(url), "tcp://%s:0", cmd.url);
      if (!mg_connect(mgr, url, resolve_cb, NULL)) s_resolve.error = true;
      else notify = false;  // resolve_cb notifies when done
    }
    if (notify) xTaskNotifyGive(cmd.caller);
  }
}

void *mg_bsd_transport_new(int domain, int type, int proto) {
  (void) domain; (void) type; (void) proto;
  // For socket() calls: allocate accept_q only; recv/send added when needed
  struct mg_xport *x = (struct mg_xport *) calloc(1, sizeof(*x));
  if (!x) return NULL;
  x->accept_q = xQueueCreate(MG_BSD_BACKLOG, sizeof(struct mg_xport *));
  if (!x->accept_q) { free(x); return NULL; }
  return x;
}

void mg_bsd_transport_free(void *t) {
  struct mg_xport *x = (struct mg_xport *) t;
  if (!x) return;
  if (x->recv_q) vQueueDelete(x->recv_q);
  if (x->send_q) vQueueDelete(x->send_q);
  if (x->accept_q) vQueueDelete(x->accept_q);
  free(x);
}

int mg_bsd_transport_listen(void *t, const struct sockaddr_in *addr) {
  struct mg_xport *x = (struct mg_xport *) t;
  int result = -1;
  struct mg_bsd_cmd cmd = {BSD_CMD_LISTEN, x, {0}, xTaskGetCurrentTaskHandle(), &result};
  snprintf(cmd.url, sizeof(cmd.url), "tcp://0.0.0.0:%d", mg_ntohs(addr->sin_port));
  xQueueSend(s_cmd_q, &cmd, portMAX_DELAY);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  return result;
}

void *mg_bsd_transport_accept(void *t, struct sockaddr_in *peer, bool nonblock) {
  struct mg_xport *x = (struct mg_xport *) t;
  struct mg_xport *nx = NULL;
  TickType_t ticks = nonblock ? 0 : portMAX_DELAY;
  if (xQueueReceive(x->accept_q, &nx, ticks) != pdTRUE || !nx) return NULL;
  if (peer) *peer = nx->peer;
  return nx;
}

ssize_t mg_bsd_transport_recv(void *t, void *buf, size_t len, bool nonblock) {
  struct mg_xport *x = (struct mg_xport *) t;
  struct mg_bsd_chunk chunk;
  TickType_t ticks = nonblock ? 0 : portMAX_DELAY;
  if (xQueueReceive(x->recv_q, &chunk, ticks) != pdTRUE) {
    errno = EAGAIN;
    return -1;
  }
  if (chunk.len == 0) return 0;  // EOF
  size_t n = chunk.len < len ? chunk.len : len;
  memcpy(buf, chunk.data, n);
  return (ssize_t) n;
}

ssize_t mg_bsd_transport_send(void *t, const void *buf, size_t len, bool nonblock) {
  struct mg_xport *x = (struct mg_xport *) t;
  if (x->closed) return -1;
  size_t sent = 0;
  TickType_t ticks = nonblock ? 0 : portMAX_DELAY;
  while (sent < len) {
    struct mg_bsd_chunk chunk;
    size_t n = len - sent;
    if (n > MG_BSD_CHUNK_SIZE) n = MG_BSD_CHUNK_SIZE;
    memcpy(chunk.data, (const uint8_t *) buf + sent, n);
    chunk.len = (uint16_t) n;
    if (xQueueSend(x->send_q, &chunk, ticks) != pdTRUE) break;
    sent += n;
  }
  return sent > 0 ? (ssize_t) sent : (errno = EAGAIN, -1);
}

int mg_bsd_transport_connect(void *t, const struct sockaddr_in *addr, bool nonblock) {
  struct mg_xport *x = (struct mg_xport *) t;
  (void) nonblock;
  if (!x->recv_q) x->recv_q = xQueueCreate(MG_BSD_Q_DEPTH, sizeof(struct mg_bsd_chunk));
  if (!x->send_q) x->send_q = xQueueCreate(MG_BSD_Q_DEPTH, sizeof(struct mg_bsd_chunk));
  if (!x->recv_q || !x->send_q) { errno = ENOMEM; return -1; }
  int result = -1;
  struct mg_bsd_cmd cmd = {BSD_CMD_CONNECT, x, {0}, xTaskGetCurrentTaskHandle(), &result};
  uint8_t *ip = (uint8_t *) &addr->sin_addr.s_addr;
  snprintf(cmd.url, sizeof(cmd.url), "tcp://%d.%d.%d.%d:%d",
           ip[0], ip[1], ip[2], ip[3], mg_ntohs(addr->sin_port));
  xQueueSend(s_cmd_q, &cmd, portMAX_DELAY);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  return result;
}

// gethostbyname: resolve via Mongoose DNS (not reentrant)
static struct hostent s_hostent;
static char *s_h_aliases[1];
static char *s_h_addr_list[2];
static uint32_t s_h_addr;
static char s_h_name[64];

struct hostent *gethostbyname(const char *name) {
  struct mg_bsd_cmd cmd = {BSD_CMD_RESOLVE, NULL, {0}, xTaskGetCurrentTaskHandle(), NULL};
  snprintf(cmd.url, sizeof(cmd.url), "%s", name);
  xQueueSend(s_cmd_q, &cmd, portMAX_DELAY);
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  if (s_resolve.error) return NULL;
  s_h_addr = s_resolve.addr.addr.ip4;
  s_h_addr_list[0] = (char *) &s_h_addr;
  s_h_addr_list[1] = NULL;
  s_h_aliases[0] = NULL;
  snprintf(s_h_name, sizeof(s_h_name), "%s", name);
  s_hostent.h_name = s_h_name;
  s_hostent.h_aliases = s_h_aliases;
  s_hostent.h_addrtype = AF_INET;
  s_hostent.h_length = 4;
  s_hostent.h_addr_list = s_h_addr_list;
  return &s_hostent;
}

int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints, struct addrinfo **res) {
  struct hostent *h = gethostbyname(node);
  if (!h) return -1;
  struct addrinfo *ai = (struct addrinfo *) calloc(1, sizeof(*ai));
  struct sockaddr_in *sa = (struct sockaddr_in *) calloc(1, sizeof(*sa));
  if (!ai || !sa) { free(ai); free(sa); return -1; }
  sa->sin_family = AF_INET;
  memcpy(&sa->sin_addr, h->h_addr, 4);
  if (service) sa->sin_port = htons((uint16_t) atoi(service));
  ai->ai_family = AF_INET;
  ai->ai_socktype = hints ? hints->ai_socktype : SOCK_STREAM;
  ai->ai_addrlen = sizeof(*sa);
  ai->ai_addr = (struct sockaddr *) sa;
  *res = ai;
  return 0;
}

void freeaddrinfo(struct addrinfo *res) {
  while (res) {
    struct addrinfo *next = res->ai_next;
    free(res->ai_addr);
    free(res);
    res = next;
  }
}

void mg_bsd_transport_close(void *t) {
  struct mg_xport *x = (struct mg_xport *) t;
  if (!x->closed && x->c) {
    int result = 0;
    struct mg_bsd_cmd cmd = {BSD_CMD_CLOSE, x, {0}, xTaskGetCurrentTaskHandle(), &result};
    xQueueSend(s_cmd_q, &cmd, portMAX_DELAY);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
  mg_bsd_transport_free(x);
}

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
    MG_ERROR("OOM");
    return false;
  }
  m->id = conn_id;
  m->len = len;
  memcpy(m->data, buf, len);
  if (xQueueSend((QueueHandle_t) mgr->pipe.q, &m, 0) != pdTRUE) {
    free(m);
    MG_ERROR("xQueueSend");
    return false;
  }
  return true;
}

#endif  // MG_ENABLE_FREERTOS
#endif  // MG_ENABLE_BSD_SOCKETS
