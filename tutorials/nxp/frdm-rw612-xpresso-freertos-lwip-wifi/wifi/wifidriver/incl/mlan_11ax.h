#if CONFIG_11AX
/** @file mlan_11ax.h
 *
 *  @brief This file defines the private and adapter data
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright 2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _MLAN_11AX_H_
#define _MLAN_11AX_H_

/** device support 2.4G 40MHZ */
#define AX_2G_40MHZ_SUPPORT MBIT(1)
/** device support 2.4G 242 tone RUs */
#define AX_2G_20MHZ_SUPPORT MBIT(5)

#ifdef RW610
/**
 *  firmware 11ax tlv sequence number, for judging 2.4G or 5G tlv
 *  firmware add 11ax tlv as 0-5G 1-2.4G to stack
 *  so driver need to parse 11ax tlv as 0-2.4G 1-5G in converse
 */
#define AX_2G_TLV_INDEX 0
#define AX_5G_TLV_INDEX 1
#endif

t_u8 wlan_check_ap_11ax_twt_supported(BSSDescriptor_t *pbss_desc);
t_u8 wlan_check_11ax_twt_supported(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc);
t_u16 wlan_fill_he_cap_tlv(mlan_private *pmpriv, t_u16 band, MrvlIEtypes_Extension_t *phe_cap, t_u8 flag);
int wlan_cmd_append_11ax_tlv(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc, t_u8 **ppbuffer);
void wlan_update_11ax_cap(mlan_adapter *pmadapter,
                          MrvlIEtypes_Extension_t *hw_he_cap
#ifdef RW610
                          ,
                          int tlv_idx
#endif
);
t_u16 wlan_11ax_bandconfig_allowed(mlan_private *pmpriv, t_u16 bss_band);
mlan_status wlan_11ax_cfg_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req);
mlan_status wlan_11ax_ioctl_cmd(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req);
int wlan_cmd_11ax_cfg(mlan_private *pmpriv, t_u16 action, mlan_ds_11ax_he_cfg *he_cfg);
mlan_status wlan_ret_11ax_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ds_11ax_he_cfg *hecfg);
mlan_status wlan_cmd_11ax_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf);
mlan_status wlan_ret_11ax_cmd(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf);
#if CONFIG_11AX_TWT
mlan_status wlan_cmd_twt_cfg(pmlan_private pmpriv, HostCmd_DS_COMMAND *cmd, t_u16 cmd_action, t_void *pdata_buf);
#endif /* CONFIG_11AX_TWT */
#endif /* _MLAN_11AX_H_ */
#endif /* CONFIG_11AX */
