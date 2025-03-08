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
**         CMSIS Peripheral Access Layer for SYSCTL0
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
 * @file SYSCTL0.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SYSCTL0
 *
 * CMSIS Peripheral Access Layer for SYSCTL0
 */

#if !defined(SYSCTL0_H_)
#define SYSCTL0_H_                               /**< Symbol preventing repeated inclusion */

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
   -- SYSCTL0 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL0_Peripheral_Access_Layer SYSCTL0 Peripheral Access Layer
 * @{
 */

/** SYSCTL0 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t AHBMATRIXPRIOR;                    /**< AHB matrix priority, offset: 0x10 */
       uint8_t RESERVED_1[28];
  __IO uint32_t M33NMISRCSEL;                      /**< M33 nmi source selection, offset: 0x30 */
  __IO uint32_t SYSTEM_STICK_CALIB;                /**< system stick calibration, offset: 0x34 */
  __IO uint32_t SYSTEM_NSTICK_CALIB;               /**< system nstick calibration, offset: 0x38 */
       uint8_t RESERVED_2[68];
  __IO uint32_t AUTOCLKGATEOVERRIDE0;              /**< auto clock gating override 0, offset: 0x80 */
  __IO uint32_t AUTOCLKGATEOVERRIDE1;              /**< auto clock gating override 1, offset: 0x84 */
       uint8_t RESERVED_3[904];
  __I  uint32_t USBCLKSTAT;                        /**< USB clock status, offset: 0x410 */
       uint8_t RESERVED_4[876];
  __IO uint32_t HWWAKE;                            /**< offset: 0x780 */
       uint8_t RESERVED_5[1692];
  __IO uint32_t ROM_HIDING_ADDR_OFFSET;            /**< rom_hiding_addr_offset, offset: 0xE20 */
  __IO uint32_t ROM_HIDING_ADDR_OFFSET_DP;         /**< rom_hiding_addr_offset_dp, offset: 0xE24 */
  __IO uint32_t ROM_HIDING_LOCK;                   /**< rom_hiding_lock, offset: 0xE28 */
} SYSCTL0_Type;

/* ----------------------------------------------------------------------------
   -- SYSCTL0 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL0_Register_Masks SYSCTL0 Register Masks
 * @{
 */

/*! @name AHBMATRIXPRIOR - AHB matrix priority */
/*! @{ */

#define SYSCTL0_AHBMATRIXPRIOR_M0_MASK           (0x3U)
#define SYSCTL0_AHBMATRIXPRIOR_M0_SHIFT          (0U)
/*! M0 - Master 0 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. (0 Low) */
#define SYSCTL0_AHBMATRIXPRIOR_M0(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M0_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M0_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M1_MASK           (0xCU)
#define SYSCTL0_AHBMATRIXPRIOR_M1_SHIFT          (2U)
/*! M1 - Master 1 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M1(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M1_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M1_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M2_MASK           (0x30U)
#define SYSCTL0_AHBMATRIXPRIOR_M2_SHIFT          (4U)
/*! M2 - Master 2 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M2(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M2_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M2_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M3_MASK           (0xC0U)
#define SYSCTL0_AHBMATRIXPRIOR_M3_SHIFT          (6U)
/*! M3 - Master 3 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M3(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M3_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M3_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M4_MASK           (0x300U)
#define SYSCTL0_AHBMATRIXPRIOR_M4_SHIFT          (8U)
/*! M4 - Master 4 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M4(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M4_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M4_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M5_MASK           (0xC00U)
#define SYSCTL0_AHBMATRIXPRIOR_M5_SHIFT          (10U)
/*! M5 - Master 5 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M5(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M5_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M5_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M6_MASK           (0x3000U)
#define SYSCTL0_AHBMATRIXPRIOR_M6_SHIFT          (12U)
/*! M6 - Master 6 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M6(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M6_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M6_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M7_MASK           (0xC000U)
#define SYSCTL0_AHBMATRIXPRIOR_M7_SHIFT          (14U)
/*! M7 - Master 7 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M7(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M7_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M7_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M8_MASK           (0x30000U)
#define SYSCTL0_AHBMATRIXPRIOR_M8_SHIFT          (16U)
/*! M8 - Master 8 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M8(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M8_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M8_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M9_MASK           (0xC0000U)
#define SYSCTL0_AHBMATRIXPRIOR_M9_SHIFT          (18U)
/*! M9 - Master 9 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M9(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M9_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M9_MASK)

#define SYSCTL0_AHBMATRIXPRIOR_M10_MASK          (0x300000U)
#define SYSCTL0_AHBMATRIXPRIOR_M10_SHIFT         (20U)
/*! M10 - Master 10 Priority. . . 0: 0, 1: 1, 2: 2, 3: 3. */
#define SYSCTL0_AHBMATRIXPRIOR_M10(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AHBMATRIXPRIOR_M10_SHIFT)) & SYSCTL0_AHBMATRIXPRIOR_M10_MASK)
/*! @} */

/*! @name M33NMISRCSEL - M33 nmi source selection */
/*! @{ */

#define SYSCTL0_M33NMISRCSEL_NMISRCSEL_MASK      (0x7FU)
#define SYSCTL0_M33NMISRCSEL_NMISRCSEL_SHIFT     (0U)
/*! NMISRCSEL - Selects one of the M33 interrupt sources as the NMI source. See M33 Interrupt Slot Table for Interrupt Slot Numers. */
#define SYSCTL0_M33NMISRCSEL_NMISRCSEL(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL0_M33NMISRCSEL_NMISRCSEL_SHIFT)) & SYSCTL0_M33NMISRCSEL_NMISRCSEL_MASK)

#define SYSCTL0_M33NMISRCSEL_NMIEN_MASK          (0x80000000U)
#define SYSCTL0_M33NMISRCSEL_NMIEN_SHIFT         (31U)
/*! NMIEN - NMI interrupt enable
 *  0b0..Disable NMI Interrupt
 *  0b1..Enable NMI Interrupt.
 */
#define SYSCTL0_M33NMISRCSEL_NMIEN(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL0_M33NMISRCSEL_NMIEN_SHIFT)) & SYSCTL0_M33NMISRCSEL_NMIEN_MASK)
/*! @} */

/*! @name SYSTEM_STICK_CALIB - system stick calibration */
/*! @{ */

#define SYSCTL0_SYSTEM_STICK_CALIB_SYSTEM_STICK_CALIB_MASK (0x3FFFFFFU)
#define SYSCTL0_SYSTEM_STICK_CALIB_SYSTEM_STICK_CALIB_SHIFT (0U)
/*! SYSTEM_STICK_CALIB - Selects the system secure tick calibration value of the M33. */
#define SYSCTL0_SYSTEM_STICK_CALIB_SYSTEM_STICK_CALIB(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_SYSTEM_STICK_CALIB_SYSTEM_STICK_CALIB_SHIFT)) & SYSCTL0_SYSTEM_STICK_CALIB_SYSTEM_STICK_CALIB_MASK)
/*! @} */

/*! @name SYSTEM_NSTICK_CALIB - system nstick calibration */
/*! @{ */

#define SYSCTL0_SYSTEM_NSTICK_CALIB_SYSTEM_NSTICK_CALIB_MASK (0x3FFFFFFU)
#define SYSCTL0_SYSTEM_NSTICK_CALIB_SYSTEM_NSTICK_CALIB_SHIFT (0U)
/*! SYSTEM_NSTICK_CALIB - Selects the system non-secure tick calibration value of the M33. */
#define SYSCTL0_SYSTEM_NSTICK_CALIB_SYSTEM_NSTICK_CALIB(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_SYSTEM_NSTICK_CALIB_SYSTEM_NSTICK_CALIB_SHIFT)) & SYSCTL0_SYSTEM_NSTICK_CALIB_SYSTEM_NSTICK_CALIB_MASK)
/*! @} */

/*! @name AUTOCLKGATEOVERRIDE0 - auto clock gating override 0 */
/*! @{ */

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB0_MASK (0x1U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB0_SHIFT (0U)
/*! AHB2APB0 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB0_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB0_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB1_MASK (0x2U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB1_SHIFT (1U)
/*! AHB2APB1 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB1(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB1_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_AHB2APB1_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_CRC_ENGINER_MASK (0x4U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_CRC_ENGINER_SHIFT (2U)
/*! CRC_ENGINER - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_CRC_ENGINER(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_CRC_ENGINER_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_CRC_ENGINER_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC0_MASK  (0x10U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC0_SHIFT (4U)
/*! DMAC0 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC0(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC0_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC0_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC1_MASK  (0x20U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC1_SHIFT (5U)
/*! DMAC1 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC1(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC1_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_DMAC1_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE0_SYSCTL_REGBANK_MASK (0x8000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_SYSCTL_REGBANK_SHIFT (15U)
/*! SYSCTL_REGBANK - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE0_SYSCTL_REGBANK(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE0_SYSCTL_REGBANK_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE0_SYSCTL_REGBANK_MASK)
/*! @} */

/*! @name AUTOCLKGATEOVERRIDE1 - auto clock gating override 1 */
/*! @{ */

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF0_MASK (0x1U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF0_SHIFT (0U)
/*! SRAM_IF0 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF0_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF0_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF1_MASK (0x2U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF1_SHIFT (1U)
/*! SRAM_IF1 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF1(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF1_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF1_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF2_MASK (0x4U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF2_SHIFT (2U)
/*! SRAM_IF2 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF2_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF2_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF3_MASK (0x8U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF3_SHIFT (3U)
/*! SRAM_IF3 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF3(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF3_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF3_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF4_MASK (0x10U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF4_SHIFT (4U)
/*! SRAM_IF4 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF4(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF4_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF4_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF5_MASK (0x20U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF5_SHIFT (5U)
/*! SRAM_IF5 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF5(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF5_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF5_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF6_MASK (0x40U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF6_SHIFT (6U)
/*! SRAM_IF6 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF6(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF6_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF6_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF7_MASK (0x80U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF7_SHIFT (7U)
/*! SRAM_IF7 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF7(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF7_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF7_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF8_MASK (0x100U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF8_SHIFT (8U)
/*! SRAM_IF8 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF8(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF8_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF8_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF9_MASK (0x200U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF9_SHIFT (9U)
/*! SRAM_IF9 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF9(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF9_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF9_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF10_MASK (0x400U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF10_SHIFT (10U)
/*! SRAM_IF10 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF10(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF10_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF10_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF11_MASK (0x800U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF11_SHIFT (11U)
/*! SRAM_IF11 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF11(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF11_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF11_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF12_MASK (0x1000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF12_SHIFT (12U)
/*! SRAM_IF12 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF12(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF12_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF12_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF13_MASK (0x2000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF13_SHIFT (13U)
/*! SRAM_IF13 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF13(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF13_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF13_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF14_MASK (0x4000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF14_SHIFT (14U)
/*! SRAM_IF14 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF14(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF14_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF14_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF15_MASK (0x8000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF15_SHIFT (15U)
/*! SRAM_IF15 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF15(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF15_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF15_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF16_MASK (0x10000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF16_SHIFT (16U)
/*! SRAM_IF16 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF16(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF16_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF16_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF17_MASK (0x20000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF17_SHIFT (17U)
/*! SRAM_IF17 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF17(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF17_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF17_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF18_MASK (0x40000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF18_SHIFT (18U)
/*! SRAM_IF18 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF18(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF18_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_SRAM_IF18_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM0_MASK (0x40000000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM0_SHIFT (30U)
/*! AON_MEM0 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM0_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM0_MASK)

#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM1_MASK (0x80000000U)
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM1_SHIFT (31U)
/*! AON_MEM1 - auto clock gating enable
 *  0b0..Enable Auto-Clk Gate
 *  0b1..Disable Auto-Clk Gate (clocks always running)
 */
#define SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM1(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM1_SHIFT)) & SYSCTL0_AUTOCLKGATEOVERRIDE1_AON_MEM1_MASK)
/*! @} */

/*! @name USBCLKSTAT - USB clock status */
/*! @{ */

#define SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST_MASK   (0x1U)
#define SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST_SHIFT  (0U)
/*! DEV_NEED_CLKST - USB0 Device USB0_NEEDCLK signal status:
 *  0b0..low
 *  0b1..high
 */
#define SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST_SHIFT)) & SYSCTL0_USBCLKSTAT_DEV_NEED_CLKST_MASK)

#define SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_MASK  (0x2U)
#define SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_SHIFT (1U)
/*! HOST_NEED_CLKST - USB0 Device Host USB0_NEEDCLK signal status:
 *  0b0..low
 *  0b1..high
 */
#define SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_SHIFT)) & SYSCTL0_USBCLKSTAT_HOST_NEED_CLKST_MASK)
/*! @} */

/*! @name HWWAKE -  */
/*! @{ */

#define SYSCTL0_HWWAKE_FORCEWAKE_MASK            (0x1U)
#define SYSCTL0_HWWAKE_FORCEWAKE_SHIFT           (0U)
/*! FORCEWAKE - Force peripheral clocking to stay on during deep-sleep mode. When 1, clocking to
 *    peripherals is prevented from being shut down when the CPU enters deep-sleep mode. This is
 *    intended to allow a coprocessor to continue operating while the main CPU(s) are shut down.
 */
#define SYSCTL0_HWWAKE_FORCEWAKE(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL0_HWWAKE_FORCEWAKE_SHIFT)) & SYSCTL0_HWWAKE_FORCEWAKE_MASK)

#define SYSCTL0_HWWAKE_FCWAKE_MASK               (0x2U)
#define SYSCTL0_HWWAKE_FCWAKE_SHIFT              (1U)
/*! FCWAKE - Wake for Flexcomm Interfaces. When 1, any Flexcomm Interface FIFO reaching the level
 *    specified by its own TXLVL will cause peripheral clocking to wake up temporarily while the
 *    related status is asserted.
 */
#define SYSCTL0_HWWAKE_FCWAKE(x)                 (((uint32_t)(((uint32_t)(x)) << SYSCTL0_HWWAKE_FCWAKE_SHIFT)) & SYSCTL0_HWWAKE_FCWAKE_MASK)

#define SYSCTL0_HWWAKE_DMICWAKE_MASK             (0x4U)
#define SYSCTL0_HWWAKE_DMICWAKE_SHIFT            (2U)
/*! DMICWAKE - Wake for Digital Microphone. When 1, the digital microphone input FIFO reaching the
 *    level specified by TRIGLVL of either channel will cause peripheral clocking to wake up
 *    temporarily while the related status is asserted.
 */
#define SYSCTL0_HWWAKE_DMICWAKE(x)               (((uint32_t)(((uint32_t)(x)) << SYSCTL0_HWWAKE_DMICWAKE_SHIFT)) & SYSCTL0_HWWAKE_DMICWAKE_MASK)

#define SYSCTL0_HWWAKE_DMAC0WAKE_MASK            (0x8U)
#define SYSCTL0_HWWAKE_DMAC0WAKE_SHIFT           (3U)
/*! DMAC0WAKE - Wake for DMAC0. When 1, DMAC0 being busy will cause peripheral clocking to remain
 *    running until DMAC0 completes. This is generally used in conjunction with bit 1 and/or 2 in
 *    order to prevent peripheral clocking from being shut down as soon as the cause of wake-up is
 *    cleared, but before DMAC0 has completed its related activity.
 */
#define SYSCTL0_HWWAKE_DMAC0WAKE(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL0_HWWAKE_DMAC0WAKE_SHIFT)) & SYSCTL0_HWWAKE_DMAC0WAKE_MASK)

#define SYSCTL0_HWWAKE_DMAC1WAKE_MASK            (0x10U)
#define SYSCTL0_HWWAKE_DMAC1WAKE_SHIFT           (4U)
/*! DMAC1WAKE - Wake for DMAC1. When 1, DMAC1 being busy will cause peripheral clocking to remain
 *    running until DMAC1 completes. This is generally used in conjunction with bit 1 and/or 2 in
 *    order to prevent peripheral clocking from being shut down as soon as the cause of wake-up is
 *    cleared, but before DMAC1 has completed its related activity.
 */
#define SYSCTL0_HWWAKE_DMAC1WAKE(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL0_HWWAKE_DMAC1WAKE_SHIFT)) & SYSCTL0_HWWAKE_DMAC1WAKE_MASK)
/*! @} */

/*! @name ROM_HIDING_ADDR_OFFSET - rom_hiding_addr_offset */
/*! @{ */

#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_ROM_HIDING_ADDR_OFFSET_MASK (0xFFFFU)
#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_ROM_HIDING_ADDR_OFFSET_SHIFT (0U)
/*! ROM_HIDING_ADDR_OFFSET - rom_hiding_addr_offset */
#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_ROM_HIDING_ADDR_OFFSET(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_ROM_HIDING_ADDR_OFFSET_ROM_HIDING_ADDR_OFFSET_SHIFT)) & SYSCTL0_ROM_HIDING_ADDR_OFFSET_ROM_HIDING_ADDR_OFFSET_MASK)
/*! @} */

/*! @name ROM_HIDING_ADDR_OFFSET_DP - rom_hiding_addr_offset_dp */
/*! @{ */

#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_DP_ROM_HIDING_ADDR_OFFSET_DP_MASK (0xFFFFU)
#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_DP_ROM_HIDING_ADDR_OFFSET_DP_SHIFT (0U)
/*! ROM_HIDING_ADDR_OFFSET_DP - rom_hiding_addr_offset_dp */
#define SYSCTL0_ROM_HIDING_ADDR_OFFSET_DP_ROM_HIDING_ADDR_OFFSET_DP(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_ROM_HIDING_ADDR_OFFSET_DP_ROM_HIDING_ADDR_OFFSET_DP_SHIFT)) & SYSCTL0_ROM_HIDING_ADDR_OFFSET_DP_ROM_HIDING_ADDR_OFFSET_DP_MASK)
/*! @} */

/*! @name ROM_HIDING_LOCK - rom_hiding_lock */
/*! @{ */

#define SYSCTL0_ROM_HIDING_LOCK_ROM_HIDING_LOCK_MASK (0xFFFFFFFFU)
#define SYSCTL0_ROM_HIDING_LOCK_ROM_HIDING_LOCK_SHIFT (0U)
/*! ROM_HIDING_LOCK - rom_hiding_addr_offset */
#define SYSCTL0_ROM_HIDING_LOCK_ROM_HIDING_LOCK(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL0_ROM_HIDING_LOCK_ROM_HIDING_LOCK_SHIFT)) & SYSCTL0_ROM_HIDING_LOCK_ROM_HIDING_LOCK_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SYSCTL0_Register_Masks */


/*!
 * @}
 */ /* end of group SYSCTL0_Peripheral_Access_Layer */


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


#endif  /* SYSCTL0_H_ */

