/*
** ###################################################################
**     Processors:          RW610ETA2I
**                          RW610HNA2I
**                          RW610UKA2I
**                          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b240715
**
**     Abstract:
**         CMSIS Peripheral Access Layer for APU
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2024 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2021-03-16)
**         Initial version.
**
** ###################################################################
*/

/*!
 * @file APU.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for APU
 *
 * CMSIS Peripheral Access Layer for APU
 */

#if !defined(APU_H_)
#define APU_H_                                   /**< Symbol preventing repeated inclusion */

#if (defined(CPU_RW610ETA2I) || defined(CPU_RW610HNA2I) || defined(CPU_RW610UKA2I))
#include "RW610_COMMON.h"
#elif (defined(CPU_RW612ETA2I) || defined(CPU_RW612HNA2I) || defined(CPU_RW612UKA2I))
#include "RW612_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
  #else
    #pragma push
    #pragma anon_unions
  #endif
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- APU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup APU_Peripheral_Access_Layer APU Peripheral Access Layer
 * @{
 */

/** APU - Register Layout Typedef */
typedef struct {
  __I  uint32_t APU_IP_REVISION;                   /**< APU IP revision, offset: 0x0 */
  __IO uint32_t CTRL;                              /**< Control, offset: 0x4 */
  __IO uint32_t TIMER_PWR_MODE;                    /**< Timer Power Mode, offset: 0x8 */
  __IO uint32_t PWR_CTRL;                          /**< Power Control, offset: 0xC */
  __IO uint32_t PLL_CTRL;                          /**< PLL Control, offset: 0x10 */
  __IO uint32_t PLL_DIS_CNT;                       /**< PLL Disable Count, offset: 0x14 */
  __IO uint32_t STABLE_CNT;                        /**< Stable Count, offset: 0x18 */
  __IO uint32_t CPU1_HOST_WKUP_MASK;               /**< CPU1 Host Wakeup Mask, offset: 0x1C */
  __IO uint32_t CPU1_HOST_WKUP_POL;                /**< CPU1 Host Wakeup Polarity, offset: 0x20 */
  __I  uint32_t CAL_WIN_CNT;                       /**< Calibration Window Count, offset: 0x24 */
  __I  uint32_t AUTO_10US;                         /**< Auto 10us, offset: 0x28 */
  __I  uint32_t ACC_CNT;                           /**< Accumulation Count, offset: 0x2C */
  __IO uint32_t CAL_WIN_SIZE;                      /**< Calibration Window Size, offset: 0x30 */
  __I  uint32_t SLP_CLK_TIMER;                     /**< Sleep Clock Timer, offset: 0x34 */
  __I  uint32_t STATUS_RD;                         /**< Status Read, offset: 0x38 */
  __IO uint32_t STABLE_CNT2;                       /**< Stable Count 2, offset: 0x3C */
  __IO uint32_t DYN_PLL_MASK;                      /**< Dynamic PLL Mask, offset: 0x40 */
  __I  uint32_t TESTBUS_RD1;                       /**< Testbus Read 1, offset: 0x44 */
  __I  uint32_t TESTBUS_RD2;                       /**< Testbus Read 2, offset: 0x48 */
  __IO uint32_t GENERIC_TIMER_CNT;                 /**< Generic Timer Count, offset: 0x4C */
  __IO uint32_t CPU1_HOST_WKUP_CTRL;               /**< CPU1 Host Wakeup Control, offset: 0x50 */
  __IO uint32_t DYN_PLL_CLK_EXT_CTRL;              /**< Dynamic PLL Clock Ext Control, offset: 0x54 */
  __IO uint32_t GENERIC_SLP_START_VAL;             /**< Generic Sleep Start Value, offset: 0x58 */
  __IO uint32_t DLY_HOST_CTRL;                     /**< Delay Host Control, offset: 0x5C */
  __I  uint32_t HOST_WKUP_CNT;                     /**< Host Wakeup Count, offset: 0x60 */
  __IO uint32_t CPU2_HOST_WKUP_MASK;               /**< CPU2 Host Wakeup Mask, offset: 0x64 */
  __IO uint32_t CPU2_HOST_WKUP_POL;                /**< CPU2 Host Wakeup Polarity, offset: 0x68 */
  __IO uint32_t CPU2_HOST_WKUP_CTRL;               /**< CPU2 Host Wakeup Control, offset: 0x6C */
  __IO uint32_t CPU2_CTRL;                         /**< CPU2 Control, offset: 0x70 */
  __IO uint32_t WLAN_PWR_CTRL_DLY;                 /**< Wlan domain FSM Power Control Delay, offset: 0x74 */
  __I  uint32_t STATUS2;                           /**< Status 2, offset: 0x78 */
  __IO uint32_t WLAN_PWR_CTRL_DLY2;                /**< Wlan domain FSM Power Control Delay 2, offset: 0x7C */
  __IO uint32_t WL_CTRL;                           /**< WLAN Control, offset: 0x80 */
  __IO uint32_t WL_WKUP_MASK;                      /**< WLAN Wakeup Mask, offset: 0x84 */
  __I  uint32_t WL_STATUS;                         /**< WLAN Status, offset: 0x88 */
  __I  uint32_t WL_ALARM_RD;                       /**< WLAN Alarm Readback, offset: 0x8C */
  __IO uint32_t WL_BCN_XP_ALARM;                   /**< WLAN Beacon Alarm, offset: 0x90 */
  __IO uint32_t WL_BCN_INTR_ALARM;                 /**< WLAN Beacon Interrupt Alarm, offset: 0x94 */
  __IO uint32_t WL_HOST_INTR_ALARM;                /**< WLAN Host Interrupt Alarm, offset: 0x98 */
  __IO uint32_t WL_BCN_PLL_ALARM;                  /**< WLAN Beacon PLL Alarm, offset: 0x9C */
  __IO uint32_t TSF_REF_FACTOR;                    /**< TSF Reference Factor, offset: 0xA0 */
  __IO uint32_t TSF_SLEEP_FACTOR;                  /**< TSF Sleep Factor, offset: 0xA4 */
  __IO uint32_t BBUD_UDR_ISO_CNT;                  /**< BBUD UDR ISO Count, offset: 0xA8 */
  __IO uint32_t WL_DVFS_CTRL;                      /**< WLAN DVFS Control, offset: 0xAC */
  __IO uint32_t WL_CTRL2;                          /**< WLAN Control 2, offset: 0xB0 */
  __IO uint32_t WL_WKUP_MASK2;                     /**< WLAN Wakeup Mask 2, offset: 0xB4 */
  __I  uint32_t WL_STATUS2;                        /**< WLAN Status 2, offset: 0xB8 */
  __I  uint32_t WL_ALARM_RD2;                      /**< WLAN Alarm Readback 2, offset: 0xBC */
  __IO uint32_t WL_BCN_XP_ALARM2;                  /**< WLAN Beacon XP Alarm 2, offset: 0xC0 */
  __IO uint32_t WL_BCN_INTR_ALARM2;                /**< WLAN Beacon Interrupt Alarm 2, offset: 0xC4 */
  __IO uint32_t WL_HOST_INTR_ALARM2;               /**< WLAN Host Interrupt Alarm 2, offset: 0xC8 */
  __IO uint32_t WL_BCN_PLL_ALARM2;                 /**< WLAN Beacon PLL Alarm 2, offset: 0xCC */
  __IO uint32_t WLCOMN_PWR_CTRL;                   /**< WLAN Comm Powerup Control, offset: 0xD0 */
  __I  uint32_t WLCOMN_STATUS;                     /**< WLAN Comm Status, offset: 0xD4 */
       uint8_t RESERVED_0[8];
  __IO uint32_t BT_CTRL;                           /**< Bluetooth Control, offset: 0xE0 */
  __IO uint32_t BT_WKUP_MASK;                      /**< Bluetooth Wakeup Mask, offset: 0xE4 */
  __I  uint32_t BT_STATUS;                         /**< Bluetooth Status, offset: 0xE8 */
  __IO uint32_t BT_CKEN_CTRL;                      /**< Bluetooth Clock Enable Control, offset: 0xEC */
  __IO uint32_t BT_RESRC_CTRL;                     /**< Bluetooth RESRC Control, offset: 0xF0 */
  __IO uint32_t BT_DVFS_CTRL;                      /**< Bluetooth DVFS Control, offset: 0xF4 */
       uint8_t RESERVED_1[8];
  __IO uint32_t FM_CTRL;                           /**< FM Control, offset: 0x100 */
  __IO uint32_t FM_WKUP_MASK;                      /**< FM Wakeup Mask, offset: 0x104 */
  __I  uint32_t FM_STATUS;                         /**< FM Status, offset: 0x108 */
  __IO uint32_t FM_CKEN_CTRL;                      /**< FM Clock Enable Control, offset: 0x10C */
  __IO uint32_t FM_RESRC_CTRL;                     /**< FM RESRC Control, offset: 0x110 */
  __IO uint32_t FM_DVFS_CTRL;                      /**< FM DVFS Control, offset: 0x114 */
       uint8_t RESERVED_2[8];
  __IO uint32_t USB_PWR_CTRL_DLY;                  /**< USB FSM Power Control Delay, offset: 0x120 */
  __IO uint32_t USB_PWR_CTRL_DLY2;                 /**< USB FSM Power Control Delay 2, offset: 0x124 */
  __IO uint32_t WLCOMN_PWR_CTRL_DLY;               /**< Wlan common domain FSM Power Control Delay, offset: 0x128 */
  __IO uint32_t WLCOMN_PWR_CTRL_DLY2;              /**< Wlan common domain FSM Power Control Delay 2, offset: 0x12C */
       uint8_t RESERVED_3[16];
  __IO uint32_t BLE_CTRL;                          /**< BLE Control, offset: 0x140 */
  __IO uint32_t BLE_WKUP_MASK;                     /**< BLE Wakeup Mask, offset: 0x144 */
  __I  uint32_t BLE_STATUS;                        /**< BLE Status, offset: 0x148 */
       uint8_t RESERVED_4[20];
  __IO uint32_t NFC_CTRL;                          /**< NFC Control, offset: 0x160 */
  __IO uint32_t NFC_WKUP_MASK;                     /**< NFC Wakeup Mask, offset: 0x164 */
  __I  uint32_t NFC_STATUS;                        /**< NFC Status, offset: 0x168 */
  __IO uint32_t NFC_CKEN_CTRL;                     /**< NFC Clock Enable Control, offset: 0x16C */
  __IO uint32_t NFC_RESRC_CTRL;                    /**< NFC RESRC Control, offset: 0x170 */
  __IO uint32_t NFC_DVFS_CTRL;                     /**< NFC DVFS Control, offset: 0x174 */
       uint8_t RESERVED_5[8];
  __IO uint32_t FP4_CTRL;                          /**< FP4(15.4) Control, offset: 0x180 */
  __IO uint32_t FP4_WKUP_MASK;                     /**< FP4 Wakeup Mask, offset: 0x184 */
  __I  uint32_t FP4_STATUS;                        /**< FP4 Status, offset: 0x188 */
  __IO uint32_t FP4_CKEN_CTRL;                     /**< FP4 Clock Enable Control, offset: 0x18C */
  __IO uint32_t FP4_RESRC_CTRL;                    /**< FP4 RESRC Control, offset: 0x190 */
  __IO uint32_t FP4_DVFS_CTRL;                     /**< FP4 DVFS Control, offset: 0x194 */
  __IO uint32_t CPU2_FP4_HOST_WKUP_MASK;           /**< CPU2 15.4 Host Wakeup Mask, offset: 0x198 */
  __IO uint32_t CPU2_FP4_HOST_WKUP_POL;            /**< CPU2 15.4 Host Wakeup Polarity, offset: 0x19C */
  __IO uint32_t CPU2_FP4_HOST_WKUP_CTRL;           /**< CPU2 15.4 Host Wakeup Control, offset: 0x1A0 */
  __IO uint32_t HW_IP_ACTIVE_INDEX_CTRL;           /**< HW IP active index Control, offset: 0x1A4 */
  __IO uint32_t HW_IP_INACTIVE_INDEX_CTRL;         /**< HW IP inactive Control, offset: 0x1A8 */
  __IO uint32_t HW_IP_DYNAMIC_CLK_SWITCH_CTRL;     /**< HW IP dynamic clock switching contrl, offset: 0x1AC */
  __IO uint32_t IHB_CTRL;                          /**< IHB Control, offset: 0x1B0 */
  __IO uint32_t IHB_WKUP_MASK;                     /**< IHB Wakeup Mask, offset: 0x1B4 */
  __IO uint32_t CPU2_IHB_WKUP_MASK;                /**< CPU2 IHB Wakeup Mask, offset: 0x1B8 */
  __I  uint32_t IHB_STATUS;                        /**< IHB Status, offset: 0x1BC */
  __IO uint32_t USB_CTRL;                          /**< USB Control, offset: 0x1C0 */
  __IO uint32_t USB_WKUP_MASK;                     /**< USB Wakeup Mask, offset: 0x1C4 */
  __I  uint32_t USB_STATUS;                        /**< USB Status, offset: 0x1C8 */
       uint8_t RESERVED_6[52];
  __IO uint32_t CPU1_DVFS_CTRL;                    /**< CPU1 DVFS Control, offset: 0x200 */
  __IO uint32_t CPU1_FREQ_REG1;                    /**< CPU1 Frequency 1, offset: 0x204 */
  __IO uint32_t CPU1_FREQ_REG2;                    /**< CPU1 Frequency 2, offset: 0x208 */
  __IO uint32_t CPU1_PLL_EN_REG;                   /**< CPU1 PLL Enable, offset: 0x20C */
  __IO uint32_t CPU1_VOL_REG1;                     /**< CPU1 Voltage 1, offset: 0x210 */
  __IO uint32_t CPU1_VOL_REG2;                     /**< CPU1 Voltage 2, offset: 0x214 */
  __IO uint32_t CPU1_VOL_REG3;                     /**< CPU1 Voltage 3, offset: 0x218 */
  __IO uint32_t CPU1_VOL_REG4;                     /**< CPU1 Voltage 4, offset: 0x21C */
  __IO uint32_t CPU2_DVFS_CTRL;                    /**< CPU2 DVFS Control, offset: 0x220 */
  __IO uint32_t CPU2_FREQ_REG1;                    /**< CPU2 Frequency 1, offset: 0x224 */
  __IO uint32_t CPU2_FREQ_REG2;                    /**< CPU2 Frequency 2, offset: 0x228 */
  __IO uint32_t CPU2_PLL_EN_REG;                   /**< CPU2 PLL Enable, offset: 0x22C */
  __IO uint32_t CPU2_VOL_REG1;                     /**< CPU2 Voltage 1, offset: 0x230 */
  __IO uint32_t CPU2_VOL_REG2;                     /**< CPU2 Voltage 2, offset: 0x234 */
  __IO uint32_t CPU2_VOL_REG3;                     /**< CPU2 Voltage 3, offset: 0x238 */
  __IO uint32_t CPU2_VOL_REG4;                     /**< CPU2 Voltage 4, offset: 0x23C */
  __IO uint32_t SYS_FREQ_REG1;                     /**< System Frequency 1, offset: 0x240 */
  __IO uint32_t SYS_FREQ_REG2;                     /**< System Frequency 2, offset: 0x244 */
  __IO uint32_t SYS_PLL_EN_REG;                    /**< System PLL Enable, offset: 0x248 */
  __IO uint32_t SYS_VOL_REG1;                      /**< System Voltage 1, offset: 0x24C */
  __IO uint32_t SYS_VOL_REG2;                      /**< System Voltage 2, offset: 0x250 */
  __IO uint32_t SYS_VOL_REG3;                      /**< System Voltage 3, offset: 0x254 */
  __IO uint32_t SYS_VOL_REG4;                      /**< System Voltage 4, offset: 0x258 */
  __IO uint32_t DVFS_CTRL;                         /**< DVFS Control, offset: 0x25C */
  __IO uint32_t PARTIAL_DVFS_CTRL;                 /**< Partial DVFS Control, offset: 0x260 */
  __IO uint32_t DVFS_TIMER;                        /**< DVFS Timer, offset: 0x264 */
       uint8_t RESERVED_7[8];
  __IO uint32_t AHB1_FREQ_REG1;                    /**< AHB1 Frequency 1, offset: 0x270 */
  __IO uint32_t AHB1_FREQ_REG2;                    /**< AHB1 Frequency 2, offset: 0x274 */
  __IO uint32_t AHB1_PLL_EN_REG;                   /**< AHB1 PLL Enable, offset: 0x278 */
  __IO uint32_t AHB1_VOL_REG1;                     /**< AHB1 Voltage 1, offset: 0x27C */
  __IO uint32_t AHB1_VOL_REG2;                     /**< AHB1 Voltage 2, offset: 0x280 */
  __IO uint32_t AHB1_VOL_REG3;                     /**< AHB1 Voltage 3, offset: 0x284 */
  __IO uint32_t AHB1_VOL_REG4;                     /**< AHB1 Voltage 4, offset: 0x288 */
       uint8_t RESERVED_8[12];
  __I  uint32_t DVFS_STATUS;                       /**< DVFS Status, offset: 0x298 */
  __IO uint32_t DVFS_DBG_CTRL;                     /**< DVFS Debug Control, offset: 0x29C */
  __IO uint32_t DVFS_DBG_PATTERN_DATA;             /**< DVFS Debug Pattern Data, offset: 0x2A0 */
  __I  uint32_t DVFS_DBG_REG0;                     /**< DVFS Debug 0, offset: 0x2A4 */
  __I  uint32_t DVFS_DBG_REG1;                     /**< DVFS Debug 1, offset: 0x2A8 */
  __I  uint32_t DVFS_DBG_REG2;                     /**< DVFS Debug 2, offset: 0x2AC */
  __I  uint32_t DVFS_DBG_REG3;                     /**< DVFS Debug 3, offset: 0x2B0 */
  __I  uint32_t DVFS_DBG_REG4;                     /**< DVFS Debug 4, offset: 0x2B4 */
  __I  uint32_t DVFS_DBG_REG5;                     /**< DVFS Debug 5, offset: 0x2B8 */
  __I  uint32_t DVFS_DBG_STATUS;                   /**< DVFS Debug Status, offset: 0x2BC */
  __IO uint32_t DVFS_DBG_PATTERN_MASK;             /**< DVFS Debug Patter Mask, offset: 0x2C0 */
       uint8_t RESERVED_9[52];
  __IO uint32_t DVFS_PMIC_TIMER;                   /**< DVFS PMIC Timer, offset: 0x2F8 */
  __IO uint32_t DVFS_PMIC_MAP;                     /**< DVFS PMIC Map, offset: 0x2FC */
  __IO uint32_t LDO_CTRL;                          /**< LDO Control, offset: 0x300 */
  __IO uint32_t LDO_BACKUP_LVL_MAP1;               /**< LDO Backup Level Map 1, offset: 0x304 */
  __IO uint32_t LDO_BACKUP_LVL_MAP2;               /**< LDO Backup Level Map 2, offset: 0x308 */
  __I  uint32_t LDO_STATUS;                        /**< LDO Status, offset: 0x30C */
  __IO uint32_t RC32_CAL_CTRL;                     /**< RC32 Calibration Control, offset: 0x310 */
  __IO uint32_t RC32_CAL_SLPCLK_TIMER;             /**< RC32 Calibration Sleep Clock Timer, offset: 0x314 */
  __I  uint32_t RC32CAL_SLPCLK_CNT_RD;             /**< RC32 Calibration Sleep Clock Count Read, offset: 0x318 */
       uint8_t RESERVED_10[28];
  __IO uint32_t TSTBUS_DATA;                       /**< Testbus Data, offset: 0x338 */
  __IO uint32_t TST_CTRL;                          /**< Test Control, offset: 0x33C */
  __IO uint32_t BCA_LTE_CTRL;                      /**< BCA LTE Control, offset: 0x340 */
  __IO uint32_t BCA_LTE_TIMER1;                    /**< BCA LTE Timer 1, offset: 0x344 */
  __IO uint32_t BCA_LTE_TIMER2;                    /**< BCA LTE Timer 2, offset: 0x348 */
  __IO uint32_t BCA_MWS_WKUP_TIMER;                /**< BCA MWS Wakeup Timer, offset: 0x34C */
  __IO uint32_t BT2_CTRL;                          /**< Bluetooth 2 Control, offset: 0x350 */
  __IO uint32_t BT2_WKUP_MASK;                     /**< Bluetooth 2 Wakeup Mask, offset: 0x354 */
  __I  uint32_t BT2_STATUS;                        /**< Bluetooth 2 Status, offset: 0x358 */
  __IO uint32_t BT2_CKEN_CTRL;                     /**< Bluetooth 2 Clock Enable Control, offset: 0x35C */
  __IO uint32_t BT2_RESRC_CTRL;                    /**< Bluetooth 2 RESRC Control, offset: 0x360 */
  __IO uint32_t BT2_DVFS_CTRL;                     /**< Bluetooth 2 DVFS Control, offset: 0x364 */
  __IO uint32_t BLE2_CTRL;                         /**< BLE 2 Control, offset: 0x368 */
  __IO uint32_t BLE2_WKUP_MASK;                    /**< BLE 2 Wakeup Mask, offset: 0x36C */
  __I  uint32_t BLE2_STATUS;                       /**< BLE 2 Status, offset: 0x370 */
       uint8_t RESERVED_11[12];
  __I  uint32_t LMU_IP_REVISION;                   /**< LMU IP Revision, offset: 0x380 */
  __IO uint32_t LMU_CPU1_STA_CFG;                  /**< LMU CPU1 STA Configuration, offset: 0x384 */
  __I  uint32_t LMU_CPU1_STA_STATUS1;              /**< LMU CPU1 STA Status 1, offset: 0x388 */
  __I  uint32_t LMU_CPU1_STA_STATUS2;              /**< LMU CPU1 STA Status 2, offset: 0x38C */
       uint8_t RESERVED_12[8];
  __IO uint32_t LMU_CPU1_DYN_CTRL;                 /**< LMU CPU1 Dynamic Control, offset: 0x398 */
  __I  uint32_t LMU_CPU1_DYN_STATUS;               /**< LMU CPU1 Dynamic Status, offset: 0x39C */
       uint8_t RESERVED_13[8];
  __IO uint32_t LMU_CPU2_STA_CFG;                  /**< LMU CPU2 STA Configuration, offset: 0x3A8 */
  __I  uint32_t LMU_CPU2_STA_STATUS1;              /**< LMU CPU2 STA Status 1, offset: 0x3AC */
  __IO uint32_t LMU_CPU3_STA_CFG;                  /**< LMU CPU3 STA Configuration, offset: 0x3B0 */
  __I  uint32_t LMU_CPU3_STA_STATUS1;              /**< LMU CPU3 STA Status 1, offset: 0x3B4 */
  __IO uint32_t LMU_G2BIST_CTRL;                   /**< LMU G2BIST Control, offset: 0x3B8 */
  __I  uint32_t LMU_STATUS;                        /**< LMU Status, offset: 0x3BC */
  __IO uint32_t LMU_STA_CFG_MASK;                  /**< LMU STA Configuration Mask, offset: 0x3C0 */
  __I  uint32_t LMU_CPU2_STA_STATUS2;              /**< LMU CPU2 STA Status 2, offset: 0x3C4 */
  __I  uint32_t LMU_CPU3_STA_STATUS2;              /**< LMU CPU3 STA Status 2, offset: 0x3C8 */
       uint8_t RESERVED_14[4];
  __IO uint32_t CPU3_HOST_WKUP_MASK;               /**< CPU3 Host Wakeup Mask, offset: 0x3D0 */
  __IO uint32_t CPU3_HOST_WKUP_POL;                /**< CPU3 Host Wakeup Polarity, offset: 0x3D4 */
  __IO uint32_t CPU3_HOST_WKUP_CTRL;               /**< CPU3 Host Wakeup Control, offset: 0x3D8 */
  __IO uint32_t CPU3_CTRL;                         /**< CPU3 Control, offset: 0x3DC */
  __IO uint32_t CPU3_DVFS_CTRL;                    /**< CPU3 DVFS Control, offset: 0x3E0 */
  __IO uint32_t CPU3_FREQ_REG1;                    /**< CPU3 Frequency 1, offset: 0x3E4 */
  __IO uint32_t CPU3_FREQ_REG2;                    /**< CPU3 Frequency 2, offset: 0x3E8 */
  __IO uint32_t CPU3_PLL_EN_REG;                   /**< CPU3 PLL Enable, offset: 0x3EC */
  __IO uint32_t CPU3_VOL_REG1;                     /**< CPU3 Voltage 1, offset: 0x3F0 */
  __IO uint32_t CPU3_VOL_REG2;                     /**< CPU3 Voltage 2, offset: 0x3F4 */
  __IO uint32_t CPU3_VOL_REG3;                     /**< CPU3 Voltage 3, offset: 0x3F8 */
  __IO uint32_t CPU3_VOL_REG4;                     /**< CPU3 Voltage 4, offset: 0x3FC */
} APU_Type;

/* ----------------------------------------------------------------------------
   -- APU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup APU_Register_Masks APU Register Masks
 * @{
 */

/*! @name APU_IP_REVISION - APU IP revision */
/*! @{ */

#define APU_APU_IP_REVISION_IP_REV_MASK          (0xFFFFU)
#define APU_APU_IP_REVISION_IP_REV_SHIFT         (0U)
/*! IP_REV - IP revision */
#define APU_APU_IP_REVISION_IP_REV(x)            (((uint32_t)(((uint32_t)(x)) << APU_APU_IP_REVISION_IP_REV_SHIFT)) & APU_APU_IP_REVISION_IP_REV_MASK)
/*! @} */

/*! @name CTRL - Control */
/*! @{ */

#define APU_CTRL_CPU1_PD_MASK_MASK               (0x1U)
#define APU_CTRL_CPU1_PD_MASK_SHIFT              (0U)
/*! CPU1_PD_MASK - CPU1 Pd Mask */
#define APU_CTRL_CPU1_PD_MASK(x)                 (((uint32_t)(((uint32_t)(x)) << APU_CTRL_CPU1_PD_MASK_SHIFT)) & APU_CTRL_CPU1_PD_MASK_MASK)

#define APU_CTRL_FW_CPU1_PD_MASK                 (0x2U)
#define APU_CTRL_FW_CPU1_PD_SHIFT                (1U)
/*! FW_CPU1_PD - FW CPU1 Pd */
#define APU_CTRL_FW_CPU1_PD(x)                   (((uint32_t)(((uint32_t)(x)) << APU_CTRL_FW_CPU1_PD_SHIFT)) & APU_CTRL_FW_CPU1_PD_MASK)

#define APU_CTRL_WLCOMN_USE_NOM_PWR_BYP_MASK     (0x8U)
#define APU_CTRL_WLCOMN_USE_NOM_PWR_BYP_SHIFT    (3U)
/*! WLCOMN_USE_NOM_PWR_BYP - WLAN comman domain uses nominal voltage */
#define APU_CTRL_WLCOMN_USE_NOM_PWR_BYP(x)       (((uint32_t)(((uint32_t)(x)) << APU_CTRL_WLCOMN_USE_NOM_PWR_BYP_SHIFT)) & APU_CTRL_WLCOMN_USE_NOM_PWR_BYP_MASK)

#define APU_CTRL_APU_SUBSYS1_HOST_MASK           (0x70U)
#define APU_CTRL_APU_SUBSYS1_HOST_SHIFT          (4U)
/*! APU_SUBSYS1_HOST - APU Subsystem1 Host */
#define APU_CTRL_APU_SUBSYS1_HOST(x)             (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_SUBSYS1_HOST_SHIFT)) & APU_CTRL_APU_SUBSYS1_HOST_MASK)

#define APU_CTRL_APU_CAL_DONE_CLEAR_METHOD_MASK  (0x80U)
#define APU_CTRL_APU_CAL_DONE_CLEAR_METHOD_SHIFT (7U)
/*! APU_CAL_DONE_CLEAR_METHOD - 1- use falling edge of apu_sw_acc_rst to clear the apu_cal_done after FW polls the apu_cal_done = 1 */
#define APU_CTRL_APU_CAL_DONE_CLEAR_METHOD(x)    (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_CAL_DONE_CLEAR_METHOD_SHIFT)) & APU_CTRL_APU_CAL_DONE_CLEAR_METHOD_MASK)

#define APU_CTRL_ENTER_ALL_SLEEP_MODE_MASK       (0x100U)
#define APU_CTRL_ENTER_ALL_SLEEP_MODE_SHIFT      (8U)
/*! ENTER_ALL_SLEEP_MODE - Enter All Sleep Mode */
#define APU_CTRL_ENTER_ALL_SLEEP_MODE(x)         (((uint32_t)(((uint32_t)(x)) << APU_CTRL_ENTER_ALL_SLEEP_MODE_SHIFT)) & APU_CTRL_ENTER_ALL_SLEEP_MODE_MASK)

#define APU_CTRL_USE_HOST_INTR_SLP_MASK          (0x200U)
#define APU_CTRL_USE_HOST_INTR_SLP_SHIFT         (9U)
/*! USE_HOST_INTR_SLP - Use Host Interrupt Sleep */
#define APU_CTRL_USE_HOST_INTR_SLP(x)            (((uint32_t)(((uint32_t)(x)) << APU_CTRL_USE_HOST_INTR_SLP_SHIFT)) & APU_CTRL_USE_HOST_INTR_SLP_MASK)

#define APU_CTRL_APU_ALLOW_AUTO_LOAD_MASK        (0x400U)
#define APU_CTRL_APU_ALLOW_AUTO_LOAD_SHIFT       (10U)
/*! APU_ALLOW_AUTO_LOAD - APU Allow Auto load */
#define APU_CTRL_APU_ALLOW_AUTO_LOAD(x)          (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_ALLOW_AUTO_LOAD_SHIFT)) & APU_CTRL_APU_ALLOW_AUTO_LOAD_MASK)

#define APU_CTRL_APU_AUTO_CALIBRATE_MASK         (0x800U)
#define APU_CTRL_APU_AUTO_CALIBRATE_SHIFT        (11U)
/*! APU_AUTO_CALIBRATE - APU Auto Calibrate */
#define APU_CTRL_APU_AUTO_CALIBRATE(x)           (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_AUTO_CALIBRATE_SHIFT)) & APU_CTRL_APU_AUTO_CALIBRATE_MASK)

#define APU_CTRL_APU_NO_TBTT_RESET_MASK          (0x1000U)
#define APU_CTRL_APU_NO_TBTT_RESET_SHIFT         (12U)
/*! APU_NO_TBTT_RESET - APU No TBTT Reset */
#define APU_CTRL_APU_NO_TBTT_RESET(x)            (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_NO_TBTT_RESET_SHIFT)) & APU_CTRL_APU_NO_TBTT_RESET_MASK)

#define APU_CTRL_SW_TIMER_LD_MASK                (0x2000U)
#define APU_CTRL_SW_TIMER_LD_SHIFT               (13U)
/*! SW_TIMER_LD - SW Timer */
#define APU_CTRL_SW_TIMER_LD(x)                  (((uint32_t)(((uint32_t)(x)) << APU_CTRL_SW_TIMER_LD_SHIFT)) & APU_CTRL_SW_TIMER_LD_MASK)

#define APU_CTRL_RST_APU_SLPCLK_TIMER_MASK       (0x4000U)
#define APU_CTRL_RST_APU_SLPCLK_TIMER_SHIFT      (14U)
/*! RST_APU_SLPCLK_TIMER - Reset APU Sleep Clock Timer */
#define APU_CTRL_RST_APU_SLPCLK_TIMER(x)         (((uint32_t)(((uint32_t)(x)) << APU_CTRL_RST_APU_SLPCLK_TIMER_SHIFT)) & APU_CTRL_RST_APU_SLPCLK_TIMER_MASK)

#define APU_CTRL_APU_SW_ACC_RST_MASK             (0x8000U)
#define APU_CTRL_APU_SW_ACC_RST_SHIFT            (15U)
/*! APU_SW_ACC_RST - APU SW ACC Reset */
#define APU_CTRL_APU_SW_ACC_RST(x)               (((uint32_t)(((uint32_t)(x)) << APU_CTRL_APU_SW_ACC_RST_SHIFT)) & APU_CTRL_APU_SW_ACC_RST_MASK)

#define APU_CTRL_SDU_SLP_RDY_MASK_MASK           (0x20000U)
#define APU_CTRL_SDU_SLP_RDY_MASK_SHIFT          (17U)
/*! SDU_SLP_RDY_MASK - SDU Sleep Ready Mask */
#define APU_CTRL_SDU_SLP_RDY_MASK(x)             (((uint32_t)(((uint32_t)(x)) << APU_CTRL_SDU_SLP_RDY_MASK_SHIFT)) & APU_CTRL_SDU_SLP_RDY_MASK_MASK)

#define APU_CTRL_IPS_CP15_SLEEP_MASK_MASK        (0x200000U)
#define APU_CTRL_IPS_CP15_SLEEP_MASK_SHIFT       (21U)
/*! IPS_CP15_SLEEP_MASK - IPS CP15 Sleep Mask */
#define APU_CTRL_IPS_CP15_SLEEP_MASK(x)          (((uint32_t)(((uint32_t)(x)) << APU_CTRL_IPS_CP15_SLEEP_MASK_SHIFT)) & APU_CTRL_IPS_CP15_SLEEP_MASK_MASK)

#define APU_CTRL_FW_IPS_CP15_SLEEP_MASK          (0x400000U)
#define APU_CTRL_FW_IPS_CP15_SLEEP_SHIFT         (22U)
/*! FW_IPS_CP15_SLEEP - FW IPS CP15 Sleep */
#define APU_CTRL_FW_IPS_CP15_SLEEP(x)            (((uint32_t)(((uint32_t)(x)) << APU_CTRL_FW_IPS_CP15_SLEEP_SHIFT)) & APU_CTRL_FW_IPS_CP15_SLEEP_MASK)

#define APU_CTRL_CP_15_SLEEP_MASK_MASK           (0x800000U)
#define APU_CTRL_CP_15_SLEEP_MASK_SHIFT          (23U)
/*! CP_15_SLEEP_MASK - CP15 Sleep Mask */
#define APU_CTRL_CP_15_SLEEP_MASK(x)             (((uint32_t)(((uint32_t)(x)) << APU_CTRL_CP_15_SLEEP_MASK_SHIFT)) & APU_CTRL_CP_15_SLEEP_MASK_MASK)

#define APU_CTRL_FW_CP15_SLEEP_MASK              (0x1000000U)
#define APU_CTRL_FW_CP15_SLEEP_SHIFT             (24U)
/*! FW_CP15_SLEEP - FW CP15 Sleep */
#define APU_CTRL_FW_CP15_SLEEP(x)                (((uint32_t)(((uint32_t)(x)) << APU_CTRL_FW_CP15_SLEEP_SHIFT)) & APU_CTRL_FW_CP15_SLEEP_MASK)

#define APU_CTRL_CPU1_MSG_RDY_MASK_MASK          (0x2000000U)
#define APU_CTRL_CPU1_MSG_RDY_MASK_SHIFT         (25U)
/*! CPU1_MSG_RDY_MASK - CPU1 Message Ready Mask */
#define APU_CTRL_CPU1_MSG_RDY_MASK(x)            (((uint32_t)(((uint32_t)(x)) << APU_CTRL_CPU1_MSG_RDY_MASK_SHIFT)) & APU_CTRL_CPU1_MSG_RDY_MASK_MASK)
/*! @} */

/*! @name TIMER_PWR_MODE - Timer Power Mode */
/*! @{ */

#define APU_TIMER_PWR_MODE_MCI_XP_REQ_MASK_MASK  (0x1U)
#define APU_TIMER_PWR_MODE_MCI_XP_REQ_MASK_SHIFT (0U)
/*! MCI_XP_REQ_MASK - MCI XP Request Mask */
#define APU_TIMER_PWR_MODE_MCI_XP_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_MCI_XP_REQ_MASK_SHIFT)) & APU_TIMER_PWR_MODE_MCI_XP_REQ_MASK_MASK)

#define APU_TIMER_PWR_MODE_SLP_CLK_TIMER_EN_MASK (0x10U)
#define APU_TIMER_PWR_MODE_SLP_CLK_TIMER_EN_SHIFT (4U)
/*! SLP_CLK_TIMER_EN - free running sleep clock timer enable */
#define APU_TIMER_PWR_MODE_SLP_CLK_TIMER_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_SLP_CLK_TIMER_EN_SHIFT)) & APU_TIMER_PWR_MODE_SLP_CLK_TIMER_EN_MASK)

#define APU_TIMER_PWR_MODE_WL_BCN_ALARM_TIMER_EN_MASK (0x20U)
#define APU_TIMER_PWR_MODE_WL_BCN_ALARM_TIMER_EN_SHIFT (5U)
/*! WL_BCN_ALARM_TIMER_EN - WLAN beacon alarm timer enable */
#define APU_TIMER_PWR_MODE_WL_BCN_ALARM_TIMER_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_WL_BCN_ALARM_TIMER_EN_SHIFT)) & APU_TIMER_PWR_MODE_WL_BCN_ALARM_TIMER_EN_MASK)

#define APU_TIMER_PWR_MODE_WL2_BCN_ALARM_TIMER_EN_MASK (0x40U)
#define APU_TIMER_PWR_MODE_WL2_BCN_ALARM_TIMER_EN_SHIFT (6U)
/*! WL2_BCN_ALARM_TIMER_EN - WLAN2 beacon alarm timer enable */
#define APU_TIMER_PWR_MODE_WL2_BCN_ALARM_TIMER_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_WL2_BCN_ALARM_TIMER_EN_SHIFT)) & APU_TIMER_PWR_MODE_WL2_BCN_ALARM_TIMER_EN_MASK)

#define APU_TIMER_PWR_MODE_DIS_GATED_SLP_CLK_MASK (0x80U)
#define APU_TIMER_PWR_MODE_DIS_GATED_SLP_CLK_SHIFT (7U)
/*! DIS_GATED_SLP_CLK - Disable Gated Sleep Clock */
#define APU_TIMER_PWR_MODE_DIS_GATED_SLP_CLK(x)  (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_DIS_GATED_SLP_CLK_SHIFT)) & APU_TIMER_PWR_MODE_DIS_GATED_SLP_CLK_MASK)

#define APU_TIMER_PWR_MODE_XP_OPT_EN_MASK        (0x1000U)
#define APU_TIMER_PWR_MODE_XP_OPT_EN_SHIFT       (12U)
/*! XP_OPT_EN - Set to 1 to enable the sleep FSM to look for wakeup req after power shutdown and before reference clock shutdown. */
#define APU_TIMER_PWR_MODE_XP_OPT_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_XP_OPT_EN_SHIFT)) & APU_TIMER_PWR_MODE_XP_OPT_EN_MASK)

#define APU_TIMER_PWR_MODE_DIS_APU_INTR_ON_WAKEUP_MASK (0x2000U)
#define APU_TIMER_PWR_MODE_DIS_APU_INTR_ON_WAKEUP_SHIFT (13U)
/*! DIS_APU_INTR_ON_WAKEUP - Disable APU Interrupt on Wakeup */
#define APU_TIMER_PWR_MODE_DIS_APU_INTR_ON_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_DIS_APU_INTR_ON_WAKEUP_SHIFT)) & APU_TIMER_PWR_MODE_DIS_APU_INTR_ON_WAKEUP_MASK)

#define APU_TIMER_PWR_MODE_SOC_XOSC_PWR_ON_MASK  (0x20000U)
#define APU_TIMER_PWR_MODE_SOC_XOSC_PWR_ON_SHIFT (17U)
/*! SOC_XOSC_PWR_ON - SoC XOSC Power on */
#define APU_TIMER_PWR_MODE_SOC_XOSC_PWR_ON(x)    (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_SOC_XOSC_PWR_ON_SHIFT)) & APU_TIMER_PWR_MODE_SOC_XOSC_PWR_ON_MASK)

#define APU_TIMER_PWR_MODE_ENA_FAST_WKUP_MASK    (0x40000U)
#define APU_TIMER_PWR_MODE_ENA_FAST_WKUP_SHIFT   (18U)
/*! ENA_FAST_WKUP - Enable Fast Wakeup */
#define APU_TIMER_PWR_MODE_ENA_FAST_WKUP(x)      (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_ENA_FAST_WKUP_SHIFT)) & APU_TIMER_PWR_MODE_ENA_FAST_WKUP_MASK)

#define APU_TIMER_PWR_MODE_GENERIC_TIMER_EN_MASK (0x200000U)
#define APU_TIMER_PWR_MODE_GENERIC_TIMER_EN_SHIFT (21U)
/*! GENERIC_TIMER_EN - Enable sleep timer to generate interrupt when value matches generic timer value. Debug only */
#define APU_TIMER_PWR_MODE_GENERIC_TIMER_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_GENERIC_TIMER_EN_SHIFT)) & APU_TIMER_PWR_MODE_GENERIC_TIMER_EN_MASK)

#define APU_TIMER_PWR_MODE_GENERIC_TIMER_BYPASS_MASK (0x400000U)
#define APU_TIMER_PWR_MODE_GENERIC_TIMER_BYPASS_SHIFT (22U)
/*! GENERIC_TIMER_BYPASS - When set to 1, generic_alarm_en2 will be used for generic_alarm_en1 */
#define APU_TIMER_PWR_MODE_GENERIC_TIMER_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_GENERIC_TIMER_BYPASS_SHIFT)) & APU_TIMER_PWR_MODE_GENERIC_TIMER_BYPASS_MASK)

#define APU_TIMER_PWR_MODE_XOSC_ON_WHILE_SLEEP_MASK (0x800000U)
#define APU_TIMER_PWR_MODE_XOSC_ON_WHILE_SLEEP_SHIFT (23U)
/*! XOSC_ON_WHILE_SLEEP - XOSC On While Sleep */
#define APU_TIMER_PWR_MODE_XOSC_ON_WHILE_SLEEP(x) (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_XOSC_ON_WHILE_SLEEP_SHIFT)) & APU_TIMER_PWR_MODE_XOSC_ON_WHILE_SLEEP_MASK)

#define APU_TIMER_PWR_MODE_USE_HSIC_SLP_SYNC_MASK (0x1000000U)
#define APU_TIMER_PWR_MODE_USE_HSIC_SLP_SYNC_SHIFT (24U)
/*! USE_HSIC_SLP_SYNC - Use HSIC Sleep Sync */
#define APU_TIMER_PWR_MODE_USE_HSIC_SLP_SYNC(x)  (((uint32_t)(((uint32_t)(x)) << APU_TIMER_PWR_MODE_USE_HSIC_SLP_SYNC_SHIFT)) & APU_TIMER_PWR_MODE_USE_HSIC_SLP_SYNC_MASK)
/*! @} */

/*! @name PWR_CTRL - Power Control */
/*! @{ */

#define APU_PWR_CTRL_APU_PD_CTRL_EN_MASK         (0x1U)
#define APU_PWR_CTRL_APU_PD_CTRL_EN_SHIFT        (0U)
/*! APU_PD_CTRL_EN - APU power domain control enable */
#define APU_PWR_CTRL_APU_PD_CTRL_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_APU_PD_CTRL_EN_SHIFT)) & APU_PWR_CTRL_APU_PD_CTRL_EN_MASK)

#define APU_PWR_CTRL_APU_SW_PD_EN_MASK           (0x2U)
#define APU_PWR_CTRL_APU_SW_PD_EN_SHIFT          (1U)
/*! APU_SW_PD_EN - APU switch power down enable */
#define APU_PWR_CTRL_APU_SW_PD_EN(x)             (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_APU_SW_PD_EN_SHIFT)) & APU_PWR_CTRL_APU_SW_PD_EN_MASK)

#define APU_PWR_CTRL_APU_UDR_FIREWALL_BAR_EN_MASK (0x4U)
#define APU_PWR_CTRL_APU_UDR_FIREWALL_BAR_EN_SHIFT (2U)
/*! APU_UDR_FIREWALL_BAR_EN - APU UDR enable */
#define APU_PWR_CTRL_APU_UDR_FIREWALL_BAR_EN(x)  (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_APU_UDR_FIREWALL_BAR_EN_SHIFT)) & APU_PWR_CTRL_APU_UDR_FIREWALL_BAR_EN_MASK)

#define APU_PWR_CTRL_APU_ISO_CELL_EN_MASK        (0x8U)
#define APU_PWR_CTRL_APU_ISO_CELL_EN_SHIFT       (3U)
/*! APU_ISO_CELL_EN - APU isolation enable */
#define APU_PWR_CTRL_APU_ISO_CELL_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_APU_ISO_CELL_EN_SHIFT)) & APU_PWR_CTRL_APU_ISO_CELL_EN_MASK)

#define APU_PWR_CTRL_DLY_UDR2ISO_MASK            (0x10U)
#define APU_PWR_CTRL_DLY_UDR2ISO_SHIFT           (4U)
/*! DLY_UDR2ISO - UDR firewall_bar to isolation delay enable */
#define APU_PWR_CTRL_DLY_UDR2ISO(x)              (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_DLY_UDR2ISO_SHIFT)) & APU_PWR_CTRL_DLY_UDR2ISO_MASK)

#define APU_PWR_CTRL_ISO_ENABLE_REFCK_MASK       (0x20U)
#define APU_PWR_CTRL_ISO_ENABLE_REFCK_SHIFT      (5U)
/*! ISO_ENABLE_REFCK - ISO Enable Reference Clock */
#define APU_PWR_CTRL_ISO_ENABLE_REFCK(x)         (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_ISO_ENABLE_REFCK_SHIFT)) & APU_PWR_CTRL_ISO_ENABLE_REFCK_MASK)

#define APU_PWR_CTRL_NON_UDR_RSTB_EN_MASK        (0x40U)
#define APU_PWR_CTRL_NON_UDR_RSTB_EN_SHIFT       (6U)
/*! NON_UDR_RSTB_EN - APU non-UDR reset enable */
#define APU_PWR_CTRL_NON_UDR_RSTB_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_NON_UDR_RSTB_EN_SHIFT)) & APU_PWR_CTRL_NON_UDR_RSTB_EN_MASK)

#define APU_PWR_CTRL_XOSC_OFF_DURING_POWER_OFF_MASK (0x80U)
#define APU_PWR_CTRL_XOSC_OFF_DURING_POWER_OFF_SHIFT (7U)
/*! XOSC_OFF_DURING_POWER_OFF - XOSC Off During Power Off */
#define APU_PWR_CTRL_XOSC_OFF_DURING_POWER_OFF(x) (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_XOSC_OFF_DURING_POWER_OFF_SHIFT)) & APU_PWR_CTRL_XOSC_OFF_DURING_POWER_OFF_MASK)

#define APU_PWR_CTRL_SRAM_PD_EN_MASK             (0x100U)
#define APU_PWR_CTRL_SRAM_PD_EN_SHIFT            (8U)
/*! SRAM_PD_EN - APU SRAM power down enable */
#define APU_PWR_CTRL_SRAM_PD_EN(x)               (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_SRAM_PD_EN_SHIFT)) & APU_PWR_CTRL_SRAM_PD_EN_MASK)

#define APU_PWR_CTRL_CLK_DIV_RESETB_ENABLE_MASK  (0x200U)
#define APU_PWR_CTRL_CLK_DIV_RESETB_ENABLE_SHIFT (9U)
/*! CLK_DIV_RESETB_ENABLE - CLK_DIV_RESETB Enable */
#define APU_PWR_CTRL_CLK_DIV_RESETB_ENABLE(x)    (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_CLK_DIV_RESETB_ENABLE_SHIFT)) & APU_PWR_CTRL_CLK_DIV_RESETB_ENABLE_MASK)

#define APU_PWR_CTRL_CLK_DIV_RESETB_REFCLK_MASK  (0x400U)
#define APU_PWR_CTRL_CLK_DIV_RESETB_REFCLK_SHIFT (10U)
/*! CLK_DIV_RESETB_REFCLK - CLK_DIV_RESETB Reference Clock */
#define APU_PWR_CTRL_CLK_DIV_RESETB_REFCLK(x)    (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_CLK_DIV_RESETB_REFCLK_SHIFT)) & APU_PWR_CTRL_CLK_DIV_RESETB_REFCLK_MASK)

#define APU_PWR_CTRL_SOC_USE_NOM_VOL_BYP_MASK    (0x800U)
#define APU_PWR_CTRL_SOC_USE_NOM_VOL_BYP_SHIFT   (11U)
/*! SOC_USE_NOM_VOL_BYP - SoC Use Nominal Vol Bypass */
#define APU_PWR_CTRL_SOC_USE_NOM_VOL_BYP(x)      (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_SOC_USE_NOM_VOL_BYP_SHIFT)) & APU_PWR_CTRL_SOC_USE_NOM_VOL_BYP_MASK)

#define APU_PWR_CTRL_SOC_USE_UNSYNC_VOL_LVL_MASK (0x1000U)
#define APU_PWR_CTRL_SOC_USE_UNSYNC_VOL_LVL_SHIFT (12U)
/*! SOC_USE_UNSYNC_VOL_LVL - SoC Use unSync Vol Level */
#define APU_PWR_CTRL_SOC_USE_UNSYNC_VOL_LVL(x)   (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_SOC_USE_UNSYNC_VOL_LVL_SHIFT)) & APU_PWR_CTRL_SOC_USE_UNSYNC_VOL_LVL_MASK)

#define APU_PWR_CTRL_RFU_IN_WLAN_DOMAIN_MASK     (0x2000U)
#define APU_PWR_CTRL_RFU_IN_WLAN_DOMAIN_SHIFT    (13U)
/*! RFU_IN_WLAN_DOMAIN - RFU WLAN mode */
#define APU_PWR_CTRL_RFU_IN_WLAN_DOMAIN(x)       (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_RFU_IN_WLAN_DOMAIN_SHIFT)) & APU_PWR_CTRL_RFU_IN_WLAN_DOMAIN_MASK)

#define APU_PWR_CTRL_DLY_NON_UDR_RSTB_MASK       (0x4000U)
#define APU_PWR_CTRL_DLY_NON_UDR_RSTB_SHIFT      (14U)
/*! DLY_NON_UDR_RSTB - Delay non-UDR RSTb */
#define APU_PWR_CTRL_DLY_NON_UDR_RSTB(x)         (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_DLY_NON_UDR_RSTB_SHIFT)) & APU_PWR_CTRL_DLY_NON_UDR_RSTB_MASK)

#define APU_PWR_CTRL_PCIE_P_REQ_MASK_FOR_SLPFSM_MASK (0x8000U)
#define APU_PWR_CTRL_PCIE_P_REQ_MASK_FOR_SLPFSM_SHIFT (15U)
/*! PCIE_P_REQ_MASK_FOR_SLPFSM - Mask pcie_p_req and pcie_xp_req for slp_fsm */
#define APU_PWR_CTRL_PCIE_P_REQ_MASK_FOR_SLPFSM(x) (((uint32_t)(((uint32_t)(x)) << APU_PWR_CTRL_PCIE_P_REQ_MASK_FOR_SLPFSM_SHIFT)) & APU_PWR_CTRL_PCIE_P_REQ_MASK_FOR_SLPFSM_MASK)
/*! @} */

/*! @name PLL_CTRL - PLL Control */
/*! @{ */

#define APU_PLL_CTRL_DIS_APU_PLL_EN0_MASK        (0x1U)
#define APU_PLL_CTRL_DIS_APU_PLL_EN0_SHIFT       (0U)
/*! DIS_APU_PLL_EN0 - When set to 1, apu_pll_en[0]=0 (pll0 is Tcpu in Skyhawk) */
#define APU_PLL_CTRL_DIS_APU_PLL_EN0(x)          (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_DIS_APU_PLL_EN0_SHIFT)) & APU_PLL_CTRL_DIS_APU_PLL_EN0_MASK)

#define APU_PLL_CTRL_DIS_APU_PLL_EN1_MASK        (0x2U)
#define APU_PLL_CTRL_DIS_APU_PLL_EN1_SHIFT       (1U)
/*! DIS_APU_PLL_EN1 - When set to 1, apu_pll_en[1]=0 (pll1 is Tsoc in Skyhawk) */
#define APU_PLL_CTRL_DIS_APU_PLL_EN1(x)          (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_DIS_APU_PLL_EN1_SHIFT)) & APU_PLL_CTRL_DIS_APU_PLL_EN1_MASK)

#define APU_PLL_CTRL_FW_USE_PLL0_MASK            (0x10U)
#define APU_PLL_CTRL_FW_USE_PLL0_SHIFT           (4U)
/*! FW_USE_PLL0 - When set to 1, FW controls pll_en[0] value (pll0 is Tcpu in Skyhawk) */
#define APU_PLL_CTRL_FW_USE_PLL0(x)              (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_FW_USE_PLL0_SHIFT)) & APU_PLL_CTRL_FW_USE_PLL0_MASK)

#define APU_PLL_CTRL_FW_PLL0_EN_SET_MASK         (0x20U)
#define APU_PLL_CTRL_FW_PLL0_EN_SET_SHIFT        (5U)
/*! FW_PLL0_EN_SET - Fw sets pll_en[0] value when apu_pll_ctrl[4] = 1, */
#define APU_PLL_CTRL_FW_PLL0_EN_SET(x)           (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_FW_PLL0_EN_SET_SHIFT)) & APU_PLL_CTRL_FW_PLL0_EN_SET_MASK)

#define APU_PLL_CTRL_FW_USE_PLL1_MASK            (0x100U)
#define APU_PLL_CTRL_FW_USE_PLL1_SHIFT           (8U)
/*! FW_USE_PLL1 - When set to 1, FW controls pll_en[1] value (pll0 is Tcpu in Skyhawk) */
#define APU_PLL_CTRL_FW_USE_PLL1(x)              (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_FW_USE_PLL1_SHIFT)) & APU_PLL_CTRL_FW_USE_PLL1_MASK)

#define APU_PLL_CTRL_FW_PLL1_EN_SET_MASK         (0x200U)
#define APU_PLL_CTRL_FW_PLL1_EN_SET_SHIFT        (9U)
/*! FW_PLL1_EN_SET - Fw sets pll_en[1] value when apu_pll_ctrl[4] = 1, */
#define APU_PLL_CTRL_FW_PLL1_EN_SET(x)           (((uint32_t)(((uint32_t)(x)) << APU_PLL_CTRL_FW_PLL1_EN_SET_SHIFT)) & APU_PLL_CTRL_FW_PLL1_EN_SET_MASK)
/*! @} */

/*! @name PLL_DIS_CNT - PLL Disable Count */
/*! @{ */

#define APU_PLL_DIS_CNT_PLL_DIS_CNT_MASK         (0xFFFFU)
#define APU_PLL_DIS_CNT_PLL_DIS_CNT_SHIFT        (0U)
/*! PLL_DIS_CNT - PLL disable count in reference clocks */
#define APU_PLL_DIS_CNT_PLL_DIS_CNT(x)           (((uint32_t)(((uint32_t)(x)) << APU_PLL_DIS_CNT_PLL_DIS_CNT_SHIFT)) & APU_PLL_DIS_CNT_PLL_DIS_CNT_MASK)
/*! @} */

/*! @name STABLE_CNT - Stable Count */
/*! @{ */

#define APU_STABLE_CNT_XOSC_STABLE_CNT_MASK      (0xFFFFU)
#define APU_STABLE_CNT_XOSC_STABLE_CNT_SHIFT     (0U)
/*! XOSC_STABLE_CNT - XOSC stable count in sleep clocks after APU asserts xosc_en */
#define APU_STABLE_CNT_XOSC_STABLE_CNT(x)        (((uint32_t)(((uint32_t)(x)) << APU_STABLE_CNT_XOSC_STABLE_CNT_SHIFT)) & APU_STABLE_CNT_XOSC_STABLE_CNT_MASK)

#define APU_STABLE_CNT_PLL_STABLE_CNT_MASK       (0xFFFF0000U)
#define APU_STABLE_CNT_PLL_STABLE_CNT_SHIFT      (16U)
/*! PLL_STABLE_CNT - PLL stable count in sleep clocks after APU asserts pll_en */
#define APU_STABLE_CNT_PLL_STABLE_CNT(x)         (((uint32_t)(((uint32_t)(x)) << APU_STABLE_CNT_PLL_STABLE_CNT_SHIFT)) & APU_STABLE_CNT_PLL_STABLE_CNT_MASK)
/*! @} */

/*! @name CPU1_HOST_WKUP_MASK - CPU1 Host Wakeup Mask */
/*! @{ */

#define APU_CPU1_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK (0xFFFFU)
#define APU_CPU1_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT (0U)
/*! HOST_WKUP_MASK - Host Wakeup Mask */
#define APU_CPU1_HOST_WKUP_MASK_HOST_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT)) & APU_CPU1_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK)
/*! @} */

/*! @name CPU1_HOST_WKUP_POL - CPU1 Host Wakeup Polarity */
/*! @{ */

#define APU_CPU1_HOST_WKUP_POL_HOST_WKUP_POL_MASK (0xFFFFU)
#define APU_CPU1_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT (0U)
/*! HOST_WKUP_POL - Host Wakeup Polarity */
#define APU_CPU1_HOST_WKUP_POL_HOST_WKUP_POL(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU1_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT)) & APU_CPU1_HOST_WKUP_POL_HOST_WKUP_POL_MASK)
/*! @} */

/*! @name CAL_WIN_CNT - Calibration Window Count */
/*! @{ */

#define APU_CAL_WIN_CNT_APU_CAL_WIN_CNT_MASK     (0xFFFFFFFFU)
#define APU_CAL_WIN_CNT_APU_CAL_WIN_CNT_SHIFT    (0U)
/*! APU_CAL_WIN_CNT - Number of calibration clocks elapsed during calibration window */
#define APU_CAL_WIN_CNT_APU_CAL_WIN_CNT(x)       (((uint32_t)(((uint32_t)(x)) << APU_CAL_WIN_CNT_APU_CAL_WIN_CNT_SHIFT)) & APU_CAL_WIN_CNT_APU_CAL_WIN_CNT_MASK)
/*! @} */

/*! @name AUTO_10US - Auto 10us */
/*! @{ */

#define APU_AUTO_10US_APU_AUTO_10US_MASK         (0xFFFFFFFFU)
#define APU_AUTO_10US_APU_AUTO_10US_SHIFT        (0U)
/*! APU_AUTO_10US - auto alarm value loaded during auto calibration */
#define APU_AUTO_10US_APU_AUTO_10US(x)           (((uint32_t)(((uint32_t)(x)) << APU_AUTO_10US_APU_AUTO_10US_SHIFT)) & APU_AUTO_10US_APU_AUTO_10US_MASK)
/*! @} */

/*! @name ACC_CNT - Accumulation Count */
/*! @{ */

#define APU_ACC_CNT_APU_ACC_CNT_MASK             (0xFFFFFFFFU)
#define APU_ACC_CNT_APU_ACC_CNT_SHIFT            (0U)
/*! APU_ACC_CNT - number of sleep clocks elapsed during the calibration window */
#define APU_ACC_CNT_APU_ACC_CNT(x)               (((uint32_t)(((uint32_t)(x)) << APU_ACC_CNT_APU_ACC_CNT_SHIFT)) & APU_ACC_CNT_APU_ACC_CNT_MASK)
/*! @} */

/*! @name CAL_WIN_SIZE - Calibration Window Size */
/*! @{ */

#define APU_CAL_WIN_SIZE_APU_CAL_WIN_SIZE_MASK   (0xFFFFFFFFU)
#define APU_CAL_WIN_SIZE_APU_CAL_WIN_SIZE_SHIFT  (0U)
/*! APU_CAL_WIN_SIZE - calibration window size in number of calibration clocks. */
#define APU_CAL_WIN_SIZE_APU_CAL_WIN_SIZE(x)     (((uint32_t)(((uint32_t)(x)) << APU_CAL_WIN_SIZE_APU_CAL_WIN_SIZE_SHIFT)) & APU_CAL_WIN_SIZE_APU_CAL_WIN_SIZE_MASK)
/*! @} */

/*! @name SLP_CLK_TIMER - Sleep Clock Timer */
/*! @{ */

#define APU_SLP_CLK_TIMER_APU_SLP_CLK_TIMER_MASK (0xFFFFFFFFU)
#define APU_SLP_CLK_TIMER_APU_SLP_CLK_TIMER_SHIFT (0U)
/*! APU_SLP_CLK_TIMER - reads back the current value of the free running sleep clock counter. */
#define APU_SLP_CLK_TIMER_APU_SLP_CLK_TIMER(x)   (((uint32_t)(((uint32_t)(x)) << APU_SLP_CLK_TIMER_APU_SLP_CLK_TIMER_SHIFT)) & APU_SLP_CLK_TIMER_APU_SLP_CLK_TIMER_MASK)
/*! @} */

/*! @name STATUS_RD - Status Read */
/*! @{ */

#define APU_STATUS_RD_CALIBRATION_STATUS_MASK    (0x1U)
#define APU_STATUS_RD_CALIBRATION_STATUS_SHIFT   (0U)
/*! CALIBRATION_STATUS - Calibration Status */
#define APU_STATUS_RD_CALIBRATION_STATUS(x)      (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_CALIBRATION_STATUS_SHIFT)) & APU_STATUS_RD_CALIBRATION_STATUS_MASK)

#define APU_STATUS_RD_APU_PLL_STABLE_0_MASK      (0x2U)
#define APU_STATUS_RD_APU_PLL_STABLE_0_SHIFT     (1U)
/*! APU_PLL_STABLE_0 - T1 PLL stable */
#define APU_STATUS_RD_APU_PLL_STABLE_0(x)        (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_PLL_STABLE_0_SHIFT)) & APU_STATUS_RD_APU_PLL_STABLE_0_MASK)

#define APU_STATUS_RD_APU_PLL_STABLE_1_MASK      (0x4U)
#define APU_STATUS_RD_APU_PLL_STABLE_1_SHIFT     (2U)
/*! APU_PLL_STABLE_1 - APU PLL Stable[1] */
#define APU_STATUS_RD_APU_PLL_STABLE_1(x)        (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_PLL_STABLE_1_SHIFT)) & APU_STATUS_RD_APU_PLL_STABLE_1_MASK)

#define APU_STATUS_RD_APU_XOSC_STABLE_MASK       (0x8U)
#define APU_STATUS_RD_APU_XOSC_STABLE_SHIFT      (3U)
/*! APU_XOSC_STABLE - APU XOSC Stable */
#define APU_STATUS_RD_APU_XOSC_STABLE(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_XOSC_STABLE_SHIFT)) & APU_STATUS_RD_APU_XOSC_STABLE_MASK)

#define APU_STATUS_RD_APU_BT2_CLK_EN_MASK        (0x10U)
#define APU_STATUS_RD_APU_BT2_CLK_EN_SHIFT       (4U)
/*! APU_BT2_CLK_EN - APU Bluetooth2 Clock Enable */
#define APU_STATUS_RD_APU_BT2_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_BT2_CLK_EN_SHIFT)) & APU_STATUS_RD_APU_BT2_CLK_EN_MASK)

#define APU_STATUS_RD_APU_MAC_CLK_EN2_MASK       (0x20U)
#define APU_STATUS_RD_APU_MAC_CLK_EN2_SHIFT      (5U)
/*! APU_MAC_CLK_EN2 - APU MAC Clock Enable 2 */
#define APU_STATUS_RD_APU_MAC_CLK_EN2(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_MAC_CLK_EN2_SHIFT)) & APU_STATUS_RD_APU_MAC_CLK_EN2_MASK)

#define APU_STATUS_RD_APU_BT_CLK_EN_MASK         (0x40U)
#define APU_STATUS_RD_APU_BT_CLK_EN_SHIFT        (6U)
/*! APU_BT_CLK_EN - APU Bluetooth Clock Enable */
#define APU_STATUS_RD_APU_BT_CLK_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_BT_CLK_EN_SHIFT)) & APU_STATUS_RD_APU_BT_CLK_EN_MASK)

#define APU_STATUS_RD_APU_MAC_CLK_EN1_MASK       (0x80U)
#define APU_STATUS_RD_APU_MAC_CLK_EN1_SHIFT      (7U)
/*! APU_MAC_CLK_EN1 - APU MAC Clock Enable 1 */
#define APU_STATUS_RD_APU_MAC_CLK_EN1(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_MAC_CLK_EN1_SHIFT)) & APU_STATUS_RD_APU_MAC_CLK_EN1_MASK)

#define APU_STATUS_RD_APU_SYS_CLK_EN_MASK        (0x100U)
#define APU_STATUS_RD_APU_SYS_CLK_EN_SHIFT       (8U)
/*! APU_SYS_CLK_EN - APU System Clock Enable */
#define APU_STATUS_RD_APU_SYS_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_SYS_CLK_EN_SHIFT)) & APU_STATUS_RD_APU_SYS_CLK_EN_MASK)

#define APU_STATUS_RD_APU_SOC_CLK_EN3_MASK       (0x200U)
#define APU_STATUS_RD_APU_SOC_CLK_EN3_SHIFT      (9U)
/*! APU_SOC_CLK_EN3 - APU SoC Clock Enable 3 */
#define APU_STATUS_RD_APU_SOC_CLK_EN3(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_SOC_CLK_EN3_SHIFT)) & APU_STATUS_RD_APU_SOC_CLK_EN3_MASK)

#define APU_STATUS_RD_APU_SOC_CLK_EN2_MASK       (0x400U)
#define APU_STATUS_RD_APU_SOC_CLK_EN2_SHIFT      (10U)
/*! APU_SOC_CLK_EN2 - APU SoC Clock Enable 2 */
#define APU_STATUS_RD_APU_SOC_CLK_EN2(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_SOC_CLK_EN2_SHIFT)) & APU_STATUS_RD_APU_SOC_CLK_EN2_MASK)

#define APU_STATUS_RD_APU_SOC_CLK_EN1_MASK       (0x800U)
#define APU_STATUS_RD_APU_SOC_CLK_EN1_SHIFT      (11U)
/*! APU_SOC_CLK_EN1 - APU SoC Clock Enable 1 */
#define APU_STATUS_RD_APU_SOC_CLK_EN1(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_SOC_CLK_EN1_SHIFT)) & APU_STATUS_RD_APU_SOC_CLK_EN1_MASK)

#define APU_STATUS_RD_APU_SLEEP_FSM_STATE_MASK   (0xF000U)
#define APU_STATUS_RD_APU_SLEEP_FSM_STATE_SHIFT  (12U)
/*! APU_SLEEP_FSM_STATE - APU Sleep FSM State */
#define APU_STATUS_RD_APU_SLEEP_FSM_STATE(x)     (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_APU_SLEEP_FSM_STATE_SHIFT)) & APU_STATUS_RD_APU_SLEEP_FSM_STATE_MASK)

#define APU_STATUS_RD_HOST_WKUP_AFTER_MASK_MASK  (0xFFFF0000U)
#define APU_STATUS_RD_HOST_WKUP_AFTER_MASK_SHIFT (16U)
/*! HOST_WKUP_AFTER_MASK - Host Wakeup After Mask */
#define APU_STATUS_RD_HOST_WKUP_AFTER_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_STATUS_RD_HOST_WKUP_AFTER_MASK_SHIFT)) & APU_STATUS_RD_HOST_WKUP_AFTER_MASK_MASK)
/*! @} */

/*! @name STABLE_CNT2 - Stable Count 2 */
/*! @{ */

#define APU_STABLE_CNT2_PLL2_STBL_CNT_MASK       (0xFFFFU)
#define APU_STABLE_CNT2_PLL2_STBL_CNT_SHIFT      (0U)
/*! PLL2_STBL_CNT - T3 stable count in reference clocks */
#define APU_STABLE_CNT2_PLL2_STBL_CNT(x)         (((uint32_t)(((uint32_t)(x)) << APU_STABLE_CNT2_PLL2_STBL_CNT_SHIFT)) & APU_STABLE_CNT2_PLL2_STBL_CNT_MASK)
/*! @} */

/*! @name DYN_PLL_MASK - Dynamic PLL Mask */
/*! @{ */

#define APU_DYN_PLL_MASK_BT_PLL_REQ_MASK_MASK    (0x20U)
#define APU_DYN_PLL_MASK_BT_PLL_REQ_MASK_SHIFT   (5U)
/*! BT_PLL_REQ_MASK - When mask =0, bt_pll_req treated as pll_req */
#define APU_DYN_PLL_MASK_BT_PLL_REQ_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BT_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BT_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_BRF_PLL_REQ_MASK_MASK   (0x40U)
#define APU_DYN_PLL_MASK_BRF_PLL_REQ_MASK_SHIFT  (6U)
/*! BRF_PLL_REQ_MASK - When mask =0, brf_pll_req treated as pll_req */
#define APU_DYN_PLL_MASK_BRF_PLL_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BRF_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BRF_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_BT_AES_CLK_REQ_MASK_MASK (0x80U)
#define APU_DYN_PLL_MASK_BT_AES_CLK_REQ_MASK_SHIFT (7U)
/*! BT_AES_CLK_REQ_MASK - When mask is 0, bt_aes_clk_req is treated as pll_req of bt_aes_nco_mode =0 */
#define APU_DYN_PLL_MASK_BT_AES_CLK_REQ_MASK(x)  (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BT_AES_CLK_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BT_AES_CLK_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_BT_CLK_REQ_PLL_REQ_MASK_MASK (0x100U)
#define APU_DYN_PLL_MASK_BT_CLK_REQ_PLL_REQ_MASK_SHIFT (8U)
/*! BT_CLK_REQ_PLL_REQ_MASK - When mask is 0, bt_clk_req is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_BT_CLK_REQ_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BT_CLK_REQ_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BT_CLK_REQ_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_BT_SLP_RDY_PLL_REQ_MASK_MASK (0x200U)
#define APU_DYN_PLL_MASK_BT_SLP_RDY_PLL_REQ_MASK_SHIFT (9U)
/*! BT_SLP_RDY_PLL_REQ_MASK - When mask is 0, !bt_slp_rdy is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_BT_SLP_RDY_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BT_SLP_RDY_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BT_SLP_RDY_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_BLE_SLP_RDY_PLL_REQ_MASK_MASK (0x400U)
#define APU_DYN_PLL_MASK_BLE_SLP_RDY_PLL_REQ_MASK_SHIFT (10U)
/*! BLE_SLP_RDY_PLL_REQ_MASK - When mask is 0, !ble_slp_rdy is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_BLE_SLP_RDY_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_BLE_SLP_RDY_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_BLE_SLP_RDY_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_CPU2_CP15_PLL_REQ_MASK_MASK (0x800U)
#define APU_DYN_PLL_MASK_CPU2_CP15_PLL_REQ_MASK_SHIFT (11U)
/*! CPU2_CP15_PLL_REQ_MASK - When mask is 0, !cpu2_cp15_sleep is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_CPU2_CP15_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_CPU2_CP15_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_CPU2_CP15_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_SD_CLK_SWITCH_OK_MASK_MASK (0x1000U)
#define APU_DYN_PLL_MASK_SD_CLK_SWITCH_OK_MASK_SHIFT (12U)
/*! SD_CLK_SWITCH_OK_MASK - SD Clock Switch Ok Mask */
#define APU_DYN_PLL_MASK_SD_CLK_SWITCH_OK_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_SD_CLK_SWITCH_OK_MASK_SHIFT)) & APU_DYN_PLL_MASK_SD_CLK_SWITCH_OK_MASK_MASK)

#define APU_DYN_PLL_MASK_GEN_TIMER_WKUP_PLL_REQ_MASK_MASK (0x2000U)
#define APU_DYN_PLL_MASK_GEN_TIMER_WKUP_PLL_REQ_MASK_SHIFT (13U)
/*! GEN_TIMER_WKUP_PLL_REQ_MASK - When mask is 0, gen_timer_wkup_wkup is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_GEN_TIMER_WKUP_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_GEN_TIMER_WKUP_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_GEN_TIMER_WKUP_PLL_REQ_MASK_MASK)

#define APU_DYN_PLL_MASK_FULL_SLP_MASK_MASK      (0x4000U)
#define APU_DYN_PLL_MASK_FULL_SLP_MASK_SHIFT     (14U)
/*! FULL_SLP_MASK - Full Sleep Mask */
#define APU_DYN_PLL_MASK_FULL_SLP_MASK(x)        (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_FULL_SLP_MASK_SHIFT)) & APU_DYN_PLL_MASK_FULL_SLP_MASK_MASK)

#define APU_DYN_PLL_MASK_HOST_WKUP_PLL_REQ_MASK_MASK (0xFFFF0000U)
#define APU_DYN_PLL_MASK_HOST_WKUP_PLL_REQ_MASK_SHIFT (16U)
/*! HOST_WKUP_PLL_REQ_MASK - When mask is 0, host_wkup is treated as pll_req if soc_use_ref_only = 0 */
#define APU_DYN_PLL_MASK_HOST_WKUP_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_MASK_HOST_WKUP_PLL_REQ_MASK_SHIFT)) & APU_DYN_PLL_MASK_HOST_WKUP_PLL_REQ_MASK_MASK)
/*! @} */

/*! @name TESTBUS_RD1 - Testbus Read 1 */
/*! @{ */

#define APU_TESTBUS_RD1_TIMER_CNTR_MASK          (0xFFFFU)
#define APU_TESTBUS_RD1_TIMER_CNTR_SHIFT         (0U)
/*! TIMER_CNTR - DVFS internal counter */
#define APU_TESTBUS_RD1_TIMER_CNTR(x)            (((uint32_t)(((uint32_t)(x)) << APU_TESTBUS_RD1_TIMER_CNTR_SHIFT)) & APU_TESTBUS_RD1_TIMER_CNTR_MASK)

#define APU_TESTBUS_RD1_DVFS_TIMER_MASK          (0xFFFF0000U)
#define APU_TESTBUS_RD1_DVFS_TIMER_SHIFT         (16U)
/*! DVFS_TIMER - DVFS timer */
#define APU_TESTBUS_RD1_DVFS_TIMER(x)            (((uint32_t)(((uint32_t)(x)) << APU_TESTBUS_RD1_DVFS_TIMER_SHIFT)) & APU_TESTBUS_RD1_DVFS_TIMER_MASK)
/*! @} */

/*! @name TESTBUS_RD2 - Testbus Read 2 */
/*! @{ */

#define APU_TESTBUS_RD2_APU_TESTBUS_RD2_MASK     (0xFFFFFFFFU)
#define APU_TESTBUS_RD2_APU_TESTBUS_RD2_SHIFT    (0U)
/*! APU_TESTBUS_RD2 - Last 8 FSM state changes are stored in the register */
#define APU_TESTBUS_RD2_APU_TESTBUS_RD2(x)       (((uint32_t)(((uint32_t)(x)) << APU_TESTBUS_RD2_APU_TESTBUS_RD2_SHIFT)) & APU_TESTBUS_RD2_APU_TESTBUS_RD2_MASK)
/*! @} */

/*! @name GENERIC_TIMER_CNT - Generic Timer Count */
/*! @{ */

#define APU_GENERIC_TIMER_CNT_GENERIC_TIMER_CNT_MASK (0xFFFFFFFFU)
#define APU_GENERIC_TIMER_CNT_GENERIC_TIMER_CNT_SHIFT (0U)
/*! GENERIC_TIMER_CNT - Generic Timer Count */
#define APU_GENERIC_TIMER_CNT_GENERIC_TIMER_CNT(x) (((uint32_t)(((uint32_t)(x)) << APU_GENERIC_TIMER_CNT_GENERIC_TIMER_CNT_SHIFT)) & APU_GENERIC_TIMER_CNT_GENERIC_TIMER_CNT_MASK)
/*! @} */

/*! @name CPU1_HOST_WKUP_CTRL - CPU1 Host Wakeup Control */
/*! @{ */

#define APU_CPU1_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK (0xFFFFU)
#define APU_CPU1_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT (0U)
/*! HOST_WKUP_CTRL - Host Wakeup Control */
#define APU_CPU1_HOST_WKUP_CTRL_HOST_WKUP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT)) & APU_CPU1_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK)

#define APU_CPU1_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK (0xFFFF0000U)
#define APU_CPU1_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT (16U)
/*! HOST_INTR_MASK - Host Interrupt Mask */
#define APU_CPU1_HOST_WKUP_CTRL_HOST_INTR_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT)) & APU_CPU1_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK)
/*! @} */

/*! @name DYN_PLL_CLK_EXT_CTRL - Dynamic PLL Clock Ext Control */
/*! @{ */

#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK1_EXT_EN_MASK (0x1U)
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK1_EXT_EN_SHIFT (0U)
/*! SOC_CLK1_EXT_EN - SoC Clock 1 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK1_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK1_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK1_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK2_EXT_EN_MASK (0x2U)
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK2_EXT_EN_SHIFT (1U)
/*! SOC_CLK2_EXT_EN - SoC Clock 2 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK2_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK2_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK2_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK3_EXT_EN_MASK (0x4U)
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK3_EXT_EN_SHIFT (2U)
/*! SOC_CLK3_EXT_EN - SoC Clock 3 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK3_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK3_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_SOC_CLK3_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_PLL_EXT_EN_MASK (0x10U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_PLL_EXT_EN_SHIFT (4U)
/*! CPU1_PLL_EXT_EN - CPU1 PLL Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_PLL_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU1_PLL_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU1_PLL_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_INACTIVE_EXT_EN_MASK (0x20U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_INACTIVE_EXT_EN_SHIFT (5U)
/*! CPU1_INACTIVE_EXT_EN - CPU1 Inactive Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU1_INACTIVE_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU1_INACTIVE_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU1_INACTIVE_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_PLL_EXT_EN_MASK (0x40U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_PLL_EXT_EN_SHIFT (6U)
/*! CPU2_PLL_EXT_EN - CPU2 PLL Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_PLL_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU2_PLL_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU2_PLL_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_INACTIVE_EXT_EN_MASK (0x80U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_INACTIVE_EXT_EN_SHIFT (7U)
/*! CPU2_INACTIVE_EXT_EN - CPU2 Inactive Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU2_INACTIVE_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU2_INACTIVE_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU2_INACTIVE_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_T1_EXT_EN_MASK  (0x100U)
#define APU_DYN_PLL_CLK_EXT_CTRL_T1_EXT_EN_SHIFT (8U)
/*! T1_EXT_EN - T1 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_T1_EXT_EN(x)    (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_T1_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_T1_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_T3_EXT_EN_MASK  (0x200U)
#define APU_DYN_PLL_CLK_EXT_CTRL_T3_EXT_EN_SHIFT (9U)
/*! T3_EXT_EN - T3 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_T3_EXT_EN(x)    (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_T3_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_T3_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_DVFS_T1_EXT_EN_MASK (0x400U)
#define APU_DYN_PLL_CLK_EXT_CTRL_DVFS_T1_EXT_EN_SHIFT (10U)
/*! DVFS_T1_EXT_EN - DVFS T1 Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_DVFS_T1_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_DVFS_T1_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_DVFS_T1_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_PLL_EXT_EN_MASK (0x800U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_PLL_EXT_EN_SHIFT (11U)
/*! CPU3_PLL_EXT_EN - CPU3 PLL Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_PLL_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU3_PLL_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU3_PLL_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_INACTIVE_EXT_EN_MASK (0x1000U)
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_INACTIVE_EXT_EN_SHIFT (12U)
/*! CPU3_INACTIVE_EXT_EN - CPU3 Inactive Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_CPU3_INACTIVE_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_CPU3_INACTIVE_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_CPU3_INACTIVE_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_CLK_EXT_EN_MASK (0x10000000U)
#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_CLK_EXT_EN_SHIFT (28U)
/*! BT2_AES_CLK_EXT_EN - BLuetooth2 AES Clock Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_CLK_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_CLK_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_CLK_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_PLL_EXT_EN_MASK (0x20000000U)
#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_PLL_EXT_EN_SHIFT (29U)
/*! BT2_AES_PLL_EXT_EN - Bluetooth2 AES PLL Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_PLL_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_PLL_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_BT2_AES_PLL_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_CLK_EXT_EN_MASK (0x40000000U)
#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_CLK_EXT_EN_SHIFT (30U)
/*! BT_AES_CLK_EXT_EN - BLuetooth AES Clock Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_CLK_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_CLK_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_CLK_EXT_EN_MASK)

#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_PLL_EXT_EN_MASK (0x80000000U)
#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_PLL_EXT_EN_SHIFT (31U)
/*! BT_AES_PLL_EXT_EN - Bluetooth AES PLL Ext Enable */
#define APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_PLL_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_PLL_EXT_EN_SHIFT)) & APU_DYN_PLL_CLK_EXT_CTRL_BT_AES_PLL_EXT_EN_MASK)
/*! @} */

/*! @name GENERIC_SLP_START_VAL - Generic Sleep Start Value */
/*! @{ */

#define APU_GENERIC_SLP_START_VAL_GENERIC_SLP_START_VAL_MASK (0xFFFFFFFFU)
#define APU_GENERIC_SLP_START_VAL_GENERIC_SLP_START_VAL_SHIFT (0U)
/*! GENERIC_SLP_START_VAL - Generic Sleep Start Value */
#define APU_GENERIC_SLP_START_VAL_GENERIC_SLP_START_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_GENERIC_SLP_START_VAL_GENERIC_SLP_START_VAL_SHIFT)) & APU_GENERIC_SLP_START_VAL_GENERIC_SLP_START_VAL_MASK)
/*! @} */

/*! @name DLY_HOST_CTRL - Delay Host Control */
/*! @{ */

#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_EN_MASK  (0x1U)
#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_EN_SHIFT (0U)
/*! DLY_HOST_WKUP_EN - Delay Host Wakeup Enable */
#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_EN(x)    (((uint32_t)(((uint32_t)(x)) << APU_DLY_HOST_CTRL_DLY_HOST_WKUP_EN_SHIFT)) & APU_DLY_HOST_CTRL_DLY_HOST_WKUP_EN_MASK)

#define APU_DLY_HOST_CTRL_HOST_WKUP_SEL_MASK     (0x78U)
#define APU_DLY_HOST_CTRL_HOST_WKUP_SEL_SHIFT    (3U)
/*! HOST_WKUP_SEL - selects which host wakeup to be delayed out of 16 hosts */
#define APU_DLY_HOST_CTRL_HOST_WKUP_SEL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DLY_HOST_CTRL_HOST_WKUP_SEL_SHIFT)) & APU_DLY_HOST_CTRL_HOST_WKUP_SEL_MASK)

#define APU_DLY_HOST_CTRL_RST_HOST_WKUP_CNT_MASK (0x80U)
#define APU_DLY_HOST_CTRL_RST_HOST_WKUP_CNT_SHIFT (7U)
/*! RST_HOST_WKUP_CNT - Reset Host Wakeup Count */
#define APU_DLY_HOST_CTRL_RST_HOST_WKUP_CNT(x)   (((uint32_t)(((uint32_t)(x)) << APU_DLY_HOST_CTRL_RST_HOST_WKUP_CNT_SHIFT)) & APU_DLY_HOST_CTRL_RST_HOST_WKUP_CNT_MASK)

#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_CNT_MASK (0xFFFFFF00U)
#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_CNT_SHIFT (8U)
/*! DLY_HOST_WKUP_CNT - Number of sleep clocks to delay host wakeup interrupt */
#define APU_DLY_HOST_CTRL_DLY_HOST_WKUP_CNT(x)   (((uint32_t)(((uint32_t)(x)) << APU_DLY_HOST_CTRL_DLY_HOST_WKUP_CNT_SHIFT)) & APU_DLY_HOST_CTRL_DLY_HOST_WKUP_CNT_MASK)
/*! @} */

/*! @name HOST_WKUP_CNT - Host Wakeup Count */
/*! @{ */

#define APU_HOST_WKUP_CNT_HOST_WKUP_DLY_CNT_MASK (0xFFFFFFU)
#define APU_HOST_WKUP_CNT_HOST_WKUP_DLY_CNT_SHIFT (0U)
/*! HOST_WKUP_DLY_CNT - Host Wakeup Delay Count */
#define APU_HOST_WKUP_CNT_HOST_WKUP_DLY_CNT(x)   (((uint32_t)(((uint32_t)(x)) << APU_HOST_WKUP_CNT_HOST_WKUP_DLY_CNT_SHIFT)) & APU_HOST_WKUP_CNT_HOST_WKUP_DLY_CNT_MASK)
/*! @} */

/*! @name CPU2_HOST_WKUP_MASK - CPU2 Host Wakeup Mask */
/*! @{ */

#define APU_CPU2_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK (0xFFFFU)
#define APU_CPU2_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT (0U)
/*! HOST_WKUP_MASK - Host Wakeup Mask */
#define APU_CPU2_HOST_WKUP_MASK_HOST_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT)) & APU_CPU2_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK)
/*! @} */

/*! @name CPU2_HOST_WKUP_POL - CPU2 Host Wakeup Polarity */
/*! @{ */

#define APU_CPU2_HOST_WKUP_POL_HOST_WKUP_POL_MASK (0xFFFFU)
#define APU_CPU2_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT (0U)
/*! HOST_WKUP_POL - Host Wakeup Polarity */
#define APU_CPU2_HOST_WKUP_POL_HOST_WKUP_POL(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU2_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT)) & APU_CPU2_HOST_WKUP_POL_HOST_WKUP_POL_MASK)
/*! @} */

/*! @name CPU2_HOST_WKUP_CTRL - CPU2 Host Wakeup Control */
/*! @{ */

#define APU_CPU2_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK (0xFFFFU)
#define APU_CPU2_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT (0U)
/*! HOST_WKUP_CTRL - Host Wakeup Control */
#define APU_CPU2_HOST_WKUP_CTRL_HOST_WKUP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT)) & APU_CPU2_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK)

#define APU_CPU2_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK (0xFFFF0000U)
#define APU_CPU2_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT (16U)
/*! HOST_INTR_MASK - Host Interrupt Mask */
#define APU_CPU2_HOST_WKUP_CTRL_HOST_INTR_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT)) & APU_CPU2_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK)
/*! @} */

/*! @name CPU2_CTRL - CPU2 Control */
/*! @{ */

#define APU_CPU2_CTRL_CPU2_MSG_RDY_MASK_MASK     (0x1U)
#define APU_CPU2_CTRL_CPU2_MSG_RDY_MASK_SHIFT    (0U)
/*! CPU2_MSG_RDY_MASK - CPU2 Message Ready Mask */
#define APU_CPU2_CTRL_CPU2_MSG_RDY_MASK(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_CPU2_MSG_RDY_MASK_SHIFT)) & APU_CPU2_CTRL_CPU2_MSG_RDY_MASK_MASK)

#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_VAL_MASK (0x4U)
#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_VAL_SHIFT (2U)
/*! CPU2_CP15_SLP_BYPASS_VAL - CPU2 CP15 Sleep Bypass Value */
#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_VAL_SHIFT)) & APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_VAL_MASK)

#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_EN_MASK (0x8U)
#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_EN_SHIFT (3U)
/*! CPU2_CP15_SLP_BYPASS_EN - CPU2 CP15 Sleep Bypass Enable */
#define APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_EN_SHIFT)) & APU_CPU2_CTRL_CPU2_CP15_SLP_BYPASS_EN_MASK)

#define APU_CPU2_CTRL_SOC_USE_REF_ONLY_MASK      (0x10U)
#define APU_CPU2_CTRL_SOC_USE_REF_ONLY_SHIFT     (4U)
/*! SOC_USE_REF_ONLY - SoC Use Ref Only */
#define APU_CPU2_CTRL_SOC_USE_REF_ONLY(x)        (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_SOC_USE_REF_ONLY_SHIFT)) & APU_CPU2_CTRL_SOC_USE_REF_ONLY_MASK)

#define APU_CPU2_CTRL_GENERIC_TIMER_EN2_MASK     (0x20U)
#define APU_CPU2_CTRL_GENERIC_TIMER_EN2_SHIFT    (5U)
/*! GENERIC_TIMER_EN2 - Generic Timer Enable 2 */
#define APU_CPU2_CTRL_GENERIC_TIMER_EN2(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_GENERIC_TIMER_EN2_SHIFT)) & APU_CPU2_CTRL_GENERIC_TIMER_EN2_MASK)

#define APU_CPU2_CTRL_APU_SUBSYS2_HOST_MASK      (0x70000U)
#define APU_CPU2_CTRL_APU_SUBSYS2_HOST_SHIFT     (16U)
/*! APU_SUBSYS2_HOST - APU Subsystem 2 Host */
#define APU_CPU2_CTRL_APU_SUBSYS2_HOST(x)        (((uint32_t)(((uint32_t)(x)) << APU_CPU2_CTRL_APU_SUBSYS2_HOST_SHIFT)) & APU_CPU2_CTRL_APU_SUBSYS2_HOST_MASK)
/*! @} */

/*! @name WLAN_PWR_CTRL_DLY - Wlan domain FSM Power Control Delay */
/*! @{ */

#define APU_WLAN_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK  (0xFFFFFFFFU)
#define APU_WLAN_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT (0U)
/*! PWR_CTRL_DLY - Wlan domain FSM Power Control Delay */
#define APU_WLAN_PWR_CTRL_DLY_PWR_CTRL_DLY(x)    (((uint32_t)(((uint32_t)(x)) << APU_WLAN_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT)) & APU_WLAN_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK)
/*! @} */

/*! @name STATUS2 - Status 2 */
/*! @{ */

#define APU_STATUS2_CPU1_MSG_RDY_INT_MASK        (0x1U)
#define APU_STATUS2_CPU1_MSG_RDY_INT_SHIFT       (0U)
/*! CPU1_MSG_RDY_INT - CPU1 Message Ready */
#define APU_STATUS2_CPU1_MSG_RDY_INT(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_CPU1_MSG_RDY_INT_SHIFT)) & APU_STATUS2_CPU1_MSG_RDY_INT_MASK)

#define APU_STATUS2_CPU2_MSG_RDY_INT_MASK        (0x2U)
#define APU_STATUS2_CPU2_MSG_RDY_INT_SHIFT       (1U)
/*! CPU2_MSG_RDY_INT - CPU2 Message Ready */
#define APU_STATUS2_CPU2_MSG_RDY_INT(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_CPU2_MSG_RDY_INT_SHIFT)) & APU_STATUS2_CPU2_MSG_RDY_INT_MASK)

#define APU_STATUS2_CPU3_MSG_RDY_INT_MASK        (0x4U)
#define APU_STATUS2_CPU3_MSG_RDY_INT_SHIFT       (2U)
/*! CPU3_MSG_RDY_INT - CPU3 Message Ready */
#define APU_STATUS2_CPU3_MSG_RDY_INT(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_CPU3_MSG_RDY_INT_SHIFT)) & APU_STATUS2_CPU3_MSG_RDY_INT_MASK)

#define APU_STATUS2_LMU_G2BIST_FINISH_MASK       (0x8U)
#define APU_STATUS2_LMU_G2BIST_FINISH_SHIFT      (3U)
/*! LMU_G2BIST_FINISH - LMU G2BIST finish */
#define APU_STATUS2_LMU_G2BIST_FINISH(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_LMU_G2BIST_FINISH_SHIFT)) & APU_STATUS2_LMU_G2BIST_FINISH_MASK)

#define APU_STATUS2_APU_BT_AES_CLK_EN_MASK       (0x10U)
#define APU_STATUS2_APU_BT_AES_CLK_EN_SHIFT      (4U)
/*! APU_BT_AES_CLK_EN - BT AES Clk enable */
#define APU_STATUS2_APU_BT_AES_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BT_AES_CLK_EN_SHIFT)) & APU_STATUS2_APU_BT_AES_CLK_EN_MASK)

#define APU_STATUS2_APU_BT2_AES_CLK_EN_MASK      (0x20U)
#define APU_STATUS2_APU_BT2_AES_CLK_EN_SHIFT     (5U)
/*! APU_BT2_AES_CLK_EN - BT2 AES Clk enable */
#define APU_STATUS2_APU_BT2_AES_CLK_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BT2_AES_CLK_EN_SHIFT)) & APU_STATUS2_APU_BT2_AES_CLK_EN_MASK)

#define APU_STATUS2_APU_BT_AES_CLK_SEL_MASK      (0x40U)
#define APU_STATUS2_APU_BT_AES_CLK_SEL_SHIFT     (6U)
/*! APU_BT_AES_CLK_SEL - BT AES clk select */
#define APU_STATUS2_APU_BT_AES_CLK_SEL(x)        (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BT_AES_CLK_SEL_SHIFT)) & APU_STATUS2_APU_BT_AES_CLK_SEL_MASK)

#define APU_STATUS2_APU_BT2_AES_CLK_SEL_MASK     (0x80U)
#define APU_STATUS2_APU_BT2_AES_CLK_SEL_SHIFT    (7U)
/*! APU_BT2_AES_CLK_SEL - BT2 AES clk select */
#define APU_STATUS2_APU_BT2_AES_CLK_SEL(x)       (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BT2_AES_CLK_SEL_SHIFT)) & APU_STATUS2_APU_BT2_AES_CLK_SEL_MASK)

#define APU_STATUS2_APU_BRF_CLK_EN_MASK          (0x100U)
#define APU_STATUS2_APU_BRF_CLK_EN_SHIFT         (8U)
/*! APU_BRF_CLK_EN - BRF clk enable */
#define APU_STATUS2_APU_BRF_CLK_EN(x)            (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BRF_CLK_EN_SHIFT)) & APU_STATUS2_APU_BRF_CLK_EN_MASK)

#define APU_STATUS2_APU_BRF2_CLK_EN_MASK         (0x200U)
#define APU_STATUS2_APU_BRF2_CLK_EN_SHIFT        (9U)
/*! APU_BRF2_CLK_EN - BRF2 clk enable */
#define APU_STATUS2_APU_BRF2_CLK_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BRF2_CLK_EN_SHIFT)) & APU_STATUS2_APU_BRF2_CLK_EN_MASK)

#define APU_STATUS2_APU_INTR_BT_WAKEUP_MASK      (0x400U)
#define APU_STATUS2_APU_INTR_BT_WAKEUP_SHIFT     (10U)
/*! APU_INTR_BT_WAKEUP - APU BT2 wakeup interrupt */
#define APU_STATUS2_APU_INTR_BT_WAKEUP(x)        (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_INTR_BT_WAKEUP_SHIFT)) & APU_STATUS2_APU_INTR_BT_WAKEUP_MASK)

#define APU_STATUS2_APU_INTR_BT2_WAKEUP_MASK     (0x800U)
#define APU_STATUS2_APU_INTR_BT2_WAKEUP_SHIFT    (11U)
/*! APU_INTR_BT2_WAKEUP - APU BT2 wakeup interrupt */
#define APU_STATUS2_APU_INTR_BT2_WAKEUP(x)       (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_INTR_BT2_WAKEUP_SHIFT)) & APU_STATUS2_APU_INTR_BT2_WAKEUP_MASK)

#define APU_STATUS2_APU_BBUD_CLK_EN1_MASK        (0x1000U)
#define APU_STATUS2_APU_BBUD_CLK_EN1_SHIFT       (12U)
/*! APU_BBUD_CLK_EN1 - bbud2 clk enable */
#define APU_STATUS2_APU_BBUD_CLK_EN1(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BBUD_CLK_EN1_SHIFT)) & APU_STATUS2_APU_BBUD_CLK_EN1_MASK)

#define APU_STATUS2_APU_BBUD_CLK_EN2_MASK        (0x2000U)
#define APU_STATUS2_APU_BBUD_CLK_EN2_SHIFT       (13U)
/*! APU_BBUD_CLK_EN2 - bbud1 clk enable */
#define APU_STATUS2_APU_BBUD_CLK_EN2(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_BBUD_CLK_EN2_SHIFT)) & APU_STATUS2_APU_BBUD_CLK_EN2_MASK)

#define APU_STATUS2_APU_WL_RF_CLK_EN1_MASK       (0x4000U)
#define APU_STATUS2_APU_WL_RF_CLK_EN1_SHIFT      (14U)
/*! APU_WL_RF_CLK_EN1 - WLAN RFU1 clk enable */
#define APU_STATUS2_APU_WL_RF_CLK_EN1(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_WL_RF_CLK_EN1_SHIFT)) & APU_STATUS2_APU_WL_RF_CLK_EN1_MASK)

#define APU_STATUS2_APU_WL_RF_CLK_EN2_MASK       (0x8000U)
#define APU_STATUS2_APU_WL_RF_CLK_EN2_SHIFT      (15U)
/*! APU_WL_RF_CLK_EN2 - WLAN RFU1 clk enable */
#define APU_STATUS2_APU_WL_RF_CLK_EN2(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_WL_RF_CLK_EN2_SHIFT)) & APU_STATUS2_APU_WL_RF_CLK_EN2_MASK)

#define APU_STATUS2_APU_CAU_BT_ACTIVE_MASK       (0x10000U)
#define APU_STATUS2_APU_CAU_BT_ACTIVE_SHIFT      (16U)
/*! APU_CAU_BT_ACTIVE - APU CAU Bluetooth Active */
#define APU_STATUS2_APU_CAU_BT_ACTIVE(x)         (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_CAU_BT_ACTIVE_SHIFT)) & APU_STATUS2_APU_CAU_BT_ACTIVE_MASK)

#define APU_STATUS2_APU_SOC_CAU_LDO_XOSC_EN_MASK (0x20000U)
#define APU_STATUS2_APU_SOC_CAU_LDO_XOSC_EN_SHIFT (17U)
/*! APU_SOC_CAU_LDO_XOSC_EN - APU SoC CAU LOD XOSC Enable */
#define APU_STATUS2_APU_SOC_CAU_LDO_XOSC_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_SOC_CAU_LDO_XOSC_EN_SHIFT)) & APU_STATUS2_APU_SOC_CAU_LDO_XOSC_EN_MASK)

#define APU_STATUS2_PCIE_P_REQ_MASK              (0x40000U)
#define APU_STATUS2_PCIE_P_REQ_SHIFT             (18U)
/*! PCIE_P_REQ - PCIe P req */
#define APU_STATUS2_PCIE_P_REQ(x)                (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_PCIE_P_REQ_SHIFT)) & APU_STATUS2_PCIE_P_REQ_MASK)

#define APU_STATUS2_PCIE_XP_REQ_MASK             (0x80000U)
#define APU_STATUS2_PCIE_XP_REQ_SHIFT            (19U)
/*! PCIE_XP_REQ - PCIe XP req */
#define APU_STATUS2_PCIE_XP_REQ(x)               (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_PCIE_XP_REQ_SHIFT)) & APU_STATUS2_PCIE_XP_REQ_MASK)

#define APU_STATUS2_BCA_CLK_REQ_MASK             (0x100000U)
#define APU_STATUS2_BCA_CLK_REQ_SHIFT            (20U)
/*! BCA_CLK_REQ - BCA Clock Request */
#define APU_STATUS2_BCA_CLK_REQ(x)               (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_BCA_CLK_REQ_SHIFT)) & APU_STATUS2_BCA_CLK_REQ_MASK)

#define APU_STATUS2_BCA_CLK_EN_MASK              (0x200000U)
#define APU_STATUS2_BCA_CLK_EN_SHIFT             (21U)
/*! BCA_CLK_EN - APU BCA Clock Enable */
#define APU_STATUS2_BCA_CLK_EN(x)                (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_BCA_CLK_EN_SHIFT)) & APU_STATUS2_BCA_CLK_EN_MASK)

#define APU_STATUS2_BCA_SLNA_ON_MASK             (0x400000U)
#define APU_STATUS2_BCA_SLNA_ON_SHIFT            (22U)
/*! BCA_SLNA_ON - BCA SLNA On */
#define APU_STATUS2_BCA_SLNA_ON(x)               (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_BCA_SLNA_ON_SHIFT)) & APU_STATUS2_BCA_SLNA_ON_MASK)

#define APU_STATUS2_BT_CLK_REQ_MASK              (0x800000U)
#define APU_STATUS2_BT_CLK_REQ_SHIFT             (23U)
/*! BT_CLK_REQ - BT clk req */
#define APU_STATUS2_BT_CLK_REQ(x)                (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_BT_CLK_REQ_SHIFT)) & APU_STATUS2_BT_CLK_REQ_MASK)

#define APU_STATUS2_BT2_CLK_REQ_MASK             (0x1000000U)
#define APU_STATUS2_BT2_CLK_REQ_SHIFT            (24U)
/*! BT2_CLK_REQ - BT2 clk req */
#define APU_STATUS2_BT2_CLK_REQ(x)               (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_BT2_CLK_REQ_SHIFT)) & APU_STATUS2_BT2_CLK_REQ_MASK)

#define APU_STATUS2_APU_DVFS_CLK_SEL_MASK        (0x2000000U)
#define APU_STATUS2_APU_DVFS_CLK_SEL_SHIFT       (25U)
/*! APU_DVFS_CLK_SEL - APU DVFS Clock Select */
#define APU_STATUS2_APU_DVFS_CLK_SEL(x)          (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_DVFS_CLK_SEL_SHIFT)) & APU_STATUS2_APU_DVFS_CLK_SEL_MASK)

#define APU_STATUS2_APU_INTR_WLAN_WAKEUP2_MASK   (0x4000000U)
#define APU_STATUS2_APU_INTR_WLAN_WAKEUP2_SHIFT  (26U)
/*! APU_INTR_WLAN_WAKEUP2 - APU wlan2 wakeup interrupt */
#define APU_STATUS2_APU_INTR_WLAN_WAKEUP2(x)     (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_INTR_WLAN_WAKEUP2_SHIFT)) & APU_STATUS2_APU_INTR_WLAN_WAKEUP2_MASK)

#define APU_STATUS2_APU_INTR_WLAN_WAKEUP1_MASK   (0x8000000U)
#define APU_STATUS2_APU_INTR_WLAN_WAKEUP1_SHIFT  (27U)
/*! APU_INTR_WLAN_WAKEUP1 - APU wlan1 wakeup interrupt */
#define APU_STATUS2_APU_INTR_WLAN_WAKEUP1(x)     (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_APU_INTR_WLAN_WAKEUP1_SHIFT)) & APU_STATUS2_APU_INTR_WLAN_WAKEUP1_MASK)

#define APU_STATUS2_AMU1_CP15_SLEEP_MASK         (0x10000000U)
#define APU_STATUS2_AMU1_CP15_SLEEP_SHIFT        (28U)
/*! AMU1_CP15_SLEEP - CPU1 CP15 Sleep */
#define APU_STATUS2_AMU1_CP15_SLEEP(x)           (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_AMU1_CP15_SLEEP_SHIFT)) & APU_STATUS2_AMU1_CP15_SLEEP_MASK)

#define APU_STATUS2_AMU2_CP15_SLEEP_MASK         (0x20000000U)
#define APU_STATUS2_AMU2_CP15_SLEEP_SHIFT        (29U)
/*! AMU2_CP15_SLEEP - CPU2 CP15 Sleep */
#define APU_STATUS2_AMU2_CP15_SLEEP(x)           (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_AMU2_CP15_SLEEP_SHIFT)) & APU_STATUS2_AMU2_CP15_SLEEP_MASK)

#define APU_STATUS2_AMU3_CP15_SLEEP_MASK         (0x40000000U)
#define APU_STATUS2_AMU3_CP15_SLEEP_SHIFT        (30U)
/*! AMU3_CP15_SLEEP - CPU3 CP15 Sleep */
#define APU_STATUS2_AMU3_CP15_SLEEP(x)           (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_AMU3_CP15_SLEEP_SHIFT)) & APU_STATUS2_AMU3_CP15_SLEEP_MASK)

#define APU_STATUS2_CPU1_APU_PD_MASK             (0x80000000U)
#define APU_STATUS2_CPU1_APU_PD_SHIFT            (31U)
/*! CPU1_APU_PD - CPU1 to APU powerdown */
#define APU_STATUS2_CPU1_APU_PD(x)               (((uint32_t)(((uint32_t)(x)) << APU_STATUS2_CPU1_APU_PD_SHIFT)) & APU_STATUS2_CPU1_APU_PD_MASK)
/*! @} */

/*! @name WLAN_PWR_CTRL_DLY2 - Wlan domain FSM Power Control Delay 2 */
/*! @{ */

#define APU_WLAN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK (0xFFFFFFFFU)
#define APU_WLAN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT (0U)
/*! PWR_CTRL_DLY2 - WLAN domain FSM power control delay 2 */
#define APU_WLAN_PWR_CTRL_DLY2_PWR_CTRL_DLY2(x)  (((uint32_t)(((uint32_t)(x)) << APU_WLAN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT)) & APU_WLAN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK)
/*! @} */

/*! @name WL_CTRL - WLAN Control */
/*! @{ */

#define APU_WL_CTRL_WL_SLP_RDY_MASK              (0x1U)
#define APU_WL_CTRL_WL_SLP_RDY_SHIFT             (0U)
/*! WL_SLP_RDY - WLAN Sleep Ready */
#define APU_WL_CTRL_WL_SLP_RDY(x)                (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WL_SLP_RDY_SHIFT)) & APU_WL_CTRL_WL_SLP_RDY_MASK)

#define APU_WL_CTRL_WL_SLP_RDYMASK_MASK          (0x2U)
#define APU_WL_CTRL_WL_SLP_RDYMASK_SHIFT         (1U)
/*! WL_SLP_RDYMASK - WLAN Sleep Ready Mask */
#define APU_WL_CTRL_WL_SLP_RDYMASK(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WL_SLP_RDYMASK_SHIFT)) & APU_WL_CTRL_WL_SLP_RDYMASK_MASK)

#define APU_WL_CTRL_WL_SLP_RDY_FW_MASK           (0x4U)
#define APU_WL_CTRL_WL_SLP_RDY_FW_SHIFT          (2U)
/*! WL_SLP_RDY_FW - WLAN Sleep Ready Firmware */
#define APU_WL_CTRL_WL_SLP_RDY_FW(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WL_SLP_RDY_FW_SHIFT)) & APU_WL_CTRL_WL_SLP_RDY_FW_MASK)

#define APU_WL_CTRL_IDLE2ISO_DLY_EN_MASK         (0x20U)
#define APU_WL_CTRL_IDLE2ISO_DLY_EN_SHIFT        (5U)
/*! IDLE2ISO_DLY_EN - Delay from idle to iso to ensure delay from bbud_non_udr_rst_b =0 to wlan_iso_en = 1 */
#define APU_WL_CTRL_IDLE2ISO_DLY_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_IDLE2ISO_DLY_EN_SHIFT)) & APU_WL_CTRL_IDLE2ISO_DLY_EN_MASK)

#define APU_WL_CTRL_APU_WKUP_WLRF_RX_MASK        (0x40U)
#define APU_WL_CTRL_APU_WKUP_WLRF_RX_SHIFT       (6U)
/*! APU_WKUP_WLRF_RX - APU Wakeup WL RF Rx */
#define APU_WL_CTRL_APU_WKUP_WLRF_RX(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_APU_WKUP_WLRF_RX_SHIFT)) & APU_WL_CTRL_APU_WKUP_WLRF_RX_MASK)

#define APU_WL_CTRL_USE_WL_INTR_SLP_MASK         (0x80U)
#define APU_WL_CTRL_USE_WL_INTR_SLP_SHIFT        (7U)
/*! USE_WL_INTR_SLP - Use WLAN Interrupt Sleep */
#define APU_WL_CTRL_USE_WL_INTR_SLP(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_USE_WL_INTR_SLP_SHIFT)) & APU_WL_CTRL_USE_WL_INTR_SLP_MASK)

#define APU_WL_CTRL_WL_USE_NOM_PWR_BYP_MASK      (0x200U)
#define APU_WL_CTRL_WL_USE_NOM_PWR_BYP_SHIFT     (9U)
/*! WL_USE_NOM_PWR_BYP - WLAN Use Nominal Power Bypass */
#define APU_WL_CTRL_WL_USE_NOM_PWR_BYP(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WL_USE_NOM_PWR_BYP_SHIFT)) & APU_WL_CTRL_WL_USE_NOM_PWR_BYP_MASK)

#define APU_WL_CTRL_USE_WL_PWR_RDY_FOR_WLRF_CLK_MASK (0x400U)
#define APU_WL_CTRL_USE_WL_PWR_RDY_FOR_WLRF_CLK_SHIFT (10U)
/*! USE_WL_PWR_RDY_FOR_WLRF_CLK - Use WLAN Power Ready for WL RF Clock */
#define APU_WL_CTRL_USE_WL_PWR_RDY_FOR_WLRF_CLK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_USE_WL_PWR_RDY_FOR_WLRF_CLK_SHIFT)) & APU_WL_CTRL_USE_WL_PWR_RDY_FOR_WLRF_CLK_MASK)

#define APU_WL_CTRL_APU_FW_RST_PE_MASK           (0x8000U)
#define APU_WL_CTRL_APU_FW_RST_PE_SHIFT          (15U)
/*! APU_FW_RST_PE - APU FW Reset PE */
#define APU_WL_CTRL_APU_FW_RST_PE(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_APU_FW_RST_PE_SHIFT)) & APU_WL_CTRL_APU_FW_RST_PE_MASK)

#define APU_WL_CTRL_APU_TSF_UPD_CNT_MASK         (0x1F0000U)
#define APU_WL_CTRL_APU_TSF_UPD_CNT_SHIFT        (16U)
/*! APU_TSF_UPD_CNT - APU TSF Update Count */
#define APU_WL_CTRL_APU_TSF_UPD_CNT(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_APU_TSF_UPD_CNT_SHIFT)) & APU_WL_CTRL_APU_TSF_UPD_CNT_MASK)

#define APU_WL_CTRL_APU_TSF_AUTO_UPDATE_MASK     (0x200000U)
#define APU_WL_CTRL_APU_TSF_AUTO_UPDATE_SHIFT    (21U)
/*! APU_TSF_AUTO_UPDATE - APU TSF Auto Update */
#define APU_WL_CTRL_APU_TSF_AUTO_UPDATE(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_APU_TSF_AUTO_UPDATE_SHIFT)) & APU_WL_CTRL_APU_TSF_AUTO_UPDATE_MASK)

#define APU_WL_CTRL_APU_WLAN_RF_MUX_SEL_MASK     (0x400000U)
#define APU_WL_CTRL_APU_WLAN_RF_MUX_SEL_SHIFT    (22U)
/*! APU_WLAN_RF_MUX_SEL - APU WLAN RF Mux Select */
#define APU_WL_CTRL_APU_WLAN_RF_MUX_SEL(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_APU_WLAN_RF_MUX_SEL_SHIFT)) & APU_WL_CTRL_APU_WLAN_RF_MUX_SEL_MASK)

#define APU_WL_CTRL_FW_FORCE_WL_PWRUP_MASK       (0x1000000U)
#define APU_WL_CTRL_FW_FORCE_WL_PWRUP_SHIFT      (24U)
/*! FW_FORCE_WL_PWRUP - FW Force WLAN Powerup */
#define APU_WL_CTRL_FW_FORCE_WL_PWRUP(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_FW_FORCE_WL_PWRUP_SHIFT)) & APU_WL_CTRL_FW_FORCE_WL_PWRUP_MASK)

#define APU_WL_CTRL_WLAN_USE_UNSYNC_PWR_LVL_MASK (0x2000000U)
#define APU_WL_CTRL_WLAN_USE_UNSYNC_PWR_LVL_SHIFT (25U)
/*! WLAN_USE_UNSYNC_PWR_LVL - WLAN Use Unsync Power Level */
#define APU_WL_CTRL_WLAN_USE_UNSYNC_PWR_LVL(x)   (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WLAN_USE_UNSYNC_PWR_LVL_SHIFT)) & APU_WL_CTRL_WLAN_USE_UNSYNC_PWR_LVL_MASK)

#define APU_WL_CTRL_WL_HOST_SLP_RDY_MASK         (0x80000000U)
#define APU_WL_CTRL_WL_HOST_SLP_RDY_SHIFT        (31U)
/*! WL_HOST_SLP_RDY - WLAN Host Sleep Ready */
#define APU_WL_CTRL_WL_HOST_SLP_RDY(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL_WL_HOST_SLP_RDY_SHIFT)) & APU_WL_CTRL_WL_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name WL_WKUP_MASK - WLAN Wakeup Mask */
/*! @{ */

#define APU_WL_WKUP_MASK_WL_BCN_TIMER_WKUP_MASK_MASK (0x1U)
#define APU_WL_WKUP_MASK_WL_BCN_TIMER_WKUP_MASK_SHIFT (0U)
/*! WL_BCN_TIMER_WKUP_MASK - WLAN Beacon Timer Wakeup Mask */
#define APU_WL_WKUP_MASK_WL_BCN_TIMER_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_BCN_TIMER_WKUP_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_BCN_TIMER_WKUP_MASK_MASK)

#define APU_WL_WKUP_MASK_BBUD_T2_PLL_REQ_MASK_MASK (0x2U)
#define APU_WL_WKUP_MASK_BBUD_T2_PLL_REQ_MASK_SHIFT (1U)
/*! BBUD_T2_PLL_REQ_MASK - BBUD T2 PLL Request Mask */
#define APU_WL_WKUP_MASK_BBUD_T2_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_BBUD_T2_PLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_BBUD_T2_PLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_RF_PLL_REQ_MASK_MASK (0x4U)
#define APU_WL_WKUP_MASK_WL_RF_PLL_REQ_MASK_SHIFT (2U)
/*! WL_RF_PLL_REQ_MASK - WLAN RF PLL Request Mask */
#define APU_WL_WKUP_MASK_WL_RF_PLL_REQ_MASK(x)   (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_RF_PLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_RF_PLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_BCA_MWS_WKUP_XP_MASK_MASK (0x8U)
#define APU_WL_WKUP_MASK_BCA_MWS_WKUP_XP_MASK_SHIFT (3U)
/*! BCA_MWS_WKUP_XP_MASK - BCA MWS Wakeup XP Mask */
#define APU_WL_WKUP_MASK_BCA_MWS_WKUP_XP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_BCA_MWS_WKUP_XP_MASK_SHIFT)) & APU_WL_WKUP_MASK_BCA_MWS_WKUP_XP_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_RF_PLL_DYNPLL_REQ_MASK_MASK (0x100U)
#define APU_WL_WKUP_MASK_WL_RF_PLL_DYNPLL_REQ_MASK_SHIFT (8U)
/*! WL_RF_PLL_DYNPLL_REQ_MASK - When mask = 0, wl_rf_pll_req is treated as pll_req */
#define APU_WL_WKUP_MASK_WL_RF_PLL_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_RF_PLL_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_RF_PLL_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_HOST_DYNPLL_REQ_MASK_MASK (0x200U)
#define APU_WL_WKUP_MASK_WL_HOST_DYNPLL_REQ_MASK_SHIFT (9U)
/*! WL_HOST_DYNPLL_REQ_MASK - When mask = 0, wlan_host_wkup is treated as pll_req */
#define APU_WL_WKUP_MASK_WL_HOST_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_HOST_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_HOST_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_TIMER_DYNPLL_REQ_MASK_MASK (0x400U)
#define APU_WL_WKUP_MASK_WL_TIMER_DYNPLL_REQ_MASK_SHIFT (10U)
/*! WL_TIMER_DYNPLL_REQ_MASK - When mask = 0, wlan_timer_xpp req is treated as pll_req */
#define APU_WL_WKUP_MASK_WL_TIMER_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_TIMER_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_TIMER_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_SLP_RDY_DYNPLL_REQ_MASK_MASK (0x800U)
#define APU_WL_WKUP_MASK_WL_SLP_RDY_DYNPLL_REQ_MASK_SHIFT (11U)
/*! WL_SLP_RDY_DYNPLL_REQ_MASK - When mask = 0, !wl_slp_rdy is treated as pll_req */
#define APU_WL_WKUP_MASK_WL_SLP_RDY_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_SLP_RDY_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_WL_SLP_RDY_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_IHB_WL_XPP_DYNPLL_REQ_MASK_MASK (0x1000U)
#define APU_WL_WKUP_MASK_IHB_WL_XPP_DYNPLL_REQ_MASK_SHIFT (12U)
/*! IHB_WL_XPP_DYNPLL_REQ_MASK - When mask = 0, ihb_wl_xpp_req is treated as pll_req */
#define APU_WL_WKUP_MASK_IHB_WL_XPP_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_IHB_WL_XPP_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK_IHB_WL_XPP_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK_WL_HOST_MAP_MASK        (0xFFFF0000U)
#define APU_WL_WKUP_MASK_WL_HOST_MAP_SHIFT       (16U)
/*! WL_HOST_MAP - WLAN Host Map */
#define APU_WL_WKUP_MASK_WL_HOST_MAP(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK_WL_HOST_MAP_SHIFT)) & APU_WL_WKUP_MASK_WL_HOST_MAP_MASK)
/*! @} */

/*! @name WL_STATUS - WLAN Status */
/*! @{ */

#define APU_WL_STATUS_APU_MAC_CLK_EN_MASK        (0x1U)
#define APU_WL_STATUS_APU_MAC_CLK_EN_SHIFT       (0U)
/*! APU_MAC_CLK_EN - APU MAC Clock Enable */
#define APU_WL_STATUS_APU_MAC_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_MAC_CLK_EN_SHIFT)) & APU_WL_STATUS_APU_MAC_CLK_EN_MASK)

#define APU_WL_STATUS_APU_BBUD_CLK_EN_MASK       (0x2U)
#define APU_WL_STATUS_APU_BBUD_CLK_EN_SHIFT      (1U)
/*! APU_BBUD_CLK_EN - APU BBUD Clock Enable */
#define APU_WL_STATUS_APU_BBUD_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_BBUD_CLK_EN_SHIFT)) & APU_WL_STATUS_APU_BBUD_CLK_EN_MASK)

#define APU_WL_STATUS_APU_WL_RF_CLK_EN_MASK      (0x4U)
#define APU_WL_STATUS_APU_WL_RF_CLK_EN_SHIFT     (2U)
/*! APU_WL_RF_CLK_EN - APU WLAN Ref Clock Enable */
#define APU_WL_STATUS_APU_WL_RF_CLK_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WL_RF_CLK_EN_SHIFT)) & APU_WL_STATUS_APU_WL_RF_CLK_EN_MASK)

#define APU_WL_STATUS_APU_WL_SLP_RDY_AFTER_MASK_MASK (0x8U)
#define APU_WL_STATUS_APU_WL_SLP_RDY_AFTER_MASK_SHIFT (3U)
/*! APU_WL_SLP_RDY_AFTER_MASK - APU WLAN Sleep Ready After Mask */
#define APU_WL_STATUS_APU_WL_SLP_RDY_AFTER_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WL_SLP_RDY_AFTER_MASK_SHIFT)) & APU_WL_STATUS_APU_WL_SLP_RDY_AFTER_MASK_MASK)

#define APU_WL_STATUS_APU_WL_RF_CTRL_MASK        (0x30U)
#define APU_WL_STATUS_APU_WL_RF_CTRL_SHIFT       (4U)
/*! APU_WL_RF_CTRL - APU WLAN RF Control for PE1/PE2 */
#define APU_WL_STATUS_APU_WL_RF_CTRL(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WL_RF_CTRL_SHIFT)) & APU_WL_STATUS_APU_WL_RF_CTRL_MASK)

#define APU_WL_STATUS_APU_PLL1_EN_MASK           (0x40U)
#define APU_WL_STATUS_APU_PLL1_EN_SHIFT          (6U)
/*! APU_PLL1_EN - APU PLL1 Enable */
#define APU_WL_STATUS_APU_PLL1_EN(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_PLL1_EN_SHIFT)) & APU_WL_STATUS_APU_PLL1_EN_MASK)

#define APU_WL_STATUS_APU_PLL3_EN_MASK           (0x80U)
#define APU_WL_STATUS_APU_PLL3_EN_SHIFT          (7U)
/*! APU_PLL3_EN - APU PLL3 Enable */
#define APU_WL_STATUS_APU_PLL3_EN(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_PLL3_EN_SHIFT)) & APU_WL_STATUS_APU_PLL3_EN_MASK)

#define APU_WL_STATUS_BCA_MWS_WKUP_XP_MASK       (0x100U)
#define APU_WL_STATUS_BCA_MWS_WKUP_XP_SHIFT      (8U)
/*! BCA_MWS_WKUP_XP - BCA MWS Wakeup XP */
#define APU_WL_STATUS_BCA_MWS_WKUP_XP(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_BCA_MWS_WKUP_XP_SHIFT)) & APU_WL_STATUS_BCA_MWS_WKUP_XP_MASK)

#define APU_WL_STATUS_APU_RTDP_WU_RSTB_MASK      (0x200U)
#define APU_WL_STATUS_APU_RTDP_WU_RSTB_SHIFT     (9U)
/*! APU_RTDP_WU_RSTB - APU RTDP WU RSTb */
#define APU_WL_STATUS_APU_RTDP_WU_RSTB(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_RTDP_WU_RSTB_SHIFT)) & APU_WL_STATUS_APU_RTDP_WU_RSTB_MASK)

#define APU_WL_STATUS_APU_BBUD_NON_UDR_RST_B_MASK (0x400U)
#define APU_WL_STATUS_APU_BBUD_NON_UDR_RST_B_SHIFT (10U)
/*! APU_BBUD_NON_UDR_RST_B - APU BBUD non-UDR RSTb */
#define APU_WL_STATUS_APU_BBUD_NON_UDR_RST_B(x)  (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_BBUD_NON_UDR_RST_B_SHIFT)) & APU_WL_STATUS_APU_BBUD_NON_UDR_RST_B_MASK)

#define APU_WL_STATUS_APU_WL_ST_MASK             (0xF000U)
#define APU_WL_STATUS_APU_WL_ST_SHIFT            (12U)
/*! APU_WL_ST - APU WLAN St */
#define APU_WL_STATUS_APU_WL_ST(x)               (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WL_ST_SHIFT)) & APU_WL_STATUS_APU_WL_ST_MASK)

#define APU_WL_STATUS_APU_WLAN_SWITCH_PD_MASK    (0x10000U)
#define APU_WL_STATUS_APU_WLAN_SWITCH_PD_SHIFT   (16U)
/*! APU_WLAN_SWITCH_PD - APU WLAN Switch Pd */
#define APU_WL_STATUS_APU_WLAN_SWITCH_PD(x)      (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WLAN_SWITCH_PD_SHIFT)) & APU_WL_STATUS_APU_WLAN_SWITCH_PD_MASK)

#define APU_WL_STATUS_APU_WLAN_UDR_FIREWALL_B_MASK (0x20000U)
#define APU_WL_STATUS_APU_WLAN_UDR_FIREWALL_B_SHIFT (17U)
/*! APU_WLAN_UDR_FIREWALL_B - APU WLAN UDR Firewall b */
#define APU_WL_STATUS_APU_WLAN_UDR_FIREWALL_B(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WLAN_UDR_FIREWALL_B_SHIFT)) & APU_WL_STATUS_APU_WLAN_UDR_FIREWALL_B_MASK)

#define APU_WL_STATUS_APU_WLAN_CLK_DIV_RSTB_MASK (0x40000U)
#define APU_WL_STATUS_APU_WLAN_CLK_DIV_RSTB_SHIFT (18U)
/*! APU_WLAN_CLK_DIV_RSTB - APU WLAN Clock Div RSTb */
#define APU_WL_STATUS_APU_WLAN_CLK_DIV_RSTB(x)   (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WLAN_CLK_DIV_RSTB_SHIFT)) & APU_WL_STATUS_APU_WLAN_CLK_DIV_RSTB_MASK)

#define APU_WL_STATUS_APU_WLAN_ISO_EN_MASK       (0x80000U)
#define APU_WL_STATUS_APU_WLAN_ISO_EN_SHIFT      (19U)
/*! APU_WLAN_ISO_EN - APU WLAN ISO Enable */
#define APU_WL_STATUS_APU_WLAN_ISO_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WLAN_ISO_EN_SHIFT)) & APU_WL_STATUS_APU_WLAN_ISO_EN_MASK)

#define APU_WL_STATUS_APU_WLAN_SRAM_PD_MASK      (0x100000U)
#define APU_WL_STATUS_APU_WLAN_SRAM_PD_SHIFT     (20U)
/*! APU_WLAN_SRAM_PD - APU WLAN SRAM Enable */
#define APU_WL_STATUS_APU_WLAN_SRAM_PD(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_APU_WLAN_SRAM_PD_SHIFT)) & APU_WL_STATUS_APU_WLAN_SRAM_PD_MASK)

#define APU_WL_STATUS_WLAN_VOL_REACHED_MASK      (0x200000U)
#define APU_WL_STATUS_WLAN_VOL_REACHED_SHIFT     (21U)
/*! WLAN_VOL_REACHED - WLAN Volume Reached */
#define APU_WL_STATUS_WLAN_VOL_REACHED(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WLAN_VOL_REACHED_SHIFT)) & APU_WL_STATUS_WLAN_VOL_REACHED_MASK)

#define APU_WL_STATUS_SOC_VOL_REACHED_MASK       (0x400000U)
#define APU_WL_STATUS_SOC_VOL_REACHED_SHIFT      (22U)
/*! SOC_VOL_REACHED - SoC Volume Reached */
#define APU_WL_STATUS_SOC_VOL_REACHED(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_SOC_VOL_REACHED_SHIFT)) & APU_WL_STATUS_SOC_VOL_REACHED_MASK)

#define APU_WL_STATUS_WL_PWR_RDY_MASK            (0x800000U)
#define APU_WL_STATUS_WL_PWR_RDY_SHIFT           (23U)
/*! WL_PWR_RDY - WLAN Power Ready */
#define APU_WL_STATUS_WL_PWR_RDY(x)              (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WL_PWR_RDY_SHIFT)) & APU_WL_STATUS_WL_PWR_RDY_MASK)

#define APU_WL_STATUS_WL_HOST_INTR_REF_MASK      (0x1000000U)
#define APU_WL_STATUS_WL_HOST_INTR_REF_SHIFT     (24U)
/*! WL_HOST_INTR_REF - WLAN Host Interrupt Reference */
#define APU_WL_STATUS_WL_HOST_INTR_REF(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WL_HOST_INTR_REF_SHIFT)) & APU_WL_STATUS_WL_HOST_INTR_REF_MASK)

#define APU_WL_STATUS_WLAN_HOST_WKUP_MASK        (0x2000000U)
#define APU_WL_STATUS_WLAN_HOST_WKUP_SHIFT       (25U)
/*! WLAN_HOST_WKUP - WLAN Host Wakeup */
#define APU_WL_STATUS_WLAN_HOST_WKUP(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WLAN_HOST_WKUP_SHIFT)) & APU_WL_STATUS_WLAN_HOST_WKUP_MASK)

#define APU_WL_STATUS_WL_TIMER_INTR_MASK         (0x4000000U)
#define APU_WL_STATUS_WL_TIMER_INTR_SHIFT        (26U)
/*! WL_TIMER_INTR - WLAN Timer Interrupt */
#define APU_WL_STATUS_WL_TIMER_INTR(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WL_TIMER_INTR_SHIFT)) & APU_WL_STATUS_WL_TIMER_INTR_MASK)

#define APU_WL_STATUS_TIMER_XPP_WAKEUP_MASK      (0x8000000U)
#define APU_WL_STATUS_TIMER_XPP_WAKEUP_SHIFT     (27U)
/*! TIMER_XPP_WAKEUP - Timer XPP Wakeup */
#define APU_WL_STATUS_TIMER_XPP_WAKEUP(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_TIMER_XPP_WAKEUP_SHIFT)) & APU_WL_STATUS_TIMER_XPP_WAKEUP_MASK)

#define APU_WL_STATUS_TIMER_WAKEUP_MASK          (0x10000000U)
#define APU_WL_STATUS_TIMER_WAKEUP_SHIFT         (28U)
/*! TIMER_WAKEUP - Timer Wakeup */
#define APU_WL_STATUS_TIMER_WAKEUP(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_TIMER_WAKEUP_SHIFT)) & APU_WL_STATUS_TIMER_WAKEUP_MASK)

#define APU_WL_STATUS_WLRF_PLL_REQ_MASK          (0x20000000U)
#define APU_WL_STATUS_WLRF_PLL_REQ_SHIFT         (29U)
/*! WLRF_PLL_REQ - WL RF PLL Request */
#define APU_WL_STATUS_WLRF_PLL_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_WLRF_PLL_REQ_SHIFT)) & APU_WL_STATUS_WLRF_PLL_REQ_MASK)

#define APU_WL_STATUS_BBUD_T2_PLL_REQ_MASK       (0x40000000U)
#define APU_WL_STATUS_BBUD_T2_PLL_REQ_SHIFT      (30U)
/*! BBUD_T2_PLL_REQ - BBUD T2 PLL Request */
#define APU_WL_STATUS_BBUD_T2_PLL_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_BBUD_T2_PLL_REQ_SHIFT)) & APU_WL_STATUS_BBUD_T2_PLL_REQ_MASK)

#define APU_WL_STATUS_BCA_CLK_REQ_MASK           (0x80000000U)
#define APU_WL_STATUS_BCA_CLK_REQ_SHIFT          (31U)
/*! BCA_CLK_REQ - BCA Clock Request */
#define APU_WL_STATUS_BCA_CLK_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS_BCA_CLK_REQ_SHIFT)) & APU_WL_STATUS_BCA_CLK_REQ_MASK)
/*! @} */

/*! @name WL_ALARM_RD - WLAN Alarm Readback */
/*! @{ */

#define APU_WL_ALARM_RD_APU_WL_ALARM_RD_MASK     (0xFFFFFFFFU)
#define APU_WL_ALARM_RD_APU_WL_ALARM_RD_SHIFT    (0U)
/*! APU_WL_ALARM_RD - reads back current beacon timer alarm value */
#define APU_WL_ALARM_RD_APU_WL_ALARM_RD(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_ALARM_RD_APU_WL_ALARM_RD_SHIFT)) & APU_WL_ALARM_RD_APU_WL_ALARM_RD_MASK)
/*! @} */

/*! @name WL_BCN_XP_ALARM - WLAN Beacon Alarm */
/*! @{ */

#define APU_WL_BCN_XP_ALARM_WL_BCN_XP_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_XP_ALARM_WL_BCN_XP_ALARM_SHIFT (0U)
/*! WL_BCN_XP_ALARM - number of sleep clocks until WLAN beacon timer requests for reference clock and power */
#define APU_WL_BCN_XP_ALARM_WL_BCN_XP_ALARM(x)   (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_XP_ALARM_WL_BCN_XP_ALARM_SHIFT)) & APU_WL_BCN_XP_ALARM_WL_BCN_XP_ALARM_MASK)
/*! @} */

/*! @name WL_BCN_INTR_ALARM - WLAN Beacon Interrupt Alarm */
/*! @{ */

#define APU_WL_BCN_INTR_ALARM_WL_BCN_INTR_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_INTR_ALARM_WL_BCN_INTR_ALARM_SHIFT (0U)
/*! WL_BCN_INTR_ALARM - number of ref clocks after WLAN beacon timer request to firmware wakeup */
#define APU_WL_BCN_INTR_ALARM_WL_BCN_INTR_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_INTR_ALARM_WL_BCN_INTR_ALARM_SHIFT)) & APU_WL_BCN_INTR_ALARM_WL_BCN_INTR_ALARM_MASK)
/*! @} */

/*! @name WL_HOST_INTR_ALARM - WLAN Host Interrupt Alarm */
/*! @{ */

#define APU_WL_HOST_INTR_ALARM_WL_HOST_INTR_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_HOST_INTR_ALARM_WL_HOST_INTR_ALARM_SHIFT (0U)
/*! WL_HOST_INTR_ALARM - number of ref clocks after WLAN host request to firmware wakeup */
#define APU_WL_HOST_INTR_ALARM_WL_HOST_INTR_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_HOST_INTR_ALARM_WL_HOST_INTR_ALARM_SHIFT)) & APU_WL_HOST_INTR_ALARM_WL_HOST_INTR_ALARM_MASK)
/*! @} */

/*! @name WL_BCN_PLL_ALARM - WLAN Beacon PLL Alarm */
/*! @{ */

#define APU_WL_BCN_PLL_ALARM_WL_BCN_PLL_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_PLL_ALARM_WL_BCN_PLL_ALARM_SHIFT (0U)
/*! WL_BCN_PLL_ALARM - number of sleep clocks until WLAN beacon timer requests for reference clock, power, and PLL */
#define APU_WL_BCN_PLL_ALARM_WL_BCN_PLL_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_PLL_ALARM_WL_BCN_PLL_ALARM_SHIFT)) & APU_WL_BCN_PLL_ALARM_WL_BCN_PLL_ALARM_MASK)
/*! @} */

/*! @name TSF_REF_FACTOR - TSF Reference Factor */
/*! @{ */

#define APU_TSF_REF_FACTOR_TSF_REF_FACTOR_MASK   (0x7FFFFU)
#define APU_TSF_REF_FACTOR_TSF_REF_FACTOR_SHIFT  (0U)
/*! TSF_REF_FACTOR - number of 1us in 1 reference clock */
#define APU_TSF_REF_FACTOR_TSF_REF_FACTOR(x)     (((uint32_t)(((uint32_t)(x)) << APU_TSF_REF_FACTOR_TSF_REF_FACTOR_SHIFT)) & APU_TSF_REF_FACTOR_TSF_REF_FACTOR_MASK)
/*! @} */

/*! @name TSF_SLEEP_FACTOR - TSF Sleep Factor */
/*! @{ */

#define APU_TSF_SLEEP_FACTOR_TSF_SLEEP_FACTOR_MASK (0xFFFFFFFU)
#define APU_TSF_SLEEP_FACTOR_TSF_SLEEP_FACTOR_SHIFT (0U)
/*! TSF_SLEEP_FACTOR - number of 1us in 1 sleep clock */
#define APU_TSF_SLEEP_FACTOR_TSF_SLEEP_FACTOR(x) (((uint32_t)(((uint32_t)(x)) << APU_TSF_SLEEP_FACTOR_TSF_SLEEP_FACTOR_SHIFT)) & APU_TSF_SLEEP_FACTOR_TSF_SLEEP_FACTOR_MASK)
/*! @} */

/*! @name BBUD_UDR_ISO_CNT - BBUD UDR ISO Count */
/*! @{ */

#define APU_BBUD_UDR_ISO_CNT_BBUD_UDR_ASSERT_CNT_MASK (0x1FFFU)
#define APU_BBUD_UDR_ISO_CNT_BBUD_UDR_ASSERT_CNT_SHIFT (0U)
/*! BBUD_UDR_ASSERT_CNT - number of reference clocks before bbud_non_udr_rst de-assertion after bbud_clk_en is asserted */
#define APU_BBUD_UDR_ISO_CNT_BBUD_UDR_ASSERT_CNT(x) (((uint32_t)(((uint32_t)(x)) << APU_BBUD_UDR_ISO_CNT_BBUD_UDR_ASSERT_CNT_SHIFT)) & APU_BBUD_UDR_ISO_CNT_BBUD_UDR_ASSERT_CNT_MASK)

#define APU_BBUD_UDR_ISO_CNT_BBUD_CLK_ASSERT_CNT_MASK (0x1FFF0000U)
#define APU_BBUD_UDR_ISO_CNT_BBUD_CLK_ASSERT_CNT_SHIFT (16U)
/*! BBUD_CLK_ASSERT_CNT - number of reference clocks before bbud_iso_en de-assertion after bbud_non_udr_rst de-asserts */
#define APU_BBUD_UDR_ISO_CNT_BBUD_CLK_ASSERT_CNT(x) (((uint32_t)(((uint32_t)(x)) << APU_BBUD_UDR_ISO_CNT_BBUD_CLK_ASSERT_CNT_SHIFT)) & APU_BBUD_UDR_ISO_CNT_BBUD_CLK_ASSERT_CNT_MASK)
/*! @} */

/*! @name WL_DVFS_CTRL - WLAN DVFS Control */
/*! @{ */

#define APU_WL_DVFS_CTRL_WLAN_VOL_VAL_MASK       (0x7FU)
#define APU_WL_DVFS_CTRL_WLAN_VOL_VAL_SHIFT      (0U)
/*! WLAN_VOL_VAL - WLAN Vol Value */
#define APU_WL_DVFS_CTRL_WLAN_VOL_VAL(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_DVFS_CTRL_WLAN_VOL_VAL_SHIFT)) & APU_WL_DVFS_CTRL_WLAN_VOL_VAL_MASK)
/*! @} */

/*! @name WL_CTRL2 - WLAN Control 2 */
/*! @{ */

#define APU_WL_CTRL2_WL_SLP_RDY_MASK             (0x1U)
#define APU_WL_CTRL2_WL_SLP_RDY_SHIFT            (0U)
/*! WL_SLP_RDY - WLAN Sleep Ready */
#define APU_WL_CTRL2_WL_SLP_RDY(x)               (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WL_SLP_RDY_SHIFT)) & APU_WL_CTRL2_WL_SLP_RDY_MASK)

#define APU_WL_CTRL2_WL_SLP_RDYMASK_MASK         (0x2U)
#define APU_WL_CTRL2_WL_SLP_RDYMASK_SHIFT        (1U)
/*! WL_SLP_RDYMASK - WLAN Sleep Ready Mask */
#define APU_WL_CTRL2_WL_SLP_RDYMASK(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WL_SLP_RDYMASK_SHIFT)) & APU_WL_CTRL2_WL_SLP_RDYMASK_MASK)

#define APU_WL_CTRL2_WL_SLP_RDY_FW_MASK          (0x4U)
#define APU_WL_CTRL2_WL_SLP_RDY_FW_SHIFT         (2U)
/*! WL_SLP_RDY_FW - WLAN Sleep Ready Firmware */
#define APU_WL_CTRL2_WL_SLP_RDY_FW(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WL_SLP_RDY_FW_SHIFT)) & APU_WL_CTRL2_WL_SLP_RDY_FW_MASK)

#define APU_WL_CTRL2_IDLE2ISO_DLY_EN_MASK        (0x20U)
#define APU_WL_CTRL2_IDLE2ISO_DLY_EN_SHIFT       (5U)
/*! IDLE2ISO_DLY_EN - Delay from idle to iso to ensure delay from bbud_non_udr_rst_b =0 to wlan_iso_en = 1 */
#define APU_WL_CTRL2_IDLE2ISO_DLY_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_IDLE2ISO_DLY_EN_SHIFT)) & APU_WL_CTRL2_IDLE2ISO_DLY_EN_MASK)

#define APU_WL_CTRL2_APU_WKUP_WLRF_RX_MASK       (0x40U)
#define APU_WL_CTRL2_APU_WKUP_WLRF_RX_SHIFT      (6U)
/*! APU_WKUP_WLRF_RX - APU Wakeup WL RF Rx */
#define APU_WL_CTRL2_APU_WKUP_WLRF_RX(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_APU_WKUP_WLRF_RX_SHIFT)) & APU_WL_CTRL2_APU_WKUP_WLRF_RX_MASK)

#define APU_WL_CTRL2_USE_WL_INTR_SLP_MASK        (0x80U)
#define APU_WL_CTRL2_USE_WL_INTR_SLP_SHIFT       (7U)
/*! USE_WL_INTR_SLP - Use WLAN Interrupt Sleep */
#define APU_WL_CTRL2_USE_WL_INTR_SLP(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_USE_WL_INTR_SLP_SHIFT)) & APU_WL_CTRL2_USE_WL_INTR_SLP_MASK)

#define APU_WL_CTRL2_WL_USE_NOM_PWR_BYP_MASK     (0x200U)
#define APU_WL_CTRL2_WL_USE_NOM_PWR_BYP_SHIFT    (9U)
/*! WL_USE_NOM_PWR_BYP - Use nom_pwr_lvl from dvfs for wl_pwr_lvl_reached */
#define APU_WL_CTRL2_WL_USE_NOM_PWR_BYP(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WL_USE_NOM_PWR_BYP_SHIFT)) & APU_WL_CTRL2_WL_USE_NOM_PWR_BYP_MASK)

#define APU_WL_CTRL2_USE_WL_PWR_RDY_FOR_WLRF_CLK_MASK (0x400U)
#define APU_WL_CTRL2_USE_WL_PWR_RDY_FOR_WLRF_CLK_SHIFT (10U)
/*! USE_WL_PWR_RDY_FOR_WLRF_CLK - Use WLAN Power Ready for WL RF Clock */
#define APU_WL_CTRL2_USE_WL_PWR_RDY_FOR_WLRF_CLK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_USE_WL_PWR_RDY_FOR_WLRF_CLK_SHIFT)) & APU_WL_CTRL2_USE_WL_PWR_RDY_FOR_WLRF_CLK_MASK)

#define APU_WL_CTRL2_APU_FW_RST_PE_MASK          (0x8000U)
#define APU_WL_CTRL2_APU_FW_RST_PE_SHIFT         (15U)
/*! APU_FW_RST_PE - APU FW Reset PE */
#define APU_WL_CTRL2_APU_FW_RST_PE(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_APU_FW_RST_PE_SHIFT)) & APU_WL_CTRL2_APU_FW_RST_PE_MASK)

#define APU_WL_CTRL2_APU_TSF_UPD_CNT_MASK        (0x1F0000U)
#define APU_WL_CTRL2_APU_TSF_UPD_CNT_SHIFT       (16U)
/*! APU_TSF_UPD_CNT - APU TSF Update Count */
#define APU_WL_CTRL2_APU_TSF_UPD_CNT(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_APU_TSF_UPD_CNT_SHIFT)) & APU_WL_CTRL2_APU_TSF_UPD_CNT_MASK)

#define APU_WL_CTRL2_APU_TSF_AUTO_UPDATE_MASK    (0x200000U)
#define APU_WL_CTRL2_APU_TSF_AUTO_UPDATE_SHIFT   (21U)
/*! APU_TSF_AUTO_UPDATE - APU TSF Auto Update */
#define APU_WL_CTRL2_APU_TSF_AUTO_UPDATE(x)      (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_APU_TSF_AUTO_UPDATE_SHIFT)) & APU_WL_CTRL2_APU_TSF_AUTO_UPDATE_MASK)

#define APU_WL_CTRL2_APU_WLAN_RF_MUX_SEL_MASK    (0x400000U)
#define APU_WL_CTRL2_APU_WLAN_RF_MUX_SEL_SHIFT   (22U)
/*! APU_WLAN_RF_MUX_SEL - APU WLAN RF Mux Select */
#define APU_WL_CTRL2_APU_WLAN_RF_MUX_SEL(x)      (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_APU_WLAN_RF_MUX_SEL_SHIFT)) & APU_WL_CTRL2_APU_WLAN_RF_MUX_SEL_MASK)

#define APU_WL_CTRL2_FW_FORCE_WL_PWRUP_MASK      (0x1000000U)
#define APU_WL_CTRL2_FW_FORCE_WL_PWRUP_SHIFT     (24U)
/*! FW_FORCE_WL_PWRUP - FW Force WLAN Powerup */
#define APU_WL_CTRL2_FW_FORCE_WL_PWRUP(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_FW_FORCE_WL_PWRUP_SHIFT)) & APU_WL_CTRL2_FW_FORCE_WL_PWRUP_MASK)

#define APU_WL_CTRL2_WLAN_USE_UNSYNC_PWR_LVL_MASK (0x2000000U)
#define APU_WL_CTRL2_WLAN_USE_UNSYNC_PWR_LVL_SHIFT (25U)
/*! WLAN_USE_UNSYNC_PWR_LVL - WLAN Use Unsync Power Level */
#define APU_WL_CTRL2_WLAN_USE_UNSYNC_PWR_LVL(x)  (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WLAN_USE_UNSYNC_PWR_LVL_SHIFT)) & APU_WL_CTRL2_WLAN_USE_UNSYNC_PWR_LVL_MASK)

#define APU_WL_CTRL2_WL_HOST_SLP_RDY_MASK        (0x80000000U)
#define APU_WL_CTRL2_WL_HOST_SLP_RDY_SHIFT       (31U)
/*! WL_HOST_SLP_RDY - WLAN Host Sleep Ready */
#define APU_WL_CTRL2_WL_HOST_SLP_RDY(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_CTRL2_WL_HOST_SLP_RDY_SHIFT)) & APU_WL_CTRL2_WL_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name WL_WKUP_MASK2 - WLAN Wakeup Mask 2 */
/*! @{ */

#define APU_WL_WKUP_MASK2_WL_BCN_TIMER_WKUP_MASK_MASK (0x1U)
#define APU_WL_WKUP_MASK2_WL_BCN_TIMER_WKUP_MASK_SHIFT (0U)
/*! WL_BCN_TIMER_WKUP_MASK - WLAN Beacon Timer Wakeup Mask */
#define APU_WL_WKUP_MASK2_WL_BCN_TIMER_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_BCN_TIMER_WKUP_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_BCN_TIMER_WKUP_MASK_MASK)

#define APU_WL_WKUP_MASK2_BBUD_T2_PLL_REQ_MASK_MASK (0x2U)
#define APU_WL_WKUP_MASK2_BBUD_T2_PLL_REQ_MASK_SHIFT (1U)
/*! BBUD_T2_PLL_REQ_MASK - BBUD T2 PLL Request Mask */
#define APU_WL_WKUP_MASK2_BBUD_T2_PLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_BBUD_T2_PLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_BBUD_T2_PLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_RF_PLL_REQ_MASK_MASK (0x4U)
#define APU_WL_WKUP_MASK2_WL_RF_PLL_REQ_MASK_SHIFT (2U)
/*! WL_RF_PLL_REQ_MASK - WLAN RF PLL Request Mask */
#define APU_WL_WKUP_MASK2_WL_RF_PLL_REQ_MASK(x)  (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_RF_PLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_RF_PLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_BCA_MWS_WKUP_XP_MASK_MASK (0x8U)
#define APU_WL_WKUP_MASK2_BCA_MWS_WKUP_XP_MASK_SHIFT (3U)
/*! BCA_MWS_WKUP_XP_MASK - BCA MWS Wakeup XP Mask */
#define APU_WL_WKUP_MASK2_BCA_MWS_WKUP_XP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_BCA_MWS_WKUP_XP_MASK_SHIFT)) & APU_WL_WKUP_MASK2_BCA_MWS_WKUP_XP_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_RF_PLL_DYNPLL_REQ_MASK_MASK (0x100U)
#define APU_WL_WKUP_MASK2_WL_RF_PLL_DYNPLL_REQ_MASK_SHIFT (8U)
/*! WL_RF_PLL_DYNPLL_REQ_MASK - When mask = 0, wl_rf_pll_req is treated as pll_req */
#define APU_WL_WKUP_MASK2_WL_RF_PLL_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_RF_PLL_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_RF_PLL_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_HOST_DYNPLL_REQ_MASK_MASK (0x200U)
#define APU_WL_WKUP_MASK2_WL_HOST_DYNPLL_REQ_MASK_SHIFT (9U)
/*! WL_HOST_DYNPLL_REQ_MASK - When mask = 0, wlan_host_wkup is treated as pll_req */
#define APU_WL_WKUP_MASK2_WL_HOST_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_HOST_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_HOST_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_TIMER_DYNPLL_REQ_MASK_MASK (0x400U)
#define APU_WL_WKUP_MASK2_WL_TIMER_DYNPLL_REQ_MASK_SHIFT (10U)
/*! WL_TIMER_DYNPLL_REQ_MASK - When mask = 0, wlan_timer_xpp req is treated as pll_req */
#define APU_WL_WKUP_MASK2_WL_TIMER_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_TIMER_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_TIMER_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_SLP_RDY_DYNPLL_REQ_MASK_MASK (0x800U)
#define APU_WL_WKUP_MASK2_WL_SLP_RDY_DYNPLL_REQ_MASK_SHIFT (11U)
/*! WL_SLP_RDY_DYNPLL_REQ_MASK - When mask = 0, !wl_slp_rdy is treated as pll_req */
#define APU_WL_WKUP_MASK2_WL_SLP_RDY_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_SLP_RDY_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_WL_SLP_RDY_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_IHB_WL_XPP_DYNPLL_REQ_MASK_MASK (0x1000U)
#define APU_WL_WKUP_MASK2_IHB_WL_XPP_DYNPLL_REQ_MASK_SHIFT (12U)
/*! IHB_WL_XPP_DYNPLL_REQ_MASK - When mask = 0, ihb_wl_xpp_req is treated as pll_req */
#define APU_WL_WKUP_MASK2_IHB_WL_XPP_DYNPLL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_IHB_WL_XPP_DYNPLL_REQ_MASK_SHIFT)) & APU_WL_WKUP_MASK2_IHB_WL_XPP_DYNPLL_REQ_MASK_MASK)

#define APU_WL_WKUP_MASK2_WL_HOST_MAP_MASK       (0xFFFF0000U)
#define APU_WL_WKUP_MASK2_WL_HOST_MAP_SHIFT      (16U)
/*! WL_HOST_MAP - WLAN Host Map */
#define APU_WL_WKUP_MASK2_WL_HOST_MAP(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_WKUP_MASK2_WL_HOST_MAP_SHIFT)) & APU_WL_WKUP_MASK2_WL_HOST_MAP_MASK)
/*! @} */

/*! @name WL_STATUS2 - WLAN Status 2 */
/*! @{ */

#define APU_WL_STATUS2_APU_MAC_CLK_EN_MASK       (0x1U)
#define APU_WL_STATUS2_APU_MAC_CLK_EN_SHIFT      (0U)
/*! APU_MAC_CLK_EN - APU MAC Clock Enable */
#define APU_WL_STATUS2_APU_MAC_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_MAC_CLK_EN_SHIFT)) & APU_WL_STATUS2_APU_MAC_CLK_EN_MASK)

#define APU_WL_STATUS2_APU_BBUD_CLK_EN_MASK      (0x2U)
#define APU_WL_STATUS2_APU_BBUD_CLK_EN_SHIFT     (1U)
/*! APU_BBUD_CLK_EN - APU BBUD Clock Enable */
#define APU_WL_STATUS2_APU_BBUD_CLK_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_BBUD_CLK_EN_SHIFT)) & APU_WL_STATUS2_APU_BBUD_CLK_EN_MASK)

#define APU_WL_STATUS2_APU_WL_RF_CLK_EN_MASK     (0x4U)
#define APU_WL_STATUS2_APU_WL_RF_CLK_EN_SHIFT    (2U)
/*! APU_WL_RF_CLK_EN - APU WLAN Ref Clock Enable */
#define APU_WL_STATUS2_APU_WL_RF_CLK_EN(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WL_RF_CLK_EN_SHIFT)) & APU_WL_STATUS2_APU_WL_RF_CLK_EN_MASK)

#define APU_WL_STATUS2_APU_WL_SLP_RDY_AFTER_MASK_MASK (0x8U)
#define APU_WL_STATUS2_APU_WL_SLP_RDY_AFTER_MASK_SHIFT (3U)
/*! APU_WL_SLP_RDY_AFTER_MASK - APU WLAN Sleep Ready After Mask */
#define APU_WL_STATUS2_APU_WL_SLP_RDY_AFTER_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WL_SLP_RDY_AFTER_MASK_SHIFT)) & APU_WL_STATUS2_APU_WL_SLP_RDY_AFTER_MASK_MASK)

#define APU_WL_STATUS2_APU_WL_RF_CTRL_MASK       (0x30U)
#define APU_WL_STATUS2_APU_WL_RF_CTRL_SHIFT      (4U)
/*! APU_WL_RF_CTRL - APU WLAN RF Control */
#define APU_WL_STATUS2_APU_WL_RF_CTRL(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WL_RF_CTRL_SHIFT)) & APU_WL_STATUS2_APU_WL_RF_CTRL_MASK)

#define APU_WL_STATUS2_APU_PLL1_EN_MASK          (0x40U)
#define APU_WL_STATUS2_APU_PLL1_EN_SHIFT         (6U)
/*! APU_PLL1_EN - APU PLL1 Enable */
#define APU_WL_STATUS2_APU_PLL1_EN(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_PLL1_EN_SHIFT)) & APU_WL_STATUS2_APU_PLL1_EN_MASK)

#define APU_WL_STATUS2_APU_PLL3_EN_MASK          (0x80U)
#define APU_WL_STATUS2_APU_PLL3_EN_SHIFT         (7U)
/*! APU_PLL3_EN - APU PLL3 Enable */
#define APU_WL_STATUS2_APU_PLL3_EN(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_PLL3_EN_SHIFT)) & APU_WL_STATUS2_APU_PLL3_EN_MASK)

#define APU_WL_STATUS2_BCA_MWS_WKUP_XP_MASK      (0x100U)
#define APU_WL_STATUS2_BCA_MWS_WKUP_XP_SHIFT     (8U)
/*! BCA_MWS_WKUP_XP - BCA MWS Wakeup XP */
#define APU_WL_STATUS2_BCA_MWS_WKUP_XP(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_BCA_MWS_WKUP_XP_SHIFT)) & APU_WL_STATUS2_BCA_MWS_WKUP_XP_MASK)

#define APU_WL_STATUS2_APU_RTDP_WU_RSTB_MASK     (0x200U)
#define APU_WL_STATUS2_APU_RTDP_WU_RSTB_SHIFT    (9U)
/*! APU_RTDP_WU_RSTB - APU RTDP WU RSTb */
#define APU_WL_STATUS2_APU_RTDP_WU_RSTB(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_RTDP_WU_RSTB_SHIFT)) & APU_WL_STATUS2_APU_RTDP_WU_RSTB_MASK)

#define APU_WL_STATUS2_APU_BBUD_NON_UDR_RST_B_MASK (0x400U)
#define APU_WL_STATUS2_APU_BBUD_NON_UDR_RST_B_SHIFT (10U)
/*! APU_BBUD_NON_UDR_RST_B - APU BBUD non-UDR RSTb */
#define APU_WL_STATUS2_APU_BBUD_NON_UDR_RST_B(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_BBUD_NON_UDR_RST_B_SHIFT)) & APU_WL_STATUS2_APU_BBUD_NON_UDR_RST_B_MASK)

#define APU_WL_STATUS2_APU_WL_ST_MASK            (0xF000U)
#define APU_WL_STATUS2_APU_WL_ST_SHIFT           (12U)
/*! APU_WL_ST - APU WLAN St */
#define APU_WL_STATUS2_APU_WL_ST(x)              (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WL_ST_SHIFT)) & APU_WL_STATUS2_APU_WL_ST_MASK)

#define APU_WL_STATUS2_APU_WLAN_SWITCH_PD_MASK   (0x10000U)
#define APU_WL_STATUS2_APU_WLAN_SWITCH_PD_SHIFT  (16U)
/*! APU_WLAN_SWITCH_PD - APU WLAN Switch Pd */
#define APU_WL_STATUS2_APU_WLAN_SWITCH_PD(x)     (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WLAN_SWITCH_PD_SHIFT)) & APU_WL_STATUS2_APU_WLAN_SWITCH_PD_MASK)

#define APU_WL_STATUS2_APU_WLAN_UDR_FIREWALL_B_MASK (0x20000U)
#define APU_WL_STATUS2_APU_WLAN_UDR_FIREWALL_B_SHIFT (17U)
/*! APU_WLAN_UDR_FIREWALL_B - APU WLAN UDR Firewall b */
#define APU_WL_STATUS2_APU_WLAN_UDR_FIREWALL_B(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WLAN_UDR_FIREWALL_B_SHIFT)) & APU_WL_STATUS2_APU_WLAN_UDR_FIREWALL_B_MASK)

#define APU_WL_STATUS2_APU_WLAN_CLK_DIV_RSTB_MASK (0x40000U)
#define APU_WL_STATUS2_APU_WLAN_CLK_DIV_RSTB_SHIFT (18U)
/*! APU_WLAN_CLK_DIV_RSTB - APU WLAN Clock Div RSTb */
#define APU_WL_STATUS2_APU_WLAN_CLK_DIV_RSTB(x)  (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WLAN_CLK_DIV_RSTB_SHIFT)) & APU_WL_STATUS2_APU_WLAN_CLK_DIV_RSTB_MASK)

#define APU_WL_STATUS2_APU_WLAN_ISO_EN_MASK      (0x80000U)
#define APU_WL_STATUS2_APU_WLAN_ISO_EN_SHIFT     (19U)
/*! APU_WLAN_ISO_EN - APU WLAN ISO Enable */
#define APU_WL_STATUS2_APU_WLAN_ISO_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WLAN_ISO_EN_SHIFT)) & APU_WL_STATUS2_APU_WLAN_ISO_EN_MASK)

#define APU_WL_STATUS2_APU_WLAN_SRAM_PD_MASK     (0x100000U)
#define APU_WL_STATUS2_APU_WLAN_SRAM_PD_SHIFT    (20U)
/*! APU_WLAN_SRAM_PD - APU WLAN SRAM Pd */
#define APU_WL_STATUS2_APU_WLAN_SRAM_PD(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_APU_WLAN_SRAM_PD_SHIFT)) & APU_WL_STATUS2_APU_WLAN_SRAM_PD_MASK)

#define APU_WL_STATUS2_WLAN_VOL_REACHED_MASK     (0x200000U)
#define APU_WL_STATUS2_WLAN_VOL_REACHED_SHIFT    (21U)
/*! WLAN_VOL_REACHED - WLAN Volume Reached */
#define APU_WL_STATUS2_WLAN_VOL_REACHED(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WLAN_VOL_REACHED_SHIFT)) & APU_WL_STATUS2_WLAN_VOL_REACHED_MASK)

#define APU_WL_STATUS2_SOC_VOL_REACHED_MASK      (0x400000U)
#define APU_WL_STATUS2_SOC_VOL_REACHED_SHIFT     (22U)
/*! SOC_VOL_REACHED - SoC Volume Reached */
#define APU_WL_STATUS2_SOC_VOL_REACHED(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_SOC_VOL_REACHED_SHIFT)) & APU_WL_STATUS2_SOC_VOL_REACHED_MASK)

#define APU_WL_STATUS2_WL_PWR_RDY_MASK           (0x800000U)
#define APU_WL_STATUS2_WL_PWR_RDY_SHIFT          (23U)
/*! WL_PWR_RDY - WLAN Power Ready */
#define APU_WL_STATUS2_WL_PWR_RDY(x)             (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WL_PWR_RDY_SHIFT)) & APU_WL_STATUS2_WL_PWR_RDY_MASK)

#define APU_WL_STATUS2_WL_HOST_INTR_REF_MASK     (0x1000000U)
#define APU_WL_STATUS2_WL_HOST_INTR_REF_SHIFT    (24U)
/*! WL_HOST_INTR_REF - WLAN Host Interrupt Reference */
#define APU_WL_STATUS2_WL_HOST_INTR_REF(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WL_HOST_INTR_REF_SHIFT)) & APU_WL_STATUS2_WL_HOST_INTR_REF_MASK)

#define APU_WL_STATUS2_WLAN_HOST_WKUP_MASK       (0x2000000U)
#define APU_WL_STATUS2_WLAN_HOST_WKUP_SHIFT      (25U)
/*! WLAN_HOST_WKUP - WLAN Host Wakeup */
#define APU_WL_STATUS2_WLAN_HOST_WKUP(x)         (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WLAN_HOST_WKUP_SHIFT)) & APU_WL_STATUS2_WLAN_HOST_WKUP_MASK)

#define APU_WL_STATUS2_WL_TIMER_INTR_MASK        (0x4000000U)
#define APU_WL_STATUS2_WL_TIMER_INTR_SHIFT       (26U)
/*! WL_TIMER_INTR - WLAN Timer Interrupt */
#define APU_WL_STATUS2_WL_TIMER_INTR(x)          (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WL_TIMER_INTR_SHIFT)) & APU_WL_STATUS2_WL_TIMER_INTR_MASK)

#define APU_WL_STATUS2_TIMER_XPP_WAKEUP_MASK     (0x8000000U)
#define APU_WL_STATUS2_TIMER_XPP_WAKEUP_SHIFT    (27U)
/*! TIMER_XPP_WAKEUP - Timer XPP Wakeup */
#define APU_WL_STATUS2_TIMER_XPP_WAKEUP(x)       (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_TIMER_XPP_WAKEUP_SHIFT)) & APU_WL_STATUS2_TIMER_XPP_WAKEUP_MASK)

#define APU_WL_STATUS2_TIMER_WAKEUP_MASK         (0x10000000U)
#define APU_WL_STATUS2_TIMER_WAKEUP_SHIFT        (28U)
/*! TIMER_WAKEUP - Timer Wakeup */
#define APU_WL_STATUS2_TIMER_WAKEUP(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_TIMER_WAKEUP_SHIFT)) & APU_WL_STATUS2_TIMER_WAKEUP_MASK)

#define APU_WL_STATUS2_WLRF_PLL_REQ_MASK         (0x20000000U)
#define APU_WL_STATUS2_WLRF_PLL_REQ_SHIFT        (29U)
/*! WLRF_PLL_REQ - WLAN RF PLL Request */
#define APU_WL_STATUS2_WLRF_PLL_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_WLRF_PLL_REQ_SHIFT)) & APU_WL_STATUS2_WLRF_PLL_REQ_MASK)

#define APU_WL_STATUS2_BBUD_T2_PLL_REQ_MASK      (0x40000000U)
#define APU_WL_STATUS2_BBUD_T2_PLL_REQ_SHIFT     (30U)
/*! BBUD_T2_PLL_REQ - BBUD T2 PLL Request */
#define APU_WL_STATUS2_BBUD_T2_PLL_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_BBUD_T2_PLL_REQ_SHIFT)) & APU_WL_STATUS2_BBUD_T2_PLL_REQ_MASK)

#define APU_WL_STATUS2_BCA_CLK_REQ_MASK          (0x80000000U)
#define APU_WL_STATUS2_BCA_CLK_REQ_SHIFT         (31U)
/*! BCA_CLK_REQ - BCA Clock Request */
#define APU_WL_STATUS2_BCA_CLK_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_WL_STATUS2_BCA_CLK_REQ_SHIFT)) & APU_WL_STATUS2_BCA_CLK_REQ_MASK)
/*! @} */

/*! @name WL_ALARM_RD2 - WLAN Alarm Readback 2 */
/*! @{ */

#define APU_WL_ALARM_RD2_APU_WL_ALARM_RD_MASK    (0xFFFFFFFFU)
#define APU_WL_ALARM_RD2_APU_WL_ALARM_RD_SHIFT   (0U)
/*! APU_WL_ALARM_RD - reads back current beacon timer alarm value */
#define APU_WL_ALARM_RD2_APU_WL_ALARM_RD(x)      (((uint32_t)(((uint32_t)(x)) << APU_WL_ALARM_RD2_APU_WL_ALARM_RD_SHIFT)) & APU_WL_ALARM_RD2_APU_WL_ALARM_RD_MASK)
/*! @} */

/*! @name WL_BCN_XP_ALARM2 - WLAN Beacon XP Alarm 2 */
/*! @{ */

#define APU_WL_BCN_XP_ALARM2_WL_BCN_XP_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_XP_ALARM2_WL_BCN_XP_ALARM_SHIFT (0U)
/*! WL_BCN_XP_ALARM - number of sleep clocks until WLAN beacon timer requests for reference clock and power */
#define APU_WL_BCN_XP_ALARM2_WL_BCN_XP_ALARM(x)  (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_XP_ALARM2_WL_BCN_XP_ALARM_SHIFT)) & APU_WL_BCN_XP_ALARM2_WL_BCN_XP_ALARM_MASK)
/*! @} */

/*! @name WL_BCN_INTR_ALARM2 - WLAN Beacon Interrupt Alarm 2 */
/*! @{ */

#define APU_WL_BCN_INTR_ALARM2_WL_BCN_INTR_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_INTR_ALARM2_WL_BCN_INTR_ALARM_SHIFT (0U)
/*! WL_BCN_INTR_ALARM - number of ref clocks after WLAN beacon timer request to firmware wakeup */
#define APU_WL_BCN_INTR_ALARM2_WL_BCN_INTR_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_INTR_ALARM2_WL_BCN_INTR_ALARM_SHIFT)) & APU_WL_BCN_INTR_ALARM2_WL_BCN_INTR_ALARM_MASK)
/*! @} */

/*! @name WL_HOST_INTR_ALARM2 - WLAN Host Interrupt Alarm 2 */
/*! @{ */

#define APU_WL_HOST_INTR_ALARM2_WL_HOST_INTR_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_HOST_INTR_ALARM2_WL_HOST_INTR_ALARM_SHIFT (0U)
/*! WL_HOST_INTR_ALARM - number of ref clocks after WLAN host request to firmware wakeup */
#define APU_WL_HOST_INTR_ALARM2_WL_HOST_INTR_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_HOST_INTR_ALARM2_WL_HOST_INTR_ALARM_SHIFT)) & APU_WL_HOST_INTR_ALARM2_WL_HOST_INTR_ALARM_MASK)
/*! @} */

/*! @name WL_BCN_PLL_ALARM2 - WLAN Beacon PLL Alarm 2 */
/*! @{ */

#define APU_WL_BCN_PLL_ALARM2_WL_BCN_PLL_ALARM_MASK (0xFFFFFFFFU)
#define APU_WL_BCN_PLL_ALARM2_WL_BCN_PLL_ALARM_SHIFT (0U)
/*! WL_BCN_PLL_ALARM - number of sleep clocks until WLAN beacon timer requests for reference clock, power, and PLL */
#define APU_WL_BCN_PLL_ALARM2_WL_BCN_PLL_ALARM(x) (((uint32_t)(((uint32_t)(x)) << APU_WL_BCN_PLL_ALARM2_WL_BCN_PLL_ALARM_SHIFT)) & APU_WL_BCN_PLL_ALARM2_WL_BCN_PLL_ALARM_MASK)
/*! @} */

/*! @name WLCOMN_PWR_CTRL - WLAN Comm Powerup Control */
/*! @{ */

#define APU_WLCOMN_PWR_CTRL_WLCOMN_UDR_ASSERT_CNT_MASK (0xFFU)
#define APU_WLCOMN_PWR_CTRL_WLCOMN_UDR_ASSERT_CNT_SHIFT (0U)
/*! WLCOMN_UDR_ASSERT_CNT - WL COMM UDR Assert Count */
#define APU_WLCOMN_PWR_CTRL_WLCOMN_UDR_ASSERT_CNT(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_WLCOMN_UDR_ASSERT_CNT_SHIFT)) & APU_WLCOMN_PWR_CTRL_WLCOMN_UDR_ASSERT_CNT_MASK)

#define APU_WLCOMN_PWR_CTRL_USE_WLCOMN_PWR_RDY_FOR_WL_MASK (0x100U)
#define APU_WLCOMN_PWR_CTRL_USE_WLCOMN_PWR_RDY_FOR_WL_SHIFT (8U)
/*! USE_WLCOMN_PWR_RDY_FOR_WL - Use WL COMM Power Ready fro WLAN */
#define APU_WLCOMN_PWR_CTRL_USE_WLCOMN_PWR_RDY_FOR_WL(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_USE_WLCOMN_PWR_RDY_FOR_WL_SHIFT)) & APU_WLCOMN_PWR_CTRL_USE_WLCOMN_PWR_RDY_FOR_WL_MASK)

#define APU_WLCOMN_PWR_CTRL_USE_DEEPSLEEP_FOR_SYS_CLK_EN_MASK (0x200U)
#define APU_WLCOMN_PWR_CTRL_USE_DEEPSLEEP_FOR_SYS_CLK_EN_SHIFT (9U)
/*! USE_DEEPSLEEP_FOR_SYS_CLK_EN - Use Deep Sleep for SYS_CLK Enable */
#define APU_WLCOMN_PWR_CTRL_USE_DEEPSLEEP_FOR_SYS_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_USE_DEEPSLEEP_FOR_SYS_CLK_EN_SHIFT)) & APU_WLCOMN_PWR_CTRL_USE_DEEPSLEEP_FOR_SYS_CLK_EN_MASK)

#define APU_WLCOMN_PWR_CTRL_IDLE2ISO_DLY_EN_MASK (0x400U)
#define APU_WLCOMN_PWR_CTRL_IDLE2ISO_DLY_EN_SHIFT (10U)
/*! IDLE2ISO_DLY_EN - wlcomn FSM state delay enable. When 1, use wlcomn_pwr_ctrl_dly and wlcomn_pwr_ctrl_dly2. When 0, no delay. */
#define APU_WLCOMN_PWR_CTRL_IDLE2ISO_DLY_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_IDLE2ISO_DLY_EN_SHIFT)) & APU_WLCOMN_PWR_CTRL_IDLE2ISO_DLY_EN_MASK)

#define APU_WLCOMN_PWR_CTRL_CPU_VINITHI_MASK     (0x8000U)
#define APU_WLCOMN_PWR_CTRL_CPU_VINITHI_SHIFT    (15U)
/*! CPU_VINITHI - CPU to CPU Delay */
#define APU_WLCOMN_PWR_CTRL_CPU_VINITHI(x)       (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_CPU_VINITHI_SHIFT)) & APU_WLCOMN_PWR_CTRL_CPU_VINITHI_MASK)

#define APU_WLCOMN_PWR_CTRL_WLCOMN_PWRUP_CNT_MASK (0xFFFF0000U)
#define APU_WLCOMN_PWR_CTRL_WLCOMN_PWRUP_CNT_SHIFT (16U)
/*! WLCOMN_PWRUP_CNT - number of reference clocks after soc_pwr_rdy to push wlcomn power up */
#define APU_WLCOMN_PWR_CTRL_WLCOMN_PWRUP_CNT(x)  (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_WLCOMN_PWRUP_CNT_SHIFT)) & APU_WLCOMN_PWR_CTRL_WLCOMN_PWRUP_CNT_MASK)
/*! @} */

/*! @name WLCOMN_STATUS - WLAN Comm Status */
/*! @{ */

#define APU_WLCOMN_STATUS_START_WLCOMN_WKUP_REFCK_MASK (0x400U)
#define APU_WLCOMN_STATUS_START_WLCOMN_WKUP_REFCK_SHIFT (10U)
/*! START_WLCOMN_WKUP_REFCK - WLCOMN FSM start ip wake up */
#define APU_WLCOMN_STATUS_START_WLCOMN_WKUP_REFCK(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_START_WLCOMN_WKUP_REFCK_SHIFT)) & APU_WLCOMN_STATUS_START_WLCOMN_WKUP_REFCK_MASK)

#define APU_WLCOMN_STATUS_ENTER_CPU1_SUB_DSLP_REFCK_MASK (0x800U)
#define APU_WLCOMN_STATUS_ENTER_CPU1_SUB_DSLP_REFCK_SHIFT (11U)
/*! ENTER_CPU1_SUB_DSLP_REFCK - WLCOMN FSM enter ip power down */
#define APU_WLCOMN_STATUS_ENTER_CPU1_SUB_DSLP_REFCK(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_ENTER_CPU1_SUB_DSLP_REFCK_SHIFT)) & APU_WLCOMN_STATUS_ENTER_CPU1_SUB_DSLP_REFCK_MASK)

#define APU_WLCOMN_STATUS_APU_WLCOMN_ST_MASK     (0xF000U)
#define APU_WLCOMN_STATUS_APU_WLCOMN_ST_SHIFT    (12U)
/*! APU_WLCOMN_ST - APU WLAN COMN St */
#define APU_WLCOMN_STATUS_APU_WLCOMN_ST(x)       (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_APU_WLCOMN_ST_SHIFT)) & APU_WLCOMN_STATUS_APU_WLCOMN_ST_MASK)

#define APU_WLCOMN_STATUS_APU_WLCOMN_SWITCH_PD_MASK (0x10000U)
#define APU_WLCOMN_STATUS_APU_WLCOMN_SWITCH_PD_SHIFT (16U)
/*! APU_WLCOMN_SWITCH_PD - APU WLAN COMN Switch Pd */
#define APU_WLCOMN_STATUS_APU_WLCOMN_SWITCH_PD(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_APU_WLCOMN_SWITCH_PD_SHIFT)) & APU_WLCOMN_STATUS_APU_WLCOMN_SWITCH_PD_MASK)

#define APU_WLCOMN_STATUS_APU_WLCOMN_UDR_FIREWALL_B_MASK (0x20000U)
#define APU_WLCOMN_STATUS_APU_WLCOMN_UDR_FIREWALL_B_SHIFT (17U)
/*! APU_WLCOMN_UDR_FIREWALL_B - APU WLAN COMN UDR Firewall b */
#define APU_WLCOMN_STATUS_APU_WLCOMN_UDR_FIREWALL_B(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_APU_WLCOMN_UDR_FIREWALL_B_SHIFT)) & APU_WLCOMN_STATUS_APU_WLCOMN_UDR_FIREWALL_B_MASK)

#define APU_WLCOMN_STATUS_APU_WLCOMN_CLK_DIV_RSTB_MASK (0x40000U)
#define APU_WLCOMN_STATUS_APU_WLCOMN_CLK_DIV_RSTB_SHIFT (18U)
/*! APU_WLCOMN_CLK_DIV_RSTB - APU WLAN COMN Clock Div RSTb */
#define APU_WLCOMN_STATUS_APU_WLCOMN_CLK_DIV_RSTB(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_APU_WLCOMN_CLK_DIV_RSTB_SHIFT)) & APU_WLCOMN_STATUS_APU_WLCOMN_CLK_DIV_RSTB_MASK)

#define APU_WLCOMN_STATUS_APU_WLCOMN_ISO_EN_MASK (0x80000U)
#define APU_WLCOMN_STATUS_APU_WLCOMN_ISO_EN_SHIFT (19U)
/*! APU_WLCOMN_ISO_EN - APU WLAN COMN ISO Enable */
#define APU_WLCOMN_STATUS_APU_WLCOMN_ISO_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_STATUS_APU_WLCOMN_ISO_EN_SHIFT)) & APU_WLCOMN_STATUS_APU_WLCOMN_ISO_EN_MASK)
/*! @} */

/*! @name BT_CTRL - Bluetooth Control */
/*! @{ */

#define APU_BT_CTRL_BT_SLP_RDY_MASK              (0x1U)
#define APU_BT_CTRL_BT_SLP_RDY_SHIFT             (0U)
/*! BT_SLP_RDY - Bluetooth Sleep Ready */
#define APU_BT_CTRL_BT_SLP_RDY(x)                (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_SLP_RDY_SHIFT)) & APU_BT_CTRL_BT_SLP_RDY_MASK)

#define APU_BT_CTRL_BT_SLP_RDYMASK_MASK          (0x2U)
#define APU_BT_CTRL_BT_SLP_RDYMASK_SHIFT         (1U)
/*! BT_SLP_RDYMASK - Bluetoot Sleep Ready Mask */
#define APU_BT_CTRL_BT_SLP_RDYMASK(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_SLP_RDYMASK_SHIFT)) & APU_BT_CTRL_BT_SLP_RDYMASK_MASK)

#define APU_BT_CTRL_BT_SLP_RDY_FW_MASK           (0x4U)
#define APU_BT_CTRL_BT_SLP_RDY_FW_SHIFT          (2U)
/*! BT_SLP_RDY_FW - Bluetooth Sleep Ready FW */
#define APU_BT_CTRL_BT_SLP_RDY_FW(x)             (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_SLP_RDY_FW_SHIFT)) & APU_BT_CTRL_BT_SLP_RDY_FW_MASK)

#define APU_BT_CTRL_BTU_CLK_NCO_MODE_SEL_EN_MASK (0x8U)
#define APU_BT_CTRL_BTU_CLK_NCO_MODE_SEL_EN_SHIFT (3U)
/*! BTU_CLK_NCO_MODE_SEL_EN - 0- disable the btu_clk_nco_mode from CIU2, keep the bt_clk_req as XP wakeup source */
#define APU_BT_CTRL_BTU_CLK_NCO_MODE_SEL_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BTU_CLK_NCO_MODE_SEL_EN_SHIFT)) & APU_BT_CTRL_BTU_CLK_NCO_MODE_SEL_EN_MASK)

#define APU_BT_CTRL_USE_BT_INTR_SLP_MASK         (0x80U)
#define APU_BT_CTRL_USE_BT_INTR_SLP_SHIFT        (7U)
/*! USE_BT_INTR_SLP - Use Bluetooth interrupt Sleep */
#define APU_BT_CTRL_USE_BT_INTR_SLP(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_USE_BT_INTR_SLP_SHIFT)) & APU_BT_CTRL_USE_BT_INTR_SLP_MASK)

#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL1_MASK   (0x4000000U)
#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL1_SHIFT  (26U)
/*! BT_CLK_SYNC_MODE_SEL1 - Bluetooth Clock Sync Mode Select 1 */
#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL1(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL1_SHIFT)) & APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL1_MASK)

#define APU_BT_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_MASK (0x8000000U)
#define APU_BT_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_SHIFT (27U)
/*! USE_GLITCH_FREE_BT_CLK_REQ - Use Glitch-Free Bluetooth Clock Request */
#define APU_BT_CTRL_USE_GLITCH_FREE_BT_CLK_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_SHIFT)) & APU_BT_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_MASK)

#define APU_BT_CTRL_BRF_CLK_SYNC_MODE_SEL_MASK   (0x10000000U)
#define APU_BT_CTRL_BRF_CLK_SYNC_MODE_SEL_SHIFT  (28U)
/*! BRF_CLK_SYNC_MODE_SEL - BRF Clock Sync Mode Select */
#define APU_BT_CTRL_BRF_CLK_SYNC_MODE_SEL(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BRF_CLK_SYNC_MODE_SEL_SHIFT)) & APU_BT_CTRL_BRF_CLK_SYNC_MODE_SEL_MASK)

#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL0_MASK   (0x20000000U)
#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL0_SHIFT  (29U)
/*! BT_CLK_SYNC_MODE_SEL0 - Bluetooth Clock Sync Mode Select 0 */
#define APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL0(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL0_SHIFT)) & APU_BT_CTRL_BT_CLK_SYNC_MODE_SEL0_MASK)

#define APU_BT_CTRL_BT_PLL_SYNC_MODE_SEL_MASK    (0x40000000U)
#define APU_BT_CTRL_BT_PLL_SYNC_MODE_SEL_SHIFT   (30U)
/*! BT_PLL_SYNC_MODE_SEL - Bluetooth PLL Sync Mode Select */
#define APU_BT_CTRL_BT_PLL_SYNC_MODE_SEL(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_PLL_SYNC_MODE_SEL_SHIFT)) & APU_BT_CTRL_BT_PLL_SYNC_MODE_SEL_MASK)

#define APU_BT_CTRL_BT_HOST_SLP_RDY_MASK         (0x80000000U)
#define APU_BT_CTRL_BT_HOST_SLP_RDY_SHIFT        (31U)
/*! BT_HOST_SLP_RDY - Bluetooth Host Sleep Ready */
#define APU_BT_CTRL_BT_HOST_SLP_RDY(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_CTRL_BT_HOST_SLP_RDY_SHIFT)) & APU_BT_CTRL_BT_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name BT_WKUP_MASK - Bluetooth Wakeup Mask */
/*! @{ */

#define APU_BT_WKUP_MASK_BCA_CLK_REQ_MASK_MASK   (0x1U)
#define APU_BT_WKUP_MASK_BCA_CLK_REQ_MASK_SHIFT  (0U)
/*! BCA_CLK_REQ_MASK - BCA Clock Request Mask */
#define APU_BT_WKUP_MASK_BCA_CLK_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BCA_CLK_REQ_MASK_SHIFT)) & APU_BT_WKUP_MASK_BCA_CLK_REQ_MASK_MASK)

#define APU_BT_WKUP_MASK_BT_CLK_REQ_MASK_MASK    (0x2U)
#define APU_BT_WKUP_MASK_BT_CLK_REQ_MASK_SHIFT   (1U)
/*! BT_CLK_REQ_MASK - Bluetooth Clock Request Mask */
#define APU_BT_WKUP_MASK_BT_CLK_REQ_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BT_CLK_REQ_MASK_SHIFT)) & APU_BT_WKUP_MASK_BT_CLK_REQ_MASK_MASK)

#define APU_BT_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_MASK (0x4U)
#define APU_BT_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_SHIFT (2U)
/*! BT_WB_ACTIVE_REQ_MASK - Bluetooth WB Active Request Mask */
#define APU_BT_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_SHIFT)) & APU_BT_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_MASK)

#define APU_BT_WKUP_MASK_BT_INTR_MASK_MASK       (0x8U)
#define APU_BT_WKUP_MASK_BT_INTR_MASK_SHIFT      (3U)
/*! BT_INTR_MASK - Bluetooth Interrupt Mask */
#define APU_BT_WKUP_MASK_BT_INTR_MASK(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BT_INTR_MASK_SHIFT)) & APU_BT_WKUP_MASK_BT_INTR_MASK_MASK)

#define APU_BT_WKUP_MASK_BT_PLL_REQ_MASK_MASK    (0x10U)
#define APU_BT_WKUP_MASK_BT_PLL_REQ_MASK_SHIFT   (4U)
/*! BT_PLL_REQ_MASK - Bluetooth PLL Request Mask */
#define APU_BT_WKUP_MASK_BT_PLL_REQ_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BT_PLL_REQ_MASK_SHIFT)) & APU_BT_WKUP_MASK_BT_PLL_REQ_MASK_MASK)

#define APU_BT_WKUP_MASK_BRF_PLL_REQ_MASK_MASK   (0x20U)
#define APU_BT_WKUP_MASK_BRF_PLL_REQ_MASK_SHIFT  (5U)
/*! BRF_PLL_REQ_MASK - BRF PLL Request Mask */
#define APU_BT_WKUP_MASK_BRF_PLL_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BRF_PLL_REQ_MASK_SHIFT)) & APU_BT_WKUP_MASK_BRF_PLL_REQ_MASK_MASK)

#define APU_BT_WKUP_MASK_BT_HOST_MAP_MASK        (0xFFFF0000U)
#define APU_BT_WKUP_MASK_BT_HOST_MAP_SHIFT       (16U)
/*! BT_HOST_MAP - Bluetooth Host Map */
#define APU_BT_WKUP_MASK_BT_HOST_MAP(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT_WKUP_MASK_BT_HOST_MAP_SHIFT)) & APU_BT_WKUP_MASK_BT_HOST_MAP_MASK)
/*! @} */

/*! @name BT_STATUS - Bluetooth Status */
/*! @{ */

#define APU_BT_STATUS_APU_BT_CLK_EN_MASK         (0x1U)
#define APU_BT_STATUS_APU_BT_CLK_EN_SHIFT        (0U)
/*! APU_BT_CLK_EN - APU Bluetooth Clock Enable */
#define APU_BT_STATUS_APU_BT_CLK_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_APU_BT_CLK_EN_SHIFT)) & APU_BT_STATUS_APU_BT_CLK_EN_MASK)

#define APU_BT_STATUS_APU_BRF_CLK_EN_MASK        (0x2U)
#define APU_BT_STATUS_APU_BRF_CLK_EN_SHIFT       (1U)
/*! APU_BRF_CLK_EN - APU BRF Clock Enable */
#define APU_BT_STATUS_APU_BRF_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_APU_BRF_CLK_EN_SHIFT)) & APU_BT_STATUS_APU_BRF_CLK_EN_MASK)

#define APU_BT_STATUS_BT_ACTIVE_SLPCK_MASK       (0x4U)
#define APU_BT_STATUS_BT_ACTIVE_SLPCK_SHIFT      (2U)
/*! BT_ACTIVE_SLPCK - Bluetooth Active Sleep Clock */
#define APU_BT_STATUS_BT_ACTIVE_SLPCK(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_ACTIVE_SLPCK_SHIFT)) & APU_BT_STATUS_BT_ACTIVE_SLPCK_MASK)

#define APU_BT_STATUS_BT_HOST_WKUP_MASK          (0x10U)
#define APU_BT_STATUS_BT_HOST_WKUP_SHIFT         (4U)
/*! BT_HOST_WKUP - Bluetooth Host Wakeup */
#define APU_BT_STATUS_BT_HOST_WKUP(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_HOST_WKUP_SHIFT)) & APU_BT_STATUS_BT_HOST_WKUP_MASK)

#define APU_BT_STATUS_BT_INTERRUPT_MASK          (0x20U)
#define APU_BT_STATUS_BT_INTERRUPT_SHIFT         (5U)
/*! BT_INTERRUPT - Bluetooth Interrupt */
#define APU_BT_STATUS_BT_INTERRUPT(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_INTERRUPT_SHIFT)) & APU_BT_STATUS_BT_INTERRUPT_MASK)

#define APU_BT_STATUS_BT_PLL_REQ_MASK            (0x40U)
#define APU_BT_STATUS_BT_PLL_REQ_SHIFT           (6U)
/*! BT_PLL_REQ - Bluetooth PLL Request */
#define APU_BT_STATUS_BT_PLL_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_PLL_REQ_SHIFT)) & APU_BT_STATUS_BT_PLL_REQ_MASK)

#define APU_BT_STATUS_BT_CLK_REQ_MASK            (0x80U)
#define APU_BT_STATUS_BT_CLK_REQ_SHIFT           (7U)
/*! BT_CLK_REQ - Bluetooth Clock Request */
#define APU_BT_STATUS_BT_CLK_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_CLK_REQ_SHIFT)) & APU_BT_STATUS_BT_CLK_REQ_MASK)

#define APU_BT_STATUS_BRF_PLL_REQ_MASK           (0x100U)
#define APU_BT_STATUS_BRF_PLL_REQ_SHIFT          (8U)
/*! BRF_PLL_REQ - BRF PLL Request */
#define APU_BT_STATUS_BRF_PLL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BRF_PLL_REQ_SHIFT)) & APU_BT_STATUS_BRF_PLL_REQ_MASK)

#define APU_BT_STATUS_BT_WIDEBAND_ACTIVE_MASK    (0x200U)
#define APU_BT_STATUS_BT_WIDEBAND_ACTIVE_SHIFT   (9U)
/*! BT_WIDEBAND_ACTIVE - Bluetooth Wideband Active */
#define APU_BT_STATUS_BT_WIDEBAND_ACTIVE(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_BT_WIDEBAND_ACTIVE_SHIFT)) & APU_BT_STATUS_BT_WIDEBAND_ACTIVE_MASK)

#define APU_BT_STATUS_HOST_WKUP_IN_MASK          (0xFFFF0000U)
#define APU_BT_STATUS_HOST_WKUP_IN_SHIFT         (16U)
/*! HOST_WKUP_IN - Host Wakeup In */
#define APU_BT_STATUS_HOST_WKUP_IN(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT_STATUS_HOST_WKUP_IN_SHIFT)) & APU_BT_STATUS_HOST_WKUP_IN_MASK)
/*! @} */

/*! @name BT_CKEN_CTRL - Bluetooth Clock Enable Control */
/*! @{ */

#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_PWR_MASK  (0x1U)
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_PWR_SHIFT (0U)
/*! BT_CLK_EN_VAL_PWR - control value for bt_clk_en when power ready */
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_PWR(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_PWR_SHIFT)) & APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_PWR_MASK)

#define APU_BT_CKEN_CTRL_BL_CLK_EN_SEL_PWR_MASK  (0x2U)
#define APU_BT_CKEN_CTRL_BL_CLK_EN_SEL_PWR_SHIFT (1U)
/*! BL_CLK_EN_SEL_PWR - selection for bt_clk_en when power ready */
#define APU_BT_CKEN_CTRL_BL_CLK_EN_SEL_PWR(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BL_CLK_EN_SEL_PWR_SHIFT)) & APU_BT_CKEN_CTRL_BL_CLK_EN_SEL_PWR_MASK)

#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_MASK (0x4U)
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_SHIFT (2U)
/*! BT_CLK_EN_VAL_XOSC - control value for bt_clk_en when XOSC ready */
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_XOSC(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_SHIFT)) & APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_MASK)

#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_MASK (0x8U)
#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_SHIFT (3U)
/*! BT_CLK_EN_SEL_XOSC - selection for bt_clk_en when XOSC ready */
#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_XOSC(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_SHIFT)) & APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_MASK)

#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_MASK      (0x10U)
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_SHIFT     (4U)
/*! BT_CLK_EN_VAL - control value for bt_clk_en when PLL ready */
#define APU_BT_CKEN_CTRL_BT_CLK_EN_VAL(x)        (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_SHIFT)) & APU_BT_CKEN_CTRL_BT_CLK_EN_VAL_MASK)

#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_MASK      (0x20U)
#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_SHIFT     (5U)
/*! BT_CLK_EN_SEL - selection for bt_clk_en when PLL ready */
#define APU_BT_CKEN_CTRL_BT_CLK_EN_SEL(x)        (((uint32_t)(((uint32_t)(x)) << APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_SHIFT)) & APU_BT_CKEN_CTRL_BT_CLK_EN_SEL_MASK)
/*! @} */

/*! @name BT_RESRC_CTRL - Bluetooth RESRC Control */
/*! @{ */

#define APU_BT_RESRC_CTRL_FW_PWR_REQ_MASK        (0x1U)
#define APU_BT_RESRC_CTRL_FW_PWR_REQ_SHIFT       (0U)
/*! FW_PWR_REQ - override hw power request */
#define APU_BT_RESRC_CTRL_FW_PWR_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_PWR_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_PWR_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_PWR_REQ_VAL_MASK    (0x2U)
#define APU_BT_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT   (1U)
/*! FW_PWR_REQ_VAL - override value when apu_bt_resrc_ctrl[0] is set */
#define APU_BT_RESRC_CTRL_FW_PWR_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_PWR_REQ_VAL_MASK)

#define APU_BT_RESRC_CTRL_FW_XOSC_REQ_MASK       (0x4U)
#define APU_BT_RESRC_CTRL_FW_XOSC_REQ_SHIFT      (2U)
/*! FW_XOSC_REQ - override hw xosc request */
#define APU_BT_RESRC_CTRL_FW_XOSC_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_XOSC_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_XOSC_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK   (0x8U)
#define APU_BT_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT  (3U)
/*! FW_XOSC_REQ_VAL - override value when apu_bt_resrc_ctrl[2] is set */
#define APU_BT_RESRC_CTRL_FW_XOSC_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK)

#define APU_BT_RESRC_CTRL_FW_XP_REQ_MASK         (0x10U)
#define APU_BT_RESRC_CTRL_FW_XP_REQ_SHIFT        (4U)
/*! FW_XP_REQ - override hw xosc + pwr request */
#define APU_BT_RESRC_CTRL_FW_XP_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_XP_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_XP_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_XP_REQ_VAL_MASK     (0x20U)
#define APU_BT_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT    (5U)
/*! FW_XP_REQ_VAL - override value when apu_bt_resrc_ctrl[4] is set */
#define APU_BT_RESRC_CTRL_FW_XP_REQ_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_XP_REQ_VAL_MASK)

#define APU_BT_RESRC_CTRL_FW_SB_REQ_MASK         (0x40U)
#define APU_BT_RESRC_CTRL_FW_SB_REQ_SHIFT        (6U)
/*! FW_SB_REQ - override hw xosc + pwr + pll sb request */
#define APU_BT_RESRC_CTRL_FW_SB_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_SB_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_SB_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_SB_REQ_VAL_MASK     (0x80U)
#define APU_BT_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT    (7U)
/*! FW_SB_REQ_VAL - override value when apu_bt_resrc_ctrl[6] is set */
#define APU_BT_RESRC_CTRL_FW_SB_REQ_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_SB_REQ_VAL_MASK)

#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_MASK     (0x100U)
#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT    (8U)
/*! FW_IPWAKE_REQ - override hw xosc + pwr + pll ipwake request */
#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ(x)       (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK (0x200U)
#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT (9U)
/*! FW_IPWAKE_REQ_VAL - override value when apu_bt_resrc_ctrl[8] is set */
#define APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_VAL(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK)

#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_MASK    (0x400U)
#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT   (10U)
/*! FW_ALLWAKE_REQ - firmware override hw xosc + pwr + pll allwake request */
#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT)) & APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_MASK)

#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK (0x800U)
#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT (11U)
/*! FW_ALLWAKE_REQ_VAL - firmware override value when apu_bt_resrc_ctrl[10] is set */
#define APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT)) & APU_BT_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK)
/*! @} */

/*! @name BT_DVFS_CTRL - Bluetooth DVFS Control */
/*! @{ */

#define APU_BT_DVFS_CTRL_BT_VOL_VAL_MASK         (0x7FU)
#define APU_BT_DVFS_CTRL_BT_VOL_VAL_SHIFT        (0U)
/*! BT_VOL_VAL - Blueooth Vol Value */
#define APU_BT_DVFS_CTRL_BT_VOL_VAL(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT_DVFS_CTRL_BT_VOL_VAL_SHIFT)) & APU_BT_DVFS_CTRL_BT_VOL_VAL_MASK)
/*! @} */

/*! @name FM_CTRL - FM Control */
/*! @{ */

#define APU_FM_CTRL_FM_SLP_RDY_MASK              (0x1U)
#define APU_FM_CTRL_FM_SLP_RDY_SHIFT             (0U)
/*! FM_SLP_RDY - FM Sleep Delay */
#define APU_FM_CTRL_FM_SLP_RDY(x)                (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_SLP_RDY_SHIFT)) & APU_FM_CTRL_FM_SLP_RDY_MASK)

#define APU_FM_CTRL_FM_SLP_RDYMASK_MASK          (0x2U)
#define APU_FM_CTRL_FM_SLP_RDYMASK_SHIFT         (1U)
/*! FM_SLP_RDYMASK - FM Sleep Ready Mask */
#define APU_FM_CTRL_FM_SLP_RDYMASK(x)            (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_SLP_RDYMASK_SHIFT)) & APU_FM_CTRL_FM_SLP_RDYMASK_MASK)

#define APU_FM_CTRL_FM_SLP_RDY_FW_MASK           (0x4U)
#define APU_FM_CTRL_FM_SLP_RDY_FW_SHIFT          (2U)
/*! FM_SLP_RDY_FW - FM Sleep Ready FW */
#define APU_FM_CTRL_FM_SLP_RDY_FW(x)             (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_SLP_RDY_FW_SHIFT)) & APU_FM_CTRL_FM_SLP_RDY_FW_MASK)

#define APU_FM_CTRL_FM_PRESENT_MASK              (0x8U)
#define APU_FM_CTRL_FM_PRESENT_SHIFT             (3U)
/*! FM_PRESENT - FM Present */
#define APU_FM_CTRL_FM_PRESENT(x)                (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_PRESENT_SHIFT)) & APU_FM_CTRL_FM_PRESENT_MASK)

#define APU_FM_CTRL_FM_DYN_VOL_EN_MASK           (0x10U)
#define APU_FM_CTRL_FM_DYN_VOL_EN_SHIFT          (4U)
/*! FM_DYN_VOL_EN - FM Dynamic Vol Enable */
#define APU_FM_CTRL_FM_DYN_VOL_EN(x)             (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_DYN_VOL_EN_SHIFT)) & APU_FM_CTRL_FM_DYN_VOL_EN_MASK)

#define APU_FM_CTRL_FM_CLK_SYNC_MODE_SEL_MASK    (0x20U)
#define APU_FM_CTRL_FM_CLK_SYNC_MODE_SEL_SHIFT   (5U)
/*! FM_CLK_SYNC_MODE_SEL - FM Clock Sync Mode Select */
#define APU_FM_CTRL_FM_CLK_SYNC_MODE_SEL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_CLK_SYNC_MODE_SEL_SHIFT)) & APU_FM_CTRL_FM_CLK_SYNC_MODE_SEL_MASK)

#define APU_FM_CTRL_USE_FM_INTR_SLP_MASK         (0x80U)
#define APU_FM_CTRL_USE_FM_INTR_SLP_SHIFT        (7U)
/*! USE_FM_INTR_SLP - Use FM Interrupt Sleep */
#define APU_FM_CTRL_USE_FM_INTR_SLP(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_USE_FM_INTR_SLP_SHIFT)) & APU_FM_CTRL_USE_FM_INTR_SLP_MASK)

#define APU_FM_CTRL_FM_HOST_SLP_RDY_MASK         (0x80000000U)
#define APU_FM_CTRL_FM_HOST_SLP_RDY_SHIFT        (31U)
/*! FM_HOST_SLP_RDY - FM Host Sleep Ready */
#define APU_FM_CTRL_FM_HOST_SLP_RDY(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_CTRL_FM_HOST_SLP_RDY_SHIFT)) & APU_FM_CTRL_FM_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name FM_WKUP_MASK - FM Wakeup Mask */
/*! @{ */

#define APU_FM_WKUP_MASK_FM_CLK_REQ_MASK_MASK    (0x1U)
#define APU_FM_WKUP_MASK_FM_CLK_REQ_MASK_SHIFT   (0U)
/*! FM_CLK_REQ_MASK - FM Clock Request Mask */
#define APU_FM_WKUP_MASK_FM_CLK_REQ_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_FM_WKUP_MASK_FM_CLK_REQ_MASK_SHIFT)) & APU_FM_WKUP_MASK_FM_CLK_REQ_MASK_MASK)

#define APU_FM_WKUP_MASK_FM_INTR_MASK_MASK       (0x2U)
#define APU_FM_WKUP_MASK_FM_INTR_MASK_SHIFT      (1U)
/*! FM_INTR_MASK - FM Interrupt Mask */
#define APU_FM_WKUP_MASK_FM_INTR_MASK(x)         (((uint32_t)(((uint32_t)(x)) << APU_FM_WKUP_MASK_FM_INTR_MASK_SHIFT)) & APU_FM_WKUP_MASK_FM_INTR_MASK_MASK)

#define APU_FM_WKUP_MASK_FM_PRESENT_MASK_MASK    (0x4U)
#define APU_FM_WKUP_MASK_FM_PRESENT_MASK_SHIFT   (2U)
/*! FM_PRESENT_MASK - FM Present Mask */
#define APU_FM_WKUP_MASK_FM_PRESENT_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_FM_WKUP_MASK_FM_PRESENT_MASK_SHIFT)) & APU_FM_WKUP_MASK_FM_PRESENT_MASK_MASK)

#define APU_FM_WKUP_MASK_FM_HOST_MAP_MASK        (0xFFFF0000U)
#define APU_FM_WKUP_MASK_FM_HOST_MAP_SHIFT       (16U)
/*! FM_HOST_MAP - FM to host-HOST Delay */
#define APU_FM_WKUP_MASK_FM_HOST_MAP(x)          (((uint32_t)(((uint32_t)(x)) << APU_FM_WKUP_MASK_FM_HOST_MAP_SHIFT)) & APU_FM_WKUP_MASK_FM_HOST_MAP_MASK)
/*! @} */

/*! @name FM_STATUS - FM Status */
/*! @{ */

#define APU_FM_STATUS_APU_FM_CLK_EN_MASK         (0x1U)
#define APU_FM_STATUS_APU_FM_CLK_EN_SHIFT        (0U)
/*! APU_FM_CLK_EN - APU FM Clock Enable */
#define APU_FM_STATUS_APU_FM_CLK_EN(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_APU_FM_CLK_EN_SHIFT)) & APU_FM_STATUS_APU_FM_CLK_EN_MASK)

#define APU_FM_STATUS_SOC_CLK_EN2_MASK           (0x2U)
#define APU_FM_STATUS_SOC_CLK_EN2_SHIFT          (1U)
/*! SOC_CLK_EN2 - SoC Clock Enable 2 */
#define APU_FM_STATUS_SOC_CLK_EN2(x)             (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_SOC_CLK_EN2_SHIFT)) & APU_FM_STATUS_SOC_CLK_EN2_MASK)

#define APU_FM_STATUS_XOSC_STABLE_REFCK_MASK     (0x4U)
#define APU_FM_STATUS_XOSC_STABLE_REFCK_SHIFT    (2U)
/*! XOSC_STABLE_REFCK - XOSC Stable Ref Clock */
#define APU_FM_STATUS_XOSC_STABLE_REFCK(x)       (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_XOSC_STABLE_REFCK_SHIFT)) & APU_FM_STATUS_XOSC_STABLE_REFCK_MASK)

#define APU_FM_STATUS_FM_HOST_WKUP_MASK          (0x10U)
#define APU_FM_STATUS_FM_HOST_WKUP_SHIFT         (4U)
/*! FM_HOST_WKUP - FM Host Wakeup */
#define APU_FM_STATUS_FM_HOST_WKUP(x)            (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_FM_HOST_WKUP_SHIFT)) & APU_FM_STATUS_FM_HOST_WKUP_MASK)

#define APU_FM_STATUS_FM_INTR_MASK               (0x20U)
#define APU_FM_STATUS_FM_INTR_SHIFT              (5U)
/*! FM_INTR - FM Interrupt */
#define APU_FM_STATUS_FM_INTR(x)                 (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_FM_INTR_SHIFT)) & APU_FM_STATUS_FM_INTR_MASK)

#define APU_FM_STATUS_FM_CLK_REQ_MASK            (0x40U)
#define APU_FM_STATUS_FM_CLK_REQ_SHIFT           (6U)
/*! FM_CLK_REQ - FM Clock Request */
#define APU_FM_STATUS_FM_CLK_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_FM_CLK_REQ_SHIFT)) & APU_FM_STATUS_FM_CLK_REQ_MASK)

#define APU_FM_STATUS_FM_PRESENT_MASK            (0x80U)
#define APU_FM_STATUS_FM_PRESENT_SHIFT           (7U)
/*! FM_PRESENT - FM Present */
#define APU_FM_STATUS_FM_PRESENT(x)              (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_FM_PRESENT_SHIFT)) & APU_FM_STATUS_FM_PRESENT_MASK)

#define APU_FM_STATUS_HOST_WKUP_IN_MASK          (0xFFFF0000U)
#define APU_FM_STATUS_HOST_WKUP_IN_SHIFT         (16U)
/*! HOST_WKUP_IN - Host Wakeup Interrupt */
#define APU_FM_STATUS_HOST_WKUP_IN(x)            (((uint32_t)(((uint32_t)(x)) << APU_FM_STATUS_HOST_WKUP_IN_SHIFT)) & APU_FM_STATUS_HOST_WKUP_IN_MASK)
/*! @} */

/*! @name FM_CKEN_CTRL - FM Clock Enable Control */
/*! @{ */

#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_VAL_MASK  (0x1U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_VAL_SHIFT (0U)
/*! FM_CLK_EN_PWR_VAL - control value for fm_clk_en when power ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_VAL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_VAL_MASK)

#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_SEL_MASK  (0x2U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_SEL_SHIFT (1U)
/*! FM_CLK_EN_PWR_SEL - selection for fm_clk_en when power ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_SEL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_SEL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_PWR_SEL_MASK)

#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_VAL_MASK (0x4U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_VAL_SHIFT (2U)
/*! FM_CLK_EN_XOSC_VAL - control value for fm_clk_en when xosc is ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_VAL(x)   (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_VAL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_VAL_MASK)

#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_SEL_MASK (0x8U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_SEL_SHIFT (3U)
/*! FM_CLK_EN_XOSC_SEL - selection for fm_clk_en when XOSC ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_SEL(x)   (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_SEL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_XOSC_SEL_MASK)

#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_VAL_MASK  (0x10U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_VAL_SHIFT (4U)
/*! FM_CLK_EN_PLL_VAL - control value for fm_clk_en when PLL ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_VAL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_VAL_MASK)

#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_SEL_MASK  (0x20U)
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_SEL_SHIFT (5U)
/*! FM_CLK_EN_PLL_SEL - selection for fm_clk_en when PLL ready */
#define APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_SEL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_SEL_SHIFT)) & APU_FM_CKEN_CTRL_FM_CLK_EN_PLL_SEL_MASK)
/*! @} */

/*! @name FM_RESRC_CTRL - FM RESRC Control */
/*! @{ */

#define APU_FM_RESRC_CTRL_FM_PWR_REQ_MASK        (0x1U)
#define APU_FM_RESRC_CTRL_FM_PWR_REQ_SHIFT       (0U)
/*! FM_PWR_REQ - override hw power request */
#define APU_FM_RESRC_CTRL_FM_PWR_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_PWR_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_PWR_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_PWR_REQ_VAL_MASK    (0x2U)
#define APU_FM_RESRC_CTRL_FM_PWR_REQ_VAL_SHIFT   (1U)
/*! FM_PWR_REQ_VAL - override value when apu_fm_resrc_ctrl[0] is set */
#define APU_FM_RESRC_CTRL_FM_PWR_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_PWR_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_PWR_REQ_VAL_MASK)

#define APU_FM_RESRC_CTRL_FM_XOSC_REQ_MASK       (0x4U)
#define APU_FM_RESRC_CTRL_FM_XOSC_REQ_SHIFT      (2U)
/*! FM_XOSC_REQ - override hw xosc request */
#define APU_FM_RESRC_CTRL_FM_XOSC_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_XOSC_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_XOSC_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_XOSC_REQ_VAL_MASK   (0x8U)
#define APU_FM_RESRC_CTRL_FM_XOSC_REQ_VAL_SHIFT  (3U)
/*! FM_XOSC_REQ_VAL - override value when apu_fm_resrc_ctrl[2] is set */
#define APU_FM_RESRC_CTRL_FM_XOSC_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_XOSC_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_XOSC_REQ_VAL_MASK)

#define APU_FM_RESRC_CTRL_FM_XP_REQ_MASK         (0x10U)
#define APU_FM_RESRC_CTRL_FM_XP_REQ_SHIFT        (4U)
/*! FM_XP_REQ - override hw xosc + pwr request */
#define APU_FM_RESRC_CTRL_FM_XP_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_XP_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_XP_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_XP_REQ_VAL_MASK     (0x20U)
#define APU_FM_RESRC_CTRL_FM_XP_REQ_VAL_SHIFT    (5U)
/*! FM_XP_REQ_VAL - override value when apu_fm_resrc_ctrl[4] is set */
#define APU_FM_RESRC_CTRL_FM_XP_REQ_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_XP_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_XP_REQ_VAL_MASK)

#define APU_FM_RESRC_CTRL_FM_SB_REQ_MASK         (0x40U)
#define APU_FM_RESRC_CTRL_FM_SB_REQ_SHIFT        (6U)
/*! FM_SB_REQ - override hw xosc + pwr + pll sb request */
#define APU_FM_RESRC_CTRL_FM_SB_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_SB_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_SB_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_SB_REQ_VAL_MASK     (0x80U)
#define APU_FM_RESRC_CTRL_FM_SB_REQ_VAL_SHIFT    (7U)
/*! FM_SB_REQ_VAL - override value when apu_fm_resrc_ctrl[6] is set */
#define APU_FM_RESRC_CTRL_FM_SB_REQ_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_SB_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_SB_REQ_VAL_MASK)

#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_MASK     (0x100U)
#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_SHIFT    (8U)
/*! FM_IPWAKE_REQ - override hw xosc + pwr + pll ipwake request */
#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ(x)       (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_VAL_MASK (0x200U)
#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_VAL_SHIFT (9U)
/*! FM_IPWAKE_REQ_VAL - override value when apu_fm_resrc_ctrl[8] is set */
#define APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_VAL(x)   (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_IPWAKE_REQ_VAL_MASK)

#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_MASK    (0x400U)
#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_SHIFT   (10U)
/*! FM_ALLWAKE_REQ - firmware override hw xosc + pwr + pll allwake request */
#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ(x)      (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_SHIFT)) & APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_MASK)

#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_VAL_MASK (0x800U)
#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_VAL_SHIFT (11U)
/*! FM_ALLWAKE_REQ_VAL - firmware override value when apu_fm_resrc_ctrl[10] is set */
#define APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_VAL_SHIFT)) & APU_FM_RESRC_CTRL_FM_ALLWAKE_REQ_VAL_MASK)
/*! @} */

/*! @name FM_DVFS_CTRL - FM DVFS Control */
/*! @{ */

#define APU_FM_DVFS_CTRL_FM_VOL_VAL_MASK         (0x7FU)
#define APU_FM_DVFS_CTRL_FM_VOL_VAL_SHIFT        (0U)
/*! FM_VOL_VAL - FM Vol Value */
#define APU_FM_DVFS_CTRL_FM_VOL_VAL(x)           (((uint32_t)(((uint32_t)(x)) << APU_FM_DVFS_CTRL_FM_VOL_VAL_SHIFT)) & APU_FM_DVFS_CTRL_FM_VOL_VAL_MASK)
/*! @} */

/*! @name USB_PWR_CTRL_DLY - USB FSM Power Control Delay */
/*! @{ */

#define APU_USB_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK   (0xFFFFFFFFU)
#define APU_USB_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT  (0U)
/*! PWR_CTRL_DLY - USB FSM Power Control Delay */
#define APU_USB_PWR_CTRL_DLY_PWR_CTRL_DLY(x)     (((uint32_t)(((uint32_t)(x)) << APU_USB_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT)) & APU_USB_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK)
/*! @} */

/*! @name USB_PWR_CTRL_DLY2 - USB FSM Power Control Delay 2 */
/*! @{ */

#define APU_USB_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK (0xFFFFFFFFU)
#define APU_USB_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT (0U)
/*! PWR_CTRL_DLY2 - USB FSM Power Control Delay 2 */
#define APU_USB_PWR_CTRL_DLY2_PWR_CTRL_DLY2(x)   (((uint32_t)(((uint32_t)(x)) << APU_USB_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT)) & APU_USB_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK)
/*! @} */

/*! @name WLCOMN_PWR_CTRL_DLY - Wlan common domain FSM Power Control Delay */
/*! @{ */

#define APU_WLCOMN_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK (0xFFFFFFFFU)
#define APU_WLCOMN_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT (0U)
/*! PWR_CTRL_DLY - WLAN COMN FSM power control delay */
#define APU_WLCOMN_PWR_CTRL_DLY_PWR_CTRL_DLY(x)  (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_DLY_PWR_CTRL_DLY_SHIFT)) & APU_WLCOMN_PWR_CTRL_DLY_PWR_CTRL_DLY_MASK)
/*! @} */

/*! @name WLCOMN_PWR_CTRL_DLY2 - Wlan common domain FSM Power Control Delay 2 */
/*! @{ */

#define APU_WLCOMN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK (0xFFFFFFFFU)
#define APU_WLCOMN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT (0U)
/*! PWR_CTRL_DLY2 - WLAN COMN FSM power control delay */
#define APU_WLCOMN_PWR_CTRL_DLY2_PWR_CTRL_DLY2(x) (((uint32_t)(((uint32_t)(x)) << APU_WLCOMN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_SHIFT)) & APU_WLCOMN_PWR_CTRL_DLY2_PWR_CTRL_DLY2_MASK)
/*! @} */

/*! @name BLE_CTRL - BLE Control */
/*! @{ */

#define APU_BLE_CTRL_BLE_SLP_RDY_MASK            (0x1U)
#define APU_BLE_CTRL_BLE_SLP_RDY_SHIFT           (0U)
/*! BLE_SLP_RDY - BLE Sleep Ready */
#define APU_BLE_CTRL_BLE_SLP_RDY(x)              (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BLE_SLP_RDY_SHIFT)) & APU_BLE_CTRL_BLE_SLP_RDY_MASK)

#define APU_BLE_CTRL_BLE_SLP_RDYMASK_MASK        (0x2U)
#define APU_BLE_CTRL_BLE_SLP_RDYMASK_SHIFT       (1U)
/*! BLE_SLP_RDYMASK - BLE Sleep Ready Mask */
#define APU_BLE_CTRL_BLE_SLP_RDYMASK(x)          (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BLE_SLP_RDYMASK_SHIFT)) & APU_BLE_CTRL_BLE_SLP_RDYMASK_MASK)

#define APU_BLE_CTRL_BLE_SLP_RDY_FW_MASK         (0x4U)
#define APU_BLE_CTRL_BLE_SLP_RDY_FW_SHIFT        (2U)
/*! BLE_SLP_RDY_FW - BLE Sleep Ready FW */
#define APU_BLE_CTRL_BLE_SLP_RDY_FW(x)           (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BLE_SLP_RDY_FW_SHIFT)) & APU_BLE_CTRL_BLE_SLP_RDY_FW_MASK)

#define APU_BLE_CTRL_BLE_MODE_EN_MASK            (0x8U)
#define APU_BLE_CTRL_BLE_MODE_EN_SHIFT           (3U)
/*! BLE_MODE_EN - BLE Mode Enable */
#define APU_BLE_CTRL_BLE_MODE_EN(x)              (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BLE_MODE_EN_SHIFT)) & APU_BLE_CTRL_BLE_MODE_EN_MASK)

#define APU_BLE_CTRL_BT_AES_NCO_MODE_MASK        (0x8000U)
#define APU_BLE_CTRL_BT_AES_NCO_MODE_SHIFT       (15U)
/*! BT_AES_NCO_MODE - Bluetooth AES NCO Mode */
#define APU_BLE_CTRL_BT_AES_NCO_MODE(x)          (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BT_AES_NCO_MODE_SHIFT)) & APU_BLE_CTRL_BT_AES_NCO_MODE_MASK)

#define APU_BLE_CTRL_BLE_XP_REQ_MASK             (0x80000000U)
#define APU_BLE_CTRL_BLE_XP_REQ_SHIFT            (31U)
/*! BLE_XP_REQ - BLE XP Request */
#define APU_BLE_CTRL_BLE_XP_REQ(x)               (((uint32_t)(((uint32_t)(x)) << APU_BLE_CTRL_BLE_XP_REQ_SHIFT)) & APU_BLE_CTRL_BLE_XP_REQ_MASK)
/*! @} */

/*! @name BLE_WKUP_MASK - BLE Wakeup Mask */
/*! @{ */

#define APU_BLE_WKUP_MASK_BT_AES_CLK_REQ_MASK_MASK (0x8U)
#define APU_BLE_WKUP_MASK_BT_AES_CLK_REQ_MASK_SHIFT (3U)
/*! BT_AES_CLK_REQ_MASK - Bluetooth AES Clock Request Mask */
#define APU_BLE_WKUP_MASK_BT_AES_CLK_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_BLE_WKUP_MASK_BT_AES_CLK_REQ_MASK_SHIFT)) & APU_BLE_WKUP_MASK_BT_AES_CLK_REQ_MASK_MASK)

#define APU_BLE_WKUP_MASK_LBC_XP_REQ_MASK_MASK   (0x10U)
#define APU_BLE_WKUP_MASK_LBC_XP_REQ_MASK_SHIFT  (4U)
/*! LBC_XP_REQ_MASK - LBC XP Request Mask */
#define APU_BLE_WKUP_MASK_LBC_XP_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BLE_WKUP_MASK_LBC_XP_REQ_MASK_SHIFT)) & APU_BLE_WKUP_MASK_LBC_XP_REQ_MASK_MASK)
/*! @} */

/*! @name BLE_STATUS - BLE Status */
/*! @{ */

#define APU_BLE_STATUS_APU_BT_AES_CLK_EN_MASK    (0x1U)
#define APU_BLE_STATUS_APU_BT_AES_CLK_EN_SHIFT   (0U)
/*! APU_BT_AES_CLK_EN - APU Bluetooth AES Clock Enable */
#define APU_BLE_STATUS_APU_BT_AES_CLK_EN(x)      (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_APU_BT_AES_CLK_EN_SHIFT)) & APU_BLE_STATUS_APU_BT_AES_CLK_EN_MASK)

#define APU_BLE_STATUS_BT_AES_CLK_SEL_MASK       (0x2U)
#define APU_BLE_STATUS_BT_AES_CLK_SEL_SHIFT      (1U)
/*! BT_AES_CLK_SEL - Bluetooth AES Clock Select */
#define APU_BLE_STATUS_BT_AES_CLK_SEL(x)         (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_BT_AES_CLK_SEL_SHIFT)) & APU_BLE_STATUS_BT_AES_CLK_SEL_MASK)

#define APU_BLE_STATUS_XOSC_STABLE_REFCK_MASK    (0x4U)
#define APU_BLE_STATUS_XOSC_STABLE_REFCK_SHIFT   (2U)
/*! XOSC_STABLE_REFCK - XOSC Stable Ref Clock */
#define APU_BLE_STATUS_XOSC_STABLE_REFCK(x)      (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_XOSC_STABLE_REFCK_SHIFT)) & APU_BLE_STATUS_XOSC_STABLE_REFCK_MASK)

#define APU_BLE_STATUS_T1_STABLE_MASK            (0x8U)
#define APU_BLE_STATUS_T1_STABLE_SHIFT           (3U)
/*! T1_STABLE - T1 Delay */
#define APU_BLE_STATUS_T1_STABLE(x)              (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_T1_STABLE_SHIFT)) & APU_BLE_STATUS_T1_STABLE_MASK)

#define APU_BLE_STATUS_BT_INTERRUPT_MASK         (0x10U)
#define APU_BLE_STATUS_BT_INTERRUPT_SHIFT        (4U)
/*! BT_INTERRUPT - Bluetooth Interrupt */
#define APU_BLE_STATUS_BT_INTERRUPT(x)           (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_BT_INTERRUPT_SHIFT)) & APU_BLE_STATUS_BT_INTERRUPT_MASK)

#define APU_BLE_STATUS_BLE_XP_REQ_MASK           (0x20U)
#define APU_BLE_STATUS_BLE_XP_REQ_SHIFT          (5U)
/*! BLE_XP_REQ - BLE XP Request */
#define APU_BLE_STATUS_BLE_XP_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_BLE_XP_REQ_SHIFT)) & APU_BLE_STATUS_BLE_XP_REQ_MASK)

#define APU_BLE_STATUS_BT_AES_NCO_MODE_MASK      (0x40U)
#define APU_BLE_STATUS_BT_AES_NCO_MODE_SHIFT     (6U)
/*! BT_AES_NCO_MODE - Blueooth AES NCO Mode */
#define APU_BLE_STATUS_BT_AES_NCO_MODE(x)        (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_BT_AES_NCO_MODE_SHIFT)) & APU_BLE_STATUS_BT_AES_NCO_MODE_MASK)

#define APU_BLE_STATUS_LBC_XP_REQ_MASK           (0x100U)
#define APU_BLE_STATUS_LBC_XP_REQ_SHIFT          (8U)
/*! LBC_XP_REQ - LBC XP Request */
#define APU_BLE_STATUS_LBC_XP_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_LBC_XP_REQ_SHIFT)) & APU_BLE_STATUS_LBC_XP_REQ_MASK)

#define APU_BLE_STATUS_BT_AES_CLK_REQ_MASK       (0x200U)
#define APU_BLE_STATUS_BT_AES_CLK_REQ_SHIFT      (9U)
/*! BT_AES_CLK_REQ - Bluetooth AES Clock Request Mask */
#define APU_BLE_STATUS_BT_AES_CLK_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_BLE_STATUS_BT_AES_CLK_REQ_SHIFT)) & APU_BLE_STATUS_BT_AES_CLK_REQ_MASK)
/*! @} */

/*! @name NFC_CTRL - NFC Control */
/*! @{ */

#define APU_NFC_CTRL_NFC_SLP_RDY_MASK            (0x1U)
#define APU_NFC_CTRL_NFC_SLP_RDY_SHIFT           (0U)
/*! NFC_SLP_RDY - NFC Sleep Ready */
#define APU_NFC_CTRL_NFC_SLP_RDY(x)              (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_SLP_RDY_SHIFT)) & APU_NFC_CTRL_NFC_SLP_RDY_MASK)

#define APU_NFC_CTRL_NFC_SLP_RDYMASK_MASK        (0x2U)
#define APU_NFC_CTRL_NFC_SLP_RDYMASK_SHIFT       (1U)
/*! NFC_SLP_RDYMASK - NFC Sleep Ready Mask */
#define APU_NFC_CTRL_NFC_SLP_RDYMASK(x)          (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_SLP_RDYMASK_SHIFT)) & APU_NFC_CTRL_NFC_SLP_RDYMASK_MASK)

#define APU_NFC_CTRL_NFC_SLP_RDY_FW_MASK         (0x4U)
#define APU_NFC_CTRL_NFC_SLP_RDY_FW_SHIFT        (2U)
/*! NFC_SLP_RDY_FW - NFC Sleep Ready FW */
#define APU_NFC_CTRL_NFC_SLP_RDY_FW(x)           (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_SLP_RDY_FW_SHIFT)) & APU_NFC_CTRL_NFC_SLP_RDY_FW_MASK)

#define APU_NFC_CTRL_USE_NFC_REF_ONLY_MASK       (0x8U)
#define APU_NFC_CTRL_USE_NFC_REF_ONLY_SHIFT      (3U)
/*! USE_NFC_REF_ONLY - Use NFC Reference Only */
#define APU_NFC_CTRL_USE_NFC_REF_ONLY(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_USE_NFC_REF_ONLY_SHIFT)) & APU_NFC_CTRL_USE_NFC_REF_ONLY_MASK)

#define APU_NFC_CTRL_USE_NFC_INTR_SLP_MASK       (0x10U)
#define APU_NFC_CTRL_USE_NFC_INTR_SLP_SHIFT      (4U)
/*! USE_NFC_INTR_SLP - Use NFC Interrupt Sleep */
#define APU_NFC_CTRL_USE_NFC_INTR_SLP(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_USE_NFC_INTR_SLP_SHIFT)) & APU_NFC_CTRL_USE_NFC_INTR_SLP_MASK)

#define APU_NFC_CTRL_NFC_USE_SOC_PWR_SEQ_MASK    (0x20U)
#define APU_NFC_CTRL_NFC_USE_SOC_PWR_SEQ_SHIFT   (5U)
/*! NFC_USE_SOC_PWR_SEQ - Use SoC power sequence for NFC as well */
#define APU_NFC_CTRL_NFC_USE_SOC_PWR_SEQ(x)      (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_USE_SOC_PWR_SEQ_SHIFT)) & APU_NFC_CTRL_NFC_USE_SOC_PWR_SEQ_MASK)

#define APU_NFC_CTRL_FW_CLR_NFC_INTR_MASK        (0x80U)
#define APU_NFC_CTRL_FW_CLR_NFC_INTR_SHIFT       (7U)
/*! FW_CLR_NFC_INTR - FW Clear NFC Interrupt */
#define APU_NFC_CTRL_FW_CLR_NFC_INTR(x)          (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_FW_CLR_NFC_INTR_SHIFT)) & APU_NFC_CTRL_FW_CLR_NFC_INTR_MASK)

#define APU_NFC_CTRL_NFC_USE_CPU2_SUBSLP_MASK    (0x100U)
#define APU_NFC_CTRL_NFC_USE_CPU2_SUBSLP_SHIFT   (8U)
/*! NFC_USE_CPU2_SUBSLP - Use CPU2 subsystem sleep for NFC power down */
#define APU_NFC_CTRL_NFC_USE_CPU2_SUBSLP(x)      (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_USE_CPU2_SUBSLP_SHIFT)) & APU_NFC_CTRL_NFC_USE_CPU2_SUBSLP_MASK)

#define APU_NFC_CTRL_NFC_USE_NOM_PWR_BYP_MASK    (0x200U)
#define APU_NFC_CTRL_NFC_USE_NOM_PWR_BYP_SHIFT   (9U)
/*! NFC_USE_NOM_PWR_BYP - Use nom_pwr_lvl from dvfs for nfc_pwr_lvl_reached */
#define APU_NFC_CTRL_NFC_USE_NOM_PWR_BYP(x)      (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_USE_NOM_PWR_BYP_SHIFT)) & APU_NFC_CTRL_NFC_USE_NOM_PWR_BYP_MASK)

#define APU_NFC_CTRL_FW_FORCE_NFC_PWRUP_MASK     (0x1000000U)
#define APU_NFC_CTRL_FW_FORCE_NFC_PWRUP_SHIFT    (24U)
/*! FW_FORCE_NFC_PWRUP - FW Force NFC Powerup */
#define APU_NFC_CTRL_FW_FORCE_NFC_PWRUP(x)       (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_FW_FORCE_NFC_PWRUP_SHIFT)) & APU_NFC_CTRL_FW_FORCE_NFC_PWRUP_MASK)

#define APU_NFC_CTRL_NFC_USE_UNSYNC_PWR_LVL_MASK (0x2000000U)
#define APU_NFC_CTRL_NFC_USE_UNSYNC_PWR_LVL_SHIFT (25U)
/*! NFC_USE_UNSYNC_PWR_LVL - NFC Use Unsync Power Level */
#define APU_NFC_CTRL_NFC_USE_UNSYNC_PWR_LVL(x)   (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_USE_UNSYNC_PWR_LVL_SHIFT)) & APU_NFC_CTRL_NFC_USE_UNSYNC_PWR_LVL_MASK)

#define APU_NFC_CTRL_NFC_HOST_SLP_RDY_MASK       (0x80000000U)
#define APU_NFC_CTRL_NFC_HOST_SLP_RDY_SHIFT      (31U)
/*! NFC_HOST_SLP_RDY - NFC Host Sleep Ready */
#define APU_NFC_CTRL_NFC_HOST_SLP_RDY(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_CTRL_NFC_HOST_SLP_RDY_SHIFT)) & APU_NFC_CTRL_NFC_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name NFC_WKUP_MASK - NFC Wakeup Mask */
/*! @{ */

#define APU_NFC_WKUP_MASK_NFC_P_WKUP_MASK_MASK   (0x1U)
#define APU_NFC_WKUP_MASK_NFC_P_WKUP_MASK_SHIFT  (0U)
/*! NFC_P_WKUP_MASK - NFC P Wakeup Mask */
#define APU_NFC_WKUP_MASK_NFC_P_WKUP_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_NFC_WKUP_MASK_NFC_P_WKUP_MASK_SHIFT)) & APU_NFC_WKUP_MASK_NFC_P_WKUP_MASK_MASK)

#define APU_NFC_WKUP_MASK_NFC_XP_WKUP_MASK_MASK  (0x2U)
#define APU_NFC_WKUP_MASK_NFC_XP_WKUP_MASK_SHIFT (1U)
/*! NFC_XP_WKUP_MASK - NFC XP Wakeup Mask */
#define APU_NFC_WKUP_MASK_NFC_XP_WKUP_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_NFC_WKUP_MASK_NFC_XP_WKUP_MASK_SHIFT)) & APU_NFC_WKUP_MASK_NFC_XP_WKUP_MASK_MASK)

#define APU_NFC_WKUP_MASK_NFC_INT_WKUP_MASK_MASK (0x4U)
#define APU_NFC_WKUP_MASK_NFC_INT_WKUP_MASK_SHIFT (2U)
/*! NFC_INT_WKUP_MASK - NFC Interrupt Wakeup Mask */
#define APU_NFC_WKUP_MASK_NFC_INT_WKUP_MASK(x)   (((uint32_t)(((uint32_t)(x)) << APU_NFC_WKUP_MASK_NFC_INT_WKUP_MASK_SHIFT)) & APU_NFC_WKUP_MASK_NFC_INT_WKUP_MASK_MASK)

#define APU_NFC_WKUP_MASK_NFC_HOST_MAP_MASK      (0xFFFF0000U)
#define APU_NFC_WKUP_MASK_NFC_HOST_MAP_SHIFT     (16U)
/*! NFC_HOST_MAP - NFC Host Map */
#define APU_NFC_WKUP_MASK_NFC_HOST_MAP(x)        (((uint32_t)(((uint32_t)(x)) << APU_NFC_WKUP_MASK_NFC_HOST_MAP_SHIFT)) & APU_NFC_WKUP_MASK_NFC_HOST_MAP_MASK)
/*! @} */

/*! @name NFC_STATUS - NFC Status */
/*! @{ */

#define APU_NFC_STATUS_APU_NFC_AHBCLK_EN_MASK    (0x1U)
#define APU_NFC_STATUS_APU_NFC_AHBCLK_EN_SHIFT   (0U)
/*! APU_NFC_AHBCLK_EN - APU NFC AHB Clock Enable */
#define APU_NFC_STATUS_APU_NFC_AHBCLK_EN(x)      (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_AHBCLK_EN_SHIFT)) & APU_NFC_STATUS_APU_NFC_AHBCLK_EN_MASK)

#define APU_NFC_STATUS_NFC_INT_MASK              (0x2U)
#define APU_NFC_STATUS_NFC_INT_SHIFT             (1U)
/*! NFC_INT - NFC Interrupt */
#define APU_NFC_STATUS_NFC_INT(x)                (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_NFC_INT_SHIFT)) & APU_NFC_STATUS_NFC_INT_MASK)

#define APU_NFC_STATUS_NFC_XP_REQ_MASK           (0x4U)
#define APU_NFC_STATUS_NFC_XP_REQ_SHIFT          (2U)
/*! NFC_XP_REQ - NFC XP Req */
#define APU_NFC_STATUS_NFC_XP_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_NFC_XP_REQ_SHIFT)) & APU_NFC_STATUS_NFC_XP_REQ_MASK)

#define APU_NFC_STATUS_NFC_P_REQ_MASK            (0x8U)
#define APU_NFC_STATUS_NFC_P_REQ_SHIFT           (3U)
/*! NFC_P_REQ - NFC P Req */
#define APU_NFC_STATUS_NFC_P_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_NFC_P_REQ_SHIFT)) & APU_NFC_STATUS_NFC_P_REQ_MASK)

#define APU_NFC_STATUS_APU_NFC_SWITCH_PD_AON_MASK (0x10U)
#define APU_NFC_STATUS_APU_NFC_SWITCH_PD_AON_SHIFT (4U)
/*! APU_NFC_SWITCH_PD_AON - APU NFC Switch PD AON */
#define APU_NFC_STATUS_APU_NFC_SWITCH_PD_AON(x)  (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_SWITCH_PD_AON_SHIFT)) & APU_NFC_STATUS_APU_NFC_SWITCH_PD_AON_MASK)

#define APU_NFC_STATUS_APU_NFC_UDR_FIREWALL_B_AON_MASK (0x20U)
#define APU_NFC_STATUS_APU_NFC_UDR_FIREWALL_B_AON_SHIFT (5U)
/*! APU_NFC_UDR_FIREWALL_B_AON - APU NFC UDR Firewall B AON */
#define APU_NFC_STATUS_APU_NFC_UDR_FIREWALL_B_AON(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_UDR_FIREWALL_B_AON_SHIFT)) & APU_NFC_STATUS_APU_NFC_UDR_FIREWALL_B_AON_MASK)

#define APU_NFC_STATUS_APU_NFC_CLK_DIV_RSTB_AON_MASK (0x40U)
#define APU_NFC_STATUS_APU_NFC_CLK_DIV_RSTB_AON_SHIFT (6U)
/*! APU_NFC_CLK_DIV_RSTB_AON - APU NFC Clock Divider RSTb AON */
#define APU_NFC_STATUS_APU_NFC_CLK_DIV_RSTB_AON(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_CLK_DIV_RSTB_AON_SHIFT)) & APU_NFC_STATUS_APU_NFC_CLK_DIV_RSTB_AON_MASK)

#define APU_NFC_STATUS_APU_NFC_ISO_EN_AON_MASK   (0x80U)
#define APU_NFC_STATUS_APU_NFC_ISO_EN_AON_SHIFT  (7U)
/*! APU_NFC_ISO_EN_AON - APU NFC ISO Enable AON */
#define APU_NFC_STATUS_APU_NFC_ISO_EN_AON(x)     (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_ISO_EN_AON_SHIFT)) & APU_NFC_STATUS_APU_NFC_ISO_EN_AON_MASK)

#define APU_NFC_STATUS_APU_NFC_SRAM_PD_AON_MASK  (0x100U)
#define APU_NFC_STATUS_APU_NFC_SRAM_PD_AON_SHIFT (8U)
/*! APU_NFC_SRAM_PD_AON - APU NFC SRAM PD AON */
#define APU_NFC_STATUS_APU_NFC_SRAM_PD_AON(x)    (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_SRAM_PD_AON_SHIFT)) & APU_NFC_STATUS_APU_NFC_SRAM_PD_AON_MASK)

#define APU_NFC_STATUS_NFC_VOL_REACHED_MASK      (0x200U)
#define APU_NFC_STATUS_NFC_VOL_REACHED_SHIFT     (9U)
/*! NFC_VOL_REACHED - NFC Vol Reached */
#define APU_NFC_STATUS_NFC_VOL_REACHED(x)        (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_NFC_VOL_REACHED_SHIFT)) & APU_NFC_STATUS_NFC_VOL_REACHED_MASK)

#define APU_NFC_STATUS_SOC_VOL_REACHED_MASK      (0x400U)
#define APU_NFC_STATUS_SOC_VOL_REACHED_SHIFT     (10U)
/*! SOC_VOL_REACHED - SoC Vol Reached */
#define APU_NFC_STATUS_SOC_VOL_REACHED(x)        (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_SOC_VOL_REACHED_SHIFT)) & APU_NFC_STATUS_SOC_VOL_REACHED_MASK)

#define APU_NFC_STATUS_NFC_PWR_RDY_MASK          (0x800U)
#define APU_NFC_STATUS_NFC_PWR_RDY_SHIFT         (11U)
/*! NFC_PWR_RDY - NFC Power ready */
#define APU_NFC_STATUS_NFC_PWR_RDY(x)            (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_NFC_PWR_RDY_SHIFT)) & APU_NFC_STATUS_NFC_PWR_RDY_MASK)

#define APU_NFC_STATUS_APU_NFC_ST_MASK           (0x7000U)
#define APU_NFC_STATUS_APU_NFC_ST_SHIFT          (12U)
/*! APU_NFC_ST - APU NFC St */
#define APU_NFC_STATUS_APU_NFC_ST(x)             (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_APU_NFC_ST_SHIFT)) & APU_NFC_STATUS_APU_NFC_ST_MASK)

#define APU_NFC_STATUS_HOST_WKUP_IN_MASK         (0xFFFF0000U)
#define APU_NFC_STATUS_HOST_WKUP_IN_SHIFT        (16U)
/*! HOST_WKUP_IN - Host Wakeup Interrupt */
#define APU_NFC_STATUS_HOST_WKUP_IN(x)           (((uint32_t)(((uint32_t)(x)) << APU_NFC_STATUS_HOST_WKUP_IN_SHIFT)) & APU_NFC_STATUS_HOST_WKUP_IN_MASK)
/*! @} */

/*! @name NFC_CKEN_CTRL - NFC Clock Enable Control */
/*! @{ */

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_VAL_MASK (0x1U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_VAL_SHIFT (0U)
/*! NFC_CLK_EN_PWR_VAL - control value for nfc_clk_en when power ready */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_VAL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_VAL_MASK)

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_SEL_MASK (0x2U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_SEL_SHIFT (1U)
/*! NFC_CLK_EN_PWR_SEL - selection for nfc_cm3_clk_en when power ready */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_SEL(x)  (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_SEL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_PWR_SEL_MASK)

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_VAL_MASK (0x4U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_VAL_SHIFT (2U)
/*! NFC_CLK_EN_XOSC_VAL - Control value for nfc_cm3_clk_en when XOSC ready */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_VAL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_VAL_MASK)

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_SEL_MASK (0x8U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_SEL_SHIFT (3U)
/*! NFC_CLK_EN_XOSC_SEL - selection for nfc_cm3_clk_en when XOSC ready */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_SEL(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_SEL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_XOSC_SEL_MASK)

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_VAL_MASK (0x10U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_VAL_SHIFT (4U)
/*! NFC_CLK_EN_PLL_VAL - NFC Clock Enable PLL Value */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_VAL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_VAL_MASK)

#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_SEL_MASK (0x20U)
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_SEL_SHIFT (5U)
/*! NFC_CLK_EN_PLL_SEL - selection for nfc_cm3_clk_en when PLL ready */
#define APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_SEL(x)  (((uint32_t)(((uint32_t)(x)) << APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_SEL_SHIFT)) & APU_NFC_CKEN_CTRL_NFC_CLK_EN_PLL_SEL_MASK)
/*! @} */

/*! @name NFC_RESRC_CTRL - NFC RESRC Control */
/*! @{ */

#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ_MASK      (0x1U)
#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ_SHIFT     (0U)
/*! NFC_PWR_REQ - override hw power request */
#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_PWR_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_PWR_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ_VAL_MASK  (0x2U)
#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ_VAL_SHIFT (1U)
/*! NFC_PWR_REQ_VAL - override value when apu_nfc_resrc_ctrl[0] is set */
#define APU_NFC_RESRC_CTRL_NFC_PWR_REQ_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_PWR_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_PWR_REQ_VAL_MASK)

#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_MASK     (0x4U)
#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_SHIFT    (2U)
/*! NFC_XOSC_REQ - override hw xosc request */
#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ(x)       (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_VAL_MASK (0x8U)
#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_VAL_SHIFT (3U)
/*! NFC_XOSC_REQ_VAL - override value when apu_nfc_resrc_ctrl[2] is set */
#define APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_VAL(x)   (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_XOSC_REQ_VAL_MASK)

#define APU_NFC_RESRC_CTRL_NFC_XP_REQ_MASK       (0x10U)
#define APU_NFC_RESRC_CTRL_NFC_XP_REQ_SHIFT      (4U)
/*! NFC_XP_REQ - override hw xosc + pwr request */
#define APU_NFC_RESRC_CTRL_NFC_XP_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_XP_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_XP_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_XP_REQ_VAL_MASK   (0x20U)
#define APU_NFC_RESRC_CTRL_NFC_XP_REQ_VAL_SHIFT  (5U)
/*! NFC_XP_REQ_VAL - override value when apu_nfc_resrc_ctrl[4] is set */
#define APU_NFC_RESRC_CTRL_NFC_XP_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_XP_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_XP_REQ_VAL_MASK)

#define APU_NFC_RESRC_CTRL_NFC_SB_REQ_MASK       (0x40U)
#define APU_NFC_RESRC_CTRL_NFC_SB_REQ_SHIFT      (6U)
/*! NFC_SB_REQ - override hw xosc + pwr + pll sb request */
#define APU_NFC_RESRC_CTRL_NFC_SB_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_SB_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_SB_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_SB_REQ_VAL_MASK   (0x80U)
#define APU_NFC_RESRC_CTRL_NFC_SB_REQ_VAL_SHIFT  (7U)
/*! NFC_SB_REQ_VAL - override value when apu_nfc_resrc_ctrl[6] is set */
#define APU_NFC_RESRC_CTRL_NFC_SB_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_SB_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_SB_REQ_VAL_MASK)

#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_MASK   (0x100U)
#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_SHIFT  (8U)
/*! NFC_IPWAKE_REQ - override hw xosc + pwr + pll ipwake request */
#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ(x)     (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_VAL_MASK (0x200U)
#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_VAL_SHIFT (9U)
/*! NFC_IPWAKE_REQ_VAL - override value when apu_nfc_resrc_ctrl[8] is set */
#define APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_IPWAKE_REQ_VAL_MASK)

#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_MASK  (0x400U)
#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_SHIFT (10U)
/*! NFC_ALLWAKE_REQ - firmware override hw xosc + pwr + pll allwake request */
#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ(x)    (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_MASK)

#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_VAL_MASK (0x800U)
#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_VAL_SHIFT (11U)
/*! NFC_ALLWAKE_REQ_VAL - firmware override value when apu_nfc_resrc_ctrl[10] is set */
#define APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_VAL_SHIFT)) & APU_NFC_RESRC_CTRL_NFC_ALLWAKE_REQ_VAL_MASK)
/*! @} */

/*! @name NFC_DVFS_CTRL - NFC DVFS Control */
/*! @{ */

#define APU_NFC_DVFS_CTRL_NFC_VOL_VAL_MASK       (0x7FU)
#define APU_NFC_DVFS_CTRL_NFC_VOL_VAL_SHIFT      (0U)
/*! NFC_VOL_VAL - NFC Vol Value */
#define APU_NFC_DVFS_CTRL_NFC_VOL_VAL(x)         (((uint32_t)(((uint32_t)(x)) << APU_NFC_DVFS_CTRL_NFC_VOL_VAL_SHIFT)) & APU_NFC_DVFS_CTRL_NFC_VOL_VAL_MASK)
/*! @} */

/*! @name FP4_CTRL - FP4(15.4) Control */
/*! @{ */

#define APU_FP4_CTRL_FP4_SLP_RDY_MASK            (0x1U)
#define APU_FP4_CTRL_FP4_SLP_RDY_SHIFT           (0U)
/*! FP4_SLP_RDY - FP4 Sleep Ready */
#define APU_FP4_CTRL_FP4_SLP_RDY(x)              (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_SLP_RDY_SHIFT)) & APU_FP4_CTRL_FP4_SLP_RDY_MASK)

#define APU_FP4_CTRL_FP4_SLP_RDYMASK_MASK        (0x2U)
#define APU_FP4_CTRL_FP4_SLP_RDYMASK_SHIFT       (1U)
/*! FP4_SLP_RDYMASK - FP4 Sleep Ready Mask */
#define APU_FP4_CTRL_FP4_SLP_RDYMASK(x)          (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_SLP_RDYMASK_SHIFT)) & APU_FP4_CTRL_FP4_SLP_RDYMASK_MASK)

#define APU_FP4_CTRL_FP4_SLP_RDY_FW_MASK         (0x4U)
#define APU_FP4_CTRL_FP4_SLP_RDY_FW_SHIFT        (2U)
/*! FP4_SLP_RDY_FW - Bluetooth Sleep Ready FW */
#define APU_FP4_CTRL_FP4_SLP_RDY_FW(x)           (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_SLP_RDY_FW_SHIFT)) & APU_FP4_CTRL_FP4_SLP_RDY_FW_MASK)

#define APU_FP4_CTRL_USE_FP4_INTR_SLP_MASK       (0x80U)
#define APU_FP4_CTRL_USE_FP4_INTR_SLP_SHIFT      (7U)
/*! USE_FP4_INTR_SLP - Use 15.4 interrupt Sleep */
#define APU_FP4_CTRL_USE_FP4_INTR_SLP(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_USE_FP4_INTR_SLP_SHIFT)) & APU_FP4_CTRL_USE_FP4_INTR_SLP_MASK)

#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL1_MASK (0x4000000U)
#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL1_SHIFT (26U)
/*! FP4_CLK_SYNC_MODE_SEL1 - FP4 Clock Sync Mode Select 1 */
#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL1_SHIFT)) & APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL1_MASK)

#define APU_FP4_CTRL_USE_GLITCH_FREE_FP4_CLK_REQ_MASK (0x8000000U)
#define APU_FP4_CTRL_USE_GLITCH_FREE_FP4_CLK_REQ_SHIFT (27U)
/*! USE_GLITCH_FREE_FP4_CLK_REQ - Use Glitch-Free FP4 Clock Request */
#define APU_FP4_CTRL_USE_GLITCH_FREE_FP4_CLK_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_USE_GLITCH_FREE_FP4_CLK_REQ_SHIFT)) & APU_FP4_CTRL_USE_GLITCH_FREE_FP4_CLK_REQ_MASK)

#define APU_FP4_CTRL_FRF_CLK_SYNC_MODE_SEL_MASK  (0x10000000U)
#define APU_FP4_CTRL_FRF_CLK_SYNC_MODE_SEL_SHIFT (28U)
/*! FRF_CLK_SYNC_MODE_SEL - FRF Clock Sync Mode Select */
#define APU_FP4_CTRL_FRF_CLK_SYNC_MODE_SEL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FRF_CLK_SYNC_MODE_SEL_SHIFT)) & APU_FP4_CTRL_FRF_CLK_SYNC_MODE_SEL_MASK)

#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL0_MASK (0x20000000U)
#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL0_SHIFT (29U)
/*! FP4_CLK_SYNC_MODE_SEL0 - FP4 Clock Sync Mode Select 0 */
#define APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL0_SHIFT)) & APU_FP4_CTRL_FP4_CLK_SYNC_MODE_SEL0_MASK)

#define APU_FP4_CTRL_FRF_PLL_SYNC_MODE_SEL_MASK  (0x40000000U)
#define APU_FP4_CTRL_FRF_PLL_SYNC_MODE_SEL_SHIFT (30U)
/*! FRF_PLL_SYNC_MODE_SEL - FRF PLL Sync Mode Select */
#define APU_FP4_CTRL_FRF_PLL_SYNC_MODE_SEL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FRF_PLL_SYNC_MODE_SEL_SHIFT)) & APU_FP4_CTRL_FRF_PLL_SYNC_MODE_SEL_MASK)

#define APU_FP4_CTRL_FP4_HOST_SLP_RDY_MASK       (0x80000000U)
#define APU_FP4_CTRL_FP4_HOST_SLP_RDY_SHIFT      (31U)
/*! FP4_HOST_SLP_RDY - FP4 Host Sleep Ready */
#define APU_FP4_CTRL_FP4_HOST_SLP_RDY(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_CTRL_FP4_HOST_SLP_RDY_SHIFT)) & APU_FP4_CTRL_FP4_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name FP4_WKUP_MASK - FP4 Wakeup Mask */
/*! @{ */

#define APU_FP4_WKUP_MASK_FP4_CLK_REQ_MASK_MASK  (0x1U)
#define APU_FP4_WKUP_MASK_FP4_CLK_REQ_MASK_SHIFT (0U)
/*! FP4_CLK_REQ_MASK - FP4 Clock Request Mask */
#define APU_FP4_WKUP_MASK_FP4_CLK_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_WKUP_MASK_FP4_CLK_REQ_MASK_SHIFT)) & APU_FP4_WKUP_MASK_FP4_CLK_REQ_MASK_MASK)

#define APU_FP4_WKUP_MASK_FP4_INTERRUPT_MASK_MASK (0x2U)
#define APU_FP4_WKUP_MASK_FP4_INTERRUPT_MASK_SHIFT (1U)
/*! FP4_INTERRUPT_MASK - FP4 Interrupt Mask */
#define APU_FP4_WKUP_MASK_FP4_INTERRUPT_MASK(x)  (((uint32_t)(((uint32_t)(x)) << APU_FP4_WKUP_MASK_FP4_INTERRUPT_MASK_SHIFT)) & APU_FP4_WKUP_MASK_FP4_INTERRUPT_MASK_MASK)

#define APU_FP4_WKUP_MASK_FRF_PLL_REQ_MASK_MASK  (0x4U)
#define APU_FP4_WKUP_MASK_FRF_PLL_REQ_MASK_SHIFT (2U)
/*! FRF_PLL_REQ_MASK - FRF PLL Request Mask */
#define APU_FP4_WKUP_MASK_FRF_PLL_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_WKUP_MASK_FRF_PLL_REQ_MASK_SHIFT)) & APU_FP4_WKUP_MASK_FRF_PLL_REQ_MASK_MASK)

#define APU_FP4_WKUP_MASK_FP4_PLL_REQ_MASK_MASK  (0x8U)
#define APU_FP4_WKUP_MASK_FP4_PLL_REQ_MASK_SHIFT (3U)
/*! FP4_PLL_REQ_MASK - 15.4 PLL Request Mask */
#define APU_FP4_WKUP_MASK_FP4_PLL_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_WKUP_MASK_FP4_PLL_REQ_MASK_SHIFT)) & APU_FP4_WKUP_MASK_FP4_PLL_REQ_MASK_MASK)

#define APU_FP4_WKUP_MASK_FP4_HOST_MAP_MASK      (0xFFFF0000U)
#define APU_FP4_WKUP_MASK_FP4_HOST_MAP_SHIFT     (16U)
/*! FP4_HOST_MAP - 15.4 Host Map */
#define APU_FP4_WKUP_MASK_FP4_HOST_MAP(x)        (((uint32_t)(((uint32_t)(x)) << APU_FP4_WKUP_MASK_FP4_HOST_MAP_SHIFT)) & APU_FP4_WKUP_MASK_FP4_HOST_MAP_MASK)
/*! @} */

/*! @name FP4_STATUS - FP4 Status */
/*! @{ */

#define APU_FP4_STATUS_APU_FP4_CLK_EN_MASK       (0x1U)
#define APU_FP4_STATUS_APU_FP4_CLK_EN_SHIFT      (0U)
/*! APU_FP4_CLK_EN - APU FP4 Clock Enable */
#define APU_FP4_STATUS_APU_FP4_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_APU_FP4_CLK_EN_SHIFT)) & APU_FP4_STATUS_APU_FP4_CLK_EN_MASK)

#define APU_FP4_STATUS_APU_FRF_CLK_EN_MASK       (0x2U)
#define APU_FP4_STATUS_APU_FRF_CLK_EN_SHIFT      (1U)
/*! APU_FRF_CLK_EN - APU FRF Clock Enable */
#define APU_FP4_STATUS_APU_FRF_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_APU_FRF_CLK_EN_SHIFT)) & APU_FP4_STATUS_APU_FRF_CLK_EN_MASK)

#define APU_FP4_STATUS_FP4_ACTIVE_SLPCK_MASK     (0x4U)
#define APU_FP4_STATUS_FP4_ACTIVE_SLPCK_SHIFT    (2U)
/*! FP4_ACTIVE_SLPCK - FP4 Active Sleep Clock */
#define APU_FP4_STATUS_FP4_ACTIVE_SLPCK(x)       (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_FP4_ACTIVE_SLPCK_SHIFT)) & APU_FP4_STATUS_FP4_ACTIVE_SLPCK_MASK)

#define APU_FP4_STATUS_BT_HOST_WKUP_MASK         (0x8U)
#define APU_FP4_STATUS_BT_HOST_WKUP_SHIFT        (3U)
/*! BT_HOST_WKUP - Bluetooth Host Wakeup */
#define APU_FP4_STATUS_BT_HOST_WKUP(x)           (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_BT_HOST_WKUP_SHIFT)) & APU_FP4_STATUS_BT_HOST_WKUP_MASK)

#define APU_FP4_STATUS_FP4_INTERRUPT_MASK        (0x10U)
#define APU_FP4_STATUS_FP4_INTERRUPT_SHIFT       (4U)
/*! FP4_INTERRUPT - FP4 Interrupt */
#define APU_FP4_STATUS_FP4_INTERRUPT(x)          (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_FP4_INTERRUPT_SHIFT)) & APU_FP4_STATUS_FP4_INTERRUPT_MASK)

#define APU_FP4_STATUS_FP4_PLL_REQ_MASK          (0x20U)
#define APU_FP4_STATUS_FP4_PLL_REQ_SHIFT         (5U)
/*! FP4_PLL_REQ - FP4 PLL Request */
#define APU_FP4_STATUS_FP4_PLL_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_FP4_PLL_REQ_SHIFT)) & APU_FP4_STATUS_FP4_PLL_REQ_MASK)

#define APU_FP4_STATUS_FP4_CLK_REQ_MASK          (0x40U)
#define APU_FP4_STATUS_FP4_CLK_REQ_SHIFT         (6U)
/*! FP4_CLK_REQ - FP4 Clock Request */
#define APU_FP4_STATUS_FP4_CLK_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_FP4_CLK_REQ_SHIFT)) & APU_FP4_STATUS_FP4_CLK_REQ_MASK)

#define APU_FP4_STATUS_FRF_PLL_REQ_MASK          (0x80U)
#define APU_FP4_STATUS_FRF_PLL_REQ_SHIFT         (7U)
/*! FRF_PLL_REQ - FRF PLL Request */
#define APU_FP4_STATUS_FRF_PLL_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_FRF_PLL_REQ_SHIFT)) & APU_FP4_STATUS_FRF_PLL_REQ_MASK)

#define APU_FP4_STATUS_HOST_WKUP_IN_MASK         (0xFFFF0000U)
#define APU_FP4_STATUS_HOST_WKUP_IN_SHIFT        (16U)
/*! HOST_WKUP_IN - Host Wakeup In */
#define APU_FP4_STATUS_HOST_WKUP_IN(x)           (((uint32_t)(((uint32_t)(x)) << APU_FP4_STATUS_HOST_WKUP_IN_SHIFT)) & APU_FP4_STATUS_HOST_WKUP_IN_MASK)
/*! @} */

/*! @name FP4_CKEN_CTRL - FP4 Clock Enable Control */
/*! @{ */

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_PWR_MASK (0x1U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_PWR_SHIFT (0U)
/*! FP4_CLK_EN_VAL_PWR - control value for fp4_clk_en when power ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_PWR(x)  (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_PWR_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_PWR_MASK)

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_PWR_MASK (0x2U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_PWR_SHIFT (1U)
/*! FP4_CLK_EN_SEL_PWR - selection for fp4_clk_en when power ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_PWR(x)  (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_PWR_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_PWR_MASK)

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_XOSC_MASK (0x4U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_XOSC_SHIFT (2U)
/*! FP4_CLK_EN_VAL_XOSC - control value for fp4_clk_en when XOSC ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_XOSC(x) (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_XOSC_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_XOSC_MASK)

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_XOSC_MASK (0x8U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_XOSC_SHIFT (3U)
/*! FP4_CLK_EN_SEL_XOSC - selection for fp4_clk_en when XOSC ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_XOSC(x) (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_XOSC_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_XOSC_MASK)

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_MASK    (0x10U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_SHIFT   (4U)
/*! FP4_CLK_EN_VAL - control value for fp4_clk_en when PLL ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_VAL_MASK)

#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_MASK    (0x20U)
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_SHIFT   (5U)
/*! FP4_CLK_EN_SEL - selection for fp4_clk_en when PLL ready */
#define APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_SHIFT)) & APU_FP4_CKEN_CTRL_FP4_CLK_EN_SEL_MASK)
/*! @} */

/*! @name FP4_RESRC_CTRL - FP4 RESRC Control */
/*! @{ */

#define APU_FP4_RESRC_CTRL_FW_PWR_REQ_MASK       (0x1U)
#define APU_FP4_RESRC_CTRL_FW_PWR_REQ_SHIFT      (0U)
/*! FW_PWR_REQ - override hw power request */
#define APU_FP4_RESRC_CTRL_FW_PWR_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_PWR_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_PWR_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_PWR_REQ_VAL_MASK   (0x2U)
#define APU_FP4_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT  (1U)
/*! FW_PWR_REQ_VAL - override value when apu_fp4_resrc_ctrl[0] is set */
#define APU_FP4_RESRC_CTRL_FW_PWR_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_PWR_REQ_VAL_MASK)

#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ_MASK      (0x4U)
#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ_SHIFT     (2U)
/*! FW_XOSC_REQ - override hw xosc request */
#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_XOSC_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_XOSC_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK  (0x8U)
#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT (3U)
/*! FW_XOSC_REQ_VAL - override value when apu_fp4_resrc_ctrl[2] is set */
#define APU_FP4_RESRC_CTRL_FW_XOSC_REQ_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK)

#define APU_FP4_RESRC_CTRL_FW_XP_REQ_MASK        (0x10U)
#define APU_FP4_RESRC_CTRL_FW_XP_REQ_SHIFT       (4U)
/*! FW_XP_REQ - override hw xosc + pwr request */
#define APU_FP4_RESRC_CTRL_FW_XP_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_XP_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_XP_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_XP_REQ_VAL_MASK    (0x20U)
#define APU_FP4_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT   (5U)
/*! FW_XP_REQ_VAL - override value when apu_fp4_resrc_ctrl[4] is set */
#define APU_FP4_RESRC_CTRL_FW_XP_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_XP_REQ_VAL_MASK)

#define APU_FP4_RESRC_CTRL_FW_SB_REQ_MASK        (0x40U)
#define APU_FP4_RESRC_CTRL_FW_SB_REQ_SHIFT       (6U)
/*! FW_SB_REQ - override hw xosc + pwr + pll sb request */
#define APU_FP4_RESRC_CTRL_FW_SB_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_SB_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_SB_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_SB_REQ_VAL_MASK    (0x80U)
#define APU_FP4_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT   (7U)
/*! FW_SB_REQ_VAL - override value when apu_fp4_resrc_ctrl[6] is set */
#define APU_FP4_RESRC_CTRL_FW_SB_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_SB_REQ_VAL_MASK)

#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_MASK    (0x100U)
#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT   (8U)
/*! FW_IPWAKE_REQ - override hw xosc + pwr + pll ipwake request */
#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ(x)      (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK (0x200U)
#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT (9U)
/*! FW_IPWAKE_REQ_VAL - override value when apu_fp4_resrc_ctrl[8] is set */
#define APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK)

#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_MASK   (0x400U)
#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT  (10U)
/*! FW_ALLWAKE_REQ - firmware override hw xosc + pwr + pll allwake request */
#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ(x)     (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT)) & APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_MASK)

#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK (0x800U)
#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT (11U)
/*! FW_ALLWAKE_REQ_VAL - firmware override value when apu_fp4_resrc_ctrl[10] is set */
#define APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT)) & APU_FP4_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK)
/*! @} */

/*! @name FP4_DVFS_CTRL - FP4 DVFS Control */
/*! @{ */

#define APU_FP4_DVFS_CTRL_FP4_VOL_VAL_MASK       (0x7FU)
#define APU_FP4_DVFS_CTRL_FP4_VOL_VAL_SHIFT      (0U)
/*! FP4_VOL_VAL - FP4 Vol Value */
#define APU_FP4_DVFS_CTRL_FP4_VOL_VAL(x)         (((uint32_t)(((uint32_t)(x)) << APU_FP4_DVFS_CTRL_FP4_VOL_VAL_SHIFT)) & APU_FP4_DVFS_CTRL_FP4_VOL_VAL_MASK)
/*! @} */

/*! @name CPU2_FP4_HOST_WKUP_MASK - CPU2 15.4 Host Wakeup Mask */
/*! @{ */

#define APU_CPU2_FP4_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK (0xFFFFU)
#define APU_CPU2_FP4_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT (0U)
/*! HOST_WKUP_MASK - Host Wakeup Mask */
#define APU_CPU2_FP4_HOST_WKUP_MASK_HOST_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FP4_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT)) & APU_CPU2_FP4_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK)
/*! @} */

/*! @name CPU2_FP4_HOST_WKUP_POL - CPU2 15.4 Host Wakeup Polarity */
/*! @{ */

#define APU_CPU2_FP4_HOST_WKUP_POL_HOST_WKUP_POL_MASK (0xFFFFU)
#define APU_CPU2_FP4_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT (0U)
/*! HOST_WKUP_POL - Host Wakeup Polarity */
#define APU_CPU2_FP4_HOST_WKUP_POL_HOST_WKUP_POL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FP4_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT)) & APU_CPU2_FP4_HOST_WKUP_POL_HOST_WKUP_POL_MASK)
/*! @} */

/*! @name CPU2_FP4_HOST_WKUP_CTRL - CPU2 15.4 Host Wakeup Control */
/*! @{ */

#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK (0xFFFFU)
#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT (0U)
/*! HOST_WKUP_CTRL - Host Wakeup Control */
#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_WKUP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT)) & APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK)

#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK (0xFFFF0000U)
#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT (16U)
/*! HOST_INTR_MASK - Host Interrupt Mask */
#define APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_INTR_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT)) & APU_CPU2_FP4_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK)
/*! @} */

/*! @name HW_IP_ACTIVE_INDEX_CTRL - HW IP active index Control */
/*! @{ */

#define APU_HW_IP_ACTIVE_INDEX_CTRL_MASTERX_ACTIVE_INDEX_MASK (0xFFFFFFFFU)
#define APU_HW_IP_ACTIVE_INDEX_CTRL_MASTERX_ACTIVE_INDEX_SHIFT (0U)
/*! MASTERX_ACTIVE_INDEX - master0-7 Active Index[3:0] */
#define APU_HW_IP_ACTIVE_INDEX_CTRL_MASTERX_ACTIVE_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_HW_IP_ACTIVE_INDEX_CTRL_MASTERX_ACTIVE_INDEX_SHIFT)) & APU_HW_IP_ACTIVE_INDEX_CTRL_MASTERX_ACTIVE_INDEX_MASK)
/*! @} */

/*! @name HW_IP_INACTIVE_INDEX_CTRL - HW IP inactive Control */
/*! @{ */

#define APU_HW_IP_INACTIVE_INDEX_CTRL_MASTERX_INACTIVE_INDEX_MASK (0xFFFFFFFFU)
#define APU_HW_IP_INACTIVE_INDEX_CTRL_MASTERX_INACTIVE_INDEX_SHIFT (0U)
/*! MASTERX_INACTIVE_INDEX - master0-7 inactive Index[3:0] */
#define APU_HW_IP_INACTIVE_INDEX_CTRL_MASTERX_INACTIVE_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_HW_IP_INACTIVE_INDEX_CTRL_MASTERX_INACTIVE_INDEX_SHIFT)) & APU_HW_IP_INACTIVE_INDEX_CTRL_MASTERX_INACTIVE_INDEX_MASK)
/*! @} */

/*! @name HW_IP_DYNAMIC_CLK_SWITCH_CTRL - HW IP dynamic clock switching contrl */
/*! @{ */

#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_EN_MASK (0xFFU)
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_EN_SHIFT (0U)
/*! MASTERX_APU_IDLE_BYPASS_EN - 1-FW bypasses hw_ip_idle; 0 - use hw_ip_idle */
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_EN_SHIFT)) & APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_EN_MASK)

#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_VAL_MASK (0xFF00U)
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_VAL_SHIFT (8U)
/*! MASTERX_APU_IDLE_BYPASS_VAL - idle bypass val */
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_VAL_SHIFT)) & APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_APU_IDLE_BYPASS_VAL_MASK)

#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_ACTIVE_INDEX_BYPASS_EN_MASK (0xFF0000U)
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_ACTIVE_INDEX_BYPASS_EN_SHIFT (16U)
/*! MASTERX_ACTIVE_INDEX_BYPASS_EN - 1- use masterx_active-index from register hw_ip_active_index_ctrl; 0-use active index from HW latched version */
#define APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_ACTIVE_INDEX_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_ACTIVE_INDEX_BYPASS_EN_SHIFT)) & APU_HW_IP_DYNAMIC_CLK_SWITCH_CTRL_MASTERX_ACTIVE_INDEX_BYPASS_EN_MASK)
/*! @} */

/*! @name IHB_CTRL - IHB Control */
/*! @{ */

#define APU_IHB_CTRL_IHB_VOL_REQ_AS_P_REQ_MASK   (0x1U)
#define APU_IHB_CTRL_IHB_VOL_REQ_AS_P_REQ_SHIFT  (0U)
/*! IHB_VOL_REQ_AS_P_REQ - IHB Vol Request as P Request */
#define APU_IHB_CTRL_IHB_VOL_REQ_AS_P_REQ(x)     (((uint32_t)(((uint32_t)(x)) << APU_IHB_CTRL_IHB_VOL_REQ_AS_P_REQ_SHIFT)) & APU_IHB_CTRL_IHB_VOL_REQ_AS_P_REQ_MASK)

#define APU_IHB_CTRL_IHB_VOL_REQ_AS_XP_REQ_MASK  (0x2U)
#define APU_IHB_CTRL_IHB_VOL_REQ_AS_XP_REQ_SHIFT (1U)
/*! IHB_VOL_REQ_AS_XP_REQ - IHB Vol Request as XP Request */
#define APU_IHB_CTRL_IHB_VOL_REQ_AS_XP_REQ(x)    (((uint32_t)(((uint32_t)(x)) << APU_IHB_CTRL_IHB_VOL_REQ_AS_XP_REQ_SHIFT)) & APU_IHB_CTRL_IHB_VOL_REQ_AS_XP_REQ_MASK)

#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XP_REQ_MASK  (0x4U)
#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XP_REQ_SHIFT (2U)
/*! IHB_CLK_REQ_AS_XP_REQ - IHB Clock Request as XP Request */
#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XP_REQ(x)    (((uint32_t)(((uint32_t)(x)) << APU_IHB_CTRL_IHB_CLK_REQ_AS_XP_REQ_SHIFT)) & APU_IHB_CTRL_IHB_CLK_REQ_AS_XP_REQ_MASK)

#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XPP_REQ_MASK (0x8U)
#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XPP_REQ_SHIFT (3U)
/*! IHB_CLK_REQ_AS_XPP_REQ - IHB Clock Request as XPP Request */
#define APU_IHB_CTRL_IHB_CLK_REQ_AS_XPP_REQ(x)   (((uint32_t)(((uint32_t)(x)) << APU_IHB_CTRL_IHB_CLK_REQ_AS_XPP_REQ_SHIFT)) & APU_IHB_CTRL_IHB_CLK_REQ_AS_XPP_REQ_MASK)

#define APU_IHB_CTRL_IHB_VOL_VAL_MASK            (0x7F0000U)
#define APU_IHB_CTRL_IHB_VOL_VAL_SHIFT           (16U)
/*! IHB_VOL_VAL - Voltage value needed for Bluetooth function */
#define APU_IHB_CTRL_IHB_VOL_VAL(x)              (((uint32_t)(((uint32_t)(x)) << APU_IHB_CTRL_IHB_VOL_VAL_SHIFT)) & APU_IHB_CTRL_IHB_VOL_VAL_MASK)
/*! @} */

/*! @name IHB_WKUP_MASK - IHB Wakeup Mask */
/*! @{ */

#define APU_IHB_WKUP_MASK_IHB_HI_VOL_REQ_MASK_MASK (0x1U)
#define APU_IHB_WKUP_MASK_IHB_HI_VOL_REQ_MASK_SHIFT (0U)
/*! IHB_HI_VOL_REQ_MASK - IHB High Vol Request Mask */
#define APU_IHB_WKUP_MASK_IHB_HI_VOL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_IHB_WKUP_MASK_IHB_HI_VOL_REQ_MASK_SHIFT)) & APU_IHB_WKUP_MASK_IHB_HI_VOL_REQ_MASK_MASK)

#define APU_IHB_WKUP_MASK_IHB_LO_VOL_REQ_MASK_MASK (0x2U)
#define APU_IHB_WKUP_MASK_IHB_LO_VOL_REQ_MASK_SHIFT (1U)
/*! IHB_LO_VOL_REQ_MASK - IHB Low Vol Request Mask */
#define APU_IHB_WKUP_MASK_IHB_LO_VOL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_IHB_WKUP_MASK_IHB_LO_VOL_REQ_MASK_SHIFT)) & APU_IHB_WKUP_MASK_IHB_LO_VOL_REQ_MASK_MASK)

#define APU_IHB_WKUP_MASK_IHB_CLK_REQ_MASK_MASK  (0x4U)
#define APU_IHB_WKUP_MASK_IHB_CLK_REQ_MASK_SHIFT (2U)
/*! IHB_CLK_REQ_MASK - IHB Clock Request Mask */
#define APU_IHB_WKUP_MASK_IHB_CLK_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_IHB_WKUP_MASK_IHB_CLK_REQ_MASK_SHIFT)) & APU_IHB_WKUP_MASK_IHB_CLK_REQ_MASK_MASK)

#define APU_IHB_WKUP_MASK_IHB_WL_WKUP_REQ_MASK_MASK (0x8U)
#define APU_IHB_WKUP_MASK_IHB_WL_WKUP_REQ_MASK_SHIFT (3U)
/*! IHB_WL_WKUP_REQ_MASK - IHB WLAN Wakeup Request Mask */
#define APU_IHB_WKUP_MASK_IHB_WL_WKUP_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_IHB_WKUP_MASK_IHB_WL_WKUP_REQ_MASK_SHIFT)) & APU_IHB_WKUP_MASK_IHB_WL_WKUP_REQ_MASK_MASK)

#define APU_IHB_WKUP_MASK_CPU1_IHB_PMU_WKUP_MASK_MASK (0x10U)
#define APU_IHB_WKUP_MASK_CPU1_IHB_PMU_WKUP_MASK_SHIFT (4U)
/*! CPU1_IHB_PMU_WKUP_MASK - CPU1 IHB PMU Wakeup Mask */
#define APU_IHB_WKUP_MASK_CPU1_IHB_PMU_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_IHB_WKUP_MASK_CPU1_IHB_PMU_WKUP_MASK_SHIFT)) & APU_IHB_WKUP_MASK_CPU1_IHB_PMU_WKUP_MASK_MASK)
/*! @} */

/*! @name CPU2_IHB_WKUP_MASK - CPU2 IHB Wakeup Mask */
/*! @{ */

#define APU_CPU2_IHB_WKUP_MASK_CPU2_IHB_PMU_WKUP_MASK_MASK (0x1U)
#define APU_CPU2_IHB_WKUP_MASK_CPU2_IHB_PMU_WKUP_MASK_SHIFT (0U)
/*! CPU2_IHB_PMU_WKUP_MASK - CPU2 IHB PMU Wakeup Mask */
#define APU_CPU2_IHB_WKUP_MASK_CPU2_IHB_PMU_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_IHB_WKUP_MASK_CPU2_IHB_PMU_WKUP_MASK_SHIFT)) & APU_CPU2_IHB_WKUP_MASK_CPU2_IHB_PMU_WKUP_MASK_MASK)
/*! @} */

/*! @name IHB_STATUS - IHB Status */
/*! @{ */

#define APU_IHB_STATUS_IHB_STATUS_MASK           (0xFFFFFFFFU)
#define APU_IHB_STATUS_IHB_STATUS_SHIFT          (0U)
/*! IHB_STATUS - IHB Status */
#define APU_IHB_STATUS_IHB_STATUS(x)             (((uint32_t)(((uint32_t)(x)) << APU_IHB_STATUS_IHB_STATUS_SHIFT)) & APU_IHB_STATUS_IHB_STATUS_MASK)
/*! @} */

/*! @name USB_CTRL - USB Control */
/*! @{ */

#define APU_USB_CTRL_USB_PWR_DWN_MASK_MASK       (0x1U)
#define APU_USB_CTRL_USB_PWR_DWN_MASK_SHIFT      (0U)
/*! USB_PWR_DWN_MASK - USB Power Down Mask */
#define APU_USB_CTRL_USB_PWR_DWN_MASK(x)         (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_USB_PWR_DWN_MASK_SHIFT)) & APU_USB_CTRL_USB_PWR_DWN_MASK_MASK)

#define APU_USB_CTRL_USE_CLK_SEL_ACK_AS_USB_PWR_DWN_MASK (0x10U)
#define APU_USB_CTRL_USE_CLK_SEL_ACK_AS_USB_PWR_DWN_SHIFT (4U)
/*! USE_CLK_SEL_ACK_AS_USB_PWR_DWN - Use Clock Select Ack as USB Power Down */
#define APU_USB_CTRL_USE_CLK_SEL_ACK_AS_USB_PWR_DWN(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_USE_CLK_SEL_ACK_AS_USB_PWR_DWN_SHIFT)) & APU_USB_CTRL_USE_CLK_SEL_ACK_AS_USB_PWR_DWN_MASK)

#define APU_USB_CTRL_IDLE2ISO_DLY_EN_MASK        (0x20U)
#define APU_USB_CTRL_IDLE2ISO_DLY_EN_SHIFT       (5U)
/*! IDLE2ISO_DLY_EN - USB FSM state counter enable. When set to 1, use usb_pwr_ctrl_dly and usb_pwr_ctrl_dly2. When set to 0, no delay */
#define APU_USB_CTRL_IDLE2ISO_DLY_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_IDLE2ISO_DLY_EN_SHIFT)) & APU_USB_CTRL_IDLE2ISO_DLY_EN_MASK)

#define APU_USB_CTRL_USB_USE_NOM_PWR_BYP_MASK    (0x200U)
#define APU_USB_CTRL_USB_USE_NOM_PWR_BYP_SHIFT   (9U)
/*! USB_USE_NOM_PWR_BYP - USB Use Nominal Power Bypass */
#define APU_USB_CTRL_USB_USE_NOM_PWR_BYP(x)      (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_USB_USE_NOM_PWR_BYP_SHIFT)) & APU_USB_CTRL_USB_USE_NOM_PWR_BYP_MASK)

#define APU_USB_CTRL_FW_FORCE_USB_PWRUP_MASK     (0x1000000U)
#define APU_USB_CTRL_FW_FORCE_USB_PWRUP_SHIFT    (24U)
/*! FW_FORCE_USB_PWRUP - FW Force USB Powerup */
#define APU_USB_CTRL_FW_FORCE_USB_PWRUP(x)       (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_FW_FORCE_USB_PWRUP_SHIFT)) & APU_USB_CTRL_FW_FORCE_USB_PWRUP_MASK)

#define APU_USB_CTRL_USB_USE_UNSYNC_VOL_LVL_MASK (0x2000000U)
#define APU_USB_CTRL_USB_USE_UNSYNC_VOL_LVL_SHIFT (25U)
/*! USB_USE_UNSYNC_VOL_LVL - USB Use Unsync Vol Level */
#define APU_USB_CTRL_USB_USE_UNSYNC_VOL_LVL(x)   (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_USB_USE_UNSYNC_VOL_LVL_SHIFT)) & APU_USB_CTRL_USB_USE_UNSYNC_VOL_LVL_MASK)

#define APU_USB_CTRL_USB_CLK_SEL_ACK_EXT_EN_MASK (0x80000000U)
#define APU_USB_CTRL_USB_CLK_SEL_ACK_EXT_EN_SHIFT (31U)
/*! USB_CLK_SEL_ACK_EXT_EN - USB Clock Select Ack Ext Enable */
#define APU_USB_CTRL_USB_CLK_SEL_ACK_EXT_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_USB_CTRL_USB_CLK_SEL_ACK_EXT_EN_SHIFT)) & APU_USB_CTRL_USB_CLK_SEL_ACK_EXT_EN_MASK)
/*! @} */

/*! @name USB_WKUP_MASK - USB Wakeup Mask */
/*! @{ */

#define APU_USB_WKUP_MASK_USB_P_REQ_MASK_MASK    (0x1U)
#define APU_USB_WKUP_MASK_USB_P_REQ_MASK_SHIFT   (0U)
/*! USB_P_REQ_MASK - USB P Request Mask */
#define APU_USB_WKUP_MASK_USB_P_REQ_MASK(x)      (((uint32_t)(((uint32_t)(x)) << APU_USB_WKUP_MASK_USB_P_REQ_MASK_SHIFT)) & APU_USB_WKUP_MASK_USB_P_REQ_MASK_MASK)

#define APU_USB_WKUP_MASK_USB_AXI_CLK_REQ_MASK_MASK (0x2U)
#define APU_USB_WKUP_MASK_USB_AXI_CLK_REQ_MASK_SHIFT (1U)
/*! USB_AXI_CLK_REQ_MASK - USB AXI Clock Request Mask */
#define APU_USB_WKUP_MASK_USB_AXI_CLK_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_WKUP_MASK_USB_AXI_CLK_REQ_MASK_SHIFT)) & APU_USB_WKUP_MASK_USB_AXI_CLK_REQ_MASK_MASK)

#define APU_USB_WKUP_MASK_USB_CLK_SEL_REQ_MASK_MASK (0x4U)
#define APU_USB_WKUP_MASK_USB_CLK_SEL_REQ_MASK_SHIFT (2U)
/*! USB_CLK_SEL_REQ_MASK - USB Clock Select Request Mask */
#define APU_USB_WKUP_MASK_USB_CLK_SEL_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_WKUP_MASK_USB_CLK_SEL_REQ_MASK_SHIFT)) & APU_USB_WKUP_MASK_USB_CLK_SEL_REQ_MASK_MASK)
/*! @} */

/*! @name USB_STATUS - USB Status */
/*! @{ */

#define APU_USB_STATUS_USB_P_REQ_MASK            (0x1U)
#define APU_USB_STATUS_USB_P_REQ_SHIFT           (0U)
/*! USB_P_REQ - USB P Request */
#define APU_USB_STATUS_USB_P_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_P_REQ_SHIFT)) & APU_USB_STATUS_USB_P_REQ_MASK)

#define APU_USB_STATUS_APU_USB_P_ACK_MASK        (0x2U)
#define APU_USB_STATUS_APU_USB_P_ACK_SHIFT       (1U)
/*! APU_USB_P_ACK - APU USB P Ack */
#define APU_USB_STATUS_APU_USB_P_ACK(x)          (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_P_ACK_SHIFT)) & APU_USB_STATUS_APU_USB_P_ACK_MASK)

#define APU_USB_STATUS_USB_CLK_SEL_REQ_MASK      (0x4U)
#define APU_USB_STATUS_USB_CLK_SEL_REQ_SHIFT     (2U)
/*! USB_CLK_SEL_REQ - USB Clock Select Request */
#define APU_USB_STATUS_USB_CLK_SEL_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_CLK_SEL_REQ_SHIFT)) & APU_USB_STATUS_USB_CLK_SEL_REQ_MASK)

#define APU_USB_STATUS_APU_USB_CLK_SEL_ACK_MASK  (0x8U)
#define APU_USB_STATUS_APU_USB_CLK_SEL_ACK_SHIFT (3U)
/*! APU_USB_CLK_SEL_ACK - APU USB Clock Select Ack */
#define APU_USB_STATUS_APU_USB_CLK_SEL_ACK(x)    (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_CLK_SEL_ACK_SHIFT)) & APU_USB_STATUS_APU_USB_CLK_SEL_ACK_MASK)

#define APU_USB_STATUS_USB_AXI_CLK_REQ_MASK      (0x10U)
#define APU_USB_STATUS_USB_AXI_CLK_REQ_SHIFT     (4U)
/*! USB_AXI_CLK_REQ - USB AXI Clock Request */
#define APU_USB_STATUS_USB_AXI_CLK_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_AXI_CLK_REQ_SHIFT)) & APU_USB_STATUS_USB_AXI_CLK_REQ_MASK)

#define APU_USB_STATUS_APU_USB_AXI_CLK_ACK_MASK  (0x20U)
#define APU_USB_STATUS_APU_USB_AXI_CLK_ACK_SHIFT (5U)
/*! APU_USB_AXI_CLK_ACK - APU USB AXI Clock Ack */
#define APU_USB_STATUS_APU_USB_AXI_CLK_ACK(x)    (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_AXI_CLK_ACK_SHIFT)) & APU_USB_STATUS_APU_USB_AXI_CLK_ACK_MASK)

#define APU_USB_STATUS_SOC_PWR_RDY_MASK          (0x40U)
#define APU_USB_STATUS_SOC_PWR_RDY_SHIFT         (6U)
/*! SOC_PWR_RDY - SOC Power Ready */
#define APU_USB_STATUS_SOC_PWR_RDY(x)            (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_SOC_PWR_RDY_SHIFT)) & APU_USB_STATUS_SOC_PWR_RDY_MASK)

#define APU_USB_STATUS_APU_USB_SWITCH_PD_MASK    (0x80U)
#define APU_USB_STATUS_APU_USB_SWITCH_PD_SHIFT   (7U)
/*! APU_USB_SWITCH_PD - APU USB Switch Power Down */
#define APU_USB_STATUS_APU_USB_SWITCH_PD(x)      (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_SWITCH_PD_SHIFT)) & APU_USB_STATUS_APU_USB_SWITCH_PD_MASK)

#define APU_USB_STATUS_USB_CLK_SEL_ACK_EXT_EN_MASK (0x100U)
#define APU_USB_STATUS_USB_CLK_SEL_ACK_EXT_EN_SHIFT (8U)
/*! USB_CLK_SEL_ACK_EXT_EN - extension enable for the usb_clk_sel_ack */
#define APU_USB_STATUS_USB_CLK_SEL_ACK_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_CLK_SEL_ACK_EXT_EN_SHIFT)) & APU_USB_STATUS_USB_CLK_SEL_ACK_EXT_EN_MASK)

#define APU_USB_STATUS_APU_USB_AXI_CLK_EN_MASK   (0x200U)
#define APU_USB_STATUS_APU_USB_AXI_CLK_EN_SHIFT  (9U)
/*! APU_USB_AXI_CLK_EN - APU USB AXI Clock Enable */
#define APU_USB_STATUS_APU_USB_AXI_CLK_EN(x)     (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_AXI_CLK_EN_SHIFT)) & APU_USB_STATUS_APU_USB_AXI_CLK_EN_MASK)

#define APU_USB_STATUS_XOSC_STABLE_REFCK_MASK    (0x400U)
#define APU_USB_STATUS_XOSC_STABLE_REFCK_SHIFT   (10U)
/*! XOSC_STABLE_REFCK - OSC clock stable on ref clock domain */
#define APU_USB_STATUS_XOSC_STABLE_REFCK(x)      (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_XOSC_STABLE_REFCK_SHIFT)) & APU_USB_STATUS_XOSC_STABLE_REFCK_MASK)

#define APU_USB_STATUS_APU_USB_CLK_SEL_MASK      (0x800U)
#define APU_USB_STATUS_APU_USB_CLK_SEL_SHIFT     (11U)
/*! APU_USB_CLK_SEL - APU USB Clock Select */
#define APU_USB_STATUS_APU_USB_CLK_SEL(x)        (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_CLK_SEL_SHIFT)) & APU_USB_STATUS_APU_USB_CLK_SEL_MASK)

#define APU_USB_STATUS_USB_VOL_LVL_REACHED_MASK  (0x1000U)
#define APU_USB_STATUS_USB_VOL_LVL_REACHED_SHIFT (12U)
/*! USB_VOL_LVL_REACHED - USB Vol Level Reached */
#define APU_USB_STATUS_USB_VOL_LVL_REACHED(x)    (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_VOL_LVL_REACHED_SHIFT)) & APU_USB_STATUS_USB_VOL_LVL_REACHED_MASK)

#define APU_USB_STATUS_DVFS_NOM_VOL_LVL_REACHED_MASK (0x2000U)
#define APU_USB_STATUS_DVFS_NOM_VOL_LVL_REACHED_SHIFT (13U)
/*! DVFS_NOM_VOL_LVL_REACHED - DVFS Nominal Vol Level Reached */
#define APU_USB_STATUS_DVFS_NOM_VOL_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_DVFS_NOM_VOL_LVL_REACHED_SHIFT)) & APU_USB_STATUS_DVFS_NOM_VOL_LVL_REACHED_MASK)

#define APU_USB_STATUS_DVFS_USB_VOL_LVL_REACHED_MASK (0x4000U)
#define APU_USB_STATUS_DVFS_USB_VOL_LVL_REACHED_SHIFT (14U)
/*! DVFS_USB_VOL_LVL_REACHED - DVFS USB Vol Level Reached */
#define APU_USB_STATUS_DVFS_USB_VOL_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_DVFS_USB_VOL_LVL_REACHED_SHIFT)) & APU_USB_STATUS_DVFS_USB_VOL_LVL_REACHED_MASK)

#define APU_USB_STATUS_USB_PWR_DWN_MASK          (0x8000U)
#define APU_USB_STATUS_USB_PWR_DWN_SHIFT         (15U)
/*! USB_PWR_DWN - USB Power Down */
#define APU_USB_STATUS_USB_PWR_DWN(x)            (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_USB_PWR_DWN_SHIFT)) & APU_USB_STATUS_USB_PWR_DWN_MASK)

#define APU_USB_STATUS_APU_USB_ST_MASK           (0xF0000U)
#define APU_USB_STATUS_APU_USB_ST_SHIFT          (16U)
/*! APU_USB_ST - APU USB St */
#define APU_USB_STATUS_APU_USB_ST(x)             (((uint32_t)(((uint32_t)(x)) << APU_USB_STATUS_APU_USB_ST_SHIFT)) & APU_USB_STATUS_APU_USB_ST_MASK)
/*! @} */

/*! @name CPU1_DVFS_CTRL - CPU1 DVFS Control */
/*! @{ */

#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_INDEX_MASK (0xFU)
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_INDEX_SHIFT (0U)
/*! CPU1_ACTIVE_INDEX - CPU1 Active Index */
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_INDEX(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_INDEX_MASK (0xF0U)
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_INDEX_SHIFT (4U)
/*! CPU1_INACTIVE_INDEX - CPU1 Inactive Index */
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_SYS_INDEX_MASK (0xF00U)
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_SYS_INDEX_SHIFT (8U)
/*! CPU1_ACTIVE_SYS_INDEX - CPU1 Active System Index */
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_SYS_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_SYS_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_SYS_INDEX_MASK (0xF000U)
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_SYS_INDEX_SHIFT (12U)
/*! CPU1_INACTIVE_SYS_INDEX - CPU1 Inactive System Index */
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_SYS_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_SYS_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_AHB1_INDEX_MASK (0xF0000U)
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_AHB1_INDEX_SHIFT (16U)
/*! CPU1_ACTIVE_AHB1_INDEX - CPU1 Active AHB1 Index */
#define APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_ACTIVE_AHB1_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_AHB1_INDEX_MASK (0xF00000U)
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_AHB1_INDEX_SHIFT (20U)
/*! CPU1_INACTIVE_AHB1_INDEX - CPU1 Inactive AHB1 Index */
#define APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU1_DVFS_CTRL_CPU1_INACTIVE_AHB1_INDEX_MASK)

#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_WKUP_DEP_MASK (0x20000000U)
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_WKUP_DEP_SHIFT (29U)
/*! PCIE_VOL_HOST_WKUP_DEP - PCIe Vol Host Wakeup Dep */
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_WKUP_DEP(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_WKUP_DEP_SHIFT)) & APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_WKUP_DEP_MASK)

#define APU_CPU1_DVFS_CTRL_PCIE_VOL_FLR_DEP_MASK (0x40000000U)
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_FLR_DEP_SHIFT (30U)
/*! PCIE_VOL_FLR_DEP - PCIe Vol Flr Dep */
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_FLR_DEP(x)   (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_PCIE_VOL_FLR_DEP_SHIFT)) & APU_CPU1_DVFS_CTRL_PCIE_VOL_FLR_DEP_MASK)

#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_SLP_DEP_MASK (0x80000000U)
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_SLP_DEP_SHIFT (31U)
/*! PCIE_VOL_HOST_SLP_DEP - PCIe Vol Host Sleep Dep */
#define APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_SLP_DEP(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_SLP_DEP_SHIFT)) & APU_CPU1_DVFS_CTRL_PCIE_VOL_HOST_SLP_DEP_MASK)
/*! @} */

/*! @name CPU1_FREQ_REG1 - CPU1 Frequency 1 */
/*! @{ */

#define APU_CPU1_FREQ_REG1_CPU1_FREQ_REG1_MASK   (0xFFFFFFFFU)
#define APU_CPU1_FREQ_REG1_CPU1_FREQ_REG1_SHIFT  (0U)
/*! CPU1_FREQ_REG1 - CPU1 Frequency 1 */
#define APU_CPU1_FREQ_REG1_CPU1_FREQ_REG1(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU1_FREQ_REG1_CPU1_FREQ_REG1_SHIFT)) & APU_CPU1_FREQ_REG1_CPU1_FREQ_REG1_MASK)
/*! @} */

/*! @name CPU1_FREQ_REG2 - CPU1 Frequency 2 */
/*! @{ */

#define APU_CPU1_FREQ_REG2_CPU1_FREQ_REG2_MASK   (0xFFFFFFFFU)
#define APU_CPU1_FREQ_REG2_CPU1_FREQ_REG2_SHIFT  (0U)
/*! CPU1_FREQ_REG2 - CPU1 Frequency 2 */
#define APU_CPU1_FREQ_REG2_CPU1_FREQ_REG2(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU1_FREQ_REG2_CPU1_FREQ_REG2_SHIFT)) & APU_CPU1_FREQ_REG2_CPU1_FREQ_REG2_MASK)
/*! @} */

/*! @name CPU1_PLL_EN_REG - CPU1 PLL Enable */
/*! @{ */

#define APU_CPU1_PLL_EN_REG_CPU1_PLL_EN_REG_MASK (0xFFFFFFFFU)
#define APU_CPU1_PLL_EN_REG_CPU1_PLL_EN_REG_SHIFT (0U)
/*! CPU1_PLL_EN_REG - CPU1 PLL Enable */
#define APU_CPU1_PLL_EN_REG_CPU1_PLL_EN_REG(x)   (((uint32_t)(((uint32_t)(x)) << APU_CPU1_PLL_EN_REG_CPU1_PLL_EN_REG_SHIFT)) & APU_CPU1_PLL_EN_REG_CPU1_PLL_EN_REG_MASK)
/*! @} */

/*! @name CPU1_VOL_REG1 - CPU1 Voltage 1 */
/*! @{ */

#define APU_CPU1_VOL_REG1_CPU1_VOL_REG1_MASK     (0xFFFFFFFFU)
#define APU_CPU1_VOL_REG1_CPU1_VOL_REG1_SHIFT    (0U)
/*! CPU1_VOL_REG1 - CPU1 Voltage 1 */
#define APU_CPU1_VOL_REG1_CPU1_VOL_REG1(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU1_VOL_REG1_CPU1_VOL_REG1_SHIFT)) & APU_CPU1_VOL_REG1_CPU1_VOL_REG1_MASK)
/*! @} */

/*! @name CPU1_VOL_REG2 - CPU1 Voltage 2 */
/*! @{ */

#define APU_CPU1_VOL_REG2_CPU1_VOL_REG2_MASK     (0xFFFFFFFFU)
#define APU_CPU1_VOL_REG2_CPU1_VOL_REG2_SHIFT    (0U)
/*! CPU1_VOL_REG2 - CPU1 Voltage 2 */
#define APU_CPU1_VOL_REG2_CPU1_VOL_REG2(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU1_VOL_REG2_CPU1_VOL_REG2_SHIFT)) & APU_CPU1_VOL_REG2_CPU1_VOL_REG2_MASK)
/*! @} */

/*! @name CPU1_VOL_REG3 - CPU1 Voltage 3 */
/*! @{ */

#define APU_CPU1_VOL_REG3_CPU1_VOL_REG3_MASK     (0xFFFFFFFFU)
#define APU_CPU1_VOL_REG3_CPU1_VOL_REG3_SHIFT    (0U)
/*! CPU1_VOL_REG3 - CPU1 Voltage 3 */
#define APU_CPU1_VOL_REG3_CPU1_VOL_REG3(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU1_VOL_REG3_CPU1_VOL_REG3_SHIFT)) & APU_CPU1_VOL_REG3_CPU1_VOL_REG3_MASK)
/*! @} */

/*! @name CPU1_VOL_REG4 - CPU1 Voltage 4 */
/*! @{ */

#define APU_CPU1_VOL_REG4_CPU1_VOL_REG4_MASK     (0xFFFFFFFFU)
#define APU_CPU1_VOL_REG4_CPU1_VOL_REG4_SHIFT    (0U)
/*! CPU1_VOL_REG4 - CPU1 Voltage 4 */
#define APU_CPU1_VOL_REG4_CPU1_VOL_REG4(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU1_VOL_REG4_CPU1_VOL_REG4_SHIFT)) & APU_CPU1_VOL_REG4_CPU1_VOL_REG4_MASK)
/*! @} */

/*! @name CPU2_DVFS_CTRL - CPU2 DVFS Control */
/*! @{ */

#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_INDEX_MASK (0xFU)
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_INDEX_SHIFT (0U)
/*! CPU2_ACTIVE_INDEX - CPU2 Active Index */
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_INDEX(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_INDEX_MASK (0xF0U)
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_INDEX_SHIFT (4U)
/*! CPU2_INACTIVE_INDEX - CPU2 Inactive Index */
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_SYS_INDEX_MASK (0xF00U)
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_SYS_INDEX_SHIFT (8U)
/*! CPU2_ACTIVE_SYS_INDEX - CPU2 Active System Index */
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_SYS_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_SYS_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_SYS_INDEX_MASK (0xF000U)
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_SYS_INDEX_SHIFT (12U)
/*! CPU2_INACTIVE_SYS_INDEX - CPU2 Inactive System Index */
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_SYS_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_SYS_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_AHB1_INDEX_MASK (0xF0000U)
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_AHB1_INDEX_SHIFT (16U)
/*! CPU2_ACTIVE_AHB1_INDEX - CPU2 Active AHB1 Index */
#define APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_ACTIVE_AHB1_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_AHB1_INDEX_MASK (0xF00000U)
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_AHB1_INDEX_SHIFT (20U)
/*! CPU2_INACTIVE_AHB1_INDEX - CPU2 Inactive AHB1 Index */
#define APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_INACTIVE_AHB1_INDEX_MASK)

#define APU_CPU2_DVFS_CTRL_CPU2_AHB1_INDEX_SEL_METHOD_MASK (0x1000000U)
#define APU_CPU2_DVFS_CTRL_CPU2_AHB1_INDEX_SEL_METHOD_SHIFT (24U)
/*! CPU2_AHB1_INDEX_SEL_METHOD - CPU2 AHB1 index select method */
#define APU_CPU2_DVFS_CTRL_CPU2_AHB1_INDEX_SEL_METHOD(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_CPU2_AHB1_INDEX_SEL_METHOD_SHIFT)) & APU_CPU2_DVFS_CTRL_CPU2_AHB1_INDEX_SEL_METHOD_MASK)

#define APU_CPU2_DVFS_CTRL_UART_VOL_VAL_MASK     (0xFE000000U)
#define APU_CPU2_DVFS_CTRL_UART_VOL_VAL_SHIFT    (25U)
/*! UART_VOL_VAL - UART Vol Value */
#define APU_CPU2_DVFS_CTRL_UART_VOL_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_DVFS_CTRL_UART_VOL_VAL_SHIFT)) & APU_CPU2_DVFS_CTRL_UART_VOL_VAL_MASK)
/*! @} */

/*! @name CPU2_FREQ_REG1 - CPU2 Frequency 1 */
/*! @{ */

#define APU_CPU2_FREQ_REG1_CPU2_FREQ_REG1_MASK   (0xFFFFFFFFU)
#define APU_CPU2_FREQ_REG1_CPU2_FREQ_REG1_SHIFT  (0U)
/*! CPU2_FREQ_REG1 - CPU2 Frequency 1 */
#define APU_CPU2_FREQ_REG1_CPU2_FREQ_REG1(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FREQ_REG1_CPU2_FREQ_REG1_SHIFT)) & APU_CPU2_FREQ_REG1_CPU2_FREQ_REG1_MASK)
/*! @} */

/*! @name CPU2_FREQ_REG2 - CPU2 Frequency 2 */
/*! @{ */

#define APU_CPU2_FREQ_REG2_CPU2_FREQ_REG2_MASK   (0xFFFFFFFFU)
#define APU_CPU2_FREQ_REG2_CPU2_FREQ_REG2_SHIFT  (0U)
/*! CPU2_FREQ_REG2 - CPU2Frequency 2 */
#define APU_CPU2_FREQ_REG2_CPU2_FREQ_REG2(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU2_FREQ_REG2_CPU2_FREQ_REG2_SHIFT)) & APU_CPU2_FREQ_REG2_CPU2_FREQ_REG2_MASK)
/*! @} */

/*! @name CPU2_PLL_EN_REG - CPU2 PLL Enable */
/*! @{ */

#define APU_CPU2_PLL_EN_REG_CPU2_PLL_EN_REG_MASK (0xFFFFFFFFU)
#define APU_CPU2_PLL_EN_REG_CPU2_PLL_EN_REG_SHIFT (0U)
/*! CPU2_PLL_EN_REG - CPU2 PLL Enable */
#define APU_CPU2_PLL_EN_REG_CPU2_PLL_EN_REG(x)   (((uint32_t)(((uint32_t)(x)) << APU_CPU2_PLL_EN_REG_CPU2_PLL_EN_REG_SHIFT)) & APU_CPU2_PLL_EN_REG_CPU2_PLL_EN_REG_MASK)
/*! @} */

/*! @name CPU2_VOL_REG1 - CPU2 Voltage 1 */
/*! @{ */

#define APU_CPU2_VOL_REG1_CPU2_VOL_REG1_MASK     (0xFFFFFFFFU)
#define APU_CPU2_VOL_REG1_CPU2_VOL_REG1_SHIFT    (0U)
/*! CPU2_VOL_REG1 - CPU2 Voltage 1 */
#define APU_CPU2_VOL_REG1_CPU2_VOL_REG1(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_VOL_REG1_CPU2_VOL_REG1_SHIFT)) & APU_CPU2_VOL_REG1_CPU2_VOL_REG1_MASK)
/*! @} */

/*! @name CPU2_VOL_REG2 - CPU2 Voltage 2 */
/*! @{ */

#define APU_CPU2_VOL_REG2_CPU2_VOL_REG2_MASK     (0xFFFFFFFFU)
#define APU_CPU2_VOL_REG2_CPU2_VOL_REG2_SHIFT    (0U)
/*! CPU2_VOL_REG2 - CPU2 Voltage 2 */
#define APU_CPU2_VOL_REG2_CPU2_VOL_REG2(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_VOL_REG2_CPU2_VOL_REG2_SHIFT)) & APU_CPU2_VOL_REG2_CPU2_VOL_REG2_MASK)
/*! @} */

/*! @name CPU2_VOL_REG3 - CPU2 Voltage 3 */
/*! @{ */

#define APU_CPU2_VOL_REG3_CPU2_VOL_REG3_MASK     (0xFFFFFFFFU)
#define APU_CPU2_VOL_REG3_CPU2_VOL_REG3_SHIFT    (0U)
/*! CPU2_VOL_REG3 - CPU2 Voltage 3 */
#define APU_CPU2_VOL_REG3_CPU2_VOL_REG3(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_VOL_REG3_CPU2_VOL_REG3_SHIFT)) & APU_CPU2_VOL_REG3_CPU2_VOL_REG3_MASK)
/*! @} */

/*! @name CPU2_VOL_REG4 - CPU2 Voltage 4 */
/*! @{ */

#define APU_CPU2_VOL_REG4_CPU2_VOL_REG4_MASK     (0xFFFFFFFFU)
#define APU_CPU2_VOL_REG4_CPU2_VOL_REG4_SHIFT    (0U)
/*! CPU2_VOL_REG4 - CPU2 Voltage 4 */
#define APU_CPU2_VOL_REG4_CPU2_VOL_REG4(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU2_VOL_REG4_CPU2_VOL_REG4_SHIFT)) & APU_CPU2_VOL_REG4_CPU2_VOL_REG4_MASK)
/*! @} */

/*! @name SYS_FREQ_REG1 - System Frequency 1 */
/*! @{ */

#define APU_SYS_FREQ_REG1_SYS_FREQ_REG1_MASK     (0xFFFFFFFFU)
#define APU_SYS_FREQ_REG1_SYS_FREQ_REG1_SHIFT    (0U)
/*! SYS_FREQ_REG1 - System Frequency 1 */
#define APU_SYS_FREQ_REG1_SYS_FREQ_REG1(x)       (((uint32_t)(((uint32_t)(x)) << APU_SYS_FREQ_REG1_SYS_FREQ_REG1_SHIFT)) & APU_SYS_FREQ_REG1_SYS_FREQ_REG1_MASK)
/*! @} */

/*! @name SYS_FREQ_REG2 - System Frequency 2 */
/*! @{ */

#define APU_SYS_FREQ_REG2_SYS_FREQ_REG2_MASK     (0xFFFFFFFFU)
#define APU_SYS_FREQ_REG2_SYS_FREQ_REG2_SHIFT    (0U)
/*! SYS_FREQ_REG2 - System Frequency 2 */
#define APU_SYS_FREQ_REG2_SYS_FREQ_REG2(x)       (((uint32_t)(((uint32_t)(x)) << APU_SYS_FREQ_REG2_SYS_FREQ_REG2_SHIFT)) & APU_SYS_FREQ_REG2_SYS_FREQ_REG2_MASK)
/*! @} */

/*! @name SYS_PLL_EN_REG - System PLL Enable */
/*! @{ */

#define APU_SYS_PLL_EN_REG_SYS_PLL_EN_REG_MASK   (0xFFFFFFFFU)
#define APU_SYS_PLL_EN_REG_SYS_PLL_EN_REG_SHIFT  (0U)
/*! SYS_PLL_EN_REG - System PLL Enable */
#define APU_SYS_PLL_EN_REG_SYS_PLL_EN_REG(x)     (((uint32_t)(((uint32_t)(x)) << APU_SYS_PLL_EN_REG_SYS_PLL_EN_REG_SHIFT)) & APU_SYS_PLL_EN_REG_SYS_PLL_EN_REG_MASK)
/*! @} */

/*! @name SYS_VOL_REG1 - System Voltage 1 */
/*! @{ */

#define APU_SYS_VOL_REG1_SYS_VOL_REG1_MASK       (0xFFFFFFFFU)
#define APU_SYS_VOL_REG1_SYS_VOL_REG1_SHIFT      (0U)
/*! SYS_VOL_REG1 - System Voltage 1 */
#define APU_SYS_VOL_REG1_SYS_VOL_REG1(x)         (((uint32_t)(((uint32_t)(x)) << APU_SYS_VOL_REG1_SYS_VOL_REG1_SHIFT)) & APU_SYS_VOL_REG1_SYS_VOL_REG1_MASK)
/*! @} */

/*! @name SYS_VOL_REG2 - System Voltage 2 */
/*! @{ */

#define APU_SYS_VOL_REG2_SYS_VOL_REG2_MASK       (0xFFFFFFFFU)
#define APU_SYS_VOL_REG2_SYS_VOL_REG2_SHIFT      (0U)
/*! SYS_VOL_REG2 - System Voltage 2 */
#define APU_SYS_VOL_REG2_SYS_VOL_REG2(x)         (((uint32_t)(((uint32_t)(x)) << APU_SYS_VOL_REG2_SYS_VOL_REG2_SHIFT)) & APU_SYS_VOL_REG2_SYS_VOL_REG2_MASK)
/*! @} */

/*! @name SYS_VOL_REG3 - System Voltage 3 */
/*! @{ */

#define APU_SYS_VOL_REG3_SYS_VOL_REG3_MASK       (0xFFFFFFFFU)
#define APU_SYS_VOL_REG3_SYS_VOL_REG3_SHIFT      (0U)
/*! SYS_VOL_REG3 - System Voltage 3 */
#define APU_SYS_VOL_REG3_SYS_VOL_REG3(x)         (((uint32_t)(((uint32_t)(x)) << APU_SYS_VOL_REG3_SYS_VOL_REG3_SHIFT)) & APU_SYS_VOL_REG3_SYS_VOL_REG3_MASK)
/*! @} */

/*! @name SYS_VOL_REG4 - System Voltage 4 */
/*! @{ */

#define APU_SYS_VOL_REG4_SYS_VOL_REG4_MASK       (0xFFFFFFFFU)
#define APU_SYS_VOL_REG4_SYS_VOL_REG4_SHIFT      (0U)
/*! SYS_VOL_REG4 - System Voltage 4 */
#define APU_SYS_VOL_REG4_SYS_VOL_REG4(x)         (((uint32_t)(((uint32_t)(x)) << APU_SYS_VOL_REG4_SYS_VOL_REG4_SHIFT)) & APU_SYS_VOL_REG4_SYS_VOL_REG4_MASK)
/*! @} */

/*! @name DVFS_CTRL - DVFS Control */
/*! @{ */

#define APU_DVFS_CTRL_DVFS_HOST_VOL_VAL_MASK     (0x7FU)
#define APU_DVFS_CTRL_DVFS_HOST_VOL_VAL_SHIFT    (0U)
/*! DVFS_HOST_VOL_VAL - DVFS Host Vol Value */
#define APU_DVFS_CTRL_DVFS_HOST_VOL_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_HOST_VOL_VAL_SHIFT)) & APU_DVFS_CTRL_DVFS_HOST_VOL_VAL_MASK)

#define APU_DVFS_CTRL_DVFS_EAS_VOL_MASK          (0x3F80U)
#define APU_DVFS_CTRL_DVFS_EAS_VOL_SHIFT         (7U)
/*! DVFS_EAS_VOL - DVFS EAS Vol */
#define APU_DVFS_CTRL_DVFS_EAS_VOL(x)            (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_EAS_VOL_SHIFT)) & APU_DVFS_CTRL_DVFS_EAS_VOL_MASK)

#define APU_DVFS_CTRL_DVFS_CLK_SWITCH_EN_MASK    (0x4000U)
#define APU_DVFS_CTRL_DVFS_CLK_SWITCH_EN_SHIFT   (14U)
/*! DVFS_CLK_SWITCH_EN - DVFS Clock Switch Enable */
#define APU_DVFS_CTRL_DVFS_CLK_SWITCH_EN(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_CLK_SWITCH_EN_SHIFT)) & APU_DVFS_CTRL_DVFS_CLK_SWITCH_EN_MASK)

#define APU_DVFS_CTRL_DVFS_DYN_CLK_EN_MASK       (0x8000U)
#define APU_DVFS_CTRL_DVFS_DYN_CLK_EN_SHIFT      (15U)
/*! DVFS_DYN_CLK_EN - DVFS Dynamic Clock Enable */
#define APU_DVFS_CTRL_DVFS_DYN_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_DYN_CLK_EN_SHIFT)) & APU_DVFS_CTRL_DVFS_DYN_CLK_EN_MASK)

#define APU_DVFS_CTRL_DVFS_MODE_MASK             (0x10000U)
#define APU_DVFS_CTRL_DVFS_MODE_SHIFT            (16U)
/*! DVFS_MODE - DVFS Mode 0: only use partial_dvfs_vol(default) */
#define APU_DVFS_CTRL_DVFS_MODE(x)               (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_MODE_SHIFT)) & APU_DVFS_CTRL_DVFS_MODE_MASK)

#define APU_DVFS_CTRL_EN_FASTER_DYN_CLK_MASK     (0x20000U)
#define APU_DVFS_CTRL_EN_FASTER_DYN_CLK_SHIFT    (17U)
/*! EN_FASTER_DYN_CLK - Enable Faster Dynamic Clock */
#define APU_DVFS_CTRL_EN_FASTER_DYN_CLK(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_EN_FASTER_DYN_CLK_SHIFT)) & APU_DVFS_CTRL_EN_FASTER_DYN_CLK_MASK)

#define APU_DVFS_CTRL_DVFS_EAS_VOL_DIS_MASK      (0x40000U)
#define APU_DVFS_CTRL_DVFS_EAS_VOL_DIS_SHIFT     (18U)
/*! DVFS_EAS_VOL_DIS - DVFS EAS Vol Disable */
#define APU_DVFS_CTRL_DVFS_EAS_VOL_DIS(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_DVFS_EAS_VOL_DIS_SHIFT)) & APU_DVFS_CTRL_DVFS_EAS_VOL_DIS_MASK)

#define APU_DVFS_CTRL_SW_LOCK_GUARD_DIS_MASK     (0x80000U)
#define APU_DVFS_CTRL_SW_LOCK_GUARD_DIS_SHIFT    (19U)
/*! SW_LOCK_GUARD_DIS - SW Lock Guard Disable */
#define APU_DVFS_CTRL_SW_LOCK_GUARD_DIS(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_SW_LOCK_GUARD_DIS_SHIFT)) & APU_DVFS_CTRL_SW_LOCK_GUARD_DIS_MASK)

#define APU_DVFS_CTRL_EN_FASTER_DVFS_MASK        (0x100000U)
#define APU_DVFS_CTRL_EN_FASTER_DVFS_SHIFT       (20U)
/*! EN_FASTER_DVFS - Enable Faster DVFS */
#define APU_DVFS_CTRL_EN_FASTER_DVFS(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_EN_FASTER_DVFS_SHIFT)) & APU_DVFS_CTRL_EN_FASTER_DVFS_MASK)

#define APU_DVFS_CTRL_UPDATE_BUCK_EN_MASK        (0x200000U)
#define APU_DVFS_CTRL_UPDATE_BUCK_EN_SHIFT       (21U)
/*! UPDATE_BUCK_EN - to enable the forcing buck value when the resolution is not aligned */
#define APU_DVFS_CTRL_UPDATE_BUCK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_UPDATE_BUCK_EN_SHIFT)) & APU_DVFS_CTRL_UPDATE_BUCK_EN_MASK)

#define APU_DVFS_CTRL_INACTIVE_ACK_EXT_EN_MASK   (0x1000000U)
#define APU_DVFS_CTRL_INACTIVE_ACK_EXT_EN_SHIFT  (24U)
/*! INACTIVE_ACK_EXT_EN - Inactive Ack Ext Enable */
#define APU_DVFS_CTRL_INACTIVE_ACK_EXT_EN(x)     (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_INACTIVE_ACK_EXT_EN_SHIFT)) & APU_DVFS_CTRL_INACTIVE_ACK_EXT_EN_MASK)

#define APU_DVFS_CTRL_SDIO_VOL_VAL_MASK          (0xFE000000U)
#define APU_DVFS_CTRL_SDIO_VOL_VAL_SHIFT         (25U)
/*! SDIO_VOL_VAL - SDIO Vol Value */
#define APU_DVFS_CTRL_SDIO_VOL_VAL(x)            (((uint32_t)(((uint32_t)(x)) << APU_DVFS_CTRL_SDIO_VOL_VAL_SHIFT)) & APU_DVFS_CTRL_SDIO_VOL_VAL_MASK)
/*! @} */

/*! @name PARTIAL_DVFS_CTRL - Partial DVFS Control */
/*! @{ */

#define APU_PARTIAL_DVFS_CTRL_PARTIAL_DVFS_VOL_MASK (0x7FU)
#define APU_PARTIAL_DVFS_CTRL_PARTIAL_DVFS_VOL_SHIFT (0U)
/*! PARTIAL_DVFS_VOL - Partial DVFS Vol */
#define APU_PARTIAL_DVFS_CTRL_PARTIAL_DVFS_VOL(x) (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_PARTIAL_DVFS_VOL_SHIFT)) & APU_PARTIAL_DVFS_CTRL_PARTIAL_DVFS_VOL_MASK)

#define APU_PARTIAL_DVFS_CTRL_STATIC_PLL_EN_MASK (0x80U)
#define APU_PARTIAL_DVFS_CTRL_STATIC_PLL_EN_SHIFT (7U)
/*! STATIC_PLL_EN - Static PLL Enable */
#define APU_PARTIAL_DVFS_CTRL_STATIC_PLL_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_STATIC_PLL_EN_SHIFT)) & APU_PARTIAL_DVFS_CTRL_STATIC_PLL_EN_MASK)

#define APU_PARTIAL_DVFS_CTRL_STATIC_DVFS_FREQ_MASK (0xF00U)
#define APU_PARTIAL_DVFS_CTRL_STATIC_DVFS_FREQ_SHIFT (8U)
/*! STATIC_DVFS_FREQ - Static DVFS Frequency */
#define APU_PARTIAL_DVFS_CTRL_STATIC_DVFS_FREQ(x) (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_STATIC_DVFS_FREQ_SHIFT)) & APU_PARTIAL_DVFS_CTRL_STATIC_DVFS_FREQ_MASK)

#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_REF_MASK (0x1F0000U)
#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_REF_SHIFT (16U)
/*! VOL_RESOLUTION_REF - Vol Resolution Ref */
#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_REF(x) (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_REF_SHIFT)) & APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_REF_MASK)

#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_SLP_MASK (0x3E00000U)
#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_SLP_SHIFT (21U)
/*! VOL_RESOLUTION_SLP - Vol Resolution Sleep */
#define APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_SLP(x) (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_SLP_SHIFT)) & APU_PARTIAL_DVFS_CTRL_VOL_RESOLUTION_SLP_MASK)

#define APU_PARTIAL_DVFS_CTRL_BYPASS_DVFS_FSM_MASK (0x80000000U)
#define APU_PARTIAL_DVFS_CTRL_BYPASS_DVFS_FSM_SHIFT (31U)
/*! BYPASS_DVFS_FSM - Bypass DVFS FSM */
#define APU_PARTIAL_DVFS_CTRL_BYPASS_DVFS_FSM(x) (((uint32_t)(((uint32_t)(x)) << APU_PARTIAL_DVFS_CTRL_BYPASS_DVFS_FSM_SHIFT)) & APU_PARTIAL_DVFS_CTRL_BYPASS_DVFS_FSM_MASK)
/*! @} */

/*! @name DVFS_TIMER - DVFS Timer */
/*! @{ */

#define APU_DVFS_TIMER_SLP_CNT_MASK              (0xFFFFU)
#define APU_DVFS_TIMER_SLP_CNT_SHIFT             (0U)
/*! SLP_CNT - Sleep Count */
#define APU_DVFS_TIMER_SLP_CNT(x)                (((uint32_t)(((uint32_t)(x)) << APU_DVFS_TIMER_SLP_CNT_SHIFT)) & APU_DVFS_TIMER_SLP_CNT_MASK)

#define APU_DVFS_TIMER_REF_CNT_MASK              (0xFFFF0000U)
#define APU_DVFS_TIMER_REF_CNT_SHIFT             (16U)
/*! REF_CNT - Reference Count */
#define APU_DVFS_TIMER_REF_CNT(x)                (((uint32_t)(((uint32_t)(x)) << APU_DVFS_TIMER_REF_CNT_SHIFT)) & APU_DVFS_TIMER_REF_CNT_MASK)
/*! @} */

/*! @name AHB1_FREQ_REG1 - AHB1 Frequency 1 */
/*! @{ */

#define APU_AHB1_FREQ_REG1_AHB1_FREQ_REG1_MASK   (0xFFFFFFFFU)
#define APU_AHB1_FREQ_REG1_AHB1_FREQ_REG1_SHIFT  (0U)
/*! AHB1_FREQ_REG1 - AHB1 Frequency 1 */
#define APU_AHB1_FREQ_REG1_AHB1_FREQ_REG1(x)     (((uint32_t)(((uint32_t)(x)) << APU_AHB1_FREQ_REG1_AHB1_FREQ_REG1_SHIFT)) & APU_AHB1_FREQ_REG1_AHB1_FREQ_REG1_MASK)
/*! @} */

/*! @name AHB1_FREQ_REG2 - AHB1 Frequency 2 */
/*! @{ */

#define APU_AHB1_FREQ_REG2_AHB1_FREQ_REG2_MASK   (0xFFFFFFFFU)
#define APU_AHB1_FREQ_REG2_AHB1_FREQ_REG2_SHIFT  (0U)
/*! AHB1_FREQ_REG2 - AHB1 Frequency 2 */
#define APU_AHB1_FREQ_REG2_AHB1_FREQ_REG2(x)     (((uint32_t)(((uint32_t)(x)) << APU_AHB1_FREQ_REG2_AHB1_FREQ_REG2_SHIFT)) & APU_AHB1_FREQ_REG2_AHB1_FREQ_REG2_MASK)
/*! @} */

/*! @name AHB1_PLL_EN_REG - AHB1 PLL Enable */
/*! @{ */

#define APU_AHB1_PLL_EN_REG_AHB1_PLL_EN_REG_MASK (0xFFFFFFFFU)
#define APU_AHB1_PLL_EN_REG_AHB1_PLL_EN_REG_SHIFT (0U)
/*! AHB1_PLL_EN_REG - AHB1 PLL Enable */
#define APU_AHB1_PLL_EN_REG_AHB1_PLL_EN_REG(x)   (((uint32_t)(((uint32_t)(x)) << APU_AHB1_PLL_EN_REG_AHB1_PLL_EN_REG_SHIFT)) & APU_AHB1_PLL_EN_REG_AHB1_PLL_EN_REG_MASK)
/*! @} */

/*! @name AHB1_VOL_REG1 - AHB1 Voltage 1 */
/*! @{ */

#define APU_AHB1_VOL_REG1_AHB1_VOL_REG1_MASK     (0xFFFFFFFFU)
#define APU_AHB1_VOL_REG1_AHB1_VOL_REG1_SHIFT    (0U)
/*! AHB1_VOL_REG1 - AHB1 Voltage 1 */
#define APU_AHB1_VOL_REG1_AHB1_VOL_REG1(x)       (((uint32_t)(((uint32_t)(x)) << APU_AHB1_VOL_REG1_AHB1_VOL_REG1_SHIFT)) & APU_AHB1_VOL_REG1_AHB1_VOL_REG1_MASK)
/*! @} */

/*! @name AHB1_VOL_REG2 - AHB1 Voltage 2 */
/*! @{ */

#define APU_AHB1_VOL_REG2_AHB1_VOL_REG2_MASK     (0xFFFFFFFFU)
#define APU_AHB1_VOL_REG2_AHB1_VOL_REG2_SHIFT    (0U)
/*! AHB1_VOL_REG2 - AHB1 Voltage 2 */
#define APU_AHB1_VOL_REG2_AHB1_VOL_REG2(x)       (((uint32_t)(((uint32_t)(x)) << APU_AHB1_VOL_REG2_AHB1_VOL_REG2_SHIFT)) & APU_AHB1_VOL_REG2_AHB1_VOL_REG2_MASK)
/*! @} */

/*! @name AHB1_VOL_REG3 - AHB1 Voltage 3 */
/*! @{ */

#define APU_AHB1_VOL_REG3_AHB1_VOL_REG3_MASK     (0xFFFFFFFFU)
#define APU_AHB1_VOL_REG3_AHB1_VOL_REG3_SHIFT    (0U)
/*! AHB1_VOL_REG3 - AHB1 Voltage 3 */
#define APU_AHB1_VOL_REG3_AHB1_VOL_REG3(x)       (((uint32_t)(((uint32_t)(x)) << APU_AHB1_VOL_REG3_AHB1_VOL_REG3_SHIFT)) & APU_AHB1_VOL_REG3_AHB1_VOL_REG3_MASK)
/*! @} */

/*! @name AHB1_VOL_REG4 - AHB1 Voltage 4 */
/*! @{ */

#define APU_AHB1_VOL_REG4_AHB1_VOL_REG4_MASK     (0xFFFFFFFFU)
#define APU_AHB1_VOL_REG4_AHB1_VOL_REG4_SHIFT    (0U)
/*! AHB1_VOL_REG4 - AHB1 Voltage 4 */
#define APU_AHB1_VOL_REG4_AHB1_VOL_REG4(x)       (((uint32_t)(((uint32_t)(x)) << APU_AHB1_VOL_REG4_AHB1_VOL_REG4_SHIFT)) & APU_AHB1_VOL_REG4_AHB1_VOL_REG4_MASK)
/*! @} */

/*! @name DVFS_STATUS - DVFS Status */
/*! @{ */

#define APU_DVFS_STATUS_SYS_LOCK_FREQ_MASK       (0x1U)
#define APU_DVFS_STATUS_SYS_LOCK_FREQ_SHIFT      (0U)
/*! SYS_LOCK_FREQ - System Lock Frequency */
#define APU_DVFS_STATUS_SYS_LOCK_FREQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SYS_LOCK_FREQ_SHIFT)) & APU_DVFS_STATUS_SYS_LOCK_FREQ_MASK)

#define APU_DVFS_STATUS_CPU2_LOCK_FREQ_MASK      (0x2U)
#define APU_DVFS_STATUS_CPU2_LOCK_FREQ_SHIFT     (1U)
/*! CPU2_LOCK_FREQ - CPU2 Lock Frequency */
#define APU_DVFS_STATUS_CPU2_LOCK_FREQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU2_LOCK_FREQ_SHIFT)) & APU_DVFS_STATUS_CPU2_LOCK_FREQ_MASK)

#define APU_DVFS_STATUS_CPU1_LOCK_FREQ_MASK      (0x4U)
#define APU_DVFS_STATUS_CPU1_LOCK_FREQ_SHIFT     (2U)
/*! CPU1_LOCK_FREQ - CPU1 Lock Frequency */
#define APU_DVFS_STATUS_CPU1_LOCK_FREQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU1_LOCK_FREQ_SHIFT)) & APU_DVFS_STATUS_CPU1_LOCK_FREQ_MASK)

#define APU_DVFS_STATUS_LOCK_FREQ_REQ_MASK       (0x8U)
#define APU_DVFS_STATUS_LOCK_FREQ_REQ_SHIFT      (3U)
/*! LOCK_FREQ_REQ - Lock Frequency Request */
#define APU_DVFS_STATUS_LOCK_FREQ_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_LOCK_FREQ_REQ_SHIFT)) & APU_DVFS_STATUS_LOCK_FREQ_REQ_MASK)

#define APU_DVFS_STATUS_SYS_LOCK_VOL_MASK        (0x10U)
#define APU_DVFS_STATUS_SYS_LOCK_VOL_SHIFT       (4U)
/*! SYS_LOCK_VOL - System Lock Vol */
#define APU_DVFS_STATUS_SYS_LOCK_VOL(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SYS_LOCK_VOL_SHIFT)) & APU_DVFS_STATUS_SYS_LOCK_VOL_MASK)

#define APU_DVFS_STATUS_CPU2_LOCK_VOL_MASK       (0x20U)
#define APU_DVFS_STATUS_CPU2_LOCK_VOL_SHIFT      (5U)
/*! CPU2_LOCK_VOL - CPU2 Lock Vol */
#define APU_DVFS_STATUS_CPU2_LOCK_VOL(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU2_LOCK_VOL_SHIFT)) & APU_DVFS_STATUS_CPU2_LOCK_VOL_MASK)

#define APU_DVFS_STATUS_CPU1_LOCK_VOL_MASK       (0x40U)
#define APU_DVFS_STATUS_CPU1_LOCK_VOL_SHIFT      (6U)
/*! CPU1_LOCK_VOL - CPU1 Lock Vol */
#define APU_DVFS_STATUS_CPU1_LOCK_VOL(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU1_LOCK_VOL_SHIFT)) & APU_DVFS_STATUS_CPU1_LOCK_VOL_MASK)

#define APU_DVFS_STATUS_LOCK_VOL_REQ_MASK        (0x80U)
#define APU_DVFS_STATUS_LOCK_VOL_REQ_SHIFT       (7U)
/*! LOCK_VOL_REQ - Lock Vol Request */
#define APU_DVFS_STATUS_LOCK_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_LOCK_VOL_REQ_SHIFT)) & APU_DVFS_STATUS_LOCK_VOL_REQ_MASK)

#define APU_DVFS_STATUS_SYS_FREQ_CTRL_MASK       (0xF00U)
#define APU_DVFS_STATUS_SYS_FREQ_CTRL_SHIFT      (8U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_STATUS_SYS_FREQ_CTRL(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_STATUS_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_STATUS_CPU2_FREQ_CTRL_MASK      (0xF000U)
#define APU_DVFS_STATUS_CPU2_FREQ_CTRL_SHIFT     (12U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_STATUS_CPU2_FREQ_CTRL(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_STATUS_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_STATUS_CPU1_FREQ_CTRL_MASK      (0xF0000U)
#define APU_DVFS_STATUS_CPU1_FREQ_CTRL_SHIFT     (16U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_STATUS_CPU1_FREQ_CTRL(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_STATUS_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_STATUS_SOC_POWER_LVL_REACHED_MASK (0x100000U)
#define APU_DVFS_STATUS_SOC_POWER_LVL_REACHED_SHIFT (20U)
/*! SOC_POWER_LVL_REACHED - SoC Power Level Reached */
#define APU_DVFS_STATUS_SOC_POWER_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SOC_POWER_LVL_REACHED_SHIFT)) & APU_DVFS_STATUS_SOC_POWER_LVL_REACHED_MASK)

#define APU_DVFS_STATUS_USB_VOL_LVL_REACHED_MASK (0x200000U)
#define APU_DVFS_STATUS_USB_VOL_LVL_REACHED_SHIFT (21U)
/*! USB_VOL_LVL_REACHED - USB Vol Level Reached */
#define APU_DVFS_STATUS_USB_VOL_LVL_REACHED(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_USB_VOL_LVL_REACHED_SHIFT)) & APU_DVFS_STATUS_USB_VOL_LVL_REACHED_MASK)

#define APU_DVFS_STATUS_PCIE_VOL_LVL_REACHED_MASK (0x400000U)
#define APU_DVFS_STATUS_PCIE_VOL_LVL_REACHED_SHIFT (22U)
/*! PCIE_VOL_LVL_REACHED - PCIe Vol Level Reached */
#define APU_DVFS_STATUS_PCIE_VOL_LVL_REACHED(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_PCIE_VOL_LVL_REACHED_SHIFT)) & APU_DVFS_STATUS_PCIE_VOL_LVL_REACHED_MASK)

#define APU_DVFS_STATUS_APU_BUCK_LVL_CTRL_MASK   (0x3F800000U)
#define APU_DVFS_STATUS_APU_BUCK_LVL_CTRL_SHIFT  (23U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_STATUS_APU_BUCK_LVL_CTRL(x)     (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_STATUS_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_STATUS_SW_FREQ_GAURD_MASK       (0x40000000U)
#define APU_DVFS_STATUS_SW_FREQ_GAURD_SHIFT      (30U)
/*! SW_FREQ_GAURD - SW Frequency Guard */
#define APU_DVFS_STATUS_SW_FREQ_GAURD(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SW_FREQ_GAURD_SHIFT)) & APU_DVFS_STATUS_SW_FREQ_GAURD_MASK)

#define APU_DVFS_STATUS_SW_LOCK_GAURD_MASK       (0x80000000U)
#define APU_DVFS_STATUS_SW_LOCK_GAURD_SHIFT      (31U)
/*! SW_LOCK_GAURD - SW Lock Guard */
#define APU_DVFS_STATUS_SW_LOCK_GAURD(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_STATUS_SW_LOCK_GAURD_SHIFT)) & APU_DVFS_STATUS_SW_LOCK_GAURD_MASK)
/*! @} */

/*! @name DVFS_DBG_CTRL - DVFS Debug Control */
/*! @{ */

#define APU_DVFS_DBG_CTRL_DVFS_DBG_MODE_MASK     (0x1U)
#define APU_DVFS_DBG_CTRL_DVFS_DBG_MODE_SHIFT    (0U)
/*! DVFS_DBG_MODE - DVFS Debug Mode */
#define APU_DVFS_DBG_CTRL_DVFS_DBG_MODE(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_DVFS_DBG_MODE_SHIFT)) & APU_DVFS_DBG_CTRL_DVFS_DBG_MODE_MASK)

#define APU_DVFS_DBG_CTRL_START_TRIGGER_MASK     (0x2U)
#define APU_DVFS_DBG_CTRL_START_TRIGGER_SHIFT    (1U)
/*! START_TRIGGER - Start Trigger */
#define APU_DVFS_DBG_CTRL_START_TRIGGER(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_START_TRIGGER_SHIFT)) & APU_DVFS_DBG_CTRL_START_TRIGGER_MASK)

#define APU_DVFS_DBG_CTRL_END_TRIGGER_MASK       (0x4U)
#define APU_DVFS_DBG_CTRL_END_TRIGGER_SHIFT      (2U)
/*! END_TRIGGER - End Trigger */
#define APU_DVFS_DBG_CTRL_END_TRIGGER(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_END_TRIGGER_SHIFT)) & APU_DVFS_DBG_CTRL_END_TRIGGER_MASK)

#define APU_DVFS_DBG_CTRL_DBG_VOL_SEL_MASK       (0x70U)
#define APU_DVFS_DBG_CTRL_DBG_VOL_SEL_SHIFT      (4U)
/*! DBG_VOL_SEL - Debug Vol Select */
#define APU_DVFS_DBG_CTRL_DBG_VOL_SEL(x)         (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_DBG_VOL_SEL_SHIFT)) & APU_DVFS_DBG_CTRL_DBG_VOL_SEL_MASK)

#define APU_DVFS_DBG_CTRL_DBG_SLP_TIMER_SEL_MASK (0x300U)
#define APU_DVFS_DBG_CTRL_DBG_SLP_TIMER_SEL_SHIFT (8U)
/*! DBG_SLP_TIMER_SEL - Debug Sleep Timer Select */
#define APU_DVFS_DBG_CTRL_DBG_SLP_TIMER_SEL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_DBG_SLP_TIMER_SEL_SHIFT)) & APU_DVFS_DBG_CTRL_DBG_SLP_TIMER_SEL_MASK)

#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_BYPASS_MASK (0x1000U)
#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_BYPASS_SHIFT (12U)
/*! DVFS_CLK_SEL_FW_BYPASS - DVFS Clock Select FW Bypass */
#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_BYPASS_SHIFT)) & APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_BYPASS_MASK)

#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_VAL_MASK (0x2000U)
#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_VAL_SHIFT (13U)
/*! DVFS_CLK_SEL_FW_VAL - DVFS Clock Select FW Value */
#define APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_VAL_SHIFT)) & APU_DVFS_DBG_CTRL_DVFS_CLK_SEL_FW_VAL_MASK)

#define APU_DVFS_DBG_CTRL_BUCK_EFF_MODE_FIX_MASK (0x8000U)
#define APU_DVFS_DBG_CTRL_BUCK_EFF_MODE_FIX_SHIFT (15U)
/*! BUCK_EFF_MODE_FIX - BUCK Efficiency Mode Fix */
#define APU_DVFS_DBG_CTRL_BUCK_EFF_MODE_FIX(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_BUCK_EFF_MODE_FIX_SHIFT)) & APU_DVFS_DBG_CTRL_BUCK_EFF_MODE_FIX_MASK)

#define APU_DVFS_DBG_CTRL_SD_CLK_SWITCH_OK_MASK_MASK (0x10000U)
#define APU_DVFS_DBG_CTRL_SD_CLK_SWITCH_OK_MASK_SHIFT (16U)
/*! SD_CLK_SWITCH_OK_MASK - SD Clock Switch Ok Mask */
#define APU_DVFS_DBG_CTRL_SD_CLK_SWITCH_OK_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_CTRL_SD_CLK_SWITCH_OK_MASK_SHIFT)) & APU_DVFS_DBG_CTRL_SD_CLK_SWITCH_OK_MASK_MASK)
/*! @} */

/*! @name DVFS_DBG_PATTERN_DATA - DVFS Debug Pattern Data */
/*! @{ */

#define APU_DVFS_DBG_PATTERN_DATA_VOL_REQ_MASK   (0x1FU)
#define APU_DVFS_DBG_PATTERN_DATA_VOL_REQ_SHIFT  (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_PATTERN_DATA_VOL_REQ(x)     (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_VOL_REQ_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_VOL_REQ_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_SYS_FREQ_CTRL_MASK (0x1E0U)
#define APU_DVFS_DBG_PATTERN_DATA_SYS_FREQ_CTRL_SHIFT (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_PATTERN_DATA_SYS_FREQ_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_CPU2_FREQ_CTRL_MASK (0x1E00U)
#define APU_DVFS_DBG_PATTERN_DATA_CPU2_FREQ_CTRL_SHIFT (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_PATTERN_DATA_CPU2_FREQ_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_CPU1_FREQ_CTRL_MASK (0x1E000U)
#define APU_DVFS_DBG_PATTERN_DATA_CPU1_FREQ_CTRL_SHIFT (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_PATTERN_DATA_CPU1_FREQ_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_PATTERN_DATA_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_PATTERN_DATA_APU_BUCK_LVL_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_SW_VOL_REQ_MASK (0x7C00000U)
#define APU_DVFS_DBG_PATTERN_DATA_SW_VOL_REQ_SHIFT (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_PATTERN_DATA_SW_VOL_REQ(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_PATTERN_DATA_HW_VOL_REQ_MASK (0xF8000000U)
#define APU_DVFS_DBG_PATTERN_DATA_HW_VOL_REQ_SHIFT (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_PATTERN_DATA_HW_VOL_REQ(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_DATA_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_PATTERN_DATA_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG0 - DVFS Debug 0 */
/*! @{ */

#define APU_DVFS_DBG_REG0_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG0_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG0_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG0_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG0_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG0_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG0_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG0_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG0_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG0_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG0_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG0_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG0_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG0_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG0_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG0_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG0_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG0_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG0_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG0_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG0_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG0_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG0_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG0_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG0_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG0_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG0_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG0_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG0_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG1 - DVFS Debug 1 */
/*! @{ */

#define APU_DVFS_DBG_REG1_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG1_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG1_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG1_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG1_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG1_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG1_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG1_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG1_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG1_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG1_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG1_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG1_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG1_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG1_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG1_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG1_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG1_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG1_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG1_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG1_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG1_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG1_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG1_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG1_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG1_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG1_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG1_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG1_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG2 - DVFS Debug 2 */
/*! @{ */

#define APU_DVFS_DBG_REG2_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG2_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG2_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG2_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG2_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG2_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG2_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG2_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG2_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG2_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG2_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG2_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG2_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG2_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG2_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG2_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG2_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG2_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG2_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG2_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG2_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG2_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG2_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG2_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG2_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG2_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG2_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG2_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG2_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG3 - DVFS Debug 3 */
/*! @{ */

#define APU_DVFS_DBG_REG3_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG3_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG3_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG3_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG3_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG3_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG3_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG3_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG3_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG3_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG3_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG3_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG3_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG3_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG3_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG3_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG3_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG3_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG3_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG3_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG3_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG3_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG3_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG3_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG3_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG3_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG3_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG3_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG3_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG4 - DVFS Debug 4 */
/*! @{ */

#define APU_DVFS_DBG_REG4_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG4_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG4_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG4_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG4_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG4_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG4_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG4_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG4_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG4_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG4_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG4_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG4_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG4_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG4_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG4_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG4_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG4_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG4_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG4_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG4_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG4_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG4_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG4_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG4_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG4_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG4_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG4_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG4_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_REG5 - DVFS Debug 5 */
/*! @{ */

#define APU_DVFS_DBG_REG5_VOL_REQ_MASK           (0x1FU)
#define APU_DVFS_DBG_REG5_VOL_REQ_SHIFT          (0U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_REG5_VOL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG5_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG5_SYS_FREQ_CTRL_MASK     (0x1E0U)
#define APU_DVFS_DBG_REG5_SYS_FREQ_CTRL_SHIFT    (5U)
/*! SYS_FREQ_CTRL - System Frequency Control */
#define APU_DVFS_DBG_REG5_SYS_FREQ_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_SYS_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG5_SYS_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG5_CPU2_FREQ_CTRL_MASK    (0x1E00U)
#define APU_DVFS_DBG_REG5_CPU2_FREQ_CTRL_SHIFT   (9U)
/*! CPU2_FREQ_CTRL - CPU2 Frequency Control */
#define APU_DVFS_DBG_REG5_CPU2_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_CPU2_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG5_CPU2_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG5_CPU1_FREQ_CTRL_MASK    (0x1E000U)
#define APU_DVFS_DBG_REG5_CPU1_FREQ_CTRL_SHIFT   (13U)
/*! CPU1_FREQ_CTRL - CPU1 Frequency Control */
#define APU_DVFS_DBG_REG5_CPU1_FREQ_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_CPU1_FREQ_CTRL_SHIFT)) & APU_DVFS_DBG_REG5_CPU1_FREQ_CTRL_MASK)

#define APU_DVFS_DBG_REG5_APU_BUCK_LVL_CTRL_MASK (0x3E0000U)
#define APU_DVFS_DBG_REG5_APU_BUCK_LVL_CTRL_SHIFT (17U)
/*! APU_BUCK_LVL_CTRL - APU BUCK Level Control */
#define APU_DVFS_DBG_REG5_APU_BUCK_LVL_CTRL(x)   (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_APU_BUCK_LVL_CTRL_SHIFT)) & APU_DVFS_DBG_REG5_APU_BUCK_LVL_CTRL_MASK)

#define APU_DVFS_DBG_REG5_SW_VOL_REQ_MASK        (0x7C00000U)
#define APU_DVFS_DBG_REG5_SW_VOL_REQ_SHIFT       (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_REG5_SW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG5_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_REG5_HW_VOL_REQ_MASK        (0xF8000000U)
#define APU_DVFS_DBG_REG5_HW_VOL_REQ_SHIFT       (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_REG5_HW_VOL_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_REG5_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_REG5_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_STATUS - DVFS Debug Status */
/*! @{ */

#define APU_DVFS_DBG_STATUS_DVFS_DBG_LOG_DONE_MASK (0x1U)
#define APU_DVFS_DBG_STATUS_DVFS_DBG_LOG_DONE_SHIFT (0U)
/*! DVFS_DBG_LOG_DONE - DVFS Debug Log Done */
#define APU_DVFS_DBG_STATUS_DVFS_DBG_LOG_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_DVFS_DBG_LOG_DONE_SHIFT)) & APU_DVFS_DBG_STATUS_DVFS_DBG_LOG_DONE_MASK)

#define APU_DVFS_DBG_STATUS_BUCK_LVL_REACHED_MASK (0x3EU)
#define APU_DVFS_DBG_STATUS_BUCK_LVL_REACHED_SHIFT (1U)
/*! BUCK_LVL_REACHED - voltage corresponding to PMIC code 0x2 */
#define APU_DVFS_DBG_STATUS_BUCK_LVL_REACHED(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_BUCK_LVL_REACHED_SHIFT)) & APU_DVFS_DBG_STATUS_BUCK_LVL_REACHED_MASK)

#define APU_DVFS_DBG_STATUS_KEEP_DVFS_LVL_ACTIVE_MASK (0x40U)
#define APU_DVFS_DBG_STATUS_KEEP_DVFS_LVL_ACTIVE_SHIFT (6U)
/*! KEEP_DVFS_LVL_ACTIVE - Keep DVFS Level Active */
#define APU_DVFS_DBG_STATUS_KEEP_DVFS_LVL_ACTIVE(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_KEEP_DVFS_LVL_ACTIVE_SHIFT)) & APU_DVFS_DBG_STATUS_KEEP_DVFS_LVL_ACTIVE_MASK)

#define APU_DVFS_DBG_STATUS_RESET_COUNTER_SYNCED_MASK (0x80U)
#define APU_DVFS_DBG_STATUS_RESET_COUNTER_SYNCED_SHIFT (7U)
/*! RESET_COUNTER_SYNCED - voltage corresponding to PMIC code 0x3 */
#define APU_DVFS_DBG_STATUS_RESET_COUNTER_SYNCED(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_RESET_COUNTER_SYNCED_SHIFT)) & APU_DVFS_DBG_STATUS_RESET_COUNTER_SYNCED_MASK)

#define APU_DVFS_DBG_STATUS_SW_LOCK_GAURD_EN_MASK (0x100U)
#define APU_DVFS_DBG_STATUS_SW_LOCK_GAURD_EN_SHIFT (8U)
/*! SW_LOCK_GAURD_EN - SW Lock Guard Enable */
#define APU_DVFS_DBG_STATUS_SW_LOCK_GAURD_EN(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_SW_LOCK_GAURD_EN_SHIFT)) & APU_DVFS_DBG_STATUS_SW_LOCK_GAURD_EN_MASK)

#define APU_DVFS_DBG_STATUS_DVFS_CLK_SEL_DVFSCK_MASK (0x200U)
#define APU_DVFS_DBG_STATUS_DVFS_CLK_SEL_DVFSCK_SHIFT (9U)
/*! DVFS_CLK_SEL_DVFSCK - DVFS Clock Select DVFSCK */
#define APU_DVFS_DBG_STATUS_DVFS_CLK_SEL_DVFSCK(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_DVFS_CLK_SEL_DVFSCK_SHIFT)) & APU_DVFS_DBG_STATUS_DVFS_CLK_SEL_DVFSCK_MASK)

#define APU_DVFS_DBG_STATUS_EAS_SEL_MASK         (0x400U)
#define APU_DVFS_DBG_STATUS_EAS_SEL_SHIFT        (10U)
/*! EAS_SEL - EAS Select */
#define APU_DVFS_DBG_STATUS_EAS_SEL(x)           (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_EAS_SEL_SHIFT)) & APU_DVFS_DBG_STATUS_EAS_SEL_MASK)

#define APU_DVFS_DBG_STATUS_SW_FREQ_GAURD_EN_MASK (0x800U)
#define APU_DVFS_DBG_STATUS_SW_FREQ_GAURD_EN_SHIFT (11U)
/*! SW_FREQ_GAURD_EN - SW Frequency Guard Enable */
#define APU_DVFS_DBG_STATUS_SW_FREQ_GAURD_EN(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_SW_FREQ_GAURD_EN_SHIFT)) & APU_DVFS_DBG_STATUS_SW_FREQ_GAURD_EN_MASK)

#define APU_DVFS_DBG_STATUS_PMIC_TIMER_DONE_REG_MASK (0x1000U)
#define APU_DVFS_DBG_STATUS_PMIC_TIMER_DONE_REG_SHIFT (12U)
/*! PMIC_TIMER_DONE_REG - PMIC Timer Done */
#define APU_DVFS_DBG_STATUS_PMIC_TIMER_DONE_REG(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_PMIC_TIMER_DONE_REG_SHIFT)) & APU_DVFS_DBG_STATUS_PMIC_TIMER_DONE_REG_MASK)

#define APU_DVFS_DBG_STATUS_WLAN_VOL_LVL_REACHED_MASK (0x8000U)
#define APU_DVFS_DBG_STATUS_WLAN_VOL_LVL_REACHED_SHIFT (15U)
/*! WLAN_VOL_LVL_REACHED - WLAN Vol Level Reached */
#define APU_DVFS_DBG_STATUS_WLAN_VOL_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_WLAN_VOL_LVL_REACHED_SHIFT)) & APU_DVFS_DBG_STATUS_WLAN_VOL_LVL_REACHED_MASK)

#define APU_DVFS_DBG_STATUS_NFC_VOL_LVL_REACHED_MASK (0x10000U)
#define APU_DVFS_DBG_STATUS_NFC_VOL_LVL_REACHED_SHIFT (16U)
/*! NFC_VOL_LVL_REACHED - NFC Vol Level Reached */
#define APU_DVFS_DBG_STATUS_NFC_VOL_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_NFC_VOL_LVL_REACHED_SHIFT)) & APU_DVFS_DBG_STATUS_NFC_VOL_LVL_REACHED_MASK)

#define APU_DVFS_DBG_STATUS_NOM_VOL_LVL_REACHED_MASK (0x20000U)
#define APU_DVFS_DBG_STATUS_NOM_VOL_LVL_REACHED_SHIFT (17U)
/*! NOM_VOL_LVL_REACHED - Nominal Vol Level Reached */
#define APU_DVFS_DBG_STATUS_NOM_VOL_LVL_REACHED(x) (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_NOM_VOL_LVL_REACHED_SHIFT)) & APU_DVFS_DBG_STATUS_NOM_VOL_LVL_REACHED_MASK)

#define APU_DVFS_DBG_STATUS_VOL_REQ_MASK         (0x3C0000U)
#define APU_DVFS_DBG_STATUS_VOL_REQ_SHIFT        (18U)
/*! VOL_REQ - Vol Request */
#define APU_DVFS_DBG_STATUS_VOL_REQ(x)           (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_VOL_REQ_SHIFT)) & APU_DVFS_DBG_STATUS_VOL_REQ_MASK)

#define APU_DVFS_DBG_STATUS_SW_VOL_REQ_MASK      (0x7C00000U)
#define APU_DVFS_DBG_STATUS_SW_VOL_REQ_SHIFT     (22U)
/*! SW_VOL_REQ - SW Vol Request */
#define APU_DVFS_DBG_STATUS_SW_VOL_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_SW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_STATUS_SW_VOL_REQ_MASK)

#define APU_DVFS_DBG_STATUS_HW_VOL_REQ_MASK      (0xF8000000U)
#define APU_DVFS_DBG_STATUS_HW_VOL_REQ_SHIFT     (27U)
/*! HW_VOL_REQ - HW Vol Request */
#define APU_DVFS_DBG_STATUS_HW_VOL_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_STATUS_HW_VOL_REQ_SHIFT)) & APU_DVFS_DBG_STATUS_HW_VOL_REQ_MASK)
/*! @} */

/*! @name DVFS_DBG_PATTERN_MASK - DVFS Debug Patter Mask */
/*! @{ */

#define APU_DVFS_DBG_PATTERN_MASK_MASK_MASK      (0xFFFFFFFFU)
#define APU_DVFS_DBG_PATTERN_MASK_MASK_SHIFT     (0U)
/*! MASK - Mask to Delay */
#define APU_DVFS_DBG_PATTERN_MASK_MASK(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_DBG_PATTERN_MASK_MASK_SHIFT)) & APU_DVFS_DBG_PATTERN_MASK_MASK_MASK)
/*! @} */

/*! @name DVFS_PMIC_TIMER - DVFS PMIC Timer */
/*! @{ */

#define APU_DVFS_PMIC_TIMER_REF_CNT_MASK         (0xFFFFU)
#define APU_DVFS_PMIC_TIMER_REF_CNT_SHIFT        (0U)
/*! REF_CNT - Reference Count */
#define APU_DVFS_PMIC_TIMER_REF_CNT(x)           (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_TIMER_REF_CNT_SHIFT)) & APU_DVFS_PMIC_TIMER_REF_CNT_MASK)

#define APU_DVFS_PMIC_TIMER_SLP_CNT_MASK         (0xF0000U)
#define APU_DVFS_PMIC_TIMER_SLP_CNT_SHIFT        (16U)
/*! SLP_CNT - Sleep Counts */
#define APU_DVFS_PMIC_TIMER_SLP_CNT(x)           (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_TIMER_SLP_CNT_SHIFT)) & APU_DVFS_PMIC_TIMER_SLP_CNT_MASK)

#define APU_DVFS_PMIC_TIMER_ACTIVE_CNT_MASK      (0xFFF00000U)
#define APU_DVFS_PMIC_TIMER_ACTIVE_CNT_SHIFT     (20U)
/*! ACTIVE_CNT - Active Count */
#define APU_DVFS_PMIC_TIMER_ACTIVE_CNT(x)        (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_TIMER_ACTIVE_CNT_SHIFT)) & APU_DVFS_PMIC_TIMER_ACTIVE_CNT_MASK)
/*! @} */

/*! @name DVFS_PMIC_MAP - DVFS PMIC Map */
/*! @{ */

#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC0_MASK (0x7FU)
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC0_SHIFT (0U)
/*! DVFS_VOL_FOR_PMIC0 - DVFS Vol for PMIC 0 */
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC0(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC0_SHIFT)) & APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC0_MASK)

#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC1_MASK (0x7F00U)
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC1_SHIFT (8U)
/*! DVFS_VOL_FOR_PMIC1 - DVFS Vol for PMIC 1 */
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC1(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC1_SHIFT)) & APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC1_MASK)

#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC2_MASK (0x7F0000U)
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC2_SHIFT (16U)
/*! DVFS_VOL_FOR_PMIC2 - DVFS Vol for PMIC 2 */
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC2(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC2_SHIFT)) & APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC2_MASK)

#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC3_MASK (0x7F000000U)
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC3_SHIFT (24U)
/*! DVFS_VOL_FOR_PMIC3 - DVFS Vol for PMIC 3 */
#define APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC3(x)  (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC3_SHIFT)) & APU_DVFS_PMIC_MAP_DVFS_VOL_FOR_PMIC3_MASK)

#define APU_DVFS_PMIC_MAP_USE_PMIC_TIMER_MASK    (0x80000000U)
#define APU_DVFS_PMIC_MAP_USE_PMIC_TIMER_SHIFT   (31U)
/*! USE_PMIC_TIMER - Use PMIC Timer */
#define APU_DVFS_PMIC_MAP_USE_PMIC_TIMER(x)      (((uint32_t)(((uint32_t)(x)) << APU_DVFS_PMIC_MAP_USE_PMIC_TIMER_SHIFT)) & APU_DVFS_PMIC_MAP_USE_PMIC_TIMER_MASK)
/*! @} */

/*! @name LDO_CTRL - LDO Control */
/*! @{ */

#define APU_LDO_CTRL_MAIN_DELAY_CNT_EN_MASK      (0x1U)
#define APU_LDO_CTRL_MAIN_DELAY_CNT_EN_SHIFT     (0U)
/*! MAIN_DELAY_CNT_EN - Main Delay Count Enable */
#define APU_LDO_CTRL_MAIN_DELAY_CNT_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_MAIN_DELAY_CNT_EN_SHIFT)) & APU_LDO_CTRL_MAIN_DELAY_CNT_EN_MASK)

#define APU_LDO_CTRL_BACKUP_DELAY_CNT_EN_MASK    (0x2U)
#define APU_LDO_CTRL_BACKUP_DELAY_CNT_EN_SHIFT   (1U)
/*! BACKUP_DELAY_CNT_EN - Backup Delay Count Enable */
#define APU_LDO_CTRL_BACKUP_DELAY_CNT_EN(x)      (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_BACKUP_DELAY_CNT_EN_SHIFT)) & APU_LDO_CTRL_BACKUP_DELAY_CNT_EN_MASK)

#define APU_LDO_CTRL_MAIN_DELAY_COUNTER_VAL_MASK (0x30U)
#define APU_LDO_CTRL_MAIN_DELAY_COUNTER_VAL_SHIFT (4U)
/*! MAIN_DELAY_COUNTER_VAL - Main Delay Counter Value */
#define APU_LDO_CTRL_MAIN_DELAY_COUNTER_VAL(x)   (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_MAIN_DELAY_COUNTER_VAL_SHIFT)) & APU_LDO_CTRL_MAIN_DELAY_COUNTER_VAL_MASK)

#define APU_LDO_CTRL_BACKUP_DELAY_COUNTER_VAL_MASK (0xFF00U)
#define APU_LDO_CTRL_BACKUP_DELAY_COUNTER_VAL_SHIFT (8U)
/*! BACKUP_DELAY_COUNTER_VAL - Backup Delay Counter Value */
#define APU_LDO_CTRL_BACKUP_DELAY_COUNTER_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_BACKUP_DELAY_COUNTER_VAL_SHIFT)) & APU_LDO_CTRL_BACKUP_DELAY_COUNTER_VAL_MASK)

#define APU_LDO_CTRL_USE_XOSC_EN_AS_SEL_MASK     (0x40000000U)
#define APU_LDO_CTRL_USE_XOSC_EN_AS_SEL_SHIFT    (30U)
/*! USE_XOSC_EN_AS_SEL - Use XOSC Enable as Select */
#define APU_LDO_CTRL_USE_XOSC_EN_AS_SEL(x)       (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_USE_XOSC_EN_AS_SEL_SHIFT)) & APU_LDO_CTRL_USE_XOSC_EN_AS_SEL_MASK)

#define APU_LDO_CTRL_KEEP_LDO_MAIN_WHILE_SLP_MASK (0x80000000U)
#define APU_LDO_CTRL_KEEP_LDO_MAIN_WHILE_SLP_SHIFT (31U)
/*! KEEP_LDO_MAIN_WHILE_SLP - Keep LDO Main While Sleep */
#define APU_LDO_CTRL_KEEP_LDO_MAIN_WHILE_SLP(x)  (((uint32_t)(((uint32_t)(x)) << APU_LDO_CTRL_KEEP_LDO_MAIN_WHILE_SLP_SHIFT)) & APU_LDO_CTRL_KEEP_LDO_MAIN_WHILE_SLP_MASK)
/*! @} */

/*! @name LDO_BACKUP_LVL_MAP1 - LDO Backup Level Map 1 */
/*! @{ */

#define APU_LDO_BACKUP_LVL_MAP1_LDO_BACKUP_LVL_MAP1_MASK (0xFFFFFFFFU)
#define APU_LDO_BACKUP_LVL_MAP1_LDO_BACKUP_LVL_MAP1_SHIFT (0U)
/*! LDO_BACKUP_LVL_MAP1 - LDO Backup Level Map 1 */
#define APU_LDO_BACKUP_LVL_MAP1_LDO_BACKUP_LVL_MAP1(x) (((uint32_t)(((uint32_t)(x)) << APU_LDO_BACKUP_LVL_MAP1_LDO_BACKUP_LVL_MAP1_SHIFT)) & APU_LDO_BACKUP_LVL_MAP1_LDO_BACKUP_LVL_MAP1_MASK)
/*! @} */

/*! @name LDO_BACKUP_LVL_MAP2 - LDO Backup Level Map 2 */
/*! @{ */

#define APU_LDO_BACKUP_LVL_MAP2_LDO_BACKUP_LVL_MAP2_MASK (0xFFFFFFFFU)
#define APU_LDO_BACKUP_LVL_MAP2_LDO_BACKUP_LVL_MAP2_SHIFT (0U)
/*! LDO_BACKUP_LVL_MAP2 - LDO Backup Level Map 2 */
#define APU_LDO_BACKUP_LVL_MAP2_LDO_BACKUP_LVL_MAP2(x) (((uint32_t)(((uint32_t)(x)) << APU_LDO_BACKUP_LVL_MAP2_LDO_BACKUP_LVL_MAP2_SHIFT)) & APU_LDO_BACKUP_LVL_MAP2_LDO_BACKUP_LVL_MAP2_MASK)
/*! @} */

/*! @name LDO_STATUS - LDO Status */
/*! @{ */

#define APU_LDO_STATUS_LDO_BACKUP_LVL_MASK       (0x7U)
#define APU_LDO_STATUS_LDO_BACKUP_LVL_SHIFT      (0U)
/*! LDO_BACKUP_LVL - LDO Backup Level */
#define APU_LDO_STATUS_LDO_BACKUP_LVL(x)         (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_LDO_BACKUP_LVL_SHIFT)) & APU_LDO_STATUS_LDO_BACKUP_LVL_MASK)

#define APU_LDO_STATUS_LDO_MAIN_LVL_MASK         (0xF0U)
#define APU_LDO_STATUS_LDO_MAIN_LVL_SHIFT        (4U)
/*! LDO_MAIN_LVL - LDO Main Level */
#define APU_LDO_STATUS_LDO_MAIN_LVL(x)           (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_LDO_MAIN_LVL_SHIFT)) & APU_LDO_STATUS_LDO_MAIN_LVL_MASK)

#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MASK    (0x300U)
#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT_SHIFT   (8U)
/*! MAIN_PD_DELAY_CNT - Main Powerdown Delay Count */
#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT(x)      (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_MAIN_PD_DELAY_CNT_SHIFT)) & APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MASK)

#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MET_MASK (0x400U)
#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MET_SHIFT (10U)
/*! MAIN_PD_DELAY_CNT_MET - Main Powerdown Delay Count Met */
#define APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MET(x)  (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MET_SHIFT)) & APU_LDO_STATUS_MAIN_PD_DELAY_CNT_MET_MASK)

#define APU_LDO_STATUS_LDO_MAIN_PD_MASK          (0x800U)
#define APU_LDO_STATUS_LDO_MAIN_PD_SHIFT         (11U)
/*! LDO_MAIN_PD - LDO Main Powerdown */
#define APU_LDO_STATUS_LDO_MAIN_PD(x)            (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_LDO_MAIN_PD_SHIFT)) & APU_LDO_STATUS_LDO_MAIN_PD_MASK)

#define APU_LDO_STATUS_ACTIVE_SEL_MASK           (0x1000U)
#define APU_LDO_STATUS_ACTIVE_SEL_SHIFT          (12U)
/*! ACTIVE_SEL - Active Select */
#define APU_LDO_STATUS_ACTIVE_SEL(x)             (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_ACTIVE_SEL_SHIFT)) & APU_LDO_STATUS_ACTIVE_SEL_MASK)

#define APU_LDO_STATUS_BACK_DELAY_CNT_MET_MASK   (0x2000U)
#define APU_LDO_STATUS_BACK_DELAY_CNT_MET_SHIFT  (13U)
/*! BACK_DELAY_CNT_MET - Back Delay Count Met */
#define APU_LDO_STATUS_BACK_DELAY_CNT_MET(x)     (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_BACK_DELAY_CNT_MET_SHIFT)) & APU_LDO_STATUS_BACK_DELAY_CNT_MET_MASK)

#define APU_LDO_STATUS_D_LDO_BACKUP_EN_MASK      (0x4000U)
#define APU_LDO_STATUS_D_LDO_BACKUP_EN_SHIFT     (14U)
/*! D_LDO_BACKUP_EN - D LDO Backup Enable */
#define APU_LDO_STATUS_D_LDO_BACKUP_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_D_LDO_BACKUP_EN_SHIFT)) & APU_LDO_STATUS_D_LDO_BACKUP_EN_MASK)

#define APU_LDO_STATUS_LDO_BACKUP_EN_MASK        (0x8000U)
#define APU_LDO_STATUS_LDO_BACKUP_EN_SHIFT       (15U)
/*! LDO_BACKUP_EN - LDO Backup Enable */
#define APU_LDO_STATUS_LDO_BACKUP_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_LDO_BACKUP_EN_SHIFT)) & APU_LDO_STATUS_LDO_BACKUP_EN_MASK)

#define APU_LDO_STATUS_BACKUP_DELAY_CNT_MASK     (0xFF0000U)
#define APU_LDO_STATUS_BACKUP_DELAY_CNT_SHIFT    (16U)
/*! BACKUP_DELAY_CNT - Backup Delay Count (to check testbus) */
#define APU_LDO_STATUS_BACKUP_DELAY_CNT(x)       (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_BACKUP_DELAY_CNT_SHIFT)) & APU_LDO_STATUS_BACKUP_DELAY_CNT_MASK)

#define APU_LDO_STATUS_BKUP_LVL2_SEL_MASK        (0x1000000U)
#define APU_LDO_STATUS_BKUP_LVL2_SEL_SHIFT       (24U)
/*! BKUP_LVL2_SEL - Backup Level 2 Select */
#define APU_LDO_STATUS_BKUP_LVL2_SEL(x)          (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_BKUP_LVL2_SEL_SHIFT)) & APU_LDO_STATUS_BKUP_LVL2_SEL_MASK)

#define APU_LDO_STATUS_BKUP_LVL1_SEL_MASK        (0x2000000U)
#define APU_LDO_STATUS_BKUP_LVL1_SEL_SHIFT       (25U)
/*! BKUP_LVL1_SEL - Backup Level 1 Select */
#define APU_LDO_STATUS_BKUP_LVL1_SEL(x)          (((uint32_t)(((uint32_t)(x)) << APU_LDO_STATUS_BKUP_LVL1_SEL_SHIFT)) & APU_LDO_STATUS_BKUP_LVL1_SEL_MASK)
/*! @} */

/*! @name RC32_CAL_CTRL - RC32 Calibration Control */
/*! @{ */

#define APU_RC32_CAL_CTRL_RC32_FULL_CAL_EN_MASK  (0x1U)
#define APU_RC32_CAL_CTRL_RC32_FULL_CAL_EN_SHIFT (0U)
/*! RC32_FULL_CAL_EN - RC32 Full Calibration Enable */
#define APU_RC32_CAL_CTRL_RC32_FULL_CAL_EN(x)    (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_CTRL_RC32_FULL_CAL_EN_SHIFT)) & APU_RC32_CAL_CTRL_RC32_FULL_CAL_EN_MASK)

#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_MASK (0x2U)
#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_SHIFT (1U)
/*! RC32_PARTIAL_CAL_EN - RC32 Partial Calibration Enable */
#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_SHIFT)) & APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_MASK)

#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_ON_BT_WKUP_MASK (0x4U)
#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_ON_BT_WKUP_SHIFT (2U)
/*! RC32_PARTIAL_CAL_EN_ON_BT_WKUP - RC32 Partial Calibration Enable on Bluetooth Wakeup */
#define APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_ON_BT_WKUP(x) (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_ON_BT_WKUP_SHIFT)) & APU_RC32_CAL_CTRL_RC32_PARTIAL_CAL_EN_ON_BT_WKUP_MASK)

#define APU_RC32_CAL_CTRL_USE_RC32_CAL_DONE_MASK (0x8U)
#define APU_RC32_CAL_CTRL_USE_RC32_CAL_DONE_SHIFT (3U)
/*! USE_RC32_CAL_DONE - Use RC32 Calibration Done */
#define APU_RC32_CAL_CTRL_USE_RC32_CAL_DONE(x)   (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_CTRL_USE_RC32_CAL_DONE_SHIFT)) & APU_RC32_CAL_CTRL_USE_RC32_CAL_DONE_MASK)

#define APU_RC32_CAL_CTRL_RC32_CAL_VOL_VAL_MASK  (0x7F0U)
#define APU_RC32_CAL_CTRL_RC32_CAL_VOL_VAL_SHIFT (4U)
/*! RC32_CAL_VOL_VAL - RC32 Calibration Vol Value */
#define APU_RC32_CAL_CTRL_RC32_CAL_VOL_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_CTRL_RC32_CAL_VOL_VAL_SHIFT)) & APU_RC32_CAL_CTRL_RC32_CAL_VOL_VAL_MASK)
/*! @} */

/*! @name RC32_CAL_SLPCLK_TIMER - RC32 Calibration Sleep Clock Timer */
/*! @{ */

#define APU_RC32_CAL_SLPCLK_TIMER_RC32_CAL_SLPCLK_TIMER_MASK (0xFFFFFFFFU)
#define APU_RC32_CAL_SLPCLK_TIMER_RC32_CAL_SLPCLK_TIMER_SHIFT (0U)
/*! RC32_CAL_SLPCLK_TIMER - RC32 Calibration Sleep Clock Timer */
#define APU_RC32_CAL_SLPCLK_TIMER_RC32_CAL_SLPCLK_TIMER(x) (((uint32_t)(((uint32_t)(x)) << APU_RC32_CAL_SLPCLK_TIMER_RC32_CAL_SLPCLK_TIMER_SHIFT)) & APU_RC32_CAL_SLPCLK_TIMER_RC32_CAL_SLPCLK_TIMER_MASK)
/*! @} */

/*! @name RC32CAL_SLPCLK_CNT_RD - RC32 Calibration Sleep Clock Count Read */
/*! @{ */

#define APU_RC32CAL_SLPCLK_CNT_RD_RC32CAL_SLPCLK_CNT_RD_MASK (0xFFFFFFFFU)
#define APU_RC32CAL_SLPCLK_CNT_RD_RC32CAL_SLPCLK_CNT_RD_SHIFT (0U)
/*! RC32CAL_SLPCLK_CNT_RD - RC32 Calibration Sleep Clock Count Read */
#define APU_RC32CAL_SLPCLK_CNT_RD_RC32CAL_SLPCLK_CNT_RD(x) (((uint32_t)(((uint32_t)(x)) << APU_RC32CAL_SLPCLK_CNT_RD_RC32CAL_SLPCLK_CNT_RD_SHIFT)) & APU_RC32CAL_SLPCLK_CNT_RD_RC32CAL_SLPCLK_CNT_RD_MASK)
/*! @} */

/*! @name TSTBUS_DATA - Testbus Data */
/*! @{ */

#define APU_TSTBUS_DATA_TSTBUS_DATA_MASK         (0xFFFFFFFFU)
#define APU_TSTBUS_DATA_TSTBUS_DATA_SHIFT        (0U)
/*! TSTBUS_DATA - Testbus Data */
#define APU_TSTBUS_DATA_TSTBUS_DATA(x)           (((uint32_t)(((uint32_t)(x)) << APU_TSTBUS_DATA_TSTBUS_DATA_SHIFT)) & APU_TSTBUS_DATA_TSTBUS_DATA_MASK)
/*! @} */

/*! @name TST_CTRL - Test Control */
/*! @{ */

#define APU_TST_CTRL_BT_BLE_TST_CTRL_MASK        (0x1U)
#define APU_TST_CTRL_BT_BLE_TST_CTRL_SHIFT       (0U)
/*! BT_BLE_TST_CTRL - Bluetooth BLE Test Control */
#define APU_TST_CTRL_BT_BLE_TST_CTRL(x)          (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_BT_BLE_TST_CTRL_SHIFT)) & APU_TST_CTRL_BT_BLE_TST_CTRL_MASK)

#define APU_TST_CTRL_FM_TST_CTRL_MASK            (0x2U)
#define APU_TST_CTRL_FM_TST_CTRL_SHIFT           (1U)
/*! FM_TST_CTRL - FM Test Control */
#define APU_TST_CTRL_FM_TST_CTRL(x)              (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_FM_TST_CTRL_SHIFT)) & APU_TST_CTRL_FM_TST_CTRL_MASK)

#define APU_TST_CTRL_NFC_TST_CTRL_MASK           (0x4U)
#define APU_TST_CTRL_NFC_TST_CTRL_SHIFT          (2U)
/*! NFC_TST_CTRL - NFC Test Control */
#define APU_TST_CTRL_NFC_TST_CTRL(x)             (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_NFC_TST_CTRL_SHIFT)) & APU_TST_CTRL_NFC_TST_CTRL_MASK)

#define APU_TST_CTRL_CPU2_CP15_SLP_CTRL_MASK     (0x8U)
#define APU_TST_CTRL_CPU2_CP15_SLP_CTRL_SHIFT    (3U)
/*! CPU2_CP15_SLP_CTRL - CPU2 CP15 Sleep Control */
#define APU_TST_CTRL_CPU2_CP15_SLP_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_CPU2_CP15_SLP_CTRL_SHIFT)) & APU_TST_CTRL_CPU2_CP15_SLP_CTRL_MASK)

#define APU_TST_CTRL_WLAN_TST_CTRL_MASK          (0x10U)
#define APU_TST_CTRL_WLAN_TST_CTRL_SHIFT         (4U)
/*! WLAN_TST_CTRL - WLAN Test Control */
#define APU_TST_CTRL_WLAN_TST_CTRL(x)            (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_WLAN_TST_CTRL_SHIFT)) & APU_TST_CTRL_WLAN_TST_CTRL_MASK)

#define APU_TST_CTRL_CPU1_CP15_SLP_CTRL_MASK     (0x20U)
#define APU_TST_CTRL_CPU1_CP15_SLP_CTRL_SHIFT    (5U)
/*! CPU1_CP15_SLP_CTRL - CPU1 CP15 Sleep Control */
#define APU_TST_CTRL_CPU1_CP15_SLP_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_CPU1_CP15_SLP_CTRL_SHIFT)) & APU_TST_CTRL_CPU1_CP15_SLP_CTRL_MASK)

#define APU_TST_CTRL_CPU2_HOST_TST_CTRL_MASK     (0xF80U)
#define APU_TST_CTRL_CPU2_HOST_TST_CTRL_SHIFT    (7U)
/*! CPU2_HOST_TST_CTRL - CPU2 Host Test Control */
#define APU_TST_CTRL_CPU2_HOST_TST_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_CPU2_HOST_TST_CTRL_SHIFT)) & APU_TST_CTRL_CPU2_HOST_TST_CTRL_MASK)

#define APU_TST_CTRL_TEST_MUX_SEL_SHIFT_BIT_MASK (0xF000U)
#define APU_TST_CTRL_TEST_MUX_SEL_SHIFT_BIT_SHIFT (12U)
/*! TEST_MUX_SEL_SHIFT_BIT - Test MUX Select Shift */
#define APU_TST_CTRL_TEST_MUX_SEL_SHIFT_BIT(x)   (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_TEST_MUX_SEL_SHIFT_BIT_SHIFT)) & APU_TST_CTRL_TEST_MUX_SEL_SHIFT_BIT_MASK)

#define APU_TST_CTRL_APU_TESTBUS_SEL_MASK        (0x1F0000U)
#define APU_TST_CTRL_APU_TESTBUS_SEL_SHIFT       (16U)
/*! APU_TESTBUS_SEL - APU Testbus Select */
#define APU_TST_CTRL_APU_TESTBUS_SEL(x)          (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_APU_TESTBUS_SEL_SHIFT)) & APU_TST_CTRL_APU_TESTBUS_SEL_MASK)

#define APU_TST_CTRL_APU_REG_SUBTEST_SEL_MASK    (0x600000U)
#define APU_TST_CTRL_APU_REG_SUBTEST_SEL_SHIFT   (21U)
/*! APU_REG_SUBTEST_SEL - APU Reg Subtest Select */
#define APU_TST_CTRL_APU_REG_SUBTEST_SEL(x)      (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_APU_REG_SUBTEST_SEL_SHIFT)) & APU_TST_CTRL_APU_REG_SUBTEST_SEL_MASK)

#define APU_TST_CTRL_USE_SOC_APU_SUBTEST_MASK    (0x800000U)
#define APU_TST_CTRL_USE_SOC_APU_SUBTEST_SHIFT   (23U)
/*! USE_SOC_APU_SUBTEST - Use SoC APU Subtest */
#define APU_TST_CTRL_USE_SOC_APU_SUBTEST(x)      (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_USE_SOC_APU_SUBTEST_SHIFT)) & APU_TST_CTRL_USE_SOC_APU_SUBTEST_MASK)

#define APU_TST_CTRL_HOST_TST_CTRL_MASK          (0x3000000U)
#define APU_TST_CTRL_HOST_TST_CTRL_SHIFT         (24U)
/*! HOST_TST_CTRL - Host Test Control */
#define APU_TST_CTRL_HOST_TST_CTRL(x)            (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_HOST_TST_CTRL_SHIFT)) & APU_TST_CTRL_HOST_TST_CTRL_MASK)

#define APU_TST_CTRL_CPU1_HOST_TST_CTRL_MASK     (0x70000000U)
#define APU_TST_CTRL_CPU1_HOST_TST_CTRL_SHIFT    (28U)
/*! CPU1_HOST_TST_CTRL - CPU1 Host Test Control */
#define APU_TST_CTRL_CPU1_HOST_TST_CTRL(x)       (((uint32_t)(((uint32_t)(x)) << APU_TST_CTRL_CPU1_HOST_TST_CTRL_SHIFT)) & APU_TST_CTRL_CPU1_HOST_TST_CTRL_MASK)
/*! @} */

/*! @name BCA_LTE_CTRL - BCA LTE Control */
/*! @{ */

#define APU_BCA_LTE_CTRL_LTE_CNT_START_MASK      (0x1U)
#define APU_BCA_LTE_CTRL_LTE_CNT_START_SHIFT     (0U)
/*! LTE_CNT_START - LTE Count Start */
#define APU_BCA_LTE_CTRL_LTE_CNT_START(x)        (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_LTE_CNT_START_SHIFT)) & APU_BCA_LTE_CTRL_LTE_CNT_START_MASK)

#define APU_BCA_LTE_CTRL_LTE_TMR1_INT_MASK       (0x2U)
#define APU_BCA_LTE_CTRL_LTE_TMR1_INT_SHIFT      (1U)
/*! LTE_TMR1_INT - LTE TMR1 Interrupt */
#define APU_BCA_LTE_CTRL_LTE_TMR1_INT(x)         (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_LTE_TMR1_INT_SHIFT)) & APU_BCA_LTE_CTRL_LTE_TMR1_INT_MASK)

#define APU_BCA_LTE_CTRL_LTE_TMR2_INT_MASK       (0x4U)
#define APU_BCA_LTE_CTRL_LTE_TMR2_INT_SHIFT      (2U)
/*! LTE_TMR2_INT - LTE TMR2 Interrupt */
#define APU_BCA_LTE_CTRL_LTE_TMR2_INT(x)         (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_LTE_TMR2_INT_SHIFT)) & APU_BCA_LTE_CTRL_LTE_TMR2_INT_MASK)

#define APU_BCA_LTE_CTRL_LTE_TMR1_CNT_FREEZE_MASK (0x8U)
#define APU_BCA_LTE_CTRL_LTE_TMR1_CNT_FREEZE_SHIFT (3U)
/*! LTE_TMR1_CNT_FREEZE - LTE TMR1 Count Freeze */
#define APU_BCA_LTE_CTRL_LTE_TMR1_CNT_FREEZE(x)  (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_LTE_TMR1_CNT_FREEZE_SHIFT)) & APU_BCA_LTE_CTRL_LTE_TMR1_CNT_FREEZE_MASK)

#define APU_BCA_LTE_CTRL_LTE_TMR2_CNT_FREEZE_MASK (0x10U)
#define APU_BCA_LTE_CTRL_LTE_TMR2_CNT_FREEZE_SHIFT (4U)
/*! LTE_TMR2_CNT_FREEZE - LTE TMR2 Count Freeze */
#define APU_BCA_LTE_CTRL_LTE_TMR2_CNT_FREEZE(x)  (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_LTE_TMR2_CNT_FREEZE_SHIFT)) & APU_BCA_LTE_CTRL_LTE_TMR2_CNT_FREEZE_MASK)

#define APU_BCA_LTE_CTRL_BCA_LTE_CLK_BYP_MASK    (0x80U)
#define APU_BCA_LTE_CTRL_BCA_LTE_CLK_BYP_SHIFT   (7U)
/*! BCA_LTE_CLK_BYP - BCA LTE Clock Bypass */
#define APU_BCA_LTE_CTRL_BCA_LTE_CLK_BYP(x)      (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_BCA_LTE_CLK_BYP_SHIFT)) & APU_BCA_LTE_CTRL_BCA_LTE_CLK_BYP_MASK)

#define APU_BCA_LTE_CTRL_BCA_WL_LTE_COEX_EN_MASK (0x8000U)
#define APU_BCA_LTE_CTRL_BCA_WL_LTE_COEX_EN_SHIFT (15U)
/*! BCA_WL_LTE_COEX_EN - BCA WLAN LTE Coexistence Enable */
#define APU_BCA_LTE_CTRL_BCA_WL_LTE_COEX_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_BCA_WL_LTE_COEX_EN_SHIFT)) & APU_BCA_LTE_CTRL_BCA_WL_LTE_COEX_EN_MASK)

#define APU_BCA_LTE_CTRL_BCA_BT_LTE_COEX_EN_MASK (0x10000U)
#define APU_BCA_LTE_CTRL_BCA_BT_LTE_COEX_EN_SHIFT (16U)
/*! BCA_BT_LTE_COEX_EN - BCA Bluetooth LTE Coexistence Enable */
#define APU_BCA_LTE_CTRL_BCA_BT_LTE_COEX_EN(x)   (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_BCA_BT_LTE_COEX_EN_SHIFT)) & APU_BCA_LTE_CTRL_BCA_BT_LTE_COEX_EN_MASK)

#define APU_BCA_LTE_CTRL_USE_SLNA_WHILE_BT_MASK  (0x100000U)
#define APU_BCA_LTE_CTRL_USE_SLNA_WHILE_BT_SHIFT (20U)
/*! USE_SLNA_WHILE_BT - Use sLNA While Bluetooth */
#define APU_BCA_LTE_CTRL_USE_SLNA_WHILE_BT(x)    (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_USE_SLNA_WHILE_BT_SHIFT)) & APU_BCA_LTE_CTRL_USE_SLNA_WHILE_BT_MASK)

#define APU_BCA_LTE_CTRL_BCA_FORCE_ON_WHILE_BT_MASK (0x40000000U)
#define APU_BCA_LTE_CTRL_BCA_FORCE_ON_WHILE_BT_SHIFT (30U)
/*! BCA_FORCE_ON_WHILE_BT - BCA Force on While Bluetooth */
#define APU_BCA_LTE_CTRL_BCA_FORCE_ON_WHILE_BT(x) (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_BCA_FORCE_ON_WHILE_BT_SHIFT)) & APU_BCA_LTE_CTRL_BCA_FORCE_ON_WHILE_BT_MASK)

#define APU_BCA_LTE_CTRL_BCA_CLK_FW_EN_MASK      (0x80000000U)
#define APU_BCA_LTE_CTRL_BCA_CLK_FW_EN_SHIFT     (31U)
/*! BCA_CLK_FW_EN - BCA Clock FW Enable */
#define APU_BCA_LTE_CTRL_BCA_CLK_FW_EN(x)        (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_CTRL_BCA_CLK_FW_EN_SHIFT)) & APU_BCA_LTE_CTRL_BCA_CLK_FW_EN_MASK)
/*! @} */

/*! @name BCA_LTE_TIMER1 - BCA LTE Timer 1 */
/*! @{ */

#define APU_BCA_LTE_TIMER1_BCA_LTE_TIMER1_MASK   (0xFFFFFFFFU)
#define APU_BCA_LTE_TIMER1_BCA_LTE_TIMER1_SHIFT  (0U)
/*! BCA_LTE_TIMER1 - BCA LTE Timer 1 */
#define APU_BCA_LTE_TIMER1_BCA_LTE_TIMER1(x)     (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_TIMER1_BCA_LTE_TIMER1_SHIFT)) & APU_BCA_LTE_TIMER1_BCA_LTE_TIMER1_MASK)
/*! @} */

/*! @name BCA_LTE_TIMER2 - BCA LTE Timer 2 */
/*! @{ */

#define APU_BCA_LTE_TIMER2_BCA_LTE_TIMER2_MASK   (0xFFFFFFFFU)
#define APU_BCA_LTE_TIMER2_BCA_LTE_TIMER2_SHIFT  (0U)
/*! BCA_LTE_TIMER2 - BCA LTE Timer 2 */
#define APU_BCA_LTE_TIMER2_BCA_LTE_TIMER2(x)     (((uint32_t)(((uint32_t)(x)) << APU_BCA_LTE_TIMER2_BCA_LTE_TIMER2_SHIFT)) & APU_BCA_LTE_TIMER2_BCA_LTE_TIMER2_MASK)
/*! @} */

/*! @name BCA_MWS_WKUP_TIMER - BCA MWS Wakeup Timer */
/*! @{ */

#define APU_BCA_MWS_WKUP_TIMER_BCA_MWS_TIMER_MASK (0xFFFFU)
#define APU_BCA_MWS_WKUP_TIMER_BCA_MWS_TIMER_SHIFT (0U)
/*! BCA_MWS_TIMER - BCA MWS Timer */
#define APU_BCA_MWS_WKUP_TIMER_BCA_MWS_TIMER(x)  (((uint32_t)(((uint32_t)(x)) << APU_BCA_MWS_WKUP_TIMER_BCA_MWS_TIMER_SHIFT)) & APU_BCA_MWS_WKUP_TIMER_BCA_MWS_TIMER_MASK)

#define APU_BCA_MWS_WKUP_TIMER_BCA_BCN_WKUP_CNT_MASK (0xF0000000U)
#define APU_BCA_MWS_WKUP_TIMER_BCA_BCN_WKUP_CNT_SHIFT (28U)
/*! BCA_BCN_WKUP_CNT - BCA Beacon Wakeup Count */
#define APU_BCA_MWS_WKUP_TIMER_BCA_BCN_WKUP_CNT(x) (((uint32_t)(((uint32_t)(x)) << APU_BCA_MWS_WKUP_TIMER_BCA_BCN_WKUP_CNT_SHIFT)) & APU_BCA_MWS_WKUP_TIMER_BCA_BCN_WKUP_CNT_MASK)
/*! @} */

/*! @name BT2_CTRL - Bluetooth 2 Control */
/*! @{ */

#define APU_BT2_CTRL_BT_SLP_RDY_MASK             (0x1U)
#define APU_BT2_CTRL_BT_SLP_RDY_SHIFT            (0U)
/*! BT_SLP_RDY - Bluetooth Sleep Ready */
#define APU_BT2_CTRL_BT_SLP_RDY(x)               (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_SLP_RDY_SHIFT)) & APU_BT2_CTRL_BT_SLP_RDY_MASK)

#define APU_BT2_CTRL_BT_SLP_RDYMASK_MASK         (0x2U)
#define APU_BT2_CTRL_BT_SLP_RDYMASK_SHIFT        (1U)
/*! BT_SLP_RDYMASK - Bluetoot Sleep Ready Mask */
#define APU_BT2_CTRL_BT_SLP_RDYMASK(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_SLP_RDYMASK_SHIFT)) & APU_BT2_CTRL_BT_SLP_RDYMASK_MASK)

#define APU_BT2_CTRL_BT_SLP_RDY_FW_MASK          (0x4U)
#define APU_BT2_CTRL_BT_SLP_RDY_FW_SHIFT         (2U)
/*! BT_SLP_RDY_FW - Bluetooth Sleep Ready FW */
#define APU_BT2_CTRL_BT_SLP_RDY_FW(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_SLP_RDY_FW_SHIFT)) & APU_BT2_CTRL_BT_SLP_RDY_FW_MASK)

#define APU_BT2_CTRL_BTU_CLK_NCO_MODE_SEL_EN_MASK (0x8U)
#define APU_BT2_CTRL_BTU_CLK_NCO_MODE_SEL_EN_SHIFT (3U)
/*! BTU_CLK_NCO_MODE_SEL_EN - 1- ignore the btu_clk_nco_mode from CIU2, keep the bt_clk_req as XP wakeup source */
#define APU_BT2_CTRL_BTU_CLK_NCO_MODE_SEL_EN(x)  (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BTU_CLK_NCO_MODE_SEL_EN_SHIFT)) & APU_BT2_CTRL_BTU_CLK_NCO_MODE_SEL_EN_MASK)

#define APU_BT2_CTRL_USE_BT_INTR_SLP_MASK        (0x80U)
#define APU_BT2_CTRL_USE_BT_INTR_SLP_SHIFT       (7U)
/*! USE_BT_INTR_SLP - Use Bluetooth interrupt Sleep */
#define APU_BT2_CTRL_USE_BT_INTR_SLP(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_USE_BT_INTR_SLP_SHIFT)) & APU_BT2_CTRL_USE_BT_INTR_SLP_MASK)

#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL1_MASK  (0x4000000U)
#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL1_SHIFT (26U)
/*! BT_CLK_SYNC_MODE_SEL1 - Bluetooth Clock Sync Mode Select 1 */
#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL1(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL1_SHIFT)) & APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL1_MASK)

#define APU_BT2_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_MASK (0x8000000U)
#define APU_BT2_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_SHIFT (27U)
/*! USE_GLITCH_FREE_BT_CLK_REQ - Use Glitch-Free Bluetooth Clock Request */
#define APU_BT2_CTRL_USE_GLITCH_FREE_BT_CLK_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_SHIFT)) & APU_BT2_CTRL_USE_GLITCH_FREE_BT_CLK_REQ_MASK)

#define APU_BT2_CTRL_BRF_CLK_SYNC_MODE_SEL_MASK  (0x10000000U)
#define APU_BT2_CTRL_BRF_CLK_SYNC_MODE_SEL_SHIFT (28U)
/*! BRF_CLK_SYNC_MODE_SEL - BRF Clock Sync Mode Select */
#define APU_BT2_CTRL_BRF_CLK_SYNC_MODE_SEL(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BRF_CLK_SYNC_MODE_SEL_SHIFT)) & APU_BT2_CTRL_BRF_CLK_SYNC_MODE_SEL_MASK)

#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL0_MASK  (0x20000000U)
#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL0_SHIFT (29U)
/*! BT_CLK_SYNC_MODE_SEL0 - Bluetooth Clock Sync Mode Select 0 */
#define APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL0(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL0_SHIFT)) & APU_BT2_CTRL_BT_CLK_SYNC_MODE_SEL0_MASK)

#define APU_BT2_CTRL_BT_PLL_SYNC_MODE_SEL_MASK   (0x40000000U)
#define APU_BT2_CTRL_BT_PLL_SYNC_MODE_SEL_SHIFT  (30U)
/*! BT_PLL_SYNC_MODE_SEL - Bluetooth PLL Sync Mode Select */
#define APU_BT2_CTRL_BT_PLL_SYNC_MODE_SEL(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_PLL_SYNC_MODE_SEL_SHIFT)) & APU_BT2_CTRL_BT_PLL_SYNC_MODE_SEL_MASK)

#define APU_BT2_CTRL_BT_HOST_SLP_RDY_MASK        (0x80000000U)
#define APU_BT2_CTRL_BT_HOST_SLP_RDY_SHIFT       (31U)
/*! BT_HOST_SLP_RDY - Bluetooth Host Sleep Ready */
#define APU_BT2_CTRL_BT_HOST_SLP_RDY(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_CTRL_BT_HOST_SLP_RDY_SHIFT)) & APU_BT2_CTRL_BT_HOST_SLP_RDY_MASK)
/*! @} */

/*! @name BT2_WKUP_MASK - Bluetooth 2 Wakeup Mask */
/*! @{ */

#define APU_BT2_WKUP_MASK_BCA_CLK_REQ_MASK_MASK  (0x1U)
#define APU_BT2_WKUP_MASK_BCA_CLK_REQ_MASK_SHIFT (0U)
/*! BCA_CLK_REQ_MASK - BCA Clock Request Mask */
#define APU_BT2_WKUP_MASK_BCA_CLK_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BCA_CLK_REQ_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BCA_CLK_REQ_MASK_MASK)

#define APU_BT2_WKUP_MASK_BT_CLK_REQ_MASK_MASK   (0x2U)
#define APU_BT2_WKUP_MASK_BT_CLK_REQ_MASK_SHIFT  (1U)
/*! BT_CLK_REQ_MASK - Bluetooth Clock Request Mask */
#define APU_BT2_WKUP_MASK_BT_CLK_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BT_CLK_REQ_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BT_CLK_REQ_MASK_MASK)

#define APU_BT2_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_MASK (0x4U)
#define APU_BT2_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_SHIFT (2U)
/*! BT_WB_ACTIVE_REQ_MASK - Bluetooth WB Active Request Mask */
#define APU_BT2_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BT_WB_ACTIVE_REQ_MASK_MASK)

#define APU_BT2_WKUP_MASK_BT_INTR_MASK_MASK      (0x8U)
#define APU_BT2_WKUP_MASK_BT_INTR_MASK_SHIFT     (3U)
/*! BT_INTR_MASK - Bluetooth Interrupt Mask */
#define APU_BT2_WKUP_MASK_BT_INTR_MASK(x)        (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BT_INTR_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BT_INTR_MASK_MASK)

#define APU_BT2_WKUP_MASK_BT_PLL_REQ_MASK_MASK   (0x10U)
#define APU_BT2_WKUP_MASK_BT_PLL_REQ_MASK_SHIFT  (4U)
/*! BT_PLL_REQ_MASK - Bluetooth PLL Request Mask */
#define APU_BT2_WKUP_MASK_BT_PLL_REQ_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BT_PLL_REQ_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BT_PLL_REQ_MASK_MASK)

#define APU_BT2_WKUP_MASK_BRF_PLL_REQ_MASK_MASK  (0x20U)
#define APU_BT2_WKUP_MASK_BRF_PLL_REQ_MASK_SHIFT (5U)
/*! BRF_PLL_REQ_MASK - BRF PLL Request Mask */
#define APU_BT2_WKUP_MASK_BRF_PLL_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BRF_PLL_REQ_MASK_SHIFT)) & APU_BT2_WKUP_MASK_BRF_PLL_REQ_MASK_MASK)

#define APU_BT2_WKUP_MASK_BT_HOST_MAP_MASK       (0xFFFF0000U)
#define APU_BT2_WKUP_MASK_BT_HOST_MAP_SHIFT      (16U)
/*! BT_HOST_MAP - Bluetooth Host Map */
#define APU_BT2_WKUP_MASK_BT_HOST_MAP(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT2_WKUP_MASK_BT_HOST_MAP_SHIFT)) & APU_BT2_WKUP_MASK_BT_HOST_MAP_MASK)
/*! @} */

/*! @name BT2_STATUS - Bluetooth 2 Status */
/*! @{ */

#define APU_BT2_STATUS_APU_BT_CLK_EN_MASK        (0x1U)
#define APU_BT2_STATUS_APU_BT_CLK_EN_SHIFT       (0U)
/*! APU_BT_CLK_EN - APU Bluetooth Clock Enable */
#define APU_BT2_STATUS_APU_BT_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_APU_BT_CLK_EN_SHIFT)) & APU_BT2_STATUS_APU_BT_CLK_EN_MASK)

#define APU_BT2_STATUS_APU_BRF_CLK_EN_MASK       (0x2U)
#define APU_BT2_STATUS_APU_BRF_CLK_EN_SHIFT      (1U)
/*! APU_BRF_CLK_EN - APU BRF Clock Enable */
#define APU_BT2_STATUS_APU_BRF_CLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_APU_BRF_CLK_EN_SHIFT)) & APU_BT2_STATUS_APU_BRF_CLK_EN_MASK)

#define APU_BT2_STATUS_BT_ACTIVE_SLPCK_MASK      (0x4U)
#define APU_BT2_STATUS_BT_ACTIVE_SLPCK_SHIFT     (2U)
/*! BT_ACTIVE_SLPCK - Bluetooth Active Sleep Clock */
#define APU_BT2_STATUS_BT_ACTIVE_SLPCK(x)        (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_ACTIVE_SLPCK_SHIFT)) & APU_BT2_STATUS_BT_ACTIVE_SLPCK_MASK)

#define APU_BT2_STATUS_BT_HOST_WKUP_MASK         (0x10U)
#define APU_BT2_STATUS_BT_HOST_WKUP_SHIFT        (4U)
/*! BT_HOST_WKUP - Bluetooth Host Wakeup */
#define APU_BT2_STATUS_BT_HOST_WKUP(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_HOST_WKUP_SHIFT)) & APU_BT2_STATUS_BT_HOST_WKUP_MASK)

#define APU_BT2_STATUS_BT_INTERRUPT_MASK         (0x20U)
#define APU_BT2_STATUS_BT_INTERRUPT_SHIFT        (5U)
/*! BT_INTERRUPT - Bluetooth Interrupt */
#define APU_BT2_STATUS_BT_INTERRUPT(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_INTERRUPT_SHIFT)) & APU_BT2_STATUS_BT_INTERRUPT_MASK)

#define APU_BT2_STATUS_BT_PLL_REQ_MASK           (0x40U)
#define APU_BT2_STATUS_BT_PLL_REQ_SHIFT          (6U)
/*! BT_PLL_REQ - Bluetooth PLL Request */
#define APU_BT2_STATUS_BT_PLL_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_PLL_REQ_SHIFT)) & APU_BT2_STATUS_BT_PLL_REQ_MASK)

#define APU_BT2_STATUS_BT_CLK_REQ_MASK           (0x80U)
#define APU_BT2_STATUS_BT_CLK_REQ_SHIFT          (7U)
/*! BT_CLK_REQ - Bluetooth Clock Request */
#define APU_BT2_STATUS_BT_CLK_REQ(x)             (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_CLK_REQ_SHIFT)) & APU_BT2_STATUS_BT_CLK_REQ_MASK)

#define APU_BT2_STATUS_BRF_PLL_REQ_MASK          (0x100U)
#define APU_BT2_STATUS_BRF_PLL_REQ_SHIFT         (8U)
/*! BRF_PLL_REQ - BRF PLL Request */
#define APU_BT2_STATUS_BRF_PLL_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BRF_PLL_REQ_SHIFT)) & APU_BT2_STATUS_BRF_PLL_REQ_MASK)

#define APU_BT2_STATUS_BT_WIDEBAND_ACTIVE_MASK   (0x200U)
#define APU_BT2_STATUS_BT_WIDEBAND_ACTIVE_SHIFT  (9U)
/*! BT_WIDEBAND_ACTIVE - Bluetooth Wideband Active */
#define APU_BT2_STATUS_BT_WIDEBAND_ACTIVE(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_BT_WIDEBAND_ACTIVE_SHIFT)) & APU_BT2_STATUS_BT_WIDEBAND_ACTIVE_MASK)

#define APU_BT2_STATUS_HOST_WKUP_IN_MASK         (0xFFFF0000U)
#define APU_BT2_STATUS_HOST_WKUP_IN_SHIFT        (16U)
/*! HOST_WKUP_IN - Host Wakeup In */
#define APU_BT2_STATUS_HOST_WKUP_IN(x)           (((uint32_t)(((uint32_t)(x)) << APU_BT2_STATUS_HOST_WKUP_IN_SHIFT)) & APU_BT2_STATUS_HOST_WKUP_IN_MASK)
/*! @} */

/*! @name BT2_CKEN_CTRL - Bluetooth 2 Clock Enable Control */
/*! @{ */

#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_PWR_MASK (0x1U)
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_PWR_SHIFT (0U)
/*! BT_CLK_EN_VAL_PWR - control value for bt_clk_en when power ready */
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_PWR(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_PWR_SHIFT)) & APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_PWR_MASK)

#define APU_BT2_CKEN_CTRL_BL_CLK_EN_SEL_PWR_MASK (0x2U)
#define APU_BT2_CKEN_CTRL_BL_CLK_EN_SEL_PWR_SHIFT (1U)
/*! BL_CLK_EN_SEL_PWR - selection for bt_clk_en when power ready */
#define APU_BT2_CKEN_CTRL_BL_CLK_EN_SEL_PWR(x)   (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BL_CLK_EN_SEL_PWR_SHIFT)) & APU_BT2_CKEN_CTRL_BL_CLK_EN_SEL_PWR_MASK)

#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_MASK (0x4U)
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_SHIFT (2U)
/*! BT_CLK_EN_VAL_XOSC - control value for bt_clk_en when XOSC ready */
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_XOSC(x)  (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_SHIFT)) & APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_XOSC_MASK)

#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_MASK (0x8U)
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_SHIFT (3U)
/*! BT_CLK_EN_SEL_XOSC - selection for bt_clk_en when XOSC ready */
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_XOSC(x)  (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_SHIFT)) & APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_XOSC_MASK)

#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_MASK     (0x10U)
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_SHIFT    (4U)
/*! BT_CLK_EN_VAL - control value for bt_clk_en when PLL ready */
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_SHIFT)) & APU_BT2_CKEN_CTRL_BT_CLK_EN_VAL_MASK)

#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_MASK     (0x20U)
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_SHIFT    (5U)
/*! BT_CLK_EN_SEL - selection for bt_clk_en when PLL ready */
#define APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL(x)       (((uint32_t)(((uint32_t)(x)) << APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_SHIFT)) & APU_BT2_CKEN_CTRL_BT_CLK_EN_SEL_MASK)
/*! @} */

/*! @name BT2_RESRC_CTRL - Bluetooth 2 RESRC Control */
/*! @{ */

#define APU_BT2_RESRC_CTRL_FW_PWR_REQ_MASK       (0x1U)
#define APU_BT2_RESRC_CTRL_FW_PWR_REQ_SHIFT      (0U)
/*! FW_PWR_REQ - override hw power request */
#define APU_BT2_RESRC_CTRL_FW_PWR_REQ(x)         (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_PWR_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_PWR_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_PWR_REQ_VAL_MASK   (0x2U)
#define APU_BT2_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT  (1U)
/*! FW_PWR_REQ_VAL - override value when apu_bt_resrc_ctrl[0] is set */
#define APU_BT2_RESRC_CTRL_FW_PWR_REQ_VAL(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_PWR_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_PWR_REQ_VAL_MASK)

#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ_MASK      (0x4U)
#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ_SHIFT     (2U)
/*! FW_XOSC_REQ - override hw xosc request */
#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_XOSC_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_XOSC_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK  (0x8U)
#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT (3U)
/*! FW_XOSC_REQ_VAL - override value when apu_bt_resrc_ctrl[2] is set */
#define APU_BT2_RESRC_CTRL_FW_XOSC_REQ_VAL(x)    (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_XOSC_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_XOSC_REQ_VAL_MASK)

#define APU_BT2_RESRC_CTRL_FW_XP_REQ_MASK        (0x10U)
#define APU_BT2_RESRC_CTRL_FW_XP_REQ_SHIFT       (4U)
/*! FW_XP_REQ - override hw xosc + pwr request */
#define APU_BT2_RESRC_CTRL_FW_XP_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_XP_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_XP_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_XP_REQ_VAL_MASK    (0x20U)
#define APU_BT2_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT   (5U)
/*! FW_XP_REQ_VAL - override value when apu_bt_resrc_ctrl[4] is set */
#define APU_BT2_RESRC_CTRL_FW_XP_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_XP_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_XP_REQ_VAL_MASK)

#define APU_BT2_RESRC_CTRL_FW_SB_REQ_MASK        (0x40U)
#define APU_BT2_RESRC_CTRL_FW_SB_REQ_SHIFT       (6U)
/*! FW_SB_REQ - override hw xosc + pwr + pll sb request */
#define APU_BT2_RESRC_CTRL_FW_SB_REQ(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_SB_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_SB_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_SB_REQ_VAL_MASK    (0x80U)
#define APU_BT2_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT   (7U)
/*! FW_SB_REQ_VAL - override value when apu_bt_resrc_ctrl[6] is set */
#define APU_BT2_RESRC_CTRL_FW_SB_REQ_VAL(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_SB_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_SB_REQ_VAL_MASK)

#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_MASK    (0x100U)
#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT   (8U)
/*! FW_IPWAKE_REQ - override hw xosc + pwr + pll ipwake request */
#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ(x)      (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK (0x200U)
#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT (9U)
/*! FW_IPWAKE_REQ_VAL - override value when apu_bt_resrc_ctrl[8] is set */
#define APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_VAL(x)  (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_IPWAKE_REQ_VAL_MASK)

#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_MASK   (0x400U)
#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT  (10U)
/*! FW_ALLWAKE_REQ - firmware override hw xosc + pwr + pll allwake request */
#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ(x)     (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_SHIFT)) & APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_MASK)

#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK (0x800U)
#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT (11U)
/*! FW_ALLWAKE_REQ_VAL - firmware override value when apu_bt_resrc_ctrl[10] is set */
#define APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_SHIFT)) & APU_BT2_RESRC_CTRL_FW_ALLWAKE_REQ_VAL_MASK)
/*! @} */

/*! @name BT2_DVFS_CTRL - Bluetooth 2 DVFS Control */
/*! @{ */

#define APU_BT2_DVFS_CTRL_BT_VOL_VAL_MASK        (0x7FU)
#define APU_BT2_DVFS_CTRL_BT_VOL_VAL_SHIFT       (0U)
/*! BT_VOL_VAL - Blueooth Vol Value */
#define APU_BT2_DVFS_CTRL_BT_VOL_VAL(x)          (((uint32_t)(((uint32_t)(x)) << APU_BT2_DVFS_CTRL_BT_VOL_VAL_SHIFT)) & APU_BT2_DVFS_CTRL_BT_VOL_VAL_MASK)
/*! @} */

/*! @name BLE2_CTRL - BLE 2 Control */
/*! @{ */

#define APU_BLE2_CTRL_BLE_SLP_RDY_MASK           (0x1U)
#define APU_BLE2_CTRL_BLE_SLP_RDY_SHIFT          (0U)
/*! BLE_SLP_RDY - BLE Sleep Ready */
#define APU_BLE2_CTRL_BLE_SLP_RDY(x)             (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BLE_SLP_RDY_SHIFT)) & APU_BLE2_CTRL_BLE_SLP_RDY_MASK)

#define APU_BLE2_CTRL_BLE_SLP_RDYMASK_MASK       (0x2U)
#define APU_BLE2_CTRL_BLE_SLP_RDYMASK_SHIFT      (1U)
/*! BLE_SLP_RDYMASK - BLE Sleep Ready Mask */
#define APU_BLE2_CTRL_BLE_SLP_RDYMASK(x)         (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BLE_SLP_RDYMASK_SHIFT)) & APU_BLE2_CTRL_BLE_SLP_RDYMASK_MASK)

#define APU_BLE2_CTRL_BLE_SLP_RDY_FW_MASK        (0x4U)
#define APU_BLE2_CTRL_BLE_SLP_RDY_FW_SHIFT       (2U)
/*! BLE_SLP_RDY_FW - BLE Sleep Ready FW */
#define APU_BLE2_CTRL_BLE_SLP_RDY_FW(x)          (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BLE_SLP_RDY_FW_SHIFT)) & APU_BLE2_CTRL_BLE_SLP_RDY_FW_MASK)

#define APU_BLE2_CTRL_BLE_MODE_EN_MASK           (0x8U)
#define APU_BLE2_CTRL_BLE_MODE_EN_SHIFT          (3U)
/*! BLE_MODE_EN - BLE Mode Enable */
#define APU_BLE2_CTRL_BLE_MODE_EN(x)             (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BLE_MODE_EN_SHIFT)) & APU_BLE2_CTRL_BLE_MODE_EN_MASK)

#define APU_BLE2_CTRL_BT_AES_NCO_MODE_MASK       (0x8000U)
#define APU_BLE2_CTRL_BT_AES_NCO_MODE_SHIFT      (15U)
/*! BT_AES_NCO_MODE - Bluetooth AES NCO Mode */
#define APU_BLE2_CTRL_BT_AES_NCO_MODE(x)         (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BT_AES_NCO_MODE_SHIFT)) & APU_BLE2_CTRL_BT_AES_NCO_MODE_MASK)

#define APU_BLE2_CTRL_BLE_XP_REQ_MASK            (0x80000000U)
#define APU_BLE2_CTRL_BLE_XP_REQ_SHIFT           (31U)
/*! BLE_XP_REQ - BLE XP Request */
#define APU_BLE2_CTRL_BLE_XP_REQ(x)              (((uint32_t)(((uint32_t)(x)) << APU_BLE2_CTRL_BLE_XP_REQ_SHIFT)) & APU_BLE2_CTRL_BLE_XP_REQ_MASK)
/*! @} */

/*! @name BLE2_WKUP_MASK - BLE 2 Wakeup Mask */
/*! @{ */

#define APU_BLE2_WKUP_MASK_BT_AES_CLK_REQ_MASK_MASK (0x8U)
#define APU_BLE2_WKUP_MASK_BT_AES_CLK_REQ_MASK_SHIFT (3U)
/*! BT_AES_CLK_REQ_MASK - Bluetooth AES Clock Request Mask */
#define APU_BLE2_WKUP_MASK_BT_AES_CLK_REQ_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_BLE2_WKUP_MASK_BT_AES_CLK_REQ_MASK_SHIFT)) & APU_BLE2_WKUP_MASK_BT_AES_CLK_REQ_MASK_MASK)

#define APU_BLE2_WKUP_MASK_LBC_XP_REQ_MASK_MASK  (0x10U)
#define APU_BLE2_WKUP_MASK_LBC_XP_REQ_MASK_SHIFT (4U)
/*! LBC_XP_REQ_MASK - LBC XP Request Mask */
#define APU_BLE2_WKUP_MASK_LBC_XP_REQ_MASK(x)    (((uint32_t)(((uint32_t)(x)) << APU_BLE2_WKUP_MASK_LBC_XP_REQ_MASK_SHIFT)) & APU_BLE2_WKUP_MASK_LBC_XP_REQ_MASK_MASK)
/*! @} */

/*! @name BLE2_STATUS - BLE 2 Status */
/*! @{ */

#define APU_BLE2_STATUS_APU_BT_AES_CLK_EN_MASK   (0x1U)
#define APU_BLE2_STATUS_APU_BT_AES_CLK_EN_SHIFT  (0U)
/*! APU_BT_AES_CLK_EN - APU Bluetooth AES Clock Enable */
#define APU_BLE2_STATUS_APU_BT_AES_CLK_EN(x)     (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_APU_BT_AES_CLK_EN_SHIFT)) & APU_BLE2_STATUS_APU_BT_AES_CLK_EN_MASK)

#define APU_BLE2_STATUS_BT_AES_CLK_SEL_MASK      (0x2U)
#define APU_BLE2_STATUS_BT_AES_CLK_SEL_SHIFT     (1U)
/*! BT_AES_CLK_SEL - Bluetooth AES Clock Select */
#define APU_BLE2_STATUS_BT_AES_CLK_SEL(x)        (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_BT_AES_CLK_SEL_SHIFT)) & APU_BLE2_STATUS_BT_AES_CLK_SEL_MASK)

#define APU_BLE2_STATUS_XOSC_STABLE_REFCK_MASK   (0x4U)
#define APU_BLE2_STATUS_XOSC_STABLE_REFCK_SHIFT  (2U)
/*! XOSC_STABLE_REFCK - XOSC Stable Ref Clock */
#define APU_BLE2_STATUS_XOSC_STABLE_REFCK(x)     (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_XOSC_STABLE_REFCK_SHIFT)) & APU_BLE2_STATUS_XOSC_STABLE_REFCK_MASK)

#define APU_BLE2_STATUS_T1_STABLE_MASK           (0x8U)
#define APU_BLE2_STATUS_T1_STABLE_SHIFT          (3U)
/*! T1_STABLE - T1 Delay */
#define APU_BLE2_STATUS_T1_STABLE(x)             (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_T1_STABLE_SHIFT)) & APU_BLE2_STATUS_T1_STABLE_MASK)

#define APU_BLE2_STATUS_BT_INTERRUPT_MASK        (0x10U)
#define APU_BLE2_STATUS_BT_INTERRUPT_SHIFT       (4U)
/*! BT_INTERRUPT - Bluetooth Interrupt */
#define APU_BLE2_STATUS_BT_INTERRUPT(x)          (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_BT_INTERRUPT_SHIFT)) & APU_BLE2_STATUS_BT_INTERRUPT_MASK)

#define APU_BLE2_STATUS_BLE_XP_REQ_MASK          (0x20U)
#define APU_BLE2_STATUS_BLE_XP_REQ_SHIFT         (5U)
/*! BLE_XP_REQ - BLE XP Request */
#define APU_BLE2_STATUS_BLE_XP_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_BLE_XP_REQ_SHIFT)) & APU_BLE2_STATUS_BLE_XP_REQ_MASK)

#define APU_BLE2_STATUS_BT_AES_NCO_MODE_MASK     (0x40U)
#define APU_BLE2_STATUS_BT_AES_NCO_MODE_SHIFT    (6U)
/*! BT_AES_NCO_MODE - Blueooth AES NCO Mode */
#define APU_BLE2_STATUS_BT_AES_NCO_MODE(x)       (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_BT_AES_NCO_MODE_SHIFT)) & APU_BLE2_STATUS_BT_AES_NCO_MODE_MASK)

#define APU_BLE2_STATUS_LBC_XP_REQ_MASK          (0x100U)
#define APU_BLE2_STATUS_LBC_XP_REQ_SHIFT         (8U)
/*! LBC_XP_REQ - LBC XP Request */
#define APU_BLE2_STATUS_LBC_XP_REQ(x)            (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_LBC_XP_REQ_SHIFT)) & APU_BLE2_STATUS_LBC_XP_REQ_MASK)

#define APU_BLE2_STATUS_BT_AES_CLK_REQ_MASK      (0x200U)
#define APU_BLE2_STATUS_BT_AES_CLK_REQ_SHIFT     (9U)
/*! BT_AES_CLK_REQ - Bluetooth AES Clock Request Mask */
#define APU_BLE2_STATUS_BT_AES_CLK_REQ(x)        (((uint32_t)(((uint32_t)(x)) << APU_BLE2_STATUS_BT_AES_CLK_REQ_SHIFT)) & APU_BLE2_STATUS_BT_AES_CLK_REQ_MASK)
/*! @} */

/*! @name LMU_IP_REVISION - LMU IP Revision */
/*! @{ */

#define APU_LMU_IP_REVISION_IP_REV_MASK          (0xFFFFU)
#define APU_LMU_IP_REVISION_IP_REV_SHIFT         (0U)
/*! IP_REV - IP Revision */
#define APU_LMU_IP_REVISION_IP_REV(x)            (((uint32_t)(((uint32_t)(x)) << APU_LMU_IP_REVISION_IP_REV_SHIFT)) & APU_LMU_IP_REVISION_IP_REV_MASK)
/*! @} */

/*! @name LMU_CPU1_STA_CFG - LMU CPU1 STA Configuration */
/*! @{ */

#define APU_LMU_CPU1_STA_CFG_BANK0_STA_OFF_EN_MASK (0x1U)
#define APU_LMU_CPU1_STA_CFG_BANK0_STA_OFF_EN_SHIFT (0U)
/*! BANK0_STA_OFF_EN - Bank0 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK0_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK0_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK0_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED15_MASK     (0x2U)
#define APU_LMU_CPU1_STA_CFG_RESERVED15_SHIFT    (1U)
/*! RESERVED15 - Reserved 15 */
#define APU_LMU_CPU1_STA_CFG_RESERVED15(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED15_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED15_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK1_STA_OFF_EN_MASK (0x4U)
#define APU_LMU_CPU1_STA_CFG_BANK1_STA_OFF_EN_SHIFT (2U)
/*! BANK1_STA_OFF_EN - Bank1 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK1_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK1_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK1_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED14_MASK     (0x8U)
#define APU_LMU_CPU1_STA_CFG_RESERVED14_SHIFT    (3U)
/*! RESERVED14 - Reserved 14 */
#define APU_LMU_CPU1_STA_CFG_RESERVED14(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED14_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED14_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK2_STA_OFF_EN_MASK (0x10U)
#define APU_LMU_CPU1_STA_CFG_BANK2_STA_OFF_EN_SHIFT (4U)
/*! BANK2_STA_OFF_EN - Bank2 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK2_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK2_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK2_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED13_MASK     (0x20U)
#define APU_LMU_CPU1_STA_CFG_RESERVED13_SHIFT    (5U)
/*! RESERVED13 - Reserved 13 */
#define APU_LMU_CPU1_STA_CFG_RESERVED13(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED13_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED13_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK3_STA_OFF_EN_MASK (0x40U)
#define APU_LMU_CPU1_STA_CFG_BANK3_STA_OFF_EN_SHIFT (6U)
/*! BANK3_STA_OFF_EN - Bank3 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK3_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK3_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK3_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED12_MASK     (0x80U)
#define APU_LMU_CPU1_STA_CFG_RESERVED12_SHIFT    (7U)
/*! RESERVED12 - Reserved 12 */
#define APU_LMU_CPU1_STA_CFG_RESERVED12(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED12_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED12_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK4_STA_OFF_EN_MASK (0x100U)
#define APU_LMU_CPU1_STA_CFG_BANK4_STA_OFF_EN_SHIFT (8U)
/*! BANK4_STA_OFF_EN - Bank4 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK4_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK4_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK4_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED11_MASK     (0x200U)
#define APU_LMU_CPU1_STA_CFG_RESERVED11_SHIFT    (9U)
/*! RESERVED11 - Reserved 11 */
#define APU_LMU_CPU1_STA_CFG_RESERVED11(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED11_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED11_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK5_STA_OFF_EN_MASK (0x400U)
#define APU_LMU_CPU1_STA_CFG_BANK5_STA_OFF_EN_SHIFT (10U)
/*! BANK5_STA_OFF_EN - Bank5 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK5_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK5_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK5_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED10_MASK     (0x800U)
#define APU_LMU_CPU1_STA_CFG_RESERVED10_SHIFT    (11U)
/*! RESERVED10 - Reserved 10 */
#define APU_LMU_CPU1_STA_CFG_RESERVED10(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED10_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED10_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK6_STA_OFF_EN_MASK (0x1000U)
#define APU_LMU_CPU1_STA_CFG_BANK6_STA_OFF_EN_SHIFT (12U)
/*! BANK6_STA_OFF_EN - Bank6 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK6_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK6_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK6_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED9_MASK      (0x2000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED9_SHIFT     (13U)
/*! RESERVED9 - Reserved 9 */
#define APU_LMU_CPU1_STA_CFG_RESERVED9(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED9_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED9_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK7_STA_OFF_EN_MASK (0x4000U)
#define APU_LMU_CPU1_STA_CFG_BANK7_STA_OFF_EN_SHIFT (14U)
/*! BANK7_STA_OFF_EN - Bank7 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK7_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK7_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK7_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED8_MASK      (0x8000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED8_SHIFT     (15U)
/*! RESERVED8 - Reserved 8 */
#define APU_LMU_CPU1_STA_CFG_RESERVED8(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED8_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED8_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK8_STA_OFF_EN_MASK (0x10000U)
#define APU_LMU_CPU1_STA_CFG_BANK8_STA_OFF_EN_SHIFT (16U)
/*! BANK8_STA_OFF_EN - Bank8 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK8_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK8_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK8_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED7_MASK      (0x20000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED7_SHIFT     (17U)
/*! RESERVED7 - Reserved 7 */
#define APU_LMU_CPU1_STA_CFG_RESERVED7(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED7_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED7_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK9_STA_OFF_EN_MASK (0x40000U)
#define APU_LMU_CPU1_STA_CFG_BANK9_STA_OFF_EN_SHIFT (18U)
/*! BANK9_STA_OFF_EN - Bank9 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK9_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK9_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK9_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED6_MASK      (0x80000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED6_SHIFT     (19U)
/*! RESERVED6 - Reserved 6 */
#define APU_LMU_CPU1_STA_CFG_RESERVED6(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED6_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED6_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK10_STA_OFF_EN_MASK (0x100000U)
#define APU_LMU_CPU1_STA_CFG_BANK10_STA_OFF_EN_SHIFT (20U)
/*! BANK10_STA_OFF_EN - Bank10 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK10_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK10_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK10_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED5_MASK      (0x200000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED5_SHIFT     (21U)
/*! RESERVED5 - Reserved 5 */
#define APU_LMU_CPU1_STA_CFG_RESERVED5(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED5_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED5_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK11_STA_OFF_EN_MASK (0x400000U)
#define APU_LMU_CPU1_STA_CFG_BANK11_STA_OFF_EN_SHIFT (22U)
/*! BANK11_STA_OFF_EN - Bank11 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK11_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK11_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK11_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED4_MASK      (0x800000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED4_SHIFT     (23U)
/*! RESERVED4 - Reserved 4 */
#define APU_LMU_CPU1_STA_CFG_RESERVED4(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED4_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED4_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK12_STA_OFF_EN_MASK (0x1000000U)
#define APU_LMU_CPU1_STA_CFG_BANK12_STA_OFF_EN_SHIFT (24U)
/*! BANK12_STA_OFF_EN - Bank12 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK12_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK12_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK12_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED3_MASK      (0x2000000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED3_SHIFT     (25U)
/*! RESERVED3 - Reserved 3 */
#define APU_LMU_CPU1_STA_CFG_RESERVED3(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED3_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED3_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK13_STA_OFF_EN_MASK (0x4000000U)
#define APU_LMU_CPU1_STA_CFG_BANK13_STA_OFF_EN_SHIFT (26U)
/*! BANK13_STA_OFF_EN - Bank13 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK13_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK13_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK13_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED2_MASK      (0x8000000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED2_SHIFT     (27U)
/*! RESERVED2 - Reserved 2 */
#define APU_LMU_CPU1_STA_CFG_RESERVED2(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED2_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED2_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK14_STA_OFF_EN_MASK (0x10000000U)
#define APU_LMU_CPU1_STA_CFG_BANK14_STA_OFF_EN_SHIFT (28U)
/*! BANK14_STA_OFF_EN - Bank14 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK14_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK14_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK14_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED1_MASK      (0x20000000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED1_SHIFT     (29U)
/*! RESERVED1 - Reserved 1 */
#define APU_LMU_CPU1_STA_CFG_RESERVED1(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED1_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED1_MASK)

#define APU_LMU_CPU1_STA_CFG_BANK15_STA_OFF_EN_MASK (0x40000000U)
#define APU_LMU_CPU1_STA_CFG_BANK15_STA_OFF_EN_SHIFT (30U)
/*! BANK15_STA_OFF_EN - Bank15 STA Off Enable */
#define APU_LMU_CPU1_STA_CFG_BANK15_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_BANK15_STA_OFF_EN_SHIFT)) & APU_LMU_CPU1_STA_CFG_BANK15_STA_OFF_EN_MASK)

#define APU_LMU_CPU1_STA_CFG_RESERVED0_MASK      (0x80000000U)
#define APU_LMU_CPU1_STA_CFG_RESERVED0_SHIFT     (31U)
/*! RESERVED0 - Reserved 0 */
#define APU_LMU_CPU1_STA_CFG_RESERVED0(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_CFG_RESERVED0_SHIFT)) & APU_LMU_CPU1_STA_CFG_RESERVED0_MASK)
/*! @} */

/*! @name LMU_CPU1_STA_STATUS1 - LMU CPU1 STA Status 1 */
/*! @{ */

#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK0_FSM_ST_REP_DONE - Bank0 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_SHIFT (1U)
/*! BANK0_FSM_ST - Bank0 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK0_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK1_FSM_ST_REP_DONE - Bank1FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_SHIFT (5U)
/*! BANK1_FSM_ST - Bank1 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK1_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK2_FSM_ST_REP_DONE - Bank2 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_SHIFT (9U)
/*! BANK2_FSM_ST - Bank2 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK2_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK3_FSM_ST_REP_DONE - Bank3 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_SHIFT (13U)
/*! BANK3_FSM_ST - Bank3 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK3_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK4_FSM_ST_REP_DONE - Bank4 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_SHIFT (17U)
/*! BANK4_FSM_ST - Bank4 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK4_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK5_FSM_ST_REP_DONE - Bank5 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_SHIFT (21U)
/*! BANK5_FSM_ST - Bank5 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK5_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK6_FSM_ST_REP_DONE - Bank6 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_SHIFT (25U)
/*! BANK6_FSM_ST - Bank6 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK6_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK7_FSM_ST_REP_DONE - Bank7 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_SHIFT (29U)
/*! BANK7_FSM_ST - Bank7 FSM St */
#define APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS1_BANK7_FSM_ST_MASK)
/*! @} */

/*! @name LMU_CPU1_STA_STATUS2 - LMU CPU1 STA Status 2 */
/*! @{ */

#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK8_FSM_ST_REP_DONE - Bank8 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_SHIFT (1U)
/*! BANK8_FSM_ST - Bank8 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK8_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK9_FSM_ST_REP_DONE - Bank9 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_SHIFT (5U)
/*! BANK9_FSM_ST - Bank9 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK9_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK10_FSM_ST_REP_DONE - Bank10 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_SHIFT (9U)
/*! BANK10_FSM_ST - Bank10 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK10_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK11_FSM_ST_REP_DONE - Bank11 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_SHIFT (13U)
/*! BANK11_FSM_ST - Bank11 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK11_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK12_FSM_ST_REP_DONE - Bank12 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_SHIFT (17U)
/*! BANK12_FSM_ST - Bank12 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK12_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK13_FSM_ST_REP_DONE - Bank13 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_SHIFT (21U)
/*! BANK13_FSM_ST - Bank13 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK13_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK14_FSM_ST_REP_DONE - Bank14 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_SHIFT (25U)
/*! BANK14_FSM_ST - Bank14 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK14_FSM_ST_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK15_FSM_ST_REP_DONE - Bank15 FSM St Rep Done */
#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_SHIFT (29U)
/*! BANK15_FSM_ST - Bank15 FSM St */
#define APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_SHIFT)) & APU_LMU_CPU1_STA_STATUS2_BANK15_FSM_ST_MASK)
/*! @} */

/*! @name LMU_CPU1_DYN_CTRL - LMU CPU1 Dynamic Control */
/*! @{ */

#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_MODE_EN_MASK (0x1U)
#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_MODE_EN_SHIFT (0U)
/*! BANK0_DYN_MODE_EN - Bank0 Dynamic Mode Enable */
#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_MODE_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_MODE_EN_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_MODE_EN_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_PD_MASK  (0x2U)
#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_PD_SHIFT (1U)
/*! BANK0_DYN_PD - Bank0 Dynamic Powerdown */
#define APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_PD(x)    (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_PD_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK0_DYN_PD_MASK)

#define APU_LMU_CPU1_DYN_CTRL_RESERVED3_MASK     (0xCU)
#define APU_LMU_CPU1_DYN_CTRL_RESERVED3_SHIFT    (2U)
/*! RESERVED3 - Reserved 3 */
#define APU_LMU_CPU1_DYN_CTRL_RESERVED3(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_RESERVED3_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_RESERVED3_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_MODE_EN_MASK (0x10U)
#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_MODE_EN_SHIFT (4U)
/*! BANK1_DYN_MODE_EN - Bank1 Dynamic Mode Enable */
#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_MODE_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_MODE_EN_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_MODE_EN_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_PD_MASK  (0x20U)
#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_PD_SHIFT (5U)
/*! BANK1_DYN_PD - Bank1 Dynamic Powerdown */
#define APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_PD(x)    (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_PD_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK1_DYN_PD_MASK)

#define APU_LMU_CPU1_DYN_CTRL_RESERVED2_MASK     (0xC0U)
#define APU_LMU_CPU1_DYN_CTRL_RESERVED2_SHIFT    (6U)
/*! RESERVED2 - Reserved 2 */
#define APU_LMU_CPU1_DYN_CTRL_RESERVED2(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_RESERVED2_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_RESERVED2_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_MODE_EN_MASK (0x100U)
#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_MODE_EN_SHIFT (8U)
/*! BANK2_DYN_MODE_EN - Bank2 Dynamic Mode Enable */
#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_MODE_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_MODE_EN_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_MODE_EN_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_PD_MASK  (0x200U)
#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_PD_SHIFT (9U)
/*! BANK2_DYN_PD - Bank2 Dynamic Powerdown */
#define APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_PD(x)    (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_PD_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK2_DYN_PD_MASK)

#define APU_LMU_CPU1_DYN_CTRL_RESERVED1_MASK     (0xC00U)
#define APU_LMU_CPU1_DYN_CTRL_RESERVED1_SHIFT    (10U)
/*! RESERVED1 - Reserved 1 */
#define APU_LMU_CPU1_DYN_CTRL_RESERVED1(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_RESERVED1_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_RESERVED1_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_MODE_EN_MASK (0x1000U)
#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_MODE_EN_SHIFT (12U)
/*! BANK3_DYN_MODE_EN - Bank3 Dynamic Mode Enable */
#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_MODE_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_MODE_EN_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_MODE_EN_MASK)

#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_PD_MASK  (0x2000U)
#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_PD_SHIFT (13U)
/*! BANK3_DYN_PD - Bank3 Dynamic Powerdown */
#define APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_PD(x)    (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_PD_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_BANK3_DYN_PD_MASK)

#define APU_LMU_CPU1_DYN_CTRL_RESERVED0_MASK     (0xFFFFC000U)
#define APU_LMU_CPU1_DYN_CTRL_RESERVED0_SHIFT    (14U)
/*! RESERVED0 - Reserved 0 */
#define APU_LMU_CPU1_DYN_CTRL_RESERVED0(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_CTRL_RESERVED0_SHIFT)) & APU_LMU_CPU1_DYN_CTRL_RESERVED0_MASK)
/*! @} */

/*! @name LMU_CPU1_DYN_STATUS - LMU CPU1 Dynamic Status */
/*! @{ */

#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_REQ_MASK (0x1U)
#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_REQ_SHIFT (0U)
/*! BANK0_DYN_REP_REQ - Bank0 Dynamic Rep Request */
#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_REQ_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_REQ_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_MASK (0x2U)
#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_SHIFT (1U)
/*! BANK0_DYN_REP - Bank0 Dynamic Rep */
#define APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK0_DYN_REP_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK0_FSM_ST_MASK (0xCU)
#define APU_LMU_CPU1_DYN_STATUS_BANK0_FSM_ST_SHIFT (2U)
/*! BANK0_FSM_ST - Bank0 FSM St */
#define APU_LMU_CPU1_DYN_STATUS_BANK0_FSM_ST(x)  (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK0_FSM_ST_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK0_FSM_ST_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_REQ_MASK (0x10U)
#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_REQ_SHIFT (4U)
/*! BANK1_DYN_REP_REQ - Bank1 Dynamic Rep Request */
#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_REQ_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_REQ_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_MASK (0x20U)
#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_SHIFT (5U)
/*! BANK1_DYN_REP - Bank1 Dynamic Rep */
#define APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK1_DYN_REP_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK1_FSM_ST_MASK (0xC0U)
#define APU_LMU_CPU1_DYN_STATUS_BANK1_FSM_ST_SHIFT (6U)
/*! BANK1_FSM_ST - Bank1 FSM St */
#define APU_LMU_CPU1_DYN_STATUS_BANK1_FSM_ST(x)  (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK1_FSM_ST_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK1_FSM_ST_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_REQ_MASK (0x100U)
#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_REQ_SHIFT (8U)
/*! BANK2_DYN_REP_REQ - Bank2 Dynamic Rep Request */
#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_REQ_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_REQ_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_MASK (0x200U)
#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_SHIFT (9U)
/*! BANK2_DYN_REP - Bank2 Dynamic Rep */
#define APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK2_DYN_REP_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK2_FSM_ST_MASK (0xC00U)
#define APU_LMU_CPU1_DYN_STATUS_BANK2_FSM_ST_SHIFT (10U)
/*! BANK2_FSM_ST - Bank2 FSM St */
#define APU_LMU_CPU1_DYN_STATUS_BANK2_FSM_ST(x)  (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK2_FSM_ST_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK2_FSM_ST_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_REQ_MASK (0x1000U)
#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_REQ_SHIFT (12U)
/*! BANK3_DYN_REP_REQ - Bank3 Dynamic Rep Request */
#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_REQ_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_REQ_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_MASK (0x2000U)
#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_SHIFT (13U)
/*! BANK3_DYN_REP - Bank3 Dynamic Rep */
#define APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK3_DYN_REP_MASK)

#define APU_LMU_CPU1_DYN_STATUS_BANK3_FSM_ST_MASK (0xC000U)
#define APU_LMU_CPU1_DYN_STATUS_BANK3_FSM_ST_SHIFT (14U)
/*! BANK3_FSM_ST - Bank3 FSM St */
#define APU_LMU_CPU1_DYN_STATUS_BANK3_FSM_ST(x)  (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU1_DYN_STATUS_BANK3_FSM_ST_SHIFT)) & APU_LMU_CPU1_DYN_STATUS_BANK3_FSM_ST_MASK)
/*! @} */

/*! @name LMU_CPU2_STA_CFG - LMU CPU2 STA Configuration */
/*! @{ */

#define APU_LMU_CPU2_STA_CFG_BANK0_STA_OFF_EN_MASK (0x1U)
#define APU_LMU_CPU2_STA_CFG_BANK0_STA_OFF_EN_SHIFT (0U)
/*! BANK0_STA_OFF_EN - Bank0 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK0_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK0_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK0_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED7_MASK      (0x2U)
#define APU_LMU_CPU2_STA_CFG_RESERVED7_SHIFT     (1U)
/*! RESERVED7 - Reserved 7 */
#define APU_LMU_CPU2_STA_CFG_RESERVED7(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED7_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED7_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK1_STA_OFF_EN_MASK (0x4U)
#define APU_LMU_CPU2_STA_CFG_BANK1_STA_OFF_EN_SHIFT (2U)
/*! BANK1_STA_OFF_EN - Bank1 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK1_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK1_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK1_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED6_MASK      (0x8U)
#define APU_LMU_CPU2_STA_CFG_RESERVED6_SHIFT     (3U)
/*! RESERVED6 - Reserved 6 */
#define APU_LMU_CPU2_STA_CFG_RESERVED6(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED6_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED6_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK2_STA_OFF_EN_MASK (0x10U)
#define APU_LMU_CPU2_STA_CFG_BANK2_STA_OFF_EN_SHIFT (4U)
/*! BANK2_STA_OFF_EN - Bank2 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK2_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK2_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK2_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED5_MASK      (0x20U)
#define APU_LMU_CPU2_STA_CFG_RESERVED5_SHIFT     (5U)
/*! RESERVED5 - Reserved 5 */
#define APU_LMU_CPU2_STA_CFG_RESERVED5(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED5_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED5_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK3_STA_OFF_EN_MASK (0x40U)
#define APU_LMU_CPU2_STA_CFG_BANK3_STA_OFF_EN_SHIFT (6U)
/*! BANK3_STA_OFF_EN - Bank3 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK3_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK3_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK3_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED4_MASK      (0x80U)
#define APU_LMU_CPU2_STA_CFG_RESERVED4_SHIFT     (7U)
/*! RESERVED4 - Reserved 4 */
#define APU_LMU_CPU2_STA_CFG_RESERVED4(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED4_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED4_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK4_STA_OFF_EN_MASK (0x100U)
#define APU_LMU_CPU2_STA_CFG_BANK4_STA_OFF_EN_SHIFT (8U)
/*! BANK4_STA_OFF_EN - Bank4 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK4_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK4_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK4_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED3_MASK      (0x200U)
#define APU_LMU_CPU2_STA_CFG_RESERVED3_SHIFT     (9U)
/*! RESERVED3 - Reserved 3 */
#define APU_LMU_CPU2_STA_CFG_RESERVED3(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED3_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED3_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK5_STA_OFF_EN_MASK (0x400U)
#define APU_LMU_CPU2_STA_CFG_BANK5_STA_OFF_EN_SHIFT (10U)
/*! BANK5_STA_OFF_EN - Bank5 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK5_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK5_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK5_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED2_MASK      (0x800U)
#define APU_LMU_CPU2_STA_CFG_RESERVED2_SHIFT     (11U)
/*! RESERVED2 - Reserved 2 */
#define APU_LMU_CPU2_STA_CFG_RESERVED2(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED2_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED2_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK6_STA_OFF_EN_MASK (0x1000U)
#define APU_LMU_CPU2_STA_CFG_BANK6_STA_OFF_EN_SHIFT (12U)
/*! BANK6_STA_OFF_EN - Bank6 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK6_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK6_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK6_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED1_MASK      (0x2000U)
#define APU_LMU_CPU2_STA_CFG_RESERVED1_SHIFT     (13U)
/*! RESERVED1 - Reserved 1 */
#define APU_LMU_CPU2_STA_CFG_RESERVED1(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED1_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED1_MASK)

#define APU_LMU_CPU2_STA_CFG_BANK7_STA_OFF_EN_MASK (0x4000U)
#define APU_LMU_CPU2_STA_CFG_BANK7_STA_OFF_EN_SHIFT (14U)
/*! BANK7_STA_OFF_EN - Bank7 STA Off Enable */
#define APU_LMU_CPU2_STA_CFG_BANK7_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_BANK7_STA_OFF_EN_SHIFT)) & APU_LMU_CPU2_STA_CFG_BANK7_STA_OFF_EN_MASK)

#define APU_LMU_CPU2_STA_CFG_RESERVED0_MASK      (0xFFFF8000U)
#define APU_LMU_CPU2_STA_CFG_RESERVED0_SHIFT     (15U)
/*! RESERVED0 - Reserved 0 */
#define APU_LMU_CPU2_STA_CFG_RESERVED0(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_CFG_RESERVED0_SHIFT)) & APU_LMU_CPU2_STA_CFG_RESERVED0_MASK)
/*! @} */

/*! @name LMU_CPU2_STA_STATUS1 - LMU CPU2 STA Status 1 */
/*! @{ */

#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK0_FSM_ST_REP_DONE - Bank0 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_SHIFT (1U)
/*! BANK0_FSM_ST - Bank0 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK0_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK1_FSM_ST_REP_DONE - Bank1 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_SHIFT (5U)
/*! BANK1_FSM_ST - Bank1 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK1_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK2_FSM_ST_REP_DONE - Bank2 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_SHIFT (9U)
/*! BANK2_FSM_ST - Bank2FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK2_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK3_FSM_ST_REP_DONE - Bank3 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_SHIFT (13U)
/*! BANK3_FSM_ST - Bank3 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK3_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK4_FSM_ST_REP_DONE - Bank4 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_SHIFT (17U)
/*! BANK4_FSM_ST - Bank4 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK4_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK5_FSM_ST_REP_DONE - Bank5 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_SHIFT (21U)
/*! BANK5_FSM_ST - Bank5 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK5_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK6_FSM_ST_REP_DONE - Bank6 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_SHIFT (25U)
/*! BANK6_FSM_ST - Bank6 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK6_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK7_FSM_ST_REP_DONE - Bank7 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_SHIFT (29U)
/*! BANK7_FSM_ST - Bank7 FSM St */
#define APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS1_BANK7_FSM_ST_MASK)
/*! @} */

/*! @name LMU_CPU3_STA_CFG - LMU CPU3 STA Configuration */
/*! @{ */

#define APU_LMU_CPU3_STA_CFG_BANK0_STA_OFF_EN_MASK (0x1U)
#define APU_LMU_CPU3_STA_CFG_BANK0_STA_OFF_EN_SHIFT (0U)
/*! BANK0_STA_OFF_EN - Bank0 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK0_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK0_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK0_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED7_MASK      (0x2U)
#define APU_LMU_CPU3_STA_CFG_RESERVED7_SHIFT     (1U)
/*! RESERVED7 - Reserved 7 */
#define APU_LMU_CPU3_STA_CFG_RESERVED7(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED7_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED7_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK1_STA_OFF_EN_MASK (0x4U)
#define APU_LMU_CPU3_STA_CFG_BANK1_STA_OFF_EN_SHIFT (2U)
/*! BANK1_STA_OFF_EN - Bank1 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK1_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK1_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK1_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED6_MASK      (0x8U)
#define APU_LMU_CPU3_STA_CFG_RESERVED6_SHIFT     (3U)
/*! RESERVED6 - Reserved 6 */
#define APU_LMU_CPU3_STA_CFG_RESERVED6(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED6_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED6_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK2_STA_OFF_EN_MASK (0x10U)
#define APU_LMU_CPU3_STA_CFG_BANK2_STA_OFF_EN_SHIFT (4U)
/*! BANK2_STA_OFF_EN - Bank2 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK2_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK2_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK2_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED5_MASK      (0x20U)
#define APU_LMU_CPU3_STA_CFG_RESERVED5_SHIFT     (5U)
/*! RESERVED5 - Reserved 5 */
#define APU_LMU_CPU3_STA_CFG_RESERVED5(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED5_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED5_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK3_STA_OFF_EN_MASK (0x40U)
#define APU_LMU_CPU3_STA_CFG_BANK3_STA_OFF_EN_SHIFT (6U)
/*! BANK3_STA_OFF_EN - Bank3 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK3_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK3_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK3_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED4_MASK      (0x80U)
#define APU_LMU_CPU3_STA_CFG_RESERVED4_SHIFT     (7U)
/*! RESERVED4 - Reserved 4 */
#define APU_LMU_CPU3_STA_CFG_RESERVED4(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED4_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED4_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK4_STA_OFF_EN_MASK (0x100U)
#define APU_LMU_CPU3_STA_CFG_BANK4_STA_OFF_EN_SHIFT (8U)
/*! BANK4_STA_OFF_EN - Bank4 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK4_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK4_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK4_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED3_MASK      (0x200U)
#define APU_LMU_CPU3_STA_CFG_RESERVED3_SHIFT     (9U)
/*! RESERVED3 - Reserved 3 */
#define APU_LMU_CPU3_STA_CFG_RESERVED3(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED3_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED3_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK5_STA_OFF_EN_MASK (0x400U)
#define APU_LMU_CPU3_STA_CFG_BANK5_STA_OFF_EN_SHIFT (10U)
/*! BANK5_STA_OFF_EN - Bank5 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK5_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK5_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK5_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED2_MASK      (0x800U)
#define APU_LMU_CPU3_STA_CFG_RESERVED2_SHIFT     (11U)
/*! RESERVED2 - Reserved 2 */
#define APU_LMU_CPU3_STA_CFG_RESERVED2(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED2_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED2_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK6_STA_OFF_EN_MASK (0x1000U)
#define APU_LMU_CPU3_STA_CFG_BANK6_STA_OFF_EN_SHIFT (12U)
/*! BANK6_STA_OFF_EN - Bank6 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK6_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK6_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK6_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED1_MASK      (0x2000U)
#define APU_LMU_CPU3_STA_CFG_RESERVED1_SHIFT     (13U)
/*! RESERVED1 - Reserved 1 */
#define APU_LMU_CPU3_STA_CFG_RESERVED1(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED1_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED1_MASK)

#define APU_LMU_CPU3_STA_CFG_BANK7_STA_OFF_EN_MASK (0x4000U)
#define APU_LMU_CPU3_STA_CFG_BANK7_STA_OFF_EN_SHIFT (14U)
/*! BANK7_STA_OFF_EN - Bank7 STA Off Enable */
#define APU_LMU_CPU3_STA_CFG_BANK7_STA_OFF_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_BANK7_STA_OFF_EN_SHIFT)) & APU_LMU_CPU3_STA_CFG_BANK7_STA_OFF_EN_MASK)

#define APU_LMU_CPU3_STA_CFG_RESERVED0_MASK      (0xFFFF8000U)
#define APU_LMU_CPU3_STA_CFG_RESERVED0_SHIFT     (15U)
/*! RESERVED0 - Reserved 0 */
#define APU_LMU_CPU3_STA_CFG_RESERVED0(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_CFG_RESERVED0_SHIFT)) & APU_LMU_CPU3_STA_CFG_RESERVED0_MASK)
/*! @} */

/*! @name LMU_CPU3_STA_STATUS1 - LMU CPU3 STA Status 1 */
/*! @{ */

#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK0_FSM_ST_REP_DONE - Bank0 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_SHIFT (1U)
/*! BANK0_FSM_ST - Bank0 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK0_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK1_FSM_ST_REP_DONE - Bank1 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_SHIFT (5U)
/*! BANK1_FSM_ST - Bank1 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK1_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK2_FSM_ST_REP_DONE - Bank2 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_SHIFT (9U)
/*! BANK2_FSM_ST - Bank2FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK2_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK3_FSM_ST_REP_DONE - Bank3 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_SHIFT (13U)
/*! BANK3_FSM_ST - Bank3 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK3_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK4_FSM_ST_REP_DONE - Bank4 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_SHIFT (17U)
/*! BANK4_FSM_ST - Bank4 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK4_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK5_FSM_ST_REP_DONE - Bank5 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_SHIFT (21U)
/*! BANK5_FSM_ST - Bank5 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK5_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK6_FSM_ST_REP_DONE - Bank6 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_SHIFT (25U)
/*! BANK6_FSM_ST - Bank6 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK6_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK7_FSM_ST_REP_DONE - Bank7 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_SHIFT (29U)
/*! BANK7_FSM_ST - Bank7 FSM St */
#define APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS1_BANK7_FSM_ST_MASK)
/*! @} */

/*! @name LMU_G2BIST_CTRL - LMU G2BIST Control */
/*! @{ */

#define APU_LMU_G2BIST_CTRL_LMU_G2BIST_MODE_MASK (0x1FU)
#define APU_LMU_G2BIST_CTRL_LMU_G2BIST_MODE_SHIFT (0U)
/*! LMU_G2BIST_MODE - 1 = fuse load mode, default mode when powered up. All repairable memories will be repaired by g2bist engine */
#define APU_LMU_G2BIST_CTRL_LMU_G2BIST_MODE(x)   (((uint32_t)(((uint32_t)(x)) << APU_LMU_G2BIST_CTRL_LMU_G2BIST_MODE_SHIFT)) & APU_LMU_G2BIST_CTRL_LMU_G2BIST_MODE_MASK)

#define APU_LMU_G2BIST_CTRL_DISABLE_STA_REPAIR_MASK (0x20000000U)
#define APU_LMU_G2BIST_CTRL_DISABLE_STA_REPAIR_SHIFT (29U)
/*! DISABLE_STA_REPAIR - 1= disable static memory repair */
#define APU_LMU_G2BIST_CTRL_DISABLE_STA_REPAIR(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_G2BIST_CTRL_DISABLE_STA_REPAIR_SHIFT)) & APU_LMU_G2BIST_CTRL_DISABLE_STA_REPAIR_MASK)

#define APU_LMU_G2BIST_CTRL_DISABLE_DYN_REPAIR_MASK (0x40000000U)
#define APU_LMU_G2BIST_CTRL_DISABLE_DYN_REPAIR_SHIFT (30U)
/*! DISABLE_DYN_REPAIR - 1= disable dynamic memory repair */
#define APU_LMU_G2BIST_CTRL_DISABLE_DYN_REPAIR(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_G2BIST_CTRL_DISABLE_DYN_REPAIR_SHIFT)) & APU_LMU_G2BIST_CTRL_DISABLE_DYN_REPAIR_MASK)
/*! @} */

/*! @name LMU_STATUS - LMU Status */
/*! @{ */

#define APU_LMU_STATUS_REPAIR_FSM_ST_MASK        (0xFU)
#define APU_LMU_STATUS_REPAIR_FSM_ST_SHIFT       (0U)
/*! REPAIR_FSM_ST - Repair FSM St */
#define APU_LMU_STATUS_REPAIR_FSM_ST(x)          (((uint32_t)(((uint32_t)(x)) << APU_LMU_STATUS_REPAIR_FSM_ST_SHIFT)) & APU_LMU_STATUS_REPAIR_FSM_ST_MASK)

#define APU_LMU_STATUS_NXT_ST_MASK               (0xF0U)
#define APU_LMU_STATUS_NXT_ST_SHIFT              (4U)
/*! NXT_ST - NXT St */
#define APU_LMU_STATUS_NXT_ST(x)                 (((uint32_t)(((uint32_t)(x)) << APU_LMU_STATUS_NXT_ST_SHIFT)) & APU_LMU_STATUS_NXT_ST_MASK)

#define APU_LMU_STATUS_LMU_G2BIST_MODE_MASK      (0xF00U)
#define APU_LMU_STATUS_LMU_G2BIST_MODE_SHIFT     (8U)
/*! LMU_G2BIST_MODE - LMU_g to BIST Mode */
#define APU_LMU_STATUS_LMU_G2BIST_MODE(x)        (((uint32_t)(((uint32_t)(x)) << APU_LMU_STATUS_LMU_G2BIST_MODE_SHIFT)) & APU_LMU_STATUS_LMU_G2BIST_MODE_MASK)

#define APU_LMU_STATUS_LMU_G2BIST_PWRDMN_RPR_REQ_MASK (0xFF000U)
#define APU_LMU_STATUS_LMU_G2BIST_PWRDMN_RPR_REQ_SHIFT (12U)
/*! LMU_G2BIST_PWRDMN_RPR_REQ - LMU_g to BIST Powerdown RPR Request */
#define APU_LMU_STATUS_LMU_G2BIST_PWRDMN_RPR_REQ(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_STATUS_LMU_G2BIST_PWRDMN_RPR_REQ_SHIFT)) & APU_LMU_STATUS_LMU_G2BIST_PWRDMN_RPR_REQ_MASK)

#define APU_LMU_STATUS_LMU_G2BIST_START_MASK     (0x100000U)
#define APU_LMU_STATUS_LMU_G2BIST_START_SHIFT    (20U)
/*! LMU_G2BIST_START - LMU_g to BIST Start */
#define APU_LMU_STATUS_LMU_G2BIST_START(x)       (((uint32_t)(((uint32_t)(x)) << APU_LMU_STATUS_LMU_G2BIST_START_SHIFT)) & APU_LMU_STATUS_LMU_G2BIST_START_MASK)
/*! @} */

/*! @name LMU_STA_CFG_MASK - LMU STA Configuration Mask */
/*! @{ */

#define APU_LMU_STA_CFG_MASK_STA_CFG_MASK_MASK   (0xFFU)
#define APU_LMU_STA_CFG_MASK_STA_CFG_MASK_SHIFT  (0U)
/*! STA_CFG_MASK - STA Configuration Mask */
#define APU_LMU_STA_CFG_MASK_STA_CFG_MASK(x)     (((uint32_t)(((uint32_t)(x)) << APU_LMU_STA_CFG_MASK_STA_CFG_MASK_SHIFT)) & APU_LMU_STA_CFG_MASK_STA_CFG_MASK_MASK)
/*! @} */

/*! @name LMU_CPU2_STA_STATUS2 - LMU CPU2 STA Status 2 */
/*! @{ */

#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK8_FSM_ST_REP_DONE - Bank8 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_SHIFT (1U)
/*! BANK8_FSM_ST - Bank8 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK8_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK9_FSM_ST_REP_DONE - Bank9 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_SHIFT (5U)
/*! BANK9_FSM_ST - Bank9 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK9_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK10_FSM_ST_REP_DONE - Bank10 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_SHIFT (9U)
/*! BANK10_FSM_ST - Bank10 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK10_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK11_FSM_ST_REP_DONE - Bank11 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_SHIFT (13U)
/*! BANK11_FSM_ST - Bank11 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK11_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK12_FSM_ST_REP_DONE - Bank12 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_SHIFT (17U)
/*! BANK12_FSM_ST - Bank12 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK12_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK13_FSM_ST_REP_DONE - Bank13 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_SHIFT (21U)
/*! BANK13_FSM_ST - Bank13 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK13_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK14_FSM_ST_REP_DONE - Bank14 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_SHIFT (25U)
/*! BANK14_FSM_ST - Bank14 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK14_FSM_ST_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK15_FSM_ST_REP_DONE - Bank15 FSM St Rep Done */
#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_SHIFT (29U)
/*! BANK15_FSM_ST - Bank15 FSM St */
#define APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_SHIFT)) & APU_LMU_CPU2_STA_STATUS2_BANK15_FSM_ST_MASK)
/*! @} */

/*! @name LMU_CPU3_STA_STATUS2 - LMU CPU3 STA Status 2 */
/*! @{ */

#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK (0x1U)
#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT (0U)
/*! BANK8_FSM_ST_REP_DONE - Bank8 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_MASK (0xEU)
#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_SHIFT (1U)
/*! BANK8_FSM_ST - Bank8 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK8_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK (0x10U)
#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT (4U)
/*! BANK9_FSM_ST_REP_DONE - Bank9 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_MASK (0xE0U)
#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_SHIFT (5U)
/*! BANK9_FSM_ST - Bank9 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK9_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK (0x100U)
#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT (8U)
/*! BANK10_FSM_ST_REP_DONE - Bank10 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_MASK (0xE00U)
#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_SHIFT (9U)
/*! BANK10_FSM_ST - Bank10 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK10_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK (0x1000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT (12U)
/*! BANK11_FSM_ST_REP_DONE - Bank11 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_MASK (0xE000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_SHIFT (13U)
/*! BANK11_FSM_ST - Bank11 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK11_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK (0x10000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT (16U)
/*! BANK12_FSM_ST_REP_DONE - Bank12 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_MASK (0xE0000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_SHIFT (17U)
/*! BANK12_FSM_ST - Bank12 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK12_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK (0x100000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT (20U)
/*! BANK13_FSM_ST_REP_DONE - Bank13 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_MASK (0xE00000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_SHIFT (21U)
/*! BANK13_FSM_ST - Bank13 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK13_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK (0x1000000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT (24U)
/*! BANK14_FSM_ST_REP_DONE - Bank14 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_MASK (0xE000000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_SHIFT (25U)
/*! BANK14_FSM_ST - Bank14 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK14_FSM_ST_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK (0x10000000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT (28U)
/*! BANK15_FSM_ST_REP_DONE - Bank15 FSM St Rep Done */
#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_REP_DONE(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_REP_DONE_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_REP_DONE_MASK)

#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_MASK (0xE0000000U)
#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_SHIFT (29U)
/*! BANK15_FSM_ST - Bank15 FSM St */
#define APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST(x) (((uint32_t)(((uint32_t)(x)) << APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_SHIFT)) & APU_LMU_CPU3_STA_STATUS2_BANK15_FSM_ST_MASK)
/*! @} */

/*! @name CPU3_HOST_WKUP_MASK - CPU3 Host Wakeup Mask */
/*! @{ */

#define APU_CPU3_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK (0xFFFFU)
#define APU_CPU3_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT (0U)
/*! HOST_WKUP_MASK - Host Wakeup Mask */
#define APU_CPU3_HOST_WKUP_MASK_HOST_WKUP_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_HOST_WKUP_MASK_HOST_WKUP_MASK_SHIFT)) & APU_CPU3_HOST_WKUP_MASK_HOST_WKUP_MASK_MASK)
/*! @} */

/*! @name CPU3_HOST_WKUP_POL - CPU3 Host Wakeup Polarity */
/*! @{ */

#define APU_CPU3_HOST_WKUP_POL_HOST_WKUP_POL_MASK (0xFFFFU)
#define APU_CPU3_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT (0U)
/*! HOST_WKUP_POL - Host Wakeup Polarity */
#define APU_CPU3_HOST_WKUP_POL_HOST_WKUP_POL(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU3_HOST_WKUP_POL_HOST_WKUP_POL_SHIFT)) & APU_CPU3_HOST_WKUP_POL_HOST_WKUP_POL_MASK)
/*! @} */

/*! @name CPU3_HOST_WKUP_CTRL - CPU3 Host Wakeup Control */
/*! @{ */

#define APU_CPU3_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK (0xFFFFU)
#define APU_CPU3_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT (0U)
/*! HOST_WKUP_CTRL - Host Wakeup Control */
#define APU_CPU3_HOST_WKUP_CTRL_HOST_WKUP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_HOST_WKUP_CTRL_HOST_WKUP_CTRL_SHIFT)) & APU_CPU3_HOST_WKUP_CTRL_HOST_WKUP_CTRL_MASK)

#define APU_CPU3_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK (0xFFFF0000U)
#define APU_CPU3_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT (16U)
/*! HOST_INTR_MASK - Host Interrupt Mask */
#define APU_CPU3_HOST_WKUP_CTRL_HOST_INTR_MASK(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_HOST_WKUP_CTRL_HOST_INTR_MASK_SHIFT)) & APU_CPU3_HOST_WKUP_CTRL_HOST_INTR_MASK_MASK)
/*! @} */

/*! @name CPU3_CTRL - CPU3 Control */
/*! @{ */

#define APU_CPU3_CTRL_CPU3_MSG_RDY_MASK_MASK     (0x1U)
#define APU_CPU3_CTRL_CPU3_MSG_RDY_MASK_SHIFT    (0U)
/*! CPU3_MSG_RDY_MASK - CPU3 Message Ready Mask */
#define APU_CPU3_CTRL_CPU3_MSG_RDY_MASK(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_CPU3_MSG_RDY_MASK_SHIFT)) & APU_CPU3_CTRL_CPU3_MSG_RDY_MASK_MASK)

#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_VAL_MASK (0x4U)
#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_VAL_SHIFT (2U)
/*! CPU3_CP15_SLP_BYPASS_VAL - CPU3 CP15 Sleep Bypass Value */
#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_VAL_SHIFT)) & APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_VAL_MASK)

#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_EN_MASK (0x8U)
#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_EN_SHIFT (3U)
/*! CPU3_CP15_SLP_BYPASS_EN - CPU3 CP15 Sleep Bypass Enable */
#define APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_EN_SHIFT)) & APU_CPU3_CTRL_CPU3_CP15_SLP_BYPASS_EN_MASK)

#define APU_CPU3_CTRL_SOC_USE_REF_ONLY_MASK      (0x10U)
#define APU_CPU3_CTRL_SOC_USE_REF_ONLY_SHIFT     (4U)
/*! SOC_USE_REF_ONLY - SoC Use Ref Only */
#define APU_CPU3_CTRL_SOC_USE_REF_ONLY(x)        (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_SOC_USE_REF_ONLY_SHIFT)) & APU_CPU3_CTRL_SOC_USE_REF_ONLY_MASK)

#define APU_CPU3_CTRL_GENERIC_TIMER_EN2_MASK     (0x20U)
#define APU_CPU3_CTRL_GENERIC_TIMER_EN2_SHIFT    (5U)
/*! GENERIC_TIMER_EN2 - Generic Timer Enable 2 */
#define APU_CPU3_CTRL_GENERIC_TIMER_EN2(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_GENERIC_TIMER_EN2_SHIFT)) & APU_CPU3_CTRL_GENERIC_TIMER_EN2_MASK)

#define APU_CPU3_CTRL_APU_SUBSYS3_HOST_MASK      (0x70000U)
#define APU_CPU3_CTRL_APU_SUBSYS3_HOST_SHIFT     (16U)
/*! APU_SUBSYS3_HOST - APU Subsystem 2 Host */
#define APU_CPU3_CTRL_APU_SUBSYS3_HOST(x)        (((uint32_t)(((uint32_t)(x)) << APU_CPU3_CTRL_APU_SUBSYS3_HOST_SHIFT)) & APU_CPU3_CTRL_APU_SUBSYS3_HOST_MASK)
/*! @} */

/*! @name CPU3_DVFS_CTRL - CPU3 DVFS Control */
/*! @{ */

#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_INDEX_MASK (0xFU)
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_INDEX_SHIFT (0U)
/*! CPU3_ACTIVE_INDEX - CPU3 Active Index */
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_INDEX(x)  (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_INDEX_MASK (0xF0U)
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_INDEX_SHIFT (4U)
/*! CPU3_INACTIVE_INDEX - CPU3 Inactive Index */
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_SYS_INDEX_MASK (0xF00U)
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_SYS_INDEX_SHIFT (8U)
/*! CPU3_ACTIVE_SYS_INDEX - CPU3 Active System Index */
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_SYS_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_SYS_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_SYS_INDEX_MASK (0xF000U)
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_SYS_INDEX_SHIFT (12U)
/*! CPU3_INACTIVE_SYS_INDEX - CPU3 Inactive System Index */
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_SYS_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_SYS_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_SYS_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_AHB1_INDEX_MASK (0xF0000U)
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_AHB1_INDEX_SHIFT (16U)
/*! CPU3_ACTIVE_AHB1_INDEX - CPU3 Active AHB1 Index */
#define APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_ACTIVE_AHB1_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_AHB1_INDEX_MASK (0xF00000U)
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_AHB1_INDEX_SHIFT (20U)
/*! CPU3_INACTIVE_AHB1_INDEX - CPU3 Inactive AHB1 Index */
#define APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_AHB1_INDEX(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_AHB1_INDEX_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_INACTIVE_AHB1_INDEX_MASK)

#define APU_CPU3_DVFS_CTRL_CPU3_AHB1_INDEX_SEL_METHOD_MASK (0x1000000U)
#define APU_CPU3_DVFS_CTRL_CPU3_AHB1_INDEX_SEL_METHOD_SHIFT (24U)
/*! CPU3_AHB1_INDEX_SEL_METHOD - CPU3 AHB1 index select method */
#define APU_CPU3_DVFS_CTRL_CPU3_AHB1_INDEX_SEL_METHOD(x) (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_CPU3_AHB1_INDEX_SEL_METHOD_SHIFT)) & APU_CPU3_DVFS_CTRL_CPU3_AHB1_INDEX_SEL_METHOD_MASK)

#define APU_CPU3_DVFS_CTRL_UART_VOL_VAL_MASK     (0xFE000000U)
#define APU_CPU3_DVFS_CTRL_UART_VOL_VAL_SHIFT    (25U)
/*! UART_VOL_VAL - UART Vol Value */
#define APU_CPU3_DVFS_CTRL_UART_VOL_VAL(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_DVFS_CTRL_UART_VOL_VAL_SHIFT)) & APU_CPU3_DVFS_CTRL_UART_VOL_VAL_MASK)
/*! @} */

/*! @name CPU3_FREQ_REG1 - CPU3 Frequency 1 */
/*! @{ */

#define APU_CPU3_FREQ_REG1_CPU3_FREQ_REG1_MASK   (0xFFFFFFFFU)
#define APU_CPU3_FREQ_REG1_CPU3_FREQ_REG1_SHIFT  (0U)
/*! CPU3_FREQ_REG1 - CPU3 Frequency 1 */
#define APU_CPU3_FREQ_REG1_CPU3_FREQ_REG1(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU3_FREQ_REG1_CPU3_FREQ_REG1_SHIFT)) & APU_CPU3_FREQ_REG1_CPU3_FREQ_REG1_MASK)
/*! @} */

/*! @name CPU3_FREQ_REG2 - CPU3 Frequency 2 */
/*! @{ */

#define APU_CPU3_FREQ_REG2_CPU3_FREQ_REG2_MASK   (0xFFFFFFFFU)
#define APU_CPU3_FREQ_REG2_CPU3_FREQ_REG2_SHIFT  (0U)
/*! CPU3_FREQ_REG2 - CPU3Frequency 2 */
#define APU_CPU3_FREQ_REG2_CPU3_FREQ_REG2(x)     (((uint32_t)(((uint32_t)(x)) << APU_CPU3_FREQ_REG2_CPU3_FREQ_REG2_SHIFT)) & APU_CPU3_FREQ_REG2_CPU3_FREQ_REG2_MASK)
/*! @} */

/*! @name CPU3_PLL_EN_REG - CPU3 PLL Enable */
/*! @{ */

#define APU_CPU3_PLL_EN_REG_CPU3_PLL_EN_REG_MASK (0xFFFFFFFFU)
#define APU_CPU3_PLL_EN_REG_CPU3_PLL_EN_REG_SHIFT (0U)
/*! CPU3_PLL_EN_REG - CPU3 PLL Enable */
#define APU_CPU3_PLL_EN_REG_CPU3_PLL_EN_REG(x)   (((uint32_t)(((uint32_t)(x)) << APU_CPU3_PLL_EN_REG_CPU3_PLL_EN_REG_SHIFT)) & APU_CPU3_PLL_EN_REG_CPU3_PLL_EN_REG_MASK)
/*! @} */

/*! @name CPU3_VOL_REG1 - CPU3 Voltage 1 */
/*! @{ */

#define APU_CPU3_VOL_REG1_CPU3_VOL_REG1_MASK     (0xFFFFFFFFU)
#define APU_CPU3_VOL_REG1_CPU3_VOL_REG1_SHIFT    (0U)
/*! CPU3_VOL_REG1 - CPU3 Voltage 1 */
#define APU_CPU3_VOL_REG1_CPU3_VOL_REG1(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_VOL_REG1_CPU3_VOL_REG1_SHIFT)) & APU_CPU3_VOL_REG1_CPU3_VOL_REG1_MASK)
/*! @} */

/*! @name CPU3_VOL_REG2 - CPU3 Voltage 2 */
/*! @{ */

#define APU_CPU3_VOL_REG2_CPU3_VOL_REG2_MASK     (0xFFFFFFFFU)
#define APU_CPU3_VOL_REG2_CPU3_VOL_REG2_SHIFT    (0U)
/*! CPU3_VOL_REG2 - CPU3 Voltage 2 */
#define APU_CPU3_VOL_REG2_CPU3_VOL_REG2(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_VOL_REG2_CPU3_VOL_REG2_SHIFT)) & APU_CPU3_VOL_REG2_CPU3_VOL_REG2_MASK)
/*! @} */

/*! @name CPU3_VOL_REG3 - CPU3 Voltage 3 */
/*! @{ */

#define APU_CPU3_VOL_REG3_CPU3_VOL_REG3_MASK     (0xFFFFFFFFU)
#define APU_CPU3_VOL_REG3_CPU3_VOL_REG3_SHIFT    (0U)
/*! CPU3_VOL_REG3 - CPU3 Voltage 3 */
#define APU_CPU3_VOL_REG3_CPU3_VOL_REG3(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_VOL_REG3_CPU3_VOL_REG3_SHIFT)) & APU_CPU3_VOL_REG3_CPU3_VOL_REG3_MASK)
/*! @} */

/*! @name CPU3_VOL_REG4 - CPU3 Voltage 4 */
/*! @{ */

#define APU_CPU3_VOL_REG4_CPU3_VOL_REG4_MASK     (0xFFFFFFFFU)
#define APU_CPU3_VOL_REG4_CPU3_VOL_REG4_SHIFT    (0U)
/*! CPU3_VOL_REG4 - CPU3 Voltage 4 */
#define APU_CPU3_VOL_REG4_CPU3_VOL_REG4(x)       (((uint32_t)(((uint32_t)(x)) << APU_CPU3_VOL_REG4_CPU3_VOL_REG4_SHIFT)) & APU_CPU3_VOL_REG4_CPU3_VOL_REG4_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group APU_Register_Masks */


/*!
 * @}
 */ /* end of group APU_Peripheral_Access_Layer */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
  #else
    #pragma pop
  #endif
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_access_layer */


#endif  /* APU_H_ */

