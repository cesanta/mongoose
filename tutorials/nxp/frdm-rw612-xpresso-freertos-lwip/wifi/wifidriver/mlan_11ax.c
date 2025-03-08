/** @file mlan_11ax.c
 *
 *  @brief This file defines the private and adapter data
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright 2021-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

#if CONFIG_11AX

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
/********************************************************
 *    Local Variables
 *    ********************************************************/

static t_u8 user_he_cap_band;
/********************************************************
 *    Global Variables
 *    ********************************************************/

/********************************************************
 *    Local Functions
 *    *******************************************************/

/**
 *  @brief This function check if AP support TWT Response.
 *
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *
 *  @return        MTRUE/MFALSE
 */
t_u8 wlan_check_ap_11ax_twt_supported(BSSDescriptor_t *pbss_desc)
{
    if (!pbss_desc->phe_cap)
        return MFALSE;
    if (!(pbss_desc->phe_cap->he_mac_cap[0] & HE_MAC_CAP_TWT_RESP_SUPPORT))
        return MFALSE;
    if (!pbss_desc->pext_cap)
        return MFALSE;
    if (!ISSUPP_EXTCAP_EXT_TWT_RESP(pbss_desc->pext_cap->ext_cap))
        return MFALSE;
    return MTRUE;
}

/**
 *  @brief This function check if we should enable TWT support
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *
 *  @return        MTRUE/MFALSE
 */
t_u8 wlan_check_11ax_twt_supported(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
#if CONFIG_5GHz_SUPPORT
    MrvlIEtypes_He_cap_t *phecap    = (MrvlIEtypes_He_cap_t *)&pmpriv->user_he_cap;
    MrvlIEtypes_He_cap_t *hw_he_cap = (MrvlIEtypes_He_cap_t *)&pmadapter->hw_he_cap;
#else
    MrvlIEtypes_He_cap_t *phecap    = (MrvlIEtypes_He_cap_t *)&pmpriv->user_2g_he_cap;
    MrvlIEtypes_He_cap_t *hw_he_cap = (MrvlIEtypes_He_cap_t *)&pmadapter->hw_2g_he_cap;
#endif

    if (pbss_desc && !wlan_check_ap_11ax_twt_supported(pbss_desc))
    {
        PRINTM(MINFO, "AP don't support twt feature\n");
        return MFALSE;
    }
    if (pbss_desc)
    {
        if (pbss_desc->bss_band & BAND_A)
        {
            hw_he_cap = (MrvlIEtypes_He_cap_t *)&pmadapter->hw_he_cap;
            phecap    = (MrvlIEtypes_He_cap_t *)&pmpriv->user_he_cap;
        }
        else
        {
            hw_he_cap = (MrvlIEtypes_He_cap_t *)&pmadapter->hw_2g_he_cap;
            phecap    = (MrvlIEtypes_He_cap_t *)&pmpriv->user_2g_he_cap;
        }
    }
    if (!(hw_he_cap->he_mac_cap[0] & HE_MAC_CAP_TWT_REQ_SUPPORT))
    {
        PRINTM(MINFO, "FW don't support TWT\n");
        return MFALSE;
    }
    if (phecap->he_mac_cap[0] & HE_MAC_CAP_TWT_REQ_SUPPORT)
        return MTRUE;
    PRINTM(MINFO, "USER HE_MAC_CAP don't support TWT\n");
    return MFALSE;
}

/**
 *  @brief This function fills the HE cap tlv out put format is LE, not CPU
 *
 *  @param pmpriv         A pointer to mlan_private structure
 *  @param band           5G or 2.4 G
 *  @param phe_cap        A pointer to MrvlIEtypes_Data_t structure
 *  @param flag           MTRUE -- phe_cap has the setting for resp
 *                                 MFALSE -- phe_cap is clean
 *
 *  @return bytes added to the phe_cap
 */

t_u16 wlan_fill_he_cap_tlv(mlan_private *pmpriv, t_u16 band, MrvlIEtypes_Extension_t *phe_cap, t_u8 flag)
{
    t_u16 len = 0;

    if (!phe_cap)
        return 0;

    if (user_he_cap_band)
    {
        if (user_he_cap_band & MBIT(1))
        {
            (void)__memcpy(pmpriv->adapter, (t_u8 *)phe_cap, pmpriv->user_he_cap, pmpriv->user_hecap_len);
            len = pmpriv->user_hecap_len;
        }
        else
        {
            (void)__memcpy(pmpriv->adapter, (t_u8 *)phe_cap, pmpriv->user_2g_he_cap, pmpriv->user_2g_hecap_len);
            len = pmpriv->user_2g_hecap_len;
        }
    }
    else
    {
        if (band & (t_u16)BAND_AAX)
        {
            (void)__memcpy(pmpriv->adapter, (t_u8 *)phe_cap, pmpriv->user_he_cap, pmpriv->user_hecap_len);
            len = pmpriv->user_hecap_len;
        }
        else
        {
            (void)__memcpy(pmpriv->adapter, (t_u8 *)phe_cap, pmpriv->user_2g_he_cap, pmpriv->user_2g_hecap_len);
            len = pmpriv->user_2g_hecap_len;
        }
    }
    phe_cap->type = wlan_cpu_to_le16(phe_cap->type);
    phe_cap->len  = wlan_cpu_to_le16(phe_cap->len);

    return len;
}

/**
 *  @brief This function append the 802_11ax HE capability  tlv
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *  @param ppbuffer     A Pointer to command buffer pointer
 *
 *  @return bytes added to the buffer
 */
int wlan_cmd_append_11ax_tlv(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc, t_u8 **ppbuffer)
{
    MrvlIEtypes_He_cap_t *phecap = MNULL;
    int len                      = 0;

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
    /** check if AP support HE, if not return right away */
    if (!pbss_desc->phe_cap)
    {
        LEAVE();
        return 0;
    }
    phecap = (MrvlIEtypes_He_cap_t *)*ppbuffer;
    if (pbss_desc->bss_band & BAND_A)
    {
        (void)__memcpy(pmpriv->adapter, *ppbuffer, pmpriv->user_he_cap, pmpriv->user_hecap_len);
        *ppbuffer += pmpriv->user_hecap_len;
        len = pmpriv->user_hecap_len;
    }
    else
    {
        (void)__memcpy(pmpriv->adapter, *ppbuffer, pmpriv->user_2g_he_cap, pmpriv->user_2g_hecap_len);
        *ppbuffer += pmpriv->user_2g_hecap_len;
        len = pmpriv->user_2g_hecap_len;
    }
    phecap->type = wlan_cpu_to_le16(phecap->type);
    phecap->len  = wlan_cpu_to_le16(phecap->len);
    phecap->he_phy_cap[0] &= ~(MBIT(3) | MBIT(4));
    LEAVE();
    return len;
}

/**
 *  @brief This function save the 11ax cap from FW.
 *
 *  @param pmadapater   A pointer to mlan_adapter
 *  @param hw_he_cap    A pointer to MrvlIEtypes_Extension_t
 *  @param tlv_idx      0 for 2.4G, 1 for 5G
 *
 *  @return N/A
 */
void wlan_update_11ax_cap(mlan_adapter *pmadapter,
                          MrvlIEtypes_Extension_t *hw_he_cap
#ifdef RW610
                          ,
                          int tlv_idx
#endif
)
{
#ifndef RW610
    MrvlIEtypes_He_cap_t *phe_cap = MNULL;
#endif
    t_u8 i         = 0;
    t_u8 he_cap_2g = 0;
#if CONFIG_11AX_TWT
    MrvlIEtypes_He_cap_t *user_he_cap_tlv = MNULL;
#endif

    ENTER();
    if ((hw_he_cap->len + sizeof(MrvlIEtypesHeader_t)) > sizeof(pmadapter->hw_he_cap))
    {
        PRINTM(MERROR, "hw_he_cap too big, len=%d\n", hw_he_cap->len);
        LEAVE();
        return;
    }
#ifndef RW610
    phe_cap = (MrvlIEtypes_He_cap_t *)hw_he_cap;
    if (phe_cap->he_phy_cap[0] & (AX_2G_20MHZ_SUPPORT | AX_2G_40MHZ_SUPPORT))
#else
    if (tlv_idx == AX_2G_TLV_INDEX)
#endif
    {
        pmadapter->hw_2g_hecap_len = hw_he_cap->len + sizeof(MrvlIEtypesHeader_t);
        (void)__memcpy(pmadapter, pmadapter->hw_2g_he_cap, (t_u8 *)hw_he_cap,
                       hw_he_cap->len + sizeof(MrvlIEtypesHeader_t));
        pmadapter->fw_bands |= BAND_GAX;
        pmadapter->config_bands |= BAND_GAX;
        he_cap_2g = MTRUE;
        DBG_HEXDUMP(MCMD_D, "2.4G HE capability IE ", (t_u8 *)pmadapter->hw_2g_he_cap, pmadapter->hw_2g_hecap_len);
    }
    else
    {
        pmadapter->fw_bands |= BAND_AAX;
        pmadapter->config_bands |= BAND_AAX;
        pmadapter->hw_hecap_len = hw_he_cap->len + sizeof(MrvlIEtypesHeader_t);
        (void)__memcpy(pmadapter, pmadapter->hw_he_cap, (t_u8 *)hw_he_cap,
                       hw_he_cap->len + sizeof(MrvlIEtypesHeader_t));
        DBG_HEXDUMP(MCMD_D, "5G HE capability IE ", (t_u8 *)pmadapter->hw_he_cap, pmadapter->hw_hecap_len);
    }
    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i])
        {
            pmadapter->priv[i]->config_bands = pmadapter->config_bands;
            if (he_cap_2g)
            {
                pmadapter->priv[i]->user_2g_hecap_len = pmadapter->hw_2g_hecap_len;
                (void)__memcpy(pmadapter, pmadapter->priv[i]->user_2g_he_cap, pmadapter->hw_2g_he_cap,
                               pmadapter->hw_2g_hecap_len);
            }
            else
            {
                pmadapter->priv[i]->user_hecap_len = pmadapter->hw_hecap_len;
                (void)__memcpy(pmadapter, pmadapter->priv[i]->user_he_cap, pmadapter->hw_he_cap,
                               pmadapter->hw_hecap_len);
            }
#if CONFIG_11AX_TWT
            /**
             *  Clear TWT bits in he_mac_cap by bss role
             *  STA mode should clear TWT responder bit
             *  UAP mode should clear TWT request bit
             */
            if (he_cap_2g)
                user_he_cap_tlv = (MrvlIEtypes_He_cap_t *)&pmadapter->priv[i]->user_2g_he_cap;
            else
                user_he_cap_tlv = (MrvlIEtypes_He_cap_t *)&pmadapter->priv[i]->user_he_cap;

            if (pmadapter->priv[i]->bss_role == MLAN_BSS_ROLE_STA)
                user_he_cap_tlv->he_mac_cap[0] &= ~HE_MAC_CAP_TWT_RESP_SUPPORT;
            else
                user_he_cap_tlv->he_mac_cap[0] &= ~HE_MAC_CAP_TWT_REQ_SUPPORT;
#endif
        }
    }
    LEAVE();
    return;
}

/**
 *  @brief This function check if 11AX is allowed in bandcfg
 *
 *  @param pmpriv	A pointer to mlan_private structure
 *  @param bss_band 	bss band
 *
 *  @return 0--not allowed, other value allowed
 */
t_u16 wlan_11ax_bandconfig_allowed(mlan_private *pmpriv, t_u16 bss_band)
{
    if (!IS_FW_SUPPORT_11AX(pmpriv->adapter))
        return MFALSE;
    if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS)
    {
        if (bss_band & BAND_G)
            return (pmpriv->adapter->adhoc_start_band & BAND_GAX);
#if CONFIG_5GHz_SUPPORT
        else if (bss_band & BAND_A)
            return (pmpriv->adapter->adhoc_start_band & BAND_AAX);
#endif
    }
    else
    {
        if (bss_band & BAND_G)
            return (pmpriv->config_bands & BAND_GAX);
#if CONFIG_5GHz_SUPPORT
        else if (bss_band & BAND_A)
            return (pmpriv->config_bands & BAND_AAX);
#endif
    }
    return MFALSE;
}

mlan_status wlan_11ax_ioctl_cmd(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status status        = MLAN_STATUS_SUCCESS;
    mlan_ds_11ax_cmd_cfg *cfg = MNULL;
    mlan_private *pmpriv      = pmadapter->priv[pioctl_req->bss_index];
    t_u16 cmd_action          = 0;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_11ax_cmd_cfg))
    {
        PRINTM(MINFO, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed    = sizeof(mlan_ds_11ax_cmd_cfg);
        pioctl_req->status_code       = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    cfg = (mlan_ds_11ax_cmd_cfg *)pioctl_req->pbuf;

    if (pioctl_req->action == MLAN_ACT_SET)
        cmd_action = HostCmd_ACT_GEN_SET;
    else
        cmd_action = HostCmd_ACT_GEN_GET;

    /* Send request to firmware */
    status = wifi_prepare_and_send_cmd(pmpriv, HostCmd_CMD_11AX_CMD, cmd_action, 0, (t_void *)pioctl_req, (t_void *)cfg,
                                       pmpriv->bss_type, NULL);
    if (status == MLAN_STATUS_SUCCESS)
        status = MLAN_STATUS_PENDING;

    LEAVE();
    return status;
}

/**
 *  @brief 11ax configuration handler
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return     MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_11ax_cfg_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status status    = MLAN_STATUS_SUCCESS;
    mlan_ds_11ax_cfg *cfg = MNULL;

    ENTER();

    cfg = (mlan_ds_11ax_cfg *)pioctl_req->pbuf;
    switch (cfg->sub_command)
    {
        case MLAN_OID_11AX_CMD_CFG:
            status = wlan_11ax_ioctl_cmd(pmadapter, pioctl_req);
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
 *  @brief This function prepares and sends 11ax cfg command
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param action       the action: GET or SET
 *  @param he_cfg       A pointer to mlan_ds_11ax_he_cfg structure
 *
 *  @return         MLAN_STATUS_SUCCESS
 */
int wlan_cmd_11ax_cfg(mlan_private *pmpriv, t_u16 action, mlan_ds_11ax_he_cfg *he_cfg)
{
    HostCmd_DS_11AX_CFG *axcfg   = MNULL;
    t_u8 *pos                    = MNULL;
    MrvlIEtypes_Extension_t *tlv = MNULL;

    ENTER();
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    cmd->command            = wlan_cpu_to_le16(HostCmd_CMD_11AX_CFG);
    cmd->size               = S_DS_GEN + sizeof(HostCmd_DS_11AX_CFG);
    axcfg                   = (HostCmd_DS_11AX_CFG *)((t_u32)cmd + S_DS_GEN);
    axcfg->action           = action;
    axcfg->band_config      = he_cfg->band & 0xFF;
    pos                     = (t_u8 *)axcfg->val;
    /** HE Capability */
    if (he_cfg->he_cap.len && (he_cfg->he_cap.ext_id == HE_CAPABILITY))
    {
        tlv       = (MrvlIEtypes_Extension_t *)pos;
        tlv->type = wlan_cpu_to_le16(he_cfg->he_cap.id);
        tlv->len  = wlan_cpu_to_le16(he_cfg->he_cap.len);
        (void)__memcpy(pmpriv->adapter, &tlv->ext_id, &he_cfg->he_cap.ext_id, he_cfg->he_cap.len);
        cmd->size += he_cfg->he_cap.len + sizeof(MrvlIEtypesHeader_t);
        pos += he_cfg->he_cap.len + sizeof(MrvlIEtypesHeader_t);
    }
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, pmpriv->bss_index);
    cmd->result  = 0x00;

    wifi_wait_for_cmdresp(he_cfg);
    LEAVE();
    return wm_wifi.cmd_resp_status;
}

/**
 *  @brief This function handles the command response of 11axcfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param hecfg        A pointer to mlan_ds_11ax_he_cfg structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_11ax_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ds_11ax_he_cfg *hecfg)
{
    HostCmd_DS_11AX_CFG *axcfg   = (HostCmd_DS_11AX_CFG *)&resp->params.axcfg;
    mlan_ds_11ax_he_capa *hecap  = MNULL;
    MrvlIEtypes_Extension_t *tlv = MNULL;
    t_u16 left_len = 0, tlv_type = 0, tlv_len = 0;

    ENTER();

    if (hecfg == MNULL)
        goto done;

    hecfg->band = axcfg->band_config;
    hecap       = (mlan_ds_11ax_he_capa *)&hecfg->he_cap;

    /* TLV parse */
    left_len = resp->size - sizeof(HostCmd_DS_11AX_CFG) - S_DS_GEN;
    tlv      = (MrvlIEtypes_Extension_t *)axcfg->val;

    while (left_len > sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if (tlv_type == EXTENSION)
        {
            switch (tlv->ext_id)
            {
                case HE_CAPABILITY:
                    hecap->id  = tlv_type;
                    hecap->len = tlv_len;
                    (void)__memcpy(pmpriv->adapter, (t_u8 *)&hecap->ext_id, (t_u8 *)&tlv->ext_id, tlv_len);
                    user_he_cap_band = hecfg->band;
                    if (hecfg->band & MBIT(1))
                    {
                        (void)__memcpy(pmpriv->adapter, (t_u8 *)&pmpriv->user_he_cap, (t_u8 *)tlv,
                                       tlv_len + sizeof(MrvlIEtypesHeader_t));
                        pmpriv->user_hecap_len =
                            MIN(tlv_len + sizeof(MrvlIEtypesHeader_t), sizeof(pmpriv->user_he_cap));
                        PRINTM(MCMND, "user_hecap_len=%d\n", pmpriv->user_hecap_len);
                        wcmdr_d("user_hecap_len=%d\n", pmpriv->user_hecap_len);
                    }
                    else
                    {
                        (void)__memcpy(pmpriv->adapter, (t_u8 *)&pmpriv->user_2g_he_cap, (t_u8 *)tlv,
                                       tlv_len + sizeof(MrvlIEtypesHeader_t));
                        pmpriv->user_2g_hecap_len =
                            MIN(tlv_len + sizeof(MrvlIEtypesHeader_t), sizeof(pmpriv->user_2g_he_cap));
                        PRINTM(MCMND, "user_2g_hecap_len=%d\n", pmpriv->user_2g_hecap_len);
                        wcmdr_d("user_2g_hecap_len=%d\n", pmpriv->user_2g_hecap_len);
                    }
                    break;
                default:
                    PRINTM(MINFO, "Unexpected extentsion \n");
                    break;
            }
        }

        left_len -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypes_Extension_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#if CONFIG_11AX_TWT
/**
 *  @brief              This function prepares TWT cfg command to configure setup/teardown
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *
 *  @return             Status returned
 */
mlan_status wlan_cmd_twt_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    HostCmd_DS_TWT_CFG *hostcmd_twtcfg              = (HostCmd_DS_TWT_CFG *)&cmd->params.twtcfg;
    mlan_ds_twtcfg *ds_twtcfg                       = (mlan_ds_twtcfg *)pdata_buf;
    hostcmd_twt_setup *twt_setup_params             = MNULL;
    hostcmd_twt_teardown *twt_teardown_params       = MNULL;
    hostcmd_twt_report *twt_report_params           = MNULL;
    mlan_status ret                                 = MLAN_STATUS_SUCCESS;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TWT_CFG);

    hostcmd_twtcfg->action = wlan_cpu_to_le16(cmd_action);
    hostcmd_twtcfg->sub_id = wlan_cpu_to_le16(ds_twtcfg->sub_id);

    cmd->size = S_DS_GEN + sizeof(hostcmd_twtcfg->action) + sizeof(hostcmd_twtcfg->sub_id);
    switch (hostcmd_twtcfg->sub_id)
    {
        case MLAN_11AX_TWT_SETUP_SUBID:
            twt_setup_params = &hostcmd_twtcfg->param.twt_setup;
            __memset(pmpriv->adapter, twt_setup_params, 0x00, sizeof(hostcmd_twtcfg->param.twt_setup));
            twt_setup_params->implicit            = ds_twtcfg->param.twt_setup.implicit;
            twt_setup_params->announced           = ds_twtcfg->param.twt_setup.announced;
            twt_setup_params->trigger_enabled     = ds_twtcfg->param.twt_setup.trigger_enabled;
            twt_setup_params->twt_info_disabled   = ds_twtcfg->param.twt_setup.twt_info_disabled;
            twt_setup_params->negotiation_type    = ds_twtcfg->param.twt_setup.negotiation_type;
            twt_setup_params->twt_wakeup_duration = ds_twtcfg->param.twt_setup.twt_wakeup_duration;
            twt_setup_params->flow_identifier     = ds_twtcfg->param.twt_setup.flow_identifier;
            twt_setup_params->hard_constraint     = ds_twtcfg->param.twt_setup.hard_constraint;
            twt_setup_params->twt_exponent        = ds_twtcfg->param.twt_setup.twt_exponent;
            twt_setup_params->twt_mantissa        = wlan_cpu_to_le16(ds_twtcfg->param.twt_setup.twt_mantissa);
            twt_setup_params->twt_request         = ds_twtcfg->param.twt_setup.twt_request;
            twt_setup_params->bcnMiss_threshold   = wlan_cpu_to_le16(
                        ds_twtcfg->param.twt_setup.bcnMiss_threshold);
            cmd->size += sizeof(hostcmd_twtcfg->param.twt_setup);
            break;
        case MLAN_11AX_TWT_TEARDOWN_SUBID:
            twt_teardown_params = &hostcmd_twtcfg->param.twt_teardown;
            __memset(pmpriv->adapter, twt_teardown_params, 0x00, sizeof(hostcmd_twtcfg->param.twt_teardown));
            twt_teardown_params->flow_identifier  = ds_twtcfg->param.twt_teardown.flow_identifier;
            twt_teardown_params->negotiation_type = ds_twtcfg->param.twt_teardown.negotiation_type;
            twt_teardown_params->teardown_all_twt = ds_twtcfg->param.twt_teardown.teardown_all_twt;
            cmd->size += sizeof(hostcmd_twtcfg->param.twt_teardown);
            break;
        case MLAN_11AX_TWT_REPORT_SUBID:

            twt_report_params = &hostcmd_twtcfg->param.twt_report;
            __memset(pmpriv->adapter, twt_report_params, 0x00, sizeof(hostcmd_twtcfg->param.twt_report));
            twt_report_params->type = ds_twtcfg->param.twt_report.type;
            cmd->size += sizeof(hostcmd_twtcfg->param.twt_report);
            break;
        default:
            PRINTM(MERROR, "Unknown subcmd %x\n", ds_twtcfg->sub_id);
            ret = MLAN_STATUS_FAILURE;
            break;
    }
    cmd->size = wlan_cpu_to_le16(cmd->size);

    LEAVE();
    return ret;
}
#endif /* CONFIG_11AX_TWT */

/**
 *  @brief This function prepares 11ax command
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd      A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11ax_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf)
{
    HostCmd_DS_11AX_CMD_CFG *axcmd        = &cmd->params.axcmd;
    mlan_ds_11ax_cmd_cfg *ds_11ax_cmd     = (mlan_ds_11ax_cmd_cfg *)pdata_buf;
    mlan_ds_11ax_txomi_cmd *txomi_cmd     = (mlan_ds_11ax_txomi_cmd *)&ds_11ax_cmd->param;
    mlan_ds_11ax_toltime_cmd *toltime_cmd = (mlan_ds_11ax_toltime_cmd *)&ds_11ax_cmd->param;


    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11AX_CMD);
    cmd->size    = sizeof(HostCmd_DS_11AX_CMD_CFG) + S_DS_GEN;

    axcmd->action = wlan_cpu_to_le16(cmd_action);
    axcmd->sub_id = wlan_cpu_to_le16(ds_11ax_cmd->sub_id);
    switch (ds_11ax_cmd->sub_id)
    {
        case MLAN_11AXCMD_TXOMI_SUBID:
            (void)__memcpy(pmpriv->adapter, axcmd->val, txomi_cmd, sizeof(mlan_ds_11ax_txomi_cmd));
            cmd->size += sizeof(mlan_ds_11ax_txomi_cmd);
            break;
        case MLAN_11AXCMD_OBSS_TOLTIME_SUBID:
            (void)__memcpy(pmpriv->adapter, axcmd->val, &toltime_cmd->tol_time, sizeof(t_u32));
            cmd->size += sizeof(t_u32);
            break;
        default:
            PRINTM(MERROR, "Unknown subcmd %x\n", ds_11ax_cmd->sub_id);
            break;
    }

    cmd->size = wlan_cpu_to_le16(cmd->size);

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function handles the command response of 11axcmd
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_11ax_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf)
{
    mlan_ds_11ax_cmd_cfg *cfg      = MNULL;
    HostCmd_DS_11AX_CMD_CFG *axcmd = &resp->params.axcmd;
    MrvlIEtypes_Data_t *tlv        = MNULL;
    t_s16 left_len                 = 0;
    t_u16 tlv_len                  = 0;

    ENTER();
    if (axcmd->action != HostCmd_ACT_GEN_GET)
    {
        goto done;
    }
    cfg = (mlan_ds_11ax_cmd_cfg *)pioctl_buf->pbuf;
    switch (axcmd->sub_id)
    {
        case MLAN_11AXCMD_SR_SUBID:
            /* TLV parse */
            left_len = resp->size - sizeof(HostCmd_DS_11AX_CMD_CFG) - S_DS_GEN;
            tlv      = (MrvlIEtypes_Data_t *)axcmd->val;
            while (left_len > (t_s16)sizeof(MrvlIEtypesHeader_t))
            {
                tlv_len = wlan_le16_to_cpu(tlv->header.len);
                (void)__memcpy(pmpriv->adapter, cfg->param.sr_cfg.param.obss_pd_offset.offset, tlv->data, tlv_len);
                left_len -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
                tlv = (MrvlIEtypes_Data_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
            }
            break;
        case MLAN_11AXCMD_BEAM_SUBID:
            cfg->param.beam_cfg.value = *axcmd->val;
            break;
        case MLAN_11AXCMD_HTC_SUBID:
            cfg->param.htc_cfg.value = *axcmd->val;
            break;
        case MLAN_11AXCMD_TXOPRTS_SUBID:
            (void)__memcpy(pmpriv->adapter, &cfg->param.txop_cfg.rts_thres, axcmd->val, sizeof(t_u16));
            break;
        case MLAN_11AXCMD_TXOMI_SUBID:
            (void)__memcpy(pmpriv->adapter, &cfg->param.txomi_cfg, axcmd->val, sizeof(mlan_ds_11ax_txomi_cmd));
            break;
        case MLAN_11AXCMD_RUPOWER_SUBID:
        {
            wifi_rutxpwrlimit_t *ru_pwr_cfg = (wifi_rutxpwrlimit_t *)wm_wifi.cmd_resp_priv;
            mlan_ds_11ax_chanlrupwrcft_cmd *rupwr_tlv;
            t_u8 *pByte;
            pByte    = axcmd->val;
            left_len = resp->size - sizeof(HostCmd_DS_11AX_CMD_CFG) - S_DS_GEN;
            while (left_len >= sizeof(MrvlIEtypesHeader_t))
            {
                rupwr_tlv = (mlan_ds_11ax_chanlrupwrcft_cmd *)pByte;
                if (rupwr_tlv->type == TLV_TYPE_CHANNEL_RU_PWR_CONFIG)
                {
                    t_u8 i;
                    ru_pwr_cfg->rupwrlimit_config[ru_pwr_cfg->num_chans].start_freq =
                        rupwr_tlv->rupwrlimit_config.start_freq;
                    ru_pwr_cfg->rupwrlimit_config[ru_pwr_cfg->num_chans].width = rupwr_tlv->rupwrlimit_config.width;
                    ru_pwr_cfg->rupwrlimit_config[ru_pwr_cfg->num_chans].chan_num =
                        rupwr_tlv->rupwrlimit_config.chan_num;
                    for (i = 0; i < MAX_RU_COUNT; i++)
                    {
                        ru_pwr_cfg->rupwrlimit_config[ru_pwr_cfg->num_chans].ruPower[i] =
                            rupwr_tlv->rupwrlimit_config.ruPower[i];
                    }
                    ru_pwr_cfg->num_chans++;
                }
                left_len -= (rupwr_tlv->len + sizeof(MrvlIEtypesHeader_t));
                pByte += (rupwr_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
        }
        break;
        case MLAN_11AXCMD_OBSS_TOLTIME_SUBID:
            (void)__memcpy(pmpriv->adapter, &cfg->param.toltime_cfg.tol_time, axcmd->val, sizeof(t_u32));
            break;
        default:
            PRINTM(MERROR, "Unknown subcmd %x\n", axcmd->sub_id);
            break;
    }

done:
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif /* CONFIG_11AX */
