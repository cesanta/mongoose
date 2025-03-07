/** @file mlan.h
 *
 *  @brief This file declares all APIs that will be called from MOAL module.
 *  It also defines the data structures used for APIs between MLAN and MOAL.
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

/******************************************************
Change log:
    10/13/2008: initial version
    11/07/2008: split mlan.h into mlan_decl.h & mlan_ioctl.h
******************************************************/

#ifndef _MLAN_H_
#define _MLAN_H_

#include <wifi_config_default.h>

#ifdef __ZEPHYR__
#include "nxp_wifi.h"
#endif



#ifndef __ZEPHYR__
#if !CONFIG_STA_AUTO_DHCPV4
#define CONFIG_STA_AUTO_DHCPV4 1
#endif
#endif

#ifndef __ZEPHYR__
#if !CONFIG_WIFI_STA_RECONNECT
#define CONFIG_WIFI_STA_RECONNECT 1
#endif
#endif

#define CONFIG_GTK_REKEY_OFFLOAD 0


#if defined(SD9177) || defined(IW610)
#define CONFIG_TCP_ACK_ENH 1
#define CONFIG_FW_VDLL     1
#if !CONFIG_WIFI_CAPA
#define CONFIG_WIFI_CAPA 1
#endif

#if CONFIG_11AX
#if !CONFIG_11K
#define CONFIG_11K 1
#endif
#if !CONFIG_11V
#define CONFIG_11V 1
#endif
#endif
#endif

#include <osa.h>

#include "mlan_decl.h"
#include "mlan_ioctl.h"
#include "mlan_ieee.h"

#endif /* !_MLAN_H_ */
