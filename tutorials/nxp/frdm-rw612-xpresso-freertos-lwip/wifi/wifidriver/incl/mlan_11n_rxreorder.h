/** @file mlan_11n_rxreorder.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of 11n RxReordering functionalities
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/

#ifndef _MLAN_11N_RXREORDER_H_
#define _MLAN_11N_RXREORDER_H_

/** Max value a TID can take = 2^12 = 4096 */
#define MAX_TID_VALUE 4096U
/** 2^11 = 2048 */
#define TWOPOW11 2048U

/** Tid Mask used for extracting TID from BlockAckParamSet */
#define BLOCKACKPARAM_TID_MASK 0x3CU
/** Tid position in BlockAckParamSet */
#define BLOCKACKPARAM_TID_POS 2U
/** WinSize Mask used for extracting WinSize from BlockAckParamSet */
#define BLOCKACKPARAM_WINSIZE_MASK 0xffc0U
/** WinSize Mask used for extracting WinSize from BlockAckParamSet */
#define BLOCKACKPARAM_AMSDU_SUPP_MASK 0x1U
/** WinSize position in BlockAckParamSet */
#define BLOCKACKPARAM_WINSIZE_POS 6U
/** Position of TID in DelBA Param set */
#define DELBA_TID_POS 12
/** Position of INITIATOR in DelBA Param set */
#define DELBA_INITIATOR_POS 11
/** Reason code: Requested from peer STA as it does not want to use the mechanism */
#define REASON_CODE_STA_DONT_WANT 37
/** Reason code: Requested from peer STA due to timeout*/
#define REASON_CODE_STA_TIMEOUT 39
/** Type: send delba command */
#define TYPE_DELBA_SENT 1
/** Type: recieve delba command */
#define TYPE_DELBA_RECEIVE 2U
/** Set Initiator Bit */
#define DELBA_INITIATOR(paramset) ((paramset) = ((paramset) | (1U << 11)))
/** Reset Initiator Bit for recipient */
#define DELBA_RECIPIENT(paramset) ((paramset) = ((paramset) & ~(1U << 11)))
/** Immediate block ack */
#define IMMEDIATE_BLOCK_ACK 0x2U

/** The request has been declined */
#define ADDBA_RSP_STATUS_DECLINED 37U
/** ADDBA response status : Reject */
#define ADDBA_RSP_STATUS_REJECT 1U
/** ADDBA response status : Accept */
#define ADDBA_RSP_STATUS_ACCEPT 0U

/** DEFAULT SEQ NUM */
#define DEFAULT_SEQ_NUM 0xffffU

/** Indicate packet has been dropped in FW */
#define RX_PKT_DROPPED_IN_FW 0xffffffffU

#if CONFIG_RSN_REPLAY_DETECTION
t_void wlan_reset_pn_value(mlan_private *pmpriv, mlan_ds_encrypt_key *key);
t_u8 wlan_is_rsn_replay_attack(mlan_private *pmpriv, t_void *payload, RxReorderTbl *rx_reor_tbl_ptr);
void wlan_reset_pn_on_rekey(mlan_private *priv, t_u8 *event_buf);
#endif

mlan_status mlan_11n_rxreorder_pkt(void *priv, t_u16 seq_num, t_u16 tid, t_u8 *ta, t_u8 pkt_type, void *payload);
void mlan_11n_update_bastream_tbl(mlan_private *priv, int Tid, t_u8 *PeerMACAddr, t_u8 type, int initiator);
void wlan_11n_ba_stream_timeout(mlan_private *priv, HostCmd_DS_11N_BATIMEOUT *event);
mlan_status wlan_ret_11n_addba_resp(mlan_private *priv, HostCmd_DS_COMMAND *resp);
mlan_status wlan_cmd_11n_delba(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf);
mlan_status wlan_cmd_11n_addba_rspgen(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf);
mlan_status wlan_cmd_11n_uap_addba_rspgen(mlan_private *priv, HostCmd_DS_COMMAND *cmd, void *pdata_buf);
mlan_status wlan_cmd_11n_addba_req(mlan_private *priv, HostCmd_DS_COMMAND *cmd, t_void *pdata_buf);
void wlan_11n_cleanup_reorder_tbl(mlan_private *priv);
RxReorderTbl *wlan_11n_get_rxreorder_tbl(mlan_private *priv, int tid, t_u8 *ta);
void wlan_11n_rxba_sync_event(mlan_private *priv, t_u8 *event_buf, t_u16 len);

/** send delba for all entries in reorder_tbl */
t_void wlan_send_delba_to_all_in_reorder_tbl(pmlan_private priv);
void wlan_update_rxreorder_tbl(pmlan_adapter pmadapter, bool flag);

/** clean up reorder_tbl */
void wlan_cleanup_reorder_tbl(mlan_private *priv, t_u8 *ta);
#endif /* _MLAN_11N_RXREORDER_H_ */
