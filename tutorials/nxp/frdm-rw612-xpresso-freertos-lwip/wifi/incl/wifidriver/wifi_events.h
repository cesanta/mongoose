/*
 *  Copyright 2008-2020, 2023-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*! \file wifi_events.h
 * \brief This file provides Wi-Fi driver event enum.
 */

#ifndef __WIFI_EVENTS_H__
#define __WIFI_EVENTS_H__

#include <osa.h>

/** Wi-Fi events */
enum wifi_event
{
    /** uAP Started */
    WIFI_EVENT_UAP_STARTED = 0,
    /** uAP Client Assoc */
    WIFI_EVENT_UAP_CLIENT_ASSOC,
    /** uAP Client connected */
    WIFI_EVENT_UAP_CLIENT_CONN,
    /** uAP Client De-authentication */
    WIFI_EVENT_UAP_CLIENT_DEAUTH,
    /** uAP Network Address Configuration */
    WIFI_EVENT_UAP_NET_ADDR_CONFIG,
    /** uAP Stopped */
    WIFI_EVENT_UAP_STOPPED,
    /** uAP TX Data Pause */
    WIFI_EVENT_UAP_TX_DATA_PAUSE,
    /** uAP Last */
    WIFI_EVENT_UAP_LAST,
    /* All the uAP related events need to be above and STA related events
     * below */
    /** Scan start event when scan is started */
    WIFI_EVENT_SCAN_START,
    /** Scan Result */
    WIFI_EVENT_SCAN_RESULT,
    /** Survey Result Get */
    WIFI_EVENT_SURVEY_RESULT_GET,
    /** Get hardware spec */
    WIFI_EVENT_GET_HW_SPEC,
    /** Association */
    WIFI_EVENT_ASSOCIATION,
#if CONFIG_WPA_SUPP
#if CONFIG_AUTO_RECONNECT
    /** Association Notify */
    WIFI_EVENT_ASSOCIATION_NOTIFY,
#endif
#if CONFIG_HOSTAPD
    WIFI_EVENT_ACS_COMPLETE,
#endif
#endif
    /** PMK */
    WIFI_EVENT_PMK,
    /** Authentication */
    WIFI_EVENT_AUTHENTICATION,
    /** Disassociation */
    WIFI_EVENT_DISASSOCIATION,
    /** De-authentication */
    WIFI_EVENT_DEAUTHENTICATION,
    /** Link Loss */
    WIFI_EVENT_LINK_LOSS,
    /* Wi-Fi RSSI Low Event */
    WIFI_EVENT_RSSI_LOW,
    /** Firmware Hang event */
    WIFI_EVENT_FW_HANG,
    /** Firmware Reset event */
    WIFI_EVENT_FW_RESET,
#if CONFIG_SUBSCRIBE_EVENT_SUPPORT
    /* Wi-Fi RSSI High Event */
    WIFI_EVENT_RSSI_HIGH,
    /* Wi-Fi SRN Low Event */
    WIFI_EVENT_SNR_LOW,
    /* Wi-Fi SNR High Event */
    WIFI_EVENT_SNR_HIGH,
    /* Wi-Fi Max Fail Event */
    WIFI_EVENT_MAX_FAIL,
    /* Wi-Fi Beacon miised Event */
    WIFI_EVENT_BEACON_MISSED,
    /* Wi-Fi Data RSSI Low Event */
    WIFI_EVENT_DATA_RSSI_LOW,
    /* Wi-Fi Data RSSI High Event */
    WIFI_EVENT_DATA_RSSI_HIGH,
    /* Wi-Fi Data SNR Low Event */
    WIFI_EVENT_DATA_SNR_LOW,
    /* Wi-Fi Data SNR High Event */
    WIFI_EVENT_DATA_SNR_HIGH,
    /* Wi-Fi Link Quality Event */
    WIFI_EVENT_FW_LINK_QUALITY,
    /* Wi-Fi Pre Beacon Lost Event */
    WIFI_EVENT_FW_PRE_BCN_LOST,
#endif
#if CONFIG_HOST_SLEEP
    /* Host sleep activated */
    WIFI_EVENT_HS_ACTIVATED,
    /** HS configuration */
    WIFI_EVENT_HS_CONFIG,
    /* Sleep confirm done */
    WIFI_EVENT_SLEEP_CONFIRM_DONE,
#endif
    /** Network station address configuration */
    WIFI_EVENT_NET_STA_ADDR_CONFIG,
    /** Network interface configuration */
    WIFI_EVENT_NET_INTERFACE_CONFIG,
    /** WEP configuration */
    WIFI_EVENT_WEP_CONFIG,
    /** STA MAC address configuration */
    WIFI_EVENT_STA_MAC_ADDR_CONFIG,
    /** UAP MAC address configuration */
    WIFI_EVENT_UAP_MAC_ADDR_CONFIG,
    /** Network DHCP configuration */
    WIFI_EVENT_NET_DHCP_CONFIG,
    /** Supplicant PMK */
    WIFI_EVENT_SUPPLICANT_PMK,
    /** Sleep */
    WIFI_EVENT_SLEEP,
    /** IEEE PS */
    WIFI_EVENT_IEEE_PS,
    /** Deep Sleep */
    WIFI_EVENT_DEEP_SLEEP,
    /** WNM ps */
    WIFI_EVENT_WNM_PS,
    /** IEEE and Deep Sleep */
    WIFI_EVENT_IEEE_DEEP_SLEEP,
    /** WNM and Deep Sleep */
    WIFI_EVENT_WNM_DEEP_SLEEP,
    /** PS Invalid */
    WIFI_EVENT_PS_INVALID,
    /** Error Multicast */
    WIFI_EVENT_ERR_MULTICAST,
    /** error Unicast */
    WIFI_EVENT_ERR_UNICAST,
    /** 802.11K/11V neighbor report */
    WIFI_EVENT_NLIST_REPORT,
    /* Add Block Ack */
    /** 802.11N send add block ack */
    WIFI_EVENT_11N_SEND_ADDBA,
    /** 802.11N receive add block ack */
    WIFI_EVENT_11N_RECV_ADDBA,
    /** 802.11N block Ack stream timeout */
    WIFI_EVENT_11N_BA_STREAM_TIMEOUT,
    /** 802.11n Delete block add */
    WIFI_EVENT_11N_DELBA,
    /** 802.11n aggregation control */
    WIFI_EVENT_11N_AGGR_CTRL,
    /** Channel Switch Announcement */
    WIFI_EVENT_CHAN_SWITCH_ANN,
    /** Channel Switch */
    WIFI_EVENT_CHAN_SWITCH,
#if CONFIG_IPV6
    /** IPv6 address state change */
    WIFI_EVENT_NET_IPV6_CONFIG,
#endif
    /* Background Scan Report */
    WIFI_EVENT_BG_SCAN_REPORT,
    /* Background Scan Stop */
    WIFI_EVENT_BG_SCAN_STOPPED,
    /* Event to indicate RX Management Frame */
    WIFI_EVENT_MGMT_FRAME,
    /* Event to indicate remain on channel started */
    WIFI_EVENT_REMAIN_ON_CHANNEL,
    /* Event to indicate Management tx status */
    WIFI_EVENT_MGMT_TX_STATUS,
#if CONFIG_CSI
    /* Recv csi data */
    WIFI_EVENT_CSI,
    WIFI_EVENT_CSI_STATUS,
#endif
#if (CONFIG_11MC) || (CONFIG_11AZ)
    /* Event to trigger or stop ftm*/
    WIFI_EVENT_FTM_COMPLETE,
#if CONFIG_WLS_CSI_PROC
    WIFI_EVENT_WLS_CSI,
#endif
#endif
    /** Event to set region power*/
    WIFI_EVENT_REGION_POWER_CFG,
    /** TX Data Pause */
    WIFI_EVENT_TX_DATA_PAUSE,
    /** Event to indicate end of Wi-Fi events */
    WIFI_EVENT_LAST,
    /* other events can be added after this, however this must
       be the last event in the Wi-Fi module */
};

/** Wi-Fi Event Reason */
enum wifi_event_reason
{
    /** Success */
    WIFI_EVENT_REASON_SUCCESS,
    /** Timeout */
    WIFI_EVENT_REASON_TIMEOUT,
    /** Failure */
    WIFI_EVENT_REASON_FAILURE,
};

/** Network wireless BSS Type */
enum wlan_bss_type
{
    /** Station */
    WLAN_BSS_TYPE_STA = 0,
    /** uAP */
    WLAN_BSS_TYPE_UAP = 1,
    /** Any */
    WLAN_BSS_TYPE_ANY = 0xff,
};

/** Network wireless BSS Role */
enum wlan_bss_role
{
    /** Infrastructure network. The system will act as a station connected
     *  to an Access Point. */
    WLAN_BSS_ROLE_STA = 0,
    /** uAP (micro-AP) network.  The system will act as an uAP node to
     * which other Wireless clients can connect. */
    WLAN_BSS_ROLE_UAP = 1,
    /** Either Infrastructure network or micro-AP network */
    WLAN_BSS_ROLE_ANY = 0xff,
};

/** This enum defines various wakeup events
 * for which wakeup will occur */
enum wifi_wakeup_event_t
{
    /** Wakeup on broadcast  */
    WIFI_WAKE_ON_ALL_BROADCAST = 1,
    /** Wakeup on unicast  */
    WIFI_WAKE_ON_UNICAST = 1 << 1,
    /** Wakeup on MAC event  */
    WIFI_WAKE_ON_MAC_EVENT = 1 << 2,
    /** Wakeup on multicast  */
    WIFI_WAKE_ON_MULTICAST = 1 << 3,
    /** Wakeup on ARP broadcast  */
    WIFI_WAKE_ON_ARP_BROADCAST = 1 << 4,
    /** Wakeup on receiving a management frame  */
    WIFI_WAKE_ON_MGMT_FRAME = 1 << 6,
};

#endif /*__WIFI_EVENTS_H__*/
