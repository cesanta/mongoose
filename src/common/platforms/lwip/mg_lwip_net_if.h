/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_LWIP_MG_NET_IF_LWIP_H_
#define CS_COMMON_PLATFORMS_LWIP_MG_NET_IF_LWIP_H_

#ifndef MG_ENABLE_NET_IF_LWIP_LOW_LEVEL
#define MG_ENABLE_NET_IF_LWIP_LOW_LEVEL MG_NET_IF == MG_NET_IF_LWIP_LOW_LEVEL
#endif

#if MG_ENABLE_NET_IF_LWIP_LOW_LEVEL

#include <stdint.h>

extern const struct mg_iface_vtable mg_lwip_iface_vtable;

struct mg_lwip_conn_state {
  struct mg_connection *nc;
  struct mg_connection *lc;
  union {
    struct tcp_pcb *tcp;
    struct udp_pcb *udp;
  } pcb;
  err_t err;
  size_t num_sent; /* Number of acknowledged bytes to be reported to the core */
  struct pbuf *rx_chain; /* Chain of incoming data segments. */
  size_t rx_offset; /* Offset within the first pbuf (if partially consumed) */
  /* Last SSL write size, for retries. */
  int last_ssl_write_size;
  /* Whether MG_SIG_RECV is already pending for this connection */
  int recv_pending : 1;
  /* Whether the connection is about to close, just `rx_chain` needs to drain */
  int draining_rx_chain : 1;
};

enum mg_sig_type {
  MG_SIG_CONNECT_RESULT = 1,
  MG_SIG_RECV = 2,
  MG_SIG_CLOSE_CONN = 3,
  MG_SIG_TOMBSTONE = 4,
  MG_SIG_ACCEPT = 5,
};

void mg_lwip_post_signal(enum mg_sig_type sig, struct mg_connection *nc);

/* To be implemented by the platform. */
void mg_lwip_mgr_schedule_poll(struct mg_mgr *mgr);

#endif /* MG_ENABLE_NET_IF_LWIP_LOW_LEVEL */

#endif /* CS_COMMON_PLATFORMS_LWIP_MG_NET_IF_LWIP_H_ */
