/** @file mlan_sta_event.c
 *
 *  @brief  This file provides Function for STA event handling
 *
 *  Copyright 2008-2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    10/13/2008: initial version
********************************************************/

#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

/**
 *  @brief This function handles disconnect event, reports disconnect
 *  		to upper layer, cleans tx/rx packets,
 *  		resets link state etc.
 *
 *  @param priv            A pointer to mlan_private structure
 *  @param drv_disconnect  Flag indicating the driver should disconnect
 *                         and flush pending packets.
 *
 *  @return        N/A
 */
t_void wlan_reset_connect_state(pmlan_private priv, t_u8 drv_disconnect)
{
    ENTER();


    PRINTM(MINFO, "Handles disconnect event.\n");

    if (drv_disconnect == MTRUE)
    {
        priv->media_connected = MFALSE;
    }

    if (priv->port_ctrl_mode == MTRUE)
    {
        /* Close the port on Disconnect */
        PRINTM(MINFO, "DISC: port_status = CLOSED\n");
        priv->port_open = MFALSE;
    }
    priv->scan_block = MFALSE;

    /* Reset SNR/NF/RSSI values */
    priv->data_rssi_last = 0;
    priv->data_nf_last   = 0;
    priv->data_rssi_avg  = 0;
    priv->data_nf_avg    = 0;
    priv->bcn_rssi_last  = 0;
    priv->bcn_nf_last    = 0;
    priv->bcn_rssi_avg   = 0;
    priv->bcn_nf_avg     = 0;
    priv->rxpd_rate      = 0;
#ifdef SD8801
    priv->rxpd_htinfo = 0;
#else
    priv->rxpd_rate_info = 0;
#endif
    priv->max_amsdu = 0;

    priv->tx_pause = 0;
#if (CONFIG_WPS2) || (CONFIG_WPA_SUPP_WPS)
    priv->wps.session_enable = MFALSE;
    (void)__memset(priv->adapter, (t_u8 *)&priv->wps.wps_ie, 0x00, sizeof(priv->wps.wps_ie));
#endif /* CONFIG_WPS2 */

    /* Enable auto data rate */
    priv->is_data_rate_auto = MTRUE;
    priv->data_rate         = 0;
#if CONFIG_11K
    priv->neighbor_rep_token = (t_u8)1U;
#endif
#if CONFIG_11V
    priv->bss_trans_query_token = (t_u8)1U;
#endif
    if (priv->bss_mode == MLAN_BSS_MODE_IBSS)
    {
        priv->adhoc_state                        = (t_u8)ADHOC_IDLE;
        priv->adhoc_is_link_sensed               = MFALSE;
        priv->intf_state_11h.adhoc_auto_sel_chan = MTRUE;
    }

#if CONFIG_WMM_UAPSD
    /* Need to put uapsd_sem before getting ra_list.plock in wlan_ralist_del_all_enh */
    if (priv->adapter->pps_uapsd_mode)
    {
        OSA_SemaphorePost((osa_semaphore_handle_t)uapsd_sem);
    }
    priv->adapter->tx_lock_flag   = MFALSE;
    priv->adapter->pps_uapsd_mode = MFALSE;
#endif

#if CONFIG_GTK_REKEY_OFFLOAD
    (void)__memset(pmadapter, &priv->gtk_rekey, 0, sizeof(priv->gtk_rekey));
#endif

    if (drv_disconnect == MTRUE)
    {
        /* Free Tx and Rx packets, report disconnect to upper layer */
        wlan_clean_txrx(priv);

        /* Need to erase the current SSID and BSSID info */
        (void)__memset(priv->adapter, &priv->curr_bss_params, 0x00, sizeof(priv->curr_bss_params));
    }

    LEAVE();
}

/**
 *  @brief This function handles link lost, deauth and
 *  		disassoc events.
 *
 *  @param priv    A pointer to mlan_private structure
 *  @return        N/A
 */
t_void wlan_handle_disconnect_event(pmlan_private pmpriv)
{
    ENTER();

        wlan_reset_connect_state(pmpriv, MTRUE);

    LEAVE();
}

