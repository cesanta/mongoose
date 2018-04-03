/*
 * Copyright (c) 2014-2018 Cesanta Software Limited
 * All rights reserved
 *
 * Licensed under the Apache License, Version 2.0 (the ""License"");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an ""AS IS"" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
