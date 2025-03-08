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
**         CMSIS Peripheral Access Layer for CDOG
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
 * @file CDOG.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CDOG
 *
 * CMSIS Peripheral Access Layer for CDOG
 */

#if !defined(CDOG_H_)
#define CDOG_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- CDOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CDOG_Peripheral_Access_Layer CDOG Peripheral Access Layer
 * @{
 */

/** CDOG - Register Layout Typedef */
typedef struct {
  __IO uint32_t CONTROL;                           /**< Control, offset: 0x0 */
  __IO uint32_t RELOAD;                            /**< Instruction Timer reload, offset: 0x4 */
  __IO uint32_t INSTRUCTION_TIMER;                 /**< Instruction Timer, offset: 0x8 */
  __IO uint32_t SECURE_COUNTER;                    /**< Secure Counter, offset: 0xC */
  __I  uint32_t STATUS;                            /**< Status 1, offset: 0x10 */
  __I  uint32_t STATUS2;                           /**< Status 2, offset: 0x14 */
  __IO uint32_t FLAGS;                             /**< Flags, offset: 0x18 */
  __IO uint32_t PERSISTENT;                        /**< Persistent Data Storage, offset: 0x1C */
  __O  uint32_t START;                             /**< START Command, offset: 0x20 */
  __O  uint32_t STOP;                              /**< STOP Command, offset: 0x24 */
  __O  uint32_t RESTART;                           /**< RESTART Command, offset: 0x28 */
  __O  uint32_t ADD;                               /**< ADD Command, offset: 0x2C */
  __O  uint32_t ADD1;                              /**< ADD1 Command, offset: 0x30 */
  __O  uint32_t ADD16;                             /**< ADD16 Command, offset: 0x34 */
  __O  uint32_t ADD256;                            /**< ADD256 Command, offset: 0x38 */
  __O  uint32_t SUB;                               /**< SUB Command, offset: 0x3C */
  __O  uint32_t SUB1;                              /**< SUB1 Command, offset: 0x40 */
  __O  uint32_t SUB16;                             /**< SUB16 Command, offset: 0x44 */
  __O  uint32_t SUB256;                            /**< SUB256 Command, offset: 0x48 */
} CDOG_Type;

/* ----------------------------------------------------------------------------
   -- CDOG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CDOG_Register_Masks CDOG Register Masks
 * @{
 */

/*! @name CONTROL - Control */
/*! @{ */

#define CDOG_CONTROL_LOCK_CTRL_MASK              (0x3U)
#define CDOG_CONTROL_LOCK_CTRL_SHIFT             (0U)
/*! LOCK_CTRL - Lock control
 *  0b01..Locked
 *  0b10..Unlocked
 */
#define CDOG_CONTROL_LOCK_CTRL(x)                (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_LOCK_CTRL_SHIFT)) & CDOG_CONTROL_LOCK_CTRL_MASK)

#define CDOG_CONTROL_TIMEOUT_CTRL_MASK           (0x1CU)
#define CDOG_CONTROL_TIMEOUT_CTRL_SHIFT          (2U)
/*! TIMEOUT_CTRL - TIMEOUT fault control
 *  0b100..Disable both reset and interrupt
 *  0b001..Enable reset
 *  0b010..Enable interrupt
 */
#define CDOG_CONTROL_TIMEOUT_CTRL(x)             (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_TIMEOUT_CTRL_SHIFT)) & CDOG_CONTROL_TIMEOUT_CTRL_MASK)

#define CDOG_CONTROL_MISCOMPARE_CTRL_MASK        (0xE0U)
#define CDOG_CONTROL_MISCOMPARE_CTRL_SHIFT       (5U)
/*! MISCOMPARE_CTRL - MISCOMPARE fault control
 *  0b100..Disable both reset and interrupt
 *  0b001..Enable reset
 *  0b010..Enable interrupt
 */
#define CDOG_CONTROL_MISCOMPARE_CTRL(x)          (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_MISCOMPARE_CTRL_SHIFT)) & CDOG_CONTROL_MISCOMPARE_CTRL_MASK)

#define CDOG_CONTROL_SEQUENCE_CTRL_MASK          (0x700U)
#define CDOG_CONTROL_SEQUENCE_CTRL_SHIFT         (8U)
/*! SEQUENCE_CTRL - SEQUENCE fault control
 *  0b001..Enable reset
 *  0b010..Enable interrupt
 *  0b100..Disable both reset and interrupt
 */
#define CDOG_CONTROL_SEQUENCE_CTRL(x)            (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_SEQUENCE_CTRL_SHIFT)) & CDOG_CONTROL_SEQUENCE_CTRL_MASK)

#define CDOG_CONTROL_CONTROL_CTRL_MASK           (0x3800U)
#define CDOG_CONTROL_CONTROL_CTRL_SHIFT          (11U)
/*! CONTROL_CTRL - CONTROL fault control
 *  0b001..Enable reset
 *  0b100..Disable reset
 */
#define CDOG_CONTROL_CONTROL_CTRL(x)             (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_CONTROL_CTRL_SHIFT)) & CDOG_CONTROL_CONTROL_CTRL_MASK)

#define CDOG_CONTROL_STATE_CTRL_MASK             (0x1C000U)
#define CDOG_CONTROL_STATE_CTRL_SHIFT            (14U)
/*! STATE_CTRL - STATE fault control
 *  0b001..Enable reset
 *  0b010..Enable interrupt
 *  0b100..Disable both reset and interrupt
 */
#define CDOG_CONTROL_STATE_CTRL(x)               (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_STATE_CTRL_SHIFT)) & CDOG_CONTROL_STATE_CTRL_MASK)

#define CDOG_CONTROL_ADDRESS_CTRL_MASK           (0xE0000U)
#define CDOG_CONTROL_ADDRESS_CTRL_SHIFT          (17U)
/*! ADDRESS_CTRL - ADDRESS fault control
 *  0b001..Enable reset
 *  0b010..Enable interrupt
 *  0b100..Disable both reset and interrupt
 */
#define CDOG_CONTROL_ADDRESS_CTRL(x)             (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_ADDRESS_CTRL_SHIFT)) & CDOG_CONTROL_ADDRESS_CTRL_MASK)

#define CDOG_CONTROL_IRQ_PAUSE_MASK              (0x30000000U)
#define CDOG_CONTROL_IRQ_PAUSE_SHIFT             (28U)
/*! IRQ_PAUSE - IRQ pause control
 *  0b01..Keep the timer running
 *  0b10..Stop the timer
 */
#define CDOG_CONTROL_IRQ_PAUSE(x)                (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_IRQ_PAUSE_SHIFT)) & CDOG_CONTROL_IRQ_PAUSE_MASK)

#define CDOG_CONTROL_DEBUG_HALT_CTRL_MASK        (0xC0000000U)
#define CDOG_CONTROL_DEBUG_HALT_CTRL_SHIFT       (30U)
/*! DEBUG_HALT_CTRL - DEBUG_HALT control
 *  0b01..Keep the timer running
 *  0b10..Stop the timer
 */
#define CDOG_CONTROL_DEBUG_HALT_CTRL(x)          (((uint32_t)(((uint32_t)(x)) << CDOG_CONTROL_DEBUG_HALT_CTRL_SHIFT)) & CDOG_CONTROL_DEBUG_HALT_CTRL_MASK)
/*! @} */

/*! @name RELOAD - Instruction Timer reload */
/*! @{ */

#define CDOG_RELOAD_RLOAD_MASK                   (0xFFFFFFFFU)
#define CDOG_RELOAD_RLOAD_SHIFT                  (0U)
/*! RLOAD - Instruction Timer reload value */
#define CDOG_RELOAD_RLOAD(x)                     (((uint32_t)(((uint32_t)(x)) << CDOG_RELOAD_RLOAD_SHIFT)) & CDOG_RELOAD_RLOAD_MASK)
/*! @} */

/*! @name INSTRUCTION_TIMER - Instruction Timer */
/*! @{ */

#define CDOG_INSTRUCTION_TIMER_INSTIM_MASK       (0xFFFFFFFFU)
#define CDOG_INSTRUCTION_TIMER_INSTIM_SHIFT      (0U)
/*! INSTIM - Current value of the Instruction Timer */
#define CDOG_INSTRUCTION_TIMER_INSTIM(x)         (((uint32_t)(((uint32_t)(x)) << CDOG_INSTRUCTION_TIMER_INSTIM_SHIFT)) & CDOG_INSTRUCTION_TIMER_INSTIM_MASK)
/*! @} */

/*! @name SECURE_COUNTER - Secure Counter */
/*! @{ */

#define CDOG_SECURE_COUNTER_SECCNT_MASK          (0xFFFFFFFFU)
#define CDOG_SECURE_COUNTER_SECCNT_SHIFT         (0U)
/*! SECCNT - Secure Counter */
#define CDOG_SECURE_COUNTER_SECCNT(x)            (((uint32_t)(((uint32_t)(x)) << CDOG_SECURE_COUNTER_SECCNT_SHIFT)) & CDOG_SECURE_COUNTER_SECCNT_MASK)
/*! @} */

/*! @name STATUS - Status 1 */
/*! @{ */

#define CDOG_STATUS_NUMTOF_MASK                  (0xFFU)
#define CDOG_STATUS_NUMTOF_SHIFT                 (0U)
/*! NUMTOF - Number of TIMEOUT faults since the last POR */
#define CDOG_STATUS_NUMTOF(x)                    (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS_NUMTOF_SHIFT)) & CDOG_STATUS_NUMTOF_MASK)

#define CDOG_STATUS_NUMMISCOMPF_MASK             (0xFF00U)
#define CDOG_STATUS_NUMMISCOMPF_SHIFT            (8U)
/*! NUMMISCOMPF - Number of MISCOMPARE faults since the last POR */
#define CDOG_STATUS_NUMMISCOMPF(x)               (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS_NUMMISCOMPF_SHIFT)) & CDOG_STATUS_NUMMISCOMPF_MASK)

#define CDOG_STATUS_NUMILSEQF_MASK               (0xFF0000U)
#define CDOG_STATUS_NUMILSEQF_SHIFT              (16U)
/*! NUMILSEQF - Number of SEQUENCE faults since the last POR */
#define CDOG_STATUS_NUMILSEQF(x)                 (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS_NUMILSEQF_SHIFT)) & CDOG_STATUS_NUMILSEQF_MASK)

#define CDOG_STATUS_CURST_MASK                   (0xF0000000U)
#define CDOG_STATUS_CURST_SHIFT                  (28U)
/*! CURST - Current State */
#define CDOG_STATUS_CURST(x)                     (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS_CURST_SHIFT)) & CDOG_STATUS_CURST_MASK)
/*! @} */

/*! @name STATUS2 - Status 2 */
/*! @{ */

#define CDOG_STATUS2_NUMCNTF_MASK                (0xFFU)
#define CDOG_STATUS2_NUMCNTF_SHIFT               (0U)
/*! NUMCNTF - Number of CONTROL faults since the last POR */
#define CDOG_STATUS2_NUMCNTF(x)                  (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS2_NUMCNTF_SHIFT)) & CDOG_STATUS2_NUMCNTF_MASK)

#define CDOG_STATUS2_NUMILLSTF_MASK              (0xFF00U)
#define CDOG_STATUS2_NUMILLSTF_SHIFT             (8U)
/*! NUMILLSTF - Number of STATE faults since the last POR */
#define CDOG_STATUS2_NUMILLSTF(x)                (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS2_NUMILLSTF_SHIFT)) & CDOG_STATUS2_NUMILLSTF_MASK)

#define CDOG_STATUS2_NUMILLA_MASK                (0xFF0000U)
#define CDOG_STATUS2_NUMILLA_SHIFT               (16U)
/*! NUMILLA - Number of ADDRESS faults since the last POR */
#define CDOG_STATUS2_NUMILLA(x)                  (((uint32_t)(((uint32_t)(x)) << CDOG_STATUS2_NUMILLA_SHIFT)) & CDOG_STATUS2_NUMILLA_MASK)
/*! @} */

/*! @name FLAGS - Flags */
/*! @{ */

#define CDOG_FLAGS_TO_FLAG_MASK                  (0x1U)
#define CDOG_FLAGS_TO_FLAG_SHIFT                 (0U)
/*! TO_FLAG - TIMEOUT fault flag
 *  0b0..A TIMEOUT fault has not occurred
 *  0b1..A TIMEOUT fault has occurred
 */
#define CDOG_FLAGS_TO_FLAG(x)                    (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_TO_FLAG_SHIFT)) & CDOG_FLAGS_TO_FLAG_MASK)

#define CDOG_FLAGS_MISCOM_FLAG_MASK              (0x2U)
#define CDOG_FLAGS_MISCOM_FLAG_SHIFT             (1U)
/*! MISCOM_FLAG - MISCOMPARE fault flag
 *  0b0..A MISCOMPARE fault has not occurred
 *  0b1..A MISCOMPARE fault has occurred
 */
#define CDOG_FLAGS_MISCOM_FLAG(x)                (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_MISCOM_FLAG_SHIFT)) & CDOG_FLAGS_MISCOM_FLAG_MASK)

#define CDOG_FLAGS_SEQ_FLAG_MASK                 (0x4U)
#define CDOG_FLAGS_SEQ_FLAG_SHIFT                (2U)
/*! SEQ_FLAG - SEQUENCE fault flag
 *  0b0..A SEQUENCE fault has not occurred
 *  0b1..A SEQUENCE fault has occurred
 */
#define CDOG_FLAGS_SEQ_FLAG(x)                   (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_SEQ_FLAG_SHIFT)) & CDOG_FLAGS_SEQ_FLAG_MASK)

#define CDOG_FLAGS_CNT_FLAG_MASK                 (0x8U)
#define CDOG_FLAGS_CNT_FLAG_SHIFT                (3U)
/*! CNT_FLAG - CONTROL fault flag
 *  0b0..A CONTROL fault has not occurred
 *  0b1..A CONTROL fault has occurred
 */
#define CDOG_FLAGS_CNT_FLAG(x)                   (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_CNT_FLAG_SHIFT)) & CDOG_FLAGS_CNT_FLAG_MASK)

#define CDOG_FLAGS_STATE_FLAG_MASK               (0x10U)
#define CDOG_FLAGS_STATE_FLAG_SHIFT              (4U)
/*! STATE_FLAG - STATE fault flag
 *  0b0..A STATE fault has not occurred
 *  0b1..A STATE fault has occurred
 */
#define CDOG_FLAGS_STATE_FLAG(x)                 (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_STATE_FLAG_SHIFT)) & CDOG_FLAGS_STATE_FLAG_MASK)

#define CDOG_FLAGS_ADDR_FLAG_MASK                (0x20U)
#define CDOG_FLAGS_ADDR_FLAG_SHIFT               (5U)
/*! ADDR_FLAG - ADDRESS fault flag
 *  0b0..An ADDRESS fault has not occurred
 *  0b1..An ADDRESS fault has occurred
 */
#define CDOG_FLAGS_ADDR_FLAG(x)                  (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_ADDR_FLAG_SHIFT)) & CDOG_FLAGS_ADDR_FLAG_MASK)

#define CDOG_FLAGS_POR_FLAG_MASK                 (0x10000U)
#define CDOG_FLAGS_POR_FLAG_SHIFT                (16U)
/*! POR_FLAG - Power-on reset flag
 *  0b0..A Power-on reset event has not occurred
 *  0b1..A Power-on reset event has occurred
 */
#define CDOG_FLAGS_POR_FLAG(x)                   (((uint32_t)(((uint32_t)(x)) << CDOG_FLAGS_POR_FLAG_SHIFT)) & CDOG_FLAGS_POR_FLAG_MASK)
/*! @} */

/*! @name PERSISTENT - Persistent Data Storage */
/*! @{ */

#define CDOG_PERSISTENT_PERSIS_MASK              (0xFFFFFFFFU)
#define CDOG_PERSISTENT_PERSIS_SHIFT             (0U)
/*! PERSIS - Persistent Storage */
#define CDOG_PERSISTENT_PERSIS(x)                (((uint32_t)(((uint32_t)(x)) << CDOG_PERSISTENT_PERSIS_SHIFT)) & CDOG_PERSISTENT_PERSIS_MASK)
/*! @} */

/*! @name START - START Command */
/*! @{ */

#define CDOG_START_STRT_MASK                     (0xFFFFFFFFU)
#define CDOG_START_STRT_SHIFT                    (0U)
/*! STRT - Start command */
#define CDOG_START_STRT(x)                       (((uint32_t)(((uint32_t)(x)) << CDOG_START_STRT_SHIFT)) & CDOG_START_STRT_MASK)
/*! @} */

/*! @name STOP - STOP Command */
/*! @{ */

#define CDOG_STOP_STP_MASK                       (0xFFFFFFFFU)
#define CDOG_STOP_STP_SHIFT                      (0U)
/*! STP - Stop command */
#define CDOG_STOP_STP(x)                         (((uint32_t)(((uint32_t)(x)) << CDOG_STOP_STP_SHIFT)) & CDOG_STOP_STP_MASK)
/*! @} */

/*! @name RESTART - RESTART Command */
/*! @{ */

#define CDOG_RESTART_RSTRT_MASK                  (0xFFFFFFFFU)
#define CDOG_RESTART_RSTRT_SHIFT                 (0U)
/*! RSTRT - Restart command */
#define CDOG_RESTART_RSTRT(x)                    (((uint32_t)(((uint32_t)(x)) << CDOG_RESTART_RSTRT_SHIFT)) & CDOG_RESTART_RSTRT_MASK)
/*! @} */

/*! @name ADD - ADD Command */
/*! @{ */

#define CDOG_ADD_AD_MASK                         (0xFFFFFFFFU)
#define CDOG_ADD_AD_SHIFT                        (0U)
/*! AD - ADD Write Value */
#define CDOG_ADD_AD(x)                           (((uint32_t)(((uint32_t)(x)) << CDOG_ADD_AD_SHIFT)) & CDOG_ADD_AD_MASK)
/*! @} */

/*! @name ADD1 - ADD1 Command */
/*! @{ */

#define CDOG_ADD1_AD1_MASK                       (0xFFFFFFFFU)
#define CDOG_ADD1_AD1_SHIFT                      (0U)
/*! AD1 - ADD 1 */
#define CDOG_ADD1_AD1(x)                         (((uint32_t)(((uint32_t)(x)) << CDOG_ADD1_AD1_SHIFT)) & CDOG_ADD1_AD1_MASK)
/*! @} */

/*! @name ADD16 - ADD16 Command */
/*! @{ */

#define CDOG_ADD16_AD16_MASK                     (0xFFFFFFFFU)
#define CDOG_ADD16_AD16_SHIFT                    (0U)
/*! AD16 - ADD 16 */
#define CDOG_ADD16_AD16(x)                       (((uint32_t)(((uint32_t)(x)) << CDOG_ADD16_AD16_SHIFT)) & CDOG_ADD16_AD16_MASK)
/*! @} */

/*! @name ADD256 - ADD256 Command */
/*! @{ */

#define CDOG_ADD256_AD256_MASK                   (0xFFFFFFFFU)
#define CDOG_ADD256_AD256_SHIFT                  (0U)
/*! AD256 - ADD 256 */
#define CDOG_ADD256_AD256(x)                     (((uint32_t)(((uint32_t)(x)) << CDOG_ADD256_AD256_SHIFT)) & CDOG_ADD256_AD256_MASK)
/*! @} */

/*! @name SUB - SUB Command */
/*! @{ */

#define CDOG_SUB_S0B_MASK                        (0xFFFFFFFFU)
#define CDOG_SUB_S0B_SHIFT                       (0U)
/*! S0B - Subtract Write Value */
#define CDOG_SUB_S0B(x)                          (((uint32_t)(((uint32_t)(x)) << CDOG_SUB_S0B_SHIFT)) & CDOG_SUB_S0B_MASK)
/*! @} */

/*! @name SUB1 - SUB1 Command */
/*! @{ */

#define CDOG_SUB1_S1B_MASK                       (0xFFFFFFFFU)
#define CDOG_SUB1_S1B_SHIFT                      (0U)
/*! S1B - Subtract 1 */
#define CDOG_SUB1_S1B(x)                         (((uint32_t)(((uint32_t)(x)) << CDOG_SUB1_S1B_SHIFT)) & CDOG_SUB1_S1B_MASK)
/*! @} */

/*! @name SUB16 - SUB16 Command */
/*! @{ */

#define CDOG_SUB16_SB16_MASK                     (0xFFFFFFFFU)
#define CDOG_SUB16_SB16_SHIFT                    (0U)
/*! SB16 - Subtract 16 */
#define CDOG_SUB16_SB16(x)                       (((uint32_t)(((uint32_t)(x)) << CDOG_SUB16_SB16_SHIFT)) & CDOG_SUB16_SB16_MASK)
/*! @} */

/*! @name SUB256 - SUB256 Command */
/*! @{ */

#define CDOG_SUB256_SB256_MASK                   (0xFFFFFFFFU)
#define CDOG_SUB256_SB256_SHIFT                  (0U)
/*! SB256 - Subtract 256 */
#define CDOG_SUB256_SB256(x)                     (((uint32_t)(((uint32_t)(x)) << CDOG_SUB256_SB256_SHIFT)) & CDOG_SUB256_SB256_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CDOG_Register_Masks */


/*!
 * @}
 */ /* end of group CDOG_Peripheral_Access_Layer */


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


#endif  /* CDOG_H_ */

