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
**         CMSIS Peripheral Access Layer for AON_SOC_CIU
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
 * @file AON_SOC_CIU.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for AON_SOC_CIU
 *
 * CMSIS Peripheral Access Layer for AON_SOC_CIU
 */

#if !defined(AON_SOC_CIU_H_)
#define AON_SOC_CIU_H_                           /**< Symbol preventing repeated inclusion */

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
   -- AON_SOC_CIU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AON_SOC_CIU_Peripheral_Access_Layer AON_SOC_CIU Peripheral Access Layer
 * @{
 */

/** AON_SOC_CIU - Register Layout Typedef */
typedef struct {
  __IO uint32_t PAD_CONFIG0;                       /**< ", offset: 0x0 */
       uint8_t RESERVED_0[12];
  __IO uint32_t PAD_PWRDOWN_CTRL2;                 /**< Pad Power-down Control 2, offset: 0x10 */
       uint8_t RESERVED_1[60];
  __IO uint32_t SR_CONFIG1;                        /**< GPIO Slew Rate control, offset: 0x50 */
       uint8_t RESERVED_2[32];
  __IO uint32_t PAD_PU_PD_EN1;                     /**< Pad Pull-up Pull-down Enable2, offset: 0x74 */
       uint8_t RESERVED_3[20];
  __IO uint32_t PAD_SLP_EN0;                       /**< Pad Sleep Mode Enable, offset: 0x8C */
       uint8_t RESERVED_4[8];
  __IO uint32_t PAD_SLP_VAL0;                      /**< Pad Sleep Mode Value, offset: 0x98 */
       uint8_t RESERVED_5[112];
  __IO uint32_t RST_SW;                            /**< Reset Controls for SOC_RESET_GEN, offset: 0x10C */
       uint8_t RESERVED_6[32];
  __I  uint32_t STRAP_FINISH_STATUS;               /**< SOC Strap Finish Status, offset: 0x130 */
       uint8_t RESERVED_7[8];
  __IO uint32_t SOC_OTP_CONTROL;                   /**< Power Switch (1.8v) Control for SOC OTP, offset: 0x13C */
       uint8_t RESERVED_8[196];
  __IO uint32_t PAD_AON_VREG_VSENSOR_CTRL;         /**< AON Vsensor and Vreg Pad Control, offset: 0x204 */
       uint8_t RESERVED_9[632];
  __I  uint32_t STRAP_RDBK;                        /**< Strap Readback, offset: 0x480 */
  __IO uint32_t STRAP_SW;                          /**< Software Strap Override, offset: 0x484 */
       uint8_t RESERVED_10[120];
  __IO uint32_t PAD_SLP_PU_PD_DIS0;                /**< Pad Sleep Pullup and Pulldown Disable1, offset: 0x500 */
       uint8_t RESERVED_11[52];
  __IO uint32_t MCI_IOMUX_EN0;                     /**< mci_iomux_enable control for GPIO[27:22], offset: 0x538 */
} AON_SOC_CIU_Type;

/* ----------------------------------------------------------------------------
   -- AON_SOC_CIU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AON_SOC_CIU_Register_Masks AON_SOC_CIU Register Masks
 * @{
 */

/*! @name PAD_CONFIG0 - " */
/*! @{ */

#define AON_SOC_CIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_MASK (0x2U)
#define AON_SOC_CIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_SHIFT (1U)
/*! PAD_PWRDOWN_LATCH - Enables the pd_sel latching */
#define AON_SOC_CIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_SHIFT)) & AON_SOC_CIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_MASK)

#define AON_SOC_CIU_PAD_CONFIG0_XOSC_OD_EN_MASK  (0x20000U)
#define AON_SOC_CIU_PAD_CONFIG0_XOSC_OD_EN_SHIFT (17U)
/*! XOSC_OD_EN - Crystal Oscillator Enable Output Open-Drain Enable for GPIO[0] */
#define AON_SOC_CIU_PAD_CONFIG0_XOSC_OD_EN(x)    (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_CONFIG0_XOSC_OD_EN_SHIFT)) & AON_SOC_CIU_PAD_CONFIG0_XOSC_OD_EN_MASK)

#define AON_SOC_CIU_PAD_CONFIG0_XOSC_ENA_PAD_SEL_MASK (0x80000000U)
#define AON_SOC_CIU_PAD_CONFIG0_XOSC_ENA_PAD_SEL_SHIFT (31U)
/*! XOSC_ENA_PAD_SEL - xosc_ena_pads selection from CAU or mci_top */
#define AON_SOC_CIU_PAD_CONFIG0_XOSC_ENA_PAD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_CONFIG0_XOSC_ENA_PAD_SEL_SHIFT)) & AON_SOC_CIU_PAD_CONFIG0_XOSC_ENA_PAD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL2 - Pad Power-down Control 2 */
/*! @{ */

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO22_PD_SEL_MASK (0x7U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO22_PD_SEL_SHIFT (0U)
/*! GPIO22_PD_SEL - Power Down Output Value for GPIO[22] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO22_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO22_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO22_PD_SEL_MASK)

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO23_PD_SEL_MASK (0x70U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO23_PD_SEL_SHIFT (4U)
/*! GPIO23_PD_SEL - Power Down Output Value for GPIO[23] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO23_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO23_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO23_PD_SEL_MASK)

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO24_PD_SEL_MASK (0x700U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO24_PD_SEL_SHIFT (8U)
/*! GPIO24_PD_SEL - Power Down Output Value for GPIO[24] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO24_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO24_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO24_PD_SEL_MASK)

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO25_PD_SEL_MASK (0x7000U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO25_PD_SEL_SHIFT (12U)
/*! GPIO25_PD_SEL - Power Down Output Value for GPIO[25] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO25_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO25_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO25_PD_SEL_MASK)

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO26_PD_SEL_MASK (0x70000U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO26_PD_SEL_SHIFT (16U)
/*! GPIO26_PD_SEL - Power Down Output Value for GPIO[26] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO26_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO26_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO26_PD_SEL_MASK)

#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO27_PD_SEL_MASK (0x700000U)
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO27_PD_SEL_SHIFT (20U)
/*! GPIO27_PD_SEL - Power Down Output Value for GPIO[27] Pad */
#define AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO27_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO27_PD_SEL_SHIFT)) & AON_SOC_CIU_PAD_PWRDOWN_CTRL2_GPIO27_PD_SEL_MASK)
/*! @} */

/*! @name SR_CONFIG1 - GPIO Slew Rate control */
/*! @{ */

#define AON_SOC_CIU_SR_CONFIG1_GPIO22_SR_MASK    (0x3000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO22_SR_SHIFT   (12U)
/*! GPIO22_SR - Slew Rate Control for GPIO[22] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO22_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO22_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO22_SR_MASK)

#define AON_SOC_CIU_SR_CONFIG1_GPIO23_SR_MASK    (0xC000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO23_SR_SHIFT   (14U)
/*! GPIO23_SR - Slew Rate Control for GPIO[23] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO23_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO23_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO23_SR_MASK)

#define AON_SOC_CIU_SR_CONFIG1_GPIO24_SR_MASK    (0x30000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO24_SR_SHIFT   (16U)
/*! GPIO24_SR - Slew Rate Control for GPIO[24] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO24_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO24_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO24_SR_MASK)

#define AON_SOC_CIU_SR_CONFIG1_GPIO25_SR_MASK    (0xC0000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO25_SR_SHIFT   (18U)
/*! GPIO25_SR - Slew Rate Control for GPIO[25] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO25_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO25_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO25_SR_MASK)

#define AON_SOC_CIU_SR_CONFIG1_GPIO26_SR_MASK    (0x300000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO26_SR_SHIFT   (20U)
/*! GPIO26_SR - Slew Rate Control for GPIO[26] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO26_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO26_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO26_SR_MASK)

#define AON_SOC_CIU_SR_CONFIG1_GPIO27_SR_MASK    (0xC00000U)
#define AON_SOC_CIU_SR_CONFIG1_GPIO27_SR_SHIFT   (22U)
/*! GPIO27_SR - Slew Rate Control for GPIO[27] */
#define AON_SOC_CIU_SR_CONFIG1_GPIO27_SR(x)      (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SR_CONFIG1_GPIO27_SR_SHIFT)) & AON_SOC_CIU_SR_CONFIG1_GPIO27_SR_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN1 - Pad Pull-up Pull-down Enable2 */
/*! @{ */

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN_MASK (0x3000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN_SHIFT (12U)
/*! GPIO22_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[22] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN_MASK)

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN_MASK (0xC000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN_SHIFT (14U)
/*! GPIO23_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[23] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN_MASK)

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO24_PU_PD_EN_MASK (0x30000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO24_PU_PD_EN_SHIFT (16U)
/*! GPIO24_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[24] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO24_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO24_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO24_PU_PD_EN_MASK)

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO25_PU_PD_EN_MASK (0xC0000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO25_PU_PD_EN_SHIFT (18U)
/*! GPIO25_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[25] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO25_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO25_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO25_PU_PD_EN_MASK)

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO26_PU_PD_EN_MASK (0x300000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO26_PU_PD_EN_SHIFT (20U)
/*! GPIO26_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[26] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO26_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO26_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO26_PU_PD_EN_MASK)

#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO27_PU_PD_EN_MASK (0xC00000U)
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO27_PU_PD_EN_SHIFT (22U)
/*! GPIO27_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[27] */
#define AON_SOC_CIU_PAD_PU_PD_EN1_GPIO27_PU_PD_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_PU_PD_EN1_GPIO27_PU_PD_EN_SHIFT)) & AON_SOC_CIU_PAD_PU_PD_EN1_GPIO27_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_SLP_EN0 - Pad Sleep Mode Enable */
/*! @{ */

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO22_SLP_EN_MASK (0x400000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO22_SLP_EN_SHIFT (22U)
/*! GPIO22_SLP_EN - Enable Forcing GPIO[22] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO22_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO22_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO22_SLP_EN_MASK)

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO23_SLP_EN_MASK (0x800000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO23_SLP_EN_SHIFT (23U)
/*! GPIO23_SLP_EN - Enable Forcing GPIO[23] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO23_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO23_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO23_SLP_EN_MASK)

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO24_SLP_EN_MASK (0x1000000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO24_SLP_EN_SHIFT (24U)
/*! GPIO24_SLP_EN - Enable Forcing GPIO[24] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO24_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO24_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO24_SLP_EN_MASK)

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO25_SLP_EN_MASK (0x2000000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO25_SLP_EN_SHIFT (25U)
/*! GPIO25_SLP_EN - Enable Forcing GPIO[25] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO25_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO25_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO25_SLP_EN_MASK)

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO26_SLP_EN_MASK (0x4000000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO26_SLP_EN_SHIFT (26U)
/*! GPIO26_SLP_EN - Enable Forcing GPIO[26] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO26_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO26_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO26_SLP_EN_MASK)

#define AON_SOC_CIU_PAD_SLP_EN0_GPIO27_SLP_EN_MASK (0x8000000U)
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO27_SLP_EN_SHIFT (27U)
/*! GPIO27_SLP_EN - Enable Forcing GPIO[27] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_EN0_GPIO27_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_EN0_GPIO27_SLP_EN_SHIFT)) & AON_SOC_CIU_PAD_SLP_EN0_GPIO27_SLP_EN_MASK)
/*! @} */

/*! @name PAD_SLP_VAL0 - Pad Sleep Mode Value */
/*! @{ */

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO22_SLP_VAL_MASK (0x400000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO22_SLP_VAL_SHIFT (22U)
/*! GPIO22_SLP_VAL - Force GPIO[22] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO22_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO22_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO22_SLP_VAL_MASK)

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO23_SLP_VAL_MASK (0x800000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO23_SLP_VAL_SHIFT (23U)
/*! GPIO23_SLP_VAL - Force GPIO[23] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO23_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO23_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO23_SLP_VAL_MASK)

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO24_SLP_VAL_MASK (0x1000000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO24_SLP_VAL_SHIFT (24U)
/*! GPIO24_SLP_VAL - Force GPIO[24] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO24_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO24_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO24_SLP_VAL_MASK)

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO25_SLP_VAL_MASK (0x2000000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO25_SLP_VAL_SHIFT (25U)
/*! GPIO25_SLP_VAL - Force GPIO[25] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO25_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO25_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO25_SLP_VAL_MASK)

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO26_SLP_VAL_MASK (0x4000000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO26_SLP_VAL_SHIFT (26U)
/*! GPIO26_SLP_VAL - Force GPIO[26] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO26_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO26_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO26_SLP_VAL_MASK)

#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO27_SLP_VAL_MASK (0x8000000U)
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO27_SLP_VAL_SHIFT (27U)
/*! GPIO27_SLP_VAL - Force GPIO[27] Output During Sleep */
#define AON_SOC_CIU_PAD_SLP_VAL0_GPIO27_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_VAL0_GPIO27_SLP_VAL_SHIFT)) & AON_SOC_CIU_PAD_SLP_VAL0_GPIO27_SLP_VAL_MASK)
/*! @} */

/*! @name RST_SW - Reset Controls for SOC_RESET_GEN */
/*! @{ */

#define AON_SOC_CIU_RST_SW_BUCK_REG_RST_MASK     (0x200U)
#define AON_SOC_CIU_RST_SW_BUCK_REG_RST_SHIFT    (9U)
/*! BUCK_REG_RST - config reg SW reset for BUCK18 and BUCK11, active high */
#define AON_SOC_CIU_RST_SW_BUCK_REG_RST(x)       (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_RST_SW_BUCK_REG_RST_SHIFT)) & AON_SOC_CIU_RST_SW_BUCK_REG_RST_MASK)

#define AON_SOC_CIU_RST_SW_SOCAON_ITRC_CHIP_RSTB_EN_MASK (0x1000000U)
#define AON_SOC_CIU_RST_SW_SOCAON_ITRC_CHIP_RSTB_EN_SHIFT (24U)
/*! SOCAON_ITRC_CHIP_RSTB_EN - Default the itrc chip reset can reset SOCAON (jtag), SW can disable this if not required */
#define AON_SOC_CIU_RST_SW_SOCAON_ITRC_CHIP_RSTB_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_RST_SW_SOCAON_ITRC_CHIP_RSTB_EN_SHIFT)) & AON_SOC_CIU_RST_SW_SOCAON_ITRC_CHIP_RSTB_EN_MASK)

#define AON_SOC_CIU_RST_SW_AON_SOCCIU_RSTB_MASK  (0x2000000U)
#define AON_SOC_CIU_RST_SW_AON_SOCCIU_RSTB_SHIFT (25U)
/*! AON_SOCCIU_RSTB - reset for the aon socciu registers. It gets auto clear */
#define AON_SOC_CIU_RST_SW_AON_SOCCIU_RSTB(x)    (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_RST_SW_AON_SOCCIU_RSTB_SHIFT)) & AON_SOC_CIU_RST_SW_AON_SOCCIU_RSTB_MASK)
/*! @} */

/*! @name STRAP_FINISH_STATUS - SOC Strap Finish Status */
/*! @{ */

#define AON_SOC_CIU_STRAP_FINISH_STATUS_STRAP_FINISH_MASK (0x1U)
#define AON_SOC_CIU_STRAP_FINISH_STATUS_STRAP_FINISH_SHIFT (0U)
/*! STRAP_FINISH - Strap Finish status from strap logic. */
#define AON_SOC_CIU_STRAP_FINISH_STATUS_STRAP_FINISH(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_FINISH_STATUS_STRAP_FINISH_SHIFT)) & AON_SOC_CIU_STRAP_FINISH_STATUS_STRAP_FINISH_MASK)
/*! @} */

/*! @name SOC_OTP_CONTROL - Power Switch (1.8v) Control for SOC OTP */
/*! @{ */

#define AON_SOC_CIU_SOC_OTP_CONTROL_SOC_OTP_PSW18_PD_MASK (0x1U)
#define AON_SOC_CIU_SOC_OTP_CONTROL_SOC_OTP_PSW18_PD_SHIFT (0U)
/*! SOC_OTP_PSW18_PD - Power-Down control for the 1.8V Power-Switch for OTPs on SOC side */
#define AON_SOC_CIU_SOC_OTP_CONTROL_SOC_OTP_PSW18_PD(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_SOC_OTP_CONTROL_SOC_OTP_PSW18_PD_SHIFT)) & AON_SOC_CIU_SOC_OTP_CONTROL_SOC_OTP_PSW18_PD_MASK)
/*! @} */

/*! @name PAD_AON_VREG_VSENSOR_CTRL - AON Vsensor and Vreg Pad Control */
/*! @{ */

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK (0x1U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT (0U)
/*! VIO_REG_ENB - VIO_AON Pad Regulator */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_ENB(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_MASK (0x2U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_SHIFT (1U)
/*! VIO_REG_CTRL_EN - VIO reg control enable function */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK (0x10U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT (4U)
/*! VSENSOR_BYPASS - Active High Enable Signal for Bypass Mode */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK (0x20U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT (5U)
/*! VSENSOR_V18EN_12_IN - Bypass Value when Vsensor_Bypass Bit Set */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK (0x40U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT (6U)
/*! VSENSOR_DISABLE_12 - Vsensor AON disable */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK (0x80U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT (7U)
/*! VSENSOR_CLK_12 - Vsensor AON Clock */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_CLK_12(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK (0x100U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT (8U)
/*! VSENSOR_TE - Vsensor AON Test Enable */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TE(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK (0xE00U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT (9U)
/*! VSENSOR_TEST - Vsensor AON Test Point Mux Selection */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TEST(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK (0x1000U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT (12U)
/*! VSENSOR_VTHRESH - Vsensor AON Detection Threshold */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_V25EN_CORE_MASK (0x2000U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT (13U)
/*! V25EN_CORE - V25EN_CORE */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_V25EN_CORE(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_V25EN_CORE_MASK)

#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK (0x4000U)
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT (14U)
/*! VSENSOR_DETECT - VIO_AON_Vsensor_Detect_V18 Status */
#define AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT)) & AON_SOC_CIU_PAD_AON_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK)
/*! @} */

/*! @name STRAP_RDBK - Strap Readback */
/*! @{ */

#define AON_SOC_CIU_STRAP_RDBK_MODE_MASK         (0xFU)
#define AON_SOC_CIU_STRAP_RDBK_MODE_SHIFT        (0U)
/*! MODE - Boot Options: */
#define AON_SOC_CIU_STRAP_RDBK_MODE(x)           (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_MODE_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_MODE_MASK)

#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_DETECT_MASK (0x10U)
#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_DETECT_SHIFT (4U)
/*! REF_CLK_DETECT - ref_clk_detect (reserved) */
#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_DETECT(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_REF_CLK_DETECT_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_REF_CLK_DETECT_MASK)

#define AON_SOC_CIU_STRAP_RDBK_XOSC_SEL_MASK     (0x60U)
#define AON_SOC_CIU_STRAP_RDBK_XOSC_SEL_SHIFT    (5U)
/*! XOSC_SEL - Crystal Osc Enable */
#define AON_SOC_CIU_STRAP_RDBK_XOSC_SEL(x)       (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_XOSC_SEL_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_XOSC_SEL_MASK)

#define AON_SOC_CIU_STRAP_RDBK_VTOR_SEL_MASK     (0x80U)
#define AON_SOC_CIU_STRAP_RDBK_VTOR_SEL_SHIFT    (7U)
/*! VTOR_SEL - Strap bit to select secure VTOR base addr of CM33 */
#define AON_SOC_CIU_STRAP_RDBK_VTOR_SEL(x)       (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_VTOR_SEL_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_VTOR_SEL_MASK)

#define AON_SOC_CIU_STRAP_RDBK_DIS_KEY_ROT_DBG_MASK (0x100U)
#define AON_SOC_CIU_STRAP_RDBK_DIS_KEY_ROT_DBG_SHIFT (8U)
/*! DIS_KEY_ROT_DBG - dis_key_rot_dbg */
#define AON_SOC_CIU_STRAP_RDBK_DIS_KEY_ROT_DBG(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_DIS_KEY_ROT_DBG_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_DIS_KEY_ROT_DBG_MASK)

#define AON_SOC_CIU_STRAP_RDBK_DAP_USE_JTAG_MASK (0x800U)
#define AON_SOC_CIU_STRAP_RDBK_DAP_USE_JTAG_SHIFT (11U)
/*! DAP_USE_JTAG - 1 (Default): DAP uses JTAG */
#define AON_SOC_CIU_STRAP_RDBK_DAP_USE_JTAG(x)   (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_DAP_USE_JTAG_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_DAP_USE_JTAG_MASK)

#define AON_SOC_CIU_STRAP_RDBK_QFN_BOND_MASK     (0x4000U)
#define AON_SOC_CIU_STRAP_RDBK_QFN_BOND_SHIFT    (14U)
/*! QFN_BOND - Bond Strap Value */
#define AON_SOC_CIU_STRAP_RDBK_QFN_BOND(x)       (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_QFN_BOND_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_QFN_BOND_MASK)

#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_RATE_MASK (0xF00000U)
#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_RATE_SHIFT (20U)
/*! REF_CLK_RATE - CAU Reference Clock Rate */
#define AON_SOC_CIU_STRAP_RDBK_REF_CLK_RATE(x)   (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_RDBK_REF_CLK_RATE_SHIFT)) & AON_SOC_CIU_STRAP_RDBK_REF_CLK_RATE_MASK)
/*! @} */

/*! @name STRAP_SW - Software Strap Override */
/*! @{ */

#define AON_SOC_CIU_STRAP_SW_MODE_MASK           (0xFU)
#define AON_SOC_CIU_STRAP_SW_MODE_SHIFT          (0U)
/*! MODE - Below are the modes selected based on the value of this field: */
#define AON_SOC_CIU_STRAP_SW_MODE(x)             (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_SW_MODE_SHIFT)) & AON_SOC_CIU_STRAP_SW_MODE_MASK)

#define AON_SOC_CIU_STRAP_SW_XOSC_SEL_MASK       (0x60U)
#define AON_SOC_CIU_STRAP_SW_XOSC_SEL_SHIFT      (5U)
/*! XOSC_SEL - Crystal Osc Enable */
#define AON_SOC_CIU_STRAP_SW_XOSC_SEL(x)         (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_SW_XOSC_SEL_SHIFT)) & AON_SOC_CIU_STRAP_SW_XOSC_SEL_MASK)

#define AON_SOC_CIU_STRAP_SW_QFN_BOND_MASK       (0x4000U)
#define AON_SOC_CIU_STRAP_SW_QFN_BOND_SHIFT      (14U)
/*! QFN_BOND - Bond Type */
#define AON_SOC_CIU_STRAP_SW_QFN_BOND(x)         (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_SW_QFN_BOND_SHIFT)) & AON_SOC_CIU_STRAP_SW_QFN_BOND_MASK)

#define AON_SOC_CIU_STRAP_SW_REF_CLK_RATE_MASK   (0xF00000U)
#define AON_SOC_CIU_STRAP_SW_REF_CLK_RATE_SHIFT  (20U)
/*! REF_CLK_RATE - CAU Reference Clock Rate */
#define AON_SOC_CIU_STRAP_SW_REF_CLK_RATE(x)     (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_SW_REF_CLK_RATE_SHIFT)) & AON_SOC_CIU_STRAP_SW_REF_CLK_RATE_MASK)

#define AON_SOC_CIU_STRAP_SW_ENABLE_MASK         (0x80000000U)
#define AON_SOC_CIU_STRAP_SW_ENABLE_SHIFT        (31U)
/*! ENABLE - Software Strap Mode Enable */
#define AON_SOC_CIU_STRAP_SW_ENABLE(x)           (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_STRAP_SW_ENABLE_SHIFT)) & AON_SOC_CIU_STRAP_SW_ENABLE_MASK)
/*! @} */

/*! @name PAD_SLP_PU_PD_DIS0 - Pad Sleep Pullup and Pulldown Disable1 */
/*! @{ */

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO22_PU_PD_DIS_MASK (0x400000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO22_PU_PD_DIS_SHIFT (22U)
/*! GPIO22_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[22] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO22_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO22_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO22_PU_PD_DIS_MASK)

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO23_PU_PD_DIS_MASK (0x800000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO23_PU_PD_DIS_SHIFT (23U)
/*! GPIO23_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[23] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO23_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO23_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO23_PU_PD_DIS_MASK)

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO24_PU_PD_DIS_MASK (0x1000000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO24_PU_PD_DIS_SHIFT (24U)
/*! GPIO24_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[24] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO24_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO24_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO24_PU_PD_DIS_MASK)

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO25_PU_PD_DIS_MASK (0x2000000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO25_PU_PD_DIS_SHIFT (25U)
/*! GPIO25_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[25] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO25_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO25_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO25_PU_PD_DIS_MASK)

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO26_PU_PD_DIS_MASK (0x4000000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO26_PU_PD_DIS_SHIFT (26U)
/*! GPIO26_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[26] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO26_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO26_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO26_PU_PD_DIS_MASK)

#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO27_PU_PD_DIS_MASK (0x8000000U)
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO27_PU_PD_DIS_SHIFT (27U)
/*! GPIO27_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[27] During Sleep Mode */
#define AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO27_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO27_PU_PD_DIS_SHIFT)) & AON_SOC_CIU_PAD_SLP_PU_PD_DIS0_GPIO27_PU_PD_DIS_MASK)
/*! @} */

/*! @name MCI_IOMUX_EN0 - mci_iomux_enable control for GPIO[27:22] */
/*! @{ */

#define AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22_MASK  (0xFC00000U)
#define AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22_SHIFT (22U)
/*! EN_27_22 - Bitwise enable control for mci_io_mux GPIO[27:22] */
#define AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22(x)    (((uint32_t)(((uint32_t)(x)) << AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22_SHIFT)) & AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group AON_SOC_CIU_Register_Masks */


/*!
 * @}
 */ /* end of group AON_SOC_CIU_Peripheral_Access_Layer */


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


#endif  /* AON_SOC_CIU_H_ */

