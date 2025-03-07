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
**         CMSIS Peripheral Access Layer for FLEXCOMM
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
 * @file FLEXCOMM.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for FLEXCOMM
 *
 * CMSIS Peripheral Access Layer for FLEXCOMM
 */

#if !defined(FLEXCOMM_H_)
#define FLEXCOMM_H_                              /**< Symbol preventing repeated inclusion */

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
   -- FLEXCOMM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FLEXCOMM_Peripheral_Access_Layer FLEXCOMM Peripheral Access Layer
 * @{
 */

/** FLEXCOMM - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[4088];
  __IO uint32_t PSELID;                            /**< Peripheral Select and Flexcomm module ID, offset: 0xFF8 */
  __I  uint32_t PID;                               /**< Peripheral Identification, offset: 0xFFC */
} FLEXCOMM_Type;

/* ----------------------------------------------------------------------------
   -- FLEXCOMM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FLEXCOMM_Register_Masks FLEXCOMM Register Masks
 * @{
 */

/*! @name PSELID - Peripheral Select and Flexcomm module ID */
/*! @{ */

#define FLEXCOMM_PSELID_PERSEL_MASK              (0x7U)
#define FLEXCOMM_PSELID_PERSEL_SHIFT             (0U)
/*! PERSEL - Peripheral Select
 *  0b000..No peripheral selected.
 *  0b001..USART function selected
 *  0b010..SPI function selected
 *  0b011..I2C
 *  0b100..I2S Transmit
 *  0b101..I2S Receive
 *  0b110..Reserved
 *  0b111..Reserved
 */
#define FLEXCOMM_PSELID_PERSEL(x)                (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_PERSEL_SHIFT)) & FLEXCOMM_PSELID_PERSEL_MASK)

#define FLEXCOMM_PSELID_LOCK_MASK                (0x8U)
#define FLEXCOMM_PSELID_LOCK_SHIFT               (3U)
/*! LOCK - Lock the peripheral select
 *  0b0..Peripheral select can be changed by software.
 *  0b1..Peripheral select is locked and cannot be changed until this Flexcomm module or the entire device is reset.
 */
#define FLEXCOMM_PSELID_LOCK(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_LOCK_SHIFT)) & FLEXCOMM_PSELID_LOCK_MASK)

#define FLEXCOMM_PSELID_USARTPRESENT_MASK        (0x10U)
#define FLEXCOMM_PSELID_USARTPRESENT_SHIFT       (4U)
/*! USARTPRESENT - USART present indicator
 *  0b0..This Flexcomm module does not include the USART function.
 *  0b1..This Flexcomm module includes the USART function.
 */
#define FLEXCOMM_PSELID_USARTPRESENT(x)          (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_USARTPRESENT_SHIFT)) & FLEXCOMM_PSELID_USARTPRESENT_MASK)

#define FLEXCOMM_PSELID_SPIPRESENT_MASK          (0x20U)
#define FLEXCOMM_PSELID_SPIPRESENT_SHIFT         (5U)
/*! SPIPRESENT - SPI present indicator
 *  0b0..This Flexcomm module does not include the SPI function.
 *  0b1..This Flexcomm module includes the SPI function.
 */
#define FLEXCOMM_PSELID_SPIPRESENT(x)            (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_SPIPRESENT_SHIFT)) & FLEXCOMM_PSELID_SPIPRESENT_MASK)

#define FLEXCOMM_PSELID_I2CPRESENT_MASK          (0x40U)
#define FLEXCOMM_PSELID_I2CPRESENT_SHIFT         (6U)
/*! I2CPRESENT - I2C present indicator
 *  0b0..I2C Not Present
 *  0b1..I2C Present
 */
#define FLEXCOMM_PSELID_I2CPRESENT(x)            (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_I2CPRESENT_SHIFT)) & FLEXCOMM_PSELID_I2CPRESENT_MASK)

#define FLEXCOMM_PSELID_I2SPRESENT_MASK          (0x80U)
#define FLEXCOMM_PSELID_I2SPRESENT_SHIFT         (7U)
/*! I2SPRESENT - I2S Present
 *  0b0..I2S Not Present
 *  0b1..I2S Present
 */
#define FLEXCOMM_PSELID_I2SPRESENT(x)            (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_I2SPRESENT_SHIFT)) & FLEXCOMM_PSELID_I2SPRESENT_MASK)

#define FLEXCOMM_PSELID_ID_MASK                  (0xFFFFF000U)
#define FLEXCOMM_PSELID_ID_SHIFT                 (12U)
/*! ID - Flexcomm ID */
#define FLEXCOMM_PSELID_ID(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PSELID_ID_SHIFT)) & FLEXCOMM_PSELID_ID_MASK)
/*! @} */

/*! @name PID - Peripheral Identification */
/*! @{ */

#define FLEXCOMM_PID_MINOR_REV_MASK              (0xF00U)
#define FLEXCOMM_PID_MINOR_REV_SHIFT             (8U)
/*! MINOR_REV - Minor revision of module implementation */
#define FLEXCOMM_PID_MINOR_REV(x)                (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PID_MINOR_REV_SHIFT)) & FLEXCOMM_PID_MINOR_REV_MASK)

#define FLEXCOMM_PID_MAJOR_REV_MASK              (0xF000U)
#define FLEXCOMM_PID_MAJOR_REV_SHIFT             (12U)
/*! MAJOR_REV - Major revision of module implementation */
#define FLEXCOMM_PID_MAJOR_REV(x)                (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PID_MAJOR_REV_SHIFT)) & FLEXCOMM_PID_MAJOR_REV_MASK)

#define FLEXCOMM_PID_ID_MASK                     (0xFFFF0000U)
#define FLEXCOMM_PID_ID_SHIFT                    (16U)
/*! ID - Module identifier for the selected function */
#define FLEXCOMM_PID_ID(x)                       (((uint32_t)(((uint32_t)(x)) << FLEXCOMM_PID_ID_SHIFT)) & FLEXCOMM_PID_ID_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group FLEXCOMM_Register_Masks */


/*!
 * @}
 */ /* end of group FLEXCOMM_Peripheral_Access_Layer */


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


#endif  /* FLEXCOMM_H_ */

