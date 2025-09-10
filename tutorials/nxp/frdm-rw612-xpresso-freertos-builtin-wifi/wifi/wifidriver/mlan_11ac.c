/** @file mlan_11ac.c
 *
 *  @brief This file defines the private and adapter data
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

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
#if CONFIG_11AC
/**
 *  @brief determine the center frquency center index for bandwidth
 *         of 80 MHz and 160 MHz
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param band         band
 *  @param pri_chan     primary channel
 *  @param chan_bw      channel bandwidth
 *
 *  @return             channel center frequency center, if found; O, otherwise
 */

t_u8 wlan_get_center_freq_idx(IN mlan_private *pmpriv, IN t_u8 band, IN t_u32 pri_chan, IN t_u8 chan_bw)
{
    t_u8 center_freq_idx = 0;

    if ((band & BAND_AAC) != 0U)
    {
        switch (pri_chan)
        {
            case 36:
            case 40:
            case 44:
            case 48:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 42;
                }
                break;
            case 52:
            case 56:
            case 60:
            case 64:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 58;
                }
                else if (chan_bw == (t_u8)CHANNEL_BW_160MHZ)
                {
                    center_freq_idx = 50;
                }
                else
                {
                    /* Do nothing */
                }
                break;
            case 100:
            case 104:
            case 108:
            case 112:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 106;
                }
                break;
            case 116:
            case 120:
            case 124:
            case 128:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 122;
                }
                else if (chan_bw == (t_u8)CHANNEL_BW_160MHZ)
                {
                    center_freq_idx = 114;
                }
                else
                {
                    /* Do nothing */
                }
                break;
            case 132:
            case 136:
            case 140:
            case 144:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 138;
                }
                break;
            case 149:
            case 153:
            case 157:
            case 161:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 155;
                }
                break;
            case 165:
            case 169:
            case 173:
            case 177:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 171;
                }
                break;
            case 184:
            case 188:
            case 192:
            case 196:
                if (chan_bw == (t_u8)CHANNEL_BW_80MHZ)
                {
                    center_freq_idx = 190;
                }
                break;
            default: /* error. go to the default */
                center_freq_idx = 42;
                break;
        }
    }
    return center_freq_idx;
}
#endif

/**
 *  @brief This function gets the bitmap of nss which supports VHT mcs
 *
 *  @param mcs_map_set  VHT mcs map
 *
 *  @return             The bitmap of supported nss
 */
static t_u8 wlan_get_nss_vht_mcs(t_u16 mcs_map_set)
{
    t_u8 nss, nss_map = 0;
    for (nss = 1; nss <= 8U; nss++)
    {
        if (GET_VHTNSSMCS(mcs_map_set, nss) != NO_NSS_SUPPORT)
        {
            nss_map |= 1U << (nss - 1U);
        }
    }
    PRINTM(MCMND, "Supported nss bit map:0x%02x\n", nss_map);
    return nss_map;
}

/**
 *  @brief This function gets the number of nss which supports VHT mcs
 *
 *  @param mcs_map_set  VHT mcs map
 *
 *  @return             Number of supported nss
 */
static t_u8 wlan_get_nss_num_vht_mcs(t_u16 mcs_map_set)
{
    t_u8 nss, nss_num = 0;
    for (nss = 1; nss <= 8U; nss++)
    {
        if (GET_VHTNSSMCS(mcs_map_set, nss) != NO_NSS_SUPPORT)
        {
            nss_num++;
        }
    }
    PRINTM(MCMND, "Supported nss:%d\n", nss_num);
    return nss_num;
}

/**
 *  @brief This function fills the cap info
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param pht_cap      A pointer to MrvlIETypes_HTCap_t structure
 *  @param bands        Band configuration
 *
 *  @return             N/A
 */
static void wlan_fill_cap_info(mlan_private *priv, VHT_capa_t *vht_cap, t_u16 bands)
{
    mlan_adapter *pmadapter = priv->adapter;
    t_u32 usr_dot_11ac_dev_cap;

    ENTER();

#if CONFIG_5GHz_SUPPORT
    if ((bands & BAND_A) != 0U)
    {
        usr_dot_11ac_dev_cap = pmadapter->usr_dot_11ac_dev_cap_a;
    }
    else
#endif
    {
        usr_dot_11ac_dev_cap = pmadapter->usr_dot_11ac_dev_cap_bg;
    }

    vht_cap->vht_cap_info = usr_dot_11ac_dev_cap;
#ifdef RW610
    if (GET_VHTCAP_MAXMPDULEN(vht_cap->vht_cap_info) != 0U)
        RESET_11ACMAXMPDULEN(vht_cap->vht_cap_info);
#endif

    LEAVE();
}

#if CONFIG_11AC
/**
 *  @brief Set/get 11ac configuration
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return     MLAN_STATUS_PENDING --success, otherwise fail
 */

mlan_status wlan_11ac_ioctl_vhtcfg(IN mlan_private *pmpriv, IN t_u8 action, IN mlan_ds_11ac_vht_cfg *vht_cfg)
{
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    mlan_adapter *pmadapter = pmpriv->adapter;
    t_u16 cmd_action        = 0;
    t_u32 usr_vht_cap_info  = 0;
    t_u32 cfg_value         = 0;
    t_u32 hw_value          = 0;
    t_u8 nss                = 0;
    t_u16 no_nss_support    = NO_NSS_SUPPORT;

    ENTER();

#define VHT_CAP_INFO_BIT_FIELDS                                                                                \
    (MBIT(4) | MBIT(5) | MBIT(6) | MBIT(7) | MBIT(11) | MBIT(12) | MBIT(19) | MBIT(20) | MBIT(21) | MBIT(22) | \
     MBIT(28) | MBIT(29))

    if (action == (t_u8)MLAN_ACT_SET)
    {
        /** SET operation */
        /** validate the user input and correct it if necessary */
        if (pmpriv->bss_role == MLAN_BSS_ROLE_STA)
        {
            if (vht_cfg->txrx == 3U)
            {
                PRINTM(MERROR, "Configuration of VHT capabilities for TX/RX 3 is not supported in STA mode!\n");
                return MLAN_STATUS_FAILURE;
            }
        }
        if (pmpriv->bss_role == MLAN_BSS_ROLE_UAP)
        {
            if (vht_cfg->txrx != 3U)
            {
                PRINTM(MERROR, "Configuration of VHT capabilities for TX/RX %d is not supported in UAP mode!\n",
                       vht_cfg->txrx);

                return MLAN_STATUS_FAILURE;
            }
        }
        /** set bit fileds */
        usr_vht_cap_info = VHT_CAP_INFO_BIT_FIELDS & vht_cfg->vht_cap_info & pmadapter->hw_dot_11ac_dev_cap;
        /** set MAX MPDU LEN field (bit 0 - bit 1) */
        cfg_value = GET_VHTCAP_MAXMPDULEN(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_MAXMPDULEN(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_MAXMPDULEN(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set CHAN Width Set field (bit 2 - bit 3) */
        cfg_value = GET_VHTCAP_CHWDSET(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_CHWDSET(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_CHWDSET(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set Rx STBC field (bit 8 - bit 10) */
        cfg_value = GET_VHTCAP_RXSTBC(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_RXSTBC(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_RXSTBC(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set Steering Number of BFer Ant (bit 13 - bit 15) */
        cfg_value = GET_VHTCAP_SNBFERANT(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_SNBFERANT(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_SNBFERANT(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set Number of Sounding Dimension (bit 16 - bit 18) */
        cfg_value = GET_VHTCAP_NUMSNDDM(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_NUMSNDDM(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_NUMSNDDM(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set Number of Max AMPDU Length Exponent (bit 23 - bit 25) */
        cfg_value = GET_VHTCAP_MAXAMPDULENEXP(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_MAXAMPDULENEXP(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_MAXAMPDULENEXP(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** set VHT Link Adaptation Capable (bit 26 - bit 27) */
        cfg_value = GET_VHTCAP_LINKADPCAP(vht_cfg->vht_cap_info);
        hw_value  = GET_VHTCAP_LINKADPCAP(pmadapter->hw_dot_11ac_dev_cap);
        SET_VHTCAP_LINKADPCAP(usr_vht_cap_info, MIN(cfg_value, hw_value));
        /** update the user setting if it is beyond the hw capabiliteis */
        vht_cfg->vht_cap_info = usr_vht_cap_info;
        PRINTM(MINFO, "Set: vht cap info  0x%x\n", usr_vht_cap_info);

        /** update the RX MCS map */
        if ((vht_cfg->txrx & MLAN_RADIO_RX) != 0U)
        {
            /* use the previous user value */
            if (vht_cfg->vht_rx_mcs == 0xffffffffU)
            {
                vht_cfg->vht_rx_mcs = GET_VHTMCS(pmadapter->usr_dot_11ac_mcs_support);
            }
            for (nss = 1; nss <= 8U; nss++)
            {
                cfg_value = GET_VHTNSSMCS(vht_cfg->vht_rx_mcs, nss);
                hw_value  = GET_DEVNSSRXMCS(pmadapter->hw_dot_11ac_mcs_support, nss);
                if ((hw_value == NO_NSS_SUPPORT) || (cfg_value == NO_NSS_SUPPORT))
                {
                    SET_VHTNSSMCS(vht_cfg->vht_rx_mcs, nss, no_nss_support);
                }
                else
                {
                    SET_VHTNSSMCS(vht_cfg->vht_rx_mcs, nss, MIN(cfg_value, hw_value));
                }
            }
            PRINTM(MINFO, "Set: vht rx mcs set 0x%08x\n", vht_cfg->vht_rx_mcs);
            /* use the previous user value */
            if (vht_cfg->vht_tx_mcs == 0xffffffffU)
            {
                vht_cfg->vht_tx_mcs = GET_VHTMCS(pmadapter->usr_dot_11ac_mcs_support >> 16);
            }
            for (nss = 1; nss <= 8U; nss++)
            {
                cfg_value = GET_VHTNSSMCS(vht_cfg->vht_tx_mcs, nss);
                hw_value  = GET_DEVNSSTXMCS(pmadapter->hw_dot_11ac_mcs_support, nss);
                if ((hw_value == NO_NSS_SUPPORT) || (cfg_value == NO_NSS_SUPPORT))
                {
                    SET_VHTNSSMCS(vht_cfg->vht_tx_mcs, nss, no_nss_support);
                }
                else
                {
                    SET_VHTNSSMCS(vht_cfg->vht_tx_mcs, nss, MIN(cfg_value, hw_value));
                }
            }

            PRINTM(MINFO, "Set: vht tx mcs set 0x%08x\n", vht_cfg->vht_tx_mcs);
            if (!vht_cfg->skip_usr_11ac_mcs_cfg)
            {
                RESET_DEVRXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
                pmadapter->usr_dot_11ac_mcs_support |= GET_VHTMCS(vht_cfg->vht_rx_mcs);
                RESET_DEVTXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
                pmadapter->usr_dot_11ac_mcs_support |= (GET_VHTMCS(vht_cfg->vht_tx_mcs) << 16);
                PRINTM(MINFO, "Set: vht mcs set 0x%08x\n", pmadapter->usr_dot_11ac_mcs_support);
            }
            else
            {
                PRINTM(MINFO, "Skipped user 11ac mcs configuration\n");
                vht_cfg->skip_usr_11ac_mcs_cfg = MFALSE;
            }
        }
    }

    if (pmpriv->bss_role == MLAN_BSS_ROLE_STA)
    {
        if ((vht_cfg->txrx & MLAN_RADIO_RX) != 0U)
        {
            /* maximum VHT configuration used in association */

            if (action == (t_u8)MLAN_ACT_SET)
            {
                if (vht_cfg->band == BAND_SELECT_BG)
                {
                    pmadapter->usr_dot_11ac_dev_cap_bg = usr_vht_cap_info;
                }
#if CONFIG_5GHz_SUPPORT
                else if (vht_cfg->band == BAND_SELECT_A)
                {
                    pmadapter->usr_dot_11ac_dev_cap_a = usr_vht_cap_info;
                }
#endif
                else
                {
                    pmadapter->usr_dot_11ac_dev_cap_bg = usr_vht_cap_info;
#if CONFIG_5GHz_SUPPORT
                    pmadapter->usr_dot_11ac_dev_cap_a = usr_vht_cap_info;
#endif
                }
                pmadapter->usr_dot_11ac_bw = (t_u8)vht_cfg->bwcfg;
            }
            else
            {
                /** GET operation */
                if (vht_cfg->band == BAND_SELECT_BG)
                {
                    vht_cfg->vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_bg;
                    PRINTM(MINFO, "Get: vht cap info for 2.4GHz 0x%x\n", pmadapter->usr_dot_11ac_dev_cap_bg);
                }
#if CONFIG_5GHz_SUPPORT
                else if (vht_cfg->band == BAND_SELECT_A)
                {
                    vht_cfg->vht_cap_info = pmadapter->usr_dot_11ac_dev_cap_a;
                    PRINTM(MINFO, "Get: vht cap info for 5GHz 0x%x\n", pmadapter->usr_dot_11ac_dev_cap_a);
                }
#endif
                else
                {
                    PRINTM(MINFO, "Get: invalid band selection for vht cap info\n");
                    ret = MLAN_STATUS_FAILURE;
                }
                vht_cfg->bwcfg      = pmadapter->usr_dot_11ac_bw;
                vht_cfg->vht_rx_mcs = GET_DEVRXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
                vht_cfg->vht_tx_mcs = GET_DEVTXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
                vht_cfg->vht_rx_max_rate =
                    wlan_convert_mcsmap_to_maxrate(pmpriv, (t_u8)vht_cfg->band, (t_u16)vht_cfg->vht_rx_mcs);
                vht_cfg->vht_tx_max_rate =
                    wlan_convert_mcsmap_to_maxrate(pmpriv, (t_u8)vht_cfg->band, (t_u16)vht_cfg->vht_tx_mcs);
            }
            LEAVE();
            return ret;
        }
    }

    if (action == (t_u8)MLAN_ACT_SET)
    {
        cmd_action = HostCmd_ACT_GEN_SET;
    }
    else
    {
        cmd_action = HostCmd_ACT_GEN_GET;
    }

    /* Send request to firmware */
    ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_11AC_CFG, cmd_action, 0, NULL, (t_void *)vht_cfg);
    if (ret == MLAN_STATUS_SUCCESS)
    {
        ret = MLAN_STATUS_PENDING;
    }

    LEAVE();
    return ret;
}
#endif

#if 0
/**
 *  @brief Get/Set Operating Mode Notification cfg
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return     MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_11ac_ioctl_opermodecfg(
    IN pmlan_adapter    pmadapter,
    IN pmlan_ioctl_req  pioctl_req)
{
    mlan_private *pmpriv = pmadapter->priv[pioctl_req->bss_index];
    t_u8 hw_bw_160or8080 = 0;
    t_u8 hw_rx_nss = 0;

    ENTER();

    cfg = (mlan_ds_11ac_cfg *)pioctl_req->pbuf;
    if (pioctl_req->action == MLAN_ACT_GET) {
        cfg->param.opermode_cfg.bw = pmpriv->usr_dot_11ac_opermode_bw;
        cfg->param.opermode_cfg.nss = pmpriv->usr_dot_11ac_opermode_nss;
    } else if (pioctl_req->action == MLAN_ACT_SET) {
        hw_bw_160or8080 = GET_VHTCAP_CHWDSET(pmadapter->hw_dot_11ac_dev_cap);
        hw_rx_nss = wlan_get_nss_num_vht_mcs(GET_DEVRXMCSMAP(pmadapter->hw_dot_11ac_mcs_support));
        if ((((cfg->param.opermode_cfg.bw - 1) > BW_80MHZ) && !hw_bw_160or8080) ||
            (cfg->param.opermode_cfg.nss > hw_rx_nss)) {
            PRINTM(MERROR, "bw or nss NOT supported. HW support bw_160or8080=%d rx_nss=%d.\n",
                hw_bw_160or8080, hw_rx_nss);
            LEAVE();
            return MLAN_STATUS_FAILURE;
        }
        pmpriv->usr_dot_11ac_opermode_bw = cfg->param.opermode_cfg.bw;
        pmpriv->usr_dot_11ac_opermode_nss = cfg->param.opermode_cfg.nss;
    }

    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/**
 *  @brief Get supported MCS set
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return     MLAN_STATUS_SUCCESS --success, otherwise fail
 */
static mlan_status wlan_11ac_ioctl_supported_mcs_set(
    IN pmlan_adapter    pmadapter,
    IN pmlan_ioctl_req  pioctl_req)
{
    /*mlan_ds_11ac_cfg *cfg= MNULL;*/
    /*int rx_mcs_supp;*/
    /*t_u8 mcs_set[NUM_MCS_SUPP];*/

    ENTER();
#if 0
    if (pioctl_req->action == MLAN_ACT_SET) {
        PRINTM(MERROR, "Set operation is not supported\n");
        pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
        LEAVE();
        return MLAN_STATUS_FAILURE;
    }
    rx_mcs_supp = GET_11ACRXMCSSUPP(pmadapter->usr_dot_11ac_mcs_support);
    /* Set MCS */
    (void)__memset(pmadapter, (t_u8 *) mcs_set, 0xff, rx_mcs_supp);
    /* Clear all the other values */
    (void)__memset(pmadapter, (t_u8 *) &mcs_set[rx_mcs_supp], 0,
                 NUM_MCS_FIELD - rx_mcs_supp);
    /* Set MCS32 with 40MHz support */
    if (ISSUPP_CHANWIDTH80(pmadapter->usr_dot_11ac_dev_cap_bg)
#if CONFIG_5GHz_SUPPORT
        || ISSUPP_CHANWIDTH80(pmadapter->usr_dot_11ac_dev_cap_a)
#endif
        )
        SETHT_MCS32(mcs_set);

    cfg = (mlan_ds_11ac_cfg *)pioctl_req->pbuf;
    (void)__memcpy(pmadapter, cfg->param.supported_mcs_set, mcs_set, NUM_MCS_SUPP);

#endif
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

/********************************************************
			Global Functions
********************************************************/

/**
 *  @brief This function prints the 802.11ac device capability
 *
 *  @param pmadapter     A pointer to mlan_adapter structure
 *  @param cap           Capability value
 *
 *  @return        N/A
 */
void wlan_show_dot11acdevcap(pmlan_adapter pmadapter, t_u32 cap)
{
    ENTER();

    switch (GET_VHTCAP_MAXMPDULEN(cap)) {
        case 0x0:
            PRINTM(MINFO, "GET_HW_SPEC: Maximum MSDU length = 3895 octets\n");
            break;
        case 0x1:
            PRINTM(MINFO, "GET_HW_SPEC: Maximum MSDU length = 7991 octets\n");
            break;
        case 0x2:
            PRINTM(MINFO, "GET_HW_SPEC: Maximum MSDU length = 11454 octets\n");
            break;
        default:
            PRINTM(MINFO, "Unsupport value\n");
            break;
    }

    PRINTM(MINFO, "GET_HW_SPEC: HTC-VHT %s\n",
           (ISSUPP_11ACVHTHTCVHT(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: VHT TXOP PS %s\n",
           (ISSUPP_11ACVHTTXOPPS(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: MU RX beamformee %s\n",
           (ISSUPP_11ACMURXBEAMFORMEE(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: MU TX beamformee %s\n",
           (ISSUPP_11ACMUTXBEAMFORMEE(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: SU RX Beamformee %s\n",
           (ISSUPP_11ACSUBEAMFORMEE(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: SU TX Beamformer %s\n",
           (ISSUPP_11ACSUBEAMFORMER(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Rx STBC %s\n",
           (ISSUPP_11ACRXSTBC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Tx STBC %s\n",
           (ISSUPP_11ACTXSTBC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Short GI %s for 160MHz BW\n",
           (ISSUPP_11ACSGI160(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Short GI %s for 80MHz BW\n",
           (ISSUPP_11ACSGI80(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: LDPC coding %s\n",
           (ISSUPP_11ACLDPC(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Channel BW 20/40/80/160/80+80 MHz %s\n",
           (ISSUPP_11ACBW8080(cap) ? "supported" : "not supported"));
    PRINTM(MINFO, "GET_HW_SPEC: Channel BW 20/40/80/160 MHz %s\n",
           (ISSUPP_11ACBW160(cap) ? "supported" : "not supported"));

    LEAVE();
    return;
}

/**
 *  @brief This function prints the 802.11ac device MCS
 *
 *  @param pmadapter A pointer to mlan_adapter structure
 *  @param support   Support value
 *
 *  @return        N/A
 */
void wlan_show_dot11acmcssupport(pmlan_adapter pmadapter, t_u32 support)
{
    ENTER();

    PRINTM(MINFO, "GET_HW_SPEC: MCSs for %2dx%2d MIMO\n", GET_DEVRXMCSMAP(support),
                        GET_DEVTXMCSMAP(support));

    LEAVE();
    return;
}

#endif

/**
 *  @brief This function converts the 2-bit MCS map to the highest long GI
 *  VHT PPDU data rate
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param bands        Supported bands
 *  @param mcs_map      2-bit MCS map
 *
 *  @return             the max data rate for long GI
 */
t_u16 wlan_convert_mcsmap_to_maxrate(mlan_private *priv, t_u8 bands, t_u16 mcs_map)
{
    mlan_adapter *pmadapter = priv->adapter;
    t_u8 i;
    t_u8 nss;
    t_u8 max_mcs;
    t_u16 max_rate         = 0;
    t_u32 usr_vht_cap_info = 0;
    t_u32 usr_dot_11n_dev_cap;

    /* tables of the MCS map to the highest data rate (in Mbps)
     * supported for long GI */
    t_u16 max_rate_lgi_20MHZ[8][3] = {
        {0x41, 0x4E, 0x0},     /* NSS = 1 */
        {0x82, 0x9C, 0x0},     /* NSS = 2 */
        {0xC3, 0xEA, 0x104},   /* NSS = 3 */
        {0x104, 0x138, 0x0},   /* NSS = 4 */
        {0x145, 0x186, 0x0},   /* NSS = 5 */
        {0x186, 0x1D4, 0x208}, /* NSS = 6 */
        {0x1C7, 0x222, 0x0},   /* NSS = 7 */
        {0x208, 0x270, 0x0}    /* NSS = 8 */
    };

    t_u16 max_rate_lgi_40MHZ[8][3] = {
        {0x87, 0xA2, 0xB4},    /* NSS = 1 */
        {0x10E, 0x144, 0x168}, /* NSS = 2 */
        {0x195, 0x1E6, 0x21C}, /* NSS = 3 */
        {0x21C, 0x288, 0x2D0}, /* NSS = 4 */
        {0x2A3, 0x32A, 0x384}, /* NSS = 5 */
        {0x32A, 0x3CC, 0x438}, /* NSS = 6 */
        {0x3B1, 0x46E, 0x4EC}, /* NSS = 7 */
        {0x438, 0x510, 0x5A0}  /* NSS = 8 */
    };

    t_u16 max_rate_lgi_80MHZ[8][3] = {
        {0x124, 0x15F, 0x186}, /* NSS = 1 */
        {0x249, 0x2BE, 0x30C}, /* NSS = 2 */
        {0x36D, 0x41D, 0x492}, /* NSS = 3 */
        {0x492, 0x57C, 0x618}, /* NSS = 4 */
        {0x5B6, 0x6DB, 0x79E}, /* NSS = 5 */
        {0x6DB, 0x83A, 0x0},   /* NSS = 6 */
        {0x7FF, 0x999, 0xAAA}, /* NSS = 7 */
        {0x924, 0xAF8, 0xC30}  /* NSS = 8 */
    };
    t_u16 max_rate_lgi_160MHZ[8][3] = {
        {0x249, 0x2BE, 0x30C},   /* NSS = 1 */
        {0x492, 0x57C, 0x618},   /* NSS = 2 */
        {0x6DB, 0x83A, 0x0},     /* NSS = 3 */
        {0x924, 0xAF8, 0xC30},   /* NSS = 4 */
        {0xB6D, 0xDB6, 0xF3C},   /* NSS = 5 */
        {0xDB6, 0x1074, 0x1248}, /* NSS = 6 */
        {0xFFF, 0x1332, 0x1554}, /* NSS = 7 */
        {0x1248, 0x15F0, 0x1860} /* NSS = 8 */
    };

#if CONFIG_5GHz_SUPPORT
    if ((bands & BAND_AAC) != 0U)
    {
        usr_vht_cap_info    = pmadapter->usr_dot_11ac_dev_cap_a;
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_a;
    }
    else
    {
#endif
        usr_vht_cap_info    = pmadapter->usr_dot_11ac_dev_cap_bg;
        usr_dot_11n_dev_cap = pmadapter->usr_dot_11n_dev_cap_bg;
#if CONFIG_5GHz_SUPPORT
    }
#endif

    /* find the max NSS supported */
    nss = 0;
    for (i = 0; i < 8U; i++)
    {
        max_mcs = (t_u8)((mcs_map >> (2U * i)) & 0x3U);
        if (max_mcs < 3U)
        {
            nss = i;
        }
    }

    max_mcs = (t_u8)((mcs_map >> (2U * nss)) & 0x3U);
    /* if max_mcs is 3, nss must be 0 (SS = 1). Thus, max mcs is MCS 9*/
    if (max_mcs >= 3U)
    {
        max_mcs = (t_u8)2U;
    }

    if (GET_VHTCAP_CHWDSET(usr_vht_cap_info) != 0U)
    {
        /* support 160 MHz */
        max_rate = max_rate_lgi_160MHZ[nss][max_mcs];
        if (max_mcs >= 1U && max_rate == 0U)
        {
            /* MCS9 is not supported in NSS6 */
            max_rate = max_rate_lgi_160MHZ[nss][max_mcs - 1U];
        }
    }
    else
    {
        if (pmadapter->usr_dot_11ac_bw == BW_FOLLOW_VHTCAP)
        {
            max_rate = max_rate_lgi_80MHZ[nss][max_mcs];
            if (max_mcs >= 1U && max_rate == 0U)
            {
                /* MCS9 is not supported in NSS3 */
                max_rate = max_rate_lgi_80MHZ[nss][max_mcs - 1U];
            }
        }
        else
        {
            if (ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap) != 0U)
            {
                max_rate = max_rate_lgi_40MHZ[nss][max_mcs];
            }
            else
            {
                max_rate = max_rate_lgi_20MHZ[nss][max_mcs];
                /* MCS9 is not supported in NSS1/2/4/5/7/8 */
                if (max_mcs >= 1U && max_rate == 0U)
                {
                    max_rate = max_rate_lgi_20MHZ[nss][max_mcs - 1U];
                }
            }
        }
    }
    PRINTM(MCMND, "max_rate=%dM\n", max_rate);
    return max_rate;
}

/**
 *  @brief This function fills the VHT cap tlv out put format is LE, not CPU
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param pvht_cap      A pointer to MrvlIETypes_HTCap_t structure
 *  @param bands        Band configuration
 *  @param flag         TREU--pvht_cap has the setting for resp
 *                      MFALSE -- pvht_cap is clean
 *  @return             N/A
 */
void wlan_fill_vht_cap_tlv(mlan_private *priv, MrvlIETypes_VHTCap_t *pvht_cap, t_u16 bands, t_u8 flag)
{
    mlan_adapter *pmadapter = priv->adapter;
    t_u16 mcs_map_user      = 0;
    t_u16 mcs_map_resp      = 0;
    t_u32 mcs_map_result    = 0U;
    t_u16 mcs_user          = 0;
    t_u16 mcs_resp          = 0;
    t_u16 nss;
    t_u16 no_nss_support = NO_NSS_SUPPORT;

    ENTER();

    /* Fill VHT cap info */
    wlan_fill_cap_info(priv, &pvht_cap->vht_cap, bands);
    pvht_cap->vht_cap.vht_cap_info = wlan_cpu_to_le32(pvht_cap->vht_cap.vht_cap_info);

    /* Fill VHT MCS Set */
    /* rx MCS Set, find the minimum of the user rx mcs and ap rx mcs*/
    mcs_map_resp = mcs_map_user = (t_u16)GET_DEVRXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
    if (flag != 0U)
    {
        mcs_map_resp = wlan_le16_to_cpu(pvht_cap->vht_cap.mcs_sets.rx_mcs_map);
    }
    mcs_map_result = 0U;
    for (nss = 1; nss <= 8U; nss++)
    {
        mcs_user = GET_VHTNSSMCS(mcs_map_user, nss);
        mcs_resp = GET_VHTNSSMCS(mcs_map_resp, nss);
        if ((mcs_user == NO_NSS_SUPPORT) || (mcs_resp == NO_NSS_SUPPORT))
        {
            SET_VHTNSSMCS(mcs_map_result, nss, no_nss_support);
        }
        else
        {
            SET_VHTNSSMCS(mcs_map_result, nss, MIN(mcs_user, mcs_resp));
        }
    }
    /* rx MCS map */
    pvht_cap->vht_cap.mcs_sets.rx_mcs_map = wlan_cpu_to_le16(mcs_map_result);

    /* rx highest rate */
    pvht_cap->vht_cap.mcs_sets.rx_max_rate = wlan_convert_mcsmap_to_maxrate(priv, (t_u8)bands, (t_u16)mcs_map_result);
    pvht_cap->vht_cap.mcs_sets.rx_max_rate = wlan_cpu_to_le16(pvht_cap->vht_cap.mcs_sets.rx_max_rate);

    /* tx MCS Set find the minimum of the user tx mcs and ap tx mcs */
    mcs_map_resp = mcs_map_user = (t_u16)GET_DEVTXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
    if (flag != 0U)
    {
        mcs_map_resp = wlan_le16_to_cpu(pvht_cap->vht_cap.mcs_sets.tx_mcs_map);
    }
    mcs_map_result = 0U;
    for (nss = 1; nss <= 8U; nss++)
    {
        mcs_user = GET_VHTNSSMCS(mcs_map_user, nss);
        mcs_resp = GET_VHTNSSMCS(mcs_map_resp, nss);
        if ((mcs_user == NO_NSS_SUPPORT) || (mcs_resp == NO_NSS_SUPPORT))
        {
            SET_VHTNSSMCS(mcs_map_result, nss, no_nss_support);
        }
        else
        {
            SET_VHTNSSMCS(mcs_map_result, nss, MIN(mcs_user, mcs_resp));
        }
    }

    /* tx MCS map */
    pvht_cap->vht_cap.mcs_sets.tx_mcs_map = wlan_cpu_to_le16(mcs_map_result);
    /* tx highest rate */
    pvht_cap->vht_cap.mcs_sets.tx_max_rate = wlan_convert_mcsmap_to_maxrate(priv, (t_u8)bands, (t_u16)mcs_map_result);
    pvht_cap->vht_cap.mcs_sets.tx_max_rate = wlan_cpu_to_le16(pvht_cap->vht_cap.mcs_sets.tx_max_rate);

    LEAVE();
    return;
}

#if 0
/**
 *  @brief This function fills the VHT cap tlv out put format is CPU
 *
 *  @param priv         A pointer to mlan_private structure
 *  @param pvht_cap      A pointer to MrvlIETypes_HTCap_t structure
 *  @param bands        Band configuration
 *
 *  @return             N/A
 */
void wlan_fill_vht_cap_ie(mlan_private *priv,
        IEEEtypes_VHTCap_t *pvht_cap, t_u16 bands)
{
    ENTER();

    pvht_cap->ieee_hdr.element_id = VHT_CAPABILITY;
    pvht_cap->ieee_hdr.len = sizeof(VHT_capa_t);

    /* Fill VHT cap info */
    wlan_fill_cap_info(priv, &pvht_cap->vht_cap, bands);

    /* rx MCS map */
    pvht_cap->vht_cap.mcs_sets.rx_mcs_map =
        GET_DEVRXMCSMAP(priv->usr_dot_11ac_mcs_support);

    /* rx highest rate */
    pvht_cap->vht_cap.mcs_sets.rx_max_rate =
        wlan_convert_mcsmap_to_maxrate(priv, bands, pvht_cap->vht_cap.mcs_sets.rx_mcs_map);

    /* tx MCS map */
    pvht_cap->vht_cap.mcs_sets.tx_mcs_map = GET_DEVTXMCSMAP(priv->usr_dot_11ac_mcs_support);
    /* tx highest rate */
    pvht_cap->vht_cap.mcs_sets.tx_max_rate =
        wlan_convert_mcsmap_to_maxrate(priv, bands,pvht_cap->vht_cap.mcs_sets.tx_mcs_map);

    LEAVE();
    return;
}

#endif

/**
 *  @brief This function append the 802_11AC tlv
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param pbss_desc    A pointer to BSSDescriptor_t structure
 *  @param ppbuffer     A Pointer to command buffer pointer
 *
 *  @return bytes added to the buffer
 */
int wlan_cmd_append_11ac_tlv(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc, t_u8 **ppbuffer)
{
    pmlan_adapter pmadapter = pmpriv->adapter;
    MrvlIETypes_VHTCap_t *pvht_cap;
    MrvlIETypes_OperModeNtf_t *pmrvl_oper_mode;
    t_u16 mcs_map_user = 0;
    t_u16 nss;
    int ret_len = 0;

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
    /* VHT Capabilities IE */
    if (pbss_desc->pvht_cap != MNULL && wlan_get_nss_vht_mcs(pbss_desc->pvht_cap->vht_cap.mcs_sets.rx_mcs_map) != 0U)
    {
        pvht_cap = (MrvlIETypes_VHTCap_t *)(void *)*ppbuffer;
        (void)__memset(pmadapter, pvht_cap, 0, sizeof(MrvlIETypes_VHTCap_t));
        pvht_cap->header.type = wlan_cpu_to_le16(VHT_CAPABILITY);
        pvht_cap->header.len  = (t_u16)sizeof(VHT_capa_t);
        (void)__memcpy(pmadapter, (t_u8 *)pvht_cap + sizeof(MrvlIEtypesHeader_t),
                       (t_u8 *)pbss_desc->pvht_cap + sizeof(IEEEtypes_Header_t), pvht_cap->header.len);

        wlan_fill_vht_cap_tlv(pmpriv, pvht_cap, pbss_desc->bss_band, (t_u8)MTRUE);
        if (wlan_use_non_default_ht_vht_cap(pbss_desc))
        {
            /* Indicate 3 STS in VHT cap info */
            pvht_cap->vht_cap.vht_cap_info = ((pvht_cap->vht_cap.vht_cap_info & (~(0x7 << 13))) | (0x2 << 13));
        }

        HEXDUMP("VHT_CAPABILITIES IE", (t_u8 *)pvht_cap, sizeof(MrvlIETypes_VHTCap_t));
        *ppbuffer += sizeof(MrvlIETypes_VHTCap_t);
        ret_len += (int)sizeof(MrvlIETypes_VHTCap_t);
        pvht_cap->header.len = wlan_cpu_to_le16(pvht_cap->header.len);
    }
    else
    {
        LEAVE();
        return 0;
    }

    /* Operating Mode Notification IE */
    pmrvl_oper_mode = (MrvlIETypes_OperModeNtf_t *)(void *)*ppbuffer;
    (void)__memset(pmadapter, pmrvl_oper_mode, 0, sizeof(MrvlIETypes_OperModeNtf_t));
    pmrvl_oper_mode->header.type = wlan_cpu_to_le16(OPER_MODE_NTF);
    pmrvl_oper_mode->header.len  = (t_u16)sizeof(t_u8);

    if (pmadapter->usr_dot_11ac_opermode_bw != 0U || pmadapter->usr_dot_11ac_opermode_nss != 0U)
    {
        pmrvl_oper_mode->oper_mode |= (pmadapter->usr_dot_11ac_opermode_nss - 1U) << 4U;
        pmrvl_oper_mode->oper_mode |= pmadapter->usr_dot_11ac_opermode_bw - 1U;
        if ((pbss_desc->bss_band & BAND_G) != 0U)
        {
            if (!(IS_OPER_MODE_20M(pmrvl_oper_mode->oper_mode)))
            {
                if ((pbss_desc->pht_cap->ht_cap.ht_cap_info & MBIT(1)) != 0U)
                {
                    SET_OPER_MODE_40M(pmrvl_oper_mode->oper_mode);
                }
                else
                {
                    SET_OPER_MODE_20M(pmrvl_oper_mode->oper_mode);
                }
            }
        }
    }
    else
    {
        /** set default bandwidth:80M*/
        SET_OPER_MODE_80M(pmrvl_oper_mode->oper_mode);

        mcs_map_user = (t_u16)GET_DEVRXMCSMAP(pmadapter->usr_dot_11ac_mcs_support);
        nss          = wlan_get_nss_num_vht_mcs(mcs_map_user);
        pmrvl_oper_mode->oper_mode |= (t_u8)(nss - 1U) << 4U;

        switch (pbss_desc->curr_bandwidth)
        {
            case BW_20MHZ:
                SET_OPER_MODE_20M(pmrvl_oper_mode->oper_mode);
                break;
            case BW_40MHZ:
                SET_OPER_MODE_40M(pmrvl_oper_mode->oper_mode);
                break;
            case BW_80MHZ:
            default:
                PRINTM(MINFO, "Unexpected bandwidth.\n");
                break;
        }
    }
    HEXDUMP("OPER MODE NTF IE", (t_u8 *)pmrvl_oper_mode, sizeof(MrvlIETypes_OperModeNtf_t));
    *ppbuffer += sizeof(MrvlIETypes_OperModeNtf_t);
    ret_len += (int)sizeof(MrvlIETypes_OperModeNtf_t);
    pmrvl_oper_mode->header.len = wlan_cpu_to_le16(pmrvl_oper_mode->header.len);

    LEAVE();
    return ret_len;
}

#if 0
/**
 *  @brief 11ac configuration handler
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param pioctl_req   A pointer to ioctl request buffer
 *
 *  @return     MLAN_STATUS_SUCCESS --success, otherwise fail
 */
mlan_status wlan_11ac_cfg_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req)
{
    mlan_status status = MLAN_STATUS_SUCCESS;
    mlan_ds_11ac_cfg *cfg = MNULL;

    ENTER();

    if (pioctl_req->buf_len < sizeof(mlan_ds_11ac_cfg)) {
        PRINTM(MINFO, "MLAN bss IOCTL length is too short.\n");
        pioctl_req->data_read_written = 0;
        pioctl_req->buf_len_needed = sizeof(mlan_ds_11ac_cfg);
        pioctl_req->status_code = MLAN_ERROR_INVALID_PARAMETER;
        LEAVE();
        return MLAN_STATUS_RESOURCE;
    }
    cfg = (mlan_ds_11ac_cfg *)pioctl_req->pbuf;
    switch (cfg->sub_command) {
        case MLAN_OID_11AC_VHT_CFG:
            status = wlan_11ac_ioctl_vhtcfg(pmadapter, pioctl_req);
            break;
        case MLAN_OID_11AC_CFG_SUPPORTED_MCS_SET:
            status = wlan_11ac_ioctl_supported_mcs_set(pmadapter, pioctl_req);
            break;
        case MLAN_OID_11AC_OPERMODE_CFG:
            status = wlan_11ac_ioctl_opermodecfg(pmadapter, pioctl_req);
            break;
        default:
            pioctl_req->status_code = MLAN_ERROR_IOCTL_INVALID;
            status = MLAN_STATUS_FAILURE;
            break;
    }
    LEAVE();
    return status;
}
#endif

/**
 *  @brief This function prepares 11ac cfg command
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param cmd      A pointer to HostCmd_DS_COMMAND structure
 *  @param cmd_action   the action: GET or SET
 *  @param pdata_buf    A pointer to data buffer
 *  @return         MLAN_STATUS_SUCCESS
 */
mlan_status wlan_cmd_11ac_cfg(IN pmlan_private pmpriv,
                              IN HostCmd_DS_COMMAND *cmd,
                              IN t_u16 cmd_action,
                              IN t_void *pdata_buf)
{
    HostCmd_DS_11AC_CFG *vhtcfg   = &cmd->params.vhtcfg;
    mlan_ds_11ac_vht_cfg *vht_cfg = (mlan_ds_11ac_vht_cfg *)pdata_buf;

    ENTER();
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_11AC_CFG);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_11AC_CFG) + S_DS_GEN);
    if (pmpriv->bss_type == MLAN_BSS_TYPE_UAP)
    {
        cmd->seq_num = (t_u16)(0x01U) << 12;
    }
    vhtcfg->action      = wlan_cpu_to_le16(cmd_action);
    vhtcfg->band_config = (t_u8)(vht_cfg->band & 0xFFU);
    vhtcfg->misc_config = (t_u8)(vht_cfg->txrx & 0x3U);
    if (vhtcfg->misc_config != 2U)
    {
        vhtcfg->misc_config |= (t_u8)(vht_cfg->bwcfg << 2);
    }

    vhtcfg->vht_cap_info = wlan_cpu_to_le32(vht_cfg->vht_cap_info);
    vht_cfg->vht_rx_mcs  = wlan_cpu_to_le32(vht_cfg->vht_rx_mcs);
    (void)__memcpy(pmpriv->adapter, &vhtcfg->vht_supp_mcs_set[0], &vht_cfg->vht_rx_mcs, sizeof(t_u32));
    vht_cfg->vht_tx_mcs = wlan_cpu_to_le32(vht_cfg->vht_tx_mcs);
    (void)__memcpy(pmpriv->adapter, &vhtcfg->vht_supp_mcs_set[4], &vht_cfg->vht_tx_mcs, sizeof(t_u32));
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}

#if 0
/**
 *  @brief This function handles the command response of 11ac cfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param resp         A pointer to HostCmd_DS_COMMAND
 *  @param pioctl_buf   A pointer to mlan_ioctl_req structure
 *
 *  @return        MLAN_STATUS_SUCCESS
 */
mlan_status wlan_ret_11ac_cfg(
    IN  pmlan_private        pmpriv,
    IN  HostCmd_DS_COMMAND   *resp,
    IN  mlan_ioctl_req       *pioctl_buf)
{
    pmlan_adapter        pmadapter = pmpriv->adapter;
    mlan_ds_11ac_cfg    *cfg = MNULL;
    HostCmd_DS_11AC_CFG *vhtcfg = &resp->params.vhtcfg;

    ENTER();
    if (pioctl_buf && (wlan_le16_to_cpu(vhtcfg->action) == HostCmd_ACT_GEN_GET)) {
        cfg = (mlan_ds_11ac_cfg *)pioctl_buf->pbuf;
        cfg->param.vht_cfg.band = vhtcfg->band_config;
        cfg->param.vht_cfg.txrx = vhtcfg->misc_config & 0x03;
        if (cfg->param.vht_cfg.txrx & 0x1)
            cfg->param.vht_cfg.bwcfg = (vhtcfg->misc_config & 0x04) >> 2;
        else
            cfg->param.vht_cfg.bwcfg = 0;

        cfg->param.vht_cfg.vht_cap_info = wlan_le32_to_cpu(vhtcfg->vht_cap_info);
        (void)__memcpy(pmadapter, &cfg->param.vht_cfg.vht_rx_mcs,
                          &vhtcfg->vht_supp_mcs_set[0], sizeof(t_u32));
        cfg->param.vht_cfg.vht_rx_mcs =
            wlan_le32_to_cpu(cfg->param.vht_cfg.vht_rx_mcs);
        (void)__memcpy(pmadapter, &cfg->param.vht_cfg.vht_tx_mcs,
                          &vhtcfg->vht_supp_mcs_set[4], sizeof(t_u32));
        cfg->param.vht_cfg.vht_tx_mcs =
            wlan_le32_to_cpu(cfg->param.vht_cfg.vht_tx_mcs);
        cfg->param.vht_cfg.vht_rx_max_rate =
			wlan_convert_mcsmap_to_maxrate(pmpriv,cfg->param.vht_cfg.band,cfg->param.vht_cfg.vht_rx_mcs);
        cfg->param.vht_cfg.vht_tx_max_rate =
			wlan_convert_mcsmap_to_maxrate(pmpriv, cfg->param.vht_cfg.band,cfg->param.vht_cfg.vht_tx_mcs);
    }
    LEAVE();
    return MLAN_STATUS_SUCCESS;
}


void wlan_update_11ac_cap(mlan_private * pmpriv){

    mlan_adapter* pmadapter = pmpriv->adapter;

    pmpriv->usr_dot_11ac_mcs_support = pmadapter->hw_dot_11ac_mcs_support;
    pmpriv->usr_dot_11ac_dev_cap_bg =
		pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#if CONFIG_5GHz_SUPPORT
    pmpriv->usr_dot_11ac_dev_cap_a =
		pmadapter->hw_dot_11ac_dev_cap & ~DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK;
#endif
    pmpriv->usr_dot_11ac_bw = BW_FOLLOW_VHTCAP;
}
#endif

/**
 *  @brief This function check if 11AC is allowed in bandcfg
 *
 *  @param pmpriv       A pointer to mlan_private structure
 *  @param bss_band     bss band
 *
 *  @return 0--not allowed, other value allowed
 */
t_u8 wlan_11ac_bandconfig_allowed(mlan_private *pmpriv, t_u16 bss_band)
{
    if (pmpriv->bss_mode == MLAN_BSS_MODE_IBSS)
    {
        if ((bss_band & BAND_G) != 0U)
        {
            return (t_u8)(pmpriv->adapter->adhoc_start_band & BAND_GAC);
        }
#if CONFIG_5GHz_SUPPORT
        else if ((bss_band & BAND_A) != 0U)
        {
            return (t_u8)(pmpriv->adapter->adhoc_start_band & BAND_AAC);
        }
        else
        { /* Do Nothing */
        }
#endif
    }
    else
    {
        if ((bss_band & BAND_G) != 0U)
        {
            return (t_u8)(pmpriv->config_bands & BAND_GAC);
        }
#if CONFIG_5GHz_SUPPORT
        else if ((bss_band & BAND_A) != 0U)
        {
            return (t_u8)(pmpriv->config_bands & BAND_AAC);
        }
        else
        { /* Do Nothing */
        }
#endif
    }
    return 0;
}
