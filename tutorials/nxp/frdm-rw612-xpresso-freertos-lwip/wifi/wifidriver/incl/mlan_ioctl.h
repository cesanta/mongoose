/** @file mlan_ioctl.h
 *
 *  @brief This file declares the IOCTL data structures and APIs.
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    11/07/2008: initial version
******************************************************/

#ifndef _MLAN_IOCTL_H_
#define _MLAN_IOCTL_H_

/** Enumeration for IOCTL request ID */
typedef enum _mlan_ioctl_req_id
{
    /* Scan Group */
    MLAN_IOCTL_SCAN = 0x00010000,
    MLAN_OID_SCAN_NORMAL,
    MLAN_OID_SCAN_SPECIFIC_SSID,
    MLAN_OID_SCAN_USER_CONFIG,
    MLAN_OID_SCAN_CONFIG,
    MLAN_OID_SCAN_GET_CURRENT_BSS,
    MLAN_OID_SCAN_CANCEL,
    MLAN_OID_SCAN_TABLE_FLUSH,
    MLAN_OID_SCAN_BGSCAN_CONFIG,
    /* BSS Configuration Group */
    MLAN_IOCTL_BSS = 0x00020000,
    MLAN_OID_BSS_START,
    MLAN_OID_BSS_STOP,
    MLAN_OID_BSS_MODE,
    MLAN_OID_BSS_CHANNEL,
    MLAN_OID_BSS_CHANNEL_LIST,
    MLAN_OID_BSS_MAC_ADDR,
    MLAN_OID_BSS_MULTICAST_LIST,
    MLAN_OID_BSS_FIND_BSS,
    MLAN_OID_IBSS_BCN_INTERVAL,
    MLAN_OID_IBSS_ATIM_WINDOW,
    MLAN_OID_IBSS_CHANNEL,
#if UAP_SUPPORT
    MLAN_OID_UAP_BSS_CONFIG,
    MLAN_OID_UAP_DEAUTH_STA,
    MLAN_OID_UAP_BSS_RESET,
#endif
#if defined(STA_SUPPORT) && UAP_SUPPORT
    MLAN_OID_BSS_ROLE,
#endif
#if UAP_HOST_MLME
#if UAP_SUPPORT
    MLAN_OID_UAP_ADD_STATION = 0x0002001C,
#endif
#endif
#if CONFIG_ECSA
    MLAN_OID_ACTION_CHAN_SWITCH = 0x0002001E,
#endif

    /* Radio Configuration Group */
    MLAN_IOCTL_RADIO_CFG = 0x00030000,
    MLAN_OID_RADIO_CTRL,
    MLAN_OID_BAND_CFG,
    MLAN_OID_ANT_CFG,

    /* SNMP MIB Group */
    MLAN_IOCTL_SNMP_MIB = 0x00040000,
    MLAN_OID_SNMP_MIB_RTS_THRESHOLD,
    MLAN_OID_SNMP_MIB_FRAG_THRESHOLD,
    MLAN_OID_SNMP_MIB_RETRY_COUNT,
#if UAP_SUPPORT
    MLAN_OID_SNMP_MIB_DOT11D,
    MLAN_OID_SNMP_MIB_DOT11H,
#endif
    MLAN_OID_SNMP_MIB_DTIM_PERIOD,
    MLAN_OID_SNMP_MIB_CTRL_DEAUTH,

    /* Status Information Group */
    MLAN_IOCTL_GET_INFO = 0x00050000,
    MLAN_OID_GET_STATS,
    MLAN_OID_GET_SIGNAL,
    MLAN_OID_GET_FW_INFO,
    MLAN_OID_GET_VER_EXT,
    MLAN_OID_GET_BSS_INFO,
    MLAN_OID_GET_DEBUG_INFO,
#if UAP_SUPPORT
    MLAN_OID_UAP_STA_LIST,
#endif

    /* Security Configuration Group */
    MLAN_IOCTL_SEC_CFG = 0x00060000,
    MLAN_OID_SEC_CFG_AUTH_MODE,
    MLAN_OID_SEC_CFG_ENCRYPT_MODE,
    MLAN_OID_SEC_CFG_WPA_ENABLED,
    MLAN_OID_SEC_CFG_ENCRYPT_KEY,
    MLAN_OID_SEC_CFG_PASSPHRASE,
    MLAN_OID_SEC_CFG_PASSWORD,
    MLAN_OID_SEC_CFG_EWPA_ENABLED,
    MLAN_OID_SEC_CFG_ESUPP_MODE,
    MLAN_OID_SEC_CFG_WAPI_ENABLED,
    MLAN_OID_SEC_CFG_PORT_CTRL_ENABLED,
    MLAN_OID_SEC_CFG_REPORT_MIC_ERR,
    MLAN_OID_SEC_QUERY_KEY,

    /* Rate Group */
    MLAN_IOCTL_RATE = 0x00070000,
    MLAN_OID_RATE_CFG,
    MLAN_OID_GET_DATA_RATE,
    MLAN_OID_SUPPORTED_RATES,

    /* Power Configuration Group */
    MLAN_IOCTL_POWER_CFG = 0x00080000,
    MLAN_OID_POWER_CFG,
    MLAN_OID_POWER_CFG_EXT,

    /* Power Management Configuration Group */
    MLAN_IOCTL_PM_CFG = 0x00090000,
    MLAN_OID_PM_CFG_IEEE_PS,
    MLAN_OID_PM_CFG_HS_CFG,
    MLAN_OID_PM_CFG_INACTIVITY_TO,
    MLAN_OID_PM_CFG_DEEP_SLEEP,
    MLAN_OID_PM_CFG_SLEEP_PD,
    MLAN_OID_PM_CFG_PS_CFG,
    MLAN_OID_PM_CFG_SLEEP_PARAMS,
#if UAP_SUPPORT
    MLAN_OID_PM_CFG_PS_MODE,
#endif /* UAP_SUPPORT */
    MLAN_OID_PM_INFO,
    MLAN_OID_PM_HS_WAKEUP_REASON = 0x0009000B,

    /* WMM Configuration Group */
    MLAN_IOCTL_WMM_CFG = 0x000A0000,
    MLAN_OID_WMM_CFG_ENABLE,
    MLAN_OID_WMM_CFG_QOS,
    MLAN_OID_WMM_CFG_ADDTS,
    MLAN_OID_WMM_CFG_DELTS,
    MLAN_OID_WMM_CFG_QUEUE_CONFIG,
    MLAN_OID_WMM_CFG_QUEUE_STATS,
    MLAN_OID_WMM_CFG_QUEUE_STATUS,
    MLAN_OID_WMM_CFG_TS_STATUS,

    /* WPS Configuration Group */
    MLAN_IOCTL_WPS_CFG = 0x000B0000,
    MLAN_OID_WPS_CFG_SESSION,

    /* 802.11n Configuration Group */
    MLAN_IOCTL_11N_CFG = 0x000C0000,
    MLAN_OID_11N_CFG_TX,
    MLAN_OID_11N_HTCAP_CFG,
    MLAN_OID_11N_CFG_ADDBA_REJECT,
    MLAN_OID_11N_CFG_AGGR_PRIO_TBL,
    MLAN_OID_11N_CFG_ADDBA_PARAM,
    MLAN_OID_11N_CFG_MAX_TX_BUF_SIZE,
    MLAN_OID_11N_CFG_AMSDU_AGGR_CTRL,
    MLAN_OID_11N_CFG_SUPPORTED_MCS_SET,
    MLAN_OID_11N_CFG_TX_BF_CAP,

    MLAN_OID_11N_CFG_DELBA            = 0x000C000C,
    MLAN_OID_11N_CFG_REJECT_ADDBA_REQ = 0x000C000D,

    /* 802.11d Configuration Group */
    MLAN_IOCTL_11D_CFG = 0x000D0000,
#ifdef STA_SUPPORT
    MLAN_OID_11D_CFG_ENABLE,
    MLAN_OID_11D_CLR_CHAN_TABLE,
#endif /* STA_SUPPORT */
    MLAN_OID_11D_DOMAIN_INFO,

    /* Register Memory Access Group */
    MLAN_IOCTL_REG_MEM = 0x000E0000,
    MLAN_OID_REG_RW,
    MLAN_OID_EEPROM_RD,
    MLAN_OID_MEM_RW,

    /* Multi-Radio Configuration Group */
    MLAN_IOCTL_MFR_CFG = 0x00100000,

    /* 802.11h Configuration Group */
    MLAN_IOCTL_11H_CFG = 0x00110000,
    MLAN_OID_11H_CHANNEL_CHECK,
    MLAN_OID_11H_LOCAL_POWER_CONSTRAINT,


#if CONFIG_11AX
    /* 802.11ax Configuration Group  */
    MLAN_IOCTL_11AX_CFG   = 0x00170000,
    MLAN_OID_11AX_HE_CFG  = 0x00170001,
    MLAN_OID_11AX_CMD_CFG = 0x00170002,
#endif /* ENABLE_802_11AX */

    /* Miscellaneous Configuration Group */
    MLAN_IOCTL_MISC_CFG = 0x00200000,
    MLAN_OID_MISC_GEN_IE,
    MLAN_OID_MISC_REGION,
    MLAN_OID_MISC_WARM_RESET,
#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
    MLAN_OID_MISC_SDIO_MPA_CTRL,
#endif
    MLAN_OID_MISC_HOST_CMD,
    MLAN_OID_MISC_SYS_CLOCK,
    MLAN_OID_MISC_SOFT_RESET,
    MLAN_OID_MISC_WWS,
    MLAN_OID_MISC_INIT_SHUTDOWN,
    MLAN_OID_MISC_CUSTOM_IE,
    MLAN_OID_MISC_TX_DATAPAUSE,
    MLAN_OID_MISC_IP_ADDR,
    MLAN_OID_MISC_MAC_CONTROL,
    MLAN_OID_MISC_MEF_CFG,
    MLAN_OID_MISC_CFP_CODE,
    MLAN_OID_MISC_COUNTRY_CODE,
    MLAN_OID_MISC_THERMAL,
    MLAN_OID_MISC_RX_MGMT_IND,
    MLAN_OID_MISC_SUBSCRIBE_EVENT,
#ifdef DEBUG_LEVEL1
    MLAN_OID_MISC_DRVDBG,
#endif
    MLAN_OID_MISC_HOTSPOT_CFG,
    MLAN_OID_MISC_OTP_USER_DATA,
#ifdef WLAN_LOW_POWER_ENABLE
    MLAN_OID_MISC_LOW_PWR_MODE,
#endif // WLAN_LOW_POWER_ENABLE
#if CONFIG_GTK_REKEY_OFFLOAD
    MLAN_OID_MISC_CONFIG_GTK_REKEY_OFFLOAD = 0x00200037,
#endif
#if CONFIG_ECSA
    MLAN_OID_MISC_OPER_CLASS = 0x00200038,
#endif
#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
    MLAN_OID_MISC_IND_RST_CFG = 0x00200040,
#endif
#if CONFIG_ECSA
    MLAN_OID_MISC_OPER_CLASS_CHECK = 0x00200049,
#endif
    MLAN_OID_MISC_GET_REGIONPWR_CFG,
#if CONFIG_WIFI_CLOCKSYNC
    MLAN_OID_MISC_GPIO_TSF_LATCH = 0x00200082,
    MLAN_OID_MISC_GET_TSF_INFO   = 0x00200083
#endif /* CONFIG_WIFI_CLOCKSYNC */
} mlan_ioctl_req_id;

/** Sub command size */
#define MLAN_SUB_COMMAND_SIZE 4U

/** Enumeration for the action of IOCTL request */
typedef enum _mlan_act_ioctl
{
    MLAN_ACT_SET = 1,
    MLAN_ACT_GET,
    MLAN_ACT_CANCEL,
    MLAN_ACT_CLEAR,
    MLAN_ACT_RESET,
    MLAN_ACT_DEFAULT
} mlan_act_ioctl;

/** Enumeration for generic enable/disable */
enum _mlan_act_generic
{
    MLAN_ACT_DISABLE = 0,
    MLAN_ACT_ENABLE  = 1
};

/** Enumeration for scan mode */
enum _mlan_scan_mode
{
    MLAN_SCAN_MODE_UNCHANGED = 0,
    MLAN_SCAN_MODE_BSS,
    MLAN_SCAN_MODE_IBSS,
    MLAN_SCAN_MODE_ANY
};

/** Enumeration for scan type */
typedef enum _mlan_scan_type
{
    MLAN_SCAN_TYPE_UNCHANGED = 0,
    MLAN_SCAN_TYPE_ACTIVE,
    MLAN_SCAN_TYPE_PASSIVE,
#if defined(RW610) || defined(SD9177) || defined(IW610)
    MLAN_SCAN_TYPE_PASSIVE_TO_ACTIVE,
#endif
} mlan_scan_type;

/** mlan_ioctl_req data structure */
typedef struct _mlan_ioctl_req
{
    /** Status code from firmware/driver */
    mlan_error_code status_code;
    /** BSS index number for multiple BSS support */
    t_u32 bss_index;
    /** Request id */
    t_u32 req_id;
    /** Action: set or get */
    mlan_act_ioctl action;

    /** Pointer to buffer */
    t_u8 *pbuf;
    /** Length of buffer */
    t_u32 buf_len;
    /** Length of the data read/written in buffer */
    t_u32 data_read_written;
    /** Length of buffer needed */
    t_u32 buf_len_needed;
    /** Reserved for MOAL module */
    t_ptr reserved_1;
} mlan_ioctl_req, *pmlan_ioctl_req;

/** Max number of supported rates */
#define MLAN_SUPPORTED_RATES 32

/** RSSI scan */
#define SCAN_RSSI(RSSI) (0x100U - ((t_u8)(RSSI)))

/** Max passive scan time for each channel in milliseconds */
#define MRVDRV_MAX_PASSIVE_SCAN_CHAN_TIME 2000

/** Max active scan time for each channel in milliseconds  */
#define MRVDRV_MAX_ACTIVE_SCAN_CHAN_TIME 500

#if CONFIG_SCAN_CHANNEL_GAP
/** Max gap time between 2 scan in milliseconds  */
#define MRVDRV_MAX_SCAN_CHAN_GAP_TIME 500
#endif

/** Maximum number of probes to send on each channel */
#define MAX_PROBES 4U

/** Default number of probes to send on each channel */
#define DEFAULT_PROBES 2

/**
 *  @brief Sub-structure passed in wlan_ioctl_get_scan_table_entry for each BSS
 *
 *  Fixed field information returned for the scan response in the IOCTL
 *    response.
 */
typedef struct _wlan_get_scan_table_fixed
{
    /** BSSID of this network */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** Channel this beacon/probe response was detected */
    t_u8 channel;
    /** RSSI for the received packet */
    t_u8 rssi;
#if CONFIG_SCAN_CHANNEL_GAP
    /** channel load */
    t_u8 chan_load;
#endif
    /** TSF value in microseconds from the firmware at packet reception */
    t_u64 network_tsf;
} wlan_get_scan_table_fixed;

/** mlan_802_11_ssid data structure */
typedef struct _mlan_802_11_ssid
{
    /** SSID Length */
    t_u32 ssid_len;
    /** SSID information field */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
} mlan_802_11_ssid, *pmlan_802_11_ssid;

typedef MLAN_PACK_START struct _tx_status_event
{
    /** packet type */
    t_u8 packet_type;
    /** tx_token_id */
    t_u8 tx_token_id;
    /** 0--success, 1--fail, 2--watchdogtimeout */
    t_u8 status;
} MLAN_PACK_END tx_status_event;

/**
 *  Sructure to retrieve the scan table
 */
typedef struct
{
    /**
     *  - Zero based scan entry to start retrieval in command request
     *  - Number of scans entries returned in command response
     */
    t_u32 scan_number;
    /**
     * Buffer marker for multiple wlan_ioctl_get_scan_table_entry structures.
     *   Each struct is padded to the nearest 32 bit boundary.
     */
    t_u8 scan_table_entry_buf[1];
} wlan_ioctl_get_scan_table_info;

/**
 *  Structure passed in the wlan_ioctl_get_scan_table_info for each
 *    BSS returned in the WLAN_GET_SCAN_RESP IOCTL
 */
typedef struct _wlan_ioctl_get_scan_table_entry
{
    /**
     *  Fixed field length included in the response.
     *
     *  Length value is included so future fixed fields can be added to the
     *   response without breaking backwards compatibility.  Use the length
     *   to find the offset for the bssInfoLength field, not a sizeof() calc.
     */
    t_u32 fixed_field_length;

    /**
     *  Length of the BSS Information (probe resp or beacon) that
     *    follows after the fixed_field_length
     */
    t_u32 bss_info_length;

    /**
     *  Always present, fixed length data fields for the BSS
     */
    wlan_get_scan_table_fixed fixed_fields;

    /*
     *  Probe response or beacon scanned for the BSS.
     *
     *  Field layout:
     *   - TSF              8 octets
     *   - Beacon Interval  2 octets
     *   - Capability Info  2 octets
     *
     *   - IEEE Infomation Elements; variable number & length per 802.11 spec
     */
    /* t_u8 bss_info_buffer[0]; */
} wlan_ioctl_get_scan_table_entry;

/** Type definition of mlan_scan_time_params */
typedef struct _mlan_scan_time_params
{
    /** Scan channel time for specific scan in milliseconds */
    t_u32 specific_scan_time;
    /** Scan channel time for active scan in milliseconds */
    t_u32 active_scan_time;
    /** Scan channel time for passive scan in milliseconds */
    t_u32 passive_scan_time;
} mlan_scan_time_params, *pmlan_scan_time_params;

/** Type definition of mlan_user_scan */
typedef struct _mlan_user_scan
{
    /** Length of scan_cfg_buf */
    t_u32 scan_cfg_len;
    /** Buffer of scan config */
    t_u8 scan_cfg_buf[1];
} mlan_user_scan, *pmlan_user_scan;

/** Type definition of mlan_scan_req */
typedef struct _mlan_scan_req
{
    /** BSS mode for scanning */
    t_u32 scan_mode;
    /** Scan type */
    mlan_scan_type scan_type;
    /** SSID */
    mlan_802_11_ssid scan_ssid;
    /** Scan time parameters */
    mlan_scan_time_params scan_time;
    /** Scan config parameters in user scan */
    mlan_user_scan user_scan;
} mlan_scan_req, *pmlan_scan_req;

/** Type definition of mlan_ds_misc_tx_frame for MLAN_OID_MISC_TX_FRAME */
typedef struct _mlan_ds_misc_tx_frame
{
    /** Band Configuration */
    Band_Config_t bandcfg;
    /** channel */
    t_u8 channel;
    /** Buffer type: data, cmd, event etc. */
    mlan_buf_type buf_type;
    /** QoS priority */
    t_u32 priority;
    /** Flags for this buffer */
    t_u32 flags;
    /** tx_seq_num */
    t_u32 tx_seq_num;
    /** tx_buf length */
    t_u16 data_len;
    /** Tx buffer */
    t_u8 tx_buf[512];
} mlan_ds_misc_tx_frame;

/** Type defnition of mlan_scan_resp */
typedef struct _mlan_scan_resp
{
    /** Number of scan result */
    t_u32 num_in_scan_table;
    /** Scan table */
    t_u8 *pscan_table;
    /* Age in seconds */
    t_u32 age_in_secs;
#if CONFIG_SCAN_CHANNEL_GAP
    /** channel statstics */
    t_u8 *pchan_stats;
    /** Number of records in the chan_stats */
    t_u32 num_in_chan_stats;
#endif
} mlan_scan_resp, *pmlan_scan_resp;

/** Type definition of mlan_scan_cfg */
typedef struct _mlan_scan_cfg
{
    /** Scan type */
    mlan_scan_type scan_type;
    /** BSS mode for scanning */
    t_u32 scan_mode;
    /** Scan probe */
    t_u32 scan_probe;
    /** Scan time parameters */
    mlan_scan_time_params scan_time;
#if CONFIG_EXT_SCAN_SUPPORT
    /** Extended Scan */
    t_u32 ext_scan;
#if CONFIG_SCAN_CHANNEL_GAP
    /** scan channel gap */
    t_u32 scan_chan_gap;
#endif
#endif
} mlan_scan_cfg, *pmlan_scan_cfg;

/** Type defnition of mlan_ds_scan for MLAN_IOCTL_SCAN */
typedef struct _mlan_ds_scan
{
    /** Sub-command */
    t_u32 sub_command;
    /** Scan request/response */
    union
    {
        /** Scan request */
        mlan_scan_req scan_req;
        /** Scan response */
        mlan_scan_resp scan_resp;
        /** Scan config parameters in user scan */
        mlan_user_scan user_scan;
        /** Scan config parameters */
        mlan_scan_cfg scan_cfg;
    } param;
} mlan_ds_scan, *pmlan_ds_scan;

/*-----------------------------------------------------------------*/
/** BSS Configuration Group */
/*-----------------------------------------------------------------*/
/** Enumeration for BSS mode */
typedef enum _mlan_bss_mode
{
    MLAN_BSS_MODE_NEGATIVE = -1,
    MLAN_BSS_MODE_INFRA    = 1,
    MLAN_BSS_MODE_IBSS,
    MLAN_BSS_MODE_AUTO
} mlan_bss_mode;

/** Maximum key length */
#define MLAN_MAX_KEY_LENGTH 32U

/** max Wmm AC queues */
#define MAX_AC_QUEUES 4

/** Maximum atim window in milliseconds */
#define MLAN_MAX_ATIM_WINDOW 50

/** Minimum beacon interval */
#define MLAN_MIN_BEACON_INTERVAL 20
/** Maximum beacon interval */
#define MLAN_MAX_BEACON_INTERVAL 1000
/** Default beacon interval */
#define MLAN_BEACON_INTERVAL 100

/** Receive all packets */
#define MLAN_PROMISC_MODE 1
/** Receive multicast packets in multicast list */
#define MLAN_MULTICAST_MODE 2
/** Receive all multicast packets */
#define MLAN_ALL_MULTI_MODE 4

/** Maximum size of multicast list */
#define MLAN_MAX_MULTICAST_LIST_SIZE 32U

/** mlan_multicast_list data structure for MLAN_OID_BSS_MULTICAST_LIST */
typedef struct _mlan_multicast_list
{
    /** Multicast mode */
    t_u32 mode;
    /** Number of multicast addresses in the list */
    t_u32 num_multicast_addr;
    /** Multicast address list */
    mlan_802_11_mac_addr mac_list[MLAN_MAX_MULTICAST_LIST_SIZE];
} mlan_multicast_list, *pmlan_multicast_list;

/** World Wide Safe Mode Max channel */
#if CONFIG_5GHz_SUPPORT
#define MLAN_WWSM_MAX_CHANNEL 15
#else
#define MLAN_WWSM_MAX_CHANNEL 11
#endif

/** Max channel */
#if CONFIG_5GHz_SUPPORT
#if (CONFIG_UNII4_BAND_SUPPORT)
#define MLAN_MAX_CHANNEL 177U
#else
#define MLAN_MAX_CHANNEL 165U
#endif
#else
#define MLAN_MAX_CHANNEL 14U
#endif

/** Maximum number of channels in table */
#define MLAN_MAX_CHANNEL_NUM 128

/** Channel/frequence for MLAN_OID_BSS_CHANNEL */
typedef struct _chan_freq
{
    /** Channel Number */
    t_u32 channel;
    /** Frequency of this Channel */
    t_u32 freq;
} chan_freq_t;

/** mlan_chan_list data structure for MLAN_OID_BSS_CHANNEL_LIST */
typedef struct _mlan_chan_list
{
    /** Number of channel */
    t_u32 num_of_chan;
    /** Channel-Frequency table */
    chan_freq_t cf[MLAN_MAX_CHANNEL_NUM];
} mlan_chan_list;

/** mlan_ssid_bssid  data structure for MLAN_OID_BSS_START and MLAN_OID_BSS_FIND_BSS */
typedef struct _mlan_ssid_bssid
{
    /** SSID */
    mlan_802_11_ssid ssid;
    /** BSSID */
    mlan_802_11_mac_addr bssid;
    /** index in BSSID list, start from 1 */
    t_u32 idx;
} mlan_ssid_bssid;

/** Data structure of WMM QoS information */
typedef struct _wmm_qos_info_t
{
    /** Parameter set count */
    t_u8 para_set_count : 4;
    /** Reserved */
    t_u8 reserved : 3;
    /** QoS UAPSD */
    t_u8 qos_uapsd : 1;
} wmm_qos_info_t, *pwmm_qos_info_t;

/** Data structure of WMM ECW */
typedef struct _wmm_ecw_t
{
    /** Minimum Ecw */
    t_u8 ecw_min : 4;
    /** Maximum Ecw */
    t_u8 ecw_max : 4;
} wmm_ecw_t, *pwmm_ecw_t;

/** Data structure of WMM Aci/Aifsn */
typedef struct _wmm_aci_aifsn_t
{
    /** Aifsn */
    t_u8 aifsn : 4;
    /** Acm */
    t_u8 acm : 1;
    /** Aci */
    t_u8 aci : 2;
    /** Reserved */
    t_u8 reserved : 1;
} wmm_aci_aifsn_t, *pwmm_aci_aifsn_t;

/** Data structure of WMM AC parameters  */
typedef struct _wmm_ac_parameters_t
{
    wmm_aci_aifsn_t aci_aifsn; /**< AciAifSn */
    wmm_ecw_t ecw;             /**< Ecw */
    t_u16 tx_op_limit;         /**< Tx op limit */
} wmm_ac_parameters_t, *pwmm_ac_parameters_t;

/** Data structure of WMM parameter IE  */
typedef struct _wmm_parameter_t
{
    /** OuiType:  00:50:f2:02 */
    t_u8 ouitype[4];
    /** Oui subtype: 01 */
    t_u8 ouisubtype;
    /** version: 01 */
    t_u8 version;
    /** QoS information */
    t_u8 qos_info;
    /** Reserved */
    t_u8 reserved;
    /** AC Parameters Record WMM_AC_BE, WMM_AC_BK, WMM_AC_VI, WMM_AC_VO */
    wmm_ac_parameters_t ac_params[MAX_AC_QUEUES];
} wmm_parameter_t, *pwmm_parameter_t;

#if UAP_SUPPORT
/** UAP FLAG: Host based */
#define UAP_FLAG_HOST_BASED MBIT(0)
#if UAP_HOST_MLME
/** UAP FLAG: Host mlme */
#define UAP_FLAG_HOST_MLME MBIT(1)
#endif
#endif

#if CONFIG_11AX
#define MLAN_11AXCMD_SR_SUBID           0x102
#define MLAN_11AXCMD_BEAM_SUBID         0x103
#define MLAN_11AXCMD_HTC_SUBID          0x104
#define MLAN_11AXCMD_TXOMI_SUBID        0x105
#define MLAN_11AXCMD_OBSS_TOLTIME_SUBID 0x106
#define MLAN_11AXCMD_TXOPRTS_SUBID      0x108
#define MLAN_11AXCMD_RUPOWER_SUBID      0x117

#if CONFIG_11AX_TWT
#define MLAN_11AX_TWT_SETUP_SUBID       0x114
#define MLAN_11AX_TWT_TEARDOWN_SUBID    0x115
#define MLAN_11AX_TWT_REPORT_SUBID      0x116
#endif /* CONFIG_11AX_TWT */

#if CONFIG_MMSF
#define MLAN_11AX_DEBUG_MMSF_SUBID 0x12d
#endif
#endif /* CONFIG_11AX */

#if CONFIG_WIFI_RECOVERY
#define MLAN_RECOVERY_TEST_SUBID 0x117
#endif

#if CONFIG_NET_MONITOR
/** Maximum monior mac filter num */
#define MAX_MONIT_MAC_FILTER_NUM 3
#endif

/** Maximum data rates */
#define MAX_DATA_RATES 14U

/** Maximum channel number in bg mode */
#define MAX_CHANNELS_BG 14U

#if UAP_SUPPORT
/** Maximum packet forward control value */
#define MAX_PKT_FWD_CTRL 15
/** Maximum BEACON period */
#define MAX_BEACON_PERIOD 4000U
/** Minimum BEACON period */
#define MIN_BEACON_PERIOD 50U
/** Maximum Channel Switch count */
#define MAX_CHSW_COUNT 15U
/** Minimum Channel Switch count */
#define MIN_CHSW_COUNT 5U
/** Maximum DTIM period */
#define MAX_DTIM_PERIOD 100U
/** Minimum DTIM period */
#define MIN_DTIM_PERIOD 1U
/** Maximum TX Power Limit */
#define MAX_TX_POWER 20
/** Minimum TX Power Limit */
#define MIN_TX_POWER 0
/** MAX station count */
#define MAX_UAP_STA_COUNT 10
/** Maximum RTS threshold */
#define MAX_RTS_THRESHOLD 2347
/** Maximum fragmentation threshold */
#define MAX_FRAG_THRESHOLD 2346
/** Minimum fragmentation threshold */
#define MIN_FRAG_THRESHOLD 256
/** data rate 54 M */
#define DATA_RATE_54M 108
/** Maximum value of bcast_ssid_ctl */
#define MAX_BCAST_SSID_CTL 2
/** antenna A */
#define ANTENNA_MODE_A 0
/** antenna B */
#define ANTENNA_MODE_B 1
/** transmit antenna */
#define TX_ANTENNA 1
/** receive antenna */
#define RX_ANTENNA 0
/** Maximum stage out time */
#define MAX_STAGE_OUT_TIME 864000
/** Minimum stage out time */
#define MIN_STAGE_OUT_TIME 300
/** Maximum Retry Limit */
#define MAX_RETRY_LIMIT 14

/** Maximum group key timer in seconds */
#define MAX_GRP_TIMER 86400

/** Maximum value of 4 byte configuration */
#define MAX_VALID_DWORD 0x7FFFFFFF /* (1 << 31) - 1 */

/** Band config ACS mode */
#define BAND_CONFIG_ACS_MODE 0x40U
/** Band config manual */
#define BAND_CONFIG_MANUAL 0x00

/** auto data rate */
#define DATA_RATE_AUTO 0

/**filter mode: disable */
#define MAC_FILTER_MODE_DISABLE 0
/**filter mode: block mac address */
#define MAC_FILTER_MODE_ALLOW_MAC 1
/**filter mode: block mac address */
#define MAC_FILTER_MODE_BLOCK_MAC 2
/** Maximum mac filter num */
#define MAX_MAC_FILTER_NUM 16

/* Bitmap for protocol to use */
/** No security */
#define PROTOCOL_NO_SECURITY 0x01
/** Static WEP */
#define PROTOCOL_STATIC_WEP 0x02
/** WPA */
#define PROTOCOL_WPA 0x08U
/** WPA2 */
#define PROTOCOL_WPA2 0x20U
/** WP2 Mixed */
#define PROTOCOL_WPA2_MIXED 0x28U
/** EAP */
#define PROTOCOL_EAP 0x40U
/** WAPI */
#define PROTOCOL_WAPI 0x80U
/** WPA3 SAE */
#define PROTOCOL_WPA3_SAE 256U
#if CONFIG_DRIVER_OWE
/** OWE */
#define PROTOCOL_OWE 512U
#endif

/** Key_mgmt_psk_sha256 */
#define KEY_MGMT_PSK_SHA256 0x100
/** Key_mgmt_sae */
#define KEY_MGMT_SAE 0x400
#if CONFIG_DRIVER_OWE
/** Key_mgmt_owe */
#define KEY_MGMT_OWE 0x200
#endif
/** Key_mgmt_none */
#define KEY_MGMT_NONE 0x04
/** Key_mgmt_psk */
#define KEY_MGMT_PSK 0x02
/** Key_mgmt_psk_sha256 */
#define KEY_MGMT_PSK_SHA256 0x100
/** Key_mgmt_eap  */
#define KEY_MGMT_EAP 0x01
/** Key_mgmt_psk sha256 */
#define KEY_MGMT_PSK_SHA256 0x100

/** TKIP */
#define CIPHER_TKIP 0x04
/** AES CCMP */
#define CIPHER_AES_CCMP 0x08

/** Valid cipher bitmap */
#define VALID_CIPHER_BITMAP 0x0cU

/** Channel List Entry */
typedef struct _channel_list
{
    /** Channel Number */
    t_u8 chan_number;
    /** Band Config */
    t_u8 band_config_type;
} scan_chan_list;

/** mac_filter data structure */
typedef struct _mac_filter
{
    /** mac filter mode */
    t_u16 filter_mode;
    /** mac adress count */
    t_u16 mac_count;
    /** mac address list */
    mlan_802_11_mac_addr mac_list[MAX_MAC_FILTER_NUM];
} mac_filter;

/** wpa parameter */
typedef struct _wpa_param
{
    /** Pairwise cipher WPA */
    t_u8 pairwise_cipher_wpa;
    /** Pairwise cipher WPA2 */
    t_u8 pairwise_cipher_wpa2;
    /** group cipher */
    t_u8 group_cipher;
    /** RSN replay protection */
    t_u8 rsn_protection;
    /** passphrase length */
    t_u32 length;
    /** passphrase */
    t_u8 passphrase[64];
    /** password length */
    t_u32 password_length;
    /** wpa3 sae password */
    t_u8 password[255];
    /**group key rekey time in seconds */
    t_u32 gk_rekey_time;
} wpa_param;

/** wep key */
typedef struct _wep_key
{
    /** key index 0-3 */
    t_u8 key_index;
    /** is default */
    t_u8 is_default;
    /** length */
    t_u16 length;
    /** key data */
    t_u8 key[26];
} wep_key;

/** wep param */
typedef struct _wep_param
{
    /** key 0 */
    wep_key key0;
    /** key 1 */
    wep_key key1;
    /** key 2 */
    wep_key key2;
    /** key 3 */
    wep_key key3;
} wep_param;

/** mlan_bss_param
 * Note: For each entry you must enter an invalid value
 * in the MOAL function woal_set_sys_config_invalid_data().
 * Otherwise for a valid data an unwanted TLV will be
 * added to that command.
 */
typedef struct _mlan_uap_bss_param
{
    /** AP mac addr */
    mlan_802_11_mac_addr mac_addr;
    /** SSID */
    mlan_802_11_ssid ssid;
    /** Broadcast ssid control */
    t_u8 bcast_ssid_ctl;
    /** Radio control: on/off */
    t_u8 radio_ctl;
    /** dtim period */
    t_u8 dtim_period;
    /** beacon period */
    t_u16 beacon_period;
    /** channel switch time to send ECSA */
    t_u8 chan_sw_count;
    /** rates */
    t_u8 rates[MAX_DATA_RATES];
    /** Tx data rate */
    t_u16 tx_data_rate;
    /** multicast/broadcast data rate */
    t_u16 mcbc_data_rate;
    /** Tx power level in dBm */
    t_u8 tx_power_level;
    /** Tx antenna */
    t_u8 tx_antenna;
    /** Rx antenna */
    t_u8 rx_antenna;
    /** packet forward control */
    t_u8 pkt_forward_ctl;
    /** max station count */
    t_u16 max_sta_count;
    /** mac filter */
    mac_filter filter;
    /** station ageout timer in unit of 100ms  */
    t_u32 sta_ageout_timer;
    /** PS station ageout timer in unit of 100ms  */
    t_u32 ps_sta_ageout_timer;
    /** RTS threshold */
    t_u16 rts_threshold;
    /** fragmentation threshold */
    t_u16 frag_threshold;
    /**  retry_limit */
    t_u16 retry_limit;
    /**  pairwise update timeout in milliseconds */
    t_u32 pairwise_update_timeout;
    /** pairwise handshake retries */
    t_u32 pwk_retries;
    /**  groupwise update timeout in milliseconds */
    t_u32 groupwise_update_timeout;
    /** groupwise handshake retries */
    t_u32 gwk_retries;
    /** preamble type */
    t_u8 preamble_type;
    /** band cfg */
    t_u8 band_cfg;
    /** channel */
    t_u8 channel;
    /** auth mode */
    t_u16 auth_mode;
    /** PWE derivation */
    t_u8 pwe_derivation;
    /** transition disable */
    t_u8 transition_disable;
    /** encryption protocol */
    t_u16 protocol;
    /** key managment type */
    t_u16 key_mgmt;
    /** wep param */
    wep_param wep_cfg;
    /** wpa param */
    wpa_param wpa_cfg;
    /** Mgmt IE passthru mask */
    t_u32 mgmt_ie_passthru_mask;
    /*
     * 11n HT Cap  HTCap_t  ht_cap
     */
    /** HT Capabilities Info field */
    t_u16 ht_cap_info;
    /** A-MPDU Parameters field */
    t_u8 ampdu_param;
    /** Supported MCS Set field */
    t_u8 supported_mcs_set[16];
    /** HT Extended Capabilities field */
    t_u16 ht_ext_cap;
    /** Transmit Beamforming Capabilities field */
    t_u32 tx_bf_cap;
    /** Antenna Selection Capability field */
    t_u8 asel;
    /** Enable 2040 Coex */
    t_u8 enable_2040coex;
    /** key management operation */
    t_u16 key_mgmt_operation;
    /** BSS status */
    t_u16 bss_status;
    /** Number of channels in scan_channel_list */
    t_u32 num_of_chan;
    /** scan channel list in ACS mode */
    scan_chan_list chan_list[MLAN_MAX_CHANNEL];
    /** Wmm parameters */
    wmm_parameter_t wmm_para;
    /** uap host based config */
    t_u32 uap_host_based_config;
} mlan_uap_bss_param;

/** mlan_deauth_param */
typedef struct _mlan_deauth_param
{
    /** STA mac addr */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** deauth reason */
    t_u16 reason_code;
} mlan_deauth_param;
#endif

#if CONFIG_ECSA
/** mlan_chan_switch_param */
typedef struct _mlan_action_chan_switch
{
    /** mode*/
    t_u8 mode;
    /** switch mode*/
    t_u8 chan_switch_mode;
    /** oper class*/
    t_u8 new_oper_class;
    /** new channel */
    t_u8 new_channel_num;
    /** chan_switch_count */
    t_u8 chan_switch_count;
} mlan_action_chan_switch;

typedef struct _mlan_ds_bw_chan_oper
{
    /* bandwidth 20:20M 40:40M 80:80M*/
    t_u8 bandwidth;
    /* channel number */
    t_u8 channel;
    /* Non-global operating class */
    t_u8 oper_class;
} mlan_ds_bw_chan_oper;
#endif

/** mlan_uap_acs_scan */
typedef struct _mlan_uap_acs_scan
{
    /** band */
    Band_Config_t bandcfg;
    /** channel */
    t_u8 chan;
} mlan_uap_acs_scan;

#if UAP_HOST_MLME
/** station is authorized (802.1X) */
#define STA_FLAG_AUTHORIZED MBIT(1)
/** Station is capable of receiving frames with short barker preamble */
#define STA_FLAG_SHORT_PREAMBLE MBIT(2)
/** station is WME/QoS capable */
#define STA_FLAG_WME MBIT(3)
/** station uses management frame protection */
#define STA_FLAG_MFP MBIT(4)
/** station is authenticated */
#define STA_FLAG_AUTHENTICATED MBIT(5)
/** station is a TDLS peer */
#define STA_FLAG_TDLS_PEER MBIT(6)
/** station is associated */
#define STA_FLAG_ASSOCIATED MBIT(7)
/** mlan_ds_sta_info */
typedef struct _mlan_ds_sta_info
{
    /** aid */
    t_u16 aid;
    /** peer_mac */
    t_u8 peer_mac[MLAN_MAC_ADDR_LENGTH];
    /** Listen Interval */
    int listen_interval;
    /** Capability Info */
    t_u16 cap_info;
    /** station flag */
    t_u32 sta_flags;
    /** tlv len */
    t_u16 tlv_len;
    /** tlv start */
    t_u8 tlv[];
} mlan_ds_sta_info;
#endif

/** Type definition of mlan_embedded_dhcp_config */
typedef MLAN_PACK_START struct _mlan_embedded_dhcp_config
{
    /** Host IP address */
    t_u32 host_ip_addr;
    /** Start IP address */
    t_u32 start_ip_addr;
    /** Sub mask */
    t_u32 subnet_mask;
    /** Lease time */
    t_u32 lease_time;
    /** Limit count */
    t_u8 limit_count;
    /** Enabled/disbaled */
    t_u8 is_enabled;
    /** Get / Set action*/
    t_u16 action;
} MLAN_PACK_END mlan_embedded_dhcp_config, *pmlan_embedded_dhcp_config;

/** Type definition of mlan_ds_bss for MLAN_IOCTL_BSS */
typedef struct _mlan_ds_bss
{
    /** Sub-command */
    mlan_ioctl_req_id sub_command;
    /** BSS parameter */
    union
    {
        /** SSID-BSSID for MLAN_OID_BSS_START */
        mlan_ssid_bssid ssid_bssid;
#if 0
        /** BSSID for MLAN_OID_BSS_STOP */
        mlan_802_11_mac_addr bssid;
        /** BSS mode for MLAN_OID_BSS_MODE */
        t_u32 bss_mode;
        /** BSS channel/frequency for MLAN_OID_BSS_CHANNEL */
        chan_freq bss_chan;
        /** BSS channel list for MLAN_OID_BSS_CHANNEL_LIST */
        mlan_chan_list chanlist;
        /** MAC address for MLAN_OID_BSS_MAC_ADDR */
        mlan_802_11_mac_addr mac_addr;
        /** Multicast list for MLAN_OID_BSS_MULTICAST_LIST */
        mlan_multicast_list multicast_list;
        /** Beacon interval for MLAN_OID_IBSS_BCN_INTERVAL */
        t_u32 bcn_interval;
        /** ATIM window for MLAN_OID_IBSS_ATIM_WINDOW */
        t_u32 atim_window;
#endif
#if UAP_SUPPORT
        /** BSS param for AP mode */
        mlan_uap_bss_param bss_config;
#if CONFIG_ECSA
        /** channel switch for MLAN_OID_UAP_CHAN_SWITCH */
        mlan_action_chan_switch chanswitch;
#endif
#if 0
        /** deauth param for MLAN_OID_UAP_DEAUTH_STA */
        mlan_deauth_param deauth_param;
#endif
#endif
#if defined(STA_SUPPORT) && UAP_SUPPORT
        /** BSS role */
        mlan_bss_role bss_role;
#endif
        /** AP acs scan MLAN_OID_UAP_ACS_SCAN */
        mlan_uap_acs_scan ap_acs_scan;
        /** host based flag for MLAN_OID_BSS_START */
        t_u8 host_based;
#if UAP_HOST_MLME
#if UAP_SUPPORT
        /** STA info for MLAN_OID_UAP_ADD_STATION */
        mlan_ds_sta_info sta_info;
#endif
#endif
    } param;
} mlan_ds_bss, *pmlan_ds_bss;

#ifdef OTP_CHANINFO
/* OTP Region info */
typedef MLAN_PACK_START struct _otp_region_info
{
    t_u8 country_code[2];
    t_u8 region_code;
    t_u8 environment;
    t_u16 force_reg : 1;
    t_u16 reserved : 15;
} MLAN_PACK_END otp_region_info_t;

/** Type definition of mlan_ds_custom_reg_domain */
typedef struct _mlan_ds_custom_reg_domain
{
    otp_region_info_t region;
    /** num of 2g channels in custom_reg_domain */
    t_u8 num_bg_chan;
#if CONFIG_5GHz_SUPPORT
    /** num of 5g channels in custom_reg_domain */
    t_u8 num_a_chan;
#endif
    /** cfp table */
    chan_freq_power_t cfp_tbl[0];
} mlan_ds_custom_reg_domain;
#endif

/*-----------------------------------------------------------------*/
/** Radio Control Group */
/*-----------------------------------------------------------------*/
/** Enumeration for band */
#define BAND_B   1U
#define BAND_G   2U
#define BAND_A   4U
#define BAND_GN  8U
#define BAND_AN  16U
#define BAND_GAC 32U
#define BAND_AAC 64U
#if CONFIG_11AX
#define BAND_GAX 256U
#define BAND_AAX 512U
#endif

#if CONFIG_BG_SCAN
/** band AUTO */
#define WIFI_FREQUENCY_BAND_AUTO 0
/** band 5G */
#define WIFI_FREQUENCY_BAND_5GHZ 1
/** band 2G */
#define WIFI_FREQUENCY_BAND_2GHZ 2
/** All band */
#define WIFI_FREQUENCY_ALL_BAND 3
#endif

/** NO secondary channel */
#define NO_SEC_CHANNEL 0
/** secondary channel is above primary channel */
#define SEC_CHANNEL_ABOVE 1
/** secondary channel is below primary channel */
#define SEC_CHANNEL_BELOW 3
/** secondary channel is 80Mhz bandwidth for 11ac */
#define CHANNEL_BW_80MHZ  4
#define CHANNEL_BW_160MHZ 5
/** Channel bandwidth */
#define CHANNEL_BW_20MHZ       0
#define CHANNEL_BW_40MHZ_ABOVE 1
#define CHANNEL_BW_40MHZ_BELOW 3

/** Type definition of mlan_ds_band_cfg for MLAN_OID_BAND_CFG */
typedef struct _mlan_ds_band_cfg
{
    /** Infra band */
    t_u16 config_bands;
    /** Ad-hoc start band */
    t_u16 adhoc_start_band;
    /** Ad-hoc start channel */
    t_u32 adhoc_channel;
    /** Ad-hoc channel bandwidth */
    t_u32 sec_chan_offset;
    /** fw supported band */
    t_u16 fw_bands;
} mlan_ds_band_cfg;

/** Type definition of mlan_ds_remain_chan for MLAN_OID_REMAIN_CHAN_CFG */
typedef struct _mlan_ds_remain_chan
{
    /** remove flag */
    t_u16 remove;
    /** status */
    t_u8 status;
    /** Band cfg */
    t_u8 bandcfg;
    /** channel */
    t_u8 channel;
    /** remain time: Unit ms*/
    t_u32 remain_period;
} mlan_ds_remain_chan, *pmlan_ds_remain_chan;

/** Type definition of mlan_ds_radio_cfg for MLAN_IOCTL_RADIO_CFG */
typedef struct _mlan_ds_radio_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Radio control parameter */
    union
    {
        /** Radio on/off for MLAN_OID_RADIO_CTRL */
        t_u32 radio_on_off;
        /** Band info for MLAN_OID_BAND_CFG */
        mlan_ds_band_cfg band_cfg;
        /** Antenna info for MLAN_OID_ANT_CFG */
        t_u32 antenna;
    } param;
} mlan_ds_radio_cfg, *pmlan_ds_radio_cfg;

/** Type definition of mlan_ds_ant_cfg_1x1 for MLAN_OID_ANT_CFG */
typedef struct _mlan_ds_ant_cfg_1x1
{
    /** Antenna mode */
    t_u32 antenna;
    /** Evaluate time */
    t_u16 evaluate_time;
    /** Current antenna */
    t_u16 current_antenna;
#ifdef RW610
    /** Evaluate time */
    t_u8 evaluate_mode;
#endif
} mlan_ds_ant_cfg_1x1, *pmlan_ds_ant_cfg_1x1;


/*-----------------------------------------------------------------*/
/** SNMP MIB Group */
/*-----------------------------------------------------------------*/
/** Type definition of mlan_ds_snmp_mib for MLAN_IOCTL_SNMP_MIB */
typedef struct _mlan_ds_snmp_mib
{
    /** Sub-command */
    t_u32 sub_command;
    /** SNMP MIB parameter */
    union
    {
        /** RTS threshold for MLAN_OID_SNMP_MIB_RTS_THRESHOLD */
        t_u32 rts_threshold;
        /** Fragment threshold for MLAN_OID_SNMP_MIB_FRAG_THRESHOLD */
        t_u32 frag_threshold;
        /** Retry count for MLAN_OID_SNMP_MIB_RETRY_COUNT */
        t_u32 retry_count;
#if UAP_SUPPORT
        /** OID value for MLAN_OID_SNMP_MIB_DOT11D/H */
        t_u32 oid_value;
#endif
        /** DTIM period for MLAN_OID_SNMP_MIB_DTIM_PERIOD */
        t_u32 dtim_period;
        /** Control deauth when uap switch channel */
        t_u8 deauthctrl;
    } param;
} mlan_ds_snmp_mib, *pmlan_ds_snmp_mib;

/*-----------------------------------------------------------------*/
/** Status Information Group */
/*-----------------------------------------------------------------*/
/** Enumeration for ad-hoc status */
enum _mlan_adhoc_status
{
    ADHOC_IDLE,
    ADHOC_STARTED,
    ADHOC_JOINED,
    ADHOC_COALESCED,
    ADHOC_STARTING
};

/** Type definition of mlan_ds_get_stats for MLAN_OID_GET_STATS */
typedef struct _mlan_ds_get_stats
{
    /** Statistics counter */
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
} mlan_ds_get_stats, *pmlan_ds_get_stats;

/** Type definition of mlan_ds_uap_stats for MLAN_OID_GET_STATS */
typedef struct _mlan_ds_uap_stats
{
    /** tkip mic failures */
    t_u32 tkip_mic_failures;
    /** ccmp decrypt errors */
    t_u32 ccmp_decrypt_errors;
    /** wep undecryptable count */
    t_u32 wep_undecryptable_count;
    /** wep icv error count */
    t_u32 wep_icv_error_count;
    /** decrypt failure count */
    t_u32 decrypt_failure_count;
    /** dot11 multicast tx count */
    t_u32 mcast_tx_count;
    /** dot11 failed count */
    t_u32 failed_count;
    /** dot11 retry count */
    t_u32 retry_count;
    /** dot11 multi retry count */
    t_u32 multi_retry_count;
    /** dot11 frame duplicate count */
    t_u32 frame_dup_count;
    /** dot11 rts success count */
    t_u32 rts_success_count;
    /** dot11 rts failure count */
    t_u32 rts_failure_count;
    /** dot11 ack failure count */
    t_u32 ack_failure_count;
    /** dot11 rx ragment count */
    t_u32 rx_fragment_count;
    /** dot11 mcast rx frame count */
    t_u32 mcast_rx_frame_count;
    /** dot11 fcs error count */
    t_u32 fcs_error_count;
    /** dot11 tx frame count */
    t_u32 tx_frame_count;
    /** dot11 rsna tkip cm invoked */
    t_u32 rsna_tkip_cm_invoked;
    /** dot11 rsna 4way handshake failures */
    t_u32 rsna_4way_hshk_failures;
} mlan_ds_uap_stats, *pmlan_ds_uap_stats;

/** Mask of last beacon RSSI */
#define BCN_RSSI_LAST_MASK 0x00000001
/** Mask of average beacon RSSI */
#define BCN_RSSI_AVG_MASK 0x00000002
/** Mask of last data RSSI */
#define DATA_RSSI_LAST_MASK 0x00000004
/** Mask of average data RSSI */
#define DATA_RSSI_AVG_MASK 0x00000008
/** Mask of last beacon SNR */
#define BCN_SNR_LAST_MASK 0x00000010
/** Mask of average beacon SNR */
#define BCN_SNR_AVG_MASK 0x00000020
/** Mask of last data SNR */
#define DATA_SNR_LAST_MASK 0x00000040
/** Mask of average data SNR */
#define DATA_SNR_AVG_MASK 0x00000080
/** Mask of last beacon NF */
#define BCN_NF_LAST_MASK 0x00000100
/** Mask of average beacon NF */
#define BCN_NF_AVG_MASK 0x00000200
/** Mask of last data NF */
#define DATA_NF_LAST_MASK 0x00000400
/** Mask of average data NF */
#define DATA_NF_AVG_MASK 0x00000800
/** Mask of all RSSI_INFO */
#define ALL_RSSI_INFO_MASK 0x00000fff

/** Type definition of mlan_ds_get_signal for MLAN_OID_GET_SIGNAL */
typedef struct _mlan_ds_get_signal
{
    /** Selector of get operation */
    /*
     * Bit0:  Last Beacon RSSI,  Bit1:  Average Beacon RSSI,
     * Bit2:  Last Data RSSI,    Bit3:  Average Data RSSI,
     * Bit4:  Last Beacon SNR,   Bit5:  Average Beacon SNR,
     * Bit6:  Last Data SNR,     Bit7:  Average Data SNR,
     * Bit8:  Last Beacon NF,    Bit9:  Average Beacon NF,
     * Bit10: Last Data NF,      Bit11: Average Data NF
     */
    t_u16 selector;

    /** RSSI */
    /** RSSI of last beacon */
    t_s16 bcn_rssi_last;
    /** RSSI of beacon average */
    t_s16 bcn_rssi_avg;
    /** RSSI of last data packet */
    t_s16 data_rssi_last;
    /** RSSI of data packet average */
    t_s16 data_rssi_avg;

    /** SNR */
    /** SNR of last beacon */
    t_s16 bcn_snr_last;
    /** SNR of beacon average */
    t_s16 bcn_snr_avg;
    /** SNR of last data packet */
    t_s16 data_snr_last;
    /** SNR of data packet average */
    t_s16 data_snr_avg;

    /** NF */
    /** NF of last beacon */
    t_s16 bcn_nf_last;
    /** NF of beacon average */
    t_s16 bcn_nf_avg;
    /** NF of last data packet */
    t_s16 data_nf_last;
    /** NF of data packet average */
    t_s16 data_nf_avg;
} mlan_ds_get_signal, *pmlan_ds_get_signal;


/** mlan_fw_info data structure for MLAN_OID_GET_FW_INFO */
typedef struct _mlan_fw_info
{
    /** Firmware version */
    t_u32 fw_ver;
    /** MAC address */
    mlan_802_11_mac_addr mac_addr;
    /** Device support for MIMO abstraction of MCSs */
    t_u8 hw_dev_mcs_support;
    /** fw supported band */
    t_u16 fw_bands;
#if CONFIG_11AX
    /** length of hw he capability */
    t_u8 hw_hecap_len;
    /** 802.11ax HE capability */
    t_u8 hw_he_cap[54];
    /** length of hw 2.4G he capability */
    t_u8 hw_2g_hecap_len;
    /** 802.11ax 2.4G HE capability */
    t_u8 hw_2g_he_cap[54];
#ifdef ENABLE_802_116E
    /** 802.11ax 6G HE capability */
    t_u16 hw_he_6g_cap;
#endif
#endif
} mlan_fw_info, *pmlan_fw_info;

/** Version string buffer length */
#define MLAN_MAX_VER_STR_LEN 128

/** mlan_ver_ext data structure for MLAN_OID_GET_VER_EXT */
typedef struct _mlan_ver_ext
{
    /** Selected version string */
    t_u32 version_str_sel;
    /** Version string */
    char version_str[MLAN_MAX_VER_STR_LEN];
} mlan_ver_ext, *pmlan_ver_ext;

/** mlan_bss_info data structure for MLAN_OID_GET_BSS_INFO */
typedef struct _mlan_bss_info
{
    /** BSS mode */
    mlan_bss_mode bss_mode;
    /** SSID */
    mlan_802_11_ssid ssid;
    /** Table index */
    t_u32 scan_table_idx;
    /** Channel */
    t_u32 bss_chan;
    /** Band */
    t_u8 bss_band;
    /** Region code */
    t_u32 region_code;
    /** Connection status */
    t_u32 media_connected;
    /** Radio on */
    t_u32 radio_on;
    /** Max power level in dBm */
    t_u32 max_power_level;
    /** Min power level in dBm */
    t_u32 min_power_level;
    /** Adhoc state */
    t_u32 adhoc_state;
    /** NF of last beacon */
    t_s32 bcn_nf_last;
    /** wep status */
    t_u32 wep_status;
    /** Host Sleep configured flag */
    t_u32 is_hs_configured;
    /** Deep Sleep flag */
    t_u32 is_deep_sleep;
    /** BSSID */
    mlan_802_11_mac_addr bssid;
#ifdef STA_SUPPORT
    /** Capability Info */
    t_u16 capability_info;
    /** Beacon Interval */
    t_u16 beacon_interval;
    /** Listen Interval */
    t_u16 listen_interval;
    /** Association Id  */
    t_u16 assoc_id;
    /** AP/Peer supported rates */
    t_u8 peer_supp_rates[MLAN_SUPPORTED_RATES];
#endif /* STA_SUPPORT */
} mlan_bss_info, *pmlan_bss_info;

/** MAXIMUM number of TID */
#define MAX_NUM_TID 8U

/** Max RX Win size */
#define MAX_RX_WINSIZE 64

/** rx_reorder_tbl */
typedef struct
{
    /** TID */
    t_u16 tid;
    /** TA */
    t_u8 ta[MLAN_MAC_ADDR_LENGTH];
    /** Start window */
    t_u32 start_win;
    /** Window size */
    t_u32 win_size;
    /** amsdu flag */
    t_u8 amsdu;
    /** buffer status */
    t_u32 buffer[MAX_RX_WINSIZE];
} rx_reorder_tbl;

/** tx_ba_stream_tbl */
typedef struct
{
    /** TID */
    t_u16 tid;
    /** RA */
    t_u8 ra[MLAN_MAC_ADDR_LENGTH];
    /** amsdu flag */
    t_u8 amsdu;
} tx_ba_stream_tbl;

/** Debug command number */
#define DBG_CMD_NUM 5

/** mlan_debug_info data structure for MLAN_OID_GET_DEBUG_INFO */
typedef struct _mlan_debug_info
{
    /* WMM AC_BK count */
    t_u32 wmm_ac_bk;
    /* WMM AC_BE count */
    t_u32 wmm_ac_be;
    /* WMM AC_VI count */
    t_u32 wmm_ac_vi;
    /* WMM AC_VO count */
    t_u32 wmm_ac_vo;
    /** Corresponds to max_tx_buf_size member of mlan_adapter*/
    t_u32 max_tx_buf_size;
    /** Corresponds to tx_buf_size member of mlan_adapter*/
    t_u32 tx_buf_size;
    /** Corresponds to curr_tx_buf_size member of mlan_adapter*/
    t_u32 curr_tx_buf_size;
    /** Tx table num */
    t_u32 tx_tbl_num;
    /** Tx ba stream table */
    tx_ba_stream_tbl tx_tbl[MLAN_MAX_TX_BASTREAM_SUPPORTED];
    /** Rx table num */
    t_u32 rx_tbl_num;
    /** Rx reorder table*/
    rx_reorder_tbl rx_tbl[MLAN_MAX_RX_BASTREAM_SUPPORTED];
    /** Corresponds to ps_mode member of mlan_adapter */
    t_u16 ps_mode;
    /** Corresponds to ps_state member of mlan_adapter */
    t_u32 ps_state;
#ifdef STA_SUPPORT
    /** Corresponds to is_deep_sleep member of mlan_adapter */
    t_u8 is_deep_sleep;
#endif /** STA_SUPPORT */
    /** Corresponds to pm_wakeup_card_req member of mlan_adapter */
    t_u8 pm_wakeup_card_req;
    /** Corresponds to pm_wakeup_fw_try member of mlan_adapter */
    t_u32 pm_wakeup_fw_try;
    /** Corresponds to is_hs_configured member of mlan_adapter */
    t_u8 is_hs_configured;
    /** Corresponds to hs_activated member of mlan_adapter */
    t_u8 hs_activated;
    /** Corresponds to pps_uapsd_mode member of mlan_adapter */
    t_u16 pps_uapsd_mode;
    /** Corresponds to sleep_period.period member of mlan_adapter */
    t_u16 sleep_pd;
    /** Corresponds to wmm_qosinfo member of mlan_private */
    t_u8 qos_cfg;
    /** Corresponds to tx_lock_flag member of mlan_adapter */
    t_u8 tx_lock_flag;
    /** Corresponds to port_open member of mlan_private */
    t_u8 port_open;
    /** Corresponds to scan_processing member of mlan_adapter */
    t_u32 scan_processing;
    /** Number of host to card command failures */
    t_u32 num_cmd_host_to_card_failure;
    /** Number of host to card sleep confirm failures */
    t_u32 num_cmd_sleep_cfm_host_to_card_failure;
    /** Number of host to card Tx failures */
    t_u32 num_tx_host_to_card_failure;
    /** Number of card to host command/event failures */
    t_u32 num_cmdevt_card_to_host_failure;
    /** Number of card to host Rx failures */
    t_u32 num_rx_card_to_host_failure;
    /** Number of interrupt read failures */
    t_u32 num_int_read_failure;
    /** Last interrupt status */
    t_u32 last_int_status;
    /** Number of deauthentication events */
    t_u32 num_event_deauth;
    /** Number of disassosiation events */
    t_u32 num_event_disassoc;
    /** Number of link lost events */
    t_u32 num_event_link_lost;
    /** Number of deauthentication commands */
    t_u32 num_cmd_deauth;
    /** Number of association comamnd successes */
    t_u32 num_cmd_assoc_success;
    /** Number of association command failures */
    t_u32 num_cmd_assoc_failure;
    /** Number of Tx timeouts */
    t_u32 num_tx_timeout;
    /** Number of command timeouts */
    t_u32 num_cmd_timeout;
    /** Timeout command ID */
    t_u16 timeout_cmd_id;
    /** Timeout command action */
    t_u16 timeout_cmd_act;
    /** List of last command IDs */
    t_u16 last_cmd_id[DBG_CMD_NUM];
    /** List of last command actions */
    t_u16 last_cmd_act[DBG_CMD_NUM];
    /** Last command index */
    t_u16 last_cmd_index;
    /** List of last command response IDs */
    t_u16 last_cmd_resp_id[DBG_CMD_NUM];
    /** Last command response index */
    t_u16 last_cmd_resp_index;
    /** List of last events */
    t_u16 last_event[DBG_CMD_NUM];
    /** Last event index */
    t_u16 last_event_index;

    /** Corresponds to data_sent member of mlan_adapter */
    t_u8 data_sent;
    /** Corresponds to cmd_sent member of mlan_adapter */
    t_u8 cmd_sent;
    /** SDIO multiple port read bitmap */
    t_u32 mp_rd_bitmap;
    /** SDIO multiple port write bitmap */
    t_u32 mp_wr_bitmap;
    /** Current available port for read */
    t_u8 curr_rd_port;
    /** Current available port for write */
    t_u8 curr_wr_port;
    /** Corresponds to cmdresp_received member of mlan_adapter */
    t_u8 cmd_resp_received;
    /** Corresponds to event_received member of mlan_adapter */
    t_u8 event_received;
    /**  pendig tx pkts */
    t_u32 tx_pkts_queued;
#if UAP_SUPPORT
    /**  pending bridge pkts */
    t_u16 num_bridge_pkts;
    /**  dropped pkts */
    t_u32 num_drop_pkts;
#endif
} mlan_debug_info, *pmlan_debug_info;

#if UAP_SUPPORT
/** Maximum number of clients supported by AP */
#define MAX_NUM_CLIENTS 16U

/** station info */
typedef struct _sta_info
{
    /** STA MAC address */
    t_u8 mac_address[MLAN_MAC_ADDR_LENGTH];
    /** Power mfg status */
    t_u8 power_mfg_status;
    /** RSSI */
    t_s8 rssi;
} sta_info;

/** mlan_ds_sta_list structure for MLAN_OID_UAP_STA_LIST */
typedef struct _mlan_ds_sta_list
{
    /** station count */
    t_u16 sta_count;
    /** station list */
    sta_info info[MAX_NUM_CLIENTS];
} mlan_ds_sta_list, *pmlan_ds_sta_list;
#endif

/** Type definition of mlan_ds_get_info for MLAN_IOCTL_GET_INFO */
typedef struct _mlan_ds_get_info
{
    /** Sub-command */
    t_u32 sub_command;

    /** Status information parameter */
    union
    {
        /** Signal information for MLAN_OID_GET_SIGNAL */
        mlan_ds_get_signal signal;
        /** Statistics information for MLAN_OID_GET_STATS */
        mlan_ds_get_stats stats;
        /** Firmware information for MLAN_OID_GET_FW_INFO */
        mlan_fw_info fw_info;
        /** Extended version information for MLAN_OID_GET_VER_EXT */
        mlan_ver_ext ver_ext;
        /** BSS information for MLAN_OID_GET_BSS_INFO */
        mlan_bss_info bss_info;
        /** Debug information for MLAN_OID_GET_DEBUG_INFO */
        mlan_debug_info debug_info;
#if UAP_SUPPORT
        /** UAP Statistics information for MLAN_OID_GET_STATS */
        mlan_ds_uap_stats ustats;
        /** UAP station list for MLAN_OID_UAP_STA_LIST */
        mlan_ds_sta_list sta_list;
#endif
    } param;
} mlan_ds_get_info, *pmlan_ds_get_info;

/*-----------------------------------------------------------------*/
/** Security Configuration Group */
/*-----------------------------------------------------------------*/
/** Enumeration for authentication mode */
typedef enum _mlan_auth_mode
{
    MLAN_AUTH_MODE_OPEN   = 0x00,
    MLAN_AUTH_MODE_SHARED = 0x01,
    MLAN_AUTH_MODE_FT     = 0x02,
    MLAN_AUTH_MODE_SAE    = 0x03,
#if CONFIG_DRIVER_OWE
    MLAN_AUTH_MODE_OWE = 0x04,
#endif
    MLAN_AUTH_MODE_NETWORKEAP = 0x80,
    MLAN_AUTH_MODE_AUTO       = 0xFF,
} mlan_auth_mode;

/**Enumeration for AssocAgent authentication mode, sync from FW.*/
typedef enum
{
    AssocAgentAuth_Open,
    AssocAgentAuth_Shared,
    AssocAgentAuth_FastBss,
    AssocAgentAuth_FastBss_Skip,
    AssocAgentAuth_FastBss_SAE,
    AssocAgentAuth_Wpa3Sae = 6,
#if CONFIG_DRIVER_OWE
    AssocAgentAuth_Owe = 7,
#endif
    AssocAgentAuth_Auto,
} AssocAgentAuthType_e;

/** Enumeration for encryption mode */
typedef enum _mlan_encryption_mode
{
    MLAN_ENCRYPTION_MODE_NONE   = 0,
    MLAN_ENCRYPTION_MODE_WEP40  = 1,
    MLAN_ENCRYPTION_MODE_TKIP   = 2,
    MLAN_ENCRYPTION_MODE_CCMP   = 3,
    MLAN_ENCRYPTION_MODE_WEP104 = 4,
} mlan_encryption_mode;

/** Enumeration for PSK */
typedef enum _mlan_psk_type
{
    MLAN_PSK_PASSPHRASE = 1,
    MLAN_PSK_PMK,
    MLAN_PSK_CLEAR,
    MLAN_PSK_QUERY,
    MLAN_PSK_PASSWORD,
} mlan_psk_type;

/** The bit to indicate the key is for unicast */
#define MLAN_KEY_INDEX_UNICAST 0x40000000
/** The key index to indicate default key */
#define MLAN_KEY_INDEX_DEFAULT 0x000000ffU
/** Maximum key length */
// #define MLAN_MAX_KEY_LENGTH        32
/** Minimum passphrase length */
#define MLAN_MIN_PASSPHRASE_LENGTH 8U
/** Maximum passphrase length */
/** FIXME: This macro is used for both passphrase and PSK length
 * As per standards maximum passphrase length is 63 and
 * maximum PSK length is 64.
 *
 * For now setting it to 64 to avoid 4 way Handshake timeout issue
 * if connection attempt is with PSK.
 */
#define MLAN_MAX_PASSPHRASE_LENGTH 64U
/** PMK length */
#define MLAN_PMK_HEXSTR_LENGTH 64
/* A few details needed for WEP (Wireless Equivalent Privacy) */
/** Minimum password length */
#define MLAN_MIN_PASSWORD_LENGTH 1U
/** Maximum passphrase length */
#define MLAN_MAX_PASSWORD_LENGTH 255U
/** 104 bits */
#define MAX_WEP_KEY_SIZE 13U
/** 40 bits RC4 - WEP */
#define MIN_WEP_KEY_SIZE 5
/** packet number size */
#define PN_SIZE 16
/** max seq size of wpa/wpa2 key */
#define SEQ_MAX_SIZE 8

/** key flag for tx_seq */
#define KEY_FLAG_TX_SEQ_VALID 0x00000001
/** key flag for rx_seq */
#define KEY_FLAG_RX_SEQ_VALID 0x00000002
/** key flag for group key */
#define KEY_FLAG_GROUP_KEY 0x00000004U
/** key flag for tx and rx */
#define KEY_FLAG_SET_TX_KEY 0x00000008U
/** key flag for mcast IGTK */
#define KEY_FLAG_AES_MCAST_IGTK 0x00000010U
#ifdef MAC80211_SUPPORT_MESH
/** key flag for mesh group Rx key */
#define KEY_FLAG_SET_GRP_TX_KEY 0x00000100
#endif
/** key flag for remove key */
#define KEY_FLAG_REMOVE_KEY 0x80000000
/** key flag for GCMP */
#define KEY_FLAG_GCMP 0x00000020
/** key flag for GCMP_256 */
#define KEY_FLAG_GCMP_256 0x00000040
/** key flag for ccmp 256 */
#define KEY_FLAG_CCMP_256 0x00000080
/** key flag for GMAC_128 */
#define KEY_FLAG_GMAC_128 0x00000100
/** key flag for GMAC_256 */
#define KEY_FLAG_GMAC_256 0x00000200

/* Clear all key indexes */
#define KEY_INDEX_CLEAR_ALL 0x0000000F

/** Type definition of mlan_ds_encrypt_key for MLAN_OID_SEC_CFG_ENCRYPT_KEY */
typedef struct _mlan_ds_encrypt_key
{
    /** Key disabled, all other fields will be ignore when this flag set to MTRUE */
    t_u32 key_disable;
    /** key removed flag, when this flag is set to MTRUE, only key_index will be check */
    t_u32 key_remove;
    /** Key index, used as current tx key index when is_current_wep_key is set to MTRUE */
    t_u32 key_index;
    /** Current Tx key flag */
    bool is_current_wep_key;
    /** Key length */
    t_u32 key_len;
    /** Key */
    t_u8 key_material[MLAN_MAX_KEY_LENGTH];
    /** mac address */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** wapi key flag */
    t_u32 is_wapi_key;
    /** Initial packet number */
    t_u8 pn[PN_SIZE];
    /** key flags */
    t_u32 key_flags;
} mlan_ds_encrypt_key, *pmlan_ds_encrypt_key;

/** Type definition of mlan_passphrase_t */
typedef struct _mlan_passphrase_t
{
    /** Length of passphrase */
    t_u32 passphrase_len;
    /** Passphrase */
    t_u8 passphrase[MLAN_MAX_PASSPHRASE_LENGTH];
} mlan_passphrase_t;

/** Type defnition of mlan_pmk_t */
typedef struct _mlan_pmk_t
{
    /** PMK */
    t_u8 pmk[MLAN_MAX_KEY_LENGTH];
} mlan_pmk_t;

/** Type definition of mlan_passphrase_t */
typedef struct _mlan_password_t
{
    /** Length of passphrase */
    t_u32 password_len;
    /** Passphrase */
    t_u8 password[MLAN_MAX_PASSWORD_LENGTH];
} mlan_password_t;

/** Embedded supplicant RSN type: No RSN */
#define RSN_TYPE_NO_RSN MBIT(0)
/** Embedded supplicant RSN type: WPA */
#define RSN_TYPE_WPA MBIT(3)
/** Embedded supplicant RSN type: WPA-NONE */
#define RSN_TYPE_WPANONE MBIT(4)
/** Embedded supplicant RSN type: WPA2 */
#define RSN_TYPE_WPA2 MBIT(5)
/** Embedded supplicant RSN type: RFU */
#define RSN_TYPE_VALID_BITS (RSN_TYPE_NO_RSN | RSN_TYPE_WPA | RSN_TYPE_WPANONE | RSN_TYPE_WPA2)

/** Embedded supplicant cipher type: TKIP */
#define EMBED_CIPHER_TKIP MBIT(2)
/** Embedded supplicant cipher type: AES */
#define EMBED_CIPHER_AES MBIT(3)
/** Embedded supplicant cipher type: RFU */
#define EMBED_CIPHER_VALID_BITS (EMBED_CIPHER_TKIP | EMBED_CIPHER_AES)

/** Type definition of mlan_ds_passphrase for MLAN_OID_SEC_CFG_PASSPHRASE */
typedef struct _mlan_ds_passphrase
{
    /** SSID may be used */
    mlan_802_11_ssid ssid;
    /** BSSID may be used */
    mlan_802_11_mac_addr bssid;
    /** Flag for passphrase or pmk used */
    mlan_psk_type psk_type;
    /** Passphrase or PMK */
    union
    {
        /** Passphrase */
        mlan_passphrase_t passphrase;
        /** PMK */
        mlan_pmk_t pmk;
    } psk;
    /** Password */
    mlan_password_t password;
} mlan_ds_passphrase, *pmlan_ds_passphrase;

/** Type definition of mlan_ds_esupp_mode for MLAN_OID_SEC_CFG_ESUPP_MODE */
typedef struct _mlan_ds_ewpa_mode
{
    /** RSN mode */
    t_u32 rsn_mode;
    /** Active pairwise cipher */
    t_u32 act_paircipher;
    /** Active pairwise cipher */
    t_u32 act_groupcipher;
} mlan_ds_esupp_mode, *pmlan_ds_esupp_mode;

/** Type definition of mlan_ds_sec_cfg for MLAN_IOCTL_SEC_CFG */
typedef struct _mlan_ds_sec_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Security configuration parameter */
    union
    {
        /** Authentication mode for MLAN_OID_SEC_CFG_AUTH_MODE */
        t_u32 auth_mode;
        /** Encryption mode for MLAN_OID_SEC_CFG_ENCRYPT_MODE */
        t_u32 encrypt_mode;
        /** WPA enabled flag for MLAN_OID_SEC_CFG_WPA_ENABLED */
        t_u32 wpa_enabled;
        /** WAPI enabled flag for MLAN_OID_SEC_CFG_WAPI_ENABLED */
        t_u32 wapi_enabled;
        /** Port Control enabled flag for MLAN_OID_SEC_CFG_PORT_CTRL */
        t_u32 port_ctrl_enabled;
        /** Encryption key for MLAN_OID_SEC_CFG_ENCRYPT_KEY */
        mlan_ds_encrypt_key encrypt_key;
        /** Passphrase for MLAN_OID_SEC_CFG_PASSPHRASE */
        mlan_ds_passphrase passphrase;
        /** Embedded supplicant WPA enabled flag for MLAN_OID_SEC_CFG_EWPA_ENABLED */
        t_u32 ewpa_enabled;
        /** Embedded supplicant mode for MLAN_OID_SEC_CFG_ESUPP_MODE */
        mlan_ds_esupp_mode esupp_mode;
    } param;
} mlan_ds_sec_cfg, *pmlan_ds_sec_cfg;

/*-----------------------------------------------------------------*/
/** Rate Configuration Group */
/*-----------------------------------------------------------------*/
/** Enumeration for rate type */
enum _mlan_rate_type
{
    MLAN_RATE_INDEX,
    MLAN_RATE_VALUE,
    MLAN_RATE_BITMAP
};

/** Enumeration for rate format */
typedef enum _mlan_rate_format
{
    MLAN_RATE_FORMAT_LG = 0,
    MLAN_RATE_FORMAT_HT,
#if CONFIG_11AC
    MLAN_RATE_FORMAT_VHT,
#endif
#if CONFIG_11AX
    MLAN_RATE_FORMAT_HE,
#endif
    MLAN_RATE_FORMAT_AUTO = 0xFF,
} mlan_rate_format;

#if CONFIG_11AX
#define MAX_BITMAP_RATES_SIZE 26
#else
/** Max bitmap rates size */
#define MAX_BITMAP_RATES_SIZE 18
#endif

/** Type definition of mlan_rate_cfg_t for MLAN_OID_RATE_CFG */
typedef struct _mlan_rate_cfg_t
{
    /** Fixed rate: 0, auto rate: 1 */
    bool is_rate_auto;
    /** Rate type. 0: index; 1: value; 2: bitmap */
    t_u32 rate_type;
    /** Rate/MCS index or rate value if fixed rate */
    t_u32 rate;
    /** Rate Bitmap */
    t_u16 bitmap_rates[MAX_BITMAP_RATES_SIZE];
#if (CONFIG_11AC) || (CONFIG_11AX)
    /** NSS */
    t_u32 nss;
#endif
    /* LG rate: 0, HT rate: 1, VHT rate: 2 */
    mlan_rate_format rate_format;
    /** Rate Setting */
    t_u16 rate_setting;
} mlan_rate_cfg_t;

/** HT channel bandwidth */
typedef enum _mlan_ht_bw
{
    MLAN_HT_BW20,
    MLAN_HT_BW40,
#if CONFIG_11AC
    MLAN_VHT_BW80,
    MLAN_VHT_BW160,
#endif
} mlan_ht_bw;

/** HT guard interval */
typedef enum _mlan_ht_gi
{
    MLAN_HT_LGI,
    MLAN_HT_SGI,
} mlan_ht_gi;

#if CONFIG_11AC
typedef enum _mlan_vht_stbc
{
    MLAN_VHT_STBC,
    MLAN_VHT_NO_STBC,
} mlan_vht_stbc;

typedef enum _mlan_vht_ldpc
{
    MLAN_VHT_LDPC,
    MLAN_VHT_NO_LDPC,
} mlan_vht_ldpc;
#endif

/** Band and BSS mode */
typedef struct _mlan_band_data_rate
{
    /** Band configuration */
    t_u16 config_bands;
    /** BSS mode (Infra or IBSS) */
    mlan_bss_mode bss_mode;
} mlan_band_data_rate;

/** Type definition of mlan_data_rate for MLAN_OID_GET_DATA_RATE */
typedef struct _mlan_data_rate
{
    /** Tx data rate */
    t_u32 tx_data_rate;
    /** Rx data rate */
    t_u32 rx_data_rate;

    /** Tx channel bandwidth */
    t_u32 tx_ht_bw;
    /** Tx guard interval */
    t_u32 tx_ht_gi;
    /** Rx channel bandwidth */
    t_u32 rx_ht_bw;
    /** Rx guard interval */
    t_u32 rx_ht_gi;

#ifndef SD8801
    /** MCS index */
    t_u32 tx_mcs_index;
    t_u32 rx_mcs_index;

#if (CONFIG_11AC) || (CONFIG_11AX)
    /** NSS */
    t_u32 tx_nss;
    t_u32 rx_nss;
#endif

    /* LG rate: 0, HT rate: 1, VHT rate: 2 */
    mlan_rate_format tx_rate_format;
    mlan_rate_format rx_rate_format;
#endif
} mlan_data_rate;

/** Type definition of mlan_ds_rate for MLAN_IOCTL_RATE */
typedef struct _mlan_ds_rate
{
    /** Sub-command */
    mlan_ioctl_req_id sub_command;
#if CONFIG_AUTO_NULL_TX
    /** Only set auto tx fix rate */
    t_u16 auto_null_fixrate_enable;
#endif
    /** Rate configuration parameter */
    union
    {
        /** Rate configuration for MLAN_OID_RATE_CFG */
        mlan_rate_cfg_t rate_cfg;
        /** Data rate for MLAN_OID_GET_DATA_RATE */
        mlan_data_rate data_rate;
        /** Supported rates for MLAN_OID_SUPPORTED_RATES */
        t_u8 rates[MLAN_SUPPORTED_RATES];
        /** Band/BSS mode for getting supported rates */
        mlan_band_data_rate rate_band_cfg;
    } param;
} mlan_ds_rate, *pmlan_ds_rate;

/*-----------------------------------------------------------------*/
/** Power Configuration Group */
/*-----------------------------------------------------------------*/

/** Type definition of mlan_power_cfg_t for MLAN_OID_POWER_CFG */
typedef struct _mlan_power_cfg_t
{
    /** Is power auto */
    bool is_power_auto;
    /** Power level in dBm */
    t_u32 power_level;
} mlan_power_cfg_t;

/** max power table size */
#define MAX_POWER_TABLE_SIZE 128

/** The HT BW40 bit in Tx rate index */
#define TX_RATE_HT_BW40_BIT MBIT(7)

/** Type definition of mlan_power_cfg_ext for MLAN_OID_POWER_CFG_EXT */
typedef struct _mlan_power_cfg_ext
{
    /** Length of power_data */
    t_u32 len;
    /** Buffer of power configuration data */
    t_u32 power_data[MAX_POWER_TABLE_SIZE];
} mlan_power_cfg_ext;

/** Type definition of mlan_ds_power_cfg for MLAN_IOCTL_POWER_CFG */
typedef struct _mlan_ds_power_cfg
{
    /** Sub-command */
    mlan_ioctl_req_id sub_command;
    /** Power configuration parameter */
    union
    {
        /** Power configuration for MLAN_OID_POWER_CFG */
        mlan_power_cfg_t power_cfg;
        /** Extended power configuration for MLAN_OID_POWER_CFG_EXT */
        mlan_power_cfg_ext power_ext;
    } param;
} mlan_ds_power_cfg, *pmlan_ds_power_cfg;

/*-----------------------------------------------------------------*/
/** Power Management Configuration Group */
/*-----------------------------------------------------------------*/
#if CONFIG_HOST_SLEEP
/** Host sleep config conditions : Cancel */
#define HOST_SLEEP_CFG_CANCEL 0xffffffffU
/** Host sleep config conditions : NULL (used for offload features) */
#define HOST_SLEEP_NO_COND 0

/** Host sleep config condition: broadcast data */
#define HOST_SLEEP_COND_BROADCAST_DATA MBIT(0)
/** Host sleep config condition: unicast data */
#define HOST_SLEEP_COND_UNICAST_DATA MBIT(1)
/** Host sleep config condition: mac event */
#define HOST_SLEEP_COND_MAC_EVENT MBIT(2)
/** Host sleep config condition: multicast data */
#define HOST_SLEEP_COND_MULTICAST_DATA MBIT(3)

/** Host sleep config conditions: Default */
#define HOST_SLEEP_DEF_COND (HOST_SLEEP_COND_BROADCAST_DATA | HOST_SLEEP_COND_UNICAST_DATA | HOST_SLEEP_COND_MAC_EVENT)
/** Host sleep config GPIO : Default */
#define HOST_SLEEP_DEF_GPIO 0xff
/** Host sleep config gap : Default */
#define HOST_SLEEP_DEF_GAP 200

/** Type definition of mlan_ds_hs_cfg for MLAN_OID_PM_CFG_HS_CFG */
typedef struct _mlan_ds_hs_cfg
{
    /** MTRUE to invoke the HostCmd, MFALSE otherwise */
    t_u32 is_invoke_hostcmd;
    /** Host sleep config condition */
    /** Bit0: broadcast data
     *  Bit1: unicast data
     *  Bit2: mac event
     *  Bit3: multicast data
     */
    t_u32 conditions;
    /** GPIO pin or 0xff for interface */
    t_u32 gpio;
    /** Gap in milliseconds or or 0xff for special setting when GPIO is used to wakeup host */
    t_u32 gap;
} mlan_ds_hs_cfg, *pmlan_ds_hs_cfg;
#endif

/** Enable deep sleep mode */
#define DEEP_SLEEP_ON 1
/** Disable deep sleep mode */
#define DEEP_SLEEP_OFF 0

/** Default idle time in milliseconds for auto deep sleep */
#if defined(SD9177) || defined(IW610)
#define DEEP_SLEEP_IDLE_TIME 300
#else
#define DEEP_SLEEP_IDLE_TIME 100
#endif

typedef struct _mlan_ds_auto_ds
{
    /** auto ds mode, 0 - disable, 1 - enable */
    t_u16 auto_ds;
    /** auto ds idle time in milliseconds */
    t_u16 idletime;
} mlan_ds_auto_ds;

/** Type definition of mlan_ds_inactivity_to for MLAN_OID_PM_CFG_INACTIVITY_TO */
typedef struct _mlan_ds_inactivity_to
{
    /** Timeout unit in microsecond, 0 means 1000us (1ms) */
    t_u32 timeout_unit;
    /** Inactivity timeout for unicast data */
    t_u32 unicast_timeout;
    /** Inactivity timeout for multicast data */
    t_u32 mcast_timeout;
    /** Timeout for additional Rx traffic after Null PM1 packet exchange */
    t_u32 ps_entry_timeout;
} mlan_ds_inactivity_to, *pmlan_ds_inactivity_to;

/** Minimum sleep period in milliseconds */
#define MIN_SLEEP_PERIOD 10
/** Maximum sleep period in milliseconds */
#define MAX_SLEEP_PERIOD 60
/** Special setting for UPSD certification tests */
#define SLEEP_PERIOD_RESERVED_FF 0xFF

/** PS null interval disable */
#define PS_NULL_DISABLE (-1)

/** Local listen interval disable */
#define MRVDRV_LISTEN_INTERVAL_DISABLE (-1)
/** Minimum listen interval */
#define MRVDRV_MIN_LISTEN_INTERVAL 0

/** Minimum multiple DTIM */
#define MRVDRV_MIN_MULTIPLE_DTIM 0
/** Maximum multiple DTIM */
#define MRVDRV_MAX_MULTIPLE_DTIM 5
/** Ignore multiple DTIM */
#define MRVDRV_IGNORE_MULTIPLE_DTIM 0xfffe
/** Match listen interval to closest DTIM */
#define MRVDRV_MATCH_CLOSEST_DTIM 0xfffd

/** Minimum adhoc awake period */
#define MIN_ADHOC_AWAKE_PD 0
/** Maximum adhoc awake period */
#define MAX_ADHOC_AWAKE_PD 31
/** Special adhoc awake period */
#define SPECIAL_ADHOC_AWAKE_PD 255

/** Minimum beacon miss timeout in milliseconds */
#define MIN_BCN_MISS_TO 0
/** Maximum beacon miss timeout in milliseconds */
#define MAX_BCN_MISS_TO 50
/** Disable beacon miss timeout */
#define DISABLE_BCN_MISS_TO 65535

/** Minimum delay to PS in milliseconds */
#define MIN_DELAY_TO_PS 0
/** Maximum delay to PS in milliseconds */
#define MAX_DELAY_TO_PS 65535
/** Delay to PS unchanged */
#define DELAY_TO_PS_UNCHANGED (-1)
/** Default delay to PS in milliseconds */
#define DELAY_TO_PS_DEFAULT 1000
/** Delay for WNM retry request in milliseconds */
#define DELAY_TO_PS_WNM 500

/** PS mode: Unchanged */
#define PS_MODE_UNCHANGED 0
/** PS mode: Auto */
#define PS_MODE_AUTO 1
/** PS mode: Poll */
#define PS_MODE_POLL 2
/** PS mode: Null */
#define PS_MODE_NULL 3

/** Type definition of mlan_ds_ps_cfg for MLAN_OID_PM_CFG_PS_CFG */
typedef struct _mlan_ds_ps_cfg
{
    /** PS null interval in seconds */
    t_u32 ps_null_interval;
    /** Multiple DTIM interval */
    t_u32 multiple_dtim_interval;
    /** Listen interval */
    t_u32 listen_interval;
    /** Adhoc awake period */
    t_u32 adhoc_awake_period;
    /** Beacon miss timeout in milliseconds */
    t_u32 bcn_miss_timeout;
    /** Delay to PS in milliseconds */
    t_s32 delay_to_ps;
    /** PS mode */
    t_u32 ps_mode;
} mlan_ds_ps_cfg, *pmlan_ds_ps_cfg;

/** Type definition of mlan_ds_sleep_params for MLAN_OID_PM_CFG_SLEEP_PARAMS */
typedef struct _mlan_ds_sleep_params
{
    /** Error */
    t_u32 error;
    /** Offset in microseconds */
    t_u32 offset;
    /** Stable time in microseconds */
    t_u32 stable_time;
    /** Calibration control */
    t_u32 cal_control;
    /** External sleep clock */
    t_u32 ext_sleep_clk;
    /** Reserved */
    t_u32 reserved;
} mlan_ds_sleep_params, *pmlan_ds_sleep_params;

/** sleep_param */
typedef struct _ps_sleep_param
{
    /** control bitmap */
    t_u32 ctrl_bitmap;
    /** minimum sleep period (micro second) */
    t_u32 min_sleep;
    /** maximum sleep period (micro second) */
    t_u32 max_sleep;
} ps_sleep_param;

/** inactivity sleep_param */
typedef struct _inact_sleep_param
{
    /** inactivity timeout (micro second) */
    t_u32 inactivity_to;
    /** miniumu awake period (micro second) */
    t_u32 min_awake;
    /** maximum awake period (micro second) */
    t_u32 max_awake;
} inact_sleep_param;

/** flag for ps mode */
#define PS_FLAG_PS_MODE 1U
/** flag for sleep param */
#define PS_FLAG_SLEEP_PARAM 2U
/** flag for inactivity sleep param */
#define PS_FLAG_INACT_SLEEP_PARAM 4U

/** Disable power mode */
#define PS_MODE_DISABLE 0
/** Enable periodic dtim ps */
#define PS_MODE_PERIODIC_DTIM 1
/** Enable inactivity ps */
#define PS_MODE_INACTIVITY 2

/** mlan_ds_ps_mgmt */
typedef struct _mlan_ds_ps_mgmt
{
    /** flags for valid field */
    t_u16 flags;
    /** power mode */
    t_u16 ps_mode;
    /** sleep param */
    ps_sleep_param sleep_param;
    /** inactivity sleep param */
    inact_sleep_param inact_param;
} mlan_ds_ps_mgmt;

/** mlan_ds_ps_info */
typedef struct _mlan_ds_ps_info
{
    /** suspend allowed flag */
    t_u32 is_suspend_allowed;
} mlan_ds_ps_info;

/** Type definition of mlan_ds_wakeup_reason for MLAN_OID_PM_HS_WAKEUP_REASON */
typedef struct _mlan_ds_hs_wakeup_reason
{
    t_u16 hs_wakeup_reason;
} mlan_ds_hs_wakeup_reason;

/** Type definition of mlan_ds_pm_cfg for MLAN_IOCTL_PM_CFG */
typedef struct _mlan_ds_pm_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Power management parameter */
    union
    {
        /** Power saving mode for MLAN_OID_PM_CFG_IEEE_PS */
        t_u32 ps_mode;
#if CONFIG_HOST_SLEEP
        /** Host Sleep configuration for MLAN_OID_PM_CFG_HS_CFG */
        mlan_ds_hs_cfg hs_cfg;
#endif
        /** Deep sleep mode for MLAN_OID_PM_CFG_DEEP_SLEEP */
        mlan_ds_auto_ds auto_deep_sleep;
        /** Inactivity timeout for MLAN_OID_PM_CFG_INACTIVITY_TO */
        mlan_ds_inactivity_to inactivity_to;
        /** Sleep period for MLAN_OID_PM_CFG_SLEEP_PD */
        t_u32 sleep_period;
        /** PS configuration parameters for MLAN_OID_PM_CFG_PS_CFG */
        mlan_ds_ps_cfg ps_cfg;
        /** PS configuration parameters for MLAN_OID_PM_CFG_SLEEP_PARAMS */
        mlan_ds_sleep_params sleep_params;
        /** PS configuration parameters for MLAN_OID_PM_CFG_PS_MODE */
        mlan_ds_ps_mgmt ps_mgmt;
        /** power info for MLAN_OID_PM_INFO */
        mlan_ds_ps_info ps_info;
        /** hs wakeup reason for MLAN_OID_PM_HS_WAKEUP_REASON */
        mlan_ds_hs_wakeup_reason wakeup_reason;
    } param;
} mlan_ds_pm_cfg, *pmlan_ds_pm_cfg;

/*-----------------------------------------------------------------*/
/** WMM Configuration Group */
/*-----------------------------------------------------------------*/

/** WMM TSpec size */
#define MLAN_WMM_TSPEC_SIZE 63
/** WMM Add TS extra IE bytes */
#define MLAN_WMM_ADDTS_EXTRA_IE_BYTES 256
/** WMM statistics for packets hist bins */
#define MLAN_WMM_STATS_PKTS_HIST_BINS 7
/** Maximum number of AC QOS queues available */
#define MLAN_WMM_MAX_AC_QUEUES 4

/**
 *  @brief IOCTL structure to send an ADDTS request and retrieve the response.
 *
 *  IOCTL structure from the application layer relayed to firmware to
 *    instigate an ADDTS management frame with an appropriate TSPEC IE as well
 *    as any additional IEs appended in the ADDTS Action frame.
 *
 *  @sa woal_wmm_addts_req_ioctl
 */
typedef struct
{
    mlan_cmd_result_e cmd_result;                  /**< Firmware execution result */

    t_u32 timeout_ms;                              /**< Timeout value in milliseconds */
    t_u8 ieee_status_code;                         /**< IEEE status code */

    t_u32 ie_data_len;                             /**< Length of ie block in ie_data */
    t_u8 ie_data[MLAN_WMM_TSPEC_SIZE               /**< TSPEC to send in the ADDTS */
                 + MLAN_WMM_ADDTS_EXTRA_IE_BYTES]; /**< Extra IE buf*/
} wlan_ioctl_wmm_addts_req_t;

/**
 *  @brief IOCTL structure to send a DELTS request.
 *
 *  IOCTL structure from the application layer relayed to firmware to
 *    instigate an DELTS management frame with an appropriate TSPEC IE.
 *
 *  @sa woal_wmm_delts_req_ioctl
 */
typedef struct
{
    mlan_cmd_result_e cmd_result;      /**< Firmware execution result */
    t_u8 ieee_reason_code;             /**< IEEE reason code sent, unused for WMM */
    t_u32 ie_data_len;                 /**< Length of ie block in ie_data */
    t_u8 ie_data[MLAN_WMM_TSPEC_SIZE]; /**< TSPEC to send in the DELTS */
} wlan_ioctl_wmm_delts_req_t;

/**
 *  @brief IOCTL structure to configure a specific AC Queue's parameters
 *
 *  IOCTL structure from the application layer relayed to firmware to
 *    get, set, or default the WMM AC queue parameters.
 *
 *  - msdu_lifetime_expiry is ignored if set to 0 on a set command
 *
 *  @sa woal_wmm_queue_config_ioctl
 */
typedef struct
{
    mlan_wmm_queue_config_action_e action; /**< Set, Get, or Default */
    mlan_wmm_ac_e access_category;         /**< WMM_AC_BK(0) to WMM_AC_VO(3) */
    t_u16 msdu_lifetime_expiry;            /**< lifetime expiry in TUs */
    t_u8 supported_rates[10];              /**< Not supported yet */
} wlan_ioctl_wmm_queue_config_t;

/**
 *  @brief IOCTL structure to start, stop, and get statistics for a WMM AC
 *
 *  IOCTL structure from the application layer relayed to firmware to
 *    start or stop statistical collection for a given AC.  Also used to
 *    retrieve and clear the collected stats on a given AC.
 *
 *  @sa woal_wmm_queue_stats_ioctl
 */
typedef struct
{
    /** Action of Queue Config : Start, Stop, or Get */
    mlan_wmm_queue_stats_action_e action;
    /** User Priority */
    t_u8 user_priority;
    /** Number of successful packets transmitted */
    t_u16 pkt_count;
    /** Packets lost; not included in pkt_count */
    t_u16 pkt_loss;
    /** Average Queue delay in microseconds */
    t_u32 avg_queue_delay;
    /** Average Transmission delay in microseconds */
    t_u32 avg_tx_delay;
    /** Calculated used time in units of 32 microseconds */
    t_u16 used_time;
    /** Calculated policed time in units of 32 microseconds */
    t_u16 policed_time;
    /** Queue Delay Histogram; number of packets per queue delay range
     *
     *  [0] -  0ms <= delay < 5ms
     *  [1] -  5ms <= delay < 10ms
     *  [2] - 10ms <= delay < 20ms
     *  [3] - 20ms <= delay < 30ms
     *  [4] - 30ms <= delay < 40ms
     *  [5] - 40ms <= delay < 50ms
     *  [6] - 50ms <= delay < msduLifetime (TUs)
     */
    t_u16 delay_histogram[MLAN_WMM_STATS_PKTS_HIST_BINS];
} wlan_ioctl_wmm_queue_stats_t,
    /** Type definition of mlan_ds_wmm_queue_stats for MLAN_OID_WMM_CFG_QUEUE_STATS */
    mlan_ds_wmm_queue_stats, *pmlan_ds_wmm_queue_stats;

/**
 *  @brief IOCTL sub structure for a specific WMM AC Status
 */
typedef struct
{
    /** WMM Acm */
    t_u8 wmm_acm;
    /** Flow required flag */
    t_u8 flow_required;
    /** Flow created flag */
    t_u8 flow_created;
    /** Disabled flag */
    t_u8 disabled;
} wlan_ioctl_wmm_queue_status_ac_t;

/**
 *  @brief IOCTL structure to retrieve the WMM AC Queue status
 *
 *  IOCTL structure from the application layer to retrieve:
 *     - ACM bit setting for the AC
 *     - Firmware status (flow required, flow created, flow disabled)
 *
 *  @sa woal_wmm_queue_status_ioctl
 */
typedef struct
{
    /** WMM AC queue status */
    wlan_ioctl_wmm_queue_status_ac_t ac_status[MLAN_WMM_MAX_AC_QUEUES];
} wlan_ioctl_wmm_queue_status_t,
    /** Type definition of mlan_ds_wmm_queue_status for MLAN_OID_WMM_CFG_QUEUE_STATUS */
    mlan_ds_wmm_queue_status, *pmlan_ds_wmm_queue_status;

/** Type definition of mlan_ds_wmm_addts for MLAN_OID_WMM_CFG_ADDTS */
typedef struct _mlan_ds_wmm_addts
{
    /** Result of ADDTS request */
    mlan_cmd_result_e result;
    /** Timeout value in milliseconds */
    t_u32 timeout;
    /** IEEE status code */
    t_u32 status_code;
    /** Dialog token */
    t_u8 dialog_tok;
    /** TSPEC data length */
    t_u8 ie_data_len;
    /** TSPEC to send in the ADDTS + buffering for any extra IEs */
    t_u8 ie_data[MLAN_WMM_TSPEC_SIZE + MLAN_WMM_ADDTS_EXTRA_IE_BYTES];
} mlan_ds_wmm_addts, *pmlan_ds_wmm_addts;

/** Type definition of mlan_ds_wmm_delts for MLAN_OID_WMM_CFG_DELTS */
typedef struct _mlan_ds_wmm_delts
{
    /** Result of DELTS request */
    mlan_cmd_result_e result;
    /** IEEE status code */
    t_u32 status_code;
    /** TSPEC data length */
    t_u8 ie_data_len;
    /** TSPEC to send in the DELTS */
    t_u8 ie_data[MLAN_WMM_TSPEC_SIZE];
} mlan_ds_wmm_delts, *pmlan_ds_wmm_delts;

/** Type definition of mlan_ds_wmm_queue_config for MLAN_OID_WMM_CFG_QUEUE_CONFIG */
typedef struct _mlan_ds_wmm_queue_config
{
    /** Action of Queue Config : Set, Get, or Default */
    mlan_wmm_queue_config_action_e action;
    /** WMM Access Category: WMM_AC_BK(0) to WMM_AC_VO(3) */
    mlan_wmm_ac_e access_category;
    /** Lifetime expiry in TUs */
    t_u16 msdu_lifetime_expiry;
    /** Reserve for future use */
    t_u8 reserved[10];
} mlan_ds_wmm_queue_config, *pmlan_ds_wmm_queue_config;

/** Type definition of mlan_ds_wmm_cfg for MLAN_IOCTL_WMM_CFG */
typedef struct _mlan_ds_wmm_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** WMM configuration parameter */
    union
    {
        /** WMM enable for MLAN_OID_WMM_CFG_ENABLE */
        t_u32 wmm_enable;
        /** QoS configuration for MLAN_OID_WMM_CFG_QOS */
        t_u8 qos_cfg;
        /** WMM add TS for MLAN_OID_WMM_CFG_ADDTS */
        mlan_ds_wmm_addts addts;
        /** WMM delete TS for MLAN_OID_WMM_CFG_DELTS */
        mlan_ds_wmm_delts delts;
        /** WMM queue configuration for MLAN_OID_WMM_CFG_QUEUE_CONFIG */
        mlan_ds_wmm_queue_config q_cfg;
        /** AC Parameters Record WMM_AC_BE, WMM_AC_BK, WMM_AC_VI,
         * WMM_AC_VO */
        wmm_ac_parameters_t ac_params[MAX_AC_QUEUES];
        /** WMM queue status for MLAN_OID_WMM_CFG_QUEUE_STATS */
        mlan_ds_wmm_queue_stats q_stats;
        /** WMM queue status for MLAN_OID_WMM_CFG_QUEUE_STATUS */
        mlan_ds_wmm_queue_status q_status;
        /** WMM TS status for MLAN_OID_WMM_CFG_TS_STATUS */
        mlan_ds_wmm_ts_status ts_status;
    } param;
} mlan_ds_wmm_cfg, *pmlan_ds_wmm_cfg;

/*-----------------------------------------------------------------*/
/** WPS Configuration Group */
/*-----------------------------------------------------------------*/
/** Enumeration for WPS session */
enum _mlan_wps_status
{
    MLAN_WPS_CFG_SESSION_START = 1,
    MLAN_WPS_CFG_SESSION_END   = 0
};

/** Type definition of mlan_ds_wps_cfg for MLAN_IOCTL_WPS_CFG */
typedef struct _mlan_ds_wps_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** WPS configuration parameter */
    union
    {
        /** WPS session for MLAN_OID_WPS_CFG_SESSION */
        t_u32 wps_session;
    } param;
} mlan_ds_wps_cfg, *pmlan_ds_wps_cfg;

/*-----------------------------------------------------------------*/
/** 802.11n Configuration Group */
/*-----------------------------------------------------------------*/
/** Maximum MCS */
#define NUM_MCS_FIELD 16

/* Both 2.4G and 5G band selected */
#define BAND_SELECT_BOTH 0U
/* Band 2.4G selected */
#define BAND_SELECT_BG 1U
/* Band 5G selected */
#define BAND_SELECT_A 2U

/** Type definition of mlan_ds_11n_htcap_cfg for MLAN_OID_11N_HTCAP_CFG */
typedef struct _mlan_ds_11n_htcap_cfg
{
    /** HT Capability information */
    t_u32 htcap;
    /** Band selection */
    t_u32 misc_cfg;
    /** Hardware HT cap information required */
    t_u32 hw_cap_req;
} mlan_ds_11n_htcap_cfg, *pmlan_ds_11n_htcap_cfg;

/** Type definition of mlan_ds_11n_addba_param for MLAN_OID_11N_CFG_ADDBA_PARAM */
typedef struct _mlan_ds_11n_addba_param
{
    /** Timeout */
    t_u32 timeout;
    /** Buffer size for ADDBA request */
    t_u32 txwinsize;
    /** Buffer size for ADDBA response */
    t_u32 rxwinsize;
    /** amsdu for ADDBA request */
    t_u8 txamsdu;
    /** amsdu for ADDBA response */
    t_u8 rxamsdu;
} mlan_ds_11n_addba_param, *pmlan_ds_11n_addba_param;

/** Type definition of mlan_ds_11n_tx_cfg for MLAN_OID_11N_CFG_TX */
typedef struct _mlan_ds_11n_tx_cfg
{
    /** HTTxCap */
    t_u16 httxcap;
    /** HTTxInfo */
    t_u16 httxinfo;
    /** Band selection */
    t_u32 misc_cfg;
} mlan_ds_11n_tx_cfg, *pmlan_ds_11n_tx_cfg;

/** Tx */
#define MLAN_RADIO_TX MBIT(0)
/** Rx */
#define MLAN_RADIO_RX MBIT(1)
/** Tx & Rx */
#define MLAN_RADIO_TXRX (MLAN_RADIO_TX | MLAN_RADIO_RX)

/** Type definition of mlan_ds_11ac_tx_cfg for MLAN_OID_11AC_CFG */
typedef struct _mlan_ds_11ac_vht_cfg
{
    /** Band selection (1: 2.4G, 2: 5 G, 3: both 2.4G and 5G) */
    t_u32 band;
    /** TxRx (1: Tx, 2: Rx, 3: both Tx and Rx) */
    t_u32 txrx;
    /** BW CFG (0: 11N CFG, 1: vhtcap) */
    t_u32 bwcfg;
    /** VHT capabilities. */
    t_u32 vht_cap_info;
    /** VHT Tx mcs */
    t_u32 vht_tx_mcs;
    /** VHT Rx mcs */
    t_u32 vht_rx_mcs;
    /** VHT rx max rate */
    t_u16 vht_rx_max_rate;
    /** VHT max tx rate */
    t_u16 vht_tx_max_rate;
    /** Skip usr 11ac mcs cfg */
    t_bool skip_usr_11ac_mcs_cfg;
} mlan_ds_11ac_vht_cfg, *pmlan_ds_11ac_vht_cfg;

#if CONFIG_11AX
#define MAX_RU_COUNT    6
#define MAX_RUTXPWR_NUM 140
typedef MLAN_PACK_START struct _mlan_rupwrlimit_config_t
{
    /** start freq */
    t_u16 start_freq;
    /* channel width */
    t_u8 width;
    /** channel number */
    t_u8 chan_num;
    /** chan ru Power */
    t_s8 ruPower[MAX_RU_COUNT];
} MLAN_PACK_END mlan_rupwrlimit_config_t;

typedef MLAN_PACK_START struct _mlan_11axcmdcfg_obss_pd_offset
{
    /** <NON_SRG_OffSET, SRG_OFFSET> */
    t_u8 offset[2];
} MLAN_PACK_END mlan_11axcmdcfg_obss_pd_offset;

/** Type definition of mlan_11axcmdcfg_sr_control for MLAN_OID_11AX_CMD_CFG */
typedef MLAN_PACK_START struct _mlan_11axcmdcfg_sr_control
{
    /** 1 enable, 0 disable */
    t_u8 control;
} MLAN_PACK_END mlan_11axcmdcfg_sr_control;

/** Type definition of mlan_ds_11ax_he_capa for MLAN_OID_11AX_HE_CFG */
typedef MLAN_PACK_START struct _mlan_ds_11ax_he_capa
{
    /** tlv id of he capability */
    t_u16 id;
    /** length of the payload */
    t_u16 len;
    /** extension id */
    t_u8 ext_id;
    /** he mac capability info */
    t_u8 he_mac_cap[6];
    /** he phy capability info */
    t_u8 he_phy_cap[11];
    /** he txrx mcs support for 80MHz */
    t_u8 he_txrx_mcs_support[4];
    /** val for txrx mcs 160Mhz or 80+80, and PPE thresholds */
    t_u8 val[28];
} MLAN_PACK_END mlan_ds_11ax_he_capa, *pmlan_ds_11ax_he_capa;

/** Type definition of mlan_ds_11ax_he_cfg for MLAN_OID_11AX_HE_CFG */
typedef struct _mlan_ds_11ax_he_cfg
{
    /** band, BIT0:2.4G, BIT1:5G*/
    t_u8 band;
    /** mlan_ds_11ax_he_capa */
    mlan_ds_11ax_he_capa he_cap;
} mlan_ds_11ax_he_cfg, *pmlan_ds_11ax_he_cfg;

/** Type definition of mlan_ds_11ax_sr_cmd for MLAN_OID_11AX_CMD_CFG */
typedef MLAN_PACK_START struct _mlan_ds_11ax_sr_cmd
{
    /** type*/
    t_u16 type;
    /** length of TLV */
    t_u16 len;
    /** value */
    union
    {
        mlan_11axcmdcfg_obss_pd_offset obss_pd_offset;
        mlan_11axcmdcfg_sr_control sr_control;
    } param;
} MLAN_PACK_END mlan_ds_11ax_sr_cmd, *pmlan_ds_11ax_sr_cmd;

/** Type definition of mlan_ds_11ax_beam_cmd for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_beam_cmd
{
    /** command value: 1 is disable, 0 is enable*/
    t_u8 value;
} mlan_ds_11ax_beam_cmd, *pmlan_ds_11ax_beam_cmd;

/** Type definition of mlan_ds_11ax_htc_cmd for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_htc_cmd
{
    /** command value: 1 is enable, 0 is disable*/
    t_u8 value;
} mlan_ds_11ax_htc_cmd, *pmlan_ds_11ax_htc_cmd;

/** Type definition of mlan_ds_11ax_htc_cmd for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_txop_cmd
{
    /** Two byte rts threshold value of which only 10 bits, bit 0 to bit 9
     * are valid */
    t_u16 rts_thres;
} mlan_ds_11ax_txop_cmd, *pmlan_ds_11ax_txop_cmd;

/** Type definition of mlan_ds_11ax_htc_cmd for MLAN_OID_11AX_CMD_CFG */
typedef MLAN_PACK_START struct _mlan_ds_11ax_txomi_cmd
{
    /* 11ax spec 9.2.4.6a.2 OM Control 12 bits. Bit 0 to bit 11 */
    t_u16 omi;
    /* tx option
     * 0: send OMI in QoS NULL; 1: send OMI in QoS data;
     * 0xFF: OMI is transmitted in both QoS NULL and QoS data frame.
     */
    t_u8 tx_option;
    /* num_data_pkts is applied only if OMI is sent in QoS data frame.
     * It specifies the number of consecutive data frames containing the OMI.
     * Minimum number of data packets should be 1 and maximum should be 16.
     */
    t_u8 num_data_pkts;
} MLAN_PACK_END mlan_ds_11ax_txomi_cmd, *pmlan_ds_11ax_txomi_cmd;

/** Type definition of mlan_ds_11ax_toltime_cmd for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_toltime_cmd
{
    /* OBSS Narrow Bandwidth RU Tolerance Time */
    t_u32 tol_time;
} mlan_ds_11ax_toltime_cmd, *pmlan_ds_11ax_toltime_cmd;

/** Type definition of mlan_ds_11ax_chanlrupwrcft_cmd for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_chanlrupwrcft_cmd
{
    /** type*/
    t_u16 type;
    /** length of TLV */
    t_u16 len;
    /* Channel RU TX power limit Config */
    mlan_rupwrlimit_config_t rupwrlimit_config;
} mlan_ds_11ax_chanlrupwrcft_cmd, *pmlan_ds_11ax_chanlrupwrcft_cmd;

/** Type definition of mlan_ds_11ax_cmd_cfg for MLAN_OID_11AX_CMD_CFG */
typedef struct _mlan_ds_11ax_cmd_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Sub-id */
    t_u32 sub_id;
    /** 802.11n configuration parameter */
    union
    {
        /** SR configuration for MLAN_11AXCMD_SR_SUBID */
        mlan_ds_11ax_sr_cmd sr_cfg;
        /** Beam configuration for MLAN_11AXCMD_BEAM_SUBID */
        mlan_ds_11ax_beam_cmd beam_cfg;
        /** HTC configuration for MLAN_11AXCMD_HTC_SUBID */
        mlan_ds_11ax_htc_cmd htc_cfg;
        /** txop RTS configuration for MLAN_11AXCMD_TXOPRTS_SUBID */
        mlan_ds_11ax_txop_cmd txop_cfg;
        /** tx omi configuration for MLAN_11AXCMD_TXOMI_SUBID */
        mlan_ds_11ax_txomi_cmd txomi_cfg;
        /** OBSS tolerance time configuration for
         * MLAN_11AXCMD_TOLTIME_SUBID */
        mlan_ds_11ax_toltime_cmd toltime_cfg;
        /** Channel RU TX power limit Config for
         * MLAN_11AXCMD_RUPOWER_SUBID */
        mlan_ds_11ax_chanlrupwrcft_cmd rupwr_cfg;
    } param;
} mlan_ds_11ax_cmd_cfg, *pmlan_ds_11ax_cmd_cfg;

#if CONFIG_11AX_TWT
/** Type definition of mlan_ds_twt_setup for MLAN_OID_11AX_TWT_CFG */
typedef MLAN_PACK_START struct _mlan_ds_twt_setup
{
    /** Implicit, 0: TWT session is explicit, 1: Session is implicit */
    t_u8 implicit;
    /** Announced, 0: Unannounced, 1: Announced TWT */
    t_u8 announced;
    /** Trigger Enabled, 0: Non-Trigger enabled, 1: Trigger enabled TWT */
    t_u8 trigger_enabled;
    /** TWT Information Disabled, 0: TWT info enabled, 1: TWT info disabled */
    t_u8 twt_info_disabled;
    /** Negotiation Type, 0: Future Individual TWT SP start time, 1: Next
     * Wake TBTT time */
    t_u8 negotiation_type;
    /** TWT Wakeup Duration, time after which the TWT requesting STA can
     * transition to doze state */
    t_u8 twt_wakeup_duration;
    /** Flow Identifier. Range: [0-7]*/
    t_u8 flow_identifier;
    /** Hard Constraint, 0: FW can tweak the TWT setup parameters if it is
     *rejected by AP.
     ** 1: Firmware should not tweak any parameters. */
    t_u8 hard_constraint;
    /** TWT Exponent, Range: [0-63] */
    t_u8 twt_exponent;
    /** TWT Mantissa Range: [0-sizeof(UINT16)] */
    t_u16 twt_mantissa;
    /** TWT Request Type, 0: REQUEST_TWT, 1: SUGGEST_TWT*/
    t_u8 twt_request;
    /** TWT Setup State. Set to 0 by driver, filled by FW in response*/
    t_u8 twt_setup_state;
    /** TWT link lost timeout threshold */
    t_u16 bcnMiss_threshold;
} MLAN_PACK_END mlan_ds_twt_setup, *pmlan_ds_twt_setup;

/** Type definition of mlan_ds_twt_teardown for MLAN_OID_11AX_TWT_CFG */
typedef MLAN_PACK_START struct _mlan_ds_twt_teardown
{
    /** TWT Flow Identifier. Range: [0-7] */
    t_u8 flow_identifier;
    /** Negotiation Type. 0: Future Individual TWT SP start time, 1: Next
     * Wake TBTT time */
    t_u8 negotiation_type;
    /** Tear down all TWT. 1: To teardown all TWT, 0 otherwise */
    t_u8 teardown_all_twt;
} MLAN_PACK_END mlan_ds_twt_teardown, *pmlan_ds_twt_teardown;

/** Type definition of mlan_ds_twt_report for MLAN_OID_11AX_TWT_CFG */
typedef MLAN_PACK_START struct _mlan_ds_twt_report
{
    /** TWT report type, 0: BTWT id */
    t_u8 type;
    /** TWT report length of value in data */
    t_u8 length;
    t_u8 reserve[2];
    /** TWT report payload for FW response to fill, 4 * 9bytes */
    t_u8 data[54]; //WLAN_BTWT_REPORT_LEN* WLAN_BTWT_REPORT_MAX_NUM
} MLAN_PACK_END mlan_ds_twt_report, *pmlan_ds_twt_report;

/** Type definition of mlan_ds_twtcfg for MLAN_OID_11AX_TWT_CFG */
typedef MLAN_PACK_START struct _mlan_ds_twtcfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Sub-id */
    t_u32 sub_id;
    /** TWT Setup/Teardown configuration parameter */
    union
    {
        /** TWT Setup config for Sub ID: MLAN_11AX_TWT_SETUP_SUBID */
        mlan_ds_twt_setup twt_setup;
        /** TWT Teardown config for Sub ID: MLAN_11AX_TWT_TEARDOWN_SUBID */
        mlan_ds_twt_teardown twt_teardown;
        /** TWT report for Sub ID: MLAN_11AX_TWT_REPORT_SUBID */
        mlan_ds_twt_report twt_report;
    } param;
} MLAN_PACK_END mlan_ds_twtcfg, *pmlan_ds_twtcfg;
#endif /* CONFIG_11AX_TWT */

/** Type definition of mlan_ds_11as_cfg for MLAN_IOCTL_11AX_CFG */
typedef struct _mlan_ds_11ax_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** 802.11n configuration parameter */
    union
    {
        /** HE configuration for MLAN_OID_11AX_HE_CFG */
        mlan_ds_11ax_he_cfg he_cfg;
    } param;
} mlan_ds_11ax_cfg, *pmlan_ds_11ax_cfg;
#endif

#if CONFIG_WIFI_CLOCKSYNC
/** Type definition of mlan_ds_gpio_tsf_latch */
typedef MLAN_PACK_START struct _mlan_ds_gpio_tsf_latch
{
    /**clock sync Mode */
    t_u8 clock_sync_mode;
    /**clock sync Role */
    t_u8 clock_sync_Role;
    /**clock sync GPIO Pin Number */
    t_u8 clock_sync_gpio_pin_number;
    /**clock sync GPIO Level or Toggle */
    t_u8 clock_sync_gpio_level_toggle;
    /**clock sync GPIO Pulse Width */
    t_u16 clock_sync_gpio_pulse_width;
} MLAN_PACK_END mlan_ds_gpio_tsf_latch, *pmlan_ds_gpio_tsf_latch;

/** Type definition of mlan_ds_tsf_info */
typedef MLAN_PACK_START struct _mlan_ds_tsf_info
{
    /**get tsf info format */
    t_u16 tsf_format;
    /**tsf info */
    t_u16 tsf_info;
    /**tsf */
    t_u64 tsf;
    /**Positive or negative offset in microsecond from Beacon TSF to GPIO toggle TSF  */
    t_s32 tsf_offset;
} MLAN_PACK_END mlan_ds_tsf_info, *pmlan_ds_tsf_info;
#endif /* CONFIG_WIFI_CLOCKSYNC */

/** Type definition of mlan_ds_11n_amsdu_aggr_ctrl for
 * MLAN_OID_11N_AMSDU_AGGR_CTRL*/
typedef struct _mlan_ds_11n_amsdu_aggr_ctrl
{
    /** Enable/Disable */
    t_u16 enable;
    /** Current AMSDU size valid */
    t_u16 curr_buf_size;
} mlan_ds_11n_amsdu_aggr_ctrl, *pmlan_ds_11n_amsdu_aggr_ctrl;

/** Type definition of mlan_ds_11n_aggr_prio_tbl for MLAN_OID_11N_CFG_AGGR_PRIO_TBL */
typedef struct _mlan_ds_11n_aggr_prio_tbl
{
    /** ampdu priority table */
    t_u8 ampdu[MAX_NUM_TID];
    /** amsdu priority table */
    t_u8 amsdu[MAX_NUM_TID];
} mlan_ds_11n_aggr_prio_tbl, *pmlan_ds_11n_aggr_prio_tbl;

/** DelBA All TIDs */
#define DELBA_ALL_TIDS 0xff
/** DelBA Tx */
#define DELBA_TX MBIT(0)
/** DelBA Rx */
#define DELBA_RX MBIT(1)

/** Type definition of mlan_ds_11n_delba for MLAN_OID_11N_CFG_DELBA */
typedef struct _mlan_ds_11n_delba
{
    /** TID */
    t_u8 tid;
    /** Peer MAC address */
    t_u8 peer_mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Direction (Tx: bit 0, Rx: bit 1) */
    t_u8 direction;
} mlan_ds_11n_delba, *pmlan_ds_11n_delba;

/** Type definition of mlan_ds_11n_cfg for MLAN_IOCTL_11N_CFG */
typedef struct _mlan_ds_11n_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** 802.11n configuration parameter */
    union
    {
        /** Tx param for 11n for MLAN_OID_11N_CFG_TX */
        mlan_ds_11n_tx_cfg tx_cfg;
        /** Aggr priority table for MLAN_OID_11N_CFG_AGGR_PRIO_TBL */
        mlan_ds_11n_aggr_prio_tbl aggr_prio_tbl;
        /** Add BA param for MLAN_OID_11N_CFG_ADDBA_PARAM */
        mlan_ds_11n_addba_param addba_param;
        /** Add BA Reject paramters for MLAN_OID_11N_CFG_ADDBA_REJECT */
        t_u8 addba_reject[MAX_NUM_TID];
        /** Tx buf size for MLAN_OID_11N_CFG_MAX_TX_BUF_SIZE */
        t_u32 tx_buf_size;
        /** HT cap info configuration for MLAN_OID_11N_HTCAP_CFG */
        mlan_ds_11n_htcap_cfg htcap_cfg;
        /** Tx param for 11n for MLAN_OID_11N_AMSDU_AGGR_CTRL */
        mlan_ds_11n_amsdu_aggr_ctrl amsdu_aggr_ctrl;
        /** Supported MCS Set field */
        t_u8 supported_mcs_set[NUM_MCS_FIELD];
        /** Transmit Beamforming Capabilities field */
        t_u32 tx_bf_cap;
        /** DelBA for MLAN_OID_11N_CFG_DELBA */
        mlan_ds_11n_delba del_ba;
    } param;
} mlan_ds_11n_cfg, *pmlan_ds_11n_cfg;

/** Country code length */
#define COUNTRY_CODE_LEN 3U

/*-----------------------------------------------------------------*/
/** 802.11d Configuration Group */
/*-----------------------------------------------------------------*/
/** Maximum subbands for 11d */
#if CONFIG_5GHz_SUPPORT
#define MRVDRV_MAX_SUBBAND_802_11D 83U
#else
#define MRVDRV_MAX_SUBBAND_802_11D 14
#endif /* CONFIG_5GHz_SUPPORT */

#ifdef STA_SUPPORT
/** Data structure for subband set */
typedef struct _mlan_ds_subband_set_t
{
    /** First channel */
    t_u8 first_chan;
    /** Number of channels */
    t_u8 no_of_chan;
    /** Maximum Tx power in dBm */
    t_u8 max_tx_pwr;
} mlan_ds_subband_set_t;

/** Domain regulatory information */
typedef struct _mlan_ds_11d_domain_info
{
    /** Country Code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Band that channels in sub_band belong to */
    t_u16 band;
    /** No. of subband in below */
    t_u8 no_of_sub_band;
    /** Subband data to send/last sent */
    mlan_ds_subband_set_t sub_band[MRVDRV_MAX_SUBBAND_802_11D];
} mlan_ds_11d_domain_info;
#endif

/** Type definition of mlan_ds_11d_cfg for MLAN_IOCTL_11D_CFG */
typedef struct _mlan_ds_11d_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** 802.11d configuration parameter */
    union
    {
#ifdef STA_SUPPORT
        /** Enable for MLAN_OID_11D_CFG_ENABLE */
        t_u32 enable_11d;
        /** Domain info for MLAN_OID_11D_DOMAIN_INFO */
        mlan_ds_11d_domain_info domain_info;
#endif /* STA_SUPPORT */
#if UAP_SUPPORT
        /** tlv data for MLAN_OID_11D_DOMAIN_INFO */
        t_u8 domain_tlv[MAX_IE_SIZE];
#endif /* UAP_SUPPORT */
    } param;
} mlan_ds_11d_cfg, *pmlan_ds_11d_cfg;

/*-----------------------------------------------------------------*/
/** Register Memory Access Group */
/*-----------------------------------------------------------------*/
/** Enumeration for register type */
enum _mlan_reg_type
{
    MLAN_REG_MAC = 1,
    MLAN_REG_BBP,
    MLAN_REG_RF,
    MLAN_REG_CAU = 5,
};

/** Type definition of mlan_ds_reg_rw for MLAN_OID_REG_RW */
typedef struct _mlan_ds_reg_rw
{
    /** Register type */
    t_u32 type;
    /** Offset */
    t_u32 offset;
    /** Value */
    t_u32 value;
} mlan_ds_reg_rw;

/** Maximum EEPROM data */
/* wmsdk: The value of MAX_EEPROM_DATA has been changed from 256 to 1. This is
 * done as this structure is used to set only the offset and byte_count. The
 * command (iwpriv mlan rdeeprom <offset> <length>) being GET command, its value
 * parameter is always sent as 0 */
#define MAX_EEPROM_DATA 1

/** Type definition of mlan_ds_read_eeprom for MLAN_OID_EEPROM_RD */
typedef struct _mlan_ds_read_eeprom
{
    /** Multiples of 4 */
    t_u16 offset;
    /** Number of bytes */
    t_u16 byte_count;
    /** Value */
    t_u8 value[MAX_EEPROM_DATA];
} mlan_ds_read_eeprom;

/** Type definition of mlan_ds_mem_rw for MLAN_OID_MEM_RW */
typedef struct _mlan_ds_mem_rw
{
    /** Address */
    t_u32 addr;
    /** Value */
    t_u32 value;
} mlan_ds_mem_rw;

/** Type definition of mlan_ds_reg_mem for MLAN_IOCTL_REG_MEM */
typedef struct _mlan_ds_reg_mem
{
    /** Sub-command */
    t_u32 sub_command;
    /** Register memory access parameter */
    union
    {
        /** Register access for MLAN_OID_REG_RW */
        mlan_ds_reg_rw reg_rw;
        /** EEPROM access for MLAN_OID_EEPROM_RD */
        mlan_ds_read_eeprom rd_eeprom;
        /** Memory access for MLAN_OID_MEM_RW */
        mlan_ds_mem_rw mem_rw;
    } param;
} mlan_ds_reg_mem, *pmlan_ds_reg_mem;

/** Type definition of mlan_ds_auto_reconnect */
typedef struct _mlan_ds_auto_reconnect
{
    /** Reconnect Counter */
    t_u8 reconnect_counter;
    /** Reconnect Interval */
    t_u8 reconnect_interval;
    /** Flags */
    t_u16 flags;
} mlan_ds_auto_reconnect;

/** Type definition of mlan_ds_rx_mgmt_indication */
typedef struct _mlan_ds_rx_mgmt_indication
{
    /** Mgmt subtype mask for MLAN_OID_MISC_RX_MGMT_IND */
    t_u32 mgmt_subtype_mask;
} mlan_ds_rx_mgmt_indication;

/** Type definition of mlan_bridge_mode */
typedef struct _mlan_bridge_mode
{
    /** Value */
    t_u8 enable;
} mlan_bridge_mode;

/*-----------------------------------------------------------------*/
/** Multi-Radio Configuration Group */
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/** 802.11h Configuration Group */
/*-----------------------------------------------------------------*/

/** Type definition of mlan_ds_11h_cfg for MLAN_IOCTL_11H_CFG */
typedef struct _mlan_ds_11h_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    union
    {
        /** Local power constraint for MLAN_OID_11H_LOCAL_POWER_CONSTRAINT */
        t_s8 usr_local_power_constraint;
    } param;
} mlan_ds_11h_cfg, *pmlan_ds_11h_cfg;

/*-----------------------------------------------------------------*/
/** Miscellaneous Configuration Group */
/*-----------------------------------------------------------------*/

/** CMD buffer size */
#define MLAN_SIZE_OF_CMD_BUFFER 2048

/** LDO Internal */
#define LDO_INTERNAL 0
/** LDO External */
#define LDO_EXTERNAL 1

/** Enumeration for IE type */
enum _mlan_ie_type
{
    MLAN_IE_TYPE_GEN_IE = 0,
#ifdef STA_SUPPORT
    MLAN_IE_TYPE_ARP_FILTER,
#endif /* STA_SUPPORT */
};

/** Type definition of mlan_ds_misc_gen_ie for MLAN_OID_MISC_GEN_IE */
typedef struct _mlan_ds_misc_gen_ie
{
    /** IE type */
    t_u32 type;
    /** IE length */
    t_u32 len;
    /** IE buffer */
    t_u8 ie_data[MAX_IE_SIZE];
} mlan_ds_misc_gen_ie;

#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
/** Type definition of mlan_ds_misc_sdio_mpa_ctrl for MLAN_OID_MISC_SDIO_MPA_CTRL */
typedef struct _mlan_ds_misc_sdio_mpa_ctrl
{
    /** SDIO MP-A TX enable/disable */
    t_u16 tx_enable;
    /** SDIO MP-A RX enable/disable */
    t_u16 rx_enable;
    /** SDIO MP-A TX buf size */
    t_u16 tx_buf_size;
    /** SDIO MP-A RX buf size */
    t_u16 rx_buf_size;
    /** SDIO MP-A TX Max Ports */
    t_u16 tx_max_ports;
    /** SDIO MP-A RX Max Ports */
    t_u16 rx_max_ports;
} mlan_ds_misc_sdio_mpa_ctrl;
#endif

/** Type definition of mlan_ds_misc_cmd for MLAN_OID_MISC_HOST_CMD */
typedef struct _mlan_ds_misc_cmd
{
    /** Command length */
    t_u32 len;
    /** Command buffer */
    t_u8 cmd[MLAN_SIZE_OF_CMD_BUFFER];
} mlan_ds_misc_cmd;

/** Maximum number of system clocks */
#define MLAN_MAX_CLK_NUM 16

/** Clock type : Configurable */
#define MLAN_CLK_CONFIGURABLE 0
/** Clock type : Supported */
#define MLAN_CLK_SUPPORTED 1

/** Type definition of mlan_ds_misc_sys_clock for MLAN_OID_MISC_SYS_CLOCK */
typedef struct _mlan_ds_misc_sys_clock
{
    /** Current system clock */
    t_u16 cur_sys_clk;
    /** Clock type */
    t_u16 sys_clk_type;
    /** Number of clocks */
    t_u16 sys_clk_num;
    /** System clocks */
    t_u16 sys_clk[MLAN_MAX_CLK_NUM];
} mlan_ds_misc_sys_clock;

/** Enumeration for function init/shutdown */
enum _mlan_func_cmd
{
    MLAN_FUNC_INIT = 1,
    MLAN_FUNC_SHUTDOWN,
};

/** Type definition of mlan_ds_misc_tx_datapause for MLAN_OID_MISC_TX_DATAPAUSE */
typedef struct _mlan_ds_misc_tx_datapause
{
    /** Tx data pause flag */
    t_u16 tx_pause;
    /** Max number of Tx buffers for all PS clients */
    t_u16 tx_buf_cnt;
} mlan_ds_misc_tx_datapause;

/** IP address length */
#define IPADDR_LEN (16)
/** Max number of ip */
#define MAX_IPADDR (4)
/** IP address type - IPv4*/
#define IPADDR_TYPE_IPV4 (1)
/** IP operation remove */
#define MLAN_IPADDR_OP_IP_REMOVE (0)
/** IP operation ARP filter */
#define MLAN_IPADDR_OP_ARP_FILTER MBIT(0)
/** IP operation ARP response */
#define MLAN_IPADDR_OP_AUTO_ARP_RESP MBIT(1)

/** Type definition of mlan_ds_misc_ipaddr_cfg for MLAN_OID_MISC_IP_ADDR */
typedef struct _mlan_ds_misc_ipaddr_cfg
{
    /** Operation code */
    t_u32 op_code;
    /** IP address type */
    t_u32 ip_addr_type;
    /** Number of IP */
    t_u32 ip_addr_num;
    /** IP address */
    t_u8 ip_addr[MAX_IPADDR][IPADDR_LEN];
} mlan_ds_misc_ipaddr_cfg;

/* MEF configuration disable */
#define MEF_CFG_DISABLE 0
/* MEF configuration Rx filter enable */
#define MEF_CFG_RX_FILTER_ENABLE 1
/* MEF configuration auto ARP response */
#define MEF_CFG_AUTO_ARP_RESP 2
/* MEF configuration host command */
#define MEF_CFG_HOSTCMD 0xFFFF

/** Type definition of mlan_ds_misc_mef_cfg for MLAN_OID_MISC_MEF_CFG */
typedef struct _mlan_ds_misc_mef_cfg
{
    /** Sub-ID for operation */
    t_u32 sub_id;
    /** Parameter according to sub-ID */
    union
    {
        /** MEF command buffer for MEF_CFG_HOSTCMD */
        mlan_ds_misc_cmd cmd_buf;
    } param;
} mlan_ds_misc_mef_cfg;

/** Type definition of mlan_ds_misc_cfp_code for MLAN_OID_MISC_CFP_CODE */
typedef struct _mlan_ds_misc_cfp_code
{
    /** CFP table code for 2.4GHz */
    t_u32 cfp_code_bg;
    /** CFP table code for 5GHz */
    t_u32 cfp_code_a;
} mlan_ds_misc_cfp_code;

/** Type definition of mlan_ds_misc_country_code for MLAN_OID_MISC_COUNTRY_CODE */
typedef struct _mlan_ds_misc_country_code
{
    /** Country Code */
    t_u8 country_code[COUNTRY_CODE_LEN];
} mlan_ds_misc_country_code;

/** action for set */
#define SUBSCRIBE_EVT_ACT_BITWISE_SET 0x0002
/** action for clear */
#define SUBSCRIBE_EVT_ACT_BITWISE_CLR 0x0003
/** BITMAP for subscribe event rssi low */
#define SUBSCRIBE_EVT_RSSI_LOW MBIT(0)
/** BITMAP for subscribe event snr low */
#define SUBSCRIBE_EVT_SNR_LOW MBIT(1)
/** BITMAP for subscribe event max fail */
#define SUBSCRIBE_EVT_MAX_FAIL MBIT(2)
/** BITMAP for subscribe event beacon missed */
#define SUBSCRIBE_EVT_BEACON_MISSED MBIT(3)
/** BITMAP for subscribe event rssi high */
#define SUBSCRIBE_EVT_RSSI_HIGH MBIT(4)
/** BITMAP for subscribe event snr high */
#define SUBSCRIBE_EVT_SNR_HIGH MBIT(5)
/** BITMAP for subscribe event data rssi low */
#define SUBSCRIBE_EVT_DATA_RSSI_LOW MBIT(6)
/** BITMAP for subscribe event data snr low */
#define SUBSCRIBE_EVT_DATA_SNR_LOW MBIT(7)
/** BITMAP for subscribe event data rssi high */
#define SUBSCRIBE_EVT_DATA_RSSI_HIGH MBIT(8)
/** BITMAP for subscribe event data snr high */
#define SUBSCRIBE_EVT_DATA_SNR_HIGH MBIT(9)
/** BITMAP for subscribe event link quality */
#define SUBSCRIBE_EVT_LINK_QUALITY MBIT(10)
/** BITMAP for subscribe event pre_beacon_lost */
#define SUBSCRIBE_EVT_PRE_BEACON_LOST MBIT(11)
/** default PRE_BEACON_MISS_COUNT */
#define DEFAULT_PRE_BEACON_MISS 30

/** Type definition of mlan_ds_subscribe_evt for MLAN_OID_MISC_CFP_CODE */
typedef struct _mlan_ds_subscribe_evt
{
    /** evt action */
    t_u16 evt_action;
    /** bitmap for subscribe event */
    t_u16 evt_bitmap;
    /** Absolute value of RSSI threshold value (dBm) */
    t_u8 low_rssi;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 low_rssi_freq;
    /** SNR threshold value (dB) */
    t_u8 low_snr;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 low_snr_freq;
    /** Failure count threshold */
    t_u8 failure_count;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 failure_count_freq;
    /** num of missed beacons */
    t_u8 beacon_miss;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 beacon_miss_freq;
    /** Absolute value of RSSI threshold value (dBm) */
    t_u8 high_rssi;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 high_rssi_freq;
    /** SNR threshold value (dB) */
    t_u8 high_snr;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 high_snr_freq;
    /** Absolute value of data RSSI threshold value (dBm) */
    t_u8 data_low_rssi;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 data_low_rssi_freq;
    /** Absolute value of data SNR threshold value (dBm) */
    t_u8 data_low_snr;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 data_low_snr_freq;
    /** Absolute value of data RSSI threshold value (dBm) */
    t_u8 data_high_rssi;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 data_high_rssi_freq;
    /** Absolute value of data SNR threshold value (dBm) */
    t_u8 data_high_snr;
    /** 0--report once, 1--report everytime happen, N -- report only happend > N consecutive times */
    t_u8 data_high_snr_freq;
    /* Link SNR threshold (dB) */
    t_u16 link_snr;
    /* Link SNR frequency */
    t_u16 link_snr_freq;
    /* Second minimum rate value as per the rate table below */
    t_u16 link_rate;
    /* Second minimum rate frequency */
    t_u16 link_rate_freq;
    /* Tx latency value (us) */
    t_u16 link_tx_latency;
    /* Tx latency frequency */
    t_u16 link_tx_lantency_freq;
    /* Number of pre missed beacons */
    t_u8 pre_beacon_miss;
} mlan_ds_subscribe_evt;
/** Max OTP user data length */
#define MAX_OTP_USER_DATA_LEN 252U

/** Type definition of mlan_ds_misc_otp_user_data for MLAN_OID_MISC_OTP_USER_DATA */
typedef struct _mlan_ds_misc_otp_user_data
{
    /** Reserved */
    t_u16 reserved;
    /** OTP user data length */
    t_u16 user_data_length;
    /** User data buffer */
    t_u8 user_data[MAX_OTP_USER_DATA_LEN];
} mlan_ds_misc_otp_user_data;

/** mlan_ds_misc_pmfcfg structure */
typedef struct _mlan_ds_misc_pmfcfg
{
    /** Management Frame Protection Capable */
    t_u8 mfpc;
    /** Management Frame Protection Required */
    t_u8 mfpr;
} mlan_ds_misc_pmfcfg;

/** mlan_ds_misc_ed_mac_mode structure */
typedef struct _mlan_ds_ed_mac_mode
{
    /** ED CTRL 2G */
    t_u16 ed_ctrl_2g;
    /** ED Offset 2G */
    t_s16 ed_offset_2g;
#if CONFIG_5GHz_SUPPORT
    /** ED CTRL 5G */
    t_u16 ed_ctrl_5g;
    /** ED Offset 5G */
    t_s16 ed_offset_5g;
#endif
} mlan_ds_misc_ed_mac_mode;

#ifdef OTP_CHANINFO
typedef struct _mlan_ds_misc_chnrgpwr_cfg
{
    /** length */
    t_u16 length;
    /** chnrgpwr buf */
    t_u8 chnrgpwr_buf[2048];
} mlan_ds_misc_chnrgpwr_cfg;
#endif

#if CONFIG_RF_TEST_MODE

typedef enum _mlan_rf_test_mode
{
    MLAN_OID_MISC_RF_TEST_GENERIC              = 0x00200075,
    MLAN_OID_MISC_RF_TEST_TX_CONT              = 0x00200076,
    MLAN_OID_MISC_RF_TEST_TX_FRAME             = 0x00200077,
    MLAN_OID_MISC_RF_TEST_HE_POWER             = 0X0020007F,
    MLAN_OID_MISC_RF_TEST_CONFIG_TRIGGER_FRAME = 0x0020008C,
} mlan_rf_test_mode;

#define MFG_CMD_SET_TEST_MODE        1
#define MFG_CMD_UNSET_TEST_MODE      0
#define MFG_CMD_TX_ANT               0x1004
#define MFG_CMD_RX_ANT               0x1005
#define MFG_CMD_TX_CONT              0x1009
#define MFG_CMD_RF_CHAN              0x100A
#define MFG_CMD_CLR_RX_ERR           0x1010
#define MFG_CMD_TX_FRAME             0x1021
#define MFG_CMD_RFPWR                0x1033
#define MFG_CMD_RF_BAND_AG           0x1034
#define MFG_CMD_RF_CHANNELBW         0x1044
#define MFG_CMD_RADIO_MODE_CFG       0x1211
#define MFG_CMD_CONFIG_MAC_HE_TB_TX  0x110A
#define MFG_CMD_CONFIG_TRIGGER_FRAME 0x110C
#define MFG_CMD_OTP_MAC_ADD          0x108C
#define MFG_CMD_OTP_CAL_DATA         0x121A

/** Configuration for Manufacturing generic command */
typedef MLAN_PACK_START struct _mlan_ds_mfg_cmd_generic_cfg
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** value 1 */
    t_u32 data1;
    /** value 2 */
    t_u32 data2;
    /** value 3 */
    t_u32 data3;
} MLAN_PACK_END mlan_ds_mfg_cmd_generic_cfg;

/** Configuration for Manufacturing command Tx Frame */
typedef MLAN_PACK_START struct _mlan_ds_mfg_cmd_tx_frame2
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** enable */
    t_u32 enable;
    /** data_rate */
    t_u32 data_rate;
    /** frame pattern */
    t_u32 frame_pattern;
    /** frame length */
    t_u32 frame_length;
    /** BSSID */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** Adjust burst sifs */
    t_u16 adjust_burst_sifs;
    /** Burst sifs in us*/
    t_u32 burst_sifs_in_us;
    /** short preamble */
    t_u32 short_preamble;
    /** active sub channel */
    t_u32 act_sub_ch;
    /** short GI */
    t_u32 short_gi;
    /** Adv coding */
    t_u32 adv_coding;
    /** Tx beamforming */
    t_u32 tx_bf;
    /** HT Greenfield Mode*/
    t_u32 gf_mode;
    /** STBC */
    t_u32 stbc;
    /** power id */
    t_u32 rsvd[1];
    /**signal bw*/
    t_u32 signal_bw;
    /** NumPkt */
    t_u32 NumPkt;
    /** MaxPE */
    t_u32 MaxPE;
    /** BeamChange */
    t_u32 BeamChange;
    /** Dcm */
    t_u32 Dcm;
    /** Doppler */
    t_u32 Doppler;
    /** MidP */
    t_u32 MidP;
    /** QNum */
    t_u32 QNum;
} MLAN_PACK_END mlan_ds_mfg_cmd_tx_frame2;

/** Configuration for Manufacturing command Tx Continuous */
typedef MLAN_PACK_START struct _mlan_ds_mfg_cmd_tx_cont
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** enable Tx*/
    t_u32 enable_tx;
    /** Continuous Wave mode */
    t_u32 cw_mode;
    /** payload pattern */
    t_u32 payload_pattern;
    /** CS Mode */
    t_u32 cs_mode;
    /** active sub channel */
    t_u32 act_sub_ch;
    /** Tx rate */
    t_u32 tx_rate;
    /** power id */
    t_u32 rsvd;
} MLAN_PACK_END mlan_ds_mfg_cmd_tx_cont;

typedef PACK_START struct _mlan_ds_mfg_Cmd_HE_TBTx_t
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** Enable Tx */
    t_u16 enable;
    /** Q num */
    t_u16 qnum;
    /** AID */
    t_u16 aid;
    /** AXQ Mu Timer */
    t_u16 axq_mu_timer;
    /** Tx Power */
    t_s16 tx_power;
} PACK_END mlan_ds_mfg_Cmd_HE_TBTx_t;

typedef MLAN_PACK_START struct _mlan_ds_mfg_cmd_otp_mac_addr_rd_wr_t
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** Destination MAC Address */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
} MLAN_PACK_END mlan_ds_mfg_cmd_otp_mac_addr_rd_wr_t;

#define CAL_DATA_LEN 2800
typedef MLAN_PACK_START struct _mlan_ds_mfg_cmd_otp_cal_data_rd_wr_t
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** CAL Data write status */
    t_u32 cal_data_status;
    /** CAL Data Length*/
    t_u32 cal_data_len;
    /** Destination MAC Address */
    t_u8 cal_data[CAL_DATA_LEN];
} MLAN_PACK_END mlan_ds_mfg_cmd_otp_cal_data_rd_wr_t;

typedef MLAN_PACK_START struct _mfg_cmd_IEEEtypes_HETrigComInfo_t
{
    t_u64 trigger_type : 4;
    t_u64 ul_len : 12;
    t_u64 more_tf : 1;
    t_u64 cs_required : 1;
    t_u64 ul_bw : 2;

    t_u64 ltf_type : 2;
    t_u64 ltf_mode : 1;
    t_u64 ltf_symbol : 3;
    t_u64 ul_stbc : 1;
    t_u64 ldpc_ess : 1;

    t_u64 ap_tx_pwr : 6;
    t_u64 pre_fec_pad_fct : 2;
    t_u64 pe_disambig : 1;
    t_u64 spatial_reuse : 16;
    t_u64 doppler : 1;
    t_u64 he_sig2 : 9;
    t_u64 reserved : 1;

} MLAN_PACK_END mfg_cmd_IEEEtypes_HETrigComInfo_t;

typedef MLAN_PACK_START struct _mfg_cmd_IEEEtypes_HETrigUserInfo_t
{
    t_u32 aid12 : 12;
    t_u32 ru_alloc_reg : 1;
    t_u32 ru_alloc : 7;
    t_u32 ul_coding_type : 1;
    t_u32 ul_mcs : 4;
    t_u32 ul_dcm : 1;
    t_u32 ss_alloc : 6;
    t_u8 ul_target_rssi : 7;
    t_u8 reserved : 1;
} MLAN_PACK_END mfg_cmd_IEEEtypes_HETrigUserInfo_t;

typedef MLAN_PACK_START struct _mfg_cmd_IEEETypes_BasicHETrigUserInfo_t
{
    t_u8 mpdu_mu_sf : 2;
    t_u8 tid_al : 3;
    t_u8 ac_pl : 1;
    t_u8 pref_ac : 2;
} MLAN_PACK_END mfg_cmd_IEEETypes_BasicHETrigUserInfo_t;

typedef MLAN_PACK_START struct _mfg_cmd_IEEEtypes_FrameCtrl_t
{
    /** Protocol Version */
    t_u8 protocol_version : 2;
    /** Type */
    t_u8 type : 2;
    /** Sub Type */
    t_u8 sub_type : 4;
    /** To DS */
    t_u8 to_ds : 1;
    /** From DS */
    t_u8 from_ds : 1;
    /** More Frag */
    t_u8 more_frag : 1;
    /** Retry */
    t_u8 retry : 1;
    /** Power Mgmt */
    t_u8 pwr_mgmt : 1;
    /** More Data */
    t_u8 more_data : 1;
    /** Wep */
    t_u8 wep : 1;
    /** Order */
    t_u8 order : 1;
} MLAN_PACK_END mfg_cmd_IEEEtypes_FrameCtrl_t;

typedef MLAN_PACK_START struct _mfg_Cmd_IEEEtypes_CtlBasicTrigHdr_t
{
    /** MFG command code */
    t_u32 mfg_cmd;
    /** Action */
    t_u16 action;
    /** Device ID */
    t_u16 device_id;
    /** MFG Error code */
    t_u32 error;
    /** enable Tx*/
    t_u32 enable_tx;
    /** enable Stand Alone HE TB */
    t_u32 standalone_hetb;
    /** Frame Control */
    mfg_cmd_IEEEtypes_FrameCtrl_t frmCtl;
    /** Duration */
    t_u16 duration;
    /** Destination MAC Address */
    t_u8 dest_addr[MLAN_MAC_ADDR_LENGTH];
    /** Source MAC Address */
    t_u8 src_addr[MLAN_MAC_ADDR_LENGTH];
    /** Common Info Field **/
    mfg_cmd_IEEEtypes_HETrigComInfo_t trig_common_field;
    /** User Info Field **/
    mfg_cmd_IEEEtypes_HETrigUserInfo_t trig_user_info_field;
    /** Trigger Dependent User Info Field **/
    mfg_cmd_IEEETypes_BasicHETrigUserInfo_t basic_trig_user_info;
} MLAN_PACK_END mfg_Cmd_IEEEtypes_CtlBasicTrigHdr_t;
#endif

#if CONFIG_MULTI_CHAN
typedef MLAN_PACK_START struct _mlan_ds_multi_chan_cfg
{
    /** Channel Time */
    t_u32 channel_time;
    /** Buffer Weight */
    t_u8 buffer_weight;
    /** tlv len */
    t_u16 tlv_len;
    /** TLV buffer */
    t_u8 tlv_buf[];
} MLAN_PACK_END mlan_ds_multi_chan_cfg;

typedef MLAN_PACK_START struct _mlan_ds_drcs_cfg
{
    /** Channel Index*/
    t_u16 chan_idx;
    /** Channel time (in TU) for chan_idx */
    t_u8 chantime;
    /** Channel swith time (in TU) for chan_idx */
    t_u8 switchtime;
    /** Undoze time (in TU) for chan_idx */
    t_u8 undozetime;
    /** Rx traffic control scheme when channel switch*/
    /** only valid for GC/STA interface*/
    t_u8 mode;
} MLAN_PACK_END mlan_ds_drcs_cfg;
#endif


#if (CONFIG_GTK_REKEY_OFFLOAD)
#define MLAN_KCK_LEN        16
#define MLAN_KEK_LEN        16
#define MLAN_REPLAY_CTR_LEN 8
/** mlan_ds_misc_gtk_rekey_data */
typedef struct _mlan_ds_misc_gtk_rekey_data
{
    /** key encryption key */
    t_u8 kek[MLAN_KEK_LEN];
    /** key confirmation key */
    t_u8 kck[MLAN_KCK_LEN];
    /** replay counter */
    t_u8 replay_ctr[MLAN_REPLAY_CTR_LEN];
} mlan_ds_misc_gtk_rekey_data;
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
typedef struct _mlan_ds_ind_rst_cfg
{
    /** Set or Get */
    t_u16 action;
    /** oob mode enable/ disable */
    t_u8 ir_mode;
    /** gpio pin */
    t_u8 gpio_pin;
} mlan_ds_ind_rst_cfg;
#endif

/** Type definition of mlan_ds_misc_cfg for MLAN_IOCTL_MISC_CFG */
typedef struct _mlan_ds_misc_cfg
{
    /** Sub-command */
    t_u32 sub_command;
    /** Miscellaneous configuration parameter */
    union
    {
        /** Generic IE for MLAN_OID_MISC_GEN_IE */
        mlan_ds_misc_gen_ie gen_ie;
        /** Region code for MLAN_OID_MISC_REGION */
        t_u32 region_code;
#if defined(SDIO_MULTI_PORT_TX_AGGR) || defined(SDIO_MULTI_PORT_RX_AGGR)
        /** SDIO MP-A Ctrl command for MLAN_OID_MISC_SDIO_MPA_CTRL */
        mlan_ds_misc_sdio_mpa_ctrl mpa_ctrl;
#endif
        /** Hostcmd for MLAN_OID_MISC_HOST_CMD */
        mlan_ds_misc_cmd hostcmd;
        /** System clock for MLAN_OID_MISC_SYS_CLOCK */
        mlan_ds_misc_sys_clock sys_clock;
#if CONFIG_WIFI_CLOCKSYNC
        mlan_ds_gpio_tsf_latch gpio_tsf_latch_config;
        mlan_ds_tsf_info tsf_info;
#endif /* CONFIG_WIFI_CLOCKSYNC */
        /** WWS set/get for MLAN_OID_MISC_WWS */
        t_u32 wws_cfg;
        /** Function init/shutdown for MLAN_OID_MISC_INIT_SHUTDOWN */
        t_u32 func_init_shutdown;
        /** Custom IE for MLAN_OID_MISC_CUSTOM_IE */
        mlan_ds_misc_custom_ie cust_ie;
        /** Tx data pause for MLAN_OID_MISC_TX_DATAPAUSE */
        mlan_ds_misc_tx_datapause tx_datapause;
        /** IP address configuration */
        mlan_ds_misc_ipaddr_cfg ipaddr_cfg;
        /** MAC control for MLAN_OID_MISC_MAC_CONTROL */
        t_u32 mac_ctrl;
        /** MEF configuration for MLAN_OID_MISC_MEF_CFG */
        mlan_ds_misc_mef_cfg mef_cfg;
        /** CFP code for MLAN_OID_MISC_CFP_CODE */
        mlan_ds_misc_cfp_code cfp_code;
        /** Country code for MLAN_OID_MISC_COUNTRY_CODE */
        mlan_ds_misc_country_code country_code;
        /** Thermal reading for MLAN_OID_MISC_THERMAL */
        t_u32 thermal;
        /** Mgmt subtype mask for MLAN_OID_MISC_RX_MGMT_IND */
        t_u32 mgmt_subtype_mask;
        /** subscribe event for MLAN_OID_MISC_SUBSCRIBE_EVENT */
        mlan_ds_subscribe_evt subscribe_event;
#ifdef DEBUG_LEVEL1
        /** Driver debug bit masks */
        t_u32 drvdbg;
#endif

        /** boot sleep enable or disable */
        t_u16 boot_sleep;

        /** Hotspot config param set */
        t_u32 hotspot_cfg;
        mlan_ds_misc_otp_user_data otp_user_data;
#ifdef WLAN_LOW_POWER_ENABLE
        t_u8 low_pwr_mode;
#endif // WLAN_LOW_POWER_ENABLE
#ifdef OTP_CHANINFO
        /** Custom regulatory domain */
        mlan_ds_custom_reg_domain custom_reg_domain;
        mlan_ds_misc_chnrgpwr_cfg rgchnpwr_cfg;
#endif
#if CONFIG_RF_TEST_MODE
        mlan_ds_mfg_cmd_generic_cfg mfg_generic_cfg;
        mlan_ds_mfg_cmd_tx_frame2 mfg_tx_frame2;
        mlan_ds_mfg_cmd_tx_cont mfg_tx_cont;
        mlan_ds_mfg_Cmd_HE_TBTx_t mfg_he_power;
        mfg_Cmd_IEEEtypes_CtlBasicTrigHdr_t mfg_tx_trigger_config;
        mlan_ds_mfg_cmd_otp_mac_addr_rd_wr_t mfg_otp_mac_addr_rd_wr;
        mlan_ds_mfg_cmd_otp_cal_data_rd_wr_t mfg_otp_cal_data_rd_wr;
#endif
#if CONFIG_MULTI_CHAN
        /** Multi-channel config for MLAN_OID_MISC_MULTI_CHAN_CFG */
        mlan_ds_multi_chan_cfg multi_chan_cfg;
        /** Multi-channel policy for MLAN_OID_MISC_MULTI_CHAN_POLICY */
        t_u16 multi_chan_policy;
        /** channel drcs time slicing config for MLAN_OID_MISC_DRCS_CFG
         */
        mlan_ds_drcs_cfg drcs_cfg[2];
#endif
#if CONFIG_ECSA
        mlan_ds_bw_chan_oper bw_chan_oper;
#endif
        mlan_embedded_dhcp_config embedded_dhcp_config;
#if CONFIG_GTK_REKEY_OFFLOAD
        /** GTK rekey data */
        mlan_ds_misc_gtk_rekey_data gtk_rekey;
#endif
#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
        mlan_ds_ind_rst_cfg ind_rst_cfg;
#endif
    } param;
} mlan_ds_misc_cfg, *pmlan_ds_misc_cfg;

/** mlan ioctl */
MLAN_API mlan_status mlan_ioctl(IN t_void *pmlan_adapter, IN pmlan_ioctl_req pioctl_req);

#endif /* !_MLAN_IOCTL_H_ */
