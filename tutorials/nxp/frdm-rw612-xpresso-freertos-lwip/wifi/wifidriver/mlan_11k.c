/** @file mlan_11k.c
 *
 *  @brief  This file provides functions for process 11k(RRM) feature
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    08/24/2022: initial version
********************************************************/

#include <mlan_api.h>

#if CONFIG_11K
#define LINK_MSR_REPORT_BUF_SIZE  64U
#define NEIGHBOR_REQUEST_BUF_SIZE 64U
#define rrm_bits_max              255U

/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/
static uint8_t broadcast_mac_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
/********************************************************
                Local Functions
********************************************************/
static t_u8 wlan_rrm_rssi_to_rcpi(int rssi)
{
    t_u8 ret_rcpi = 0;
    int rcpi      = 0;

    if (rssi == 0)
    {
        return (t_u8)255U;
    }
    else if (rssi > 0)
    {
        return (t_u8)220U;
    }
    else if (rssi < -110)
    {
        return (t_u8)0U;
    }
    else
    {
        rcpi     = (rssi + 110) * 2;
        ret_rcpi = (t_u8)rcpi;
        return ret_rcpi;
    }
}

static void wlan_rrm_put_le64(t_u8 *a, t_u64 val)
{
    a[7] = (t_u8)(val >> 56);
    a[6] = (t_u8)(val >> 48);
    a[5] = (t_u8)(val >> 40);
    a[4] = (t_u8)(val >> 32);
    a[3] = (t_u8)(val >> 24);
    a[2] = (t_u8)(val >> 16);
    a[1] = (t_u8)(val >> 8);
    a[0] = (t_u8)(val & 0xffULL);
}

static void wlan_rrm_put_le16(t_u8 *a, t_u16 val)
{
    a[1] = (t_u8)(val >> 8);
    a[0] = (t_u8)(val & (t_u16)0xff);
}

static void wlan_rrm_bit_field_set(t_u8 *bits_field, t_u8 bit)
{
    if (bit >= (t_u8)rrm_bits_max)
    {
        return;
    }

    bits_field[bit / 8U] |= BIT(bit % 8U);
}

static bool wlan_rrm_bit_field_is_set(t_u8 *bit_field, t_u8 bit)
{
    if (bit >= (t_u8)rrm_bits_max)
    {
        return 0;
    }

    return ((bit_field[bit / (t_u8)8U] & (t_u8)(BIT((bit % 8U)))) != (t_u8)0U);
}

/* mod_group : ModulationGroup\n
 *               0: CCK (1,2,5.5,11 Mbps)\n
 *               1: OFDM (6,9,12,18 Mbps)\n
 */
static t_u8 wlan_link_measurement_get_tx_power(wifi_txpwrlimit_t *txpwrlimit, t_u8 ModulationGroup, t_u32 channel)
{
    t_u8 i;
    t_u8 tx_power = 20;

    for (i = 0; i < txpwrlimit->num_chans; i++)
    {
        if (txpwrlimit->txpwrlimit_config[i].chan_desc.chan_num == channel)
        {
            tx_power = txpwrlimit->txpwrlimit_config[i].txpwrlimit_entry[ModulationGroup].tx_power;
            return tx_power;
        }
    }
    return tx_power;
}

static int wlan_process_rm_beacon_req_subelement(wlan_rrm_beacon_report_data *rep_data,
                                                 t_u8 sub_id,
                                                 t_u8 sub_len,
                                                 t_u8 *sub_elem)
{
    t_u8 i;

    switch (sub_id)
    {
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_SSID:
            if (sub_len == (t_u8)0U)
            {
                break;
            }

            if (sub_len > MLAN_MAX_SSID_LENGTH)
            {
                wifi_d("Wrong SSID sub_element len: %u", sub_len);
                return -1;
            }

            rep_data->ssid_length = sub_len;
            (void)memcpy(rep_data->ssid, sub_elem, rep_data->ssid_length);
            break;
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REPORTING_INFO:
            if (sub_len != 2U)
            {
                wifi_d("Wrong reporting info sub_element len: %u", sub_len);
                return -1;
            }

            break;
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REPORTING_DETAIL:
            if (sub_len != 1U)
            {
                wifi_d("Wrong reporting datail sub_element len: %u", sub_len);
                return -1;
            }

            if (rep_data->report_detail > WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS)
            {
                wifi_d("Wrong reporting datail value: %u", rep_data->report_detail);
                return -1;
            }
            rep_data->report_detail = (enum wlan_rrm_beacon_reporting_detail)sub_elem[0];

            break;
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_REQUEST:
            if (rep_data->report_detail != WLAN_RRM_REPORTING_DETAIL_AS_REQUEST)
            {
                wifi_d("Sub_lement request is present with wrong report detail: %u", rep_data->report_detail);
                return -1;
            }

            if (sub_len == (t_u8)0U)
            {
                wifi_d("wrong request sub_element len: %u", sub_len);
                return -1;
            }

            for (i = 0; i < sub_len; i++)
            {
                wlan_rrm_bit_field_set(rep_data->bits_field, sub_elem[i]);
            }
            break;
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_AP_CHANNEL:
            if (rep_data->channel_num + sub_len - 1U > (t_u8)MAX_CHANNEL_LIST)
            {
                return 0;
            }

            for (i = 0; i < (t_u8)(sub_len - 1U); i++)
            {
                rep_data->channel[rep_data->channel_num + i] = sub_elem[i + 1U];
            }

            rep_data->channel_num += sub_len - 1U;
            break;
        case WLAN_RRM_BEACON_REQUEST_SUBELEMENT_LAST_INDICATION:
            if (sub_len != 1U)
            {
                wifi_d("wrong last indication sub_element len: %u", sub_len);
                return -1;
            }

            rep_data->last_ind = sub_elem[0];
            break;
        default:
            wifi_d("Sub element id: %u is not handled for beacon request", sub_id);
            break;
    }

    return 0;
}

static void wlan_process_rm_beacon_report_table(
    wlan_rrm_beacon_report_data *rep_data, t_u8 dialog_tok, t_u8 *dest_addr, t_u8 *src_addr, bool protect)
{
    t_u16 i;
    t_u8 *rep_buf = NULL;
    t_u8 *buf_pos = NULL;
    /* The sufficient size is the length including reporting frame body */
    t_u16 suffi_len           = 250;
    t_u32 pos_last_indication = 0;
    bool match_ap_found       = false;
    int meas_rep_len          = 0;

#if !CONFIG_MEM_POOLS
    rep_buf = (t_u8 *)OSA_MemoryAllocate(BEACON_REPORT_BUF_SIZE);
#else
    rep_buf = (t_u8 *)OSA_MemoryPoolAllocate(buf_1536_MemoryPool);
#endif

    if (rep_buf == NULL)
    {
        wifi_e("Cannot allocate memory for report buffer");
        return;
    }

    (void)memset(rep_buf, 0, BEACON_REPORT_BUF_SIZE);
    buf_pos = rep_buf;
    for (i = 0; i < mlan_adap->num_in_scan_table; i++)
    {
        if (wlan_rrm_matched_ap_found(rep_data, &mlan_adap->pscan_table[i]))
        {
            wlan_add_rm_beacon_report(rep_data, &mlan_adap->pscan_table[i], &buf_pos,
                                      BEACON_REPORT_BUF_SIZE - (buf_pos - rep_buf), &pos_last_indication);
        }

        /* If current rep_buf is not enough and still have AP not added, just send the report */
        if ((buf_pos + suffi_len - rep_buf > BEACON_REPORT_BUF_SIZE) &&
            (i < (t_u16)(mlan_adap->num_in_scan_table - 1U)) &&
            wlan_rrm_matched_ap_found(rep_data, &mlan_adap->pscan_table[i + 1U]))
        {
            match_ap_found = 1;
            meas_rep_len   = buf_pos - rep_buf;
            /* send beacon report, not the last one */
            wlan_send_mgmt_rm_beacon_report(dialog_tok, dest_addr, src_addr, rep_buf, (t_u32)(meas_rep_len), protect);
            /* Prepare for the next beacon report */
            (void)memset(rep_buf, 0, BEACON_REPORT_BUF_SIZE);
            buf_pos = rep_buf;
        }

        /* Last AP in scan table, and matched AP found */
        if ((i == mlan_adap->num_in_scan_table - 1U) && (buf_pos > rep_buf))
        {
            match_ap_found = 1;
            /* Update last indication, the last one */
            if (rep_data->last_ind > (t_u8)0U && pos_last_indication > 0U)
            {
                *(char *)pos_last_indication = (char)1U;
            }
            meas_rep_len = buf_pos - rep_buf;
            /* send beacon report, the last one */
            wlan_send_mgmt_rm_beacon_report(dialog_tok, dest_addr, src_addr, rep_buf, (t_u32)(meas_rep_len), protect);
        }
    }

    /* If no matched AP found, no beacon report detail */
    if (!match_ap_found)
    {
        *buf_pos++ = (t_u8)MEASURE_REPORT;
        /* Tag length */
        *buf_pos++   = 3;
        *buf_pos++   = rep_data->token;
        *buf_pos++   = WLAN_RRM_REPORT_MODE_ACCEPT;
        *buf_pos++   = WLAN_RRM_MEASURE_TYPE_BEACON;
        meas_rep_len = buf_pos - rep_buf;
        /* send beacon report */
        wlan_send_mgmt_rm_beacon_report(dialog_tok, dest_addr, src_addr, rep_buf, (t_u32)(meas_rep_len), protect);
    }
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(rep_buf);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, rep_buf);
#endif
}

bool wlan_rrm_matched_ap_found(wlan_rrm_beacon_report_data *rep_data, BSSDescriptor_t *bss_entry)
{
    if (memcmp(rep_data->bssid, broadcast_mac_addr, IEEEtypes_ADDRESS_SIZE) != 0 &&
        memcmp(rep_data->bssid, bss_entry->mac_address, IEEEtypes_ADDRESS_SIZE) != 0)
    {
        return 0;
    }

    if (rep_data->ssid_length > 0U && (rep_data->ssid_length != bss_entry->ssid.ssid_len ||
                                       memcmp(rep_data->ssid, bss_entry->ssid.ssid, bss_entry->ssid.ssid_len) != 0))
    {
        return 0;
    }

    return 1;
}

static void wlan_process_rm_beacon_req(t_u8 *req,
                                       t_u32 len,
                                       t_u8 element_token,
                                       t_u8 dialog_tok,
                                       t_u8 *dest_addr,
                                       t_u8 *src_addr,
                                       bool protect,
                                       bool duration_mandatory)
{
    mgmt_rrm_meas_beacon_request *beacon_req = (mgmt_rrm_meas_beacon_request *)(void *)req;
    t_u8 *sub_element;
    int element_len;
    int ret = 0, i;
    wlan_scan_params_v2_t wlan_scan_param;
    wlan_rrm_scan_cb_param *param = NULL;

    if (beacon_req->mode != (t_u8)WLAN_RRM_MEASUREMENT_MODE_PASSIVE &&
        beacon_req->mode != (t_u8)WLAN_RRM_MEASUREMENT_MODE_ACTIVE &&
        beacon_req->mode != (t_u8)WLAN_RRM_MEASUREMENT_MODE_TABLE)
    {
        return;
    }

    sub_element = beacon_req->variable;
    element_len = (int)len - ((int)sizeof(mgmt_rrm_meas_beacon_request) - 1);

#if !CONFIG_MEM_POOLS
    param = (wlan_rrm_scan_cb_param *)OSA_MemoryAllocate(sizeof(wlan_rrm_scan_cb_param));
#else
    param = (wlan_rrm_scan_cb_param *)OSA_MemoryPoolAllocate(buf_256_MemoryPool);
#endif
    if (param == NULL)
    {
        wifi_e("Cannot allocate memory for rm scan param");
        return;
    }
    (void)memset(param, 0, sizeof(wlan_rrm_scan_cb_param));
    param->rep_data.token         = element_token;
    param->rep_data.report_detail = WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS;
    param->rep_data.duration      = wlan_le16_to_cpu(beacon_req->duration);
    (void)memcpy(param->rep_data.bssid, beacon_req->bssid, IEEEtypes_ADDRESS_SIZE);

    while (element_len >= 2)
    {
        ret = wlan_process_rm_beacon_req_subelement(&param->rep_data, sub_element[0], sub_element[1], &sub_element[2]);
        if (ret < 0)
        {
            goto output;
        }

        element_len -= 2 + (int)sub_element[1];
        sub_element += 2U + sub_element[1];
    }

    /* Measurement mode: Beacon Table */
    if (beacon_req->mode == (t_u8)WLAN_RRM_MEASUREMENT_MODE_TABLE)
    {
        wlan_process_rm_beacon_report_table(&param->rep_data, dialog_tok, dest_addr, src_addr, protect);
        goto output;
    }

    /* Measurement mode: Passive or Active, need to scan first */
    (void)memset(&wlan_scan_param, 0, sizeof(wlan_scan_params_v2_t));
    if (beacon_req->channel > 0U && beacon_req->channel != 255U)
    {
        wlan_scan_param.num_channels             = 1;
        wlan_scan_param.chan_list[0].chan_number = beacon_req->channel;
        if (beacon_req->mode == (t_u8)WLAN_RRM_MEASUREMENT_MODE_ACTIVE)
        {
            wlan_scan_param.chan_list[0].scan_type = MLAN_SCAN_TYPE_ACTIVE;
        }
        else
        {
            wlan_scan_param.chan_list[0].scan_type = MLAN_SCAN_TYPE_PASSIVE;
        }

        if (duration_mandatory)
        {
            wlan_scan_param.chan_list[0].scan_time = beacon_req->duration;
        }
    }
    else if (beacon_req->channel == 255U && param->rep_data.channel_num > (t_u8)0U)
    {
        wlan_scan_param.num_channels = param->rep_data.channel_num;
        for (i = 0; i < (int)param->rep_data.channel_num && i < MAX_CHANNEL_LIST; i++)
        {
            wlan_scan_param.chan_list[i].chan_number = param->rep_data.channel[i];
            if (beacon_req->mode == (t_u8)WLAN_RRM_MEASUREMENT_MODE_ACTIVE)
            {
                wlan_scan_param.chan_list[i].scan_type = MLAN_SCAN_TYPE_ACTIVE;
            }
            else
            {
                wlan_scan_param.chan_list[i].scan_type = MLAN_SCAN_TYPE_PASSIVE;
            }

            if (duration_mandatory)
            {
                wlan_scan_param.chan_list[i].scan_time = beacon_req->duration;
            }
        }
    }
    else
    {
        /* Do nothing */
    }

    if (param->rep_data.ssid_length > (t_u8)0U)
    {
        (void)memcpy((void *)&wlan_scan_param.ssid[0], (const void *)param->rep_data.ssid,
                     (size_t)param->rep_data.ssid_length);
    }

    param->dialog_tok = dialog_tok;
    param->protect    = (t_u8)protect;
    (void)memcpy(param->dst_addr, src_addr, IEEEtypes_ADDRESS_SIZE);
    wlan_scan_param.cb = _wlan_rrm_scan_cb;

    /* After scanning done, will send beacon report */
    wlan_rrm_request_scan(&wlan_scan_param, param);
output:
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(param);
#else
    OSA_MemoryPoolFree(buf_256_MemoryPool, param);
#endif

    return;
}

/********************************************************
                Global functions
********************************************************/
void wlan_add_rm_beacon_report(wlan_rrm_beacon_report_data *rep_data,
                               BSSDescriptor_t *bss_entry,
                               t_u8 **buf_pos,
                               t_s32 remained_len,
                               t_u32 *pos_last_indication)
{
    mgmt_rrm_meas_beacon_report report;
    t_u8 *pos         = *buf_pos;
    t_u8 *pos_tag_len = NULL;
    t_u8 *pos_sub_len = NULL;
    t_u16 cap_info    = (t_u16)0U;
    int meas_tag_len  = 0;
    int meas_sub_len  = 0;

    (void)memset(&report, 0, sizeof(mgmt_rrm_meas_beacon_report) - 1U);
    (void)wlan_get_curr_global_oper_class(mlan_adap->priv[0], (t_u8)bss_entry->channel, bss_entry->curr_bandwidth,
                                          &report.op_class);

    /* Measurement report */
    *pos++ = (t_u8)MEASURE_REPORT;
    /* The length will be filled below */
    pos_tag_len = pos;
    pos++;
    *pos++ = rep_data->token;
    *pos++ = WLAN_RRM_REPORT_MODE_ACCEPT;
    *pos++ = WLAN_RRM_MEASURE_TYPE_BEACON;

    /* Beacon report element */
    report.channel = bss_entry->channel;
    /* Start time is not verified in CERT, to be done */
    report.m_start_time = 0;
    report.duration     = wlan_cpu_to_le16(rep_data->duration);
    report.rcpi         = wlan_rrm_rssi_to_rcpi(-(bss_entry->rssi));
    report.rsni         = 255;
    (void)memcpy(report.bssid, bss_entry->mac_address, IEEEtypes_ADDRESS_SIZE);
    report.ant_id = 0;
    /* Parent tsf is not verified in CERT, to be done */
    report.parent_tsf = 0;
    if (bss_entry->pvht_cap != MNULL)
    {
        report.report_info = (t_u8)WLAN_PHY_TYPE_VHT;
    }
    else if (bss_entry->pht_cap != MNULL)
    {
        report.report_info = (t_u8)WLAN_PHY_TYPE_HT;
    }
    else
    {
        /* Do nothing */
    }

    (void)memcpy((void *)pos, (const void *)&report, sizeof(mgmt_rrm_meas_beacon_report) - 1U);
    pos += sizeof(mgmt_rrm_meas_beacon_report) - 1U;

    /* Start adding reported frame body */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_NONE)
    {
        goto without_subelem;
    }
    /* The min length of reported frame body is 14 */
    if (pos + 14 - *buf_pos > remained_len)
    {
        goto without_subelem;
    }

    *pos++ = WLAN_RRM_BEACON_REP_SUBELEM_FRAME_BODY;
    /* The length will be filled below */
    pos_sub_len = pos;
    pos++;
    /* TSF to be done */
    wlan_rrm_put_le64(pos, bss_entry->network_tsf);
    pos += 8;
    wlan_rrm_put_le16(pos, bss_entry->beacon_period);
    pos += 2;
    (void)memcpy((void *)&cap_info, (const void *)&bss_entry->cap_info, sizeof(t_u16));
    wlan_rrm_put_le16(pos, cap_info);
    pos += 2;
    /* SSID tag */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS ||
        wlan_rrm_bit_field_is_set(rep_data->bits_field, (t_u8)SSID))
    {
        if (pos + sizeof(IEEEtypes_Header_t) + bss_entry->ssid.ssid_len - *buf_pos > remained_len)
        {
            goto part_subelem;
        }

        *pos++ = (t_u8)SSID;
        *pos++ = (t_u8)bss_entry->ssid.ssid_len;
        if (bss_entry->ssid.ssid_len > 0U)
        {
            (void)memcpy(pos, &(bss_entry->ssid.ssid), bss_entry->ssid.ssid_len);
            pos += bss_entry->ssid.ssid_len;
        }
    }
    /* RSN tag */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS ||
        wlan_rrm_bit_field_is_set(rep_data->bits_field, (t_u8)RSN_IE))
    {
        if (bss_entry->rsn_ie_buff_len > (size_t)0U)
        {
            if (pos + bss_entry->rsn_ie_buff_len - *buf_pos > remained_len)
            {
                goto part_subelem;
            }

            (void)memcpy(pos, bss_entry->rsn_ie_buff, bss_entry->rsn_ie_buff_len);
            pos += bss_entry->rsn_ie_buff_len;
        }
    }
    /* Mobility Domain tag */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS ||
        wlan_rrm_bit_field_is_set(rep_data->bits_field, (t_u8)MOBILITY_DOMAIN))
    {
        if (pos + sizeof(IEEEtypes_MobilityDomain_t) - *buf_pos > remained_len)
        {
            goto part_subelem;
        }

        if (bss_entry->mob_domain_exist)
        {
            (void)memcpy((void *)pos, (const void *)bss_entry->md_ie_buff, sizeof(IEEEtypes_MobilityDomain_t));
            pos += sizeof(IEEEtypes_MobilityDomain_t);
        }
    }
    /* RM Enable Capabilities tag */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS ||
        wlan_rrm_bit_field_is_set(rep_data->bits_field, (t_u8)RRM_ENABLED_CAP))
    {
        if (pos + sizeof(bss_entry->rm_cap_saved) - *buf_pos > remained_len)
        {
            goto part_subelem;
        }

        if (bss_entry->rm_cap_exist)
        {
            (void)memcpy((void *)pos, (const void *)&bss_entry->rm_cap_saved, sizeof(bss_entry->rm_cap_saved));
            pos += sizeof(bss_entry->rm_cap_saved);
        }
    }
    /* Vendor Specific tag */
    if (rep_data->report_detail == WLAN_RRM_REPORTING_DETAIL_ALL_FIELDS_AND_ELEMENTS ||
        wlan_rrm_bit_field_is_set(rep_data->bits_field, (t_u8)VENDOR_SPECIFIC_221))
    {
        /* wpa */
        if (bss_entry->wpa_ie_buff_len > 0U)
        {
            if (pos + bss_entry->wpa_ie_buff_len - *buf_pos > remained_len)
            {
                goto part_subelem;
            }

            (void)memcpy(pos, bss_entry->wpa_ie_buff, bss_entry->wpa_ie_buff_len);
            pos += bss_entry->wpa_ie_buff_len;
        }
        /* wmm */
        if (pos + sizeof(bss_entry->wmm_ie) - *buf_pos > remained_len)
        {
            goto part_subelem;
        }

        if (wlan_strlen((char *)(&bss_entry->wmm_ie)) > 0U)
        {
            (void)memcpy((void *)pos, (const void *)&bss_entry->wmm_ie, sizeof(bss_entry->wmm_ie));
            pos += sizeof(bss_entry->wmm_ie);
        }
        /* Others */
        if (bss_entry->vendor_ie_len > (t_u8)0U)
        {
            if (pos + bss_entry->vendor_ie_len - *buf_pos > remained_len)
            {
                goto part_subelem;
            }

            (void)memcpy(pos, bss_entry->vendor_ie_buff, bss_entry->vendor_ie_len);
            pos += bss_entry->vendor_ie_len;
        }
    }

    if (rep_data->last_ind > 0U)
    {
        if (pos + WLAN_RRM_BEACON_REP_FRAME_BODY_FRAGMENT_SUB_LEN + WLAN_RRM_BEACON_REP_LAST_INDICATION_SUB_LEN -
                *buf_pos >
            remained_len)
        {
            goto part_subelem;
        }

        /* Frame body fragment id subelement */
        pos[0] = WLAN_RRM_BEACON_REP_SUBELEM_FRAME_BODY_FRAGMENT_ID;
        pos[1] = 2; /* Length */
        pos[2] = 1; /* Beacon report id */
        pos[3] = 0; /* Fragment id number */
        pos += WLAN_RRM_BEACON_REP_FRAME_BODY_FRAGMENT_SUB_LEN;
        /* Last indication subelement */
        pos[0]               = WLAN_RRM_BEACON_REP_SUBELEM_LAST_INDICATION;
        pos[1]               = 1; /* Length */
        pos[2]               = 0; /* Temporarily, not the last packet */
        *pos_last_indication = (t_u32)&pos[2];
        pos += WLAN_RRM_BEACON_REP_LAST_INDICATION_SUB_LEN;
    }
    /* Fill the length */
part_subelem:
    meas_sub_len = pos - pos_sub_len - 1;
    *pos_sub_len = (t_u8)meas_sub_len;
without_subelem:
    meas_tag_len = pos - pos_tag_len - 1;
    *pos_tag_len = (t_u8)meas_tag_len;
    *buf_pos     = pos;
}

void wlan_send_mgmt_rm_beacon_report(
    t_u8 dialog_tok, t_u8 *src_addr, t_u8 *dst_addr, t_u8 *rep, t_u32 rep_len, bool protect)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    IEEEtypes_FrameCtl_t *mgmt_fc_p = MNULL;
    t_u8 *pos                       = MNULL;
    t_u16 pkt_len                   = 0;
    int meas_pkt_len                = 0;

    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(
        SUBTYPE_ACTION, (mlan_802_11_mac_addr *)(void *)dst_addr, (mlan_802_11_mac_addr *)(void *)src_addr,
        (mlan_802_11_mac_addr *)(void *)dst_addr, sizeof(wlan_mgmt_pkt) + (t_u32)BEACON_REPORT_BUF_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_d("No memory available for beacon report");
        return;
    }

    mgmt_fc_p = (IEEEtypes_FrameCtl_t *)(void *)&pmgmt_pkt_hdr->wlan_header.frm_ctl;
    if (protect)
    {
        mgmt_fc_p->wep = 1;
    }

    /* 802.11 management body */
    pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    pos[0] = (t_u8)IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC;
    pos[1] = (t_u8)IEEE_MGMT_RRM_RADIO_MEASUREMENT_REPORT;
    pos[2] = dialog_tok;

    pos += 3;
    if (rep != MNULL && rep_len > 0U)
    {
        (void)memcpy(pos, rep, rep_len);
        pos += rep_len;
    }
    meas_pkt_len           = pos - (t_u8 *)pmgmt_pkt_hdr;
    pkt_len                = (t_u16)(meas_pkt_len);
    pmgmt_pkt_hdr->frm_len = (t_u16)pkt_len - (t_u16)sizeof(t_u16);

    /* Send packet */
    (void)wifi_inject_frame(WLAN_BSS_TYPE_STA, (t_u8 *)pmgmt_pkt_hdr, pkt_len);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif
}

void wlan_process_radio_measurement_request(t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect)
{
    t_u8 *pos = frame;
    t_u8 dialog_tok;
    bool duration_mandatory;

    if (len < 3U)
    {
        wifi_d("Ignoring too short radio measurement request");
        return;
    }

    dialog_tok = *pos;
    /* Bypass dialog token and repetitions */
    pos += 3;
    len -= 3U;

    /* Start process measurement quest */
    mgmt_rrm_radio_meas_request *request = (mgmt_rrm_radio_meas_request *)(void *)pos;
    if (request->ele_id != (t_u8)MEASURE_REQUEST)
    {
        wifi_d("eid %u is not radio measure request element", request->ele_id);
        return;
    }
    if (request->length < 3U)
    {
        wifi_d("radio measure request element length too short");
        return;
    }
    if (request->length > len - 2U)
    {
        wifi_d("radio measure request element length too long");
        return;
    }

    duration_mandatory = !!((request->mode & WLAN_RRM_MEAS_REQUEST_MODE_DURATION_MANDATORY) != 0U);

    switch (request->type)
    {
        /* Now only support beacon request */
        case WLAN_RRM_MEASURE_TYPE_BEACON:
            wlan_process_rm_beacon_req(request->variable, (t_u32)request->length - (t_u32)3, request->token, dialog_tok,
                                       dest_addr, src_addr, protect, duration_mandatory);
            break;
        default:
            wifi_d("radio measure request type %u is not supported", request->type);
            break;
    }
}

static void wlan_send_mgmt_link_measurement_report(
    t_u8 *src_addr, t_u8 *dst_addr, t_u8 *rep, t_u32 rep_len, bool protect)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    IEEEtypes_FrameCtl_t *mgmt_fc_p = MNULL;
    t_u8 *pos                       = MNULL;
    t_u16 pkt_len                   = 0;
    int meas_pkt_len                = 0;

    pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(
        SUBTYPE_ACTION, (mlan_802_11_mac_addr *)(void *)dst_addr, (mlan_802_11_mac_addr *)(void *)src_addr,
        (mlan_802_11_mac_addr *)(void *)dst_addr, sizeof(wlan_mgmt_pkt) + LINK_MSR_REPORT_BUF_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_d("No memory available for beacon report");
        return;
    }

    mgmt_fc_p = (IEEEtypes_FrameCtl_t *)(void *)&pmgmt_pkt_hdr->wlan_header.frm_ctl;
    if (protect)
    {
        mgmt_fc_p->wep = 1;
    }

    /* 802.11 management body */
    pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    pos[0] = (t_u8)IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC;
    pos[1] = (t_u8)IEEE_MGMT_RRM_LINK_MEASUREMENT_REPORT;
    pos += 2;

    if (rep != MNULL && rep_len > 0U)
    {
        (void)memcpy(pos, rep, rep_len);
        pos += rep_len;
    }
    meas_pkt_len           = pos - (t_u8 *)pmgmt_pkt_hdr;
    pkt_len                = (t_u16)meas_pkt_len;
    pmgmt_pkt_hdr->frm_len = (t_u16)pkt_len - (t_u16)sizeof(t_u16);

    /* Send packet */
    (void)wifi_inject_frame(WLAN_BSS_TYPE_STA, (t_u8 *)pmgmt_pkt_hdr, pkt_len);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(pmgmt_pkt_hdr);
#else
    OSA_MemoryPoolFree(buf_1536_MemoryPool, pmgmt_pkt_hdr);
#endif
}

void wlan_process_link_measurement_request(
    t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect, RxPD *rxpd)
{
    mgmt_rrm_link_meas_request *request = (mgmt_rrm_link_meas_request *)(void *)frame;
    mgmt_rrm_link_meas_report report;
    t_u32 channel;
    wifi_SubBand_t subband;
    wlan_txpwrlimit_t *txpwrlimit = NULL;
    t_u8 ModulationGroup          = 1; /* Default use OFDM modulation */
    int meas_link_margin          = 0;
    int meas_noise                = 0;

    if (len < sizeof(mgmt_rrm_link_meas_request) - 1U)
    {
        wifi_d("Link measurement request too short");
        return;
    }

    (void)memset(&report, 0, sizeof(report));
    report.dialog_tok = request->dialog_tok;
    meas_noise        = -((int)rxpd->nf + (int)rxpd->snr);
    report.rsni       = (t_u8)(meas_noise);
    report.rcpi       = wlan_rrm_rssi_to_rcpi((int)report.rsni);

    /* TPC Report */
    channel = mlan_adap->priv[0]->curr_bss_params.bss_descriptor.channel;
    if (channel <= 14U)
    {
        subband = SubBand_2_4_GHz;
        /* use CCK modulation */
        ModulationGroup = 0;
    }
    else if (channel < 100U)
    {
        subband = SubBand_5_GHz_0;
    }
    else if (channel < 149U)
    {
        subband = SubBand_5_GHz_1;
    }
    else if (channel < 183U)
    {
        subband = SubBand_5_GHz_2;
    }
    else
    {
        subband = SubBand_5_GHz_3;
    }

#if !CONFIG_MEM_POOLS
    txpwrlimit = OSA_MemoryAllocate(sizeof(wlan_txpwrlimit_t));
#else
    txpwrlimit = OSA_MemoryPoolAllocate(buf_2048_MemoryPool);
#endif

    if (txpwrlimit == NULL)
    {
        wifi_e("Cannot allocate memory for txpwrlimit");
        return;
    }

    int rv = wlan_get_txpwrlimit(subband, txpwrlimit);
    if (rv != WM_SUCCESS)
    {
        wifi_d("Unable to get TX PWR Limit configuration");
    }
    else
    {
        report.tpc_report.tx_power = (t_s8)wlan_link_measurement_get_tx_power(txpwrlimit, ModulationGroup, channel);
    }

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(txpwrlimit);
#else
    OSA_MemoryPoolFree(buf_2048_MemoryPool, txpwrlimit);
#endif

    /* Default use CCK5_5Mbps */
    meas_link_margin              = 78 - (int)report.rsni;
    report.tpc_report.link_margin = (t_s8)meas_link_margin;
    report.tpc_report.element_id  = TPC_REPORT;
    report.tpc_report.len         = 2;

    wlan_send_mgmt_link_measurement_report(dest_addr, src_addr, (t_u8 *)&report, sizeof(mgmt_rrm_link_meas_report) - 1U,
                                           protect);
}

static bool wifi_find_in_channels(t_u8 *channels, t_u8 entry_num, t_u8 chan)
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

void wlan_process_neighbor_report_response(t_u8 *frame, t_u32 len, t_u8 *dest_addr, t_u8 *src_addr, bool protect)
{
    t_u8 *pos      = frame;
    t_u8 entry_num = 0, chan;
#if !CONFIG_MEM_POOLS
    wlan_nlist_report_param *pnlist_rep_param =
        (wlan_nlist_report_param *)OSA_MemoryAllocate(sizeof(wlan_nlist_report_param));
#else
    wlan_nlist_report_param *pnlist_rep_param = (wlan_nlist_report_param *)OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif

    wifi_d("Neighbor report event");
#if CONFIG_WIFI_EXTRA_DEBUG
    dump_hex(frame, len);
#endif
    if (pnlist_rep_param == MNULL)
    {
        wifi_e("11k nlist report param buffer alloc failed %d", sizeof(wlan_nlist_report_param));
        return;
    }

    if (len < 3U)
    {
        wifi_d("Ignoring too short radio measurement request");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree((void *)pnlist_rep_param);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
        return;
    }

    memset(pnlist_rep_param, 0, sizeof(wlan_nlist_report_param));
    /* Bypass dialog token */
    pos += 1;
    len -= 1U;

    /* Start process neighbor report response */

#define NR_IE_MIN_LEN            (IEEEtypes_ADDRESS_SIZE + 4 + 1 + 1 + 1)
#define WLAN_EID_NEIGHBOR_REPORT 52

    while (len >= 2 + NR_IE_MIN_LEN)
    {
        t_u8 nr_len = pos[1];

        if (pos[0] != WLAN_EID_NEIGHBOR_REPORT || nr_len < NR_IE_MIN_LEN)
        {
            wifi_d("Invalid Neighbor Report element: id=%u len=%u", pos[0], nr_len);
            goto out;
        }

        if (2U + nr_len > len)
        {
            wifi_d("Invalid Neighbor Report element: id=%u len=%zu nr_len=%u", pos[0], len, nr_len);
            goto out;
        }
        pos += 2;

        chan = pos[IEEEtypes_ADDRESS_SIZE + 5];

        wifi_d("channel = %d", chan);
        if (!wifi_find_in_channels(pnlist_rep_param->channels, entry_num, chan))
        {
            pnlist_rep_param->channels[entry_num] = chan;
            entry_num++;
        }

        pos += nr_len;
        len -= 2 + nr_len;
    }

    if (entry_num == 0U)
    {
        goto out;
    }

    pnlist_rep_param->nlist_mode   = WLAN_NLIST_11K;
    pnlist_rep_param->num_channels = entry_num;

    if (wifi_event_completion(WIFI_EVENT_NLIST_REPORT, WIFI_EVENT_REASON_SUCCESS, pnlist_rep_param) != WM_SUCCESS)
    {
        /* If fail to send message on queue, free allocated memory ! */
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree((void *)pnlist_rep_param);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
    }
    return;

out:
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree((void *)pnlist_rep_param);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, pnlist_rep_param);
#endif
}

/**
 * @brief This function sets up the RRM Enabled Capabilites IE.
 *
 * @param pRrmCapIe A pointer to Rrm Enabled Capabilities element structure
 * @param bcnInterval Beacon interval
 *
 * @return void
 */
void wlan_dot11k_formatRrmCapabilities(IEEEtypes_RrmEnabledCapabilities_t *pRrmCapIe, t_u16 bcnInterval)
{
    (void)memset((void *)pRrmCapIe, 0x00, sizeof(IEEEtypes_RrmEnabledCapabilities_t));

    pRrmCapIe->LinkMeas       = 1;
    pRrmCapIe->NborRpt        = 0;
    pRrmCapIe->BcnPassiveMeas = 1;
    pRrmCapIe->BcnActiveMeas  = 1;
    pRrmCapIe->BcnTableMeas   = 1;
    pRrmCapIe->TxStreamMeas   = 1;

    pRrmCapIe->OpChanMaxMeas    = 4; /* TBD: copy the result from fw dot11k_getRmMeasMax() */
    pRrmCapIe->NonOpChanMaxMeas = 2; /* TBD: copy the result from fw dot11k_getRmMeasMax() */

    pRrmCapIe->ParallelMeas = 0;
    pRrmCapIe->RepeatMeas   = 0;
}

int wlan_send_mgmt_rm_neighbor_request(mlan_private *pmpriv, t_u8 *ssid, t_u8 ssid_len)
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

    da = &pmpriv->curr_bss_params.bss_descriptor.mac_address;
    sa = (mlan_802_11_mac_addr *)(void *)(&pmpriv->curr_addr[0]);
    pmgmt_pkt_hdr =
        wifi_PrepDefaultMgtMsg(SUBTYPE_ACTION, da, sa, da, sizeof(wlan_mgmt_pkt) + NEIGHBOR_REQUEST_BUF_SIZE);
    if (pmgmt_pkt_hdr == MNULL)
    {
        wifi_e("No memory for neighbor report request");
        return (int)MLAN_STATUS_FAILURE;
    }

    /* 802.11 management body */
    pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
    pos[0] = (t_u8)IEEE_MGMT_ACTION_CATEGORY_RADIO_RSRC;
    pos[1] = (t_u8)IEEE_MGMT_RRM_NEIGHBOR_REPORT_REQUEST;
    pos[2] = pmpriv->neighbor_rep_token++;

    if (pmpriv->neighbor_rep_token == (t_u8)255U)
    {
        pmpriv->neighbor_rep_token = (t_u8)1U;
    }

    pos += 3;

    /* SSID Tag */
    if (ssid_len > (t_u8)0U)
    {
        pos[0] = (t_u8)SSID;
        pos[1] = ssid_len;
        (void)memcpy(&pos[2], ssid, ssid_len);
        pos += ssid_len + 2U;
    }

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
#endif /* CONFIG_11K */
