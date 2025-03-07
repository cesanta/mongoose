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
**         CMSIS Peripheral Access Layer for RF_SYSCON
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
 * @file RF_SYSCON.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for RF_SYSCON
 *
 * CMSIS Peripheral Access Layer for RF_SYSCON
 */

#if !defined(RF_SYSCON_H_)
#define RF_SYSCON_H_                             /**< Symbol preventing repeated inclusion */

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
   -- RF_SYSCON Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RF_SYSCON_Peripheral_Access_Layer RF_SYSCON Peripheral Access Layer
 * @{
 */

/** RF_SYSCON - Size of Registers Arrays */
#define RF_SYSCON_WO_SCRATCH_REG_COUNT            8u
#define RF_SYSCON_RW_SCRATCH_REG_COUNT            8u

/** RF_SYSCON - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[1024];
  __I  uint32_t WO_SCRATCH_REG[RF_SYSCON_WO_SCRATCH_REG_COUNT]; /**< Write once scratch register 0..Write once scratch register 7, array offset: 0x400, array step: 0x4 */
       uint8_t RESERVED_1[96];
  __IO uint32_t RW_SCRATCH_REG[RF_SYSCON_RW_SCRATCH_REG_COUNT]; /**< Scratch register 0..Scratch register 7, array offset: 0x480, array step: 0x4 */
} RF_SYSCON_Type;

/* ----------------------------------------------------------------------------
   -- RF_SYSCON Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RF_SYSCON_Register_Masks RF_SYSCON Register Masks
 * @{
 */

/*! @name WO_SCRATCH_REG - Write once scratch register 0..Write once scratch register 7 */
/*! @{ */

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG0_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG0_SHIFT (0U)
/*! WO_SCRATCH_REG0 - Write once scratch register 0 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG0(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG0_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG0_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG1_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG1_SHIFT (0U)
/*! WO_SCRATCH_REG1 - Write once scratch register 1 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG1(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG1_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG1_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG2_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG2_SHIFT (0U)
/*! WO_SCRATCH_REG2 - Write once scratch register 2 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG2(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG2_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG2_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG3_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG3_SHIFT (0U)
/*! WO_SCRATCH_REG3 - Write once scratch register 3 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG3(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG3_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG3_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG4_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG4_SHIFT (0U)
/*! WO_SCRATCH_REG4 - Write once scratch register 4 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG4(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG4_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG4_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG5_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG5_SHIFT (0U)
/*! WO_SCRATCH_REG5 - Write once scratch register 5 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG5(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG5_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG5_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG6_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG6_SHIFT (0U)
/*! WO_SCRATCH_REG6 - Write once scratch register 6 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG6(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG6_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG6_MASK)

#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG7_MASK (0xFFFFFFFFU)
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG7_SHIFT (0U)
/*! WO_SCRATCH_REG7 - Write once scratch register 7 */
#define RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG7(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG7_SHIFT)) & RF_SYSCON_WO_SCRATCH_REG_WO_SCRATCH_REG7_MASK)
/*! @} */

/*! @name RW_SCRATCH_REG - Scratch register 0..Scratch register 7 */
/*! @{ */

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG0_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG0_SHIFT (0U)
/*! RW_SCRATCH_REG0 - Scratch register 0 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG0(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG0_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG0_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG1_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG1_SHIFT (0U)
/*! RW_SCRATCH_REG1 - Scratch register 1 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG1(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG1_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG1_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG2_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG2_SHIFT (0U)
/*! RW_SCRATCH_REG2 - Scratch register 2 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG2(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG2_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG2_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG3_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG3_SHIFT (0U)
/*! RW_SCRATCH_REG3 - Scratch register 3 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG3(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG3_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG3_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG4_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG4_SHIFT (0U)
/*! RW_SCRATCH_REG4 - Scratch register 4 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG4(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG4_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG4_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG5_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG5_SHIFT (0U)
/*! RW_SCRATCH_REG5 - Scratch register 5 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG5(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG5_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG5_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG6_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG6_SHIFT (0U)
/*! RW_SCRATCH_REG6 - Scratch register 6 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG6(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG6_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG6_MASK)

#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG7_MASK (0xFFFFFFFFU)
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG7_SHIFT (0U)
/*! RW_SCRATCH_REG7 - Scratch register 7 */
#define RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG7(x) (((uint32_t)(((uint32_t)(x)) << RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG7_SHIFT)) & RF_SYSCON_RW_SCRATCH_REG_RW_SCRATCH_REG7_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group RF_SYSCON_Register_Masks */


/*!
 * @}
 */ /* end of group RF_SYSCON_Peripheral_Access_Layer */


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


#endif  /* RF_SYSCON_H_ */

