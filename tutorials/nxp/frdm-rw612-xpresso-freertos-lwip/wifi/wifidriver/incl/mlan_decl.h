/** @file mlan_decl.h
 *
 *  @brief This file declares the generic data structures and APIs.
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

#ifndef _MLAN_DECL_H_
#define _MLAN_DECL_H_

#include "type_decls.h"
#include <osa.h>
#if CONFIG_WPA_SUPP
#include <ieee802_11_defs.h>
#endif

/** MLAN release version */
#define MLAN_RELEASE_VERSION "310"

/** Constants below */

#ifdef __GNUC__
/** Structure packing begins */
#define MLAN_PACK_START
/** Structure packeing end */
#define MLAN_PACK_END __attribute__((packed))
#else /* !__GNUC__ */
#ifdef PRAGMA_PACK
/** Structure packing begins */
#define MLAN_PACK_START
/** Structure packeing end */
#define MLAN_PACK_END
#else /* !PRAGMA_PACK */
/** Structure packing begins */
#define MLAN_PACK_START __packed
/** Structure packing end */
#define MLAN_PACK_END
#endif /* PRAGMA_PACK */
#endif /* __GNUC__ */

#ifndef INLINE
#ifdef __GNUC__
/** inline directive */
#define INLINE inline
#else
/** inline directive */
#define INLINE __inline
#endif
#endif

/** MLAN TRUE */
#define MTRUE (1)
/** MLAN FALSE */
#define MFALSE (0)

/** Macros for Data Alignment : size */
#define ALIGN_SZ(p, a) (((p) + ((a)-1U)) & ~((a)-1U))

/** Macros for Data Alignment : address */
#define ALIGN_ADDR(p, a) ((((t_ptr)(p)) + (((t_ptr)(a)) - 1U)) & ~(((t_ptr)(a)) - 1U))

#ifndef MACSTR
/** MAC address security format */
#define MACSTR "%02x:XX:XX:XX:%02x:%02x"
#endif

#ifndef MAC2STR
/** MAC address security print arguments */
#define MAC2STR(a) (a)[0], (a)[4], (a)[5]
#endif

/** Return the byte offset of a field in the given structure */
#define MLAN_FIELD_OFFSET(type, field) ((t_u32)(t_ptr) & (((type *)0)->field))
/** Return aligned offset */
#define OFFSET_ALIGN_ADDR(p, a) (t_u32)(ALIGN_ADDR(p, a) - (t_ptr)p)

/** Maximum BSS numbers */
/* fixme: We have reduced this from 16 to 2. Ensure that this is Ok */
#if UAP_SUPPORT
#define MLAN_MAX_BSS_NUM 2U
#else
#define MLAN_MAX_BSS_NUM 1U
#endif

/** NET IP alignment */
#define MLAN_NET_IP_ALIGN 0

/** DMA alignment */
#define DMA_ALIGNMENT 32U
/** max size of TxPD */
#define MAX_TXPD_SIZE 32

/** Minimum data header length */
#define MLAN_MIN_DATA_HEADER_LEN (DMA_ALIGNMENT + MAX_TXPD_SIZE)

/** rx data header length */
#define MLAN_RX_HEADER_LEN MLAN_MIN_DATA_HEADER_LEN

/** This is current limit on Maximum Tx AMPDU allowed */
#define MLAN_MAX_TX_BASTREAM_SUPPORTED 2U
/** This is current limit on Maximum Rx AMPDU allowed */
#define MLAN_MAX_RX_BASTREAM_SUPPORTED 16

/** US country code */
#define COUNTRY_CODE_US 0x10

#ifdef STA_SUPPORT
/** Default Win size attached during ADDBA request */
#ifndef MLAN_STA_AMPDU_DEF_TXWINSIZE
#if (defined(SD9177) || defined(IW610)) && defined(COEX_APP_SUPPORT)
#define MLAN_STA_AMPDU_DEF_TXWINSIZE 32
#else
#define MLAN_STA_AMPDU_DEF_TXWINSIZE 64
#endif
#endif

/** Default Win size attached during ADDBA response */
#ifndef MLAN_STA_AMPDU_DEF_RXWINSIZE
#if (defined(SD9177) || defined(IW610)) && !defined(COEX_APP_SUPPORT)
#define MLAN_STA_AMPDU_DEF_RXWINSIZE 64
#else
#define MLAN_STA_AMPDU_DEF_RXWINSIZE 32
#endif
#endif
#endif /* STA_SUPPORT */

#if UAP_SUPPORT
/** Default Win size attached during ADDBA request */
#ifndef MLAN_UAP_AMPDU_DEF_TXWINSIZE
#if (defined(SD9177) || defined(IW610)) && defined(COEX_APP_SUPPORT)
#define MLAN_UAP_AMPDU_DEF_TXWINSIZE 32
#else
#define MLAN_UAP_AMPDU_DEF_TXWINSIZE 64
#endif
#endif

/** Default Win size attached during ADDBA response */
#ifndef MLAN_UAP_AMPDU_DEF_RXWINSIZE
#if (defined(SD9177) || defined(IW610)) && !defined(COEX_APP_SUPPORT)
#define MLAN_UAP_AMPDU_DEF_RXWINSIZE 64
#else
#define MLAN_UAP_AMPDU_DEF_RXWINSIZE 32
#endif
#endif

#endif /* UAP_SUPPORT */

/** Block ack timeout value */
#define MLAN_DEFAULT_BLOCK_ACK_TIMEOUT 0U
/** Maximum Tx Win size configured for ADDBA request [10 bits] */
#define MLAN_AMPDU_MAX_TXWINSIZE 0x3ff
/** Maximum Rx Win size configured for ADDBA request [10 bits] */
#define MLAN_AMPDU_MAX_RXWINSIZE 0x3ff

/** Rate index for HR/DSSS 0 */
#define MLAN_RATE_INDEX_HRDSSS0 0
/** Rate index for HR/DSSS 3 */
#define MLAN_RATE_INDEX_HRDSSS3 3
/** Rate index for OFDM 0 */
#define MLAN_RATE_INDEX_OFDM0 4U
/** Rate index for OFDM 7 */
#define MLAN_RATE_INDEX_OFDM7 11
/** Rate index for MCS 0 */
#define MLAN_RATE_INDEX_MCS0 0U
/** Rate index for MCS 2 */
#define MLAN_RATE_INDEX_MCS2 2U
/** Rate index for MCS 4 */
#define MLAN_RATE_INDEX_MCS4 4U
/** Rate index for MCS 7 */
#define MLAN_RATE_INDEX_MCS7 7U
/** Rate index for MCS 8 */
#define MLAN_RATE_INDEX_MCS8 8U
/** Rate index for MCS 9 */
#define MLAN_RATE_INDEX_MCS9 9U
#if CONFIG_11AX
/** Rate index for MCS11 */
#define MLAN_RATE_INDEX_MCS11 11U
#endif
/** Rate index for MCS 32 */
#define MLAN_RATE_INDEX_MCS32 32U
/** Rate index for MCS 127 */
#define MLAN_RATE_INDEX_MCS127 127U
#if (CONFIG_11AC) || (CONFIG_11AX)
#define MLAN_RATE_NSS1 1
#define MLAN_RATE_NSS2 2
#endif

/** Rate bitmap for OFDM 0 */
#define MLAN_RATE_BITMAP_OFDM0 16
/** Rate bitmap for OFDM 7 */
#define MLAN_RATE_BITMAP_OFDM7 23
/** Rate bitmap for MCS 0 */
#define MLAN_RATE_BITMAP_MCS0 32U
/** Rate bitmap for MCS 127 */
#define MLAN_RATE_BITMAP_MCS127 159
#if CONFIG_11AC
#define MLAN_RATE_BITMAP_NSS1_MCS0 160
#define MLAN_RATE_BITMAP_NSS1_MCS9 169
#define MLAN_RATE_BITMAP_NSS2_MCS0 176
#define MLAN_RATE_BITMAP_NSS2_MCS9 185
#endif

/** MU beamformer */
#define DEFALUT_11AC_CAP_BEAMFORMING_RESET_MASK (MBIT(19))
#ifdef RW610
/** Short GI for 80MHz/TVHT_MODE_4C */
#define DEFALUT_11AC_CAP_SHORTGI_80MHZ_RESET_MASK (MBIT(5))
/** HE Phy Cap Info(40MHz in 2.4GHz band) */
#define DEFAULT_11AX_CAP_40MHZIH2_4GHZBAND_RESET_MASK (MBIT(1))
#endif

/** Size of rx data buffer */
#define MLAN_RX_DATA_BUF_SIZE (4 * 1024)
/** Size of rx command buffer */
#define MLAN_RX_CMD_BUF_SIZE (2 * 1024)

/** MLAN MAC Address Length */
#define MLAN_MAC_ADDR_LENGTH (6U)
/** MLAN 802.11 MAC Address */
typedef t_u8 mlan_802_11_mac_addr[MLAN_MAC_ADDR_LENGTH];

/** MLAN Maximum SSID Length */
#define MLAN_MAX_SSID_LENGTH (32U)

/** RTS/FRAG related defines */
/** Minimum RTS value */
#define MLAN_RTS_MIN_VALUE (0)
/** Maximum RTS value */
#define MLAN_RTS_MAX_VALUE (2347)
/** Minimum FRAG value */
#define MLAN_FRAG_MIN_VALUE (256)
/** Maximum FRAG value */
#define MLAN_FRAG_MAX_VALUE (2346)

/** Minimum tx retry count */
#define MLAN_TX_RETRY_MIN (0)
/** Maximum tx retry count */
#define MLAN_TX_RETRY_MAX (14)

/** define SDIO block size for data Tx/Rx */
/* We support up to 480-byte block size due to FW buffer limitation. */
#define MLAN_SDIO_BLOCK_SIZE 256U

/** define SDIO block size for firmware download */
#define MLAN_SDIO_BLOCK_SIZE_FW_DNLD MLAN_SDIO_BLOCK_SIZE

/** define allocated buffer size */
#define ALLOC_BUF_SIZE (4 * 1024)

/** SDIO IO Port mask */
#define MLAN_SDIO_IO_PORT_MASK 0xfffff
/** SDIO Block/Byte mode mask */
#define MLAN_SDIO_BYTE_MODE_MASK 0x80000000U

/** Max retry number of IO write */
#define MAX_READ_IOMEM_RETRY 2

/** IN parameter */
#define IN
/** OUT parameter */
#define OUT

/** BIT value */
#define MBIT(x) (((t_u32)1) << (x))

#define MRVL_PKT_TYPE_MGMT_FRAME 0xE5

/** Buffer flag for requeued packet */
#define MLAN_BUF_FLAG_REQUEUED_PKT MBIT(0)
/** Buffer flag for transmit buf from moal */
#define MLAN_BUF_FLAG_MOAL_TX_BUF MBIT(1)
/** Buffer flag for malloc mlan_buffer */
#define MLAN_BUF_FLAG_MALLOC_BUF MBIT(2)

/** Buffer flag for bridge packet */
#define MLAN_BUF_FLAG_BRIDGE_BUF MBIT(3)

/** Buffer flag for TX_STATUS */
#define MLAN_BUF_FLAG_TX_STATUS MBIT(10)

#ifdef DEBUG_LEVEL1
/** Debug level bit definition */
#define MMSG   MBIT(0)
#define MFATAL MBIT(1)
#define MERROR MBIT(2)
#define MDATA  MBIT(3)
#define MCMND  MBIT(4)
#define MEVENT MBIT(5)
#define MINTR  MBIT(6)
#define MIOCTL MBIT(7)

#define MDAT_D MBIT(16)
#define MCMD_D MBIT(17)
#define MEVT_D MBIT(18)
#define MFW_D  MBIT(19)
#define MIF_D  MBIT(20)

#define MENTRY    MBIT(28)
#define MWARN     MBIT(29)
#define MINFO     MBIT(30)
#define MHEX_DUMP MBIT(31)
#endif /* DEBUG_LEVEL1 */

/** Memory allocation type: DMA */
#define MLAN_MEM_DMA MBIT(0U)

/** Default memory allocation flag */
#define MLAN_MEM_DEF 0U

#if CONFIG_WIFI_IND_DNLD
/** driver initial the fw reset */
#define FW_RELOAD_SDIO_INBAND_RESET 1
/** out band reset trigger reset, no interface re-emulation */
#define FW_RELOAD_NO_EMULATION 2
/** out band reset with interface re-emulation */
#define FW_RELOAD_WITH_EMULATION 3
/** sdio hw reset */
#define FW_RELOAD_SDIO_HW_RESET 5
#endif

/** MrvlExtIEtypesHeader_t */
typedef MLAN_PACK_START struct _MrvlExtIEtypesHeader
{
    /** Header type */
    t_u16 type;
    /** Header length */
    t_u16 len;
    /** ext id */
    t_u8 ext_id;
} MLAN_PACK_END MrvlExtIEtypesHeader_t;

/** MrvlIEtypes_Data_t */
typedef MLAN_PACK_START struct _MrvlExtIEtypes_Data_t
{
    /** Header */
    MrvlExtIEtypesHeader_t header;
    /** Data */
    t_u8 data[];
} MLAN_PACK_END MrvlExtIEtypes_Data_t;

/** mlan_status */
typedef enum _mlan_status
{
    MLAN_STATUS_FAILURE = 0xffffffff,
    MLAN_STATUS_SUCCESS = 0,
    MLAN_STATUS_PENDING,
    MLAN_STATUS_RESOURCE,
} mlan_status;

/** mlan_error_code */
typedef enum _mlan_error_code
{
    /** No error */
    MLAN_ERROR_NO_ERROR = 0,
    /** Firmware/device errors below (MSB=0) */
    MLAN_ERROR_FW_NOT_READY = 0x00000001,
    MLAN_ERROR_FW_BUSY,
    MLAN_ERROR_FW_CMDRESP,
    MLAN_ERROR_DATA_TX_FAIL,
    MLAN_ERROR_DATA_RX_FAIL,
    /** Driver errors below (MSB=1) */
    MLAN_ERROR_PKT_SIZE_INVALID = 0x80000001,
    MLAN_ERROR_PKT_TIMEOUT,
    MLAN_ERROR_PKT_INVALID,
    MLAN_ERROR_CMD_INVALID,
    MLAN_ERROR_CMD_TIMEOUT,
    MLAN_ERROR_CMD_DNLD_FAIL,
    MLAN_ERROR_CMD_CANCEL,
    MLAN_ERROR_CMD_RESP_FAIL,
    MLAN_ERROR_CMD_ASSOC_FAIL,
    MLAN_ERROR_CMD_SCAN_FAIL,
    MLAN_ERROR_IOCTL_INVALID,
    MLAN_ERROR_IOCTL_FAIL,
    MLAN_ERROR_EVENT_UNKNOWN,
    MLAN_ERROR_INVALID_PARAMETER,
    MLAN_ERROR_NO_MEM,
    /** More to add */
} mlan_error_code;

/** mlan_buf_type */
typedef enum _mlan_buf_type
{
    MLAN_BUF_TYPE_CMD = 1,
    MLAN_BUF_TYPE_DATA,
    MLAN_BUF_TYPE_EVENT,
    MLAN_BUF_TYPE_RAW_DATA,
} mlan_buf_type;

/** MLAN BSS type */
typedef enum _mlan_bss_type
{
    MLAN_BSS_TYPE_STA = 0,
    MLAN_BSS_TYPE_UAP = 1,
    /* fixme: This macro will be enabled when
     * mlan is completely integrated with wlan
     */
    /* #ifdef WIFI_DIRECT_SUPPORT*/
    MLAN_BSS_TYPE_WIFIDIRECT = 2,
    /*#endif*/
    MLAN_BSS_TYPE_ANY = 0xff,
} mlan_bss_type;

/** MLAN BSS role */
typedef enum _mlan_bss_role
{
    MLAN_BSS_ROLE_STA = 0,
    MLAN_BSS_ROLE_UAP = 1,
    MLAN_BSS_ROLE_ANY = 0xff,
} mlan_bss_role;

/** BSS role bit mask */
#define BSS_ROLE_BIT_MASK MBIT(0)

/** Get BSS role */
#define GET_BSS_ROLE(priv) ((unsigned)(priv)->bss_role & (BSS_ROLE_BIT_MASK))

/** mlan_data_frame_type */
typedef enum _mlan_data_frame_type
{
    MLAN_DATA_FRAME_TYPE_ETH_II = 0,
    MLAN_DATA_FRAME_TYPE_802_11,
} mlan_data_frame_type;

/** mlan_event_id */
typedef enum _mlan_event_id
{
    /* Event generated by firmware (MSB=0) */
    MLAN_EVENT_ID_FW_UNKNOWN = 0x00000001,
    MLAN_EVENT_ID_FW_ADHOC_LINK_SENSED,
    MLAN_EVENT_ID_FW_ADHOC_LINK_LOST,
    MLAN_EVENT_ID_FW_DISCONNECTED,
    MLAN_EVENT_ID_FW_MIC_ERR_UNI,
    MLAN_EVENT_ID_FW_MIC_ERR_MUL,
    MLAN_EVENT_ID_FW_BCN_RSSI_LOW,
    MLAN_EVENT_ID_FW_BCN_RSSI_HIGH,
    MLAN_EVENT_ID_FW_BCN_SNR_LOW,
    MLAN_EVENT_ID_FW_BCN_SNR_HIGH,
    MLAN_EVENT_ID_FW_MAX_FAIL,
    MLAN_EVENT_ID_FW_DATA_RSSI_LOW,
    MLAN_EVENT_ID_FW_DATA_RSSI_HIGH,
    MLAN_EVENT_ID_FW_DATA_SNR_LOW,
    MLAN_EVENT_ID_FW_DATA_SNR_HIGH,
    MLAN_EVENT_ID_FW_LINK_QUALITY,
    MLAN_EVENT_ID_FW_PORT_RELEASE,
    MLAN_EVENT_ID_FW_PRE_BCN_LOST,
    MLAN_EVENT_ID_FW_WMM_CONFIG_CHANGE,
    MLAN_EVENT_ID_FW_HS_WAKEUP,
    MLAN_EVENT_ID_FW_BG_SCAN,
    MLAN_EVENT_ID_FW_WEP_ICV_ERR,
    MLAN_EVENT_ID_FW_STOP_TX,
    MLAN_EVENT_ID_FW_START_TX,
    MLAN_EVENT_ID_FW_CHANNEL_SWITCH_ANN,
    MLAN_EVENT_ID_FW_RADAR_DETECTED,
    MLAN_EVENT_ID_FW_CHANNEL_REPORT_RDY,
    MLAN_EVENT_ID_FW_BW_CHANGED,
#if UAP_SUPPORT
    MLAN_EVENT_ID_UAP_FW_BSS_START,
    MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE,
    MLAN_EVENT_ID_UAP_FW_BSS_IDLE,
    MLAN_EVENT_ID_UAP_FW_STA_CONNECT,
    MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT,
#endif

    /* Event generated by MLAN driver (MSB=1) */
    MLAN_EVENT_ID_DRV_CONNECTED = 0x80000001,
    MLAN_EVENT_ID_DRV_DEFER_HANDLING,
    MLAN_EVENT_ID_DRV_HS_ACTIVATED,
    MLAN_EVENT_ID_DRV_HS_DEACTIVATED,
    MLAN_EVENT_ID_DRV_MGMT_FRAME,
    MLAN_EVENT_ID_DRV_OBSS_SCAN_PARAM,
    MLAN_EVENT_ID_DRV_PASSTHRU,
    MLAN_EVENT_ID_DRV_SCAN_REPORT,
    MLAN_EVENT_ID_DRV_MEAS_REPORT,
    MLAN_EVENT_ID_DRV_ASSOC_FAILURE_REPORT,
    MLAN_EVENT_ID_DRV_REPORT_STRING,
    MLAN_EVENT_ID_DRV_DBG_DUMP,
} mlan_event_id;

/** Data Structures */
/** mlan_image data structure */
typedef struct _mlan_fw_image
{
    /** Helper image buffer pointer */
    t_u8 *phelper_buf;
    /** Helper image length */
    t_u32 helper_len;
    /** Firmware image buffer pointer */
    t_u8 *pfw_buf;
    /** Firmware image length */
    t_u32 fw_len;
#if CONFIG_WIFI_IND_DNLD
    /** Firmware reload flag */
    t_u8 fw_reload;
#endif
} mlan_fw_image, *pmlan_fw_image;

/** Custom data structure */
typedef struct _mlan_init_param
{
    /** Cal data buffer pointer */
    t_u8 *pcal_data_buf;
    /** Cal data length */
    t_u32 cal_data_len;
    /** Other custom data */
} mlan_init_param, *pmlan_init_param;

/** Channel usability flags */
#define NXP_CHANNEL_NO_OFDM  MBIT(9)
#define NXP_CHANNEL_NO_CCK   MBIT(8)
#define NXP_CHANNEL_DISABLED MBIT(7)
/* BIT 5/6 resevered for FW */
#define NXP_CHANNEL_NOHT160 MBIT(4)
#define NXP_CHANNEL_NOHT80  MBIT(3)
#define NXP_CHANNEL_NOHT40  MBIT(2)
#define NXP_CHANNEL_DFS     MBIT(1)
#define NXP_CHANNEL_PASSIVE MBIT(0)

/** CFP dynamic (non-const) elements */
typedef struct _cfp_dyn_t
{
    /** extra flags to specify channel usability
     *  bit 9 : if set, channel is non-OFDM
     *  bit 8 : if set, channel is non-CCK
     *  bit 7 : if set, channel is disabled
     *  bit  5/6 resevered for FW
     *  bit 4 : if set, 160MHz on channel is disabled
     *  bit 3 : if set, 80MHz on channel is disabled
     *  bit 2 : if set, 40MHz on channel is disabled
     *  bit 1 : if set, channel is DFS channel
     *  bit 0 : if set, channel is passive
     */
    t_u16 flags;
    /** TRUE: Channel is blacklisted (do not use) */
    t_bool blacklist;
} cfp_dyn_t;

/** Chan-Freq-TxPower mapping table*/
typedef struct _chan_freq_power_t
{
    /** Channel Number */
    t_u16 channel;
    /** Frequency of this Channel */
    t_u32 freq;
    /** Max allowed Tx power level */
    t_u16 max_tx_power;
    /** TRUE:radar detect required for BAND A or passive scan for BAND B/G;
     * FALSE:radar detect not required for BAND A or active scan for BAND B/G*/
    t_bool passive_scan_or_radar_detect;
    /** Elements associated to cfp that change at run-time */
    cfp_dyn_t dynamic;
} chan_freq_power_t;

/** mlan_event data structure */
typedef struct _mlan_event
{
    /** BSS index number for multiple BSS support */
    t_u32 bss_index;
    /** Event ID */
    mlan_event_id event_id;
    /** Event length */
    t_u32 event_len;
    /** Event buffer */
    t_u8 event_buf[1];
} mlan_event, *pmlan_event;


#if CONFIG_EXT_SCAN_SUPPORT
/** mlan_event_scan_result data structure */
typedef MLAN_PACK_START struct _mlan_event_scan_result
{
    /** Event ID */
    t_u16 event_id;
    /** BSS index number for multiple BSS support */
    t_u8 bss_index;
    /** BSS type */
    t_u8 bss_type;
    /** More event available or not */
    t_u8 more_event;
    /** Reserved */
    t_u8 reserved[3];
    /** Size of the response buffer */
    t_u16 buf_size;
    /** Number of BSS in scan response */
    t_u8 num_of_set;
} MLAN_PACK_END mlan_event_scan_result, *pmlan_event_scan_result;
#endif

/** mlan_buffer data structure */
typedef struct _mlan_buffer
{
    /** Pointer to previous mlan_buffer */
    // struct _mlan_buffer *pprev;
    /** Pointer to next mlan_buffer */
    // struct _mlan_buffer *pnext;
    /** Status code from firmware/driver */
    t_u32 status_code;
    /** Flags for this buffer */
    t_u32 flags;
    /** BSS index number for multiple BSS support */
    t_u32 bss_index;
    /** Buffer descriptor, e.g. skb in Linux */
    t_void *pdesc;
    /** Private member added for WMSDK. This is used to store the lwip pbuf
 pointer */
    t_void *lwip_pbuf;
    /** Pointer to buffer */
    t_u8 *pbuf;
    /** Offset to data */
    t_u32 data_offset;
    /** Data length */
    t_u32 data_len;
    /** Buffer type: data, cmd, event etc. */
    mlan_buf_type buf_type;

    /** Fields below are valid for data packet only */
    /** QoS priority */
    t_u32 priority;
    /** Time stamp when packet is received (seconds) */
    // t_u32 in_ts_sec;
    /** Time stamp when packet is received (micro seconds) */
    // t_u32 in_ts_usec;
    /** Time stamp when packet is processed (seconds) */
    // t_u32 out_ts_sec;
    /** Time stamp when packet is processed (micro seconds) */
    // t_u32 out_ts_usec;

    /** Fields below are valid for MLAN module only */
    /** Pointer to parent mlan_buffer */
    // struct _mlan_buffer *pparent;
    /** Use count for this buffer */
    t_u32 use_count;
} mlan_buffer, *pmlan_buffer;

/** mlan_bss_attr data structure */
typedef struct _mlan_bss_attr
{
    /** BSS type */
    mlan_bss_type bss_type;
    /** Data frame type: Ethernet II, 802.11, etc. */
    t_u32 frame_type;
    /** The BSS is active (non-0) or not (0). */
    t_u32 active;
    /** BSS Priority */
    t_u32 bss_priority;
    /** BSS number */
    t_u32 bss_num;
} mlan_bss_attr, *pmlan_bss_attr;

#ifdef PRAGMA_PACK
#pragma pack(push, 1)
#endif

/** Type enumeration for the command result */
typedef MLAN_PACK_START enum _mlan_cmd_result_e {
    MLAN_CMD_RESULT_SUCCESS      = 0,
    MLAN_CMD_RESULT_FAILURE      = 1,
    MLAN_CMD_RESULT_TIMEOUT      = 2,
    MLAN_CMD_RESULT_INVALID_DATA = 3
} MLAN_PACK_END mlan_cmd_result_e;

#define WMM_AC_BK 0
#define WMM_AC_BE 1
#define WMM_AC_VI 2
#define WMM_AC_VO 3

typedef t_u8 mlan_wmm_ac_e;

/** Type enumeration for the action field in the Queue Config command */
typedef MLAN_PACK_START enum _mlan_wmm_queue_config_action_e {
    MLAN_WMM_QUEUE_CONFIG_ACTION_GET     = 0,
    MLAN_WMM_QUEUE_CONFIG_ACTION_SET     = 1,
    MLAN_WMM_QUEUE_CONFIG_ACTION_DEFAULT = 2,
    MLAN_WMM_QUEUE_CONFIG_ACTION_MAX
} MLAN_PACK_END mlan_wmm_queue_config_action_e;

/** Type enumeration for the action field in the queue stats command */
typedef MLAN_PACK_START enum _mlan_wmm_queue_stats_action_e {
    MLAN_WMM_STATS_ACTION_START   = 0,
    MLAN_WMM_STATS_ACTION_STOP    = 1,
    MLAN_WMM_STATS_ACTION_GET_CLR = 2,
    MLAN_WMM_STATS_ACTION_SET_CFG = 3, /* Not currently used */
    MLAN_WMM_STATS_ACTION_GET_CFG = 4, /* Not currently used */
    MLAN_WMM_STATS_ACTION_MAX
} MLAN_PACK_END mlan_wmm_queue_stats_action_e;

/**
 *  @brief IOCTL structure for a Traffic stream status.
 *
 */
typedef MLAN_PACK_START struct
{
    /** TSID: Range: 0->7 */
    t_u8 tid;
    /** TSID specified is valid */
    t_u8 valid;
    /** AC TSID is active on */
    t_u8 access_category;
    /** UP specified for the TSID */
    t_u8 user_priority;
    /** Power save mode for TSID: 0 (legacy), 1 (UAPSD) */
    t_u8 psb;
    /** Upstream(0), Downlink(1), Bidirectional(3) */
    t_u8 flow_dir;
    /** Medium time granted for the TSID */
    t_u16 medium_time;
} MLAN_PACK_END wlan_ioctl_wmm_ts_status_t,
    /** Type definition of mlan_ds_wmm_ts_status for MLAN_OID_WMM_CFG_TS_STATUS */
    mlan_ds_wmm_ts_status, *pmlan_ds_wmm_ts_status;

/** Max Ie length */
#define MAX_IE_SIZE 256U

/** custom IE */
typedef MLAN_PACK_START struct _custom_ie
{
    /** IE Index */
    t_u16 ie_index;
    /** Mgmt Subtype Mask */
    t_u16 mgmt_subtype_mask;
    /** IE Length */
    t_u16 ie_length;
    /** IE buffer */
    t_u8 ie_buffer[MAX_IE_SIZE];
} MLAN_PACK_END custom_ie;

/** TLV buffer : custom IE */
typedef MLAN_PACK_START struct _tlvbuf_custom_ie
{
    /** Tag */
    t_u16 type;
    /** Length */
    t_u16 length;
    /** custom IE data */
    custom_ie ie_data[0];
} MLAN_PACK_END tlvbuf_custom_ie;

/** Max IE index to FW */
#define MAX_MGMT_IE_INDEX_TO_FW 4U
/** Max IE index per BSS */
#define MAX_MGMT_IE_INDEX 16

/** custom IE info */
typedef MLAN_PACK_START struct _custom_ie_info
{
    /** size of buffer */
    t_u16 buf_size;
    /** no of buffers of buf_size */
    t_u16 buf_count;
} MLAN_PACK_END custom_ie_info;

/** TLV buffer : Max Mgmt IE */
typedef MLAN_PACK_START struct _tlvbuf_max_mgmt_ie
{
    /** Type */
    t_u16 type;
    /** Length */
    t_u16 len;
    /** No of tuples */
    t_u16 count;
    /** custom IE info tuples */
    custom_ie_info info[MAX_MGMT_IE_INDEX];
} MLAN_PACK_END tlvbuf_max_mgmt_ie;

/** TLV buffer : custom IE */
typedef MLAN_PACK_START struct _mlan_ds_misc_custom_ie
{
    /** Type */
    t_u16 type;
    /** Length */
    t_u16 len;
    /** IE data */
    custom_ie ie_data_list[MAX_MGMT_IE_INDEX_TO_FW];
    /** Max mgmt IE TLV */
    tlvbuf_max_mgmt_ie max_mgmt_ie;
} MLAN_PACK_END mlan_ds_misc_custom_ie;

/** channel type */
enum mlan_channel_type
{
    CHAN_NO_HT,
    CHAN_HT20,
    CHAN_HT40MINUS,
    CHAN_HT40PLUS,
    CHAN_VHT80
};

/** channel band */
enum
{
    BAND_2GHZ = 0,
    BAND_5GHZ = 1,
    BAND_6GHZ = 2,
    BAND_4GHZ = 3,
};

/** Band_Config_t */
typedef MLAN_PACK_START struct _Band_Config_t
{
    /** Band Info - (00)=2.4GHz, (01)=5GHz */
    t_u8 chanBand : 2;
    /** Channel Width - (00)=20MHz, (10)=40MHz, (11)=80MHz */
    t_u8 chanWidth : 2;
    /** Secondary Channel Offset - (00)=None, (01)=Above, (11)=Below */
    t_u8 chan2Offset : 2;
    /** Channel Selection Mode - (00)=manual, (01)=ACS, (02)=Adoption mode*/
    t_u8 scanMode : 2;
} MLAN_PACK_END Band_Config_t;

/** channel_band_t */
typedef MLAN_PACK_START struct _chan_band_info
{
    /** Band Configuration */
    Band_Config_t bandcfg;
    /** channel */
    t_u8 channel;
    /** 11n flag */
    t_u8 is_11n_enabled;
    /** center channel */
    t_u8 center_chan;
#if defined(ENABLE_802_11H) && defined(DFS_SUPPORT)
    /** dfs channel flag */
    t_u8 is_dfs_chan;
#endif
} MLAN_PACK_END chan_band_info;

/** csi event data structure */
#if CONFIG_CSI
typedef MLAN_PACK_START struct _csi_record_ds
{
    /** Length in DWORDS, including header */
    t_u16 Len;
    /** CSI signature. 0xABCD fixed */
    t_u16 CSI_Sign;
    /** User defined HeaderID  */
    t_u32 CSI_HeaderID;
    /** Packet info field */
    t_u16 PKT_info;
    /** Frame control field for the received packet*/
    t_u16 FCF;
    /** Timestamp when packet received */
    t_u64 TSF;
    /** Received Packet Destination MAC Address */
    t_u8 Dst_MAC[6];
    /** Received Packet Source MAC Address */
    t_u8 Src_MAC[6];
    /** RSSI for antenna A */
    t_u8 Rx_RSSI_A;
    /** RSSI for antenna B */
    t_u8 Rx_RSSI_B;
    /** Noise floor for antenna A */
    t_u8 Rx_NF_A;
    /** Noise floor for antenna A */
    t_u8 Rx_NF_B;
    /** Rx signal strength above noise floor */
    t_u8 Rx_SINR;
    /** Channel */
    t_u8 channel;
    /** user defined Chip ID */
    t_u16 chip_id;
    /** Reserved */
    t_u32 rsvd;
    /** CSI data length in DWORDs */
    t_u32 CSI_Data_Length;
    /** Start of CSI data */
    t_u8 CSI_Data[0];
    /** At the end of CSI raw data, user defined TailID of 4 bytes*/
} MLAN_PACK_END csi_record_ds, *pcsi_record_ds;
#endif

#ifdef PRAGMA_PACK
#pragma pack(pop)
#endif

/** mlan_callbacks data structure */
typedef struct _mlan_callbacks
{
#if 0
    /** moal_get_fw_data */
    mlan_status(*moal_get_fw_data) (IN t_void * pmoal_handle,
                                    IN t_u32 offset,
                                    IN t_u32 len, OUT t_u8 * pbuf);
    /** moal_init_fw_complete */
    mlan_status(*moal_init_fw_complete) (IN t_void * pmoal_handle,
                                         IN mlan_status status);
    /** moal_shutdown_fw_complete */
    mlan_status(*moal_shutdown_fw_complete) (IN t_void * pmoal_handle,
                                             IN mlan_status status);
    /** moal_send_packet_complete */
    mlan_status(*moal_send_packet_complete) (IN t_void * pmoal_handle,
                                             IN pmlan_buffer pmbuf,
                                             IN mlan_status status);
    /** moal_recv_complete */
    mlan_status(*moal_recv_complete) (IN t_void * pmoal_handle,
                                      IN pmlan_buffer pmbuf,
                                      IN t_u32 port, IN mlan_status status);
#endif /* 0 */
    /** moal_recv_packet */
    mlan_status (*moal_recv_packet)(IN t_void *pmoal_handle, IN pmlan_buffer pmbuf);
#if 0
    /** moal_recv_event */
    mlan_status(*moal_recv_event) (IN t_void * pmoal_handle,
                                   IN pmlan_event pmevent);
    /** moal_ioctl_complete */
    mlan_status(*moal_ioctl_complete) (IN t_void * pmoal_handle,
                                       IN pmlan_ioctl_req pioctl_req,
                                       IN mlan_status status);
    /** moal_alloc_mlan_buffer */
    mlan_status(*moal_alloc_mlan_buffer) (IN t_void * pmoal_handle,
                                          IN t_u32 size,
                                          OUT pmlan_buffer * pmbuf);
    /** moal_free_mlan_buffer */
    mlan_status(*moal_free_mlan_buffer) (IN t_void * pmoal_handle,
                                         IN pmlan_buffer pmbuf);
    /** moal_write_reg */
    mlan_status(*moal_write_reg) (IN t_void * pmoal_handle,
                                  IN t_u32 reg, IN t_u32 data);
    /** moal_read_reg */
    mlan_status(*moal_read_reg) (IN t_void * pmoal_handle,
                                 IN t_u32 reg, OUT t_u32 * data);
    /** moal_write_data_sync */
    mlan_status(*moal_write_data_sync) (IN t_void * pmoal_handle,
                                        IN pmlan_buffer pmbuf,
                                        IN t_u32 port, IN t_u32 timeout);
    /** moal_read_data_sync */
    mlan_status(*moal_read_data_sync) (IN t_void * pmoal_handle,
                                       IN OUT pmlan_buffer pmbuf,
                                       IN t_u32 port, IN t_u32 timeout);
#endif /* 0 */
    /** moal_malloc */
    mlan_status (*moal_malloc)(IN t_void *pmoal_handle, IN t_u32 size, IN t_u32 flag, OUT t_u8 **ppbuf);
    /** moal_mfree */
    mlan_status (*moal_mfree)(IN t_void *pmoal_handle, IN t_u8 *pbuf);
#if 0
    /** moal_memset */
    t_void *(*moal_memset) (IN t_void * pmoal_handle,
                            IN t_void * pmem, IN t_u8 byte, IN t_u32 num);
    /** moal_memcpy */
    t_void *(*moal_memcpy) (IN t_void * pmoal_handle,
                            IN t_void * pdest,
                            IN const t_void * psrc, IN t_u32 num);
    /** moal_memmove */
    t_void *(*moal_memmove) (IN t_void * pmoal_handle,
                             IN t_void * pdest,
                             IN const t_void * psrc, IN t_u32 num);
    /** moal_memcmp */
      t_s32(*moal_memcmp) (IN t_void * pmoal_handle,
                           IN const t_void * pmem1,
                           IN const t_void * pmem2, IN t_u32 num);
    /** moal_udelay */
      t_void(*moal_udelay) (IN t_void * pmoal_handle, IN t_u32 udelay);
    /** moal_get_system_time */
      mlan_status(*moal_get_system_time) (IN t_void * pmoal_handle,
                                          OUT t_u32 * psec, OUT t_u32 * pusec);
#endif /* 0 */

    /** moal_memcpy_ext */
    t_void *(*moal_memcpy_ext)(t_void *pmoal, t_void *pdest, const t_void *psrc, t_u32 num, t_u32 dest_size);

    /** moal_init_timer*/
    mlan_status (*moal_init_timer)(IN t_void *pmoal_handle,
                                   OUT t_void *ptimer,
                                   IN t_void (*callback)(osa_timer_arg_t arg),
                                   IN t_void *pcontext);
    /** moal_free_timer */
    mlan_status (*moal_free_timer)(IN t_void *pmoal_handle, IN t_void *ptimer);
    /** moal_start_timer*/
    mlan_status (*moal_start_timer)(IN t_void *pmoal_handle, IN t_void *ptimer, IN bool periodic, IN t_u32 msec);
    /** moal_reset_timer*/
    mlan_status (*moal_reset_timer)(IN t_void *pmoal_handle, IN t_void *ptimer);
    /** moal_stop_timer*/
    mlan_status (*moal_stop_timer)(IN t_void *pmoal_handle, IN t_void *ptimer);
    /** moal_init_lock */
    mlan_status (*moal_init_lock)(IN t_void *pmoal_handle, OUT t_void *plock);
    /** moal_free_lock */
    mlan_status (*moal_free_lock)(IN t_void *pmoal_handle, IN t_void *plock);
    /** moal_spin_lock */
    mlan_status (*moal_spin_lock)(IN t_void *pmoal_handle, IN t_void *plock);
    /** moal_spin_unlock */
    mlan_status (*moal_spin_unlock)(IN t_void *pmoal_handle, IN t_void *plock);
#if CONFIG_WMM
    /** moal_init_semaphore */
    mlan_status (*moal_init_semaphore)(IN t_void *pmoal_handle, IN const char *name, OUT t_void *plock);
    /** moal_free_semaphore */
    mlan_status (*moal_free_semaphore)(IN t_void *pmoal_handle, IN t_void *plock);
    /** moal_semaphore_get */
    mlan_status (*moal_semaphore_get)(IN t_void *pmoal_handle, IN t_void *plock);
    /** moal_semaphore_put */
    mlan_status (*moal_semaphore_put)(IN t_void *pmoal_handle, IN t_void *plock);
#endif
#if 0
    /** moal_print */
      t_void(*moal_print) (IN t_void * pmoal_handle,
                           IN t_u32 level, IN t_s8 * pformat, IN ...);
    /** moal_print_netintf */
      t_void(*moal_print_netintf) (IN t_void * pmoal_handle,
                                   IN t_u32 bss_index, IN t_u32 level);
    /** moal_assert */
      t_void(*moal_assert) (IN t_void * pmoal_handle, IN t_u32 cond);
#endif /* 0 */
} mlan_callbacks, *pmlan_callbacks;

/** Interrupt Mode SDIO */
#define INT_MODE_SDIO 0
/** Interrupt Mode GPIO */
#define INT_MODE_GPIO 1

/** Parameter unchanged, use MLAN default setting */
#define MLAN_INIT_PARA_UNCHANGED 0
/** Parameter enabled, override MLAN default setting */
#define MLAN_INIT_PARA_ENABLED 1
/** Parameter disabled, override MLAN default setting */
#define MLAN_INIT_PARA_DISABLED 2U

/** mlan_device data structure */
typedef struct _mlan_device
{
    /** BSS Attributes */
    mlan_bss_attr bss_attr[MLAN_MAX_BSS_NUM];
    /** Callbacks */
    mlan_callbacks callbacks;
} mlan_device, *pmlan_device;

/** MLAN API function prototype */
#define MLAN_API

/** Registration */
MLAN_API mlan_status mlan_register(IN pmlan_device pmdevice, OUT t_void **ppmlan_adapter);

/** Un-registration */
MLAN_API mlan_status mlan_unregister(IN t_void *pmlan_adapter);


/** Firmware Initialization */
MLAN_API mlan_status mlan_init_fw(IN t_void *pmlan_adapter);


#endif /* !_MLAN_DECL_H_ */
