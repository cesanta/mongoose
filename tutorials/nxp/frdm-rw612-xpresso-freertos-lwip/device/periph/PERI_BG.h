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
**         CMSIS Peripheral Access Layer for BG
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
 * @file BG.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for BG
 *
 * CMSIS Peripheral Access Layer for BG
 */

#if !defined(BG_H_)
#define BG_H_                                    /**< Symbol preventing repeated inclusion */

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
   -- BG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BG_Peripheral_Access_Layer BG Peripheral Access Layer
 * @{
 */

/** BG - Register Layout Typedef */
typedef struct {
  __IO uint32_t CTRL;                              /**< Control Register, offset: 0x0 */
  __I  uint32_t STATUS;                            /**< Status Register, offset: 0x4 */
} BG_Type;

/* ----------------------------------------------------------------------------
   -- BG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BG_Register_Masks BG Register Masks
 * @{
 */

/*! @name CTRL - Control Register */
/*! @{ */

#define BG_CTRL_PD_MASK                          (0x1U)
#define BG_CTRL_PD_SHIFT                         (0U)
/*! PD - Bandgap power down.
 *  0b1..Power down
 *  0b0..Power up
 */
#define BG_CTRL_PD(x)                            (((uint32_t)(((uint32_t)(x)) << BG_CTRL_PD_SHIFT)) & BG_CTRL_PD_MASK)

#define BG_CTRL_RES_TRIM_MASK                    (0xF0U)
#define BG_CTRL_RES_TRIM_SHIFT                   (4U)
/*! RES_TRIM - 1.2V voltage reference resistor trim.
 *  0b0000..1.159V
 *  0b0001..1.163V
 *  0b0010..1.168V
 *  0b0011..1.172V
 *  0b0100..1.177V
 *  0b0101..1.181V
 *  0b0110..1.186V
 *  0b0111..1.190V
 *  0b1000..1.194V
 *  0b1001..1.199V
 *  0b1010..1.204V
 *  0b1011..1.208V
 *  0b1100..1.213V
 *  0b1101..1.217V
 *  0b1110..1.222V
 *  0b1111..1.226V
 */
#define BG_CTRL_RES_TRIM(x)                      (((uint32_t)(((uint32_t)(x)) << BG_CTRL_RES_TRIM_SHIFT)) & BG_CTRL_RES_TRIM_MASK)
/*! @} */

/*! @name STATUS - Status Register */
/*! @{ */

#define BG_STATUS_RDY_MASK                       (0x1U)
#define BG_STATUS_RDY_SHIFT                      (0U)
/*! RDY - 1'b1 indicates BG ready flag. */
#define BG_STATUS_RDY(x)                         (((uint32_t)(((uint32_t)(x)) << BG_STATUS_RDY_SHIFT)) & BG_STATUS_RDY_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group BG_Register_Masks */


/*!
 * @}
 */ /* end of group BG_Peripheral_Access_Layer */


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


#endif  /* BG_H_ */

