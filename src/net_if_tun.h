/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_NET_IF_TUN_H_
#define CS_MONGOOSE_SRC_NET_IF_TUN_H_

#if MG_ENABLE_TUN

#include "mongoose/src/net_if.h"

struct mg_tun_client;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern const struct mg_iface_vtable mg_tun_iface_vtable;

struct mg_connection *mg_tun_if_find_conn(struct mg_tun_client *client,
                                          uint32_t stream_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MG_ENABLE_TUN */

#endif /* CS_MONGOOSE_SRC_NET_IF_TUN_H_ */
