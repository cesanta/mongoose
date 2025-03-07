/** @file mlan_11n.h
 *
 *  @brief Interface for the 802.11n mlan_11n module implemented in mlan_11n.c
 *
 *  Driver interface functions and type declarations for the 11n module
 *    implemented in mlan_11n.c.
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    12/01/2008: initial version
********************************************************/

#ifndef _MLAN_11N_H_
#define _MLAN_11N_H_

#include "mlan_11n_aggr.h"
#include "mlan_11n_rxreorder.h"
#include "mlan_wmm.h"

/** Print the 802.11n device capability */
void wlan_show_dot11ndevcap(pmlan_adapter pmadapter, t_u32 cap);
/** Print the 802.11n device MCS */
void wlan_show_devmcssupport(pmlan_adapter pmadapter, t_u8 support);
/** Handle the command response of a delete block ack request */
mlan_status wlan_ret_11n_delba(mlan_private *priv, HostCmd_DS_COMMAND *resp);
/** Handle the command response of an add block ack request */
mlan_status wlan_ret_11n_addba_req(mlan_private *priv, HostCmd_DS_COMMAND *resp);
/** Handle the command response of 11ncfg command */
mlan_status wlan_ret_11n_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN mlan_ioctl_req *pioctl_buf);
/** Prepare 11ncfg command */
mlan_status wlan_cmd_11n_cfg(IN pmlan_private pmpriv,
                             IN HostCmd_DS_COMMAND *cmd,
                             IN t_u16 cmd_action,
                             IN t_void *pdata_buf);
/** Prepare TX BF configuration command */
mlan_status wlan_cmd_tx_bf_cfg(IN pmlan_private pmpriv,
                               IN HostCmd_DS_COMMAND *cmd,
                               IN t_u16 cmd_action,
                               IN t_void *pdata_buf);
/** Handle the command response TX BF configuration */
mlan_status wlan_ret_tx_bf_cfg(IN pmlan_private pmpriv, IN HostCmd_DS_COMMAND *resp, IN mlan_ioctl_req *pioctl_buf);
#ifdef STA_SUPPORT
/** Append the 802_11N tlv */
t_u32 wlan_cmd_append_11n_tlv(IN mlan_private *pmpriv, IN BSSDescriptor_t *pbss_desc, OUT t_u8 **ppbuffer);
/** wlan fill HT cap tlv */
void wlan_fill_ht_cap_tlv(mlan_private *priv, MrvlIETypes_HTCap_t *pht_cap, t_u16 bands);
#endif /* STA_SUPPORT */
/** Miscellaneous configuration handler */
mlan_status wlan_11n_cfg_ioctl(IN pmlan_adapter pmadapter, IN pmlan_ioctl_req pioctl_req);
/** Delete Tx BA stream table entry */
void wlan_11n_delete_txbastream_tbl_entry(mlan_private *priv, t_u8 *ra);
/** Delete all Tx BA stream table entries */
void wlan_11n_deleteall_txbastream_tbl(mlan_private *priv);
/** Get Tx BA stream table */
TxBAStreamTbl *wlan_11n_get_txbastream_tbl(mlan_private *priv, t_u8 *ra);
/** update Tx ampud_stat */
void wlan_11n_update_txbastream_tbl_ampdu_stat(mlan_private *priv, t_u8 *ra, t_u8 status, t_u8 tid);
/** update Tx ampdu_supported */
void wlan_11n_update_txbastream_tbl_ampdu_supported(mlan_private *priv, t_u8 *ra, t_u8 supported);
/** update Tx threshold */
void wlan_11n_update_txbastream_tbl_tx_thresh(mlan_private *priv, t_u8 *ra, t_u8 tx_thresh);
/** update Tx ampdu_tx cnt */
void wlan_11n_update_txbastream_tbl_tx_cnt(mlan_private *priv, t_u8 *ra);
/** get sta peer amsdu */
int wlan_11n_get_sta_peer_amsdu(mlan_private *priv);
/** Create Tx BA stream table */
void wlan_11n_create_txbastream_tbl(mlan_private *priv, t_u8 *ra, baStatus_e ba_status);
/** Send ADD BA request */
int wlan_send_addba(mlan_private *priv, int tid, const t_u8 *peer_mac);
/** Send DEL BA request */
mlan_status wlan_send_delba(mlan_private *priv, pmlan_ioctl_req pioctl_req, int tid, t_u8 *peer_mac, int initiator);
/** This function handles the command response of delete a block ack request*/
void wlan_11n_delete_bastream(mlan_private *priv, t_u8 *del_ba);
/** get rx reorder table */
int wlan_get_rxreorder_tbl(mlan_private *priv, rx_reorder_tbl *buf);
/** get tx ba stream table */
int wlan_get_txbastream_tbl(mlan_private *priv, tx_ba_stream_tbl *buf);
#if CONFIG_AMSDU_IN_AMPDU
/** Minimum number of AMSDU */
#define MIN_NUM_AMSDU 2
/** AMSDU Aggr control cmd resp */
mlan_status wlan_ret_amsdu_aggr_ctrl(pmlan_private pmpriv, HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf);
#endif
/** reconfigure tx buf size */
mlan_status wlan_cmd_recfg_tx_buf(mlan_private *priv, HostCmd_DS_COMMAND *cmd, int cmd_action, void *pdata_buf);
/** AMSDU aggr control cmd */
mlan_status wlan_cmd_amsdu_aggr_ctrl(mlan_private *priv, HostCmd_DS_COMMAND *cmd, int cmd_action, void *pdata_buf);

/** clean up txbastream_tbl */
void wlan_11n_cleanup_txbastream_tbl(mlan_private *priv, t_u8 *ra);
/**
 *  @brief This function checks whether a station has 11N enabled or not
 *
 *  @param priv     A pointer to mlan_private
 *  @param mac      station mac address
 *  @return 	    MTRUE or MFALSE
 */
INLINE
static t_u8 is_station_11n_enabled(mlan_private *priv, t_u8 *mac)
{
    sta_node *sta_ptr = MNULL;
    sta_ptr           = wlan_get_station_entry(priv, mac);
    if (sta_ptr != MNULL)
    {
        return (sta_ptr->is_11n_enabled) ? MTRUE : MFALSE;
    }
    return MFALSE;
}

/**
 *  @brief This function get station max amsdu size
 *
 *  @param priv     A pointer to mlan_private
 *  @param mac      station mac address
 *  @return 	    max amsdu size statio supported
 */
INLINE
static t_u16 get_station_max_amsdu_size(mlan_private *priv, t_u8 *mac)
{
    sta_node *sta_ptr = MNULL;
    sta_ptr           = wlan_get_station_entry(priv, mac);
    if (sta_ptr != MNULL)
    {
        return sta_ptr->max_amsdu;
    }
    return 0;
}

/**
 *  @brief This function checks whether a station allows AMPDU or not
 *
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param tid      TID value for ptr
 *  @return 	    MTRUE or MFALSE
 */
#if 0
INLINE
static t_u8
is_station_ampdu_allowed(mlan_private * priv, raListTbl * ptr, int tid)
{
    sta_node *sta_ptr = MNULL;
    if ((sta_ptr = wlan_get_station_entry(priv, ptr->ra))) {
        if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) {
            if (priv->sec_info.wapi_enabled && !sta_ptr->wapi_key_on)
                return MFALSE;
        }
        return ((sta_ptr->ampdu_sta[tid] != BA_STREAM_NOT_ALLOWED)
                ? MTRUE : MFALSE);
    }
    return MFALSE;
}
#endif /* 0 */

/**
 *  @brief This function disable station ampdu for specific tid
 *
 *  @param priv     A pointer to mlan_private
 *  @param tid     tid index
 *  @param ra      station mac address
 *  @return 	   N/A
 */
INLINE
static void disable_station_ampdu(mlan_private *priv, t_u8 tid, t_u8 *ra)
{
    sta_node *sta_ptr = MNULL;
    sta_ptr           = wlan_get_station_entry(priv, ra);
    if (sta_ptr != MNULL)
    {
        sta_ptr->ampdu_sta[tid] = BA_STREAM_NOT_ALLOWED;
    }
    return;
}


/**
 *  @brief This function checks whether AMPDU is allowed or not
 *
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param tid      TID value for ptr
 *
 *  @return 	    MTRUE or MFALSE
 */
#if 0
INLINE
static t_u8
wlan_is_ampdu_allowed(mlan_private * priv, raListTbl * ptr, int tid)
{
#if UAP_SUPPORT
    if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP)
        return is_station_ampdu_allowed(priv, ptr, tid);
#endif /* UAP_SUPPORT */
    if (priv->sec_info.wapi_enabled && !priv->sec_info.wapi_key_on)
        return MFALSE;

    return ((priv->aggr_prio_tbl[tid].ampdu_ap != BA_STREAM_NOT_ALLOWED)
            ? MTRUE : MFALSE);
}
#endif /* 0 */

/**
 *  @brief This function checks whether AMSDU is allowed for BA stream
 *
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param tid	    TID value for ptr
 *
 *  @return 	    MTRUE or MFALSE
 */
INLINE
static int wlan_is_amsdu_in_ampdu_allowed(mlan_private *priv, raListTbl *ptr, int tid)
{
    TxBAStreamTbl *ptx_tbl;
    ENTER();
    ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ptr->ra);
    if (ptx_tbl != MNULL)
    {
        LEAVE();
        return (int)(ptx_tbl->amsdu);
    }
    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function checks whether AMSDU is allowed or not
 *
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param tid      TID value for ptr
 *
 *  @return 	    MTRUE or MFALSE
 */
#if 0
INLINE
static t_u8
wlan_is_amsdu_allowed(mlan_private * priv, raListTbl * ptr, int tid)
{
#if UAP_SUPPORT
    sta_node *sta_ptr = MNULL;
    if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) {
        if ((sta_ptr = wlan_get_station_entry(priv, ptr->ra))) {
            if (priv->sec_info.wapi_enabled && !sta_ptr->wapi_key_on)
                return MFALSE;
        }
    }
#endif /* UAP_SUPPORT */
#define TXRATE_BITMAP_INDEX_MCS0_7 2
    return (((priv->aggr_prio_tbl[tid].amsdu != BA_STREAM_NOT_ALLOWED)
             && ((priv->is_data_rate_auto)
                 || !((priv->bitmap_rates[TXRATE_BITMAP_INDEX_MCS0_7]) & 0x03)))
            ? MTRUE : MFALSE);
}
#endif /* 0 */

/**
 *  @brief This function checks whether a BA stream is available or not
 *
 *  @param priv     A pointer to mlan_private
 *
 *  @return 	    MTRUE or MFALSE
 */
INLINE
static t_u8 wlan_is_bastream_avail(mlan_private *priv)
{
    mlan_private *pmpriv = MNULL;
    t_u8 i               = 0;
    t_u32 bastream_num   = 0;
    for (i = 0; i < priv->adapter->priv_num; i++)
    {
        pmpriv = priv->adapter->priv[i];
        if (pmpriv != MNULL)
        {
            bastream_num += wlan_wmm_list_len(priv->adapter, (pmlan_list_head)&pmpriv->tx_ba_stream_tbl_ptr);
        }
    }
    return ((bastream_num < MLAN_MAX_TX_BASTREAM_SUPPORTED) ? MTRUE : MFALSE);
}


/**
 *  @brief This function checks whether BA stream is setup
 *
 *  @param priv     A pointer to mlan_private
 *  @param ptr      A pointer to RA list table
 *  @param tid	    TID value for ptr
 *
 *  @return 	    MTRUE or MFALSE
 */
INLINE
static int wlan_is_bastream_setup(mlan_private *priv, raListTbl *ptr, int tid)
{
    TxBAStreamTbl *ptx_tbl;

    ENTER();
    ptx_tbl = wlan_11n_get_txbastream_tbl(priv, ptr->ra);
    if (ptx_tbl != MNULL)
    {
        LEAVE();
        return IS_BASTREAM_SETUP(ptx_tbl) ? MTRUE : MFALSE;
    }

    LEAVE();
    return MFALSE;
}

/**
 *  @brief This function checks whether 11n is supported
 *
 *  @param priv     A pointer to mlan_private
 *  @param ra       Address of the receiver STA
 *
 *  @return 	    MTRUE or MFALSE
 */
INLINE
static int wlan_is_11n_enabled(mlan_private *priv, t_u8 *ra)
{
    int ret = MFALSE;
    ENTER();
#if UAP_SUPPORT
    if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP)
    {
        if ((!(ra[0] & 0x01U)) && (priv->is_11n_enabled))
        {
            ret = (int)is_station_11n_enabled(priv, ra);
        }
    }
#endif /* UAP_SUPPORT */
    LEAVE();
    return ret;
}

/**
 *  @brief This function checks whether amsdu is allowed
 *
 *  @param interface     interface to indicate uap or STA
 *  @param pkt_cnt       current packets conuter in the queue
 *
 *  @return 	    MTRUE or MFALSE
 */
#if CONFIG_AMSDU_IN_AMPDU
INLINE
static bool wlan_is_amsdu_allowed(mlan_private *priv, t_u8 interface, t_u8 pkt_cnt, t_u8 tid)
{
    // First stage, only consider tx amsdu on STA side
    if (interface == MLAN_BSS_TYPE_STA && pkt_cnt >= MIN_NUM_AMSDU && priv->is_amsdu_enabled && priv->max_amsdu &&
        wlan_11n_get_sta_peer_amsdu(priv))
    {
        return MTRUE;
    }
    else
    {
        return MFALSE;
    }
}
#endif
#endif /* !_MLAN_11N_H_ */
