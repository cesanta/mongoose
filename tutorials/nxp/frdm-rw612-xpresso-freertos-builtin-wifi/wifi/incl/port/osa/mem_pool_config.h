/*  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file mem_pool_config.h
 *\brief This file declares interface for memeory pool.
 */

#ifndef _MEM_POOL_CONFIG_H_
#define _MEM_POOL_CONFIG_H_

#include <mem_pool.h>

#include <wmlog.h>
#define mpool_e(...) wmlog_e("mpool", ##__VA_ARGS__)
#define mpool_w(...) wmlog_w("mpool", ##__VA_ARGS__)

#if CONFIG_MEM_POOL_DEBUG
#define mpool_d(...) wmlog("mpool", ##__VA_ARGS__)
#else
#define mpool_d(...)
#endif /* ! CONFIG_MEM_POOL_DEBUG */

/**
 *  Handle for memory pools.
 *
 *  These are fixed allocation size memory areas.
 */
extern MemoryPool_t pmAdapterMemoryPool;
extern MemoryPool_t pmPrivateMemoryPool;
extern MemoryPool_t buf_32_MemoryPool;
extern MemoryPool_t buf_128_MemoryPool;
extern MemoryPool_t buf_256_MemoryPool;
extern MemoryPool_t buf_512_MemoryPool;
extern MemoryPool_t buf_768_MemoryPool;
extern MemoryPool_t buf_1024_MemoryPool;
extern MemoryPool_t buf_1280_MemoryPool;
extern MemoryPool_t buf_1536_MemoryPool;
extern MemoryPool_t buf_1792_MemoryPool;
extern MemoryPool_t buf_2048_MemoryPool;
extern MemoryPool_t buf_2560_MemoryPool;
extern MemoryPool_t buf_3072_MemoryPool;
extern MemoryPool_t buf_4096_MemoryPool;

int mem_pool_init();

#endif // _MEM_POOL_CONFIG_H_
