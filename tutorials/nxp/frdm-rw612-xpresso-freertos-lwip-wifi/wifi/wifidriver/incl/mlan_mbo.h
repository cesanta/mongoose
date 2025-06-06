/** @file mlan_mbo.h
 *
 *  @brief Interface for the MBO feature implemented in mlan_mbo.c
 *
 *  Driver interface functions and type declarations for the process MBO
 *    module implemented in mlan_mbo.c.
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    8/SEP/2022: initial version
********************************************************/
#ifndef _MLAN_MBO_H
#define _MLAN_MBO_H

#if CONFIG_DRIVER_MBO
/** MBO attributes ID */
enum MBO_ATTRIB_ID
{
    MBO_AP_CAP_IND = 1,
    MBO_NON_PERFER_CH_REPORT,
    MBO_CELLULAR_DATA_CAP,
    MBO_ASSOC_DISALLOWED,
    MBO_CELLULAR_DATA_CONNECT_PREFER,
    MBO_TRANSIT_REASON,
    MBO_TRANSIT_REJECCT_REASON,
    MBO_ASSOC_RETRY_DELAY,
};

extern const t_u8 mbo_oui[];

t_u8 *wlan_add_mbo_oui(t_u8 *oui);
t_u8 *wlan_add_mbo_oui_type(t_u8 *oui_type);
t_u8 *wlan_add_mbo_attr_id(t_u8 *attr_id);
t_u8 *wlan_add_mbo_cellular_cap(t_u8 *attrib);
t_u8 *wlan_add_mbo_prefer_ch(t_u8 *attrib, t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1);
void wlan_send_mgmt_wnm_notification(
    t_u8 *src_addr, t_u8 *dst_addr, t_u8 *target_bssid, t_u8 *tag_nr, t_u8 tag_len, bool protect);
#endif
#endif
