/** @file mlan_11v.c
 *
 *  @brief  This file provides functions for process 11v(BTM) frames
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
#ifndef RW610
#include "wifi-sdio.h"
#endif
#if CONFIG_11V
#define BTM_RESP_BUF_SIZE      200
#define WNM_BTM_QUERY_BUF_SIZE 10U
#define WLAN_FC_TYPE_MGMT      0

/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/
static void wlan_wnm_parse_neighbor_report(t_u8 *pos, t_u8 len, struct wnm_neighbor_report *rep)
{
    t_u8 remain_len = 0;
    if (len < (t_u8)13U)
    {
        wifi_d("WNM: This neighbor report is too short");
    }

    (void)memcpy(rep->bssid, pos, MLAN_MAC_ADDR_LENGTH);
    rep->bssid_info = wlan_cpu_to_le32(*(t_u32 *)(void *)(pos + MLAN_MAC_ADDR_LENGTH));
    rep->reg_class  = *(pos + 10);
    rep->channel    = *(pos + 11);
    rep->PhyType    = *(pos + 12);
    pos += 13;
    remain_len = (t_u8)(len - (t_u8)13U);

    while (remain_len >= (t_u8)2U)
    {
        t_u8 e_id, e_len;

        e_id  = *pos++;
        e_len = *pos++;
        remain_len -= (t_u8)2U;
        if (e_len > remain_len)
        {
            wifi_d("WNM: neighbor report length not matched");
            break;
        }
        switch (e_id)
        {
            case MGMT_WNM_NEIGHBOR_BSS_TRANSITION_CANDIDATE:
                if (e_len < (t_u8)1U)
                {
                    break;
                }
                rep->prefer        = pos[0];
                rep->prefer_select = 1;
                break;
            default:
                (void)PRINTF("UNKNOWN nbor Report e id\r\n");
                break;
        }

        remain_len -= e_len;
        pos += e_len;
    }
}

void wlan_send_mgmt_wnm_btm_resp(t_u8 dialog_token,
                                 enum wnm_btm_status_code status,
                                 t_u8 *dst_addr,
                                 t_u8 *src_addr,
                                 t_u8 *target_bssid,
                                 t_u8 *tag_nr,
                                 t_u8 tag_len,
                                 bool protect)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    IEEEtypes_FrameCtl_t *mgmt_fc_p = MNULL;
    t_u8 *pos                       = MNULL;
    t_u16 pkt_len                   = 0;

    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(
        SUBTYPE_ACTION, (mlan_802_11_mac_addr *)(void *)dst_addr, (mlan_802_11_mac_addr *)(void *)src_addr,
        (mlan_802_11_mac_addr *)(void *)dst_addr, sizeof(wlan_mgmt_pkt) + (size_t)BTM_RESP_BUF_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_d("No memory available for BTM resp");
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
    pos[1] = (t_u8)IEEE_MGMT_WNM_BTM_RESPONSE;
    pos[2] = dialog_token;
    pos[3] = (t_u8)status;
    pos[4] = 0; /* delay */
    pos += 5;

    if (target_bssid != NULL)
    {
        (void)memcpy((void *)pos, (const void *)target_bssid, (size_t)MLAN_MAC_ADDR_LENGTH);
        pos += 6;
    }
    else if (status == WNM_BTM_ACCEPT)
    {
        (void)memcpy((void *)pos, "\0\0\0\0\0\0", (size_t)MLAN_MAC_ADDR_LENGTH);
        pos += 6;
    }
    else
    {
        /* Do nothing */
    }

    if (status == WNM_BTM_ACCEPT && tag_nr != NULL)
    {
        (void)memcpy((void *)pos, (const void *)tag_nr, (size_t)tag_len);
        pos += tag_len;
    }
    pkt_len                = (t_u16)(pos - (t_u8 *)pmgmt_pkt_hdr);
    pmgmt_pkt_hdr->frm_len = (t_u16)((t_u16)pkt_len - sizeof(t_u16));
    (void)wifi_inject_frame(WLAN_BSS_TYPE_STA, (t_u8 *)pmgmt_pkt_hdr, pkt_len);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif
}

static bool wlan_11v_find_in_channels(t_u8 *channels, t_u8 entry_num, t_u8 chan)
{
    t_u8 i;
    for (i = 0; i < entry_num; i++)
    {
        if (channels[i] == chan)
        {
            return true;
        }
    }
    return false;
}
/********************************************************
                Global functions
********************************************************/
/**
 *  @brief This function process BTM request frame
 *
 *  @param pos          BTM request frame head
 *  @param end          end of frame
 *  @param src_addr     source address
 *
 */
void wlan_process_mgmt_wnm_btm_req(t_u8 *pos, t_u8 *end, t_u8 *src_addr, t_u8 *dest_addr, bool protect)
{
    t_u8 dialog_token;
    t_u8 wnm_num_neighbor_report = 0, neighbor_index = 0;
    t_u8 btm_mode;
    t_u8 prefer_old = 0, prefer_select = 0;
    t_u8 *ptagnr                              = NULL;
    t_u8 tagnr_len                            = 0;
    wlan_nlist_report_param *pnlist_rep_param = MNULL;
    t_u8 entry_num                            = 0;

    if (end - pos < 5)
    {
        return;
    }

#if !CONFIG_MEM_POOLS
    pnlist_rep_param = (wlan_nlist_report_param *)OSA_MemoryAllocate(sizeof(wlan_nlist_report_param));
#else
    pnlist_rep_param = (wlan_nlist_report_param *)OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif

    if (pnlist_rep_param == MNULL)
    {
        wifi_e("11v nlist report param buffer alloc failed %d", sizeof(wlan_nlist_report_param));
        return;
    }

    (void)memset(pnlist_rep_param, 0, sizeof(wlan_nlist_report_param));

    dialog_token = pos[0];
    btm_mode     = pos[1];
    pos += 5;

    if ((btm_mode & IEEE_WNM_BTM_REQUEST_BSS_TERMINATION_INCLUDED) != 0U)
    {
        pos += 12; /* BSS Termination Duration */
    }

    if ((btm_mode & IEEE_WNM_BTM_REQUEST_PREFERENCE_CAND_LIST_INCLUDED) != 0U)
    {
#if !CONFIG_MEM_POOLS
        struct wnm_neighbor_report *preport =
            OSA_MemoryAllocate((size_t)WLAN_WNM_MAX_NEIGHBOR_REPORT * sizeof(struct wnm_neighbor_report));
#else
        struct wnm_neighbor_report *preport = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif
        if (preport == NULL)
        {
            wifi_e("No memory available for neighbor report.");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree((void *)pnlist_rep_param);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
            return;
        }

        while (end - pos >= 2 && wnm_num_neighbor_report < (t_u8)WLAN_WNM_MAX_NEIGHBOR_REPORT)
        {
            t_u8 tag = *pos++;
            t_u8 len = *pos++;

            if ((int)len > (end - pos))
            {
                wifi_d("WNM: Truncated BTM request");
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree((void *)preport);
                OSA_MemoryFree((void *)pnlist_rep_param);
#else
                OSA_MemoryPoolFree(buf_128_MemoryPool, preport);
                OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
                return;
            }

            if (tag == (t_u8)NEIGHBOR_REPORT)
            {
                struct wnm_neighbor_report *rep;
                rep = &preport[wnm_num_neighbor_report];
                wlan_wnm_parse_neighbor_report(pos, len, rep);
                if (!wlan_11v_find_in_channels(pnlist_rep_param->channels, entry_num, rep->channel))
                {
                    pnlist_rep_param->channels[entry_num] = rep->channel;
                    entry_num++;
                }
                if (rep->prefer_select != (t_u8)0U && (rep->prefer > prefer_old))
                {
                    ptagnr         = pos - 2;
                    tagnr_len      = len + (t_u8)2U;
                    prefer_old     = (t_u8)rep->prefer;
                    prefer_select  = 1;
                    neighbor_index = wnm_num_neighbor_report;
                }
                wnm_num_neighbor_report++;
            }
            pos += len;
        }

        if (wnm_num_neighbor_report == (t_u8)0U)
        {
            wlan_send_mgmt_wnm_btm_resp(dialog_token, WNM_BTM_REJECT_NO_SUITABLE_CANDIDATES, dest_addr, src_addr, NULL,
                                        ptagnr, tagnr_len, protect);
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree((void *)preport);
            OSA_MemoryFree((void *)pnlist_rep_param);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, preport);
            OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif

            return;
        }

        if (prefer_select == (t_u8)1U)
        {
            wlan_send_mgmt_wnm_btm_resp(dialog_token, WNM_BTM_ACCEPT, dest_addr, src_addr,
                                        preport[neighbor_index].bssid, ptagnr, tagnr_len, protect);

            /* disconnect and re-assocate with AP2 */
            pnlist_rep_param->nlist_mode   = WLAN_NLIST_11V_PREFERRED;
            pnlist_rep_param->num_channels = 1;
            pnlist_rep_param->channels[0]  = preport[neighbor_index].channel;
            pnlist_rep_param->btm_mode     = btm_mode;
            (void)memcpy((void *)pnlist_rep_param->bssid, (const void *)preport[neighbor_index].bssid,
                         (size_t)MLAN_MAC_ADDR_LENGTH);
        }
        else
        {
            pnlist_rep_param->nlist_mode   = WLAN_NLIST_11V;
            pnlist_rep_param->num_channels = entry_num;
            pnlist_rep_param->btm_mode     = btm_mode;
            pnlist_rep_param->dialog_token = dialog_token;
            pnlist_rep_param->protect      = protect;
            (void)memcpy((void *)pnlist_rep_param->dst_addr, (const void *)dest_addr, (size_t)MLAN_MAC_ADDR_LENGTH);
        }

        if (wifi_event_completion(WIFI_EVENT_NLIST_REPORT, WIFI_EVENT_REASON_SUCCESS, (void *)pnlist_rep_param) !=
            WM_SUCCESS)
        {
            /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree((void *)pnlist_rep_param);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
        }
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(preport);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, preport);
#endif
    }
    else
    {
        enum wnm_btm_status_code status;
        if ((btm_mode & IEEE_WNM_BTM_REQUEST_ESS_DISASSOC_IMMINENT) != 0U)
        {
            status = WNM_BTM_ACCEPT;
        }
        else
        {
            status = WNM_BTM_REJECT_UNSPECIFIED;
        }

        wlan_send_mgmt_wnm_btm_resp(dialog_token, status, dest_addr, src_addr, NULL, ptagnr, tagnr_len, protect);
        /* If don't use variable pnlist_rep_param, free allocated memory ! */
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree((void *)pnlist_rep_param);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
    }
}

int wlan_send_mgmt_bss_trans_query(mlan_private *pmpriv, t_u8 query_reason)
{
    t_u16 pkt_len;
    mlan_802_11_mac_addr *da     = MNULL;
    mlan_802_11_mac_addr *sa     = MNULL;
    wlan_mgmt_pkt *pmgmt_pkt_hdr = MNULL;
    t_u8 *pos                    = MNULL;
    int meas_pkt_len             = 0;

    if (pmpriv->bss_index != (t_u8)MLAN_BSS_ROLE_STA || pmpriv->media_connected != MTRUE)
    {
        wifi_d("invalid interface %d for sending neighbor report request", pmpriv->bss_index);
        return (int)MLAN_STATUS_FAILURE;
    }

    da            = &pmpriv->curr_bss_params.bss_descriptor.mac_address;
    sa            = (mlan_802_11_mac_addr *)(void *)(&pmpriv->curr_addr[0]);
    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(SUBTYPE_ACTION, da, sa, da, sizeof(wlan_mgmt_pkt) + WNM_BTM_QUERY_BUF_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_e("No memory for bss transition management query");
        return (int)MLAN_STATUS_FAILURE;
    }

    /* 802.11 management body */
    pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    pos[0] = (t_u8)IEEE_MGMT_ACTION_CATEGORY_WNM;
    pos[1] = (t_u8)IEEE_MGMT_WNM_BTM_QUERY;
    pos[2] = pmpriv->bss_trans_query_token++;

    if (pmpriv->bss_trans_query_token == (t_u8)255U)
    {
        pmpriv->bss_trans_query_token = (t_u8)1U;
    }

    pos[3] = query_reason;
    pos += 4;

    meas_pkt_len           = pos - (t_u8 *)pmgmt_pkt_hdr;
    pkt_len                = (t_u16)meas_pkt_len;
    pmgmt_pkt_hdr->frm_len = pkt_len - (t_u16)sizeof(pmgmt_pkt_hdr->frm_len);

    (void)wifi_inject_frame(WLAN_BSS_TYPE_STA, (t_u8 *)pmgmt_pkt_hdr, pkt_len);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif

    return (int)MLAN_STATUS_SUCCESS;
}
#endif /* CONFIG_11V */
