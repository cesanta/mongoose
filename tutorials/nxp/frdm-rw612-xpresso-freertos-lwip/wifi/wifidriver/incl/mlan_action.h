/** @file mlan_action.h
 *
 *  @brief Interface for the mlan_action module implemented in mlan_action.c
 *
 *  Driver interface functions and type declarations for the process action frame
 *    module implemented in mlan_action.c.
 *
 *  Copyright 2022 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/********************************************************
Change log:
    08/11/2022: initial version
********************************************************/

#ifndef _MLAN_ACTION_H_
#define _MLAN_ACTION_H_

#include "mlan_fw.h"

/** process rx action frame */
mlan_status wlan_process_mgmt_action(t_u8 *payload, t_u32 payload_len, RxPD *rxpd);

#endif /* !_MLAN_ACTION_H_ */
