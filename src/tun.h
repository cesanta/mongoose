/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_TUN_H_
#define CS_MONGOOSE_SRC_TUN_H_

#if MG_ENABLE_TUN

#include "mongoose/src/net.h"
#include "common/mg_str.h"

#ifndef MG_TUN_RECONNECT_INTERVAL
#define MG_TUN_RECONNECT_INTERVAL 1
#endif

#define MG_TUN_PROTO_NAME "mg_tun"

#define MG_TUN_DATA_FRAME 0x0
#define MG_TUN_F_END_STREAM 0x1

/*
 * MG TUN frame format is loosely based on HTTP/2.
 * However since the communication happens via WebSocket
 * there is no need to encode the frame length, since that's
 * solved by WebSocket framing.
 *
 * TODO(mkm): Detailed description of the protocol.
 */
struct mg_tun_frame {
  uint8_t type;
  uint8_t flags;
  uint32_t stream_id; /* opaque stream identifier */
  struct mg_str body;
};

struct mg_tun_ssl_opts {
#if MG_ENABLE_SSL
  const char *ssl_cert;
  const char *ssl_key;
  const char *ssl_ca_cert;
#else
  int dummy; /* some compilers don't like empty structs */
#endif
};

struct mg_tun_client {
  struct mg_mgr *mgr;
  struct mg_iface *iface;
  const char *disp_url;
  struct mg_tun_ssl_opts ssl;

  uint32_t last_stream_id; /* stream id of most recently accepted connection */

  struct mg_connection *disp;
  struct mg_connection *listener;
  struct mg_connection *reconnect;
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct mg_connection *mg_tun_bind_opt(struct mg_mgr *mgr,
                                      const char *dispatcher,
                                      MG_CB(mg_event_handler_t handler,
                                            void *user_data),
                                      struct mg_bind_opts opts);

int mg_tun_parse_frame(void *data, size_t len, struct mg_tun_frame *frame);

void mg_tun_send_frame(struct mg_connection *ws, uint32_t stream_id,
                       uint8_t type, uint8_t flags, struct mg_str msg);

void mg_tun_destroy_client(struct mg_tun_client *client);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_TUN */

#endif /* CS_MONGOOSE_SRC_TUN_H_ */
