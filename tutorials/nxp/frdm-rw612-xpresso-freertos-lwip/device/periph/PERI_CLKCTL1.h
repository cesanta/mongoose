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
**         CMSIS Peripheral Access Layer for CLKCTL1
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
 * @file CLKCTL1.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CLKCTL1
 *
 * CMSIS Peripheral Access Layer for CLKCTL1
 */

#if !defined(CLKCTL1_H_)
#define CLKCTL1_H_                               /**< Symbol preventing repeated inclusion */

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
   -- CLKCTL1 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CLKCTL1_Peripheral_Access_Layer CLKCTL1 Peripheral Access Layer
 * @{
 */

/** CLKCTL1 - Size of Registers Arrays */
#define CLKCTL1_FLEXCOMM_COUNT                    4u
#define CLKCTL1_CT32BITFCLKSEL_COUNT              4u

/** CLKCTL1 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t PSCCTL0;                           /**< Peripheral clock control 0, offset: 0x10 */
  __IO uint32_t PSCCTL1;                           /**< Peripheral clock control 1, offset: 0x14 */
  __IO uint32_t PSCCTL2;                           /**< Peripheral clock control 2, offset: 0x18 */
       uint8_t RESERVED_1[36];
  __O  uint32_t PSCCTL0_SET;                       /**< Peripheral clock set 0, offset: 0x40 */
  __O  uint32_t PSCCTL1_SET;                       /**< Peripheral clock set 1, offset: 0x44 */
  __O  uint32_t PSCCTL2_SET;                       /**< Peripheral clock set 2, offset: 0x48 */
       uint8_t RESERVED_2[36];
  __O  uint32_t PSCCTL0_CLR;                       /**< Peripheral clock clear 0, offset: 0x70 */
  __O  uint32_t PSCCTL1_CLR;                       /**< Peripheral clock clear 1, offset: 0x74 */
  __O  uint32_t PSCCTL2_CLR;                       /**< Peripheral clock clear 2, offset: 0x78 */
       uint8_t RESERVED_3[452];
  __IO uint32_t AUDIOPLLCLKDIV;                    /**< Audio PLL0 clock divider, offset: 0x240 */
       uint8_t RESERVED_4[572];
  __IO uint32_t OSEVENTFCLKSEL;                    /**< OS EVENT clock selection, offset: 0x480 */
       uint8_t RESERVED_5[124];
  struct {                                         /* offset: 0x500, array step: 0x20 */
    __IO uint32_t FRGCLKSEL;                         /**< FRG clock selection 0..FRG clock selection 3, array offset: 0x500, array step: 0x20 */
    __IO uint32_t FRGCTL;                            /**< FRG clock controller 0..FRG clock controller 3, array offset: 0x504, array step: 0x20 */
    __IO uint32_t FCFCLKSEL;                         /**< Flexcomm Interface clock selection 0..Flexcomm Interface clock selection 3, array offset: 0x508, array step: 0x20 */
         uint8_t RESERVED_0[20];
  } FLEXCOMM[CLKCTL1_FLEXCOMM_COUNT];
       uint8_t RESERVED_6[320];
  __IO uint32_t FRG14CLKSEL;                       /**< FRG clock selection 14, offset: 0x6C0 */
  __IO uint32_t FRG14CTL;                          /**< FRG clock controller 14, offset: 0x6C4 */
  __IO uint32_t FC14FCLKSEL;                       /**< Flexcomm Interface clock selection 14, offset: 0x6C8 */
       uint8_t RESERVED_7[48];
  __IO uint32_t FRGPLLCLKDIV;                      /**< FRG PLL clock divider, offset: 0x6FC */
  __IO uint32_t DMIC0FCLKSEL;                      /**< DMIC0 clock selection, offset: 0x700 */
  __IO uint32_t DMIC0CLKDIV;                       /**< DMIC clock divider, offset: 0x704 */
       uint8_t RESERVED_8[24];
  __IO uint32_t CT32BITFCLKSEL[CLKCTL1_CT32BITFCLKSEL_COUNT]; /**< Ct32bit timer 0 clock selection..Ct32bit timer 3 clock selection, array offset: 0x720, array step: 0x4 */
       uint8_t RESERVED_9[16];
  __IO uint32_t AUDIOMCLKSEL;                      /**< Audio MCLK selection, offset: 0x740 */
  __IO uint32_t AUDIOMCLKDIV;                      /**< Audio MCLK divider, offset: 0x744 */
       uint8_t RESERVED_10[24];
  __IO uint32_t CLKOUTSEL0;                        /**< Clock out selection 0, offset: 0x760 */
  __IO uint32_t CLKOUTSEL1;                        /**< Clock out selection 1, offset: 0x764 */
  __IO uint32_t CLKOUTDIV;                         /**< Clock out divider, offset: 0x768 */
  __IO uint32_t CLKOUTSEL2;                        /**< Clock out selection 2, offset: 0x76C */
} CLKCTL1_Type;

/* ----------------------------------------------------------------------------
   -- CLKCTL1 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CLKCTL1_Register_Masks CLKCTL1 Register Masks
 * @{
 */

/*! @name PSCCTL0 - Peripheral clock control 0 */
/*! @{ */

#define CLKCTL1_PSCCTL0_FC0_MASK                 (0x100U)
#define CLKCTL1_PSCCTL0_FC0_SHIFT                (8U)
/*! FC0 - fc0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_FC0(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_FC0_SHIFT)) & CLKCTL1_PSCCTL0_FC0_MASK)

#define CLKCTL1_PSCCTL0_FC1_MASK                 (0x200U)
#define CLKCTL1_PSCCTL0_FC1_SHIFT                (9U)
/*! FC1 - fc1 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_FC1(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_FC1_SHIFT)) & CLKCTL1_PSCCTL0_FC1_MASK)

#define CLKCTL1_PSCCTL0_FC2_MASK                 (0x400U)
#define CLKCTL1_PSCCTL0_FC2_SHIFT                (10U)
/*! FC2 - fc2 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_FC2(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_FC2_SHIFT)) & CLKCTL1_PSCCTL0_FC2_MASK)

#define CLKCTL1_PSCCTL0_FC3_MASK                 (0x800U)
#define CLKCTL1_PSCCTL0_FC3_SHIFT                (11U)
/*! FC3 - fc3 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_FC3(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_FC3_SHIFT)) & CLKCTL1_PSCCTL0_FC3_MASK)

#define CLKCTL1_PSCCTL0_FC14_MASK                (0x400000U)
#define CLKCTL1_PSCCTL0_FC14_SHIFT               (22U)
/*! FC14 - fc14 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_FC14(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_FC14_SHIFT)) & CLKCTL1_PSCCTL0_FC14_MASK)

#define CLKCTL1_PSCCTL0_DMIC0_MASK               (0x1000000U)
#define CLKCTL1_PSCCTL0_DMIC0_SHIFT              (24U)
/*! DMIC0 - dmic0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_DMIC0(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_DMIC0_SHIFT)) & CLKCTL1_PSCCTL0_DMIC0_MASK)

#define CLKCTL1_PSCCTL0_OSEVENTTIMER_MASK        (0x8000000U)
#define CLKCTL1_PSCCTL0_OSEVENTTIMER_SHIFT       (27U)
/*! OSEVENTTIMER - oseventtimer clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL0_OSEVENTTIMER(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_OSEVENTTIMER_SHIFT)) & CLKCTL1_PSCCTL0_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PSCCTL1 - Peripheral clock control 1 */
/*! @{ */

#define CLKCTL1_PSCCTL1_HSGPIO0_MASK             (0x1U)
#define CLKCTL1_PSCCTL1_HSGPIO0_SHIFT            (0U)
/*! HSGPIO0 - hsgpio0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL1_HSGPIO0(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_HSGPIO0_SHIFT)) & CLKCTL1_PSCCTL1_HSGPIO0_MASK)

#define CLKCTL1_PSCCTL1_HSGPIO1_MASK             (0x2U)
#define CLKCTL1_PSCCTL1_HSGPIO1_SHIFT            (1U)
/*! HSGPIO1 - hsgpio1 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL1_HSGPIO1(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_HSGPIO1_SHIFT)) & CLKCTL1_PSCCTL1_HSGPIO1_MASK)

#define CLKCTL1_PSCCTL1_CRC_MASK                 (0x10000U)
#define CLKCTL1_PSCCTL1_CRC_SHIFT                (16U)
/*! CRC - crc clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL1_CRC(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_CRC_SHIFT)) & CLKCTL1_PSCCTL1_CRC_MASK)

#define CLKCTL1_PSCCTL1_FREQME_MASK              (0x80000000U)
#define CLKCTL1_PSCCTL1_FREQME_SHIFT             (31U)
/*! FREQME - freqme clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL1_FREQME(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_FREQME_SHIFT)) & CLKCTL1_PSCCTL1_FREQME_MASK)
/*! @} */

/*! @name PSCCTL2 - Peripheral clock control 2 */
/*! @{ */

#define CLKCTL1_PSCCTL2_CT32B0_MASK              (0x1U)
#define CLKCTL1_PSCCTL2_CT32B0_SHIFT             (0U)
/*! CT32B0 - ct32b0 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_CT32B0(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CT32B0_SHIFT)) & CLKCTL1_PSCCTL2_CT32B0_MASK)

#define CLKCTL1_PSCCTL2_CT32B1_MASK              (0x2U)
#define CLKCTL1_PSCCTL2_CT32B1_SHIFT             (1U)
/*! CT32B1 - ct32b1 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_CT32B1(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CT32B1_SHIFT)) & CLKCTL1_PSCCTL2_CT32B1_MASK)

#define CLKCTL1_PSCCTL2_CT32B2_MASK              (0x4U)
#define CLKCTL1_PSCCTL2_CT32B2_SHIFT             (2U)
/*! CT32B2 - ct32b2 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_CT32B2(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CT32B2_SHIFT)) & CLKCTL1_PSCCTL2_CT32B2_MASK)

#define CLKCTL1_PSCCTL2_CT32B3_MASK              (0x8U)
#define CLKCTL1_PSCCTL2_CT32B3_SHIFT             (3U)
/*! CT32B3 - ct32b3 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_CT32B3(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CT32B3_SHIFT)) & CLKCTL1_PSCCTL2_CT32B3_MASK)

#define CLKCTL1_PSCCTL2_CT32B4_MASK              (0x10U)
#define CLKCTL1_PSCCTL2_CT32B4_SHIFT             (4U)
/*! CT32B4 - ct32b4 clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_CT32B4(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CT32B4_SHIFT)) & CLKCTL1_PSCCTL2_CT32B4_MASK)

#define CLKCTL1_PSCCTL2_PMU_MASK                 (0x40U)
#define CLKCTL1_PSCCTL2_PMU_SHIFT                (6U)
/*! PMU - pmu clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_PMU(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_PMU_SHIFT)) & CLKCTL1_PSCCTL2_PMU_MASK)

#define CLKCTL1_PSCCTL2_RTC_LITE_MASK            (0x80U)
#define CLKCTL1_PSCCTL2_RTC_LITE_SHIFT           (7U)
/*! RTC_LITE - rtc_lite clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_RTC_LITE(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_RTC_LITE_SHIFT)) & CLKCTL1_PSCCTL2_RTC_LITE_MASK)

#define CLKCTL1_PSCCTL2_MRT_MASK                 (0x100U)
#define CLKCTL1_PSCCTL2_MRT_SHIFT                (8U)
/*! MRT - mrt clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_MRT(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_MRT_SHIFT)) & CLKCTL1_PSCCTL2_MRT_MASK)

#define CLKCTL1_PSCCTL2_GPIO_INT_MASK            (0x40000000U)
#define CLKCTL1_PSCCTL2_GPIO_INT_SHIFT           (30U)
/*! GPIO_INT - gpio_int clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_GPIO_INT(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_GPIO_INT_SHIFT)) & CLKCTL1_PSCCTL2_GPIO_INT_MASK)

#define CLKCTL1_PSCCTL2_PMUX_MASK                (0x80000000U)
#define CLKCTL1_PSCCTL2_PMUX_SHIFT               (31U)
/*! PMUX - pmux clock control
 *  0b0..Disable clock
 *  0b1..Enable clock
 */
#define CLKCTL1_PSCCTL2_PMUX(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_PMUX_SHIFT)) & CLKCTL1_PSCCTL2_PMUX_MASK)
/*! @} */

/*! @name PSCCTL0_SET - Peripheral clock set 0 */
/*! @{ */

#define CLKCTL1_PSCCTL0_SET_FC0_MASK             (0x100U)
#define CLKCTL1_PSCCTL0_SET_FC0_SHIFT            (8U)
/*! FC0 - fc0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_FC0(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_FC0_SHIFT)) & CLKCTL1_PSCCTL0_SET_FC0_MASK)

#define CLKCTL1_PSCCTL0_SET_FC1_MASK             (0x200U)
#define CLKCTL1_PSCCTL0_SET_FC1_SHIFT            (9U)
/*! FC1 - fc1 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_FC1(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_FC1_SHIFT)) & CLKCTL1_PSCCTL0_SET_FC1_MASK)

#define CLKCTL1_PSCCTL0_SET_FC2_MASK             (0x400U)
#define CLKCTL1_PSCCTL0_SET_FC2_SHIFT            (10U)
/*! FC2 - fc2 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_FC2(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_FC2_SHIFT)) & CLKCTL1_PSCCTL0_SET_FC2_MASK)

#define CLKCTL1_PSCCTL0_SET_FC3_MASK             (0x800U)
#define CLKCTL1_PSCCTL0_SET_FC3_SHIFT            (11U)
/*! FC3 - fc3 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_FC3(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_FC3_SHIFT)) & CLKCTL1_PSCCTL0_SET_FC3_MASK)

#define CLKCTL1_PSCCTL0_SET_FC14_MASK            (0x400000U)
#define CLKCTL1_PSCCTL0_SET_FC14_SHIFT           (22U)
/*! FC14 - fc14 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_FC14(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_FC14_SHIFT)) & CLKCTL1_PSCCTL0_SET_FC14_MASK)

#define CLKCTL1_PSCCTL0_SET_DMIC0_MASK           (0x1000000U)
#define CLKCTL1_PSCCTL0_SET_DMIC0_SHIFT          (24U)
/*! DMIC0 - dmic0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_DMIC0(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_DMIC0_SHIFT)) & CLKCTL1_PSCCTL0_SET_DMIC0_MASK)

#define CLKCTL1_PSCCTL0_SET_OSEVENTTIMER_MASK    (0x8000000U)
#define CLKCTL1_PSCCTL0_SET_OSEVENTTIMER_SHIFT   (27U)
/*! OSEVENTTIMER - oseventtimer clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_SET_OSEVENTTIMER(x)      (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_SET_OSEVENTTIMER_SHIFT)) & CLKCTL1_PSCCTL0_SET_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PSCCTL1_SET - Peripheral clock set 1 */
/*! @{ */

#define CLKCTL1_PSCCTL1_SET_HSGPIO0_MASK         (0x1U)
#define CLKCTL1_PSCCTL1_SET_HSGPIO0_SHIFT        (0U)
/*! HSGPIO0 - hsgpio0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_SET_HSGPIO0(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_SET_HSGPIO0_SHIFT)) & CLKCTL1_PSCCTL1_SET_HSGPIO0_MASK)

#define CLKCTL1_PSCCTL1_SET_HSGPIO1_MASK         (0x2U)
#define CLKCTL1_PSCCTL1_SET_HSGPIO1_SHIFT        (1U)
/*! HSGPIO1 - hsgpio1 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_SET_HSGPIO1(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_SET_HSGPIO1_SHIFT)) & CLKCTL1_PSCCTL1_SET_HSGPIO1_MASK)

#define CLKCTL1_PSCCTL1_SET_CRC_MASK             (0x10000U)
#define CLKCTL1_PSCCTL1_SET_CRC_SHIFT            (16U)
/*! CRC - crc clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_SET_CRC(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_SET_CRC_SHIFT)) & CLKCTL1_PSCCTL1_SET_CRC_MASK)

#define CLKCTL1_PSCCTL1_SET_FREQME_MASK          (0x80000000U)
#define CLKCTL1_PSCCTL1_SET_FREQME_SHIFT         (31U)
/*! FREQME - freqme clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_SET_FREQME(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_SET_FREQME_SHIFT)) & CLKCTL1_PSCCTL1_SET_FREQME_MASK)
/*! @} */

/*! @name PSCCTL2_SET - Peripheral clock set 2 */
/*! @{ */

#define CLKCTL1_PSCCTL2_SET_CT32B0_MASK          (0x1U)
#define CLKCTL1_PSCCTL2_SET_CT32B0_SHIFT         (0U)
/*! CT32B0 - ct32b0 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_CT32B0(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_CT32B0_SHIFT)) & CLKCTL1_PSCCTL2_SET_CT32B0_MASK)

#define CLKCTL1_PSCCTL2_SET_CT32B1_MASK          (0x2U)
#define CLKCTL1_PSCCTL2_SET_CT32B1_SHIFT         (1U)
/*! CT32B1 - ct32b1 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_CT32B1(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_CT32B1_SHIFT)) & CLKCTL1_PSCCTL2_SET_CT32B1_MASK)

#define CLKCTL1_PSCCTL2_SET_CT32B2_MASK          (0x4U)
#define CLKCTL1_PSCCTL2_SET_CT32B2_SHIFT         (2U)
/*! CT32B2 - ct32b2 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_CT32B2(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_CT32B2_SHIFT)) & CLKCTL1_PSCCTL2_SET_CT32B2_MASK)

#define CLKCTL1_PSCCTL2_SET_CT32B3_MASK          (0x8U)
#define CLKCTL1_PSCCTL2_SET_CT32B3_SHIFT         (3U)
/*! CT32B3 - ct32b3 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_CT32B3(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_CT32B3_SHIFT)) & CLKCTL1_PSCCTL2_SET_CT32B3_MASK)

#define CLKCTL1_PSCCTL2_SET_CT32B4_MASK          (0x10U)
#define CLKCTL1_PSCCTL2_SET_CT32B4_SHIFT         (4U)
/*! CT32B4 - ct32b4 clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_CT32B4(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_CT32B4_SHIFT)) & CLKCTL1_PSCCTL2_SET_CT32B4_MASK)

#define CLKCTL1_PSCCTL2_SET_PMU_MASK             (0x40U)
#define CLKCTL1_PSCCTL2_SET_PMU_SHIFT            (6U)
/*! PMU - pmu clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_PMU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_PMU_SHIFT)) & CLKCTL1_PSCCTL2_SET_PMU_MASK)

#define CLKCTL1_PSCCTL2_SET_RTC_LITE_MASK        (0x80U)
#define CLKCTL1_PSCCTL2_SET_RTC_LITE_SHIFT       (7U)
/*! RTC_LITE - rtc_lite clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_RTC_LITE(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_RTC_LITE_SHIFT)) & CLKCTL1_PSCCTL2_SET_RTC_LITE_MASK)

#define CLKCTL1_PSCCTL2_SET_MRT_MASK             (0x100U)
#define CLKCTL1_PSCCTL2_SET_MRT_SHIFT            (8U)
/*! MRT - mrt clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_MRT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_MRT_SHIFT)) & CLKCTL1_PSCCTL2_SET_MRT_MASK)

#define CLKCTL1_PSCCTL2_SET_GPIO_INT_MASK        (0x40000000U)
#define CLKCTL1_PSCCTL2_SET_GPIO_INT_SHIFT       (30U)
/*! GPIO_INT - gpio_int clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_GPIO_INT(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_GPIO_INT_SHIFT)) & CLKCTL1_PSCCTL2_SET_GPIO_INT_MASK)

#define CLKCTL1_PSCCTL2_SET_PMUX_MASK            (0x80000000U)
#define CLKCTL1_PSCCTL2_SET_PMUX_SHIFT           (31U)
/*! PMUX - pmux clock set
 *  0b0..No effect
 *  0b1..Sets the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_SET_PMUX(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_SET_PMUX_SHIFT)) & CLKCTL1_PSCCTL2_SET_PMUX_MASK)
/*! @} */

/*! @name PSCCTL0_CLR - Peripheral clock clear 0 */
/*! @{ */

#define CLKCTL1_PSCCTL0_CLR_FC0_MASK             (0x100U)
#define CLKCTL1_PSCCTL0_CLR_FC0_SHIFT            (8U)
/*! FC0 - fc0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_FC0(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_FC0_SHIFT)) & CLKCTL1_PSCCTL0_CLR_FC0_MASK)

#define CLKCTL1_PSCCTL0_CLR_FC1_MASK             (0x200U)
#define CLKCTL1_PSCCTL0_CLR_FC1_SHIFT            (9U)
/*! FC1 - fc1 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_FC1(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_FC1_SHIFT)) & CLKCTL1_PSCCTL0_CLR_FC1_MASK)

#define CLKCTL1_PSCCTL0_CLR_FC2_MASK             (0x400U)
#define CLKCTL1_PSCCTL0_CLR_FC2_SHIFT            (10U)
/*! FC2 - fc2 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_FC2(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_FC2_SHIFT)) & CLKCTL1_PSCCTL0_CLR_FC2_MASK)

#define CLKCTL1_PSCCTL0_CLR_FC3_MASK             (0x800U)
#define CLKCTL1_PSCCTL0_CLR_FC3_SHIFT            (11U)
/*! FC3 - fc3 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_FC3(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_FC3_SHIFT)) & CLKCTL1_PSCCTL0_CLR_FC3_MASK)

#define CLKCTL1_PSCCTL0_CLR_FC14_MASK            (0x400000U)
#define CLKCTL1_PSCCTL0_CLR_FC14_SHIFT           (22U)
/*! FC14 - fc14 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_FC14(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_FC14_SHIFT)) & CLKCTL1_PSCCTL0_CLR_FC14_MASK)

#define CLKCTL1_PSCCTL0_CLR_DMIC0_MASK           (0x1000000U)
#define CLKCTL1_PSCCTL0_CLR_DMIC0_SHIFT          (24U)
/*! DMIC0 - dmic0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_DMIC0(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_DMIC0_SHIFT)) & CLKCTL1_PSCCTL0_CLR_DMIC0_MASK)

#define CLKCTL1_PSCCTL0_CLR_OSEVENTTIMER_MASK    (0x8000000U)
#define CLKCTL1_PSCCTL0_CLR_OSEVENTTIMER_SHIFT   (27U)
/*! OSEVENTTIMER - oseventtimer clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL0 Bit
 */
#define CLKCTL1_PSCCTL0_CLR_OSEVENTTIMER(x)      (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL0_CLR_OSEVENTTIMER_SHIFT)) & CLKCTL1_PSCCTL0_CLR_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PSCCTL1_CLR - Peripheral clock clear 1 */
/*! @{ */

#define CLKCTL1_PSCCTL1_CLR_HSGPIO0_MASK         (0x1U)
#define CLKCTL1_PSCCTL1_CLR_HSGPIO0_SHIFT        (0U)
/*! HSGPIO0 - hsgpio0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_CLR_HSGPIO0(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_CLR_HSGPIO0_SHIFT)) & CLKCTL1_PSCCTL1_CLR_HSGPIO0_MASK)

#define CLKCTL1_PSCCTL1_CLR_HSGPIO1_MASK         (0x2U)
#define CLKCTL1_PSCCTL1_CLR_HSGPIO1_SHIFT        (1U)
/*! HSGPIO1 - hsgpio1 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_CLR_HSGPIO1(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_CLR_HSGPIO1_SHIFT)) & CLKCTL1_PSCCTL1_CLR_HSGPIO1_MASK)

#define CLKCTL1_PSCCTL1_CLR_CRC_MASK             (0x10000U)
#define CLKCTL1_PSCCTL1_CLR_CRC_SHIFT            (16U)
/*! CRC - crc clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_CLR_CRC(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_CLR_CRC_SHIFT)) & CLKCTL1_PSCCTL1_CLR_CRC_MASK)

#define CLKCTL1_PSCCTL1_CLR_FREQME_MASK          (0x80000000U)
#define CLKCTL1_PSCCTL1_CLR_FREQME_SHIFT         (31U)
/*! FREQME - freqme clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL1 Bit
 */
#define CLKCTL1_PSCCTL1_CLR_FREQME(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL1_CLR_FREQME_SHIFT)) & CLKCTL1_PSCCTL1_CLR_FREQME_MASK)
/*! @} */

/*! @name PSCCTL2_CLR - Peripheral clock clear 2 */
/*! @{ */

#define CLKCTL1_PSCCTL2_CLR_CT32B0_MASK          (0x1U)
#define CLKCTL1_PSCCTL2_CLR_CT32B0_SHIFT         (0U)
/*! CT32B0 - ct32b0 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_CT32B0(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_CT32B0_SHIFT)) & CLKCTL1_PSCCTL2_CLR_CT32B0_MASK)

#define CLKCTL1_PSCCTL2_CLR_CT32B1_MASK          (0x2U)
#define CLKCTL1_PSCCTL2_CLR_CT32B1_SHIFT         (1U)
/*! CT32B1 - ct32b1 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_CT32B1(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_CT32B1_SHIFT)) & CLKCTL1_PSCCTL2_CLR_CT32B1_MASK)

#define CLKCTL1_PSCCTL2_CLR_CT32B2_MASK          (0x4U)
#define CLKCTL1_PSCCTL2_CLR_CT32B2_SHIFT         (2U)
/*! CT32B2 - ct32b2 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_CT32B2(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_CT32B2_SHIFT)) & CLKCTL1_PSCCTL2_CLR_CT32B2_MASK)

#define CLKCTL1_PSCCTL2_CLR_CT32B3_MASK          (0x8U)
#define CLKCTL1_PSCCTL2_CLR_CT32B3_SHIFT         (3U)
/*! CT32B3 - ct32b3 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_CT32B3(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_CT32B3_SHIFT)) & CLKCTL1_PSCCTL2_CLR_CT32B3_MASK)

#define CLKCTL1_PSCCTL2_CLR_CT32B4_MASK          (0x10U)
#define CLKCTL1_PSCCTL2_CLR_CT32B4_SHIFT         (4U)
/*! CT32B4 - ct32b4 clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_CT32B4(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_CT32B4_SHIFT)) & CLKCTL1_PSCCTL2_CLR_CT32B4_MASK)

#define CLKCTL1_PSCCTL2_CLR_PMU_MASK             (0x40U)
#define CLKCTL1_PSCCTL2_CLR_PMU_SHIFT            (6U)
/*! PMU - pmu clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_PMU(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_PMU_SHIFT)) & CLKCTL1_PSCCTL2_CLR_PMU_MASK)

#define CLKCTL1_PSCCTL2_CLR_RTC_LITE_MASK        (0x80U)
#define CLKCTL1_PSCCTL2_CLR_RTC_LITE_SHIFT       (7U)
/*! RTC_LITE - rtc_lite clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_RTC_LITE(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_RTC_LITE_SHIFT)) & CLKCTL1_PSCCTL2_CLR_RTC_LITE_MASK)

#define CLKCTL1_PSCCTL2_CLR_MRT_MASK             (0x100U)
#define CLKCTL1_PSCCTL2_CLR_MRT_SHIFT            (8U)
/*! MRT - mrt clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_MRT(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_MRT_SHIFT)) & CLKCTL1_PSCCTL2_CLR_MRT_MASK)

#define CLKCTL1_PSCCTL2_CLR_GPIO_INT_MASK        (0x40000000U)
#define CLKCTL1_PSCCTL2_CLR_GPIO_INT_SHIFT       (30U)
/*! GPIO_INT - gpio_int clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_GPIO_INT(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_GPIO_INT_SHIFT)) & CLKCTL1_PSCCTL2_CLR_GPIO_INT_MASK)

#define CLKCTL1_PSCCTL2_CLR_PMUX_MASK            (0x80000000U)
#define CLKCTL1_PSCCTL2_CLR_PMUX_SHIFT           (31U)
/*! PMUX - pmux clock clear
 *  0b0..No effect
 *  0b1..Clears the PSCCTL2 Bit
 */
#define CLKCTL1_PSCCTL2_CLR_PMUX(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_PSCCTL2_CLR_PMUX_SHIFT)) & CLKCTL1_PSCCTL2_CLR_PMUX_MASK)
/*! @} */

/*! @name AUDIOPLLCLKDIV - Audio PLL0 clock divider */
/*! @{ */

#define CLKCTL1_AUDIOPLLCLKDIV_DIV_MASK          (0xFFU)
#define CLKCTL1_AUDIOPLLCLKDIV_DIV_SHIFT         (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL1_AUDIOPLLCLKDIV_DIV(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOPLLCLKDIV_DIV_SHIFT)) & CLKCTL1_AUDIOPLLCLKDIV_DIV_MASK)

#define CLKCTL1_AUDIOPLLCLKDIV_RESET_MASK        (0x20000000U)
#define CLKCTL1_AUDIOPLLCLKDIV_RESET_SHIFT       (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL1_AUDIOPLLCLKDIV_RESET(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOPLLCLKDIV_RESET_SHIFT)) & CLKCTL1_AUDIOPLLCLKDIV_RESET_MASK)

#define CLKCTL1_AUDIOPLLCLKDIV_HALT_MASK         (0x40000000U)
#define CLKCTL1_AUDIOPLLCLKDIV_HALT_SHIFT        (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL1_AUDIOPLLCLKDIV_HALT(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOPLLCLKDIV_HALT_SHIFT)) & CLKCTL1_AUDIOPLLCLKDIV_HALT_MASK)

#define CLKCTL1_AUDIOPLLCLKDIV_REQFLAG_MASK      (0x80000000U)
#define CLKCTL1_AUDIOPLLCLKDIV_REQFLAG_SHIFT     (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL1_AUDIOPLLCLKDIV_REQFLAG(x)        (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOPLLCLKDIV_REQFLAG_SHIFT)) & CLKCTL1_AUDIOPLLCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name OSEVENTFCLKSEL - OS EVENT clock selection */
/*! @{ */

#define CLKCTL1_OSEVENTFCLKSEL_SEL_MASK          (0x7U)
#define CLKCTL1_OSEVENTFCLKSEL_SEL_SHIFT         (0U)
/*! SEL - OS Event Timer Functional Clock Source Selection
 *  0b000..1m_lposc
 *  0b001..32 kHz Clock
 *  0b010..Cortex-M33 clock (hclk)
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_OSEVENTFCLKSEL_SEL(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_OSEVENTFCLKSEL_SEL_SHIFT)) & CLKCTL1_OSEVENTFCLKSEL_SEL_MASK)
/*! @} */

/*! @name FRGCLKSEL - FRG clock selection 0..FRG clock selection 3 */
/*! @{ */

#define CLKCTL1_FRGCLKSEL_SEL_MASK               (0x7U)
#define CLKCTL1_FRGCLKSEL_SEL_SHIFT              (0U)
/*! SEL - Fractional Gen. Clock Source Selection
 *  0b000..Main Clock
 *  0b001..FRG PLL Clock
 *  0b010..SFRO Clock (16m_irc).
 *  0b011..FFRO Clock (48/60m_irc).
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_FRGCLKSEL_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGCLKSEL_SEL_SHIFT)) & CLKCTL1_FRGCLKSEL_SEL_MASK)
/*! @} */

/* The count of CLKCTL1_FRGCLKSEL */
#define CLKCTL1_FRGCLKSEL_COUNT                  (4U)

/*! @name FRGCTL - FRG clock controller 0..FRG clock controller 3 */
/*! @{ */

#define CLKCTL1_FRGCTL_DIV_MASK                  (0xFFU)
#define CLKCTL1_FRGCTL_DIV_SHIFT                 (0U)
/*! DIV - Denominator of the fractional divider. DIV is minus 1 encoded, the denominator value is
 *    the value of this field + 1. Always set to 0xFF (denominator = 256) to use with the fractional
 *    baud rate generator.
 */
#define CLKCTL1_FRGCTL_DIV(x)                    (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGCTL_DIV_SHIFT)) & CLKCTL1_FRGCTL_DIV_MASK)

#define CLKCTL1_FRGCTL_MULT_MASK                 (0xFF00U)
#define CLKCTL1_FRGCTL_MULT_SHIFT                (8U)
/*! MULT - Numerator of the fractional divider. MULT is not minus 1 encoded, so the numerator value
 *    is simply the value in this field.
 */
#define CLKCTL1_FRGCTL_MULT(x)                   (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGCTL_MULT_SHIFT)) & CLKCTL1_FRGCTL_MULT_MASK)
/*! @} */

/* The count of CLKCTL1_FRGCTL */
#define CLKCTL1_FRGCTL_COUNT                     (4U)

/*! @name FCFCLKSEL - Flexcomm Interface clock selection 0..Flexcomm Interface clock selection 3 */
/*! @{ */

#define CLKCTL1_FCFCLKSEL_SEL_MASK               (0x7U)
#define CLKCTL1_FCFCLKSEL_SEL_SHIFT              (0U)
/*! SEL - Flexcomm Functional Clock Source Selection
 *  0b000..SFRO Clock (16m_irc)
 *  0b001..FFRO Clock (48/60m_irc).
 *  0b010..Audio PLL Clock (audio_pll_clk).
 *  0b011..Master Clock In (mclk_in).
 *  0b100..FCn FRG Clock (frg_clk n).
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_FCFCLKSEL_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FCFCLKSEL_SEL_SHIFT)) & CLKCTL1_FCFCLKSEL_SEL_MASK)
/*! @} */

/* The count of CLKCTL1_FCFCLKSEL */
#define CLKCTL1_FCFCLKSEL_COUNT                  (4U)

/*! @name FRG14CLKSEL - FRG clock selection 14 */
/*! @{ */

#define CLKCTL1_FRG14CLKSEL_SEL_MASK             (0x7U)
#define CLKCTL1_FRG14CLKSEL_SEL_SHIFT            (0U)
/*! SEL - Fractional Gen. Clock Source Selection
 *  0b000..Main Clock
 *  0b001..FRG PLL Clock
 *  0b010..SFRO Clock (16m_irc).
 *  0b011..FFRO Clock (48/60m_irc).
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_FRG14CLKSEL_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRG14CLKSEL_SEL_SHIFT)) & CLKCTL1_FRG14CLKSEL_SEL_MASK)
/*! @} */

/*! @name FRG14CTL - FRG clock controller 14 */
/*! @{ */

#define CLKCTL1_FRG14CTL_DIV_MASK                (0xFFU)
#define CLKCTL1_FRG14CTL_DIV_SHIFT               (0U)
/*! DIV - Denominator of the fractional divider. DIV is minus 1 encoded, the denominator value is
 *    the value of this field + 1. Always set to 0xFF (denominator = 256) to use with the fractional
 *    baud rate generator.
 */
#define CLKCTL1_FRG14CTL_DIV(x)                  (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRG14CTL_DIV_SHIFT)) & CLKCTL1_FRG14CTL_DIV_MASK)

#define CLKCTL1_FRG14CTL_MULT_MASK               (0xFF00U)
#define CLKCTL1_FRG14CTL_MULT_SHIFT              (8U)
/*! MULT - Numerator of the fractional divider. MULT is not minus 1 encoded, so the numerator value
 *    is simply the value in this field.
 */
#define CLKCTL1_FRG14CTL_MULT(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRG14CTL_MULT_SHIFT)) & CLKCTL1_FRG14CTL_MULT_MASK)
/*! @} */

/*! @name FC14FCLKSEL - Flexcomm Interface clock selection 14 */
/*! @{ */

#define CLKCTL1_FC14FCLKSEL_SEL_MASK             (0x7U)
#define CLKCTL1_FC14FCLKSEL_SEL_SHIFT            (0U)
/*! SEL - Flexcomm Functional Clock Source Selection
 *  0b000..SFRO Clock (16m_irc)
 *  0b001..FFRO Clock (48/60m_irc).
 *  0b010..Audio PLL Clock (audio_pll_clk).
 *  0b011..Master Clock In (mclk_in).
 *  0b100..FCn FRG Clock (frg_clk n).
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_FC14FCLKSEL_SEL(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FC14FCLKSEL_SEL_SHIFT)) & CLKCTL1_FC14FCLKSEL_SEL_MASK)
/*! @} */

/*! @name FRGPLLCLKDIV - FRG PLL clock divider */
/*! @{ */

#define CLKCTL1_FRGPLLCLKDIV_DIV_MASK            (0xFFU)
#define CLKCTL1_FRGPLLCLKDIV_DIV_SHIFT           (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL1_FRGPLLCLKDIV_DIV(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGPLLCLKDIV_DIV_SHIFT)) & CLKCTL1_FRGPLLCLKDIV_DIV_MASK)

#define CLKCTL1_FRGPLLCLKDIV_RESET_MASK          (0x20000000U)
#define CLKCTL1_FRGPLLCLKDIV_RESET_SHIFT         (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL1_FRGPLLCLKDIV_RESET(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGPLLCLKDIV_RESET_SHIFT)) & CLKCTL1_FRGPLLCLKDIV_RESET_MASK)

#define CLKCTL1_FRGPLLCLKDIV_HALT_MASK           (0x40000000U)
#define CLKCTL1_FRGPLLCLKDIV_HALT_SHIFT          (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL1_FRGPLLCLKDIV_HALT(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGPLLCLKDIV_HALT_SHIFT)) & CLKCTL1_FRGPLLCLKDIV_HALT_MASK)

#define CLKCTL1_FRGPLLCLKDIV_REQFLAG_MASK        (0x80000000U)
#define CLKCTL1_FRGPLLCLKDIV_REQFLAG_SHIFT       (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL1_FRGPLLCLKDIV_REQFLAG(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_FRGPLLCLKDIV_REQFLAG_SHIFT)) & CLKCTL1_FRGPLLCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name DMIC0FCLKSEL - DMIC0 clock selection */
/*! @{ */

#define CLKCTL1_DMIC0FCLKSEL_SEL_MASK            (0x7U)
#define CLKCTL1_DMIC0FCLKSEL_SEL_SHIFT           (0U)
/*! SEL - DMIC Functional Clock Source Selection
 *  0b000..SFRO Clock (16m_irc)
 *  0b001..FFRO Clock (48/60m_irc).
 *  0b010..Audio PLL Clock (audio_pll_clk).
 *  0b011..Master Clock In (mclk_in).
 *  0b100..1m_lposc
 *  0b101..32 kHz Wake Clk
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_DMIC0FCLKSEL_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_DMIC0FCLKSEL_SEL_SHIFT)) & CLKCTL1_DMIC0FCLKSEL_SEL_MASK)
/*! @} */

/*! @name DMIC0CLKDIV - DMIC clock divider */
/*! @{ */

#define CLKCTL1_DMIC0CLKDIV_DIV_MASK             (0xFFU)
#define CLKCTL1_DMIC0CLKDIV_DIV_SHIFT            (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL1_DMIC0CLKDIV_DIV(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_DMIC0CLKDIV_DIV_SHIFT)) & CLKCTL1_DMIC0CLKDIV_DIV_MASK)

#define CLKCTL1_DMIC0CLKDIV_RESET_MASK           (0x20000000U)
#define CLKCTL1_DMIC0CLKDIV_RESET_SHIFT          (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL1_DMIC0CLKDIV_RESET(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_DMIC0CLKDIV_RESET_SHIFT)) & CLKCTL1_DMIC0CLKDIV_RESET_MASK)

#define CLKCTL1_DMIC0CLKDIV_HALT_MASK            (0x40000000U)
#define CLKCTL1_DMIC0CLKDIV_HALT_SHIFT           (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL1_DMIC0CLKDIV_HALT(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_DMIC0CLKDIV_HALT_SHIFT)) & CLKCTL1_DMIC0CLKDIV_HALT_MASK)

#define CLKCTL1_DMIC0CLKDIV_REQFLAG_MASK         (0x80000000U)
#define CLKCTL1_DMIC0CLKDIV_REQFLAG_SHIFT        (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL1_DMIC0CLKDIV_REQFLAG(x)           (((uint32_t)(((uint32_t)(x)) << CLKCTL1_DMIC0CLKDIV_REQFLAG_SHIFT)) & CLKCTL1_DMIC0CLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name CT32BITFCLKSEL - Ct32bit timer 0 clock selection..Ct32bit timer 3 clock selection */
/*! @{ */

#define CLKCTL1_CT32BITFCLKSEL_SEL_MASK          (0x7U)
#define CLKCTL1_CT32BITFCLKSEL_SEL_SHIFT         (0U)
/*! SEL - CT32Bit Functional Clock Source Selection
 *  0b000..Main Clock
 *  0b001..SFRO Clock (16m_irc).
 *  0b010..FFRO Clock (48/60m_irc).
 *  0b011..Audio PLL Clock (audio_pll_clk).
 *  0b100..Master Clock In (mclk_in).
 *  0b101..1m_lposc
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_CT32BITFCLKSEL_SEL(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CT32BITFCLKSEL_SEL_SHIFT)) & CLKCTL1_CT32BITFCLKSEL_SEL_MASK)
/*! @} */

/*! @name AUDIOMCLKSEL - Audio MCLK selection */
/*! @{ */

#define CLKCTL1_AUDIOMCLKSEL_SEL_MASK            (0x7U)
#define CLKCTL1_AUDIOMCLKSEL_SEL_SHIFT           (0U)
/*! SEL - Audio MCLK Clock Source Selection
 *  0b000..FFRO Clock (48/60m_irc).
 *  0b001..Audio PLL Clock (audio_pll_clk).
 *  0b010..main_clk
 *  0b011..Reserved
 *  0b100..Reserved
 *  0b101..Reserved
 *  0b110..Reserved
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_AUDIOMCLKSEL_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOMCLKSEL_SEL_SHIFT)) & CLKCTL1_AUDIOMCLKSEL_SEL_MASK)
/*! @} */

/*! @name AUDIOMCLKDIV - Audio MCLK divider */
/*! @{ */

#define CLKCTL1_AUDIOMCLKDIV_DIV_MASK            (0xFFU)
#define CLKCTL1_AUDIOMCLKDIV_DIV_SHIFT           (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL1_AUDIOMCLKDIV_DIV(x)              (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOMCLKDIV_DIV_SHIFT)) & CLKCTL1_AUDIOMCLKDIV_DIV_MASK)

#define CLKCTL1_AUDIOMCLKDIV_RESET_MASK          (0x20000000U)
#define CLKCTL1_AUDIOMCLKDIV_RESET_SHIFT         (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL1_AUDIOMCLKDIV_RESET(x)            (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOMCLKDIV_RESET_SHIFT)) & CLKCTL1_AUDIOMCLKDIV_RESET_MASK)

#define CLKCTL1_AUDIOMCLKDIV_HALT_MASK           (0x40000000U)
#define CLKCTL1_AUDIOMCLKDIV_HALT_SHIFT          (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL1_AUDIOMCLKDIV_HALT(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOMCLKDIV_HALT_SHIFT)) & CLKCTL1_AUDIOMCLKDIV_HALT_MASK)

#define CLKCTL1_AUDIOMCLKDIV_REQFLAG_MASK        (0x80000000U)
#define CLKCTL1_AUDIOMCLKDIV_REQFLAG_SHIFT       (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL1_AUDIOMCLKDIV_REQFLAG(x)          (((uint32_t)(((uint32_t)(x)) << CLKCTL1_AUDIOMCLKDIV_REQFLAG_SHIFT)) & CLKCTL1_AUDIOMCLKDIV_REQFLAG_MASK)
/*! @} */

/*! @name CLKOUTSEL0 - Clock out selection 0 */
/*! @{ */

#define CLKCTL1_CLKOUTSEL0_SEL_MASK              (0x7U)
#define CLKCTL1_CLKOUTSEL0_SEL_SHIFT             (0U)
/*! SEL - Clock Output Select 1st Stage
 *  0b000..SFRO Clock
 *  0b001..External clock (clk_in).
 *  0b010..1m_lposc
 *  0b011..FFRO Clock
 *  0b100..Main Clock (main_clk).
 *  0b101..refclk_sys(38.4M).
 *  0b110..avpll_ch2_CLKOUT(64M)
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_CLKOUTSEL0_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTSEL0_SEL_SHIFT)) & CLKCTL1_CLKOUTSEL0_SEL_MASK)
/*! @} */

/*! @name CLKOUTSEL1 - Clock out selection 1 */
/*! @{ */

#define CLKCTL1_CLKOUTSEL1_SEL_MASK              (0x7U)
#define CLKCTL1_CLKOUTSEL1_SEL_SHIFT             (0U)
/*! SEL - Clock Output Select 2nd Stage
 *  0b000..CLKOUTSEL0 Multiplexed Output
 *  0b001..Main PLL Clock (main_pll_clk).
 *  0b010..AUX0 PLL clock (aux0_pll_clk).
 *  0b011..Reserved
 *  0b100..AUX1 PLL clock (aux1_pll_clk)
 *  0b101..Audio PLL Clock (audio_pll_clk).
 *  0b110..32 kHz RTC Clock.
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_CLKOUTSEL1_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTSEL1_SEL_SHIFT)) & CLKCTL1_CLKOUTSEL1_SEL_MASK)
/*! @} */

/*! @name CLKOUTDIV - Clock out divider */
/*! @{ */

#define CLKCTL1_CLKOUTDIV_DIV_MASK               (0xFFU)
#define CLKCTL1_CLKOUTDIV_DIV_SHIFT              (0U)
/*! DIV - Clock Divider Value Selection: 0: Divide by 1... 255: Divide by 256 */
#define CLKCTL1_CLKOUTDIV_DIV(x)                 (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTDIV_DIV_SHIFT)) & CLKCTL1_CLKOUTDIV_DIV_MASK)

#define CLKCTL1_CLKOUTDIV_RESET_MASK             (0x20000000U)
#define CLKCTL1_CLKOUTDIV_RESET_SHIFT            (29U)
/*! RESET - Resets the divider counter. Can be used to make sure a new divider value is used right
 *    away rather than completing the previous count
 */
#define CLKCTL1_CLKOUTDIV_RESET(x)               (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTDIV_RESET_SHIFT)) & CLKCTL1_CLKOUTDIV_RESET_MASK)

#define CLKCTL1_CLKOUTDIV_HALT_MASK              (0x40000000U)
#define CLKCTL1_CLKOUTDIV_HALT_SHIFT             (30U)
/*! HALT - Halts the divider counter. The intent is to allow the divider clock source to be changed
 *    without the risk of a glitch at the output
 */
#define CLKCTL1_CLKOUTDIV_HALT(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTDIV_HALT_SHIFT)) & CLKCTL1_CLKOUTDIV_HALT_MASK)

#define CLKCTL1_CLKOUTDIV_REQFLAG_MASK           (0x80000000U)
#define CLKCTL1_CLKOUTDIV_REQFLAG_SHIFT          (31U)
/*! REQFLAG - Divider status flag. Set when a change is made to the divider value, cleared when the
 *    change is complete. The clock being divided must be running for this status to change
 */
#define CLKCTL1_CLKOUTDIV_REQFLAG(x)             (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTDIV_REQFLAG_SHIFT)) & CLKCTL1_CLKOUTDIV_REQFLAG_MASK)
/*! @} */

/*! @name CLKOUTSEL2 - Clock out selection 2 */
/*! @{ */

#define CLKCTL1_CLKOUTSEL2_SEL_MASK              (0x7U)
#define CLKCTL1_CLKOUTSEL2_SEL_SHIFT             (0U)
/*! SEL - Clock Output Select 3rd Stage
 *  0b000..CLKOUTSEL1 Multiplexed Output
 *  0b001..tcpu_mci_flexspi_clk
 *  0b010..tddr_mci_flexspi_clk
 *  0b011..t3pll_mci_flexspi_clk
 *  0b100..t3pll_mci_256m
 *  0b101..cau_slp_ref_clk
 *  0b110..tddr_mci_enet_clk
 *  0b111..None, this may be selected in order to reduce power when no output is needed
 */
#define CLKCTL1_CLKOUTSEL2_SEL(x)                (((uint32_t)(((uint32_t)(x)) << CLKCTL1_CLKOUTSEL2_SEL_SHIFT)) & CLKCTL1_CLKOUTSEL2_SEL_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CLKCTL1_Register_Masks */


/*!
 * @}
 */ /* end of group CLKCTL1_Peripheral_Access_Layer */


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


#endif  /* CLKCTL1_H_ */

