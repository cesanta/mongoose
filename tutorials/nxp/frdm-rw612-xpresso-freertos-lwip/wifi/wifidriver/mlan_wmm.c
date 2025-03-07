/** @file mlan_wmm.c
 *
 *  @brief  This file provides functions for WMM
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    10/24/2008: initial version
********************************************************/

#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#if CONFIG_TX_RX_ZERO_COPY
#include <wm_net.h>
#endif
/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
    Local Variables
********************************************************/

/** Maximum value FW can accept for driver delay in packet transmission */
#define DRV_PKT_DELAY_TO_FW_MAX 512

/*
 * Upper and Lower threshold for packet queuing in the driver

 *    - When the number of packets queued reaches the upper limit,
 *      the driver will stop the net queue in the app/kernel space.

 *    - When the number of packets drops beneath the lower limit after
 *      having reached the upper limit, the driver will restart the net
 *      queue.
 */

/** Lower threshold for packet queuing in the driver.
 * When the number of packets drops beneath the lower limit after having
 * reached the upper limit, the driver will restart the net queue.
 */
#define WMM_QUEUED_PACKET_LOWER_LIMIT 180

/** Upper threshold for packet queuing in the driver.
 * When the number of packets queued reaches the upper limit, the driver
 * will stop the net queue in the app/kernel space.
 */
#define WMM_QUEUED_PACKET_UPPER_LIMIT 200

/** Offset for TOS field in the IP header */
#define IPTOS_OFFSET 5

/** WMM information IE */
static const t_u8 wmm_info_ie[] = {(t_u8)WMM_IE, 0x07, 0x00, 0x50, 0xf2, 0x02, 0x00, 0x01, 0x00};

/** Type enumeration of WMM AC_QUEUES */
typedef MLAN_PACK_START enum _wmm_ac_e { AC_BE, AC_BK, AC_VI, AC_VO } MLAN_PACK_END wmm_ac_e;


/**
 * This table will be used to store the tid values based on ACs.
 * It is initialized to default values per TID.
 */
static t_u8 tos_to_tid[] = {
    /* TID DSCP_P2 DSCP_P1 DSCP_P0 WMM_AC */
    0x01, /* 0 1 0 AC_BK */
    0x02, /* 0 0 0 AC_BK */
    0x00, /* 0 0 1 AC_BE */
    0x03, /* 0 1 1 AC_BE */
    0x04, /* 1 0 0 AC_VI */
    0x05, /* 1 0 1 AC_VI */
    0x06, /* 1 1 0 AC_VO */
    0x07  /* 1 1 1 AC_VO */
};


/**
 * This table will provide the tid value for given ac. This table does not
 * change and will be used to copy back the default values to tos_to_tid in
 * case of disconnect.
 */
static const t_u8 ac_to_tid[4][2] = {{1, 2}, {0, 3}, {4, 5}, {6, 7}};
raListTbl *wlan_wmm_get_ralist_node(pmlan_private priv, t_u8 tid, t_u8 *ra_addr);
/********************************************************
    Local Functions
********************************************************/
#ifdef DEBUG_LEVEL2
/**
 *  @brief Debug print function to display the priority parameters for a WMM AC
 *
 *  @param pac_param	Pointer to the AC parameters to display
 *
 *  @return		N/A
 */
static void wlan_wmm_ac_debug_print(const IEEEtypes_WmmAcParameters_t *pac_param)
{
    const char *ac_str[] = {"BK", "BE", "VI", "VO"};

    ENTER();

    PRINTM(MINFO,
           "WMM AC_%s: ACI=%d, ACM=%d, Aifsn=%d, "
           "EcwMin=%d, EcwMax=%d, TxopLimit=%d\n",
           ac_str[wmm_aci_to_qidx_map[pac_param->aci_aifsn.aci]], pac_param->aci_aifsn.aci, pac_param->aci_aifsn.acm,
           pac_param->aci_aifsn.aifsn, pac_param->ecw.ecw_min, pac_param->ecw.ecw_max,
           wlan_le16_to_cpu(pac_param->tx_op_limit));

    LEAVE();
}

/** Print the WMM AC for debug purpose */
#define PRINTM_AC(pac_param) wlan_wmm_ac_debug_print(pac_param)
#else
/** Print the WMM AC for debug purpose */
#define PRINTM_AC(pac_param)
#endif


/**
 *  @brief  This function cleans Tx/Rx queues
 *
 *  @param priv		A pointer to mlan_private
 *
 *  @return		N/A
 */
t_void wlan_clean_txrx(pmlan_private priv)
{
    mlan_adapter *pmadapter = priv->adapter;

    ENTER();

#if CONFIG_WMM
    wlan_cleanup_bypass_txq(GET_BSS_ROLE(priv));
#endif

    wlan_11n_cleanup_reorder_tbl(priv);

    (void)pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);
    wlan_11n_deleteall_txbastream_tbl(priv);
#ifdef SDIO_MULTI_PORT_TX_AGGR
    MP_TX_AGGR_BUF_RESET(priv->adapter);
#endif
#if CONFIG_WMM
    wlan_ralist_del_all_enh(priv);
#endif /* CONFIG_WMM */
    (void)__memcpy(pmadapter, tos_to_tid, ac_to_tid, sizeof(tos_to_tid));

#if UAP_SUPPORT
    priv->num_drop_pkts = 0;
#endif
    (void)pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);

    LEAVE();
}

/**
 *  @brief Set the WMM queue priorities to their default values
 *
 *  @param priv     Pointer to the mlan_private driver data struct
 *
 *  @return         N/A
 */
void wlan_wmm_default_queue_priorities(pmlan_private priv)
{
    ENTER();

    /* Default queue priorities: VO->VI->BE->BK */
    priv->wmm.queue_priority[0] = (t_u8)WMM_AC_VO;
    priv->wmm.queue_priority[1] = (t_u8)WMM_AC_VI;
    priv->wmm.queue_priority[2] = (t_u8)WMM_AC_BE;
    priv->wmm.queue_priority[3] = (t_u8)WMM_AC_BK;

    LEAVE();
}


/**
 *  @brief Initialize the WMM parameter.
 *
 *  @param pmadapter  Pointer to the mlan_adapter data structure
 *
 *  @return         N/A
 */
t_void wlan_init_wmm_param(pmlan_adapter pmadapter)
{
    /* Reuse the same structure of WmmAcParameters_t for configuration
     * purpose here. the definition of acm bit is changed to ucm (user
     * configuration mode) FW will take the setting of
     * aifsn,ecw_max,ecw_min, tx_op_limit only when ucm is set to 1.
     * othewise the default setting/behavoir in firmware will be used.
     */
#ifdef RW610
    pmadapter->ac_params[AC_BE].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_BE].aci_aifsn.aci   = AC_BE;
    pmadapter->ac_params[AC_BE].aci_aifsn.aifsn = 5;
    pmadapter->ac_params[AC_BE].ecw.ecw_max     = 6;
    pmadapter->ac_params[AC_BE].ecw.ecw_min     = 4;
    pmadapter->ac_params[AC_BE].tx_op_limit     = 0;

    pmadapter->ac_params[AC_BK].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_BK].aci_aifsn.aci   = AC_BK;
    pmadapter->ac_params[AC_BK].aci_aifsn.aifsn = 9;
    pmadapter->ac_params[AC_BK].ecw.ecw_max     = 10;
    pmadapter->ac_params[AC_BK].ecw.ecw_min     = 4;
    pmadapter->ac_params[AC_BK].tx_op_limit     = 0;

    pmadapter->ac_params[AC_VI].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_VI].aci_aifsn.aci   = AC_VI;
    pmadapter->ac_params[AC_VI].aci_aifsn.aifsn = 3;
    pmadapter->ac_params[AC_VI].ecw.ecw_max     = 4;
    pmadapter->ac_params[AC_VI].ecw.ecw_min     = 3;
    pmadapter->ac_params[AC_VI].tx_op_limit     = 94;

    pmadapter->ac_params[AC_VO].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_VO].aci_aifsn.aci   = AC_VO;
    pmadapter->ac_params[AC_VO].aci_aifsn.aifsn = 3;
    pmadapter->ac_params[AC_VO].ecw.ecw_max     = 3;
    pmadapter->ac_params[AC_VO].ecw.ecw_min     = 2;
    pmadapter->ac_params[AC_VO].tx_op_limit     = 47;
#else
    pmadapter->ac_params[AC_BE].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_BE].aci_aifsn.aci   = AC_BE;
    pmadapter->ac_params[AC_BE].aci_aifsn.aifsn = 3;
    pmadapter->ac_params[AC_BE].ecw.ecw_max     = 10;
    pmadapter->ac_params[AC_BE].ecw.ecw_min     = 4;
    pmadapter->ac_params[AC_BE].tx_op_limit     = 0;

    pmadapter->ac_params[AC_BK].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_BK].aci_aifsn.aci   = AC_BK;
    pmadapter->ac_params[AC_BK].aci_aifsn.aifsn = 7;
    pmadapter->ac_params[AC_BK].ecw.ecw_max     = 10;
    pmadapter->ac_params[AC_BK].ecw.ecw_min     = 4;
    pmadapter->ac_params[AC_BK].tx_op_limit     = 0;

    pmadapter->ac_params[AC_VI].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_VI].aci_aifsn.aci   = AC_VI;
    pmadapter->ac_params[AC_VI].aci_aifsn.aifsn = 2;
    pmadapter->ac_params[AC_VI].ecw.ecw_max     = 4;
    pmadapter->ac_params[AC_VI].ecw.ecw_min     = 3;
    pmadapter->ac_params[AC_VI].tx_op_limit     = 188;

    pmadapter->ac_params[AC_VO].aci_aifsn.acm   = 0;
    pmadapter->ac_params[AC_VO].aci_aifsn.aci   = AC_VO;
    pmadapter->ac_params[AC_VO].aci_aifsn.aifsn = 2;
    pmadapter->ac_params[AC_VO].ecw.ecw_max     = 3;
    pmadapter->ac_params[AC_VO].ecw.ecw_min     = 2;
    pmadapter->ac_params[AC_VO].tx_op_limit     = 102;
#endif
}

/**
 *  @brief Initialize the WMM state information and the WMM data path queues.
 *
 *  @param pmadapter  Pointer to the mlan_adapter data structure
 *
 *  @return         N/A
 */
t_void wlan_wmm_init(pmlan_adapter pmadapter)
{
    t_u8 i, j;
    pmlan_private priv;

    ENTER();

    for (j = 0; j < pmadapter->priv_num; ++j)
    {
        priv = pmadapter->priv[j];
        if (priv != MNULL)
        {
            for (i = 0; i < MAX_NUM_TID; ++i)
            {
                priv->wmm.pkts_queued[i]              = 0;
                priv->wmm.tid_tbl_ptr[i].ra_list_curr = MNULL;
            }

            priv->add_ba_param.timeout = MLAN_DEFAULT_BLOCK_ACK_TIMEOUT;
#ifdef STA_SUPPORT
            if (priv->bss_type == MLAN_BSS_TYPE_STA)
            {
                priv->add_ba_param.tx_win_size = MLAN_STA_AMPDU_DEF_TXWINSIZE;
                priv->add_ba_param.rx_win_size = MLAN_STA_AMPDU_DEF_RXWINSIZE;
            }
#endif
#if UAP_SUPPORT
            if (priv->bss_type == MLAN_BSS_TYPE_UAP
            )
            {
                priv->add_ba_param.tx_win_size = MLAN_UAP_AMPDU_DEF_TXWINSIZE;
                priv->add_ba_param.rx_win_size = MLAN_UAP_AMPDU_DEF_RXWINSIZE;
            }
#endif
            priv->add_ba_param.tx_amsdu = MTRUE;
            priv->add_ba_param.rx_amsdu = MTRUE;
            (void)__memset(priv->adapter, priv->rx_seq, 0xff, sizeof(priv->rx_seq));
            wlan_wmm_default_queue_priorities(priv);
        }
    }

    LEAVE();
}

/**
 *   @brief Get ralist node
 *
 *   @param priv     Pointer to the mlan_private driver data struct
 *   @param tid      TID
 *   @param ra_addr  Pointer to the route address
 *
 *   @return         ra_list or MNULL
 */
raListTbl *wlan_wmm_get_ralist_node(pmlan_private priv, t_u8 tid, t_u8 *ra_addr)
{
    raListTbl *ra_list;
    ENTER();
    ra_list =
        (raListTbl *)util_peek_list(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[tid].ra_list, MNULL, MNULL);
    while (ra_list && (ra_list != (raListTbl *)&priv->wmm.tid_tbl_ptr[tid].ra_list))
    {
        if (!__memcmp(priv->adapter, ra_list->ra, ra_addr, MLAN_MAC_ADDR_LENGTH))
        {
            LEAVE();
            return ra_list;
        }
        ra_list = ra_list->pnext;
    }
    LEAVE();
    return MNULL;
}


#ifdef STA_SUPPORT

/**
 *  @brief Call back from the command module to allow insertion of a WMM TLV
 *
 *  If the BSS we are associating to supports WMM, add the required WMM
 *    Information IE to the association request command buffer in the form
 *    of a NXP extended IEEE IE.
 *
 *  @param priv        Pointer to the mlan_private driver data struct
 *  @param ppAssocBuf  Output parameter: Pointer to the TLV output buffer,
 *                     modified on return to point after the appended WMM TLV
 *  @param pWmmIE      Pointer to the WMM IE for the BSS we are joining
 *  @param pHTCap      Pointer to the HT IE for the BSS we are joining
 *
 *  @return Length of data appended to the association tlv buffer
 */
t_u32 wlan_wmm_process_association_req(pmlan_private priv,
                                       t_u8 **ppAssocBuf,
                                       IEEEtypes_WmmParameter_t *pWmmIE,
                                       IEEEtypes_HTCap_t *pHTCap)
{
    MrvlIEtypes_WmmParamSet_t *pwmm_tlv;
    t_u32 ret_len = 0;

    ENTER();

    /* Null checks */
    if (ppAssocBuf == MNULL)
    {
        LEAVE();
        return 0;
    }
    if ((*ppAssocBuf) == MNULL)
    {
        LEAVE();
        return 0;
    }

    if (pWmmIE == MNULL)
    {
        LEAVE();
        return 0;
    }

    PRINTM(MINFO, "WMM: process assoc req: bss->wmmIe=0x%x\n", pWmmIE->vend_hdr.element_id);

    if ((priv->wmm_required || ((pHTCap != MNULL) && (pHTCap->ieee_hdr.element_id == HT_CAPABILITY) &&
                                (priv->config_bands & BAND_GN || priv->config_bands & BAND_AN))) &&
        pWmmIE->vend_hdr.element_id == WMM_IE)
    {
        pwmm_tlv              = (MrvlIEtypes_WmmParamSet_t *)(void *)*ppAssocBuf;
        pwmm_tlv->header.type = (t_u16)wmm_info_ie[0];
        pwmm_tlv->header.type = wlan_cpu_to_le16(pwmm_tlv->header.type);
        pwmm_tlv->header.len  = (t_u16)wmm_info_ie[1];
        (void)__memcpy(priv->adapter, pwmm_tlv->wmm_ie, &wmm_info_ie[2], pwmm_tlv->header.len);
        if (pWmmIE->qos_info.qos_uapsd != 0U)
        {
            (void)__memcpy(priv->adapter, (t_u8 *)(pwmm_tlv->wmm_ie + pwmm_tlv->header.len - sizeof(priv->wmm_qosinfo)),
                           &priv->wmm_qosinfo, sizeof(priv->wmm_qosinfo));
        }

        ret_len              = sizeof(pwmm_tlv->header) + pwmm_tlv->header.len;
        pwmm_tlv->header.len = wlan_cpu_to_le16(pwmm_tlv->header.len);

        HEXDUMP("ASSOC_CMD: WMM IE", (t_u8 *)pwmm_tlv, ret_len);
        *ppAssocBuf += ret_len;
    }

    LEAVE();
    return ret_len;
}
#endif /* STA_SUPPORT */

#if CONFIG_WMM
/**
 *  @brief This function prepares the command of WMM_PARAM_CONFIG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   cmd action.
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_wmm_param_config(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u8 cmd_action, t_void *pdata_buf)
{
    wmm_ac_parameters_t *ac_params        = (wmm_ac_parameters_t *)pdata_buf;
    HostCmd_DS_WMM_PARAM_CONFIG *pcmd_cfg = &cmd->params.param_config;
    t_u8 i                                = 0;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_WMM_PARAM_CONFIG);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_WMM_PARAM_CONFIG) + S_DS_GEN);
    cmd->result  = 0;

    pcmd_cfg->action = cmd_action;
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        (void)__memcpy(pmpriv->adapter, pcmd_cfg->ac_params, ac_params, sizeof(wmm_ac_parameters_t) * MAX_AC_QUEUES);
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            pcmd_cfg->ac_params[i].tx_op_limit = wlan_cpu_to_le16(pcmd_cfg->ac_params[i].tx_op_limit);
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of WMM_PARAM_CONFIG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_wmm_param_config(pmlan_private pmpriv, const HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_wmm_cfg *pwmm             = MNULL;
    HostCmd_DS_WMM_PARAM_CONFIG *pcfg = (HostCmd_DS_WMM_PARAM_CONFIG *)&resp->params.param_config;
    t_u8 i;

    ENTER();

    if (pioctl_buf)
    {
        pwmm = (mlan_ds_wmm_cfg *)pioctl_buf->pbuf;
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            pcfg->ac_params[i].tx_op_limit = wlan_le16_to_cpu(pcfg->ac_params[i].tx_op_limit);
        }
        (void)__memcpy(pmpriv->adapter, pwmm->param.ac_params, pcfg->ac_params,
                       sizeof(wmm_ac_parameters_t) * MAX_AC_QUEUES);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_WMM
#if CONFIG_WMM_DEBUG
#define MAX_HISTORY_RA_LIST_NUM 32

static raListTbl *wlan_ralist_get_history(mlan_private *priv, t_u8 *ra, t_u8 ac)
{
    raListTbl *ra_list = (raListTbl *)util_peek_list(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], MNULL, MNULL);

    if (ra_list == MNULL)
        return MNULL;

    while (ra_list && ra_list != (raListTbl *)&priv->wmm.hist_ra[ac])
    {
        if (!memcmp(priv->adapter, ra, ra_list->ra, MLAN_MAC_ADDR_LENGTH))
            return ra_list;

        ra_list = ra_list->pnext;
    }
    return MNULL;
}

/* restore ralist to history list for debug */
static void wlan_ralist_restore_history(mlan_private *priv, raListTbl *ra_list, t_u8 ac)
{
    raListTbl *tmp = MNULL;

    /* history lists has same ra, update stats and insert to tail */
    tmp = wlan_ralist_get_history(priv, ra_list->ra, ac);
    if (tmp != MNULL)
    {
        util_unlink_list(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], (mlan_linked_list *)tmp, MNULL, MNULL);
        ra_list->drop_count += tmp->drop_count;
        util_enqueue_list_tail(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], (mlan_linked_list *)ra_list, MNULL,
                               MNULL);
        goto FREE_TMP;
    }

    /* history ralist count reaches max, dequeue the earliest ralist */
    if (priv->wmm.hist_ra_count[ac] >= MAX_HISTORY_RA_LIST_NUM)
    {
        tmp = (raListTbl *)util_dequeue_list(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], MNULL, MNULL);
        if (tmp == MNULL)
        {
            wifi_e("%s error history ralist count %hhu", priv->wmm.hist_ra_count[ac]);
            goto FREE_IN;
        }

        util_enqueue_list_tail(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], (mlan_linked_list *)ra_list, MNULL,
                               MNULL);
        goto FREE_TMP;
    }
    else
    {
        util_enqueue_list_tail(priv->adapter->pmoal_handle, &priv->wmm.hist_ra[ac], (mlan_linked_list *)ra_list, MNULL,
                               MNULL);
        priv->wmm.hist_ra_count[ac]++;
    }

    return;
FREE_TMP:
    priv->adapter->callbacks.moal_free_semaphore(priv->adapter->pmoal_handle, &tmp->buf_head.plock);
    priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle, (t_u8 *)tmp);
    return;
FREE_IN:
    priv->adapter->callbacks.moal_free_semaphore(priv->adapter->pmoal_handle, &ra_list->buf_head.plock);
    priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle, (t_u8 *)ra_list);
    return;
}
#endif

/*
 *  transfer destination address to receive address
 *  consider 802.3 DA as RA
 *  for broadcast and multicast, consider RA as broadcast mac address
 */
void wifi_wmm_da_to_ra(uint8_t *da, uint8_t *ra)
{
    if ((da[0] & 0x01) == 1)
        (void)__memset(mlan_adap, ra, 0xff, MLAN_MAC_ADDR_LENGTH);
    else
        (void)__memcpy(mlan_adap, ra, da, MLAN_MAC_ADDR_LENGTH);
}

/*
 *  check ra tx_pause status
 *  1. STA mode: check priv->tx_pause
 *  2. UAP mode:
 *      a. broadcast/multicast ra: check in ralists
 *      b. unicast ra: check in ampdu_stat_array for quick access
 */
static uint8_t wifi_wmm_is_tx_pause(const uint8_t interface, mlan_wmm_ac_e queue, uint8_t *ra)
{
    t_u8 is_tx_pause   = MFALSE;
#if UAP_SUPPORT
    raListTbl *ra_list = MNULL;
#endif

    if (interface == MLAN_BSS_TYPE_STA)
    {
        is_tx_pause = mlan_adap->priv[0]->tx_pause;
    }
#if UAP_SUPPORT
    else if (interface == MLAN_BSS_TYPE_UAP)
    {
        if (mlan_adap->priv[1]->tx_pause == MTRUE)
        {
            is_tx_pause = MTRUE;
        }
        else
        {
            mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle,
                                                    &mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list.plock);

            ra_list = wlan_wmm_get_ralist_node(mlan_adap->priv[interface], queue, ra);
            if (ra_list != MNULL)
                is_tx_pause = ra_list->tx_pause;

            mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle,
                                                    &mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list.plock);
        }
    }
#else
    (void)queue;
    (void)ra;
#endif

    return is_tx_pause;
}

/*
 *  find the alternative buffer paused in txqueue and replace it,
 *  priv->tx_pause 1: replace any ra node's oldest packet
 *  tx_pause 1: replace the same ra node's oldest packet
 *  tx_pause 0: replace any other tx_paused ra node's oldest packet
 */
static outbuf_t *wifi_wmm_get_alter_buf_from_txqueue(const uint8_t interface, t_u8 *ra, t_u8 queue, t_u8 tx_pause)
{
    raListTbl *ra_list_head = MNULL;
    raListTbl *ra_list      = MNULL;
    outbuf_t *buf           = MNULL;

    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle,
                                            &mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list.plock);

    if (mlan_adap->priv[interface]->wmm.pkts_queued[queue] == 0)
        goto FAIL;

    ra_list_head = (raListTbl *)&mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list;
    ra_list      = (raListTbl *)util_peek_list(mlan_adap->pmoal_handle, (mlan_list_head *)ra_list_head, MNULL, MNULL);

    while (ra_list && ra_list != ra_list_head)
    {
        mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &ra_list->buf_head.plock);
        if (__memcmp(mlan_adap, ra, ra_list->ra, MLAN_MAC_ADDR_LENGTH) && ra_list->tx_pause == MTRUE &&
            ra_list->total_pkts != 0)
        {
            buf = (outbuf_t *)util_dequeue_list(mlan_adap->pmoal_handle, &ra_list->buf_head, MNULL, MNULL);
            if (buf != MNULL)
                goto SUCC;
        }
        mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &ra_list->buf_head.plock);
        ra_list = ra_list->pnext;
    }

FAIL:
    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle,
                                            &mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list.plock);
    return MNULL;
SUCC:
    mlan_adap->priv[interface]->wmm.pkts_queued[queue]--;
    ra_list->total_pkts--;
    ra_list->drop_count++;
#if CONFIG_TX_RX_ZERO_COPY
    /* Before replacement, need free the buffer from stack first */
    net_stack_buffer_free(buf->buffer);
#endif

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &ra_list->buf_head.plock);

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle,
                                            &mlan_adap->priv[interface]->wmm.tid_tbl_ptr[queue].ra_list.plock);

    wifi_wmm_drop_pause_replaced(interface);
    return buf;
}

/* wmm enhance get free buffer */
uint8_t *wifi_wmm_get_outbuf_enh(
    uint32_t *outbuf_len, mlan_wmm_ac_e queue, const uint8_t interface, uint8_t *ra, bool *is_tx_pause)
{
    t_u8 i;
    outbuf_t *buf = MNULL;
    t_u8 tx_pause;

    buf = wifi_wmm_buf_get();
    if (buf != MNULL)
        goto SUCC;

    /* check tx_pause */
    tx_pause     = wifi_wmm_is_tx_pause(interface, queue, ra);
    *is_tx_pause = (tx_pause == MTRUE) ? true : false;

    /* loop tid_tbl to find buf to replace in wmm ralists */
    if (tx_pause == MFALSE)
    {
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            buf = wifi_wmm_get_alter_buf_from_txqueue(interface, ra, i, tx_pause);
            if (buf != MNULL)
                goto SUCC;
        }
    }

    *outbuf_len = 0;
    return MNULL;
SUCC:
    *outbuf_len = OUTBUF_WMM_LEN;
    return (uint8_t *)buf;
}

/*
 *  get RA list for tx enqueue,
 *  check add broadcast RA list,
 *  drop unknown RA packets in UAP mode
 */
static raListTbl *wlan_wmm_get_queue_raptr_enh(pmlan_private priv, t_u8 ac, t_u8 *ra_addr)
{
    raListTbl *ra_list = MNULL;
    t_u8 bcast_addr[]  = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    ra_list = wlan_wmm_get_ralist_node(priv, ac, ra_addr);
    if (ra_list != MNULL)
        return ra_list;

#if CONFIG_WPA_SUPP
    if ((GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) &&
         (0 != memcmp(ra_addr, bcast_addr, sizeof(bcast_addr))))
    {
        if (MNULL == wlan_get_station_entry(priv, ra_addr))
        {
            PRINTM(MERROR, "Drop packets to unknow station " MACSTR "\n",  MAC2STR(ra_addr));
            LEAVE();
            return MNULL;
        }
    }
#else
    if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP && __memcmp(priv->adapter, ra_addr, bcast_addr, MLAN_MAC_ADDR_LENGTH))
        return MNULL;
#endif

    /* wlan_ralist_add_enh will hold wmm lock, so need to unlock first */
    priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[ac].ra_list.plock);

    wlan_ralist_add_enh(priv, ra_addr);

    priv->adapter->callbacks.moal_semaphore_get(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[ac].ra_list.plock);

    ra_list = wlan_wmm_get_ralist_node(priv, ac, ra_addr);
    return ra_list;
}

/* wmm enhance enqueue tx buffer */
int wlan_wmm_add_buf_txqueue_enh(const uint8_t interface, const uint8_t *buffer, const uint16_t len, uint8_t pkt_prio)
{
    mlan_private *priv            = MNULL;
    t_u8 ra[MLAN_MAC_ADDR_LENGTH] = {0x0};
    raListTbl *ralist             = MNULL;

    CHECK_BSS_TYPE(interface, MLAN_STATUS_FAILURE);
    priv = mlan_adap->priv[interface];

    /* refer to low_level_output payload memcpy */
#if CONFIG_TX_RX_ZERO_COPY
    wifi_wmm_da_to_ra(&((outbuf_t *)buffer)->eth_header[0], ra);
#else
    wifi_wmm_da_to_ra(&((outbuf_t *)buffer)->data[0], ra);
#endif
    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &priv->wmm.tid_tbl_ptr[pkt_prio].ra_list.plock);

    ralist = wlan_wmm_get_queue_raptr_enh(priv, pkt_prio, ra);
    if (ralist == MNULL)
    {
        /* drop for unknown ra when enqueue */
        wifi_wmm_buf_put((outbuf_t *)buffer);
        mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle,
                                                &priv->wmm.tid_tbl_ptr[pkt_prio].ra_list.plock);
        return MLAN_STATUS_FAILURE;
    }

    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &ralist->buf_head.plock);

    util_enqueue_list_tail(mlan_adap->pmoal_handle, &ralist->buf_head, (mlan_linked_list *)buffer, MNULL, MNULL);
    ralist->total_pkts++;
    priv->wmm.pkts_queued[pkt_prio]++;

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &ralist->buf_head.plock);

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &priv->wmm.tid_tbl_ptr[pkt_prio].ra_list.plock);

    return MLAN_STATUS_SUCCESS;
}

/* wmm enhance buffer pool management */
/* return NULL if wmm free list is empty */
outbuf_t *wifi_wmm_buf_get(void)
{
    outbuf_t *buf = MNULL;

    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);

    assert(mlan_adap->outbuf_pool.free_cnt >= 0);

    if (mlan_adap->outbuf_pool.free_cnt == 0)
    {
        mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);
        return MNULL;
    }

    buf = (outbuf_t *)util_dequeue_list(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, MNULL, MNULL);
    assert(buf != MNULL);
    mlan_adap->outbuf_pool.free_cnt--;

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);
    return buf;
}

void wifi_wmm_buf_put(outbuf_t *buf)
{
    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);

    assert(mlan_adap->outbuf_pool.free_cnt < MAX_WMM_BUF_NUM);

#if CONFIG_TX_RX_ZERO_COPY
    /* Free driver's reference count for network buffer */
    net_stack_buffer_free(buf->buffer);
#endif

    util_enqueue_list_tail(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, &buf->entry, MNULL, MNULL);
    mlan_adap->outbuf_pool.free_cnt++;

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);
}

/* init free list, insert all buffers to free list */
int wifi_wmm_buf_pool_init(uint8_t *pool)
{
    int i;
    outbuf_t *buf = MNULL;

    __memset(mlan_adap, &mlan_adap->outbuf_pool, 0x00, sizeof(outbuf_pool_t));

    util_init_list_head(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, MFALSE, MNULL);

    if (mlan_adap->callbacks.moal_init_semaphore(mlan_adap->pmoal_handle, "wmm_buf_pool_sem",
                                                 &mlan_adap->outbuf_pool.free_list.plock) != MLAN_STATUS_SUCCESS)
        return WM_E_NOMEM;

    for (i = 0; i < MAX_WMM_BUF_NUM; i++)
    {
        /* TODO: where to put buffer pool mgmt codes */
        // buf = (outbuf_t *)&outbuf_arr[i][0];
        buf = (outbuf_t *)(pool + (i * OUTBUF_WMM_LEN));
        util_init_list(&buf->entry);
        util_enqueue_list_tail(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, &buf->entry, MNULL, MNULL);
    }
    mlan_adap->outbuf_pool.free_cnt = MAX_WMM_BUF_NUM;

    return WM_SUCCESS;
}

/* deinit free list, should be called after all buffers are put back */
void wifi_wmm_buf_pool_deinit(void)
{
    mlan_adap->outbuf_pool.free_cnt = 0;

    mlan_adap->callbacks.moal_free_semaphore(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list.plock);

    util_free_list_head(mlan_adap->pmoal_handle, &mlan_adap->outbuf_pool.free_list, MNULL);

    __memset(mlan_adap, &mlan_adap->outbuf_pool, 0x00, sizeof(outbuf_pool_t));
}

/* wmm enhance ralist operation */
/* should be called inside wmm tid_tbl_ptr ra_list lock */
void wlan_ralist_pkts_free_enh(mlan_private *priv, raListTbl *ra_list, t_u8 ac)
{
    outbuf_t *buff = MNULL;

    priv->adapter->callbacks.moal_semaphore_get(priv->adapter->pmoal_handle, &ra_list->buf_head.plock);

    while ((buff = (outbuf_t *)util_peek_list(priv->adapter->pmoal_handle, &ra_list->buf_head, MNULL, MNULL)))
    {
        util_unlink_list(priv->adapter->pmoal_handle, &ra_list->buf_head, &buff->entry, MNULL, MNULL);
        wifi_wmm_buf_put(buff);

        priv->wmm.pkts_queued[ac]--;
        wifi_wmm_drop_no_media(priv->bss_index);
        ra_list->drop_count++;
    }
    ra_list->total_pkts = 0;

    priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle, &ra_list->buf_head.plock);
}

/* should be called inside wmm tid_tbl_ptr ra_list lock */
static void wlan_ralist_free_enh(mlan_private *priv, raListTbl *ra_list, t_u8 ac)
{
#if CONFIG_WMM_DEBUG
    wlan_ralist_restore_history(priv, ra_list, ac);
#else
    priv->adapter->callbacks.moal_free_semaphore(priv->adapter->pmoal_handle, &ra_list->buf_head.plock);
#if !CONFIG_MEM_POOLS

    priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle, (t_u8 *)ra_list);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, ra_list);
#endif
#endif
}

static raListTbl *wlan_ralist_alloc_enh(pmlan_adapter pmadapter, t_u8 *ra)
{
    mlan_status ret;
    raListTbl *ra_list = MNULL;

#if !CONFIG_MEM_POOLS
    ret = pmadapter->callbacks.moal_malloc(pmadapter->pmoal_handle, sizeof(raListTbl), MLAN_MEM_DEF, (t_u8 **)&ra_list);
    if (ret != MLAN_STATUS_SUCCESS || ra_list == MNULL)
        return MNULL;
#else
    ra_list = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
    if (ra_list == MNULL)
    {
        return MNULL;
    }

#endif

    util_init_list((pmlan_linked_list)ra_list);
    util_init_list_head((t_void *)pmadapter->pmoal_handle, &ra_list->buf_head, MFALSE, MNULL);

    ret = pmadapter->callbacks.moal_init_semaphore(pmadapter->pmoal_handle, "buf_head_sem", &ra_list->buf_head.plock);
    if (ret != MLAN_STATUS_SUCCESS)
    {
        pmadapter->callbacks.moal_mfree(pmadapter->pmoal_handle, (t_u8 *)ra_list);
        return MNULL;
    }

    (void)__memcpy(pmadapter, ra_list->ra, ra, MLAN_MAC_ADDR_LENGTH);

    ra_list->total_pkts = 0;
    ra_list->tx_pause   = 0;
    ra_list->drop_count = 0;

    wifi_d("RAList: Allocating buffers for TID %p\n", ra_list);

    return ra_list;
}

void wlan_ralist_add_enh(mlan_private *priv, t_u8 *ra)
{
    int i;
    raListTbl *ra_list      = MNULL;
    pmlan_adapter pmadapter = priv->adapter;

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        if ((ra_list = wlan_wmm_get_ralist_node(priv, i, ra)))
            continue;

        ra_list = wlan_ralist_alloc_enh(pmadapter, ra);
        wifi_d("Creating RA List %p for tid %d\n", ra_list, i);
        if (ra_list == MNULL)
        {
            wifi_e("Creating RA List for tid %d fail", i);
            break;
        }

        pmadapter->callbacks.moal_semaphore_get(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list.plock);

        util_enqueue_list_tail(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list, (pmlan_linked_list)ra_list,
                               MNULL, MNULL);

        if (priv->wmm.tid_tbl_ptr[i].ra_list_curr == MNULL)
            priv->wmm.tid_tbl_ptr[i].ra_list_curr = ra_list;

        pmadapter->callbacks.moal_semaphore_put(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
    }
}

int wlan_ralist_update_enh(mlan_private *priv, t_u8 *old_ra, t_u8 *new_ra)
{
    int i;
    int update_count   = 0;
    raListTbl *ra_list = MNULL;
#if CONFIG_WMM_DEBUG
    raListTbl *hist_ra_list = MNULL;
#endif

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        priv->adapter->callbacks.moal_semaphore_get(priv->adapter->pmoal_handle,
                                                    &priv->wmm.tid_tbl_ptr[i].ra_list.plock);

        ra_list = wlan_wmm_get_ralist_node(priv, i, old_ra);
        if (ra_list == MNULL)
        {
            priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle,
                                                        &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
            continue;
        }

        update_count++;

        wlan_ralist_pkts_free_enh(priv, ra_list, i);
        ra_list->tx_pause = MFALSE;

        (void)__memcpy(priv->adapter, ra_list->ra, new_ra, MLAN_MAC_ADDR_LENGTH);

#if CONFIG_WMM_DEBUG
        hist_ra_list = wlan_ralist_alloc_enh(priv->adapter, old_ra);
        if (hist_ra_list != MNULL)
        {
            hist_ra_list->drop_count = ra_list->drop_count;
            wlan_ralist_free_enh(priv, hist_ra_list, i);
        }
#endif

        priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle,
                                                    &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
    }

    return update_count;
}

void wlan_ralist_del_enh(mlan_private *priv, t_u8 *ra)
{
    int i;
    pmlan_adapter pmadapter = priv->adapter;
    raListTbl *ra_list      = MNULL;

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        pmadapter->callbacks.moal_semaphore_get(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list.plock);

        ra_list = wlan_wmm_get_ralist_node(priv, i, ra);
        if (ra_list == MNULL)
        {
            pmadapter->callbacks.moal_semaphore_put(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
            continue;
        }

        wlan_ralist_pkts_free_enh(priv, ra_list, i);

        util_unlink_list(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list, (pmlan_linked_list)ra_list, MNULL,
                         MNULL);

        if (priv->wmm.tid_tbl_ptr[i].ra_list_curr == ra_list)
            priv->wmm.tid_tbl_ptr[i].ra_list_curr = (raListTbl *)&priv->wmm.tid_tbl_ptr[i].ra_list;

        wlan_ralist_free_enh(priv, ra_list, i);

        pmadapter->callbacks.moal_semaphore_put(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
    }
}

void wlan_ralist_del_all_enh(mlan_private *priv)
{
    int i;
    raListTbl *ra_list      = MNULL;
    pmlan_adapter pmadapter = priv->adapter;

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        priv->adapter->callbacks.moal_semaphore_get(priv->adapter->pmoal_handle,
                                                    &priv->wmm.tid_tbl_ptr[i].ra_list.plock);

        while ((ra_list = (raListTbl *)util_peek_list(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list, MNULL,
                                                      MNULL)))
        {
            util_unlink_list(pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list, (pmlan_linked_list)ra_list,
                             MNULL, MNULL);

            wlan_ralist_pkts_free_enh(priv, ra_list, i);

            wlan_ralist_free_enh(priv, ra_list, i);
        }

        /* do not reinit list lock, so use util_init_list instead of util_init_list_head */
        util_init_list((pmlan_linked_list)&priv->wmm.tid_tbl_ptr[i].ra_list);
        priv->wmm.tid_tbl_ptr[i].ra_list_curr = MNULL;
        priv->wmm.pkts_queued[i]              = 0;

        priv->adapter->callbacks.moal_semaphore_put(priv->adapter->pmoal_handle,
                                                    &priv->wmm.tid_tbl_ptr[i].ra_list.plock);
    }
}

void wlan_ralist_deinit_enh(mlan_private *priv)
{
    int i;

    /* already del all ralists in wlan_clean_txrx */
    // wlan_ralist_del_all_enh(priv);

    for (i = 0; i < MAX_AC_QUEUES; i++)
    {
        /* free ralist lock */
        priv->adapter->callbacks.moal_free_semaphore(priv->adapter->pmoal_handle,
                                                     &priv->wmm.tid_tbl_ptr[i].ra_list.plock);

        /* deinit ralists table */
        util_free_list_head(priv->adapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[i].ra_list, MNULL);
    }
}

/* debug statistics */
void wifi_wmm_drop_err_mem(const uint8_t interface)
{
    CHECK_BSS_TYPE_RET_VOID(interface);
    mlan_adap->priv[interface]->driver_error_cnt.tx_err_mem++;
}

void wifi_wmm_drop_no_media(const uint8_t interface)
{
    CHECK_BSS_TYPE_RET_VOID(interface);
    mlan_adap->priv[interface]->driver_error_cnt.tx_no_media++;
}

void wifi_wmm_drop_retried_drop(const uint8_t interface)
{
    CHECK_BSS_TYPE_RET_VOID(interface);
    mlan_adap->priv[interface]->driver_error_cnt.tx_wmm_retried_drop++;
}

void wifi_wmm_drop_pause_drop(const uint8_t interface)
{
    CHECK_BSS_TYPE_RET_VOID(interface);
    mlan_adap->priv[interface]->driver_error_cnt.tx_wmm_pause_drop++;
}

void wifi_wmm_drop_pause_replaced(const uint8_t interface)
{
    CHECK_BSS_TYPE_RET_VOID(interface);
    mlan_adap->priv[interface]->driver_error_cnt.tx_wmm_pause_replaced++;
}

void wlan_get_bypass_lock(uint8_t interface)
{
    pmlan_private priv = mlan_adap->priv[interface];

    mlan_adap->callbacks.moal_semaphore_get(mlan_adap->pmoal_handle, &priv->bypass_txq.plock);
}

void wlan_put_bypass_lock(uint8_t interface)
{
    pmlan_private priv = mlan_adap->priv[interface];

    mlan_adap->callbacks.moal_semaphore_put(mlan_adap->pmoal_handle, &priv->bypass_txq.plock);
}

void wlan_add_buf_bypass_txq(const uint8_t *buffer, const uint8_t interface)
{
    pmlan_private priv = mlan_adap->priv[interface];

    wlan_get_bypass_lock(interface);

    util_enqueue_list_tail(mlan_adap->pmoal_handle, &priv->bypass_txq, (mlan_linked_list *)buffer, MNULL, MNULL);
    priv->bypass_txq_cnt++;

    wlan_put_bypass_lock(interface);
}

t_u8 wlan_bypass_txq_empty(uint8_t interface)
{
    return (mlan_adap->priv[interface]->bypass_txq_cnt) ? MFALSE : MTRUE;
}

int wifi_bypass_txq_init(void)
{
    pmlan_private priv = NULL;
    int i;

    for (i = 0; i < mlan_adap->priv_num; ++i)
    {
        priv = mlan_adap->priv[i];
        if (priv != MNULL)
        {
            /* Initialize bypass_txq */
            util_init_list_head((t_void *)mlan_adap->pmoal_handle, &priv->bypass_txq, MFALSE, MNULL);
            priv->bypass_txq_cnt = 0;

            if (mlan_adap->callbacks.moal_init_semaphore(mlan_adap->pmoal_handle, "bypass_txq_sem",
                                                         &priv->bypass_txq.plock) != MLAN_STATUS_SUCCESS)
                return WM_E_NOMEM;
        }
    }

    return WM_SUCCESS;
}

void wlan_cleanup_bypass_txq(uint8_t interface)
{
    bypass_outbuf_t *buf;
    pmlan_private priv = mlan_adap->priv[interface];

    CHECK_BSS_TYPE_RET_VOID(interface);

    /*Free hold buff*/
    while (!wlan_bypass_txq_empty(interface))
    {
        wlan_get_bypass_lock(interface);

        buf = (bypass_outbuf_t *)util_dequeue_list(mlan_adap->pmoal_handle, &priv->bypass_txq, MNULL, MNULL);
        priv->bypass_txq_cnt--;
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_1536_MemoryPool, buf);
#endif

        wlan_put_bypass_lock(interface);
    }
}

void wifi_bypass_txq_deinit(void)
{
    pmlan_private priv = NULL;
    int i;

    for (i = 0; i < mlan_adap->priv_num; ++i)
    {
        priv = mlan_adap->priv[i];
        if (priv != MNULL)
        {
            wlan_cleanup_bypass_txq(i);

            mlan_adap->callbacks.moal_free_semaphore(mlan_adap->pmoal_handle, &priv->bypass_txq.plock);
            /* Deinit bypass_txq */
            util_free_list_head(mlan_adap->pmoal_handle, &priv->bypass_txq, MNULL);
            priv->bypass_txq_cnt = 0;
        }
    }
}
#endif /* CONFIG_WMM */
