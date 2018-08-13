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

#ifndef CS_COMMON_PLATFORMS_PLATFORM_TM4C129_H_
#define CS_COMMON_PLATFORMS_PLATFORM_TM4C129_H_
#if CS_PLATFORM == CS_P_TM4C129

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef __TI_COMPILER_VERSION__
#include <fcntl.h>
#include <sys/time.h>
#endif

#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define DIRSEP '/'
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT PRId64
#define INT64_X_FMT PRIx64
#define __cdecl

#ifndef MG_NET_IF
#include <lwip/opt.h>
#if LWIP_SOCKET
#define MG_NET_IF MG_NET_IF_SOCKET
#else
#define MG_NET_IF MG_NET_IF_LWIP_LOW_LEVEL
#endif
#define MG_LWIP 1
#elif MG_NET_IF == MG_NET_IF_SIMPLELINK
#include "common/platforms/simplelink/cs_simplelink.h"
#endif

#ifndef CS_ENABLE_STDIO
#define CS_ENABLE_STDIO 1
#endif

#ifdef __TI_COMPILER_VERSION__
/* As of 5.2.8, TI compiler does not support va_copy() yet. */
#define va_copy(apc, ap) ((apc) = (ap))
#endif /* __TI_COMPILER_VERSION__ */

#ifdef __cplusplus
}
#endif

#endif /* CS_PLATFORM == CS_P_TM4C129 */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_TM4C129_H_ */
