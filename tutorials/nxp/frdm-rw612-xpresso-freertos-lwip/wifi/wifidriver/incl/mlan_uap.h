/** @file mlan_uap.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of uap functionalities
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    02/05/2009: initial version
********************************************************/

#ifndef _MLAN_UAP_H_
#define _MLAN_UAP_H_

/** Convert TxPD to little endian format from CPU format */
#define uap_endian_convert_TxPD(x) \
    do                             \
    {                              \
    } while (0)
/** Convert RxPD from little endian format to CPU format */
#define uap_endian_convert_RxPD(x) \
    do                             \
    {                              \
    } while (0)

/** Guard mlan_private access when uAP disabled */
#if UAP_SUPPORT
#define CHECK_BSS_TYPE(id, rvl)
#define CHECK_BSS_TYPE_RET_VOID(id)
#else
#define CHECK_BSS_TYPE(id, rvl) \
    if ((int)(id) > MLAN_BSS_ROLE_STA) \
        return (rvl)
#define CHECK_BSS_TYPE_RET_VOID(id) \
    if ((int)(id) > MLAN_BSS_ROLE_STA) \
        return
#endif

/** Band config 5GHz */
#define UAP_BAND_CONFIG_5GHZ 0x01

/** Packet forwarding to be done by FW or host */
#define PKT_FWD_FW_BIT 0x01U
/** Intra-BSS broadcast packet forwarding allow bit */
#define PKT_FWD_INTRA_BCAST 0x02
/** Intra-BSS unicast packet forwarding allow bit */
#define PKT_FWD_INTRA_UCAST 0x04
/** Inter-BSS unicast packet forwarding allow bit */
#define PKT_FWD_INTER_UCAST 0x08
/** Intra-BSS unicast packet */
#define PKT_INTRA_UCAST 0x01
/** Inter-BSS unicast packet */
#define PKT_INTER_UCAST 0x02
/** Enable Host PKT forwarding */
#define PKT_FWD_ENABLE_BIT 0x01U


#if UAP_SUPPORT
mlan_status wlan_ops_uap_ioctl(t_void *adapter, pmlan_ioctl_req pioctl_req);

mlan_status wlan_ops_uap_prepare_cmd(IN t_void *priv,
                                     IN t_u16 cmd_no,
                                     IN t_u16 cmd_action,
                                     IN t_u32 cmd_oid,
                                     IN t_void *pioctl_buf,
                                     IN t_void *pdata_buf,
                                     IN t_void *pcmd_buf);



int wifi_uap_enable_11d_support(void);

int wifi_uap_group_rekey_timer_getset(uint8_t action, uint32_t *group_rekey_timer);

int wifi_uap_mcbc_rate_getset(uint8_t action, uint16_t *mcbc_rate);

int wifi_uap_tx_power_getset(uint8_t action, uint8_t *tx_power_dbm);

int wifi_set_uap_max_clients(unsigned int *max_sta_num);

int wifi_get_uap_max_clients(unsigned int *max_sta_num);

void wifi_uap_set_ecsa(void);

int wifi_uap_ctrl_deauth(bool enable);

void wifi_uap_set_hidden_ssid(const t_u8 bcast_ssid_ctl);

void wifi_uap_set_htcapinfo(const t_u16 ht_cap_info);

void wifi_uap_set_beacon_period(const t_u16 beacon_period);

#else
static inline mlan_status wlan_ops_uap_ioctl(t_void *adapter, pmlan_ioctl_req pioctl_req)
{
    (void)PRINTF("wlan_ops_uap_ioctl UAP not supported %s:%d\r\n", __func__, __LINE__);
    return MLAN_STATUS_FAILURE;
}
#endif /* UAP_SUPPORT */

#endif /* _MLAN_UAP_H_ */
