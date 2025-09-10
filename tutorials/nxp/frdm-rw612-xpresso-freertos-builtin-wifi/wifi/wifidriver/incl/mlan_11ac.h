/** @file mlan_11ac.h
 *
 *  @brief This file defines the private and adapter data
 *  structures and declares global function prototypes used
 *  in MLAN module.
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _MLAN_11AC_H_
#define _MLAN_11AC_H_

#include "mlan_11n_aggr.h"
#include "mlan_11n_rxreorder.h"
#include "mlan_wmm.h"
#if 0
void wlan_show_dot11acdevcap(pmlan_adapter pmadapter, t_u32 cap);
void wlan_show_dot11acmcssupport(pmlan_adapter pmadapter, t_u32 support);
#endif
t_u16 wlan_convert_mcsmap_to_maxrate(mlan_private *priv, t_u8 bands, t_u16 mcs_map);
void wlan_fill_vht_cap_tlv(mlan_private *priv, MrvlIETypes_VHTCap_t *pvht_cap, t_u16 bands, t_u8 flag);
#if 0
void wlan_fill_vht_cap_ie(mlan_private *priv, IEEEtypes_VHTCap_t *pvht_cap, t_u16 bands);
#endif
int wlan_cmd_append_11ac_tlv(mlan_private *pmpriv, BSSDescriptor_t *pbss_desc, t_u8 **ppbuffer);
#if 0
mlan_status wlan_11ac_cfg_ioctl(pmlan_adapter pmadapter, pmlan_ioctl_req pioctl_req);
void wlan_update_11ac_cap(mlan_private *pmpriv);
#endif
#if CONFIG_11AC

t_u8 wlan_get_center_freq_idx(mlan_private *pmpriv, t_u8 band, t_u32 pri_chan, t_u8 chan_bw);

#endif /*CONFIG_11AC*/
t_u8 wlan_11ac_bandconfig_allowed(mlan_private *pmpriv, t_u16 bss_band);

mlan_status wlan_cmd_11ac_cfg(IN pmlan_private pmpriv,
                              IN HostCmd_DS_COMMAND *cmd,
                              IN t_u16 cmd_action,
                              IN t_void *pdata_buf);
#if 0
mlan_status wlan_ret_11ac_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN mlan_ioctl_req *pioctl_buf);
#endif
mlan_status wlan_11ac_ioctl_vhtcfg(IN mlan_private *pmpriv, IN t_u8 action, IN mlan_ds_11ac_vht_cfg *vht_cfg);

#endif /* _MLAN_11AC_H_ */
