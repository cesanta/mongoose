/** @file mlan_misc.c
 *
 *  @brief  This file provides Miscellaneous functions for MLAN module
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*************************************************************
Change Log:
    05/11/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#include <mlan_fw.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>


/**
 *  @brief This function allocates a mlan_buffer.
 *
 *  @param pmadapter Pointer to mlan_adapter
 *  @param data_len   Data length
 *  @param head_room  head_room reserved in mlan_buffer
 *  @param malloc_flag  flag to user moal_malloc
 *  @return           mlan_buffer pointer or MNULL
 */
pmlan_buffer wlan_alloc_mlan_buffer(mlan_adapter *pmadapter, t_u32 data_len, t_u32 head_room, t_u32 malloc_flag)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_buffer pmbuf = MNULL;
    t_u32 buf_size     = 0;
    /* t_u8 *tmp_buf = MNULL; */
    pmlan_callbacks pcb = &pmadapter->callbacks;

    ENTER();

    /* make sure that the data length is at least SDIO block size */
    data_len = ALIGN_SZ(data_len, MLAN_SDIO_BLOCK_SIZE);

    /* head_room is not implemented for malloc mlan buffer */
    if (malloc_flag == MTRUE)
    {
        buf_size = sizeof(mlan_buffer) + data_len + DMA_ALIGNMENT;
        ret =
            pcb->moal_malloc(pmadapter->pmoal_handle, buf_size, MLAN_MEM_DEF | MLAN_MEM_DMA, (t_u8 **)(void **)&pmbuf);
        if ((ret != MLAN_STATUS_SUCCESS) || (pmbuf == MNULL))
        {
            pmbuf = MNULL;
            goto exit;
        }
        (void)__memset(pmadapter, pmbuf, 0, sizeof(mlan_buffer));

        pmbuf->pdesc = MNULL;
        /* Align address */
        pmbuf->pbuf        = (t_u8 *)ALIGN_ADDR((t_u8 *)pmbuf + sizeof(mlan_buffer), DMA_ALIGNMENT);
        pmbuf->data_offset = 0;
        pmbuf->data_len    = data_len;
        pmbuf->flags |= MLAN_BUF_FLAG_MALLOC_BUF;
    }
exit:
    LEAVE();
    return pmbuf;
}

/**
 *  @brief This function frees a mlan_buffer.
 *
 *  @param pmadapter  Pointer to mlan_adapter
 *  @param pmbuf      Pointer to mlan_buffer
 *
 *  @return           N/A
 */
t_void wlan_free_mlan_buffer(mlan_adapter *pmadapter, pmlan_buffer pmbuf)
{
    return;
}


/**
 *  @brief This function will check if station list is empty
 *
 *  @param priv    A pointer to mlan_private
 *
 *  @return	   MFALSE/MTRUE
 */
t_u8 wlan_is_station_list_empty(mlan_private *priv)
{
    ENTER();
    if (!(util_peek_list(priv->adapter->pmoal_handle, &priv->sta_list, priv->adapter->callbacks.moal_spin_lock,
                         priv->adapter->callbacks.moal_spin_unlock)))
    {
        LEAVE();
        return MTRUE;
    }
    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function will return the pointer to station entry in station list
 *  		table which matches the give mac address
 *
 *  @param priv    A pointer to mlan_private
 *  @param mac     mac address to find in station list table
 *
 *  @return	   A pointer to structure sta_node
 */
sta_node *wlan_get_station_entry(mlan_private *priv, t_u8 *mac)
{
    sta_node *sta_ptr;

    ENTER();

    if (!mac)
    {
        LEAVE();
        return MNULL;
    }
    if (!(sta_ptr = (sta_node *)util_peek_list(priv->adapter->pmoal_handle, &priv->sta_list,
                                               priv->adapter->callbacks.moal_spin_lock,
                                               priv->adapter->callbacks.moal_spin_unlock)))
    {
        LEAVE();
        return MNULL;
    }
    while (sta_ptr != (sta_node *)&priv->sta_list)
    {
        if (!__memcmp(priv->adapter, sta_ptr->mac_addr, mac, MLAN_MAC_ADDR_LENGTH))
        {
            LEAVE();
            return sta_ptr;
        }
        sta_ptr = sta_ptr->pnext;
    }
    LEAVE();
    return MNULL;
}

/**
 *  @brief This function will add a pointer to station entry in station list
 *  		table with the give mac address, if it does not exist already
 *
 *  @param priv    A pointer to mlan_private
 *  @param mac     mac address to find in station list table
 *
 *  @return	   A pointer to structure sta_node
 */
sta_node *wlan_add_station_entry(mlan_private *priv, t_u8 *mac)
{
    sta_node *sta_ptr       = MNULL;
    mlan_adapter *pmadapter = priv->adapter;

    ENTER();
    pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);

    sta_ptr = wlan_get_station_entry(priv, mac);
    if (sta_ptr)
        goto done;
    if (priv->adapter->callbacks.moal_malloc(priv->adapter->pmoal_handle, sizeof(sta_node), MLAN_MEM_DEF,
                                             (t_u8 **)&sta_ptr))
    {
        PRINTM(MERROR, "Failed to allocate memory for station node\n");
        LEAVE();
        return MNULL;
    }
    (void)__memset(priv->adapter, sta_ptr, 0, sizeof(sta_node));
    (void)__memcpy(priv->adapter, sta_ptr->mac_addr, mac, MLAN_MAC_ADDR_LENGTH);
    util_enqueue_list_tail(priv->adapter->pmoal_handle, &priv->sta_list, (pmlan_linked_list)sta_ptr,
                           priv->adapter->callbacks.moal_spin_lock, priv->adapter->callbacks.moal_spin_unlock);
done:
    pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);
    LEAVE();
    return sta_ptr;
}

/**
 *  @brief This function will delete a station entry from station list
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param mac     station's mac address
 *
 *  @return	   N/A
 */
t_void wlan_delete_station_entry(mlan_private *priv, t_u8 *mac)
{
    sta_node *sta_ptr       = MNULL;
    mlan_adapter *pmadapter = priv->adapter;
    ENTER();
    pmadapter->callbacks.moal_spin_lock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);
    if ((sta_ptr = wlan_get_station_entry(priv, mac)))
    {
        util_unlink_list(priv->adapter->pmoal_handle, &priv->sta_list, (pmlan_linked_list)sta_ptr,
                         priv->adapter->callbacks.moal_spin_lock, priv->adapter->callbacks.moal_spin_unlock);
        priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle, (t_u8 *)sta_ptr);
    }
    pmadapter->callbacks.moal_spin_unlock(pmadapter->pmoal_handle, priv->wmm.ra_list_spinlock);
    LEAVE();
    return;
}

/**
 *  @brief Clean up wapi station list
 *
 *  @param priv  Pointer to the mlan_private driver data struct
 *
 *  @return      N/A
 */
t_void wlan_delete_station_list(pmlan_private priv)
{
    sta_node *sta_ptr;

    ENTER();
    while ((sta_ptr = (sta_node *)util_dequeue_list(priv->adapter->pmoal_handle, &priv->sta_list,
                                                    priv->adapter->callbacks.moal_spin_lock,
                                                    priv->adapter->callbacks.moal_spin_unlock)))
    {
        priv->adapter->callbacks.moal_mfree(priv->adapter->pmoal_handle, (t_u8 *)sta_ptr);
    }
    LEAVE();
    return;
}



/**
 *   @brief This function processes the 802.11 mgmt Frame
 *
 *   @param priv      A pointer to mlan_private
 *   @param payload   A pointer to the received buffer
 *   @param payload_len Length of the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_process_802dot11_mgmt_pkt(IN mlan_private *priv, IN t_u8 *payload, IN t_u32 payload_len, RxPD *rxpd)
{
    mlan_status ret                   = MLAN_STATUS_SUCCESS;
    wlan_802_11_header *pieee_pkt_hdr = MNULL;
    t_u16 sub_type                    = 0;
    ENTER();
    /* Check packet type-subtype and compare with mgmt_passthru_mask If event
       is needed to host, just eventify it */
    pieee_pkt_hdr = (wlan_802_11_header *)payload;
    sub_type      = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(pieee_pkt_hdr->frm_ctl);
    if ((((1 << sub_type) & priv->mgmt_frame_passthru_mask) == 0) && (sub_type != SUBTYPE_ACTION))
    {
        PRINTM(MINFO, "Dropping mgmt frame for subtype %d.\n", sub_type);
        LEAVE();
        return ret;
    }
    switch (sub_type)
    {
        case SUBTYPE_ACTION:
            ret = wlan_process_mgmt_action(payload, payload_len, rxpd);
            if (ret == MLAN_STATUS_SUCCESS)
            {
                return ret;
            }
            break;
        case SUBTYPE_ASSOC_REQUEST:
        case SUBTYPE_REASSOC_REQUEST:
        case SUBTYPE_DISASSOC:
        case SUBTYPE_DEAUTH:
        case SUBTYPE_AUTH:
        case SUBTYPE_PROBE_RESP:
            break;
        default:
            PRINTM(MINFO, "Unexpected pkt subtype \n");
            break;
    }
    LEAVE();
    return ret;
}

mlan_status wlan_bypass_802dot11_mgmt_pkt(void *data)
{
    RxPD *rxpd                        = (RxPD *)data;
    wlan_mgmt_pkt *pmgmt_pkt_hdr      = NULL;
    t_u16 sub_type                    = 0;
    wlan_802_11_header *pieee_pkt_hdr = MNULL;
    t_u8 category                     = 0;
    mlan_private *priv                = mlan_adap->priv[0];
    mlan_status ret                   = MLAN_STATUS_SUCCESS;

    pmgmt_pkt_hdr = (wlan_mgmt_pkt *)((t_u8 *)rxpd + rxpd->rx_pkt_offset);
    pieee_pkt_hdr = (wlan_802_11_header *)&pmgmt_pkt_hdr->wlan_header;
    sub_type      = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(pieee_pkt_hdr->frm_ctl);
    // coverity[overrun-local:SUPPRESS]
    category = *((t_u8 *)pieee_pkt_hdr + sizeof(wlan_802_11_header));

    if ((pmgmt_pkt_hdr->wlan_header.frm_ctl & IEEE80211_FC_MGMT_FRAME_TYPE_MASK) == 0)
    {
        if ((((1 << sub_type) & priv->mgmt_frame_passthru_mask) == 0) && (sub_type != SUBTYPE_ACTION))
        {
            PRINTM(MINFO, "Dropping mgmt frame for subtype %d.\n", sub_type);
            LEAVE();
            return ret;
        }

        if (sub_type == SUBTYPE_ACTION)
        {
            if (category == IEEE_MGMT_ACTION_CATEGORY_BLOCK_ACK)
            {
                PRINTM(MINFO, "Dropping mgmt frame for category %d.\n", category);
                LEAVE();
                return ret;
            }
        }
    }

    ret = MLAN_STATUS_FAILURE;
    return ret;
}


/**
 *  @brief Add Extended Capabilities IE
 *
 *  @param pmpriv             A pointer to mlan_private structure
 *  @param pptlv_out          A pointer to TLV to fill in
 *  @param BSSDescriptor      A poiter to bss descriptor
 *  @return                   N/A
 */
void wlan_add_ext_capa_info_ie(IN mlan_private *pmpriv, IN BSSDescriptor_t *pbss_desc, OUT t_u8 **pptlv_out)
{
    MrvlIETypes_ExtCap_t *pext_cap = MNULL;

    ENTER();

    pext_cap = (MrvlIETypes_ExtCap_t *)(void *)*pptlv_out;
    (void)__memset(pmpriv->adapter, pext_cap, 0, sizeof(MrvlIETypes_ExtCap_t));
    pext_cap->header.type = wlan_cpu_to_le16(EXT_CAPABILITY);
    pext_cap->header.len  = wlan_cpu_to_le16(sizeof(ExtCap_t));
    if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_INTERWORKING_IND) != 0U)
    {
        pext_cap->ext_cap.Interworking = 1;
    }
    if ((((t_u8)(pmpriv->hotspot_cfg >> 8)) & HOTSPOT_ENABLE_TDLS_IND) != 0U)
    {
        pext_cap->ext_cap.TDLSSupport = 1;
    }

#if (CONFIG_WNM_PS)
    if ((((mlan_private *)mlan_adap->priv[0])->wnm_set == true) && (pbss_desc != MNULL) &&
        (pbss_desc->pext_cap->ext_cap.WNM_Sleep == true))
    {
        pext_cap->ext_cap.WNM_Sleep = 1;
    }
    else
    {
        pext_cap->ext_cap.WNM_Sleep = 0;
    }
#endif
#if CONFIG_11AX
#if CONFIG_MULTI_BSSID_SUPPORT
    if (pbss_desc && pbss_desc->multi_bssid_ap)
        SET_EXTCAP_MULTI_BSSID(pext_cap->ext_cap);
#endif
    if (wlan_check_11ax_twt_supported(pmpriv, pbss_desc))
        SET_EXTCAP_TWT_REQ(pext_cap->ext_cap);
#endif
#if CONFIG_11V
    pext_cap->ext_cap.BSS_Transition = 1;
#endif
#if CONFIG_11MC
    pext_cap->ext_cap.FTMI          = 1;
    pext_cap->ext_cap.CivicLocation = 1;
#endif

    *pptlv_out += sizeof(MrvlIETypes_ExtCap_t);

    LEAVE();
}


/**
 *  @brief Get rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_rate_ioctl_get_rate_index(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_GET, 0, (t_void *)pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Set rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_rate_ioctl_set_rate_index(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    t_s32 rate_index;
    mlan_rate_format rate_format;
#if (CONFIG_11AC) || (CONFIG_11AX)
    t_u32 nss;
#endif
    t_u32 i;
    mlan_ds_rate *ds_rate = MNULL;
    mlan_status ret       = MLAN_STATUS_FAILURE;
    mlan_private *pmpriv  = pmadapter->priv[pioctl_req->bss_index];
    t_u16 bitmap_rates[MAX_BITMAP_RATES_SIZE];
    t_u16 shift_index = 1U;

    ENTER();

    ds_rate = (mlan_ds_rate *)(void *)pioctl_req->pbuf;

    rate_format = ds_rate->param.rate_cfg.rate_format;
#if (CONFIG_11AC) || (CONFIG_11AX)
    nss = ds_rate->param.rate_cfg.nss;
#endif
    rate_index = (t_s32)ds_rate->param.rate_cfg.rate;

    if (ds_rate->param.rate_cfg.is_rate_auto == MTRUE)
    {
        (void)__memset(pmadapter, bitmap_rates, 0, sizeof(bitmap_rates));
        /* Rates talbe [0]: HR/DSSS;[1]: OFDM; [2..9] HT; */
        /* Support all HR/DSSS rates */
        bitmap_rates[0] = 0x000F;
        /* Support all OFDM rates */
        bitmap_rates[1] = 0x00FF;
        /* Support all HT-MCSs rate */
        for (i = 2; i < 9U; i++)
        {
            bitmap_rates[i] = 0xFFFF;
        }
        bitmap_rates[9] = 0x3FFF;
#if CONFIG_11AC
        /* [10..17] VHT */
#ifdef RW610
        /* RW610 only supports VHT MCS0 ~ MCS8*/
        bitmap_rates[10] = 0x01FF; /* 9 Bits valid */
        /* RW610 only supports 1 NSS*/
        bitmap_rates[11] = 0x0;
#else
        /* Support all VHT-MCSs rate for NSS 1 and 2 */
        for (i = 10; i < 12; i++)
        {
            bitmap_rates[i] = 0x03FF; /* 10 Bits valid */
        }
#endif
        /* Set to 0 as default value for all other NSSs */
        for (i = 12; i < NELEMENTS(bitmap_rates); i++)
        {
            bitmap_rates[i] = 0x0;
        }
#endif
#if CONFIG_11AX
        /* [18..25] HE */
#ifdef RW610
        /* RW610 only supports HE MCS0 ~ MCS9*/
        bitmap_rates[18] = 0x03FF; /* 10 Bits valid */
        /* RW610 only supports 1 NSS*/
        bitmap_rates[19] = 0x0;
#else
        /* Support all HE-MCSs rate for NSS1 and 2 */
        for (i = 18; i < 20; i++)
            bitmap_rates[i] = 0x0FFF;
#endif
        for (i = 20; i < NELEMENTS(bitmap_rates); i++)
            bitmap_rates[i] = 0x0;
#endif
    }
    else
    {
        PRINTM(MINFO, "Rate index is %d\n", rate_index);


        (void)__memset(pmadapter, bitmap_rates, 0, sizeof(bitmap_rates));
        if (rate_format == MLAN_RATE_FORMAT_LG)
        {
            /* Bitmap of HR/DSSS rates */
            if (rate_index <= MLAN_RATE_INDEX_HRDSSS3)
            {
                bitmap_rates[0] = (shift_index << rate_index);
                ret             = MLAN_STATUS_SUCCESS;
                /* Bitmap of OFDM rates */
            }
            else if (rate_index <= MLAN_RATE_INDEX_OFDM7)
            {
                bitmap_rates[1] = (shift_index << (rate_index - MLAN_RATE_INDEX_OFDM0));
                ret             = MLAN_STATUS_SUCCESS;
            }
            else
            {
                /*Do Nothing*/
            }
        }
        else if (rate_format == MLAN_RATE_FORMAT_HT)
        {
            if (rate_index <= MLAN_RATE_INDEX_MCS32)
            {
#ifdef SD8801
                rate_index -= MLAN_RATE_INDEX_MCS0;
#endif
                bitmap_rates[2 + (rate_index / 16)] = (shift_index << (rate_index % 16));
                ret                                 = MLAN_STATUS_SUCCESS;
            }
        }
        else
        {
            /*DO Nothing*/
        }

#if CONFIG_11AC
        if (rate_format == MLAN_RATE_FORMAT_VHT)
        {
            if ((rate_index <= MLAN_RATE_INDEX_MCS9) && (MLAN_RATE_NSS1 <= nss) && (nss <= MLAN_RATE_NSS2))
            {
                bitmap_rates[10 + nss - MLAN_RATE_NSS1] = (shift_index << rate_index);
                ret                                     = MLAN_STATUS_SUCCESS;
            }
        }
#endif
#if CONFIG_11AX
        if (rate_format == MLAN_RATE_FORMAT_HE)
        {
            if (IS_FW_SUPPORT_11AX(pmadapter))
            {
                if ((rate_index <= MLAN_RATE_INDEX_MCS11) && (MLAN_RATE_NSS1 <= nss) && (nss <= MLAN_RATE_NSS2))
                {
                    bitmap_rates[18 + nss - MLAN_RATE_NSS1] = (1 << rate_index);
                    ret                                     = MLAN_STATUS_SUCCESS;
                }
            }
            else
            {
                PRINTM(MERROR, "Error! Fw doesn't support 11AX\n");
                LEAVE();
                return MLAN_STATUS_FAILURE;
            }
        }
#endif
        if (ret == MLAN_STATUS_FAILURE)
        {
            PRINTM(MERROR, "Invalid MCS index=%d. \n", rate_index);
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
    }

    PRINTM(MINFO,
           "RateBitmap=%04x%04x%04x%04x%04x%04x%04x%04x"
           "%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x, "
           "IsRateAuto=%d, DataRate=%d\n",
           bitmap_rates[17], bitmap_rates[16], bitmap_rates[15], bitmap_rates[14], bitmap_rates[13], bitmap_rates[12],
           bitmap_rates[11], bitmap_rates[10], bitmap_rates[9], bitmap_rates[8], bitmap_rates[7], bitmap_rates[6],
           bitmap_rates[5], bitmap_rates[4], bitmap_rates[3], bitmap_rates[2], bitmap_rates[1], bitmap_rates[0],
           pmpriv->is_data_rate_auto, pmpriv->data_rate);

    /* Send request to firmware */
#if CONFIG_AUTO_NULL_TX
    if (ds_rate->auto_null_fixrate_enable == 1)
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_SPC_AUTO_SET, 0, (t_void *)pioctl_req,
                               bitmap_rates);
        ds_rate->auto_null_fixrate_enable = 0xff;
    }
    else if (ds_rate->auto_null_fixrate_enable == 0)
    {
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_SPC_AUTO_NOSET, 0, (t_void *)pioctl_req,
                               bitmap_rates);
        ds_rate->auto_null_fixrate_enable = 0xff;
    }
    else
#endif
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TX_RATE_CFG, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req,
                               (t_void *)bitmap_rates);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Rate configuration command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_rate_ioctl_cfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;

    ENTER();

        if (pioctl_req->action == MLAN_ACT_GET)
        {
            status = wlan_rate_ioctl_get_rate_index(pmadapter, pioctl_req);
        }
        else
        {
            status = wlan_rate_ioctl_set_rate_index(pmadapter, pioctl_req);
        }

    LEAVE();
    return status;
}

/**
 *  @brief This function prepares command of rf_antenna.
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param cmd      A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_802_11_rf_antenna(IN pmlan_private pmpriv,
                                       IN HostCmd_DS_COMMAND *cmd,
                                       IN t_u16 cmd_action,
                                       IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_RF_ANTENNA *pantenna = &cmd->params.antenna;
    mlan_ds_ant_cfg_1x1 *ant_cfg_1x1 = (mlan_ds_ant_cfg_1x1 *)pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_RF_ANTENNA);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_RF_ANTENNA) + S_DS_GEN);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        pantenna->action        = wlan_cpu_to_le16(HostCmd_ACT_SET_BOTH);
        pantenna->antenna_mode  = wlan_cpu_to_le16((t_u16)ant_cfg_1x1->antenna);
        pantenna->evaluate_time = wlan_cpu_to_le16((t_u16)ant_cfg_1x1->evaluate_time);
#ifdef RW610
        pantenna->evaluate_mode = wlan_cpu_to_le16((t_u8)ant_cfg_1x1->evaluate_mode);
#endif
    }
    else
    {
        pantenna->action        = wlan_cpu_to_le16(HostCmd_ACT_GET_BOTH);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_NET_MONITOR
mlan_status wlan_cmd_802_11_net_monitor(IN pmlan_private pmpriv,
                                        IN HostCmd_DS_COMMAND *cmd,
                                        IN t_u16 cmd_action,
                                        IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_NET_MONITOR *net_mon = &cmd->params.net_mon;
    wifi_net_monitor_t *monitor            = (wifi_net_monitor_t *)pdata_buf;
    ENTER();

    (void)__memset(pmpriv->adapter, net_mon, 0x00, sizeof(HostCmd_DS_802_11_NET_MONITOR));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_NET_MONITOR);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_NET_MONITOR) + S_DS_GEN);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        net_mon->action                                         = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
        net_mon->monitor_activity                               = wlan_cpu_to_le16(monitor->monitor_activity);
        net_mon->filter_flags                                   = wlan_cpu_to_le16(monitor->filter_flags);
        net_mon->monitor_channel.header.type                    = TLV_TYPE_UAP_CHAN_BAND_CONFIG;
        net_mon->monitor_channel.header.len                     = 2;
        net_mon->monitor_channel.chan_band_param[0].radio_type  = (t_u8)monitor->radio_type;
        net_mon->monitor_channel.chan_band_param[0].chan_number = (t_u8)monitor->chan_number;

        net_mon->monitor_filter.header.type = TLV_TYPE_UAP_STA_MAC_ADDR_FILTER;
        net_mon->monitor_filter.header.len  = MLAN_MAC_ADDR_LENGTH * monitor->filter_num + sizeof(t_u8);
        net_mon->monitor_filter.filter_num  = (t_u8)monitor->filter_num;
        __memcpy(priv->adapter, (t_u8 *)net_mon->monitor_filter.mac_list, (t_u8 *)monitor->mac_addr,
                 MAX_MONIT_MAC_FILTER_NUM * MLAN_MAC_ADDR_LENGTH);
    }
    else
    {
        net_mon->action = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif


#ifdef WLAN_LOW_POWER_ENABLE
/**
 *  @brief Set/Get Low Power Mode
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_misc_ioctl_low_pwr_mode(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret        = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc = MNULL;
    mlan_private *pmpriv   = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc = (mlan_ds_misc_cfg *)pioctl_req->pbuf;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_LOW_POWER_MODE, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req,
                           &misc->param.low_pwr_mode);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}
#endif // WLAN_LOW_POWER_ENABLE

#if CONFIG_WIFI_CLOCKSYNC
/**
 *  @brief Set/Get GPIO TSF Latch config
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_misc_gpio_tsf_latch_config(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action           = 0;
    mlan_private *pmpriv       = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc_cfg = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG, cmd_action, 0, (t_void *)pioctl_req,
                           &misc_cfg->param.gpio_tsf_latch_config);

    LEAVE();
    return ret;
}

/**
 *  @brief Get TSF info
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_misc_get_tsf_info(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action           = 0;
    mlan_private *pmpriv       = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    misc_cfg   = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG, cmd_action, 0, (t_void *)pioctl_req,
                           &misc_cfg->param.tsf_info);

    LEAVE();
    return ret;
}
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_ECSA
/**
 *  @brief Get non-global operating class
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   Pointer to the IOCTL request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_misc_ioctl_oper_class(pmlan_adapter pmadapter, mlan_ioctl_req *pioctl_req)
{
    pmlan_private pmpriv   = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_misc_cfg *misc = MNULL;
    t_u8 channel, bandwidth, oper_class = 0;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    misc    = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    channel = misc->param.bw_chan_oper.channel;
    switch (misc->param.bw_chan_oper.bandwidth)
    {
        case 20:
            bandwidth = BW_20MHZ;
            break;
        case 40:
            bandwidth = BW_40MHZ;
            break;
        case 80:
            bandwidth = BW_80MHZ;
            break;
        default:
            bandwidth = BW_20MHZ;
            break;
    }

    if (pioctl_req->action == MLAN_ACT_GET)
    {
        ret                                 = wlan_get_curr_oper_class(pmpriv, channel, bandwidth, &oper_class);
        misc->param.bw_chan_oper.oper_class = oper_class;
    }
    else
    {
        PRINTM(MERROR, "Unsupported cmd_action\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Check operating class validation
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   Pointer to the IOCTL request buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_misc_ioctl_operclass_validation(pmlan_adapter pmadapter, mlan_ioctl_req *pioctl_req)
{
    pmlan_private pmpriv   = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_misc_cfg *misc = MNULL;
    t_u8 channel, oper_class;
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    misc       = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    channel    = misc->param.bw_chan_oper.channel;
    oper_class = misc->param.bw_chan_oper.oper_class;
    if (pioctl_req->action == MLAN_ACT_GET)
    {
        ret = wlan_check_operclass_validation(pmpriv, channel, oper_class);
    }
    else
    {
        PRINTM(MERROR, "Unsupported cmd_action\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return ret;
}
#endif

#if CONFIG_RF_TEST_MODE
/**
 *  @brief RF Test Mode config
 *
 *  @param pmadapter   A pointer to mlan_adapter structure
 *  @param pioctl_req  A pointer to ioctl request buffer
 *
 *  @return        MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_misc_ioctl_rf_test_cfg(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv    = MNULL;
    mlan_ds_misc_cfg *pmisc = MNULL;
    mlan_status ret         = MLAN_STATUS_FAILURE;
    t_u16 cmd_action        = 0;

    ENTER();

    if (!pioctl_req)
        goto done;

    pmpriv = pmadapter->priv[pioctl_req->bss_index];
    pmisc  = (mlan_ds_misc_cfg *)pioctl_req->pbuf;

    switch (pmisc->sub_command)
    {
        case MLAN_OID_MISC_RF_TEST_GENERIC:
            if (pioctl_req->action == MLAN_ACT_SET)
                cmd_action = HostCmd_ACT_GEN_SET;
            else
                cmd_action = HostCmd_ACT_GEN_GET;
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_MFG_COMMAND, cmd_action, 0, (t_void *)pioctl_req,
                                   &(pmisc->param.mfg_generic_cfg));
            break;
        case MLAN_OID_MISC_RF_TEST_TX_CONT:
            if (pioctl_req->action == MLAN_ACT_SET)
                cmd_action = HostCmd_ACT_GEN_SET;
            else
            {
                PRINTM(MERROR, "Unsupported cmd_action\n");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_MFG_COMMAND, cmd_action, 0, (t_void *)pioctl_req,
                                   &(pmisc->param.mfg_tx_cont));
            break;
        case MLAN_OID_MISC_RF_TEST_TX_FRAME:
            if (pioctl_req->action == MLAN_ACT_SET)
                cmd_action = HostCmd_ACT_GEN_SET;
            else
            {
                PRINTM(MERROR, "Unsupported cmd_action\n");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_MFG_COMMAND, cmd_action, 0, (t_void *)pioctl_req,
                                   &(pmisc->param.mfg_tx_frame2));
            break;
        case MLAN_OID_MISC_RF_TEST_CONFIG_TRIGGER_FRAME:
            if (pioctl_req->action == MLAN_ACT_SET)
                cmd_action = HostCmd_ACT_GEN_SET;
            else
            {
                PRINTM(MERROR, "Unsupported cmd_action\n");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_MFG_COMMAND, cmd_action, 0, (t_void *)pioctl_req,
                                   &(pmisc->param.mfg_tx_trigger_config));
            break;

        case MLAN_OID_MISC_RF_TEST_HE_POWER:
            if (pioctl_req->action == MLAN_ACT_SET)
                cmd_action = HostCmd_ACT_GEN_SET;
            else
            {
                PRINTM(MERROR, "Unsupported cmd_action\n");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_MFG_COMMAND, cmd_action, 0, (t_void *)pioctl_req,
                                   &(pmisc->param.mfg_he_power));
            break;
    }

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;
done:
    LEAVE();
    return ret;
}
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
/**
 *  @brief Configure GPIO independent reset
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return             MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_misc_ioctl_ind_rst_cfg(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv   = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_misc_cfg *misc = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    mlan_status ret        = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action       = 0;

    ENTER();

    if (pioctl_req->action == MLAN_ACT_GET)
        cmd_action = HostCmd_ACT_GEN_GET;
    else
        cmd_action = HostCmd_ACT_GEN_SET;

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_INDEPENDENT_RESET_CFG, cmd_action, 0, (t_void *)pioctl_req,
                           (t_void *)&misc->param.ind_rst_cfg);

    LEAVE();
    return ret;
}
#endif
