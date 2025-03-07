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
**         CMSIS Peripheral Access Layer for DBGMAILBOX
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
 * @file DBGMAILBOX.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for DBGMAILBOX
 *
 * CMSIS Peripheral Access Layer for DBGMAILBOX
 */

#if !defined(DBGMAILBOX_H_)
#define DBGMAILBOX_H_                            /**< Symbol preventing repeated inclusion */

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
   -- DBGMAILBOX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DBGMAILBOX_Peripheral_Access_Layer DBGMAILBOX Peripheral Access Layer
 * @{
 */

/** DBGMAILBOX - Register Layout Typedef */
typedef struct {
  __IO uint32_t CSW;                               /**< Command and status word, offset: 0x0 */
  __IO uint32_t REQUEST;                           /**< Request Value, offset: 0x4 */
  __IO uint32_t RETURN;                            /**< Return Value, offset: 0x8 */
       uint8_t RESERVED_0[240];
  __I  uint32_t ID;                                /**< Identification, offset: 0xFC */
} DBGMAILBOX_Type;

/* ----------------------------------------------------------------------------
   -- DBGMAILBOX Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DBGMAILBOX_Register_Masks DBGMAILBOX Register Masks
 * @{
 */

/*! @name CSW - Command and status word */
/*! @{ */

#define DBGMAILBOX_CSW_RESYNCH_REQ_MASK          (0x1U)
#define DBGMAILBOX_CSW_RESYNCH_REQ_SHIFT         (0U)
/*! RESYNCH_REQ - Re-synchronization Request
 *  0b0..No Request
 *  0b1..Request for re-synchronization
 */
#define DBGMAILBOX_CSW_RESYNCH_REQ(x)            (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_RESYNCH_REQ_SHIFT)) & DBGMAILBOX_CSW_RESYNCH_REQ_MASK)

#define DBGMAILBOX_CSW_REQ_PENDING_MASK          (0x2U)
#define DBGMAILBOX_CSW_REQ_PENDING_SHIFT         (1U)
/*! REQ_PENDING - Request Pending
 *  0b0..No Request Pending
 *  0b1..Request for Re-synchronization Pending
 */
#define DBGMAILBOX_CSW_REQ_PENDING(x)            (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_REQ_PENDING_SHIFT)) & DBGMAILBOX_CSW_REQ_PENDING_MASK)

#define DBGMAILBOX_CSW_DBG_OR_ERR_MASK           (0x4U)
#define DBGMAILBOX_CSW_DBG_OR_ERR_SHIFT          (2U)
/*! DBG_OR_ERR - Debug Overrun Error
 *  0b0..No Debug Overrun error
 *  0b1..Debug Overrun Error. A debug overrun occurred.
 */
#define DBGMAILBOX_CSW_DBG_OR_ERR(x)             (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_DBG_OR_ERR_SHIFT)) & DBGMAILBOX_CSW_DBG_OR_ERR_MASK)

#define DBGMAILBOX_CSW_AHB_OR_ERR_MASK           (0x8U)
#define DBGMAILBOX_CSW_AHB_OR_ERR_SHIFT          (3U)
/*! AHB_OR_ERR - AHB Overrun Error
 *  0b0..No AHB Overrun Error
 *  0b1..AHB Overrun Error. An AHB overrun occurred.
 */
#define DBGMAILBOX_CSW_AHB_OR_ERR(x)             (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_AHB_OR_ERR_SHIFT)) & DBGMAILBOX_CSW_AHB_OR_ERR_MASK)

#define DBGMAILBOX_CSW_SOFT_RESET_MASK           (0x10U)
#define DBGMAILBOX_CSW_SOFT_RESET_SHIFT          (4U)
/*! SOFT_RESET - Soft Reset */
#define DBGMAILBOX_CSW_SOFT_RESET(x)             (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_SOFT_RESET_SHIFT)) & DBGMAILBOX_CSW_SOFT_RESET_MASK)

#define DBGMAILBOX_CSW_CHIP_RESET_REQ_MASK       (0x20U)
#define DBGMAILBOX_CSW_CHIP_RESET_REQ_SHIFT      (5U)
/*! CHIP_RESET_REQ - Chip Reset Request */
#define DBGMAILBOX_CSW_CHIP_RESET_REQ(x)         (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_CSW_CHIP_RESET_REQ_SHIFT)) & DBGMAILBOX_CSW_CHIP_RESET_REQ_MASK)
/*! @} */

/*! @name REQUEST - Request Value */
/*! @{ */

#define DBGMAILBOX_REQUEST_REQUEST_MASK          (0xFFFFFFFFU)
#define DBGMAILBOX_REQUEST_REQUEST_SHIFT         (0U)
/*! REQUEST - Request Value */
#define DBGMAILBOX_REQUEST_REQUEST(x)            (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_REQUEST_REQUEST_SHIFT)) & DBGMAILBOX_REQUEST_REQUEST_MASK)
/*! @} */

/*! @name RETURN - Return Value */
/*! @{ */

#define DBGMAILBOX_RETURN_RET_MASK               (0xFFFFFFFFU)
#define DBGMAILBOX_RETURN_RET_SHIFT              (0U)
/*! RET - Return Value */
#define DBGMAILBOX_RETURN_RET(x)                 (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_RETURN_RET_SHIFT)) & DBGMAILBOX_RETURN_RET_MASK)
/*! @} */

/*! @name ID - Identification */
/*! @{ */

#define DBGMAILBOX_ID_ID_MASK                    (0xFFFFFFFFU)
#define DBGMAILBOX_ID_ID_SHIFT                   (0U)
/*! ID - Identification Value */
#define DBGMAILBOX_ID_ID(x)                      (((uint32_t)(((uint32_t)(x)) << DBGMAILBOX_ID_ID_SHIFT)) & DBGMAILBOX_ID_ID_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group DBGMAILBOX_Register_Masks */


/*!
 * @}
 */ /* end of group DBGMAILBOX_Peripheral_Access_Layer */


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


#endif  /* DBGMAILBOX_H_ */

