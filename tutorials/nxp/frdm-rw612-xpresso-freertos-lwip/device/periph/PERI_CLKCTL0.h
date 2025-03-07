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
**         CMSIS Peripheral Access Layer for CLKCTL0
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
 * @file CLKCTL0.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CLKCTL0
 *
 * CMSIS Peripheral Access Layer for CLKCTL0
 */

#if !defined(CLKCTL0_H_)
#define CLKCTL0_H_                               /**< Symbol preventing repeated inclusion */

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
   -- CLKCTL0 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CLKCTL0_Peripheral_Access_Layer CLKCTL0 Peripheral Access Layer
 * @{
 */

/** CLKCTL0 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t PSCCTL0;                           /**< clock control 0, offset: 0x10 */
  __IO uint32_t PSCCTL1;                           /**< clock control 1, offset: 0x14 */
  __IO uint32_t PSCCTL2;                           /**< clock control 2, offset: 0x18 */
       uint8_t RESERVED_1[36];
  __O  uint32_t PSCCTL0_SET;                       /**< Peripheral clock set 0, offset: 0x40 */
  __O  uint32_t PSCCTL1_SET;                       /**< Peripheral clock set 1, offset: 0x44 */
  __O  uint32_t PSCCTL2_SET;                       /**< Peripheral clock set 2, offset: 0x48 */
       uint8_t RESERVED_2[36];
  __O  uint32_t PSCCTL0_CLR;                       /**< Peripheral clock clear 0, offset: 0x70 */
  __O  uint32_t PSCCTL1_CLR;                       /**< Peripheral clock clear 1, offset: 0x74 */
  __O  uint32_t PSCCTL2_CLR;                       /**< Peripheral clock clear 2, offset: 0x78 */
       uint8_t RESERVED_3[236];
  __IO uint32_t SYSOSCBYPASS;                      /**< System oscillator bypass, offset: 0x168 */
       uint8_t RESERVED_4[84];
  __IO uint32_t CLK32KHZCTL0;                      /**< 32k control 0, offset: 0x1C0 */
       uint8_t RESERVED_5[124];
  __IO uint32_t MAINPLLCLKDIV;                     /**< Main PLL clock divider, offset: 0x240 */
       uint8_t RESERVED_6[4];
  __IO uint32_t AUX0PLLCLKDIV;                     /**< AUX0 PLL clock divider, offset: 0x248 */
  __IO uint32_t AUX1PLLCLKDIV;                     /**< AUX1 PLL clock divider, offset: 0x24C */
       uint8_t RESERVED_7[432];
  __IO uint32_t SYSCPUAHBCLKDIV;                   /**< System CPU AHB clock divider, offset: 0x400 */
       uint8_t RESERVED_8[44];
  __IO uint32_t MAINCLKSELA;                       /**< Main clock selection A, offset: 0x430 */
  __IO uint32_t MAINCLKSELB;                       /**< Main clock selection B, offset: 0x434 */
       uint8_t RESERVED_9[488];
  __IO uint32_t FLEXSPIFCLKSEL;                    /**< FlexSPI FCLK selection, offset: 0x620 */
  __IO uint32_t FLEXSPIFCLKDIV;                    /**< FlexSPI FCLK divider, offset: 0x624 */
       uint8_t RESERVED_10[24];
  __IO uint32_t SCTFCLKSEL;                        /**< SCT FCLK selection, offset: 0x640 */
  __IO uint32_t SCTFCLKDIV;                        /**< SCT FCLK divider, offset: 0x644 */
       uint8_t RESERVED_11[184];
  __IO uint32_t UTICKFCLKSEL;                      /**< UTICK FCLK selection, offset: 0x700 */
       uint8_t RESERVED_12[28];
  __IO uint32_t WDT0FCLKSEL;                       /**< WDT clock selection, offset: 0x720 */
       uint8_t RESERVED_13[60];
  __IO uint32_t SYSTICKFCLKSEL;                    /**< System tick FCLK selection, offset: 0x760 */
  __IO uint32_t SYSTICKFCLKDIV;                    /**< System tick FCLK divider, offset: 0x764 */
  __IO uint32_t LCDFCLKDIV;                        /**< Lcd FCLK divider, offset: 0x768 */
  __IO uint32_t GAUFCLKDIV;                        /**< Gau FCLK divider, offset: 0x76C */
  __IO uint32_t USIMFCLKDIV;                       /**< Usim FCLK divider, offset: 0x770 */
  __IO uint32_t USIMFCLKSEL;                       /**< USIM FCLK selection, offset: 0x774 */
  __IO uint32_t LCDFCLKSEL;                        /**< LCD FCLK selection, offset: 0x778 */
  __IO uint32_t GAUFCLKSEL;                        /**< GAU FCLK selection, offset: 0x77C */
  __IO uint32_t PMUFCLKDIV;                        /**< Pmu FCLK divider, offset: 0x780 */
       uint8_t RESERVED_14[12];
  __IO uint32_t BRG_CLK_EN;                        /**< wl/ble/soc bridge clock enable signal, offset: 0x790 */
       uint8_t RESERVED_15[12];
  __IO uint32_t G2BIST_CLK_EN;                     /**< g2bist clock enable signal, offset: 0x7A0 */
  __IO uint32_t MAIN_RAM_CLK_EN;                   /**< main ram clock enable signal, offset: 0x7A4 */
  __IO uint32_t ELS_GDET_CLK_SEL;                  /**< ELS GDET CLK selection, offset: 0x7A8 */
} CLKCTL0_Type;

/* ----------------------------------------------------------------------------
   -- CLKCTL0 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CLKCTL0_Register_Masks CLKCTL0 Register Masks
 * @{
 */

/*! @name PSCCTL0 - clock control 0 */
/*! @{ */

#define CLKCTL0_PSCCTL0_PQ_MASK                  (0x100U)
#define CLKCTL0_PSCCTL0_PQ_SHIFT                 (8U)
/*! PQ - pq clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_PQ(x)                    (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_PQ_SHIFT)) & CLKCTL0_PSCCTL0_PQ_MASK)

#define CLKCTL0_PSCCTL0_PKC_MASK                 (0x200U)
#define CLKCTL0_PSCCTL0_PKC_SHIFT                (9U)
/*! PKC - pkc clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_PKC(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_PKC_SHIFT)) & CLKCTL0_PSCCTL0_PKC_MASK)

#define CLKCTL0_PSCCTL0_ELS_MASK                 (0x400U)
#define CLKCTL0_PSCCTL0_ELS_SHIFT                (10U)
/*! ELS - els clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_ELS(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_ELS_SHIFT)) & CLKCTL0_PSCCTL0_ELS_MASK)

#define CLKCTL0_PSCCTL0_PUF_MASK                 (0x800U)
#define CLKCTL0_PSCCTL0_PUF_SHIFT                (11U)
/*! PUF - puf clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_PUF(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_PUF_SHIFT)) & CLKCTL0_PSCCTL0_PUF_MASK)

#define CLKCTL0_PSCCTL0_FLEXSPI0_MASK            (0x10000U)
#define CLKCTL0_PSCCTL0_FLEXSPI0_SHIFT           (16U)
/*! FLEXSPI0 - flexspi0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_FLEXSPI0(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_FLEXSPI0_SHIFT)) & CLKCTL0_PSCCTL0_FLEXSPI0_MASK)

#define CLKCTL0_PSCCTL0_HPU_MASK                 (0x100000U)
#define CLKCTL0_PSCCTL0_HPU_SHIFT                (20U)
/*! HPU - hpu clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_HPU(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_HPU_SHIFT)) & CLKCTL0_PSCCTL0_HPU_MASK)

#define CLKCTL0_PSCCTL0_USB_MASK                 (0x400000U)
#define CLKCTL0_PSCCTL0_USB_SHIFT                (22U)
/*! USB - usb clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_USB(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_USB_SHIFT)) & CLKCTL0_PSCCTL0_USB_MASK)

#define CLKCTL0_PSCCTL0_SCT_MASK                 (0x1000000U)
#define CLKCTL0_PSCCTL0_SCT_SHIFT                (24U)
/*! SCT - sct clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_SCT(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SCT_SHIFT)) & CLKCTL0_PSCCTL0_SCT_MASK)

#define CLKCTL0_PSCCTL0_AON_MEM_MASK             (0x2000000U)
#define CLKCTL0_PSCCTL0_AON_MEM_SHIFT            (25U)
/*! AON_MEM - aon_mem clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_AON_MEM(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_AON_MEM_SHIFT)) & CLKCTL0_PSCCTL0_AON_MEM_MASK)

#define CLKCTL0_PSCCTL0_GDMA_MASK                (0x10000000U)
#define CLKCTL0_PSCCTL0_GDMA_SHIFT               (28U)
/*! GDMA - gdma clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_GDMA(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_GDMA_SHIFT)) & CLKCTL0_PSCCTL0_GDMA_MASK)

#define CLKCTL0_PSCCTL0_DMA0_MASK                (0x20000000U)
#define CLKCTL0_PSCCTL0_DMA0_SHIFT               (29U)
/*! DMA0 - dma0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_DMA0(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_DMA0_SHIFT)) & CLKCTL0_PSCCTL0_DMA0_MASK)

#define CLKCTL0_PSCCTL0_DMA1_MASK                (0x40000000U)
#define CLKCTL0_PSCCTL0_DMA1_SHIFT               (30U)
/*! DMA1 - dma1 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_DMA1(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_DMA1_SHIFT)) & CLKCTL0_PSCCTL0_DMA1_MASK)

#define CLKCTL0_PSCCTL0_SDIO_MASK                (0x80000000U)
#define CLKCTL0_PSCCTL0_SDIO_SHIFT               (31U)
/*! SDIO - sdio clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL0_SDIO(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SDIO_SHIFT)) & CLKCTL0_PSCCTL0_SDIO_MASK)
/*! @} */

/*! @name PSCCTL1 - clock control 1 */
/*! @{ */

#define CLKCTL0_PSCCTL1_ELS_APB_MASK             (0x1U)
#define CLKCTL0_PSCCTL1_ELS_APB_SHIFT            (0U)
/*! ELS_APB - els_apb clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_ELS_APB(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_ELS_APB_SHIFT)) & CLKCTL0_PSCCTL1_ELS_APB_MASK)

#define CLKCTL0_PSCCTL1_SDIO_SLV_MASK            (0x4U)
#define CLKCTL0_PSCCTL1_SDIO_SLV_SHIFT           (2U)
/*! SDIO_SLV - sdio_slv clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_SDIO_SLV(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SDIO_SLV_SHIFT)) & CLKCTL0_PSCCTL1_SDIO_SLV_MASK)

#define CLKCTL0_PSCCTL1_GAU_MASK                 (0x10000U)
#define CLKCTL0_PSCCTL1_GAU_SHIFT                (16U)
/*! GAU - gau clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_GAU(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_GAU_SHIFT)) & CLKCTL0_PSCCTL1_GAU_MASK)

#define CLKCTL0_PSCCTL1_OTP_MASK                 (0x20000U)
#define CLKCTL0_PSCCTL1_OTP_SHIFT                (17U)
/*! OTP - otp clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_OTP(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_OTP_SHIFT)) & CLKCTL0_PSCCTL1_OTP_MASK)

#define CLKCTL0_PSCCTL1_SECURE_GPIO_MASK         (0x1000000U)
#define CLKCTL0_PSCCTL1_SECURE_GPIO_SHIFT        (24U)
/*! SECURE_GPIO - secure_gpio clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_SECURE_GPIO(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SECURE_GPIO_SHIFT)) & CLKCTL0_PSCCTL1_SECURE_GPIO_MASK)

#define CLKCTL0_PSCCTL1_ENET_IPG_MASK            (0x2000000U)
#define CLKCTL0_PSCCTL1_ENET_IPG_SHIFT           (25U)
/*! ENET_IPG - enet_ipg clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_ENET_IPG(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_ENET_IPG_SHIFT)) & CLKCTL0_PSCCTL1_ENET_IPG_MASK)

#define CLKCTL0_PSCCTL1_ENET_IPG_S_MASK          (0x4000000U)
#define CLKCTL0_PSCCTL1_ENET_IPG_S_SHIFT         (26U)
/*! ENET_IPG_S - enet_ipg_s clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_ENET_IPG_S(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_ENET_IPG_S_SHIFT)) & CLKCTL0_PSCCTL1_ENET_IPG_S_MASK)

#define CLKCTL0_PSCCTL1_TRNG_MASK                (0x8000000U)
#define CLKCTL0_PSCCTL1_TRNG_SHIFT               (27U)
/*! TRNG - trng clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL1_TRNG(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_TRNG_SHIFT)) & CLKCTL0_PSCCTL1_TRNG_MASK)
/*! @} */

/*! @name PSCCTL2 - clock control 2 */
/*! @{ */

#define CLKCTL0_PSCCTL2_UTICK_MASK               (0x1U)
#define CLKCTL0_PSCCTL2_UTICK_SHIFT              (0U)
/*! UTICK - utick clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_UTICK(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_UTICK_SHIFT)) & CLKCTL0_PSCCTL2_UTICK_MASK)

#define CLKCTL0_PSCCTL2_WWDT0_MASK               (0x2U)
#define CLKCTL0_PSCCTL2_WWDT0_SHIFT              (1U)
/*! WWDT0 - wwdt0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_WWDT0(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_WWDT0_SHIFT)) & CLKCTL0_PSCCTL2_WWDT0_MASK)

#define CLKCTL0_PSCCTL2_USIM_MASK                (0x4U)
#define CLKCTL0_PSCCTL2_USIM_SHIFT               (2U)
/*! USIM - usim clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_USIM(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_USIM_SHIFT)) & CLKCTL0_PSCCTL2_USIM_MASK)

#define CLKCTL0_PSCCTL2_ITRC_MASK                (0x8U)
#define CLKCTL0_PSCCTL2_ITRC_SHIFT               (3U)
/*! ITRC - itrc clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_ITRC(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_ITRC_SHIFT)) & CLKCTL0_PSCCTL2_ITRC_MASK)

#define CLKCTL0_PSCCTL2_FREEMRT_MASK             (0x4000000U)
#define CLKCTL0_PSCCTL2_FREEMRT_SHIFT            (26U)
/*! FREEMRT - freemrt clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_FREEMRT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_FREEMRT_SHIFT)) & CLKCTL0_PSCCTL2_FREEMRT_MASK)

#define CLKCTL0_PSCCTL2_LCDIC_MASK               (0x8000000U)
#define CLKCTL0_PSCCTL2_LCDIC_SHIFT              (27U)
/*! LCDIC - lcdic clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL0_PSCCTL2_LCDIC(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_LCDIC_SHIFT)) & CLKCTL0_PSCCTL2_LCDIC_MASK)
/*! @} */

/*! @name PSCCTL0_SET - Peripheral clock set 0 */
/*! @{ */

#define CLKCTL0_PSCCTL0_SET_PQ_MASK              (0x100U)
#define CLKCTL0_PSCCTL0_SET_PQ_SHIFT             (8U)
/*! PQ - pq clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_PQ(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_PQ_SHIFT)) & CLKCTL0_PSCCTL0_SET_PQ_MASK)

#define CLKCTL0_PSCCTL0_SET_PKC_MASK             (0x200U)
#define CLKCTL0_PSCCTL0_SET_PKC_SHIFT            (9U)
/*! PKC - pkc clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_PKC(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_PKC_SHIFT)) & CLKCTL0_PSCCTL0_SET_PKC_MASK)

#define CLKCTL0_PSCCTL0_SET_ELS_MASK             (0x400U)
#define CLKCTL0_PSCCTL0_SET_ELS_SHIFT            (10U)
/*! ELS - els clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_ELS(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_ELS_SHIFT)) & CLKCTL0_PSCCTL0_SET_ELS_MASK)

#define CLKCTL0_PSCCTL0_SET_PUF_MASK             (0x800U)
#define CLKCTL0_PSCCTL0_SET_PUF_SHIFT            (11U)
/*! PUF - puf clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_PUF(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_PUF_SHIFT)) & CLKCTL0_PSCCTL0_SET_PUF_MASK)

#define CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK        (0x10000U)
#define CLKCTL0_PSCCTL0_SET_FLEXSPI0_SHIFT       (16U)
/*! FLEXSPI0 - flexspi0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_FLEXSPI0(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_FLEXSPI0_SHIFT)) & CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK)

#define CLKCTL0_PSCCTL0_SET_HPU_MASK             (0x100000U)
#define CLKCTL0_PSCCTL0_SET_HPU_SHIFT            (20U)
/*! HPU - hpu clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_HPU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_HPU_SHIFT)) & CLKCTL0_PSCCTL0_SET_HPU_MASK)

#define CLKCTL0_PSCCTL0_SET_USB_MASK             (0x400000U)
#define CLKCTL0_PSCCTL0_SET_USB_SHIFT            (22U)
/*! USB - usb clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_USB(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_USB_SHIFT)) & CLKCTL0_PSCCTL0_SET_USB_MASK)

#define CLKCTL0_PSCCTL0_SET_SCT_MASK             (0x1000000U)
#define CLKCTL0_PSCCTL0_SET_SCT_SHIFT            (24U)
/*! SCT - sct clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_SCT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_SCT_SHIFT)) & CLKCTL0_PSCCTL0_SET_SCT_MASK)

#define CLKCTL0_PSCCTL0_SET_AON_MEM_MASK         (0x2000000U)
#define CLKCTL0_PSCCTL0_SET_AON_MEM_SHIFT        (25U)
/*! AON_MEM - aon_mem clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_AON_MEM(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_AON_MEM_SHIFT)) & CLKCTL0_PSCCTL0_SET_AON_MEM_MASK)

#define CLKCTL0_PSCCTL0_SET_GDMA_MASK            (0x10000000U)
#define CLKCTL0_PSCCTL0_SET_GDMA_SHIFT           (28U)
/*! GDMA - gdma clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_GDMA(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_GDMA_SHIFT)) & CLKCTL0_PSCCTL0_SET_GDMA_MASK)

#define CLKCTL0_PSCCTL0_SET_DMA0_MASK            (0x20000000U)
#define CLKCTL0_PSCCTL0_SET_DMA0_SHIFT           (29U)
/*! DMA0 - dma0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_DMA0(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_DMA0_SHIFT)) & CLKCTL0_PSCCTL0_SET_DMA0_MASK)

#define CLKCTL0_PSCCTL0_SET_DMA1_MASK            (0x40000000U)
#define CLKCTL0_PSCCTL0_SET_DMA1_SHIFT           (30U)
/*! DMA1 - dma1 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_DMA1(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_DMA1_SHIFT)) & CLKCTL0_PSCCTL0_SET_DMA1_MASK)

#define CLKCTL0_PSCCTL0_SET_SDIO_MASK            (0x80000000U)
#define CLKCTL0_PSCCTL0_SET_SDIO_SHIFT           (31U)
/*! SDIO - sdio clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_SET_SDIO(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_SET_SDIO_SHIFT)) & CLKCTL0_PSCCTL0_SET_SDIO_MASK)
/*! @} */

/*! @name PSCCTL1_SET - Peripheral clock set 1 */
/*! @{ */

#define CLKCTL0_PSCCTL1_SET_ELS_APB_MASK         (0x1U)
#define CLKCTL0_PSCCTL1_SET_ELS_APB_SHIFT        (0U)
/*! ELS_APB - els_apb clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_ELS_APB(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_ELS_APB_SHIFT)) & CLKCTL0_PSCCTL1_SET_ELS_APB_MASK)

#define CLKCTL0_PSCCTL1_SET_SDIO_SLV_MASK        (0x4U)
#define CLKCTL0_PSCCTL1_SET_SDIO_SLV_SHIFT       (2U)
/*! SDIO_SLV - sdio_slv clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_SDIO_SLV(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_SDIO_SLV_SHIFT)) & CLKCTL0_PSCCTL1_SET_SDIO_SLV_MASK)

#define CLKCTL0_PSCCTL1_SET_GAU_MASK             (0x10000U)
#define CLKCTL0_PSCCTL1_SET_GAU_SHIFT            (16U)
/*! GAU - gau clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_GAU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_GAU_SHIFT)) & CLKCTL0_PSCCTL1_SET_GAU_MASK)

#define CLKCTL0_PSCCTL1_SET_OTP_MASK             (0x20000U)
#define CLKCTL0_PSCCTL1_SET_OTP_SHIFT            (17U)
/*! OTP - otp clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL1_SET_OTP(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_OTP_SHIFT)) & CLKCTL0_PSCCTL1_SET_OTP_MASK)

#define CLKCTL0_PSCCTL1_SET_SECURE_GPIO_MASK     (0x1000000U)
#define CLKCTL0_PSCCTL1_SET_SECURE_GPIO_SHIFT    (24U)
/*! SECURE_GPIO - secure_gpio clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_SECURE_GPIO(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_SECURE_GPIO_SHIFT)) & CLKCTL0_PSCCTL1_SET_SECURE_GPIO_MASK)

#define CLKCTL0_PSCCTL1_SET_ENET_IPG_MASK        (0x2000000U)
#define CLKCTL0_PSCCTL1_SET_ENET_IPG_SHIFT       (25U)
/*! ENET_IPG - enet_ipg clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_ENET_IPG(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_ENET_IPG_SHIFT)) & CLKCTL0_PSCCTL1_SET_ENET_IPG_MASK)

#define CLKCTL0_PSCCTL1_SET_ENET_IPG_S_MASK      (0x4000000U)
#define CLKCTL0_PSCCTL1_SET_ENET_IPG_S_SHIFT     (26U)
/*! ENET_IPG_S - enet_ipg_s clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_ENET_IPG_S(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_ENET_IPG_S_SHIFT)) & CLKCTL0_PSCCTL1_SET_ENET_IPG_S_MASK)

#define CLKCTL0_PSCCTL1_SET_TRNG_MASK            (0x8000000U)
#define CLKCTL0_PSCCTL1_SET_TRNG_SHIFT           (27U)
/*! TRNG - trng clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_SET_TRNG(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_SET_TRNG_SHIFT)) & CLKCTL0_PSCCTL1_SET_TRNG_MASK)
/*! @} */

/*! @name PSCCTL2_SET - Peripheral clock set 2 */
/*! @{ */

#define CLKCTL0_PSCCTL2_SET_UTICK_MASK           (0x1U)
#define CLKCTL0_PSCCTL2_SET_UTICK_SHIFT          (0U)
/*! UTICK - utick clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_UTICK(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_UTICK_SHIFT)) & CLKCTL0_PSCCTL2_SET_UTICK_MASK)

#define CLKCTL0_PSCCTL2_SET_WWDT0_MASK           (0x2U)
#define CLKCTL0_PSCCTL2_SET_WWDT0_SHIFT          (1U)
/*! WWDT0 - wwdt0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_WWDT0(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_WWDT0_SHIFT)) & CLKCTL0_PSCCTL2_SET_WWDT0_MASK)

#define CLKCTL0_PSCCTL2_SET_USIM_MASK            (0x4U)
#define CLKCTL0_PSCCTL2_SET_USIM_SHIFT           (2U)
/*! USIM - usim clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_USIM(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_USIM_SHIFT)) & CLKCTL0_PSCCTL2_SET_USIM_MASK)

#define CLKCTL0_PSCCTL2_SET_ITRC_MASK            (0x8U)
#define CLKCTL0_PSCCTL2_SET_ITRC_SHIFT           (3U)
/*! ITRC - itrc clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_ITRC(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_ITRC_SHIFT)) & CLKCTL0_PSCCTL2_SET_ITRC_MASK)

#define CLKCTL0_PSCCTL2_SET_FREEMRT_MASK         (0x4000000U)
#define CLKCTL0_PSCCTL2_SET_FREEMRT_SHIFT        (26U)
/*! FREEMRT - freemrt clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_FREEMRT(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_FREEMRT_SHIFT)) & CLKCTL0_PSCCTL2_SET_FREEMRT_MASK)

#define CLKCTL0_PSCCTL2_SET_LCDIC_MASK           (0x8000000U)
#define CLKCTL0_PSCCTL2_SET_LCDIC_SHIFT          (27U)
/*! LCDIC - lcdic clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_SET_LCDIC(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_SET_LCDIC_SHIFT)) & CLKCTL0_PSCCTL2_SET_LCDIC_MASK)
/*! @} */

/*! @name PSCCTL0_CLR - Peripheral clock clear 0 */
/*! @{ */

#define CLKCTL0_PSCCTL0_CLR_PQ_MASK              (0x100U)
#define CLKCTL0_PSCCTL0_CLR_PQ_SHIFT             (8U)
/*! PQ - pq clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_PQ(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_PQ_SHIFT)) & CLKCTL0_PSCCTL0_CLR_PQ_MASK)

#define CLKCTL0_PSCCTL0_CLR_PKC_MASK             (0x200U)
#define CLKCTL0_PSCCTL0_CLR_PKC_SHIFT            (9U)
/*! PKC - pkc clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_PKC(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_PKC_SHIFT)) & CLKCTL0_PSCCTL0_CLR_PKC_MASK)

#define CLKCTL0_PSCCTL0_CLR_ELS_MASK             (0x400U)
#define CLKCTL0_PSCCTL0_CLR_ELS_SHIFT            (10U)
/*! ELS - els clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_ELS(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_ELS_SHIFT)) & CLKCTL0_PSCCTL0_CLR_ELS_MASK)

#define CLKCTL0_PSCCTL0_CLR_PUF_MASK             (0x800U)
#define CLKCTL0_PSCCTL0_CLR_PUF_SHIFT            (11U)
/*! PUF - puf clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_PUF(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_PUF_SHIFT)) & CLKCTL0_PSCCTL0_CLR_PUF_MASK)

#define CLKCTL0_PSCCTL0_CLR_FLEXSPI0_MASK        (0x10000U)
#define CLKCTL0_PSCCTL0_CLR_FLEXSPI0_SHIFT       (16U)
/*! FLEXSPI0 - flexspi0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_FLEXSPI0(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_FLEXSPI0_SHIFT)) & CLKCTL0_PSCCTL0_CLR_FLEXSPI0_MASK)

#define CLKCTL0_PSCCTL0_CLR_HPU_MASK             (0x100000U)
#define CLKCTL0_PSCCTL0_CLR_HPU_SHIFT            (20U)
/*! HPU - hpu clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_HPU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_HPU_SHIFT)) & CLKCTL0_PSCCTL0_CLR_HPU_MASK)

#define CLKCTL0_PSCCTL0_CLR_USB_MASK             (0x400000U)
#define CLKCTL0_PSCCTL0_CLR_USB_SHIFT            (22U)
/*! USB - usb clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_USB(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_USB_SHIFT)) & CLKCTL0_PSCCTL0_CLR_USB_MASK)

#define CLKCTL0_PSCCTL0_CLR_SCT_MASK             (0x1000000U)
#define CLKCTL0_PSCCTL0_CLR_SCT_SHIFT            (24U)
/*! SCT - sct clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_SCT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_SCT_SHIFT)) & CLKCTL0_PSCCTL0_CLR_SCT_MASK)

#define CLKCTL0_PSCCTL0_CLR_AON_MEM_MASK         (0x2000000U)
#define CLKCTL0_PSCCTL0_CLR_AON_MEM_SHIFT        (25U)
/*! AON_MEM - aon_mem clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_AON_MEM(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_AON_MEM_SHIFT)) & CLKCTL0_PSCCTL0_CLR_AON_MEM_MASK)

#define CLKCTL0_PSCCTL0_CLR_GDMA_MASK            (0x10000000U)
#define CLKCTL0_PSCCTL0_CLR_GDMA_SHIFT           (28U)
/*! GDMA - gdma clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_GDMA(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_GDMA_SHIFT)) & CLKCTL0_PSCCTL0_CLR_GDMA_MASK)

#define CLKCTL0_PSCCTL0_CLR_DMA0_MASK            (0x20000000U)
#define CLKCTL0_PSCCTL0_CLR_DMA0_SHIFT           (29U)
/*! DMA0 - dma0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_DMA0(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_DMA0_SHIFT)) & CLKCTL0_PSCCTL0_CLR_DMA0_MASK)

#define CLKCTL0_PSCCTL0_CLR_DMA1_MASK            (0x40000000U)
#define CLKCTL0_PSCCTL0_CLR_DMA1_SHIFT           (30U)
/*! DMA1 - dma1 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_DMA1(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_DMA1_SHIFT)) & CLKCTL0_PSCCTL0_CLR_DMA1_MASK)

#define CLKCTL0_PSCCTL0_CLR_SDIO_MASK            (0x80000000U)
#define CLKCTL0_PSCCTL0_CLR_SDIO_SHIFT           (31U)
/*! SDIO - sdio clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL0_CLR_SDIO(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL0_CLR_SDIO_SHIFT)) & CLKCTL0_PSCCTL0_CLR_SDIO_MASK)
/*! @} */

/*! @name PSCCTL1_CLR - Peripheral clock clear 1 */
/*! @{ */

#define CLKCTL0_PSCCTL1_CLR_ELS_APB_MASK         (0x1U)
#define CLKCTL0_PSCCTL1_CLR_ELS_APB_SHIFT        (0U)
/*! ELS_APB - els_apb clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_ELS_APB(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_ELS_APB_SHIFT)) & CLKCTL0_PSCCTL1_CLR_ELS_APB_MASK)

#define CLKCTL0_PSCCTL1_CLR_SDIO_SLV_MASK        (0x4U)
#define CLKCTL0_PSCCTL1_CLR_SDIO_SLV_SHIFT       (2U)
/*! SDIO_SLV - sdio_slv clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_SDIO_SLV(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_SDIO_SLV_SHIFT)) & CLKCTL0_PSCCTL1_CLR_SDIO_SLV_MASK)

#define CLKCTL0_PSCCTL1_CLR_GAU_MASK             (0x10000U)
#define CLKCTL0_PSCCTL1_CLR_GAU_SHIFT            (16U)
/*! GAU - gau clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_GAU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_GAU_SHIFT)) & CLKCTL0_PSCCTL1_CLR_GAU_MASK)

#define CLKCTL0_PSCCTL1_CLR_OTP_MASK             (0x20000U)
#define CLKCTL0_PSCCTL1_CLR_OTP_SHIFT            (17U)
/*! OTP - otp clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_OTP(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_OTP_SHIFT)) & CLKCTL0_PSCCTL1_CLR_OTP_MASK)

#define CLKCTL0_PSCCTL1_CLR_SECURE_GPIO_MASK     (0x1000000U)
#define CLKCTL0_PSCCTL1_CLR_SECURE_GPIO_SHIFT    (24U)
/*! SECURE_GPIO - secure_gpio clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_SECURE_GPIO(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_SECURE_GPIO_SHIFT)) & CLKCTL0_PSCCTL1_CLR_SECURE_GPIO_MASK)

#define CLKCTL0_PSCCTL1_CLR_ENET_IPG_MASK        (0x2000000U)
#define CLKCTL0_PSCCTL1_CLR_ENET_IPG_SHIFT       (25U)
/*! ENET_IPG - enet_ipg clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_ENET_IPG(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_ENET_IPG_SHIFT)) & CLKCTL0_PSCCTL1_CLR_ENET_IPG_MASK)

#define CLKCTL0_PSCCTL1_CLR_ENET_IPG_S_MASK      (0x4000000U)
#define CLKCTL0_PSCCTL1_CLR_ENET_IPG_S_SHIFT     (26U)
/*! ENET_IPG_S - enet_ipg_s clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_ENET_IPG_S(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_ENET_IPG_S_SHIFT)) & CLKCTL0_PSCCTL1_CLR_ENET_IPG_S_MASK)

#define CLKCTL0_PSCCTL1_CLR_TRNG_MASK            (0x8000000U)
#define CLKCTL0_PSCCTL1_CLR_TRNG_SHIFT           (27U)
/*! TRNG - trng clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL0_PSCCTL1_CLR_TRNG(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL1_CLR_TRNG_SHIFT)) & CLKCTL0_PSCCTL1_CLR_TRNG_MASK)
/*! @} */

/*! @name PSCCTL2_CLR - Peripheral clock clear 2 */
/*! @{ */

#define CLKCTL0_PSCCTL2_CLR_UTICK_MASK           (0x1U)
#define CLKCTL0_PSCCTL2_CLR_UTICK_SHIFT          (0U)
/*! UTICK - utick clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_UTICK(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_UTICK_SHIFT)) & CLKCTL0_PSCCTL2_CLR_UTICK_MASK)

#define CLKCTL0_PSCCTL2_CLR_WWDT0_MASK           (0x2U)
#define CLKCTL0_PSCCTL2_CLR_WWDT0_SHIFT          (1U)
/*! WWDT0 - wwdt0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_WWDT0(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_WWDT0_SHIFT)) & CLKCTL0_PSCCTL2_CLR_WWDT0_MASK)

#define CLKCTL0_PSCCTL2_CLR_USIM_MASK            (0x4U)
#define CLKCTL0_PSCCTL2_CLR_USIM_SHIFT           (2U)
/*! USIM - usim clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_USIM(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_USIM_SHIFT)) & CLKCTL0_PSCCTL2_CLR_USIM_MASK)

#define CLKCTL0_PSCCTL2_CLR_ITRC_MASK            (0x8U)
#define CLKCTL0_PSCCTL2_CLR_ITRC_SHIFT           (3U)
/*! ITRC - itrc clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_ITRC(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_ITRC_SHIFT)) & CLKCTL0_PSCCTL2_CLR_ITRC_MASK)

#define CLKCTL0_PSCCTL2_CLR_FREEMRT_MASK         (0x4000000U)
#define CLKCTL0_PSCCTL2_CLR_FREEMRT_SHIFT        (26U)
/*! FREEMRT - freemrt clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_FREEMRT(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_FREEMRT_SHIFT)) & CLKCTL0_PSCCTL2_CLR_FREEMRT_MASK)

#define CLKCTL0_PSCCTL2_CLR_LCDIC_MASK           (0x8000000U)
#define CLKCTL0_PSCCTL2_CLR_LCDIC_SHIFT          (27U)
/*! LCDIC - lcdic clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL0_PSCCTL2_CLR_LCDIC(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PSCCTL2_CLR_LCDIC_SHIFT)) & CLKCTL0_PSCCTL2_CLR_LCDIC_MASK)
/*! @} */

/*! @name SYSOSCBYPASS - System oscillator bypass */
/*! @{ */

#define CLKCTL0_SYSOSCBYPASS_SEL_MASK            (0x7U)
#define CLKCTL0_SYSOSCBYPASS_SEL_SHIFT           (0U)
/*! SEL - External clock source selection
 *  0b000..Output of the external crystal oscillator
 *  0b001..External clock input (CLKIN function from a pin, selected by IOCON)
 *  0b010..Reserved
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..NONE.this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_SYSOSCBYPASS_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSOSCBYPASS_SEL_SHIFT)) & CLKCTL0_SYSOSCBYPASS_SEL_MASK)
/*! @} */

/*! @name CLK32KHZCTL0 - 32k control 0 */
/*! @{ */

#define CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_MASK      (0x1U)
#define CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_SHIFT     (0U)
/*! ENA_32KHZ - 32 kHz Enable
 *  0b0..Disable
 *  0b1..Enable
 */
#define CLKCTL0_CLK32KHZCTL0_ENA_32KHZ(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_SHIFT)) & CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_MASK)
/*! @} */

/*! @name MAINPLLCLKDIV - Main PLL clock divider */
/*! @{ */

#define CLKCTL0_MAINPLLCLKDIV_DIV_MASK           (0xFFU)
#define CLKCTL0_MAINPLLCLKDIV_DIV_SHIFT          (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_MAINPLLCLKDIV_DIV(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINPLLCLKDIV_DIV_SHIFT)) & CLKCTL0_MAINPLLCLKDIV_DIV_MASK)

#define CLKCTL0_MAINPLLCLKDIV_RESET_MASK         (0x20000000U)
#define CLKCTL0_MAINPLLCLKDIV_RESET_SHIFT        (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_MAINPLLCLKDIV_RESET(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINPLLCLKDIV_RESET_SHIFT)) & CLKCTL0_MAINPLLCLKDIV_RESET_MASK)

#define CLKCTL0_MAINPLLCLKDIV_HALT_MASK          (0x40000000U)
#define CLKCTL0_MAINPLLCLKDIV_HALT_SHIFT         (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_MAINPLLCLKDIV_HALT(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINPLLCLKDIV_HALT_SHIFT)) & CLKCTL0_MAINPLLCLKDIV_HALT_MASK)

#define CLKCTL0_MAINPLLCLKDIV_REQFLAG_MASK       (0x80000000U)
#define CLKCTL0_MAINPLLCLKDIV_REQFLAG_SHIFT      (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_MAINPLLCLKDIV_REQFLAG(x)         (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINPLLCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_MAINPLLCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name AUX0PLLCLKDIV - AUX0 PLL clock divider */
/*! @{ */

#define CLKCTL0_AUX0PLLCLKDIV_DIV_MASK           (0xFFU)
#define CLKCTL0_AUX0PLLCLKDIV_DIV_SHIFT          (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_AUX0PLLCLKDIV_DIV(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX0PLLCLKDIV_DIV_SHIFT)) & CLKCTL0_AUX0PLLCLKDIV_DIV_MASK)

#define CLKCTL0_AUX0PLLCLKDIV_RESET_MASK         (0x20000000U)
#define CLKCTL0_AUX0PLLCLKDIV_RESET_SHIFT        (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_AUX0PLLCLKDIV_RESET(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX0PLLCLKDIV_RESET_SHIFT)) & CLKCTL0_AUX0PLLCLKDIV_RESET_MASK)

#define CLKCTL0_AUX0PLLCLKDIV_HALT_MASK          (0x40000000U)
#define CLKCTL0_AUX0PLLCLKDIV_HALT_SHIFT         (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_AUX0PLLCLKDIV_HALT(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX0PLLCLKDIV_HALT_SHIFT)) & CLKCTL0_AUX0PLLCLKDIV_HALT_MASK)

#define CLKCTL0_AUX0PLLCLKDIV_REQFLAG_MASK       (0x80000000U)
#define CLKCTL0_AUX0PLLCLKDIV_REQFLAG_SHIFT      (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_AUX0PLLCLKDIV_REQFLAG(x)         (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX0PLLCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_AUX0PLLCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name AUX1PLLCLKDIV - AUX1 PLL clock divider */
/*! @{ */

#define CLKCTL0_AUX1PLLCLKDIV_DIV_MASK           (0xFFU)
#define CLKCTL0_AUX1PLLCLKDIV_DIV_SHIFT          (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_AUX1PLLCLKDIV_DIV(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX1PLLCLKDIV_DIV_SHIFT)) & CLKCTL0_AUX1PLLCLKDIV_DIV_MASK)

#define CLKCTL0_AUX1PLLCLKDIV_RESET_MASK         (0x20000000U)
#define CLKCTL0_AUX1PLLCLKDIV_RESET_SHIFT        (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_AUX1PLLCLKDIV_RESET(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX1PLLCLKDIV_RESET_SHIFT)) & CLKCTL0_AUX1PLLCLKDIV_RESET_MASK)

#define CLKCTL0_AUX1PLLCLKDIV_HALT_MASK          (0x40000000U)
#define CLKCTL0_AUX1PLLCLKDIV_HALT_SHIFT         (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_AUX1PLLCLKDIV_HALT(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX1PLLCLKDIV_HALT_SHIFT)) & CLKCTL0_AUX1PLLCLKDIV_HALT_MASK)

#define CLKCTL0_AUX1PLLCLKDIV_REQFLAG_MASK       (0x80000000U)
#define CLKCTL0_AUX1PLLCLKDIV_REQFLAG_SHIFT      (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_AUX1PLLCLKDIV_REQFLAG(x)         (((uint32_t)(((uint32_t)(x)) << CLKCTL0_AUX1PLLCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_AUX1PLLCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name SYSCPUAHBCLKDIV - System CPU AHB clock divider */
/*! @{ */

#define CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK         (0xFFU)
#define CLKCTL0_SYSCPUAHBCLKDIV_DIV_SHIFT        (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_SYSCPUAHBCLKDIV_DIV(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSCPUAHBCLKDIV_DIV_SHIFT)) & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK)

#define CLKCTL0_SYSCPUAHBCLKDIV_REQFLAG_MASK     (0x80000000U)
#define CLKCTL0_SYSCPUAHBCLKDIV_REQFLAG_SHIFT    (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_SYSCPUAHBCLKDIV_REQFLAG(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSCPUAHBCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_SYSCPUAHBCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name MAINCLKSELA - Main clock selection A */
/*! @{ */

#define CLKCTL0_MAINCLKSELA_SEL_MASK             (0x3U)
#define CLKCTL0_MAINCLKSELA_SEL_SHIFT            (0U)
/*! SEL - Control Main 1st Stage Control Clock Source
 *  0b00..External clock (clk_in) or REFCLK_SYS
 *  0b01..FFRO Clock (48/60m_irc) divided by 4
 *  0b10..1m_lposc
 *  0b11..FFRO Clock
 */
#define CLKCTL0_MAINCLKSELA_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINCLKSELA_SEL_SHIFT)) & CLKCTL0_MAINCLKSELA_SEL_MASK)
/*! @} */

/*! @name MAINCLKSELB - Main clock selection B */
/*! @{ */

#define CLKCTL0_MAINCLKSELB_SEL_MASK             (0x3U)
#define CLKCTL0_MAINCLKSELB_SEL_SHIFT            (0U)
/*! SEL - Control Main 2nd Stage Control Clock Source
 *  0b00..MAINCLKSELA 1st Stage Clock
 *  0b01..SFRO Clock
 *  0b10..Main PLL Clock (main_pll_clk).
 *  0b11..32 kHz Clock
 */
#define CLKCTL0_MAINCLKSELB_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAINCLKSELB_SEL_SHIFT)) & CLKCTL0_MAINCLKSELB_SEL_MASK)
/*! @} */

/*! @name FLEXSPIFCLKSEL - FlexSPI FCLK selection */
/*! @{ */

#define CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK          (0x7U)
#define CLKCTL0_FLEXSPIFCLKSEL_SEL_SHIFT         (0U)
/*! SEL - FlexSPI Functional Clock Source Selection
 *  0b000..Main Clock
 *  0b001..t3pll_mci_flexspi_clk(365M)
 *  0b010..AUX0 PLL clock (aux0_pll_clk).
 *  0b011..tcpu_mci_flexspi_clk(312M)
 *  0b100..AUX1 PLL clock (aux1_pll_clk).
 *  0b101..tddr_mci_flexspi_clk(320/355/400M)
 *  0b110..t3pll_mci_256m
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_FLEXSPIFCLKSEL_SEL(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_FLEXSPIFCLKSEL_SEL_SHIFT)) & CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK)
/*! @} */

/*! @name FLEXSPIFCLKDIV - FlexSPI FCLK divider */
/*! @{ */

#define CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK          (0xFFU)
#define CLKCTL0_FLEXSPIFCLKDIV_DIV_SHIFT         (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_FLEXSPIFCLKDIV_DIV(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_FLEXSPIFCLKDIV_DIV_SHIFT)) & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK)

#define CLKCTL0_FLEXSPIFCLKDIV_RESET_MASK        (0x20000000U)
#define CLKCTL0_FLEXSPIFCLKDIV_RESET_SHIFT       (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_FLEXSPIFCLKDIV_RESET(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_FLEXSPIFCLKDIV_RESET_SHIFT)) & CLKCTL0_FLEXSPIFCLKDIV_RESET_MASK)

#define CLKCTL0_FLEXSPIFCLKDIV_HALT_MASK         (0x40000000U)
#define CLKCTL0_FLEXSPIFCLKDIV_HALT_SHIFT        (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_FLEXSPIFCLKDIV_HALT(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_FLEXSPIFCLKDIV_HALT_SHIFT)) & CLKCTL0_FLEXSPIFCLKDIV_HALT_MASK)

#define CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_MASK      (0x80000000U)
#define CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_SHIFT     (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_FLEXSPIFCLKDIV_REQFLAG(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_FLEXSPIFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name SCTFCLKSEL - SCT FCLK selection */
/*! @{ */

#define CLKCTL0_SCTFCLKSEL_SEL_MASK              (0x7U)
#define CLKCTL0_SCTFCLKSEL_SEL_SHIFT             (0U)
/*! SEL - SCT Functional Clock Source Selection
 *  0b000..Main Clock
 *  0b001..Main PLL Clock (main_pll_clk).
 *  0b010..AUX0 PLL clock (aux0_pll_clk).
 *  0b011..FFRO Clock (48/60m_irc).
 *  0b100..AUX1 PLL clock (aux1_pll_clk).
 *  0b101..Audio PLL Clock (audio_pll_clk).
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_SCTFCLKSEL_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SCTFCLKSEL_SEL_SHIFT)) & CLKCTL0_SCTFCLKSEL_SEL_MASK)
/*! @} */

/*! @name SCTFCLKDIV - SCT FCLK divider */
/*! @{ */

#define CLKCTL0_SCTFCLKDIV_DIV_MASK              (0xFFU)
#define CLKCTL0_SCTFCLKDIV_DIV_SHIFT             (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_SCTFCLKDIV_DIV(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SCTFCLKDIV_DIV_SHIFT)) & CLKCTL0_SCTFCLKDIV_DIV_MASK)

#define CLKCTL0_SCTFCLKDIV_RESET_MASK            (0x20000000U)
#define CLKCTL0_SCTFCLKDIV_RESET_SHIFT           (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_SCTFCLKDIV_RESET(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SCTFCLKDIV_RESET_SHIFT)) & CLKCTL0_SCTFCLKDIV_RESET_MASK)

#define CLKCTL0_SCTFCLKDIV_HALT_MASK             (0x40000000U)
#define CLKCTL0_SCTFCLKDIV_HALT_SHIFT            (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_SCTFCLKDIV_HALT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SCTFCLKDIV_HALT_SHIFT)) & CLKCTL0_SCTFCLKDIV_HALT_MASK)

#define CLKCTL0_SCTFCLKDIV_REQFLAG_MASK          (0x80000000U)
#define CLKCTL0_SCTFCLKDIV_REQFLAG_SHIFT         (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_SCTFCLKDIV_REQFLAG(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SCTFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_SCTFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name UTICKFCLKSEL - UTICK FCLK selection */
/*! @{ */

#define CLKCTL0_UTICKFCLKSEL_SEL_MASK            (0x7U)
#define CLKCTL0_UTICKFCLKSEL_SEL_SHIFT           (0U)
/*! SEL - UTICK Functional Clock Source Selection
 *  0b000..1m_lposc
 *  0b001..Main Clock
 *  0b010..Reserved
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_UTICKFCLKSEL_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_UTICKFCLKSEL_SEL_SHIFT)) & CLKCTL0_UTICKFCLKSEL_SEL_MASK)
/*! @} */

/*! @name WDT0FCLKSEL - WDT clock selection */
/*! @{ */

#define CLKCTL0_WDT0FCLKSEL_SEL_MASK             (0x7U)
#define CLKCTL0_WDT0FCLKSEL_SEL_SHIFT            (0U)
/*! SEL - WDT0 Functional Clock Source Selection
 *  0b000..1m_lposc
 *  0b001..Main Clock
 *  0b010..Reserved
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_WDT0FCLKSEL_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_WDT0FCLKSEL_SEL_SHIFT)) & CLKCTL0_WDT0FCLKSEL_SEL_MASK)
/*! @} */

/*! @name SYSTICKFCLKSEL - System tick FCLK selection */
/*! @{ */

#define CLKCTL0_SYSTICKFCLKSEL_SEL_MASK          (0x7U)
#define CLKCTL0_SYSTICKFCLKSEL_SEL_SHIFT         (0U)
/*! SEL - SYSTICK Functional Clock Source Selection
 *  0b000..Systick Divider Output Clock
 *  0b001..1m_lposc
 *  0b010..32 kHz Clock
 *  0b011..SFRO Clock (16m_irc).
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_SYSTICKFCLKSEL_SEL(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSTICKFCLKSEL_SEL_SHIFT)) & CLKCTL0_SYSTICKFCLKSEL_SEL_MASK)
/*! @} */

/*! @name SYSTICKFCLKDIV - System tick FCLK divider */
/*! @{ */

#define CLKCTL0_SYSTICKFCLKDIV_DIV_MASK          (0xFFU)
#define CLKCTL0_SYSTICKFCLKDIV_DIV_SHIFT         (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_SYSTICKFCLKDIV_DIV(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSTICKFCLKDIV_DIV_SHIFT)) & CLKCTL0_SYSTICKFCLKDIV_DIV_MASK)

#define CLKCTL0_SYSTICKFCLKDIV_RESET_MASK        (0x20000000U)
#define CLKCTL0_SYSTICKFCLKDIV_RESET_SHIFT       (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_SYSTICKFCLKDIV_RESET(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSTICKFCLKDIV_RESET_SHIFT)) & CLKCTL0_SYSTICKFCLKDIV_RESET_MASK)

#define CLKCTL0_SYSTICKFCLKDIV_HALT_MASK         (0x40000000U)
#define CLKCTL0_SYSTICKFCLKDIV_HALT_SHIFT        (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_SYSTICKFCLKDIV_HALT(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSTICKFCLKDIV_HALT_SHIFT)) & CLKCTL0_SYSTICKFCLKDIV_HALT_MASK)

#define CLKCTL0_SYSTICKFCLKDIV_REQFLAG_MASK      (0x80000000U)
#define CLKCTL0_SYSTICKFCLKDIV_REQFLAG_SHIFT     (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_SYSTICKFCLKDIV_REQFLAG(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_SYSTICKFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_SYSTICKFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name LCDFCLKDIV - Lcd FCLK divider */
/*! @{ */

#define CLKCTL0_LCDFCLKDIV_DIV_MASK              (0xFFU)
#define CLKCTL0_LCDFCLKDIV_DIV_SHIFT             (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_LCDFCLKDIV_DIV(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_LCDFCLKDIV_DIV_SHIFT)) & CLKCTL0_LCDFCLKDIV_DIV_MASK)

#define CLKCTL0_LCDFCLKDIV_RESET_MASK            (0x20000000U)
#define CLKCTL0_LCDFCLKDIV_RESET_SHIFT           (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_LCDFCLKDIV_RESET(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_LCDFCLKDIV_RESET_SHIFT)) & CLKCTL0_LCDFCLKDIV_RESET_MASK)

#define CLKCTL0_LCDFCLKDIV_HALT_MASK             (0x40000000U)
#define CLKCTL0_LCDFCLKDIV_HALT_SHIFT            (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_LCDFCLKDIV_HALT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_LCDFCLKDIV_HALT_SHIFT)) & CLKCTL0_LCDFCLKDIV_HALT_MASK)

#define CLKCTL0_LCDFCLKDIV_REQFLAG_MASK          (0x80000000U)
#define CLKCTL0_LCDFCLKDIV_REQFLAG_SHIFT         (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_LCDFCLKDIV_REQFLAG(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_LCDFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_LCDFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name GAUFCLKDIV - Gau FCLK divider */
/*! @{ */

#define CLKCTL0_GAUFCLKDIV_DIV_MASK              (0xFFU)
#define CLKCTL0_GAUFCLKDIV_DIV_SHIFT             (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_GAUFCLKDIV_DIV(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_GAUFCLKDIV_DIV_SHIFT)) & CLKCTL0_GAUFCLKDIV_DIV_MASK)

#define CLKCTL0_GAUFCLKDIV_RESET_MASK            (0x20000000U)
#define CLKCTL0_GAUFCLKDIV_RESET_SHIFT           (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_GAUFCLKDIV_RESET(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_GAUFCLKDIV_RESET_SHIFT)) & CLKCTL0_GAUFCLKDIV_RESET_MASK)

#define CLKCTL0_GAUFCLKDIV_HALT_MASK             (0x40000000U)
#define CLKCTL0_GAUFCLKDIV_HALT_SHIFT            (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_GAUFCLKDIV_HALT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_GAUFCLKDIV_HALT_SHIFT)) & CLKCTL0_GAUFCLKDIV_HALT_MASK)

#define CLKCTL0_GAUFCLKDIV_REQFLAG_MASK          (0x80000000U)
#define CLKCTL0_GAUFCLKDIV_REQFLAG_SHIFT         (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_GAUFCLKDIV_REQFLAG(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_GAUFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_GAUFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name USIMFCLKDIV - Usim FCLK divider */
/*! @{ */

#define CLKCTL0_USIMFCLKDIV_DIV_MASK             (0xFFU)
#define CLKCTL0_USIMFCLKDIV_DIV_SHIFT            (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_USIMFCLKDIV_DIV(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_USIMFCLKDIV_DIV_SHIFT)) & CLKCTL0_USIMFCLKDIV_DIV_MASK)

#define CLKCTL0_USIMFCLKDIV_RESET_MASK           (0x20000000U)
#define CLKCTL0_USIMFCLKDIV_RESET_SHIFT          (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_USIMFCLKDIV_RESET(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL0_USIMFCLKDIV_RESET_SHIFT)) & CLKCTL0_USIMFCLKDIV_RESET_MASK)

#define CLKCTL0_USIMFCLKDIV_HALT_MASK            (0x40000000U)
#define CLKCTL0_USIMFCLKDIV_HALT_SHIFT           (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_USIMFCLKDIV_HALT(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_USIMFCLKDIV_HALT_SHIFT)) & CLKCTL0_USIMFCLKDIV_HALT_MASK)

#define CLKCTL0_USIMFCLKDIV_REQFLAG_MASK         (0x80000000U)
#define CLKCTL0_USIMFCLKDIV_REQFLAG_SHIFT        (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_USIMFCLKDIV_REQFLAG(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_USIMFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_USIMFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name USIMFCLKSEL - USIM FCLK selection */
/*! @{ */

#define CLKCTL0_USIMFCLKSEL_SEL_MASK             (0x7U)
#define CLKCTL0_USIMFCLKSEL_SEL_SHIFT            (0U)
/*! SEL - USIM Functional Clock Source Selection
 *  0b000..Main Clock
 *  0b001..Audio PLL Clock (audio_pll_clk).
 *  0b010..FFRO clock
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_USIMFCLKSEL_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_USIMFCLKSEL_SEL_SHIFT)) & CLKCTL0_USIMFCLKSEL_SEL_MASK)
/*! @} */

/*! @name LCDFCLKSEL - LCD FCLK selection */
/*! @{ */

#define CLKCTL0_LCDFCLKSEL_SEL_MASK              (0x7U)
#define CLKCTL0_LCDFCLKSEL_SEL_SHIFT             (0U)
/*! SEL - LCD Functional Clock Source Selection
 *  0b000..Main Clock
 *  0b001..t3pll_mci_flexspi_clk
 *  0b010..tcpu_mci_flexspi_clk
 *  0b011..tddr_mci_flexspi_clk
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_LCDFCLKSEL_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_LCDFCLKSEL_SEL_SHIFT)) & CLKCTL0_LCDFCLKSEL_SEL_MASK)
/*! @} */

/*! @name GAUFCLKSEL - GAU FCLK selection */
/*! @{ */

#define CLKCTL0_GAUFCLKSEL_SEL_MASK              (0x3U)
#define CLKCTL0_GAUFCLKSEL_SEL_SHIFT             (0U)
/*! SEL - GAU Functional Clock Source Selection
 *  0b00..Main Clock
 *  0b01..t3pll_mci_256m
 *  0b10..avpll_ch2_CLKOUT--64MHz
 *  0b11..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_GAUFCLKSEL_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_GAUFCLKSEL_SEL_SHIFT)) & CLKCTL0_GAUFCLKSEL_SEL_MASK)
/*! @} */

/*! @name PMUFCLKDIV - Pmu FCLK divider */
/*! @{ */

#define CLKCTL0_PMUFCLKDIV_DIV_MASK              (0xFFU)
#define CLKCTL0_PMUFCLKDIV_DIV_SHIFT             (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL0_PMUFCLKDIV_DIV(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PMUFCLKDIV_DIV_SHIFT)) & CLKCTL0_PMUFCLKDIV_DIV_MASK)

#define CLKCTL0_PMUFCLKDIV_RESET_MASK            (0x20000000U)
#define CLKCTL0_PMUFCLKDIV_RESET_SHIFT           (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL0_PMUFCLKDIV_RESET(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PMUFCLKDIV_RESET_SHIFT)) & CLKCTL0_PMUFCLKDIV_RESET_MASK)

#define CLKCTL0_PMUFCLKDIV_HALT_MASK             (0x40000000U)
#define CLKCTL0_PMUFCLKDIV_HALT_SHIFT            (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL0_PMUFCLKDIV_HALT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PMUFCLKDIV_HALT_SHIFT)) & CLKCTL0_PMUFCLKDIV_HALT_MASK)

#define CLKCTL0_PMUFCLKDIV_REQFLAG_MASK          (0x80000000U)
#define CLKCTL0_PMUFCLKDIV_REQFLAG_SHIFT         (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL0_PMUFCLKDIV_REQFLAG(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL0_PMUFCLKDIV_REQFLAG_SHIFT)) & CLKCTL0_PMUFCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name BRG_CLK_EN - wl/ble/soc bridge clock enable signal */
/*! @{ */

#define CLKCTL0_BRG_CLK_EN_WL_MASK               (0x1U)
#define CLKCTL0_BRG_CLK_EN_WL_SHIFT              (0U)
/*! WL - 1:enable, clock running 0:disable, clock gated */
#define CLKCTL0_BRG_CLK_EN_WL(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL0_BRG_CLK_EN_WL_SHIFT)) & CLKCTL0_BRG_CLK_EN_WL_MASK)

#define CLKCTL0_BRG_CLK_EN_BLE_MASK              (0x2U)
#define CLKCTL0_BRG_CLK_EN_BLE_SHIFT             (1U)
/*! BLE - 1:enable, clock running 0:disable, clock gated */
#define CLKCTL0_BRG_CLK_EN_BLE(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_BRG_CLK_EN_BLE_SHIFT)) & CLKCTL0_BRG_CLK_EN_BLE_MASK)

#define CLKCTL0_BRG_CLK_EN_SOC_MASK              (0x4U)
#define CLKCTL0_BRG_CLK_EN_SOC_SHIFT             (2U)
/*! SOC - 1:enable, clock running 0:disable, clock gated */
#define CLKCTL0_BRG_CLK_EN_SOC(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL0_BRG_CLK_EN_SOC_SHIFT)) & CLKCTL0_BRG_CLK_EN_SOC_MASK)
/*! @} */

/*! @name G2BIST_CLK_EN - g2bist clock enable signal */
/*! @{ */

#define CLKCTL0_G2BIST_CLK_EN_VALUE_MASK         (0x1U)
#define CLKCTL0_G2BIST_CLK_EN_VALUE_SHIFT        (0U)
/*! VALUE - 1:enable, g2bist_clk running 0:disable, g2bist_clk gated */
#define CLKCTL0_G2BIST_CLK_EN_VALUE(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL0_G2BIST_CLK_EN_VALUE_SHIFT)) & CLKCTL0_G2BIST_CLK_EN_VALUE_MASK)
/*! @} */

/*! @name MAIN_RAM_CLK_EN - main ram clock enable signal */
/*! @{ */

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY0_MASK      (0x1U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY0_SHIFT     (0U)
/*! ARRAY0 - 1:enable, main ram array0 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY0(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY0_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY0_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY1_MASK      (0x2U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY1_SHIFT     (1U)
/*! ARRAY1 - 1:enable, main ram array1 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY1(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY1_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY1_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY2_MASK      (0x4U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY2_SHIFT     (2U)
/*! ARRAY2 - 1:enable, main ram array2 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY2(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY2_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY2_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY3_MASK      (0x8U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY3_SHIFT     (3U)
/*! ARRAY3 - 1:enable, main ram array3 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY3(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY3_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY3_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY4_MASK      (0x10U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY4_SHIFT     (4U)
/*! ARRAY4 - 1:enable, main ram array4 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY4(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY4_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY4_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY5_MASK      (0x20U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY5_SHIFT     (5U)
/*! ARRAY5 - 1:enable, main ram array5 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY5(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY5_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY5_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY6_MASK      (0x40U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY6_SHIFT     (6U)
/*! ARRAY6 - 1:enable, main ram array6 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY6(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY6_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY6_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY7_MASK      (0x80U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY7_SHIFT     (7U)
/*! ARRAY7 - 1:enable, main ram array7 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY7(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY7_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY7_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY8_MASK      (0x100U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY8_SHIFT     (8U)
/*! ARRAY8 - 1:enable, main ram array8 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY8(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY8_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY8_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY9_MASK      (0x200U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY9_SHIFT     (9U)
/*! ARRAY9 - 1:enable, main ram array9 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY9(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY9_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY9_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY10_MASK     (0x400U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY10_SHIFT    (10U)
/*! ARRAY10 - 1:enable, main ram array10 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY10(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY10_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY10_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY11_MASK     (0x800U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY11_SHIFT    (11U)
/*! ARRAY11 - 1:enable, main ram array11 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY11(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY11_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY11_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY12_MASK     (0x1000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY12_SHIFT    (12U)
/*! ARRAY12 - 1:enable, main ram array12 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY12(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY12_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY12_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY13_MASK     (0x2000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY13_SHIFT    (13U)
/*! ARRAY13 - 1:enable, main ram array13 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY13(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY13_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY13_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY14_MASK     (0x4000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY14_SHIFT    (14U)
/*! ARRAY14 - 1:enable, main ram array14 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY14(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY14_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY14_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY15_MASK     (0x8000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY15_SHIFT    (15U)
/*! ARRAY15 - 1:enable, main ram array15 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY15(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY15_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY15_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY16_MASK     (0x10000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY16_SHIFT    (16U)
/*! ARRAY16 - 1:enable, main ram array16 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY16(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY16_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY16_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY17_MASK     (0x20000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY17_SHIFT    (17U)
/*! ARRAY17 - 1:enable, main ram array17 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY17(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY17_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY17_MASK)

#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY18_MASK     (0x40000U)
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY18_SHIFT    (18U)
/*! ARRAY18 - 1:enable, main ram array18 clock running 0:disable, main ram array0 clock gated */
#define CLKCTL0_MAIN_RAM_CLK_EN_ARRAY18(x)       (((uint32_t)(((uint32_t)(x)) << CLKCTL0_MAIN_RAM_CLK_EN_ARRAY18_SHIFT)) & CLKCTL0_MAIN_RAM_CLK_EN_ARRAY18_MASK)
/*! @} */

/*! @name ELS_GDET_CLK_SEL - ELS GDET CLK selection */
/*! @{ */

#define CLKCTL0_ELS_GDET_CLK_SEL_SEL_MASK        (0x7U)
#define CLKCTL0_ELS_GDET_CLK_SEL_SEL_SHIFT       (0U)
/*! SEL - ELS GDET Clock Source Selection
 *  0b000..t3pll_mci_256m:256Mhz
 *  0b001..t3pll_mci_256m/2:128Mhz
 *  0b010..t3pll_mci_256m/4:64Mhz
 *  0b011..t3pll_mci_256m/8:32Mhz
 *  0b100..reserved
 *  0b101..reserved
 *  0b110..reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL0_ELS_GDET_CLK_SEL_SEL(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL0_ELS_GDET_CLK_SEL_SEL_SHIFT)) & CLKCTL0_ELS_GDET_CLK_SEL_SEL_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CLKCTL0_Register_Masks */


/*!
 * @}
 */ /* end of group CLKCTL0_Peripheral_Access_Layer */


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


#endif  /* CLKCTL0_H_ */

