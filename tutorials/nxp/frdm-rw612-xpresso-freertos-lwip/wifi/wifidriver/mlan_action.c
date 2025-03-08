/** @file mlan_action.c
 *
 *  @brief  This file provides functions for action management frame
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

#include <mlan_api.h>
#include "wifi.h"
#ifdef RW610
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif

/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/
/**
 *  @brief This function process rx radio measurement action frame
 *
 *  @param payload      rx frame including 802.11 header
 *  @param payload_len  length of action frame
 *  @param src_addr     source address
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_process_mgmt_radio_measurement_action(
    t_u8 *payload, t_u32 payload_len, t_u8 *dest_addr, t_u8 *src_addr, RxPD *rxpd)
{
    t_u8 action_code = 0;
    t_u8 *pos;
    mlan_status ret = MLAN_STATUS_FAILURE;

    pos         = payload + sizeof(wlan_802_11_header) + 1;
    action_code = *pos++;
#if CONFIG_11K
    IEEEtypes_FrameCtl_t *mgmt_fc_p =
        (IEEEtypes_FrameCtl_t *)(void *)&(((wlan_802_11_header *)(void *)payload)->frm_ctl);
    payload_len -= (sizeof(wlan_802_11_header) + 2U);
#endif

    switch (action_code)
    {
#if CONFIG_11K
        case (t_u8)IEEE_MGMT_RRM_RADIO_MEASUREMENT_REQUEST:
        {
            wlan_process_radio_measurement_request(pos, payload_len, dest_addr, src_addr, (bool)mgmt_fc_p->wep);
            ret = MLAN_STATUS_SUCCESS;
            break;
        }
        case (t_u8)IEEE_MGMT_RRM_LINK_MEASUREMENT_REQUEST:
        {
            wlan_process_link_measurement_request(pos, payload_len, dest_addr, src_addr, (bool)mgmt_fc_p->wep, rxpd);
            ret = MLAN_STATUS_SUCCESS;
            break;
        }
        case (t_u8)IEEE_MGMT_RRM_NEIGHBOR_REPORT_RESPONSE:
        {
            wlan_process_neighbor_report_response(pos, payload_len, dest_addr, src_addr, (bool)mgmt_fc_p->wep);
            ret = MLAN_STATUS_SUCCESS;
            break;
        }
#else
        case 0:
            break;
#endif
        default:
            wifi_d("RRM: Unknown request: %u", action_code);
            break;
    }
    return ret;
}

/**
 *  @brief This function process rx action frame
 *
 *  @param payload      rx frame including 802.11 header
 *  @param payload_len  length of action frame
 *  @param src_addr     source address
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
static mlan_status wlan_process_mgmt_wnm_action(t_u8 *payload, t_u32 payload_len, t_u8 *src_addr, t_u8 *dest_addr)
{
    IEEEtypes_WNM_ActionFieldType_e action_code = (IEEEtypes_WNM_ActionFieldType_e)0;
    t_u8 *pos;
    mlan_status ret = MLAN_STATUS_FAILURE;

    pos         = payload + sizeof(wlan_802_11_header) + 1;
    action_code = (IEEEtypes_WNM_ActionFieldType_e)(*pos++);

    switch (action_code)
    {
#if CONFIG_11V
        case IEEE_MGMT_WNM_BTM_REQUEST:
        {
            IEEEtypes_FrameCtl_t *mgmt_fc_p =
                (IEEEtypes_FrameCtl_t *)(void *)&(((wlan_802_11_header *)(void *)payload)->frm_ctl);

            wlan_process_mgmt_wnm_btm_req(pos, (payload + payload_len), src_addr, dest_addr, (bool)mgmt_fc_p->wep);
            ret = MLAN_STATUS_SUCCESS;
            break;
        }
#endif
        default:
            wifi_d("WNM: Unknown request: %u", action_code);
            break;
    }
    return ret;
}

static mlan_status wlan_process_mgmt_unprotect_wnm_action(t_u8 *payload, t_u32 payload_len, RxPD *rxpd)
{
    t_u8 action_code = 0;
    t_u8 *pos;
    mlan_status ret = MLAN_STATUS_FAILURE;

    pos         = payload + sizeof(wlan_802_11_header) + 1;
    action_code = *(pos++);

    switch (action_code)
    {
        case 1:
            break;
        default:
            wifi_d("unprotect WNM: Unknown request: %u", action_code);
            break;
    }
    return ret;
}


/********************************************************
                Global functions
********************************************************/
/**
 *  @brief This function process rx action frame
 *
 *  @param payload      rx frame including 802.11 header
 *  @param payload_len  length of action frame
 *
 *  @return             MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_process_mgmt_action(t_u8 *payload, t_u32 payload_len, RxPD *rxpd)
{
    wlan_802_11_header *pieee_pkt_hdr   = MNULL;
    IEEEtypes_ActionCategory_e category = (IEEEtypes_ActionCategory_e)0;
    mlan_status ret                     = MLAN_STATUS_FAILURE;

    pieee_pkt_hdr = (wlan_802_11_header *)(void *)payload;
    category      = (IEEEtypes_ActionCategory_e)(*(payload + sizeof(wlan_802_11_header)));

    switch (category)
    {
        case IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC:
            ret = wlan_process_mgmt_radio_measurement_action(payload, payload_len, pieee_pkt_hdr->addr1,
                                                             pieee_pkt_hdr->addr2, rxpd);
            break;
        case IEEE_MGMT_ACTION_CATEGORY_WNM:
            ret = wlan_process_mgmt_wnm_action(payload, payload_len, pieee_pkt_hdr->addr1, pieee_pkt_hdr->addr2);
            break;
        case IEEE_MGMT_ACTION_CATEGORY_UNPROTECT_WNM:
            ret = wlan_process_mgmt_unprotect_wnm_action(payload, payload_len, rxpd);
            break;
        default:
            wifi_d("Action: Unknown request: %u", category);
            break;
    }
    return ret;
}
