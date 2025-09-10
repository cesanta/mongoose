/** @file mlan_mbo.c
 *
 *  @brief  This file provides functions for process MBO feature
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

#include <mlan_api.h>
#include "mlan_mbo.h"

#if CONFIG_DRIVER_MBO
#define WNM_NOTIFICATION_SIZE 200U
/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/
const t_u8 mbo_oui[3]             = {0x50, 0x6f, 0x9a};
static const t_u8 mbo_oui_type[1] = {0x16};
/* MBO ATTR ID NON PREF CHAN REPORT */
static const t_u8 mbo_attr_id[1] = {0x2};
/********************************************************
                Local Functions
********************************************************/
/** Dialog Token */
static t_u8 mbo_dialog_token = 0;
/********************************************************
                Global functions
********************************************************/

/**
 * @brief This function add MBO OUI.
 *
 * @param oui A pointer to MBO OCE element structure
 *
 * @return pointer incremented to the end of the element
 */
t_u8 *wlan_add_mbo_oui(t_u8 *oui)
{
    (void)memcpy(oui, mbo_oui, sizeof(mbo_oui));
    return (oui + sizeof(mbo_oui));
}

/**
 * @brief This function add MBO OUI TYPE.
 *
 * @param oui A pointer to MBO OCE element structure
 *
 * @return void
 */
t_u8 *wlan_add_mbo_oui_type(t_u8 *oui_type)
{
    (void)memcpy(oui_type, mbo_oui_type, sizeof(mbo_oui_type));
    return (oui_type + sizeof(mbo_oui_type));
}

/**
 * @brief This function add MBO Attributes id.
 *
 * @param oui A pointer to MBO OCE element structure
 *
 * @return void
 */
t_u8 *wlan_add_mbo_attr_id(t_u8 *attr_id)
{
    (void)memcpy(attr_id, mbo_attr_id, sizeof(mbo_attr_id));
    return (attr_id + sizeof(mbo_attr_id));
}

/**
 * @brief This function add cellular data CAP attribute into MBO OCE IE.
 *
 * @param oui A pointer to MBO OCE element structure
 *
 * @return void
 */
t_u8 *wlan_add_mbo_cellular_cap(t_u8 *attrib)
{
    attrib[0] = (t_u8)MBO_CELLULAR_DATA_CAP;
    attrib[1] = 0x01;
    attrib[2] = 0x03;
    return (attrib + 3);
}

/**
 * @brief This function add prefer or non-prefer channels into MBO OCE IE.
 *
 * @param oui A pointer to MBO OCE element structure
 *
 * @return void
 */
t_u8 *wlan_add_mbo_prefer_ch(t_u8 *attrib, t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1)
{
    t_u8 oper_class      = 0;
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    attrib[0] = (t_u8)MBO_NON_PERFER_CH_REPORT;
    attrib[1] = 0x04;
    (void)wlan_get_curr_global_oper_class(pmpriv, ch0, BW_20MHZ, &oper_class);
    attrib[2] = oper_class; /*Wi-Fi CERTIFIED Agile Multiband. Test Plan v1.4 section 5.2.8. Set Global operating class
                               to this field. */
    attrib[3] = ch0;
    attrib[4] = pefer0;
    attrib[5] = 0;

    attrib[6] = (t_u8)MBO_NON_PERFER_CH_REPORT;
    attrib[7] = 0x04;
    (void)wlan_get_curr_global_oper_class(pmpriv, ch1, BW_20MHZ, &oper_class);
    attrib[8]  = oper_class;
    attrib[9]  = ch1;
    attrib[10] = pefer1;
    attrib[11] = 0;
    return (attrib + 12);
}

void wlan_send_mgmt_wnm_notification(
    t_u8 *src_addr, t_u8 *dst_addr, t_u8 *target_bssid, t_u8 *tag_nr, t_u8 tag_len, bool protect)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    IEEEtypes_FrameCtl_t *mgmt_fc_p = MNULL;
    t_u8 *pos                       = MNULL;
    t_u16 pkt_len                   = 0;
    t_u32 meas_pkt_len              = 0;

    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(
        SUBTYPE_ACTION, (mlan_802_11_mac_addr *)(void *)dst_addr, (mlan_802_11_mac_addr *)(void *)src_addr,
        (mlan_802_11_mac_addr *)(void *)dst_addr, sizeof(wlan_mgmt_pkt) + WNM_NOTIFICATION_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        PRINTM(MERROR, "No memory available for BTM resp");
        return;
    }

    mgmt_fc_p = (IEEEtypes_FrameCtl_t *)(void *)&pmgmt_pkt_hdr->wlan_header.frm_ctl;
    if (protect)
    {
        mgmt_fc_p->wep = 1;
    }

    /* 802.11 management body */
    pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    pos[0] = (t_u8)IEEE_MGMT_ACTION_CATEGORY_WNM;
    pos[1] = (t_u8)IEEE_MGMT_WNM_NOTIFICATION_REQUEST;
    pos[2] = mbo_dialog_token++;
    pos[3] = 221; /* type */
    pos += 4;
    (void)memcpy(pos, tag_nr, tag_len);
    pos += tag_len;

    meas_pkt_len           = sizeof(wlan_mgmt_pkt) + 4U + (t_u32)tag_len;
    pkt_len                = (t_u16)meas_pkt_len;
    pmgmt_pkt_hdr->frm_len = (t_u16)pkt_len - (t_u16)sizeof(t_u16);
    (void)wifi_inject_frame(WLAN_BSS_TYPE_STA, (t_u8 *)pmgmt_pkt_hdr, pkt_len);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif
}

#endif /* CONFIG_DRIVER_MBO */
