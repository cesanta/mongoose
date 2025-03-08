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
**         CMSIS Peripheral Access Layer for GDMA
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
 * @file GDMA.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for GDMA
 *
 * CMSIS Peripheral Access Layer for GDMA
 */

#if !defined(GDMA_H_)
#define GDMA_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- GDMA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GDMA_Peripheral_Access_Layer GDMA Peripheral Access Layer
 * @{
 */

/** GDMA - Size of Registers Arrays */
#define GDMA_CH_COUNT                             4u

/** GDMA - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[64];
  struct {                                         /* offset: 0x40, array step: 0x40 */
    __IO uint32_t SADR;                              /**< DMA SOURCE ADDRESS REGISTERS, array offset: 0x40, array step: 0x40 */
    __IO uint32_t DADR;                              /**< DMA DESTINATION ADDRESS REGISTERS, array offset: 0x44, array step: 0x40 */
    __IO uint32_t LLI;                               /**< DMA CHANNEL LINKED LIST ITEM REGISTERS, array offset: 0x48, array step: 0x40 */
    __IO uint32_t CTRL;                              /**< DMA CONTROL REGISTERS, array offset: 0x4C, array step: 0x40 */
    __IO uint32_t CONFIG;                            /**< DMA CONFIGURE REGISTERS, array offset: 0x50, array step: 0x40 */
    __IO uint32_t CHL_EN;                            /**< DMA CHANNEL ENABLE REGISTERS, array offset: 0x54, array step: 0x40 */
    __IO uint32_t CHL_STOP;                          /**< DMA CHANNEL STOP REGISTERS, array offset: 0x58, array step: 0x40 */
    __IO uint32_t CHNL_INT;                          /**< DMA INT REGISTERS, array offset: 0x5C, array step: 0x40 */
         uint8_t RESERVED_0[16];
    __IO uint32_t NUM_OF_DESCRIPTOR;                 /**< the number of descriptor, array offset: 0x70, array step: 0x40 */
         uint8_t RESERVED_1[12];
  } CH[GDMA_CH_COUNT];
} GDMA_Type;

/* ----------------------------------------------------------------------------
   -- GDMA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GDMA_Register_Masks GDMA Register Masks
 * @{
 */

/*! @name SADR - DMA SOURCE ADDRESS REGISTERS */
/*! @{ */

#define GDMA_SADR_SRCADDR_MASK                   (0xFFFFFFFFU)
#define GDMA_SADR_SRCADDR_SHIFT                  (0U)
/*! SRCADDR - SOURCE ADDRESS */
#define GDMA_SADR_SRCADDR(x)                     (((uint32_t)(((uint32_t)(x)) << GDMA_SADR_SRCADDR_SHIFT)) & GDMA_SADR_SRCADDR_MASK)
/*! @} */

/* The count of GDMA_SADR */
#define GDMA_SADR_COUNT                          (4U)

/*! @name DADR - DMA DESTINATION ADDRESS REGISTERS */
/*! @{ */

#define GDMA_DADR_DESTADDR_MASK                  (0xFFFFFFFFU)
#define GDMA_DADR_DESTADDR_SHIFT                 (0U)
/*! DESTADDR - DESTINATION ADDRESS */
#define GDMA_DADR_DESTADDR(x)                    (((uint32_t)(((uint32_t)(x)) << GDMA_DADR_DESTADDR_SHIFT)) & GDMA_DADR_DESTADDR_MASK)
/*! @} */

/* The count of GDMA_DADR */
#define GDMA_DADR_COUNT                          (4U)

/*! @name LLI - DMA CHANNEL LINKED LIST ITEM REGISTERS */
/*! @{ */

#define GDMA_LLI_STOP_MASK                       (0x1U)
#define GDMA_LLI_STOP_SHIFT                      (0U)
/*! STOP - STOP the channel after transaction is finished */
#define GDMA_LLI_STOP(x)                         (((uint32_t)(((uint32_t)(x)) << GDMA_LLI_STOP_SHIFT)) & GDMA_LLI_STOP_MASK)

#define GDMA_LLI_DESC_INT_EN_MASK                (0x2U)
#define GDMA_LLI_DESC_INT_EN_SHIFT               (1U)
/*! DESC_INT_EN - the interrupt enable for descriptor finish */
#define GDMA_LLI_DESC_INT_EN(x)                  (((uint32_t)(((uint32_t)(x)) << GDMA_LLI_DESC_INT_EN_SHIFT)) & GDMA_LLI_DESC_INT_EN_MASK)

#define GDMA_LLI_LLI_MASK                        (0xFFFFFFF0U)
#define GDMA_LLI_LLI_SHIFT                       (4U)
/*! LLI - LINKED LIST ITEM ADDRESS */
#define GDMA_LLI_LLI(x)                          (((uint32_t)(((uint32_t)(x)) << GDMA_LLI_LLI_SHIFT)) & GDMA_LLI_LLI_MASK)
/*! @} */

/* The count of GDMA_LLI */
#define GDMA_LLI_COUNT                           (4U)

/*! @name CTRL - DMA CONTROL REGISTERS */
/*! @{ */

#define GDMA_CTRL_LEN_MASK                       (0x1FFFU)
#define GDMA_CTRL_LEN_SHIFT                      (0U)
/*! LEN - Length of the transfer in bytes */
#define GDMA_CTRL_LEN(x)                         (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_LEN_SHIFT)) & GDMA_CTRL_LEN_MASK)

#define GDMA_CTRL_DESTBSIZE_MASK                 (0xE0000U)
#define GDMA_CTRL_DESTBSIZE_SHIFT                (17U)
/*! DESTBSIZE - Destination peripheral/memory transfer burst size */
#define GDMA_CTRL_DESTBSIZE(x)                   (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_DESTBSIZE_SHIFT)) & GDMA_CTRL_DESTBSIZE_MASK)

#define GDMA_CTRL_SRCBSIZE_MASK                  (0x700000U)
#define GDMA_CTRL_SRCBSIZE_SHIFT                 (20U)
/*! SRCBSIZE - Source peripheral/memory transfer burst size */
#define GDMA_CTRL_SRCBSIZE(x)                    (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_SRCBSIZE_SHIFT)) & GDMA_CTRL_SRCBSIZE_MASK)

#define GDMA_CTRL_DESTWIDTH_MASK                 (0x1800000U)
#define GDMA_CTRL_DESTWIDTH_SHIFT                (23U)
/*! DESTWIDTH - Destination peripheral/memory transfer width */
#define GDMA_CTRL_DESTWIDTH(x)                   (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_DESTWIDTH_SHIFT)) & GDMA_CTRL_DESTWIDTH_MASK)

#define GDMA_CTRL_SRCWIDTH_MASK                  (0x6000000U)
#define GDMA_CTRL_SRCWIDTH_SHIFT                 (25U)
/*! SRCWIDTH - Source peripheral/memory transfer width */
#define GDMA_CTRL_SRCWIDTH(x)                    (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_SRCWIDTH_SHIFT)) & GDMA_CTRL_SRCWIDTH_MASK)

#define GDMA_CTRL_DESTADDRINC_MASK               (0x8000000U)
#define GDMA_CTRL_DESTADDRINC_SHIFT              (27U)
/*! DESTADDRINC - destination address increment */
#define GDMA_CTRL_DESTADDRINC(x)                 (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_DESTADDRINC_SHIFT)) & GDMA_CTRL_DESTADDRINC_MASK)

#define GDMA_CTRL_SRCADDRINC_MASK                (0x10000000U)
#define GDMA_CTRL_SRCADDRINC_SHIFT               (28U)
/*! SRCADDRINC - Source address increment */
#define GDMA_CTRL_SRCADDRINC(x)                  (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_SRCADDRINC_SHIFT)) & GDMA_CTRL_SRCADDRINC_MASK)

#define GDMA_CTRL_PROT_MASK                      (0xE0000000U)
#define GDMA_CTRL_PROT_SHIFT                     (29U)
/*! PROT - Protection info for AHB master bus */
#define GDMA_CTRL_PROT(x)                        (((uint32_t)(((uint32_t)(x)) << GDMA_CTRL_PROT_SHIFT)) & GDMA_CTRL_PROT_MASK)
/*! @} */

/* The count of GDMA_CTRL */
#define GDMA_CTRL_COUNT                          (4U)

/*! @name CONFIG - DMA CONFIGURE REGISTERS */
/*! @{ */

#define GDMA_CONFIG_LLE_MASK                     (0x1U)
#define GDMA_CONFIG_LLE_SHIFT                    (0U)
/*! LLE - Link List Enable */
#define GDMA_CONFIG_LLE(x)                       (((uint32_t)(((uint32_t)(x)) << GDMA_CONFIG_LLE_SHIFT)) & GDMA_CONFIG_LLE_MASK)

#define GDMA_CONFIG_HW_EN_SRC_MASK               (0x38000000U)
#define GDMA_CONFIG_HW_EN_SRC_SHIFT              (27U)
/*! HW_EN_SRC - hardware enable trigger source */
#define GDMA_CONFIG_HW_EN_SRC(x)                 (((uint32_t)(((uint32_t)(x)) << GDMA_CONFIG_HW_EN_SRC_SHIFT)) & GDMA_CONFIG_HW_EN_SRC_MASK)

#define GDMA_CONFIG_HW_EN_MASK                   (0x40000000U)
#define GDMA_CONFIG_HW_EN_SHIFT                  (30U)
/*! HW_EN - Hardware enable configuration of current channel */
#define GDMA_CONFIG_HW_EN(x)                     (((uint32_t)(((uint32_t)(x)) << GDMA_CONFIG_HW_EN_SHIFT)) & GDMA_CONFIG_HW_EN_MASK)

#define GDMA_CONFIG_FIFO_EMPTY_MASK              (0x80000000U)
#define GDMA_CONFIG_FIFO_EMPTY_SHIFT             (31U)
/*! FIFO_EMPTY - FIFO status of current channel */
#define GDMA_CONFIG_FIFO_EMPTY(x)                (((uint32_t)(((uint32_t)(x)) << GDMA_CONFIG_FIFO_EMPTY_SHIFT)) & GDMA_CONFIG_FIFO_EMPTY_MASK)
/*! @} */

/* The count of GDMA_CONFIG */
#define GDMA_CONFIG_COUNT                        (4U)

/*! @name CHL_EN - DMA CHANNEL ENABLE REGISTERS */
/*! @{ */

#define GDMA_CHL_EN_CHL_PRIORITY_WEIGHT_MASK     (0xFU)
#define GDMA_CHL_EN_CHL_PRIORITY_WEIGHT_SHIFT    (0U)
/*! CHL_PRIORITY_WEIGHT - DMA Channel Priority Weight */
#define GDMA_CHL_EN_CHL_PRIORITY_WEIGHT(x)       (((uint32_t)(((uint32_t)(x)) << GDMA_CHL_EN_CHL_PRIORITY_WEIGHT_SHIFT)) & GDMA_CHL_EN_CHL_PRIORITY_WEIGHT_MASK)

#define GDMA_CHL_EN_TEST_BUS_SIGNAL_SEL_MASK     (0xF0U)
#define GDMA_CHL_EN_TEST_BUS_SIGNAL_SEL_SHIFT    (4U)
/*! TEST_BUS_SIGNAL_SEL - TEST_BUS_SIGNAL_SEL */
#define GDMA_CHL_EN_TEST_BUS_SIGNAL_SEL(x)       (((uint32_t)(((uint32_t)(x)) << GDMA_CHL_EN_TEST_BUS_SIGNAL_SEL_SHIFT)) & GDMA_CHL_EN_TEST_BUS_SIGNAL_SEL_MASK)

#define GDMA_CHL_EN_CHL_EN_MASK                  (0x80000000U)
#define GDMA_CHL_EN_CHL_EN_SHIFT                 (31U)
/*! CHL_EN - Enable/Disable the channel */
#define GDMA_CHL_EN_CHL_EN(x)                    (((uint32_t)(((uint32_t)(x)) << GDMA_CHL_EN_CHL_EN_SHIFT)) & GDMA_CHL_EN_CHL_EN_MASK)
/*! @} */

/* The count of GDMA_CHL_EN */
#define GDMA_CHL_EN_COUNT                        (4U)

/*! @name CHL_STOP - DMA CHANNEL STOP REGISTERS */
/*! @{ */

#define GDMA_CHL_STOP_CHL_STOP_MASK              (0x80000000U)
#define GDMA_CHL_STOP_CHL_STOP_SHIFT             (31U)
/*! CHL_STOP - Stop the running channel */
#define GDMA_CHL_STOP_CHL_STOP(x)                (((uint32_t)(((uint32_t)(x)) << GDMA_CHL_STOP_CHL_STOP_SHIFT)) & GDMA_CHL_STOP_CHL_STOP_MASK)
/*! @} */

/* The count of GDMA_CHL_STOP */
#define GDMA_CHL_STOP_COUNT                      (4U)

/*! @name CHNL_INT - DMA INT REGISTERS */
/*! @{ */

#define GDMA_CHNL_INT_MASK_BLOCKINT_MASK         (0x1U)
#define GDMA_CHNL_INT_MASK_BLOCKINT_SHIFT        (0U)
/*! MASK_BLOCKINT - DMA channel block transfer interrupt mask bit */
#define GDMA_CHNL_INT_MASK_BLOCKINT(x)           (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_MASK_BLOCKINT_SHIFT)) & GDMA_CHNL_INT_MASK_BLOCKINT_MASK)

#define GDMA_CHNL_INT_STATUS_BLOCKINT_MASK       (0x2U)
#define GDMA_CHNL_INT_STATUS_BLOCKINT_SHIFT      (1U)
/*! STATUS_BLOCKINT - DMA channel block transfer interrupt bit */
#define GDMA_CHNL_INT_STATUS_BLOCKINT(x)         (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_STATUS_BLOCKINT_SHIFT)) & GDMA_CHNL_INT_STATUS_BLOCKINT_MASK)

#define GDMA_CHNL_INT_MASK_TFRINT_MASK           (0x4U)
#define GDMA_CHNL_INT_MASK_TFRINT_SHIFT          (2U)
/*! MASK_TFRINT - transfer interrupt mask bit */
#define GDMA_CHNL_INT_MASK_TFRINT(x)             (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_MASK_TFRINT_SHIFT)) & GDMA_CHNL_INT_MASK_TFRINT_MASK)

#define GDMA_CHNL_INT_STATUS_TFRINT_MASK         (0x8U)
#define GDMA_CHNL_INT_STATUS_TFRINT_SHIFT        (3U)
/*! STATUS_TFRINT - transfer interrupt bit */
#define GDMA_CHNL_INT_STATUS_TFRINT(x)           (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_STATUS_TFRINT_SHIFT)) & GDMA_CHNL_INT_STATUS_TFRINT_MASK)

#define GDMA_CHNL_INT_MASK_BUSERRINT_MASK        (0x10U)
#define GDMA_CHNL_INT_MASK_BUSERRINT_SHIFT       (4U)
/*! MASK_BUSERRINT - DMA channel bus error interrupt mask bit */
#define GDMA_CHNL_INT_MASK_BUSERRINT(x)          (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_MASK_BUSERRINT_SHIFT)) & GDMA_CHNL_INT_MASK_BUSERRINT_MASK)

#define GDMA_CHNL_INT_STATUS_BUSERRINT_MASK      (0x20U)
#define GDMA_CHNL_INT_STATUS_BUSERRINT_SHIFT     (5U)
/*! STATUS_BUSERRINT - DMA channel bus error interrupt bit */
#define GDMA_CHNL_INT_STATUS_BUSERRINT(x)        (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_STATUS_BUSERRINT_SHIFT)) & GDMA_CHNL_INT_STATUS_BUSERRINT_MASK)

#define GDMA_CHNL_INT_MASK_ADDRERRINT_MASK       (0x40U)
#define GDMA_CHNL_INT_MASK_ADDRERRINT_SHIFT      (6U)
/*! MASK_ADDRERRINT - DMA Channel source/destination address alignment error interrupt mask bit */
#define GDMA_CHNL_INT_MASK_ADDRERRINT(x)         (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_MASK_ADDRERRINT_SHIFT)) & GDMA_CHNL_INT_MASK_ADDRERRINT_MASK)

#define GDMA_CHNL_INT_STATUS_ADDRERRINT_MASK     (0x80U)
#define GDMA_CHNL_INT_STATUS_ADDRERRINT_SHIFT    (7U)
/*! STATUS_ADDRERRINT - DMA Channel source/destination address alignment error interrupt bit */
#define GDMA_CHNL_INT_STATUS_ADDRERRINT(x)       (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_STATUS_ADDRERRINT_SHIFT)) & GDMA_CHNL_INT_STATUS_ADDRERRINT_MASK)

#define GDMA_CHNL_INT_STATUS_CHLINT_MASK         (0x100U)
#define GDMA_CHNL_INT_STATUS_CHLINT_SHIFT        (8U)
/*! STATUS_CHLINT - DMA channel interrupt */
#define GDMA_CHNL_INT_STATUS_CHLINT(x)           (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_STATUS_CHLINT_SHIFT)) & GDMA_CHNL_INT_STATUS_CHLINT_MASK)

#define GDMA_CHNL_INT_MASK_DESC_TFRINT_MASK      (0x200U)
#define GDMA_CHNL_INT_MASK_DESC_TFRINT_SHIFT     (9U)
/*! MASK_DESC_TFRINT - descriptor transfer interrupt mask bit */
#define GDMA_CHNL_INT_MASK_DESC_TFRINT(x)        (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_MASK_DESC_TFRINT_SHIFT)) & GDMA_CHNL_INT_MASK_DESC_TFRINT_MASK)

#define GDMA_CHNL_INT_DESC_STATUS_TFRINT_MASK    (0x400U)
#define GDMA_CHNL_INT_DESC_STATUS_TFRINT_SHIFT   (10U)
/*! DESC_STATUS_TFRINT - configurable descriptor transfer interrupt bit */
#define GDMA_CHNL_INT_DESC_STATUS_TFRINT(x)      (((uint32_t)(((uint32_t)(x)) << GDMA_CHNL_INT_DESC_STATUS_TFRINT_SHIFT)) & GDMA_CHNL_INT_DESC_STATUS_TFRINT_MASK)
/*! @} */

/* The count of GDMA_CHNL_INT */
#define GDMA_CHNL_INT_COUNT                      (4U)

/*! @name NUM_OF_DESCRIPTOR - the number of descriptor */
/*! @{ */

#define GDMA_NUM_OF_DESCRIPTOR_DATA_MASK         (0xFFFFFFFFU)
#define GDMA_NUM_OF_DESCRIPTOR_DATA_SHIFT        (0U)
/*! DATA - the number of descriptor */
#define GDMA_NUM_OF_DESCRIPTOR_DATA(x)           (((uint32_t)(((uint32_t)(x)) << GDMA_NUM_OF_DESCRIPTOR_DATA_SHIFT)) & GDMA_NUM_OF_DESCRIPTOR_DATA_MASK)
/*! @} */

/* The count of GDMA_NUM_OF_DESCRIPTOR */
#define GDMA_NUM_OF_DESCRIPTOR_COUNT             (4U)


/*!
 * @}
 */ /* end of group GDMA_Register_Masks */


/*!
 * @}
 */ /* end of group GDMA_Peripheral_Access_Layer */


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


#endif  /* GDMA_H_ */

