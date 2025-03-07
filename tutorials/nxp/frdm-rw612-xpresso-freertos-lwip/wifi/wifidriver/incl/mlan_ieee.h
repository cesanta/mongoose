/** @file mlan_ieee.h
 *
 *  @brief This file contains IEEE information element related
 *  definitions used in MLAN and MOAL module.
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    11/03/2008: initial version
******************************************************/

#ifndef _MLAN_IEEE_H_
#define _MLAN_IEEE_H_

/** FIX IES size in beacon buffer */
#define WLAN_802_11_FIXED_IE_SIZE 12
/** WLAN supported rates */
#define WLAN_SUPPORTED_RATES 14U

/** WLAN supported rates extension*/
#define WLAN_SUPPORTED_RATES_EXT 60U

enum wifi_nxp_wpa_versions
{
    WIFI_NXP_WPA_VERSION_1 = 1 << 0,
    WIFI_NXP_WPA_VERSION_2 = 1 << 1,
};

/** Enumeration definition*/
/** WLAN_802_11_NETWORK_TYPE */
typedef enum _WLAN_802_11_NETWORK_TYPE
{
    Wlan802_11FH,
    Wlan802_11DS,
    /* Defined as upper bound */
    Wlan802_11NetworkTypeMax
} WLAN_802_11_NETWORK_TYPE;

/** Maximum size of IEEE Information Elements */
#define IEEE_MAX_IE_SIZE 500

/** Frame control: Type Mgmt frame */
#define IEEE80211_FC_MGMT_FRAME_TYPE_MASK       0x000C
/** Frame control: SubType Mgmt frame */
#define IEEE80211_GET_FC_MGMT_FRAME_SUBTYPE(fc) (((fc)&0x00F0U) >> 4)

#ifdef PRAGMA_PACK
#pragma pack(push, 1)
#endif

/*
*****************************************************************************
**
**
**                         802.11 Message Types
**
**
*****************************************************************************
*/
typedef enum
{
    IEEE_TYPE_MANAGEMENT = 0,
    IEEE_TYPE_CONTROL,
    IEEE_TYPE_DATA
} IEEEtypes_MsgType_e;

#if CONFIG_11AX
typedef enum _IEEEtypes_Ext_ElementId_e
{
    HE_CAPABILITY = 35,
    HE_OPERATION  = 36
} IEEEtypes_Ext_ElementId_e;
#endif

/** IEEE Type definitions  */
typedef MLAN_PACK_START enum _IEEEtypes_ElementId_e {
    SSID            = 0,
    SUPPORTED_RATES = 1,

    FH_PARAM_SET   = 2,
    DS_PARAM_SET   = 3,
    CF_PARAM_SET   = 4,
    DTIM_PARAM_SET = 5,
    IBSS_PARAM_SET = 6,

#ifdef STA_SUPPORT
    COUNTRY_INFO = 7,
#endif /* STA_SUPPORT */

    POWER_CONSTRAINT   = 32,
    POWER_CAPABILITY   = 33,
    TPC_REQUEST        = 34,
    TPC_REPORT         = 35,
    SUPPORTED_CHANNELS = 36,
    CHANNEL_SWITCH_ANN = 37,
#if CONFIG_11K
    MEASURE_REQUEST = 38,
    MEASURE_REPORT  = 39,
#endif
    QUIET         = 40,
    IBSS_DFS      = 41,
    HT_CAPABILITY = 45,
    QOS_INFO      = 46,

#if (CONFIG_11K) || (CONFIG_11V)
    NEIGHBOR_REPORT = 52,
#endif
#if (CONFIG_11R) || (CONFIG_11K)
    /*IEEE802.11r*/
    MOBILITY_DOMAIN     = 54,
    FAST_BSS_TRANSITION = 55,
    TIMEOUT_INTERVAL    = 56,
    RIC                 = 57,
#endif

    REGULATORY_CLASS = 59,
#if CONFIG_ECSA
    EXTEND_CHANNEL_SWITCH_ANN = 60,
#endif
    HT_OPERATION = 61,
#if CONFIG_11K
    RRM_ENABLED_CAP = 70,
#endif
#if CONFIG_MULTI_BSSID_SUPPORT
    MULTI_BSSID = 71,
#endif
    BSSCO_2040          = 72,
    OVERLAPBSSSCANPARAM = 74,
#if CONFIG_MULTI_BSSID_SUPPORT
    NONTX_BSSID_CAP = 83,
#endif

#if CONFIG_MULTI_BSSID_SUPPORT
    MBSSID_INDEX = 85,
#endif
    EXT_CAPABILITY = 127,

    /* ERP_INFO = 42, */

    EXTENDED_SUPPORTED_RATES = 50,

    VHT_CAPABILITY = 191,
    VHT_OPERATION  = 192,
/*EXT_BSS_LOAD        = 193,*/
#if CONFIG_ECSA
    BW_CHANNEL_SWITCH = 194,
#endif
    VHT_TX_POWER_ENV = 195,
#if CONFIG_ECSA
    EXT_POWER_CONSTR = 196,
#endif
    /*AID_INFO            = 197,
    QUIET_CHAN          = 198,*/
    OPER_MODE_NTF = 199,

    VENDOR_SPECIFIC_221 = 221,
    WMM_IE              = VENDOR_SPECIFIC_221,

    WPS_IE = VENDOR_SPECIFIC_221,

    WPA_IE    = VENDOR_SPECIFIC_221,
    RSN_IE    = 48,
    VS_IE     = VENDOR_SPECIFIC_221,
    WAPI_IE   = 68,
    FRAGMENT  = 242,
    RSNX_IE   = 244,
    EXTENSION = 255,

} MLAN_PACK_END IEEEtypes_ElementId_e;

/** IEEE IE header */
typedef MLAN_PACK_START struct _IEEEtypes_Header_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
} MLAN_PACK_END IEEEtypes_Header_t, *pIEEEtypes_Header_t;

/** Vendor specific IE header */
typedef MLAN_PACK_START struct _IEEEtypes_VendorHeader_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** OUI */
    t_u8 oui[3];
    /** OUI type */
    t_u8 oui_type;
    /** OUI subtype */
    t_u8 oui_subtype;
    /** Version */
    t_u8 version;
} MLAN_PACK_END IEEEtypes_VendorHeader_t, *pIEEEtypes_VendorHeader_t;

/** Vendor specific IE */
typedef MLAN_PACK_START struct _IEEEtypes_VendorSpecific_t
{
    /** Vendor specific IE header */
    IEEEtypes_VendorHeader_t vend_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_VendorHeader_t)];
} MLAN_PACK_END IEEEtypes_VendorSpecific_t, *pIEEEtypes_VendorSpecific_t;

/** IEEE IE */
typedef MLAN_PACK_START struct _IEEEtypes_Generic_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** IE Max - size of previous fields */
    t_u8 data[IEEE_MAX_IE_SIZE - sizeof(IEEEtypes_Header_t)];
} MLAN_PACK_END IEEEtypes_Generic_t, *pIEEEtypes_Generic_t;

#ifdef ENABLE_802_11R
/**ft capability policy*/
typedef MLAN_PACK_START struct _IEEEtypes_FtCapPolicy_t
{
    /** FT over the DS capable */
    t_u8 ft_over_ds : 1;
    /** RIC support */
    t_u8 ric : 1;
    /** Reserved */
    t_u8 reserved : 6;
} MLAN_PACK_END IEEEtypes_FtCapPolicy_t;

/** Mobility domain IE */
typedef MLAN_PACK_START struct _IEEEtypes_MobilityDomain_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Mobility Domain ID */
    t_u16 mdid;
    /** FT Capability policy */
    t_u8 ft_cap;
} MLAN_PACK_END IEEEtypes_MobilityDomain_t;

/**FT MIC Control*/
typedef MLAN_PACK_START struct _IEEEtypes_FT_MICControl_t
{
    /** reserved */
    t_u8 reserved;
    /** element count */
    t_u8 element_count;
} MLAN_PACK_END IEEEtypes_FT_MICControl_t;

/** FTIE MIC LEN */
#define FTIE_MIC_LEN 16

/**FT IE*/
typedef MLAN_PACK_START struct _IEEEtypes_FastBssTransElement_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** mic control */
    IEEEtypes_FT_MICControl_t mic_control;
    /** mic */
    t_u8 mic[FTIE_MIC_LEN];
    /** ANonce */
    t_u8 a_nonce[32];
    /** SNonce */
    t_u8 s_nonce[32];
    /** sub element */
    t_u8 sub_element[1];
} MLAN_PACK_END IEEEtypes_FastBssTransElement_t;

/*Category for FT*/
#define FT_CATEGORY 6
/** FT ACTION request */
#define FT_ACTION_REQUEST 1
/** FT ACTION response */
#define FT_ACTION_RESPONSE 2

/*FT response and FT ack*/
typedef MLAN_PACK_START struct
{
    /** category */
    t_u8 category;
    /** action */
    t_u8 action;
    /** sta address */
    t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
    /** target ap address */
    t_u8 target_ap_addr[MLAN_MAC_ADDR_LENGTH];
    /** status code */
    t_u16 status_code;
    /** varible */
    t_u8 variable[];
} MLAN_PACK_END IEEEtypes_Ft_action_response;

/**FT request */
typedef MLAN_PACK_START struct
{
    /** category */
    t_u8 category;
    /** action */
    t_u8 action;
    /** sta address */
    t_u8 sta_addr[MLAN_MAC_ADDR_LENGTH];
    /** target ap address */
    t_u8 target_ap_addr[MLAN_MAC_ADDR_LENGTH];
    /** varible */
    t_u8 variable[];
} MLAN_PACK_END IEEEtypes_Ft_action_request;
#endif

/** auth frame body*/
typedef MLAN_PACK_START struct
{
    /** auth alg */
    t_u16 auth_alg;
    /** auth transaction */
    t_u16 auth_transaction;
    /** status code */
    t_u16 status_code;
    /** variable */
    t_u8 variable[];
} MLAN_PACK_END IEEEtypes_Auth_framebody;

/** associate request frame */
typedef MLAN_PACK_START struct
{
    t_u16 capab_info;
    t_u16 listen_interval;
    /** followed by SSID and Supported rates */
    t_u8 variablep[];
} MLAN_PACK_END IEEEtypes_assoc_req;

/** deauth request frame */
typedef MLAN_PACK_START struct
{
    t_u16 reason_code;
    t_u8 variablep[];
} MLAN_PACK_END IEEEtypes_deauth_req;

/*Mgmt frame*/
typedef MLAN_PACK_START struct
{
    /** frame control */
    t_u16 frame_control;
    /** duration */
    t_u16 duration;
    /** dest address */
    t_u8 da[MLAN_MAC_ADDR_LENGTH];
    /** source address */
    t_u8 sa[MLAN_MAC_ADDR_LENGTH];
    /** bssid */
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    /** seq control */
    t_u16 seq_ctrl;
    union
    {
        IEEEtypes_Auth_framebody auth;
        IEEEtypes_assoc_req assoc_req;
        IEEEtypes_deauth_req deauth_req;
#ifdef ENABLE_802_11R
        IEEEtypes_Ft_action_response ft_resp;
        IEEEtypes_Ft_action_request ft_req;
#endif
    } u;
} MLAN_PACK_END IEEE80211_MGMT;

/** Capability information mask */
#define CAPINFO_MASK (~(MBIT(15) | MBIT(14) | MBIT(11) | MBIT(9)))

/** Capability Bit Map*/
typedef MLAN_PACK_START struct _IEEEtypes_CapInfo_t
{
    /** Capability Bit Map : ESS */
    t_u8 ess : 1;
    /** Capability Bit Map : IBSS */
    t_u8 ibss : 1;
    /** Capability Bit Map : CF pollable */
    t_u8 cf_pollable : 1;
    /** Capability Bit Map : CF poll request */
    t_u8 cf_poll_rqst : 1;
    /** Capability Bit Map : privacy */
    t_u8 privacy : 1;
    /** Capability Bit Map : Short preamble */
    t_u8 short_preamble : 1;
    /** Capability Bit Map : PBCC */
    t_u8 pbcc : 1;
    /** Capability Bit Map : Channel agility */
    t_u8 chan_agility : 1;
    /** Capability Bit Map : Spectrum management */
    t_u8 spectrum_mgmt : 1;
    /** Capability Bit Map : Qos */
    t_u8 qos : 1;
    /** Capability Bit Map : Short slot time */
    t_u8 short_slot_time : 1;
    /** Capability Bit Map : APSD */
    t_u8 Apsd : 1;
    /** Capability Bit Map : Radio Rsrc Measurement */
    t_u8 radio_measurement : 1;
    /** Capability Bit Map : DSS OFDM */
    t_u8 dsss_ofdm : 1;
    /** Capability Bit Map : Delayed Block Ack */
    t_u8 delayed_block_ack : 1;
    /** Capability Bit Map : Immediate Block Ack */
    t_u8 immediate_block_ack : 1;
} MLAN_PACK_END IEEEtypes_CapInfo_t, *pIEEEtypes_CapInfo_t;

#if CONFIG_MULTI_BSSID_SUPPORT
/** IEEEtypes_Ssid_t */
typedef MLAN_PACK_START struct _IEEEtypes_Ssid_t
{
    /** SSID: Element ID */
    t_u8 element_id;
    /** SSID : Length */
    t_u8 len;
    /** ssid */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH];
} MLAN_PACK_END IEEEtypes_Ssid_t, *pIEEEtypes_Ssid_t;
#endif

/** IEEEtypes_CfParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_CfParamSet_t
{
    /** CF peremeter : Element ID */
    IEEEtypes_ElementId_e element_id;
    /** CF peremeter : Length */
    t_u8 len;
    /** CF peremeter : Count */
    t_u8 cfp_cnt;
    /** CF peremeter : Period */
    t_u8 cfp_period;
    /** CF peremeter : Maximum duration */
    t_u16 cfp_max_duration;
    /** CF peremeter : Remaining duration */
    t_u16 cfp_duration_remaining;
} MLAN_PACK_END IEEEtypes_CfParamSet_t, *pIEEEtypes_CfParamSet_t;

/** IEEEtypes_IbssParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_IbssParamSet_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** ATIM window value in milliseconds */
    t_u16 atim_window;
} MLAN_PACK_END IEEEtypes_IbssParamSet_t, *pIEEEtypes_IbssParamSet_t;

/** IEEEtypes_SsParamSet_t */
typedef MLAN_PACK_START union _IEEEtypes_SsParamSet_t
{
    /** SS parameter : CF parameter set */
    IEEEtypes_CfParamSet_t cf_param_set;
    /** SS parameter : IBSS parameter set */
    IEEEtypes_IbssParamSet_t ibss_param_set;
} MLAN_PACK_END IEEEtypes_SsParamSet_t, *pIEEEtypes_SsParamSet_t;

/** IEEEtypes_FhParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_FhParamSet_t
{
    /** FH parameter : Element ID */
    IEEEtypes_ElementId_e element_id;
    /** FH parameter : Length */
    t_u8 len;
    /** FH parameter : Dwell time in milliseconds */
    t_u16 dwell_time;
    /** FH parameter : Hop set */
    t_u8 hop_set;
    /** FH parameter : Hop pattern */
    t_u8 hop_pattern;
    /** FH parameter : Hop index */
    t_u8 hop_index;
} MLAN_PACK_END IEEEtypes_FhParamSet_t, *pIEEEtypes_FhParamSet_t;

/** IEEEtypes_DsParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_DsParamSet_t
{
    /** DS parameter : Element ID */
    IEEEtypes_ElementId_e element_id;
    /** DS parameter : Length */
    t_u8 len;
    /** DS parameter : Current channel */
    t_u8 current_chan;
} MLAN_PACK_END IEEEtypes_DsParamSet_t, *pIEEEtypes_DsParamSet_t;

/** IEEEtypes_DTIMParamSet_t */
typedef MLAN_PACK_START struct _IEEEtypes_DtimParamSet_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** DTIM Count */
    t_u8 dtim_count;
    /** DTIM Period */
    t_u8 dtim_period;
    /** Bitmap Control */
    t_u8 bitmap_control;
    /** Partial Virtual Bitmap */
    t_u8 partial_virtual_bitmap;
} MLAN_PACK_END IEEEtypes_DtimParamSet_t, *pIEEEtypes_DtimParamSet_t;

/** IEEEtypes_PhyParamSet_t */
typedef MLAN_PACK_START union _IEEEtypes_PhyParamSet_t
{
    /** FH parameter set */
    IEEEtypes_FhParamSet_t fh_param_set;
    /** DS parameter set */
    IEEEtypes_DsParamSet_t ds_param_set;
} MLAN_PACK_END IEEEtypes_PhyParamSet_t, *pIEEEtypes_PhyParamSet_t;

/** IEEEtypes_ERPInfo_t */
typedef MLAN_PACK_START struct _IEEEtypes_ERPInfo_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** ERP flags */
    t_u8 erp_flags;
} MLAN_PACK_END IEEEtypes_ERPInfo_t, *pIEEEtypes_ERPInfo_t;

/** IEEEtypes_AId_t */
typedef t_u16 IEEEtypes_AId_t;

/** IEEEtypes_StatusCode_t */
typedef t_u16 IEEEtypes_StatusCode_t;

/** IEEEtypes_StatusCode_e */
typedef MLAN_PACK_START enum _IEEEtypes_StatusCode_e
{
    IEEE_STATUS_CODE_SUCCESS                   = 0,
    IEEE_STATUS_CODE_REASON_UNSPECIFIED        = 1,
    IEEE_STATUS_CODE_DENIED_OTHER_REASON       = 12,
    IEEE_STATUS_CODE_POOR_CHANNEL_CONDITIONS   = 34,
    IEEE_STATUS_CODE_REQUEST_DECLINED          = 37,
    IEEE_STATUS_CODE_INVALID_PARAMETERS        = 38,
    IEEE_STATUS_CODE_INVALID_RSNE_CAPABILITIES = 45,
    IEEE_STATUS_CODE_TRANSMISSION_FAILURE      = 79,
}MLAN_PACK_END IEEEtypes_StatusCode_e;

#if CONFIG_HOST_MLME
/** IEEEtypes_SeqCtl_t */
typedef MLAN_PACK_START struct _IEEEtypes_SeqCtl_t
{
    /** Fragment Number */
    t_u16 FragNum : 4;
    /** Sequence Number */
    t_u16 SeqNum : 12;
} MLAN_PACK_END IEEEtypes_SeqCtl_t;

/** IEEEtypes_MgmtHdr_t */
typedef MLAN_PACK_START struct _IEEEtypes_MgmtHdr_t
{
    /** FrmCtl*/
    t_u16 FrmCtl;
    /** Duration*/
    t_u16 Duration;
    /** Destination Addr*/
    t_u8 DestAddr[6];
    /** Source Addr*/
    t_u8 SrcAddr[6];
    /** BSSID */
    t_u8 BssId[6];
    /** IEEEtypes_SeqCtl_t */
    IEEEtypes_SeqCtl_t SeqCtl;
} MLAN_PACK_END IEEEtypes_MgmtHdr_t;
#endif

/** IEEEtypes_AssocRsp_t */
typedef MLAN_PACK_START struct _IEEEtypes_AssocRsp_t
{
    /** Capability information */
    IEEEtypes_CapInfo_t capability;
    /** Association response status code */
    IEEEtypes_StatusCode_t status_code;
    /** Association ID */
    IEEEtypes_AId_t a_id;
    /** IE data buffer */
    t_u8 ie_buffer[1];
} MLAN_PACK_END IEEEtypes_AssocRsp_t, *pIEEEtypes_AssocRsp_t;

/** 802.11 supported rates */
typedef t_u8 WLAN_802_11_RATES[WLAN_SUPPORTED_RATES];

/** cipher TKIP */
#define WPA_CIPHER_TKIP2 2
/** cipher AES CCMP 128 */
#define WPA_CIPHER_AES_CCM 4
/** cipher AES GCMP 128 */
#define WPA_CIPHER_AES_GCM 8
/** cipher AES CCMP 256 */
#define WPA_CIPHER_AES_CCM_256 10
/** cipher AES GCMP 256 */
#define WPA_CIPHER_AES_GCM_256 9

/** AKM: 8021x */
#define RSN_AKM_8021X 1
/** AKM: FT 8021x */
#define RSN_AKM_FT_8021X 3
/** AKM: 8021x SUITE B*/
#define RSN_AKM_8021X_SUITEB 0xb
/** AKM: 8021x SUITE B 192*/
#define RSN_AKM_8021X_SUITEB_192 0xc
/** AKM: FT 8021x SHA384 */
#define RSN_AKM_FT_8021X_SHA384 0xd
/** AKM: PSK */
#define RSN_AKM_PSK 2
/** AKM: FT PSK */
#define RSN_AKM_FT_PSK 4
/** AKM: 8021x_SHA256 */
#define RSN_AKM_8021X_SHA256 5
/** AKM: PSK SHA256 */
#define RSN_AKM_PSK_SHA256 6

#if UAP_HOST_MLME
/** AKM: SAE SHA256 */
#define RSN_AKM_SAE 8
/** AKM: FT SAE SHA256 */
#define RSN_AKM_FT_SAE 9
/** AKM: OWE SHA256 */
#define RSN_AKM_OWE 18
#endif

#if defined(STA_SUPPORT) && defined(ENABLE_802_11W) && defined(EMBEDDED_SUPPLICANT)
/** Pairwise Cipher Suite length */
#define PAIRWISE_CIPHER_SUITE_LEN 4
/** AKM Suite length */
#define AKM_SUITE_LEN 4
/** MFPC bit in RSN capability */
#define MFPC_BIT 7
/** MFPR bit in RSN capability */
#define MFPR_BIT 6
/** PMF ORing mask */
#define PMF_MASK 0x00c0
#endif

/** wpa_suite_t */
typedef MLAN_PACK_START struct _wpa_suite_t
{
    /** OUI */
    t_u8 oui[3];
    /** tyep */
    t_u8 type;
} MLAN_PACK_END wpa_suite, wpa_suite_mcast_t;

/** wpa_suite_ucast_t */
typedef MLAN_PACK_START struct
{
    /* count */
    t_u16 count;
    /** wpa_suite list */
    wpa_suite list[1];
} MLAN_PACK_END wpa_suite_ucast_t, wpa_suite_auth_key_mgmt_t;

/** IEEEtypes_Rsn_t */
typedef MLAN_PACK_START struct _IEEEtypes_Rsn_t
{
    /** Rsn : Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Rsn : Length */
    t_u8 len;
    /** Rsn : version */
    t_u16 version;
    /** Rsn : group cipher */
    wpa_suite_mcast_t group_cipher;
    /** Rsn : pairwise cipher */
    wpa_suite_ucast_t pairwise_cipher;
    /** Rsn : AuthKey Mgmt */
    // wpa_suite_auth_key_mgmt_t auth_key_mgmt;
} MLAN_PACK_END IEEEtypes_Rsn_t, *pIEEEtypes_Rsn_t;

/** IEEEtypes_Rsnx_t */
typedef MLAN_PACK_START struct _IEEEtypes_Rsnx_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Rsnx : data */
    t_u8 data[1];
} MLAN_PACK_END IEEEtypes_Rsnx_t, *pIEEEtypes_Rsnx_t;

/** IEEEtypes_Wpa_t */
typedef MLAN_PACK_START struct _IEEEtypes_Wpa_t
{
    /** Wpa : Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Wpa : Length */
    t_u8 len;
    /** Wpa : oui */
    t_u8 oui[4];
    /** version */
    t_u16 version;
    /** Wpa : group cipher */
    wpa_suite_mcast_t group_cipher;
    /** Wpa : pairwise cipher */
    wpa_suite_ucast_t pairwise_cipher;
    /** Wpa : AuthKey Mgmt */
    // wpa_suite_auth_key_mgmt_t auth_key_mgmt;
} MLAN_PACK_END IEEEtypes_Wpa_t, *pIEEEtypes_Wpa_t;

/** Maximum number of AC QOS queues available in the driver/firmware */
#define MAX_AC_QUEUES 4

/** Data structure of WMM QoS information */
typedef MLAN_PACK_START struct _IEEEtypes_WmmQosInfo_t
{
    /** Parameter set count */
    t_u8 para_set_count : 4;
    /** Reserved */
    t_u8 reserved : 3;
    /** QoS UAPSD */
    t_u8 qos_uapsd : 1;
} MLAN_PACK_END IEEEtypes_WmmQosInfo_t, *pIEEEtypes_WmmQosInfo_t;

/** Data structure of WMM Aci/Aifsn */
typedef MLAN_PACK_START struct _IEEEtypes_WmmAciAifsn_t
{
    /** Aifsn */
    t_u8 aifsn : 4;
    /** Acm */
    t_u8 acm : 1;
    /** Aci */
    t_u8 aci : 2;
    /** Reserved */
    t_u8 reserved : 1;
} MLAN_PACK_END IEEEtypes_WmmAciAifsn_t, *pIEEEtypes_WmmAciAifsn_t;

/** Data structure of WMM ECW */
typedef MLAN_PACK_START struct _IEEEtypes_WmmEcw_t
{
    /** Minimum Ecw */
    t_u8 ecw_min : 4;
    /** Maximum Ecw */
    t_u8 ecw_max : 4;
} MLAN_PACK_END IEEEtypes_WmmEcw_t, *pIEEEtypes_WmmEcw_t;

/** Data structure of WMM AC parameters  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmAcParameters_t
{
    IEEEtypes_WmmAciAifsn_t aci_aifsn; /**< AciAifSn */
    IEEEtypes_WmmEcw_t ecw;            /**< Ecw */
    t_u16 tx_op_limit;                 /**< Tx op limit */
} MLAN_PACK_END IEEEtypes_WmmAcParameters_t, *pIEEEtypes_WmmAcParameters_t;

/** Data structure of WMM Info IE  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmInfo_t
{
    /**
     * WMM Info IE - Vendor Specific Header:
     *   element_id  [221/0xdd]
     *   Len         [7]
     *   Oui         [00:50:f2]
     *   OuiType     [2]
     *   OuiSubType  [0]
     *   Version     [1]
     */
    IEEEtypes_VendorHeader_t vend_hdr;

    /** QoS information */
    IEEEtypes_WmmQosInfo_t qos_info;

} MLAN_PACK_END IEEEtypes_WmmInfo_t, *pIEEEtypes_WmmInfo_t;

/** Data structure of WMM parameter IE  */
typedef MLAN_PACK_START struct _IEEEtypes_WmmParameter_t
{
    /**
     * WMM Parameter IE - Vendor Specific Header:
     *   element_id  [221/0xdd]
     *   Len         [24]
     *   Oui         [00:50:f2]
     *   OuiType     [2]
     *   OuiSubType  [1]
     *   Version     [1]
     */
    IEEEtypes_VendorHeader_t vend_hdr;

    /** QoS information */
    IEEEtypes_WmmQosInfo_t qos_info;
    /** Reserved */
    t_u8 reserved;

    /** AC Parameters Record WMM_AC_BE, WMM_AC_BK, WMM_AC_VI, WMM_AC_VO */
    IEEEtypes_WmmAcParameters_t ac_params[MAX_AC_QUEUES];
} MLAN_PACK_END IEEEtypes_WmmParameter_t, *pIEEEtypes_WmmParameter_t;

/** Enumerator for TSPEC direction */
typedef MLAN_PACK_START enum _IEEEtypes_WMM_TSPEC_TS_Info_Direction_e {

    TSPEC_DIR_UPLINK   = 0,
    TSPEC_DIR_DOWNLINK = 1,
    /* 2 is a reserved value */
    TSPEC_DIR_BIDIRECT = 3,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_Direction_e;

/** Enumerator for TSPEC PSB */
typedef MLAN_PACK_START enum _IEEEtypes_WMM_TSPEC_TS_Info_PSB_e {

    TSPEC_PSB_LEGACY = 0,
    TSPEC_PSB_TRIG   = 1,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_PSB_e;

/** Enumerator for TSPEC Ack Policy */
typedef MLAN_PACK_START enum _IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e {

    TSPEC_ACKPOLICY_NORMAL = 0,
    TSPEC_ACKPOLICY_NOACK  = 1,
    /* 2 is reserved */
    TSPEC_ACKPOLICY_BLOCKACK = 3,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e;

/** Enumerator for TSPEC Trafffice type */
typedef MLAN_PACK_START enum _IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e {

    TSPEC_TRAFFIC_APERIODIC = 0,
    TSPEC_TRAFFIC_PERIODIC  = 1,

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e;

/** Data structure of WMM TSPEC information */
typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_TSPEC_TS_TRAFFIC_TYPE_e TrafficType : 1;
    t_u8 TID : 4;                                            // ! Unique identifier
    IEEEtypes_WMM_TSPEC_TS_Info_Direction_e Direction : 2;
    t_u8 AccessPolicy1 : 1;                                  // !
    t_u8 AccessPolicy2 : 1;                                  // !
    t_u8 Aggregation : 1;                                    // ! Reserved
    IEEEtypes_WMM_TSPEC_TS_Info_PSB_e PowerSaveBehavior : 1; // !
                                                             // Legacy/Trigg
    t_u8 UserPri : 3;                                        // ! 802.1d User Priority
    IEEEtypes_WMM_TSPEC_TS_Info_AckPolicy_e AckPolicy : 2;
    t_u8 Schedule : 1;
    t_u8 Reserved17_23 : 7; // ! Reserved
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_TS_Info_t;

/** Data structure of WMM TSPEC Nominal Size */
typedef MLAN_PACK_START struct
{
    t_u16 Size : 15;        // ! Nominal size in octets
    t_u16 Fixed : 1;        // ! 1: Fixed size given in Size, 0: Var, size
                            // is nominal
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_NomMSDUSize_t;

/** Data structure of WMM TSPEC SBWA */
typedef MLAN_PACK_START struct
{
    t_u16 Fractional : 13;  // ! Fractional portion
    t_u16 Whole : 3;        // ! Whole portion
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_SBWA;

/** Data structure of WMM TSPEC Body */
typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_TSPEC_TS_Info_t TSInfo;
    IEEEtypes_WMM_TSPEC_NomMSDUSize_t NomMSDUSize;
    t_u16 MaximumMSDUSize;
    t_u32 MinServiceInterval;
    t_u32 MaxServiceInterval;
    t_u32 InactivityInterval;
    t_u32 SuspensionInterval;
    t_u32 ServiceStartTime;
    t_u32 MinimumDataRate;
    t_u32 MeanDataRate;
    t_u32 PeakDataRate;
    t_u32 MaxBurstSize;
    t_u32 DelayBound;
    t_u32 MinPHYRate;
    IEEEtypes_WMM_TSPEC_SBWA SurplusBWAllowance;
    t_u16 MediumTime;
} MLAN_PACK_END IEEEtypes_WMM_TSPEC_Body_t;

/** Data structure of WMM TSPEC all elements */
typedef MLAN_PACK_START struct
{
    t_u8 ElementId;
    t_u8 Len;
    t_u8 OuiType[4]; /* 00:50:f2:02 */
    t_u8 OuiSubType; /* 01 */
    t_u8 Version;

    IEEEtypes_WMM_TSPEC_Body_t TspecBody;

} MLAN_PACK_END IEEEtypes_WMM_TSPEC_t;

/** WMM Action Category values */
typedef MLAN_PACK_START enum _IEEEtypes_ActionCategory_e {
    IEEE_MGMT_ACTION_CATEGORY_SPECTRUM_MGMT  = 0,
    IEEE_MGMT_ACTION_CATEGORY_QOS            = 1,
    IEEE_MGMT_ACTION_CATEGORY_DLS            = 2,
    IEEE_MGMT_ACTION_CATEGORY_BLOCK_ACK      = 3,
    IEEE_MGMT_ACTION_CATEGORY_PUBLIC         = 4,
    IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC     = 5,
    IEEE_MGMT_ACTION_CATEGORY_FAST_BSS_TRANS = 6,
    IEEE_MGMT_ACTION_CATEGORY_HT             = 7,
    IEEE_MGMT_ACTION_CATEGORY_SA_QUERY       = 8,
    IEEE_MGMT_ACTION_CATEGORY_PROTECTED_DUAL = 9,
    IEEE_MGMT_ACTION_CATEGORY_WNM            = 10,
    IEEE_MGMT_ACTION_CATEGORY_UNPROTECT_WNM  = 11,
    IEEE_MGMT_ACTION_CATEGORY_WMM_TSPEC      = 17,
    IEEE_MGMT_ACTION_CATEGORY_FST            = 18,
    IEEE_MGMT_ACTION_CATEGORY_AV_STREAMING   = 19,
    IEEE_MGMT_ACTION_CATEGORY_VENDOR_SPEC    = 127
} MLAN_PACK_END IEEEtypes_ActionCategory_e;

/** IEEE Std 802.11-2016 - Table 9-354 WNM Action field values */
typedef MLAN_PACK_START enum _IEEEtypes_WNM_ActionFieldType_e {

    IEEE_MGMT_WNM_EVENT_REQUEST                   = 0,
    IEEE_MGMT_WNM_EVENT_REPORT                    = 1,
    IEEE_MGMT_WNM_DIAGNOSTIC_REQUEST              = 2,
    IEEE_MGMT_WNM_DIAGNOSTIC_REPORT               = 3,
    IEEE_MGMT_WNM_LOCATION_CFG_REQUEST            = 4,
    IEEE_MGMT_WNM_LOCATION_CFG_RESPONSE           = 5,
    IEEE_MGMT_WNM_BTM_QUERY                       = 6,
    IEEE_MGMT_WNM_BTM_REQUEST                     = 7,
    IEEE_MGMT_WNM_BTM_RESPONSE                    = 8,
    IEEE_MGMT_WNM_FMS_REQUEST                     = 9,
    IEEE_MGMT_WNM_FMS_RESPONSE                    = 10,
    IEEE_MGMT_WNM_COLLOCATED_INTERFERENCE_REQUEST = 11,
    IEEE_MGMT_WNM_COLLOCATED_INTERFERENCE_REPORT  = 12,
    IEEE_MGMT_WNM_TFS_REQUEST                     = 13,
    IEEE_MGMT_WNM_TFS_RESPONSE                    = 14,
    IEEE_MGMT_WNM_TFS_NOTIFY                      = 15,
    IEEE_MGMT_WNM_SLEEP_MODE_REQUEST              = 16,
    IEEE_MGMT_WNM_SLEEP_MODE_RESPONSE             = 17,
    IEEE_MGMT_WNM_TIM_BROADCAST_REQUEST           = 18,
    IEEE_MGMT_WNM_TIM_BROADCAST_RESPONSE          = 19,
    IEEE_MGMT_WNM_QOS_TRAFFIC_CAPABILITY_UPDATE   = 20,
    IEEE_MGMT_WNM_CHANNEL_USAGE_REQUEST           = 21,
    IEEE_MGMT_WNM_CHANNEL_USAGE_RESPONSE          = 22,
    IEEE_MGMT_WNM_DMS_REQUEST                     = 23,
    IEEE_MGMT_WNM_DMS_RESPONSE                    = 24,
    IEEE_MGMT_WNM_TIMING_MEASUREMENT_REQUEST      = 25,
    IEEE_MGMT_WNM_NOTIFICATION_REQUEST            = 26,
    IEEE_MGMT_WNM_NOTIFICATION_RESPONSE           = 27,
    IEEE_MGMT_WNM_NOTIFY_RESPONSE                 = 28
} MLAN_PACK_END IEEEtypes_WNM_ActionFieldType_e;

#if CONFIG_11V
/* IEEE Std 802.11-2016 - Figure 9-702 Request Mode field */
#define IEEE_WNM_BTM_REQUEST_PREFERENCE_CAND_LIST_INCLUDED MBIT(0)
#define IEEE_WNM_BTM_REQUEST_ABRIDGED                      MBIT(1)
#define IEEE_WNM_BTM_REQUEST_DISASSOC_IMMINENT             MBIT(2)
#define IEEE_WNM_BTM_REQUEST_BSS_TERMINATION_INCLUDED      MBIT(3)
#define IEEE_WNM_BTM_REQUEST_ESS_DISASSOC_IMMINENT         MBIT(4)
#endif

/** WMM TSPEC operations */
typedef MLAN_PACK_START enum _IEEEtypes_WMM_Tspec_Action_e {

    TSPEC_ACTION_CODE_ADDTS_REQ = 0,
    TSPEC_ACTION_CODE_ADDTS_RSP = 1,
    TSPEC_ACTION_CODE_DELTS     = 2,

} MLAN_PACK_END IEEEtypes_WMM_Tspec_Action_e;

/** WMM TSPEC Category Action Base */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ActionCategory_e category;
    IEEEtypes_WMM_Tspec_Action_e action;
    t_u8 dialogToken;

} MLAN_PACK_END IEEEtypes_WMM_Tspec_Action_Base_Tspec_t;

/** WMM TSPEC AddTS request structure */
typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 statusCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

    /* Place holder for additional elements after the TSPEC */
    t_u8 subElem[256];

} MLAN_PACK_END IEEEtypes_Action_WMM_AddTsReq_t;

/** WMM TSPEC AddTS response structure */
typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 statusCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

    /* Place holder for additional elements after the TSPEC */
    t_u8 subElem[256];

} MLAN_PACK_END IEEEtypes_Action_WMM_AddTsRsp_t;

/** WMM TSPEC DelTS structure */
typedef MLAN_PACK_START struct
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;
    t_u8 reasonCode;
    IEEEtypes_WMM_TSPEC_t tspecIE;

} MLAN_PACK_END IEEEtypes_Action_WMM_DelTs_t;

/** union of WMM TSPEC structures */
typedef MLAN_PACK_START union
{
    IEEEtypes_WMM_Tspec_Action_Base_Tspec_t tspecAct;

    IEEEtypes_Action_WMM_AddTsReq_t addTsReq;
    IEEEtypes_Action_WMM_AddTsRsp_t addTsRsp;
    IEEEtypes_Action_WMM_DelTs_t delTs;

} MLAN_PACK_END IEEEtypes_Action_WMMAC_t;

/** union of WMM TSPEC & Action category */
typedef MLAN_PACK_START union
{
    IEEEtypes_ActionCategory_e category;

    IEEEtypes_Action_WMMAC_t wmmAc;

} MLAN_PACK_END IEEEtypes_ActionFrame_t;

/** action code for 20/40 BSS Coexsitence Management frame */
#define BSS_20_40_COEX 0
/** action code for FILS Discovery Management frame */
#define FILS_DISCOVERY 34

/** Data structure for subband set */
typedef MLAN_PACK_START struct _IEEEtypes_SubbandSet_t
{
    /** First channel */
    t_u8 first_chan;
    /** Number of channels */
    t_u8 no_of_chan;
    /** Maximum Tx power in dBm */
    t_u8 max_tx_pwr;
} MLAN_PACK_END IEEEtypes_SubbandSet_t, *pIEEEtypes_SubbandSet_t;

#ifdef STA_SUPPORT
/** Data structure for Country IE */
typedef MLAN_PACK_START struct _IEEEtypes_CountryInfoSet_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[1];
} MLAN_PACK_END IEEEtypes_CountryInfoSet_t, *pIEEEtypes_CountryInfoSet_t;

/** Data structure for Country IE full set */
typedef MLAN_PACK_START struct _IEEEtypes_CountryInfoFullSet_t
{
    /** Element ID */
    IEEEtypes_ElementId_e element_id;
    /** Length */
    t_u8 len;
    /** Country code */
    t_u8 country_code[COUNTRY_CODE_LEN];
    /** Set of subbands */
    IEEEtypes_SubbandSet_t sub_band[MRVDRV_MAX_SUBBAND_802_11D];
} MLAN_PACK_END IEEEtypes_CountryInfoFullSet_t, *pIEEEtypes_CountryInfoFullSet_t;

#endif /* STA_SUPPORT */

/** HT Capabilities Data */
typedef MLAN_PACK_START struct _HTCap_t
{
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
} MLAN_PACK_END HTCap_t, *pHTCap_t;

/** HT Information Data */
typedef MLAN_PACK_START struct _HTInfo_t
{
    /** Primary channel */
    t_u8 pri_chan;
    /** Field 2 */
    t_u8 field2;
    /** Field 3 */
    t_u16 field3;
    /** Field 4 */
    t_u16 field4;
    /** Bitmap indicating MCSs supported by all HT STAs in the BSS */
    t_u8 basic_mcs_set[16];
} MLAN_PACK_END HTInfo_t, *pHTInfo_t;

/** 20/40 BSS Coexistence Data */
typedef MLAN_PACK_START struct _BSSCo2040_t
{
    /** 20/40 BSS Coexistence value */
    t_u8 bss_co_2040_value;
} MLAN_PACK_END BSSCo2040_t, *pBSSCo2040_t;

/** Extended Capabilities Data */
typedef MLAN_PACK_START struct _ExtCap_t
{
    /** Extended Capabilities value */
    t_u8 BSS_CoexistSupport : 1;   /* bit 0 */
    t_u8 Reserved1 : 1;            /* bit 1 */
    t_u8 ExtChanSwitching : 1;     /* bit 2 */
    t_u8 RejectUnadmFrame : 1;     /* bit 3 */
    t_u8 PSMP_Capable : 1;         /* bit 4 */
    t_u8 Reserved5 : 1;            /* bit 5 */
    t_u8 SPSMP_Support : 1;        /* bit 6 */
    t_u8 Event : 1;                /* bit 7 */
    t_u8 Diagnostics : 1;          /* bit 8 */
    t_u8 MulticastDiagnostics : 1; /* bit 9 */
    t_u8 LocationTracking : 1;     /* bit 10 */
    t_u8 FMS : 1;                  /* bit 11 */
    t_u8 ProxyARPService : 1;      /* bit 12 */
    t_u8 CollocatedIntf : 1;       /* bit 13 */
    t_u8 CivicLocation : 1;        /* bit 14 */
    t_u8 GeospatialLocation : 1;   /* bit 15 */
    t_u8 TFS : 1;                  /* bit 16 */
    t_u8 WNM_Sleep : 1;            /* bit 17 */
    t_u8 TIM_Broadcast : 1;        /* bit 18 */
    t_u8 BSS_Transition : 1;       /* bit 19 */
    t_u8 QoSTrafficCap : 1;        /* bit 20 */
    t_u8 AC_StationCount : 1;      /* bit 21 */
    t_u8 MultipleBSSID : 1;        /* bit 22 */
    t_u8 TimingMeasurement : 1;    /* bit 23 */
    t_u8 ChannelUsage : 1;         /* bit 24 */
    t_u8 SSID_List : 1;            /* bit 25 */
    t_u8 DMS : 1;                  /* bit 26 */
    t_u8 UTC : 1;                  /* bit 27 */
    t_u8 TDLSPeerUAPSDSupport : 1; /* bit 28 */
    t_u8 TDLSPeerPSMSupport : 1;   /* bit 29 */
    t_u8 TDLSChannelSwitching : 1; /* bit 30 */
    t_u8 Interworking : 1;         /* bit 31 */
    t_u8 Qos_Map : 1;              /* bit 32 */
    t_u8 EBR : 1;                  /* bit 33 */
    t_u8 SSPN_Interface : 1;       /* bit 34 */
    t_u8 Reserved35 : 1;           /* bit 35 */
    t_u8 MSGCF_Capa : 1;           /* bit 36 */
    t_u8 TDLSSupport : 1;          /* bit 37 */
    t_u8 TDLSProhibited : 1;       /* bit 38 */
    t_u8 TDLSChlSwitchProhib : 1;  /* bit 39 */
    t_u8 rsvdBit40 : 1;            /* bit 40 */
    t_u8 rsvdBit41 : 1;            /* bit 41 */
    t_u8 rsvdBit42 : 1;            /* bit 42 */
    t_u8 rsvdBit43 : 1;            /* bit 43 */
    t_u8 rsvdBit44 : 1;            /* bit 44 */
    t_u8 rsvdBit45 : 1;            /* bit 45 */
    t_u8 rsvdBit46 : 1;            /* bit 46 */
    t_u8 rsvdBit47 : 1;            /* bit 47 */
    t_u8 rsvdBit48 : 1;            /* bit 48 */
    t_u8 rsvdBit49 : 1;            /* bit 49 */
    t_u8 rsvdBit50 : 1;            /* bit 50 */
    t_u8 rsvdBit51 : 1;            /* bit 51 */
    t_u8 rsvdBit52 : 1;            /* bit 52 */
    t_u8 rsvdBit53 : 1;            /* bit 53 */
    t_u8 rsvdBit54 : 1;            /* bit 54 */
    t_u8 rsvdBit55 : 1;            /* bit 55 */
    t_u8 rsvdBit56 : 1;            /* bit 56 */
    t_u8 rsvdBit57 : 1;            /* bit 57 */
    t_u8 rsvdBit58 : 1;            /* bit 58 */
    t_u8 rsvdBit59 : 1;            /* bit 59 */
    t_u8 rsvdBit60 : 1;            /* bit 60 */
    t_u8 TDLSWildBandwidth : 1;    /* bit 61 */
    t_u8 OperModeNtf : 1;          /* bit 62 */
    t_u8 MaxAMSDU0 : 1;            /* bit 63 */
    t_u8 MaxAMSDU1 : 1;            /* bit 64 */
    t_u8 chanSchedMgnt : 1;        /* bit 65 */
    t_u8 rsvdBit66 : 1;            /* bit 66 */
    t_u8 NCC : 1;                  /* bit 67 */
    t_u8 rsvdBit68 : 1;            /* bit 68 */
    t_u8 CAQ : 1;                  /* bit 69 */
    t_u8 FTMR : 1;                 /* bit 70 */
    t_u8 FTMI : 1;                 /* bit 71 */
    t_u8 FILS : 1;                 /* bit 72 */
    t_u8 rsvdBit73 : 1;            /* bit 73 */
    t_u8 rsvdBit74 : 1;            /* bit 74 */
    t_u8 rsvdBit75 : 1;            /* bit 75 */
    t_u8 rsvdBit76 : 1;            /* bit 76 */
#if CONFIG_11AX
    t_u8 TWTReq : 1;               /* bit 77 */
    t_u8 TWTResp : 1;              /* bit 78 */
#else
    t_u8 rsvdBit77 : 1; /* bit 77 */
    t_u8 rsvdBit78 : 1; /* bit 78 */
#endif
    t_u8 rsvdBit79 : 1;            /* bit 79 */

} MLAN_PACK_END ExtCap_t, *pExtCap_t;

/** Overlapping BSS Scan Parameters Data */
typedef MLAN_PACK_START struct _OverlapBSSScanParam_t
{
    /** OBSS Scan Passive Dwell in milliseconds */
    t_u16 obss_scan_passive_dwell;
    /** OBSS Scan Active Dwell in milliseconds */
    t_u16 obss_scan_active_dwell;
    /** BSS Channel Width Trigger Scan Interval in seconds */
    t_u16 bss_chan_width_trigger_scan_int;
    /** OBSS Scan Passive Total Per Channel */
    t_u16 obss_scan_passive_total;
    /** OBSS Scan Active Total Per Channel */
    t_u16 obss_scan_active_total;
    /** BSS Width Channel Transition Delay Factor */
    t_u16 bss_width_chan_trans_delay;
    /** OBSS Scan Activity Threshold */
    t_u16 obss_scan_active_threshold;
} MLAN_PACK_END OBSSScanParam_t, *pOBSSScanParam_t;

/** HT Capabilities IE */
typedef MLAN_PACK_START struct _IEEEtypes_HTCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTCap struct */
    HTCap_t ht_cap;
} MLAN_PACK_END IEEEtypes_HTCap_t, *pIEEEtypes_HTCap_t;

/** HT Information IE */
typedef MLAN_PACK_START struct _IEEEtypes_HTInfo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** HTInfo struct */
    HTInfo_t ht_info;
} MLAN_PACK_END IEEEtypes_HTInfo_t, *pIEEEtypes_HTInfo_t;

#if CONFIG_MULTI_BSSID_SUPPORT
/** the AP which send the multi_bssid IE */
#define MULTI_BSSID_AP 1
/** the AP which don't send beacon */
#define MULTI_BSSID_SUB_AP 2
/** IEEEtypes_NotxBssCap_t */
typedef MLAN_PACK_START struct _IEEEtypes_NotxBssCap_t
{
    /** Nontransmitted BSSID Capability: Element ID */
    t_u8 element_id;
    /** Nontransmitted BSSID Capability : Length */
    t_u8 len;
    /** capability */
    t_u16 cap;
} MLAN_PACK_END IEEEtypes_NotxBssCap_t, *pIEEEtypes_NotxBssCap_t;

/** Multi BSSID IE */
typedef MLAN_PACK_START struct _IEEEtypes_MultiBSSIDIndex_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** BSSID Index */
    t_u8 bssid_index;
    /** DTIM Period (Optional, not Present in ProbeRsp) */
    t_u8 dtim_period;
    /** DTIM Count (Optional, not Present in ProbeRsp) */
    t_u8 dtim_count;
} MLAN_PACK_END IEEEtypes_MultiBSSIDIndex_t, *pIEEEtypes_MultiBSSIDIndex_t;

/** NonTransmitted BSSID Profile Subelement IE */
/** SUBID for IEEEtypes_NonTransBSSIDCap_t */
#define NONTRANS_BSSID_PROFILE_SUBELEM_ID 0

/** NonTransmitted BSSID Capability IE */
typedef MLAN_PACK_START struct _IEEEtypes_NonTransBSSIDProfile_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    t_u8 profile_data[];
} MLAN_PACK_END IEEEtypes_NonTransBSSIDProfile_t, *pIEEEtypes_NonTransBSSIDProfile_t;

/** Multi BSSID IE */
typedef MLAN_PACK_START struct _IEEEtypes_MultiBSSID_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Max BSSID Indicator */
    t_u8 max_bssid_indicator;
    /** Optional Subelement data*/
    t_u8 sub_elem_data[];
} MLAN_PACK_END IEEEtypes_MultiBSSID_t, *pIEEEtypes_MultiBSSID_t;
#endif

/** 20/40 BSS Coexistence IE */
typedef MLAN_PACK_START struct _IEEEtypes_2040BSSCo_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** BSSCo2040_t struct */
    BSSCo2040_t bss_co_2040;
} MLAN_PACK_END IEEEtypes_2040BSSCo_t, *pIEEEtypes_2040BSSCo_t;

/** Extended Capabilities IE */
typedef MLAN_PACK_START struct _IEEEtypes_ExtCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** ExtCap_t struct */
    ExtCap_t ext_cap;
} MLAN_PACK_END IEEEtypes_ExtCap_t, *pIEEEtypes_ExtCap_t;

/** Overlapping BSS Scan Parameters IE */
typedef MLAN_PACK_START struct _IEEEtypes_OverlapBSSScanParam_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** OBSSScanParam_t struct */
    OBSSScanParam_t obss_scan_param;
} MLAN_PACK_END IEEEtypes_OverlapBSSScanParam_t, *pIEEEtypes_OverlapBSSScanParam_t;

/** VHT MCS rate set field, refer to 802.11ac */
typedef MLAN_PACK_START struct _VHT_MCS_set
{
    t_u16 rx_mcs_map;
    t_u16 rx_max_rate; /* bit 29-31 reserved */
    t_u16 tx_mcs_map;
    t_u16 tx_max_rate; /* bit 61-63 reserved */
} MLAN_PACK_END VHT_MCS_set_t, *pVHT_MCS_set_t;

/** VHT Capabilities info field, reference 802.11ac D1.4 p89 */
typedef MLAN_PACK_START struct _VHT_capa
{
#if 0
    t_u8 reserved_1:4;
    t_u8 link_apapt_capa:2;
    t_u8 max_ampdu_len:3;
    t_u8 HTC_VHT_capa:1;
    t_u8 VHT_TXOP_ps:1;
    t_u8 MU_beamformee_capa:1;
    t_u8 MU_beamformer_capa:1;
    t_u8 sounding_dim_num:3;
    t_u8 beamformer_ante_num:3;
    t_u8 SU_beamformee_capa:1;
    t_u8 SU_beamformer_capa:1;
    t_u8 rx_STBC:3;
    t_u8 tx_STBC:1;
    t_u8 sgi_160:1;
    t_u8 sgi_80:1;
    t_u8 rx_LDPC:1;
    t_u8 chan_width:2;
    t_u8 mpdu_max_len:2;
#endif
    t_u32 vht_cap_info;
    VHT_MCS_set_t mcs_sets;
} MLAN_PACK_END VHT_capa_t, *pVHT_capa_t;

/** VHT Capabilities IE */
typedef MLAN_PACK_START struct _IEEEtypes_VHTCap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    VHT_capa_t vht_cap;
} MLAN_PACK_END IEEEtypes_VHTCap_t, *pIEEEtypes_VHTCap_t;

#define VHT_CAP_CHWD_80MHZ    0
#define VHT_CAP_CHWD_160MHZ   1
#define VHT_CAP_CHWD_80_80MHZ 2

/** VHT Operations IE */
typedef MLAN_PACK_START struct _IEEEtypes_VHTOprat_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Channel width */
    t_u8 chan_width;
    /** Channel center frequency 1 */
    t_u8 chan_center_freq_1;
    /** Channel center frequency 2 */
    t_u8 chan_center_freq_2;
    /** Basic MCS set map, each 2 bits stands for a Nss */
    t_u16 basic_MCS_map;
} MLAN_PACK_END IEEEtypes_VHTOprat_t, *pIEEEtypes_VHTOprat_t;

#define VHT_OPER_CHWD_20_40MHZ 0
#define VHT_OPER_CHWD_80MHZ    1U
#define VHT_OPER_CHWD_160MHZ   2
#define VHT_OPER_CHWD_80_80MHZ 3

/** VHT Transmit Power Envelope IE */
typedef MLAN_PACK_START struct _IEEEtypes_VHTtxpower_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** maximum tx power */
    t_u8 max_tx_power;
    /** channel center frequency */
    t_u8 chan_center_freq;
    /** channel width */
    t_u8 chan_width;
} MLAN_PACK_END IEEEtypes_VHTtxpower_t, *pIEEEtypes_VHTtxpower_t;

/** Extended Power Constraint IE */
typedef MLAN_PACK_START struct _IEEEtypes_ExtPwerCons_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** channel width */
    t_u8 chan_width;
    /** local power constraint */
    t_u8 local_power_cons;
} MLAN_PACK_END IEEEtypes_ExtPwerCons_t, *pIEEEtypes_ExtPwerCons_t;

#if (CONFIG_11AC) || (CONFIG_ECSA)
/*  IEEE Wide Bandwidth Channel Switch Element */
/**
 *  Provided in beacons and probe responses.  Used to advertise when
 *    and to which channel it is changing to.  Only starting STAs in
 *    an IBSS and APs are allowed to originate a wide bandwidth chan
 *    switch element.
 */
typedef MLAN_PACK_START struct
{
    /** Generic IE header IEEE Element ID = 194*/
    IEEEtypes_Header_t ieee_hdr;
    t_u8 new_channel_width;
    t_u8 new_channel_center_freq0;
    t_u8 new_channel_center_freq1;
} MLAN_PACK_END IEEEtypes_WideBWChanSwitch_t;

/*  IEEE VHT Transmit Power Envelope Element */
/**
 *  Provided in beacons and probe responses.  Used to advertise the max
 *    TX power in sepeate bandwidth and as a sub element of Channel Switch
 *    Wrapper IE.
 */
typedef MLAN_PACK_START struct
{
    /** Generic IE header IEEE Element ID = 195*/
    IEEEtypes_Header_t ieee_hdr;
    t_u8 tpc_info;                     /**< Transmit Power Information>*/
    t_u8 local_max_tp_20mhz;           /**< Local Maximum Transmit Power for 20 MHZ>*/
    t_u8 local_max_tp_40mhz;           /**< Local Maximum Transmit Power for 40 MHZ>*/
    t_u8 local_max_tp_80mhz;           /**< Local Maximum Transmit Power for 80 MHZ>*/
    t_u8 local_max_tp_160mhz_80_80mhz; /**< Local Maximum Transmit Power for 160/80+80 MHZ>*/
} MLAN_PACK_END IEEEtypes_VhtTpcEnvelope_t;
#endif

#if CONFIG_ECSA
/** data structure for extended channel switch */
typedef MLAN_PACK_START struct
{
    /** IEEE element ID = 60 */
    t_u8 element_id;
    /** Element length after id and len, set to 4 */
    t_u8 len;
    /** STA should not transmit any frames if 1 */
    t_u8 chan_switch_mode;
    /** Operate class # that AP/IBSS is moving to */
    t_u8 new_oper_class;
    /** Channel # that AP/IBSS is moving to */
    t_u8 new_channel_num;
    /** of TBTTs before channel switch */
    t_u8 chan_switch_count;
} MLAN_PACK_END IEEEtypes_ExtChanSwitchAnn_t;
#endif

/** Extended BSS Load IE */
typedef MLAN_PACK_START struct _IEEEtypes_ExtBSSload_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** MIMO capability count */
    t_u8 MU_MIMO_capa_count;
    /** stream under utilization */
    t_u8 stream_underutilization;
    /** VHT 40 util */
    t_u8 VHT40_util;
    /** VHT 80 util */
    t_u8 VHT80_util;
    /** VHT 160 util */
    t_u8 VHT160_util;
} MLAN_PACK_END IEEEtypes_ExtBSSload_t, *pIEEEtypes_ExtBSSload_t;

/** Quiet Channel IE */
typedef MLAN_PACK_START struct _IEEEtypes_QuietChan_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** AP quiet mode */
    t_u8 AP_quiet_mode;
    /** Quiet count */
    t_u8 quiet_count;
    /** Quiet period */
    t_u8 quiet_period;
    /** Quiet duration */
    t_u16 quiet_dur;
    /** Quiet offset */
    t_u16 quiet_offset;
} MLAN_PACK_END IEEEtypes_QuietChan_t, *pIEEEtypes_QuietChan_t;

/** Wide Bandwidth Channel Switch IE */
typedef MLAN_PACK_START struct _IEEEtypes_BWSwitch_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** New channel width */
    t_u8 new_chan_width;
    /** New channel center frequency 1 */
    t_u8 new_chan_center_freq_1;
    /** New channel center frequency 2 */
    t_u8 new_chan_center_freq_2;
} MLAN_PACK_END IEEEtypes_BWSwitch_t, *pIEEEtypes_BWSwitch_t;

/** AID IE */
typedef MLAN_PACK_START struct _IEEEtypes_AID_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** AID number */
    t_u16 AID;
} MLAN_PACK_END IEEEtypes_AID_t, *pIEEEtypes_AID_t;

/** Operating Mode Notificaton IE */
typedef MLAN_PACK_START struct _IEEEtypes_OperModeNtf_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Operating Mode */
    t_u8 oper_mode;
} MLAN_PACK_END IEEEtypes_OperModeNtf_t, *pIEEEtypes_OperModeNtf_t;

/** Maximum number of subbands in the IEEEtypes_SupportedChannels_t structure */
#define WLAN_11H_MAX_SUBBANDS 5

/** Maximum number of DFS channels configured in IEEEtypes_IBSS_DFS_t */
#define WLAN_11H_MAX_IBSS_DFS_CHANNELS 25

/**  IEEE Power Constraint element (7.3.2.15) */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 32 */
    t_u8 len;                         /**< Element length after id and len */
    t_u8 local_constraint;            /**< Local power constraint applied to 11d chan info */
} MLAN_PACK_END IEEEtypes_PowerConstraint_t;

/**  IEEE Power Capability element (7.3.2.16) */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 33 */
    t_u8 len;                         /**< Element length after id and len */
    t_s8 min_tx_power_capability;     /**< Minimum Transmit power (dBm) */
    t_s8 max_tx_power_capability;     /**< Maximum Transmit power (dBm) */
} MLAN_PACK_END IEEEtypes_PowerCapability_t;

/**  IEEE TPC Report element (7.3.2.18) */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 35 */
    t_u8 len;                         /**< Element length after id and len */
    t_s8 tx_power;                    /**< Max power used to transmit the TPC Report frame (dBm) */
    t_s8 link_margin;                 /**< Link margin when TPC Request received (dB) */
} MLAN_PACK_END IEEEtypes_TPCReport_t;

/*  IEEE Supported Channel sub-band description (7.3.2.19) */
/**
 *  Sub-band description used in the supported channels element.
 */
typedef MLAN_PACK_START struct
{
    t_u8 start_chan; /**< Starting channel in the subband */
    t_u8 num_chans;  /**< Number of channels in the subband */

} MLAN_PACK_END IEEEtypes_SupportChan_Subband_t;

/*  IEEE Supported Channel element (7.3.2.19) */
/**
 *  Sent in association requests. Details the sub-bands and number
 *    of channels supported in each subband
 */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 36 */
    t_u8 len;                         /**< Element length after id and len */

    /** Configured sub-bands information in the element */
    IEEEtypes_SupportChan_Subband_t subband[WLAN_11H_MAX_SUBBANDS];

} MLAN_PACK_END IEEEtypes_SupportedChannels_t;

/*  IEEE Channel Switch Announcement Element (7.3.2.20) */
/**
 *  Provided in beacons and probe responses.  Used to advertise when
 *    and to which channel it is changing to.  Only starting STAs in
 *    an IBSS and APs are allowed to originate a chan switch element.
 */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 37 */
    t_u8 len;                         /**< Element length after id and len */
    t_u8 chan_switch_mode;            /**< STA should not transmit any frames if 1 */
    t_u8 new_channel_num;             /**< Channel # that AP/IBSS is moving to */
    t_u8 chan_switch_count;           /**< # of TBTTs before channel switch */

} MLAN_PACK_END IEEEtypes_ChanSwitchAnn_t;

/*  IEEE Quiet Period Element (7.3.2.23) */
/**
 *  Provided in beacons and probe responses.  Indicates times during
 *    which the STA should not be transmitting data.  Only starting STAs in
 *    an IBSS and APs are allowed to originate a quiet element.
 */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id; /**< IEEE Element ID = 40 */
    t_u8 len;                         /**< Element length after id and len */
    t_u8 quiet_count;                 /**< Number of TBTTs until beacon with the quiet period */
    t_u8 quiet_period;                /**< Regular quiet period, # of TBTTS between periods */
    t_u16 quiet_duration;             /**< Duration of the quiet period in TUs */
    t_u16 quiet_offset;               /**< Offset in TUs from the TBTT for the quiet period */

} MLAN_PACK_END IEEEtypes_Quiet_t;

/**
***  @brief Map octet of the basic measurement report (7.3.2.22.1)
**/
typedef MLAN_PACK_START struct
{
    t_u8 bss : 1;              /**< At least one valid MPDU received on channel */
    t_u8 ofdm_preamble : 1;    /**< OFDM preamble detected on channel */
    t_u8 unidentified_sig : 1; /**< Unidentified signal found on channel */
    t_u8 radar : 1;            /**< Radar detected on channel */
    t_u8 unmeasured : 1;       /**< Channel is unmeasured */
    t_u8 rsvd5_7 : 3;          /**< Reserved */

} MLAN_PACK_END MeasRptBasicMap_t;

/*  IEEE DFS Channel Map field (7.3.2.24) */
/**
 *  Used to list supported channels and provide a octet "map" field which
 *    contains a basic measurement report for that channel in the
 *    IEEEtypes_IBSS_DFS_t element
 */
typedef MLAN_PACK_START struct
{
    t_u8 channel_number;       /**< Channel number */
    MeasRptBasicMap_t rpt_map; /**< Basic measurement report for the channel */

} MLAN_PACK_END IEEEtypes_ChannelMap_t;

/*  IEEE IBSS DFS Element (7.3.2.24) */
/**
 *  IBSS DFS element included in ad hoc beacons and probe responses.
 *    Provides information regarding the IBSS DFS Owner as well as the
 *    originating STAs supported channels and basic measurement results.
 */
typedef MLAN_PACK_START struct
{
    IEEEtypes_ElementId_e element_id;     /**< IEEE Element ID = 41 */
    t_u8 len;                             /**< Element length after id and len */
    t_u8 dfs_owner[MLAN_MAC_ADDR_LENGTH]; /**< DFS Owner STA Address */
    t_u8 dfs_recovery_interval;           /**< DFS Recovery time in TBTTs */

    /** Variable length map field, one Map entry for each supported channel */
    IEEEtypes_ChannelMap_t channel_map[WLAN_11H_MAX_IBSS_DFS_CHANNELS];

} MLAN_PACK_END IEEEtypes_IBSS_DFS_t;

/* 802.11h BSS information kept for each BSSID received in scan results */
/**
 * IEEE BSS information needed from scan results for later processing in
 *    join commands
 */
typedef struct
{
    t_u8 sensed_11h;                              /**< Capability bit set or 11h IE found in this BSS */

    IEEEtypes_PowerConstraint_t power_constraint; /**< Power Constraint IE */
    IEEEtypes_PowerCapability_t power_capability; /**< Power Capability IE */
    IEEEtypes_TPCReport_t tpc_report;             /**< TPC Report IE */
    IEEEtypes_ChanSwitchAnn_t chan_switch_ann;    /**< Channel Switch Announcement IE */
    IEEEtypes_Quiet_t quiet;                      /**< Quiet IE */
    IEEEtypes_IBSS_DFS_t ibss_dfs;                /**< IBSS DFS Element IE */

} wlan_11h_bss_info_t;

#ifdef STA_SUPPORT
/** Macro for maximum size of scan response buffer */
#define MAX_SCAN_RSP_BUF (16 * 1024)

/** Maximum number of channels that can be sent in user scan config */
#define WLAN_USER_SCAN_CHAN_MAX 50U

/** Maximum length of SSID list */
#define MRVDRV_MAX_SSID_LIST_LENGTH 10

/** Scan all the channels in specified band */
#define BAND_SPECIFIED 0x80U

/**
 *  IOCTL SSID List sub-structure sent in wlan_ioctl_user_scan_cfg
 *
 *  Used to specify SSID specific filters as well as SSID pattern matching
 *    filters for scan result processing in firmware.
 */
typedef MLAN_PACK_START struct _wlan_user_scan_ssid
{
    /** SSID */
    t_u8 ssid[MLAN_MAX_SSID_LENGTH + 1];
    /** Maximum length of SSID */
    t_u8 max_len;
} MLAN_PACK_END wlan_user_scan_ssid;

/**
 *  @brief IOCTL channel sub-structure sent in wlan_ioctl_user_scan_cfg
 *
 *  Multiple instances of this structure are included in the IOCTL command
 *   to configure a instance of a scan on the specific channel.
 */
typedef MLAN_PACK_START struct _wlan_user_scan_chan
{
    /** Channel Number to scan */
    t_u8 chan_number;
    /** Radio type: 'B/G' Band = 0, 'A' Band = 1 */
    t_u8 radio_type;
    /** Scan type: Active = 1, Passive = 2 */
    mlan_scan_type scan_type;
    /** Reserved */
    t_u8 reserved;
    /** Scan duration in milliseconds; if 0 default used */
    t_u32 scan_time;
} MLAN_PACK_END wlan_user_scan_chan;

#if CONFIG_SCAN_CHANNEL_GAP
/** channel statictics */
typedef MLAN_PACK_START struct _ChanStatistics_t
{
    /** channle number */
    t_u8 chan_num;
    /** band info */
    Band_Config_t bandcfg;
    /** flags */
    t_u8 flags;
    /** noise */
    t_s8 noise;
    /** total network */
    t_u16 total_networks;
    /** scan duration */
    t_u16 cca_scan_duration;
    /** busy duration */
    t_u16 cca_busy_duration;
    /** min rss */
    t_u8 min_rss;
    /** max rssi */
    t_u8 max_rss;
} MLAN_PACK_END ChanStatistics_t;
#endif

/**
 *  Input structure to configure an immediate scan cmd to firmware
 *
 *  Specifies a number of parameters to be used in general for the scan
 *    as well as a channel list (wlan_user_scan_chan) for each scan period
 *    desired.
 */
typedef MLAN_PACK_START struct
{
    /**
     *  Flag set to keep the previous scan table intact
     *
     *  If set, the scan results will accumulate, replacing any previous
     *   matched entries for a BSS with the new scan data
     */
    t_u8 keep_previous_scan;
    /**
     *  BSS mode to be sent in the firmware command
     *
     *  Field can be used to restrict the types of networks returned in the
     *    scan.  Valid settings are:
     *
     *   - MLAN_SCAN_MODE_BSS  (infrastructure)
     *   - MLAN_SCAN_MODE_IBSS (adhoc)
     *   - MLAN_SCAN_MODE_ANY  (unrestricted, adhoc and infrastructure)
     */
    t_u8 bss_mode;
    /**
     *  Configure the number of probe requests for active chan scans
     */
    t_u8 num_probes;
#if CONFIG_SCAN_WITH_RSSIFILTER
    /**
     *  Threshold of RSSI
     */
    t_s16 rssi_threshold;
#endif
    /**
     *  @brief Reserved
     */
    t_u8 reserved;
    /**
     *  @brief BSSID filter sent in the firmware command to limit the results
     */
    t_u8 specific_bssid[MLAN_MAC_ADDR_LENGTH];
    /**
     *  SSID filter list used in the to limit the scan results
     */
    wlan_user_scan_ssid ssid_list[MRVDRV_MAX_SSID_LIST_LENGTH];
    /**
     *  Variable number (fixed maximum) of channels to scan up
     */
    wlan_user_scan_chan chan_list[WLAN_USER_SCAN_CHAN_MAX];
#if CONFIG_SCAN_CHANNEL_GAP
    /** scan channel gap */
    t_u16 scan_chan_gap;
#endif
} MLAN_PACK_END wlan_user_scan_cfg;
#if CONFIG_BG_SCAN
/** Default scan interval in millisecond*/
#define DEFAULT_BGSCAN_INTERVAL 30000

/** action get all, except pps/uapsd config */
#define BG_SCAN_ACT_GET 0x0000
/** action set all, except pps/uapsd config */
#define BG_SCAN_ACT_SET 0x0001
#if CONFIG_WMM_UAPSD
/** action get pps/uapsd config */
#define BG_SCAN_ACT_GET_PPS_UAPSD 0x0100
/** action set pps/uapsd config */
#define BG_SCAN_ACT_SET_PPS_UAPSD 0x0101
#endif
/** action set all */
#define BG_SCAN_ACT_SET_ALL 0xff01
/** ssid match */
#define BG_SCAN_SSID_MATCH 0x0001
/** ssid match and RSSI exceeded */
#define BG_SCAN_SSID_RSSI_MATCH 0x0004
/**wait for all channel scan to complete to report scan result*/
#define BG_SCAN_WAIT_ALL_CHAN_DONE 0x80000000
/** Maximum number of channels that can be sent in bg scan config */
#define WLAN_BG_SCAN_CHAN_MAX 38
#if CONFIG_UNII4_BAND_SUPPORT
/** max bgscan chan number, include UNII_4 channel */
#define WLAN_BG_SCAN_CHAN_MAX_UNII_4 41
#endif
/** Min BGSCAN interval 30 second */
#define MIN_BGSCAN_INTERVAL 30000
/** default repeat count */
#define DEF_REPEAT_COUNT 6
#endif

/** default rssi low threshold */
#define DEFAULT_RSSI_LOW_THRESHOLD 70
/** RSSI HYSTERSIS */
#define RSSI_HYSTERESIS 4
/** lowest rssi threshold */
#define LOWEST_RSSI_THRESHOLD 82
/** delta rssi */
#define DELTA_RSSI 10
#if CONFIG_SCAN_CHANNEL_GAP
/** Scan channel gap */
#define SCAN_CHANNEL_GAP_VALUE 50U
#endif
#if CONFIG_11AX
typedef MLAN_PACK_START struct _IEEEtypes_Extension_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Element id extension */
    t_u8 ext_id;
    /** payload */
    t_u8 data[];
} MLAN_PACK_END IEEEtypes_Extension_t, *pIEEEtypes_Extension_t;

typedef MLAN_PACK_START struct _IEEEtypes_HeMcsNss_t
{
    /** HE Rx MCS and NSS Set */
    t_u16 rx_mcs;
    /** HE Tx MCS and NSS Set*/
    t_u16 tx_mcs;
} MLAN_PACK_END IEEEtypes_HeMcsNss_t, *pIEEEtypes_HeMcsNss_t;

typedef MLAN_PACK_START struct _IEEEtypes_HECap_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Element id extension */
    t_u8 ext_id;
    /** he mac capability info */
    t_u8 he_mac_cap[6];
    /** he phy capability info */
    t_u8 he_phy_cap[11];
    /** he txrx mcs support , size would be 4 or 8 or 12 */
    t_u8 he_txrx_mcs_support[4];
    /** PPE Thresholds (optional) */
} MLAN_PACK_END IEEEtypes_HECap_t, *pIEEEtypes_HECap_t;

typedef MLAN_PACK_START struct _IEEEtypes_HeOpParam_t {
    /** Default PE Duration */
    t_u16 default_pe_dur : 3;                       /* bit 0-2 */
    /** TWT Required */
    t_u16 twt_req : 1;                              /* bit 3 */
    /** TXOP Duration RTS Threshold */
    t_u16 txop_dur_rts_threshold : 10;              /* bit 4-13 */
    /** VHT Operation Info Present */
    t_u16 vht_op_info_present : 1;                  /* bit 14 */
    /** Co-Hosted BSS */
    t_u16 co_located_bss : 1;                       /* bit 15 */
    /** ER SU Disable */
    t_u8  er_su_disable : 1;                        /* bit 16 */
    /* 6g operation info present */
    t_u8  he_6g_op_info_present :1;                 /* bit 17 */
    /** Reserved bit 18-23 */
    t_u8  reserved : 6;                             /* bit 18-23 */
} MLAN_PACK_END IEEEtypes_HeOpParam_t;

typedef MLAN_PACK_START struct _IEEEtypes_HeBssColorInfo_t {
    /** BSS Color */
    t_u8 bss_color : 6;                             /* bit 0-5 */
    /** Partial BSS Color */
    t_u8 partial_bss_color : 1;                     /* bit 6 */
    /** BSS Color Disabled */
    t_u8 bss_color_disabled : 1;                    /* bit 7 */
} MLAN_PACK_END IEEEtypes_HeBssColorInfo_t;

typedef MLAN_PACK_START struct _IEEEtypes_HeMcsMap_t {
    /** Max HE-MAC for 1 SS */
    t_u8 max_mcs_1ss: 2;
    /** Max HE-MAC for 2 SS */
    t_u8 max_mcs_2ss: 2;
    /** Max HE-MAC for 3 SS */
    t_u8 max_mcs_3ss: 2;
    /** Max HE-MAC for 4 SS */
    t_u8 max_mcs_4ss: 2;
    /** Max HE-MAC for 5 SS */
    t_u8 max_mcs_5ss: 2;
    /** Max HE-MAC for 6 SS */
    t_u8 max_mcs_6ss: 2;
    /** Max HE-MAC for 7 SS */
    t_u8 max_mcs_7ss: 2;
    /** Max HE-MAC for 8 SS */
    t_u8 max_mcs_8ss: 2;
} MLAN_PACK_END IEEEtypes_HeMcsMap_t, *pIEEEtypes_HeMcsMap_t;

typedef MLAN_PACK_START struct _IEEEtypes_HeOp_t {
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Element id extension */
    t_u8 ext_id;
    /** HE Operation Parameters */
    IEEEtypes_HeOpParam_t           he_op_param;
    /** BSS Color Info */
    IEEEtypes_HeBssColorInfo_t      bss_color_info;
    /** Basic HE-MCS and NSS Set */
    IEEEtypes_HeMcsMap_t            basic_he_mcs_nss;
    /** Optional Field, including VHT Operation Info Max Co-Hosted BSSID Indicator, and 6Ghz Operation Info  */
    t_u8 option[9];
} MLAN_PACK_END IEEEtypes_HeOp_t;
#endif

/** MBO IE header */
#define MBO_IE_HEADER_LEN 6U

/** MBO attribute header */
#define MBO_ATTR_HEADER_LEN 2

/*
*****************************************************************************
**
**
**                     802.11k RRM definitions
**
**
*****************************************************************************
*/
#if CONFIG_11K
typedef MLAN_PACK_START struct _IEEEtypes_RrmEnabledCapabilities_t
{
    /* First byte */
    t_u8 LinkMeas : 1;
    t_u8 NborRpt : 1;
    t_u8 ParallelMeas : 1;
    t_u8 RepeatMeas : 1;
    t_u8 BcnPassiveMeas : 1;
    t_u8 BcnActiveMeas : 1;
    t_u8 BcnTableMeas : 1;
    t_u8 BcnMeasRptCond : 1;

    /* Second byte */
    t_u8 FrameMeas : 1;
    t_u8 ChanLoadMeas : 1;
    t_u8 NoiseHistMeas : 1;
    t_u8 StatsMeas : 1;
    t_u8 LciMeas : 1;
    t_u8 LciAzimuth : 1;
    t_u8 TxStreamMeas : 1;
    t_u8 TrigTxStreamMeas : 1;

    /* Third byte */
    t_u8 ApChanRpt : 1;
    t_u8 RrmMib : 1;
    t_u8 OpChanMaxMeas : 3;
    t_u8 NonOpChanMaxMeas : 3;

    /* Fourth byte */
    t_u8 MeasPilot : 3;
    t_u8 MeasPilotTxInfo : 1;
    t_u8 NborRptTsfOffset : 1;
    t_u8 RcpiMeas : 1;
    t_u8 RsniMeas : 1;
    t_u8 BssAvgAccessDelay : 1;

    /* Fifth byte */
    t_u8 BssAvailAdmCap : 1;
    t_u8 AntennaInfo : 1;
    t_u8 FtmRangeReport : 1;
    t_u8 CivicLocation : 1;
    t_u8 Reserved : 4;
} MLAN_PACK_END IEEEtypes_RrmEnabledCapabilities_t, *pIEEEtypes_RrmEnabledCapabilities_t;

typedef MLAN_PACK_START struct _IEEEtypes_RrmElement_t
{
    /** Element ID */
    t_u8 element_id;
    /** Length */
    t_u8 len;

    IEEEtypes_RrmEnabledCapabilities_t RrmEnabledCapabilities;
} MLAN_PACK_END IEEEtypes_RrmElement_t, *pIEEEtypes_RrmElement_t;

/** Mobility Domain element */
typedef MLAN_PACK_START struct _MrvlIETypes_MobDomain_t
{
    /** Header */
    IEEEtypes_Header_t header;
    /** Mobility Domain Identifier */
    t_u16 mob_domain_id;
    /** FT Capability and Policy */
    t_u8 ft_cap_policy;
} MLAN_PACK_END MrvlIETypes_MobDomain_t;

typedef MLAN_PACK_START enum _IEEEtypes_RRM_ActionFieldType_e {
    IEEE_MGMT_RRM_RADIO_MEASUREMENT_REQUEST = 0,
    IEEE_MGMT_RRM_RADIO_MEASUREMENT_REPORT  = 1,
    IEEE_MGMT_RRM_LINK_MEASUREMENT_REQUEST  = 2,
    IEEE_MGMT_RRM_LINK_MEASUREMENT_REPORT   = 3,
    IEEE_MGMT_RRM_NEIGHBOR_REPORT_REQUEST   = 4,
    IEEE_MGMT_RRM_NEIGHBOR_REPORT_RESPONSE  = 5
} MLAN_PACK_END IEEEtypes_RRM_ActionFieldType_e;
#endif

#if CONFIG_BG_SCAN
/**
 *  Input structure to configure bs scan cmd to firmware
 */
typedef MLAN_PACK_START struct
{
    /** action */
    t_u16 action;
    /** enable/disable */
    t_u8 enable;
    /**  BSS type:
     *   MLAN_SCAN_MODE_BSS  (infrastructure)
     *   MLAN_SCAN_MODE_IBSS (adhoc)
     *   MLAN_SCAN_MODE_ANY  (unrestricted, adhoc and infrastructure)
     */
    t_u8 bss_type;
    /** number of channel scanned during each scan */
    t_u8 chan_per_scan;
    /** interval between consecutive scan */
    t_u32 scan_interval;
    /** bit 0: ssid match bit 1: ssid match and SNR exceeded
     *  bit 2: ssid match and RSSI exceeded
     *  bit 31: wait for all channel scan to complete to report scan result
     */
    t_u32 report_condition;
    /* Configure the number of probe requests for active chan scans */
    t_u8 num_probes;
    /** RSSI threshold */
    t_u8 rssi_threshold;
    /** SNR threshold */
    t_u8 snr_threshold;
    /** repeat count */
    t_u16 repeat_count;
    /** start later flag */
    t_u16 start_later;
    /** SSID filter list used in the to limit the scan results */
    wlan_user_scan_ssid ssid_list[MRVDRV_MAX_SSID_LIST_LENGTH];
    /** Variable number (fixed maximum) of channels to scan up */
    wlan_user_scan_chan chan_list[WLAN_BG_SCAN_CHAN_MAX];
#if CONFIG_SCAN_CHANNEL_GAP
    /** scan channel gap */
    t_u16 scan_chan_gap;
#endif
} MLAN_PACK_END wlan_bgscan_cfg;
#endif /* ROAMING */
#endif /* STA_SUPPORT */

#ifdef PRAGMA_PACK
#pragma pack(pop)
#endif

#if (CONFIG_11R) || (CONFIG_11K)
/** Mobility domain IE */
typedef MLAN_PACK_START struct _IEEEtypes_MobilityDomain_t
{
    /** Generic IE header */
    IEEEtypes_Header_t ieee_hdr;
    /** Mobility Domain ID */
    t_u16 mdid;
    /** FT Capability policy */
    t_u8 ft_cap;
} MLAN_PACK_END IEEEtypes_MobilityDomain_t;
#endif

/** BSSDescriptor_t
 *    Structure used to store information for beacon/probe response
 */
typedef struct _BSSDescriptor_t
{
    /** MAC address */
    mlan_802_11_mac_addr mac_address;

    /** SSID */
    mlan_802_11_ssid ssid;

    /** Transition MAC address */
    mlan_802_11_mac_addr trans_mac_address;

    /** Transition SSID */
    mlan_802_11_ssid trans_ssid;

#if CONFIG_DRIVER_OWE
    /** OWE Transition mode */
    t_u8 owe_transition_mode;
#endif

    /** WEP encryption requirement */
    t_u32 privacy;

    /** Receive signal strength in dBm */
    t_s32 rssi;

#if CONFIG_SCAN_CHANNEL_GAP
    /** channel load */
    t_u16 chan_load;
    /** channel load */
    t_u16 chan_noise;
#endif

    /** Channel */
    t_u8 channel;

    /** Freq */
    t_u32 freq;

    /** Beacon period */
    t_u16 beacon_period;

    /** DTIM period */
    t_u16 dtim_period;

    /** ATIM window */
    t_u32 atim_window;

    /** ERP flags */
    t_u8 erp_flags;

    /** Type of network in use */
    WLAN_802_11_NETWORK_TYPE network_type_use;

    /** Network infrastructure mode */
    mlan_bss_mode bss_mode;

    /** Network supported rates */
    WLAN_802_11_RATES supported_rates;

    /** Supported data rates */
    t_u8 data_rates[WLAN_SUPPORTED_RATES];

    /** Current channel bandwidth
     *  0 : 20MHZ
     *  1 : 40MHZ
     *  2 : 80MHZ
     *  3 : 160MHZ
     */
    t_u8 curr_bandwidth;

    /** Network band.
     * BAND_B(0x01): 'b' band
     * BAND_G(0x02): 'g' band
     * BAND_A(0X04): 'a' band
     */
    t_u16 bss_band;

    /** TSF record at driver receive the scan result from FW (in us) */
    unsigned int scan_result_tsf;

    /** TSF timestamp from the current firmware TSF */
    t_u64 network_tsf;

    /** TSF value included in the beacon/probe response */
    t_u8 time_stamp[8];

    /** PHY parameter set */
    IEEEtypes_PhyParamSet_t phy_param_set;

    /** SS parameter set */
    IEEEtypes_SsParamSet_t ss_param_set;

    /** Capability information */
    IEEEtypes_CapInfo_t cap_info;

    /** WMM IE */
    IEEEtypes_WmmParameter_t wmm_ie;

    /** 802.11h BSS information */

    /** Indicate disabling 11n when associate with AP */
    t_u8 disable_11n;
    /** 802.11n BSS information */
    /** HT Capabilities IE */
    IEEEtypes_HTCap_t *pht_cap;
    /** HT Capabilities Offset */
    /* t_u16 ht_cap_offset; */
    /** HT Information IE */
    IEEEtypes_HTInfo_t *pht_info;
    /** HT Information Offset */
    /* t_u16 ht_info_offset; */
#if CONFIG_MULTI_BSSID_SUPPORT
    /** Flag to indicate this is multi_bssid_ap */
    t_u8 multi_bssid_ap;
    /** the mac address of multi-bssid AP */
    mlan_802_11_mac_addr multi_bssid_ap_addr;
#endif
    /** 20/40 BSS Coexistence IE */
    IEEEtypes_2040BSSCo_t *pbss_co_2040;
    /** VHT Capabilities IE */
    IEEEtypes_VHTCap_t *pvht_cap;
    /** VHT Capabilities IE offset */
    /** VHT Operations IE */
    IEEEtypes_VHTOprat_t *pvht_oprat;
    /** VHT Operations IE offset */
    /** VHT Transmit Power Envelope IE */
    IEEEtypes_VHTtxpower_t *pvht_txpower;
    /** Operating Mode Notification IE */
    IEEEtypes_OperModeNtf_t *ppoper_mode;
    /** 20/40 BSS Coexistence Offset */
    t_u16 bss_co_2040_offset;
    /** Extended Capabilities IE */
    IEEEtypes_ExtCap_t *pext_cap;
    /** Extended Capabilities Offset */
    t_u16 ext_cap_offset;
    /** Overlapping BSS Scan Parameters IE */
    IEEEtypes_OverlapBSSScanParam_t *poverlap_bss_scan_param;
    /** Overlapping BSS Scan Parameters Offset */
    t_u16 overlap_bss_offset;

#ifdef STA_SUPPORT
    /** Country information set */
    IEEEtypes_CountryInfoFullSet_t country_info;
#endif /* STA_SUPPORT */

    /** WPA IE */
    IEEEtypes_VendorSpecific_t *pwpa_ie;
    /** WPA IE offset in the beacon buffer */
    t_u16 wpa_offset;
    /** RSN IE */
    IEEEtypes_Generic_t *prsn_ie;
    /** RSN IE offset in the beacon buffer */
    t_u16 rsn_offset;
#ifdef STA_SUPPORT
    /** WAPI IE */
    IEEEtypes_Generic_t *pwapi_ie;
    /** WAPI IE offset in the beacon buffer */
    t_u16 wapi_offset;
#endif

    /** Pointer to the returned scan response */
    t_u8 *pbeacon_buf;
    /** Length of the stored scan response */
    t_u32 beacon_buf_size;
    /** Max allocated size for updated scan response */
    t_u32 beacon_buf_size_max;

#if CONFIG_WPA_SUPP
    /** Pointer to the returned scan response */
    t_u8 *ies;
    /** Length of the stored scan response */
    t_u32 ies_len;
#endif

    /* Added for WMSDK */

    /*
     * Since we do not save the beacon buffer because it is too huge
     * for us we need to save selected data structures for later
     * perusal.
     */
    IEEEtypes_HTCap_t ht_cap_saved;
    IEEEtypes_HTInfo_t ht_info_saved;
    IEEEtypes_2040BSSCo_t bss_co_2040_saved;

#if CONFIG_11AC
    IEEEtypes_VHTCap_t vht_cap_saved;
    IEEEtypes_VHTOprat_t vht_oprat_saved;
    IEEEtypes_VHTtxpower_t vht_txpower_saved;
    IEEEtypes_OperModeNtf_t poper_mode_saved;
#endif
    IEEEtypes_ExtCap_t ext_cap_saved;
    bool ext_cap_exist;
#if CONFIG_11AX
    /** HE Capability IE */
    IEEEtypes_HECap_t *phe_cap;
    /** HE Capability IE offset */
    t_u16 he_cap_offset;
    /** HE operation IE */
    IEEEtypes_HeOp_t *phe_oprat;
    /** HE operation IE offset */
    t_u16 he_oprat_offset;
    IEEEtypes_HECap_t he_cap_saved;
    IEEEtypes_HeOp_t he_oprat_saved;
#endif
    /*
      fixme: The legacy code used IEEEtypes_RSN_IE_t which is of 24
      bytes. There seems to be confusion about the exact structure to
      use in this situation. Seems that 256 bytes are used by
      mlan_private (wpa_ie). We cannot use that much here. Lets take a
      reasonable amount.
    */
    unsigned char wpa_ie_buff[MLAN_WMSDK_MAX_WPA_IE_LEN];
    size_t wpa_ie_buff_len;
    unsigned char rsn_ie_buff[MLAN_WMSDK_MAX_WPA_IE_LEN];
    size_t rsn_ie_buff_len;

    bool wps_IE_exist;
    t_u16 wps_session;

    bool wpa2_entp_IE_exist;
    /** RSNX IE */
    IEEEtypes_Rsnx_t *prsnx_ie;
    IEEEtypes_Rsnx_t rsnx_ie_saved;
    /** RSNX IE offset in the beacon buffer */
    t_u16 rsnx_offset;

    bool brcm_ie_exist;
    bool epigram_ie_exist;
#if (CONFIG_11R) || (CONFIG_11K)
    unsigned char md_ie_buff[MLAN_MAX_MDIE_LEN];
    size_t md_ie_buff_len;
    /* Mobility domain IE */
    IEEEtypes_MobilityDomain_t *pmd_ie;
    bool mob_domain_exist;
#endif
#if CONFIG_11K
    bool rm_cap_exist;
    IEEEtypes_RrmElement_t rm_cap_saved;
    unsigned char vendor_ie_buff[MLAN_MAX_VENDOR_IE_LEN];
    t_u8 vendor_ie_len;
    bool neighbor_report_supported;
#endif
#if CONFIG_11V
    bool bss_transition_supported;
#endif
#if CONFIG_DRIVER_MBO
    bool mbo_assoc_disallowed;
#endif
} BSSDescriptor_t, *pBSSDescriptor_t;

#endif /* !_MLAN_IEEE_H_ */
