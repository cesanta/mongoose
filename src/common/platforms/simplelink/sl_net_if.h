/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_SIMPLELINK_SL_NET_IF_H_
#define CS_COMMON_PLATFORMS_SIMPLELINK_SL_NET_IF_H_

#include "mongoose/src/net_if.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_ENABLE_NET_IF_SIMPLELINK
#define MG_ENABLE_NET_IF_SIMPLELINK MG_NET_IF == MG_NET_IF_SIMPLELINK
#endif

extern const struct mg_iface_vtable mg_simplelink_iface_vtable;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_PLATFORMS_SIMPLELINK_SL_NET_IF_H_ */
