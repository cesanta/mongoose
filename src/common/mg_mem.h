/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_COMMON_MG_MEM_H_
#define CS_COMMON_MG_MEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MG_MALLOC
#define MG_MALLOC malloc
#endif

#ifndef MG_CALLOC
#define MG_CALLOC calloc
#endif

#ifndef MG_REALLOC
#define MG_REALLOC realloc
#endif

#ifndef MG_FREE
#define MG_FREE free
#endif

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_MG_MEM_H_ */
