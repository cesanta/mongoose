/** @file mlan_api.c
 *
 *  @brief This file provides more APIs for mlan.
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <mlan_api.h>
#if CONFIG_COMBO_SCAN
#include <string.h>
#endif
/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

#include <wifi.h>

#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#include "wifi-internal.h"

#if CONFIG_DRIVER_MBO
#include "mlan_mbo.h"
#endif

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>

#if (CONFIG_11MC) || (CONFIG_11AZ)
#if CONFIG_WLS_CSI_PROC
#include <wls_param_defines.h>
#include <wls_api.h>
#include <wls_structure_defs.h>
#include <range_kalman.h>
#endif
#endif

static const char driver_version_format[] = "SD878x-%s-%s-WM";
static const char driver_version[]        = "702.1.0";

static unsigned int mgmt_ie_index_bitmap = 0x0000000F;

#if (CONFIG_11MC) || (CONFIG_11AZ)
ftm_start_param ftm_param;
#if CONFIG_WLS_CSI_PROC
#define NL_MAX_PAYLOAD (3 * 1024)
unsigned int csi_res_array[8];
uint32_t wls_data[WLS_CSI_DATA_LEN_DW];
range_kalman_state range_input_str = {0};
#define RANGE_DRIVE_VAR       1e-5f // in meter/(s^2)
#define RANGE_MEASUREMENT_VAR 4e-2f // in meter^2
#define RANGE_RATE_INIT       1e-3f // in (meter/s)^2
#define CSI_TSF_LEN           6 * sizeof(uint32_t)
#define FFT_INBUFFER_LEN_DW   (MAX_RX * MAX_TX + NUM_PROC_BUF) * (MAX_IFFT_SIZE_CSI)
uint32_t fftInBuffer_t[FFT_INBUFFER_LEN_DW];
#endif
#endif

/* This were static functions in mlan file */
mlan_status wlan_cmd_802_11_deauthenticate(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);
mlan_status wlan_cmd_reg_access(IN HostCmd_DS_COMMAND *cmd, IN t_u16 cmd_action, IN t_void *pdata_buf);
mlan_status wlan_cmd_mem_access(IN HostCmd_DS_COMMAND *cmd, IN t_u16 cmd_action, IN t_void *pdata_buf);
mlan_status wlan_cmd_auto_reconnect(IN HostCmd_DS_COMMAND *cmd, IN t_u16 cmd_action, IN t_void *pdata_buf);
mlan_status wlan_misc_ioctl_region(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req);

int wifi_set_mac_multicast_addr(const char *mlist, t_u32 num_of_addr);
int wifi_send_disable_supplicant(int mode);
int wifi_send_rf_channel_cmd(wifi_rf_channel_t *rf_channel);
int wifi_get_set_rf_tx_power(t_u16 cmd_action, wifi_tx_power_t *tx_power);

int wifi_send_shutdown_cmd()
{
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    wifi_get_command_lock();

    cmd->command = HostCmd_CMD_FUNC_SHUTDOWN;
    cmd->size    = S_DS_GEN;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_deauthenticate(uint8_t *bssid)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    if (pmpriv->media_connected == MFALSE)
    {
        return WM_SUCCESS;
    }

    (void)wifi_get_command_lock();

    /* fixme: check if this static selection is ok */
    cmd->seq_num                      = 0x0;
    cmd->result = 0x0;

    (void)wlan_cmd_802_11_deauthenticate((mlan_private *)mlan_adap->priv[0], cmd, bssid);
    (void)wifi_wait_for_cmdresp(NULL);

    return WM_SUCCESS;
}

#if CONFIG_WPA_SUPP
#define REASON_CODE_PEER_STA_LEAVING 36
int wifi_nxp_deauthenticate(unsigned int bss_type, const uint8_t *bssid, uint16_t reason_code)
{
    mlan_private *pmpriv                      = (mlan_private *)mlan_adap->priv[bss_type];
    HostCmd_DS_COMMAND *cmd                   = wifi_get_command_buffer();
    HostCmd_DS_802_11_DEAUTHENTICATE *pdeauth = &cmd->params.deauth;

    pmpriv->curr_bss_params.host_mlme = 0;
    pmpriv->auth_flag                 = 0;
    pmpriv->auth_alg                  = 0xFFFF;

    if (pmpriv->media_connected == MFALSE)
    {
        return WM_SUCCESS;
    }

    (void)wifi_get_command_lock();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_DEAUTHENTICATE);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_802_11_DEAUTHENTICATE) + S_DS_GEN);
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;

    /* Set AP MAC address */ (void)memcpy((void *)pdeauth->mac_addr, (const void *)bssid, (size_t)MLAN_MAC_ADDR_LENGTH);

    wifi_d("Deauth: %02x:%02x:%02x:%02x:%02x:%02x", pdeauth->mac_addr[0], pdeauth->mac_addr[1], pdeauth->mac_addr[2],
           pdeauth->mac_addr[3], pdeauth->mac_addr[4], pdeauth->mac_addr[5]);

    if (pmpriv->adapter->state_11h.recvd_chanswann_event)
    { /** Reason code 36 = Requested from peer station as it is leaving the BSS */
        pdeauth->reason_code = wlan_cpu_to_le16(REASON_CODE_PEER_STA_LEAVING);
    }
    else
    {
        pdeauth->reason_code = wlan_cpu_to_le16(reason_code);
    }
    (void)wifi_wait_for_cmdresp(NULL);

    return WM_SUCCESS;
}
#endif

int wifi_get_eeprom_data(uint32_t offset, uint32_t byte_count, uint8_t *buf)
{
    mlan_ds_read_eeprom eeprom_rd;
    eeprom_rd.offset     = offset;
    eeprom_rd.byte_count = byte_count;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = HostCmd_CMD_802_11_EEPROM_ACCESS;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_cmd_reg_access(cmd, HostCmd_ACT_GEN_GET, &eeprom_rd);
    (void)wifi_wait_for_cmdresp(buf);
    return wm_wifi.cmd_resp_status;
}

int wifi_reg_access(wifi_reg_t reg_type, uint16_t action, uint32_t offset, uint32_t *value)
{
    mlan_ds_reg_rw reg_rw;
    reg_rw.offset = offset;
    reg_rw.value  = *value;
    uint16_t hostcmd;
    int ret = WM_SUCCESS;
    switch (reg_type)
    {
        case REG_MAC:
            hostcmd = HostCmd_CMD_MAC_REG_ACCESS;
            break;
        case REG_BBP:
            hostcmd = HostCmd_CMD_BBP_REG_ACCESS;
            break;
        case REG_RF:
            hostcmd = HostCmd_CMD_RF_REG_ACCESS;
            break;
        case REG_CAU:
            hostcmd = HostCmd_CMD_CAU_REG_ACCESS;
            break;
        default:
            wifi_e("Incorrect register type");
            ret = -WM_FAIL;
            break;
    }

    if (ret != WM_SUCCESS)
    {
        return ret;
    }

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = hostcmd;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_cmd_reg_access(cmd, action, &reg_rw);
    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? value : NULL);
    return wm_wifi.cmd_resp_status;
}

int wifi_mem_access(uint16_t action, uint32_t addr, uint32_t *value)
{
    mlan_ds_mem_rw mem_rw;
    mem_rw.addr  = addr;
    mem_rw.value = *value;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = HostCmd_CMD_MEM_ACCESS;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_cmd_mem_access(cmd, action, &mem_rw);

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? value : NULL);
    return wm_wifi.cmd_resp_status;
}

#if CONFIG_WIFI_BOOT_SLEEP
int wifi_boot_sleep(uint16_t action, uint16_t *enable)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = HostCmd_CMD_BOOT_SLEEP;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)wlan_cmd_boot_sleep(pmpriv, cmd, action, enable);

    (void)wifi_wait_for_cmdresp(enable);
    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_AUTO_RECONNECT
static int wifi_auto_reconnect(uint16_t action, wifi_auto_reconnect_config_t *auto_reconnect_config)
{
    mlan_ds_auto_reconnect auto_reconnect;

    (void)memset(&auto_reconnect, 0x00, sizeof(mlan_ds_auto_reconnect));

    if (auto_reconnect_config)
    {
        auto_reconnect.reconnect_counter  = auto_reconnect_config->reconnect_counter;
        auto_reconnect.reconnect_interval = auto_reconnect_config->reconnect_interval;
        auto_reconnect.flags              = auto_reconnect_config->flags;
    }

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = HostCmd_CMD_AUTO_RECONNECT;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    wlan_cmd_auto_reconnect(cmd, action, &auto_reconnect);

    wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? auto_reconnect_config : NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_auto_reconnect_enable(wifi_auto_reconnect_config_t auto_reconnect_config)
{
    return wifi_auto_reconnect(HostCmd_ACT_GEN_SET, &auto_reconnect_config);
}

int wifi_auto_reconnect_disable(void)
{
    return wifi_auto_reconnect(HostCmd_ACT_GEN_SET, NULL);
}

int wifi_get_auto_reconnect_config(wifi_auto_reconnect_config_t *auto_reconnect_config)
{
    return wifi_auto_reconnect(HostCmd_ACT_GEN_GET, auto_reconnect_config);
}
#endif

int wifi_get_tsf(uint32_t *tsf_high, uint32_t *tsf_low)
{
    t_u64 tsf = 0x00;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_GET_TSF,
                                              HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(&tsf);

    *tsf_high = tsf >> 32;
    *tsf_low  = (t_u32)tsf;

    return wm_wifi.cmd_resp_status;
}


int wifi_send_rssi_info_cmd(wifi_rssi_info_t *rssi_info)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_RSSI_INFO,
                                              HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(rssi_info);
    return wm_wifi.cmd_resp_status;
}

int wifi_send_rf_channel_cmd(wifi_rf_channel_t *rf_channel)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    /*
      SET operation is not supported according to spec. So we are
      sending NULL as one param below.
    */
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_RF_CHANNEL,
                                              HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(rf_channel);
    return wm_wifi.cmd_resp_status;
}

int wifi_send_remain_on_channel_cmd(unsigned int bss_type, wifi_remain_on_channel_t *remain_on_channel)
{
    CHECK_BSS_TYPE(bss_type, -WM_FAIL);
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, bss_type);
    cmd->result  = 0x0;
    /*save remain on channel bss index*/
    mlan_adap->remain_bss_index = mlan_adap->priv[bss_type]->bss_index;
    mlan_status rv =
        wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[bss_type], HostCmd_CMD_802_11_REMAIN_ON_CHANNEL,
                                 HostCmd_ACT_GEN_SET, 0, NULL, remain_on_channel, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(NULL);
    return wm_wifi.cmd_resp_status;
}

/* power_level is not used when cmd_action is GET */
int wifi_get_set_rf_tx_power(t_u16 cmd_action, wifi_tx_power_t *tx_power)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_RF_TX_POWER,
                                              cmd_action, 0, NULL, &tx_power->current_level, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(cmd_action == HostCmd_ACT_GEN_GET ? tx_power : NULL);
    return wm_wifi.cmd_resp_status;
}

int wifi_get_data_rate(wifi_ds_rate *ds_rate, mlan_bss_type bss_type)
{
    CHECK_BSS_TYPE(bss_type, -WM_FAIL);
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;

    mlan_status rv = MLAN_STATUS_SUCCESS;
    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        if (is_uap_started())
            rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[1], HostCmd_CMD_802_11_TX_RATE_QUERY, 0, 0,
                                          NULL, NULL, cmd);
        else
            wifi_e("uap isn't up\n\r");
    }
    else if (bss_type == MLAN_BSS_TYPE_STA)
    {
        if (is_sta_associated())
            rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_TX_RATE_QUERY, 0, 0,
                                          NULL, NULL, cmd);
        else
            wifi_e("sta connection required before setting tx rate\n\r");
    }

    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_put_command_lock();
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(ds_rate);
    return wm_wifi.cmd_resp_status;
}

int wifi_set_pmfcfg(t_u8 mfpc, t_u8 mfpr)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    pmpriv->pmfcfg.mfpc = mfpc;
    pmpriv->pmfcfg.mfpr = mfpr;

    return WM_SUCCESS;
}

int wifi_get_pmfcfg(t_u8 *mfpc, t_u8 *mfpr)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    *mfpc = pmpriv->pmfcfg.mfpc;
    *mfpr = pmpriv->pmfcfg.mfpr;

    return WM_SUCCESS;
}

int wifi_set_packet_filters(wifi_flt_cfg_t *flt_cfg)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    HostCmd_DS_MEF_CFG *mef_hdr;
    mef_entry_header *entry_hdr;
    t_u8 *buf = (t_u8 *)cmd, *filter_buf = NULL;
    t_u32 buf_len;
    int i, j;
    mef_op op;
    t_u32 dnum;

    (void)memset(cmd, 0, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_MEF_CFG);
    buf_len      = S_DS_GEN;

    /** Fill HostCmd_DS_MEF_CFG */
    mef_hdr           = (HostCmd_DS_MEF_CFG *)(void *)(buf + buf_len);
    mef_hdr->criteria = wlan_cpu_to_le32(flt_cfg->criteria);
    mef_hdr->nentries = wlan_cpu_to_le16(flt_cfg->nentries);
    buf_len += sizeof(HostCmd_DS_MEF_CFG);

    for (i = 0; i < flt_cfg->nentries; i++)
    {
        /** Fill entry header data */
        entry_hdr         = (mef_entry_header *)(buf + buf_len);
        entry_hdr->mode   = flt_cfg->mef_entry[i].mode;
        entry_hdr->action = flt_cfg->mef_entry[i].action;
        buf_len += sizeof(mef_entry_header);
        for (j = 0; j < flt_cfg->mef_entry[i].filter_num; j++)
        {
            if (flt_cfg->mef_entry[i].filter_item[j].type == TYPE_DNUM_EQ)
            {
                /* Format of decimal num:
                 * |   5 bytes  |    5 bytes    |    5 bytes    |        1 byte         |
                 * |   pattern  |     offset    |  num of bytes |  type (TYPE_DNUM_EQ)  |
                 */
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push pattern */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].pattern;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push offset */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].offset;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push num of bytes */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].num_bytes;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push type */
                op.operand_type = TYPE_DNUM_EQ;
                (void)memcpy(filter_buf, &(op.operand_type), 1);
                buf_len += 1;
                filter_buf = (t_u8 *)(buf + buf_len);
            }
            else if (flt_cfg->mef_entry[i].filter_item[j].type == TYPE_BYTE_EQ)
            {
                /* Format of byte seq:
                 * |   5 bytes  |      val      |    5 bytes    |        1 byte         |
                 * |   repeat   |   bytes seq   |    offset     |  type (TYPE_BYTE_EQ)  |
                 */
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push repeat */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].repeat;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push bytes seq */
                op.operand_type = OPERAND_BYTE_SEQ;
                (void)memcpy(filter_buf, flt_cfg->mef_entry[i].filter_item[j].byte_seq,
                             flt_cfg->mef_entry[i].filter_item[j].num_byte_seq);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_byte_seq,
                             &(flt_cfg->mef_entry[i].filter_item[j].num_byte_seq), 1);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_byte_seq + 1, &(op.operand_type), 1);
                buf_len += flt_cfg->mef_entry[i].filter_item[j].num_byte_seq + 2;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push offset */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].offset;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push type */
                op.operand_type = TYPE_BYTE_EQ;
                (void)memcpy(filter_buf, &(op.operand_type), 1);
                buf_len += 1;
                filter_buf = (t_u8 *)(buf + buf_len);
            }
            else if (flt_cfg->mef_entry[i].filter_item[j].type == TYPE_BIT_EQ)
            {
                /* Format of bit seq:
                 * |   val      |    5 bytes    |      val      |        1 byte         |
                 * | bytes seq  |    offset     |    mask seq   |  type (TYPE_BIT_EQ)   |
                 */
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push bytes seq */
                op.operand_type = OPERAND_BYTE_SEQ;
                (void)memcpy(filter_buf, flt_cfg->mef_entry[i].filter_item[j].byte_seq,
                             flt_cfg->mef_entry[i].filter_item[j].num_byte_seq);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_byte_seq,
                             &(flt_cfg->mef_entry[i].filter_item[j].num_byte_seq), 1);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_byte_seq + 1, &(op.operand_type), 1);
                buf_len += flt_cfg->mef_entry[i].filter_item[j].num_byte_seq + 2;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push offset */
                op.operand_type = OPERAND_DNUM;
                dnum            = flt_cfg->mef_entry[i].filter_item[j].offset;
                (void)memcpy(filter_buf, &dnum, sizeof(dnum));
                (void)memcpy(filter_buf + sizeof(dnum), &(op.operand_type), 1);
                buf_len += sizeof(dnum) + 1;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push mask seq */
                op.operand_type = OPERAND_BYTE_SEQ;
                (void)memcpy(filter_buf, flt_cfg->mef_entry[i].filter_item[j].mask_seq,
                             flt_cfg->mef_entry[i].filter_item[j].num_mask_seq);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_mask_seq,
                             &(flt_cfg->mef_entry[i].filter_item[j].num_mask_seq), 1);
                (void)memcpy(filter_buf + flt_cfg->mef_entry[i].filter_item[j].num_mask_seq + 1, &(op.operand_type), 1);
                buf_len += flt_cfg->mef_entry[i].filter_item[j].num_mask_seq + 2;
                filter_buf = (t_u8 *)(buf + buf_len);

                /* push type */
                op.operand_type = TYPE_BIT_EQ;
                (void)memcpy(filter_buf, &(op.operand_type), 1);
                buf_len += 1;
                filter_buf = (t_u8 *)(buf + buf_len);
            }
            else
                goto done;
            if (j != 0)
            {
                filter_buf      = (t_u8 *)(buf + buf_len);
                op.operand_type = flt_cfg->mef_entry[i].rpn[j];
                (void)memcpy(filter_buf, &(op.operand_type), 1);
                buf_len += 1;
                filter_buf = (t_u8 *)(buf + buf_len);
            }
        }
        if (filter_buf != NULL)
            entry_hdr->len = (t_u32)filter_buf - (t_u32)entry_hdr - sizeof(mef_entry_header);
    }

    cmd->size = wlan_cpu_to_le16(buf_len);
done:
    (void)wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}

#define FLTR_BUF_IP_OFFSET 24

int wifi_set_auto_arp(t_u32 *ipv4_addr)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    HostCmd_DS_MEF_CFG *mef_hdr;
    t_u8 *buf = (t_u8 *)cmd, *filter = NULL;
    t_u32 buf_len;
    t_u8 fltr_buf[] = {0x01, 0x10, 0x21, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x08, 0x06, 0x02, 0x02,
                       0x14, 0x00, 0x00, 0x00, 0x01, 0x41, 0x01, 0x00, 0x00, 0x00, 0x01, 0xc0, 0xa8,
                       0x01, 0x6d, 0x04, 0x02, 0x2e, 0x00, 0x00, 0x00, 0x01, 0x41, 0x44};

    (void)memset(cmd, 0, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_MEF_CFG);
    buf_len      = S_DS_GEN;

    /** Fill HostCmd_DS_MEF_CFG*/
    mef_hdr           = (HostCmd_DS_MEF_CFG *)(buf + buf_len);
    mef_hdr->criteria = wlan_cpu_to_le32(MBIT(0) | MBIT(1) | MBIT(3));
    mef_hdr->nentries = wlan_cpu_to_le16(1);
    buf_len += sizeof(HostCmd_DS_MEF_CFG);

    filter = buf + buf_len;
    (void)memcpy((void *)filter, (const void *)fltr_buf, sizeof(fltr_buf));
    (void)memcpy((void *)&filter[FLTR_BUF_IP_OFFSET], (const void *)ipv4_addr, sizeof(t_u32));
    buf_len += sizeof(fltr_buf);

    cmd->size = wlan_cpu_to_le16(buf_len);
    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_tcp_keep_alive(wifi_tcp_keep_alive_t *keep_alive, t_u8 *src_mac, t_u32 src_ip)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    t_u16 cmd_action        = HostCmd_ACT_GEN_SET;

    HostCmd_DS_AUTO_TX *auto_tx_cmd = (HostCmd_DS_AUTO_TX *)((t_u8 *)cmd + S_DS_GEN);
    t_u8 *pos                       = (t_u8 *)auto_tx_cmd + sizeof(auto_tx_cmd->action);
    t_u16 len                       = 0;

    MrvlIEtypes_Cloud_Keep_Alive_t *keep_alive_tlv = MNULL;
    MrvlIEtypes_Keep_Alive_Ctrl_t *ctrl_tlv        = MNULL;
    MrvlIEtypes_Keep_Alive_Pkt_t *pkt_tlv          = MNULL;
    t_u8 eth_ip[]                                  = {0x08, 0x00};
    t_u8 ip_packet[67] = {0x45, 0x00, 0x00, 0x43, 0x8c, 0x9e, 0x00, 0x00, 0xff, 0x06, 0xac, 0xbf, 0xc0, 0xa8,
                          0x00, 0x7c, 0xc0, 0xa8, 0x00, 0x8a, 0xc0, 0x03, 0x22, 0xb7, 0xb0, 0xb6, 0x60, 0x9f,
                          0x42, 0xdd, 0x9e, 0x1e, 0x50, 0x18, 0x80, 0x00, 0xd0, 0x88, 0x00, 0x00, 0x74, 0x68,
                          0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x6b, 0x65, 0x65, 0x70, 0x20, 0x61,
                          0x6c, 0x69, 0x76, 0x65, 0x20, 0x70, 0x61, 0x63, 0x6b, 0x65, 0x74};
#if 0
	t_u8 ip_packet2[41] = {0x45, 0x00, 0x00, 0x29, 0x76, 0x51, 0x40, 0x00, 0x80, 0x06, 0xf2, 0x4c, 0xc0, 0xa8, 0x01, 0x0a, 0xc0, 0xa8, 0x01, 0x0c, 0xfb, 0xd8, 0x01, 0xbd, 0x76, 0xe3, 0x34, 0x62, 0x06, 0x80, 0x8b, 0x62, 0x50, 0x10, 0x01, 0x00, 0xe1, 0xe4, 0x00, 0x00, 0x00};
#endif
    t_u16 pkt_len = 67;

    if (keep_alive->reset)
        cmd_action = HostCmd_ACT_GEN_RESET;

    cmd->command        = wlan_cpu_to_le16(HostCmd_CMD_AUTO_TX);
    cmd->size           = S_DS_GEN + sizeof(HostCmd_DS_AUTO_TX);
    auto_tx_cmd->action = wlan_cpu_to_le16(cmd_action);

    keep_alive_tlv = (MrvlIEtypes_Cloud_Keep_Alive_t *)pos;

    keep_alive_tlv->header.type   = wlan_cpu_to_le16(TLV_TYPE_CLOUD_KEEP_ALIVE);
    keep_alive_tlv->keep_alive_id = 1; // keep_alive->mkeep_alive_id;
    keep_alive_tlv->enable        = keep_alive->enable;
    len                           = len + sizeof(keep_alive_tlv->keep_alive_id) + sizeof(keep_alive_tlv->enable);
    pos                           = pos + len + sizeof(MrvlIEtypesHeader_t);
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        if (keep_alive->enable)
        {
            ctrl_tlv              = (MrvlIEtypes_Keep_Alive_Ctrl_t *)pos;
            ctrl_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_CTRL);
            ctrl_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t) - sizeof(MrvlIEtypesHeader_t));
            ctrl_tlv->snd_interval   = wlan_cpu_to_le32(keep_alive->timeout);
            ctrl_tlv->retry_interval = wlan_cpu_to_le16(keep_alive->interval);
            ctrl_tlv->retry_count    = wlan_cpu_to_le16(keep_alive->max_keep_alives);
            len                      = len + sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t);

            pos                  = pos + sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t);
            pkt_tlv              = (MrvlIEtypes_Keep_Alive_Pkt_t *)pos;
            pkt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_PKT);
            (void)memcpy((void *)pkt_tlv->eth_header.dest_addr, (const void *)keep_alive->dst_mac,
                         MLAN_MAC_ADDR_LENGTH);
            (void)memcpy((void *)pkt_tlv->eth_header.src_addr, (const void *)src_mac, MLAN_MAC_ADDR_LENGTH);
            (void)memcpy((void *)((t_u8 *)&pkt_tlv->eth_header.h803_len), (const void *)eth_ip, sizeof(t_u16));
            (void)memcpy((void *)(ip_packet + 12), (const void *)&src_ip, sizeof(t_u32));
            (void)memcpy((void *)(ip_packet + 16), (const void *)&keep_alive->dst_ip, sizeof(t_u32));
            (void)memcpy((void *)pkt_tlv->ip_packet, (const void *)ip_packet, pkt_len);
            pkt_tlv->header.len = wlan_cpu_to_le16(sizeof(Eth803Hdr_t) + pkt_len);
            len                 = len + sizeof(MrvlIEtypesHeader_t) + sizeof(Eth803Hdr_t) + pkt_len;
        }
        else
        {
            pkt_tlv              = (MrvlIEtypes_Keep_Alive_Pkt_t *)pos;
            pkt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_PKT);
            pkt_tlv->header.len  = 0;
            len                  = len + sizeof(MrvlIEtypesHeader_t);
        }
    }
    if (cmd_action == HostCmd_ACT_GEN_RESET)
    {
        pkt_tlv              = (MrvlIEtypes_Keep_Alive_Pkt_t *)pos;
        pkt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_PKT);
        pkt_tlv->header.len  = 0;
        len                  = len + sizeof(MrvlIEtypesHeader_t);
    }
    keep_alive_tlv->header.len = wlan_cpu_to_le16(len);

    cmd->size = cmd->size + len + sizeof(MrvlIEtypesHeader_t);
    cmd->size = wlan_cpu_to_le16(cmd->size);

    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}


#if CONFIG_CLOUD_KEEP_ALIVE
int wifi_cloud_keep_alive(wifi_cloud_keep_alive_t *keep_alive, t_u16 action, t_u8 *enable)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    t_u16 cmd_action;
    HostCmd_DS_AUTO_TX *auto_tx_cmd                = (HostCmd_DS_AUTO_TX *)((t_u8 *)cmd + S_DS_GEN);
    t_u8 *pos                                      = (t_u8 *)auto_tx_cmd + sizeof(auto_tx_cmd->action);
    t_u16 len                                      = 0;
    MrvlIEtypes_Cloud_Keep_Alive_t *keep_alive_tlv = MNULL;

    if (keep_alive == NULL)
        return -WM_E_INVAL;

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_STA);
    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_AUTO_TX);
    cmd->size    = S_DS_GEN + sizeof(HostCmd_DS_AUTO_TX);

    if ((action == MLAN_ACT_GET) && (enable != NULL))
    {
        cmd_action = HostCmd_ACT_GEN_GET;
    }
    else if ((action == MLAN_ACT_SET) && !keep_alive->reset)
    {
        cmd_action = HostCmd_ACT_GEN_SET;
    }
    else if ((action == MLAN_ACT_SET) && keep_alive->reset)
    {
        cmd_action = HostCmd_ACT_GEN_RESET;
    }
    else
    {
        return -WM_E_INVAL;
    }
    auto_tx_cmd->action = wlan_cpu_to_le16(cmd_action);

    keep_alive_tlv = (MrvlIEtypes_Cloud_Keep_Alive_t *)pos;

    keep_alive_tlv->header.type   = wlan_cpu_to_le16(TLV_TYPE_CLOUD_KEEP_ALIVE);
    keep_alive_tlv->keep_alive_id = keep_alive->mkeep_alive_id;
    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        keep_alive_tlv->enable     = MFALSE;
        len                        = len + sizeof(keep_alive_tlv->keep_alive_id) + sizeof(keep_alive_tlv->enable);
        keep_alive_tlv->header.len = wlan_cpu_to_le16(len);

        cmd->size = cmd->size + len + sizeof(MrvlIEtypesHeader_t);
        cmd->size = wlan_cpu_to_le16(cmd->size);
    }
    else if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        if (keep_alive->enable)
        {
            MrvlIEtypes_Keep_Alive_Ctrl_t *ctrl_tlv = MNULL;
            MrvlIEtypes_Keep_Alive_Pkt_t *pkt_tlv   = MNULL;
            t_u8 eth_ip[]                           = {0x08, 0x00};

            keep_alive_tlv->enable = keep_alive->enable;
            len                    = len + sizeof(keep_alive_tlv->keep_alive_id) + sizeof(keep_alive_tlv->enable);
            pos                    = pos + len + sizeof(MrvlIEtypesHeader_t);
            ctrl_tlv               = (MrvlIEtypes_Keep_Alive_Ctrl_t *)pos;
            ctrl_tlv->header.type  = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_CTRL);
            ctrl_tlv->header.len =
                wlan_cpu_to_le16(sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t) - sizeof(MrvlIEtypesHeader_t));
            ctrl_tlv->snd_interval   = wlan_cpu_to_le32(keep_alive->send_interval);
            ctrl_tlv->retry_interval = wlan_cpu_to_le16(keep_alive->retry_interval);
            ctrl_tlv->retry_count    = wlan_cpu_to_le16(keep_alive->retry_count);
            len                      = len + sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t);

            pos                  = pos + sizeof(MrvlIEtypes_Keep_Alive_Ctrl_t);
            pkt_tlv              = (MrvlIEtypes_Keep_Alive_Pkt_t *)pos;
            pkt_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_KEEP_ALIVE_PKT);
            (void)memcpy(pkt_tlv->eth_header.dest_addr, keep_alive->dst_mac, MLAN_MAC_ADDR_LENGTH);
            (void)memcpy(pkt_tlv->eth_header.src_addr, keep_alive->src_mac, MLAN_MAC_ADDR_LENGTH);
            (void)memcpy((t_u8 *)&pkt_tlv->eth_header.h803_len, eth_ip, sizeof(t_u16));
            (void)memcpy(pkt_tlv->ip_packet, keep_alive->packet, keep_alive->pkt_len);
            pkt_tlv->header.len        = wlan_cpu_to_le16(sizeof(Eth803Hdr_t) + keep_alive->pkt_len);
            len                        = len + sizeof(MrvlIEtypesHeader_t) + sizeof(Eth803Hdr_t) + keep_alive->pkt_len;
            keep_alive_tlv->header.len = wlan_cpu_to_le16(len);

            cmd->size = cmd->size + len + sizeof(MrvlIEtypesHeader_t);
            cmd->size = wlan_cpu_to_le16(cmd->size);
        }
        else
        {
            keep_alive_tlv->enable     = MFALSE;
            len                        = len + sizeof(keep_alive_tlv->keep_alive_id) + sizeof(keep_alive_tlv->enable);
            keep_alive_tlv->header.len = wlan_cpu_to_le16(len);

            cmd->size = cmd->size + len + sizeof(MrvlIEtypesHeader_t);
            cmd->size = wlan_cpu_to_le16(cmd->size);
        }
    }
    else if (cmd_action == HostCmd_ACT_GEN_RESET)
    {
        keep_alive_tlv->enable     = MFALSE;
        len                        = len + sizeof(keep_alive_tlv->keep_alive_id) + sizeof(keep_alive_tlv->enable);
        keep_alive_tlv->header.len = wlan_cpu_to_le16(len);

        cmd->size = cmd->size + len + sizeof(MrvlIEtypesHeader_t);
        cmd->size = wlan_cpu_to_le16(cmd->size);
    }

    cmd->result = 0x00;
    if (cmd_action == HostCmd_ACT_GEN_GET)
    {
        wifi_wait_for_cmdresp((void *)enable);
    }
    else
    {
        wifi_wait_for_cmdresp(NULL);
    }

    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_RF_TEST_MODE
#ifdef RW610
/* 802.11n/a/g/b data rate IDs */
#define DATARATE_1M  0x0001
#define DATARATE_2M  0x0002
#define DATARATE5_5M 0x0003
#define DATARATE_11M 0x0004
#define RESERVED_1   0x0005

#define DATARATE_6M  0x0006
#define DATARATE_9M  0x0007
#define DATARATE_12M 0x0008
#define DATARATE_18M 0x0009
#define DATARATE_24M 0x000A
#define DATARATE_36M 0x000B
#define DATARATE_48M 0x000C
#define DATARATE_54M 0x000D
#define RESERVED_2   0x000E

#define HT_MCS0 0x000F
#define HT_MCS1 0x0010
#define HT_MCS2 0x0011
#define HT_MCS3 0x0012
#define HT_MCS4 0x0013
#define HT_MCS5 0x0014
#define HT_MCS6 0x0015
#define HT_MCS7 0x0016

/* 802.11ac VHT MCS rates ID */
#define VHT_SS1_MCS0 0x1100
#define VHT_SS1_MCS1 0x1101
#define VHT_SS1_MCS2 0x1102
#define VHT_SS1_MCS3 0x1103
#define VHT_SS1_MCS4 0x1104
#define VHT_SS1_MCS5 0x1105
#define VHT_SS1_MCS6 0x1106
#define VHT_SS1_MCS7 0x1107
#define VHT_SS1_MCS8 0x1108

/* 802.11ax HE MCS rates ID */
#define HE_SS1_MCS0 0x2100
#define HE_SS1_MCS1 0x2101
#define HE_SS1_MCS2 0x2102
#define HE_SS1_MCS3 0x2103
#define HE_SS1_MCS4 0x2104
#define HE_SS1_MCS5 0x2105
#define HE_SS1_MCS6 0x2106
#define HE_SS1_MCS7 0x2107
#define HE_SS1_MCS8 0x2108
#define HE_SS1_MCS9 0x2109

static uint32_t tx_data_rate_ids[] = {
    /* 802.11n/a/g/b data rate IDs */
    DATARATE_1M, DATARATE_2M, DATARATE5_5M, DATARATE_11M, RESERVED_1, DATARATE_6M, DATARATE_9M, DATARATE_12M,
    DATARATE_18M, DATARATE_24M, DATARATE_36M, DATARATE_48M, DATARATE_54M, RESERVED_2, HT_MCS0, HT_MCS1, HT_MCS2,
    HT_MCS3, HT_MCS4, HT_MCS5, HT_MCS6, HT_MCS7,
    /* 802.11ac VHT MCS rates id */
    VHT_SS1_MCS0, VHT_SS1_MCS1, VHT_SS1_MCS2, VHT_SS1_MCS3, VHT_SS1_MCS4, VHT_SS1_MCS5, VHT_SS1_MCS6, VHT_SS1_MCS7,
    VHT_SS1_MCS8,
    /* 802.11ax HE MCS rates ID */
    HE_SS1_MCS0, HE_SS1_MCS1, HE_SS1_MCS2, HE_SS1_MCS3, HE_SS1_MCS4, HE_SS1_MCS5, HE_SS1_MCS6, HE_SS1_MCS7, HE_SS1_MCS8,
    HE_SS1_MCS9};
#endif

static uint8_t band_set       = 0;
static uint8_t bandwidth_set  = 0;
static uint8_t tx_antenna_set = 0;
static uint8_t rx_antenna_set = 0;

int wifi_get_set_rf_test_generic(t_u16 cmd_action, wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    mlan_ds_misc_cfg misc;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_generic_cfg, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_generic_cfg, (wifi_mfg_cmd_generic_cfg_t *)&misc.param.mfg_generic_cfg,
           sizeof(wifi_mfg_cmd_generic_cfg_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_test_tx_frame(t_u16 cmd_action,
                                  wifi_mfg_cmd_tx_frame_t *wifi_mfg_cmd_tx_frame,
                                  wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg misc;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_tx_frame, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_generic_cfg, (wifi_mfg_cmd_generic_cfg_t *)&misc.param.mfg_generic_cfg,
           sizeof(wifi_mfg_cmd_generic_cfg_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_trigger_frame_cfg(t_u16 cmd_action,
                                      wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr_t *wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr,
                                      wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg misc;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_generic_cfg, (wifi_mfg_cmd_generic_cfg_t *)&misc.param.mfg_generic_cfg,
           sizeof(wifi_mfg_cmd_generic_cfg_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_he_tb_tx(t_u16 cmd_action,
                             wifi_mfg_cmd_he_tb_tx_t *wifi_mfg_cmd_he_tb_tx,
                             wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg misc;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_he_tb_tx, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_generic_cfg, (wifi_mfg_cmd_generic_cfg_t *)&misc.param.mfg_generic_cfg,
           sizeof(wifi_mfg_cmd_generic_cfg_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_otp_mac_addr(t_u16 cmd_action, wifi_mfg_cmd_otp_mac_addr_rd_wr_t *wifi_mfg_cmd_otp_mac_addr_rd_wr)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg misc;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_otp_mac_addr_rd_wr, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_otp_mac_addr_rd_wr, (wifi_mfg_cmd_otp_mac_addr_rd_wr_t *)&misc.param.mfg_otp_mac_addr_rd_wr,
           sizeof(wifi_mfg_cmd_otp_mac_addr_rd_wr_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_otp_cal_data(t_u16 cmd_action, wifi_mfg_cmd_otp_cal_data_rd_wr_t *wifi_mfg_cmd_otp_cal_data_rd_wr)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg *misc  = NULL;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    misc = (mlan_ds_misc_cfg *)OSA_MemoryAllocate(sizeof(mlan_ds_misc_cfg));
    (void)memset(misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_otp_cal_data_rd_wr, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        (void)OSA_MemoryFree(misc);
        return -WM_FAIL;
    }

    wifi_wait_for_cmdresp(misc);
    memcpy(wifi_mfg_cmd_otp_cal_data_rd_wr, (wifi_mfg_cmd_otp_cal_data_rd_wr_t *)&(misc->param.mfg_otp_cal_data_rd_wr),
           sizeof(wifi_mfg_cmd_otp_cal_data_rd_wr_t));
    (void)OSA_MemoryFree(misc);
    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_rf_test_tx_cont(t_u16 cmd_action,
                                 wifi_mfg_cmd_tx_cont_t *wifi_mfg_cmd_tx_cont,
                                 wifi_mfg_cmd_generic_cfg_t *wifi_mfg_cmd_generic_cfg)
{
    wifi_get_command_lock();
    mlan_ds_misc_cfg misc;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));
    cmd->seq_num   = 0x0;
    cmd->result    = 0x0;
    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MFG_COMMAND, cmd_action,
                                              0, NULL, wifi_mfg_cmd_tx_cont, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    wifi_wait_for_cmdresp(&misc);
    memcpy(wifi_mfg_cmd_generic_cfg, (wifi_mfg_cmd_generic_cfg_t *)&misc.param.mfg_generic_cfg,
           sizeof(wifi_mfg_cmd_generic_cfg_t));
    return wm_wifi.cmd_resp_status;
}

int wifi_set_rf_test_mode(void)
{
    int ret;
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_SET_TEST_MODE;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set rf test mode fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_unset_rf_test_mode(void)
{
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_UNSET_TEST_MODE;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    (void)wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);

    return WM_SUCCESS;
}

int wifi_set_rf_channel(const uint8_t channel)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    /* To align with mxdriver, skip channel valid check for rf test mode */
#if 0
    /* Check if Channel is allowed as per WWSM */
    if (!wlan_is_channel_valid(channel))
        return -WM_FAIL;
#endif

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_CHAN;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_generic_cfg.data1 = channel;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);

    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set rf channel fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_radio_mode(const uint8_t mode)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    /* Check if radio mode is valid */
    if (!wlan_is_radio_mode_valid(mode))
    {
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RADIO_MODE_CFG;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;
    wifi_mfg_cmd_generic_cfg.data1   = mode;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set radio mode fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_channel(uint8_t *channel)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_CHAN;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);

    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *channel = wifi_mfg_cmd_generic_cfg.data1;
        return WM_SUCCESS;
    }

    wifi_e("wifi get rf channel fail, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_radio_mode(uint8_t *mode)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RADIO_MODE_CFG;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);

    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *mode = wifi_mfg_cmd_generic_cfg.data1;
        return WM_SUCCESS;
    }

    wifi_e("wifi get rf radio fail, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_band(const uint8_t band)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (band != 0U
#if CONFIG_5GHz_SUPPORT
        && band != 1U
#endif
    )
        return -WM_FAIL;

#if CONFIG_5GHz_SUPPORT
    if ((band == 1) && ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        wifi_e("Not support 5G, please set 2G band");
        return -WM_FAIL;
    }
#endif

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_BAND_AG;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_generic_cfg.data1 = band;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);

    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        band_set = 1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi set rf band fails, error code: 0x%x", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_band(uint8_t *band)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (!band_set)
    {
        wifi_e("RF Band not set");
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_BAND_AG;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *band = wifi_mfg_cmd_generic_cfg.data1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi get rf band fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_bandwidth(const uint8_t bandwidth)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if ((bandwidth != 0U)
#if CONFIG_5GHz_SUPPORT
        && (bandwidth != 1U)
#endif
#if CONFIG_11AC
        && (bandwidth != 4U)
#endif
    )
    {
        return -WM_FAIL;
    }

    if ((bandwidth != 0) && ISSUPP_NO5G(mlan_adap->fw_cap_ext))
    {
        wifi_e("Not support 5G, please set 2G bandwidth");
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_CHANNELBW;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_generic_cfg.data1 = bandwidth;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        bandwidth_set = 1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi set rf bandwidth fails, error code: 0x%x", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_bandwidth(uint8_t *bandwidth)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (!bandwidth_set)
    {
        wifi_e("Bandwidth not set");
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RF_CHANNELBW;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *bandwidth = wifi_mfg_cmd_generic_cfg.data1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi get rf bandwidth fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_per(uint32_t *rx_tot_pkt_count, uint32_t *rx_mcast_bcast_count, uint32_t *rx_pkt_fcs_error)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_CLR_RX_ERR;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *rx_tot_pkt_count     = wifi_mfg_cmd_generic_cfg.data1;
        *rx_mcast_bcast_count = wifi_mfg_cmd_generic_cfg.data2;
        *rx_pkt_fcs_error     = wifi_mfg_cmd_generic_cfg.data3;
        return WM_SUCCESS;
    }

    wifi_e("Wifi get rf per fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_tx_cont_mode(const uint32_t enable_tx,
                             const uint32_t cw_mode,
                             const uint32_t payload_pattern,
                             const uint32_t cs_mode,
                             const uint32_t act_sub_ch,
                             const uint32_t tx_rate)
{
    wifi_mfg_cmd_tx_cont_t wifi_mfg_cmd_tx_cont;
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;
    int ret;

    if ((enable_tx > 1U) || (cw_mode > 1U) || (cs_mode > 1U) || (act_sub_ch == 2U || act_sub_ch > 3U))
        return -WM_FAIL;

    (void)memset(&wifi_mfg_cmd_tx_cont, 0x00, sizeof(wifi_mfg_cmd_tx_cont_t));
    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_tx_cont.mfg_cmd = MFG_CMD_TX_CONT;
    wifi_mfg_cmd_tx_cont.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_tx_cont.enable_tx       = enable_tx;
    wifi_mfg_cmd_tx_cont.cw_mode         = cw_mode;
    wifi_mfg_cmd_tx_cont.payload_pattern = payload_pattern;
    wifi_mfg_cmd_tx_cont.cs_mode         = cs_mode;
    wifi_mfg_cmd_tx_cont.act_sub_ch      = act_sub_ch;
    wifi_mfg_cmd_tx_cont.tx_rate         = tx_rate;

    ret = wifi_get_set_rf_test_tx_cont(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_tx_cont, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("Wifi set rf tx cont mode fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_tx_antenna(const uint8_t antenna)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (antenna != 1U && antenna != 2U)
        return -WM_FAIL;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_TX_ANT;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_generic_cfg.data1 = antenna;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        tx_antenna_set = 1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi set rf tx antenna fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_tx_antenna(uint8_t *antenna)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (!tx_antenna_set)
    {
        wifi_e("Tx Antenna not set");
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_TX_ANT;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        *antenna = wifi_mfg_cmd_generic_cfg.data1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi get rf tx antenna fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
    ;
}

int wifi_set_rf_rx_antenna(const uint8_t antenna)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (antenna != 1U && antenna != 2U)
        return -WM_FAIL;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RX_ANT;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_generic_cfg.data1 = antenna;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        rx_antenna_set = 1;
        return WM_SUCCESS;
    }

    wifi_e("Wifi get rf tx antenna fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_get_rf_rx_antenna(uint8_t *antenna)
{
    int ret;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;

    if (!rx_antenna_set)
    {
        wifi_e("Rx antenna not set");
        return -WM_FAIL;
    }

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RX_ANT;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_generic_cfg);

    if (ret == WM_SUCCESS)
        *antenna = wifi_mfg_cmd_generic_cfg.data1;

    return ret;
}

int wifi_set_rf_tx_power(const uint32_t power, const uint8_t mod, const uint8_t path_id)
{
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;
    int ret;

    if (mod != 0U && mod != 1U && mod != 2U)
        return -WM_FAIL;

    if (path_id != 0U && path_id != 1U && path_id != 2U)
        return -WM_FAIL;

    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));

    wifi_mfg_cmd_generic_cfg.mfg_cmd = MFG_CMD_RFPWR;
    wifi_mfg_cmd_generic_cfg.action  = HostCmd_ACT_GEN_SET;
#if defined(SD9177) || defined(IW610)
    /* Firecrest firmware expects value * 16 */
    wifi_mfg_cmd_generic_cfg.data1 = power * 16;
#else
    wifi_mfg_cmd_generic_cfg.data1 = power;
#endif
    wifi_mfg_cmd_generic_cfg.data2 = mod;
    wifi_mfg_cmd_generic_cfg.data3 = path_id;

    ret = wifi_get_set_rf_test_generic(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_generic_cfg);
    if (ret == WM_SUCCESS)
    {
        return WM_SUCCESS;
    }

    wifi_e("Wifi set rf tx power fails, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

#ifdef RW610
static int wifi_check_data_rate_id(const uint32_t data_rate)
{
    uint8_t i;
    for (i = 0; i < sizeof(tx_data_rate_ids) / sizeof(tx_data_rate_ids[0]); i++)
    {
        if (data_rate == tx_data_rate_ids[i])
        {
            return WM_SUCCESS;
        }
    }
    return -WM_FAIL;
}
#endif

int wifi_set_rf_tx_frame(const uint32_t enable,
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
                         const uint8_t *bssid)
{
    wifi_mfg_cmd_tx_frame_t wifi_mfg_cmd_tx_frame;
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;
    int ret;

    if (enable > 1U || frame_length < 1U || frame_length > 0x400U || burst_sifs_in_us > 255U || short_preamble > 1U ||
        act_sub_ch == 2U || act_sub_ch > 3U || short_gi > 1U || adv_coding > 1U || tx_bf > 1U || gf_mode > 1U ||
        stbc > 1U)
        return -WM_FAIL;

    (void)memset(&wifi_mfg_cmd_tx_frame, 0x00, sizeof(wifi_mfg_cmd_tx_frame_t));
    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));
#ifdef RW610
    ret = wifi_check_data_rate_id(data_rate);
    if ((enable == 1U) && (ret != WM_SUCCESS))
    {
        wifi_e("The configured data rate ID is illegal. data_rate_id: 0x%x\r\n", data_rate);
        return ret;
    }
#endif

    wifi_mfg_cmd_tx_frame.mfg_cmd = MFG_CMD_TX_FRAME;
    wifi_mfg_cmd_tx_frame.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_tx_frame.enable    = enable;
    wifi_mfg_cmd_tx_frame.data_rate = data_rate;
#ifdef RW610
    /* on fw side, data rate id of 802.11n/a/g/b start from 0, the data rate id need reduce 1 */
    if (data_rate <= HT_MCS7)
    {
        wifi_mfg_cmd_tx_frame.data_rate--;
    }
#endif

    wifi_mfg_cmd_tx_frame.frame_pattern = frame_pattern;
    wifi_mfg_cmd_tx_frame.frame_length  = frame_length;
    (void)memcpy((void *)wifi_mfg_cmd_tx_frame.bssid, (const void *)bssid, MLAN_MAC_ADDR_LENGTH);
    wifi_mfg_cmd_tx_frame.adjust_burst_sifs = adjust_burst_sifs;
    wifi_mfg_cmd_tx_frame.burst_sifs_in_us  = burst_sifs_in_us;
    wifi_mfg_cmd_tx_frame.short_preamble    = short_preamble;
    wifi_mfg_cmd_tx_frame.act_sub_ch        = act_sub_ch;
    wifi_mfg_cmd_tx_frame.short_gi          = short_gi;
    wifi_mfg_cmd_tx_frame.adv_coding        = adv_coding;
    wifi_mfg_cmd_tx_frame.tx_bf             = tx_bf;
    wifi_mfg_cmd_tx_frame.gf_mode           = gf_mode;
    wifi_mfg_cmd_tx_frame.stbc              = stbc;
    wifi_mfg_cmd_tx_frame.signal_bw         = -1;
    wifi_mfg_cmd_tx_frame.NumPkt            = -1;
    wifi_mfg_cmd_tx_frame.MaxPE             = -1;
    wifi_mfg_cmd_tx_frame.BeamChange        = -1;
    wifi_mfg_cmd_tx_frame.Dcm               = -1;
    wifi_mfg_cmd_tx_frame.Doppler           = -1;
    wifi_mfg_cmd_tx_frame.MidP              = -1;
    wifi_mfg_cmd_tx_frame.QNum              = -1;

    ret = wifi_get_set_rf_test_tx_frame(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_tx_frame, &wifi_mfg_cmd_generic_cfg);
    if (WM_SUCCESS == ret && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set rf tx frame fail, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_rf_trigger_frame_cfg(uint32_t Enable_tx,
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
                              uint8_t Pref_AC)
{
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr_t wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr;
    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;
    int ret;

    (void)memset(&wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr, 0x00, sizeof(wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr_t));
    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.mfg_cmd = MFG_CMD_CONFIG_TRIGGER_FRAME;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.enable_tx       = Enable_tx;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.standalone_hetb = Standalone_hetb;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.frmCtl.type     = FRAME_CTRL_TYPE;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.frmCtl.sub_type = FRAME_CTRL_SUBTYPE;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.duration        = FRAME_DURATION;

    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.trigger_type    = TriggerType;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ul_len          = UlLen;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.more_tf         = MoreTF;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.cs_required     = CSRequired;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ul_bw           = UlBw;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ltf_type        = LTFType;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ltf_mode        = LTFMode;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ltf_symbol      = LTFSymbol;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ul_stbc         = UlSTBC;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ldpc_ess        = LdpcESS;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.ap_tx_pwr       = ApTxPwr;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.pre_fec_pad_fct = PreFecPadFct;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.pe_disambig     = PeDisambig;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.spatial_reuse   = SpatialReuse;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.doppler         = Doppler;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_common_field.he_sig2         = HeSig2;

    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.aid12          = AID12;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ru_alloc_reg   = RUAllocReg;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ru_alloc       = RUAlloc;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ul_coding_type = UlCodingType;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ul_mcs         = UlMCS;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ul_dcm         = UlDCM;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ss_alloc       = SSAlloc;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.trig_user_info_field.ul_target_rssi = UlTargetRSSI;

    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.basic_trig_user_info.mpdu_mu_sf = MPDU_MU_SF;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.basic_trig_user_info.tid_al     = TID_AL;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.basic_trig_user_info.ac_pl      = AC_PL;
    wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr.basic_trig_user_info.pref_ac    = Pref_AC;

    ret = wifi_get_set_rf_trigger_frame_cfg(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_IEEEtypes_CtlBasicTrigHdr,
                                            &wifi_mfg_cmd_generic_cfg);
    if (WM_SUCCESS == ret && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set rf tx frame fail, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_cfg_rf_he_tb_tx(uint16_t enable, uint16_t qnum, uint16_t aid, uint16_t axq_mu_timer, int16_t tx_power)
{
    wifi_mfg_cmd_he_tb_tx_t wifi_mfg_cmd_he_tb_tx;

    wifi_mfg_cmd_generic_cfg_t wifi_mfg_cmd_generic_cfg;
    int ret;

    (void)memset(&wifi_mfg_cmd_he_tb_tx, 0x00, sizeof(wifi_mfg_cmd_he_tb_tx_t));
    (void)memset(&wifi_mfg_cmd_generic_cfg, 0x00, sizeof(wifi_mfg_cmd_generic_cfg_t));
    wifi_mfg_cmd_he_tb_tx.mfg_cmd = MFG_CMD_CONFIG_MAC_HE_TB_TX;
    wifi_mfg_cmd_he_tb_tx.action  = HostCmd_ACT_GEN_SET;

    wifi_mfg_cmd_he_tb_tx.enable       = enable;
    wifi_mfg_cmd_he_tb_tx.qnum         = qnum;
    wifi_mfg_cmd_he_tb_tx.aid          = aid;
    wifi_mfg_cmd_he_tb_tx.axq_mu_timer = axq_mu_timer;
    wifi_mfg_cmd_he_tb_tx.tx_power     = tx_power;

    ret = wifi_get_set_rf_he_tb_tx(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_he_tb_tx, &wifi_mfg_cmd_generic_cfg);
    if (WM_SUCCESS == ret && wifi_mfg_cmd_generic_cfg.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set rf tx frame fail, error code: 0x%x\r\n", wifi_mfg_cmd_generic_cfg.error);
    return -WM_FAIL;
}

int wifi_set_rf_otp_mac_addr(uint8_t *mac)
{
    int ret;

    wifi_mfg_cmd_otp_mac_addr_rd_wr_t wifi_mfg_cmd_otp_mac_addr_rd_wr;

    (void)memset(&wifi_mfg_cmd_otp_mac_addr_rd_wr, 0x00, sizeof(wifi_mfg_cmd_otp_mac_addr_rd_wr_t));

    wifi_mfg_cmd_otp_mac_addr_rd_wr.mfg_cmd = MFG_CMD_OTP_MAC_ADD;
    wifi_mfg_cmd_otp_mac_addr_rd_wr.action  = HostCmd_ACT_GEN_SET;
    (void)memcpy((void *)wifi_mfg_cmd_otp_mac_addr_rd_wr.mac_addr, (const void *)mac, MLAN_MAC_ADDR_LENGTH);

    ret = wifi_get_set_rf_otp_mac_addr(HostCmd_ACT_GEN_SET, &wifi_mfg_cmd_otp_mac_addr_rd_wr);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_otp_mac_addr_rd_wr.error == 0)
    {
        return WM_SUCCESS;
    }

    wifi_e("wifi set otp mac address fails, error code: 0x%x\r\n", wifi_mfg_cmd_otp_mac_addr_rd_wr.error);
    return -WM_FAIL;
}

int wifi_get_rf_otp_mac_addr(uint8_t *mac)
{
    int ret;

    wifi_mfg_cmd_otp_mac_addr_rd_wr_t wifi_mfg_cmd_otp_mac_addr_rd_wr;

    (void)memset(&wifi_mfg_cmd_otp_mac_addr_rd_wr, 0x00, sizeof(wifi_mfg_cmd_otp_mac_addr_rd_wr_t));

    wifi_mfg_cmd_otp_mac_addr_rd_wr.mfg_cmd = MFG_CMD_OTP_MAC_ADD;
    wifi_mfg_cmd_otp_mac_addr_rd_wr.action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_otp_mac_addr(HostCmd_ACT_GEN_GET, &wifi_mfg_cmd_otp_mac_addr_rd_wr);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_otp_mac_addr_rd_wr.error == 0)
    {
        (void)memcpy((void *)mac, (const void *)wifi_mfg_cmd_otp_mac_addr_rd_wr.mac_addr, MLAN_MAC_ADDR_LENGTH);
        return WM_SUCCESS;
    }

    wifi_e("wifi get otp mac address fails, error code: 0x%x\r\n", wifi_mfg_cmd_otp_mac_addr_rd_wr.error);
    return -WM_FAIL;
}

int wifi_set_rf_otp_cal_data(const uint8_t *cal_data, uint32_t cal_data_len)
{
    int ret;

    wifi_mfg_cmd_otp_cal_data_rd_wr_t *wifi_mfg_cmd_otp_cal_data_rd_wr = NULL;

    wifi_mfg_cmd_otp_cal_data_rd_wr =
        (wifi_mfg_cmd_otp_cal_data_rd_wr_t *)OSA_MemoryAllocate(sizeof(wifi_mfg_cmd_otp_cal_data_rd_wr_t));
    (void)memset(wifi_mfg_cmd_otp_cal_data_rd_wr, 0x00, sizeof(wifi_mfg_cmd_otp_cal_data_rd_wr_t));

    wifi_mfg_cmd_otp_cal_data_rd_wr->mfg_cmd      = MFG_CMD_OTP_CAL_DATA;
    wifi_mfg_cmd_otp_cal_data_rd_wr->action       = HostCmd_ACT_GEN_SET;
    wifi_mfg_cmd_otp_cal_data_rd_wr->cal_data_len = cal_data_len;
    (void)memcpy((void *)wifi_mfg_cmd_otp_cal_data_rd_wr->cal_data, (const void *)cal_data, cal_data_len);

    ret = wifi_get_set_rf_otp_cal_data(HostCmd_ACT_GEN_SET, wifi_mfg_cmd_otp_cal_data_rd_wr);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_otp_cal_data_rd_wr->error == 0)
    {
        ret = WM_SUCCESS;
    }
    else
    {
        wifi_e("wifi set cal data fails, error code: 0x%x\r\n", wifi_mfg_cmd_otp_cal_data_rd_wr->error);
        ret = -WM_FAIL;
    }

    (void)OSA_MemoryFree(wifi_mfg_cmd_otp_cal_data_rd_wr);
    return ret;
}

int wifi_get_rf_otp_cal_data(uint8_t *cal_data)
{
    int ret;
    uint32_t cal_data_status = 0;

    wifi_mfg_cmd_otp_cal_data_rd_wr_t *wifi_mfg_cmd_otp_cal_data_rd_wr = NULL;

    wifi_mfg_cmd_otp_cal_data_rd_wr =
        (wifi_mfg_cmd_otp_cal_data_rd_wr_t *)OSA_MemoryAllocate(sizeof(wifi_mfg_cmd_otp_cal_data_rd_wr_t));
    (void)memset(wifi_mfg_cmd_otp_cal_data_rd_wr, 0x00, sizeof(wifi_mfg_cmd_otp_cal_data_rd_wr_t));

    wifi_mfg_cmd_otp_cal_data_rd_wr->mfg_cmd = MFG_CMD_OTP_CAL_DATA;
    wifi_mfg_cmd_otp_cal_data_rd_wr->action  = HostCmd_ACT_GEN_GET;

    ret = wifi_get_set_rf_otp_cal_data(HostCmd_ACT_GEN_GET, wifi_mfg_cmd_otp_cal_data_rd_wr);
    if (ret == WM_SUCCESS && wifi_mfg_cmd_otp_cal_data_rd_wr->error == 0)
    {
        cal_data_status = wifi_mfg_cmd_otp_cal_data_rd_wr->cal_data_status;
        if (cal_data_status == 1)
        {
            (void)memcpy((void *)cal_data, (const void *)wifi_mfg_cmd_otp_cal_data_rd_wr->cal_data,
                         wifi_mfg_cmd_otp_cal_data_rd_wr->cal_data_len);
            ret = WM_SUCCESS;
        }
        else
        {
            ret = -WM_FAIL;
        }
    }
    else
    {
        wifi_e("wifi get otp cal data fails, error code: 0x%x\r\n", wifi_mfg_cmd_otp_cal_data_rd_wr->error);
        ret = -WM_FAIL;
    }

    (void)OSA_MemoryFree(wifi_mfg_cmd_otp_cal_data_rd_wr);

    return ret;
}
#endif

/*
 * fixme: Currently, we support only single SSID based scan. We can extend
 * this to a list of multiple SSIDs. The mlan API supports this.
 */
int wifi_send_scan_cmd(t_u8 bss_mode,
                       const t_u8 *specific_bssid,
                       const char *ssid,
                       uint8_t ssid_num,
                       const t_u8 num_channels,
                       const wifi_scan_channel_list_t *chan_list,
                       const t_u8 num_probes,
#if CONFIG_SCAN_WITH_RSSIFILTER
                       const t_s16 rssi_threshold,
#endif
#if CONFIG_SCAN_CHANNEL_GAP
                       const t_u16 scan_chan_gap,
#endif
                       const bool keep_previous_scan,
                       const bool active_scan_triggered)
{
    int ssid_len         = 0;
    char const *tmp_ssid = ssid;
    t_u8 i;
#if CONFIG_COMBO_SCAN
    const char wildcard_ssid[] = "*";
#endif
    mlan_adap->active_scan_triggered = MFALSE;

#if CONFIG_WPA_SUPP
    if (mlan_adap->wpa_supp_scan_triggered == MTRUE)
    {
        return -WM_E_BUSY;
    }

    mlan_adap->wpa_supp_scan_triggered = wm_wifi.wpa_supp_scan;
    wm_wifi.wpa_supp_scan              = MFALSE;
#endif
    if (ssid_num > MRVDRV_MAX_SSID_LIST_LENGTH)
        return -WM_E_INVAL;
    tmp_ssid = ssid;
    for (i = 0; i < ssid_num; i++)
    {
        ssid_len = strlen(tmp_ssid);
        tmp_ssid += ssid_len;
        tmp_ssid++;
        if (ssid_len > MLAN_MAX_SSID_LENGTH)
        {
#if CONFIG_WPA_SUPP
            mlan_adap->wpa_supp_scan_triggered = MFALSE;
#endif
            return -WM_E_INVAL;
        }
    }

#if !CONFIG_MEM_POOLS
    wlan_user_scan_cfg *user_scan_cfg = (wlan_user_scan_cfg *)OSA_MemoryAllocate(sizeof(wlan_user_scan_cfg));
#else
    wlan_user_scan_cfg *user_scan_cfg = (wlan_user_scan_cfg *)OSA_MemoryPoolAllocate(buf_768_MemoryPool);
#endif
    if (user_scan_cfg == MNULL)
    {
#if CONFIG_WPA_SUPP
        mlan_adap->wpa_supp_scan_triggered = MFALSE;
#endif
        return -WM_E_NOMEM;
    }

    (void)memset(user_scan_cfg, 0x00, sizeof(wlan_user_scan_cfg));

    user_scan_cfg->bss_mode           = bss_mode;
    user_scan_cfg->keep_previous_scan = keep_previous_scan;

#if CONFIG_SCAN_WITH_RSSIFILTER
    user_scan_cfg->rssi_threshold = rssi_threshold;
#endif

    if (num_probes > 0U && num_probes <= MAX_PROBES)
    {
        user_scan_cfg->num_probes = num_probes;
    }

    if (specific_bssid != NULL)
    {
        (void)memcpy((void *)user_scan_cfg->specific_bssid, (const void *)specific_bssid, MLAN_MAC_ADDR_LENGTH);
    }

    tmp_ssid = ssid;
    for (i = 0; i < ssid_num; i++)
    {
        ssid_len = strlen(tmp_ssid);
        (void)memcpy((void *)user_scan_cfg->ssid_list[i].ssid, (const void *)tmp_ssid, ssid_len);
        tmp_ssid += ssid_len;
        tmp_ssid++;
    }

#if CONFIG_COMBO_SCAN
    for (i = 0; (i < MRVDRV_MAX_SSID_LIST_LENGTH) && (*user_scan_cfg->ssid_list[i].ssid); i++)
    {
        if (!strncmp(wildcard_ssid, (char *)(user_scan_cfg->ssid_list[i].ssid), strlen(wildcard_ssid)))
        {
            (void)memset(user_scan_cfg->ssid_list[i].ssid, 0x00, sizeof(user_scan_cfg->ssid_list[i].ssid));
            user_scan_cfg->ssid_list[i].max_len = 40;
        }
    }
#endif

    if ((chan_list != MNULL) && (chan_list[0].radio_type & BAND_SPECIFIED))
    {
        user_scan_cfg->chan_list[0].radio_type = chan_list[0].radio_type;
    }
    else if (num_channels > 0U && num_channels <= WLAN_USER_SCAN_CHAN_MAX && chan_list != MNULL)
    {
        for (i = 0; i < num_channels; i++)
        {
            /** Channel Number to scan */
            user_scan_cfg->chan_list[i].chan_number = chan_list[i].chan_number;
            /** Radio type: 'B/G' Band = 0, 'A' Band = 1 */
            /* fixme: B/G is hardcoded here. Ask the caller first to
               send the radio type and then change here */
            if (chan_list[i].chan_number > 14U)
            {
                user_scan_cfg->chan_list[i].radio_type = 1;
            }
            /** Scan type: Active = 1, Passive = 2 */
            /* fixme: Active is hardcoded here. Ask the caller first to
               send the  type and then change here */
            user_scan_cfg->chan_list[i].scan_type = chan_list[i].scan_type;
            /** Scan duration in milliseconds; if 0 default used */
            user_scan_cfg->chan_list[i].scan_time = chan_list[i].scan_time;
        }
    }

    if (active_scan_triggered)
    {
        mlan_adap->active_scan_triggered = MTRUE;
    }
#if CONFIG_SCAN_CHANNEL_GAP
    user_scan_cfg->scan_chan_gap = scan_chan_gap;
#endif
    if (wm_wifi.g_user_scan_cfg != NULL)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree((void *)user_scan_cfg);
#else
        OSA_MemoryPoolFree(buf_768_MemoryPool, user_scan_cfg);
#endif

#if CONFIG_WPA_SUPP
        mlan_adap->wpa_supp_scan_triggered = MFALSE;
        return -WM_E_BUSY;
#else
        return -WM_E_BUSY;
#endif
    }

    wm_wifi.g_user_scan_cfg = user_scan_cfg;

#if CONFIG_ZEPHYR
    (void)OSA_EventNotifyPost(wm_wifi.wifi_scan_task_Handle);
#else
    (void)OSA_EventSet((osa_event_handle_t)wm_wifi.wifi_event_Handle, WIFI_EVENT_SCAN);
#endif

    return WM_SUCCESS;
}

#if CONFIG_WPA_SUPP
int wifi_send_sched_scan_cmd(nxp_wifi_trigger_sched_scan_t *params)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    int ret, i;

    memset(&pmpriv->scan_cfg, 0, sizeof(pmpriv->scan_cfg));

    if (params->num_ssids != 0U)
    {
        for (i = 0; i < params->num_ssids; i++)
        {
            memcpy((void *)pmpriv->scan_cfg.ssid_list[i].ssid, (const void *)params->scan_ssids[i].ssid,
                   (size_t)params->scan_ssids[i].ssid_len);
            pmpriv->scan_cfg.ssid_list[i].max_len = 0;
        }
    }
    else
    {
        pmpriv->scan_cfg.ssid_list[0].max_len = 0xff;
    }

    for (i = 0; params->num_chans; i++)
    {
        pmpriv->scan_cfg.chan_list[i].chan_number = params->chan_list[i];

        pmpriv->scan_cfg.chan_list[i].scan_type = MLAN_SCAN_TYPE_ACTIVE;

        if (pmpriv->scan_cfg.chan_list[i].chan_number > 14)
        {
            pmpriv->scan_cfg.chan_list[i].radio_type = HostCmd_SCAN_RADIO_TYPE_A;
        }
        else
        {
            pmpriv->scan_cfg.chan_list[i].radio_type = HostCmd_SCAN_RADIO_TYPE_BG;
        }
    }

    if (pmpriv->probe_req_index != -1)
    {
        ret = wifi_clear_mgmt_ie2(MLAN_BSS_TYPE_STA, pmpriv->probe_req_index);

        if (ret != WM_SUCCESS)
        {
            wifi_e("Clear probe req IE failed");
            return -WM_FAIL;
        }
        pmpriv->probe_req_index = -1;
    }

    if (params->extra_ies.ie_len)
    {
        pmpriv->probe_req_index = wifi_set_mgmt_ie2(MLAN_BSS_TYPE_STA, MGMT_MASK_PROBE_REQ,
                                                    (void *)params->extra_ies.ie, params->extra_ies.ie_len);

        if (pmpriv->probe_req_index == -1)
        {
            wifi_e("Set probe req IE failed");
            return -WM_FAIL;
        }
    }

    pmpriv->scan_cfg.report_condition = params->report_condition;
    pmpriv->scan_cfg.rssi_threshold   = params->filter_rssi;
    pmpriv->scan_cfg.repeat_count     = params->repeat_count;
    pmpriv->scan_cfg.scan_interval    = params->scan_interval;
    pmpriv->scan_cfg.chan_per_scan    = params->chan_per_scan;
    pmpriv->scan_cfg.num_probes       = 2;

    pmpriv->scan_cfg.bss_type = MLAN_BSS_MODE_INFRA;
    pmpriv->scan_cfg.action   = BG_SCAN_ACT_SET;
    pmpriv->scan_cfg.enable   = MTRUE;

    ret = wifi_request_bgscan(pmpriv);
    if (ret)
    {
        wifi_d("Failed to request bgscan");
        return -WM_FAIL;
    }

    pmpriv->sched_scanning   = MTRUE;
    pmpriv->bg_scan_start    = MTRUE;
    pmpriv->bg_scan_reported = MFALSE;

    return ret;
}

int wifi_send_stop_sched_scan_cmd(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    pmpriv->sched_scanning   = MFALSE;
    pmpriv->bg_scan_start    = MFALSE;
    pmpriv->bg_scan_reported = MFALSE;

    memset(&pmpriv->scan_cfg, 0, sizeof(pmpriv->scan_cfg));

    pmpriv->scan_cfg.action = BG_SCAN_ACT_SET;
    pmpriv->scan_cfg.enable = MFALSE;
    int ret                 = wifi_request_bgscan(pmpriv);
    if (ret)
    {
        wifi_d("Failed to request bgscan");
    }

    return ret;
}
#endif

static int wifi_send_key_material_cmd(int bss_index, mlan_ds_sec_cfg *sec)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;
    mlan_status rv = MLAN_STATUS_SUCCESS;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)sec;
    req.buf_len   = sizeof(mlan_ds_sec_cfg);
    req.bss_index = bss_index;
    req.req_id    = MLAN_IOCTL_SEC_CFG;
    req.action    = MLAN_ACT_SET;

    if (bss_index != 0)
    {
        rv = wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else
    {
        rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    }

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_set_key(int bss_index,
                 bool is_pairwise,
                 const uint8_t key_index,
                 const uint8_t *key,
                 unsigned key_len,
                 const uint8_t *seq,
                 unsigned seq_len,
                 const uint8_t *mac_addr,
                 unsigned int flags)
{
    /* fixme: check if this needs to go on heap */
    mlan_ds_sec_cfg sec;
    int ret           = WM_SUCCESS;
    t_u8 bcast_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#if CONFIG_GTK_REKEY_OFFLOAD
    mlan_private *pmpriv        = (mlan_private *)mlan_adap->priv[bss_index];
    t_u8 zero_kek[MLAN_KEK_LEN] = {0};
#endif

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;

    if (key_len > MAX_WEP_KEY_SIZE)
    {
        if (seq && seq_len)
        {
            memcpy(sec.param.encrypt_key.pn, seq, seq_len);
            sec.param.encrypt_key.key_flags |= KEY_FLAG_RX_SEQ_VALID;
        }

        if (mac_addr)
        {
            if (is_pairwise)
            {
                sec.param.encrypt_key.key_flags |= KEY_FLAG_SET_TX_KEY;
            }
            else
            {
                sec.param.encrypt_key.key_flags |= KEY_FLAG_GROUP_KEY;
            }
            (void)memcpy((void *)sec.param.encrypt_key.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
        }
        else
        {
            memcpy(sec.param.encrypt_key.mac_addr, bcast_addr, MLAN_MAC_ADDR_LENGTH);
            sec.param.encrypt_key.key_flags |= KEY_FLAG_GROUP_KEY;
        }
        sec.param.encrypt_key.key_index = key_index;

        if (flags)
        {
            sec.param.encrypt_key.key_flags |= flags;
        }
    }
    else
    {
        sec.param.encrypt_key.key_index          = MLAN_KEY_INDEX_DEFAULT;
        sec.param.encrypt_key.is_current_wep_key = MTRUE;
    }

    sec.param.encrypt_key.key_len = key_len;
    (void)memcpy((void *)sec.param.encrypt_key.key_material, (const void *)key, key_len);

    ret = wifi_send_key_material_cmd(bss_index, &sec);

#if CONFIG_GTK_REKEY_OFFLOAD

    if ((ret == WM_SUCCESS) && (is_pairwise == false))
    {
        if (memcmp(pmpriv->gtk_rekey.kek, zero_kek, sizeof(zero_kek)) != 0)
        {
            mlan_status status = MLAN_STATUS_SUCCESS;
            ret = wlan_prepare_cmd(pmpriv, HostCmd_CMD_CONFIG_GTK_REKEY_OFFLOAD_CFG, HostCmd_ACT_GEN_SET, 0, MNULL,
                                   &pmpriv->gtk_rekey);
            if (status)
            {
                PRINTM(MINFO, "Error sending message to FW\n");
                ret = -WM_FAIL;
            }
            (void)__memset(pmpriv->adapter, &pmpriv->gtk_rekey, 0, sizeof(mlan_ds_misc_gtk_rekey_data));
        }
    }
#endif

    return ret;
}

int wifi_set_rekey_info(
    int bss_index, const t_u8 *kek, size_t kek_len, const t_u8 *kck, size_t kck_len, const t_u8 *replay_ctr)
{
#if CONFIG_GTK_REKEY_OFFLOAD
    mlan_ds_misc_cfg misc;
    mlan_ioctl_req req;
    mlan_status rv = MLAN_STATUS_SUCCESS;

    (void)memset(&misc, 0x00, sizeof(mlan_ds_misc_cfg));

    misc.sub_command = MLAN_OID_MISC_CONFIG_GTK_REKEY_OFFLOAD;

    (void)memcpy(misc.param.gtk_rekey.kek, kek, MLAN_KEK_LEN);
    (void)memcpy(misc.param.gtk_rekey.kck, kck, MLAN_KCK_LEN);
    (void)memcpy(misc.param.gtk_rekey.replay_ctr, replay_ctr, MLAN_REPLAY_CTR_LEN);

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)&misc;
    req.buf_len   = sizeof(mlan_ds_misc_cfg);
    req.bss_index = bss_index;
    req.req_id    = MLAN_IOCTL_MISC_CFG;
    req.action    = MLAN_ACT_SET;

    if (bss_index != 0)
    {
        rv = wlan_ops_uap_ioctl(mlan_adap, &req);
    }
    else
    {
        rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    }

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
#else
    return WM_SUCCESS;
#endif
}

int wifi_set_igtk_key(int bss_index, const uint8_t *pn, const uint16_t key_index, const uint8_t *key, unsigned key_len)
{
    /* fixme: check if this needs to go on heap */
    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;

    sec.param.encrypt_key.key_flags = KEY_FLAG_AES_MCAST_IGTK;
    sec.param.encrypt_key.key_index = key_index;

    (void)memcpy((void *)sec.param.encrypt_key.pn, (const void *)pn, SEQ_MAX_SIZE);
    sec.param.encrypt_key.key_len = key_len;
    (void)memcpy((void *)sec.param.encrypt_key.key_material, (const void *)key, key_len);

    return wifi_send_key_material_cmd(bss_index, &sec);
}

int wifi_remove_key(int bss_index, bool is_pairwise, const uint8_t key_index, const uint8_t *mac_addr)
{
    /* fixme: check if this needs to go on heap */
    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_ENCRYPT_KEY;

    if (key_index == KEY_INDEX_CLEAR_ALL)
    {
        sec.param.encrypt_key.key_disable = MTRUE;
    }
    else
    {
        sec.param.encrypt_key.key_remove = MTRUE;
        sec.param.encrypt_key.key_index  = key_index;
    }

    sec.param.encrypt_key.key_len   = MLAN_MAX_KEY_LENGTH;
    sec.param.encrypt_key.key_flags = KEY_FLAG_REMOVE_KEY;

    if (mac_addr)
    {
        (void)memcpy((void *)sec.param.encrypt_key.mac_addr, (const void *)mac_addr, MLAN_MAC_ADDR_LENGTH);
    }

    return wifi_send_key_material_cmd(bss_index, &sec);
}



static int wifi_send_rf_antenna_cmd(t_u16 action, wifi_antcfg_t *wifi_antcfg)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    mlan_ds_ant_cfg_1x1 ant_cfg_1x1;

    (void)memset(&ant_cfg_1x1, 0x00, sizeof(mlan_ds_ant_cfg_1x1));

    ant_cfg_1x1.antenna       = (t_u32) * (wifi_antcfg->ant_mode);
    ant_cfg_1x1.evaluate_time = (t_u16) * (wifi_antcfg->evaluate_time);
#ifdef RW610
    ant_cfg_1x1.evaluate_mode = (t_u8) * (wifi_antcfg->evaluate_mode);
#endif

    if (action != HostCmd_ACT_GEN_GET && action != HostCmd_ACT_GEN_SET)
    {
        return -WM_FAIL;
    }

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, S_DS_GEN + sizeof(HostCmd_DS_802_11_RF_ANTENNA));

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    mlan_status rv =
        wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_RF_ANTENNA, action, 0, NULL, &ant_cfg_1x1, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        (void)wifi_put_command_lock();
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? wifi_antcfg : NULL);

    return wm_wifi.cmd_resp_status;
}

#ifndef RW610
int wifi_get_antenna(t_u32 *ant_mode, t_u16 *evaluate_time, t_u16 *current_antenna)
#else
int wifi_get_antenna(t_u32 *ant_mode, t_u16 *evaluate_time, t_u8 *evaluate_mode, t_u16 *current_antenna)
#endif
{
    if (ant_mode == MNULL)
    {
        return -WM_E_INVAL;
    }

    wifi_antcfg_t antenna_cfg;
    antenna_cfg.ant_mode        = ant_mode;
    antenna_cfg.evaluate_time   = evaluate_time;
#ifdef RW610
    antenna_cfg.evaluate_mode   = evaluate_mode;
#endif
    antenna_cfg.current_antenna = current_antenna;

    int rv = wifi_send_rf_antenna_cmd(HostCmd_ACT_GEN_GET, &antenna_cfg);
    if (rv != WM_SUCCESS || wm_wifi.cmd_resp_status != WM_SUCCESS)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}


#ifndef RW610
int wifi_set_antenna(t_u32 ant_mode, t_u16 evaluate_time)
{
    wifi_antcfg_t antenna_cfg;
    antenna_cfg.ant_mode      = &ant_mode;
    antenna_cfg.evaluate_time = &evaluate_time;

    return wifi_send_rf_antenna_cmd(HostCmd_ACT_GEN_SET, &antenna_cfg);
}
#else
int wifi_set_antenna(t_u32 ant_mode, t_u16 evaluate_time, t_u8 evaluate_mode)
{
    wifi_antcfg_t antenna_cfg;
    antenna_cfg.ant_mode      = &ant_mode;
    antenna_cfg.evaluate_time = &evaluate_time;
    antenna_cfg.evaluate_mode = &evaluate_mode;

    return wifi_send_rf_antenna_cmd(HostCmd_ACT_GEN_SET, &antenna_cfg);
}
#endif /*RW610*/

#if CONFIG_WIFI_GET_LOG
static int wifi_send_get_log_cmd(wlan_pkt_stats_t *stats, mlan_bss_type bss_type)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;

    mlan_status rv =
        wlan_ops_sta_prepare_cmd(pmpriv, HostCmd_CMD_802_11_GET_LOG, HostCmd_ACT_GEN_GET, 0, NULL, NULL, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    return wifi_wait_for_cmdresp(stats);
}

int wifi_get_log(wlan_pkt_stats_t *stats, mlan_bss_type bss_type)

{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[bss_type];
    int rv;

    CHECK_BSS_TYPE(bss_type, -WM_FAIL);
    rv = wifi_send_get_log_cmd(stats, bss_type);
    if (rv != WM_SUCCESS || wm_wifi.cmd_resp_status != WM_SUCCESS)
        return -WM_FAIL;

    stats->rx_unicast_cnt = stats->rx_frag - stats->mcast_rx_frame;
    stats->tx_overrun_cnt = pmpriv->tx_overrun_cnt;
    stats->rx_overrun_cnt = pmpriv->rx_overrun_cnt;

    return WM_SUCCESS;
}
#endif

static int wifi_send_cmd_802_11_supplicant_pmk(int mode, mlan_ds_sec_cfg *sec, mlan_act_ioctl action)
{
    /* fixme: check if this needs to go on heap */
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    req.pbuf      = (t_u8 *)sec;
    req.buf_len   = sizeof(mlan_ds_sec_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_SEC_CFG;
    req.action    = action;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_send_add_wpa_pmk(int mode, char *ssid, char *bssid, char *pmk, unsigned int len)
{
    if (ssid == MNULL || (len != MLAN_MAX_KEY_LENGTH))
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_passphrase *pp = &sec.param.passphrase;
    pp->ssid.ssid_len      = ssid_len;
    (void)memcpy((void *)pp->ssid.ssid, (const void *)ssid, ssid_len);

    /* MAC */
    if (bssid != NULL)
    {
        (void)memcpy((void *)pp->bssid, (const void *)bssid, MLAN_MAC_ADDR_LENGTH);
    }

    /* PMK */
    pp->psk_type = MLAN_PSK_PMK;
    (void)memcpy((void *)pp->psk.pmk.pmk, (const void *)pmk, len);

    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

/* fixme: This function has not been tested because of known issue in
   calling function. The calling function has been disabled for that */
int wifi_send_get_wpa_pmk(int mode, char *ssid)
{
    if (ssid == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_passphrase *pp = &sec.param.passphrase;
    pp->ssid.ssid_len      = ssid_len;
    (void)memcpy((void *)pp->ssid.ssid, (const void *)ssid, ssid_len);

    /* Zero MAC */

    pp->psk_type = MLAN_PSK_QUERY;

    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_GET);
}

/*
Note:
Passphrase can be between 8 to 63 if it is ASCII and 64 if its PSK
hexstring
*/
int wifi_send_add_wpa_psk(int mode, char *ssid, char *passphrase, unsigned int len)
{
    if (ssid == MNULL || ((len < MLAN_MIN_PASSPHRASE_LENGTH) || (len > MLAN_MAX_PASSPHRASE_LENGTH)))
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_passphrase *pp = &sec.param.passphrase;
    pp->ssid.ssid_len      = ssid_len;
    (void)memcpy((void *)pp->ssid.ssid, (const void *)ssid, ssid_len);

    /* Zero MAC */

    /* Passphrase */
    pp->psk_type                      = MLAN_PSK_PASSPHRASE;
    pp->psk.passphrase.passphrase_len = len;
    (void)memcpy((void *)pp->psk.passphrase.passphrase, (const void *)passphrase, len);

    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

/*
Note:
Password can be between 1 to 255 if it is ASCII
*/
int wifi_send_add_wpa3_password(int mode, char *ssid, char *password, unsigned int len)
{
    if (ssid == MNULL || ((len < MLAN_MIN_PASSWORD_LENGTH) || (len > MLAN_MAX_PASSWORD_LENGTH)))
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSWORD;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_passphrase *pp = &sec.param.passphrase;
    pp->ssid.ssid_len      = ssid_len;
    (void)memcpy((void *)pp->ssid.ssid, (const void *)ssid, ssid_len);

    /* Zero MAC */

    /* Passphrase */
    pp->psk_type              = MLAN_PSK_PASSWORD;
    pp->password.password_len = len;
    (void)memcpy((void *)pp->password.password, (const void *)password, len);

    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

int wifi_send_clear_wpa_psk(int mode, const char *ssid)
{
    if (ssid == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    sec.param.passphrase.ssid.ssid_len = ssid_len;
    (void)strcpy((char *)sec.param.passphrase.ssid.ssid, ssid);

    /* Zero MAC */

    sec.param.passphrase.psk_type = MLAN_PSK_CLEAR;
    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

int wifi_send_enable_supplicant(int mode, const char *ssid)
{
    if (ssid == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    /* SSID */
    int ssid_len = strlen(ssid);
    if (ssid_len > MLAN_MAX_SSID_LENGTH)
    {
        return -WM_E_INVAL;
    }

    sec.param.passphrase.ssid.ssid_len = ssid_len;
    (void)strcpy((char *)sec.param.passphrase.ssid.ssid, ssid);

    /* Zero MAC */

    sec.param.passphrase.psk_type = MLAN_PSK_PASSPHRASE;
    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

int wifi_send_disable_supplicant(int mode)
{
    mlan_ds_sec_cfg sec;

    (void)memset(&sec, 0x00, sizeof(mlan_ds_sec_cfg));
    sec.sub_command = MLAN_OID_SEC_CFG_PASSPHRASE;

    sec.param.passphrase.psk_type = MLAN_PSK_CLEAR;

    return wifi_send_cmd_802_11_supplicant_pmk(mode, &sec, MLAN_ACT_SET);
}

int wifi_set_mac_multicast_addr(const char *mlist, t_u32 num_of_addr)
{
    if (mlist == MNULL)
    {
        return -WM_E_INVAL;
    }
    if (num_of_addr > MLAN_MAX_MULTICAST_LIST_SIZE)
    {
        return -WM_E_INVAL;
    }
#if !CONFIG_MEM_POOLS
    mlan_multicast_list *mcast_list = (mlan_multicast_list *)OSA_MemoryAllocate(sizeof(mlan_multicast_list));
#else
    mlan_multicast_list *mcast_list = (mlan_multicast_list *)OSA_MemoryPoolAllocate(buf_256_MemoryPool);
#endif
    if (mcast_list == NULL)
    {
        return -WM_FAIL;
    }

    (void)memset(mcast_list, 0x0, sizeof(mlan_multicast_list));
    (void)memcpy(mcast_list->mac_list, (const void *)mlist, num_of_addr * MLAN_MAC_ADDR_LENGTH);
    mcast_list->num_multicast_addr = num_of_addr;
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_MAC_MULTICAST_ADR,
                                              HostCmd_ACT_GEN_SET, 0, NULL, mcast_list, cmd);

    if (rv != MLAN_STATUS_SUCCESS)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(mcast_list);
#else
        OSA_MemoryPoolFree(buf_256_MemoryPool, mcast_list);
#endif

        return -WM_FAIL;
    }
    (void)wifi_wait_for_cmdresp(NULL);
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(mcast_list);
#else
    OSA_MemoryPoolFree(buf_256_MemoryPool, mcast_list);
#endif

    return WM_SUCCESS;
}

int wifi_get_otp_user_data(uint8_t *buf, uint16_t len)
{
    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    mlan_ds_misc_otp_user_data pdata;

    if (buf == MNULL)
    {
        return -WM_E_INVAL;
    }

    cmd->seq_num           = 0x0;
    cmd->result            = 0x0;
    pdata.user_data_length = len > MAX_OTP_USER_DATA_LEN ? MAX_OTP_USER_DATA_LEN : len;

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_OTP_READ_USER_DATA,
                                              HostCmd_ACT_GEN_GET, 0, NULL, &pdata, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    (void)wifi_wait_for_cmdresp(buf);
    return wm_wifi.cmd_resp_status;
}

int wifi_get_cal_data(wifi_cal_data_t *cal_data)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_802_11_CFG_DATA) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_CFG_DATA);

    HostCmd_DS_802_11_CFG_DATA *cfg_data_cmd = (HostCmd_DS_802_11_CFG_DATA *)((uint32_t)cmd + S_DS_GEN);

    cfg_data_cmd->action   = HostCmd_ACT_GEN_GET;
    cfg_data_cmd->type     = 0x02;
    cfg_data_cmd->data_len = 0x00;

    cmd->size    = size;
    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(cal_data);

    return wm_wifi.cmd_resp_status;
}

#if CONFIG_11AX
void wifi_request_get_fw_info(mlan_private *priv, mlan_fw_info *fw_info)
{
    (void)memset(fw_info, 0x0, sizeof(mlan_fw_info));

    fw_info->fw_ver = mlan_adap->fw_release_number;
    (void)memcpy(fw_info->mac_addr, priv->curr_addr, MLAN_MAC_ADDR_LENGTH);
    fw_info->fw_bands           = mlan_adap->fw_bands;
    fw_info->hw_dev_mcs_support = mlan_adap->hw_dev_mcs_support;

    fw_info->hw_2g_hecap_len = mlan_adap->hw_2g_hecap_len;

    (void)memcpy(fw_info->hw_2g_he_cap, mlan_adap->hw_2g_he_cap, mlan_adap->hw_2g_hecap_len);

    fw_info->hw_hecap_len = mlan_adap->hw_hecap_len;

    (void)memcpy(fw_info->hw_he_cap, mlan_adap->hw_he_cap, mlan_adap->hw_hecap_len);
}
#endif

#if CONFIG_WIFI_CAPA
void wifi_get_fw_info(mlan_bss_type type, t_u16 *fw_bands)
{
    mlan_fw_info fw_info;

    (void)memset(&fw_info, 0x0, sizeof(mlan_fw_info));
    fw_info.fw_ver = mlan_adap->fw_release_number;
    (void)memcpy(fw_info.mac_addr, mlan_adap->priv[type]->curr_addr, MLAN_MAC_ADDR_LENGTH);
    fw_info.fw_bands           = mlan_adap->fw_bands;
    fw_info.hw_dev_mcs_support = mlan_adap->hw_dev_mcs_support;

    *fw_bands = fw_info.fw_bands;
}
#endif

int wifi_get_firmware_version_ext(wifi_fw_version_ext_t *version_ext)
{
    if (version_ext == MNULL)
    {
        return -WM_E_INVAL;
    }

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    mlan_status rv       = wifi_prepare_and_send_cmd(pmpriv, HostCmd_CMD_VERSION_EXT, HostCmd_ACT_GEN_GET, 0, NULL,
                                                     &version_ext->version_str_sel, MLAN_BSS_TYPE_STA, version_ext);
    return (rv == MLAN_STATUS_SUCCESS ? WM_SUCCESS : -WM_FAIL);
}

int wifi_get_firmware_version(wifi_fw_version_t *ver)
{
    if (ver == MNULL)
    {
        return -WM_E_INVAL;
    }

    union
    {
        uint32_t l;
        uint8_t c[4];
    } u_ver;
    char fw_ver[32];

    u_ver.l = mlan_adap->fw_release_number;
    (void)sprintf(fw_ver, "%u.%u.%u.p%u", u_ver.c[2], u_ver.c[1], u_ver.c[0], u_ver.c[3]);

    (void)snprintf(ver->version_str, MLAN_MAX_VER_STR_LEN, driver_version_format, fw_ver, driver_version);

    return WM_SUCCESS;
}

/* Region: US(US) or Canada(CA) or Singapore(SG) 2.4 GHz */
static wifi_sub_band_set_t subband_US_CA_SG_2_4_GHz[] = {{1, 11, 20}};

/* Region: Europe(EU), Australia(AU), Republic of Korea(KR),
China(CN) 2.4 GHz */
static wifi_sub_band_set_t subband_EU_AU_KR_CN_2_4GHz[] = {{1, 13, 20}};

/* Region: Japan(JP) 2.4 GHz */
static wifi_sub_band_set_t subband_JP_2_4GHz[] = {
    {1, 14, 20},
};

/* Region: World Wide Safe Mode(WWSM) 2.4 GHz */
static wifi_sub_band_set_t subband_WWSM_2_4GHz[] = {
    {1, 14, 22},
};

/* Region: Constrained 2.4 Ghz */
static wifi_sub_band_set_t subband_CS_2_4GHz[] = {{1, 9, 20}, {10, 2, 10}};

#if CONFIG_5GHz_SUPPORT

#if (CONFIG_UNII4_BAND_SUPPORT)
/* Region: US(US) 5 GHz */
wifi_sub_band_set_t subband_US_5_GHz[] = {{36, 8, 20}, {100, 11, 20}, {149, 8, 20}};

/* Region: France(FR) or Singapore(SG) 5 GHz */
wifi_sub_band_set_t subband_SG_FR_5_GHz[] = {{36, 8, 20}, {100, 11, 20}, {149, 5, 20}};
#else
/* Region: US(US) or France(FR) or Singapore(SG) 5 GHz */
static wifi_sub_band_set_t subband_US_SG_FR_5_GHz[] = {{36, 8, 20}, {100, 11, 20}, {149, 5, 20}};
#endif

/* Region: Canada(CA) 5 GHz */
static wifi_sub_band_set_t subband_CA_5_GHz[] = {{36, 8, 20}, {100, 5, 20}, {132, 3, 20}, {149, 5, 20}};

/* Region: Region: Europe(EU), Australia(AU), Republic of Korea(KR)
 * 5 GHz */
static wifi_sub_band_set_t subband_EU_AU_KR_5_GHz[] = {
    {36, 8, 20},
    {100, 11, 20},
    {149, 5, 20},
};

/* Region: Japan(JP) 5 GHz */
static wifi_sub_band_set_t subband_JP_5_GHz[] = {
    {8, 3, 23},
    {36, 8, 23},
    {100, 11, 23},
};

/* Region: China(CN) 5 Ghz */
static wifi_sub_band_set_t subband_CN_5_GHz[] = {
    {36, 4, 23},
    {52, 4, 23},
    {149, 5, 33},
};

/* Region: World Wide Safe Mode(WWSM) 5 GHz */
static wifi_sub_band_set_t subband_WWSM_5_GHz[] = {{36, 8, 22}, {100, 11, 22}, {149, 5, 22}};

#endif /* CONFIG_5GHz_SUPPORT */

int wifi_get_region_code(t_u32 *region_code)
{
    *region_code = mlan_adap->region_code;
    return WM_SUCCESS;
}

int wifi_set_region_code(t_u32 region_code)
{
    mlan_ds_misc_cfg misc = {
        .param.region_code = region_code,
    };

    if ((misc.param.region_code == 0x41) || (misc.param.region_code == 0xFE))
    {
        (void)PRINTF("Region code 0XFF is used for Japan to support channels of both 2.4GHz band and 5GHz band.\r\n");
        (void)PRINTF("Region code 0X40 is used for Japan to support channels of 5GHz band.\r\n");
        return -WM_FAIL;
    }

    mlan_ioctl_req req = {
        .bss_index = 0,
        .pbuf      = (t_u8 *)&misc,
        .action    = MLAN_ACT_SET,
    };

    mlan_status mrv = wlan_misc_ioctl_region(mlan_adap, &req);
    if (mrv != MLAN_STATUS_SUCCESS)
    {
        wifi_w("Unable to set region code");
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}

int wifi_enable_11d_support()
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    wrapper_wlan_11d_enable(ENABLE_11D);

    wlan_11d_support_APIs(pmpriv);

    return wlan_enable_11d_support(pmpriv);
}

int wifi_disable_11d_support()
{
    mlan_adap->priv[0]->state_11d.user_enable_11d_support = DISABLE_11D;
    wrapper_wlan_11d_enable(DISABLE_11D);

    return WM_SUCCESS;
}

#if UAP_SUPPORT
int wifi_enable_uap_11d_support()
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];

    wrapper_wlan_uap_11d_enable(ENABLE_11D);

    wlan_11d_support_APIs(pmpriv);

    /* set callback for setting domain params when uap start */
    wifi_uap_enable_11d_support();

    return wlan_enable_11d_support(pmpriv);
}

int wifi_disable_uap_11d_support()
{
    mlan_adap->priv[1]->state_11d.user_enable_11d_support = DISABLE_11D;
    wrapper_wlan_uap_11d_enable(DISABLE_11D);

    wm_wifi.enable_11d_support   = false;
    wm_wifi.uap_support_11d_apis = MNULL;
    return WM_SUCCESS;
}
#endif /* UAP_SUPPORT */

int wifi_enable_11d_support_APIs(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    return wlan_11d_support_APIs(pmpriv);
}

wifi_sub_band_set_t *get_sub_band_from_region_code(int region_code, t_u8 *nr_sb)
{
    *nr_sb                        = 1;
    wifi_sub_band_set_t *ret_band = NULL;

    switch (region_code)
    {
        case 0x10:
        case 0x20:
            ret_band = subband_US_CA_SG_2_4_GHz;
            break;
        case 0x30:
        case 0x32:
        case 0x50:
            ret_band = subband_EU_AU_KR_CN_2_4GHz;
            break;
        case 0xFF:
            ret_band = subband_JP_2_4GHz;
            break;
        case 0x00:
            ret_band = subband_WWSM_2_4GHz;
            break;
        default:
            *nr_sb   = 2;
            ret_band = subband_CS_2_4GHz;
            break;
    }
    return ret_band;
}

#if CONFIG_5GHz_SUPPORT
wifi_sub_band_set_t *get_sub_band_from_region_code_5ghz(int region_code, t_u8 *nr_sb)
{
    *nr_sb                        = 1;
    wifi_sub_band_set_t *ret_band = NULL;

    switch (region_code)
    {
        case 0x10:
#if (CONFIG_UNII4_BAND_SUPPORT)
            *nr_sb = 3;
            return subband_US_5_GHz;
#endif
        case 0x32:
            *nr_sb = 3;
#if (CONFIG_UNII4_BAND_SUPPORT)
            ret_band = subband_SG_FR_5_GHz;
#else
            ret_band = subband_US_SG_FR_5_GHz;
#endif
            break;
        case 0x20:
            *nr_sb   = 4;
            ret_band = subband_CA_5_GHz;
            break;
        case 0x30:
            *nr_sb   = 3;
            ret_band = subband_EU_AU_KR_5_GHz;
            break;
        case 0x40:
        case 0xFF:
            *nr_sb   = 3;
            ret_band = subband_JP_5_GHz;
            break;
        case 0x50:
            *nr_sb   = 3;
            ret_band = subband_CN_5_GHz;
            break;
        case 0x00:
            *nr_sb   = 3;
            ret_band = subband_WWSM_5_GHz;
            break;
        default:
            *nr_sb = 3;
#if (CONFIG_UNII4_BAND_SUPPORT)
            ret_band = subband_US_5_GHz;
#else
            ret_band = subband_US_SG_FR_5_GHz;
#endif
            break;
    }
    return ret_band;
}
#endif /* CONFIG_5GHz_SUPPORT */

bool wifi_11d_is_channel_allowed(int channel)
{
    t_u8 i, j;
    t_u8 k;
    t_u8 nr_sb = 0;

    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    wifi_sub_band_set_t *sub_band = NULL;

    if (channel > 14)
    {
#if CONFIG_5GHz_SUPPORT
        sub_band = get_sub_band_from_region_code_5ghz(pmpriv->adapter->region_code, &nr_sb);
#else
        wifi_w("5 GHz support is not enabled");

        return false;
#endif /* CONFIG_5GHz_SUPPORT */
    }
    else
    {
        sub_band = get_sub_band_from_region_code(pmpriv->adapter->region_code, &nr_sb);
    }

    for (i = 0; i < nr_sb; i++)
    {
        j = sub_band[i].first_chan;

        for (k = 0; k < sub_band[i].no_of_chan; k++)
        {
            if (j == channel)
            {
                return true;
            }

            if (channel > 14)
            {
                j += 4;
            }
            else
            {
                j++;
            }
        }
    }

    /*For channel 144*/
    if (144 == channel)
    {
        return wlan_check_channel_by_region_table(pmpriv, channel);
    }

    return false;
}

int wifi_enable_ecsa_support(void)
{
    return wrapper_wlan_ecsa_enable();
}

bool wifi_is_ecsa_enabled(void)
{
    return mlan_adap->ecsa_enable;
}

static int get_free_mgmt_ie_index(unsigned int *mgmt_ie_index)
{
    unsigned int idx;

    for (idx = 0; idx < 32; idx++)
    {
        if ((mgmt_ie_index_bitmap & MBIT((t_u32)idx)) == 0U)
        {
            *mgmt_ie_index = idx;
            return WM_SUCCESS;
        }
    }
    return -WM_FAIL;
}

static void set_ie_index(unsigned int index)
{
    mgmt_ie_index_bitmap |= (MBIT(index));
}

static void clear_ie_index(unsigned int index)
{
    mgmt_ie_index_bitmap &= ~(MBIT(index));
}

#ifdef SD8801
static int wifi_config_ext_coex(int action,
                                const wifi_ext_coex_config_t *ext_coex_config,
                                wifi_ext_coex_stats_t *ext_coex_stats)
{
    int ret;
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)wifi_get_command_lock();

    cmd->command                                           = HostCmd_CMD_ROBUST_COEX;
    cmd->size                                              = sizeof(HostCmd_DS_ExtBLECoex_Config_t) + S_DS_GEN;
    cmd->seq_num                                           = 0;
    cmd->result                                            = 0;
    cmd->params.ext_ble_coex_cfg.action                    = action;
    cmd->params.ext_ble_coex_cfg.reserved                  = 0;
    cmd->params.ext_ble_coex_cfg.coex_cfg_data.header.type = TLV_TYPE_EXT_BLE_COEX_CFG;
    cmd->params.ext_ble_coex_cfg.coex_cfg_data.header.len =
        sizeof(MrvlIETypes_ExtBLECoex_Config_t) - sizeof(MrvlIEtypesHeader_t);

    if (action == HostCmd_ACT_GEN_SET)
    {
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.Enabled         = ext_coex_config->Enabled;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.IgnorePriority  = ext_coex_config->IgnorePriority;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.DefaultPriority = ext_coex_config->DefaultPriority;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.EXT_RADIO_REQ_ip_gpio_num =
            ext_coex_config->EXT_RADIO_REQ_ip_gpio_num;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.EXT_RADIO_REQ_ip_gpio_polarity =
            ext_coex_config->EXT_RADIO_REQ_ip_gpio_polarity;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.EXT_RADIO_PRI_ip_gpio_num =
            ext_coex_config->EXT_RADIO_PRI_ip_gpio_num;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.EXT_RADIO_PRI_ip_gpio_polarity =
            ext_coex_config->EXT_RADIO_PRI_ip_gpio_polarity;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.WLAN_GRANT_op_gpio_num = ext_coex_config->WLAN_GRANT_op_gpio_num;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.WLAN_GRANT_op_gpio_polarity =
            ext_coex_config->WLAN_GRANT_op_gpio_polarity;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.reserved_1 = ext_coex_config->reserved_1;
        cmd->params.ext_ble_coex_cfg.coex_cfg_data.reserved_2 = ext_coex_config->reserved_2;
    }
    ret = wifi_wait_for_cmdresp(ext_coex_stats);
    return ret;
}
#endif

static bool ie_index_is_set(unsigned int index)
{
    return (mgmt_ie_index_bitmap & (MBIT(index))) ? MTRUE : MFALSE;
}

void reset_ie_index()
{
    mgmt_ie_index_bitmap = 0x0000000F;
}

static int wifi_config_mgmt_ie(mlan_bss_type bss_type,
                               t_u16 action,
                               IEEEtypes_ElementId_t index,
                               void *buffer,
                               unsigned int *ie_len,
                               int mgmt_bitmap_index)
{
    uint8_t *buf, *pos;
    IEEEtypes_Header_t *ptlv_header = NULL;
    uint16_t buf_len                = 0;
    tlvbuf_custom_ie *tlv           = NULL;
    custom_ie *ie_ptr               = NULL;
    unsigned int mgmt_ie_index      = -1;
    int total_len =
        sizeof(tlvbuf_custom_ie) + 2U * (sizeof(custom_ie) - MAX_IE_SIZE) + sizeof(IEEEtypes_Header_t) + *ie_len;
    int ret = WM_SUCCESS;

#if !CONFIG_MEM_POOLS
    buf = (uint8_t *)OSA_MemoryAllocate(total_len);
#else
    buf = OSA_MemoryPoolAllocate(buf_512_MemoryPool);
#endif
    if (buf == MNULL)
    {
        wifi_e("Cannot allocate memory");
        return -WM_FAIL;
    }

    (void)memset(buf, 0, total_len);

    tlv       = (tlvbuf_custom_ie *)(void *)buf;
    tlv->type = MRVL_MGMT_IE_LIST_TLV_ID;

    /* Locate headers */
    ie_ptr = (custom_ie *)(tlv->ie_data);
    /* Set TLV fields */
    buf_len = sizeof(tlvbuf_custom_ie);

    if (action == HostCmd_ACT_GEN_SET)
    {
        if (*ie_len == 0U)
        {
            /*
               MGMT_WPA_IE = MGMT_VENDOR_SPECIFIC_221
               MGMT_WPS_IE = MGMT_VENDOR_SPECIFIC_221
               */

            if (!ie_index_is_set(mgmt_bitmap_index))
            {
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(buf);
#else
                OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
                return -WM_FAIL;
            }

            ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
            ie_ptr->ie_length         = 0;
            ie_ptr->ie_index          = (t_u16)mgmt_bitmap_index;

            tlv->length = sizeof(custom_ie) - MAX_IE_SIZE;
            buf_len += tlv->length;
            clear_ie_index(mgmt_bitmap_index);
        }
        else
        {
            ret = get_free_mgmt_ie_index(&mgmt_ie_index);

            if (WM_SUCCESS != ret)
            {
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(buf);
#else
                OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
                return -WM_FAIL;
            }

            pos         = ie_ptr->ie_buffer;
            ptlv_header = (IEEEtypes_Header_t *)(void *)pos;
            pos += sizeof(IEEEtypes_Header_t);

            ptlv_header->element_id = (IEEEtypes_ElementId_e)index;
            ptlv_header->len        = *ie_len;
            if (bss_type == MLAN_BSS_TYPE_UAP)
            {
                ie_ptr->mgmt_subtype_mask =
                    MGMT_MASK_BEACON | MGMT_MASK_PROBE_RESP | MGMT_MASK_ASSOC_RESP | MGMT_MASK_REASSOC_RESP;
            }
            else if (bss_type == MLAN_BSS_TYPE_STA)
            {
                ie_ptr->mgmt_subtype_mask = MGMT_MASK_PROBE_REQ | MGMT_MASK_ASSOC_REQ | MGMT_MASK_REASSOC_REQ;
            }
            else
            { /* Do Nothing */
            }

            tlv->length       = sizeof(custom_ie) + sizeof(IEEEtypes_Header_t) + *ie_len - MAX_IE_SIZE;
            ie_ptr->ie_length = sizeof(IEEEtypes_Header_t) + *ie_len;
            ie_ptr->ie_index  = mgmt_ie_index;

            buf_len += tlv->length;

            (void)memcpy((void *)pos, (const void *)buffer, *ie_len);
        }
    }
    else if (action == HostCmd_ACT_GEN_GET)
    {
        /* Get WPS IE */
        tlv->length = 0;
    }
    else
    { /* Do Nothing */
    }

    mlan_status rv = wrapper_wlan_cmd_mgmt_ie(bss_type, buf, buf_len, action);

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }

    if (action == HostCmd_ACT_GEN_GET)
    {
        if (wm_wifi.cmd_resp_status != 0)
        {
            wifi_w("Unable to get mgmt ie buffer");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(buf);
#else
            OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
            return wm_wifi.cmd_resp_status;
        }
        ie_ptr = (custom_ie *)(void *)(buf);
        (void)memcpy((void *)buffer, (const void *)ie_ptr->ie_buffer, ie_ptr->ie_length);
        *ie_len = ie_ptr->ie_length;
    }
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(buf);
#else
    OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif

    if ((action == HostCmd_ACT_GEN_SET) && *ie_len)
    {
        set_ie_index(mgmt_ie_index);
        return mgmt_ie_index;
    }
    else
    {
        return WM_SUCCESS;
    }
}

int wifi_get_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, void *buf, unsigned int *buf_len)
{
    return wifi_config_mgmt_ie(bss_type, HostCmd_ACT_GEN_GET, index, buf, buf_len, 0);
}

int wifi_set_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t id, void *buf, unsigned int buf_len)
{
    unsigned int data_len = buf_len;

    return wifi_config_mgmt_ie(bss_type, HostCmd_ACT_GEN_SET, id, buf, &data_len, 0);
}

int wifi_clear_mgmt_ie(mlan_bss_type bss_type, IEEEtypes_ElementId_t index, int mgmt_bitmap_index)
{
    unsigned int data_len = 0;
    return wifi_config_mgmt_ie(bss_type, HostCmd_ACT_GEN_SET, index, NULL, &data_len, mgmt_bitmap_index);
}

static int wifi_config_mgmt_ie2(
    mlan_bss_type bss_type, t_u16 action, t_u16 mask, void *buffer, unsigned int *ie_len, int mgmt_bitmap_index)
{
    uint8_t *buf;
    uint16_t buf_len           = 0;
    tlvbuf_custom_ie *tlv      = NULL;
    custom_ie *ie_ptr          = NULL;
    unsigned int mgmt_ie_index = -1;
    int total_len              = sizeof(tlvbuf_custom_ie) + (sizeof(custom_ie) - MAX_IE_SIZE) + *ie_len;
    int ret                    = WM_SUCCESS;

#if !CONFIG_MEM_POOLS
    buf = (uint8_t *)OSA_MemoryAllocate(total_len);
#else
    buf = OSA_MemoryPoolAllocate(buf_512_MemoryPool);
#endif
    if (buf == MNULL)
    {
        wifi_e("Cannot allocate memory");
        return -WM_FAIL;
    }

    (void)memset(buf, 0, total_len);

    tlv       = (tlvbuf_custom_ie *)(void *)buf;
    tlv->type = MRVL_MGMT_IE_LIST_TLV_ID;

    /* Locate headers */
    ie_ptr = (custom_ie *)(tlv->ie_data);
    /* Set TLV fields */
    buf_len = sizeof(tlvbuf_custom_ie);

    if (action == HostCmd_ACT_GEN_SET)
    {
        if (*ie_len == 0U)
        {
            /*
               MGMT_WPA_IE = MGMT_VENDOR_SPECIFIC_221
               MGMT_WPS_IE = MGMT_VENDOR_SPECIFIC_221
               */

            if (!ie_index_is_set(mgmt_bitmap_index))
            {
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(buf);
#else
                OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
                return -WM_FAIL;
            }

            ie_ptr->mgmt_subtype_mask = MGMT_MASK_CLEAR;
            ie_ptr->ie_length         = 0;
            ie_ptr->ie_index          = (t_u16)mgmt_bitmap_index;

            tlv->length = sizeof(custom_ie) - MAX_IE_SIZE;
            buf_len += tlv->length;
            clear_ie_index(mgmt_bitmap_index);
        }
        else
        {
            ret = get_free_mgmt_ie_index(&mgmt_ie_index);

            if (WM_SUCCESS != ret)
            {
#if !CONFIG_MEM_POOLS
                OSA_MemoryFree(buf);
#else
                OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
                return -WM_FAIL;
            }

            tlv->length      = (sizeof(custom_ie) - MAX_IE_SIZE) + *ie_len;
            ie_ptr->ie_index = mgmt_ie_index;

            ie_ptr->mgmt_subtype_mask = mask;

            ie_ptr->ie_length = *ie_len;

            buf_len += tlv->length;

            (void)memcpy((void *)&ie_ptr->ie_buffer, (const void *)buffer, *ie_len);
        }
    }
    else if (action == HostCmd_ACT_GEN_GET)
    {
        /* Get WPS IE */
        tlv->length = 0;
    }
    else
    { /* Do Nothing */
    }

    mlan_status rv = wrapper_wlan_cmd_mgmt_ie(bss_type, buf, buf_len, action);

    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }

    if (action == HostCmd_ACT_GEN_GET)
    {
        if (wm_wifi.cmd_resp_status != 0)
        {
            wifi_w("Unable to get mgmt ie buffer");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(buf);
#else
            OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif
            return wm_wifi.cmd_resp_status;
        }
        ie_ptr = (custom_ie *)(void *)(buf);
        (void)memcpy((void *)buffer, (const void *)ie_ptr->ie_buffer, ie_ptr->ie_length);
        *ie_len = ie_ptr->ie_length;
    }

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(buf);
#else
    OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif

    if ((action == HostCmd_ACT_GEN_SET) && *ie_len)
    {
        set_ie_index(mgmt_ie_index);
        return mgmt_ie_index;
    }
    else
    {
        return WM_SUCCESS;
    }
}

int wifi_get_mgmt_ie2(mlan_bss_type bss_type, void *buf, unsigned int *buf_len)
{
    return wifi_config_mgmt_ie2(bss_type, HostCmd_ACT_GEN_GET, 0, buf, buf_len, 0);
}

int wifi_set_mgmt_ie2(mlan_bss_type bss_type, unsigned short mask, void *buf, unsigned int buf_len)
{
    unsigned int data_len = buf_len;

    return wifi_config_mgmt_ie2(bss_type, HostCmd_ACT_GEN_SET, mask, buf, &data_len, 0);
}

int wifi_clear_mgmt_ie2(mlan_bss_type bss_type, int mgmt_bitmap_index)
{
    unsigned int data_len = 0;

    return wifi_config_mgmt_ie2(bss_type, HostCmd_ACT_GEN_SET, 0, NULL, &data_len, mgmt_bitmap_index);
}

#ifdef SD8801
int wifi_get_ext_coex_stats(wifi_ext_coex_stats_t *ext_coex_stats)
{
    if (ext_coex_stats == NULL)
    {
        wifi_e("Invalid structure passed");
        return -WM_FAIL;
    }

    return wifi_config_ext_coex(HostCmd_ACT_GEN_GET, NULL, ext_coex_stats);
}

int wifi_set_ext_coex_config(const wifi_ext_coex_config_t *ext_coex_config)
{
    if (ext_coex_config == NULL)
    {
        wifi_e("Invalid structure passed");
        return -WM_FAIL;
    }

    return wifi_config_ext_coex(HostCmd_ACT_GEN_SET, ext_coex_config, NULL);
}
#endif

#if CONFIG_WPA_SUPP
#if UAP_SUPPORT
int wifi_set_custom_ie(custom_ie *beacon_ies_data,
                       custom_ie *beacon_wps_ies_data,
                       custom_ie *proberesp_ies_data,
                       custom_ie *assocresp_ies_data)
{
    mlan_ds_misc_custom_ie *pcustom_ie = NULL;
    t_u8 *pos                          = NULL;
    t_u16 len                          = 0;
    mlan_status status                 = MLAN_STATUS_SUCCESS;
    t_u32 remain_len                   = 0;
    HostCmd_DS_COMMAND *cmd            = NULL;

    ENTER();

    pcustom_ie = OSA_MemoryAllocate(sizeof(mlan_ds_misc_custom_ie));
    if (!pcustom_ie)
    {
        PRINTM(MERROR, "Fail to allocate custome_ie\n");
        status = MLAN_STATUS_FAILURE;
        goto done;
    }

    pcustom_ie->type = TLV_TYPE_MGMT_IE;

    pos        = (t_u8 *)pcustom_ie->ie_data_list;
    remain_len = sizeof(pcustom_ie->ie_data_list);
    if (beacon_ies_data)
    {
        len = sizeof(*beacon_ies_data) - MAX_IE_SIZE + beacon_ies_data->ie_length;
        memcpy(pos, beacon_ies_data, len);
        pos += len;
        remain_len -= len;
        pcustom_ie->len += len;
    }

    if (beacon_wps_ies_data)
    {
        len = sizeof(*beacon_wps_ies_data) - MAX_IE_SIZE + beacon_wps_ies_data->ie_length;
        memcpy(pos, beacon_wps_ies_data, len);
        pos += len;
        remain_len -= len;
        pcustom_ie->len += len;
    }

    if (proberesp_ies_data)
    {
        len = sizeof(*proberesp_ies_data) - MAX_IE_SIZE + proberesp_ies_data->ie_length;
        memcpy(pos, proberesp_ies_data, len);
        pos += len;
        remain_len -= len;
        pcustom_ie->len += len;
    }

    if (assocresp_ies_data)
    {
        len = sizeof(*assocresp_ies_data) - MAX_IE_SIZE + assocresp_ies_data->ie_length;
        memcpy(pos, assocresp_ies_data, len);
        pos += len;
        remain_len -= len;
        pcustom_ie->len += len;
    }

    (void)wifi_get_command_lock();

    cmd = wifi_get_command_buffer();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, MLAN_BSS_TYPE_UAP);

    cmd->result = 0x0;

    status = wlan_ops_uap_prepare_cmd((mlan_private *)mlan_adap->priv[1], HOST_CMD_APCMD_SYS_CONFIGURE,
                                      HostCmd_ACT_GEN_SET, 0, NULL, (void *)pcustom_ie, cmd);

    (void)wifi_wait_for_cmdresp(NULL);

    OSA_MemoryFree(pcustom_ie);

done:
    LEAVE();
    return status;
}
#endif

void wifi_get_scan_table(mlan_private *pmpriv, mlan_scan_resp *pscan_resp)
{
    mlan_adapter *pmadapter = pmpriv->adapter;

    pscan_resp->pscan_table       = (t_u8 *)pmadapter->pscan_table;
    pscan_resp->num_in_scan_table = pmadapter->num_in_scan_table;
    pscan_resp->age_in_secs       = pmadapter->age_in_secs;
#if CONFIG_SCAN_CHANNEL_GAP
    pscan_resp->pchan_stats       = (t_u8 *)pmadapter->pchan_stats;
    pscan_resp->num_in_chan_stats = pmadapter->num_in_chan_stats;
#endif
}
#endif

int wifi_set_chanlist(wifi_chanlist_t *chanlist)
{
    mlan_status ret;
    t_u8 i         = 0;
    t_u8 cfp_no_bg = 0;
#if CONFIG_5GHz_SUPPORT
    t_u8 cfp_no_a = 0;
#endif
    mlan_adapter *pmadapter = mlan_adap->priv[0]->adapter;

#ifdef OTP_CHANINFO
    if ((pmadapter->otp_region == MNULL) || (pmadapter->otp_region->force_reg == 0U))
    {
#endif
        /*
         * Validate if the channels provided in the channel list
         * are valid channels according to World Wide Safe Mode.
         */
        for (i = 0; i < chanlist->num_chans; i++)
        {
            if (!wlan_is_channel_and_freq_valid(pmadapter, chanlist->chan_info[i].chan_num,
                                                chanlist->chan_info[i].chan_freq))
            {
                wifi_e("Invalid channel %d\r\n", chanlist->chan_info[i].chan_num);
                return -WM_FAIL;
            }
        }

        /* Configure Custom CFP Tables */
#if CONFIG_5GHz_SUPPORT
        ret = wlan_set_custom_cfp_table(chanlist, &cfp_no_bg, &cfp_no_a);
#else
    ret = wlan_set_custom_cfp_table(chanlist, &cfp_no_bg);
#endif
        if (ret != MLAN_STATUS_SUCCESS)
        {
            wifi_e("Failed to set Custom CFP Table");
            return -WM_FAIL;
        }

        /* Set Region Table */
#if CONFIG_5GHz_SUPPORT
        wlan_set_custom_regiontable((mlan_private *)mlan_adap->priv[0], cfp_no_bg, cfp_no_a);
#else
    wlan_set_custom_regiontable((mlan_private *)mlan_adap->priv[0], cfp_no_bg);
#endif
#ifdef OTP_CHANINFO
    }
#endif

    return WM_SUCCESS;
}

int wifi_get_chanlist(wifi_chanlist_t *chanlist)
{
    mlan_adapter *pmadapter      = mlan_adap->priv[0]->adapter;
    region_chan_t *pchan_region  = MNULL;
    const chan_freq_power_t *cfp = MNULL;
    t_u32 region_idx             = 0;
    t_u32 next_chan              = 0;
    chanlist->num_chans          = 0;

    for (region_idx = 0; region_idx < NELEMENTS(pmadapter->region_channel); region_idx++)
    {
        if (!pmadapter->region_channel[region_idx].valid)
        {
            continue;
        }

        pchan_region = &pmadapter->region_channel[region_idx];

        for (next_chan = 0; next_chan < pchan_region->num_cfp; next_chan++)
        {
            cfp = pchan_region->pcfp + next_chan;
            if (cfp == MNULL)
            {
                wifi_e("No cfp configured");
                return -WM_FAIL;
            }

            if ((cfp->dynamic.flags & NXP_CHANNEL_DISABLED) != 0U)
            {
                continue;
            }

            chanlist->chan_info[chanlist->num_chans].chan_num                     = cfp->channel;
            chanlist->chan_info[chanlist->num_chans].chan_freq                    = cfp->freq;
            chanlist->chan_info[chanlist->num_chans].passive_scan_or_radar_detect = cfp->passive_scan_or_radar_detect;
            chanlist->num_chans++;

            if (chanlist->num_chans >= NELEMENTS(chanlist->chan_info))
            {
                break;
            }
        }
    }

    return WM_SUCCESS;
}

#if UAP_SUPPORT
void wifi_get_active_channel_list(t_u8 *chan_list, t_u8 *num_chans, t_u16 acs_band)
{
    if (chan_list != MNULL && num_chans != MNULL)
    {
        wlan_get_active_channel_list((mlan_private *)mlan_adap->priv[1], chan_list, num_chans, acs_band);
    }
}
#endif

int wifi_set_txpwrlimit(wifi_txpwrlimit_t *txpwrlimit)
{
    t_u8 i;
    int ret;
    HostCmd_DS_COMMAND *cmd                = wifi_get_command_buffer();
    t_u8 *pByte                            = NULL;
    MrvlIETypes_ChanTRPCConfig_t *trpc_tlv = NULL;

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_CHANNEL_TRPC_CONFIG;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    cmd->size    = S_DS_GEN + 2U * sizeof(t_u16) +
                txpwrlimit->num_chans * (sizeof(MrvlIEtypesHeader_t) + sizeof(MrvlChannelDesc_t)) +
                (txpwrlimit->num_chans * txpwrlimit->txpwrlimit_config->num_mod_grps * sizeof(MrvlChanTrpcEntry_t));

    HostCmd_DS_CHAN_TRPC_CONFIG *txpwrlimit_config = (HostCmd_DS_CHAN_TRPC_CONFIG *)(void *)((uint8_t *)cmd + S_DS_GEN);

    txpwrlimit_config->action   = HostCmd_ACT_GEN_SET;
    txpwrlimit_config->reserved = txpwrlimit->subband;

    pByte = (t_u8 *)txpwrlimit_config->tlv_buffer;

    for (i = 0; i < txpwrlimit->num_chans; i++)
    {
        trpc_tlv              = (MrvlIETypes_ChanTRPCConfig_t *)(void *)pByte;
        trpc_tlv->header.type = TLV_TYPE_CHANNEL_TRPC_CONFIG;
        trpc_tlv->header.len =
            sizeof(MrvlChannelDesc_t) + txpwrlimit->txpwrlimit_config->num_mod_grps * sizeof(MrvlChanTrpcEntry_t);
        trpc_tlv->start_freq = txpwrlimit->txpwrlimit_config[i].chan_desc.start_freq;
        trpc_tlv->width      = txpwrlimit->txpwrlimit_config[i].chan_desc.chan_width;
        trpc_tlv->chan_num   = txpwrlimit->txpwrlimit_config[i].chan_desc.chan_num;
        (void)memcpy((void *)trpc_tlv->mod_group, (const void *)txpwrlimit->txpwrlimit_config[i].txpwrlimit_entry,
                     txpwrlimit->txpwrlimit_config->num_mod_grps * sizeof(MrvlChanTrpcEntry_t));
        pByte += trpc_tlv->header.len + sizeof(trpc_tlv->header);
    }
    ret = wifi_wait_for_cmdresp(NULL);
    return ret;
}

int wifi_get_txpwrlimit(wifi_SubBand_t subband, wifi_txpwrlimit_t *txpwrlimit)
{
    int ret;

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_CHANNEL_TRPC_CONFIG;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    cmd->size    = S_DS_GEN + 2U * sizeof(t_u16);

    HostCmd_DS_CHAN_TRPC_CONFIG *txpwrlimit_config = (HostCmd_DS_CHAN_TRPC_CONFIG *)(void *)((uint8_t *)cmd + S_DS_GEN);

    txpwrlimit_config->action   = HostCmd_ACT_GEN_GET;
    txpwrlimit_config->reserved = subband;

    ret = wifi_wait_for_cmdresp(txpwrlimit);
    return ret;
}

#if CONFIG_WIFI_RTS_THRESHOLD
int wifi_set_rts(int rts, mlan_bss_type bss_type)
{
    mlan_ioctl_req req;
    mlan_ds_snmp_mib *mib = NULL;
    mlan_status ret       = MLAN_STATUS_FAILURE;
#if UAP_SUPPORT
    wifi_sta_list_t *sl   = NULL;
#endif

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    /* Allocate an IOCTL request buffer */
#if !CONFIG_MEM_POOLS
    mib = OSA_MemoryAllocate(sizeof(mlan_ds_snmp_mib));
#else
    mib = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif

    if (mib == NULL)
        return -WM_FAIL;

    /* Fill request buffer */
    mib->sub_command = MLAN_OID_SNMP_MIB_RTS_THRESHOLD;
    req.pbuf         = (t_u8 *)mib;
    req.buf_len      = sizeof(mlan_ds_snmp_mib);
    req.req_id       = MLAN_IOCTL_SNMP_MIB;
    req.action       = MLAN_ACT_SET;
    req.bss_index    = bss_type;

    if (req.action == MLAN_ACT_SET)
    {
        if (rts < MLAN_RTS_MIN_VALUE || rts > MLAN_RTS_MAX_VALUE)
        {
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }
        mib->param.rts_threshold = rts;
    }

#if UAP_SUPPORT
    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        if (!is_uap_started())
        {
            wifi_e("uap isn't up\n\r");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }
        wifi_uap_bss_sta_list(&sl);
        if (!sl)
        {
            wifi_e("Failed to get sta list\n\r");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }
        if (sl->count >= 1)
            ret = wlan_ops_sta_ioctl(mlan_adap, &req);
        else
            wifi_e("uap required sta to connect before setting rts threshold\n\r");
    }
    else if (bss_type == MLAN_BSS_TYPE_STA)
#else
    if (bss_type == MLAN_BSS_TYPE_STA)
#endif
    {
        if (is_sta_connected())
            ret = wlan_ops_sta_ioctl(mlan_adap, &req);
        else
            wifi_e("sta connection required before setting rts threshold\n\r");
    }

    if (ret != MLAN_STATUS_SUCCESS && ret != MLAN_STATUS_PENDING)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(mib);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
        return -WM_FAIL;
    }

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(mib);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif

    return WM_SUCCESS;
}
#endif

#if CONFIG_WIFI_FRAG_THRESHOLD
int wifi_set_frag(int frag, mlan_bss_type bss_type)
{
    mlan_ioctl_req req;
    mlan_ds_snmp_mib *mib = NULL;
    mlan_status ret       = MLAN_STATUS_FAILURE;
#if UAP_SUPPORT
    wifi_sta_list_t *sl   = NULL;
#endif

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

    /* Allocate an IOCTL request buffer */
#if !CONFIG_MEM_POOLS
    mib = OSA_MemoryAllocate(sizeof(mlan_ds_snmp_mib));
#else
    mib = OSA_MemoryPoolAllocate(buf_128_MemoryPool);
#endif

    if (mib == NULL)
        return -WM_FAIL;

    /* Fill request buffer */
    mib->sub_command = MLAN_OID_SNMP_MIB_FRAG_THRESHOLD;
    req.pbuf         = (t_u8 *)mib;
    req.buf_len      = sizeof(mlan_ds_snmp_mib);
    req.req_id       = MLAN_IOCTL_SNMP_MIB;
    req.action       = MLAN_ACT_SET;
    req.bss_index    = bss_type;

    if (req.action == MLAN_ACT_SET)
    {
        if (frag < MLAN_FRAG_MIN_VALUE || frag > MLAN_FRAG_MAX_VALUE)
        {
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }
        mib->param.frag_threshold = frag;
    }

#if UAP_SUPPORT
    if (bss_type == MLAN_BSS_TYPE_UAP)
    {
        if (!is_uap_started())
        {
            wifi_e("uap isn't up\n\r");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }
        wifi_uap_bss_sta_list(&sl);
        if (!sl)
        {
            wifi_e("Failed to get sta list\n\r");
#if !CONFIG_MEM_POOLS
            OSA_MemoryFree(mib);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
            return -WM_FAIL;
        }

        if (sl->count >= 1)
            ret = wlan_ops_sta_ioctl(mlan_adap, &req);
        else
            wifi_e("uap required sta to connect before setting fragment threshold\n\r");
    }
    else if (bss_type == MLAN_BSS_TYPE_STA)
#else
    if (bss_type == MLAN_BSS_TYPE_STA)
#endif
    {
        if (is_sta_connected())
            ret = wlan_ops_sta_ioctl(mlan_adap, &req);
        else
            wifi_e("sta connection required before setting fragment threshold\n\r");
    }

    if (ret != MLAN_STATUS_SUCCESS && ret != MLAN_STATUS_PENDING)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(mib);
#else
        OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
        return -WM_FAIL;
    }
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(mib);
#else
    OSA_MemoryPoolFree(buf_128_MemoryPool, mib);
#endif
    return WM_SUCCESS;
}
#endif

void wifi_set_curr_bss_channel(uint8_t channel)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    pmpriv->curr_bss_params.bss_descriptor.channel = channel;
}


#if CONFIG_11K
int wifi_host_11k_cfg(int enable_11k)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
#if !CONFIG_WPA_SUPP
    IEEEtypes_RrmElement_t rrmCap;
#endif
    int ret = (int)MLAN_STATUS_SUCCESS;

    if (enable_11k == (int)pmpriv->enable_host_11k)
    {
        return (int)MLAN_STATUS_SUCCESS;
    }

#if !CONFIG_WPA_SUPP
    if (enable_11k == 1)
    {
        if (pmpriv->rrm_mgmt_bitmap_index != -1)
        {
            ret = wifi_clear_mgmt_ie(MLAN_BSS_TYPE_STA, MGMT_RRM_ENABLED_CAP, pmpriv->rrm_mgmt_bitmap_index);

            pmpriv->rrm_mgmt_bitmap_index = -1;
        }
        rrmCap.element_id = (t_u8)MGMT_RRM_ENABLED_CAP;
        rrmCap.len        = (t_u8)sizeof(IEEEtypes_RrmEnabledCapabilities_t);
        wlan_dot11k_formatRrmCapabilities(&(rrmCap.RrmEnabledCapabilities), 100);

        /* Append the passed data to the end of
         * the genIeBuffer */
        __memcpy(pmpriv->adapter, pmpriv->assoc_req_buf, &rrmCap, sizeof(IEEEtypes_RrmElement_t));

        /* Increment the stored buffer length by
         * the size passed */
        pmpriv->assoc_req_size = sizeof(IEEEtypes_RrmElement_t);
    }
    else
    {
        pmpriv->assoc_req_size = 0;
    }
#endif

    pmpriv->enable_host_11k = (t_u8)enable_11k;

    return ret;
}

int wifi_host_11k_neighbor_req(const char *ssid)
{
    if (wlan_strlen((t_s8 *)ssid) > IEEEtypes_SSID_SIZE)
    {
        return -WM_FAIL;
    }
    else
    {
        return wlan_send_mgmt_rm_neighbor_request(mlan_adap->priv[0], (t_u8 *)ssid, (t_u8)wlan_strlen((t_s8 *)ssid));
    }
}
#endif

#if CONFIG_11V
int wifi_host_11v_bss_trans_query(t_u8 query_reason)
{
    return wlan_send_mgmt_bss_trans_query(mlan_adap->priv[0], query_reason);
}
#endif

#if CONFIG_DRIVER_MBO
int wifi_host_mbo_cfg(int enable_mbo)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    IEEEtypes_VendorSpecific_t mboie;
    int ret = (int)MLAN_STATUS_SUCCESS;
    t_u8 *pos;
    int meas_vend_hdr_len = 0;

    if ((t_u8)enable_mbo == pmpriv->enable_mbo)
    {
        /* Do nothing */
        return (int)MLAN_STATUS_SUCCESS;
    }

    if (enable_mbo != 0)
    {
        mboie.vend_hdr.element_id = (IEEEtypes_ElementId_e)MGMT_MBO_IE;
        pos                       = mboie.vend_hdr.oui;
        pos                       = wlan_add_mbo_oui(pos);
        pos                       = wlan_add_mbo_oui_type(pos);
        pos                       = wlan_add_mbo_cellular_cap(pos);
        meas_vend_hdr_len         = pos - mboie.vend_hdr.oui;
        mboie.vend_hdr.len        = (t_u8)meas_vend_hdr_len;
        pmpriv->mbo_mgmt_bitmap_index =
            wifi_set_mgmt_ie(MLAN_BSS_TYPE_STA, MGMT_MBO_IE, (void *)&(mboie.vend_hdr.oui), mboie.vend_hdr.len);
    }
    else
    {
        ret = wifi_clear_mgmt_ie(MLAN_BSS_TYPE_STA, MGMT_MBO_IE, pmpriv->mbo_mgmt_bitmap_index);
    }
    pmpriv->enable_mbo = (t_u8)enable_mbo;

    return ret;
}

int wifi_mbo_preferch_cfg(t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    IEEEtypes_VendorSpecific_t mboie;
    int ret = (int)MLAN_STATUS_SUCCESS;
    t_u8 *pos;
    int meas_vend_hdr_len = 0;

    if (0U == pmpriv->enable_mbo)
    {
        wifi_e("Please enable MBO first!");
        return (int)MLAN_STATUS_FAILURE;
    }

    if (pmpriv->enable_mbo != 0U)
    {
        /* remove MBO OCE IE in case there is already a MBO OCE IE. */
        ret                       = wifi_clear_mgmt_ie(MLAN_BSS_TYPE_STA, MGMT_MBO_IE, pmpriv->mbo_mgmt_bitmap_index);
        mboie.vend_hdr.element_id = (IEEEtypes_ElementId_e)MGMT_MBO_IE;
        pos                       = mboie.vend_hdr.oui;
        pos                       = wlan_add_mbo_oui(pos);
        pos                       = wlan_add_mbo_oui_type(pos);
        pos                       = wlan_add_mbo_cellular_cap(pos);
        pos                       = wlan_add_mbo_prefer_ch(pos, ch0, pefer0, ch1, pefer1);
        meas_vend_hdr_len         = pos - mboie.vend_hdr.oui;
        mboie.vend_hdr.len        = (t_u8)meas_vend_hdr_len;
        pmpriv->mbo_mgmt_bitmap_index =
            wifi_set_mgmt_ie(MLAN_BSS_TYPE_STA, MGMT_MBO_IE, (void *)&(mboie.vend_hdr.oui), mboie.vend_hdr.len);
    }

    return ret;
}

int wifi_mbo_send_preferch_wnm(t_u8 *src_addr, t_u8 *target_bssid, t_u8 ch0, t_u8 pefer0, t_u8 ch1, t_u8 pefer1)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    int ret              = MLAN_STATUS_SUCCESS;
    t_u8 *buf, *pos, *pos_len1, *pos_len2;
    t_u8 global_oper_class_0 = 0, global_oper_class_1 = 0, num = 0, i;

    if (0 == pmpriv->enable_mbo)
    {
        wifi_e("Please enable MBO first!\r\n");
        return MLAN_STATUS_FAILURE;
    }

    if (pmpriv->enable_mbo)
    {
#if !CONFIG_MEM_POOLS
        buf = OSA_MemoryAllocate(sizeof(IEEEtypes_VendorSpecific_t));
#else
        buf = OSA_MemoryPoolAllocate(buf_512_MemoryPool);
#endif
        pos = buf;

        /* No non-preferred channels */
        if (!ch0 && !pefer0 && !ch1 && !pefer1)
        {
            *pos = MGMT_MBO_IE;
            pos++;
            *pos = 4;
            pos++;
            pos = wlan_add_mbo_oui(pos);
            pos = wlan_add_mbo_attr_id(pos);
        }
        else
        {
            wlan_get_curr_oper_class(pmpriv, ch0, BW_20MHZ, &global_oper_class_0);
            wlan_get_curr_oper_class(pmpriv, ch1, BW_20MHZ, &global_oper_class_1);
            if (global_oper_class_0 != global_oper_class_1 || pefer0 != pefer1)
                num = 2;
            else
                num = 1;

            for (i = 0; i < num; i++)
            {
                *pos = MGMT_MBO_IE;
                pos++;
                if (i == 0)
                    pos_len1 = pos;
                else
                    pos_len2 = pos;
                pos++;
                pos = wlan_add_mbo_oui(pos);
                pos = wlan_add_mbo_attr_id(pos);
                if (num == 1)
                {
                    pos[0] = global_oper_class_0;
                    pos[1] = ch0;
                    pos[2] = ch1;
                    pos[3] = pefer0;
                    pos += 4;
                }
                else
                {
                    if (i == 0)
                    {
                        pos[0] = global_oper_class_0;
                        pos[1] = ch0;
                        pos[2] = pefer0;
                    }
                    else
                    {
                        pos[0] = global_oper_class_1;
                        pos[1] = ch1;
                        pos[2] = pefer1;
                    }
                    pos += 3;
                }

                /* Reason code */
                *pos = 0;
                pos++;

                if (i == 0)
                    *pos_len1 = pos - (pos_len1 + 1);
                else
                    *pos_len2 = pos - (pos_len2 + 1);
            }
        }
        wlan_send_mgmt_wnm_notification(src_addr, target_bssid, target_bssid, buf, pos - buf, false);
    }
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(buf);
#else
    OSA_MemoryPoolFree(buf_512_MemoryPool, buf);
#endif

    return ret;
}
#endif

#ifdef OTP_CHANINFO
int wifi_get_fw_region_and_cfp_tables(void)
{
    int ret;

    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_CHAN_REGION_CFG;
    cmd->seq_num = 0x0;
    cmd->result  = 0x0;
    cmd->size    = S_DS_GEN + sizeof(HostCmd_DS_CHAN_REGION_CFG);

    HostCmd_DS_CHAN_REGION_CFG *chan_region_cfg = (HostCmd_DS_CHAN_REGION_CFG *)(void *)((uint8_t *)cmd + S_DS_GEN);

    chan_region_cfg->action = HostCmd_ACT_GEN_GET;

    ret = wifi_wait_for_cmdresp(NULL);
    return ret;
}

void wifi_free_fw_region_and_cfp_tables(void)
{
    mlan_adapter *pmadapter = mlan_adap->priv[0]->adapter;
    wlan_free_fw_cfp_tables(pmadapter);
}
#endif

int wifi_set_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl, int bss_type)
{
    int ret;

    if (wifi_ed_mac_ctrl == MNULL)
    {
        return -WM_FAIL;
    }

    mlan_private *pmpriv    = (mlan_private *)mlan_adap->priv[bss_type];
    CHECK_BSS_TYPE(bss_type, -WM_FAIL);
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    (void)wifi_get_command_lock();

    cmd->command = HostCmd_CMD_ED_MAC_MODE;
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->result  = 0x0;
    cmd->size    = S_DS_GEN + sizeof(HostCmd_CONFIG_ED_MAC_MODE);

    HostCmd_CONFIG_ED_MAC_MODE *ed_mac_mode = (HostCmd_CONFIG_ED_MAC_MODE *)(void *)((uint8_t *)cmd + S_DS_GEN);

    ed_mac_mode->ed_ctrl_2g   = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_ctrl_2g);
    ed_mac_mode->ed_offset_2g = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_offset_2g);
#if CONFIG_5GHz_SUPPORT
    ed_mac_mode->ed_ctrl_5g   = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_ctrl_5g);
    ed_mac_mode->ed_offset_5g = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_offset_5g);
#if defined(SD9177) || defined(IW610)
    ed_mac_mode->ed_bitmap_txq_lock = 0x1e00ff;
#else
    ed_mac_mode->ed_bitmap_txq_lock = 0xff;
#endif
#endif

    pmpriv->ed_mac_mode.ed_ctrl_2g   = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_ctrl_2g);
    pmpriv->ed_mac_mode.ed_offset_2g = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_offset_2g);
#if CONFIG_5GHz_SUPPORT
    pmpriv->ed_mac_mode.ed_ctrl_5g   = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_ctrl_5g);
    pmpriv->ed_mac_mode.ed_offset_5g = wlan_cpu_to_le16(wifi_ed_mac_ctrl->ed_offset_5g);
#endif

    ret = wifi_wait_for_cmdresp(NULL);
    return ret;
}

int wifi_get_ed_mac_mode(wifi_ed_mac_ctrl_t *wifi_ed_mac_ctrl, int bss_type)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[bss_type];
    CHECK_BSS_TYPE(bss_type, -WM_FAIL);

    if (wifi_ed_mac_ctrl == MNULL)
    {
        return -WM_FAIL;
    }

    (void)memset(wifi_ed_mac_ctrl, 0x00, sizeof(wifi_ed_mac_ctrl_t));

    wifi_ed_mac_ctrl->ed_ctrl_2g   = wlan_cpu_to_le16(pmpriv->ed_mac_mode.ed_ctrl_2g);
    wifi_ed_mac_ctrl->ed_offset_2g = wlan_cpu_to_le16(pmpriv->ed_mac_mode.ed_offset_2g);
#if CONFIG_5GHz_SUPPORT
    wifi_ed_mac_ctrl->ed_ctrl_5g   = wlan_cpu_to_le16(pmpriv->ed_mac_mode.ed_ctrl_5g);
    wifi_ed_mac_ctrl->ed_offset_5g = wlan_cpu_to_le16(pmpriv->ed_mac_mode.ed_offset_5g);
#endif

    return WM_SUCCESS;
}

#ifndef IEEEtypes_SSID_SIZE
#define IEEEtypes_SSID_SIZE 32
#endif /* IEEEtypes_SSID_SIZE */
#if UAP_SUPPORT
#define MRVL_SSID_TLV_ID          0x0000
#define MRVL_BEACON_PERIOD_TLV_ID (PROPRIETARY_TLV_BASE_ID + 0x2cU)
#define TLV_TYPE_SMCADDRRANGE     (PROPRIETARY_TLV_BASE_ID + 0xccU)
#define TLV_TYPE_SMCFRAMEFILTER   (PROPRIETARY_TLV_BASE_ID + 0xd1U)

int wifi_set_smart_mode_cfg(char *ssid,
                            int beacon_period,
                            wifi_chan_list_param_set_t *chan_list,
                            uint8_t *smc_start_addr,
                            uint8_t *smc_end_addr,
                            uint16_t filter_type,
                            int smc_frame_filter_len,
                            uint8_t *smc_frame_filter,
                            int custom_ie_len,
                            uint8_t *custom_ie)
{
    unsigned int ssid_len                              = 0, i;
    uint32_t size                                      = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;
    MrvlIEtypes_SsIdParamSet_t *tlv_ssid               = NULL;
    MrvlIEtypes_beacon_period_t *tlv_beacon_period     = NULL;
    MrvlIEtypes_ChanListParamSet_t *tlv_chan_list      = NULL;
    MrvlIEtypes_Data_t *tlv_custom_ie                  = NULL;
    MrvlIETypes_SmcAddrRange_t *tlv_smc_addr_range     = NULL;
    MrvlIETypes_SmcFrameFilter_t *tlv_smc_frame_filter = NULL;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_SMART_MODE_CFG);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_SET;
    uint8_t *tlv                          = (uint8_t *)sys_config_cmd->tlv_buffer;

    ssid_len = strlen(ssid);
    if (ssid_len > IEEEtypes_SSID_SIZE || custom_ie_len > 255)
    {
        return -WM_E_INVAL;
    };

    tlv_ssid              = (MrvlIEtypes_SsIdParamSet_t *)(void *)sys_config_cmd->tlv_buffer;
    tlv_ssid->header.type = MRVL_SSID_TLV_ID;
    tlv_ssid->header.len  = strlen(ssid);
    (void)memcpy((void *)tlv_ssid->ssid, (const void *)ssid, strlen(ssid));
    size += sizeof(tlv_ssid->header) + tlv_ssid->header.len;
    tlv += sizeof(tlv_ssid->header) + tlv_ssid->header.len;
    tlv_beacon_period                = (MrvlIEtypes_beacon_period_t *)(void *)tlv;
    tlv_beacon_period->header.type   = MRVL_BEACON_PERIOD_TLV_ID;
    tlv_beacon_period->header.len    = sizeof(uint16_t);
    tlv_beacon_period->beacon_period = beacon_period;

    size += sizeof(tlv_beacon_period->header) + tlv_beacon_period->header.len;
    tlv += sizeof(tlv_beacon_period->header) + tlv_beacon_period->header.len;

    tlv_chan_list              = (MrvlIEtypes_ChanListParamSet_t *)(void *)tlv;
    tlv_chan_list->header.type = TLV_TYPE_CHANLIST;
    tlv_chan_list->header.len  = chan_list->no_of_channels * sizeof(ChanScanParamSet_t);

    for (i = 0; i < chan_list->no_of_channels; i++)
    {
        tlv_chan_list->chan_scan_param[i].chan_number   = chan_list->chan_scan_param[i].chan_number;
        tlv_chan_list->chan_scan_param[i].min_scan_time = chan_list->chan_scan_param[i].min_scan_time;
        tlv_chan_list->chan_scan_param[i].max_scan_time = chan_list->chan_scan_param[i].max_scan_time;
    }

    size += sizeof(tlv_chan_list->header) + tlv_chan_list->header.len;
    tlv += sizeof(tlv_chan_list->header) + tlv_chan_list->header.len;

    if (custom_ie != MNULL && custom_ie_len > 0)
    {
        tlv_custom_ie              = (MrvlIEtypes_Data_t *)(void *)tlv;
        tlv_custom_ie->header.type = TLV_TYPE_PASSTHROUGH;
        tlv_custom_ie->header.len  = custom_ie_len;
        (void)memcpy((void *)tlv_custom_ie->data, (const void *)custom_ie, custom_ie_len);

        size += sizeof(tlv_custom_ie->header) + tlv_custom_ie->header.len;
        tlv += sizeof(tlv_custom_ie->header) + tlv_custom_ie->header.len;
    }

    if (smc_start_addr != MNULL && smc_end_addr != MNULL)
    {
        tlv_smc_addr_range              = (MrvlIETypes_SmcAddrRange_t *)(void *)tlv;
        tlv_smc_addr_range->header.type = TLV_TYPE_SMCADDRRANGE;
        tlv_smc_addr_range->header.len  = 2U * MLAN_MAC_ADDR_LENGTH + sizeof(uint16_t);

        (void)memcpy((void *)tlv_smc_addr_range->smcstartAddr, (const void *)smc_start_addr, MLAN_MAC_ADDR_LENGTH);
        (void)memcpy((void *)tlv_smc_addr_range->smcendAddr, (const void *)smc_end_addr, MLAN_MAC_ADDR_LENGTH);

        tlv_smc_addr_range->filter_type = filter_type;

        size += sizeof(tlv_smc_addr_range->header) + tlv_smc_addr_range->header.len;
        tlv += sizeof(tlv_smc_addr_range->header) + tlv_smc_addr_range->header.len;
    }

    tlv_smc_frame_filter              = (MrvlIETypes_SmcFrameFilter_t *)(void *)tlv;
    tlv_smc_frame_filter->header.type = TLV_TYPE_SMCFRAMEFILTER;
    tlv_smc_frame_filter->header.len  = smc_frame_filter_len;
    (void)memcpy((void *)tlv_smc_frame_filter->frame_filter, (const void *)smc_frame_filter, smc_frame_filter_len);

    size += sizeof(tlv_smc_frame_filter->header) + tlv_smc_frame_filter->header.len;
    tlv += sizeof(tlv_smc_frame_filter->header) + tlv_smc_frame_filter->header.len;

    cmd->size    = size;
    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(NULL);

    return WM_SUCCESS;
}

int wifi_get_smart_mode_cfg(void)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_SMART_MODE_CFG);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_GET;

    cmd->size    = size;
    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_start_smart_mode(void)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_SMART_MODE_CFG);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_START;

    cmd->size    = size;
    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(NULL);
    return WM_SUCCESS;
}

int wifi_stop_smart_mode(void)
{
    uint32_t size = S_DS_GEN + sizeof(HostCmd_DS_SYS_CONFIG) - 1U;

    (void)wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                          = wlan_cpu_to_le16(HOST_CMD_SMART_MODE_CFG);
    HostCmd_DS_SYS_CONFIG *sys_config_cmd = (HostCmd_DS_SYS_CONFIG *)((uint32_t)cmd + S_DS_GEN);
    sys_config_cmd->action                = HostCmd_ACT_GEN_STOP;

    cmd->size    = size;
    cmd->seq_num = 0x00;
    cmd->result  = 0x00;

    (void)wifi_wait_for_cmdresp(NULL);

    return WM_SUCCESS;
}
#endif /* UAP_SUPPORT */

#if CONFIG_BG_SCAN
void wifi_get_band(mlan_private *pmpriv, int *band)
{
    int support_band = 0;

    if (pmpriv->config_bands & (BAND_B | BAND_G | BAND_GN))
        support_band |= WIFI_FREQUENCY_BAND_2GHZ;
#if CONFIG_5GHz_SUPPORT
    if (pmpriv->config_bands & (BAND_A | BAND_AN))
        support_band |= WIFI_FREQUENCY_BAND_5GHZ;
#endif
    *band = support_band;
    if (support_band == WIFI_FREQUENCY_ALL_BAND)
        *band = WIFI_FREQUENCY_BAND_AUTO;
}

int wifi_get_bgscan_results(mlan_private *pmpriv)
{
    mlan_adapter *pmadapter = pmpriv->adapter;
    int ret                 = 0;
#if CONFIG_WPA_SUPP
    BSSDescriptor_t *bss_entry = NULL;
    int i;
#endif

    ENTER();

#if CONFIG_WPA_SUPP
    pmadapter->wpa_supp_scan_triggered = MTRUE;
    for (i = 0; i < pmadapter->num_in_scan_table; i++)
    {
        bss_entry = &pmadapter->pscan_table[i];
        if (bss_entry && bss_entry->ies != NULL)
        {
            OSA_MemoryFree(bss_entry->ies);
        }
    }
#endif

    memset(pmadapter->pscan_table, 0x00, sizeof(BSSDescriptor_t) * MRVDRV_MAX_BSSID_LIST);
    pmadapter->num_in_scan_table = 0;
    ret                          = wifi_request_bgscan_query(pmpriv);
    pmadapter->bgscan_reported   = MFALSE;
    LEAVE();
    return ret;
}

int wifi_send_scan_query(void)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    int ret              = 0;

    ENTER();
    ret = wifi_get_bgscan_results(pmpriv);
    if (ret)
    {
        PRINTM(MERROR, "Failed to get scan results\n");
        goto done;
    }
done:
    /* config rssi low threshold again */
    pmpriv->rssi_low = DEFAULT_RSSI_LOW_THRESHOLD;
    LEAVE();
    return ret;
}
#endif

int wifi_send_hostcmd(
    const void *cmd_buf, uint32_t cmd_buf_len, void *resp_buf, uint32_t resp_buf_len, uint32_t *reqd_resp_len)
{
    uint32_t ret = WM_SUCCESS;
    /* Store IN & OUT params to be used by driver to update internaally*/
    /* These variables are updated from reponse handlers */
    wm_wifi.hostcmd_cfg.resp_buf      = resp_buf;
    wm_wifi.hostcmd_cfg.resp_buf_len  = resp_buf_len;
    wm_wifi.hostcmd_cfg.reqd_resp_len = reqd_resp_len;

    /* Check if command is larger than the command size that can be handled by firmware */
    if (cmd_buf_len > WIFI_FW_CMDBUF_SIZE)
    {
        *reqd_resp_len = 0;
        return WM_E_INBIG;
    }
    else if (cmd_buf_len < WIFI_HOST_CMD_FIXED_HEADER_LEN)
    /* Check if command is smaller than the minimum command size needed, which is WIFI_HOST_CMD_FIXED_HEADER_LEN */
    {
        *reqd_resp_len = 0;
        return WM_E_INSMALL;
    }
    else
    {
        /* Do Nothing */
    }
    (void)wifi_get_command_lock();
    /* Copy command buffer to driver command buffer */
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memcpy((void *)cmd, (const void *)cmd_buf, cmd_buf_len);

    /* Set global variable to say that this command is from user invocation */
    wm_wifi.hostcmd_cfg.is_hostcmd = true;
    (void)wifi_wait_for_cmdresp(&wm_wifi.hostcmd_cfg);

    if (*reqd_resp_len > resp_buf_len)
    {
        ret = WM_E_OUTBIG;
    }
    /*Response fail check not checked here, as thats caller's responsibility */
    return ret;
}

#if CONFIG_WIFI_EU_CRYPTO
int wifi_set_eu_crypto(EU_Crypto *Crypto_Data, enum _crypto_algorithm Algorithm, t_u16 EncDec)
{
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    t_u16 cmd_size;
    t_u16 *DataLength = Crypto_Data->DataLength;

    wifi_get_command_lock();

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->command = HostCmd_CMD_EU_CRYPTO;
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_ROLE_STA);

    switch (Algorithm)
    {
        case CRYPTO_RC4:
        case CRYPTO_AES_ECB:
        case CRYPTO_AES_WRAP:
        {
            cmd_size                        = sizeof(HostCmd_DS_EU_CRYPTO) - 1 + 8 /*cmd header */;
            cmd->params.eu_crypto.Algorithm = Algorithm;
            cmd->params.eu_crypto.KeyLength = Crypto_Data->KeyLength;
            memcpy(cmd->params.eu_crypto.Key, Crypto_Data->Key, Crypto_Data->KeyLength);
            cmd->params.eu_crypto.KeyIVLength = Crypto_Data->KeyIVLength;
            memcpy(cmd->params.eu_crypto.KeyIV, Crypto_Data->KeyIV, Crypto_Data->KeyIVLength);
            cmd->params.eu_crypto.DataLength = *DataLength;
            memcpy(cmd->params.eu_crypto.Data, Crypto_Data->Data, *DataLength);
            cmd_size += cmd->params.eu_crypto.DataLength;
            cmd->params.eu_crypto.EncDec   = EncDec;
            cmd->params.eu_crypto.DataType = 0x0111;
            break;
        }
        case CRYPTO_AES_CCMP:
        case CRYPTO_AES_GCMP:
        {
            cmd_size                            = sizeof(HostCmd_DS_EU_AES_CRYPTO) - 1 + 8 /* cmd header */;
            cmd->params.eu_aes_crypto.Algorithm = Algorithm;
            cmd->params.eu_aes_crypto.KeyLength = Crypto_Data->KeyLength;
            memcpy(cmd->params.eu_aes_crypto.Key, Crypto_Data->Key, Crypto_Data->KeyLength);
            cmd->params.eu_aes_crypto.NonceLength = Crypto_Data->NonceLength;
            memcpy(cmd->params.eu_aes_crypto.Nonce, Crypto_Data->Nonce, Crypto_Data->NonceLength);
            cmd->params.eu_aes_crypto.AADLength = Crypto_Data->AADLength;
            memcpy(cmd->params.eu_aes_crypto.AAD, Crypto_Data->AAD, Crypto_Data->AADLength);
            cmd->params.eu_aes_crypto.DataLength = *DataLength;
            memcpy(cmd->params.eu_aes_crypto.Data, Crypto_Data->Data, *DataLength);
            cmd_size += cmd->params.eu_aes_crypto.DataLength;
            cmd->params.eu_aes_crypto.EncDec   = EncDec;
            cmd->params.eu_aes_crypto.DataType = 0x0111;
            break;
        }
        default:
            return -WM_FAIL;
    }
    cmd->size = cmd_size;

    return wifi_wait_for_cmdresp(Crypto_Data);
}
#endif

int wifi_set_rx_mgmt_indication(unsigned int bss_type, unsigned int mgmt_subtype_mask)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[bss_type];
    CHECK_BSS_TYPE(bss_type, -WM_FAIL);

    mlan_ds_rx_mgmt_indication rx_mgmt_indication;

    memset(&rx_mgmt_indication, 0x00, sizeof(mlan_ds_rx_mgmt_indication));

    rx_mgmt_indication.mgmt_subtype_mask = mgmt_subtype_mask;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command = HostCmd_CMD_RX_MGMT_IND;
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0U /* seq_num */, 0U /* bss_num */, bss_type);
    cmd->result = 0x0;

    wlan_cmd_rx_mgmt_indication(pmpriv, cmd, HostCmd_ACT_GEN_SET, &rx_mgmt_indication);

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_get_set_bandcfg(wifi_bandcfg_t *bandcfg, mlan_act_ioctl action)
{
    mlan_ioctl_req req;
    mlan_ds_radio_cfg radio_cfg;

    if ((action != MLAN_ACT_GET) && (action != MLAN_ACT_SET))
    {
        return -WM_FAIL;
    }

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&radio_cfg, 0x00, sizeof(mlan_ds_radio_cfg));
    radio_cfg.sub_command = MLAN_OID_BAND_CFG;

    req.pbuf      = (t_u8 *)&radio_cfg;
    req.buf_len   = sizeof(mlan_ds_radio_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_RADIO_CFG;
    req.action    = action;
    if (action == MLAN_ACT_SET)
    {
        radio_cfg.param.band_cfg.config_bands = bandcfg->config_bands;
    }

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        return -WM_FAIL;
    }

    if (action == MLAN_ACT_GET)
    {
        bandcfg->config_bands = radio_cfg.param.band_cfg.config_bands;
        bandcfg->fw_bands = radio_cfg.param.band_cfg.fw_bands;
    }

    return WM_SUCCESS;
}

#if CONFIG_WPS2
/* enable/disable WPS session */
int wifi_send_wps_cfg_cmd(int option)
{
    mlan_ioctl_req req;
    mlan_ds_wps_cfg pwps;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&pwps, 0x00, sizeof(mlan_ds_wps_cfg));
    pwps.sub_command = MLAN_OID_WPS_CFG_SESSION;
    if (option)
        pwps.param.wps_session = MLAN_WPS_CFG_SESSION_START;
    else
        pwps.param.wps_session = MLAN_WPS_CFG_SESSION_END;
    req.pbuf      = (t_u8 *)&pwps;
    req.buf_len   = sizeof(mlan_ds_wps_cfg);
    req.bss_index = 0;
    req.req_id    = MLAN_IOCTL_WPS_CFG;
    req.action    = MLAN_ACT_SET;

    mlan_status rv = wlan_ops_sta_ioctl(mlan_adap, &req);
    if (rv != MLAN_STATUS_SUCCESS && rv != MLAN_STATUS_PENDING)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}
#endif /* CONFIG_WPS2 */

wlan_mgmt_pkt *wifi_PrepDefaultMgtMsg(t_u8 sub_type,
                                      mlan_802_11_mac_addr *DestAddr,
                                      mlan_802_11_mac_addr *SrcAddr,
                                      mlan_802_11_mac_addr *Bssid,
                                      t_u16 pkt_len)
{
    wlan_mgmt_pkt *pmgmt_pkt_hdr    = MNULL;
    IEEEtypes_FrameCtl_t *mgmt_fc_p = MNULL;
    t_u8 *pBuf                      = MNULL;

#if !CONFIG_MEM_POOLS
    pBuf = OSA_MemoryAllocate(pkt_len);
#else
    pBuf         = OSA_MemoryPoolAllocate(buf_1536_MemoryPool);
#endif

    if (pBuf == MNULL)
    {
        return MNULL;
    }

    pmgmt_pkt_hdr = (wlan_mgmt_pkt *)(void *)pBuf;
    /* 802.11 header */
    mgmt_fc_p           = (IEEEtypes_FrameCtl_t *)(void *)&pmgmt_pkt_hdr->wlan_header.frm_ctl;
    mgmt_fc_p->sub_type = sub_type;
    mgmt_fc_p->type     = (t_u8)IEEE_TYPE_MANAGEMENT;
    (void)memcpy(pmgmt_pkt_hdr->wlan_header.addr1, DestAddr, MLAN_MAC_ADDR_LENGTH);
    (void)memcpy(pmgmt_pkt_hdr->wlan_header.addr2, SrcAddr, MLAN_MAC_ADDR_LENGTH);
    (void)memcpy(pmgmt_pkt_hdr->wlan_header.addr3, Bssid, MLAN_MAC_ADDR_LENGTH);

    return pmgmt_pkt_hdr;
}

#if CONFIG_ECSA
wifi_ecsa_status_control ecsa_status_control = {false, 0};

void set_ecsa_block_tx_time(t_u8 switch_count)
{
    ecsa_status_control.block_time = switch_count;
}

t_u8 get_ecsa_block_tx_time()
{
    return ecsa_status_control.block_time;
}

void set_ecsa_block_tx_flag(bool block_tx)
{
    ecsa_status_control.required = block_tx;
}

bool get_ecsa_block_tx_flag()
{
    return ecsa_status_control.required;
}

void wifi_put_ecsa_sem()
{
    OSA_SemaphorePost((osa_semaphore_handle_t)ecsa_status_control.ecsa_sem);
}

int wlan_get_nonglobal_operclass_by_bw_channel(t_u8 bandwidth, t_u8 channel, t_u8 *oper_class)
{
    int ret = 0;
    mlan_ioctl_req req;
    mlan_ds_misc_cfg *misc = NULL;
    mlan_status status     = MLAN_STATUS_SUCCESS;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

#if !CONFIG_MEM_POOLS
    misc = OSA_MemoryAllocate(sizeof(mlan_ds_misc_cfg));
#else
    misc = OSA_MemoryPoolAllocate(buf_3072_MemoryPool);
#endif

    if (misc == NULL)
    {
        return -WM_FAIL;
    }

    req.bss_index                      = MLAN_BSS_ROLE_UAP;
    req.pbuf                           = (t_u8 *)misc;
    misc->sub_command                  = MLAN_OID_MISC_OPER_CLASS;
    req.req_id                         = MLAN_IOCTL_MISC_CFG;
    req.action                         = MLAN_ACT_GET;
    misc->param.bw_chan_oper.bandwidth = bandwidth;
    misc->param.bw_chan_oper.channel   = channel;

    status = wlan_ops_uap_ioctl(mlan_adap, &req);
    if (status != MLAN_STATUS_SUCCESS)
    {
        wifi_e("Failed to get operclass");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(misc);
#else
        OSA_MemoryPoolFree(buf_3072_MemoryPool, misc);
#endif
        return -WM_FAIL;
    }
    *oper_class = misc->param.bw_chan_oper.oper_class;

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(misc);
#else
    OSA_MemoryPoolFree(buf_3072_MemoryPool, misc);
#endif

    return ret;
}

int wifi_set_ecsa_cfg(t_u8 block_tx, t_u8 oper_class, t_u8 channel, t_u8 switch_count, t_u8 band_width, t_u8 ecsa)
{
    IEEEtypes_ExtChanSwitchAnn_t *ext_chan_switch = NULL;
    IEEEtypes_ChanSwitchAnn_t *chan_switch        = NULL;
    custom_ie *pcust_chansw_ie                    = NULL;
    t_u32 usr_dot_11n_dev_cap                     = 0;
#if CONFIG_5GHz_SUPPORT
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[1];
    BSSDescriptor_t *pbss_desc;
    pbss_desc = &pmpriv->curr_bss_params.bss_descriptor;
#endif
    t_u8 new_oper_class = oper_class;
    t_u8 bw;
    int ret = MLAN_STATUS_SUCCESS;
#if (CONFIG_11AC)
    t_u8 center_freq_idx                       = 0;
    IEEEtypes_Header_t *pChanSwWrap_ie         = NULL;
    IEEEtypes_WideBWChanSwitch_t *pbwchansw_ie = NULL;
    IEEEtypes_VhtTpcEnvelope_t *pvhttpcEnv_ie  = NULL;
#endif
    uint8_t *buf               = NULL;
    tlvbuf_custom_ie *tlv      = NULL;
    unsigned int mgmt_ie_index = -1;
    int total_len = sizeof(tlvbuf_custom_ie) + (sizeof(custom_ie) - MAX_IE_SIZE) + sizeof(IEEEtypes_ChanSwitchAnn_t) +
                    sizeof(IEEEtypes_ExtChanSwitchAnn_t);
#if (CONFIG_11AC)
    total_len += sizeof(IEEEtypes_WideBWChanSwitch_t) + sizeof(IEEEtypes_VhtTpcEnvelope_t) + sizeof(IEEEtypes_Header_t);
#endif
    uint16_t buf_len = 0;

#if !CONFIG_MEM_POOLS
    buf = (uint8_t *)OSA_MemoryAllocate(total_len);
#else
    buf = OSA_MemoryPoolAllocate(buf_1024_MemoryPool);
#endif
    if (!buf)
    {
        wifi_e("ECSA allocate memory failed \r\n");
        return -WM_FAIL;
    }

    (void)memset(buf, 0, total_len);
    tlv       = (tlvbuf_custom_ie *)buf;
    tlv->type = MRVL_MGMT_IE_LIST_TLV_ID;

    ret = get_free_mgmt_ie_index(&mgmt_ie_index);
    if (WM_SUCCESS != ret)
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_1024_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }

    pcust_chansw_ie                    = (custom_ie *)(tlv->ie_data);
    pcust_chansw_ie->ie_index          = mgmt_ie_index;
    pcust_chansw_ie->ie_length         = sizeof(IEEEtypes_ChanSwitchAnn_t);
    pcust_chansw_ie->mgmt_subtype_mask = MGMT_MASK_BEACON | MGMT_MASK_PROBE_RESP; /*Add IE for
                                                                 BEACON/probe resp*/
    chan_switch                    = (IEEEtypes_ChanSwitchAnn_t *)pcust_chansw_ie->ie_buffer;
    chan_switch->element_id        = CHANNEL_SWITCH_ANN;
    chan_switch->len               = 3;
    chan_switch->chan_switch_mode  = block_tx;
    chan_switch->new_channel_num   = channel;
    chan_switch->chan_switch_count = switch_count;
    DBG_HEXDUMP(MCMD_D, "CSA IE", (t_u8 *)pcust_chansw_ie->ie_buffer, pcust_chansw_ie->ie_length);

#if CONFIG_5GHz_SUPPORT
    if (pbss_desc->bss_band & BAND_A)
        usr_dot_11n_dev_cap = mlan_adap->usr_dot_11n_dev_cap_a;
    else
#endif
        usr_dot_11n_dev_cap = mlan_adap->usr_dot_11n_dev_cap_bg;

    if (!ISSUPP_CHANWIDTH40(usr_dot_11n_dev_cap))
    {
        band_width = 0;
    }

    switch (band_width)
    {
        case CHANNEL_BW_40MHZ_ABOVE:
        case CHANNEL_BW_40MHZ_BELOW:
            bw = 40;
            break;
#if (CONFIG_11AC)
        case CHANNEL_BW_80MHZ:
            bw = 80;
            break;
        case CHANNEL_BW_160MHZ:
            bw = 160;
            break;
#endif
        default:
            bw = 20;
            break;
    }

    if (!new_oper_class && ecsa)
        wlan_get_nonglobal_operclass_by_bw_channel(bw, channel, &new_oper_class);

    if (new_oper_class)
    {
        pcust_chansw_ie->ie_length += sizeof(IEEEtypes_ExtChanSwitchAnn_t);
        ext_chan_switch =
            (IEEEtypes_ExtChanSwitchAnn_t *)(pcust_chansw_ie->ie_buffer + sizeof(IEEEtypes_ChanSwitchAnn_t));
        ext_chan_switch->element_id        = EXTEND_CHANNEL_SWITCH_ANN;
        ext_chan_switch->len               = 4;
        ext_chan_switch->chan_switch_mode  = block_tx;
        ext_chan_switch->new_oper_class    = new_oper_class;
        ext_chan_switch->new_channel_num   = channel;
        ext_chan_switch->chan_switch_count = switch_count;
        DBG_HEXDUMP(MCMD_D, "ECSA IE", (t_u8 *)(pcust_chansw_ie->ie_buffer + sizeof(IEEEtypes_ChanSwitchAnn_t)),
                    pcust_chansw_ie->ie_length - sizeof(IEEEtypes_ChanSwitchAnn_t));
    }

#if (CONFIG_11AC)
    /* bandwidth 40/80/160 should set channel switch wrapper ie for 11ac 5G
     * channel*/
    if (band_width && channel > 14)
    {
        pChanSwWrap_ie             = (IEEEtypes_Header_t *)(pcust_chansw_ie->ie_buffer + pcust_chansw_ie->ie_length);
        pChanSwWrap_ie->element_id = EXT_POWER_CONSTR;
        pChanSwWrap_ie->len        = sizeof(IEEEtypes_WideBWChanSwitch_t);

        pbwchansw_ie = (IEEEtypes_WideBWChanSwitch_t *)((t_u8 *)pChanSwWrap_ie + sizeof(IEEEtypes_Header_t));
        pbwchansw_ie->ieee_hdr.element_id = BW_CHANNEL_SWITCH;
        pbwchansw_ie->ieee_hdr.len        = sizeof(IEEEtypes_WideBWChanSwitch_t) - sizeof(IEEEtypes_Header_t);

        center_freq_idx = wlan_get_center_freq_idx((mlan_private *)mlan_adap->priv[1], BAND_AAC, channel, band_width);
        if (band_width == CHANNEL_BW_40MHZ_ABOVE || band_width == CHANNEL_BW_40MHZ_BELOW)
        {
            pbwchansw_ie->new_channel_width        = 0;
            pbwchansw_ie->new_channel_center_freq0 = center_freq_idx;
        }
        else if (band_width == CHANNEL_BW_80MHZ)
        {
            pbwchansw_ie->new_channel_width        = 1;
            pbwchansw_ie->new_channel_center_freq0 = center_freq_idx;
        }
        else if (band_width == CHANNEL_BW_160MHZ)
        {
            pbwchansw_ie->new_channel_width        = 2;
            pbwchansw_ie->new_channel_center_freq0 = center_freq_idx - 8;
            pbwchansw_ie->new_channel_center_freq1 = center_freq_idx + 8;
        }
        else
            wifi_e("Invalid bandwidth.Support value 1/3/4/5 for 40+/40-/80/160MHZ\n");

        /*prepare the VHT Transmit Power Envelope IE*/
        pvhttpcEnv_ie = (IEEEtypes_VhtTpcEnvelope_t *)((t_u8 *)pChanSwWrap_ie + sizeof(IEEEtypes_Header_t) +
                                                       sizeof(IEEEtypes_WideBWChanSwitch_t));
        pvhttpcEnv_ie->ieee_hdr.element_id = VHT_TX_POWER_ENV;
        pvhttpcEnv_ie->ieee_hdr.len        = sizeof(IEEEtypes_VhtTpcEnvelope_t) - sizeof(IEEEtypes_Header_t);
        /* Local Max TX Power Count= 3,
         * Local TX Power Unit Inter=EIP(0) */
        pvhttpcEnv_ie->tpc_info                     = 3;
        pvhttpcEnv_ie->local_max_tp_20mhz           = 0xff;
        pvhttpcEnv_ie->local_max_tp_40mhz           = 0xff;
        pvhttpcEnv_ie->local_max_tp_80mhz           = 0xff;
        pvhttpcEnv_ie->local_max_tp_160mhz_80_80mhz = 0xff;
        pChanSwWrap_ie->len += sizeof(IEEEtypes_VhtTpcEnvelope_t);
        pcust_chansw_ie->ie_length += pChanSwWrap_ie->len + sizeof(IEEEtypes_Header_t);
        DBG_HEXDUMP(MCMD_D, "Channel switch wrapper IE", (t_u8 *)pChanSwWrap_ie,
                    pChanSwWrap_ie->len + sizeof(IEEEtypes_Header_t));
    }
#endif
    tlv->length = sizeof(custom_ie) + pcust_chansw_ie->ie_length - MAX_IE_SIZE;

    buf_len = pcust_chansw_ie->ie_length + sizeof(tlvbuf_custom_ie) + sizeof(custom_ie) - MAX_IE_SIZE;

    ret = wrapper_wlan_cmd_mgmt_ie(MLAN_BSS_TYPE_UAP, buf, buf_len, HostCmd_ACT_GEN_SET);
    if (ret != MLAN_STATUS_SUCCESS && ret != MLAN_STATUS_PENDING)
    {
        wifi_e("Failed to set ECSA IE");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_1024_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }
    set_ie_index(mgmt_ie_index);

    OSA_SemaphoreWait((osa_semaphore_handle_t)ecsa_status_control.ecsa_sem, (switch_count + 2) * wm_wifi.beacon_period);
    set_ecsa_block_tx_flag(false);

    if (!ie_index_is_set(mgmt_ie_index))
    {
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_1024_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }

    /*Clear ECSA ie*/
    (void)memset(buf, 0, total_len);
    tlv         = (tlvbuf_custom_ie *)buf;
    tlv->type   = MRVL_MGMT_IE_LIST_TLV_ID;
    tlv->length = sizeof(custom_ie) - MAX_IE_SIZE;

    pcust_chansw_ie->mgmt_subtype_mask = MGMT_MASK_CLEAR;
    pcust_chansw_ie->ie_length         = 0;
    pcust_chansw_ie->ie_index          = mgmt_ie_index;
    buf_len                            = sizeof(tlvbuf_custom_ie) + tlv->length;

    ret = wrapper_wlan_cmd_mgmt_ie(MLAN_BSS_TYPE_UAP, buf, buf_len, HostCmd_ACT_GEN_SET);
    if (ret != MLAN_STATUS_SUCCESS && ret != MLAN_STATUS_PENDING)
    {
        wifi_e("Failed to clear ECSA IE");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(buf);
#else
        OSA_MemoryPoolFree(buf_1024_MemoryPool, buf);
#endif
        return -WM_FAIL;
    }
    clear_ie_index(mgmt_ie_index);

#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(buf);
#else
    OSA_MemoryPoolFree(buf_1024_MemoryPool, buf);
#endif

    return WM_SUCCESS;
}

int wifi_set_action_ecsa_cfg(t_u8 block_tx, t_u8 oper_class, t_u8 channel, t_u8 switch_count)
{
    mlan_status ret  = MLAN_STATUS_SUCCESS;
    mlan_ds_bss *bss = NULL;
    mlan_ioctl_req req;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));

#if !CONFIG_MEM_POOLS
    bss = OSA_MemoryAllocate(sizeof(mlan_ds_bss));
#else
    bss = OSA_MemoryPoolAllocate(buf_1280_MemoryPool);
#endif

    if (bss == NULL)
    {
        return -WM_FAIL;
    }

    req.bss_index                           = MLAN_BSS_ROLE_UAP;
    req.pbuf                                = (t_u8 *)bss;
    bss->sub_command                        = MLAN_OID_ACTION_CHAN_SWITCH;
    req.req_id                              = MLAN_IOCTL_BSS;
    req.action                              = MLAN_ACT_SET;
    bss->param.chanswitch.chan_switch_mode  = block_tx;
    bss->param.chanswitch.new_channel_num   = channel;
    bss->param.chanswitch.chan_switch_count = switch_count;
    bss->param.chanswitch.new_oper_class    = oper_class;

    ret = wlan_ops_uap_ioctl(mlan_adap, &req);

    if (ret != MLAN_STATUS_SUCCESS && ret != MLAN_STATUS_PENDING)
    {
        wifi_e("Failed to set ECSA IE");
#if !CONFIG_MEM_POOLS
        OSA_MemoryFree(bss);
#else
        OSA_MemoryPoolFree(buf_1280_MemoryPool, bss);
#endif
        return -WM_FAIL;
    }
#if !CONFIG_MEM_POOLS
    OSA_MemoryFree(bss);
#else
    OSA_MemoryPoolFree(buf_1280_MemoryPool, bss);
#endif

    return WM_SUCCESS;
}

#endif

#define SUBTYPE_AUTH          11
#define AUTH_REQUEST_BUF_SIZE 512

#if CONFIG_WPA_SUPP

#if CONFIG_11R
bool wifi_same_ess_ft()
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    return pmpriv->auth_alg == MLAN_AUTH_MODE_FT ? true : false;
}
#endif

int wifi_nxp_set_default_scan_ies(const u8 *ies, size_t ies_len)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];

    if (ies && ies_len < sizeof(pmpriv->default_scan_ies))
    {
        pmpriv->default_scan_ies_len = ies_len;
        memcpy(pmpriv->default_scan_ies, ies, ies_len);
    }

    return WM_SUCCESS;
}

#define HEADER_SIZE 8
// frmctl + durationid + addr1 + addr2 + addr3 + seqctl + addr4
#define MGMT_HEADER_LEN (2 + 2 + 6 + 6 + 6 + 2 + 6)
// 6   = auth_alg + auth_transaction +auth_status
#define AUTH_BODY_LEN 6

static int wlan_send_mgmt_auth_request(mlan_private *pmpriv,
                                       const t_u8 channel,
                                       const t_u8 auth_alg,
                                       const t_u8 *auth_seq_num,
                                       const t_u8 *status_code,
                                       const t_u8 *dest,
                                       const t_u8 *sae_data,
                                       const t_u16 sae_data_len)
{
    mlan_adapter *pmadapter      = pmpriv->adapter;
    t_u16 pkt_len                = 0;
    mlan_802_11_mac_addr *da     = MNULL;
    mlan_802_11_mac_addr *sa     = MNULL;
    wlan_mgmt_pkt *pmgmt_pkt_hdr = MNULL;
    t_u8 *pos                    = MNULL;
    int meas_pkt_len             = 0;
    t_s32 i                      = -1;
    BSSDescriptor_t *pbss_desc   = MNULL;
    WLAN_802_11_RATES rates = {0x00};
    t_u32 rates_size;
    t_u8 addr[]                  = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    t_u8 baserates[] = {0x82, 0x84, 0x8b, 0x96, 0x8c, 0x98, 0xb0};

    if (pmpriv->bss_index != (t_u8)MLAN_BSS_ROLE_STA)
    {
        wifi_d("invalid interface %d for sending auth request", pmpriv->bss_index);
        return (int)MLAN_STATUS_FAILURE;
    }

    da = (mlan_802_11_mac_addr *)(void *)dest;
    sa = (mlan_802_11_mac_addr *)(void *)(&pmpriv->curr_addr[0]);

    i = wlan_find_bssid_in_list(pmpriv, dest, MLAN_BSS_MODE_AUTO);
    if (i >= 0)
    {
        pbss_desc = &pmadapter->pscan_table[i];
        if (wlan_setup_rates_from_bssdesc(pmpriv, pbss_desc, rates, &rates_size) != MLAN_STATUS_SUCCESS)
        {
            wifi_d("Not support the rates");
            return (int)MLAN_STATUS_FAILURE;
        }
        t_u8 *prate = (t_u8 *)&(pbss_desc->supported_rates);
        t_u8 rateIndex = 0xff;

        for (int j = 0; j < rates_size; j++)
        {
            if (prate[j] >= 0x82)
            {
                for (int k = 0; k < sizeof(baserates); k++)
                {
                    if (prate[j] == baserates[k] && k < rateIndex)
                    rateIndex = k;
                }
            }
        }
        pmpriv->pkt_tx_ctrl |= 1 << 15;
        switch (baserates[rateIndex])
        {
        case 0x82:
            pmpriv->pkt_tx_ctrl |= 0 << 16;
            break;
        case 0x84:
            pmpriv->pkt_tx_ctrl |= 1 << 16;
            break;
        case 0x8b:
            pmpriv->pkt_tx_ctrl |= 2 << 16;
            break;
        case 0x96:
            pmpriv->pkt_tx_ctrl |= 3 << 16;
            break;
        case 0x8c:
            pmpriv->pkt_tx_ctrl |= 5 << 16;
            break;
        case 0x98:
            pmpriv->pkt_tx_ctrl |= 7 << 16;
            break;
        case 0xb0:
            pmpriv->pkt_tx_ctrl |= 9 << 16;
            break;
        default:
            pmpriv->pkt_tx_ctrl = 0;
            wifi_d("Not support the base rates");
            break;
        }
    }
    if (pmadapter->cmd_tx_data == 1U)
    {
        (void)wifi_get_command_lock();
        HostCmd_DS_COMMAND *cmd           = wifi_get_command_buffer();
        mlan_ds_misc_tx_frame tx_frame    = {0};
        wlan_802_11_header *pwlan_pkt_hdr = MNULL;
        IEEEtypes_FrameCtl_t *mgmt_fc_p   = MNULL;
        t_u8 *pBuf                        = &tx_frame.tx_buf[0];
        t_u32 pkt_type;

        pkt_len = MGMT_HEADER_LEN + AUTH_BODY_LEN;

        memset(cmd, 0x00, pkt_len);

        pkt_type   = MRVL_PKT_TYPE_MGMT_FRAME;

        /* Add pkt_type and tx_control */
        memcpy(pBuf, &pkt_type, sizeof(pkt_type));
        memcpy(pBuf + sizeof(pkt_type), &(pmpriv->pkt_tx_ctrl), sizeof(pmpriv->pkt_tx_ctrl));

        pwlan_pkt_hdr = (wlan_802_11_header *)(void *)(pBuf + HEADER_SIZE + sizeof(pkt_len));
        /* 802.11 header */
        mgmt_fc_p           = (IEEEtypes_FrameCtl_t *)(void *)&pwlan_pkt_hdr->frm_ctl;
        mgmt_fc_p->sub_type = SUBTYPE_AUTH;
        mgmt_fc_p->type     = (t_u8)IEEE_TYPE_MANAGEMENT;
        (void)memcpy(pwlan_pkt_hdr->addr1, da, MLAN_MAC_ADDR_LENGTH);
        (void)memcpy(pwlan_pkt_hdr->addr2, sa, MLAN_MAC_ADDR_LENGTH);
        (void)memcpy(pwlan_pkt_hdr->addr3, da, MLAN_MAC_ADDR_LENGTH);

        (void)memcpy(pwlan_pkt_hdr->addr4, addr, MLAN_MAC_ADDR_LENGTH);

        /* 802.11 management body */
        pos    = (t_u8 *)pwlan_pkt_hdr + sizeof(wlan_802_11_header);
        pos[0] = auth_alg;
        pos[1] = 0;
        pos[2] = auth_seq_num[0];
        pos[3] = auth_seq_num[1];
        pos[4] = status_code[0];
        pos[5] = status_code[1];

        pos += 6;

        if ((sae_data != NULL) && (sae_data_len > 0))
        {
            memcpy(pos, sae_data, sae_data_len);
            pos += sae_data_len;
        }

        meas_pkt_len = pos - (t_u8 *)pwlan_pkt_hdr;
        pkt_len      = (t_u16)meas_pkt_len;

        /*Add packet len*/
        pkt_len = wlan_cpu_to_le16(pkt_len);
        memcpy(pBuf + HEADER_SIZE, &pkt_len, sizeof(pkt_len));

        tx_frame.bandcfg.chanBand = channel > 14 ? BAND_5GHZ : BAND_2GHZ;
        tx_frame.channel          = channel;
        tx_frame.data_len         = HEADER_SIZE + pkt_len + 2 * sizeof(pkt_len);
        tx_frame.buf_type         = MLAN_BUF_TYPE_RAW_DATA;
        tx_frame.priority         = 7;

        cmd->seq_num = 0x0;
        cmd->result  = 0x0;

        mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_TX_FRAME,
                                                  HostCmd_ACT_GEN_SET, 0, NULL, &tx_frame, cmd);
        if (rv != MLAN_STATUS_SUCCESS)
        {
            return -WM_FAIL;
        }

        (void)wifi_wait_for_cmdresp(NULL);
        return wm_wifi.cmd_resp_status;
    }
    else
    {
        pmgmt_pkt_hdr = wifi_PrepDefaultMgtMsg(SUBTYPE_AUTH, da, sa, da, sizeof(wlan_mgmt_pkt) + AUTH_REQUEST_BUF_SIZE);
        if (pmgmt_pkt_hdr == MNULL)
        {
            wifi_e("No memory for auth request");
            return (int)MLAN_STATUS_FAILURE;
        }

        (void)memcpy(pmgmt_pkt_hdr->wlan_header.addr4, addr, MLAN_MAC_ADDR_LENGTH);

        /* 802.11 management body */
        pos    = (t_u8 *)pmgmt_pkt_hdr + sizeof(wlan_mgmt_pkt);
        pos[0] = auth_alg;
        pos[1] = 0;
        pos[2] = auth_seq_num[0];
        pos[3] = auth_seq_num[1];
        pos[4] = status_code[0];
        pos[5] = status_code[1];

        pos += 6;

        if ((sae_data != NULL) && (sae_data_len > 0))
        {
            memcpy(pos, sae_data, sae_data_len);
            pos += sae_data_len;
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
    }

    pmpriv->pkt_tx_ctrl = 0;
    return (int)MLAN_STATUS_SUCCESS;
}

int wifi_send_mgmt_auth_request(const t_u8 channel,
                                const t_u8 auth_alg,
                                const t_u8 *auth_seq_num,
                                const t_u8 *status_code,
                                const t_u8 *dest,
                                const t_u8 *sae_data,
                                const t_u16 sae_data_len)
{
    mlan_private *pmpriv = (mlan_private *)mlan_adap->priv[0];
    int ret;

    if ((pmpriv->auth_alg != WLAN_AUTH_SAE) && (pmpriv->auth_flag & HOST_MLME_AUTH_PENDING))
    {
        wifi_d("pending auth on going");
        return -WM_FAIL;
    }

#if CONFIG_11R
    if (auth_alg == MLAN_AUTH_MODE_FT)
    {
        pmpriv->ft_roam = MTRUE;
    }
#endif

    if (pmpriv->auth_flag == 0)
    {
        wifi_set_rx_mgmt_indication(MLAN_BSS_TYPE_STA, WIFI_MGMT_AUTH | WIFI_MGMT_DEAUTH | WIFI_MGMT_DIASSOC);

        wifi_remain_on_channel(true, channel, 6000);
    }

    pmpriv->curr_bss_params.host_mlme = 1;
    pmpriv->auth_flag                 = HOST_MLME_AUTH_PENDING;
    pmpriv->auth_alg                  = wlan_cpu_to_le16(auth_alg);

    ret =
        wlan_send_mgmt_auth_request(pmpriv, channel, auth_alg, auth_seq_num, status_code, dest, sae_data, sae_data_len);

    if (ret != WM_SUCCESS)
    {
        wifi_set_rx_mgmt_indication(MLAN_BSS_TYPE_STA, 0);
        wifi_remain_on_channel(false, 0, 0);

        pmpriv->curr_bss_params.host_mlme = 0;
        pmpriv->auth_flag                 = 0;
        pmpriv->auth_alg                  = 0xFFFF;
#if CONFIG_11R
        pmpriv->ft_roam = MFALSE;
#endif
    }
    return ret;
}
#endif

#if CONFIG_WMM_UAPSD
int wifi_set_wmm_qos_cfg(t_u8 qos_cfg)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    mlan_ioctl_req req;
    mlan_ds_wmm_cfg cfg;

    (void)memset(&req, 0x00, sizeof(mlan_ioctl_req));
    (void)memset(&cfg, 0x00, sizeof(mlan_ds_wmm_cfg));
    cfg.sub_command   = MLAN_OID_WMM_CFG_QOS;
    cfg.param.qos_cfg = qos_cfg;
    req.pbuf          = (t_u8 *)&cfg;
    req.buf_len       = sizeof(mlan_ds_wmm_cfg);
    req.req_id        = MLAN_IOCTL_WMM_CFG;
    req.action        = MLAN_ACT_SET;

    ret = wlan_ops_sta_ioctl(mlan_adap, &req);
    return ret;
}

void wifi_set_sleep_period(uint16_t sleep_period)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, MLAN_BSS_TYPE_STA);
    cmd->result  = 0x0;
    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_SLEEP_PERIOD, HostCmd_ACT_GEN_SET,
                             0, NULL, &sleep_period, cmd);
    wifi_wait_for_cmdresp(NULL);
}
#endif

#if CONFIG_11AX
#if CONFIG_MMSF
int wifi_mmsf_cfg(const t_u16 action, t_u8 *enable, t_u8 *Density, t_u8 *MMSF)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_DBGS_CFG);
    cmd->size    = S_DS_GEN;

    HostCmd_DS_MMSF_CFG *MMSF_CFG = (HostCmd_DS_MMSF_CFG *)&cmd->params.mmsf_cfg;
    MMSF_CFG->action              = wlan_cpu_to_le16(action);
    MMSF_CFG->sub_id              = wlan_cpu_to_le16(MLAN_11AX_DEBUG_MMSF_SUBID);

    (void)memcpy(&MMSF_CFG->enableMMSF, enable, sizeof(MMSF_CFG->enableMMSF));
    (void)memcpy(&MMSF_CFG->ampduDensity, Density, sizeof(MMSF_CFG->ampduDensity));
    (void)memcpy(&MMSF_CFG->ampduMMSF, MMSF, sizeof(MMSF_CFG->ampduMMSF));

    cmd->size += sizeof(HostCmd_DS_MMSF_CFG);
    cmd->size = wlan_cpu_to_le16(cmd->size);

    if (action == ACTION_SET)
    {
        return wifi_wait_for_cmdresp(NULL);
    }
    else
    {
        wifi_mmsf_cfg_t mmsf_cfg_resp;
        mmsf_cfg_resp.enable  = enable;
        mmsf_cfg_resp.Density = Density;
        mmsf_cfg_resp.MMSF    = MMSF;
        return wifi_wait_for_cmdresp(&mmsf_cfg_resp);
    }
}
#endif
#endif

#if CONFIG_WIFI_RECOVERY
int wifi_recovery_test(void)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_DBGS_CFG);
    cmd->size    = S_DS_GEN;
    // HostCmd_DS_TMRC_CFG tmrc_cfg;

    HostCmd_DS_TMRC_CFG *tmrc_cfg = (HostCmd_DS_TMRC_CFG *)&cmd->params.tmrc_cfg;
    tmrc_cfg->action              = wlan_cpu_to_le16(HostCmd_ACT_GEN_GET);
    tmrc_cfg->sub_id              = wlan_cpu_to_le16(MLAN_RECOVERY_TEST_SUBID);

    cmd->size += sizeof(HostCmd_DS_TMRC_CFG);
    cmd->size = wlan_cpu_to_le16(cmd->size);

    return wifi_wait_for_cmdresp(NULL);
}
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
int wifi_tx_ampdu_prot_mode(tx_ampdu_prot_mode_para *prot_mode, t_u16 action)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = 0x00;
    cmd->result  = 0x0;

    wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_TX_AMPDU_PROT_MODE, action, 0, NULL,
                             prot_mode, cmd);

    return wifi_wait_for_cmdresp(action == HostCmd_ACT_GEN_GET ? prot_mode : NULL);
}
#endif

#if CONFIG_CSI
int wifi_csi_cfg(wifi_csi_config_params_t *csi_params)
{
    t_u16 action = CSI_CMD_DISABLE;

    action = csi_params->csi_enable;
    if (action != CSI_CMD_ENABLE && action != CSI_CMD_DISABLE)
        return -WM_FAIL;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    if (csi_params->bss_type == BSS_TYPE_UAP)
        cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, BSS_TYPE_UAP);
    else
        cmd->seq_num = 0x0;
    cmd->result = 0x0;

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_CSI,
                                              csi_params->csi_enable, 0, NULL, csi_params, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
    {
        wifi_put_command_lock();
        return -WM_FAIL;
    }

    return wifi_wait_for_cmdresp(NULL);
}
#endif

#if (CONFIG_IPS)
/* enable/disable config for IPS */
int wifi_set_ips_config(mlan_bss_type interface, int option)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    t_u16 cmd_size;

    if ((option != 0) && (option != 1))
        return -WM_FAIL;

    cmd_size = sizeof(HostCmd_DS_IPS_CONFIG) + S_DS_GEN /* cmd header */;
    (void)memset(cmd, 0x00, cmd_size);
    cmd->command = HostCmd_CMD_IPS_CONFIG;
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, interface);
    cmd->size    = cmd_size;

    cmd->params.ips_config.enable = option;

    return wifi_wait_for_cmdresp(NULL);
}
#endif

#if CONFIG_NET_MONITOR
int wifi_net_monitor_cfg(wifi_net_monitor_t *monitor)
{
    t_u16 action = HostCmd_ACT_GEN_SET;

    action = monitor->action;

    if (action != HostCmd_ACT_GEN_GET && action != HostCmd_ACT_GEN_SET)
        return -WM_FAIL;

    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->seq_num = 0x0;
    cmd->result  = 0x0;

    mlan_status rv = wlan_ops_sta_prepare_cmd((mlan_private *)mlan_adap->priv[0], HostCmd_CMD_802_11_NET_MONITOR,
                                              monitor->action, 0, NULL, monitor, cmd);
    if (rv != MLAN_STATUS_SUCCESS)
        return -WM_FAIL;

    return wifi_wait_for_cmdresp(NULL);
}
#endif

#if CONFIG_TSP
int wifi_tsp_cfg(const t_u16 action,
                 t_u16 *enable,
                 t_u32 *back_off,
                 t_u32 *highThreshold,
                 t_u32 *lowThreshold,
                 t_u32 *dutycycstep,
                 t_u32 *dutycycmin,
                 int *highthrtemp,
                 int *lowthrtemp,
                 int *currCAUTemp,
                 int *currRFUTemp)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    HostCmd_DS_TSP_CFG *tsp_cfg = &cmd->params.tsp_cfg;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_TSP_CFG);
    cmd->size    = sizeof(HostCmd_DS_TSP_CFG) + S_DS_GEN;

    (void)memcpy(&tsp_cfg->thermalPowerMgmtenable, enable, sizeof(t_u16));
    (void)memcpy(&tsp_cfg->powerMgmtBackoff, back_off, sizeof(t_u32));
    (void)memcpy(&tsp_cfg->highPwrBOThrshld, highThreshold, sizeof(t_u32));
    (void)memcpy(&tsp_cfg->lowPwrBOThrshld, lowThreshold, sizeof(t_u32));
    (void)memcpy(&tsp_cfg->dutycycstep, dutycycstep, sizeof(t_u32));
    (void)memcpy(&tsp_cfg->dutycycmin, dutycycmin, sizeof(t_u32));
    (void)memcpy(&tsp_cfg->highthrtemp, highthrtemp, sizeof(t_s32));
    (void)memcpy(&tsp_cfg->lowthrtemp, lowthrtemp, sizeof(t_s32));

    tsp_cfg->action                 = wlan_cpu_to_le16(action);
    tsp_cfg->thermalPowerMgmtenable = wlan_cpu_to_le16(tsp_cfg->thermalPowerMgmtenable);
    tsp_cfg->powerMgmtBackoff       = wlan_cpu_to_le16(tsp_cfg->powerMgmtBackoff);
    tsp_cfg->highPwrBOThrshld       = wlan_cpu_to_le16(tsp_cfg->highPwrBOThrshld);
    tsp_cfg->lowPwrBOThrshld        = wlan_cpu_to_le16(tsp_cfg->lowPwrBOThrshld);
    tsp_cfg->dutycycstep            = wlan_cpu_to_le16(tsp_cfg->dutycycstep);
    tsp_cfg->dutycycmin             = wlan_cpu_to_le16(tsp_cfg->dutycycmin);
    tsp_cfg->highthrtemp            = wlan_cpu_to_le16(tsp_cfg->highthrtemp);
    tsp_cfg->lowthrtemp             = wlan_cpu_to_le16(tsp_cfg->lowthrtemp);

    cmd->size = wlan_cpu_to_le16(cmd->size);

    if (action == MLAN_ACT_SET)
        return wifi_wait_for_cmdresp(NULL);
    else
    {
        TSP_CFG tsp_get_cfg;
        tsp_get_cfg.thermalPowerMgmtenable = enable;
        tsp_get_cfg.powerMgmtBackoff       = back_off;
        tsp_get_cfg.highPwrBOThrshld       = highThreshold;
        tsp_get_cfg.lowPwrBOThrshld        = lowThreshold;
        tsp_get_cfg.dutycycstep            = dutycycstep;
        tsp_get_cfg.dutycycmin             = dutycycmin;
        tsp_get_cfg.highthrtemp            = highthrtemp;
        tsp_get_cfg.lowthrtemp             = lowthrtemp;
        tsp_get_cfg.currCAUTemp            = currCAUTemp;
        tsp_get_cfg.currRFUTemp            = currRFUTemp;

        return wifi_wait_for_cmdresp(&tsp_get_cfg);
    }
}
#endif

#if CONFIG_TURBO_MODE
int wifi_get_turbo_mode(t_u8 *mode)
{
    return wlan_get_set_turbo_mode(ACTION_GET, mode, MLAN_BSS_TYPE_STA);
}

int wifi_get_uap_turbo_mode(t_u8 *mode)
{
    return wlan_get_set_turbo_mode(ACTION_GET, mode, MLAN_BSS_TYPE_UAP);
}

int wifi_set_turbo_mode(t_u8 mode)
{
    return wlan_get_set_turbo_mode(ACTION_SET, &mode, MLAN_BSS_TYPE_STA);
}

int wifi_set_uap_turbo_mode(t_u8 mode)
{
    return wlan_get_set_turbo_mode(ACTION_SET, &mode, MLAN_BSS_TYPE_UAP);
}

int wlan_get_set_turbo_mode(t_u16 action, t_u8 *mode, mlan_bss_type bss_type)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_802_11_SNMP_MIB);
    cmd->size    = S_DS_GEN;
    cmd->seq_num = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);

    uint8_t *tlv = (uint8_t *)((uint8_t *)cmd + S_DS_GEN);

    turbo_mode_para *turbo_ptr = (turbo_mode_para *)tlv;
    turbo_ptr->action          = action;
    turbo_ptr->oid             = OID_WMM_TURBO_MODE;
    turbo_ptr->size            = 0x1;
    if (action == ACTION_SET)
        (void)memcpy(&turbo_ptr->mode, mode, sizeof(t_u8));

    cmd->size += sizeof(turbo_mode_para);
    cmd->size = wlan_cpu_to_le16(cmd->size);

    if (action == ACTION_GET)
        return wifi_wait_for_cmdresp(mode);
    else
        return wifi_wait_for_cmdresp(NULL);
}
#endif

#if (CONFIG_11MC) || (CONFIG_11AZ)
int wifi_unassoc_ftm_cfg(const t_u16 action, const t_u16 config)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                       = wlan_cpu_to_le16(HostCmd_CMD_DOT11MC_UNASSOC_FTM_CFG);
    cmd->size                          = S_DS_GEN + sizeof(HostCmd_DOT11MC_UNASSOC_FTM_CFG);
    cmd->size                          = wlan_cpu_to_le16(cmd->size);
    cmd->params.unassoc_ftm_cfg.action = wlan_cpu_to_le16(action);
    cmd->params.unassoc_ftm_cfg.config = wlan_cpu_to_le16(config);

    return wifi_wait_for_cmdresp(NULL);
}

int wifi_ftm_start_stop(const t_u16 action, const t_u8 loop_cnt, const t_u8 *mac, const t_u8 channel)
{
    if (action != FTM_ACTION_STOP)
    {
        ftm_param.channel = channel;
        (void)memcpy(ftm_param.peer_mac, mac, MLAN_MAC_ADDR_LENGTH);
        ftm_param.loop_cnt = loop_cnt;
        ftm_param.status   = (ftm_param.loop_cnt == 0) ? 1 : 0;
#if CONFIG_WLS_CSI_PROC
        g_csi_event_for_wls = 1;
#endif
        return wifi_ftm_start(action, mac, channel);
    }
    else
    {
        ftm_param.loop_cnt = 0;
        ftm_param.status   = 0;
#if CONFIG_WLS_CSI_PROC
        g_csi_event_for_wls = 0;
#endif
        return wifi_ftm_stop(FTM_ACTION_STOP, ftm_param.peer_mac, ftm_param.channel);
    }
}

int wifi_ftm_start(const t_u16 action, const t_u8 *mac, const t_u8 channel)
{
    /*if (is_sta_connected() || is_sta_ipv4_connected()
#if CONFIG_IPV6
        || is_sta_ipv6_connected()
#endif
    )*/
    {
        wifi_get_command_lock();
        HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

        cmd->command                             = wlan_cpu_to_le16(HostCmd_CMD_FTM_SESSION_CTRL);
        cmd->size                                = S_DS_GEN + sizeof(HostCmd_FTM_SESSION_CTRL);
        cmd->size                                = wlan_cpu_to_le16(cmd->size);
        cmd->params.ftm_session_ctrl.action      = wlan_cpu_to_le16(action);
        cmd->params.ftm_session_ctrl.for_ranging = wlan_cpu_to_le16(FOR_RANGING);
        (void)memcpy(cmd->params.ftm_session_ctrl.peer_mac, mac, MLAN_MAC_ADDR_LENGTH);
        cmd->params.ftm_session_ctrl.chan     = wlan_cpu_to_le16(channel);
        cmd->params.ftm_session_ctrl.chanBand = (cmd->params.ftm_session_ctrl.chan < 32) ? 0 : 1;

        return wifi_wait_for_cmdresp(NULL);
    }
    /*    else
        {
            PRINTF("Cannot Start FTM, STA not associated !\r\n");
            return -WM_FAIL;
        }*/
}

int wifi_ftm_stop(const t_u16 action, const t_u8 *mac, const t_u8 channel)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                             = wlan_cpu_to_le16(HostCmd_CMD_FTM_SESSION_CTRL);
    cmd->size                                = S_DS_GEN + sizeof(HostCmd_FTM_SESSION_CTRL);
    cmd->size                                = wlan_cpu_to_le16(cmd->size);
    cmd->params.ftm_session_ctrl.action      = wlan_cpu_to_le16(action);
    cmd->params.ftm_session_ctrl.for_ranging = wlan_cpu_to_le16(FOR_RANGING);
    (void)memcpy(cmd->params.ftm_session_ctrl.peer_mac, mac, MLAN_MAC_ADDR_LENGTH);
    cmd->params.ftm_session_ctrl.chan = wlan_cpu_to_le16(channel);

    return wifi_wait_for_cmdresp(NULL);
}

int wifi_ftm_11mc_cfg(ftm_11mc_nego_cfg_t *ftm_11mc_nego_cfg)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                       = wlan_cpu_to_le16(HostCmd_CMD_FTM_SESSION_CFG);
    cmd->size                          = wlan_cpu_to_le16(S_DS_GEN);
    cmd->params.ftm_session_cfg.action = MLAN_ACT_SET;

    wlan_dot11mc_ftm_cfg(cmd, ftm_11mc_nego_cfg);

    return wifi_wait_for_cmdresp(NULL);
}

int wifi_ftm_location_cfg(location_cfg_info_t *ftm_location_cfg)
{
    wlan_location_ftm_cfg(ftm_location_cfg);
    return 0;
}

int wifi_ftm_civic_cfg(location_civic_rep_t *ftm_civic_cfg)
{
    wlan_civic_ftm_cfg(ftm_civic_cfg);
    return 0;
}

int wifi_ftm_cfg(const t_u8 protocol, ranging_11az_cfg_t *ftm_ranging_cfg)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();

    cmd->command                       = wlan_cpu_to_le16(HostCmd_CMD_FTM_SESSION_CFG);
    cmd->size                          = wlan_cpu_to_le16(S_DS_GEN);
    cmd->params.ftm_session_cfg.action = MLAN_ACT_SET;

    HostCmd_FTM_SESSION_CFG ftm_session_cfg;
    dot11az_ftm_cfg_t *cfg_11az                       = (dot11az_ftm_cfg_t *)&ftm_session_cfg.tlv.cfg_11az;
    cfg_11az->range_tlv.val.format_bw                 = ftm_ranging_cfg->format_bw;
    cfg_11az->range_tlv.val.max_i2r_sts_upto80        = ftm_ranging_cfg->max_i2r_sts_upto80;
    cfg_11az->range_tlv.val.max_r2i_sts_upto80        = ftm_ranging_cfg->max_r2i_sts_upto80;
    cfg_11az->range_tlv.val.az_measurement_freq       = ftm_ranging_cfg->az_measurement_freq;
    cfg_11az->range_tlv.val.az_number_of_measurements = ftm_ranging_cfg->az_number_of_measurements;
    cfg_11az->range_tlv.val.i2r_lmr_feedback          = ftm_ranging_cfg->i2r_lmr_feedback;
    cfg_11az->range_tlv.val.civic_req                 = ftm_ranging_cfg->civic_req;
    cfg_11az->range_tlv.val.lci_req                   = ftm_ranging_cfg->lci_req;
    wlan_dto11az_ranging_cfg(cmd, protocol, &ftm_session_cfg);

    return wifi_wait_for_cmdresp(NULL);
}

int wifi_process_wlc_ftm_event()
{
    int ret = -WM_FAIL;
    if (ftm_param.loop_cnt > 0)
        ftm_param.loop_cnt--;
    if (ftm_param.loop_cnt > 0 || (ftm_param.status))
        ret = wifi_ftm_start(FTM_ACTION_START, ftm_param.peer_mac, ftm_param.channel);
    // else
    //    ret = wifi_ftm_stop(FTM_ACTION_STOP, ftm_param.peer_mac, ftm_param.channel);

    return ret;
}

#if CONFIG_WLS_CSI_PROC

static int mlanwls_update_distance_to_gui(int distance, unsigned int tsf)
{
    int distance_m, distance_cm;
    unsigned int time_ms = tsf / 1000;
    float distance_flt   = 1.0f * distance / (1 << 8); // in meters
    float distance_kalman;
    OSA_TimeDelay(10);

    if (range_input_str.time == 0)
    {
        range_kalman_init(&range_input_str, distance_flt, time_ms, RANGE_DRIVE_VAR, RANGE_MEASUREMENT_VAR,
                          RANGE_RATE_INIT);
        range_input_str.time = 1;
    }
    else
    {
        range_input_str.range_measurement = distance_flt;
        range_input_str.time              = time_ms;
        range_kalman(&range_input_str);
    }
    distance_kalman = range_input_str.last_range;

    distance_cm = (int)(distance_kalman * 100);
    distance_m  = distance_cm / 100;
    distance_cm -= distance_m * 100;

    wifi_d("Measured Distance: %f m; Kalman Distance: %f m [%d ms]\r\n", (double)distance_flt, (double)distance_kalman,
           time_ms);

    return 0;
}

static int send_csi_ack(unsigned int *resArray)
{
    int ret;
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    HostCmd_WLS_CSI_ACK *phostcmd = (HostCmd_WLS_CSI_ACK *)&cmd->params.wls_csi_ack;

    cmd->command     = wlan_cpu_to_le16(HostCmd_CMD_DBGS_CFG);
    cmd->size        = S_DS_GEN + sizeof(HostCmd_WLS_CSI_ACK) + 8;
    phostcmd->action = 0;
    phostcmd->sub_id = 0x333;
    phostcmd->ack    = 1;

    phostcmd->phase_roll       = resArray[0];
    phostcmd->firstpath_delay  = resArray[1];
    phostcmd->fft_size_pointer = resArray[2];
    phostcmd->csi_tsf          = resArray[3];

    cmd->size += CSI_TSF_LEN;
    cmd->size = wlan_cpu_to_le16(cmd->size);

    ret = wifi_wait_for_cmdresp(NULL);

    return ret;
}

static void proc_csi_event(void *event, unsigned int *resArray)
{
    uint8_t *csiBuffer = (uint8_t *)(event);
    hal_wls_packet_params_t packetparams;
    hal_wls_processing_input_params_t inputVals;
    unsigned int tsf = ((unsigned int *)csiBuffer)[3];
    int distance     = ((unsigned int *)csiBuffer)[19];

    if (distance >= 0)
        mlanwls_update_distance_to_gui(distance, tsf);

    inputVals.enableCsi              = 1;                  // turn on CSI processing
    inputVals.enableAoA              = AOA_DEFAULT;        // turn on AoA (req. enableCsi==1)
    inputVals.nTx                    = 3;                  // limit # tx streams to process
    inputVals.nRx                    = 3;                  // limit # rx to process
    inputVals.selCal                 = 0;                  // choose cal values
    inputVals.dumpMul                = 0;                  // dump extra peaks in AoA
    inputVals.enableAntCycling       = 0;                  // enable antenna cycling
    inputVals.dumpRawAngle           = 0;                  // Dump Raw Angle
    inputVals.useToaMin              = TOA_MIN_DEFAULT;    // 1: use min combining, 0: power combining;
    inputVals.useSubspace            = SUBSPACE_DEFAULT;   // 1: use subspace algo; 0: no;
    inputVals.useFindAngleDelayPeaks = ENABLE_DELAY_PEAKS; // use this algorithm for AoA

    resArray[0] = 0xffffffff;
    resArray[1] = 0xffffffff;
    resArray[2] = 0xffffffff;
    resArray[3] = 0xffffffff;

    wls_process_csi((unsigned int *)csiBuffer, (unsigned int *)fftInBuffer_t, &packetparams, &inputVals, resArray);
    // record TSF
    resArray[3] = tsf;

    wifi_d("EVENT: MLAN_CSI Processing results: %d | %d (%x), TSF[%x]\r\n", resArray[0], resArray[1], resArray[2], tsf);

    return;
}

int wifi_process_wls_csi_event(void *p_data)
{
    int ret;

    proc_csi_event(((t_u8 *)p_data + sizeof(csi_event_t)), csi_res_array);
    // wifi_put_wls_csi_sem(); // After processing CSI raw data, release csi sem for next CSI event.
    ret = send_csi_ack(csi_res_array);
    return ret;
}

#endif

#endif

#if CONFIG_COEX_DUTY_CYCLE
int wifi_single_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command                               = wlan_cpu_to_le16(HostCmd_CMD_ROBUST_COEX);
    cmd->size                                  = sizeof(HostCmd_SIGNLE_ANT_DUTY_CYCLE) + S_DS_GEN;
    cmd->seq_num                               = 0x0;
    cmd->result                                = 0x00;
    cmd->params.single_ant_duty_cycle.action   = HostCmd_ACT_GEN_SET;
    cmd->params.single_ant_duty_cycle.reserved = 0;
    cmd->params.single_ant_duty_cycle.single_ant_cfg_data.header.type = TLV_TYPE_COEX_DUTY_CYCLE;

    if (enable)
    {
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.header.len =
            sizeof(MrvlIETypes_SingleAntDutyCycle_Config_t) - 4;
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.enabled  = 0x0002;
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.nbTime   = nbTime;
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.wlanTime = wlanTime;
    }
    else
    {
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.header.len = sizeof(t_u16);
        cmd->params.single_ant_duty_cycle.single_ant_cfg_data.enabled    = 0x0004;
    }

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}

int wifi_dual_ant_duty_cycle(t_u16 enable, t_u16 nbTime, t_u16 wlanTime, t_u16 wlanBlockTime)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command                                                  = wlan_cpu_to_le16(HostCmd_CMD_ROBUST_COEX);
    cmd->size                                                     = sizeof(HostCmd_DUAL_ANT_DUTY_CYCLE) + S_DS_GEN;
    cmd->seq_num                                                  = 0x0;
    cmd->result                                                   = 0x00;
    cmd->params.dual_ant_duty_cycle.action                        = HostCmd_ACT_GEN_SET;
    cmd->params.dual_ant_duty_cycle.reserved                      = 0;
    cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.header.type = TLV_TYPE_COEX_DUTY_CYCLE;

    if (enable)
    {
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.header.len =
            sizeof(MrvlIETypes_DualAntDutyCycle_Config_t) - 4;
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.enabled       = 0x0002;
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.nbTime        = nbTime;
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.wlanTime      = wlanTime;
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.wlanBlockTime = wlanBlockTime;
    }
    else
    {
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.header.len = sizeof(t_u16);
        cmd->params.dual_ant_duty_cycle.dual_ant_cfg_data.enabled    = 0x0004;
    }

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_EXTERNAL_COEX_PTA

int wifi_external_coex_pta_cfg(ext_coex_pta_cfg coex_pta_config)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_ROBUST_COEX);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_EXTERNAL_COEX_PTA) + S_DS_GEN);
    cmd->seq_num = 0x0;
    cmd->result  = 0x00;

    HostCmd_EXTERNAL_COEX_PTA *external_coex_pta = (HostCmd_EXTERNAL_COEX_PTA *)&cmd->params.external_coex_pta;
    external_coex_pta->action                    = wlan_cpu_to_le16(ACTION_SET);
    external_coex_pta->reserved                  = 0x00;

    MrvlIETypes_ExternalCoexPta_Config_t *coex_pta_cfg_data =
        (MrvlIETypes_ExternalCoexPta_Config_t *)&external_coex_pta->coex_pta_cfg_data;

    coex_pta_cfg_data->param.tlv_type = wlan_cpu_to_le16(TLV_TYPE_ROBUST_COEX);
    coex_pta_cfg_data->param.tlv_length =
        wlan_cpu_to_le16(sizeof(MrvlIETypes_ExternalCoexPta_Config_t) - sizeof(MrvlIETypes_Coex_params_t));

    coex_pta_cfg_data->enabled                = wlan_cpu_to_le16(coex_pta_config.enabled);
    coex_pta_cfg_data->ext_WifiBtArb          = wlan_cpu_to_le16(coex_pta_config.ext_WifiBtArb);
    coex_pta_cfg_data->polGrantPin            = wlan_cpu_to_le16(coex_pta_config.polGrantPin);
    coex_pta_cfg_data->enable_PriPtaInt       = wlan_cpu_to_le16(coex_pta_config.enable_PriPtaInt);
    coex_pta_cfg_data->enable_StatusFromPta   = wlan_cpu_to_le16(coex_pta_config.enable_StatusFromPta);
    coex_pta_cfg_data->setPriSampTiming       = wlan_cpu_to_le16(coex_pta_config.setPriSampTiming);
    coex_pta_cfg_data->setStateInfoSampTiming = wlan_cpu_to_le16(coex_pta_config.setStateInfoSampTiming);
    coex_pta_cfg_data->extRadioTrafficPrio    = wlan_cpu_to_le16(coex_pta_config.extRadioTrafficPrio);
    coex_pta_cfg_data->extCoexHwIntWci2       = wlan_cpu_to_le16(coex_pta_config.extCoexHwIntWci2);

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_IMD3_CFG
int wifi_imd3_cfg(t_u8 imd3_value)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_ROBUST_COEX);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_IMD3_CFG) + S_DS_GEN);
    cmd->seq_num = 0x0;
    cmd->result  = 0x00;

    HostCmd_IMD3_CFG *imd3_cfg = (HostCmd_IMD3_CFG *)&cmd->params.imd3_cfg;
    imd3_cfg->action           = wlan_cpu_to_le16(ACTION_SET);
    imd3_cfg->reserved         = 0x00;

    MrvlIETypes_IMD_Config_t *imd_cfg = (MrvlIETypes_IMD_Config_t *)&imd3_cfg->imd_cfg;

    imd_cfg->param.tlv_type   = wlan_cpu_to_le16(TLV_TYPE_IMD_VALIDATION);
    imd_cfg->param.tlv_length = wlan_cpu_to_le16(sizeof(MrvlIETypes_IMD_Config_t) - sizeof(MrvlIETypes_Coex_params_t));

    imd_cfg->rbc_mode    = 0x00;
    imd_cfg->reserved    = wlan_cpu_to_le16(imd3_value);
    imd_cfg->DynamicMode = 0x0000;

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_INACTIVITY_TIMEOUT_EXT
int wifi_sta_inactivityto(wifi_inactivity_to_t *inac_to, t_u16 cmd_action)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd = wifi_get_command_buffer();
    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    HostCmd_DS_INACTIVITY_TIMEOUT_EXT *inac_to_ext = (HostCmd_DS_INACTIVITY_TIMEOUT_EXT *)&cmd->params.inactivity_to;

    cmd->command = wlan_cpu_to_le16(HostCmd_CMD_INACTIVITY_TIMEOUT_EXT);
    cmd->size    = wlan_cpu_to_le16(sizeof(HostCmd_DS_INACTIVITY_TIMEOUT_EXT) + S_DS_GEN);
    cmd->seq_num = 0x0;
    cmd->result  = 0x00;

    inac_to_ext->action = wlan_cpu_to_le16(cmd_action);
    if (cmd_action == HostCmd_ACT_GEN_SET)
    {
        inac_to_ext->timeout_unit     = wlan_cpu_to_le16((t_u16)inac_to->timeout_unit);
        inac_to_ext->unicast_timeout  = wlan_cpu_to_le16((t_u16)inac_to->unicast_timeout);
        inac_to_ext->mcast_timeout    = wlan_cpu_to_le16((t_u16)inac_to->mcast_timeout);
        inac_to_ext->ps_entry_timeout = wlan_cpu_to_le16((t_u16)inac_to->ps_entry_timeout);
        inac_to_ext->ps_cmd_timeout   = wlan_cpu_to_le16((t_u16)inac_to->ps_cmd_timeout);
        wifi_wait_for_cmdresp(NULL);
    }
    else
    {
        wifi_wait_for_cmdresp(inac_to);
    }

    return wm_wifi.cmd_resp_status;
}
#endif

#if CONFIG_AUTO_NULL_TX
int wifi_auto_null_tx(wifi_auto_null_tx_t *auto_null_tx, mlan_bss_type bss_type)
{
    wifi_get_command_lock();
    HostCmd_DS_COMMAND *cmd         = wifi_get_command_buffer();
    HostCmd_DS_AUTO_TX *auto_tx_cmd = (HostCmd_DS_AUTO_TX *)((t_u8 *)cmd + S_DS_GEN);

    if (auto_null_tx == NULL)
        return -WM_E_INVAL;

    (void)memset(cmd, 0x00, sizeof(HostCmd_DS_COMMAND));
    cmd->seq_num        = HostCmd_SET_SEQ_NO_BSS_INFO(0 /* seq_num */, 0 /* bss_num */, bss_type);
    cmd->command        = wlan_cpu_to_le16(HostCmd_CMD_AUTO_TX);
    cmd->size           = S_DS_GEN + sizeof(HostCmd_DS_AUTO_TX);
    auto_tx_cmd->action = wlan_cpu_to_le16(HostCmd_ACT_GEN_SET);

    if (auto_null_tx->start)
    {
        MrvlIEtypes_Auto_Null_Tx_t *auto_null_tx_tlv =
            (MrvlIEtypes_Auto_Null_Tx_t *)((t_u8 *)auto_tx_cmd + sizeof(auto_tx_cmd->action));
        auto_null_tx_tlv->header.type = wlan_cpu_to_le16(TLV_TYPE_AUTO_TX);
        auto_null_tx_tlv->header.len =
            wlan_cpu_to_le16(sizeof(MrvlIEtypes_Auto_Null_Tx_t) - sizeof(MrvlIEtypesHeader_t));
        auto_null_tx_tlv->interval = auto_null_tx->interval;
        auto_null_tx_tlv->priority = auto_null_tx->priority;
        /* Packet index, set to 0 for auto null tx */
        auto_null_tx_tlv->index = 0;
        /* getTodToAForPkts, set to 0 for auto null tx */
        auto_null_tx_tlv->getTodToAForPkts = 0;
        auto_null_tx_tlv->frame_len        = sizeof(auto_null_tx_tlv->dest_mac_addr) +
                                      sizeof(auto_null_tx_tlv->dest_mac_addr) +
                                      sizeof(auto_null_tx_tlv->frame_body_len);

        (void)memcpy((void *)auto_null_tx_tlv->dest_mac_addr, (const void *)auto_null_tx->dst_mac,
                     MLAN_MAC_ADDR_LENGTH);
        (void)memcpy((void *)auto_null_tx_tlv->src_mac_addr, (const void *)auto_null_tx->src_mac, MLAN_MAC_ADDR_LENGTH);
        /* fram body length, '0x00,0x00' for auto null tx */
        auto_null_tx_tlv->frame_body_len = 0;
        cmd->size                        = cmd->size + sizeof(MrvlIEtypes_Auto_Null_Tx_t);
        cmd->size                        = wlan_cpu_to_le16(cmd->size);
    }

    cmd->result = 0x00;

    wifi_wait_for_cmdresp(NULL);

    return wm_wifi.cmd_resp_status;
}
#endif
