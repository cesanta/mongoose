/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#if MG_ENABLE_TUN

#include "common/cs_dbg.h"
#include "common/cs_time.h"
#include "mongoose/src/internal.h"
#include "mongoose/src/net_if_tun.h"
#include "mongoose/src/tun.h"
#include "mongoose/src/util.h"

#define MG_TCP_RECV_BUFFER_SIZE 1024
#define MG_UDP_RECV_BUFFER_SIZE 1500

void mg_tun_if_connect_tcp(struct mg_connection *nc,
                           const union socket_address *sa) {
  (void) nc;
  (void) sa;
}

void mg_tun_if_connect_udp(struct mg_connection *nc) {
  (void) nc;
}

int mg_tun_if_listen_tcp(struct mg_connection *nc, union socket_address *sa) {
  (void) nc;
  (void) sa;
  return 0;
}

int mg_tun_if_listen_udp(struct mg_connection *nc, union socket_address *sa) {
  (void) nc;
  (void) sa;
  return -1;
}

void mg_tun_if_tcp_send(struct mg_connection *nc, const void *buf, size_t len) {
  struct mg_tun_client *client = (struct mg_tun_client *) nc->iface->data;
  uint32_t stream_id = (uint32_t)(uintptr_t) nc->mgr_data;
  struct mg_str msg = {(char *) buf, len};
#if MG_ENABLE_HEXDUMP
  char hex[512];
  mg_hexdump(buf, len, hex, sizeof(hex));
  LOG(LL_DEBUG, ("sending to stream 0x%x:\n%s", (unsigned int) stream_id, hex));
#endif

  mg_tun_send_frame(client->disp, stream_id, MG_TUN_DATA_FRAME, 0, msg);
}

void mg_tun_if_udp_send(struct mg_connection *nc, const void *buf, size_t len) {
  (void) nc;
  (void) buf;
  (void) len;
}

void mg_tun_if_recved(struct mg_connection *nc, size_t len) {
  (void) nc;
  (void) len;
}

int mg_tun_if_create_conn(struct mg_connection *nc) {
  (void) nc;
  return 1;
}

void mg_tun_if_destroy_conn(struct mg_connection *nc) {
  struct mg_tun_client *client = (struct mg_tun_client *) nc->iface->data;

  if (nc->flags & MG_F_LISTENING) {
    mg_tun_destroy_client(client);
  } else if (client->disp) {
    uint32_t stream_id = (uint32_t)(uintptr_t) nc->mgr_data;
    struct mg_str msg = {NULL, 0};

    LOG(LL_DEBUG, ("closing 0x%x:", (unsigned int) stream_id));
    mg_tun_send_frame(client->disp, stream_id, MG_TUN_DATA_FRAME,
                      MG_TUN_F_END_STREAM, msg);
  }
}

/* Associate a socket to a connection. */
void mg_tun_if_sock_set(struct mg_connection *nc, sock_t sock) {
  (void) nc;
  (void) sock;
}

void mg_tun_if_init(struct mg_iface *iface) {
  (void) iface;
}

void mg_tun_if_free(struct mg_iface *iface) {
  (void) iface;
}

void mg_tun_if_add_conn(struct mg_connection *nc) {
  nc->sock = INVALID_SOCKET;
}

void mg_tun_if_remove_conn(struct mg_connection *nc) {
  (void) nc;
}

time_t mg_tun_if_poll(struct mg_iface *iface, int timeout_ms) {
  (void) iface;
  (void) timeout_ms;
  return (time_t) cs_time();
}

void mg_tun_if_get_conn_addr(struct mg_connection *nc, int remote,
                             union socket_address *sa) {
  (void) nc;
  (void) remote;
  (void) sa;
}

struct mg_connection *mg_tun_if_find_conn(struct mg_tun_client *client,
                                          uint32_t stream_id) {
  struct mg_connection *nc = NULL;

  for (nc = client->mgr->active_connections; nc != NULL; nc = nc->next) {
    if (nc->iface != client->iface || (nc->flags & MG_F_LISTENING)) {
      continue;
    }
    if (stream_id == (uint32_t)(uintptr_t) nc->mgr_data) {
      return nc;
    }
  }

  if (stream_id > client->last_stream_id) {
    /* create a new connection */
    LOG(LL_DEBUG, ("new stream 0x%x, accepting", (unsigned int) stream_id));
    nc = mg_if_accept_new_conn(client->listener);
    nc->mgr_data = (void *) (uintptr_t) stream_id;
    client->last_stream_id = stream_id;
  } else {
    LOG(LL_DEBUG,
        ("Ignoring stream 0x%x (last_stream_id 0x%x)", (unsigned int) stream_id,
         (unsigned int) client->last_stream_id));
  }

  return nc;
}

/* clang-format off */
#define MG_TUN_IFACE_VTABLE                                             \
  {                                                                     \
    mg_tun_if_init,                                                     \
    mg_tun_if_free,                                                     \
    mg_tun_if_add_conn,                                                 \
    mg_tun_if_remove_conn,                                              \
    mg_tun_if_poll,                                                     \
    mg_tun_if_listen_tcp,                                               \
    mg_tun_if_listen_udp,                                               \
    mg_tun_if_connect_tcp,                                              \
    mg_tun_if_connect_udp,                                              \
    mg_tun_if_tcp_send,                                                 \
    mg_tun_if_udp_send,                                                 \
    mg_tun_if_recved,                                                   \
    mg_tun_if_create_conn,                                              \
    mg_tun_if_destroy_conn,                                             \
    mg_tun_if_sock_set,                                                 \
    mg_tun_if_get_conn_addr,                                            \
  }
/* clang-format on */

const struct mg_iface_vtable mg_tun_iface_vtable = MG_TUN_IFACE_VTABLE;

#endif /* MG_ENABLE_TUN */
