/** @file mlan_txrx.c
 *
 *  @brief  This file provides the handling of TX/RX in MLAN
 *
 *  Copyright 2008-2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*************************************************************
Change Log:
    05/11/2009: initial version
************************************************************/
#include <mlan_api.h>

/* Additional WMSDK header files */
#include <wmerrno.h>
#include <osa.h>

#if CONFIG_WIFI_PKT_FWD
#include <wm_net.h>
#if defined(RW610)
#include "wifi-imu.h"
#else
#include "wifi-sdio.h"
#endif
#endif
/* Always keep this include at the end of all include files */
#include <mlan_remap_mem_operations.h>
/********************************************************
                Local Variables
********************************************************/

/********************************************************
                Global Variables
********************************************************/

/********************************************************
                Local Functions
********************************************************/

/********************************************************
                Global Functions
********************************************************/
/**
 *   @brief This function processes the received buffer
 *
 *   @param pmadapter A pointer to mlan_adapter
 *   @param pmbuf     A pointer to the received buffer
 *
 *   @return        MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_handle_rx_packet(pmlan_adapter pmadapter, pmlan_buffer pmbuf)
{
    mlan_status ret    = MLAN_STATUS_SUCCESS;
    pmlan_private priv = wlan_get_priv(pmadapter, MLAN_BSS_ROLE_ANY);
    RxPD *prx_pd;
#ifdef DEBUG_LEVEL1
    t_u32 sec, usec;
#endif

    ENTER();

    prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);
    /* Get the BSS number from RxPD, get corresponding priv */
    priv = wlan_get_priv_by_id(pmadapter, prx_pd->bss_num & BSS_NUM_MASK, prx_pd->bss_type);
    if (priv == MNULL)
    {
        priv = wlan_get_priv(pmadapter, MLAN_BSS_ROLE_ANY);
    }
    pmbuf->bss_index = priv->bss_index;
    PRINTM_GET_SYS_TIME(MDATA, &sec, &usec);
    PRINTM_NETINTF(MDATA, priv);
    /* PRINTM(MDATA, "%lu.%06lu : Data <= FW\n", sec, usec); */
    ret = priv->ops.process_rx_packet(pmadapter, pmbuf);

    LEAVE();
    return ret;
}

#if CONFIG_WIFI_PKT_FWD
/**
 *  @brief This function processes received packet and forwards it
 *             to kernel/upper layer or send back to firmware
 *
 *  @param priv A pointer to mlan_private
 *  @param pmbuf     A pointer to mlan_buffer which includes the received packet
 *
 *  @return       MLAN_STATUS_SUCCESS or MLAN_STATUS_FAILURE
 */
mlan_status wlan_process_uap_rx_packet(mlan_private *priv, pmlan_buffer pmbuf)
{
    mlan_status ret = MLAN_STATUS_SUCCESS;
    pmlan_adapter pmadapter = priv->adapter;
    RxPacketHdr_t *prx_pkt = (RxPacketHdr_t *)pmbuf->pdesc;
    RxPD *prx_pd = (RxPD *)(void *)(pmbuf->pbuf + pmbuf->data_offset);

    /* Don't do packet forwarding in disconnected state */
    if (priv->media_connected == MFALSE)
        goto upload;

    if (prx_pkt->eth803_hdr.dest_addr[0] & 0x01)
    {
        t_u32 pkt_len = sizeof(TxPD) + INTF_HEADER_LEN;
        t_u32 link_point_len = sizeof(mlan_linked_list);
        bypass_outbuf_t *poutbuf = NULL;

#if !CONFIG_MEM_POOLS
        poutbuf = OSA_MemoryAllocate(link_point_len + pkt_len + prx_pd->rx_pkt_length);
#else
        poutbuf = (bypass_outbuf_t *)OSA_MemoryPoolAllocate(buf_1536_MemoryPool);
#endif
        if (!poutbuf)
        {
            wuap_e("[%s] ERR:Cannot allocate buffer!\r\n", __func__);
            return MLAN_STATUS_FAILURE;
        }

        (void)memset((t_u8 *)poutbuf, 0, link_point_len + pkt_len);
        (void)net_stack_buffer_copy_partial(pmbuf->lwip_pbuf, (void *)((t_u8 *)poutbuf + link_point_len + pkt_len),
            (t_u16)prx_pd->rx_pkt_length, 0);
        /* process packet headers with interface header and TxPD */
        process_pkt_hdrs((void *)((t_u8 *)poutbuf + link_point_len), pkt_len + prx_pd->rx_pkt_length,
            WLAN_BSS_TYPE_UAP, 0, 0);
        wlan_add_buf_bypass_txq((t_u8 *)poutbuf, WLAN_BSS_TYPE_UAP);
        send_wifi_driver_bypass_data_event(WLAN_BSS_TYPE_UAP);
    }
    else
    {
        if (wlan_11n_get_txbastream_tbl(priv, prx_pkt->eth803_hdr.dest_addr))
        {
            int iret = net_wifi_pkt_fwd(WLAN_BSS_TYPE_UAP, pmbuf->lwip_pbuf);
            if (iret != WM_SUCCESS)
            {
                ret = MLAN_STATUS_FAILURE;
            }
            net_stack_buffer_free(pmbuf->lwip_pbuf);
#if !(CONFIG_TX_RX_ZERO_COPY) && !(FSL_USDHC_ENABLE_SCATTER_GATHER_TRANSFER)
#if !CONFIG_MEM_POOLS
            /* Free RxPD */
            OSA_MemoryFree(pmbuf->pbuf);
            OSA_MemoryFree(pmbuf);
#else
            OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf->pbuf);
            OSA_MemoryPoolFree(buf_128_MemoryPool, pmbuf);
#endif
#endif
            return ret;
        }
    }

upload:
    ret = pmadapter->callbacks.moal_recv_packet(pmadapter->pmoal_handle, pmbuf);
    if (ret == MLAN_STATUS_FAILURE)
    {
        pmbuf->status_code = (t_u32)MLAN_ERROR_PKT_INVALID;
        PRINTM(MERROR, "uAP Rx Error: moal_recv_packet returned error\n");
    }

    if (ret != MLAN_STATUS_PENDING)
    {
        wlan_free_mlan_buffer(pmadapter, pmbuf);
    }

    LEAVE();
    return ret;
}
#endif
