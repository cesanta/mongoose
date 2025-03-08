/** @file wifi-mem.c
 *
 *  @brief This file provides WIFI dymanic memory allocation APIs.
 *
 *  Copyright 2008-2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <mlan_api.h>
#include <string.h>
#include <osa.h>

#include <wifi-debug.h>

/* Simple memory allocator for Operating Systems that do not support dynamic
 * allocation. The size of the allocation is hard-coded to the need of the cli
 * module.
 *
 * If required this can be blown into a better slab-kind of allocator.
 */
#define HUGE_BUF_SIZE 2000
static char buffhuge[HUGE_BUF_SIZE];

void *wifi_mem_malloc_cmdrespbuf(void)
{
    /* NOTE: There is no corresponding free call for cmdrespbuf */
    /* CMD are all serialised and they dont need any locking */
    return buffhuge;
}

void *wifi_malloc_eventbuf(size_t size)
{
#if !CONFIG_MEM_POOLS
    void *ptr = OSA_MemoryAllocate(size);

    if (ptr != NULL)
    {
        w_mem_d("[evtbuf] Alloc: A: %p S: %d", ptr, size);
    }
    else
    {
        w_mem_e("[evtbuf] Alloc: S: %d FAILED", size);
    }
#else
    void *ptr = OSA_MemoryPoolAllocate(buf_2560_MemoryPool);
#endif

    return ptr;
}

void wifi_free_eventbuf(void *buffer)
{
#if !CONFIG_MEM_POOLS
    w_mem_d("[evtbuf] Free: A: %p\n\r", buffer);
    OSA_MemoryFree(buffer);
#else
    OSA_MemoryPoolFree(buf_2560_MemoryPool, buffer);
#endif
}

mlan_status wrapper_moal_malloc(IN t_void *pmoal_handle, IN t_u32 size, IN t_u32 flag, OUT t_u8 **ppbuf)
{
    *ppbuf = OSA_MemoryAllocate(size);


    if (*ppbuf != NULL)
    {
        w_mem_d("[mlan]: Alloc: A: %p S: %d", *ppbuf, size);
        return MLAN_STATUS_SUCCESS;
    }
    else
    {
        w_mem_e("[mlan] Alloc: S: %d FAILED", size);
        /*
         * fixme: check if MLAN_STATUS_SUCCESS is to be returned in
         * spite of the status failure.
         */
        return MLAN_STATUS_FAILURE;
    }
}

mlan_status wrapper_moal_mfree(IN t_void *pmoal_handle, IN t_u8 *pbuf)
{
    w_mem_d("[mlan] Free: A: %p", pbuf);
    OSA_MemoryFree(pbuf);
    return MLAN_STATUS_SUCCESS;
}
