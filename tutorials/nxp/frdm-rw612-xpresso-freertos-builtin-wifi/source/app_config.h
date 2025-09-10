/*
 *  Copyright 2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define RW610
#define FRDMRW610
#define WIFI_BT_USE_IMU_INTERFACE

#if CONFIG_MONOLITHIC_WIFI
#define CONFIG_SOC_SERIES_RW6XX_REVISION_A2 1
#endif


#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#define CONFIG_BT_SNOOP 1