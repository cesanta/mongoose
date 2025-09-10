/** @file mlan_11n_aggr.h
 *
 *  @brief This file contains related macros, enum, and struct
 *  of 11n aggregation functionalities
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    11/10/2008: initial version
********************************************************/

#ifndef _MLAN_11N_AGGR_H_
#define _MLAN_11N_AGGR_H_

/** Aggregate 11N packets */
mlan_status wlan_11n_deaggregate_pkt(mlan_private *priv, pmlan_buffer pmbuf);
/** Deaggregate 11N packets */
int wlan_11n_aggregate_pkt(mlan_private *priv, raListTbl *ptr, int headroom, int ptrindex);
#if CONFIG_AMSDU_IN_AMPDU
/** Form A-MSDU packets */
int wlan_11n_form_amsdu_pkt(t_u8 *amsdu_buf, t_u8 *data, int pkt_len, int *pad);
#endif
#endif /* !_MLAN_11N_AGGR_H_ */
