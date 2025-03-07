/** @file wifi-internal.h
 *
 *  @brief WLAN Internal API
 *
 *  Copyright 2008-2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __WIFI_INTERNAL_H__
#define __WIFI_INTERNAL_H__

#include <limits.h>

#if CONFIG_WPA_SUPP
#include "wifi_nxp_internal.h"
#endif

#include <mlan_api.h>

#include <osa.h>
#include <wifi_events.h>
#include <wifi-decl.h>
#if CONFIG_WPA_SUPP
#include <ieee802_11_defs.h>
#endif

/* We don't see events coming in quick succession,
 * MAX_EVENTS = 20 is fairly big value */
#define MAX_EVENTS 20

struct bus_message
{
    uint16_t event;
    uint16_t reason;
    void *data;
};

typedef struct
{
    int (*wifi_uap_downld_domain_params_p)(int band);
} wifi_uap_11d_apis_t;

typedef struct mcast_filter
{
    uint8_t mac_addr[MLAN_MAC_ADDR_LENGTH];
    struct mcast_filter *next;
} mcast_filter;

/* User response buffer parameters for hostcmd */
typedef struct _hostcmd_cfg
{
    void *resp_buf;
    uint32_t resp_buf_len;
    uint32_t *reqd_resp_len;
    bool is_hostcmd;
} hostcmd_cfg_t;

/** This enum defines various thread events
 * for which thread processing will occur */
enum wifi_thread_event_t
{
    WIFI_EVENT_STA            = 1,
    WIFI_EVENT_UAP            = 1 << 1,
    WIFI_EVENT_SDIO           = 1 << 2,
    WIFI_EVENT_SCAN           = 1 << 3,
    WIFI_EVENT_TX_DATA        = 1 << 4,
    WIFI_EVENT_TX_NULL_DATA   = 1 << 5,
    WIFI_EVENT_TX_BYPASS_DATA = 1 << 6,
};

typedef struct
{
    const uint8_t *fw_start_addr;
    size_t size;
    t_u8 wifi_init_done;
    t_u8 wifi_core_init_done;
    OSA_TASK_HANDLE_DEFINE(wifi_drv_task_Handle);
#ifdef SD9177
    OSA_TASK_HANDLE_DEFINE(wifi_pre_asleep_task_Handle);
#endif

#ifndef RW610
    OSA_TASK_HANDLE_DEFINE(wifi_core_task_Handle);
#endif
    OSA_TASK_HANDLE_DEFINE(wifi_scan_task_Handle);
#if CONFIG_WMM
    /** Thread handle for sending data */
    OSA_TASK_HANDLE_DEFINE(wifi_drv_tx_task_Handle);
#endif
    OSA_TASK_HANDLE_DEFINE(wifi_powersave_task_Handle);

    OSA_EVENT_HANDLE_DEFINE(wifi_event_Handle);

    osa_msgq_handle_t *wlc_mgr_event_queue;

    void (*data_input_callback)(const uint8_t interface, const uint8_t *buffer, const uint16_t len);
#if FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER
    void *(*wifi_get_rxbuf_desc)(t_u16 rx_len);
#endif
    void (*amsdu_data_input_callback)(uint8_t interface, uint8_t *buffer, uint16_t len);
    void (*deliver_packet_above_callback)(void *rxpd, t_u8 interface, t_void *lwip_pbuf);
    bool (*wrapper_net_is_ip_or_ipv6_callback)(const t_u8 *buffer);
#ifdef SD9177
    OSA_SEMAPHORE_HANDLE_DEFINE(command_lock);
#else
    OSA_MUTEX_HANDLE_DEFINE(command_lock);
#endif
    OSA_SEMAPHORE_HANDLE_DEFINE(command_resp_sem);

    OSA_MUTEX_HANDLE_DEFINE(mcastf_mutex);

#if CONFIG_WMM
    /** Semaphore to protect data parameters */
    OSA_SEMAPHORE_HANDLE_DEFINE(tx_data_sem);
#ifdef __ZEPHYR__
    /** Queue for sending data packets to fw */
    OSA_MSGQ_HANDLE_DEFINE(tx_data, MAX_EVENTS, sizeof(struct bus_message));
#endif
#endif
    unsigned last_sent_cmd_msec;

    /* Queue for events/data from low level interface driver */
    OSA_MSGQ_HANDLE_DEFINE(io_events, MAX_EVENTS, sizeof(struct bus_message));
#ifdef SD9177
    OSA_MSGQ_HANDLE_DEFINE(pre_asleep_events, MAX_EVENTS, sizeof(struct bus_message));
#endif
    OSA_MSGQ_HANDLE_DEFINE(powersave_queue, MAX_EVENTS, sizeof(struct bus_message));

    mcast_filter *start_list;

    /*
     * Usage note:
     * There are a number of API's (for e.g. wifi_get_antenna()) which
     * return some data in the buffer passed by the caller. Most of the
     * time this data needs to be retrived from the firmware. This
     * retrival happens in a different thread context. Hence, we need
     * to store the buffer pointer passed by the user at a shared
     * location. This pointer to used for this purpose.
     *
     * Note to the developer: Please ensure to set this to NULL after
     * use in the wifi driver thread context.
     */
    void *cmd_resp_priv;
    void *cmd_resp_ioctl;
    /*
     * In continuation with the description written for the
     * cmd_resp_priv member above, the below member indicates the
     * result of the retrieval operation from the firmware.
     */
    int cmd_resp_status;

    /*
     * Store 11D support status in Wi-Fi driver.
     */
    bool enable_11d_support;
    /*
     * 11D support callback function
     */
    wifi_uap_11d_apis_t *uap_support_11d_apis;
    /** Broadcast ssid control */
    t_u8 hidden_ssid;
    /** beacon period */
    t_u16 beacon_period;
    /** Wi-Fi Bandwidth */
    t_u8 bandwidth;
    /** channel switch time to send ECSA */
    t_u8 chan_sw_count;
    /** Sniffer channel number */
    t_u8 chan_num;
    /** ACS channel number */
    t_u8 acs_chan;
    /** HT Capability Info */
    t_u16 ht_cap_info;
    /** HTTX Cfg */
    t_u16 ht_tx_cfg;
#if CONFIG_WIFI_FW_DEBUG
    /** This function mount USB device.
     *
     * return WM_SUCCESS on success
     * return -WM_FAIL on failure.
     */
    int (*wifi_usb_mount_cb)();
    /** This function will open file for writing FW dump.
     *
     * \param[in] test_file_name Name of file to write FW dump data.
     *
     * \return WM_SUCCESS if opening of file is successful.
     * \return -WM_FAIL in case of failure.
     */
    int (*wifi_usb_file_open_cb)(char *test_file_name);
    /** This function will write data to file opened using wifi_usb_file_open_cb()
     *
     * \param[in] data Buffer containing FW dump data.
     * \param[in] data_len Length of data that needs to be written.
     *
     * \return WM_SUCCESS if write is successful
     * \return -WM_FAIL in case of failure.
     */
    int (*wifi_usb_file_write_cb)(uint8_t *data, size_t data_len);
    /** This function will close the file on which FW dump is written.
     *
     * \note This will close file that is opened using wifi_usb_file_open_cb().
     *
     * \return WM_SUCCESS on success.
     * \return -WM_FAIL on failure.
     */
    int (*wifi_usb_file_close_cb)();
#endif
    /** Structure to store the response from WiFi firmware in
     * response buffer provided by application layers
     * structure also stores lengths for usage and validation internally*/
    hostcmd_cfg_t hostcmd_cfg;
    wlan_user_scan_cfg *g_user_scan_cfg;

    bool scan_stop;
#if CONFIG_WPA_SUPP
    void *if_priv;
    void *hapd_if_priv;
    wifi_nxp_callbk_fns_t *supp_if_callbk_fns;
    nxp_wifi_event_mlme_t mgmt_resp;
    nxp_wifi_assoc_event_mlme_t assoc_resp;
    nxp_wifi_event_mlme_t mgmt_rx;
    nxp_wifi_event_eapol_mlme_t eapol_rx;
    bool wpa_supp_scan;
    bool external_scan;
#if CONFIG_HOSTAPD
    bool hostapd_op;
#endif
#endif
} wm_wifi_t;

extern wm_wifi_t wm_wifi;
extern bool split_scan_in_progress;

/* fixme: This structure seems to have been removed from mlan. This was
   copied from userif_ext.h file temporarily. Change the handling of events to
   make it similar to what mlan does */

/** Event structure */
typedef MLAN_PACK_START struct _Event_Ext_t
{
    /** No of bytes in packet including this field */
    uint16_t length;
    /** Type: Event (3) */
    uint16_t type;
    /** Event ID */
    uint16_t event_id;
    /** BSS index number for multiple BSS support */
    uint8_t bss_index;
    /** BSS type */
    uint8_t bss_type;
    /** Reason code */
    uint16_t reason_code;
    /** Source MAC address */
    uint8_t src_mac_addr[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END Event_Ext_t;

typedef MLAN_PACK_START struct _mac_address
{
    unsigned char addr[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END mac_address_t;

typedef MLAN_PACK_START struct _nxp_wifi_acl_info
{
    unsigned char acl_policy;
    unsigned int num_mac_acl;
    mac_address_t mac_acl[];
} MLAN_PACK_END nxp_wifi_acl_info_t;

/* PACK_START struct ieee80211_hdr
{
    t_u16 frame_control;
    t_u16 duration_id;
    t_u8 addr1[6];
    t_u8 addr2[6];
    t_u8 addr3[6];
    t_u16 seq_ctrl;
    t_u8 addr4[6];
} PACK_END; */

/**
 * This function handles events received from the firmware.
 */
int wifi_handle_fw_event(struct bus_message *msg);

/**
 * This function is used to send events to the upper layer through the
 * message queue registered by the upper layer.
 */
int wifi_event_completion(enum wifi_event event, enum wifi_event_reason result, void *data);

/**
 * Use this function to know whether a split scan is in progress.
 */
bool is_split_scan_complete(void);

#ifdef SD9177
/**
 * This function will handle pre asleep command response
 */
void wifi_handle_preasleep_response(void);
#endif

/**
 * Waits for Command processing to complete and waits for command response
 */
int wifi_wait_for_cmdresp(void *cmd_resp_priv);
#if CONFIG_FW_VDLL
/**
 * Waits for Command processing to complete and waits for command response for VDLL
 */
int wifi_wait_for_vdllcmdresp(void *cmd_resp_priv);
#endif
/**
 * Register an event queue
 *
 * This queue is used to send events and command responses to the wifi
 * driver from the stack dispatcher thread.
 */
int bus_register_event_queue(osa_msgq_handle_t event_queue);

/**
 * De-register the event queue.
 */
void bus_deregister_event_queue(void);

/**
 * Register DATA input function with SDIO driver.
 *
 * This queue is used to DATA frames to the wifi
 * driver from the stack dispatcher thread.
 */
int bus_register_data_input_function(int (*wifi_low_level_input)(const uint8_t interface,
                                                                 const uint8_t *buffer,
                                                                 const uint16_t len));

/**
 * De-register the DATA input function with SDIO driver.
 */
void bus_deregister_data_input_funtion(void);

/*
 * @internal
 *
 *
 */
int wifi_get_command_lock(void);

int wifi_put_command_resp_sem(void);

/*
 * @internal
 *
 *
 */
int wifi_put_command_lock(void);

#if ((CONFIG_11MC) || (CONFIG_11AZ)) && (CONFIG_WLS_CSI_PROC)
/*
 * @internal
 *
 *
 */
int wifi_get_wls_csi_sem(void);

/*
 * @internal
 *
 *
 */
int wifi_put_wls_csi_sem(void);
#endif

/*
 * Process the command reponse received from the firmware.
 *
 * Change the type of param below to HostCmd_DS_COMMAND after mlan
 * integration complete and then move it to header file.
 */
int wifi_process_cmd_response(HostCmd_DS_COMMAND *resp);

/*
 * @internal
 *
 *
 */
void *wifi_mem_malloc_cmdrespbuf(void);

/*
 * @internal
 *
 *
 */
void *wifi_malloc_eventbuf(size_t size);
void wifi_free_eventbuf(void *buffer);

#if UAP_SUPPORT
void wifi_uap_handle_cmd_resp(HostCmd_DS_COMMAND *resp);
#else
static inline void wifi_uap_handle_cmd_resp(HostCmd_DS_COMMAND *resp)
{
    (void)PRINTF("wifi_uap_handle_cmd_resp UAP not supported %s:%d\r\n", __func__, __LINE__);
}
#endif

mlan_status wrapper_moal_malloc(t_void *pmoal_handle, t_u32 size, t_u32 flag, t_u8 **ppbuf);
mlan_status wrapper_moal_mfree(t_void *pmoal_handle, t_u8 *pbuf);

#if defined(RW610)
int wifi_imu_lock(void);
void wifi_imu_unlock(void);
#else
int wifi_sdio_lock(void);
void wifi_sdio_unlock(void);
#endif

#if CONFIG_WIFI_IND_RESET
bool wifi_ind_reset_in_progress(void);
void wifi_ind_reset_start(void);
void wifi_ind_reset_stop(void);
#endif

mlan_status wrapper_wlan_cmd_mgmt_ie(int bss_type, void *buffer, unsigned int len, t_u16 action);

/**
 * This function should be called when user scan is
 * finished with success/failure.
 *
 */
void wifi_user_scan_config_cleanup(void);

/**
 * This function should be called to wait for scan task done before resetting.
 *
 */
void wifi_scan_stop(void);
int wifi_remain_on_channel(const bool status, const uint8_t channel, const uint32_t duration);
#if CONFIG_WPA_SUPP
void wpa_supp_handle_link_lost(mlan_private *priv);

int wifi_set_scan_ies(void *ie, size_t ie_len);
#if CONFIG_WPA_SUPP_WPS
bool wifi_nxp_wps_session_enable(void);
#endif

int wifi_setup_ht_cap(t_u16 *ht_capab, t_u8 *mcs_set, t_u8 *a_mpdu_params, t_u8 band);
void wifi_setup_channel_info(void *channels, int num_channels, t_u8 band);

#if CONFIG_11AC
int wifi_setup_vht_cap(t_u32 *vht_capab, t_u8 *vht_mcs_set, t_u8 band);
#endif

#if CONFIG_11AX
int wifi_setup_he_cap(nxp_wifi_he_capabilities *he_cap, t_u8 band);
#endif
int wifi_nxp_send_assoc(nxp_wifi_assoc_info_t *assoc_info);
int wifi_nxp_send_mlme(unsigned int bss_type, int channel, unsigned int wait_time, const t_u8 *data, size_t data_len);
int wifi_nxp_beacon_config(nxp_wifi_ap_info_t *params);
int wifi_set_uap_rts(int rts_threshold);
int wifi_set_uap_frag(int frag_threshold);
int wifi_nxp_sta_add(nxp_wifi_sta_info_t *params);
int wifi_nxp_sta_remove(const uint8_t *addr);
void wifi_nxp_uap_disconnect(mlan_private *priv, t_u16 reason_code, t_u8 *mac);
int wifi_nxp_stop_ap(void);
int wifi_nxp_set_acl(nxp_wifi_acl_info_t *acl_params);
int wifi_nxp_set_country(unsigned int bss_type, const char *alpha2);
int wifi_nxp_get_country(unsigned int bss_type, char *alpha2);
int wifi_nxp_get_signal(unsigned int bss_type, nxp_wifi_signal_info_t *signal_params);
int wifi_nxp_scan_res_num(void);
int wifi_nxp_scan_res_get2(t_u32 table_idx, nxp_wifi_event_new_scan_result_t *scan_res);
#endif /* CONFIG_WPA_SUPP */


#if CONFIG_WMM
int send_wifi_driver_tx_data_event(t_u8 interface);
int send_wifi_driver_tx_null_data_event(t_u8 interface);
#endif

#endif /* __WIFI_INTERNAL_H__ */
