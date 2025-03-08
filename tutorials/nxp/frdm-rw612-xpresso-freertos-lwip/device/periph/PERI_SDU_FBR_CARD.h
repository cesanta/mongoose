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
**         CMSIS Peripheral Access Layer for SDU_FBR_CARD
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
 * @file SDU_FBR_CARD.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SDU_FBR_CARD
 *
 * CMSIS Peripheral Access Layer for SDU_FBR_CARD
 */

#if !defined(SDU_FBR_CARD_H_)
#define SDU_FBR_CARD_H_                          /**< Symbol preventing repeated inclusion */

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
   -- SDU_FBR_CARD Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FBR_CARD_Peripheral_Access_Layer SDU_FBR_CARD Peripheral Access Layer
 * @{
 */

/** SDU_FBR_CARD - Register Layout Typedef */
typedef struct {
  __IO uint8_t FN_CODE;                            /**< Function Code, offset: 0x0 */
  __I  uint8_t FN_EXT_CODE;                        /**< Extended Function Code, offset: 0x1 */
  __IO uint8_t FN_POWER_SELECT;                    /**< Function Power Select, offset: 0x2 */
       uint8_t RESERVED_0[2];
  __I  uint8_t FN_CIS_0;                           /**< Function CIS Pointer 0, offset: 0x5 */
  __I  uint8_t FN_CIS_1;                           /**< Function CIS Pointer 1, offset: 0x6 */
  __I  uint8_t FN_CIS_2;                           /**< Function CIS Pointer 2, offset: 0x7 */
  __I  uint8_t FN_BLOCK_SIZE_0;                    /**< Function Block Size 0, offset: 0x8 */
  __I  uint8_t FN_BLOCK_SIZE_1;                    /**< Function Block Size 1, offset: 0x9 */
} SDU_FBR_CARD_Type;

/* ----------------------------------------------------------------------------
   -- SDU_FBR_CARD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FBR_CARD_Register_Masks SDU_FBR_CARD Register Masks
 * @{
 */

/*! @name FN_CODE - Function Code */
/*! @{ */

#define SDU_FBR_CARD_FN_CODE_CODE_MASK           (0xFU)
#define SDU_FBR_CARD_FN_CODE_CODE_SHIFT          (0U)
/*! CODE - Standard I/O device interface code */
#define SDU_FBR_CARD_FN_CODE_CODE(x)             (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CODE_CODE_SHIFT)) & SDU_FBR_CARD_FN_CODE_CODE_MASK)

#define SDU_FBR_CARD_FN_CODE_CSA_MASK            (0x40U)
#define SDU_FBR_CARD_FN_CODE_CSA_SHIFT           (6U)
/*! CSA - Function supports Code Storage Area (CSA) 0 = function does not support CSA 1 = function supports CSA */
#define SDU_FBR_CARD_FN_CODE_CSA(x)              (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CODE_CSA_SHIFT)) & SDU_FBR_CARD_FN_CODE_CSA_MASK)

#define SDU_FBR_CARD_FN_CODE_CSA_EN_MASK         (0x80U)
#define SDU_FBR_CARD_FN_CODE_CSA_EN_SHIFT        (7U)
/*! CSA_EN - Function CSA Enable */
#define SDU_FBR_CARD_FN_CODE_CSA_EN(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CODE_CSA_EN_SHIFT)) & SDU_FBR_CARD_FN_CODE_CSA_EN_MASK)
/*! @} */

/*! @name FN_EXT_CODE - Extended Function Code */
/*! @{ */

#define SDU_FBR_CARD_FN_EXT_CODE_FN_EXT_CODE_MASK (0xFFU)
#define SDU_FBR_CARD_FN_EXT_CODE_FN_EXT_CODE_SHIFT (0U)
/*! FN_EXT_CODE - Extended SDIO standard function interface code. */
#define SDU_FBR_CARD_FN_EXT_CODE_FN_EXT_CODE(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_EXT_CODE_FN_EXT_CODE_SHIFT)) & SDU_FBR_CARD_FN_EXT_CODE_FN_EXT_CODE_MASK)
/*! @} */

/*! @name FN_POWER_SELECT - Function Power Select */
/*! @{ */

#define SDU_FBR_CARD_FN_POWER_SELECT_SPS_MASK    (0x1U)
#define SDU_FBR_CARD_FN_POWER_SELECT_SPS_SHIFT   (0U)
/*! SPS - Power Selection 0 = no power selection 1 = two power modes selected by EPS bit */
#define SDU_FBR_CARD_FN_POWER_SELECT_SPS(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_POWER_SELECT_SPS_SHIFT)) & SDU_FBR_CARD_FN_POWER_SELECT_SPS_MASK)

#define SDU_FBR_CARD_FN_POWER_SELECT_EPS_MASK    (0x2U)
#define SDU_FBR_CARD_FN_POWER_SELECT_EPS_SHIFT   (1U)
/*! EPS - Current Mode 0 = function operates in high current mode 1 = function operates in low current mode */
#define SDU_FBR_CARD_FN_POWER_SELECT_EPS(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_POWER_SELECT_EPS_SHIFT)) & SDU_FBR_CARD_FN_POWER_SELECT_EPS_MASK)

#define SDU_FBR_CARD_FN_POWER_SELECT_FN_PS_MASK  (0xF0U)
#define SDU_FBR_CARD_FN_POWER_SELECT_FN_PS_SHIFT (4U)
/*! FN_PS - Power State */
#define SDU_FBR_CARD_FN_POWER_SELECT_FN_PS(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_POWER_SELECT_FN_PS_SHIFT)) & SDU_FBR_CARD_FN_POWER_SELECT_FN_PS_MASK)
/*! @} */

/*! @name FN_CIS_0 - Function CIS Pointer 0 */
/*! @{ */

#define SDU_FBR_CARD_FN_CIS_0_FN_CIS_PTR0_MASK   (0xFFU)
#define SDU_FBR_CARD_FN_CIS_0_FN_CIS_PTR0_SHIFT  (0U)
/*! FN_CIS_PTR0 - 24b Function Pointer [7:0] Function card information structure pointer [7:0]
 *    Function n CIS pointer is (0x8000 | n << 7) Therefore, Function 1 CIS pointer is 0x008080 Function
 *    2 CIS pointer is 0x008100 Function 3 CIS pointer is 0x008180 ... Function 7 CIS pointer is
 *    0x008380
 */
#define SDU_FBR_CARD_FN_CIS_0_FN_CIS_PTR0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CIS_0_FN_CIS_PTR0_SHIFT)) & SDU_FBR_CARD_FN_CIS_0_FN_CIS_PTR0_MASK)
/*! @} */

/*! @name FN_CIS_1 - Function CIS Pointer 1 */
/*! @{ */

#define SDU_FBR_CARD_FN_CIS_1_FN_CIS_PTR1_MASK   (0xFFU)
#define SDU_FBR_CARD_FN_CIS_1_FN_CIS_PTR1_SHIFT  (0U)
/*! FN_CIS_PTR1 - 24b Function 1 Pointer [15:8] Function card information structure pointer [15:8] */
#define SDU_FBR_CARD_FN_CIS_1_FN_CIS_PTR1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CIS_1_FN_CIS_PTR1_SHIFT)) & SDU_FBR_CARD_FN_CIS_1_FN_CIS_PTR1_MASK)
/*! @} */

/*! @name FN_CIS_2 - Function CIS Pointer 2 */
/*! @{ */

#define SDU_FBR_CARD_FN_CIS_2_FN_CIS_PTR2_MASK   (0xFFU)
#define SDU_FBR_CARD_FN_CIS_2_FN_CIS_PTR2_SHIFT  (0U)
/*! FN_CIS_PTR2 - 24b Function 1 Pointer [23:16] Function card information structure pointer [23:16] */
#define SDU_FBR_CARD_FN_CIS_2_FN_CIS_PTR2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_CIS_2_FN_CIS_PTR2_SHIFT)) & SDU_FBR_CARD_FN_CIS_2_FN_CIS_PTR2_MASK)
/*! @} */

/*! @name FN_BLOCK_SIZE_0 - Function Block Size 0 */
/*! @{ */

#define SDU_FBR_CARD_FN_BLOCK_SIZE_0_FN_BLK_SIZE0_MASK (0xFFU)
#define SDU_FBR_CARD_FN_BLOCK_SIZE_0_FN_BLK_SIZE0_SHIFT (0U)
/*! FN_BLK_SIZE0 - Block Size [7:0] for I/O Block Operation */
#define SDU_FBR_CARD_FN_BLOCK_SIZE_0_FN_BLK_SIZE0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_BLOCK_SIZE_0_FN_BLK_SIZE0_SHIFT)) & SDU_FBR_CARD_FN_BLOCK_SIZE_0_FN_BLK_SIZE0_MASK)
/*! @} */

/*! @name FN_BLOCK_SIZE_1 - Function Block Size 1 */
/*! @{ */

#define SDU_FBR_CARD_FN_BLOCK_SIZE_1_FN_BLK_SIZE1_MASK (0x1U)
#define SDU_FBR_CARD_FN_BLOCK_SIZE_1_FN_BLK_SIZE1_SHIFT (0U)
/*! FN_BLK_SIZE1 - Block Size [8] for I/O Block Operation */
#define SDU_FBR_CARD_FN_BLOCK_SIZE_1_FN_BLK_SIZE1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FBR_CARD_FN_BLOCK_SIZE_1_FN_BLK_SIZE1_SHIFT)) & SDU_FBR_CARD_FN_BLOCK_SIZE_1_FN_BLK_SIZE1_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SDU_FBR_CARD_Register_Masks */


/*!
 * @}
 */ /* end of group SDU_FBR_CARD_Peripheral_Access_Layer */


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


#endif  /* SDU_FBR_CARD_H_ */

