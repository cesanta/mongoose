/*
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
/*!\file wifi_config_default.h
 *\brief This file provides default macros for Wi-Fi.
 */

#if !defined WIFI_HDR_CONFIG_H
#define WIFI_HDR_CONFIG_H

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you don't like!
 */

#include "wifi_config.h"

#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)

/*
 * #define OVERRIDE_CALIBRATION_DATA "wifi_cal_data_rw61x_override.h"
 * if use the specific calibration data
 */
#if !defined(OVERRIDE_CALIBRATION_DATA)
#if defined(RW610)
/*
 * FRDMRW610
 */
#if defined(FRDMRW610)
/* FRDMRW610 1ANT */
#define DEFAULT_CALDATA_RW610 "wifi_cal_data_frdmrw61x_1ant.h"
#else
/*
 * RW610
 */
/* RW610 1ANT */
#if CONFIG_WLAN_CALDATA_1ANT
#define DEFAULT_CALDATA_RW610 "wifi_cal_data_rw61x_1ant.h"
/* RW610 3ANT_DIVERSITY */
#elif CONFIG_WLAN_CALDATA_3ANT_DIVERSITY
#define DEFAULT_CALDATA_RW610 "wifi_cal_data_rw61x_3ant_diversity.h"
/* RW610 1ANT_DIVERSITY */
#elif CONFIG_WLAN_CALDATA_1ANT_WITH_DIVERSITY
#define DEFAULT_CALDATA_RW610 "wifi_cal_data_rw61x_1ant_diversity.h"
/* RW610 2ANT */
#else
#define DEFAULT_CALDATA_RW610 "wifi_cal_data_rw61x_2ant.h"
#endif
#endif
#endif /* RW610 */
#endif /* OVERRIDE_CALIBRATION_DATA */

#if !defined CONFIG_WIFI_AUTO_POWER_SAVE
#define CONFIG_WIFI_AUTO_POWER_SAVE 1
#endif

#if !defined CONFIG_WIFI_MAX_PRIO
#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 1)
#endif

#if !defined CONFIG_MAX_AP_ENTRIES
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_MAX_AP_ENTRIES 10
#else
#define CONFIG_MAX_AP_ENTRIES 30
#endif
#endif

#if !defined CONFIG_5GHz_SUPPORT
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_5GHz_SUPPORT 1
#endif
#endif

#if defined(SD8801)
#undef CONFIG_5GHz_SUPPORT
#define CONFIG_5GHz_SUPPORT 0
#endif

#if !defined CONFIG_FW_DNLD_ASYNC
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_FW_DNLD_ASYNC 0
#endif
#endif

#if !defined CONFIG_11AC
#if defined(RW610) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_11AC 1
#endif
#endif

#if CONFIG_11AC
#if defined(SD8801) || defined(SD8978)
#undef CONFIG_11AC
#define CONFIG_11AC 0
#endif
#endif

#if !defined CONFIG_11AX
#if defined(RW610) || defined(SD9177) || defined(IW610)
#define CONFIG_11AX CONFIG_11AC
#endif
#endif

#if !CONFIG_11AC
#undef CONFIG_11AX
#define CONFIG_11AX 0
#endif

#if !defined CONFIG_11AX_TWT
#if defined(RW610) || defined(SD9177) || defined(IW610)
#define CONFIG_11AX_TWT CONFIG_11AX
#endif
#endif

#if !CONFIG_11AX
#undef CONFIG_11AX_TWT
#define CONFIG_11AX_TWT 0
#endif

/* WMM options */
#if !defined CONFIG_WMM
#if defined(RW610)
#define CONFIG_WMM 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WMM 0
#endif
#endif

#if !defined CONFIG_SDIO_MULTI_PORT_RX_AGGR
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_SDIO_MULTI_PORT_RX_AGGR 1
#endif
#endif

#if CONFIG_SDIO_MULTI_PORT_RX_AGGR
#if defined(RW610)
#undef CONFIG_SDIO_MULTI_PORT_RX_AGGR
#define CONFIG_SDIO_MULTI_PORT_RX_AGGR 0
#endif
#endif

#if !defined CONFIG_SDIO_MULTI_PORT_TX_AGGR
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_SDIO_MULTI_PORT_TX_AGGR CONFIG_WMM
#endif
#endif

#if defined(RW610)
#undef CONFIG_SDIO_MULTI_PORT_TX_AGGR
#define CONFIG_SDIO_MULTI_PORT_TX_AGGR 0
#endif

#if !CONFIG_WMM
#undef CONFIG_SDIO_MULTI_PORT_TX_AGGR
#define CONFIG_SDIO_MULTI_PORT_TX_AGGR 0
#endif

/** Multi port aggregation packet limit */
#if !defined CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT (4 + (CONFIG_WMM * 4))
#endif
#endif

#if CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT > 8
#undef CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT
#if CONFIG_WMM && CONFIG_SDIO_MULTI_PORT_TX_AGGR
#define CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT 8
#else
#define CONFIG_SDIO_MP_AGGR_DEF_PKT_LIMIT 4
#endif
#endif

#if !defined CONFIG_RF_TEST_MODE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_RF_TEST_MODE 0
#endif
#endif

#if !defined CONFIG_UNII4_BAND_SUPPORT
#if defined(RW610)
#define CONFIG_UNII4_BAND_SUPPORT 1
#elif defined(SD8978) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_UNII4_BAND_SUPPORT 0
#endif
#endif

#if !CONFIG_5GHz_SUPPORT
#undef CONFIG_UNII4_BAND_SUPPORT
#define CONFIG_UNII4_BAND_SUPPORT 0
#endif

#if !defined CONFIG_COMPRESS_TX_PWTBL
#if defined(RW610) || defined(SD9177) || defined(IW610)
#define CONFIG_COMPRESS_TX_PWTBL 1
#endif
#endif

#if CONFIG_COMPRESS_TX_PWTBL
#if defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_COMPRESS_TX_PWTBL
#define CONFIG_COMPRESS_TX_PWTBL 0
#endif
#endif

#if !defined CONFIG_COMPRESS_RU_TX_PWTBL
#if defined(SD9177) || defined(RW610) || defined(IW610)
#define CONFIG_COMPRESS_RU_TX_PWTBL 1
#endif
#endif

#if CONFIG_COMPRESS_RU_TX_PWTBL
#if defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_COMPRESS_RU_TX_PWTBL
#define CONFIG_COMPRESS_RU_TX_PWTBL 0
#endif
#endif

#if !defined CONFIG_WIFI_FEATURES
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_FEATURES 0
#endif
#endif

#if CONFIG_WIFI_FEATURES
#if defined(SD8801)
#undef CONFIG_WIFI_FEATURES
#define CONFIG_WIFI_FEATURES 0
#endif
#endif

#if !defined CONFIG_OFFLOAD
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_OFFLOAD CONFIG_WIFI_FEATURES
#endif
#endif

#if !CONFIG_WIFI_FEATURES
#undef CONFIG_OFFLOAD
#define CONFIG_OFFLOAD 0
#endif

#if !defined CONFIG_RW610_A1
#if defined(RW610)
#define CONFIG_RW610_A1 1
#endif
#endif

#if !defined PRINTF_FLOAT_ENABLE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define PRINTF_FLOAT_ENABLE 0
#endif
#endif

#if !defined CONFIG_IMU_GDMA
#if defined(RW610)
#define CONFIG_IMU_GDMA 0
#endif
#endif

#if !defined CONFIG_AMSDU_IN_AMPDU
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_AMSDU_IN_AMPDU 0
#endif
#endif

#if !defined CONFIG_SCAN_WITH_RSSIFILTER
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_SCAN_WITH_RSSIFILTER 1
#endif
#endif

/* WLAN white/black list opt */
#if !defined CONFIG_UAP_STA_MAC_ADDR_FILTER
#if defined(RW610) && CONFIG_HOSTAPD
#define CONFIG_UAP_STA_MAC_ADDR_FILTER 1
#else
#define CONFIG_UAP_STA_MAC_ADDR_FILTER 0
#endif
#endif

#if !defined CONFIG_WIFI_DTIM_PERIOD
#if defined(RW610)
#define CONFIG_WIFI_DTIM_PERIOD 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_DTIM_PERIOD 0
#endif
#endif

#if !defined CONFIG_UART_INTERRUPT
#if defined(RW610)
#define CONFIG_UART_INTERRUPT 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_UART_INTERRUPT 0
#endif
#endif

#if !defined CONFIG_WIFI_MAX_CLIENTS_CNT
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_MAX_CLIENTS_CNT 1
#endif
#endif

#if !defined CONFIG_WIFI_FRAG_THRESHOLD
#if defined(RW610)
#define CONFIG_WIFI_FRAG_THRESHOLD 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_FRAG_THRESHOLD 0
#endif
#endif

#if !defined CONFIG_WMM_UAPSD
#if defined(RW610)
#define CONFIG_WMM_UAPSD 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WMM_UAPSD 0
#endif
#endif

#if !defined CONFIG_WIFI_GET_LOG
#if defined(RW610)
#define CONFIG_WIFI_GET_LOG 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_GET_LOG 0
#endif
#endif

#if !defined CONFIG_WIFI_TX_PER_TRACK
#if defined(RW610)
#define CONFIG_WIFI_TX_PER_TRACK 0
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_TX_PER_TRACK 0
#endif
#endif

#if CONFIG_WIFI_TX_PER_TRACK
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#undef CONFIG_WIFI_TX_PER_TRACK
#define CONFIG_WIFI_TX_PER_TRACK 0
#endif
#endif

#if !defined CONFIG_POWER_MANAGER
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_POWER_MANAGER 0
#endif
#endif

#if CONFIG_POWER_MANAGER
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#undef CONFIG_POWER_MANAGER
#define CONFIG_POWER_MANAGER 0
#endif
#endif

#if !defined CONFIG_CSI
#if defined(RW610)
#define CONFIG_CSI 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_CSI 0
#endif
#endif

#if CONFIG_CSI
#if defined(SD8801)
#undef CONFIG_CSI
#define CONFIG_CSI 0
#endif
#endif

#if !defined CONFIG_WIFI_RESET
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_RESET 1
#endif
#endif

#if !defined CONFIG_NET_MONITOR
#if defined(RW610)
#define CONFIG_NET_MONITOR 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_NET_MONITOR 0
#endif
#endif

#if CONFIG_NET_MONITOR
#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#undef CONFIG_NET_MONITOR
#define CONFIG_NET_MONITOR 0
#endif
#endif

#if !defined CONFIG_WIFI_MEM_ACCESS
#if defined(RW610)
#define CONFIG_WIFI_MEM_ACCESS 0
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_MEM_ACCESS 0
#endif
#endif

#if CONFIG_WIFI_MEM_ACCESS
#if defined(SD8801)
#undef CONFIG_WIFI_MEM_ACCESS
#define CONFIG_WIFI_MEM_ACCESS 0
#endif
#endif

#if !defined CONFIG_WIFI_REG_ACCESS
#if defined(RW610)
#define CONFIG_WIFI_REG_ACCESS 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_REG_ACCESS 0
#endif
#endif

#if CONFIG_WIFI_REG_ACCESS
#if defined(SD8801)
#undef CONFIG_WIFI_REG_ACCESS
#define CONFIG_WIFI_REG_ACCESS 0
#endif
#endif

#if !defined CONFIG_ECSA
#if defined(RW610)
#define CONFIG_ECSA 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_ECSA 0
#endif
#endif

#if !defined CONFIG_RX_ABORT_CFG
#if defined(RW610)
#define CONFIG_RX_ABORT_CFG 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_RX_ABORT_CFG 0
#endif
#endif

#if !defined CONFIG_RX_ABORT_CFG_EXT
#if defined(RW610)
#define CONFIG_RX_ABORT_CFG_EXT 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_RX_ABORT_CFG_EXT 0
#endif
#endif

#if !defined CONFIG_CCK_DESENSE_CFG
#if defined(RW610)
#define CONFIG_CCK_DESENSE_CFG 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_CCK_DESENSE_CFG 0
#endif
#endif

#if !defined CONFIG_IPS
#if defined(RW610)
#define CONFIG_IPS 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_IPS 0
#endif
#endif

#if !defined CONFIG_SUBSCRIBE_EVENT_SUPPORT
#if defined(RW610)
#define CONFIG_SUBSCRIBE_EVENT_SUPPORT 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_SUBSCRIBE_EVENT_SUPPORT 0
#endif
#endif

#if !defined CONFIG_WIFI_FORCE_RTS
#if defined(RW610)
#define CONFIG_WIFI_FORCE_RTS 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_FORCE_RTS 0
#endif
#endif

#if !defined CONFIG_TX_AMPDU_PROT_MODE
#if defined(RW610)
#define CONFIG_TX_AMPDU_PROT_MODE 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_TX_AMPDU_PROT_MODE 0
#endif
#endif

#if !defined CONFIG_TSP
#if defined(RW610)
#define CONFIG_TSP 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_TSP 0
#endif
#endif

#if !defined CONFIG_TX_RX_HISTOGRAM
#if defined(RW610)
#define CONFIG_TX_RX_HISTOGRAM 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_TX_RX_HISTOGRAM 0
#endif
#endif

#if !defined CONFIG_IPV6
#define CONFIG_IPV6 0
#endif

#if !defined CONFIG_MAX_IPV6_ADDRESSES
#define CONFIG_MAX_IPV6_ADDRESSES 0
#endif

#if !defined CONFIG_WIFI_CAPA
#if defined(RW610)
#define CONFIG_WIFI_CAPA 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_CAPA 0
#endif
#endif

#if !defined CONFIG_ROAMING
#if defined(RW610)
#define CONFIG_ROAMING 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_ROAMING 0
#endif
#endif

#if !defined CONFIG_CLOUD_KEEP_ALIVE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_CLOUD_KEEP_ALIVE 0
#endif
#endif

#if CONFIG_CLOUD_KEEP_ALIVE
#if defined(SD8801)
#undef CONFIG_CLOUD_KEEP_ALIVE
#define CONFIG_CLOUD_KEEP_ALIVE 0
#endif
#endif

#if !defined CONFIG_TURBO_MODE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_TURBO_MODE CONFIG_WMM
#endif
#endif

#if CONFIG_TURBO_MODE
#undef CONFIG_TURBO_MODE
#if defined(RW610)
#define CONFIG_TURBO_MODE 0
#else
#define CONFIG_TURBO_MODE CONFIG_WMM
#endif
#endif

#if !defined CONFIG_AUTO_RECONNECT
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_AUTO_RECONNECT 0
#endif
#endif

#if CONFIG_AUTO_RECONNECT
#if defined(SD8801)
#undef CONFIG_AUTO_RECONNECT
#define CONFIG_AUTO_RECONNECT 0
#endif
#endif

#if !defined CONFIG_EXT_SCAN_SUPPORT
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_EXT_SCAN_SUPPORT 1
#endif
#endif

#if CONFIG_EXT_SCAN_SUPPORT
#if defined(SD8801)
#undef CONFIG_EXT_SCAN_SUPPORT
#define CONFIG_EXT_SCAN_SUPPORT 0
#endif
#endif

#if !defined CONFIG_WIFI_EU_CRYPTO
#if defined(RW610)
#define CONFIG_WIFI_EU_CRYPTO 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_EU_CRYPTO 0
#endif
#endif

#if CONFIG_WIFI_EU_CRYPTO
#if defined(SD8801)
#undef CONFIG_WIFI_EU_CRYPTO
#define CONFIG_WIFI_EU_CRYPTO 0
#endif
#endif

#if !defined CONFIG_WIFI_IND_DNLD
#if defined(SD8978) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_IND_DNLD 0
#endif
#endif

#if CONFIG_WIFI_IND_DNLD
#if defined(RW610) || defined(SD8801)
#undef CONFIG_WIFI_IND_DNLD
#define CONFIG_WIFI_IND_DNLD 0
#endif
#endif

#if !defined CONFIG_WIFI_IND_RESET
#if defined(SD8978) || defined(SD8987) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_IND_RESET 0
#endif
#endif

#if CONFIG_WIFI_IND_RESET
#if defined(RW610) || defined(SD8801)
#undef CONFIG_WIFI_IND_RESET
#define CONFIG_WIFI_IND_RESET 0
#endif
#endif

#if !defined CONFIG_HOST_SLEEP
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_HOST_SLEEP 0
#endif
#endif

#if !defined CONFIG_DRIVER_FIPS
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_DRIVER_FIPS 0
#endif
#endif

#if CONFIG_DRIVER_FIPS
#define CONFIG_FIPS
#endif

#if !CONFIG_DRIVER_FIPS
#undef CONFIG_FIPS
#endif

#if !defined CONFIG_11K
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_11K 0
#endif
#endif

#if CONFIG_11AX
#undef CONFIG_11K
#define CONFIG_11K CONFIG_11AX
#endif

#if !defined CONFIG_11V
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_11V 0
#endif
#endif

#if CONFIG_11AX
#undef CONFIG_11V
#define CONFIG_11V CONFIG_11AX
#endif

#if !defined CONFIG_TCP_ACK_ENH
#if defined(SD9177) || defined(IW610)
#define CONFIG_TCP_ACK_ENH 1
#endif
#endif

#if CONFIG_TCP_ACK_ENH
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_TCP_ACK_ENH
#define CONFIG_TCP_ACK_ENH
#endif
#endif

#if !defined CONFIG_FW_VDLL
#if defined(SD9177) || defined(IW610)
#define CONFIG_FW_VDLL 1
#endif
#endif

#if !CONFIG_FW_VDLL
#if defined(SD9177) || defined(IW610)
#undef CONFIG_FW_VDLL
#define CONFIG_FW_VDLL 1
#endif
#endif

/*
 * Config options for wpa supplicant
 */
#if !defined CONFIG_WPA_SUPP
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP 0
#endif
#endif

#if !defined CONFIG_DRIVER_MBO
#if defined(RW610) || defined(SD9177) || defined(IW610)
#define CONFIG_DRIVER_MBO (CONFIG_11AX && !CONFIG_WPA_SUPP)
#endif
#endif

#if CONFIG_DRIVER_MBO
#if !CONFIG_11AX || CONFIG_WPA_SUPP
#undef CONFIG_DRIVER_MBO
#define CONFIG_DRIVER_MBO 0
#endif
#endif

#if CONFIG_DRIVER_MBO
#undef CONFIG_MBO
#endif

#if !CONFIG_DRIVER_MBO
#define CONFIG_MBO
#endif

#if !defined CONFIG_DRIVER_OWE
#if defined(RW610)
#define CONFIG_DRIVER_OWE 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_DRIVER_OWE 0
#endif
#endif

#if (CONFIG_DRIVER_OWE && CONFIG_WPA_SUPP)
#define CONFIG_OWE
#endif

#if (!CONFIG_DRIVER_OWE && CONFIG_WPA_SUPP)
#undef CONFIG_OWE
#endif

#if !defined CONFIG_11R
#if defined(RW610)
#define CONFIG_11R 1
#elif defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_11R 0
#endif
#endif

#if CONFIG_11R
#if defined(RW610) || defined(SD8801) || defined(SD9177) || defined(IW610)
#undef CONFIG_11R
#define CONFIG_11R CONFIG_WPA_SUPP
#endif
#endif

#if !defined CONFIG_WPA_SUPP_WPS
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_WPS 0
#endif
#endif

#if CONFIG_WPA_SUPP_WPS
#undef CONFIG_WPA_SUPP_WPS
#define CONFIG_WPA_SUPP_WPS CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_WPA_SUPP_WPA3
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_WPA3 CONFIG_WPA_SUPP
#endif
#endif

#if CONFIG_WPA_SUPP_WPA3
#undef CONFIG_WPA_SUPP_WPA3
#define CONFIG_WPA_SUPP_WPA3 CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_WPA_SUPP_DPP
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_DPP 0
#endif
#endif

#if CONFIG_WPA_SUPP_DPP
#if defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_WPA_SUPP_DPP
#define CONFIG_WPA_SUPP_DPP 0
#endif
#endif

#if !defined CONFIG_WPA_SUPP_DPP2
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_DPP2 0
#endif
#endif

#if CONFIG_WPA_SUPP_DPP2
#if defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_WPA_SUPP_DPP2
#define CONFIG_WPA_SUPP_DPP2 CONFIG_WPA_SUPP_DPP
#endif
#endif

#if !defined CONFIG_WPA_SUPP_DPP3
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_DPP3 0
#endif
#endif

#if CONFIG_WPA_SUPP_DPP3
#if defined(SD8978) || defined(SD8987) || defined(SD8801)
#undef CONFIG_WPA_SUPP_DPP3
#define CONFIG_WPA_SUPP_DPP3 (CONFIG_WPA_SUPP_DPP && CONFIG_WPA_SUPP_DPP2)
#endif
#endif

#if !defined CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE 0
#endif
#endif

#if CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#undef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#define CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE 0
#endif
#endif

#if CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE
#undef CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE
#define CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE && CONFIG_HOSTAPD)
#endif

#if !defined CONFIG_EAP_TLS
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_TLS 0
#endif
#endif

#if !CONFIG_WPA2_ENTP
#if CONFIG_EAP_TLS
#undef CONFIG_EAP_TLS
#define CONFIG_EAP_TLS (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif
#endif

#if !defined CONFIG_EAP_PEAP
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_PEAP 0
#endif
#endif

#if CONFIG_EAP_PEAP
#undef CONFIG_EAP_PEAP
#define CONFIG_EAP_PEAP (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif

#if !defined CONFIG_EAP_TTLS
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_TTLS 0
#endif
#endif

#if CONFIG_EAP_TTLS
#undef CONFIG_EAP_TTLS
#define CONFIG_EAP_TTLS (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif

#if !defined CONFIG_EAP_FAST
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_FAST 0
#endif
#endif

#if CONFIG_EAP_FAST
#undef CONFIG_EAP_FAST
#define CONFIG_EAP_FAST (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif

#if !defined CONFIG_EAP_SIM
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_SIM 0
#endif
#endif

#if CONFIG_EAP_SIM
#undef CONFIG_EAP_SIM
#define CONFIG_EAP_SIM (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif

#if !defined CONFIG_EAP_AKA
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_AKA 0
#endif
#endif

#if CONFIG_EAP_AKA
#undef CONFIG_EAP_AKA
#define CONFIG_EAP_AKA (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE)
#endif

#if !defined CONFIG_EAP_AKA_PRIME
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_AKA_PRIME 0
#endif
#endif

#if CONFIG_EAP_AKA_PRIME
#undef CONFIG_EAP_AKA_PRIME
#define CONFIG_EAP_AKA_PRIME (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE && CONFIG_EAP_AKA)
#endif

#if !defined CONFIG_EAP_MSCHAPV2
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_MSCHAPV2 (CONFIG_EAP_PEAP || CONFIG_EAP_TTLS || CONFIG_EAP_FAST)
#endif
#endif

#if CONFIG_EAP_MSCHAPV2
#undef CONFIG_EAP_MSCHAPV2
#define CONFIG_EAP_MSCHAPV2 \
    (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE && (CONFIG_EAP_PEAP || CONFIG_EAP_TTLS || CONFIG_EAP_FAST))
#endif

#if !defined CONFIG_EAP_GTC
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_EAP_GTC (CONFIG_EAP_PEAP || CONFIG_EAP_TTLS || CONFIG_EAP_FAST)
#endif
#endif

#if CONFIG_EAP_GTC
#undef CONFIG_EAP_GTC
#define CONFIG_EAP_GTC \
    (CONFIG_WPA_SUPP && CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE && (CONFIG_EAP_PEAP || CONFIG_EAP_TTLS || CONFIG_EAP_FAST))
#endif

#if !defined CONFIG_WPS2
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPS2 0
#endif
#endif

#if CONFIG_WPS2
#undef CONFIG_WPS2
#define CONFIG_WPS2 !CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_WPA2_ENTP
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WPA2_ENTP 0
#endif
#endif

#if CONFIG_WPA2_ENTP
#undef CONFIG_WPA2_ENTP
#define CONFIG_WPA2_ENTP !CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_PEAP_MSCHAPV2
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_PEAP_MSCHAPV2 0
#endif
#endif

#if CONFIG_PEAP_MSCHAPV2
#undef CONFIG_PEAP_MSCHAPV2
#define CONFIG_PEAP_MSCHAPV2 !CONFIG_WPA_SUPP
#endif

#if !defined CONFIG_WIFI_USB_FILE_ACCESS
#if defined(RW610) || defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177) || defined(IW610)
#define CONFIG_WIFI_USB_FILE_ACCESS 0
#endif
#endif

#if !defined CONFIG_MMSF
#if defined(RW610)
#define CONFIG_MMSF 1
#endif
#endif

#if !defined CONFIG_COEX_DUTY_CYCLE
#if defined(RW610)
#define CONFIG_COEX_DUTY_CYCLE 1
#endif
#endif

/** If define CONFIG_TX_RX_ZERO_COPY 1, please make sure
 *  #define PBUF_POOL_BUFSIZE 1752
 *  in lwipopts.h
 */
#if !defined CONFIG_TX_RX_ZERO_COPY
#if defined(RW610)
#define CONFIG_TX_RX_ZERO_COPY 0
#endif
#endif

#if !defined CONFIG_WIFI_CLOCKSYNC
#if defined(RW610)
#define CONFIG_WIFI_CLOCKSYNC 1
#endif
#endif

#if !defined CONFIG_INACTIVITY_TIMEOUT_EXT
#if defined(RW610)
#define CONFIG_INACTIVITY_TIMEOUT_EXT 1
#endif
#endif

#if !defined CONFIG_AUTO_NULL_TX
#if defined(RW610)
#define CONFIG_AUTO_NULL_TX 1
#endif
#endif

#if !defined CONFIG_EXTERNAL_COEX_PTA
#if defined(RW610)
#define CONFIG_EXTERNAL_COEX_PTA 1
#endif
#endif

#if !defined CONFIG_FW_VDLLV2
#if defined(RW610)
#define CONFIG_FW_VDLLV2 1
#endif
#endif

/** Wi-Fi NXP internal macros */
#define LWIPERF_REVERSE_MODE       1
#define CONFIG_MLAN_WMSDK          1
#define CONFIG_11N                 1
#define STA_SUPPORT                1
#define UAP_SUPPORT                1
#define WPA                        1
#define KEY_MATERIAL_WEP           1
#define KEY_PARAM_SET_V2           1
#define ENABLE_802_11W             1
#define ENABLE_GCMP_SUPPORT        1
#define CONFIG_STA_AMPDU_RX        1
#define CONFIG_STA_AMPDU_TX        1
#define CONFIG_ENABLE_AMSDU_RX     1
#define CONFIG_UAP_AMPDU_TX        1
#define CONFIG_UAP_AMPDU_RX        1
#define CONFIG_WIFIDRIVER_PS_LOCK  1
#define CONFIG_WNM_PS              1
#define CONFIG_SCAN_CHANNEL_GAP    1
#define CONFIG_COMBO_SCAN          1
#define CONFIG_BG_SCAN             1
#define CONFIG_HOST_MLME           1
#define UAP_HOST_MLME              1
#define CONFIG_WIFI_RTS_THRESHOLD  1
#define CONFIG_MEF_CFG             1
#define CONFIG_MULTI_BSSID_SUPPORT 1
#define CONFIG_SET_SU              1
#define CONFIG_RX_CHAN_INFO        1
#define CONFIG_TXPD_RXPD_V3        1

#if CONFIG_WNM_PS
#if defined(RW610)
#undef CONFIG_WNM_PS
#define CONFIG_WNM_PS 0
#endif
#endif

#if !defined CONFIG_SEND_HOSTCMD
#define CONFIG_SEND_HOSTCMD 1
#endif

/* Logs */
#if !defined CONFIG_ENABLE_ERROR_LOGS
#define CONFIG_ENABLE_ERROR_LOGS 1
#endif

#if !defined CONFIG_ENABLE_WARNING_LOGS
#define CONFIG_ENABLE_WARNING_LOGS 1
#endif

/* WLCMGR debug */
#if !defined CONFIG_WLCMGR_DEBUG
#define CONFIG_WLCMGR_DEBUG 0
#endif

/*
 * Wifi extra debug options
 */
#if !defined CONFIG_WIFI_EXTRA_DEBUG
#define CONFIG_WIFI_EXTRA_DEBUG 0
#endif

#if !defined CONFIG_WIFI_EVENTS_DEBUG
#define CONFIG_WIFI_EVENTS_DEBUG 0
#endif

#if !defined CONFIG_WIFI_CMD_RESP_DEBUG
#define CONFIG_WIFI_CMD_RESP_DEBUG 0
#endif

#if !defined CONFIG_WIFI_PKT_DEBUG
#define CONFIG_WIFI_PKT_DEBUG 0
#endif

#if !defined CONFIG_WIFI_SCAN_DEBUG
#define CONFIG_WIFI_SCAN_DEBUG 0
#endif

#if !defined CONFIG_WIFI_IO_INFO_DUMP
#define CONFIG_WIFI_IO_INFO_DUMP 0
#endif

#if !defined CONFIG_WIFI_IO_DEBUG
#define CONFIG_WIFI_IO_DEBUG 0
#endif

#if !defined CONFIG_WIFI_IO_DUMP
#define CONFIG_WIFI_IO_DUMP 0
#endif

#if !defined CONFIG_WIFI_MEM_DEBUG
#define CONFIG_WIFI_MEM_DEBUG 0
#endif

#if !defined CONFIG_WIFI_AMPDU_DEBUG
#define CONFIG_WIFI_AMPDU_DEBUG 0
#endif

#if !defined CONFIG_WIFI_TIMER_DEBUG
#define CONFIG_WIFI_TIMER_DEBUG 0
#endif

#if !defined CONFIG_WIFI_SDIO_DEBUG
#define CONFIG_WIFI_SDIO_DEBUG 0
#endif

#if !defined CONFIG_WIFI_FW_DEBUG
#define CONFIG_WIFI_FW_DEBUG 0
#endif

#if !defined CONFIG_WIFI_UAP_DEBUG
#define CONFIG_WIFI_UAP_DEBUG 0
#endif

#if !defined CONFIG_WPS_DEBUG
#define CONFIG_WPS_DEBUG 0
#endif

#if !defined CONFIG_FW_VDLL_DEBUG
#define CONFIG_FW_VDLL_DEBUG 0
#endif

#if !defined CONFIG_DHCP_SERVER_DEBUG
#define CONFIG_DHCP_SERVER_DEBUG 0
#endif

#if !defined CONFIG_WIFI_SDIO_DEBUG
#define CONFIG_WIFI_SDIO_DEBUG 0
#endif

#if !defined CONFIG_FWDNLD_IO_DEBUG
#define CONFIG_FWDNLD_IO_DEBUG 0
#endif

#if !defined CONFIG_WIFI_HTC_DEBUG
#define CONFIG_WIFI_HTC_DEBUG 0
#endif

/*
 * Heap debug options
 */
#if !defined CONFIG_HEAP_DEBUG
#define CONFIG_HEAP_DEBUG 0
#endif

#if !defined CONFIG_HEAP_STAT
#define CONFIG_HEAP_STAT 0
#endif

/*
 * wpa supplicant debug options
 */
#if !defined CONFIG_WPA_SUPP_DEBUG_LEVEL
#define CONFIG_WPA_SUPP_DEBUG_LEVEL 6
#endif

#if !defined CONFIG_SUPP_DEBUG
#define CONFIG_SUPP_DEBUG 0
#endif

/*
 * Wi-Fi AP feature options adjust
 */
#if !CONFIG_NXP_WIFI_SOFTAP_SUPPORT
#if UAP_SUPPORT
#undef UAP_SUPPORT
#define UAP_SUPPORT 0
#endif

#if UAP_HOST_MLME
#undef UAP_HOST_MLME
#define UAP_HOST_MLME 0
#endif

#if CONFIG_UAP_AMPDU_TX
#undef CONFIG_UAP_AMPDU_TX
#define CONFIG_UAP_AMPDU_TX 0
#endif

#if CONFIG_UAP_AMPDU_RX
#undef CONFIG_UAP_AMPDU_RX
#define CONFIG_UAP_AMPDU_RX 0
#endif

#if !CONFIG_WIFI_SLIM_UAP
#undef CONFIG_WIFI_SLIM_UAP
#define CONFIG_WIFI_SLIM_UAP 1
#endif

#if CONFIG_WIFI_CAPA
#undef CONFIG_WIFI_CAPA
#define CONFIG_WIFI_CAPA 0
#endif

#if CONFIG_ECSA
#undef CONFIG_ECSA
#define CONFIG_ECSA 0
#endif
#endif /* !CONFIG_WIFI_NXP_AP */

#if !CONFIG_WIFI_PKT_FWD
#if UAP_SUPPORT
#if defined(RW610)
#define CONFIG_WIFI_PKT_FWD 1
#else
#define CONFIG_WIFI_PKT_FWD 0
#endif
#endif
#endif

/*
 * Wi-Fi SLIM feature options
 */

#ifndef CONFIG_WIFI_SLIM_ROAM
#define CONFIG_WIFI_SLIM_ROAM 0
#endif

#ifndef CONFIG_WIFI_SLIM_STA
#define CONFIG_WIFI_SLIM_STA 0
#endif

#ifndef CONFIG_WIFI_SLIM_UAP
#define CONFIG_WIFI_SLIM_UAP 0
#endif

#ifndef CONFIG_WIFI_SLIM_DISABLE_DBG
#define CONFIG_WIFI_SLIM_DISABLE_DBG 0
#endif

#if CONFIG_WIFI_SLIM_ROAM

#if CONFIG_ROAMING
#undef CONFIG_ROAMING
#define CONFIG_ROAMING 0
#endif

#if CONFIG_11K
#undef CONFIG_11K
#define CONFIG_11K 0
#endif

#if CONFIG_11V
#undef CONFIG_11V
#define CONFIG_11V 0
#endif

#if CONFIG_11R
#undef CONFIG_11R
#define CONFIG_11R 0
#endif

#endif /* CONFIG_WIFI_SLIM_ROAM */

#if CONFIG_WIFI_SLIM_STA

#if defined(RW610)
#if CONFIG_5GHz_SUPPORT
#undef CONFIG_5GHz_SUPPORT
#define CONFIG_5GHz_SUPPORT 0
#endif
#endif

#if CONFIG_CLOUD_KEEP_ALIVE
#undef CONFIG_CLOUD_KEEP_ALIVE
#define CONFIG_CLOUD_KEEP_ALIVE 0
#endif

#if CONFIG_WIFI_EU_CRYPTO
#undef CONFIG_WIFI_EU_CRYPTO
#define CONFIG_WIFI_EU_CRYPTO 0
#endif

#if CONFIG_TX_AMPDU_PROT_MODE
#undef CONFIG_TX_AMPDU_PROT_MODE
#define CONFIG_TX_AMPDU_PROT_MODE 0
#endif

#if CONFIG_WNM_PS
#undef CONFIG_WNM_PS
#define CONFIG_WNM_PS 0
#endif

#if CONFIG_TURBO_MODE
#undef CONFIG_TURBO_MODE
#define CONFIG_TURBO_MODE 0
#endif

#if CONFIG_AUTO_RECONNECT
#undef CONFIG_AUTO_RECONNECT
#define CONFIG_AUTO_RECONNECT 0
#endif

#if CONFIG_DRIVER_OWE
#undef CONFIG_DRIVER_OWE
#define CONFIG_DRIVER_OWE 0
#endif

#ifdef CONFIG_OWE
#undef CONFIG_OWE
#endif

#if CONFIG_WIFI_FORCE_RTS
#undef CONFIG_WIFI_FORCE_RTS
#define CONFIG_WIFI_FORCE_RTS 0
#endif

#if CONFIG_WIFI_FRAG_THRESHOLD
#undef CONFIG_WIFI_FRAG_THRESHOLD
#define CONFIG_WIFI_FRAG_THRESHOLD 0
#endif

#if CONFIG_COMBO_SCAN
#undef CONFIG_COMBO_SCAN
#define CONFIG_COMBO_SCAN 0
#endif

#if CONFIG_SCAN_CHANNEL_GAP
#undef CONFIG_SCAN_CHANNEL_GAP
#define CONFIG_SCAN_CHANNEL_GAP 0
#endif

#if CONFIG_MAX_AP_ENTRIES
#undef CONFIG_MAX_AP_ENTRIES
#define CONFIG_MAX_AP_ENTRIES 5
#endif

#endif /* CONFIG_WIFI_SLIM_STA */

#if CONFIG_WIFI_SLIM_UAP

#if CONFIG_UAP_STA_MAC_ADDR_FILTER
#undef CONFIG_UAP_STA_MAC_ADDR_FILTER
#define CONFIG_UAP_STA_MAC_ADDR_FILTER 0
#endif

#if CONFIG_WIFI_MAX_CLIENTS_CNT
#undef CONFIG_WIFI_MAX_CLIENTS_CNT
#define CONFIG_WIFI_MAX_CLIENTS_CNT 0
#endif

#endif /* CONFIG_WIFI_SLIM_UAP */

/*
 * Development phase, recommend to set CONFIG_WIFI_SLIM_DISABLE_DBG to 0 for availablity of debug log.
 *
 * Customers can define CONFIG_WIFI_SLIM_DISABLE_DBG to 1 in wifi_config.h in product phase,
 * it can save about 19.54KB memory(flash: 17.58KB, sram: 1.97KB).
 * Memory footprint data is collected from sdk next gen flash_release wifi_cli.
 */

#if CONFIG_WIFI_SLIM_DISABLE_DBG

#if CONFIG_ENABLE_ERROR_LOGS
#undef CONFIG_ENABLE_ERROR_LOGS
#define CONFIG_ENABLE_ERROR_LOGS 0
#endif

#if CONFIG_ENABLE_WARNING_LOGS
#undef CONFIG_ENABLE_WARNING_LOGS
#define CONFIG_ENABLE_WARNING_LOGS 0
#endif

#if CONFIG_SEND_HOSTCMD
#undef CONFIG_SEND_HOSTCMD
#define CONFIG_SEND_HOSTCMD 0
#endif

#endif /* CONFIG_WIFI_SLIM_DISABLE_DBG */

#else
#error "Please define supported Wi-Fi module"
#endif

#endif /* WIFI_HDR_CONFIG_H */
