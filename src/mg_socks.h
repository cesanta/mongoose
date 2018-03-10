/*
 * Copyright (c) 2017 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_SOCKS_H_
#define CS_MONGOOSE_SRC_SOCKS_H_

#if MG_ENABLE_SOCKS

#define MG_SOCKS_VERSION 5

#define MG_SOCKS_HANDSHAKE_DONE MG_F_USER_1
#define MG_SOCKS_CONNECT_DONE MG_F_USER_2

/* SOCKS5 handshake methods */
enum mg_socks_handshake_method {
  MG_SOCKS_HANDSHAKE_NOAUTH = 0,     /* Handshake method - no authentication */
  MG_SOCKS_HANDSHAKE_GSSAPI = 1,     /* Handshake method - GSSAPI auth */
  MG_SOCKS_HANDSHAKE_USERPASS = 2,   /* Handshake method - user/password auth */
  MG_SOCKS_HANDSHAKE_FAILURE = 0xff, /* Handshake method - failure */
};

/* SOCKS5 commands */
enum mg_socks_command {
  MG_SOCKS_CMD_CONNECT = 1,       /* Command: CONNECT */
  MG_SOCKS_CMD_BIND = 2,          /* Command: BIND */
  MG_SOCKS_CMD_UDP_ASSOCIATE = 3, /* Command: UDP ASSOCIATE */
};

/* SOCKS5 address types */
enum mg_socks_address_type {
  MG_SOCKS_ADDR_IPV4 = 1,   /* Address type: IPv4 */
  MG_SOCKS_ADDR_DOMAIN = 3, /* Address type: Domain name */
  MG_SOCKS_ADDR_IPV6 = 4,   /* Address type: IPv6 */
};

/* SOCKS5 response codes */
enum mg_socks_response {
  MG_SOCKS_SUCCESS = 0,            /* Response: success */
  MG_SOCKS_FAILURE = 1,            /* Response: failure */
  MG_SOCKS_NOT_ALLOWED = 2,        /* Response: connection not allowed */
  MG_SOCKS_NET_UNREACHABLE = 3,    /* Response: network unreachable */
  MG_SOCKS_HOST_UNREACHABLE = 4,   /* Response: network unreachable */
  MG_SOCKS_CONN_REFUSED = 5,       /* Response: network unreachable */
  MG_SOCKS_TTL_EXPIRED = 6,        /* Response: network unreachable */
  MG_SOCKS_CMD_NOT_SUPPORTED = 7,  /* Response: network unreachable */
  MG_SOCKS_ADDR_NOT_SUPPORTED = 8, /* Response: network unreachable */
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Turn the connection into the SOCKS server */
void mg_set_protocol_socks(struct mg_connection *c);

/* Create socks tunnel for the client connection */
struct mg_iface *mg_socks_mk_iface(struct mg_mgr *, const char *proxy_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
#endif
