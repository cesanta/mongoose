/*
* Copyright (c) 2014-2017 Cesanta Software Limited
* All rights reserved
*/

#ifndef CS_MONGOOSE_SRC_NET_IF_SOCKS_H_
#define CS_MONGOOSE_SRC_NET_IF_SOCKS_H_

#if MG_ENABLE_SOCKS
#include "mg_net_if.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern const struct mg_iface_vtable mg_socks_iface_vtable;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MG_ENABLE_SOCKS */
#endif /* CS_MONGOOSE_SRC_NET_IF_SOCKS_H_ */
