/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"

#define SDK_MEM_MAGIC_NUMBER 12345U

typedef struct _mem_align_control_block
{
    uint16_t identifier; /*!< Identifier for the memory control block. */
    uint16_t offset;     /*!< offset from aligned address to real address */
} mem_align_cb_t;

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.common"
#endif

#if !((defined(__DSC__) && defined(__CW__)))
void *SDK_Malloc(size_t size, size_t alignbytes)
{
    mem_align_cb_t *p_cb = NULL;
    uint32_t alignedsize;

    /* Check overflow. */
    alignedsize = (uint32_t)(unsigned int)SDK_SIZEALIGN(size, alignbytes);
    if (alignedsize < size)
    {
        return NULL;
    }

    if (alignedsize > SIZE_MAX - alignbytes - sizeof(mem_align_cb_t))
    {
        return NULL;
    }

    alignedsize += alignbytes + (uint32_t)sizeof(mem_align_cb_t);

    union
    {
        void *pointer_value;
        uintptr_t unsigned_value;
    } p_align_addr, p_addr;

    p_addr.pointer_value = malloc((size_t)alignedsize);

    if (p_addr.pointer_value == NULL)
    {
        return NULL;
    }

    p_align_addr.unsigned_value = SDK_SIZEALIGN(p_addr.unsigned_value + sizeof(mem_align_cb_t), alignbytes);

    p_cb             = (mem_align_cb_t *)(p_align_addr.unsigned_value - 4U);
    p_cb->identifier = SDK_MEM_MAGIC_NUMBER;
    p_cb->offset     = (uint16_t)(p_align_addr.unsigned_value - p_addr.unsigned_value);

    return p_align_addr.pointer_value;
}

void SDK_Free(void *ptr)
{
    union
    {
        void *pointer_value;
        uintptr_t unsigned_value;
    } p_free;
    p_free.pointer_value = ptr;
    mem_align_cb_t *p_cb = (mem_align_cb_t *)(p_free.unsigned_value - 4U);

    if (p_cb->identifier != SDK_MEM_MAGIC_NUMBER)
    {
        return;
    }

    p_free.unsigned_value = p_free.unsigned_value - p_cb->offset;

    free(p_free.pointer_value);
}
#endif
