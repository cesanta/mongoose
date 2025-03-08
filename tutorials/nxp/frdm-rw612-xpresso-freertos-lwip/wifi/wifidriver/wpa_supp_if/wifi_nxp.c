/** @file wifi_nxp.c
 *
 * @brief This file provides Core Wi-Fi definition for wpa supplicant rtos driver.
 *
 * Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdlib.h>
#include <string.h>

#include <osa.h>
#include <wm_net.h>
#include <wifi.h>
#include <wifi-debug.h>

#if CONFIG_WPA_SUPP

#include <wifi_nxp.h>
#include <rtos_wpa_supp_if.h>

#include "supp_main.h"

static t_u8 wifi_supp_init_done;
static struct wifi_nxp_ctx_rtos *g_wifi_if_ctx_rtos = NULL;

int wifi_nxp_set_mac_addr(const t_u8 *mac);

static int wifi_nxp_wpa_supp_set_mac_addr(void *if_priv, const t_u8 *addr)
{
    if (!if_priv || !addr)
    {
        supp_e("%s: Invalid params", __func__);
        return -WM_FAIL;
    }

    return wifi_nxp_set_mac_addr(addr);
}

const rtos_wpa_supp_dev_ops wpa_supp_ops = {
    .init                     = wifi_nxp_wpa_supp_dev_init,
    .deinit                   = wifi_nxp_wpa_supp_dev_deinit,
    .set_mac_addr             = wifi_nxp_wpa_supp_set_mac_addr,
    .scan2                    = wifi_nxp_wpa_supp_scan2,
    .set_default_scan_ies     = wifi_nxp_wpa_supp_set_default_scan_ies,
    .sched_scan               = wifi_nxp_wpa_supp_sched_scan,
    .stop_sched_scan          = wifi_nxp_wpa_supp_stop_sched_scan,
    .scan_abort               = wifi_nxp_wpa_supp_scan_abort,
    .get_scan_results2        = wifi_nxp_wpa_supp_scan_results_get,
    .deauthenticate           = wifi_nxp_wpa_supp_deauthenticate,
    .authenticate             = wifi_nxp_wpa_supp_authenticate,
    .associate                = wifi_nxp_wpa_supp_associate,
    .set_key                  = wifi_nxp_wpa_supp_set_key,
    .del_key                  = wifi_nxp_wpa_supp_del_key,
    .set_rekey_info           = wifi_nxp_wpa_supp_set_rekey_info,
    .set_supp_port            = wifi_nxp_wpa_set_supp_port,
    .set_country              = wifi_nxp_wpa_supp_set_country,
    .get_country              = wifi_nxp_wpa_supp_get_country,
    .signal_poll              = wifi_nxp_wpa_supp_signal_poll,
    .send_mlme                = wifi_nxp_wpa_send_mlme,
    .remain_on_channel        = wifi_nxp_wpa_supp_remain_on_channel,
    .cancel_remain_on_channel = wifi_nxp_wpa_supp_cancel_remain_on_channel,
    .get_survey_results       = wifi_nxp_wpa_supp_survey_results_get,
    .dpp_listen               = wifi_nxp_wpa_dpp_listen,
    .get_modes                = wifi_nxp_wpa_get_modes,
#if CONFIG_HOSTAPD
    .hapd_init                = wifi_nxp_hostapd_dev_init,
    .hapd_deinit              = wifi_nxp_hostapd_dev_deinit,
    .set_modes                = wifi_nxp_hostapd_set_modes,
    .do_acs                   = wifi_nxp_hostapd_do_acs,
    .set_ap                   = wifi_nxp_hostapd_set_ap,
    .sta_add                  = wifi_nxp_hostapd_sta_add,
    .sta_remove               = wifi_nxp_hostapd_sta_remove,
    .hapd_send_eapol          = wifi_nxp_hostapd_send_eapol,
    .set_freq                 = wifi_nxp_hostapd_set_freq,
    .set_rts                  = wifi_nxp_hostapd_set_rts,
    .set_frag                 = wifi_nxp_hostapd_set_frag,
    .stop_ap                  = wifi_nxp_hostapd_stop_ap,
    .set_acl                  = wifi_nxp_hostapd_set_acl,
#endif /* CONFIG_HOSTAPD */
};

static void wifi_nxp_event_proc_scan_start(void *if_ctx)
{
    wifi_nxp_wpa_supp_event_proc_scan_start(if_ctx);
}

static void wifi_nxp_event_proc_scan_abort(void *if_ctx)
{
    wifi_nxp_wpa_supp_event_proc_scan_abort(if_ctx);
}

static void wifi_nxp_event_proc_scan_done(void *if_priv, int external_scan)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (wifi_if_ctx_rtos == NULL)
    {
        wifi_e("%s: wifi_if_ctx_rtos is NULL", __func__);
        return;
    }
    wifi_nxp_wpa_supp_event_proc_scan_done(if_priv, 0, external_scan);
}

static void wifi_nxp_event_reamin_on_channel(void *if_priv, int cancel_channel)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (wifi_if_ctx_rtos == NULL)
    {
        wifi_e("%s: wifi_if_ctx_rtos is NULL", __func__);
        return;
    }
    wifi_nxp_wpa_supp_event_proc_remain_on_channel(if_priv, cancel_channel);
}

static const wifi_nxp_callbk_fns_t supp_callbk_fns = {
    .mac_changed_callbk_fn         = wifi_nxp_wpa_supp_event_proc_mac_changed,
    .chan_list_changed_callbk_fn   = wifi_nxp_wpa_supp_event_proc_chan_list_changed,
    .scan_start_callbk_fn          = wifi_nxp_event_proc_scan_start,
    .scan_done_callbk_fn           = wifi_nxp_event_proc_scan_done,
    .scan_abort_callbk_fn          = wifi_nxp_event_proc_scan_abort,
    .survey_res_callbk_fn          = wifi_nxp_wpa_supp_event_proc_survey_res,
    .auth_resp_callbk_fn           = wifi_nxp_wpa_supp_event_proc_auth_resp,
    .assoc_resp_callbk_fn          = wifi_nxp_wpa_supp_event_proc_assoc_resp,
    .deauth_callbk_fn              = wifi_nxp_wpa_supp_event_proc_deauth,
    .disassoc_callbk_fn            = wifi_nxp_wpa_supp_event_proc_disassoc,
    .acs_channel_sel_callbk_fn     = wifi_nxp_wpa_supp_event_acs_channel_selected,
    .mgmt_tx_status_callbk_fn      = wifi_nxp_wpa_supp_event_mgmt_tx_status,
    .unprot_mlme_mgmt_rx_callbk_fn = wifi_nxp_wpa_supp_event_proc_unprot_mgmt,
    .remain_on_channel_callbk_fn   = wifi_nxp_event_reamin_on_channel,
    .mgmt_rx_callbk_fn             = wifi_nxp_wpa_supp_event_proc_mgmt_rx,
    .eapol_rx_callbk_fn            = wifi_nxp_wpa_supp_event_proc_eapol_rx,
    .signal_change_callbk_fn       = wifi_nxp_wpa_supp_event_signal_change,
    .ecsa_complete_callbk_fn       = wifi_nxp_wpa_supp_event_proc_ecsa_complete,
    .dfs_cac_started_callbk_fn     = wifi_nxp_wpa_supp_event_proc_dfs_cac_started,
    .dfs_cac_finished_callbk_fn    = wifi_nxp_wpa_supp_event_proc_dfs_cac_finished,
};

#ifndef __ZEPHYR__
static int g_net_idx = -1;
#endif

int wifi_supp_init(void)
{
    int ret = -WM_FAIL;
    char sta_iface_name[NETIF_NAMESIZE], uap_iface_name[NETIF_NAMESIZE];
    struct netif *iface = NULL;

    if (wifi_supp_init_done != 0U)
    {
        return WM_SUCCESS;
    }

    wm_wifi.supp_if_callbk_fns = (wifi_nxp_callbk_fns_t *)&supp_callbk_fns;

    g_wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)OSA_MemoryAllocate(sizeof(struct wifi_nxp_ctx_rtos));

    if (!g_wifi_if_ctx_rtos)
    {
        wifi_e("Interface ctx alloc failed.");
        goto out;
    }

    wm_wifi.if_priv = (void *)g_wifi_if_ctx_rtos;

    iface = net_get_sta_interface();

    if (iface == NULL)
    {
        wifi_e("net_get_sta_interface failed. status code %d", ret);
        goto out;
    }

#ifndef __ZEPHYR__
    if (g_net_idx == -1)
    {
        g_net_idx = net_alloc_client_data_id();

        if (g_net_idx == -1)
        {
            wifi_e("net_alloc_client_data_id failed. net idx %d", g_net_idx);
            goto out;
        }
    }

    netif_set_client_data(iface, LWIP_NETIF_CLIENT_DATA_INDEX_MAX, (void *)&wpa_supp_ops);
#endif

    (void)net_get_if_name_netif(sta_iface_name, iface);

#if UAP_SUPPORT
    g_wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)OSA_MemoryAllocate(sizeof(struct wifi_nxp_ctx_rtos));

    if (!g_wifi_if_ctx_rtos)
    {
        wifi_e("Interface ctx alloc failed.");
        goto out;
    }

    wm_wifi.hapd_if_priv = (void *)g_wifi_if_ctx_rtos;

    iface = net_get_uap_interface();

    if (iface == NULL)
    {
        wifi_e("net_get_uap_interface failed. status code %d", ret);
        goto out;
    }

#ifndef __ZEPHYR__
    netif_set_client_data(iface, LWIP_NETIF_CLIENT_DATA_INDEX_MAX, (void *)&wpa_supp_ops);
#endif

    (void)net_get_if_name_netif(uap_iface_name, iface);
#endif

    ret = start_wpa_supplicant(sta_iface_name);

    if (ret != WM_SUCCESS)
    {
        wifi_e("start wpa supplicant failed. status code %d", ret);
        goto out;
    }

    if (ret == WM_SUCCESS)
    {
        wifi_supp_init_done = 1U;
    }

    return WM_SUCCESS;

out:
    wifi_supp_deinit();
    return ret;
}

void wifi_supp_deinit(void)
{
    int ret;

    if (wifi_supp_init_done != 1U)
    {
        return;
    }

    ret = stop_wpa_supplicant();
    if (ret != WM_SUCCESS)
    {
        wifi_e("stop wpa supplicant failed. status code %d", ret);
    }

    if (wm_wifi.if_priv)
    {
        OSA_MemoryFree(wm_wifi.if_priv);
        wm_wifi.if_priv = NULL;
    }

    if (wm_wifi.hapd_if_priv)
    {
        OSA_MemoryFree(wm_wifi.hapd_if_priv);
        wm_wifi.hapd_if_priv = NULL;
    }
    wifi_supp_init_done = 0U;
}

int wifi_nxp_set_mac_addr(const t_u8 *mac)
{
    wlan_set_mac_addr((uint8_t *)mac);
    return WM_SUCCESS;
}
#endif /* CONFIG_WPA_SUPP */
