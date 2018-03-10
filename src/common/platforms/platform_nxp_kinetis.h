/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_PLATFORMS_PLATFORM_NXP_KINETIS_H_
#define CS_COMMON_PLATFORMS_PLATFORM_NXP_KINETIS_H_

#if CS_PLATFORM == CS_P_NXP_KINETIS

#include <ctype.h>
#include <inttypes.h>
#include <string.h>
#include <sys/time.h>

#define SIZE_T_FMT "u"
typedef struct stat cs_stat_t;
#define to64(x) strtoll(x, NULL, 10)
#define INT64_FMT "lld"
#define INT64_X_FMT "llx"
#define __cdecl

#define MG_LWIP 1

#define MG_NET_IF MG_NET_IF_LWIP_LOW_LEVEL

/* struct timeval is defined in sys/time.h. */
#define LWIP_TIMEVAL_PRIVATE 0

#endif /* CS_PLATFORM == CS_P_NXP_KINETIS */
#endif /* CS_COMMON_PLATFORMS_PLATFORM_NXP_KINETIS_H_ */
