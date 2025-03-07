/*
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*!\file wlan.h
 *\brief This file provides Wi-Fi APIs for the application.
 */

#ifndef __WLAN_H__
#define __WLAN_H__

#ifdef __ZEPHYR__
#include <nxp_wifi.h>
#endif

#include <wmtypes.h>
#include <wmerrno.h>
#include <stdint.h>
#include <wifi_events.h>
#include <wifi.h>

#define WLAN_DRV_VERSION "v1.3.r48.p31"

#if CONFIG_WPA2_ENTP
#include <wm_mbedtls_helper_api.h>
#endif

#define ARG_UNUSED(x) (void)(x)
/* Configuration */

#if !CONFIG_WLAN_KNOWN_NETWORKS
#define CONFIG_WLAN_KNOWN_NETWORKS 5U
#endif

#include <wmlog.h>
#define wlcm_e(...) wmlog_e("wlcm", ##__VA_ARGS__)
#define wlcm_w(...) wmlog_w("wlcm", ##__VA_ARGS__)

#if CONFIG_WLCMGR_DEBUG
#define wlcm_d(...) wmlog("wlcm", ##__VA_ARGS__)
#else
#define wlcm_d(...)
#endif /* ! CONFIG_WLCMGR_DEBUG */

#if (defined(configSUPPORT_STATIC_ALLOCATION) && (configSUPPORT_STATIC_ALLOCATION > 0U)) && \
    !((defined(configSUPPORT_DYNAMIC_ALLOCATION) && (configSUPPORT_DYNAMIC_ALLOCATION == 1U)))

#if CONFIG_WPA_SUPP
#error "Static memory allocation is not supported for wpa supplicant "
#endif

#endif

/** Action GET */
#define ACTION_GET (0U)
/** Action SET */
#define ACTION_SET (1)

/** Maximum SSID length */
#ifndef IEEEtypes_SSID_SIZE
#define IEEEtypes_SSID_SIZE 32U
#endif /* IEEEtypes_SSID_SIZE */

/** MAC Address length */
#ifndef IEEEtypes_ADDRESS_SIZE
#define IEEEtypes_ADDRESS_SIZE 6
#endif /* IEEEtypes_ADDRESS_SIZE */

#if CONFIG_HOST_SLEEP
#if CONFIG_POWER_MANAGER
extern osa_msg_handle_t mon_thread_event_queue;
#endif
#endif

#define WLAN_REASON_CODE_PREV_AUTH_NOT_VALID 2U

typedef enum
{
    BSS_INFRASTRUCTURE = 1,
    BSS_INDEPENDENT,
    BSS_ANY
} IEEEtypes_Bss_t;

/* The possible types of basic service sets */

/** The number of times that the Wi-Fi connection manager look for a
 *  network before giving up. */
#if CONFIG_MAX_RESCAN_LIMIT
#define WLAN_RESCAN_LIMIT CONFIG_MAX_RESCAN_LIMIT
#else
#if CONFIG_WPA_SUPP
#define WLAN_RESCAN_LIMIT 30U
#else
#define WLAN_RESCAN_LIMIT 5U
#endif /* CONFIG_WPA_SUPP */
#endif /* CONFIG_MAX_RESCAN_LIMIT */

#define WLAN_11D_SCAN_LIMIT 3U
/** The number of times that the Wi-Fi connection manager attempts a
 * reconnection with the network before giving up. */
#ifndef WLAN_RECONNECT_LIMIT
#define WLAN_RECONNECT_LIMIT 5U
#endif
/** Minimum length for network names, see \ref wlan_network. */
#define WLAN_NETWORK_NAME_MIN_LENGTH 1U
/** Maximum length for network names, see \ref wlan_network */
#define WLAN_NETWORK_NAME_MAX_LENGTH 32U
/** Minimum WPA2 passphrase can be up to 8 ASCII chars */
#define WLAN_PSK_MIN_LENGTH 8U
/** Maximum WPA2 passphrase can be up to 63 ASCII chars or 64 hexadecimal digits + 1 '\0' char */
#define WLAN_PSK_MAX_LENGTH 65U
/** Minimum WPA3 password can be up to 8 ASCII chars */
#define WLAN_PASSWORD_MIN_LENGTH 8U
/** Maximum WPA3 password can be up to 255 ASCII chars */
#define WLAN_PASSWORD_MAX_LENGTH 255U
/** Maximum enterprise identity can be up to 64 characters */
#define IDENTITY_MAX_LENGTH 64U
/** Maximum enterprise password can be up to 128 characters */
#define PASSWORD_MAX_LENGTH 128U
/** Maximum identities for EAP server users */
#define MAX_USERS 8U
/** Maximum length of encryption key for EAP-FAST PAC-Opaque values. */
#define PAC_OPAQUE_ENCR_KEY_MAX_LENGTH 33U
/** Maximum length of A-ID, A-ID indicates the identity of the authority that issues PACs. */
#define A_ID_MAX_LENGTH 33U
/** Maximum length of CA certification hash */
#define HASH_MAX_LENGTH 40U
/** Maximum length of domain match */
#define DOMAIN_MATCH_MAX_LENGTH 64U

#if CONFIG_WLAN_KNOWN_NETWORKS
/** The size of the list of known networks maintained by the Wi-Fi connection manager */
#define WLAN_MAX_KNOWN_NETWORKS CONFIG_WLAN_KNOWN_NETWORKS
#else
#error "CONFIG_WLAN_KNOWN_NETWORKS is not defined"
#endif /* CONFIG_WLAN_KNOWN_NETWORKS */
/** Length of a pairwise master key (PMK).  It's always 256 bits (32 Bytes) */
#define WLAN_PMK_LENGTH 32

#if CONFIG_WMM_UAPSD
#define WMM_UAPSD_QOS_INFO     0x0F
#define WMM_UAPSD_SLEEP_PERIOD 20
#endif

#if CONFIG_UAP_STA_MAC_ADDR_FILTER

/* Maximum number of STA filter list can be up to 16 */
#define WLAN_MAX_STA_FILTER_NUM 16

/* The length of Wi-Fi MAC address */
#define WLAN_MAC_ADDR_LENGTH 6
#endif

/** Error codes */
/** The operation was successful. */
#define WLAN_ERROR_NONE 0
/** The operation failed due to an error with one or more parameters. */
#define WLAN_ERROR_PARAM 1
/** The operation could not be performed because there is not enough memory. */
#define WLAN_ERROR_NOMEM 2
/** The operation could not be performed in the current system state. */
#define WLAN_ERROR_STATE 3
/** The operation failed due to an internal error. */
#define WLAN_ERROR_ACTION 4
/** The operation to change power state could not be performed*/
#define WLAN_ERROR_PS_ACTION 5
/** The requested feature is not supported*/
#define WLAN_ERROR_NOT_SUPPORTED 6

/*
 * HOST_WAKEUP_GPIO_PIN / CARD_WAKEUP_GPIO_PIN
 *
 *   Default GPIO pin number. This is chip
 *   specific, and a compile time setting depending on the system
 *   board level build!
 */
#if defined(SD8997) || defined(SD9098) || defined(SD9064) || defined(RW610)
#define HOST_WAKEUP_GPIO_PIN 12
#define CARD_WAKEUP_GPIO_PIN 13
#elif defined(SD9177) || defined(IW610)
#define HOST_WAKEUP_GPIO_PIN 17
#define CARD_WAKEUP_GPIO_PIN 16
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
#define HOST_WAKEUP_GPIO_PIN 13
#define CARD_WAKEUP_GPIO_PIN 16
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
#define HOST_WAKEUP_GPIO_PIN 2
#define CARD_WAKEUP_GPIO_PIN 16
#else
#define HOST_WAKEUP_GPIO_PIN 1
#define CARD_WAKEUP_GPIO_PIN 16
#endif

#define WLAN_MGMT_DIASSOC MBIT(10)
#define WLAN_MGMT_AUTH    MBIT(11)
#define WLAN_MGMT_DEAUTH  MBIT(12)
/** BITMAP for Action frame */
#define WLAN_MGMT_ACTION MBIT(13)

#if CONFIG_WMM_UAPSD
#define WMM_UAPSD_QOS_INFO     0x0F
#define WMM_UAPSD_SLEEP_PERIOD 20
#endif

#define WLAN_KEY_MGMT_IEEE8021X             MBIT(0)
#define WLAN_KEY_MGMT_PSK                   MBIT(1)
#define WLAN_KEY_MGMT_NONE                  MBIT(2)
#define WLAN_KEY_MGMT_IEEE8021X_NO_WPA      MBIT(3)
#define WLAN_KEY_MGMT_WPA_NONE              MBIT(4)
#define WLAN_KEY_MGMT_FT_IEEE8021X          MBIT(5)
#define WLAN_KEY_MGMT_FT_PSK                MBIT(6)
#define WLAN_KEY_MGMT_IEEE8021X_SHA256      MBIT(7)
#define WLAN_KEY_MGMT_PSK_SHA256            MBIT(8)
#define WLAN_KEY_MGMT_WPS                   MBIT(9)
#define WLAN_KEY_MGMT_SAE                   MBIT(10)
#define WLAN_KEY_MGMT_FT_SAE                MBIT(11)
#define WLAN_KEY_MGMT_WAPI_PSK              MBIT(12)
#define WLAN_KEY_MGMT_WAPI_CERT             MBIT(13)
#define WLAN_KEY_MGMT_CCKM                  MBIT(14)
#define WLAN_KEY_MGMT_OSEN                  MBIT(15)
#define WLAN_KEY_MGMT_IEEE8021X_SUITE_B     MBIT(16)
#define WLAN_KEY_MGMT_IEEE8021X_SUITE_B_192 MBIT(17)
#define WLAN_KEY_MGMT_FILS_SHA256           MBIT(18)
#define WLAN_KEY_MGMT_FILS_SHA384           MBIT(19)
#define WLAN_KEY_MGMT_FT_FILS_SHA256        MBIT(20)
#define WLAN_KEY_MGMT_FT_FILS_SHA384        MBIT(21)
#define WLAN_KEY_MGMT_OWE                   MBIT(22)
#define WLAN_KEY_MGMT_DPP                   MBIT(23)
#define WLAN_KEY_MGMT_FT_IEEE8021X_SHA384   MBIT(24)
#define WLAN_KEY_MGMT_PASN                  MBIT(25)
#define WLAN_KEY_MGMT_SAE_EXT_KEY           MBIT(26)

/** Fast BSS Transition(11r) key management */
#define WLAN_KEY_MGMT_FT                                                                                            \
    (WLAN_KEY_MGMT_FT_PSK | WLAN_KEY_MGMT_FT_IEEE8021X | WLAN_KEY_MGMT_FT_IEEE8021X_SHA384 | WLAN_KEY_MGMT_FT_SAE | \
     WLAN_KEY_MGMT_FT_FILS_SHA256 | WLAN_KEY_MGMT_FT_FILS_SHA384)

#if CONFIG_WPA_SUPP

#define WLAN_CIPHER_NONE         MBIT(0)
#define WLAN_CIPHER_WEP40        MBIT(1)
#define WLAN_CIPHER_WEP104       MBIT(2)
#define WLAN_CIPHER_TKIP         MBIT(3)
#define WLAN_CIPHER_CCMP         MBIT(4)
#define WLAN_CIPHER_AES_128_CMAC MBIT(5)
#define WLAN_CIPHER_GCMP         MBIT(6)
#define WLAN_CIPHER_SMS4         MBIT(7)
#define WLAN_CIPHER_GCMP_256     MBIT(8)
#define WLAN_CIPHER_CCMP_256     MBIT(9)
#define WLAN_CIPHER_BIP_GMAC_128 MBIT(11)
#define WLAN_CIPHER_BIP_GMAC_256 MBIT(12)
#define WLAN_CIPHER_BIP_CMAC_256 MBIT(13)
#define WLAN_CIPHER_GTK_NOT_USED MBIT(14)

#endif

/** Enum for Wi-Fi errors */
enum wm_wlan_errno
{
    WM_E_WLAN_ERRNO_BASE = MOD_ERROR_START(MOD_WLAN),
    /** The firmware download operation failed. */
    WLAN_ERROR_FW_DNLD_FAILED,
    /** The firmware ready register not set. */
    WLAN_ERROR_FW_NOT_READY,
    /** The Wi-Fi SoC not found. */
    WLAN_ERROR_CARD_NOT_DETECTED,
    /** The Wi-Fi Firmware not found. */
    WLAN_ERROR_FW_NOT_DETECTED,
    /** BSSID not found in scan list */
    WLAN_BSSID_NOT_FOUND_IN_SCAN_LIST,
};

/* Events and states */

/** Wi-Fi connection manager event reason */
enum wlan_event_reason
{
    /** The Wi-Fi connection manager has successfully connected to a network and
     *  is now in the \ref WLAN_CONNECTED state. */
    WLAN_REASON_SUCCESS,
    /** The Wi-Fi connection manager has successfully authenticated to a network and
     *  is now in the \ref WLAN_ASSOCIATED state. */
    WLAN_REASON_AUTH_SUCCESS,
    /** The Wi-Fi connection manager failed to connect before actual
     * connection attempt with AP due to incorrect Wi-Fi network profile.
     * or the Wi-Fi connection manager failed to reconnect to previously connected
     * network and it is now in the \ref WLAN_DISCONNECTED state. */
    WLAN_REASON_CONNECT_FAILED,
    /** The Wi-Fi connection manager could not find the network that it was
     *  connecting to and it is now in the \ref WLAN_DISCONNECTED state. */
    WLAN_REASON_NETWORK_NOT_FOUND,
#if CONFIG_BG_SCAN
    /** The Wi-Fi connection manager could not find the network in background scan during roam attempt that it was
     *  connecting to and it is now in the \ref WLAN_CONNECTED state with previous AP. */
    WLAN_REASON_BGSCAN_NETWORK_NOT_FOUND,
#endif
    /** The Wi-Fi connection manager failed to authenticate with the network
     *  and is now in the \ref WLAN_DISCONNECTED state. */
    WLAN_REASON_NETWORK_AUTH_FAILED,
    /** DHCP lease has been renewed. */
    WLAN_REASON_ADDRESS_SUCCESS,
    /** The Wi-Fi connection manager failed to obtain an IP address
     *  or TCP stack configuration has failed or the IP address
     *  configuration was lost due to a DHCP error.  The system is
     *  now in the \ref WLAN_DISCONNECTED state. */
    WLAN_REASON_ADDRESS_FAILED,
    /** The Wi-Fi connection manager has lost the link to the current network. */
    WLAN_REASON_LINK_LOST,
    /** The Wi-Fi connection manager has received the channel switch
     * announcement from the current network. */
    WLAN_REASON_CHAN_SWITCH,
    /** The Wi-Fi connection manager has disconnected from the WPS network
     *  (or has canceled a connection attempt) by request and is now in the
     *  WLAN_DISCONNECTED state. */
    WLAN_REASON_WPS_DISCONNECT,
    /** The Wi-Fi connection manager has disconnected from the current network
     *  (or has canceled a connection attempt) by request and is now in the
     *  WLAN_DISCONNECTED state. */
    WLAN_REASON_USER_DISCONNECT,
    /** The Wi-Fi connection manager is initialized and is ready for use.
     *  That is, it's now possible to scan or to connect to a network. */
    WLAN_REASON_INITIALIZED,
    /** The Wi-Fi connection manager has failed to initialize and is therefore
     *  not running. It is not possible to scan or to connect to a network.  The
     *  Wi-Fi connection manager should be stopped and started again via
     *  wlan_stop() and wlan_start() respectively. */
    WLAN_REASON_INITIALIZATION_FAILED,
    /** The Wi-Fi connection manager has entered in hang mode. */
    WLAN_REASON_FW_HANG,
    /** The Wi-Fi connection manager has reset fw successfully. */
    WLAN_REASON_FW_RESET,
    /** The Wi-Fi connection manager has entered power save mode. */
    WLAN_REASON_PS_ENTER,
    /** The Wi-Fi connection manager has exited from power save mode. */
    WLAN_REASON_PS_EXIT,
    /** The Wi-Fi connection manager has started uAP (micro access point) */
    WLAN_REASON_UAP_SUCCESS,
    /** A Wi-Fi client has joined uAP's BSS network */
    WLAN_REASON_UAP_CLIENT_ASSOC,
    /** A Wi-Fi client has authenticated and connected to uAP's BSS network */
    WLAN_REASON_UAP_CLIENT_CONN,
    /** A Wi-Fi client has left uAP's BSS network */
    WLAN_REASON_UAP_CLIENT_DISSOC,
    /** The Wi-Fi connection manager has failed to start uAP */
    WLAN_REASON_UAP_START_FAILED,
    /** The Wi-Fi connection manager has failed to stop uAP */
    WLAN_REASON_UAP_STOP_FAILED,
    /** The Wi-Fi connection manager has stopped uAP */
    WLAN_REASON_UAP_STOPPED,
    /** The Wi-Fi connection manager has received subscribed RSSI low event on station interface as per configured
       threshold and frequency. If CONFIG_11K, CONFIG_11V, CONFIG_11R or CONFIG_ROAMING enabled then RSSI low event is
       processed internally. */
    WLAN_REASON_RSSI_LOW,
#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
    /** The Wi-Fi connection manager has received subscribed RSSI high event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_RSSI_HIGH,
    /** The Wi-Fi connection manager has received subscribed SNR low event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_SNR_LOW,
    /** The Wi-Fi connection manager has received subscribed SNR high event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_SNR_HIGH,
    /** The Wi-Fi connection manager has received subscribed maximum fail event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_MAX_FAIL,
    /** The Wi-Fi connection manager has received subscribed beacon missed fail event on station interface as per
       configured threshold and frequency. */
    WLAN_REASON_BEACON_MISSED,
    /** The Wi-Fi connection manager has received subscribed data RSSI low event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_DATA_RSSI_LOW,
    /** The Wi-Fi connection manager has received subscribed data RSSI high event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_DATA_RSSI_HIGH,
    /** The Wi-Fi connection manager has received subscribed data SNR low event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_DATA_SNR_LOW,
    /** The Wi-Fi connection manager has received subscribed data SNR high event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_DATA_SNR_HIGH,
    /** The Wi-Fi connection manager has received subscribed link quality event on station interface as per configured
    link_snr threshold and frequency, link_rate threshold and frequency, link_tx_latency threshold and frequency*/
    WLAN_REASON_LINK_QUALITY,
    /** The Wi-Fi connection manager has received subscribed pre beacon lost event on station interface as per configured
       threshold and frequency. */
    WLAN_REASON_PRE_BEACON_LOST,
#endif
#if CONFIG_NCP
    /** Scan is done */
    WLAN_REASON_SCAN_DONE,
    /** WPS session is done */
    WLAN_REASON_WPS_SESSION_DONE,
#endif
};

/** Wakeup event bitmap */
enum wlan_wakeup_event_t
{
    /** Wakeup on broadcast  */
    WAKE_ON_ALL_BROADCAST = 1,
    /** Wakeup on unicast  */
    WAKE_ON_UNICAST = 1 << 1,
    /** Wakeup on MAC event  */
    WAKE_ON_MAC_EVENT = 1 << 2,
    /** Wakeup on multicast  */
    WAKE_ON_MULTICAST = 1 << 3,
    /** Wakeup on ARP broadcast  */
    WAKE_ON_ARP_BROADCAST = 1 << 4,
    /** Wakeup on receiving a management frame  */
    WAKE_ON_MGMT_FRAME = 1 << 6,
};

/** Wi-Fi station/uAP/Wi-Fi direct connection/status state */
enum wlan_connection_state
{
    /** The Wi-Fi connection manager is not connected and no connection attempt
     *  is in progress. It is possible to connect to a network or scan. */
    WLAN_DISCONNECTED,
    /** The Wi-Fi connection manager is not connected but it is currently
     *  attempting to connect to a network.  It is not possible to scan at this
     *  time.  It is possible to connect to a different network. */
    WLAN_CONNECTING,
    /** The Wi-Fi connection manager is not connected but associated. */
    WLAN_ASSOCIATED,
    /** The Wi-Fi connection manager is not connected but authenticated. */
    WLAN_AUTHENTICATED,
    /** The Wi-Fi connection manager is connected. It is possible to scan and
     *  connect to another network at this time. Information about the current
     *  network configuration is available. */
    WLAN_CONNECTED,
    /** The Wi-Fi connection manager has started uAP */
    WLAN_UAP_STARTED,
    /** The Wi-Fi connection manager has stopped uAP */
    WLAN_UAP_STOPPED,
    /** The Wi-Fi connection manager is not connected and network scan
     * is in progress. */
    WLAN_SCANNING,
    /** The Wi-Fi connection manager is not connected and network association
     * is in progress. */
    WLAN_ASSOCIATING,
};

/* Data Structures */

/** Station power save mode */
typedef enum wlan_ps_mode
{
    /** Active mode */
    WLAN_ACTIVE = 0,
    /** IEEE power save mode */
    WLAN_IEEE,
    /** Deep sleep power save mode */
    WLAN_DEEP_SLEEP,
    /** IEEE and deep sleep power save mode */
    WLAN_IEEE_DEEP_SLEEP,
#if CONFIG_WNM_PS
    /** WNM power save mode */
    WLAN_WNM,
    /** WNM and Deep sleep power save mode */
    WLAN_WNM_DEEP_SLEEP,
#endif
} wlan_ps_mode;

enum wlan_ps_state
{
    PS_STATE_AWAKE = 0,
    PS_STATE_PRE_SLEEP,
    PS_STATE_SLEEP_CFM,
    PS_STATE_SLEEP
};

typedef enum _ENH_PS_MODES
{
    GET_PS        = 0,
    SLEEP_CONFIRM = 5,
    EXT_PS_PARAM  = 6,
#if (CONFIG_WNM_PS)
    DIS_WNM_PS = 0xfc,
    EN_WNM_PS  = 0xfd,
#endif
    DIS_AUTO_PS = 0xfe,
    EN_AUTO_PS  = 0xff,
} ENH_PS_MODES;

typedef enum _Host_Sleep_Action
{
    HS_CONFIGURE = 0x0001,
    HS_ACTIVATE  = 0x0002,
} Host_Sleep_Action;

#if (CONFIG_WNM_PS)
typedef PACK_START struct
{
    uint8_t action;
    uint8_t result;
} PACK_END wnm_sleep_result_t;
#endif

#if CONFIG_CSI
enum wlan_csi_opt
{
    CSI_FILTER_OPT_ADD = 0,
    CSI_FILTER_OPT_DELETE,
    CSI_FILTER_OPT_CLEAR,
    CSI_FILTER_OPT_DUMP,
};
#endif

enum wlan_monitor_opt
{
    MONITOR_FILTER_OPT_ADD_MAC = 0,
    MONITOR_FILTER_OPT_DELETE_MAC,
    MONITOR_FILTER_OPT_CLEAR_MAC,
    MONITOR_FILTER_OPT_DUMP,
};

#if (CONFIG_11MC) || (CONFIG_11AZ)
#define FTM_ACTION_START 1
#define FTM_ACTION_STOP  2

#define PROTO_DOT11AZ_NTB 1
#define PROTO_DOT11AZ_TB  2
#define PROTO_DOT11MC     0

/* DOT11MC CFG */
/* Burst duration
 0 - 1: Reserved
 2: 250 microseconds
 3: 500 microseconds
 4: 1 ms
 5: 2 ms
 6: 4 ms
 7: 8 ms
 8: 16 ms
 9: 32 ms
 10: 64 ms
 11: 128 ms
 12-14 reserved*/
#define BURST_DURATION 11
/* Burst period in units of 100 milli seconds */
#define BURST_PERIOD 10
/* FTM frames per burst */
#define FTM_PER_BURST 5
/* Indicates minimum time between consecutive FTM (fine timing measurement) frames. It is specified in units of 100 microseconds. */
#define MIN_DELTA 60
/* ASAP */
#define IS_ASAP 1
/* Bandwidth
 9  - HT20
 10 - VHT20
 11 - HT40
 12 - VHT40
 13 - VHT80 */
#define BW 13 /* RW610 only allows 20M bandwidth */
/*Indicates how many burst instances are requested for the FTM session */
#define BURST_EXP 3

/* LCI */
#define LCI_REQUEST                1
#define LCI_LATITIUDE              -33.8570095
#define LCI_LONGITUDE              151.2152005
#define LCI_LATITUDE_UNCERTAINITY  18
#define LCI_LONGITUDE_UNCERTAINITY 18
#define LCI_ALTITUDE               11.2
#define LCI_ALTITUDE_UNCERTAINITY  15
#define Z_INFO                     0

/* CIVIC */
#define CIVIC_REQUEST       1
#define CIVIC_LOCATION      1
#define CIVIC_LOCATION_TYPE 1
#define CIVIC_COUNTRY_CODE  0 /* US */
#define CIVIC_ADDRESS_TYPE  22
#define CIVIC_ADDRESS       "#123"

/* DOT11AZ CFG */
#define FORMAT_BW 2 /* RW610 only allows 20M bandwidth */
/*Maximum number of space-time streams to be used in DL/UL NDP frames in the session up to 80MHz */
#define MAX_I2R_STS_UPTO80 0 /* RW610 only allows to send 1 N_STS*/
#define MAX_R2I_STS_UPTO80 0
/* Measurement frequency in Hz to calculate measurement interval */
#define AZ_MEASUREMENT_FREQ       4 /* in 0.1 Hz increments */
#define AZ_NUMBER_OF_MEASUREMENTS 6
#define I2R_LMR_FEEDBACK          0 /* allow RSTA to request I2R reporting */

#define FOR_RANGING 0

/** Structure of FTM_SESSION_CFG_NTB_RANGING / FTM_SESSION_CFG_TB_RANGING TLV data */
typedef struct _ranging_11az_cfg
{
    /** Indicates the channel BW for session
     * 0: HE20, 1: HE40, 2: HE80, 3: HE80+80, 4: HE160, 5:HE160_SRF */
    t_u8 format_bw;
    /** Indicates for bandwidths less than or equal to 80 MHz the maximum number of space-time streams to be used in
     * DL/UL NDP frames in the session*/
    t_u8 max_i2r_sts_upto80;
    /** Indicates for bandwidths less than or equal to 80 MHz the maximum number of space-time streams to be used in
     * DL/UL NDP frames in the session*/
    t_u8 max_r2i_sts_upto80;
    /** Specify measurement freq in Hz to calculate measurement interval */
    t_u8 az_measurement_freq;
    /** Indicates the number of measurements to be done for session */
    t_u8 az_number_of_measurements;
    /** Initator lmr feedback */
    t_u8 i2r_lmr_feedback;
    /** Include location civic request (Expect location civic from responder) */
    t_u8 civic_req;
    /** Include LCI request (Expect LCI info from responder) */
    t_u8 lci_req;
} ranging_11az_cfg_t;

typedef struct _location_cfg_info
{
    /** known latitude uncertainty */
    t_u8 lat_unc;
    /** known longitude uncertainty */
    t_u8 long_unc;
    /** Known altitude uncertainty */
    t_u8 alt_unc;
    /** Include LCI request (expect LCI information from responder) */
    t_u8 lci_req;
    /** known longitude */
    double longitude;
    /** known latitude */
    double latitude;
    /** known altitude */
    double altitude;
} location_cfg_info_t;

typedef struct _location_civic_rep
{
    /** Civic location type */
    t_u8 civic_location_type;
    /**Civic address type*/
    t_u8 civic_address_type;
    /**Civic address length*/
    t_u8 civic_address_length;
    /**Include LCI request (Expect LCI info from responder)*/
    t_u8 civic_req;
    /**Country code*/
    t_u16 country_code;
} location_civic_rep_t;

/** Structure of FTM_SESSION_CFG TLV data */
typedef struct _ftm_11mc_nego_cfg
{
    /** Indicates how many burst instances are requested for the FTM session*/
    t_u8 burst_exponent;
    /** Indicates the duration of a burst instance*/
    t_u8 burst_duration;
    /**Minimum time between consecutive FTM frames*/
    t_u8 min_delta_FTM;
    /**ASAP/non-ASAP casel*/
    t_u8 is_ASAP;
    /**Number of FTMs per burst*/
    t_u8 per_burst_FTM;
    /**FTM channel spacing: HT20/HT40/VHT80/... */
    t_u8 channel_spacing;
    /**Indicates the interval between two consecutive burst instances*/
    t_u16 burst_period;
} ftm_11mc_nego_cfg_t;
#endif

/** Scan result */
struct wlan_scan_result
{
    /** The network SSID, represented as a NULL-terminated C string of 0 to 32
     *  characters. If the network has a hidden SSID, this can be the empty
     *  string.
     */
    char ssid[WLAN_NETWORK_NAME_MAX_LENGTH + 1];
    /** SSID length */
    unsigned int ssid_len;
    /** The network BSSID, represented as a 6-byte array. */
    char bssid[IEEEtypes_ADDRESS_SIZE];
    /** The network channel. */
    unsigned int channel;
    /** The Wi-Fi network type. */
    enum wlan_bss_type type;
    /** The Wi-Fi network mode. */
    enum wlan_bss_role role;

    /* network features */
    /** The network supports 802.11N.  This is set to 0 if the network does not
     *  support 802.11N or if the system does not have 802.11N support enabled. */
    unsigned dot11n : 1;
#if CONFIG_11AC
    /** The network supports 802.11AC.  This is set to 0 if the network does not
     *  support 802.11AC or if the system does not have 802.11AC support enabled. */
    unsigned dot11ac : 1;
#endif
#if CONFIG_11AX
    /** The network supports 802.11AX.  This is set to 0 if the network does not
     *  support 802.11AX or if the system does not have 802.11AX support enabled. */
    unsigned dot11ax : 1;
#endif

    /** The network supports WMM.  This is set to 0 if the network does not
     *  support WMM or if the system does not have WMM support enabled. */
    unsigned wmm : 1;
#if (CONFIG_WPA_SUPP_WPS) || (CONFIG_WPS2)
    /** The network supports WPS.  This is set to 0 if the network does not
     *  support WPS or if the system does not have WPS support enabled. */
    unsigned wps : 1;
    /** WPS Type \ref WPS_SESSION_PBC/ \ref WPS_SESSION_PIN */
    unsigned int wps_session;
#endif
    /** The network uses WEP security. */
    unsigned wep : 1;
    /** The network uses WPA security. */
    unsigned wpa : 1;
    /** The network uses WPA2 security */
    unsigned wpa2 : 1;
    /** The network uses WPA2 SHA256 security */
    unsigned wpa2_sha256 : 1;
#if CONFIG_DRIVER_OWE
    /** The network uses OWE security */
    unsigned owe : 1;
#endif
    /** The network uses WPA3 SAE security */
    unsigned wpa3_sae : 1;
    /** The network uses WPA2 Enterprise security */
    unsigned wpa2_entp : 1;
    /** The network uses WPA3 Enterprise security */
    unsigned wpa3_entp : 1;
    /** The network uses WPA3 Enterprise SHA256 security */
    unsigned wpa3_1x_sha256 : 1;
    /** The network uses WPA3 Enterprise SHA384 security */
    unsigned wpa3_1x_sha384 : 1;
#if CONFIG_11R
    /** The network uses FT 802.1x security */
    unsigned ft_1x : 1;
    /** The network uses FT 892.1x SHA384 security */
    unsigned ft_1x_sha384 : 1;
    /** The network uses FT PSK security */
    unsigned ft_psk : 1;
    /** The network uses FT SAE security */
    unsigned ft_sae : 1;
#endif
    /** The signal strength of the beacon */
    unsigned char rssi;
    /** The network SSID, represented as a NULL-terminated C string of 0 to 32
     *  characters. If the network has a hidden SSID, this should be the empty
     *  string.
     */
    char trans_ssid[WLAN_NETWORK_NAME_MAX_LENGTH + 1];
    /** SSID length */
    unsigned int trans_ssid_len;
    /** The network BSSID, represented as a 6-byte array. */
    char trans_bssid[IEEEtypes_ADDRESS_SIZE];

    /** Beacon period */
    uint16_t beacon_period;

    /** DTIM (delivery traffic indication map) period */
    uint8_t dtim_period;

    /** MFPC (Management Frame Protection Capable) bit of AP (Access Point) */
    t_u8 ap_mfpc;
    /** MFPR (Management Frame Protection Required) bit of AP (Access Point) */
    t_u8 ap_mfpr;
    /** PWE (Password Element) bit of AP (Access Point) */
    t_u8 ap_pwe;

#if CONFIG_11K
    /** Neighbor report support */
    bool neighbor_report_supported;
#endif
#if CONFIG_11V
    /** bss transition support */
    bool bss_transition_supported;
#endif
};

typedef enum
{
    Band_2_4_GHz = 0,
    Band_5_GHz   = 1,
    Band_4_GHz   = 2,

} ChanBand_e;

#define NUM_CHAN_BAND_ENUMS 3

typedef enum
{
    ChanWidth_20_MHz = 0,
    ChanWidth_10_MHz = 1,
    ChanWidth_40_MHz = 2,
    ChanWidth_80_MHz = 3,
} ChanWidth_e;

typedef enum
{
    SECONDARY_CHAN_NONE  = 0,
    SECONDARY_CHAN_ABOVE = 1,
    SECONDARY_CHAN_BELOW = 3,
    // reserved 2, 4~255
} Chan2Offset_e;

typedef enum
{
    MANUAL_MODE = 0,
    ACS_MODE    = 1,
} ScanMode_e;

typedef PACK_START struct
{
    ChanBand_e chanBand : 2;
    ChanWidth_e chanWidth : 2;
    Chan2Offset_e chan2Offset : 2;
    ScanMode_e scanMode : 2;
} PACK_END BandConfig_t;

typedef PACK_START struct
{
    BandConfig_t bandConfig;
    uint8_t chanNum;

} PACK_END ChanBandInfo_t;


#if CONFIG_5GHz_SUPPORT
#define DFS_REC_HDR_LEN (8)
#define DFS_REC_HDR_NUM (10)
#define BIN_COUNTER_LEN (7)

typedef PACK_START struct _Event_Radar_Detected_Info
{
    t_u32 detect_count;
    t_u8 reg_domain;    /*1=fcc, 2=etsi, 3=mic*/
    t_u8 main_det_type; /*0=none, 1=pw(chirp), 2=pri(radar)*/
    t_u16 pw_chirp_type;
    t_u8 pw_chirp_idx;
    t_u8 pw_value;
    t_u8 pri_radar_type;
    t_u8 pri_binCnt;
    t_u8 binCounter[BIN_COUNTER_LEN];
    t_u8 numDfsRecords;
    t_u8 dfsRecordHdrs[DFS_REC_HDR_NUM][DFS_REC_HDR_LEN];
    t_u32 reallyPassed;
} PACK_END Event_Radar_Detected_Info;
#endif

/** Network security types */
enum wlan_security_type
{
    /** The network does not use security. */
    WLAN_SECURITY_NONE,
    /** The network uses WEP security with open key. */
    WLAN_SECURITY_WEP_OPEN,
    /** The network uses WEP security with shared key. */
    WLAN_SECURITY_WEP_SHARED,
    /** The network uses WPA security with PSK. */
    WLAN_SECURITY_WPA,
    /** The network uses WPA2 security with PSK. */
    WLAN_SECURITY_WPA2,
    /** The network uses WPA/WPA2 mixed security with PSK */
    WLAN_SECURITY_WPA_WPA2_MIXED,
#if CONFIG_11R
    /** The network uses WPA2 security with PSK FT. */
    WLAN_SECURITY_WPA2_FT,
#endif
    /** The network uses WPA3 security with SAE. */
    WLAN_SECURITY_WPA3_SAE,
#if CONFIG_WPA_SUPP
#if CONFIG_11R
    /** The network uses WPA3 security with SAE FT. */
    WLAN_SECURITY_WPA3_FT_SAE,
#endif
#endif
    /** The network uses WPA3 security with new SAE AKM suite 24. */
    WLAN_SECURITY_WPA3_SAE_EXT_KEY,
    /** The network uses WPA2/WPA3 SAE mixed security with PSK. */
    WLAN_SECURITY_WPA2_WPA3_SAE_MIXED,
#if CONFIG_DRIVER_OWE
    /** The network uses OWE only security without Transition mode support. */
    WLAN_SECURITY_OWE_ONLY,
#endif
#if (CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE) || (CONFIG_WPA2_ENTP)
    /** The network uses WPA2 Enterprise EAP-TLS security
     * The identity field in \ref wlan_network structure is used */
    WLAN_SECURITY_EAP_TLS,
#endif
#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#if CONFIG_EAP_TLS
    /** The network uses WPA2 Enterprise EAP-TLS SHA256 security.
     * The identity field in \ref wlan_network structure is used */
    WLAN_SECURITY_EAP_TLS_SHA256,
#if CONFIG_11R
    /** The network uses WPA2 Enterprise EAP-TLS FT security.
     * The identity field in \ref wlan_network structure is used */
    WLAN_SECURITY_EAP_TLS_FT,
    /** The network uses WPA2 Enterprise EAP-TLS FT SHA384 security
     * The identity field in \ref wlan_network structure is used */
    WLAN_SECURITY_EAP_TLS_FT_SHA384,
#endif
#endif
#if CONFIG_EAP_TTLS
    /** The network uses WPA2 Enterprise EAP-TTLS security.
     * The identity field in \ref wlan_network structure is used */
    WLAN_SECURITY_EAP_TTLS,
#endif
#if CONFIG_EAP_MSCHAPV2
    /** The network uses WPA2 Enterprise EAP-TTLS-MSCHAPV2 security.
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_TTLS_MSCHAPV2,
#endif
#endif
#if (CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE) || (CONFIG_PEAP_MSCHAPV2) || (CONFIG_WPA2_ENTP)
    /** The network uses WPA2 Enterprise EAP-PEAP-MSCHAPV2 security.
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_PEAP_MSCHAPV2,
#endif
#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#if CONFIG_EAP_PEAP
#if CONFIG_EAP_TLS
    /** The network uses WPA2 Enterprise EAP-PEAP-TLS security.
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_PEAP_TLS,
#endif
#if CONFIG_EAP_GTC
    /** The network uses WPA2 Enterprise EAP-PEAP-GTC security.
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_PEAP_GTC,
#endif
#endif
#if CONFIG_EAP_FAST
#if CONFIG_EAP_MSCHAPV2
    /** The network uses WPA2 Enterprise EAP-FAST-MSCHAPV2 security.
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_FAST_MSCHAPV2,
#endif
#if CONFIG_EAP_GTC
    /** The network uses WPA2 Enterprise EAP-FAST-GTC security
     * The anonymous identity, identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_FAST_GTC,
#endif
#endif
#if CONFIG_EAP_SIM
    /** The network uses WPA2 Enterprise EAP-SIM security
     * The identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_SIM,
#endif
#if CONFIG_EAP_AKA
    /** The network uses WPA2 Enterprise EAP-AKA security
     * The identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_AKA,
#endif
#if CONFIG_EAP_AKA_PRIME
    /** The network uses WPA2 Enterprise EAP-AKA-PRIME security
     * The identity and password fields in
     * \ref wlan_network structure are used */
    WLAN_SECURITY_EAP_AKA_PRIME,
#endif
#endif
#if CONFIG_WPA_SUPP_DPP
    /** The network uses DPP security with NAK(Net Access Key) */
    WLAN_SECURITY_DPP,
#endif
    /** The network can use any security method. This is often used when
     * the user only knows the name and passphrase but not the security
     * type.  */
    WLAN_SECURITY_WILDCARD,
};

#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#if CONFIG_EAP_TLS
/** EAP TLS Cipher types*/
enum eap_tls_cipher_type
{
    EAP_TLS_NONE,
    /** EAP TLS with ECDH & ECDSA with p384 */
    EAP_TLS_ECC_P384,
    /** EAP TLS with ECDH & RSA with > 3K */
    EAP_TLS_RSA_3K,
};
#endif
#endif

/** Wi-Fi cipher structure */
struct wlan_cipher
{
    /** 1 bit value can be set for none */
    uint16_t none : 1;
    /** 1 bit value can be set for wep40 */
    uint16_t wep40 : 1;
    /** 1 bit value can be set for wep104 */
    uint16_t wep104 : 1;
    /** 1 bit value can be set for tkip */
    uint16_t tkip : 1;
    /** 1 bit value can be set for ccmp */
    uint16_t ccmp : 1;
    /**  1 bit value can be set for aes 128 cmac */
    uint16_t aes_128_cmac : 1;
    /** 1 bit value can be set for gcmp */
    uint16_t gcmp : 1;
    /** 1 bit value can be set for sms4 */
    uint16_t sms4 : 1;
    /** 1 bit value can be set for gcmp 256 */
    uint16_t gcmp_256 : 1;
    /** 1 bit value can be set for ccmp 256 */
    uint16_t ccmp_256 : 1;
    /** 1 bit is reserved */
    uint16_t rsvd : 1;
    /** 1 bit value can be set for bip gmac 128 */
    uint16_t bip_gmac_128 : 1;
    /** 1 bit value can be set for bip gmac 256 */
    uint16_t bip_gmac_256 : 1;
    /** 1 bit value can be set for bip cmac 256 */
    uint16_t bip_cmac_256 : 1;
    /** 1 bit value can be set for gtk not used */
    uint16_t gtk_not_used : 1;
    /** 4 bits are reserved */
    uint16_t rsvd2 : 2;
};

static inline int is_valid_security(int security)
{
    /*Currently only these modes are supported */
    if ((security == WLAN_SECURITY_NONE) || (security == WLAN_SECURITY_WEP_OPEN) || (security == WLAN_SECURITY_WPA) ||
        (security == WLAN_SECURITY_WPA2) ||
#if CONFIG_11R
        (security == WLAN_SECURITY_WPA2_FT) ||
#endif
        (security == WLAN_SECURITY_WPA_WPA2_MIXED) ||
#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#if CONFIG_EAP_TLS
        (security == WLAN_SECURITY_EAP_TLS) || (security == WLAN_SECURITY_EAP_TLS_SHA256) ||
#if CONFIG_11R
        (security == WLAN_SECURITY_EAP_TLS_FT) || (security == WLAN_SECURITY_EAP_TLS_FT_SHA384) ||
#endif
#endif
#if CONFIG_EAP_TTLS
        (security == WLAN_SECURITY_EAP_TTLS) ||
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_TTLS_MSCHAPV2) ||
#endif
#endif
#if CONFIG_EAP_PEAP
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
#endif
#if CONFIG_EAP_TLS
        (security == WLAN_SECURITY_EAP_PEAP_TLS) ||
#endif
#if CONFIG_EAP_GTC
        (security == WLAN_SECURITY_EAP_PEAP_GTC) ||
#endif
#endif
#if CONFIG_EAP_FAST
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
#endif
#if CONFIG_EAP_GTC
        (security == WLAN_SECURITY_EAP_FAST_GTC) ||
#endif
#endif
#if CONFIG_EAP_SIM
        (security == WLAN_SECURITY_EAP_SIM) ||
#endif
#if CONFIG_EAP_AKA
        (security == WLAN_SECURITY_EAP_AKA) ||
#endif
#if CONFIG_EAP_AKA_PRIME
        (security == WLAN_SECURITY_EAP_AKA_PRIME) ||
#endif
#else
#if CONFIG_WPA2_ENTP
        (security == WLAN_SECURITY_EAP_TLS) ||
#endif
#if CONFIG_PEAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
#endif
#endif /* CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE */
#if CONFIG_DRIVER_OWE
        (security == WLAN_SECURITY_OWE_ONLY) ||
#endif
        (security == WLAN_SECURITY_WPA3_SAE) || (security == WLAN_SECURITY_WPA2_WPA3_SAE_MIXED) ||
#if CONFIG_WPA_SUPP
#if CONFIG_11R
        (security == WLAN_SECURITY_WPA3_FT_SAE) ||
#endif
#endif
        (security == WLAN_SECURITY_WPA3_SAE_EXT_KEY) || (security == WLAN_SECURITY_WILDCARD))
    {
        return 1;
    }
    return 0;
}

#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
static inline int is_ep_valid_security(int security)
{
    /*Currently only these modes are supported */
    if (
#if CONFIG_EAP_TLS
        (security == WLAN_SECURITY_EAP_TLS) || (security == WLAN_SECURITY_EAP_TLS_SHA256) ||
#if CONFIG_11R
        (security == WLAN_SECURITY_EAP_TLS_FT) || (security == WLAN_SECURITY_EAP_TLS_FT_SHA384) ||
#endif
#endif
#if CONFIG_EAP_TTLS
        (security == WLAN_SECURITY_EAP_TTLS) ||
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_TTLS_MSCHAPV2) ||
#endif
#endif
#if CONFIG_EAP_PEAP
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_PEAP_MSCHAPV2) ||
#endif
#if CONFIG_EAP_TLS
        (security == WLAN_SECURITY_EAP_PEAP_TLS) ||
#endif
#if CONFIG_EAP_GTC
        (security == WLAN_SECURITY_EAP_PEAP_GTC) ||
#endif
#endif
#if CONFIG_EAP_FAST
#if CONFIG_EAP_MSCHAPV2
        (security == WLAN_SECURITY_EAP_FAST_MSCHAPV2) ||
#endif
#if CONFIG_EAP_GTC
        (security == WLAN_SECURITY_EAP_FAST_GTC) ||
#endif
#endif
#if CONFIG_EAP_SIM
        (security == WLAN_SECURITY_EAP_SIM) ||
#endif
#if CONFIG_EAP_AKA
        (security == WLAN_SECURITY_EAP_AKA) ||
#endif
#if CONFIG_EAP_AKA_PRIME
        (security == WLAN_SECURITY_EAP_AKA_PRIME) ||
#endif
        false)
    {
        return 1;
    }
    return 0;
}
#endif

/** Network security configuration */
struct wlan_network_security
{
    /** Type of network security to use. Specified by enum
     * \ref wlan_security_type. */
    enum wlan_security_type type;
    /** Key management type */
    int key_mgmt;
    /** Type of network security Group Cipher suite */
    struct wlan_cipher mcstCipher;
    /** Type of network security Pairwise Cipher suite */
    struct wlan_cipher ucstCipher;
#if CONFIG_WPA_SUPP
    /** Proactive key caching */
    unsigned pkc : 1;
    /** Type of network security Group Cipher suite */
    int group_cipher;
    /** Type of network security Pairwise Cipher suite */
    int pairwise_cipher;
    /** Type of network security Pairwise Cipher suite */
    int group_mgmt_cipher;
#endif
    /** Is PMF (protected management frame) required */
    bool is_pmf_required;
    /** Pre-shared key (network password).  For WEP networks this is a hex byte
     * sequence of length psk_len, for WPA and WPA2 networks this is an ASCII
     * pass-phrase of length psk_len.  This field is ignored for networks with no
     * security. */
    char psk[WLAN_PSK_MAX_LENGTH];
    /** Length of the WEP key or WPA/WPA2 pass phrase, \ref WLAN_PSK_MIN_LENGTH to \ref
     * WLAN_PSK_MAX_LENGTH.  Ignored for networks with no security. */
    uint8_t psk_len;
    /** WPA3 SAE password, for WPA3 SAE networks this is an ASCII
     * password of length password_len.  This field is ignored for networks with no
     * security. */
    char password[WLAN_PASSWORD_MAX_LENGTH + 1];
    /** Length of the WPA3 SAE Password, \ref WLAN_PASSWORD_MIN_LENGTH to \ref
     * WLAN_PASSWORD_MAX_LENGTH.  Ignored for networks with no security. */
    size_t password_len;
    /** Preference list of enabled groups for SAE. By default (if this parameter is not set),
     * the mandatory group 19 (ECC group defined over a 256-bit prime order field) is preferred,
     * but other groups are also enabled. If this parameter is set, the groups is tried
     * in the indicated order. */
    char *sae_groups;
    /** SAE (Simultaneous Authentication of Equals) mechanism for PWE (Password Element) derivation */
    /* 0: hunting-and-pecking loop only, 1: hash-to-element only,
     * 2: both hunting-and-pecking loop and hash-to-element enabled */
    uint8_t pwe_derivation;
    /** Transition Disable indication */
    /* bit 0 (0x01): WPA3-Personal (i.e., disable WPA2-Personal = WPA-PSK
     * and only allow SAE to be used)
     * bit 1 (0x02): SAE-PK (disable SAE without use of SAE-PK)
     * bit 2 (0x04): WPA3-Enterprise (move to requiring PMF)
     * bit 3 (0x08): Enhanced Open (disable use of open network; require
     * OWE)
     */
    uint8_t transition_disable;
#if CONFIG_DRIVER_OWE
    /** OWE Groups */
    char *owe_groups;
#endif
    /** PMK (pairwise master key). When pmk_valid is set, this is the PMK calculated
     * from the PSK for WPA/PSK networks. If pmk_valid is not set, this field
     * is ignored. When adding networks with \ref wlan_add_network, users
     * can initialize PMK and set pmk_valid in lieu of setting the psk. After
     * successfully connecting to a WPA/PSK network, users can call \ref
     * wlan_get_current_network to inspect pmk_valid and pmk. Thus, the pmk
     * value can be populated in subsequent calls to \ref wlan_add_network.
     * This saves the CPU time required to otherwise calculate the PMK.
     */
    char pmk[WLAN_PMK_LENGTH];

    /** Flag reporting whether PMK is valid or not. */
    bool pmk_valid;
    /** Management frame protection capable (MFPC) */
    int8_t mfpc;
    /** Management frame protection required (MFPR) */
    int8_t mfpr;
#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
    /** WPA3 Enterprise mode */
    unsigned wpa3_ent : 1;
    /** WPA3 Enterprise Suite B mode */
    unsigned wpa3_sb : 1;
    /** WPA3 Enterprise Suite B 192 mode */
    unsigned wpa3_sb_192 : 1;
    /** EAP (Extensible Authentication Protocol) version */
    unsigned eap_ver : 1;
#if CONFIG_EAP_PEAP
    /** PEAP (Protected Extensible Authentication Protocol) label */
    unsigned peap_label : 1;
    /** crypto_binding option can be used to control \ref WLAN_SECURITY_EAP_PEAP_MSCHAPV2, \ref
     * WLAN_SECURITY_EAP_PEAP_TLS and \ref WLAN_SECURITY_EAP_PEAP_GTC version 0 cryptobinding behavior: 0 = do not use
     * cryptobinding (default) 1 = use cryptobinding if server supports it 2 = require cryptobinding */
    uint8_t eap_crypto_binding;
#endif
#if (CONFIG_EAP_SIM) || (CONFIG_EAP_AKA) || (CONFIG_EAP_AKA_PRIME)
    /** eap_result_ind=1 can be used to enable \ref WLAN_SECURITY_EAP_SIM, \ref WLAN_SECURITY_EAP_AKA and \ref
     * WLAN_SECURITY_EAP_AKA_PRIME to use protected result indication.*/
    unsigned eap_result_ind : 1;
#endif
#if CONFIG_EAP_TLS
    /** Cipher for EAP TLS (Extensible Authentication Protocol Transport Layer Security) */
    unsigned char tls_cipher;
#endif
    /** Identity string for EAP */
    char identity[IDENTITY_MAX_LENGTH];
    /** Anonymous identity string for EAP */
    char anonymous_identity[IDENTITY_MAX_LENGTH];
    /** Password string for EAP. */
    char eap_password[PASSWORD_MAX_LENGTH];
#if CONFIG_EAP_MSCHAPV2
    /** whether verify peer with CA or not
     *  0: not verify,
     *  1: verify. */
    bool verify_peer;
#endif
    /** CA (Certificate Authority) certification blob (Binary Large Object) in PEM (Base64 ASCII)/DER (binary) format */
    unsigned char *ca_cert_data;
    /** CA (Certificate Authority) certification blob (Binary Large Object) length */
    size_t ca_cert_len;
    /** Client certification blob (Binary Large Object) in PEM (Base64 ASCII)/DER (binary) format */
    unsigned char *client_cert_data;
    /** Client certification blob (Binary Large Object) length */
    size_t client_cert_len;
    /** Client key blob (Binary Large Object) */
    unsigned char *client_key_data;
    /** Client key blob (Binary Large Object) length */
    size_t client_key_len;
    /** Client key password */
    char client_key_passwd[PASSWORD_MAX_LENGTH];
    /** CA certification HASH */
    char ca_cert_hash[HASH_MAX_LENGTH];
    /** Domain */
    char domain_match[DOMAIN_MATCH_MAX_LENGTH];
    /** Domain Suffix */
    char domain_suffix_match[DOMAIN_MATCH_MAX_LENGTH]; /*suffix max length same as full domain name length*/
    /** CA (Certificate Authority) certification blob (Binary Large Object) in PEM (Base64 ASCII)/DER (binary) format for phase two */
    unsigned char *ca_cert2_data;
    /** CA (Certificate Authority) certification blob (Binary Large Object) length for phase two */
    size_t ca_cert2_len;
    /** Client certification blob (Binary Large Object) in PEM (Base64 ASCII)/DER (binary) format for phase two */
    unsigned char *client_cert2_data;
    /** Client certification blob (Binary Large Object) length for phase two */
    size_t client_cert2_len;
    /** Client key blob (Binary Large Object) for phase two */
    unsigned char *client_key2_data;
    /** Client key blob (Binary Large Object) length for phase two */
    size_t client_key2_len;
    /** Client key password for phase two */
    char client_key2_passwd[PASSWORD_MAX_LENGTH];
#if CONFIG_HOSTAPD
#if CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE
    /** DH (Diffie Hellman) parameters blob (Binary Large Object) */
    unsigned char *dh_data;
    /** DH (Diffie Hellman) parameters blob (Binary Large Object) length */
    size_t dh_len;
    /** Server certification blob (Binary Large Object) in PEM (Base64 ASCII)/DER (binary) format */
    unsigned char *server_cert_data;
    /** Server certification blob (Binary Large Object) length */
    size_t server_cert_len;
    /** Server key blob (Binary Large Object) */
    unsigned char *server_key_data;
    /** Server key blob (Binary Large Object) length */
    size_t server_key_len;
    /** Server key password */
    char server_key_passwd[PASSWORD_MAX_LENGTH];
    /** Number of EAP users */
    size_t nusers;
    /** User Identities */
    char identities[MAX_USERS][IDENTITY_MAX_LENGTH];
    /** User Passwords */
    char passwords[MAX_USERS][PASSWORD_MAX_LENGTH];
#if CONFIG_EAP_FAST
    /** Encryption key for EAP-FAST PAC-Opaque values */
    char pac_opaque_encr_key[PAC_OPAQUE_ENCR_KEY_MAX_LENGTH];
    /** EAP-FAST authority identity (A-ID) */
    char a_id[A_ID_MAX_LENGTH];
    /** EAP-FAST provisioning modes:
     * 0 = provisioning disabled
     * 1 = only anonymous provisioning allowed
     * 2 = only authenticated provisioning allowed
     * 3 = both provisioning modes allowed (default)
     */
    uint8_t fast_prov;
#endif
#endif
#endif
#elif (CONFIG_WPA2_ENTP)
    /** TLS client cert configuration */
    wm_mbedtls_cert_t tls_cert;
    /** mbedtls_ssl_config handle */
    mbedtls_ssl_config *wlan_ctx;
    /** mbedtls_ssl_context handle */
    mbedtls_ssl_context *wlan_ssl;
#endif
#if CONFIG_WPA_SUPP_DPP
    unsigned char *dpp_connector;
    unsigned char *dpp_c_sign_key;
    unsigned char *dpp_net_access_key;
#endif
};

/** Configuration for Wi-Fi scan */
#define MAX_CHANNEL_LIST 6
/** This structure is used to configure Wi-Fi scan parameters */
struct wifi_scan_params_t
{
    /** BSSID (basic service set ID) */
    uint8_t *bssid;
    /** SSID (service set ID) */
    char *ssid;
    /** Channel list */
    int channel[MAX_CHANNEL_LIST];
    /** BSS (basic service set) type.
    1: Infrastructure BSS,
    2: Indenpent BSS.
    */
    IEEEtypes_Bss_t bss_type;
    /** Time for scan duration */
    int scan_duration;
    /** split scan delay */
    int split_scan_delay;
};

#if CONFIG_WIFI_GET_LOG
/** Wi-Fi firmware stat from \ref wifi_pkt_stats_t
 */
typedef wifi_pkt_stats_t wlan_pkt_stats_t;
#endif

/** Configuration for Wi-Fi scan channel list from
 * \ref wifi_scan_channel_list_t
 */
typedef wifi_scan_channel_list_t wlan_scan_channel_list_t;
/** Configuration for Wi-Fi scan parameters v2 from
 * \ref wifi_scan_params_v2_t
 */
typedef wifi_scan_params_v2_t wlan_scan_params_v2_t;


/** Configuration for Wi-Fi calibration data from
 * \ref wifi_cal_data_t
 */
typedef wifi_cal_data_t wlan_cal_data_t;

#if CONFIG_AUTO_RECONNECT
/** Configuration for auto reconnect configuration from
 * \ref wifi_auto_reconnect_config_t
 */
typedef wifi_auto_reconnect_config_t wlan_auto_reconnect_config_t;
#endif

/** Configuration for memory efficient filters in Wi-Fi firmware from
 * \ref wifi_flt_cfg_t
 */
typedef wifi_flt_cfg_t wlan_flt_cfg_t;

/** Configuration for wowlan pattern parameters from
 * \ref wifi_wowlan_ptn_cfg_t
 */
typedef wifi_wowlan_ptn_cfg_t wlan_wowlan_ptn_cfg_t;
/** Configuration for TCP keep alive parameters from
 * \ref wifi_tcp_keep_alive_t
 */
typedef wifi_tcp_keep_alive_t wlan_tcp_keep_alive_t;

#if CONFIG_CLOUD_KEEP_ALIVE
/** Configuration for cloud keep alive parameters from
 * \ref wifi_cloud_keep_alive_t
 */
typedef wifi_cloud_keep_alive_t wlan_cloud_keep_alive_t;
#endif

/** Configuration for TX rate and get data rate from
 * \ref wifi_ds_rate
 */
typedef wifi_ds_rate wlan_ds_rate;
/** Configuration for ED MAC Control parameters from
 * \ref wifi_ed_mac_ctrl_t
 */
typedef wifi_ed_mac_ctrl_t wlan_ed_mac_ctrl_t;
/** Configuration for band from
 * \ref wifi_bandcfg_t
 */
typedef wifi_bandcfg_t wlan_bandcfg_t;
/** Configuration for CW mode parameters from
 * \ref wifi_cw_mode_ctrl_t
 */
typedef wifi_cw_mode_ctrl_t wlan_cw_mode_ctrl_t;
/** Configuration for channel list from
 * \ref wifi_chanlist_t
 */
typedef wifi_chanlist_t wlan_chanlist_t;
/** Configuration for TX power Limit from
 * \ref wifi_txpwrlimit_t
 */
typedef wifi_txpwrlimit_t wlan_txpwrlimit_t;
#ifdef SD8801
/** Statistic of External Coex from
 * \ref wifi_ext_coex_config_t
 */
typedef wifi_ext_coex_stats_t wlan_ext_coex_stats_t;
/** Configuration for external Coex from
 * \ref wifi_ext_coex_config_t
 */
typedef wifi_ext_coex_config_t wlan_ext_coex_config_t;
#endif

#if CONFIG_11AX
/** Configuration for RU TX power limit from
 * \ref wifi_rutxpwrlimit_t
 */
typedef wifi_rutxpwrlimit_t wlan_rutxpwrlimit_t;
/** Configuration for 802.11ax capabilities
 * \ref wifi_11ax_config_t
 */
typedef wifi_11ax_config_t wlan_11ax_config_t;
#if CONFIG_11AX_TWT
/** Configuration for TWT setup
 * \ref wifi_twt_setup_config_t
 */
typedef wifi_twt_setup_config_t wlan_twt_setup_config_t;
/** Configuration for TWT teardown
 * \ref wifi_twt_teardown_config_t
 */
typedef wifi_twt_teardown_config_t wlan_twt_teardown_config_t;
/** Configuration for Broadcast TWT setup
 * \ref wifi_btwt_config_t
 */
typedef wifi_btwt_config_t wlan_btwt_config_t;
/** Configuration for TWT report
 * \ref wifi_twt_report_t
 */
typedef wifi_twt_report_t wlan_twt_report_t;
#endif /* CONFIG_11AX_TWT */
#if CONFIG_MMSF
#define WLAN_AMPDU_DENSITY 0x30
#define WLAN_AMPDU_MMSF    0x6
#endif
#endif
#if CONFIG_WIFI_CLOCKSYNC
/** Configuration for clock sync GPIO TSF latch
 * \ref wifi_clock_sync_gpio_tsf_t
 */
typedef wifi_clock_sync_gpio_tsf_t wlan_clock_sync_gpio_tsf_t;
/** Configuration for TSF info
 * \ref wifi_tsf_info_t
 */
typedef wifi_tsf_info_t wlan_tsf_info_t;
#endif

#if CONFIG_MULTI_CHAN
/** Configuration for multi-channel switch
 * \ref wifi_drcs_cfg_t
 */
typedef wifi_drcs_cfg_t wlan_drcs_cfg_t;
#endif

typedef wifi_mgmt_frame_t wlan_mgmt_frame_t;


#if CONFIG_CSI
/** Configuration for CSI config params from
 * \ref wifi_csi_config_params_t
 */
typedef wifi_csi_config_params_t wlan_csi_config_params_t;
#endif

#if CONFIG_NET_MONITOR
/** Configuration for net monitor from
 * \ref wifi_net_monitor_t
 */
typedef wifi_net_monitor_t wlan_net_monitor_t;
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
/** Configuration for GPIO independent reset
 * \ref wifi_indrst_cfg_t
 */
typedef wifi_indrst_cfg_t wlan_indrst_cfg_t;
#endif

#if CONFIG_11AX
/** Configuration for TX rate setting from
 * \ref txrate_setting
 */
typedef txrate_setting wlan_txrate_setting;
#endif

#ifdef STA_SUPPORT
/** Configuration for RSSI information
 * \ref wifi_rssi_info_t
 */
typedef wifi_rssi_info_t wlan_rssi_info_t;
#endif

#if CONFIG_EXTERNAL_COEX_PTA
#define MIN_SAMP_TIMING              20
#define MAX_SAMP_TIMING              200
#define COEX_PTA_FEATURE_ENABLE      1
#define COEX_PTA_FEATURE_DISABLE     0
#define POL_GRANT_PIN_HIGH           0
#define POL_GRANT_PIN_LOW            1
#define STATE_INPUT_DISABLE          0
#define STATE_PTA_PIN                1
#define STATE_PRIORITY_PIN           2
#define SAMPLE_TIMING_VALUE          100
#define EXT_COEX_PTA_INTERFACE       5
#define EXT_COEX_WCI2_INTERFACE      6
#define EXT_COEX_WCI2_GPIO_INTERFACE 7

typedef struct _external_coex_pta_cfg
{
    /** Enable: 0x01, Disable: 0x00 */
    t_u8 enabled;
    /** Enable extended Wi-Fi and Bluetooth LE arbitration: 0x01, disable : 0x00 */
    t_u8 ext_WifiBtArb;
    /** Active high: 0x00, Active low: 0x01 */
    t_u8 polGrantPin;
    /**  Enable PriPtaInt: 0x01, Disable PriPtaInt: 0x00 */
    t_u8 enable_PriPtaInt;
    /** State input disable: 0x00, State info is from state pin: 0x01, State info is sampled on priority pin: 0x02 */
    t_u8 enable_StatusFromPta;
    /** Timing to sample Priority bit */
    t_u16 setPriSampTiming;
    /** Timing to sample TX/RX info */
    t_u16 setStateInfoSampTiming;
    /** Enable external traffic TX/RX Priority: 0x01, Disable external traffic TX/RX Priority: 0x00 */
    t_u8 extRadioTrafficPrio;
    /** Enable wci-2 interface: 0x01, Disable wci-2 interface: 0x00 */
    t_u8 extCoexHwIntWci2;
} ext_coex_pta_cfg;
#endif

/**
* Check whether the scan duration is valid or not.
*
* \param[in] scan_duration: scan duration time
*
* \return 0 if the time is valid, else return -1.
*/
int verify_scan_duration_value(int scan_duration);

/**
* Check whether the scan channel is valid or not.
*
* \param[in] channel: the scan channel
*
* \return 0 if the channel is valid, else return -1.
*/
int verify_scan_channel_value(int channel);

/**
* Check whether the scan delay time is valid or not.
*
* \param[in] delay: the scan delay time.
*
* \return 0 if the time is valid, else return -1.
*/
int verify_split_scan_delay(int delay);

/**
* Set the scan parameters.
*
* \param[in] wifi_scan_params: Wi-Fi scan parameter structure pointer.
*
* \return 0 if Wi-Fi scan parameters are set successfully, else return -1.
*/
int set_scan_params(struct wifi_scan_params_t *wifi_scan_params);

/**
* Get the scan parameters.
*
* \param[out] wifi_scan_params: Wi-Fi scan parameter structure pointer.
*
* \return WM_SUCCESS.
*/
int get_scan_params(struct wifi_scan_params_t *wifi_scan_params);

/**
* Get the current RSSI value.
*
* \param[out] rssi: pointer to get the current RSSI (Received Signal Strength Indicator)
*
* \return WM_SUCCESS.
*/
int wlan_get_current_rssi(short *rssi);

/**
* Get the current noise floor.
*
* \return The noise floor value
*/
int wlan_get_current_nf(void);

/** Address types to be used by the element wlan_ip_config.addr_type below
 */
enum address_types
{
    /** Static IP address */
    ADDR_TYPE_STATIC = 0,
    /** Dynamic IP address */
    ADDR_TYPE_DHCP = 1,
    /** Link level address */
    ADDR_TYPE_LLA = 2,
    /** For Bridge Mode, no IP address */
    ADDR_TYPE_BRIDGE_MODE = 3,
};

/** This data structure represents an IPv4 address */
struct ipv4_config
{
    /** Set to \ref ADDR_TYPE_DHCP to use DHCP to obtain the IP address or set to
     *  \ref ADDR_TYPE_STATIC to use a static IP. In case of static IP
     *  address ip, gw, netmask and dns members should be specified. When
     *  using DHCP, the ip, gw, netmask and dns are overwritten by the
     *  values obtained from the DHCP server. They should be zeroed out if
     *  not used. */
    enum address_types addr_type;
    /** The system's IP address in network order. */
    unsigned address;
    /** The system's default gateway in network order. */
    unsigned gw;
    /** The system's subnet mask in network order. */
    unsigned netmask;
    /** The system's primary dns server in network order. */
    unsigned dns1;
    /** The system's secondary dns server in network order. */
    unsigned dns2;
};

#if CONFIG_IPV6
/** This data structure represents an IPv6 address */
struct ipv6_config
{
    /** The system's IPv6 address in network order. */
    unsigned address[4];
    /** The address type: linklocal, site-local or global. */
    unsigned char addr_type;
    /** The state of IPv6 address (Tentative, Preferred, etc.). */
    unsigned char addr_state;
};
#endif

/** Network IP configuration.
 *
 *  This data structure represents the network IP configuration
 *  for IPv4 as well as IPv6 addresses
 */
struct wlan_ip_config
{
#if CONFIG_IPV6
    /** The network IPv6 address configuration that should be
     * associated with this interface. */
    struct ipv6_config ipv6[CONFIG_MAX_IPV6_ADDRESSES];
    /** The network IPv6 valid addresses count */
    size_t ipv6_count;
#endif
    /** The network IPv4 address configuration that should be
     * associated with this interface. */
    struct ipv4_config ipv4;
};

/** Wi-Fi network profile
 *
 *  This data structure represents a Wi-Fi network profile. It consists of an
 *  arbitrary name, Wi-Fi configuration, and IP address configuration.
 *
 *  Every network profile is associated with one of the two interfaces. The
 *  network profile can be used for the station interface (i.e. to connect to an
 *  Access Point) by setting the role field to \ref
 *  WLAN_BSS_ROLE_STA. The network profile can be used for the uAP
 *  interface (i.e. to start a network of our own.) by setting the mode field to
 *  \ref WLAN_BSS_ROLE_UAP.
 *
 *  If the mode field is \ref WLAN_BSS_ROLE_STA, either of the SSID or
 *  BSSID fields are used to identify the network, while the other members like
 *  channel and security settings characterize the network.
 *
 *  If the mode field is \ref WLAN_BSS_ROLE_UAP, the SSID, channel and security
 *  fields are used to define the network to be started.
 *
 *  In both the above cases, the address field is used to determine the type of
 *  address assignment to be used for this interface.
 */
struct wlan_network
{
#if CONFIG_WPA_SUPP
    /** Identifier for network profile */
    int id;
    /** WPS network flag. */
    int wps_network;
#endif
    /** The name of this network profile. Each network profile that is
     *  added to the Wi-Fi connection manager should have a unique name. */
    char name[WLAN_NETWORK_NAME_MAX_LENGTH + 1];
    /** The network SSID, represented as a C string of up to 32 characters
     *  in length.
     *  If this profile is used in the uAP mode, this field is
     *  used as the SSID of the network.
     *  If this profile is used in the station mode, this field is
     *  used to identify the network. Set the first byte of the SSID to NULL
     *  (a 0-length string) to use only the BSSID to find the network.
     */
    char ssid[IEEEtypes_SSID_SIZE + 1];
    /** The network BSSID, represented as a 6-byte array.
     *  If this profile is used in the uAP mode, this field is
     *  ignored.
     *  If this profile is used in the station mode, this field is
     *  used to identify the network. Set all 6 bytes to 0 to use any BSSID,
     *  in which case only the SSID is used to find the network.
     */
    char bssid[IEEEtypes_ADDRESS_SIZE];
    /** The channel for this network.
     *
     *  If this profile is used in uAP mode, this field
     *  specifies the channel to start the uAP interface on. Set this
     *  to 0 for auto channel selection.
     *
     *  If this profile is used in the station mode, this constrains the
     *  channel on which the network to connect should be present. Set this
     *  to 0 to allow the network to be found on any channel. */
    unsigned int channel;
    /** The secondary channel offset **/
    uint8_t sec_channel_offset;
    /** The ACS (auto channel selection) band if set channel to 0. */
    uint16_t acs_band;
    /** RSSI (received signal strength indicator) value. */
    int rssi;
#if CONFIG_SCAN_WITH_RSSIFILTER
    /** Specify RSSI threshold (dBm) for scan */
    short rssi_threshold;
#endif
#if CONFIG_WPA_SUPP
    /** HT capabilities info field within HT capabilities information element */
    unsigned short ht_capab;
#if CONFIG_11AC
    /** VHT capabilities info field within VHT capabilities information element */
    unsigned int vht_capab;
    /** VHT bandwidth */
    unsigned char vht_oper_chwidth;
#endif
#if CONFIG_11AX
    /** HE bandwidth */
    unsigned char he_oper_chwidth;
#endif
#endif
    /** BSS type */
    enum wlan_bss_type type;
    /** The network Wi-Fi mode enum wlan_bss_role. Set this
     *  to specify what type of Wi-Fi network mode to use.
     *  This can either be \ref WLAN_BSS_ROLE_STA for use in
     *  the station mode, or it can be \ref WLAN_BSS_ROLE_UAP
     *  for use in the uAP mode. */
    enum wlan_bss_role role;
    /** The network security configuration specified by struct
     * wlan_network_security for the network. */
    struct wlan_network_security security;
    /** The network IP address configuration specified by struct
     * wlan_ip_config that should be associated with this interface. */
    struct wlan_ip_config ip;
#if CONFIG_WPA2_ENTP
    /** WPA2 Enterprise identity, the max can be up to 256 characters */
    char identity[IDENTITY_MAX_LENGTH];
#if CONFIG_PEAP_MSCHAPV2
    char anonymous_identity[IDENTITY_MAX_LENGTH];
    /** password string */
    char password[PASSWORD_MAX_LENGTH];
#endif
#endif

    /* Private Fields */

    /**
     * If set to 1, the ssid field contains the specific SSID for this
     * network.
     * the Wi-Fi connection manager can only connect to networks with matching SSID
     * matches.  If set to 0, the ssid field contents are not used when
     * deciding whether to connect to a network or not. The BSSID field is used
     * instead and any network with matching BSSID matches is accepted.
     *
     * This field can be set to 1 if the network is added with the SSID
     * specified (not an empty string), otherwise it is set to 0.
     */
    unsigned ssid_specific : 1;
#if CONFIG_DRIVER_OWE
    /**
     * If set to 1, the ssid field contains the transitional SSID for this
     * network.
     */
    unsigned trans_ssid_specific : 1;
#endif
    /** If set to 1, the bssid field contains the specific BSSID for this
     *  network. The Wi-Fi connection manager cannot connect to any other
     *  network with the same SSID unless the BSSID matches.  If set to 0, the
     *  Wi-Fi connection manager can connect to any network whose SSID matches.
     *
     *  This field set to 1 if the network is added with the BSSID
     *  specified (not set to all zeroes), otherwise it is set to 0. */
    unsigned bssid_specific : 1;
    /**
     * If set to 1, the channel field contains the specific channel for this
     * network. The Wi-Fi connection manager cannot look for this network on
     * any other channel. If set to 0, the Wi-Fi connection manager can look
     * for this network on any available channel.
     *
     * This field is set to 1 if the network is added with the channel
     * specified (not set to 0), otherwise it is set to 0. */
    unsigned channel_specific : 1;
    /**
     * If set to 0, any security that matches is used. This field is
     * internally set when the security type parameter above is set to
     * WLAN_SECURITY_WILDCARD.
     */
    unsigned security_specific : 1;
#if CONFIG_WPS2
    /** This indicates this network is used as an internal network for
     * WPS */
    unsigned wps_specific : 1;
#endif

    /** The network supports 802.11N. */
    unsigned dot11n : 1;
#if CONFIG_11AC
    /** The network supports 802.11AC. */
    unsigned dot11ac : 1;
#endif
#if CONFIG_11AX
    /** The network supports 802.11AX. */
    unsigned dot11ax : 1;
#endif

#if CONFIG_11R
    /** Mobility Domain ID */
    uint16_t mdid;
    /** The network uses FT 802.1x security */
    unsigned ft_1x : 1;
    /** The network uses FT PSK security */
    unsigned ft_psk : 1;
    /** The network uses FT SAE security */
    unsigned ft_sae : 1;
#endif
#if CONFIG_DRIVER_OWE
    /** OWE (opportunistic wireless encryption) Transition mode */
    unsigned int owe_trans_mode;
    /** The network transitional SSID, represented as a C string of up to 32 characters
     *  in length.
     */
    char trans_ssid[IEEEtypes_SSID_SIZE + 1];
    /** Transitional SSID length */
    unsigned int trans_ssid_len;
#endif
    /** Beacon period of associated BSS */
    uint16_t beacon_period;
    /** DTIM period of associated BSS */
    uint8_t dtim_period;
#if CONFIG_WIFI_CAPA
    /** Wi-Fi capabilities of the uAP network 802.11n, 802.11ac or/and 802.11ax */
    uint8_t wlan_capa;
#endif
#if CONFIG_11V
    /** BTM mode */
    uint8_t btm_mode;
    /** BSS transition support */
    bool bss_transition_supported;
#endif
#if CONFIG_11K
    /** Neighbor report support */
    bool neighbor_report_supported;
#endif
};

/** This structure is for IEEE PS (power save) configuration */
struct wlan_ieeeps_config
{
    /** The interval that STA sends null packet */
    t_u32 ps_null_interval;
    /** The count of listen interval */
    t_u32 multiple_dtim_interval;
    /** Periodic interval that STA listens to AP beacons */
    t_u32 listen_interval;
    /** Periodic awake period for adhoc networks */
    t_u32 adhoc_awake_period;
    /** Beacon miss timeout in milliseconds */
    t_u32 bcn_miss_timeout;
    /** The delay of enabling IEEE-PS in milliseconds */
    t_s32 delay_to_ps;
    /** PS mode,
     * 1: PS-auto mode,
     * 2: PS-poll mode,
     * 3: PS-null mode. */
    t_u32 ps_mode;
};

#if CONFIG_WIFI_TX_PER_TRACK
/** TX PER tracking structure
 * Driver sets TX per tracking statistic to firmware.
 * Firmware can check TX packet error rate periodically and
 * report per to host if per is high.
 */
struct wlan_tx_pert_info
{
    /** Enable/Disable TX per tracking */
    t_u8 tx_pert_check;
    /** Check period (unit sec) */
    t_u8 tx_pert_check_peroid;
    /** (Fail TX packet)/(Total TX packet) ratio (unit 10%)
     * default: 5
     */
    t_u8 tx_pert_check_ratio;
    /** A watermark of check number (default 5) */
    t_u16 tx_pert_check_num;
};
#endif
typedef enum
{
    CLI_DISABLE_WIFI,
    CLI_ENABLE_WIFI,
    CLI_RESET_WIFI,
} cli_reset_option;

enum wlan_mon_task_event
{
    HOST_SLEEP_HANDSHAKE = 1,
    HOST_SLEEP_EXIT,
    WIFI_RECOVERY_REQ,
};

#if CONFIG_HOST_SLEEP
enum wlan_hostsleep_state
{
    HOST_SLEEP_DISABLE,
    HOST_SLEEP_ONESHOT,
    HOST_SLEEP_PERIODIC,
};

#define WLAN_HOSTSLEEP_SUCCESS    1
#define WLAN_HOSTSLEEP_IN_PROCESS 2
#define WLAN_HOSTSLEEP_FAIL       3
#endif

#if CONFIG_TX_RX_HISTOGRAM
struct wlan_txrx_histogram_info
{
    /**  Enable or disable  */
    t_u8 enable;
    /** Choose to get TX, RX or both */
    t_u16 action;
};

#define FLAG_TX_HISTOGRAM       0x01
#define FLAG_RX_HISTOGRAM       0x02
#define DISABLE_TX_RX_HISTOGRAM 0x00
#define ENABLE_TX_RX_HISTOGRAM  0x01
#define GET_TX_RX_HISTOGRAM     0x02

/** Sum of TX packets for HT (802.11n high throughput) rate. */
typedef struct _tx_pkt_ht_rate_info
{
    /** Sum of TX packets for HT rate. The array index represents MSC0~MCS15,
    the following array indexes have the same effect. */
    t_u32 htmcs_txcnt[16];
    /** Sum of TX short GI (guard interval) packets for HT rate. */
    t_u32 htsgi_txcnt[16];
    /** Sum of TX STBC (space time block code) packets for HT rate. */
    t_u32 htstbcrate_txcnt[16];
} tx_pkt_ht_rate_info;

/** Sum of TX packets for VHT (802.11ac very high throughput) rate. */
typedef struct _tx_pkt_vht_rate_info
{
	/** Sum of TX packets for VHT rate. The array index represents MSC0~MCS9,
	the following array indexes have the same effect. */
    t_u32 vhtmcs_txcnt[10];
	/** Sum of TX short GI packets for HT mode. */
    t_u32 vhtsgi_txcnt[10];
	/** Sum of TX STBC (space time block code) packets for VHT mode. */
    t_u32 vhtstbcrate_txcnt[10];
} tx_pkt_vht_rate_info;

/** Sum of TX packets for HE (802.11ax high efficiency) rate. */
typedef struct _tx_pkt_he_rate_info
{
    /** Sum of TX packets for HE rate. The array index represents MSC0~MCS11,
	the following array indexes have the same effect. */
    t_u32 hemcs_txcnt[12];
    /** Sum of TX STBC (space time block code) packets for HE rate. */
    t_u32 hestbcrate_txcnt[12];
} tx_pkt_he_rate_info;

/** Sum of TX packets. */
typedef struct _tx_pkt_rate_info
{
    /** Sum of TX NSS (N*N MIMO spatial stream) packets.
    nss_txcnt[0] is for NSS 1,
    nss_txcnt[1] is for NSS 2.
    */
    t_u32 nss_txcnt[2];
    /** Sum of TX packets for three bandwidths.
    bandwidth_txcnt[0] is for 20MHz,
    bandwidth_txcnt[1] is for 40MHz,
    bandwidth_txcnt[2] is for 80MHz.
    */
    t_u32 bandwidth_txcnt[3];
    /** Sum of RX packets for four preamble format types.
    preamble_txcnt[0] is for preamble format 0,
    preamble_txcnt[1] is for preamble format 1,
    preamble_txcnt[2] is for preamble format 2,
    preamble_txcnt[3] is for preamble format 3,
    */
    t_u32 preamble_txcnt[4];
	/** Sum of TX LDPC (low density parity check) packets. */
    t_u32 ldpc_txcnt;
    /** Sum of TX RTS (request to send) packets */
    t_u32 rts_txcnt;
    /** RSSI of ACK packet */
    t_s32 ack_RSSI;
} tx_pkt_rate_info;

/** Sum of RX packets for HT (802.11n high throughput) rate. */
typedef struct _rx_pkt_ht_rate_info
{
    /** Sum of RX packets for HT rate. The array index represents MSC0~MCS15,
    the following array indexes have the same effect.
    */
    t_u32 htmcs_rxcnt[16];
	/** Sum of TX short GI (guard interval) packets for HT rate. */
    t_u32 htsgi_rxcnt[16];
	/** Sum of TX STBC (space time block code) packets for HT rate. */
    t_u32 htstbcrate_rxcnt[16];
} rx_pkt_ht_rate_info;

/** Sum of RX packets for VHT (802.11ac very high throughput) rate. */
typedef struct _rx_pkt_vht_rate_info
{
	/** Sum of RX packets for VHT rate. The array index represents MSC0~MCS9,
	the following array indexes have the same effect. */
    t_u32 vhtmcs_rxcnt[10];
    /** Sum of RX short GI (guard interval) packets for VHT rate. */
    t_u32 vhtsgi_rxcnt[10];
	/** Sum of RX STBC (space time block code) packets for VHT rate. */
    t_u32 vhtstbcrate_rxcnt[10];
} rx_pkt_vht_rate_info;

/** Sum of RX packets for HE (802.11ax high efficiency) rate. */
typedef struct _rx_pkt_he_rate_info
{
    /** Sum of RX packets for HE rate. The array index represents MSC0~MCS11,
	the following array indexes have the same effect. */
    t_u32 hemcs_rxcnt[12];
    /** Sum of RX STBC (space time block code) packets for HE rate. */
    t_u32 hestbcrate_rxcnt[12];
} rx_pkt_he_rate_info;

/** Sum of RX packets. */
typedef struct _rx_pkt_rate_info
{
    /** Sum of RX NSS (N*N MIMO spatial stream) packets. 
    nss_txcnt[0] is for NSS 1,
    nss_txcnt[1] is for NSS 2. 
    */
    t_u32 nss_rxcnt[2];
    /** Sum of received packets for all STBC rates. */
    t_u32 nsts_rxcnt;
    /** Sum of received packets for three bandwidth types.
    bandwidth_rxcnt[0] is for 20MHz,
    bandwidth_rxcnt[1] is for 40MHz,
    bandwidth_rxcnt[2] is for 80MHz.
    */
    t_u32 bandwidth_rxcnt[3];
    /** Sum of received packets for four preamble format types.
    preamble_txcnt[0] is for preamble format 0,
    preamble_txcnt[1] is for preamble format 1,
    preamble_txcnt[2] is for preamble format 2,
    preamble_txcnt[3] is for preamble format 3,
    preamble_txcnt[4] and preamble_txcnt[5] are as reserved.
    */
    t_u32 preamble_rxcnt[6];
    /** Sum of packets for TX LDPC packets. */
    t_u32 ldpc_txbfcnt[2];
    /** Average RSSI */
    t_s32 rssi_value[2];
    /** RSSI value of path A */
    t_s32 rssi_chain0[4];
    /** RSSI value of path B */
    t_s32 rssi_chain1[4];
} rx_pkt_rate_info;
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
#define TX_AMPDU_RTS_CTS            0
#define TX_AMPDU_CTS_2_SELF         1
#define TX_AMPDU_DISABLE_PROTECTION 2
#define TX_AMPDU_DYNAMIC_RTS_CTS    3

/** Set protection mode for the transmit AMPDU packet */
typedef struct _tx_ampdu_prot_mode_para
{
    /**
    mode,
    0: set RTS/CTS mode,
    1: set CTS to self mode,
    2: disable protection mode,
    3: set dynamic RTS/CTS mode.
    */
    int mode;
} tx_ampdu_prot_mode_para;
#endif

typedef wifi_uap_client_disassoc_t wlan_uap_client_disassoc_t;

#if CONFIG_INACTIVITY_TIMEOUT_EXT
typedef wifi_inactivity_to_t wlan_inactivity_to_t;
#endif

/* Wi-Fi connection manager API */
/** Initialize the Wi-Fi driver and create the Wi-Fi driver thread.
 *
 * \param[in]  fw_start_addr: Start address of the Wi-Fi firmware.
 * \param[in]  size: Size of the Wi-Fi firmware.
 *
 * \return WM_SUCCESS if the Wi-Fi connection manager service has
 *         initialized successfully.
 * \return Negative value if initialization failed.
 */
int wlan_init(const uint8_t *fw_start_addr, const size_t size);

#if CONFIG_FW_DNLD_ASYNC
/** Initialize Wi-Fi driver and create the Wi-Fi driver thread in non blocking mode.
 * This API creates a new thread and performs \ref wlan_init and \ref wlan_start API
 * calls internally.
 *
 * \param[in]  fw_start_addr: Start address of the Wi-Fi firmware.
 * \param[in]  size: Size of the Wi-Fi firmware.
 * \param[in]  cb: A pointer to a callback function that handles Wi-Fi events. All
 * further WLCMGR events can be notified in this callback. Refer to enum
 * \ref wlan_event_reason for the various events for which this callback is called.
 *
 * \return WM_SUCCESS if the Wi-Fi connection manager service has
 *         initialized successfully.
 * \return Negative value if initialization failed.
 */
int wlan_init_nb(const uint8_t *fw_start_addr, const size_t size, int (*cb)(enum wlan_event_reason reason, void *data));
#endif

/** Start the Wi-Fi connection manager service.
 *
 * This function starts the Wi-Fi connection manager.
 *
 * \note The status of the Wi-Fi connection manager is notified asynchronously
 * through the callback, \a cb, with a WLAN_REASON_INITIALIZED event
 * (if initialization succeeded) or WLAN_REASON_INITIALIZATION_FAILED
 * (if initialization failed).
 * If the Wi-Fi connection manager fails to initialize, the caller should
 * stop Wi-Fi connection manager via wlan_stop() and try wlan_start() again.
 *
 * \param[in] cb: A pointer to a callback function that handles Wi-Fi events. All
 * further WLCMGR events can be notified in this callback. Refer to enum
 * \ref wlan_event_reason for the various events for which this callback is called.
 *
 * \return WM_SUCCESS if the Wi-Fi connection manager service has started
 *         successfully.
 * \return -WM_E_INVAL if the \a cb pointer is NULL.
 * \return -WM_FAIL if an internal error occurred.
 * \return WLAN_ERROR_STATE if the Wi-Fi connection manager is already running.
 */
int wlan_start(int (*cb)(enum wlan_event_reason reason, void *data));

/** Stop the Wi-Fi connection manager service.
 *
 *  This function stops the Wi-Fi connection manager, causing the station interface
 *  to disconnect from the currently connected network and stop the
 * uAP interface.
 *
 *  \return WM_SUCCESS if the Wi-Fi connection manager service has been
 *          stopped successfully.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not
 *          running.
 */
int wlan_stop(void);

/** Deinitialize the Wi-Fi driver, send a shutdown command to the Wi-Fi firmware
 *  and delete the Wi-Fi driver thread.
 *  \param[in] action: Additional action to be taken with deinit.
 *                    Should input 0 here.
 */
void wlan_deinit(int action);

#if CONFIG_WPS2
/** Generate valid pin code for WPS session.
 *
 *  This function generate pin for WPS pin session.
 *
 * \param[out]  pin: A pointer to WPS pin to be generated.
 */
void wlan_wps_generate_pin(uint32_t *pin);

/** Start WPS pin session.
 *
 *  This function starts WPS pin session.
 *
 * \param[in]  pin: Pin for WPS session.
 *
 * \return WM_SUCCESS if the pin entered is valid.
 * \return -WM_FAIL if invalid pin entered.
 */
int wlan_start_wps_pin(uint32_t pin);

/** Start WPS PBC session.
 *
 *  This function starts WPS PBC session.
 *
 * \return  WM_SUCCESS if successful
 */
int wlan_start_wps_pbc(void);
/**
 * Set None/WPS/802.1x session.
 *
 *\param[in] session:       0 -- PROV_NON_SESSION_ATTEMPT, 1 -- PROV_WPS_SESSION_ATTEMPT, 2 -- PROV_ENTP_SESSION_ATTEMPT.
 */
void wlan_set_prov_session(int session);

/**
 * Get connect session type.
 *
 * \return 0 -- PROV_NON_SESSION_ATTEMPT, 1 -- PROV_WPS_SESSION_ATTEMPT, 2 -- PROV_ENTP_SESSION_ATTEMPT.
 */
int wlan_get_prov_session(void);
#endif

/** Stop and remove all Wi-Fi network profiles.
 *
 *  \return WM_SUCCESS if successful otherwise return -WM_E_INVAL.
 */
int wlan_remove_all_network_profiles(void);

/** Reset the driver.
 *  \param[in] ResetOption: Option including enable, disable or reset Wi-Fi driver
 *  can be chosen.
 */
void wlan_reset(cli_reset_option ResetOption);
/** Stop and remove all Wi-Fi network (access point).
 *
 *  \return WM_SUCCESS if successful.
 */
int wlan_remove_all_networks(void);

/**
 * This API destroys all tasks.
 */
void wlan_destroy_all_tasks(void);
/** Retrieve the status information of if Wi-Fi started.
 *
 *  \return TRUE if Wi-Fi network is started.
 *  \return FALSE if not started.
 */
int wlan_is_started(void);

#if CONFIG_NCP
/** uAP provisioning deinit callback function */
void wlan_register_uap_prov_deinit_cb(int (*cb)(void));
/** uAP provisioning cleanup callback function */
void wlan_register_uap_prov_cleanup_cb(void (*cb)(void));
/** Stop all Wi-Fi network.
 *
 *  \return WM_SUCCESS if successful.
 */
int wlan_stop_all_networks(void);
#endif

#if CONFIG_RX_ABORT_CFG
struct wlan_rx_abort_cfg
{
    /** Enable/Disable RX abort configuration */
    t_u8 enable;
    /** RX weak RSSI threshold */
    int rssi_threshold;
};
#endif

#if CONFIG_RX_ABORT_CFG_EXT
struct wlan_rx_abort_cfg_ext
{
    /** Enable/Disable dyn RX abort on weak packet RSSI */
    int enable;
    /** Specify RSSI margin */
    int rssi_margin;
    /** Specify ceil RSSI threshold */
    int ceil_rssi_threshold;
    /** Specify floor RSSI threshold */
    int floor_rssi_threshold;
    /** Current dynamic RSSI threshold */
    int current_dynamic_rssi_threshold;
    /** RSSI config: default or user configured */
    int rssi_default_config;
    /** EDMAC status */
    int edmac_enable;
};
#endif

#if CONFIG_CCK_DESENSE_CFG
#define CCK_DESENSE_MODE_DISABLED 0
#define CCK_DESENSE_MODE_DYNAMIC  1
#define CCK_DESENSE_MODE_DYN_ENH  2

struct wlan_cck_desense_cfg
{
    /** CCK (complementary code keying) desense mode: 0:disable 1:normal 2:dynamic */
    t_u16 mode;
    /** Specify RSSI margin */
    int margin;
    /** Specify ceil RSSI threshold */
    int ceil_thresh;
    /** CCK (complementary code keying) desense "on" interval count */
    int num_on_intervals;
    /** CCK desense "off" interval count */
    int num_off_intervals;
};
#endif
#if CONFIG_RX_ABORT_CFG
/**
 * Set/Get RX abort configuration to/from firmware.
 *
 * \param[in,out] cfg: A pointer to information buffer
 * \param[in] action: Command action: get or set
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_get_rx_abort_cfg(struct wlan_rx_abort_cfg *cfg, t_u16 action);
#endif

#if CONFIG_RX_ABORT_CFG_EXT
/**
 * Set the dynamic RX abort configuration to firmware.
 *
 * \param[in] cfg: A pointer to information buffer
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_rx_abort_cfg_ext(const struct wlan_rx_abort_cfg_ext *cfg);

/**
 * Get the dynamic RX abort configuration from firmware.
 *
 * \param[out] cfg: A pointer to information buffer
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_rx_abort_cfg_ext(struct wlan_rx_abort_cfg_ext *cfg);
#endif

#if CONFIG_CCK_DESENSE_CFG
/**
 * Set/Get CCK (complementary code keying) desense configuration to/from firmware.
 *
 * \param[in,out] cfg: A pointer to information buffer
 * \param[in] action: get or set.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_get_cck_desense_cfg(struct wlan_cck_desense_cfg *cfg, t_u16 action);
#endif

/** Initialize the uAP network information.
 *
 * This API initializes a uAP network with default configurations.
 * The network ssid, passphrase is initialized to NULL.
 * Channel is set to auto. The IP Address of the
 * uAP interface is 192.168.10.1/255.255.255.0. The network name is set to
 * 'uap-network'.
 *
 * \param[out] net: Pointer to the initialized uAP network
 */
void wlan_initialize_uap_network(struct wlan_network *net);

/** Initialize the station network information.
 *
 * This API initializes a station network with default configurations.
 * The network ssid, passphrase is initialized to NULL.
 * Channel is set to auto.
 *
 * \param[out] net: Pointer to the initialized station network
 */
void wlan_initialize_sta_network(struct wlan_network *net);

/** Add a network profile to the list of known networks.
 *
 *  This function copies the contents of \a network to the list of known
 *  networks in the Wi-Fi connection manager. The network's 'name' field is
 *  unique and between \ref WLAN_NETWORK_NAME_MIN_LENGTH and \ref
 *  WLAN_NETWORK_NAME_MAX_LENGTH characters.  The network must specify at least
 *  an SSID or BSSID.  the Wi-Fi connection manager can store up to
 *  WLAN_MAX_KNOWN_NETWORKS networks.
 *
 *  \note Profiles for the station interface may be added only when the station
 *  interface is in the \ref WLAN_DISCONNECTED or \ref WLAN_CONNECTED state.
 *
 *  \note This API can be used to add profiles for station or
 *  uAP interfaces.
 *
 *  \note Set mfpc and mfpr to -1 for default configurations.
 *
 *  \param[in] network: A pointer to the \ref wlan_network that can be copied
 *             to the list of known networks in the Wi-Fi connection manager
 *             successfully.
 *
 *  \return WM_SUCCESS if the contents pointed to by \a network have been
 *          added to the Wi-Fi connection manager.
 *  \return -WM_E_INVAL if \a network is NULL or the network name
 *          is not unique or the network name length is not valid
 *          or network security is \ref WLAN_SECURITY_WPA3_SAE but
 *          Management Frame Protection Capable is not enabled.
 *          in \ref wlan_network_security field. if network security type is
 *          \ref WLAN_SECURITY_WPA or \ref WLAN_SECURITY_WPA2 or \ref
 *          WLAN_SECURITY_WPA_WPA2_MIXED, but the passphrase length is less
 *          than 8 or greater than 63, or the psk length equal to 64 but not
 *          hexadecimal digits. if network security type is \ref WLAN_SECURITY_WPA3_SAE,
 *          but the password length is less than 8 or greater than 255.
 *          if network security type is \ref WLAN_SECURITY_WEP_OPEN or
 *          \ref WLAN_SECURITY_WEP_SHARED.
 *  \return -WM_E_NOMEM if there was no room to add the network.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager
 *          was running and not in the \ref WLAN_DISCONNECTED,
 *          \ref WLAN_ASSOCIATED or \ref WLAN_CONNECTED state.
 */
int wlan_add_network(struct wlan_network *network);

/** Remove a network profile from the list of known networks.
 *
 *  This function removes a network (identified by its name) from the WLAN
 *  Connection Manager, disconnecting from that network if connected.
 *
 *  \note This function is asynchronous if it is called while the WLAN
 *  Connection Manager is running and connected to the network to be removed.
 *  In that case, the Wi-Fi connection manager can disconnect from the network
 *  and generate an event with reason \ref WLAN_REASON_USER_DISCONNECT. This
 *  function is synchronous otherwise.
 *
 *  \note This API can be used to remove profiles for station or
 *  uAP interfaces. Station network can not be removed if it is
 *  in \ref WLAN_CONNECTED state and uAP network can not be removed
 *  if it is in \ref WLAN_UAP_STARTED state.
 *
 *  \param[in] name: A pointer to the string representing the name of the
 *             network to remove.
 *
 *  \return WM_SUCCESS if the network named \a name was removed from the
 *          Wi-Fi connection manager successfully. Otherwise,
 *          the network is not removed.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *          running and the station interface was not in the \ref
 *          WLAN_DISCONNECTED state.
 *  \return -WM_E_INVAL if \a name is NULL or the network was not found in
 *          the list of known networks.
 *  \return -WM_FAIL if an internal error occurred
 *          while trying to disconnect from the network specified for
 *          removal.
 */
int wlan_remove_network(const char *name);

/** Connect to a Wi-Fi network (access point).
 *
 *  When this function is called, Wi-Fi connection manager starts connection attempts
 *  to the network specified by \a name. The connection result can be notified
 *  asynchronously to the WLCMGR callback when the connection process has
 *  completed.
 *
 *  When connecting to a network, the event refers to the connection
 *  attempt to that network.
 *
 *  Calling this function when the station interface is in the \ref
 *  WLAN_DISCONNECTED state should, if successful, cause the interface to
 *  transition into the \ref WLAN_CONNECTING state. If the connection attempt
 *  succeeds, the station interface should transition to the \ref WLAN_CONNECTED state,
 *  otherwise it should return to the \ref WLAN_DISCONNECTED state.  If this
 *  function is called while the station interface is in the \ref
 *  WLAN_CONNECTING or \ref WLAN_CONNECTED state, the Wi-Fi connection manager
 *  should first cancel its connection attempt or disconnect from the network,
 *  respectively, and generate an event with reason \ref
 *  WLAN_REASON_USER_DISCONNECT. This should be followed by a second event that
 *  reports the result of the new connection attempt.
 *
 *  If the connection attempt was successful the WLCMGR callback is notified
 *  with the event \ref WLAN_REASON_SUCCESS, while if the connection attempt
 *  fails then either of the events, \ref WLAN_REASON_NETWORK_NOT_FOUND, \ref
 *  WLAN_REASON_NETWORK_AUTH_FAILED, \ref WLAN_REASON_CONNECT_FAILED
 *  or \ref WLAN_REASON_ADDRESS_FAILED are reported as appropriate.
 *
 *  \param[in] name: A pointer to a string representing the name of the network
 *              to connect to.
 *
 *  \return WM_SUCCESS if a connection attempt was started successfully
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running.
 *  \return -WM_E_INVAL if there are no known networks to connect to
 *          or the network specified by \a name is not in the list
 *          of known networks or network \a name is NULL.
 *  \return -WM_FAIL if an internal error has occurred.
 */
int wlan_connect(char *name);

/** Connect to a Wi-Fi network (access point) with options.
 *
 *  When this function is called, the Wi-Fi connection manager starts connection attempts
 *  to the network specified by \a name. The connection result should be notified
 *  asynchronously to the WLCMGR callback when the connection process has
 *  completed.
 *
 *  When connecting to a network, the event refers to the connection
 *  attempt to that network.
 *
 *  Calling this function when the station interface is in the \ref
 *  WLAN_DISCONNECTED state should, if successful, cause the interface to
 *  transition into the \ref WLAN_CONNECTING state.  If the connection attempt
 *  succeeds, the station interface should transition to the \ref WLAN_CONNECTED state,
 *  otherwise it should return to the \ref WLAN_DISCONNECTED state.  If this
 *  function is called while the station interface is in the \ref
 *  WLAN_CONNECTING or \ref WLAN_CONNECTED state, the Wi-Fi connection manager
 *  should first cancel its connection attempt or disconnect from the network,
 *  respectively, and generate an event with reason \ref
 *  WLAN_REASON_USER_DISCONNECT. This should be followed by a second event that
 *  reports the result of the new connection attempt.
 *
 *  If the connection attempt was successful the WLCMGR callback is notified
 *  with the event \ref WLAN_REASON_SUCCESS, while if the connection attempt
 *  fails then either of the events, \ref WLAN_REASON_NETWORK_NOT_FOUND, \ref
 *  WLAN_REASON_NETWORK_AUTH_FAILED, \ref WLAN_REASON_CONNECT_FAILED
 *  or \ref WLAN_REASON_ADDRESS_FAILED are reported as appropriate.
 *
 *  \param[in] name: A pointer to a string representing the name of the network
 *              to connect to.
 *  \param[in] skip_dfs: Option to skip DFS channel when doing scan.
 *
 *  \return WM_SUCCESS if a connection attempt was started successfully
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running.
 *  \return -WM_E_INVAL if there are no known networks to connect to
 *          or the network specified by \a name is not in the list
 *          of known networks or network \a name is NULL.
 *  \return -WM_FAIL if an internal error has occurred.
 */
int wlan_connect_opt(char *name, bool skip_dfs);

/** Reassociate to a Wi-Fi network (access point).
 *
 *  When this function is called, the Wi-Fi connection manager starts reassociation
 *  attempts using same SSID as currently connected network .
 *  The connection result should be notified asynchronously to the WLCMGR
 *  callback when the connection process has completed.
 *
 *  When connecting to a network, the event refers to the connection
 *  attempt to that network.
 *
 *  Calling this function when the station interface is in the \ref
 *  WLAN_DISCONNECTED state should have no effect.
 *
 *  Calling this function when the station interface is in the \ref
 *  WLAN_CONNECTED state should, if successful, cause the interface to
 *  reassociate to another network (access point).
 *
 *  If the connection attempt was successful the WLCMGR (Wi-Fi command manager) callback is notified
 *  with the event \ref WLAN_REASON_SUCCESS, while if the connection attempt
 *  fails then either of the events, \ref WLAN_REASON_NETWORK_AUTH_FAILED,
 *  \ref WLAN_REASON_CONNECT_FAILED or \ref WLAN_REASON_ADDRESS_FAILED
 *  are reported as appropriate.
 *
 *  \return WM_SUCCESS if a reassociation attempt was started successfully
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running.
 *          or Wi-Fi connection manager was not in \ref WLAN_CONNECTED state.
 *  \return -WM_E_INVAL if there are no known networks to connect to
 *  \return -WM_FAIL if an internal error has occurred.
 */
int wlan_reassociate(void);

/** Disconnect from the current Wi-Fi network (access point).
 *
 *  When this function is called, the Wi-Fi connection manager attempts to disconnect
 *  the station interface from its currently connected network (or cancel an in-progress
 *  connection attempt) and return to the \ref WLAN_DISCONNECTED state. Calling
 *  this function has no effect if the station interface is already
 *  disconnected.
 *
 *  \note This is an asynchronous function and successful disconnection should be
 *  notified using the \ref WLAN_REASON_USER_DISCONNECT.
 *
 * \return  WM_SUCCESS if successful
 * \return  WLAN_ERROR_STATE otherwise
 */
int wlan_disconnect(void);

/** Start a Wi-Fi network (access point).
 *
 *  When this function is called, the Wi-Fi connection manager starts the network
 *  specified by \a name. The network with the specified \a name is
 *  first added using \ref wlan_add_network and is a uAP network with
 *  a valid SSID.
 *
 *  \note The WLCMGR callback is asynchronously notified of the status. On
 *  success, the event \ref WLAN_REASON_UAP_SUCCESS is reported, while on
 *  failure, the event \ref WLAN_REASON_UAP_START_FAILED is reported.
 *
 *  \param[in] name: A pointer to string representing the name of the network
 *             to connect to.
 *
 *  \return WM_SUCCESS if successful.
 *  \return WLAN_ERROR_STATE if in power save state or uAP already running.
 *  \return -WM_E_INVAL if \a name was NULL or the network \a
 *          name was not found or it not have a specified SSID.
 */
int wlan_start_network(const char *name);

/** Stop a Wi-Fi network (access point).
 *
 *  When this function is called, the Wi-Fi connection manager stops the network
 *  specified by \a name. The specified network is a valid uAP
 *  network that has already been started.
 *
 *  \note The WLCMGR callback is asynchronously notified of the status. On
 *  success, the event \ref WLAN_REASON_UAP_STOPPED is reported, while on
 *  failure, the event \ref WLAN_REASON_UAP_STOP_FAILED is reported.
 *
 *  \param[in] name: A pointer to a string representing the name of the network
 *             to stop.
 *
 *  \return WM_SUCCESS if successful.
 *  \return WLAN_ERROR_STATE if uAP is in power save state.
 *  \return -WM_E_INVAL if \a name was NULL or the network \a
 *          name was not found or that the network \a name is not a uAP
 *          network or it is a uAP network but does not have a specified
 *          SSID.
 */
int wlan_stop_network(const char *name);

/** Retrieve the Wi-Fi MAC address of the station interface.
 *
 *  This function copies the MAC address of the Wi-Fi station interface to
 *  the 6-byte array pointed to by \a dest. In the event of an error, nothing
 *  is copied to \a dest.
 *
 *  \param[out] dest: A pointer to a 6-byte array where the MAC address should be
 *              copied.
 *
 *  \return WM_SUCCESS if the MAC address was copied.
 *  \return -WM_E_INVAL if \a dest is NULL.
 */
int wlan_get_mac_address(uint8_t *dest);

/** Retrieve the Wi-Fi MAC address of the uAP interface.
 *
 *  This function copies the MAC address of the Wi-Fi uAP interface to
 *  the 6-byte array pointed to by \a dest. In the event of an error, nothing
 *  is copied to \a dest.
 *
 *  \param[out] dest: A pointer to a 6-byte array where the MAC address can be
 *              copied.
 *
 *  \return WM_SUCCESS if the MAC address was copied.
 *  \return -WM_E_INVAL if \a dest is NULL.
 */
int wlan_get_mac_address_uap(uint8_t *dest);

/** Retrieve the IP address configuration of the station interface.
 *
 *  This function retrieves the IP address configuration
 *  of the station interface and copies it to the memory
 *  location pointed to by \a addr.
 *
 *  \note This function may only be called when the station interface is in the
 *  \ref WLAN_CONNECTED state.
 *
 *  \param[out] addr: A pointer to the \ref wlan_ip_config.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a addr is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running or was
 *          not in the \ref WLAN_CONNECTED state.
 *  \return -WM_FAIL if an internal error
 *          occurred when retrieving IP address information from the
 *          TCP stack.
 */
int wlan_get_address(struct wlan_ip_config *addr);

/** Retrieve the IP address of the uAP interface.
 *
 *  This function retrieves the current IP address configuration of the uAP
 *  and copies it to the memory location pointed to by \a addr.
 *
 *  \note This function may only be called when the uAP interface is in the
 *  \ref WLAN_UAP_STARTED state.
 *
 *  \param[out] addr: A pointer to the \ref wlan_ip_config.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a addr is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running or
 *          the uAP interface was not in the \ref WLAN_UAP_STARTED state.
 *  \return -WM_FAIL if an internal error
 *          occurred when retrieving IP address information from the
 *          TCP stack.
 */
int wlan_get_uap_address(struct wlan_ip_config *addr);

/** Retrieve the channel of the uAP interface.
 *
 *  This function retrieves the channel number of the uAP
 *  and copies it to the memory location pointed to by \a channel.
 *
 *  \note This function may only be called when the uAP interface is in the
 *  \ref WLAN_UAP_STARTED state.
 *
 *  \param[out] channel: A pointer to variable that stores channel number.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a channel is NULL.
 *  \return -WM_FAIL if an internal error has occurred.
 */
int wlan_get_uap_channel(int *channel);

/** Retrieve the current network configuration of the station interface.
 *
 *  This function retrieves the current network configuration of the station
 *  interface when the station interface is in the \ref WLAN_CONNECTED
 *  state.
 *
 *  \param[out] network: A pointer to the \ref wlan_network.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a network is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *          not running or not in the \ref WLAN_CONNECTED state.
 */
int wlan_get_current_network(struct wlan_network *network);

/** Retrieve the current network ssid of the station interface.
 *
 *  This function retrieves the current network ssid of the station
 *  interface when the station interface is in the \ref WLAN_CONNECTED
 *  state.
 *
 *  \param[out] ssid: A pointer to the ssid char string with NULL termination.
 *                   Maximum length is 32 (not include NULL termination).
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a ssid is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *          not running or not in the \ref WLAN_CONNECTED state.
 */
int wlan_get_current_network_ssid(char *ssid);

/** Retrieve the current network bssid of the station interface.
 *
 *  This function retrieves the current network bssid of the station
 *  interface when the station interface is in the \ref WLAN_CONNECTED
 *  state.
 *
 *  \param[out] bssid: A pointer to the bssid char string without NULL termination.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a bssid is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *          not running or not in the \ref WLAN_CONNECTED state.
 */
int wlan_get_current_network_bssid(char *bssid);

/** Retrieve the current network configuration of the uAP interface.
 *
 *  This function retrieves the current network configuration of the uAP
 *  interface when the uAP interface is in the \ref WLAN_UAP_STARTED state.
 *
 *  \param[out] network: A pointer to the \ref wlan_network.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a network is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *           not running or not in the \ref WLAN_UAP_STARTED state.
 */
int wlan_get_current_uap_network(struct wlan_network *network);

/** Retrieve the current network ssid of the uAP interface.
 *
 *  This function retrieves the current network ssid of the uAP
 *  interface when the uAP interface is in the \ref WLAN_UAP_STARTED state.
 *
 *  \param[out] ssid: A pointer to the ssid char string with NULL termination.
 *                   Maximum length is 32 (not include NULL termination).
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a ssid is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *           not running or not in the \ref WLAN_UAP_STARTED state.
 */
int wlan_get_current_uap_network_ssid(char *ssid);

#if CONFIG_SCAN_WITH_RSSIFILTER
int wlan_set_rssi_threshold(int rssithr);
#endif

/** Retrieve the status information of the uAP interface.
 *
 *  \return TRUE if uAP interface is in \ref WLAN_UAP_STARTED state.
 *  \return FALSE otherwise.
 */
bool is_uap_started(void);

/** Retrieve the status information of the station interface.
 *
 *  \return TRUE if station interface is in or above the \ref WLAN_ASSOCIATED state.
 *  \return FALSE otherwise.
 */
bool is_sta_associated(void);

/** Retrieve the status information of the station interface.
 *
 *  \return TRUE if station interface is in \ref WLAN_CONNECTED state.
 *  \return FALSE otherwise.
 */
bool is_sta_connected(void);

/** Retrieve the status information of the ipv4 network of the station interface.
 *
 *  \return TRUE if ipv4 network of the station interface is in \ref WLAN_CONNECTED
 *  state.
 *  \return FALSE otherwise.
 */
bool is_sta_ipv4_connected(void);

#if CONFIG_IPV6
/** Retrieve the status information of the ipv6 network of the station interface.
 *
 *  \return TRUE if ipv6 network of the station interface is in \ref WLAN_CONNECTED
 *  state.
 *  \return FALSE otherwise.
 */
bool is_sta_ipv6_connected(void);
#endif

/** Retrieve the information about a known network using \a index.
 *
 *  This function retrieves the contents of a network at \a index in the
 *  list of known networks maintained by the Wi-Fi connection manager and
 *  copies it to the location pointed to by \a network.
 *
 *  \note \ref wlan_get_network_count() can be used to retrieve the number
 *  of known networks. \ref wlan_get_network() can be used to retrieve
 *  information about networks at \a index 0 to one minus the number of networks.
 *
 *  \note This function can be called regardless of whether the Wi-Fi connection
 *  manager is running or not. Calls to this function are synchronous.
 *
 *  \param[in] index: The index of the network to retrieve.
 *  \param[out] network: A pointer to the \ref wlan_network where the network
 *              configuration for the network at \a index can be copied.
 *
 *  \returns WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a network is NULL or \a index is out of range.
 */
int wlan_get_network(unsigned int index, struct wlan_network *network);

/** Retrieve information about a known network using \a name.
 *
 *  This function retrieves the contents of a named network in the
 *  list of known networks maintained by the Wi-Fi connection manager and
 *  copies it to the location pointed to by \a network.
 *
 *  \note This function can be called regardless of whether the Wi-Fi Connection
 *  Manager is running or not. Calls to this function are synchronous.
 *
 *  \param[in] name: The name of the network to retrieve.
 *  \param[out] network: A pointer to the \ref wlan_network where the network
 *              configuration for the network having name as \a name should be copied.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a network is NULL or \a name is NULL.
 */
int wlan_get_network_byname(char *name, struct wlan_network *network);

/** Retrieve the number of networks known to the Wi-Fi connection manager.
 *
 *  This function retrieves the number of known networks in the list maintained
 *  by the Wi-Fi connection manager and copies it to \a count.
 *
 *  \note This function can be called regardless of whether the Wi-Fi Connection
 *  Manager is running or not. Calls to this function are synchronous.
 *
 *  \param[out] count: A pointer to the memory location where the number of
 *              networks should be copied.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a count is NULL.
 */
int wlan_get_network_count(unsigned int *count);

/** Retrieve the connection state of the station interface.
 *
 *  This function retrieves the connection state of the station interface, which is
 *  one of \ref WLAN_DISCONNECTED, \ref WLAN_CONNECTING, \ref WLAN_ASSOCIATED
 *  or \ref WLAN_CONNECTED.
 *
 *  \param[out] state: A pointer to the \ref wlan_connection_state where the
 *         current connection state should be copied.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a state is NULL
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running.
 */
int wlan_get_connection_state(enum wlan_connection_state *state);

/** Retrieve the connection state of the uAP interface.
 *
 *  This function retrieves the connection state of the uAP interface, which is
 *  one of \ref WLAN_UAP_STARTED, or \ref WLAN_UAP_STOPPED.
 *
 *  \param[out] state: A pointer to the \ref wlan_connection_state where the
 *         current connection state should be copied.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a state is NULL
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was not running.
 */
int wlan_get_uap_connection_state(enum wlan_connection_state *state);

/** Scan for Wi-Fi networks.
 *
 *  When this function is called, the Wi-Fi connection manager starts scan
 *  for Wi-Fi networks. On completion of the scan the Wi-Fi connection manager
 *  can call the specified callback function \a cb. The callback function should then
 *  retrieve the scan results by using the \ref wlan_get_scan_result() function.
 *
 *  \note This function may only be called when the station interface is in the
 *  \ref WLAN_DISCONNECTED or \ref WLAN_CONNECTED state. scan is disabled
 *  in the \ref WLAN_CONNECTING state.
 *
 *  \note This function should block until it can issue a scan request if called
 *  while another scan is in progress.
 *
 *  \param[in] cb: A pointer to the function that should be called to handle scan
 *         results when they are available.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_NOMEM if failed to allocated memory for \ref
 *	     wlan_scan_params_v2_t structure.
 *  \return -WM_E_INVAL if \a cb scan result callback function pointer is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *           not running or not in the \ref WLAN_DISCONNECTED or \ref
 *           WLAN_CONNECTED states.
 *  \return -WM_FAIL if an internal error has occurred and
 *           the system is unable to scan.
 */
int wlan_scan(int (*cb)(unsigned int count));

/** Scan for Wi-Fi networks using options provided.
 *
 *  When this function is called, the Wi-Fi connection manager starts scanning
 *  for Wi-Fi networks. On completion of the scan the Wi-Fi connection manager
 *  should call the specified callback function \a t_wlan_scan_param.cb.
 *  The callback function should then retrieve the scan results by using
 *  the \ref wlan_get_scan_result() function.
 *
 *  \note This function may only be called when the station interface is in the
 *  \ref WLAN_DISCONNECTED or \ref WLAN_CONNECTED state. scan is disabled
 *  in the \ref WLAN_CONNECTING state.
 *
 *  \note This function can block until it issues a scan request if called
 *  while another scan is in progress.
 *
 *  \param[in] t_wlan_scan_param:  A \ref wlan_scan_params_v2_t structure holding
 *	       a pointer to function that should be called
 *	       to handle scan results when they are available,
 *	       SSID of a Wi-Fi network, BSSID of a Wi-Fi network,
 *	       number of channels with scan type information and number of
 *	       probes.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_NOMEM if failed to allocated memory for \ref
 *	     wlan_scan_params_v2_t structure.
 *  \return -WM_E_INVAL if \a cb scan result callback function pointer is NULL.
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager was
 *           not running or not in the \ref WLAN_DISCONNECTED or \ref
 *           WLAN_CONNECTED states.
 *  \return -WM_FAIL if an internal error has occurred and
 *           the system is unable to scan.
 */
int wlan_scan_with_opt(wlan_scan_params_v2_t t_wlan_scan_param);

/** Retrieve a scan result.
 *
 *  This function can be called to retrieve scan results when the Wi-Fi
 *  connection manager has finished scanning. It is called from within the
 *  scan result callback (see \ref wlan_scan()) as scan results are valid
 *  only in that context. The callback argument 'count' provides the number
 *  of scan results that can be retrieved and \ref wlan_get_scan_result() can
 *  be used to retrieve scan results at \a index 0 through that number.
 *
 *  \note This function may only be called in the context of the scan results
 *  callback.
 *
 *  \note Calls to this function are synchronous.
 *
 *  \param[in] index: The scan result to retrieve.
 *  \param[out] res: A pointer to the \ref wlan_scan_result where the scan
 *              result information should be copied.
 *
 *  \return WM_SUCCESS if successful.
 *  \return -WM_E_INVAL if \a res is NULL
 *  \return WLAN_ERROR_STATE if the Wi-Fi connection manager
 *          was not running
 *  \return -WM_FAIL if the scan result at \a
 *          index could not be retrieved (that is, \a index
 *          is out of range).
 */
int wlan_get_scan_result(unsigned int index, struct wlan_scan_result *res);

#ifdef WLAN_LOW_POWER_ENABLE
/**
 * Enable low power mode in Wi-Fi Firmware.
 *
 * \note When low power mode is enabled, the output power should be clipped at
 * ~+10dBm and the PA current is expected to be in the 80-90 mA
 * range for b/g/n modes.
 *
 * This function can be called to enable low power mode in firmware.
 * This should be called before \ref wlan_init() function.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 *
 */
int wlan_enable_low_pwr_mode(void);
#endif

/**
 * Configure Energy Detect MAC mode for the station in the Wi-Fi Firmware.
 *
 * \note When ED MAC mode is enabled,
 * the Wi-Fi Firmware can behave in the following way:
 *
 * When the background noise had reached the Energy Detect threshold or above,
 * the Wi-Fi chipset/module should hold the data transmission
 * until the condition is removed.
 * The 2.4GHz and 5GHz bands are configured separately.
 *
 * \param[in] wlan_ed_mac_ctrl:  Struct with following parameters
 *	 ed_ctrl_2g	     0  - disable EU adaptivity for 2.4GHz band
 *                           1  - enable EU adaptivity for 2.4GHz band
 *
 *       ed_offset_2g        0  - Default Energy Detect threshold (Default: 0x9)
 *                           offset value range: 0x80 to 0x7F
 *
 * \note If 5GH enabled then add following parameters
 *
 *       ed_ctrl_5g          0  - disable EU adaptivity for 5GHz band
 *                           1  - enable EU adaptivity for 5GHz band
 *
 *       ed_offset_5g        0  - Default Energy Detect threshold(Default: 0xC)
 *                           offset value range: 0x80 to 0x7F
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 *
 */
int wlan_set_ed_mac_mode(wlan_ed_mac_ctrl_t wlan_ed_mac_ctrl);

/**
 * Configure Energy Detect MAC mode for the uAP in the Wi-Fi firmware.
 *
 * \note When ED MAC mode is enabled,
 * the Wi-Fi Firmware can behave in the following way:
 *
 * When the background noise had reached the Energy Detect threshold or above,
 * the Wi-Fi chipset/module should hold data transmission
 * until the condition is removed.
 * The 2.4GHz and 5GHz bands are configured separately.
 *
 * \param[in] wlan_ed_mac_ctrl:  Struct with following parameters
 *	 ed_ctrl_2g	     0  - disable EU adaptivity for 2.4GHz band
 *                           1  - enable EU adaptivity for 2.4GHz band
 *
 *       ed_offset_2g        0  - Default energy detect threshold (Default: 0x9)
 *                           offset value range: 0x80 to 0x7F
 *
 * \note If 5GH enabled then add following parameters
 *
 *       ed_ctrl_5g          0  - disable EU adaptivity for 5GHz band
 *                           1  - enable EU adaptivity for 5GHz band
 *
 *       ed_offset_5g        0  - Default energy detect threshold(Default: 0xC)
 *                           offset value range: 0x80 to 0x7F
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 *
 */
int wlan_set_uap_ed_mac_mode(wlan_ed_mac_ctrl_t wlan_ed_mac_ctrl);

/**
 * This API can be used to get current ED MAC MODE configuration for station.
 *
 * \param[out] wlan_ed_mac_ctrl: A pointer to \ref wlan_ed_mac_ctrl_t
 * 			with parameters mentioned in above set API.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 *
 */
int wlan_get_ed_mac_mode(wlan_ed_mac_ctrl_t *wlan_ed_mac_ctrl);

/**
 * This API can be used to get current ED MAC MODE configuration for uAP.
 *
 * \param[out] wlan_ed_mac_ctrl: A pointer to \ref wlan_ed_mac_ctrl_t
 * 			with parameters mentioned in above set API.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 *
 */
int wlan_get_uap_ed_mac_mode(wlan_ed_mac_ctrl_t *wlan_ed_mac_ctrl);

/** Set the Wi-Fi calibration data in the Wi-Fi firmware.
 *
 * This function can be used to set the Wi-Fi calibration data in the firmware.
 * This should be call before \ref wlan_init() function.
 *
 * \param[in] cal_data: The calibration data buffer
 * \param[in] cal_data_size: Size of calibration data buffer.
 *
 */
void wlan_set_cal_data(const uint8_t *cal_data, const unsigned int cal_data_size);

/** Set the Wi-Fi MAC Address in the Wi-Fi firmware.
 *
 * This function can be used to set Wi-Fi MAC Address in firmware.
 * When called after Wi-Fi initialization done, the incoming MAC is treated as the STA MAC address directly. And mac[4] plus 1, the
 * modified MAC is used as the uAP MAC address.
 *
 * \param[in] MAC: The MAC Address in 6 bytes array format like
 *                uint8_t mac[] = { 0x00, 0x50, 0x43, 0x21, 0x19, 0x6E};
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_mac_addr(uint8_t *mac);

/** Set the Wi-Fi MAC address for the STA in the Wi-Fi firmware.
 *
 * This function can be used to set the Wi-Fi MAC address for the station in the firmware.
 * Should be called after Wi-Fi initialization done. It sets the station MAC address only.
 *
 * \param[in] MAC: The MAC Address in 6 byte array format like
 *                uint8_t mac[] = { 0x00, 0x50, 0x43, 0x21, 0x19, 0x6E};
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_sta_mac_addr(uint8_t *mac);

/** Set the Wi-Fi MAC address for the uAP in the Wi-Fi firmware.
 *
 * This function can be used to set the Wi-Fi MAC address for the uAP in the firmware.
 * Should be called after Wi-Fi initialization done. It sets the uAP MAC address only.
 *
 * \param[in] MAC: The MAC Address in 6 bytes array format like
 *                uint8_t mac[] = { 0x00, 0x50, 0x43, 0x21, 0x19, 0x6E};
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_uap_mac_addr(uint8_t *mac);

#if CONFIG_WMM_UAPSD
/** Set the QOS info of the UAPSD (unscheduled automatic power save delivery)
 * in the Wi-Fi firmware.
 *
 * \param[in,out] qos_info: UAPSD (unscheduled automatic power save delivery) QOS info.
 * \param[in] action: Set/get action.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_wmm_uapsd_qosinfo(t_u8 *qos_info, t_u8 action);
/** Enable/Disable the UAPSD in the Wi-Fi firmware.
 *
 * \param[in] uapsd_enable Enable/Disable UAPSD.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_wmm_uapsd(t_u8 uapsd_enable);
/** Set/get UAPSD sleep period in the Wi-Fi firmware.
 *
 * \param[in,out] sleep_period: UAPSD sleep period. Unit is ms.
 * \param[in] action: Set/get action.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_sleep_period(unsigned int *sleep_period, t_u8 action);
/** Check whether UAPSD is enabled or not.
 *
 * \return true if UAPSD is enabled.
 * \return false if UAPSD is disabled.
 */
t_u8 wlan_is_wmm_uapsd_enabled(void);
#endif

#if CONFIG_WIFI_TX_BUFF
/** Reconfigure Wi-Fi TX buffer size in Wi-Fi firmware.
 *
 * This function can be called to reconfigure Wi-Fi TX buffer size in firmware.
 * This should be call before \ref wlan_init() function.
 *
 * \param[in] buf_size: The new buffer size
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 *
 */
void wlan_recfg_tx_buf_size(uint16_t buf_size, mlan_bss_type bss_type);
#endif

#if CONFIG_WIFI_TX_PER_TRACK
/** Set TX per tracking config.
 * This function can be called to set TX per tracking in firmware.
 *
 * \param[in] tx_pert: User configured parameters of TX per tracking
 *            period, ratio and number of TX packets.
 *
 * \param[in] bss_type: BSS type for STA or uAP.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
void wlan_set_tx_pert(struct wlan_tx_pert_info *tx_pert, mlan_bss_type bss_type);
#endif

#if CONFIG_TX_RX_HISTOGRAM
/** Set TX RX histogram config.
 * This function can be called to set TX RX histogram config.
 *
 * \param[in] txrx_histogram: User configured parameters of TX RX histogram.
 *            including enable and action.
 * \param[out] data: TX RX histogram data from FW.
 */
void wlan_set_txrx_histogram(struct wlan_txrx_histogram_info *txrx_histogram, t_u8 *data);
#endif

#if CONFIG_ROAMING
/** Set soft roaming config.
 *
 * This function can be used to enable/disable soft roaming
 * by specifying the RSSI threshold.
 *
 * \note <b>RSSI Threshold setting for soft roaming</b>:
 * The provided RSSI low threshold value is used to subscribe
 * RSSI low event from the firmware. On reception of this event,
 * the background scan is started in the firmware with the same RSSI
 * threshold to find out APs with a better signal strength than
 * the RSSI threshold.
 *
 * If an AP with better signal strength is found, the reassociation is triggered.
 * Otherwise the background scan is started again until the scan count
 * reaches BG_SCAN_LIMIT.
 *
 * If still AP is not found then Wi-Fi connection manager sends
 * \ref WLAN_REASON_BGSCAN_NETWORK_NOT_FOUND event to
 * application. In this case,
 * if application again wants to use soft roaming then it
 * can call this API again or use
 * \ref wlan_set_rssi_low_threshold API to set RSSI low
 * threshold again.
 *
 * \param[in] enable: Enable/Disable roaming.
 * \param[in] rssi_low_threshold: RSSI low threshold value
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_roaming(const int enable, const uint8_t rssi_low_threshold);

/** Get the roaming status.
 *
 * \return 1 if roaming is enabled.
 * \return 0 if roaming is disbled.
 */
int wlan_get_roaming_status(void);
#endif

#if CONFIG_HOST_SLEEP
#ifdef RW610
#if CONFIG_MEF_CFG
/** Wowlan (wake on wireless LAN) configuration.
 * This function may be called to configure host sleep in firmware.
 *
 * \param[in] is_mef: Flag to indicate use MEF (memory efficient filtering) condition or not.
 * \param[in] wake_up_conds: Bit map of default condition.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_wowlan_config(uint8_t is_mef, t_u32 wake_up_conds);
#else
/** Wowlan configuration.
 * This function may be called to configure host sleep in firmware.
 *
 * \param[in] wake_up_conds: Bit map of default condition.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_wowlan_config(t_u32 wake_up_conds);
#endif

/** Host sleep configuration.
 * This function may be called to configure host sleep in firmware.
 *
 * \param[in] is_manual: Flag to indicate host enter low power mode with power manager or by command.
 * \param[in] is_periodic: Flag to indicate host enter low power periodically or once with power manager.
 */
void wlan_config_host_sleep(bool is_manual, t_u8 is_periodic);

/** This function sends host sleep events to mon_thread
 * \param[in] id: Event ID.
 * \param[in] data: Pointer to event msg.
 * \return kStatus_Success if successful else return -WM_FAIL.
 */
status_t wlan_hs_send_event(int id, void *data);
#endif /*RW610*/

/** Cancel host sleep.
 * This function is called to cancel the host sleep in the firmware.
 */
void wlan_cancel_host_sleep(void);

/** Clear host sleep configurations in driver.
 * This function clears all the host sleep related configures in driver.
 */
void wlan_clear_host_sleep_config(void);

/** This function set multicast MEF (memory efficient filtering) entry
 *
 * \param[in] mef_action: To be 0--discard and not wake host, 1--discard and wake host 3--allow and wake host.
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_multicast(t_u8 mef_action);
#endif

/** Set configuration parameters of IEEE power save mode.
 *
 * \param [in] ps_cfg: Power save configuration includes multiple parameters.
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_set_ieeeps_cfg(struct wlan_ieeeps_config *ps_cfg);

/** Configure listening interval of IEEE power save mode.
 *
 * \note <b>Delivery traffic indication message (DTIM)</b>:
 * It is a concept in 802.11
 * It is a time duration after which AP can send out buffered
 * BROADCAST / MULTICAST data and stations connected to the AP
 * should wakeup to take this broadcast / multicast data.

 * \note <b>Traffic Indication Map (TIM)</b>:
 * It is a bitmap which the AP sends with each beacon.
 * The bitmap has one bit each for a station connected to AP.
 *
 * \note Each station is recognized by an association ID (AID).
 * If AP has buffered data for a station, it will set corresponding bit
 * of bitmap in TIM based on AID.
 * Ideally AP does not buffer any unicast data it just sends
 * unicast data to the station on every beacon when station
 * is not sleeping.\n
 * When broadcast data / multicast data is to be send AP sets bit 0
 * of TIM indicating broadcast / multicast.\n
 * The occurrence of DTIM is defined by AP.\n
 * Each beacon has a number indicating period at which DTIM occurs.\n
 * The number is expressed in terms of number of beacons.\n
 * This period is called DTIM Period / DTIM interval.\n
 * For example:\n
 *     If AP has DTIM period = 3 the stations connected to AP
 *     have to wake up (if they are sleeping) to receive
 *     broadcast /multicast data on every third beacon.\n
 * Generic:\n
 *     When DTIM period is X
 *     AP buffers broadcast data / multicast data for X beacons.
 *     Then it transmits the data no matter whether station is awake or not.\n
 * Listen interval:\n
 * This is time interval on station side which indicates when station
 * can be awake to listen i.e. accept data.\n
 * Long listen interval:\n
 * It comes into picture when station sleeps (IEEE PS) and it does
 * not want to wake up on every DTIM
 * So station is not worried about broadcast data/multicast data
 * in this case.\n
 * This should be a design decision what should be chosen
 * Firmware suggests values which are about 3 times DTIM
 * at the max to gain optimal usage and reliability.\n
 * In the IEEE power save mode, the Wi-Fi firmware goes to sleep and
 * periodically wakes up to check if the AP has any pending packets for it. A
 * longer listen interval implies that the Wi-Fi SoC stays in power save for a
 * longer duration at the cost of additional delays while receiving data.
 * Note that choosing incorrect value for listen interval 
 * causes poor response from device during data transfer.
 * Actual listen interval selected by firmware is equal to closest DTIM.\n
 * For example:\n
 *            AP beacon period : 100 ms\n
 *            AP DTIM period : 2\n
 *            Application request value: 500ms\n
 *            Actual listen interval = 400ms (This is the closest DTIM).
 * Actual listen interval set should be a multiple of DTIM closest to but
 * lower than the value provided by the application.\n
 *
 *  \note This API can be called before/after association.
 *  The configured listen interval can be used in subsequent association
 *  attempt.
 *
 *  \param [in]  listen_interval: Listen interval as below\n
 *               0 : Unchanged,\n
 *              -1 : Disable,\n
 *             1-49: Value in beacon intervals,\n
 *            >= 50: Value in TUs\n
 */
void wlan_configure_listen_interval(int listen_interval);

/** Set timeout configuration before Wi-Fi power save mode.
 *
 * \param [in] timeout_ms: timout time, in milliseconds.
 *
 */
void wlan_configure_delay_to_ps(unsigned int timeout_ms);

/** Set timeout value before Wi-Fi enter deep sleep mode.
 *
 * param [in] timeout_ms: timout time, in milliseconds.
 *
 * \note The minimum value of \ref timeout_ms is 100.
 */
void wlan_configure_idle_time(unsigned int timeout_ms);

/** Get timeout value of deep sleep mode, in milliseconds.
 *
 * \return idle time value.
 */
unsigned int wlan_get_idle_time(void);

/** Get listen interval .
 *
 * \return listen interval value.
 *
 */
unsigned short wlan_get_listen_interval(void);

/** Get delay time for Wi-Fi power save mode.
 *
 * \return delay time value.
 *
 */
unsigned int wlan_get_delay_to_ps(void);

/** Check whether Wi-Fi power save is enabled or not.
 *
 * \return TRUE if Wi-Fi power save is enabled, else return FALSE.
 *
 */
bool wlan_is_power_save_enabled(void);

/** Configure NULL packet interval of IEEE power save mode.
 *
 *  \note In IEEE PS (power save), station sends a NULL packet to AP to indicate that
 *  the station is alive and maintain connection with the AP.
 *  If null packet is not sent some APs may disconnect station
 *  which might lead to a loss of connectivity.
 *  The time is specified in seconds.
 *  Default value is 30 seconds.
 *
 *  \note This API should be called before configuring IEEE Power save.
 *
 *  \param [in] time_in_secs: -1 Disables null packet transmission,
 *                             0  Null packet interval is unchanged,
 *                             n  Null packet interval in seconds.
 */
void wlan_configure_null_pkt_interval(int time_in_secs);

#ifndef RW610
/** This API can be used to set the mode of TX/RX antenna.
 * If SAD (software antenna diversity) is enabled, this API can also be used to set SAD antenna
 * evaluate time interval(antenna mode is antenna diversity
 * when set SAD evaluate time interval).
 *
 * \param[in] ant: Antenna valid values are 1, 2 and 0xFFFF
 *                1 : TX/RX antenna 1
 *                2 : TX/RX antenna 2
 *	          0xFFFF: TX/RX antenna diversity
 *                (Refer to hardware schematic)
 * \param[in] evaluate_time:
 *	      SAD evaluate time interval (unit: milliseconds), default value is 6s(0x1770).
 *
 * \return WM_SUCCESS if successful.
 * \return WLAN_ERROR_STATE if unsuccessful.
 *
 */
int wlan_set_antcfg(uint32_t ant, uint16_t evaluate_time);

/** This API can be used to get the mode of TX/RX antenna.
 * If SAD (software antenna diversity) is enabled, this API can also be used to get SAD antenna
 * evaluate time interval(antenna mode is antenna diversity
 * when set SAD evaluate time interval).
 *
 * \param[out] ant: pointer to antenna variable.
 *                  antenna variable:
 *                  1 : TX/RX antenna 1
 *                  2 : TX/RX antenna 2
 *	            0xFFFF: TX/RX antenna diversity
 * \param[out] evaluate_time: pointer to evaluate_time variable for SAD.
 * \param[out] current_antenna: pointer to current antenna.
 *                 evaluate_mode:
 *	           0: PCB Ant  + Ext Ant0
 *                 1: Ext Ant0 + Ext Ant1
 *                 2: PCB Ant  + Ext Ant1
 *              0xFF: Default divisity mode.
 *
 * \return WM_SUCCESS if successful.
 * \return WLAN_ERROR_STATE if unsuccessful.
 */
int wlan_get_antcfg(uint32_t *ant, uint16_t *evaluate_time, uint16_t *current_antenna);
#else
/** This API can be used to set the mode of TX/RX antenna.
 * If SAD (software antenna diversity) is enabled, this API can also be used to set SAD antenna
 * evaluate time interval(antenna mode is antenna diversity
 * when set SAD evaluate time interval).
 *
 * \param[in] ant: Antenna valid values are 1, 2 and 0xFFFF
 *                1 : TX/RX antenna 1
 *                2 : TX/RX antenna 2
 *	          0xFFFF: TX/RX antenna diversity
 * \param[in] evaluate_time:
 *	      SAD evaluate time interval, default value is 6s(0x1770).
 *  \param[in] evaluate_mode:
 *	           0: PCB Ant  + Ext Ant0
 *                 1: Ext Ant0 + Ext Ant1
 *                 2: PCB Ant  + Ext Ant1
 *              0xFF: Default divisity mode.
 *
 * \return WM_SUCCESS if successful.
 * \return WLAN_ERROR_STATE if unsuccessful.
 *
 */
int wlan_set_antcfg(uint32_t ant, uint16_t evaluate_time, uint8_t evaluate_mode);

/** This API can be used to get the mode of TX/RX antenna.
 * If SAD (software antenna diversity) is enabled, this API can also be used to get SAD antenna
 * evaluate time interval(antenna mode is antenna diversity
 * when set SAD evaluate time interval).
 *
 * \param[out] ant: pointer to antenna variable.
 *                 antenna variable:
 *                 1 : TX/RX antenna 1
 *                 2 : TX/RX antenna 2
 *	           0xFFFF: TX/RX antenna diversity
 * \param[out] evaluate_time: pointer to evaluate_time variable for SAD.
 * \param[out] current_mode: pointer to evaluate_mode.
 *                 evaluate_mode:
 *	           0: PCB Ant  + Ext Ant0
 *                 1: Ext Ant0 + Ext Ant1
 *                 2: PCB Ant  + Ext Ant1
 *              0xFF: Default divisity mode.
 * \param[out] current_antenna: pointer to current antenna.
 *
 * \return WM_SUCCESS if successful.
 * \return WLAN_ERROR_STATE if unsuccessful.
 */
int wlan_get_antcfg(uint32_t *ant, uint16_t *evaluate_time, uint8_t *evaluate_mode, uint16_t *current_antenna);
#endif /*RW610*/

/** Get the Wi-Fi firmware version extension string.
 *
 * \note This API does not allocate memory for pointer.
 *       It just returns pointer of WLCMGR internal static
 *       buffer. So no need to free the pointer by caller.
 *
 * \return Wi-Fi firmware version extension string pointer stored in
 *         WLCMGR
 */
char *wlan_get_firmware_version_ext(void);

/** Use this API to print Wi-Fi driver and firmware extended version on console.
 * \note Call this API when SDK_DEBUGCONSOLE not set to DEBUGCONSOLE_DISABLE.
 */
void wlan_version_extended(void);

/**
 * Use this API to get the TSF (timing synchronization function) from Wi-Fi firmware.
 *
 * \param[in] tsf_high: Pointer to store TSF higher 32bits.
 * \param[in] tsf_low: Pointer to store TSF lower 32bits.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 *
 */
int wlan_get_tsf(uint32_t *tsf_high, uint32_t *tsf_low);

/** Enable IEEE power save with host sleep configuration
 *
 * When enabled, Wi-Fi SoC is opportunistically put into IEEE power save mode.
 * Before putting the Wi-Fi SoC in power
 * save this also sets the host sleep configuration on the SoC as
 * specified. This makes the SoC generate a wakeup for the processor if
 * any of the wakeup conditions are met.
 *
 * \param[in] wakeup_conditions: conditions to wake the host. This should
 *            be a logical OR of the conditions in \ref wlan_wakeup_event_t.
 *            Typically devices would want to wake up on
 *            \ref WAKE_ON_ALL_BROADCAST,
 *            \ref WAKE_ON_UNICAST,
 *            \ref WAKE_ON_MAC_EVENT.
 *            \ref WAKE_ON_MULTICAST,
 *            \ref WAKE_ON_ARP_BROADCAST,
 *            \ref WAKE_ON_MGMT_FRAME
 *
 * \note IEEE power save mode applies only when STA has connected to an AP.
 *       It could be enabled/disabled when STA connected or disconnected,
 *       but only take effect when STA has connected to an AP.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_ieeeps_on(unsigned int wakeup_conditions);

/** Turn off IEEE power save mode.
 *
 * \note IEEE power save mode applies only when STA has connected to an AP.
 *       It could be enabled/disabled when STA connected or disconnected,
 *       but only take effect when STA has connected to an AP.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_ieeeps_off(void);

#if !CONFIG_WPA_SUPP
#if (CONFIG_WNM_PS)
/** Enable WNM with host sleep configuration
 *
 * When enabled, it opportunistically puts the Wi-Fi SoC into IEEE PS (power save) mode.
 * Before putting the Wi-Fi SoC in power
 * save this also sets the host sleep configuration on the SoC as
 * specified. This makes the SoC generate a wakeup for the processor if
 * any of the wakeup conditions are met.
 *
 * \param[in] wakeup_conditions: conditions to wake the host. This should
 *            be a logical OR of the conditions in \ref wlan_wakeup_event_t.
 *            Typical devices would want to wake up on
 *            \ref WAKE_ON_ALL_BROADCAST,
 *            \ref WAKE_ON_UNICAST,
 *            \ref WAKE_ON_MAC_EVENT.
 *            \ref WAKE_ON_MULTICAST,
 *            \ref WAKE_ON_ARP_BROADCAST,
 *            \ref WAKE_ON_MGMT_FRAME
 * \param[in] wnm_sleep_time: wnm sleep interval.(number of dtims)
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_wnmps_on(unsigned int wakeup_conditions, t_u16 wnm_sleep_time);

/** Turn off WNM power save mode.
 *
 * \note This call is asynchronous. The system exits the power save mode
 *       only when all requisite conditions are met.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_wnmps_off(void);
#endif
#endif

/** Turn on deep sleep power save mode.
 *
 * \note deep sleep power save mode only applies when STA disconnected.
 *       It could be enabled/disabled when STA connected or disconnected,
 *       but only take effect when STA disconnected.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_deepsleepps_on(void);

/** Turn off deep sleep power save mode.
 *
 * \note deep sleep power save mode only applies when STA disconnected.
 *       It could be enabled/disabled when STA connected or disconnected,
 *       but only take effect when STA disconnected.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL otherwise.
 *
 */
int wlan_deepsleepps_off(void);

/**
 * Use this API to configure the TCP keep alive parameters in Wi-Fi firmware.
 * \ref wlan_tcp_keep_alive_t provides the parameters which are available
 * for configuration.
 *
 * \note To reset current TCP keep alive configuration, just set the reset member
 * of wlan_tcp_keep_alive_t with value 1, all other parameters are ignored in this case.
 *
 * \note This API is called after successful connection
 * and before putting Wi-Fi SoC in IEEE power save mode.
 *
 * \param[in] keep_alive: A pointer to \ref wlan_tcp_keep_alive_t
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_tcp_keep_alive(wlan_tcp_keep_alive_t *keep_alive);


/**
 * Use this API to get the beacon period of associated BSS from the cached state information.
 *
 * \return beacon_period if operation is successful.
 * \return 0 if command fails.
 */
uint16_t wlan_get_beacon_period(void);

/**
 * Use this API to get the dtim period of associated BSS.
 * When this API called, the radio sends a probe request to the AP for this information.
 *
 * \return dtim_period if operation is successful.
 * \return 0 if DTIM IE is not found in AP's Probe response.
 * \note This API should not be called from Wi-Fi event handler
 *        registered by application during \ref wlan_start.
 */
uint8_t wlan_get_dtim_period(void);

/**
 * Use this API to get the current TX and RX rates along with
 * bandwidth and guard interval information if rate is 802.11n.
 *
 * \param[in] ds_rate: A pointer to structure which has
 *            tx, RX rate information along with bandwidth and guard
 *	      interval information.
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 *
 * \note If rate is greater than 11 then it is 802.11n rate and from 12
 *       MCS0 rate starts. The bandwidth mapping is like value 0 is for
 *	 20MHz, 1 is 40MHz, 2 is for 80MHz.
 *	 The guard interval value zero means Long otherwise Short.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_data_rate(wlan_ds_rate *ds_rate, mlan_bss_type bss_type);

/**
 * Use this API to get the management frame protection parameters for sta.
 *
 * \param[out] mfpc: Management frame protection capable (MFPC)
 *                       1: Management frame protection capable
 *                       0: Management frame protection not capable
 * \param[out] mfpr: Management frame protection required (MFPR)
 *                       1: Management frame protection required
 *                       0: Management frame protection optional
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_pmfcfg(uint8_t *mfpc, uint8_t *mfpr);

/**
 * Use this API to get the set management frame protection parameters for uAP.
 *
 * \param[out] mfpc: Management frame protection capable (MFPC)
 *                       1: management frame protection capable.
 *                       0: management frame protection not capable.
 * \param[out] mfpr: Management frame protection required (MFPR)
 *                       1: management frame protection required.
 *                       0: management frame protection optional.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_uap_get_pmfcfg(uint8_t *mfpc, uint8_t *mfpr);


/**
 * Use this API to set packet filters in Wi-Fi firmware.
 *
 * \param[in] flt_cfg: A pointer to structure which holds the
 *	      the packet filters \ref wlan_flt_cfg_t.
 *
 * \note For example:\n
 * MEF Configuration command\n
 * mefcfg={\n
 * Criteria: bit0-broadcast, bit1-unicast, bit3-multicast\n
 * Criteria=2 		Unicast frames are received during host sleep mode\n
 * NumEntries=1		Number of activated MEF entries\n
 * mef_entry_0: example filters to match TCP destination port 80 send by 192.168.0.88 pkt or magic pkt.\n
 * mef_entry_0={\n
 *  mode: bit0--hostsleep mode, bit1--non hostsleep mode\n
 *  mode=1		HostSleep mode\n
 *  action: 0--discard and not wake host, 1--discard and wake host 3--allow and wake host\n
 *  action=3	Allow and Wake host\n
 *  filter_num=3    Number of filter\n
 *   RPN only support "&&" and "||" operators, space cannot be removed between operators.\n
 *   RPN=Filter_0 && Filter_1 || Filter_2\n
 *   Byte comparison filter's type is 0x41, decimal comparison filter's type is 0x42,\n
 *   Bit comparison filter's type is 0x43\n
 *  Filter_0 is decimal comparison filter, it always with type=0x42\n
 *  Decimal filter always has type, pattern, offset, numbyte 4 field\n
 *  Filter_0 matches RX packet with TCP destination port 80\n
 *  Filter_0={\n
 *    type=0x42	      decimal comparison filter\n
 *    pattern=80      80 is the decimal constant to be compared\n
 *    offset=44	      44 is the byte offset of the field in RX pkt to be compare\n
 *    numbyte=2       2 is the number of bytes of the field\n
 *  }\n
 *  Filter_1 is Byte comparison filter, it always with type=0x41\n
 *  Byte filter always has type, byte, repeat, offset 4 filed\n
 *  Filter_1 matches RX packet send by IP address 192.168.0.88\n
 *  Filter_1={\n
 *   type=0x41         Byte comparison filter\n
 *   repeat=1          1 copies of 'c0:a8:00:58'\n
 *   byte=c0:a8:00:58  'c0:a8:00:58' is the byte sequence constant with each byte\n
 *   in hex format, with ':' as delimiter between two byte.\n
 *   offset=34         34 is the byte offset of the equal length field of rx'd pkt.\n
 *  }\n
 *  Filter_2 is Magic packet, it can look for 16 contiguous copies of '00:50:43:20:01:02' from\n
 *  the RX pkt's offset 14\n
 *  Filter_2={\n
 *   type=0x41	       Byte comparison filter\n
 *   repeat=16         16 copies of '00:50:43:20:01:02'\n
 *   byte=00:50:43:20:01:02  # '00:50:43:20:01:02' is the byte sequence constant\n
 *   offset=14	       14 is the byte offset of the equal length field of rx'd pkt.\n
 *  }\n
 * }\n
 * }\n
 * Above filters can be set by filling values in following way in \ref wlan_flt_cfg_t structure.\n
 * wlan_flt_cfg_t flt_cfg;\n
 * uint8_t byte_seq1[] = {0xc0, 0xa8, 0x00, 0x58};\n
 * uint8_t byte_seq2[] = {0x00, 0x50, 0x43, 0x20, 0x01, 0x02};\n
 *\n
 * memset(&flt_cfg, 0, sizeof(wlan_flt_cfg_t));\n
 *\n
 * flt_cfg.criteria = 2;\n
 * flt_cfg.nentries = 1;\n
 *\n
 * flt_cfg.mef_entry.mode = 1;\n
 * flt_cfg.mef_entry.action = 3;\n
 *\n
 * flt_cfg.mef_entry.filter_num = 3;\n
 *\n
 * flt_cfg.mef_entry.filter_item[0].type = TYPE_DNUM_EQ;\n
 * flt_cfg.mef_entry.filter_item[0].pattern = 80;\n
 * flt_cfg.mef_entry.filter_item[0].offset = 44;\n
 * flt_cfg.mef_entry.filter_item[0].num_bytes = 2;\n
 *\n
 * flt_cfg.mef_entry.filter_item[1].type = TYPE_BYTE_EQ;\n
 * flt_cfg.mef_entry.filter_item[1].repeat = 1;\n
 * flt_cfg.mef_entry.filter_item[1].offset = 34;\n
 * flt_cfg.mef_entry.filter_item[1].num_byte_seq = 4;\n
 * memcpy(flt_cfg.mef_entry.filter_item[1].byte_seq, byte_seq1, 4);\n
 * flt_cfg.mef_entry.rpn[1] = RPN_TYPE_AND;\n
 *\n
 * flt_cfg.mef_entry.filter_item[2].type = TYPE_BYTE_EQ;\n
 * flt_cfg.mef_entry.filter_item[2].repeat = 16;\n
 * flt_cfg.mef_entry.filter_item[2].offset = 14;\n
 * flt_cfg.mef_entry.filter_item[2].num_byte_seq = 6;\n
 * memcpy(flt_cfg.mef_entry.filter_item[2].byte_seq, byte_seq2, 6);\n
 * flt_cfg.mef_entry.rpn[2] = RPN_TYPE_OR;\n
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_packet_filters(wlan_flt_cfg_t *flt_cfg);

/**
 * Use this API to enable ARP (address resolution protocol) offload in Wi-Fi firmware
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_auto_arp(void);

#if CONFIG_AUTO_PING
/**
 * Use this API to enable ping offload in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_auto_ping(void);
#endif /*  CONFIG_AUTO_PING */

/**
 * Use this API to enable WOWLAN (wake-on-wireless-LAN) on magic packet RX in Wi-Fi firmware
 *
 * \param[in] ptn_cfg: A pointer to \ref wlan_wowlan_ptn_cfg_t containing wake on Wi-Fi pattern configuration
 *
 *\return WM_SUCCESS if operation is successful.
 *\return -WM_FAIL if command fails
 */
int wlan_wowlan_cfg_ptn_match(wlan_wowlan_ptn_cfg_t *ptn_cfg);
/**
 * Use this API to enable NS offload in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_ipv6_ns_offload(void);

#if CONFIG_HOST_SLEEP

/** Use this API to set configuration before going to host sleep */
void wlan_hs_pre_cfg(void);

/** Use this API to get and print the reason of waking up from host sleep */
void wlan_hs_post_cfg(void);

/**
 * Use this API to configure host sleep parameters in Wi-Fi firmware.
 *
 * \param[in] wakeup_condition: bit 0: WAKE_ON_ALL_BROADCAST
 *                              bit 1: WAKE_ON_UNICAST
 *                              bit 2: WAKE_ON_MAC_EVENT
 *                              bit 3: WAKE_ON_MULTICAST
 *                              bit 4: WAKE_ON_ARP_BROADCAST
 *                              bit 6: WAKE_ON_MGMT_FRAME
 *                              All bit 0 discard and not wakeup host
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_send_host_sleep(uint32_t wakeup_condition);

/**
 * Use this API to get host sleep wakeup reason from Wi-Fi firmware after waking up from host sleep by Wi-Fi.
 *
 * \param[out] hs_wakeup_reason: wakeupReason:
 *                               0: unknown
 *                               1: Broadcast data matched
 *                               2: Multicast data matched
 *                               3: Unicast data matched
 *                               4: Maskable event matched
 *                               5. Non-maskable event matched
 *                               6: Non-maskable condition matched (EAPoL rekey)
 *                               7: Magic pattern matched
 *                               Others: reserved. (set to 0)
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_wakeup_reason(uint16_t *hs_wakeup_reason);
#endif

/**
 * Use this API to get the BSSID of associated BSS when in station mode.
 *
 * \param[out] bssid: A pointer to array(char, length is 6) to store the BSSID.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_current_bssid(uint8_t *bssid);

/**
 * Use this API to get the channel number of associated BSS.
 *
 * \return channel number if operation is successful.
 * \return 0 if command fails.
 */
uint8_t wlan_get_current_channel(void);

#if CONFIG_WIFI_GET_LOG
/**
 * Use this API to get the various statistics of STA from Wi-Fi firmware
 *
 * \param[out] stats: A pointer to structure where stats collected from Wi-Fi firmware
 *	      can be copied.\n
 *            Explore the elements of the \ref wlan_pkt_stats_t strucutre for
 * 	      more information on stats.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_log(wlan_pkt_stats_t *stats);

/**
 * Use this API to get the various statistics of the uAP from Wi-Fi firmware.
 *
 * \param[out] stats: A pointer to structure where stats collected from Wi-Fi firmware
 *	      can be copied.\n
 *            Explore the elements of the \ref wlan_pkt_stats_t strucutre for
 * 	      more information on stats.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_uap_get_log(wlan_pkt_stats_t *stats);
#endif

/** Get station interface power save mode.
 *
 * \param[out] ps_mode: A pointer to \ref wlan_ps_mode where station interface
 * 	      power save mode should be stored.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_E_INVAL if \a ps_mode was NULL.
 */
int wlan_get_ps_mode(enum wlan_ps_mode *ps_mode);

/** Send message to Wi-Fi connection manager thread.
 *
 * \param[in] event: An event from \ref wifi_event.
 * \param[in] reason: A reason code.
 * \param[in] data: A pointer to data buffer associated with event.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if failed.
 */
int wlan_wlcmgr_send_msg(enum wifi_event event, enum wifi_event_reason reason, void *data);

/** Register WFA basic Wi-Fi CLI (command line input) commands
 *
 * This function registers basic Wi-Fi CLI commands like showing
 * version information, MAC address.
 *
 * \note This function can only be called by the application after
 * \ref wlan_init() called.
 *
 * \return WLAN_ERROR_NONE if the CLI commands were registered or
 * \return WLAN_ERROR_ACTION if they were not registered (for example
 *   if this function was called while the CLI commands were already
 *   registered).
 */
int wlan_wfa_basic_cli_init(void);

/** Unregister WFA basic Wi-Fi CLI (command line input) commands
 *
 * This function unregisters basic Wi-Fi CLI commands like showing
 * version information, MAC address.
 *
 * \note This function can only be called by the application after
 * \ref wlan_init() called.
 *
 * \return WLAN_ERROR_NONE if the CLI commands were unregistered or
 * \return WLAN_ERROR_ACTION if they were not unregistered
 */
int wlan_wfa_basic_cli_deinit(void);

/** Register basic Wi-Fi CLI (command line input) commands
 *
 * This function registers basic Wi-Fi CLI commands like showing
 * version information, MAC address.
 *
 * \note This function can only be called by the application after
 * \ref wlan_init() called.
 *
 * \note This function gets called by \ref wlan_cli_init(), hence
 * only one function out of these two functions should be called in
 * the application.
 *
 * \return WLAN_ERROR_NONE if the CLI commands were registered
 * \return WLAN_ERROR_ACTION if they were not registered (for example
 * if this function was called while the CLI commands were already
 * registered).
 */
int wlan_basic_cli_init(void);

/** Unregister basic Wi-Fi CLI commands
 *
 * This function unregisters basic Wi-Fi CLI commands like showing
 * version information, MAC address.
 *
 * \note This function gets called by \ref wlan_cli_deinit(), hence
 * only one function out of these two functions should be called in
 * the application.
 *
 * \return WLAN_ERROR_NONE if the CLI commands were unregistered
 * \return WLAN_ERROR_ACTION if they were not unregistered (for example
 * if this function was called while the CLI commands were not registered or
 * were already unregistered).
 */
int wlan_basic_cli_deinit(void);

/** Register Wi-Fi CLI (command line input) commands.
 *
 *  Try to register the Wi-Fi CLI commands with the CLI subsystem. This
 *  function is available for the application for use.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \note This function internally calls \ref wlan_basic_cli_init(), hence
 *  only one function out of these two functions should be called in
 *  the application.
 *
 *  \return WM_SUCCESS if the CLI commands were registered or
 *  \return -WM_FAIL if they were not (for example if this function
 *          was called while the CLI commands were already registered).
 */
int wlan_cli_init(void);

/** Unregister Wi-Fi CLI commands.
 *
 *  Try to unregister the Wi-Fi CLI commands with the CLI subsystem. This
 *  function is available for the application for use.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \note This function internally calls \ref wlan_basic_cli_deinit(), hence
 *  only one function out of these two functions should be called in
 *  the application.
 *
 *  \return WM_SUCCESS if the CLI commands were unregistered or
 *  \return -WM_FAIL if they were not (for example if this function
 *          was called while the CLI commands were already unregistered).
 */
int wlan_cli_deinit(void);

/** Register Wi-Fi enhanced CLI commands.
 *
 *  Register the Wi-Fi enhanced CLI commands like set or get tx-power,
 *  tx-datarate, tx-modulation etc. with the CLI subsystem.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \return WM_SUCCESS if the CLI commands were registered or
 *  \return -WM_FAIL if they were not (for example if this function
 *           was called while the CLI commands were already registered).
 */
int wlan_enhanced_cli_init(void);

/** Unregister Wi-Fi enhanced CLI commands.
 *
 *  Unregister the Wi-Fi enhanced CLI commands like set or get tx-power,
 *  tx-datarate, tx-modulation etc. with the CLI subsystem.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \return WM_SUCCESS if the CLI commands were unregistered or
 *  \return -WM_FAIL if they were not unregistered.
 */

int wlan_enhanced_cli_deinit(void);

#if CONFIG_RF_TEST_MODE
/** Register Wi-Fi test mode CLI commands.
 *
 *  Register the Wi-Fi test mode CLI commands like set or get channel,
 *  band, bandwidth, per and more with the CLI subsystem.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \return WM_SUCCESS if the CLI commands were registered or
 *  \return -WM_FAIL if they were not (for example if this function
 *           was called while the CLI commands were already registered).
 */
int wlan_test_mode_cli_init(void);

/** Unregister Wi-Fi test mode CLI commands.
 *
 *  Unregister the Wi-Fi test mode CLI commands like set or get channel,
 *  band, bandwidth, PER and more with the CLI subsystem.
 *
 *  \note This function can only be called by the application after \ref wlan_init()
 *  called.
 *
 *  \return WM_SUCCESS if the CLI commands were unregistered or
 *  \return -WM_FAIL if they were not unregistered
 */
int wlan_test_mode_cli_deinit(void);
#endif

/**
 * Get maximum number of the stations Wi-Fi firmware supported that can be
 * allowed to connect to the uAP.
 *
 * \return Maximum number of the stations Wi-Fi firmware supported that can be
 * allowed to connect to the uAP.
 *
 * \note Get operation is allowed in any uAP state.
 */
unsigned int wlan_get_uap_supported_max_clients(void);

/**
 * Get current maximum number of the stations that
 * can be allowed to connect to the uAP.
 *
 * \param[out] max_sta_num: A pointer to variable where current maximum
 *             number of the stations of the uAP interface can be stored.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 * \note Get operation is allowed in any uAP state.
 */
int wlan_get_uap_max_clients(unsigned int *max_sta_num);

/**
 * Set maximum number of the stations that can be allowed to connect to the uAP.
 *
 * \param[in] max_sta_num: Number of maximum stations for uAP.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 * \note Set operation in not allowed in \ref WLAN_UAP_STARTED state.
 */
int wlan_set_uap_max_clients(unsigned int max_sta_num);

/**
 * Use this API to configure some of parameters in HT capability information IE
 *       (such as short GI, channel bandwidth, and green field support)
 *
 * \param[in] htcapinfo: This is a bitmap and should be used as following\n
 *               Bit 29: Green field Enable/Disable\n
 *               Bit 26: RX STBC Support Enable/Disable. (As we support\n
 *                       single spatial stream only 1 bit is used for RX STBC)\n
 *               Bit 25: TX STBC support Enable/Disable.\n
 *               Bit 24: Short GI in 40 Mhz Enable/Disable\n
 *               Bit 23: Short GI in 20 Mhz Enable/Disable\n
 *               Bit 22: RX LDPC Enable/Disable\n
 *               Bit 17: 20/40 Mhz enable disable.\n
 *               Bit  8: Enable/Disable 40Mhz intolerant bit in HT capinfo.\n
 *                       0 can reset this bit and 1 can set this bit in\n
 *                       htcapinfo attached in association request.\n
 *               All others are reserved and should be set to 0.\n
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_htcapinfo(unsigned int htcapinfo);

/**
 * Use this API to configure various 802.11n specific configuration
 *       for transmit (such as short GI, channel bandwidth and green field support)
 *
 * \param[in] httxcfg: This is a bitmap and should be used as following\n
 *               Bit 15-10: Reserved set to 0\n
 *               Bit 9-8: RX STBC set to 0x01\n
 *               BIT9 BIT8  Description\n
 *               0    0     No spatial streams\n
 *               0    1     One spatial stream supported\n
 *               1    0     Reserved\n
 *               1    1     Reserved\n
 *               Bit 7: STBC Enable/Disable\n
 *               Bit 6: Short GI in 40 Mhz Enable/Disable\n
 *               Bit 5: Short GI in 20 Mhz Enable/Disable\n
 *               Bit 4: Green field Enable/Disable\n
 *               Bit 3-2: Reserved set to 1\n
 *               Bit 1: 20/40 Mhz enable disable.\n
 *               Bit 0: LDPC Enable/Disable\n
 *
 *       When Bit 1 is set then firmware could transmit in 20Mhz or 40Mhz based\n
 *       on rate adaptation. When this bit is reset then firmware can only\n
 *       transmit in 20Mhz.\n
 *
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_httxcfg(unsigned short httxcfg);

/**
 * Use this API to set the transmit data rate.
 *
 * \note The data rate can be set only after association.
 *
 * \param[in] ds_rate: struct contains following fields
 *             sub_command It should be WIFI_DS_RATE_CFG
 *             and rate_cfg should have following parameters.\n
 *              rate_format - This parameter specifies
 *                              the data rate format used
 *                              in this command\n
 *               0:    LG\n
 *               1:    HT\n
 *               2:    VHT\n
 *               0xff: Auto\n
 *
 *              index - This parameter specifies the rate or MCS index\n
 *              If  rate_format is 0 (LG),\n
 *               0       1 Mbps\n
 *               1       2 Mbps\n
 *               2       5.5 Mbps\n
 *               3       11 Mbps\n
 *               4       6 Mbps\n
 *               5       9 Mbps\n
 *               6       12 Mbps\n
 *               7       18 Mbps\n
 *               8       24 Mbps\n
 *               9       36 Mbps\n
 *               10      48 Mbps\n
 *               11      54 Mbps\n
 *              If  rate_format is 1 (HT),\n
 *               0       MCS0\n
 *               1       MCS1\n
 *               2       MCS2\n
 *               3       MCS3\n
 *               4       MCS4\n
 *               5       MCS5\n
 *               6       MCS6\n
 *               7       MCS7\n
 *	        If STREAM_2X2\n
 *               8       MCS8\n
 *               9       MCS9\n
 *               10      MCS10\n
 *               11      MCS11\n
 *               12      MCS12\n
 *               13      MCS13\n
 *               14      MCS14\n
 *               15      MCS15\n
 *              If  rate_format is 2 (VHT),\n
 *               0       MCS0\n
 *               1       MCS1\n
 *               2       MCS2\n
 *               3       MCS3\n
 *               4       MCS4\n
 *               5       MCS5\n
 *               6       MCS6\n
 *               7       MCS7\n
 *               8       MCS8\n
 *               9       MCS9\n
 *              nss - This parameter specifies the NSS.\n
 *			It is valid only for VHT\n
 *              If  rate_format is 2 (VHT),\n
 *               1       NSS1\n
 *               2       NSS2\n
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_txratecfg(wlan_ds_rate ds_rate, mlan_bss_type bss_type);

/**
 * Use this API to get the transmit data rate.
 *
 * \param[in] ds_rate: A pointer to \ref wlan_ds_rate where TX Rate
 * 		configuration can be stored.
 * \param[in] bss_type: 0: STA, 1: uAP
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_get_txratecfg(wlan_ds_rate *ds_rate, mlan_bss_type bss_type);

/**
 * Get station transmit power
 *
 * \param[out] power_level: Transmit power level (unit: dBm).
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_get_sta_tx_power(t_u32 *power_level);

/**
 * Set station transmit power
 *
 * \param[in] power_level: Transmit power level (unit: dBm).
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_sta_tx_power(t_u32 power_level);

/**
 * Set worldwide safe mode TX power limits.
 * Set TX power limit and ru TX power limit according to the region code.
 * TX power limit: \ref rg_power_cfg_rw610
 * ru TX power limit: \ref ru_power_cfg_rw610
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_wwsm_txpwrlimit(void);

#ifndef RW610
/**
 * Get Wi-Fi region code from TX power config
 *
 * \return Wi-Fi region code in string format.
 *
 */
const char *wlan_get_wlan_region_code(void);
#endif

/**
 * Get Management IE for given BSS type (interface) and index.
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 * \param[in] index: IE index.
 *
 * \param[out] buf: Buffer to store requested IE data.
 * \param[out] buf_len: Length of IE data.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_get_mgmt_ie(enum wlan_bss_type bss_type, IEEEtypes_ElementId_t index, void *buf, unsigned int *buf_len);

/**
 * Set management IE for given BSS type (interface) and index.
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 * \param[in] id: Type/ID of Management IE.
 * \param[in] buf: Buffer containing IE data.
 * \param[in] buf_len: Length of IE data.
 *
 * \return Management IE index if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_mgmt_ie(enum wlan_bss_type bss_type, IEEEtypes_ElementId_t id, void *buf, unsigned int buf_len);

#ifdef SD8801
/**
 * Get external radio coex statistics.
 *
 * \param[out] ext_coex_stats: A pointer to structure to get coex statistics.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_get_ext_coex_stats(wlan_ext_coex_stats_t *ext_coex_stats);

/**
 * Set external radio coex configuration.
 *
 * \param[in] ext_coex_config: to apply coex configuration.
 *
 * \return IE index if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_set_ext_coex_config(const wlan_ext_coex_config_t ext_coex_config);
#endif

/**
 * Clear management IE for given BSS type (interface) and index.
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 * \param[in] index: IE index.
 * \param[in] mgmt_bitmap_index: management bitmap index.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_FAIL if unsuccessful.
 *
 */
int wlan_clear_mgmt_ie(enum wlan_bss_type bss_type, IEEEtypes_ElementId_t index, int mgmt_bitmap_index);

/**
 * Get current status of 802.11d support.
 *
 * \return true if 802.11d support is enabled by application.
 * \return false if not enabled.
 *
 */
bool wlan_get_11d_enable_status(void);

/**
 * Get current RSSI and signal to noise ratio from Wi-Fi firmware.
 *
 * \param[out] RSSI: A pointer to variable to store current RSSI
 * \param[out] snr: A pointer to variable to store current SNR.
 *
 * \return WM_SUCCESS if successful.
 */
int wlan_get_current_signal_strength(short *rssi, int *snr);

/**
 * Get average RSSI and signal to noise ratio (average value of the former 8 packets) from Wi-Fi firmware.
 *
 * \param[out] RSSI: A pointer to variable to store current RSSI
 * \param[out] snr: A pointer to variable to store current SNR.
 *
 * \return WM_SUCCESS if successful.
 */
int wlan_get_average_signal_strength(short *rssi, int *snr);

/**
 * This API is used to set/cancel the remain on channel configuration.
 *
 * \note When status is false, channel and duration parameters are
 * ignored.
 *
 * \param[in] bss_type: The interface to set channel  bss_type 0: STA, 1: uAP
 * \param[in] status: false : Cancel the remain on channel configuration
 *                    true : Set the remain on channel configuration
 * \param[in] channel: The channel to configure
 * \param[in] duration: The duration for which to
 *	      remain on channel in milliseconds.
 *
 * \return WM_SUCCESS on success or error code.
 *
 */
int wlan_remain_on_channel(const enum wlan_bss_type bss_type,
                           const bool status,
                           const uint8_t channel,
                           const uint32_t duration);

/**
 * Get user data from OTP (one-time pramming) memory
 *
 * \param[out] buf: Pointer to buffer where data should be stored
 * \param[out] len: Number of bytes to read
 *
 * \return WM_SUCCESS if user data read operation is successful.
 * \return -WM_E_INVAL if buf is not valid or of insufficient size.
 * \return -WM_FAIL if user data field is not present or command fails.
 */
int wlan_get_otp_user_data(uint8_t *buf, uint16_t len);

/**
 * Get calibration data from Wi-Fi firmware.
 *
 * \param[out] cal_data: Pointer to calibration data structure where
 *	      calibration data and it's length should be stored.
 *
 * \return WM_SUCCESS if calibration data read operation is successful.
 * \return -WM_E_INVAL if cal_data is not valid.
 * \return -WM_FAIL if command fails.
 *
 * \note The user of this API should free the allocated buffer for
 *	 calibration data.
 */
int wlan_get_cal_data(wlan_cal_data_t *cal_data);

#if CONFIG_COMPRESS_TX_PWTBL
/**
 * Set the compressed (use LZW algorithm) TX power limit configuration.
 *
 * \param[in] data: A pointer to TX power limit configuration.
 * \param[in] len: Length of TX power limit configuration.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 */
int wlan_set_region_power_cfg(const t_u8 *data, t_u16 len);
#endif

/**
 * Set the TRPC (transient receptor potential canonical) channel list and TX power limit configuration.
 *
 * \param[in] chanlist: A poiner to \ref wlan_chanlist_t channel List configuration.
 * \param[in] txpwrlimit: A pointer to \ref wlan_txpwrlimit_t TX power limit configuration.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 */
int wlan_set_chanlist_and_txpwrlimit(wlan_chanlist_t *chanlist, wlan_txpwrlimit_t *txpwrlimit);

/**
 * Set the channel list configuration \ref wlan_chanlist_t.
 *
 * \param[in] chanlist: A pointer to \ref wlan_chanlist_t channel list configuration.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 * \note If region enforcement flag is enabled in the OTP then this API should
 * not take effect.
 */
int wlan_set_chanlist(wlan_chanlist_t *chanlist);

/**
 * Get the channel list configuration.
 *
 * \param[out] chanlist: A pointer to wlan_chanlist_t channel list configuration.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 * \note The \ref wlan_chanlist_t struct allocates memory for a maximum of 54.
 * channels.
 *
 */
int wlan_get_chanlist(wlan_chanlist_t *chanlist);

/**
 * Set the TRPC (transient receptor potential canonical) channel configuration.
 *
 * \param[in] txpwrlimit: A pointer to \ref wlan_txpwrlimit_t TX power limit configuration.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 */
int wlan_set_txpwrlimit(wlan_txpwrlimit_t *txpwrlimit);

/**
 * Get the TRPC (transient receptor potential canonical) channel configuration.
 *
 * \param[in] subband:  Where subband is:\n
 *              0x00 2G subband  (2.4G: channel 1-14)\n
 *              0x10 5G subband0 (5G: channel 36,40,44,48,\n
 *                                            52,56,60,64)\n
 *              0x11 5G subband1 (5G: channel 100,104,108,112,\n
 *                                            116,120,124,128,\n
 *                                            132,136,140,144)\n
 *              0x12 5G subband2 (5G: channel 149,153,157,161,165,172)\n
 *              0x13 5G subband3 (5G: channel 183,184,185,187,188,\n
 *                                            189, 192,196;\n
 *                                5G: channel 7,8,11,12,16,34)\n
 *
 * \param[out] txpwrlimit: A pointer to \ref wlan_txpwrlimit_t TX power
 * 		Limit configuration structure where Wi-Fi firmware
 * 		configuration can get copied.
 *
 * \return WM_SUCCESS on success, error otherwise.
 *
 * \note application can use print_txpwrlimit API to print the
 *	 content of the txpwrlimit structure.
 */
int wlan_get_txpwrlimit(wifi_SubBand_t subband, wifi_txpwrlimit_t *txpwrlimit);

#if CONFIG_AUTO_RECONNECT
/**
 * Enable auto reconnect feature in Wi-Fi firmware.
 *
 * \param[in] auto_reconnect_config: auto reconnect configuration
 *	      structure holding following parameters:
 *	      1. reconnect counter(0x1-0xff) - The number of times the Wi-Fi
 *		 firmware retries connection attempt with AP.
 *				The value 0xff means retry forever.
 *				(default 0xff).
 *	      2. reconnect interval(0x0-0xff) - Time gap in seconds between
 *				each connection attempt (default 10).
 *	      3. flags - Bit 0:
 *			 Set to 1: Firmware should report link-loss to host
 *				if AP rejects authentication/association
 *				while reconnecting.
 *			 Set to 0: Default behavior: Firmware does not report
 *				link-loss to host on AP rejection and
 *				continues internally.
 *			 Bit 1-15: Reserved.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 *
 */
int wlan_auto_reconnect_enable(wlan_auto_reconnect_config_t auto_reconnect_config);

/**
 * Disable auto reconnect feature in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 *
 */
int wlan_auto_reconnect_disable(void);

/**
 * Get auto reconnect configuration from Wi-Fi firmware.
 *
 * \param[out] auto_reconnect_config: auto reconnect configuration
 *	       structure where response from Wi-Fi firmware
 *	       gets stored.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_E_INVAL if auto_reconnect_config is not valid.
 * \return -WM_FAIL if command fails.
 *
 */
int wlan_get_auto_reconnect_config(wlan_auto_reconnect_config_t *auto_reconnect_config);
#endif
/**
 * Set reassociation control in Wi-Fi connection manager.
 * When reassociation control enabled, Wi-Fi connection manager attempts reconnection
 * with the network for \ref WLAN_RECONNECT_LIMIT times before giving up.

 * \note Reassociation is enabled by default in the Wi-Fi connection manager.
 *
 * \param[in] reassoc_control: Reassociation enable/disable
 *
 */
void wlan_set_reassoc_control(bool reassoc_control);

/** API to set the beacon period of the uAP
 *
 *\param[in] beacon_period: Beacon period in TU (1 TU = 1024 microseconds)
 *
 *\note  Call this API before calling uAP start API.
 *
 */
void wlan_uap_set_beacon_period(const uint16_t beacon_period);

/** API to set the bandwidth of the uAP
 *
 *\param[in] bandwidth: Wi-Fi AP bandwidth\n
    1: 20 MHz 2: 40 MHz 3: 80 MHz
 *
 *\return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *\return -WM_FAIL if command fails.
 *
 *\note Not applicable to 20MHZ only chip sets (Redfinch, SD8801)
 *\note  Call this API before calling uAP start API.
 *\note Default bandwidth setting is 40 MHz.
 *
 */
int wlan_uap_set_bandwidth(const uint8_t bandwidth);

/** API to get the bandwidth of the uAP
 *
 *\param[out] bandwidth: Wi-Fi AP bandwidth
    1: 20 MHz 2: 40 MHz 3: 80 MHz
 *
 *\return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *\return -WM_FAIL if command fails.
 *
 *\note  Call this API before calling uAP start API.
 *
 */
int wlan_uap_get_bandwidth(uint8_t *bandwidth);

/** API to control SSID broadcast capability of the uAP
 *
 * This API enables/disables the SSID broadcast feature
 * (also known as the hidden SSID feature). When broadcast SSID
 * is enabled, the AP responds to probe requests from client stations
 * that contain null SSID. When broadcast SSID is disabled, the AP
 * does not respond to probe requests that contain null SSID and
 * generates beacons that contain null SSID.
 *
 *\param[in] hidden_ssid: Hidden SSID control
 *           hidden_ssid=0: broadcast SSID in beacons.
 *           hidden_ssid=1: send empty SSID (length=0) in beacon.
 *           hidden_ssid=2: clear SSID (ACSII 0), but keep the original length
 *
 *\return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *\return -WM_FAIL if command fails.
 *
 *\note  Call this API before calling uAP start API.
 *
 */
int wlan_uap_set_hidden_ssid(const t_u8 hidden_ssid);

/** API to control the deauthentication during uAP channel switch.
 *
 *\param[in] enable: 0 -- Wi-Fi firmware can use default behavior, send deauth
 *                        packet when uAP move to another channel.
 *                   1 -- Wi-Fi firmware cannot send deauth packet
 *                        when uAP move to another channel.
 *
 *\note  Call this API before calling uAP start API.
 *
 */
void wlan_uap_ctrl_deauth(const bool enable);

/** API to enable channel switch announcement functionality on uAP.
 *
 *\note  Call this API before calling uAP start API. Also
 *	note that 802.11n should be enabled on uAP. The channel switch announcement IE
 *	is transmitted in 7 beacons before the channel switch, during a station
 *	connection attempt on a different channel with Ex-AP.
 *
 */
void wlan_uap_set_ecsa(void);

/** API to set the HT capability information of the uAP.
 *
 *\param[in] ht_cap_info: - This is a bitmap and should be used as following\n
 *             Bit 15: L Sig TxOP protection - reserved, set to 0 \n
 *             Bit 14: 40 MHz intolerant - reserved, set to 0 \n
 *             Bit 13: PSMP - reserved, set to 0 \n
 *             Bit 12: DSSS Cck40MHz mode\n
 *             Bit 11: Maximal A-MSDU size - reserved, set to 0 \n
 *             Bit 10: Delayed BA - reserved, set to 0 \n
 *             Bits 9:8: RX STBC - reserved, set to 0 \n
 *             Bit 7: TX STBC - reserved, set to 0 \n
 *             Bit 6: Short GI 40 MHz\n
 *             Bit 5: Short GI 20 MHz\n
 *             Bit 4: GF preamble\n
 *             Bits 3:2: MIMO power save - reserved, set to 0 \n
 *             Bit 1: SuppChanWidth - set to 0 for 2.4 GHz band \n
 *             Bit 0: LDPC coding - reserved, set to 0 \n
 *
 *\note  Call this API before calling uAP start API.
 *
 */
void wlan_uap_set_htcapinfo(const uint16_t ht_cap_info);

/**
 * This API can be used to configure various 802.11n specific configuration
 *       for transmit (such as short GI, channel bandwidth and green field support)
 *       for uAP interface.
 *
 * \param[in] httxcfg: This is a bitmap and should be used as following\n
 *               Bit 15-8: Reserved set to 0\n
 *               Bit 7: STBC Enable/Disable\n
 *               Bit 6: Short GI in 40 Mhz Enable/Disable\n
 *               Bit 5: Short GI in 20 Mhz Enable/Disable\n
 *               Bit 4: Green field Enable/Disable\n
 *               Bit 3-2: Reserved set to 1\n
 *               Bit 1: 20/40 Mhz enable disable.\n
 *               Bit 0: LDPC Enable/Disable\n
 *
 *       When Bit 1 is set then firmware could transmit in 20Mhz or 40Mhz based\n
 *       on rate adaptation. When this bit is reset then firmware can only\n
 *       transmit in 20Mhz.\n
 *
 *\note  Call this API before calling uAP start API.
 *
 */
void wlan_uap_set_httxcfg(unsigned short httxcfg);

/**
 * This API can be used to enable AMPDU support
 * when station is a transmitter.
 *
 * \note By default the station AMPDU TX support is enabled if
 * configuration option CONFIG_STA_AMPDU_TX is defined 1.
 */
void wlan_sta_ampdu_tx_enable(void);

/**
 * This API can be used to disable AMPDU support
 * when station is a transmitter.
 *
 *\note By default the station AMPDU TX support is enabled if
 * configuration option CONFIG_STA_AMPDU_TX is defined 1.
 *
 */
void wlan_sta_ampdu_tx_disable(void);

/**
 * This API can be used to enable AMPDU support
 * when station is a receiver.
 *
 *\note By default the station AMPDU RX support is enabled if
 * configuration option CONFIG_STA_AMPDU_RX is defined 1.
 *
 */
void wlan_sta_ampdu_rx_enable(void);

/**
 * This API can be used to disable AMPDU support
 * when station is a receiver.
 *
 *\note By default the station AMPDU RX support is enabled if
 * configuration option CONFIG_STA_AMPDU_RX is defined 1.
 *
 */
void wlan_sta_ampdu_rx_disable(void);

/**
 * This API can be used to enable AMPDU support
 * when uAP is a transmitter.
 *
 * \note By default the uAP AMPDU TX support is enabled if
 * configuration option CONFIG_UAP_AMPDU_TX is defined 1.
 *
 */
void wlan_uap_ampdu_tx_enable(void);

/**
 * This API can be used to disable AMPDU support
 * when uAP is a transmitter.
 *
 * \note By default the uAP AMPDU TX support is enabled if
 * configuration option CONFIG_UAP_AMPDU_TX is defined 1.
 *
 */
void wlan_uap_ampdu_tx_disable(void);

/**
 * This API can be used to enable AMPDU support
 * when uAP is a receiver.
 *
 * \note By default the uAP AMPDU TX support is enabled if
 * configuration option CONFIG_UAP_AMPDU_RX is defined 1.
 *
 */
void wlan_uap_ampdu_rx_enable(void);

/**
 * This API can be used to disable AMPDU support
 * when uAP is a receiver.
 *
 * \note By default the uAP AMPDU TX support is enabled if
 * configuration option CONFIG_UAP_AMPDU_RX is defined 1.
 *
 */
void wlan_uap_ampdu_rx_disable(void);


/**
 * Set number of channels and channel number used during automatic
 * channel selection of the uAP.
 *
 *\param[in] scan_chan_list: A structure holding the number of channels and
 *	     channel numbers.
 *
 *\note  Call this API before uAP start API in order to set the user
 *      defined channels, otherwise it can have no effect. There is no need
 *      to call this API every time before uAP start, if once set same channel
 *      configuration can get used in all upcoming uAP start call. If user
 *      wish to change the channels at run time then it make sense to call
 *      this API before every uAP start API.
 */
void wlan_uap_set_scan_chan_list(wifi_scan_chan_list_t scan_chan_list);

#if CONFIG_WPA2_ENTP

/**
 * Use this API if application want to allow station
 * connection to WPA2 Enterprise ap profiles only.
 *
 * If called, then in scan result only the WPA2 Enterprise AP
 * can be listed and only station network profile with WPA2
 * Enterprise security can be allowed to add to network profile
 * list.
 */
void wlan_enable_wpa2_enterprise_ap_only(void);
#endif

#if CONFIG_WIFI_RTS_THRESHOLD
/**
 * Set the RTS(Request to Send) threshold of STA in Wi-Fi firmware.
 *
 * \param[in] rts: the value of rts threshold configuration.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_rts(int rts);

/**
 * Set the RTS(Request to Send) threshold of the uAP in Wi-Fi firmware.
 *
 * \param[in] rts: the value of rts threshold configuration.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_uap_rts(int rts);
#endif

#if CONFIG_WIFI_FRAG_THRESHOLD
/**
 * Set the fragment threshold of STA in Wi-Fi firmware.
 * If the size of packet exceeds the fragment threshold, the packet is divided into fragments.
 * For example, if the fragment threshold is set to 300, a ping packet of size 1300 is divided
 * into 5 fragments.
 *
 * \param[in] frag: The value of fragment threshold configuration.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_frag(int frag);

/**
 * Set the fragment threshold of the uAP in Wi-Fi firmware.
 * If the size of packet exceeds the fragment threshold, the packet is divided into fragments.
 * For example, if the fragment threshold is set to 300, a ping packet of size 1300 is divided
 * into 5 fragments.
 *
 * \param[in] frag: the value of fragment threshold configuration.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_uap_frag(int frag);
#endif


#if CONFIG_UAP_STA_MAC_ADDR_FILTER
/**
 * Set the STA MAC filter in Wi-Fi firmware.
 * Apply for uAP mode only. When STA MAC filter enabled,
 * wlan firmware blocks all the packets from station with MAC address in black list
 * and not blocks packets from station with MAC address in white list.
 *
 * \param[in] filter_mode: Channel filter mode (disable/white/black list)
 * \param[in] mac_count: The count of MAC list
 * \param[in] mac_addr: The pointer to MAC address list
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_sta_mac_filter(int filter_mode, int mac_count, unsigned char *mac_addr);
#endif

static inline void print_mac(const char *mac)
{
    (void)PRINTF("%02X:%02X:%02X:%02X:%02X:%02X ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

#if CONFIG_RF_TEST_MODE

/**
 * Set the RF test mode in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_rf_test_mode(void);

/**
 * Unset the RF test mode in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful.
 */
int wlan_unset_rf_test_mode(void);

/**
 * Set the RF channel in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] channel: The channel number to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_channel(const uint8_t channel);

/**
 * Set the RF radio mode in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] mode: The radio mode number to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_radio_mode(const uint8_t mode);

/**
 * Get the RF channel from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] channel: A pointer to a variable where channel number to get.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_channel(uint8_t *channel);

/**
 * Get the RF radio mode from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] mode: A pointer to a variable where radio mode number to get.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_radio_mode(uint8_t *mode);

/**
 * Set the RF band in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] band: The bandwidth to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_band(const uint8_t band);

/**
 * Get the RF band from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] band: A Pointer to a variable where RF band is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_band(uint8_t *band);

/**
 * Set the RF bandwidth in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] bandwidth: The bandwidth to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_bandwidth(const uint8_t bandwidth);

/**
 * Get the RF bandwidth from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] bandwidth: A Pointer to a variable where bandwidth to get.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_bandwidth(uint8_t *bandwidth);

/**
 * Get the RF RX total packet and multicast/broadcast packet count.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] rx_tot_pkt_count: A Pointer to a variable where RX total packet count to get.
 * \param[out] rx_mcast_bcast_count: A Pointer to a variable where RX total multicast/broadcast packet count to get.
 * \param[out] rx_pkt_fcs_error: A Pointer to a variable where RX total packet count with FCS (frame check sequence) error to get.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_per(uint32_t *rx_tot_pkt_count, uint32_t *rx_mcast_bcast_count, uint32_t *rx_pkt_fcs_error);

/**
 * Set the RF TX continuous mode in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] enable_tx: Enable TX.
 * \param[in] cw_mode: Set CW (continuous wave) mode.
 * \param[in] payload_pattern: Set payload pattern.
 * \param[in] cs_mode: Set CS mode.
 * \param[in] act_sub_ch: Active subchannel.
 * \param[in] tx_rate: Set TX rate.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL..
 *
 */
int wlan_set_rf_tx_cont_mode(const uint32_t enable_tx,
                             const uint32_t cw_mode,
                             const uint32_t payload_pattern,
                             const uint32_t cs_mode,
                             const uint32_t act_sub_ch,
                             const uint32_t tx_rate);

/**
 * Set the RF HE TB TX in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] enable: Enable/Disable trigger response mode
 * \param[in] qnum: AXQ to be used for the trigger response frame
 * \param[in] aid: AID of the peer to which response is to be generated
 * \param[in] axq_mu_timer: MU timer for the AXQ on which response is sent
 * \param[in] tx_power: TxPwr to be configured for the response
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_cfg_rf_he_tb_tx(uint16_t enable, uint16_t qnum, uint16_t aid, uint16_t axq_mu_timer, int16_t tx_power);

/**
 * Set the RF Trigger Frame Config in Wi-Fi firmware.
 *
 * \note call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] Enable_tx: Enable or Disable trigger frame transmission.
 * \param[in] Standalone_hetb: Enable or Disable Standalone HE TB support.
 * \param[in] FRAME_CTRL_TYPE: Frame control type.
 * \param[in] FRAME_CTRL_SUBTYPE: Frame control subtype.
 * \param[in] FRAME_DURATION: Max Duration time.
 * \param[in] TriggerType: Identifies the Trigger frame variant and its encoding.
 * \param[in] UlLen: Indicates the value of the L-SIG LENGTH field of the solicited HE TB PPDU.
 * \param[in] MoreTF: Indicates whether a subsequent Trigger frame is scheduled for transmission or not.
 * \param[in] CSRequired: Required to use ED to sense the medium and to consider the medium state and the NAV in
 * determining whether to respond or not.
 * \param[in] UlBw: Indicates the bandwidth in the HE-SIG-A field of the HE TB PPDU.
 * \param[in] LTFType: Indicates the LTF type of the HE TB PPDU response.
 * \param[in] LTFMode: Indicates the LTF mode for an HE TB PPDU.
 * \param[in] LTFSymbol: Indicates the number of LTF symbols present in the HE TB PPDU.
 * \param[in] UlSTBC: Indicates the status of STBC encoding for the solicited HE TB PPDUs.
 * \param[in] LdpcESS: Indicates the status of the LDPC extra symbol segment.
 * \param[in] ApTxPwr: Indicates the AP's combined transmit power at the transmit antenna connector of all the antennas
 * used to transmit the triggering PPDU.
 * \param[in] PreFecPadFct: Indicates the pre-FEC padding factor.
 * \param[in] PeDisambig: Indicates PE disambiguity.
 * \param[in] SpatialReuse: Carries the values to be included in the Spatial Reuse
 * fields in the HE-SIG-A field of the solicited HE TB PPDUs.
 * \param[in] Doppler: Indicate that a midamble is present in the HE TB PPDU.
 * \param[in] HeSig2: Carries the value to be included in the Reserved field in the HE-SIG-A2 subfield of
 * the solicited HE TB PPDUs.
 * \param[in] AID12: If set to 0 allocates one or more contiguous RA-RUs for associated STAs.
 * \param[in] RUAllocReg: RUAllocReg.
 * \param[in] RUAlloc: Identifies the size and the location of the RU.
 * \param[in] UlCodingType: Indicates the code type of the solicited HE TB PPDU.
 * \param[in] UlMCS: Indicates the HE-MCS of the solicited HE TB PPDU.
 * \param[in] UlDCM: Indicates DCM of the solicited HE TB PPDU.
 * \param[in] SSAlloc: Indicates the spatial streams of the solicited HE TB PPDU.
 * \param[in] UlTargetRSSI: Indicates the expected receive signal power.
 * \param[in] MPDU_MU_SF: Used for calculating the value by which the minimum MPDU start spacing is multiplied.
 * \param[in] TID_AL: Indicates the MPDUs allowed in an A-MPDU carried in the HE TB PPDU and the maximum number of TIDs
 * that can be aggregated by the STA in the A-MPDU.
 * \param[in] AC_PL: Reserved.
 * \param[in] Pref_AC: Indicates the lowest AC that is recommended for aggregation of MPDUs in the A-MPDU contained in
 * the HE TB PPDU sent as a response to the trigger frame.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_rf_trigger_frame_cfg(uint32_t Enable_tx,
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

/**
 * Set the RF TX antenna in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] antenna: The TX antenna to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_tx_antenna(const uint8_t antenna);

/**
 * Get the RF TX antenna from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] antenna: A Pointer to a variable where TX antenna is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_tx_antenna(uint8_t *antenna);

/**
 * Set RF RX antenna in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] antenna: The RX antenna to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_rx_antenna(const uint8_t antenna);

/**
 * Get RF RX antenna from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] antenna: A Pointer to a variable where RX antenna is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_rx_antenna(uint8_t *antenna);

/**
 * Set RF RX power in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] power: The RF RX power to be set in Wi-Fi firmware.
 *                  For RW610, 20M bandwidth max linear output power is 20db per data sheet.
 * \param[in] mod: The modulation to be set in Wi-Fi firmware.
 * \param[in] path_id: The Path ID to be set in Wi-Fi firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_tx_power(const uint32_t power, const uint8_t mod, const uint8_t path_id);

/**
 * Set the RF TX Frame in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] enable: Enable/Disable RF TX Frame
 * \param[in] data_rate: Rate index corresponding to legacy/HT/VHT rates
 * \param[in] frame_pattern: Payload pattern
 * \param[in] frame_length: Payload length
 * \param[in] adjust_burst_sifs: Enabl/Disable adjust burst SIFS3 Gap
 * \param[in] burst_sifs_in_us: Burst SIFS in us
 * \param[in] short_preamble: Enable/Disable short preamble
 * \param[in] act_sub_ch: Enable/Disable active sub channel
 * \param[in] short_gi: Short guard interval
 * \param[in] adv_coding: Enable/Disable adv coding
 * \param[in] tx_bf: Enable/Disable beamforming
 * \param[in] gf_mode: Enable/Disable green field mode
 * \param[in] stbc: Enable/Disable STBC
 * \param[in] bssid: BSSID
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_tx_frame(const uint32_t enable,
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

/**
 * Set the RF OTP (one-time password) MAC address in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] MAC: A pointer to a variable where OTP MAC address is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_otp_mac_addr(uint8_t *mac);

/**
 * Get the RF OTP MAC address from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] MAC: A Pointer to a variable where OTP MAC address is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_otp_mac_addr(uint8_t *mac);

/**
 * Set the RF OTP calculate data in Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[in] cal_data: A Pointer to a variable where OTP calculate data is to be stored.
 * \param[in] cal_data_len: The length of OTP calculate data.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_set_rf_otp_cal_data(const uint8_t *cal_data, uint32_t cal_data_len);

/**
 * Get the RF OTP calculate data from Wi-Fi firmware.
 *
 * \note  call \ref wlan_set_rf_test_mode API before using this API.
 *
 * \param[out] cal_data: A pointer to a variable where OTP calculate data is to be stored.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_get_rf_otp_cal_data(uint8_t *cal_data);
#endif
#if CONFIG_WIFI_FW_DEBUG
/** This function registers callbacks which are used to generate firmware dump on USB
 * device.
 *
 * \param[in] wlan_usb_init_cb: Callback to initialize usb device.
 * \param[in] wlan_usb_mount_cb: Callback to mount usb device.
 * \param[in] wlan_usb_file_open_cb: Callback to open file on usb device for firmware dump.
 * \param[in] wlan_usb_file_write_cb: Callback to write firmware dump data to opened file.
 * \param[in] wlan_usb_file_close_cb: Callback to close firmware dump file.
 *
 */
void wlan_register_fw_dump_cb(void (*wlan_usb_init_cb)(void),
                              int (*wlan_usb_mount_cb)(),
                              int (*wlan_usb_file_open_cb)(char *test_file_name),
                              int (*wlan_usb_file_write_cb)(uint8_t *data, size_t data_len),
                              int (*wlan_usb_file_close_cb)());

#endif

#if CONFIG_WIFI_EU_CRYPTO
#define EU_CRYPTO_DATA_MAX_LENGTH  1300U
#define EU_CRYPTO_KEY_MAX_LENGTH   32U
#define EU_CRYPTO_KEYIV_MAX_LENGTH 32U
#define EU_CRYPTO_NONCE_MAX_LENGTH 14U
#define EU_CRYPTO_AAD_MAX_LENGTH   32U

/** Set crypto RC4 (rivest cipher 4) algorithm encrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The KeyLength + KeyIVLength valid range [1,256].
 * \param[in] KeyIV: KeyIV
 * \param[in] KeyIVLength: The KeyLength + KeyIVLength valid range [1,256].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 1200.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the encrypted
 * data. The value of DataLength is updated to the encrypted data length. The length of the encrypted data is the same
 * as the origin DataLength.
 */
int wlan_set_crypto_RC4_encrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto RC4 (rivest cipher 4) algorithm decrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The KeyLength + KeyIVLength valid range [1,256].
 * \param[in] KeyIV: KeyIV
 * \param[in] KeyIVLength: The KeyLength + KeyIVLength valid range [1,256].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 1200.
 *
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the decrypted
 * data. The value of DataLength is updated to the decrypted data length. The length of the decrypted data is the same
 * as the origin DataLength.
 */
int wlan_set_crypto_RC4_decrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto AES_ECB (advanced encryption standard, electronic codebook) algorithm encrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/24/32.
 * \param[in] KeyIV: KeyIV should point to a 8 bytes array with any value in the array.
 * \param[in] KeyIVLength: The keyIV length is 8.
 * \param[in] Data: Data
 * \param[in] DataLength: The data length is 16.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the encrypted
 * data. The value of DataLength is updated to the encrypted data length. The length of the encrypted data is the same
 * as the origin DataLength.
 */
int wlan_set_crypto_AES_ECB_encrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto AES_ECB (advanced encryption standard, electronic codebook) algorithm decrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/24/32.
 * \param[in] KeyIV: KeyIV should point to a 8 bytes array with any value in the array.
 * \param[in] KeyIVLength: The keyIV length is 8.
 * \param[in] Data: Data
 * \param[in] DataLength: The data length is 16.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the decrypted
 * data. The value of DataLength is updated to the decrypted data length. The length of the decrypted data is the same
 * as the origin DataLength.
 */
int wlan_set_crypto_AES_ECB_decrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto AES_WRAP (advanced encryption standard wrap) algorithm encrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/24/32.
 * \param[in] KeyIV: KeyIV
 * \param[in] KeyIVLength: The keyIV length is 8.
 * \param[in] Data: Data
 * \param[in] DataLength: The data length valid range [8,1016].
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the encrypted
 * data. The value of DataLength is updated to the encrypted data length. The encrypted data is 8 bytes more than the
 * original data. Therefore, the address pointed to by Data needs to reserve enough space.
 */
int wlan_set_crypto_AES_WRAP_encrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto AES_WRAP algorithm decrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/24/32.
 * \param[in] KeyIV: KeyIV
 * \param[in] KeyIVLength: The keyIV length is 8.
 * \param[in] Data: Data
 * \param[in] DataLength: The data length valid range [8,1016].
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the decrypted
 * data. The value of DataLength is updated to the decrypted data length. The decrypted data is 8 bytes less than the
 * original data.
 */
int wlan_set_crypto_AES_WRAP_decrypt(
    const t_u8 *Key, const t_u16 KeyLength, const t_u8 *KeyIV, const t_u16 KeyIVLength, t_u8 *Data, t_u16 *DataLength);

/** Set crypto AES_CCMP (counter mode with cipher block chaining message authentication code protocol) 
 * algorithm encrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/32.
 * \param[in] AAD: AAD
 * \param[in] AADLength: The maximum AAD length is 30.
 * \param[in] Nonce: Nonce
 * \param[in] NonceLength: The nonce length valid range [7,13].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 80.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the encrypted
 * data. The value of DataLength is updated to the encrypted data length. The encrypted data is 8 bytes (when key length is 16)
 * or 16 bytes (when key length is 32) more than the original data.
 * Therefore, the address pointed to by Data needs to reserve enough space.
 */
int wlan_set_crypto_AES_CCMP_encrypt(const t_u8 *Key,
                                     const t_u16 KeyLength,
                                     const t_u8 *AAD,
                                     const t_u16 AADLength,
                                     const t_u8 *Nonce,
                                     const t_u16 NonceLength,
                                     t_u8 *Data,
                                     t_u16 *DataLength);

/** Set crypto AES_CCMP algorithm decrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/32.
 * \param[in] AAD: AAD
 * \param[in] AADLength: The maximum AAD length is 30.
 * \param[in] Nonce: Nonce
 * \param[in] NonceLength: The nonce length valid range [7,13].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 80.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the decrypted
 * data. The value of DataLength is updated to the decrypted data length. The decrypted data is 8 bytes (when key length is 16)
 * or 16 bytes (when key length is 32) less than the original data.
 */
int wlan_set_crypto_AES_CCMP_decrypt(const t_u8 *Key,
                                     const t_u16 KeyLength,
                                     const t_u8 *AAD,
                                     const t_u16 AADLength,
                                     const t_u8 *Nonce,
                                     const t_u16 NonceLength,
                                     t_u8 *Data,
                                     t_u16 *DataLength);

/** Set crypto AES_GCMP (galois/counter mode with AES-GMAC) algorithm encrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/32.
 * \param[in] AAD: AAD
 * \param[in] AADLength: The maximum AAD length is 30.
 * \param[in] Nonce: Nonce
 * \param[in] NonceLength: The nonce length valid range [7,13].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 80.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the encrypted
 * data. The value of DataLength is updated to the encrypted data length. The encrypted data is 16 bytes more than the
 * original data. Therefore, the address pointed to by Data needs to reserve enough space.
 */
int wlan_set_crypto_AES_GCMP_encrypt(const t_u8 *Key,
                                     const t_u16 KeyLength,
                                     const t_u8 *AAD,
                                     const t_u16 AADLength,
                                     const t_u8 *Nonce,
                                     const t_u16 NonceLength,
                                     t_u8 *Data,
                                     t_u16 *DataLength);

/** Set crypto AES_CCMP algorithm decrypt command parameters.
 *
 * \param[in] Key: key
 * \param[in] KeyLength: The key length is 16/32.
 * \param[in] AAD: AAD
 * \param[in] AADLength: The maximum AAD length is 30.
 * \param[in] Nonce: Nonce
 * \param[in] NonceLength: The nonce length valid range [7,13].
 * \param[in] Data: Data
 * \param[in] DataLength: The maximum data length is 80.
 * \return WM_SUCCESS if successful.
 * \return -WM_E_PERM if not supported.
 * \return -WM_FAIL if failure.
 *
 * \note If the function returns WM_SUCCESS, the data in the memory pointed to by data is overwritten by the decrypted
 * data. The value of DataLength is updated to the decrypted data length. The decrypted data is 16 bytes less than the
 * original data.
 */
int wlan_set_crypto_AES_GCMP_decrypt(const t_u8 *Key,
                                     const t_u16 KeyLength,
                                     const t_u8 *AAD,
                                     const t_u16 AADLength,
                                     const t_u8 *Nonce,
                                     const t_u16 NonceLength,
                                     t_u8 *Data,
                                     t_u16 *DataLength);
#endif

#if CONFIG_WIFI_MEM_ACCESS
/** This function reads/writes adapter memory location value.
 *
 *\param[in]        action:      0 -- read, 1 -- write
 *\param[in]        addr:        Specifies the memory address that is to be read/write.
 *\param[in,out]    value:       Value if specified, stand for write action, then that value can be written to that
 *offset in the specified register. Value should be specified in hexadecimal. Otherwise, it stands for read action, the
 *value is updated with read value.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_mem_access(uint16_t action, uint32_t addr, uint32_t *value);
#endif

#if CONFIG_WIFI_BOOT_SLEEP
/** This function get/set Wi-Fi boot sleep enable status.
 *
 *\param[in]        action:      0 -- get, 1 -- set
 *\param[in,out]    enable:      If action is get then enable value is used to store firmware returned value otherwise it
 *is set in firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_boot_sleep(uint16_t action, uint16_t *enable);
#endif

#if !defined(RW610)
/**
 * This function sends the host command to firmware and copies back response to caller provided buffer in case of
 * success response from firmware is not parsed by this function but just copied back to the caller buffer.
 *
 * \param[in]    cmd_buf:         Buffer containing the host command with header
 * \param[in]    cmd_buf_len:     length of valid bytes in cmd_buf
 * \param[out]   host_resp_buf:   Caller provided buffer, in case of success command response is copied to this
 * buffer can be same as cmd_buf
 * \param[in]    resp_buf_len:    resp_buf's allocated length
 * \param[out]   reqd_resp_len:
 * length of valid bytes in response buffer if successful otherwise invalid.
 * \return       WM_SUCCESS in case of success.
 * \return       WM_E_INBIG in case cmd_buf_len is bigger than the commands that can be handled by driver.
 * \return       WM_E_INSMALL in case cmd_buf_len is smaller than the minimum length. Minimum length is at least
 * the length of command header. see Note for same.
 * \return       WM_E_OUTBIG in case the
 * resp_buf_len is not sufficient to copy response from firmware. reqd_resp_len is updated with the response size.
 * \return       WM_E_INVAL in case cmd_buf_len and resp_buf_len have invalid values.
 * \return       WM_E_NOMEM in case cmd_buf, resp_buf and reqd_resp_len are NULL
 * \note         Brief on the command Header: Start 8 bytes of cmd_buf should have these values set.
 *               Firmware would update resp_buf with these 8 bytes at the start.\n
 *               2 bytes : Command.\n
 *               2 bytes : Size.\n
 *               2 bytes : Sequence number.\n
 *               2 bytes : Result.\n
 *               Rest of buffer length is Command/Response Body.
 */
int wlan_send_hostcmd(
    const void *cmd_buf, uint32_t cmd_buf_len, void *host_resp_buf, uint32_t resp_buf_len, uint32_t *reqd_resp_len);
#endif

#if CONFIG_11AX
#if CONFIG_WIFI_HTC_DEBUG
/**
 * This function is used to set HTC (high throughput control) parameter.
 *
 *  \param[in]    count:            count of MPDUs with SW HTC
 *  \param[in]    vht:              vht bit of HT Control
 *  \param[in]    he:               he bit of HT Control
 *  \param[in]    rxNss:            Rx NSS subfield of OM Control
 *  \param[in]    channelWidth:     Channel Width subfield of OM Control
 *  \param[in]    ulMuDisable:      UL MU Disable subfield of OM Control
 *  \param[in]    txNSTS:           Tx NSTS subfield of OM Control
 *  \param[in]    erSuDisable:      ER SU Disable subfield of OM Control
 *  \param[in]    dlResoundRecomm:  DL MU-MIMO Resound Recommendation subfield of OM Control
 *  \param[in]    ulMuDataDisable:  UL MU Data Disable subfield of OM Control
 *
 * \return WM_SUCCESS if operation is successful, otherwise return -WM_FAIL
 */
int wlan_send_debug_htc(const uint8_t count,
                        const uint8_t vht,
                        const uint8_t he,
                        const uint8_t rxNss,
                        const uint8_t channelWidth,
                        const uint8_t ulMuDisable,
                        const uint8_t txNSTS,
                        const uint8_t erSuDisable,
                        const uint8_t dlResoundRecomm,
                        const uint8_t ulMuDataDisable);
#endif

/**
 * This function is used to enable/disable HTC (high throughput control).
 *
 *  \param[in]    option:         1 => Enable; 0 => Disable
 *
 * \return WM_SUCCESS if operation is successful, otherwise return -WM_FAIL
 */
int wlan_enable_disable_htc(uint8_t option);
#endif

#if CONFIG_11AX
/**
 * Use this API to set the set 802.11ax TX OMI (operating mode indication).
 *
 * \param[in] interface: Interface type STA or uAP.
 *            0: STA
 *            1: uAP
 * \param[in] tx_omi: value to be sent to firmware
 * \param[in] tx_option: value to be sent to firmware
 *            1: send OMI (operating mode indication) in QoS (quality of service) data.
 * \param[in] num_data_pkts: value to be sent to firmware
 *            num_data_pkts is applied only if OMI is sent in QoS data frame.
 *            It specifies the number of consecutive data frames containing the OMI.
 *            Minimum value is 1
 *            Maximum value is 16
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_11ax_tx_omi(const t_u8 interface, const t_u16 tx_omi, const t_u8 tx_option, const t_u8 num_data_pkts);
/**
 * Set 802.11ax OBSS (overlapping basic service set) narrow bandwidth RU (resource unit) tolerance time
 * In uplink transmission, AP sends a trigger frame to all the stations that can be involved in the upcoming
 *transmission, and then these stations transmit Trigger-based(TB) PPDU in response to the trigger frame. If STA
 *connects to AP which channel is set to 100,STA doesn't support 26 tones RU. The API should be called when station is
 *in disconnected state.
 *
 *
 * \param[in] tol_time:     Valid range [1...3600]
 *          tolerance time is in unit of seconds.
 *			STA periodically check AP's beacon for ext cap bit79 (OBSS Narrow bandwidth RU in ofdma tolerance support)
 * 			and set 20 tone RU tolerance time if ext cap bit79 is not set
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_11ax_tol_time(const t_u32 tol_time);
/**
 * Use this API to set the RU TX power limit.
 *
 * \param[in] rutx_pwr_cfg:       802.11ax rutxpwr of sub-bands to be sent to firmware.
 *                                refer to \ref rutxpowerlimit_cfg_set_WW[]
 * \param[in] rutx_pwr_cfg_len:   Size of rutx_pwr_cfg buffer.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_11ax_rutxpowerlimit(const void *rutx_pwr_cfg, uint32_t rutx_pwr_cfg_len);

/**
 * Use this API to set the RU TX power limit by channel based approach.
 *
 * \param[in] ru_pwr_cfg: 802.11ax rutxpwr of channels to be sent to firmware.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_11ax_rutxpowerlimit_legacy(const wlan_rutxpwrlimit_t *ru_pwr_cfg);

/**
 * Use this API to get the RU TX power limit by channel based approach.
 *
 * \param[out] ru_pwr_cfg: 802.11ax rutxpwr of channels to be get from firmware.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_get_11ax_rutxpowerlimit_legacy(wlan_rutxpwrlimit_t *ru_pwr_cfg);

/** Set 802.11ax configuration parameters
 *
 * \param[in] ax_config: 802.11ax configuration parameters to be sent to firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_11ax_cfg(wlan_11ax_config_t *ax_config);

/** Get default 802.11ax configuration parameters
 *
 * \return 802.11ax configuration parameters default array.
 */
wlan_11ax_config_t *wlan_get_11ax_cfg(void);

#if CONFIG_11AX_TWT
/** Set broadcast TWT (target wake time) configuration parameters
 *
 * \param[in] btwt_config: Broadcast TWT setup parameters to be sent to firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_btwt_cfg(const wlan_btwt_config_t *btwt_config);

/** Get broadcast TWT configuration parameters
 *
 * \return Broadcast TWT setup parameters default configuration array.
 */
wlan_btwt_config_t *wlan_get_btwt_cfg(void);

/** Set TWT setup configuration parameters
 *
 * \param[in] twt_setup: TWT setup parameters to be sent to firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_twt_setup_cfg(const wlan_twt_setup_config_t *twt_setup);

/** Get TWT setup configuration parameters
 *
 * \return TWT setup parameters default array.
 */
wlan_twt_setup_config_t *wlan_get_twt_setup_cfg(void);

/** Set TWT teardown configuration parameters
 *
 * \param[in] teardown_config: TWT teardown parameters sent to firmware.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_twt_teardown_cfg(const wlan_twt_teardown_config_t *teardown_config);

/** Get TWT teardown configuration parameters
 *
 * \return TWT Teardown parameters default array
 */
wlan_twt_teardown_config_t *wlan_get_twt_teardown_cfg(void);

/** Get TWT report
 *
 * \param[out] twt_report: TWT report parameter.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_twt_report(wlan_twt_report_t *twt_report);

#endif /* CONFIG_11AX_TWT */

#if CONFIG_MMSF
/**
 * Set 802.11ax AMPDU (aggregate medium access control (MAC) protocol data unit) density configuration.
 * \param[in] enable:     0 - Disbale MMSF;  1 - Enable MMSF
 * \param[in] Density:    AMPDU density value. Default value is 0x30.
 * \param[in] MMSF:       AMPDU MMSF value. Default value is 0x6.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_mmsf(const t_u8 enable, const t_u8 Density, const t_u8 MMSF);

/**
 * Get 802.11ax AMPDU density configuration.
 * \param[out] enable:     0 - Disbale MMSF;  1 - Enable MMSF
 * \param[out] Density:    AMPDU Density value.
 * \param[out] MMSF:       AMPDU MMSF value. Default value is 0x6.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_mmsf(t_u8 *enable, t_u8 *Density, t_u8 *MMSF);
#endif
#endif /* CONFIG_11AX */

#if CONFIG_WIFI_RECOVERY
int wlan_recovery_test(void);
#endif

#if CONFIG_WIFI_CLOCKSYNC
/** Set clock sync GPIO based TSF (time synchronization function).
 *
 * \param[in] tsf_latch: Clock sync TSF latch parameters to be sent to firmware
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_clocksync_cfg(const wlan_clock_sync_gpio_tsf_t *tsf_latch);
/** Get TSF info from firmware using GPIO latch.
 *
 * \param[out] tsf_info: TSF info parameter received from firmware
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_tsf_info(wlan_tsf_info_t *tsf_info);
#endif /* CONFIG_WIFI_CLOCKSYNC */

#if CONFIG_HEAP_DEBUG
/**
 * Show os mem alloc and free info.
 *
 */
void wlan_show_os_mem_stat(void);
#endif

#if CONFIG_MULTI_CHAN
/**
 * Set multi-channel status disable/enable.
 * \param[in]      status:   multi channel status
 * 0-disable, 1-enable
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_multi_chan_status(const int status);

/**
 * Get dynamic rapid channel switch status disable/enable.
 * \param[out]      status:  multi channel status
 * 0-disable, 1-enable
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_multi_chan_status(int *status);

/**
 * Set dynamic rapid channel switch config.
 * \param[in]      num:       array length of drcs_cfg[]
 * \param[in]     drcs_cfg:   multi-channel config, maybe an array
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_drcs_cfg(const wlan_drcs_cfg_t *drcs_cfg, const int num);

/**
 * Get dynamic rapid channel switch config.
 * \param[in]      num:       array length of drcs_cfg[]
 * \param[out] drcs_cfg:  multi-channel config, maybe an array
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_drcs_cfg(wlan_drcs_cfg_t *drcs_cfg, int num);
#endif

#if CONFIG_11R
/**
 * Start FT roaming : This API is used to initiate fast BSS transition based
 * roaming.
 *
 * \param[in] bssid:       BSSID of AP to roam
 * \param[in] channel:     Channel of AP to roam
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_ft_roam(const t_u8 *bssid, const t_u8 channel);
#endif

/**
 * This API can be used to start/stop the management frame forwarded
 * to host through data path.
 *
 * \param[in] bss_type: The interface from which management frame needs to be
 *            collected 0: STA, 1: uAP

 * \param[in] mgmt_subtype_mask:     Management Subtype Mask
 *            If Bit X is set in mask, it means that IEEE Management Frame
 *            SubType X is to be filtered and passed through to host.
 *            Bit                   Description
 *            [31:14]               Reserved
 *            [13]                  Action frame
 *            [12:9]                Reserved
 *            [8]                   Beacon
 *            [7:6]                 Reserved
 *            [5]                   Probe response
 *            [4]                   Probe request
 *            [3]                   Reassociation response
 *            [2]                   Reassociation request
 *            [1]                   Association response
 *            [0]                   Association request
 *            Support multiple bits set.
 *            0 = stop forward frame
 *            1 = start forward frame
 *\param[in] rx_mgmt_callback: The receive callback where the received management
 *           frames are passed.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 *
 * \note Pass management subtype mask all zero to disable all the management
 *       frame forward to host.
 */
int wlan_rx_mgmt_indication(const enum wlan_bss_type bss_type,
                            const uint32_t mgmt_subtype_mask,
                            int (*rx_mgmt_callback)(const enum wlan_bss_type bss_type,
                                                    const wlan_mgmt_frame_t *frame,
                                                    const size_t len));

#if CONFIG_WMM
void wlan_wmm_tx_stats_dump(int bss_type);
#endif

#if CONFIG_SCAN_CHANNEL_GAP
/**
 * Set scan channel gap.
 * \param[in] scan_chan_gap:      Time gap to be used between two consecutive channels scan.
 *
 */
void wlan_set_scan_channel_gap(unsigned scan_chan_gap);
#endif

#if CONFIG_11K
/**
 * Enable/Disable host 802.11k feature.
 *
 * \param[in] enable_11k: the value of 802.11k configuration.
 *            0: disable host 11k
 *            1: enable host 11k
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_host_11k_cfg(int enable_11k);

/**
 * Get enable/disable host 802.11k feature flag.
 *
 * \return TRUE if 802.11k is enabled, return FALSE if 802.11k is disabled.
 *
 */
bool wlan_get_host_11k_status(void);

/**
 * Host send neighbor report request.
 *
 * \param[in] ssid: The SSID for neighbor report
 * \note ssid parameter is optional, pass NULL pointer to ignore SSID input if not specify SSID
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_host_11k_neighbor_req(const char *ssid);
#endif

#if CONFIG_11V
/**
 * Host send BSS transition management query.
 * STA sends BTM (BSS transition management) query, and the AP supporting 11V will response BTM request,
 * the AP will parse neighbor report in the BTM request and response the BTM response to AP to indicate the receive status.
 * \param[in] query_reason: [0..16] IEEE 802.11v BTM (BSS transition management) Query reasons.
 * Refer to IEEE Std 802.11v-2011 - Table 7-43x-Transition and Transition Query reasons table.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_host_11v_bss_trans_query(t_u8 query_reason);
#endif

#if !CONFIG_WPA_SUPP
#if CONFIG_DRIVER_MBO
/**
 * Enable/Disable MBO (multi band operation) feature.
 *
 * \param[in] enable_mbo: The value of MBO configuration.
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_host_mbo_cfg(int enable_mbo);

/**
 * MBO channel operation preference configuration
 *
 * \param[in] ch0: Channel number.
 * \param[in] prefer0: Operation preference for ch0.
 * \param[in] ch1: Channel number.
 * \param[in] prefer1: Operation preference for ch1.
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_mbo_peferch_cfg(t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
#endif
#endif

#if (CONFIG_11MC) || (CONFIG_11AZ)
int wlan_unassoc_ftm_cfg(const t_u16 action, const t_u16 config);

/**
 * Start or stop FTM (Wi-Fi fine time measurement) based on the command from CLI.
 * \param[in] action: 1: start FTM  2: stop FTM.
 * \param[in] loop_cnt:         number of FTM sessions to run repeatedly (default:1,  0: non-stop, n>1: n times).
 * \param[in] MAC:              MAC address of the peer with whom FTM session is required.
 * \param[in] channel:          Channel on which FTM is started.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_ftm_start_stop(const t_u16 action, const t_u8 loop_cnt, const t_u8 *mac, const t_u8 channel);

/**
 * Config FTM protocol.
 * \param[in] protocol: 0: Dot11mc, 1: Dot11az_ntb, 2: Dot11az_tb
 * \param[in] ftm_ranging_cfg:  FTM ranging config.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_ftm_cfg(const t_u8 protocol, ranging_11az_cfg_t *ftm_ranging_cfg);

int wlan_ftm_11mc_cfg(ftm_11mc_nego_cfg_t *ftm_11mc_nego_cfg);

int wlan_ftm_location_cfg(location_cfg_info_t *ftm_location_cfg);

int wlan_ftm_civic_cfg(location_civic_rep_t *ftm_civic_cfg);

extern location_cfg_info_t g_ftm_location_cfg;
extern location_civic_rep_t g_ftm_civic_cfg;

#endif

#if CONFIG_WPA_SUPP
#if (CONFIG_11AX && defined(CONFIG_MBO))
/**
 * Multi band operation (MBO) non-preferred channels
 *
 * A space delimited list of non-preferred channels where each channel is a colon delimited list of values.
 *
 * Format:
 *
 * non_pref_chan=oper_class:chan:preference:reason
 * Example:
 *
 * non_pref_chan=81:5:10:2 81:1:0:2 81:9:0:2
 *
 * \param[in] non_pref_chan: list of non-preferred channels.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_mbo_peferch_cfg(const char *non_pref_chan);

/**
 * MBO set cellular data capabilities
 *
 * \param[in] cell_capa: 1 = Cellular data connection available
 * 2 = Cellular data connection not available
 * 3 = Not cellular capable (default)
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_mbo_set_cell_capa(t_u8 cell_capa);

/**
 * Optimized connectivity experience (OCE)
 *
 * \param[in] oce: Enable OCE features
 * 1 = Enable OCE in non-AP STA mode (default; disabled if the driver
 * does not indicate support for OCE in STA mode).
 * 2 = Enable OCE in STA-CFON mode.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_mbo_set_oce(t_u8 oce);
#endif

/**
 * Opportunistic key caching (also known as proactive key caching) default
 * This parameter can be used to set the default behavior for the
 * proactive_key_caching parameter. By default, OKC is disabled unless enabled
 * with the global okc=1 parameter or with the per-network
 * pkc(proactive_key_caching)=1 parameter. With okc=1, OKC is enabled by default, but
 * can be disabled with per-network pkc(proactive_key_caching)=0 parameter.
 *
 * \param[in] okc: Enable opportunistic key caching
 *
 * 0 = Disable OKC (default)
 * 1 = Enable OKC
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_okc(t_u8 okc);

/**
 * Dump text list of entries in PMKSA (pairwise master key security association) cache.
 *
 * \param[out] buf: Buffer to save PMKSA cache text list
 * \param[in] buflen: length of the buffer
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_pmksa_list(char *buf, size_t buflen);

/**
 * Flush PTKSA cache entries
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_pmksa_flush(void);

/**
 * Set wpa supplicant scan interval in seconds
 *
 * \param[in] scan_int: Scan interval in seconds
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_scan_interval(int scan_int);
#endif


#if CONFIG_ECSA
/**
 * Send the ecsa configuration parameter to FW.
 *
 *\param[in] block_tx:      0 -- no need to block traffic,1 -- need block traffic.
 *\param[in] oper_class:    Operating class according to IEEE std802.11 spec, refer to Annex E,
 *                          when 0 is used, automatically get operclass through band_width and channel.
 *\param[in] channel:       The channel can switch to.
 *\param[in] switch_count:  Channel switch time to send ECSA ie, unit is 110ms.
 *\param[in] band_width:    Channel width switch to(optional), only for 5G channels.
 *                          Depends on the hardware capabilities, when the hardware does not support, it can
 *automatically downgrade. Redfinch support 20M. 0 -- 20MHZ, 1 -- 40M above, 3 -- 40M below, 4 -- 80M, 5 -- 160M
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_uap_set_ecsa_cfg(t_u8 block_tx, t_u8 oper_class, t_u8 channel, t_u8 switch_count, t_u8 band_width);
#endif

#if CONFIG_SUBSCRIBE_EVENT_SUPPORT

/** Type enum definition of subscribe event */
typedef enum
{
    /** Event Id for subscribe event RSSI low */
    EVENT_SUB_RSSI_LOW = 0,
    /** Event Id for subscribe event RSSI high */
    EVENT_SUB_RSSI_HIGH,
    /** Event Id for subscribe event snr low */
    EVENT_SUB_SNR_LOW,
    /** Event Id for subscribe event snr high */
    EVENT_SUB_SNR_HIGH,
    /** Event Id for subscribe event max fail */
    EVENT_SUB_MAX_FAIL,
    /** Event Id for subscribe event beacon missed */
    EVENT_SUB_BEACON_MISSED,
    /** Event Id for subscribe event data RSSI low */
    EVENT_SUB_DATA_RSSI_LOW,
    /** Event Id for subscribe event data RSSI high */
    EVENT_SUB_DATA_RSSI_HIGH,
    /** Event Id for subscribe event data snr low */
    EVENT_SUB_DATA_SNR_LOW,
    /** Event Id for subscribe event data snr high */
    EVENT_SUB_DATA_SNR_HIGH,
    /** Event Id for subscribe event link quality */
    EVENT_SUB_LINK_QUALITY,
    /** Event Id for subscribe event pre_beacon_lost */
    EVENT_SUB_PRE_BEACON_LOST,
    /** Fail event id */
    MAX_EVENT_ID,
} sub_event_id;

/** Configuration for subscribe events from \ref wlan_ds_subscribe_evt */
typedef wifi_ds_subscribe_evt wlan_ds_subscribe_evt;

/**
 * Subscribe specified event from the Wi-Fi firmware. Wi-Fi firmware report the registered event to driver upon
 * configured report conditions are met.
 * \param[in] event_id: event to register as per \ref sub_event_id except for EVENT_SUB_LINK_QUALITY
 * \param[in] thresh_value: the RSSI threshold value (dBm)
 * \param[in] freq:     event frequency 0--report once, 1--report every time happened, N --
 *                      report only happened > N consecutive times.
 * \return WM_SUCCESS if set successfully, otherwise return failure.
 */
int wlan_set_subscribe_event(unsigned int event_id, unsigned int thresh_value, unsigned int freq);
/**
 * Get all subscribed events from Wi-Fi firmware along with threshold value and report frequency.
 * \param[out] sub_evt: A pointer to \ref wlan_ds_subscribe_evt to store the events data.
 * \return WM_SUCCESS if set successfully, otherwise return failure.
 */
int wlan_get_subscribe_event(wlan_ds_subscribe_evt *sub_evt);
/**
 * cancel the subscribe event to firmware
 * \param[in] event_id: event id to clear as per \ref sub_event_id
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_clear_subscribe_event(unsigned int event_id);
/**
 * subscribe link quality event
 * \param[in] event_id: event id to set, EVENT_SUB_LINK_QUALITY
 * \param[in] link_snr: link quality snr value
 * \param[in] link_snr_freq: link quality snr freq
 * \param[in] link_rate: link quality rate
 * \param[in] link_rate_freq: link quality rate freq
 * \param[in] link_tx_latency: link quality write latency
 * \param[in] link_tx_lantency_freq: link quality write latency freq
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_threshold_link_quality(unsigned int evend_id,
                                    unsigned int link_snr,
                                    unsigned int link_snr_freq,
                                    unsigned int link_rate,
                                    unsigned int link_rate_freq,
                                    unsigned int link_tx_latency,
                                    unsigned int link_tx_lantency_freq);
#endif

#if CONFIG_TSP
/**
 * Get TSP (thermal safeguard protection) configuration.
 * TSP algorithm monitors PA Tj and primarily backs off data throughput.
 * \param[out] enable:            Enable/Disable TSP algorithm
 * \param[out] back_off:          Power back off   [0...20]dB
 * \param[out] highThreshold:     High threshold  [0...300]°C
 * \param[out] lowThreshold:      Low threshold   [0...300]°C
 *             High Threshold is Greater than low threshold.
 * \param[out] dutycycstep:       Duty cycle step(percentage)
 * \param[out] dutycycmin:        Duty cycle min(percentage)
 * \param[out] highthrtemp:       High throttle threshold temperature(celsius)
 * \param[out] lowthrtemp:        Low throttle threshold temperature(celsius)
 * \param[out] currCAUTemp:       CAU TSEN temperature
 * \param[out] currRFUTemp:       RFU temperature
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_tsp_cfg(t_u16 *enable,
                     t_u32 *back_off,
                     t_u32 *highThreshold,
                     t_u32 *lowThreshold,
                     t_u32 *dutycycstep,
                     t_u32 *dutycycmin,
                     int *highthrtemp,
                     int *lowthrtemp,
                     int *currCAUTemp,
                     int *currRFUTemp);

/**
 * Set TSP (thermal safeguard protection) configuration.
 * TSP algorithm monitors and primarily backs off data throughput.
 * \param[in] enable:            Enable/Disable tsp algorithm
 * \param[in] back_off:          Power back off   [0...20]dB
 * \param[in] highThreshold:     High threshold  [0...300]Celsius
 * \param[in] lowThreshold:      Low threshold   [0...300]Celsius
 *                               High threshold is greater than low threshold.
 * \param[in] dutycycstep:       Duty cycle step(percentage)
 * \param[in] dutycycmin:        Duty cycle min(percentage)
 * \param[out] highthrtemp:      High throttle threshold temperature (celsius)
 * \param[out] lowthrtemp:       Low throttle threshold temperature (celsius)
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_tsp_cfg(t_u16 enable,
                     t_u32 back_off,
                     t_u32 highThreshold,
                     t_u32 lowThreshold,
                     t_u32 dutycycstep,
                     t_u32 dutycycmin,
                     int highthrtemp,
                     int lowthrtemp);
#endif

#if CONFIG_WIFI_REG_ACCESS
/** This function reads/writes adapter registers value.
 *
 *\param[in]        type:        Register type: 1 -- MAC, 2 -- BBP, 3 -- RF.
 *\param[in]        action:      0 -- read, 1 -- write
 *\param[in]        offset:      Specifies the offset location that is to be read/write.
 *\param[in,out]    value:       Value if specified, stand for write action, then that value can be written to that
 *                               offset in the specified register. Value should be specified in hexadecimal. Otherwise,
 *                               it stands for read action, the
 *value is updated with read value.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_reg_access(wifi_reg_t type, uint16_t action, uint32_t offset, uint32_t *value);
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
/**
 * Set/Get TX AMPDU protect mode.
 *
 * \param[in/out] prot_mode:   TX AMPDU protect mode
 *                             \ref tx_ampdu_prot_mode_para
 * \param[in]     action:      Command action
 *                             0: get TX AMPDU protect mode
 *                             1: set TX AMPDU protect mode
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_tx_ampdu_prot_mode(tx_ampdu_prot_mode_para *prot_mode, t_u16 action);
#endif

struct wlan_message
{
    t_u16 id;
    void *data;
};

#if CONFIG_MEF_CFG
enum wlan_mef_type
{
    MEF_TYPE_DELETE = 0,
    MEF_TYPE_PING,
    MEF_TYPE_ARP,
    MEF_TYPE_MULTICAST,
    MEF_TYPE_IPV6_NS,
    MEF_TYPE_END,
};
/** This function set auto ARP configuration.
 *
 * \param[in] mef_action: To be 0--discard and not wake host, 1--discard and wake host, 3--allow and wake host.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_mef_set_auto_arp(t_u8 mef_action);
/** This function set auto ping configuration.
 *
 * \param[in] mef_action: To be\n
 *                        0--discard ping packet and not wake host\n
 *                        1--discard ping packet and wake host\n
 *                        3--allow ping packet and wake host.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 *
 */
int wlan_mef_set_auto_ping(t_u8 mef_action);

/** This function set/delete MEF entries configuration.
 *
 * \param[in] type:       MEF type: MEF_TYPE_DELETE, MEF_TYPE_AUTO_PING, MEF_TYPE_AUTO_ARP
 * \param[in] mef_action: To be 0--discard and not wake host, 1--discard and wake host 3--allow and wake host.
 *
 * \return WM_SUCCESS if the call was successful.
 * \return -WM_FAIL if failed.
 */
int wlan_config_mef(int type, t_u8 mef_action);
/**
 * Use this API to enable IPv6 neighbor solicitation offload in Wi-Fi firmware.
 *
 * \param[in] mef_action: 0--discard and not wake host, 1--discard and wake host 3--allow and wake host.
 *
 * \return WM_SUCCESS if operation is successful.
 * \return -WM_FAIL if command fails.
 */
int wlan_set_ipv6_ns_mef(t_u8 mef_action);
#endif

#if CONFIG_CSI
/**
 * Send the CSI configuration parameter to firmware.
 *
 *\param[in] csi_params: CSI configuration parameter
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_csi_cfg(wlan_csi_config_params_t *csi_params);

/** This function registers callback which are used to deliver CSI (channel state information) data to user.
 *
 * \param[in] csi_data_recv_callback: Callback to deliver CSI data and max data length is 768 bytes.
 * Process data as soon as possible in callback, or else shall block there. Type of callback return value is int.
 *
 *          Memory layout of buffer:
 *          size(byte)                         items
 *          2                                  buffer len[bit 0:12]
 *          2                                  CSI signature, 0xABCD fixed
 *          4                                  User defined HeaderID
 *          2                                  Packet info
 *          2                                  Frame control field for the received packet
 *          8                                  Timestamp when packet received
 *          6                                  Received packet destination MAC Address
 *          6                                  Received packet source MAC address
 *          1                                  RSSI for antenna A
 *          1                                  RSSI for antenna B
 *          1                                  Noise floor for antenna A
 *          1                                  Noise floor for antenna B
 *          1                                  RX signal strength above noise floor
 *          1                                  Channel
 *          2                                  user defined chip ID
 *          4                                  Reserved
 *          4                                  CSI data length in DWORDs
 *                                             CSI data
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_register_csi_user_callback(int (*csi_data_recv_callback)(void *buffer, size_t len));

/** This function unregisters callback which are used to deliver CSI data to user.
 *
 * \return  WM_SUCCESS if successful
 */
int wlan_unregister_csi_user_callback(void);
/** This function get CSI default configuration data.
 *
 * \return CSI data pointer.
 */
wlan_csi_config_params_t *wlan_get_csi_cfg_param_default(void);
/** This function set CSI default configuration data.
 *
 * \param[in] in_csi_cfg: CSI default configuration data to be set.
 *
 * \return if successful return 1 else return 0.
 */
int wlan_set_csi_cfg_param_default(wlan_csi_config_params_t *in_csi_cfg);

/** 
 * This function reset Wi-Fi CSI filter data.
 */
void wlan_reset_csi_filter_data(void);
#endif

#if (CONFIG_11K) || (CONFIG_11V) || (CONFIG_11R) || (CONFIG_ROAMING)
/**
 * Use this API to set the RSSI threshold value for low RSSI event subscription.
 * When RSSI falls below this threshold firmware can generate the low RSSI event to driver.
 * This low RSSI event is used when either of CONFIG_11R, CONFIG_11K, CONFIG_11V or CONFIG_ROAMING is enabled.
 * \note By default RSSI low threshold is set at -70 dbm.
 *
 * \param[in]     threshold:     Threshold RSSI value to be set
 *
 */
void wlan_set_rssi_low_threshold(uint8_t threshold);
#endif

#if CONFIG_WPA_SUPP
#if CONFIG_WPA_SUPP_WPS
/**
 *  This function generate pin for WPS pin session.
 *
 * \param[in]  pin: A pointer to WPS pin to be generated.
 */
void wlan_wps_generate_pin(uint32_t *pin);

/** Start WPS pin session.
 *
 *  This function starts WPS pin session.
 *
 * \param[in]  pin: Pin for WPS session.
 *
 * \return WM_SUCCESS if the pin entered is valid.
 * \return -WM_FAIL if invalid pin entered.
 */
int wlan_start_wps_pin(const char *pin);

/** Start WPS PBC (push button configuration) session.
 *
 *  This function starts WPS PBC (push button configuration) session.
 *
 * \return  WM_SUCCESS if successful
 * \return -WM_FAIL if invalid pin entered.
 *
 */
int wlan_start_wps_pbc(void);

/** Cancel WPS session.
 *
 *  This function cancels ongoing WPS session.
 *
 * \return  WM_SUCCESS if successful
 * \return -WM_FAIL if invalid pin entered.
 *
 */
int wlan_wps_cancel(void);

#if CONFIG_HOSTAPD
/** Start WPS pin session.
 *
 *  This function starts AP WPS pin session.
 *
 * \param[in] pin: Pin for WPS session.
 *
 * \return WM_SUCCESS if the pin entered is valid.
 * \return -WM_FAIL if invalid pin entered.
 */
int wlan_start_ap_wps_pin(const char *pin);

/** Start WPS PBC session.
 *
 *  This function starts AP WPS PBC session.
 *
 * \return  WM_SUCCESS if successful
 * \return -WM_FAIL if invalid pin entered.
 *
 */
int wlan_start_ap_wps_pbc(void);

/** Cancel AP's WPS session.
 *
 *  This function cancels ongoing WPS session.
 *
 * \return  WM_SUCCESS if successful
 * \return -WM_FAIL if invalid pin entered.
 *
 */
int wlan_wps_ap_cancel(void);
#endif
#endif
#endif

#if (CONFIG_WPA2_ENTP) || (CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#define FILE_TYPE_NONE              0
#define FILE_TYPE_ENTP_CA_CERT      1
#define FILE_TYPE_ENTP_CLIENT_CERT  2
#define FILE_TYPE_ENTP_CLIENT_KEY   3
#define FILE_TYPE_ENTP_CA_CERT2     4
#define FILE_TYPE_ENTP_CLIENT_CERT2 5
#define FILE_TYPE_ENTP_CLIENT_KEY2  6

#if CONFIG_HOSTAPD
#define FILE_TYPE_ENTP_SERVER_CERT 8
#define FILE_TYPE_ENTP_SERVER_KEY  9
#define FILE_TYPE_ENTP_DH_PARAMS   10
#endif

/** This function specifies the enterprise certificate file
 *  This function is used before adding network profile. It can store certificate data
 *  in "wlan" global structure.
 *
 * \param[in]        cert_type:  certificate file type:
 * 1 -- FILE_TYPE_ENTP_CA_CERT,
 * 2 -- FILE_TYPE_ENTP_CLIENT_CERT,
 * 3 -- FILE_TYPE_ENTP_CLIENT_KEY.
 * \param[in]        data:       raw data of the enterprise certificate file
 * \param[in]        data_len:   length of the enterprise certificate file
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_entp_cert_files(int cert_type, t_u8 *data, t_u32 data_len);

/** This function get enterprise certificate data from "wlan" global structure
 * \param[in]        cert_type:  certificate file type:
 * 1 -- FILE_TYPE_ENTP_CA_CERT,
 * 2 -- FILE_TYPE_ENTP_CLIENT_CERT,
 * 3 -- FILE_TYPE_ENTP_CLIENT_KEY.
 * \param[out]    data:       raw data of the enterprise certificate file
 *
 * \return size of raw data
 */
t_u32 wlan_get_entp_cert_files(int cert_type, t_u8 **data);

/** This function free the temporary memory of enterprise certificate data
 *  After add new enterprise network profile, the certificate data has been parsed by mbedtls into another data, which
 * can be freed.
 *
 */
void wlan_free_entp_cert_files(void);
#endif

#if CONFIG_NET_MONITOR
/**
 * Send the network monitor configuration parameter to firmware.
 *
 *\param[in] monitor: Monitor configuration parameter
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_net_monitor_cfg(wlan_net_monitor_t *monitor);

/** This function registers callback which are used to deliver monitor data to user.
 *
 * \param[in] monitor_data_recv_callback: Callback to deliver monitor data and data length to user.
 *          Memory layout of buffer:
 *          offset(byte)                        items
 *          0                                   rssi
 *          1                                   802.11 MAC header
 *          1 + 'size of 802.11 MAC header'     frame body
 *
 */
void wlan_register_monitor_user_callback(int (*monitor_data_recv_callback)(void *buffer, t_u16 data_len));

/** This function deregisters monitor callback.
 *
 */
void wlan_deregister_net_monitor_user_callback(void);
#endif

#if CONFIG_WIFI_CAPA
/** Check if Wi-Fi hardware support 802.11n for on 2.4G or 5G bands.
 *
 * \param[in] channel: Channel number.
 *
 * \return true if 802.11n is supported or false if not.
 */
uint8_t wlan_check_11n_capa(unsigned int channel);

/** Check if Wi-Fi hardware support 802.11ac for on 2.4G or 5G bands.
 *
 * \param[in] channel: Channel number.
 *
 * \return true if 802.11ac is supported or false if not.
 */
uint8_t wlan_check_11ac_capa(unsigned int channel);

/** Check if Wi-Fi hardware support 802.11ax for on 2.4G or 5G bands.
 *
 * \param[in] channel: Channel number.
 *
 * \return true if 802.11ax is supported or false if not.
 */
uint8_t wlan_check_11ax_capa(unsigned int channel);
#endif

#if (CONFIG_IPS)
/**
 * Config IEEE power save mode (IPS). If the option is 1, the IPS hardware listens to beacon frames after Wi-Fi CPU enters
 * power save mode. When there is work needed to done by Wi-Fi CPU, Wi-Fi CPU can be woken up by ips hardware.
 *
 * \param[in] option:   0/1  disable/enable ips
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_ips(int option);
#endif

#ifdef STA_SUPPORT
/**
 * Get RSSI information.
 * \param[out] signal:   RSSI information get report buffer
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_signal_info(wlan_rssi_info_t *signal);
#endif

/**
 * Set band configuration.
 * \param[in] bandcfg:   band configuration
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_bandcfg(wlan_bandcfg_t *bandcfg);

/**
 * Get band configuration.
 * \param[out] bandcfg:   band configuration
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_bandcfg(wlan_bandcfg_t *bandcfg);

#if CONFIG_COMPRESS_TX_PWTBL
/**
 * Set TX power table according to region code
 * \param[in] region_code: region code
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_rg_power_cfg(t_u16 region_code);
#endif

#if (CONFIG_COMPRESS_RU_TX_PWTBL) && (CONFIG_11AX)
/**
 * set ru tx power table
 * \param[in] region_code: region code
 * \return WM_SUCCESS if successful otherwise failure.
 */
int wlan_set_ru_power_cfg(t_u16 region_code);
#endif

#if CONFIG_TURBO_MODE
/**
 * Get turbo mode.
 * \param[out] mode:   turbo mode
 *                          0: disable turbo mode
 *                          1: turbo mode 1
 *                          2: turbo mode 2
 *                          3: turbo mode 3
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_turbo_mode(t_u8 *mode);

/**
 * Get uAP turbo mode.
 * \param[out] mode:   turbo mode
 *                          0: disable turbo mode
 *                          1: turbo mode 1
 *                          2: turbo mode 2
 *                          3: turbo mode 3
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_uap_turbo_mode(t_u8 *mode);

/**
 * Set turbo mode.
 * \param[in] mode:   turbo mode
 *                          0: disable turbo mode
 *                          1: turbo mode 1
 *                          2: turbo mode 2
 *                          3: turbo mode 3
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_turbo_mode(t_u8 mode);

/**
 * Set uAP turbo mode.
 * \param[in] mode:   turbo mode
 *                          0: disable turbo mode
 *                          1: turbo mode 1
 *                          2: turbo mode 2
 *                          3: turbo mode 3
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_uap_turbo_mode(t_u8 mode);
#endif

/**
 * Set multiple dtim for next wakeup RX beacon time
 *
 * \param[in] multiple_dtims:        num dtims, range [1,20]
 * \param[in] bcn_miss_timeout:      becaon miss interval
 * \param[in] local_listen_interval: local listen interval
 * \param[in] adhoc_wake_period:     adhoc awake period
 * \param[in] mode:                  mode - (0x01 - firmware to automatically choose PS_POLL or NULL mode,
 *                                           0x02 - PS_POLL,
 *                                           0x03 - NULL mode )
 * \param[in] delay_to_ps:           Delay to PS in milliseconds
 */
void wlan_set_ps_cfg(t_u16 multiple_dtims,
                     t_u16 bcn_miss_timeout,
                     t_u16 local_listen_interval,
                     t_u16 adhoc_wake_period,
                     t_u16 mode,
                     t_u16 delay_to_ps);

#if CONFIG_CLOUD_KEEP_ALIVE
/**
 * Save start cloud keep alive parameters
 *
 * \param[in] cloud_keep_alive:    cloud keep alive information
 * \param[in] src_port: Source port
 * \param[in] dst_port: Destination port
 * \param[in] seq_number: Sequence number
 * \param[in] ack_number: Acknowledgement number
 * \param[in] enable: Enable
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_save_cloud_keep_alive_params(wlan_cloud_keep_alive_t *cloud_keep_alive,
                                      t_u16 src_port,
                                      t_u16 dst_port,
                                      t_u32 seq_number,
                                      t_u32 ack_number,
                                      t_u8 enable);

/**
 * Get cloud keep alive status for given destination ip and port
 *
 * \param[in] dst_ip: Destination ip address
 * \param[in] dst_port: Destination port
 *
 * \return 1 if enabled otherwise 0.
 */
int wlan_cloud_keep_alive_enabled(t_u32 dst_ip, t_u16 dst_port);

/**
 * Start cloud keep alive
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_start_cloud_keep_alive(void);
/**
 * Stop cloud keep alive
 * \param[in] cloud_keep_alive:    cloud keep alive information
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_stop_cloud_keep_alive(wlan_cloud_keep_alive_t *cloud_keep_alive);
#endif

/**
 * Set country code
 *
 * \note This API should be called after Wi-Fi is initialized
 * but before starting uAP interface.
 *
 * \param[in] alpha2: country code in 3 octets string, 2 octets country code and 1 octet environment
 *            2 octets country code supported:
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
 * For the third octet, STA is always 0.
 * for uAP environment:
 * All environments of the current frequency band and country (default)
 * alpha2[2]=0x20
 * Outdoor environment only
 * alpha2[2]=0x4f
 * Indoor environment only
 * alpha2[2]=0x49
 * Noncountry entity (country_code=XX)
 * alpha[2]=0x58
 * IEEE 802.11 standard Annex E table indication: 0x01 .. 0x1f
 * Annex E, Table E-4 (Global operating classes)
 * alpha[2]=0x04
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_country_code(const char *alpha2);

/** Set ignore region code.
 *
 * \param[in] ignore:     0: don't ignore, 1: ignore
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_country_ie_ignore(uint8_t *ignore);

/** Set region code.
 *
 * \param[in] region_code: region code to be set.
 * \return WM_SUCCESS if successful otherwise fail.
 */
int wlan_set_region_code(unsigned int region_code);

/** Get region code.
 *
 * \param[out] region_code: pointer
 *             The value:
 *             0x00: World Wide Safe
 *             0x10: US FCC
 *             0x20: IC Canada
 *             0x10: Singapore
 *             0x30: ETSI
 *             0x30: Australia
 *             0x30: Republic Of Korea
 *             0x32: France
 *             0xFF: Japan
 *             0x50: China
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_region_code(unsigned int *region_code);

/** Set STA/uAP 802.11d feature Enable/Disable.
 *
 * \param[in] bss_type: 0: STA, 1: uAP
 * \param[in] state:    0: disable, 1: enable
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_11d_state(int bss_type, int state);

#if CONFIG_COEX_DUTY_CYCLE
/**
 * Set single antenna: duty cycle.
 * \param[in] enable: enable/disable single duty cycle\n
 *            0: Disable\n
 *            1: enable
 * \param[in] nbTime: time in unit 1ms, no more than wlanTime
 * \param[in] wlanTime: time in unit 1ms, total duty cycle time
 * \note wlanTime should not equal to wlanTime-nbTime
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_single_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime);

/**
 * Set dual antenna duty cycle.
 * \param[in] enable: enable/disable single duty cycle\n
 *            0: Disable\n
 *            1: enable
 * \param[in] nbTime: time in units 1ms, no more than wlanTime
 * \param[in] wlanTime: time in unit 1ms, total duty cycle time
 * \param[in] wlanBlockTime: time in unit 1ms
 * \note nbTime, wlanTime and wlanBlockTime should not equal to each other
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dual_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime, t_u16 wlanBlockTime);
#endif

#if CONFIG_EXTERNAL_COEX_PTA
/**
 * Set external coex PTA (packet traffic arbitration) parameters.
 * \param[in] coex_pta_config: \ref ext_coex_pta_cfg
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_external_coex_pta_cfg(ext_coex_pta_cfg coex_pta_config);
#endif

#if CONFIG_WPA_SUPP_DPP
/** Add a DPP (device provisioning protocol) configurator.
 *
 *  If this device is DPP configurator, add it to get configurator ID.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "curve=P-256"
 *
 * \return configurator ID if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_configurator_add(int is_ap, const char *cmd);

/** Set DPP (device provisioning protocol) configurator parameter
 *
 *  set DPP configurator params.
 *  for example:" conf=<sta-dpp/ap-dpp> ssid=<hex ssid> configurator=conf_id"
 *  #space character exists between " & conf word.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     " conf=<sta-dpp/ap-dpp/sta-psk> ssid=<hex ssid> configurator=conf_id..."
 *
 * \return void
 */
void wlan_dpp_configurator_params(int is_ap, const char *cmd);

/** MUD URL for enrollee's DPP configuration request (optional)
 *
 *  Wi-Fi_CERTIFIED_Easy_Connect_Test_Plan_v3.0.pdf
 *  5.1.23 STAUT sends the MUD URL
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "https://example.com/mud"
 *
 * \return void
 */
void wlan_dpp_mud_url(int is_ap, const char *cmd);

/** Generate QR code.
 *
 *  This function generates QR code and return bootstrap-id
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "type=qrcode mac=<mac-address-of-device> chan=<operating-class/channel>..."
 *
 * \return bootstrap-id if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_bootstrap_gen(int is_ap, const char *cmd);

/** Get QR code by bootstrap-id.
 *
 *  This function gets QR code string by bootstrap-id
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  id:      bootstrap-id
 *
 * \return QR code string if successful otherwise NULL.
 */
const char *wlan_dpp_bootstrap_get_uri(int is_ap, unsigned int id);

/** Enter the QR code in the DPP device.
 *
 *  This function set the QR code and return qr-code-id.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP
 * \param[in]  uri:     QR code provided by other device.
 *
 * \return qr-code-id if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_qr_code(int is_ap, char *uri);

/** Send provisioning auth request to responder.
 *
 *  This function send Auth request to responder by qr-code-id.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     " peer=<qr-code-id> conf=<sta-dpp/ap-dpp/sta-psk> ...."
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_auth_init(int is_ap, const char *cmd);

/** Make device listen to DPP request.
 *
 *  Responder generates QR code and listening on its operating channel to wait Auth request.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "<frequency>"
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_listen(int is_ap, const char *cmd);

/** DPP stop listen.
 *
 *  Stop dpp listen and clear listen frequency
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_stop_listen(int is_ap);

/** Set bootstrapping through PKEX (Public Key Exchange).
 *
 *  Support in-band bootstrapping through PKEX
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "own=<bootstrap_id> identifier=<string> code=<string>"
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_pkex_add(int is_ap, const char *cmd);

/** sends DPP presence announcement.
 *
 *  Send DPP presence announcement from responder.
 *  After the Initiator enters the QRcode URI provided by the Responder,
 *  the Responder sends the presence announcement to trigger Auth Request from Initiator.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP.
 * \param[in]  cmd:     "own=<bootstrap id> listen=<freq> ..."
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_chirp(int is_ap, const char *cmd);

/** DPP reconfig.
 *
 *  DPP reconfig and make a new DPP connection.
 *
 * \param[in]  cmd:     "<network id> ..."
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_reconfig(const char *cmd);

/** Configurator configures itself as an Enrollee AP/STA.
 *
 *  Wi-Fi_CERTIFIED_Easy_Connect_Test_Plan_v3.0.pdf
 *  5.3.8 & 5.3.9 Configurator configures itself as an Enrollee AP/STA
 *
 *  for example:" conf=<sta-dpp/ap-dpp> ssid=<hex ssid> configurator=conf_id"
 *  #space character exists between " & conf word.
 *
 * \param[in]  is_ap:   0 is STA, 1 is uAP
 * \param[in]  cmd:     " conf=<sta-dpp/ap-dpp/sta-psk> ssid=<hex ssid> configurator=conf_id..."
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_dpp_configurator_sign(int is_ap, const char *cmd);
#endif

#if CONFIG_IMD3_CFG
/**
 * Set imd validation parameters.
 * \param[in] imd3_value:  disable imd3: imd3_value = 0;
 *                         enable imd3: low 4 bits: enable, high 4 bits: isolation index.
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_imd3_cfg(t_u8 imd3_value);
#endif

#if CONFIG_UAP_STA_MAC_ADDR_FILTER
int wlan_host_set_sta_mac_filter(int filter_mode, int mac_count, unsigned char *mac_addr);
#endif

#if (CONFIG_WIFI_IND_RESET) && (CONFIG_WIFI_IND_DNLD)
/**
 * Set GPIO independent reset configuration
 *
 * \param[in] indrst_cfg: GPIO independent reset configuration to be sent to firmware
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_set_indrst_cfg(const wifi_indrst_cfg_t *indrst_cfg);

/* Get GPIO independent reset configuration
 *
 * \param[out] indrst_cfg: GPIO independent reset configuration set in Firmware
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_get_indrst_cfg(wifi_indrst_cfg_t *indrst_cfg);

/** Test independent firmware reset
 *
 * This function can either send command that can cause timeout in firmware or
 * send GPIO pulse that can cause out of band reset in firmware as per configuration
 * int earlier \ref wlan_set_indrst_cfg API.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_independent_reset(void);

#endif

int wlan_set_network_ip_byname(char *name, struct wlan_ip_config *ip);

#if CONFIG_INACTIVITY_TIMEOUT_EXT
/**
 * Get/Set inactivity timeout extend
 * \param[in] inac_to: \ref wlan_inactivity_to_t
 * \param[in] action: \n
 *            0: get\n
 *            1: set
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_sta_inactivityto(wlan_inactivity_to_t *inac_to, t_u16 action);
#endif

/**
 * Get 802.11 Status Code.
 *
 * \param[in] reason: wlcmgr event reason
 *
 * \return status code defined in IEEE 802.11-2020 standard.
 */
t_u16 wlan_get_status_code(enum wlan_event_reason reason);

#ifdef RW610
/**
 * Get board temperature.
 * \return board temperature.
 */
int32_t wlan_get_temperature(void);
#endif

#if CONFIG_CPU_LOADING
/**
 * Set parameters for cpu loading test.
 *
 * \param[in]  start:   0 stop test, 1 start test.
 * \param[in]  number:  The number of cpu loading test.
 * \param[in]  period:  The period of cpu loading test.
 *
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_cpu_loading(uint8_t start, uint32_t number, uint8_t period);
#endif

#if CONFIG_AUTO_NULL_TX
/** Configuration for auto null TX parameters from
 * \ref wifi_auto_null_tx_t
 */
typedef wifi_auto_null_tx_t wlan_auto_null_tx_t;

/**
 * Start/Stop auto TX null.
 * Call this API to auto transmit and one shot quality of service data packets
 * to get the CSI after STA connected one AP or uAP was connected with external STA.
 * \note STA cannot send auto NULL data if not connected AP, not support auto TX
 * without connecting AP. uAP cannot send auto NULL data if is not connected,
 * not support auto tx without connecting with external STA.
 *
 * \param[in]  auto_null_tx: auto null RX information
 * \param[in]  bss_type:     0: station; 1: uAP
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_auto_null_tx(wlan_auto_null_tx_t *auto_null_tx, mlan_bss_type bss_type);
#endif

/**
 * Allocate memory for a string and copy the string to the allocated memory
 *
 * \param[in] s: the source/target string
 *
 * \return new string if successful, otherwise return -WM_FAIL.
 */
char *wlan_string_dup(const char *s);

/**
 * Get board type.
 *
 * \return board type.
 *         0x02: RW610_PACKAGE_TYPE_BGA
 *         0xFF: others
 */
uint32_t wlan_get_board_type(void);

#if UAP_SUPPORT
/**
 * Disconnect to STA which is connected with internal uAP.
 *
 * \param[in]  sta_addr:   STA MAC address
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_uap_disconnect_sta(uint8_t *sta_addr);
#endif

/**
 * Check if 802.11n is allowed in capability.
 *
 * \param[in]  network:   A pointer to the \ref wlan_network
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_11n_allowed(struct wlan_network *network);

#if CONFIG_11AC
/**
 * Check if 802.11ac is allowed in capability.
 *
 * \param[in]  network:   A pointer to the \ref wlan_network
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_11ac_allowed(struct wlan_network *network);
#endif

#if CONFIG_11AX
/**
 * Check if 802.11ax is allowed in capability.
 *
 * \param[in]  network:   A pointer to the \ref wlan_network
 * \return WM_SUCCESS if successful otherwise return -WM_FAIL.
 */
int wlan_11ax_allowed(struct wlan_network *network);
#endif
#endif /* __WLAN_H__ */
