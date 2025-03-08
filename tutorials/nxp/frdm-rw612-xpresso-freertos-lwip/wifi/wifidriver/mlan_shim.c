/** @file mlan_shim.c
 *
 *  @brief  This file provides  APIs to MOAL module
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/**
 *  @mainpage MLAN Driver
 *
 *  @section overview_sec Overview
 *
 *  The MLAN is an OS independent WLAN driver for NXP 802.11
 *  embedded chipset.
 */

/********************************************************
Change log:
    10/13/2008: initial version
********************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
                Local Variables
********************************************************/

/********************************************************
        Global Variables
********************************************************/
#ifdef STA_SUPPORT
static mlan_operations mlan_sta_ops = {
    /* cmd handler */
    wlan_ops_sta_prepare_cmd,
    /* rx handler */
    wlan_ops_process_rx_packet,
    /* BSS role: STA */
    MLAN_BSS_ROLE_STA,
};
#endif
#if UAP_SUPPORT
static mlan_operations mlan_uap_ops = {
    /* cmd handler */
    wlan_ops_uap_prepare_cmd,
       /* rx handler */
    wlan_ops_process_rx_packet,
    /* BSS role: uAP */
    MLAN_BSS_ROLE_UAP,
};
#endif

/** mlan function table */
static mlan_operations *mlan_ops[] = {
#ifdef STA_SUPPORT
    &mlan_sta_ops,
#endif
#if UAP_SUPPORT
    &mlan_uap_ops,
#endif
    MNULL,
};
extern bus_operations bus_ops;
/** Global moal_assert callback */
t_void (*assert_callback)(IN t_void *pmoal_handle, IN t_u32 cond) = MNULL;
#ifdef DEBUG_LEVEL1
#ifdef DEBUG_LEVEL2
#define DEFAULT_DEBUG_MASK (0xffffffffU)
#else
#define DEFAULT_DEBUG_MASK (MMSG | MFATAL | MERROR)
#endif

/** Global moal_print callback */
t_void (*print_callback)(IN t_void *pmoal_handle, IN t_u32 level, IN t_s8 *pformat, IN...) = MNULL;

/** Global moal_get_system_time callback */
mlan_status (*get_sys_time_callback)(IN t_void *pmoal_handle, OUT t_u32 *psec, OUT t_u32 *pusec) = MNULL;

/** Global driver debug mit masks */
t_u32 drvdbg = DEFAULT_DEBUG_MASK;
#endif

/********************************************************
        Local Functions
*******************************************************/

/********************************************************
        Global Functions
********************************************************/

/**
 *  @brief This function registers MOAL to MLAN module.
 *
 *  @param pmdevice        A pointer to a mlan_device structure
 *                         allocated in MOAL
 *  @param ppmlan_adapter  A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer as the context
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The registration succeeded.
 *                         MLAN_STATUS_FAILURE
 *                             The registration failed.
 *
 * mlan_status mlan_register (
 *   IN pmlan_device     pmdevice,
 *   OUT t_void          **ppmlan_adapter
 * );
 *
 * Comments
 *   MOAL constructs mlan_device data structure to pass moal_handle and
 *   mlan_callback table to MLAN. MLAN returns mlan_adapter pointer to
 *   the ppmlan_adapter buffer provided by MOAL.
 * Headers:
 *   declared in mlan_decl.h
 * See Also
 *   mlan_unregister
 */
mlan_status mlan_register(IN pmlan_device pmdevice, OUT t_void **ppmlan_adapter)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = MNULL;
    pmlan_callbacks pcb     = MNULL;
    t_u8 i                  = 0;
    t_u32 j                 = 0;

    MASSERT(pmdevice);
    MASSERT(ppmlan_adapter);
    /* MASSERT(pmdevice->callbacks.moal_print); */
#ifdef DEBUG_LEVEL1
    print_callback        = pmdevice->callbacks.moal_print;
    get_sys_time_callback = pmdevice->callbacks.moal_get_system_time;
#endif
    /* assert_callback = pmdevice->callbacks.moal_assert; */

    ENTER();

    MASSERT(pmdevice->callbacks.moal_malloc);
    /* MASSERT(pmdevice->callbacks.moal_memset); */
    /* MASSERT(pmdevice->callbacks.moal_memmove); */

    /* Allocate memory for adapter structure */
#if !CONFIG_MEM_POOLS
    if ((pmdevice->callbacks.moal_malloc(/* pmdevice->pmoal_handle */ NULL, sizeof(mlan_adapter), MLAN_MEM_DEF,
                                         (t_u8 **)(void **)&pmadapter) != MLAN_STATUS_SUCCESS) ||
        (pmadapter == MNULL))
    {
        ret = MLAN_STATUS_FAILURE;
        goto exit_register;
    }
#else
    pmadapter = OSA_MemoryPoolAllocate(pmAdapterMemoryPool);
    if (pmadapter == MNULL)
    {
        ret = MLAN_STATUS_FAILURE;
        goto exit_register;
    }
#endif

    (void)__memset(pmadapter, pmadapter, 0, sizeof(mlan_adapter));

    pcb = &pmadapter->callbacks;

    /* Save callback functions */
    (void)__memmove(pmadapter->pmoal_handle, pcb, &pmdevice->callbacks, sizeof(mlan_callbacks));

    pmadapter->priv_num = 0;
    for (i = 0; i < MLAN_MAX_BSS_NUM; i++)
    {
        pmadapter->priv[i] = MNULL;
        if (pmdevice->bss_attr[i].active == MTRUE)
        {
            /* For valid bss_attr, allocate memory for private structure */
#if !CONFIG_MEM_POOLS
            if ((pcb->moal_malloc(pmadapter->pmoal_handle, sizeof(mlan_private), MLAN_MEM_DEF,
                                  (t_u8 **)(void **)&pmadapter->priv[i]) != MLAN_STATUS_SUCCESS) ||
                (pmadapter->priv[i] == MNULL))
            {
                ret = MLAN_STATUS_FAILURE;
                goto error;
            }
#else
            pmadapter->priv[i] = OSA_MemoryPoolAllocate(pmPrivateMemoryPool);
            if (pmadapter->priv[i] == MNULL)
            {
                ret = MLAN_STATUS_FAILURE;
                goto error;
            }
#endif
            pmadapter->priv_num++;
            (void)__memset(pmadapter, pmadapter->priv[i], 0, sizeof(mlan_private));

            pmadapter->priv[i]->adapter = pmadapter;

            /* Save bss_type, frame_type & bss_priority */
            pmadapter->priv[i]->bss_type     = pmdevice->bss_attr[i].bss_type;
            pmadapter->priv[i]->frame_type   = (t_u8)pmdevice->bss_attr[i].frame_type;
            pmadapter->priv[i]->bss_priority = (t_u8)pmdevice->bss_attr[i].bss_priority;
            if (pmdevice->bss_attr[i].bss_type == MLAN_BSS_TYPE_STA)
            {
                pmadapter->priv[i]->bss_role = MLAN_BSS_ROLE_STA;
                pmadapter->priv[i]->bss_mode = MLAN_BSS_MODE_INFRA;
            }
            else if (pmdevice->bss_attr[i].bss_type == MLAN_BSS_TYPE_UAP)
            {
                pmadapter->priv[i]->bss_role = MLAN_BSS_ROLE_UAP;
                pmadapter->priv[i]->bss_mode = MLAN_BSS_MODE_AUTO;
            }
            else
            {
                /*Do Nothing*/
            }
            /* Save bss_index and bss_num */
            pmadapter->priv[i]->bss_index = i;
            pmadapter->priv[i]->bss_num   = (t_u8)pmdevice->bss_attr[i].bss_num;

            /* init function table */
            for (j = 0; mlan_ops[j] != MNULL; j++)
            {
                if (mlan_ops[j]->bss_role == GET_BSS_ROLE(pmadapter->priv[i]))
                {
                    (void)__memcpy(pmadapter, &pmadapter->priv[i]->ops, mlan_ops[j], sizeof(mlan_operations));
                }
            }
        }
    }

    (void)__memcpy(pmadapter, &pmadapter->bus_ops, &bus_ops, sizeof(bus_operations));

    /* Initialize lock variables */
    if (wlan_init_lock_list(pmadapter) != MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    /* Allocate memory for member of adapter structure */
    if (wlan_allocate_adapter(pmadapter) != MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_FAILURE;
        goto error;
    }

    /* Return pointer of mlan_adapter to MOAL */
    *ppmlan_adapter = pmadapter;

    goto exit_register;

error:
    PRINTM(MINFO, "Leave mlan_register with error\n");

    for (i = 0; i < MLAN_MAX_BSS_NUM; i++)
    {
        if (pmadapter->priv[i])
#if !CONFIG_MEM_POOLS
            pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->priv[i]);
#else
            OSA_MemoryPoolFree(pmPrivateMemoryPool, pmadapter->priv[i]);
#endif
    }
#if !CONFIG_MEM_POOLS
    (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter);
#else
    OSA_MemoryPoolFree(pmAdapterMemoryPool, pmadapter);
#endif

exit_register:
    LEAVE();
    return ret;
}

/**
 *  @brief This function unregisters MOAL from MLAN module.
 *
 *  @param pmlan_adapter   A pointer to a mlan_device structure
 *                         allocated in MOAL
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The deregistration succeeded.
 */
MLAN_API mlan_status mlan_unregister(IN t_void *pmlan_adapter)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *)pmlan_adapter;
#if !CONFIG_MEM_POOLS
    pmlan_callbacks pcb;
#endif
    t_s32 i = 0;

    MASSERT(pmlan_adapter);

    ENTER();

#if !CONFIG_MEM_POOLS
    pcb = &pmadapter->callbacks;
#endif

    wlan_free_adapter(pmadapter);

    /* Free private structures */
    for (i = 0; i < MIN(pmadapter->priv_num, MLAN_MAX_BSS_NUM); i++)
    {
        if (pmadapter->priv[i] != MNULL)
        {
            wlan_delete_station_list(pmadapter->priv[i]);
#if !CONFIG_MEM_POOLS
            (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter->priv[i]);
#else
            OSA_MemoryPoolFree(pmPrivateMemoryPool, pmadapter->priv[i]);
#endif
            pmadapter->priv[i] = MNULL;
        }
    }

#if !CONFIG_MEM_POOLS
    /* Free mlan_adapter */
    (void)pcb->moal_mfree(pmadapter->pmoal_handle, (t_u8 *)pmadapter);
#else
    OSA_MemoryPoolFree(pmAdapterMemoryPool, pmadapter);
#endif

    LEAVE();
    return ret;
}


/**
 *  @brief This function initializes the firmware
 *
 *  @param pmlan_adapter   A pointer to a t_void pointer to store
 *                         mlan_adapter structure pointer
 *
 *  @return                MLAN_STATUS_SUCCESS
 *                             The firmware initialization succeeded.
 *                         MLAN_STATUS_PENDING
 *                             The firmware initialization is pending.
 *                         MLAN_STATUS_FAILURE
 *                             The firmware initialization failed.
 */
mlan_status mlan_init_fw(IN t_void *pmlan_adapter)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = (mlan_adapter *)pmlan_adapter;

    ENTER();
    MASSERT(pmlan_adapter);

    /* pmadapter->hw_status = WlanHardwareStatusInitializing; */

    /* Initialize firmware, may return PENDING */
    ret = wlan_init_fw(pmadapter);
    PRINTM(MINFO, "wlan_init_fw returned ret=0x%x\n", ret);

    LEAVE();
    return ret;
}

