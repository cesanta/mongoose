/** @file rtos_wpa_supp_if.c
 *
 *  @brief  This file provides RTOS WPA Supplicant interface wifi APIs
 *
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdlib.h>
#include "fsl_os_abstraction.h"
#include "wifi.h"
#include <wm_net.h>

#ifndef __ZEPHYR__
#include "fsl_debug_console.h"
#endif

#if CONFIG_WPA_SUPP

#include "rtos_wpa_supp_if.h"
#include "wifi-internal.h"

#define MAX_MGMT_TX_FRAME_SIZE 1500

static const chan_to_freq_t chan_to_freq[] = {
    {2412, 1, 0},   {2417, 2, 0},   {2422, 3, 0},   {2427, 4, 0},   {2432, 5, 0},   {2437, 6, 0},   {2442, 7, 0},
    {2447, 8, 0},   {2452, 9, 0},   {2457, 10, 0},  {2462, 11, 0},  {2467, 12, 0},  {2472, 13, 0},  {2484, 14, 0},
    {4915, 183, 1}, {4920, 184, 1}, {4925, 185, 1}, {4935, 187, 1}, {4940, 188, 1}, {4945, 189, 1}, {4960, 192, 1},
    {4980, 196, 1}, {5035, 7, 1},   {5040, 8, 1},   {5045, 9, 1},   {5055, 11, 1},  {5060, 12, 1},  {5080, 16, 1},
    {5170, 34, 1},  {5180, 36, 1},  {5190, 38, 1},  {5200, 40, 1},  {5210, 42, 1},  {5220, 44, 1},  {5230, 46, 1},
    {5240, 48, 1},  {5260, 52, 1},  {5280, 56, 1},  {5300, 60, 1},  {5320, 64, 1},  {5500, 100, 1}, {5520, 104, 1},
    {5540, 108, 1}, {5560, 112, 1}, {5580, 116, 1}, {5600, 120, 1}, {5620, 124, 1}, {5640, 128, 1}, {5660, 132, 1},
    {5680, 136, 1}, {5700, 140, 1}, {5720, 144, 1}, {5745, 149, 1}, {5765, 153, 1}, {5785, 157, 1}, {5805, 161, 1},
#if CONFIG_UNII4_BAND_SUPPORT
    {5825, 165, 1}, {5845, 169, 1}, {5865, 173, 1}, {5885, 177, 1},
#else
    {5825, 165, 1},
#endif
};

static unsigned char get_algo_from_auth_type(int wpa_auth_alg)
{
    if (wpa_auth_alg & WPA_AUTH_ALG_OPEN)
    {
        return MLAN_AUTH_MODE_OPEN;
    }
    if (wpa_auth_alg & WPA_AUTH_ALG_SHARED)
    {
        return MLAN_AUTH_MODE_SHARED;
    }
    if (wpa_auth_alg & WPA_AUTH_ALG_LEAP)
    {
        return MLAN_AUTH_MODE_NETWORKEAP;
    }
    if (wpa_auth_alg & WPA_AUTH_ALG_FT)
    {
        return MLAN_AUTH_MODE_FT;
    }
    if (wpa_auth_alg & WPA_AUTH_ALG_SAE)
    {
        return MLAN_AUTH_MODE_SAE;
    }

    return MLAN_AUTH_MODE_AUTO;
}

static unsigned int wpa_alg_to_cipher_suite(enum wpa_alg alg, size_t key_len)
{
    unsigned flags = 0;

    if (alg == WPA_ALG_GCMP)
    {
        flags |= KEY_FLAG_GCMP;
    }
    else if (alg == WPA_ALG_GCMP_256)
    {
        flags |= KEY_FLAG_GCMP_256;
    }

    if (alg == WPA_ALG_CCMP_256)
    {
        flags |= KEY_FLAG_CCMP_256;
    }

    if ((alg == WPA_ALG_BIP_CMAC_128) || (alg == WPA_ALG_BIP_CMAC_256) || (alg == WPA_ALG_BIP_GMAC_128) ||
        (alg == WPA_ALG_BIP_GMAC_256))
    {
        flags |= KEY_FLAG_AES_MCAST_IGTK;

        if (alg == WPA_ALG_BIP_GMAC_128)
        {
            flags |= KEY_FLAG_GMAC_128;
        }
        else if (alg == WPA_ALG_BIP_GMAC_256)
        {
            flags |= KEY_FLAG_GMAC_256;
        }
    }

    return flags;
}

#if 0
static enum chan_width drv2supp_chan_width(int width)
{
	switch (width) {
	case NXP_WIFI_CHAN_WIDTH_20_NOHT:
		return CHAN_WIDTH_20_NOHT;
	case NXP_WIFI_CHAN_WIDTH_20:
		return CHAN_WIDTH_20;
	case NXP_WIFI_CHAN_WIDTH_40:
		return CHAN_WIDTH_40;
	case NXP_WIFI_CHAN_WIDTH_80:
		return CHAN_WIDTH_80;
	case NXP_WIFI_CHAN_WIDTH_80P80:
		return CHAN_WIDTH_80P80;
	case NXP_WIFI_CHAN_WIDTH_160:
		return CHAN_WIDTH_160;
	default:
		break;
	}
	return CHAN_WIDTH_UNKNOWN;
}
#endif

/** Convertion from/to frequency/channel */
/**
 *  @brief Get frequency for channel in given band
 *
 *  @param channel      channel
 *  @param band         band
 *
 *  @return             freq
 */
int channel_to_frequency(t_u16 channel, t_u8 band)
{
    int i = 0;
    for (i = 0; i < (int)ARRAY_SIZE(chan_to_freq); i++)
    {
        if (channel == chan_to_freq[i].channel && band == chan_to_freq[i].band)
        {
            return chan_to_freq[i].freq;
        }
    }
    return 0;
}

t_u16 freq_to_chan(unsigned int freq)
{
    int i = 0;
    for (i = 0; i < (int)ARRAY_SIZE(chan_to_freq); i++)
    {
        if (freq == chan_to_freq[i].freq)
        {
            return chan_to_freq[i].channel;
        }
    }
    return 0;
}

void wifi_nxp_wpa_supp_event_proc_mac_changed(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.mac_changed(wifi_if_ctx_rtos->hapd_drv_if_ctx);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.mac_changed(wifi_if_ctx_rtos->supp_drv_if_ctx);
    }
}

void wifi_nxp_wpa_supp_event_proc_chan_list_changed(void *if_priv, const char *alpha2)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (!if_priv)
    {
        supp_e("%s: Missing interface context", __func__);
        return;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    if (!alpha2)
    {
        supp_e("%s: Missing alpha2 data", __func__);
        return;
    }

    event.channel_list_changed.initiator = REGDOM_SET_BY_USER;
    event.channel_list_changed.type      = REGDOM_TYPE_COUNTRY;
    event.channel_list_changed.alpha2[0] = alpha2[0];
    event.channel_list_changed.alpha2[1] = alpha2[1];
    event.channel_list_changed.alpha2[2] = alpha2[2];

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.chan_list_changed(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.chan_list_changed(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_scan_start(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    struct os_time t;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    os_get_time(&t);

    memcpy(&wifi_if_ctx_rtos->scan_start_tsf_bssid, &wifi_if_ctx_rtos->assoc_bssid, ETH_ALEN);

    wifi_if_ctx_rtos->scan_start_tsf = t.sec * 1000;

    if (wifi_if_ctx_rtos->supp_callbk_fns.scan_start)
    {
        wifi_if_ctx_rtos->supp_callbk_fns.scan_start(wifi_if_ctx_rtos->supp_drv_if_ctx);
    }
}

void wifi_nxp_wpa_supp_event_proc_scan_abort(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_if_ctx_rtos->scan_in_progress = false;

    wifi_if_ctx_rtos->supp_callbk_fns.scan_abort(wifi_if_ctx_rtos->supp_drv_if_ctx);
}

void wifi_nxp_wpa_supp_event_signal_change(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    struct wpa_signal_info *signal_change = NULL;
    wifi_rssi_info_t rssi_info;

    memset(&event, 0, sizeof(event));
    signal_change                = &event.signal_change;

    (void)wifi_send_rssi_info_cmd(&rssi_info);

    signal_change->above_threshold   = rssi_info.data_rssi_avg;
    signal_change->current_txrate    = rssi_info.bcn_rssi_avg;
    signal_change->current_signal    = rssi_info.bcn_rssi_last;
    signal_change->current_noise     = rssi_info.bcn_nf_last;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_if_ctx_rtos->supp_callbk_fns.signal_change(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
}

void wifi_nxp_wpa_supp_event_proc_scan_done(void *if_priv, int aborted, int external_scan)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    struct scan_info *info = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    info                = &event.scan_info;
    info->aborted       = aborted;
    info->external_scan = external_scan;
    info->nl_scan_event = 1;

    memcpy(&info->scan_start_tsf, &wifi_if_ctx_rtos->scan_start_tsf, sizeof(info->scan_start_tsf));

    memcpy(&info->scan_start_tsf_bssid, &wifi_if_ctx_rtos->scan_start_tsf_bssid, ETH_ALEN);

    wifi_if_ctx_rtos->scan_in_progress = false;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.scan_done(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.scan_done(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_survey_res(void *if_priv,
                                             nxp_wifi_event_new_survey_result_t *survey_res,
                                             unsigned int event_len,
                                             bool more_res)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    struct freq_survey *survey                 = NULL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (survey_res == NULL)
    {
#if CONFIG_HOSTAPD
        if (wifi_if_ctx_rtos->hostapd)
        {
            wifi_if_ctx_rtos->hostapd_callbk_fns.survey_res(wifi_if_ctx_rtos->hapd_drv_if_ctx, survey, more_res);
        }
        else
#endif
        {
            wifi_if_ctx_rtos->supp_callbk_fns.survey_res(wifi_if_ctx_rtos->supp_drv_if_ctx, survey, more_res);
        }
        return;
    }

    survey = (struct freq_survey *)OSA_MemoryAllocate(sizeof(*survey));

    if (!survey)
    {
        supp_e("%s: Unable to calloc memory for survey result\n", __func__);
        return;
    }

    survey->freq = survey_res->freq;

    if (survey_res->nf)
    {
        survey->nf = survey_res->nf;
        survey->filled |= SURVEY_HAS_NF;
    }

    if (survey_res->channel_time)
    {
        (void)memcpy((void *)&survey->channel_time, (const void *)&survey_res->channel_time,
                     sizeof(survey->channel_time));
        survey->filled |= SURVEY_HAS_CHAN_TIME;
    }

    if (survey_res->channel_time_busy)
    {
        (void)memcpy((void *)&survey->channel_time_busy, (const void *)&survey_res->channel_time_busy,
                     sizeof(survey->channel_time_busy));
        survey->filled |= SURVEY_HAS_CHAN_TIME_BUSY;
    }

    if (survey_res->channel_time_rx)
    {
        (void)memcpy((void *)&survey->channel_time_rx, (const void *)&survey_res->channel_time_rx,
                     sizeof(survey->channel_time_rx));
        survey->filled |= SURVEY_HAS_CHAN_TIME_RX;
    }

    if (survey_res->channel_time_tx)
    {
        (void)memcpy((void *)&survey->channel_time_tx, (const void *)&survey_res->channel_time_tx,
                     sizeof(survey->channel_time_tx));
        survey->filled |= SURVEY_HAS_CHAN_TIME_TX;
    }

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.survey_res(wifi_if_ctx_rtos->hapd_drv_if_ctx, survey, more_res);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.survey_res(wifi_if_ctx_rtos->supp_drv_if_ctx, survey, more_res);
    }
}

void wifi_nxp_wpa_supp_event_proc_auth_resp(void *if_priv, nxp_wifi_event_mlme_t *auth_resp, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt = NULL;
    const unsigned char *frame        = NULL;
    unsigned int frame_len            = 0;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)auth_resp->frame.frame;
    frame_len = auth_resp->frame.frame_len;
    mgmt      = (const struct ieee80211_mgmt *)frame;

    if (frame_len < 4 + (2 * WIFI_ETH_ADDR_LEN))
    {
        supp_e("%s: MLME event too short", __func__);
        return;
    }

    if (frame_len < 24 + sizeof(mgmt->u.auth))
    {
        supp_e("%s: Authentication response frame too short", __func__);
        return;
    }

    memset(&event, 0, sizeof(event));

    memcpy(event.auth.peer, mgmt->sa, ETH_ALEN);

    event.auth.auth_type = le_to_host16(mgmt->u.auth.auth_alg);

    event.auth.auth_transaction = le_to_host16(mgmt->u.auth.auth_transaction);

    event.auth.status_code = le_to_host16(mgmt->u.auth.status_code);

    if (frame_len > 24 + sizeof(mgmt->u.auth))
    {
        event.auth.ies     = mgmt->u.auth.variable;
        event.auth.ies_len = (frame_len - 24 - sizeof(mgmt->u.auth));
    }

    wifi_if_ctx_rtos->supp_callbk_fns.auth_resp(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
}

void wifi_nxp_wpa_supp_event_proc_assoc_resp(void *if_priv,
                                             nxp_wifi_assoc_event_mlme_t *assoc_resp,
                                             unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt              = NULL;
    const unsigned char *frame                     = NULL;
    const unsigned char *req_frame                 = NULL;
    const unsigned char *bssid                     = NULL;
    unsigned int frame_len                         = 0;
    unsigned short status                          = WLAN_STATUS_UNSPECIFIED_FAILURE;
    enum sta_connect_fail_reason_codes reason_code = STA_CONNECT_FAIL_REASON_UNSPECIFIED;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)assoc_resp->frame.frame;
    frame_len = assoc_resp->frame.frame_len;
    req_frame = (const unsigned char *)assoc_resp->req_ie;

    mgmt = (const struct ieee80211_mgmt *)frame;

    memset(&event, 0, sizeof(event));
    memset(&wifi_if_ctx_rtos->assoc_bssid, 0, ETH_ALEN);

    if (frame_len < 24 + sizeof(mgmt->u.assoc_resp))
    {
        supp_d("%s: Association response frame too short", __func__);
        bssid       = (unsigned char *)&wifi_if_ctx_rtos->attempt_bssid;
        reason_code = STA_CONNECT_FAIL_REASON_ASSOC_NO_RESP_RECEIVED;
        goto fail;
    }

    status = le_to_host16(mgmt->u.assoc_resp.status_code);

    bssid = mgmt->bssid;

    if (status != WLAN_STATUS_SUCCESS)
    {
    fail:
        wifi_if_ctx_rtos->associated = false;
        event.assoc_reject.bssid     = bssid;

        if (frame_len > 24 + sizeof(mgmt->u.assoc_resp))
        {
            event.assoc_reject.resp_ies     = (unsigned char *)mgmt->u.assoc_resp.variable;
            event.assoc_reject.resp_ies_len = (frame_len - 24 - sizeof(mgmt->u.assoc_resp));
        }

        event.assoc_reject.status_code    = status;
        event.assoc_reject.reason_code    = reason_code;
        event.assoc_reject.timeout_reason = NULL;
    }
    else
    {
        wifi_if_ctx_rtos->associated = true;
        memcpy(&wifi_if_ctx_rtos->assoc_bssid, mgmt->bssid, ETH_ALEN);

        event.assoc_info.addr           = mgmt->bssid;
        event.assoc_info.resp_frame     = frame;
        event.assoc_info.resp_frame_len = frame_len;
        event.assoc_info.freq           = wifi_if_ctx_rtos->assoc_freq;

        if (frame_len > 24 + sizeof(mgmt->u.assoc_resp))
        {
            event.assoc_info.resp_ies     = (unsigned char *)mgmt->u.assoc_resp.variable;
            event.assoc_info.resp_ies_len = (frame_len - 24 - sizeof(mgmt->u.assoc_resp));
        }
        if (assoc_resp->req_ie_len)
        {
            event.assoc_info.req_ies     = (unsigned char *)req_frame;
            event.assoc_info.req_ies_len = assoc_resp->req_ie_len;
        }
    }

    wifi_if_ctx_rtos->supp_callbk_fns.assoc_resp(wifi_if_ctx_rtos->supp_drv_if_ctx, &event, status);
}

void wifi_nxp_wpa_supp_event_proc_deauth(void *if_priv, nxp_wifi_event_mlme_t *deauth, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt = NULL;
    const unsigned char *frame        = NULL;
    unsigned int frame_len            = 0;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)deauth->frame.frame;
    frame_len = deauth->frame.frame_len;
    mgmt      = (const struct ieee80211_mgmt *)frame;

    if (frame_len < 24 + sizeof(mgmt->u.deauth))
    {
        supp_e("%s: Deauthentication response frame too short", __func__);
        return;
    }

    memset(&event, 0, sizeof(event));

    event.deauth_info.addr        = &mgmt->sa[0];
    event.deauth_info.reason_code = le_to_host16(WLAN_REASON_DEAUTH_LEAVING);
    if (frame + frame_len > mgmt->u.deauth.variable)
    {
        event.deauth_info.ie     = mgmt->u.deauth.variable;
        event.deauth_info.ie_len = (frame + frame_len - mgmt->u.deauth.variable);
    }

    (void)wifi_event_completion(WIFI_EVENT_DEAUTHENTICATION, le_to_host16(mgmt->u.deauth.reason_code), NULL);

    wifi_if_ctx_rtos->supp_callbk_fns.deauth(wifi_if_ctx_rtos->supp_drv_if_ctx, &event, mgmt);
}

void wifi_nxp_wpa_supp_event_proc_disassoc(void *if_priv, nxp_wifi_event_mlme_t *disassoc, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt = NULL;
    const unsigned char *frame        = NULL;
    unsigned int frame_len            = 0;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)disassoc->frame.frame;
    frame_len = disassoc->frame.frame_len;
    mgmt      = (const struct ieee80211_mgmt *)frame;

    if (frame_len < 24 + sizeof(mgmt->u.disassoc))
    {
        supp_e("%s: Dis-association response frame too short", __func__);
        return;
    }

    memset(&event, 0, sizeof(event));

    event.disassoc_info.addr        = &mgmt->sa[0];
    event.disassoc_info.reason_code = le_to_host16(mgmt->u.disassoc.reason_code);
    if (frame + frame_len > mgmt->u.disassoc.variable)
    {
        event.disassoc_info.ie     = mgmt->u.disassoc.variable;
        event.disassoc_info.ie_len = (frame + frame_len - mgmt->u.disassoc.variable);
    }

    wifi_if_ctx_rtos->supp_callbk_fns.disassoc(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
}

void wifi_nxp_wpa_supp_event_proc_remain_on_channel(void *if_priv, int cancel_channel)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    wifi_if_ctx_rtos                           = (struct wifi_nxp_ctx_rtos *)if_priv;
    union wpa_event_data event;
    os_memset(&event, 0, sizeof(event));
    event.remain_on_channel.freq     = wifi_if_ctx_rtos->remain_on_channel_freq;
    event.remain_on_channel.duration = wifi_if_ctx_rtos->remain_on_channel_duration;
    wifi_if_ctx_rtos->supp_callbk_fns.remain_on_channel(wifi_if_ctx_rtos->supp_drv_if_ctx, cancel_channel, &event);
}

void *wifi_nxp_wpa_supp_dev_init(void *supp_drv_if_ctx,
                                 const char *iface_name,
                                 rtos_wpa_supp_dev_callbk_fns *supp_callbk_fns)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    const struct netif *iface = NULL;

#ifdef __ZEPHYR__
    iface = net_if_get_binding(iface_name);
#else
    LOCK_TCPIP_CORE();
    iface = netif_find(iface_name);
    UNLOCK_TCPIP_CORE();
#endif

    if (!iface)
    {
        supp_e("%s: Interface %s not found", __func__, iface_name);
        return NULL;
    }

    if (strstr(iface_name, "ml"))
        wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.if_priv;
    else
        wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.hapd_if_priv;

    if (!wifi_if_ctx_rtos)
    {
        supp_e("%s: Interface %s not properly initialized", __func__, iface_name);
        return NULL;
    }

    memset(wifi_if_ctx_rtos, 0x00, sizeof(struct wifi_nxp_ctx_rtos));

    wifi_if_ctx_rtos->iface_ctx       = iface;
    wifi_if_ctx_rtos->supp_drv_if_ctx = supp_drv_if_ctx;

    if (strstr(iface_name, "ml"))
    {
        wifi_if_ctx_rtos->bss_type = BSS_TYPE_STA;
    }
    else
    {
        wifi_if_ctx_rtos->bss_type = BSS_TYPE_UAP;
    }
    wifi_if_ctx_rtos->last_mgmt_tx_data = (uint8_t *)OSA_MemoryAllocate(MAX_MGMT_TX_FRAME_SIZE);

    if (!wifi_if_ctx_rtos->last_mgmt_tx_data)
    {
        supp_e("%s: Buffer to store mgmt tx failed", __func__);
        return NULL;
    }

    memcpy(&wifi_if_ctx_rtos->supp_callbk_fns, supp_callbk_fns, sizeof(wifi_if_ctx_rtos->supp_callbk_fns));
    return wifi_if_ctx_rtos;
}

void wifi_nxp_wpa_supp_dev_deinit(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (wifi_if_ctx_rtos != NULL)
    {
        if (wifi_if_ctx_rtos->last_mgmt_tx_data != NULL)
        {
            OSA_MemoryFree(wifi_if_ctx_rtos->last_mgmt_tx_data);
        }
        memset(wifi_if_ctx_rtos, 0x00, sizeof(struct wifi_nxp_ctx_rtos));
    }
}

static void wifi_nxp_sort_channels(t_u8 channels[], unsigned char num_chans)
{
    t_u8 i, j;

    /* Bubble sort */
    for (i = 0; i < num_chans; i++)
    {
        for (j = 1; j < num_chans - i; j++)
        {
            if ((t_u8)channels[j - 1] > (t_u8)channels[j])
            {
                SWAP_U8(channels[j - 1], channels[j]);
            }
        }
    }
}

int wifi_nxp_wpa_supp_scan2(void *if_priv, struct wpa_driver_scan_params *params)
{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -1;
    int i                                      = 0;
    unsigned char num_chans                    = 0;
    t_u8 bss_mode                              = BSS_INFRASTRUCTURE;
    const char *ssid = NULL, *ssid2 = NULL;
    char ssid_v[(MLAN_MAX_SSID_LENGTH + 1) * MRVDRV_MAX_SSID_LIST_LENGTH] = {0};
    const t_u8 *bssid                                                     = NULL;
    wifi_scan_channel_list_t *chan_list                                   = NULL;
    t_u8 channels[WIFI_SCAN_MAX_NUM_CHAN]                                 = {0};
    mlan_scan_type scan_type                                              = MLAN_SCAN_TYPE_ACTIVE;

    if (!if_priv || !params)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (wifi_if_ctx_rtos->scan_in_progress)
    {
        supp_d("%s: Scan already in progress", __func__);
        ret = -EBUSY;
        goto out;
    }

    if (wifi_is_remain_on_channel())
    {
        supp_e("%s: Block scan while remaining on channel", __func__);
        ret = -EBUSY;
        goto out;
    }

    wifi_d("initiating wifi-scan");

    if (params->freqs)
    {
        for (i = 0; params->freqs[i] && i < WIFI_SCAN_MAX_NUM_CHAN; i++)
        {
            channels[i] = freq_to_chan(params->freqs[i]);
        }

        num_chans = i;
    }
    else
    {
        num_chans = 0;
    }
    uint8_t ssid_off = 0;
    for (i = 0; i < params->num_ssids; i++)
    {
        memcpy(ssid_v + ssid_off, (const char *)params->ssids[i].ssid, params->ssids[i].ssid_len);
        ssid_off += params->ssids[i].ssid_len;
        ssid_v[ssid_off] = '\0';
        ssid_off++;
    }
    ssid = (const char *)&ssid_v;
#if 0
#if CONFIG_COMBO_SCAN
    if (params->num_filter_ssids > 1)
    {
        if (params->filter_ssids[1].ssid_len)
        {
            memcpy(ssid_v2, (const char *)params->filter_ssids[1].ssid, params->filter_ssids[1].ssid_len);
            ssid2 = (const char *)&ssid_v2;
        }
    }
#endif
#endif
    /*
     * no ssids means passive scan
     * refer to woal_cfg80211_scan
     */
    if (!params->num_ssids)
    {
        scan_type = MLAN_SCAN_TYPE_PASSIVE;
    }

    bssid = params->bssid;

    if (num_chans != 0)
    {
        chan_list = OSA_MemoryAllocate(sizeof(wifi_scan_channel_list_t) * num_chans);

        if (chan_list != NULL)
        {
            wifi_nxp_sort_channels(channels, num_chans);
            for (i = 0; i < num_chans; i++)
            {
                chan_list[i].chan_number = channels[i];
                chan_list[i].scan_type   = scan_type;
                chan_list[i].scan_time   = 100;
            }
        }
    }

    if (params->extra_ies_len)
    {
        status = wifi_set_scan_ies((void *)params->extra_ies, params->extra_ies_len);

        if (status != WM_SUCCESS)
        {
            wifi_d("wifi set scan IEs failed");
            goto out;
        }
    }

    wm_wifi.external_scan = false;
    wm_wifi.wpa_supp_scan = true;

#if CONFIG_HOSTAPD
    wm_wifi.hostapd_op = false;

    if (wifi_if_ctx_rtos->hostapd)
    {
        wm_wifi.hostapd_op = true;
    }
#endif

    status = wifi_send_scan_cmd(bss_mode, bssid, ssid, params->num_ssids, num_chans, chan_list, 0,
#if CONFIG_SCAN_WITH_RSSIFILTER
                                params->filter_rssi,
#endif
#if CONFIG_SCAN_CHANNEL_GAP
                                50U,
#endif
                                false, false);
    if (status != WM_SUCCESS)
    {
        wifi_d("wifi send scan cmd failed");
        goto out;
    }

    wifi_if_ctx_rtos->scan_in_progress = true;
    ret                                = 0;
out:
    if (chan_list != NULL)
    {
        OSA_MemoryFree((void *)chan_list);
    }

    return ret;
}

int wifi_nxp_wpa_supp_set_default_scan_ies(void *priv, const u8 *ies, size_t ies_len)
{
    int ret = -1;

    if ((!priv) || (!ies))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    ret = wifi_nxp_set_default_scan_ies(ies, ies_len);

    if (ret != WM_SUCCESS)
    {
        supp_e("%s: Default scan ies set failed", __func__);
    }
    else
    {
        supp_d("%s: Default scan ies set successfully", __func__);
    }
out:
    return ret;
}

int wifi_nxp_wpa_supp_sched_scan(void *if_priv, struct wpa_driver_scan_params *params)
{
    int status = -WM_FAIL;
    // struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret = -1;
    int i   = 0;
    nxp_wifi_trigger_sched_scan_t *wifi_sched_scan_params;

    if (!if_priv || !params)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    // wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_sched_scan_params = (nxp_wifi_trigger_sched_scan_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_trigger_sched_scan_t));

    if (!wifi_sched_scan_params)
    {
        supp_e("%s:  wifi sched scan params calloc failed", __func__);
        goto out;
    }

    if (params->num_ssids != 0U)
    {
        wifi_sched_scan_params->num_ssids = MIN(WIFI_SCAN_MAX_NUM_SSIDS, params->num_ssids);

        for (i = 0; i < wifi_sched_scan_params->num_ssids; i++)
        {
            memcpy(wifi_sched_scan_params->scan_ssids[i].ssid, (const unsigned char *)params->ssids[i].ssid,
                   params->ssids[i].ssid_len);

            wifi_sched_scan_params->scan_ssids[i].ssid_len = params->ssids[i].ssid_len;
        }
    }

    for (i = 0; params->freqs[i] && i < WIFI_SCAN_MAX_NUM_CHAN; i++)
    {
        wifi_sched_scan_params->chan_list[i] = freq_to_chan(params->freqs[i]);
    }

    wifi_sched_scan_params->num_chans = i;

    if ((params->extra_ies) && (params->extra_ies_len))
    {
        memcpy(wifi_sched_scan_params->extra_ies.ie, params->extra_ies, params->extra_ies_len);
        wifi_sched_scan_params->extra_ies.ie_len = params->extra_ies_len;
    }

    wifi_sched_scan_params->chan_per_scan = MIN(WLAN_BG_SCAN_CHAN_MAX, wifi_sched_scan_params->num_chans);

    wifi_sched_scan_params->scan_interval = MIN_BGSCAN_INTERVAL;
    wifi_sched_scan_params->repeat_count  = 2;

    wifi_sched_scan_params->report_condition = BG_SCAN_SSID_MATCH | BG_SCAN_WAIT_ALL_CHAN_DONE;

    wifi_sched_scan_params->filter_rssi = params->filter_rssi;

    status = wifi_send_sched_scan_cmd(wifi_sched_scan_params);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: Sched Scan trigger failed", __func__);
        goto out;
    }
    ret = 0;
out:
    return ret;
}

int wifi_nxp_wpa_supp_stop_sched_scan(void *if_priv)
{
    int status = WM_SUCCESS;
    int ret    = -1;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    status = wifi_send_stop_sched_scan_cmd();
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi_supp_stop_sched_scan failed", __func__);
        goto out;
    }
    ret = 0;
out:
    return ret;
}

int wifi_nxp_wpa_supp_scan_abort(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int status                                 = -WM_FAIL;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (!wifi_if_ctx_rtos || (!wifi_if_ctx_rtos->scan_in_progress))
    {
        supp_d("%s:Ignore scan abort, no scan in progress", __func__);
        goto out;
    }

    wlan_abort_split_scan();

    status = WM_SUCCESS;

out:
    return status;
}

struct wpa_scan_res *wifi_nxp_wpa_supp_proc_scan_res(nxp_wifi_event_new_scan_result_t *scan_res,
                                                     struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos)
{
    struct wpa_scan_res *r  = NULL;
    const unsigned char *ie = NULL;
    unsigned int ie_len     = 0;
    unsigned char *pos      = NULL;

    if (scan_res->ies.ie_len != 0U)
    {
        ie     = (const unsigned char *)scan_res->ies.ie;
        ie_len = scan_res->ies.ie_len;
    }

    r = (struct wpa_scan_res *)OSA_MemoryAllocate(sizeof(*r) + ie_len);

    if (!r)
    {
        supp_e("%s: Unable to calloc  memory for scan result\n", __func__);
        if (ie)
        {
            OSA_MemoryFree((void *)ie);
        }
        return NULL;
    }

    memcpy(r->bssid, scan_res->mac_addr, ETH_ALEN);

    r->freq = scan_res->frequency;

    r->beacon_int = scan_res->beacon_interval;

    r->caps = scan_res->capability;

    if (scan_res->noise == 0)
    {
        r->flags |= WPA_SCAN_NOISE_INVALID;
    }
    else
    {
        r->noise = scan_res->noise;
    }

    r->level = scan_res->rssi;

    r->flags |= WPA_SCAN_LEVEL_DBM | WPA_SCAN_QUAL_INVALID;

    (void)memcpy((void *)&r->tsf, (const void *)&scan_res->ies_tsf, sizeof(r->tsf));

    if (scan_res->beacon_ies_tsf > r->tsf)
    {
        (void)memcpy((void *)&r->tsf, (const void *)&scan_res->beacon_ies_tsf, sizeof(r->tsf));
    }

    if (scan_res->seen_ms_ago)
    {
        r->age = scan_res->seen_ms_ago;
    }

    (void)memcpy((void *)&r->parent_tsf, (const void *)&scan_res->ies_tsf, sizeof(r->parent_tsf));

    memcpy(r->tsf_bssid, wifi_if_ctx_rtos->scan_start_tsf_bssid, ETH_ALEN);

    r->ie_len = ie_len;

    pos = (unsigned char *)(r + 1);

    if (ie_len)
    {
        memcpy(pos, ie, ie_len);

        pos += ie_len;

        OSA_MemoryFree((void *)ie);
    }

    if (scan_res->status)
    {
        r->flags |= WPA_SCAN_ASSOCIATED;
    }

    return r;
}

int wifi_nxp_wpa_supp_scan_results_get(void *if_priv, struct wpa_scan_results *scan_res2)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    int ret                                    = -1;
    unsigned int i, num;
    nxp_wifi_event_new_scan_result_t scan_res;
    struct wpa_scan_res *sr = NULL;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    num = wifi_nxp_scan_res_num();

    if (num == 0)
    {
        supp_d("%s: No networks found", __func__);
        goto done;
    }

    scan_res2->res = (struct wpa_scan_res **)OSA_MemoryAllocate(num * sizeof(struct wpa_scan_res *));
    if (!scan_res2->res)
    {
        supp_e("%s: Failed to calloc scan result array", __func__);
        goto out;
    }

    for (i = 0; i < num; i++)
    {
        memset(&scan_res, 0, sizeof(nxp_wifi_event_new_scan_result_t));
        (void)wifi_nxp_scan_res_get2(i, &scan_res);

        sr = wifi_nxp_wpa_supp_proc_scan_res(&scan_res, wifi_if_ctx_rtos);

        if (sr)
        {
            scan_res2->res[scan_res2->num++] = sr;
        }
    }

done:
    ret = 0;
out:
    return ret;
}

int wifi_nxp_wpa_supp_survey_results_get(void *if_priv)
{
    int status                                 = WM_SUCCESS;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -1;
    nxp_wifi_trigger_op_t *wifi_survey_params;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_survey_params = (nxp_wifi_trigger_op_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_trigger_op_t));

    if (!wifi_survey_params)
    {
        supp_e("%s:  wifi survey params calloc failed", __func__);
        goto out;
    }

    wifi_survey_params->hostapd = wifi_if_ctx_rtos->hostapd;

    status = wifi_event_completion(WIFI_EVENT_SURVEY_RESULT_GET, WIFI_EVENT_REASON_SUCCESS, wifi_survey_params);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi_supp_survey_res_get failed", __func__);
        OSA_MemoryFree(wifi_survey_params);
        goto out;
    }
    ret = 0;
out:
    return ret;
}

int wifi_nxp_wpa_supp_deauthenticate(void *if_priv, const char *addr, unsigned short reason_code)
{
    int status = -WM_FAIL;
    int ret    = -1;

    if ((!if_priv) || (!addr))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_d("initiating wifi-deauth");

    status = wifi_nxp_deauthenticate(MLAN_BSS_TYPE_STA, (const unsigned char *)addr, reason_code);
#if CONFIG_11MC
    g_ftm_civic_cfg.civic_req  = 0;
    g_ftm_location_cfg.lci_req = 0;
#endif
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi_nxp_wpa_supp_deauthenticate failed", __func__);
        goto out;
    }
    ret = 0;
out:
    return ret;
}

#if 0
int wifi_nxp_wpa_supp_add_key(struct nxp_wifi_umac_key_info *key_info, enum wpa_alg alg,
			      int key_idx,
			      int defkey, const unsigned char *seq, size_t seq_len,
			      const unsigned char *key, size_t key_len)
{
	unsigned int suite = 0;

	suite = wpa_alg_to_cipher_suite(alg, key_len);

	if (!suite) {
		return -1;
	}

	if (defkey && alg == WPA_ALG_BIP_CMAC_128) {
		key_info->nxp_wifi_flags = NXP_WIFI_KEY_DEFAULT_MGMT;
	} else if (defkey) {
		key_info->nxp_wifi_flags = NXP_WIFI_KEY_DEFAULT;
	}

	key_info->key_idx = key_idx;
	key_info->cipher_suite = suite;

	if (key && key_len) {
		memcpy(key_info->key.nxp_wifi_key, key, key_len);
		key_info->key.nxp_wifi_key_len = key_len;
	}
	if (seq && seq_len) {
		memcpy(key_info->seq.nxp_wifi_seq, seq, seq_len);
		key_info->seq.nxp_wifi_seq_len = seq_len;
	}

	return 0;
}
#endif

int wifi_nxp_wpa_supp_authenticate(void *if_priv, struct wpa_driver_auth_params *params, struct wpa_bss *curr_bss)
{
    int status         = -WM_FAIL;
    unsigned char *pos = NULL;
    unsigned char auth_alg;
    unsigned char auth_trans_num[2] = {1, 0};
    unsigned char status_code[2]    = {0, 0};
    int ret                         = -1;
    unsigned short len              = 0;
    int channel;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    auth_alg = get_algo_from_auth_type(params->auth_alg);

    if (params->auth_alg == WPA_AUTH_ALG_FT)
    {
        pos = (unsigned char *)params->ie;
        len = params->ie_len;
    }
    else if ((params->auth_data != NULL) && (params->auth_data_len >= 4))
    {
        pos = (unsigned char *)params->auth_data;

        auth_trans_num[0] = pos[0];
        auth_trans_num[1] = pos[1];
        status_code[0]    = pos[2];
        status_code[1]    = pos[3];
        pos += 4;
        len = params->auth_data_len - 4;
    }

    channel = freq_to_chan(params->freq);

    wifi_d("initiating wifi-auth");

    status = wifi_send_mgmt_auth_request(channel, auth_alg, auth_trans_num, status_code, params->bssid,
                                         (const unsigned char *)pos, len);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: MLME command failed (auth): ret=%d", __func__, ret);
        ret = -1;
    }
    else
    {
        supp_d("%s:Authentication request sent successfully", __func__);
        ret = 0;
    }
out:
    return ret;
}

int wifi_nxp_wpa_supp_associate(void *if_priv, struct wpa_driver_associate_params *params)
{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    nxp_wifi_assoc_info_t *assoc_params;
    int ret = -1;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    assoc_params = (nxp_wifi_assoc_info_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_assoc_info_t));

    if (!assoc_params)
    {
        supp_e("%s: assoc params calloc failed", __func__);
        goto out;
    }

    wifi_if_ctx_rtos->associated = false;

    if (params->auth_alg & WPA_AUTH_ALG_FT)
    {
        assoc_params->is_ft = true;
    }

    if (params->bssid)
    {
        memcpy(&wifi_if_ctx_rtos->attempt_bssid, params->bssid, ETH_ALEN);

        memcpy(assoc_params->bssid, params->bssid, sizeof(assoc_params->bssid));
    }

    if (params->prev_bssid)
    {
        memcpy(assoc_params->prev_bssid, params->prev_bssid, sizeof(assoc_params->prev_bssid));
    }

    if (params->freq.freq)
    {
        int channel           = freq_to_chan(params->freq.freq);
        assoc_params->channel = channel;

        wifi_if_ctx_rtos->assoc_freq = params->freq.freq;
    }
    else
    {
        wifi_if_ctx_rtos->assoc_freq = 0;
    }

    if (params->ssid)
    {
        assoc_params->ssid.ssid_len = params->ssid_len;

        memcpy(assoc_params->ssid.ssid, params->ssid, params->ssid_len);
    }

    if (params->wpa_ie)
    {
        assoc_params->wpa_ie.ie_len = params->wpa_ie_len;
        memcpy(assoc_params->wpa_ie.ie, params->wpa_ie, params->wpa_ie_len);
    }

    assoc_params->control_port = 1;

    wifi_d("initiating wifi-assoc");

    status = wifi_nxp_send_assoc(assoc_params);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: MLME command failed (assoc)", __func__);
    }
    else
    {
        supp_d("%s: Association request sent successfully", __func__);
        ret = 0;
    }
    OSA_MemoryFree((void *)assoc_params);

out:
    return ret;
}

int wifi_nxp_wpa_supp_set_key(void *if_priv,
                              const unsigned char *ifname,
                              enum wpa_alg alg,
                              const unsigned char *addr,
                              int key_idx,
                              int set_tx,
                              const unsigned char *seq,
                              size_t seq_len,
                              const unsigned char *key,
                              size_t key_len,
                              enum key_flag key_flag)

{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    unsigned int flags                         = 0;
    int ret                                    = -1;
    bool is_pairwise                           = false;
    int skip_set_key                           = 1;

    if ((!if_priv) || (!ifname))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    if ((key_flag & KEY_FLAG_PAIRWISE_MASK) == KEY_FLAG_PAIRWISE_RX_TX_MODIFY)
    {
        supp_d("SET_KEY (pairwise RX/TX modify)");
    }
    else if (alg == WPA_ALG_NONE && (key_flag & KEY_FLAG_RX_TX))
    {
        supp_d("%s: invalid key_flag to delete key", __func__);
        ret = -1;
        goto out;
    }
    else if (alg == WPA_ALG_NONE)
    {
        supp_d("DEL_KEY");

        status = wifi_remove_key(wifi_if_ctx_rtos->bss_type, is_pairwise, key_idx, addr);

        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi_nxp_del_key failed", __func__);
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        flags = wpa_alg_to_cipher_suite(alg, key_len);

        /* TODO: Implement/check set_tx */
        if (addr && !is_broadcast_ether_addr(addr))
        {
            is_pairwise = true;
            if ((key_flag & KEY_FLAG_PAIRWISE_MASK) == KEY_FLAG_PAIRWISE_RX ||
                (key_flag & KEY_FLAG_PAIRWISE_MASK) == KEY_FLAG_PAIRWISE_RX_TX_MODIFY)
            {
            }
            else if ((key_flag & KEY_FLAG_GROUP_MASK) == KEY_FLAG_GROUP_RX)
            {
            }
            else if (!(key_flag & KEY_FLAG_PAIRWISE))
            {
                supp_d("   key_flag missing PAIRWISE when setting a pairwise key");
                ret = -1;
                goto out;
            }
            else if (alg == WPA_ALG_WEP && (key_flag & KEY_FLAG_RX_TX) == KEY_FLAG_RX_TX)
            {
                supp_d("   unicast WEP key");
                skip_set_key = 0;
            }
            else
            {
                supp_d("   pairwise key");
            }
        }
        else if ((key_flag & KEY_FLAG_PAIRWISE) || !(key_flag & KEY_FLAG_GROUP))
        {
            supp_d("   invalid key_flag for a broadcast key");
            ret = -1;
            goto out;
        }
        else
        {
            supp_d("   broadcast key");
            is_pairwise = false;
            if (key_flag & KEY_FLAG_DEFAULT)
                skip_set_key = 0;
        }

        status =
            wifi_set_key(wifi_if_ctx_rtos->bss_type, is_pairwise, key_idx, key, key_len, seq, seq_len, addr, flags);

        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi_set_key failed", __func__);
        }
        else
        {
            ret = 0;
        }

        if (ret || skip_set_key)
            return ret;
    }
out:
    return ret;
}

int wifi_nxp_wpa_supp_del_key(void *if_priv, const unsigned char *addr, int key_idx)

{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -1;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    status = wifi_remove_key(wifi_if_ctx_rtos->bss_type, 0, key_idx, addr);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi_nxp_del_key failed", __func__);
    }
    else
    {
        ret = 0;
    }

out:
    return ret;
}

int wifi_nxp_wpa_supp_set_rekey_info(
    void *if_priv, const u8 *kek, size_t kek_len, const u8 *kck, size_t kck_len, const u8 *replay_ctr)
{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -1;

    if ((!if_priv) || (!kek) || (!kck) || (!replay_ctr))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    status = wifi_set_rekey_info(wifi_if_ctx_rtos->bss_type, kek, kek_len, kck, kck_len, replay_ctr);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi_set_rekey_info failed", __func__);
        goto out;
    }
    else
    {
        ret = 0;
    }

out:
    return ret;
}

int wifi_nxp_wpa_set_supp_port(void *if_priv, int authorized, char *bssid)
{
    int ret                                    = -1;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    if ((!if_priv) || (!bssid))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_d("initiating wifi-set-port authorized: %d", authorized);

    if (wifi_if_ctx_rtos->associated)
    {
#if CONFIG_WPA_SUPP_WPS
        if (!wifi_nxp_wps_session_enable())
        {
#endif
            if (authorized)
            {
                (void)wifi_event_completion(WIFI_EVENT_AUTHENTICATION, WIFI_EVENT_REASON_SUCCESS, NULL);
            }
#if CONFIG_WPA_SUPP_WPS
        }
#endif
    }

    ret = 0;
out:
    return ret;
}

int wifi_nxp_wpa_supp_set_country(void *if_priv, const char *alpha2)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -WM_FAIL;
    char country[COUNTRY_CODE_LEN]             = {0};
    t_u8 region_code_rw610;
    unsigned char country3 = 0x20;

    if ((!if_priv) || (!alpha2))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    if ((alpha2[2] == 0x4f) || (alpha2[2] == 0x49) || (alpha2[2] == 0x58) || (alpha2[2] == 0x04))
    {
        country3 = alpha2[2];
    }

    country[0] = alpha2[0];
    country[1] = alpha2[1];
    country[2] = country3;

    ret = wlan_11d_region_2_code(mlan_adap, (t_u8 *)country, &region_code_rw610);
    if(ret != WM_SUCCESS)
    {
        goto out;
    }
    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    ret              = wifi_nxp_set_country(wifi_if_ctx_rtos->bss_type, alpha2);
    if (ret != WM_SUCCESS)
    {
        goto out;
    }

#if defined(RW610) && (CONFIG_COMPRESS_TX_PWTBL)
    ret = wlan_set_rg_power_cfg(region_code_rw610);
    if (ret != WM_SUCCESS)
    {
        goto out;
    }
#endif
#if defined(RW610) && ((CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX))
    ret = wlan_set_ru_power_cfg(region_code_rw610);
    if (ret != WM_SUCCESS)
    {
        goto out;
    }
#endif

    return ret;

out:
    return -1;
}

int wifi_nxp_wpa_supp_get_country(void *if_priv, char *alpha2)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    if ((!if_priv) || (!alpha2))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    return wifi_nxp_get_country(wifi_if_ctx_rtos->bss_type, alpha2);

out:
    return -1;
}

int wifi_nxp_wpa_supp_signal_poll(void *if_priv, struct wpa_signal_info *si, unsigned char *bssid)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -WM_FAIL;
    nxp_wifi_signal_info_t signal_params;

    if (!if_priv || !si || !bssid)
    {
        supp_e("%s: Invalid params\r\n", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    ret = wifi_nxp_get_signal(wifi_if_ctx_rtos->bss_type, &signal_params);
    if (ret != WM_SUCCESS)
    {
        supp_e("%s: wifi_nxp_get_signal failed", __func__);
        goto out;
    }

    memset(si, 0x00, sizeof(struct wpa_signal_info));

    si->frequency         = wifi_if_ctx_rtos->assoc_freq;
    si->current_signal    = signal_params.current_signal;
    si->avg_signal        = signal_params.avg_signal;
    si->avg_beacon_signal = signal_params.avg_beacon_signal;
    si->current_noise     = signal_params.current_noise;

out:
    return ret;
}

void wifi_nxp_wpa_supp_event_acs_channel_selected(void *if_priv, nxp_wifi_acs_params *acs_params)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (!if_priv)
    {
        supp_e("%s: Missing interface context", __func__);
        return;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    if (!acs_params)
    {
        supp_e("%s: Missing acs params data", __func__);
        return;
    }

    event.acs_selected_channels.pri_freq = acs_params->pri_freq;
    event.acs_selected_channels.sec_freq = acs_params->sec_freq;
    event.acs_selected_channels.ch_width = acs_params->ch_width;
    event.acs_selected_channels.hw_mode  = (enum hostapd_hw_mode)acs_params->hw_mode;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.acs_channel_sel(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.acs_channel_sel(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_mgmt_tx_status(void *if_priv, nxp_wifi_event_mlme_t *mlme_event, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    if (!if_priv)
    {
        supp_e("%s: Missing interface context", __func__);
        return;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (!mlme_event)
    {
        supp_e("%s: Missing MLME event data", __func__);
        return;
    }

    if (wifi_if_ctx_rtos->mgmt_tx_status == 0U)
    {
        supp_d("%s: Only send mgmt tx status", __func__);
        return;
    }

    if (wifi_if_ctx_rtos->last_mgmt_tx_data_len && wifi_if_ctx_rtos->mgmt_tx_status)
    {
        memcpy((void *)mlme_event->frame.frame, (const void *)wifi_if_ctx_rtos->last_mgmt_tx_data,
               (size_t)wifi_if_ctx_rtos->last_mgmt_tx_data_len);
        mlme_event->frame.frame_len             = wifi_if_ctx_rtos->last_mgmt_tx_data_len;
        wifi_if_ctx_rtos->last_mgmt_tx_data_len = 0;
        wifi_if_ctx_rtos->mgmt_tx_status        = 0;
    }

    wifi_if_ctx_rtos->mgmt_tx_status = 0;

    if (mlme_event->frame.frame_len == 0)
    {
        supp_d("%s: mgmt tx status frame invalid", __func__);
        return;
    }

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.mgmt_tx_status(wifi_if_ctx_rtos->hapd_drv_if_ctx,
                                                            (const unsigned char *)mlme_event->frame.frame,
                                                            mlme_event->frame.frame_len, true);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.mgmt_tx_status(wifi_if_ctx_rtos->supp_drv_if_ctx,
                                                         (const unsigned char *)mlme_event->frame.frame,
                                                         mlme_event->frame.frame_len, true);
    }
}

void wifi_nxp_wpa_supp_event_proc_unprot_mgmt(void *if_priv, nxp_wifi_event_mlme_t *unprot_mgmt, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt = NULL;
    const unsigned char *frame        = NULL;
    unsigned int frame_len            = 0;

    if (!if_priv)
    {
        supp_e("%s: Missing interface context", __func__);
        return;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)unprot_mgmt->frame.frame;
    frame_len = unprot_mgmt->frame.frame_len;

    mgmt = (const struct ieee80211_mgmt *)frame;

    if (frame_len < 24 + sizeof(mgmt->u.deauth))
    {
        supp_e("%s: Unprotected mgmt frame too short", __func__);
        return;
    }

    memset(&event, 0, sizeof(event));

    event.unprot_deauth.sa = &mgmt->sa[0];
    event.unprot_deauth.da = &mgmt->da[0];

    //	if (cmd_evnt == NXP_WIFI_EVENT_UNPROT_DEAUTHENTICATE) {
    event.unprot_deauth.reason_code = le_to_host16(mgmt->u.deauth.reason_code);
    wifi_if_ctx_rtos->supp_callbk_fns.unprot_deauth(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    //	} else if (cmd_evnt == NXP_WIFI_EVENT_UNPROT_DISASSOCIATE) {
    event.unprot_disassoc.reason_code = le_to_host16(mgmt->u.deauth.reason_code);
    wifi_if_ctx_rtos->supp_callbk_fns.unprot_disassoc(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    //	}
}

int wifi_nxp_wpa_send_mlme(void *if_priv,
                           const u8 *data,
                           size_t data_len,
                           int noack,
                           unsigned int freq,
                           int no_cck,
                           int offchanok,
                           unsigned int wait_time,
                           int cookie)
{
    int status                                 = -WM_FAIL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    const struct ieee80211_hdr *hdr;
    u16 fc, stype;

    hdr   = (const struct ieee80211_hdr *)data;
    fc    = le_to_host16(hdr->frame_control);
    stype = WLAN_FC_GET_STYPE(fc);

    if (!if_priv)
    {
        supp_e("%s: Missing interface context", __func__);
        goto out;
    }

    if (data_len > 1500)
    {
        supp_d("%s: Invalid data length", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    wifi_if_ctx_rtos->mgmt_tx_status = 0;

    status = wifi_nxp_send_mlme(wifi_if_ctx_rtos->bss_type, freq_to_chan(freq), wait_time, data, data_len);

    if (status == -WM_FAIL)
    {
        wifi_if_ctx_rtos->last_mgmt_tx_data_len = 0;
        wifi_if_ctx_rtos->mgmt_tx_status        = 0;
        supp_e("%s: wifi_inject_frame failed", __func__);
        goto out;
    }

    if (((wifi_if_ctx_rtos->bss_type == BSS_TYPE_UAP) &&
         ((stype == WLAN_FC_STYPE_ASSOC_RESP) || (stype == WLAN_FC_STYPE_REASSOC_RESP))) ||
        (stype == WLAN_FC_STYPE_ACTION))
    {
        memcpy((void *)wifi_if_ctx_rtos->last_mgmt_tx_data, (const void *)data, (size_t)data_len);
        wifi_if_ctx_rtos->last_mgmt_tx_data_len = data_len;
        wifi_if_ctx_rtos->mgmt_tx_status        = 1;
    }
    else
    {
        wifi_if_ctx_rtos->last_mgmt_tx_data_len = 0;
    }

out:
    return status;
}

int wifi_nxp_wpa_supp_remain_on_channel(void *if_priv, unsigned int freq, unsigned int duration)
{
    int status                                 = -WM_FAIL;
    int ret                                    = -1;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int channel;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }
    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    if (freq)
    {
        wifi_if_ctx_rtos->remain_on_channel_freq = freq;
    }
    wifi_if_ctx_rtos->remain_on_channel_duration = duration;

    channel = freq_to_chan(wifi_if_ctx_rtos->remain_on_channel_freq);

    wifi_if_ctx_rtos->supp_called_remain_on_chan = true;
    wifi_if_ctx_rtos->remain_on_chan_is_canceled = false;
    status                                       = wifi_remain_on_channel(true, channel, duration);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: Remain on channel cmd failed", __func__);
        ret = -1;
    }
    else
    {
        supp_d("%s:Remain on channel sent successfully", __func__);
        ret = 0;
    }
out:
    return ret;
}

int wifi_nxp_wpa_supp_cancel_remain_on_channel(void *if_priv)
{
    int status                                 = -WM_FAIL;
    int ret                                    = -1;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }
    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    if (wifi_if_ctx_rtos->remain_on_chan_is_canceled)
    {
        supp_d("%s:Already canceled, ignore it", __func__);
        ret = 0;
        goto out;
    }

    wifi_if_ctx_rtos->supp_called_remain_on_chan = true;
    wifi_if_ctx_rtos->remain_on_chan_is_canceled = true;
    status                                       = wifi_remain_on_channel(false, 0, 0);

    if (status != WM_SUCCESS)
    {
        supp_e("%s: Cancel on channel cmd failed", __func__);
        ret = -1;
    }
    else
    {
        supp_d("%s:Cancel on channel sent successfully", __func__);
        ret = 0;
    }
out:
    return ret;
}

void wifi_nxp_wpa_supp_event_proc_mgmt_rx(void *if_priv, nxp_wifi_event_mlme_t *mgmt_rx, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;
    const struct ieee80211_mgmt *mgmt = NULL;
    const unsigned char *frame        = NULL;
    unsigned int frame_len            = 0;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    frame     = (const unsigned char *)mgmt_rx->frame.frame;
    frame_len = mgmt_rx->frame.frame_len;
    mgmt      = (const struct ieee80211_mgmt *)frame;

    if (frame_len < 4 + (2 * WIFI_ETH_ADDR_LEN))
    {
        supp_e("%s: MLME event too short", __func__);
        return;
    }

    if (frame_len < 24 + sizeof(mgmt->u.deauth))
    {
        supp_e("%s: MGMT RX frame too short", __func__);
        return;
    }

    memset(&event, 0, sizeof(event));

    event.rx_mgmt.frame     = (const u8 *)mgmt;
    event.rx_mgmt.frame_len = frame_len;
    event.rx_mgmt.freq      = mgmt_rx->frame.freq;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.mgmt_rx(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.mgmt_rx(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_eapol_rx(void *if_priv, nxp_wifi_event_eapol_mlme_t *eapol_rx, unsigned int event_len)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    event.eapol_rx.src      = (const unsigned char *)eapol_rx->mac_addr;
    event.eapol_rx.data     = (const unsigned char *)eapol_rx->frame.frame;
    event.eapol_rx.data_len = eapol_rx->frame.frame_len;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.eapol_rx(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.eapol_rx(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_ecsa_complete(void *if_priv, nxp_wifi_ch_switch_info *ch_switch_info)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    event.ch_switch.freq       = ch_switch_info->center_freq;
    event.ch_switch.ht_enabled = ch_switch_info->ht_enabled;
    event.ch_switch.ch_offset  = ch_switch_info->ch_offset;
    event.ch_switch.ch_width   = (enum chan_width)ch_switch_info->ch_width;
    event.ch_switch.cf1        = ch_switch_info->center_freq1;
    event.ch_switch.cf2        = ch_switch_info->center_freq2;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.ecsa_complete(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.ecsa_complete(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_dfs_cac_started(void *if_priv, nxp_wifi_dfs_cac_info *dfs_cac_info)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    event.dfs_event.freq        = dfs_cac_info->center_freq;
    event.dfs_event.ht_enabled  = dfs_cac_info->ht_enabled;
    event.dfs_event.chan_offset = dfs_cac_info->ch_offset;
    event.dfs_event.chan_width  = (enum chan_width)dfs_cac_info->ch_width;
    event.dfs_event.cf1         = dfs_cac_info->center_freq1;
    event.dfs_event.cf2         = dfs_cac_info->center_freq2;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.dfs_cac_started(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.dfs_cac_started(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

void wifi_nxp_wpa_supp_event_proc_dfs_cac_finished(void *if_priv, nxp_wifi_dfs_cac_info *dfs_cac_info)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    union wpa_event_data event;

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    memset(&event, 0, sizeof(event));

    event.dfs_event.freq        = dfs_cac_info->center_freq;
    event.dfs_event.ht_enabled  = dfs_cac_info->ht_enabled;
    event.dfs_event.chan_offset = dfs_cac_info->ch_offset;
    event.dfs_event.chan_width  = (enum chan_width)dfs_cac_info->ch_width;
    event.dfs_event.cf1         = dfs_cac_info->center_freq1;
    event.dfs_event.cf2         = dfs_cac_info->center_freq2;

#if CONFIG_HOSTAPD
    if (wifi_if_ctx_rtos->hostapd)
    {
        wifi_if_ctx_rtos->hostapd_callbk_fns.dfs_cac_finished(wifi_if_ctx_rtos->hapd_drv_if_ctx, &event);
    }
    else
#endif
    {
        wifi_if_ctx_rtos->supp_callbk_fns.dfs_cac_finished(wifi_if_ctx_rtos->supp_drv_if_ctx, &event);
    }
}

#if CONFIG_HOSTAPD

void *wifi_nxp_hostapd_dev_init(void *hapd_drv_if_ctx,
                                const char *iface_name,
                                rtos_hostapd_dev_callbk_fns *hostapd_callbk_fns)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    const struct netif *iface = NULL;

#ifdef __ZEPHYR__
    iface = net_if_get_binding(iface_name);
#else
    LOCK_TCPIP_CORE();
    iface = netif_find(iface_name);
    UNLOCK_TCPIP_CORE();
#endif

    if (!iface)
    {
        supp_e("%s: Interface %s not found", __func__, iface_name);
        return NULL;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.hapd_if_priv;

    if (!wifi_if_ctx_rtos)
    {
        supp_e("%s: Interface %s not properly initialized", __func__, iface_name);
        return NULL;
    }

    memset(wifi_if_ctx_rtos, 0x00, sizeof(struct wifi_nxp_ctx_rtos));

    wifi_if_ctx_rtos->iface_ctx = iface;

    wifi_if_ctx_rtos->hostapd = true;

    wifi_if_ctx_rtos->bss_type = BSS_TYPE_UAP;

    wifi_if_ctx_rtos->hapd_drv_if_ctx = hapd_drv_if_ctx;

    wifi_if_ctx_rtos->last_mgmt_tx_data = (uint8_t *)OSA_MemoryAllocate(MAX_MGMT_TX_FRAME_SIZE);

    if (!wifi_if_ctx_rtos->last_mgmt_tx_data)
    {
        supp_e("%s: Buffer to store mgmt tx failed", __func__);
        return NULL;
    }

    memcpy(&wifi_if_ctx_rtos->hostapd_callbk_fns, hostapd_callbk_fns, sizeof(wifi_if_ctx_rtos->hostapd_callbk_fns));

    return wifi_if_ctx_rtos;
}

void wifi_nxp_hostapd_dev_deinit(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if (wifi_if_ctx_rtos != NULL)
    {
        OSA_MemoryFree((void *)wifi_if_ctx_rtos->last_mgmt_tx_data);
        memset(wifi_if_ctx_rtos, 0x00, sizeof(struct wifi_nxp_ctx_rtos));
    }
}

int wifi_nxp_hostapd_set_modes(void *if_priv, struct hostapd_hw_modes *modes)
{
    int status     = -WM_FAIL;
    t_u8 bandwidth = wifi_uap_get_bandwidth();

    if ((!if_priv) || (!modes))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    status = wifi_setup_ht_cap(&modes[HOSTAPD_MODE_IEEE80211G].ht_capab, &modes[HOSTAPD_MODE_IEEE80211G].mcs_set[0],
                               &modes[HOSTAPD_MODE_IEEE80211G].a_mpdu_params, 0);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi nxp set 2G infra ht cap failed", __func__);
        goto out;
    }

#if CONFIG_5GHz_SUPPORT
    if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        status = wifi_setup_ht_cap(&modes[HOSTAPD_MODE_IEEE80211A].ht_capab, &modes[HOSTAPD_MODE_IEEE80211A].mcs_set[0],
                                   &modes[HOSTAPD_MODE_IEEE80211A].a_mpdu_params, 1);
        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi nxp set 5G infra ht cap failed", __func__);
            goto out;
        }
    }
#endif

    modes[HOSTAPD_MODE_IEEE80211G].flags |= HOSTAPD_MODE_FLAG_HT_INFO_KNOWN;
#if CONFIG_5GHz_SUPPORT
    if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        modes[HOSTAPD_MODE_IEEE80211A].flags |= HOSTAPD_MODE_FLAG_HT_INFO_KNOWN;
    }
#endif

#if CONFIG_11AC
    status = wifi_setup_vht_cap((t_u32 *)&modes[HOSTAPD_MODE_IEEE80211G].vht_capab,
                                modes[HOSTAPD_MODE_IEEE80211G].vht_mcs_set, 0);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi nxp set 2G infra vht cap failed", __func__);
        goto out;
    }

#if CONFIG_5GHz_SUPPORT
    if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        status = wifi_setup_vht_cap((t_u32 *)&modes[HOSTAPD_MODE_IEEE80211A].vht_capab,
                                    modes[HOSTAPD_MODE_IEEE80211A].vht_mcs_set, 1);
        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi nxp set 5G infra vht cap failed", __func__);
            goto out;
        }
    }
#endif

    modes[HOSTAPD_MODE_IEEE80211G].flags |= HOSTAPD_MODE_FLAG_VHT_INFO_KNOWN;
#if CONFIG_5GHz_SUPPORT
    if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        modes[HOSTAPD_MODE_IEEE80211A].flags |= HOSTAPD_MODE_FLAG_VHT_INFO_KNOWN;
    }
#endif
#endif

#if CONFIG_11AX
    if (IS_FW_SUPPORT_11AX(mlan_adap))
    {
        status = wifi_setup_he_cap(
            (nxp_wifi_he_capabilities *)&modes[HOSTAPD_MODE_IEEE80211G].he_capab[IEEE80211_MODE_INFRA], 0);
        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi nxp set 2G infra he cap failed", __func__);
            goto out;
        }

        status = wifi_setup_he_cap(
            (nxp_wifi_he_capabilities *)&modes[HOSTAPD_MODE_IEEE80211G].he_capab[IEEE80211_MODE_AP], 0);
        if (status != WM_SUCCESS)
        {
            supp_e("%s: wifi nxp set 2G ap he cap failed", __func__);
            goto out;
        }
        if (bandwidth == BANDWIDTH_20MHZ)
        {
            modes[HOSTAPD_MODE_IEEE80211G].he_capab[IEEE80211_MODE_AP].phy_cap[HE_PHYCAP_CHANNEL_WIDTH_SET_IDX] = 0;
        }
#if CONFIG_5GHz_SUPPORT
        if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
        {
            status = wifi_setup_he_cap(
                (nxp_wifi_he_capabilities *)&modes[HOSTAPD_MODE_IEEE80211A].he_capab[IEEE80211_MODE_INFRA], 1);
            if (status != WM_SUCCESS)
            {
                supp_e("%s: wifi nxp set 5G infra he cap failed", __func__);
                goto out;
            }

            status = wifi_setup_he_cap(
                (nxp_wifi_he_capabilities *)&modes[HOSTAPD_MODE_IEEE80211A].he_capab[IEEE80211_MODE_AP], 1);
            if (status != WM_SUCCESS)
            {
                supp_e("%s: wifi nxp set 5G ap he cap failed", __func__);
                goto out;
            }
        }
        if (bandwidth == BANDWIDTH_20MHZ)
        {
            modes[HOSTAPD_MODE_IEEE80211A].he_capab[IEEE80211_MODE_AP].phy_cap[HE_PHYCAP_CHANNEL_WIDTH_SET_IDX] = 0;
        }
#endif
    }
#endif

    wifi_setup_channel_info(modes[HOSTAPD_MODE_IEEE80211B].channels, modes[HOSTAPD_MODE_IEEE80211B].num_channels,
                            BAND_2GHZ);
    wifi_setup_channel_info(modes[HOSTAPD_MODE_IEEE80211G].channels, modes[HOSTAPD_MODE_IEEE80211G].num_channels,
                            BAND_2GHZ);
#if CONFIG_5GHz_SUPPORT
    if (!ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        wifi_setup_channel_info(modes[HOSTAPD_MODE_IEEE80211A].channels, modes[HOSTAPD_MODE_IEEE80211A].num_channels,
                                BAND_5GHZ);
    }
#endif

    status = WM_SUCCESS;

out:
    return status;
}

int wifi_nxp_hostapd_do_acs(void *if_priv, struct drv_acs_params *params)
{
    int status = -WM_FAIL;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    status = wifi_uap_do_acs(params->freq_list);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi uap do acs failed", __func__);
        goto out;
    }

    status = WM_SUCCESS;

out:
    return status;
}

#define BSSID_OFFSET 16

int wifi_nxp_hostapd_set_ap(void *if_priv, int beacon_set, struct wpa_driver_ap_params *params)
{
    int status                                 = -WM_FAIL;
    int ret                                    = -1;
    nxp_wifi_ap_info_t *ap_params              = NULL;
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    ap_params = (nxp_wifi_ap_info_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_ap_info_t));

    if (!ap_params)
    {
        supp_e("%s: ap params calloc failed", __func__);
        goto out;
    }

    ap_params->beacon_set = beacon_set;

    memcpy(ap_params->bssid, params->head + BSSID_OFFSET, WIFI_ETH_ADDR_LEN);

    memcpy(ap_params->head_ie.ie, params->head, params->head_len);

    ap_params->head_ie.ie_len = params->head_len;

    memcpy(ap_params->tail_ie.ie, params->tail, params->tail_len);

    ap_params->tail_ie.ie_len = params->tail_len;

    ap_params->dtim_period = params->dtim_period;

    ap_params->beacon_int = params->beacon_int;

    if (params->proberesp && params->proberesp_len)
    {
        memcpy(ap_params->proberesp.ie, params->proberesp, params->proberesp_len);

        ap_params->proberesp_ies.ie_len = params->proberesp_len;
    }

    if (params->ssid && params->ssid_len)
    {
        memcpy((void *)ap_params->ssid.ssid, (const void *)params->ssid, (size_t)params->ssid_len);

        ap_params->ssid.ssid_len = params->ssid_len;
    }

    ap_params->hide_ssid = params->hide_ssid;

    ap_params->pairwise_ciphers = params->pairwise_ciphers;
    ap_params->group_cipher     = params->group_cipher;
    ap_params->key_mgmt_suites  = params->key_mgmt_suites;
    ap_params->auth_algs        = params->auth_algs;
    ap_params->wpa_version      = params->wpa_version;
    ap_params->privacy          = params->privacy;

    if (params->beacon_ies)
    {
        memcpy(ap_params->beacon_ies.ie, params->beacon_ies->buf, params->beacon_ies->used);

        ap_params->beacon_ies.ie_len = params->beacon_ies->used;
    }

    if (params->proberesp_ies)
    {
        memcpy(ap_params->proberesp_ies.ie, params->proberesp_ies->buf, params->proberesp_ies->used);

        ap_params->proberesp_ies.ie_len = params->proberesp_ies->used;
    }

    if (params->assocresp_ies)
    {
        memcpy(ap_params->assocresp_ies.ie, params->assocresp_ies->buf, params->assocresp_ies->used);

        ap_params->assocresp_ies.ie_len = params->assocresp_ies->used;
    }

    ap_params->ht_opmode         = params->ht_opmode;
    ap_params->ap_max_inactivity = params->ap_max_inactivity;
    ap_params->reenable          = params->reenable;
    ap_params->twt_responder     = params->twt_responder;
    ap_params->sae_pwe           = params->sae_pwe;

    if (params->freq)
    {
        ap_params->chan.mode               = (enum wifi_mode)params->freq->mode;
        ap_params->chan.freq               = params->freq->freq;
        ap_params->chan.channel            = params->freq->channel;
        ap_params->chan.sec_channel_offset = params->freq->sec_channel_offset;
        ap_params->chan.bandwidth          = params->freq->bandwidth;
        ap_params->chan.ht_enabled         = params->freq->ht_enabled;
        ap_params->chan.vht_enabled        = params->freq->vht_enabled;
        ap_params->chan.he_enabled         = params->freq->he_enabled;
        ap_params->chan.center_freq1       = params->freq->center_freq1;
        ap_params->chan.center_freq2       = params->freq->center_freq2;
    }

    status = wifi_nxp_beacon_config(ap_params);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi nxp beacon config failed", __func__);
        goto out;
    }

    if (!beacon_set)
    {
        wifi_if_ctx_rtos->uap_started = true;
    }

    ret = 0;
out:
    if (ap_params != NULL)
    {
        OSA_MemoryFree((void *)ap_params);
    }
    return ret;
}

int wifi_nxp_hostapd_sta_add(void *if_priv, struct hostapd_sta_add_params *params)
{
    int status                      = -WM_FAIL;
    nxp_wifi_sta_info_t *sta_params = NULL;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    sta_params = (nxp_wifi_sta_info_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_sta_info_t));

    if (!sta_params)
    {
        supp_e("%s: sta params calloc failed", __func__);
        goto out;
    }

    sta_params->set = params->set;

    memcpy(sta_params->addr, params->addr, WIFI_ETH_ADDR_LEN);

    sta_params->aid = params->aid;

    sta_params->capability = params->capability;

    if (params->supp_rates_len)
    {
        sta_params->supp_rates_len = params->supp_rates_len;
        memcpy((void *)sta_params->supp_rates, (const void *)params->supp_rates, (size_t)sta_params->supp_rates_len);
    }

    sta_params->listen_interval = params->listen_interval;

    if (params->ext_capab)
    {
        memcpy(sta_params->ext_capab, params->ext_capab, params->ext_capab_len);
        sta_params->ext_capab_len = params->ext_capab_len;
    }

    sta_params->qosinfo = params->qosinfo;

    if (params->flags & WPA_STA_AUTHORIZED)
        sta_params->flags |= STA_FLAG_AUTHORIZED;

    if (params->flags & WPA_STA_WMM)
        sta_params->flags |= STA_FLAG_WME;

    if (params->flags & WPA_STA_SHORT_PREAMBLE)
        sta_params->flags |= STA_FLAG_SHORT_PREAMBLE;

    if (params->flags & WPA_STA_MFP)
        sta_params->flags |= STA_FLAG_MFP;

    if (params->flags & WPA_STA_AUTHENTICATED)
        sta_params->flags |= STA_FLAG_AUTHENTICATED;

    if (params->flags & WPA_STA_ASSOCIATED)
        sta_params->flags |= STA_FLAG_ASSOCIATED;

    if (params->ht_capabilities)
    {
        memcpy(&sta_params->ht_capab, params->ht_capabilities, sizeof(struct ieee80211_ht_capabilities));
        sta_params->ht_capab_len = sizeof(struct ieee80211_ht_capabilities);
    }

    if (params->vht_capabilities)
    {
        memcpy(&sta_params->vht_capab, params->vht_capabilities, sizeof(struct ieee80211_vht_capabilities));
        sta_params->vht_capab_len = sizeof(struct ieee80211_vht_capabilities);
    }

    if (params->he_capab)
    {
        memcpy(&sta_params->he_capab, params->he_capab, sizeof(struct ieee80211_he_capabilities));
        sta_params->he_capab_len = params->he_capab_len;
    }

    status = wifi_nxp_sta_add(sta_params);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi nxp sta add failed", __func__);
    }
out:
    if (sta_params != NULL)
    {
        OSA_MemoryFree((void *)sta_params);
    }
    return status;
}

int wifi_nxp_hostapd_sta_remove(void *if_priv, const u8 *addr)
{
    int status = -WM_FAIL;

    if ((!if_priv) || (!addr))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    status = wifi_nxp_sta_remove(addr);
    if (status != WM_SUCCESS)
    {
        supp_e("%s: wifi nxp sta remove failed", __func__);
    }
out:
    return status;
}

int wifi_nxp_hostapd_send_eapol(void *if_priv, const u8 *data, size_t data_len)
{
    int ret = -1;
    if ((!if_priv) || (!data))
    {
        supp_e("%s: Invalid params\n", __func__);
        goto out;
    }

    ret = wifi_supp_inject_frame(WLAN_BSS_TYPE_UAP, data, data_len);

out:
    return ret;
}

int wifi_nxp_hostapd_set_freq(void *if_priv, struct hostapd_freq_params *freq)
{
    int ret = -1;
    nxp_wifi_chan_info_t chan;

    if ((!if_priv) || (!freq))
    {
        supp_e("%s: Invalid params\n", __func__);
        goto out;
    }

    memset(&chan, 0x00, sizeof(nxp_wifi_chan_info_t));

#if 0
    PRINTF("freq->freq: %d\r\n", freq->freq);
    PRINTF("freq->channel: %d\r\n", freq->channel);
    PRINTF("freq->ht_enabled: %d\r\n", freq->ht_enabled);
    PRINTF("freq->sec_channel_offset: %d\r\n", freq->sec_channel_offset);
    PRINTF("freq->vht_enabled: %d\r\n", freq->vht_enabled);
    PRINTF("freq->he_enabled: %d\r\n", freq->he_enabled);
    PRINTF("freq->bandwidth: %d\r\n", freq->bandwidth);
#endif
    ret = 0;

    chan.mode               = (enum wifi_mode)freq->mode;
    chan.freq               = freq->freq;
    chan.channel            = freq->channel;
    chan.sec_channel_offset = freq->sec_channel_offset;
    chan.bandwidth          = freq->bandwidth;
    chan.ht_enabled         = freq->ht_enabled;
    chan.vht_enabled        = freq->vht_enabled;
    chan.he_enabled         = freq->he_enabled;
    chan.center_freq1       = freq->center_freq1;
    chan.center_freq2       = freq->center_freq2;

    ret = 0; // wifi_nxp_set_chan(chan);

out:
    return ret;
}

int wifi_nxp_hostapd_set_rts(void *if_priv, int rts_threshold)
{
    int ret = -1;
    if (!if_priv)
    {
        supp_e("%s: Invalid params\n", __func__);
        goto out;
    }

    // ret = wifi_nxp_set_rts(rts_threshold);
    ret = wifi_set_uap_rts(rts_threshold);

out:
    return ret;
}

int wifi_nxp_hostapd_set_frag(void *if_priv, int frag_threshold)
{
    int ret = -1;
    if (!if_priv)
    {
        supp_e("%s: Invalid params\n", __func__);
        goto out;
    }

    ret = wifi_set_uap_frag(frag_threshold);

out:
    return ret;
}

int wifi_nxp_hostapd_stop_ap(void *if_priv)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                                    = -1;
    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    ret = wifi_nxp_stop_ap();
    if (ret != 0)
    {
        supp_e("%s: Stop AP failed", __func__);
        goto out;
    }
    wifi_if_ctx_rtos->uap_started = false;
out:
    return ret;
}

int wifi_nxp_hostapd_set_acl(void *if_priv, struct hostapd_acl_params *params)
{
    // struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;
    int ret                         = -1;
    nxp_wifi_acl_info_t *acl_params = NULL;
    size_t acl_sz                   = 0;
    unsigned int i;

    if ((!if_priv) || (!params))
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    // wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;

    acl_sz = WIFI_ETH_ADDR_LEN * params->num_mac_acl;

    acl_params = (nxp_wifi_acl_info_t *)OSA_MemoryAllocate(sizeof(nxp_wifi_acl_info_t) + acl_sz);
    if (!acl_params)
    {
        supp_e("%s: acl params calloc failed", __func__);
        goto out;
    }

    acl_params->acl_policy  = params->acl_policy;
    acl_params->num_mac_acl = params->num_mac_acl;

    for (i = 0; i < params->num_mac_acl; i++)
    {
        memcpy(acl_params->mac_acl[i].addr, params->mac_acl[i].addr, WIFI_ETH_ADDR_LEN);
    }

    ret = wifi_nxp_set_acl(acl_params);
    if (ret != 0)
    {
        supp_e("%s: Set ACL failed", __func__);
    }

out:
    if (acl_params)
        OSA_MemoryFree((void *)acl_params);
    return ret;
}
#endif /* CONFIG_HOSTAPD */

int wifi_nxp_wpa_dpp_listen(void *if_priv, bool enable)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

    if (!if_priv)
    {
        supp_e("%s: Invalid params", __func__);
        goto out;
    }

    wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)if_priv;
    if (enable && (wifi_if_ctx_rtos->bss_type == BSS_TYPE_STA))
    {
        return wifi_set_rx_mgmt_indication(wifi_if_ctx_rtos->bss_type, WLAN_MGMT_ACTION);
    }
    else
    {
        return 0;
    }

out:
    return -1;
}

bool wifi_nxp_wpa_get_modes(void *if_priv)
{
    return (!ISSUPP_NO5G(mlan_adap->fw_cap_ext)
            && ((mlan_adap->fw_bands & BAND_A)
            || (mlan_adap->fw_bands & BAND_AN)
#if CONFIG_11AC
            || (mlan_adap->fw_bands & BAND_AAC)
#endif
           ));
}
#endif
