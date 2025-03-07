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
**         CMSIS Peripheral Access Layer for ITRC
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
 * @file ITRC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for ITRC
 *
 * CMSIS Peripheral Access Layer for ITRC
 */

#if !defined(ITRC_H_)
#define ITRC_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- ITRC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ITRC_Peripheral_Access_Layer ITRC Peripheral Access Layer
 * @{
 */

/** ITRC - Register Layout Typedef */
typedef struct {
  __IO uint32_t STATUS0;                           /**< Status0 register, offset: 0x0 */
  __IO uint32_t STATUS1;                           /**< Status1 register, offset: 0x4 */
  __IO uint32_t OUT0_SEL0;                         /**< ITRC_IRQ Trigger source selector 0 register for Event 0 to 15., offset: 0x8 */
  __IO uint32_t OUT0_SEL1;                         /**< ITRC_IRQ Trigger source selector 1 register for Event 0 to 15., offset: 0xC */
  __IO uint32_t OUT1_SEL0;                         /**< CHIP_RESET Trigger source selector 0 register for Event 0 to 15., offset: 0x10 */
  __IO uint32_t OUT1_SEL1;                         /**< CHIP_RESET Trigger source selector 1 register for Event 0 to 15., offset: 0x14 */
       uint8_t RESERVED_0[48];
  __IO uint32_t OUT0_SEL0_EVENT16_31;              /**< ITRC_IRQ Trigger source selector 0 register for Event 16 to 31., offset: 0x48 */
  __IO uint32_t OUT0_SEL1_EVENT16_31;              /**< ITRC_IRQ Trigger source selector 1 register for Event 16 to 31., offset: 0x4C */
  __IO uint32_t OUT1_SEL0_EVENT16_31;              /**< CHIP_RESET Trigger source selector 0 register for Event 16 to 31., offset: 0x50 */
  __IO uint32_t OUT1_SEL1_EVENT16_31;              /**< CHIP_RESET Trigger source selector 1 register for Event 16 to 31., offset: 0x54 */
       uint8_t RESERVED_1[152];
  __O  uint32_t SW_EVENT0;                         /**< Software event 0, offset: 0xF0 */
  __O  uint32_t SW_EVENT1;                         /**< Software event 1, offset: 0xF4 */
} ITRC_Type;

/* ----------------------------------------------------------------------------
   -- ITRC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ITRC_Register_Masks ITRC Register Masks
 * @{
 */

/*! @name STATUS0 - Status0 register */
/*! @{ */

#define ITRC_STATUS0_IN0_STATUS_MASK             (0x1U)
#define ITRC_STATUS0_IN0_STATUS_SHIFT            (0U)
/*! IN0_STATUS - CAU Temeprature Sensor detector event occurred. */
#define ITRC_STATUS0_IN0_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN0_STATUS_SHIFT)) & ITRC_STATUS0_IN0_STATUS_MASK)

#define ITRC_STATUS0_IN1_STATUS_MASK             (0x2U)
#define ITRC_STATUS0_IN1_STATUS_SHIFT            (1U)
/*! IN1_STATUS - PMIP Temperature Sensor detector event occurred. */
#define ITRC_STATUS0_IN1_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN1_STATUS_SHIFT)) & ITRC_STATUS0_IN1_STATUS_MASK)

#define ITRC_STATUS0_IN2_STATUS_MASK             (0x4U)
#define ITRC_STATUS0_IN2_STATUS_SHIFT            (2U)
/*! IN2_STATUS - Voltage Sensor detector event occured on VDD_CORE rail. */
#define ITRC_STATUS0_IN2_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN2_STATUS_SHIFT)) & ITRC_STATUS0_IN2_STATUS_MASK)

#define ITRC_STATUS0_IN3_STATUS_MASK             (0x8U)
#define ITRC_STATUS0_IN3_STATUS_SHIFT            (3U)
/*! IN3_STATUS - Voltage Sensor detector event occured on VDD_18 rail. */
#define ITRC_STATUS0_IN3_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN3_STATUS_SHIFT)) & ITRC_STATUS0_IN3_STATUS_MASK)

#define ITRC_STATUS0_IN4_STATUS_MASK             (0x10U)
#define ITRC_STATUS0_IN4_STATUS_SHIFT            (4U)
/*! IN4_STATUS - Voltage Sensor detector event occured on VDD_33 rail. */
#define ITRC_STATUS0_IN4_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN4_STATUS_SHIFT)) & ITRC_STATUS0_IN4_STATUS_MASK)

#define ITRC_STATUS0_IN5_STATUS_MASK             (0x20U)
#define ITRC_STATUS0_IN5_STATUS_SHIFT            (5U)
/*! IN5_STATUS - CAU Analog glitch sensor event occurred on VDD_CORE rail. */
#define ITRC_STATUS0_IN5_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN5_STATUS_SHIFT)) & ITRC_STATUS0_IN5_STATUS_MASK)

#define ITRC_STATUS0_IN6_STATUS_MASK             (0x40U)
#define ITRC_STATUS0_IN6_STATUS_SHIFT            (6U)
/*! IN6_STATUS - Analog Sensor configuration control anamoly detected. */
#define ITRC_STATUS0_IN6_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN6_STATUS_SHIFT)) & ITRC_STATUS0_IN6_STATUS_MASK)

#define ITRC_STATUS0_IN7_STATUS_MASK             (0x80U)
#define ITRC_STATUS0_IN7_STATUS_SHIFT            (7U)
/*! IN7_STATUS - AHB secure bus checkers detected illegal access. */
#define ITRC_STATUS0_IN7_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN7_STATUS_SHIFT)) & ITRC_STATUS0_IN7_STATUS_MASK)

#define ITRC_STATUS0_IN8_STATUS_MASK             (0x100U)
#define ITRC_STATUS0_IN8_STATUS_SHIFT            (8U)
/*! IN8_STATUS - Code watchdog detected an code execution anomaly. */
#define ITRC_STATUS0_IN8_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN8_STATUS_SHIFT)) & ITRC_STATUS0_IN8_STATUS_MASK)

#define ITRC_STATUS0_IN9_STATUS_MASK             (0x200U)
#define ITRC_STATUS0_IN9_STATUS_SHIFT            (9U)
/*! IN9_STATUS - ELS error event occurred. */
#define ITRC_STATUS0_IN9_STATUS(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN9_STATUS_SHIFT)) & ITRC_STATUS0_IN9_STATUS_MASK)

#define ITRC_STATUS0_IN10_STATUS_MASK            (0x400U)
#define ITRC_STATUS0_IN10_STATUS_SHIFT           (10U)
/*! IN10_STATUS - PKC module detected an error event. */
#define ITRC_STATUS0_IN10_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN10_STATUS_SHIFT)) & ITRC_STATUS0_IN10_STATUS_MASK)

#define ITRC_STATUS0_IN11_STATUS_MASK            (0x800U)
#define ITRC_STATUS0_IN11_STATUS_SHIFT           (11U)
/*! IN11_STATUS - OTP module detected an error event. */
#define ITRC_STATUS0_IN11_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN11_STATUS_SHIFT)) & ITRC_STATUS0_IN11_STATUS_MASK)

#define ITRC_STATUS0_IN12_STATUS_MASK            (0x1000U)
#define ITRC_STATUS0_IN12_STATUS_SHIFT           (12U)
/*! IN12_STATUS - Prince IP module detected an error event. */
#define ITRC_STATUS0_IN12_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN12_STATUS_SHIFT)) & ITRC_STATUS0_IN12_STATUS_MASK)

#define ITRC_STATUS0_IN13_STATUS_MASK            (0x2000U)
#define ITRC_STATUS0_IN13_STATUS_SHIFT           (13U)
/*! IN13_STATUS - ELS glitch detector module detected an error event. */
#define ITRC_STATUS0_IN13_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN13_STATUS_SHIFT)) & ITRC_STATUS0_IN13_STATUS_MASK)

#define ITRC_STATUS0_IN14_STATUS_MASK            (0x4000U)
#define ITRC_STATUS0_IN14_STATUS_SHIFT           (14U)
/*! IN14_STATUS - Security IP Command violation error event. */
#define ITRC_STATUS0_IN14_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN14_STATUS_SHIFT)) & ITRC_STATUS0_IN14_STATUS_MASK)

#define ITRC_STATUS0_IN15_STATUS_MASK            (0x8000U)
#define ITRC_STATUS0_IN15_STATUS_SHIFT           (15U)
/*! IN15_STATUS - True Random Number generator error event. */
#define ITRC_STATUS0_IN15_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_IN15_STATUS_SHIFT)) & ITRC_STATUS0_IN15_STATUS_MASK)

#define ITRC_STATUS0_OUT0_STATUS_MASK            (0x10000U)
#define ITRC_STATUS0_OUT0_STATUS_SHIFT           (16U)
/*! OUT0_STATUS - ITRC triggered ITRC_IRQ output. */
#define ITRC_STATUS0_OUT0_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_OUT0_STATUS_SHIFT)) & ITRC_STATUS0_OUT0_STATUS_MASK)

#define ITRC_STATUS0_OUT1_STATUS_MASK            (0x20000U)
#define ITRC_STATUS0_OUT1_STATUS_SHIFT           (17U)
/*! OUT1_STATUS - ITRC triggered CHIP_RESET to reset the chip after all other response process finished. */
#define ITRC_STATUS0_OUT1_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS0_OUT1_STATUS_SHIFT)) & ITRC_STATUS0_OUT1_STATUS_MASK)
/*! @} */

/*! @name STATUS1 - Status1 register */
/*! @{ */

#define ITRC_STATUS1_IN16_STATUS_MASK            (0x1U)
#define ITRC_STATUS1_IN16_STATUS_SHIFT           (0U)
/*! IN16_STATUS - PMIP Analog glitch sensor event occurred on VDD_18 rail. */
#define ITRC_STATUS1_IN16_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN16_STATUS_SHIFT)) & ITRC_STATUS1_IN16_STATUS_MASK)

#define ITRC_STATUS1_IN17_STATUS_MASK            (0x2U)
#define ITRC_STATUS1_IN17_STATUS_SHIFT           (1U)
/*! IN17_STATUS - PMIP Analog glitch sensor event occurred on VDD_CORE rail. */
#define ITRC_STATUS1_IN17_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN17_STATUS_SHIFT)) & ITRC_STATUS1_IN17_STATUS_MASK)

#define ITRC_STATUS1_IN18_STATUS_MASK            (0x4U)
#define ITRC_STATUS1_IN18_STATUS_SHIFT           (2U)
/*! IN18_STATUS - TCPU PLL UnLock Error occurred. */
#define ITRC_STATUS1_IN18_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN18_STATUS_SHIFT)) & ITRC_STATUS1_IN18_STATUS_MASK)

#define ITRC_STATUS1_IN19_STATUS_MASK            (0x8U)
#define ITRC_STATUS1_IN19_STATUS_SHIFT           (3U)
/*! IN19_STATUS - T3 PLL UnLock Error occurred. */
#define ITRC_STATUS1_IN19_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN19_STATUS_SHIFT)) & ITRC_STATUS1_IN19_STATUS_MASK)

#define ITRC_STATUS1_IN20_STATUS_MASK            (0x10U)
#define ITRC_STATUS1_IN20_STATUS_SHIFT           (4U)
/*! IN20_STATUS - Software event 0 occurred. */
#define ITRC_STATUS1_IN20_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN20_STATUS_SHIFT)) & ITRC_STATUS1_IN20_STATUS_MASK)

#define ITRC_STATUS1_IN21_STATUS_MASK            (0x20U)
#define ITRC_STATUS1_IN21_STATUS_SHIFT           (5U)
/*! IN21_STATUS - Software event 1 occurred. */
#define ITRC_STATUS1_IN21_STATUS(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_STATUS1_IN21_STATUS_SHIFT)) & ITRC_STATUS1_IN21_STATUS_MASK)
/*! @} */

/*! @name OUT0_SEL0 - ITRC_IRQ Trigger source selector 0 register for Event 0 to 15. */
/*! @{ */

#define ITRC_OUT0_SEL0_IN0_SEL0_MASK             (0x3U)
#define ITRC_OUT0_SEL0_IN0_SEL0_SHIFT            (0U)
/*! IN0_SEL0 - Selects CAU Temeprature Sensor event as a trigger source. */
#define ITRC_OUT0_SEL0_IN0_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN0_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN0_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN1_SEL0_MASK             (0xCU)
#define ITRC_OUT0_SEL0_IN1_SEL0_SHIFT            (2U)
/*! IN1_SEL0 - Selects PMIP Temperature Sensor event as a trigger source. */
#define ITRC_OUT0_SEL0_IN1_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN1_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN1_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN2_SEL0_MASK             (0x30U)
#define ITRC_OUT0_SEL0_IN2_SEL0_SHIFT            (4U)
/*! IN2_SEL0 - Selects Voltage Sensor detector event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL0_IN2_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN2_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN2_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN3_SEL0_MASK             (0xC0U)
#define ITRC_OUT0_SEL0_IN3_SEL0_SHIFT            (6U)
/*! IN3_SEL0 - Selects Voltage Sensor detector event on VDD_18 rail as a trigger source. */
#define ITRC_OUT0_SEL0_IN3_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN3_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN3_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN4_SEL0_MASK             (0x300U)
#define ITRC_OUT0_SEL0_IN4_SEL0_SHIFT            (8U)
/*! IN4_SEL0 - Selects Voltage Sensor detector event on VDD_33 rail as a trigger source. */
#define ITRC_OUT0_SEL0_IN4_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN4_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN4_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN5_SEL0_MASK             (0xC00U)
#define ITRC_OUT0_SEL0_IN5_SEL0_SHIFT            (10U)
/*! IN5_SEL0 - Selects Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL0_IN5_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN5_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN5_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN6_SEL0_MASK             (0x3000U)
#define ITRC_OUT0_SEL0_IN6_SEL0_SHIFT            (12U)
/*! IN6_SEL0 - Selects Analog Sensor configuration anamoly event as a trigger source. */
#define ITRC_OUT0_SEL0_IN6_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN6_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN6_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN7_SEL0_MASK             (0xC000U)
#define ITRC_OUT0_SEL0_IN7_SEL0_SHIFT            (14U)
/*! IN7_SEL0 - Selects AHB secure bus illegal access event as a trigger source. */
#define ITRC_OUT0_SEL0_IN7_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN7_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN7_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN8_SEL0_MASK             (0x30000U)
#define ITRC_OUT0_SEL0_IN8_SEL0_SHIFT            (16U)
/*! IN8_SEL0 - Selects Code Watch Dog event as a trigger source. */
#define ITRC_OUT0_SEL0_IN8_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN8_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN8_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN9_SEL0_MASK             (0xC0000U)
#define ITRC_OUT0_SEL0_IN9_SEL0_SHIFT            (18U)
/*! IN9_SEL0 - Selects ELS error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN9_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN9_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN9_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN10_SEL0_MASK            (0x300000U)
#define ITRC_OUT0_SEL0_IN10_SEL0_SHIFT           (20U)
/*! IN10_SEL0 - Selects PKC error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN10_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN10_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN10_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN11_SEL0_MASK            (0xC00000U)
#define ITRC_OUT0_SEL0_IN11_SEL0_SHIFT           (22U)
/*! IN11_SEL0 - Selects OTP error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN11_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN11_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN11_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN12_SEL0_MASK            (0x3000000U)
#define ITRC_OUT0_SEL0_IN12_SEL0_SHIFT           (24U)
/*! IN12_SEL0 - Selects PRINCE IP error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN12_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN12_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN12_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN13_SEL0_MASK            (0xC000000U)
#define ITRC_OUT0_SEL0_IN13_SEL0_SHIFT           (26U)
/*! IN13_SEL0 - Selects ELS glitch detector error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN13_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN13_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN13_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN14_SEL0_MASK            (0x30000000U)
#define ITRC_OUT0_SEL0_IN14_SEL0_SHIFT           (28U)
/*! IN14_SEL0 - Selects Security IP Command violation error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN14_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN14_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN14_SEL0_MASK)

#define ITRC_OUT0_SEL0_IN15_SEL0_MASK            (0xC0000000U)
#define ITRC_OUT0_SEL0_IN15_SEL0_SHIFT           (30U)
/*! IN15_SEL0 - Selects TRNG violation error event as a trigger source. */
#define ITRC_OUT0_SEL0_IN15_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_IN15_SEL0_SHIFT)) & ITRC_OUT0_SEL0_IN15_SEL0_MASK)
/*! @} */

/*! @name OUT0_SEL1 - ITRC_IRQ Trigger source selector 1 register for Event 0 to 15. */
/*! @{ */

#define ITRC_OUT0_SEL1_IN0_SEL1_MASK             (0x3U)
#define ITRC_OUT0_SEL1_IN0_SEL1_SHIFT            (0U)
/*! IN0_SEL1 - Selects CAU Temeprature Sensor event as a trigger source. */
#define ITRC_OUT0_SEL1_IN0_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN0_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN0_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN1_SEL1_MASK             (0xCU)
#define ITRC_OUT0_SEL1_IN1_SEL1_SHIFT            (2U)
/*! IN1_SEL1 - Selects PMIP Temperature Sensor event as a trigger source. */
#define ITRC_OUT0_SEL1_IN1_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN1_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN1_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN2_SEL1_MASK             (0x30U)
#define ITRC_OUT0_SEL1_IN2_SEL1_SHIFT            (4U)
/*! IN2_SEL1 - Selects Voltage Sensor detector event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL1_IN2_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN2_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN2_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN3_SEL1_MASK             (0xC0U)
#define ITRC_OUT0_SEL1_IN3_SEL1_SHIFT            (6U)
/*! IN3_SEL1 - Selects Voltage Sensor detector event on VDD_18 rail as a trigger source. */
#define ITRC_OUT0_SEL1_IN3_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN3_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN3_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN4_SEL1_MASK             (0x300U)
#define ITRC_OUT0_SEL1_IN4_SEL1_SHIFT            (8U)
/*! IN4_SEL1 - Selects Voltage Sensor detector event on VDD_33 rail as a trigger source. */
#define ITRC_OUT0_SEL1_IN4_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN4_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN4_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN5_SEL1_MASK             (0xC00U)
#define ITRC_OUT0_SEL1_IN5_SEL1_SHIFT            (10U)
/*! IN5_SEL1 - Selects Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL1_IN5_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN5_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN5_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN6_SEL1_MASK             (0x3000U)
#define ITRC_OUT0_SEL1_IN6_SEL1_SHIFT            (12U)
/*! IN6_SEL1 - >Selects Analog Sensor configuration anamoly event as a trigger source. */
#define ITRC_OUT0_SEL1_IN6_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN6_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN6_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN7_SEL1_MASK             (0xC000U)
#define ITRC_OUT0_SEL1_IN7_SEL1_SHIFT            (14U)
/*! IN7_SEL1 - Selects AHB secure bus illegal access event as a trigger source. */
#define ITRC_OUT0_SEL1_IN7_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN7_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN7_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN8_SEL1_MASK             (0x30000U)
#define ITRC_OUT0_SEL1_IN8_SEL1_SHIFT            (16U)
/*! IN8_SEL1 - Selects Code Watch Dog event as a trigger source. */
#define ITRC_OUT0_SEL1_IN8_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN8_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN8_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN9_SEL1_MASK             (0xC0000U)
#define ITRC_OUT0_SEL1_IN9_SEL1_SHIFT            (18U)
/*! IN9_SEL1 - Selects ELS error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN9_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN9_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN9_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN10_SEL1_MASK            (0x300000U)
#define ITRC_OUT0_SEL1_IN10_SEL1_SHIFT           (20U)
/*! IN10_SEL1 - Selects PKC error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN10_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN10_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN10_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN11_SEL1_MASK            (0xC00000U)
#define ITRC_OUT0_SEL1_IN11_SEL1_SHIFT           (22U)
/*! IN11_SEL1 - Selects OTP error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN11_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN11_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN11_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN12_SEL1_MASK            (0x3000000U)
#define ITRC_OUT0_SEL1_IN12_SEL1_SHIFT           (24U)
/*! IN12_SEL1 - Selects PRINCE IP error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN12_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN12_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN12_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN13_SEL1_MASK            (0xC000000U)
#define ITRC_OUT0_SEL1_IN13_SEL1_SHIFT           (26U)
/*! IN13_SEL1 - Selects ELS glitch detector error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN13_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN13_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN13_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN14_SEL1_MASK            (0x30000000U)
#define ITRC_OUT0_SEL1_IN14_SEL1_SHIFT           (28U)
/*! IN14_SEL1 - Selects Security IP Command violation error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN14_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN14_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN14_SEL1_MASK)

#define ITRC_OUT0_SEL1_IN15_SEL1_MASK            (0xC0000000U)
#define ITRC_OUT0_SEL1_IN15_SEL1_SHIFT           (30U)
/*! IN15_SEL1 - Selects TRNG violation error event as a trigger source. */
#define ITRC_OUT0_SEL1_IN15_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_IN15_SEL1_SHIFT)) & ITRC_OUT0_SEL1_IN15_SEL1_MASK)
/*! @} */

/*! @name OUT1_SEL0 - CHIP_RESET Trigger source selector 0 register for Event 0 to 15. */
/*! @{ */

#define ITRC_OUT1_SEL0_IN0_SEL0_MASK             (0x3U)
#define ITRC_OUT1_SEL0_IN0_SEL0_SHIFT            (0U)
/*! IN0_SEL0 - Selects CAU Temeprature Sensor event as a trigger source. */
#define ITRC_OUT1_SEL0_IN0_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN0_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN0_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN1_SEL0_MASK             (0xCU)
#define ITRC_OUT1_SEL0_IN1_SEL0_SHIFT            (2U)
/*! IN1_SEL0 - Selects PMIP Temperature Sensor event as a trigger source. */
#define ITRC_OUT1_SEL0_IN1_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN1_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN1_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN2_SEL0_MASK             (0x30U)
#define ITRC_OUT1_SEL0_IN2_SEL0_SHIFT            (4U)
/*! IN2_SEL0 - Selects Voltage Sensor detector event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL0_IN2_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN2_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN2_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN3_SEL0_MASK             (0xC0U)
#define ITRC_OUT1_SEL0_IN3_SEL0_SHIFT            (6U)
/*! IN3_SEL0 - Selects Voltage Sensor detector event on VDD_18 rail as a trigger source. */
#define ITRC_OUT1_SEL0_IN3_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN3_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN3_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN4_SEL0_MASK             (0x300U)
#define ITRC_OUT1_SEL0_IN4_SEL0_SHIFT            (8U)
/*! IN4_SEL0 - Selects Voltage Sensor detector event on VDD_33 rail as a trigger source. */
#define ITRC_OUT1_SEL0_IN4_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN4_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN4_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN5_SEL0_MASK             (0xC00U)
#define ITRC_OUT1_SEL0_IN5_SEL0_SHIFT            (10U)
/*! IN5_SEL0 - Selects Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL0_IN5_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN5_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN5_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN6_SEL0_MASK             (0x3000U)
#define ITRC_OUT1_SEL0_IN6_SEL0_SHIFT            (12U)
/*! IN6_SEL0 - >Selects Analog Sensor configuration anamoly event as a trigger source. */
#define ITRC_OUT1_SEL0_IN6_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN6_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN6_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN7_SEL0_MASK             (0xC000U)
#define ITRC_OUT1_SEL0_IN7_SEL0_SHIFT            (14U)
/*! IN7_SEL0 - Selects AHB secure bus illegal access event as a trigger source. */
#define ITRC_OUT1_SEL0_IN7_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN7_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN7_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN8_SEL0_MASK             (0x30000U)
#define ITRC_OUT1_SEL0_IN8_SEL0_SHIFT            (16U)
/*! IN8_SEL0 - Selects Code Watch Dog event as a trigger source. */
#define ITRC_OUT1_SEL0_IN8_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN8_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN8_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN9_SEL0_MASK             (0xC0000U)
#define ITRC_OUT1_SEL0_IN9_SEL0_SHIFT            (18U)
/*! IN9_SEL0 - Selects ELS error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN9_SEL0(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN9_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN9_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN10_SEL0_MASK            (0x300000U)
#define ITRC_OUT1_SEL0_IN10_SEL0_SHIFT           (20U)
/*! IN10_SEL0 - Selects PKC error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN10_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN10_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN10_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN11_SEL0_MASK            (0xC00000U)
#define ITRC_OUT1_SEL0_IN11_SEL0_SHIFT           (22U)
/*! IN11_SEL0 - Selects OTP error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN11_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN11_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN11_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN12_SEL0_MASK            (0x3000000U)
#define ITRC_OUT1_SEL0_IN12_SEL0_SHIFT           (24U)
/*! IN12_SEL0 - Selects PRINCE IP error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN12_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN12_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN12_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN13_SEL0_MASK            (0xC000000U)
#define ITRC_OUT1_SEL0_IN13_SEL0_SHIFT           (26U)
/*! IN13_SEL0 - Selects ELS glitch detector error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN13_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN13_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN13_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN14_SEL0_MASK            (0x30000000U)
#define ITRC_OUT1_SEL0_IN14_SEL0_SHIFT           (28U)
/*! IN14_SEL0 - Selects Security IP Command violation error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN14_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN14_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN14_SEL0_MASK)

#define ITRC_OUT1_SEL0_IN15_SEL0_MASK            (0xC0000000U)
#define ITRC_OUT1_SEL0_IN15_SEL0_SHIFT           (30U)
/*! IN15_SEL0 - Selects TRNG violation error event as a trigger source. */
#define ITRC_OUT1_SEL0_IN15_SEL0(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_IN15_SEL0_SHIFT)) & ITRC_OUT1_SEL0_IN15_SEL0_MASK)
/*! @} */

/*! @name OUT1_SEL1 - CHIP_RESET Trigger source selector 1 register for Event 0 to 15. */
/*! @{ */

#define ITRC_OUT1_SEL1_IN0_SEL1_MASK             (0x3U)
#define ITRC_OUT1_SEL1_IN0_SEL1_SHIFT            (0U)
/*! IN0_SEL1 - Selects CAU Temeprature Sensor event as a trigger source. */
#define ITRC_OUT1_SEL1_IN0_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN0_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN0_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN1_SEL1_MASK             (0xCU)
#define ITRC_OUT1_SEL1_IN1_SEL1_SHIFT            (2U)
/*! IN1_SEL1 - Selects PMIP Temperature Sensor event as a trigger source. */
#define ITRC_OUT1_SEL1_IN1_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN1_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN1_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN2_SEL1_MASK             (0x30U)
#define ITRC_OUT1_SEL1_IN2_SEL1_SHIFT            (4U)
/*! IN2_SEL1 - Selects Voltage Sensor detector event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL1_IN2_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN2_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN2_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN3_SEL1_MASK             (0xC0U)
#define ITRC_OUT1_SEL1_IN3_SEL1_SHIFT            (6U)
/*! IN3_SEL1 - Selects Voltage Sensor detector event on VDD_18 rail as a trigger source. */
#define ITRC_OUT1_SEL1_IN3_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN3_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN3_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN4_SEL1_MASK             (0x300U)
#define ITRC_OUT1_SEL1_IN4_SEL1_SHIFT            (8U)
/*! IN4_SEL1 - Selects Voltage Sensor detector event on VDD_33 rail as a trigger source. */
#define ITRC_OUT1_SEL1_IN4_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN4_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN4_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN5_SEL1_MASK             (0xC00U)
#define ITRC_OUT1_SEL1_IN5_SEL1_SHIFT            (10U)
/*! IN5_SEL1 - Selects Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL1_IN5_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN5_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN5_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN6_SEL1_MASK             (0x3000U)
#define ITRC_OUT1_SEL1_IN6_SEL1_SHIFT            (12U)
/*! IN6_SEL1 - >Selects Analog Sensor configuration anamoly event as a trigger source. */
#define ITRC_OUT1_SEL1_IN6_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN6_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN6_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN7_SEL1_MASK             (0xC000U)
#define ITRC_OUT1_SEL1_IN7_SEL1_SHIFT            (14U)
/*! IN7_SEL1 - Selects AHB secure bus illegal access event as a trigger source. */
#define ITRC_OUT1_SEL1_IN7_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN7_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN7_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN8_SEL1_MASK             (0x30000U)
#define ITRC_OUT1_SEL1_IN8_SEL1_SHIFT            (16U)
/*! IN8_SEL1 - Selects Code Watch Dog event as a trigger source. */
#define ITRC_OUT1_SEL1_IN8_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN8_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN8_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN9_SEL1_MASK             (0xC0000U)
#define ITRC_OUT1_SEL1_IN9_SEL1_SHIFT            (18U)
/*! IN9_SEL1 - Selects ELS error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN9_SEL1(x)               (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN9_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN9_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN10_SEL1_MASK            (0x300000U)
#define ITRC_OUT1_SEL1_IN10_SEL1_SHIFT           (20U)
/*! IN10_SEL1 - Selects PKC error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN10_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN10_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN10_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN11_SEL1_MASK            (0xC00000U)
#define ITRC_OUT1_SEL1_IN11_SEL1_SHIFT           (22U)
/*! IN11_SEL1 - Selects OTP error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN11_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN11_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN11_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN12_SEL1_MASK            (0x3000000U)
#define ITRC_OUT1_SEL1_IN12_SEL1_SHIFT           (24U)
/*! IN12_SEL1 - Selects PRINCE IP error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN12_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN12_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN12_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN13_SEL1_MASK            (0xC000000U)
#define ITRC_OUT1_SEL1_IN13_SEL1_SHIFT           (26U)
/*! IN13_SEL1 - Selects ELS glitch detector error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN13_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN13_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN13_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN14_SEL1_MASK            (0x30000000U)
#define ITRC_OUT1_SEL1_IN14_SEL1_SHIFT           (28U)
/*! IN14_SEL1 - Selects Security IP Command violation error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN14_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN14_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN14_SEL1_MASK)

#define ITRC_OUT1_SEL1_IN15_SEL1_MASK            (0xC0000000U)
#define ITRC_OUT1_SEL1_IN15_SEL1_SHIFT           (30U)
/*! IN15_SEL1 - Selects TRNG violation error event as a trigger source. */
#define ITRC_OUT1_SEL1_IN15_SEL1(x)              (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_IN15_SEL1_SHIFT)) & ITRC_OUT1_SEL1_IN15_SEL1_MASK)
/*! @} */

/*! @name OUT0_SEL0_EVENT16_31 - ITRC_IRQ Trigger source selector 0 register for Event 16 to 31. */
/*! @{ */

#define ITRC_OUT0_SEL0_EVENT16_31_IN16_SEL0_MASK (0x3U)
#define ITRC_OUT0_SEL0_EVENT16_31_IN16_SEL0_SHIFT (0U)
/*! IN16_SEL0 - Selects PMIP Analog glitch sensor event on VDD_18 rail as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN16_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN16_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN16_SEL0_MASK)

#define ITRC_OUT0_SEL0_EVENT16_31_IN17_SEL0_MASK (0xCU)
#define ITRC_OUT0_SEL0_EVENT16_31_IN17_SEL0_SHIFT (2U)
/*! IN17_SEL0 - Selects PMIP Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN17_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN17_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN17_SEL0_MASK)

#define ITRC_OUT0_SEL0_EVENT16_31_IN18_SEL0_MASK (0x30U)
#define ITRC_OUT0_SEL0_EVENT16_31_IN18_SEL0_SHIFT (4U)
/*! IN18_SEL0 - Selects TCPU PLL Unlock event as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN18_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN18_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN18_SEL0_MASK)

#define ITRC_OUT0_SEL0_EVENT16_31_IN19_SEL0_MASK (0xC0U)
#define ITRC_OUT0_SEL0_EVENT16_31_IN19_SEL0_SHIFT (6U)
/*! IN19_SEL0 - Selects T3 PLL Unlock event as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN19_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN19_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN19_SEL0_MASK)

#define ITRC_OUT0_SEL0_EVENT16_31_IN20_SEL0_MASK (0x300U)
#define ITRC_OUT0_SEL0_EVENT16_31_IN20_SEL0_SHIFT (8U)
/*! IN20_SEL0 - Selects software event 0 as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN20_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN20_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN20_SEL0_MASK)

#define ITRC_OUT0_SEL0_EVENT16_31_IN21_SEL0_MASK (0xC00U)
#define ITRC_OUT0_SEL0_EVENT16_31_IN21_SEL0_SHIFT (10U)
/*! IN21_SEL0 - Selects software event 1 as a trigger source. */
#define ITRC_OUT0_SEL0_EVENT16_31_IN21_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL0_EVENT16_31_IN21_SEL0_SHIFT)) & ITRC_OUT0_SEL0_EVENT16_31_IN21_SEL0_MASK)
/*! @} */

/*! @name OUT0_SEL1_EVENT16_31 - ITRC_IRQ Trigger source selector 1 register for Event 16 to 31. */
/*! @{ */

#define ITRC_OUT0_SEL1_EVENT16_31_IN16_SEL1_MASK (0x3U)
#define ITRC_OUT0_SEL1_EVENT16_31_IN16_SEL1_SHIFT (0U)
/*! IN16_SEL1 - Selects PMIP Analog glitch sensor event on VDD_18 rail as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN16_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN16_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN16_SEL1_MASK)

#define ITRC_OUT0_SEL1_EVENT16_31_IN17_SEL1_MASK (0xCU)
#define ITRC_OUT0_SEL1_EVENT16_31_IN17_SEL1_SHIFT (2U)
/*! IN17_SEL1 - Selects PMIP Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN17_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN17_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN17_SEL1_MASK)

#define ITRC_OUT0_SEL1_EVENT16_31_IN18_SEL1_MASK (0x30U)
#define ITRC_OUT0_SEL1_EVENT16_31_IN18_SEL1_SHIFT (4U)
/*! IN18_SEL1 - Selects TCPU PLL Unlock event as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN18_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN18_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN18_SEL1_MASK)

#define ITRC_OUT0_SEL1_EVENT16_31_IN19_SEL1_MASK (0xC0U)
#define ITRC_OUT0_SEL1_EVENT16_31_IN19_SEL1_SHIFT (6U)
/*! IN19_SEL1 - Selects T3 PLL Unlock event as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN19_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN19_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN19_SEL1_MASK)

#define ITRC_OUT0_SEL1_EVENT16_31_IN20_SEL1_MASK (0x300U)
#define ITRC_OUT0_SEL1_EVENT16_31_IN20_SEL1_SHIFT (8U)
/*! IN20_SEL1 - Selects software event 0 as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN20_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN20_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN20_SEL1_MASK)

#define ITRC_OUT0_SEL1_EVENT16_31_IN21_SEL1_MASK (0xC00U)
#define ITRC_OUT0_SEL1_EVENT16_31_IN21_SEL1_SHIFT (10U)
/*! IN21_SEL1 - Selects software event 1 as a trigger source. */
#define ITRC_OUT0_SEL1_EVENT16_31_IN21_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT0_SEL1_EVENT16_31_IN21_SEL1_SHIFT)) & ITRC_OUT0_SEL1_EVENT16_31_IN21_SEL1_MASK)
/*! @} */

/*! @name OUT1_SEL0_EVENT16_31 - CHIP_RESET Trigger source selector 0 register for Event 16 to 31. */
/*! @{ */

#define ITRC_OUT1_SEL0_EVENT16_31_IN16_SEL0_MASK (0x3U)
#define ITRC_OUT1_SEL0_EVENT16_31_IN16_SEL0_SHIFT (0U)
/*! IN16_SEL0 - Selects PMIP Analog glitch sensor event on VDD_18 rail as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN16_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN16_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN16_SEL0_MASK)

#define ITRC_OUT1_SEL0_EVENT16_31_IN17_SEL0_MASK (0xCU)
#define ITRC_OUT1_SEL0_EVENT16_31_IN17_SEL0_SHIFT (2U)
/*! IN17_SEL0 - Selects PMIP Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN17_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN17_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN17_SEL0_MASK)

#define ITRC_OUT1_SEL0_EVENT16_31_IN18_SEL0_MASK (0x30U)
#define ITRC_OUT1_SEL0_EVENT16_31_IN18_SEL0_SHIFT (4U)
/*! IN18_SEL0 - Selects TCPU PLL Unlock event as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN18_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN18_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN18_SEL0_MASK)

#define ITRC_OUT1_SEL0_EVENT16_31_IN19_SEL0_MASK (0xC0U)
#define ITRC_OUT1_SEL0_EVENT16_31_IN19_SEL0_SHIFT (6U)
/*! IN19_SEL0 - Selects T3 PLL Unlock event as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN19_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN19_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN19_SEL0_MASK)

#define ITRC_OUT1_SEL0_EVENT16_31_IN20_SEL0_MASK (0x300U)
#define ITRC_OUT1_SEL0_EVENT16_31_IN20_SEL0_SHIFT (8U)
/*! IN20_SEL0 - Selects software event 0 as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN20_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN20_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN20_SEL0_MASK)

#define ITRC_OUT1_SEL0_EVENT16_31_IN21_SEL0_MASK (0xC00U)
#define ITRC_OUT1_SEL0_EVENT16_31_IN21_SEL0_SHIFT (10U)
/*! IN21_SEL0 - Selects software event 1 as a trigger source. */
#define ITRC_OUT1_SEL0_EVENT16_31_IN21_SEL0(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL0_EVENT16_31_IN21_SEL0_SHIFT)) & ITRC_OUT1_SEL0_EVENT16_31_IN21_SEL0_MASK)
/*! @} */

/*! @name OUT1_SEL1_EVENT16_31 - CHIP_RESET Trigger source selector 1 register for Event 16 to 31. */
/*! @{ */

#define ITRC_OUT1_SEL1_EVENT16_31_IN16_SEL1_MASK (0x3U)
#define ITRC_OUT1_SEL1_EVENT16_31_IN16_SEL1_SHIFT (0U)
/*! IN16_SEL1 - Selects PMIP Analog glitch sensor event on VDD_18 rail as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN16_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN16_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN16_SEL1_MASK)

#define ITRC_OUT1_SEL1_EVENT16_31_IN17_SEL1_MASK (0xCU)
#define ITRC_OUT1_SEL1_EVENT16_31_IN17_SEL1_SHIFT (2U)
/*! IN17_SEL1 - Selects PMIP Analog glitch sensor event on VDD_CORE rail as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN17_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN17_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN17_SEL1_MASK)

#define ITRC_OUT1_SEL1_EVENT16_31_IN18_SEL1_MASK (0x30U)
#define ITRC_OUT1_SEL1_EVENT16_31_IN18_SEL1_SHIFT (4U)
/*! IN18_SEL1 - Selects TCPU PLL Unlock event as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN18_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN18_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN18_SEL1_MASK)

#define ITRC_OUT1_SEL1_EVENT16_31_IN19_SEL1_MASK (0xC0U)
#define ITRC_OUT1_SEL1_EVENT16_31_IN19_SEL1_SHIFT (6U)
/*! IN19_SEL1 - Selects T3 PLL Unlock event as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN19_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN19_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN19_SEL1_MASK)

#define ITRC_OUT1_SEL1_EVENT16_31_IN20_SEL1_MASK (0x300U)
#define ITRC_OUT1_SEL1_EVENT16_31_IN20_SEL1_SHIFT (8U)
/*! IN20_SEL1 - Selects software event 0 as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN20_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN20_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN20_SEL1_MASK)

#define ITRC_OUT1_SEL1_EVENT16_31_IN21_SEL1_MASK (0xC00U)
#define ITRC_OUT1_SEL1_EVENT16_31_IN21_SEL1_SHIFT (10U)
/*! IN21_SEL1 - Selects software event 1 as a trigger source. */
#define ITRC_OUT1_SEL1_EVENT16_31_IN21_SEL1(x)   (((uint32_t)(((uint32_t)(x)) << ITRC_OUT1_SEL1_EVENT16_31_IN21_SEL1_SHIFT)) & ITRC_OUT1_SEL1_EVENT16_31_IN21_SEL1_MASK)
/*! @} */

/*! @name SW_EVENT0 - Software event 0 */
/*! @{ */

#define ITRC_SW_EVENT0_TRIGGER_SW_EVENT_0_MASK   (0xFFFFFFFFU)
#define ITRC_SW_EVENT0_TRIGGER_SW_EVENT_0_SHIFT  (0U)
/*! TRIGGER_SW_EVENT_0 - Trigger software event 0. */
#define ITRC_SW_EVENT0_TRIGGER_SW_EVENT_0(x)     (((uint32_t)(((uint32_t)(x)) << ITRC_SW_EVENT0_TRIGGER_SW_EVENT_0_SHIFT)) & ITRC_SW_EVENT0_TRIGGER_SW_EVENT_0_MASK)
/*! @} */

/*! @name SW_EVENT1 - Software event 1 */
/*! @{ */

#define ITRC_SW_EVENT1_TRIGGER_SW_EVENT_1_MASK   (0xFFFFFFFFU)
#define ITRC_SW_EVENT1_TRIGGER_SW_EVENT_1_SHIFT  (0U)
/*! TRIGGER_SW_EVENT_1 - Trigger software event 1. */
#define ITRC_SW_EVENT1_TRIGGER_SW_EVENT_1(x)     (((uint32_t)(((uint32_t)(x)) << ITRC_SW_EVENT1_TRIGGER_SW_EVENT_1_SHIFT)) & ITRC_SW_EVENT1_TRIGGER_SW_EVENT_1_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group ITRC_Register_Masks */


/*!
 * @}
 */ /* end of group ITRC_Peripheral_Access_Layer */


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


#endif  /* ITRC_H_ */

