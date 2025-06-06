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

/**
 *  @brief This function checks the conditions and sends packet to device
 *
 *  @param priv	   A pointer to mlan_private structure
 *  @param pmbuf   A pointer to the mlan_buffer for process
 *  @param tx_param A pointer to mlan_tx_param structure
 *
 *  @return 	    MLAN_STATUS_SUCCESS/MLAN_STATUS_PENDING --success, otherwise failure
 */
