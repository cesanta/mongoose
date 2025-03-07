/** @file wifi_pwrmgr.c
 *
 *  @brief This file provides all power management code for WIFI.
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <mlan_api.h>

#include <wmerrno.h>
#include <osa.h>
#include <wifi.h>
#include <wifi_events.h>

#include <wmlog.h>
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#include "wifi-internal.h"

#define pwr_e(...) wmlog_e("pwr", ##__VA_ARGS__)
#define pwr_w(...) wmlog_w("pwr", ##__VA_ARGS__)

#define pwr_d(...)

#define MAX_LISTEN_INTERVAL_IN_BCON     49
#define MIN_LISTEN_INTERVAL_IN_TU       50
#define MAX_LISTEN_INTERVAL_IN_TU       65000
#define CLOSEST_DTIM_TO_LISTEN_INTERVAL 65534

static bool ieeeps_enabled;
static bool deepsleepps_enabled;

void wifi_set_ps_cfg(t_u16 multiple_dtims,
                     t_u16 bcn_miss_timeout,
                     t_u16 local_listen_interval,
                     t_u16 adhoc_wake_period,
                     t_u16 mode,
                     t_u16 delay_to_ps)
{
    pmlan_adapter pmadapter          = ((mlan_private *)mlan_adap->priv[0])->adapter;
    pmadapter->bcn_miss_time_out     = bcn_miss_timeout;
    pmadapter->multiple_dtim         = multiple_dtims;
    pmadapter->local_listen_interval = local_listen_interval;
    pmadapter->delay_to_ps           = delay_to_ps;
    pmadapter->enhanced_ps_mode      = mode;
    pmadapter->adhoc_awake_period    = adhoc_wake_period;
}

void wifi_configure_null_pkt_interval(unsigned int null_pkt_interval)
{
    pmlan_adapter pmadapter      = ((mlan_private *)mlan_adap->priv[0])->adapter;
    pmadapter->null_pkt_interval = (t_u16)null_pkt_interval;
}

void wifi_configure_listen_interval(int listen_interval)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    pmpriv->listen_interval = (t_u16)listen_interval;

    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;
    wifi_set_ps_cfg(CLOSEST_DTIM_TO_LISTEN_INTERVAL, DEFAULT_BCN_MISS_TIMEOUT, 0, 0, PS_MODE_AUTO, DELAY_TO_PS_DEFAULT);
    pmadapter->local_listen_interval = (t_u16)listen_interval;
}

void wifi_configure_delay_to_ps(unsigned int timeout_ms)
{
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;

    pmadapter->delay_to_ps = (t_u16)timeout_ms;
}

void wifi_configure_idle_time(unsigned int timeout_ms)
{
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;

    if (timeout_ms < DEEP_SLEEP_IDLE_TIME)
    {
        pwr_e("The idle time is too small. Minimum value: 100ms");
        return;
    }
    pmadapter->idle_time = (t_u16)timeout_ms;
}

unsigned short wifi_get_listen_interval()
{
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;

    return (unsigned short)pmadapter->local_listen_interval;
}

unsigned int wifi_get_delay_to_ps()
{
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;

    return (unsigned int)pmadapter->delay_to_ps;
}

unsigned int wifi_get_idle_time()
{
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;

    return (unsigned int)pmadapter->idle_time;
}

#if CONFIG_HOST_SLEEP
int wifi_send_hs_cfg_cmd(mlan_bss_type interface, t_u32 ipv4_addr, t_u16 action, t_u32 conditions)
{
    pmlan_adapter pmadapter     = ((mlan_private *)mlan_adap->priv[0])->adapter;
    arpfilter_header *arpfilter = NULL;
    filter_entry *entry         = NULL;
    void *pdata_buf             = NULL;
    hs_config_param hs_cfg_obj;
    t_u8 *tlv                                      = NULL;
    MrvlIEtypes_MgmtFrameFilter_t *mgmt_filter_tlv = NULL;

    (void)wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    (void)memset(&hs_cfg_obj, 0x00, sizeof(hs_config_param));

    cmd->seq_num = (t_u16)(HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u32)(interface)));
    if (action == (t_u16)HS_CONFIGURE)
    {
        hs_cfg_obj.conditions = conditions;
        hs_cfg_obj.gap        = 0xc8;
#ifdef RW610
        hs_cfg_obj.gpio = 0xff;
#else
        hs_cfg_obj.gpio = HOST_WAKEUP_GPIO_PIN;
#endif
        pdata_buf = &hs_cfg_obj;

        /* wake conditions for broadcast is
         * enabled when bit 0 is set.
         * The code below sets the correct bit which
         * firmware will use to give host wakeup
         */
        if ((conditions != (t_u32)(HOST_SLEEP_CFG_CANCEL)) &&
            ((conditions & (t_u32)(WIFI_WAKE_ON_ARP_BROADCAST)) != 0U))
        {
            hs_cfg_obj.conditions |= (t_u32)(WIFI_WAKE_ON_ALL_BROADCAST);
            hs_cfg_obj.conditions &= ~((t_u32)(WIFI_WAKE_ON_ARP_BROADCAST));
        }
    }

    mlan_status status = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_HS_CFG_ENH,
                                                  HostCmd_ACT_GEN_SET, 0, NULL, pdata_buf, cmd);
    /* Construct the ARP filter TLV */
    arpfilter       = (arpfilter_header *)((uint32_t)cmd + cmd->size);
    arpfilter->type = TLV_TYPE_ARP_FILTER;
    arpfilter->len  = 0;

    if ((ipv4_addr != 0U) && (action == (t_u16)HS_CONFIGURE) &&
        (conditions &
         (WIFI_WAKE_ON_MULTICAST | WIFI_WAKE_ON_ALL_BROADCAST | WIFI_WAKE_ON_UNICAST | WIFI_WAKE_ON_ARP_BROADCAST)))
    {
        entry = (filter_entry *)((uint32_t)arpfilter + sizeof(arpfilter_header));
        if ((conditions & (t_u32)(WIFI_WAKE_ON_MULTICAST)) != 0U)
        {
            entry->addr_type = ADDR_TYPE_MULTICAST;
            entry->eth_type  = ETHER_TYPE_ANY;
            entry->ipv4_addr = IPV4_ADDR_ANY;
            entry++;
            arpfilter->len += sizeof(filter_entry);
        }

        if ((conditions & (t_u32)(WIFI_WAKE_ON_ALL_BROADCAST | WIFI_WAKE_ON_ARP_BROADCAST)) != 0U)
        {
            entry->addr_type = ADDR_TYPE_BROADCAST;
            if ((conditions & (t_u32)(WIFI_WAKE_ON_ALL_BROADCAST)) != 0U)
            {
                entry->eth_type  = ETHER_TYPE_ANY;
                entry->ipv4_addr = IPV4_ADDR_ANY;
            }
            else
            {
                entry->eth_type  = ETHER_TYPE_ARP;
                entry->ipv4_addr = ipv4_addr;
            }
            entry++;
            arpfilter->len += sizeof(filter_entry);
        }

        if ((conditions & (t_u32)(WIFI_WAKE_ON_UNICAST)) != 0U)
        {
            entry->addr_type = ADDR_TYPE_UNICAST;
            entry->eth_type  = ETHER_TYPE_ANY;
            entry->ipv4_addr = IPV4_ADDR_ANY;
            arpfilter->len += sizeof(filter_entry);
        }
        cmd->size = (t_u16)(cmd->size + sizeof(arpfilter_header) + arpfilter->len);
    }
    else if (action == (t_u16)HS_ACTIVATE)
    {
        arpfilter->len = 0;
        cmd->size -= (t_u16)sizeof(t_u32);
    }
    else
    {
        /** Do nothing */
    }
    if (action == (t_u16)HS_CONFIGURE)
    {
        tlv = (t_u8 *)((uint32_t)cmd + cmd->size);

        if (pmadapter->mgmt_filter[0].type)
        {
            int i = 0;
            mgmt_frame_filter mgmt_filter[MAX_MGMT_FRAME_FILTER];
            (void)memset(mgmt_filter, 0, MAX_MGMT_FRAME_FILTER * sizeof(mgmt_frame_filter));
            mgmt_filter_tlv              = (MrvlIEtypes_MgmtFrameFilter_t *)tlv;
            mgmt_filter_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_MGMT_FRAME_WAKEUP);
            tlv += sizeof(MrvlIEtypesHeader_t);
            while (i < MAX_MGMT_FRAME_FILTER && pmadapter->mgmt_filter[i].type)
            {
                mgmt_filter[i].action     = (t_u8)pmadapter->mgmt_filter[i].action;
                mgmt_filter[i].type       = (t_u8)pmadapter->mgmt_filter[i].type;
                mgmt_filter[i].frame_mask = wlan_cpu_to_le32(pmadapter->mgmt_filter[i].frame_mask);
                i++;
            }
            (void)memcpy((t_u8 *)mgmt_filter_tlv->filter, (t_u8 *)mgmt_filter, i * sizeof(mgmt_frame_filter));
            tlv += i * sizeof(mgmt_frame_filter);
            mgmt_filter_tlv->header.len = wlan_cpu_to_le16(i * sizeof(mgmt_frame_filter));
            cmd->size += i * sizeof(mgmt_frame_filter) + sizeof(MrvlIEtypesHeader_t);
        }
    }

    (void)wifi_wait_for_cmdresp(NULL);
    return (int)status;
}

int wifi_cancel_host_sleep(mlan_bss_type interface)
{
    void *pdata_buf = NULL;
    hs_config_param hs_cfg_obj;

    wifi_get_command_lock();

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    (void)memset(&hs_cfg_obj, 0x00, sizeof(hs_config_param));

    cmd->seq_num          = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, interface);
    hs_cfg_obj.conditions = HOST_SLEEP_CFG_CANCEL;
    pdata_buf             = &hs_cfg_obj;
    mlan_status status    = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_HS_CFG_ENH,
                                                     HostCmd_ACT_GEN_SET, 0, NULL, pdata_buf, cmd);
    wifi_wait_for_cmdresp(NULL);
    return status;
}
#endif

static int wifi_send_power_save_command(ENH_PS_MODES action, t_u16 ps_bitmap, mlan_bss_type interface, void *pdata_buf)
{
    mlan_status status;
    mlan_ds_auto_ds ds_param;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)(interface));
    if (ps_bitmap == BITMAP_AUTO_DS)
    {
        ds_param.idletime = (*(t_u16 *)pdata_buf);
        pdata_buf         = &ds_param;
    }

    cmd->result = 0x0;
    status      = wlan_cmd_enh_power_mode((mlan_private *)mlan_adap->priv[0], cmd, action, (t_u16)ps_bitmap, pdata_buf);

    (void)wifi_wait_for_cmdresp(NULL);
    return (int)status;
}

int wifi_enter_ieee_power_save(void)
{
    return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, NULL);
}

int wifi_exit_ieee_power_save(void)
{
    return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, NULL);
}

#if (CONFIG_WNM_PS)
int wifi_enter_wnm_power_save(t_u16 wnm_sleep_time)
{
    ((mlan_private *)mlan_adap->priv[0])->wnm_set = true;
    t_u16 interval                                = wnm_sleep_time;
    return wifi_send_power_save_command(EN_WNM_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, &interval);
}

int wifi_exit_wnm_power_save(void)
{
    return wifi_send_power_save_command(DIS_WNM_PS, BITMAP_STA_PS, MLAN_BSS_TYPE_STA, NULL);
}
#endif

int wifi_enter_deepsleep_power_save(void)
{
    t_u16 idletime = mlan_adap->idle_time;

    return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_AUTO_DS, MLAN_BSS_TYPE_STA, &idletime);
}

int wifi_exit_deepsleep_power_save(void)
{
    t_u16 idletime = 0;
    return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_AUTO_DS, MLAN_BSS_TYPE_STA, &idletime);
}

int wifi_set_power_save_mode(void)
{
    t_u32 mode = BLOCK_CMD_IN_PRE_ASLEEP;

    return wifi_send_power_save_command(EXT_PS_PARAM, 0U, MLAN_BSS_TYPE_STA, &mode);
}

int wifi_uap_ps_inactivity_sleep_enter(mlan_bss_type type,
                                       unsigned int ctrl_bitmap,
                                       unsigned int min_sleep,
                                       unsigned int max_sleep,
                                       unsigned int inactivity_to,
                                       unsigned int min_awake,
                                       unsigned int max_awake)
{
    mlan_ds_ps_mgmt data_buff;

    (void)memset(&data_buff, 0x00, sizeof(mlan_ds_ps_mgmt));

    data_buff.sleep_param.ctrl_bitmap   = ctrl_bitmap;
    data_buff.sleep_param.min_sleep     = min_sleep;
    data_buff.sleep_param.max_sleep     = max_sleep;
    data_buff.inact_param.inactivity_to = inactivity_to;
    data_buff.inact_param.min_awake     = min_awake;
    data_buff.inact_param.max_awake     = max_awake;
    data_buff.ps_mode                   = PS_MODE_INACTIVITY;
    data_buff.flags                     = PS_FLAG_INACT_SLEEP_PARAM | PS_FLAG_SLEEP_PARAM;

    return wifi_send_power_save_command(EN_AUTO_PS, BITMAP_UAP_INACT_PS, type, &data_buff);
}

int wifi_uap_ps_inactivity_sleep_exit(mlan_bss_type type)
{
    return wifi_send_power_save_command(DIS_AUTO_PS, BITMAP_UAP_INACT_PS, type, NULL);
}

void send_sleep_confirm_command(mlan_bss_type interface)
{
    OPT_Confirm_Sleep *ps_cfm_sleep;

    HostCmd_DS_COMMAND *command = wifi_get_command_buffer();

    (void)wifi_get_command_lock();
    ps_cfm_sleep = (OPT_Confirm_Sleep *)(void *)(command);

    (void)memset(ps_cfm_sleep, 0, sizeof(OPT_Confirm_Sleep));
    ps_cfm_sleep->command = HostCmd_CMD_802_11_PS_MODE_ENH;
    ps_cfm_sleep->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)(interface));

    ps_cfm_sleep->size                = (t_u16)sizeof(OPT_Confirm_Sleep);
    ps_cfm_sleep->result              = 0;
    ps_cfm_sleep->action              = (t_u16)SLEEP_CONFIRM;
    ps_cfm_sleep->sleep_cfm.resp_ctrl = (t_u16)RESP_NEEDED;

    /* If driver did not send out sleep confirm in the expected time,
       FW would switch to awake and send out PS AWAKE event.
       After received AWAKE event when presleep, state would switch to AWAKE.
       So here only send out sleep confirm when state is presleep,
       and would not send out sleep confirm if state has switched to AWAKE */
    if (mlan_adap->ps_state == PS_STATE_PRE_SLEEP)
    {
        mlan_adap->ps_state = PS_STATE_SLEEP_CFM;
#if CONFIG_WIFI_PS_DEBUG
        wcmdr_d("+");
#endif

        /* Write mutex is used to avoid the case that, during waiting for sleep confirm cmd response,
         * wifi_driver_tx task or other tx task might be scheduled and send data to FW */
        (void)OSA_MutexLock((osa_mutex_handle_t)sleep_rwlock.write_mutex, osaWaitForever_c);

        (void)wifi_wait_for_cmdresp(NULL);

        OSA_MutexUnlock((osa_mutex_handle_t)sleep_rwlock.write_mutex);
    }
    else
    {
        (void)wifi_put_command_lock();
    }
}

#ifdef SD9177
void prepare_error_sleep_confirm_command(mlan_bss_type interface)
{
    OPT_Confirm_Sleep *ps_cfm_sleep;
    // Command lock not taken here since it was already taken for previous command and we are not out of loop yet
    HostCmd_DS_COMMAND *command = wifi_get_command_buffer();

    ps_cfm_sleep = (OPT_Confirm_Sleep *)(void *)(command);

    (void)memset(ps_cfm_sleep, 0, sizeof(OPT_Confirm_Sleep));
    ps_cfm_sleep->command = HostCmd_CMD_802_11_PS_MODE_ENH;
    ps_cfm_sleep->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, (t_u8)(interface));

    ps_cfm_sleep->size                = (t_u16)sizeof(OPT_Confirm_Sleep);
    ps_cfm_sleep->result              = 0;
    ps_cfm_sleep->action              = (t_u16)SLEEP_CONFIRM;
    ps_cfm_sleep->sleep_cfm.resp_ctrl = (t_u16)RESP_NEEDED;
}
#endif

#if CONFIG_HOST_SLEEP
/* fixme: accept HostCmd_DS_COMMAND directly */
void wifi_process_hs_cfg_resp(t_u8 *cmd_res_buffer)
{
    pmlan_adapter pmadapter              = ((mlan_private *)mlan_adap->priv[0])->adapter;
    HostCmd_DS_802_11_HS_CFG_ENH *hs_cfg = (HostCmd_DS_802_11_HS_CFG_ENH *)(void *)(cmd_res_buffer + S_DS_GEN);
    if (hs_cfg->action == (t_u16)HS_ACTIVATE)
    {
        pwr_d("Host sleep activated");
        pmadapter->is_hs_configured = MFALSE;
        wlan_update_rxreorder_tbl(pmadapter, MTRUE);
        wifi_event_completion(WIFI_EVENT_HS_ACTIVATED, WIFI_EVENT_REASON_SUCCESS, NULL);
    }
    else
    {
        if (hs_cfg->params.hs_config.conditions != HOST_SLEEP_CFG_CANCEL)
        {
            pmadapter->is_hs_configured = MTRUE;
        }
        pwr_d("Host sleep configuration done");
    }
}
#endif

enum wifi_event_reason wifi_process_ps_enh_response(t_u8 *cmd_res_buffer, t_u16 *ps_event, t_u16 *action)
{
    enum wifi_event_reason result = WIFI_EVENT_REASON_FAILURE;
    MrvlIEtypesHeader_t *mrvl_tlv = NULL;
#if CONFIG_HOST_SLEEP
    pmlan_adapter pmadapter = ((mlan_private *)mlan_adap->priv[0])->adapter;
#endif
    HostCmd_DS_802_11_PS_MODE_ENH *ps_mode = (HostCmd_DS_802_11_PS_MODE_ENH *)(void *)(cmd_res_buffer + S_DS_GEN);

    *ps_event = (t_u16)WIFI_EVENT_PS_INVALID;
    *action   = (t_u16)(ps_mode->action);
    if (ps_mode->action == (t_u16)EN_AUTO_PS)
    {
        if ((ps_mode->params.auto_ps.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Enabled deep sleep mode");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_AUTO_DS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }

            *ps_event           = (t_u16)WIFI_EVENT_DEEP_SLEEP;
            deepsleepps_enabled = true;
        }
        if ((ps_mode->params.auto_ps.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d(
                "Enabled IEEE power "
                "save mode");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_PS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
            pwr_d(
                "pscfg: %u %u %u %u "
                "%u %u",
                ps_tlv->param.null_pkt_interval, ps_tlv->param.multiple_dtims, ps_tlv->param.local_listen_interval,
                ps_tlv->param.bcn_miss_timeout, ps_tlv->param.delay_to_ps, ps_tlv->param.mode);

            *ps_event      = (t_u16)WIFI_EVENT_IEEE_PS;
            ieeeps_enabled = true;
        }
        return WIFI_EVENT_REASON_SUCCESS;
    }
    else if (ps_mode->action == (t_u16)DIS_AUTO_PS)
    {
        if ((ps_mode->params.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Disabled DeepSleep mode");
            *ps_event           = (t_u16)WIFI_EVENT_DEEP_SLEEP;
            deepsleepps_enabled = false;
        }
        if ((ps_mode->params.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d(
                "Disabled IEEE power "
                "save mode");
            *ps_event      = (t_u16)WIFI_EVENT_IEEE_PS;
            ieeeps_enabled = false;
        }
        return WIFI_EVENT_REASON_SUCCESS;
    }
#if (CONFIG_WNM_PS)
    else if (ps_mode->action == EN_WNM_PS)
    {
        if ((ps_mode->params.auto_ps.ps_bitmap & BITMAP_STA_PS) != 0)
        {
            mrvl_tlv = (MrvlIEtypesHeader_t *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            pwr_d("ps_enh_response: bitmap = 0x%x, type = 0x%x\n", ps_mode->params.auto_ps.ps_bitmap, mrvl_tlv->type);
            if (((mlan_private *)mlan_adap->priv[0])->wnm_set == true)
            {
                pwr_d("Enable WNM PS mode, wait for the enable success event");
            }
            else
            {
                /* Do nothing */
            }
        }

        *ps_event = (t_u16)WIFI_EVENT_WNM_PS;
        result    = WIFI_EVENT_REASON_SUCCESS;
    }
    else if (ps_mode->action == DIS_WNM_PS)
    {
        if ((ps_mode->params.ps_bitmap & BITMAP_STA_PS) != 0)
        {
            if (((mlan_private *)mlan_adap->priv[0])->wnm_set == true)
            {
                pwr_d(
                    "Disabled WNM power "
                    "save mode");
                *ps_event                                     = (t_u16)WIFI_EVENT_WNM_PS;
                ((mlan_private *)mlan_adap->priv[0])->wnm_set = false;
            }
            else
            {
                /* Do nothing */
            }
        }

        *ps_event = (t_u16)WIFI_EVENT_WNM_PS;
        return WIFI_EVENT_REASON_SUCCESS;
    }
#endif
    else if (ps_mode->action == (t_u16)GET_PS)
    {
        if ((ps_mode->params.ps_bitmap & BITMAP_AUTO_DS) != 0U)
        {
            pwr_d("Deep sleep mode is on");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_AUTO_DS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
        }
        if ((ps_mode->params.ps_bitmap & BITMAP_STA_PS) != 0U)
        {
            pwr_d("IEEE power save mode is on");
            mrvl_tlv = (MrvlIEtypesHeader_t *)(void *)((uint8_t *)ps_mode + AUTO_PS_FIX_SIZE);
            while (mrvl_tlv->type != TLV_TYPE_PS_PARAM)
            {
                mrvl_tlv =
                    (MrvlIEtypesHeader_t *)(void *)((uint8_t *)mrvl_tlv + mrvl_tlv->len + sizeof(MrvlIEtypesHeader_t));
            }
            pwr_d(
                "pscfg: %u %u %u %u "
                "%u %u\r\n",
                ps_tlv->param.null_pkt_interval, ps_tlv->param.multiple_dtims, ps_tlv->param.local_listen_interval,
                ps_tlv->param.bcn_miss_timeout, ps_tlv->param.delay_to_ps, ps_tlv->param.mode);
        }
    }
    else if (ps_mode->action == (t_u16)SLEEP_CONFIRM)
    {
#if CONFIG_WIFI_PS_DEBUG
        wcmdr_d("#");
#endif

        if ((ieeeps_enabled) && (deepsleepps_enabled))
        {
            *ps_event = (t_u16)WIFI_EVENT_IEEE_DEEP_SLEEP;
        }
#if (CONFIG_WNM_PS)
        else if ((((mlan_private *)mlan_adap->priv[0])->wnm_set) && (deepsleepps_enabled))
        {
            *ps_event = (t_u16)WIFI_EVENT_WNM_DEEP_SLEEP;
        }
#endif
        else if (ieeeps_enabled)
        {
            *ps_event = (t_u16)WIFI_EVENT_IEEE_PS;
        }
        else if (deepsleepps_enabled)
        {
            *ps_event = (t_u16)WIFI_EVENT_DEEP_SLEEP;
        }
#if (CONFIG_WNM_PS)
        else if (((mlan_private *)mlan_adap->priv[0])->wnm_set)
        {
            *ps_event = (t_u16)WIFI_EVENT_WNM_PS;
        }
#endif
        else
        {
            return WIFI_EVENT_REASON_FAILURE;
        }

        if (ieeeps_enabled || deepsleepps_enabled
#if CONFIG_WNM_PS
            || (((mlan_private *)mlan_adap->priv[0])->wnm_set)
#endif
        )
        {
            /* sleep confirm response needs to get the sleep_rwlock, for this lock
             * is an indication that host needs to wakeup FW when reader (cmd/tx)
             * could not get the sleep_rwlock */
            int ret             = OSA_RWLockWriteLock(&sleep_rwlock, osaWaitForever_c);
            mlan_adap->ps_state = PS_STATE_SLEEP;
#if CONFIG_HOST_SLEEP
            wakelock_put();
#endif
            if (ret == WM_SUCCESS)
            {
#if CONFIG_WIFI_PS_DEBUG
                wcmdr_d("Get sleep rw lock successfully");
#endif
            }
            else
            {
#if CONFIG_WIFI_PS_DEBUG
                pwr_e("Failed to get sleep rw lock");
#endif
                return WIFI_EVENT_REASON_FAILURE;
            }
        }
        else
        {
            return WIFI_EVENT_REASON_FAILURE;
        }

        result = WIFI_EVENT_REASON_SUCCESS;
#if CONFIG_HOST_SLEEP
        if (pmadapter->is_hs_configured)
        {
            pwr_d("Host sleep activated");
            pmadapter->is_hs_configured = MFALSE;
            wlan_update_rxreorder_tbl(pmadapter, MTRUE);
            wifi_event_completion(WIFI_EVENT_SLEEP_CONFIRM_DONE, result, NULL);
        }
#endif
    }
    else
    { /* Do Nothing */
    }

    return result;
}

#if CONFIG_HOST_SLEEP
int wifi_get_wakeup_reason(t_u16 *hs_wakeup_reason)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->result  = 0x0;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_HS_WAKEUP_REASON, HostCmd_ACT_GEN_GET, 0,
                             NULL, hs_wakeup_reason, cmd);
    wifi_wait_for_cmdresp(hs_wakeup_reason);
    return WM_SUCCESS;
}
#endif

