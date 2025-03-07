/*
 *  Copyright 2021-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WIFI_BT_MODULE_CONFIG_H_
#define _WIFI_BT_MODULE_CONFIG_H_

/* Wi-Fi boards configuration list */

/* AzureWave AW-NM191-uSD */
#if defined(WIFI_88W8801_BOARD_AW_NM191_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                        \
    {                                           \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x1B \
    }

/* AzureWave AW-NM191MA */
#elif defined(WIFI_88W8801_BOARD_AW_NM191MA)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                        \
    {                                           \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x1B \
    }

/* AzureWave AW-AM457-uSD */
#elif defined(WIFI_IW416_BOARD_AW_AM457_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_USD_INTERFACE
#define OVERRIDE_CALIBRATION_DATA "WIFI_IW416_BOARD_AW_AM457_CAL_DATA_EXT.h"
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* AzureWave AW-AM457MA */
#elif defined(WIFI_IW416_BOARD_AW_AM457MA)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_M2_INTERFACE
#define OVERRIDE_CALIBRATION_DATA "WIFI_IW416_BOARD_AW_AM457_CAL_DATA_EXT.h"
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* AzureWave AW-AM510-uSD */
#elif defined(WIFI_IW416_BOARD_AW_AM510_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define CONFIG_BR_SCO_PCM_DIRECTION 1
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* AzureWave AW-AM510MA */
#elif defined(WIFI_IW416_BOARD_AW_AM510MA)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define CONFIG_BR_SCO_PCM_DIRECTION 1
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* AzureWave AW-CM358-uSD */
#elif defined(WIFI_88W8987_BOARD_AW_CM358_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* AzureWave AW-CM358MA */
#elif defined(WIFI_88W8987_BOARD_AW_CM358MA)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* Murata 2DS + Murata uSD-M.2 adapter */
#elif defined(WIFI_88W8801_BOARD_MURATA_2DS_USD)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_WW.h"
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingSDR25HighSpeedMode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                        \
    {                                           \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x0E \
    }

/* Murata 2DS */
#elif defined(WIFI_88W8801_BOARD_MURATA_2DS_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2DS_WW.h"
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingSDR25HighSpeedMode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                        \
    {                                           \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x0E \
    }

/* Murata 1XK + Murata uSD-M.2 adapter */
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_USD)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x0, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }

/* Murata 1XK */
#elif defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x0, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }

/* Murata 1ZM + Murata uSD-M.2 adapter */
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_WW.h"
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x6, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }

/* Murata 1ZM */
#elif defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_CA.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_EU.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_JP.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_US.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1ZM_WW.h"
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x6, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }

/* USD Firecrest module */
#elif defined(WIFI_IW612_BOARD_RD_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* RD Firecrest module with M2 interface */
#elif defined(WIFI_IW612_BOARD_RD_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_CA_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_EU_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_JP_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_US_RU_Tx_power.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* 2EL Firecrest module with uSD adapter */
#elif defined(WIFI_IW612_BOARD_MURATA_2EL_USD)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_CA_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_EU_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_JP_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_US_RU_Tx_power.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* 2EL Firecrest module with M2 interface */
#elif defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_CA_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_EU_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_JP_RU_Tx_power.h"
// #define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_US_RU_Tx_power.h"
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* u-blox EVK-LILY-W131/-W132 */
#elif defined(WIFI_88W8801_BOARD_UBX_LILY_W1_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                        \
    {                                           \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x1B \
    }

/* u-blox EVK-JODY-W263 */
#elif defined(WIFI_88W8987_BOARD_UBX_JODY_W2_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }
/* Murata 2DL + Murata uSD-M.2 adapter */
#elif defined(WIFI_IW611_BOARD_MURATA_2DL_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* Murata 2DL */
#elif defined(WIFI_IW611_BOARD_MURATA_2DL_M2)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_2EL_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* u-blox JODY W5 uSD */
#elif defined(WIFI_AW611_BOARD_UBX_JODY_W5_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_jody_w5_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* u-blox JODY W5 M2 */
#elif defined(WIFI_AW611_BOARD_UBX_JODY_W5_M2)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_jody_w5_WW.h"
#define SD9177
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* u-blox EVK-MAYA-W161/-W166 */
#elif defined(WIFI_IW416_BOARD_UBX_MAYA_W1_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW.h"
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x9, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xC \
    }

/* Redfinch */
#elif defined(WIFI_BOARD_RW610)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_WW_rw610.h"
#define RW610

/* K32W061 transceiver */
#elif defined(K32W061_TRANSCEIVER)
/*
 * Wifi functions are not used with K32W061 but wifi files require to
 * be built, so stub macro are defined. Wifi functions won't be used at
 * link stage for k32w061 transceiver
 *
 */
#define SD8987

/* USD IW610 module */
#elif defined(WIFI_IW610_BOARD_RD_USD)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_NH_FCC.h"
#define IW610
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

/* RD IW610 module with M2 interface */
#elif defined(WIFI_IW610_BOARD_RD_M2)
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_NH_FCC.h"
#define IW610
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#define WIFI_BT_USE_M2_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0xA, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0xA \
    }

#else
#error "Please define macro related to wifi board"
#endif

#endif /* _WIFI_BT_MODULE_CONFIG_H_ */
