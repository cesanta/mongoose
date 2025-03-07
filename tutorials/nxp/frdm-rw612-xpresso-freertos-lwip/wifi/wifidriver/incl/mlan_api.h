/** @file mlan_api.h
 *
 *  @brief MLAN Interface
 *
 *  Copyright 2008-2022, 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MLAN_API_H__
#define __MLAN_API_H__
#include <string.h>
#include <wmtypes.h>
#include <wlan.h>

#ifndef __ZEPHYR__
#include "fsl_debug_console.h"
#endif

#define MLAN_WMSDK_MAX_WPA_IE_LEN 64U
#define MLAN_MAX_MDIE_LEN         10U
#define MLAN_MAX_VENDOR_IE_LEN    100U

#include "mlan.h"
#include "mlan_join.h"
#include "mlan_util.h"
#include "mlan_fw.h"
#include "mlan_main.h"
#ifndef RW610
#include "mlan_main_defs.h"
#endif
#include "mlan_wmm.h"
#include "mlan_11n.h"
#include "mlan_11h.h"
#include "mlan_11ac.h"
#if CONFIG_11AX
#include "mlan_11ax.h"
#endif
#include "mlan_11n_aggr.h"
#ifndef RW610
#include "mlan_sdio.h"
#endif
#include "mlan_11n_rxreorder.h"
#include "mlan_meas.h"
#include "mlan_uap.h"
#include <wifi-debug.h>
#ifndef RW610
#include <wifi-sdio.h>
#endif
#include "wifi-internal.h"
#include "mlan_action.h"
#if CONFIG_11V
#include "mlan_11v.h"
#endif
#if CONFIG_11K
#include "mlan_11k.h"
#endif
/* #define CONFIG_WIFI_DEBUG */

static inline void panic(const char *msg)
{
    PRINTF("%s\r\n", msg);
    assert(0);
}

/** Wait until a condition becomes true */
#define ASSERT(cond)                                          \
    do                                                        \
    {                                                         \
        if (!(cond))                                          \
        {                                                     \
            PRINTF("ASSERT: %s: %d\r\n", __func__, __LINE__); \
            panic("Assert failed: Panic!");                   \
        }                                                     \
    } while (0)

#if CONFIG_WIFI_DEBUG
/* #define DEBUG_11N_ASSOC */
/* #define DEBUG_11N_AGGR */
/* #define DEBUG_11N_REORDERING */
#define DEBUG_MLAN
/* #define DEBUG_DEVELOPMENT */
/* #define DUMP_PACKET_MAC */
#endif /* CONFIG_WIFI_DEBUG */

#ifdef EXIT
#undef EXIT
#define EXIT(...)
#endif /* EXIT */

#ifdef DEBUG_MLAN

#ifdef PRINTM
#undef PRINTM
#define PRINTM(level, ...)                   \
    do                                       \
    {                                        \
        (void)PRINTF("[mlan] " __VA_ARGS__); \
        (void)PRINTF("\n\r");                \
    } while (0)
#else
#define PRINTM(...)
#endif /* PRINTM */

#ifdef DBG_HEXDUMP
#undef DBG_HEXDUMP
#define DBG_HEXDUMP(level, x, y, z)       \
    do                                    \
    {                                     \
        (void)PRINTF("[mlan] %s\r\n", x); \
        dump_hex(y, z);                   \
        (void)PRINTF("\r\n");             \
    } while (0)
#else
#define DBG_HEXDUMP(...)
#endif /* DBG_HEXDUMP */

#ifdef HEXDUMP
#undef HEXDUMP
#define HEXDUMP(x, y, z)                  \
    do                                    \
    {                                     \
        (void)PRINTF("[mlan] %s\r\n", x); \
        dump_hex(y, z);                   \
        (void)PRINTF("\r\n");             \
    } while (0)
#else
#define HEXDUMP(...)
#endif /* HEXDUMP */
#endif /* DEBUG_MLAN */

#define DOT11N_CFG_ENABLE_RIFS            0x08
#define DOT11N_CFG_ENABLE_GREENFIELD_XMIT (1U << 4)
#define DOT11N_CFG_ENABLE_SHORT_GI_20MHZ  (1U << 5)
#define DOT11N_CFG_ENABLE_SHORT_GI_40MHZ  (1U << 6)

#define CLOSEST_DTIM_TO_LISTEN_INTERVAL 65534

#define SDIO_DMA_ALIGNMENT 4

#define MAX_WAIT_WAKEUP_TIME 3000

/*
 * Bit 0 : Assoc Req
 * Bit 1 : Assoc Resp
 * Bit 2 : ReAssoc Req
 * Bit 3 : ReAssoc Resp
 * Bit 4 : Probe Req
 * Bit 5 : Probe Resp
 * Bit 8 : Beacon
 */
/** Mask for Assoc request frame */
#define MGMT_MASK_ASSOC_REQ 0x01
/** Mask for ReAssoc request frame */
#define MGMT_MASK_REASSOC_REQ 0x04
/** Mask for Assoc response frame */
#define MGMT_MASK_ASSOC_RESP 0x02
/** Mask for ReAssoc response frame */
#define MGMT_MASK_REASSOC_RESP 0x08
/** Mask for probe request frame */
#define MGMT_MASK_PROBE_REQ 0x10
/** Mask for probe response frame */
#define MGMT_MASK_PROBE_RESP 0x20
/** Mask for beacon frame */
#define MGMT_MASK_BEACON 0x100
/** Mask for action frame */
#define MGMT_MASK_ACTION 0x2000
/** Mask to clear previous settings */
#define MGMT_MASK_CLEAR 0x000

/* Following is allocated in mlan_register */
extern mlan_adapter *mlan_adap;

#if CONFIG_WPS2
extern int wps_session_attempt;
#endif

extern osa_rw_lock_t sleep_rwlock;

#if CONFIG_WMM_UAPSD
extern OSA_SEMAPHORE_HANDLE_DEFINE(uapsd_sem);
#endif

#if CONFIG_WMM
extern OSA_SEMAPHORE_HANDLE_DEFINE(txbuf_sem);
#endif

#if CONFIG_STA_AMPDU_RX
extern bool sta_ampdu_rx_enable;
#endif
#ifdef DUMP_PACKET_MAC
void dump_mac_addr(const char *msg, unsigned char *addr);
#endif /* DUMP_PACKET_MAC */
mlan_status wlan_setup_rates_from_bssdesc(mlan_private *pmpriv,
                                          BSSDescriptor_t *pbss_desc,
                                          t_u8 *pout_rates,
                                          t_u32 *pout_rates_size);

mlan_status wifi_prepare_and_send_cmd(IN mlan_private *pmpriv,
                                      IN t_u16 cmd_no,
                                      IN t_u16 cmd_action,
                                      IN t_u32 cmd_oid,
                                      IN t_void *pioctl_buf,
                                      IN t_void *pdata_buf,
                                      mlan_bss_type bss_type,
                                      void *priv);
#if UAP_SUPPORT
int wifi_uap_prepare_and_send_cmd(mlan_private *pmpriv,
                                  t_u16 cmd_no,
                                  t_u16 cmd_action,
                                  t_u32 cmd_oid,
                                  t_void *pioctl_buf,
                                  t_void *pdata_buf,
                                  mlan_bss_type bss_type,
                                  void *priv);
#else
static inline int wifi_uap_prepare_and_send_cmd(mlan_private *pmpriv,
                                                t_u16 cmd_no,
                                                t_u16 cmd_action,
                                                t_u32 cmd_oid,
                                                t_void *pioctl_buf,
                                                t_void *pdata_buf,
                                                mlan_bss_type bss_type,
                                                void *priv)
{
    (void)PRINTF("wifi_uap_prepare_and_send_cmd UAP not suppored %s:%d\r\n", __func__, __LINE__);
    return -1;
}
#endif

bool wmsdk_is_11N_enabled(void);

/**
 * Abort the split scan if it is in progress.
 *
 * After this call returns this scan function will abort the current split
 * scan and return back to the caller. The scan list may be incomplete at
 * this moment. There are no other side effects on the scan function apart
 * from this. The next call to scan function should proceed as normal.
 */
void wlan_abort_split_scan(void);

void wlan_scan_process_results(IN mlan_private *pmpriv);
bool wlan_use_non_default_ht_vht_cap(IN BSSDescriptor_t *pbss_desc);
bool check_for_wpa2_entp_ie(bool *wpa2_entp_IE_exist, const void *element_data, unsigned element_len);

#if CONFIG_WPA2_ENTP
bool wifi_get_scan_enable_wpa2_enterprise_ap_only();

static inline mlan_status wifi_check_bss_entry_wpa2_entp_only(BSSDescriptor_t *pbss_entry, t_u8 element_id)
{
    if (element_id == RSN_IE)
    {
        if ((wifi_get_scan_enable_wpa2_enterprise_ap_only()) &&
            (!check_for_wpa2_entp_ie(&pbss_entry->wpa2_entp_IE_exist, pbss_entry->rsn_ie_buff + 8,
                                     pbss_entry->rsn_ie_buff_len - 10)))
        {
            return MLAN_STATUS_RESOURCE;
        }
        else
        {
            check_for_wpa2_entp_ie(&pbss_entry->wpa2_entp_IE_exist, pbss_entry->rsn_ie_buff + 8,
                                   pbss_entry->rsn_ie_buff_len - 10);
        }
    }
    else if (element_id == VENDOR_SPECIFIC_221)
    {
        if (wifi_get_scan_enable_wpa2_enterprise_ap_only())
            return MLAN_STATUS_RESOURCE;
    }
    else if (!element_id)
    {
        if ((wifi_get_scan_enable_wpa2_enterprise_ap_only()) && (pbss_entry->privacy != Wlan802_11PrivFilter8021xWEP) &&
            (!pbss_entry->pwpa_ie) && (!pbss_entry->prsn_ie))
            return MLAN_STATUS_RESOURCE;
    }

    return MLAN_STATUS_SUCCESS;
}
#else
static inline mlan_status wifi_check_bss_entry_wpa2_entp_only(BSSDescriptor_t *pbss_entry,
                                                              IEEEtypes_ElementId_e element_id)
{
    if (element_id == RSN_IE)
    {
        (void)check_for_wpa2_entp_ie(&pbss_entry->wpa2_entp_IE_exist, pbss_entry->rsn_ie_buff + 8,
                                     pbss_entry->rsn_ie_buff_len - 10);
    }
    return MLAN_STATUS_SUCCESS;
}
#endif
#if CONFIG_BG_SCAN
int wifi_request_bgscan_query(mlan_private *pmpriv);
int wifi_send_scan_query(void);
void wifi_get_band(mlan_private *pmpriv, int *band);
#endif

int wifi_send_hostcmd(
    const void *cmd_buf, uint32_t cmd_buf_len, void *resp_buf, uint32_t resp_buf_len, uint32_t *reqd_resp_len);

int wifi_send_get_wpa_pmk(int mode, char *ssid);
int wifi_deauthenticate(uint8_t *bssid);
#if CONFIG_WPA_SUPP
int wifi_nxp_deauthenticate(unsigned int bss_type, const uint8_t *bssid, uint16_t reason_code);
void wifi_get_scan_table(mlan_private *pmpriv, mlan_scan_resp *pscan_resp);
#endif
int wifi_get_eeprom_data(uint32_t offset, uint32_t byte_count, uint8_t *buf);
int wifi_get_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, void *buf, unsigned int *buf_len);
int wifi_send_remain_on_channel_cmd(unsigned int bss_type, wifi_remain_on_channel_t *remain_on_channel);
int wifi_set_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t id, void *buf, unsigned int buf_len);
#ifdef SD8801
int wifi_get_ext_coex_stats(wifi_ext_coex_stats_t *ext_coex_stats);
int wifi_set_ext_coex_config(const wifi_ext_coex_config_t *ext_coex_config);
#endif
int wifi_send_enable_supplicant(int mode, const char *ssid);
int wifi_send_clear_wpa_psk(int mode, const char *ssid);
int wifi_send_add_wpa_psk(int mode, char *ssid, char *passphrase, unsigned int len);
int wifi_send_add_wpa3_password(int mode, char *ssid, char *password, unsigned int len);
int wifi_send_add_wpa_pmk(int mode, char *ssid, char *bssid, char *pmk, unsigned int len);
bool wifi_11d_is_channel_allowed(int channel);

#if CONFIG_11AX
void wifi_request_get_fw_info(mlan_private *priv, mlan_fw_info *fw_info);

#if CONFIG_MMSF
int wifi_mmsf_cfg(const t_u16 action, t_u8 *enable, t_u8 *Density, t_u8 *MMSF);
#endif
#endif

#if CONFIG_WIFI_RECOVERY
int wifi_recovery_test(void);
#endif

/**
 * Get the string representation of the wlan firmware extended version.
 *
 * @param[out] version_ext Extended Version
 *
 * @return WM_SUCCESS on success or error code.
 */
int wifi_get_firmware_version_ext(wifi_fw_version_ext_t *version_ext);
/**
 * Get the string representation of the wlan firmware version.
 *
 * @param[out] ver Version
 *
 * @return WM_SUCCESS on success or error code.
 */
int wifi_get_firmware_version(wifi_fw_version_t *ver);

#if UAP_SUPPORT
int wifi_set_smart_mode_cfg(char *ssid,
                            int beacon_period,
                            wifi_chan_list_param_set_t *chan_list,
                            uint8_t *smc_start_addr,
                            uint8_t *smc_end_addr,
                            uint16_t filter_type,
                            int smc_frame_filter_len,
                            uint8_t *smc_frame_filter,
                            int custom_ie_len,
                            uint8_t *custom_ie);
int wifi_get_smart_mode_cfg(void);
int wifi_start_smart_mode(void);
int wifi_stop_smart_mode(void);
#endif

/**
 * Get Calibration data from WLAN firmware
 *
 * \param[out] cal_data Pointer to calibration data structure where
 *	      calibration data and it's length will be stored.
 *
 * \return WM_SUCCESS if cal data read operation is successful.
 * \return -WM_FAIL if cal data field is not present or command fails.
 *
 * \note The user of this API should free the allocated buffer for
 *	 calibration data.
 *
 */
int wifi_get_cal_data(wifi_cal_data_t *cal_data);
int wifi_set_key(int bss_index,
                 bool is_pairwise,
                 const uint8_t key_index,
                 const uint8_t *key,
                 unsigned key_len,
                 const uint8_t *seq,
                 unsigned seq_len,
                 const uint8_t *mac_addr,
                 unsigned int flags);
int wifi_set_rekey_info(
    int bss_index, const t_u8 *kek, size_t kek_len, const t_u8 *kck, size_t kck_len, const t_u8 *replay_ctr);

/**
 * Get User Data from OTP Memory
 *
 * \param[in] buf Pointer to buffer where data will be stored
 * \param[in] len Number of bytes to read
 *
 * \return WM_SUCCESS if user data read operation is successful.
 * \return -WM_FAIL if user data field is not present or command fails.
 */
int wifi_get_otp_user_data(uint8_t *buf, uint16_t len);
int wifi_reg_access(wifi_reg_t reg_type, uint16_t action, uint32_t offset, uint32_t *value);
int wifi_enable_11d_support_APIs(void);
int wifi_get_tsf(uint32_t *tsf_high, uint32_t *tsf_low);
int wifi_set_igtk_key(int bss_index, const uint8_t *pn, const uint16_t key_index, const uint8_t *key, unsigned key_len);
int wifi_send_scan_cmd(t_u8 bss_mode,
                       const t_u8 *specific_bssid,
                       const char *ssid,
                       uint8_t ssid_num,
                       const t_u8 num_channels,
                       const wifi_scan_channel_list_t *chan_list,
                       const t_u8 num_probes,
#if CONFIG_SCAN_WITH_RSSIFILTER
                       const t_s16 rssi_threshold,
#endif
#if CONFIG_SCAN_CHANNEL_GAP
                       const t_u16 scan_chan_gap,
#endif
                       const bool keep_previous_scan,
                       const bool active_scan_triggered);
int wifi_remove_key(int bss_index, bool is_pairwise, const uint8_t key_index, const uint8_t *mac_addr);
int wifi_enable_ecsa_support(void);
int wifi_set_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl, int bss_type);
int wifi_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr);
int wifi_get_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl, int bss_type);
int wifi_set_pmfcfg(t_u8 mfpc, t_u8 mfpr);
int wifi_set_chanlist(wifi_chanlist_t *chanlist);
int wifi_get_txpwrlimit(wifi_SubBand_t subband, wifi_txpwrlimit_t *txpwrlimit);
int wifi_get_data_rate(wifi_ds_rate *ds_rate, mlan_bss_type bss_type);
#if UAP_SUPPORT
void wifi_get_active_channel_list(t_u8 *chan_list, t_u8 *num_chans, t_u16 acs_band);
#endif
bool wifi_is_ecsa_enabled(void);
int wifi_set_txpwrlimit(wifi_txpwrlimit_t *txpwrlimit);
int wifi_send_rssi_info_cmd(wifi_rssi_info_t *rssi_info);
void wifi_set_curr_bss_channel(uint8_t channel);
int wifi_get_chanlist(wifi_chanlist_t *chanlist);
int wifi_get_set_bandcfg(wifi_bandcfg_t *bandcfg, mlan_act_ioctl action);
#if (CONFIG_IPS)
int wifi_set_ips_config(mlan_bss_type interface, int option);
#endif
#if CONFIG_WIFI_EU_CRYPTO
int wifi_set_eu_crypto(EU_Crypto *Crypto_Data, enum _crypto_algorithm Algorithm, t_u16 EncDec);
#endif
int wifi_set_rx_mgmt_indication(unsigned int bss_type, unsigned int mgmt_subtype_mask);
mlan_status wlan_cmd_rx_mgmt_indication(IN pmlan_private pmpriv,
                                        IN HostCmd_DS_COMMAND *cmd,
                                        IN t_u16 cmd_action,
                                        IN t_void *pdata_buf);
wlan_mgmt_pkt *wifi_PrepDefaultMgtMsg(t_u8 sub_type,
                                      mlan_802_11_mac_addr *DestAddr,
                                      mlan_802_11_mac_addr *SrcAddr,
                                      mlan_802_11_mac_addr *Bssid,
                                      t_u16 pkt_len);

#if CONFIG_11MC
void wlan_location_ftm_cfg(location_cfg_info_t *ftm_location_cfg);
void wlan_civic_ftm_cfg(location_civic_rep_t *ftm_civic_cfg);
void wlan_dot11mc_ftm_cfg(void *cmd, ftm_11mc_nego_cfg_t *ftm_11mc_nego_cfg);
#endif
#if CONFIG_11AZ
void wlan_dto11az_ranging_cfg(void *cmd, const t_u8 protocl, HostCmd_FTM_SESSION_CFG *ftm_session_cfg);
#endif

#if (CONFIG_11MC) || (CONFIG_11AZ)
/* ftm ctrl params */
typedef struct
{
    int loop_cnt;
    t_u8 channel;
    t_u8 peer_mac[MLAN_MAC_ADDR_LENGTH];
    t_u8 status;
} ftm_start_param;

int wifi_unassoc_ftm_cfg(const t_u16 action, const t_u16 config);
int wifi_ftm_start_stop(const t_u16 action, const t_u8 loop_cnt, const t_u8 *mac, const t_u8 channel);
int wifi_ftm_start(const t_u16 action, const t_u8 *mac, const t_u8 channel);
int wifi_ftm_stop(const t_u16 action, const t_u8 *mac, const t_u8 channel);
int wifi_ftm_cfg(const t_u8 protocol, ranging_11az_cfg_t *ftm_ranging_cfg);
int wifi_ftm_11mc_cfg(ftm_11mc_nego_cfg_t *ftm_11mc_nego_cfg);
int wifi_ftm_location_cfg(location_cfg_info_t *ftm_location_cfg);
int wifi_ftm_civic_cfg(location_civic_rep_t *ftm_civic_cfg);
int wifi_process_wlc_ftm_event();
void wifi_ftm_process_cfg_resp(void *resp_buff);
void wifi_ftm_process_ctrl_resp(void *resp_buff);
void wifi_ftm_process_event(void *p_data);
#if CONFIG_WLS_CSI_PROC
int wifi_process_wls_csi_event(void *p_data);
void wls_csi_process_event(void *p_data);
extern t_u8 g_csi_event_for_wls;
#endif
#endif

#if UAP_SUPPORT
int wifi_set_custom_ie(custom_ie *beacon_ies_data,
                       custom_ie *beacon_wps_ies_data,
                       custom_ie *proberesp_ies_data,
                       custom_ie *assocresp_ies_data);
#endif

#if CONFIG_11K
/**
 * rrm scan callback function to process scan results
 *
 * \param[in] count the count of available scan results
 * \return WM_SUCCESS if successful otherwise failure.
 *
 */
int _wlan_rrm_scan_cb(unsigned int count);

/**
 * rrm scan request
 *
 * \param[in] wlan_scan_param the scan parameters
 * \param[in] scan_cb_param the rm scan parameters
 *
 */
void wlan_rrm_request_scan(wlan_scan_params_v2_t *wlan_scan_param, wlan_rrm_scan_cb_param *scan_cb_param);
#endif

int wrapper_bssdesc_first_set(int bss_index,
                              uint8_t *BssId,
                              bool *is_ibss_bit_set,
                              int *ssid_len,
                              uint8_t *ssid,
                              uint8_t *Channel,
                              uint8_t *RSSI,
                              uint16_t *beacon_period,
                              uint16_t *dtim_period,
                              _SecurityMode_t *WPA_WPA2_WEP,
                              _Cipher_t *wpa_mcstCipher,
                              _Cipher_t *wpa_ucstCipher,
                              _Cipher_t *rsn_mcstCipher,
                              _Cipher_t *rsn_ucstCipher,
                              t_u8 *ap_mfpc,
                              t_u8 *ap_mfpr,
                              t_u8 *ap_pwe);

int wrapper_bssdesc_second_set(int bss_index,
                               bool *phtcap_ie_present,
                               bool *phtinfo_ie_present,
#if CONFIG_11AC
                               bool *pvhtcap_ie_present,
#endif
#if CONFIG_11AX
                               bool *phecap_ie_present,
#endif
                               bool *wmm_ie_present,
                               uint16_t *band,
                               bool *wps_IE_exist,
                               uint16_t *wps_session,
                               bool *wpa2_entp_IE_exist,
#if CONFIG_11R
                               uint16_t *mdid,
#endif
#if CONFIG_11K
                               bool *neighbor_report_supported,
#endif
#if CONFIG_11V
                               bool *bss_transition_supported,
#endif
                               uint8_t *trans_mode,
                               uint8_t *trans_bssid,
                               int *trans_ssid_len,
                               uint8_t *trans_ssid
#if CONFIG_DRIVER_MBO
                               ,
                               bool *mbo_assoc_disallowed
#endif
);

int wifi_get_mgmt_ie2(mlan_bss_type bss_type, void *buf, unsigned int *buf_len);
int wifi_set_mgmt_ie2(mlan_bss_type bss_type, unsigned short mask, void *buf, unsigned int buf_len);
int wifi_clear_mgmt_ie2(mlan_bss_type bss_type, int mgmt_bitmap_index);

#if CONFIG_BG_SCAN
int wifi_request_bgscan(mlan_private *pmpriv);
#endif

#if CONFIG_WPA_SUPP
int wifi_send_sched_scan_cmd(nxp_wifi_trigger_sched_scan_t *params);
int wifi_send_stop_sched_scan_cmd(void);
#endif

#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
/*submit subscribe event cmd to firmware*/
int wifi_subscribe_event_submit(mlan_private *pmpriv, mlan_ds_subscribe_evt *sub_evt);

/*get subscribe event*/
int wifi_get_subscribe_event(mlan_private *pmpriv, mlan_ds_subscribe_evt *sub_evt);

/*disable specific subscribe event*/
int wifi_clear_subscribe_event(mlan_private *pmpriv, int evt_bitmap);

/*set subscribe event thresh_value and freq*/
int wifi_set_threshold_rssi_low(mlan_private *pmpriv, unsigned int rssi_low, unsigned int freq);
int wifi_set_threshold_rssi_high(mlan_private *pmpriv, unsigned int rssi_high, unsigned int freq);
int wifi_set_threshold_snr_low(mlan_private *pmpriv, unsigned int snr_low, unsigned int freq);
int wifi_set_threshold_snr_high(mlan_private *pmpriv, unsigned int snr_high, unsigned int freq);
int wifi_set_threshold_max_fail(mlan_private *pmpriv, unsigned int max_fail, unsigned int freq);
int wifi_set_threshold_beacon_miss(mlan_private *pmpriv, unsigned int beacon_miss, unsigned int freq);
int wifi_set_threshold_data_rssi_low(mlan_private *pmpriv, unsigned int data_rssi_low, unsigned int freq);
int wifi_set_threshold_data_rssi_high(mlan_private *pmpriv, unsigned int data_rssi_high, unsigned int freq);
int wifi_set_threshold_data_snr_low(mlan_private *pmpriv, unsigned int data_snr_low, unsigned int freq);
int wifi_set_threshold_data_snr_high(mlan_private *pmpriv, unsigned int data_snr_high, unsigned int freq);
int wifi_set_threshold_link_quality(mlan_private *pmpriv,
                                    unsigned int link_snr,
                                    unsigned int link_snr_freq,
                                    unsigned int link_rate,
                                    unsigned int link_rate_freq,
                                    unsigned int link_tx_latency,
                                    unsigned int link_tx_lantency_freq);
int wifi_set_threshold_pre_beacon_lost(mlan_private *pmpriv, unsigned int pre_beacon_lost, unsigned int freq);
#endif

#if CONFIG_TSP
int wifi_tsp_cfg(const t_u16 action,
                 t_u16 *enable,
                 t_u32 *back_off,
                 t_u32 *highThreshold,
                 t_u32 *lowThreshold,
                 t_u32 *dutycycstep,
                 t_u32 *dutycycmin,
                 int *highthrtemp,
                 int *lowthrtemp,
                 int *currCAUTemp,
                 int *currRFUTemp);
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
int wifi_tx_ampdu_prot_mode(tx_ampdu_prot_mode_para *prot_mode, t_u16 action);
#endif

#if CONFIG_EXTERNAL_COEX_PTA
int wifi_external_coex_pta_cfg(ext_coex_pta_cfg coex_pta_config);
#endif

#if CONFIG_IMD3_CFG
int wifi_imd3_cfg(t_u8 imd3_value);
#endif

int send_wifi_driver_bypass_data_event(t_u8 interface);

#if CONFIG_AUTO_RECONNECT
int wifi_auto_reconnect_enable(wifi_auto_reconnect_config_t auto_reconnect_config);
int wifi_auto_reconnect_disable(void);
int wifi_get_auto_reconnect_config(wifi_auto_reconnect_config_t *auto_reconnect_config);
#endif

#if CONFIG_INACTIVITY_TIMEOUT_EXT
int wifi_sta_inactivityto(wifi_inactivity_to_t *inac_to, t_u16 cmd_action);
#endif
#endif /* __MLAN_API_H__ */
