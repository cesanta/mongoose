/** @file rtos_wpa_supp_if.h
 *
 *  @brief  This file provides RTOS WPA Supplicant interface wifi APIs
 *
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __RTOS_WPA_SUPP_IF_H__
#define __RTOS_WPA_SUPP_IF_H__

#include <wifi_config_default.h>

#if CONFIG_WPA_SUPP
#include "includes.h"
#include "common.h"
#include "eloop.h"
#include "wpa_supplicant/config.h"
#include "wpa_supplicant_i.h"
#include "bss.h"
#include "driver_i.h"

#include <wmlog.h>
#include <wifi_nxp.h>
#include <wifi_nxp_internal.h>

#define supp_e(...) wmlog_e("supp_if", ##__VA_ARGS__)

#if CONFIG_SUPP_DEBUG
#define supp_d(...) wmlog("supp_if", ##__VA_ARGS__)
#else
#define supp_d(...)
#endif /* ! CONFIG_SUPP_DEBUG */

void *wifi_nxp_wpa_supp_dev_init(void *supp_drv_if_ctx,
                                 const char *iface_name,
                                 rtos_wpa_supp_dev_callbk_fns *supp_callbk_fns);

void wifi_nxp_wpa_supp_dev_deinit(void *if_priv);

int wifi_nxp_wpa_supp_scan2(void *if_priv, struct wpa_driver_scan_params *params);

int wifi_nxp_wpa_supp_set_default_scan_ies(void *priv, const u8 *ies, size_t ies_len);

int wifi_nxp_wpa_supp_sched_scan(void *if_priv, struct wpa_driver_scan_params *params);

int wifi_nxp_wpa_supp_stop_sched_scan(void *if_priv);

int wifi_nxp_wpa_supp_scan_abort(void *if_priv);

int wifi_nxp_wpa_supp_scan_results_get(void *if_priv, struct wpa_scan_results *scan_res2);

int wifi_nxp_wpa_supp_survey_results_get(void *if_priv);

int wifi_nxp_wpa_supp_deauthenticate(void *if_priv, const char *addr, unsigned short reason_code);

int wifi_nxp_wpa_supp_authenticate(void *if_priv, struct wpa_driver_auth_params *params, struct wpa_bss *curr_bss);

int wifi_nxp_wpa_supp_associate(void *if_priv, struct wpa_driver_associate_params *params);

int wifi_nxp_wpa_set_supp_port(void *if_priv, int authorized, char *bssid);

int wifi_nxp_wpa_supp_set_country(void *if_priv, const char *alpha2);

int wifi_nxp_wpa_supp_get_country(void *if_priv, char *alpha2);

int wifi_nxp_wpa_supp_signal_poll(void *if_priv, struct wpa_signal_info *si, unsigned char *bssid);

int wifi_nxp_wpa_send_mlme(void *if_priv,
                           const u8 *data,
                           size_t data_len,
                           int noack,
                           unsigned int freq,
                           int no_cck,
                           int offchanok,
                           unsigned int wait_time,
                           int cookie);

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
                              enum key_flag key_flag);

int wifi_nxp_wpa_supp_del_key(void *if_priv, const unsigned char *addr, int key_idx);

int wifi_nxp_wpa_supp_set_rekey_info(
    void *if_priv, const u8 *kek, size_t kek_len, const u8 *kck, size_t kck_len, const u8 *replay_ctr);

int wifi_nxp_wpa_supp_remain_on_channel(void *if_priv, unsigned int freq, unsigned int duration);

int wifi_nxp_wpa_supp_cancel_remain_on_channel(void *if_priv);

void wifi_nxp_wpa_supp_event_proc_mac_changed(void *if_priv);

void wifi_nxp_wpa_supp_event_proc_chan_list_changed(void *if_priv, const char *alpha2);

void wifi_nxp_wpa_supp_event_proc_scan_start(void *if_priv);

void wifi_nxp_wpa_supp_event_proc_scan_abort(void *if_priv);

void wifi_nxp_wpa_supp_event_proc_scan_done(void *if_priv, int aborted, int external_scan);

void wifi_nxp_wpa_supp_event_proc_scan_res(void *if_priv,
                                           nxp_wifi_event_new_scan_result_t *scan_res,
                                           unsigned int event_len,
                                           bool more_res);

void wifi_nxp_wpa_supp_event_proc_survey_res(void *if_priv,
                                             nxp_wifi_event_new_survey_result_t *survey_res,
                                             unsigned int event_len,
                                             bool more_res);

void wifi_nxp_wpa_supp_event_proc_auth_resp(void *if_priv,
                                            nxp_wifi_event_mlme_t *auth_resp_event,
                                            unsigned int event_len);

void wifi_nxp_wpa_supp_event_proc_assoc_resp(void *if_priv,
                                             nxp_wifi_assoc_event_mlme_t *assoc_resp_event,
                                             unsigned int event_len);

void wifi_nxp_wpa_supp_event_proc_deauth(void *if_priv, nxp_wifi_event_mlme_t *deauth_event, unsigned int event_len);

void wifi_nxp_wpa_supp_event_proc_disassoc(void *if_priv,
                                           nxp_wifi_event_mlme_t *disassoc_event,
                                           unsigned int event_len);

void wifi_nxp_wpa_supp_event_acs_channel_selected(void *if_priv, nxp_wifi_acs_params *acs_params);

void wifi_nxp_wpa_supp_event_proc_unprot_mgmt(void *if_priv,
                                              nxp_wifi_event_mlme_t *unprot_mgmt_event,
                                              unsigned int event_len);
void wifi_nxp_wpa_supp_event_proc_remain_on_channel(void *if_priv, int cancel_channel);
void wifi_nxp_wpa_supp_event_proc_mgmt_rx(void *if_priv, nxp_wifi_event_mlme_t *mgmt_rx, unsigned int event_len);
void wifi_nxp_wpa_supp_event_proc_eapol_rx(void *if_priv,
                                           nxp_wifi_event_eapol_mlme_t *eapol_rx,
                                           unsigned int event_len);
void wifi_nxp_wpa_supp_event_signal_change(void *if_priv);

void wifi_nxp_wpa_supp_event_mgmt_tx_status(void *if_priv, nxp_wifi_event_mlme_t *mlme_event, unsigned int event_len);

void wifi_nxp_wpa_supp_event_proc_ecsa_complete(void *if_priv, nxp_wifi_ch_switch_info *ch_switch_info);
void wifi_nxp_wpa_supp_event_proc_dfs_cac_started(void *if_priv, nxp_wifi_dfs_cac_info *dfs_cac_info);
void wifi_nxp_wpa_supp_event_proc_dfs_cac_finished(void *if_priv, nxp_wifi_dfs_cac_info *dfs_cac_info);

#if CONFIG_HOSTAPD
void *wifi_nxp_hostapd_dev_init(void *hapd_drv_if_ctx,
                                const char *iface_name,
                                rtos_hostapd_dev_callbk_fns *hostapd_callbk_fns);
void wifi_nxp_hostapd_dev_deinit(void *if_priv);
int wifi_nxp_hostapd_do_acs(void *priv, struct drv_acs_params *params);
int wifi_nxp_hostapd_set_ap(void *if_priv, int beacon_set, struct wpa_driver_ap_params *params);
int wifi_nxp_hostapd_set_modes(void *if_priv, struct hostapd_hw_modes *modes);
int wifi_nxp_hostapd_sta_add(void *if_priv, struct hostapd_sta_add_params *params);
int wifi_nxp_hostapd_sta_remove(void *if_priv, const u8 *addr);
int wifi_nxp_hostapd_send_eapol(void *if_priv, const u8 *data, size_t data_len);
int wifi_nxp_hostapd_set_freq(void *if_priv, struct hostapd_freq_params *freq);
int wifi_nxp_hostapd_set_rts(void *if_priv, int rts_threshold);
int wifi_nxp_hostapd_set_frag(void *if_priv, int frag_threshold);
int wifi_nxp_hostapd_stop_ap(void *if_priv);
int wifi_nxp_hostapd_set_acl(void *if_priv, struct hostapd_acl_params *params);
#endif /* CONFIG_HOSTAPD */
int wifi_nxp_wpa_dpp_listen(void *if_priv, bool enable);
bool wifi_nxp_wpa_get_modes(void *if_priv);
#endif /* CONFIG_WPA_SUPP */
#endif /*  __rtos_WPA_SUPP_IF_H__ */
