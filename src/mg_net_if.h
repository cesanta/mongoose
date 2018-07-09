/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_NET_IF_H_
#define CS_MONGOOSE_SRC_NET_IF_H_

#include "common/platform.h"

/*
 * Internal async networking core interface.
 * Consists of calls made by the core, which should not block,
 * and callbacks back into the core ("..._cb").
 * Callbacks may (will) cause methods to be invoked from within,
 * but methods are not allowed to invoke callbacks inline.
 *
 * Implementation must ensure that only one callback is invoked at any time.
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MG_MAIN_IFACE 0

struct mg_mgr;
struct mg_connection;
union socket_address;

struct mg_iface_vtable;

struct mg_iface {
  struct mg_mgr *mgr;
  void *data; /* Implementation-specific data */
  const struct mg_iface_vtable *vtable;
};

struct mg_iface_vtable {
  void (*init)(struct mg_iface *iface);
  void (*free)(struct mg_iface *iface);
  void (*add_conn)(struct mg_connection *nc);
  void (*remove_conn)(struct mg_connection *nc);
  time_t (*poll)(struct mg_iface *iface, int timeout_ms);

  /* Set up a listening TCP socket on a given address. rv = 0 -> ok. */
  int (*listen_tcp)(struct mg_connection *nc, union socket_address *sa);
  /* Request that a "listening" UDP socket be created. */
  int (*listen_udp)(struct mg_connection *nc, union socket_address *sa);

  /* Request that a TCP connection is made to the specified address. */
  void (*connect_tcp)(struct mg_connection *nc, const union socket_address *sa);
  /* Open a UDP socket. Doesn't actually connect anything. */
  void (*connect_udp)(struct mg_connection *nc);

  /* Send functions for TCP and UDP. Sent data is copied before return. */
  int (*tcp_send)(struct mg_connection *nc, const void *buf, size_t len);
  int (*udp_send)(struct mg_connection *nc, const void *buf, size_t len);

  int (*tcp_recv)(struct mg_connection *nc, void *buf, size_t len);
  int (*udp_recv)(struct mg_connection *nc, void *buf, size_t len,
                  union socket_address *sa, size_t *sa_len);

  /* Perform interface-related connection initialization. Return 1 on ok. */
  int (*create_conn)(struct mg_connection *nc);
  /* Perform interface-related cleanup on connection before destruction. */
  void (*destroy_conn)(struct mg_connection *nc);

  /* Associate a socket to a connection. */
  void (*sock_set)(struct mg_connection *nc, sock_t sock);

  /* Put connection's address into *sa, local (remote = 0) or remote. */
  void (*get_conn_addr)(struct mg_connection *nc, int remote,
                        union socket_address *sa);
};

extern const struct mg_iface_vtable *mg_ifaces[];
extern int mg_num_ifaces;

/* Creates a new interface instance. */
struct mg_iface *mg_if_create_iface(const struct mg_iface_vtable *vtable,
                                    struct mg_mgr *mgr);

/*
 * Find an interface with a given implementation. The search is started from
 * interface `from`, exclusive. Returns NULL if none is found.
 */
struct mg_iface *mg_find_iface(struct mg_mgr *mgr,
                               const struct mg_iface_vtable *vtable,
                               struct mg_iface *from);
/*
 * Deliver a new TCP connection. Returns NULL in case on error (unable to
 * create connection, in which case interface state should be discarded.
 * This is phase 1 of the two-phase process - MG_EV_ACCEPT will be delivered
 * when mg_if_accept_tcp_cb is invoked.
 */
struct mg_connection *mg_if_accept_new_conn(struct mg_connection *lc);
void mg_if_accept_tcp_cb(struct mg_connection *nc, union socket_address *sa,
                         size_t sa_len);

/* Callback invoked by connect methods. err = 0 -> ok, != 0 -> error. */
void mg_if_connect_cb(struct mg_connection *nc, int err);
/*
 * Callback that tells the core that data can be received.
 * Core will use tcp/udp_recv to retrieve the data.
 */
void mg_if_can_recv_cb(struct mg_connection *nc);
void mg_if_can_send_cb(struct mg_connection *nc);
/*
 * Receive callback.
 * buf must be heap-allocated and ownership is transferred to the core.
 */
void mg_if_recv_udp_cb(struct mg_connection *nc, void *buf, int len,
                       union socket_address *sa, size_t sa_len);

/* void mg_if_close_conn(struct mg_connection *nc); */

/* Deliver a POLL event to the connection. */
int mg_if_poll(struct mg_connection *nc, double now);

/*
 * Return minimal timer value amoung connections in the manager.
 * Returns 0 if there aren't any timers.
 */
double mg_mgr_min_timer(const struct mg_mgr *mgr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_NET_IF_H_ */
