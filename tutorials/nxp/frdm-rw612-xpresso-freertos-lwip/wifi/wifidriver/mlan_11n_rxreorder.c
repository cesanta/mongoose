/** @file mlan_11n_rxreorder.c
 *
 *  @brief  This file provides handling of RxReordering in wlan
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#include <wm_net.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
    Local Variables
********************************************************/

/********************************************************
    Global Variables
********************************************************/
SDK_ALIGN(uint8_t amsdu_inbuf[4096], 32);
/********************************************************
    Local Functions
********************************************************/
/**
 *  @brief This function will dispatch amsdu packet and
 *  		forward it to kernel/upper layer
 *
 *  @param priv    	A pointer to mlan_private
 *  @param pmbuf    A pointer to the received buffer
 *
 *  @return 	   	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_11n_dispatch_amsdu_pkt(mlan_private *priv, pmlan_buffer pmbuf)
{
    RxPD *prx_pd;
    prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);

    ENTER();
    if (prx_pd->rx_pkt_type == PKT_TYPE_AMSDU)
    {
        pmbuf->data_len = prx_pd->rx_pkt_length;
        pmbuf->data_offset += prx_pd->rx_pkt_offset;

        (void)__memcpy(priv->adapter, amsdu_inbuf, pmbuf->pbuf, sizeof(RxPD));
#if defined(SDK_OS_FREE_RTOS)
        net_stack_buffer_copy_partial(pmbuf->lwip_pbuf, amsdu_inbuf + pmbuf->data_offset, prx_pd->rx_pkt_length, 0);
#endif
#if !CONFIG_TX_RX_ZERO_COPY
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(pmbuf->pbuf);
        net_stack_buffer_free(pmbuf->lwip_pbuf);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf->pbuf);
#endif
#endif
        pmbuf->pbuf = amsdu_inbuf;

        (void)wlan_11n_deaggregate_pkt(priv, pmbuf);

#if CONFIG_TX_RX_ZERO_COPY
        /* Free the net stack buffer after deaggregation and delivered to stack */
#if defined(SDK_OS_FREE_RTOS)
        net_stack_buffer_free(pmbuf->lwip_pbuf);
#endif
#else
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(pmbuf);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf);
#endif
#endif
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }
    LEAVE();
    return MLAN_STATUS_FAILURE;
}

/**
 *  @brief This function will process the rx packet and
 *  		forward it to kernel/upper layer
 *
 *  @param priv    	A pointer to mlan_private
 *  @param payload  A pointer to rx packet payload
 *
 *  @return 	   	MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_11n_dispatch_pkt(t_void *priv, t_void *payload, RxReorderTbl *rx_reor_tbl_ptr)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
#ifdef STA_SUPPORT
    pmlan_adapter pmadapter = ((pmlan_private)priv)->adapter;
#endif
    ENTER();
    if (payload == (t_void *)RX_PKT_DROPPED_IN_FW)
    {
        LEAVE();
        return ret;
    }

#if CONFIG_RSN_REPLAY_DETECTION
    if (ISSUPP_RSN_REPLAY_DETECTION(pmadapter->fw_cap_info) && rx_reor_tbl_ptr &&
        wlan_is_rsn_replay_attack((mlan_private *)priv, payload, rx_reor_tbl_ptr))
    {
        LEAVE();
        return ret;
    }
#endif


#if defined(STA_SUPPORT) || UAP_SUPPORT
    if (MLAN_STATUS_SUCCESS == wlan_11n_dispatch_amsdu_pkt((mlan_private *)priv, (pmlan_buffer)payload))
    {
        LEAVE();
        return ret;
    }
    ret = wlan_process_rx_packet(pmadapter, (pmlan_buffer)payload);
#endif
    LEAVE();
    return ret;
}

/**
 *  @brief This function restarts the reordering timeout timer
 *
 *  @param pmadapter    	A pointer to mlan_adapter
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *
 *  @return                 N/A
 */
static void mlan_11n_rxreorder_timer_restart(pmlan_adapter pmadapter, RxReorderTbl *rx_reor_tbl_ptr)
{
    ENTER();
    if (rx_reor_tbl_ptr->timer_context.timer_is_set != MFALSE)
    {
        (void)pmadapter->callbacks.moal_stop_timer(pmadapter->pmoal_handle, rx_reor_tbl_ptr->timer_context.timer);
    }

    (void)pmadapter->callbacks.moal_start_timer(pmadapter->pmoal_handle, rx_reor_tbl_ptr->timer_context.timer, MFALSE,
                                                (rx_reor_tbl_ptr->win_size * MIN_FLUSH_TIMER_MS));

    rx_reor_tbl_ptr->timer_context.timer_is_set = MTRUE;
    LEAVE();
}

/**
 *  @brief This function dispatches all the packets in the buffer.
 *  		There could be holes in the buffer.
 *
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *  @param start_win        Start window
 *
 *  @return 	   	        MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_11n_dispatch_pkt_until_start_win(t_void *priv, RxReorderTbl *rx_reor_tbl_ptr, t_u16 start_win)
{
    t_u16 no_pkt_to_send, i, xchg;
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    void *rx_tmp_ptr     = MNULL;
    mlan_private *pmpriv = (mlan_private *)priv;

    ENTER();

    no_pkt_to_send = (start_win > rx_reor_tbl_ptr->start_win) ?
                         MIN((start_win - rx_reor_tbl_ptr->start_win), rx_reor_tbl_ptr->win_size) :
                         rx_reor_tbl_ptr->win_size;

    for (i = 0; i < no_pkt_to_send; ++i)
    {
        (void)pmpriv->adapter->callbacks.moal_spin_lock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        rx_tmp_ptr = MNULL;
        if (rx_reor_tbl_ptr->rx_reorder_ptr[i] != NULL)
        {
            rx_tmp_ptr                         = rx_reor_tbl_ptr->rx_reorder_ptr[i];
            rx_reor_tbl_ptr->rx_reorder_ptr[i] = MNULL;
        }
        (void)pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        if (rx_tmp_ptr != NULL)
        {
            (void)wlan_11n_dispatch_pkt(priv, rx_tmp_ptr, rx_reor_tbl_ptr);
        }
    }

    (void)pmpriv->adapter->callbacks.moal_spin_lock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
    /*
     * We don't have a circular buffer, hence use rotation to simulate
     * circular buffer
     */
    xchg = rx_reor_tbl_ptr->win_size - no_pkt_to_send;
    for (i = 0; i < xchg; ++i)
    {
        rx_reor_tbl_ptr->rx_reorder_ptr[i]                  = rx_reor_tbl_ptr->rx_reorder_ptr[no_pkt_to_send + i];
        rx_reor_tbl_ptr->rx_reorder_ptr[no_pkt_to_send + i] = MNULL;
    }

    rx_reor_tbl_ptr->start_win = start_win;
    /* clear the bits of reorder bitmap that has been dispatched */
    rx_reor_tbl_ptr->bitmap = rx_reor_tbl_ptr->bitmap >> no_pkt_to_send;
    (void)pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);

    LEAVE();
    return ret;
}

/**
 *  @brief This function free all the packets in one rxreorder node.
 *
 *  @param priv    	        A pointer to mlan_private
 *
 *  @return 	   	        MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_11n_free_rxreorder_pkt(t_void *priv, RxReorderTbl *rx_reor_tbl_ptr)
{
    mlan_private *pmpriv = (mlan_private *)priv;
    void *rx_tmp_ptr     = MNULL;
    int i                = 0;
    mlan_status ret      = MLAN_STATUS_SUCCESS;

    ENTER();

    for (i = 0; i < rx_reor_tbl_ptr->win_size; i++)
    {
        pmpriv->adapter->callbacks.moal_spin_lock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        rx_tmp_ptr = MNULL;
        if (rx_reor_tbl_ptr->rx_reorder_ptr[i] != NULL)
        {
            rx_tmp_ptr                         = rx_reor_tbl_ptr->rx_reorder_ptr[i];
            rx_reor_tbl_ptr->rx_reorder_ptr[i] = MNULL;
        }
        pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        if (rx_tmp_ptr != NULL)
        {
#if defined(SDK_OS_FREE_RTOS)
            net_stack_buffer_free(((pmlan_buffer)rx_tmp_ptr)->lwip_pbuf);
#endif
#if !CONFIG_TX_RX_ZERO_COPY
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(((pmlan_buffer)rx_tmp_ptr)->pbuf);
            OSA_MemoryFree(rx_tmp_ptr);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, ((pmlan_buffer)rx_tmp_ptr)->pbuf);
            OSA_MemoryPoolFree(buf_128_MemoryPool, rx_tmp_ptr);
#endif
#endif
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function will display the rxReorder table
 *
 *  @param pmadapter          A pointer to mlan_adapter structure
 *  @param rx_reor_tbl_ptr    A pointer to structure RxReorderTbl
 *
 *  @return 	   	N/A
 */
static t_void wlan_11n_display_tbl_ptr(pmlan_adapter pmadapter, RxReorderTbl *rx_reor_tbl_ptr)
{
    ENTER();

    DBG_HEXDUMP(MDAT_D, "Reorder ptr", rx_reor_tbl_ptr->rx_reorder_ptr, sizeof(t_void *) * rx_reor_tbl_ptr->win_size);

    LEAVE();
}

/**
 *  @brief This function will dispatch all packets sequentially
 *  		from start_win until a hole is found and adjust the
 *  		start_win appropriately
 *
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *
 *  @return 	   	        MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_11n_scan_and_dispatch(t_void *priv, RxReorderTbl *rx_reor_tbl_ptr)
{
    t_u16 i, j, xchg;
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    void *rx_tmp_ptr     = MNULL;
    mlan_private *pmpriv = (mlan_private *)priv;

    ENTER();

    for (i = 0; i < rx_reor_tbl_ptr->win_size; ++i)
    {
        (void)pmpriv->adapter->callbacks.moal_spin_lock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        if (rx_reor_tbl_ptr->rx_reorder_ptr[i] == MNULL)
        {
            (void)pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
            break;
        }
        rx_tmp_ptr                         = rx_reor_tbl_ptr->rx_reorder_ptr[i];
        rx_reor_tbl_ptr->rx_reorder_ptr[i] = MNULL;
        (void)pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
        (void)wlan_11n_dispatch_pkt(priv, rx_tmp_ptr, rx_reor_tbl_ptr);
    }

    (void)pmpriv->adapter->callbacks.moal_spin_lock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
    /*
     * We don't have a circular buffer, hence use rotation to simulate
     * circular buffer
     */
    if (i > 0U)
    {
        xchg = rx_reor_tbl_ptr->win_size - i;
        for (j = 0; j < xchg; ++j)
        {
            rx_reor_tbl_ptr->rx_reorder_ptr[j]     = rx_reor_tbl_ptr->rx_reorder_ptr[i + j];
            rx_reor_tbl_ptr->rx_reorder_ptr[i + j] = MNULL;
        }
    }
    /* clear the bits of reorder bitmap that has been dispatched */
    rx_reor_tbl_ptr->bitmap = rx_reor_tbl_ptr->bitmap >> i;

    rx_reor_tbl_ptr->start_win = (rx_reor_tbl_ptr->start_win + i) & (MAX_TID_VALUE - 1U);

    (void)pmpriv->adapter->callbacks.moal_spin_unlock(pmpriv->adapter->pmoal_handle, pmpriv->rx_pkt_lock);
    LEAVE();
    return ret;
}

/**
 *  @brief This function delete rxreorder table's entry
 *         	and free the memory
 *
 *  @param priv    	        A pointer to mlan_private
 *  @param rx_reor_tbl_ptr  A pointer to structure RxReorderTbl
 *
 *  @return 	   	        N/A
 */
static t_void wlan_11n_delete_rxreorder_tbl_entry(mlan_private *priv, RxReorderTbl *rx_reor_tbl_ptr)
{
    pmlan_adapter pmadapter = priv->adapter;
    osa_status_t ret = KOSA_StatusSuccess;

    ENTER();

    if (rx_reor_tbl_ptr == MNULL)
    {
        LEAVE();
        return;
    }

    /* Get and unlick the delete node using lock */
    ret = OSA_SemaphoreWait((osa_semaphore_handle_t)priv->rx_reorder_tbl_lock, osaWaitForever_c);
    if (ret != KOSA_StatusSuccess)
    {
        PRINTM(MWARN, "%s: rx_reorder_tbl_lock not ready: %d", __func__, ret);
        return;
    }
    PRINTM(MDAT_D, "Delete rx_reor_tbl_ptr: %p\n", rx_reor_tbl_ptr);
    rx_reor_tbl_ptr = (RxReorderTbl *)(void *)util_dequeue_list(priv->adapter->pmoal_handle, &priv->rx_reorder_tbl_ptr,
                            priv->adapter->callbacks.moal_spin_lock,
                            priv->adapter->callbacks.moal_spin_unlock);
    OSA_SemaphorePost((osa_semaphore_handle_t)priv->rx_reorder_tbl_lock);

    if (rx_reor_tbl_ptr == MNULL)
    {
        LEAVE();
        return;
    }

    if (mlan_adap->in_reset)
        (void)wlan_11n_free_rxreorder_pkt(priv, rx_reor_tbl_ptr);
    else
        (void)wlan_11n_dispatch_pkt_until_start_win(
            priv, rx_reor_tbl_ptr, (rx_reor_tbl_ptr->start_win + rx_reor_tbl_ptr->win_size) & (MAX_TID_VALUE - 1));

    (void)priv->adapter->callbacks.moal_stop_timer(pmadapter->pmoal_handle, rx_reor_tbl_ptr->timer_context.timer);
    (void)priv->adapter->callbacks.moal_free_timer(pmadapter->pmoal_handle, &rx_reor_tbl_ptr->timer_context.timer);

#if !CONFIG_MEM_POOLS
    (void)pmadapter->callbacks.moal_mfree(pmadapter->pmoal_handle, (t_u8 *)rx_reor_tbl_ptr->rx_reorder_ptr);
#else
    OSA_MemoryPoolFree(buf_1024_MemoryPool, rx_reor_tbl_ptr->rx_reorder_ptr);
#endif

#if !CONFIG_MEM_POOLS
    (void)pmadapter->callbacks.moal_mfree(pmadapter->pmoal_handle, (t_u8 *)rx_reor_tbl_ptr);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, rx_reor_tbl_ptr);
#endif

    LEAVE();
}

/**
 *  @brief This function returns the last used sequence number
 *
 *  @param rx_reorder_tbl_ptr   A pointer to structure RxReorderTbl
 *
 *  @return 	   	            Last used sequence number
 */
static t_s16 wlan_11n_find_last_seqnum(RxReorderTbl *rx_reorder_tbl_ptr)
{
    t_s16 i;

    ENTER();
    for (i = (t_s16)(rx_reorder_tbl_ptr->win_size) - 1; i >= 0; --i)
    {
        if (rx_reorder_tbl_ptr->rx_reorder_ptr[i] != NULL)
        {
            LEAVE();
            return i;
        }
    }
    LEAVE();
    return -1;
}

/**
 *  @brief This function flushes all data
 *
 *  @param context      Reorder context pointer
 *
 *  @return 	   	    N/A
 */
static t_void wlan_flush_data(osa_timer_arg_t tmr_handle)
{
    /* Note: Giving tmr_handle as a parameter in callback is a feature
       of FreeRTOS. Hence, we have to change the default mlan code here
       to get the actual context expected by it */
    reorder_tmr_cnxt_t *reorder_cnxt = (reorder_tmr_cnxt_t *)OSA_TimerGetContext(&tmr_handle);
    t_u16 startWin_u                 = 0U;
    t_s16 startWin                   = 0;

    ENTER();
    if (reorder_cnxt == MNULL)
    {
        PRINTM(MWARN, "Flush data failed\n");
        LEAVE();
        return;
    }

    reorder_cnxt->timer_is_set = MFALSE;
    wlan_11n_display_tbl_ptr(reorder_cnxt->priv->adapter, reorder_cnxt->ptr);

    startWin   = wlan_11n_find_last_seqnum(reorder_cnxt->ptr);
    startWin_u = (t_u16)startWin;

    if (startWin >= 0)
    {
        PRINTM(MINFO, "Flush data %d\n", startWin_u);
        (void)wlan_11n_dispatch_pkt_until_start_win(
            reorder_cnxt->priv, reorder_cnxt->ptr,
            ((reorder_cnxt->ptr->start_win + startWin_u + 1U) & (MAX_TID_VALUE - 1)));
    }

    wlan_11n_display_tbl_ptr(reorder_cnxt->priv->adapter, reorder_cnxt->ptr);
    LEAVE();
}

/**
 *  @brief This function will create a entry in rx reordering table for the
 *  		given ta/tid and will initialize it with seq_num, win_size
 *
 *  @param priv     A pointer to mlan_private
 *  @param ta       ta to find in reordering table
 *  @param tid	    tid to find in reordering table
 *  @param win_size win_size for the give ta/tid pair.
 *  @param seq_num  Starting sequence number for current entry.
 *
 *  @return 	    N/A
 */
static t_void wlan_11n_create_rxreorder_tbl(mlan_private *priv, t_u8 *ta, int tid, t_u16 win_size, t_u16 seq_num)
{
    t_u16 i;
    pmlan_adapter pmadapter = priv->adapter;
    RxReorderTbl *rx_reor_tbl_ptr, *new_node;
    /* sta_node *sta_ptr = MNULL; */
    t_u16 last_seq = 0;

    ENTER();


    /*
     * If we get a TID, ta pair which is already present dispatch all the
     * the packets and move the window size until the ssn
     */
    rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, ta);
    if (rx_reor_tbl_ptr != MNULL)
    {
        (void)wlan_11n_dispatch_pkt_until_start_win(priv, rx_reor_tbl_ptr, seq_num);
    }
    else
    {
        PRINTM(MDAT_D,
               "%s: seq_num %d, tid %d, ta %02x:%02x:%02x:%02x:"
               "%02x:%02x, win_size %d\n",
               __FUNCTION__, seq_num, tid, ta[0], ta[1], ta[2], ta[3], ta[4], ta[5], win_size);
#if !CONFIG_MEM_POOLS
        if ((pmadapter->callbacks.moal_malloc(pmadapter->pmoal_handle, sizeof(RxReorderTbl), MLAN_MEM_DEF,
                                              (t_u8 **)(void **)&new_node)) != MLAN_STATUS_SUCCESS)
#else
        new_node = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
        if (new_node == MNULL)
#endif
        {
            PRINTM(MERROR, "Rx reorder memory allocation failed\n");
            LEAVE();
            return;
        }

        util_init_list((pmlan_linked_list)(void *)new_node);
        new_node->tid = tid;
        (void)__memcpy(pmadapter, new_node->ta, ta, MLAN_MAC_ADDR_LENGTH);
        new_node->start_win = seq_num;
        new_node->pkt_count = 0;
        if (queuing_ra_based(priv) == MTRUE)
        {
            if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP)
            {
                TxBAStreamTbl *ptx_tbl = NULL;
                /* txbastream table also is used as connected STAs data base */
                if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ta)))
                {
                    last_seq = ptx_tbl->rx_seq[tid];
                }
            }
        }
        else
        {
            last_seq = priv->rx_seq[tid];
        }
        new_node->last_seq        = last_seq;
        new_node->win_size        = win_size;
        new_node->force_no_drop   = MFALSE;
        new_node->check_start_win = MTRUE;
        new_node->bitmap          = 0;

#if !CONFIG_MEM_POOLS
        if ((pmadapter->callbacks.moal_malloc(pmadapter->pmoal_handle, 4U * win_size, MLAN_MEM_DEF,
                                              (t_u8 **)&new_node->rx_reorder_ptr)) != MLAN_STATUS_SUCCESS)
#else
        new_node->rx_reorder_ptr = OSA_MemoryPoolAllocate(buf_1024_MemoryPool);
        if (new_node->rx_reorder_ptr == MNULL)
#endif
        {
            PRINTM(MERROR,
                   "Rx reorder table memory allocation"
                   "failed\n");
#if !CONFIG_MEM_POOLS
            (void)pmadapter->callbacks.moal_mfree(pmadapter->pmoal_handle, (t_u8 *)new_node);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, new_node);
#endif
            LEAVE();
            return;
        }

        PRINTM(MDAT_D, "Create ReorderPtr: %p start_win=%d last_seq=%d\n", new_node, new_node->start_win, last_seq);
        new_node->timer_context.ptr          = new_node;
        new_node->timer_context.priv         = priv;
        new_node->timer_context.timer_is_set = MFALSE;
        new_node->ba_status                  = BA_STREAM_SETUP_INPROGRESS;

        (void)pmadapter->callbacks.moal_init_timer(pmadapter->pmoal_handle, &new_node->timer_context.timer,
                                                   wlan_flush_data, &new_node->timer_context);
        for (i = 0; i < win_size; ++i)
        {
            new_node->rx_reorder_ptr[i] = MNULL;
        }

#if CONFIG_RSN_REPLAY_DETECTION
        new_node->hi_curr_rx_count32 = 0xffffffff;
        new_node->lo_curr_rx_count16 = 0;
#endif

        util_enqueue_list_tail(pmadapter->pmoal_handle, &priv->rx_reorder_tbl_ptr, (pmlan_linked_list)(void *)new_node,
                               pmadapter->callbacks.moal_spin_lock, pmadapter->callbacks.moal_spin_unlock);
    }

    LEAVE();
}

/********************************************************
    Global Functions
********************************************************/

/**
 *  @brief This function will return the pointer to a entry in rx reordering
 *  		table which matches the give TA/TID pair
 *
 *  @param priv    A pointer to mlan_private
 *  @param ta      ta to find in reordering table
 *  @param tid	   tid to find in reordering table
 *
 *  @return	   A pointer to structure RxReorderTbl
 */
RxReorderTbl *wlan_11n_get_rxreorder_tbl(mlan_private *priv, int tid, t_u8 *ta)
{
    RxReorderTbl *rx_reor_tbl_ptr;

    ENTER();
    rx_reor_tbl_ptr = (RxReorderTbl *)(void *)util_peek_list(priv->adapter->pmoal_handle, &priv->rx_reorder_tbl_ptr,
                                                             priv->adapter->callbacks.moal_spin_lock,
                                                             priv->adapter->callbacks.moal_spin_unlock);
    if (rx_reor_tbl_ptr == MNULL)
    {
        LEAVE();
        return MNULL;
    }

    while (rx_reor_tbl_ptr != (RxReorderTbl *)(void *)&priv->rx_reorder_tbl_ptr)
    {
        if ((!__memcmp(priv->adapter, rx_reor_tbl_ptr->ta, ta, MLAN_MAC_ADDR_LENGTH)) && (rx_reor_tbl_ptr->tid == tid))
        {
            LEAVE();
            return rx_reor_tbl_ptr;
        }

        rx_reor_tbl_ptr = rx_reor_tbl_ptr->pnext;
    }

    LEAVE();
    return MNULL;
}

/**
 *  @brief This function prepares command for adding a block ack
 *  		request.
 *
 *  @param priv        A pointer to mlan_private structure
 *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf   A pointer to data buffer
 *
 *  @return            MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11n_addba_req(mlan_private *priv, HostCmd_DS_COMMAND *cmd, t_void *pdata_buf)
{
    HostCmd_DS_11N_ADDBA_REQ *padd_ba_req = (HostCmd_DS_11N_ADDBA_REQ *)&cmd->params.add_ba_req;
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_ADDBA_REQ);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_ADDBA_REQ) + S_DS_GEN);

    (void)__memcpy(priv->adapter, padd_ba_req, pdata_buf, sizeof(HostCmd_DS_11N_ADDBA_REQ));
    padd_ba_req->block_ack_param_set = wlan_cpu_to_le16(padd_ba_req->block_ack_param_set);
    padd_ba_req->block_ack_tmo       = wlan_cpu_to_le16(padd_ba_req->block_ack_tmo);
    padd_ba_req->ssn                 = wlan_cpu_to_le16(padd_ba_req->ssn);
    padd_ba_req->add_req_result      = 0;

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command for adding a block ack
 *  		response.
 *
 *  @param priv        A pointer to mlan_private structure
 *  @param cmd         A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf   A pointer to data buffer
 *
 *  @return            MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11n_addba_rspgen(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf)
{
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp    = (HostCmd_DS_11N_ADDBA_RSP *)&cmd->params.add_ba_rsp;
    HostCmd_DS_11N_ADDBA_REQ *pevt_addba_req = (HostCmd_DS_11N_ADDBA_REQ *)pdata_buf;
    t_u8 tid                                 = 0U;
    t_u16 win_size                           = 0U;

    ENTER();

    pevt_addba_req->block_ack_param_set = wlan_le16_to_cpu(pevt_addba_req->block_ack_param_set);
    pevt_addba_req->block_ack_tmo       = wlan_le16_to_cpu(pevt_addba_req->block_ack_tmo);
    pevt_addba_req->ssn                 = wlan_le16_to_cpu(pevt_addba_req->ssn);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_ADDBA_RSP);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_ADDBA_RSP) + S_DS_GEN);

    (void)__memcpy(NULL, padd_ba_rsp->peer_mac_addr, pevt_addba_req->peer_mac_addr, MLAN_MAC_ADDR_LENGTH);
    padd_ba_rsp->dialog_token   = pevt_addba_req->dialog_token;
    padd_ba_rsp->block_ack_tmo  = wlan_cpu_to_le16(pevt_addba_req->block_ack_tmo);
    padd_ba_rsp->ssn            = wlan_cpu_to_le16(pevt_addba_req->ssn);
    padd_ba_rsp->add_rsp_result = 0;

    padd_ba_rsp->block_ack_param_set = pevt_addba_req->block_ack_param_set;
    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK) >> BLOCKACKPARAM_TID_POS;
    if ((priv->addba_reject[tid] != ADDBA_RSP_STATUS_ACCEPT)
#ifdef STA_SUPPORT
#endif
    /* wmsdk: we are not using UAP with mlan right now */
    )
    {
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
    }
    else
    {
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_ACCEPT);
    }
    padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_WINSIZE_MASK;
#if CONFIG_AMSDU_IN_AMPDU
    /* To be done: change priv->aggr_prio_tbl[tid].amsdu for specific AMSDU support by CLI cmd */
    if (!priv->add_ba_param.rx_amsdu)
#endif
    {
        /* We do not support AMSDU inside AMPDU, hence reset the bit */
        padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_AMSDU_SUPP_MASK;
    }

    padd_ba_rsp->block_ack_param_set |= (priv->add_ba_param.rx_win_size << BLOCKACKPARAM_WINSIZE_POS);
    win_size = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_WINSIZE_MASK) >> BLOCKACKPARAM_WINSIZE_POS;
    if (win_size == 0U)
    {
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
    }

    padd_ba_rsp->block_ack_param_set = wlan_cpu_to_le16(padd_ba_rsp->block_ack_param_set);

#if CONFIG_STA_AMPDU_RX
    if (!wifi_sta_ampdu_rx_enable_per_tid_is_allowed(tid))
    {
        padd_ba_rsp->status_code    = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
        padd_ba_rsp->add_rsp_result = BA_RESULT_FAILURE;
    }
#else
    padd_ba_rsp->status_code    = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
    padd_ba_rsp->add_rsp_result = BA_RESULT_FAILURE;
#endif

    if (padd_ba_rsp->status_code == wlan_cpu_to_le16(ADDBA_RSP_STATUS_ACCEPT))
    {
        wlan_11n_create_rxreorder_tbl(priv, pevt_addba_req->peer_mac_addr, tid, win_size, pevt_addba_req->ssn);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

mlan_status wlan_cmd_11n_uap_addba_rspgen(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf)
{
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp    = (HostCmd_DS_11N_ADDBA_RSP *)&cmd->params.add_ba_rsp;
    HostCmd_DS_11N_ADDBA_REQ *pevt_addba_req = (HostCmd_DS_11N_ADDBA_REQ *)pdata_buf;
    t_u8 tid                                 = 0;
    int win_size                             = 0;

    ENTER();

    pevt_addba_req->block_ack_param_set = wlan_le16_to_cpu(pevt_addba_req->block_ack_param_set);
    pevt_addba_req->block_ack_tmo       = wlan_le16_to_cpu(pevt_addba_req->block_ack_tmo);
    pevt_addba_req->ssn                 = wlan_le16_to_cpu(pevt_addba_req->ssn);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_ADDBA_RSP);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_ADDBA_RSP) + S_DS_GEN);

    (void)__memcpy(NULL, padd_ba_rsp->peer_mac_addr, pevt_addba_req->peer_mac_addr, MLAN_MAC_ADDR_LENGTH);
    padd_ba_rsp->dialog_token  = pevt_addba_req->dialog_token;
    padd_ba_rsp->block_ack_tmo = wlan_cpu_to_le16(pevt_addba_req->block_ack_tmo);
    padd_ba_rsp->ssn           = wlan_cpu_to_le16(pevt_addba_req->ssn);

    padd_ba_rsp->block_ack_param_set = pevt_addba_req->block_ack_param_set;
    padd_ba_rsp->add_rsp_result      = 0;
    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK) >> BLOCKACKPARAM_TID_POS;
    if (priv->addba_reject[tid])
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
    else
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_ACCEPT);

#if CONFIG_AMSDU_IN_AMPDU
        /* To be done: change priv->aggr_prio_tbl[tid].amsdu for specific AMSDU support by CLI cmd */
#if 0
    if (!priv->add_ba_param.rx_amsdu || (priv->aggr_prio_tbl[tid].amsdu == BA_STREAM_NOT_ALLOWED))
#else
    if (!priv->add_ba_param.rx_amsdu)
#endif
#endif
        /* We do not support AMSDU inside AMPDU, hence reset the bit */
        padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_AMSDU_SUPP_MASK;

#if CONFIG_UAP_AMPDU_RX
    if (!wifi_uap_ampdu_rx_enable_per_tid_is_allowed(tid))
    {
        padd_ba_rsp->status_code    = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
        padd_ba_rsp->add_rsp_result = BA_RESULT_FAILURE;
    }
#else
    padd_ba_rsp->status_code    = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);
    padd_ba_rsp->add_rsp_result = BA_RESULT_FAILURE;
#endif
    padd_ba_rsp->block_ack_param_set &= ~BLOCKACKPARAM_WINSIZE_MASK;
    padd_ba_rsp->block_ack_param_set |= (priv->add_ba_param.rx_win_size << BLOCKACKPARAM_WINSIZE_POS);
    win_size = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_WINSIZE_MASK) >> BLOCKACKPARAM_WINSIZE_POS;
    if (win_size == 0)
        padd_ba_rsp->status_code = wlan_cpu_to_le16(ADDBA_RSP_STATUS_DECLINED);

    padd_ba_rsp->block_ack_param_set = wlan_cpu_to_le16(padd_ba_rsp->block_ack_param_set);

    if (padd_ba_rsp->status_code == wlan_cpu_to_le16(ADDBA_RSP_STATUS_ACCEPT))
        wlan_11n_create_rxreorder_tbl(priv, pevt_addba_req->peer_mac_addr, tid, win_size, pevt_addba_req->ssn);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command for deleting a block ack
 *  		request.
 *
 *  @param priv       A pointer to mlan_private structure
 *  @param cmd        A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf  A pointer to data buffer
 *
 *  @return           MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11n_delba(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf)
{
    HostCmd_DS_11N_DELBA *pdel_ba = (HostCmd_DS_11N_DELBA *)&cmd->params.del_ba;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11N_DELBA);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_DELBA) + S_DS_GEN);

    (void)__memcpy(priv->adapter, pdel_ba, pdata_buf, sizeof(HostCmd_DS_11N_DELBA));
    pdel_ba->del_ba_param_set = wlan_cpu_to_le16(pdel_ba->del_ba_param_set);
    pdel_ba->reason_code      = wlan_cpu_to_le16(pdel_ba->reason_code);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_RSN_REPLAY_DETECTION
/**
 *  @bref This function is to reset PN value when ptk rekey
 *  @param pmpriv                pointer to mlan_private
 *  @param key               pointer to mlan_ds_encrypt_key
 *
 *  @return                     N/A
 */
t_void wlan_reset_pn_value(mlan_private *pmpriv, mlan_ds_encrypt_key *key)
{
    RxReorderTbl *rx_reor_tbl_ptr = MNULL;
    t_u8 tid                      = 0;

    ENTER();

    if (key->key_flags & KEY_FLAG_REMOVE_KEY || key->key_flags & KEY_FLAG_GROUP_KEY)
        goto done;

    for (tid = 0; tid < 7; tid++)
    {
        rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(pmpriv, tid, key->mac_addr);
        if (rx_reor_tbl_ptr)
        {
            rx_reor_tbl_ptr->hi_curr_rx_count32 = 0xffffffff;
            rx_reor_tbl_ptr->lo_curr_rx_count16 = 0;
        }
    }

done:
    LEAVE();
    return;
}

/**
 *  @bref This function is to reset PN value when rekey
 *  @param pmpriv      pointer to mlan_private
 *  @param event_buf  A pointer to event buf
 *
 *  @return                     N/A
 */
void wlan_reset_pn_on_rekey(mlan_private *priv, t_u8 *event_buf)
{
    t_u8 tid                      = 0;
    RxReorderTbl *rx_reor_tbl_ptr = MNULL;
    ENTER();
    for (tid = 0; tid < 7; tid++)
    {
        rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, event_buf);

        if (rx_reor_tbl_ptr)
        {
            rx_reor_tbl_ptr->hi_curr_rx_count32 = 0xffffffff;
            rx_reor_tbl_ptr->lo_curr_rx_count16 = 0;
        }
    }
    LEAVE();
    return;
}

/**
 *  @bref This function check PN numbers to detect replay counter attack
 *  @param pmpriv                pointer to mlan_private
 *  @param payload               pointer to mlan_buffer
 *  @param rx_reor_tbl_ptr       pointer to RxReorderTbl
 *
 *  @return                      MTRUE/MFALSE
 */
t_u8 wlan_is_rsn_replay_attack(mlan_private *pmpriv, t_void *payload, RxReorderTbl *rx_reor_tbl_ptr)
{
    RxPD *prx_pd       = MNULL;
    pmlan_buffer pmbuf = MNULL;

    ENTER();

    pmbuf = (pmlan_buffer)payload;

    prx_pd = (RxPD *)(pmbuf->pbuf + pmbuf->data_offset);

    if (!(prx_pd->flags & RXPD_FLAG_PN_CHECK_SUPPORT))
    {
        LEAVE();
        return MFALSE;
    }

    if ((prx_pd->hi_rx_count32 == rx_reor_tbl_ptr->hi_curr_rx_count32 &&
         prx_pd->lo_rx_count16 <= rx_reor_tbl_ptr->lo_curr_rx_count16) ||
        (rx_reor_tbl_ptr->hi_curr_rx_count32 != 0xffffffff &&
         prx_pd->hi_rx_count32 < rx_reor_tbl_ptr->hi_curr_rx_count32))
    {
        PRINTM(MERROR, "Drop packet because of invalid PN value. Seq_num %d Last PN:0x%x 0x%x,New PN:0x%x 0x%x\n",
               prx_pd->seq_num, rx_reor_tbl_ptr->hi_curr_rx_count32, rx_reor_tbl_ptr->lo_curr_rx_count16,
               prx_pd->hi_rx_count32, prx_pd->lo_rx_count16);
        net_stack_buffer_free(((pmlan_buffer)payload)->lwip_pbuf);
#if !CONFIG_TX_RX_ZERO_COPY
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(((pmlan_buffer)payload)->pbuf);
        OSA_MemoryFree(payload);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, ((pmlan_buffer)payload)->pbuf);
        OSA_MemoryPoolFree(buf_128_MemoryPool, payload);
#endif
#endif
        rx_reor_tbl_ptr->pn_drop_count++;

        LEAVE();
        return MTRUE;
    }

    rx_reor_tbl_ptr->lo_curr_rx_count16 = prx_pd->lo_rx_count16;
    rx_reor_tbl_ptr->hi_curr_rx_count32 = prx_pd->hi_rx_count32;
    PRINTM(MDAT_D, "seq_num %d PN:0x%x 0x%x\n", prx_pd->seq_num, prx_pd->hi_rx_count32, prx_pd->lo_rx_count16);

    LEAVE();
    return MFALSE;
}
#endif

/**
 *  @brief This function will identify if RxReodering is needed for the packet
 *  		and will do the reordering if required before sending it to kernel
 *
 *  @param priv     A pointer to mlan_private
 *  @param seq_num  Seqence number of the current packet
 *  @param tid	    Tid of the current packet
 *  @param ta	    Transmiter address of the current packet
 *  @param pkt_type Packetype for the current packet (to identify if its a BAR)
 *  @param payload  Pointer to the payload
 *
 *  @return 	    MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status mlan_11n_rxreorder_pkt(void *priv, t_u16 seq_num, t_u16 tid, t_u8 *ta, t_u8 pkt_type, void *payload)
{
    RxReorderTbl *rx_reor_tbl_ptr;
    t_u16 start_win, end_win, win_size;
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = ((mlan_private *)priv)->adapter;

    ENTER();
    rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl((mlan_private *)priv, (int)tid, ta);
    if (rx_reor_tbl_ptr == MNULL)
    {
        if (pkt_type != PKT_TYPE_BAR)
        {
            (void)wlan_11n_dispatch_pkt(priv, payload, rx_reor_tbl_ptr);
        }

        LEAVE();
        return ret;
    }
    else
    {
        if ((pkt_type == PKT_TYPE_AMSDU) && (rx_reor_tbl_ptr->amsdu == 0U))
        {
            (void)wlan_11n_dispatch_pkt(priv, payload, rx_reor_tbl_ptr);
            LEAVE();
            return ret;
        }
        if (pkt_type == PKT_TYPE_BAR)
        {
            PRINTM(MDAT_D, "BAR ");
        }
        if (pkt_type == PKT_TYPE_AMSDU)
        {
            PRINTM(MDAT_D, "AMSDU ");
        }

        if (rx_reor_tbl_ptr->check_start_win != MFALSE)
        {
            if (seq_num == rx_reor_tbl_ptr->start_win)
            {
                rx_reor_tbl_ptr->check_start_win = MFALSE;
            }
            else
            {
                rx_reor_tbl_ptr->pkt_count++;
                if (rx_reor_tbl_ptr->pkt_count < (rx_reor_tbl_ptr->win_size / 2U))
                {
                    if (rx_reor_tbl_ptr->last_seq == seq_num)
                    {
                        /** drop duplicate packet */
                        ret = MLAN_STATUS_FAILURE;
                    }
                    else
                    {
                        /** forward the packet to kernel */
                        rx_reor_tbl_ptr->last_seq = seq_num;
                        if (pkt_type != PKT_TYPE_BAR)
                        {
                            (void)wlan_11n_dispatch_pkt(priv, payload, rx_reor_tbl_ptr);
                        }
                    }
                    LEAVE();
                    return ret;
                }

                rx_reor_tbl_ptr->check_start_win = MFALSE;
                if ((seq_num != rx_reor_tbl_ptr->start_win) && (rx_reor_tbl_ptr->last_seq != DEFAULT_SEQ_NUM))
                {
                    end_win = (rx_reor_tbl_ptr->start_win + rx_reor_tbl_ptr->win_size - 1U) & (MAX_TID_VALUE - 1U);
                    if (((end_win > rx_reor_tbl_ptr->start_win) &&
                         (rx_reor_tbl_ptr->last_seq >= rx_reor_tbl_ptr->start_win) &&
                         (rx_reor_tbl_ptr->last_seq < end_win)) ||
                        ((end_win < rx_reor_tbl_ptr->start_win) &&
                         ((rx_reor_tbl_ptr->last_seq >= rx_reor_tbl_ptr->start_win) ||
                          (rx_reor_tbl_ptr->last_seq < end_win))))
                    {
                        PRINTM(MDAT_D, "Update start_win: last_seq=%d, start_win=%d seq_num=%d\n",
                               rx_reor_tbl_ptr->last_seq, rx_reor_tbl_ptr->start_win, seq_num);
                        rx_reor_tbl_ptr->start_win = rx_reor_tbl_ptr->last_seq + 1U;
                    }
                    else if ((seq_num < rx_reor_tbl_ptr->start_win) && (seq_num > rx_reor_tbl_ptr->last_seq))
                    {
                        PRINTM(MDAT_D, "Update start_win: last_seq=%d, start_win=%d seq_num=%d\n",
                               rx_reor_tbl_ptr->last_seq, rx_reor_tbl_ptr->start_win, seq_num);
                        rx_reor_tbl_ptr->start_win = rx_reor_tbl_ptr->last_seq + 1U;
                    }
                    else
                    { /* Do Nothing */
                    }
                }
            }
        }

        start_win = rx_reor_tbl_ptr->start_win;
        win_size  = rx_reor_tbl_ptr->win_size;
        end_win   = ((start_win + win_size) - 1U) & (MAX_TID_VALUE - 1U);

        PRINTM(MDAT_D, "TID %d, TA %02x:%02x:%02x:%02x:%02x:%02x\n", tid, ta[0], ta[1], ta[2], ta[3], ta[4], ta[5]);
        PRINTM(MDAT_D, "1:seq_num %d start_win %d win_size %d end_win %d\n", seq_num, start_win, win_size, end_win);
        /*
         * If seq_num is less then starting win then ignore and drop
         * the packet
         */
        if (rx_reor_tbl_ptr->force_no_drop != MFALSE)
        {
            PRINTM(MDAT_D, "Force no drop packet after HS_ACTIVED\n");
            rx_reor_tbl_ptr->force_no_drop = MFALSE;
        }
        else
        {
            if ((start_win + TWOPOW11) > (MAX_TID_VALUE - 1U))
            { /* Wrap */
                if (seq_num >= ((start_win + (TWOPOW11)) & (MAX_TID_VALUE - 1U)) && (seq_num < start_win))
                {
                    ret = MLAN_STATUS_FAILURE;
                    goto done;
                }
            }
            else if ((seq_num < start_win) || (seq_num > (start_win + (TWOPOW11))))
            {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            else
            { /* Do Nothing */
            }
        }

        /*
         * If this packet is a BAR we adjust seq_num as
         * WinStart = seq_num
         */
        if (pkt_type == PKT_TYPE_BAR)
        {
            seq_num = ((seq_num + win_size) - 1U) & (MAX_TID_VALUE - 1U);
        }

        PRINTM(MDAT_D, "2:seq_num %d start_win %d win_size %d end_win %d\n", seq_num, start_win, win_size, end_win);

        if (((end_win < start_win) && (seq_num < start_win) && (seq_num > end_win)) ||
            ((end_win > start_win) && ((seq_num > end_win) || (seq_num < start_win))))
        {
            t_s16 delta = (t_s16)seq_num - (t_s16)win_size;
            end_win     = seq_num;
            if ((delta + 1) >= 0)
            {
                start_win = (end_win - win_size) + 1U;
            }
            else
            {
                start_win = (MAX_TID_VALUE - (win_size - seq_num)) + 1U;
            }

            if ((ret = wlan_11n_dispatch_pkt_until_start_win(priv, rx_reor_tbl_ptr, start_win)) != MLAN_STATUS_SUCCESS)
            {
                goto done;
            }
        }

        PRINTM(MDAT_D,
               "3:seq_num %d start_win %d win_size %d"
               " end_win %d\n",
               seq_num, start_win, win_size, end_win);
        if (pkt_type != PKT_TYPE_BAR)
        {
            if (seq_num >= start_win)
            {
                if (rx_reor_tbl_ptr->rx_reorder_ptr[seq_num - start_win] != NULL)
                {
                    PRINTM(MDAT_D, "Drop Duplicate Pkt\n");
                    ret = MLAN_STATUS_FAILURE;
                    goto done;
                }
                rx_reor_tbl_ptr->rx_reorder_ptr[seq_num - start_win] = payload;
                MLAN_SET_BIT_U64(rx_reor_tbl_ptr->bitmap, seq_num - start_win);
            }
            else
            { /* Wrap condition */
                if (rx_reor_tbl_ptr->rx_reorder_ptr[(seq_num + (MAX_TID_VALUE)) - start_win] != NULL)
                {
                    PRINTM(MDAT_D, "Drop Duplicate Pkt\n");
                    ret = MLAN_STATUS_FAILURE;
                    goto done;
                }
                rx_reor_tbl_ptr->rx_reorder_ptr[(seq_num + (MAX_TID_VALUE)) - start_win] = payload;
                MLAN_SET_BIT_U64(rx_reor_tbl_ptr->bitmap, (seq_num + (MAX_TID_VALUE)) - start_win);
            }
        }

        wlan_11n_display_tbl_ptr(pmadapter, rx_reor_tbl_ptr);

        /*
         * Dispatch all packets sequentially from start_win until a
         * hole is found and adjust the start_win appropriately
         */
        ret = wlan_11n_scan_and_dispatch(priv, rx_reor_tbl_ptr);

        wlan_11n_display_tbl_ptr(pmadapter, rx_reor_tbl_ptr);
    }

done:
    if ((rx_reor_tbl_ptr->timer_context.timer_is_set != MFALSE) && (rx_reor_tbl_ptr->bitmap == 0U))
    {
        (void)pmadapter->callbacks.moal_stop_timer(pmadapter->pmoal_handle, rx_reor_tbl_ptr->timer_context.timer);
        rx_reor_tbl_ptr->timer_context.timer_is_set = MFALSE;
    }

    if ((rx_reor_tbl_ptr->timer_context.timer_is_set == MFALSE) && (rx_reor_tbl_ptr->bitmap != 0U))
    {
        mlan_11n_rxreorder_timer_restart(pmadapter, rx_reor_tbl_ptr);
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function will update an entry for a given tid/ta pair. tid/ta
 *  		are taken from delba_event body
 *
 *  @param priv    	    A pointer to mlan_private
 *  @param tid		    tid to send delba
 *  @param peer_mac	    MAC address to send delba
 *  @param type 	    TYPE_DELBA_SENT	or TYPE_DELBA_RECEIVE
 *  @param initiator    MTRUE if we are initiator of ADDBA, MFALSE otherwise
 *
 *  @return 	   	    N/A
 */
void mlan_11n_update_bastream_tbl(mlan_private *priv, int tid, t_u8 *peer_mac, t_u8 type, int initiator)
{
    RxReorderTbl *rx_reor_tbl_ptr;
    TxBAStreamTbl *ptxtbl;
    t_u8 cleanup_rx_reorder_tbl;

    ENTER();

    if (type == TYPE_DELBA_RECEIVE)
    {
        cleanup_rx_reorder_tbl = (initiator) ? MTRUE : MFALSE;
    }
    else
    {
        cleanup_rx_reorder_tbl = (initiator) ? MFALSE : MTRUE;
    }

    PRINTM(MEVENT,
           "DELBA: %02x:%02x:%02x:%02x:%02x:%02x tid=%d,"
           "initiator=%d\n",
           peer_mac[0], peer_mac[1], peer_mac[2], peer_mac[3], peer_mac[4], peer_mac[5], tid, initiator);

    if (cleanup_rx_reorder_tbl != 0U)
    {
        rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, peer_mac);
        if (rx_reor_tbl_ptr == MNULL)
        {
            PRINTM(MWARN, "TID, TA not found in table!\n");
            LEAVE();
            return;
        }
        wlan_11n_delete_rxreorder_tbl_entry(priv, rx_reor_tbl_ptr);
    }
    else
    {
        wlan_request_ralist_lock(priv);
        ptxtbl = wlan_11n_get_txbastream_tbl(priv, peer_mac);
        if (ptxtbl == MNULL)
        {
            PRINTM(MWARN, "TID, RA not found in table!\n");
            wlan_release_ralist_lock(priv);
            LEAVE();
            return;
        }

        wlan_11n_update_txbastream_tbl_ampdu_stat(priv, peer_mac, MFALSE, tid);
        wlan_release_ralist_lock(priv);
    }

    LEAVE();
}

/**
 *  @brief This function handles the command response of
 *  		a block ack response
 *
 *  @param priv    A pointer to mlan_private structure
 *  @param resp    A pointer to HostCmd_DS_COMMAND
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_11n_addba_resp(mlan_private *priv, HostCmd_DS_COMMAND *resp)
{
    HostCmd_DS_11N_ADDBA_RSP *padd_ba_rsp = (HostCmd_DS_11N_ADDBA_RSP *)&resp->params.add_ba_rsp;
    int tid;
    RxReorderTbl *rx_reor_tbl_ptr = MNULL;

    ENTER();

    padd_ba_rsp->status_code         = wlan_le16_to_cpu(padd_ba_rsp->status_code);
    padd_ba_rsp->block_ack_param_set = wlan_le16_to_cpu(padd_ba_rsp->block_ack_param_set);
    padd_ba_rsp->block_ack_tmo       = wlan_le16_to_cpu(padd_ba_rsp->block_ack_tmo);
    padd_ba_rsp->ssn                 = wlan_le16_to_cpu(padd_ba_rsp->ssn);

    tid = (padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_TID_MASK) >> BLOCKACKPARAM_TID_POS;
    /* Check if we had rejected the ADDBA, if yes then do not create the stream */
    if (padd_ba_rsp->status_code == BA_RESULT_SUCCESS)
    {
        PRINTM(MCMND, "ADDBA RSP: %02x:%02x:%02x:%02x:%02x:%02x tid=%d ssn=%d win_size=%d,amsdu=%d\n",
               padd_ba_rsp->peer_mac_addr[0], padd_ba_rsp->peer_mac_addr[1], padd_ba_rsp->peer_mac_addr[2],
               padd_ba_rsp->peer_mac_addr[3], padd_ba_rsp->peer_mac_addr[4], padd_ba_rsp->peer_mac_addr[5], tid,
               padd_ba_rsp->ssn,
               ((padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_WINSIZE_MASK) >> BLOCKACKPARAM_WINSIZE_POS),
               padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_AMSDU_SUPP_MASK);

        if ((rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, padd_ba_rsp->peer_mac_addr)) != NULL)
        {
            rx_reor_tbl_ptr->ba_status = BA_STREAM_SETUP_COMPLETE;
            if ((padd_ba_rsp->block_ack_param_set & BLOCKACKPARAM_AMSDU_SUPP_MASK) && priv->add_ba_param.rx_amsdu &&
                (priv->aggr_prio_tbl[tid].amsdu != BA_STREAM_NOT_ALLOWED))
            {
                rx_reor_tbl_ptr->amsdu = MTRUE;
            }
            else
            {
                rx_reor_tbl_ptr->amsdu = MFALSE;
            }
        }
    }
    else
    {
        PRINTM(MERROR, "ADDBA RSP: Failed(%02x:%02x:%02x:%02x:%02x:%02x tid=%d)\n", padd_ba_rsp->peer_mac_addr[0],
               padd_ba_rsp->peer_mac_addr[1], padd_ba_rsp->peer_mac_addr[2], padd_ba_rsp->peer_mac_addr[3],
               padd_ba_rsp->peer_mac_addr[4], padd_ba_rsp->peer_mac_addr[5], tid);
        if ((rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, tid, padd_ba_rsp->peer_mac_addr)) != NULL)
        {
            wlan_11n_delete_rxreorder_tbl_entry(priv, rx_reor_tbl_ptr);
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles ba_stream_timeout event
 *
 *  @param priv    	A pointer to mlan_private
 *  @param event    A pointer to structure HostCmd_DS_11N_BATIMEOUT
 *
 *  @return 	   	N/A
 */
void wlan_11n_ba_stream_timeout(mlan_private *priv, HostCmd_DS_11N_BATIMEOUT *event)
{
    HostCmd_DS_11N_DELBA delba;

    ENTER();

    DBG_HEXDUMP(MCMD_D, "Event:", (t_u8 *)event, 20);

    (void)__memset(priv->adapter, &delba, 0, sizeof(HostCmd_DS_11N_DELBA));
    (void)__memcpy(priv->adapter, delba.peer_mac_addr, event->peer_mac_addr, MLAN_MAC_ADDR_LENGTH);

    delba.del_ba_param_set |= (t_u16)event->tid << DELBA_TID_POS;
    delba.del_ba_param_set |= (t_u16)event->origninator << DELBA_INITIATOR_POS;
    delba.reason_code = REASON_CODE_STA_TIMEOUT;
    (void)wlan_prepare_cmd(priv, HostCmd_CMD_11N_DELBA, 0, 0, MNULL, &delba);

    LEAVE();
    return;
}

/**
 *  @brief This function cleans up reorder tbl
 *
 *  @param priv    	A pointer to mlan_private
 *
 *  @return 	   	N/A
 */
void wlan_11n_cleanup_reorder_tbl(mlan_private *priv)
{
    RxReorderTbl *del_tbl_ptr;

    ENTER();

    while ((del_tbl_ptr = (RxReorderTbl *)(void *)util_peek_list(priv->adapter->pmoal_handle, &priv->rx_reorder_tbl_ptr,
                                                                 priv->adapter->callbacks.moal_spin_lock,
                                                                 priv->adapter->callbacks.moal_spin_unlock)) != MNULL)
    {
        wlan_11n_delete_rxreorder_tbl_entry(priv, del_tbl_ptr);
    }

    util_init_list((pmlan_linked_list)(void *)&priv->rx_reorder_tbl_ptr);

    (void)__memset(priv->adapter, priv->rx_seq, 0xff, sizeof(priv->rx_seq));
    LEAVE();
}

/**
 *  @brief This function cleans up reorder tbl for specific station
 *
 *  @param priv    	A pointer to mlan_private
 *  @param ta      ta to find in reordering table
 *  @return 	   	N/A
 */
void wlan_cleanup_reorder_tbl(mlan_private *priv, t_u8 *ta)
{
    RxReorderTbl *rx_reor_tbl_ptr = MNULL;
    t_u8 i;
    ENTER();
    for (i = 0; i < MAX_NUM_TID; ++i)
    {
        if ((rx_reor_tbl_ptr = wlan_11n_get_rxreorder_tbl(priv, i, ta)) != NULL)
        {
            wlan_11n_delete_rxreorder_tbl_entry(priv, rx_reor_tbl_ptr);
        }
    }
    LEAVE();
    return;
}

/**
 *  @brief This function will set force_no_drop flag in rxreorder_tbl.
 *
 *  @param priv    A pointer to mlan_private
 *  @param flag    MTRUE/MFALSE
 *
 *  @return	 N/A
 */
static void wlan_set_rxreorder_tbl_no_drop_flag(mlan_private *priv, bool flag)
{
    RxReorderTbl *rx_reor_tbl_ptr;

    ENTER();
    rx_reor_tbl_ptr = (RxReorderTbl *)(void *)util_peek_list(priv->adapter->pmoal_handle, &priv->rx_reorder_tbl_ptr,
                                                             priv->adapter->callbacks.moal_spin_lock,
                                                             priv->adapter->callbacks.moal_spin_unlock);
    if (rx_reor_tbl_ptr == MNULL)
    {
        LEAVE();
        return;
    }

    while (rx_reor_tbl_ptr != (RxReorderTbl *)(void *)&priv->rx_reorder_tbl_ptr)
    {
        rx_reor_tbl_ptr->force_no_drop = flag;
        rx_reor_tbl_ptr                = rx_reor_tbl_ptr->pnext;
    }

    LEAVE();
    return;
}

/**
 *  @brief This function update all the rx_reorder_tbl's force_no_drop flag
 *
 *  @param pmadapter   	A pointer to mlan_adapter
 *  @param flag		    MTRUE/MFALSE
 *  @return 	        N/A
 */
void wlan_update_rxreorder_tbl(pmlan_adapter pmadapter, bool flag)
{
    t_u8 i;
    pmlan_private priv = MNULL;
    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i] != NULL)
        {
            priv = pmadapter->priv[i];
            wlan_set_rxreorder_tbl_no_drop_flag(priv, flag);
        }
    }
    return;
}
