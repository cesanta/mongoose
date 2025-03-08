/*
 *  Copyright 2008-2022, 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file wifi-decl.h
 *\brief This file provieds Wi-Fi structure declarations
 */
#ifndef __WIFI_DECL_H__
#define __WIFI_DECL_H__

#include <stdint.h>
#include <stdbool.h>
#include <wm_utils.h>
#include <mlan_decl.h>
#include <mlan_ioctl.h>
#include <wifi_events.h>

/* fixme: remove these after complete integration with mlan */
#define MLAN_MAC_ADDR_LENGTH (6U)
/** Version string buffer length */
#define MLAN_MAX_VER_STR_LEN 128

#define WIFI_MAX_CHANNEL_NUM 42

#define PMK_BIN_LEN 32
#define PMK_HEX_LEN 64

#define MOD_GROUPS 7

#if CONFIG_DRIVER_OWE
/** The open AP in OWE transmition Mode */
#define OWE_TRANS_MODE_OPEN 1U
/** The security AP in OWE trsnsition Mode */
#define OWE_TRANS_MODE_OWE 2U
#endif

#if CONFIG_WIFI_CAPA
#define WIFI_SUPPORT_11AX   (1 << 3)
#define WIFI_SUPPORT_11AC   (1 << 2)
#define WIFI_SUPPORT_11N    (1 << 1)
#define WIFI_SUPPORT_LEGACY (1 << 0)
#endif

#if 0
/** channel_field.flags */
#define CHANNEL_FLAGS_TURBO                   0x0010
#define CHANNEL_FLAGS_CCK                     0x0020
#define CHANNEL_FLAGS_OFDM                    0x0040
#define CHANNEL_FLAGS_2GHZ                    0x0080
#define CHANNEL_FLAGS_5GHZ                    0x0100
#define CHANNEL_FLAGS_ONLY_PASSIVSCAN_ALLOW   0x0200
#define CHANNEL_FLAGS_DYNAMIC_CCK_OFDM        0x0400
#define CHANNEL_FLAGS_GFSK                    0x0800
PACK_START struct channel_field {
    t_u16 frequency;
    t_u16 flags;
} PACK_END;

/** mcs_field.known */
#define MCS_KNOWN_BANDWIDTH                   0x01
#define MCS_KNOWN_MCS_INDEX_KNOWN             0x02
#define MCS_KNOWN_GUARD_INTERVAL              0x04
#define MCS_KNOWN_HT_FORMAT                   0x08
#define MCS_KNOWN_FEC_TYPE                    0x10
#define MCS_KNOWN_STBC_KNOWN                  0x20
#define MCS_KNOWN_NESS_KNOWN                  0x40
#define MCS_KNOWN_NESS_DATA                   0x80
/** bandwidth */
#define RX_BW_20                              0
#define RX_BW_40                              1
#define RX_BW_20L                             2
#define RX_BW_20U                             3
/** mcs_field.flags
The flags field is any combination of the following:
0x03    bandwidth - 0: 20, 1: 40, 2: 20L, 3: 20U
0x04    guard interval - 0: long GI, 1: short GI
0x08    HT format - 0: mixed, 1: greenfield
0x10    FEC type - 0: BCC, 1: LDPC
0x60    Number of STBC streams
0x80    Ness - bit 0 (LSB) of Number of extension spatial streams */
PACK_START struct mcs_field {
    t_u8 known;
    t_u8 flags;
    t_u8 mcs;
} PACK_END;

/** radiotap_body.flags */
#define RADIOTAP_FLAGS_DURING_CFG             0x01
#define RADIOTAP_FLAGS_SHORT_PREAMBLE         0x02
#define RADIOTAP_FLAGS_WEP_ENCRYPTION         0x04
#define RADIOTAP_FLAGS_WITH_FRAGMENT          0x08
#define RADIOTAP_FLAGS_INCLUDE_FCS            0x10
#define RADIOTAP_FLAGS_PAD_BTW_HEADER_PAYLOAD 0x20
#define RADIOTAP_FLAGS_FAILED_FCS_CHECK       0x40
#define RADIOTAP_FLAGS_USE_SGI_HT             0x80
PACK_START struct radiotap_body {
    t_u64 timestamp;
    t_u8 flags;
    t_u8 rate;
    struct channel_field channel;
    t_s8 antenna_signal;
    t_s8 antenna_noise;
    t_u8 antenna;
    struct mcs_field mcs;
} PACK_END;

typedef PACK_START struct _radiotap_header {
    struct ieee80211_radiotap_header hdr;
    struct radiotap_body body;
} PACK_END radiotap_header_t;
#endif

/** Station information structure */
typedef struct
{
    /** MAC address buffer */
    t_u8 mac[MLAN_MAC_ADDR_LENGTH];
    /**
     * Power management status
     * 0 = active (not in power save)
     * 1 = in power save status
     */
    t_u8 power_mgmt_status;
    /** RSSI: dBm */
    t_s8 rssi;
} wifi_sta_info_t;

/** Channel list structure */
typedef PACK_START struct _wifi_scan_chan_list_t
{
    /** Number of channels */
    uint8_t num_of_chan;
    /** Channel number */
    uint8_t chan_number[MLAN_MAX_CHANNEL];
} PACK_END wifi_scan_chan_list_t;

/**
 *  Note: This is variable length structure. The size of array mac_list is
 *  equal to count. The caller of the API which returns this structure does
 *  not need to separately free the array mac_list. It only needs to free
 *  the sta_list_t object after use.
 */
typedef struct
{
    /** Count */
    int count;
    /*
     * Variable length array. Max size is MAX_NUM_CLIENTS.
     */
    /* wifi_sta_info_t *list; */
} wifi_sta_list_t;

/** BSS type : STA */
#define BSS_TYPE_STA 0U
/** BSS type : UAP */
#define BSS_TYPE_UAP 1U

#define UAP_DEFAULT_CHANNEL       0
#define UAP_DEFAULT_BANDWIDTH     2
#define UAP_DEFAULT_BEACON_PERIOD 100
#define UAP_DEFAULT_HIDDEN_SSID   0

enum wifi_bss_security
{
    WIFI_SECURITY_NONE = 0,
    WIFI_SECURITY_WEP_STATIC,
    WIFI_SECURITY_WEP_DYNAMIC,
    WIFI_SECURITY_WPA,
    WIFI_SECURITY_WPA2,
};

enum wifi_bss_features
{
    WIFI_BSS_FEATURE_WMM = 0,
    WIFI_BSS_FEATURE_WPS = 1,
};

struct wifi_message
{
    uint16_t event;
    enum wifi_event_reason reason;
    void *data;
};


/* Wlan Cipher structure */
typedef struct
{
    /** 1 bit value can be set for none */
    uint16_t none : 1;
    /** 1 bit value can be set for wep40 */
    uint16_t wep40 : 1;
    /** 1 bit value can be set for wep104 */
    uint16_t wep104 : 1;
    /** 1 bit value can be set for tkip */
    uint16_t tkip : 1;
    /** 1 bit valuecan be set for ccmp */
    uint16_t ccmp : 1;
    /**  1 bit valuecan be set for aes 128 cmac */
    uint16_t aes_128_cmac : 1;
    /** 1 bit value can be set for gcmp */
    uint16_t gcmp : 1;
    /** 1 bit value can be set for sms4 */
    uint16_t sms4 : 1;
    /** 1 bit value can be set for gcmp 256 */
    uint16_t gcmp_256 : 1;
    /** 1 bit valuecan be set for ccmp 256 */
    uint16_t ccmp_256 : 1;
    /** 1 bit is reserved */
    uint16_t rsvd : 1;
    /** 1 bit value can be set for bip gmac 128 */
    uint16_t bip_gmac_128 : 1;
    /** 1 bit value can be set for bip gmac 256 */
    uint16_t bip_gmac_256 : 1;
    /** 1 bit value can be set for bip cmac 256 */
    uint16_t bip_cmac_256 : 1;
    /** 1 bit valuecan be set for gtk not used */
    uint16_t gtk_not_used : 1;
    /** 4 bits are reserved */
    uint16_t rsvd2 : 2;
} _Cipher_t;

/* Security mode structure */
typedef struct
{
    /** No security */
    uint32_t noRsn : 1;
    /** WEP static */
    uint32_t wepStatic : 1;
    /** WEP dynamic */
    uint32_t wepDynamic : 1;
    /** WPA */
    uint32_t wpa : 1;
    /** WPA none */
    uint32_t wpaNone : 1;
    /** WPA 2 */
    uint32_t wpa2 : 1;
    /** WPA 2 sha256 */
    uint32_t wpa2_sha256 : 1;
    /** OWE */
    uint32_t owe : 1;
    /** WPA3 SAE */
    uint32_t wpa3_sae : 1;
    /** 802.1x */
    uint32_t wpa2_entp : 1;
    /** FT 802.1x */
    uint32_t ft_1x : 1;
    /** FT 802.1x sha384 */
    uint32_t ft_1x_sha384 : 1;
    /** FT PSK  */
    uint32_t ft_psk : 1;
    /** FT SAE */
    uint32_t ft_sae : 1;
    /** WPA3 Enterprise */
    uint32_t wpa3_entp : 1;
    /** WPA3 802.1x sha256 */
    uint32_t wpa3_1x_sha256 : 1;
    /** WPA3 802.1x sha384 */
    uint32_t wpa3_1x_sha384 : 1;
    /** Reserved 16 bits */
    uint32_t rsvd : 16;
} _SecurityMode_t;

/* TODO: clean up the parts brought over from the Host SME BSSDescriptor_t,
 * remove ifdefs, consolidate security info */

/** MLAN Maximum SSID Length */
#define MLAN_MAX_SSID_LENGTH (32U)
/** MLAN Maximum PASSPHRASE Length */
#define MLAN_MAX_PASS_LENGTH (64)

/** Scan result information */
struct wifi_scan_result2
{
    uint8_t bssid[MLAN_MAC_ADDR_LENGTH]; /*!< BSSID array */
    bool is_ibss_bit_set;                /*!< Is bssid set? */

    uint8_t ssid[MLAN_MAX_SSID_LENGTH];  /*!< ssid array */
    int ssid_len;                        /*!< SSID length */
    uint8_t Channel;                     /*!< Channel associated to the BSSID */
    uint8_t RSSI;                        /*!< Received signal strength */
    uint16_t beacon_period;              /*!< Beacon period */
    uint16_t dtim_period;                /*!< DTIM period */
    _SecurityMode_t WPA_WPA2_WEP;        /*!< Security mode info */
    _Cipher_t wpa_mcstCipher;            /*!< WPA multicast cipher */
    _Cipher_t wpa_ucstCipher;            /*!< WPA unicast cipher */
    _Cipher_t rsn_mcstCipher;            /*!< No security multicast cipher */
    _Cipher_t rsn_ucstCipher;            /*!< No security unicast cipher */
    bool is_pmf_required;                /*!< Is pmf required flag */
    t_u8 ap_mfpc;                        /*!< MFPC bit of AP */
    t_u8 ap_mfpr;                        /*!< MFPR bit of AP */
    t_u8 ap_pwe;                         /*!< PWE bit of AP */

    /*!<
     **  WPA_WPA2 = 0 => Security not enabled
     **  = 1 => WPA mode
     **  = 2 => WPA2 mode
     **  = 3 => WEP mode
     */
    bool phtcap_ie_present;  /*!< PHT CAP IE present info */
    bool phtinfo_ie_present; /*!< PHT INFO IE present info */
#if CONFIG_11AC
    /** 11AC VHT capab support */
    bool pvhtcap_ie_present;
#endif
#if CONFIG_11AX
    /** 11AX HE capab support */
    bool phecap_ie_present;
#endif

    bool wmm_ie_present;                       /*!< WMM IE present info */
    uint16_t band;                             /*!< Band info */

    bool wps_IE_exist;                         /*!< WPS IE exist info */
    uint16_t wps_session;                      /*!< WPS session */
    bool wpa2_entp_IE_exist;                   /*!< WPA2 enterprise IE exist info */
    uint8_t trans_mode;                        /*!< Trans mode */
    uint8_t trans_bssid[MLAN_MAC_ADDR_LENGTH]; /*!< Trans bssid array */
    uint8_t trans_ssid[MLAN_MAX_SSID_LENGTH];  /*!< Trans ssid array */
    int trans_ssid_len;                        /*!< Trans bssid length */
#if CONFIG_DRIVER_MBO
    bool mbo_assoc_disallowed;                 /*!< MBO disallowed */
#endif
#if CONFIG_11R
    /** Mobility domain identifier */
    uint16_t mdid;
#endif
#if CONFIG_11K
    /** Neigbort report support */
    bool neighbor_report_supported;
#endif
#if CONFIG_11V
    /** bss transition support */
    bool bss_transition_supported;
#endif
};

/** MAC address */
typedef struct
{
    /** Mac address array */
    char mac[MLAN_MAC_ADDR_LENGTH];
} wifi_mac_addr_t;

/** Firmware version */
typedef struct
{
    /** Firmware version string */
    char version_str[MLAN_MAX_VER_STR_LEN];
} wifi_fw_version_t;

/** Extended Firmware version */
typedef struct
{
    /** ID for extended version select */
    uint8_t version_str_sel;
    /** Firmware version string */
    char version_str[MLAN_MAX_VER_STR_LEN];
} wifi_fw_version_ext_t;

enum wlan_type
{
    WLAN_TYPE_NORMAL = 0,
    WLAN_TYPE_WIFI_CALIB,
    WLAN_TYPE_FCC_CERTIFICATION,
};

/** Tx power levels */
typedef struct
{
    /** Current power level */
    uint16_t current_level;
    /** Maximum power level */
    uint8_t max_power;
    /** Minimum power level */
    uint8_t min_power;

} wifi_tx_power_t;

/** Rf channel */
typedef struct
{
    /** Current channel */
    uint16_t current_channel;
    /** RF Type */
    uint16_t rf_type;
} wifi_rf_channel_t;

/** Remain on channel info structure */
typedef struct
{
    /** Remove */
    uint16_t remove;
    /** Current status */
    uint8_t status;
    /** band configuration */
    uint8_t bandcfg;
    /** Channel */
    uint8_t channel;
    /** Remain on channel period */
    uint32_t remain_period;
} wifi_remain_on_channel_t;

#if CONFIG_11AX
/** TX Rate Setting */
typedef PACK_START struct _txrate_setting
{
    /** Preamble */
    t_u16 preamble : 2; /*BIT1-BIT0:
                         *  For legacy 11b: preamble type
                         *    00    = long
                         *    01    = short
                         *    10/11  = reserved
                         *  For legacy 11g: reserved
                         *  For 11n: Green field PPDU indicator
                         *    00 = HT-mix
                         *    01 = HT-GF
                         *    10/11 = reserved.
                         *  For 11ac: reserved.
                         *  For 11ax:
                         *    00 = HE-SU
                         *    01 = HE-EXT-SU
                         *    10 = HE-MU
                         *    11 = HE trigger based
                         */
    /** Bandwidth */
    t_u16 bandwidth : 3; /* BIT2- BIT4
                          * For 11n and 11ac traffic: Bandwidth
                          *    0 = 20Mhz
                          *    1 = 40Mhz
                          *    2 = 80 Mhz
                          *    3 = 160 Mhz
                          *    4-7 = reserved
                          *  For legacy rate : BW>0 implies non-HT duplicates.
                          *  For HE SU PPDU:
                          *    0 = 20Mhz
                          *    1 = 40Mhz
                          *    2 = 80 Mhz
                          *    3 = 160 Mhz
                          *    4-7 = reserved
                          *  For HE ER SU PPDU:
                          *    0 = 242-tone RU
                          *    1 = upper frequency 106 tone RU within the primary 20 Mhz.
                          *  For HE MU PPDU:
                          *    0 = 20Mhz.
                          *    1 = 40Mhz.
                          *    2 = 80Mhz non-preamble puncturing mode
                          *    3 = 160Mhz and 80+80 Mhz non-preamble.
                          *    4 = for preemble puncturing in 80 Mhz ,
                          *        where in the preamble only the secondary 20Mhz is punctured.
                          *    5 = for preemble puncturing in 80 Mhz ,
                          *        where in the preamble only one of the two 20Mhz subchannels in the secondary 40Mhz is
                          * punctured.  6 = for preemble puncturing in 160 Mhz or 80 Mhz + 80 Mhz,  where in the primary
                          * 80 Mhz of the preamble only the secondary 20 Mhz is punctured.  7 = for preemble puncturing
                          * in 160 Mhz or 80 Mhz + 80 Mhz,  where in the primary 80 Mhz of the preamble the primary 40
                          * Mhz is present.
                          */
    /** Short GI */
    t_u16 shortGI : 2; /*BIT5- BIT6
                        *  For legacy: not used
                        *  For 11n: 00 = normal, 01 =shortGI, 10/11 = reserved
                        *  For 11ac: SGI map to VHT-SIG-A2[0]
                        *           VHT-SIG-A2[1] is set to 1 if short guard interval is used
                        *           and NSYM mod 10 = 9, otherwise set to 0.
                        *  For 11ax:
                        *           00 = 1xHELTF+GI0.8usec
                        *           01 = 2xHELTF+GI0.8usec
                        *           10 = 2xHELTF+GI1.6usec
                        *           11 = 4xHELTF+GI0.8 usec if both DCM and STBC are 1
                        *                4xHELTF+GI3.2 usec otherwise
                        */
    /** STBC */
    t_u16 stbc : 1; // BIT7, 0: no STBC; 1: STBC
    /** DCM */
    t_u16 dcm : 1; // BIT8, 0: no DCM; 1: DCM used.
    /** Adv coding */
    t_u16 adv_coding : 1; // BIT9, 0: BCC; 1: LDPC.
    /** Doppler */
    t_u16 doppler : 2; /* BIT11-BIT10,
                          00: Doppler0
                          01: Doppler 1 with Mma =10
                          10: Doppler 1 with Mma =20
                       */
    /** Max PK text */
    t_u16 max_pktext : 2; /*BIT12-BIT13:
                           * Max packet extension
                           *  0 - 0 usec
                           *  1 - 8 usec
                           *  2 - 16 usec.
                           */
    /** Reserved */
    t_u16 reserverd : 2; // BIT14-BIT15
} PACK_END txrate_setting;

#if CONFIG_MMSF
typedef struct
{
    t_u8 *enable;
    t_u8 *Density;
    t_u8 *MMSF;
} wifi_mmsf_cfg_t;
#endif
#endif

/** Data structure for cmd txratecfg */
typedef PACK_START struct _wifi_rate_cfg_t
{
    /** LG rate: 0, HT rate: 1, VHT rate: 2 */
    mlan_rate_format rate_format;
    /** Rate/MCS index (0xFF: auto) */
    t_u32 rate_index;
    /** Rate rate */
    t_u32 rate;
#if (CONFIG_11AC) || (CONFIG_11AX)
    /** NSS */
    t_u32 nss;
#endif
    /** Rate Setting */
    t_u16 rate_setting;
} PACK_END wifi_rate_cfg_t;

/** Data structure for cmd get data rate */
typedef PACK_START struct _wifi_data_rate_t
{
    /** Tx data rate */
    t_u32 tx_data_rate;
    /** Rx data rate */
    t_u32 rx_data_rate;

    /** Tx channel bandwidth */
    t_u32 tx_bw;
    /** Tx guard interval */
    t_u32 tx_gi;
    /** Rx channel bandwidth */
    t_u32 rx_bw;
    /** Rx guard interval */
    t_u32 rx_gi;

#ifndef SD8801
    /** MCS index */
    t_u32 tx_mcs_index;
    /** MCS index */
    t_u32 rx_mcs_index;
#if (CONFIG_11AC) || (CONFIG_11AX)
    /** NSS */
    t_u32 tx_nss;
    /** NSS */
    t_u32 rx_nss;
#endif
    /** LG rate: 0, HT rate: 1, VHT rate: 2 */
    mlan_rate_format tx_rate_format;
    /** LG rate: 0, HT rate: 1, VHT rate: 2 */
    mlan_rate_format rx_rate_format;
#endif
} PACK_END wifi_data_rate_t;

enum wifi_ds_command_type
{
    WIFI_DS_RATE_CFG      = 0,
    WIFI_DS_GET_DATA_RATE = 1,
};

/** Type definition of wifi_ds_rate */
typedef PACK_START struct _wifi_ds_rate
{
    /** Sub-command */
    enum wifi_ds_command_type sub_command;
#if CONFIG_AUTO_NULL_TX
    /** Only set auto tx fix rate */
    t_u16 auto_null_fixrate_enable;
#endif
    /** Rate configuration parameter */
    union
    {
        /** Rate configuration for MLAN_OID_RATE_CFG */
        wifi_rate_cfg_t rate_cfg;
        /** Data rate for MLAN_OID_GET_DATA_RATE */
        wifi_data_rate_t data_rate;
    } param;
} PACK_END wifi_ds_rate;

/** Type definition of wifi_ed_mac_ctrl_t */
typedef PACK_START struct _wifi_ed_mac_ctrl_t
{
    /** ED CTRL 2G */
    t_u16 ed_ctrl_2g;
    /** ED Offset 2G */
    t_s16 ed_offset_2g;
    /** ED CTRL 5G */
    t_u16 ed_ctrl_5g;
    /** ED Offset 5G */
    t_s16 ed_offset_5g;
} PACK_END wifi_ed_mac_ctrl_t;

/** Type definition of wifi_bandcfg_t */
typedef PACK_START struct _wifi_bandcfg_t
{
    /** Infra band */
    t_u16 config_bands;
    /** fw supported band */
    t_u16 fw_bands;
} PACK_END wifi_bandcfg_t;

#ifdef SD8801
/** Type definition of wifi_ext_coex_config_t */
typedef PACK_START struct _wifi_ext_coex_config_t
{
    /** Enable or disable external coexistence */
    t_u8 Enabled;
    /** Ignore the priority of the external radio request */
    t_u8 IgnorePriority;
    /** Default priority when the priority of the external radio
request is ignored */
    t_u8 DefaultPriority;
    /** Input request GPIO pin for EXT_RADIO_REQ signal */
    t_u8 EXT_RADIO_REQ_ip_gpio_num;
    /** Input request GPIO polarity for EXT_RADIO_REQ signal */
    t_u8 EXT_RADIO_REQ_ip_gpio_polarity;
    /** Input priority GPIO pin for EXT_RADIO_PRI signal */
    t_u8 EXT_RADIO_PRI_ip_gpio_num;
    /** Input priority GPIO polarity for EXT_RADIO_PRI signal */
    t_u8 EXT_RADIO_PRI_ip_gpio_polarity;
    /** Output grant GPIO pin for WLAN_GRANT signal */
    t_u8 WLAN_GRANT_op_gpio_num;
    /** Output grant GPIO polarity of WLAN_GRANT */
    t_u8 WLAN_GRANT_op_gpio_polarity;
    /** Reserved Bytes */
    t_u16 reserved_1;
    /** Reserved Bytes */
    t_u16 reserved_2;
} PACK_END wifi_ext_coex_config_t;

/** Type definition of wifi_ext_coex_stats_t */
typedef PACK_START struct _wifi_ext_coex_stats_t
{
    /** External Radio Request count */
    t_u16 ext_radio_req_count;
    /** External Radio Priority count */
    t_u16 ext_radio_pri_count;
    /** WLAN GRANT count */
    t_u16 wlan_grant_count;
} PACK_END wifi_ext_coex_stats_t;
#endif

/** Type definition of wifi_antcfg_t */
typedef PACK_START struct _wifi_antcfg_t
{
    /** Antenna Mode */
    t_u32 *ant_mode;
    /** Evaluate Time */
    t_u16 *evaluate_time;
    /** Current antenna*/
    t_u16 *current_antenna;
#ifdef RW610
    /** Evaluate mode */
    t_u8 *evaluate_mode;
#endif
} PACK_END wifi_antcfg_t;

/** CW_MODE_CTRL structure */
typedef PACK_START struct _wifi_cw_mode_ctrl_t
{
    /** Mode of Operation 0:Disable 1: Tx Continuous Packet 2 : Tx
 Continuous Wave */
    t_u8 mode;
    /** channel */
    t_u8 channel;
    /** channel info */
    t_u8 chanInfo;
    /** Tx Power level in dBm */
    t_u16 txPower;
    /** Packet Length */
    t_u16 pktLength;
    /** bit rate info */
    t_u32 rateInfo;
} PACK_END wifi_cw_mode_ctrl_t;

/** TBTT offset structure */
typedef struct
{
    /** Min TBTT offset */
    t_u32 min_tbtt_offset;
    /** Max TBTT offset */
    t_u32 max_tbtt_offset;
    /** AVG TBTT offset */
    t_u32 avg_tbtt_offset;
} wifi_tbtt_offset_t;

#ifndef BIT
#define BIT(n) (1U << (n))
#endif
#define WOWLAN_MAX_PATTERN_LEN           20
#define WOWLAN_MAX_OFFSET_LEN            50
#define MAX_NUM_FILTERS                  10
#define MEF_MODE_HOST_SLEEP              (1 << 0)
#define MEF_MODE_NON_HOST_SLEEP          (1 << 1)
#define MEF_ACTION_WAKE                  (1 << 0)
#define MEF_ACTION_ALLOW                 (1 << 1)
#define MEF_ACTION_ALLOW_AND_WAKEUP_HOST 3
#define MEF_AUTO_ARP                     0x10
#define MEF_AUTO_PING                    0x20
#define MEF_NS_RESP                      0x40
#define MEF_MAGIC_PKT                    0x80
#define CRITERIA_BROADCAST               MBIT(0)
#define CRITERIA_UNICAST                 MBIT(1)
#define CRITERIA_MULTICAST               MBIT(3)

#define MAX_NUM_ENTRIES  8
#define MAX_NUM_BYTE_SEQ 6
#define MAX_NUM_MASK_SEQ 6

#define OPERAND_DNUM     1
#define OPERAND_BYTE_SEQ 2

#define MAX_OPERAND  0x40
#define TYPE_BYTE_EQ (MAX_OPERAND + 1)
#define TYPE_DNUM_EQ (MAX_OPERAND + 2)
#define TYPE_BIT_EQ  (MAX_OPERAND + 3)

#define RPN_TYPE_AND (MAX_OPERAND + 4)
#define RPN_TYPE_OR  (MAX_OPERAND + 5)

#define ICMP_OF_IP_PROTOCOL 0x01
#define TCP_OF_IP_PROTOCOL  0x06
#define UDP_OF_IP_PROTOCOL  0x11

#define IPV4_PKT_OFFSET      20
#define IP_PROTOCOL_OFFSET   31
#define PORT_PROTOCOL_OFFSET 44

#define FILLING_TYPE      MBIT(0)
#define FILLING_PATTERN   MBIT(1)
#define FILLING_OFFSET    MBIT(2)
#define FILLING_NUM_BYTES MBIT(3)
#define FILLING_REPEAT    MBIT(4)
#define FILLING_BYTE_SEQ  MBIT(5)
#define FILLING_MASK_SEQ  MBIT(6)

/** Type definition of filter_item
 *  support three match methods:
 *  <1>Byte comparison type=0x41
 *  <2>Decimal comparison type=0x42
 *  <3>Bit comparison type=0x43
 */
typedef struct _wifi_mef_filter_t
{
    /** flag*/
    t_u32 fill_flag;
    /** BYTE 0X41; Decimal 0X42; Bit 0x43*/
    t_u16 type;
    /** value*/
    t_u32 pattern;
    /** offset*/
    t_u16 offset;
    /** number of bytes*/
    t_u16 num_bytes;
    /** repeat*/
    t_u16 repeat;
    /** byte number*/
    t_u8 num_byte_seq;
    /** array*/
    t_u8 byte_seq[MAX_NUM_BYTE_SEQ];
    /** mask numbers*/
    t_u8 num_mask_seq;
    /** array*/
    t_u8 mask_seq[MAX_NUM_MASK_SEQ];
} wifi_mef_filter_t;

/** MEF entry struct */
typedef struct _wifi_mef_entry_t
{
    /** mode: bit0--hostsleep mode; bit1--non hostsleep mode */
    t_u8 mode;
    /** action: 0--discard and not wake host;
                1--discard and wake host;
                3--allow and wake host;*/
    t_u8 action;
    /** filter number */
    t_u8 filter_num;
    /** filter array*/
    wifi_mef_filter_t filter_item[MAX_NUM_FILTERS];
    /** rpn array*/
    t_u8 rpn[MAX_NUM_FILTERS];
} wifi_mef_entry_t;

/** Wifi filter config struct */
typedef struct _wifi_flt_cfg
{
    /** Filter Criteria */
    t_u32 criteria;
    /** Number of entries */
    t_u16 nentries;
    /** MEF entry*/
    wifi_mef_entry_t mef_entry[MAX_NUM_ENTRIES];
} wifi_flt_cfg_t;

/* User defined pattern struct */
typedef struct
{
    /** pattern offset of received pattern */
    t_u8 pkt_offset;
    /** pattern length */
    t_u8 pattern_len;
    /** wowlan pattern */
    t_u8 pattern[WOWLAN_MAX_PATTERN_LEN];
    /** mask */
    t_u8 mask[6];
} wifi_wowlan_pattern_t;

/** Wowlan Pattern config struct */
typedef struct
{
    /** Enable user defined pattern*/
    t_u8 enable;
    /** number of patterns******/
    t_u8 n_patterns;
    /** user define pattern*/
    wifi_wowlan_pattern_t patterns[MAX_NUM_FILTERS];
} wifi_wowlan_ptn_cfg_t;

/** TCP keep alive information */
typedef struct
{
    /** Enable keep alive */
    t_u8 enable;
    /** Reset */
    t_u8 reset;
    /** Keep alive timeout */
    t_u32 timeout;
    /** Keep alive interval */
    t_u16 interval;
    /** Maximum keep alives */
    t_u16 max_keep_alives;
    /** Destination MAC address */
    t_u8 dst_mac[MLAN_MAC_ADDR_LENGTH];
    /** Destination IP */
    t_u32 dst_ip;
    /** Destination TCP port */
    t_u16 dst_tcp_port;
    /** Source TCP port */
    t_u16 src_tcp_port;
    /** Sequence number */
    t_u32 seq_no;
} wifi_tcp_keep_alive_t;

/** TCP nat keep alive information */
typedef struct
{
    /** Keep alive interval */
    t_u16 interval;
    /** Destination MAC address */
    t_u8 dst_mac[MLAN_MAC_ADDR_LENGTH];
    /** Destination IP */
    t_u32 dst_ip;
    /** Destination port */
    t_u16 dst_port;
} wifi_nat_keep_alive_t;

#if CONFIG_CLOUD_KEEP_ALIVE
#define MKEEP_ALIVE_IP_PKT_MAX 256
/** Cloud keep alive information */
typedef struct
{
    /** Keep alive id */
    t_u8 mkeep_alive_id;
    /** Enable keep alive */
    t_u8 enable;
    /** Enable/Disable tcp reset */
    t_u8 reset;
    /** Saved in driver */
    t_u8 cached;
    /** Period to send keep alive packet(The unit is milliseconds) */
    t_u32 send_interval;
    /** Period to send retry packet(The unit is milliseconds) */
    t_u16 retry_interval;
    /** Count to send retry packet */
    t_u16 retry_count;
    /** Source MAC address */
    t_u8 src_mac[MLAN_MAC_ADDR_LENGTH];
    /** Destination MAC address */
    t_u8 dst_mac[MLAN_MAC_ADDR_LENGTH];
    /** Source IP */
    t_u32 src_ip;
    /** Destination IP */
    t_u32 dst_ip;
    /** Source Port */
    t_u16 src_port;
    /** Destination Port */
    t_u16 dst_port;
    /** Packet length */
    t_u16 pkt_len;
    /** Packet buffer */
    t_u8 packet[MKEEP_ALIVE_IP_PKT_MAX];
} wifi_cloud_keep_alive_t;
#endif

/** RSSI information */
typedef struct
{
    /** Data RSSI last */
    int16_t data_rssi_last;
    /** Data nf last */
    int16_t data_nf_last;
    /** Data RSSI average */
    int16_t data_rssi_avg;
    /** Data nf average */
    int16_t data_nf_avg;
    /** BCN SNR */
    int16_t bcn_snr_last;
    /** BCN SNR average */
    int16_t bcn_snr_avg;
    /** Data SNR last */
    int16_t data_snr_last;
    /** Data SNR average */
    int16_t data_snr_avg;
    /** BCN RSSI */
    int16_t bcn_rssi_last;
    /** BCN nf */
    int16_t bcn_nf_last;
    /** BCN RSSI average */
    int16_t bcn_rssi_avg;
    /** BCN nf average */
    int16_t bcn_nf_avg;
} wifi_rssi_info_t;

/**
 * Data structure for subband set
 *
 * For uAP 11d support
 */
typedef struct
{
    /** First channel */
    t_u8 first_chan;
    /** Number of channels */
    t_u8 no_of_chan;
    /** Maximum Tx power in dBm */
    t_u8 max_tx_pwr;

} wifi_sub_band_set_t;

/**
 * Data structure for Channel attributes
 *
 */
typedef PACK_START struct
{
    /** Channel Number */
    t_u8 chan_num;
    /** Channel frequency for this channel */
    t_u16 chan_freq;
    /** Passice Scan or RADAR Detect*/
    bool passive_scan_or_radar_detect;
} PACK_END wifi_chan_info_t;

/**
 * Data structure for Channel List Config
 *
 */
typedef PACK_START struct
{
    /** Number of Channels */
    t_u8 num_chans;
    /** Channel Info */
    wifi_chan_info_t chan_info[54];
} PACK_END wifi_chanlist_t;

/** Wifi subband enum */
typedef enum
{
    /** Subband 2.4 GHz */
    SubBand_2_4_GHz = 0x00,
    /** Subband 5 GHz 0 */
    SubBand_5_GHz_0 = 0x10,
    /** Subband 5 GHz 1 */
    SubBand_5_GHz_1 = 0x11,
    /** Subband 5 GHz 2 */
    SubBand_5_GHz_2 = 0x12,
    /** Subband 5 GHz 3 */
    SubBand_5_GHz_3 = 0x13,
} wifi_SubBand_t;

/**
 * Data structure for Channel descriptor
 *
 * Set CFG data for Tx power limitation
 *
 * start_freq: Starting Frequency of the band for this channel\n
 *                  2407, 2414 or 2400 for 2.4 GHz\n
 *                  5000\n
 *                  4000\n
 * chan_width: Channel Width\n
 *                  20\n
 * chan_num  : Channel Number\n
 *
 */
typedef PACK_START struct
{
    /** Starting frequency of the band for this channel */
    t_u16 start_freq;
    /** Channel width */
    t_u8 chan_width;
    /** Channel Number */
    t_u8 chan_num;
} PACK_END wifi_channel_desc_t;

/**
 * Data structure for Modulation Group
 *
 * mod_group : ModulationGroup\n
 *                  0: CCK (1,2,5.5,11 Mbps)\n
 *                  1: OFDM (6,9,12,18 Mbps)\n
 *                  2: OFDM (24,36 Mbps)\n
 *                  3: OFDM (48,54 Mbps)\n
 *                  4: HT20 (0,1,2)\n
 *                  5: HT20 (3,4)\n
 *                  6: HT20 (5,6,7)\n
 *                  7: HT40 (0,1,2)\n
 *                  8: HT40 (3,4)\n
 *                  9: HT40 (5,6,7)\n
 *                  10: HT2_20 (8,9,10)\n
 *                  11: HT2_20 (11,12)\n
 *                  12: HT2_20 (13,14,15)\n
 *tx_power   : Power Limit in dBm\n
 *
 */
typedef PACK_START struct
{
    /** Modulation group */
    t_u8 mod_group;
    /** Tx Power */
    t_u8 tx_power;
} PACK_END wifi_txpwrlimit_entry_t;

/**
 * Data structure for TRPC config
 *
 * For TRPC support
 */
typedef PACK_START struct
{
    /** Number of modulation groups */
    t_u8 num_mod_grps;
    /** Chnannel descriptor */
    wifi_channel_desc_t chan_desc;
    /** Channel Modulation groups */
#if CONFIG_11AX
    wifi_txpwrlimit_entry_t txpwrlimit_entry[20];
#elif (CONFIG_11AC)
    wifi_txpwrlimit_entry_t txpwrlimit_entry[16];
#else
    wifi_txpwrlimit_entry_t txpwrlimit_entry[10];
#endif /* CONFIG_11AX */
} PACK_END wifi_txpwrlimit_config_t;

/**
 * Data structure for Channel TRPC config
 *
 * For TRPC support
 */
typedef PACK_START struct
{
    /** SubBand */
    wifi_SubBand_t subband;
    /** Number of Channels */
    t_u8 num_chans;
    /** TRPC config */
#if defined(SD9177) || defined(IW610)
    wifi_txpwrlimit_config_t txpwrlimit_config[43];
#else
    wifi_txpwrlimit_config_t txpwrlimit_config[40];
#endif
} PACK_END wifi_txpwrlimit_t;

#if CONFIG_11AX
typedef PACK_START struct _wifi_rupwrlimit_config_t
{
    /** start freq */
    t_u16 start_freq;
    /* channel width */
    t_u8 width;
    /** channel number */
    t_u8 chan_num;
    /** chan ru Power */
    t_s16 ruPower[MAX_RU_COUNT];
} PACK_END wifi_rupwrlimit_config_t;

/**
 * Data structure for Channel RU PWR config
 *
 * For RU PWR support
 */
typedef PACK_START struct
{
    /** Number of Channels */
    t_u8 num_chans;
    /** RU PWR config */
    wifi_rupwrlimit_config_t rupwrlimit_config[MAX_RUTXPWR_NUM];
} PACK_END wifi_rutxpwrlimit_t;

/** Wi-Fi 11AX Configuration */
typedef PACK_START struct
{
    /** Band */
    t_u8 band;
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
    /** val for PE thresholds */
    t_u8 val[4];
} PACK_END wifi_11ax_config_t;

#if CONFIG_11AX_TWT
/** Wi-Fi TWT setup configuration */
typedef PACK_START struct
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
} PACK_END wifi_twt_setup_config_t;

/** Wi-Fi Teardown Configuration */
typedef PACK_START struct
{
    /** TWT Flow Identifier. Range: [0-7] */
    t_u8 flow_identifier;
    /** Negotiation Type. 0: Future Individual TWT SP start time, 1: Next
     * Wake TBTT time */
    t_u8 negotiation_type;
    /** Tear down all TWT. 1: To teardown all TWT, 0 otherwise */
    t_u8 teardown_all_twt;
} PACK_END wifi_twt_teardown_config_t;

/** Wi-Fi BTWT Configuration */
typedef PACK_START struct
{
    /** Only support 1: Set*/
    t_u16 action;
    /** Broadcast TWT AP config */
    t_u16 sub_id;
    /** Range 64-255 */
    t_u8 nominal_wake;
    /** Max STA Support */
    t_u8 max_sta_support;
    /** TWT Mantissa */
    t_u16 twt_mantissa;
    /** TWT Offset */
    t_u16 twt_offset;
    /** TWT Exponent */
    t_u8 twt_exponent;
    /** SP Gap */
    t_u8 sp_gap;
} PACK_END wifi_btwt_config_t;

#define WLAN_BTWT_REPORT_LEN     9
#define WLAN_BTWT_REPORT_MAX_NUM 6
/** Wi-Fi TWT Report Configuration */
typedef PACK_START struct
{
    /** TWT report type, 0: BTWT id */
    t_u8 type;
    /** TWT report length of value in data */
    t_u8 length;
    /** Reserved 2 */
    t_u8 reserve[2];
    /** TWT report buffer */
    t_u8 data[WLAN_BTWT_REPORT_LEN * WLAN_BTWT_REPORT_MAX_NUM];
} PACK_END wifi_twt_report_t;

#endif /* CONFIG_11AX_TWT */
#endif

#if CONFIG_WIFI_CLOCKSYNC
/** Wi-Fi Clock sync configuration */
typedef PACK_START struct
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
} PACK_END wifi_clock_sync_gpio_tsf_t;

/** Wi-Fi TSF information */
typedef PACK_START struct
{
    /**get tsf info format */
    t_u16 tsf_format;
    /**tsf info */
    t_u16 tsf_info;
    /**tsf */
    t_u64 tsf;
    /**Positive or negative offset in microsecond from Beacon TSF to GPIO toggle TSF  */
    t_s32 tsf_offset;
} PACK_END wifi_tsf_info_t;
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_NET_MONITOR
typedef t_u8 wifi_802_11_mac_addr[MLAN_MAC_ADDR_LENGTH];

/** Network monitor structure */
typedef PACK_START struct
{
    /** Action */
    t_u16 action;
    /** Monitor activity */
    t_u16 monitor_activity;
    /** Filter flags */
    t_u16 filter_flags;
    /** Channel scan parameter : Radio type */
    t_u8 radio_type;
    /** Channel number */
    t_u8 chan_number;
    /** mac num of filter*/
    t_u8 filter_num;
    /** Source address of the packet to receive */
    wifi_802_11_mac_addr mac_addr[MAX_MONIT_MAC_FILTER_NUM];
} PACK_END wifi_net_monitor_t;

/** Beacon information structure */
typedef PACK_START struct
{
    /** Frame control flags */
    uint8_t frame_ctrl_flags;
	/** time interval */
    uint16_t duration;
    /** Destination MAC address */
    char dest[MLAN_MAC_ADDR_LENGTH];
    /** Source MAC address */
    char src[MLAN_MAC_ADDR_LENGTH];
    /** BSSID */
    char bssid[MLAN_MAC_ADDR_LENGTH];
	/** Fragment number of frame fragments */
    uint16_t seq_frag_num;
    /** Beacon timestamp */
    uint8_t timestamp[8];
	/** Beacon interval, Send periodically at Beacon time intervals, typically abbreviated as TU, representing 1024 microseconds */
    uint16_t beacon_interval;
	/** capability info, when sending beacon signals, it is used to notify all parties of the performance of the network */
    uint16_t cap_info;
	/** element identification code */
    uint8_t ssid_element_id;
    /** SSID Length */
    uint8_t ssid_len;
    /** SSID string */
    char ssid[MLAN_MAX_SSID_LENGTH];
} PACK_END wifi_beacon_info_t;

/** Wifi data information */
typedef PACK_START struct
{
    /** Frame control flags */
    uint8_t frame_ctrl_flags;
	/** data time interval */
    uint16_t duration;
	/** basic service set identifier */
    char bssid[MLAN_MAC_ADDR_LENGTH];
    /** Source MAC address */
    char src[MLAN_MAC_ADDR_LENGTH];
    /** Destination MAC address */
    char dest[MLAN_MAC_ADDR_LENGTH];
	/** sequence frage number */
    uint16_t seq_frag_num;
    /** QOS control */
    uint16_t qos_ctrl;
} PACK_END wifi_data_info_t;
#endif
/** Wifi frame types */
typedef enum
{
    /** Assoc request frame */
    ASSOC_REQ_FRAME = 0x00,
    /** Assoc response frame */
    ASSOC_RESP_FRAME = 0x10,
    /** ReAssoc request frame */
    REASSOC_REQ_FRAME = 0x20,
    /** ReAssoc response frame */
    REASSOC_RESP_FRAME = 0x30,
    /** Probe request frame */
    PROBE_REQ_FRAME = 0x40,
    /** Probe response frame */
    PROBE_RESP_FRAME = 0x50,
    /** BEACON frame */
    BEACON_FRAME = 0x80,
    /** Dis assoc frame */
    DISASSOC_FRAME = 0xA0,
    /** Auth frame */
    AUTH_FRAME = 0xB0,
    /** Deauth frame */
    DEAUTH_FRAME = 0xC0,
    /** Action frame */
    ACTION_FRAME = 0xD0,
    /** Data frame */
    DATA_FRAME = 0x08,
    /** QOS frame */
    QOS_DATA_FRAME = 0x88,
} wifi_frame_type_t;

typedef PACK_START struct
{
    wifi_frame_type_t frame_type;
#if CONFIG_NET_MONITOR
    union
    {
        wifi_beacon_info_t beacon_info;
        wifi_data_info_t data_info;
    } frame_data;
#endif
} PACK_END wifi_frame_t;

typedef struct
{
    uint8_t mfpc;
    uint8_t mfpr;
} wifi_pmf_params_t;

/** Channel scan parameters */
typedef struct
{
    /** channel number */
    t_u8 chan_number;
    /** minimum scan time */
    t_u16 min_scan_time;
    /** maximum scan time */
    t_u16 max_scan_time;
} wifi_chan_scan_param_set_t;

/** Channel list parameter set */
typedef struct
{
    /** number of channels */
    t_u8 no_of_channels;
    /** channel scan array */
    wifi_chan_scan_param_set_t chan_scan_param[1];
} wifi_chan_list_param_set_t;

/** 802_11_header packet */
typedef PACK_START struct _wifi_mgmt_frame_t
{
    /** Packet Length */
    t_u16 frm_len;
    /** Frame Type */
    wifi_frame_type_t frame_type;
    /** Frame Control flags */
    t_u8 frame_ctrl_flags;
    /** Duration ID */
    t_u16 duration_id;
    /** Address 1 */
    t_u8 addr1[MLAN_MAC_ADDR_LENGTH];
    /** Address 2 */
    t_u8 addr2[MLAN_MAC_ADDR_LENGTH];
    /** Address 3 */
    t_u8 addr3[MLAN_MAC_ADDR_LENGTH];
    /** Sequence Control */
    t_u16 seq_ctl;
    /** Address 4 */
    t_u8 addr4[MLAN_MAC_ADDR_LENGTH];
    /** Frame payload */
    t_u8 payload[1];
} PACK_END wifi_mgmt_frame_t;

/** Calibration Data */
typedef PACK_START struct _wifi_cal_data_t
{
    /** Calibration data length */
    t_u16 data_len;
    /** Calibration data */
    t_u8 *data;
} PACK_END wifi_cal_data_t;

/** Auto reconnect structure */
typedef PACK_START struct _wifi_auto_reconnect_config_t
{
    /** Reconnect counter */
    t_u8 reconnect_counter;
    /** Reconnect interval */
    t_u8 reconnect_interval;
    /** Flags */
    t_u16 flags;
} PACK_END wifi_auto_reconnect_config_t;

/** Scan all the channels in specified band */
#define BAND_SPECIFIED 0x80U

/** Scan channel list */
typedef PACK_START struct _wifi_scan_channel_list_t
{
    /** Channel scan parameter : Radio type */
    t_u8 radio_type;
    /** Channel numder */
    t_u8 chan_number;
    /** Scan type Active = 1, Passive = 2 */
    mlan_scan_type scan_type;
    /** Scan time */
    t_u16 scan_time;
} PACK_END wifi_scan_channel_list_t;

/* Configuration for wireless scanning */
#if defined(RW610) && (CONFIG_ANT_DETECT)
#define ANT_DETECT_MAX_CHANNEL_LIST 50U
#endif
#define MAX_CHANNEL_LIST 6
#if CONFIG_COMBO_SCAN
#define MAX_NUM_SSID 2
#endif
/** V2 scan parameters */
typedef PACK_START struct _wifi_scan_params_v2_t
{
#if CONFIG_WPA_SUPP
    /** Scan Only */
    t_u8 scan_only;
    /** BSSID present */
    t_u8 is_bssid;
    /** SSID present */
    t_u8 is_ssid;
#endif
    /** BSSID to scan */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** SSID to scan */
#if CONFIG_COMBO_SCAN
    char ssid[MAX_NUM_SSID][MLAN_MAX_SSID_LENGTH + 1];
#else
    char ssid[MLAN_MAX_SSID_LENGTH + 1];
#endif
    /** Number of channels */
    t_u8 num_channels;
    /** Channel list with channel information */
#if defined(RW610) && (CONFIG_ANT_DETECT)
    wifi_scan_channel_list_t chan_list[ANT_DETECT_MAX_CHANNEL_LIST];
#else
    wifi_scan_channel_list_t chan_list[MAX_CHANNEL_LIST];
#endif
    /** Number of probes */
    t_u8 num_probes;
#if CONFIG_SCAN_WITH_RSSIFILTER
    /** Threshold of rssi */
    t_s16 rssi_threshold;
#endif
#if CONFIG_SCAN_CHANNEL_GAP
    /** scan channel gap */
    t_u16 scan_chan_gap;
#endif
    /** Callback to be called when scan is completed */
    int (*cb)(unsigned int count);
} PACK_END wifi_scan_params_v2_t;

#if CONFIG_RF_TEST_MODE
/** Configuration for Manufacturing generic command */
typedef PACK_START struct _wifi_mfg_cmd_generic_cfg
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
} PACK_END wifi_mfg_cmd_generic_cfg_t;

/** Configuration for Manufacturing command Tx Frame */
typedef PACK_START struct _wifi_mfg_cmd_tx_frame
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
} PACK_END wifi_mfg_cmd_tx_frame_t;

/** Configuration for Manufacturing command Tx Continuous */
typedef PACK_START struct _wifi_mfg_cmd_tx_cont
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
} PACK_END wifi_mfg_cmd_tx_cont_t;

typedef PACK_START struct wifi_mfg_cmd_he_tb_tx
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
} PACK_END wifi_mfg_cmd_he_tb_tx_t;

typedef PACK_START struct wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr
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
} PACK_END wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr_t;

typedef PACK_START struct wifi_mfg_cmd_otp_mac_addr_rd_wr
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
} PACK_END wifi_mfg_cmd_otp_mac_addr_rd_wr_t;

typedef PACK_START struct wifi_mfg_cmd_otp_cal_data_rd_wr
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
} PACK_END wifi_mfg_cmd_otp_cal_data_rd_wr_t;
#endif

#if CONFIG_HEAP_DEBUG
#define MAX_FUNC_SYMBOL_LEN    64
#define OS_MEM_STAT_TABLE_SIZE 128

typedef struct
{
    char name[MAX_FUNC_SYMBOL_LEN];
    t_u32 size;
    t_u32 line_num;

    t_u32 alloc_cnt;
    t_u32 free_cnt;
} wifi_os_mem_info;
#endif

#if CONFIG_MULTI_CHAN
typedef PACK_START struct
{
    /** Channel Index*/
    t_u16 chan_idx;
    /** Channel time (in TU) for chan_idx */
    t_u8 chantime;
    /** Channel switch time (in TU) for chan_idx */
    t_u8 switchtime;
    /** Undoze time (in TU) for chan_idx */
    t_u8 undozetime;
    /** Rx traffic control scheme when channel switch*/
    /** only valid for GC/STA interface*/
    t_u8 mode;
} PACK_END wifi_drcs_cfg_t;
#endif


#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
/** Type definition of mlan_ds_subscribe_evt for subscribe events */
typedef struct _wifi_ds_subscribe_evt
{
    /** bitmap for subscribe event */
    t_u16 evt_bitmap;
    /** Absolute value of RSSI threshold value (dBm) */
    t_u8 low_rssi;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 low_rssi_freq;
    /** SNR threshold value (dB) */
    t_u8 low_snr;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 low_snr_freq;
    /** Failure count threshold */
    t_u8 failure_count;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 failure_count_freq;
    /** num of missed beacons */
    t_u8 beacon_miss;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 beacon_miss_freq;
    /** Absolute value of RSSI threshold value (dBm) */
    t_u8 high_rssi;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 high_rssi_freq;
    /** SNR threshold value (dB) */
    t_u8 high_snr;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 high_snr_freq;
    /** Absolute value of data RSSI threshold value (dBm) */
    t_u8 data_low_rssi;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 data_low_rssi_freq;
    /** Absolute value of data SNR threshold value (dBm) */
    t_u8 data_low_snr;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 data_low_snr_freq;
    /** Absolute value of data RSSI threshold value (dBm) */
    t_u8 data_high_rssi;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
    t_u8 data_high_rssi_freq;
    /** Absolute value of data SNR threshold value (dBm) */
    t_u8 data_high_snr;
    /** 0--report once, 1--report everytime happend, N -- report only happend > N consecutive times */
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
} wifi_ds_subscribe_evt;
#endif

#if CONFIG_CSI
#define CSI_FILTER_MAX 16
/** Structure of CSI filters */
typedef PACK_START struct _wifi_csi_filter_t
{
    /** Source address of the packet to receive */
    t_u8 mac_addr[MLAN_MAC_ADDR_LENGTH];
    /** Pakcet type of the interested CSI */
    t_u8 pkt_type;
    /** Packet subtype of the interested CSI */
    t_u8 subtype;
    /** Other filter flags */
    t_u8 flags;
} PACK_END wifi_csi_filter_t;
/** Structure of CSI parameters */
typedef PACK_START struct _wifi_csi_config_params_t
{
    /** 0: station; 1: uap */
	t_u8 bss_type;
    /** CSI enable flag. 1: enable, 2: disable */
    t_u16 csi_enable;
    /** Header ID*/
    t_u32 head_id;
    /** Tail ID */
    t_u32 tail_id;
    /** Number of CSI filters */
    t_u8 csi_filter_cnt;
    /** Chip ID */
    t_u8 chip_id;
    /** band config */
    t_u8 band_config;
    /** Channel num */
    t_u8 channel;
    /** Enable getting CSI data on special channel */
    t_u8 csi_monitor_enable;
    /** CSI data received in cfg channel with mac addr filter, not only RA is us or other*/
    t_u8 ra4us;
    /** CSI filters */
    wifi_csi_filter_t csi_filter[CSI_FILTER_MAX];
} PACK_END wifi_csi_config_params_t;
#endif /* CSI_SUPPORT */

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
/** Wi-Fi independent reset config */
typedef PACK_START struct
{
    /** reset mode enable/ disable */
    t_u8 ir_mode;
    /** gpio pin */
    t_u8 gpio_pin;
} PACK_END wifi_indrst_cfg_t;
#endif

#if CONFIG_INACTIVITY_TIMEOUT_EXT
/** Type definition of wifi_inactivity_to
 *  for MLAN_OID_PM_CFG_INACTIVITY_TO
 */
typedef PACK_START struct
{
    /** Timeout unit in microsecond, 0 means 1000us (1ms) */
    t_u32 timeout_unit;
    /** Inactivity timeout for unicast data */
    t_u32 unicast_timeout;
    /** Inactivity timeout for multicast data */
    t_u32 mcast_timeout;
    /** Timeout for additional Rx traffic after Null PM1 packet exchange */
    t_u32 ps_entry_timeout;
    /** Inactivity timeout for cmd */
    t_u32 ps_cmd_timeout;
} PACK_END wifi_inactivity_to_t;
#endif

#if CONFIG_AUTO_NULL_TX
/** auto null tx information */
typedef struct
{
    /** 1-start 0-stop */
    t_u8 start;
    /** bit15:14 unit: 00-s 01-us 10-ms 11-one_shot  bit13-0: interval */
    t_u16 interval;
    /** bit7-4: bandwidth. bit3-0: priority, ignored if non-WMM */
    t_u8 priority;
    /** Destination MAC address */
    t_u8 dst_mac[MLAN_MAC_ADDR_LENGTH];
    /** Source MAC address */
    t_u8 src_mac[MLAN_MAC_ADDR_LENGTH];
} wifi_auto_null_tx_t;
#endif

#if defined(RW610) && (CONFIG_ANT_DETECT)
#define NORMAL_DETECT_MODE                 0
#define QUICK_DETECT_MODE                  1
#define PCB_DETECT_MODE                    2
#define PCB_DETECT_MODE_CHECK_DEVICE_COUNT 2
#define ANT_DETECT_MAX_SCAN_ENTRY          5
#define MAX_ANTENNA_PORT_NUM               4
typedef PACK_START struct _scan_result_entry_t
{
    char ssid[33];
    unsigned int ssid_len;
    char bssid[6];
    unsigned int channel;
    unsigned char rssi;
} PACK_END scan_result_entry_t;

typedef PACK_START struct _wlan_ant_info_t
{
    uint8_t scan_done;
    unsigned char avg_rssi;
    uint8_t entry_idx;
    scan_result_entry_t scan_entry[ANT_DETECT_MAX_SCAN_ENTRY];
} PACK_END wlan_ant_scan_info_t;

typedef PACK_START struct _cfg_scan_channel_list_t
{
    uint8_t num_channels;
    uint8_t chan_number[ANT_DETECT_MAX_CHANNEL_LIST];
} PACK_END cfg_scan_channel_list_t;

typedef PACK_START struct _wlan_ant_detect_data_t
{
    uint8_t detect_mode;
    int detect_done;
    uint16_t current_ant;
    uint8_t ant_port_count;
    uint16_t best_ant;
    uint16_t next_best_ant;
    cfg_scan_channel_list_t *channel_list;
    wlan_ant_scan_info_t scan_info[MAX_ANTENNA_PORT_NUM];
} PACK_END wlan_ant_detect_data_t;
#endif

#endif /* __WIFI_DECL_H__ */
