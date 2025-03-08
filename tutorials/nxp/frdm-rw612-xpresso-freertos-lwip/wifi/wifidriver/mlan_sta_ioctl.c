/** @file mlan_sta_ioctl.c
 *
 *  @brief  This file provides functions for station ioctl
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    10/21/2008: initial version
******************************************************/

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

mlan_status wlan_misc_ioctl_region(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req);
t_u8 wlan_get_random_charactor(pmlan_adapter pmadapter);

#if (CONFIG_WIFI_RTS_THRESHOLD) || (CONFIG_WIFI_FRAG_THRESHOLD)
/**
 *  @brief Set/Get SNMP MIB handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_snmp_mib_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv  = pmadapter->priv[pioctl_req->bss_index];
    mlan_status ret       = MLAN_STATUS_SUCCESS;
    t_u16 cmd_action      = 0;
    t_u16 cmd_oid         = 0;
    mlan_ds_snmp_mib *mib = MNULL;
    t_u32 value           = 0;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_snmp_mib))
    {
        PRINTM(MWARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_snmp_mib);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        ret                           = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    mib = (mlan_ds_snmp_mib *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    switch (mib->sub_command)
    {
        case MLAN_OID_SNMP_MIB_RTS_THRESHOLD:
            value   = mib->param.rts_threshold;
            cmd_oid = RtsThresh_i;
            break;
        case MLAN_OID_SNMP_MIB_FRAG_THRESHOLD:
            value   = mib->param.frag_threshold;
            cmd_oid = FragThresh_i;
            break;
        case MLAN_OID_SNMP_MIB_RETRY_COUNT:
            value   = mib->param.retry_count;
            cmd_oid = ShortRetryLim_i;
            break;
        case MLAN_OID_SNMP_MIB_DTIM_PERIOD:
            value   = mib->param.dtim_period;
            cmd_oid = DtimPeriod_i;
            break;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SNMP_MIB, cmd_action, cmd_oid, (t_void *)pioctl_req, &value);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

exit:
    LEAVE();
    return ret;
}
#endif

/**
 *  @brief Set/Get Infra/Ad-hoc band configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_radio_ioctl_band_cfg(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    t_u8 i = 0;
    t_u16 global_band = 0;
    t_u16 infra_band              = 0;
    mlan_ds_radio_cfg *radio_cfg = MNULL;
    mlan_private *pmpriv         = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    radio_cfg = (mlan_ds_radio_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        infra_band    = radio_cfg->param.band_cfg.config_bands;

        /* SET Infra band */
        if ((infra_band | pmadapter->fw_bands) & ~pmadapter->fw_bands)
        {
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }


        for (i = 0; i < pmadapter->priv_num; i++)
        {
            if (pmadapter->priv[i] && pmadapter->priv[i] != pmpriv &&
                GET_BSS_ROLE(pmadapter->priv[i]) == MLAN_BSS_ROLE_STA)
                global_band |= pmadapter->priv[i]->config_bands;
        }
        global_band |= infra_band;

        if (wlan_set_regiontable(pmpriv, (t_u8)pmadapter->region_code, global_band
        ))
        {
            pioctl_req->status_code = MLAN_ERROR_IOCTL_FAIL;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }

        if (wlan_11d_set_universaltable(pmpriv, global_band
        ))
        {
            pioctl_req->status_code = MLAN_ERROR_IOCTL_FAIL;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmpriv->config_bands    = infra_band;
        pmadapter->config_bands = global_band;

    }
    else
    {
        radio_cfg->param.band_cfg.config_bands = pmpriv->config_bands;            /* Infra
                                                                                     Bands
                                                                                   */
        radio_cfg->param.band_cfg.adhoc_start_band = pmadapter->adhoc_start_band; /* Adhoc
                                                                                     Band
                                                                                   */
        radio_cfg->param.band_cfg.adhoc_channel = pmpriv->adhoc_channel;          /* Adhoc
                                                                                     Channel
                                                                                   */
        radio_cfg->param.band_cfg.fw_bands = pmadapter->fw_bands;                 /* FW
                                                                                     support
                                                                                     Bands
                                                                                   */
        PRINTM(MINFO, "Global config band = %d\n", pmadapter->config_bands);
        radio_cfg->param.band_cfg.sec_chan_offset = pmadapter->chan_bandwidth;    /* adhoc
                                                                                     channel
                                                                                     bandwidth
                                                                                   */
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Radio command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_radio_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status           = MLAN_STATUS_SUCCESS;
    mlan_ds_radio_cfg *radio_cfg = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_radio_cfg))
    {
        PRINTM(MWARN, "MLAN IOCTL information buffer length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_radio_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    radio_cfg = (mlan_ds_radio_cfg *)pioctl_req->pbuf;
    switch (radio_cfg->sub_command)
    {
        case MLAN_OID_BAND_CFG:
            status = wlan_radio_ioctl_band_cfg(pmadapter, pioctl_req);
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
 *  @brief Start BSS
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_bss_ioctl_start(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_bss *bss     = (mlan_ds_bss *)(void *)pioctl_req->pbuf;
    t_s32 i              = -1;
    t_u8 zero_mac[]      = {0, 0, 0, 0, 0, 0};

    ENTER();

    /* Before ASSOC REQ, If "port ctrl" mode is enabled, move the port to
       CLOSED state */
    if (pmpriv->port_ctrl_mode == MTRUE)
    {
        PRINTM(MINFO, "bss_ioctl_start(): port_state=CLOSED\n");
        pmpriv->prior_port_status = pmpriv->port_open;
        pmpriv->port_open         = MFALSE;
    }
    pmpriv->scan_block = MFALSE;

    if (pmpriv->bss_mode == MLAN_BSS_MODE_INFRA)
    {
        if (!bss->param.ssid_bssid.idx || bss->param.ssid_bssid.idx > pmadapter->num_in_scan_table)
        {
            /* Search for the requested SSID in the scan table */
            if (bss->param.ssid_bssid.ssid.ssid_len != 0U)
            {
                if (__memcmp(pmadapter, &bss->param.ssid_bssid.bssid, zero_mac, sizeof(zero_mac)) != 0U)
                {
                    i = wlan_find_ssid_in_list(pmpriv, &bss->param.ssid_bssid.ssid,
                                               (t_u8 *)&bss->param.ssid_bssid.bssid, MLAN_BSS_MODE_INFRA);
                }
                else
                {
                    i = wlan_find_ssid_in_list(pmpriv, &bss->param.ssid_bssid.ssid, MNULL, MLAN_BSS_MODE_INFRA);
                }
            }
            else
            {
                i = wlan_find_bssid_in_list(pmpriv, (t_u8 *)&bss->param.ssid_bssid.bssid, MLAN_BSS_MODE_INFRA);
            }
        }
        else
        {
            /* fixme: Disabled for now since handling is done in legacy
             * code. It is IMPORTANT and needs to enabled.
             */
            i = ((t_s32)bss->param.ssid_bssid.idx - 1);
        }
        if (i >= 0)
        {
            PRINTM(MINFO, "SSID found in scan list ... associating...\n");

            /* Clear any past association response stored for application
               retrieval */
            /* fixme: enable if needed later */
            /* pmpriv->assoc_rsp_size = 0; */
            ret = wlan_associate(pmpriv, pioctl_req, &pmadapter->pscan_table[i]);
            if (ret != MLAN_STATUS_SUCCESS)
            {
                goto start_ssid_done;
            }
        }
        else
        {   /* i >= 0 */
            /* fixme: The below seems related to Adhoc. Check later. */
        }
    }
    else
    {
    }

        ret = MLAN_STATUS_PENDING;

start_ssid_done:
    LEAVE();
    return ret;
}


/**
 *  @brief BSS command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_bss_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_bss *bss   = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_bss))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_bss);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }

    bss = (mlan_ds_bss *)(void *)pioctl_req->pbuf;

    switch (bss->sub_command)
    {
        case MLAN_OID_BSS_START:
            status = wlan_bss_ioctl_start(pmadapter, pioctl_req);
            break;
        default:
            status = MLAN_STATUS_FAILURE;
            break;
    }

    LEAVE();
    return status;
}


/**
 *  @brief Rate command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_rate_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_rate *rate = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_rate))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_rate);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    rate = (mlan_ds_rate *)(void *)pioctl_req->pbuf;
    switch (rate->sub_command)
    {
        case MLAN_OID_RATE_CFG:
            status = wlan_rate_ioctl_cfg(pmadapter, pioctl_req);
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
 *  @brief Get Tx power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param cmd_no	Firmware command number used to retrieve power values
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_power_ioctl_get_power(IN pmlan_adapter pmadapter,
                                              IN t_u16 cmd_no,
                                              IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, cmd_no, HostCmd_ACT_GEN_GET, 0, (t_void *)pioctl_req, MNULL);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Set Tx power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_power_ioctl_set_power(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_ds_power_cfg *power = MNULL;
    mlan_status ret          = MLAN_STATUS_SUCCESS;

    HostCmd_DS_TXPWR_CFG *txp_cfg   = MNULL;
    MrvlTypes_Power_Group_t *pg_tlv = MNULL;
    Power_Group_t *pg               = MNULL;
    pmlan_callbacks pcb             = &pmadapter->callbacks;
    t_u8 *buf                       = MNULL;
    t_u16 dbm                       = 0;
    mlan_private *pmpriv            = pmadapter->priv[pioctl_req->bss_index];

    ENTER();

    power = (mlan_ds_power_cfg *)(void *)pioctl_req->pbuf;
    if (!power->param.power_cfg.is_power_auto)
    {
        dbm = (t_u16)power->param.power_cfg.power_level;
#if 0
        if ((dbm < pmpriv->min_tx_power_level) ||
            (dbm > pmpriv->max_tx_power_level)) {
            PRINTM(MERROR,
                   "The set txpower value %d dBm is out of range (%d dBm-%d dBm)!\n",
                   dbm, pmpriv->min_tx_power_level, pmpriv->max_tx_power_level);
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            ret = MLAN_STATUS_FAILURE;
            goto exit;
        }
#endif
    }

    ret = pcb->moal_malloc(pmadapter->pmoal_handle, MRVDRV_SIZE_OF_CMD_BUFFER, MLAN_MEM_DEF, &buf);
    if (ret != MLAN_STATUS_SUCCESS || (buf == MNULL))
    {
        PRINTM(MERROR, "ALLOC_CMD_BUF: Failed to allocate command buffer\n");
        pioctl_req->status_code = MLAN_ERROR_NO_MEM;
        ret                     = MLAN_STATUS_FAILURE;
        goto exit;
    }
    (void)__memset(pmadapter, buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    txp_cfg         = (HostCmd_DS_TXPWR_CFG *)(void *)buf;
    txp_cfg->action = HostCmd_ACT_GEN_SET;
    if (!power->param.power_cfg.is_power_auto)
    {
        txp_cfg->mode  = 1;
        pg_tlv         = (MrvlTypes_Power_Group_t *)(void *)(buf + sizeof(HostCmd_DS_TXPWR_CFG));
        pg_tlv->type   = TLV_TYPE_POWER_GROUP;
        pg_tlv->length = 4U * sizeof(Power_Group_t);
        pg = (Power_Group_t *)(void *)(buf + sizeof(HostCmd_DS_TXPWR_CFG) + sizeof(MrvlTypes_Power_Group_t));
        /* Power group for modulation class HR/DSSS */
        pg->first_rate_code  = 0x00;
        pg->last_rate_code   = 0x03;
        pg->modulation_class = MOD_CLASS_HR_DSSS;
        pg->power_step       = (t_s8)0;
        pg->power_min        = (t_s8)dbm;
        pg->power_max        = (t_s8)dbm;
        pg++;
        /* Power group for modulation class OFDM */
        pg->first_rate_code  = 0x00;
        pg->last_rate_code   = 0x07;
        pg->modulation_class = MOD_CLASS_OFDM;
        pg->power_step       = (t_s8)0;
        pg->power_min        = (t_s8)dbm;
        pg->power_max        = (t_s8)dbm;
        pg++;
        /* Power group for modulation class HTBW20 */
        pg->first_rate_code  = 0x00;
        pg->last_rate_code   = 0x20;
        pg->modulation_class = MOD_CLASS_HT;
        pg->power_step       = (t_s8)0;
        pg->power_min        = (t_s8)dbm;
        pg->power_max        = (t_s8)dbm;
        pg->ht_bandwidth     = HT_BW_20;
        pg++;
        /* Power group for modulation class HTBW40 */
        pg->first_rate_code  = 0x00;
        pg->last_rate_code   = 0x20;
        pg->modulation_class = MOD_CLASS_HT;
        pg->power_step       = (t_s8)0;
        pg->power_min        = (t_s8)dbm;
        pg->power_max        = (t_s8)dbm;
        pg->ht_bandwidth     = HT_BW_40;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TXPWR_CFG, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req, buf);

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    (void)pcb->moal_mfree(pmadapter->pmoal_handle, buf);

exit:
    LEAVE();
    return ret;
}

/**
 *  @brief Get modulation class from rate index
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param rate_index	Rate index
 *
 *  @return		0 fail, otherwise return modulation class
 */
static int wlan_get_modulation_class(pmlan_adapter pmadapter, int rate_index)
{
    ENTER();
    if (rate_index >= MLAN_RATE_INDEX_HRDSSS0 && rate_index <= MLAN_RATE_INDEX_HRDSSS3)
    {
        LEAVE();
        return (int)MOD_CLASS_HR_DSSS;
    }
    else if (rate_index >= MLAN_RATE_INDEX_OFDM0 && rate_index <= MLAN_RATE_INDEX_OFDM7)
    {
        LEAVE();
        return (int)MOD_CLASS_OFDM;
    }
    else if (rate_index <= MLAN_RATE_INDEX_MCS127)
    {
        LEAVE();
        return (int)MOD_CLASS_HT;
    }
    else
    { /* Do Nothing */
    }
    PRINTM(MERROR, "Invalid rate index = %d supplied!\n", rate_index);

    LEAVE();
    return 0;
}

/**
 *  @brief Set extended power configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_power_ioctl_set_power_ext(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret                 = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv            = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_power_cfg *power        = MNULL;
    pmlan_callbacks pcb             = &pmadapter->callbacks;
    t_u8 *buf                       = MNULL;
    HostCmd_DS_TXPWR_CFG *txp_cfg   = MNULL;
    MrvlTypes_Power_Group_t *pg_tlv = MNULL;
    Power_Group_t *pg               = MNULL;
    int mod_class;
    t_u32 data[4] = {0};
    t_u8 ht_bw;

    ENTER();

    power = (mlan_ds_power_cfg *)(void *)pioctl_req->pbuf;
    ret   = pcb->moal_malloc(pmadapter->pmoal_handle, MRVDRV_SIZE_OF_CMD_BUFFER, MLAN_MEM_DEF, &buf);
    if (ret != MLAN_STATUS_SUCCESS || (buf == MNULL))
    {
        PRINTM(MERROR, "ALLOC_CMD_BUF: Failed to allocate command buffer\n");
        pioctl_req->status_code = MLAN_ERROR_NO_MEM;
        ret                     = MLAN_STATUS_FAILURE;
        goto exit;
    }
    (void)__memset(pmadapter, buf, 0, MRVDRV_SIZE_OF_CMD_BUFFER);
    txp_cfg         = (HostCmd_DS_TXPWR_CFG *)(void *)buf;
    txp_cfg->action = HostCmd_ACT_GEN_SET;
    (void)__memcpy(pmadapter, (t_u8 *)&data, (t_u8 *)power->param.power_ext.power_data, sizeof(data));
    switch (power->param.power_ext.len)
    {
        case 1:
            if (data[0] == 0xFFU)
            {
                txp_cfg->mode = 0;
            }
            else
            {
                pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
                ret                     = MLAN_STATUS_FAILURE;
            }
            break;
        case 2:
        case 4:
            ht_bw = (data[0] & TX_RATE_HT_BW40_BIT) ? HT_BW_40 : HT_BW_20;
            data[0] &= ~TX_RATE_HT_BW40_BIT;
            if (!(mod_class = wlan_get_modulation_class(pmadapter, (int)data[0])))
            {
                pioctl_req->status_code = MLAN_ERROR_CMD_RESP_FAIL;
                ret                     = MLAN_STATUS_FAILURE;
                break;
            }
            if (ht_bw && mod_class != MOD_CLASS_HT)
            {
                pioctl_req->status_code = MLAN_ERROR_CMD_RESP_FAIL;
                ret                     = MLAN_STATUS_FAILURE;
                break;
            }
            txp_cfg->mode  = 1;
            pg_tlv         = (MrvlTypes_Power_Group_t *)(void *)(buf + sizeof(HostCmd_DS_TXPWR_CFG));
            pg_tlv->type   = TLV_TYPE_POWER_GROUP;
            pg_tlv->length = (t_u16)sizeof(Power_Group_t);
            pg = (Power_Group_t *)(void *)(buf + sizeof(HostCmd_DS_TXPWR_CFG) + sizeof(MrvlTypes_Power_Group_t));
            pg->modulation_class = (t_u8)mod_class;
            pg->first_rate_code  = (t_u8)data[0];
            pg->last_rate_code   = (t_u8)data[0];
            if (mod_class == MOD_CLASS_OFDM)
            {
                pg->first_rate_code = (t_u8)(data[0] - MLAN_RATE_INDEX_OFDM0);
                pg->last_rate_code  = (t_u8)(data[0] - MLAN_RATE_INDEX_OFDM0);
            }
            else if (mod_class == MOD_CLASS_HT)
            {
                pg->first_rate_code = (t_u8)(data[0] - MLAN_RATE_INDEX_MCS0);
                pg->last_rate_code  = (t_u8)(data[0] - MLAN_RATE_INDEX_MCS0);
                pg->ht_bandwidth    = ht_bw;
            }
            else
            {
                /* Do Nothing */
            }
            pg->power_min = (t_s8)data[1];
            pg->power_max = (t_s8)data[1];
            if (power->param.power_ext.len == 4U)
            {
                pg->power_max  = (t_s8)data[2];
                pg->power_step = (t_s8)data[3];
            }
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            ret                     = MLAN_STATUS_FAILURE;
            break;
    }
    if (ret == MLAN_STATUS_FAILURE)
    {
        (void)pcb->moal_mfree(pmadapter->pmoal_handle, buf);
        goto exit;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_TXPWR_CFG, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req, buf);
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }
    (void)pcb->moal_mfree(pmadapter->pmoal_handle, buf);

exit:
    LEAVE();
    return ret;
}

/**
 *  @brief Power configuration command handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_power_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status       = MLAN_STATUS_SUCCESS;
    mlan_ds_power_cfg *power = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_power_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_power_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    power = (mlan_ds_power_cfg *)(void *)pioctl_req->pbuf;
    switch (power->sub_command)
    {
        case MLAN_OID_POWER_CFG:
            if (pioctl_req->action == MLAN_ACT_GET)
            {
                status = wlan_power_ioctl_get_power(pmadapter, HostCmd_CMD_TXPWR_CFG, pioctl_req);
            }
            else
            {
                status = wlan_power_ioctl_set_power(pmadapter, pioctl_req);
            }
            break;
        case MLAN_OID_POWER_CFG_EXT:
            if (pioctl_req->action == MLAN_ACT_GET)
            {
                status = wlan_power_ioctl_get_power(pmadapter, HostCmd_CMD_TXPWR_CFG, pioctl_req);
            }
            else
            {
                status = wlan_power_ioctl_set_power_ext(pmadapter, pioctl_req);
            }
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}

#if CONFIG_WMM_UAPSD
/**
 *  @brief Set/Get WMM QoS configuration
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success
 */
static mlan_status wlan_wmm_ioctl_qos(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_wmm_cfg *wmm = MNULL;

    ENTER();

    wmm = (mlan_ds_wmm_cfg *)pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_GET)
        wmm->param.qos_cfg = pmpriv->wmm_qosinfo;
    else
    {
        pmpriv->wmm_qosinfo = wmm->param.qos_cfg;
    }

    pioctl_req->data_read_written = sizeof(t_u8) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}
#endif
#if CONFIG_WMM_UAPSD
#endif
#if CONFIG_WMM_UAPSD
/**
 *  @brief WMM configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_wmm_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status   = MLAN_STATUS_SUCCESS;
    mlan_ds_wmm_cfg *wmm = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_wmm_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_wmm_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    wmm = (mlan_ds_wmm_cfg *)pioctl_req->pbuf;
    switch (wmm->sub_command)
    {
        case MLAN_OID_WMM_CFG_QOS:
            status = wlan_wmm_ioctl_qos(pmadapter, pioctl_req);
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}
#endif

/**
 *  @brief Get Random charactor
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		random charactor
 */
t_u8 wlan_get_random_charactor(pmlan_adapter pmadapter)
{
    t_u32 sec, usec;
    t_u32 ch_32 = 0;
    t_u8 ch     = 0;

    ENTER();

    sec  = 10; // wmtime_time_get_posix();
    usec = 0;

    sec   = (sec & 0xFFFFU) + (sec >> 16);
    usec  = (usec & 0xFFFFU) + (usec >> 16);
    ch_32 = (((sec << 16) + usec) % 26U);
    ch    = (t_u8)ch_32 + (t_u8)'a';

    LEAVE();
    return ch;
}


#ifdef WPA
/**
 *  @brief Set WEP keys
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */

/**
 *  @brief Set WPA key
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_sec_ioctl_set_wpa_key(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_sec_cfg *sec = MNULL;
#ifndef KEY_PARAM_SET_V2
    t_u8 broadcast_mac_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    t_u8 remove_key           = MFALSE;
#endif /* KEY_PARAM_SET_V2 */

    ENTER();

    sec = (mlan_ds_sec_cfg *)(void *)pioctl_req->pbuf;
    /* Current driver only supports key length of up to 32 bytes */
    if (sec->param.encrypt_key.key_len > MLAN_MAX_KEY_LENGTH)
    {
        PRINTM(MERROR, "Key length is incorrect\n");
        pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
        ret                     = MLAN_STATUS_FAILURE;
        goto exit;
    }

#ifndef KEY_PARAM_SET_V2
#ifdef ENABLE_WPA_NONE
    if ((pmpriv->bss_mode == MLAN_BSS_MODE_IBSS) && pmpriv->sec_info.wpa_enabled)
    {
        /*
         * IBSS/WPA-None uses only one key (Group) for both receiving and
         *  sending unicast and multicast packets.
         */
        /* Send the key as PTK to firmware */
        sec->param.encrypt_key.key_index = MLAN_KEY_INDEX_UNICAST;
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL, HostCmd_ACT_GEN_SET, KEY_INFO_ENABLED, MNULL,
                               &sec->param.encrypt_key);
        if (ret)
            goto exit;

        /* Send the key as GTK to firmware */
        sec->param.encrypt_key.key_index = ~MLAN_KEY_INDEX_UNICAST;
    }
#endif /* ENABLE_WPA_NONE */
#endif /* KEY_PARAM_SET_V2 */



#ifdef KEY_PARAM_SET_V2
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL, HostCmd_ACT_GEN_SET, 0, (t_void *)pioctl_req,
                           &sec->param.encrypt_key);
#else
    if (__memcmp(pmadapter, sec->param.encrypt_key.mac_addr, broadcast_mac_addr, MLAN_MAC_ADDR_LENGTH))
    {
        sec->param.encrypt_key.key_index |= MLAN_KEY_INDEX_UNICAST;
    }

    if (remove_key == MTRUE)
    {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL, HostCmd_ACT_GEN_SET, !(KEY_INFO_ENABLED),
                               (t_void *)pioctl_req, &sec->param.encrypt_key);
    }
    else
    {
        /* Send request to firmware */
        ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_KEY_MATERIAL, HostCmd_ACT_GEN_SET, KEY_INFO_ENABLED,
                               (t_void *)pioctl_req, &sec->param.encrypt_key);
    }
#endif /* KEY_PARAM_SET_V2 */

    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

exit:
    LEAVE();
    return ret;
}
#endif /* WPA */


/**
 *  @brief Set security key(s)
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_sec_ioctl_encrypt_key(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status   = MLAN_STATUS_SUCCESS;
    mlan_ds_sec_cfg *sec = MNULL;
    ENTER();
    sec = (mlan_ds_sec_cfg *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
#ifdef WPA
            if (sec->param.encrypt_key.key_len > MAX_WEP_KEY_SIZE)
        {
            status = wlan_sec_ioctl_set_wpa_key(pmadapter, pioctl_req);
        }
#endif /* WPA */
    }
    else
    {
        /* fixme: enable when needed */
        /* status = wlan_sec_ioctl_get_key(pmadapter, pioctl_req); */
    }
    LEAVE();
    return status;
}


/**
 *  @brief Set/Get WPA passphrase from embedded supplicant
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_sec_ioctl_passphrase(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_sec_cfg *sec = MNULL;
    t_u16 cmd_action     = 0;
    /* BSSDescriptor_t *pbss_desc; */
    /* int i = 0; */

    ENTER();

    sec = (mlan_ds_sec_cfg *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        if (sec->param.passphrase.psk_type == MLAN_PSK_CLEAR)
        {
            cmd_action = HostCmd_ACT_GEN_REMOVE;
        }
        else
        {
            cmd_action = HostCmd_ACT_GEN_SET;
        }
    }
    else
    {
        if (sec->param.passphrase.psk_type == MLAN_PSK_QUERY)
        {
            if (sec->param.passphrase.ssid.ssid_len == 0U)
            {
                /* fixme: We do not need this functionality right now. */
            }
            else
            {
                (void)__memset(pmadapter, &sec->param.passphrase.bssid, 0, MLAN_MAC_ADDR_LENGTH);
            }
        }
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_SUPPLICANT_PMK, cmd_action, 0, (t_void *)pioctl_req,
                           &sec->param.passphrase);
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}

/**
 *  @brief Set/Get WPA3 SAE password from embedded supplicant
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_sec_ioctl_password(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret      = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_sec_cfg *sec = MNULL;
    t_u16 cmd_action     = 0;
    /* BSSDescriptor_t *pbss_desc; */
    /* int i = 0; */

    ENTER();

    sec = (mlan_ds_sec_cfg *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        if (sec->param.passphrase.psk_type == MLAN_PSK_CLEAR)
        {
            cmd_action = HostCmd_ACT_GEN_REMOVE;
        }
        else
        {
            cmd_action = HostCmd_ACT_GEN_SET;
        }
    }
    else
    {
        if (sec->param.passphrase.psk_type == MLAN_PSK_QUERY)
        {
            if (sec->param.passphrase.ssid.ssid_len == 0U)
            {
                /* fixme: We do not need this functionality right now. */
            }
            else
            {
                (void)__memset(pmadapter, &sec->param.passphrase.bssid, 0, MLAN_MAC_ADDR_LENGTH);
            }
        }
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_SUPPLICANT_PMK, cmd_action, 0, (t_void *)pioctl_req,
                           &sec->param.passphrase);
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}


/**
 *  @brief Security configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_sec_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status   = MLAN_STATUS_SUCCESS;
    mlan_ds_sec_cfg *sec = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_sec_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_sec_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    sec = (mlan_ds_sec_cfg *)(void *)pioctl_req->pbuf;
    switch (sec->sub_command)
    {
        case MLAN_OID_SEC_CFG_ENCRYPT_KEY:
            status = wlan_sec_ioctl_encrypt_key(pmadapter, pioctl_req);
            break;
        case MLAN_OID_SEC_CFG_PASSPHRASE:
            status = wlan_sec_ioctl_passphrase(pmadapter, pioctl_req);
            break;
        case MLAN_OID_SEC_CFG_PASSWORD:
            status = wlan_sec_ioctl_password(pmadapter, pioctl_req);
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
 *  @brief 11D configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_11d_cfg_ioctl(IN mlan_private *pmpriv, IN pmlan_ioctl_req pioctl_req)
{
    mlan_adapter *pmadapter   = pmpriv->adapter;
    mlan_status status        = MLAN_STATUS_SUCCESS;
    mlan_ds_11d_cfg *pcfg_11d = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_11d_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_11d_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        status                        = MLAN_STATUS_RESOURCE;
        goto exit;
    }

    pcfg_11d = (mlan_ds_11d_cfg *)(void *)pioctl_req->pbuf;
    switch (pcfg_11d->sub_command)
    {
        case MLAN_OID_11D_DOMAIN_INFO:
            if (pmpriv->support_11d_APIs != NULL)
            {
                status = pmpriv->support_11d_APIs->wlan_11d_cfg_domain_info_p(pmadapter, pioctl_req);
            }
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }

exit:
    LEAVE();
    return status;
}

#if CONFIG_WPS2
/**
 *  @brief WPS configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_wps_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status    = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv  = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_wps_cfg *pwps = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_wps_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_wps_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }

    pwps = (mlan_ds_wps_cfg *)pioctl_req->pbuf;
    switch (pwps->sub_command)
    {
        case MLAN_OID_WPS_CFG_SESSION:
            if (pioctl_req->action == MLAN_ACT_SET)
            {
                if (pwps->param.wps_session == MLAN_WPS_CFG_SESSION_START)
                    pmpriv->wps.session_enable = MTRUE;
                else
                    pmpriv->wps.session_enable = MFALSE;
            }
            else
            {
                pwps->param.wps_session       = (t_u32)pmpriv->wps.session_enable;
                pioctl_req->data_read_written = sizeof(t_u32);
                PRINTM(MINFO, "wpscfg GET=%d\n", pwps->param.wps_session);
            }
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }

    LEAVE();
    return status;
}
#endif /* CONFIG_WPS2 */


       /**
        *  @brief Set/Get region code
        *
        *  @param pmadapter	A pointer to mlan_adapter structure
        *  @param pioctl_req	A pointer to ioctl request buffer
        *
        *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
        */
/* static */ mlan_status wlan_misc_ioctl_region(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status ret        = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv   = pmadapter->priv[pioctl_req->bss_index];
    mlan_ds_misc_cfg *misc = MNULL;
    int i;

    ENTER();

    misc = (mlan_ds_misc_cfg *)(void *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET)
    {
        misc->param.region_code = pmadapter->region_code;
    }
    else
    {
#ifdef OTP_CHANINFO
        if (pmadapter->otp_region && pmadapter->otp_region->force_reg)
        {
            (void)PRINTF(
                "ForceRegionRule is set in the on-chip OTP"
                " memory\r\n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
#endif
        for (i = 0; i < MRVDRV_MAX_REGION_CODE; i++)
        {
            /* Use the region code to search for the index */
            if (misc->param.region_code == region_code_index[i])
            {
                pmadapter->region_code = (t_u16)misc->param.region_code;
                break;
            }
        }
        /* It's unidentified region code */
        if (i >= MRVDRV_MAX_REGION_CODE)
        {
            PRINTM(MERROR, "Region Code not identified\n");
            pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmadapter->cfp_code_bg = (t_u8)misc->param.region_code;
        pmadapter->cfp_code_a  = (t_u8)misc->param.region_code;
        if (wlan_set_regiontable(pmpriv, (t_u8)pmadapter->region_code,
                                 (t_u16)(pmadapter->config_bands | pmadapter->adhoc_start_band)) != MLAN_STATUS_SUCCESS)
        {
            pioctl_req->status_code = MLAN_ERROR_IOCTL_FAIL;
            ret                     = MLAN_STATUS_FAILURE;
        }
    }
    pioctl_req->data_read_written = sizeof(t_u32) + MLAN_SUB_COMMAND_SIZE;

    LEAVE();
    return ret;
}


#if CONFIG_GTK_REKEY_OFFLOAD
/**
 *  @brief Gtk Rekey Offload
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_misc_ioctl_gtk_rekey_offload(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc_cfg = MNULL;
    t_u16 cmd_action           = 0;
    mlan_private *pmpriv       = pmadapter->priv[pioctl_req->bss_index];

    ENTER();
    misc_cfg = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else if (pioctl_req->action == MLAN_ACT_CLEAR)
        cmd_action = HostCmd_ACT_GEN_REMOVE;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    if (!pmpriv->wpa_is_gtk_set)
    {
        /* Store the gtk rekey data if it has already set gtk */
        (void)__memcpy(pmadapter, &pmpriv->gtk_rekey, &misc_cfg->param.gtk_rekey, sizeof(mlan_ds_misc_gtk_rekey_data));
        LEAVE();
        return ret;
    }
    /* Send request to firmware if it hasn't set gtk yet */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_CONFIG_GTK_REKEY_OFFLOAD_CFG, cmd_action, 0, (t_void *)pioctl_req,
                           &misc_cfg->param.gtk_rekey);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}
#endif

/**
 *  @brief Get/Set subscribe event
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return             MLAN_STATUS_PENDING -- success, otherwise fail
 */
mlan_status wlan_misc_ioctl_subscribe_evt(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_private *pmpriv   = pmadapter->priv[pioctl_req->bss_index];
    mlan_status ret        = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc = MNULL;
    t_u16 cmd_action       = 0;

    ENTER();

    misc = (mlan_ds_misc_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_SET)
    {
        cmd_action = HostCmd_ACT_GEN_SET;
    }
    else
    {
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send command to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_802_11_SUBSCRIBE_EVENT, cmd_action, 0, (t_void *)pioctl_req,
                           &misc->param.subscribe_event);

    if (ret == MLAN_STATUS_SUCCESS)
        ret = MLAN_STATUS_PENDING;

    LEAVE();
    return ret;
}


/**
 *  @brief Miscellaneous configuration handler
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
static mlan_status wlan_misc_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req)
{
    mlan_status status     = MLAN_STATUS_SUCCESS;
    mlan_ds_misc_cfg *misc = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_misc_cfg))
    {
        PRINTM(MWARN, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_misc_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    if ((pioctl_req == MNULL) || (pioctl_req->pbuf == MNULL))
    {
        PRINTM(MERROR, "Request buffer not found!\n");
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)(void *)pioctl_req->pbuf;
    switch (misc->sub_command)
    {
        case MLAN_OID_MISC_REGION:
            status = wlan_misc_ioctl_region(pmadapter, pioctl_req);
            break;
#if CONFIG_GTK_REKEY_OFFLOAD
        case MLAN_OID_MISC_CONFIG_GTK_REKEY_OFFLOAD:
            status = wlan_misc_ioctl_gtk_rekey_offload(pmadapter, pioctl_req);
            break;
#endif
#if CONFIG_ROAMING
        case MLAN_OID_MISC_SUBSCRIBE_EVENT:
            status = wlan_misc_ioctl_subscribe_evt(pmadapter, pioctl_req);
            break;
#endif
#ifdef WLAN_LOW_POWER_ENABLE
        case MLAN_OID_MISC_LOW_PWR_MODE:
            status = wlan_misc_ioctl_low_pwr_mode(pmadapter, pioctl_req);
            break;
#endif // WLAN_LOW_POWER_ENABLE
#if CONFIG_WIFI_CLOCKSYNC
        case MLAN_OID_MISC_GPIO_TSF_LATCH:
            status = wlan_misc_gpio_tsf_latch_config(pmadapter, pioctl_req);
            break;
        case MLAN_OID_MISC_GET_TSF_INFO:
            status = wlan_misc_get_tsf_info(pmadapter, pioctl_req);
            break;
#endif /* CONFIG_WIFI_CLOCKSYNC */
#if CONFIG_RF_TEST_MODE
        case MLAN_OID_MISC_RF_TEST_GENERIC:
        case MLAN_OID_MISC_RF_TEST_TX_CONT:
        case MLAN_OID_MISC_RF_TEST_CONFIG_TRIGGER_FRAME:
        case MLAN_OID_MISC_RF_TEST_TX_FRAME:
        case MLAN_OID_MISC_RF_TEST_HE_POWER:
            status = wlan_misc_ioctl_rf_test_cfg(pmadapter, pioctl_req);
            break;
#endif /* CONFIG_RF_TEST_MODE */
#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
        case MLAN_OID_MISC_IND_RST_CFG:
            status = wlan_misc_ioctl_ind_rst_cfg(pmadapter, pioctl_req);
            break;
#endif
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;

            status = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}



/**
 *  @brief MLAN station ioctl handler
 *
 *  @param adapter 	A pointer to mlan_adapter structure
 *  @param pioctl_req	A pointer to ioctl request buffer
 *
 *  @return		MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise fail
 */
mlan_status wlan_ops_sta_ioctl(t_void *adapter, pmlan_ioctl_req pioctl_req)
{
    pmlan_private pmpriv    = MNULL;
    pmlan_adapter pmadapter = (pmlan_adapter)adapter;
    mlan_status status      = MLAN_STATUS_SUCCESS;

    ENTER();

    if (pioctl_req != MNULL)
    {
        CHECK_BSS_TYPE(pioctl_req->bss_index, MLAN_STATUS_FAILURE);
        pmpriv = pmadapter->priv[pioctl_req->bss_index];
    }
    else
    {
        PRINTM(MERROR, "MLAN IOCTL information is not present\n");
        return MLAN_STATUS_FAILURE;
    }

    switch (pioctl_req->req_id)
    {
        case MLAN_IOCTL_BSS:
            status = wlan_bss_ioctl(pmadapter, pioctl_req);
            break;
        case MLAN_IOCTL_RADIO_CFG:
            status = wlan_radio_ioctl(pmadapter, pioctl_req);
            break;
#if (CONFIG_WIFI_RTS_THRESHOLD) || (CONFIG_WIFI_FRAG_THRESHOLD)
        case MLAN_IOCTL_SNMP_MIB:
            status = wlan_snmp_mib_ioctl(pmadapter, pioctl_req);
            break;
#endif
        case MLAN_IOCTL_SEC_CFG:
            status = wlan_sec_cfg_ioctl(pmadapter, pioctl_req);
            break;
        case MLAN_IOCTL_RATE:
            status = wlan_rate_ioctl(pmadapter, pioctl_req);
            break;
        case MLAN_IOCTL_POWER_CFG:
            status = wlan_power_ioctl(pmadapter, pioctl_req);
            break;
#if CONFIG_WMM_UAPSD
        case MLAN_IOCTL_WMM_CFG:
            status = wlan_wmm_cfg_ioctl(pmadapter, pioctl_req);
            break;
#endif
#if CONFIG_WPS2
        case MLAN_IOCTL_WPS_CFG:
            status = wlan_wps_cfg_ioctl(pmadapter, pioctl_req);
            break;
#endif /* CONFIG_WPS2 */
        case MLAN_IOCTL_11N_CFG:
            status = wlan_11n_cfg_ioctl(pmadapter, pioctl_req);
            break;
        case MLAN_IOCTL_11D_CFG:
            if (pmpriv->support_11d_APIs != NULL)
            {
                status = pmpriv->support_11d_APIs->wlan_11d_cfg_ioctl_p(pmpriv, pioctl_req);
            }
            break;
        case MLAN_IOCTL_MISC_CFG:
            status = wlan_misc_cfg_ioctl(pmadapter, pioctl_req);
            break;
#if CONFIG_11AX
        case MLAN_IOCTL_11AX_CFG:
            status = wlan_11ax_cfg_ioctl(pmadapter, pioctl_req);
            break;
#endif
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status                  = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}
