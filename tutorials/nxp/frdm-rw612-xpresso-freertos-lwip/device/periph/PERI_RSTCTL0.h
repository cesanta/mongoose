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
**         CMSIS Peripheral Access Layer for RSTCTL0
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
 * @file RSTCTL0.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for RSTCTL0
 *
 * CMSIS Peripheral Access Layer for RSTCTL0
 */

#if !defined(RSTCTL0_H_)
#define RSTCTL0_H_                               /**< Symbol preventing repeated inclusion */

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
   -- RSTCTL0 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RSTCTL0_Peripheral_Access_Layer RSTCTL0 Peripheral Access Layer
 * @{
 */

/** RSTCTL0 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t PRSTCTL0;                          /**< Peripheral reset control 0, offset: 0x10 */
  __IO uint32_t PRSTCTL1;                          /**< Peripheral reset control 1, offset: 0x14 */
  __IO uint32_t PRSTCTL2;                          /**< Peripheral reset control 2, offset: 0x18 */
       uint8_t RESERVED_1[36];
  __IO uint32_t PRSTCTL0_SET;                      /**< Peripheral reset set 0, offset: 0x40 */
  __O  uint32_t PRSTCTL1_SET;                      /**< Peripheral reset set 1, offset: 0x44 */
  __O  uint32_t PRSTCTL2_SET;                      /**< Peripheral reset set 2, offset: 0x48 */
       uint8_t RESERVED_2[36];
  __O  uint32_t PRSTCTL0_CLR;                      /**< Peripheral reset clear 0, offset: 0x70 */
  __O  uint32_t PRSTCTL1_CLR;                      /**< Peripheral reset clear 1, offset: 0x74 */
  __O  uint32_t PRSTCTL2_CLR;                      /**< Peripheral reset clear 2, offset: 0x78 */
} RSTCTL0_Type;

/* ----------------------------------------------------------------------------
   -- RSTCTL0 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RSTCTL0_Register_Masks RSTCTL0 Register Masks
 * @{
 */

/*! @name PRSTCTL0 - Peripheral reset control 0 */
/*! @{ */

#define RSTCTL0_PRSTCTL0_PQ_MASK                 (0x100U)
#define RSTCTL0_PRSTCTL0_PQ_SHIFT                (8U)
/*! PQ - pq reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_PQ(x)                   (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_PQ_SHIFT)) & RSTCTL0_PRSTCTL0_PQ_MASK)

#define RSTCTL0_PRSTCTL0_PKC_MASK                (0x200U)
#define RSTCTL0_PRSTCTL0_PKC_SHIFT               (9U)
/*! PKC - pkc reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_PKC(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_PKC_SHIFT)) & RSTCTL0_PRSTCTL0_PKC_MASK)

#define RSTCTL0_PRSTCTL0_ELS_MASK                (0x400U)
#define RSTCTL0_PRSTCTL0_ELS_SHIFT               (10U)
/*! ELS - els reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_ELS(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_ELS_SHIFT)) & RSTCTL0_PRSTCTL0_ELS_MASK)

#define RSTCTL0_PRSTCTL0_PUF_MASK                (0x800U)
#define RSTCTL0_PRSTCTL0_PUF_SHIFT               (11U)
/*! PUF - puf reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_PUF(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_PUF_SHIFT)) & RSTCTL0_PRSTCTL0_PUF_MASK)

#define RSTCTL0_PRSTCTL0_FLEXSPI0_MASK           (0x10000U)
#define RSTCTL0_PRSTCTL0_FLEXSPI0_SHIFT          (16U)
/*! FLEXSPI0 - flexspi0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_FLEXSPI0(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_FLEXSPI0_SHIFT)) & RSTCTL0_PRSTCTL0_FLEXSPI0_MASK)

#define RSTCTL0_PRSTCTL0_HPU_MASK                (0x100000U)
#define RSTCTL0_PRSTCTL0_HPU_SHIFT               (20U)
/*! HPU - hpu reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_HPU(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_HPU_SHIFT)) & RSTCTL0_PRSTCTL0_HPU_MASK)

#define RSTCTL0_PRSTCTL0_USB_MASK                (0x400000U)
#define RSTCTL0_PRSTCTL0_USB_SHIFT               (22U)
/*! USB - usb reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_USB(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_USB_SHIFT)) & RSTCTL0_PRSTCTL0_USB_MASK)

#define RSTCTL0_PRSTCTL0_SCT_MASK                (0x1000000U)
#define RSTCTL0_PRSTCTL0_SCT_SHIFT               (24U)
/*! SCT - sct reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_SCT(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SCT_SHIFT)) & RSTCTL0_PRSTCTL0_SCT_MASK)

#define RSTCTL0_PRSTCTL0_AON_MEM_MASK            (0x2000000U)
#define RSTCTL0_PRSTCTL0_AON_MEM_SHIFT           (25U)
/*! AON_MEM - aon_mem reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_AON_MEM(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_AON_MEM_SHIFT)) & RSTCTL0_PRSTCTL0_AON_MEM_MASK)

#define RSTCTL0_PRSTCTL0_GDMA_MASK               (0x10000000U)
#define RSTCTL0_PRSTCTL0_GDMA_SHIFT              (28U)
/*! GDMA - gdma reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_GDMA(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_GDMA_SHIFT)) & RSTCTL0_PRSTCTL0_GDMA_MASK)

#define RSTCTL0_PRSTCTL0_DMA0_MASK               (0x20000000U)
#define RSTCTL0_PRSTCTL0_DMA0_SHIFT              (29U)
/*! DMA0 - dma0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_DMA0(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_DMA0_SHIFT)) & RSTCTL0_PRSTCTL0_DMA0_MASK)

#define RSTCTL0_PRSTCTL0_DMA1_MASK               (0x40000000U)
#define RSTCTL0_PRSTCTL0_DMA1_SHIFT              (30U)
/*! DMA1 - dma1 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_DMA1(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_DMA1_SHIFT)) & RSTCTL0_PRSTCTL0_DMA1_MASK)

#define RSTCTL0_PRSTCTL0_SDIO_MASK               (0x80000000U)
#define RSTCTL0_PRSTCTL0_SDIO_SHIFT              (31U)
/*! SDIO - sdio reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL0_SDIO(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SDIO_SHIFT)) & RSTCTL0_PRSTCTL0_SDIO_MASK)
/*! @} */

/*! @name PRSTCTL1 - Peripheral reset control 1 */
/*! @{ */

#define RSTCTL0_PRSTCTL1_ELS_APB_MASK            (0x1U)
#define RSTCTL0_PRSTCTL1_ELS_APB_SHIFT           (0U)
/*! ELS_APB - els_apb reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_ELS_APB(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_ELS_APB_SHIFT)) & RSTCTL0_PRSTCTL1_ELS_APB_MASK)

#define RSTCTL0_PRSTCTL1_ELS_GDET_REF_RST_N_MASK (0x2U)
#define RSTCTL0_PRSTCTL1_ELS_GDET_REF_RST_N_SHIFT (1U)
/*! ELS_GDET_REF_RST_N - els_gdet_ref_rst_n control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_ELS_GDET_REF_RST_N(x)   (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_ELS_GDET_REF_RST_N_SHIFT)) & RSTCTL0_PRSTCTL1_ELS_GDET_REF_RST_N_MASK)

#define RSTCTL0_PRSTCTL1_SDIO_SLV_MASK           (0x4U)
#define RSTCTL0_PRSTCTL1_SDIO_SLV_SHIFT          (2U)
/*! SDIO_SLV - sdio_slv reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_SDIO_SLV(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SDIO_SLV_SHIFT)) & RSTCTL0_PRSTCTL1_SDIO_SLV_MASK)

#define RSTCTL0_PRSTCTL1_GAU_MASK                (0x10000U)
#define RSTCTL0_PRSTCTL1_GAU_SHIFT               (16U)
/*! GAU - gau reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_GAU(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_GAU_SHIFT)) & RSTCTL0_PRSTCTL1_GAU_MASK)

#define RSTCTL0_PRSTCTL1_OTP_MASK                (0x20000U)
#define RSTCTL0_PRSTCTL1_OTP_SHIFT               (17U)
/*! OTP - otp reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_OTP(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_OTP_SHIFT)) & RSTCTL0_PRSTCTL1_OTP_MASK)

#define RSTCTL0_PRSTCTL1_SECURE_GPIO_MASK        (0x1000000U)
#define RSTCTL0_PRSTCTL1_SECURE_GPIO_SHIFT       (24U)
/*! SECURE_GPIO - secure_gpio reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_SECURE_GPIO(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SECURE_GPIO_SHIFT)) & RSTCTL0_PRSTCTL1_SECURE_GPIO_MASK)

#define RSTCTL0_PRSTCTL1_ENET_IPG_MASK           (0x2000000U)
#define RSTCTL0_PRSTCTL1_ENET_IPG_SHIFT          (25U)
/*! ENET_IPG - enet_ipg reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_ENET_IPG(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_ENET_IPG_SHIFT)) & RSTCTL0_PRSTCTL1_ENET_IPG_MASK)

#define RSTCTL0_PRSTCTL1_ENET_IPG_S_MASK         (0x4000000U)
#define RSTCTL0_PRSTCTL1_ENET_IPG_S_SHIFT        (26U)
/*! ENET_IPG_S - enet_ipg_s reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_ENET_IPG_S(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_ENET_IPG_S_SHIFT)) & RSTCTL0_PRSTCTL1_ENET_IPG_S_MASK)

#define RSTCTL0_PRSTCTL1_TRNG_MASK               (0x8000000U)
#define RSTCTL0_PRSTCTL1_TRNG_SHIFT              (27U)
/*! TRNG - trng reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL1_TRNG(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_TRNG_SHIFT)) & RSTCTL0_PRSTCTL1_TRNG_MASK)
/*! @} */

/*! @name PRSTCTL2 - Peripheral reset control 2 */
/*! @{ */

#define RSTCTL0_PRSTCTL2_UTICK_MASK              (0x1U)
#define RSTCTL0_PRSTCTL2_UTICK_SHIFT             (0U)
/*! UTICK - utick reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL2_UTICK(x)                (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_UTICK_SHIFT)) & RSTCTL0_PRSTCTL2_UTICK_MASK)

#define RSTCTL0_PRSTCTL2_WWDT0_MASK              (0x2U)
#define RSTCTL0_PRSTCTL2_WWDT0_SHIFT             (1U)
/*! WWDT0 - wwdt0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL2_WWDT0(x)                (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_WWDT0_SHIFT)) & RSTCTL0_PRSTCTL2_WWDT0_MASK)

#define RSTCTL0_PRSTCTL2_USIM_MASK               (0x4U)
#define RSTCTL0_PRSTCTL2_USIM_SHIFT              (2U)
/*! USIM - usim reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL2_USIM(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_USIM_SHIFT)) & RSTCTL0_PRSTCTL2_USIM_MASK)

#define RSTCTL0_PRSTCTL2_FREEMRT_MASK            (0x4000000U)
#define RSTCTL0_PRSTCTL2_FREEMRT_SHIFT           (26U)
/*! FREEMRT - freemrt reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL2_FREEMRT(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_FREEMRT_SHIFT)) & RSTCTL0_PRSTCTL2_FREEMRT_MASK)

#define RSTCTL0_PRSTCTL2_LCDIC_MASK              (0x8000000U)
#define RSTCTL0_PRSTCTL2_LCDIC_SHIFT             (27U)
/*! LCDIC - lcdic reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL0_PRSTCTL2_LCDIC(x)                (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_LCDIC_SHIFT)) & RSTCTL0_PRSTCTL2_LCDIC_MASK)
/*! @} */

/*! @name PRSTCTL0_SET - Peripheral reset set 0 */
/*! @{ */

#define RSTCTL0_PRSTCTL0_SET_PQ_MASK             (0x100U)
#define RSTCTL0_PRSTCTL0_SET_PQ_SHIFT            (8U)
/*! PQ - pq reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_PQ(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_PQ_SHIFT)) & RSTCTL0_PRSTCTL0_SET_PQ_MASK)

#define RSTCTL0_PRSTCTL0_SET_PKC_MASK            (0x200U)
#define RSTCTL0_PRSTCTL0_SET_PKC_SHIFT           (9U)
/*! PKC - pkc reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_PKC(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_PKC_SHIFT)) & RSTCTL0_PRSTCTL0_SET_PKC_MASK)

#define RSTCTL0_PRSTCTL0_SET_ELS_MASK            (0x400U)
#define RSTCTL0_PRSTCTL0_SET_ELS_SHIFT           (10U)
/*! ELS - els reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_ELS(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_ELS_SHIFT)) & RSTCTL0_PRSTCTL0_SET_ELS_MASK)

#define RSTCTL0_PRSTCTL0_SET_PUF_MASK            (0x800U)
#define RSTCTL0_PRSTCTL0_SET_PUF_SHIFT           (11U)
/*! PUF - puf reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_PUF(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_PUF_SHIFT)) & RSTCTL0_PRSTCTL0_SET_PUF_MASK)

#define RSTCTL0_PRSTCTL0_SET_FLEXSPI0_MASK       (0x10000U)
#define RSTCTL0_PRSTCTL0_SET_FLEXSPI0_SHIFT      (16U)
/*! FLEXSPI0 - flexspi0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_FLEXSPI0(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_FLEXSPI0_SHIFT)) & RSTCTL0_PRSTCTL0_SET_FLEXSPI0_MASK)

#define RSTCTL0_PRSTCTL0_SET_HPU_MASK            (0x100000U)
#define RSTCTL0_PRSTCTL0_SET_HPU_SHIFT           (20U)
/*! HPU - hpu reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_HPU(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_HPU_SHIFT)) & RSTCTL0_PRSTCTL0_SET_HPU_MASK)

#define RSTCTL0_PRSTCTL0_SET_USB_MASK            (0x400000U)
#define RSTCTL0_PRSTCTL0_SET_USB_SHIFT           (22U)
/*! USB - usb reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_USB(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_USB_SHIFT)) & RSTCTL0_PRSTCTL0_SET_USB_MASK)

#define RSTCTL0_PRSTCTL0_SET_SCT_MASK            (0x1000000U)
#define RSTCTL0_PRSTCTL0_SET_SCT_SHIFT           (24U)
/*! SCT - sct reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_SCT(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_SCT_SHIFT)) & RSTCTL0_PRSTCTL0_SET_SCT_MASK)

#define RSTCTL0_PRSTCTL0_SET_AON_MEM_MASK        (0x2000000U)
#define RSTCTL0_PRSTCTL0_SET_AON_MEM_SHIFT       (25U)
/*! AON_MEM - aon_mem reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_AON_MEM(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_AON_MEM_SHIFT)) & RSTCTL0_PRSTCTL0_SET_AON_MEM_MASK)

#define RSTCTL0_PRSTCTL0_SET_GDMA_MASK           (0x10000000U)
#define RSTCTL0_PRSTCTL0_SET_GDMA_SHIFT          (28U)
/*! GDMA - gdma reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_GDMA(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_GDMA_SHIFT)) & RSTCTL0_PRSTCTL0_SET_GDMA_MASK)

#define RSTCTL0_PRSTCTL0_SET_DMA0_MASK           (0x20000000U)
#define RSTCTL0_PRSTCTL0_SET_DMA0_SHIFT          (29U)
/*! DMA0 - dma0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_DMA0(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_DMA0_SHIFT)) & RSTCTL0_PRSTCTL0_SET_DMA0_MASK)

#define RSTCTL0_PRSTCTL0_SET_DMA1_MASK           (0x40000000U)
#define RSTCTL0_PRSTCTL0_SET_DMA1_SHIFT          (30U)
/*! DMA1 - dma1 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_DMA1(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_DMA1_SHIFT)) & RSTCTL0_PRSTCTL0_SET_DMA1_MASK)

#define RSTCTL0_PRSTCTL0_SET_SDIO_MASK           (0x80000000U)
#define RSTCTL0_PRSTCTL0_SET_SDIO_SHIFT          (31U)
/*! SDIO - sdio reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_SET_SDIO(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_SET_SDIO_SHIFT)) & RSTCTL0_PRSTCTL0_SET_SDIO_MASK)
/*! @} */

/*! @name PRSTCTL1_SET - Peripheral reset set 1 */
/*! @{ */

#define RSTCTL0_PRSTCTL1_SET_ELS_APB_MASK        (0x1U)
#define RSTCTL0_PRSTCTL1_SET_ELS_APB_SHIFT       (0U)
/*! ELS_APB - els_apb reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_ELS_APB(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_ELS_APB_SHIFT)) & RSTCTL0_PRSTCTL1_SET_ELS_APB_MASK)

#define RSTCTL0_PRSTCTL1_SET_ELS_GDET_REF_RST_N_MASK (0x2U)
#define RSTCTL0_PRSTCTL1_SET_ELS_GDET_REF_RST_N_SHIFT (1U)
/*! ELS_GDET_REF_RST_N - els_gdet_ref_rst_n reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_ELS_GDET_REF_RST_N(x) (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_ELS_GDET_REF_RST_N_SHIFT)) & RSTCTL0_PRSTCTL1_SET_ELS_GDET_REF_RST_N_MASK)

#define RSTCTL0_PRSTCTL1_SET_SDIO_SLV_MASK       (0x4U)
#define RSTCTL0_PRSTCTL1_SET_SDIO_SLV_SHIFT      (2U)
/*! SDIO_SLV - sdio_slv reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_SDIO_SLV(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_SDIO_SLV_SHIFT)) & RSTCTL0_PRSTCTL1_SET_SDIO_SLV_MASK)

#define RSTCTL0_PRSTCTL1_SET_GAU_MASK            (0x10000U)
#define RSTCTL0_PRSTCTL1_SET_GAU_SHIFT           (16U)
/*! GAU - gau reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_GAU(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_GAU_SHIFT)) & RSTCTL0_PRSTCTL1_SET_GAU_MASK)

#define RSTCTL0_PRSTCTL1_SET_OTP_MASK            (0x20000U)
#define RSTCTL0_PRSTCTL1_SET_OTP_SHIFT           (17U)
/*! OTP - otp reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_OTP(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_OTP_SHIFT)) & RSTCTL0_PRSTCTL1_SET_OTP_MASK)

#define RSTCTL0_PRSTCTL1_SET_SECURE_GPIO_MASK    (0x1000000U)
#define RSTCTL0_PRSTCTL1_SET_SECURE_GPIO_SHIFT   (24U)
/*! SECURE_GPIO - secure_gpio reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_SECURE_GPIO(x)      (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_SECURE_GPIO_SHIFT)) & RSTCTL0_PRSTCTL1_SET_SECURE_GPIO_MASK)

#define RSTCTL0_PRSTCTL1_SET_ENET_IPG_MASK       (0x2000000U)
#define RSTCTL0_PRSTCTL1_SET_ENET_IPG_SHIFT      (25U)
/*! ENET_IPG - enet_ipg reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_ENET_IPG(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_ENET_IPG_SHIFT)) & RSTCTL0_PRSTCTL1_SET_ENET_IPG_MASK)

#define RSTCTL0_PRSTCTL1_SET_ENET_IPG_S_MASK     (0x4000000U)
#define RSTCTL0_PRSTCTL1_SET_ENET_IPG_S_SHIFT    (26U)
/*! ENET_IPG_S - enet_ipg_s reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_ENET_IPG_S(x)       (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_ENET_IPG_S_SHIFT)) & RSTCTL0_PRSTCTL1_SET_ENET_IPG_S_MASK)

#define RSTCTL0_PRSTCTL1_SET_TRNG_MASK           (0x8000000U)
#define RSTCTL0_PRSTCTL1_SET_TRNG_SHIFT          (27U)
/*! TRNG - trng reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_SET_TRNG(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_SET_TRNG_SHIFT)) & RSTCTL0_PRSTCTL1_SET_TRNG_MASK)
/*! @} */

/*! @name PRSTCTL2_SET - Peripheral reset set 2 */
/*! @{ */

#define RSTCTL0_PRSTCTL2_SET_UTICK_MASK          (0x1U)
#define RSTCTL0_PRSTCTL2_SET_UTICK_SHIFT         (0U)
/*! UTICK - utick reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_SET_UTICK(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_SET_UTICK_SHIFT)) & RSTCTL0_PRSTCTL2_SET_UTICK_MASK)

#define RSTCTL0_PRSTCTL2_SET_WWDT0_MASK          (0x2U)
#define RSTCTL0_PRSTCTL2_SET_WWDT0_SHIFT         (1U)
/*! WWDT0 - wwdt0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_SET_WWDT0(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_SET_WWDT0_SHIFT)) & RSTCTL0_PRSTCTL2_SET_WWDT0_MASK)

#define RSTCTL0_PRSTCTL2_SET_USIM_MASK           (0x4U)
#define RSTCTL0_PRSTCTL2_SET_USIM_SHIFT          (2U)
/*! USIM - usim reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_SET_USIM(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_SET_USIM_SHIFT)) & RSTCTL0_PRSTCTL2_SET_USIM_MASK)

#define RSTCTL0_PRSTCTL2_SET_FREEMRT_MASK        (0x4000000U)
#define RSTCTL0_PRSTCTL2_SET_FREEMRT_SHIFT       (26U)
/*! FREEMRT - freemrt reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_SET_FREEMRT(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_SET_FREEMRT_SHIFT)) & RSTCTL0_PRSTCTL2_SET_FREEMRT_MASK)

#define RSTCTL0_PRSTCTL2_SET_LCDIC_MASK          (0x8000000U)
#define RSTCTL0_PRSTCTL2_SET_LCDIC_SHIFT         (27U)
/*! LCDIC - lcdic reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_SET_LCDIC(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_SET_LCDIC_SHIFT)) & RSTCTL0_PRSTCTL2_SET_LCDIC_MASK)
/*! @} */

/*! @name PRSTCTL0_CLR - Peripheral reset clear 0 */
/*! @{ */

#define RSTCTL0_PRSTCTL0_CLR_PQ_MASK             (0x100U)
#define RSTCTL0_PRSTCTL0_CLR_PQ_SHIFT            (8U)
/*! PQ - pq reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_PQ(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_PQ_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_PQ_MASK)

#define RSTCTL0_PRSTCTL0_CLR_PKC_MASK            (0x200U)
#define RSTCTL0_PRSTCTL0_CLR_PKC_SHIFT           (9U)
/*! PKC - pkc reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_PKC(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_PKC_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_PKC_MASK)

#define RSTCTL0_PRSTCTL0_CLR_ELS_MASK            (0x400U)
#define RSTCTL0_PRSTCTL0_CLR_ELS_SHIFT           (10U)
/*! ELS - els reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_ELS(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_ELS_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_ELS_MASK)

#define RSTCTL0_PRSTCTL0_CLR_PUF_MASK            (0x800U)
#define RSTCTL0_PRSTCTL0_CLR_PUF_SHIFT           (11U)
/*! PUF - puf reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_PUF(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_PUF_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_PUF_MASK)

#define RSTCTL0_PRSTCTL0_CLR_FLEXSPI0_MASK       (0x10000U)
#define RSTCTL0_PRSTCTL0_CLR_FLEXSPI0_SHIFT      (16U)
/*! FLEXSPI0 - flexspi0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_FLEXSPI0(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_FLEXSPI0_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_FLEXSPI0_MASK)

#define RSTCTL0_PRSTCTL0_CLR_HPU_MASK            (0x100000U)
#define RSTCTL0_PRSTCTL0_CLR_HPU_SHIFT           (20U)
/*! HPU - hpu reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_HPU(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_HPU_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_HPU_MASK)

#define RSTCTL0_PRSTCTL0_CLR_USB_MASK            (0x400000U)
#define RSTCTL0_PRSTCTL0_CLR_USB_SHIFT           (22U)
/*! USB - usb reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_USB(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_USB_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_USB_MASK)

#define RSTCTL0_PRSTCTL0_CLR_SCT_MASK            (0x1000000U)
#define RSTCTL0_PRSTCTL0_CLR_SCT_SHIFT           (24U)
/*! SCT - sct reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_SCT(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_SCT_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_SCT_MASK)

#define RSTCTL0_PRSTCTL0_CLR_AON_MEM_MASK        (0x2000000U)
#define RSTCTL0_PRSTCTL0_CLR_AON_MEM_SHIFT       (25U)
/*! AON_MEM - aon_mem reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_AON_MEM(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_AON_MEM_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_AON_MEM_MASK)

#define RSTCTL0_PRSTCTL0_CLR_GDMA_MASK           (0x10000000U)
#define RSTCTL0_PRSTCTL0_CLR_GDMA_SHIFT          (28U)
/*! GDMA - gdma reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_GDMA(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_GDMA_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_GDMA_MASK)

#define RSTCTL0_PRSTCTL0_CLR_DMA0_MASK           (0x20000000U)
#define RSTCTL0_PRSTCTL0_CLR_DMA0_SHIFT          (29U)
/*! DMA0 - dma0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_DMA0(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_DMA0_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_DMA0_MASK)

#define RSTCTL0_PRSTCTL0_CLR_DMA1_MASK           (0x40000000U)
#define RSTCTL0_PRSTCTL0_CLR_DMA1_SHIFT          (30U)
/*! DMA1 - dma1 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_DMA1(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_DMA1_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_DMA1_MASK)

#define RSTCTL0_PRSTCTL0_CLR_SDIO_MASK           (0x80000000U)
#define RSTCTL0_PRSTCTL0_CLR_SDIO_SHIFT          (31U)
/*! SDIO - sdio reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL0_CLR_SDIO(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL0_CLR_SDIO_SHIFT)) & RSTCTL0_PRSTCTL0_CLR_SDIO_MASK)
/*! @} */

/*! @name PRSTCTL1_CLR - Peripheral reset clear 1 */
/*! @{ */

#define RSTCTL0_PRSTCTL1_CLR_ELS_APB_MASK        (0x1U)
#define RSTCTL0_PRSTCTL1_CLR_ELS_APB_SHIFT       (0U)
/*! ELS_APB - els_apb reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_ELS_APB(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_ELS_APB_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_ELS_APB_MASK)

#define RSTCTL0_PRSTCTL1_CLR_ELS_GDET_REF_RST_N_MASK (0x2U)
#define RSTCTL0_PRSTCTL1_CLR_ELS_GDET_REF_RST_N_SHIFT (1U)
/*! ELS_GDET_REF_RST_N - els_gdet_ref_rst_n reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_ELS_GDET_REF_RST_N(x) (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_ELS_GDET_REF_RST_N_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_ELS_GDET_REF_RST_N_MASK)

#define RSTCTL0_PRSTCTL1_CLR_SDIO_SLV_MASK       (0x4U)
#define RSTCTL0_PRSTCTL1_CLR_SDIO_SLV_SHIFT      (2U)
/*! SDIO_SLV - sdio_slv reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_SDIO_SLV(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_SDIO_SLV_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_SDIO_SLV_MASK)

#define RSTCTL0_PRSTCTL1_CLR_GAU_MASK            (0x10000U)
#define RSTCTL0_PRSTCTL1_CLR_GAU_SHIFT           (16U)
/*! GAU - gau reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_GAU(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_GAU_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_GAU_MASK)

#define RSTCTL0_PRSTCTL1_CLR_OTP_MASK            (0x20000U)
#define RSTCTL0_PRSTCTL1_CLR_OTP_SHIFT           (17U)
/*! OTP - otp reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_OTP(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_OTP_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_OTP_MASK)

#define RSTCTL0_PRSTCTL1_CLR_SECURE_GPIO_MASK    (0x1000000U)
#define RSTCTL0_PRSTCTL1_CLR_SECURE_GPIO_SHIFT   (24U)
/*! SECURE_GPIO - secure_gpio reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_SECURE_GPIO(x)      (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_SECURE_GPIO_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_SECURE_GPIO_MASK)

#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG_MASK       (0x2000000U)
#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG_SHIFT      (25U)
/*! ENET_IPG - enet_ipg reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_ENET_IPG_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_ENET_IPG_MASK)

#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG_S_MASK     (0x4000000U)
#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG_S_SHIFT    (26U)
/*! ENET_IPG_S - enet_ipg_s reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_ENET_IPG_S(x)       (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_ENET_IPG_S_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_ENET_IPG_S_MASK)

#define RSTCTL0_PRSTCTL1_CLR_TRNG_MASK           (0x8000000U)
#define RSTCTL0_PRSTCTL1_CLR_TRNG_SHIFT          (27U)
/*! TRNG - trng reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL0_PRSTCTL1_CLR_TRNG(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL1_CLR_TRNG_SHIFT)) & RSTCTL0_PRSTCTL1_CLR_TRNG_MASK)
/*! @} */

/*! @name PRSTCTL2_CLR - Peripheral reset clear 2 */
/*! @{ */

#define RSTCTL0_PRSTCTL2_CLR_UTICK_MASK          (0x1U)
#define RSTCTL0_PRSTCTL2_CLR_UTICK_SHIFT         (0U)
/*! UTICK - utick reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_CLR_UTICK(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_CLR_UTICK_SHIFT)) & RSTCTL0_PRSTCTL2_CLR_UTICK_MASK)

#define RSTCTL0_PRSTCTL2_CLR_WWDT0_MASK          (0x2U)
#define RSTCTL0_PRSTCTL2_CLR_WWDT0_SHIFT         (1U)
/*! WWDT0 - wwdt0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_CLR_WWDT0(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_CLR_WWDT0_SHIFT)) & RSTCTL0_PRSTCTL2_CLR_WWDT0_MASK)

#define RSTCTL0_PRSTCTL2_CLR_USIM_MASK           (0x4U)
#define RSTCTL0_PRSTCTL2_CLR_USIM_SHIFT          (2U)
/*! USIM - usim reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_CLR_USIM(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_CLR_USIM_SHIFT)) & RSTCTL0_PRSTCTL2_CLR_USIM_MASK)

#define RSTCTL0_PRSTCTL2_CLR_FREEMRT_MASK        (0x4000000U)
#define RSTCTL0_PRSTCTL2_CLR_FREEMRT_SHIFT       (26U)
/*! FREEMRT - freemrt reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_CLR_FREEMRT(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_CLR_FREEMRT_SHIFT)) & RSTCTL0_PRSTCTL2_CLR_FREEMRT_MASK)

#define RSTCTL0_PRSTCTL2_CLR_LCDIC_MASK          (0x8000000U)
#define RSTCTL0_PRSTCTL2_CLR_LCDIC_SHIFT         (27U)
/*! LCDIC - lcdic reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL0_PRSTCTL2_CLR_LCDIC(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL0_PRSTCTL2_CLR_LCDIC_SHIFT)) & RSTCTL0_PRSTCTL2_CLR_LCDIC_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group RSTCTL0_Register_Masks */


/*!
 * @}
 */ /* end of group RSTCTL0_Peripheral_Access_Layer */


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


#endif  /* RSTCTL0_H_ */

