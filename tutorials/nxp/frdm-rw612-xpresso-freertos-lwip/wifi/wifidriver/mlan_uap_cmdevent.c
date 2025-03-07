/** @file mlan_uap_cmdevent.c
 *
 *  @brief  This file provides the handling of AP mode command and event
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#if CONFIG_NXP_WIFI_SOFTAP_SUPPORT
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>


/**
 *  @brief This function prepares command for config uap settings
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_ap_config(pmlan_private pmpriv,
                                          IN HostCmd_DS_COMMAND *cmd,
                                          IN t_u16 cmd_action,
                                          IN pmlan_ioctl_req pioctl_buf)
{
    mlan_ds_bss *bss                               = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config              = (HostCmd_DS_SYS_CONFIG *)&cmd->params.sys_config;
    t_u8 *tlv                                      = MNULL;
    MrvlIEtypes_MacAddr_t *tlv_mac                 = MNULL;
    MrvlIEtypes_SsIdParamSet_t *tlv_ssid           = MNULL;
    MrvlIEtypes_beacon_period_t *tlv_beacon_period = MNULL;
    MrvlIEtypes_ecsa_config_t *tlv_ecsa_config     = MNULL;
    MrvlIEtypes_dtim_period_t *tlv_dtim_period     = MNULL;
    MrvlIEtypes_RatesParamSet_t *tlv_rates         = MNULL;
    MrvlIEtypes_bcast_ssid_t *tlv_bcast_ssid = MNULL;
    MrvlIEtypes_auth_type_t *tlv_auth_type               = MNULL;
    MrvlIEtypes_channel_band_t *tlv_chan_band            = MNULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list        = MNULL;
    ChanScanParamSet_t *pscan_chan                       = MNULL;
    MrvlIEtypes_encrypt_protocol_t *tlv_encrypt_protocol = MNULL;
    MrvlIEtypes_akmp_t *tlv_akmp                         = MNULL;
    MrvlIEtypes_pwk_cipher_t *tlv_pwk_cipher             = MNULL;
    MrvlIEtypes_gwk_cipher_t *tlv_gwk_cipher             = MNULL;
    MrvlIEtypes_passphrase_t *tlv_passphrase = MNULL;
    MrvlIEtypes_password_t *tlv_password     = MNULL;
    MrvlIEtypes_wmm_parameter_t *tlv_wmm_parameter = MNULL;
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    MrvlIETypes_HTCap_t *tlv_htcap = MNULL;
#endif
    t_u32 cmd_size  = 0;
    t_u8 zero_mac[] = {0, 0, 0, 0, 0, 0};
    t_u16 i;
    t_u16 ac;

    ENTER();
    if (pioctl_buf == MNULL)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }

    bss = (mlan_ds_bss *)(void *)pioctl_buf->pbuf;

    cmd->command       = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd_size           = sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN;

    tlv = (t_u8 *)sys_config->tlv_buffer;
    if (__memcmp(pmpriv->adapter, zero_mac, &bss->param.bss_config.mac_addr, MLAN_MAC_ADDR_LENGTH) != 0)
    {
        tlv_mac              = (MrvlIEtypes_MacAddr_t *)(void *)tlv;
        tlv_mac->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAC_ADDRESS);
        tlv_mac->header.len  = wlan_cpu_to_le16(MLAN_MAC_ADDR_LENGTH);
        (void)__memcpy(pmpriv->adapter, tlv_mac->mac, &bss->param.bss_config.mac_addr, MLAN_MAC_ADDR_LENGTH);
        cmd_size += sizeof(MrvlIEtypes_MacAddr_t);
        tlv += sizeof(MrvlIEtypes_MacAddr_t);
    }

    if (bss->param.bss_config.ssid.ssid_len != 0U)
    {
        tlv_ssid              = (MrvlIEtypes_SsIdParamSet_t *)(void *)tlv;
        tlv_ssid->header.type = wlan_cpu_to_le16(TLV_TYPE_SSID);
        tlv_ssid->header.len  = wlan_cpu_to_le16((t_u16)bss->param.bss_config.ssid.ssid_len);
        (void)__memcpy(pmpriv->adapter, tlv_ssid->ssid, bss->param.bss_config.ssid.ssid,
                       bss->param.bss_config.ssid.ssid_len);
        cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
        tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.ssid.ssid_len;
    }

    if ((bss->param.bss_config.beacon_period >= MIN_BEACON_PERIOD) &&
        (bss->param.bss_config.beacon_period <= MAX_BEACON_PERIOD))
    {
        tlv_beacon_period                = (MrvlIEtypes_beacon_period_t *)(void *)tlv;
        tlv_beacon_period->header.type   = wlan_cpu_to_le16(TLV_TYPE_UAP_BEACON_PERIOD);
        tlv_beacon_period->header.len    = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_beacon_period->beacon_period = wlan_cpu_to_le16(bss->param.bss_config.beacon_period);
        cmd_size += sizeof(MrvlIEtypes_beacon_period_t);
        tlv += sizeof(MrvlIEtypes_beacon_period_t);
    }

    if ((bss->param.bss_config.chan_sw_count >= MIN_CHSW_COUNT) &&
        (bss->param.bss_config.chan_sw_count <= MAX_CHSW_COUNT))
    {
        tlv_ecsa_config                     = (MrvlIEtypes_ecsa_config_t *)(void *)tlv;
        tlv_ecsa_config->header.type        = wlan_cpu_to_le16(TLV_TYPE_UAP_ECSA_CONFIG);
        tlv_ecsa_config->header.len         = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
        tlv_ecsa_config->enable             = 1;
        tlv_ecsa_config->ChannelSwitchMode  = 0;
        tlv_ecsa_config->ChannelSwitchCount = bss->param.bss_config.chan_sw_count;
        cmd_size += sizeof(MrvlIEtypes_ecsa_config_t);
        tlv += sizeof(MrvlIEtypes_ecsa_config_t);
    }

    if ((bss->param.bss_config.dtim_period >= MIN_DTIM_PERIOD) &&
        (bss->param.bss_config.dtim_period <= MAX_DTIM_PERIOD))
    {
        tlv_dtim_period              = (MrvlIEtypes_dtim_period_t *)(void *)tlv;
        tlv_dtim_period->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_DTIM_PERIOD);
        tlv_dtim_period->header.len  = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_dtim_period->dtim_period = bss->param.bss_config.dtim_period;
        cmd_size += sizeof(MrvlIEtypes_dtim_period_t);
        tlv += sizeof(MrvlIEtypes_dtim_period_t);
    }

    if (bss->param.bss_config.rates[0] != 0U)
    {
        tlv_rates              = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;
        tlv_rates->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
        for (i = 0; i < MAX_DATA_RATES && bss->param.bss_config.rates[i]; i++)
        {
            tlv_rates->rates[i] = bss->param.bss_config.rates[i];
        }
        tlv_rates->header.len = wlan_cpu_to_le16(i);
        cmd_size += sizeof(MrvlIEtypesHeader_t) + i;
        tlv += sizeof(MrvlIEtypesHeader_t) + i;
    }


    if (bss->param.bss_config.bcast_ssid_ctl <= MAX_BCAST_SSID_CTL)
    {
        tlv_bcast_ssid                 = (MrvlIEtypes_bcast_ssid_t *)(void *)tlv;
        tlv_bcast_ssid->header.type    = wlan_cpu_to_le16(TLV_TYPE_UAP_BCAST_SSID_CTL);
        tlv_bcast_ssid->header.len     = wlan_cpu_to_le16(sizeof(t_u8));
        tlv_bcast_ssid->bcast_ssid_ctl = bss->param.bss_config.bcast_ssid_ctl;
        cmd_size += sizeof(MrvlIEtypes_bcast_ssid_t);
        tlv += sizeof(MrvlIEtypes_bcast_ssid_t);
    }


    if ((((bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE) == BAND_CONFIG_MANUAL) &&
         (bss->param.bss_config.channel > 0U) && (bss->param.bss_config.channel <= MLAN_MAX_CHANNEL)) ||
        (bss->param.bss_config.band_cfg & BAND_CONFIG_ACS_MODE))
    {
        tlv_chan_band              = (MrvlIEtypes_channel_band_t *)(void *)tlv;
        tlv_chan_band->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
        tlv_chan_band->header.len  = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
        tlv_chan_band->band_config = bss->param.bss_config.band_cfg;
        tlv_chan_band->channel     = bss->param.bss_config.channel;
        cmd_size += sizeof(MrvlIEtypes_channel_band_t);
        tlv += sizeof(MrvlIEtypes_channel_band_t);
    }

    if ((bss->param.bss_config.num_of_chan) && (bss->param.bss_config.num_of_chan <= MLAN_MAX_CHANNEL))
    {
        tlv_chan_list              = (MrvlIEtypes_ChanListParamSet_t *)(void *)tlv;
        tlv_chan_list->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
        tlv_chan_list->header.len =
            wlan_cpu_to_le16((t_u16)(sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan));
        pscan_chan = tlv_chan_list->chan_scan_param;
        for (i = 0; i < bss->param.bss_config.num_of_chan; i++)
        {
            (void)__memset(pmpriv->adapter, pscan_chan, 0x00, sizeof(ChanScanParamSet_t));
            pscan_chan->chan_number = bss->param.bss_config.chan_list[i].chan_number;
            pscan_chan->radio_type  = bss->param.bss_config.chan_list[i].band_config_type;
            pscan_chan++;
        }
        cmd_size += sizeof(tlv_chan_list->header) + (sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan);
        tlv += sizeof(tlv_chan_list->header) + (sizeof(ChanScanParamSet_t) * bss->param.bss_config.num_of_chan);
    }

    if ((bss->param.bss_config.auth_mode <= MLAN_AUTH_MODE_SHARED) ||
        (bss->param.bss_config.auth_mode == MLAN_AUTH_MODE_AUTO) ||
		(bss->param.bss_config.auth_mode == MLAN_AUTH_MODE_SAE))
    {
        tlv_auth_type                 = (MrvlIEtypes_auth_type_t *)tlv;
        tlv_auth_type->header.type    = wlan_cpu_to_le16(TLV_TYPE_AUTH_TYPE);
        tlv_auth_type->header.len     = wlan_cpu_to_le16(sizeof(MrvlIEtypes_auth_type_t) - sizeof(MrvlIEtypesHeader_t));
        tlv_auth_type->auth_type      = (t_u8)bss->param.bss_config.auth_mode;
        tlv_auth_type->PWE_derivation = (t_u8)bss->param.bss_config.pwe_derivation;
        tlv_auth_type->transition_disable = (t_u8)bss->param.bss_config.transition_disable;
        cmd_size += sizeof(MrvlIEtypes_auth_type_t);
        tlv += sizeof(MrvlIEtypes_auth_type_t);
    }

    if (bss->param.bss_config.protocol != 0U)
    {
        tlv_encrypt_protocol              = (MrvlIEtypes_encrypt_protocol_t *)(void *)tlv;
        tlv_encrypt_protocol->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_ENCRYPT_PROTOCOL);
        tlv_encrypt_protocol->header.len  = wlan_cpu_to_le16(sizeof(t_u16));
        tlv_encrypt_protocol->protocol    = wlan_cpu_to_le16(bss->param.bss_config.protocol);
        cmd_size += sizeof(MrvlIEtypes_encrypt_protocol_t);
        tlv += sizeof(MrvlIEtypes_encrypt_protocol_t);
    }

    if ((bss->param.bss_config.protocol & PROTOCOL_WPA) || (bss->param.bss_config.protocol & PROTOCOL_WPA2) ||
        (bss->param.bss_config.protocol & PROTOCOL_WPA3_SAE) ||
#if CONFIG_DRIVER_OWE
        (bss->param.bss_config.protocol & PROTOCOL_OWE) ||
#endif
        (bss->param.bss_config.protocol & PROTOCOL_EAP))
    {
        tlv_akmp                     = (MrvlIEtypes_akmp_t *)(void *)tlv;
        tlv_akmp->header.type        = wlan_cpu_to_le16(TLV_TYPE_UAP_AKMP);
        tlv_akmp->key_mgmt           = wlan_cpu_to_le16(bss->param.bss_config.key_mgmt);
        tlv_akmp->header.len         = (t_u16)sizeof(t_u16);
        tlv_akmp->key_mgmt_operation = wlan_cpu_to_le16(bss->param.bss_config.key_mgmt_operation);
        tlv_akmp->header.len += (t_u16)sizeof(t_u16);
        tlv_akmp->header.len = wlan_cpu_to_le16(tlv_akmp->header.len);
        cmd_size += sizeof(MrvlIEtypes_akmp_t);
        tlv += sizeof(MrvlIEtypes_akmp_t);

        if ((bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_pwk_cipher                  = (MrvlIEtypes_pwk_cipher_t *)(void *)tlv;
            tlv_pwk_cipher->header.type     = wlan_cpu_to_le16(TLV_TYPE_PWK_CIPHER);
            tlv_pwk_cipher->header.len      = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
            tlv_pwk_cipher->protocol        = wlan_cpu_to_le16(PROTOCOL_WPA);
            tlv_pwk_cipher->pairwise_cipher = bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa;
            cmd_size += sizeof(MrvlIEtypes_pwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_pwk_cipher_t);
        }

        if ((bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa2 & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_pwk_cipher              = (MrvlIEtypes_pwk_cipher_t *)(void *)tlv;
            tlv_pwk_cipher->header.type = wlan_cpu_to_le16(TLV_TYPE_PWK_CIPHER);
            tlv_pwk_cipher->header.len  = wlan_cpu_to_le16(sizeof(t_u16) + sizeof(t_u8) + sizeof(t_u8));
            if ((bss->param.bss_config.protocol & PROTOCOL_WPA3_SAE) != 0U)
            {
                tlv_pwk_cipher->protocol = wlan_cpu_to_le16(PROTOCOL_WPA3_SAE);
            }
            else
            {
                tlv_pwk_cipher->protocol = wlan_cpu_to_le16(PROTOCOL_WPA2);
            }
            tlv_pwk_cipher->pairwise_cipher = bss->param.bss_config.wpa_cfg.pairwise_cipher_wpa2;
            cmd_size += sizeof(MrvlIEtypes_pwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_pwk_cipher_t);
        }

        if ((bss->param.bss_config.wpa_cfg.group_cipher & VALID_CIPHER_BITMAP) != 0U)
        {
            tlv_gwk_cipher               = (MrvlIEtypes_gwk_cipher_t *)(void *)tlv;
            tlv_gwk_cipher->header.type  = wlan_cpu_to_le16(TLV_TYPE_GWK_CIPHER);
            tlv_gwk_cipher->header.len   = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
            tlv_gwk_cipher->group_cipher = bss->param.bss_config.wpa_cfg.group_cipher;
            cmd_size += sizeof(MrvlIEtypes_gwk_cipher_t);
            tlv += sizeof(MrvlIEtypes_gwk_cipher_t);
        }


        if (bss->param.bss_config.wpa_cfg.length != 0U)
        {
            tlv_passphrase              = (MrvlIEtypes_passphrase_t *)(void *)tlv;
            tlv_passphrase->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WPA_PASSPHRASE);
            tlv_passphrase->header.len  = (t_u16)wlan_cpu_to_le16(bss->param.bss_config.wpa_cfg.length);
            (void)__memcpy(pmpriv->adapter, tlv_passphrase->passphrase, bss->param.bss_config.wpa_cfg.passphrase,
                           bss->param.bss_config.wpa_cfg.length);
            cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.length;
            tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.length;
        }

        if (bss->param.bss_config.wpa_cfg.password_length != 0U)
        {
            tlv_password              = (MrvlIEtypes_password_t *)(void *)tlv;
            tlv_password->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_WPA3_SAE_PASSWORD);
            tlv_password->header.len  = (t_u16)wlan_cpu_to_le16(bss->param.bss_config.wpa_cfg.password_length);
            (void)__memcpy(pmpriv->adapter, tlv_password->password, bss->param.bss_config.wpa_cfg.password,
                           bss->param.bss_config.wpa_cfg.password_length);
            cmd_size += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.password_length;
            tlv += sizeof(MrvlIEtypesHeader_t) + bss->param.bss_config.wpa_cfg.password_length;
        }

    }
    else
    {
    }

#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    if ((bss->param.bss_config.ht_cap_info) != 0U)
    {
        /* wmsdk: All the values received will be zero by default. */
        tlv_htcap                     = (MrvlIETypes_HTCap_t *)(void *)tlv;
        tlv_htcap->header.type        = wlan_cpu_to_le16(HT_CAPABILITY);
        tlv_htcap->header.len         = wlan_cpu_to_le16(sizeof(HTCap_t));
        tlv_htcap->ht_cap.ht_cap_info = wlan_cpu_to_le16(bss->param.bss_config.ht_cap_info);
        tlv_htcap->ht_cap.ampdu_param = bss->param.bss_config.ampdu_param;
        (void)__memcpy(pmpriv->adapter, tlv_htcap->ht_cap.supported_mcs_set, bss->param.bss_config.supported_mcs_set,
                       16);
#if CONFIG_WIFI_CAPA
        /* Disable 802.11n */
        if (!pmpriv->adapter->usr_dot_11n_enable)
        {
            tlv_htcap->ht_cap.supported_mcs_set[0] = 0;
            tlv_htcap->ht_cap.supported_mcs_set[4] = 0;
        }
#endif
        tlv_htcap->ht_cap.ht_ext_cap = wlan_cpu_to_le16(bss->param.bss_config.ht_ext_cap);
        tlv_htcap->ht_cap.tx_bf_cap  = wlan_cpu_to_le32(bss->param.bss_config.tx_bf_cap);
        tlv_htcap->ht_cap.asel       = bss->param.bss_config.asel;
        cmd_size += sizeof(MrvlIETypes_HTCap_t);
        tlv += sizeof(MrvlIETypes_HTCap_t);
    }
#endif


    if ((bss->param.bss_config.uap_host_based_config == MTRUE) ||
        (bss->param.bss_config.wmm_para.qos_info & 0x80 || bss->param.bss_config.wmm_para.qos_info == 0x00))
    {
        tlv_wmm_parameter              = (MrvlIEtypes_wmm_parameter_t *)tlv;
        (void)__memset(pmpriv->adapter, tlv_wmm_parameter, 0x00, sizeof(MrvlIEtypes_wmm_parameter_t));
        tlv_wmm_parameter->header.type = wlan_cpu_to_le16(TLV_TYPE_VENDOR_SPECIFIC_IE);
        tlv_wmm_parameter->header.len  = wlan_cpu_to_le16(sizeof(bss->param.bss_config.wmm_para));
        (void)__memcpy(pmpriv->adapter, tlv_wmm_parameter->wmm_para.ouitype, bss->param.bss_config.wmm_para.ouitype,
                       sizeof(tlv_wmm_parameter->wmm_para.ouitype));
        tlv_wmm_parameter->wmm_para.ouisubtype = bss->param.bss_config.wmm_para.ouisubtype;
        tlv_wmm_parameter->wmm_para.version    = bss->param.bss_config.wmm_para.version;
        tlv_wmm_parameter->wmm_para.qos_info   = bss->param.bss_config.wmm_para.qos_info;
        tlv_wmm_parameter->wmm_para.reserved   = 0x00;
        for (ac = 0; ac < 4; ac++)
        {
            tlv_wmm_parameter->wmm_para.ac_params[ac].aci_aifsn.aifsn =
                bss->param.bss_config.wmm_para.ac_params[ac].aci_aifsn.aifsn;
            tlv_wmm_parameter->wmm_para.ac_params[ac].aci_aifsn.acm =
                bss->param.bss_config.wmm_para.ac_params[ac].aci_aifsn.acm;
            tlv_wmm_parameter->wmm_para.ac_params[ac].aci_aifsn.aci =
                bss->param.bss_config.wmm_para.ac_params[ac].aci_aifsn.aci;
            tlv_wmm_parameter->wmm_para.ac_params[ac].ecw.ecw_max =
                bss->param.bss_config.wmm_para.ac_params[ac].ecw.ecw_max;
            tlv_wmm_parameter->wmm_para.ac_params[ac].ecw.ecw_min =
                bss->param.bss_config.wmm_para.ac_params[ac].ecw.ecw_min;
            tlv_wmm_parameter->wmm_para.ac_params[ac].tx_op_limit =
                wlan_cpu_to_le16(bss->param.bss_config.wmm_para.ac_params[ac].tx_op_limit);
        }
        cmd_size += sizeof(MrvlIEtypes_wmm_parameter_t);
        tlv += sizeof(MrvlIEtypes_wmm_parameter_t);
    }

    cmd->size = (t_u16)wlan_cpu_to_le16(cmd_size);
    PRINTM(MCMND, "AP config: cmd_size=%d\n", cmd_size);
#if CONFIG_WIFI_EXTRA_DEBUG
    PRINTF("wlan_uap_cmd_ap_config : cmd\r\n");
    dump_hex(cmd, cmd->size);
#endif
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief This function prepares command of sys_config
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_sys_configure(pmlan_private pmpriv,
                                              IN HostCmd_DS_COMMAND *cmd,
                                              IN t_u16 cmd_action,
                                              IN pmlan_ioctl_req pioctl_buf,
                                              IN t_void *pdata_buf)
{
    mlan_ds_bss *bss                          = MNULL;
    HostCmd_DS_SYS_CONFIG *sys_config         = (HostCmd_DS_SYS_CONFIG *)&cmd->params.sys_config;
    MrvlIEtypes_channel_band_t *chan_band_tlv = MNULL, *pdat_tlv_cb = MNULL;
    MrvlIEtypes_max_sta_count_t *max_sta_cnt_tlv = MNULL, *pdat_tlv_ccb = MNULL;
    mlan_ds_misc_custom_ie *cust_ie = MNULL;
    MrvlIEtypesHeader_t *ie_header  = (MrvlIEtypesHeader_t *)sys_config->tlv_buffer;
    t_u8 *ie                        = (t_u8 *)sys_config->tlv_buffer + sizeof(MrvlIEtypesHeader_t);
    t_u16 req_len = 0, travel_len = 0;
    custom_ie *cptr = MNULL;

#if CONFIG_ECSA
    MrvlIEtypes_action_chan_switch_t *tlv_chan_switch = MNULL;
    IEEEtypes_ChanSwitchAnn_t *csa_ie                 = MNULL;
    IEEEtypes_ExtChanSwitchAnn_t *ecsa_ie             = MNULL;
#endif

    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    cmd->command       = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    sys_config->action = wlan_cpu_to_le16(cmd_action);
    cmd->size          = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN);
    if (pioctl_buf == MNULL)
    {
        if (pdata_buf != NULL)
        {
            switch (*(t_u16 *)pdata_buf)
            {
                case TLV_TYPE_UAP_CHAN_BAND_CONFIG:
                    pdat_tlv_cb                = (MrvlIEtypes_channel_band_t *)pdata_buf;
                    chan_band_tlv              = (MrvlIEtypes_channel_band_t *)(void *)sys_config->tlv_buffer;
                    cmd->size                  = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN +
                                                                  sizeof(MrvlIEtypes_channel_band_t));
                    chan_band_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
                    chan_band_tlv->header.len =
                        wlan_cpu_to_le16(sizeof(MrvlIEtypes_channel_band_t) - sizeof(MrvlIEtypesHeader_t));
                    if (cmd_action != 0U)
                    {
                        chan_band_tlv->band_config = pdat_tlv_cb->band_config;
                        chan_band_tlv->channel     = pdat_tlv_cb->channel;
                    }
                    ret = MLAN_STATUS_SUCCESS;
                    break;
                case TLV_TYPE_UAP_MAX_STA_CNT:
                    pdat_tlv_ccb                 = (MrvlIEtypes_max_sta_count_t *)pdata_buf;
                    max_sta_cnt_tlv              = (MrvlIEtypes_max_sta_count_t *)(void *)sys_config->tlv_buffer;
                    cmd->size                    = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1U + S_DS_GEN +
                                                                    sizeof(MrvlIEtypes_max_sta_count_t));
                    max_sta_cnt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MAX_STA_CNT);

                    if (cmd_action != 0U)
                    {
                        max_sta_cnt_tlv->header.len =
                            wlan_cpu_to_le16(sizeof(MrvlIEtypes_max_sta_count_t) - sizeof(MrvlIEtypesHeader_t));
                        max_sta_cnt_tlv->max_sta_count = pdat_tlv_ccb->max_sta_count;
                    }
                    else
                    {
                        max_sta_cnt_tlv->header.len    = 0;
                        max_sta_cnt_tlv->max_sta_count = 0;
                    }
                    ret = MLAN_STATUS_SUCCESS;
                    break;
                case TLV_TYPE_MGMT_IE:
                    cust_ie         = (mlan_ds_misc_custom_ie *)pdata_buf;
                    cmd->size       = wlan_cpu_to_le16(sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN +
                                                       sizeof(MrvlIEtypesHeader_t) + cust_ie->len);
                    ie_header->type = wlan_cpu_to_le16(TLV_TYPE_MGMT_IE);
                    ie_header->len  = wlan_cpu_to_le16(cust_ie->len);

                    if (ie)
                    {
                        req_len    = cust_ie->len;
                        travel_len = 0;
                        /* conversion for index, mask, len */
                        if (req_len == sizeof(t_u16))
                            cust_ie->ie_data_list[0].ie_index = wlan_cpu_to_le16(cust_ie->ie_data_list[0].ie_index);
                        while (req_len > sizeof(t_u16))
                        {
                            cptr = (custom_ie *)(((t_u8 *)&cust_ie->ie_data_list) + travel_len);
                            travel_len += cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                            req_len -= cptr->ie_length + sizeof(custom_ie) - MAX_IE_SIZE;
                            cptr->ie_index          = wlan_cpu_to_le16(cptr->ie_index);
                            cptr->mgmt_subtype_mask = wlan_cpu_to_le16(cptr->mgmt_subtype_mask);
                            cptr->ie_length         = wlan_cpu_to_le16(cptr->ie_length);
                        }
                        (void)__memcpy(pmpriv->adapter, ie, cust_ie->ie_data_list, cust_ie->len);
                    }
                    break;
                default:
                    PRINTM(MERROR, "Wrong data, or missing TLV_TYPE 0x%04x handler.\n", *(t_u16 *)pdata_buf);
                    break;
            }
            goto done;
        }
        else
        {
        }
    }

    if (pioctl_buf->req_id == (t_u32)MLAN_IOCTL_BSS)
    {
        bss = (mlan_ds_bss *)(void *)pioctl_buf->pbuf;
        if (bss->sub_command == MLAN_OID_BSS_MAC_ADDR)
        {
        }
        else if ((bss->sub_command == MLAN_OID_UAP_BSS_CONFIG) && (cmd_action == HostCmd_ACT_GEN_SET))
        {
            ret = wlan_uap_cmd_ap_config(pmpriv, cmd, cmd_action, pioctl_buf);
            goto done;
        }
#if CONFIG_ECSA
        else if (bss->sub_command == MLAN_OID_ACTION_CHAN_SWITCH)
        {
            cmd->size       = sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN + sizeof(MrvlIEtypes_action_chan_switch_t);
            tlv_chan_switch = (MrvlIEtypes_action_chan_switch_t *)sys_config->tlv_buffer;
            tlv_chan_switch->header.type = wlan_cpu_to_le16(MRVL_ACTION_CHAN_SWITCH_ANNOUNCE);
            // mode reserve for future use
            tlv_chan_switch->mode = 0;
            if (bss->param.chanswitch.new_oper_class)
            {
                tlv_chan_switch->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_action_chan_switch_t) - sizeof(MrvlIEtypesHeader_t) +
                                     sizeof(IEEEtypes_ExtChanSwitchAnn_t));
                ecsa_ie                    = (IEEEtypes_ExtChanSwitchAnn_t *)tlv_chan_switch->ie_buf;
                ecsa_ie->element_id        = EXTEND_CHANNEL_SWITCH_ANN;
                ecsa_ie->len               = sizeof(IEEEtypes_ExtChanSwitchAnn_t) - sizeof(IEEEtypes_Header_t);
                ecsa_ie->chan_switch_mode  = bss->param.chanswitch.chan_switch_mode;
                ecsa_ie->chan_switch_count = bss->param.chanswitch.chan_switch_count;
                ecsa_ie->new_channel_num   = bss->param.chanswitch.new_channel_num;
                ecsa_ie->new_oper_class    = bss->param.chanswitch.new_oper_class;
                cmd->size += sizeof(IEEEtypes_ExtChanSwitchAnn_t);
            }
            else
            {
                tlv_chan_switch->header.len =
                    wlan_cpu_to_le16(sizeof(MrvlIEtypes_action_chan_switch_t) - sizeof(MrvlIEtypesHeader_t) +
                                     sizeof(IEEEtypes_ChanSwitchAnn_t));
                csa_ie                    = (IEEEtypes_ChanSwitchAnn_t *)tlv_chan_switch->ie_buf;
                csa_ie->element_id        = CHANNEL_SWITCH_ANN;
                csa_ie->len               = sizeof(IEEEtypes_ChanSwitchAnn_t) - sizeof(IEEEtypes_Header_t);
                csa_ie->chan_switch_mode  = bss->param.chanswitch.chan_switch_mode;
                csa_ie->chan_switch_count = bss->param.chanswitch.chan_switch_count;
                csa_ie->new_channel_num   = bss->param.chanswitch.new_channel_num;
                cmd->size += sizeof(IEEEtypes_ChanSwitchAnn_t);
            }
            cmd->size = wlan_cpu_to_le16(cmd->size);
        }
#endif
        else
        { /* Do Nothing */
        }
    }
    else if (pioctl_buf->req_id == (t_u32)MLAN_IOCTL_MISC_CFG)
    {
    }
    else
    {
        goto done;
    }
done:
    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of snmp_mib
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @param pdata_buf    A pointer to information buffer
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_snmp_mib(pmlan_private pmpriv,
                                         IN HostCmd_DS_COMMAND *cmd,
                                         IN t_u16 cmd_action,
                                         IN t_u32 cmd_oid,
                                         IN pmlan_ioctl_req pioctl_buf,
                                         IN t_void *pdata_buf)
{
    HostCmd_DS_802_11_SNMP_MIB *psnmp_mib = &cmd->params.smib;
    mlan_status ret                       = MLAN_STATUS_SUCCESS;
    t_u8 *psnmp_oid                       = MNULL;
#if (CONFIG_WIFI_FRAG_THRESHOLD) || (CONFIG_WIFI_RTS_THRESHOLD)
    t_u32 ul_temp;
#endif
    t_u8 i;

    t_u8 snmp_oids[] = {
        (t_u8)tkip_mic_failures,
        (t_u8)ccmp_decrypt_errors,
        (t_u8)wep_undecryptable_count,
        (t_u8)wep_icv_error_count,
        (t_u8)decrypt_failure_count,
        (t_u8)dot11_mcast_tx_count,
        (t_u8)dot11_failed_count,
        (t_u8)dot11_retry_count,
        (t_u8)dot11_multi_retry_count,
        (t_u8)dot11_frame_dup_count,
        (t_u8)dot11_rts_success_count,
        (t_u8)dot11_rts_failure_count,
        (t_u8)dot11_ack_failure_count,
        (t_u8)dot11_rx_fragment_count,
        (t_u8)dot11_mcast_rx_frame_count,
        (t_u8)dot11_fcs_error_count,
        (t_u8)dot11_tx_frame_count,
        (t_u8)dot11_rsna_tkip_cm_invoked,
        (t_u8)dot11_rsna_4way_hshk_failures,
    };

    ENTER();

    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        cmd->command          = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
        psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
        if (cmd_oid == (t_u32)StopDeauth_i)
        {
            psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)StopDeauth_i);
            psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u8));
            cmd->size           = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_SNMP_MIB) + S_DS_GEN);
        }
        else
        {
            cmd->size = wlan_cpu_to_le16(sizeof(t_u16) + S_DS_GEN + sizeof(snmp_oids) * sizeof(MrvlIEtypes_snmp_oid_t));
            psnmp_oid = (t_u8 *)&psnmp_mib->oid;
            for (i = 0; i < sizeof(snmp_oids); i++)
            {
                /* SNMP OID header type */
                // coverity[overrun-local:SUPPRESS]
                *(t_u16 *)(void *)psnmp_oid = wlan_cpu_to_le16(snmp_oids[i]);
                psnmp_oid += sizeof(t_u16);
                /* SNMP OID header length */
                *(t_u16 *)(void *)psnmp_oid = wlan_cpu_to_le16(sizeof(t_u32));
                psnmp_oid += sizeof(t_u16) + sizeof(t_u32);
            }
        }
    }
    else
    { /* cmd_action == ACT_SET */
        cmd->command          = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
        cmd->size             = sizeof(HostCmd_DS_802_11_SNMP_MIB) - 1U + S_DS_GEN;
        psnmp_mib->query_type = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);

        switch (cmd_oid)
        {
            case Dot11D_i:
            case Dot11H_i:
                psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)cmd_oid);
                psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u16));
                // ul_temp = *(t_u32 *) pdata_buf;
                //*((t_u16 *) (psnmp_mib->value)) = wlan_cpu_to_le16((t_u16) ul_temp);
                cmd->size += (t_u16)sizeof(t_u16);
                break;
            case StopDeauth_i:
                psnmp_mib->oid      = wlan_cpu_to_le16((t_u16)cmd_oid);
                psnmp_mib->buf_size = wlan_cpu_to_le16(sizeof(t_u8));
                psnmp_mib->value[0] = *((t_u8 *)pdata_buf);
                cmd->size += (t_u16)sizeof(t_u8);
                break;
#if CONFIG_WIFI_FRAG_THRESHOLD
            case FragThresh_i:
                psnmp_mib->oid                 = wlan_cpu_to_le16((t_u16)FragThresh_i);
                psnmp_mib->buf_size            = wlan_cpu_to_le16(sizeof(t_u16));
                ul_temp                        = *((t_u32 *)pdata_buf);
                *((t_u16 *)(psnmp_mib->value)) = wlan_cpu_to_le16((t_u16)ul_temp);
                cmd->size += sizeof(t_u16);
                break;
#endif
#if CONFIG_WIFI_RTS_THRESHOLD
            case RtsThresh_i:
                psnmp_mib->oid                 = wlan_cpu_to_le16((t_u16)RtsThresh_i);
                psnmp_mib->buf_size            = wlan_cpu_to_le16(sizeof(t_u16));
                ul_temp                        = *((t_u32 *)pdata_buf);
                *((t_u16 *)(psnmp_mib->value)) = wlan_cpu_to_le16((t_u16)ul_temp);
                cmd->size += sizeof(t_u16);
                break;
#endif
            default:
                PRINTM(MERROR, "Unsupported OID.\n");
                ret = MLAN_STATUS_FAILURE;
                break;
        }
        cmd->size = wlan_cpu_to_le16(cmd->size);
    }

    LEAVE();
    return ret;
}


/**
 *  @brief This function prepares command of deauth station
 *
 *  @param pmpriv		A pointer to mlan_private structure
 *  @param cmd	   		A pointer to HostCmd_DS_COMMAND structure
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_uap_cmd_sta_deauth(pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf)
{
    HostCmd_DS_STA_DEAUTH *pcmd_sta_deauth = (HostCmd_DS_STA_DEAUTH *)&cmd->params.sta_deauth;
    mlan_deauth_param *deauth              = (mlan_deauth_param *)pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_STA_DEAUTH);
    cmd->size    = wlan_cpu_to_le16(S_DS_GEN + sizeof(HostCmd_DS_STA_DEAUTH));
    (void)__memcpy(pmpriv->adapter, pcmd_sta_deauth->mac, deauth->mac_addr, MLAN_MAC_ADDR_LENGTH);
    pcmd_sta_deauth->reason = wlan_cpu_to_le16(deauth->reason_code);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


#if defined(WAPI_AP) || defined(HOST_AUTHENTICATOR) || (CONFIG_HOSTAPD)

/**
 *  @brief This function prepares command of key material
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   The action: GET or SET
 *  @param cmd_oid      OID: ENABLE or DISABLE
 *  @param pdata_buf    A pointer to data buffer
 *  @return             MLAN_STATUS_SUCCESS
 */
static mlan_status wlan_uap_cmd_key_material(
    pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_u16 cmd_oid, t_void *pdata_buf)
{
    HostCmd_DS_802_11_KEY_MATERIAL *pkey_material = &cmd->params.key_material;
    mlan_ds_encrypt_key *pkey                     = (mlan_ds_encrypt_key *)pdata_buf;
    mlan_status ret                               = MLAN_STATUS_SUCCESS;

    ENTER();
    if (!pkey)
    {
        ret = MLAN_STATUS_FAILURE;
        goto done;
    }
    cmd->command          = wlan_cpu_to_le16(HostCmd_CMD_802_11_KEY_MATERIAL);
    pkey_material->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        cmd->size = wlan_cpu_to_le16(sizeof(pkey_material->action) + S_DS_GEN);
        goto done;
    }
    memset(&pkey_material->key_param_set, 0, sizeof(MrvlIEtype_KeyParamSetV2_t));
    if (pkey->key_flags & KEY_FLAG_REMOVE_KEY)
    {
        pkey_material->action                 = wlan_cpu_to_le16(HostCmd_ACT_GEN_REMOVE);
        pkey_material->key_param_set.type     = wlan_cpu_to_le16(TLV_TYPE_KEY_PARAM_V2);
        pkey_material->key_param_set.length   = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN);
        pkey_material->key_param_set.key_idx  = pkey->key_index & KEY_INDEX_MASK;
        pkey_material->key_param_set.key_info = wlan_cpu_to_le16(KEY_INFO_MCAST_KEY | KEY_INFO_UCAST_KEY);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.mac_addr, pkey->mac_addr, MLAN_MAC_ADDR_LENGTH,
                   MLAN_MAC_ADDR_LENGTH);
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(pkey_material->action));
        wifi_d("Remove Key");
        goto done;
    }
    pkey_material->action                 = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
    pkey_material->key_param_set.key_idx  = pkey->key_index & KEY_INDEX_MASK;
    pkey_material->key_param_set.type     = wlan_cpu_to_le16(TLV_TYPE_KEY_PARAM_V2);
    pkey_material->key_param_set.key_info = KEY_INFO_ENABLE_KEY;
    memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.mac_addr, pkey->mac_addr, MLAN_MAC_ADDR_LENGTH,
               MLAN_MAC_ADDR_LENGTH);
    if (pkey->key_len <= MAX_WEP_KEY_SIZE)
    {
        pkey_material->key_param_set.length   = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(wep_param_t));
        pkey_material->key_param_set.key_type = KEY_TYPE_ID_WEP;
        pkey_material->key_param_set.key_info |= KEY_INFO_MCAST_KEY | KEY_INFO_UCAST_KEY;
        if (pkey_material->key_param_set.key_idx == (pmpriv->wep_key_curr_index & KEY_INDEX_MASK))
            pkey_material->key_param_set.key_info |= KEY_INFO_DEFAULT_KEY;
        pkey_material->key_param_set.key_info               = wlan_cpu_to_le16(pkey_material->key_param_set.key_info);
        pkey_material->key_param_set.key_params.wep.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.wep.key, pkey->key_material, pkey->key_len,
                   MAX_WEP_KEY_SIZE);
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(wep_param_t) + sizeof(pkey_material->action));
        wifi_d("Set WEP Key");
        goto done;
    }
    if (pkey->key_flags & KEY_FLAG_GROUP_KEY)
        pkey_material->key_param_set.key_info |= KEY_INFO_MCAST_KEY;
    else
        pkey_material->key_param_set.key_info |= KEY_INFO_UCAST_KEY;
#ifdef ENABLE_802_11W
    if (pkey->key_flags & KEY_FLAG_AES_MCAST_IGTK)
        pkey_material->key_param_set.key_info |= KEY_INFO_CMAC_AES_KEY;
#endif
    if (pkey->key_flags & KEY_FLAG_SET_TX_KEY)
        pkey_material->key_param_set.key_info |= KEY_INFO_TX_KEY | KEY_INFO_RX_KEY;
    else
        pkey_material->key_param_set.key_info |= KEY_INFO_TX_KEY;
    pkey_material->key_param_set.key_info |= KEY_INFO_DEFAULT_KEY;
    pkey_material->key_param_set.key_info = wlan_cpu_to_le16(pkey_material->key_param_set.key_info);
    if (pkey->key_flags & KEY_FLAG_GCMP || pkey->key_flags & KEY_FLAG_GCMP_256)
    {
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
        {
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.gcmp.pn, pkey->pn, SEQ_MAX_SIZE,
                       WPA_PN_SIZE);
        }
        if (pkey->key_flags & KEY_FLAG_GCMP)
            pkey_material->key_param_set.key_type = KEY_TYPE_ID_GCMP;
        else
            pkey_material->key_param_set.key_type = KEY_TYPE_ID_GCMP_256;
        pkey_material->key_param_set.key_params.gcmp.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.gcmp.key, pkey->key_material, pkey->key_len,
                   WPA_GCMP_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(gcmp_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(gcmp_param) + sizeof(pkey_material->action));
        PRINTM(MCMND, "Set GCMP Key\n");
        goto done;
    }
    if (pkey->key_flags & KEY_FLAG_CCMP_256)
    {
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
        {
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.ccmp256.pn, pkey->pn, SEQ_MAX_SIZE,
                       WPA_PN_SIZE);
        }
        pkey_material->key_param_set.key_type                   = KEY_TYPE_ID_CCMP_256;
        pkey_material->key_param_set.key_params.ccmp256.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.ccmp256.key, pkey->key_material,
                   pkey->key_len, WPA_CCMP_256_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(ccmp_256_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(ccmp_256_param) + sizeof(pkey_material->action));
        PRINTM(MCMND, "Set CCMP256 Key\n");
        goto done;
    }
#ifdef ENABLE_802_11W
    if (pkey->key_len == WPA_AES_KEY_LEN && !(pkey->key_flags & KEY_FLAG_AES_MCAST_IGTK))
    {
#else
    if (pkey->key_len == WPA_AES_KEY_LEN)
    {
#endif
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.aes.pn, pkey->pn, SEQ_MAX_SIZE,
                       WPA_PN_SIZE);
        pkey_material->key_param_set.key_type               = KEY_TYPE_ID_AES;
        pkey_material->key_param_set.key_params.aes.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.aes.key, pkey->key_material, pkey->key_len,
                   WPA_AES_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(aes_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN + sizeof(aes_param) +
                                     sizeof(pkey_material->action));
        wifi_d("Set AES Key");
        goto done;
    }
#ifdef ENABLE_802_11W
    if (pkey->key_len == WPA_IGTK_KEY_LEN && (pkey->key_flags & KEY_FLAG_AES_MCAST_IGTK))
    {
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.cmac_aes.ipn, pkey->pn, SEQ_MAX_SIZE,
                       IGTK_PN_SIZE);
        pkey_material->key_param_set.key_info &= ~(wlan_cpu_to_le16(KEY_INFO_MCAST_KEY));
        pkey_material->key_param_set.key_info |= wlan_cpu_to_le16(KEY_INFO_AES_MCAST_IGTK);
        if (pkey->key_flags & KEY_FLAG_GMAC_128)
            pkey_material->key_param_set.key_type = KEY_TYPE_ID_BIP_GMAC_128;
        else
            pkey_material->key_param_set.key_type = KEY_TYPE_ID_AES_CMAC;
        pkey_material->key_param_set.key_params.cmac_aes.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.cmac_aes.key, pkey->key_material,
                   pkey->key_len, CMAC_AES_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(cmac_aes_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(cmac_aes_param) + sizeof(pkey_material->action));
        if (pkey->key_flags & KEY_FLAG_GMAC_128)
            PRINTM(MCMND, "Set AES 128 GMAC Key\n");
        else
            PRINTM(MCMND, "Set CMAC AES Key\n");
        goto done;
    }
    if (pkey->key_len == WPA_IGTK_256_KEY_LEN && (pkey->key_flags & KEY_FLAG_AES_MCAST_IGTK))
    {
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.gmac_aes.ipn, pkey->pn, SEQ_MAX_SIZE,
                       IGTK_PN_SIZE);
        pkey_material->key_param_set.key_info &= ~(wlan_cpu_to_le16(KEY_INFO_MCAST_KEY));
        pkey_material->key_param_set.key_info |= wlan_cpu_to_le16(KEY_INFO_AES_MCAST_IGTK);
        pkey_material->key_param_set.key_type                    = KEY_TYPE_ID_BIP_GMAC_256;
        pkey_material->key_param_set.key_params.gmac_aes.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.gmac_aes.key, pkey->key_material,
                   pkey->key_len, WPA_IGTK_256_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(gmac_aes_256_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(gmac_aes_256_param) + sizeof(pkey_material->action));
        PRINTM(MCMND, "Set AES 256 GMAC Key\n");
        goto done;
    }
#endif
    if (pkey->key_len == WPA_TKIP_KEY_LEN)
    {
        if (pkey->key_flags & (KEY_FLAG_RX_SEQ_VALID | KEY_FLAG_TX_SEQ_VALID))
            memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.tkip.pn, pkey->pn, SEQ_MAX_SIZE,
                       WPA_PN_SIZE);
        pkey_material->key_param_set.key_type                = KEY_TYPE_ID_TKIP;
        pkey_material->key_param_set.key_params.tkip.key_len = wlan_cpu_to_le16(pkey->key_len);
        memcpy_ext(pmpriv->adapter, pkey_material->key_param_set.key_params.tkip.key, pkey->key_material, pkey->key_len,
                   WPA_TKIP_KEY_LEN);
        pkey_material->key_param_set.length = wlan_cpu_to_le16(KEY_PARAMS_FIXED_LEN + sizeof(tkip_param));
        cmd->size = wlan_cpu_to_le16(sizeof(MrvlIEtypesHeader_t) + S_DS_GEN + KEY_PARAMS_FIXED_LEN +
                                     sizeof(tkip_param) + sizeof(pkey_material->action));
        PRINTM(MCMND, "Set TKIP Key\n");
    }
done:
    LEAVE();
    return ret;
}

#endif /* WAPI_AP || HOST_AUTHENTICATOR || CONFIG_HOSTAPD */


/**
 *  @brief This function will search for the specific ie
 *
 *
 *  @param priv    A pointer to mlan_private
 *  @param pevent  A pointer to event buf
 *  @param sta_ptr A pointer to sta_node
 *
 *  @return	       N/A
 */
void wlan_check_sta_capability(pmlan_private priv, pmlan_buffer pevent, sta_node *sta_ptr)
{
    t_u16 tlv_type, tlv_len;
    t_u16 frame_control, frame_sub_type = 0;
    t_u8 *assoc_req_ie = MNULL;
    t_u8 ie_len = 0, assoc_ie_len = 0;
    IEEEtypes_HTCap_t *pht_cap = MNULL;
    int tlv_buf_left           = pevent->data_len - ASSOC_EVENT_FIX_SIZE;
    MrvlIEtypesHeader_t *tlv   = (MrvlIEtypesHeader_t *)(pevent->pbuf + pevent->data_offset + ASSOC_EVENT_FIX_SIZE);
    MrvlIETypes_MgmtFrameSet_t *mgmt_tlv = MNULL;

    ENTER();
    while (tlv_buf_left >= (int)sizeof(MrvlIEtypesHeader_t))
    {
        tlv_type = wlan_le16_to_cpu(tlv->type);
        tlv_len  = wlan_le16_to_cpu(tlv->len);
        if ((sizeof(MrvlIEtypesHeader_t) + tlv_len) > (unsigned int)tlv_buf_left)
        {
            wifi_d("wrong tlv: tlvLen=%d, tlvBufLeft=%d", tlv_len, tlv_buf_left);
            break;
        }
        if (tlv_type == TLV_TYPE_UAP_MGMT_FRAME)
        {
            mgmt_tlv = (MrvlIETypes_MgmtFrameSet_t *)tlv;
            (void)__memcpy(priv->adapter, &frame_control, (t_u8 *)&(mgmt_tlv->frame_control), sizeof(frame_control));
            frame_sub_type = IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(frame_control);
            if ((mgmt_tlv->frame_control.type == 0) &&
                ((frame_sub_type == SUBTYPE_ASSOC_REQUEST) || (frame_sub_type == SUBTYPE_REASSOC_REQUEST)))
            {
                if (frame_sub_type == SUBTYPE_ASSOC_REQUEST)
                    assoc_ie_len = sizeof(IEEEtypes_AssocRqst_t);
                else if (frame_sub_type == SUBTYPE_REASSOC_REQUEST)
                    assoc_ie_len = sizeof(IEEEtypes_ReAssocRqst_t);

                ie_len       = tlv_len - sizeof(IEEEtypes_FrameCtl_t) - assoc_ie_len;
                assoc_req_ie = (t_u8 *)tlv + sizeof(MrvlIETypes_MgmtFrameSet_t) + assoc_ie_len;
                pht_cap      = (IEEEtypes_HTCap_t *)wlan_get_specific_ie(priv, assoc_req_ie, ie_len, HT_CAPABILITY, 0);
                if (pht_cap)
                {
                    wifi_d("STA supports 11n");
                    sta_ptr->is_11n_enabled = MTRUE;
                    if (GETHT_MAXAMSDU(pht_cap->ht_cap.ht_cap_info))
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    else
                        sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                }
                else
                {
                    wifi_d("STA doesn't support 11n");
                }
                break;
            }
        }
        tlv_buf_left -= (sizeof(MrvlIEtypesHeader_t) + tlv_len);
        tlv = (MrvlIEtypesHeader_t *)((t_u8 *)tlv + tlv_len + sizeof(MrvlIEtypesHeader_t));
    }
    LEAVE();

    return;
}


#if UAP_HOST_MLME
/**
 *  @brief	Check 11B support Rates
 *
 *
 *  @param pmadapter	Private mlan adapter structure
 *
 *  @return MTRUE/MFALSE
 *
 */
static t_u8 wlan_check_11B_support_rates(MrvlIEtypes_RatesParamSet_t *prates_tlv)
{
    int i;
    t_u8 rate;
    t_u8 ret = MTRUE;
    for (i = 0; i < prates_tlv->header.len; i++)
    {
        rate = prates_tlv->rates[i] & 0x7f;
        if ((rate != 0x02) && (rate != 0x04) && (rate != 0x0b) && (rate != 0x16))
        {
            ret = MFALSE;
            break;
        }
    }
    return ret;
}

/**
 *  @brief This function prepares command of sys_config
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   cmd action
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *  @return         MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_uap_cmd_add_station(pmlan_private pmpriv,
                                            HostCmd_DS_COMMAND *cmd,
                                            t_u16 cmd_action,
                                            pmlan_ioctl_req pioctl_buf)
{
    mlan_ds_bss *bss                = MNULL;
    HostCmd_DS_ADD_STATION *new_sta = (HostCmd_DS_ADD_STATION *)&cmd->params.sta_info;
    sta_node *sta_ptr               = MNULL;
    t_u16 tlv_buf_left;
    t_u8 *pos        = MNULL;
    t_u8 *tlv_buf    = MNULL;
    t_u16 travel_len = 0;
    MrvlIEtypesHeader_t *tlv;
    t_u16 tlv_len = 0;
    t_u8 b_only   = MFALSE;
    MrvlIETypes_HTCap_t *phtcap;
#if CONFIG_11AC
    MrvlIETypes_VHTCap_t *pvhtcap;
#endif
#if CONFIG_11AX
    MrvlIEtypes_Extension_t *pext_tlv = MNULL;
#endif
    MrvlIEtypes_StaFlag_t *pstaflag;
    int i;

    ENTER();

    if (!pioctl_buf)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    (void)__memset(pmpriv->adapter, new_sta, 0x00, sizeof(HostCmd_DS_ADD_STATION));
    bss = (mlan_ds_bss *)pioctl_buf->pbuf;

    cmd->command    = wlan_cpu_to_le16(HostCmd_CMD_ADD_NEW_STATION);
    new_sta->action = wlan_cpu_to_le16(cmd_action);
    cmd->size       = sizeof(HostCmd_DS_ADD_STATION) + S_DS_GEN;
    if (cmd_action == HostCmd_ACT_ADD_STA)
    {
        sta_ptr = wlan_get_station_entry(pmpriv, bss->param.sta_info.peer_mac);
        if (!sta_ptr)
            sta_ptr = wlan_add_station_entry(pmpriv, bss->param.sta_info.peer_mac);
    }
    else
    {
        sta_ptr = wlan_add_station_entry(pmpriv, bss->param.sta_info.peer_mac);
    }
    if (!sta_ptr)
    {
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
#ifdef EASYMESH
    /* Save station aid for multi-ap */
    sta_ptr->aid = bss->param.sta_info.aid;
#endif
    (void)__memcpy(NULL, new_sta->peer_mac, bss->param.sta_info.peer_mac, MLAN_MAC_ADDR_LENGTH);
    if (cmd_action != HostCmd_ACT_ADD_STA)
        goto done;
    new_sta->aid             = wlan_cpu_to_le16(bss->param.sta_info.aid);
    new_sta->listen_interval = wlan_cpu_to_le32(bss->param.sta_info.listen_interval);
    if (bss->param.sta_info.cap_info)
        new_sta->cap_info = wlan_cpu_to_le16(bss->param.sta_info.cap_info);
    else
        new_sta->cap_info = wlan_cpu_to_le16(sta_ptr->capability);
    tlv_buf_left = bss->param.sta_info.tlv_len;
    pos          = new_sta->tlv;
    tlv_buf      = bss->param.sta_info.tlv;
    tlv          = (MrvlIEtypesHeader_t *)tlv_buf;
    if (bss->param.sta_info.sta_flags & STA_FLAG_WME)
    {
        wifi_d("STA flags supports wmm");
        sta_ptr->is_wmm_enabled = MTRUE;
    }
    // append sta_flag_flags.
    pstaflag              = (MrvlIEtypes_StaFlag_t *)pos;
    pstaflag->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_STA_FLAGS);
    pstaflag->header.len  = wlan_cpu_to_le16(sizeof(t_u32));
    pstaflag->sta_flags   = wlan_cpu_to_le32(bss->param.sta_info.sta_flags);
    pos += sizeof(MrvlIEtypes_StaFlag_t);
    cmd->size += sizeof(MrvlIEtypes_StaFlag_t);

    while (tlv_buf_left >= sizeof(MrvlIEtypesHeader_t))
    {
        if (tlv_buf_left < (sizeof(MrvlIEtypesHeader_t) + tlv->len))
            break;
        switch (tlv->type)
        {
            case EXT_CAPABILITY:
                break;
            case SUPPORTED_RATES:
                b_only = wlan_check_11B_support_rates((MrvlIEtypes_RatesParamSet_t *)tlv);
                if (b_only)
                    wm_wifi.bandwidth = BANDWIDTH_20MHZ;
                break;
            case QOS_INFO:
                wifi_d("STA supports wmm");
                sta_ptr->is_wmm_enabled = MTRUE;
                break;
            case HT_CAPABILITY:
                wifi_d("STA supports 11n");
                sta_ptr->is_11n_enabled = MTRUE;
                phtcap                  = (MrvlIETypes_HTCap_t *)tlv;
                if (sta_ptr->HTcap.ieee_hdr.element_id == HT_CAPABILITY)
                {
                    if (GETHT_40MHZ_INTOLARANT(sta_ptr->HTcap.ht_cap.ht_cap_info))
                    {
                        wifi_d("SETHT_40MHZ_INTOLARANT");
                        SETHT_40MHZ_INTOLARANT(phtcap->ht_cap.ht_cap_info);
                    }
                }
                if (GETHT_MAXAMSDU(phtcap->ht_cap.ht_cap_info))
                    sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                else
                    sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                break;
#if CONFIG_11AC
            case VHT_CAPABILITY:
                wifi_d("STA supports 11ac");
                sta_ptr->is_11ac_enabled = MTRUE;
                pvhtcap                  = (MrvlIETypes_VHTCap_t *)tlv;
                if (GET_VHTCAP_MAXMPDULEN(pvhtcap->vht_cap.vht_cap_info) == 2)
                    sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_12K;
                else if (GET_VHTCAP_MAXMPDULEN(pvhtcap->vht_cap.vht_cap_info) == 1)
                    sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                else
                    sta_ptr->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                break;
            case OPER_MODE_NTF:
                break;
#endif
#if CONFIG_11AX
            case EXTENSION:
                pext_tlv = (MrvlIEtypes_Extension_t *)tlv;
                if (pext_tlv->ext_id == HE_CAPABILITY)
                {
                    sta_ptr->is_11ax_enabled = MTRUE;
                    wifi_d("STA supports 11ax");
                }
#ifdef ENABLE_802_116E
                else if (pext_tlv->ext_id == HE_6G_CAPABILITY)
                {
                    MrvlIEtypes_He_6g_cap_t *phe_6g_cap = MNULL;
                    phe_6g_cap                          = (MrvlIEtypes_He_6g_cap_t *)tlv;
                    if (GET_6G_BAND_CAP_MAXMPDULEN(phe_6g_cap->capa) == 2)
                        pmpriv->max_amsdu = MLAN_TX_DATA_BUF_SIZE_12K;
                    else if (GET_6G_BAND_CAP_MAXMPDULEN(phe_6g_cap->capa) == 1)
                        pmpriv->max_amsdu = MLAN_TX_DATA_BUF_SIZE_8K;
                    else
                        pmpriv->max_amsdu = MLAN_TX_DATA_BUF_SIZE_4K;
                }
#endif
                else
                {
                    pext_tlv = MNULL;
                }
                break;
#endif
            default:
                break;
        }
        tlv->type = wlan_cpu_to_le16(tlv->type);
        tlv->len  = wlan_cpu_to_le16(tlv->len);
        tlv_len   = tlv->len;
        (void)__memcpy(NULL, pos, (t_u8 *)tlv, sizeof(MrvlIEtypesHeader_t) + tlv_len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv_len;
        tlv_buf += sizeof(MrvlIEtypesHeader_t) + tlv_len;
        tlv = (MrvlIEtypesHeader_t *)tlv_buf;
        travel_len += sizeof(MrvlIEtypesHeader_t) + tlv_len;
        tlv_buf_left -= sizeof(MrvlIEtypesHeader_t) + tlv_len;
    }
#if CONFIG_11AX
    if (sta_ptr->is_11ax_enabled)
    {
        if (pext_tlv == MNULL)
        {
            tlv       = (MrvlIEtypesHeader_t *)pos;
            tlv->type = wlan_cpu_to_le16(EXTENSION);
            tlv->len  = wlan_cpu_to_le16(
                MIN(sta_ptr->he_cap.ieee_hdr.len, sizeof(IEEEtypes_HECap_t) - sizeof(IEEEtypes_Header_t)));

            pos += sizeof(MrvlIEtypesHeader_t);
            (void)__memcpy(NULL, pos, (t_u8 *)&sta_ptr->he_cap.ext_id, tlv->len);
            pos += tlv->len;
            travel_len += sizeof(MrvlIEtypesHeader_t) + tlv->len;
        }
    }
#endif

    if (sta_ptr->is_11n_enabled)
    {
        if (pmpriv->uap_channel <= 14)
            sta_ptr->bandmode = BAND_GN;
#if CONFIG_5GHz_SUPPORT
        else
            sta_ptr->bandmode = BAND_AN;
#endif
    }
    else if (!b_only)
    {
        if (pmpriv->uap_channel <= 14)
            sta_ptr->bandmode = BAND_G;
#if CONFIG_5GHz_SUPPORT
        else
            sta_ptr->bandmode = BAND_A;
#endif
    }
    else
        sta_ptr->bandmode = BAND_B;
#if CONFIG_11AC
    if (sta_ptr->is_11ac_enabled)
    {
        if (pmpriv->uap_channel <= 14)
            sta_ptr->bandmode = BAND_GAC;
        else
            sta_ptr->bandmode = BAND_AAC;
    }
#endif
#if CONFIG_11AX
    if (sta_ptr->is_11ax_enabled)
    {
        if (pmpriv->uap_channel <= 14)
            sta_ptr->bandmode = BAND_GAX;
        else
            sta_ptr->bandmode = BAND_AAX;
    }
#endif

    for (i = 0; i < MAX_NUM_TID; i++)
    {
        if (sta_ptr->is_11n_enabled
#if CONFIG_11AX
            || sta_ptr->is_11ax_enabled
#endif
        )
            sta_ptr->ampdu_sta[i] = pmpriv->aggr_prio_tbl[i].ampdu_user;
        else
            sta_ptr->ampdu_sta[i] = BA_STREAM_NOT_ALLOWED;
    }
    (void)__memset(pmpriv->adapter, sta_ptr->rx_seq, 0xff, sizeof(sta_ptr->rx_seq));
done:
    cmd->size += travel_len;
    cmd->size = wlan_cpu_to_le16(cmd->size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}
#endif

/**
 *  @brief This function prepares command of bss_start.
 *
 * @param pmpriv       A pointer to mlan_private structure
 * @param cmd          A pointer to HostCmd_DS_COMMAND structure
 *
 * @return             MLAN_STATUS_SUCCESS
 **/
static mlan_status wlan_uap_cmd_bss_start(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd)
{
#if UAP_HOST_MLME
    MrvlIEtypes_HostMlme_t *tlv;
#endif
    ENTER();
    cmd->command = wlan_cpu_to_le16(HOST_CMD_APCMD_BSS_START);
    cmd->size    = S_DS_GEN;
#if UAP_HOST_MLME
    if (pmpriv->uap_host_based)
    {
        tlv              = (MrvlIEtypes_HostMlme_t *)((t_u8 *)cmd + cmd->size);
        tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_HOST_MLME);
        tlv->header.len  = wlan_cpu_to_le16(sizeof(tlv->host_mlme));
        tlv->host_mlme   = MTRUE;
        cmd->size += sizeof(MrvlIEtypes_HostMlme_t);
    }
#endif
    cmd->size = wlan_cpu_to_le16(cmd->size);
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/********************************************************
    Global Functions
********************************************************/
/**
 *  @brief This function prepare the command before sending to firmware.
 *
 *  @param priv       A pointer to mlan_private structure
 *  @param cmd_no       Command number
 *  @param cmd_action   Command action: GET or SET
 *  @param cmd_oid      Cmd oid: treated as sub command
 *  @param pioctl_buf   A pointer to MLAN IOCTL Request buffer
 *  @param pdata_buf    A pointer to information buffer
 *  @param pcmd_buf      A pointer to cmd buf
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ops_uap_prepare_cmd(IN t_void *priv,
                                     IN t_u16 cmd_no,
                                     IN t_u16 cmd_action,
                                     IN t_u32 cmd_oid,
                                     IN t_void *pioctl_buf,
                                     IN t_void *pdata_buf,
                                     IN t_void *pcmd_buf)
{
    HostCmd_DS_COMMAND *cmd_ptr = (HostCmd_DS_COMMAND *)pcmd_buf;
    mlan_private *pmpriv        = (mlan_private *)priv;
    mlan_status ret             = MLAN_STATUS_SUCCESS;
    pmlan_ioctl_req pioctl_req  = (mlan_ioctl_req *)pioctl_buf;

    ENTER();

    /* Prepare command */
    switch (cmd_no)
    {
        case HostCMD_APCMD_ACS_SCAN:
        case HostCmd_CMD_SOFT_RESET:
        case HOST_CMD_APCMD_BSS_STOP:
        case HOST_CMD_APCMD_SYS_INFO:
        case HOST_CMD_APCMD_SYS_RESET:
        case HOST_CMD_APCMD_STA_LIST:
            cmd_ptr->command = wlan_cpu_to_le16(cmd_no);
            cmd_ptr->size    = wlan_cpu_to_le16(S_DS_GEN);
            break;
        case HOST_CMD_APCMD_BSS_START:
            ret = wlan_uap_cmd_bss_start(pmpriv, cmd_ptr);
            break;
        case HOST_CMD_APCMD_SYS_CONFIGURE:
            ret = wlan_uap_cmd_sys_configure(pmpriv, cmd_ptr, cmd_action, (pmlan_ioctl_req)pioctl_buf, pdata_buf);
            break;
        case HostCmd_CMD_802_11_SNMP_MIB:
            ret = wlan_uap_cmd_snmp_mib(pmpriv, cmd_ptr, cmd_action, cmd_oid, (pmlan_ioctl_req)pioctl_buf, pdata_buf);
            break;
        case HostCmd_CMD_802_11D_DOMAIN_INFO:
            if (pmpriv->support_11d_APIs != NULL)
            {
                ret = pmpriv->support_11d_APIs->wlan_cmd_802_11d_domain_info_p(pmpriv, cmd_ptr, cmd_action);
            }
            break;
        case HOST_CMD_APCMD_STA_DEAUTH:
            ret = wlan_uap_cmd_sta_deauth(pmpriv, cmd_ptr, pdata_buf);
            break;
#if defined(WAPI_AP) || defined(HOST_AUTHENTICATOR) || (CONFIG_HOSTAPD)
        case HostCmd_CMD_802_11_KEY_MATERIAL:
            ret = wlan_uap_cmd_key_material(pmpriv, cmd_ptr, cmd_action, cmd_oid, pdata_buf);
            break;
#endif
        case HostCmd_CMD_11N_CFG:
            ret = wlan_cmd_11n_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
        case HostCmd_CMD_11N_ADDBA_REQ:
            ret = wlan_cmd_11n_addba_req(pmpriv, cmd_ptr, pdata_buf);
            break;
        case HostCmd_CMD_11N_DELBA:
            ret = wlan_cmd_11n_delba(pmpriv, cmd_ptr, pdata_buf);
            break;
#if UAP_HOST_MLME
        case HostCmd_CMD_ADD_NEW_STATION:
            ret = wlan_uap_cmd_add_station(pmpriv, cmd_ptr, cmd_action, (pmlan_ioctl_req)pioctl_buf);
            break;
#endif
        case HostCmd_CMD_TX_RATE_CFG:
            ret = wlan_cmd_tx_rate_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf, (pmlan_ioctl_req)pioctl_buf);
            break;
        case HostCmd_CMD_802_11_TX_RATE_QUERY:
            cmd_ptr->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_TX_RATE_QUERY);
            cmd_ptr->size    = wlan_cpu_to_le16(sizeof(HostCmd_TX_RATE_QUERY) + S_DS_GEN);
            pmpriv->tx_rate  = 0;
            ret              = MLAN_STATUS_SUCCESS;
            break;
        case HostCmd_CMD_11AC_CFG:
            ret = wlan_cmd_11ac_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
#if CONFIG_WIFI_CLOCKSYNC
        case HostCmd_GPIO_TSF_LATCH_PARAM_CONFIG:
            ret = wlan_cmd_gpio_tsf_latch(pmpriv, cmd_ptr, cmd_action, pioctl_buf, pdata_buf);
            break;
#endif
#if CONFIG_11AX
        case HostCmd_CMD_11AX_CMD:
            ret = (mlan_status)wlan_cmd_11ax_cmd(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
        case HostCmd_CMD_11AX_CFG:
            ret = (mlan_status)wlan_cmd_11ax_cfg(pmpriv, cmd_action, pdata_buf);
            break;
#if CONFIG_11AX_TWT
        case HostCmd_CMD_TWT_CFG:
            ret = wlan_cmd_twt_cfg(pmpriv, cmd_ptr, cmd_action, pdata_buf);
            break;
#endif /* CONFIG_11AX_TWT */
#endif /* CONFIG_11AX */
        default:
            PRINTM(MERROR, "PREP_CMD: unknown command- %#x\n", cmd_no);
            if (pioctl_req != NULL)
            {
                pioctl_req->status_code = MLAN_ERROR_CMD_INVALID;
            }
            ret = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return ret;
}
#endif /* CONFIG_NXP_WIFI_SOFTAP_SUPPORT */
