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
**         CMSIS Peripheral Access Layer for RSTCTL1
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
 * @file RSTCTL1.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for RSTCTL1
 *
 * CMSIS Peripheral Access Layer for RSTCTL1
 */

#if !defined(RSTCTL1_H_)
#define RSTCTL1_H_                               /**< Symbol preventing repeated inclusion */

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
   -- RSTCTL1 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RSTCTL1_Peripheral_Access_Layer RSTCTL1 Peripheral Access Layer
 * @{
 */

/** RSTCTL1 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t PRSTCTL0;                          /**< Peripheral reset control 0, offset: 0x10 */
  __IO uint32_t PRSTCTL1;                          /**< Peripheral reset control 1, offset: 0x14 */
  __IO uint32_t PRSTCTL2;                          /**< Peripheral reset control 2, offset: 0x18 */
       uint8_t RESERVED_1[36];
  __O  uint32_t PRSTCTL0_SET;                      /**< Peripheral reset set 0, offset: 0x40 */
  __IO uint32_t PRSTCTL1_SET;                      /**< Peripheral reset set 1, offset: 0x44 */
  __O  uint32_t PRSTCTL2_SET;                      /**< Peripheral reset set 2, offset: 0x48 */
       uint8_t RESERVED_2[36];
  __O  uint32_t PRSTCTL0_CLR;                      /**< Peripheral reset clear 0, offset: 0x70 */
  __O  uint32_t PRSTCTL1_CLR;                      /**< Peripheral reset clear 1, offset: 0x74 */
  __O  uint32_t PRSTCTL2_CLR;                      /**< Peripheral reset clear 2, offset: 0x78 */
       uint8_t RESERVED_3[20];
  __IO uint32_t SDIO;                              /**< SDIO sdclk_sw_rst_n control, offset: 0x90 */
} RSTCTL1_Type;

/* ----------------------------------------------------------------------------
   -- RSTCTL1 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RSTCTL1_Register_Masks RSTCTL1 Register Masks
 * @{
 */

/*! @name PRSTCTL0 - Peripheral reset control 0 */
/*! @{ */

#define RSTCTL1_PRSTCTL0_FC0_MASK                (0x100U)
#define RSTCTL1_PRSTCTL0_FC0_SHIFT               (8U)
/*! FC0 - fc0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_FC0(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_FC0_SHIFT)) & RSTCTL1_PRSTCTL0_FC0_MASK)

#define RSTCTL1_PRSTCTL0_FC1_MASK                (0x200U)
#define RSTCTL1_PRSTCTL0_FC1_SHIFT               (9U)
/*! FC1 - fc1 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_FC1(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_FC1_SHIFT)) & RSTCTL1_PRSTCTL0_FC1_MASK)

#define RSTCTL1_PRSTCTL0_FC2_MASK                (0x400U)
#define RSTCTL1_PRSTCTL0_FC2_SHIFT               (10U)
/*! FC2 - fc2 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_FC2(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_FC2_SHIFT)) & RSTCTL1_PRSTCTL0_FC2_MASK)

#define RSTCTL1_PRSTCTL0_FC3_MASK                (0x800U)
#define RSTCTL1_PRSTCTL0_FC3_SHIFT               (11U)
/*! FC3 - fc3 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_FC3(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_FC3_SHIFT)) & RSTCTL1_PRSTCTL0_FC3_MASK)

#define RSTCTL1_PRSTCTL0_FC14_MASK               (0x400000U)
#define RSTCTL1_PRSTCTL0_FC14_SHIFT              (22U)
/*! FC14 - fc14 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_FC14(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_FC14_SHIFT)) & RSTCTL1_PRSTCTL0_FC14_MASK)

#define RSTCTL1_PRSTCTL0_DMIC0_MASK              (0x1000000U)
#define RSTCTL1_PRSTCTL0_DMIC0_SHIFT             (24U)
/*! DMIC0 - dmic0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_DMIC0(x)                (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_DMIC0_SHIFT)) & RSTCTL1_PRSTCTL0_DMIC0_MASK)

#define RSTCTL1_PRSTCTL0_OSEVENTTIMER_MASK       (0x8000000U)
#define RSTCTL1_PRSTCTL0_OSEVENTTIMER_SHIFT      (27U)
/*! OSEVENTTIMER - oseventtimer reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL0_OSEVENTTIMER(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_OSEVENTTIMER_SHIFT)) & RSTCTL1_PRSTCTL0_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PRSTCTL1 - Peripheral reset control 1 */
/*! @{ */

#define RSTCTL1_PRSTCTL1_HSGPIO0_MASK            (0x1U)
#define RSTCTL1_PRSTCTL1_HSGPIO0_SHIFT           (0U)
/*! HSGPIO0 - hsgpio0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL1_HSGPIO0(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_HSGPIO0_SHIFT)) & RSTCTL1_PRSTCTL1_HSGPIO0_MASK)

#define RSTCTL1_PRSTCTL1_HSGPIO1_MASK            (0x2U)
#define RSTCTL1_PRSTCTL1_HSGPIO1_SHIFT           (1U)
/*! HSGPIO1 - hsgpio1 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL1_HSGPIO1(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_HSGPIO1_SHIFT)) & RSTCTL1_PRSTCTL1_HSGPIO1_MASK)

#define RSTCTL1_PRSTCTL1_CRC_MASK                (0x10000U)
#define RSTCTL1_PRSTCTL1_CRC_SHIFT               (16U)
/*! CRC - crc reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL1_CRC(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_CRC_SHIFT)) & RSTCTL1_PRSTCTL1_CRC_MASK)

#define RSTCTL1_PRSTCTL1_FREQME_MASK             (0x80000000U)
#define RSTCTL1_PRSTCTL1_FREQME_SHIFT            (31U)
/*! FREQME - freqme reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL1_FREQME(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_FREQME_SHIFT)) & RSTCTL1_PRSTCTL1_FREQME_MASK)
/*! @} */

/*! @name PRSTCTL2 - Peripheral reset control 2 */
/*! @{ */

#define RSTCTL1_PRSTCTL2_CT32B0_MASK             (0x1U)
#define RSTCTL1_PRSTCTL2_CT32B0_SHIFT            (0U)
/*! CT32B0 - ct32b0 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_CT32B0(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CT32B0_SHIFT)) & RSTCTL1_PRSTCTL2_CT32B0_MASK)

#define RSTCTL1_PRSTCTL2_CT32B1_MASK             (0x2U)
#define RSTCTL1_PRSTCTL2_CT32B1_SHIFT            (1U)
/*! CT32B1 - ct32b1 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_CT32B1(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CT32B1_SHIFT)) & RSTCTL1_PRSTCTL2_CT32B1_MASK)

#define RSTCTL1_PRSTCTL2_CT32B2_MASK             (0x4U)
#define RSTCTL1_PRSTCTL2_CT32B2_SHIFT            (2U)
/*! CT32B2 - ct32b2 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_CT32B2(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CT32B2_SHIFT)) & RSTCTL1_PRSTCTL2_CT32B2_MASK)

#define RSTCTL1_PRSTCTL2_CT32B3_MASK             (0x8U)
#define RSTCTL1_PRSTCTL2_CT32B3_SHIFT            (3U)
/*! CT32B3 - ct32b3 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_CT32B3(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CT32B3_SHIFT)) & RSTCTL1_PRSTCTL2_CT32B3_MASK)

#define RSTCTL1_PRSTCTL2_CT32B4_MASK             (0x10U)
#define RSTCTL1_PRSTCTL2_CT32B4_SHIFT            (4U)
/*! CT32B4 - ct32b4 reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_CT32B4(x)               (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CT32B4_SHIFT)) & RSTCTL1_PRSTCTL2_CT32B4_MASK)

#define RSTCTL1_PRSTCTL2_MRT_MASK                (0x100U)
#define RSTCTL1_PRSTCTL2_MRT_SHIFT               (8U)
/*! MRT - mrt reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_MRT(x)                  (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_MRT_SHIFT)) & RSTCTL1_PRSTCTL2_MRT_MASK)

#define RSTCTL1_PRSTCTL2_GPIO_INT_MASK           (0x40000000U)
#define RSTCTL1_PRSTCTL2_GPIO_INT_SHIFT          (30U)
/*! GPIO_INT - gpio_int reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_GPIO_INT(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_GPIO_INT_SHIFT)) & RSTCTL1_PRSTCTL2_GPIO_INT_MASK)

#define RSTCTL1_PRSTCTL2_PMUX_MASK               (0x80000000U)
#define RSTCTL1_PRSTCTL2_PMUX_SHIFT              (31U)
/*! PMUX - pmux reset control
 *  0b0..Clear reset
 *  0b1..Set reset
 */
#define RSTCTL1_PRSTCTL2_PMUX(x)                 (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_PMUX_SHIFT)) & RSTCTL1_PRSTCTL2_PMUX_MASK)
/*! @} */

/*! @name PRSTCTL0_SET - Peripheral reset set 0 */
/*! @{ */

#define RSTCTL1_PRSTCTL0_SET_FC0_MASK            (0x100U)
#define RSTCTL1_PRSTCTL0_SET_FC0_SHIFT           (8U)
/*! FC0 - fc0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_FC0(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_FC0_SHIFT)) & RSTCTL1_PRSTCTL0_SET_FC0_MASK)

#define RSTCTL1_PRSTCTL0_SET_FC1_MASK            (0x200U)
#define RSTCTL1_PRSTCTL0_SET_FC1_SHIFT           (9U)
/*! FC1 - fc1 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_FC1(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_FC1_SHIFT)) & RSTCTL1_PRSTCTL0_SET_FC1_MASK)

#define RSTCTL1_PRSTCTL0_SET_FC2_MASK            (0x400U)
#define RSTCTL1_PRSTCTL0_SET_FC2_SHIFT           (10U)
/*! FC2 - fc2 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_FC2(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_FC2_SHIFT)) & RSTCTL1_PRSTCTL0_SET_FC2_MASK)

#define RSTCTL1_PRSTCTL0_SET_FC3_MASK            (0x800U)
#define RSTCTL1_PRSTCTL0_SET_FC3_SHIFT           (11U)
/*! FC3 - fc3 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_FC3(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_FC3_SHIFT)) & RSTCTL1_PRSTCTL0_SET_FC3_MASK)

#define RSTCTL1_PRSTCTL0_SET_FC14_MASK           (0x400000U)
#define RSTCTL1_PRSTCTL0_SET_FC14_SHIFT          (22U)
/*! FC14 - fc14 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_FC14(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_FC14_SHIFT)) & RSTCTL1_PRSTCTL0_SET_FC14_MASK)

#define RSTCTL1_PRSTCTL0_SET_DMIC0_MASK          (0x1000000U)
#define RSTCTL1_PRSTCTL0_SET_DMIC0_SHIFT         (24U)
/*! DMIC0 - dmic0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_DMIC0(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_DMIC0_SHIFT)) & RSTCTL1_PRSTCTL0_SET_DMIC0_MASK)

#define RSTCTL1_PRSTCTL0_SET_OSEVENTTIMER_MASK   (0x8000000U)
#define RSTCTL1_PRSTCTL0_SET_OSEVENTTIMER_SHIFT  (27U)
/*! OSEVENTTIMER - oseventtimer reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_SET_OSEVENTTIMER(x)     (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_SET_OSEVENTTIMER_SHIFT)) & RSTCTL1_PRSTCTL0_SET_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PRSTCTL1_SET - Peripheral reset set 1 */
/*! @{ */

#define RSTCTL1_PRSTCTL1_SET_HSGPIO0_MASK        (0x1U)
#define RSTCTL1_PRSTCTL1_SET_HSGPIO0_SHIFT       (0U)
/*! HSGPIO0 - hsgpio0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_SET_HSGPIO0(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_SET_HSGPIO0_SHIFT)) & RSTCTL1_PRSTCTL1_SET_HSGPIO0_MASK)

#define RSTCTL1_PRSTCTL1_SET_HSGPIO1_MASK        (0x2U)
#define RSTCTL1_PRSTCTL1_SET_HSGPIO1_SHIFT       (1U)
/*! HSGPIO1 - hsgpio1 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_SET_HSGPIO1(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_SET_HSGPIO1_SHIFT)) & RSTCTL1_PRSTCTL1_SET_HSGPIO1_MASK)

#define RSTCTL1_PRSTCTL1_SET_CRC_MASK            (0x10000U)
#define RSTCTL1_PRSTCTL1_SET_CRC_SHIFT           (16U)
/*! CRC - crc reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_SET_CRC(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_SET_CRC_SHIFT)) & RSTCTL1_PRSTCTL1_SET_CRC_MASK)

#define RSTCTL1_PRSTCTL1_SET_FREQME_MASK         (0x80000000U)
#define RSTCTL1_PRSTCTL1_SET_FREQME_SHIFT        (31U)
/*! FREQME - freqme reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_SET_FREQME(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_SET_FREQME_SHIFT)) & RSTCTL1_PRSTCTL1_SET_FREQME_MASK)
/*! @} */

/*! @name PRSTCTL2_SET - Peripheral reset set 2 */
/*! @{ */

#define RSTCTL1_PRSTCTL2_SET_CT32B0_MASK         (0x1U)
#define RSTCTL1_PRSTCTL2_SET_CT32B0_SHIFT        (0U)
/*! CT32B0 - ct32b0 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_CT32B0(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_CT32B0_SHIFT)) & RSTCTL1_PRSTCTL2_SET_CT32B0_MASK)

#define RSTCTL1_PRSTCTL2_SET_CT32B1_MASK         (0x2U)
#define RSTCTL1_PRSTCTL2_SET_CT32B1_SHIFT        (1U)
/*! CT32B1 - ct32b1 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_CT32B1(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_CT32B1_SHIFT)) & RSTCTL1_PRSTCTL2_SET_CT32B1_MASK)

#define RSTCTL1_PRSTCTL2_SET_CT32B2_MASK         (0x4U)
#define RSTCTL1_PRSTCTL2_SET_CT32B2_SHIFT        (2U)
/*! CT32B2 - ct32b2 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_CT32B2(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_CT32B2_SHIFT)) & RSTCTL1_PRSTCTL2_SET_CT32B2_MASK)

#define RSTCTL1_PRSTCTL2_SET_CT32B3_MASK         (0x8U)
#define RSTCTL1_PRSTCTL2_SET_CT32B3_SHIFT        (3U)
/*! CT32B3 - ct32b3 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_CT32B3(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_CT32B3_SHIFT)) & RSTCTL1_PRSTCTL2_SET_CT32B3_MASK)

#define RSTCTL1_PRSTCTL2_SET_CT32B4_MASK         (0x10U)
#define RSTCTL1_PRSTCTL2_SET_CT32B4_SHIFT        (4U)
/*! CT32B4 - ct32b4 reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_CT32B4(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_CT32B4_SHIFT)) & RSTCTL1_PRSTCTL2_SET_CT32B4_MASK)

#define RSTCTL1_PRSTCTL2_SET_MRT_MASK            (0x100U)
#define RSTCTL1_PRSTCTL2_SET_MRT_SHIFT           (8U)
/*! MRT - mrt reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_MRT(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_MRT_SHIFT)) & RSTCTL1_PRSTCTL2_SET_MRT_MASK)

#define RSTCTL1_PRSTCTL2_SET_GPIO_INT_MASK       (0x40000000U)
#define RSTCTL1_PRSTCTL2_SET_GPIO_INT_SHIFT      (30U)
/*! GPIO_INT - gpio_int reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_GPIO_INT(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_GPIO_INT_SHIFT)) & RSTCTL1_PRSTCTL2_SET_GPIO_INT_MASK)

#define RSTCTL1_PRSTCTL2_SET_PMUX_MASK           (0x80000000U)
#define RSTCTL1_PRSTCTL2_SET_PMUX_SHIFT          (31U)
/*! PMUX - pmux reset set
 *  0b0..No effect
 *  0b1..Sets the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_SET_PMUX(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_SET_PMUX_SHIFT)) & RSTCTL1_PRSTCTL2_SET_PMUX_MASK)
/*! @} */

/*! @name PRSTCTL0_CLR - Peripheral reset clear 0 */
/*! @{ */

#define RSTCTL1_PRSTCTL0_CLR_FC0_MASK            (0x100U)
#define RSTCTL1_PRSTCTL0_CLR_FC0_SHIFT           (8U)
/*! FC0 - fc0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_FC0(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_FC0_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_FC0_MASK)

#define RSTCTL1_PRSTCTL0_CLR_FC1_MASK            (0x200U)
#define RSTCTL1_PRSTCTL0_CLR_FC1_SHIFT           (9U)
/*! FC1 - fc1 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_FC1(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_FC1_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_FC1_MASK)

#define RSTCTL1_PRSTCTL0_CLR_FC2_MASK            (0x400U)
#define RSTCTL1_PRSTCTL0_CLR_FC2_SHIFT           (10U)
/*! FC2 - fc2 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_FC2(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_FC2_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_FC2_MASK)

#define RSTCTL1_PRSTCTL0_CLR_FC3_MASK            (0x800U)
#define RSTCTL1_PRSTCTL0_CLR_FC3_SHIFT           (11U)
/*! FC3 - fc3 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_FC3(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_FC3_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_FC3_MASK)

#define RSTCTL1_PRSTCTL0_CLR_FC14_MASK           (0x400000U)
#define RSTCTL1_PRSTCTL0_CLR_FC14_SHIFT          (22U)
/*! FC14 - fc14 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_FC14(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_FC14_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_FC14_MASK)

#define RSTCTL1_PRSTCTL0_CLR_DMIC0_MASK          (0x1000000U)
#define RSTCTL1_PRSTCTL0_CLR_DMIC0_SHIFT         (24U)
/*! DMIC0 - dmic0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_DMIC0(x)            (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_DMIC0_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_DMIC0_MASK)

#define RSTCTL1_PRSTCTL0_CLR_OSEVENTTIMER_MASK   (0x8000000U)
#define RSTCTL1_PRSTCTL0_CLR_OSEVENTTIMER_SHIFT  (27U)
/*! OSEVENTTIMER - oseventtimer reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL0 Bit
 */
#define RSTCTL1_PRSTCTL0_CLR_OSEVENTTIMER(x)     (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL0_CLR_OSEVENTTIMER_SHIFT)) & RSTCTL1_PRSTCTL0_CLR_OSEVENTTIMER_MASK)
/*! @} */

/*! @name PRSTCTL1_CLR - Peripheral reset clear 1 */
/*! @{ */

#define RSTCTL1_PRSTCTL1_CLR_HSGPIO0_MASK        (0x1U)
#define RSTCTL1_PRSTCTL1_CLR_HSGPIO0_SHIFT       (0U)
/*! HSGPIO0 - hsgpio0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_CLR_HSGPIO0(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_CLR_HSGPIO0_SHIFT)) & RSTCTL1_PRSTCTL1_CLR_HSGPIO0_MASK)

#define RSTCTL1_PRSTCTL1_CLR_HSGPIO1_MASK        (0x2U)
#define RSTCTL1_PRSTCTL1_CLR_HSGPIO1_SHIFT       (1U)
/*! HSGPIO1 - hsgpio1 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_CLR_HSGPIO1(x)          (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_CLR_HSGPIO1_SHIFT)) & RSTCTL1_PRSTCTL1_CLR_HSGPIO1_MASK)

#define RSTCTL1_PRSTCTL1_CLR_CRC_MASK            (0x10000U)
#define RSTCTL1_PRSTCTL1_CLR_CRC_SHIFT           (16U)
/*! CRC - crc reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_CLR_CRC(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_CLR_CRC_SHIFT)) & RSTCTL1_PRSTCTL1_CLR_CRC_MASK)

#define RSTCTL1_PRSTCTL1_CLR_FREQME_MASK         (0x80000000U)
#define RSTCTL1_PRSTCTL1_CLR_FREQME_SHIFT        (31U)
/*! FREQME - freqme reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL1 Bit
 */
#define RSTCTL1_PRSTCTL1_CLR_FREQME(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL1_CLR_FREQME_SHIFT)) & RSTCTL1_PRSTCTL1_CLR_FREQME_MASK)
/*! @} */

/*! @name PRSTCTL2_CLR - Peripheral reset clear 2 */
/*! @{ */

#define RSTCTL1_PRSTCTL2_CLR_CT32B0_MASK         (0x1U)
#define RSTCTL1_PRSTCTL2_CLR_CT32B0_SHIFT        (0U)
/*! CT32B0 - ct32b0 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_CT32B0(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_CT32B0_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_CT32B0_MASK)

#define RSTCTL1_PRSTCTL2_CLR_CT32B1_MASK         (0x2U)
#define RSTCTL1_PRSTCTL2_CLR_CT32B1_SHIFT        (1U)
/*! CT32B1 - ct32b1 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_CT32B1(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_CT32B1_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_CT32B1_MASK)

#define RSTCTL1_PRSTCTL2_CLR_CT32B2_MASK         (0x4U)
#define RSTCTL1_PRSTCTL2_CLR_CT32B2_SHIFT        (2U)
/*! CT32B2 - ct32b2 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_CT32B2(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_CT32B2_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_CT32B2_MASK)

#define RSTCTL1_PRSTCTL2_CLR_CT32B3_MASK         (0x8U)
#define RSTCTL1_PRSTCTL2_CLR_CT32B3_SHIFT        (3U)
/*! CT32B3 - ct32b3 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_CT32B3(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_CT32B3_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_CT32B3_MASK)

#define RSTCTL1_PRSTCTL2_CLR_CT32B4_MASK         (0x10U)
#define RSTCTL1_PRSTCTL2_CLR_CT32B4_SHIFT        (4U)
/*! CT32B4 - ct32b4 reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_CT32B4(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_CT32B4_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_CT32B4_MASK)

#define RSTCTL1_PRSTCTL2_CLR_MRT_MASK            (0x100U)
#define RSTCTL1_PRSTCTL2_CLR_MRT_SHIFT           (8U)
/*! MRT - mrt reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_MRT(x)              (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_MRT_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_MRT_MASK)

#define RSTCTL1_PRSTCTL2_CLR_GPIO_INT_MASK       (0x40000000U)
#define RSTCTL1_PRSTCTL2_CLR_GPIO_INT_SHIFT      (30U)
/*! GPIO_INT - gpio_int reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_GPIO_INT(x)         (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_GPIO_INT_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_GPIO_INT_MASK)

#define RSTCTL1_PRSTCTL2_CLR_PMUX_MASK           (0x80000000U)
#define RSTCTL1_PRSTCTL2_CLR_PMUX_SHIFT          (31U)
/*! PMUX - pmux reset clear
 *  0b0..No effect
 *  0b1..Clears the PRSTCTL2 Bit
 */
#define RSTCTL1_PRSTCTL2_CLR_PMUX(x)             (((uint32_t)(((uint32_t)(x)) << RSTCTL1_PRSTCTL2_CLR_PMUX_SHIFT)) & RSTCTL1_PRSTCTL2_CLR_PMUX_MASK)
/*! @} */

/*! @name SDIO - SDIO sdclk_sw_rst_n control */
/*! @{ */

#define RSTCTL1_SDIO_SDCLK_SW_RST_N_MASK         (0x1U)
#define RSTCTL1_SDIO_SDCLK_SW_RST_N_SHIFT        (0U)
/*! SDCLK_SW_RST_N - 0: sw reset 1: reset release */
#define RSTCTL1_SDIO_SDCLK_SW_RST_N(x)           (((uint32_t)(((uint32_t)(x)) << RSTCTL1_SDIO_SDCLK_SW_RST_N_SHIFT)) & RSTCTL1_SDIO_SDCLK_SW_RST_N_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group RSTCTL1_Register_Masks */


/*!
 * @}
 */ /* end of group RSTCTL1_Peripheral_Access_Layer */


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


#endif  /* RSTCTL1_H_ */

