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
**         CMSIS Peripheral Access Layer for BUCK11
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
 * @file BUCK11.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for BUCK11
 *
 * CMSIS Peripheral Access Layer for BUCK11
 */

#if !defined(BUCK11_H_)
#define BUCK11_H_                                /**< Symbol preventing repeated inclusion */

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
   -- BUCK11 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BUCK11_Peripheral_Access_Layer BUCK11 Peripheral Access Layer
 * @{
 */

/** BUCK11 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[64];
  __I  uint8_t BUCK_BYPASS_SOC_CTRL_ONE_RO_REG;    /**< offset: 0x40 */
  __I  uint8_t BUCK_BYPASS_SOC_CTRL_TWO_RO_REG;    /**< offset: 0x41 */
  __I  uint8_t REG_RO_ONE_REG;                     /**< offset: 0x42 */
  __I  uint8_t REG_RO_TWO_REG;                     /**< offset: 0x43 */
  __I  uint8_t REG_RO_THREE_REG;                   /**< offset: 0x44 */
  __I  uint8_t REG_RO_FOUR_REG;                    /**< offset: 0x45 */
  __IO uint8_t SYS_CTRL_REG;                       /**< offset: 0x46 */
  __IO uint8_t BUCK_BYPASS_SOC_CTRL_ONE_RW_REG;    /**< offset: 0x47 */
  __IO uint8_t BUCK_BYPASS_SOC_CTRL_TWO_RW_REG;    /**< offset: 0x48 */
  __IO uint8_t BUCK_CTRL_ONE_REG;                  /**< offset: 0x49 */
  __IO uint8_t BUCK_CTRL_TWO_REG;                  /**< offset: 0x4A */
  __IO uint8_t BUCK_CTRL_THREE_REG;                /**< offset: 0x4B */
  __IO uint8_t BUCK_CTRL_FOUR_REG;                 /**< offset: 0x4C */
  __IO uint8_t BUCK_CTRL_FIVE_REG;                 /**< offset: 0x4D */
  __IO uint8_t BUCK_CTRL_SIX_REG;                  /**< offset: 0x4E */
  __IO uint8_t BUCK_CTRL_SEVEN_REG;                /**< offset: 0x4F */
  __IO uint8_t BUCK_CTRL_EIGHT_REG;                /**< offset: 0x50 */
  __IO uint8_t BUCK_CTRL_NINE_REG;                 /**< offset: 0x51 */
  __IO uint8_t BUCK_CTRL_TEN_REG;                  /**< offset: 0x52 */
  __IO uint8_t BUCK_CTRL_ELEVEN_REG;               /**< offset: 0x53 */
  __IO uint8_t BUCK_CTRL_TWELVE_REG;               /**< offset: 0x54 */
  __IO uint8_t BUCK_CTRL_THIRTEEN_REG;             /**< offset: 0x55 */
  __IO uint8_t BUCK_CTRL_FOURTEEN_REG;             /**< offset: 0x56 */
  __IO uint8_t BUCK_CTRL_FIFTEEN_REG;              /**< offset: 0x57 */
  __IO uint8_t BUCK_CTRL_SIXTEEN_REG;              /**< offset: 0x58 */
  __IO uint8_t BUCK_CTRL_SEVENTEEN_REG;            /**< offset: 0x59 */
  __IO uint8_t BUCK_CTRL_EIGHTEEN_REG;             /**< offset: 0x5A */
  __IO uint8_t BUCK_CTRL_NINTEEN_REG;              /**< offset: 0x5B */
  __IO uint8_t BUCK_CTRL_TWENTY_REG;               /**< offset: 0x5C */
} BUCK11_Type;

/* ----------------------------------------------------------------------------
   -- BUCK11 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup BUCK11_Register_Masks BUCK11 Register Masks
 * @{
 */

/*! @name BUCK_BYPASS_SOC_CTRL_ONE_RO_REG -  */
/*! @{ */

#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RO_REG_BUCK_BYPASS_SOC_CTRL_ONE_RO_MASK (0xFFU)
#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RO_REG_BUCK_BYPASS_SOC_CTRL_ONE_RO_SHIFT (0U)
/*! BUCK_BYPASS_SOC_CTRL_ONE_RO - BUCK_BYPASS_SOC_CTRL_ONE_RO */
#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RO_REG_BUCK_BYPASS_SOC_CTRL_ONE_RO(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RO_REG_BUCK_BYPASS_SOC_CTRL_ONE_RO_SHIFT)) & BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RO_REG_BUCK_BYPASS_SOC_CTRL_ONE_RO_MASK)
/*! @} */

/*! @name BUCK_BYPASS_SOC_CTRL_TWO_RO_REG -  */
/*! @{ */

#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RO_REG_BUCK_BYPASS_SOC_CTRL_TWO_RO_MASK (0xFFU)
#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RO_REG_BUCK_BYPASS_SOC_CTRL_TWO_RO_SHIFT (0U)
/*! BUCK_BYPASS_SOC_CTRL_TWO_RO - BUCK_BYPASS_SOC_CTRL_TWO_RO */
#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RO_REG_BUCK_BYPASS_SOC_CTRL_TWO_RO(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RO_REG_BUCK_BYPASS_SOC_CTRL_TWO_RO_SHIFT)) & BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RO_REG_BUCK_BYPASS_SOC_CTRL_TWO_RO_MASK)
/*! @} */

/*! @name REG_RO_ONE_REG -  */
/*! @{ */

#define BUCK11_REG_RO_ONE_REG_REG_RO_ONE_MASK    (0xFFU)
#define BUCK11_REG_RO_ONE_REG_REG_RO_ONE_SHIFT   (0U)
/*! REG_RO_ONE - REG_RO_ONE */
#define BUCK11_REG_RO_ONE_REG_REG_RO_ONE(x)      (((uint8_t)(((uint8_t)(x)) << BUCK11_REG_RO_ONE_REG_REG_RO_ONE_SHIFT)) & BUCK11_REG_RO_ONE_REG_REG_RO_ONE_MASK)
/*! @} */

/*! @name REG_RO_TWO_REG -  */
/*! @{ */

#define BUCK11_REG_RO_TWO_REG_REG_RO_TWO_MASK    (0xFFU)
#define BUCK11_REG_RO_TWO_REG_REG_RO_TWO_SHIFT   (0U)
/*! REG_RO_TWO - REG_RO_TWO */
#define BUCK11_REG_RO_TWO_REG_REG_RO_TWO(x)      (((uint8_t)(((uint8_t)(x)) << BUCK11_REG_RO_TWO_REG_REG_RO_TWO_SHIFT)) & BUCK11_REG_RO_TWO_REG_REG_RO_TWO_MASK)
/*! @} */

/*! @name REG_RO_THREE_REG -  */
/*! @{ */

#define BUCK11_REG_RO_THREE_REG_REG_RO_THREE_MASK (0xFFU)
#define BUCK11_REG_RO_THREE_REG_REG_RO_THREE_SHIFT (0U)
/*! REG_RO_THREE - REG_RO_THREE */
#define BUCK11_REG_RO_THREE_REG_REG_RO_THREE(x)  (((uint8_t)(((uint8_t)(x)) << BUCK11_REG_RO_THREE_REG_REG_RO_THREE_SHIFT)) & BUCK11_REG_RO_THREE_REG_REG_RO_THREE_MASK)
/*! @} */

/*! @name REG_RO_FOUR_REG -  */
/*! @{ */

#define BUCK11_REG_RO_FOUR_REG_REG_RO_FOUR_MASK  (0xFFU)
#define BUCK11_REG_RO_FOUR_REG_REG_RO_FOUR_SHIFT (0U)
/*! REG_RO_FOUR - REG_RO_FOUR */
#define BUCK11_REG_RO_FOUR_REG_REG_RO_FOUR(x)    (((uint8_t)(((uint8_t)(x)) << BUCK11_REG_RO_FOUR_REG_REG_RO_FOUR_SHIFT)) & BUCK11_REG_RO_FOUR_REG_REG_RO_FOUR_MASK)
/*! @} */

/*! @name SYS_CTRL_REG -  */
/*! @{ */

#define BUCK11_SYS_CTRL_REG_REG_SYS_CTRL_MASK    (0xFFU)
#define BUCK11_SYS_CTRL_REG_REG_SYS_CTRL_SHIFT   (0U)
/*! REG_SYS_CTRL - REG_SYS_CTRL */
#define BUCK11_SYS_CTRL_REG_REG_SYS_CTRL(x)      (((uint8_t)(((uint8_t)(x)) << BUCK11_SYS_CTRL_REG_REG_SYS_CTRL_SHIFT)) & BUCK11_SYS_CTRL_REG_REG_SYS_CTRL_MASK)
/*! @} */

/*! @name BUCK_BYPASS_SOC_CTRL_ONE_RW_REG -  */
/*! @{ */

#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_ONE_RW_MASK (0xFFU)
#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_ONE_RW_SHIFT (0U)
/*! REG_BUCK_BYPASS_SOC_CTRL_ONE_RW - REG_BUCK_BYPASS_SOC_CTRL_ONE_RW */
#define BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_ONE_RW(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_ONE_RW_SHIFT)) & BUCK11_BUCK_BYPASS_SOC_CTRL_ONE_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_ONE_RW_MASK)
/*! @} */

/*! @name BUCK_BYPASS_SOC_CTRL_TWO_RW_REG -  */
/*! @{ */

#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_TWO_RW_MASK (0xFFU)
#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_TWO_RW_SHIFT (0U)
/*! REG_BUCK_BYPASS_SOC_CTRL_TWO_RW - REG_BUCK_BYPASS_SOC_CTRL_TWO_RW */
#define BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_TWO_RW(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_TWO_RW_SHIFT)) & BUCK11_BUCK_BYPASS_SOC_CTRL_TWO_RW_REG_REG_BUCK_BYPASS_SOC_CTRL_TWO_RW_MASK)
/*! @} */

/*! @name BUCK_CTRL_ONE_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_ONE_REG_REG_BUCK_CTRL_ONE_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_ONE_REG_REG_BUCK_CTRL_ONE_SHIFT (0U)
/*! REG_BUCK_CTRL_ONE - REG_BUCK_CTRL_ONE */
#define BUCK11_BUCK_CTRL_ONE_REG_REG_BUCK_CTRL_ONE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_ONE_REG_REG_BUCK_CTRL_ONE_SHIFT)) & BUCK11_BUCK_CTRL_ONE_REG_REG_BUCK_CTRL_ONE_MASK)
/*! @} */

/*! @name BUCK_CTRL_TWO_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_TWO_REG_REG_BUCK_CTRL_TWO_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_TWO_REG_REG_BUCK_CTRL_TWO_SHIFT (0U)
/*! REG_BUCK_CTRL_TWO - REG_BUCK_CTRL_TWO */
#define BUCK11_BUCK_CTRL_TWO_REG_REG_BUCK_CTRL_TWO(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_TWO_REG_REG_BUCK_CTRL_TWO_SHIFT)) & BUCK11_BUCK_CTRL_TWO_REG_REG_BUCK_CTRL_TWO_MASK)
/*! @} */

/*! @name BUCK_CTRL_THREE_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_THREE_REG_REG_BUCK_CTRL_THREE_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_THREE_REG_REG_BUCK_CTRL_THREE_SHIFT (0U)
/*! REG_BUCK_CTRL_THREE - REG_BUCK_CTRL_THREE */
#define BUCK11_BUCK_CTRL_THREE_REG_REG_BUCK_CTRL_THREE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_THREE_REG_REG_BUCK_CTRL_THREE_SHIFT)) & BUCK11_BUCK_CTRL_THREE_REG_REG_BUCK_CTRL_THREE_MASK)
/*! @} */

/*! @name BUCK_CTRL_FOUR_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_FOUR_REG_REG_BUCK_CTRL_FOUR_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_FOUR_REG_REG_BUCK_CTRL_FOUR_SHIFT (0U)
/*! REG_BUCK_CTRL_FOUR - REG_BUCK_CTRL_FOUR */
#define BUCK11_BUCK_CTRL_FOUR_REG_REG_BUCK_CTRL_FOUR(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_FOUR_REG_REG_BUCK_CTRL_FOUR_SHIFT)) & BUCK11_BUCK_CTRL_FOUR_REG_REG_BUCK_CTRL_FOUR_MASK)
/*! @} */

/*! @name BUCK_CTRL_FIVE_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_FIVE_REG_REG_BUCK_CTRL_FIVE_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_FIVE_REG_REG_BUCK_CTRL_FIVE_SHIFT (0U)
/*! REG_BUCK_CTRL_FIVE - REG_BUCK_CTRL_FIVE */
#define BUCK11_BUCK_CTRL_FIVE_REG_REG_BUCK_CTRL_FIVE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_FIVE_REG_REG_BUCK_CTRL_FIVE_SHIFT)) & BUCK11_BUCK_CTRL_FIVE_REG_REG_BUCK_CTRL_FIVE_MASK)
/*! @} */

/*! @name BUCK_CTRL_SIX_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_SIX_REG_REG_BUCK_CTRL_SIX_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_SIX_REG_REG_BUCK_CTRL_SIX_SHIFT (0U)
/*! REG_BUCK_CTRL_SIX - REG_BUCK_CTRL_SIX */
#define BUCK11_BUCK_CTRL_SIX_REG_REG_BUCK_CTRL_SIX(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_SIX_REG_REG_BUCK_CTRL_SIX_SHIFT)) & BUCK11_BUCK_CTRL_SIX_REG_REG_BUCK_CTRL_SIX_MASK)
/*! @} */

/*! @name BUCK_CTRL_SEVEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_SEVEN_REG_REG_BUCK_CTRL_SEVEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_SEVEN_REG_REG_BUCK_CTRL_SEVEN_SHIFT (0U)
/*! REG_BUCK_CTRL_SEVEN - REG_BUCK_CTRL_SEVEN */
#define BUCK11_BUCK_CTRL_SEVEN_REG_REG_BUCK_CTRL_SEVEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_SEVEN_REG_REG_BUCK_CTRL_SEVEN_SHIFT)) & BUCK11_BUCK_CTRL_SEVEN_REG_REG_BUCK_CTRL_SEVEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_EIGHT_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_EIGHT_REG_REG_BUCK_CTRL_EIGHT_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_EIGHT_REG_REG_BUCK_CTRL_EIGHT_SHIFT (0U)
/*! REG_BUCK_CTRL_EIGHT - REG_BUCK_CTRL_EIGHT */
#define BUCK11_BUCK_CTRL_EIGHT_REG_REG_BUCK_CTRL_EIGHT(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_EIGHT_REG_REG_BUCK_CTRL_EIGHT_SHIFT)) & BUCK11_BUCK_CTRL_EIGHT_REG_REG_BUCK_CTRL_EIGHT_MASK)
/*! @} */

/*! @name BUCK_CTRL_NINE_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_NINE_REG_REG_BUCK_CTRL_NINE_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_NINE_REG_REG_BUCK_CTRL_NINE_SHIFT (0U)
/*! REG_BUCK_CTRL_NINE - REG_BUCK_CTRL_NINE */
#define BUCK11_BUCK_CTRL_NINE_REG_REG_BUCK_CTRL_NINE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_NINE_REG_REG_BUCK_CTRL_NINE_SHIFT)) & BUCK11_BUCK_CTRL_NINE_REG_REG_BUCK_CTRL_NINE_MASK)
/*! @} */

/*! @name BUCK_CTRL_TEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_TEN_REG_REG_BUCK_CTRL_TEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_TEN_REG_REG_BUCK_CTRL_TEN_SHIFT (0U)
/*! REG_BUCK_CTRL_TEN - REG_BUCK_CTRL_TEN */
#define BUCK11_BUCK_CTRL_TEN_REG_REG_BUCK_CTRL_TEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_TEN_REG_REG_BUCK_CTRL_TEN_SHIFT)) & BUCK11_BUCK_CTRL_TEN_REG_REG_BUCK_CTRL_TEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_ELEVEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_ELEVEN_REG_REG_BUCK_CTRL_ELEVEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_ELEVEN_REG_REG_BUCK_CTRL_ELEVEN_SHIFT (0U)
/*! REG_BUCK_CTRL_ELEVEN - REG_BUCK_CTRL_ELEVEN */
#define BUCK11_BUCK_CTRL_ELEVEN_REG_REG_BUCK_CTRL_ELEVEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_ELEVEN_REG_REG_BUCK_CTRL_ELEVEN_SHIFT)) & BUCK11_BUCK_CTRL_ELEVEN_REG_REG_BUCK_CTRL_ELEVEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_TWELVE_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_TWELVE_REG_REG_BUCK_CTRL_TWELVE_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_TWELVE_REG_REG_BUCK_CTRL_TWELVE_SHIFT (0U)
/*! REG_BUCK_CTRL_TWELVE - REG_BUCK_CTRL_TWELVE */
#define BUCK11_BUCK_CTRL_TWELVE_REG_REG_BUCK_CTRL_TWELVE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_TWELVE_REG_REG_BUCK_CTRL_TWELVE_SHIFT)) & BUCK11_BUCK_CTRL_TWELVE_REG_REG_BUCK_CTRL_TWELVE_MASK)
/*! @} */

/*! @name BUCK_CTRL_THIRTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_THIRTEEN_REG_REG_BUCK_CTRL_THIRTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_THIRTEEN_REG_REG_BUCK_CTRL_THIRTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_THIRTEEN - REG_BUCK_CTRL_THIRTEEN */
#define BUCK11_BUCK_CTRL_THIRTEEN_REG_REG_BUCK_CTRL_THIRTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_THIRTEEN_REG_REG_BUCK_CTRL_THIRTEEN_SHIFT)) & BUCK11_BUCK_CTRL_THIRTEEN_REG_REG_BUCK_CTRL_THIRTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_FOURTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_FOURTEEN_REG_REG_BUCK_CTRL_FOURTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_FOURTEEN_REG_REG_BUCK_CTRL_FOURTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_FOURTEEN - REG_BUCK_CTRL_FOURTEEN */
#define BUCK11_BUCK_CTRL_FOURTEEN_REG_REG_BUCK_CTRL_FOURTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_FOURTEEN_REG_REG_BUCK_CTRL_FOURTEEN_SHIFT)) & BUCK11_BUCK_CTRL_FOURTEEN_REG_REG_BUCK_CTRL_FOURTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_FIFTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_FIFTEEN_REG_REG_BUCK_CTRL_FIFTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_FIFTEEN_REG_REG_BUCK_CTRL_FIFTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_FIFTEEN - REG_BUCK_CTRL_FIFTEEN */
#define BUCK11_BUCK_CTRL_FIFTEEN_REG_REG_BUCK_CTRL_FIFTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_FIFTEEN_REG_REG_BUCK_CTRL_FIFTEEN_SHIFT)) & BUCK11_BUCK_CTRL_FIFTEEN_REG_REG_BUCK_CTRL_FIFTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_SIXTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_SIXTEEN_REG_REG_BUCK_CTRL_SIXTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_SIXTEEN_REG_REG_BUCK_CTRL_SIXTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_SIXTEEN - REG_BUCK_CTRL_SIXTEEN */
#define BUCK11_BUCK_CTRL_SIXTEEN_REG_REG_BUCK_CTRL_SIXTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_SIXTEEN_REG_REG_BUCK_CTRL_SIXTEEN_SHIFT)) & BUCK11_BUCK_CTRL_SIXTEEN_REG_REG_BUCK_CTRL_SIXTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_SEVENTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_SEVENTEEN_REG_REG_BUCK_CTRL_SEVENTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_SEVENTEEN_REG_REG_BUCK_CTRL_SEVENTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_SEVENTEEN - REG_BUCK_CTRL_SEVENTEEN */
#define BUCK11_BUCK_CTRL_SEVENTEEN_REG_REG_BUCK_CTRL_SEVENTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_SEVENTEEN_REG_REG_BUCK_CTRL_SEVENTEEN_SHIFT)) & BUCK11_BUCK_CTRL_SEVENTEEN_REG_REG_BUCK_CTRL_SEVENTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_EIGHTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_EXT_BG_CAL_CODE_MASK (0xFU)
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_EXT_BG_CAL_CODE_SHIFT (0U)
/*! EXT_BG_CAL_CODE - External calibration code for BG */
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_EXT_BG_CAL_CODE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_EIGHTEEN_REG_EXT_BG_CAL_CODE_SHIFT)) & BUCK11_BUCK_CTRL_EIGHTEEN_REG_EXT_BG_CAL_CODE_MASK)

#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SEL_BG_CAL_CODE_MASK (0x10U)
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SEL_BG_CAL_CODE_SHIFT (4U)
/*! SEL_BG_CAL_CODE - Select internal or external calibration code for BG */
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SEL_BG_CAL_CODE(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_EIGHTEEN_REG_SEL_BG_CAL_CODE_SHIFT)) & BUCK11_BUCK_CTRL_EIGHTEEN_REG_SEL_BG_CAL_CODE_MASK)

#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SHORT_C11_ENB_MASK (0x20U)
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SHORT_C11_ENB_SHIFT (5U)
/*! SHORT_C11_ENB - Short switch enable signal for internal C11 */
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_SHORT_C11_ENB(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_EIGHTEEN_REG_SHORT_C11_ENB_SHIFT)) & BUCK11_BUCK_CTRL_EIGHTEEN_REG_SHORT_C11_ENB_MASK)

#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_LPBG_TRIM_MASK (0xC0U)
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_LPBG_TRIM_SHIFT (6U)
/*! LPBG_TRIM - Bandgap temperature curve trimming */
#define BUCK11_BUCK_CTRL_EIGHTEEN_REG_LPBG_TRIM(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_EIGHTEEN_REG_LPBG_TRIM_SHIFT)) & BUCK11_BUCK_CTRL_EIGHTEEN_REG_LPBG_TRIM_MASK)
/*! @} */

/*! @name BUCK_CTRL_NINTEEN_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_NINTEEN_REG_REG_BUCK_CTRL_NINTEEN_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_NINTEEN_REG_REG_BUCK_CTRL_NINTEEN_SHIFT (0U)
/*! REG_BUCK_CTRL_NINTEEN - REG_BUCK_CTRL_NINTEEN */
#define BUCK11_BUCK_CTRL_NINTEEN_REG_REG_BUCK_CTRL_NINTEEN(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_NINTEEN_REG_REG_BUCK_CTRL_NINTEEN_SHIFT)) & BUCK11_BUCK_CTRL_NINTEEN_REG_REG_BUCK_CTRL_NINTEEN_MASK)
/*! @} */

/*! @name BUCK_CTRL_TWENTY_REG -  */
/*! @{ */

#define BUCK11_BUCK_CTRL_TWENTY_REG_REG_BUCK_CTRL_TWENTY_MASK (0xFFU)
#define BUCK11_BUCK_CTRL_TWENTY_REG_REG_BUCK_CTRL_TWENTY_SHIFT (0U)
/*! REG_BUCK_CTRL_TWENTY - REG_BUCK_CTRL_TWENTY */
#define BUCK11_BUCK_CTRL_TWENTY_REG_REG_BUCK_CTRL_TWENTY(x) (((uint8_t)(((uint8_t)(x)) << BUCK11_BUCK_CTRL_TWENTY_REG_REG_BUCK_CTRL_TWENTY_SHIFT)) & BUCK11_BUCK_CTRL_TWENTY_REG_REG_BUCK_CTRL_TWENTY_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group BUCK11_Register_Masks */


/*!
 * @}
 */ /* end of group BUCK11_Peripheral_Access_Layer */


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


#endif  /* BUCK11_H_ */

