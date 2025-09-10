/** @file mlan_11v.h
 *
 *  @brief Interface for the BTM module implemented in mlan_11v.c
 *
 *  Driver interface functions and type declarations for the process BTM frame
 *    module implemented in mlan_11v.c.
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    08/11/2022: initial version
********************************************************/

#ifndef _MLAN_11V_H_
#define _MLAN_11V_H_

#if CONFIG_11V

#define WLAN_WNM_MAX_NEIGHBOR_REPORT               6
#define MGMT_WNM_NEIGHBOR_BSS_TRANSITION_CANDIDATE 3

struct wnm_neighbor_report
{
    t_u8 bssid[MLAN_MAC_ADDR_LENGTH];
    t_u32 bssid_info;
    t_u8 reg_class;
    t_u8 channel;
    t_u8 PhyType;
    t_u8 prefer;
    t_u8 prefer_select;
};

/* IEEE Std 802.11-2016 - Table 9-357 BTM status code definitions */
enum wnm_btm_status_code
{
    WNM_BTM_ACCEPT                                 = 0,
    WNM_BTM_REJECT_UNSPECIFIED                     = 1,
    WNM_BTM_REJECT_INSUFFICIENT_BEACON_PROBE_RESP  = 2,
    WNM_BTM_REJECT_INSUFFICIENT_AVAILABLE_CAPABITY = 3,
    WNM_BTM_REJECT_TERMINATION_UNDESIRED           = 4,
    WNM_BTM_REJECT_TERMINATION_DELAY_REQUEST       = 5,
    WNM_BTM_REJECT_STA_CANDIDATE_LIST_PROVIDED     = 6,
    WNM_BTM_REJECT_NO_SUITABLE_CANDIDATES          = 7,
    WNM_BTM_REJECT_LEAVING_ESS                     = 8
};

/** process rx action frame */
void wlan_process_mgmt_wnm_btm_req(t_u8 *pos, t_u8 *end, t_u8 *src_addr, t_u8 *dest_addr, bool protect);

/* send mgmt bss transition query frame */
int wlan_send_mgmt_bss_trans_query(mlan_private *pmpriv, t_u8 query_reason);

/* send mgmt bss transition mgmt response */
void wlan_send_mgmt_wnm_btm_resp(t_u8 dialog_token,
                                 enum wnm_btm_status_code status,
                                 t_u8 *dst_addr,
                                 t_u8 *src_addr,
                                 t_u8 *target_bssid,
                                 t_u8 *tag_nr,
                                 t_u8 tag_len,
                                 bool protect);
#endif /* CONFIG_11V */

#endif /* !_MLAN_11V_H_ */
