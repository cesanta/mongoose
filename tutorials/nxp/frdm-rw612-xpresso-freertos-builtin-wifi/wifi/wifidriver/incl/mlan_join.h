/** @file mlan_join.h
 *
 *  @brief This file defines the interface for the WLAN infrastructure
 *  and adhoc join routines.
 *
 *  Driver interface functions and type declarations for the join module
 *  implemented in mlan_join.c.  Process all start/join requests for
 *  both adhoc and infrastructure networks
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    10/13/2008: initial version
******************************************************/

#ifndef _MLAN_JOIN_H_
#define _MLAN_JOIN_H_

/** Size of buffer allocated to store the association response from firmware */
#define MRVDRV_ASSOC_RSP_BUF_SIZE 500

/** Size of buffer allocated to store IEs passed to firmware in the assoc req */
#define MRVDRV_GENIE_BUF_SIZE 256

#endif /* _MLAN_JOIN_H_ */
