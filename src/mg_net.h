/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
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

/*
 * === Core API: TCP/UDP/SSL
 *
 * NOTE: Mongoose manager is single threaded. It does not protect
 * its data structures by mutexes, therefore all functions that are dealing
 * with a particular event manager should be called from the same thread,
 * with exception of the `mg_broadcast()` function. It is fine to have different
 * event managers handled by different threads.
 */

#ifndef CS_MONGOOSE_SRC_NET_H_
#define CS_MONGOOSE_SRC_NET_H_

#include "mg_common.h"
#include "mg_net_if.h"
#include "common/mbuf.h"

#ifndef MG_VPRINTF_BUFFER_SIZE
#define MG_VPRINTF_BUFFER_SIZE 100
#endif

#ifdef MG_USE_READ_WRITE
#define MG_RECV_FUNC(s, b, l, f) read(s, b, l)
#define MG_SEND_FUNC(s, b, l, f) write(s, b, l)
#else
#define MG_RECV_FUNC(s, b, l, f) recv(s, b, l, f)
#define MG_SEND_FUNC(s, b, l, f) send(s, b, l, f)
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

union socket_address {
  struct sockaddr sa;
  struct sockaddr_in sin;
#if MG_ENABLE_IPV6
  struct sockaddr_in6 sin6;
#else
  struct sockaddr sin6;
#endif
};

struct mg_connection;

/*
 * Callback function (event handler) prototype. Must be defined by the user.
 * Mongoose calls the event handler, passing the events defined below.
 */
typedef void (*mg_event_handler_t)(struct mg_connection *nc, int ev,
                                   void *ev_data MG_UD_ARG(void *user_data));

/* Events. Meaning of event parameter (evp) is given in the comment. */
#define MG_EV_POLL 0    /* Sent to each connection on each mg_mgr_poll() call */
#define MG_EV_ACCEPT 1  /* New connection accepted. union socket_address * */
#define MG_EV_CONNECT 2 /* connect() succeeded or failed. int *  */
#define MG_EV_RECV 3    /* Data has been received. int *num_bytes */
#define MG_EV_SEND 4    /* Data has been written to a socket. int *num_bytes */
#define MG_EV_CLOSE 5   /* Connection is closed. NULL */
#define MG_EV_TIMER 6   /* now >= conn->ev_timer_time. double * */

/*
 * Mongoose event manager.
 */
struct mg_mgr {
  struct mg_connection *active_connections;
#if MG_ENABLE_HEXDUMP
  const char *hexdump_file; /* Debug hexdump file path */
#endif
#if MG_ENABLE_BROADCAST
  sock_t ctl[2]; /* Socketpair for mg_broadcast() */
#endif
  void *user_data; /* User data */
  int num_ifaces;
  struct mg_iface **ifaces; /* network interfaces */
  const char *nameserver;   /* DNS server to use */
};

/*
 * Mongoose connection.
 */
struct mg_connection {
  struct mg_connection *next, *prev; /* mg_mgr::active_connections linkage */
  struct mg_connection *listener;    /* Set only for accept()-ed connections */
  struct mg_mgr *mgr;                /* Pointer to containing manager */

  sock_t sock; /* Socket to the remote peer */
  int err;
  union socket_address sa; /* Remote peer address */
  size_t recv_mbuf_limit;  /* Max size of recv buffer */
  struct mbuf recv_mbuf;   /* Received data */
  struct mbuf send_mbuf;   /* Data scheduled for sending */
  time_t last_io_time;     /* Timestamp of the last socket IO */
  double ev_timer_time;    /* Timestamp of the future MG_EV_TIMER */
#if MG_ENABLE_SSL
  void *ssl_if_data; /* SSL library data. */
#endif
  mg_event_handler_t proto_handler; /* Protocol-specific event handler */
  void *proto_data;                 /* Protocol-specific data */
  void (*proto_data_destructor)(void *proto_data);
  mg_event_handler_t handler; /* Event handler function */
  void *user_data;            /* User-specific data */
  union {
    void *v;
    /*
     * the C standard is fussy about fitting function pointers into
     * void pointers, since some archs might have fat pointers for functions.
     */
    mg_event_handler_t f;
  } priv_1;
  void *priv_2;
  void *mgr_data; /* Implementation-specific event manager's data. */
  struct mg_iface *iface;
  unsigned long flags;
/* Flags set by Mongoose */
#define MG_F_LISTENING (1 << 0)          /* This connection is listening */
#define MG_F_UDP (1 << 1)                /* This connection is UDP */
#define MG_F_RESOLVING (1 << 2)          /* Waiting for async resolver */
#define MG_F_CONNECTING (1 << 3)         /* connect() call in progress */
#define MG_F_SSL (1 << 4)                /* SSL is enabled on the connection */
#define MG_F_SSL_HANDSHAKE_DONE (1 << 5) /* SSL hanshake has completed */
#define MG_F_WANT_READ (1 << 6)          /* SSL specific */
#define MG_F_WANT_WRITE (1 << 7)         /* SSL specific */
#define MG_F_IS_WEBSOCKET (1 << 8)       /* Websocket specific */

/* Flags that are settable by user */
#define MG_F_SEND_AND_CLOSE (1 << 10)      /* Push remaining data and close  */
#define MG_F_CLOSE_IMMEDIATELY (1 << 11)   /* Disconnect */
#define MG_F_WEBSOCKET_NO_DEFRAG (1 << 12) /* Websocket specific */
#define MG_F_DELETE_CHUNK (1 << 13)        /* HTTP specific */
#define MG_F_ENABLE_BROADCAST (1 << 14)    /* Allow broadcast address usage */

#define MG_F_USER_1 (1 << 20) /* Flags left for application */
#define MG_F_USER_2 (1 << 21)
#define MG_F_USER_3 (1 << 22)
#define MG_F_USER_4 (1 << 23)
#define MG_F_USER_5 (1 << 24)
#define MG_F_USER_6 (1 << 25)
};

/*
 * Initialise Mongoose manager. Side effect: ignores SIGPIPE signal.
 * `mgr->user_data` field will be initialised with a `user_data` parameter.
 * That is an arbitrary pointer, where the user code can associate some data
 * with the particular Mongoose manager. For example, a C++ wrapper class
 * could be written in which case `user_data` can hold a pointer to the
 * class instance.
 */
void mg_mgr_init(struct mg_mgr *mgr, void *user_data);

/*
 * Optional parameters to `mg_mgr_init_opt()`.
 *
 * If `main_iface` is not NULL, it will be used as the main interface in the
 * default interface set. The pointer will be free'd by `mg_mgr_free`.
 * Otherwise, the main interface will be autodetected based on the current
 * platform.
 *
 * If `num_ifaces` is 0 and `ifaces` is NULL, the default interface set will be
 * used.
 * This is an advanced option, as it requires you to construct a full interface
 * set, including special networking interfaces required by some optional
 * features such as TCP tunneling. Memory backing `ifaces` and each of the
 * `num_ifaces` pointers it contains will be reclaimed by `mg_mgr_free`.
 */
struct mg_mgr_init_opts {
  const struct mg_iface_vtable *main_iface;
  int num_ifaces;
  const struct mg_iface_vtable **ifaces;
  const char *nameserver;
};

/*
 * Like `mg_mgr_init` but with more options.
 *
 * Notably, this allows you to create a manger and choose
 * dynamically which networking interface implementation to use.
 */
void mg_mgr_init_opt(struct mg_mgr *mgr, void *user_data,
                     struct mg_mgr_init_opts opts);

/*
 * De-initialises Mongoose manager.
 *
 * Closes and deallocates all active connections.
 */
void mg_mgr_free(struct mg_mgr *);

/*
 * This function performs the actual IO and must be called in a loop
 * (an event loop). It returns the current timestamp.
 * `milli` is the maximum number of milliseconds to sleep.
 * `mg_mgr_poll()` checks all connections for IO readiness. If at least one
 * of the connections is IO-ready, `mg_mgr_poll()` triggers the respective
 * event handlers and returns.
 */
time_t mg_mgr_poll(struct mg_mgr *, int milli);

#if MG_ENABLE_BROADCAST
/*
 * Passes a message of a given length to all connections.
 *
 * Must be called from a thread that does NOT call `mg_mgr_poll()`.
 * Note that `mg_broadcast()` is the only function
 * that can be, and must be, called from a different (non-IO) thread.
 *
 * `func` callback function will be called by the IO thread for each
 * connection. When called, the event will be `MG_EV_POLL`, and a message will
 * be passed as the `ev_data` pointer. Maximum message size is capped
 * by `MG_CTL_MSG_MESSAGE_SIZE` which is set to 8192 bytes.
 */
void mg_broadcast(struct mg_mgr *mgr, mg_event_handler_t cb, void *data,
                  size_t len);
#endif

/*
 * Iterates over all active connections.
 *
 * Returns the next connection from the list
 * of active connections or `NULL` if there are no more connections. Below
 * is the iteration idiom:
 *
 * ```c
 * for (c = mg_next(srv, NULL); c != NULL; c = mg_next(srv, c)) {
 *   // Do something with connection `c`
 * }
 * ```
 */
struct mg_connection *mg_next(struct mg_mgr *mgr, struct mg_connection *c);

/*
 * Optional parameters to `mg_add_sock_opt()`.
 *
 * `flags` is an initial `struct mg_connection::flags` bitmask to set,
 * see `MG_F_*` flags definitions.
 */
struct mg_add_sock_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Initial connection flags */
  const char **error_string; /* Placeholder for the error string */
  struct mg_iface *iface;    /* Interface instance */
};

/*
 * Creates a connection, associates it with the given socket and event handler
 * and adds it to the manager.
 *
 * For more options see the `mg_add_sock_opt` variant.
 */
struct mg_connection *mg_add_sock(struct mg_mgr *mgr, sock_t sock,
                                  MG_CB(mg_event_handler_t handler,
                                        void *user_data));

/*
 * Creates a connection, associates it with the given socket and event handler
 * and adds to the manager.
 *
 * See the `mg_add_sock_opts` structure for a description of the options.
 */
struct mg_connection *mg_add_sock_opt(struct mg_mgr *mgr, sock_t sock,
                                      MG_CB(mg_event_handler_t handler,
                                            void *user_data),
                                      struct mg_add_sock_opts opts);

/*
 * Optional parameters to `mg_bind_opt()`.
 *
 * `flags` is an initial `struct mg_connection::flags` bitmask to set,
 * see `MG_F_*` flags definitions.
 */
struct mg_bind_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Extra connection flags */
  const char **error_string; /* Placeholder for the error string */
  struct mg_iface *iface;    /* Interface instance */
#if MG_ENABLE_SSL
  /*
   * SSL settings.
   *
   * Server certificate to present to clients or client certificate to
   * present to tunnel dispatcher (for tunneled connections).
   */
  const char *ssl_cert;
  /* Private key corresponding to the certificate. If ssl_cert is set but
   * ssl_key is not, ssl_cert is used. */
  const char *ssl_key;
  /* CA bundle used to verify client certificates or tunnel dispatchers. */
  const char *ssl_ca_cert;
  /* Colon-delimited list of acceptable cipher suites.
   * Names depend on the library used, for example:
   *
   * ECDH-ECDSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256 (OpenSSL)
   * TLS-ECDH-ECDSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-128-GCM-SHA256
   *   (mbedTLS)
   *
   * For OpenSSL the list can be obtained by running "openssl ciphers".
   * For mbedTLS, names can be found in library/ssl_ciphersuites.c
   * If NULL, a reasonable default is used.
   */
  const char *ssl_cipher_suites;
#endif
};

/*
 * Creates a listening connection.
 *
 * See `mg_bind_opt` for full documentation.
 */
struct mg_connection *mg_bind(struct mg_mgr *mgr, const char *address,
                              MG_CB(mg_event_handler_t handler,
                                    void *user_data));
/*
 * Creates a listening connection.
 *
 * The `address` parameter specifies which address to bind to. It's format is
 * the same as for the `mg_connect()` call, where `HOST` part is optional.
 * `address` can be just a port number, e.g. `:8000`. To bind to a specific
 * interface, an IP address can be specified, e.g. `1.2.3.4:8000`. By default,
 * a TCP connection is created. To create UDP connection, prepend `udp://`
 * prefix, e.g. `udp://:8000`. To summarize, `address` parameter has following
 * format: `[PROTO://][IP_ADDRESS]:PORT`, where `PROTO` could be `tcp` or
 * `udp`.
 *
 * See the `mg_bind_opts` structure for a description of the optional
 * parameters.
 *
 * Returns a new listening connection or `NULL` on error.
 * NOTE: The connection remains owned by the manager, do not free().
 */
struct mg_connection *mg_bind_opt(struct mg_mgr *mgr, const char *address,
                                  MG_CB(mg_event_handler_t handler,
                                        void *user_data),
                                  struct mg_bind_opts opts);

/* Optional parameters to `mg_connect_opt()` */
struct mg_connect_opts {
  void *user_data;           /* Initial value for connection's user_data */
  unsigned int flags;        /* Extra connection flags */
  const char **error_string; /* Placeholder for the error string */
  struct mg_iface *iface;    /* Interface instance */
  const char *nameserver;    /* DNS server to use, NULL for default */
#if MG_ENABLE_SSL
  /*
   * SSL settings.
   * Client certificate to present to the server.
   */
  const char *ssl_cert;
  /*
   * Private key corresponding to the certificate.
   * If ssl_cert is set but ssl_key is not, ssl_cert is used.
   */
  const char *ssl_key;
  /*
   * Verify server certificate using this CA bundle. If set to "*", then SSL
   * is enabled but no cert verification is performed.
   */
  const char *ssl_ca_cert;
  /* Colon-delimited list of acceptable cipher suites.
   * Names depend on the library used, for example:
   *
   * ECDH-ECDSA-AES128-GCM-SHA256:DHE-RSA-AES128-SHA256 (OpenSSL)
   * TLS-ECDH-ECDSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-128-GCM-SHA256
   *   (mbedTLS)
   *
   * For OpenSSL the list can be obtained by running "openssl ciphers".
   * For mbedTLS, names can be found in library/ssl_ciphersuites.c
   * If NULL, a reasonable default is used.
   */
  const char *ssl_cipher_suites;
  /*
   * Server name verification. If ssl_ca_cert is set and the certificate has
   * passed verification, its subject will be verified against this string.
   * By default (if ssl_server_name is NULL) hostname part of the address will
   * be used. Wildcard matching is supported. A special value of "*" disables
   * name verification.
   */
  const char *ssl_server_name;
  /*
   * PSK identity and key. Identity is a NUL-terminated string and key is a hex
   * string. Key must be either 16 or 32 bytes (32 or 64 hex digits) for AES-128
   * or AES-256 respectively.
   * Note: Default list of cipher suites does not include PSK suites, if you
   * want to use PSK you will need to set ssl_cipher_suites as well.
   */
  const char *ssl_psk_identity;
  const char *ssl_psk_key;
#endif
};

/*
 * Connects to a remote host.
 *
 * See `mg_connect_opt()` for full documentation.
 */
struct mg_connection *mg_connect(struct mg_mgr *mgr, const char *address,
                                 MG_CB(mg_event_handler_t handler,
                                       void *user_data));

/*
 * Connects to a remote host.
 *
 * The `address` format is `[PROTO://]HOST:PORT`. `PROTO` could be `tcp` or
 * `udp`. `HOST` could be an IP address,
 * IPv6 address (if Mongoose is compiled with `-DMG_ENABLE_IPV6`) or a host
 * name. If `HOST` is a name, Mongoose will resolve it asynchronously. Examples
 * of valid addresses: `google.com:80`, `udp://1.2.3.4:53`, `10.0.0.1:443`,
 * `[::1]:80`
 *
 * See the `mg_connect_opts` structure for a description of the optional
 * parameters.
 *
 * Returns a new outbound connection or `NULL` on error.
 *
 * NOTE: The connection remains owned by the manager, do not free().
 *
 * NOTE: To enable IPv6 addresses `-DMG_ENABLE_IPV6` should be specified
 * in the compilation flags.
 *
 * NOTE: The new connection will receive `MG_EV_CONNECT` as its first event
 * which will report the connect success status.
 * If the asynchronous resolution fails or the `connect()` syscall fails for
 * whatever reason (e.g. with `ECONNREFUSED` or `ENETUNREACH`), then
 * `MG_EV_CONNECT` event will report failure. Code example below:
 *
 * ```c
 * static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
 *   int connect_status;
 *
 *   switch (ev) {
 *     case MG_EV_CONNECT:
 *       connect_status = * (int *) ev_data;
 *       if (connect_status == 0) {
 *         // Success
 *       } else  {
 *         // Error
 *         printf("connect() error: %s\n", strerror(connect_status));
 *       }
 *       break;
 *     ...
 *   }
 * }
 *
 *   ...
 *   mg_connect(mgr, "my_site.com:80", ev_handler);
 * ```
 */
struct mg_connection *mg_connect_opt(struct mg_mgr *mgr, const char *address,
                                     MG_CB(mg_event_handler_t handler,
                                           void *user_data),
                                     struct mg_connect_opts opts);

#if MG_ENABLE_SSL && MG_NET_IF != MG_NET_IF_SIMPLELINK
/*
 * Note: This function is deprecated. Please, use SSL options in
 * mg_connect_opt.
 *
 * Enables SSL for a given connection.
 * `cert` is a server certificate file name for a listening connection
 * or a client certificate file name for an outgoing connection.
 * The certificate files must be in PEM format. The server certificate file
 * must contain a certificate, concatenated with a private key, optionally
 * concatenated with DH parameters.
 * `ca_cert` is a CA certificate or NULL if peer verification is not
 * required.
 * Return: NULL on success or error message on error.
 */
const char *mg_set_ssl(struct mg_connection *nc, const char *cert,
                       const char *ca_cert);
#endif

/*
 * Sends data to the connection.
 *
 * Note that sending functions do not actually push data to the socket.
 * They just append data to the output buffer. MG_EV_SEND will be delivered when
 * the data has actually been pushed out.
 */
void mg_send(struct mg_connection *, const void *buf, int len);

/* Enables format string warnings for mg_printf */
#if defined(__GNUC__)
__attribute__((format(printf, 2, 3)))
#endif
/* don't separate from mg_printf declaration */

/*
 * Sends `printf`-style formatted data to the connection.
 *
 * See `mg_send` for more details on send semantics.
 */
int mg_printf(struct mg_connection *, const char *fmt, ...);

/* Same as `mg_printf()`, but takes `va_list ap` as an argument. */
int mg_vprintf(struct mg_connection *, const char *fmt, va_list ap);

/*
 * Creates a socket pair.
 * `sock_type` can be either `SOCK_STREAM` or `SOCK_DGRAM`.
 * Returns 0 on failure and 1 on success.
 */
int mg_socketpair(sock_t[2], int sock_type);

#if MG_ENABLE_SYNC_RESOLVER
/*
 * Convert domain name into IP address.
 *
 * This is a utility function. If compilation flags have
 * `-DMG_ENABLE_GETADDRINFO`, then `getaddrinfo()` call is used for name
 * resolution. Otherwise, `gethostbyname()` is used.
 *
 * CAUTION: this function can block.
 * Return 1 on success, 0 on failure.
 */
int mg_resolve(const char *domain_name, char *ip_addr_buf, size_t buf_len);
#endif

/*
 * Verify given IP address against the ACL.
 *
 * `remote_ip` - an IPv4 address to check, in host byte order
 * `acl` - a comma separated list of IP subnets: `x.x.x.x/x` or `x.x.x.x`.
 * Each subnet is
 * prepended by either a - or a + sign. A plus sign means allow, where a
 * minus sign means deny. If a subnet mask is omitted, such as `-1.2.3.4`,
 * it means that only that single IP address is denied.
 * Subnet masks may vary from 0 to 32, inclusive. The default setting
 * is to allow all access. On each request the full list is traversed,
 * and the last match wins. Example:
 *
 * `-0.0.0.0/0,+192.168/16` - deny all accesses, only allow 192.168/16 subnet
 *
 * To learn more about subnet masks, see this
 * link:https://en.wikipedia.org/wiki/Subnetwork[Wikipedia page on Subnetwork].
 *
 * Returns -1 if ACL is malformed, 0 if address is disallowed, 1 if allowed.
 */
int mg_check_ip_acl(const char *acl, uint32_t remote_ip);

/*
 * Schedules an MG_EV_TIMER event to be delivered at `timestamp` time.
 * `timestamp` is UNIX time (the number of seconds since Epoch). It is
 * `double` instead of `time_t` to allow for sub-second precision.
 * Returns the old timer value.
 *
 * Example: set the connect timeout to 1.5 seconds:
 *
 * ```
 *  c = mg_connect(&mgr, "cesanta.com", ev_handler);
 *  mg_set_timer(c, mg_time() + 1.5);
 *  ...
 *
 *  void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
 *  switch (ev) {
 *    case MG_EV_CONNECT:
 *      mg_set_timer(c, 0);  // Clear connect timer
 *      break;
 *    case MG_EV_TIMER:
 *      log("Connect timeout");
 *      c->flags |= MG_F_CLOSE_IMMEDIATELY;
 *      break;
 * ```
 */
double mg_set_timer(struct mg_connection *c, double timestamp);

/*
 * A sub-second precision version of time().
 */
double mg_time(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_NET_H_ */
