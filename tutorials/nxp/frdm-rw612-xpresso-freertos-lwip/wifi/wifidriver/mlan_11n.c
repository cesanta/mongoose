/** @file mlan_11n.c
 *
 *  @brief  This file provides functions for 11n handling.
 *
 *  Copyright 2008-2024 NXP
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

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
    Local Variables
********************************************************/

/********************************************************
    Global Variables
********************************************************/

/********************************************************
    Local Functions
********************************************************/

/* Disable the optional features of 11N */
#undef _80211n_OPT_FEATURES



/**
 *  @brief Set/get htcapinfo configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return				MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_11n_ioctl_htusrcfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_11n_cfg *cfg = MNULL;

    ENTER();

    cfg = (mlan_ds_11n_cfg *)(void *)pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_SET)
    {
        if (((cfg->param.htcap_cfg.htcap & ~IGN_HW_DEV_CAP) & pmpriv->adapter->hw_dot_11n_dev_cap) !=
            (cfg->param.htcap_cfg.htcap & ~IGN_HW_DEV_CAP))
        {
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            ret                     = MLAN_STATUS_FAILURE;
        }
        else
        {
            if (cfg->param.htcap_cfg.misc_cfg == BAND_SELECT_BG)
            {
                pmadapter->usr_dot_11n_dev_cap_bg = cfg->param.htcap_cfg.htcap;
                PRINTM(MINFO, "Set: UsrDot11nCap for 2.4GHz 0x%x\n", pmadapter->usr_dot_11n_dev_cap_bg);
            }
            if (cfg->param.htcap_cfg.misc_cfg == BAND_SELECT_A)
            {
                pmadapter->usr_dot_11n_dev_cap_a = cfg->param.htcap_cfg.htcap;
                PRINTM(MINFO, "Set: UsrDot11nCap for 5GHz 0x%x\n", pmadapter->usr_dot_11n_dev_cap_a);
            }
            if (cfg->param.htcap_cfg.misc_cfg == BAND_SELECT_BOTH)
            {
                pmadapter->usr_dot_11n_dev_cap_bg = cfg->param.htcap_cfg.htcap;
                pmadapter->usr_dot_11n_dev_cap_a  = cfg->param.htcap_cfg.htcap;
                PRINTM(MINFO, "Set: UsrDot11nCap for 2.4GHz and 5GHz 0x%x\n", cfg->param.htcap_cfg.htcap);
            }
        }
    }
    else
    {
        /* Hardware 11N device capability required */
        if (cfg->param.htcap_cfg.hw_cap_req != 0U)
        {
            cfg->param.htcap_cfg.htcap = pmadapter->hw_dot_11n_dev_cap;
        }
        else
        {
            if (cfg->param.htcap_cfg.misc_cfg == BAND_SELECT_BG)
            {
                cfg->param.htcap_cfg.htcap = pmadapter->usr_dot_11n_dev_cap_bg;
                PRINTM(MINFO, "Get: UsrDot11nCap for 2.4GHz 0x%x\n", cfg->param.htcap_cfg.htcap);
            }
            if (cfg->param.htcap_cfg.misc_cfg == BAND_SELECT_A)
            {
                cfg->param.htcap_cfg.htcap = pmadapter->usr_dot_11n_dev_cap_a;
                PRINTM(MINFO, "Get: UsrDot11nCap for 5GHz 0x%x\n", cfg->param.htcap_cfg.htcap);
            }
        }
    }

    LEAVE();
    return ret;
}


/**
 *  @brief Set/get 11n configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_11n_ioctl_httxcfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_11n_cfg *cfg = MNULL;
    t_u16 cmd_action     = 0;

    ENTER();

    cfg = (mlan_ds_11n_cfg *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        cmd_action = HostCmd_ACT_GEN_SET;
    }
    else
    {
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_11N_CFG, cmd_action, 0, (t_void *)pioctl_req,
                           (t_void *)&cfg->param.tx_cfg);
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}


/**
 *  @brief This function will send DELBA to entries in the priv's
 *          Tx BA stream table
 *
 *  @param priv                 A pointer to mlan_private
 *  @param pioctl_req          A pointer to ioctl request buffer
 *  @param tid                  TID
 *  @param peer_address         A pointer to peer address
 *  @param last_tx_ba_to_delete A pointer to the last entry in TxBAStreamTbl
 *
 *  @return            MLAN_STATUS_SUCCESS or MLAN_STATUS_PENDING
 */
static mlan_status wlan_send_delba_to_entry_in_txbastream_tbl(
    pmlan_private priv, pmlan_ioctl_req pioctl_req, t_u8 tid, t_u8 *peer_address, TxBAStreamTbl *last_tx_ba_to_delete)
{
    pmlan_adapter pmadapter = priv->adapter;
    TxBAStreamTbl *tx_ba_stream_tbl_ptr;
    t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH] = {0};
    mlan_status ret                     = MLAN_STATUS_SUCCESS;
    int i;

    ENTER();

    wlan_request_ralist_lock(priv);
    tx_ba_stream_tbl_ptr =
        (TxBAStreamTbl *)util_peek_list(pmadapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr, MNULL, MNULL);
    if (!tx_ba_stream_tbl_ptr)
    {
        wlan_release_ralist_lock(priv);
        LEAVE();
        return ret;
    }

    if (__memcmp(pmadapter, peer_address, zero_mac, MLAN_MAC_ADDR_LENGTH))
    {
        if (!wlan_11n_get_txbastream_tbl(priv, peer_address))
            return MLAN_STATUS_FAILURE;
    }

    while (tx_ba_stream_tbl_ptr != (TxBAStreamTbl *)&priv->tx_ba_stream_tbl_ptr)
    {
        if (tx_ba_stream_tbl_ptr->ba_status == BA_STREAM_SETUP_COMPLETE)
        {
            if (__memcmp(pmadapter, peer_address, zero_mac, MLAN_MAC_ADDR_LENGTH) &&
                !__memcmp(pmadapter, peer_address, tx_ba_stream_tbl_ptr->ra, MLAN_MAC_ADDR_LENGTH))
            {
                tx_ba_stream_tbl_ptr = wlan_11n_get_txbastream_tbl(priv, peer_address);

                if (tid == DELBA_ALL_TIDS)
                {
                    for (i = 0; i < MAX_NUM_TID; i++)
                    {
                        if (tx_ba_stream_tbl_ptr->ampdu_stat[i])
                        {
                            if (last_tx_ba_to_delete && (tx_ba_stream_tbl_ptr == last_tx_ba_to_delete))
                                ret = wlan_send_delba(priv, pioctl_req, i, tx_ba_stream_tbl_ptr->ra, 1);
                            else
                                ret = wlan_send_delba(priv, MNULL, i, tx_ba_stream_tbl_ptr->ra, 1);
                        }
                    }
                }
                else
                {
                    if (tx_ba_stream_tbl_ptr->ampdu_stat[tid])
                    {
                        if (last_tx_ba_to_delete && (tx_ba_stream_tbl_ptr == last_tx_ba_to_delete))
                            ret = wlan_send_delba(priv, pioctl_req, tid, tx_ba_stream_tbl_ptr->ra, 1);
                        else
                            ret = wlan_send_delba(priv, MNULL, tid, tx_ba_stream_tbl_ptr->ra, 1);
                    }
                }

                return ret;
            }
            else
            {
                if (tid == DELBA_ALL_TIDS)
                {
                    for (i = 0; i < MAX_NUM_TID; i++)
                    {
                        if (tx_ba_stream_tbl_ptr->ampdu_stat[i])
                        {
                            if (last_tx_ba_to_delete && (tx_ba_stream_tbl_ptr == last_tx_ba_to_delete))
                                ret = wlan_send_delba(priv, pioctl_req, i, tx_ba_stream_tbl_ptr->ra, 1);
                            else
                                ret = wlan_send_delba(priv, MNULL, i, tx_ba_stream_tbl_ptr->ra, 1);
                        }
                    }
                }
                else
                {
                    if (tx_ba_stream_tbl_ptr->ampdu_stat[tid])
                    {
                        if (last_tx_ba_to_delete && (tx_ba_stream_tbl_ptr == last_tx_ba_to_delete))
                            ret = wlan_send_delba(priv, pioctl_req, tid, tx_ba_stream_tbl_ptr->ra, 1);
                        else
                            ret = wlan_send_delba(priv, MNULL, tid, tx_ba_stream_tbl_ptr->ra, 1);
                    }
                }
            }
        }

        tx_ba_stream_tbl_ptr = tx_ba_stream_tbl_ptr->pnext;
    }
    wlan_release_ralist_lock(priv);

    LEAVE();
    return ret;
}

/**
 *  @brief This function will send DELBA to entries in the priv's
 *          rx reordering table
 *
 *  @param priv                 A pointer to mlan_private
 *  @param pioctl_req          A pointer to ioctl request buffer
 *  @param tid                  TID
 *  @param peer_address         A pointer to peer address
 *  @param last_rx_ba_to_delete A pointer to the last entry in RxReorderTbl
 *
 *  @return            MLAN_STATUS_SUCCESS or MLAN_STATUS_PENDING
 */
static mlan_status wlan_send_delba_to_entry_in_reorder_tbl(
    pmlan_private priv, pmlan_ioctl_req pioctl_req, t_u8 tid, t_u8 *peer_address, RxReorderTbl *last_rx_ba_to_delete)
{
    pmlan_adapter pmadapter = priv->adapter;
    RxReorderTbl *rx_reor_tbl_ptr;
    RxReorderTbl *next_rx_reor_tbl_ptr;
    t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH] = {0};
    mlan_status ret                     = MLAN_STATUS_SUCCESS;

    ENTER();

    rx_reor_tbl_ptr =
        (RxReorderTbl *)util_peek_list(pmadapter->pmoal_handle, &priv->rx_reorder_tbl_ptr,
                                       pmadapter->callbacks.moal_spin_lock, pmadapter->callbacks.moal_spin_unlock);
    if (!rx_reor_tbl_ptr)
    {
        LEAVE();
        return ret;
    }

    next_rx_reor_tbl_ptr = rx_reor_tbl_ptr->pnext;

    while (rx_reor_tbl_ptr != (RxReorderTbl *)&priv->rx_reorder_tbl_ptr)
    {
        if (rx_reor_tbl_ptr->ba_status == BA_STREAM_SETUP_COMPLETE)
        {
            if (((tid == DELBA_ALL_TIDS) || (tid == rx_reor_tbl_ptr->tid)) &&
                (!__memcmp(pmadapter, peer_address, zero_mac, MLAN_MAC_ADDR_LENGTH) ||
                 !__memcmp(pmadapter, peer_address, rx_reor_tbl_ptr->ta, MLAN_MAC_ADDR_LENGTH)))
            {
                if (last_rx_ba_to_delete && (rx_reor_tbl_ptr == last_rx_ba_to_delete))
                    ret = wlan_send_delba(priv, pioctl_req, rx_reor_tbl_ptr->tid, rx_reor_tbl_ptr->ta, 0);
                else
                    ret = wlan_send_delba(priv, MNULL, rx_reor_tbl_ptr->tid, rx_reor_tbl_ptr->ta, 0);
            }
        }

        rx_reor_tbl_ptr = next_rx_reor_tbl_ptr;

        next_rx_reor_tbl_ptr = rx_reor_tbl_ptr->pnext;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief IOCTL to delete BA
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return			MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_11n_ioctl_delba(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_11n_cfg *cfg = MNULL;
    TxBAStreamTbl *tx_ba_stream_tbl_ptr, *last_tx_ba_to_delete = MNULL;
    RxReorderTbl *rx_reor_tbl_ptr, *last_rx_ba_to_delete       = MNULL;
    t_u8 zero_mac[MLAN_MAC_ADDR_LENGTH] = {0};
    t_u8 tid, *peer_address;

    ENTER();

    cfg          = (mlan_ds_11n_cfg *)pioctl_req->pbuf;
    tid          = cfg->param.del_ba.tid;
    peer_address = cfg->param.del_ba.peer_mac_addr;

    PRINTM(MINFO, "DelBA: direction %d, TID %d, peer address " MACSTR "\n", cfg->param.del_ba.direction, tid,
           MAC2STR(peer_address));

    if (cfg->param.del_ba.direction & DELBA_RX)
    {
        rx_reor_tbl_ptr =
            (RxReorderTbl *)util_peek_list(pmadapter->pmoal_handle, &pmpriv->rx_reorder_tbl_ptr,
                                           pmadapter->callbacks.moal_spin_lock, pmadapter->callbacks.moal_spin_unlock);

        if (rx_reor_tbl_ptr)
        {
            while (rx_reor_tbl_ptr != (RxReorderTbl *)&pmpriv->rx_reorder_tbl_ptr)
            {
                if (rx_reor_tbl_ptr->ba_status == BA_STREAM_SETUP_COMPLETE)
                {
                    if (((tid == DELBA_ALL_TIDS) || (tid == rx_reor_tbl_ptr->tid)) &&
                        (!__memcmp(pmadapter, peer_address, zero_mac, MLAN_MAC_ADDR_LENGTH) ||
                         !__memcmp(pmadapter, peer_address, rx_reor_tbl_ptr->ta, MLAN_MAC_ADDR_LENGTH)))
                    {
                        /* Found RX BA to delete */
                        last_rx_ba_to_delete = rx_reor_tbl_ptr;
                    }
                }
                rx_reor_tbl_ptr = rx_reor_tbl_ptr->pnext;
            }
        }
    }

    if ((last_rx_ba_to_delete == MNULL) && (cfg->param.del_ba.direction & DELBA_TX))
    {
        wlan_request_ralist_lock(pmpriv);
        tx_ba_stream_tbl_ptr =
            (TxBAStreamTbl *)util_peek_list(pmadapter->pmoal_handle, &pmpriv->tx_ba_stream_tbl_ptr, MNULL, MNULL);

        if (tx_ba_stream_tbl_ptr)
        {
            while (tx_ba_stream_tbl_ptr != (TxBAStreamTbl *)&pmpriv->tx_ba_stream_tbl_ptr)
            {
                if (tx_ba_stream_tbl_ptr->ba_status == BA_STREAM_SETUP_COMPLETE)
                {
                    if (((tid == DELBA_ALL_TIDS) || (tx_ba_stream_tbl_ptr->ampdu_stat[tid])) &&
                        (!__memcmp(pmadapter, peer_address, zero_mac, MLAN_MAC_ADDR_LENGTH) ||
                         !__memcmp(pmadapter, peer_address, tx_ba_stream_tbl_ptr->ra, MLAN_MAC_ADDR_LENGTH)))
                    {
                        /* Found TX BA to delete */
                        last_tx_ba_to_delete = tx_ba_stream_tbl_ptr;
                    }
                }
                tx_ba_stream_tbl_ptr = tx_ba_stream_tbl_ptr->pnext;
            }
        }
        wlan_release_ralist_lock(pmpriv);
    }

    if (cfg->param.del_ba.direction & DELBA_TX)
    {
        if (last_rx_ba_to_delete)
            ret = wlan_send_delba_to_entry_in_txbastream_tbl(pmpriv, MNULL, tid, peer_address, MNULL);
        else
            ret =
                wlan_send_delba_to_entry_in_txbastream_tbl(pmpriv, pioctl_req, tid, peer_address, last_tx_ba_to_delete);
    }
    if (last_rx_ba_to_delete)
    {
        ret = wlan_send_delba_to_entry_in_reorder_tbl(pmpriv, pioctl_req, tid, peer_address, last_rx_ba_to_delete);
    }

    LEAVE();
    return ret;
}


/********************************************************
    Global Functions
********************************************************/

#ifdef STA_SUPPORT
/**
 *  @brief This function fills the cap info
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param pht_cap      A pointer to MrvlIETypes_HTCap_t structure
 *  @param bands        Band configuration
 *
 *  @return             N/A
 */
static void wlan_fill_cap_info(mlan_private *priv, MrvlIETypes_HTCap_t *pht_cap, t_u16 bands)
{
    mlan_adapter *pmadapter = priv->adapter;
    t_u32 usr_dot_11n_dev_cap;

    ENTER();

#if CONFIG_5GHz_SUPPORT
    if ((bands & BAND_A) != 0U)
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_a;
    }
    else
#endif
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_bg;
    }

    if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_SUPPCHANWIDTH(pht_cap->ht_cap.ht_cap_info);
        SETHT_DSSSCCK40(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_SUPPCHANWIDTH(pht_cap->ht_cap.ht_cap_info);
        RESETHT_DSSSCCK40(pht_cap->ht_cap.ht_cap_info);
    }

    if (ISSUPP_GREENFIELD(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_GREENFIELD(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_GREENFIELD(pht_cap->ht_cap.ht_cap_info);
    }

    if (ISSUPP_SHORTGI20(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_SHORTGI20(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_SHORTGI20(pht_cap->ht_cap.ht_cap_info);
    }

    if (ISSUPP_SHORTGI40(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_SHORTGI40(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_SHORTGI40(pht_cap->ht_cap.ht_cap_info);
    }

    if (ISSUPP_RXSTBC(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_RXSTBC(pht_cap->ht_cap.ht_cap_info, 1U);
    }
    else
    {
        RESETHT_RXSTBC(pht_cap->ht_cap.ht_cap_info);
    }

    if (ISENABLED_40MHZ_INTOLARENT(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_40MHZ_INTOLARANT(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_40MHZ_INTOLARANT(pht_cap->ht_cap.ht_cap_info);
    }

    /* No user config for LDPC coding capability yet */
    if (ISSUPP_RXLDPC(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_LDPCCODINGCAP(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_LDPCCODINGCAP(pht_cap->ht_cap.ht_cap_info);
    }

    /* No user config for TX STBC yet */
    if (ISSUPP_TXSTBC(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_TXSTBC(pht_cap->ht_cap.ht_cap_info);
    }
    else
    {
        RESETHT_TXSTBC(pht_cap->ht_cap.ht_cap_info);
    }

    /* No user config for Delayed BACK yet */
    RESETHT_DELAYEDBACK(pht_cap->ht_cap.ht_cap_info);

    /* Need change to support 8k AMSDU receive */
    RESETHT_MAXAMSDU(pht_cap->ht_cap.ht_cap_info);

    /* SM power save */
    if (ISSUPP_MIMOPS(priv->adapter->hw_dot_11n_dev_cap) != 0U)
    {
        RESETHT_SM_POWERSAVE(pht_cap->ht_cap.ht_cap_info); /* Enable HT SMPS*/
    }
    else
    {
        SETHT_STATIC_SMPS(pht_cap->ht_cap.ht_cap_info); /* Disable HT SMPS */
    }

    LEAVE();
}

/**
 *  @brief This function fills the HT cap tlv
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param pht_cap      A pointer to MrvlIETypes_HTCap_t structure
 *  @param bands        Band configuration
 *
 *  @return             N/A
 */
void wlan_fill_ht_cap_tlv(mlan_private *priv, MrvlIETypes_HTCap_t *pht_cap, t_u16 bands)
{
    mlan_adapter *pmadapter = priv->adapter;
    int rx_mcs_supp;
    t_u32 usr_dot_11n_dev_cap;

    ENTER();

#if CONFIG_5GHz_SUPPORT
    if ((bands & BAND_A) != 0U)
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_a;
    }
    else
#endif
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_bg;
    }

    /* Fill HT cap info */
    wlan_fill_cap_info(priv, pht_cap, bands);
    pht_cap->ht_cap.ht_cap_info = wlan_cpu_to_le16(pht_cap->ht_cap.ht_cap_info);

    /* Set ampdu param */
    SETAMPDU_SIZE(pht_cap->ht_cap.ampdu_param, AMPDU_FACTOR_64K);

#ifdef RW610_SERIES
    SETAMPDU_SPACING(pht_cap->ht_cap.ampdu_param, 0x5);
#else
    SETAMPDU_SPACING(pht_cap->ht_cap.ampdu_param, pmadapter->hw_mpdu_density);
#endif

    rx_mcs_supp = GET_RXMCSSUPP(pmadapter->usr_dev_mcs_support);
    /* Set MCS for 1x1/2x2 */
    (void)__memset(pmadapter, (t_u8 *)pht_cap->ht_cap.supported_mcs_set, 0xff, rx_mcs_supp);
    /* Clear all the other values */
    (void)__memset(pmadapter, (t_u8 *)&pht_cap->ht_cap.supported_mcs_set[rx_mcs_supp], 0, NUM_MCS_FIELD - rx_mcs_supp);
    /* Set MCS32 with 40MHz support */
    /* if current channel only support 20MHz, we should not set 40Mz supprot*/
    if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap) != 0U)
    {
        SETHT_MCS32(pht_cap->ht_cap.supported_mcs_set);
    }

    /* Clear RD responder bit */
    RESETHT_EXTCAP_RDG(pht_cap->ht_cap.ht_ext_cap);
    pht_cap->ht_cap.ht_ext_cap = wlan_cpu_to_le16(pht_cap->ht_cap.ht_ext_cap);

    /* Set Tx BF cap */
    pht_cap->ht_cap.tx_bf_cap = wlan_cpu_to_le32(priv->tx_bf_cap);

    LEAVE();
    return;
}
#endif /* STA_SUPPORT */

/**
 *  @brief This function prints the 802.11n device capability
 *
 *  @param pmadapter     A pointer to mlan_adapter structure
 *  @param cap           Capability value
 *
 *  @return        N/A
 */
void wlan_show_dot11ndevcap(pmlan_adapter pmadapter, t_u32 cap)
{
    ENTER();

    PRINTM(MINFO, "GET_HW_SPEC: Maximum MSDU length = %s octets\n", (ISSUPP_MAXAMSDU(cap) ? "7935" : "3839"));
    PRINTM(MINFO, "GET_HW_SPEC: Beam forming %s\n", (ISSUPP_BEAMFORMING(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Greenfield preamble %s\n", (ISSUPP_GREENFIELD(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: AMPDU %s\n", (ISSUPP_AMPDU(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: MIMO Power Save %s\n", (ISSUPP_MIMOPS(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Rx STBC %s\n", (ISSUPP_RXSTBC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Tx STBC %s\n", (ISSUPP_TXSTBC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Short GI for 40 Mhz %s\n", (ISSUPP_SHORTGI40(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Short GI for 20 Mhz %s\n", (ISSUPP_SHORTGI20(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: LDPC coded packet receive %s\n", (ISSUPP_RXLDPC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Number of TX BA streams supported %d\n", ISSUPP_GETTXBASTREAM(cap));
    PRINTM(MINFO, "GET_HW_SPEC: 40 Mhz channel width %s\n", (ISSUPP_CHANWIDTH40(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: 20 Mhz channel width %s\n", (ISSUPP_CHANWIDTH20(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: 10 Mhz channel width %s\n", (ISSUPP_CHANWIDTH10(cap) ? "supported" : "not supported"));

    if (ISSUPP_RXANTENNAA(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Rx antenna A\n");
    }
    if (ISSUPP_RXANTENNAB(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Rx antenna B\n");
    }
    if (ISSUPP_RXANTENNAC(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Rx antenna C\n");
    }
    if (ISSUPP_RXANTENNAD(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Rx antenna D\n");
    }
    if (ISSUPP_TXANTENNAA(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Tx antenna A\n");
    }
    if (ISSUPP_TXANTENNAB(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Tx antenna B\n");
    }
    if (ISSUPP_TXANTENNAC(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Tx antenna C\n");
    }
    if (ISSUPP_TXANTENNAD(cap) != 0U)
    {
        PRINTM(MINFO, "GET_HW_SPEC: Presence of Tx antenna D\n");
    }

    LEAVE();
    return;
}

/**
 *  @brief This function prints the 802.11n device MCS
 *
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param support   Support value
 *
 *  @return        N/A
 */
void wlan_show_devmcssupport(pmlan_adapter pmadapter, t_u8 support)
{
    ENTER();

    PRINTM(MINFO, "GET_HW_SPEC: MCSs for %dx%d MIMO\n", GET_RXMCSSUPP(support), GET_TXMCSSUPP(support));

    LEAVE();
    return;
}


/**
 *  @brief This function prepares command of reconfigure tx buf
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_recfg_tx_buf(mlan_private *priv, HostCmd_DS_COMMAND *cmd, int cmd_action, void *pdata_buf)
{
    HostCmd_DS_TXBUF_CFG *ptx_buf = &cmd->params.tx_buf;
    t_u16 action                  = (t_u16)cmd_action;
    t_u16 buf_size                = *((t_u16 *)pdata_buf);

    ENTER();
    cmd->command    = wlan_cpu_to_le16(HostCmd_CMD_RECONFIGURE_TX_BUFF);
    cmd->size       = wlan_cpu_to_le16(sizeof(HostCmd_DS_TXBUF_CFG) + S_DS_GEN);
    ptx_buf->action = wlan_cpu_to_le16(action);
    switch (action)
    {
        case HostCmd_ACT_GEN_SET:
            PRINTM(MCMND, "set tx_buf = %d\n", buf_size);
            ptx_buf->buff_size = wlan_cpu_to_le16(buf_size);
            break;
        case HostCmd_ACT_GEN_GET:
        default:
            ptx_buf->buff_size = 0;
            break;
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of amsdu aggr control
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_amsdu_aggr_ctrl(mlan_private *priv, HostCmd_DS_COMMAND *cmd, int cmd_action, void *pdata_buf)
{
    HostCmd_DS_AMSDU_AGGR_CTRL *pamsdu_ctrl = &cmd->params.amsdu_aggr_ctrl;
    t_u16 action                            = (t_u16)cmd_action;
    mlan_ds_11n_amsdu_aggr_ctrl *aa_ctrl    = (mlan_ds_11n_amsdu_aggr_ctrl *)pdata_buf;

    ENTER();

    cmd->command        = wlan_cpu_to_le16(HostCmd_CMD_AMSDU_AGGR_CTRL);
    cmd->size           = wlan_cpu_to_le16(sizeof(HostCmd_DS_AMSDU_AGGR_CTRL) + S_DS_GEN);
    pamsdu_ctrl->action = wlan_cpu_to_le16(action);
    switch (action)
    {
        case HostCmd_ACT_GEN_SET:
            pamsdu_ctrl->enable        = wlan_cpu_to_le16(aa_ctrl->enable);
            pamsdu_ctrl->curr_buf_size = 0;
            break;
        case HostCmd_ACT_GEN_GET:
        default:
            pamsdu_ctrl->curr_buf_size = 0;
            break;
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of amsdu aggr ctrl
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
#if CONFIG_AMSDU_IN_AMPDU
mlan_status wlan_ret_amsdu_aggr_ctrl(IN pmlan_private pmpriv,
                                     IN HostCmd_DS_COMMAND *resp,
                                     IN mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_11n_cfg *cfg                   = MNULL;
    HostCmd_DS_AMSDU_AGGR_CTRL *amsdu_ctrl = &resp->params.amsdu_aggr_ctrl;

    ENTER();


    if (pioctl_buf != NULL)
    {
        cfg                                      = (mlan_ds_11n_cfg *)(void *)pioctl_buf->pbuf;
        cfg->param.amsdu_aggr_ctrl.enable        = wlan_le16_to_cpu(amsdu_ctrl->enable);
        cfg->param.amsdu_aggr_ctrl.curr_buf_size = wlan_le16_to_cpu(amsdu_ctrl->curr_buf_size);
    }
    pmpriv->is_amsdu_enabled = wlan_le16_to_cpu(amsdu_ctrl->enable);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function prepares 11n cfg command
 *
 *  @param pmpriv    	A pointer to mlan_private structure
 *  @param cmd	   	A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action 	the action: GET or SET
 *  @param pdata_buf 	A pointer to data buffer
 *  @return 	   	MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11n_cfg(IN pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND *cmd,
                             IN t_u16 cmd_action,
                             IN t_void *pdata_buf)
{
    HostCmd_DS_11N_CFG *htcfg = &cmd->params.htcfg;
    mlan_ds_11n_tx_cfg *txcfg = (mlan_ds_11n_tx_cfg *)pdata_buf;

    ENTER();
    cmd->command       = wlan_cpu_to_le16(HostCmd_CMD_11N_CFG);
    cmd->size          = wlan_cpu_to_le16(sizeof(HostCmd_DS_11N_CFG) + S_DS_GEN);
    htcfg->action      = wlan_cpu_to_le16(cmd_action);
    htcfg->ht_tx_cap   = wlan_cpu_to_le16(txcfg->httxcap);
    htcfg->ht_tx_info  = wlan_cpu_to_le16(txcfg->httxinfo);
    htcfg->misc_config = wlan_cpu_to_le16(txcfg->misc_cfg);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of 11ncfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_11n_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_11n_cfg *cfg      = MNULL;
    HostCmd_DS_11N_CFG *htcfg = &resp->params.htcfg;


    ENTER();
    if (pioctl_buf != MNULL && (wlan_le16_to_cpu(htcfg->action) == HostCmd_ACT_GEN_GET))
    {
        cfg                        = (mlan_ds_11n_cfg *)(void *)pioctl_buf->pbuf;
        cfg->param.tx_cfg.httxcap  = wlan_le16_to_cpu(htcfg->ht_tx_cap);
        cfg->param.tx_cfg.httxinfo = wlan_le16_to_cpu(htcfg->ht_tx_info);
        cfg->param.tx_cfg.misc_cfg = wlan_le16_to_cpu(htcfg->misc_config);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#ifdef STA_SUPPORT

/**
 *  @brief This function check if ht40 is allowed in current region
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *
 *  @return MTRUE/MFALSE
 */
static int wlan_check_chan_width_ht40_by_region(IN mlan_private *pmpriv, IN BSSDescriptor_t *pbss_desc)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    t_u8 i                  = 0;
    int cover_pri_chan      = MFALSE;
    t_u8 pri_chan;
    t_u8 chan_offset;
    t_u8 num_cfp;

    ENTER();

    if (pbss_desc->pht_info == MNULL)
    {
        PRINTM(MERROR, "ht_info pointer NULL, force use HT20\n");
        LEAVE();
        return MFALSE;
    }
#if 0
    if(pmpriv->curr_chan_flags & CHAN_FLAGS_NO_HT40PLUS &&
        pmpriv->curr_chan_flags & CHAN_FLAGS_NO_HT40MINUS){
        LEAVE();
        return MFALSE;
    }
#endif
    pri_chan    = pbss_desc->pht_info->ht_info.pri_chan;
    chan_offset = GET_SECONDARYCHAN(pbss_desc->pht_info->ht_info.field2);
#if 0
    if((chan_offset == SEC_CHAN_ABOVE) && (pmpriv->curr_chan_flags & CHAN_FLAGS_NO_HT40PLUS))
        return MFALSE;
    if((chan_offset == SEC_CHAN_BELOW) && (pmpriv->curr_chan_flags & CHAN_FLAGS_NO_HT40MINUS))
        return MFALSE;
#endif
    num_cfp = pmadapter->region_channel[0].num_cfp;

    if ((pbss_desc->bss_band & (BAND_B | BAND_G)) && pmadapter->region_channel[0].valid)
    {
        for (i = 0; i < num_cfp; i++)
        {
            if (pri_chan == pmadapter->region_channel[0].pcfp[i].channel)
            {
                cover_pri_chan = MTRUE;
                break;
            }
        }
        if (!cover_pri_chan)
        {
            PRINTM(MERROR, "Invalid channel, force use HT20\n");
            LEAVE();
            return MFALSE;
        }

        if (chan_offset == (t_u8)SEC_CHAN_ABOVE)
        {
            if (pri_chan > num_cfp - 4U)
            {
                PRINTM(MERROR, "Invalid second channel offset, force use HT20\n");
                LEAVE();
                return MFALSE;
            }
        }
    }
    LEAVE();
    return MTRUE;
}

/**
 *  @brief This function append the 802_11N tlv
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *  @param ppbuffer     A Pointer to command buffer pointer
 *
 *  @return bytes added to the buffer
 */
t_u32 wlan_cmd_append_11n_tlv(IN mlan_private *pmpriv, IN BSSDescriptor_t *pbss_desc, OUT t_u8 **ppbuffer)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    MrvlIETypes_HTCap_t *pht_cap;
    MrvlIETypes_HTInfo_t *pht_info;
    MrvlIEtypes_ChanListParamSet_t *pchan_list;
#if CONFIG_5GHz_SUPPORT
    MrvlIETypes_2040BSSCo_t *p2040_bss_co;
#endif
    MrvlIETypes_ExtCap_t *pext_cap;
    t_u32 usr_dot_11n_dev_cap, orig_usr_dot_11n_dev_cap = 0;
    t_u8 usr_dot_11ac_bw;
    t_u32 ret_len = 0;

    ENTER();

    /* Null Checks */
    if (ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }
    if (*ppbuffer == MNULL)
    {
        LEAVE();
        return 0;
    }

#if CONFIG_5GHz_SUPPORT
    if ((pbss_desc->bss_band & BAND_A) != 0U)
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_a;
    }
    else
#endif
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_bg;
    }

    if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS)
    {
        usr_dot_11ac_bw = BW_FOLLOW_VHTCAP;
    }
    else
    {
        usr_dot_11ac_bw = pmadapter->usr_dot_11ac_bw;
    }

    if (((pbss_desc->bss_band & (BAND_B | BAND_G
#if CONFIG_5GHz_SUPPORT
                                 | BAND_A
#endif
                                 )) &&
         ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap) && !wlan_check_chan_width_ht40_by_region(pmpriv, pbss_desc)) ||
        (pbss_desc->bss_band & (BAND_B | BAND_G | BAND_GN)))
    {
        orig_usr_dot_11n_dev_cap = usr_dot_11n_dev_cap;
#ifdef RW610
        RESETSUPP_CHANWIDTH40(usr_dot_11n_dev_cap);
        RESETSUPP_SHORTGI40(usr_dot_11n_dev_cap);
#endif
        RESET_40MHZ_INTOLARENT(usr_dot_11n_dev_cap);
        pmadapter->usr_dot_11n_dev_cap_bg = usr_dot_11n_dev_cap;
        pbss_desc->curr_bandwidth         = BW_20MHZ;
    }

    if (pbss_desc->pht_cap != MNULL)
    {
        pht_cap = (MrvlIETypes_HTCap_t *)(void *)*ppbuffer;
        (void)__memset(pmadapter, pht_cap, 0, sizeof(MrvlIETypes_HTCap_t));
        pht_cap->header.type = wlan_cpu_to_le16(HT_CAPABILITY);
        pht_cap->header.len  = sizeof(HTCap_t);
        (void)__memcpy(pmadapter, (t_u8 *)pht_cap + sizeof(MrvlIEtypesHeader_t),
                       (t_u8 *)pbss_desc->pht_cap + sizeof(IEEEtypes_Header_t), pht_cap->header.len);

        if (!ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap))
        {
            pht_cap->ht_cap.ht_cap_info &= ~(MBIT(1) | MBIT(6));
        }

        pht_cap->ht_cap.ht_cap_info = wlan_le16_to_cpu(pht_cap->ht_cap.ht_cap_info);
        pht_cap->ht_cap.ht_ext_cap  = wlan_le16_to_cpu(pht_cap->ht_cap.ht_ext_cap);
        wlan_fill_ht_cap_tlv(pmpriv, pht_cap, pbss_desc->bss_band);
        if (wlan_use_non_default_ht_vht_cap(pbss_desc))
        {
            /* Indicate 3 streams in TxBF cap*/
            pht_cap->ht_cap.tx_bf_cap = ((pht_cap->ht_cap.tx_bf_cap & (~(0x3 << 23))) | (0x2 << 23));
            pht_cap->ht_cap.tx_bf_cap = ((pht_cap->ht_cap.tx_bf_cap & (~(0x3 << 27))) | (0x2 << 27));
        }

        HEXDUMP("HT_CAPABILITIES IE", (t_u8 *)pht_cap, sizeof(MrvlIETypes_HTCap_t));
        *ppbuffer += sizeof(MrvlIETypes_HTCap_t);
        ret_len += sizeof(MrvlIETypes_HTCap_t);
        pht_cap->header.len = wlan_cpu_to_le16(pht_cap->header.len);
    }

    if (pbss_desc->pht_info != MNULL)
    {
        if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS)
        {
            pht_info = (MrvlIETypes_HTInfo_t *)(void *)*ppbuffer;
            (void)__memset(pmadapter, pht_info, 0, sizeof(MrvlIETypes_HTInfo_t));
            pht_info->header.type = wlan_cpu_to_le16(HT_OPERATION);
            pht_info->header.len  = sizeof(HTInfo_t);

            (void)__memcpy(pmadapter, (t_u8 *)pht_info + sizeof(MrvlIEtypesHeader_t),
                           (t_u8 *)pbss_desc->pht_info + sizeof(IEEEtypes_Header_t), pht_info->header.len);

            if (!ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap))
            {
                RESET_CHANWIDTH40(pht_info->ht_info.field2);
            }

            *ppbuffer += sizeof(MrvlIETypes_HTInfo_t);
            ret_len += sizeof(MrvlIETypes_HTInfo_t);
            pht_info->header.len = wlan_cpu_to_le16(pht_info->header.len);
        }

        pchan_list = (MrvlIEtypes_ChanListParamSet_t *)(void *)*ppbuffer;
        (void)__memset(pmadapter, pchan_list, 0, sizeof(MrvlIEtypes_ChanListParamSet_t));
        pchan_list->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
        pchan_list->header.len  = sizeof(MrvlIEtypes_ChanListParamSet_t) - sizeof(MrvlIEtypesHeader_t);
        pchan_list->chan_scan_param[0].chan_number = pbss_desc->pht_info->ht_info.pri_chan;
        pchan_list->chan_scan_param[0].radio_type  = wlan_band_to_radio_type((t_u8)pbss_desc->bss_band);
        /* support the VHT if the network to be join has the VHT operation */
        if (ISSUPP_11ACENABLED(pmadapter->fw_cap_info) && (usr_dot_11ac_bw == BW_FOLLOW_VHTCAP) &&
            ISSUPP_CHANWIDTH40(pmadapter->hw_dot_11n_dev_cap) &&
            wlan_11ac_bandconfig_allowed(pmpriv, pbss_desc->bss_band) && pbss_desc->pvht_oprat != MNULL &&
            pbss_desc->pvht_oprat->chan_width == VHT_OPER_CHWD_80MHZ)
        {
            //               pchan_list->chan_scan_param[0].bandcfg.chanWidth = CHAN_BW_80MHZ;
            //               pchan_list->chan_scan_param[0].bandcfg.chan2Offset =
            //                        GET_SECONDARYCHAN(pbss_desc->pht_info->ht_info.field2);
            SET_SECONDARYCHAN(pchan_list->chan_scan_param[0].radio_type,
                              GET_SECONDARYCHAN(pbss_desc->pht_info->ht_info.field2));
            pchan_list->chan_scan_param[0].radio_type |= (MBIT(2) | MBIT(3));
            pbss_desc->curr_bandwidth = BW_80MHZ;
        }
        else if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap) &&
                 ISALLOWED_CHANWIDTH40(pbss_desc->pht_info->ht_info.field2) &&
                 wlan_check_chan_width_ht40_by_region(pmpriv, pbss_desc))
        {
            SET_SECONDARYCHAN(pchan_list->chan_scan_param[0].radio_type,
                              GET_SECONDARYCHAN(pbss_desc->pht_info->ht_info.field2));
            pchan_list->chan_scan_param[0].radio_type |= MBIT(3);
            pbss_desc->curr_bandwidth = BW_40MHZ;
        }
        else
        {
            /* Do nothing */
        }

        HEXDUMP("ChanList", (t_u8 *)pchan_list, sizeof(MrvlIEtypes_ChanListParamSet_t));
        HEXDUMP("pht_info", (t_u8 *)pbss_desc->pht_info, sizeof(MrvlIETypes_HTInfo_t) - 2);
        *ppbuffer += sizeof(MrvlIEtypes_ChanListParamSet_t);
        ret_len += sizeof(MrvlIEtypes_ChanListParamSet_t);
        pchan_list->header.len = wlan_cpu_to_le16(pchan_list->header.len);
    }

#if CONFIG_5GHz_SUPPORT
    if ((pbss_desc->bss_band & (BAND_A | BAND_AN)) && pbss_desc->pbss_co_2040 != MNULL)
    {
        p2040_bss_co = (MrvlIETypes_2040BSSCo_t *)(void *)*ppbuffer;
        (void)__memset(pmadapter, p2040_bss_co, 0, sizeof(MrvlIETypes_2040BSSCo_t));
        p2040_bss_co->header.type = wlan_cpu_to_le16(BSSCO_2040);
        p2040_bss_co->header.len  = sizeof(BSSCo2040_t);

        (void)__memcpy(pmadapter, (t_u8 *)p2040_bss_co + sizeof(MrvlIEtypesHeader_t),
                       (t_u8 *)pbss_desc->pbss_co_2040 + sizeof(IEEEtypes_Header_t), p2040_bss_co->header.len);

        HEXDUMP("20/40 BSS Coexistence IE", (t_u8 *)p2040_bss_co, sizeof(MrvlIETypes_2040BSSCo_t));
        *ppbuffer += sizeof(MrvlIETypes_2040BSSCo_t);
        ret_len += sizeof(MrvlIETypes_2040BSSCo_t);
        p2040_bss_co->header.len = wlan_cpu_to_le16(p2040_bss_co->header.len);
    }
#endif

    if (pbss_desc->pext_cap != NULL)
    {
        pext_cap = (MrvlIETypes_ExtCap_t *)(void *)*ppbuffer;
        (void)__memset(pmadapter, pext_cap, 0, sizeof(MrvlIETypes_ExtCap_t));
        pext_cap->header.type = wlan_cpu_to_le16(EXT_CAPABILITY);
        pext_cap->header.len  = sizeof(ExtCap_t);

        (void)__memcpy(pmadapter, (t_u8 *)pext_cap + sizeof(MrvlIEtypesHeader_t),
                       (t_u8 *)pbss_desc->pext_cap + sizeof(IEEEtypes_Header_t), pbss_desc->pext_cap->ieee_hdr.len);

#if CONFIG_MULTI_BSSID_SUPPORT
        if (pbss_desc && pbss_desc->multi_bssid_ap)
            SET_EXTCAP_MULTI_BSSID(pext_cap->ext_cap);
#endif

#if !defined(SD8801) && !defined(RW610)
        pext_cap->ext_cap.BSS_CoexistSupport = 0x01; /*2040 CoEx support must be always set*/
#endif

        if (pmpriv->hotspot_cfg & HOTSPOT_ENABLED)
        {
            if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_INTERWORKING_IND) != 0U)
            {
                pext_cap->ext_cap.Interworking = 1;
            }
            if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_TDLS_IND) != 0U)
            {
                pext_cap->ext_cap.TDLSSupport = 1;
            }
        }
#if (CONFIG_WNM_PS)
        if ((((mlan_private *)mlan_adap->priv[0])->wnm_set == true) && (pbss_desc->pext_cap->ext_cap.WNM_Sleep == true))
        {
            pext_cap->ext_cap.WNM_Sleep = 1;
        }
        else
        {
            pext_cap->ext_cap.WNM_Sleep = 0;
        }
#endif

#if CONFIG_11V
        if (pbss_desc->pext_cap->ext_cap.BSS_Transition == true)
        {
            pext_cap->ext_cap.BSS_Transition = 1;
        }
        else
        {
            pext_cap->ext_cap.BSS_Transition = 0;
        }
#endif
#if CONFIG_11MC || CONFIG_11AZ
        pext_cap->ext_cap.FTMI = 1;
#endif
#if CONFIG_11MC
        pext_cap->ext_cap.CivicLocation = 1;
#endif
#if CONFIG_11AX
        SET_EXTCAP_TWT_REQ(pext_cap->ext_cap);
        pext_cap->ext_cap.TWTResp = 0;
#endif
        HEXDUMP("Extended Capabilities IE", (t_u8 *)pext_cap, sizeof(MrvlIETypes_ExtCap_t));
        *ppbuffer += sizeof(MrvlIETypes_ExtCap_t);
        ret_len += sizeof(MrvlIETypes_ExtCap_t);
        pext_cap->header.len = wlan_cpu_to_le16(pext_cap->header.len);
    }
    else if (pmpriv->hotspot_cfg & HOTSPOT_ENABLED)
    {
        wlan_add_ext_capa_info_ie(pmpriv, pbss_desc, ppbuffer);
        ret_len += sizeof(MrvlIETypes_ExtCap_t);
    }
    else
    {
        /* Do nothing */
    }
    if (orig_usr_dot_11n_dev_cap != 0U)
    {
        pmadapter->usr_dot_11n_dev_cap_bg = orig_usr_dot_11n_dev_cap;
    }
    LEAVE();
    return ret_len;
}

#endif /* STA_SUPPORT */

/**
 *  @brief 11n configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_11n_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status   = MLAN_STATUS_SUCCESS;
    mlan_ds_11n_cfg *cfg = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_11n_cfg))
    {
        PRINTM(MINFO, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_11n_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    cfg = (mlan_ds_11n_cfg *)(void *)pioctl_req->pbuf;
    switch (cfg->sub_command)
    {
        case MLAN_OID_11N_CFG_TX:
            status = wlan_11n_ioctl_httxcfg(pmadapter, pioctl_req);
            break;
        case MLAN_OID_11N_HTCAP_CFG:
            status = wlan_11n_ioctl_htusrcfg(pmadapter, pioctl_req);
            break;
        case MLAN_OID_11N_CFG_DELBA:
            status = wlan_11n_ioctl_delba(pmadapter, pioctl_req);
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}

/**
 *  @brief This function will delete the given entry in Tx BA Stream table
 *
 *  @param priv    	Pointer to mlan_private
 *  @param ptx_tbl	Pointer to tx ba stream entry to delete
 *
 *  @return 	        N/A
 */
void wlan_11n_delete_txbastream_tbl_entry(mlan_private *priv, t_u8 *ra)
{
    TxBAStreamTbl *ptx_tbl  = MNULL;
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();

    (void)pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle, priv->tx_ba_stream_tbl_ptr.plock);

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ra)))
    {
        PRINTM(MINFO, "Delete BA stream table entry: %p\n", ptx_tbl);

        util_unlink_list(pmadapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr, (pmlan_linked_list)ptx_tbl, MNULL,
                         MNULL);
    }

    if(ptx_tbl == NULL)
    {
        return;
    }

    (void)pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle, priv->tx_ba_stream_tbl_ptr.plock);

#if !CONFIG_MEM_POOLS
    pmadapter->callbacks.moal_mfree(pmadapter->pmoal_handle, (t_u8 *)ptx_tbl);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, ptx_tbl);
#endif

    LEAVE();
}

/**
 *  @brief This function will delete all the entries in Tx BA Stream table
 *
 *  @param priv         A pointer to mlan_private
 *
 *  @return             N/A
 */
void wlan_11n_deleteall_txbastream_tbl(mlan_private *priv)
{
    TxBAStreamTbl *del_tbl_ptr = MNULL;

    ENTER();

    wlan_request_ralist_lock(priv);
    while ((del_tbl_ptr = (TxBAStreamTbl *)util_peek_list(priv->adapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr,
                                                          priv->adapter->callbacks.moal_spin_lock,
                                                          priv->adapter->callbacks.moal_spin_unlock)) != NULL)
    {
        wlan_11n_delete_txbastream_tbl_entry(priv, del_tbl_ptr->ra);
    }

    util_init_list((pmlan_linked_list)&priv->tx_ba_stream_tbl_ptr);
    wlan_release_ralist_lock(priv);

    LEAVE();
}

/**
 *  @brief This function will return the pointer to an entry in BA Stream
 *  		table which matches the give RA/TID pair
 *
 *  @param priv    A pointer to mlan_private
 *  @param ra      RA to find in reordering table
 *
 *  @return 	   A pointer to first entry matching RA/TID in BA stream
 *                 NULL if not found
 */
TxBAStreamTbl *wlan_11n_get_txbastream_tbl(mlan_private *priv, t_u8 *ra)
{
    TxBAStreamTbl *ptx_tbl;
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();
    ptx_tbl = (TxBAStreamTbl *)(void *)util_peek_list(pmadapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr,
                                                      pmadapter->callbacks.moal_spin_lock,
                                                      pmadapter->callbacks.moal_spin_unlock);
    if (ptx_tbl == MNULL)
    {
        LEAVE();
        return MNULL;
    }

    while (ptx_tbl != (TxBAStreamTbl *)(void *)&priv->tx_ba_stream_tbl_ptr)
    {
        DBG_HEXDUMP(MDAT_D, "RA", ptx_tbl->ra, MLAN_MAC_ADDR_LENGTH);

        if (!__memcmp(pmadapter, ptx_tbl->ra, ra, MLAN_MAC_ADDR_LENGTH))
        {
            LEAVE();
            return ptx_tbl;
        }

        ptx_tbl = ptx_tbl->pnext;
    }

    LEAVE();
    return MNULL;
}

/**
 *  @brief This function will create a entry in tx ba stream table for the
 *  		given RA/TID.
 *
 *  @param priv      A pointer to mlan_private
 *  @param ra        RA to find in reordering table
 *  @param tid	     TID to find in reordering table
 *  @param ba_status BA stream status to create the stream with
 *
 *  @return 	    N/A
 */
void wlan_11n_create_txbastream_tbl(mlan_private *priv, t_u8 *ra, baStatus_e ba_status)
{
    TxBAStreamTbl *newNode  = MNULL;
    pmlan_adapter pmadapter = priv->adapter;

    ENTER();

    if (!wlan_11n_get_txbastream_tbl(priv, ra))
    {
        DBG_HEXDUMP(MDAT_D, "RA", ra, MLAN_MAC_ADDR_LENGTH);

#if !CONFIG_MEM_POOLS
        pmadapter->callbacks.moal_malloc(pmadapter->pmoal_handle, sizeof(TxBAStreamTbl), MLAN_MEM_DEF,
                                         (t_u8 **)&newNode);
#else
        newNode = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif
        if (newNode == MNULL)
        {
            return;
        }

        (void)__memset(pmadapter, newNode, 0, sizeof(TxBAStreamTbl));
        util_init_list((pmlan_linked_list)newNode);

        newNode->ba_status   = ba_status;
        newNode->txba_thresh = OSA_RandRange(5, 5);
        (void)__memcpy(pmadapter, newNode->ra, ra, MLAN_MAC_ADDR_LENGTH);
        (void)__memset(priv->adapter, newNode->rx_seq, 0xff, sizeof(newNode->rx_seq));

        util_enqueue_list_tail(pmadapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr, (pmlan_linked_list)newNode,
                               pmadapter->callbacks.moal_spin_lock, pmadapter->callbacks.moal_spin_unlock);
    }

    LEAVE();
}

/**
 *  @brief This function will update ampdu status in tx ba stream table for the
 *  		given RA/TID.
 *
 *  @param priv      A pointer to mlan_private
 *  @param ra        RA to find in reordering table
 *  @param tid	     TID to find in reordering table
 *  @param status    ampdu status
 *
 *  @return 	    N/A
 */

void wlan_11n_update_txbastream_tbl_ampdu_stat(mlan_private *priv, t_u8 *ra, t_u8 status, t_u8 tid)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ra)))
    {
        ptx_tbl->ampdu_stat[tid] = status;
    }
    else
        PRINTM(MERROR, "update txbastream_tbl ampdu status error\n");

    LEAVE();
    return;
}

/**
 *  @brief This function will update ampdu supported in tx ba stream table for the
 *  		given RA.
 *
 *  @param priv      A pointer to mlan_private
 *  @param ra        RA to find in reordering table
 *  @param supported ampdu support
 *
 *  @return 	    N/A
 */

void wlan_11n_update_txbastream_tbl_ampdu_supported(mlan_private *priv, t_u8 *ra, t_u8 supported)
{
    TxBAStreamTbl *ptx_tbl;
    int i;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ra)))
    {
        for (i = 0; i < MAX_NUM_TID; i++)
            ptx_tbl->ampdu_supported[i] = supported;
    }
    else
        PRINTM(MERROR, "update txbastream_tbl ampdu supported error\n");

    LEAVE();
    return;
}

/**
 *  @brief This function will update ampdu tx threshold in tx ba stream table for the
 *  		given RA.
 *
 *  @param priv      A pointer to mlan_private
 *  @param ra        RA to find in reordering table
 *  @param tx_thresh tx ba threshold
 *
 *  @return 	    N/A
 */

void wlan_11n_update_txbastream_tbl_tx_thresh(mlan_private *priv, t_u8 *ra, t_u8 tx_thresh)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ra)))
    {
        ptx_tbl->txba_thresh = tx_thresh;
    }
    else
        PRINTM(MERROR, "update txbastream_tbl ampdu supported error\n");

    LEAVE();
    return;
}

/**
 *  @brief This function will update ampdu supported in tx ba stream table for the
 *  		given RA.
 *
 *  @param priv      A pointer to mlan_private
 *  @param ra        RA to find in reordering table
 *
 *  @return 	    N/A
 */

void wlan_11n_update_txbastream_tbl_tx_cnt(mlan_private *priv, t_u8 *ra)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ra)))
    {
        ptx_tbl->txpkt_cnt++;
    }
    else
        PRINTM(MERROR, "update txbastream_tbl tx cnt error\n");

    LEAVE();
    return;
}

/**
 *  @brief This function will get sta peer amsdu
 *
 *  @param priv      A pointer to mlan_private
 *
 *  @return 	    amsdu value
 */

int wlan_11n_get_sta_peer_amsdu(mlan_private *priv)
{
    TxBAStreamTbl *ptx_tbl = MNULL;
    int ret                = MFALSE;

    ENTER();

    if ((ptx_tbl = wlan_11n_get_txbastream_tbl(priv, priv->curr_bss_params.bss_descriptor.mac_address)))
    {
        ret = ptx_tbl->amsdu;
    }

    LEAVE();

    return ret;
}

/**
 *  @brief This function will send a block ack to given tid/ra
 *
 *  @param priv     A pointer to mlan_private
 *  @param tid	    TID to send the ADDBA
 *  @param peer_mac MAC address to send the ADDBA
 *
 *  @return 	    MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
int wlan_send_addba(mlan_private *priv, int tid, const t_u8 *peer_mac)
{
    HostCmd_DS_11N_ADDBA_REQ add_ba_req;
    static t_u8 dialog_tok;
    mlan_status ret;

    ENTER();

    PRINTM(MCMND, "Send addba: TID %d\n", tid);
    DBG_HEXDUMP(MCMD_D, "Send addba RA", peer_mac, MLAN_MAC_ADDR_LENGTH);

    add_ba_req.block_ack_param_set =
        (t_u16)((tid << BLOCKACKPARAM_TID_POS) | (priv->add_ba_param.tx_win_size << BLOCKACKPARAM_WINSIZE_POS) |
                IMMEDIATE_BLOCK_ACK);
#if CONFIG_AMSDU_IN_AMPDU
    /** enable AMSDU inside AMPDU */
    /* To be done: change priv->aggr_prio_tbl[tid].amsdu for specific AMSDU support by CLI cmd */
#if 0
    if (priv->add_ba_param.tx_amsdu && (priv->aggr_prio_tbl[tid].amsdu != BA_STREAM_NOT_ALLOWED))
#else
    if (priv->add_ba_param.tx_amsdu && priv->bss_type == MLAN_BSS_TYPE_STA)
#endif
    {
        add_ba_req.block_ack_param_set |= BLOCKACKPARAM_AMSDU_SUPP_MASK;
    }
#endif
    add_ba_req.block_ack_tmo = (t_u16)priv->add_ba_param.timeout;

    ++dialog_tok;

    if (dialog_tok == 0U)
    {
        dialog_tok = 1;
    }

    add_ba_req.dialog_token = dialog_tok;
    (void)__memset(priv->adapter, &add_ba_req.peer_mac_addr, 0x0, MLAN_MAC_ADDR_LENGTH);
    (void)__memcpy(priv->adapter, &add_ba_req.peer_mac_addr, peer_mac, MLAN_MAC_ADDR_LENGTH);
#ifdef DUMP_PACKET_MAC
    wmprintf("wlan_send_addba bss_type:%d\r\n", priv->bss_type);
    dump_mac_addr(NULL, peer_mac);
#endif
    /* We don't wait for the response of this command */
    ret = wifi_prepare_and_send_cmd(priv, HostCmd_CMD_11N_ADDBA_REQ, 0, 0, MNULL, &add_ba_req, priv->bss_type, NULL);

    LEAVE();
    return ret;
}

/**
 *  @brief This function will delete a block ack to given tid/ra
 *
 *  @param priv         A pointer to mlan_private
 *  @param pioctl_req   A pointer to ioctl request buffer
 *  @param tid          TID to send the ADDBA
 *  @param peer_mac     MAC address to send the ADDBA
 *  @param initiator    MTRUE if we have initiated ADDBA, MFALSE otherwise
 *
 *  @return             MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_send_delba(mlan_private *priv, pmlan_ioctl_req pioctl_req, int tid, t_u8 *peer_mac, int initiator)
{
    HostCmd_DS_11N_DELBA delba;
    mlan_status ret;

    ENTER();

    __memset(priv->adapter, &delba, 0, sizeof(delba));
    delba.del_ba_param_set = (tid << DELBA_TID_POS);

    if (initiator)
        DELBA_INITIATOR(delba.del_ba_param_set);
    else
        DELBA_RECIPIENT(delba.del_ba_param_set);

    (void)__memcpy(priv->adapter, &delba.peer_mac_addr, peer_mac, MLAN_MAC_ADDR_LENGTH);

    ret = wlan_prepare_cmd(priv, HostCmd_CMD_11N_DELBA, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req, (t_void *)&delba);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}

/**
 *  @brief This function handles the command response of
 *  		delete a block ack request
 *
 *  @param priv		A pointer to mlan_private structure
 *  @param del_ba	A pointer to command response buffer
 *
 *  @return        N/A
 */
void wlan_11n_delete_bastream(mlan_private *priv, t_u8 *del_ba)
{
    HostCmd_DS_11N_DELBA *pdel_ba = (HostCmd_DS_11N_DELBA *)(void *)del_ba;
    int tid;

    ENTER();

    DBG_HEXDUMP(MCMD_D, "Delba:", (t_u8 *)pdel_ba, 20);
    pdel_ba->del_ba_param_set = wlan_le16_to_cpu(pdel_ba->del_ba_param_set);
    pdel_ba->reason_code      = wlan_le16_to_cpu(pdel_ba->reason_code);

    tid = pdel_ba->del_ba_param_set >> DELBA_TID_POS;
    mlan_11n_update_bastream_tbl(priv, tid, pdel_ba->peer_mac_addr, TYPE_DELBA_RECEIVE,
                                 INITIATOR_BIT(pdel_ba->del_ba_param_set));

    LEAVE();
}

