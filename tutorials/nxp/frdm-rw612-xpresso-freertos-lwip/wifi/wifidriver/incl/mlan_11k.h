/** @file mlan_11k.h
 *
 *  @brief Interface for the BTM module implemented in mlan_11k.c
 *
 *  Driver interface functions and type declarations for the process RRM data
 *    module implemented in mlan_11k.c.
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    08/24/2022: initial version
********************************************************/

#ifndef _MLAN_11K_H_
#define _MLAN_11K_H_

#if CONFIG_11K
/* Link Measurement Request frame */
typedef MLAN_PACK_START struct _mgmt_rrm_link_meas_request
{
    t_u8 dialog_tok;
    /* Transmit power used to transmit the frame containing the Link Measurement Request */
    t_s8 tx_power;
    /* The upper limit on the transmit power */
    t_s8 max_tx_power;
    t_u8 variable[1];
} MLAN_PACK_END mgmt_rrm_link_meas_request;

/* Link Measurement Report frame */
typedef MLAN_PACK_START struct _mgmt_rrm_link_meas_report
{
    t_u8 dialog_tok;
    /* Transmit power and link margin information */
    IEEEtypes_TPCReport_t tpc_report;
    /* Receive Antenna ID */
    t_u8 rx_ant_id;
    /* Transmit Antenna ID */
    t_u8 tx_ant_id;
    /* Received channel power indicator in a dBm scale */
    t_u8 rcpi;
    /* Received signal to noise indicator in a dBm scale */
    t_u8 rsni;
    t_u8 variable[1];
} MLAN_PACK_END mgmt_rrm_link_meas_report;

/* Measurement Request element */
typedef MLAN_PACK_START struct _mgmt_rrm_radio_meas_request
{
    /* Element ID */
    t_u8 ele_id;
    /* Length */
    t_u8 length;
    /* Measurement Token */
    t_u8 token;
    /* Measurement Request Mode */
    t_u8 mode;
    /* Measurement Type */
    t_u8 type;
    /* Measurement Request */
    t_u8 variable[1];
} MLAN_PACK_END mgmt_rrm_radio_meas_request;

/* Beacon request */
typedef MLAN_PACK_START struct _mgmt_rrm_meas_beacon_request
{
    /* Operating Class */
    t_u8 oper_class;
    /* Channel Number */
    t_u8 channel;
    /* Randomization Interval (in TUs) */
    t_u16 rand_intv;
    /* Measurement Duration (in TUs) */
    t_u16 duration;
    /* Measurement Mode */
    t_u8 mode;
    /* BSSID */
    t_u8 bssid[IEEEtypes_ADDRESS_SIZE];
    /* Optional Subelements */
    t_u8 variable[1];
} MLAN_PACK_END mgmt_rrm_meas_beacon_request;

/* Beacon report */
typedef MLAN_PACK_START struct _mgmt_rrm_meas_beacon_report
{
    /* Operating Class */
    t_u8 op_class;
    /* Channel Number */
    t_u8 channel;
    /* Actual Measurement Start Time (in TSF of the BSS requesting the measurement) */
    t_u64 m_start_time;
    /* in TUs */
    t_u16 duration;
    /* Reported Frame Information */
    t_u8 report_info;
    /* Received channel power indicator in a dBm scale */
    t_u8 rcpi;
    /* Received signal to noise indicator in a dBm scale */
    t_u8 rsni;
    /* BSSID */
    t_u8 bssid[IEEEtypes_ADDRESS_SIZE];
    /* Antenna ID */
    t_u8 ant_id;
    /* Parent TSF */
    t_u32 parent_tsf;
    /* Optional Subelements */
    t_u8 variable[1];
} MLAN_PACK_END mgmt_rrm_meas_beacon_report;

/* Measurement requests type */
#define WLAN_RRM_MEASURE_TYPE_BEACON 5

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

/* dot11PHYType */
enum wlan_phy_type
{
    WLAN_PHY_TYPE_UNSPECIFIED = 0,
    WLAN_PHY_TYPE_FHSS        = 1,
    WLAN_PHY_TYPE_DSSS        = 2,
    WLAN_PHY_TYPE_IRBASEBAND  = 3,
    WLAN_PHY_TYPE_OFDM        = 4,
    WLAN_PHY_TYPE_HRDSSS      = 5,
    WLAN_PHY_TYPE_ERP         = 6,
    WLAN_PHY_TYPE_HT          = 7,
    WLAN_PHY_TYPE_DMG         = 8,
    WLAN_PHY_TYPE_VHT         = 9,
};

/* Measurement Mode of Beacon request */
enum wlan_rrm_beacon_report_mode
{
    WLAN_RRM_MEASUREMENT_MODE_PASSIVE = 0,
    WLAN_RRM_MEASUREMENT_MODE_ACTIVE  = 1,
    WLAN_RRM_MEASUREMENT_MODE_TABLE   = 2,
};

/* Beacon Request subelement IDs */
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_SSID             0
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REPORTING_INFO   1
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REPORTING_DETAIL 2
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REQUEST          10
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_AP_CHANNEL       51
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_LAST_INDICATION  164
#define WLAN_RRM_BEACON_REQUEST_SUBELEMENT_VENDOR           221

/* Measurement Report Mode field */
#define WLAN_RRM_REPORT_MODE_ACCEPT           0
#define WLAN_RRM_REPORT_MODE_REJECT_LATE      BIT(0)
#define WLAN_RRM_REPORT_MODE_REJECT_INCAPABLE BIT(1)
#define WLAN_RRM_REPORT_MODE_REJECT_REFUSED   BIT(2)

/* Beacon report Subelement IDs */
#define WLAN_RRM_BEACON_REP_SUBELEM_FRAME_BODY             1
#define WLAN_RRM_BEACON_REP_SUBELEM_FRAME_BODY_FRAGMENT_ID 2
#define WLAN_RRM_BEACON_REP_SUBELEM_LAST_INDICATION        164

#define WLAN_RRM_BEACON_REP_FRAME_BODY_FRAGMENT_SUB_LEN 4
#define WLAN_RRM_BEACON_REP_LAST_INDICATION_SUB_LEN     3

/* Measurement Request Mode field */
#define WLAN_RRM_MEAS_REQUEST_MODE_PARALLEL           BIT(0)
#define WLAN_RRM_MEAS_REQUEST_MODE_ENABLE             BIT(1)
#define WLAN_RRM_MEAS_REQUEST_MODE_REQUEST            BIT(2)
#define WLAN_RRM_MEAS_REQUEST_MODE_REPORT             BIT(3)
#define WLAN_RRM_MEAS_REQUEST_MODE_DURATION_MANDATORY BIT(4)

void wlan_process_radio_measurement_request(t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect);
void wlan_process_neighbor_report_response(t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect);
void wlan_process_link_measurement_request(
    t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect, RxPD *rxpd);
void wlan_send_mgmt_rm_beacon_report(
    t_u8 dialog_tok, t_u8 *src_addr, t_u8 *dst_addr, t_u8 *rep, t_u32 rep_len, bool protect);
void wlan_add_rm_beacon_report(wlan_rrm_beacon_report_data *rep_data,
                               BSSDescriptor_t *bss_entry,
                               t_u8 **buf_pos,
                               t_s32 remained_len,
                               t_u32 *pos_last_indication);
bool wlan_rrm_matched_ap_found(wlan_rrm_beacon_report_data *rep_data, BSSDescriptor_t *bss_entry);

/** Add RRM Capabilities */
void wlan_dot11k_formatRrmCapabilities(IEEEtypes_RrmEnabledCapabilities_t *pRrmCapIe, t_u16 bcnInterval);

/* send mgmt neighbor report request frame */
int wlan_send_mgmt_rm_neighbor_request(mlan_private *pmpriv, t_u8 *ssid, t_u8 ssid_len);
#endif /* CONFIG_11K */

#endif /* !_MLAN_11K_H_ */
