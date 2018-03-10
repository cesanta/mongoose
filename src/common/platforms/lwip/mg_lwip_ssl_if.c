/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_SSL && MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL

#include "common/mg_mem.h"
#include "common/cs_dbg.h"

#include <lwip/pbuf.h>
#include <lwip/tcp.h>

#ifndef MG_LWIP_SSL_IO_SIZE
#define MG_LWIP_SSL_IO_SIZE 1024
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

void mg_lwip_ssl_do_hs(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  int server_side = (nc->listener != NULL);
  enum mg_ssl_if_result res;
  if (nc->flags & MG_F_CLOSE_IMMEDIATELY) return;
  res = mg_ssl_if_handshake(nc);
  DBG(("%p %lu %d %d", nc, nc->flags, server_side, res));
  if (res != MG_SSL_OK) {
    if (res == MG_SSL_WANT_WRITE) {
      nc->flags |= MG_F_WANT_WRITE;
      cs->err = 0;
    } else if (res == MG_SSL_WANT_READ) {
      /*
       * Nothing to do in particular, we are callback-driven.
       * What we definitely do not need anymore is SSL reading (nothing left).
       */
      nc->flags &= ~MG_F_WANT_READ;
      cs->err = 0;
    } else {
      cs->err = res;
      if (server_side) {
        mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
      } else {
        mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
      }
    }
  } else {
    cs->err = 0;
    nc->flags &= ~MG_F_WANT_WRITE;
    /*
     * Handshake is done. Schedule a read immediately to consume app data
     * which may already be waiting.
     */
    nc->flags |= (MG_F_SSL_HANDSHAKE_DONE | MG_F_WANT_READ);
    if (server_side) {
      mg_lwip_accept_conn(nc, cs->pcb.tcp);
    } else {
      mg_lwip_post_signal(MG_SIG_CONNECT_RESULT, nc);
    }
  }
}

void mg_lwip_ssl_send(struct mg_connection *nc) {
  if (nc->sock == INVALID_SOCKET) {
    DBG(("%p invalid socket", nc));
    return;
  }
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  /* It's ok if the buffer is empty. Return value of 0 may also be valid. */
  int len = cs->last_ssl_write_size;
  if (len == 0) {
    len = MIN(MG_LWIP_SSL_IO_SIZE, nc->send_mbuf.len);
  }
  int ret = mg_ssl_if_write(nc, nc->send_mbuf.buf, len);
  DBG(("%p SSL_write %u = %d", nc, len, ret));
  if (ret > 0) {
    mg_if_sent_cb(nc, ret);
    cs->last_ssl_write_size = 0;
  } else if (ret < 0) {
    /* This is tricky. We must remember the exact data we were sending to retry
     * exactly the same send next time. */
    cs->last_ssl_write_size = len;
  }
  if (ret == len) {
    nc->flags &= ~MG_F_WANT_WRITE;
  } else if (ret == MG_SSL_WANT_WRITE) {
    nc->flags |= MG_F_WANT_WRITE;
  } else {
    mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
  }
}

void mg_lwip_ssl_recv(struct mg_connection *nc) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  /* Don't deliver data before connect callback */
  if (nc->flags & MG_F_CONNECTING) return;
  while (nc->recv_mbuf.len < nc->recv_mbuf_limit) {
    char *buf = (char *) MG_MALLOC(MG_LWIP_SSL_IO_SIZE);
    if (buf == NULL) return;
    int ret = mg_ssl_if_read(nc, buf, MG_LWIP_SSL_IO_SIZE);
    DBG(("%p %p SSL_read %u = %d", nc, cs->rx_chain, MG_LWIP_SSL_IO_SIZE, ret));
    if (ret <= 0) {
      MG_FREE(buf);
      if (ret == MG_SSL_WANT_WRITE) {
        nc->flags |= MG_F_WANT_WRITE;
        return;
      } else if (ret == MG_SSL_WANT_READ) {
        /*
         * Nothing to do in particular, we are callback-driven.
         * What we definitely do not need anymore is SSL reading (nothing left).
         */
        nc->flags &= ~MG_F_WANT_READ;
        cs->err = 0;
        return;
      } else {
        mg_lwip_post_signal(MG_SIG_CLOSE_CONN, nc);
        return;
      }
    } else {
      mg_if_recv_tcp_cb(nc, buf, ret, 1 /* own */);
    }
  }
}

#ifdef KR_VERSION

ssize_t kr_send(int fd, const void *buf, size_t len) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) fd;
  int ret = mg_lwip_tcp_write(cs->nc, buf, len);
  DBG(("%p mg_lwip_tcp_write %u = %d", cs->nc, len, ret));
  if (ret == 0) ret = KR_IO_WOULDBLOCK;
  return ret;
}

ssize_t kr_recv(int fd, void *buf, size_t len) {
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) fd;
  struct pbuf *seg = cs->rx_chain;
  if (seg == NULL) {
    DBG(("%u - nothing to read", len));
    return KR_IO_WOULDBLOCK;
  }
  size_t seg_len = (seg->len - cs->rx_offset);
  DBG(("%u %u %u %u", len, cs->rx_chain->len, seg_len, cs->rx_chain->tot_len));
  len = MIN(len, seg_len);
  pbuf_copy_partial(seg, buf, len, cs->rx_offset);
  cs->rx_offset += len;
  tcp_recved(cs->pcb.tcp, len);
  if (cs->rx_offset == cs->rx_chain->len) {
    cs->rx_chain = pbuf_dechain(cs->rx_chain);
    pbuf_free(seg);
    cs->rx_offset = 0;
  }
  return len;
}

#elif MG_SSL_IF == MG_SSL_IF_MBEDTLS

int ssl_socket_send(void *ctx, const unsigned char *buf, size_t len) {
  struct mg_connection *nc = (struct mg_connection *) ctx;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  int ret = mg_lwip_tcp_write(cs->nc, buf, len);
  if (ret == 0) ret = MBEDTLS_ERR_SSL_WANT_WRITE;
  LOG(LL_DEBUG, ("%p %d -> %d", nc, len, ret));
  return ret;
}

int ssl_socket_recv(void *ctx, unsigned char *buf, size_t len) {
  struct mg_connection *nc = (struct mg_connection *) ctx;
  struct mg_lwip_conn_state *cs = (struct mg_lwip_conn_state *) nc->sock;
  struct pbuf *seg = cs->rx_chain;
  if (seg == NULL) {
    DBG(("%u - nothing to read", len));
    return MBEDTLS_ERR_SSL_WANT_READ;
  }
  size_t seg_len = (seg->len - cs->rx_offset);
  DBG(("%u %u %u %u", len, cs->rx_chain->len, seg_len, cs->rx_chain->tot_len));
  mgos_lock();
  len = MIN(len, seg_len);
  pbuf_copy_partial(seg, buf, len, cs->rx_offset);
  cs->rx_offset += len;
  /* TCP PCB may be NULL if connection has already been closed
   * but we still have data to deliver to SSL. */
  if (cs->pcb.tcp != NULL) tcp_recved(cs->pcb.tcp, len);
  if (cs->rx_offset == cs->rx_chain->len) {
    cs->rx_chain = pbuf_dechain(cs->rx_chain);
    pbuf_free(seg);
    cs->rx_offset = 0;
  }
  mgos_unlock();
  LOG(LL_DEBUG, ("%p <- %d", nc, (int) len));
  return len;
}

#endif

#endif /* MG_ENABLE_SSL && MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL */
