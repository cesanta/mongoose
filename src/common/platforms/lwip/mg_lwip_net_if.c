/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_NET_IF_LWIP_LOW_LEVEL

#include "common/mg_mem.h"

#include <lwip/init.h>
#include <lwip/pbuf.h>
#include <lwip/tcp.h>
#include <lwip/tcpip.h>
#if ((LWIP_VERSION_MAJOR << 8) | LWIP_VERSION_MINOR) >= 0x0105
#include <lwip/priv/tcp_priv.h> /* For tcp_seg */
#else
#include <lwip/tcp_impl.h>
#endif
#include <lwip/udp.h>

#include "common/cs_dbg.h"

/*
 * Newest versions of LWIP have ip_2_ip4, older have ipX_2_ip,
 * even older have nothing.
 */
#ifndef ip_2_ip4
#ifdef ipX_2_ip
#define ip_2_ip4(addr) ipX_2_ip(addr)
#else
#define ip_2_ip4(addr) (addr)
#endif
#endif

/*
 * Depending on whether Mongoose is compiled with ipv6 support, use right
 * lwip functions
 */
#if MG_ENABLE_IPV6
#define TCP_NEW tcp_new_ip6
#define TCP_BIND tcp_bind_ip6
#define UDP_BIND udp_bind_ip6
#define IPADDR_NTOA(x) ip6addr_ntoa((const ip6_addr_t *)(x))
#define SET_ADDR(dst, src)                               \
  memcpy((dst)->sin6.sin6_addr.s6_addr, (src)->ip6.addr, \
         sizeof((dst)->sin6.sin6_addr.s6_addr))
#else
#define TCP_NEW tcp_new
#define TCP_BIND tcp_bind
#define UDP_BIND udp_bind
#define IPADDR_NTOA ipaddr_ntoa
#define SET_ADDR(dst, src) (dst)->sin.sin_addr.s_addr = ip_2_ip4(src)->addr
#endif

#if NO_SYS
#define tcpip_callback(fn, arg) (fn)(arg)
typedef void (*tcpip_callback_fn)(void *arg);
#endif

void mg_lwip_ssl_do_hs(struct mg_connection *nc);
void mg_lwip_ssl_send(struct mg_connection *nc);
void mg_lwip_ssl_recv(struct mg_connection *nc);

void mg_lwip_if_init(struct mg_iface *iface);
void mg_lwip_if_free(struct mg_iface *iface);
void mg_lwip_if_add_conn(struct mg_connection *nc);
void mg_lwip_if_remove_conn(struct mg_connection *nc);
time_t mg_lwip_if_poll(struct mg_iface *iface, int timeout_ms);

#if defined(RTOS_SDK) || defined(ESP_PLATFORM)
extern void mgos_lock();
extern void mgos_unlock();
#else
#define mgos_lock()
#define mgos_unlock()
#endif

static void mg_lwip_recv_common(struct mg_connection *nc, struct pbuf *p);

#if LWIP_TCP_KEEPALIVE
void mg_lwip_set_keepalive_params(struct mg_connection *nc, int idle,
                                  int interval, int count) {
  if (nc->sock == INVALID_SOCKET || nc->flags & MG_F_UDP) {
    return;
  }
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct tcp_pcb *tpcb = cs->pcb.tcp;
  if (idle > 0 && interval > 0 && count > 0) {
    tpcb->keep_idle = idle * 1000;
    tpcb->keep_intvl = interval * 1000;
    tpcb->keep_cnt = count;
    tpcb->so_options |= SOF_KEEPALIVE;
  } else {
    tpcb->so_options &= ~SOF_KEEPALIVE;
  }
}
#elif !defined(MG_NO_LWIP_TCP_KEEPALIVE)
#warning LWIP TCP keepalive is disabled. Please consider enabling it.
#endif /* LWIP_TCP_KEEPALIVE */

static err_t mg_lwip_tcp_conn_cb(void *arg, struct tcp_pcb *tpcb, err_t err) {
  struct mg_connection *nc = (struct mg_connection *) arg;
  DBG(("%p connect to %s:%u = %d", nc, IPADDR_NTOA(ipX_2_ip(&tpcb->remote_ip)),
       tpcb->remote_port, err));
  if (nc == NULL) {
    tcp_abort(tpcb);
    return ERR_ARG;
  }
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  cs->err = err;
#if LWIP_TCP_KEEPALIVE
  if (err == 0) mg_lwip_set_keepalive_params(nc, 60, 10, 6);
#endif
  mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
  return ERR_OK;
}

static void mg_lwip_tcp_error_cb(void *arg, err_t err) {
  struct mg_connection *nc = (struct mg_connection *) arg;
  DBG(("%p conn error %d", nc, err));
  if (nc == NULL) return;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  cs->pcb.tcp = NULL; /* Has already been deallocated */
  if (nc->flags & MG_F_CONNECTING) {
    cs->err = err;
    mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
  } else {
    mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
  }
}

static err_t mg_lwip_tcp_recv_cb(void *arg, struct tcp_pcb *tpcb,
                                 struct pbuf *p, err_t err) {
  struct mg_connection *nc = (struct mg_connection *) arg;
  DBG(("%p %p %u %d", nc, tpcb, (p != NULL ? p->tot_len : 0), err));
  if (p == NULL) {
    if (nc != NULL && !(nc->flags & MG_F_CLOSE_IMMEDIATELY)) {
      struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
      if (cs->rx_chain != NULL) {
        /*
         * rx_chain still contains non-consumed data, don't close the
         * connection
         */
        cs->draining_rx_chain = 1;
      } else {
        mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
      }
    } else {
      /* Tombstoned connection, do nothing. */
    }
    return ERR_OK;
  } else if (nc == NULL) {
    tcp_abort(tpcb);
    return ERR_ARG;
  }
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  /*
   * If we get a chain of more than one segment at once, we need to bump
   * refcount on the subsequent bufs to make them independent.
   */
  if (p->next != NULL) {
    struct pbuf *q = p->next;
    for (; q != NULL; q = q->next) pbuf_ref(q);
  }
  mgos_lock();
  if (cs->rx_chain == NULL) {
    cs->rx_offset = 0;
  } else if (pbuf_clen(cs->rx_chain) >= 4) {
    /* ESP SDK has a limited pool of 5 pbufs. We must not hog them all or RX
     * will be completely blocked. We already have at least 4 in the chain,
     * this one is, so we have to make a copy and release this one. */
    struct pbuf *np = pbuf_alloc(PBUF_RAW, p->tot_len, PBUF_RAM);
    if (np != NULL) {
      pbuf_copy(np, p);
      pbuf_free(p);
      p = np;
    }
  }
  mgos_unlock();
  mg_lwip_recv_common(nc, p);
  return ERR_OK;
}

static void mg_lwip_consume_rx_chain_tcp(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (cs->rx_chain == NULL) return;
#if MG_ENABLE_SSL
  if (nc->flags & MG_F_SSL) {
    if (nc->flags & MG_F_SSL_HANDSHAKE_DONE) {
      mg_lwip_ssl_recv(nc);
    } else {
      mg_lwip_ssl_do_hs(nc);
    }
    return;
  }
#endif
  mgos_lock();
  while (cs->rx_chain != NULL && nc->recv_mbuf.len < nc->recv_mbuf_limit) {
    struct pbuf *seg = cs->rx_chain;

    size_t seg_len = (seg->len - cs->rx_offset);
    size_t buf_avail = (nc->recv_mbuf_limit - nc->recv_mbuf.len);
    size_t len = MIN(seg_len, buf_avail);

    char *data = (char *) MG_MALLOC(len);
    if (data == NULL) {
      mgos_unlock();
      DBG(("OOM"));
      return;
    }
    pbuf_copy_partial(seg, data, len, cs->rx_offset);
    cs->rx_offset += len;
    if (cs->rx_offset == cs->rx_chain->len) {
      cs->rx_chain = pbuf_dechain(cs->rx_chain);
      pbuf_free(seg);
      cs->rx_offset = 0;
    }
    mgos_unlock();
    mg_if_recv_tcp_cb(nc, data, len, 1 /* own */);
    mgos_lock();
  }
  mgos_unlock();
}

static void mg_lwip_handle_recv_tcp(struct mg_connection *nc) {
  mg_lwip_consume_rx_chain_tcp(nc);

  if (nc->send_mbuf.len > 0) {
    mg_lwip_mgr_schedule_poll(nc->mgr);
  }
}

static err_t mg_lwip_tcp_sent_cb(void *arg, struct tcp_pcb *tpcb,
                                 u16_t num_sent) {
  struct mg_connection *nc = (struct mg_connection *) arg;
  DBG(("%p %p %u %p %p", nc, tpcb, num_sent, tpcb->unsent, tpcb->unacked));
  if (nc == NULL) return ERR_OK;
  if ((nc->flags & MG_F_SEND_AND_CLOSE) && !(nc->flags & MG_F_WANT_WRITE) &&
      nc->send_mbuf.len == 0 && tpcb->unsent == NULL && tpcb->unacked == NULL) {
    mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
  }
  return ERR_OK;
}

struct mg_lwip_if_connect_tcp_ctx {
  struct mg_connection *nc;
  const union socket_address *sa;
};

static void mg_lwip_if_connect_tcp_tcpip(void *arg) {
  struct mg_lwip_if_connect_tcp_ctx *ctx =
      (struct mg_lwip_if_connect_tcp_ctx *) arg;
  struct mg_connection *nc = ctx->nc;
  const union socket_address *sa = ctx->sa;

  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct tcp_pcb *tpcb = TCP_NEW();
  cs->pcb.tcp = tpcb;
  ip_addr_t *ip = (ip_addr_t *) &sa->sin.sin_addr.s_addr;
  u16_t port = ntohs(sa->sin.sin_port);
  tcp_arg(tpcb, nc);
  tcp_err(tpcb, mg_lwip_tcp_error_cb);
  tcp_sent(tpcb, mg_lwip_tcp_sent_cb);
  tcp_recv(tpcb, mg_lwip_tcp_recv_cb);
  cs->err = TCP_BIND(tpcb, IP_ADDR_ANY, 0 /* any port */);
  DBG(("%p tcp_bind = %d", nc, cs->err));
  if (cs->err != ERR_OK) {
    mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
    return;
  }
  cs->err = tcp_connect(tpcb, ip, port, mg_lwip_tcp_conn_cb);
  DBG(("%p tcp_connect %p = %d", nc, tpcb, cs->err));
  if (cs->err != ERR_OK) {
    mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
    return;
  }
}

void mg_lwip_if_connect_tcp(struct mg_connection *nc,
                            const union socket_address *sa) {
  struct mg_lwip_if_connect_tcp_ctx ctx = {.nc = nc, .sa = sa};
  tcpip_callback(mg_lwip_if_connect_tcp_tcpip, &ctx);
}

/*
 * Lwip included in the SDKs for nRF5x chips has different type for the
 * callback of `udp_recv()`
 */
#if ((LWIP_VERSION_MAJOR << 8) | LWIP_VERSION_MINOR) >= 0x0105
static void mg_lwip_udp_recv_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                                const ip_addr_t *addr, u16_t port)
#else
static void mg_lwip_udp_recv_cb(void *arg, struct udp_pcb *pcb, struct pbuf *p,
                                ip_addr_t *addr, u16_t port)
#endif
{
  struct mg_connection *nc = (struct mg_connection *) arg;
  DBG(("%p %s:%u %p %u %u", nc, IPADDR_NTOA(addr), port, p, p->ref, p->len));
  /* Put address in a separate pbuf and tack it onto the packet. */
  struct pbuf *sap =
      pbuf_alloc(PBUF_RAW, sizeof(union socket_address), PBUF_RAM);
  if (sap == NULL) {
    pbuf_free(p);
    return;
  }
  union socket_address *sa = (union socket_address *) sap->payload;
#if ((LWIP_VERSION_MAJOR << 8) | LWIP_VERSION_MINOR) >= 0x0105
  sa->sin.sin_addr.s_addr = ip_2_ip4(addr)->addr;
#else
  sa->sin.sin_addr.s_addr = addr->addr;
#endif
  sa->sin.sin_port = htons(port);
  /* Logic in the recv handler requires that there be exactly one data pbuf. */
  p = pbuf_coalesce(p, PBUF_RAW);
  pbuf_chain(sap, p);
  mg_lwip_recv_common(nc, sap);
  (void) pcb;
}

static void mg_lwip_recv_common(struct mg_connection *nc, struct pbuf *p) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  mgos_lock();
  if (cs->rx_chain == NULL) {
    cs->rx_chain = p;
  } else {
    pbuf_chain(cs->rx_chain, p);
  }
  if (!cs->recv_pending) {
    cs->recv_pending = 1;
    mg_lwip_post_signal(MG_SIG_RECV, nc);
  }
  mgos_unlock();
}

static void mg_lwip_handle_recv_udp(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  /*
   * For UDP, RX chain consists of interleaved address and packet bufs:
   * Address pbuf followed by exactly one data pbuf (recv_cb took care of that).
   */
  while (cs->rx_chain != NULL) {
    struct pbuf *sap = cs->rx_chain;
    struct pbuf *p = sap->next;
    cs->rx_chain = pbuf_dechain(p);
    size_t data_len = p->len;
    char *data = (char *) MG_MALLOC(data_len);
    if (data != NULL) {
      pbuf_copy_partial(p, data, data_len, 0);
      pbuf_free(p);
      mg_if_recv_udp_cb(nc, data, data_len,
                        (union socket_address *) sap->payload, sap->len);
      pbuf_free(sap);
    } else {
      pbuf_free(p);
      pbuf_free(sap);
    }
  }
}

static void mg_lwip_if_connect_udp_tcpip(void *arg) {
  struct mg_connection *nc = (struct mg_connection *) arg;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct udp_pcb *upcb = udp_new();
  cs->err = UDP_BIND(upcb, IP_ADDR_ANY, 0 /* any port */);
  DBG(("%p udp_bind %p = %d", nc, upcb, cs->err));
  if (cs->err == ERR_OK) {
    udp_recv(upcb, mg_lwip_udp_recv_cb, nc);
    cs->pcb.udp = upcb;
  } else {
    udp_remove(upcb);
  }
  mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
}

void mg_lwip_if_connect_udp(struct mg_connection *nc) {
  tcpip_callback(mg_lwip_if_connect_udp_tcpip, nc);
}

void mg_lwip_accept_conn(struct mg_connection *nc, struct tcp_pcb *tpcb) {
  union socket_address sa;
  SET_ADDR(&sa, &tpcb->remote_ip);
  sa.sin.sin_port = htons(tpcb->remote_port);
  mg_if_accept_tcp_cb(nc, &sa, sizeof(sa.sin));
}

static void tcp_close_tcpip(void *arg) {
  tcp_close((struct tcp_pcb *) arg);
}

void mg_lwip_handle_accept(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (cs->pcb.tcp == NULL) return;
#if MG_ENABLE_SSL
  if (cs->lc->flags & MG_F_SSL) {
    if (mg_ssl_if_conn_accept(nc, cs->lc) != MG_SSL_OK) {
      LOG(LL_ERROR, ("SSL error"));
      tcpip_callback(tcp_close_tcpip, cs->pcb.tcp);
    }
  } else
#endif
  {
    mg_lwip_accept_conn(nc, cs->pcb.tcp);
  }
}

static err_t mg_lwip_accept_cb(void *arg, struct tcp_pcb *newtpcb, err_t err) {
  struct mg_connection *lc = (struct mg_connection *) arg, *nc;
  struct mg_lwip_conn_state *lcs, *cs;
  struct tcp_pcb_listen *lpcb;
  LOG(LL_DEBUG,
      ("%p conn %p from %s:%u", lc, newtpcb,
       IPADDR_NTOA(ipX_2_ip(&newtpcb->remote_ip)), newtpcb->remote_port));
  if (lc == NULL) {
    tcp_abort(newtpcb);
    return ERR_ABRT;
  }
  lcs = (struct mg_lwip_conn_state *) lc->sock;
  lpcb = (struct tcp_pcb_listen *) lcs->pcb.tcp;
#if TCP_LISTEN_BACKLOG
  tcp_accepted(lpcb);
#endif
  nc = mg_if_accept_new_conn(lc);
  if (nc == NULL) {
    tcp_abort(newtpcb);
    return ERR_ABRT;
  }
  cs = (struct mg_lwip_conn_state *) nc->sock;
  cs->lc = lc;
  cs->pcb.tcp = newtpcb;
  /* We need to set up callbacks before returning because data may start
   * arriving immediately. */
  tcp_arg(newtpcb, nc);
  tcp_err(newtpcb, mg_lwip_tcp_error_cb);
  tcp_sent(newtpcb, mg_lwip_tcp_sent_cb);
  tcp_recv(newtpcb, mg_lwip_tcp_recv_cb);
#if LWIP_TCP_KEEPALIVE
  mg_lwip_set_keepalive_params(nc, 60, 10, 6);
#endif
  mg_lwip_post_signal(MG_SIG_ACCEPT, nc);
  (void) err;
  (void) lpcb;
  return ERR_OK;
}

struct mg_lwip_if_listen_ctx {
  struct mg_connection *nc;
  union socket_address *sa;
  int ret;
};

static void mg_lwip_if_listen_tcp_tcpip(void *arg) {
  struct mg_lwip_if_listen_ctx *ctx = (struct mg_lwip_if_listen_ctx *) arg;
  struct mg_connection *nc = ctx->nc;
  union socket_address *sa = ctx->sa;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct tcp_pcb *tpcb = TCP_NEW();
  ip_addr_t *ip = (ip_addr_t *) &sa->sin.sin_addr.s_addr;
  u16_t port = ntohs(sa->sin.sin_port);
  cs->err = TCP_BIND(tpcb, ip, port);
  DBG(("%p tcp_bind(%s:%u) = %d", nc, IPADDR_NTOA(ip), port, cs->err));
  if (cs->err != ERR_OK) {
    tcp_close(tpcb);
    ctx->ret = -1;
    return;
  }
  tcp_arg(tpcb, nc);
  tpcb = tcp_listen(tpcb);
  cs->pcb.tcp = tpcb;
  tcp_accept(tpcb, mg_lwip_accept_cb);
  ctx->ret = 0;
}

int mg_lwip_if_listen_tcp(struct mg_connection *nc, union socket_address *sa) {
  struct mg_lwip_if_listen_ctx ctx = {.nc = nc, .sa = sa};
  tcpip_callback(mg_lwip_if_listen_tcp_tcpip, &ctx);
  return ctx.ret;
}

static void mg_lwip_if_listen_udp_tcpip(void *arg) {
  struct mg_lwip_if_listen_ctx *ctx = (struct mg_lwip_if_listen_ctx *) arg;
  struct mg_connection *nc = ctx->nc;
  union socket_address *sa = ctx->sa;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct udp_pcb *upcb = udp_new();
  ip_addr_t *ip = (ip_addr_t *) &sa->sin.sin_addr.s_addr;
  u16_t port = ntohs(sa->sin.sin_port);
  cs->err = UDP_BIND(upcb, ip, port);
  DBG(("%p udb_bind(%s:%u) = %d", nc, IPADDR_NTOA(ip), port, cs->err));
  if (cs->err != ERR_OK) {
    udp_remove(upcb);
    ctx->ret = -1;
  } else {
    udp_recv(upcb, mg_lwip_udp_recv_cb, nc);
    cs->pcb.udp = upcb;
    ctx->ret = 0;
  }
}

int mg_lwip_if_listen_udp(struct mg_connection *nc, union socket_address *sa) {
  struct mg_lwip_if_listen_ctx ctx = {.nc = nc, .sa = sa};
  tcpip_callback(mg_lwip_if_listen_udp_tcpip, &ctx);
  return ctx.ret;
}

struct mg_lwip_tcp_write_ctx {
  struct mg_connection *nc;
  const void *data;
  uint16_t len;
  int ret;
};

static void tcp_output_tcpip(void *arg) {
  tcp_output((struct tcp_pcb *) arg);
}

static void mg_lwip_tcp_write_tcpip(void *arg) {
  struct mg_lwip_tcp_write_ctx *ctx = (struct mg_lwip_tcp_write_ctx *) arg;
  struct mg_connection *nc = ctx->nc;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct tcp_pcb *tpcb = cs->pcb.tcp;
  size_t len = MIN(tpcb->mss, MIN(ctx->len, tpcb->snd_buf));
  size_t unsent, unacked;
  if (len == 0) {
    DBG(("%p no buf avail %u %u %p %p", tpcb, tpcb->snd_buf, tpcb->snd_queuelen,
         tpcb->unsent, tpcb->unacked));
    tcpip_callback(tcp_output_tcpip, tpcb);
    ctx->ret = 0;
    return;
  }
  unsent = (tpcb->unsent != NULL ? tpcb->unsent->len : 0);
  unacked = (tpcb->unacked != NULL ? tpcb->unacked->len : 0);
/*
 * On ESP8266 we only allow one TCP segment in flight at any given time.
 * This may increase latency and reduce efficiency of tcp windowing,
 * but memory is scarce and precious on that platform so we do this to
 * reduce footprint.
 */
#if CS_PLATFORM == CS_P_ESP8266
  if (unacked > 0) {
    ctx->ret = 0;
    return;
  }
  len = MIN(len, (TCP_MSS - unsent));
#endif
  cs->err = tcp_write(tpcb, ctx->data, len, TCP_WRITE_FLAG_COPY);
  unsent = (tpcb->unsent != NULL ? tpcb->unsent->len : 0);
  unacked = (tpcb->unacked != NULL ? tpcb->unacked->len : 0);
  DBG(("%p tcp_write %u = %d, %u %u", tpcb, len, cs->err, unsent, unacked));
  if (cs->err != ERR_OK) {
    /*
     * We ignore ERR_MEM because memory will be freed up when the data is sent
     * and we'll retry.
     */
    ctx->ret = (cs->err == ERR_MEM ? 0 : -1);
    return;
  }
  ctx->ret = len;
}

static int mg_lwip_tcp_write(struct mg_connection *nc, const void *data,
                             uint16_t len) {
  struct mg_lwip_tcp_write_ctx ctx = {.nc = nc, .data = data, .len = len};
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct tcp_pcb *tpcb = cs->pcb.tcp;
  if (tpcb == NULL) {
    return -1;
  }
  tcpip_callback(mg_lwip_tcp_write_tcpip, &ctx);
  return ctx.ret;
}

struct udp_sendto_ctx {
  struct udp_pcb *upcb;
  struct pbuf *p;
  ip_addr_t *ip;
  uint16_t port;
  int ret;
};

static void udp_sendto_tcpip(void *arg) {
  struct udp_sendto_ctx *ctx = (struct udp_sendto_ctx *) arg;
  ctx->ret = udp_sendto(ctx->upcb, ctx->p, ctx->ip, ctx->port);
}

static int mg_lwip_udp_send(struct mg_connection *nc, const void *data,
                            uint16_t len) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (cs->pcb.udp == NULL) {
    /*
     * In case of UDP, this usually means, what
     * async DNS resolve is still in progress and connection
     * is not ready yet
     */
    DBG(("%p socket is not connected", nc));
    return -1;
  }
  struct udp_pcb *upcb = cs->pcb.udp;
  struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
#if defined(LWIP_IPV4) && LWIP_IPV4 && defined(LWIP_IPV6) && LWIP_IPV6
  ip_addr_t ip = {.u_addr.ip4.addr = nc->sa.sin.sin_addr.s_addr, .type = 0};
#else
  ip_addr_t ip = {.addr = nc->sa.sin.sin_addr.s_addr};
#endif
  u16_t port = ntohs(nc->sa.sin.sin_port);
  if (p == NULL) {
    DBG(("OOM"));
    return 0;
  }
  memcpy(p->payload, data, len);
  struct udp_sendto_ctx ctx = {.upcb = upcb, .p = p, .ip = &ip, .port = port};
  tcpip_callback(udp_sendto_tcpip, &ctx);
  cs->err = ctx.ret;
  pbuf_free(p);
  return (cs->err == ERR_OK ? len : -1);
}

static void mg_lwip_send_more(struct mg_connection *nc) {
  int num_sent = 0;
  if (nc->sock == INVALID_SOCKET) return;
  if (nc->flags & MG_F_UDP) {
    num_sent = mg_lwip_udp_send(nc, nc->send_mbuf.buf, nc->send_mbuf.len);
    DBG(("%p mg_lwip_udp_send %u = %d", nc, nc->send_mbuf.len, num_sent));
  } else {
    num_sent = mg_lwip_tcp_write(nc, nc->send_mbuf.buf, nc->send_mbuf.len);
    DBG(("%p mg_lwip_tcp_write %u = %d", nc, nc->send_mbuf.len, num_sent));
  }
  if (num_sent == 0) return;
  if (num_sent > 0) {
    mg_if_sent_cb(nc, num_sent);
  } else {
    mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
  }
}

void mg_lwip_if_tcp_send(struct mg_connection *nc, const void *buf,
                         size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
  mg_lwip_mgr_schedule_poll(nc->mgr);
}

void mg_lwip_if_udp_send(struct mg_connection *nc, const void *buf,
                         size_t len) {
  mbuf_append(&nc->send_mbuf, buf, len);
  mg_lwip_mgr_schedule_poll(nc->mgr);
}

struct tcp_recved_ctx {
  struct tcp_pcb *tpcb;
  size_t len;
};

void tcp_recved_tcpip(void *arg) {
  struct tcp_recved_ctx *ctx = (struct tcp_recved_ctx *) arg;
  tcp_recved(ctx->tpcb, ctx->len);
}

void mg_lwip_if_recved(struct mg_connection *nc, size_t len) {
  if (nc->flags & MG_F_UDP) return;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->sock == INVALID_SOCKET || cs->pcb.tcp == NULL) {
    DBG(("%p invalid socket", nc));
    return;
  }
  DBG(("%p %p %u %u", nc, cs->pcb.tcp, len,
       (cs->rx_chain ? cs->rx_chain->tot_len : 0)));
  struct tcp_recved_ctx ctx = {.tpcb = cs->pcb.tcp, .len = len};
#if MG_ENABLE_SSL
  if (!(nc->flags & MG_F_SSL)) {
    tcpip_callback(tcp_recved_tcpip, &ctx);
  } else {
    /* Currently SSL acknowledges data immediately.
     * TODO(rojer): Find a way to propagate mg_lwip_if_recved. */
  }
#else
  tcpip_callback(tcp_recved_tcpip, &ctx);
#endif
  mbuf_trim(&nc->recv_mbuf);
}

int mg_lwip_if_create_conn(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs =
      (struct mg_lwip_conn_state *) MG_CALLOC(1, sizeof(*cs));
  if (cs == NULL) return 0;
  cs->nc = nc;
  nc->sock = (intptr_t) cs;
  return 1;
}

static void udp_remove_tcpip(void *arg) {
  udp_remove((struct udp_pcb *) arg);
}

void mg_lwip_if_destroy_conn(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) return;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (!(nc->flags & MG_F_UDP)) {
    struct tcp_pcb *tpcb = cs->pcb.tcp;
    if (tpcb != NULL) {
      tcp_arg(tpcb, NULL);
      DBG(("%p tcp_close %p", nc, tpcb));
      tcp_arg(tpcb, NULL);
      tcpip_callback(tcp_close_tcpip, tpcb);
    }
    while (cs->rx_chain != NULL) {
      struct pbuf *seg = cs->rx_chain;
      cs->rx_chain = pbuf_dechain(cs->rx_chain);
      pbuf_free(seg);
    }
    memset(cs, 0, sizeof(*cs));
    MG_FREE(cs);
  } else if (nc->listener == NULL) {
    /* Only close outgoing UDP pcb or listeners. */
    struct udp_pcb *upcb = cs->pcb.udp;
    if (upcb != NULL) {
      DBG(("%p udp_remove %p", nc, upcb));
      tcpip_callback(udp_remove_tcpip, upcb);
    }
    memset(cs, 0, sizeof(*cs));
    MG_FREE(cs);
  }
  nc->sock = INVALID_SOCKET;
}

void mg_lwip_if_get_conn_addr(struct mg_connection *nc, int remote,
                              union socket_address *sa) {
  memset(sa, 0, sizeof(*sa));
  if (nc == NULL || nc->sock == INVALID_SOCKET) return;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->flags & MG_F_UDP) {
    struct udp_pcb *upcb = cs->pcb.udp;
    if (remote) {
      memcpy(sa, &nc->sa, sizeof(*sa));
    } else if (upcb != NULL) {
      sa->sin.sin_port = htons(upcb->local_port);
      SET_ADDR(sa, &upcb->local_ip);
    }
  } else {
    struct tcp_pcb *tpcb = cs->pcb.tcp;
    if (remote) {
      memcpy(sa, &nc->sa, sizeof(*sa));
    } else if (tpcb != NULL) {
      sa->sin.sin_port = htons(tpcb->local_port);
      SET_ADDR(sa, &tpcb->local_ip);
    }
  }
}

void mg_lwip_if_sock_set(struct mg_connection *nc, sock_t sock) {
  nc->sock = sock;
}

/* clang-format off */
#define MG_LWIP_IFACE_VTABLE                                          \
  {                                                                   \
    mg_lwip_if_init,                                                  \
    mg_lwip_if_free,                                                  \
    mg_lwip_if_add_conn,                                              \
    mg_lwip_if_remove_conn,                                           \
    mg_lwip_if_poll,                                                  \
    mg_lwip_if_listen_tcp,                                            \
    mg_lwip_if_listen_udp,                                            \
    mg_lwip_if_connect_tcp,                                           \
    mg_lwip_if_connect_udp,                                           \
    mg_lwip_if_tcp_send,                                              \
    mg_lwip_if_udp_send,                                              \
    mg_lwip_if_recved,                                                \
    mg_lwip_if_create_conn,                                           \
    mg_lwip_if_destroy_conn,                                          \
    mg_lwip_if_sock_set,                                              \
    mg_lwip_if_get_conn_addr,                                         \
  }
/* clang-format on */

const struct mg_iface_vtable mg_lwip_iface_vtable = MG_LWIP_IFACE_VTABLE;
#if MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL
const struct mg_iface_vtable mg_default_iface_vtable = MG_LWIP_IFACE_VTABLE;
#endif

#endif /* MG_ENABLE_NET_IF_LWIP_LOW_LEVEL */
