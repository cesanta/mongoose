/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_PIC32_NET_IF_H_
#define CS_COMMON_PLATFORMS_PIC32_NET_IF_H_

#include "mongoose/src/net_if.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef MG_ENABLE_NET_IF_PIC32
#define MG_ENABLE_NET_IF_PIC32 MG_NET_IF == MG_NET_IF_PIC32
#endif

extern const struct mg_iface_vtable mg_pic32_iface_vtable;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CS_COMMON_PLATFORMS_PIC32_NET_IF_H_ */
