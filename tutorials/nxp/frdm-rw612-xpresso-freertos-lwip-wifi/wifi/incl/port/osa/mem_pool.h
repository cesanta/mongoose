/*
 *  Copyright 2023-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file mem_pool.h
 *\brief This file provides OSA memory pool interfaces for Wi-Fi driver static memory operation.
 */

#ifndef MEM_POOL_H_
#define MEM_POOL_H_

#include <osa.h>

/**
 * @brief Amount of memory reserved for overhead
 */
#define POOL_OVERHEAD (sizeof(uint32_t))

#if defined(SDK_OS_FREE_RTOS)

#include "stack_simple.h"

/**
 *  The actual Memory Pool data structure.
 *
 *  This is a variable length data structure.
 */
typedef struct MemPool_t_
{
    /**
     *  We need a lock to make this thread safe.
     */
    OSA_MUTEX_HANDLE_DEFINE(Lock);

    /**
     *  Memory blocks are stored on a stack.
     */
    Stack_t Stack;

    /**
     *  Save the item size for additions.
     */
    int ItemSize;

    /**
     *  The overall alignment of an item.
     */
    int Alignment;

    /**
     *  The begining of the actual memory pool itself.
     */
    unsigned char Buffer[1];

} MemPool_t;

#elif defined(FSL_RTOS_THREADX)

typedef TX_BLOCK_POOL MemPool_t;

#endif

/** Create a MemoryPool
 *\param[in,out] MemPool the created memory pool.
 *\param[in] ItemSize How big is an allocation.
 *\param[in] PreallocatedMemory Pointer to the preallocated memory
 *           you are dedicating to this pool.
 *\param[in] PreallocatedMemorySize How big is the buffer you are
 *           passing in.
 *\param[in] Alignment Power of 2 value denoting on which address boundary the
 *           memory will be aligned to. Must be at least sizeof(unsigned char *).
 *\return A Handle to the pool, or NULL on failure.
 */
MemoryPool_t OSA_MemoryPoolCreate(
    MemPool_t *MemPool, int ItemSize, void *PreallocatedMemory, int PreallocatedMemorySize, int Alignment);

/**Get a memory buffer from the pool.
 * Note that this can block, and cannnot be used from ISR context.
 *
 *\param[in] pool A handle to a MemoryPool.
 *\return A pointer or NULL on failure.
 */
void *OSA_MemoryPoolAllocate(MemoryPool_t pool);

/**free a memory buffer to the pool.
 *
 *  note This can block, and cannnot be used from ISR context.
 *  note There is no check that the memory passed in is valid.
 *
 *\param[in] pool A handle to a MemoryPool.
 *\param[in] memory memory obtained from OSA_MemoryPoolAllocate().
 */
void OSA_MemoryPoolFree(MemoryPool_t pool, void *memory);

#endif
