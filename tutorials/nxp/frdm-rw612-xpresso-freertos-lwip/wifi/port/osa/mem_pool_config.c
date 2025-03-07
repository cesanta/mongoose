/*
 *  Copyright 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "mem_pool_config.h"

#if CONFIG_MEM_POOLS

#define ALIGNED_START(x)

/******************************** Pool ID 0 ********************************/
// Pool 0 buffer pool sizes and buffer counts
#define POOL_ID0_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID0_BUF0_CNT 1

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID0_BUF0_SZ 1500

// Pool 0 total buffer size calculation
#define POOL_ID0_SZ (POOL_ID0_BUF0_SZ * POOL_ID0_BUF0_CNT)

// Pool 0 block pool pointer overhead calculation
#define POOL_ID0_OVERHEAD ((POOL_ID0_BUF0_CNT)*POOL_OVERHEAD)

// Pool 0 total pool size calculation
#define POOL_ID0_POOL_SZ (POOL_ID0_SZ + POOL_ID0_OVERHEAD)

/******************************** Pool ID 1 ********************************/
// Pool 1 buffer pool sizes and buffer counts
#define POOL_ID1_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID1_BUF0_CNT 2

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID1_BUF0_SZ 5200

// Pool 1 total buffer size calculation
#define POOL_ID1_SZ (POOL_ID1_BUF0_SZ * POOL_ID1_BUF0_CNT)

// Pool 1 block pool pointer overhead calculation
#define POOL_ID1_OVERHEAD ((POOL_ID1_BUF0_CNT)*POOL_OVERHEAD)

// Pool 1 total pool size calculation
#define POOL_ID1_POOL_SZ (POOL_ID1_SZ + POOL_ID1_OVERHEAD)

/******************************** Pool ID 2 ********************************/
// Pool 2 buffer pool sizes and buffer counts
#define POOL_ID2_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID2_BUF0_CNT 128

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID2_BUF0_SZ 32

// Pool 2 total buffer size calculation
#define POOL_ID2_SZ (POOL_ID2_BUF0_SZ * POOL_ID2_BUF0_CNT)

// Pool 2 block pool pointer overhead calculation
#define POOL_ID2_OVERHEAD ((POOL_ID2_BUF0_CNT)*POOL_OVERHEAD)

// Pool 2 total pool size calculation
#define POOL_ID2_POOL_SZ (POOL_ID2_SZ + POOL_ID2_OVERHEAD)

/******************************** Pool ID 3 ********************************/
// Pool 3 buffer pool sizes and buffer counts
#define POOL_ID3_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID3_BUF0_CNT 256

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID3_BUF0_SZ 128

// Pool 3 total buffer size calculation
#define POOL_ID3_SZ (POOL_ID3_BUF0_SZ * POOL_ID3_BUF0_CNT)

// Pool 3 block pool pointer overhead calculation
#define POOL_ID3_OVERHEAD ((POOL_ID3_BUF0_CNT)*POOL_OVERHEAD)

// Pool 3 total pool size calculation
#define POOL_ID3_POOL_SZ (POOL_ID3_SZ + POOL_ID3_OVERHEAD)

/******************************** Pool ID 4 ********************************/
// Pool 4 buffer pool sizes and buffer counts
#define POOL_ID4_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID4_BUF0_CNT 8

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID4_BUF0_SZ 256

// Pool 4 total buffer size calculation
#define POOL_ID4_SZ (POOL_ID4_BUF0_SZ * POOL_ID4_BUF0_CNT)

// Pool 4 block pool pointer overhead calculation
#define POOL_ID4_OVERHEAD ((POOL_ID4_BUF0_CNT)*POOL_OVERHEAD)

// Pool 4 total pool size calculation
#define POOL_ID4_POOL_SZ (POOL_ID4_SZ + POOL_ID4_OVERHEAD)

/******************************** Pool ID 5 ********************************/
// Pool 5 buffer pool sizes and buffer counts
#define POOL_ID5_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID5_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID5_BUF0_SZ 512

// Pool 5 total buffer size calculation
#define POOL_ID5_SZ (POOL_ID5_BUF0_SZ * POOL_ID5_BUF0_CNT)

// Pool 5 block pool pointer overhead calculation
#define POOL_ID5_OVERHEAD ((POOL_ID5_BUF0_CNT)*POOL_OVERHEAD)

// Pool 5 total pool size calculation
#define POOL_ID5_POOL_SZ (POOL_ID5_SZ + POOL_ID5_OVERHEAD)

/******************************** Pool ID 6 ********************************/
// Pool 6 buffer pool sizes and buffer counts
#define POOL_ID6_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID6_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID6_BUF0_SZ 768

// Pool 6 total buffer size calculation
#define POOL_ID6_SZ (POOL_ID6_BUF0_SZ * POOL_ID6_BUF0_CNT)

// Pool 6 block pool pointer overhead calculation
#define POOL_ID6_OVERHEAD ((POOL_ID6_BUF0_CNT)*POOL_OVERHEAD)

// Pool 6 total pool size calculation
#define POOL_ID6_POOL_SZ (POOL_ID6_SZ + POOL_ID6_OVERHEAD)

/******************************** Pool ID 7 ********************************/
// Pool 7 buffer pool sizes and buffer counts
#define POOL_ID7_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID7_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID7_BUF0_SZ 1024

// Pool 7 total buffer size calculation
#define POOL_ID7_SZ (POOL_ID7_BUF0_SZ * POOL_ID7_BUF0_CNT)

// Pool 7 block pool pointer overhead calculation
#define POOL_ID7_OVERHEAD ((POOL_ID7_BUF0_CNT)*POOL_OVERHEAD)

// Pool 7 total pool size calculation
#define POOL_ID7_POOL_SZ (POOL_ID7_SZ + POOL_ID7_OVERHEAD)

/******************************** Pool ID 8 ********************************/
// Pool 8 buffer pool sizes and buffer counts
#define POOL_ID8_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID8_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID8_BUF0_SZ 1280

// Pool 8 total buffer size calculation
#define POOL_ID8_SZ (POOL_ID8_BUF0_SZ * POOL_ID8_BUF0_CNT)

// Pool 8 block pool pointer overhead calculation
#define POOL_ID8_OVERHEAD ((POOL_ID8_BUF0_CNT)*POOL_OVERHEAD)

// Pool 8 total pool size calculation
#define POOL_ID8_POOL_SZ (POOL_ID8_SZ + POOL_ID8_OVERHEAD)

/******************************** Pool ID 9 ********************************/
// Pool 9 buffer pool sizes and buffer counts
#define POOL_ID9_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID9_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID9_BUF0_SZ 1536

// Pool 9 total buffer size calculation
#define POOL_ID9_SZ (POOL_ID9_BUF0_SZ * POOL_ID9_BUF0_CNT)

// Pool 9 block pool pointer overhead calculation
#define POOL_ID9_OVERHEAD ((POOL_ID9_BUF0_CNT)*POOL_OVERHEAD)

// Pool 9 total pool size calculation
#define POOL_ID9_POOL_SZ (POOL_ID9_SZ + POOL_ID9_OVERHEAD)

/******************************** Pool ID 10 ********************************/
// Pool 10 buffer pool sizes and buffer counts
#define POOL_ID10_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID10_BUF0_CNT 1

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID10_BUF0_SZ 1792

// Pool 10 total buffer size calculation
#define POOL_ID10_SZ (POOL_ID10_BUF0_SZ * POOL_ID10_BUF0_CNT)

// Pool 10 block pool pointer overhead calculation
#define POOL_ID10_OVERHEAD ((POOL_ID10_BUF0_CNT)*POOL_OVERHEAD)

// Pool 10 total pool size calculation
#define POOL_ID10_POOL_SZ (POOL_ID10_SZ + POOL_ID10_OVERHEAD)

/******************************** Pool ID 11 ********************************/
// Pool 11 buffer pool sizes and buffer counts
#define POOL_ID11_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID11_BUF0_CNT 4

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID11_BUF0_SZ 2048

// Pool 11 total buffer size calculation
#define POOL_ID11_SZ (POOL_ID11_BUF0_SZ * POOL_ID11_BUF0_CNT)

// Pool 11 block pool pointer overhead calculation
#define POOL_ID11_OVERHEAD ((POOL_ID11_BUF0_CNT)*POOL_OVERHEAD)

// Pool 11 total pool size calculation
#define POOL_ID11_POOL_SZ (POOL_ID11_SZ + POOL_ID11_OVERHEAD)

/******************************** Pool ID 12 ********************************/
// Pool 12 buffer pool sizes and buffer counts
#define POOL_ID12_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID12_BUF0_CNT 1

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID12_BUF0_SZ 2560

// Pool 12 total buffer size calculation
#define POOL_ID12_SZ (POOL_ID12_BUF0_SZ * POOL_ID12_BUF0_CNT)

// Pool 12 block pool pointer overhead calculation
#define POOL_ID12_OVERHEAD ((POOL_ID12_BUF0_CNT)*POOL_OVERHEAD)

// Pool 12 total pool size calculation
#define POOL_ID12_POOL_SZ (POOL_ID12_SZ + POOL_ID12_OVERHEAD)

/******************************** Pool ID 13 ********************************/
// Pool 13 buffer pool sizes and buffer counts
#define POOL_ID13_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID13_BUF0_CNT 1

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID13_BUF0_SZ 3072

// Pool 13 total buffer size calculation
#define POOL_ID13_SZ (POOL_ID13_BUF0_SZ * POOL_ID13_BUF0_CNT)

// Pool 13 block pool pointer overhead calculation
#define POOL_ID13_OVERHEAD ((POOL_ID13_BUF0_CNT)*POOL_OVERHEAD)

// Pool 13 total pool size calculation
#define POOL_ID13_POOL_SZ (POOL_ID13_SZ + POOL_ID13_OVERHEAD)

/******************************** Pool ID 14 ********************************/
// Pool 14 buffer pool sizes and buffer counts
#define POOL_ID14_BLOCK_POOL_CNT 1

// Buffer 0

#define POOL_ID14_BUF0_CNT 1

/* Buffer descriptor size already part of TX size so don't add again */
#define POOL_ID14_BUF0_SZ 4096

// Pool 14 total buffer size calculation
#define POOL_ID14_SZ (POOL_ID14_BUF0_SZ * POOL_ID14_BUF0_CNT)

// Pool 14 block pool pointer overhead calculation
#define POOL_ID14_OVERHEAD ((POOL_ID14_BUF0_CNT)*POOL_OVERHEAD)

// Pool 14 total pool size calculation
#define POOL_ID14_POOL_SZ (POOL_ID14_SZ + POOL_ID14_OVERHEAD)

MemoryPool_t pmAdapterMemoryPool;
MemoryPool_t pmPrivateMemoryPool;
MemoryPool_t buf_32_MemoryPool;
MemoryPool_t buf_128_MemoryPool;
MemoryPool_t buf_256_MemoryPool;
MemoryPool_t buf_512_MemoryPool;
MemoryPool_t buf_768_MemoryPool;
MemoryPool_t buf_1024_MemoryPool;
MemoryPool_t buf_1280_MemoryPool;
MemoryPool_t buf_1536_MemoryPool;
MemoryPool_t buf_1792_MemoryPool;
MemoryPool_t buf_2048_MemoryPool;
MemoryPool_t buf_2560_MemoryPool;
MemoryPool_t buf_3072_MemoryPool;
MemoryPool_t buf_4096_MemoryPool;

static MemPool_t pmAdapterMemPool;
static MemPool_t pmPrivateMemPool;
static MemPool_t buf_32_MemPool;
static MemPool_t buf_128_MemPool;
static MemPool_t buf_256_MemPool;
static MemPool_t buf_512_MemPool;
static MemPool_t buf_768_MemPool;
static MemPool_t buf_1024_MemPool;
static MemPool_t buf_1280_MemPool;
static MemPool_t buf_1536_MemPool;
static MemPool_t buf_1792_MemPool;
static MemPool_t buf_2048_MemPool;
static MemPool_t buf_2560_MemPool;
static MemPool_t buf_3072_MemPool;
static MemPool_t buf_4096_MemPool;

/***************** Preallocated pmAdapter pool memory ************************/
SDK_ALIGN(uint8_t pmAdapterBufferPool[POOL_ID0_POOL_SZ], 32);

/***************** Preallocated pmPrivate pool memory ************************/
SDK_ALIGN(uint8_t pmPrivateBufferPool[POOL_ID1_POOL_SZ], 32);

/***************** Preallocated buffer 32 pool memory ************************/
SDK_ALIGN(uint8_t buf_32_BufferPool[POOL_ID2_POOL_SZ], 32);

SDK_ALIGN(uint8_t buf_128_BufferPool[POOL_ID3_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_256_BufferPool[POOL_ID4_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_512_BufferPool[POOL_ID5_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_768_BufferPool[POOL_ID6_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_1024_BufferPool[POOL_ID7_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_1280_BufferPool[POOL_ID8_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_1536_BufferPool[POOL_ID9_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_1792_BufferPool[POOL_ID10_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_2048_BufferPool[POOL_ID11_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_2560_BufferPool[POOL_ID12_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_3072_BufferPool[POOL_ID13_POOL_SZ], 32);
SDK_ALIGN(uint8_t buf_4096_BufferPool[POOL_ID14_POOL_SZ], 32);

int mem_pool_init()
{
    int ret = -1;

    pmAdapterMemoryPool =
        OSA_MemoryPoolCreate(&pmAdapterMemPool, POOL_ID0_BUF0_SZ, (void *)pmAdapterBufferPool, POOL_ID0_POOL_SZ, 0);
    if (pmAdapterMemoryPool == NULL)
    {
        mpool_e("Failed to init pmAdapter Buffer Pool");
        return ret;
    }

    pmPrivateMemoryPool =
        OSA_MemoryPoolCreate(&pmPrivateMemPool, POOL_ID1_BUF0_SZ, (void *)pmPrivateBufferPool, POOL_ID1_POOL_SZ, 0);
    if (pmPrivateMemoryPool == NULL)
    {
        mpool_e("Failed to init pmAdapter Buffer Pool");
        return ret;
    }

    buf_32_MemoryPool =
        OSA_MemoryPoolCreate(&buf_32_MemPool, POOL_ID2_BUF0_SZ, (void *)buf_32_BufferPool, POOL_ID2_POOL_SZ, 0);
    if (buf_32_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 32 Buffer Pool");
        return ret;
    }

    buf_128_MemoryPool =
        OSA_MemoryPoolCreate(&buf_128_MemPool, POOL_ID3_BUF0_SZ, (void *)buf_128_BufferPool, POOL_ID3_POOL_SZ, 0);
    if (buf_128_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 128 Buffer Pool");
        return ret;
    }

    buf_256_MemoryPool =
        OSA_MemoryPoolCreate(&buf_256_MemPool, POOL_ID4_BUF0_SZ, (void *)buf_256_BufferPool, POOL_ID4_POOL_SZ, 0);
    if (buf_256_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 256 Buffer Pool");
        return ret;
    }

    buf_512_MemoryPool =
        OSA_MemoryPoolCreate(&buf_512_MemPool, POOL_ID5_BUF0_SZ, (void *)buf_512_BufferPool, POOL_ID5_POOL_SZ, 0);
    if (buf_512_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 512 Buffer Pool");
        return ret;
    }

    buf_768_MemoryPool =
        OSA_MemoryPoolCreate(&buf_768_MemPool, POOL_ID6_BUF0_SZ, (void *)buf_768_BufferPool, POOL_ID6_POOL_SZ, 0);
    if (buf_768_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 768 Buffer Pool");
        return ret;
    }

    buf_1024_MemoryPool =
        OSA_MemoryPoolCreate(&buf_1024_MemPool, POOL_ID7_BUF0_SZ, (void *)buf_1024_BufferPool, POOL_ID7_POOL_SZ, 0);
    if (buf_1024_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 1024 Buffer Pool");
        return ret;
    }

    buf_1280_MemoryPool =
        OSA_MemoryPoolCreate(&buf_1280_MemPool, POOL_ID8_BUF0_SZ, (void *)buf_1280_BufferPool, POOL_ID8_POOL_SZ, 0);
    if (buf_1280_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 1280 Buffer Pool");
        return ret;
    }

    buf_1536_MemoryPool =
        OSA_MemoryPoolCreate(&buf_1536_MemPool, POOL_ID9_BUF0_SZ, (void *)buf_1536_BufferPool, POOL_ID9_POOL_SZ, 0);
    if (buf_1536_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 1536 Buffer Pool");
        return ret;
    }

    buf_1792_MemoryPool =
        OSA_MemoryPoolCreate(&buf_1792_MemPool, POOL_ID10_BUF0_SZ, (void *)buf_1792_BufferPool, POOL_ID10_POOL_SZ, 0);
    if (buf_1792_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 1792 Buffer Pool");
        return ret;
    }

    buf_2048_MemoryPool =
        OSA_MemoryPoolCreate(&buf_2048_MemPool, POOL_ID11_BUF0_SZ, (void *)buf_2048_BufferPool, POOL_ID11_POOL_SZ, 0);
    if (buf_2048_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 2048 Buffer Pool");
        return ret;
    }

    buf_2560_MemoryPool =
        OSA_MemoryPoolCreate(&buf_2560_MemPool, POOL_ID12_BUF0_SZ, (void *)buf_2560_BufferPool, POOL_ID12_POOL_SZ, 0);
    if (buf_2560_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 2560 Buffer Pool");
        return ret;
    }

    buf_3072_MemoryPool =
        OSA_MemoryPoolCreate(&buf_3072_MemPool, POOL_ID13_BUF0_SZ, (void *)buf_3072_BufferPool, POOL_ID13_POOL_SZ, 0);
    if (buf_3072_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 3072 Buffer Pool");
        return ret;
    }

    buf_4096_MemoryPool =
        OSA_MemoryPoolCreate(&buf_4096_MemPool, POOL_ID14_BUF0_SZ, (void *)buf_4096_BufferPool, POOL_ID14_POOL_SZ, 0);
    if (buf_4096_MemoryPool == NULL)
    {
        mpool_e("Failed to init buf 4096 Buffer Pool");
        return ret;
    }

    return 0;
}

#endif
