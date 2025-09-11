/*
 *  Copyright 2008-2020, 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*!\file wmstats.h
 *\brief This file provides state statistics for Wi-Fi connection manager.
 */

#ifndef _WMSTATS_H_
#define _WMSTATS_H_

typedef enum
{
    CONN_STATS,
    DHCP_STATS,
    HTTP_CLIENT_STATS,
    CLOUD_CUMUL_STATS,
    CLOUD_STATS,
    HTTPD_STATS,
    NET_STATS,
    IP_ADDR,
    TIME,
    PROV_TYPE,
    WD_STATUS,
} wm_stat_type_t;

/*! Define return values */
typedef enum
{
    /** */
    REASON_ADV_SETTINGS_DONE,
    /** */
    REASON_NW_ADD_FAIL,
    /** */
    REASON_NW_FAILURE,
    /** */
    REASON_SWITCH_TO_PROV,
    /** */
    REASON_AUTH_FAILURES,
    /** */
    REASON_USER_REBOOT,
    /** */
    REASON_CLOUD_WATCHDOG,
    /** */
    REASON_CLOUD_FW_UPDATE,
    /** */
    REASON_WLAN_FW_UNRESPONSIVE,
} wm_reboot_reason_t;

struct wm_stats
{
    unsigned short wm_lloss;
    /** The number of connection attempts.
     * wm_conn_att == wm_conn_succ + wm_conn_fail
     */
    unsigned short wm_conn_att;
    /** The connection success indicates successful association+authentication
        attempt. This does NOT include the address configuration step */
    unsigned short wm_conn_succ;
    /** This should be split up into multiple types of failures
     * wm_conn_fail = wm_auth_fail + wm_nwnt_found + others
     */
    unsigned short wm_conn_fail;
    unsigned short wm_auth_fail;
    unsigned short wm_nwnt_found;
    /** DHCP Address assignment */
    unsigned short wm_dhcp_succ;
    unsigned short wm_dhcp_fail;
    /** DHCP Lease renewal */
    unsigned short wm_leas_succ;
    unsigned short wm_leas_fail;
    /** 1 if DHCP */
    char wm_addr_type;
    unsigned int wm_dhcp_req_cnt;
    /** HTTP Client */
    unsigned short wm_ht_dns_fail;
    unsigned short wm_ht_sock_fail;
    unsigned short wm_ht_conn_no_route;
    unsigned short wm_ht_conn_timeout;
    unsigned short wm_ht_conn_reset;
    unsigned short wm_ht_conn_other;
    /** Cloud Stats */
    unsigned long wm_cl_post_succ;
    unsigned long wm_cl_post_fail;
    unsigned long wm_cl_total;
    unsigned int wm_cl_cum_total;
    /** Provisioning type */
    /* Tells whether the module was in NORMAL mode, PROVISIONING mode or
     * TIMED PROVISIONING mode immediately after rebooting*/
    unsigned short wm_prov_type;

    /** Reboot reason of the device */
    wm_reboot_reason_t reboot_reason;
};

extern struct wm_stats g_wm_stats;

/** Note: This should be in sync with definition in firmware file
 * tcpip_custom.h.
 * */

#endif /* ! _WMSTATS_H_ */
