/** @file mlan_sta_cmdresp.c
 *
 *  @brief  This file provides the handling of command
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
#if CONFIG_RF_TEST_MODE

/**
 *  @brief This function prepares command resp of MFG Continuous Tx
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_mfg_tx_cont(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc           = MNULL;
    HostCmd_DS_MFG_CMD_TX_CONT *mcmd = (HostCmd_DS_MFG_CMD_TX_CONT *)&resp->params.mfg_tx_cont;
    mlan_ds_mfg_cmd_tx_cont *cfg     = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mlan_ds_mfg_cmd_tx_cont *)&misc->param.mfg_tx_cont;

    cfg->error           = wlan_le32_to_cpu(mcmd->error);
    cfg->enable_tx       = wlan_le32_to_cpu(mcmd->enable_tx);
    cfg->cw_mode         = wlan_le32_to_cpu(mcmd->cw_mode);
    cfg->payload_pattern = wlan_le32_to_cpu(mcmd->payload_pattern);
    cfg->cs_mode         = wlan_le32_to_cpu(mcmd->cs_mode);
    cfg->act_sub_ch      = wlan_le32_to_cpu(mcmd->act_sub_ch);
    cfg->tx_rate         = wlan_le32_to_cpu(mcmd->tx_rate);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG Tx frame
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_mfg_tx_frame(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc             = MNULL;
    HostCmd_DS_MFG_CMD_TX_FRAME2 *mcmd = (HostCmd_DS_MFG_CMD_TX_FRAME2 *)&resp->params.mfg_tx_frame2;
    mlan_ds_mfg_cmd_tx_frame2 *cfg     = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mlan_ds_mfg_cmd_tx_frame2 *)&misc->param.mfg_tx_frame2;

    cfg->error             = wlan_le32_to_cpu(mcmd->error);
    cfg->enable            = wlan_le32_to_cpu(mcmd->enable);
    cfg->data_rate         = wlan_le32_to_cpu(mcmd->data_rate);
    cfg->frame_pattern     = wlan_le32_to_cpu(mcmd->frame_pattern);
    cfg->frame_length      = wlan_le32_to_cpu(mcmd->frame_length);
    cfg->adjust_burst_sifs = wlan_le16_to_cpu(mcmd->adjust_burst_sifs);
    cfg->burst_sifs_in_us  = wlan_le32_to_cpu(mcmd->burst_sifs_in_us);
    cfg->short_preamble    = wlan_le32_to_cpu(mcmd->short_preamble);
    cfg->act_sub_ch        = wlan_le32_to_cpu(mcmd->act_sub_ch);
    cfg->short_gi          = wlan_le32_to_cpu(mcmd->short_gi);
    cfg->adv_coding        = wlan_le32_to_cpu(mcmd->adv_coding);
    cfg->tx_bf             = wlan_le32_to_cpu(mcmd->tx_bf);
    cfg->gf_mode           = wlan_le32_to_cpu(mcmd->gf_mode);
    cfg->stbc              = wlan_le32_to_cpu(mcmd->stbc);
    cfg->signal_bw         = wlan_le32_to_cpu(mcmd->signal_bw);
    cfg->NumPkt            = wlan_le32_to_cpu(mcmd->NumPkt);
    cfg->MaxPE             = wlan_le32_to_cpu(mcmd->MaxPE);
    cfg->BeamChange        = wlan_le32_to_cpu(mcmd->BeamChange);
    cfg->Dcm               = wlan_le32_to_cpu(mcmd->Dcm);
    cfg->Doppler           = wlan_le32_to_cpu(mcmd->Doppler);
    cfg->MidP              = wlan_le32_to_cpu(mcmd->MidP);
    cfg->QNum              = wlan_le32_to_cpu(mcmd->QNum);
    memcpy(cfg->bssid, mcmd->bssid, sizeof(cfg->bssid));

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG HE TB Tx
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */

static mlan_status wlan_ret_mfg_he_tb_tx(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc             = MNULL;
    HostCmd_DS_MFG_CMD_HE_TBTX_T *mcmd = (HostCmd_DS_MFG_CMD_HE_TBTX_T *)&resp->params.mfg_he_power;
    mlan_ds_mfg_Cmd_HE_TBTx_t *cfg     = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mlan_ds_mfg_Cmd_HE_TBTx_t *)&misc->param.mfg_he_power;

    cfg->enable       = wlan_le16_to_cpu(mcmd->enable);
    cfg->qnum         = wlan_le16_to_cpu(mcmd->qnum);
    cfg->aid          = wlan_le16_to_cpu(mcmd->aid);
    cfg->axq_mu_timer = wlan_le16_to_cpu(mcmd->axq_mu_timer);
    cfg->tx_power     = wlan_le16_to_cpu(mcmd->tx_power);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG OTP MAC add
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */

static mlan_status wlan_ret_mfg_otp_mac_add(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc                 = MNULL;
    HostCmd_DS_MFG_CMD_OTP_MAC_ADD_T *mcmd = (HostCmd_DS_MFG_CMD_OTP_MAC_ADD_T *)&resp->params.mfg_otp_mac_addr_rd_wr;
    mlan_ds_mfg_cmd_otp_mac_addr_rd_wr_t *cfg = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mlan_ds_mfg_cmd_otp_mac_addr_rd_wr_t *)&misc->param.mfg_otp_mac_addr_rd_wr;

    memcpy(cfg->mac_addr, mcmd->mac_addr, MLAN_MAC_ADDR_LENGTH);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG OTP cal data
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */

static mlan_status wlan_ret_mfg_otp_cal_data(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc                  = MNULL;
    HostCmd_DS_MFG_CMD_OTP_CAL_DATA_T *mcmd = (HostCmd_DS_MFG_CMD_OTP_CAL_DATA_T *)&resp->params.mfg_otp_cal_data_rd_wr;
    mlan_ds_mfg_cmd_otp_cal_data_rd_wr_t *cfg = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mlan_ds_mfg_cmd_otp_cal_data_rd_wr_t *)&misc->param.mfg_otp_cal_data_rd_wr;

    cfg->cal_data_status = mcmd->cal_data_status;
    cfg->cal_data_len    = mcmd->cal_data_len;
    memcpy(cfg->cal_data, mcmd->cal_data, mcmd->cal_data_len);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG config Trigger frame
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_mfg_config_trigger_frame(pmlan_private pmpriv,
                                                     HostCmd_DS_COMMAND *resp,
                                                     mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_misc_cfg *misc = MNULL;
    HostCmd_MFG_CMD_IEEETYPES_CTLBASICTRIGHDR_T *mcmd =
        (HostCmd_MFG_CMD_IEEETYPES_CTLBASICTRIGHDR_T *)&resp->params.mfg_tx_trigger_config;
    mfg_Cmd_IEEEtypes_CtlBasicTrigHdr_t *cfg = MNULL;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    misc = (mlan_ds_misc_cfg *)pioctl_buf;
    cfg  = (mfg_Cmd_IEEEtypes_CtlBasicTrigHdr_t *)&misc->param.mfg_tx_trigger_config;

    cfg->enable_tx       = wlan_le32_to_cpu(mcmd->enable_tx);
    cfg->standalone_hetb = wlan_le32_to_cpu(mcmd->standalone_hetb);
    cfg->frmCtl.type     = wlan_le16_to_cpu(mcmd->frmCtl.type);
    cfg->frmCtl.sub_type = wlan_le16_to_cpu(mcmd->frmCtl.sub_type);
    cfg->duration        = wlan_le16_to_cpu(mcmd->duration);

    cfg->trig_common_field = wlan_le64_to_cpu(mcmd->trig_common_field);

    cfg->trig_user_info_field = wlan_le64_to_cpu(mcmd->trig_user_info_field);

    // memcpy_ext(pmpriv->adapter, &cfg->trig_user_info_field, &mcmd->trig_user_info_field,
    //      sizeof(mcmd->trig_user_info_field), sizeof(cfg->trig_user_info_field));

    cfg->basic_trig_user_info = wlan_le16_to_cpu(mcmd->basic_trig_user_info);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command resp of MFG Cmd
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_mfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, void *pioctl_buf)
{
    HostCmd_DS_MFG_CMD_GENERIC_CFG *mcmd = (HostCmd_DS_MFG_CMD_GENERIC_CFG *)&resp->params.mfg_generic_cfg;
    mlan_ds_misc_cfg *misc_cfg           = (mlan_ds_misc_cfg *)pioctl_buf;
    mlan_ds_mfg_cmd_generic_cfg *cfg     = MNULL;
    mlan_status ret                      = MLAN_STATUS_SUCCESS;

    ENTER();
    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    switch (wlan_le32_to_cpu(mcmd->mfg_cmd))
    {
        case MFG_CMD_TX_CONT:
            ret = wlan_ret_mfg_tx_cont(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_TX_FRAME:
            ret = wlan_ret_mfg_tx_frame(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_CONFIG_MAC_HE_TB_TX:
            ret = wlan_ret_mfg_he_tb_tx(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_CONFIG_TRIGGER_FRAME:
            ret = wlan_ret_mfg_config_trigger_frame(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_OTP_MAC_ADD:
            ret = wlan_ret_mfg_otp_mac_add(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_OTP_CAL_DATA:
            ret = wlan_ret_mfg_otp_cal_data(pmpriv, resp, pioctl_buf);
            goto cmd_mfg_done;
        case MFG_CMD_SET_TEST_MODE:
        case MFG_CMD_UNSET_TEST_MODE:
        case MFG_CMD_TX_ANT:
        case MFG_CMD_RX_ANT:
        case MFG_CMD_RF_CHAN:
        case MFG_CMD_CLR_RX_ERR:
        case MFG_CMD_RF_BAND_AG:
        case MFG_CMD_RF_CHANNELBW:
        case MFG_CMD_RADIO_MODE_CFG:
        case MFG_CMD_RFPWR:
            break;
        default:
            ret = MLAN_STATUS_FAILURE;
            goto cmd_mfg_done;
    }
    cfg = (mlan_ds_mfg_cmd_generic_cfg *)&(misc_cfg->param);

    cfg->error = wlan_le32_to_cpu(mcmd->error);
    cfg->data1 = wlan_le32_to_cpu(mcmd->data1);
    cfg->data2 = wlan_le32_to_cpu(mcmd->data2);
    cfg->data3 = wlan_le32_to_cpu(mcmd->data3);
cmd_mfg_done:
    LEAVE();
    return ret;
}
#endif


/**
 *  @brief This function handles the command response of snmp_mib
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_802_11_snmp_mib(IN pmlan_private pmpriv,
                                            IN HostCmd_DS_COMMAND *resp,
                                            IN mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psmib = &resp->params.smib;
    t_u16 oid                         = wlan_le16_to_cpu(psmib->oid);
    t_u16 query_type                  = wlan_le16_to_cpu(psmib->query_type);
    t_u32 ul_temp;

    mlan_ds_snmp_mib *mib = MNULL;

    ENTER();

    if (pioctl_buf != MNULL)
    {
        mib = (mlan_ds_snmp_mib *)(void *)pioctl_buf->pbuf;
    }

    /* wmsdk */
    PRINTM(MINFO, "SNMP_RESP: value of the oid = 0x%x, query_type=0x%x\n", oid, query_type);
    PRINTM(MINFO, "SNMP_RESP: Buf size  = 0x%x\n", wlan_le16_to_cpu(psmib->buf_size));

    if (query_type == HostCmd_ACT_GEN_GET)
    {
        /* wmsdk: GET is not used. Disable */
        switch (oid)
        {
            case DtimPeriod_i:
                ul_temp = psmib->value[0];
                PRINTM(MINFO, "SNMP_RESP: DTIM Period =%u\n", ul_temp);
                if (mib != MNULL)
                {
                    mib->param.dtim_period = ul_temp;
                }
                break;
#if CONFIG_WIFI_FRAG_THRESHOLD
            case FragThresh_i:
                ul_temp = wlan_le16_to_cpu(*((t_u16 *)(psmib->value)));
                PRINTM(MINFO, "SNMP_RESP: FragThsd =%u\n", ul_temp);
                if (mib)
                {
                    mib->param.frag_threshold = ul_temp;
                }
                break;
#endif
#if CONFIG_WIFI_RTS_THRESHOLD
            case RtsThresh_i:
                ul_temp = wlan_le16_to_cpu(*((t_u16 *)(psmib->value)));
                PRINTM(MINFO, "SNMP_RESP: RTSThsd =%u\n", ul_temp);
                if (mib)
                {
                    mib->param.rts_threshold = ul_temp;
                }
                break;
#endif
            default:
                PRINTM(MINFO, "Unexpected snmp_mib oid\n");
                break;
        }
    }
    else
    { /* (query_type == HostCmd_ACT_GEN_SET) */
        /* Update state for 11d */
        if (oid == Dot11D_i)
        {
            ul_temp = wlan_le16_to_cpu(*((t_u16 *)(void *)(psmib->value)));
            /* Set 11d state to private */
            pmpriv->state_11d.enable_11d = (state_11d_t)ul_temp;
            /* Set user enable flag if called from ioctl */
            if (pioctl_buf != NULL)
            {
                pmpriv->state_11d.user_enable_11d = (state_11d_t)ul_temp;
            }
        }
        /* Update state for 11h */
        if (oid == Dot11H_i)
        {
            ul_temp = wlan_le16_to_cpu(*((t_u16 *)(void *)(psmib->value)));
            /* Set 11h state to priv */
            pmpriv->intf_state_11h.is_11h_active = (ul_temp & ENABLE_11H_MASK) ? MTRUE : MFALSE;
            /* Set radar_det state to adapter */
            pmpriv->adapter->state_11h.is_master_radar_det_active = (ul_temp & MASTER_RADAR_DET_MASK) ? MTRUE : MFALSE;
            pmpriv->adapter->state_11h.is_slave_radar_det_active  = (ul_temp & SLAVE_RADAR_DET_MASK) ? MTRUE : MFALSE;
        }
    }

    if (pioctl_buf != NULL)
    {
        /* Indicate ioctl complete */
        pioctl_buf->data_read_written = sizeof(mlan_ds_snmp_mib);
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of get_log
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_get_log(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_802_11_GET_LOG *pget_log = (HostCmd_DS_802_11_GET_LOG *)&resp->params.get_log;
    mlan_ds_get_info *pget_info         = MNULL;

    ENTER();
    if (pioctl_buf != NULL)
    {
        pget_info                               = (mlan_ds_get_info *)(void *)pioctl_buf->pbuf;
        pget_info->param.stats.mcast_tx_frame   = wlan_le32_to_cpu(pget_log->mcast_tx_frame);
        pget_info->param.stats.failed           = wlan_le32_to_cpu(pget_log->failed);
        pget_info->param.stats.retry            = wlan_le32_to_cpu(pget_log->retry);
        pget_info->param.stats.multi_retry      = wlan_le32_to_cpu(pget_log->multiretry);
        pget_info->param.stats.frame_dup        = wlan_le32_to_cpu(pget_log->frame_dup);
        pget_info->param.stats.rts_success      = wlan_le32_to_cpu(pget_log->rts_success);
        pget_info->param.stats.rts_failure      = wlan_le32_to_cpu(pget_log->rts_failure);
        pget_info->param.stats.ack_failure      = wlan_le32_to_cpu(pget_log->ack_failure);
        pget_info->param.stats.rx_frag          = wlan_le32_to_cpu(pget_log->rx_frag);
        pget_info->param.stats.mcast_rx_frame   = wlan_le32_to_cpu(pget_log->mcast_rx_frame);
        pget_info->param.stats.fcs_error        = wlan_le32_to_cpu(pget_log->fcs_error);
        pget_info->param.stats.tx_frame         = wlan_le32_to_cpu(pget_log->tx_frame);
        pget_info->param.stats.wep_icv_error[0] = wlan_le32_to_cpu(pget_log->wep_icv_err_cnt[0]);
        pget_info->param.stats.wep_icv_error[1] = wlan_le32_to_cpu(pget_log->wep_icv_err_cnt[1]);
        pget_info->param.stats.wep_icv_error[2] = wlan_le32_to_cpu(pget_log->wep_icv_err_cnt[2]);
        pget_info->param.stats.wep_icv_error[3] = wlan_le32_to_cpu(pget_log->wep_icv_err_cnt[3]);
        /* Indicate ioctl complete */
        pioctl_buf->data_read_written = sizeof(mlan_ds_get_info);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get power level and rate index
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pdata_buf    Pointer to the data buffer
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_get_power_level(pmlan_private pmpriv, void *pdata_buf)
{
    int length = -1, max_power = -1, min_power = -1;
    MrvlTypes_Power_Group_t *ppg_tlv = MNULL;
    Power_Group_t *pg                = MNULL;

    ENTER();

    if (pdata_buf != NULL)
    {
        ppg_tlv = (MrvlTypes_Power_Group_t *)(void *)((t_u8 *)pdata_buf + sizeof(HostCmd_DS_TXPWR_CFG));
        pg      = (Power_Group_t *)(void *)((t_u8 *)ppg_tlv + sizeof(MrvlTypes_Power_Group_t));
        length  = (int)ppg_tlv->length;
        if (length > 0)
        {
            max_power = (int)pg->power_max;
            min_power = (int)pg->power_min;
            length -= (int)sizeof(Power_Group_t);
        }
        while (length > 0)
        {
            pg++;
            if (max_power < pg->power_max)
            {
                max_power = (int)pg->power_max;
            }
            if (min_power > pg->power_min)
            {
                min_power = (int)pg->power_min;
            }
            length -= (int)sizeof(Power_Group_t);
        }
        if (ppg_tlv->length > 0U)
        {
            pmpriv->min_tx_power_level = (t_u8)min_power;
            pmpriv->max_tx_power_level = (t_u8)max_power;
        }
    }
    else
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
/**
 *  @brief This function handles the command response of tx_power_cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_tx_power_cfg(IN pmlan_private pmpriv,
                                         IN HostCmd_DS_COMMAND *resp,
                                         IN mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_TXPWR_CFG *ptxp_cfg   = &resp->params.txp_cfg;
    MrvlTypes_Power_Group_t *ppg_tlv = MNULL;
    Power_Group_t *pg                = MNULL;
    t_u16 action                     = wlan_le16_to_cpu(ptxp_cfg->action);
    mlan_ds_power_cfg *power         = MNULL;
    t_u32 data[5];
    bool invalid_hostcmd = MFALSE;

    ENTER();

    ppg_tlv = (MrvlTypes_Power_Group_t *)(void *)((t_u8 *)&resp->params + sizeof(HostCmd_DS_TXPWR_CFG));
    pg      = (Power_Group_t *)(void *)((t_u8 *)ppg_tlv + sizeof(MrvlTypes_Power_Group_t));

    switch (action)
    {
        case HostCmd_ACT_GEN_GET:
            ppg_tlv->length = wlan_le16_to_cpu(ppg_tlv->length);
            if (pmpriv->adapter->hw_status == WlanHardwareStatusInitializing)
            {
                // coverity[overrun-buffer-val:SUPPRESS]
                (void)wlan_get_power_level(pmpriv, ptxp_cfg);
            }
            pmpriv->tx_power_level = (t_u16)pg->power_min;
            PRINTM(MMSG, "The Sta tx power level: %d\r\n", pmpriv->tx_power_level);
            break;

        case HostCmd_ACT_GEN_SET:
            if (wlan_le32_to_cpu(ptxp_cfg->mode) != 0U)
            {
                if (pg->power_max == pg->power_min)
                {
                    pmpriv->tx_power_level = (t_u16)pg->power_min;
                }
            }
            break;
        default:
            PRINTM(MERROR, "CMD_RESP: unknown command action %d\n", action);
            invalid_hostcmd = MTRUE;
            break;
    }
    if (invalid_hostcmd == MTRUE)
    {
        LEAVE();
        return MLAN_STATUS_SUCCESS;
    }

    PRINTM(MINFO, "Current TxPower Level = %d,Max Power=%d, Min Power=%d\n", pmpriv->tx_power_level,
           pmpriv->max_tx_power_level, pmpriv->min_tx_power_level);

    if (pioctl_buf != MNULL)
    {
        power = (mlan_ds_power_cfg *)(void *)pioctl_buf->pbuf;
        if (action == HostCmd_ACT_GEN_GET)
        {
            if (power->sub_command == MLAN_OID_POWER_CFG)
            {
                pioctl_buf->data_read_written      = sizeof(mlan_power_cfg_t) + MLAN_SUB_COMMAND_SIZE;
                power->param.power_cfg.power_level = pmpriv->tx_power_level;
                if (wlan_le32_to_cpu(ptxp_cfg->mode) != 0U)
                {
                    power->param.power_cfg.is_power_auto = MFALSE;
                }
                else
                {
                    power->param.power_cfg.is_power_auto = MTRUE;
                }
            }
            else
            {
                power->param.power_ext.len = 0;
                while (ppg_tlv->length != 0U)
                {
                    data[0] = pg->first_rate_code;
                    data[1] = pg->last_rate_code;
                    if (pg->modulation_class == MOD_CLASS_OFDM)
                    {
                        data[0] += MLAN_RATE_INDEX_OFDM0;
                        data[1] += MLAN_RATE_INDEX_OFDM0;
                    }
                    else if (pg->modulation_class == MOD_CLASS_HT)
                    {
                        data[0] += MLAN_RATE_INDEX_MCS0;
                        data[1] += MLAN_RATE_INDEX_MCS0;
                        if (pg->ht_bandwidth == HT_BW_40)
                        {
                            data[0] |= TX_RATE_HT_BW40_BIT;
                            data[1] |= TX_RATE_HT_BW40_BIT;
                        }
                    }
                    else
                    {
                        /* Do Nothing */
                    }
                    data[2] = (t_u32)pg->power_min;
                    data[3] = (t_u32)pg->power_max;
                    data[4] = (t_u32)pg->power_step;
                    (void)__memcpy(pmpriv->adapter,
                                   (t_u8 *)(&power->param.power_ext.power_data[power->param.power_ext.len]),
                                   (t_u8 *)data, sizeof(data));
                    power->param.power_ext.len += 5U;
                    pg++;
                    ppg_tlv->length -= (t_u16)sizeof(Power_Group_t);
                }
                pioctl_buf->data_read_written = sizeof(mlan_power_cfg_ext) + MLAN_SUB_COMMAND_SIZE;
            }
        }
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#if CONFIG_WMM_UAPSD
/**
 *  @brief This function handles the command response of sleep_period
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_802_11_sleep_period(IN pmlan_private pmpriv,
                                                IN HostCmd_DS_COMMAND *resp,
                                                IN mlan_ioctl_req *pioctl_buf)
{
    HostCmd_DS_802_11_SLEEP_PERIOD *pcmd_sleep_pd = &resp->params.sleep_pd;
    mlan_ds_pm_cfg *pm_cfg                        = MNULL;
    t_u16 sleep_pd                                = 0;

    ENTER();

    sleep_pd = wlan_le16_to_cpu(pcmd_sleep_pd->sleep_pd);
    if (pioctl_buf)
    {
        pm_cfg                        = (mlan_ds_pm_cfg *)pioctl_buf->pbuf;
        pm_cfg->param.sleep_period    = (t_u32)sleep_pd;
        pioctl_buf->data_read_written = sizeof(pm_cfg->param.sleep_period) + MLAN_SUB_COMMAND_SIZE;
    }
    pmpriv->adapter->sleep_period.period = sleep_pd;

    pmpriv->adapter->pps_uapsd_mode = MFALSE;
    pmpriv->adapter->tx_lock_flag   = MFALSE;
    if ((pmpriv->adapter->sleep_period.period != 0) &&
        (pmpriv->adapter->sleep_period.period != SLEEP_PERIOD_RESERVED_FF))
    {
        pmpriv->adapter->gen_null_pkt = MTRUE;
    }
    else
    {
        pmpriv->adapter->gen_null_pkt = MFALSE;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function handles the command response of deauthenticate
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_802_11_deauthenticate(IN pmlan_private pmpriv,
                                           IN HostCmd_DS_COMMAND *resp,
                                           IN mlan_ioctl_req *pioctl_buf)
{
    ENTER();
        wlan_reset_connect_state(pmpriv, MTRUE);


    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


/**
 *  @brief This function handles the command response of rf_channel
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_802_11_rf_channel(IN pmlan_private pmpriv,
                                              IN HostCmd_DS_COMMAND *resp,
                                              IN mlan_ioctl_req *pioctl_buf)
{
    ENTER();
    /* fixme: enable this part when needed */
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}



#if (CONFIG_SUBSCRIBE_EVENT_SUPPORT)
/**
 *  @brief This function handles the command response of
 *  subscribe event
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to command buffer
 *
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_ret_subscribe_event(IN pmlan_private pmpriv,
                                            IN HostCmd_DS_COMMAND *resp,
                                            IN mlan_ioctl_req *sub_evt)
{
    ENTER();
    if (sub_evt && wlan_parse_getdata(resp, (mlan_ds_subscribe_evt *)sub_evt) != WM_SUCCESS)
    {
        wevt_w("get subscribe event fail\n");
        return MLAN_STATUS_FAILURE;
    }
    LEAVE();

    return MLAN_STATUS_SUCCESS;
}
#endif


/********************************************************
                Global Functions
********************************************************/
/**
 *  @brief This function handles the station command response
 *
 *  @param priv             A pointer to mlan_private structure
 *  @param cmdresp_no       cmd no
 *  @param pcmd_buf         cmdresp buf
 *  @param pioctl           A pointer to ioctl buf
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ops_sta_process_cmdresp(IN t_void *priv, IN t_u16 cmdresp_no, IN t_void *pcmd_buf, IN t_void *pioctl)
{
    mlan_status ret            = MLAN_STATUS_SUCCESS;
    mlan_private *pmpriv       = (mlan_private *)priv;
    HostCmd_DS_COMMAND *resp   = (HostCmd_DS_COMMAND *)pcmd_buf;
    mlan_ioctl_req *pioctl_buf = (mlan_ioctl_req *)pioctl;
    /* mlan_adapter *pmadapter = pmpriv->adapter; */
    /* int ctr; */

    ENTER();

    /* fixme: enable below code when required */
    /* Command successful, handle response */
    switch (cmdresp_no)
    {
        case HostCmd_CMD_GET_HW_SPEC:
            ret = wlan_ret_get_hw_spec(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_TXPWR_CFG:
            ret = wlan_ret_tx_power_cfg(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_TX_RATE_CFG:
            ret = wlan_ret_tx_rate_cfg(pmpriv, resp, pioctl_buf);
            break;
#if !CONFIG_EXT_SCAN_SUPPORT
        case HostCmd_CMD_802_11_SCAN:
            ret        = wlan_ret_802_11_scan(pmpriv, resp, pioctl_buf);
            pioctl_buf = MNULL;
            /* pmadapter->curr_cmd->pioctl_buf = MNULL; */
            break;
#else
        case HostCmd_CMD_802_11_SCAN_EXT:
            ret        = wlan_ret_802_11_scan_ext(pmpriv, resp, pioctl_buf);
            pioctl_buf = MNULL;
            break;
#endif /* CONFIG_EXT_SCAN_SUPPORT */
#if CONFIG_WMM_UAPSD
        case HostCmd_CMD_802_11_SLEEP_PERIOD:
            ret = wlan_ret_802_11_sleep_period(pmpriv, resp, pioctl_buf);
            break;
#endif
        case HostCmd_CMD_802_11_ASSOCIATE:
            ret = wlan_ret_802_11_associate(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_802_11_GET_LOG:
            ret = wlan_ret_get_log(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_802_11_SNMP_MIB:
            ret = wlan_ret_802_11_snmp_mib(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_802_11_TX_RATE_QUERY:
            ret = wlan_ret_802_11_tx_rate_query(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_802_11_RF_CHANNEL:
            ret = wlan_ret_802_11_rf_channel(pmpriv, resp, pioctl_buf);
            break;
#if CONFIG_WMM
        case HostCmd_CMD_WMM_PARAM_CONFIG:
            ret = wlan_ret_wmm_param_config(pmpriv, resp, pioctl_buf);
            break;
#endif
#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
        case HostCmd_CMD_802_11_SUBSCRIBE_EVENT:
            ret = wlan_ret_subscribe_event(pmpriv, resp, pioctl_buf);
            break;
#endif
#if (CONFIG_BG_SCAN)
        case HostCmd_CMD_802_11_BG_SCAN_QUERY:
            ret = wlan_ret_802_11_scan(pmpriv, resp, pioctl_buf);
            PRINTM(MINFO, "CMD_RESP: BG_SCAN result is ready!\n");
            break;
#endif
#if CONFIG_RF_TEST_MODE
        case HostCmd_CMD_MFG_COMMAND:
            ret = wlan_ret_mfg(pmpriv, resp, pioctl_buf);
            break;
#endif
#ifdef OTP_CHANINFO
        case HostCmd_CMD_CHAN_REGION_CFG:
            ret = wlan_ret_chan_region_cfg(pmpriv, resp, pioctl_buf);
            break;
#endif
        case HostCmd_CMD_BOOT_SLEEP:
            ret = wlan_ret_boot_sleep(pmpriv, resp, pioctl_buf);
            break;
#if CONFIG_11AX
        case HostCmd_CMD_11AX_CMD:
            ret = wlan_ret_11ax_cmd(pmpriv, resp, pioctl_buf);
            break;
#endif
#if CONFIG_WIFI_CLOCKSYNC
        case HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG:
            ret = wlan_ret_gpio_tsf_latch(pmpriv, resp, pioctl_buf);
            break;
#endif /* CONFIG_WIFI_CLOCKSYNC */
#if CONFIG_MULTI_CHAN
        case HostCmd_CMD_MULTI_CHAN_CONFIG:
            ret = wlan_ret_multi_chan_cfg(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_MULTI_CHAN_POLICY:
            ret = wlan_ret_multi_chan_policy(pmpriv, resp, pioctl_buf);
            break;
        case HostCmd_CMD_DRCS_CONFIG:
            ret = wlan_ret_drcs_cfg(pmpriv, resp, pioctl_buf);
            break;
#endif
#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
        case HostCmd_CMD_INDEPENDENT_RESET_CFG:
            ret = wlan_ret_ind_rst_cfg(pmpriv, resp, pioctl_buf);
            break;
#endif
        default:
            PRINTM(MERROR, "CMD_RESP: Unknown command response %#x\n", resp->command);
            break;
    }

    LEAVE();
    return ret;
}
