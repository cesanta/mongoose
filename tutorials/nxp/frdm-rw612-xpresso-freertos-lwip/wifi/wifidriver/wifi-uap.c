/** @file wifi-uap.c
 *
 *  @brief This file provides UAP related APIs.
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#if CONFIG_NXP_WIFI_SOFTAP_SUPPORT
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>
#include <wifi.h>
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#if CONFIG_WPA_SUPP
#include "wifi_nxp_internal.h"
#endif
#if CONFIG_HOSTAPD
#include "rtos_wpa_supp_if.h"
#endif
#include "wifi-internal.h"
#include "mlan_ieee.h"
#include <mlan_remap_mem_operations.h>

/* fixme: Some of the following options could be added to kconfig. While
   adding the ranges in kconfig use the ones given as macros in
   mlan_uap_cmdevent.c */
#ifdef RW610
#define UAP_BEACON_PERIOD 100U
#else
#define UAP_BEACON_PERIOD 200U
#endif
#define UAP_DTIM_PERIOD 1
#define MAX_RATES       14U

#if CONFIG_5GHz_SUPPORT
static uint8_t rates_5ghz[] = {0x8c, 0x98, 0xb0, 0x12, 0x24, 0x48, 0x60, 0x6c, 0x00};
#endif

static uint8_t rates_2ghz[] = {0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c, 0x00};

static uint8_t rates_2ghz_b[] = {0x82, 0x84, 0x8b, 0x96, 0x00};

#if CONFIG_11AC
/**
 * @brief initialize AP bss config
 * @param pmpriv            A pointer to mlan_private structure
 * @param band            BAND_5G/BAND_2GHZ
 * @return                0 -- success, otherwise fail
 */
static bool wifi_check_11ac_capability(mlan_private *pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    bool enable_11ac        = MFALSE;

    ENTER();
#if CONFIG_WIFI_CAPA
    if (!pmadapter->usr_dot_11ac_enable)
    {
        return enable_11ac;
    }
#endif
    if ((band == BAND_CONFIG_5GHZ) && !(pmadapter->fw_bands & BAND_AAC))
    {
        PRINTM(MCMND, "FW don't support 5G AC\n");
        LEAVE();
        return enable_11ac;
    }
    if ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAC))
    {
        PRINTM(MCMND, "FW don't support 2G AC");
        LEAVE();
        return enable_11ac;
    }
    enable_11ac = MTRUE;

    LEAVE();
    return enable_11ac;
}

#define VHT_CAP_11AC_MASK 0x007fffff

/**
 *  @brief enable/disable 11AC
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param band     band config
 *
 *  @return         0--success, otherwise failure
 */
static int wifi_uap_set_11ac_status(mlan_private *pmpriv, t_u8 action, t_u8 bandwidth, t_u8 channel)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
    mlan_ds_11ac_vht_cfg vht_cfg;

    (void)memset(&vht_cfg, 0, sizeof(vht_cfg));

#if CONFIG_5GHz_SUPPORT
    if (channel > MAX_CHANNELS_BG)
    {
        vht_cfg.band = BAND_SELECT_A;
    }
    else
    {
        vht_cfg.band = BAND_SELECT_BG;
    }
#else
    vht_cfg.band                     = BAND_SELECT_BG;
#endif
    vht_cfg.txrx = MLAN_RADIO_TXRX;

    vht_cfg.vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_a;
    if (action == MLAN_ACT_DISABLE)
    {
        vht_cfg.bwcfg = BW_FOLLOW_HTCAP;
        vht_cfg.vht_cap_info &= ~VHT_CAP_11AC_MASK;
        vht_cfg.vht_rx_mcs = vht_cfg.vht_tx_mcs = 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg           = MTRUE;
    }
    else
    {
        if (BANDWIDTH_80MHZ == bandwidth)
        {
            vht_cfg.bwcfg = BW_FOLLOW_VHTCAP;
        }
        else
        {
            vht_cfg.bwcfg = BW_FOLLOW_HTCAP;
        }

        vht_cfg.vht_cap_info &= ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#ifdef RW610
        vht_cfg.vht_cap_info &= ~DEFALUT_11AC_CAP_SHORTGI_80MHZ_RESET_MASK;
#endif
        vht_cfg.vht_tx_mcs            = pmadapter->usr_dot_11ac_mcs_support >> 16;
        vht_cfg.vht_rx_mcs            = pmadapter->usr_dot_11ac_mcs_support & 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg = MTRUE;
    }

    if ((GET_VHTCAP_MAXMPDULEN(vht_cfg.vht_cap_info)) != 0U)
    {
        RESET_11ACMAXMPDULEN(vht_cfg.vht_cap_info);
    }
    else
    {
        /** Do Nothing */
    }

    wifi_d("Uap:11ac=%d vht_cap_info=0x%x, vht_tx_mcs=0x%x, vht_rx_mcs=0x%x\r\n", action, vht_cfg.vht_cap_info,
           vht_cfg.vht_tx_mcs, vht_cfg.vht_rx_mcs);
    ret = (int)wlan_11ac_ioctl_vhtcfg(pmpriv, (t_u8)MLAN_ACT_SET, &vht_cfg);
    return ret;
}
#endif

#if CONFIG_11AX
/**
 * @brief initialize AP bss config
 * @param pmpriv            A pointer to mlan_private structure
 * @param band            BAND_5G/BAND_2GHZ
 * @return                0 -- success, otherwise fail
 */
static t_u8 wifi_check_11ax_capability(mlan_private *pmpriv, t_u8 band)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u8 enable_11ax        = MFALSE;

    ENTER();
#if CONFIG_WIFI_CAPA
    if (!pmadapter->usr_dot_11ax_enable)
    {
        return enable_11ax;
    }
#endif
    if ((band == BAND_CONFIG_5GHZ) && !(pmadapter->fw_bands & BAND_AAX))
    {
        PRINTM(MCMND, "FW don't support 5G AX\n");
        LEAVE();
        return enable_11ax;
    }
    if ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAX))
    {
        PRINTM(MCMND, "FW don't support 2G AX\n");
        LEAVE();
        return enable_11ax;
    }
    enable_11ax = MTRUE;
    PRINTM(MCMND, "enable_11ax=%d\n", enable_11ax);
    LEAVE();
    return enable_11ax;
}

/**
 *  @brief enable/disable 11AX
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param band     band config
 *
 *  @return         0--success, otherwise failure
 */
int wifi_uap_set_11ax_status(mlan_private *pmpriv, t_u8 action, t_u8 band, t_u8 bandwidth)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
    mlan_ds_11ax_he_cfg he_cfg;

    ENTER();
    if ((band == BAND_CONFIG_5GHZ && !(pmadapter->fw_bands & BAND_AAX)) ||
        ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAX)))
    {
        PRINTM(MERROR, "fw doesn't support 11ax\n");
        ret = -WM_FAIL;
        goto done;
    }
    memset(&he_cfg, 0, sizeof(he_cfg));
    if (band == BAND_CONFIG_5GHZ)
    {
        he_cfg.band = MBIT(1);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_he_cap, pmadapter->hw_hecap_len);
        if (bandwidth == BANDWIDTH_20MHZ)
        {
            he_cfg.he_cap.he_phy_cap[0] &= ~(MBIT(2));
        }
    }
    else if (band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL)
    {
        he_cfg.band = MBIT(0);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_2g_he_cap, pmadapter->hw_2g_hecap_len);
        if (bandwidth == BANDWIDTH_20MHZ)
        {
            he_cfg.he_cap.he_phy_cap[0] &= ~(MBIT(1));
            he_cfg.he_cap.he_phy_cap[0] &= ~(MBIT(2));
        }
    }
    else
    {
        PRINTM(MERROR, "Invalid band!\n");
        ret = -WM_E_INVAL;
        goto done;
    }

#ifdef RW610
    he_cfg.he_cap.he_phy_cap[0] &= ~DEFAULT_11AX_CAP_40MHZIH2_4GHZBAND_RESET_MASK;
#endif

#if CONFIG_11AX_TWT
    /* uap mode clear TWT request bit */
    he_cfg.he_cap.he_mac_cap[0] &= ~HE_MAC_CAP_TWT_REQ_SUPPORT;
#endif

    if (action == MLAN_ACT_DISABLE)
    {
        if (he_cfg.he_cap.len && (he_cfg.he_cap.ext_id == HE_CAPABILITY))
            memset(he_cfg.he_cap.he_txrx_mcs_support, 0xff, sizeof(he_cfg.he_cap.he_txrx_mcs_support));
        else
        {
            PRINTM(MCMND, "11ax already disabled\n");
            goto done;
        }
    }
    DBG_HEXDUMP(MCMD_D, "HE_CFG ", (t_u8 *)&he_cfg, sizeof(he_cfg));
    ret = wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_SET, &he_cfg);
done:
    LEAVE();
    return ret;
}
#endif /* CONFIG_11AX */

void wifi_uap_clear_domain_info()
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd                     = wifi_get_command_buffer();
    HostCmd_DS_802_11D_DOMAIN_INFO *domain_info = (HostCmd_DS_802_11D_DOMAIN_INFO *)((t_u8 *)cmd + S_DS_GEN);

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_UAP);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11D_DOMAIN_INFO);
    cmd->size    = S_DS_GEN + sizeof(domain_info->action) + sizeof(MrvlIEtypesHeader_t);

    domain_info->action             = HostCmd_ACT_GEN_SET;
    domain_info->domain.header.type = wlan_cpu_to_le16(TLV_TYPE_DOMAIN);

    wifi_wait_for_cmdresp(NULL);
}

int wifi_uap_prepare_and_send_cmd(mlan_private *pmpriv,
                                  t_u16 cmd_no,
                                  t_u16 cmd_action,
                                  t_u32 cmd_oid,
                                  t_void *pioctl_buf,
                                  t_void *pdata_buf,
                                  mlan_bss_type bss_type,
                                  void *priv)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (uint16_t)bss_type);
    cmd->result  = 0x0;

    mlan_status rv = wlan_ops_uap_prepare_cmd(pmpriv, cmd_no, cmd_action, cmd_oid, pioctl_buf, pdata_buf, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_w("Failed to prepare cmd for uAP");
        (void)wifi_put_command_lock();
        return (int)rv;
    }

    (void)wifi_wait_for_cmdresp(priv);
    return wm_wifi.cmd_resp_status;
}

/*
 * Note: wlan_uap_domain_info() and wlan_uap_callback_domain_info() are the
 * original function which handles this functionality. However, it does it
 * through and IOCTL and its control flow will not work in our case. To
 * elaborate, it first gets the channel number by sending a command to
 * firmware. Then in the cmd resp handler to sends the domain info
 * command. As per the current design of our driver we cannot send command
 * from command resp handler. Hence, we have modified the control flow to
 * suit our design.
 *
 * This Api is set as callback and called during uap start phase,
 * getting region code by pmadapter.
 * Then it sends 80211 domain info command to firmware
 */
int wifi_uap_downld_domain_params(int band)
{
    int rv;
    mlan_adapter *pmadapter = mlan_adap;
    mlan_private *priv_uap   = pmadapter->priv[1];
    int region_code          = pmadapter->region_code;
    const t_u8 *country_code = NULL;
    int chan_spacing = 1;
    int no_of_chan = 0;
    t_u16 first_chan = 0, next_chan = 0, max_tx_power = 0;
    wlan_802_11d_domain_reg_t *pdomain = &pmadapter->domain_reg;
    const chan_freq_power_t *cfp;

    /* get band and sub band lists */
#if CONFIG_5GHz_SUPPORT
    if (band == BAND_A)
    {
        chan_spacing = 4;
        no_of_chan = pmadapter->region_channel[BAND_5GHZ].num_cfp;
        cfp = pmadapter->region_channel[BAND_5GHZ].pcfp;
    }
    else
#endif
    {
        no_of_chan = pmadapter->region_channel[BAND_2GHZ].num_cfp;
        cfp = pmadapter->region_channel[BAND_2GHZ].pcfp;
    }

    /* get country code string from region code */
    country_code = wlan_11d_code_2_region(pmadapter, (t_u8)region_code);
    if (country_code == NULL)
    {
        wuap_e("wifi_uap_downld_domain_params get country_code from region_code failed");
        return -WM_FAIL;
    }

    (void)__memset(pmadapter, pdomain, 0, sizeof(wlan_802_11d_domain_reg_t));
    (void)__memcpy(pmadapter, pdomain->country_code, country_code, COUNTRY_CODE_LEN);
    pdomain->band = band;

    for (int i = 0; i < no_of_chan; i++)
    {
        if (first_chan && next_chan &&
            next_chan + chan_spacing == cfp[i].channel &&
            max_tx_power == cfp[i].max_tx_power)
        {
            next_chan = cfp[i].channel;
            continue;
        }

        if (first_chan && next_chan)
        {
            pdomain->sub_band[pdomain->no_of_sub_band].first_chan = first_chan;
            pdomain->sub_band[pdomain->no_of_sub_band].no_of_chan =
                    (next_chan - first_chan) / chan_spacing + 1;
            pdomain->sub_band[pdomain->no_of_sub_band].max_tx_pwr = max_tx_power;
            pdomain->no_of_sub_band ++;
            first_chan = 0;
        }

        first_chan = next_chan = cfp[i].channel;
        max_tx_power = cfp[i].max_tx_power;
    }

    if (first_chan)
    {
        pdomain->sub_band[pdomain->no_of_sub_band].first_chan = first_chan;
        pdomain->sub_band[pdomain->no_of_sub_band].no_of_chan =
                (next_chan - first_chan) / chan_spacing + 1;
        pdomain->sub_band[pdomain->no_of_sub_band].max_tx_pwr = max_tx_power;
        pdomain->no_of_sub_band ++;
    }
    rv = wifi_uap_prepare_and_send_cmd(priv_uap, HostCmd_CMD_802_11D_DOMAIN_INFO, HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                       MLAN_BSS_TYPE_UAP, NULL);
    if (rv != 0)
    {
        wuap_w("Unable to send uap domain info");
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

static int wifi_cmd_uap_config(char *ssid,
                               t_u8 *mac_addr,
                               enum wlan_security_type security,
                               int key_mgmt,
                               char *passphrase,
                               char *password,
                               t_u8 channel,
                               wifi_scan_chan_list_t scan_chan_list,
                               t_u8 pwe_derivation,
                               t_u8 transition_disable,
                               t_u16 beacon_period,
                               t_u8 bandwidth,
                               t_u8 dtim_period,
                               t_u8 chan_sw_count,
                               mlan_bss_type bss_type,
                               bool mfpc,
                               bool mfpr)
{
    t_u32 ssid_len = strlen(ssid);
    uint8_t i;
    const t_u8 wmm_oui[4] = {0x00, 0x50, 0xf2, 0x02};
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    int ret;
    t_u8 supported_mcs_set[] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif
#if CONFIG_11AC
    bool enable_11ac = MFALSE;
#endif
#if CONFIG_11AX
    t_u8 enable_11ax = MFALSE;
#endif

    if (!(security == WLAN_SECURITY_NONE || security == WLAN_SECURITY_WPA2 ||
          security == WLAN_SECURITY_WPA_WPA2_MIXED || security == WLAN_SECURITY_WPA3_SAE ||
          security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
#if CONFIG_DRIVER_OWE
          || security == WLAN_SECURITY_OWE_ONLY
#endif
          ))
    {
        return -WM_E_INVAL;
    }

#if !CONFIG_WPA_SUPP
    int passphrase_len = (int)strlen(passphrase);
    int password_len   = (int)strlen(password);
#endif

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    wifi_uap_set_beacon_period(beacon_period);

    /* fixme: check if this needs to go on heap */
    mlan_ds_bss bss;
    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));

    bss.sub_command = MLAN_OID_UAP_BSS_CONFIG;

    bss.param.bss_config.ssid.ssid_len = ssid_len;
    (void)memcpy((void *)bss.param.bss_config.ssid.ssid, (const void *)ssid, ssid_len);

    if (mac_addr != NULL)
    {
        (void)memcpy((void *)bss.param.bss_config.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    }

    if (bss_type == MLAN_BSS_TYPE_UAP)
    { /* Not required for WFD */
        bss.param.bss_config.beacon_period = beacon_period;
        bss.param.bss_config.dtim_period   = dtim_period;
        if (!wm_wifi.hidden_ssid)
            bss.param.bss_config.bcast_ssid_ctl = 1;
        else if (wm_wifi.hidden_ssid == 1)
            bss.param.bss_config.bcast_ssid_ctl = 0;
        else if (wm_wifi.hidden_ssid == 2)
            bss.param.bss_config.bcast_ssid_ctl = 2;

        if (chan_sw_count != 0U)
        {
            bss.param.bss_config.dtim_period   = 1;
            bss.param.bss_config.chan_sw_count = chan_sw_count;
        }
    }
    /* Auto channel selection is not handled in 5GHz band, only
     * manual channel selection is supported right now.
     */
    if (channel != 0U)
    {
#if CONFIG_5GHz_SUPPORT
        if (channel > MAX_CHANNELS_BG)
        {
            mlan_private *priv_sta = (mlan_private *)mlan_adap->priv[0];
            if ((priv_sta->media_connected == MFALSE) && wlan_11h_radar_detect_required(pmpriv, channel))
            {
                wuap_e("Cannot start uAP on DFS channel %d", channel);
                return -WM_E_INVAL;
            }
#if CONFIG_UNII4_BAND_SUPPORT
            /* TODO: Temporary work around until firmware fix is available */
            if (channel == 173)
            {
                bss.param.bss_config.band_cfg = BAND_CONFIG_CH_173;
            }
            else if((channel == 169) || (channel == 177))
            {
                bss.param.bss_config.band_cfg = BAND_CONFIG_CH_169_177;
            }
            else
#endif
            {
                bss.param.bss_config.band_cfg = BAND_CONFIG_5GHZ;
            }
            (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_5ghz, sizeof(rates_5ghz));
        }
        else
        {
            if (channel == 14)
            {
                (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz_b, sizeof(rates_2ghz_b));
            }
            else
            {
                (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz, sizeof(rates_2ghz));
            }
            bss.param.bss_config.band_cfg = BAND_CONFIG_MANUAL;
        }
#else
        if (channel == 14)
        {
            (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz_b, sizeof(rates_2ghz_b));
        }
        else
        {
            (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_2ghz, sizeof(rates_2ghz));
        }
        bss.param.bss_config.band_cfg = BAND_CONFIG_MANUAL;
#endif
        bss.param.bss_config.channel = channel;
    }
    else
    {
        /* Auto channel selection from all channels*/
        bss.param.bss_config.band_cfg = BAND_CONFIG_ACS_MODE;
        bss.param.bss_config.channel  = 0;

        if (scan_chan_list.num_of_chan != 0U)
        {
            /* Specify channels if any for Auto channel selection */
            bss.param.bss_config.num_of_chan = scan_chan_list.num_of_chan;
            for (i = 0; i < scan_chan_list.num_of_chan; i++)
            {
                bss.param.bss_config.chan_list[i].chan_number = scan_chan_list.chan_number[i];
#if CONFIG_5GHz_SUPPORT
                if (scan_chan_list.chan_number[i] > MAX_CHANNELS_BG)
                {
                    bss.param.bss_config.chan_list[i].band_config_type = BAND_CONFIG_5GHZ;
                    (void)memcpy((void *)bss.param.bss_config.rates, (const void *)rates_5ghz, sizeof(rates_5ghz));
                }
                else
#endif
                {
                    bss.param.bss_config.chan_list[i].band_config_type = 0x10;
                    (void)memcpy(bss.param.bss_config.rates, rates_2ghz, sizeof(rates_2ghz));
                }
            }
        }
    }

#if CONFIG_11AC
    enable_11ac = wifi_check_11ac_capability(pmpriv, bss.param.bss_config.band_cfg);
#endif
#if CONFIG_11AX
    enable_11ax = wifi_check_11ax_capability(pmpriv, bss.param.bss_config.band_cfg);
#endif
#if !CONFIG_WPA_SUPP
    if (security == WLAN_SECURITY_NONE)
    {
        bss.param.bss_config.protocol = PROTOCOL_NO_SECURITY;
    }

    if (security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA_WPA2_MIXED ||
        security == WLAN_SECURITY_WPA3_SAE || security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
#if CONFIG_DRIVER_OWE
        || security == WLAN_SECURITY_OWE_ONLY
#endif
    )
    {
        bss.param.bss_config.wpa_cfg.pairwise_cipher_wpa2 = CIPHER_AES_CCMP;
        bss.param.bss_config.wpa_cfg.group_cipher         = CIPHER_AES_CCMP;
        if (security == WLAN_SECURITY_WPA2)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA2;
            if (key_mgmt & WLAN_KEY_MGMT_PSK)
            {
                bss.param.bss_config.key_mgmt = KEY_MGMT_PSK;
            }
            if (key_mgmt & WLAN_KEY_MGMT_PSK_SHA256)
            {
                bss.param.bss_config.key_mgmt |= KEY_MGMT_PSK_SHA256;
            }
        }
        else if (security == WLAN_SECURITY_WPA_WPA2_MIXED)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA2_MIXED;
            if (key_mgmt & WLAN_KEY_MGMT_PSK)
            {
                bss.param.bss_config.key_mgmt = KEY_MGMT_PSK;
            }
            if (key_mgmt & WLAN_KEY_MGMT_PSK_SHA256)
            {
                bss.param.bss_config.key_mgmt |= KEY_MGMT_PSK_SHA256;
            }
            bss.param.bss_config.wpa_cfg.pairwise_cipher_wpa  = CIPHER_TKIP | CIPHER_AES_CCMP;
            bss.param.bss_config.wpa_cfg.pairwise_cipher_wpa2 = CIPHER_TKIP | CIPHER_AES_CCMP;
            bss.param.bss_config.wpa_cfg.group_cipher         = CIPHER_TKIP;
        }
        else if (security == WLAN_SECURITY_WPA3_SAE)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA3_SAE;
            if (key_mgmt & WLAN_KEY_MGMT_SAE)
            {
                bss.param.bss_config.key_mgmt = KEY_MGMT_SAE;
            }
        }
        else if (security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
        {
            bss.param.bss_config.protocol = PROTOCOL_WPA2 | PROTOCOL_WPA3_SAE;
            if (key_mgmt & WLAN_KEY_MGMT_PSK)
            {
                bss.param.bss_config.key_mgmt = KEY_MGMT_PSK;
            }
            if (key_mgmt & WLAN_KEY_MGMT_PSK_SHA256)
            {
                bss.param.bss_config.key_mgmt |= KEY_MGMT_PSK_SHA256;
            }
            if (key_mgmt & WLAN_KEY_MGMT_SAE)
            {
                bss.param.bss_config.key_mgmt |= KEY_MGMT_SAE;
            }
        }
#if CONFIG_DRIVER_OWE
        else if (security == WLAN_SECURITY_OWE_ONLY)
        {
            bss.param.bss_config.protocol = PROTOCOL_OWE;
            if (key_mgmt & WLAN_KEY_MGMT_OWE)
            {
                bss.param.bss_config.key_mgmt = KEY_MGMT_OWE;
            }
        }
#endif
        else
        { /* Do Nothing */
        }
        /***************************************
         * Operation UINT16 Bits[15:2]: Reserved      *
         *                                  Bit[1]: Authenticator     *
         *                                  Bit[0]: KeyExchange      *
         ****************************************/
        bss.param.bss_config.key_mgmt_operation = 0x00;

            if (security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA_WPA2_MIXED ||
                security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
            {
                /*app has converted pmk with psk*/
                bss.param.bss_config.wpa_cfg.length = (t_u32)passphrase_len;
                (void)memcpy((void *)bss.param.bss_config.wpa_cfg.passphrase, (const void *)passphrase,
                             (size_t)passphrase_len);
            }
        if (security == WLAN_SECURITY_WPA3_SAE || security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED)
        {
            bss.param.bss_config.auth_mode               = MLAN_AUTH_MODE_AUTO;
            bss.param.bss_config.pwe_derivation          = pwe_derivation;
			/* For WPA3-SAE with pwe = either 1 or 2 value, need to set auth_mode = MLAN_AUTH_MODE_SAE, since FW is expecting this when SAE H2E elementes are required*/
			if(security == WLAN_SECURITY_WPA3_SAE && (pwe_derivation == 1 || pwe_derivation == 2))
			{
				bss.param.bss_config.auth_mode = MLAN_AUTH_MODE_SAE;
			}
            bss.param.bss_config.transition_disable      = transition_disable;
            bss.param.bss_config.wpa_cfg.password_length = (t_u32)password_len;
            (void)memcpy((void *)bss.param.bss_config.wpa_cfg.password, (const void *)password, (size_t)password_len);
        }
    }
#endif
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    bss.param.bss_config.ht_cap_info = wm_wifi.ht_cap_info == 0 ? (t_u16)0x112c : wm_wifi.ht_cap_info;
    wm_wifi.ht_tx_cfg                = wm_wifi.ht_tx_cfg == 0 ? (t_u16)0x002c : wm_wifi.ht_tx_cfg;

    if (bandwidth == BANDWIDTH_40MHZ
#if CONFIG_11AC
        || bandwidth == BANDWIDTH_80MHZ
#endif
    )
    {
        if (ISSUPP_CHANWIDTH40(mlan_adap->hw_dot_11n_dev_cap) != 0U)
        {
            bss.param.bss_config.ht_cap_info |= MBIT(1);
            wm_wifi.ht_tx_cfg |= MBIT(1);
            if (ISSUPP_SHORTGI40(mlan_adap->hw_dot_11n_dev_cap) != 0U)
            {
                bss.param.bss_config.ht_cap_info |= MBIT(6);
                wm_wifi.ht_tx_cfg |= MBIT(6);
            }
        }
    }
    else if (bandwidth == BANDWIDTH_20MHZ)
    {
        wm_wifi.ht_tx_cfg &= ~MBIT(1);
        wm_wifi.ht_tx_cfg &= ~MBIT(6);
        bss.param.bss_config.ht_cap_info &= ~MBIT(12);
    }
    else
    {
        /*Do Nothing*/
    }

    if (ISSUPP_RXLDPC(mlan_adap->hw_dot_11n_dev_cap) != 0U)
    {
        SETHT_LDPCCODINGCAP(bss.param.bss_config.ht_cap_info);
        SETHT_LDPCCODINGCAP(wm_wifi.ht_tx_cfg);
    }
    ret = wifi_uap_set_httxcfg_int(wm_wifi.ht_tx_cfg);
    if (ret != WM_SUCCESS)
    {
        wuap_e("Cannot set uAP HT TX Cfg:%x", wm_wifi.ht_tx_cfg);
        return -WM_E_INVAL;
    }

    if (!ISSUPP_TXSTBC(mlan_adap->hw_dot_11n_dev_cap))
        bss.param.bss_config.ht_cap_info &= (~MBIT(7));
    if (!ISSUPP_RXSTBC(mlan_adap->hw_dot_11n_dev_cap))
        bss.param.bss_config.ht_cap_info &= (~(MBIT(8) | MBIT(9)));
    if (!ISSUPP_CHANWIDTH40(mlan_adap->hw_dot_11n_dev_cap))
        bss.param.bss_config.ht_cap_info &= (~MBIT(12));

#ifdef RW610
    /* Set Tx Beam Forming Cap */
    bss.param.bss_config.tx_bf_cap = mlan_adap->priv[1]->tx_bf_cap;
#endif

#if CONFIG_11AC
    if (enable_11ac)
    {
        (void)wifi_uap_set_11ac_status(pmpriv, MLAN_ACT_ENABLE, bandwidth, channel);
    }
    else
    {
        (void)wifi_uap_set_11ac_status(pmpriv, MLAN_ACT_DISABLE, bandwidth, channel);
    }
#endif
#if CONFIG_11AX
    if (enable_11ax)
    {
        wifi_uap_set_11ax_status(pmpriv, MLAN_ACT_ENABLE, bss.param.bss_config.band_cfg, bandwidth);
    }
    else
    {
        wifi_uap_set_11ax_status(pmpriv, MLAN_ACT_DISABLE, bss.param.bss_config.band_cfg, bandwidth);
    }
#endif
#ifdef RW610_SERIES
    bss.param.bss_config.ampdu_param = 0x17;
#else
    bss.param.bss_config.ampdu_param = 0x03;
#endif
    (void)memcpy((void *)bss.param.bss_config.supported_mcs_set, (const void *)supported_mcs_set,
                 sizeof(bss.param.bss_config.supported_mcs_set));
#endif
    /*
     * Note that we are leaving htcap info set to zero by default. This
     *  will ensure that 11N is disabled.
     */

    memset(&bss.param.bss_config.wmm_para, 0x00, sizeof(wmm_parameter_t));

    memcpy(&bss.param.bss_config.wmm_para.ouitype, wmm_oui, sizeof(wmm_oui));
#if CONFIG_WIFI_CAPA
    if (pmpriv->adapter->usr_dot_11n_enable)
#endif
    {
        bss.param.bss_config.wmm_para.ouisubtype = 0x01;
        bss.param.bss_config.wmm_para.version    = 0x01;
        bss.param.bss_config.wmm_para.reserved   = 0x00;

        memcpy(&bss.param.bss_config.wmm_para.ac_params, &mlan_adap->ac_params,
               sizeof(wmm_ac_parameters_t) * MAX_AC_QUEUES);
        for (i = 0; i < MAX_AC_QUEUES; i++)
        {
            bss.param.bss_config.wmm_para.ac_params[i].tx_op_limit =
                wlan_cpu_to_le16(mlan_adap->ac_params[i].tx_op_limit);
        }
    }

    mlan_ioctl_req ioctl_buf;
    (void)memset(&ioctl_buf, 0x00, sizeof(mlan_ioctl_req));

    ioctl_buf.req_id = (t_u32)MLAN_IOCTL_BSS;
    /** Pointer to buffer */
    ioctl_buf.pbuf = (t_u8 *)&bss;

    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_SET, 0, &ioctl_buf, NULL,
                                         bss_type, NULL);
}

static wifi_uap_11d_apis_t wifi_uap_11d_apis = {
    .wifi_uap_downld_domain_params_p = wifi_uap_downld_domain_params,
};

int wifi_uap_enable_11d_support(void)
{
    wuap_d("Enabling 11d support APIs");

    wm_wifi.enable_11d_support   = true;
    wm_wifi.uap_support_11d_apis = &wifi_uap_11d_apis;
    return WM_SUCCESS;
}

int wifi_uap_ctrl_deauth(bool enable)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    return wifi_uap_prepare_and_send_cmd(pmpriv, HostCmd_CMD_802_11_SNMP_MIB, HostCmd_ACT_GEN_SET, (t_u32)StopDeauth_i,
                                         NULL, &enable, MLAN_BSS_TYPE_UAP, NULL);
}

void wifi_uap_set_beacon_period(const t_u16 beacon_period)
{
    wm_wifi.beacon_period = beacon_period;
}

int wifi_uap_set_bandwidth(const t_u8 bandwidth)
{
    if (bandwidth == BANDWIDTH_20MHZ || bandwidth == BANDWIDTH_40MHZ
#if CONFIG_11AC
        || bandwidth == BANDWIDTH_80MHZ
#endif
    )
    {
        wm_wifi.bandwidth = bandwidth;
        return WM_SUCCESS;
    }
    return (-WM_FAIL);
}

t_u8 wifi_uap_get_bandwidth()
{
    return wm_wifi.bandwidth;
}

void wifi_uap_set_hidden_ssid(const t_u8 hidden_ssid)
{
    wm_wifi.hidden_ssid = hidden_ssid;
}

void wifi_uap_set_ecsa(void)
{
#if defined(SD8801)
    wm_wifi.chan_sw_count = 7;
#endif
}

void wifi_uap_set_htcapinfo(const t_u16 ht_cap_info)
{
    wm_wifi.ht_cap_info = ht_cap_info;
}

void wifi_uap_set_httxcfg(const t_u16 ht_tx_cfg)
{
    wm_wifi.ht_tx_cfg = ht_tx_cfg;
}

#if CONFIG_WPA_SUPP
/**
 * @brief Get second channel offset
 *
 * @param chan            channel num
 * @return                second channel offset
 */
t_u8 wlan_get_second_channel_offset(mlan_private *priv, int chan)
{
    t_u8 chan2Offset = SEC_CHAN_NONE;

    /* Special Case: 20Mhz-only Channel */
#if CONFIG_UNII4_BAND_SUPPORT
    if (priv->adapter->region_code != COUNTRY_CODE_US && chan == 165)
#else
    if (chan == 165)
#endif
        return chan2Offset;

    switch (chan)
    {
        case 36:
        case 44:
        case 52:
        case 60:
        case 100:
        case 108:
        case 116:
        case 124:
        case 132:
        case 140:
        case 149:
        case 157:
#if CONFIG_UNII4_BAND_SUPPORT
        case 165:
        case 173:
#endif
            chan2Offset = SEC_CHAN_ABOVE;
            break;
        case 40:
        case 48:
        case 56:
        case 64:
        case 104:
        case 112:
        case 120:
        case 128:
        case 136:
        case 144:
        case 153:
        case 161:
#if CONFIG_UNII4_BAND_SUPPORT
        case 169:
        case 177:
#endif
            chan2Offset = SEC_CHAN_BELOW;
            break;
    }
    return chan2Offset;
}

t_u8 wifi_get_sec_channel_offset(unsigned int chan)
{
    mlan_private *pmpriv    = (mlan_private *)mlan_adap->priv[1];
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 band;
    t_u8 chan_offset;

#if CONFIG_5GHz_SUPPORT
    if (chan > MAX_CHANNELS_BG)
    {
        band = BAND_AN;
    }
    else
    {
        band = BAND_GN;
    }
#else
    band = BAND_GN;
#endif

    chan_offset = SEC_CHAN_ABOVE;

    if (band & BAND_GN)
    {
        if ((chan == 1) || (chan == 2) || (chan == 3) || (chan == 4))
            chan_offset = SEC_CHAN_ABOVE;
        else if ((chan == 10) || (chan == 11) || (chan == 12) || (chan == 13))
            chan_offset = SEC_CHAN_BELOW;
        else if (chan == 14)
            chan_offset = SEC_CHAN_NONE;

        /* check if channel 12 is supported in the region */
        if (!wlan_find_cfp_by_band_and_channel(pmadapter, band, 12))
            if ((chan == 8) || (chan == 9))
                chan_offset = SEC_CHAN_BELOW;
    }
#if CONFIG_5GHz_SUPPORT
    else if (band & BAND_AN)
        chan_offset = wlan_get_second_channel_offset(pmpriv, chan);
#endif

    return chan_offset;
}
#endif

#if CONFIG_WIFI_CAPA
void wifi_uap_config_wifi_capa(uint8_t wlan_capa)
{
    if (wlan_capa & WIFI_SUPPORT_LEGACY)
    {
        if (wlan_capa & WIFI_SUPPORT_11N)
        {
            mlan_adap->usr_dot_11n_enable = MTRUE;
#if CONFIG_11AC
            if (wlan_capa & WIFI_SUPPORT_11AC)
            {
                mlan_adap->usr_dot_11ac_enable = MTRUE;
#if CONFIG_11AX
                if (wlan_capa & WIFI_SUPPORT_11AX)
                {
                    mlan_adap->usr_dot_11ax_enable = MTRUE;
                }
                else
                {
                    mlan_adap->usr_dot_11ax_enable = MFALSE;
                }
#endif
            }
            else
#endif
            {
#if CONFIG_11AC
                mlan_adap->usr_dot_11ac_enable = MFALSE;
#endif
#if CONFIG_11AX
                mlan_adap->usr_dot_11ax_enable = MFALSE;
#endif
            }
        }
        else
        {
            mlan_adap->usr_dot_11n_enable = MFALSE;
#if CONFIG_11AC
            mlan_adap->usr_dot_11ac_enable = MFALSE;
#endif
#if CONFIG_11AX
            mlan_adap->usr_dot_11ax_enable = MFALSE;
#endif
        }
    }
    else
    {
        wuap_e("Invalid wifi capaibility setting\n");
    }
    return;
}
#endif

int wifi_uap_start(mlan_bss_type type,
                   char *ssid,
                   uint8_t *mac_addr,
                   int security,
                   int key_mgmt,
                   char *passphrase,
                   char *password,
                   int channel,
                   wifi_scan_chan_list_t scan_chan_list,
                   uint8_t pwe_derivation,
                   uint8_t transition_disable,
                   bool mfpc,
#if CONFIG_WIFI_DTIM_PERIOD
                   bool mfpr,
                   uint8_t dtim
#else
                   bool mfpr
#endif
)
{
    wuap_d("Configuring");
#if CONFIG_11AC
    if ((BANDWIDTH_80MHZ == wm_wifi.bandwidth) && (channel <= MAX_CHANNELS_BG) && (channel != 0))
    {
        wuap_e(
            "Cannot start uAP if bandwidth is configured to 80MHz, "
            "while channel is selected automatically or set to less than %d.",
            MAX_CHANNELS_BG);
        return -WM_FAIL;
    }
    else
    {
        /** Do Nothing */
    }
#endif
    if (channel == 14 || channel == 165)
    {
        wm_wifi.bandwidth = BANDWIDTH_20MHZ;
    }

    /* Configure SSID */
    int rv = wifi_cmd_uap_config(ssid, mac_addr, (enum wlan_security_type)security, key_mgmt, passphrase, password,
                                 (t_u8)channel, scan_chan_list, pwe_derivation, transition_disable,
                                 wm_wifi.beacon_period == 0U ? UAP_BEACON_PERIOD : wm_wifi.beacon_period,
                                 wm_wifi.bandwidth == 0U ? BANDWIDTH_40MHZ : wm_wifi.bandwidth,
#if CONFIG_WIFI_DTIM_PERIOD
                                 dtim == 0 ? UAP_DTIM_PERIOD : dtim,
#else
                                 UAP_DTIM_PERIOD,
#endif
                                 wm_wifi.chan_sw_count, type, mfpc, mfpr);

    if (rv != WM_SUCCESS)
    {
        wuap_e("config failed. Cannot start");
        return rv;
    }

    if ((security == WLAN_SECURITY_WPA2 || security == WLAN_SECURITY_WPA_WPA2_MIXED ||
         security == WLAN_SECURITY_WPA3_SAE || security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED
#if CONFIG_DRIVER_OWE
         || security == WLAN_SECURITY_OWE_ONLY
#endif
         ))
    {
        (void)wifi_uap_pmf_getset(HostCmd_ACT_GEN_SET, (uint8_t *)&mfpc, (uint8_t *)&mfpr);
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    mlan_private *remain_priv = pmpriv->adapter->priv[pmpriv->adapter->remain_bss_index];
    if (wifi_is_remain_on_channel() && remain_priv)
    {
        wuap_d("Cancel Remain on Channel before Starting AP");
        wifi_remain_on_channel(false, 0, 0);
    }

    if (wm_wifi.enable_11d_support && wm_wifi.uap_support_11d_apis)
    {
        wuap_d("Downloading domain params");
        wm_wifi.uap_support_11d_apis->wifi_uap_downld_domain_params_p(BAND_B);
#if CONFIG_5GHz_SUPPORT
        wm_wifi.uap_support_11d_apis->wifi_uap_downld_domain_params_p(BAND_A);
#endif
    }

    wuap_d("Starting BSS");
    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_BSS_START, HostCmd_ACT_GEN_SET, 0, NULL, NULL, type,
                                         NULL);
}

int wifi_uap_bss_sta_list(wifi_sta_list_t **list)
{
    if (list == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];

    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_STA_LIST, HostCmd_ACT_GEN_GET, 0, NULL, NULL,
                                         MLAN_BSS_TYPE_UAP, list);

    /* *list must have been filled now if everything went well */
}


int wifi_sta_deauth(uint8_t *mac_addr, uint16_t reason_code)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    mlan_deauth_param deauth;

    if (mac_addr == MNULL)
    {
        return -WM_FAIL;
    }

    (void)memcpy((void *)deauth.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    deauth.reason_code = reason_code;

    if (pmpriv->media_connected == MFALSE)
    {
        return -WM_FAIL;
    }

    /* Start BSS */
    return wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_STA_DEAUTH, HostCmd_ACT_GEN_SET, 0, NULL, &deauth,
                                         MLAN_BSS_TYPE_UAP, NULL);
}

int wifi_uap_stop()
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    int rv               = 0;

    /* Start BSS */
    rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_BSS_STOP, HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                       MLAN_BSS_TYPE_UAP, NULL);
    wifi_uap_clear_domain_info();

    return rv;
}

#ifdef SD8801
static int wifi_uap_acs_config_set()
{
    uint8_t i     = 0;
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_ACS_CONFIG) - 1U;
    uint8_t active_chan_list[40];
    uint8_t active_num_chans                      = 0;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list = NULL;
    HostCmd_DS_COMMAND *cmd;

    wifi_get_active_channel_list(active_chan_list, &active_num_chans, 0);

    (void)wifi_get_command_lock();

    cmd = wifi_get_command_buffer();

    memset(cmd, 0x00, size);

    cmd->command                   = wlan_cpu_to_le16(HostCmd_MMH_ACS_CFG);
    HostCmd_DS_ACS_CONFIG *acs_cmd = (HostCmd_DS_ACS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                   = acs_cmd->tlv_buffer;

    tlv_chan_list = (MrvlIEtypes_ChanListParamSet_t *)(void *)tlv;

    memset(tlv_chan_list, 0x00, sizeof(MrvlIEtypesHeader_t) + active_num_chans * sizeof(ChanScanParamSet_t));

    tlv_chan_list->header.type = TLV_TYPE_CHANLIST;
    tlv_chan_list->header.len  = active_num_chans * sizeof(ChanScanParamSet_t);

    for (i = 0; i < active_num_chans; i++)
    {
        tlv_chan_list->chan_scan_param[i].chan_number   = active_chan_list[i];
        tlv_chan_list->chan_scan_param[i].min_scan_time = MRVDRV_ACTIVE_SCAN_CHAN_TIME;
        tlv_chan_list->chan_scan_param[i].max_scan_time = MRVDRV_ACTIVE_SCAN_CHAN_TIME;
    }

    size += sizeof(tlv_chan_list->header) + tlv_chan_list->header.len;

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(NULL);

    return WM_SUCCESS;
}
#endif

#if CONFIG_HOSTAPD
int wifi_uap_do_acs(const int *freq_list)
#else
int wifi_uap_do_acs(const t_u16 acs_band)
#endif
{
#ifndef SD8801
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
#endif
    MrvlIEtypes_channel_band_t *tlv_chan_band     = MNULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list = MNULL;
    ChanScanParamSet_t *pscan_chan                = MNULL;
    t_u8 *tlv                                     = MNULL;
    uint8_t active_chan_list[WIFI_MAX_CHANNEL_NUM];
    uint8_t active_num_chans = 0;
    t_u16 scan_chan_num      = 0;
    t_u16 cmd_size;
    int i;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd           = wifi_get_command_buffer();
    HostCmd_DS_SYS_CONFIG *sys_config = (HostCmd_DS_SYS_CONFIG *)&cmd->params.sys_config;

    cmd->command       = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    cmd->seq_num       = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_TYPE_UAP);
    cmd->result        = 0x00;
    sys_config->action = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);
    cmd_size           = sizeof(HostCmd_DS_SYS_CONFIG) - 1 + S_DS_GEN;

    /* set band config tlv */
    tlv                        = (t_u8 *)sys_config->tlv_buffer;
    tlv_chan_band              = (MrvlIEtypes_channel_band_t *)tlv;
    tlv_chan_band->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_CHAN_BAND_CONFIG);
    tlv_chan_band->header.len  = wlan_cpu_to_le16(sizeof(t_u8) + sizeof(t_u8));
    tlv_chan_band->band_config = BAND_CONFIG_ACS_MODE;
    tlv_chan_band->channel     = 0;
    cmd_size += sizeof(MrvlIEtypes_channel_band_t);
    tlv += sizeof(MrvlIEtypes_channel_band_t);

    /* set scan channel list tlv */
    tlv_chan_list              = (MrvlIEtypes_ChanListParamSet_t *)tlv;
    tlv_chan_list->header.type = wlan_cpu_to_le16(TLV_TYPE_CHANLIST);
    pscan_chan                 = tlv_chan_list->chan_scan_param;

#if CONFIG_HOSTAPD
    /* fill in scan channel list */
    if (freq_list && freq_list[0] != 0)
    {
        /* use hostapd channel list */
        for (i = 0; i < WIFI_MAX_CHANNEL_NUM; i++)
        {
            if (freq_list[i] == 0)
                break;
            /* Following condition occurs in JP region where CH14 is allowed only in b mode.
             * But now we already have asked hostAPD to start with HT/VHT/HE modes, CH14 will not work here.
             * To avoid this, CH14 is omitted in ACS scan. Manual start of uAP in CH 14 is expected to use CH14 in JP region.
             */
            if (i == 13)
                continue;
            (void)memset(pscan_chan, 0x00, sizeof(ChanScanParamSet_t));
            pscan_chan->chan_number = freq_to_chan(freq_list[i]);
            pscan_chan->radio_type  = freq_list[i] >= 5180 ? BAND_5GHZ : BAND_2GHZ;
            pscan_chan++;
        }
        scan_chan_num = i;
        /* Following condition occurs in JP region where CH14 is allowed only in b mode.
         * But now we already have asked hostAPD to start with HT/VHT/HE modes, CH14 will not work here.
         * To avoid this, CH14 is omitted in ACS scan. Manual start of uAP in CH 14 is expected to use CH14 in JP region.
         */
        if (scan_chan_num == 14)
            scan_chan_num = 13;
    }
    else
#endif
    {
        /* create our own scan channel list on default 2.4G, as error protection */
#if CONFIG_HOSTAPD
        wifi_get_active_channel_list(active_chan_list, &active_num_chans, BAND_2GHZ);
#else
        wifi_get_active_channel_list(active_chan_list, &active_num_chans, acs_band);
#endif

        if (active_num_chans != 0 && active_num_chans < WIFI_MAX_CHANNEL_NUM)
        {
            for (i = 0; i < active_num_chans; i++)
            {
                (void)memset(pscan_chan, 0x00, sizeof(ChanScanParamSet_t));
                pscan_chan->chan_number = active_chan_list[i];
                pscan_chan->radio_type  = BAND_2GHZ;
                pscan_chan++;
            }
            scan_chan_num = active_num_chans;
        }
    }

    tlv_chan_list->header.len = wlan_cpu_to_le16(scan_chan_num * sizeof(ChanScanParamSet_t));
    cmd_size += sizeof(tlv_chan_list->header) + (scan_chan_num * sizeof(ChanScanParamSet_t));
    tlv += sizeof(tlv_chan_list->header) + (scan_chan_num * sizeof(ChanScanParamSet_t));

    cmd->size = (t_u16)wlan_cpu_to_le16(cmd_size);

    wifi_wait_for_cmdresp(NULL);

    /* Start ACS SCAN */
#ifdef SD8801
    return wifi_uap_acs_config_set();
#else
    return wifi_uap_prepare_and_send_cmd(pmpriv, HostCMD_APCMD_ACS_SCAN, HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                         MLAN_BSS_TYPE_UAP, NULL);
#endif
}

#if CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM
/*
 * The following configuration was added because of a particular
 * bug on 8787 and 8782 firmware. The firmware bugzilla ID for the
 * bug is 39609. The bug causes the firmware to send packets to the
 * STA even after giving ACK for IEEE PS. The prov. mode scanning
 * coupled with this bug causes large scale packet losses and TCP
 * backoffs finally resulting in long load times for HTTP served
 * pages for prov. UI.
 */

/** TLV header length */
#define TLVHEADER_LEN 4

#define MRVL_STICKY_TIM_CONFIG_TLV_ID (PROPRIETARY_TLV_BASE_ID + 0x96)
/** TLV : Sticky TIM MAC address */
#define MRVL_STICKY_TIM_STA_MAC_ADDR_TLV_ID (PROPRIETARY_TLV_BASE_ID + 0x97)

/** TLV buffer : sticky tim config */
typedef PACK_START struct _tlvbuf_sticky_tim_config
{
    /** Header */
    t_u16 tag;
    /** Length */
    t_u16 length;
    /** Enable */
    t_u16 enable;
    /** Duration */
    t_u16 duration;
    /** Sticky Bitmask */
    t_u16 sticky_bitmask;
} PACK_END tlvbuf_sticky_tim_config;

/** TLV buffer : sticky tim sta mac address */
typedef PACK_START struct _tlvbuf_sticky_tim_sta_mac_addr
{
    /** Header */
    t_u16 tag;
    /** Length */
    t_u16 length;
    /** Control */
    t_u16 control;
    /** Station MAC address */
    t_u8 sta_mac_address[MLAN_MAC_ADDR_LENGTH];
} PACK_END tlvbuf_sticky_tim_sta_mac_addr;

/*
 * This configures sticky TIM configuration for given MAC.
 *
 * Note that this is not present in mlan. So we have to add it here.
 */
void wifi_uap_enable_sticky_bit(const uint8_t *mac_addr)
{
    static bool global_sticky_enabled;

    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_SET;
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    if (!global_sticky_enabled)
    {
        /*
         * This enables the global sticky TIM bit enable. This
         * needs to be done only once.
         */
        tlvbuf_sticky_tim_config *tlv_sticky_tim_cfg = (tlvbuf_sticky_tim_config *)tlv;

        tlv_sticky_tim_cfg->tag    = MRVL_STICKY_TIM_CONFIG_TLV_ID;
        tlv_sticky_tim_cfg->length = sizeof(tlvbuf_sticky_tim_config) - TLVHEADER_LEN;
        tlv_sticky_tim_cfg->enable = 1;
        /* Set it permanently */
        tlv_sticky_tim_cfg->duration = 0;
        /* MoreData bit and TIM bit is made sticky */
        tlv_sticky_tim_cfg->sticky_bitmask = (t_u16)0x3;

        size += sizeof(MrvlIEtypesHeader_t) + tlv_sticky_tim_cfg->length;
        tlv += sizeof(MrvlIEtypesHeader_t) + tlv_sticky_tim_cfg->length;

        global_sticky_enabled = true;
    }

    tlvbuf_sticky_tim_sta_mac_addr *tim_cfg = (tlvbuf_sticky_tim_sta_mac_addr *)tlv;

    tim_cfg->tag    = MRVL_STICKY_TIM_STA_MAC_ADDR_TLV_ID;
    tim_cfg->length = sizeof(tlvbuf_sticky_tim_sta_mac_addr) - TLVHEADER_LEN;
    (void)memcpy((void *)tim_cfg->sta_mac_address, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    tim_cfg->control = 1;

    size += sizeof(MrvlIEtypesHeader_t) + tim_cfg->length;

    cmd->size    = size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    wifi_wait_for_cmdresp(NULL);
}
#endif /* CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM */

/*
 * Note: This function handles only one (first) TLV from the response.
 */
void wifi_uap_handle_cmd_resp(HostCmd_DS_COMMAND *resp)
{
    mlan_private *pmpriv              = (mlan_private *)mlan_adap->priv[1];
    HostCmd_DS_SYS_CONFIG *sys_config = (HostCmd_DS_SYS_CONFIG *)&resp->params.sys_config;
    uint8_t *tlv                      = sys_config->tlv_buffer;
    MrvlIEtypesHeader_t *header       = (MrvlIEtypesHeader_t *)(void *)tlv;
    if (resp->result == 0U)
    {
        switch (header->type)
        {
            case TLV_TYPE_UAP_TX_POWER:
            {
                MrvlIEtypes_tx_power_t *tlv_tx_power = (MrvlIEtypes_tx_power_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint8_t *tx_power     = (uint8_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        *tx_power             = tlv_tx_power->tx_power;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_STA_AGEOUT_TIMER:
            {
                MrvlIEtypes_sta_ageout_t *tlv_sta_ageout_timer = (MrvlIEtypes_sta_ageout_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *sta_ageout_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv      = NULL;
                        *sta_ageout_timer          = tlv_sta_ageout_timer->sta_ageout_timer;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER:
            {
                MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout_timer = (MrvlIEtypes_ps_sta_ageout_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *ps_sta_ageout_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv         = NULL;
                        *ps_sta_ageout_timer          = tlv_ps_sta_ageout_timer->ps_sta_ageout_timer;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_GRP_REKEY_TIME:
            {
                MrvlIEtypes_group_rekey_time_t *tlv_group_rekey_timer = (MrvlIEtypes_group_rekey_time_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint32_t *group_rekey_timer = (uint32_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv       = NULL;
                        *group_rekey_timer          = tlv_group_rekey_timer->gk_rekey_time;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_MCBC_DATA_RATE:
            {
                MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *)(void *)tlv;
                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        uint16_t *mcbc_rate   = (uint16_t *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        *mcbc_rate            = tlv_mcbc_rate->mcbc_data_rate;
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_RATES:
            {
                MrvlIEtypes_RatesParamSet_t *tlv_rates = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;

                if (sys_config->action == HostCmd_ACT_GEN_GET)
                {
                    if (wm_wifi.cmd_resp_priv != NULL)
                    {
                        char *rates           = (char *)wm_wifi.cmd_resp_priv;
                        wm_wifi.cmd_resp_priv = NULL;
                        (void)memset((void *)rates, 0, MAX_RATES);
                        (void)memcpy((void *)rates, (const void *)tlv_rates->rates,
                                     MIN(MAX_RATES, tlv_rates->header.len));
                    }
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_CHAN_BAND_CONFIG:
            {
                MrvlIEtypes_channel_band_t *tlv_cb    = (MrvlIEtypes_channel_band_t *)(void *)tlv;
                pmpriv->uap_state_chan_cb.band_config = tlv_cb->band_config;
                pmpriv->uap_state_chan_cb.channel     = tlv_cb->channel;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    t_u8 *channel         = (t_u8 *)wm_wifi.cmd_resp_priv;
                    wm_wifi.cmd_resp_priv = NULL;
                    *channel              = tlv_cb->channel;
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            case TLV_TYPE_UAP_MAX_STA_CNT:
            {
                MrvlIEtypes_max_sta_count_t *tlv_sta_cnt = (MrvlIEtypes_max_sta_count_t *)(void *)tlv;
                pmpriv->uap_state_chan_cb.max_sta_count  = tlv_sta_cnt->max_sta_count;
                pmpriv->uap_max_sta                      = tlv_sta_cnt->max_sta_count;
                if (wm_wifi.cmd_resp_priv != NULL)
                {
                    int *sta_count        = (int *)wm_wifi.cmd_resp_priv;
                    wm_wifi.cmd_resp_priv = NULL;
                    *sta_count            = (int)tlv_sta_cnt->max_sta_count;
                }
                wm_wifi.cmd_resp_status = WM_SUCCESS;
            }
            break;
            default:
                wm_wifi.cmd_resp_status = WM_SUCCESS;
                break;
        }
    }
    else
    {
        wm_wifi.cmd_resp_status = -WM_FAIL;
    }
}

int wifi_uap_rates_getset(uint8_t action, char *rates, uint8_t num_rates)
{
    uint8_t i     = 0;
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_RatesParamSet_t *tlv_rates = (MrvlIEtypes_RatesParamSet_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_SET;
        for (i = 0; i < num_rates; i++)
        {
            tlv_rates->rates[i] = (t_u8)rates[i];
        }
    }

    tlv_rates->header.type = wlan_cpu_to_le16(TLV_TYPE_RATES);
    tlv_rates->header.len  = wlan_cpu_to_le16(i);

    size += sizeof(MrvlIEtypesHeader_t) + i;
    tlv += sizeof(MrvlIEtypesHeader_t) + i;

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? rates : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_mcbc_rate_getset(uint8_t action, uint16_t *mcbc_rate)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_mcbc_rate_t *tlv_mcbc_rate = (MrvlIEtypes_mcbc_rate_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action        = HostCmd_ACT_GEN_SET;
        tlv_mcbc_rate->mcbc_data_rate = *mcbc_rate;
    }

    tlv_mcbc_rate->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_MCBC_DATA_RATE);
    tlv_mcbc_rate->header.len  = wlan_cpu_to_le16(sizeof(t_u16));

    size += sizeof(MrvlIEtypes_mcbc_rate_t);
    tlv += sizeof(MrvlIEtypes_mcbc_rate_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? mcbc_rate : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_tx_power_getset(uint8_t action, uint8_t *tx_power_dbm)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_tx_power_t *tlv_tx_power = (MrvlIEtypes_tx_power_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_SET;
        tlv_tx_power->tx_power = *tx_power_dbm;
    }
    tlv_tx_power->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_TX_POWER);
    tlv_tx_power->header.len  = wlan_cpu_to_le16(sizeof(t_u8));

    size += sizeof(MrvlIEtypes_tx_power_t);
    tlv += sizeof(MrvlIEtypes_tx_power_t);

    cmd->size    = (uint16_t)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? tx_power_dbm : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_sta_ageout_timer_getset(uint8_t action, uint32_t *sta_ageout_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_sta_ageout_t *tlv_sta_ageout_timer = (MrvlIEtypes_sta_ageout_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action                 = HostCmd_ACT_GEN_SET;
        tlv_sta_ageout_timer->sta_ageout_timer = wlan_cpu_to_le32(*sta_ageout_timer);
    }
    tlv_sta_ageout_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_STA_AGEOUT_TIMER);
    tlv_sta_ageout_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_sta_ageout_t);
    tlv += sizeof(MrvlIEtypes_sta_ageout_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? sta_ageout_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_ps_sta_ageout_timer_getset(uint8_t action, uint32_t *ps_sta_ageout_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_ps_sta_ageout_t *tlv_ps_sta_ageout_timer = (MrvlIEtypes_ps_sta_ageout_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action                       = HostCmd_ACT_GEN_SET;
        tlv_ps_sta_ageout_timer->ps_sta_ageout_timer = wlan_cpu_to_le32(*ps_sta_ageout_timer);
    }
    tlv_ps_sta_ageout_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_PS_STA_AGEOUT_TIMER);
    tlv_ps_sta_ageout_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_ps_sta_ageout_t);
    tlv += sizeof(MrvlIEtypes_ps_sta_ageout_t);

    cmd->size    = (uint16_t)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? ps_sta_ageout_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_group_rekey_timer_getset(uint8_t action, uint32_t *group_rekey_timer)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_APCMD_SYS_CONFIGURE);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    uint8_t *tlv                          = sys_config_cmd->tlv_buffer;

    MrvlIEtypes_group_rekey_time_t *tlv_group_rekey_timer = (MrvlIEtypes_group_rekey_time_t *)(void *)tlv;

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_config_cmd->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_config_cmd->action               = HostCmd_ACT_GEN_SET;
        tlv_group_rekey_timer->gk_rekey_time = wlan_cpu_to_le32(*group_rekey_timer);
    }
    tlv_group_rekey_timer->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_GRP_REKEY_TIME);
    tlv_group_rekey_timer->header.len  = wlan_cpu_to_le16(sizeof(t_u32));

    size += sizeof(MrvlIEtypes_group_rekey_time_t);
    tlv += sizeof(MrvlIEtypes_group_rekey_time_t);

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? group_rekey_timer : NULL);

    return wm_wifi.cmd_resp_status;
}

/* Content reproduced from wlan_uap_get_channel() */
static int wifi_send_uap_get_channel_cmd(int *channel)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    t_u16 tlv_type       = TLV_TYPE_UAP_CHAN_BAND_CONFIG;

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_GET, 0, MNULL,
                                           &tlv_type, MLAN_BSS_TYPE_UAP, channel);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to uap channel");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

/* Content reproduced from wlan_uap_get_channel() */
static int wifi_send_uap_max_sta_num_cmd(uint8_t action, unsigned int *max_sta_num)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    MrvlIEtypes_max_sta_count_t max_sta_count_tlv;

    (void)memset(&max_sta_count_tlv, 0, sizeof(MrvlIEtypes_max_sta_count_t));

    max_sta_count_tlv.header.type = TLV_TYPE_UAP_MAX_STA_CNT;

    if (action == HostCmd_ACT_GEN_SET)
    {
        max_sta_count_tlv.header.len    = (t_u16)sizeof(MrvlIEtypes_max_sta_count_t);
        max_sta_count_tlv.max_sta_count = (t_u16)*max_sta_num;
    }

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, action, 0, MNULL, &max_sta_count_tlv,
                                           MLAN_BSS_TYPE_UAP, max_sta_num);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to uap max_sta_num");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

int wifi_get_uap_max_clients(unsigned int *max_sta_num)
{
    return wifi_send_uap_max_sta_num_cmd(HostCmd_ACT_GEN_GET, max_sta_num);
}

int wifi_set_uap_max_clients(unsigned int *max_sta_num)
{
    return wifi_send_uap_max_sta_num_cmd(HostCmd_ACT_GEN_SET, max_sta_num);
}

/*
 * Note: channel can be NULL. The side effects of this function also are of
 * interest to us as pmpriv->uap_state_chan_cb.band_config and
 * pmpriv->uap_state_chan_cb.channel are updated.
 */
int wifi_get_uap_channel(int *channel)
{
    return wifi_send_uap_get_channel_cmd(channel);
}

int wifi_uap_pmf_getset(uint8_t action, uint8_t *mfpc, uint8_t *mfpr)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_PMF_PARAMS);
    wifi_pmf_params_t wifi_pmf_params;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HostCmd_CMD_PMF_PARAMS);
    HostCmd_DS_PMF_PARAMS *sys_pmf_params = (HostCmd_DS_PMF_PARAMS *)((uint32_t)cmd + S_DS_GEN);

    (void)memset(sys_pmf_params, 0x00, sizeof(HostCmd_DS_PMF_PARAMS));

    if (action == HostCmd_ACT_GEN_GET)
    {
        sys_pmf_params->action = HostCmd_ACT_GEN_GET;
    }
    else
    {
        sys_pmf_params->action = HostCmd_ACT_GEN_SET;
    }

    sys_pmf_params->params.mfpc = *mfpc;
    sys_pmf_params->params.mfpr = *mfpr;

    cmd->size    = (t_u16)size;
    cmd->seq_num = (0x01) << 12;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? &wifi_pmf_params : NULL);

    if (action == HostCmd_ACT_GEN_GET)
    {
        *mfpc = wifi_pmf_params.mfpc;
        *mfpr = wifi_pmf_params.mfpr;
    }

    return wm_wifi.cmd_resp_status;
}

int wifi_uap_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr)
{
    return wifi_uap_pmf_getset(HostCmd_ACT_GEN_GET, mfpc, mfpr);
}

#if CONFIG_UAP_STA_MAC_ADDR_FILTER
int wifi_set_sta_mac_filter(int filter_mode, int mac_count, unsigned char *mac_addr)
{
    t_u8 *buffer  = NULL;
    t_u16 cmd_len = 0;
    t_u16 buf_len = MRVDRV_SIZE_OF_CMD_BUFFER;

    HostCmd_DS_GEN *cmd_buf           = NULL;
    MrvlIEtypes_mac_filter_t *tlv     = NULL;
    HostCmd_DS_SYS_CONFIG *sys_config = NULL;

    /* Initialize the command length */
    if (filter_mode == 0)
    {
        cmd_len = sizeof(HostCmd_DS_GEN) + (sizeof(HostCmd_DS_SYS_CONFIG) - 1) +
                  (sizeof(MrvlIEtypes_mac_filter_t) - 1) + (WLAN_MAX_STA_FILTER_NUM * MLAN_MAC_ADDR_LENGTH);
    }
    else
    {
        cmd_len = sizeof(HostCmd_DS_GEN) + (sizeof(HostCmd_DS_SYS_CONFIG) - 1) +
                  (sizeof(MrvlIEtypes_mac_filter_t) - 1) + mac_count * MLAN_MAC_ADDR_LENGTH;
    }

    /* Initialize the command buffer */
#if !CONFIG_MEM_POOLS
    buffer = (t_u8 *)OSA_MemoryAllocate(buf_len);
#else
    buffer = (t_u8 *)OSA_MemoryPoolAllocate(buf_2048_MemoryPool);
#endif
    if (!buffer)
    {
        wuap_e("ERR:Cannot allocate buffer for command!\r\n");
        return -WM_FAIL;
    }

    memset(buffer, 0, buf_len);

    /* Locate headers */
    cmd_buf    = (HostCmd_DS_GEN *)buffer;
    sys_config = (HostCmd_DS_SYS_CONFIG *)(buffer + sizeof(HostCmd_DS_GEN));
    tlv        = (MrvlIEtypes_mac_filter_t *)(buffer + sizeof(HostCmd_DS_GEN) + (sizeof(HostCmd_DS_SYS_CONFIG) - 1));

    /* Fill the command buffer */
    cmd_buf->command = HOST_CMD_APCMD_SYS_CONFIGURE;
    cmd_buf->size    = cmd_len;
    cmd_buf->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_TYPE_UAP);
    cmd_buf->result  = 0;

    sys_config->action = HostCmd_ACT_GEN_SET;

    tlv->count       = mac_count;
    tlv->filter_mode = filter_mode;
    tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_UAP_STA_MAC_ADDR_FILTER);

    if (tlv->count)
    {
        tlv->header.len = tlv->count * MLAN_MAC_ADDR_LENGTH + 2;
        (void)memcpy(tlv->mac_address, mac_addr, mac_count * MLAN_MAC_ADDR_LENGTH);
    }
    else
    {
        tlv->header.len = WLAN_MAX_STA_FILTER_NUM * MLAN_MAC_ADDR_LENGTH + 2;
    }

    if (is_uap_started())
    {
        wuap_e("down the uap before setting sta filter\n\r");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buffer);
#else
        OSA_MemoryPoolFree(buf_2048_MemoryPool, buffer);
#endif
        return -WM_FAIL;
    }

    wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memcpy((t_u8 *)cmd, cmd_buf, cmd_len);

    wifi_wait_for_cmdresp(NULL);

#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buffer);
#else
        OSA_MemoryPoolFree(buf_2048_MemoryPool, buffer);
#endif

    return WM_SUCCESS;
}

#endif

#if CONFIG_HOSTAPD

void wifi_uap_client_assoc(t_u8 *sta_addr, unsigned char is_11n_enabled)
{
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    wlan_request_ralist_lock(mlan_adap->priv[1]);
    /* Clear corresponding tx/rx table if necessary */
    if (wlan_11n_get_txbastream_tbl((mlan_private *)mlan_adap->priv[1], sta_addr))
    {
        wlan_11n_delete_txbastream_tbl_entry((mlan_private *)mlan_adap->priv[1], sta_addr);
    }

    wlan_cleanup_reorder_tbl((mlan_private *)mlan_adap->priv[1], sta_addr);
#if CONFIG_WMM
    wlan_ralist_del_enh(mlan_adap->priv[1], sta_addr);
#endif
    /* txbastream table also is used as connected STAs data base */
    wlan_11n_create_txbastream_tbl((mlan_private *)mlan_adap->priv[1], sta_addr, BA_STREAM_NOT_SETUP);
    wlan_11n_update_txbastream_tbl_tx_thresh((mlan_private *)mlan_adap->priv[1], sta_addr, 3);

    if (is_11n_enabled)
    {
        wlan_11n_update_txbastream_tbl_ampdu_supported((mlan_private *)mlan_adap->priv[1], sta_addr, MTRUE);
    }

    wlan_release_ralist_lock(mlan_adap->priv[1]);

#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */

#if CONFIG_WMM
    wlan_ralist_add_enh(mlan_adap->priv[1], sta_addr);
#endif
}

void wifi_uap_client_deauth(t_u8 *sta_addr)
{
#if (CONFIG_UAP_AMPDU_TX) || (CONFIG_UAP_AMPDU_RX)
    if ((mlan_private *)mlan_adap->priv[1]->is_11n_enabled)
    {
        wlan_cleanup_reorder_tbl((mlan_private *)mlan_adap->priv[1], sta_addr);
        wlan_request_ralist_lock((mlan_private *)mlan_adap->priv[1]);
        wlan_11n_delete_txbastream_tbl_entry((mlan_private *)mlan_adap->priv[1], sta_addr);
        wlan_release_ralist_lock((mlan_private *)mlan_adap->priv[1]);
    }
#endif /* CONFIG_UAP_AMPDU_TX || CONFIG_UAP_AMPDU_RX */

#if CONFIG_WMM
    wlan_ralist_del_enh(mlan_adap->priv[1], sta_addr);
#endif
}

/**
 * @brief Verify RSN IE
 *
 * @param rsn_ie          Pointer RSN IE
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                MTRUE/MFALSE
 */
static t_u8 wifi_check_rsn_ie(IEEEtypes_Rsn_t *rsn_ie, mlan_uap_bss_param *sys_config)
{
    int left                            = 0;
    int count                           = 0;
    int i                               = 0;
    wpa_suite_auth_key_mgmt_t *key_mgmt = NULL;
    left                                = rsn_ie->len + 2;
    if (left < (int)sizeof(IEEEtypes_Rsn_t))
        return MFALSE;
    sys_config->wpa_cfg.group_cipher         = 0;
    sys_config->wpa_cfg.pairwise_cipher_wpa2 = 0;
    sys_config->key_mgmt                     = 0;
    /* check the group cipher */
    switch (rsn_ie->group_cipher.type)
    {
        case WPA_CIPHER_TKIP2:
            sys_config->wpa_cfg.group_cipher = CIPHER_TKIP;
            break;
        case WPA_CIPHER_AES_CCM:
        case WPA_CIPHER_AES_GCM:
        case WPA_CIPHER_AES_CCM_256:
        case WPA_CIPHER_AES_GCM_256:
            sys_config->wpa_cfg.group_cipher = CIPHER_AES_CCMP;
            break;
        default:
            break;
    }
    count = wlan_le16_to_cpu(rsn_ie->pairwise_cipher.count);
    for (i = 0; i < count; i++)
    {
        switch (rsn_ie->pairwise_cipher.list[i].type)
        {
            case WPA_CIPHER_TKIP2:
                sys_config->wpa_cfg.pairwise_cipher_wpa2 |= CIPHER_TKIP;
                break;
            case WPA_CIPHER_AES_CCM:
            case WPA_CIPHER_AES_GCM:
            case WPA_CIPHER_AES_CCM_256:
            case WPA_CIPHER_AES_GCM_256:
                sys_config->wpa_cfg.pairwise_cipher_wpa2 |= CIPHER_AES_CCMP;
                break;
            default:
                break;
        }
    }
    left -= sizeof(IEEEtypes_Rsn_t) + (count - 1) * sizeof(wpa_suite);
    if (left < (int)sizeof(wpa_suite_auth_key_mgmt_t))
        return MFALSE;
    key_mgmt = (wpa_suite_auth_key_mgmt_t *)((u8 *)rsn_ie + sizeof(IEEEtypes_Rsn_t) + (count - 1) * sizeof(wpa_suite));
    count    = wlan_le16_to_cpu(key_mgmt->count);
    if (left < (int)(sizeof(wpa_suite_auth_key_mgmt_t) + (count - 1) * sizeof(wpa_suite)))
        return MFALSE;
    for (i = 0; i < count; i++)
    {
        switch (key_mgmt->list[i].type)
        {
            case RSN_AKM_8021X:
            case RSN_AKM_8021X_SHA256:
            case RSN_AKM_8021X_SUITEB:
            case RSN_AKM_8021X_SUITEB_192:
#if CONFIG_11R
            case RSN_AKM_FT_8021X:
            case RSN_AKM_FT_8021X_SHA384:
#endif
                sys_config->key_mgmt |= KEY_MGMT_EAP;
                break;
            case RSN_AKM_PSK:
#if CONFIG_11R
            case RSN_AKM_FT_PSK:
#endif
                sys_config->key_mgmt |= KEY_MGMT_PSK;
                break;
            case RSN_AKM_PSK_SHA256:
                sys_config->key_mgmt |= KEY_MGMT_PSK_SHA256;
                break;
#if UAP_HOST_MLME
            case RSN_AKM_SAE:
#if CONFIG_11R
            case RSN_AKM_FT_SAE:
#endif
                sys_config->key_mgmt |= KEY_MGMT_SAE;
                break;
            case RSN_AKM_OWE:
#if CONFIG_DRIVER_OWE
                sys_config->key_mgmt |= KEY_MGMT_OWE;
#endif
                break;
#endif
        }
    }
    return MTRUE;
}

/**
 *  @brief Verify WPA IE`
 *  @param wpa_ie          Pointer WPA IE
 *  @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                MTRUE/MFALSE
 */
static t_u8 wifi_check_wpa_ie(IEEEtypes_Wpa_t *wpa_ie, mlan_uap_bss_param *sys_config)
{
    int left                            = 0;
    int count                           = 0;
    int i                               = 0;
    wpa_suite_auth_key_mgmt_t *key_mgmt = NULL;
    left                                = wpa_ie->len + 2;
    if (left < (int)sizeof(IEEEtypes_Wpa_t))
        return MFALSE;
    sys_config->wpa_cfg.group_cipher        = 0;
    sys_config->wpa_cfg.pairwise_cipher_wpa = 0;
    switch (wpa_ie->group_cipher.type)
    {
        case WPA_CIPHER_TKIP2:
            sys_config->wpa_cfg.group_cipher = CIPHER_TKIP;
            break;
        case WPA_CIPHER_AES_CCM:
            sys_config->wpa_cfg.group_cipher = CIPHER_AES_CCMP;
            break;
        default:
            break;
    }
    count = wlan_le16_to_cpu(wpa_ie->pairwise_cipher.count);
    for (i = 0; i < count; i++)
    {
        switch (wpa_ie->pairwise_cipher.list[i].type)
        {
            case WPA_CIPHER_TKIP2:
                sys_config->wpa_cfg.pairwise_cipher_wpa |= CIPHER_TKIP;
                break;
            case WPA_CIPHER_AES_CCM:
                sys_config->wpa_cfg.pairwise_cipher_wpa |= CIPHER_AES_CCMP;
                break;
            default:
                break;
        }
    }
    left -= sizeof(IEEEtypes_Wpa_t) + (count - 1) * sizeof(wpa_suite);
    if (left < (int)sizeof(wpa_suite_auth_key_mgmt_t))
        return MFALSE;
    key_mgmt = (wpa_suite_auth_key_mgmt_t *)((u8 *)wpa_ie + sizeof(IEEEtypes_Wpa_t) + (count - 1) * sizeof(wpa_suite));
    count    = wlan_le16_to_cpu(key_mgmt->count);
    if (left < (int)(sizeof(wpa_suite_auth_key_mgmt_t) + (count - 1) * sizeof(wpa_suite)))
        return MFALSE;
    for (i = 0; i < count; i++)
    {
        switch (key_mgmt->list[i].type)
        {
            case RSN_AKM_8021X:
                sys_config->key_mgmt = KEY_MGMT_EAP;
                break;
            case RSN_AKM_PSK:
                sys_config->key_mgmt = KEY_MGMT_PSK;
                break;
        }
    }
    return MTRUE;
}

/**
 * @brief Find RSN/WPA IES
 *
 * @param ie              Pointer IE buffer
 * @param sys_config      Pointer to mlan_uap_bss_param structure
 * @return                MTRUE/MFALSE
 */
static t_u8 wifi_find_wpa_ies(const t_u8 *ie, int len, mlan_uap_bss_param *sys_config)
{
    int bytes_left           = len;
    const t_u8 *pcurrent_ptr = ie;
    t_u16 total_ie_len;
    t_u8 element_len;
    t_u8 wpa2 = 0;
    t_u8 wpa  = 0;
    t_u8 ret  = MFALSE;
    IEEEtypes_ElementId_e element_id;
    IEEEtypes_VendorSpecific_t *pvendor_ie;
    const t_u8 wpa_oui[4] = {0x00, 0x50, 0xf2, 0x01};
    while (bytes_left >= 2)
    {
        element_id   = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
        element_len  = *((t_u8 *)pcurrent_ptr + 1);
        total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len)
        {
            PRINTM(MERROR, "InterpretIE: Error in processing IE, bytes left < IE length\n");
            bytes_left = 0;
            continue;
        }
        switch (element_id)
        {
            case RSN_IE:
                wpa2 = wifi_check_rsn_ie((IEEEtypes_Rsn_t *)pcurrent_ptr, sys_config);
                break;
            case VENDOR_SPECIFIC_221:
                pvendor_ie = (IEEEtypes_VendorSpecific_t *)pcurrent_ptr;
                if (!memcmp(pvendor_ie->vend_hdr.oui, wpa_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    (pvendor_ie->vend_hdr.oui_type == wpa_oui[3]))
                {
                    wpa = wifi_check_wpa_ie((IEEEtypes_Wpa_t *)pcurrent_ptr, sys_config);
                }
                break;
            default:
                break;
        }
        pcurrent_ptr += element_len + 2;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2);
    }
    if (wpa && wpa2)
    {
        sys_config->protocol = PROTOCOL_WPA | PROTOCOL_WPA2;
        ret                  = MTRUE;
    }
    else if (wpa2)
    {
        sys_config->protocol = PROTOCOL_WPA2;
        ret                  = MTRUE;
    }
    else if (wpa)
    {
        sys_config->protocol = PROTOCOL_WPA;
        ret                  = MTRUE;
    }
    return ret;
}

/**
 * @brief Find and set WMM IES
 *  @param priv            Pointer to moal_private
 *  @param ie              Pointer IE buffer
 *  @param sys_config      Pointer to mlan_uap_bss_param structure
 *
 * @return                N/A
 *
 **/
static t_void wifi_set_wmm_ies(mlan_private *priv, const t_u8 *ie, int len, mlan_uap_bss_param *sys_config)
{
    int bytes_left           = len;
    const t_u8 *pcurrent_ptr = ie;
    t_u16 total_ie_len;
    t_u8 element_len;
    IEEEtypes_VendorSpecific_t *pvendor_ie;
    IEEEtypes_ElementId_e element_id;
    const t_u8 wmm_oui[4] = {0x00, 0x50, 0xf2, 0x02};

    while (bytes_left >= 2)
    {
        element_id   = (IEEEtypes_ElementId_e)(*((t_u8 *)pcurrent_ptr));
        element_len  = *((t_u8 *)pcurrent_ptr + 1);
        total_ie_len = element_len + sizeof(IEEEtypes_Header_t);
        if (bytes_left < total_ie_len)
        {
            PRINTM(MERROR, "InterpretIE: Error in processing IE, bytes left < IE length\n");
            bytes_left = 0;
            continue;
        }
        switch (element_id)
        {
            case VENDOR_SPECIFIC_221:
                pvendor_ie = (IEEEtypes_VendorSpecific_t *)pcurrent_ptr;
                if (!memcmp(pvendor_ie->vend_hdr.oui, wmm_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    pvendor_ie->vend_hdr.oui_type == wmm_oui[3])
                {
                    if (total_ie_len == sizeof(IEEEtypes_WmmParameter_t))
                    {
#if CONFIG_WIFI_CAPA
                        if (priv->adapter->usr_dot_11n_enable)
#endif
                        {
                            /*
                             * Only accept and copy the WMM IE if
                             * it matches the size expected for the
                             * WMM Parameter IE.
                             */
                            memcpy(&sys_config->wmm_para, pcurrent_ptr + sizeof(IEEEtypes_Header_t), element_len);

                            /** Disable U-APSD for now */
                            sys_config->wmm_para.qos_info &= 0x7F;

                            /** set uap_host_based_config to true */
                            sys_config->uap_host_based_config = MTRUE;
                        }
#if CONFIG_WIFI_CAPA
                        else
                        {
                            memset(sys_config->wmm_para.ac_params, 0x00, sizeof(wmm_ac_parameters_t) * MAX_AC_QUEUES);
                        }
#endif
                    }
                }

                break;
            default:
                break;
        }
        pcurrent_ptr += element_len + 2;
        /* Need to account for IE ID and IE Len */
        bytes_left -= (element_len + 2);
    }
}

/**
 *  @brief Set/Get system configuration parameters
 *
 *  @param priv             A pointer to moal_private structure
 *  @param action           MLAN_ACT_SET or MLAN_ACT_GET
 *  @param wait_option      Wait option
 *  @param sys_cfg          A pointer to mlan_uap_bss_param structure
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wifi_set_get_sys_config(mlan_private *priv, t_u16 action, mlan_uap_bss_param *sys_cfg)
{
    int ret;
    mlan_ds_bss bss;
    mlan_ioctl_req ioctl_buf;

    ENTER();

    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));

    bss.sub_command = MLAN_OID_UAP_BSS_CONFIG;

    if (action == MLAN_ACT_SET)
        memcpy(&bss.param.bss_config, sys_cfg, sizeof(mlan_uap_bss_param));

    (void)memset(&ioctl_buf, 0x00, sizeof(mlan_ioctl_req));

    ioctl_buf.req_id = (t_u32)MLAN_IOCTL_BSS;
    /** Pointer to buffer */
    ioctl_buf.pbuf = (t_u8 *)&bss;

    ret = wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_SET, 0, &ioctl_buf, NULL,
                                        MLAN_BSS_TYPE_UAP, NULL);
    if (ret != WM_SUCCESS)
    {
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

#define IE_MASK_WPS    0x0001
#define IE_MASK_P2P    0x0002
#define IE_MASK_WFD    0x0004
#define IE_MASK_VENDOR 0x0008
#define IE_MASK_EXTCAP 0x0010

/**
 * @brief get specific ie
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 * @param ie_out          Pointer to out IE buf
 * @param ie_out_len    Total length of ie_out
 * @param mask            IE mask
 *
 * @return                out IE length
 */
static t_u16 wifi_get_specific_ie(const t_u8 *ie, int len, t_u8 *ie_out, t_u32 ie_out_len, t_u16 mask)
{
    int left_len    = len;
    const t_u8 *pos = ie;
    int length;
    t_u8 id                                = 0;
    t_u16 out_len                          = 0;
    IEEEtypes_VendorSpecific_t *pvendor_ie = NULL;
    const u8 wps_oui[4]                    = {0x00, 0x50, 0xf2, 0x04};
    const u8 p2p_oui[4]                    = {0x50, 0x6f, 0x9a, 0x09};
    const u8 wfd_oui[4]                    = {0x50, 0x6f, 0x9a, 0x0a};
    const t_u8 wmm_oui[4]                  = {0x00, 0x50, 0xf2, 0x02};

    ENTER();
    while (left_len >= 2)
    {
        length = *(pos + 1);
        id     = *pos;
        if ((length + 2) > left_len)
            break;
        if (id == VENDOR_SPECIFIC_221)
        {
            pvendor_ie = (IEEEtypes_VendorSpecific_t *)pos;
            if (!memcmp(pvendor_ie->vend_hdr.oui, wmm_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                pvendor_ie->vend_hdr.oui_type == wmm_oui[3])
            {
                PRINTM(MIOCTL, "find WMM IE\n");
            }
            else if (!memcmp(pvendor_ie->vend_hdr.oui, p2p_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                     pvendor_ie->vend_hdr.oui_type == p2p_oui[3])
            {
                if (mask & IE_MASK_P2P)
                {
                    /** only get first p2p ie here */
                    __memcpy(NULL, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                    break;
                }
            }
            else if (!memcmp(pvendor_ie->vend_hdr.oui, wps_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                     pvendor_ie->vend_hdr.oui_type == wps_oui[3])
            {
                if (mask & IE_MASK_WPS)
                {
                    if ((out_len + length + 2) < (int)ie_out_len)
                    {
                        __memcpy(NULL, ie_out + out_len, pos, length + 2);
                        out_len += length + 2;
                    }
                    else
                    {
                        PRINTM(MERROR, "get_specific_ie: IE too big, fail copy WPS IE\n");
                        break;
                    }
                }
            }
            else if (!memcmp(pvendor_ie->vend_hdr.oui, wfd_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                     pvendor_ie->vend_hdr.oui_type == wfd_oui[3])
            {
                if (mask & IE_MASK_WFD)
                {
                    if ((out_len + length + 2) < (int)ie_out_len)
                    {
                        __memcpy(NULL, ie_out + out_len, pos, length + 2);
                        out_len += length + 2;
                    }
                    else
                    {
                        PRINTM(MERROR, "get_specific_ie: IE too big, fail copy WFD IE\n");
                        break;
                    }
                }
            }
            else if (mask & IE_MASK_VENDOR)
            {
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(NULL, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    PRINTM(MERROR, "get_specific_ie:IE too big, fail copy VENDOR IE\n");
                    break;
                }
            }
        }
        pos += (length + 2);
        left_len -= (length + 2);
    }
    LEAVE();
    return out_len;
}

/**
 * @brief Find specific IE from IE buffer
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 * @param spec_ie         Pointer to specific IE buffer
 * @param spec_len        Total length of specific IE
 *
 * @return                out IE length
 */
static t_u8 wifi_find_ie(const t_u8 *ie, int len, const t_u8 *spec_ie, int spec_len)
{
    int left_len    = len;
    const t_u8 *pos = ie;
    int length;

    while (left_len >= 2)
    {
        length = *(pos + 1);
        if ((length + 2) > left_len)
            break;
        if ((length + 2) == spec_len)
        {
            if (!memcmp(pos, spec_ie, spec_len))
                return MTRUE;
        }
        pos += (length + 2);
        left_len -= (length + 2);
    }
    return MFALSE;
}

/**
 * @brief Filter specific IE in ie buf
 *
 * @param priv            pointer to moal private structure
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 * @param ie_out		  Pointer to out IE buf
 * @param ie_out_len      Total length of ie_out
 * @param wps_flag	      flag for wps/p2p
 * @param dup_ie          Pointer to duplicate ie
 * @param dup_ie_len	  duplicate IE len
 *
 * @return                out IE length
 */
static t_u16 wifi_filter_beacon_ies(mlan_private *priv,
                                    const t_u8 *ie,
                                    int len,
                                    t_u8 *ie_out,
                                    t_u32 ie_out_len,
                                    t_u16 wps_flag,
                                    const t_u8 *dup_ie,
                                    int dup_ie_len)
{
    int left_len    = len;
    const t_u8 *pos = ie;
    int length;
    t_u8 id                                = 0;
    t_u16 out_len                          = 0;
    IEEEtypes_VendorSpecific_t *pvendor_ie = NULL;
    const u8 wps_oui[4]                    = {0x00, 0x50, 0xf2, 0x04};
    const u8 p2p_oui[4]                    = {0x50, 0x6f, 0x9a, 0x09};
    const u8 wfd_oui[4]                    = {0x50, 0x6f, 0x9a, 0x0a};
    const t_u8 wmm_oui[4]                  = {0x00, 0x50, 0xf2, 0x02};
    t_u8 find_p2p_ie                       = MFALSE;
    t_u8 enable_11d                        = MFALSE;
#if CONFIG_11AX
    t_u8 ext_id = 0;
#endif
    // int ie_len;

    /* ERP_INFO/EXTENDED_SUPPORT_RATES/HT_CAPABILITY/HT_OPERATION/WMM
     * and WPS/P2P/WFD IE will be fileter out
     */
    while (left_len >= 2)
    {
        length = *(pos + 1);
        id     = *pos;
        if ((length + 2) > left_len)
            break;
        if (dup_ie && dup_ie_len && wifi_find_ie(dup_ie, dup_ie_len, pos, length + 2))
        {
            wifi_d("skip duplicate IE");
            pos += (length + 2);
            left_len -= (length + 2);
            continue;
        }
        switch (id)
        {
            case COUNTRY_INFO:
                enable_11d = MTRUE;
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(priv, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    wifi_d("IE too big, fail copy COUNTRY INFO IE");
                }
                break;
            case HT_CAPABILITY:
            case HT_OPERATION:
#if CONFIG_11AC
            case VHT_CAPABILITY:
            case VHT_OPERATION:
#endif
#if UAP_HOST_MLME
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(priv, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    wifi_d("IE too big, fail copy COUNTRY INFO IE");
                }
#endif
                break;
            case EXTENDED_SUPPORTED_RATES:
            case WLAN_EID_ERP_INFO:
                /* Fall Through */
            case REGULATORY_CLASS:
                /* Fall Through */
            case OVERLAPBSSSCANPARAM:
                break;
#if CONFIG_11AX
            case EXTENSION:
                ext_id = *(pos + 2);
#if UAP_SUPPORT
#if CONFIG_11AX
                if (ext_id == HE_CAPABILITY)
                {
                    mlan_ds_11ax_he_cfg he_cfg;
                    IEEEtypes_HECap_t *hecap_ie = NULL;
                    (void)memset((void *)&he_cfg, 0, sizeof(mlan_ds_11ax_he_cfg));

                    if (priv->uap_channel <= 14)
                        he_cfg.band = MBIT(0);
                    else
                        he_cfg.band = MBIT(1);

                    wifi_d("Retrieve 11ax cfg by channel=%d band=%d", priv->uap_channel, he_cfg.band);

                    if (0 == wlan_cmd_11ax_cfg(priv, HostCmd_ACT_GEN_GET, &he_cfg))
                    {
                        t_u16 he_cap_len;
                        hecap_ie                      = (IEEEtypes_HECap_t *)&he_cfg.he_cap.len;
                        he_cap_len                    = he_cfg.he_cap.len;
                        hecap_ie->ieee_hdr.len        = he_cap_len;
                        hecap_ie->ieee_hdr.element_id = he_cfg.he_cap.id;

                        __memcpy(priv, ie_out + out_len, hecap_ie, hecap_ie->ieee_hdr.len + 2);

                        out_len += hecap_ie->ieee_hdr.len + 2;
                    }
                    else
                    {
                        wifi_d("Fail to get 11ax he_cap parameters");
                    }
                }
                else
#endif
#endif
                {
                    if ((out_len + length + 2) < (int)ie_out_len)
                    {
                        __memcpy(priv, ie_out + out_len, pos, length + 2);
                        out_len += length + 2;
                    }
                    else
                    {
                        wifi_d("IE too big, fail copy EXTENSION IE");
                    }
                }
                break;
#endif
            case EXT_CAPABILITY:
                /* filter out EXTCAP */
                if (wps_flag & IE_MASK_EXTCAP)
                {
#if 0
                    ie_len = length + 2;
                    if (MLAN_STATUS_SUCCESS !=
                            wifi_set_get_gen_ie(priv, MLAN_ACT_SET,
                                (t_u8 *)pos, &ie_len,
                                MOAL_IOCTL_WAIT))
                        PRINTM(MERROR,
                                "Fail to set EXTCAP IE\n");
#endif
                    break;
                }
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(priv, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    wifi_d("IE too big, fail copy EXTCAP IE");
                }
                break;
            case VENDOR_SPECIFIC_221:
                /* filter out wmm ie */
                pvendor_ie = (IEEEtypes_VendorSpecific_t *)pos;
                if (!memcmp(pvendor_ie->vend_hdr.oui, wmm_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                    pvendor_ie->vend_hdr.oui_type == wmm_oui[3])
                {
                    break;
                }
                /* filter out wps ie */
                else if (!memcmp(pvendor_ie->vend_hdr.oui, wps_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                         pvendor_ie->vend_hdr.oui_type == wps_oui[3])
                {
                    if (wps_flag & IE_MASK_WPS)
                        break;
                }
                /* filter out first p2p ie */
                else if (!memcmp(pvendor_ie->vend_hdr.oui, p2p_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                         pvendor_ie->vend_hdr.oui_type == p2p_oui[3])
                {
                    if (!find_p2p_ie && (wps_flag & IE_MASK_P2P))
                    {
                        find_p2p_ie = MTRUE;
                        break;
                    }
                }
                /* filter out wfd ie */
                else if (!memcmp(pvendor_ie->vend_hdr.oui, wfd_oui, sizeof(pvendor_ie->vend_hdr.oui)) &&
                         pvendor_ie->vend_hdr.oui_type == wfd_oui[3])
                {
                    if (wps_flag & IE_MASK_WFD)
                        break;
                }
                else if (wps_flag & IE_MASK_VENDOR)
                {
                    // filter out vendor IE
                    break;
                }
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(priv, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    wifi_d("IE too big, fail copy VENDOR_SPECIFIC_221 IE");
                }
                break;
            default:
                if ((out_len + length + 2) < (int)ie_out_len)
                {
                    __memcpy(priv, ie_out + out_len, pos, length + 2);
                    out_len += length + 2;
                }
                else
                {
                    wifi_d("IE too big, fail copy %d IE", id);
                }
                break;
        }

        pos += (length + 2);
        left_len -= (length + 2);
    }

#if UAP_SUPPORT
    if (enable_11d && !priv->bss_started && !wlan_11d_is_enabled(priv))
    {
        wuap_d("Enable 11D support");
        wifi_enable_uap_11d_support();
    }
#endif
    return out_len;
}

static int wifi_nxp_set_mgmt_ies(mlan_private *priv,
                                 char *tail_ies,
                                 unsigned short tail_ies_len,
                                 char *beacon_ies,
                                 unsigned short beacon_ies_len,
                                 char *proberesp_ies,
                                 unsigned short proberesp_ies_len,
                                 char *assocresp_ies,
                                 unsigned short assocresp_ies_len)
{
    int ret        = WM_SUCCESS;
    const t_u8 *ie = NULL;
    t_u8 ie_buffer[MAX_IE_SIZE];
    t_u16 ie_len, ie_length = 0;
    custom_ie *beacon_ies_data     = NULL;
    custom_ie *beacon_wps_ies_data = NULL;
    custom_ie *proberesp_ies_data  = NULL;
    custom_ie *assocresp_ies_data  = NULL;

    beacon_ies_data     = (custom_ie *)OSA_MemoryAllocate(sizeof(custom_ie));
    beacon_wps_ies_data = (custom_ie *)OSA_MemoryAllocate(sizeof(custom_ie));
    proberesp_ies_data  = (custom_ie *)OSA_MemoryAllocate(sizeof(custom_ie));

    assocresp_ies_data = (custom_ie *)OSA_MemoryAllocate(sizeof(custom_ie));

    if ((!beacon_ies_data) || (!beacon_wps_ies_data) || (!proberesp_ies_data) || (!assocresp_ies_data))
    {
        if (beacon_ies_data)
        {
            OSA_MemoryFree(beacon_ies_data);
        }
        if (beacon_wps_ies_data)
        {
            OSA_MemoryFree(beacon_wps_ies_data);
        }
        if (proberesp_ies_data)
        {
            OSA_MemoryFree(proberesp_ies_data);
        }
        if (assocresp_ies_data)
        {
            OSA_MemoryFree(assocresp_ies_data);
        }
        return -WM_FAIL;
    }

    ie        = (const t_u8 *)tail_ies;
    ie_len    = tail_ies_len;
    ie_length = 0;

    if ((ie != NULL) && (ie_len != 0U))
    {
        if (priv->beacon_vendor_index != -1)
        {
            ret = wifi_clear_mgmt_ie2(MLAN_BSS_TYPE_UAP, priv->beacon_vendor_index);
            if (ret != WM_SUCCESS)
            {
                wuap_e("Clear uAP vendor IE failed");
                ret = -WM_FAIL;
                goto done;
            }
            priv->beacon_vendor_index = -1;
        }

        ie_length = wifi_get_specific_ie(ie, ie_len, ie_buffer, MAX_IE_SIZE, IE_MASK_VENDOR);
#if CONFIG_WIFI_IO_DUMP
        PRINTF("VENDOR IE\r\n");
        dump_hex(ie_buffer, ie_length);
#endif

        if (ie_length)
        {
            priv->beacon_vendor_index =
                wifi_set_mgmt_ie2(MLAN_BSS_TYPE_UAP, MGMT_MASK_BEACON | MGMT_MASK_ASSOC_RESP | MGMT_MASK_PROBE_RESP,
                                  (void *)ie_buffer, ie_length);

            if (priv->beacon_vendor_index == -1)
            {
                wuap_e("Set uAP vendor IE failed");
                ret = -WM_FAIL;
                goto done;
            }
        }

        ie_length = wifi_filter_beacon_ies(priv, ie, ie_len, ie_buffer, MAX_IE_SIZE,
                                           IE_MASK_WPS | IE_MASK_WFD | IE_MASK_P2P | IE_MASK_VENDOR,
                                           (const t_u8 *)proberesp_ies, proberesp_ies_len);
#if CONFIG_WIFI_IO_DUMP
        PRINTF("Beacon IE\r\n");
        dump_hex(ie_buffer, ie_length);
#endif
    }

    beacon_ies_data->ie_index = priv->beacon_index;

    if (ie_length)
    {
        beacon_ies_data->mgmt_subtype_mask = MGMT_MASK_BEACON | MGMT_MASK_ASSOC_RESP | MGMT_MASK_PROBE_RESP;
        beacon_ies_data->ie_length         = ie_length;
        memcpy(beacon_ies_data->ie_buffer, (void *)ie_buffer, ie_length);
    }
    else
    {
        beacon_ies_data->mgmt_subtype_mask = MGMT_MASK_CLEAR;
    }

    ie        = (const t_u8 *)beacon_ies;
    ie_len    = beacon_ies_len;
    ie_length = 0;

    if ((ie != NULL) && (ie_len != 0U))
    {
        ie_length = wifi_filter_beacon_ies(priv, ie, ie_len, ie_buffer, MAX_IE_SIZE, IE_MASK_VENDOR, NULL, 0);
#if CONFIG_WIFI_IO_DUMP
        PRINTF("Beacon WPS IE\r\n");
        dump_hex(ie_buffer, ie_length);
#endif
    }

    beacon_wps_ies_data->ie_index = priv->beacon_wps_index;
    if (ie_length)
    {
        beacon_wps_ies_data->mgmt_subtype_mask = MGMT_MASK_BEACON;
        beacon_wps_ies_data->ie_length         = ie_length;
        memcpy(beacon_wps_ies_data->ie_buffer, (void *)ie_buffer, ie_length);
    }
    else
    {
        beacon_wps_ies_data->mgmt_subtype_mask = MGMT_MASK_CLEAR;
    }

    ie        = (const t_u8 *)proberesp_ies;
    ie_len    = proberesp_ies_len;
    ie_length = 0;

    if ((ie != NULL) && (ie_len != 0U))
    {
        ie_length =
            wifi_filter_beacon_ies(priv, ie, ie_len, ie_buffer, MAX_IE_SIZE, IE_MASK_P2P | IE_MASK_VENDOR, NULL, 0);
#if CONFIG_WIFI_IO_DUMP
        PRINTF("ProbeResp IE\r\n");
        dump_hex(ie_buffer, ie_length);
#endif
    }

    proberesp_ies_data->ie_index = priv->proberesp_index;
    if (ie_length)
    {
        proberesp_ies_data->mgmt_subtype_mask = MGMT_MASK_PROBE_RESP;
        proberesp_ies_data->ie_length         = ie_length;
        memcpy(proberesp_ies_data->ie_buffer, (void *)ie_buffer, ie_length);
    }
    else
    {
        proberesp_ies_data->mgmt_subtype_mask = MGMT_MASK_CLEAR;
    }

    ie        = (const t_u8 *)assocresp_ies;
    ie_len    = assocresp_ies_len;
    ie_length = 0;

    if ((ie != NULL) && (ie_len != 0U))
    {
#if CONFIG_WIFI_IO_DUMP
        PRINTF("AssocResp IE\r\n");
        dump_hex(ie, ie_len);
#endif
    }

    assocresp_ies_data->ie_index = priv->assocresp_index;
    if (ie_len)
    {
        assocresp_ies_data->mgmt_subtype_mask = MGMT_MASK_ASSOC_RESP;
        assocresp_ies_data->ie_length         = ie_len;
        memcpy(assocresp_ies_data->ie_buffer, (void *)ie, ie_len);
    }
    else
    {
        assocresp_ies_data->mgmt_subtype_mask = MGMT_MASK_CLEAR;
    }

    ret = wifi_set_custom_ie(beacon_ies_data, beacon_wps_ies_data, proberesp_ies_data, assocresp_ies_data);
    if (ret != WM_SUCCESS)
    {
        ret = -WM_FAIL;
        goto done;
    }
    ret = WM_SUCCESS;
done:
    if (beacon_ies_data)
    {
        OSA_MemoryFree(beacon_ies_data);
    }
    if (beacon_wps_ies_data)
    {
        OSA_MemoryFree(beacon_wps_ies_data);
    }
    if (proberesp_ies_data)
    {
        OSA_MemoryFree(proberesp_ies_data);
    }
    if (assocresp_ies_data)
    {
        OSA_MemoryFree(assocresp_ies_data);
    }

    return ret;
}

#if CONFIG_5GHz_SUPPORT
/**
 * @brief Get second channel offset
 *
 * @param priv         A pointer to moal_private structure
 * @param chan             channel num
 * @return                second channel offset
 */
t_u8 wifi_get_second_channel_offset(mlan_private *priv, int chan)
{
    t_u8 chan2Offset = SEC_CHAN_NONE;

#if CONFIG_UNII4_BAND_SUPPORT
    mlan_adapter *pmadapter = priv->adapter;
    if (pmadapter->region_code != COUNTRY_CODE_US && chan == 165)
#else
    if (chan == 165)
#endif
        return chan2Offset;

    switch (chan)
    {
        case 36:
        case 44:
        case 52:
        case 60:
        case 100:
        case 108:
        case 116:
        case 124:
        case 132:
#if CONFIG_11AC
        case 140:
#endif
        case 149:
        case 157:
#if CONFIG_UNII4_BAND_SUPPORT
        case 165:
        case 173:
#endif
            chan2Offset = SEC_CHAN_ABOVE;
            break;
        case 40:
        case 48:
        case 56:
        case 64:
        case 104:
        case 112:
        case 120:
        case 128:
        case 136:
#if CONFIG_11AC
        case 144:
#endif
        case 153:
        case 161:
#if CONFIG_UNII4_BAND_SUPPORT
        case 169:
        case 177:
#endif
            chan2Offset = SEC_CHAN_BELOW;
            break;
    }
    return chan2Offset;
}
#endif

/**
 * @brief Look up specific IE in a buf
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 * @param id              Element id to lookup
 *
 * @return                Pointer of the specific IE -- success, NULL -- fail
 */
const t_u8 *wifi_parse_ie_tlv(const t_u8 *ie, int len, t_u8 id)
{
    int left_len    = len;
    const t_u8 *pos = ie;
    int length;

    /* IE format:
     * |   u8  |   id   |
     * |   u8  |   len  |
     * |   var |   data |
     */
    while (left_len >= 2)
    {
        length = *(pos + 1);
        if ((*pos == id) && (length + 2) <= left_len)
            return pos;
        pos += (length + 2);
        left_len -= (length + 2);
    }

    return NULL;
}

/**
 * @brief Look up specific IE in Extension IE
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 * @param ext_id         Extended Element id to lookup
 *
 * @return                Pointer of the specific Extended IE -- success, NULL
 * -- fail
 */
const t_u8 *wifi_parse_ext_ie_tlv(const t_u8 *ie, int len, t_u8 ext_id)
{
    int left_len    = len;
    const t_u8 *pos = ie;
    int length;

    /* Extension IE format:
     * |   u8  |   id   |
     * |   u8  |   len  |
     * |   u8  |   ext_id |
     * |   var |   data |
     */
    while (left_len >= 2)
    {
        length = *(pos + 1);
        if ((*pos == EXTENSION) && (length + 2) <= left_len)
        {
            if (*(pos + 2) == ext_id)
                return pos;
        }
        pos += (length + 2);
        left_len -= (length + 2);
    }
    return NULL;
}

/**
 * @brief get ht_cap from beacon ie
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 *
 * @return                ht_cap
 */
static t_u16 wifi_get_htcap_info(const t_u8 *ie, int len)
{
    t_u16 ht_cap_info           = 0;
    IEEEtypes_HTCap_t *htcap_ie = NULL;
    htcap_ie                    = (IEEEtypes_HTCap_t *)wifi_parse_ie_tlv(ie, len, HT_CAPABILITY);
    if (htcap_ie)
    {
        /* hostap has converted ht_cap_info to little endian, here
         * conver to host endian */
        ht_cap_info = wlan_le16_to_cpu(htcap_ie->ht_cap.ht_cap_info);
        wifi_d("Get ht_cap from beacon ies: 0x%x\r\n", ht_cap_info);
    }
    return ht_cap_info;
}

#if CONFIG_11AC
/**
 * @brief get vht_cap from beacon ie
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 *
 * @return                Pointer to vht_cap ie
 */
static IEEEtypes_VHTCap_t *wifi_get_vhtcap_info(const t_u8 *ie, int len)
{
    IEEEtypes_VHTCap_t *vhtcap_ie = NULL;
    vhtcap_ie                     = (IEEEtypes_VHTCap_t *)wifi_parse_ie_tlv(ie, len, VHT_CAPABILITY);
    if (vhtcap_ie)
        wifi_d("Get vht_cap from beacon ies: 0x%x\r\n", vhtcap_ie->vht_cap.vht_cap_info);
    return vhtcap_ie;
}

/**
 * @brief get vht_oper from beacon ie
 *
 * @param ie              Pointer to IEs
 * @param len             Total length of ie
 *
 * @return                Pointer to vht_opr ie
 */
static IEEEtypes_VHTOprat_t *wifi_get_vht_oprat_ie(const t_u8 *ie, int len)
{
    IEEEtypes_VHTOprat_t *vht_oprat_ie = NULL;
    vht_oprat_ie                       = (IEEEtypes_VHTOprat_t *)wifi_parse_ie_tlv(ie, len, VHT_OPERATION);
    if (vht_oprat_ie)
        wifi_d("Get vht_oprat_ie from beacon ies: chan_width=%d\r\n", vht_oprat_ie->chan_width);
    return vht_oprat_ie;
}

/**
 *  @brief enable/disable 11AC
 *
 *  @param priv     A pointer to moal_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param vht20_40 Enable VHT 20 MHz or 40 MHz band
 *  @param vhtcap_ie A pointer to vht capability IE
 *
 *  @return         0--success, otherwise failure
 */
int wifi_uap_set_11ac_status2(mlan_private *priv, t_u8 action, t_u8 vht20_40, IEEEtypes_VHTCap_t *vhtcap_ie)
{
    mlan_adapter *pmadapter = priv->adapter;
    mlan_ds_11ac_vht_cfg vht_cfg;
    int ret = 0;

    ENTER();

    (void)memset(&vht_cfg, 0, sizeof(vht_cfg));

#if CONFIG_5GHz_SUPPORT
    if (priv->uap_channel > MAX_CHANNELS_BG)
    {
        vht_cfg.band = BAND_SELECT_A;
    }
    else
    {
        vht_cfg.band = BAND_SELECT_BG;
    }
#else
    vht_cfg.band = BAND_SELECT_BG;
#endif
    vht_cfg.txrx = MLAN_RADIO_TXRX;

    /*
     * p2p GO (negotiation or auto GO) cases, wpa_supplicant will download
     * invalid vht capability with value 0 in beacon parameters, so for p2p
     * GO case (vht_cap_info = 0), driver will use hardware 11ac vht
     * capability value instead of up layer value.
     */
    if (vhtcap_ie && vhtcap_ie->vht_cap.vht_cap_info != 0)
    {
        vht_cfg.vht_cap_info = wlan_le32_to_cpu(vhtcap_ie->vht_cap.vht_cap_info);
        /** todo mcs configuration */
    }
    else
    {
#if CONFIG_5GHz_SUPPORT
        vht_cfg.vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_a;
#else
        vht_cfg.vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_bg;
#endif
    }
    if (action == MLAN_ACT_DISABLE)
    {
        vht_cfg.bwcfg = MFALSE;
        vht_cfg.vht_cap_info &= ~VHT_CAP_11AC_MASK;
        vht_cfg.vht_rx_mcs = vht_cfg.vht_tx_mcs = 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg           = MTRUE;
    }
    else
    {
        if (vht20_40)
            vht_cfg.bwcfg = MFALSE;
        else
            vht_cfg.bwcfg = MTRUE;
        vht_cfg.vht_cap_info &= ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
        vht_cfg.vht_tx_mcs            = pmadapter->usr_dot_11ac_mcs_support >> 16;
        vht_cfg.vht_rx_mcs            = pmadapter->usr_dot_11ac_mcs_support & 0xffff;
        vht_cfg.skip_usr_11ac_mcs_cfg = MTRUE;
    }

    if ((GET_VHTCAP_MAXMPDULEN(vht_cfg.vht_cap_info)) != 0U)
    {
        RESET_11ACMAXMPDULEN(vht_cfg.vht_cap_info);
    }
    else
    {
        /** Do Nothing */
    }

    wifi_d("Uap:11ac=%d vht_cap_info=0x%x, vht_tx_mcs=0x%x, vht_rx_mcs=0x%x\r\n", action, vht_cfg.vht_cap_info,
           vht_cfg.vht_tx_mcs, vht_cfg.vht_rx_mcs);

    ret = (int)wlan_11ac_ioctl_vhtcfg(priv, (t_u8)MLAN_ACT_SET, &vht_cfg);
    return ret;
}
#endif

#if CONFIG_11AX

/**
 *  @brief enable/disable 11AX
 *
 *  @param pmpriv   A pointer to mlan_private structure
 *  @param action   MLAN_ACT_DISABLE or MLAN_ACT_ENABLE
 *  @param band     band config
 *
 *  @return         0--success, otherwise failure
 */
int wifi_uap_set_11ax_status2(mlan_private *pmpriv, t_u8 action, t_u8 band, IEEEtypes_HECap_t *hecap_ie, t_u8 bandwidth)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
    mlan_ds_11ax_he_cfg he_cfg;

    ENTER();
    if ((band == BAND_CONFIG_5GHZ && !(pmadapter->fw_bands & BAND_AAX)) ||
        ((band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL) && !(pmadapter->fw_bands & BAND_GAX)))
    {
        PRINTM(MERROR, "fw doesn't support 11ax\n");
        ret = -WM_FAIL;
        goto done;
    }
    memset(&he_cfg, 0, sizeof(he_cfg));
    if (band == BAND_CONFIG_5GHZ)
    {
        he_cfg.band = MBIT(1);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_he_cap, pmadapter->hw_hecap_len);
    }
    else if (band == BAND_CONFIG_ACS_MODE || band == BAND_CONFIG_MANUAL)
    {
        he_cfg.band = MBIT(0);
        (void)memcpy((void *)&he_cfg.he_cap, (const void *)pmadapter->hw_2g_he_cap, pmadapter->hw_2g_hecap_len);
        if (bandwidth == BANDWIDTH_20MHZ)
        {
            he_cfg.he_cap.he_phy_cap[0] &= ~(MBIT(1));
        }
    }
    else
    {
        PRINTM(MERROR, "Invalid band!\n");
        ret = -WM_E_INVAL;
        goto done;
    }
#ifdef RW610
    he_cfg.he_cap.he_phy_cap[0] &= ~DEFAULT_11AX_CAP_40MHZIH2_4GHZBAND_RESET_MASK;
#endif
#if CONFIG_11AX_TWT
    /* uap mode clear TWT request bit */
    he_cfg.he_cap.he_mac_cap[0] &= ~HE_MAC_CAP_TWT_REQ_SUPPORT;
#endif
#if 0
    if (wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_GET, &he_cfg))
    {
        PRINTM(MERROR, "Fail to get 11ax cfg!\n");
        ret = -WM_FAIL;
        goto done;
    }
#endif
    if (hecap_ie)
    {
        DBG_HEXDUMP(MCMD_D, "hecap_ie", (t_u8 *)hecap_ie, hecap_ie->ieee_hdr.len + sizeof(IEEEtypes_Header_t));
        he_cfg.he_cap.id  = hecap_ie->ieee_hdr.element_id;
        he_cfg.he_cap.len = hecap_ie->ieee_hdr.len;
        memcpy(&he_cfg.he_cap.ext_id, &hecap_ie->ext_id, he_cfg.he_cap.len);
        if ((band == BAND_2GHZ) && (bandwidth == BANDWIDTH_20MHZ))
        {
            he_cfg.he_cap.he_phy_cap[0] &= ~(MBIT(1));
        }
    }

    if (action == MLAN_ACT_DISABLE)
    {
        if (he_cfg.he_cap.len && (he_cfg.he_cap.ext_id == HE_CAPABILITY))
            memset(he_cfg.he_cap.he_txrx_mcs_support, 0xff, sizeof(he_cfg.he_cap.he_txrx_mcs_support));
        else
        {
            PRINTM(MCMND, "11ax already disabled\n");
            goto done;
        }
    }
    DBG_HEXDUMP(MCMD_D, "HE_CFG ", (t_u8 *)&he_cfg, sizeof(he_cfg));
    ret = wlan_cmd_11ax_cfg(pmpriv, HostCmd_ACT_GEN_SET, &he_cfg);
done:
    LEAVE();
    return ret;
}
#endif /* CONFIG_11AX */

#if CONFIG_5GHz_SUPPORT
static void wifi_set_uap_dfs_cac(mlan_private *priv, Band_Config_t *bandcfg, t_u8 ht_enabled)
{
    if (priv->uap_channel > MAX_CHANNELS_BG)
    {
        mlan_private *priv_sta = (mlan_private *)mlan_adap->priv[0];
        if ((priv_sta->media_connected == MTRUE) && wlan_11h_radar_detect_required(priv, priv->uap_channel))
        {
            nxp_wifi_dfs_cac_info cacinfo;
            t_u8 center_chan = 0;

            memset(&cacinfo, 0, sizeof(nxp_wifi_dfs_cac_info));
            cacinfo.center_freq  = channel_to_frequency(priv->uap_channel, bandcfg->chanBand);
            cacinfo.ht_enabled   = ht_enabled;
            cacinfo.ch_offset    = bandcfg->chan2Offset;
            cacinfo.center_freq2 = 0;

            switch (bandcfg->chanWidth)
            {
                case CHAN_BW_20MHZ:
                    if (ht_enabled)
                        cacinfo.ch_width = CHAN_BAND_WIDTH_20;
                    else
                        cacinfo.ch_width = CHAN_BAND_WIDTH_20_NOHT;
                    cacinfo.center_freq1 = cacinfo.center_freq;
                    break;

                case CHAN_BW_40MHZ:
                    cacinfo.ch_width = CHAN_BAND_WIDTH_40;
                    if (bandcfg->chan2Offset == SEC_CHAN_ABOVE)
                        cacinfo.center_freq1 = cacinfo.center_freq + 10;
                    else if (bandcfg->chan2Offset == SEC_CHAN_BELOW)
                        cacinfo.center_freq1 = cacinfo.center_freq - 10;
                    break;

#if (CONFIG_11AC)
                case CHAN_BW_80MHZ:
                    cacinfo.ch_width = CHAN_BAND_WIDTH_80;
                    center_chan      = wlan_get_center_freq_idx(priv, BAND_AAC, priv->uap_channel, CHANNEL_BW_80MHZ);
                    cacinfo.center_freq1 = channel_to_frequency(center_chan, bandcfg->chanBand);
                    break;
#endif

                default:
                    break;
            }

            /* STA has connected to EXT-AP on DFS channel, then uAP should support start network
             * on DFS channel. If DFS is offloaded to the driver, supplicant won't setup uAP until
             * the CAC is done by driver. When DFS master mode is not supported, driver needs to
             * send the EVENT_DFS_CAC_STARTED event to supplicant to set the cac_started flag, and
             * send EVENT_DFS_CAC_FINISHED event to supplicant to continue AP setup for DFS channel */
            if (wm_wifi.supp_if_callbk_fns->dfs_cac_started_callbk_fn)
            {
                wm_wifi.supp_if_callbk_fns->dfs_cac_started_callbk_fn(wm_wifi.hapd_if_priv, &cacinfo);
            }
            if (wm_wifi.supp_if_callbk_fns->dfs_cac_finished_callbk_fn)
            {
                wm_wifi.supp_if_callbk_fns->dfs_cac_finished_callbk_fn(wm_wifi.hapd_if_priv, &cacinfo);
            }
        }
    }
}
#endif

int wifi_nxp_beacon_config(nxp_wifi_ap_info_t *params)
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[1];
    mlan_private *remain_priv = NULL;
    // mlan_adapter *pmadapter = priv->adapter;

    const t_u8 *ie                 = NULL;
    int ret                        = 0, ie_len;
    mlan_uap_bss_param *sys_config = NULL;
    // int i                          = 0;
    t_u8 rates_b[5]   = {0x82, 0x84, 0x8b, 0x96, 0x00};
    t_u8 rates_bg[13] = {0x82, 0x84, 0x8b, 0x96, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48, 0x60, 0x6c, 0x00};
#if CONFIG_5GHz_SUPPORT
    t_u8 rates_a[9] = {0x8c, 0x12, 0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0x00};
#endif
    t_u8 supported_mcs_set[] = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    t_u8 chan2Offset         = SEC_CHAN_NONE;
    t_u8 enable_11n          = MTRUE;
    t_u8 bandwidth           = BANDWIDTH_40MHZ;
    t_u16 ht_cap             = 0;
#if CONFIG_11AC
    t_u8 enable_11ac                = MFALSE;
    t_u8 vht20_40                   = MFALSE;
    IEEEtypes_VHTCap_t *vhtcap_ie   = NULL;
    IEEEtypes_VHTOprat_t *vhtopr_ie = NULL;
#endif
#if CONFIG_11AX
    IEEEtypes_HECap_t *hecap_ie = NULL;
    t_u8 enable_11ax            = MFALSE;
#endif
    Band_Config_t bandcfg;
    wifi_scan_chan_list_t scan_chan_list;

    ENTER();

    (void)bandwidth;

    if (!params)
    {
        return -WM_FAIL;
    }

    memset(&scan_chan_list, 0, sizeof(wifi_scan_chan_list_t));

    ie     = (const t_u8 *)params->tail_ie.ie;
    ie_len = params->tail_ie.ie_len;
    remain_priv = priv->adapter->priv[priv->adapter->remain_bss_index];
    if (wifi_is_remain_on_channel() && remain_priv)
    {
        wuap_d("Cancel Remain on Channel before Starting AP");
        wifi_remain_on_channel(false, 0, 0);
    }

    if (params->beacon_set)
    {
        ret = wifi_nxp_set_mgmt_ies(priv, params->tail_ie.ie, params->tail_ie.ie_len, params->beacon_ies.ie,
                                    params->beacon_ies.ie_len, params->proberesp_ies.ie, params->proberesp_ies.ie_len,
                                    params->assocresp_ies.ie, params->assocresp_ies.ie_len);
        if (ret != WM_SUCCESS)
        {
            wuap_e("Set uAP mgmt ie failed");
            return -WM_FAIL;
        }
    }
    else
    {
        sys_config = OSA_MemoryAllocate(sizeof(mlan_uap_bss_param));
        if (!sys_config)
        {
            wuap_e("Fail to alloc memory for mlan_uap_bss_param");
            ret = -WM_FAIL;
            goto done;
        }

        memset(&bandcfg, 0, sizeof(Band_Config_t));

        if (priv->uap_max_sta)
            sys_config->max_sta_count = priv->uap_max_sta;

        /* Setting the default values */
        sys_config->channel = params->chan.channel;
        priv->uap_channel   = sys_config->channel;
        if (priv->uap_channel != 0U)
        {
#if CONFIG_5GHz_SUPPORT
            if (priv->uap_channel > MAX_CHANNELS_BG)
            {
                mlan_private *priv_sta = (mlan_private *)mlan_adap->priv[0];
                if ((priv_sta->media_connected == MFALSE) && wlan_11h_radar_detect_required(priv, priv->uap_channel))
                {
                    wuap_e("Cannot start uAP on DFS channel %d", priv->uap_channel);
                    ret = -WM_FAIL;
                    goto done;
                }

                bandcfg.chanBand = BAND_5GHZ;
            }
            else
            {
                bandcfg.chanBand = BAND_2GHZ;
            }
#else
            bandcfg.chanBand = BAND_2GHZ;
#endif
        }

        sys_config->preamble_type         = 0;
        sys_config->mgmt_ie_passthru_mask = priv->mgmt_frame_passthru_mask;
        memcpy(sys_config->mac_addr, params->bssid, ETH_ALEN);

        /* Set frag_threshold, rts_threshold, and retry limit */
        sys_config->frag_threshold = MLAN_FRAG_MAX_VALUE;
        sys_config->rts_threshold  = MLAN_RTS_MAX_VALUE;

        sys_config->pwe_derivation = params->sae_pwe;

        if (params->beacon_int)
            sys_config->beacon_period = params->beacon_int;

        if (params->dtim_period)
            sys_config->dtim_period = params->dtim_period;

        if (sys_config->channel <= MAX_CHANNELS_BG)
        {
            if (sys_config->channel == 14)
            {
                memcpy(sys_config->rates, rates_b, sizeof(rates_b));
            }
            else
            {
                memcpy(sys_config->rates, rates_bg, sizeof(rates_bg));
            }
        }
#if CONFIG_5GHz_SUPPORT
        else
        {
            memcpy(sys_config->rates, rates_a, sizeof(rates_a));
        }
#endif

        if (params->chan.ht_enabled != 1)
        {
            enable_11n = MFALSE;
        }

        if (params->chan.sec_channel_offset == 1)
        {
            chan2Offset = SEC_CHAN_ABOVE;
        }
        else if (params->chan.sec_channel_offset == -1)
        {
            chan2Offset = SEC_CHAN_BELOW;
        }

        if (params->chan.bandwidth == 20)
        {
            bandwidth         = BANDWIDTH_20MHZ;
            bandcfg.chanWidth = CHAN_BW_20MHZ;
        }
        else if (params->chan.bandwidth == 40)
        {
            bandwidth         = BANDWIDTH_40MHZ;
            bandcfg.chanWidth = CHAN_BW_40MHZ;
        }
#if CONFIG_11AC
#if CONFIG_5GHz_SUPPORT
        else if (params->chan.bandwidth == 80)
        {
            bandwidth         = BANDWIDTH_80MHZ;
            bandcfg.chanWidth = CHAN_BW_80MHZ;
            chan2Offset       = wifi_get_second_channel_offset(priv, priv->uap_channel);
        }
#endif
#endif

        bandcfg.chan2Offset = chan2Offset;
        (void)memcpy((void *)&sys_config->band_cfg, (const void *)&bandcfg, sizeof(bandcfg));

#if CONFIG_11AC
        if (params->chan.vht_enabled == 1)
        {
            enable_11ac = wifi_check_11ac_capability(priv, bandcfg.chanBand);
            if (enable_11ac && ((bandwidth == BANDWIDTH_20MHZ) || (bandwidth == BANDWIDTH_40MHZ)))
            {
                vht20_40 = MTRUE;
            }
        }
#endif
#if CONFIG_11AX
        if (params->chan.he_enabled == 1)
        {
            enable_11ax = wifi_check_11ax_capability(priv, bandcfg.chanBand);
        }
#endif

        /* Disable GreenField by default */
        sys_config->ht_cap_info = 0x10c;
        if (enable_11n)
            sys_config->ht_cap_info |= 0x20;
        if (chan2Offset)
        {
            sys_config->ht_cap_info |= 0x1042;
            sys_config->ampdu_param = 3;
        }

        if (enable_11n)
        {
            ht_cap = wifi_get_htcap_info(ie, ie_len);
            if (ht_cap)
            {
                if (bandcfg.chanBand == BAND_2GHZ)
                    sys_config->ht_cap_info = (ht_cap & 0x13ff) | 0x0c;
                else
                    sys_config->ht_cap_info = (ht_cap & 0x13ff) | 0x0c;
            }
            wifi_d(
                "11n=%d, ht_cap=0x%x, channel=%d, bandcfg:chanBand=0x%x chanWidth=0x%x chan2Offset=0x%x "
                "scanMode=0x%x\n",
                enable_11n, sys_config->ht_cap_info, priv->uap_channel, bandcfg.chanBand, bandcfg.chanWidth,
                bandcfg.chan2Offset, bandcfg.scanMode);

            ret = wifi_uap_set_httxcfg_int(ht_cap);
            if (ret != WM_SUCCESS)
            {
                wuap_e("Cannot set uAP HT TX Cfg:%x", sys_config->ht_cap_info);
                ret = -WM_FAIL;
                goto done;
            }

            sys_config->ampdu_param = 3;

            /* Set MCS32 with 40MHz support */
            if ((bandcfg.chanWidth == CHAN_BW_40MHZ) || (bandcfg.chanWidth == CHAN_BW_80MHZ))
                SETHT_MCS32(supported_mcs_set);

            (void)memcpy((void *)sys_config->supported_mcs_set, (const void *)supported_mcs_set,
                         sizeof(sys_config->supported_mcs_set));
        }

        if (!params->ssid.ssid_len)
        {
            ret = -WM_FAIL;
            goto done;
        }
        memcpy(sys_config->ssid.ssid, params->ssid.ssid, MIN(MLAN_MAX_SSID_LENGTH, params->ssid.ssid_len));
        sys_config->ssid.ssid_len = MIN(MLAN_MAX_SSID_LENGTH, params->ssid.ssid_len);
        /**
         * hidden_ssid=0: broadcast SSID in beacons.
         * hidden_ssid=1: send empty SSID (length=0) in beacon.
         * hidden_ssid=2: clear SSID (ACSII 0), but keep the original length
         */
        if (!params->hide_ssid)
            sys_config->bcast_ssid_ctl = 1;
        else if (params->hide_ssid == 1)
            sys_config->bcast_ssid_ctl = 0;
        else if (params->hide_ssid == 2)
            sys_config->bcast_ssid_ctl = 2;

        sys_config->auth_mode = MLAN_AUTH_MODE_OPEN;

        sys_config->protocol = PROTOCOL_NO_SECURITY;
        if ((params->wpa_version & WIFI_NXP_WPA_VERSION_1) && (params->wpa_version & WIFI_NXP_WPA_VERSION_2))
            sys_config->protocol = PROTOCOL_WPA | PROTOCOL_WPA2;
        else if (params->wpa_version & WIFI_NXP_WPA_VERSION_2)
            sys_config->protocol = PROTOCOL_WPA2;
        else if (params->wpa_version & WIFI_NXP_WPA_VERSION_1)
            sys_config->protocol = PROTOCOL_WPA;

        if (params->key_mgmt_suites || (params->privacy && params->wpa_version))
            wifi_find_wpa_ies(ie, ie_len, sys_config);

        /*find and set wmm ie*/
        wifi_set_wmm_ies(priv, ie, ie_len, sys_config);

#if CONFIG_11AC
        if (enable_11ac && enable_11n)
        {
            vhtcap_ie = wifi_get_vhtcap_info(ie, ie_len);
            vhtopr_ie = wifi_get_vht_oprat_ie(ie, ie_len);
            // Enable VHT80
            if (vhtopr_ie && vhtopr_ie->chan_width)
                vht20_40 = 0;
            wifi_uap_set_11ac_status2(priv, MLAN_ACT_ENABLE, vht20_40, vhtcap_ie);
        }
        else
        {
            wifi_uap_set_11ac_status2(priv, MLAN_ACT_DISABLE, vht20_40, NULL);
        }
#endif
#if CONFIG_11AX
        if (enable_11ax && enable_11n)
        {
            hecap_ie = (IEEEtypes_HECap_t *)wifi_parse_ext_ie_tlv(ie, ie_len, HE_CAPABILITY);
            wifi_uap_set_11ax_status2(priv, MLAN_ACT_ENABLE, bandcfg.chanBand, hecap_ie, bandwidth);
        }
        else
            wifi_uap_set_11ax_status2(priv, MLAN_ACT_DISABLE, bandcfg.chanBand, NULL, bandwidth);
#endif

        if (params->ap_max_inactivity)
        {
            sys_config->sta_ageout_timer    = params->ap_max_inactivity * 10;
            sys_config->ps_sta_ageout_timer = params->ap_max_inactivity * 10;
        }
        PRINTM(MIOCTL, "inactivity_timeout=%d\n", params->ap_max_inactivity);
        PRINTM(MIOCTL, "sta_ageout_timer=%d ps_sta_ageout_timer=%d\n", sys_config->sta_ageout_timer,
               sys_config->ps_sta_ageout_timer);

        if (MLAN_STATUS_SUCCESS != wifi_set_get_sys_config(priv, MLAN_ACT_SET, sys_config))
        {
            wuap_e("Set uAP sys config failed");
            ret = -WM_FAIL;
            goto done;
        }

        ret = wifi_nxp_set_mgmt_ies(priv, params->tail_ie.ie, params->tail_ie.ie_len, params->beacon_ies.ie,
                                    params->beacon_ies.ie_len, params->proberesp_ies.ie, params->proberesp_ies.ie_len,
                                    params->assocresp_ies.ie, params->assocresp_ies.ie_len);
        if (ret != WM_SUCCESS)
        {
            wuap_e("Set uAP mgmt ie failed");
            ret = -WM_FAIL;
            goto done;
        }

        if (wm_wifi.enable_11d_support && wm_wifi.uap_support_11d_apis)
        {
            wuap_d("Downloading domain params");
            wm_wifi.uap_support_11d_apis->wifi_uap_downld_domain_params_p(BAND_B);
#if CONFIG_5GHz_SUPPORT
            wm_wifi.uap_support_11d_apis->wifi_uap_downld_domain_params_p(BAND_A);
#endif
        }

#if CONFIG_5GHz_SUPPORT
        wifi_set_uap_dfs_cac(priv, &bandcfg, enable_11n);
#endif
        priv->uap_host_based = MTRUE;

        if (!params->beacon_set)
        {
            wuap_d("Starting BSS");
            /* Start BSS */
            if (MLAN_STATUS_SUCCESS != wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_BSS_START,
                                                                     HostCmd_ACT_GEN_SET, 0, NULL, NULL,
                                                                     MLAN_BSS_TYPE_UAP, NULL))
            {
                wuap_e("Start BSS failed");
                priv->uap_host_based = MFALSE;
                ret                  = -WM_FAIL;
                goto done;
            }
            wuap_d("wlan: AP started");

            (void)wifi_set_rx_mgmt_indication(MLAN_BSS_TYPE_UAP, WIFI_MGMT_AUTH | MGMT_MASK_ASSOC_REQ |
                                                                     MGMT_MASK_REASSOC_REQ | WIFI_MGMT_DEAUTH |
                                                                     WIFI_MGMT_ACTION | WIFI_MGMT_DIASSOC);
        }

    done:
        if (sys_config != NULL)
        {
            OSA_MemoryFree(sys_config);
        }
    }

    LEAVE();
    return ret;
}

int wifi_setup_ht_cap(t_u16 *ht_capab, t_u8 *pmcs_set, t_u8 *a_mpdu_params, t_u8 band)
{
    mlan_private *priv      = (mlan_private *)mlan_adap->priv[1];
    mlan_adapter *pmadapter = priv->adapter;
    t_u32 usr_dot_11n_dev_cap;
    int rx_mcs_supp;
    t_u8 mcs_set[NUM_MCS_FIELD];

    ENTER();

    *ht_capab = HT_CAP_INFO_DSSS_CCK40MHZ;

    *a_mpdu_params = 3;

#if CONFIG_5GHz_SUPPORT
    if (band == BAND_5GHZ)
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_a;
    }
    else
#endif
    {
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_bg;
    }

    if (ISSUPP_RXLDPC(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_LDPC_CODING_CAP;
    }
    if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_SUPP_CHANNEL_WIDTH_SET;
    }
    else
    {
        *ht_capab &= ~(HT_CAP_INFO_DSSS_CCK40MHZ);
    }
    if ((usr_dot_11n_dev_cap >> 20) & 0x03) /* Delayed ACK supported */
    {
        *ht_capab |= HT_CAP_INFO_DELAYED_BA;
    }
    if (ISSUPP_GREENFIELD(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_GREEN_FIELD;
    }
    if (ISSUPP_SHORTGI20(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_SHORT_GI20MHZ;
    }
    if (ISSUPP_SHORTGI40(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_SHORT_GI40MHZ;
    }
    if (ISSUPP_TXSTBC(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_TX_STBC;
    }
    if (ISSUPP_RXSTBC(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_RX_STBC_1;
    }
    if (ISSUPP_MIMOPS(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_SMPS_STATIC;
    }
    else
    {
        *ht_capab |= HT_CAP_INFO_SMPS_DISABLED;
    }
    if (ISSUPP_MAXAMSDU(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_MAX_AMSDU_SIZE;
    }
    if (ISENABLED_40MHZ_INTOLARENT(usr_dot_11n_dev_cap))
    {
        *ht_capab |= HT_CAP_INFO_40MHZ_INTOLERANT;
    }

    rx_mcs_supp = GET_RXMCSSUPP(pmadapter->usr_dev_mcs_support);
    /* Set MCS for 1x1/2x2 */
    (void)__memset(pmadapter, (t_u8 *)mcs_set, 0xff, rx_mcs_supp);
    /* Clear all the other values */
    (void)__memset(pmadapter, (t_u8 *)&mcs_set[rx_mcs_supp], 0, NUM_MCS_FIELD - rx_mcs_supp);
    /* Set MCS32 with 40MHz support */
    if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap))
        SETHT_MCS32(mcs_set);

    (void)__memcpy(pmadapter, pmcs_set, mcs_set, NUM_MCS_FIELD);

    LEAVE();

    return WM_SUCCESS;
}

t_u16 wifi_get_default_ht_capab()
{
    t_u16 ht_capab;
    t_u8 mcs_set[16];
    t_u8 a_mpdu_params;

    wifi_setup_ht_cap(&ht_capab, &mcs_set[0], &a_mpdu_params, 0);

    return ht_capab;
}

static void wifi_setup_channel_flag(void *channels, int num_chan, region_chan_t *region)
{
    int i;
    int set_idx                             = 0;
    int get_idx                             = 0;
    const chan_freq_power_t *pchans_get     = region->pcfp;
    struct hostapd_channel_data *pchans_set = (struct hostapd_channel_data *)channels;

    for (i = 0; i < MAX(num_chan, region->num_cfp); i++)
    {
        if (set_idx >= num_chan)
            break;

        if (get_idx >= region->num_cfp || pchans_set[set_idx].chan < pchans_get[get_idx].channel)
        {
            pchans_set[set_idx].flag |= HOSTAPD_CHAN_DISABLED;

            set_idx++;
        }
        else if (pchans_set[set_idx].chan == pchans_get[get_idx].channel)
        {
            pchans_set[set_idx].max_tx_power = pchans_get[get_idx].max_tx_power;
            /* set passive scan or radar detect flag */
            if (pchans_get[get_idx].passive_scan_or_radar_detect == MTRUE)
                pchans_set[set_idx].flag |= HOSTAPD_CHAN_RADAR;

            set_idx++;
            get_idx++;
        }
        else
        {
            get_idx++;
        }
    }
}

void wifi_setup_channel_info(void *channels, int num_channels, t_u8 band)
{
    mlan_adapter *pmadapter = (mlan_adapter *)mlan_adap;
    region_chan_t *region   = NULL;

    if (band == BAND_2GHZ)
    {
        if (pmadapter->region_channel[0].valid)
            region = &pmadapter->region_channel[0];
        else if (pmadapter->universal_channel[0].valid)
            region = &pmadapter->universal_channel[0];
        else
            return;

        wifi_setup_channel_flag(channels, num_channels, region);
    }
    else if (band == BAND_5GHZ)
    {
        if (pmadapter->region_channel[1].valid)
            region = &pmadapter->region_channel[1];
        else if (pmadapter->universal_channel[1].valid)
            region = &pmadapter->universal_channel[1];
        else
            return;

        wifi_setup_channel_flag(channels, num_channels, region);
    }
    else
    {
        wuap_e("wifi_setup_channel_info unsupported band %d", band);
    }
}

#if CONFIG_11AC
int wifi_setup_vht_cap(t_u32 *vht_capab, t_u8 *vht_mcs_set, t_u8 band)
{
    mlan_private *priv      = (mlan_private *)mlan_adap->priv[1];
    mlan_adapter *pmadapter = priv->adapter;
    // t_u32 usr_dot_11ac_dev_cap;
    t_u16 vht_tx_mcs, vht_rx_mcs;

    ENTER();

    *vht_capab = pmadapter->usr_dot_11ac_dev_cap_a;

    *vht_capab &= ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#ifdef RW610
    *vht_capab &= ~DEFALUT_11AC_CAP_SHORTGI_80MHZ_RESET_MASK;
#endif

    if ((GET_VHTCAP_MAXMPDULEN(*vht_capab)) != 0U)
    {
        RESET_11ACMAXMPDULEN(*vht_capab);
    }
    else
    {
        /** Do Nothing */
    }

    vht_tx_mcs = pmadapter->usr_dot_11ac_mcs_support >> 16;
    vht_rx_mcs = pmadapter->usr_dot_11ac_mcs_support & 0xffff;

    (void)__memcpy(pmadapter, &vht_mcs_set[0], &vht_rx_mcs, sizeof(t_u16));

    (void)__memcpy(pmadapter, &vht_mcs_set[4], &vht_tx_mcs, sizeof(t_u16));

    LEAVE();

    return WM_SUCCESS;
}

t_u32 wifi_get_default_vht_capab()
{
    t_u32 vht_capab;
    t_u8 vht_mcs_set[8];

    wifi_setup_vht_cap(&vht_capab, &vht_mcs_set[0], 0);

    return vht_capab;
}
#endif

#if CONFIG_11AX
/*
===============
11AX CAP for uAP
===============
Note: bits not mentioned below are set to 0.

5G
===
HE MAC Cap:
Bit0:  1  (+HTC HE Support)
Bit25: 1  (OM Control Support. But uAP does not support
           Tx OM received from the STA, as it does not support UL OFDMA)

HE PHY Cap:
Bit1-7: 0x2 (Supported Channel Width Set.
             Note it would be changed after 80+80 MHz is supported)
Bit8-11: 0x3 (Punctured Preamble Rx.
              Note: it would be changed after 80+80 MHz is supported)
Bit12: 0x0 (Device Class)
Bit13: 0x1 (LDPC coding in Payload)
Bit17: 0x1 (NDP with 4xHE-LTF+3.2usGI)
Bit18: 0x1 (STBC Tx <= 80 MHz)
Bit19: 0x1 (STBC Rx <= 80 MHz)
Bit20: 0x1 (Doppler Tx)
Bit21: 0x1 (Doppler Rx)
Bit24-25: 0x1 (DCM Max Constellation Tx)
Bit27-28: 0x1 (DCM Max Constellation Rx)
Bit31: 0x1 (SU Beamformer)
Bit32: 0x1 (SU BeamFormee)
Bit34-36: 0x7 (Beamformee STS <= 80 MHz)
Bit40-42: 0x1 (Number of Sounding Dimentions <= 80 MHz)
Bit53: 0x1 (Partial Bandwidth Extended Range)
Bit55: 0x1 (PPE Threshold Present.
            Note: PPE threshold may have some changes later)
Bit58: 0x1 (HE SU PPDU and HE MU PPDU with 4xHE-LTF+0.8usGI)
Bit59-61: 0x1 (Max Nc)
Bit64: 0x1 (HE ER SU PPDU with 4xHE-LTF+0.8usGI)
Bit75: 0x1 (Rx 1024-QAM Support < 242-tone RU)
*/

#define UAP_HE_MAC_CAP0_MASK  0x04
#define UAP_HE_MAC_CAP1_MASK  0x00
#define UAP_HE_MAC_CAP2_MASK  0x10
#define UAP_HE_MAC_CAP3_MASK  0x02
#define UAP_HE_MAC_CAP4_MASK  0x00
#define UAP_HE_MAC_CAP5_MASK  0x00
#define UAP_HE_PHY_CAP0_MASK  0x04
#define UAP_HE_PHY_CAP1_MASK  0x23
#define UAP_HE_PHY_CAP2_MASK  0x3E
#define UAP_HE_PHY_CAP3_MASK  0x89
#define UAP_HE_PHY_CAP4_MASK  0x1D
#define UAP_HE_PHY_CAP5_MASK  0x01
#define UAP_HE_PHY_CAP6_MASK  0xA0
#define UAP_HE_PHY_CAP7_MASK  0x0C
#define UAP_HE_PHY_CAP8_MASK  0x01
#define UAP_HE_PHY_CAP9_MASK  0x08
#define UAP_HE_PHY_CAP10_MASK 0x00

/*
2G
===
HE MAC Cap:
Bit0:   1  (+HTC HE Support)
Bit25: 1  (OM Control Support. Note: uAP does not support
        Tx OM received from the STA, as it does not support UL OFDMA)

HE PHY Cap:
Bit1-7: 0x1 (Supported Channel Width Set)
Bit8-11: 0x0 (Punctured Preamble Rx)
Bit12: 0x0 (Device Class)
Bit13: 0x1 (LDPC coding in Payload)
Bit17: 0x1 (NDP with 4xLTF+3.2usGI)
Bit18: 0x1 (STBC Tx <= 80 MHz)
Bit19: 0x1 (STBC Rx <= 80 MHz)
Bit20: 0x1 (Doppler Tx)
Bit21: 0x1 (Doppler Rx)
Bit24-25: 0x1 (DCM Max Constellation Tx)
Bit27-28: 0x1 (DCM Max Constellation Rx)
Bit31: 0x1 (SU Beamformer)
Bit32: 0x1 (SU BeamFormee)
Bit34-36: 0x7 (Beamformee STS <= 80 MHz)
Bit40-42: 0x1 (Number of Sounding Dimentions <= 80 MHz)
Bit53: 0x1 (Partial Bandwidth Extended Range)
Bit55: 0x1 (PPE Threshold Present.
            Note: PPE threshold may have some changes later)
Bit58: 0x1 (HE SU PPDU and HE MU PPDU with 4xHE-LTF+0.8usGI)
Bit59-61: 0x1 (Max Nc)
Bit64: 0x1 (HE ER SU PPDU with 4xHE-LTF+0.8usGI)
Bit75: 0x1 (Rx 1024-QAM Support < 242-tone RU)
*/
#define UAP_HE_2G_MAC_CAP0_MASK  0x04
#define UAP_HE_2G_MAC_CAP1_MASK  0x00
#define UAP_HE_2G_MAC_CAP2_MASK  0x10
#define UAP_HE_2G_MAC_CAP3_MASK  0x02
#define UAP_HE_2G_MAC_CAP4_MASK  0x00
#define UAP_HE_2G_MAC_CAP5_MASK  0x00
#define UAP_HE_2G_PHY_CAP0_MASK  0x02
#define UAP_HE_2G_PHY_CAP1_MASK  0x20
#define UAP_HE_2G_PHY_CAP2_MASK  0x3E
#define UAP_HE_2G_PHY_CAP3_MASK  0x89
#define UAP_HE_2G_PHY_CAP4_MASK  0x1D
#define UAP_HE_2G_PHY_CAP5_MASK  0x01
#define UAP_HE_2G_PHY_CAP6_MASK  0xA0
#define UAP_HE_2G_PHY_CAP7_MASK  0x0C
#define UAP_HE_2G_PHY_CAP8_MASK  0x01
#define UAP_HE_2G_PHY_CAP9_MASK  0x08
#define UAP_HE_2G_PHY_CAP10_MASK 0x00

/**
 *  @brief update 11ax ie for AP mode *
 *  @param band     channel band
 *  @hecap_ie       a pointer to mlan_ds_11ax_he_capa
 *
 *  @return         0--success, otherwise failure
 */
static void wifi_uap_update_11ax_ie(t_u8 band, mlan_ds_11ax_he_capa *hecap_ie)
{
    if (band == BAND_5GHZ
#ifdef ENABLE_802_116E
        || band == BAND_6GHZ
#endif
    )
    {
        hecap_ie->he_mac_cap[0] &= UAP_HE_MAC_CAP0_MASK;
        hecap_ie->he_mac_cap[1] &= UAP_HE_MAC_CAP1_MASK;
        hecap_ie->he_mac_cap[2] &= UAP_HE_MAC_CAP2_MASK;
        hecap_ie->he_mac_cap[3] &= UAP_HE_MAC_CAP3_MASK;
        hecap_ie->he_mac_cap[4] &= UAP_HE_MAC_CAP4_MASK;
        hecap_ie->he_mac_cap[5] &= UAP_HE_MAC_CAP5_MASK;
        hecap_ie->he_phy_cap[0] &= UAP_HE_PHY_CAP0_MASK;
        hecap_ie->he_phy_cap[1] &= UAP_HE_PHY_CAP1_MASK;
        hecap_ie->he_phy_cap[2] &= UAP_HE_PHY_CAP2_MASK;
        hecap_ie->he_phy_cap[3] &= UAP_HE_PHY_CAP3_MASK;
        hecap_ie->he_phy_cap[4] &= UAP_HE_PHY_CAP4_MASK;
        hecap_ie->he_phy_cap[5] &= UAP_HE_PHY_CAP5_MASK;
        hecap_ie->he_phy_cap[6] &= UAP_HE_PHY_CAP6_MASK;
        hecap_ie->he_phy_cap[7] &= UAP_HE_PHY_CAP7_MASK;
        hecap_ie->he_phy_cap[8] &= UAP_HE_PHY_CAP8_MASK;
        hecap_ie->he_phy_cap[9] &= UAP_HE_PHY_CAP9_MASK;
        hecap_ie->he_phy_cap[10] &= UAP_HE_PHY_CAP10_MASK;
    }
    else
    {
        hecap_ie->he_mac_cap[0] &= UAP_HE_2G_MAC_CAP0_MASK;
        hecap_ie->he_mac_cap[1] &= UAP_HE_2G_MAC_CAP1_MASK;
        hecap_ie->he_mac_cap[2] &= UAP_HE_2G_MAC_CAP2_MASK;
        hecap_ie->he_mac_cap[3] &= UAP_HE_2G_MAC_CAP3_MASK;
        hecap_ie->he_mac_cap[4] &= UAP_HE_2G_MAC_CAP4_MASK;
        hecap_ie->he_mac_cap[5] &= UAP_HE_2G_MAC_CAP5_MASK;
        hecap_ie->he_phy_cap[0] &= UAP_HE_2G_PHY_CAP0_MASK;
        hecap_ie->he_phy_cap[1] &= UAP_HE_2G_PHY_CAP1_MASK;
        hecap_ie->he_phy_cap[2] &= UAP_HE_2G_PHY_CAP2_MASK;
        hecap_ie->he_phy_cap[3] &= UAP_HE_2G_PHY_CAP3_MASK;
        hecap_ie->he_phy_cap[4] &= UAP_HE_2G_PHY_CAP4_MASK;
        hecap_ie->he_phy_cap[5] &= UAP_HE_2G_PHY_CAP5_MASK;
        hecap_ie->he_phy_cap[6] &= UAP_HE_2G_PHY_CAP6_MASK;
        hecap_ie->he_phy_cap[7] &= UAP_HE_2G_PHY_CAP7_MASK;
        hecap_ie->he_phy_cap[8] &= UAP_HE_2G_PHY_CAP8_MASK;
        hecap_ie->he_phy_cap[9] &= UAP_HE_2G_PHY_CAP9_MASK;
        hecap_ie->he_phy_cap[10] &= UAP_HE_2G_PHY_CAP10_MASK;
    }
    return;
}

#if 1
/**
 *  @brief Sets up the CFG802.11 specific HE capability fields *  with default
 * values
 *
 *  @param priv         A pointer to moal private structure
 *  @param iftype_data  A pointer to ieee80211_sband_iftype_data structure
 *
 *  @return             N/A
 */
int wifi_setup_he_cap(nxp_wifi_he_capabilities *he_cap, t_u8 band)
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[1];

    mlan_fw_info fw_info;
    t_u8 extra_mcs_size           = 0;
    int ppe_threshold_len         = 0;
    mlan_ds_11ax_he_capa *phe_cap = NULL;
    t_u8 hw_hecap_len             = 0;

    memset(&fw_info, 0, sizeof(mlan_fw_info));

    wifi_request_get_fw_info(priv, &fw_info);

#if CONFIG_5GHz_SUPPORT
    if (band == BAND_5GHZ)
    {
        phe_cap      = (mlan_ds_11ax_he_capa *)fw_info.hw_he_cap;
        hw_hecap_len = fw_info.hw_hecap_len;
        wifi_uap_update_11ax_ie(BAND_5GHZ, phe_cap);
    }
    else
#endif
#ifdef ENABLE_802_116E
        if (band == BAND_6GHZ)
    {
        phe_cap      = (mlan_ds_11ax_he_capa *)fw_info.hw_he_cap;
        hw_hecap_len = fw_info.hw_hecap_len;
        wifi_uap_update_11ax_ie(BAND_6GHZ, phe_cap);
    }
    else
#endif
    {
        phe_cap      = (mlan_ds_11ax_he_capa *)fw_info.hw_2g_he_cap;
        hw_hecap_len = fw_info.hw_2g_hecap_len;
        wifi_uap_update_11ax_ie(BAND_2GHZ, phe_cap);
    }

    if (!hw_hecap_len)
        return -WM_FAIL;

    he_cap->he_supported = 1;
    (void)__memcpy(priv, he_cap->mac_cap, phe_cap->he_mac_cap, sizeof(phe_cap->he_mac_cap));
    (void)__memcpy(priv, he_cap->phy_cap, phe_cap->he_phy_cap, sizeof(phe_cap->he_phy_cap));
    memset(&he_cap->mcs, 0xff, WIFI_HE_MAX_MCS_CAPAB_SIZE);
    (void)__memcpy(priv, &he_cap->mcs, phe_cap->he_txrx_mcs_support, sizeof(phe_cap->he_txrx_mcs_support));

    // Support 160Mhz
    if (phe_cap->he_phy_cap[0] & MBIT(3))
        extra_mcs_size += 4;

    // Support 80+80
    if (phe_cap->he_phy_cap[0] & MBIT(4))
        extra_mcs_size += 4;
    if (extra_mcs_size)
        (void)__memcpy(priv, (t_u8 *)&he_cap->mcs + sizeof(phe_cap->he_txrx_mcs_support), phe_cap->val, extra_mcs_size);

#define HE_CAP_FIX_SIZE 22
    // Support PPE threshold
    ppe_threshold_len = phe_cap->len - HE_CAP_FIX_SIZE - extra_mcs_size;
    if (phe_cap->he_phy_cap[6] & MBIT(7) && ppe_threshold_len)
    {
        (void)__memcpy(priv, he_cap->ppet, &phe_cap->val[extra_mcs_size], ppe_threshold_len);
    }
    else
    {
        he_cap->phy_cap[6] &= ~MBIT(7);
        wifi_d("Clear PPE threshold 0x%x\r\n", he_cap->phy_cap[7]);
    }
#ifdef ENABLE_802_116E
    if (band->band == BAND_6GHZ)
        iftype_data->he_6ghz_capa.capa = fw_info.hw_he_6g_cap;
#endif
    LEAVE();

    return WM_SUCCESS;
}
#else
/**
 *  @brief setup uap he_cap based on FW he_cap
 *
 *  @param priv         A pointer to moal private structure
 *  @param wait_option  wait_option
 *
 *  @return             N/A
 */
void woal_cfg80211_setup_uap_he_cap(moal_private *priv, t_u8 wait_option)
{
    mlan_ds_11ax_he_capa *phe_cap = NULL;
    mlan_ds_11ax_he_cfg he_cfg;
    t_u8 hw_hecap_len;
    mlan_fw_info fw_info;
#if UAP_SUPPORT
    int ret = 0;
#endif

    woal_request_get_fw_info(priv, MOAL_IOCTL_WAIT, &fw_info);

    // Enable 2G 11AX on UAP
    if (fw_info.fw_bands & BAND_GAX)
    {
        memset(&he_cfg, 0, sizeof(he_cfg));
        phe_cap      = (mlan_ds_11ax_he_capa *)fw_info.hw_2g_he_cap;
        hw_hecap_len = fw_info.hw_2g_hecap_len;
        if (hw_hecap_len)
        {
            woal_uap_update_11ax_ie(BAND_2GHZ, phe_cap);
            he_cfg.band = MBIT(0);
            moal_memcpy_ext(priv->phandle, &he_cfg.he_cap, phe_cap, hw_hecap_len, sizeof(mlan_ds_11ax_he_capa));
            DBG_HEXDUMP(MCMD_D, "2G HE_CFG ", (t_u8 *)&he_cfg, sizeof(he_cfg));
#if UAP_SUPPORT
            ret = woal_11ax_cfg(priv, MLAN_ACT_SET, &he_cfg, wait_option);
            if (ret)
                PRINTM(MERROR, "Fail to set 2G HE CAP\n");
#endif
        }
    }
#ifdef ENABLE_802_11A
    // Enable 5G 11AX on UAP
    if (fw_info.fw_bands & BAND_AAX)
    {
        memset(&he_cfg, 0, sizeof(he_cfg));
        phe_cap      = (mlan_ds_11ax_he_capa *)fw_info.hw_he_cap;
        hw_hecap_len = fw_info.hw_hecap_len;
        if (hw_hecap_len)
        {
            woal_uap_update_11ax_ie(BAND_5GHZ, phe_cap);
            he_cfg.band = MBIT(1);
            moal_memcpy_ext(priv->phandle, &he_cfg.he_cap, phe_cap, hw_hecap_len, sizeof(mlan_ds_11ax_he_capa));
            DBG_HEXDUMP(MCMD_D, "5G HE_CFG ", (t_u8 *)&he_cfg, sizeof(he_cfg));
#if UAP_SUPPORT
            ret = woal_11ax_cfg(priv, MLAN_ACT_SET, &he_cfg, wait_option);
            if (ret)
                PRINTM(MERROR, "Fail to set 5G HE CAP\n");
#endif
        }
    }
#endif
    return;
}
#endif
#endif

/**
 *  @brief Set/Get sta information parameters
 *
 *  @param priv             A pointer to moal_private structure
 *  @param action           MLAN_ACT_SET or MLAN_ACT_GET
 *  @param wait_option      Wait option
 *  @param sys_cfg          A pointer to mlan_uap_bss_param structure
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wifi_uap_sta_info(mlan_private *priv, t_u16 action, mlan_ds_sta_info *sta_info)
{
    int ret;
    mlan_ds_bss bss;
    mlan_ioctl_req ioctl_buf;

    ENTER();

    (void)memset(&bss, 0x00, sizeof(mlan_ds_bss));

    bss.sub_command = MLAN_OID_UAP_ADD_STATION;

    // if (action == HostCmd_ACT_ADD_STA)
    memcpy((void *)&bss.param.sta_info, (const void *)sta_info, sizeof(mlan_ds_sta_info) + (size_t)sta_info->tlv_len);

    (void)memset(&ioctl_buf, 0x00, sizeof(mlan_ioctl_req));

    ioctl_buf.req_id = (t_u32)MLAN_IOCTL_BSS;
    /** Pointer to buffer */
    ioctl_buf.pbuf = (t_u8 *)&bss;

    ret = wifi_uap_prepare_and_send_cmd(priv, HostCmd_CMD_ADD_NEW_STATION, action, 0, &ioctl_buf, NULL,
                                        MLAN_BSS_TYPE_UAP, NULL);
    if (ret != WM_SUCCESS)
    {
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief uap remove sta
 *
 *  @param priv             A pointer to moal_private structure
 *  @param addr             A pointer to mac address
 *
 *  @return                 MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wifi_uap_sta_remove(mlan_private *priv, const uint8_t *addr)
{
    int ret;
    mlan_deauth_param data_buf;

    ENTER();

    (void)memset(&data_buf, 0x00, sizeof(data_buf));

    memcpy((void *)data_buf.mac_addr, (const void *)addr, sizeof(data_buf.mac_addr));
    data_buf.reason_code = WLAN_REASON_UNSPECIFIED;

    ret = wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_STA_DEAUTH, 0, 0, NULL, &data_buf,
                                        MLAN_BSS_TYPE_UAP, NULL);
    if (ret != WM_SUCCESS)
    {
        return MLAN_STATUS_FAILURE;
    }

    return MLAN_STATUS_SUCCESS;
}

int wifi_nxp_sta_add(nxp_wifi_sta_info_t *params)
{
    mlan_private *priv         = (mlan_private *)mlan_adap->priv[1];
    int ret                    = 0;
    mlan_ds_sta_info *sta_info = NULL;
    t_u8 *pos;
    t_u8 qosinfo;
    MrvlIEtypes_Data_t *tlv;
    t_u32 req_len = 0;
#if CONFIG_11AX
    MrvlExtIEtypes_Data_t *ext_tlv;
#endif

    ENTER();

    if (!params)
    {
        ret = -WM_FAIL;
        goto done;
    }

    req_len = sizeof(mlan_ds_sta_info);
    if (params->ext_capab_len)
        req_len += sizeof(MrvlIEtypesHeader_t) + params->ext_capab_len;
    if (params->supp_rates_len)
        req_len += sizeof(MrvlIEtypesHeader_t) + params->supp_rates_len;
    if (params->qosinfo)
        req_len += sizeof(MrvlIEtypesHeader_t) + sizeof(qosinfo);
    if (params->ht_capab_len)
        req_len += sizeof(MrvlIEtypesHeader_t) + sizeof(ieee80211_ht_capab_t);
#if CONFIG_11AC
    if (params->vht_capab_len)
        req_len += sizeof(MrvlIEtypesHeader_t) + sizeof(ieee80211_vht_capab_t);
#endif

#if CONFIG_11AX
    if (params->he_capab_len)
        req_len += sizeof(MrvlExtIEtypesHeader_t) + params->he_capab_len;
#endif

    sta_info = OSA_MemoryAllocate(req_len);
    if (!sta_info)
    {
        wuap_e("Fail to alloc memory for mlan_ds_sta_info");
        ret = -WM_FAIL;
        goto done;
    }

    memset(sta_info, 0x00, req_len);

    sta_info->listen_interval = params->listen_interval;
    sta_info->aid             = params->aid;
    sta_info->cap_info        = params->capability;
    sta_info->tlv_len         = 0;
    sta_info->sta_flags       = params->flags;

    memcpy(sta_info->peer_mac, params->addr, MLAN_MAC_ADDR_LENGTH);

    wuap_d("wlan: UAP/GO add peer station, address =" MACSTR "", MAC2STR(params->addr));

    wuap_d("sta_flags=0x%x listen_interval=%d aid=%d cap_info=0x%x", params->flags, params->listen_interval,
           params->aid, params->capability);

    pos = &sta_info->tlv[0];
    if (params->ext_capab_len)
    {
        tlv              = (MrvlIEtypes_Data_t *)pos;
        tlv->header.type = EXT_CAPABILITY;
        tlv->header.len  = params->ext_capab_len;
        memcpy(tlv->data, params->ext_capab, tlv->header.len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        sta_info->tlv_len += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
    if (params->supp_rates_len)
    {
        tlv              = (MrvlIEtypes_Data_t *)pos;
        tlv->header.type = SUPPORTED_RATES;
        tlv->header.len  = params->supp_rates_len;
        memcpy(tlv->data, params->supp_rates, tlv->header.len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        sta_info->tlv_len += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
    if (params->qosinfo)
    {
        tlv              = (MrvlIEtypes_Data_t *)pos;
        tlv->header.type = QOS_INFO;
        tlv->header.len  = sizeof(qosinfo);
        qosinfo          = params->qosinfo;
        memcpy(tlv->data, &qosinfo, tlv->header.len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        sta_info->tlv_len += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
    if (params->ht_capab_len)
    {
        tlv              = (MrvlIEtypes_Data_t *)pos;
        tlv->header.type = HT_CAPABILITY;
        tlv->header.len  = sizeof(ieee80211_ht_capab_t);
        memcpy(tlv->data, &params->ht_capab, tlv->header.len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        sta_info->tlv_len += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
#if CONFIG_11AC
    if (params->vht_capab_len)
    {
        tlv              = (MrvlIEtypes_Data_t *)pos;
        tlv->header.type = VHT_CAPABILITY;
        tlv->header.len  = sizeof(ieee80211_vht_capab_t);
        memcpy(tlv->data, &params->vht_capab, tlv->header.len);
        pos += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        sta_info->tlv_len += sizeof(MrvlIEtypesHeader_t) + tlv->header.len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
#endif
#if CONFIG_11AX
    if (params->he_capab_len)
    {
        ext_tlv                = (MrvlExtIEtypes_Data_t *)pos;
        ext_tlv->header.type   = EXTENSION;
        ext_tlv->header.len    = params->he_capab_len + sizeof(u8);
        ext_tlv->header.ext_id = HE_CAPABILITY;
        memcpy(ext_tlv->data, (u8 *)&params->he_capab, params->he_capab_len);
        pos += sizeof(MrvlExtIEtypesHeader_t) + params->he_capab_len;
        sta_info->tlv_len += sizeof(MrvlExtIEtypesHeader_t) + params->he_capab_len;
        tlv = (MrvlIEtypes_Data_t *)pos;
    }
#endif

    if (MLAN_STATUS_SUCCESS != wifi_uap_sta_info(priv, HostCmd_ACT_ADD_STA, sta_info))
    {
        wuap_e("uAP add station failed");
        ret = -WM_FAIL;
        goto done;
    }

done:
    if (sta_info)
        OSA_MemoryFree(sta_info);

    LEAVE();
    return ret;
}

int wifi_nxp_sta_remove(const uint8_t *addr)
{
    mlan_private *priv         = (mlan_private *)mlan_adap->priv[1];
    int ret                    = 0;
    mlan_ds_sta_info *sta_info = NULL;

    ENTER();

    if (!addr)
    {
        ret = -WM_FAIL;
        goto done;
    }

    if (MLAN_STATUS_SUCCESS != wifi_uap_sta_remove(priv, addr))
    {
        wuap_e("uAP remove station failed");
        ret = -WM_FAIL;
        goto done;
    }

#if 0
    sta_info = OSA_MemoryAllocate(sizeof(mlan_ds_sta_info));
    if (!sta_info)
    {
        wuap_e("Fail to alloc memory for mlan_ds_sta_info");
        ret = -WM_FAIL;
        goto done;
    }

    memset(sta_info, 0x00, sizeof(mlan_ds_sta_info));

    memcpy(sta_info->peer_mac, addr, MLAN_MAC_ADDR_LENGTH);

    wuap_d("wlan: UAP/GO remove peer station, address =" MACSTR "", MAC2STR(addr));

    if (MLAN_STATUS_SUCCESS != wifi_uap_sta_info(priv, HostCmd_ACT_REMOVE_STA, sta_info))
    {
        wuap_e("uAP remove station failed");
        ret = -WM_FAIL;
        goto done;
    }
#endif

done:
#if 0
    if (sta_info)
        OSA_MemoryFree(sta_info);
#endif

    LEAVE();
    return ret;
}

int wifi_set_uap_rts(int rts_threshold)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    MrvlIEtypes_rts_threshold_t rts_threshold_tlv;

    (void)memset(&rts_threshold_tlv, 0, sizeof(MrvlIEtypes_rts_threshold_t));
    rts_threshold_tlv.header.type   = TLV_TYPE_UAP_RTS_THRESHOLD;
    rts_threshold_tlv.header.len    = (t_u16)sizeof(MrvlIEtypes_rts_threshold_t);
    rts_threshold_tlv.rts_threshold = (t_u16)rts_threshold;

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_SET, 0, MNULL,
                                           &rts_threshold_tlv, MLAN_BSS_TYPE_UAP, NULL);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to set rts threshold");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

int wifi_set_uap_frag(int frag_threshold)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    MrvlIEtypes_frag_threshold_t frag_threshold_tlv;

    (void)memset(&frag_threshold_tlv, 0, sizeof(MrvlIEtypes_frag_threshold_t));
    frag_threshold_tlv.header.type    = TLV_TYPE_UAP_RTS_THRESHOLD;
    frag_threshold_tlv.header.len     = (t_u16)sizeof(MrvlIEtypes_frag_threshold_t);
    frag_threshold_tlv.frag_threshold = (t_u16)frag_threshold;

    int rv = wifi_uap_prepare_and_send_cmd(pmpriv, HOST_CMD_APCMD_SYS_CONFIGURE, HostCmd_ACT_GEN_SET, 0, MNULL,
                                           &frag_threshold_tlv, MLAN_BSS_TYPE_UAP, NULL);
    if (rv != WM_SUCCESS)
    {
        return rv;
    }

    if (wm_wifi.cmd_resp_status != 0)
    {
        wifi_w("Unable to set frag threshold");
        return wm_wifi.cmd_resp_status;
    }

    return rv;
}

void wifi_nxp_uap_disconnect(mlan_private *priv, t_u16 reason_code, t_u8 *mac)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    nxp_wifi_event_mlme_t *mgmt_rx  = &wm_wifi.mgmt_rx;
    t_u8 *pos                       = MNULL;
    t_u16 reason;
    t_u32 payload_len;

    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(SUBTYPE_DEAUTH, (mlan_802_11_mac_addr *)(void *)priv->curr_addr,
        (mlan_802_11_mac_addr *)(void *)mac, (mlan_802_11_mac_addr *)(void *)priv->curr_addr, 100);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_e("No memory available for deauth");
        return;
    }

    pos = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    reason = wlan_cpu_to_le16(reason_code);
    (void)memcpy(pos, &reason, sizeof(reason));
    payload_len = sizeof(reason) + sizeof(pmgmt_pkt_hdr->wlan_header);

    if (payload_len <= (int)sizeof(mgmt_rx->frame.frame))
    {
        memset(mgmt_rx, 0, sizeof(nxp_wifi_event_mlme_t));
        mgmt_rx->frame.frame_len = payload_len;
        (void)memcpy((void *)mgmt_rx->frame.frame, (const void *)(&pmgmt_pkt_hdr->wlan_header), mgmt_rx->frame.frame_len);
        if (wm_wifi.supp_if_callbk_fns->mgmt_rx_callbk_fn)
        {
            wm_wifi.supp_if_callbk_fns->mgmt_rx_callbk_fn(wm_wifi.hapd_if_priv, mgmt_rx, mgmt_rx->frame.frame_len);
        }
    }
    else
    {
        wifi_e("Insufficient frame buffer");
    }

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif
}

int wifi_nxp_stop_ap()
{
    mlan_private *priv = (mlan_private *)mlan_adap->priv[1];
    int ret            = WM_SUCCESS;

    if ((mlan_adap->in_reset == MTRUE) && (priv->media_connected == MFALSE))
    {
        return ret;
    }

    (void)wifi_set_rx_mgmt_indication(MLAN_BSS_TYPE_UAP, 0);

    if (priv->beacon_vendor_index != -1)
    {
        ret = wifi_clear_mgmt_ie2(MLAN_BSS_TYPE_UAP, priv->beacon_vendor_index);
        if (ret != WM_SUCCESS)
        {
            wuap_e("Clear uAP vendor IE failed");
            return -WM_FAIL;
        }
        priv->beacon_vendor_index = -1;
    }

    ret = wifi_nxp_set_mgmt_ies(priv, NULL, 0, NULL, 0, NULL, 0, NULL, 0);
    if (ret != WM_SUCCESS)
    {
        wuap_e("Set uAP mgmt ie failed");
        ret = -WM_FAIL;
        goto done;
    }

    wuap_d("Stopping BSS"); /* Stop BSS */
    if (MLAN_STATUS_SUCCESS != wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_BSS_STOP, HostCmd_ACT_GEN_SET, 0,
                                                             NULL, NULL, MLAN_BSS_TYPE_UAP, NULL))
    {
        wuap_e("Stop BSS failed");
        return -WM_FAIL;
    }
    wifi_uap_clear_domain_info();
    priv->uap_host_based = MFALSE;

    wuap_d("wlan: AP stopped");

done:
    LEAVE();
    return ret;
}

int wifi_nxp_set_acl(nxp_wifi_acl_info_t *params)
{
    mlan_private *priv             = (mlan_private *)mlan_adap->priv[1];
    int ret                        = -WM_FAIL;
    mlan_uap_bss_param *sys_config = NULL;
    bool bss_started               = MFALSE;

    if (!params)
    {
        goto done;
    }

    sys_config = OSA_MemoryAllocate(sizeof(mlan_uap_bss_param));

    if (!sys_config)
    {
        wuap_e("Fail to alloc memory for mlan_uap_bss_param");
        goto done;
    }

    memset(sys_config, 0x00, sizeof(mlan_uap_bss_param));

    if (params->num_mac_acl <= MAX_MAC_FILTER_NUM)
        sys_config->filter.mac_count = params->num_mac_acl;
    else
        sys_config->filter.mac_count = MAX_MAC_FILTER_NUM;

    if (params->acl_policy == 1U)
        sys_config->filter.filter_mode = MAC_FILTER_MODE_ALLOW_MAC;
    else if (params->acl_policy == 0U)
        sys_config->filter.filter_mode = MAC_FILTER_MODE_BLOCK_MAC;
    memcpy(sys_config->filter.mac_list, params->mac_acl, sys_config->filter.mac_count * sizeof(mlan_802_11_mac_addr));

    if (priv->uap_host_based == MTRUE)
    {
        bss_started = MTRUE;

        if (MLAN_STATUS_SUCCESS != wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_BSS_STOP, HostCmd_ACT_GEN_SET, 0,
                                                                 NULL, NULL, MLAN_BSS_TYPE_UAP, NULL))
        {
            wuap_e("Stop BSS failed");
        }
    }

    if (MLAN_STATUS_SUCCESS == wifi_set_get_sys_config(priv, MLAN_ACT_SET, sys_config))
    {
        ret = WM_SUCCESS;
    }

    if (bss_started)
    {
        if (MLAN_STATUS_SUCCESS != wifi_uap_prepare_and_send_cmd(priv, HOST_CMD_APCMD_BSS_START, HostCmd_ACT_GEN_SET, 0,
                                                                 NULL, NULL, MLAN_BSS_TYPE_UAP, NULL))
        {
            wuap_e("Start BSS failed");
        }
    }

done:
    if (sys_config)
        OSA_MemoryFree(sys_config);

    LEAVE();
    return ret;
}

#endif /* CONFIG_HOSTAPD */
#endif /* CONFIG_NXP_WIFI_SOFTAP_SUPPORT */
