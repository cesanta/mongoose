/*
 * Copyright (c) 2018 Cesanta Software Limited
 * All rights reserved
 *
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

static void mg_null_if_connect_tcp(struct mg_connection *c,
                                   const union socket_address *sa) {
  c->flags |= MG_F_CLOSE_IMMEDIATELY;
  (void) sa;
}

static void mg_null_if_connect_udp(struct mg_connection *c) {
  c->flags |= MG_F_CLOSE_IMMEDIATELY;
}

static int mg_null_if_listen_tcp(struct mg_connection *c,
                                 union socket_address *sa) {
  (void) c;
  (void) sa;
  return -1;
}

static int mg_null_if_listen_udp(struct mg_connection *c,
                                 union socket_address *sa) {
  (void) c;
  (void) sa;
  return -1;
}

static int mg_null_if_tcp_send(struct mg_connection *c, const void *buf,
                               size_t len) {
  (void) c;
  (void) buf;
  (void) len;
  return -1;
}

static int mg_null_if_udp_send(struct mg_connection *c, const void *buf,
                               size_t len) {
  (void) c;
  (void) buf;
  (void) len;
  return -1;
}

int mg_null_if_tcp_recv(struct mg_connection *c, void *buf, size_t len) {
  (void) c;
  (void) buf;
  (void) len;
  return -1;
}

int mg_null_if_udp_recv(struct mg_connection *c, void *buf, size_t len,
                        union socket_address *sa, size_t *sa_len) {
  (void) c;
  (void) buf;
  (void) len;
  (void) sa;
  (void) sa_len;
  return -1;
}

static int mg_null_if_create_conn(struct mg_connection *c) {
  (void) c;
  return 1;
}

static void mg_null_if_destroy_conn(struct mg_connection *c) {
  (void) c;
}

static void mg_null_if_sock_set(struct mg_connection *c, sock_t sock) {
  (void) c;
  (void) sock;
}

static void mg_null_if_init(struct mg_iface *iface) {
  (void) iface;
}

static void mg_null_if_free(struct mg_iface *iface) {
  (void) iface;
}

static void mg_null_if_add_conn(struct mg_connection *c) {
  c->sock = INVALID_SOCKET;
  c->flags |= MG_F_CLOSE_IMMEDIATELY;
}

static void mg_null_if_remove_conn(struct mg_connection *c) {
  (void) c;
}

static time_t mg_null_if_poll(struct mg_iface *iface, int timeout_ms) {
  struct mg_mgr *mgr = iface->mgr;
  struct mg_connection *nc, *tmp;
  double now = mg_time();
  /* We basically just run timers and poll. */
  for (nc = mgr->active_connections; nc != NULL; nc = tmp) {
    tmp = nc->next;
    mg_if_poll(nc, now);
  }
  (void) timeout_ms;
  return (time_t) now;
}

static void mg_null_if_get_conn_addr(struct mg_connection *c, int remote,
                                     union socket_address *sa) {
  (void) c;
  (void) remote;
  (void) sa;
}

#define MG_NULL_IFACE_VTABLE                                                   \
  {                                                                            \
    mg_null_if_init, mg_null_if_free, mg_null_if_add_conn,                     \
        mg_null_if_remove_conn, mg_null_if_poll, mg_null_if_listen_tcp,        \
        mg_null_if_listen_udp, mg_null_if_connect_tcp, mg_null_if_connect_udp, \
        mg_null_if_tcp_send, mg_null_if_udp_send, mg_null_if_tcp_recv,         \
        mg_null_if_udp_recv, mg_null_if_create_conn, mg_null_if_destroy_conn,  \
        mg_null_if_sock_set, mg_null_if_get_conn_addr,                         \
  }

const struct mg_iface_vtable mg_null_iface_vtable = MG_NULL_IFACE_VTABLE;

#if MG_NET_IF == MG_NET_IF_NULL
const struct mg_iface_vtable mg_default_iface_vtable = MG_NULL_IFACE_VTABLE;
#endif /* MG_NET_IF == MG_NET_IF_NULL */
