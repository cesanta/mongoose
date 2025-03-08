/** @file mlan_cmdevt.c
 *
 *  @brief  This file provides the handling of CMD/EVENT in MLAN
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*************************************************************
Change Log:
    05/12/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

/********************************************************
                Local Variables
********************************************************/

/*******************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/


/**
 *  @brief This function prepare the command before sending to firmware.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_prepare_cmd(IN mlan_private *pmpriv,
                             IN t_u16 cmd_no,
                             IN t_u16 cmd_action,
                             IN t_u32 cmd_oid,
                             IN t_void *pioctl_buf,
                             IN t_void *pdata_buf)
{
    /* Note: We send only one command at a time and do not need the linked
       list based implementation used here. So we will call our own
       implementation here.
    */

    return wifi_prepare_and_send_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, pmpriv->bss_type,
                                     NULL);
}

#if CONFIG_11AX
/**
 *  @brief Fetch bitmap rate index
 *
 *  @param rate_scope  A pointer to MrvlRateScope_t
 *
 *  @return            bitmap rate index
 */
static t_u16 wlan_get_bitmap_index(MrvlRateScope_t *rate_scope)
{
    t_u16 index = 0;
    if (rate_scope != MNULL)
    {
        index += NELEMENTS(rate_scope->ht_mcs_rate_bitmap);
        index += NELEMENTS(rate_scope->vht_mcs_rate_bitmap);
    }
    return index;
}
#endif

/**
 *  @brief This function prepares command of power mode
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param ps_bitmap    PS bitmap
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_enh_power_mode(pmlan_private pmpriv,
                                    IN HostCmd_DS_COMMAND *cmd,
                                    IN ENH_PS_MODES cmd_action,
                                    IN t_u16 ps_bitmap,
                                    IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_PS_MODE_ENH *psmode_enh = &cmd->params.psmode_enh;
    t_u8 *tlv                                 = MNULL;
    t_u16 cmd_size                            = 0;

    ENTER();

    PRINTM(MCMND, "PS Command: action = 0x%x, bitmap = 0x%x\n", cmd_action, ps_bitmap);

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_PS_MODE_ENH);
    if (cmd_action == DIS_AUTO_PS)
    {
        psmode_enh->action           = (ENH_PS_MODES)(wlan_cpu_to_le16(DIS_AUTO_PS));
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size                    = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    }
#if (CONFIG_WNM_PS)
    else if (cmd_action == DIS_WNM_PS)
    {
        psmode_enh->action           = (ENH_PS_MODES)(wlan_cpu_to_le16(DIS_WNM_PS));
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size                    = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    }
#endif
    else if (cmd_action == GET_PS)
    {
        psmode_enh->action           = (ENH_PS_MODES)(wlan_cpu_to_le16(GET_PS));
        psmode_enh->params.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd->size                    = wlan_cpu_to_le16(S_DS_GEN + AUTO_PS_FIX_SIZE);
    }
    else if (cmd_action == EXT_PS_PARAM)
    {
        psmode_enh->action                    = wlan_cpu_to_le16(EXT_PS_PARAM);
        psmode_enh->params.ext_param.reserved = 0;
        cmd->size                             = wlan_cpu_to_le16(S_DS_GEN + sizeof(t_u16) + sizeof(ext_ps_param));
        psmode_enh->params.ext_param.param.header.type = wlan_cpu_to_le16(TLV_TYPE_PS_EXT_PARAM);
        psmode_enh->params.ext_param.param.header.len  = sizeof(t_u32);
        psmode_enh->params.ext_param.param.mode        = wlan_cpu_to_le32(*((t_u32 *)pdata_buf));
    }
    else if (cmd_action == EN_AUTO_PS)
    {
        psmode_enh->action                   = (ENH_PS_MODES)(wlan_cpu_to_le16(EN_AUTO_PS));
        psmode_enh->params.auto_ps.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd_size                             = S_DS_GEN + AUTO_PS_FIX_SIZE;
        tlv                                  = (t_u8 *)cmd + cmd_size;
        if ((ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pmlan_adapter pmadapter        = pmpriv->adapter;
            MrvlIEtypes_ps_param_t *ps_tlv = (MrvlIEtypes_ps_param_t *)(void *)tlv;
            ps_param *ps_mode              = (ps_param *)&ps_tlv->param;
            ps_tlv->header.type            = wlan_cpu_to_le16(TLV_TYPE_PS_PARAM);
            ps_tlv->header.len = wlan_cpu_to_le16(sizeof(MrvlIEtypes_ps_param_t) - sizeof(MrvlIEtypesHeader_t));
            cmd_size += (t_u16)sizeof(MrvlIEtypes_ps_param_t);
            tlv += (t_u8)sizeof(MrvlIEtypes_ps_param_t);
            ps_mode->null_pkt_interval     = wlan_cpu_to_le16(pmadapter->null_pkt_interval);
            ps_mode->multiple_dtims        = wlan_cpu_to_le16(pmadapter->multiple_dtim);
            ps_mode->bcn_miss_timeout      = wlan_cpu_to_le16(pmadapter->bcn_miss_time_out);
            ps_mode->local_listen_interval = wlan_cpu_to_le16(pmadapter->local_listen_interval);
            ps_mode->adhoc_wake_period     = wlan_cpu_to_le16(pmadapter->adhoc_awake_period);
            ps_mode->delay_to_ps           = wlan_cpu_to_le16(pmadapter->delay_to_ps);
            ps_mode->mode                  = wlan_cpu_to_le16(pmadapter->enhanced_ps_mode);
        }

        if ((ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            MrvlIEtypes_auto_ds_param_t *auto_ps_tlv = (MrvlIEtypes_auto_ds_param_t *)(void *)tlv;
            auto_ds_param *auto_ds                   = (auto_ds_param *)&auto_ps_tlv->param;
            t_u16 idletime                           = 0;
            auto_ps_tlv->header.type                 = wlan_cpu_to_le16(TLV_TYPE_AUTO_DS_PARAM);
            auto_ps_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_auto_ds_param_t) - sizeof(MrvlIEtypesHeader_t));
            cmd_size += (t_u16)sizeof(MrvlIEtypes_auto_ds_param_t);
            tlv += (t_u8)sizeof(MrvlIEtypes_auto_ds_param_t);
            if (pdata_buf != NULL)
            {
                idletime = ((mlan_ds_auto_ds *)pdata_buf)->idletime;
            }
            auto_ds->deep_sleep_timeout = wlan_cpu_to_le16(idletime);
        }

#if UAP_SUPPORT
        if ((pdata_buf != MNULL) && (ps_bitmap & (BITMAP_UAP_INACT_PS | BITMAP_UAP_DTIM_PS)))
        {
            mlan_ds_ps_mgmt *ps_mgmt                   = (mlan_ds_ps_mgmt *)pdata_buf;
            MrvlIEtypes_sleep_param_t *sleep_tlv       = MNULL;
            MrvlIEtypes_inact_sleep_param_t *inact_tlv = MNULL;
            if ((ps_mgmt->flags & PS_FLAG_SLEEP_PARAM) != 0U)
            {
                sleep_tlv              = (MrvlIEtypes_sleep_param_t *)(void *)tlv;
                sleep_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AP_SLEEP_PARAM);
                sleep_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_sleep_param_t) - sizeof(MrvlIEtypesHeader_t));
                sleep_tlv->ctrl_bitmap = wlan_cpu_to_le32(ps_mgmt->sleep_param.ctrl_bitmap);
                sleep_tlv->min_sleep   = wlan_cpu_to_le32(ps_mgmt->sleep_param.min_sleep);
                sleep_tlv->max_sleep   = wlan_cpu_to_le32(ps_mgmt->sleep_param.max_sleep);
                cmd_size += (t_u16)sizeof(MrvlIEtypes_sleep_param_t);
                tlv += (t_u8)sizeof(MrvlIEtypes_sleep_param_t);
            }
            if ((ps_mgmt->flags & PS_FLAG_INACT_SLEEP_PARAM) != 0U)
            {
                inact_tlv              = (MrvlIEtypes_inact_sleep_param_t *)(void *)tlv;
                inact_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AP_INACT_SLEEP_PARAM);
                inact_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_inact_sleep_param_t) - sizeof(MrvlIEtypesHeader_t));
                inact_tlv->inactivity_to = wlan_cpu_to_le32(ps_mgmt->inact_param.inactivity_to);
                inact_tlv->min_awake     = wlan_cpu_to_le32(ps_mgmt->inact_param.min_awake);
                inact_tlv->max_awake     = wlan_cpu_to_le32(ps_mgmt->inact_param.max_awake);
                cmd_size += (t_u16)sizeof(MrvlIEtypes_inact_sleep_param_t);
                tlv += (t_u8)sizeof(MrvlIEtypes_inact_sleep_param_t);
            }
        }
#endif
        cmd->size = wlan_cpu_to_le16(cmd_size);
    }
#if (CONFIG_WNM_PS)
    else if (cmd_action == EN_WNM_PS)
    {
        psmode_enh->action                   = wlan_cpu_to_le16(EN_WNM_PS);
        psmode_enh->params.auto_ps.ps_bitmap = wlan_cpu_to_le16(ps_bitmap);
        cmd_size                             = S_DS_GEN + AUTO_PS_FIX_SIZE;
        tlv                                  = (t_u8 *)cmd + cmd_size;
        if ((ps_bitmap & BITMAP_STA_PS) != 0)
        {
            if (pdata_buf != NULL)
            {
                pmlan_adapter pmadapter                = pmpriv->adapter;
                MrvlIEtypes_wnm_ps_param_t *wnm_ps_tlv = (MrvlIEtypes_wnm_ps_param_t *)tlv;
                wnm_ps_param *wnm_ps                   = (wnm_ps_param *)&wnm_ps_tlv->param;
                t_u16 internal                         = 0;
                wnm_ps_tlv->header.type                = wlan_cpu_to_le16(TLV_TYPE_WNM_PARAM);
                wnm_ps_tlv->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_wnm_ps_param_t) - sizeof(MrvlIEtypesHeader_t));
                cmd_size += sizeof(MrvlIEtypes_wnm_ps_param_t);
                tlv += sizeof(MrvlIEtypes_wnm_ps_param_t);
                internal                      = *(t_u32 *)pdata_buf;
                wnm_ps->action                = 0;
                wnm_ps->null_pkt_interval     = wlan_cpu_to_le16(pmadapter->null_pkt_interval);
                wnm_ps->bcn_miss_timeout      = wlan_cpu_to_le16(pmadapter->bcn_miss_time_out);
                wnm_ps->local_listen_interval = wlan_cpu_to_le16(pmadapter->local_listen_interval);
                wnm_ps->ps_mode               = wlan_cpu_to_le16(pmadapter->enhanced_ps_mode);
                wnm_ps->delay_to_ps           = DELAY_TO_PS_WNM;
                wnm_ps->wnm_sleep_interval    = wlan_cpu_to_le16(internal);
            }
        }
        cmd->size = wlan_cpu_to_le16(cmd_size);
    }
#endif
    else
    { /* Do Nothing */
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


#ifdef SD8801
mlan_status wlan_ret_802_11_tx_rate_query(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wifi_ds_rate *rate      = MNULL;
    ENTER();

    pmpriv->tx_rate = resp->params.tx_rate.tx_rate;
#if CONFIG_11N
    pmpriv->tx_htinfo = resp->params.tx_rate.ht_info;
#endif
    if (!pmpriv->is_data_rate_auto)
    {
#if CONFIG_11N
        pmpriv->data_rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_htinfo);
#else
        pmpriv->data_rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, 0);
#endif
    }

    if (pioctl)
    {
        rate = (wifi_ds_rate *)pioctl;
        if (rate->sub_command == WIFI_DS_RATE_CFG)
        {
#if 0
            if(rate->param.rate_cfg.rate_type == MLAN_RATE_INDEX) {
#endif
#if CONFIG_11N
            if (pmpriv->tx_htinfo & MBIT(0))
                rate->param.rate_cfg.rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
            else
#endif
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY, there is a hole in rate table
                 * between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.rate_cfg.rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1 : pmpriv->tx_rate;
#if 0
            }
            else {
#if CONFIG_11N
                    rate->param.rate_cfg.rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate,
                                                pmpriv->tx_htinfo);
#else
                    rate->param.rate_cfg.rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, 0);
#endif
            }
#endif
        }
        else if (rate->sub_command == WIFI_DS_GET_DATA_RATE)
        {
#if CONFIG_11N
            if (pmpriv->tx_htinfo & MBIT(0))
            {
                rate->param.data_rate.tx_data_rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
                if (pmpriv->tx_htinfo & MBIT(1))
                    rate->param.data_rate.tx_bw = MLAN_HT_BW40;
                else
                    rate->param.data_rate.tx_bw = MLAN_HT_BW20;
                if (pmpriv->tx_htinfo & MBIT(2))
                    rate->param.data_rate.tx_gi = MLAN_HT_SGI;
                else
                    rate->param.data_rate.tx_gi = MLAN_HT_LGI;
            }
            else
#endif
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY, there is a hole in rate table
                   between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.data_rate.tx_data_rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1 : pmpriv->tx_rate;
#if CONFIG_11N
            if (pmpriv->rxpd_htinfo & MBIT(0))
            {
                rate->param.data_rate.rx_data_rate = pmpriv->rxpd_rate + MLAN_RATE_INDEX_MCS0;
                if (pmpriv->rxpd_htinfo & MBIT(1))
                    rate->param.data_rate.rx_bw = MLAN_HT_BW40;
                else
                    rate->param.data_rate.rx_bw = MLAN_HT_BW20;
                if (pmpriv->rxpd_htinfo & MBIT(2))
                    rate->param.data_rate.rx_gi = MLAN_HT_SGI;
                else
                    rate->param.data_rate.rx_gi = MLAN_HT_LGI;
            }
            else
#endif
                /* For rate index in RxPD, there is a hole in rate table
                   between HR/DSSS and OFDM rates, so minus 1 for OFDM rate index */
                rate->param.data_rate.rx_data_rate =
                    (pmpriv->rxpd_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->rxpd_rate - 1 : pmpriv->rxpd_rate;
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#else
/**
 *  @brief This function handles the command response of tx rate query
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_802_11_tx_rate_query(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    wifi_ds_rate *rate      = MNULL;
    ENTER();

    pmpriv->tx_rate      = resp->params.tx_rate.tx_rate;
    pmpriv->tx_rate_info = resp->params.tx_rate.tx_rate_info;

#if CONFIG_11AX
    if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
        pmpriv->ext_tx_rate_info = resp->params.tx_rate.ext_tx_rate_info;
#endif

    if (!pmpriv->is_data_rate_auto)
    {
        pmpriv->data_rate = wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info
#if CONFIG_11AX
                                                    ,
                                                    pmpriv->ext_tx_rate_info
#endif
        );
    }

    if (pioctl != NULL)
    {
        rate = (wifi_ds_rate *)pioctl;
        if (rate->sub_command == WIFI_DS_RATE_CFG)
        {
#if 0
            if(rate->param.rate_cfg.rate_type == MLAN_RATE_INDEX) {
#endif
#if CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#if CONFIG_11AX
                || ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
#endif
            )
            {
                /* VHT rate */
                rate->param.rate_cfg.rate = (t_u32)((pmpriv->tx_rate) & 0xF);
            }
            else
#endif
#if CONFIG_11N
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.rate_cfg.rate = pmpriv->tx_rate + MLAN_RATE_INDEX_MCS0;
            }
            else
#endif
            {
                /* LG rate */
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY,
                 * there is a hole (0x4) in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.rate_cfg.rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1U : pmpriv->tx_rate;
            }
#if 0
            }
            else {
                /* rate_type = MLAN_RATE_VALUE */
                rate->param.rate_cfg.rate = wlan_index_to_data_rate(pmadapter,
                                                pmpriv->tx_rate,
                                                pmpriv->tx_rate_info);
            }
#endif
        }
        else if (rate->sub_command == WIFI_DS_GET_DATA_RATE)
        {
            /* Tx rate info */
#if CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#if CONFIG_11AX
                || (mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE
#endif
            )
            {
                /* VHT/HE rate */
                rate->param.data_rate.tx_rate_format = (mlan_rate_format)(pmpriv->tx_rate_info & 0x3U);
                rate->param.data_rate.tx_bw          = (t_u32)((pmpriv->tx_rate_info & 0xC) >> 2);

#if CONFIG_11AX
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HE)
                    rate->param.data_rate.tx_gi =
                        (pmpriv->tx_rate_info & 0x10) >> 4 | (pmpriv->tx_rate_info & 0x80) >> 6;
                else
#endif
                    rate->param.data_rate.tx_gi = (t_u32)((pmpriv->tx_rate_info & 0x10) >> 4);
                rate->param.data_rate.tx_nss       = ((pmpriv->tx_rate) >> 4) & 0x03;
                rate->param.data_rate.tx_mcs_index = (t_u32)((pmpriv->tx_rate) & 0xF);
                rate->param.data_rate.tx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info
#if CONFIG_11AX
                                            ,
                                            pmpriv->ext_tx_rate_info
#endif
                    );
            }
            else
#endif
#if CONFIG_11N
                if ((mlan_rate_format)(pmpriv->tx_rate_info & 0x3U) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.data_rate.tx_rate_format = MLAN_RATE_FORMAT_HT;
                rate->param.data_rate.tx_bw          = (pmpriv->tx_rate_info & 0xCU) >> 2U;
                rate->param.data_rate.tx_gi          = (pmpriv->tx_rate_info & 0x10U) >> 4U;
                rate->param.data_rate.tx_mcs_index   = pmpriv->tx_rate;
                rate->param.data_rate.tx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->tx_rate, pmpriv->tx_rate_info
#if CONFIG_11AX
                                            ,
                                            pmpriv->ext_tx_rate_info
#endif
                    );
            }
            else
#endif
            {
                /* LG rate */
                rate->param.data_rate.tx_rate_format = MLAN_RATE_FORMAT_LG;
                /* For HostCmd_CMD_802_11_TX_RATE_QUERY,
                 * there is a hole in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.data_rate.tx_data_rate =
                    (pmpriv->tx_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->tx_rate - 1U : pmpriv->tx_rate;
            }

            /* Rx rate info */
#if CONFIG_11AC
            if ((mlan_rate_format)(pmpriv->rxpd_rate_info & 0x3U) == MLAN_RATE_FORMAT_VHT
#if CONFIG_11AX
                || (pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HE
#endif
            )
            {
                /* VHT/HE rate */
                rate->param.data_rate.rx_rate_format = (mlan_rate_format)(pmpriv->rxpd_rate_info & 0x3);
                rate->param.data_rate.rx_bw          = (t_u32)((pmpriv->rxpd_rate_info & 0xC) >> 2);

#if CONFIG_11AX
                if ((pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HE)
                    rate->param.data_rate.rx_gi =
                        (pmpriv->rxpd_rate_info & 0x10) >> 4 | (pmpriv->rxpd_rate_info & 0x80) >> 6;
                else
#endif
                    rate->param.data_rate.rx_gi = (t_u32)((pmpriv->rxpd_rate_info & 0x10) >> 4);
                rate->param.data_rate.rx_nss       = ((pmpriv->rxpd_rate) >> 4) & 0x3;
                rate->param.data_rate.rx_mcs_index = (t_u32)((pmpriv->rxpd_rate) & 0xF);
                rate->param.data_rate.rx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->rxpd_rate, pmpriv->rxpd_rate_info
#if CONFIG_11AX
                                            ,
                                            pmpriv->ext_tx_rate_info
#endif
                    );
            }
            else
#endif
#if CONFIG_11N
                if ((pmpriv->rxpd_rate_info & 0x3) == MLAN_RATE_FORMAT_HT)
            {
                /* HT rate */
                rate->param.data_rate.rx_rate_format = MLAN_RATE_FORMAT_HT;
                rate->param.data_rate.rx_bw          = (pmpriv->rxpd_rate_info & 0xCU) >> 2U;
                rate->param.data_rate.rx_gi          = (pmpriv->rxpd_rate_info & 0x10U) >> 4U;
                rate->param.data_rate.rx_mcs_index   = pmpriv->rxpd_rate;
                rate->param.data_rate.rx_data_rate =
                    wlan_index_to_data_rate(pmadapter, pmpriv->rxpd_rate, pmpriv->rxpd_rate_info
#if CONFIG_11AX
                                            ,
                                            pmpriv->ext_tx_rate_info
#endif
                    );
            }
            else
#endif
            {
                /* LG rate */
                rate->param.data_rate.rx_rate_format = MLAN_RATE_FORMAT_LG;
                /* For rate index in RxPD,
                 * there is a hole in rate table
                 * between HR/DSSS and OFDM rates,
                 * so minus 1 for OFDM rate index */
                rate->param.data_rate.rx_data_rate =
                    (t_u32)((pmpriv->rxpd_rate > MLAN_RATE_INDEX_OFDM0) ? pmpriv->rxpd_rate - 1 : pmpriv->rxpd_rate);
            }
        }
        else
        { /* Do Nothing */
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function prepares command of tx_rate_cfg.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_tx_rate_cfg(IN pmlan_private pmpriv,
                                 IN HostCmd_DS_COMMAND *cmd,
                                 IN t_u16 cmd_action,
                                 IN t_void *pdata_buf,
                                 IN mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_TX_RATE_CFG *rate_cfg = (HostCmd_DS_TX_RATE_CFG *)&cmd->params.tx_rate_cfg;
    MrvlRateScope_t *rate_scope;
    MrvlRateDropPattern_t *rate_drop;
#if CONFIG_11AX
    MrvlIETypes_rate_setting_t *rate_setting_tlv;
    mlan_ds_rate *ds_rate = MNULL;
#endif
    t_u16 *pbitmap_rates  = (t_u16 *)pdata_buf;

    t_u32 i;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TX_RATE_CFG);

    rate_cfg->action    = wlan_cpu_to_le16(cmd_action);
    rate_cfg->cfg_index = 0;

    rate_scope = (MrvlRateScope_t *)(void *)((t_u8 *)rate_cfg + sizeof(HostCmd_DS_TX_RATE_CFG));
    // coverity[overrun-local:SUPPRESS]
    rate_scope->type   = wlan_cpu_to_le16(TLV_TYPE_RATE_SCOPE);
    rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(MrvlIEtypesHeader_t));
    if (pbitmap_rates != MNULL)
    {
        rate_scope->hr_dsss_rate_bitmap = wlan_cpu_to_le16(pbitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap    = wlan_cpu_to_le16(pbitmap_rates[1]);
        for (i = 0; i < NELEMENTS(rate_scope->ht_mcs_rate_bitmap); i++)
        {
            rate_scope->ht_mcs_rate_bitmap[i] = wlan_cpu_to_le16(pbitmap_rates[2U + i]);
        }
#if CONFIG_11AC
        for (i = 0; i < NELEMENTS(rate_scope->vht_mcs_rate_bitmap); i++)
        {
            rate_scope->vht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pbitmap_rates[2U + NELEMENTS(rate_scope->ht_mcs_rate_bitmap) + i]);
        }
#endif
#if CONFIG_11AX
        if (IS_FW_SUPPORT_11AX(pmpriv->adapter))
        {
            for (i = 0; i < NELEMENTS(rate_scope->he_mcs_rate_bitmap); i++)
                rate_scope->he_mcs_rate_bitmap[i] =
                    wlan_cpu_to_le16(pbitmap_rates[2U + wlan_get_bitmap_index(rate_scope) + i]);
        }
        else
        {
            rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(rate_scope->he_mcs_rate_bitmap) -
                                                  sizeof(MrvlIEtypesHeader_t));
        }
#endif
    }
    else
    {
        rate_scope->hr_dsss_rate_bitmap = wlan_cpu_to_le16(pmpriv->bitmap_rates[0]);
        rate_scope->ofdm_rate_bitmap    = wlan_cpu_to_le16(pmpriv->bitmap_rates[1]);
        for (i = 0; i < NELEMENTS(rate_scope->ht_mcs_rate_bitmap); i++)
        {
            rate_scope->ht_mcs_rate_bitmap[i] = wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + i]);
        }
#if CONFIG_11AC
        for (i = 0; i < NELEMENTS(rate_scope->vht_mcs_rate_bitmap); i++)
        {
            rate_scope->vht_mcs_rate_bitmap[i] =
                wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + NELEMENTS(rate_scope->ht_mcs_rate_bitmap) + i]);
        }
#endif
#if CONFIG_11AX
        if (IS_FW_SUPPORT_11AX(pmpriv->adapter))
        {
            for (i = 0; i < NELEMENTS(rate_scope->he_mcs_rate_bitmap); i++)
                rate_scope->he_mcs_rate_bitmap[i] =
                    wlan_cpu_to_le16(pmpriv->bitmap_rates[2U + wlan_get_bitmap_index(rate_scope) + i]);
        }
        else
        {
            rate_scope->length = wlan_cpu_to_le16(sizeof(MrvlRateScope_t) - sizeof(rate_scope->he_mcs_rate_bitmap) -
                                                  sizeof(MrvlIEtypesHeader_t));
        }
#endif
    }

    rate_drop                 = (MrvlRateDropPattern_t *)(void *)((t_u8 *)rate_scope + sizeof(MrvlRateScope_t));
    rate_drop->type           = wlan_cpu_to_le16(TLV_TYPE_RATE_DROP_PATTERN);
    rate_drop->length         = wlan_cpu_to_le16(sizeof(rate_drop->rate_drop_mode));
    rate_drop->rate_drop_mode = 0;

    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RATE_CFG) + sizeof(MrvlRateScope_t) +
                                 sizeof(MrvlRateDropPattern_t));

#if CONFIG_11AX
    if (pioctl_buf)
    {
        ds_rate          = (mlan_ds_rate *)pioctl_buf->pbuf;
        rate_setting_tlv = (MrvlIETypes_rate_setting_t *)((t_u8 *)rate_drop + sizeof(MrvlRateDropPattern_t));
        rate_setting_tlv->header.type  = wlan_cpu_to_le16(TLV_TYPE_TX_RATE_CFG);
        rate_setting_tlv->header.len   = wlan_cpu_to_le16(sizeof(rate_setting_tlv->rate_setting));
        rate_setting_tlv->rate_setting = wlan_cpu_to_le16(ds_rate->param.rate_cfg.rate_setting);
        PRINTM(MCMND, "he rate setting = %d\n", rate_setting_tlv->rate_setting);
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RATE_CFG) + sizeof(MrvlRateScope_t) +
                                     sizeof(MrvlRateDropPattern_t) + sizeof(MrvlIETypes_rate_setting_t));
    }
#endif

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of tx_rate_cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ret_tx_rate_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN void *pioctl)
{
    mlan_adapter *pmadapter           = pmpriv->adapter;
    wifi_ds_rate *ds_rate             = MNULL;
    HostCmd_DS_TX_RATE_CFG *prate_cfg = MNULL;
    MrvlRateScope_t *prate_scope;
    MrvlIEtypesHeader_t *head = MNULL;
    t_u16 tlv;
    t_u16 tlv_buf_len = 0;
    t_u8 *tlv_buf;
    t_u32 i;
    t_s32 index;
    mlan_status ret                              = MLAN_STATUS_SUCCESS;
    MrvlIETypes_rate_setting_t *rate_setting_tlv = MNULL;
    t_u16 rate_setting                           = 0xffff;

    ENTER();

    if (resp == MNULL)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    prate_cfg = (HostCmd_DS_TX_RATE_CFG *)&(resp->params.tx_rate_cfg);

    tlv_buf = (t_u8 *)((t_u8 *)prate_cfg) + sizeof(HostCmd_DS_TX_RATE_CFG);
    if (tlv_buf != NULL)
    {
        tlv_buf_len = resp->size - (sizeof(HostCmd_DS_TX_RATE_CFG) + S_DS_GEN);
        tlv_buf_len = wlan_le16_to_cpu(tlv_buf_len);
    }

    while (tlv_buf_len > 0U)
    {
        // coverity[overrun-local:SUPPRESS]
        tlv = (t_u16)(*tlv_buf);
        tlv = tlv | (*(tlv_buf + 1) << 8);

        switch (tlv)
        {
            case TLV_TYPE_RATE_SCOPE:
                prate_scope             = (MrvlRateScope_t *)(void *)tlv_buf;
                pmpriv->bitmap_rates[0] = wlan_le16_to_cpu(prate_scope->hr_dsss_rate_bitmap);
                pmpriv->bitmap_rates[1] = wlan_le16_to_cpu(prate_scope->ofdm_rate_bitmap);
                for (i = 0; i < sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16); i++)
                {
                    pmpriv->bitmap_rates[2U + i] = wlan_le16_to_cpu(prate_scope->ht_mcs_rate_bitmap[i]);
                }
#if CONFIG_11AC
                for (i = 0; i < NELEMENTS(prate_scope->vht_mcs_rate_bitmap); i++)
                {
                    pmpriv->bitmap_rates[2 + sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16) + i] =
                        wlan_le16_to_cpu(prate_scope->vht_mcs_rate_bitmap[i]);
                }

#endif
#if CONFIG_11AX
                if (IS_FW_SUPPORT_11AX(pmadapter))
                {
                    for (i = 0; i < NELEMENTS(prate_scope->he_mcs_rate_bitmap); i++)
                    {
                        pmpriv->bitmap_rates[2 + sizeof(prate_scope->ht_mcs_rate_bitmap) / sizeof(t_u16) +
                                             sizeof(prate_scope->vht_mcs_rate_bitmap) / sizeof(t_u16) + i] =
                            wlan_le16_to_cpu(prate_scope->he_mcs_rate_bitmap[i]);
                    }
                }
#endif
                break;
            case TLV_TYPE_TX_RATE_CFG:
                rate_setting_tlv = (MrvlIETypes_rate_setting_t *)tlv_buf;
                rate_setting     = rate_setting_tlv->rate_setting;
                break;
                /* Add RATE_DROP tlv here */
            default:
                PRINTM(MINFO, "Unexpected TLV for rate cfg \n");
                break;
        }

        head      = (MrvlIEtypesHeader_t *)(void *)tlv_buf;
        head->len = wlan_le16_to_cpu(head->len);
        tlv_buf += head->len + sizeof(MrvlIEtypesHeader_t);
        tlv_buf_len -= (head->len + sizeof(MrvlIEtypesHeader_t));
    }

    pmpriv->is_data_rate_auto = wlan_is_rate_auto(pmpriv);

    if (pmpriv->is_data_rate_auto != 0U)
    {
        pmpriv->data_rate = 0;
        PRINTM(MINFO, "Rate is Auto\r\n");
    }

    if (pioctl != NULL)
    {
        ds_rate = (wifi_ds_rate *)pioctl;
        if (ds_rate == MNULL)
        {
            PRINTM(MERROR, "Request buffer not found!\n");
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        if (pmpriv->is_data_rate_auto != 0U)
        {
            // ds_rate->param.rate_cfg.is_rate_auto = MTRUE;
            ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_AUTO;
        }
        else
        {
            /* check the LG rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[0], 4);
            if (index != -1)
            {
                if ((index >= MLAN_RATE_BITMAP_OFDM0) && (index <= MLAN_RATE_BITMAP_OFDM7))
                {
                    index -= (MLAN_RATE_BITMAP_OFDM0 - MLAN_RATE_INDEX_OFDM0);
                }

                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_LG;
                ds_rate->param.rate_cfg.rate        = (t_u32)index;
            }
            /* check the HT rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[2], 16);
            if (index != -1)
            {
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_HT;
                ds_rate->param.rate_cfg.rate        = (t_u32)index;
            }

#if CONFIG_11AC
            /* check the VHT rate */
            index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[10], 16);

            if (index != -1)
            {
                ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_VHT;
                ds_rate->param.rate_cfg.rate        = (t_u32)(index % 16);
                ds_rate->param.rate_cfg.nss         = (t_u32)(index / 16);
                ds_rate->param.rate_cfg.nss += MLAN_RATE_NSS1;
            }
#endif
#if CONFIG_11AX
            /* check the HE rate */
            if (IS_FW_SUPPORT_11AX(pmadapter))
            {
                index = wlan_get_rate_index(pmadapter, &pmpriv->bitmap_rates[18], 16);
                if (index != -1)
                {
                    ds_rate->param.rate_cfg.rate_format = MLAN_RATE_FORMAT_HE;
                    ds_rate->param.rate_cfg.rate        = index % 16;
                    ds_rate->param.rate_cfg.nss         = index / 16;
                    ds_rate->param.rate_cfg.nss += MLAN_RATE_NSS1;
                }
            }
#endif
            ds_rate->param.rate_cfg.rate_setting = rate_setting;
            PRINTM(MINFO, "Rate index is %d\n", ds_rate->param.rate_cfg.rate);

            ds_rate->param.rate_cfg.rate_index = ds_rate->param.rate_cfg.rate;
        }
    }

    LEAVE();
    return ret;
}

/**
 *  @brief This function prepares command of get_hw_spec.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pcmd         A pointer to HostCmd_DS_COMMAND structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *pcmd)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &pcmd->params.hw_spec;

    ENTER();

    pcmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_HW_SPEC);
    pcmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_GET_HW_SPEC) + S_DS_GEN);
    (void)__memcpy(pmpriv->adapter, hw_spec->permanent_addr, pmpriv->curr_addr, MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of HostCmd_CMD_GET_TSF
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_get_tsf(pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_u16 cmd_action)
{
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_GET_TSF);
    cmd->size    = wlan_cpu_to_le16((sizeof(HostCmd_DS_TSF)) + S_DS_GEN);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#if (CONFIG_WIFI_TX_PER_TRACK) || (CONFIG_TX_RX_HISTOGRAM)
/**
 *  @brief This function prepares command of txrx_histogram and tx_pert, distinguish by cmd_action.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_txrx_pkt_stats(pmlan_private pmpriv,
                                    IN HostCmd_DS_COMMAND *cmd,
                                    IN t_u16 cmd_action,
                                    IN t_void *pdata_buf)
{
#if CONFIG_WIFI_TX_PER_TRACK
    if (cmd_action == HostCmd_ACT_SET_TX_PER_TRACKING)
    {
        wlan_cmd_tx_pert(pmpriv, cmd, cmd_action, pdata_buf);
    }
#endif
#if CONFIG_TX_RX_HISTOGRAM
    if (cmd_action != HostCmd_ACT_SET_TX_PER_TRACKING)
    {
        wlan_cmd_txrx_histogram(pmpriv, cmd, pdata_buf);
    }
#endif
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_WIFI_TX_PER_TRACK
/**
 *  @brief This function prepares command of tx_pert.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_tx_pert(pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND *cmd,
                             IN t_u16 cmd_action,
                             IN t_void *pdata_buf)
{
    HostCmd_DS_TX_RX_PKT_STATS *pkt_stats = &cmd->params.pkt_stats;
    MrvlTxPerTrackInfo_t *tx_pert         = NULL;
    tx_pert_info *cfg                     = (tx_pert_info *)pdata_buf;

    ENTER();
    cmd->command      = wlan_cpu_to_le16(HostCmd_CMD_TX_RX_PKT_STATS);
    pkt_stats->action = wlan_cpu_to_le16(cmd_action);
    pkt_stats->enable = cfg->tx_pert_check;
    if (cmd_action == HostCmd_ACT_SET_TX_PER_TRACKING)
    {
        tx_pert = (MrvlTxPerTrackInfo_t *)((t_u8 *)pkt_stats + sizeof(HostCmd_DS_TX_RX_PKT_STATS));
        // coverity[overrun-local:SUPPRESS]
        tx_pert->type                 = wlan_cpu_to_le16(TLV_TYPE_TX_PER_TRACK);
        tx_pert->length               = wlan_cpu_to_le16(sizeof(MrvlTxPerTrackInfo_t) - sizeof(MrvlIEtypesHeader_t));
        tx_pert->tx_stat_check_period = cfg->tx_pert_check_peroid;
        tx_pert->tx_stat_check_ratio  = cfg->tx_pert_check_ratio;
        tx_pert->tx_stat_check_num    = wlan_cpu_to_le16(cfg->tx_pert_check_num);
    }
    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RX_PKT_STATS) + sizeof(MrvlTxPerTrackInfo_t));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_TX_RX_HISTOGRAM
/**
 *  @brief This function prepares command of txrx_histogram.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_txrx_histogram(pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf)
{
    HostCmd_DS_TX_RX_HISTOGRAM *histogram = &cmd->params.histogram;
    txrx_histogram_info *cfg              = (txrx_histogram_info *)pdata_buf;

    ENTER();
    cmd->command      = wlan_cpu_to_le16(HostCmd_CMD_TX_RX_PKT_STATS);
    histogram->action = cfg->action;
    histogram->enable = cfg->enable;
    cmd->size         = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_TX_RX_HISTOGRAM));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif


/** *  @brief This function gets the no 5G status
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_get_no_5G_status(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 left_len;
    t_u16 tlv_type                        = 0;
    t_u16 tlv_len                         = 0;
    MrvlIEtypesHeader_t *tlv              = MNULL;
    MrvlIEtypes_fw_cap_info_t *fw_cap_tlv = MNULL;

    left_len = resp->size - sizeof(HostCmd_DS_GET_HW_SPEC) - S_DS_GEN;
    tlv      = (MrvlIEtypesHeader_t *)((t_u8 *)resp + S_DS_GEN + sizeof(HostCmd_DS_GET_HW_SPEC));
    while (left_len > sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if (tlv_type == TLV_TYPE_FW_CAP_INFO)
        {
            fw_cap_tlv            = (MrvlIEtypes_fw_cap_info_t *)tlv;
            pmadapter->fw_cap_ext = wlan_le32_to_cpu(fw_cap_tlv->fw_cap_ext);
            break;
        }
        left_len -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypesHeader_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of get_hw_spec
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ret_get_hw_spec(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN t_void *pioctl_buf)
{
    HostCmd_DS_GET_HW_SPEC *hw_spec = &resp->params.hw_spec;
    mlan_adapter *pmadapter         = pmpriv->adapter;
    mlan_status ret                 = MLAN_STATUS_SUCCESS;
    t_u32 i;
    pmlan_ioctl_req pioctl_req = (mlan_ioctl_req *)pioctl_buf;
    t_u16 left_len;
    t_u16 tlv_type           = 0;
    t_u16 tlv_len            = 0;
    MrvlIEtypesHeader_t *tlv = MNULL;
#if CONFIG_11AX
    MrvlIEtypes_Extension_t *ext_tlv = MNULL;
#ifdef RW610
    int he_tlv_idx = 0;
#endif
#endif
    MrvlIEtypes_fw_cap_info_t *fw_cap_tlv = MNULL;
    ENTER();

    pmadapter->fw_cap_info = wlan_le32_to_cpu(hw_spec->fw_cap_info);
    /* Get no 5G status to check whether need to disable 5G */
    wlan_get_no_5G_status(pmpriv, resp);
#ifdef STA_SUPPORT
    if ((IS_SUPPORT_MULTI_BANDS(pmadapter)) != 0U)
    {
        pmadapter->fw_bands = (t_u16)GET_FW_DEFAULT_BANDS(pmadapter);
        if (ISSUPP_NO5G(pmadapter->fw_cap_ext))
        {
            pmadapter->fw_bands &= ~(BAND_A | BAND_AN | BAND_AAC);
        }
#if !CONFIG_5GHz_SUPPORT
        /* fixme: Re-check if this is the correct way to disable 5 GHz. */
        pmadapter->fw_bands &= ~(BAND_A | BAND_AN | BAND_AAC);
#endif /* CONFIG_5GHz_SUPPORT */
    }
    else
    {
        pmadapter->fw_bands = BAND_B;
    }

    pmadapter->config_bands = pmadapter->fw_bands;
    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i] != MNULL)
        {
            pmadapter->priv[i]->config_bands = pmadapter->fw_bands;
        }
    }

    if ((pmadapter->fw_bands & BAND_A) != 0U)
    {
        if ((pmadapter->fw_bands & BAND_GN) != 0U)
        {
            pmadapter->config_bands |= BAND_AN;
            for (i = 0; i < pmadapter->priv_num; i++)
            {
                if (pmadapter->priv[i] != MNULL)
                {
                    pmadapter->priv[i]->config_bands |= BAND_AN;
                }
            }

            pmadapter->fw_bands |= BAND_AN;
        }
        if ((pmadapter->fw_bands & BAND_AAC) != 0U)
        {
            pmadapter->config_bands |= BAND_AAC;
            for (i = 0; i < pmadapter->priv_num; i++)
            {
                if (pmadapter->priv[i] != MNULL)
                {
                    pmadapter->priv[i]->config_bands |= BAND_AAC;
                }
            }
        }
        if ((pmadapter->fw_bands & BAND_AN) != 0U)
        {
            pmadapter->adhoc_start_band  = (BAND_A | BAND_AN);
            pmadapter->adhoc_11n_enabled = MTRUE;
        }
        else
        {
            pmadapter->adhoc_start_band = BAND_A;
        }
        pmpriv->adhoc_channel = DEFAULT_AD_HOC_CHANNEL_A;
    }
    else if ((pmadapter->fw_bands & BAND_GN) != 0U)
    {
        pmadapter->adhoc_start_band  = (BAND_G | BAND_B | BAND_GN);
        pmpriv->adhoc_channel        = DEFAULT_AD_HOC_CHANNEL;
        pmadapter->adhoc_11n_enabled = MTRUE;
    }
    else if ((pmadapter->fw_bands & BAND_G) != 0U)
    {
        pmadapter->adhoc_start_band = (BAND_G | BAND_B);
        pmpriv->adhoc_channel       = DEFAULT_AD_HOC_CHANNEL;
    }
    else if ((pmadapter->fw_bands & BAND_B) != 0U)
    {
        pmadapter->adhoc_start_band = BAND_B;
        pmpriv->adhoc_channel       = DEFAULT_AD_HOC_CHANNEL;
    }
    else
    {
        /* Do nothing */
    }
#endif /* STA_SUPPORT */

    pmadapter->fw_release_number = hw_spec->fw_release_number;
    pmadapter->number_of_antenna = wlan_le16_to_cpu(hw_spec->number_of_antenna);

    PRINTM(MINFO, "GET_HW_SPEC: fw_release_number- 0x%X\n", wlan_le32_to_cpu(pmadapter->fw_release_number));
    PRINTM(MINFO, "GET_HW_SPEC: Permanent addr- %2x:%2x:%2x:%2x:%2x:%2x\n", hw_spec->permanent_addr[0],
           hw_spec->permanent_addr[1], hw_spec->permanent_addr[2], hw_spec->permanent_addr[3],
           hw_spec->permanent_addr[4], hw_spec->permanent_addr[5]);
    PRINTM(MINFO, "GET_HW_SPEC: hw_if_version=0x%X  version=0x%X\n", wlan_le16_to_cpu(hw_spec->hw_if_version),
           wlan_le16_to_cpu(hw_spec->version));

    if (pmpriv->curr_addr[0] == 0xffU)
    {
        (void)__memmove(pmadapter, pmpriv->curr_addr, hw_spec->permanent_addr, MLAN_MAC_ADDR_LENGTH);
    }

    pmadapter->hw_dot_11n_dev_cap     = wlan_le32_to_cpu(hw_spec->dot_11n_dev_cap);
    pmadapter->usr_dot_11n_dev_cap_bg = pmadapter->hw_dot_11n_dev_cap & DEFAULT_11N_CAP_MASK_BG;
    pmadapter->usr_dot_11n_dev_cap_a  = pmadapter->hw_dot_11n_dev_cap & DEFAULT_11N_CAP_MASK_A;
    pmadapter->usr_dev_mcs_support = pmadapter->hw_dev_mcs_support = hw_spec->dev_mcs_support;
    pmadapter->hw_mpdu_density                                     = GET_MPDU_DENSITY(hw_spec->hw_dev_cap);
    PRINTM(MCMND, "GET_HW_SPEC: hw_mpdu_density=%d dev_mcs_support=0x%x\n", pmadapter->hw_mpdu_density,
           hw_spec->dev_mcs_support);

    wlan_show_dot11ndevcap(pmadapter, pmadapter->hw_dot_11n_dev_cap);
    wlan_show_devmcssupport(pmadapter, pmadapter->hw_dev_mcs_support);

    pmadapter->hw_dot_11ac_dev_cap     = wlan_le32_to_cpu(hw_spec->Dot11acDevCap);
    pmadapter->hw_dot_11ac_mcs_support = wlan_le32_to_cpu(hw_spec->Dot11acMcsSupport);

    pmadapter->usr_dot_11ac_mcs_support = pmadapter->hw_dot_11ac_mcs_support;

    pmadapter->usr_dot_11ac_dev_cap_bg = pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#if CONFIG_5GHz_SUPPORT
    pmadapter->usr_dot_11ac_dev_cap_a = pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#endif
    pmadapter->usr_dot_11ac_bw = BW_FOLLOW_VHTCAP;

    pmadapter->mp_end_port = wlan_le16_to_cpu(hw_spec->mp_end_port);

#ifndef RW610
    for (i = 1; i <= (unsigned)(MAX_PORT - pmadapter->mp_end_port); i++)
    {
        pmadapter->mp_data_port_mask &= ~(1U << (MAX_PORT - i));
    }
#endif


#ifdef OTP_CHANINFO
    if ((pmadapter->otp_region != MNULL) && (pmadapter->otp_region->force_reg == 0U))
    {
#endif

        /* Set the region code to WWSM by default */
        pmadapter->region_code = hw_spec->region_code;
        for (i = 0; i < MRVDRV_MAX_REGION_CODE; i++)
        {
            /* Use the region code to search for the index */
            if (pmadapter->region_code == region_code_index[i])
            {
                break;
            }
        }
        /* If it's unidentified region code, use the default */
        if (i >= MRVDRV_MAX_REGION_CODE)
        {
            pmadapter->region_code = MRVDRV_DEFAULT_REGION_CODE;
            PRINTM(MWARN, "unidentified region code, use the default (0x%02x)\n", MRVDRV_DEFAULT_REGION_CODE);
        }

        /* Back up hw_region_code in case it is overwritten by the supplicant process*/
        pmadapter->hw_region_code = pmadapter->region_code;

        /* Synchronize CFP code with region code */
        pmadapter->cfp_code_bg = (t_u8)pmadapter->region_code;
        pmadapter->cfp_code_a  = (t_u8)pmadapter->region_code;
#ifdef OTP_CHANINFO
    }
#endif
    if (wlan_set_regiontable(pmpriv, (t_u8)pmadapter->region_code, pmadapter->fw_bands) != MLAN_STATUS_SUCCESS)
    {
        if (pioctl_req != MNULL)
        {
            pioctl_req->status_code = MLAN_ERROR_CMD_SCAN_FAIL;
        }
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
#ifdef STA_SUPPORT
    if (wlan_11d_set_universaltable(pmpriv, pmadapter->fw_bands) != MLAN_STATUS_SUCCESS)
    {
        if (pioctl_req != MNULL)
        {
            pioctl_req->status_code = MLAN_ERROR_CMD_SCAN_FAIL;
        }
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
#endif /* STA_SUPPORT */
    left_len = resp->size - (t_u16)sizeof(HostCmd_DS_GET_HW_SPEC) - (t_u16)S_DS_GEN;
    tlv      = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)(&resp->params) + sizeof(HostCmd_DS_GET_HW_SPEC));
    while (left_len > sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        switch (tlv_type)
        {
#if CONFIG_11AX
            case TLV_TYPE_EXTENSION_ID:
                ext_tlv = (MrvlIEtypes_Extension_t *)tlv;
                if (ext_tlv->ext_id == HE_CAPABILITY)
                {
                    ext_tlv->type = tlv_type;
                    ext_tlv->len  = tlv_len;
#ifndef RW610
                    wlan_update_11ax_cap(pmadapter, (MrvlIEtypes_Extension_t *)ext_tlv);
#else
                    if ((he_tlv_idx == AX_2G_TLV_INDEX) || !ISSUPP_NO5G(pmadapter->fw_cap_ext))
                    {
                        wlan_update_11ax_cap(pmadapter, (MrvlIEtypes_Extension_t *)ext_tlv, he_tlv_idx);
                        he_tlv_idx++;
                    }
#endif
                }
                break;
#endif
            case TLV_TYPE_FW_CAP_INFO:
                fw_cap_tlv             = (MrvlIEtypes_fw_cap_info_t *)(void *)tlv;
                pmadapter->fw_cap_info = wlan_le32_to_cpu(fw_cap_tlv->fw_cap_info);
                pmadapter->fw_cap_ext  = wlan_le32_to_cpu(fw_cap_tlv->fw_cap_ext);
                PRINTM(MCMND, "fw_cap_info=0x%x fw_cap_ext=0x%x\n", pmadapter->fw_cap_info, pmadapter->fw_cap_ext);
                break;
            default:
                PRINTM(MINFO, "Unexpected TLV hw spec \n");
                break;
        }
        left_len -= (t_u16)(sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypesHeader_t *)(void *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }

#if defined(SD8987) || defined(SD9177) || defined(IW610)
    pmadapter->cmd_tx_data = IS_FW_SUPPORT_CMD_TX_DATA(pmadapter) ? 0x01 : 0x00;
#endif

done:
    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of remain_on_channel.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_remain_on_channel(IN pmlan_private pmpriv,
                                       IN HostCmd_DS_COMMAND *cmd,
                                       IN t_u16 cmd_action,
                                       IN t_void *pdata_buf)
{
    HostCmd_DS_REMAIN_ON_CHANNEL *remain_channel = &cmd->params.remain_on_chan;
    mlan_ds_remain_chan *cfg                     = (mlan_ds_remain_chan *)pdata_buf;
    ENTER();
    cmd->size              = wlan_cpu_to_le16((sizeof(HostCmd_DS_REMAIN_ON_CHANNEL)) + S_DS_GEN);
    cmd->command           = wlan_cpu_to_le16(HostCmd_CMD_802_11_REMAIN_ON_CHANNEL);
    remain_channel->action = cmd_action;
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        if (cfg->remove != 0U)
        {
            remain_channel->action = HostCmd_ACT_GEN_REMOVE;
        }
        else
        {
            remain_channel->status        = 0;
            remain_channel->reserved      = 0;
            remain_channel->bandcfg       = cfg->bandcfg;
            remain_channel->channel       = cfg->channel;
            remain_channel->remain_period = wlan_cpu_to_le32(cfg->remain_period);
        }
    }
    remain_channel->action = wlan_cpu_to_le16(remain_channel->action);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#ifdef OTP_CHANINFO
/**
 *  @brief This function handles the command response of chan_region_cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_chan_region_cfg(IN pmlan_private pmpriv,
                                     IN HostCmd_DS_COMMAND *resp,
                                     IN mlan_ioctl_req *pioctl_buf)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 action;
    HostCmd_DS_CHAN_REGION_CFG *reg = MNULL;
    t_u8 *tlv_buf                   = MNULL;
    t_u16 tlv_buf_left;
    mlan_ds_misc_cfg *misc_cfg     = MNULL;
    mlan_ds_misc_chnrgpwr_cfg *cfg = MNULL;
    mlan_status ret                = MLAN_STATUS_SUCCESS;

    ENTER();

    reg = (HostCmd_DS_CHAN_REGION_CFG *)(void *)&resp->params;
    if (reg == MNULL)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    action = wlan_le16_to_cpu(reg->action);
    if (action != HostCmd_ACT_GEN_GET)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    tlv_buf      = (t_u8 *)reg + sizeof(*reg);
    tlv_buf_left = (t_u16)(wlan_le16_to_cpu(resp->size) - S_DS_GEN - sizeof(*reg));

    /* Add FW cfp tables and region info */
    wlan_add_fw_cfp_tables(pmpriv, tlv_buf, tlv_buf_left);

    if (pioctl_buf == MNULL)
    {
        goto done;
    }

    if (pioctl_buf->pbuf == MNULL)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }

    misc_cfg = (mlan_ds_misc_cfg *)(void *)pioctl_buf->pbuf;

    if (misc_cfg->sub_command == MLAN_OID_MISC_GET_REGIONPWR_CFG)
    {
        cfg         = (mlan_ds_misc_chnrgpwr_cfg *)&(misc_cfg->param.rgchnpwr_cfg);
        cfg->length = wlan_le16_to_cpu(resp->size);
        (void)__memcpy(pmpriv->adapter, cfg->chnrgpwr_buf, (t_u8 *)resp, cfg->length);
    }
    else
    {
        (void)__memset(pmpriv->adapter, &misc_cfg->param.custom_reg_domain, 0, sizeof(mlan_ds_custom_reg_domain));
        if (pmadapter->otp_region != MNULL)
        {
            (void)__memcpy(pmpriv->adapter, &misc_cfg->param.custom_reg_domain.region, pmadapter->otp_region,
                           sizeof(otp_region_info_t));
        }
        if (pmadapter->cfp_otp_bg != MNULL)
        {
            misc_cfg->param.custom_reg_domain.num_bg_chan = pmadapter->tx_power_table_bg_rows;
            (void)__memcpy(pmpriv->adapter, (t_u8 *)misc_cfg->param.custom_reg_domain.cfp_tbl,
                           (t_u8 *)pmadapter->cfp_otp_bg,
                           pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t));
        }
#if CONFIG_5GHz_SUPPORT
        if (pmadapter->cfp_otp_a != MNULL)
        {
            misc_cfg->param.custom_reg_domain.num_a_chan = pmadapter->tx_power_table_a_rows;
            (void)__memcpy(pmpriv->adapter,
                           (t_u8 *)misc_cfg->param.custom_reg_domain.cfp_tbl +
                               pmadapter->tx_power_table_bg_rows * sizeof(chan_freq_power_t),
                           (t_u8 *)pmadapter->cfp_otp_a, pmadapter->tx_power_table_a_rows * sizeof(chan_freq_power_t));
        }
#endif
    }
done:
    LEAVE();
    return ret;
}
#endif

#if CONFIG_COMPRESS_TX_PWTBL
mlan_status wlan_cmd_region_power_cfg(pmlan_private pmpriv,
                                      HostCmd_DS_COMMAND *cmd,
                                      t_u16 cmd_action,
                                      t_void *pdata_buf)
{
    t_u16 buf_len;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_REGION_POWER_CFG);
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        buf_len = cmd->size - S_DS_GEN;
        __memcpy(pmpriv->adapter, (t_u8 *)cmd + S_DS_GEN, pdata_buf, buf_len);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif


#if CONFIG_RX_ABORT_CFG
/**
 *  @brief This function sends rx abort cfg command to firmware.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          Hostcmd ID
 *  @param cmd_action   Command action
 *  @param pdata_buf    A void pointer to information buffer
 *  @return             N/A
 */
mlan_status wlan_cmd_rx_abort_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    HostCmd_DS_RX_ABORT_CFG *rx_abort_cfg = &cmd->params.rx_abort_cfg;
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    rx_abort_cfg_t *cfg = (rx_abort_cfg_t *)pdata_buf;

    ENTER();

    cmd->command         = wlan_cpu_to_le16(HostCmd_CMD_RX_ABORT_CFG);
    cmd->size            = wlan_cpu_to_le16(sizeof(HostCmd_DS_RX_ABORT_CFG) + S_DS_GEN);
    rx_abort_cfg->action = wlan_cpu_to_le16(cmd_action);

    if (rx_abort_cfg->action == HostCmd_ACT_GEN_SET)
    {
        rx_abort_cfg->enable         = cfg->enable;
        rx_abort_cfg->rssi_threshold = (t_s8)cfg->rssi_threshold;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_RX_ABORT_CFG_EXT
/**
 *  @brief This function sends rx abort cfg ext command to firmware.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          Hostcmd ID
 *  @param cmd_action   Command action
 *  @param pdata_buf    A void pointer to information buffer
 *  @return             N/A
 */
mlan_status wlan_cmd_rx_abort_cfg_ext(pmlan_private pmpriv,
                                      HostCmd_DS_COMMAND *cmd,
                                      t_u16 cmd_action,
                                      t_void *pdata_buf)
{
    HostCmd_DS_RX_ABORT_CFG_EXT *rx_abort_cfg_ext = (HostCmd_DS_RX_ABORT_CFG_EXT *)&cmd->params.rx_abort_cfg_ext;
    rx_abort_cfg_ext_t *cfg                       = (rx_abort_cfg_ext_t *)pdata_buf;

    ENTER();

    cmd->command             = wlan_cpu_to_le16(HostCmd_CMD_RX_ABORT_CFG_EXT);
    cmd->size                = wlan_cpu_to_le16(sizeof(HostCmd_DS_RX_ABORT_CFG_EXT) + S_DS_GEN);
    rx_abort_cfg_ext->action = wlan_cpu_to_le16(cmd_action);

    if (rx_abort_cfg_ext->action == HostCmd_ACT_GEN_SET)
    {
        rx_abort_cfg_ext->enable               = cfg->enable;
        rx_abort_cfg_ext->rssi_margin          = (t_s8)cfg->rssi_margin;
        rx_abort_cfg_ext->ceil_rssi_threshold  = (t_s8)cfg->ceil_rssi_threshold;
        rx_abort_cfg_ext->floor_rssi_threshold = (t_s8)cfg->floor_rssi_threshold;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_CCK_DESENSE_CFG
/**
 *  @brief This function sends cck desense cfg command to firmware.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          Hostcmd ID
 *  @param cmd_action   Command action
 *  @param pdata_buf    A void pointer to information buffer
 *  @return             N/A
 */
mlan_status wlan_cmd_cck_desense_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    HostCmd_DS_CCK_DESENSE_CFG *cfg_cmd = (HostCmd_DS_CCK_DESENSE_CFG *)&cmd->params.cck_desense_cfg;
    cck_desense_cfg_t *cfg              = (cck_desense_cfg_t *)pdata_buf;

    ENTER();

    cmd->command    = wlan_cpu_to_le16(HostCmd_CMD_CCK_DESENSE_CFG);
    cmd->size       = wlan_cpu_to_le16(sizeof(HostCmd_DS_CCK_DESENSE_CFG) + S_DS_GEN);
    cfg_cmd->action = wlan_cpu_to_le16(cmd_action);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        cfg_cmd->mode              = wlan_cpu_to_le16(cfg->mode);
        cfg_cmd->margin            = (t_s8)cfg->margin;
        cfg_cmd->ceil_thresh       = (t_s8)cfg->ceil_thresh;
        cfg_cmd->num_on_intervals  = (t_u8)cfg->num_on_intervals;
        cfg_cmd->num_off_intervals = (t_u8)cfg->num_off_intervals;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

#if CONFIG_WIFI_CLOCKSYNC
/**
 *  @brief This function prepares command of GPIO TSF LATCH.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req buf
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_gpio_tsf_latch(
    pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, mlan_ioctl_req *pioctl_buf, t_void *pdata_buf)
{
    HostCmd_DS_GPIO_TSF_LATCH_PARAM_CONFIG *gpio_tsf_config = &cmd->params.gpio_tsf_latch;
    mlan_ds_gpio_tsf_latch *cfg                             = (mlan_ds_gpio_tsf_latch *)pdata_buf;
    mlan_ds_misc_cfg *misc_cfg                              = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;

    mlan_ds_tsf_info *tsf_info                               = (mlan_ds_tsf_info *)pdata_buf;
    MrvlIEtypes_GPIO_TSF_LATCH_CONFIG *gpio_tsf_latch_config = MNULL;
    MrvlIEtypes_GPIO_TSF_LATCH_REPORT *gpio_tsf_latch_report = MNULL;
    t_u8 *tlv                                                = MNULL;
    ENTER();

    cmd->size               = sizeof(HostCmd_DS_GPIO_TSF_LATCH_PARAM_CONFIG) + S_DS_GEN;
    cmd->command            = wlan_cpu_to_le16(HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG);
    gpio_tsf_config->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        tlv = (t_u8 *)gpio_tsf_config->tlv_buf;
        if (misc_cfg->sub_command == (t_u32)MLAN_OID_MISC_GPIO_TSF_LATCH)
        {
            gpio_tsf_latch_config              = (MrvlIEtypes_GPIO_TSF_LATCH_CONFIG *)tlv;
            gpio_tsf_latch_config->header.type = wlan_cpu_to_le16(TLV_TYPE_GPIO_TSF_LATCH_CONFIG);
            gpio_tsf_latch_config->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG) - sizeof(MrvlIEtypesHeader_t));
            gpio_tsf_latch_config->clock_sync_mode              = cfg->clock_sync_mode;
            gpio_tsf_latch_config->clock_sync_Role              = cfg->clock_sync_Role;
            gpio_tsf_latch_config->clock_sync_gpio_pin_number   = cfg->clock_sync_gpio_pin_number;
            gpio_tsf_latch_config->clock_sync_gpio_level_toggle = cfg->clock_sync_gpio_level_toggle;
            gpio_tsf_latch_config->clock_sync_gpio_pulse_width  = wlan_cpu_to_le16(cfg->clock_sync_gpio_pulse_width);
            cmd->size += sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG);
            tlv += sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG);
            PRINTM(
                MCMND,
                "Set GPIO TSF latch config: \r\nMode=%d Role=%d, \r\nGPIO Pin Number=%d, \r\nGPIO level/toggle=%d GPIO "
                "pulse "
                "width=%d\n\r",
                cfg->clock_sync_mode, cfg->clock_sync_Role, cfg->clock_sync_gpio_pin_number,
                cfg->clock_sync_gpio_level_toggle, (int)cfg->clock_sync_gpio_pulse_width);
        }
    }
    else if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        tlv = (t_u8 *)gpio_tsf_config->tlv_buf;
        if (misc_cfg->sub_command == (t_u32)MLAN_OID_MISC_GPIO_TSF_LATCH)
        {
            gpio_tsf_latch_config              = (MrvlIEtypes_GPIO_TSF_LATCH_CONFIG *)tlv;
            gpio_tsf_latch_config->header.type = wlan_cpu_to_le16(TLV_TYPE_GPIO_TSF_LATCH_CONFIG);
            gpio_tsf_latch_config->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG) - sizeof(MrvlIEtypesHeader_t));
            cmd->size += sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG);
            tlv += sizeof(MrvlIEtypes_GPIO_TSF_LATCH_CONFIG);
        }

        if (misc_cfg->sub_command == (t_u32)MLAN_OID_MISC_GET_TSF_INFO)
        {
            gpio_tsf_latch_report = (MrvlIEtypes_GPIO_TSF_LATCH_REPORT *)tlv;
            (void)memset(gpio_tsf_latch_report, 0, sizeof(MrvlIEtypes_GPIO_TSF_LATCH_REPORT));
            gpio_tsf_latch_report->header.type = wlan_cpu_to_le16(TLV_TYPE_GPIO_TSF_LATCH_REPORT);
            gpio_tsf_latch_report->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_GPIO_TSF_LATCH_REPORT) - sizeof(MrvlIEtypesHeader_t));
            gpio_tsf_latch_report->tsf_format = wlan_cpu_to_le16(tsf_info->tsf_format);
            PRINTM(MCMND, "Get TSF info: format=%d\n\r", tsf_info->tsf_format);
            cmd->size += sizeof(MrvlIEtypes_GPIO_TSF_LATCH_REPORT);
        }
    }
    cmd->size = wlan_cpu_to_le16(cmd->size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of GPIO TSF Latch
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_gpio_tsf_latch(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_GPIO_TSF_LATCH_PARAM_CONFIG *gpio_tsf_config  = &resp->params.gpio_tsf_latch;
    mlan_ds_misc_cfg *cfg                                    = MNULL;
    MrvlIEtypes_GPIO_TSF_LATCH_CONFIG *gpio_tsf_latch_config = MNULL;
    MrvlIEtypes_GPIO_TSF_LATCH_REPORT *gpio_tsf_latch_report = MNULL;
    MrvlIEtypesHeader_t *tlv                                 = MNULL;
    t_u16 tlv_buf_left                                       = 0;
    t_u16 tlv_type                                           = 0;
    t_u16 tlv_len                                            = 0;

    ENTER();
    if (wlan_le16_to_cpu(gpio_tsf_config->action) == HostCmd_ACT_GEN_GET)
    {
        if (pioctl_buf)
        {
            cfg          = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;
            tlv          = (MrvlIEtypesHeader_t *)(gpio_tsf_config->tlv_buf);
            tlv_buf_left = resp->size - (sizeof(HostCmd_DS_GPIO_TSF_LATCH_PARAM_CONFIG) + S_DS_GEN);
            while (tlv_buf_left >= sizeof(MrvlIEtypesHeader_t))
            {
                tlv_type = wlan_le16_to_cpu(tlv->type);
                tlv_len  = wlan_le16_to_cpu(tlv->len);
                if (tlv_buf_left < (tlv_len + sizeof(MrvlIEtypesHeader_t)))
                {
                    PRINTM(MCMND, "Error processing gpio tsf latch config TLVs, bytes left < TLV length\n");
                    break;
                }
                switch (tlv_type)
                {
                    case TLV_TYPE_GPIO_TSF_LATCH_CONFIG:
                        if (cfg->sub_command == (t_u32)MLAN_OID_MISC_GPIO_TSF_LATCH)
                        {
                            gpio_tsf_latch_config                            = (MrvlIEtypes_GPIO_TSF_LATCH_CONFIG *)tlv;
                            cfg->param.gpio_tsf_latch_config.clock_sync_mode = gpio_tsf_latch_config->clock_sync_mode;
                            cfg->param.gpio_tsf_latch_config.clock_sync_Role = gpio_tsf_latch_config->clock_sync_Role;
                            cfg->param.gpio_tsf_latch_config.clock_sync_gpio_pin_number =
                                gpio_tsf_latch_config->clock_sync_gpio_pin_number;
                            cfg->param.gpio_tsf_latch_config.clock_sync_gpio_level_toggle =
                                gpio_tsf_latch_config->clock_sync_gpio_level_toggle;
                            cfg->param.gpio_tsf_latch_config.clock_sync_gpio_pulse_width =
                                wlan_le16_to_cpu(gpio_tsf_latch_config->clock_sync_gpio_pulse_width);
                            PRINTM(
                                MCMND,
                                "Get GPIO TSF latch config: Mode=%d Role=%d, GPIO Pin Number=%d, GPIO level/toggle=%d "
                                "GPIO pulse width=%d\n\r",
                                cfg->param.gpio_tsf_latch_config.clock_sync_mode,
                                cfg->param.gpio_tsf_latch_config.clock_sync_Role,
                                cfg->param.gpio_tsf_latch_config.clock_sync_gpio_pin_number,
                                cfg->param.gpio_tsf_latch_config.clock_sync_gpio_level_toggle,
                                (int)cfg->param.gpio_tsf_latch_config.clock_sync_gpio_pulse_width);
                        }
                        break;
                    case TLV_TYPE_GPIO_TSF_LATCH_REPORT:
                        if (cfg->sub_command == (t_u32)MLAN_OID_MISC_GET_TSF_INFO)
                        {
                            gpio_tsf_latch_report          = (MrvlIEtypes_GPIO_TSF_LATCH_REPORT *)tlv;
                            cfg->param.tsf_info.tsf_format = wlan_le16_to_cpu(gpio_tsf_latch_report->tsf_format);
                            cfg->param.tsf_info.tsf_info   = wlan_le16_to_cpu(gpio_tsf_latch_report->tsf_info);
                            cfg->param.tsf_info.tsf        = wlan_le64_to_cpu(gpio_tsf_latch_report->tsf);
                            cfg->param.tsf_info.tsf_offset = wlan_le16_to_cpu(gpio_tsf_latch_report->tsf_offset);
                            PRINTM(MCMND, "Get GPIO TSF latch report : format=%d\n info=%d tsf=%llu offset=%d\r\n",
                                   cfg->param.tsf_info.tsf_format, cfg->param.tsf_info.tsf_info,
                                   cfg->param.tsf_info.tsf, cfg->param.tsf_info.tsf_offset);
                        }
                        break;
                    default:
                        wifi_d("gpio tsf latch: Unknown tlv type");
                        break;
                }
                tlv_buf_left -= tlv_len + sizeof(MrvlIEtypesHeader_t);
                tlv = (MrvlIEtypesHeader_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
            }
            if (cfg->sub_command == (t_u32)MLAN_OID_MISC_GPIO_TSF_LATCH)
                pioctl_buf->data_read_written = sizeof(mlan_ds_gpio_tsf_latch);
            else if (cfg->sub_command == (t_u32)MLAN_OID_MISC_GET_TSF_INFO)
                pioctl_buf->data_read_written = sizeof(mlan_ds_tsf_info);
        }
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_MULTI_CHAN
/**
 *  @brief This function prepares the command MULTI_CHAN_CFG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   Command action: GET or SET
 *  @param pdata_buf    A pointer to new setting buf
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_multi_chan_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    mlan_ds_multi_chan_cfg *multi_chan_cfg = (mlan_ds_multi_chan_cfg *)pdata_buf;
    HostCmd_DS_MULTI_CHAN_CFG *pmchan_cfg  = (HostCmd_DS_MULTI_CHAN_CFG *)&cmd->params.multi_chan_cfg;

    ENTER();

    cmd->command       = wlan_cpu_to_le16(HostCmd_CMD_MULTI_CHAN_CONFIG);
    pmchan_cfg->action = wlan_cpu_to_le16(cmd_action);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        pmchan_cfg->buffer_weight = multi_chan_cfg->buffer_weight;
        pmchan_cfg->channel_time  = wlan_cpu_to_le32(multi_chan_cfg->channel_time);
        PRINTM(MCMND, "Set multi-channel: buffer_weight=%d channel_time=%d\n", multi_chan_cfg->buffer_weight,
               multi_chan_cfg->channel_time);
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_MULTI_CHAN_CFG));
    }
    else
    {
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(cmd_action));
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of MULTI_CHAN_CFG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_multi_chan_cfg(pmlan_private pmpriv, const HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *pcfg                     = MNULL;
    const HostCmd_DS_MULTI_CHAN_CFG *presp_cfg = &resp->params.multi_chan_cfg;

    ENTER();

    if (pioctl_buf)
    {
        pcfg                                     = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;
        pcfg->param.multi_chan_cfg.channel_time  = wlan_le32_to_cpu(presp_cfg->channel_time);
        pcfg->param.multi_chan_cfg.buffer_weight = presp_cfg->buffer_weight;
        pcfg->param.multi_chan_cfg.tlv_len = resp->size - (sizeof(HostCmd_DS_GEN) + sizeof(HostCmd_DS_MULTI_CHAN_CFG));
        PRINTM(MCMND, "Get multi-channel: buffer_weight=%d channel_time=%d tlv_len=%d\n",
               pcfg->param.multi_chan_cfg.buffer_weight, pcfg->param.multi_chan_cfg.channel_time,
               pcfg->param.multi_chan_cfg.tlv_len);
        __memcpy(pmpriv->adapter, pcfg->param.multi_chan_cfg.tlv_buf, presp_cfg->tlv_buf,
                 pcfg->param.multi_chan_cfg.tlv_len);
        pioctl_buf->buf_len = sizeof(mlan_ds_multi_chan_cfg) + pcfg->param.multi_chan_cfg.tlv_len;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares the command MULTI_CHAN_POLICY
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   Command action: GET or SET
 *  @param pdata_buf    A pointer to new setting buf
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_multi_chan_policy(pmlan_private pmpriv,
                                       HostCmd_DS_COMMAND *cmd,
                                       t_u16 cmd_action,
                                       t_void *pdata_buf)
{
    t_u16 policy                                     = 0;
    HostCmd_DS_MULTI_CHAN_POLICY *pmulti_chan_policy = (HostCmd_DS_MULTI_CHAN_POLICY *)&cmd->params.multi_chan_policy;

    ENTER();

    cmd->command               = wlan_cpu_to_le16(HostCmd_CMD_MULTI_CHAN_POLICY);
    pmulti_chan_policy->action = wlan_cpu_to_le16(cmd_action);
    cmd->size                  = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_MULTI_CHAN_POLICY));
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        policy                     = *((t_u16 *)pdata_buf);
        pmulti_chan_policy->policy = wlan_cpu_to_le16(policy);
        PRINTM(MCMND, "Set multi-channel policy: %d\n", policy);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of MULTI_CHAN_POLICY
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_multi_chan_policy(pmlan_private pmpriv, const HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *pcfg                        = MNULL;
    const HostCmd_DS_MULTI_CHAN_POLICY *presp_cfg = &resp->params.multi_chan_policy;

    ENTER();

    if (pioctl_buf)
    {
        pcfg                          = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;
        pcfg->param.multi_chan_policy = wlan_le16_to_cpu(presp_cfg->policy);

        if (pioctl_buf->action == HostCmd_ACT_GEN_SET)
        {
            if (pcfg->param.multi_chan_policy)
                pmpriv->adapter->mc_policy = MTRUE;
            else
                pmpriv->adapter->mc_policy = MFALSE;
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares the command DRCD_CFG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   Command action: GET or SET
 *  @param pdata_buf    A pointer to new setting buf
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_drcs_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    mlan_ds_drcs_cfg *drcs_cfg                      = (mlan_ds_drcs_cfg *)pdata_buf;
    HostCmd_DS_DRCS_CFG *pdrcs_cfg                  = (HostCmd_DS_DRCS_CFG *)&cmd->params.drcs_cfg;
    MrvlTypes_DrcsTimeSlice_t *channel_time_slicing = &pdrcs_cfg->time_slicing;

    ENTER();

    cmd->command      = wlan_cpu_to_le16(HostCmd_CMD_DRCS_CONFIG);
    pdrcs_cfg->action = wlan_cpu_to_le16(cmd_action);

    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        channel_time_slicing->header.type = wlan_cpu_to_le16(MRVL_DRCS_TIME_SLICE_TLV_ID);
        channel_time_slicing->header.len =
            wlan_cpu_to_le16(sizeof(MrvlTypes_DrcsTimeSlice_t) - sizeof(MrvlIEtypesHeader_t));
        channel_time_slicing->chan_idx   = wlan_cpu_to_le16(drcs_cfg->chan_idx);
        channel_time_slicing->chantime   = drcs_cfg->chantime;
        channel_time_slicing->switchtime = drcs_cfg->switchtime;
        channel_time_slicing->undozetime = drcs_cfg->undozetime;
        channel_time_slicing->mode       = drcs_cfg->mode;
        PRINTM(MCMND, "Set multi-channel: chan_idx=%d chantime=%d switchtime=%d undozetime=%d mode=%d\n",
               channel_time_slicing->chan_idx, channel_time_slicing->chantime, channel_time_slicing->switchtime,
               channel_time_slicing->undozetime, channel_time_slicing->mode);
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_DRCS_CFG));
        /* Set two channels different parameters */
        if (0x3 != channel_time_slicing->chan_idx)
        {
            drcs_cfg++;
            channel_time_slicing              = pdrcs_cfg->drcs_buf;
            channel_time_slicing->header.type = wlan_cpu_to_le16(MRVL_DRCS_TIME_SLICE_TLV_ID);
            channel_time_slicing->header.len =
                wlan_cpu_to_le16(sizeof(MrvlTypes_DrcsTimeSlice_t) - sizeof(MrvlIEtypesHeader_t));
            channel_time_slicing->chan_idx   = wlan_cpu_to_le16(drcs_cfg->chan_idx);
            channel_time_slicing->chantime   = drcs_cfg->chantime;
            channel_time_slicing->switchtime = drcs_cfg->switchtime;
            channel_time_slicing->undozetime = drcs_cfg->undozetime;
            channel_time_slicing->mode       = drcs_cfg->mode;
            PRINTM(MCMND, "Set multi-channel: chan_idx=%d chantime=%d switchtime=%d undozetime=%d mode=%d\n",
                   channel_time_slicing->chan_idx, channel_time_slicing->chantime, channel_time_slicing->switchtime,
                   channel_time_slicing->undozetime, channel_time_slicing->mode);
            cmd->size += wlan_cpu_to_le16(sizeof(MrvlTypes_DrcsTimeSlice_t));
        }
    }
    else
    {
        cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(cmd_action));
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of DRCS_CFG
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_drcs_cfg(pmlan_private pmpriv, const HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *pcfg                                 = MNULL;
    const HostCmd_DS_DRCS_CFG *presp_cfg                   = &resp->params.drcs_cfg;
    const MrvlTypes_DrcsTimeSlice_t *channel_time_slicing  = &presp_cfg->time_slicing;
    const MrvlTypes_DrcsTimeSlice_t *channel_time_slicing1 = MNULL;
    mlan_ds_drcs_cfg *drcs_cfg1                            = MNULL;

    ENTER();

    if (pioctl_buf)
    {
        pcfg = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;
        if (wlan_le16_to_cpu(channel_time_slicing->header.type) != MRVL_DRCS_TIME_SLICE_TLV_ID ||
            wlan_le16_to_cpu(channel_time_slicing->header.len) !=
                sizeof(MrvlTypes_DrcsTimeSlice_t) - sizeof(MrvlIEtypesHeader_t))
        {
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pcfg->param.drcs_cfg[0].chan_idx   = wlan_le16_to_cpu(channel_time_slicing->chan_idx);
        pcfg->param.drcs_cfg[0].chantime   = channel_time_slicing->chantime;
        pcfg->param.drcs_cfg[0].switchtime = channel_time_slicing->switchtime;
        pcfg->param.drcs_cfg[0].undozetime = channel_time_slicing->undozetime;
        pcfg->param.drcs_cfg[0].mode       = channel_time_slicing->mode;
        PRINTM(MCMND, "multi-channel: chan_idx=%d chantime=%d switchtime=%d undozetime=%d mode=%d\n",
               pcfg->param.drcs_cfg[0].chan_idx, channel_time_slicing->chantime, channel_time_slicing->switchtime,
               channel_time_slicing->undozetime, channel_time_slicing->mode);
        pioctl_buf->buf_len = sizeof(mlan_ds_drcs_cfg);
        /*Channel for chan_idx 1 and 2 have different parameters*/
        if (0x3 != pcfg->param.drcs_cfg[0].chan_idx)
        {
            channel_time_slicing1 = presp_cfg->drcs_buf;
            if (wlan_le16_to_cpu(channel_time_slicing1->header.type) != MRVL_DRCS_TIME_SLICE_TLV_ID ||
                wlan_le16_to_cpu(channel_time_slicing1->header.len) !=
                    sizeof(MrvlTypes_DrcsTimeSlice_t) - sizeof(MrvlIEtypesHeader_t))
            {
                LEAVE();
                return MLAN_STATUS_FAILURE;
            }
            drcs_cfg1             = (mlan_ds_drcs_cfg *)&pcfg->param.drcs_cfg[1];
            drcs_cfg1->chan_idx   = wlan_le16_to_cpu(channel_time_slicing1->chan_idx);
            drcs_cfg1->chantime   = channel_time_slicing1->chantime;
            drcs_cfg1->switchtime = channel_time_slicing1->switchtime;
            drcs_cfg1->undozetime = channel_time_slicing1->undozetime;
            drcs_cfg1->mode       = channel_time_slicing1->mode;
            PRINTM(MCMND, "multi-channel: chan_idx=%d chantime=%d switchtime=%d undozetime=%d mode=%d\n",
                   drcs_cfg1->chan_idx, drcs_cfg1->chantime, drcs_cfg1->switchtime, drcs_cfg1->undozetime,
                   drcs_cfg1->mode);
            pioctl_buf->buf_len += sizeof(mlan_ds_drcs_cfg);
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif


#if CONFIG_FW_VDLL

extern const unsigned char wlan_fw_bin[];
extern unsigned int wlan_fw_bin_len;

/**
 *  @brief This function download the vdll block.
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param block            A pointer to VDLL block
 *  @param block_len      The VDLL block length
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_download_vdll_block(mlan_adapter *pmadapter, t_u8 *block, t_u16 block_len)
{
    mlan_status status   = MLAN_STATUS_FAILURE;
    int ret              = -WM_FAIL;
    pvdll_dnld_ctrl ctrl = &pmadapter->vdll_ctrl;
    t_u16 msg_len        = block_len + sizeof(HostCmd_DS_GEN);
    HostCmd_DS_GEN *cmd_hdr;

    ENTER();

    if ((msg_len > WIFI_FW_CMDBUF_SIZE) || (ctrl == NULL))
    {
        wevt_d("VDLL block mem greater than cmd buf/vdll struct not inited");
        goto done;
    }

    cmd_hdr = (HostCmd_DS_GEN *)ctrl->cmd_buf;

    cmd_hdr->command = wlan_cpu_to_le16(HostCmd_CMD_VDLL);
    cmd_hdr->seq_num = wlan_cpu_to_le16(0xFF00);
    cmd_hdr->size    = wlan_cpu_to_le16(msg_len);

    (void)__memcpy(pmadapter, ctrl->cmd_buf + sizeof(HostCmd_DS_GEN), block, block_len);

#if CONFIG_FW_VDLL_DEBUG
    wevt_d("DNLD_VDLL : block_len=%d", block_len);
#endif

    ret = wifi_wait_for_vdllcmdresp(NULL);

    if (ret == -WM_FAIL)
    {
        wevt_d("DNLD_VDLL: Host to Card Failed");
    }
    else
    {
        status = MLAN_STATUS_SUCCESS;
    }

done:
    LEAVE();
    return status;
}

/**
 *  @brief The function Get the VDLL image from moal
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param offset          offset
 *
 *  @return             MLAN_STATUS_SUCCESS
 *
 */
static mlan_status wlan_get_vdll_image(pmlan_adapter pmadapter, t_u32 vdll_len)
{
    /*Since f/w is already in .h in RT so we will use the offsets directly*/

    vdll_dnld_ctrl *ctrl = &pmadapter->vdll_ctrl;
    ENTER();
    if (ctrl != NULL)
    {
        ctrl->vdll_mem = (t_u8 *)(pmadapter->fw_start_addr + (wlan_fw_bin_len - vdll_len));
        ctrl->vdll_len = vdll_len;
        ctrl->cmd_buf  = (t_u8 *)wifi_get_vdllcommand_buffer();
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handle the multi_chan info event
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pevent       A pointer to event buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_process_vdll_event(pmlan_private pmpriv, t_u8 *pevent)
{
    mlan_status status      = MLAN_STATUS_SUCCESS;
    vdll_ind *ind           = MNULL;
    t_u32 offset            = 0;
    t_u16 block_len         = 0;
    mlan_adapter *pmadapter = pmpriv->adapter;
    vdll_dnld_ctrl *ctrl    = &pmadapter->vdll_ctrl;

    ENTER();
    ind = (vdll_ind *)(pevent + sizeof(mlan_event_id));

    switch (wlan_le16_to_cpu(ind->type))
    {
        case VDLL_IND_TYPE_REQ:
            offset    = wlan_le32_to_cpu(ind->offset);
            block_len = wlan_le16_to_cpu(ind->block_len);
#if CONFIG_FW_VDLL_DEBUG
            wevt_d("VDLL_IND: type=%d offset = 0x%x, len = %d, vdll_len=0x%x", wlan_le16_to_cpu(ind->type), offset,
                   block_len, ctrl->vdll_len);
#endif
            if (offset <= ctrl->vdll_len)
            {
                block_len = MIN(block_len, ctrl->vdll_len - offset);
                status    = wlan_download_vdll_block(pmadapter, ctrl->vdll_mem + offset, block_len);
                if (status)
                {
                    wevt_d("Fail to download VDLL block");
                }
                if (pmadapter->vdll_in_progress == MFALSE)
                {
                    (void)pmadapter->callbacks.moal_start_timer(pmadapter->pmoal_handle, pmadapter->vdll_timer, MFALSE,
                                                                2000);
                    pmadapter->vdll_in_progress = MTRUE;
                }
                else
                {
                    (void)pmadapter->callbacks.moal_reset_timer(pmadapter->pmoal_handle, pmadapter->vdll_timer);
                }
            }
            else
            {
                wevt_d("Invalid VDLL req: offset=0x%x, len=%d, vdll_len=0x%x", offset, block_len, ctrl->vdll_len);
            }
            break;

        case VDLL_IND_TYPE_OFFSET:
            offset = wlan_le32_to_cpu(ind->offset);
#if CONFIG_FW_VDLL_DEBUG
            wevt_d("VDLL_IND (OFFSET): vdll_len=0x%x", offset);
#endif
            wlan_get_vdll_image(pmadapter, offset);
            break;
        case VDLL_IND_TYPE_ERR_SIG:
            wevt_d("VDLL_IND (SIG ERR).");
            break;
        case VDLL_IND_TYPE_ERR_ID:
            wevt_d("VDLL_IND (ID ERR).");
            break;
#if defined(SD9177) || defined(IW610)
        case VDLL_IND_TYPE_ERR_SECURE:
            wevt_d("VDLL_IND (SECURE ERR).");
            break;
        case VDLL_IND_TYPE_COMPLETE:
            wevt_d("VDLL_IND (ID COMPLETE).");
            break;
#elif defined(SD8978) || defined(SD8987) || defined(SD8997)
        case VDLL_IND_TYPE_INTF_RESET:
            wevt_d("VDLLV2_IND (INTF RESET).");
            sd_wifi_reset_ports();
            break;
#endif
        default:
            wevt_d("unknown vdll ind type=%d", ind->type);
            break;
    }
    LEAVE();
    return status;
}
#endif /* CONFIG_FW_VDLL */

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
/**
 *  @brief This function prepares command of independent reset.
 *
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_ind_rst_cfg(HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    mlan_ds_ind_rst_cfg *pdata_ind_rst            = (mlan_ds_ind_rst_cfg *)pdata_buf;
    HostCmd_DS_INDEPENDENT_RESET_CFG *ind_rst_cfg = (HostCmd_DS_INDEPENDENT_RESET_CFG *)&cmd->params.ind_rst_cfg;

    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_INDEPENDENT_RESET_CFG);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_INDEPENDENT_RESET_CFG) + S_DS_GEN);

    ind_rst_cfg->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        ind_rst_cfg->ir_mode  = pdata_ind_rst->ir_mode;
        ind_rst_cfg->gpio_pin = pdata_ind_rst->gpio_pin;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
/**
 *  @brief This function handles the command response of independent reset
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_ind_rst_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc                              = MNULL;
    const HostCmd_DS_INDEPENDENT_RESET_CFG *ind_rst_cfg = (HostCmd_DS_INDEPENDENT_RESET_CFG *)&resp->params.ind_rst_cfg;

    ENTER();

    if (pioctl_buf)
    {
        misc = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;

        if (wlan_le16_to_cpu(ind_rst_cfg->action) == HostCmd_ACT_GEN_GET)
        {
            misc->param.ind_rst_cfg.ir_mode  = ind_rst_cfg->ir_mode;
            misc->param.ind_rst_cfg.gpio_pin = ind_rst_cfg->gpio_pin;
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function sends boot sleep configure command to firmware.
 *
 *  @param pmpriv         A pointer to mlan_private structure
 *  @param cmd          Hostcmd ID
 *  @param cmd_action   Command action
 *  @param pdata_buf    A void pointer to information buffer
 *  @return             MLAN_STATUS_SUCCESS/ MLAN_STATUS_FAILURE
 */
mlan_status wlan_cmd_boot_sleep(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    HostCmd_DS_BOOT_SLEEP *boot_sleep = MNULL;
    t_u16 enable                      = *(t_u16 *)pdata_buf;

    ENTER();

    cmd->command       = wlan_cpu_to_le16(HostCmd_CMD_BOOT_SLEEP);
    boot_sleep         = &cmd->params.boot_sleep;
    boot_sleep->action = wlan_cpu_to_le16(cmd_action);
    boot_sleep->enable = wlan_cpu_to_le16(enable);

    cmd->size = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_BOOT_SLEEP));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of boot sleep cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_boot_sleep(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_BOOT_SLEEP *boot_sleep = &resp->params.boot_sleep;
    mlan_ds_misc_cfg *cfg             = (mlan_ds_misc_cfg *)pioctl_buf->pbuf;

    ENTER();

    cfg->param.boot_sleep = wlan_le16_to_cpu(boot_sleep->enable);
    PRINTM(MCMND, "boot sleep cfg status %u", cfg->param.boot_sleep);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of hs wakeup reason.
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_hs_wakeup_reason(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_void *pdata_buf)
{
    ENTER();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_HS_WAKEUP_REASON);
    cmd->size    = wlan_cpu_to_le16((sizeof(HostCmd_DS_HS_WAKEUP_REASON)) + S_DS_GEN);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of
 *          hs wakeup reason
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_hs_wakeup_reason(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_HS_WAKEUP_REASON *hs_wakeup_reason = (HostCmd_DS_HS_WAKEUP_REASON *)&resp->params.hs_wakeup_reason;
    mlan_ds_pm_cfg *pm_cfg                        = MNULL;

    ENTER();

    pm_cfg                                       = (mlan_ds_pm_cfg *)pioctl_buf->pbuf;
    pm_cfg->param.wakeup_reason.hs_wakeup_reason = wlan_le16_to_cpu(hs_wakeup_reason->wakeup_reason);
    pioctl_buf->data_read_written                = sizeof(mlan_ds_pm_cfg);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of TX_FRAME
 *
 *  @param pmpriv      A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_tx_frame(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    t_u16 cmd_size                          = 0;
    HostCmd_DS_80211_TX_FRAME *tx_frame_cmd = &cmd->params.tx_frame;
    mlan_ds_misc_tx_frame *tx_frame         = (mlan_ds_misc_tx_frame *)pdata_buf;
    TxPD *plocal_tx_pd                      = (TxPD *)tx_frame_cmd->buffer;
    t_u32 pkt_type                          = 0;
    t_u32 tx_control                        = 0;
    t_u8 *pdata                             = tx_frame->tx_buf;
    t_u16 data_len                          = tx_frame->data_len;

    ENTER();
    cmd->command         = wlan_cpu_to_le16(HostCmd_CMD_802_11_TX_FRAME);
    cmd_size             = sizeof(HostCmd_DS_80211_TX_FRAME) + S_DS_GEN;
    tx_frame_cmd->action = 0;
    tx_frame_cmd->status = 0;
    (void)__memcpy(pmpriv->adapter, &tx_frame_cmd->band_config, (t_u8 *)&tx_frame->bandcfg, sizeof(t_u8));
    tx_frame_cmd->channel = tx_frame->channel;

    if (tx_frame->buf_type == MLAN_BUF_TYPE_RAW_DATA)
    {
        (void)__memcpy(pmpriv->adapter, &pkt_type, tx_frame->tx_buf, sizeof(pkt_type));
        (void)__memcpy(pmpriv->adapter, &tx_control, tx_frame->tx_buf + sizeof(pkt_type), sizeof(tx_control));
        data_len -= sizeof(pkt_type) + sizeof(tx_control);
        pdata += sizeof(pkt_type) + sizeof(tx_control);
    }
    (void)__memcpy(pmpriv->adapter, tx_frame_cmd->buffer + sizeof(TxPD), pdata, data_len);

    (void)__memset(pmpriv->adapter, plocal_tx_pd, 0, sizeof(TxPD));
    plocal_tx_pd->bss_num       = GET_BSS_NUM(pmpriv);
    plocal_tx_pd->bss_type      = pmpriv->bss_type;
    plocal_tx_pd->tx_pkt_length = (t_u16)data_len;
    plocal_tx_pd->priority      = (t_u8)tx_frame->priority;
    plocal_tx_pd->tx_pkt_offset = sizeof(TxPD);
    plocal_tx_pd->pkt_delay_2ms = 0xff;

    if (tx_frame->buf_type == MLAN_BUF_TYPE_RAW_DATA)
    {
        plocal_tx_pd->tx_pkt_type = (t_u16)pkt_type;
        plocal_tx_pd->tx_control  = tx_control;
    }

    if (tx_frame->flags & MLAN_BUF_FLAG_TX_STATUS)
    {
#ifdef TXPD_RXPD_V3
        plocal_tx_pd->tx_control_1 |= tx_frame->tx_seq_num << 8;
#else
        plocal_tx_pd->tx_token_id = (t_u8)tx_frame->tx_seq_num;
#endif
        plocal_tx_pd->flags |= MRVDRV_TxPD_FLAGS_TX_PACKET_STATUS;
    }

    endian_convert_TxPD(plocal_tx_pd);
    cmd_size += sizeof(TxPD) + data_len;
    cmd->size = wlan_cpu_to_le16(cmd_size);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
