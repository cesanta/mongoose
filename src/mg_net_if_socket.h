/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_SRC_NET_IF_SOCKET_H_
#define CS_MONGOOSE_SRC_NET_IF_SOCKET_H_

#include "mg_net_if.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_ENABLE_NET_IF_SOCKET
#define MG_ENABLE_NET_IF_SOCKET MG_NET_IF == MG_NET_IF_SOCKET
#endif

extern const struct mg_iface_vtable mg_socket_iface_vtable;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_MONGOOSE_SRC_NET_IF_SOCKET_H_ */
