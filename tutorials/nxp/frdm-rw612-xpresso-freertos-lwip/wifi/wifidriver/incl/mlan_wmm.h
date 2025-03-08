/** @file mlan_wmm.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of wmm functionalities
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/****************************************************
Change log:
    10/24/2008: initial version
****************************************************/

#ifndef _MLAN_WMM_H_
#define _MLAN_WMM_H_

/**
 *  @brief This function gets the TID
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param ptr          A pointer to RA list table
 *
 *  @return 	    TID
 */
static INLINE int wlan_get_tid(pmlan_adapter pmadapter, raListTbl *ptr)
{
    pmlan_buffer mbuf;

    ENTER();
    mbuf = (pmlan_buffer)(void *)util_peek_list(pmadapter->pmoal_handle, &ptr->buf_head, MNULL, MNULL);
    LEAVE();

    return mbuf->priority;
}

/**
 *  @brief This function gets the length of a list
 *
 *  @param pmadapter    A pointer to mlan_adapter structure
 *  @param head         A pointer to mlan_list_head
 *
 *  @return 	    Length of list
 */
static INLINE int wlan_wmm_list_len(pmlan_adapter pmadapter, pmlan_list_head head)
{
    pmlan_linked_list pos;
    int count = 0;

    ENTER();

    pos = head->pnext;

    while (pos != (pmlan_linked_list)(void *)head)
    {
        ++count;
        pos = pos->pnext;
    }

    LEAVE();
    return count;
}

/**
 *  @brief This function requests a ralist lock
 *
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             N/A
 */
static INLINE t_void wlan_request_ralist_lock(pmlan_private priv)
{

    ENTER();

    OSA_MutexLock((osa_mutex_handle_t)priv->tx_ba_stream_tbl_lock, osaWaitForever_c);

    LEAVE();
    return;
}

/**
 *  @brief This function releases a lock on ralist
 *
 *  @param priv         A pointer to mlan_private structure
 *
 *  @return             N/A
 */
static INLINE t_void wlan_release_ralist_lock(pmlan_private priv)
{

    ENTER();

    OSA_MutexUnlock((osa_mutex_handle_t)priv->tx_ba_stream_tbl_lock);


    LEAVE();
    return;
}

/** Add buffer to WMM Tx queue */
void wlan_wmm_add_buf_txqueue(pmlan_adapter pmadapter, pmlan_buffer pmbuf);
/** Add to RA list */
void wlan_ralist_add(mlan_private *priv, t_u8 *ra);
/** Update the RA list */
int wlan_ralist_update(mlan_private *priv, t_u8 *old_ra, t_u8 *new_ra);
#ifdef STA_SUPPORT
/** WMM status change command handler */
mlan_status wlan_cmd_wmm_status_change(pmlan_private priv);
#endif /* STA_SUPPORT */
/** Check if WMM lists are empty */
int wlan_wmm_lists_empty(pmlan_adapter pmadapter);
/** Process WMM transmission */
t_void wlan_wmm_process_tx(pmlan_adapter pmadapter);
/** Test to see if the ralist ptr is valid */
int wlan_is_ralist_valid(mlan_private *priv, raListTbl *ra_list, int tid);
raListTbl *wlan_wmm_get_ralist_node(pmlan_private priv, t_u8 tid, t_u8 *ra_addr);
t_u8 wlan_get_random_ba_threshold(pmlan_adapter pmadapter);

/** Compute driver packet delay */
t_u8 wlan_wmm_compute_driver_packet_delay(pmlan_private priv, const pmlan_buffer pmbuf);
/** Initialize WMM */
t_void wlan_wmm_init(pmlan_adapter pmadapter);
/** Initialize WMM paramter */
t_void wlan_init_wmm_param(pmlan_adapter pmadapter);
/** Setup WMM queues */
extern void wlan_wmm_setup_queues(pmlan_private priv);
/* Setup default queues */
void wlan_wmm_default_queue_priorities(pmlan_private priv);

#ifdef STA_SUPPORT
/** Process WMM association request */
extern t_u32 wlan_wmm_process_association_req(pmlan_private priv,
                                              t_u8 **ppAssocBuf,
                                              IEEEtypes_WmmParameter_t *pWmmIE,
                                              IEEEtypes_HTCap_t *pHTCap);
#endif /* STA_SUPPORT */

/** setup wmm queue priorities */
void wlan_wmm_setup_queue_priorities(pmlan_private priv, IEEEtypes_WmmParameter_t *wmm_ie);

/** Downgrade WMM priority queue */
void wlan_wmm_setup_ac_downgrade(pmlan_private priv);
/** select WMM queue */
t_u8 wlan_wmm_select_queue(mlan_private *pmpriv, t_u8 tid);
#if UAP_SUPPORT
t_void wlan_wmm_delete_peer_ralist(pmlan_private priv, t_u8 *mac);
#endif

/** WMM TS_STATUS command handler */
extern mlan_status wlan_cmd_wmm_ts_status(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);
/** WMM ADDTS request command response handler */
extern mlan_status wlan_ret_wmm_addts_req(IN pmlan_private pmpriv,
                                          const IN HostCmd_DS_COMMAND *resp,
                                          OUT mlan_ioctl_req *pioctl_buf);
/** WMM DELTS request command response handler */
extern mlan_status wlan_ret_wmm_delts_req(IN pmlan_private pmpriv,
                                          const IN HostCmd_DS_COMMAND *resp,
                                          OUT mlan_ioctl_req *pioctl_buf);
#ifdef STA_SUPPORT
/*
 *  Functions used in the cmd handling routine
 */
/** WMM ADDTS request command handler */
extern mlan_status wlan_cmd_wmm_addts_req(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);
/** WMM QUEUE_STATS command handler */
extern mlan_status wlan_cmd_wmm_queue_stats(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);
/** WMM QUEUE_CONFIG command handler */
extern mlan_status wlan_cmd_wmm_queue_config(IN pmlan_private pmpriv,
                                             OUT HostCmd_DS_COMMAND *cmd,
                                             IN t_void *pdata_buf);
/** WMM get status command response handler */
extern mlan_status wlan_ret_wmm_get_status(IN pmlan_private priv, IN t_u8 *ptlv, IN int resp_len);
/** WMM TS_STATUS command response handler */
extern mlan_status wlan_ret_wmm_ts_status(IN pmlan_private pmpriv,
                                          IN HostCmd_DS_COMMAND *resp,
                                          OUT mlan_ioctl_req *pioctl_buf);
/** WMM QUEUE_CONFIG command response handler */
extern mlan_status wlan_ret_wmm_queue_config(IN pmlan_private pmpriv,
                                             const IN HostCmd_DS_COMMAND *resp,
                                             OUT mlan_ioctl_req *pioctl_buf);
/** WMM QUEUE_STATS command response handler */
extern mlan_status wlan_ret_wmm_queue_stats(IN pmlan_private pmpriv,
                                            const IN HostCmd_DS_COMMAND *resp,
                                            OUT mlan_ioctl_req *pioctl_buf);
/** WMM DELTS request command handler */
extern mlan_status wlan_cmd_wmm_delts_req(IN pmlan_private pmpriv, OUT HostCmd_DS_COMMAND *cmd, IN t_void *pdata_buf);

#endif /* STA_SUPPORT */

#if UAP_SUPPORT
t_void wlan_wmm_delete_peer_ralist(pmlan_private priv, t_u8 *mac);
#endif

/* process wmm_param_config command */
mlan_status wlan_cmd_wmm_param_config(pmlan_private pmpriv,
                                      HostCmd_DS_COMMAND *cmd,
                                      t_u8 cmd_action,
                                      t_void *pdata_buf);

/* process wmm_param_config command response */
mlan_status wlan_ret_wmm_param_config(pmlan_private pmpriv, const HostCmd_DS_COMMAND *resp, mlan_ioctl_req *pioctl_buf);

#if CONFIG_WMM
/* wmm enhance buffer pool */
#define MAX_WMM_BUF_NUM 16
#define WMM_DATA_LEN    1580
#define OUTBUF_WMM_LEN  (sizeof(outbuf_t))

typedef struct
{
    mlan_linked_list entry;
    t_u8 intf_header[INTF_HEADER_LEN];
    TxPD tx_pd;
#if CONFIG_TX_RX_ZERO_COPY
    t_u8 eth_header[ETH_HDR_LEN];
#if CONFIG_AMSDU_IN_AMPDU
    t_u8 llc_header[LLC_SNAP_LEN];
#endif
    /* Data payload pointer */
    t_u8 *payload;
    /* Packet buffer structure pointer */
    void *buffer;
#else
    t_u8 data[WMM_DATA_LEN];
#endif
} outbuf_t;

typedef struct
{
    mlan_linked_list entry;
    t_u8 intf_header[INTF_HEADER_LEN];
    TxPD tx_pd;
    t_u8 data[1];
} bypass_outbuf_t;

/* transfer destination address to receive address */
void wifi_wmm_da_to_ra(uint8_t *da, uint8_t *ra);

/* wmm enhance get free buffer */
uint8_t *wifi_wmm_get_outbuf_enh(
    uint32_t *outbuf_len, mlan_wmm_ac_e queue, const uint8_t interface, uint8_t *ra, bool *is_tx_pause);

/* wmm enhance enqueue tx buffer */
int wlan_wmm_add_buf_txqueue_enh(const uint8_t interface, const uint8_t *buffer, const uint16_t len, uint8_t pkt_prio);

/* wmm enhance buffer pool management */
outbuf_t *wifi_wmm_buf_get(void);
void wifi_wmm_buf_put(outbuf_t *buf);
int wifi_wmm_buf_pool_init(uint8_t *pool);
void wifi_wmm_buf_pool_deinit(void);

/* wmm enhance ralist operation */
void wlan_ralist_add_enh(mlan_private *priv, t_u8 *ra);
int wlan_ralist_update_enh(mlan_private *priv, t_u8 *old_ra, t_u8 *new_ra);
void wlan_ralist_pkts_free_enh(mlan_private *priv, raListTbl *ra_list, t_u8 ac);
void wlan_ralist_del_enh(mlan_private *priv, t_u8 *ra);
void wlan_ralist_del_all_enh(mlan_private *priv);
void wlan_ralist_deinit_enh(mlan_private *priv);

/* debug statistics */
void wifi_wmm_drop_err_mem(const uint8_t interface);
void wifi_wmm_drop_no_media(const uint8_t interface);
void wifi_wmm_drop_retried_drop(const uint8_t interface);
void wifi_wmm_drop_pause_drop(const uint8_t interface);
void wifi_wmm_drop_pause_replaced(const uint8_t interface);

int wifi_bypass_txq_init(void);
void wifi_bypass_txq_deinit(void);
void wlan_get_bypass_lock(uint8_t interface);
void wlan_put_bypass_lock(uint8_t interface);
void wlan_add_buf_bypass_txq(const uint8_t *buffer, const uint8_t interface);
t_u8 wlan_bypass_txq_empty(uint8_t interface);
void wlan_cleanup_bypass_txq(uint8_t interface);
#endif

#endif /* !_MLAN_WMM_H_ */
