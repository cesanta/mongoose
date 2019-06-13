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

#if MG_ENABLE_NET_IF_LWIP_LOW_LEVEL

#include "common/mg_mem.h"

#include <lwip/init.h>
#include <lwip/pbuf.h>
#include <lwip/tcp.h>
#include <lwip/tcpip.h>
#if ((LWIP_VERSION_MAJOR << 8) | LWIP_VERSION_MINOR) >= 0x0105
#include <lwip/priv/tcp_priv.h>   /* For tcp_seg */
#include <lwip/priv/tcpip_priv.h> /* For tcpip_api_call */
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

#if !NO_SYS
#if LWIP_TCPIP_CORE_LOCKING
/* With locking tcpip_api_call is just a function call wrapped in lock/unlock,
 * so we can get away with just casting. */
void mg_lwip_netif_run_on_tcpip(void (*fn)(void *), void *arg) {
  tcpip_api_call((tcpip_api_call_fn) fn, (struct tcpip_api_call_data *) arg);
}
#else
static sys_sem_t s_tcpip_call_lock_sem = NULL;
static sys_sem_t s_tcpip_call_sync_sem = NULL;
struct mg_lwip_netif_tcpip_call_ctx {
  void (*fn)(void *);
  void *arg;
};
static void xxx_tcpip(void *arg) {
  struct mg_lwip_netif_tcpip_call_ctx *ctx =
      (struct mg_lwip_netif_tcpip_call_ctx *) arg;
  ctx->fn(ctx->arg);
  sys_sem_signal(&s_tcpip_call_sync_sem);
}
void mg_lwip_netif_run_on_tcpip(void (*fn)(void *), void *arg) {
  struct mg_lwip_netif_tcpip_call_ctx ctx = {.fn = fn, .arg = arg};
  sys_arch_sem_wait(&s_tcpip_call_lock_sem, 0);
  tcpip_send_msg_wait_sem(xxx_tcpip, &ctx, &s_tcpip_call_sync_sem);
  sys_sem_signal(&s_tcpip_call_lock_sem);
}
#endif
#else
#define mg_lwip_netif_run_on_tcpip(fn, arg) (fn)(arg)
#endif

void mg_lwip_if_init(struct mg_iface *iface);
void mg_lwip_if_free(struct mg_iface *iface);
void mg_lwip_if_add_conn(struct mg_connection *nc);
void mg_lwip_if_remove_conn(struct mg_connection *nc);
time_t mg_lwip_if_poll(struct mg_iface *iface, int timeout_ms);

// If compiling for Mongoose OS.
#ifdef MGOS
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
  if (nc == NULL || (nc->flags & MG_F_CLOSE_IMMEDIATELY)) return;
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
  struct mg_lwip_conn_state *cs =
      (nc ? (struct mg_lwip_conn_state *) nc->sock : NULL);
  DBG(("%p %p %p %p %u %d", nc, cs, tpcb, p, (p != NULL ? p->tot_len : 0),
       err));
  if (p == NULL) {
    if (nc != NULL && !(nc->flags & MG_F_CLOSE_IMMEDIATELY)) {
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
     * this one is the last, so we have to make a copy and release this one. */
    struct pbuf *np = pbuf_alloc(PBUF_RAW, p->tot_len, PBUF_RAM);
    if (np != NULL) {
      pbuf_copy(np, p);
      pbuf_free(p);
      p = np;
    }
  }
  mg_lwip_recv_common(nc, p);
  mgos_unlock();
  (void) err;
  return ERR_OK;
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
  if (nc->send_mbuf.len > 0 || (nc->flags & MG_F_WANT_WRITE)) {
    mg_lwip_mgr_schedule_poll(nc->mgr);
  }
  (void) num_sent;
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
  mg_lwip_netif_run_on_tcpip(mg_lwip_if_connect_tcp_tcpip, &ctx);
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
  mgos_lock();
  mg_lwip_recv_common(nc, sap);
  mgos_unlock();
  (void) pcb;
}

static void mg_lwip_recv_common(struct mg_connection *nc, struct pbuf *p) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (cs->rx_chain == NULL) {
    cs->rx_chain = p;
  } else {
    pbuf_chain(cs->rx_chain, p);
  }
  if (!cs->recv_pending) {
    cs->recv_pending = 1;
    mg_lwip_post_signal(MG_SIG_RECV, nc);
  }
}

static int mg_lwip_if_udp_recv(struct mg_connection *nc, void *buf, size_t len,
                               union socket_address *sa, size_t *sa_len) {
  /*
   * For UDP, RX chain consists of interleaved address and packet bufs:
   * Address pbuf followed by exactly one data pbuf (recv_cb took care of that).
   */
  int res = 0;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->sock == INVALID_SOCKET) return -1;
  mgos_lock();
  if (cs->rx_chain != NULL) {
    struct pbuf *ap = cs->rx_chain;
    struct pbuf *dp = ap->next;
    cs->rx_chain = pbuf_dechain(dp);
    res = MIN(dp->len, len);
    pbuf_copy_partial(dp, buf, res, 0);
    pbuf_free(dp);
    pbuf_copy_partial(ap, sa, MIN(*sa_len, ap->len), 0);
    pbuf_free(ap);
  }
  mgos_unlock();
  return res;
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
  mg_lwip_netif_run_on_tcpip(mg_lwip_if_connect_udp_tcpip, nc);
}

static void tcp_close_tcpip(void *arg) {
  tcp_close((struct tcp_pcb *) arg);
}

void mg_lwip_handle_accept(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (cs->pcb.tcp == NULL) return;
  union socket_address sa;
  struct tcp_pcb *tpcb = cs->pcb.tcp;
  SET_ADDR(&sa, &tpcb->remote_ip);
  sa.sin.sin_port = htons(tpcb->remote_port);
  mg_if_accept_tcp_cb(nc, &sa, sizeof(sa.sin));
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
  mg_lwip_netif_run_on_tcpip(mg_lwip_if_listen_tcp_tcpip, &ctx);
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
  mg_lwip_netif_run_on_tcpip(mg_lwip_if_listen_udp_tcpip, &ctx);
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
    mg_lwip_netif_run_on_tcpip(tcp_output_tcpip, tpcb);
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
  (void) unsent;
  (void) unacked;
}

int mg_lwip_if_tcp_send(struct mg_connection *nc, const void *buf, size_t len) {
  struct mg_lwip_tcp_write_ctx ctx = {.nc = nc, .data = buf, .len = len};
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->sock == INVALID_SOCKET) return -1;
  struct tcp_pcb *tpcb = cs->pcb.tcp;
  if (tpcb == NULL) return -1;
  if (tpcb->snd_buf <= 0) return 0;
  mg_lwip_netif_run_on_tcpip(mg_lwip_tcp_write_tcpip, &ctx);
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

static int mg_lwip_if_udp_send(struct mg_connection *nc, const void *data,
                               size_t len) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->sock == INVALID_SOCKET || cs->pcb.udp == NULL) return -1;
  struct udp_pcb *upcb = cs->pcb.udp;
  struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
#if defined(LWIP_IPV4) && LWIP_IPV4 && defined(LWIP_IPV6) && LWIP_IPV6
  ip_addr_t ip = {.u_addr.ip4.addr = nc->sa.sin.sin_addr.s_addr, .type = 0};
#else
  ip_addr_t ip = {.addr = nc->sa.sin.sin_addr.s_addr};
#endif
  u16_t port = ntohs(nc->sa.sin.sin_port);
  if (p == NULL) return 0;
  memcpy(p->payload, data, len);
  struct udp_sendto_ctx ctx = {.upcb = upcb, .p = p, .ip = &ip, .port = port};
  mg_lwip_netif_run_on_tcpip(udp_sendto_tcpip, &ctx);
  cs->err = ctx.ret;
  pbuf_free(p);
  return (cs->err == ERR_OK ? (int) len : -2);
}

static int mg_lwip_if_can_send(struct mg_connection *nc,
                               struct mg_lwip_conn_state *cs) {
  int can_send = 0;
  if (nc->send_mbuf.len > 0 || (nc->flags & MG_F_WANT_WRITE)) {
    /* We have stuff to send, but can we? */
    if (nc->flags & MG_F_UDP) {
      /* UDP is always ready for sending. */
      can_send = (cs->pcb.udp != NULL);
    } else {
      can_send = (cs->pcb.tcp != NULL && cs->pcb.tcp->snd_buf > 0);
/* See comment above. */
#if CS_PLATFORM == CS_P_ESP8266
      if (cs->pcb.tcp->unacked != NULL) can_send = 0;
#endif
    }
  }
  return can_send;
}

struct tcp_recved_ctx {
  struct tcp_pcb *tpcb;
  size_t len;
};

void tcp_recved_tcpip(void *arg) {
  struct tcp_recved_ctx *ctx = (struct tcp_recved_ctx *) arg;
  if (ctx->tpcb != NULL) tcp_recved(ctx->tpcb, ctx->len);
}

static int mg_lwip_if_tcp_recv(struct mg_connection *nc, void *buf,
                               size_t len) {
  int res = 0;
  char *bufp = buf;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  if (nc->sock == INVALID_SOCKET) return -1;
  mgos_lock();
  while (cs->rx_chain != NULL && len > 0) {
    struct pbuf *seg = cs->rx_chain;
    size_t seg_len = (seg->len - cs->rx_offset);
    size_t copy_len = MIN(len, seg_len);

    pbuf_copy_partial(seg, bufp, copy_len, cs->rx_offset);
    len -= copy_len;
    res += copy_len;
    bufp += copy_len;
    cs->rx_offset += copy_len;
    if (cs->rx_offset == cs->rx_chain->len) {
      cs->rx_chain = pbuf_dechain(cs->rx_chain);
      pbuf_free(seg);
      cs->rx_offset = 0;
    }
  }
  mgos_unlock();
  if (res > 0) {
    struct tcp_recved_ctx ctx = {.tpcb = cs->pcb.tcp, .len = res};
    mg_lwip_netif_run_on_tcpip(tcp_recved_tcpip, &ctx);
  }
  return res;
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
      mg_lwip_netif_run_on_tcpip(tcp_close_tcpip, tpcb);
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
      mg_lwip_netif_run_on_tcpip(udp_remove_tcpip, upcb);
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
    mg_lwip_if_tcp_recv,                                              \
    mg_lwip_if_udp_recv,                                              \
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
