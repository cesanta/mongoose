// TLS / Security
#pragma once

// Available TLS backend libraries
#define MG_TLS_NONE 0     // No TLS support
#define MG_TLS_MBED 1     // mbedTLS
#define MG_TLS_OPENSSL 2  // OpenSSL
#define MG_TLS_WOLFSSL 5  // WolfSSL (based on OpenSSL)
#define MG_TLS_BUILTIN 3  // Built-in
#define MG_TLS_CUSTOM 4   // Custom implementation

#ifndef MG_TLS
// mongoose_config.h setting. Set MG_TLS to one of the MG_TLS_* values above to
// select a TLS backend. Defaults to MG_TLS_NONE (no TLS).
#define MG_TLS MG_TLS_NONE
#endif

#include "net.h"
#include "tls_mbed.h"
#include "tls_openssl.h"

// TLS options structure passed to mg_tls_init(). All cert/key fields accept
// PEM strings or DER binary.
//
// One-way TLS:
// server sets `cert` + `key`,
// client sets `ca` + optionally `name` for hostname verification.
//
// Two-way (mutual) TLS: both sides set `ca` + `cert` + `key`.
//
// - `ca`: CA certificate. Verifies the peer's certificate.
//   Set on clients to authenticate the server. Set on servers to require
//   and verify a client certificate. If empty, peer is not verified.
// - `cert`: Our certificate. Required on servers. Also set on clients for
//   mutual TLS.
// - `key`: Our private key. May equal `cert` when PEM bundles both.
// - `name`: Server name for SNI and hostname verification. Set on clients.
//   Empty disables hostname verification.
// - `skip_verification`: Skip certificate and hostname verification.
//   Useful during development; do not use in production.
struct mg_tls_opts {
  struct mg_str ca;       // CA certificate, PEM or DER
  struct mg_str cert;     // Our certificate, PEM or DER
  struct mg_str key;      // Our private key, PEM or DER
  struct mg_str name;     // Server name for SNI + hostname verification
  bool skip_verification;  // Skip certificate and hostname verification
};

// Initialises TLS on a connection.
//
// Example:
// ```c
// // Server: one-way TLS
// if (ev == MG_EV_ACCEPT) {
//   struct mg_tls_opts opts = {.cert = mg_str(s_tls_cert),
//                              .key  = mg_str(s_tls_key)};
//   mg_tls_init(c, &opts);
// }
//
// // Client: verify server certificate and hostname
// if (ev == MG_EV_CONNECT) {
//   struct mg_tls_opts opts = {.ca   = mg_str(s_tls_ca),
//                              .name = mg_str("hostname")};
//   mg_tls_init(c, &opts);
// }
// ```
// Full examples:
//   tutorials/http/http-server, tutorials/http/http-client,
//   tutorials/mqtt/mqtt-client-aws-iot, tutorials/websocket/websocket-client
// Related APIs:
//   mg_http_listen(), mg_http_connect(), mg_ws_connect(), mg_mqtt_connect()
// Notes:
//   Call from the user-supplied event handler on MG_EV_ACCEPT for servers or
//   MG_EV_CONNECT for clients, before application data is sent. Servers usually
//   set cert and key. Clients usually set ca and name; name enables SNI and
//   hostname verification.
void mg_tls_init(struct mg_connection *c, const struct mg_tls_opts *opts);

// Private API, do not expose
void mg_tls_free(struct mg_connection *);
long mg_tls_send(struct mg_connection *, const void *buf, size_t len);
long mg_tls_recv(struct mg_connection *, void *buf, size_t len);
size_t mg_tls_pending(struct mg_connection *);
void mg_tls_flush(struct mg_connection *);
void mg_tls_handshake(struct mg_connection *);
void mg_tls_ctx_init(struct mg_mgr *);
void mg_tls_ctx_free(struct mg_mgr *);
#define MG_IS_DER(buf) (((uint8_t *) (buf))[0] == 0x30)  // DER begins with 0x30

// Low-level IO primitives used by TLS layer
enum { MG_IO_ERR = -1, MG_IO_WAIT = -2 };
long mg_io_send(struct mg_connection *c, const void *buf, size_t len);
long mg_io_recv(struct mg_connection *c, void *buf, size_t len);
