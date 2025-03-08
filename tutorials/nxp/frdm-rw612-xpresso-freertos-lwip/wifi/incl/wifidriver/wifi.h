/*
 * Copyright 2008-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file  wifi.h
 *\brief This file provides interface for Wi-Fi driver
 */

#ifndef __WIFI_H__
#define __WIFI_H__

#include <wifi_config_default.h>

#ifdef __ZEPHYR__
#include "nxp_wifi.h"
#endif

#ifndef __ZEPHYR__
#if !CONFIG_STA_AUTO_DHCPV4
#define CONFIG_STA_AUTO_DHCPV4 1
#endif
#endif

#ifndef __ZEPHYR__
#if !CONFIG_WIFI_STA_RECONNECT
#define CONFIG_WIFI_STA_RECONNECT 1
#endif
#endif

#define CONFIG_GTK_REKEY_OFFLOAD 0


#if defined(SD9177) || defined(IW610)
#define CONFIG_TCP_ACK_ENH 1
#define CONFIG_FW_VDLL     1
#if !CONFIG_WIFI_CAPA
#undef CONFIG_WIFI_CAPA
#define CONFIG_WIFI_CAPA 1
#endif

#if CONFIG_11AX
#if !CONFIG_11K
#define CONFIG_11K 1
#endif
#if !CONFIG_11V
#define CONFIG_11V 1
#endif
#endif
#endif

#include <wifi-decl.h>
#include <wifi_events.h>
#include <osa.h>
#include <wmerrno.h>

#define WIFI_REG8(x)  (*(volatile unsigned char *)(x))
#define WIFI_REG16(x) (*(volatile unsigned short *)(x))
#define WIFI_REG32(x) (*(volatile unsigned int *)(x))

#define WIFI_WRITE_REG8(reg, val)  (WIFI_REG8(reg) = (val))
#define WIFI_WRITE_REG16(reg, val) (WIFI_REG16(reg) = (val))
#define WIFI_WRITE_REG32(reg, val) (WIFI_REG32(reg) = (val))

#ifdef RW610
#define WLAN_CAU_ENABLE_ADDR         (0x45004008U)
#define WLAN_CAU_TEMPERATURE_ADDR    (0x4500400CU)
#define WLAN_CAU_TEMPERATURE_FW_ADDR (0x41382490U)
#define WLAN_FW_WAKE_STATUS_ADDR     (0x40031068U)
#define WLAN_PMIP_TSEN_ADDR          (0x45004010U)
#define WLAN_V33_VSEN_ADDR           (0x45004028U)
#define WLAN_ADC_CTRL_ADDR           (0x45004000U)
#endif

#ifdef RW610
#define RW610_PACKAGE_TYPE_QFN 0
#define RW610_PACKAGE_TYPE_CSP 1
#define RW610_PACKAGE_TYPE_BGA 2
#endif

#define WIFI_COMMAND_RESPONSE_WAIT_MS 20000

#define BANDWIDTH_20MHZ 1U
#define BANDWIDTH_40MHZ 2U
#if CONFIG_11AC
#define BANDWIDTH_80MHZ 3U
#endif

#define MAX_NUM_CHANS_IN_NBOR_RPT 6U

/** BIT value */
#ifndef MBIT
#define MBIT(x) (((t_u32)1) << (x))
#endif

#define WIFI_MGMT_DIASSOC MBIT(10)
#define WIFI_MGMT_AUTH    MBIT(11)
#define WIFI_MGMT_DEAUTH  MBIT(12)
/** BITMAP for Action frame */
#define WIFI_MGMT_ACTION MBIT(13)

extern t_u8 wifi_tx_status;
extern t_u8 wifi_tx_block_cnt;
extern t_u8 wifi_rx_status;
extern t_u8 wifi_rx_block_cnt;

extern int16_t g_bcn_nf_last;
extern uint8_t g_rssi;
extern uint16_t g_data_nf_last;
extern uint16_t g_data_snr_last;

#if CONFIG_WIFI_RECOVERY
extern bool wifi_recovery_enable;
extern t_u16 wifi_recovery_cnt;
#endif
extern bool wifi_shutdown_enable;

/** Wi-Fi Error Code */
enum
{
    WM_E_WIFI_ERRNO_START = MOD_ERROR_START(MOD_WIFI),
    /** The Firmware download operation failed. */
    WIFI_ERROR_FW_DNLD_FAILED,
    /** The Firmware ready register not set. */
    WIFI_ERROR_FW_NOT_READY,
    /** The Wi-Fi card not found. */
    WIFI_ERROR_CARD_NOT_DETECTED,
    /** The Wi-Fi Firmware not found. */
    WIFI_ERROR_FW_NOT_DETECTED,
};

/** Wi-Fi driver TX/RX data status */
enum
{
    /** Data in running status */
    WIFI_DATA_RUNNING = 0,
    /** Data in block status */
    WIFI_DATA_BLOCK = 1,
};

typedef enum
{
    MGMT_RSN_IE = 48,
#if CONFIG_11K
    MGMT_RRM_ENABLED_CAP = 70,
#endif
    MGMT_VENDOR_SPECIFIC_221 = 221,
    MGMT_WPA_IE              = MGMT_VENDOR_SPECIFIC_221,
    MGMT_WPS_IE              = MGMT_VENDOR_SPECIFIC_221,
    MGMT_MBO_IE              = MGMT_VENDOR_SPECIFIC_221,
} IEEEtypes_ElementId_t;

typedef struct wifi_uap_client_disassoc
{
    int reason_code;
    t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
} wifi_uap_client_disassoc_t;

/**
 * Initialize Wi-Fi driver module.
 *
 * Performs SDIO init, downloads Wi-Fi Firmware, creates Wi-Fi Driver
 * and command response processor thread.
 *
 * Also creates mutex, and semaphores used in command and data synchronizations.
 *
 * \param[in] fw_start_addr address of stored Wi-Fi Firmware.
 * \param[in] size Size of Wi-Fi Firmware.
 *
 * \return WM_SUCCESS on success or -WM_FAIL on error.
 *
 */
int wifi_init(const uint8_t *fw_start_addr, const size_t size);

/**
 * Initialize Wi-Fi driver module for FCC Certification.
 *
 * Performs SDIO init, downloads Wi-Fi Firmware, creates Wi-Fi Driver
 * and command response processor thread.
 *
 * Also creates mutex, and semaphores used in command and data synchronizations.
 *
 * \param[in] fw_start_addr address of stored Manufacturing Wi-Fi Firmware.
 * \param[in] size Size of Manufacturing Wi-Fi Firmware.
 *
 * \return WM_SUCCESS on success or -WM_FAIL on error.
 *
 */
int wifi_init_fcc(const uint8_t *fw_start_addr, const size_t size);

/**
 * Deinitialize Wi-Fi driver module.
 *
 * Performs SDIO deinit, send shutdown command to Wi-Fi Firmware, deletes
 * Wi-Fi Driver and command processor thread.
 *
 * Also deletes mutex and semaphores used in command and data synchronizations.
 *
 */
void wifi_deinit(void);
/**
 * This API can be used to destroy all Wi-Fi driver tasks.
 */
void wifi_destroy_wifidriver_tasks(void);
#ifdef RW610
/**
 * This API can be used to get IMU task lock.
 */
int wifi_imu_get_task_lock(void);
/**
 * This API can be used to put IMU task lock.
 */
int wifi_imu_put_task_lock(void);
#endif
/**
 * This API can be used to judge if Wi-Fi firmware is hang.
 */
bool wifi_fw_is_hang(void);
/**
 * This API can be used to send shutdown command to FW.
 */
int wifi_send_shutdown_cmd(void);
/**
 * This API can be used to set Wi-Fi driver tx status.
 *
 * @param[in] status Status to set for TX
 */
void wifi_set_tx_status(t_u8 status);

/**
 * This API can be used to set Wi-Fi driver rx status.
 *
 * @param[in] status Status to set for RX
 *
 */
void wifi_set_rx_status(t_u8 status);

/**
 * This API can be used to reset mgmt_ie_index_bitmap.
 */
void reset_ie_index();

/**
 * Register Data callback function with Wi-Fi Driver to receive
 * DATA from SDIO.
 *
 * This callback function is used to send data received from Wi-Fi
 * firmware to the networking stack.
 *
 * @param[in] data_input_callback Function that needs to be called
 *
 * @return WM_SUCCESS
 */
int wifi_register_data_input_callback(void (*data_input_callback)(const uint8_t interface,
                                                                  const uint8_t *buffer,
                                                                  const uint16_t len));

/** Deregister Data callback function from Wi-Fi Driver */
void wifi_deregister_data_input_callback(void);

#if FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER

int wifi_register_get_rxbuf_desc_callback(void *(*wifi_get_rxbuf_desc)(t_u16 rx_len));

void wifi_deregister_get_rxbuf_desc_callback(void);

#endif

/**
 * Register Data callback function with Wi-Fi Driver to receive
 * processed AMSDU DATA from Wi-Fi driver.
 *
 * This callback function is used to send data received from Wi-Fi
 * firmware to the networking stack.
 *
 * @param[in] amsdu_data_input_callback Function that needs to be called
 *
 * @return WM_SUCESS
 *
 */
int wifi_register_amsdu_data_input_callback(void (*amsdu_data_input_callback)(uint8_t interface,
                                                                              uint8_t *buffer,
                                                                              uint16_t len));

/** Deregister Data callback function from Wi-Fi Driver */
void wifi_deregister_amsdu_data_input_callback(void);

int wifi_register_deliver_packet_above_callback(void (*deliver_packet_above_callback)(void *rxpd,
                                                                                      uint8_t interface,
                                                                                      void *lwip_pbuf));

void wifi_deregister_deliver_packet_above_callback(void);

int wifi_register_wrapper_net_is_ip_or_ipv6_callback(bool (*wrapper_net_is_ip_or_ipv6_callback)(const t_u8 *buffer));

void wifi_deregister_wrapper_net_is_ip_or_ipv6_callback(void);

#if CONFIG_WMM
int wifi_add_to_bypassq(const t_u8 interface, void *pkt, t_u32 len);
#endif

/**
 * Wi-Fi Driver low level output function.
 *
 * Data received from upper layer is passed to Wi-Fi Driver for transmission.
 *
 * \param[in] interface Interface on which DATA frame will be transmitted.
 *  0 for Station interface, 1 for uAP interface and 2 for Wi-Fi
 *  Direct interface.
 * \param[in] buffer A pointer pointing to DATA frame.
 * \param[in] len Length of DATA frame.
 * \param[in] pkt_prio Priority for.sending packet.
 * \param[in] tid TID for tx.
 *
 * \return WM_SUCCESS on success or -WM_E_NOMEM if memory is not available
 *  or -WM_E_BUSY if SDIO is busy.
 *
 */
int wifi_low_level_output(const uint8_t interface,
                          const uint8_t *buffer,
                          const uint16_t len
#if CONFIG_WMM
                          ,
                          uint8_t pkt_prio,
                          uint8_t tid
#endif
);

/**
 * API to enable packet retries at Wi-Fi driver level.
 *
 * This API sets retry count which will be used by Wi-Fi driver to retry packet
 * transmission in case there was failure in earlier attempt. Failure may
 * happen due to SDIO write port un-availability or other failures in SDIO
 * write operation.
 *
 * \note Default value of retry count is zero.
 *
 * \param[in] count No of retry attempts.
 *
 */
void wifi_set_packet_retry_count(const int count);

/**
 * This API can be used to enable AMPDU support on the go
 * when station is a transmitter.
 */
void wifi_sta_ampdu_tx_enable(void);

/**
 * This API can be used to disable AMPDU support on the go
 * when station is a transmitter.
 */
void wifi_sta_ampdu_tx_disable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when station is a transmitter.
 * @param[in] tid tid value
 */
void wifi_sta_ampdu_tx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when station is a transmitter.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_sta_ampdu_tx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to enable AMPDU support on the go
 * when station is a receiver.
 */
void wifi_sta_ampdu_rx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when station is a receiver.
 * @param[in] tid tid value
 */
void wifi_sta_ampdu_rx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when station is a receiver.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_sta_ampdu_rx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to enable AMPDU support on the go
 * when uap is a receiver.
 */
void wifi_uap_ampdu_rx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when uap is a receiver.
 * @param[in] tid tid value
 */
void wifi_uap_ampdu_rx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when uap is a receiver.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_uap_ampdu_rx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to disable AMPDU support on the go
 * when uap is a receiver.
 */
void wifi_uap_ampdu_rx_disable(void);

/**
 * This API can be used to enable AMPDU support on the go
 * when uap is a transmitter.
 */
void wifi_uap_ampdu_tx_enable(void);

/**
 * This API can be used to set tid to enable AMPDU support on the go
 * when uap is a transmitter.
 * @param[in] tid tid value
 */
void wifi_uap_ampdu_tx_enable_per_tid(t_u8 tid);

/**
 * This API can be used to check if tid to enable AMPDU is allowed
 * when uap is a transmitter.
 * @param[in] tid tid value
 * @return MTRUE or MFALSE
 */
t_u8 wifi_uap_ampdu_tx_enable_per_tid_is_allowed(t_u8 tid);

/**
 * This API can be used to disable AMPDU support on the go
 * when uap is a transmitter.
 */
void wifi_uap_ampdu_tx_disable(void);

/**
 * This API can be used to disable AMPDU support on the go
 * when station is a receiver.
 */
void wifi_sta_ampdu_rx_disable(void);

/**
 * Get the device sta MAC address
 *
 * @param[out] mac_addr Mac address
 *
 * @return WM_SUCESS
 */
int wifi_get_device_mac_addr(wifi_mac_addr_t *mac_addr);

/**
 * Get the device uap MAC address
 *
 * @param[out] mac_addr_uap Mac address
 *
 * @return WM_SUCESS
 */
int wifi_get_device_uap_mac_addr(wifi_mac_addr_t *mac_addr_uap);

/**
 * Get the cached string representation of the wlan firmware extended version.
 *
 * @param[in] fw_ver_ext Firmware Version Extended
 *
 * @return WM_SUCCESS
 */
int wifi_get_device_firmware_version_ext(wifi_fw_version_ext_t *fw_ver_ext);

/**
 * Get the timestamp of the last command sent to the firmware
 *
 * @return Timestamp in millisec of the last command sent
 */
unsigned wifi_get_last_cmd_sent_ms(void);

uint32_t wifi_get_value1(void);

uint8_t *wifi_get_outbuf(uint32_t *outbuf_len);

#if CONFIG_WIFI_TX_PER_TRACK
int wifi_set_tx_pert(void *cfg, mlan_bss_type bss_type);
#endif

#if CONFIG_TX_RX_HISTOGRAM
int wifi_set_txrx_histogram(void *cfg, t_u8 *data);
#endif

#if CONFIG_ROAMING
int wifi_config_roaming(const int enable, uint8_t *rssi_low);
#endif
#if CONFIG_BG_SCAN
int wifi_config_bgscan_and_rssi(const char *ssid);
mlan_status wifi_stop_bgscan();
#endif

/**
 * This will update the last command sent variable value to current
 * time. This is used for power management.
 */
void wifi_update_last_cmd_sent_ms(void);

/**
 * Register an event queue with the Wi-Fi driver to receive events
 *
 * The list of events which can be received from the Wi-Fi driver are
 * enumerated in the file wifi_events.h
 *
 * @param[in] event_queue The queue to which Wi-Fi driver will post events.
 *
 * @note Only one queue can be registered. If the registered queue needs to
 * be changed unregister the earlier queue first.
 *
 * @return Standard SDK return codes
 */
int wifi_register_event_queue(osa_msgq_handle_t event_queue);

/**
 * Unregister an event queue from the Wi-Fi driver.
 *
 * @param[in] event_queue The queue to which was registered earlier with
 * the Wi-Fi driver.
 *
 * @return Standard SDK return codes
 */
int wifi_unregister_event_queue(osa_msgq_handle_t event_queue);

/** Get scan list
 *
 * @param[in] index Index
 * @param[out] desc Descriptor of type \ref wifi_scan_result2
 *
 * @return WM_SUCCESS on success or error code.
 *
 */
int wifi_get_scan_result(unsigned int index, struct wifi_scan_result2 **desc);

/**
 * Get the count of elements in the scan list
 *
 * @param[in,out] count Pointer to a variable which will hold the count after
 * this call returns
 *
 * @warning The count returned by this function is the current count of the
 * elements. A scan command given to the driver or some other background
 * event may change this count in the Wi-Fi driver. Thus when the API
 * \ref wifi_get_scan_result is used to get individual elements of the scan
 * list, do not assume that it will return exactly 'count' number of
 * elements. Your application should not consider such situations as a
 * major event.
 *
 * @return Standard SDK return codes.
 */
int wifi_get_scan_result_count(unsigned *count);

#if CONFIG_RX_ABORT_CFG
/**
 * Set/Get Rx abort config
 *
 * \param[in] cfg User configurations.
 * \param[in] action Set/Get
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_get_rx_abort_cfg(void *cfg, t_u16 action);
#endif

#if CONFIG_RX_ABORT_CFG_EXT
/**
 * Set/Get Rx abort config ext
 *
 * \param[in] cfg User configurations.
 * \param[in] action Set/Get
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_get_rx_abort_cfg_ext(void *cfg, t_u16 action);
#endif

#if CONFIG_CCK_DESENSE_CFG
/**
 * Set/Get CCK Desense config
 *
 * \param[in] cfg User configurations.
 * \param[in] action Set/Get
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_get_cck_desense_cfg(void *cfg, t_u16 action);
#endif

#ifdef WLAN_LOW_POWER_ENABLE
void wifi_enable_low_pwr_mode();
#endif

/** Set Wi-Fi calibration data in firmware.
 *
 * This function may be used to set Wi-Fi calibration data in firmware.
 *
 * @param[in] cdata The calibration data
 * @param[in] clen Length of calibration data
 *
 */
void wifi_set_cal_data(const uint8_t *cdata, const unsigned int clen);

/** Set Wi-Fi MAC address in firmware at load time.
 *
 * This function may be used to set Wi-Fi MAC address in firmware.
 *
 * @param[in] mac The new MAC Address
 *
 */
void wifi_set_mac_addr(uint8_t *mac);

/** Set Wi-Fi MAC address in firmware at run time.
 *
 * This function may be used to set Wi-Fi MAC address in firmware as per passed bss type.
 *
 * @param[in] mac The new MAC Address
 * @param[in] bss_type BSS Type
 *
 */
void _wifi_set_mac_addr(const uint8_t *mac, mlan_bss_type bss_type);

#if CONFIG_WMM_UAPSD
int wifi_wmm_qos_cfg(t_u8 *qos_cfg, t_u8 action);
int wifi_sleep_period(unsigned int *sleep_period, int action);
#endif

#if CONFIG_WIFI_TX_BUFF
/**
 * Check whether the tx buffer size setting is reasonable.
 *
 * \param[in]   buf_size The tx buffer size
 *
 */
bool wifi_calibrate_tx_buf_size(uint16_t buf_size);
void wifi_recfg_tx_buf_size(uint16_t buf_size);
void _wifi_recfg_tx_buf_size(uint16_t buf_size, mlan_bss_type bss_type);
#endif

int wifi_get_wpa_ie_in_assoc(uint8_t *wpa_ie);

/** Add Multicast Filter by MAC Address
 *
 * Multicast filters should be registered with the Wi-Fi driver for IP-level
 * multicast addresses to work. This API allows for registration of such filters
 * with the Wi-Fi driver.
 *
 * If multicast-mapped MAC address is 00:12:23:34:45:56 then pass mac_addr as
 * below:
 * mac_add[0] = 0x00
 * mac_add[1] = 0x12
 * mac_add[2] = 0x23
 * mac_add[3] = 0x34
 * mac_add[4] = 0x45
 * mac_add[5] = 0x56
 *
 * \param[in] mac_addr multicast mapped MAC address
 *
 * \return 0 on Success or else Error
 */
int wifi_add_mcast_filter(uint8_t *mac_addr);

/** Remove Multicast Filter by MAC Address
 *
 * This function removes multicast filters for the given multicast-mapped
 * MAC address. If multicast-mapped MAC address is 00:12:23:34:45:56
 * then pass mac_addr as below:
 * mac_add[0] = 0x00
 * mac_add[1] = 0x12
 * mac_add[2] = 0x23
 * mac_add[3] = 0x34
 * mac_add[4] = 0x45
 * mac_add[5] = 0x56
 *
 * \param[in] mac_addr multicast mapped MAC address
 *
 * \return  0 on Success or else Error
 */
int wifi_remove_mcast_filter(uint8_t *mac_addr);

/** Get Multicast Mapped Mac address from IPv4
 *
 * This function will generate Multicast Mapped MAC address from IPv4
 * Multicast Mapped MAC address will be in following format:
 * 1) Higher 24-bits filled with IANA Multicast OUI (01-00-5E)
 * 2) 24th bit set as Zero
 * 3) Lower 23-bits filled with IP address (ignoring higher 9bits).
 *
 * \param[in] ipaddr ipaddress(input)
 * \param[in] mac_addr multicast mapped MAC address(output)
 *
 */
void wifi_get_ipv4_multicast_mac(uint32_t ipaddr, uint8_t *mac_addr);

#if CONFIG_IPV6
/** Get Multicast Mapped Mac address from IPv6 address
 *
 * This function will generate Multicast Mapped MAC address from IPv6 address.
 * Multicast Mapped MAC address will be in following format:
 * 1) Higher 16-bits filled with IANA Multicast OUI (33-33)
 * 2) Lower 32-bits filled with last 4 bytes of IPv6 address
 *
 * \param[in] ipaddr last 4 bytes of IPv6 address
 * \param[in] mac_addr multicast mapped MAC address
 *
 */
void wifi_get_ipv6_multicast_mac(uint32_t ipaddr, uint8_t *mac_addr);
#endif /* CONFIG_IPV6 */


#ifndef RW610
int wifi_set_antenna(t_u32 ant_mode, t_u16 evaluate_time);
int wifi_get_antenna(t_u32 *ant_mode, t_u16 *evaluate_time, t_u16 *current_antenna);
#else
int wifi_set_antenna(t_u32 ant_mode, t_u16 evaluate_time, t_u8 evaluate_mode);
int wifi_get_antenna(t_u32 *ant_mode, t_u16 *evaluate_time, t_u8 *evaluate_mode, t_u16 *current_antenna);
#endif

void wifi_process_hs_cfg_resp(t_u8 *cmd_res_buffer);
enum wifi_event_reason wifi_process_ps_enh_response(t_u8 *cmd_res_buffer, t_u16 *ps_event, t_u16 *action);

typedef enum
{
    REG_MAC = 1,
    REG_BBP,
    REG_RF,
    REG_CAU
} wifi_reg_t;

int wifi_mem_access(uint16_t action, uint32_t addr, uint32_t *value);
/*
 * This function is supposed to be called after scan is complete from wlc
 * manager.
 */
void wifi_scan_process_results(void);

/**
 * Get the Wi-Fi region code
 *
 * This function will return one of the following values in the region_code
 * variable.\n
 * 0x10 : US FCC\n
 * 0x20 : CANADA\n
 * 0x30 : EU\n
 * 0x32 : FRANCE\n
 * 0x40 : JAPAN\n
 * 0x41 : JAPAN\n
 * 0x50 : China\n
 * 0xfe : JAPAN\n
 * 0xff : Special\n
 *
 * @param[out] region_code Region Code
 *
 * @return Standard WMSDK return codes.
 */
int wifi_get_region_code(t_u32 *region_code);

/**
 * Set the Wi-Fi region code.
 *
 * This function takes one of the values from the following array.\n
 * 0x10 : US FCC\n
 * 0x20 : CANADA\n
 * 0x30 : EU\n
 * 0x32 : FRANCE\n
 * 0x40 : JAPAN\n
 * 0x41 : JAPAN\n
 * 0x50 : China\n
 * 0xfe : JAPAN\n
 * 0xff : Special\n
 *
 * @param[in] region_code Region Code
 *
 * @return Standard WMSDK return codes.
 */
int wifi_set_region_code(t_u32 region_code);

/**
 * Set/Get country code
 * \param[in] alpha2 country code in 3bytes string, 2bytes country code and 1byte 0
 *            WW : World Wide Safe
 *            US : US FCC
 *            CA : IC Canada
 *            SG : Singapore
 *            EU : ETSI
 *            AU : Australia
 *            KR : Republic Of Korea
 *            FR : France
 *            JP : Japan
 *            CN : China
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_country_code(const char *alpha2);
int wifi_get_country_code(char *alpha2);
int wifi_set_country_ie_ignore(uint8_t *ignore);

bool wifi_11d_is_channel_allowed(int channel);
wifi_sub_band_set_t *get_sub_band_from_region_code(int region_code, t_u8 *nr_sb);
#if CONFIG_5GHz_SUPPORT
wifi_sub_band_set_t *get_sub_band_from_region_code_5ghz(int region_code, t_u8 *nr_sb);
#endif

int wifi_enable_11d_support();
int wifi_disable_11d_support();

#ifdef OTP_CHANINFO
int wifi_get_fw_region_and_cfp_tables(void);
void wifi_free_fw_region_and_cfp_tables(void);
#endif
#if CONFIG_COMPRESS_TX_PWTBL
int wifi_set_region_power_cfg(const t_u8 *data, t_u16 len);
#endif
int wifi_set_txbfcap(unsigned int tx_bf_cap);
int wifi_set_htcapinfo(unsigned int htcapinfo);
int wifi_set_httxcfg(unsigned short httxcfg);
int wifi_get_tx_power(t_u32 *power_level);
int wifi_set_tx_power(t_u32 power_level);
int wrapper_wlan_cmd_get_hw_spec(void);
/* fixme: These need to be removed later after complete mlan integration */
void set_event_chanswann(void);
void clear_event_chanswann(void);
void wifi_set_ps_cfg(t_u16 multiple_dtims,
                     t_u16 bcn_miss_timeout,
                     t_u16 local_listen_interval,
                     t_u16 adhoc_wake_period,
                     t_u16 mode,
                     t_u16 delay_to_ps);
int wifi_send_hs_cfg_cmd(mlan_bss_type interface, t_u32 ipv4_addr, t_u16 action, t_u32 conditions);
#if CONFIG_HOST_SLEEP
int wifi_cancel_host_sleep(mlan_bss_type interface);
#endif
bool wrapper_wlan_11d_support_is_enabled(void);
void wrapper_wlan_11d_clear_parsedtable(void);
void wrapper_clear_media_connected_event(void);
int wifi_enter_ieee_power_save(void);
int wifi_exit_ieee_power_save(void);
#if (CONFIG_WNM_PS)
int wifi_enter_wnm_power_save(t_u16 wnm_sleep_time);
int wifi_exit_wnm_power_save(void);
#endif
int wifi_enter_deepsleep_power_save(void);
int wifi_exit_deepsleep_power_save(void);
int wifi_set_power_save_mode(void);
int wifi_get_wakeup_reason(t_u16 *hs_wakeup_reason);
void send_sleep_confirm_command(mlan_bss_type interface);

#ifdef SD9177
void prepare_error_sleep_confirm_command(mlan_bss_type interface);
#endif

void wifi_configure_listen_interval(int listen_interval);
void wifi_configure_delay_to_ps(unsigned int timeout_ms);
void wifi_configure_idle_time(unsigned int timeout_ms);
unsigned short wifi_get_listen_interval();
unsigned int wifi_get_delay_to_ps();
unsigned int wifi_get_idle_time();
void wifi_configure_null_pkt_interval(unsigned int null_pkt_interval);
int wrapper_wifi_assoc(
    const unsigned char *bssid, int wlan_security, bool is_wpa_tkip, unsigned int owe_trans_mode, bool is_ft);
bool wifi_get_xfer_pending(void);
void wifi_set_xfer_pending(bool xfer_val);
int wrapper_wlan_cmd_11n_ba_stream_timeout(void *saved_event_buff);

int wifi_set_txratecfg(wifi_ds_rate ds_rate, mlan_bss_type bss_type);
int wifi_get_txratecfg(wifi_ds_rate *ds_rate, mlan_bss_type bss_type);
void wifi_wake_up_card(uint32_t *resp);
void wifi_tx_card_awake_lock(void);
void wifi_tx_card_awake_unlock(void);
#ifdef RW610
uint32_t wifi_get_board_type();
#endif
#if CONFIG_WPA2_ENTP
void wifi_scan_enable_wpa2_enterprise_ap_only();
#endif

int wrapper_wlan_11d_enable(t_u32 state);

int wifi_11h_enable(void);

int wrapper_wlan_cmd_11n_addba_rspgen(void *saved_event_buff);

int wrapper_wlan_cmd_11n_delba_rspgen(void *saved_event_buff);

int wrapper_wlan_ecsa_enable(void);

int wrapper_wlan_sta_ampdu_enable(
#if CONFIG_WMM
    t_u8 tid
#endif
);


#if CONFIG_WIFI_GET_LOG
/** Wi-Fi Statistics counter */
typedef PACK_START struct
{
    /** Multicast transmitted frame count */
    t_u32 mcast_tx_frame;
    /** Failure count */
    t_u32 failed;
    /** Retry count */
    t_u32 retry;
    /** Multi entry count */
    t_u32 multi_retry;
    /** Duplicate frame count */
    t_u32 frame_dup;
    /** RTS success count */
    t_u32 rts_success;
    /** RTS failure count */
    t_u32 rts_failure;
    /** Ack failure count */
    t_u32 ack_failure;
    /** Rx fragmentation count */
    t_u32 rx_frag;
    /** Multicast Tx frame count */
    t_u32 mcast_rx_frame;
    /** FCS error count */
    t_u32 fcs_error;
    /** Tx frame count */
    t_u32 tx_frame;
    /** WEP ICV error count */
    t_u32 wep_icv_error[4];
    /** beacon recv count */
    t_u32 bcn_rcv_cnt;
    /** beacon miss count */
    t_u32 bcn_miss_cnt;
    /** received amsdu count*/
    t_u32 amsdu_rx_cnt;
    /** received msdu count in amsdu*/
    t_u32 msdu_in_rx_amsdu_cnt;
    /** tx amsdu count*/
    t_u32 amsdu_tx_cnt;
    /** tx msdu count in amsdu*/
    t_u32 msdu_in_tx_amsdu_cnt;
    /** Tx frag count */
    t_u32 tx_frag_cnt;
    /** Qos Tx frag count */
    t_u32 qos_tx_frag_cnt[8];
    /** Qos failed count */
    t_u32 qos_failed_cnt[8];
    /** Qos retry count */
    t_u32 qos_retry_cnt[8];
    /** Qos multi retry count */
    t_u32 qos_multi_retry_cnt[8];
    /** Qos frame dup count */
    t_u32 qos_frm_dup_cnt[8];
    /** Qos rts success count */
    t_u32 qos_rts_suc_cnt[8];
    /** Qos rts failure count */
    t_u32 qos_rts_failure_cnt[8];
    /** Qos ack failure count */
    t_u32 qos_ack_failure_cnt[8];
    /** Qos Rx frag count */
    t_u32 qos_rx_frag_cnt[8];
    /** Qos Tx frame count */
    t_u32 qos_tx_frm_cnt[8];
    /** Qos discarded frame count */
    t_u32 qos_discarded_frm_cnt[8];
    /** Qos mpdus Rx count */
    t_u32 qos_mpdus_rx_cnt[8];
    /** Qos retry rx count */
    t_u32 qos_retries_rx_cnt[8];
    /** CMACICV errors count */
    t_u32 cmacicv_errors;
    /** CMAC replays count */
    t_u32 cmac_replays;
    /** mgmt CCMP replays count */
    t_u32 mgmt_ccmp_replays;
    /** TKIP ICV errors count */
    t_u32 tkipicv_errors;
    /** TKIP replays count */
    t_u32 tkip_replays;
    /** CCMP decrypt errors count */
    t_u32 ccmp_decrypt_errors;
    /** CCMP replays count */
    t_u32 ccmp_replays;
    /** Tx amsdu count */
    t_u32 tx_amsdu_cnt;
    /** failed amsdu count */
    t_u32 failed_amsdu_cnt;
    /** retry amsdu count */
    t_u32 retry_amsdu_cnt;
    /** multi-retry amsdu count */
    t_u32 multi_retry_amsdu_cnt;
    /** Tx octets in amsdu count */
    t_u64 tx_octets_in_amsdu_cnt;
    /** amsdu ack failure count */
    t_u32 amsdu_ack_failure_cnt;
    /** Rx amsdu count */
    t_u32 rx_amsdu_cnt;
    /** Rx octets in amsdu count */
    t_u64 rx_octets_in_amsdu_cnt;
    /** Tx ampdu count */
    t_u32 tx_ampdu_cnt;
    /** tx mpdus in ampdu count */
    t_u32 tx_mpdus_in_ampdu_cnt;
    /** tx octets in ampdu count */
    t_u64 tx_octets_in_ampdu_cnt;
    /** ampdu Rx count */
    t_u32 ampdu_rx_cnt;
    /** mpdu in Rx ampdu count */
    t_u32 mpdu_in_rx_ampdu_cnt;
    /** Rx octets ampdu count */
    t_u64 rx_octets_in_ampdu_cnt;
    /** ampdu delimiter CRC error count */
    t_u32 ampdu_delimiter_crc_error_cnt;
    /** Rx Stuck Related Info*/
    /** Rx Stuck Issue count */
    t_u32 rx_stuck_issue_cnt[2];
    /** Rx Stuck Recovery count */
    t_u32 rx_stuck_recovery_cnt;
    /** Rx Stuck TSF */
    t_u64 rx_stuck_tsf[2];
    /** Tx Watchdog Recovery Related Info */
    /** Tx Watchdog Recovery count */
    t_u32 tx_watchdog_recovery_cnt;
    /** Tx Watchdog TSF */
    t_u64 tx_watchdog_tsf[2];
    /** Channel Switch Related Info */
    /** Channel Switch Announcement Sent */
    t_u32 channel_switch_ann_sent;
    /** Channel Switch State */
    t_u32 channel_switch_state;
    /** Register Class */
    t_u32 reg_class;
    /** Channel Number */
    t_u32 channel_number;
    /** Channel Switch Mode */
    t_u32 channel_switch_mode;
    /** Reset Rx Mac Recovery Count */
    t_u32 rx_reset_mac_recovery_cnt;
    /** ISR2 Not Done Count*/
    t_u32 rx_Isr2_NotDone_Cnt;
    /** GDMA Abort Count */
    t_u32 gdma_abort_cnt;
    /** Rx Reset MAC Count */
    t_u32 g_reset_rx_mac_cnt;
    // Ownership error counters
    /** Error Ownership error count*/
    t_u32 dwCtlErrCnt;
    /** Control Ownership error count*/
    t_u32 dwBcnErrCnt;
    /** Control Ownership error count*/
    t_u32 dwMgtErrCnt;
    /** Control Ownership error count*/
    t_u32 dwDatErrCnt;
    /** BIGTK MME good count*/
    t_u32 bigtk_mmeGoodCnt;
    /** BIGTK Replay error count*/
    t_u32 bigtk_replayErrCnt;
    /** BIGTK MIC error count*/
    t_u32 bigtk_micErrCnt;
    /** BIGTK MME not included count*/
    t_u32 bigtk_mmeNotFoundCnt;
    /** RX unicast count */
    t_u32 rx_unicast_cnt;
    /** TX Buffer Overrun Dropped Count */
    t_u32 tx_overrun_cnt;
    /** RX Buffer Overrun Dropped Count */
    t_u32 rx_overrun_cnt;
} PACK_END wifi_pkt_stats_t;

int wifi_get_log(wifi_pkt_stats_t *stats, mlan_bss_type bss_type);
#endif

int wifi_set_packet_filters(wifi_flt_cfg_t *flt_cfg);

int wifi_get_data_rate(wifi_ds_rate *ds_rate, mlan_bss_type bss_type);

#if CONFIG_WIFI_RTS_THRESHOLD
int wifi_set_rts(int rts, mlan_bss_type bss_type);
#endif

#if CONFIG_WIFI_FRAG_THRESHOLD
int wifi_set_frag(int frag, mlan_bss_type bss_type);
#endif

#if CONFIG_11R
bool wifi_same_ess_ft();
#endif


#if CONFIG_11K
#define BEACON_REPORT_BUF_SIZE 1400

/* Reporting Detail values */
enum wlan_rrm_beacon_reporting_detail
{
    WLAN_RRM_REPORTING_DETAIL_NONE                    = 0,
    WLAN_RRM_REPORTING_DETAIL_AS_REQUEST              = 1,
    WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS = 2,
};

typedef struct _wlan_rrm_beacon_report_data
{
    t_u8 token;
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
    t_u8 ssid_length;
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    t_u8 channel[MAX_CHANNEL_LIST];
    t_u8 channel_num;
    t_u8 last_ind;
    t_u16 duration;
    enum wlan_rrm_beacon_reporting_detail report_detail;
    t_u8 bits_field[32];
} wlan_rrm_beacon_report_data;

typedef struct _wlan_rrm_scan_cb_param
{
    wlan_rrm_beacon_report_data rep_data;
    t_u8 dialog_tok;
    t_u8 dst_addr[MLAN_MAC_ADDR_LENGTH];
    t_u8 protect;
} wlan_rrm_scan_cb_param;

int wifi_host_11k_cfg(int enable_11k);

int wifi_host_11k_neighbor_req(const char *ssid);
#endif

#if CONFIG_11V
int wifi_host_11v_bss_trans_query(t_u8 query_reason);
#endif

#if (CONFIG_11K) || (CONFIG_11V)
/* Neighbor List Mode values */
enum wlan_nlist_mode
{
#if (CONFIG_11K)
    WLAN_NLIST_11K = 1,
#endif
#if (CONFIG_11V)
    WLAN_NLIST_11V           = 2,
    WLAN_NLIST_11V_PREFERRED = 3,
#endif
};

#define MAX_NEIGHBOR_AP_LIMIT 6U

typedef struct _wlan_rrm_neighbor_ap_t
{
    char ssid[MLAN_MAX_SSID_LENGTH];
    t_u8 bssid[MLAN_MAX_SSID_LENGTH];
    t_u8 bssidInfo[32];
    int op_class;
    int channel;
    int phy_type;
    int freq;
} wlan_rrm_neighbor_ap_t;

typedef struct _wlan_neighbor_report_t
{
    wlan_rrm_neighbor_ap_t neighbor_ap[MAX_NEIGHBOR_AP_LIMIT];
    int neighbor_cnt;
} wlan_rrm_neighbor_report_t;

typedef struct _wlan_nlist_report_param
{
    enum wlan_nlist_mode nlist_mode;
    t_u8 num_channels;
    t_u8 channels[MAX_NUM_CHANS_IN_NBOR_RPT];
#if (CONFIG_11V)
    t_u8 btm_mode;
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    t_u8 dialog_token;
    t_u8 dst_addr[MLAN_MAC_ADDR_LENGTH];
    t_u8 protect;
#endif
} wlan_nlist_report_param;
#endif

int wifi_clear_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, int mgmt_bitmap_index);

#if CONFIG_UAP_STA_MAC_ADDR_FILTER
int wifi_set_sta_mac_filter(int filter_mode, int mac_count, unsigned char *mac_addr);
#endif

int wifi_set_auto_arp(t_u32 *ipv4_addr);

int wifi_tcp_keep_alive(wifi_tcp_keep_alive_t *keep_alive, t_u8 *src_mac, t_u32 src_ip);


#if CONFIG_CLOUD_KEEP_ALIVE
int wifi_cloud_keep_alive(wifi_cloud_keep_alive_t *keep_alive, t_u16 action, t_u8 *enable);
#endif

#if CONFIG_HOST_SLEEP
int wifi_set_packet_filters(wifi_flt_cfg_t *flt_cfg);
int wakelock_get(void);
int wakelock_put(void);
int wakelock_isheld(void);
void wifi_print_wakeup_reason(t_u16 hs_wakeup_reason);
void wifi_clear_wakeup_reason(void);
#endif

int wifi_raw_packet_send(const t_u8 *packet, t_u32 length);

int wifi_raw_packet_recv(t_u8 **data, t_u32 *pkt_type);

#if CONFIG_11AX
int wifi_set_11ax_tx_omi(const mlan_bss_type bss_type,
                         const t_u16 tx_omi,
                         const t_u8 tx_option,
                         const t_u8 num_data_pkts);
int wifi_set_11ax_tol_time(const t_u32 tol_time);
int wifi_set_11ax_rutxpowerlimit(const void *rutx_pwr_cfg, uint32_t rutx_pwr_cfg_len);
int wifi_set_11ax_rutxpowerlimit_legacy(const wifi_rutxpwrlimit_t *ru_pwr_cfg);
int wifi_get_11ax_rutxpowerlimit_legacy(wifi_rutxpwrlimit_t *ru_pwr_cfg);
/** Set 11ax config params
 *
 * \param[in, out] ax_config 11AX config parameters to be sent to Firmware
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_11ax_cfg(wifi_11ax_config_t *ax_config);

#if CONFIG_11AX_TWT
/** Set btwt config params
 *
 * \param[in] btwt_config Broadcast TWT setup parameters to be sent to Firmware
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_btwt_cfg(const wifi_btwt_config_t *btwt_config);

/** Set twt setup config params
 *
 * \param[in] twt_setup TWT Setup parameters to be sent to Firmware
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_twt_setup_cfg(const wifi_twt_setup_config_t *twt_setup);

/** Set twt teardown config params
 *
 * \param[in] teardown_config TWT Teardown parameters to be sent to Firmware
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_twt_teardown_cfg(const wifi_twt_teardown_config_t *teardown_config);

/** Get twt report
 *
 * \param[out] twt_report TWT Report parameters to be sent to Firmware
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_get_twt_report(wifi_twt_report_t *twt_report);

#endif /* CONFIG_11AX_TWT */
#endif

#if CONFIG_WIFI_CLOCKSYNC
int wifi_set_clocksync_cfg(const wifi_clock_sync_gpio_tsf_t *tsf_latch, mlan_bss_type bss_type);
int wifi_get_tsf_info(wifi_tsf_info_t *tsf_info);
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_RF_TEST_MODE

int wifi_set_rf_test_mode(void);

int wifi_unset_rf_test_mode(void);

int wifi_set_rf_channel(const uint8_t channel);

int wifi_set_rf_radio_mode(const uint8_t mode);

int wifi_get_rf_channel(uint8_t *channel);

int wifi_get_rf_radio_mode(uint8_t *mode);

int wifi_set_rf_band(const uint8_t band);

int wifi_get_rf_band(uint8_t *band);

int wifi_set_rf_bandwidth(const uint8_t bandwidth);

int wifi_get_rf_bandwidth(uint8_t *bandwidth);

int wifi_get_rf_per(uint32_t *rx_tot_pkt_count, uint32_t *rx_mcast_bcast_count, uint32_t *rx_pkt_fcs_error);

int wifi_set_rf_tx_cont_mode(const uint32_t enable_tx,
                             const uint32_t cw_mode,
                             const uint32_t payload_pattern,
                             const uint32_t cs_mode,
                             const uint32_t act_sub_ch,
                             const uint32_t tx_rate);

int wifi_set_rf_tx_antenna(const uint8_t antenna);

int wifi_get_rf_tx_antenna(uint8_t *antenna);

int wifi_set_rf_rx_antenna(const uint8_t antenna);

int wifi_get_rf_rx_antenna(uint8_t *antenna);

int wifi_set_rf_tx_power(const uint32_t power, const uint8_t mod, const uint8_t path_id);

int wifi_cfg_rf_he_tb_tx(uint16_t enable, uint16_t qnum, uint16_t aid, uint16_t axq_mu_timer, int16_t tx_power);

int wifi_rf_trigger_frame_cfg(uint32_t Enable_tx,
                              uint32_t Standalone_hetb,
                              uint8_t FRAME_CTRL_TYPE,
                              uint8_t FRAME_CTRL_SUBTYPE,
                              uint16_t FRAME_DURATION,
                              uint64_t TriggerType,
                              uint64_t UlLen,
                              uint64_t MoreTF,
                              uint64_t CSRequired,
                              uint64_t UlBw,
                              uint64_t LTFType,
                              uint64_t LTFMode,
                              uint64_t LTFSymbol,
                              uint64_t UlSTBC,
                              uint64_t LdpcESS,
                              uint64_t ApTxPwr,
                              uint64_t PreFecPadFct,
                              uint64_t PeDisambig,
                              uint64_t SpatialReuse,
                              uint64_t Doppler,
                              uint64_t HeSig2,
                              uint32_t AID12,
                              uint32_t RUAllocReg,
                              uint32_t RUAlloc,
                              uint32_t UlCodingType,
                              uint32_t UlMCS,
                              uint32_t UlDCM,
                              uint32_t SSAlloc,
                              uint8_t UlTargetRSSI,
                              uint8_t MPDU_MU_SF,
                              uint8_t TID_AL,
                              uint8_t AC_PL,
                              uint8_t Pref_AC);

int wifi_set_rf_tx_frame(const uint32_t enable,
                         const uint32_t data_rate,
                         const uint32_t frame_pattern,
                         const uint32_t frame_length,
                         const uint16_t adjust_burst_sifs,
                         const uint32_t burst_sifs_in_us,
                         const uint32_t short_preamble,
                         const uint32_t act_sub_ch,
                         const uint32_t short_gi,
                         const uint32_t adv_coding,
                         const uint32_t tx_bf,
                         const uint32_t gf_mode,
                         const uint32_t stbc,
                         const uint8_t *bssid);

int wifi_set_rf_otp_mac_addr(uint8_t *mac);

int wifi_get_rf_otp_mac_addr(uint8_t *mac);

int wifi_set_rf_otp_cal_data(const uint8_t *cal_data, uint32_t cal_data_len);

int wifi_get_rf_otp_cal_data(uint8_t *cal_data);
#endif
#if CONFIG_WIFI_FW_DEBUG
/** This function registers callbacks which are used to generate FW Dump on USB
 * device.
 *
 * \param[in] wifi_usb_mount_cb Callback to mount usb device.
 * \param[in] wifi_usb_file_open_cb Callback to open file on usb device for FW dump.
 * \param[in] wifi_usb_file_write_cb Callback to write FW dump data to opened file.
 * \param[in] wifi_usb_file_close_cb Callback to close FW dump file.
 *
 */
void wifi_register_fw_dump_cb(int (*wifi_usb_mount_cb)(),
                              int (*wifi_usb_file_open_cb)(char *test_file_name),
                              int (*wifi_usb_file_write_cb)(uint8_t *data, size_t data_len),
                              int (*wifi_usb_file_close_cb)());
#endif

#if CONFIG_WMM
void wifi_wmm_init();
t_u32 wifi_wmm_get_pkt_prio(void *buf, t_u8 *tid);
t_u8 wifi_wmm_get_packet_cnt(void);
/* handle EVENT_TX_DATA_PAUSE */
void wifi_handle_event_data_pause(void *data);
void wifi_wmm_tx_stats_dump(int bss_type);
#endif /* CONFIG_WMM */

int wifi_set_rssi_low_threshold(uint8_t *low_rssi);

#if CONFIG_HEAP_DEBUG
/**
 * Show os mem alloc and free info.
 *
 */
void wifi_show_os_mem_stat();
#endif

#if CONFIG_WPS2
/**
 * enable/disable WPS session
 *
 * \param[in] option -- 1 is WPS session START, 0 is WPS session END
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_send_wps_cfg_cmd(int option);

int wps_low_level_output(const uint8_t interface, const uint8_t *buf, const uint16_t len);

#endif /* CONFIG_WPS2 */


#if CONFIG_MULTI_CHAN
/**
 * Set multi-channel stayed time in us.
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_mc_cfg(uint32_t channel_time);

/**
 * Get multi-channel stayed time in us.
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_get_mc_cfg(uint32_t *channel_time);

/**
 * Set multi-channel status disable/enable.
 * \param[in]      status       status disable/enable
 * 0-disable, 1-enable
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_mc_policy(const int status);
/**
 * Get multi-channel status disable/enable.
 *
 * \return status 0-disable, 1-enable.
 */
int wifi_get_mc_policy(void);

/**
 * Set multi-channel config.
 * \param[in]      num       array length of drcs_cfg[]
 * \param[in] 	   drcs      multi-channel config, maybe an array
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_mc_cfg_ext(const wifi_drcs_cfg_t *drcs, const int num);

/**
 * Get multi-channel config.
 * \param[in]      num       array length of drcs_cfg[]
 * \param[out]     drcs      multi-channel config, maybe an array
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_get_mc_cfg_ext(wifi_drcs_cfg_t *drcs, int num);
#endif

/**
 *Frame Tx - Injecting Wireless frames from Host
 *
 * This function is used to Inject Wireless frames from application
 * directly.
 *
 * \note All injected frames will be sent on station interface. Application
 * needs minimum of 2 KBytes stack for successful operation.
 * Also application have to take care of allocating buffer for 802.11 Wireless
 * frame (Header + Data) and freeing allocated buffer. Also this
 * API may not work when Power Save is enabled on station interface.
 *
 * \param[in] bss_type The interface on which management frame needs to be send.
 * \param[in] buff Buffer holding 802.11 Wireless frame (Header + Data).
 * \param[in] len Length of the 802.11 Wireless frame.
 *
 * \return WM_SUCCESS on success or error code.
 *
 **/

int wifi_inject_frame(const enum wlan_bss_type bss_type, const uint8_t *buff, const size_t len);

int wifi_supp_inject_frame(const unsigned int bss_type, const uint8_t *buff, const size_t len);
#if CONFIG_WPA_SUPP
void wifi_is_wpa_supplicant_input(const uint8_t interface, const uint8_t *buffer, const uint16_t len);
void wifi_wpa_supplicant_eapol_input(const uint8_t interface,
                                     const uint8_t *src_addr,
                                     const uint8_t *buffer,
                                     const uint16_t len);

t_u8 wifi_get_sec_channel_offset(unsigned int chan);
int wifi_nxp_scan_res_get(void);
int wifi_nxp_survey_res_get(void);
int wifi_nxp_set_default_scan_ies(const u8 *ies, size_t ies_len);
void wifi_nxp_reset_scan_flag();
#endif

#if CONFIG_DRIVER_MBO
int wifi_host_mbo_cfg(int enable_mbo);
int wifi_mbo_preferch_cfg(t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
int wifi_mbo_send_preferch_wnm(t_u8 *src_addr, t_u8 *target_bssid, t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
#endif

#if CONFIG_ECSA

/**
 * Send the ecsa config parameter to FW by TLV.
 *
 *\param[in] block_tx 0 -- no need to block traffic,1 -- need block traffic.
 *\param[in] oper_class Operating class according to IEEE std802.11 spec, when 0 is used, only CSA IE will be used.
 *\param[in] channel The channel will switch to.
 *\param[in] switch_count Channel switch time to send ECSA ie.
 *\param[in] band_width Channel width switch to(optional),only for 5G channels.
 *\param[in] ecsa True need to get operclass by band_width and channel, Otherwise, no need
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_ecsa_cfg(t_u8 block_tx, t_u8 oper_class, t_u8 channel, t_u8 switch_count, t_u8 band_width, t_u8 ecsa);

/**
 * Send the ecsa config parameter to FW by action.
 *
 *\param[in] block_tx 0 -- no need to block traffic,1 -- need block traffic.
 *\param[in] oper_class Operating class according to IEEE std802.11 spec, when 0 is used, only CSA IE will be used.
 *\param[in] channel The channel will switch to.
 *\param[in] switch_count Channel switch time to send ECSA ie.
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_set_action_ecsa_cfg(t_u8 block_tx, t_u8 oper_class, t_u8 channel, t_u8 switch_count);

/**
 * Record the ECSA blcok tx time.
 *
 *\param[in] switch_count FW expire at switch_count*beacon_period,and then switch to new_channel.
 *
 * \return void.
 */
void set_ecsa_block_tx_time(t_u8 switch_count);

/**
 * Get the blcok tx time when need to block traffic.
 *
 * \return blcok tx time.
 */
t_u8 get_ecsa_block_tx_time();

/**
 * Record whether block tx is required.
 *
 *\param[in] block_tx if block tx is required, block_tx is true. otherwise, flag is false.
 *
 * \return void.
 */
void set_ecsa_block_tx_flag(bool block_tx);

/**
 * Get the block tx status.
 *
 * \return true block tx is required, false not required.
 */
bool get_ecsa_block_tx_flag();

/**
 * put the ecsa semaphore .
 *
 * \return void.
 */
void wifi_put_ecsa_sem(void);

/** wifi_ecsa_status_control */
typedef struct _wifi_ecsa_status_control
{
    /** block tx required*/
    bool required;
    /** block time of one detect period*/
    t_u8 block_time;
    /** Semaphore to wait ECSA complete */
    OSA_SEMAPHORE_HANDLE_DEFINE(ecsa_sem);
} wifi_ecsa_status_control;
#endif

typedef struct _wifi_ecsa_info
{
    t_u8 bss_type;
    t_u8 band_config;
    /** channel */
    t_u8 channel;
} wifi_ecsa_info;

#if CONFIG_CSI
typedef enum _csi_state
{
    csi_enabled = 0,
    csi_disabled,
    csiconfig_wrong,
    csiinternal_restart,
    csiinternal_stop,
    csiinternal_disabled,
} csi_state;

typedef MLAN_PACK_START struct _wifi_csi_status_info
{
    csi_state status;
    t_u8 channel;
    t_u16 cnt;
} MLAN_PACK_END wifi_csi_status_info;
#endif

#ifdef RW610
#if CONFIG_HOST_SLEEP
extern int wakeup_by;
#define WAKEUP_BY_WLAN 0x1
#define WAKEUP_BY_RTC  0x2
#define WAKEUP_BY_PIN1 0x4
#endif
#endif

#if CONFIG_CSI
/**
 * Send the csi config parameter to FW.
 *
 *\param[in] csi_params Csi config parameter
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_csi_cfg(wifi_csi_config_params_t *csi_params);
int register_csi_user_callback(int (*csi_data_recv_callback)(void *buffer, size_t len));
int unregister_csi_user_callback(void);
void csi_local_buff_init();
void csi_save_data_to_local_buff(void *data);
void csi_deliver_data_to_user();

typedef struct _csi_local_buff_statu
{
    t_u8 write_index;
    t_u8 read_index;
    t_u8 valid_data_cnt;
    /** Semaphore to protect data parameters */
    OSA_SEMAPHORE_HANDLE_DEFINE(csi_data_sem);
} csi_local_buff_statu;

extern int csi_event_cnt;
extern t_u64 csi_event_data_len;
#endif
#if CONFIG_NET_MONITOR
/**
 * Send the net monitor config parameter to FW.
 *
 *\param[in] monitor Monitor config parameter.
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_net_monitor_cfg(wifi_net_monitor_t *monitor);

/**
 * Register user callback to receive monitor data.
 *
 *\param[in] monitor_cb User callback function.
 *
 * \return void.
 */
void register_monitor_user_callback(int (*monitor_cb)(void *frame, t_u16 len));

/**
 * Deregister user callback for monitor feature.
 *
 * \return void.
 */
void deregister_monitor_user_callback();

/**
 * Record the status when start monitor.
 *
 *\param[in] flag Flag is true, when in monitor mode,otherwise, flag is false.
 *
 * \return void.
 */
void set_monitor_flag(bool flag);

/**
 * Get the monitor status to determine if it is in monitor mode.
 *
 * \return true in monitor mode, false in other modes.
 */
bool get_monitor_flag();

#endif

int wifi_send_mgmt_auth_request(const t_u8 channel,
                                const t_u8 auth_alg,
                                const t_u8 *auth_seq_num,
                                const t_u8 *status_code,
                                const t_u8 *dest,
                                const t_u8 *sae_data,
                                const t_u16 sae_data_len);
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
int wifi_deauthenticate(uint8_t *bssid);

#if CONFIG_TURBO_MODE
int wifi_get_turbo_mode(t_u8 *mode);
int wifi_get_uap_turbo_mode(t_u8 *mode);
int wifi_set_turbo_mode(t_u8 mode);
int wifi_set_uap_turbo_mode(t_u8 mode);
#endif

/**
 *  @brief This function converts region string to region code
 *
 *  @param region_string    Region string
 *
 *  @return                 Region code
 */
t_u8 region_string_2_region_code(t_u8 *region_string);

#if CONFIG_COEX_DUTY_CYCLE
int wifi_single_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime);
int wifi_dual_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime, t_u16 wlanBlockTime);
#endif

#ifdef RW610
/* get CAU module temperature and write to firmware */
void wifi_cau_temperature_enable(void);
int wifi_cau_temperature_write_to_firmware(void);
int32_t wifi_get_temperature(void);
void wifi_pmip_v33_enable();
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
int wifi_set_indrst_cfg(const wifi_indrst_cfg_t *indrst_cfg, mlan_bss_type bss_type);
int wifi_get_indrst_cfg(wifi_indrst_cfg_t *indrst_cfg, mlan_bss_type bss_type);
int wifi_test_independent_reset();
int wifi_trigger_oob_indrst();
#endif

#if CONFIG_WIFI_BOOT_SLEEP
int wifi_boot_sleep(uint16_t action, uint16_t *enable);
#endif

#if CONFIG_AUTO_NULL_TX
int wifi_auto_null_tx(wifi_auto_null_tx_t *auto_null_tx, mlan_bss_type bss_type);
#endif

#if CONFIG_WPA_SUPP
void hostapd_connected_sta_list(wifi_sta_info_t *si, wifi_sta_list_t *sl);
#endif
bool wifi_is_remain_on_channel(void);
#if CONFIG_WMM
/**
 * Update STA TX pause status
 *
 *\param[in] tx_pause trigger tx handler if this is an unpause event.
 *
 * \return void.
 */
void wifi_sta_handle_event_data_pause(void *tx_pause);
#endif
/* UAP support */
#if UAP_SUPPORT
#if CONFIG_WMM
/**
 * Update uAP TX pause status
 *
 *\param[in] tx_pause trigger tx handler if this is an unpause event.
 *           for self address, update the whole priv interface status
 *           for other addresses, update corresponding ralist status
 *           trigger tx handler if this is an unpause event
 * \return void.
 */
void wifi_uap_handle_event_data_pause(void *tx_pause);
#endif
/**
 * Returns the current STA list connected to our uAP
 *
 * This function gets its information after querying the firmware. It will
 * block till the response is received from firmware or a timeout.
 *
 * @param[in, out] list After this call returns this points to the
 * structure \ref wifi_sta_list_t allocated by the callee. This is variable
 * length structure and depends on count variable inside it. <b> The caller
 * needs to free this buffer after use.</b>. If this function is unable to
 * get the sta list, the value of list parameter will be NULL
 *
 * \note The caller needs to explicitly free the buffer returned by this
 * function.
 *
 * @return void
 */
int wifi_uap_bss_sta_list(wifi_sta_list_t **list);

/**
 * Dsiconnect ex-sta which is connected to in-uap.
 *
 * \param[in] mac_addr Mac address of external station.
 * \param[in] reason_code Deauth reason code.
 *
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wifi_sta_deauth(uint8_t *mac_addr, uint16_t reason_code);

int wifi_uap_rates_getset(uint8_t action, char *rates, uint8_t num_rates);
int wifi_uap_sta_ageout_timer_getset(uint8_t action, uint32_t *sta_ageout_timer);
int wifi_uap_ps_sta_ageout_timer_getset(uint8_t action, uint32_t *ps_sta_ageout_timer);

/**
 * Get the uAP channel number
 *
 *
 * @param[in] channel Pointer to channel number. Will be initialized by
 * callee
 * @return Standard WMSDK return code
 */
int wifi_get_uap_channel(int *channel);

/**
 * Get/Set the uAP mfpc and mfpr
 *
 * @param[in] action
 *
 * \param[in, out] mfpc Management Frame Protection Capable (MFPC)
 *                       1: Management Frame Protection Capable
 *                       0: Management Frame Protection not Capable
 * \param[in, out] mfpr Management Frame Protection Required (MFPR)
 *                       1: Management Frame Protection Required
 *                       0: Management Frame Protection Optional
 *
 * @return cmd response status
 */
int wifi_uap_pmf_getset(uint8_t action, uint8_t *mfpc, uint8_t *mfpr);

/**
 * enable/disable 80211d domain feature for the uAP.
 *
 * @note This API only set 80211d domain feature.
 * The actual application will happen only during
 * starting phase of uAP. So, if the uAP is already started then the
 * configuration will not apply till uAP re-start.
 *
 * @return WM_SUCCESS on success or error code.
 *
 */
int wifi_uap_enable_11d_support();

int wifi_enable_uap_11d_support();
int wifi_disable_uap_11d_support();
int wrapper_wlan_uap_11d_enable(t_u32 state);

void wifi_uap_set_httxcfg(const t_u16 ht_tx_cfg);
int wifi_uap_set_httxcfg_int(unsigned short httxcfg);

int wifi_uap_ps_inactivity_sleep_exit(mlan_bss_type type);
int wifi_uap_ps_inactivity_sleep_enter(mlan_bss_type type,
                                       unsigned int ctrl_bitmap,
                                       unsigned int min_sleep,
                                       unsigned int max_sleep,
                                       unsigned int inactivity_to,
                                       unsigned int min_awake,
                                       unsigned int max_awake);

#if CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM
void wifi_uap_enable_sticky_bit(const uint8_t *mac_addr);
#endif /* CONFIG_WIFI_UAP_WORKAROUND_STICKY_TIM */
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
);

int wrapper_wlan_uap_ampdu_enable(uint8_t *addr
#if CONFIG_WMM
                                  ,
                                  t_u8 tid
#endif
);

int wifi_uap_stop();
#if CONFIG_HOSTAPD
int wifi_uap_do_acs(const int *freq_list);
#else
int wifi_uap_do_acs(const t_u16 acs_band);
#endif

#if CONFIG_WIFI_CAPA
/**
 * Set uAP capability
 *
 * User can set uAP capability of 11ax/11ac/11n/legacy. Default is 11ax.
 *
 * @param[in] wlan_capa uAP capability bitmap.
 *                      1111 - 11AX
 *                      0111 - 11AC
 *                      0011 - 11N
 *                      0001 - legacy
 *
 */
void wifi_uap_config_wifi_capa(uint8_t wlan_capa);
void wifi_get_fw_info(mlan_bss_type type, t_u16 *fw_bands);
#endif

int wifi_uap_set_bandwidth(const t_u8 bandwidth);

t_u8 wifi_uap_get_bandwidth();

int wifi_uap_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr);

#if CONFIG_HOSTAPD
t_u16 wifi_get_default_ht_capab();
t_u32 wifi_get_default_vht_capab();

void wifi_uap_client_assoc(t_u8 *sta_addr, unsigned char is_11n_enabled);
void wifi_uap_client_deauth(t_u8 *sta_addr);
#endif
#endif /* UAP_SUPPORT */
#endif /* __WIFI_H__ */
