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
**         CMSIS Peripheral Access Layer for PMU
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
 * @file PMU.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for PMU
 *
 * CMSIS Peripheral Access Layer for PMU
 */

#if !defined(PMU_H_)
#define PMU_H_                                   /**< Symbol preventing repeated inclusion */

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
   -- PMU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMU_Peripheral_Access_Layer PMU Peripheral Access Layer
 * @{
 */

/** PMU - Register Layout Typedef */
typedef struct {
  __IO uint32_t PWR_MODE;                          /**< Power mode control register, offset: 0x0 */
  __I  uint32_t PWR_MODE_STATUS;                   /**< Power mode status register, offset: 0x4 */
  __IO uint32_t SYS_RST_EN;                        /**< sys reset enable resister, offset: 0x8 */
  __I  uint32_t SYS_RST_STATUS;                    /**< Reset status Register, offset: 0xC */
  __IO uint32_t SYS_RST_CLR;                       /**< sys reset clear resister, offset: 0x10 */
  __IO uint32_t WAKEUP_LEVEL;                      /**< Wakeup Level Register, offset: 0x14 */
  __IO uint32_t WAKEUP_MASK;                       /**< Wakeup Mask Interrupt Register, offset: 0x18 */
  __I  uint32_t WAKEUP_STATUS;                     /**< Wakeup status register, offset: 0x1C */
  __IO uint32_t WAKE_SRC_CLR;                      /**< Wake up source clear register, offset: 0x20 */
  __IO uint32_t WL_BLE_WAKEUP_DONE;                /**< Wake up done register, offset: 0x24 */
  __IO uint32_t CAU_SLP_CTRL;                      /**< CAU sleep clock control register, offset: 0x28 */
  __I  uint32_t SOC_CIU_RDY;                       /**< soc_ciu_rdy register, offset: 0x2C */
  __IO uint32_t CAPT_PULSE;                        /**< pulse in register, offset: 0x30 */
  __IO uint32_t CAPT_PULSE_BASE_VAL;               /**< capt_pulse_base_val, offset: 0x34 */
  __I  uint32_t CAPT_PULSE_VAL;                    /**< capt_pulse_val, offset: 0x38 */
  __IO uint32_t XTAL32K_CTRL;                      /**< XTAL32k Control Register, offset: 0x3C */
       uint8_t RESERVED_0[4];
  __IO uint32_t PMIP_BUCK_LVL;                     /**< PMIP BUCK LEVEL, offset: 0x44 */
  __IO uint32_t PMIP_BUCK_CTRL;                    /**< PMIP BUCK ctrl, offset: 0x48 */
  __IO uint32_t PMIP_LDO_LVL;                      /**< PMIP LDO level ctrl, offset: 0x4C */
  __IO uint32_t PMIP_RST;                          /**< PMIP reset request register, offset: 0x50 */
       uint8_t RESERVED_1[8];
  __IO uint32_t BOD;                               /**< BOD register, offset: 0x5C */
  __IO uint32_t MEM_CFG;                           /**< mem configuration register, offset: 0x60 */
  __IO uint32_t RESET_DISABLE;                     /**< reset disable register, offset: 0x64 */
  __IO uint32_t WLAN_CTRL;                         /**< WLAN Control Register, offset: 0x68 */
  __IO uint32_t BLE_CTRL;                          /**< BLEControl Register, offset: 0x6C */
  __IO uint32_t CLK_AON;                           /**< Always on Domain Clock select, offset: 0x70 */
  __IO uint32_t SOC_MEM_PDWN;                      /**< soc mem pdwn register, offset: 0x74 */
       uint8_t RESERVED_2[8];
  __IO uint32_t AON_PAD_OUT_CTRL;                  /**< aon pad out control, offset: 0x80 */
  __IO uint32_t WAKEUP_PM2_MASK0;                  /**< Wakeup PM2 state Mask Interrupt Register, offset: 0x84 */
  __IO uint32_t WAKEUP_PM2_MASK1;                  /**< Wakeup PM2 state Mask Interrupt Register, offset: 0x88 */
       uint8_t RESERVED_3[4];
  __IO uint32_t WAKEUP_PM2_MASK3;                  /**< Wakeup PM2 state Mask Interrupt Register, offset: 0x90 */
  __I  uint32_t WAKEUP_PM2_STATUS0;                /**< Wakeup PM2 status Register, offset: 0x94 */
  __I  uint32_t WAKEUP_PM2_STATUS1;                /**< Wakeup PM2 status Register, offset: 0x98 */
       uint8_t RESERVED_4[4];
  __I  uint32_t WAKEUP_PM2_STATUS3;                /**< WAKEUP_PM2_STATUS3, offset: 0xA0 */
  __IO uint32_t WAKEUP_PM2_SRC_CLR0;               /**< Wakeup PM2 source clear Register, offset: 0xA4 */
  __IO uint32_t WAKEUP_PM2_SRC_CLR1;               /**< Wakeup PM2 source clear Register, offset: 0xA8 */
       uint8_t RESERVED_5[4];
  __IO uint32_t WAKEUP_PM2_SRC_CLR3;               /**< Wakeup PM2 source clear Register, offset: 0xB0 */
       uint8_t RESERVED_6[4];
  __IO uint32_t SW_CTRL_WL;                        /**< WL part-SW Control register bypass HW output, offset: 0xB8 */
  __IO uint32_t SW_CTRL_BLE;                       /**< BLE part-SW Control register bypass HW output, offset: 0xBC */
       uint8_t RESERVED_7[76];
  __IO uint32_t PSW18_OTP;                         /**< PSW18 OTP psw control signal, offset: 0x10C */
  __IO uint32_t TIME_OUT_CTRL;                     /**< tieme out control signal, offset: 0x110 */
  __IO uint32_t TIME_OUT_CFG_VALUE;                /**< tieme out configure value, offset: 0x114 */
       uint8_t RESERVED_8[12];
  __IO uint32_t RESERVE_REG0;                      /**< reserve R/W regs, offset: 0x124 */
  __I  uint32_t RESERVE_REG1;                      /**< reserve Read only regs, offset: 0x128 */
} PMU_Type;

/* ----------------------------------------------------------------------------
   -- PMU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMU_Register_Masks PMU Register Masks
 * @{
 */

/*! @name PWR_MODE - Power mode control register */
/*! @{ */

#define PMU_PWR_MODE_PWR_MODE_MASK               (0x3U)
#define PMU_PWR_MODE_PWR_MODE_SHIFT              (0U)
/*! PWR_MODE - Power mode switch
 *  0b00..PM0 or PM1
 *  0b01..PM2
 *  0b10..PM3
 *  0b11..PM4
 */
#define PMU_PWR_MODE_PWR_MODE(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_PWR_MODE_PWR_MODE_SHIFT)) & PMU_PWR_MODE_PWR_MODE_MASK)
/*! @} */

/*! @name PWR_MODE_STATUS - Power mode status register */
/*! @{ */

#define PMU_PWR_MODE_STATUS_PWR_MODE_STATUS_MASK (0x3U)
#define PMU_PWR_MODE_STATUS_PWR_MODE_STATUS_SHIFT (0U)
/*! PWR_MODE_STATUS - Power mode status
 *  0b00..PM0 or PM1
 *  0b01..
 *  0b10..
 *  0b11..
 */
#define PMU_PWR_MODE_STATUS_PWR_MODE_STATUS(x)   (((uint32_t)(((uint32_t)(x)) << PMU_PWR_MODE_STATUS_PWR_MODE_STATUS_SHIFT)) & PMU_PWR_MODE_STATUS_PWR_MODE_STATUS_MASK)
/*! @} */

/*! @name SYS_RST_EN - sys reset enable resister */
/*! @{ */

#define PMU_SYS_RST_EN_CM33_SYSRESETREQ_EN_MASK  (0x1U)
#define PMU_SYS_RST_EN_CM33_SYSRESETREQ_EN_SHIFT (0U)
/*! CM33_SYSRESETREQ_EN - cm33_sysresetreq reset enable */
#define PMU_SYS_RST_EN_CM33_SYSRESETREQ_EN(x)    (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_CM33_SYSRESETREQ_EN_SHIFT)) & PMU_SYS_RST_EN_CM33_SYSRESETREQ_EN_MASK)

#define PMU_SYS_RST_EN_CM33_LOCKUP_EN_MASK       (0x2U)
#define PMU_SYS_RST_EN_CM33_LOCKUP_EN_SHIFT      (1U)
/*! CM33_LOCKUP_EN - cm33_lockup reset enable */
#define PMU_SYS_RST_EN_CM33_LOCKUP_EN(x)         (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_CM33_LOCKUP_EN_SHIFT)) & PMU_SYS_RST_EN_CM33_LOCKUP_EN_MASK)

#define PMU_SYS_RST_EN_WDT_EN_MASK               (0x4U)
#define PMU_SYS_RST_EN_WDT_EN_SHIFT              (2U)
/*! WDT_EN - wdt rst enable */
#define PMU_SYS_RST_EN_WDT_EN(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_WDT_EN_SHIFT)) & PMU_SYS_RST_EN_WDT_EN_MASK)

#define PMU_SYS_RST_EN_AP_SYSRESETREQ_EN_MASK    (0x8U)
#define PMU_SYS_RST_EN_AP_SYSRESETREQ_EN_SHIFT   (3U)
/*! AP_SYSRESETREQ_EN - ap_sysresetreq rst enable */
#define PMU_SYS_RST_EN_AP_SYSRESETREQ_EN(x)      (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_AP_SYSRESETREQ_EN_SHIFT)) & PMU_SYS_RST_EN_AP_SYSRESETREQ_EN_MASK)

#define PMU_SYS_RST_EN_CODE_WDT_EN_MASK          (0x10U)
#define PMU_SYS_RST_EN_CODE_WDT_EN_SHIFT         (4U)
/*! CODE_WDT_EN - code_wdt rst enable */
#define PMU_SYS_RST_EN_CODE_WDT_EN(x)            (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_CODE_WDT_EN_SHIFT)) & PMU_SYS_RST_EN_CODE_WDT_EN_MASK)

#define PMU_SYS_RST_EN_ITRC_EN_MASK              (0x20U)
#define PMU_SYS_RST_EN_ITRC_EN_SHIFT             (5U)
/*! ITRC_EN - itrc_chip rst enable */
#define PMU_SYS_RST_EN_ITRC_EN(x)                (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_EN_ITRC_EN_SHIFT)) & PMU_SYS_RST_EN_ITRC_EN_MASK)
/*! @} */

/*! @name SYS_RST_STATUS - Reset status Register */
/*! @{ */

#define PMU_SYS_RST_STATUS_CM33_SYSRESETREQ_MASK (0x1U)
#define PMU_SYS_RST_STATUS_CM33_SYSRESETREQ_SHIFT (0U)
/*! CM33_SYSRESETREQ - CM4 System software reset request
 *  0b0..reset cause is not system software reset request
 *  0b1..reset cause is system software reset request
 */
#define PMU_SYS_RST_STATUS_CM33_SYSRESETREQ(x)   (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_CM33_SYSRESETREQ_SHIFT)) & PMU_SYS_RST_STATUS_CM33_SYSRESETREQ_MASK)

#define PMU_SYS_RST_STATUS_CM33_LOCKUP_MASK      (0x2U)
#define PMU_SYS_RST_STATUS_CM33_LOCKUP_SHIFT     (1U)
/*! CM33_LOCKUP - CM4 Lockup
 *  0b0..reset cause is not lockup
 *  0b1..reset cause is lockup
 */
#define PMU_SYS_RST_STATUS_CM33_LOCKUP(x)        (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_CM33_LOCKUP_SHIFT)) & PMU_SYS_RST_STATUS_CM33_LOCKUP_MASK)

#define PMU_SYS_RST_STATUS_WDT_RST_MASK          (0x4U)
#define PMU_SYS_RST_STATUS_WDT_RST_SHIFT         (2U)
/*! WDT_RST - WDT Reset
 *  0b0..reset cause is not watchdog timer
 *  0b1..reset cause is watchdog timer
 */
#define PMU_SYS_RST_STATUS_WDT_RST(x)            (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_WDT_RST_SHIFT)) & PMU_SYS_RST_STATUS_WDT_RST_MASK)

#define PMU_SYS_RST_STATUS_AP_SYSRESETREQ_MASK   (0x8U)
#define PMU_SYS_RST_STATUS_AP_SYSRESETREQ_SHIFT  (3U)
/*! AP_SYSRESETREQ - Debug mailbox Reset
 *  0b0..reset cause is not ap_sysresetreq
 *  0b1..reset cause is ap_sysresetreq
 */
#define PMU_SYS_RST_STATUS_AP_SYSRESETREQ(x)     (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_AP_SYSRESETREQ_SHIFT)) & PMU_SYS_RST_STATUS_AP_SYSRESETREQ_MASK)

#define PMU_SYS_RST_STATUS_CODE_WDT_RST_MASK     (0x10U)
#define PMU_SYS_RST_STATUS_CODE_WDT_RST_SHIFT    (4U)
/*! CODE_WDT_RST - CODE_WDT Reset
 *  0b0..reset cause is not code watchdog timer
 *  0b1..reset cause is code watchdog timer
 */
#define PMU_SYS_RST_STATUS_CODE_WDT_RST(x)       (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_CODE_WDT_RST_SHIFT)) & PMU_SYS_RST_STATUS_CODE_WDT_RST_MASK)

#define PMU_SYS_RST_STATUS_ITRC_CHIP_RST_MASK    (0x20U)
#define PMU_SYS_RST_STATUS_ITRC_CHIP_RST_SHIFT   (5U)
/*! ITRC_CHIP_RST - ITRC_CHIP Reset
 *  0b0..reset cause is not itrc chip reset
 *  0b1..reset cause is itrc chip reset
 */
#define PMU_SYS_RST_STATUS_ITRC_CHIP_RST(x)      (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_ITRC_CHIP_RST_SHIFT)) & PMU_SYS_RST_STATUS_ITRC_CHIP_RST_MASK)

#define PMU_SYS_RST_STATUS_SW_RESETB_SCANTEST_MASK (0x40U)
#define PMU_SYS_RST_STATUS_SW_RESETB_SCANTEST_SHIFT (6U)
/*! SW_RESETB_SCANTEST - sw_resetb_scantest Reset
 *  0b0..reset cause is not sw_resetb_scantest reset
 *  0b1..reset cause is sw_resetb_scantest reset
 */
#define PMU_SYS_RST_STATUS_SW_RESETB_SCANTEST(x) (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_STATUS_SW_RESETB_SCANTEST_SHIFT)) & PMU_SYS_RST_STATUS_SW_RESETB_SCANTEST_MASK)
/*! @} */

/*! @name SYS_RST_CLR - sys reset clear resister */
/*! @{ */

#define PMU_SYS_RST_CLR_CM33_SYSRESETREQ_CLR_MASK (0x1U)
#define PMU_SYS_RST_CLR_CM33_SYSRESETREQ_CLR_SHIFT (0U)
/*! CM33_SYSRESETREQ_CLR - cm33_sysresetreq reset clear */
#define PMU_SYS_RST_CLR_CM33_SYSRESETREQ_CLR(x)  (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_CM33_SYSRESETREQ_CLR_SHIFT)) & PMU_SYS_RST_CLR_CM33_SYSRESETREQ_CLR_MASK)

#define PMU_SYS_RST_CLR_CM33_LOCKUP_CLR_MASK     (0x2U)
#define PMU_SYS_RST_CLR_CM33_LOCKUP_CLR_SHIFT    (1U)
/*! CM33_LOCKUP_CLR - cm33_lockup reset clear */
#define PMU_SYS_RST_CLR_CM33_LOCKUP_CLR(x)       (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_CM33_LOCKUP_CLR_SHIFT)) & PMU_SYS_RST_CLR_CM33_LOCKUP_CLR_MASK)

#define PMU_SYS_RST_CLR_WDT_CLR_MASK             (0x4U)
#define PMU_SYS_RST_CLR_WDT_CLR_SHIFT            (2U)
/*! WDT_CLR - wdt rst clear */
#define PMU_SYS_RST_CLR_WDT_CLR(x)               (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_WDT_CLR_SHIFT)) & PMU_SYS_RST_CLR_WDT_CLR_MASK)

#define PMU_SYS_RST_CLR_AP_SYSRESETREQ_CLR_MASK  (0x8U)
#define PMU_SYS_RST_CLR_AP_SYSRESETREQ_CLR_SHIFT (3U)
/*! AP_SYSRESETREQ_CLR - ap_sysresetreq rst clear */
#define PMU_SYS_RST_CLR_AP_SYSRESETREQ_CLR(x)    (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_AP_SYSRESETREQ_CLR_SHIFT)) & PMU_SYS_RST_CLR_AP_SYSRESETREQ_CLR_MASK)

#define PMU_SYS_RST_CLR_CODE_WDT_CLR_MASK        (0x10U)
#define PMU_SYS_RST_CLR_CODE_WDT_CLR_SHIFT       (4U)
/*! CODE_WDT_CLR - code_wdt rst clear */
#define PMU_SYS_RST_CLR_CODE_WDT_CLR(x)          (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_CODE_WDT_CLR_SHIFT)) & PMU_SYS_RST_CLR_CODE_WDT_CLR_MASK)

#define PMU_SYS_RST_CLR_ITRC_CLR_MASK            (0x20U)
#define PMU_SYS_RST_CLR_ITRC_CLR_SHIFT           (5U)
/*! ITRC_CLR - itrc chip rst clear */
#define PMU_SYS_RST_CLR_ITRC_CLR(x)              (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_ITRC_CLR_SHIFT)) & PMU_SYS_RST_CLR_ITRC_CLR_MASK)

#define PMU_SYS_RST_CLR_SW_RESETB_SCANTEST_CLR_MASK (0x40U)
#define PMU_SYS_RST_CLR_SW_RESETB_SCANTEST_CLR_SHIFT (6U)
/*! SW_RESETB_SCANTEST_CLR - sw_resetb_scantest rst clear */
#define PMU_SYS_RST_CLR_SW_RESETB_SCANTEST_CLR(x) (((uint32_t)(((uint32_t)(x)) << PMU_SYS_RST_CLR_SW_RESETB_SCANTEST_CLR_SHIFT)) & PMU_SYS_RST_CLR_SW_RESETB_SCANTEST_CLR_MASK)
/*! @} */

/*! @name WAKEUP_LEVEL - Wakeup Level Register */
/*! @{ */

#define PMU_WAKEUP_LEVEL_PIN0_MASK               (0x1U)
#define PMU_WAKEUP_LEVEL_PIN0_SHIFT              (0U)
/*! PIN0 - 0 = connect to gound wake up
 *  0b1..connect to VDDO wake up
 */
#define PMU_WAKEUP_LEVEL_PIN0(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_LEVEL_PIN0_SHIFT)) & PMU_WAKEUP_LEVEL_PIN0_MASK)

#define PMU_WAKEUP_LEVEL_PIN1_MASK               (0x2U)
#define PMU_WAKEUP_LEVEL_PIN1_SHIFT              (1U)
/*! PIN1 - 0 = connect to gound wake up
 *  0b1..connect to VDDO wake up
 */
#define PMU_WAKEUP_LEVEL_PIN1(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_LEVEL_PIN1_SHIFT)) & PMU_WAKEUP_LEVEL_PIN1_MASK)
/*! @} */

/*! @name WAKEUP_MASK - Wakeup Mask Interrupt Register */
/*! @{ */

#define PMU_WAKEUP_MASK_PIN0_MASK_MASK           (0x1U)
#define PMU_WAKEUP_MASK_PIN0_MASK_SHIFT          (0U)
/*! PIN0_MASK - Pin0 Wakeup Mask
 *  0b0..mask pin0 wakeup interrupt
 *  0b1..unmask pin0 wakeup interrupt
 */
#define PMU_WAKEUP_MASK_PIN0_MASK(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_PIN0_MASK_SHIFT)) & PMU_WAKEUP_MASK_PIN0_MASK_MASK)

#define PMU_WAKEUP_MASK_PIN1_MASK_MASK           (0x2U)
#define PMU_WAKEUP_MASK_PIN1_MASK_SHIFT          (1U)
/*! PIN1_MASK - Pin1 Wakeup Mask
 *  0b0..mask pin1 wakeup interrupt
 *  0b1..unmask pin1 wakeup interrupt
 */
#define PMU_WAKEUP_MASK_PIN1_MASK(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_PIN1_MASK_SHIFT)) & PMU_WAKEUP_MASK_PIN1_MASK_MASK)

#define PMU_WAKEUP_MASK_RTC_MASK_MASK            (0x4U)
#define PMU_WAKEUP_MASK_RTC_MASK_SHIFT           (2U)
/*! RTC_MASK - RTC Wakeup Mask
 *  0b0..mask RTC wakeup interrupt
 *  0b1..unmask RTC wakeup interrupt
 */
#define PMU_WAKEUP_MASK_RTC_MASK(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_RTC_MASK_SHIFT)) & PMU_WAKEUP_MASK_RTC_MASK_MASK)

#define PMU_WAKEUP_MASK_CAPT_MASK_MASK           (0x10U)
#define PMU_WAKEUP_MASK_CAPT_MASK_SHIFT          (4U)
/*! CAPT_MASK - capture pulse Wakeup Mask
 *  0b0..mask capt wakeup interrupt
 *  0b1..unmask capt wakeup interrupt
 */
#define PMU_WAKEUP_MASK_CAPT_MASK(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_CAPT_MASK_SHIFT)) & PMU_WAKEUP_MASK_CAPT_MASK_MASK)

#define PMU_WAKEUP_MASK_WL_MASK_MASK             (0x60U)
#define PMU_WAKEUP_MASK_WL_MASK_SHIFT            (5U)
/*! WL_MASK - WLAN Wakeup Mask
 *  0b00..mask WLAN wakeup interrupt
 *  0b01..unmask WLAN wakeup interrupt
 */
#define PMU_WAKEUP_MASK_WL_MASK(x)               (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_WL_MASK_SHIFT)) & PMU_WAKEUP_MASK_WL_MASK_MASK)

#define PMU_WAKEUP_MASK_BLE_MASK_MASK            (0x180U)
#define PMU_WAKEUP_MASK_BLE_MASK_SHIFT           (7U)
/*! BLE_MASK - BLE Wakeup Mask
 *  0b00..mask BLE wakeup interrupt
 *  0b01..unmask BLE wakeup interrupt
 */
#define PMU_WAKEUP_MASK_BLE_MASK(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_BLE_MASK_SHIFT)) & PMU_WAKEUP_MASK_BLE_MASK_MASK)

#define PMU_WAKEUP_MASK_BOD1_MASK_MASK           (0x200U)
#define PMU_WAKEUP_MASK_BOD1_MASK_SHIFT          (9U)
/*! BOD1_MASK - bod1 Wakeup Mask
 *  0b0..mask bod1 wakeup interrupt
 *  0b1..unmask bod1 wakeup interrupt
 */
#define PMU_WAKEUP_MASK_BOD1_MASK(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_MASK_BOD1_MASK_SHIFT)) & PMU_WAKEUP_MASK_BOD1_MASK_MASK)
/*! @} */

/*! @name WAKEUP_STATUS - Wakeup status register */
/*! @{ */

#define PMU_WAKEUP_STATUS_PIN0_MASK              (0x1U)
#define PMU_WAKEUP_STATUS_PIN0_SHIFT             (0U)
/*! PIN0 - External Pin0 wakeup status */
#define PMU_WAKEUP_STATUS_PIN0(x)                (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_PIN0_SHIFT)) & PMU_WAKEUP_STATUS_PIN0_MASK)

#define PMU_WAKEUP_STATUS_PIN1_MASK              (0x2U)
#define PMU_WAKEUP_STATUS_PIN1_SHIFT             (1U)
/*! PIN1 - External Pin1 wakeup status */
#define PMU_WAKEUP_STATUS_PIN1(x)                (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_PIN1_SHIFT)) & PMU_WAKEUP_STATUS_PIN1_MASK)

#define PMU_WAKEUP_STATUS_RTC_MASK               (0x4U)
#define PMU_WAKEUP_STATUS_RTC_SHIFT              (2U)
/*! RTC - RTC wakeup status */
#define PMU_WAKEUP_STATUS_RTC(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_RTC_SHIFT)) & PMU_WAKEUP_STATUS_RTC_MASK)

#define PMU_WAKEUP_STATUS_CAPT_MASK              (0x10U)
#define PMU_WAKEUP_STATUS_CAPT_SHIFT             (4U)
/*! CAPT - capt interrupt wakeup status */
#define PMU_WAKEUP_STATUS_CAPT(x)                (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_CAPT_SHIFT)) & PMU_WAKEUP_STATUS_CAPT_MASK)

#define PMU_WAKEUP_STATUS_WL_MASK                (0x60U)
#define PMU_WAKEUP_STATUS_WL_SHIFT               (5U)
/*! WL - WL interrupt wakeup status */
#define PMU_WAKEUP_STATUS_WL(x)                  (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_WL_SHIFT)) & PMU_WAKEUP_STATUS_WL_MASK)

#define PMU_WAKEUP_STATUS_BLE_MASK               (0x180U)
#define PMU_WAKEUP_STATUS_BLE_SHIFT              (7U)
/*! BLE - BLE interrupt wakeup status */
#define PMU_WAKEUP_STATUS_BLE(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_BLE_SHIFT)) & PMU_WAKEUP_STATUS_BLE_MASK)

#define PMU_WAKEUP_STATUS_BOD1_MASK              (0x200U)
#define PMU_WAKEUP_STATUS_BOD1_SHIFT             (9U)
/*! BOD1 - bod1 wakeup status */
#define PMU_WAKEUP_STATUS_BOD1(x)                (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_STATUS_BOD1_SHIFT)) & PMU_WAKEUP_STATUS_BOD1_MASK)
/*! @} */

/*! @name WAKE_SRC_CLR - Wake up source clear register */
/*! @{ */

#define PMU_WAKE_SRC_CLR_PIN0_CLR_MASK           (0x1U)
#define PMU_WAKE_SRC_CLR_PIN0_CLR_SHIFT          (0U)
/*! PIN0_CLR - Clear Pin0 interrupt request */
#define PMU_WAKE_SRC_CLR_PIN0_CLR(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_PIN0_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_PIN0_CLR_MASK)

#define PMU_WAKE_SRC_CLR_PIN1_CLR_MASK           (0x2U)
#define PMU_WAKE_SRC_CLR_PIN1_CLR_SHIFT          (1U)
/*! PIN1_CLR - Clear Pin1 interrupt request */
#define PMU_WAKE_SRC_CLR_PIN1_CLR(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_PIN1_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_PIN1_CLR_MASK)

#define PMU_WAKE_SRC_CLR_RTC_CLR_MASK            (0x4U)
#define PMU_WAKE_SRC_CLR_RTC_CLR_SHIFT           (2U)
/*! RTC_CLR - Clear RTC interrupt request */
#define PMU_WAKE_SRC_CLR_RTC_CLR(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_RTC_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_RTC_CLR_MASK)

#define PMU_WAKE_SRC_CLR_CAPT_CLR_MASK           (0x10U)
#define PMU_WAKE_SRC_CLR_CAPT_CLR_SHIFT          (4U)
/*! CAPT_CLR - Clear capture interrupt request */
#define PMU_WAKE_SRC_CLR_CAPT_CLR(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_CAPT_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_CAPT_CLR_MASK)

#define PMU_WAKE_SRC_CLR_WL_CLR_MASK             (0x60U)
#define PMU_WAKE_SRC_CLR_WL_CLR_SHIFT            (5U)
/*! WL_CLR - Clear WL interrupt request */
#define PMU_WAKE_SRC_CLR_WL_CLR(x)               (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_WL_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_WL_CLR_MASK)

#define PMU_WAKE_SRC_CLR_BLE_CLR_MASK            (0x180U)
#define PMU_WAKE_SRC_CLR_BLE_CLR_SHIFT           (7U)
/*! BLE_CLR - Clear BLE interrupt request */
#define PMU_WAKE_SRC_CLR_BLE_CLR(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_BLE_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_BLE_CLR_MASK)

#define PMU_WAKE_SRC_CLR_BOD1_CLR_MASK           (0x200U)
#define PMU_WAKE_SRC_CLR_BOD1_CLR_SHIFT          (9U)
/*! BOD1_CLR - Clear bod1 interrupt request */
#define PMU_WAKE_SRC_CLR_BOD1_CLR(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKE_SRC_CLR_BOD1_CLR_SHIFT)) & PMU_WAKE_SRC_CLR_BOD1_CLR_MASK)
/*! @} */

/*! @name WL_BLE_WAKEUP_DONE - Wake up done register */
/*! @{ */

#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT0_MASK (0x20U)
#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT0_SHIFT (5U)
/*! WL_DONE_BIT0 - WL wake up MCI done bit0 signal */
#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT0(x)   (((uint32_t)(((uint32_t)(x)) << PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT0_SHIFT)) & PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT0_MASK)

#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT1_MASK (0x40U)
#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT1_SHIFT (6U)
/*! WL_DONE_BIT1 - WL wake up MCI done bit1 signal */
#define PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT1(x)   (((uint32_t)(((uint32_t)(x)) << PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT1_SHIFT)) & PMU_WL_BLE_WAKEUP_DONE_WL_DONE_BIT1_MASK)

#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT0_MASK (0x80U)
#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT0_SHIFT (7U)
/*! BLE_DONE_BIT0 - BLE wake up MCI done signal */
#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT0(x)  (((uint32_t)(((uint32_t)(x)) << PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT0_SHIFT)) & PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT0_MASK)

#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT1_MASK (0x100U)
#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT1_SHIFT (8U)
/*! BLE_DONE_BIT1 - BLE wake up MCI done signal */
#define PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT1(x)  (((uint32_t)(((uint32_t)(x)) << PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT1_SHIFT)) & PMU_WL_BLE_WAKEUP_DONE_BLE_DONE_BIT1_MASK)
/*! @} */

/*! @name CAU_SLP_CTRL - CAU sleep clock control register */
/*! @{ */

#define PMU_CAU_SLP_CTRL_SOC_SLP_RDY_MASK        (0x2U)
#define PMU_CAU_SLP_CTRL_SOC_SLP_RDY_SHIFT       (1U)
/*! SOC_SLP_RDY - CAU_SOC_SLP_REF_GEN_CLK is ready */
#define PMU_CAU_SLP_CTRL_SOC_SLP_RDY(x)          (((uint32_t)(((uint32_t)(x)) << PMU_CAU_SLP_CTRL_SOC_SLP_RDY_SHIFT)) & PMU_CAU_SLP_CTRL_SOC_SLP_RDY_MASK)

#define PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_MASK     (0x4U)
#define PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_SHIFT    (2U)
/*! CAU_SOC_SLP_CG - gate cau_soc_slp_ref_gen_clk */
#define PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG(x)       (((uint32_t)(((uint32_t)(x)) << PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_SHIFT)) & PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_MASK)
/*! @} */

/*! @name SOC_CIU_RDY - soc_ciu_rdy register */
/*! @{ */

#define PMU_SOC_CIU_RDY_VAL_MASK                 (0x4U)
#define PMU_SOC_CIU_RDY_VAL_SHIFT                (2U)
/*! VAL - indicate soc IO strap finish, boot rom can read strap value */
#define PMU_SOC_CIU_RDY_VAL(x)                   (((uint32_t)(((uint32_t)(x)) << PMU_SOC_CIU_RDY_VAL_SHIFT)) & PMU_SOC_CIU_RDY_VAL_MASK)
/*! @} */

/*! @name CAPT_PULSE - pulse in register */
/*! @{ */

#define PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN_MASK (0x1U)
#define PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN_SHIFT (0U)
/*! CAPTURE_SLOW_PULSE_CNT_EN - enable signal */
#define PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN(x) (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN_SHIFT)) & PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN_MASK)

#define PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN_MASK (0x2U)
#define PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN_SHIFT (1U)
/*! CAPTURE_FAST_PULSE_CNT_EN - enable signal */
#define PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN(x) (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN_SHIFT)) & PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN_MASK)

#define PMU_CAPT_PULSE_IC_EDGE_CLK_CNT_MASK      (0xCU)
#define PMU_CAPT_PULSE_IC_EDGE_CLK_CNT_SHIFT     (2U)
/*! IC_EDGE_CLK_CNT - when capture_slow_pulse_cnt_en=1, indicate the trigger condition of counter,
 *    0: rising edge 1: falling edge ; others: both
 */
#define PMU_CAPT_PULSE_IC_EDGE_CLK_CNT(x)        (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_IC_EDGE_CLK_CNT_SHIFT)) & PMU_CAPT_PULSE_IC_EDGE_CLK_CNT_MASK)

#define PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT_MASK     (0x70U)
#define PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT_SHIFT    (4U)
/*! IC_WIDTH_CLK_CNT - Input Capture Filter Width only used when capture_slow_pulse_cnt_en=1 */
#define PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT(x)       (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT_SHIFT)) & PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT_MASK)

#define PMU_CAPT_PULSE_IRQ_CLR_MASK              (0x80U)
#define PMU_CAPT_PULSE_IRQ_CLR_SHIFT             (7U)
/*! IRQ_CLR - clear the interrupt and wakeup */
#define PMU_CAPT_PULSE_IRQ_CLR(x)                (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_IRQ_CLR_SHIFT)) & PMU_CAPT_PULSE_IRQ_CLR_MASK)

#define PMU_CAPT_PULSE_IRQ_STATUS_MASK           (0x100U)
#define PMU_CAPT_PULSE_IRQ_STATUS_SHIFT          (8U)
/*! IRQ_STATUS - interrupt status */
#define PMU_CAPT_PULSE_IRQ_STATUS(x)             (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_IRQ_STATUS_SHIFT)) & PMU_CAPT_PULSE_IRQ_STATUS_MASK)

#define PMU_CAPT_PULSE_IRQ_MSK_MASK              (0x200U)
#define PMU_CAPT_PULSE_IRQ_MSK_SHIFT             (9U)
/*! IRQ_MSK - only mask the interrupt */
#define PMU_CAPT_PULSE_IRQ_MSK(x)                (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_IRQ_MSK_SHIFT)) & PMU_CAPT_PULSE_IRQ_MSK_MASK)

#define PMU_CAPT_PULSE_CLK_SEL_MASK              (0x400U)
#define PMU_CAPT_PULSE_CLK_SEL_SHIFT             (10U)
/*! CLK_SEL - 0 choose 32k for slow capture,1 choose 3.84/4M for fast capture as functional clock */
#define PMU_CAPT_PULSE_CLK_SEL(x)                (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_CLK_SEL_SHIFT)) & PMU_CAPT_PULSE_CLK_SEL_MASK)
/*! @} */

/*! @name CAPT_PULSE_BASE_VAL - capt_pulse_base_val */
/*! @{ */

#define PMU_CAPT_PULSE_BASE_VAL_CAPTURE_CNT_BASE_VAL_MASK (0xFFFFFFFFU)
#define PMU_CAPT_PULSE_BASE_VAL_CAPTURE_CNT_BASE_VAL_SHIFT (0U)
/*! CAPTURE_CNT_BASE_VAL - the counter reaches this register value, interrupt will be generated */
#define PMU_CAPT_PULSE_BASE_VAL_CAPTURE_CNT_BASE_VAL(x) (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_BASE_VAL_CAPTURE_CNT_BASE_VAL_SHIFT)) & PMU_CAPT_PULSE_BASE_VAL_CAPTURE_CNT_BASE_VAL_MASK)
/*! @} */

/*! @name CAPT_PULSE_VAL - capt_pulse_val */
/*! @{ */

#define PMU_CAPT_PULSE_VAL_CAPTURE_CNT_VAL_MASK  (0xFFFFFFFFU)
#define PMU_CAPT_PULSE_VAL_CAPTURE_CNT_VAL_SHIFT (0U)
/*! CAPTURE_CNT_VAL - counter pulse value */
#define PMU_CAPT_PULSE_VAL_CAPTURE_CNT_VAL(x)    (((uint32_t)(((uint32_t)(x)) << PMU_CAPT_PULSE_VAL_CAPTURE_CNT_VAL_SHIFT)) & PMU_CAPT_PULSE_VAL_CAPTURE_CNT_VAL_MASK)
/*! @} */

/*! @name XTAL32K_CTRL - XTAL32k Control Register */
/*! @{ */

#define PMU_XTAL32K_CTRL_X32K_RDY_MASK           (0x1U)
#define PMU_XTAL32K_CTRL_X32K_RDY_SHIFT          (0U)
/*! X32K_RDY - Assert high when ready */
#define PMU_XTAL32K_CTRL_X32K_RDY(x)             (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_RDY_SHIFT)) & PMU_XTAL32K_CTRL_X32K_RDY_MASK)

#define PMU_XTAL32K_CTRL_X32K_STUP_ASSIST_MASK   (0x6U)
#define PMU_XTAL32K_CTRL_X32K_STUP_ASSIST_SHIFT  (1U)
/*! X32K_STUP_ASSIST - Use startup assist ckt for 32 kHz xosc */
#define PMU_XTAL32K_CTRL_X32K_STUP_ASSIST(x)     (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_STUP_ASSIST_SHIFT)) & PMU_XTAL32K_CTRL_X32K_STUP_ASSIST_MASK)

#define PMU_XTAL32K_CTRL_X32K_TEST_EN_MASK       (0x8U)
#define PMU_XTAL32K_CTRL_X32K_TEST_EN_SHIFT      (3U)
/*! X32K_TEST_EN - Test enabling for 32k xtal ckt */
#define PMU_XTAL32K_CTRL_X32K_TEST_EN(x)         (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_TEST_EN_SHIFT)) & PMU_XTAL32K_CTRL_X32K_TEST_EN_MASK)

#define PMU_XTAL32K_CTRL_X32K_TMODE_MASK         (0x30U)
#define PMU_XTAL32K_CTRL_X32K_TMODE_SHIFT        (4U)
/*! X32K_TMODE - Test mode enabling for 32k xtal ckt */
#define PMU_XTAL32K_CTRL_X32K_TMODE(x)           (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_TMODE_SHIFT)) & PMU_XTAL32K_CTRL_X32K_TMODE_MASK)

#define PMU_XTAL32K_CTRL_X32K_VDDXO_CNTL_MASK    (0x300U)
#define PMU_XTAL32K_CTRL_X32K_VDDXO_CNTL_SHIFT   (8U)
/*! X32K_VDDXO_CNTL - Control VDDXO level */
#define PMU_XTAL32K_CTRL_X32K_VDDXO_CNTL(x)      (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_VDDXO_CNTL_SHIFT)) & PMU_XTAL32K_CTRL_X32K_VDDXO_CNTL_MASK)

#define PMU_XTAL32K_CTRL_X32K_EXT_OSC_EN_MASK    (0x400U)
#define PMU_XTAL32K_CTRL_X32K_EXT_OSC_EN_SHIFT   (10U)
/*! X32K_EXT_OSC_EN - Enable external oscillator mode for outside clock */
#define PMU_XTAL32K_CTRL_X32K_EXT_OSC_EN(x)      (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_EXT_OSC_EN_SHIFT)) & PMU_XTAL32K_CTRL_X32K_EXT_OSC_EN_MASK)

#define PMU_XTAL32K_CTRL_X32K_EN_MASK            (0x800U)
#define PMU_XTAL32K_CTRL_X32K_EN_SHIFT           (11U)
/*! X32K_EN - Enable 32k oscillator */
#define PMU_XTAL32K_CTRL_X32K_EN(x)              (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_EN_SHIFT)) & PMU_XTAL32K_CTRL_X32K_EN_MASK)

#define PMU_XTAL32K_CTRL_X32K_DLY_SEL_MASK       (0x3000U)
#define PMU_XTAL32K_CTRL_X32K_DLY_SEL_SHIFT      (12U)
/*! X32K_DLY_SEL - 32k Delay Select */
#define PMU_XTAL32K_CTRL_X32K_DLY_SEL(x)         (((uint32_t)(((uint32_t)(x)) << PMU_XTAL32K_CTRL_X32K_DLY_SEL_SHIFT)) & PMU_XTAL32K_CTRL_X32K_DLY_SEL_MASK)
/*! @} */

/*! @name PMIP_BUCK_LVL - PMIP BUCK LEVEL */
/*! @{ */

#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL_MASK (0x7FU)
#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL_SHIFT (0U)
/*! NORMAL_BUCK11_SEL - Select normal mode output voltage for v11_aon VOUT = LVL*5mV + 630mV */
#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL(x)   (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL_SHIFT)) & PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL_MASK)

#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL_MASK (0x7F00U)
#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL_SHIFT (8U)
/*! NORMAL_BUCK18_SEL - Select normal mode output voltage for v18_aon VOUT = LVL*10mV + 840mV */
#define PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL(x)   (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL_SHIFT)) & PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL_MASK)

#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL_MASK  (0x7F0000U)
#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL_SHIFT (16U)
/*! SLEEP_BUCK11_SEL - Select sleep mode output voltage for v11_aon VOUT = LVL*5mV + 630mV */
#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL(x)    (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL_SHIFT)) & PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL_MASK)

#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL_MASK  (0x7F000000U)
#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL_SHIFT (24U)
/*! SLEEP_BUCK18_SEL - Select sleep mode output voltage for v18_aon VOUT = LVL*10mV + 840mV */
#define PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL(x)    (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL_SHIFT)) & PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL_MASK)
/*! @} */

/*! @name PMIP_BUCK_CTRL - PMIP BUCK ctrl */
/*! @{ */

#define PMU_PMIP_BUCK_CTRL_BUCK11_SLP_EN_MASK    (0x1U)
#define PMU_PMIP_BUCK_CTRL_BUCK11_SLP_EN_SHIFT   (0U)
/*! BUCK11_SLP_EN - 1'b1: sleep mode; 1'b0: active mode. */
#define PMU_PMIP_BUCK_CTRL_BUCK11_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_BUCK11_SLP_EN_SHIFT)) & PMU_PMIP_BUCK_CTRL_BUCK11_SLP_EN_MASK)

#define PMU_PMIP_BUCK_CTRL_BUCK11_SW_PD_MASK     (0x2U)
#define PMU_PMIP_BUCK_CTRL_BUCK11_SW_PD_SHIFT    (1U)
/*! BUCK11_SW_PD - Power Down BUCK11 */
#define PMU_PMIP_BUCK_CTRL_BUCK11_SW_PD(x)       (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_BUCK11_SW_PD_SHIFT)) & PMU_PMIP_BUCK_CTRL_BUCK11_SW_PD_MASK)

#define PMU_PMIP_BUCK_CTRL_BUCK18_SLP_EN_MASK    (0x4U)
#define PMU_PMIP_BUCK_CTRL_BUCK18_SLP_EN_SHIFT   (2U)
/*! BUCK18_SLP_EN - 1'b1: sleep mode; 1'b0: active mode. */
#define PMU_PMIP_BUCK_CTRL_BUCK18_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_BUCK18_SLP_EN_SHIFT)) & PMU_PMIP_BUCK_CTRL_BUCK18_SLP_EN_MASK)

#define PMU_PMIP_BUCK_CTRL_BUCK18_SW_PD_MASK     (0x8U)
#define PMU_PMIP_BUCK_CTRL_BUCK18_SW_PD_SHIFT    (3U)
/*! BUCK18_SW_PD - Power Down BUCK18 */
#define PMU_PMIP_BUCK_CTRL_BUCK18_SW_PD(x)       (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_BUCK18_SW_PD_SHIFT)) & PMU_PMIP_BUCK_CTRL_BUCK18_SW_PD_MASK)

#define PMU_PMIP_BUCK_CTRL_PM3_BUCK11_ON_MASK    (0x40U)
#define PMU_PMIP_BUCK_CTRL_PM3_BUCK11_ON_SHIFT   (6U)
/*! PM3_BUCK11_ON - provide a enable when sleep condition:1'b1:PM3 BUCK11 ON;1'b0:PM3 BUCK11 SLEEP */
#define PMU_PMIP_BUCK_CTRL_PM3_BUCK11_ON(x)      (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_PM3_BUCK11_ON_SHIFT)) & PMU_PMIP_BUCK_CTRL_PM3_BUCK11_ON_MASK)

#define PMU_PMIP_BUCK_CTRL_PM3_BUCK18_ON_MASK    (0x80U)
#define PMU_PMIP_BUCK_CTRL_PM3_BUCK18_ON_SHIFT   (7U)
/*! PM3_BUCK18_ON - provide a enable when sleep condition:1'b1:PM3 BUCK18 ON;1'b0:PM3 BUCK18 SLEEP */
#define PMU_PMIP_BUCK_CTRL_PM3_BUCK18_ON(x)      (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_BUCK_CTRL_PM3_BUCK18_ON_SHIFT)) & PMU_PMIP_BUCK_CTRL_PM3_BUCK18_ON_MASK)
/*! @} */

/*! @name PMIP_LDO_LVL - PMIP LDO level ctrl */
/*! @{ */

#define PMU_PMIP_LDO_LVL_LDO18_SEL_MASK          (0x7U)
#define PMU_PMIP_LDO_LVL_LDO18_SEL_SHIFT         (0U)
/*! LDO18_SEL - Select output voltage for v18_aon
 *  0b000..1.60V
 *  0b001..1.65V
 *  0b010..1.70V
 *  0b011..1.75V
 *  0b100..1.80V
 *  0b101..1.85V
 *  0b110..1.90V
 *  0b111..1.95V
 */
#define PMU_PMIP_LDO_LVL_LDO18_SEL(x)            (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_LDO_LVL_LDO18_SEL_SHIFT)) & PMU_PMIP_LDO_LVL_LDO18_SEL_MASK)

#define PMU_PMIP_LDO_LVL_LDO11_SEL_MASK          (0x70U)
#define PMU_PMIP_LDO_LVL_LDO11_SEL_SHIFT         (4U)
/*! LDO11_SEL - Select output voltage for v11_aon
 *  0b000..0.75V
 *  0b001..0.80V
 *  0b010..0.85V
 *  0b011..0.90V
 *  0b100..0.95V
 *  0b101..1.00V
 *  0b110..1.05V
 *  0b111..1.10V
 */
#define PMU_PMIP_LDO_LVL_LDO11_SEL(x)            (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_LDO_LVL_LDO11_SEL_SHIFT)) & PMU_PMIP_LDO_LVL_LDO11_SEL_MASK)
/*! @} */

/*! @name PMIP_RST - PMIP reset request register */
/*! @{ */

#define PMU_PMIP_RST_SW_RST_REQ_MASK             (0x1U)
#define PMU_PMIP_RST_SW_RST_REQ_SHIFT            (0U)
/*! SW_RST_REQ - PMIP software por request
 *  0b0..no rst request
 *  0b1..send rst request, need write 1->0->1->0 sequence
 */
#define PMU_PMIP_RST_SW_RST_REQ(x)               (((uint32_t)(((uint32_t)(x)) << PMU_PMIP_RST_SW_RST_REQ_SHIFT)) & PMU_PMIP_RST_SW_RST_REQ_MASK)
/*! @} */

/*! @name BOD - BOD register */
/*! @{ */

#define PMU_BOD_EN_MASK                          (0x1U)
#define PMU_BOD_EN_SHIFT                         (0U)
/*! EN - bod enable */
#define PMU_BOD_EN(x)                            (((uint32_t)(((uint32_t)(x)) << PMU_BOD_EN_SHIFT)) & PMU_BOD_EN_MASK)

#define PMU_BOD__1_85_INT_NEG_MASK               (0x10U)
#define PMU_BOD__1_85_INT_NEG_SHIFT              (4U)
/*! _1_85_INT_NEG - bod_1_85_int negedge */
#define PMU_BOD__1_85_INT_NEG(x)                 (((uint32_t)(((uint32_t)(x)) << PMU_BOD__1_85_INT_NEG_SHIFT)) & PMU_BOD__1_85_INT_NEG_MASK)

#define PMU_BOD__1_85_INT_CLR_NEG_MASK           (0x20U)
#define PMU_BOD__1_85_INT_CLR_NEG_SHIFT          (5U)
/*! _1_85_INT_CLR_NEG - clr bod_1_85_int negedge */
#define PMU_BOD__1_85_INT_CLR_NEG(x)             (((uint32_t)(((uint32_t)(x)) << PMU_BOD__1_85_INT_CLR_NEG_SHIFT)) & PMU_BOD__1_85_INT_CLR_NEG_MASK)
/*! @} */

/*! @name MEM_CFG - mem configuration register */
/*! @{ */

#define PMU_MEM_CFG_MEM_RET_MASK                 (0x3FU)
#define PMU_MEM_CFG_MEM_RET_SHIFT                (0U)
/*! MEM_RET - mem Retention enable register in PM3 mode
 *  0b000000..in PM3, it will keep retention
 *  0b000001..in PM3, it will shut down
 */
#define PMU_MEM_CFG_MEM_RET(x)                   (((uint32_t)(((uint32_t)(x)) << PMU_MEM_CFG_MEM_RET_SHIFT)) & PMU_MEM_CFG_MEM_RET_MASK)

#define PMU_MEM_CFG_AON_MEM_RET_MASK             (0x100U)
#define PMU_MEM_CFG_AON_MEM_RET_SHIFT            (8U)
/*! AON_MEM_RET - aon mem Retention enable register in PM4 mode
 *  0b0..in PM4, it will keep retention
 *  0b1..in PM4, it will shut down
 */
#define PMU_MEM_CFG_AON_MEM_RET(x)               (((uint32_t)(((uint32_t)(x)) << PMU_MEM_CFG_AON_MEM_RET_SHIFT)) & PMU_MEM_CFG_AON_MEM_RET_MASK)
/*! @} */

/*! @name RESET_DISABLE - reset disable register */
/*! @{ */

#define PMU_RESET_DISABLE_PINMUX_MASK            (0x1U)
#define PMU_RESET_DISABLE_PINMUX_SHIFT           (0U)
/*! PINMUX - avoid reset warm reset
 *  0b0..not disable warm reset for pinmux
 *  0b1..disable warm reset for pinmux
 */
#define PMU_RESET_DISABLE_PINMUX(x)              (((uint32_t)(((uint32_t)(x)) << PMU_RESET_DISABLE_PINMUX_SHIFT)) & PMU_RESET_DISABLE_PINMUX_MASK)
/*! @} */

/*! @name WLAN_CTRL - WLAN Control Register */
/*! @{ */

#define PMU_WLAN_CTRL_WL_XOSC_EN_MASK            (0x2U)
#define PMU_WLAN_CTRL_WL_XOSC_EN_SHIFT           (1U)
/*! WL_XOSC_EN - wl request control different modes for CAU XTAL (1 for normal mode, 0 for sleep/full PD) */
#define PMU_WLAN_CTRL_WL_XOSC_EN(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WLAN_CTRL_WL_XOSC_EN_SHIFT)) & PMU_WLAN_CTRL_WL_XOSC_EN_MASK)

#define PMU_WLAN_CTRL_WL_SLEEP_MASK              (0xCU)
#define PMU_WLAN_CTRL_WL_SLEEP_SHIFT             (2U)
/*! WL_SLEEP - WLAN SYS sleep; bit[0]:SOCWLAPU_WLGATED_PSW_PD_AON, bit[1]:cpu1_cp15_sleep */
#define PMU_WLAN_CTRL_WL_SLEEP(x)                (((uint32_t)(((uint32_t)(x)) << PMU_WLAN_CTRL_WL_SLEEP_SHIFT)) & PMU_WLAN_CTRL_WL_SLEEP_MASK)

#define PMU_WLAN_CTRL_WL_WAKEUP_MASK             (0xFF00U)
#define PMU_WLAN_CTRL_WL_WAKEUP_SHIFT            (8U)
/*! WL_WAKEUP - MCI_WL_WAKEUP */
#define PMU_WLAN_CTRL_WL_WAKEUP(x)               (((uint32_t)(((uint32_t)(x)) << PMU_WLAN_CTRL_WL_WAKEUP_SHIFT)) & PMU_WLAN_CTRL_WL_WAKEUP_MASK)
/*! @} */

/*! @name BLE_CTRL - BLEControl Register */
/*! @{ */

#define PMU_BLE_CTRL_BLE_XOSC_EN_MASK            (0x2U)
#define PMU_BLE_CTRL_BLE_XOSC_EN_SHIFT           (1U)
/*! BLE_XOSC_EN - ble request control different modes for CAU XTAL (1 for normal mode, 0 for sleep/full PD) */
#define PMU_BLE_CTRL_BLE_XOSC_EN(x)              (((uint32_t)(((uint32_t)(x)) << PMU_BLE_CTRL_BLE_XOSC_EN_SHIFT)) & PMU_BLE_CTRL_BLE_XOSC_EN_MASK)

#define PMU_BLE_CTRL_BLE_SLEEP_MASK              (0xCU)
#define PMU_BLE_CTRL_BLE_SLEEP_SHIFT             (2U)
/*! BLE_SLEEP - BLE SYS sleep; bit[0]:SOCBTAPU_BLEGATED_PSW_PD_AON, bit[1]:cpu2_cp15_sleep */
#define PMU_BLE_CTRL_BLE_SLEEP(x)                (((uint32_t)(((uint32_t)(x)) << PMU_BLE_CTRL_BLE_SLEEP_SHIFT)) & PMU_BLE_CTRL_BLE_SLEEP_MASK)

#define PMU_BLE_CTRL_BLE_WAKEUP_MASK             (0xFF00U)
#define PMU_BLE_CTRL_BLE_WAKEUP_SHIFT            (8U)
/*! BLE_WAKEUP - MCI_BLE_WAKEUP */
#define PMU_BLE_CTRL_BLE_WAKEUP(x)               (((uint32_t)(((uint32_t)(x)) << PMU_BLE_CTRL_BLE_WAKEUP_SHIFT)) & PMU_BLE_CTRL_BLE_WAKEUP_MASK)
/*! @} */

/*! @name CLK_AON - Always on Domain Clock select */
/*! @{ */

#define PMU_CLK_AON_CLK_32K_AON_MASK             (0x3U)
#define PMU_CLK_AON_CLK_32K_AON_SHIFT            (0U)
/*! CLK_32K_AON - 32K clock select for PMU and RTC
 *  0b00..RC32K clock
 *  0b01..XTAL32K clock
 *  0b10..NCO32K clock
 */
#define PMU_CLK_AON_CLK_32K_AON(x)               (((uint32_t)(((uint32_t)(x)) << PMU_CLK_AON_CLK_32K_AON_SHIFT)) & PMU_CLK_AON_CLK_32K_AON_MASK)

#define PMU_CLK_AON_PMU_CLK_MASK                 (0x4U)
#define PMU_CLK_AON_PMU_CLK_SHIFT                (2U)
/*! PMU_CLK - pmu clock select
 *  0b0..fast clock, sys clock after divider
 *  0b1..slow clock,clk_32k
 */
#define PMU_CLK_AON_PMU_CLK(x)                   (((uint32_t)(((uint32_t)(x)) << PMU_CLK_AON_PMU_CLK_SHIFT)) & PMU_CLK_AON_PMU_CLK_MASK)

#define PMU_CLK_AON_PMIP_SLOW_CLK_RDY_MASK       (0x10U)
#define PMU_CLK_AON_PMIP_SLOW_CLK_RDY_SHIFT      (4U)
/*! PMIP_SLOW_CLK_RDY - one of 32k source ready signal
 *  0b0..none 32k is ready
 *  0b1..one of 32k source is ready
 */
#define PMU_CLK_AON_PMIP_SLOW_CLK_RDY(x)         (((uint32_t)(((uint32_t)(x)) << PMU_CLK_AON_PMIP_SLOW_CLK_RDY_SHIFT)) & PMU_CLK_AON_PMIP_SLOW_CLK_RDY_MASK)
/*! @} */

/*! @name SOC_MEM_PDWN - soc mem pdwn register */
/*! @{ */

#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL_MASK  (0x1U)
#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL_SHIFT (0U)
/*! MSC_MEM_PDWN_CTRL - msc mem pdwn control register
 *  0b0..HW control mem_pdwn
 *  0b1..SW CFG mem_pdwn
 */
#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL(x)    (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL_SHIFT)) & PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL_MASK)

#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL_MASK (0x2U)
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL_SHIFT (1U)
/*! SOCTOP_OTP_PDWN_CTRL - soc top otp pdwn control register
 *  0b0..HW control pdwn
 *  0b1..SW CFG pdwn
 */
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL(x) (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL_SHIFT)) & PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL_MASK)

#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_MASK       (0x10U)
#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_SHIFT      (4U)
/*! MSC_MEM_PDWN - msc mem pdwn cfg register
 *  0b0..de-assert mem_pdwn
 *  0b1..assert mem_pdwn
 */
#define PMU_SOC_MEM_PDWN_MSC_MEM_PDWN(x)         (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_SHIFT)) & PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_MASK)

#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN0_MASK   (0x20U)
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN0_SHIFT  (5U)
/*! SOCTOP_OTP_PDWN0 - soc_top_otp_pdwn0 cfg register
 *  0b0..de-assert pdwn
 *  0b1..assert pdwn
 */
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN0(x)     (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN0_SHIFT)) & PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN0_MASK)

#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN1_MASK   (0x40U)
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN1_SHIFT  (6U)
/*! SOCTOP_OTP_PDWN1 - soc_top_otp_pdwn1 cfg register
 *  0b0..de-assert pdwn
 *  0b1..assert pdwn
 */
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN1(x)     (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN1_SHIFT)) & PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN1_MASK)

#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN2_MASK   (0x80U)
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN2_SHIFT  (7U)
/*! SOCTOP_OTP_PDWN2 - soc_top_otp_pdwn2 cfg register
 *  0b0..de-assert pdwn
 *  0b1..assert pdwn
 */
#define PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN2(x)     (((uint32_t)(((uint32_t)(x)) << PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN2_SHIFT)) & PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN2_MASK)
/*! @} */

/*! @name AON_PAD_OUT_CTRL - aon pad out control */
/*! @{ */

#define PMU_AON_PAD_OUT_CTRL_EN_MASK             (0x1U)
#define PMU_AON_PAD_OUT_CTRL_EN_SHIFT            (0U)
/*! EN - aon pad ouput en */
#define PMU_AON_PAD_OUT_CTRL_EN(x)               (((uint32_t)(((uint32_t)(x)) << PMU_AON_PAD_OUT_CTRL_EN_SHIFT)) & PMU_AON_PAD_OUT_CTRL_EN_MASK)

#define PMU_AON_PAD_OUT_CTRL_VALUE_MASK          (0x2U)
#define PMU_AON_PAD_OUT_CTRL_VALUE_SHIFT         (1U)
/*! VALUE - aon pad output value */
#define PMU_AON_PAD_OUT_CTRL_VALUE(x)            (((uint32_t)(((uint32_t)(x)) << PMU_AON_PAD_OUT_CTRL_VALUE_SHIFT)) & PMU_AON_PAD_OUT_CTRL_VALUE_MASK)
/*! @} */

/*! @name WAKEUP_PM2_MASK0 - Wakeup PM2 state Mask Interrupt Register */
/*! @{ */

#define PMU_WAKEUP_PM2_MASK0_WDT0_MASK           (0x1U)
#define PMU_WAKEUP_PM2_MASK0_WDT0_SHIFT          (0U)
/*! WDT0 - WDT0 Wakeup Mask
 *  0b0..mask wdt0 wakeup interrupt
 *  0b1..unmask wdt0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_WDT0(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_WDT0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_WDT0_MASK)

#define PMU_WAKEUP_PM2_MASK0_DMA0_MASK           (0x2U)
#define PMU_WAKEUP_PM2_MASK0_DMA0_SHIFT          (1U)
/*! DMA0 - DMA0 Wakeup Mask
 *  0b0..mask dma0 wakeup interrupt
 *  0b1..unmask dma0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_DMA0(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_DMA0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_DMA0_MASK)

#define PMU_WAKEUP_PM2_MASK0_GPIO_INTA_MASK      (0x4U)
#define PMU_WAKEUP_PM2_MASK0_GPIO_INTA_SHIFT     (2U)
/*! GPIO_INTA - GPIO_INTA Wakeup Mask
 *  0b0..mask gpio_inta wakeup interrupt
 *  0b1..unmask gpio_inta wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_GPIO_INTA(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_GPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_MASK0_GPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_MASK0_GPIO_INTB_MASK      (0x8U)
#define PMU_WAKEUP_PM2_MASK0_GPIO_INTB_SHIFT     (3U)
/*! GPIO_INTB - GPIO_INTB Wakeup Mask
 *  0b0..mask gpio_intb wakeup interrupt
 *  0b1..unmask gpio_intb wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_GPIO_INTB(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_GPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_MASK0_GPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_MASK0_PIN_INT0_MASK       (0x10U)
#define PMU_WAKEUP_PM2_MASK0_PIN_INT0_SHIFT      (4U)
/*! PIN_INT0 - PIN_INT0 Wakeup Mask
 *  0b0..mask pin_int0 wakeup interrupt
 *  0b1..unmask pin_int0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_PIN_INT0(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_PIN_INT0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_PIN_INT0_MASK)

#define PMU_WAKEUP_PM2_MASK0_PIN_INT1_MASK       (0x20U)
#define PMU_WAKEUP_PM2_MASK0_PIN_INT1_SHIFT      (5U)
/*! PIN_INT1 - PIN_INT1 Wakeup Mask
 *  0b0..mask pin_int1 wakeup interrupt
 *  0b1..unmask pin_int1 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_PIN_INT1(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_PIN_INT1_SHIFT)) & PMU_WAKEUP_PM2_MASK0_PIN_INT1_MASK)

#define PMU_WAKEUP_PM2_MASK0_PIN_INT2_MASK       (0x40U)
#define PMU_WAKEUP_PM2_MASK0_PIN_INT2_SHIFT      (6U)
/*! PIN_INT2 - PIN_INT2 Wakeup Mask
 *  0b0..mask pin_int2 wakeup interrupt
 *  0b1..unmask pin_int2 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_PIN_INT2(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_PIN_INT2_SHIFT)) & PMU_WAKEUP_PM2_MASK0_PIN_INT2_MASK)

#define PMU_WAKEUP_PM2_MASK0_PIN_INT3_MASK       (0x80U)
#define PMU_WAKEUP_PM2_MASK0_PIN_INT3_SHIFT      (7U)
/*! PIN_INT3 - PIN_INT3 Wakeup Mask
 *  0b0..mask pin_int3 wakeup interrupt
 *  0b1..unmask pin_int3 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_PIN_INT3(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_PIN_INT3_SHIFT)) & PMU_WAKEUP_PM2_MASK0_PIN_INT3_MASK)

#define PMU_WAKEUP_PM2_MASK0_UTICK_MASK          (0x100U)
#define PMU_WAKEUP_PM2_MASK0_UTICK_SHIFT         (8U)
/*! UTICK - UTICK Wakeup Mask
 *  0b0..mask utick wakeup interrupt
 *  0b1..unmask utick wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_UTICK(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_UTICK_SHIFT)) & PMU_WAKEUP_PM2_MASK0_UTICK_MASK)

#define PMU_WAKEUP_PM2_MASK0_MRT_MASK            (0x200U)
#define PMU_WAKEUP_PM2_MASK0_MRT_SHIFT           (9U)
/*! MRT - MRT Wakeup Mask
 *  0b0..mask mrt wakeup interrupt
 *  0b1..unmask mrt wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_MRT(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_MRT_SHIFT)) & PMU_WAKEUP_PM2_MASK0_MRT_MASK)

#define PMU_WAKEUP_PM2_MASK0_CTIMER0_MASK        (0x400U)
#define PMU_WAKEUP_PM2_MASK0_CTIMER0_SHIFT       (10U)
/*! CTIMER0 - CTIMER0 Wakeup Mask
 *  0b0..mask ctimer0 wakeup interrupt
 *  0b1..unmask ctimer0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_CTIMER0(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_CTIMER0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_CTIMER0_MASK)

#define PMU_WAKEUP_PM2_MASK0_CTIMER1_MASK        (0x800U)
#define PMU_WAKEUP_PM2_MASK0_CTIMER1_SHIFT       (11U)
/*! CTIMER1 - CTIMER1 Wakeup Mask
 *  0b0..mask CTIMER1 wakeup interrupt
 *  0b1..unmask CTIMER1 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_CTIMER1(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_CTIMER1_SHIFT)) & PMU_WAKEUP_PM2_MASK0_CTIMER1_MASK)

#define PMU_WAKEUP_PM2_MASK0_SCT0_MASK           (0x1000U)
#define PMU_WAKEUP_PM2_MASK0_SCT0_SHIFT          (12U)
/*! SCT0 - SCT0 Wakeup Mask
 *  0b0..mask sct0 wakeup interrupt
 *  0b1..unmask sct0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_SCT0(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_SCT0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_SCT0_MASK)

#define PMU_WAKEUP_PM2_MASK0_CTIMER3_MASK        (0x2000U)
#define PMU_WAKEUP_PM2_MASK0_CTIMER3_SHIFT       (13U)
/*! CTIMER3 - CTIMER3 Wakeup Mask
 *  0b0..mask ctimer3 wakeup interrupt
 *  0b1..unmask ctimer3 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_CTIMER3(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_CTIMER3_SHIFT)) & PMU_WAKEUP_PM2_MASK0_CTIMER3_MASK)

#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM0_MASK      (0x4000U)
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM0_SHIFT     (14U)
/*! FLEXCOMM0 - FLEXCOMM0 Wakeup Mask
 *  0b0..mask flexcomm0 wakeup interrupt
 *  0b1..unmask flexcomm0 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM0(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FLEXCOMM0_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FLEXCOMM0_MASK)

#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM1_MASK      (0x8000U)
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM1_SHIFT     (15U)
/*! FLEXCOMM1 - FLEXCOMM1 Wakeup Mask
 *  0b0..mask flexcomm1 wakeup interrupt
 *  0b1..unmask flexcomm1 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM1(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FLEXCOMM1_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FLEXCOMM1_MASK)

#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM2_MASK      (0x10000U)
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM2_SHIFT     (16U)
/*! FLEXCOMM2 - FLEXCOMM2 Wakeup Mask
 *  0b0..mask flexcomm2 wakeup interrupt
 *  0b1..unmask flexcomm2 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM2(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FLEXCOMM2_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FLEXCOMM2_MASK)

#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM3_MASK      (0x20000U)
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM3_SHIFT     (17U)
/*! FLEXCOMM3 - FLEXCOMM3 Wakeup Mask
 *  0b0..mask flexcomm3 wakeup interrupt
 *  0b1..unmask flexcomm3 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM3(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FLEXCOMM3_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FLEXCOMM3_MASK)

#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM14_MASK     (0x100000U)
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM14_SHIFT    (20U)
/*! FLEXCOMM14 - FLEXCOMM14 Wakeup Mask
 *  0b0..mask flexcomm14 wakeup interrupt
 *  0b1..unmask flexcomm14 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FLEXCOMM14(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FLEXCOMM14_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FLEXCOMM14_MASK)

#define PMU_WAKEUP_PM2_MASK0_FREEMRT_GLOBAL_MASK (0x800000U)
#define PMU_WAKEUP_PM2_MASK0_FREEMRT_GLOBAL_SHIFT (23U)
/*! FREEMRT_GLOBAL - Free Multi-rate timer Wakeup Mask
 *  0b0..mask freemrt_global wakeup interrupt
 *  0b1..unmask freemrt_global wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_FREEMRT_GLOBAL(x)   (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_FREEMRT_GLOBAL_SHIFT)) & PMU_WAKEUP_PM2_MASK0_FREEMRT_GLOBAL_MASK)

#define PMU_WAKEUP_PM2_MASK0_DMIC_MASK           (0x2000000U)
#define PMU_WAKEUP_PM2_MASK0_DMIC_SHIFT          (25U)
/*! DMIC - DMIC Wakeup Mask
 *  0b0..mask dmic wakeup interrupt
 *  0b1..unmask dmic wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_DMIC(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_DMIC_SHIFT)) & PMU_WAKEUP_PM2_MASK0_DMIC_MASK)

#define PMU_WAKEUP_PM2_MASK0_WAKEUP_FROM_DEEPSLEEP_MASK (0x4000000U)
#define PMU_WAKEUP_PM2_MASK0_WAKEUP_FROM_DEEPSLEEP_SHIFT (26U)
/*! WAKEUP_FROM_DEEPSLEEP - Wakeup from Deepsleep Wakeup Mask
 *  0b0..mask wakeup_from_deepsleep wakeup interrupt
 *  0b1..unmask wakeup_from_deepsleep wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_WAKEUP_FROM_DEEPSLEEP(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_WAKEUP_FROM_DEEPSLEEP_SHIFT)) & PMU_WAKEUP_PM2_MASK0_WAKEUP_FROM_DEEPSLEEP_MASK)

#define PMU_WAKEUP_PM2_MASK0_HYPERVISOR_MASK     (0x8000000U)
#define PMU_WAKEUP_PM2_MASK0_HYPERVISOR_SHIFT    (27U)
/*! HYPERVISOR - HYPERVISOR Wakeup Mask
 *  0b0..mask hypervisor wakeup interrupt
 *  0b1..unmask hypervisor wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_HYPERVISOR(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_HYPERVISOR_SHIFT)) & PMU_WAKEUP_PM2_MASK0_HYPERVISOR_MASK)

#define PMU_WAKEUP_PM2_MASK0_SECUREVIOLATION_MASK (0x10000000U)
#define PMU_WAKEUP_PM2_MASK0_SECUREVIOLATION_SHIFT (28U)
/*! SECUREVIOLATION - Secure Violation Wakeup Mask
 *  0b0..mask secure violation wakeup interrupt
 *  0b1..unmask secure violation wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_SECUREVIOLATION(x)  (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_SECUREVIOLATION_SHIFT)) & PMU_WAKEUP_PM2_MASK0_SECUREVIOLATION_MASK)

#define PMU_WAKEUP_PM2_MASK0_HWVAD_MASK          (0x20000000U)
#define PMU_WAKEUP_PM2_MASK0_HWVAD_SHIFT         (29U)
/*! HWVAD - Hardware Voice Activity Detector Wakeup Mask
 *  0b0..mask hwvad wakeup interrupt
 *  0b1..unmask hwvad wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK0_HWVAD(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK0_HWVAD_SHIFT)) & PMU_WAKEUP_PM2_MASK0_HWVAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_MASK1 - Wakeup PM2 state Mask Interrupt Register */
/*! @{ */

#define PMU_WAKEUP_PM2_MASK1_RTC_MASK            (0x1U)
#define PMU_WAKEUP_PM2_MASK1_RTC_SHIFT           (0U)
/*! RTC - RTC Wakeup Mask
 *  0b0..mask rtc wakeup interrupt
 *  0b1..unmask rtc wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_RTC(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_RTC_SHIFT)) & PMU_WAKEUP_PM2_MASK1_RTC_MASK)

#define PMU_WAKEUP_PM2_MASK1_PIN_INT4_MASK       (0x8U)
#define PMU_WAKEUP_PM2_MASK1_PIN_INT4_SHIFT      (3U)
/*! PIN_INT4 - PIN_INT4 Wakeup Mask
 *  0b0..mask pin_int4 wakeup interrupt
 *  0b1..unmask pin_int4 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_PIN_INT4(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_PIN_INT4_SHIFT)) & PMU_WAKEUP_PM2_MASK1_PIN_INT4_MASK)

#define PMU_WAKEUP_PM2_MASK1_PIN_INT5_MASK       (0x10U)
#define PMU_WAKEUP_PM2_MASK1_PIN_INT5_SHIFT      (4U)
/*! PIN_INT5 - PIN_INT5 Wakeup Mask
 *  0b0..mask pin_int5 wakeup interrupt
 *  0b1..unmask pin_int5 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_PIN_INT5(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_PIN_INT5_SHIFT)) & PMU_WAKEUP_PM2_MASK1_PIN_INT5_MASK)

#define PMU_WAKEUP_PM2_MASK1_PIN_INT6_MASK       (0x20U)
#define PMU_WAKEUP_PM2_MASK1_PIN_INT6_SHIFT      (5U)
/*! PIN_INT6 - PIN_INT6 Wakeup Mask
 *  0b0..mask pin_int6 wakeup interrupt
 *  0b1..unmask pin_int6 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_PIN_INT6(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_PIN_INT6_SHIFT)) & PMU_WAKEUP_PM2_MASK1_PIN_INT6_MASK)

#define PMU_WAKEUP_PM2_MASK1_PIN_INT7_MASK       (0x40U)
#define PMU_WAKEUP_PM2_MASK1_PIN_INT7_SHIFT      (6U)
/*! PIN_INT7 - PIN_INT7 Wakeup Mask
 *  0b0..mask pin_int7 wakeup interrupt
 *  0b1..unmask pin_int7 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_PIN_INT7(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_PIN_INT7_SHIFT)) & PMU_WAKEUP_PM2_MASK1_PIN_INT7_MASK)

#define PMU_WAKEUP_PM2_MASK1_CTIMER2_MASK        (0x80U)
#define PMU_WAKEUP_PM2_MASK1_CTIMER2_SHIFT       (7U)
/*! CTIMER2 - CTIMER2 Wakeup Mask
 *  0b0..mask ctimer2 wakeup interrupt
 *  0b1..unmask ctimer2 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_CTIMER2(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_CTIMER2_SHIFT)) & PMU_WAKEUP_PM2_MASK1_CTIMER2_MASK)

#define PMU_WAKEUP_PM2_MASK1_OS_EVENT_TIMER_MASK (0x200U)
#define PMU_WAKEUP_PM2_MASK1_OS_EVENT_TIMER_SHIFT (9U)
/*! OS_EVENT_TIMER - OS_EVENT_TIMER Wakeup Mask
 *  0b0..mask os_event_timer wakeup interrupt
 *  0b1..unmask os_event_timer wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_OS_EVENT_TIMER(x)   (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_OS_EVENT_TIMER_SHIFT)) & PMU_WAKEUP_PM2_MASK1_OS_EVENT_TIMER_MASK)

#define PMU_WAKEUP_PM2_MASK1_FLEX_SPI_MASK       (0x400U)
#define PMU_WAKEUP_PM2_MASK1_FLEX_SPI_SHIFT      (10U)
/*! FLEX_SPI - Flex SPI Wakeup Mask
 *  0b0..mask flex_spi wakeup interrupt
 *  0b1..unmask flex_spi wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_FLEX_SPI(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_FLEX_SPI_SHIFT)) & PMU_WAKEUP_PM2_MASK1_FLEX_SPI_MASK)

#define PMU_WAKEUP_PM2_MASK1_SDU_MASK            (0x4000U)
#define PMU_WAKEUP_PM2_MASK1_SDU_SHIFT           (14U)
/*! SDU - SDU Wakeup Mask
 *  0b0..mask sdu wakeup interrupt
 *  0b1..unmask sdu wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_SDU(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_SDU_SHIFT)) & PMU_WAKEUP_PM2_MASK1_SDU_MASK)

#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTA_MASK     (0x8000U)
#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTA_SHIFT    (15U)
/*! SGPIO_INTA - SGPIO_INTA Wakeup Mask
 *  0b0..mask sgpio_inta wakeup interrupt
 *  0b1..unmask sgpio_inta wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTA(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_SGPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_MASK1_SGPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTB_MASK     (0x10000U)
#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTB_SHIFT    (16U)
/*! SGPIO_INTB - SGPIO_INTB Wakeup Mask
 *  0b0..mask sgpio_intb wakeup interrupt
 *  0b1..unmask sgpio_intb wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_SGPIO_INTB(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_SGPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_MASK1_SGPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_MASK1_USB_MASK            (0x40000U)
#define PMU_WAKEUP_PM2_MASK1_USB_SHIFT           (18U)
/*! USB - USB Wakeup Mask
 *  0b0..mask usb wakeup interrupt
 *  0b1..unmask usb wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_USB(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_USB_SHIFT)) & PMU_WAKEUP_PM2_MASK1_USB_MASK)

#define PMU_WAKEUP_PM2_MASK1_DMA1_MASK           (0x400000U)
#define PMU_WAKEUP_PM2_MASK1_DMA1_SHIFT          (22U)
/*! DMA1 - DMA1 Wakeup Mask
 *  0b0..mask dma1 wakeup interrupt
 *  0b1..unmask dma1 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_DMA1(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_DMA1_SHIFT)) & PMU_WAKEUP_PM2_MASK1_DMA1_MASK)

#define PMU_WAKEUP_PM2_MASK1_PUF_MASK            (0x800000U)
#define PMU_WAKEUP_PM2_MASK1_PUF_SHIFT           (23U)
/*! PUF - PUF Wakeup Mask
 *  0b0..mask puf wakeup interrupt
 *  0b1..unmask puf wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_PUF(x)              (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_PUF_SHIFT)) & PMU_WAKEUP_PM2_MASK1_PUF_MASK)

#define PMU_WAKEUP_PM2_MASK1_POWER_QUAD_MASK     (0x1000000U)
#define PMU_WAKEUP_PM2_MASK1_POWER_QUAD_SHIFT    (24U)
/*! POWER_QUAD - POWER QUAD Wakeup Mask
 *  0b0..mask power_quad wakeup interrupt
 *  0b1..unmask power_quad wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK1_POWER_QUAD(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK1_POWER_QUAD_SHIFT)) & PMU_WAKEUP_PM2_MASK1_POWER_QUAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_MASK3 - Wakeup PM2 state Mask Interrupt Register */
/*! @{ */

#define PMU_WAKEUP_PM2_MASK3_GAU_GPDAC_INT_FUN11_MASK (0x1000U)
#define PMU_WAKEUP_PM2_MASK3_GAU_GPDAC_INT_FUN11_SHIFT (12U)
/*! GAU_GPDAC_INT_FUN11 - gau_gpdac_int_fun11 Wakeup Mask
 *  0b0..mask gau_gpdac_int_fun11 wakeup interrupt
 *  0b1..unmask gau_gpdac_int_fun11 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_GAU_GPDAC_INT_FUN11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_GAU_GPDAC_INT_FUN11_SHIFT)) & PMU_WAKEUP_PM2_MASK3_GAU_GPDAC_INT_FUN11_MASK)

#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_WKUP11_MASK (0x2000U)
#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_WKUP11_SHIFT (13U)
/*! GAU_ACOMP_INT_WKUP11 - gau_acomp_int_wkup11 Wakeup Mask
 *  0b0..mask gau_acomp_int_wkup11 wakeup interrupt
 *  0b1..unmask gau_acomp_int_wkup11 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_WKUP11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_WKUP11_SHIFT)) & PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_WKUP11_MASK)

#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_FUNC11_MASK (0x4000U)
#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_FUNC11_SHIFT (14U)
/*! GAU_ACOMP_INT_FUNC11 - gau_acomp_int_func11 Wakeup Mask
 *  0b0..mask gau_acomp_int_func11 wakeup interrupt
 *  0b1..unmask gau_acomp_int_func11 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_MASK3_GAU_ACOMP_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC1_INT_FUNC11_MASK (0x8000U)
#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC1_INT_FUNC11_SHIFT (15U)
/*! GAU_GPADC1_INT_FUNC11 - gau_gpadc1_int_func11 Wakeup Mask
 *  0b0..mask gau_gpadc1_int_func11 wakeup interrupt
 *  0b1..unmask gau_gpadc1_int_func11 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC1_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_GAU_GPADC1_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_MASK3_GAU_GPADC1_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC0_INT_FUNC11_MASK (0x10000U)
#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC0_INT_FUNC11_SHIFT (16U)
/*! GAU_GPADC0_INT_FUNC11 - gau_gpadc0_int_func11 Wakeup Mask
 *  0b0..mask gau_gpadc0_int_func11 wakeup interrupt
 *  0b1..unmask gau_gpadc0_int_func11 wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_GAU_GPADC0_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_GAU_GPADC0_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_MASK3_GAU_GPADC0_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_MASK3_USIM_MASK           (0x20000U)
#define PMU_WAKEUP_PM2_MASK3_USIM_SHIFT          (17U)
/*! USIM - usim Wakeup Mask
 *  0b0..mask usim wakeup interrupt
 *  0b1..unmask usim wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_USIM(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_USIM_SHIFT)) & PMU_WAKEUP_PM2_MASK3_USIM_MASK)

#define PMU_WAKEUP_PM2_MASK3_ENET_MASK           (0x80000U)
#define PMU_WAKEUP_PM2_MASK3_ENET_SHIFT          (19U)
/*! ENET - enet Wakeup Mask
 *  0b0..mask enet wakeup interrupt
 *  0b1..unmask enet wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_ENET(x)             (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_ENET_SHIFT)) & PMU_WAKEUP_PM2_MASK3_ENET_MASK)

#define PMU_WAKEUP_PM2_MASK3_ENET_TIMER_MASK     (0x100000U)
#define PMU_WAKEUP_PM2_MASK3_ENET_TIMER_SHIFT    (20U)
/*! ENET_TIMER - enet_timer Wakeup Mask
 *  0b0..mask enet_timer wakeup interrupt
 *  0b1..unmask enet_timer wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_ENET_TIMER(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_ENET_TIMER_SHIFT)) & PMU_WAKEUP_PM2_MASK3_ENET_TIMER_MASK)

#define PMU_WAKEUP_PM2_MASK3_ITRC_RST_MASK       (0x800000U)
#define PMU_WAKEUP_PM2_MASK3_ITRC_RST_SHIFT      (23U)
/*! ITRC_RST - itrc_rst Wakeup Mask
 *  0b0..mask itrc_rst wakeup interrupt
 *  0b1..unmask itrc_rst wakeup interrupt
 */
#define PMU_WAKEUP_PM2_MASK3_ITRC_RST(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_MASK3_ITRC_RST_SHIFT)) & PMU_WAKEUP_PM2_MASK3_ITRC_RST_MASK)
/*! @} */

/*! @name WAKEUP_PM2_STATUS0 - Wakeup PM2 status Register */
/*! @{ */

#define PMU_WAKEUP_PM2_STATUS0_WDT0_MASK         (0x1U)
#define PMU_WAKEUP_PM2_STATUS0_WDT0_SHIFT        (0U)
/*! WDT0 - WDT0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_WDT0(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_WDT0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_WDT0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_DMA0_MASK         (0x2U)
#define PMU_WAKEUP_PM2_STATUS0_DMA0_SHIFT        (1U)
/*! DMA0 - DMA0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_DMA0(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_DMA0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_DMA0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTA_MASK    (0x4U)
#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTA_SHIFT   (2U)
/*! GPIO_INTA - GPIO_INTA interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTA(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_GPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_GPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTB_MASK    (0x8U)
#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTB_SHIFT   (3U)
/*! GPIO_INTB - GPIO_INTB interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_GPIO_INTB(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_GPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_GPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_STATUS0_PIN_INT0_MASK     (0x10U)
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT0_SHIFT    (4U)
/*! PIN_INT0 - PIN_INT0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT0(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_PIN_INT0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_PIN_INT0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_PIN_INT1_MASK     (0x20U)
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT1_SHIFT    (5U)
/*! PIN_INT1 - PIN_INT1 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT1(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_PIN_INT1_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_PIN_INT1_MASK)

#define PMU_WAKEUP_PM2_STATUS0_PIN_INT2_MASK     (0x40U)
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT2_SHIFT    (6U)
/*! PIN_INT2 - PIN_INT2 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT2(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_PIN_INT2_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_PIN_INT2_MASK)

#define PMU_WAKEUP_PM2_STATUS0_PIN_INT3_MASK     (0x80U)
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT3_SHIFT    (7U)
/*! PIN_INT3 - PIN_INT3 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_PIN_INT3(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_PIN_INT3_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_PIN_INT3_MASK)

#define PMU_WAKEUP_PM2_STATUS0_UTICK_MASK        (0x100U)
#define PMU_WAKEUP_PM2_STATUS0_UTICK_SHIFT       (8U)
/*! UTICK - UTICK interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_UTICK(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_UTICK_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_UTICK_MASK)

#define PMU_WAKEUP_PM2_STATUS0_MRT_MASK          (0x200U)
#define PMU_WAKEUP_PM2_STATUS0_MRT_SHIFT         (9U)
/*! MRT - MRT interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_MRT(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_MRT_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_MRT_MASK)

#define PMU_WAKEUP_PM2_STATUS0_CTIMER0_MASK      (0x400U)
#define PMU_WAKEUP_PM2_STATUS0_CTIMER0_SHIFT     (10U)
/*! CTIMER0 - CTIMER0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_CTIMER0(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_CTIMER0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_CTIMER0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_CTIMER1_MASK      (0x800U)
#define PMU_WAKEUP_PM2_STATUS0_CTIMER1_SHIFT     (11U)
/*! CTIMER1 - CTIMER1 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_CTIMER1(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_CTIMER1_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_CTIMER1_MASK)

#define PMU_WAKEUP_PM2_STATUS0_SCT0_MASK         (0x1000U)
#define PMU_WAKEUP_PM2_STATUS0_SCT0_SHIFT        (12U)
/*! SCT0 - SCT0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_SCT0(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_SCT0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_SCT0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_CTIMER3_MASK      (0x2000U)
#define PMU_WAKEUP_PM2_STATUS0_CTIMER3_SHIFT     (13U)
/*! CTIMER3 - CTIMER3 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_CTIMER3(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_CTIMER3_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_CTIMER3_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM0_MASK    (0x4000U)
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM0_SHIFT   (14U)
/*! FLEXCOMM0 - FLEXCOMM0 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM0(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FLEXCOMM0_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FLEXCOMM0_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM1_MASK    (0x8000U)
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM1_SHIFT   (15U)
/*! FLEXCOMM1 - FLEXCOMM1 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM1(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FLEXCOMM1_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FLEXCOMM1_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM2_MASK    (0x10000U)
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM2_SHIFT   (16U)
/*! FLEXCOMM2 - FLEXCOMM2 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM2(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FLEXCOMM2_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FLEXCOMM2_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM3_MASK    (0x20000U)
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM3_SHIFT   (17U)
/*! FLEXCOMM3 - FLEXCOMM3 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM3(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FLEXCOMM3_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FLEXCOMM3_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM14_MASK   (0x100000U)
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM14_SHIFT  (20U)
/*! FLEXCOMM14 - FLEXCOMM14 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FLEXCOMM14(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FLEXCOMM14_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FLEXCOMM14_MASK)

#define PMU_WAKEUP_PM2_STATUS0_FREEMRT_GLOBAL_MASK (0x800000U)
#define PMU_WAKEUP_PM2_STATUS0_FREEMRT_GLOBAL_SHIFT (23U)
/*! FREEMRT_GLOBAL - Free Multi-rate timer interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_FREEMRT_GLOBAL(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_FREEMRT_GLOBAL_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_FREEMRT_GLOBAL_MASK)

#define PMU_WAKEUP_PM2_STATUS0_DMIC_MASK         (0x2000000U)
#define PMU_WAKEUP_PM2_STATUS0_DMIC_SHIFT        (25U)
/*! DMIC - DMIC interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_DMIC(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_DMIC_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_DMIC_MASK)

#define PMU_WAKEUP_PM2_STATUS0_WAKEUP_FROM_DEEPSLEEP_MASK (0x4000000U)
#define PMU_WAKEUP_PM2_STATUS0_WAKEUP_FROM_DEEPSLEEP_SHIFT (26U)
/*! WAKEUP_FROM_DEEPSLEEP - Wakeup from Deepsleep interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_WAKEUP_FROM_DEEPSLEEP(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_WAKEUP_FROM_DEEPSLEEP_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_WAKEUP_FROM_DEEPSLEEP_MASK)

#define PMU_WAKEUP_PM2_STATUS0_HYPERVISOR_MASK   (0x8000000U)
#define PMU_WAKEUP_PM2_STATUS0_HYPERVISOR_SHIFT  (27U)
/*! HYPERVISOR - HYPERVISOR interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_HYPERVISOR(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_HYPERVISOR_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_HYPERVISOR_MASK)

#define PMU_WAKEUP_PM2_STATUS0_SECUREVIOLATION_MASK (0x10000000U)
#define PMU_WAKEUP_PM2_STATUS0_SECUREVIOLATION_SHIFT (28U)
/*! SECUREVIOLATION - Secure Violation interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_SECUREVIOLATION(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_SECUREVIOLATION_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_SECUREVIOLATION_MASK)

#define PMU_WAKEUP_PM2_STATUS0_HWVAD_MASK        (0x20000000U)
#define PMU_WAKEUP_PM2_STATUS0_HWVAD_SHIFT       (29U)
/*! HWVAD - Hardware Voice Activity Detector interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS0_HWVAD(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS0_HWVAD_SHIFT)) & PMU_WAKEUP_PM2_STATUS0_HWVAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_STATUS1 - Wakeup PM2 status Register */
/*! @{ */

#define PMU_WAKEUP_PM2_STATUS1_RTC_MASK          (0x1U)
#define PMU_WAKEUP_PM2_STATUS1_RTC_SHIFT         (0U)
/*! RTC - rtc interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_RTC(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_RTC_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_RTC_MASK)

#define PMU_WAKEUP_PM2_STATUS1_PIN_INT4_MASK     (0x8U)
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT4_SHIFT    (3U)
/*! PIN_INT4 - PIN_INT4 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT4(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_PIN_INT4_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_PIN_INT4_MASK)

#define PMU_WAKEUP_PM2_STATUS1_PIN_INT5_MASK     (0x10U)
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT5_SHIFT    (4U)
/*! PIN_INT5 - PIN_INT5 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT5(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_PIN_INT5_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_PIN_INT5_MASK)

#define PMU_WAKEUP_PM2_STATUS1_PIN_INT6_MASK     (0x20U)
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT6_SHIFT    (5U)
/*! PIN_INT6 - PIN_INT6 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT6(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_PIN_INT6_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_PIN_INT6_MASK)

#define PMU_WAKEUP_PM2_STATUS1_PIN_INT7_MASK     (0x40U)
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT7_SHIFT    (6U)
/*! PIN_INT7 - PIN_INT7 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_PIN_INT7(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_PIN_INT7_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_PIN_INT7_MASK)

#define PMU_WAKEUP_PM2_STATUS1_CTIMER2_MASK      (0x80U)
#define PMU_WAKEUP_PM2_STATUS1_CTIMER2_SHIFT     (7U)
/*! CTIMER2 - CTIMER2 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_CTIMER2(x)        (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_CTIMER2_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_CTIMER2_MASK)

#define PMU_WAKEUP_PM2_STATUS1_OS_EVENT_TIMER_MASK (0x200U)
#define PMU_WAKEUP_PM2_STATUS1_OS_EVENT_TIMER_SHIFT (9U)
/*! OS_EVENT_TIMER - OS_EVENT_TIMER interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_OS_EVENT_TIMER(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_OS_EVENT_TIMER_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_OS_EVENT_TIMER_MASK)

#define PMU_WAKEUP_PM2_STATUS1_FLEX_SPI_MASK     (0x400U)
#define PMU_WAKEUP_PM2_STATUS1_FLEX_SPI_SHIFT    (10U)
/*! FLEX_SPI - Flex SPI interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_FLEX_SPI(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_FLEX_SPI_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_FLEX_SPI_MASK)

#define PMU_WAKEUP_PM2_STATUS1_SDU_MASK          (0x4000U)
#define PMU_WAKEUP_PM2_STATUS1_SDU_SHIFT         (14U)
/*! SDU - SDU interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_SDU(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_SDU_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_SDU_MASK)

#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTA_MASK   (0x8000U)
#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTA_SHIFT  (15U)
/*! SGPIO_INTA - SGPIO_INTA interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTA(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_SGPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_SGPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTB_MASK   (0x10000U)
#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTB_SHIFT  (16U)
/*! SGPIO_INTB - SGPIO_INTB interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_SGPIO_INTB(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_SGPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_SGPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_STATUS1_USB_MASK          (0x40000U)
#define PMU_WAKEUP_PM2_STATUS1_USB_SHIFT         (18U)
/*! USB - USB interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_USB(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_USB_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_USB_MASK)

#define PMU_WAKEUP_PM2_STATUS1_DMA1_MASK         (0x400000U)
#define PMU_WAKEUP_PM2_STATUS1_DMA1_SHIFT        (22U)
/*! DMA1 - DMA1 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_DMA1(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_DMA1_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_DMA1_MASK)

#define PMU_WAKEUP_PM2_STATUS1_PUF_MASK          (0x800000U)
#define PMU_WAKEUP_PM2_STATUS1_PUF_SHIFT         (23U)
/*! PUF - PUF interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_PUF(x)            (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_PUF_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_PUF_MASK)

#define PMU_WAKEUP_PM2_STATUS1_POWER_QUAD_MASK   (0x1000000U)
#define PMU_WAKEUP_PM2_STATUS1_POWER_QUAD_SHIFT  (24U)
/*! POWER_QUAD - POWER QUAD interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS1_POWER_QUAD(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS1_POWER_QUAD_SHIFT)) & PMU_WAKEUP_PM2_STATUS1_POWER_QUAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_STATUS3 - WAKEUP_PM2_STATUS3 */
/*! @{ */

#define PMU_WAKEUP_PM2_STATUS3_GAU_GPDAC_INT_FUN11_MASK (0x1000U)
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPDAC_INT_FUN11_SHIFT (12U)
/*! GAU_GPDAC_INT_FUN11 - gau_gpdac_int_fun11 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPDAC_INT_FUN11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_GAU_GPDAC_INT_FUN11_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_GAU_GPDAC_INT_FUN11_MASK)

#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_WKUP11_MASK (0x2000U)
#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_WKUP11_SHIFT (13U)
/*! GAU_ACOMP_INT_WKUP11 - gau_acomp_int_wkup11 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_WKUP11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_WKUP11_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_WKUP11_MASK)

#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_FUNC11_MASK (0x4000U)
#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_FUNC11_SHIFT (14U)
/*! GAU_ACOMP_INT_FUNC11 - gau_acomp_int_func11 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_GAU_ACOMP_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC1_INT_FUNC11_MASK (0x8000U)
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC1_INT_FUNC11_SHIFT (15U)
/*! GAU_GPADC1_INT_FUNC11 - gau_gpadc1_int_func11 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC1_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_GAU_GPADC1_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_GAU_GPADC1_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC0_INT_FUNC11_MASK (0x10000U)
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC0_INT_FUNC11_SHIFT (16U)
/*! GAU_GPADC0_INT_FUNC11 - gau_gpadc0_int_func11 interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_GAU_GPADC0_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_GAU_GPADC0_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_GAU_GPADC0_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_STATUS3_USIM_MASK         (0x20000U)
#define PMU_WAKEUP_PM2_STATUS3_USIM_SHIFT        (17U)
/*! USIM - usim interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_USIM(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_USIM_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_USIM_MASK)

#define PMU_WAKEUP_PM2_STATUS3_ENET_MASK         (0x80000U)
#define PMU_WAKEUP_PM2_STATUS3_ENET_SHIFT        (19U)
/*! ENET - enet interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_ENET(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_ENET_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_ENET_MASK)

#define PMU_WAKEUP_PM2_STATUS3_ENET_TIMER_MASK   (0x100000U)
#define PMU_WAKEUP_PM2_STATUS3_ENET_TIMER_SHIFT  (20U)
/*! ENET_TIMER - enet_timer interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_ENET_TIMER(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_ENET_TIMER_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_ENET_TIMER_MASK)

#define PMU_WAKEUP_PM2_STATUS3_ITRC_RST_MASK     (0x800000U)
#define PMU_WAKEUP_PM2_STATUS3_ITRC_RST_SHIFT    (23U)
/*! ITRC_RST - itrc_rst interrupt wakeup status */
#define PMU_WAKEUP_PM2_STATUS3_ITRC_RST(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_STATUS3_ITRC_RST_SHIFT)) & PMU_WAKEUP_PM2_STATUS3_ITRC_RST_MASK)
/*! @} */

/*! @name WAKEUP_PM2_SRC_CLR0 - Wakeup PM2 source clear Register */
/*! @{ */

#define PMU_WAKEUP_PM2_SRC_CLR0_WDT0_MASK        (0x1U)
#define PMU_WAKEUP_PM2_SRC_CLR0_WDT0_SHIFT       (0U)
/*! WDT0 - clear WDT0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_WDT0(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_WDT0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_WDT0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_DMA0_MASK        (0x2U)
#define PMU_WAKEUP_PM2_SRC_CLR0_DMA0_SHIFT       (1U)
/*! DMA0 - clear DMA0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_DMA0(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_DMA0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_DMA0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTA_MASK   (0x4U)
#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTA_SHIFT  (2U)
/*! GPIO_INTA - clear GPIO_INTA interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTA(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTB_MASK   (0x8U)
#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTB_SHIFT  (3U)
/*! GPIO_INTB - clear GPIO_INTB interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTB(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_GPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT0_MASK    (0x10U)
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT0_SHIFT   (4U)
/*! PIN_INT0 - clear PIN_INT0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT0(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT1_MASK    (0x20U)
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT1_SHIFT   (5U)
/*! PIN_INT1 - clear PIN_INT1 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT1(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT1_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT1_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT2_MASK    (0x40U)
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT2_SHIFT   (6U)
/*! PIN_INT2 - clear PIN_INT2 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT2(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT2_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT2_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT3_MASK    (0x80U)
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT3_SHIFT   (7U)
/*! PIN_INT3 - clear PIN_INT3 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT3(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT3_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_PIN_INT3_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_UTICK_MASK       (0x100U)
#define PMU_WAKEUP_PM2_SRC_CLR0_UTICK_SHIFT      (8U)
/*! UTICK - clear UTICK interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_UTICK(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_UTICK_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_UTICK_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_MRT_MASK         (0x200U)
#define PMU_WAKEUP_PM2_SRC_CLR0_MRT_SHIFT        (9U)
/*! MRT - clear MRT interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_MRT(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_MRT_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_MRT_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER0_MASK     (0x400U)
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER0_SHIFT    (10U)
/*! CTIMER0 - clear CTIMER0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER0(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_CTIMER0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_CTIMER0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER1_MASK     (0x800U)
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER1_SHIFT    (11U)
/*! CTIMER1 - clear CTIMER1 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER1(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_CTIMER1_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_CTIMER1_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_SCT0_MASK        (0x1000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_SCT0_SHIFT       (12U)
/*! SCT0 - clear SCT0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_SCT0(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_SCT0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_SCT0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER3_MASK     (0x2000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER3_SHIFT    (13U)
/*! CTIMER3 - clear CTIMER3 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_CTIMER3(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_CTIMER3_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_CTIMER3_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM0_MASK   (0x4000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM0_SHIFT  (14U)
/*! FLEXCOMM0 - clear FLEXCOMM0 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM0(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM0_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM0_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM1_MASK   (0x8000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM1_SHIFT  (15U)
/*! FLEXCOMM1 - clear FLEXCOMM1 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM1(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM1_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM1_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM2_MASK   (0x10000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM2_SHIFT  (16U)
/*! FLEXCOMM2 - clear FLEXCOMM2 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM2(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM2_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM2_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM3_MASK   (0x20000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM3_SHIFT  (17U)
/*! FLEXCOMM3 - clear FLEXCOMM3 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM3(x)     (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM3_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM3_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM14_MASK  (0x100000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM14_SHIFT (20U)
/*! FLEXCOMM14 - clear FLEXCOMM14 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM14(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM14_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FLEXCOMM14_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_FREEMRT_GLOBAL_MASK (0x800000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_FREEMRT_GLOBAL_SHIFT (23U)
/*! FREEMRT_GLOBAL - clear Free Multi-rate timer interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_FREEMRT_GLOBAL(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_FREEMRT_GLOBAL_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_FREEMRT_GLOBAL_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_DMIC_MASK        (0x2000000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_DMIC_SHIFT       (25U)
/*! DMIC - clear DMIC interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_DMIC(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_DMIC_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_DMIC_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_WAKEUP_FROM_DEEPSLEEP_MASK (0x4000000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_WAKEUP_FROM_DEEPSLEEP_SHIFT (26U)
/*! WAKEUP_FROM_DEEPSLEEP - clear Wakeup from Deepsleep interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_WAKEUP_FROM_DEEPSLEEP(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_WAKEUP_FROM_DEEPSLEEP_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_WAKEUP_FROM_DEEPSLEEP_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_HYPERVISOR_MASK  (0x8000000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_HYPERVISOR_SHIFT (27U)
/*! HYPERVISOR - clear HYPERVISOR interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_HYPERVISOR(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_HYPERVISOR_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_HYPERVISOR_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_SECUREVIOLATION_MASK (0x10000000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_SECUREVIOLATION_SHIFT (28U)
/*! SECUREVIOLATION - clear Secure Violation interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_SECUREVIOLATION(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_SECUREVIOLATION_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_SECUREVIOLATION_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR0_HWVAD_MASK       (0x20000000U)
#define PMU_WAKEUP_PM2_SRC_CLR0_HWVAD_SHIFT      (29U)
/*! HWVAD - clear Hardware Voice Activity Detector interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR0_HWVAD(x)         (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR0_HWVAD_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR0_HWVAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_SRC_CLR1 - Wakeup PM2 source clear Register */
/*! @{ */

#define PMU_WAKEUP_PM2_SRC_CLR1_RTC_MASK         (0x1U)
#define PMU_WAKEUP_PM2_SRC_CLR1_RTC_SHIFT        (0U)
/*! RTC - clear rtc interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_RTC(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_RTC_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_RTC_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT4_MASK    (0x8U)
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT4_SHIFT   (3U)
/*! PIN_INT4 - clear PIN_INT4 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT4(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT4_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT4_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT5_MASK    (0x10U)
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT5_SHIFT   (4U)
/*! PIN_INT5 - clear PIN_INT5 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT5(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT5_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT5_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT6_MASK    (0x20U)
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT6_SHIFT   (5U)
/*! PIN_INT6 - clear PIN_INT6 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT6(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT6_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT6_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT7_MASK    (0x40U)
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT7_SHIFT   (6U)
/*! PIN_INT7 - clear PIN_INT7 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT7(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT7_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_PIN_INT7_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_CTIMER2_MASK     (0x80U)
#define PMU_WAKEUP_PM2_SRC_CLR1_CTIMER2_SHIFT    (7U)
/*! CTIMER2 - clear CTIMER2 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_CTIMER2(x)       (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_CTIMER2_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_CTIMER2_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_OS_EVENT_TIMER_MASK (0x200U)
#define PMU_WAKEUP_PM2_SRC_CLR1_OS_EVENT_TIMER_SHIFT (9U)
/*! OS_EVENT_TIMER - clear OS_EVENT_TIMER interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_OS_EVENT_TIMER(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_OS_EVENT_TIMER_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_OS_EVENT_TIMER_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_FLEX_SPI_MASK    (0x400U)
#define PMU_WAKEUP_PM2_SRC_CLR1_FLEX_SPI_SHIFT   (10U)
/*! FLEX_SPI - clear Flex SPI interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_FLEX_SPI(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_FLEX_SPI_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_FLEX_SPI_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_SDU_MASK         (0x4000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_SDU_SHIFT        (14U)
/*! SDU - clear SDU interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_SDU(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_SDU_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_SDU_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTA_MASK  (0x8000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTA_SHIFT (15U)
/*! SGPIO_INTA - clear SGPIO_INTA interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTA(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTA_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTA_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTB_MASK  (0x10000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTB_SHIFT (16U)
/*! SGPIO_INTB - clear SGPIO_INTB interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTB(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTB_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_SGPIO_INTB_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_USB_MASK         (0x40000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_USB_SHIFT        (18U)
/*! USB - clear USB interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_USB(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_USB_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_USB_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_DMA1_MASK        (0x400000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_DMA1_SHIFT       (22U)
/*! DMA1 - clear DMA1 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_DMA1(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_DMA1_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_DMA1_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_PUF_MASK         (0x800000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_PUF_SHIFT        (23U)
/*! PUF - clear PUF interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_PUF(x)           (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_PUF_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_PUF_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR1_POWER_QUAD_MASK  (0x1000000U)
#define PMU_WAKEUP_PM2_SRC_CLR1_POWER_QUAD_SHIFT (24U)
/*! POWER_QUAD - clear POWER QUAD interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR1_POWER_QUAD(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR1_POWER_QUAD_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR1_POWER_QUAD_MASK)
/*! @} */

/*! @name WAKEUP_PM2_SRC_CLR3 - Wakeup PM2 source clear Register */
/*! @{ */

#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPDAC_INT_FUN11_MASK (0x1000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPDAC_INT_FUN11_SHIFT (12U)
/*! GAU_GPDAC_INT_FUN11 - clear gau_gpdac_int_fun11 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPDAC_INT_FUN11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPDAC_INT_FUN11_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPDAC_INT_FUN11_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_WKUP11_MASK (0x2000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_WKUP11_SHIFT (13U)
/*! GAU_ACOMP_INT_WKUP11 - clear gau_acomp_int_wkup11 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_WKUP11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_WKUP11_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_WKUP11_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_FUNC11_MASK (0x4000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_FUNC11_SHIFT (14U)
/*! GAU_ACOMP_INT_FUNC11 - clear gau_acomp_int_func11 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_GAU_ACOMP_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC1_INT_FUNC11_MASK (0x8000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC1_INT_FUNC11_SHIFT (15U)
/*! GAU_GPADC1_INT_FUNC11 - clear gau_gpadc1_int_func11 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC1_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC1_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC1_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC0_INT_FUNC11_MASK (0x10000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC0_INT_FUNC11_SHIFT (16U)
/*! GAU_GPADC0_INT_FUNC11 - clear gau_gpadc0_int_func11 interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC0_INT_FUNC11(x) (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC0_INT_FUNC11_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_GAU_GPADC0_INT_FUNC11_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_USIM_MASK        (0x20000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_USIM_SHIFT       (17U)
/*! USIM - clear usim interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_USIM(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_USIM_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_USIM_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_ENET_MASK        (0x80000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_ENET_SHIFT       (19U)
/*! ENET - clear enet interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_ENET(x)          (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_ENET_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_ENET_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_ENET_TIMER_MASK  (0x100000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_ENET_TIMER_SHIFT (20U)
/*! ENET_TIMER - clear enet_timer interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_ENET_TIMER(x)    (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_ENET_TIMER_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_ENET_TIMER_MASK)

#define PMU_WAKEUP_PM2_SRC_CLR3_ITRC_RST_MASK    (0x800000U)
#define PMU_WAKEUP_PM2_SRC_CLR3_ITRC_RST_SHIFT   (23U)
/*! ITRC_RST - clear itrc_rst interrupt request */
#define PMU_WAKEUP_PM2_SRC_CLR3_ITRC_RST(x)      (((uint32_t)(((uint32_t)(x)) << PMU_WAKEUP_PM2_SRC_CLR3_ITRC_RST_SHIFT)) & PMU_WAKEUP_PM2_SRC_CLR3_ITRC_RST_MASK)
/*! @} */

/*! @name SW_CTRL_WL - WL part-SW Control register bypass HW output */
/*! @{ */

#define PMU_SW_CTRL_WL_WL_EN_MASK                (0x1U)
#define PMU_SW_CTRL_WL_WL_EN_SHIFT               (0U)
/*! WL_EN - WL part-SW bypass pmu HW output enable;1:SW mode;0:HW mode */
#define PMU_SW_CTRL_WL_WL_EN(x)                  (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_WL_EN_SHIFT)) & PMU_SW_CTRL_WL_WL_EN_MASK)

#define PMU_SW_CTRL_WL_MCI_ISO_WL_N_MASK         (0x8U)
#define PMU_SW_CTRL_WL_MCI_ISO_WL_N_SHIFT        (3U)
/*! MCI_ISO_WL_N - MCI_ISO_WL_EN_N, 0:iso enable,assert iso before psw off; 1:iso disable,release iso after psw on; */
#define PMU_SW_CTRL_WL_MCI_ISO_WL_N(x)           (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_MCI_ISO_WL_N_SHIFT)) & PMU_SW_CTRL_WL_MCI_ISO_WL_N_MASK)

#define PMU_SW_CTRL_WL_PSW_WL_PD_MASK            (0x40U)
#define PMU_SW_CTRL_WL_PSW_WL_PD_SHIFT           (6U)
/*! PSW_WL_PD - psw_wl, 0:power on, after request buck on,then delay some time to set psw on;
 *    1:power gated, do it before request buck off
 */
#define PMU_SW_CTRL_WL_PSW_WL_PD(x)              (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_PSW_WL_PD_SHIFT)) & PMU_SW_CTRL_WL_PSW_WL_PD_MASK)

#define PMU_SW_CTRL_WL_MCI_WL_PU_RST_MASK        (0x200U)
#define PMU_SW_CTRL_WL_MCI_WL_PU_RST_SHIFT       (9U)
/*! MCI_WL_PU_RST - mci_wl_pu_rst, 0:reset release; 1:reset assert */
#define PMU_SW_CTRL_WL_MCI_WL_PU_RST(x)          (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_MCI_WL_PU_RST_SHIFT)) & PMU_SW_CTRL_WL_MCI_WL_PU_RST_MASK)

#define PMU_SW_CTRL_WL_WL_BUCK_ON_REQ_MASK       (0x800U)
#define PMU_SW_CTRL_WL_WL_BUCK_ON_REQ_SHIFT      (11U)
/*! WL_BUCK_ON_REQ - wl request buck on,then need wait 5 fast clk_pmu cycles(about 96ns),do psw on, then iso release */
#define PMU_SW_CTRL_WL_WL_BUCK_ON_REQ(x)         (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_WL_BUCK_ON_REQ_SHIFT)) & PMU_SW_CTRL_WL_WL_BUCK_ON_REQ_MASK)

#define PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ_MASK      (0x2000U)
#define PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ_SHIFT     (13U)
/*! WL_BUCK_OFF_REQ - wl request buck off,need to be cfg after iso en, psw pd */
#define PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ(x)        (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ_SHIFT)) & PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ_MASK)

#define PMU_SW_CTRL_WL_SET_WL_SLP_MASK           (0x8000U)
#define PMU_SW_CTRL_WL_SET_WL_SLP_SHIFT          (15U)
/*! SET_WL_SLP - set_wl_slp,provide another slp way, if you don't want use HW slp signal */
#define PMU_SW_CTRL_WL_SET_WL_SLP(x)             (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_WL_SET_WL_SLP_SHIFT)) & PMU_SW_CTRL_WL_SET_WL_SLP_MASK)
/*! @} */

/*! @name SW_CTRL_BLE - BLE part-SW Control register bypass HW output */
/*! @{ */

#define PMU_SW_CTRL_BLE_BLE_EN_MASK              (0x1U)
#define PMU_SW_CTRL_BLE_BLE_EN_SHIFT             (0U)
/*! BLE_EN - BLE part-SW bypass pmu HW output enable;1:SW mode;0:HW mode */
#define PMU_SW_CTRL_BLE_BLE_EN(x)                (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_BLE_EN_SHIFT)) & PMU_SW_CTRL_BLE_BLE_EN_MASK)

#define PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_MASK       (0x8U)
#define PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_SHIFT      (3U)
/*! MCI_ISO_BLE_N - MCI_ISO_BLE_EN_N, 0:iso enable,assert iso before psw off; 1:iso disable,release iso after psw on; */
#define PMU_SW_CTRL_BLE_MCI_ISO_BLE_N(x)         (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_SHIFT)) & PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_MASK)

#define PMU_SW_CTRL_BLE_PSW_BLE_PD_MASK          (0x40U)
#define PMU_SW_CTRL_BLE_PSW_BLE_PD_SHIFT         (6U)
/*! PSW_BLE_PD - psw_ble, 0:power on, after request buck on,then delay some time to set psw on;
 *    1:power gated, do it before request buck off
 */
#define PMU_SW_CTRL_BLE_PSW_BLE_PD(x)            (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_PSW_BLE_PD_SHIFT)) & PMU_SW_CTRL_BLE_PSW_BLE_PD_MASK)

#define PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_MASK      (0x200U)
#define PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_SHIFT     (9U)
/*! MCI_BLE_PU_RST - mci_ble_pu_rst, 0:reset release; 1:reset assert */
#define PMU_SW_CTRL_BLE_MCI_BLE_PU_RST(x)        (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_SHIFT)) & PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_MASK)

#define PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ_MASK     (0x800U)
#define PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ_SHIFT    (11U)
/*! BLE_BUCK_ON_REQ - ble request buck on,then need wait 5 fast clk_pmu cycles(about 96ns), do psw on, then iso release */
#define PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ(x)       (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ_SHIFT)) & PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ_MASK)

#define PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ_MASK    (0x2000U)
#define PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ_SHIFT   (13U)
/*! BLE_BUCK_OFF_REQ - ble request buck off,need to be cfg after iso en, psw pd */
#define PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ(x)      (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ_SHIFT)) & PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ_MASK)

#define PMU_SW_CTRL_BLE_SET_BLE_SLP_MASK         (0x8000U)
#define PMU_SW_CTRL_BLE_SET_BLE_SLP_SHIFT        (15U)
/*! SET_BLE_SLP - set_ble_slp,provide another slp way, if you don't want use HW slp signal */
#define PMU_SW_CTRL_BLE_SET_BLE_SLP(x)           (((uint32_t)(((uint32_t)(x)) << PMU_SW_CTRL_BLE_SET_BLE_SLP_SHIFT)) & PMU_SW_CTRL_BLE_SET_BLE_SLP_MASK)
/*! @} */

/*! @name PSW18_OTP - PSW18 OTP psw control signal */
/*! @{ */

#define PMU_PSW18_OTP_CTRL_MASK                  (0x1U)
#define PMU_PSW18_OTP_CTRL_SHIFT                 (0U)
/*! CTRL - 0:power on; 1:power down */
#define PMU_PSW18_OTP_CTRL(x)                    (((uint32_t)(((uint32_t)(x)) << PMU_PSW18_OTP_CTRL_SHIFT)) & PMU_PSW18_OTP_CTRL_MASK)
/*! @} */

/*! @name TIME_OUT_CTRL - tieme out control signal */
/*! @{ */

#define PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT_MASK    (0x1U)
#define PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT_SHIFT   (0U)
/*! V11_RDY_NO_TMT - v11_rdy use PMIP output/time out generated logic;
 *  0b0..use time out generated, for external supply
 *  0b1..use PMIP output signal, for internal supply-BUCK
 */
#define PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT(x)      (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT_SHIFT)) & PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT_MASK)

#define PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT_MASK    (0x2U)
#define PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT_SHIFT   (1U)
/*! V18_RDY_NO_TMT - v18_rdy use PMIP output/time out generated logic;
 *  0b0..use time out generated, for external supply
 *  0b1..use PMIP output signal, for internal supply-BUCK
 */
#define PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT(x)      (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT_SHIFT)) & PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT_MASK)

#define PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_MASK (0x4U)
#define PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_SHIFT (2U)
/*! PSW_MCI_RDY_NO_TMT - psw_mci_rdy_n use psw output/time out generated logic;
 *  0b0..use time out generated
 *  0b1..use PSW output signal
 */
#define PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT(x)  (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_SHIFT)) & PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_MASK)
/*! @} */

/*! @name TIME_OUT_CFG_VALUE - tieme out configure value */
/*! @{ */

#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_ASRT_MASK (0x1FU)
#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_ASRT_SHIFT (0U)
/*! V11_RDY_ASRT - the time from buck11 on to v11_rdy assert, suggest use the max value 5'h1f */
#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_ASRT(x)   (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_V11_RDY_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_V11_RDY_ASRT_MASK)

#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_DE_ASRT_MASK (0x3E0U)
#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_DE_ASRT_SHIFT (5U)
/*! V11_RDY_DE_ASRT - the time from buck11 off to v11_rdy de-assert, suggest use the max value 5'h1f */
#define PMU_TIME_OUT_CFG_VALUE_V11_RDY_DE_ASRT(x) (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_V11_RDY_DE_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_V11_RDY_DE_ASRT_MASK)

#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_ASRT_MASK (0x7C00U)
#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_ASRT_SHIFT (10U)
/*! V18_RDY_ASRT - the time from buck18 on to v18_rdy assert, suggest use the max value 5'h1f */
#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_ASRT(x)   (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_V18_RDY_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_V18_RDY_ASRT_MASK)

#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_DE_ASRT_MASK (0xF8000U)
#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_DE_ASRT_SHIFT (15U)
/*! V18_RDY_DE_ASRT - the time from buck18 off to v18_rdy de-assert, suggest use the max value 5'h1f */
#define PMU_TIME_OUT_CFG_VALUE_V18_RDY_DE_ASRT(x) (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_V18_RDY_DE_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_V18_RDY_DE_ASRT_MASK)

#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_ASRT_MASK (0x1F00000U)
#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_ASRT_SHIFT (20U)
/*! PSW_MCI_RDY_ASRT - the time from psw_mci on to psw_mci_rdy assert */
#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_ASRT(x) (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_ASRT_MASK)

#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_DE_ASRT_MASK (0x3E000000U)
#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_DE_ASRT_SHIFT (25U)
/*! PSW_MCI_RDY_DE_ASRT - the time from psw_mci off to psw_mci_rdy de-assert */
#define PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_DE_ASRT(x) (((uint32_t)(((uint32_t)(x)) << PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_DE_ASRT_SHIFT)) & PMU_TIME_OUT_CFG_VALUE_PSW_MCI_RDY_DE_ASRT_MASK)
/*! @} */

/*! @name RESERVE_REG0 - reserve R/W regs */
/*! @{ */

#define PMU_RESERVE_REG0_VALUE_MASK              (0xFFFFFFFFU)
#define PMU_RESERVE_REG0_VALUE_SHIFT             (0U)
/*! VALUE - reserve R/W regs */
#define PMU_RESERVE_REG0_VALUE(x)                (((uint32_t)(((uint32_t)(x)) << PMU_RESERVE_REG0_VALUE_SHIFT)) & PMU_RESERVE_REG0_VALUE_MASK)
/*! @} */

/*! @name RESERVE_REG1 - reserve Read only regs */
/*! @{ */

#define PMU_RESERVE_REG1_VALUE_MASK              (0xFFFFFFFFU)
#define PMU_RESERVE_REG1_VALUE_SHIFT             (0U)
/*! VALUE - reserve Read only regs */
#define PMU_RESERVE_REG1_VALUE(x)                (((uint32_t)(((uint32_t)(x)) << PMU_RESERVE_REG1_VALUE_SHIFT)) & PMU_RESERVE_REG1_VALUE_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group PMU_Register_Masks */


/*!
 * @}
 */ /* end of group PMU_Peripheral_Access_Layer */


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


#endif  /* PMU_H_ */

