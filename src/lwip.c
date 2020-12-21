#include "dns.h"
#include "log.h"
#include "net.h"
#include "private.h"
#include "tls.h"
#include "url.h"

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
