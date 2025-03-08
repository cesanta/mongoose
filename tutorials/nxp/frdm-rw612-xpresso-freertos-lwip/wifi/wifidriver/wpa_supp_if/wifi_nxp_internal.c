/** @file wifi_nxp_internal.c
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
#include <wifi_nxp.h>
#include <dhcp-server.h>
#include <wifi-internal.h>
#include <wifi-debug.h>

#if CONFIG_WPA_SUPP

#include <rtos_wpa_supp_if.h>
#include "supp_main.h"

void wifi_survey_result_get(struct wifi_message *msg)
{
    nxp_wifi_trigger_op_t *wifi_survey_params = (nxp_wifi_trigger_op_t *)msg->data;

#if CONFIG_HOSTAPD
    wm_wifi.hostapd_op = false;

    if (wifi_survey_params->hostapd)
    {
        wm_wifi.hostapd_op = true;
    }
#endif

    wifi_nxp_survey_res_get();
    OSA_MemoryFree(wifi_survey_params);
}

/* Event handlers*/
void wifi_scan_start(struct wifi_message *msg)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.if_priv;

    if (wifi_if_ctx_rtos->scan_in_progress)
    {
        if (msg->reason == WIFI_EVENT_REASON_SUCCESS)
        {
            if (wm_wifi.supp_if_callbk_fns->scan_start_callbk_fn)
            {
                wm_wifi.supp_if_callbk_fns->scan_start_callbk_fn(wm_wifi.if_priv);
            }
        }
    }
}

void wifi_scan_done(struct wifi_message *msg)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = NULL;

#if CONFIG_HOSTAPD
    if (wm_wifi.hostapd_op)
    {
        wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.hapd_if_priv;

        wm_wifi.hostapd_op = false;
    }
    else
#endif
    {
        wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.if_priv;
    }

    wifi_nxp_reset_scan_flag();

    if (msg->reason == WIFI_EVENT_REASON_FAILURE)
    {
        if (wm_wifi.supp_if_callbk_fns->scan_abort_callbk_fn)
        {
            wm_wifi.supp_if_callbk_fns->scan_abort_callbk_fn(wm_wifi.if_priv);
        }
    }

    if (msg->reason == WIFI_EVENT_REASON_SUCCESS)
    {
        if (wm_wifi.supp_if_callbk_fns->scan_done_callbk_fn)
        {
#if CONFIG_HOSTAPD
            if (wifi_if_ctx_rtos->hostapd)
            {
                wm_wifi.supp_if_callbk_fns->scan_done_callbk_fn(wm_wifi.hapd_if_priv, wm_wifi.external_scan);
            }
            else
#endif
            {
                wm_wifi.supp_if_callbk_fns->scan_done_callbk_fn(wm_wifi.if_priv, wm_wifi.external_scan);
            }
        }
    }

    wm_wifi.external_scan = false;
}

void wifi_process_remain_on_channel(struct wifi_message *msg)
{
    struct wifi_nxp_ctx_rtos *wifi_if_ctx_rtos = (struct wifi_nxp_ctx_rtos *)wm_wifi.if_priv;
    if (wifi_if_ctx_rtos->supp_called_remain_on_chan == true)
    {
        if ((msg->reason == WIFI_EVENT_REASON_SUCCESS) &&
            (wm_wifi.supp_if_callbk_fns->remain_on_channel_callbk_fn != NULL))
        {
            if (*(t_u8 *)(msg->data) == true)
            {
                wm_wifi.supp_if_callbk_fns->remain_on_channel_callbk_fn(wifi_if_ctx_rtos, 1);
            }
            else
            {
                wm_wifi.supp_if_callbk_fns->remain_on_channel_callbk_fn(wifi_if_ctx_rtos, 0);
            }
        }
        wifi_if_ctx_rtos->supp_called_remain_on_chan = false;
    }
    if (msg->data)
    {
        OSA_MemoryFree(msg->data);
        msg->data = NULL;
    }
}

void wifi_process_mgmt_tx_status(struct wifi_message *msg)
{
    nxp_wifi_event_mlme_t *resp = &wm_wifi.mgmt_resp;
    resp->frame.frame_len       = 0;

    if (msg->reason == WIFI_EVENT_REASON_SUCCESS)
    {
        if (wm_wifi.supp_if_callbk_fns->mgmt_tx_status_callbk_fn)
        {
            if ((t_u32)(msg->data) == MLAN_BSS_TYPE_UAP)
            {
                wm_wifi.supp_if_callbk_fns->mgmt_tx_status_callbk_fn(wm_wifi.hapd_if_priv, resp, resp->frame.frame_len);
            }
            else
            {
                wm_wifi.supp_if_callbk_fns->mgmt_tx_status_callbk_fn(wm_wifi.if_priv, resp, resp->frame.frame_len);
            }
        }
    }
}

#endif
