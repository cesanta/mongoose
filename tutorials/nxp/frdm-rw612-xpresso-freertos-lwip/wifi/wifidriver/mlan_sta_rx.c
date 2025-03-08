/** @file mlan_sta_rx.c
 *
 *  @brief  This file provides  handling of RX in MLA  module.
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    10/27/2008: initial version
********************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
  Local Variables
********************************************************/

/** Ethernet II header */
typedef struct
{
    /** Ethernet II header destination address */
    t_u8 dest_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet II header source address */
    t_u8 src_addr[MLAN_MAC_ADDR_LENGTH];
    /** Ethernet II header length */
    t_u16 ethertype;

} EthII_Hdr_t;

/** IPv4 ARP request header */
typedef MLAN_PACK_START struct
{
    /** Hardware type */
    t_u16 Htype;
    /** Protocol type */
    t_u16 Ptype;
    /** Hardware address length */
    t_u8 addr_len;
    /** Protocol address length */
    t_u8 proto_len;
    /** Operation code */
    t_u16 op_code;
    /** Source mac address */
    t_u8 src_mac[MLAN_MAC_ADDR_LENGTH];
    /** Sender IP address */
    t_u8 src_ip[4];
    /** Destination mac address */
    t_u8 dst_mac[MLAN_MAC_ADDR_LENGTH];
    /** Destination IP address */
    t_u8 dst_ip[4];
} MLAN_PACK_END IPv4_ARP_t;

/** IPv6 Nadv packet header */
typedef MLAN_PACK_START struct
{
    /** IP protocol version */
    t_u8 version;
    /** flow label */
    t_u8 flow_lab[3];
    /** Payload length */
    t_u16 payload_len;
    /** Next header type */
    t_u8 next_hdr;
    /** Hot limit */
    t_u8 hop_limit;
    /** Source address */
    t_u8 src_addr[16];
    /** Destination address */
    t_u8 dst_addr[16];
    /** ICMP type */
    t_u8 icmp_type;
    /** IPv6 Code */
    t_u8 ipv6_code;
    /** IPv6 Checksum */
    t_u16 ipv6_checksum;
    /** Flags */
    t_u32 flags;
    /** Target address */
    t_u8 taget_addr[16];
    /** Reserved */
    t_u8 rev[8];
} MLAN_PACK_END IPv6_Nadv_t;

/********************************************************
  Global Variables
********************************************************/

/********************************************************
  Local Functions
********************************************************/

/********************************************************
  Global functions
********************************************************/
/**
 *  @brief This function processes received packet and forwards it
 *  		to kernel/upper layer
 *
 *  @param pmadapter A pointer to mlan_adapter
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return 	   MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_process_rx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_private priv = pmadapter->priv[pmbuf->bss_index];
    /* RxPacketHdr_t *prx_pkt; */
    RxPD *prx_pd;
    ENTER();

    prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);
    /* Note: Important. We do not have actual data @ prx_pd->rx_pkt_offset */
    /* prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset); */
    /* prx_pkt = (RxPacketHdr_t *)pmbuf->pdesc; */

/** Small debug type */
#define DBG_TYPE_SMALL 2
/** Size of debugging structure */
#define SIZE_OF_DBG_STRUCT 4
    /*
       This additional processing does not seem to be required by us for
       the moment. Let skip it for now. They seem to be doing some kind of
       ethernet header processing.
    */

    priv->rxpd_rate = prx_pd->rx_rate;
#ifdef SD8801
    priv->rxpd_htinfo = prx_pd->ht_info;
#else
    priv->rxpd_rate_info = prx_pd->rate_info;
#endif

#if CONFIG_WIFI_PKT_FWD
    if ((GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) && priv->pkt_fwd)
    {
        /* Check if needs to do packet forwarding */
        ret = wlan_process_uap_rx_packet(priv, pmbuf);
        goto done;
    }
#endif

    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
    if (ret == MLAN_STATUS_FAILURE)
    {
        pmbuf->status_code = (t_u32)MLAN_ERROR_PKT_INVALID;
        PRINTM(MERROR, "Rx Error: moal_recv_packet returned error\n");
    }

#if CONFIG_WIFI_PKT_FWD
done:
#endif
    if (ret != MLAN_STATUS_PENDING)
    {
        wlan_free_mlan_buffer(pmadapter, pmbuf);
    }
    LEAVE();

    return ret;
}

/**
 *   @brief This function processes the received buffer
 *
 *   @param adapter A pointer to mlan_adapter
 *   @param pmbuf     A pointer to the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_ops_process_rx_packet(IN t_void *adapter, IN pmlan_buffer pmbuf)
{
    pmlan_adapter pmadapter = (pmlan_adapter)adapter;
    mlan_status ret         = MLAN_STATUS_SUCCESS;
    RxPD *prx_pd;
    RxPacketHdr_t *prx_pkt;
    pmlan_private priv            = pmadapter->priv[pmbuf->bss_index];
    t_u8 ta[MLAN_MAC_ADDR_LENGTH] = {0};
    t_u16 rx_pkt_type             = 0;
    ENTER();
    TxBAStreamTbl *ptx_tbl = NULL;

    prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);
    /* Endian conversion */
    endian_convert_RxPD(prx_pd);
    rx_pkt_type = prx_pd->rx_pkt_type;

    /* Note: We do not have data @ some offset of pbuf. pbuf only has RxPD */
    /* prx_pkt = (RxPacketHdr_t *) ((t_u8 *) prx_pd + prx_pd->rx_pkt_offset); */
    prx_pkt = (RxPacketHdr_t *)pmbuf->pdesc;

    /* PRINTM(MMSG, "%p + %d: O: %d PL: %d DL: %d\n\r", */
    /* 	     pmbuf->pbuf, pmbuf->data_offset, */
    /* 	     prx_pd->rx_pkt_offset, prx_pd->rx_pkt_length, pmbuf->data_len); */

    if ((prx_pd->rx_pkt_offset + prx_pd->rx_pkt_length) > (t_u16)pmbuf->data_len)
    {
        PRINTM(MERROR,
               "Wrong rx packet: len=%d,rx_pkt_offset=%d,"
               " rx_pkt_length=%d\n",
               pmbuf->data_len, prx_pd->rx_pkt_offset, prx_pd->rx_pkt_length);
        pmbuf->status_code = (t_u32)MLAN_ERROR_PKT_SIZE_INVALID;
        ret                = MLAN_STATUS_FAILURE;
        wlan_free_mlan_buffer(pmadapter, pmbuf);
        goto done;
    }
    pmbuf->data_len = prx_pd->rx_pkt_offset + prx_pd->rx_pkt_length;

    if (pmadapter->priv[pmbuf->bss_index]->mgmt_frame_passthru_mask && prx_pd->rx_pkt_type == PKT_TYPE_MGMT_FRAME)
    {
        /* Check if this is mgmt packet and needs to forwarded to app as an
           event */
        /* fixme */
        wifi_e("Is a management packet expected here?");
        while (true)
        {
            OSA_TimeDelay(10);
        }
    }

#ifdef DUMP_PACKET_MAC
    dump_mac_addr("Own: ", priv->curr_addr);
    dump_mac_addr("Dest: ", prx_pkt->eth803_hdr.dest_addr);
#endif /* DUMP_PACKET_MAC */

    if (GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP)
    {
        /* txbastream table also is used as connected STAs data base */
        ptx_tbl = wlan_11n_get_txbastream_tbl(priv, prx_pkt->eth803_hdr.src_addr);
    }

    /*
     * If 11n isn't enabled, or if the packet is not an unicast packet for STA case,
     * then send the packet directly to os. Don't pass thru rx reordering
     */
    if (((GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_STA) &&
         ((!IS_11N_ENABLED(priv)) ||
          __memcmp(priv->adapter, priv->curr_addr, prx_pkt->eth803_hdr.dest_addr, MLAN_MAC_ADDR_LENGTH))) ||
        ((GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) && ptx_tbl && (!ptx_tbl->ampdu_supported[0])))
    {
        (void)wlan_process_rx_packet(pmadapter, pmbuf);
        goto done;
    }

    if (queuing_ra_based(priv) == MTRUE)
    {
        if ((GET_BSS_ROLE(priv) == MLAN_BSS_ROLE_UAP) && ptx_tbl && (rx_pkt_type != PKT_TYPE_BAR) &&
            (prx_pd->priority < MAX_NUM_TID))
        {
            ptx_tbl->rx_seq[prx_pd->priority] = prx_pd->seq_num;
        }
        (void)__memcpy(pmadapter, ta, prx_pkt->eth803_hdr.src_addr, MLAN_MAC_ADDR_LENGTH);
    }
    else
    {
        if ((rx_pkt_type != PKT_TYPE_BAR) && (prx_pd->priority < MAX_NUM_TID))
        {
            priv->rx_seq[prx_pd->priority] = prx_pd->seq_num;
        }
        (void)__memcpy(pmadapter, ta, priv->curr_bss_params.bss_descriptor.mac_address, MLAN_MAC_ADDR_LENGTH);
    }

    /* Reorder and send to OS */
    if ((ret = mlan_11n_rxreorder_pkt(priv, prx_pd->seq_num, prx_pd->priority, ta, (t_u8)prx_pd->rx_pkt_type,
                                      (void *)pmbuf)) ||
        (rx_pkt_type == PKT_TYPE_BAR))
    {
        wlan_free_mlan_buffer(pmadapter, pmbuf);
    }

done:

    LEAVE();
    return (ret);
}
