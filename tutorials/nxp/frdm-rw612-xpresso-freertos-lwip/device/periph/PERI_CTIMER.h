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
**         CMSIS Peripheral Access Layer for CTIMER
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
 * @file CTIMER.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CTIMER
 *
 * CMSIS Peripheral Access Layer for CTIMER
 */

#if !defined(CTIMER_H_)
#define CTIMER_H_                                /**< Symbol preventing repeated inclusion */

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
   -- CTIMER Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CTIMER_Peripheral_Access_Layer CTIMER Peripheral Access Layer
 * @{
 */

/** CTIMER - Size of Registers Arrays */
#define CTIMER_MR_COUNT                           4u
#define CTIMER_CR_COUNT                           4u
#define CTIMER_MSR_COUNT                          4u

/** CTIMER - Register Layout Typedef */
typedef struct {
  __IO uint32_t IR;                                /**< Interrupt Register., offset: 0x0 */
  __IO uint32_t TCR;                               /**< Timer Control Register, offset: 0x4 */
  __IO uint32_t TC;                                /**< Timer Counter, offset: 0x8 */
  __IO uint32_t PR;                                /**< Prescale Register, offset: 0xC */
  __IO uint32_t PC;                                /**< Prescale Counter., offset: 0x10 */
  __IO uint32_t MCR;                               /**< Match Control Register, offset: 0x14 */
  __IO uint32_t MR[CTIMER_MR_COUNT];               /**< Match Register, array offset: 0x18, array step: 0x4 */
  __IO uint32_t CCR;                               /**< Capture Control Register, offset: 0x28 */
  __I  uint32_t CR[CTIMER_CR_COUNT];               /**< Capture Register, array offset: 0x2C, array step: 0x4 */
  __IO uint32_t EMR;                               /**< External Match Register, offset: 0x3C */
       uint8_t RESERVED_0[48];
  __IO uint32_t CTCR;                              /**< Count Control Register, offset: 0x70 */
  __IO uint32_t PWMC;                              /**< PWM Control Register, offset: 0x74 */
  __IO uint32_t MSR[CTIMER_MSR_COUNT];             /**< Match Shadow Register, array offset: 0x78, array step: 0x4 */
} CTIMER_Type;

/* ----------------------------------------------------------------------------
   -- CTIMER Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CTIMER_Register_Masks CTIMER Register Masks
 * @{
 */

/*! @name IR - Interrupt Register. */
/*! @{ */

#define CTIMER_IR_MR0INT_MASK                    (0x1U)
#define CTIMER_IR_MR0INT_SHIFT                   (0U)
/*! MR0INT - Interrupt flag for match channel 0 */
#define CTIMER_IR_MR0INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_MR0INT_SHIFT)) & CTIMER_IR_MR0INT_MASK)

#define CTIMER_IR_MR1INT_MASK                    (0x2U)
#define CTIMER_IR_MR1INT_SHIFT                   (1U)
/*! MR1INT - Interrupt flag for match channel 1 */
#define CTIMER_IR_MR1INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_MR1INT_SHIFT)) & CTIMER_IR_MR1INT_MASK)

#define CTIMER_IR_MR2INT_MASK                    (0x4U)
#define CTIMER_IR_MR2INT_SHIFT                   (2U)
/*! MR2INT - Interrupt flag for match channel 2 */
#define CTIMER_IR_MR2INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_MR2INT_SHIFT)) & CTIMER_IR_MR2INT_MASK)

#define CTIMER_IR_MR3INT_MASK                    (0x8U)
#define CTIMER_IR_MR3INT_SHIFT                   (3U)
/*! MR3INT - Interrupt flag for match channel 3 */
#define CTIMER_IR_MR3INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_MR3INT_SHIFT)) & CTIMER_IR_MR3INT_MASK)

#define CTIMER_IR_CR0INT_MASK                    (0x10U)
#define CTIMER_IR_CR0INT_SHIFT                   (4U)
/*! CR0INT - Interrupt flag for capture channel 0 event */
#define CTIMER_IR_CR0INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_CR0INT_SHIFT)) & CTIMER_IR_CR0INT_MASK)

#define CTIMER_IR_CR1INT_MASK                    (0x20U)
#define CTIMER_IR_CR1INT_SHIFT                   (5U)
/*! CR1INT - Interrupt flag for capture channel 1 event */
#define CTIMER_IR_CR1INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_CR1INT_SHIFT)) & CTIMER_IR_CR1INT_MASK)

#define CTIMER_IR_CR2INT_MASK                    (0x40U)
#define CTIMER_IR_CR2INT_SHIFT                   (6U)
/*! CR2INT - Interrupt flag for capture channel 2 event */
#define CTIMER_IR_CR2INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_CR2INT_SHIFT)) & CTIMER_IR_CR2INT_MASK)

#define CTIMER_IR_CR3INT_MASK                    (0x80U)
#define CTIMER_IR_CR3INT_SHIFT                   (7U)
/*! CR3INT - Interrupt flag for capture channel 3 event */
#define CTIMER_IR_CR3INT(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_IR_CR3INT_SHIFT)) & CTIMER_IR_CR3INT_MASK)
/*! @} */

/*! @name TCR - Timer Control Register */
/*! @{ */

#define CTIMER_TCR_CEN_MASK                      (0x1U)
#define CTIMER_TCR_CEN_SHIFT                     (0U)
/*! CEN - Counter enable.
 *  0b0..Disabled. The counters are disabled.
 *  0b1..Enabled. The Timer Counter and Prescale Counter are enabled. When the timer is enabled by an external
 *       trigger or globally enabled by the external global start enable register, the CEN bit will automatically be
 *       set to 1.
 */
#define CTIMER_TCR_CEN(x)                        (((uint32_t)(((uint32_t)(x)) << CTIMER_TCR_CEN_SHIFT)) & CTIMER_TCR_CEN_MASK)

#define CTIMER_TCR_CRST_MASK                     (0x2U)
#define CTIMER_TCR_CRST_SHIFT                    (1U)
/*! CRST - Counter reset.
 *  0b0..Disabled. Do nothing.
 *  0b1..Enabled
 */
#define CTIMER_TCR_CRST(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_TCR_CRST_SHIFT)) & CTIMER_TCR_CRST_MASK)

#define CTIMER_TCR_AGCEN_MASK                    (0x10U)
#define CTIMER_TCR_AGCEN_SHIFT                   (4U)
/*! AGCEN - Allow Global Count Enable
 *  0b0..Not allowed
 *  0b1..Allow input global_enable=1 action to take effect
 */
#define CTIMER_TCR_AGCEN(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_TCR_AGCEN_SHIFT)) & CTIMER_TCR_AGCEN_MASK)

#define CTIMER_TCR_ATCEN_MASK                    (0x20U)
#define CTIMER_TCR_ATCEN_SHIFT                   (5U)
/*! ATCEN - Allow Trigger Count Enable
 *  0b0..Not allowed
 *  0b1..Allow input trigger_enable=1 action to take effect
 */
#define CTIMER_TCR_ATCEN(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_TCR_ATCEN_SHIFT)) & CTIMER_TCR_ATCEN_MASK)
/*! @} */

/*! @name TC - Timer Counter */
/*! @{ */

#define CTIMER_TC_TCVAL_MASK                     (0xFFFFFFFFU)
#define CTIMER_TC_TCVAL_SHIFT                    (0U)
/*! TCVAL - Timer counter value. */
#define CTIMER_TC_TCVAL(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_TC_TCVAL_SHIFT)) & CTIMER_TC_TCVAL_MASK)
/*! @} */

/*! @name PR - Prescale Register */
/*! @{ */

#define CTIMER_PR_PRVAL_MASK                     (0xFFFFFFFFU)
#define CTIMER_PR_PRVAL_SHIFT                    (0U)
/*! PRVAL - Prescale reload value. */
#define CTIMER_PR_PRVAL(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_PR_PRVAL_SHIFT)) & CTIMER_PR_PRVAL_MASK)
/*! @} */

/*! @name PC - Prescale Counter. */
/*! @{ */

#define CTIMER_PC_PCVAL_MASK                     (0xFFFFFFFFU)
#define CTIMER_PC_PCVAL_SHIFT                    (0U)
/*! PCVAL - Prescale counter value */
#define CTIMER_PC_PCVAL(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_PC_PCVAL_SHIFT)) & CTIMER_PC_PCVAL_MASK)
/*! @} */

/*! @name MCR - Match Control Register */
/*! @{ */

#define CTIMER_MCR_MR0I_MASK                     (0x1U)
#define CTIMER_MCR_MR0I_SHIFT                    (0U)
/*! MR0I - Interrupt on MR0
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR0I(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR0I_SHIFT)) & CTIMER_MCR_MR0I_MASK)

#define CTIMER_MCR_MR0R_MASK                     (0x2U)
#define CTIMER_MCR_MR0R_SHIFT                    (1U)
/*! MR0R - Reset on MR0
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR0R(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR0R_SHIFT)) & CTIMER_MCR_MR0R_MASK)

#define CTIMER_MCR_MR0S_MASK                     (0x4U)
#define CTIMER_MCR_MR0S_SHIFT                    (2U)
/*! MR0S - Stop on MR0
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR0S(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR0S_SHIFT)) & CTIMER_MCR_MR0S_MASK)

#define CTIMER_MCR_MR1I_MASK                     (0x8U)
#define CTIMER_MCR_MR1I_SHIFT                    (3U)
/*! MR1I - Interrupt on MR1
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR1I(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR1I_SHIFT)) & CTIMER_MCR_MR1I_MASK)

#define CTIMER_MCR_MR1R_MASK                     (0x10U)
#define CTIMER_MCR_MR1R_SHIFT                    (4U)
/*! MR1R - Reset on MR1
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR1R(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR1R_SHIFT)) & CTIMER_MCR_MR1R_MASK)

#define CTIMER_MCR_MR1S_MASK                     (0x20U)
#define CTIMER_MCR_MR1S_SHIFT                    (5U)
/*! MR1S - Stop on MR1
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR1S(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR1S_SHIFT)) & CTIMER_MCR_MR1S_MASK)

#define CTIMER_MCR_MR2I_MASK                     (0x40U)
#define CTIMER_MCR_MR2I_SHIFT                    (6U)
/*! MR2I - Interrupt on MR2
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR2I(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR2I_SHIFT)) & CTIMER_MCR_MR2I_MASK)

#define CTIMER_MCR_MR2R_MASK                     (0x80U)
#define CTIMER_MCR_MR2R_SHIFT                    (7U)
/*! MR2R - Reset on MR2
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR2R(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR2R_SHIFT)) & CTIMER_MCR_MR2R_MASK)

#define CTIMER_MCR_MR2S_MASK                     (0x100U)
#define CTIMER_MCR_MR2S_SHIFT                    (8U)
/*! MR2S - Stop on MR2
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR2S(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR2S_SHIFT)) & CTIMER_MCR_MR2S_MASK)

#define CTIMER_MCR_MR3I_MASK                     (0x200U)
#define CTIMER_MCR_MR3I_SHIFT                    (9U)
/*! MR3I - Interrupt on MR3
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR3I(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR3I_SHIFT)) & CTIMER_MCR_MR3I_MASK)

#define CTIMER_MCR_MR3R_MASK                     (0x400U)
#define CTIMER_MCR_MR3R_SHIFT                    (10U)
/*! MR3R - Reset on MR3
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR3R(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR3R_SHIFT)) & CTIMER_MCR_MR3R_MASK)

#define CTIMER_MCR_MR3S_MASK                     (0x800U)
#define CTIMER_MCR_MR3S_SHIFT                    (11U)
/*! MR3S - Stop on MR3
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR3S(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR3S_SHIFT)) & CTIMER_MCR_MR3S_MASK)

#define CTIMER_MCR_MR0RL_MASK                    (0x1000000U)
#define CTIMER_MCR_MR0RL_SHIFT                   (24U)
/*! MR0RL - Reload MR0
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR0RL(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR0RL_SHIFT)) & CTIMER_MCR_MR0RL_MASK)

#define CTIMER_MCR_MR1RL_MASK                    (0x2000000U)
#define CTIMER_MCR_MR1RL_SHIFT                   (25U)
/*! MR1RL - Reload MR1
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR1RL(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR1RL_SHIFT)) & CTIMER_MCR_MR1RL_MASK)

#define CTIMER_MCR_MR2RL_MASK                    (0x4000000U)
#define CTIMER_MCR_MR2RL_SHIFT                   (26U)
/*! MR2RL - Reload MR2
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR2RL(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR2RL_SHIFT)) & CTIMER_MCR_MR2RL_MASK)

#define CTIMER_MCR_MR3RL_MASK                    (0x8000000U)
#define CTIMER_MCR_MR3RL_SHIFT                   (27U)
/*! MR3RL - Reload MR3
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_MCR_MR3RL(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_MCR_MR3RL_SHIFT)) & CTIMER_MCR_MR3RL_MASK)
/*! @} */

/*! @name MR - Match Register */
/*! @{ */

#define CTIMER_MR_MATCH_MASK                     (0xFFFFFFFFU)
#define CTIMER_MR_MATCH_SHIFT                    (0U)
/*! MATCH - Timer counter match value */
#define CTIMER_MR_MATCH(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_MR_MATCH_SHIFT)) & CTIMER_MR_MATCH_MASK)
/*! @} */

/*! @name CCR - Capture Control Register */
/*! @{ */

#define CTIMER_CCR_CAP0RE_MASK                   (0x1U)
#define CTIMER_CCR_CAP0RE_SHIFT                  (0U)
/*! CAP0RE - Rising edge of capture channel 0: a sequence of 0 then 1 causes CR0 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP0RE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP0RE_SHIFT)) & CTIMER_CCR_CAP0RE_MASK)

#define CTIMER_CCR_CAP0FE_MASK                   (0x2U)
#define CTIMER_CCR_CAP0FE_SHIFT                  (1U)
/*! CAP0FE - Falling edge of capture channel 0: a sequence of 1 then 0 causes CR0 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP0FE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP0FE_SHIFT)) & CTIMER_CCR_CAP0FE_MASK)

#define CTIMER_CCR_CAP0I_MASK                    (0x4U)
#define CTIMER_CCR_CAP0I_SHIFT                   (2U)
/*! CAP0I - Generate interrupt on channel 0 capture event: a CR0 load generates an interrupt.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP0I(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP0I_SHIFT)) & CTIMER_CCR_CAP0I_MASK)

#define CTIMER_CCR_CAP1RE_MASK                   (0x8U)
#define CTIMER_CCR_CAP1RE_SHIFT                  (3U)
/*! CAP1RE - Rising edge of capture channel 1: a sequence of 0 then 1 causes CR1 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP1RE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP1RE_SHIFT)) & CTIMER_CCR_CAP1RE_MASK)

#define CTIMER_CCR_CAP1FE_MASK                   (0x10U)
#define CTIMER_CCR_CAP1FE_SHIFT                  (4U)
/*! CAP1FE - Falling edge of capture channel 1: a sequence of 1 then 0 causes CR1 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP1FE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP1FE_SHIFT)) & CTIMER_CCR_CAP1FE_MASK)

#define CTIMER_CCR_CAP1I_MASK                    (0x20U)
#define CTIMER_CCR_CAP1I_SHIFT                   (5U)
/*! CAP1I - Generate interrupt on channel 1 capture event: a CR1 load generates an interrupt.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP1I(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP1I_SHIFT)) & CTIMER_CCR_CAP1I_MASK)

#define CTIMER_CCR_CAP2RE_MASK                   (0x40U)
#define CTIMER_CCR_CAP2RE_SHIFT                  (6U)
/*! CAP2RE - Rising edge of capture channel 2: a sequence of 0 then 1 causes CR2 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP2RE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP2RE_SHIFT)) & CTIMER_CCR_CAP2RE_MASK)

#define CTIMER_CCR_CAP2FE_MASK                   (0x80U)
#define CTIMER_CCR_CAP2FE_SHIFT                  (7U)
/*! CAP2FE - Falling edge of capture channel 2: a sequence of 1 then 0 causes CR2 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP2FE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP2FE_SHIFT)) & CTIMER_CCR_CAP2FE_MASK)

#define CTIMER_CCR_CAP2I_MASK                    (0x100U)
#define CTIMER_CCR_CAP2I_SHIFT                   (8U)
/*! CAP2I - Generate interrupt on channel 2 capture event: a CR2 load generates an interrupt.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP2I(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP2I_SHIFT)) & CTIMER_CCR_CAP2I_MASK)

#define CTIMER_CCR_CAP3RE_MASK                   (0x200U)
#define CTIMER_CCR_CAP3RE_SHIFT                  (9U)
/*! CAP3RE - Rising edge of capture channel 3: a sequence of 0 then 1 causes CR3 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP3RE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP3RE_SHIFT)) & CTIMER_CCR_CAP3RE_MASK)

#define CTIMER_CCR_CAP3FE_MASK                   (0x400U)
#define CTIMER_CCR_CAP3FE_SHIFT                  (10U)
/*! CAP3FE - Falling edge of capture channel 3: a sequence of 1 then 0 causes CR3 to be loaded with the contents of TC.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP3FE(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP3FE_SHIFT)) & CTIMER_CCR_CAP3FE_MASK)

#define CTIMER_CCR_CAP3I_MASK                    (0x800U)
#define CTIMER_CCR_CAP3I_SHIFT                   (11U)
/*! CAP3I - Generate interrupt on channel 3 capture event: a CR3 load generates an interrupt.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define CTIMER_CCR_CAP3I(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_CCR_CAP3I_SHIFT)) & CTIMER_CCR_CAP3I_MASK)
/*! @} */

/*! @name CR - Capture Register */
/*! @{ */

#define CTIMER_CR_CAP_MASK                       (0xFFFFFFFFU)
#define CTIMER_CR_CAP_SHIFT                      (0U)
/*! CAP - Timer counter capture value. */
#define CTIMER_CR_CAP(x)                         (((uint32_t)(((uint32_t)(x)) << CTIMER_CR_CAP_SHIFT)) & CTIMER_CR_CAP_MASK)
/*! @} */

/*! @name EMR - External Match Register */
/*! @{ */

#define CTIMER_EMR_EM0_MASK                      (0x1U)
#define CTIMER_EMR_EM0_SHIFT                     (0U)
/*! EM0 - External Match 0 */
#define CTIMER_EMR_EM0(x)                        (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EM0_SHIFT)) & CTIMER_EMR_EM0_MASK)

#define CTIMER_EMR_EM1_MASK                      (0x2U)
#define CTIMER_EMR_EM1_SHIFT                     (1U)
/*! EM1 - External Match 1 */
#define CTIMER_EMR_EM1(x)                        (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EM1_SHIFT)) & CTIMER_EMR_EM1_MASK)

#define CTIMER_EMR_EM2_MASK                      (0x4U)
#define CTIMER_EMR_EM2_SHIFT                     (2U)
/*! EM2 - External Match 2 */
#define CTIMER_EMR_EM2(x)                        (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EM2_SHIFT)) & CTIMER_EMR_EM2_MASK)

#define CTIMER_EMR_EM3_MASK                      (0x8U)
#define CTIMER_EMR_EM3_SHIFT                     (3U)
/*! EM3 - External Match 3 */
#define CTIMER_EMR_EM3(x)                        (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EM3_SHIFT)) & CTIMER_EMR_EM3_MASK)

#define CTIMER_EMR_EMC0_MASK                     (0x30U)
#define CTIMER_EMR_EMC0_SHIFT                    (4U)
/*! EMC0 - External Match Control 0
 *  0b00..Do Nothing.
 *  0b01..Clear. Clear the corresponding External Match bit/output to 0 (MAT0 pin is LOW if pinned out).
 *  0b10..Set. Set the corresponding External Match bit/output to 1 (MAT0 pin is HIGH if pinned out).
 *  0b11..Toggle. Toggle the corresponding External Match bit/output.
 */
#define CTIMER_EMR_EMC0(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EMC0_SHIFT)) & CTIMER_EMR_EMC0_MASK)

#define CTIMER_EMR_EMC1_MASK                     (0xC0U)
#define CTIMER_EMR_EMC1_SHIFT                    (6U)
/*! EMC1 - External Match Control 1
 *  0b00..Do Nothing
 *  0b01..Clear. Clear the corresponding External Match bit/output to 0 (MAT1 pin is LOW if pinned out).
 *  0b10..Set. Set the corresponding External Match bit/output to 1 (MAT1 pin is HIGH if pinned out).
 *  0b11..Toggle. Toggle the corresponding External Match bit/output.
 */
#define CTIMER_EMR_EMC1(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EMC1_SHIFT)) & CTIMER_EMR_EMC1_MASK)

#define CTIMER_EMR_EMC2_MASK                     (0x300U)
#define CTIMER_EMR_EMC2_SHIFT                    (8U)
/*! EMC2 - External Match Control 2
 *  0b00..Do Nothing.
 *  0b01..Clear. Clear the corresponding External Match bit/output to 0 (MAT2 pin is LOW if pinned out).
 *  0b10..Set. Set the corresponding External Match bit/output to 1 (MAT2 pin is HIGH if pinned out).
 *  0b11..Toggle. Toggle the corresponding External Match bit/output.
 */
#define CTIMER_EMR_EMC2(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EMC2_SHIFT)) & CTIMER_EMR_EMC2_MASK)

#define CTIMER_EMR_EMC3_MASK                     (0xC00U)
#define CTIMER_EMR_EMC3_SHIFT                    (10U)
/*! EMC3 - External Match Control 3
 *  0b00..Do Nothing.
 *  0b01..Clear. Clear the corresponding External Match bit/output to 0 (MAT3 pin is LOW if pinned out).
 *  0b10..Set. Set the corresponding External Match bit/output to 1 (MAT3 pin is HIGH if pinned out).
 *  0b11..Toggle. Toggle the corresponding External Match bit/output.
 */
#define CTIMER_EMR_EMC3(x)                       (((uint32_t)(((uint32_t)(x)) << CTIMER_EMR_EMC3_SHIFT)) & CTIMER_EMR_EMC3_MASK)
/*! @} */

/*! @name CTCR - Count Control Register */
/*! @{ */

#define CTIMER_CTCR_CTMODE_MASK                  (0x3U)
#define CTIMER_CTCR_CTMODE_SHIFT                 (0U)
/*! CTMODE - The Count Control Register (CTCR) is used to select between Timer and Counter mode, and
 *    in Counter mode to select the pin and edge(s) for counting.
 *  0b00..Timer Mode
 *  0b01..Counter Mode rising edge
 *  0b10..Counter Mode falling edge
 *  0b11..Counter Mode dual edge
 */
#define CTIMER_CTCR_CTMODE(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_CTCR_CTMODE_SHIFT)) & CTIMER_CTCR_CTMODE_MASK)

#define CTIMER_CTCR_CINSEL_MASK                  (0xCU)
#define CTIMER_CTCR_CINSEL_SHIFT                 (2U)
/*! CINSEL - Count Input Select
 *  0b00..Channel 0. CAPn.0 for CTIMERn
 *  0b01..Channel 1. CAPn.1 for CTIMERn
 *  0b10..Channel 2. CAPn.2 for CTIMERn
 *  0b11..Channel 3. CAPn.3 for CTIMERn
 */
#define CTIMER_CTCR_CINSEL(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_CTCR_CINSEL_SHIFT)) & CTIMER_CTCR_CINSEL_MASK)

#define CTIMER_CTCR_ENCC_MASK                    (0x10U)
#define CTIMER_CTCR_ENCC_SHIFT                   (4U)
#define CTIMER_CTCR_ENCC(x)                      (((uint32_t)(((uint32_t)(x)) << CTIMER_CTCR_ENCC_SHIFT)) & CTIMER_CTCR_ENCC_MASK)

#define CTIMER_CTCR_SELCC_MASK                   (0xE0U)
#define CTIMER_CTCR_SELCC_SHIFT                  (5U)
/*! SELCC - Edge select
 *  0b000..Channel 0 Rising Edge. Rising edge of the signal on capture channel 0 clears the timer (if bit 4 is set).
 *  0b001..Channel 0 Falling Edge. Falling edge of the signal on capture channel 0 clears the timer (if bit 4 is set).
 *  0b010..Channel 1 Rising Edge. Rising edge of the signal on capture channel 1 clears the timer (if bit 4 is set).
 *  0b011..Channel 1 Falling Edge. Falling edge of the signal on capture channel 1 clears the timer (if bit 4 is set).
 *  0b100..Channel 2 Rising Edge. Rising edge of the signal on capture channel 2 clears the timer (if bit 4 is set).
 *  0b101..Channel 2 Falling Edge. Falling edge of the signal on capture channel 2 clears the timer (if bit 4 is set).
 */
#define CTIMER_CTCR_SELCC(x)                     (((uint32_t)(((uint32_t)(x)) << CTIMER_CTCR_SELCC_SHIFT)) & CTIMER_CTCR_SELCC_MASK)
/*! @} */

/*! @name PWMC - PWM Control Register */
/*! @{ */

#define CTIMER_PWMC_PWMEN0_MASK                  (0x1U)
#define CTIMER_PWMC_PWMEN0_SHIFT                 (0U)
/*! PWMEN0 - PWM mode enable for channel0.
 *  0b0..Match. CTIMERn_MAT0 is controlled by EM0.
 *  0b1..PWM. PWM mode is enabled for CTIMERn_MAT0.
 */
#define CTIMER_PWMC_PWMEN0(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_PWMC_PWMEN0_SHIFT)) & CTIMER_PWMC_PWMEN0_MASK)

#define CTIMER_PWMC_PWMEN1_MASK                  (0x2U)
#define CTIMER_PWMC_PWMEN1_SHIFT                 (1U)
/*! PWMEN1 - PWM mode enable for channel1.
 *  0b0..Match. CTIMERn_MAT01 is controlled by EM1.
 *  0b1..PWM. PWM mode is enabled for CTIMERn_MAT1.
 */
#define CTIMER_PWMC_PWMEN1(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_PWMC_PWMEN1_SHIFT)) & CTIMER_PWMC_PWMEN1_MASK)

#define CTIMER_PWMC_PWMEN2_MASK                  (0x4U)
#define CTIMER_PWMC_PWMEN2_SHIFT                 (2U)
/*! PWMEN2 - PWM mode enable for channel2.
 *  0b0..Match. CTIMERn_MAT2 is controlled by EM2.
 *  0b1..PWM. PWM mode is enabled for CTIMERn_MAT2.
 */
#define CTIMER_PWMC_PWMEN2(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_PWMC_PWMEN2_SHIFT)) & CTIMER_PWMC_PWMEN2_MASK)

#define CTIMER_PWMC_PWMEN3_MASK                  (0x8U)
#define CTIMER_PWMC_PWMEN3_SHIFT                 (3U)
/*! PWMEN3 - PWM mode enable for channel3. Note: It is recommended to use match channel 3 to set the PWM cycle.
 *  0b0..Match. CTIMERn_MAT3 is controlled by EM3.
 *  0b1..PWM. PWM mode is enabled for CT132Bn_MAT3.
 */
#define CTIMER_PWMC_PWMEN3(x)                    (((uint32_t)(((uint32_t)(x)) << CTIMER_PWMC_PWMEN3_SHIFT)) & CTIMER_PWMC_PWMEN3_MASK)
/*! @} */

/*! @name MSR - Match Shadow Register */
/*! @{ */

#define CTIMER_MSR_MATCH_SHADOW_MASK             (0xFFFFFFFFU)
#define CTIMER_MSR_MATCH_SHADOW_SHIFT            (0U)
/*! MATCH_SHADOW - Timer counter match shadow value. */
#define CTIMER_MSR_MATCH_SHADOW(x)               (((uint32_t)(((uint32_t)(x)) << CTIMER_MSR_MATCH_SHADOW_SHIFT)) & CTIMER_MSR_MATCH_SHADOW_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CTIMER_Register_Masks */


/*!
 * @}
 */ /* end of group CTIMER_Peripheral_Access_Layer */


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


#endif  /* CTIMER_H_ */

