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
**         CMSIS Peripheral Access Layer for SYSCTL1
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
 * @file SYSCTL1.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SYSCTL1
 *
 * CMSIS Peripheral Access Layer for SYSCTL1
 */

#if !defined(SYSCTL1_H_)
#define SYSCTL1_H_                               /**< Symbol preventing repeated inclusion */

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
   -- SYSCTL1 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL1_Peripheral_Access_Layer SYSCTL1 Peripheral Access Layer
 * @{
 */

/** SYSCTL1 - Size of Registers Arrays */
#define SYSCTL1_FCCTRLSEL_COUNT                   4u
#define SYSCTL1_SHARECTRLSET_COUNT                2u

/** SYSCTL1 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t MCLKPINDIR;                        /**< mclk direction control, offset: 0x10 */
       uint8_t RESERVED_1[44];
  __IO uint32_t FCCTRLSEL[SYSCTL1_FCCTRLSEL_COUNT]; /**< flexcomm control selection N, array offset: 0x40, array step: 0x4 */
       uint8_t RESERVED_2[48];
  __IO uint32_t SHAREDCTRLSET[SYSCTL1_SHARECTRLSET_COUNT]; /**< shared control set N, array offset: 0x80, array step: 0x4 */
       uint8_t RESERVED_3[376];
  __IO uint32_t RXEVPULSEGEN;                      /**< RX Event Pulse Generator, offset: 0x200 */
} SYSCTL1_Type;

/* ----------------------------------------------------------------------------
   -- SYSCTL1 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL1_Register_Masks SYSCTL1 Register Masks
 * @{
 */

/*! @name MCLKPINDIR - mclk direction control */
/*! @{ */

#define SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK       (0x1U)
#define SYSCTL1_MCLKPINDIR_MCLKPINDIR_SHIFT      (0U)
/*! MCLKPINDIR - mclk direction control
 *  0b0..MCLK is in input direction.
 *  0b1..MCLK is in the output direction.
 */
#define SYSCTL1_MCLKPINDIR_MCLKPINDIR(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL1_MCLKPINDIR_MCLKPINDIR_SHIFT)) & SYSCTL1_MCLKPINDIR_MCLKPINDIR_MASK)
/*! @} */

/*! @name FCCTRLSEL - flexcomm control selection N */
/*! @{ */

#define SYSCTL1_FCCTRLSEL_SCKINSEL_MASK          (0x3U)
#define SYSCTL1_FCCTRLSEL_SCKINSEL_SHIFT         (0U)
/*! SCKINSEL - SCK IN Select. . .
 *  0b00..Original FLEXCOMM I2S signals
 *  0b01..Shared Set0 I2S signals.
 *  0b10..Shared Set1 I2S signals.
 *  0b11..Reserved.
 */
#define SYSCTL1_FCCTRLSEL_SCKINSEL(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL1_FCCTRLSEL_SCKINSEL_SHIFT)) & SYSCTL1_FCCTRLSEL_SCKINSEL_MASK)

#define SYSCTL1_FCCTRLSEL_WSINSEL_MASK           (0x300U)
#define SYSCTL1_FCCTRLSEL_WSINSEL_SHIFT          (8U)
/*! WSINSEL - WS IN Select. . .
 *  0b00..Original FLEXCOMM I2S signals
 *  0b01..Shared Set0 I2S signals.
 *  0b10..Shared Set1 I2S signals.
 *  0b11..Reserved.
 */
#define SYSCTL1_FCCTRLSEL_WSINSEL(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL1_FCCTRLSEL_WSINSEL_SHIFT)) & SYSCTL1_FCCTRLSEL_WSINSEL_MASK)

#define SYSCTL1_FCCTRLSEL_DATAINSEL_MASK         (0x30000U)
#define SYSCTL1_FCCTRLSEL_DATAINSEL_SHIFT        (16U)
/*! DATAINSEL - DATA IN Select. . .
 *  0b00..Original FLEXCOMM I2S signals
 *  0b01..Shared Set0 I2S signals.
 *  0b10..Shared Set1 I2S signals.
 *  0b11..Reserved.
 */
#define SYSCTL1_FCCTRLSEL_DATAINSEL(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL1_FCCTRLSEL_DATAINSEL_SHIFT)) & SYSCTL1_FCCTRLSEL_DATAINSEL_MASK)

#define SYSCTL1_FCCTRLSEL_DATAOUTSEL_MASK        (0x3000000U)
#define SYSCTL1_FCCTRLSEL_DATAOUTSEL_SHIFT       (24U)
/*! DATAOUTSEL - DATA OUT Select. . .
 *  0b00..Original FLEXCOMM I2S signals
 *  0b01..Shared Set0 I2S signals.
 *  0b10..Shared Set1 I2S signals.
 *  0b11..Reserved.
 */
#define SYSCTL1_FCCTRLSEL_DATAOUTSEL(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL1_FCCTRLSEL_DATAOUTSEL_SHIFT)) & SYSCTL1_FCCTRLSEL_DATAOUTSEL_MASK)
/*! @} */

/*! @name SHAREDCTRLSET - shared control set N */
/*! @{ */

#define SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL_MASK  (0x7U)
#define SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL_SHIFT (0U)
/*! SHAREDSCKSEL - Shared SCK Select. . .
 *  0b000..FLEXCOMM0
 *  0b001..FLEXCOMM1
 *  0b010..FLEXCOMM2
 *  0b011..FLEXCOMM3
 */
#define SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL_SHIFT)) & SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL_MASK)

#define SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL_MASK   (0x70U)
#define SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL_SHIFT  (4U)
/*! SHAREDWSSEL - Shared WS Select. . .
 *  0b000..FLEXCOMM0
 *  0b001..FLEXCOMM1
 *  0b010..FLEXCOMM2
 *  0b011..FLEXCOMM3
 */
#define SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL_SHIFT)) & SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL_MASK)

#define SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL_MASK (0x700U)
#define SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL_SHIFT (8U)
/*! SHAREDDATASEL - Shared DATA Select. . .
 *  0b000..FLEXCOMM0
 *  0b001..FLEXCOMM1
 *  0b010..FLEXCOMM2
 *  0b011..FLEXCOMM3
 */
#define SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL_SHIFT)) & SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL_MASK)

#define SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_MASK  (0x10000U)
#define SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_SHIFT (16U)
/*! FC0DATAOUTEN - FLEXCOMM0 DATAOUT OUTPUT ENABLE
 *  0b0..Input
 *  0b1..Output
 */
#define SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_SHIFT)) & SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_MASK)

#define SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN_MASK  (0x20000U)
#define SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN_SHIFT (17U)
/*! FC1DATAOUTEN - FLEXCOMM1 DATAOUT OUTPUT ENABLE
 *  0b0..Input
 *  0b1..Output
 */
#define SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN_SHIFT)) & SYSCTL1_SHAREDCTRLSET_FC1DATAOUTEN_MASK)

#define SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN_MASK  (0x40000U)
#define SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN_SHIFT (18U)
/*! FC2DATAOUTEN - FLEXCOMM2 DATAOUT OUTPUT ENABLE
 *  0b0..Input
 *  0b1..Output
 */
#define SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN_SHIFT)) & SYSCTL1_SHAREDCTRLSET_FC2DATAOUTEN_MASK)

#define SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN_MASK  (0x80000U)
#define SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN_SHIFT (19U)
/*! FC3DATAOUTEN - FLEXCOMM3 DATAOUT OUTPUT ENABLE
 *  0b0..Input
 *  0b1..Output
 */
#define SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN_SHIFT)) & SYSCTL1_SHAREDCTRLSET_FC3DATAOUTEN_MASK)
/*! @} */

/* The count of SYSCTL1_SHAREDCTRLSET */
#define SYSCTL1_SHAREDCTRLSET_COUNT              (2U)

/*! @name RXEVPULSEGEN - RX Event Pulse Generator */
/*! @{ */

#define SYSCTL1_RXEVPULSEGEN_RXEVPULSEGEN_MASK   (0x1U)
#define SYSCTL1_RXEVPULSEGEN_RXEVPULSEGEN_SHIFT  (0U)
/*! RXEVPULSEGEN - RX Event Pulse Generator. Writing a '1' to this register will create a one PSCLK
 *    pulse width of logic '1'. It is automatically cleared.
 *  0b0..No effect.
 *  0b1..Pulse RXEV High for one PSCLK cycle.
 */
#define SYSCTL1_RXEVPULSEGEN_RXEVPULSEGEN(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL1_RXEVPULSEGEN_RXEVPULSEGEN_SHIFT)) & SYSCTL1_RXEVPULSEGEN_RXEVPULSEGEN_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SYSCTL1_Register_Masks */


/*!
 * @}
 */ /* end of group SYSCTL1_Peripheral_Access_Layer */


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


#endif  /* SYSCTL1_H_ */

