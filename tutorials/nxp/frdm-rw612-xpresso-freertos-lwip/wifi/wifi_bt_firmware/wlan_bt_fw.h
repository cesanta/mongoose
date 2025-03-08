/*
 *  Copyright 2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __WLAN_BT_FW_H__
#define __WLAN_BT_FW_H__

#include <osa.h>

#if defined(SD8801)
#include "sd8801_wlan.h"
#elif defined(SD8978)
#if !(CONFIG_WIFI_IND_DNLD) && !defined(CONFIG_BT_IND_DNLD)
#include "sduartIW416_wlan_bt.h"
#else
#include "sdIW416_wlan.h"
#include "uartIW416_bt.h"
#endif
#elif defined(SD8987)
#if !(CONFIG_WIFI_IND_DNLD) && !defined(CONFIG_BT_IND_DNLD)
#include "sduart8987_wlan_bt.h"
#else
#include "sd8987_wlan.h"
#include "uart8987_bt.h"
#endif
#elif defined(SD9177) || defined(IW610)
#if !(CONFIG_WIFI_IND_DNLD) && !defined(CONFIG_BT_IND_DNLD)
#include "sduart_nw61x_se.h"
#else
#include "sd_nw61x_se.h"
#include "uart_nw61x_se.h"
#endif
#elif defined(RW610)
extern const unsigned char *wlan_fw_bin;
extern const unsigned int wlan_fw_bin_len;
#else
#error "Please define Wi-Fi Module"
#endif

#endif /* __WLAN_BT_FW_H__ */
