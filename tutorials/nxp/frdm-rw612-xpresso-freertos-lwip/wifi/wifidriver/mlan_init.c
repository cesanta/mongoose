/** @file mlan_init.c
 *
 *  @brief  This file provides initialization for FW and HW
 *
 *  Copyright 2008-2021, 2023 NXP
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
#include "fsl_common.h"
#ifndef __ZEPHYR__
#ifndef RW610
#include "sdmmc_config.h"
#endif
#endif

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
        Global Variables
********************************************************/

#ifdef __ZEPHYR__
#define BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE 32
#endif

//_IOBUFS_ALIGNED(SDIO_DMA_ALIGNMENT)
#if defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || defined(SD9098) || defined(SD9177) || defined(IW610)
static t_u8 mp_regs_buffer[MAX_MP_REGS + DMA_ALIGNMENT];
#elif defined(SD8801)
SDK_ALIGN(uint8_t mp_regs_buffer[MAX_MP_REGS], BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#endif

/* We are allocating BSS list globally as we need heap for other purposes */
SDK_ALIGN(BSSDescriptor_t BSS_List[MRVDRV_MAX_BSSID_LIST], 32);

#if CONFIG_SCAN_CHANNEL_GAP

#if !CONFIG_5GHz_SUPPORT
static ChanStatistics_t Chan_Stats[14];
#else
static ChanStatistics_t Chan_Stats[48];
#endif

#endif

/********************************************************
        Local Functions
********************************************************/

/********************************************************
        Global Functions
********************************************************/

/**
 *  @brief This function allocates buffer for the members of adapter
 *  		structure like command buffer and BSSID list.
 *
 *  @param pmadapter A pointer to mlan_adapter structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_allocate_adapter(pmlan_adapter pmadapter)
{
    (void)__memset(MNULL, &BSS_List, 0x00, sizeof(BSS_List));

    pmadapter->pscan_table = BSS_List;
#if CONFIG_SCAN_CHANNEL_GAP

#if !CONFIG_5GHz_SUPPORT
    pmadapter->num_in_chan_stats = 14;
#else
    pmadapter->num_in_chan_stats = 48;
#endif
    pmadapter->pchan_stats = Chan_Stats;
#endif

       /* wmsdk: Use a statically allocated DMA aligned buffer */
#if defined(SD8801)
    pmadapter->mp_regs = mp_regs_buffer;
#elif defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || defined(SD9098) || defined(SD9177) || defined(IW610)
    pmadapter->mp_regs = (t_u8 *)ALIGN_ADDR(mp_regs_buffer, DMA_ALIGNMENT);
// mp_regs_buffer;
#endif

    return MLAN_STATUS_SUCCESS;
}

void wlan_clear_scan_bss(void)
{
#if CONFIG_WPA_SUPP
    BSSDescriptor_t *bss_entry = NULL;
    int i;

    for (i = 0; i < mlan_adap->num_in_scan_table; i++)
    {
        bss_entry = &mlan_adap->pscan_table[i];
        if (bss_entry && bss_entry->ies != NULL)
        {
            OSA_MemoryFree(bss_entry->ies);
        }
    }
#endif
    (void)__memset(MNULL, &BSS_List, 0x00, sizeof(BSS_List));
}

/**
 *  @brief This function initializes the private structure
 *  		and sets default values to the members of mlan_private.
 *
 *  @param priv    A pointer to mlan_private structure
 *
 *  @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_init_priv(pmlan_private priv)
{
    t_u32 i;
    /* pmlan_adapter pmadapter = priv->adapter; */
    mlan_status ret = MLAN_STATUS_SUCCESS;

    ENTER();

    priv->media_connected = MFALSE;
    (void)__memset(pmadapter, priv->curr_addr, 0xff, MLAN_MAC_ADDR_LENGTH);

#ifdef STA_SUPPORT
    priv->pkt_tx_ctrl = 0;
    priv->bss_mode    = MLAN_BSS_MODE_INFRA;

    priv->data_rate         = 0; /* Initially indicate the rate as auto */
    priv->data_rate_index   = (t_s8)-1;
    priv->is_data_rate_auto = MTRUE;
    priv->bcn_avg_factor    = DEFAULT_BCN_AVG_FACTOR;
    priv->data_avg_factor   = DEFAULT_DATA_AVG_FACTOR;

    priv->sec_info.wep_status          = Wlan802_11WEPDisabled;
    priv->sec_info.authentication_mode = MLAN_AUTH_MODE_AUTO;
    priv->sec_info.encryption_mode     = MLAN_ENCRYPTION_MODE_NONE;
    for (i = 0; i < sizeof(priv->wep_key) / sizeof(priv->wep_key[0]); i++)
    {
        (void)__memset(pmadapter, &priv->wep_key[i], 0, sizeof(mrvl_wep_key_t));
    }
    priv->wep_key_curr_index = 0;
    priv->ewpa_query         = MFALSE;
    priv->adhoc_aes_enabled  = MFALSE;
    priv->curr_pkt_filter =
#if CONFIG_11AC
        HostCmd_ACT_MAC_STATIC_DYNAMIC_BW_ENABLE |
#endif
        HostCmd_ACT_MAC_RTS_CTS_ENABLE | HostCmd_ACT_MAC_RX_ON | HostCmd_ACT_MAC_TX_ON |
        HostCmd_ACT_MAC_ETHERNETII_ENABLE;

#if CONFIG_GTK_REKEY_OFFLOAD
    (void)__memset(pmadapter, &priv->gtk_rekey, 0, sizeof(priv->gtk_rekey));
#endif
    (void)__memset(pmadapter, &priv->curr_bss_params, 0, sizeof(priv->curr_bss_params));
    priv->listen_interval = MLAN_DEFAULT_LISTEN_INTERVAL;
    wlan_11d_priv_init(priv);
    wlan_11h_priv_init(priv);

#if UAP_SUPPORT
    priv->uap_bss_started = MFALSE;
    (void)__memset(pmadapter, &priv->uap_state_chan_cb, 0, sizeof(priv->uap_state_chan_cb));
    priv->num_drop_pkts = 0;
#endif

    priv->wpa_is_gtk_set = MFALSE;
#endif /* STA_SUPPORT */

#ifdef RW610
    priv->tx_bf_cap = DEFAULT_11N_TX_BF_CAP;
#else
    priv->tx_bf_cap = 0;
#endif
    priv->wmm_required = MTRUE;
    priv->wmm_enabled  = MFALSE;
    priv->wmm_qosinfo  = 0;
#ifdef STA_SUPPORT
#endif /* STA_SUPPORT */
    priv->pmfcfg.mfpc = 0;
    priv->pmfcfg.mfpr = 0;

#if CONFIG_11K
    priv->enable_host_11k = (t_u8)MFALSE;
#endif
#if CONFIG_11K
    priv->neighbor_rep_token    = (t_u8)1U;
    priv->rrm_mgmt_bitmap_index = -1;
#endif
#if CONFIG_11V
    priv->bss_trans_query_token = (t_u8)1U;
#endif
    for (i = 0; i < MAX_NUM_TID; i++)
    {
        priv->addba_reject[i] = ADDBA_RSP_STATUS_ACCEPT;
    }
    priv->max_amsdu = 0;

    priv->scan_block = MFALSE;

    if (GET_BSS_ROLE(priv) == (unsigned)MLAN_BSS_ROLE_STA)
    {
        priv->port_ctrl_mode = MTRUE;
    }
    else
    {
        priv->port_ctrl_mode = MFALSE;
    }
    priv->port_open = MFALSE;
#if CONFIG_ROAMING
    priv->roaming_enabled = MFALSE;
#endif

#if UAP_SUPPORT
    priv->uap_bss_started = MFALSE;
    priv->uap_host_based  = MFALSE;
#endif

    reset_ie_index();
#if CONFIG_WPA_SUPP
    priv->default_scan_ies_len = 0;
    priv->probe_req_index      = -1;
#if CONFIG_WPA_SUPP_WPS
    priv->wps.wps_mgmt_bitmap_index = -1;
#endif
#if CONFIG_HOSTAPD
    priv->beacon_vendor_index = -1;
    priv->beacon_index        = 0;
    priv->proberesp_index     = 1;
    priv->assocresp_index     = 2;
    priv->beacon_wps_index    = 3;
#endif
#endif
#if CONFIG_TCP_ACK_ENH
    priv->enable_tcp_ack_enh = MTRUE;
#endif

#if CONFIG_WPA_SUPP_DPP
    priv->is_dpp_connect = MFALSE;
#endif

    LEAVE();
    return ret;
}

/**
 *  @brief This function initializes the adapter structure
 *  		and sets default values to the members of adapter.
 *
 *  @param pmadapter	A pointer to mlan_adapter structure
 *
 *  @return		N/A
 */
t_void wlan_init_adapter(pmlan_adapter pmadapter)
{
    pmadapter->mp_rd_bitmap = 0;

    /*
     * wmsdk: fixme: Originally mp_wr_bitmap was set to 0. We have changed
     * it only for legacy reason. Please check it and restore on high
     * priority.
     */
    pmadapter->mp_wr_bitmap = 0;
#ifndef RW610
#if defined(SD8801)
    pmadapter->curr_rd_port = 1;
    pmadapter->curr_wr_port = 1;
#elif defined(SD8978) || defined(SD8987) || defined(SD8997) || defined(SD9097) || defined(SD9098) || defined(SD9177) || defined(IW610)
    pmadapter->curr_rd_port      = 0;
    pmadapter->curr_wr_port      = 0;
#endif
    pmadapter->mp_data_port_mask = DATA_PORT_MASK;
#endif

#ifdef STA_SUPPORT
    /* Scan type */
    pmadapter->scan_type = MLAN_SCAN_TYPE_ACTIVE;
    /* Scan mode */
    pmadapter->scan_mode = HostCmd_BSS_MODE_ANY;
    /* Scan time */
    pmadapter->specific_scan_time = MRVDRV_SPECIFIC_SCAN_CHAN_TIME;
    pmadapter->active_scan_time   = MRVDRV_ACTIVE_SCAN_CHAN_TIME;
    pmadapter->passive_scan_time  = MRVDRV_PASSIVE_SCAN_CHAN_TIME;

    pmadapter->num_in_scan_table = 0;

    pmadapter->ecsa_enable = MFALSE;

    /* fixme: enable this later when required */
#if CONFIG_EXT_SCAN_SUPPORT
    pmadapter->ext_scan = 1;
#endif
    pmadapter->scan_probes = DEFAULT_PROBES;

#if CONFIG_SCAN_WITH_RSSIFILTER
    pmadapter->rssi_threshold = 0;
#endif

    /* fixme: enable this later when required */
    pmadapter->multiple_dtim         = MRVDRV_DEFAULT_MULTIPLE_DTIM;
    pmadapter->local_listen_interval = 0; /* default value in firmware
                                             will be used */
#endif                                    /* STA_SUPPORT */

    pmadapter->delay_to_ps       = DELAY_TO_PS_DEFAULT;
    pmadapter->idle_time         = DEEP_SLEEP_IDLE_TIME;
    pmadapter->enhanced_ps_mode  = PS_MODE_AUTO;
    pmadapter->bcn_miss_time_out = DEFAULT_BCN_MISS_TIMEOUT;

#if CONFIG_WMM_UAPSD
    pmadapter->gen_null_pkt   = MFALSE; /* Disable NULL Pkt generation-default */
    pmadapter->pps_uapsd_mode = MFALSE; /* Disable pps/uapsd mode -default */
#endif
#if CONFIG_HOST_SLEEP
    pmadapter->is_hs_configured          = MFALSE;
    pmadapter->mgmt_filter[0].action     = 0;      /* discard and not wakeup host */
    pmadapter->mgmt_filter[0].type       = 0xff;   /* management frames */
    pmadapter->mgmt_filter[0].frame_mask = 0x1400; /* Frame-Mask bits :
                                                      : Bit 0 - Association Request
                                                      : Bit 1 - Association Response
                                                      : Bit 2 - Re-Association Request
                                                      : Bit 3 - Re-Association Response
                                                      : Bit 4 - Probe Request
                                                      : Bit 5 - Probe Response
                                                      : Bit 8 - Beacon Frames
                                                      : Bit 10 - Disassociation
                                                      : Bit 11 - Authentication
                                                      : Bit 12 - Deauthentication
                                                      : Bit 13 - Action Frames
                                                   */
#endif

    pmadapter->hw_dot_11n_dev_cap     = 0;
    pmadapter->hw_dev_mcs_support     = 0;
    pmadapter->usr_dot_11n_dev_cap_bg = 0;
    pmadapter->usr_dot_11n_dev_cap_a  = 0;
    pmadapter->usr_dev_mcs_support    = 0;
#ifdef STA_SUPPORT
    pmadapter->chan_bandwidth    = 0;
    pmadapter->adhoc_11n_enabled = MFALSE;
#endif /* STA_SUPPORT */

    pmadapter->hw_dot_11ac_dev_cap       = 0;
    pmadapter->hw_dot_11ac_mcs_support   = 0;
    pmadapter->usr_dot_11ac_opermode_bw  = 0;
    pmadapter->usr_dot_11ac_opermode_nss = 0;
#if CONFIG_WIFI_CAPA
    pmadapter->usr_dot_11n_enable = MFALSE;
#if CONFIG_11AC
    pmadapter->usr_dot_11ac_enable = MFALSE;
#endif
#if CONFIG_11AX
    pmadapter->usr_dot_11ax_enable = MFALSE;
#endif
#endif

    /* Initialize 802.11d */
    wlan_11d_init(pmadapter);
    wlan_11h_init(pmadapter);

    wlan_wmm_init(pmadapter);
    wlan_init_wmm_param(pmadapter);
#if CONFIG_WMM_UAPSD
    (void)__memset(pmadapter, &pmadapter->sleep_params, 0, sizeof(pmadapter->sleep_params));
    (void)__memset(pmadapter, &pmadapter->sleep_period, 0, sizeof(pmadapter->sleep_period));

    pmadapter->tx_lock_flag = MFALSE;
#endif /* CONFIG_WMM_UAPSD */
    pmadapter->null_pkt_interval = 0;
    pmadapter->fw_bands          = 0U;
    pmadapter->config_bands      = 0U;
    pmadapter->adhoc_start_band  = 0U;
    /* pmadapter->pscan_channels = MNULL; */
    pmadapter->fw_release_number = 0;
    pmadapter->fw_cap_info       = 0;
    (void)__memset(pmadapter, &pmadapter->region_channel, 0, sizeof(pmadapter->region_channel));
    pmadapter->region_code = MRVDRV_DEFAULT_REGION_CODE;
    (void)__memcpy(pmadapter, pmadapter->country_code, MRVDRV_DEFAULT_COUNTRY_CODE, COUNTRY_CODE_LEN);
    pmadapter->adhoc_awake_period = 0;
    pmadapter->ps_state           = PS_STATE_AWAKE;
    return;
}

/**
 *  @brief This function intializes the lock variables and
 *  the list heads.
 *
 *  @param pmadapter  A pointer to a mlan_adapter structure
 *
 *  @return           MLAN_STATUS_SUCCESS -- on success,
 *                    otherwise MLAN_STATUS_FAILURE
 *
 */
mlan_status wlan_init_lock_list(IN pmlan_adapter pmadapter)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_private priv = MNULL;
    /* pmlan_callbacks pcb = &pmadapter->callbacks; */
    t_u8 i = 0;
    t_u8 j = 0;

    ENTER();
    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i] != NULL)
        {
            priv = pmadapter->priv[i];
            for (j = 0; j < MAX_NUM_TID; ++j)
            {
                util_init_list_head((t_void *)pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[j].ra_list, MTRUE,
                                    priv->adapter->callbacks.moal_init_lock);
            }

#if CONFIG_WMM
            /* wmm enhanced reuses 4 ac xmit queues */
            for (j = 0; j < MAX_AC_QUEUES; ++j)
            {
                if (priv->adapter->callbacks.moal_init_semaphore(pmadapter->pmoal_handle, "ra_list_sem",
                                                                 &priv->wmm.tid_tbl_ptr[j].ra_list.plock) !=
                    MLAN_STATUS_SUCCESS)
                {
                    wifi_e("Create ra_list_sem failed");
                    ret = MLAN_STATUS_FAILURE;
                    goto done;
                }
#if CONFIG_WMM_DEBUG
                util_init_list_head((t_void *)pmadapter->pmoal_handle, &priv->wmm.hist_ra[j], MFALSE, MNULL);
#endif
            }
#endif

            ret = (mlan_status)OSA_MutexCreate((osa_mutex_handle_t)priv->tx_ba_stream_tbl_lock);
            if (ret != MLAN_STATUS_SUCCESS)
            {
                wifi_e("Create Tx BA tbl sem failed");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }

            util_init_list_head((t_void *)pmadapter->pmoal_handle, &priv->tx_ba_stream_tbl_ptr, MTRUE,
                                pmadapter->callbacks.moal_init_lock);

            ret = (mlan_status)OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)priv->rx_reorder_tbl_lock);
            if (ret != MLAN_STATUS_SUCCESS)
            {
                wifi_e("Create Rx Reorder tbl lock failed");
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
            OSA_SemaphorePost((osa_semaphore_handle_t)priv->rx_reorder_tbl_lock);
            util_init_list_head((t_void *)pmadapter->pmoal_handle, &priv->rx_reorder_tbl_ptr, MTRUE,
                                pmadapter->callbacks.moal_init_lock);

            util_scalar_init((t_void *)pmadapter->pmoal_handle, &priv->wmm.tx_pkts_queued, 0,
                             priv->wmm.ra_list_spinlock, pmadapter->callbacks.moal_init_lock);
            util_scalar_init((t_void *)pmadapter->pmoal_handle, &priv->wmm.highest_queued_prio, HIGH_PRIO_TID,
                             priv->wmm.ra_list_spinlock, pmadapter->callbacks.moal_init_lock);
            util_init_list_head((t_void *)pmadapter->pmoal_handle, &priv->sta_list, MTRUE,
                                pmadapter->callbacks.moal_init_lock);
        }
    }

done:
    /* error: */
    if (ret != MLAN_STATUS_SUCCESS)
    {
        for (i = 0; i < pmadapter->priv_num; i++)
        {
            priv = pmadapter->priv[i];
#if CONFIG_WMM
            for (j = 0; j < MAX_AC_QUEUES; ++j)
            {
                if ((uint32_t *)(*(uint32_t *)priv->wmm.tid_tbl_ptr[j].ra_list.plock) != NULL)
                    priv->adapter->callbacks.moal_free_semaphore(
                        pmadapter->pmoal_handle, &priv->wmm.tid_tbl_ptr[j].ra_list.plock);
            }
#endif
            if ((uint32_t *)(*(uint32_t *)priv->tx_ba_stream_tbl_lock) != NULL)
                OSA_MutexDestroy((osa_mutex_handle_t)priv->tx_ba_stream_tbl_lock);
            if ((uint32_t *)(*(uint32_t *)priv->rx_reorder_tbl_lock) != NULL)
                OSA_SemaphoreDestroy((osa_semaphore_handle_t)priv->rx_reorder_tbl_lock);
        }
    }
    LEAVE();
    return ret;
}

/**
 *  @brief  This function initializes firmware
 *
 *  @param pmadapter		A pointer to mlan_adapter
 *
 *  @return		MLAN_STATUS_SUCCESS, MLAN_STATUS_PENDING or MLAN_STATUS_FAILURE
 */
mlan_status wlan_init_fw(IN pmlan_adapter pmadapter)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_private priv = MNULL;
    t_u8 i             = 0;

    ENTER();

    /* Initialize adapter structure */
    wlan_init_adapter(pmadapter);

    for (i = 0; i < pmadapter->priv_num; i++)
    {
        if (pmadapter->priv[i] != NULL)
        {
            priv = pmadapter->priv[i];

            /* Initialize private structure */
            if ((ret = wlan_init_priv(priv)) != MLAN_STATUS_SUCCESS)
            {
                ret = MLAN_STATUS_FAILURE;
                goto done;
            }
        }
    }
        /* Issue firmware initialize commands for first BSS, for other
           interfaces it will be called after getting the last init command
           response of previous interface */
done:
    LEAVE();
    return ret;
}

/**
 *  @brief This function frees the structure of adapter
 *
 *  @param pmadapter      A pointer to mlan_adapter structure
 *
 *  @return             N/A
 */
t_void wlan_free_adapter(pmlan_adapter pmadapter)
{
    ENTER();

    if (!pmadapter)
    {
        PRINTM(MERROR, "The adapter is NULL\n");
        LEAVE();
        return;
    }


    LEAVE();
    return;
}

