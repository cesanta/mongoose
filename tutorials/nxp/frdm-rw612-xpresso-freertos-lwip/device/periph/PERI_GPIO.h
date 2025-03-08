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
**         CMSIS Peripheral Access Layer for GPIO
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
 * @file GPIO.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for GPIO
 *
 * CMSIS Peripheral Access Layer for GPIO
 */

#if !defined(GPIO_H_)
#define GPIO_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- GPIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Peripheral_Access_Layer GPIO Peripheral Access Layer
 * @{
 */

/** GPIO - Size of Registers Arrays */
#define GPIO_BYTE_PIN_COUNT                       2u
#define GPIO_BYTE_PIN_BYTE_PIN_COUNT              32u
#define GPIO_WORD_PIN_COUNT                       2u
#define GPIO_WORD_PIN_WORD_PIN_COUNT              32u
#define GPIO_DIR_COUNT                            2u
#define GPIO_MASK_COUNT                           2u
#define GPIO_PIN_COUNT                            2u
#define GPIO_MPIN_COUNT                           2u
#define GPIO_SET_COUNT                            2u
#define GPIO_CLR_COUNT                            2u
#define GPIO_NOT_COUNT                            2u
#define GPIO_DIRSET_COUNT                         2u
#define GPIO_DIRCLR_COUNT                         2u
#define GPIO_DIRNOT_COUNT                         2u
#define GPIO_INTENA_COUNT                         2u
#define GPIO_INTENB_COUNT                         2u
#define GPIO_INTPOL_COUNT                         2u
#define GPIO_INTEDG_COUNT                         2u
#define GPIO_INTSTATA_COUNT                       2u
#define GPIO_INTSTATB_COUNT                       2u

/** GPIO - Register Layout Typedef */
typedef struct {
  __IO uint8_t B[GPIO_BYTE_PIN_COUNT][GPIO_BYTE_PIN_BYTE_PIN_COUNT]; /**< Byte pin registers for all port GPIO pins, array offset: 0x0, array step: index*0x20, index2*0x1, irregular array, not all indices are valid */
       uint8_t RESERVED_0[4032];
  __IO uint32_t W[GPIO_WORD_PIN_COUNT][GPIO_WORD_PIN_WORD_PIN_COUNT]; /**< Word pin registers for all port GPIO pins, array offset: 0x1000, array step: index*0x80, index2*0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_1[3840];
  __O  uint32_t DIR[GPIO_DIR_COUNT];               /**< Port direction, array offset: 0x2000, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_2[120];
  __IO uint32_t MASK[GPIO_MASK_COUNT];             /**< Port mask, array offset: 0x2080, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_3[120];
  __IO uint32_t PIN[GPIO_PIN_COUNT];               /**< Port pin, array offset: 0x2100, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_4[120];
  __IO uint32_t MPIN[GPIO_MPIN_COUNT];             /**< Masked Port Pin, array offset: 0x2180, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_5[120];
  __IO uint32_t SET[GPIO_SET_COUNT];               /**< Port set, array offset: 0x2200, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_6[120];
  __IO uint32_t CLR[GPIO_CLR_COUNT];               /**< Port clear, array offset: 0x2280, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_7[120];
  __O  uint32_t NOT[GPIO_NOT_COUNT];               /**< Port toggle, array offset: 0x2300, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_8[120];
  __O  uint32_t DIRSET[GPIO_DIRSET_COUNT];         /**< Port direction set, array offset: 0x2380, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_9[120];
  __IO uint32_t DIRCLR[GPIO_DIRCLR_COUNT];         /**< Port direction clear, array offset: 0x2400, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_10[120];
  __O  uint32_t DIRNOT[GPIO_DIRNOT_COUNT];         /**< Port direction toggle, array offset: 0x2480, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_11[120];
  __IO uint32_t INTENA[GPIO_INTENA_COUNT];         /**< Interrupt A enable control, array offset: 0x2500, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_12[120];
  __IO uint32_t INTENB[GPIO_INTENB_COUNT];         /**< Interrupt B enable control, array offset: 0x2580, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_13[120];
  __IO uint32_t INTPOL[GPIO_INTPOL_COUNT];         /**< Interupt polarity control, array offset: 0x2600, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_14[120];
  __IO uint32_t INTEDG[GPIO_INTEDG_COUNT];         /**< Interrupt edge select, array offset: 0x2680, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_15[120];
  __IO uint32_t INTSTATA[GPIO_INTSTATA_COUNT];     /**< Interrupt status for interrupt A, array offset: 0x2700, array step: 0x4, irregular array, not all indices are valid */
       uint8_t RESERVED_16[120];
  __IO uint32_t INTSTATB[GPIO_INTSTATB_COUNT];     /**< Interrupt status for interrupt B, array offset: 0x2780, array step: 0x4, irregular array, not all indices are valid */
} GPIO_Type;

/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/*! @name B - Byte pin registers for all port GPIO pins */
/*! @{ */

#define GPIO_B_PBYTE_MASK                        (0x1U)
#define GPIO_B_PBYTE_SHIFT                       (0U)
/*! PBYTE - Port Byte */
#define GPIO_B_PBYTE(x)                          (((uint8_t)(((uint8_t)(x)) << GPIO_B_PBYTE_SHIFT)) & GPIO_B_PBYTE_MASK)
/*! @} */

/* The count of GPIO_B */
#define GPIO_B_COUNT                             (2U)

/* The count of GPIO_B */
#define GPIO_B_COUNT2                            (32U)

/*! @name W - Word pin registers for all port GPIO pins */
/*! @{ */

#define GPIO_W_PWORD_MASK                        (0xFFFFFFFFU)
#define GPIO_W_PWORD_SHIFT                       (0U)
/*! PWORD - PWORD */
#define GPIO_W_PWORD(x)                          (((uint32_t)(((uint32_t)(x)) << GPIO_W_PWORD_SHIFT)) & GPIO_W_PWORD_MASK)
/*! @} */

/* The count of GPIO_W */
#define GPIO_W_COUNT                             (2U)

/* The count of GPIO_W */
#define GPIO_W_COUNT2                            (32U)

/*! @name DIR - Port direction */
/*! @{ */

#define GPIO_DIR_DIRP0_MASK                      (0x1U)
#define GPIO_DIR_DIRP0_SHIFT                     (0U)
/*! DIRP0 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP0(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP0_SHIFT)) & GPIO_DIR_DIRP0_MASK)

#define GPIO_DIR_DIRP1_MASK                      (0x2U)
#define GPIO_DIR_DIRP1_SHIFT                     (1U)
/*! DIRP1 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP1(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP1_SHIFT)) & GPIO_DIR_DIRP1_MASK)

#define GPIO_DIR_DIRP2_MASK                      (0x4U)
#define GPIO_DIR_DIRP2_SHIFT                     (2U)
/*! DIRP2 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP2(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP2_SHIFT)) & GPIO_DIR_DIRP2_MASK)

#define GPIO_DIR_DIRP3_MASK                      (0x8U)
#define GPIO_DIR_DIRP3_SHIFT                     (3U)
/*! DIRP3 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP3(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP3_SHIFT)) & GPIO_DIR_DIRP3_MASK)

#define GPIO_DIR_DIRP4_MASK                      (0x10U)
#define GPIO_DIR_DIRP4_SHIFT                     (4U)
/*! DIRP4 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP4(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP4_SHIFT)) & GPIO_DIR_DIRP4_MASK)

#define GPIO_DIR_DIRP5_MASK                      (0x20U)
#define GPIO_DIR_DIRP5_SHIFT                     (5U)
/*! DIRP5 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP5(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP5_SHIFT)) & GPIO_DIR_DIRP5_MASK)

#define GPIO_DIR_DIRP6_MASK                      (0x40U)
#define GPIO_DIR_DIRP6_SHIFT                     (6U)
/*! DIRP6 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP6(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP6_SHIFT)) & GPIO_DIR_DIRP6_MASK)

#define GPIO_DIR_DIRP7_MASK                      (0x80U)
#define GPIO_DIR_DIRP7_SHIFT                     (7U)
/*! DIRP7 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP7(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP7_SHIFT)) & GPIO_DIR_DIRP7_MASK)

#define GPIO_DIR_DIRP8_MASK                      (0x100U)
#define GPIO_DIR_DIRP8_SHIFT                     (8U)
/*! DIRP8 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP8(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP8_SHIFT)) & GPIO_DIR_DIRP8_MASK)

#define GPIO_DIR_DIRP9_MASK                      (0x200U)
#define GPIO_DIR_DIRP9_SHIFT                     (9U)
/*! DIRP9 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP9(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP9_SHIFT)) & GPIO_DIR_DIRP9_MASK)

#define GPIO_DIR_DIRP10_MASK                     (0x400U)
#define GPIO_DIR_DIRP10_SHIFT                    (10U)
/*! DIRP10 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP10(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP10_SHIFT)) & GPIO_DIR_DIRP10_MASK)

#define GPIO_DIR_DIRP11_MASK                     (0x800U)
#define GPIO_DIR_DIRP11_SHIFT                    (11U)
/*! DIRP11 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP11(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP11_SHIFT)) & GPIO_DIR_DIRP11_MASK)

#define GPIO_DIR_DIRP12_MASK                     (0x1000U)
#define GPIO_DIR_DIRP12_SHIFT                    (12U)
/*! DIRP12 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP12(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP12_SHIFT)) & GPIO_DIR_DIRP12_MASK)

#define GPIO_DIR_DIRP13_MASK                     (0x2000U)
#define GPIO_DIR_DIRP13_SHIFT                    (13U)
/*! DIRP13 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP13(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP13_SHIFT)) & GPIO_DIR_DIRP13_MASK)

#define GPIO_DIR_DIRP14_MASK                     (0x4000U)
#define GPIO_DIR_DIRP14_SHIFT                    (14U)
/*! DIRP14 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP14(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP14_SHIFT)) & GPIO_DIR_DIRP14_MASK)

#define GPIO_DIR_DIRP15_MASK                     (0x8000U)
#define GPIO_DIR_DIRP15_SHIFT                    (15U)
/*! DIRP15 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP15(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP15_SHIFT)) & GPIO_DIR_DIRP15_MASK)

#define GPIO_DIR_DIRP16_MASK                     (0x10000U)
#define GPIO_DIR_DIRP16_SHIFT                    (16U)
/*! DIRP16 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP16(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP16_SHIFT)) & GPIO_DIR_DIRP16_MASK)

#define GPIO_DIR_DIRP17_MASK                     (0x20000U)
#define GPIO_DIR_DIRP17_SHIFT                    (17U)
/*! DIRP17 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP17(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP17_SHIFT)) & GPIO_DIR_DIRP17_MASK)

#define GPIO_DIR_DIRP18_MASK                     (0x40000U)
#define GPIO_DIR_DIRP18_SHIFT                    (18U)
/*! DIRP18 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP18(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP18_SHIFT)) & GPIO_DIR_DIRP18_MASK)

#define GPIO_DIR_DIRP19_MASK                     (0x80000U)
#define GPIO_DIR_DIRP19_SHIFT                    (19U)
/*! DIRP19 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP19(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP19_SHIFT)) & GPIO_DIR_DIRP19_MASK)

#define GPIO_DIR_DIRP20_MASK                     (0x100000U)
#define GPIO_DIR_DIRP20_SHIFT                    (20U)
/*! DIRP20 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP20(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP20_SHIFT)) & GPIO_DIR_DIRP20_MASK)

#define GPIO_DIR_DIRP21_MASK                     (0x200000U)
#define GPIO_DIR_DIRP21_SHIFT                    (21U)
/*! DIRP21 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP21(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP21_SHIFT)) & GPIO_DIR_DIRP21_MASK)

#define GPIO_DIR_DIRP22_MASK                     (0x400000U)
#define GPIO_DIR_DIRP22_SHIFT                    (22U)
/*! DIRP22 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP22(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP22_SHIFT)) & GPIO_DIR_DIRP22_MASK)

#define GPIO_DIR_DIRP23_MASK                     (0x800000U)
#define GPIO_DIR_DIRP23_SHIFT                    (23U)
/*! DIRP23 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP23(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP23_SHIFT)) & GPIO_DIR_DIRP23_MASK)

#define GPIO_DIR_DIRP24_MASK                     (0x1000000U)
#define GPIO_DIR_DIRP24_SHIFT                    (24U)
/*! DIRP24 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP24(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP24_SHIFT)) & GPIO_DIR_DIRP24_MASK)

#define GPIO_DIR_DIRP25_MASK                     (0x2000000U)
#define GPIO_DIR_DIRP25_SHIFT                    (25U)
/*! DIRP25 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP25(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP25_SHIFT)) & GPIO_DIR_DIRP25_MASK)

#define GPIO_DIR_DIRP26_MASK                     (0x4000000U)
#define GPIO_DIR_DIRP26_SHIFT                    (26U)
/*! DIRP26 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP26(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP26_SHIFT)) & GPIO_DIR_DIRP26_MASK)

#define GPIO_DIR_DIRP27_MASK                     (0x8000000U)
#define GPIO_DIR_DIRP27_SHIFT                    (27U)
/*! DIRP27 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP27(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP27_SHIFT)) & GPIO_DIR_DIRP27_MASK)

#define GPIO_DIR_DIRP28_MASK                     (0x10000000U)
#define GPIO_DIR_DIRP28_SHIFT                    (28U)
/*! DIRP28 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP28(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP28_SHIFT)) & GPIO_DIR_DIRP28_MASK)

#define GPIO_DIR_DIRP29_MASK                     (0x20000000U)
#define GPIO_DIR_DIRP29_SHIFT                    (29U)
/*! DIRP29 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP29(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP29_SHIFT)) & GPIO_DIR_DIRP29_MASK)

#define GPIO_DIR_DIRP30_MASK                     (0x40000000U)
#define GPIO_DIR_DIRP30_SHIFT                    (30U)
/*! DIRP30 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP30(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP30_SHIFT)) & GPIO_DIR_DIRP30_MASK)

#define GPIO_DIR_DIRP31_MASK                     (0x80000000U)
#define GPIO_DIR_DIRP31_SHIFT                    (31U)
/*! DIRP31 - Selects pin direction for pin PIOa_b.
 *  0b0..Input
 *  0b1..Output
 */
#define GPIO_DIR_DIRP31(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_DIR_DIRP31_SHIFT)) & GPIO_DIR_DIRP31_MASK)
/*! @} */

/*! @name MASK - Port mask */
/*! @{ */

#define GPIO_MASK_MASKP0_MASK                    (0x1U)
#define GPIO_MASK_MASKP0_SHIFT                   (0U)
/*! MASKP0 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP0(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP0_SHIFT)) & GPIO_MASK_MASKP0_MASK)

#define GPIO_MASK_MASKP1_MASK                    (0x2U)
#define GPIO_MASK_MASKP1_SHIFT                   (1U)
/*! MASKP1 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP1(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP1_SHIFT)) & GPIO_MASK_MASKP1_MASK)

#define GPIO_MASK_MASKP2_MASK                    (0x4U)
#define GPIO_MASK_MASKP2_SHIFT                   (2U)
/*! MASKP2 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP2(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP2_SHIFT)) & GPIO_MASK_MASKP2_MASK)

#define GPIO_MASK_MASKP3_MASK                    (0x8U)
#define GPIO_MASK_MASKP3_SHIFT                   (3U)
/*! MASKP3 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP3(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP3_SHIFT)) & GPIO_MASK_MASKP3_MASK)

#define GPIO_MASK_MASKP4_MASK                    (0x10U)
#define GPIO_MASK_MASKP4_SHIFT                   (4U)
/*! MASKP4 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP4(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP4_SHIFT)) & GPIO_MASK_MASKP4_MASK)

#define GPIO_MASK_MASKP5_MASK                    (0x20U)
#define GPIO_MASK_MASKP5_SHIFT                   (5U)
/*! MASKP5 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP5(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP5_SHIFT)) & GPIO_MASK_MASKP5_MASK)

#define GPIO_MASK_MASKP6_MASK                    (0x40U)
#define GPIO_MASK_MASKP6_SHIFT                   (6U)
/*! MASKP6 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP6(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP6_SHIFT)) & GPIO_MASK_MASKP6_MASK)

#define GPIO_MASK_MASKP7_MASK                    (0x80U)
#define GPIO_MASK_MASKP7_SHIFT                   (7U)
/*! MASKP7 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP7(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP7_SHIFT)) & GPIO_MASK_MASKP7_MASK)

#define GPIO_MASK_MASKP8_MASK                    (0x100U)
#define GPIO_MASK_MASKP8_SHIFT                   (8U)
/*! MASKP8 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP8(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP8_SHIFT)) & GPIO_MASK_MASKP8_MASK)

#define GPIO_MASK_MASKP9_MASK                    (0x200U)
#define GPIO_MASK_MASKP9_SHIFT                   (9U)
/*! MASKP9 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP9(x)                      (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP9_SHIFT)) & GPIO_MASK_MASKP9_MASK)

#define GPIO_MASK_MASKP10_MASK                   (0x400U)
#define GPIO_MASK_MASKP10_SHIFT                  (10U)
/*! MASKP10 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP10(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP10_SHIFT)) & GPIO_MASK_MASKP10_MASK)

#define GPIO_MASK_MASKP11_MASK                   (0x800U)
#define GPIO_MASK_MASKP11_SHIFT                  (11U)
/*! MASKP11 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP11(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP11_SHIFT)) & GPIO_MASK_MASKP11_MASK)

#define GPIO_MASK_MASKP12_MASK                   (0x1000U)
#define GPIO_MASK_MASKP12_SHIFT                  (12U)
/*! MASKP12 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP12(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP12_SHIFT)) & GPIO_MASK_MASKP12_MASK)

#define GPIO_MASK_MASKP13_MASK                   (0x2000U)
#define GPIO_MASK_MASKP13_SHIFT                  (13U)
/*! MASKP13 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP13(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP13_SHIFT)) & GPIO_MASK_MASKP13_MASK)

#define GPIO_MASK_MASKP14_MASK                   (0x4000U)
#define GPIO_MASK_MASKP14_SHIFT                  (14U)
/*! MASKP14 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP14(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP14_SHIFT)) & GPIO_MASK_MASKP14_MASK)

#define GPIO_MASK_MASKP15_MASK                   (0x8000U)
#define GPIO_MASK_MASKP15_SHIFT                  (15U)
/*! MASKP15 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP15(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP15_SHIFT)) & GPIO_MASK_MASKP15_MASK)

#define GPIO_MASK_MASKP16_MASK                   (0x10000U)
#define GPIO_MASK_MASKP16_SHIFT                  (16U)
/*! MASKP16 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP16(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP16_SHIFT)) & GPIO_MASK_MASKP16_MASK)

#define GPIO_MASK_MASKP17_MASK                   (0x20000U)
#define GPIO_MASK_MASKP17_SHIFT                  (17U)
/*! MASKP17 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP17(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP17_SHIFT)) & GPIO_MASK_MASKP17_MASK)

#define GPIO_MASK_MASKP18_MASK                   (0x40000U)
#define GPIO_MASK_MASKP18_SHIFT                  (18U)
/*! MASKP18 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP18(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP18_SHIFT)) & GPIO_MASK_MASKP18_MASK)

#define GPIO_MASK_MASKP19_MASK                   (0x80000U)
#define GPIO_MASK_MASKP19_SHIFT                  (19U)
/*! MASKP19 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP19(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP19_SHIFT)) & GPIO_MASK_MASKP19_MASK)

#define GPIO_MASK_MASKP20_MASK                   (0x100000U)
#define GPIO_MASK_MASKP20_SHIFT                  (20U)
/*! MASKP20 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP20(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP20_SHIFT)) & GPIO_MASK_MASKP20_MASK)

#define GPIO_MASK_MASKP21_MASK                   (0x200000U)
#define GPIO_MASK_MASKP21_SHIFT                  (21U)
/*! MASKP21 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP21(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP21_SHIFT)) & GPIO_MASK_MASKP21_MASK)

#define GPIO_MASK_MASKP22_MASK                   (0x400000U)
#define GPIO_MASK_MASKP22_SHIFT                  (22U)
/*! MASKP22 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP22(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP22_SHIFT)) & GPIO_MASK_MASKP22_MASK)

#define GPIO_MASK_MASKP23_MASK                   (0x800000U)
#define GPIO_MASK_MASKP23_SHIFT                  (23U)
/*! MASKP23 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP23(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP23_SHIFT)) & GPIO_MASK_MASKP23_MASK)

#define GPIO_MASK_MASKP24_MASK                   (0x1000000U)
#define GPIO_MASK_MASKP24_SHIFT                  (24U)
/*! MASKP24 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP24(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP24_SHIFT)) & GPIO_MASK_MASKP24_MASK)

#define GPIO_MASK_MASKP25_MASK                   (0x2000000U)
#define GPIO_MASK_MASKP25_SHIFT                  (25U)
/*! MASKP25 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP25(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP25_SHIFT)) & GPIO_MASK_MASKP25_MASK)

#define GPIO_MASK_MASKP26_MASK                   (0x4000000U)
#define GPIO_MASK_MASKP26_SHIFT                  (26U)
/*! MASKP26 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP26(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP26_SHIFT)) & GPIO_MASK_MASKP26_MASK)

#define GPIO_MASK_MASKP27_MASK                   (0x8000000U)
#define GPIO_MASK_MASKP27_SHIFT                  (27U)
/*! MASKP27 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP27(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP27_SHIFT)) & GPIO_MASK_MASKP27_MASK)

#define GPIO_MASK_MASKP28_MASK                   (0x10000000U)
#define GPIO_MASK_MASKP28_SHIFT                  (28U)
/*! MASKP28 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP28(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP28_SHIFT)) & GPIO_MASK_MASKP28_MASK)

#define GPIO_MASK_MASKP29_MASK                   (0x20000000U)
#define GPIO_MASK_MASKP29_SHIFT                  (29U)
/*! MASKP29 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP29(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP29_SHIFT)) & GPIO_MASK_MASKP29_MASK)

#define GPIO_MASK_MASKP30_MASK                   (0x40000000U)
#define GPIO_MASK_MASKP30_SHIFT                  (30U)
/*! MASKP30 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP30(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP30_SHIFT)) & GPIO_MASK_MASKP30_MASK)

#define GPIO_MASK_MASKP31_MASK                   (0x80000000U)
#define GPIO_MASK_MASKP31_SHIFT                  (31U)
/*! MASKP31 - Port Mask
 *  0b0..Read MPIN: pin state; write MPIN: load output bit
 *  0b1..Read MPIN: 0; write MPIN: output bit not affected
 */
#define GPIO_MASK_MASKP31(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MASK_MASKP31_SHIFT)) & GPIO_MASK_MASKP31_MASK)
/*! @} */

/*! @name PIN - Port pin */
/*! @{ */

#define GPIO_PIN_PORT0_MASK                      (0x1U)
#define GPIO_PIN_PORT0_SHIFT                     (0U)
/*! PORT0 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT0(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT0_SHIFT)) & GPIO_PIN_PORT0_MASK)

#define GPIO_PIN_PORT1_MASK                      (0x2U)
#define GPIO_PIN_PORT1_SHIFT                     (1U)
/*! PORT1 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT1(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT1_SHIFT)) & GPIO_PIN_PORT1_MASK)

#define GPIO_PIN_PORT2_MASK                      (0x4U)
#define GPIO_PIN_PORT2_SHIFT                     (2U)
/*! PORT2 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT2(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT2_SHIFT)) & GPIO_PIN_PORT2_MASK)

#define GPIO_PIN_PORT3_MASK                      (0x8U)
#define GPIO_PIN_PORT3_SHIFT                     (3U)
/*! PORT3 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT3(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT3_SHIFT)) & GPIO_PIN_PORT3_MASK)

#define GPIO_PIN_PORT4_MASK                      (0x10U)
#define GPIO_PIN_PORT4_SHIFT                     (4U)
/*! PORT4 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT4(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT4_SHIFT)) & GPIO_PIN_PORT4_MASK)

#define GPIO_PIN_PORT5_MASK                      (0x20U)
#define GPIO_PIN_PORT5_SHIFT                     (5U)
/*! PORT5 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT5(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT5_SHIFT)) & GPIO_PIN_PORT5_MASK)

#define GPIO_PIN_PORT6_MASK                      (0x40U)
#define GPIO_PIN_PORT6_SHIFT                     (6U)
/*! PORT6 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT6(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT6_SHIFT)) & GPIO_PIN_PORT6_MASK)

#define GPIO_PIN_PORT7_MASK                      (0x80U)
#define GPIO_PIN_PORT7_SHIFT                     (7U)
/*! PORT7 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT7(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT7_SHIFT)) & GPIO_PIN_PORT7_MASK)

#define GPIO_PIN_PORT8_MASK                      (0x100U)
#define GPIO_PIN_PORT8_SHIFT                     (8U)
/*! PORT8 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT8(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT8_SHIFT)) & GPIO_PIN_PORT8_MASK)

#define GPIO_PIN_PORT9_MASK                      (0x200U)
#define GPIO_PIN_PORT9_SHIFT                     (9U)
/*! PORT9 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT9(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT9_SHIFT)) & GPIO_PIN_PORT9_MASK)

#define GPIO_PIN_PORT10_MASK                     (0x400U)
#define GPIO_PIN_PORT10_SHIFT                    (10U)
/*! PORT10 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT10(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT10_SHIFT)) & GPIO_PIN_PORT10_MASK)

#define GPIO_PIN_PORT11_MASK                     (0x800U)
#define GPIO_PIN_PORT11_SHIFT                    (11U)
/*! PORT11 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT11(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT11_SHIFT)) & GPIO_PIN_PORT11_MASK)

#define GPIO_PIN_PORT12_MASK                     (0x1000U)
#define GPIO_PIN_PORT12_SHIFT                    (12U)
/*! PORT12 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT12(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT12_SHIFT)) & GPIO_PIN_PORT12_MASK)

#define GPIO_PIN_PORT13_MASK                     (0x2000U)
#define GPIO_PIN_PORT13_SHIFT                    (13U)
/*! PORT13 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT13(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT13_SHIFT)) & GPIO_PIN_PORT13_MASK)

#define GPIO_PIN_PORT14_MASK                     (0x4000U)
#define GPIO_PIN_PORT14_SHIFT                    (14U)
/*! PORT14 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT14(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT14_SHIFT)) & GPIO_PIN_PORT14_MASK)

#define GPIO_PIN_PORT15_MASK                     (0x8000U)
#define GPIO_PIN_PORT15_SHIFT                    (15U)
/*! PORT15 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT15(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT15_SHIFT)) & GPIO_PIN_PORT15_MASK)

#define GPIO_PIN_PORT16_MASK                     (0x10000U)
#define GPIO_PIN_PORT16_SHIFT                    (16U)
/*! PORT16 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT16(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT16_SHIFT)) & GPIO_PIN_PORT16_MASK)

#define GPIO_PIN_PORT17_MASK                     (0x20000U)
#define GPIO_PIN_PORT17_SHIFT                    (17U)
/*! PORT17 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT17(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT17_SHIFT)) & GPIO_PIN_PORT17_MASK)

#define GPIO_PIN_PORT18_MASK                     (0x40000U)
#define GPIO_PIN_PORT18_SHIFT                    (18U)
/*! PORT18 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT18(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT18_SHIFT)) & GPIO_PIN_PORT18_MASK)

#define GPIO_PIN_PORT19_MASK                     (0x80000U)
#define GPIO_PIN_PORT19_SHIFT                    (19U)
/*! PORT19 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT19(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT19_SHIFT)) & GPIO_PIN_PORT19_MASK)

#define GPIO_PIN_PORT20_MASK                     (0x100000U)
#define GPIO_PIN_PORT20_SHIFT                    (20U)
/*! PORT20 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT20(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT20_SHIFT)) & GPIO_PIN_PORT20_MASK)

#define GPIO_PIN_PORT21_MASK                     (0x200000U)
#define GPIO_PIN_PORT21_SHIFT                    (21U)
/*! PORT21 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT21(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT21_SHIFT)) & GPIO_PIN_PORT21_MASK)

#define GPIO_PIN_PORT22_MASK                     (0x400000U)
#define GPIO_PIN_PORT22_SHIFT                    (22U)
/*! PORT22 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT22(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT22_SHIFT)) & GPIO_PIN_PORT22_MASK)

#define GPIO_PIN_PORT23_MASK                     (0x800000U)
#define GPIO_PIN_PORT23_SHIFT                    (23U)
/*! PORT23 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT23(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT23_SHIFT)) & GPIO_PIN_PORT23_MASK)

#define GPIO_PIN_PORT24_MASK                     (0x1000000U)
#define GPIO_PIN_PORT24_SHIFT                    (24U)
/*! PORT24 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT24(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT24_SHIFT)) & GPIO_PIN_PORT24_MASK)

#define GPIO_PIN_PORT25_MASK                     (0x2000000U)
#define GPIO_PIN_PORT25_SHIFT                    (25U)
/*! PORT25 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT25(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT25_SHIFT)) & GPIO_PIN_PORT25_MASK)

#define GPIO_PIN_PORT26_MASK                     (0x4000000U)
#define GPIO_PIN_PORT26_SHIFT                    (26U)
/*! PORT26 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT26(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT26_SHIFT)) & GPIO_PIN_PORT26_MASK)

#define GPIO_PIN_PORT27_MASK                     (0x8000000U)
#define GPIO_PIN_PORT27_SHIFT                    (27U)
/*! PORT27 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT27(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT27_SHIFT)) & GPIO_PIN_PORT27_MASK)

#define GPIO_PIN_PORT28_MASK                     (0x10000000U)
#define GPIO_PIN_PORT28_SHIFT                    (28U)
/*! PORT28 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT28(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT28_SHIFT)) & GPIO_PIN_PORT28_MASK)

#define GPIO_PIN_PORT29_MASK                     (0x20000000U)
#define GPIO_PIN_PORT29_SHIFT                    (29U)
/*! PORT29 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT29(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT29_SHIFT)) & GPIO_PIN_PORT29_MASK)

#define GPIO_PIN_PORT30_MASK                     (0x40000000U)
#define GPIO_PIN_PORT30_SHIFT                    (30U)
/*! PORT30 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT30(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT30_SHIFT)) & GPIO_PIN_PORT30_MASK)

#define GPIO_PIN_PORT31_MASK                     (0x80000000U)
#define GPIO_PIN_PORT31_SHIFT                    (31U)
/*! PORT31 - Port pins
 *  0b0..Read- pin is low; Write- clear output bit
 *  0b1..Read- pin is high; Write- set output bit
 */
#define GPIO_PIN_PORT31(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_PIN_PORT31_SHIFT)) & GPIO_PIN_PORT31_MASK)
/*! @} */

/*! @name MPIN - Masked Port Pin */
/*! @{ */

#define GPIO_MPIN_MPORTP0_MASK                   (0x1U)
#define GPIO_MPIN_MPORTP0_SHIFT                  (0U)
/*! MPORTP0 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP0(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP0_SHIFT)) & GPIO_MPIN_MPORTP0_MASK)

#define GPIO_MPIN_MPORTP1_MASK                   (0x2U)
#define GPIO_MPIN_MPORTP1_SHIFT                  (1U)
/*! MPORTP1 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP1(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP1_SHIFT)) & GPIO_MPIN_MPORTP1_MASK)

#define GPIO_MPIN_MPORTP2_MASK                   (0x4U)
#define GPIO_MPIN_MPORTP2_SHIFT                  (2U)
/*! MPORTP2 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP2(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP2_SHIFT)) & GPIO_MPIN_MPORTP2_MASK)

#define GPIO_MPIN_MPORTP3_MASK                   (0x8U)
#define GPIO_MPIN_MPORTP3_SHIFT                  (3U)
/*! MPORTP3 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP3(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP3_SHIFT)) & GPIO_MPIN_MPORTP3_MASK)

#define GPIO_MPIN_MPORTP4_MASK                   (0x10U)
#define GPIO_MPIN_MPORTP4_SHIFT                  (4U)
/*! MPORTP4 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP4(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP4_SHIFT)) & GPIO_MPIN_MPORTP4_MASK)

#define GPIO_MPIN_MPORTP5_MASK                   (0x20U)
#define GPIO_MPIN_MPORTP5_SHIFT                  (5U)
/*! MPORTP5 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP5(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP5_SHIFT)) & GPIO_MPIN_MPORTP5_MASK)

#define GPIO_MPIN_MPORTP6_MASK                   (0x40U)
#define GPIO_MPIN_MPORTP6_SHIFT                  (6U)
/*! MPORTP6 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP6(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP6_SHIFT)) & GPIO_MPIN_MPORTP6_MASK)

#define GPIO_MPIN_MPORTP7_MASK                   (0x80U)
#define GPIO_MPIN_MPORTP7_SHIFT                  (7U)
/*! MPORTP7 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP7(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP7_SHIFT)) & GPIO_MPIN_MPORTP7_MASK)

#define GPIO_MPIN_MPORTP8_MASK                   (0x100U)
#define GPIO_MPIN_MPORTP8_SHIFT                  (8U)
/*! MPORTP8 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP8(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP8_SHIFT)) & GPIO_MPIN_MPORTP8_MASK)

#define GPIO_MPIN_MPORTP9_MASK                   (0x200U)
#define GPIO_MPIN_MPORTP9_SHIFT                  (9U)
/*! MPORTP9 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP9(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP9_SHIFT)) & GPIO_MPIN_MPORTP9_MASK)

#define GPIO_MPIN_MPORTP10_MASK                  (0x400U)
#define GPIO_MPIN_MPORTP10_SHIFT                 (10U)
/*! MPORTP10 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP10(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP10_SHIFT)) & GPIO_MPIN_MPORTP10_MASK)

#define GPIO_MPIN_MPORTP11_MASK                  (0x800U)
#define GPIO_MPIN_MPORTP11_SHIFT                 (11U)
/*! MPORTP11 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP11(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP11_SHIFT)) & GPIO_MPIN_MPORTP11_MASK)

#define GPIO_MPIN_MPORTP12_MASK                  (0x1000U)
#define GPIO_MPIN_MPORTP12_SHIFT                 (12U)
/*! MPORTP12 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP12(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP12_SHIFT)) & GPIO_MPIN_MPORTP12_MASK)

#define GPIO_MPIN_MPORTP13_MASK                  (0x2000U)
#define GPIO_MPIN_MPORTP13_SHIFT                 (13U)
/*! MPORTP13 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP13(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP13_SHIFT)) & GPIO_MPIN_MPORTP13_MASK)

#define GPIO_MPIN_MPORTP14_MASK                  (0x4000U)
#define GPIO_MPIN_MPORTP14_SHIFT                 (14U)
/*! MPORTP14 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP14(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP14_SHIFT)) & GPIO_MPIN_MPORTP14_MASK)

#define GPIO_MPIN_MPORTP15_MASK                  (0x8000U)
#define GPIO_MPIN_MPORTP15_SHIFT                 (15U)
/*! MPORTP15 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP15(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP15_SHIFT)) & GPIO_MPIN_MPORTP15_MASK)

#define GPIO_MPIN_MPORTP16_MASK                  (0x10000U)
#define GPIO_MPIN_MPORTP16_SHIFT                 (16U)
/*! MPORTP16 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP16(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP16_SHIFT)) & GPIO_MPIN_MPORTP16_MASK)

#define GPIO_MPIN_MPORTP17_MASK                  (0x20000U)
#define GPIO_MPIN_MPORTP17_SHIFT                 (17U)
/*! MPORTP17 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP17(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP17_SHIFT)) & GPIO_MPIN_MPORTP17_MASK)

#define GPIO_MPIN_MPORTP18_MASK                  (0x40000U)
#define GPIO_MPIN_MPORTP18_SHIFT                 (18U)
/*! MPORTP18 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP18(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP18_SHIFT)) & GPIO_MPIN_MPORTP18_MASK)

#define GPIO_MPIN_MPORTP19_MASK                  (0x80000U)
#define GPIO_MPIN_MPORTP19_SHIFT                 (19U)
/*! MPORTP19 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP19(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP19_SHIFT)) & GPIO_MPIN_MPORTP19_MASK)

#define GPIO_MPIN_MPORTP20_MASK                  (0x100000U)
#define GPIO_MPIN_MPORTP20_SHIFT                 (20U)
/*! MPORTP20 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP20(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP20_SHIFT)) & GPIO_MPIN_MPORTP20_MASK)

#define GPIO_MPIN_MPORTP21_MASK                  (0x200000U)
#define GPIO_MPIN_MPORTP21_SHIFT                 (21U)
/*! MPORTP21 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP21(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP21_SHIFT)) & GPIO_MPIN_MPORTP21_MASK)

#define GPIO_MPIN_MPORTP22_MASK                  (0x400000U)
#define GPIO_MPIN_MPORTP22_SHIFT                 (22U)
/*! MPORTP22 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP22(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP22_SHIFT)) & GPIO_MPIN_MPORTP22_MASK)

#define GPIO_MPIN_MPORTP23_MASK                  (0x800000U)
#define GPIO_MPIN_MPORTP23_SHIFT                 (23U)
/*! MPORTP23 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP23(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP23_SHIFT)) & GPIO_MPIN_MPORTP23_MASK)

#define GPIO_MPIN_MPORTP24_MASK                  (0x1000000U)
#define GPIO_MPIN_MPORTP24_SHIFT                 (24U)
/*! MPORTP24 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP24(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP24_SHIFT)) & GPIO_MPIN_MPORTP24_MASK)

#define GPIO_MPIN_MPORTP25_MASK                  (0x2000000U)
#define GPIO_MPIN_MPORTP25_SHIFT                 (25U)
/*! MPORTP25 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP25(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP25_SHIFT)) & GPIO_MPIN_MPORTP25_MASK)

#define GPIO_MPIN_MPORTP26_MASK                  (0x4000000U)
#define GPIO_MPIN_MPORTP26_SHIFT                 (26U)
/*! MPORTP26 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP26(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP26_SHIFT)) & GPIO_MPIN_MPORTP26_MASK)

#define GPIO_MPIN_MPORTP27_MASK                  (0x8000000U)
#define GPIO_MPIN_MPORTP27_SHIFT                 (27U)
/*! MPORTP27 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP27(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP27_SHIFT)) & GPIO_MPIN_MPORTP27_MASK)

#define GPIO_MPIN_MPORTP28_MASK                  (0x10000000U)
#define GPIO_MPIN_MPORTP28_SHIFT                 (28U)
/*! MPORTP28 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP28(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP28_SHIFT)) & GPIO_MPIN_MPORTP28_MASK)

#define GPIO_MPIN_MPORTP29_MASK                  (0x20000000U)
#define GPIO_MPIN_MPORTP29_SHIFT                 (29U)
/*! MPORTP29 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP29(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP29_SHIFT)) & GPIO_MPIN_MPORTP29_MASK)

#define GPIO_MPIN_MPORTP30_MASK                  (0x40000000U)
#define GPIO_MPIN_MPORTP30_SHIFT                 (30U)
/*! MPORTP30 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP30(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP30_SHIFT)) & GPIO_MPIN_MPORTP30_MASK)

#define GPIO_MPIN_MPORTP31_MASK                  (0x80000000U)
#define GPIO_MPIN_MPORTP31_SHIFT                 (31U)
/*! MPORTP31 - Mask bits for port pins
 *  0b0..Read- pin is LOW and/or the corresponding bit in the MASK register is 1; write- clear output bit if the
 *       corresponding bit in the MASK register is 0
 *  0b1..Read- pin is HIGH and the corresponding bit in the MASK register is 0; write- set output bit if the
 *       corresponding bit in the MASK register is 0
 */
#define GPIO_MPIN_MPORTP31(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_MPIN_MPORTP31_SHIFT)) & GPIO_MPIN_MPORTP31_MASK)
/*! @} */

/*! @name SET - Port set */
/*! @{ */

#define GPIO_SET_SETP_MASK                       (0xFFFFFFFFU)
#define GPIO_SET_SETP_SHIFT                      (0U)
/*! SETP - Read or set output bits
 *  0b00000000000000000000000000000000..Read- output bit; write- no operation
 *  0b00000000000000000000000000000001..Read- output bit; write- set output bit
 */
#define GPIO_SET_SETP(x)                         (((uint32_t)(((uint32_t)(x)) << GPIO_SET_SETP_SHIFT)) & GPIO_SET_SETP_MASK)
/*! @} */

/*! @name CLR - Port clear */
/*! @{ */

#define GPIO_CLR_CLRP0_MASK                      (0x1U)
#define GPIO_CLR_CLRP0_SHIFT                     (0U)
/*! CLRP0 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP0(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP0_SHIFT)) & GPIO_CLR_CLRP0_MASK)

#define GPIO_CLR_CLRP1_MASK                      (0x2U)
#define GPIO_CLR_CLRP1_SHIFT                     (1U)
/*! CLRP1 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP1(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP1_SHIFT)) & GPIO_CLR_CLRP1_MASK)

#define GPIO_CLR_CLRP2_MASK                      (0x4U)
#define GPIO_CLR_CLRP2_SHIFT                     (2U)
/*! CLRP2 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP2(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP2_SHIFT)) & GPIO_CLR_CLRP2_MASK)

#define GPIO_CLR_CLRP3_MASK                      (0x8U)
#define GPIO_CLR_CLRP3_SHIFT                     (3U)
/*! CLRP3 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP3(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP3_SHIFT)) & GPIO_CLR_CLRP3_MASK)

#define GPIO_CLR_CLRP4_MASK                      (0x10U)
#define GPIO_CLR_CLRP4_SHIFT                     (4U)
/*! CLRP4 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP4(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP4_SHIFT)) & GPIO_CLR_CLRP4_MASK)

#define GPIO_CLR_CLRP5_MASK                      (0x20U)
#define GPIO_CLR_CLRP5_SHIFT                     (5U)
/*! CLRP5 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP5(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP5_SHIFT)) & GPIO_CLR_CLRP5_MASK)

#define GPIO_CLR_CLRP6_MASK                      (0x40U)
#define GPIO_CLR_CLRP6_SHIFT                     (6U)
/*! CLRP6 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP6(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP6_SHIFT)) & GPIO_CLR_CLRP6_MASK)

#define GPIO_CLR_CLRP7_MASK                      (0x80U)
#define GPIO_CLR_CLRP7_SHIFT                     (7U)
/*! CLRP7 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP7(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP7_SHIFT)) & GPIO_CLR_CLRP7_MASK)

#define GPIO_CLR_CLRP8_MASK                      (0x100U)
#define GPIO_CLR_CLRP8_SHIFT                     (8U)
/*! CLRP8 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP8(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP8_SHIFT)) & GPIO_CLR_CLRP8_MASK)

#define GPIO_CLR_CLRP9_MASK                      (0x200U)
#define GPIO_CLR_CLRP9_SHIFT                     (9U)
/*! CLRP9 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP9(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP9_SHIFT)) & GPIO_CLR_CLRP9_MASK)

#define GPIO_CLR_CLRP10_MASK                     (0x400U)
#define GPIO_CLR_CLRP10_SHIFT                    (10U)
/*! CLRP10 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP10(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP10_SHIFT)) & GPIO_CLR_CLRP10_MASK)

#define GPIO_CLR_CLRP11_MASK                     (0x800U)
#define GPIO_CLR_CLRP11_SHIFT                    (11U)
/*! CLRP11 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP11(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP11_SHIFT)) & GPIO_CLR_CLRP11_MASK)

#define GPIO_CLR_CLRP12_MASK                     (0x1000U)
#define GPIO_CLR_CLRP12_SHIFT                    (12U)
/*! CLRP12 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP12(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP12_SHIFT)) & GPIO_CLR_CLRP12_MASK)

#define GPIO_CLR_CLRP13_MASK                     (0x2000U)
#define GPIO_CLR_CLRP13_SHIFT                    (13U)
/*! CLRP13 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP13(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP13_SHIFT)) & GPIO_CLR_CLRP13_MASK)

#define GPIO_CLR_CLRP14_MASK                     (0x4000U)
#define GPIO_CLR_CLRP14_SHIFT                    (14U)
/*! CLRP14 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP14(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP14_SHIFT)) & GPIO_CLR_CLRP14_MASK)

#define GPIO_CLR_CLRP15_MASK                     (0x8000U)
#define GPIO_CLR_CLRP15_SHIFT                    (15U)
/*! CLRP15 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP15(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP15_SHIFT)) & GPIO_CLR_CLRP15_MASK)

#define GPIO_CLR_CLRP16_MASK                     (0x10000U)
#define GPIO_CLR_CLRP16_SHIFT                    (16U)
/*! CLRP16 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP16(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP16_SHIFT)) & GPIO_CLR_CLRP16_MASK)

#define GPIO_CLR_CLRP17_MASK                     (0x20000U)
#define GPIO_CLR_CLRP17_SHIFT                    (17U)
/*! CLRP17 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP17(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP17_SHIFT)) & GPIO_CLR_CLRP17_MASK)

#define GPIO_CLR_CLRP18_MASK                     (0x40000U)
#define GPIO_CLR_CLRP18_SHIFT                    (18U)
/*! CLRP18 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP18(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP18_SHIFT)) & GPIO_CLR_CLRP18_MASK)

#define GPIO_CLR_CLRP19_MASK                     (0x80000U)
#define GPIO_CLR_CLRP19_SHIFT                    (19U)
/*! CLRP19 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP19(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP19_SHIFT)) & GPIO_CLR_CLRP19_MASK)

#define GPIO_CLR_CLRP20_MASK                     (0x100000U)
#define GPIO_CLR_CLRP20_SHIFT                    (20U)
/*! CLRP20 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP20(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP20_SHIFT)) & GPIO_CLR_CLRP20_MASK)

#define GPIO_CLR_CLRP21_MASK                     (0x200000U)
#define GPIO_CLR_CLRP21_SHIFT                    (21U)
/*! CLRP21 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP21(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP21_SHIFT)) & GPIO_CLR_CLRP21_MASK)

#define GPIO_CLR_CLRP22_MASK                     (0x400000U)
#define GPIO_CLR_CLRP22_SHIFT                    (22U)
/*! CLRP22 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP22(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP22_SHIFT)) & GPIO_CLR_CLRP22_MASK)

#define GPIO_CLR_CLRP23_MASK                     (0x800000U)
#define GPIO_CLR_CLRP23_SHIFT                    (23U)
/*! CLRP23 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP23(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP23_SHIFT)) & GPIO_CLR_CLRP23_MASK)

#define GPIO_CLR_CLRP24_MASK                     (0x1000000U)
#define GPIO_CLR_CLRP24_SHIFT                    (24U)
/*! CLRP24 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP24(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP24_SHIFT)) & GPIO_CLR_CLRP24_MASK)

#define GPIO_CLR_CLRP25_MASK                     (0x2000000U)
#define GPIO_CLR_CLRP25_SHIFT                    (25U)
/*! CLRP25 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP25(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP25_SHIFT)) & GPIO_CLR_CLRP25_MASK)

#define GPIO_CLR_CLRP26_MASK                     (0x4000000U)
#define GPIO_CLR_CLRP26_SHIFT                    (26U)
/*! CLRP26 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP26(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP26_SHIFT)) & GPIO_CLR_CLRP26_MASK)

#define GPIO_CLR_CLRP27_MASK                     (0x8000000U)
#define GPIO_CLR_CLRP27_SHIFT                    (27U)
/*! CLRP27 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP27(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP27_SHIFT)) & GPIO_CLR_CLRP27_MASK)

#define GPIO_CLR_CLRP28_MASK                     (0x10000000U)
#define GPIO_CLR_CLRP28_SHIFT                    (28U)
/*! CLRP28 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP28(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP28_SHIFT)) & GPIO_CLR_CLRP28_MASK)

#define GPIO_CLR_CLRP29_MASK                     (0x20000000U)
#define GPIO_CLR_CLRP29_SHIFT                    (29U)
/*! CLRP29 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP29(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP29_SHIFT)) & GPIO_CLR_CLRP29_MASK)

#define GPIO_CLR_CLRP30_MASK                     (0x40000000U)
#define GPIO_CLR_CLRP30_SHIFT                    (30U)
/*! CLRP30 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP30(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP30_SHIFT)) & GPIO_CLR_CLRP30_MASK)

#define GPIO_CLR_CLRP31_MASK                     (0x80000000U)
#define GPIO_CLR_CLRP31_SHIFT                    (31U)
/*! CLRP31 - Clear output bits
 *  0b0..No operation
 *  0b1..Clears output bit
 */
#define GPIO_CLR_CLRP31(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_CLR_CLRP31_SHIFT)) & GPIO_CLR_CLRP31_MASK)
/*! @} */

/*! @name NOT - Port toggle */
/*! @{ */

#define GPIO_NOT_NOTP0_MASK                      (0x1U)
#define GPIO_NOT_NOTP0_SHIFT                     (0U)
/*! NOTP0 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP0(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP0_SHIFT)) & GPIO_NOT_NOTP0_MASK)

#define GPIO_NOT_NOTP1_MASK                      (0x2U)
#define GPIO_NOT_NOTP1_SHIFT                     (1U)
/*! NOTP1 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP1(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP1_SHIFT)) & GPIO_NOT_NOTP1_MASK)

#define GPIO_NOT_NOTP2_MASK                      (0x4U)
#define GPIO_NOT_NOTP2_SHIFT                     (2U)
/*! NOTP2 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP2(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP2_SHIFT)) & GPIO_NOT_NOTP2_MASK)

#define GPIO_NOT_NOTP3_MASK                      (0x8U)
#define GPIO_NOT_NOTP3_SHIFT                     (3U)
/*! NOTP3 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP3(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP3_SHIFT)) & GPIO_NOT_NOTP3_MASK)

#define GPIO_NOT_NOTP4_MASK                      (0x10U)
#define GPIO_NOT_NOTP4_SHIFT                     (4U)
/*! NOTP4 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP4(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP4_SHIFT)) & GPIO_NOT_NOTP4_MASK)

#define GPIO_NOT_NOTP5_MASK                      (0x20U)
#define GPIO_NOT_NOTP5_SHIFT                     (5U)
/*! NOTP5 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP5(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP5_SHIFT)) & GPIO_NOT_NOTP5_MASK)

#define GPIO_NOT_NOTP6_MASK                      (0x40U)
#define GPIO_NOT_NOTP6_SHIFT                     (6U)
/*! NOTP6 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP6(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP6_SHIFT)) & GPIO_NOT_NOTP6_MASK)

#define GPIO_NOT_NOTP7_MASK                      (0x80U)
#define GPIO_NOT_NOTP7_SHIFT                     (7U)
/*! NOTP7 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP7(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP7_SHIFT)) & GPIO_NOT_NOTP7_MASK)

#define GPIO_NOT_NOTP8_MASK                      (0x100U)
#define GPIO_NOT_NOTP8_SHIFT                     (8U)
/*! NOTP8 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP8(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP8_SHIFT)) & GPIO_NOT_NOTP8_MASK)

#define GPIO_NOT_NOTP9_MASK                      (0x200U)
#define GPIO_NOT_NOTP9_SHIFT                     (9U)
/*! NOTP9 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP9(x)                        (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP9_SHIFT)) & GPIO_NOT_NOTP9_MASK)

#define GPIO_NOT_NOTP10_MASK                     (0x400U)
#define GPIO_NOT_NOTP10_SHIFT                    (10U)
/*! NOTP10 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP10(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP10_SHIFT)) & GPIO_NOT_NOTP10_MASK)

#define GPIO_NOT_NOTP11_MASK                     (0x800U)
#define GPIO_NOT_NOTP11_SHIFT                    (11U)
/*! NOTP11 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP11(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP11_SHIFT)) & GPIO_NOT_NOTP11_MASK)

#define GPIO_NOT_NOTP12_MASK                     (0x1000U)
#define GPIO_NOT_NOTP12_SHIFT                    (12U)
/*! NOTP12 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP12(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP12_SHIFT)) & GPIO_NOT_NOTP12_MASK)

#define GPIO_NOT_NOTP13_MASK                     (0x2000U)
#define GPIO_NOT_NOTP13_SHIFT                    (13U)
/*! NOTP13 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP13(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP13_SHIFT)) & GPIO_NOT_NOTP13_MASK)

#define GPIO_NOT_NOTP14_MASK                     (0x4000U)
#define GPIO_NOT_NOTP14_SHIFT                    (14U)
/*! NOTP14 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP14(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP14_SHIFT)) & GPIO_NOT_NOTP14_MASK)

#define GPIO_NOT_NOTP15_MASK                     (0x8000U)
#define GPIO_NOT_NOTP15_SHIFT                    (15U)
/*! NOTP15 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP15(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP15_SHIFT)) & GPIO_NOT_NOTP15_MASK)

#define GPIO_NOT_NOTP16_MASK                     (0x10000U)
#define GPIO_NOT_NOTP16_SHIFT                    (16U)
/*! NOTP16 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP16(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP16_SHIFT)) & GPIO_NOT_NOTP16_MASK)

#define GPIO_NOT_NOTP17_MASK                     (0x20000U)
#define GPIO_NOT_NOTP17_SHIFT                    (17U)
/*! NOTP17 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP17(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP17_SHIFT)) & GPIO_NOT_NOTP17_MASK)

#define GPIO_NOT_NOTP18_MASK                     (0x40000U)
#define GPIO_NOT_NOTP18_SHIFT                    (18U)
/*! NOTP18 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP18(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP18_SHIFT)) & GPIO_NOT_NOTP18_MASK)

#define GPIO_NOT_NOTP19_MASK                     (0x80000U)
#define GPIO_NOT_NOTP19_SHIFT                    (19U)
/*! NOTP19 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP19(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP19_SHIFT)) & GPIO_NOT_NOTP19_MASK)

#define GPIO_NOT_NOTP20_MASK                     (0x100000U)
#define GPIO_NOT_NOTP20_SHIFT                    (20U)
/*! NOTP20 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP20(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP20_SHIFT)) & GPIO_NOT_NOTP20_MASK)

#define GPIO_NOT_NOTP21_MASK                     (0x200000U)
#define GPIO_NOT_NOTP21_SHIFT                    (21U)
/*! NOTP21 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP21(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP21_SHIFT)) & GPIO_NOT_NOTP21_MASK)

#define GPIO_NOT_NOTP22_MASK                     (0x400000U)
#define GPIO_NOT_NOTP22_SHIFT                    (22U)
/*! NOTP22 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP22(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP22_SHIFT)) & GPIO_NOT_NOTP22_MASK)

#define GPIO_NOT_NOTP23_MASK                     (0x800000U)
#define GPIO_NOT_NOTP23_SHIFT                    (23U)
/*! NOTP23 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP23(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP23_SHIFT)) & GPIO_NOT_NOTP23_MASK)

#define GPIO_NOT_NOTP24_MASK                     (0x1000000U)
#define GPIO_NOT_NOTP24_SHIFT                    (24U)
/*! NOTP24 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP24(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP24_SHIFT)) & GPIO_NOT_NOTP24_MASK)

#define GPIO_NOT_NOTP25_MASK                     (0x2000000U)
#define GPIO_NOT_NOTP25_SHIFT                    (25U)
/*! NOTP25 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP25(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP25_SHIFT)) & GPIO_NOT_NOTP25_MASK)

#define GPIO_NOT_NOTP26_MASK                     (0x4000000U)
#define GPIO_NOT_NOTP26_SHIFT                    (26U)
/*! NOTP26 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP26(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP26_SHIFT)) & GPIO_NOT_NOTP26_MASK)

#define GPIO_NOT_NOTP27_MASK                     (0x8000000U)
#define GPIO_NOT_NOTP27_SHIFT                    (27U)
/*! NOTP27 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP27(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP27_SHIFT)) & GPIO_NOT_NOTP27_MASK)

#define GPIO_NOT_NOTP28_MASK                     (0x10000000U)
#define GPIO_NOT_NOTP28_SHIFT                    (28U)
/*! NOTP28 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP28(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP28_SHIFT)) & GPIO_NOT_NOTP28_MASK)

#define GPIO_NOT_NOTP29_MASK                     (0x20000000U)
#define GPIO_NOT_NOTP29_SHIFT                    (29U)
/*! NOTP29 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP29(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP29_SHIFT)) & GPIO_NOT_NOTP29_MASK)

#define GPIO_NOT_NOTP30_MASK                     (0x40000000U)
#define GPIO_NOT_NOTP30_SHIFT                    (30U)
/*! NOTP30 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP30(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP30_SHIFT)) & GPIO_NOT_NOTP30_MASK)

#define GPIO_NOT_NOTP31_MASK                     (0x80000000U)
#define GPIO_NOT_NOTP31_SHIFT                    (31U)
/*! NOTP31 - Toggle output bits
 *  0b0..No operation
 *  0b1..Toggle output bit
 */
#define GPIO_NOT_NOTP31(x)                       (((uint32_t)(((uint32_t)(x)) << GPIO_NOT_NOTP31_SHIFT)) & GPIO_NOT_NOTP31_MASK)
/*! @} */

/*! @name DIRSET - Port direction set */
/*! @{ */

#define GPIO_DIRSET_DIRSETP0_MASK                (0x1U)
#define GPIO_DIRSET_DIRSETP0_SHIFT               (0U)
/*! DIRSETP0 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP0(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP0_SHIFT)) & GPIO_DIRSET_DIRSETP0_MASK)

#define GPIO_DIRSET_DIRSETP1_MASK                (0x2U)
#define GPIO_DIRSET_DIRSETP1_SHIFT               (1U)
/*! DIRSETP1 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP1(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP1_SHIFT)) & GPIO_DIRSET_DIRSETP1_MASK)

#define GPIO_DIRSET_DIRSETP2_MASK                (0x4U)
#define GPIO_DIRSET_DIRSETP2_SHIFT               (2U)
/*! DIRSETP2 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP2(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP2_SHIFT)) & GPIO_DIRSET_DIRSETP2_MASK)

#define GPIO_DIRSET_DIRSETP3_MASK                (0x8U)
#define GPIO_DIRSET_DIRSETP3_SHIFT               (3U)
/*! DIRSETP3 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP3(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP3_SHIFT)) & GPIO_DIRSET_DIRSETP3_MASK)

#define GPIO_DIRSET_DIRSETP4_MASK                (0x10U)
#define GPIO_DIRSET_DIRSETP4_SHIFT               (4U)
/*! DIRSETP4 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP4(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP4_SHIFT)) & GPIO_DIRSET_DIRSETP4_MASK)

#define GPIO_DIRSET_DIRSETP5_MASK                (0x20U)
#define GPIO_DIRSET_DIRSETP5_SHIFT               (5U)
/*! DIRSETP5 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP5(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP5_SHIFT)) & GPIO_DIRSET_DIRSETP5_MASK)

#define GPIO_DIRSET_DIRSETP6_MASK                (0x40U)
#define GPIO_DIRSET_DIRSETP6_SHIFT               (6U)
/*! DIRSETP6 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP6(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP6_SHIFT)) & GPIO_DIRSET_DIRSETP6_MASK)

#define GPIO_DIRSET_DIRSETP7_MASK                (0x80U)
#define GPIO_DIRSET_DIRSETP7_SHIFT               (7U)
/*! DIRSETP7 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP7(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP7_SHIFT)) & GPIO_DIRSET_DIRSETP7_MASK)

#define GPIO_DIRSET_DIRSETP8_MASK                (0x100U)
#define GPIO_DIRSET_DIRSETP8_SHIFT               (8U)
/*! DIRSETP8 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP8(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP8_SHIFT)) & GPIO_DIRSET_DIRSETP8_MASK)

#define GPIO_DIRSET_DIRSETP9_MASK                (0x200U)
#define GPIO_DIRSET_DIRSETP9_SHIFT               (9U)
/*! DIRSETP9 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP9(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP9_SHIFT)) & GPIO_DIRSET_DIRSETP9_MASK)

#define GPIO_DIRSET_DIRSETP10_MASK               (0x400U)
#define GPIO_DIRSET_DIRSETP10_SHIFT              (10U)
/*! DIRSETP10 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP10(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP10_SHIFT)) & GPIO_DIRSET_DIRSETP10_MASK)

#define GPIO_DIRSET_DIRSETP11_MASK               (0x800U)
#define GPIO_DIRSET_DIRSETP11_SHIFT              (11U)
/*! DIRSETP11 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP11(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP11_SHIFT)) & GPIO_DIRSET_DIRSETP11_MASK)

#define GPIO_DIRSET_DIRSETP12_MASK               (0x1000U)
#define GPIO_DIRSET_DIRSETP12_SHIFT              (12U)
/*! DIRSETP12 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP12(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP12_SHIFT)) & GPIO_DIRSET_DIRSETP12_MASK)

#define GPIO_DIRSET_DIRSETP13_MASK               (0x2000U)
#define GPIO_DIRSET_DIRSETP13_SHIFT              (13U)
/*! DIRSETP13 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP13(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP13_SHIFT)) & GPIO_DIRSET_DIRSETP13_MASK)

#define GPIO_DIRSET_DIRSETP14_MASK               (0x4000U)
#define GPIO_DIRSET_DIRSETP14_SHIFT              (14U)
/*! DIRSETP14 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP14(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP14_SHIFT)) & GPIO_DIRSET_DIRSETP14_MASK)

#define GPIO_DIRSET_DIRSETP15_MASK               (0x8000U)
#define GPIO_DIRSET_DIRSETP15_SHIFT              (15U)
/*! DIRSETP15 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP15(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP15_SHIFT)) & GPIO_DIRSET_DIRSETP15_MASK)

#define GPIO_DIRSET_DIRSETP16_MASK               (0x10000U)
#define GPIO_DIRSET_DIRSETP16_SHIFT              (16U)
/*! DIRSETP16 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP16(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP16_SHIFT)) & GPIO_DIRSET_DIRSETP16_MASK)

#define GPIO_DIRSET_DIRSETP17_MASK               (0x20000U)
#define GPIO_DIRSET_DIRSETP17_SHIFT              (17U)
/*! DIRSETP17 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP17(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP17_SHIFT)) & GPIO_DIRSET_DIRSETP17_MASK)

#define GPIO_DIRSET_DIRSETP18_MASK               (0x40000U)
#define GPIO_DIRSET_DIRSETP18_SHIFT              (18U)
/*! DIRSETP18 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP18(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP18_SHIFT)) & GPIO_DIRSET_DIRSETP18_MASK)

#define GPIO_DIRSET_DIRSETP19_MASK               (0x80000U)
#define GPIO_DIRSET_DIRSETP19_SHIFT              (19U)
/*! DIRSETP19 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP19(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP19_SHIFT)) & GPIO_DIRSET_DIRSETP19_MASK)

#define GPIO_DIRSET_DIRSETP20_MASK               (0x100000U)
#define GPIO_DIRSET_DIRSETP20_SHIFT              (20U)
/*! DIRSETP20 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP20(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP20_SHIFT)) & GPIO_DIRSET_DIRSETP20_MASK)

#define GPIO_DIRSET_DIRSETP21_MASK               (0x200000U)
#define GPIO_DIRSET_DIRSETP21_SHIFT              (21U)
/*! DIRSETP21 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP21(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP21_SHIFT)) & GPIO_DIRSET_DIRSETP21_MASK)

#define GPIO_DIRSET_DIRSETP22_MASK               (0x400000U)
#define GPIO_DIRSET_DIRSETP22_SHIFT              (22U)
/*! DIRSETP22 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP22(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP22_SHIFT)) & GPIO_DIRSET_DIRSETP22_MASK)

#define GPIO_DIRSET_DIRSETP23_MASK               (0x800000U)
#define GPIO_DIRSET_DIRSETP23_SHIFT              (23U)
/*! DIRSETP23 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP23(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP23_SHIFT)) & GPIO_DIRSET_DIRSETP23_MASK)

#define GPIO_DIRSET_DIRSETP24_MASK               (0x1000000U)
#define GPIO_DIRSET_DIRSETP24_SHIFT              (24U)
/*! DIRSETP24 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP24(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP24_SHIFT)) & GPIO_DIRSET_DIRSETP24_MASK)

#define GPIO_DIRSET_DIRSETP25_MASK               (0x2000000U)
#define GPIO_DIRSET_DIRSETP25_SHIFT              (25U)
/*! DIRSETP25 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP25(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP25_SHIFT)) & GPIO_DIRSET_DIRSETP25_MASK)

#define GPIO_DIRSET_DIRSETP26_MASK               (0x4000000U)
#define GPIO_DIRSET_DIRSETP26_SHIFT              (26U)
/*! DIRSETP26 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP26(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP26_SHIFT)) & GPIO_DIRSET_DIRSETP26_MASK)

#define GPIO_DIRSET_DIRSETP27_MASK               (0x8000000U)
#define GPIO_DIRSET_DIRSETP27_SHIFT              (27U)
/*! DIRSETP27 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP27(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP27_SHIFT)) & GPIO_DIRSET_DIRSETP27_MASK)

#define GPIO_DIRSET_DIRSETP28_MASK               (0x10000000U)
#define GPIO_DIRSET_DIRSETP28_SHIFT              (28U)
/*! DIRSETP28 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP28(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP28_SHIFT)) & GPIO_DIRSET_DIRSETP28_MASK)

#define GPIO_DIRSET_DIRSETP29_MASK               (0x20000000U)
#define GPIO_DIRSET_DIRSETP29_SHIFT              (29U)
/*! DIRSETP29 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP29(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP29_SHIFT)) & GPIO_DIRSET_DIRSETP29_MASK)

#define GPIO_DIRSET_DIRSETP30_MASK               (0x40000000U)
#define GPIO_DIRSET_DIRSETP30_SHIFT              (30U)
/*! DIRSETP30 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP30(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP30_SHIFT)) & GPIO_DIRSET_DIRSETP30_MASK)

#define GPIO_DIRSET_DIRSETP31_MASK               (0x80000000U)
#define GPIO_DIRSET_DIRSETP31_SHIFT              (31U)
/*! DIRSETP31 - Direction set bits for Port pins
 *  0b0..No operation
 *  0b1..Sets direction bit
 */
#define GPIO_DIRSET_DIRSETP31(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRSET_DIRSETP31_SHIFT)) & GPIO_DIRSET_DIRSETP31_MASK)
/*! @} */

/*! @name DIRCLR - Port direction clear */
/*! @{ */

#define GPIO_DIRCLR_DIRCLRP0_MASK                (0x1U)
#define GPIO_DIRCLR_DIRCLRP0_SHIFT               (0U)
/*! DIRCLRP0 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP0(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP0_SHIFT)) & GPIO_DIRCLR_DIRCLRP0_MASK)

#define GPIO_DIRCLR_DIRCLRP1_MASK                (0x2U)
#define GPIO_DIRCLR_DIRCLRP1_SHIFT               (1U)
/*! DIRCLRP1 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP1(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP1_SHIFT)) & GPIO_DIRCLR_DIRCLRP1_MASK)

#define GPIO_DIRCLR_DIRCLRP2_MASK                (0x4U)
#define GPIO_DIRCLR_DIRCLRP2_SHIFT               (2U)
/*! DIRCLRP2 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP2(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP2_SHIFT)) & GPIO_DIRCLR_DIRCLRP2_MASK)

#define GPIO_DIRCLR_DIRCLRP3_MASK                (0x8U)
#define GPIO_DIRCLR_DIRCLRP3_SHIFT               (3U)
/*! DIRCLRP3 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP3(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP3_SHIFT)) & GPIO_DIRCLR_DIRCLRP3_MASK)

#define GPIO_DIRCLR_DIRCLRP4_MASK                (0x10U)
#define GPIO_DIRCLR_DIRCLRP4_SHIFT               (4U)
/*! DIRCLRP4 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP4(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP4_SHIFT)) & GPIO_DIRCLR_DIRCLRP4_MASK)

#define GPIO_DIRCLR_DIRCLRP5_MASK                (0x20U)
#define GPIO_DIRCLR_DIRCLRP5_SHIFT               (5U)
/*! DIRCLRP5 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP5(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP5_SHIFT)) & GPIO_DIRCLR_DIRCLRP5_MASK)

#define GPIO_DIRCLR_DIRCLRP6_MASK                (0x40U)
#define GPIO_DIRCLR_DIRCLRP6_SHIFT               (6U)
/*! DIRCLRP6 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP6(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP6_SHIFT)) & GPIO_DIRCLR_DIRCLRP6_MASK)

#define GPIO_DIRCLR_DIRCLRP7_MASK                (0x80U)
#define GPIO_DIRCLR_DIRCLRP7_SHIFT               (7U)
/*! DIRCLRP7 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP7(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP7_SHIFT)) & GPIO_DIRCLR_DIRCLRP7_MASK)

#define GPIO_DIRCLR_DIRCLRP8_MASK                (0x100U)
#define GPIO_DIRCLR_DIRCLRP8_SHIFT               (8U)
/*! DIRCLRP8 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP8(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP8_SHIFT)) & GPIO_DIRCLR_DIRCLRP8_MASK)

#define GPIO_DIRCLR_DIRCLRP9_MASK                (0x200U)
#define GPIO_DIRCLR_DIRCLRP9_SHIFT               (9U)
/*! DIRCLRP9 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP9(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP9_SHIFT)) & GPIO_DIRCLR_DIRCLRP9_MASK)

#define GPIO_DIRCLR_DIRCLRP10_MASK               (0x400U)
#define GPIO_DIRCLR_DIRCLRP10_SHIFT              (10U)
/*! DIRCLRP10 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP10(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP10_SHIFT)) & GPIO_DIRCLR_DIRCLRP10_MASK)

#define GPIO_DIRCLR_DIRCLRP11_MASK               (0x800U)
#define GPIO_DIRCLR_DIRCLRP11_SHIFT              (11U)
/*! DIRCLRP11 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP11(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP11_SHIFT)) & GPIO_DIRCLR_DIRCLRP11_MASK)

#define GPIO_DIRCLR_DIRCLRP12_MASK               (0x1000U)
#define GPIO_DIRCLR_DIRCLRP12_SHIFT              (12U)
/*! DIRCLRP12 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP12(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP12_SHIFT)) & GPIO_DIRCLR_DIRCLRP12_MASK)

#define GPIO_DIRCLR_DIRCLRP13_MASK               (0x2000U)
#define GPIO_DIRCLR_DIRCLRP13_SHIFT              (13U)
/*! DIRCLRP13 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP13(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP13_SHIFT)) & GPIO_DIRCLR_DIRCLRP13_MASK)

#define GPIO_DIRCLR_DIRCLRP14_MASK               (0x4000U)
#define GPIO_DIRCLR_DIRCLRP14_SHIFT              (14U)
/*! DIRCLRP14 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP14(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP14_SHIFT)) & GPIO_DIRCLR_DIRCLRP14_MASK)

#define GPIO_DIRCLR_DIRCLRP15_MASK               (0x8000U)
#define GPIO_DIRCLR_DIRCLRP15_SHIFT              (15U)
/*! DIRCLRP15 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP15(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP15_SHIFT)) & GPIO_DIRCLR_DIRCLRP15_MASK)

#define GPIO_DIRCLR_DIRCLRP16_MASK               (0x10000U)
#define GPIO_DIRCLR_DIRCLRP16_SHIFT              (16U)
/*! DIRCLRP16 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP16(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP16_SHIFT)) & GPIO_DIRCLR_DIRCLRP16_MASK)

#define GPIO_DIRCLR_DIRCLRP17_MASK               (0x20000U)
#define GPIO_DIRCLR_DIRCLRP17_SHIFT              (17U)
/*! DIRCLRP17 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP17(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP17_SHIFT)) & GPIO_DIRCLR_DIRCLRP17_MASK)

#define GPIO_DIRCLR_DIRCLRP18_MASK               (0x40000U)
#define GPIO_DIRCLR_DIRCLRP18_SHIFT              (18U)
/*! DIRCLRP18 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP18(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP18_SHIFT)) & GPIO_DIRCLR_DIRCLRP18_MASK)

#define GPIO_DIRCLR_DIRCLRP19_MASK               (0x80000U)
#define GPIO_DIRCLR_DIRCLRP19_SHIFT              (19U)
/*! DIRCLRP19 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP19(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP19_SHIFT)) & GPIO_DIRCLR_DIRCLRP19_MASK)

#define GPIO_DIRCLR_DIRCLRP20_MASK               (0x100000U)
#define GPIO_DIRCLR_DIRCLRP20_SHIFT              (20U)
/*! DIRCLRP20 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP20(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP20_SHIFT)) & GPIO_DIRCLR_DIRCLRP20_MASK)

#define GPIO_DIRCLR_DIRCLRP21_MASK               (0x200000U)
#define GPIO_DIRCLR_DIRCLRP21_SHIFT              (21U)
/*! DIRCLRP21 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP21(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP21_SHIFT)) & GPIO_DIRCLR_DIRCLRP21_MASK)

#define GPIO_DIRCLR_DIRCLRP22_MASK               (0x400000U)
#define GPIO_DIRCLR_DIRCLRP22_SHIFT              (22U)
/*! DIRCLRP22 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP22(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP22_SHIFT)) & GPIO_DIRCLR_DIRCLRP22_MASK)

#define GPIO_DIRCLR_DIRCLRP23_MASK               (0x800000U)
#define GPIO_DIRCLR_DIRCLRP23_SHIFT              (23U)
/*! DIRCLRP23 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP23(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP23_SHIFT)) & GPIO_DIRCLR_DIRCLRP23_MASK)

#define GPIO_DIRCLR_DIRCLRP24_MASK               (0x1000000U)
#define GPIO_DIRCLR_DIRCLRP24_SHIFT              (24U)
/*! DIRCLRP24 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP24(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP24_SHIFT)) & GPIO_DIRCLR_DIRCLRP24_MASK)

#define GPIO_DIRCLR_DIRCLRP25_MASK               (0x2000000U)
#define GPIO_DIRCLR_DIRCLRP25_SHIFT              (25U)
/*! DIRCLRP25 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP25(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP25_SHIFT)) & GPIO_DIRCLR_DIRCLRP25_MASK)

#define GPIO_DIRCLR_DIRCLRP26_MASK               (0x4000000U)
#define GPIO_DIRCLR_DIRCLRP26_SHIFT              (26U)
/*! DIRCLRP26 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP26(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP26_SHIFT)) & GPIO_DIRCLR_DIRCLRP26_MASK)

#define GPIO_DIRCLR_DIRCLRP27_MASK               (0x8000000U)
#define GPIO_DIRCLR_DIRCLRP27_SHIFT              (27U)
/*! DIRCLRP27 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP27(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP27_SHIFT)) & GPIO_DIRCLR_DIRCLRP27_MASK)

#define GPIO_DIRCLR_DIRCLRP28_MASK               (0x10000000U)
#define GPIO_DIRCLR_DIRCLRP28_SHIFT              (28U)
/*! DIRCLRP28 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP28(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP28_SHIFT)) & GPIO_DIRCLR_DIRCLRP28_MASK)

#define GPIO_DIRCLR_DIRCLRP29_MASK               (0x20000000U)
#define GPIO_DIRCLR_DIRCLRP29_SHIFT              (29U)
/*! DIRCLRP29 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP29(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP29_SHIFT)) & GPIO_DIRCLR_DIRCLRP29_MASK)

#define GPIO_DIRCLR_DIRCLRP30_MASK               (0x40000000U)
#define GPIO_DIRCLR_DIRCLRP30_SHIFT              (30U)
/*! DIRCLRP30 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP30(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP30_SHIFT)) & GPIO_DIRCLR_DIRCLRP30_MASK)

#define GPIO_DIRCLR_DIRCLRP31_MASK               (0x80000000U)
#define GPIO_DIRCLR_DIRCLRP31_SHIFT              (31U)
/*! DIRCLRP31 - Clear direction bits.
 *  0b0..No operation
 *  0b1..Clears direction bits
 */
#define GPIO_DIRCLR_DIRCLRP31(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_DIRCLR_DIRCLRP31_SHIFT)) & GPIO_DIRCLR_DIRCLRP31_MASK)
/*! @} */

/*! @name DIRNOT - Port direction toggle */
/*! @{ */

#define GPIO_DIRNOT_DIRNOTP_MASK                 (0x1FFFFFFFU)
#define GPIO_DIRNOT_DIRNOTP_SHIFT                (0U)
/*! DIRNOTP - Toggle direction bits.
 *  0b00000000000000000000000000000..No operation
 *  0b00000000000000000000000000001..Toggles direction bit
 */
#define GPIO_DIRNOT_DIRNOTP(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_DIRNOT_DIRNOTP_SHIFT)) & GPIO_DIRNOT_DIRNOTP_MASK)
/*! @} */

/*! @name INTENA - Interrupt A enable control */
/*! @{ */

#define GPIO_INTENA_INT_EN0_MASK                 (0x1U)
#define GPIO_INTENA_INT_EN0_SHIFT                (0U)
/*! INT_EN0 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN0(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN0_SHIFT)) & GPIO_INTENA_INT_EN0_MASK)

#define GPIO_INTENA_INT_EN1_MASK                 (0x2U)
#define GPIO_INTENA_INT_EN1_SHIFT                (1U)
/*! INT_EN1 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN1(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN1_SHIFT)) & GPIO_INTENA_INT_EN1_MASK)

#define GPIO_INTENA_INT_EN2_MASK                 (0x4U)
#define GPIO_INTENA_INT_EN2_SHIFT                (2U)
/*! INT_EN2 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN2(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN2_SHIFT)) & GPIO_INTENA_INT_EN2_MASK)

#define GPIO_INTENA_INT_EN3_MASK                 (0x8U)
#define GPIO_INTENA_INT_EN3_SHIFT                (3U)
/*! INT_EN3 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN3(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN3_SHIFT)) & GPIO_INTENA_INT_EN3_MASK)

#define GPIO_INTENA_INT_EN4_MASK                 (0x10U)
#define GPIO_INTENA_INT_EN4_SHIFT                (4U)
/*! INT_EN4 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN4(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN4_SHIFT)) & GPIO_INTENA_INT_EN4_MASK)

#define GPIO_INTENA_INT_EN5_MASK                 (0x20U)
#define GPIO_INTENA_INT_EN5_SHIFT                (5U)
/*! INT_EN5 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN5(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN5_SHIFT)) & GPIO_INTENA_INT_EN5_MASK)

#define GPIO_INTENA_INT_EN6_MASK                 (0x40U)
#define GPIO_INTENA_INT_EN6_SHIFT                (6U)
/*! INT_EN6 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN6(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN6_SHIFT)) & GPIO_INTENA_INT_EN6_MASK)

#define GPIO_INTENA_INT_EN7_MASK                 (0x80U)
#define GPIO_INTENA_INT_EN7_SHIFT                (7U)
/*! INT_EN7 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN7(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN7_SHIFT)) & GPIO_INTENA_INT_EN7_MASK)

#define GPIO_INTENA_INT_EN8_MASK                 (0x100U)
#define GPIO_INTENA_INT_EN8_SHIFT                (8U)
/*! INT_EN8 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN8(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN8_SHIFT)) & GPIO_INTENA_INT_EN8_MASK)

#define GPIO_INTENA_INT_EN9_MASK                 (0x200U)
#define GPIO_INTENA_INT_EN9_SHIFT                (9U)
/*! INT_EN9 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN9(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN9_SHIFT)) & GPIO_INTENA_INT_EN9_MASK)

#define GPIO_INTENA_INT_EN10_MASK                (0x400U)
#define GPIO_INTENA_INT_EN10_SHIFT               (10U)
/*! INT_EN10 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN10(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN10_SHIFT)) & GPIO_INTENA_INT_EN10_MASK)

#define GPIO_INTENA_INT_EN11_MASK                (0x800U)
#define GPIO_INTENA_INT_EN11_SHIFT               (11U)
/*! INT_EN11 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN11(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN11_SHIFT)) & GPIO_INTENA_INT_EN11_MASK)

#define GPIO_INTENA_INT_EN12_MASK                (0x1000U)
#define GPIO_INTENA_INT_EN12_SHIFT               (12U)
/*! INT_EN12 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN12(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN12_SHIFT)) & GPIO_INTENA_INT_EN12_MASK)

#define GPIO_INTENA_INT_EN13_MASK                (0x2000U)
#define GPIO_INTENA_INT_EN13_SHIFT               (13U)
/*! INT_EN13 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN13(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN13_SHIFT)) & GPIO_INTENA_INT_EN13_MASK)

#define GPIO_INTENA_INT_EN14_MASK                (0x4000U)
#define GPIO_INTENA_INT_EN14_SHIFT               (14U)
/*! INT_EN14 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN14(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN14_SHIFT)) & GPIO_INTENA_INT_EN14_MASK)

#define GPIO_INTENA_INT_EN15_MASK                (0x8000U)
#define GPIO_INTENA_INT_EN15_SHIFT               (15U)
/*! INT_EN15 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN15(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN15_SHIFT)) & GPIO_INTENA_INT_EN15_MASK)

#define GPIO_INTENA_INT_EN16_MASK                (0x10000U)
#define GPIO_INTENA_INT_EN16_SHIFT               (16U)
/*! INT_EN16 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN16(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN16_SHIFT)) & GPIO_INTENA_INT_EN16_MASK)

#define GPIO_INTENA_INT_EN17_MASK                (0x20000U)
#define GPIO_INTENA_INT_EN17_SHIFT               (17U)
/*! INT_EN17 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN17(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN17_SHIFT)) & GPIO_INTENA_INT_EN17_MASK)

#define GPIO_INTENA_INT_EN18_MASK                (0x40000U)
#define GPIO_INTENA_INT_EN18_SHIFT               (18U)
/*! INT_EN18 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN18(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN18_SHIFT)) & GPIO_INTENA_INT_EN18_MASK)

#define GPIO_INTENA_INT_EN19_MASK                (0x80000U)
#define GPIO_INTENA_INT_EN19_SHIFT               (19U)
/*! INT_EN19 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN19(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN19_SHIFT)) & GPIO_INTENA_INT_EN19_MASK)

#define GPIO_INTENA_INT_EN20_MASK                (0x100000U)
#define GPIO_INTENA_INT_EN20_SHIFT               (20U)
/*! INT_EN20 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN20(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN20_SHIFT)) & GPIO_INTENA_INT_EN20_MASK)

#define GPIO_INTENA_INT_EN21_MASK                (0x200000U)
#define GPIO_INTENA_INT_EN21_SHIFT               (21U)
/*! INT_EN21 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN21(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN21_SHIFT)) & GPIO_INTENA_INT_EN21_MASK)

#define GPIO_INTENA_INT_EN22_MASK                (0x400000U)
#define GPIO_INTENA_INT_EN22_SHIFT               (22U)
/*! INT_EN22 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN22(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN22_SHIFT)) & GPIO_INTENA_INT_EN22_MASK)

#define GPIO_INTENA_INT_EN23_MASK                (0x800000U)
#define GPIO_INTENA_INT_EN23_SHIFT               (23U)
/*! INT_EN23 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN23(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN23_SHIFT)) & GPIO_INTENA_INT_EN23_MASK)

#define GPIO_INTENA_INT_EN24_MASK                (0x1000000U)
#define GPIO_INTENA_INT_EN24_SHIFT               (24U)
/*! INT_EN24 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN24(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN24_SHIFT)) & GPIO_INTENA_INT_EN24_MASK)

#define GPIO_INTENA_INT_EN25_MASK                (0x2000000U)
#define GPIO_INTENA_INT_EN25_SHIFT               (25U)
/*! INT_EN25 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN25(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN25_SHIFT)) & GPIO_INTENA_INT_EN25_MASK)

#define GPIO_INTENA_INT_EN26_MASK                (0x4000000U)
#define GPIO_INTENA_INT_EN26_SHIFT               (26U)
/*! INT_EN26 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN26(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN26_SHIFT)) & GPIO_INTENA_INT_EN26_MASK)

#define GPIO_INTENA_INT_EN27_MASK                (0x8000000U)
#define GPIO_INTENA_INT_EN27_SHIFT               (27U)
/*! INT_EN27 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN27(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN27_SHIFT)) & GPIO_INTENA_INT_EN27_MASK)

#define GPIO_INTENA_INT_EN28_MASK                (0x10000000U)
#define GPIO_INTENA_INT_EN28_SHIFT               (28U)
/*! INT_EN28 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN28(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN28_SHIFT)) & GPIO_INTENA_INT_EN28_MASK)

#define GPIO_INTENA_INT_EN29_MASK                (0x20000000U)
#define GPIO_INTENA_INT_EN29_SHIFT               (29U)
/*! INT_EN29 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN29(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN29_SHIFT)) & GPIO_INTENA_INT_EN29_MASK)

#define GPIO_INTENA_INT_EN30_MASK                (0x40000000U)
#define GPIO_INTENA_INT_EN30_SHIFT               (30U)
/*! INT_EN30 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN30(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN30_SHIFT)) & GPIO_INTENA_INT_EN30_MASK)

#define GPIO_INTENA_INT_EN31_MASK                (0x80000000U)
#define GPIO_INTENA_INT_EN31_SHIFT               (31U)
/*! INT_EN31 - Interrupt A enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt A
 *  0b1..Pin contributes to GPIO interrupt A
 */
#define GPIO_INTENA_INT_EN31(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENA_INT_EN31_SHIFT)) & GPIO_INTENA_INT_EN31_MASK)
/*! @} */

/*! @name INTENB - Interrupt B enable control */
/*! @{ */

#define GPIO_INTENB_INT_EN0_MASK                 (0x1U)
#define GPIO_INTENB_INT_EN0_SHIFT                (0U)
/*! INT_EN0 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN0(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN0_SHIFT)) & GPIO_INTENB_INT_EN0_MASK)

#define GPIO_INTENB_INT_EN1_MASK                 (0x2U)
#define GPIO_INTENB_INT_EN1_SHIFT                (1U)
/*! INT_EN1 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN1(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN1_SHIFT)) & GPIO_INTENB_INT_EN1_MASK)

#define GPIO_INTENB_INT_EN2_MASK                 (0x4U)
#define GPIO_INTENB_INT_EN2_SHIFT                (2U)
/*! INT_EN2 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN2(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN2_SHIFT)) & GPIO_INTENB_INT_EN2_MASK)

#define GPIO_INTENB_INT_EN3_MASK                 (0x8U)
#define GPIO_INTENB_INT_EN3_SHIFT                (3U)
/*! INT_EN3 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN3(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN3_SHIFT)) & GPIO_INTENB_INT_EN3_MASK)

#define GPIO_INTENB_INT_EN4_MASK                 (0x10U)
#define GPIO_INTENB_INT_EN4_SHIFT                (4U)
/*! INT_EN4 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN4(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN4_SHIFT)) & GPIO_INTENB_INT_EN4_MASK)

#define GPIO_INTENB_INT_EN5_MASK                 (0x20U)
#define GPIO_INTENB_INT_EN5_SHIFT                (5U)
/*! INT_EN5 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN5(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN5_SHIFT)) & GPIO_INTENB_INT_EN5_MASK)

#define GPIO_INTENB_INT_EN6_MASK                 (0x40U)
#define GPIO_INTENB_INT_EN6_SHIFT                (6U)
/*! INT_EN6 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN6(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN6_SHIFT)) & GPIO_INTENB_INT_EN6_MASK)

#define GPIO_INTENB_INT_EN7_MASK                 (0x80U)
#define GPIO_INTENB_INT_EN7_SHIFT                (7U)
/*! INT_EN7 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN7(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN7_SHIFT)) & GPIO_INTENB_INT_EN7_MASK)

#define GPIO_INTENB_INT_EN8_MASK                 (0x100U)
#define GPIO_INTENB_INT_EN8_SHIFT                (8U)
/*! INT_EN8 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN8(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN8_SHIFT)) & GPIO_INTENB_INT_EN8_MASK)

#define GPIO_INTENB_INT_EN9_MASK                 (0x200U)
#define GPIO_INTENB_INT_EN9_SHIFT                (9U)
/*! INT_EN9 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN9(x)                   (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN9_SHIFT)) & GPIO_INTENB_INT_EN9_MASK)

#define GPIO_INTENB_INT_EN10_MASK                (0x400U)
#define GPIO_INTENB_INT_EN10_SHIFT               (10U)
/*! INT_EN10 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN10(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN10_SHIFT)) & GPIO_INTENB_INT_EN10_MASK)

#define GPIO_INTENB_INT_EN11_MASK                (0x800U)
#define GPIO_INTENB_INT_EN11_SHIFT               (11U)
/*! INT_EN11 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN11(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN11_SHIFT)) & GPIO_INTENB_INT_EN11_MASK)

#define GPIO_INTENB_INT_EN12_MASK                (0x1000U)
#define GPIO_INTENB_INT_EN12_SHIFT               (12U)
/*! INT_EN12 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN12(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN12_SHIFT)) & GPIO_INTENB_INT_EN12_MASK)

#define GPIO_INTENB_INT_EN13_MASK                (0x2000U)
#define GPIO_INTENB_INT_EN13_SHIFT               (13U)
/*! INT_EN13 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN13(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN13_SHIFT)) & GPIO_INTENB_INT_EN13_MASK)

#define GPIO_INTENB_INT_EN14_MASK                (0x4000U)
#define GPIO_INTENB_INT_EN14_SHIFT               (14U)
/*! INT_EN14 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN14(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN14_SHIFT)) & GPIO_INTENB_INT_EN14_MASK)

#define GPIO_INTENB_INT_EN15_MASK                (0x8000U)
#define GPIO_INTENB_INT_EN15_SHIFT               (15U)
/*! INT_EN15 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN15(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN15_SHIFT)) & GPIO_INTENB_INT_EN15_MASK)

#define GPIO_INTENB_INT_EN16_MASK                (0x10000U)
#define GPIO_INTENB_INT_EN16_SHIFT               (16U)
/*! INT_EN16 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN16(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN16_SHIFT)) & GPIO_INTENB_INT_EN16_MASK)

#define GPIO_INTENB_INT_EN17_MASK                (0x20000U)
#define GPIO_INTENB_INT_EN17_SHIFT               (17U)
/*! INT_EN17 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN17(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN17_SHIFT)) & GPIO_INTENB_INT_EN17_MASK)

#define GPIO_INTENB_INT_EN18_MASK                (0x40000U)
#define GPIO_INTENB_INT_EN18_SHIFT               (18U)
/*! INT_EN18 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN18(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN18_SHIFT)) & GPIO_INTENB_INT_EN18_MASK)

#define GPIO_INTENB_INT_EN19_MASK                (0x80000U)
#define GPIO_INTENB_INT_EN19_SHIFT               (19U)
/*! INT_EN19 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN19(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN19_SHIFT)) & GPIO_INTENB_INT_EN19_MASK)

#define GPIO_INTENB_INT_EN20_MASK                (0x100000U)
#define GPIO_INTENB_INT_EN20_SHIFT               (20U)
/*! INT_EN20 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN20(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN20_SHIFT)) & GPIO_INTENB_INT_EN20_MASK)

#define GPIO_INTENB_INT_EN21_MASK                (0x200000U)
#define GPIO_INTENB_INT_EN21_SHIFT               (21U)
/*! INT_EN21 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN21(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN21_SHIFT)) & GPIO_INTENB_INT_EN21_MASK)

#define GPIO_INTENB_INT_EN22_MASK                (0x400000U)
#define GPIO_INTENB_INT_EN22_SHIFT               (22U)
/*! INT_EN22 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN22(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN22_SHIFT)) & GPIO_INTENB_INT_EN22_MASK)

#define GPIO_INTENB_INT_EN23_MASK                (0x800000U)
#define GPIO_INTENB_INT_EN23_SHIFT               (23U)
/*! INT_EN23 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN23(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN23_SHIFT)) & GPIO_INTENB_INT_EN23_MASK)

#define GPIO_INTENB_INT_EN24_MASK                (0x1000000U)
#define GPIO_INTENB_INT_EN24_SHIFT               (24U)
/*! INT_EN24 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN24(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN24_SHIFT)) & GPIO_INTENB_INT_EN24_MASK)

#define GPIO_INTENB_INT_EN25_MASK                (0x2000000U)
#define GPIO_INTENB_INT_EN25_SHIFT               (25U)
/*! INT_EN25 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN25(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN25_SHIFT)) & GPIO_INTENB_INT_EN25_MASK)

#define GPIO_INTENB_INT_EN26_MASK                (0x4000000U)
#define GPIO_INTENB_INT_EN26_SHIFT               (26U)
/*! INT_EN26 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN26(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN26_SHIFT)) & GPIO_INTENB_INT_EN26_MASK)

#define GPIO_INTENB_INT_EN27_MASK                (0x8000000U)
#define GPIO_INTENB_INT_EN27_SHIFT               (27U)
/*! INT_EN27 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN27(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN27_SHIFT)) & GPIO_INTENB_INT_EN27_MASK)

#define GPIO_INTENB_INT_EN28_MASK                (0x10000000U)
#define GPIO_INTENB_INT_EN28_SHIFT               (28U)
/*! INT_EN28 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN28(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN28_SHIFT)) & GPIO_INTENB_INT_EN28_MASK)

#define GPIO_INTENB_INT_EN29_MASK                (0x20000000U)
#define GPIO_INTENB_INT_EN29_SHIFT               (29U)
/*! INT_EN29 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN29(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN29_SHIFT)) & GPIO_INTENB_INT_EN29_MASK)

#define GPIO_INTENB_INT_EN30_MASK                (0x40000000U)
#define GPIO_INTENB_INT_EN30_SHIFT               (30U)
/*! INT_EN30 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN30(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN30_SHIFT)) & GPIO_INTENB_INT_EN30_MASK)

#define GPIO_INTENB_INT_EN31_MASK                (0x80000000U)
#define GPIO_INTENB_INT_EN31_SHIFT               (31U)
/*! INT_EN31 - Interrupt B enable bits.
 *  0b0..Pin does not contribute to GPIO interrupt B
 *  0b1..Pin contributes to GPIO interrupt B
 */
#define GPIO_INTENB_INT_EN31(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTENB_INT_EN31_SHIFT)) & GPIO_INTENB_INT_EN31_MASK)
/*! @} */

/*! @name INTPOL - Interupt polarity control */
/*! @{ */

#define GPIO_INTPOL_POL_CTL0_MASK                (0x1U)
#define GPIO_INTPOL_POL_CTL0_SHIFT               (0U)
/*! POL_CTL0 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL0(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL0_SHIFT)) & GPIO_INTPOL_POL_CTL0_MASK)

#define GPIO_INTPOL_POL_CTL1_MASK                (0x2U)
#define GPIO_INTPOL_POL_CTL1_SHIFT               (1U)
/*! POL_CTL1 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL1(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL1_SHIFT)) & GPIO_INTPOL_POL_CTL1_MASK)

#define GPIO_INTPOL_POL_CTL2_MASK                (0x4U)
#define GPIO_INTPOL_POL_CTL2_SHIFT               (2U)
/*! POL_CTL2 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL2(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL2_SHIFT)) & GPIO_INTPOL_POL_CTL2_MASK)

#define GPIO_INTPOL_POL_CTL3_MASK                (0x8U)
#define GPIO_INTPOL_POL_CTL3_SHIFT               (3U)
/*! POL_CTL3 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL3(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL3_SHIFT)) & GPIO_INTPOL_POL_CTL3_MASK)

#define GPIO_INTPOL_POL_CTL4_MASK                (0x10U)
#define GPIO_INTPOL_POL_CTL4_SHIFT               (4U)
/*! POL_CTL4 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL4(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL4_SHIFT)) & GPIO_INTPOL_POL_CTL4_MASK)

#define GPIO_INTPOL_POL_CTL5_MASK                (0x20U)
#define GPIO_INTPOL_POL_CTL5_SHIFT               (5U)
/*! POL_CTL5 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL5(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL5_SHIFT)) & GPIO_INTPOL_POL_CTL5_MASK)

#define GPIO_INTPOL_POL_CTL6_MASK                (0x40U)
#define GPIO_INTPOL_POL_CTL6_SHIFT               (6U)
/*! POL_CTL6 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL6(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL6_SHIFT)) & GPIO_INTPOL_POL_CTL6_MASK)

#define GPIO_INTPOL_POL_CTL7_MASK                (0x80U)
#define GPIO_INTPOL_POL_CTL7_SHIFT               (7U)
/*! POL_CTL7 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL7(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL7_SHIFT)) & GPIO_INTPOL_POL_CTL7_MASK)

#define GPIO_INTPOL_POL_CTL8_MASK                (0x100U)
#define GPIO_INTPOL_POL_CTL8_SHIFT               (8U)
/*! POL_CTL8 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL8(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL8_SHIFT)) & GPIO_INTPOL_POL_CTL8_MASK)

#define GPIO_INTPOL_POL_CTL9_MASK                (0x200U)
#define GPIO_INTPOL_POL_CTL9_SHIFT               (9U)
/*! POL_CTL9 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL9(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL9_SHIFT)) & GPIO_INTPOL_POL_CTL9_MASK)

#define GPIO_INTPOL_POL_CTL10_MASK               (0x400U)
#define GPIO_INTPOL_POL_CTL10_SHIFT              (10U)
/*! POL_CTL10 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL10(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL10_SHIFT)) & GPIO_INTPOL_POL_CTL10_MASK)

#define GPIO_INTPOL_POL_CTL11_MASK               (0x800U)
#define GPIO_INTPOL_POL_CTL11_SHIFT              (11U)
/*! POL_CTL11 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL11(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL11_SHIFT)) & GPIO_INTPOL_POL_CTL11_MASK)

#define GPIO_INTPOL_POL_CTL12_MASK               (0x1000U)
#define GPIO_INTPOL_POL_CTL12_SHIFT              (12U)
/*! POL_CTL12 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL12(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL12_SHIFT)) & GPIO_INTPOL_POL_CTL12_MASK)

#define GPIO_INTPOL_POL_CTL13_MASK               (0x2000U)
#define GPIO_INTPOL_POL_CTL13_SHIFT              (13U)
/*! POL_CTL13 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL13(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL13_SHIFT)) & GPIO_INTPOL_POL_CTL13_MASK)

#define GPIO_INTPOL_POL_CTL14_MASK               (0x4000U)
#define GPIO_INTPOL_POL_CTL14_SHIFT              (14U)
/*! POL_CTL14 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL14(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL14_SHIFT)) & GPIO_INTPOL_POL_CTL14_MASK)

#define GPIO_INTPOL_POL_CTL15_MASK               (0x8000U)
#define GPIO_INTPOL_POL_CTL15_SHIFT              (15U)
/*! POL_CTL15 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL15(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL15_SHIFT)) & GPIO_INTPOL_POL_CTL15_MASK)

#define GPIO_INTPOL_POL_CTL16_MASK               (0x10000U)
#define GPIO_INTPOL_POL_CTL16_SHIFT              (16U)
/*! POL_CTL16 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL16(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL16_SHIFT)) & GPIO_INTPOL_POL_CTL16_MASK)

#define GPIO_INTPOL_POL_CTL17_MASK               (0x20000U)
#define GPIO_INTPOL_POL_CTL17_SHIFT              (17U)
/*! POL_CTL17 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL17(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL17_SHIFT)) & GPIO_INTPOL_POL_CTL17_MASK)

#define GPIO_INTPOL_POL_CTL18_MASK               (0x40000U)
#define GPIO_INTPOL_POL_CTL18_SHIFT              (18U)
/*! POL_CTL18 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL18(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL18_SHIFT)) & GPIO_INTPOL_POL_CTL18_MASK)

#define GPIO_INTPOL_POL_CTL19_MASK               (0x80000U)
#define GPIO_INTPOL_POL_CTL19_SHIFT              (19U)
/*! POL_CTL19 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL19(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL19_SHIFT)) & GPIO_INTPOL_POL_CTL19_MASK)

#define GPIO_INTPOL_POL_CTL20_MASK               (0x100000U)
#define GPIO_INTPOL_POL_CTL20_SHIFT              (20U)
/*! POL_CTL20 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL20(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL20_SHIFT)) & GPIO_INTPOL_POL_CTL20_MASK)

#define GPIO_INTPOL_POL_CTL21_MASK               (0x200000U)
#define GPIO_INTPOL_POL_CTL21_SHIFT              (21U)
/*! POL_CTL21 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL21(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL21_SHIFT)) & GPIO_INTPOL_POL_CTL21_MASK)

#define GPIO_INTPOL_POL_CTL22_MASK               (0x400000U)
#define GPIO_INTPOL_POL_CTL22_SHIFT              (22U)
/*! POL_CTL22 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL22(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL22_SHIFT)) & GPIO_INTPOL_POL_CTL22_MASK)

#define GPIO_INTPOL_POL_CTL23_MASK               (0x800000U)
#define GPIO_INTPOL_POL_CTL23_SHIFT              (23U)
/*! POL_CTL23 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL23(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL23_SHIFT)) & GPIO_INTPOL_POL_CTL23_MASK)

#define GPIO_INTPOL_POL_CTL24_MASK               (0x1000000U)
#define GPIO_INTPOL_POL_CTL24_SHIFT              (24U)
/*! POL_CTL24 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL24(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL24_SHIFT)) & GPIO_INTPOL_POL_CTL24_MASK)

#define GPIO_INTPOL_POL_CTL25_MASK               (0x2000000U)
#define GPIO_INTPOL_POL_CTL25_SHIFT              (25U)
/*! POL_CTL25 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL25(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL25_SHIFT)) & GPIO_INTPOL_POL_CTL25_MASK)

#define GPIO_INTPOL_POL_CTL26_MASK               (0x4000000U)
#define GPIO_INTPOL_POL_CTL26_SHIFT              (26U)
/*! POL_CTL26 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL26(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL26_SHIFT)) & GPIO_INTPOL_POL_CTL26_MASK)

#define GPIO_INTPOL_POL_CTL27_MASK               (0x8000000U)
#define GPIO_INTPOL_POL_CTL27_SHIFT              (27U)
/*! POL_CTL27 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL27(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL27_SHIFT)) & GPIO_INTPOL_POL_CTL27_MASK)

#define GPIO_INTPOL_POL_CTL28_MASK               (0x10000000U)
#define GPIO_INTPOL_POL_CTL28_SHIFT              (28U)
/*! POL_CTL28 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL28(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL28_SHIFT)) & GPIO_INTPOL_POL_CTL28_MASK)

#define GPIO_INTPOL_POL_CTL29_MASK               (0x20000000U)
#define GPIO_INTPOL_POL_CTL29_SHIFT              (29U)
/*! POL_CTL29 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL29(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL29_SHIFT)) & GPIO_INTPOL_POL_CTL29_MASK)

#define GPIO_INTPOL_POL_CTL30_MASK               (0x40000000U)
#define GPIO_INTPOL_POL_CTL30_SHIFT              (30U)
/*! POL_CTL30 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL30(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL30_SHIFT)) & GPIO_INTPOL_POL_CTL30_MASK)

#define GPIO_INTPOL_POL_CTL31_MASK               (0x80000000U)
#define GPIO_INTPOL_POL_CTL31_SHIFT              (31U)
/*! POL_CTL31 - Polarity control for each pin
 *  0b0..High level or rising edge triggered
 *  0b1..Low level or falling edge triggered
 */
#define GPIO_INTPOL_POL_CTL31(x)                 (((uint32_t)(((uint32_t)(x)) << GPIO_INTPOL_POL_CTL31_SHIFT)) & GPIO_INTPOL_POL_CTL31_MASK)
/*! @} */

/*! @name INTEDG - Interrupt edge select */
/*! @{ */

#define GPIO_INTEDG_EDGE0_MASK                   (0x1U)
#define GPIO_INTEDG_EDGE0_SHIFT                  (0U)
/*! EDGE0 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE0(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE0_SHIFT)) & GPIO_INTEDG_EDGE0_MASK)

#define GPIO_INTEDG_EDGE1_MASK                   (0x2U)
#define GPIO_INTEDG_EDGE1_SHIFT                  (1U)
/*! EDGE1 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE1(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE1_SHIFT)) & GPIO_INTEDG_EDGE1_MASK)

#define GPIO_INTEDG_EDGE2_MASK                   (0x4U)
#define GPIO_INTEDG_EDGE2_SHIFT                  (2U)
/*! EDGE2 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE2(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE2_SHIFT)) & GPIO_INTEDG_EDGE2_MASK)

#define GPIO_INTEDG_EDGE3_MASK                   (0x8U)
#define GPIO_INTEDG_EDGE3_SHIFT                  (3U)
/*! EDGE3 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE3(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE3_SHIFT)) & GPIO_INTEDG_EDGE3_MASK)

#define GPIO_INTEDG_EDGE4_MASK                   (0x10U)
#define GPIO_INTEDG_EDGE4_SHIFT                  (4U)
/*! EDGE4 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE4(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE4_SHIFT)) & GPIO_INTEDG_EDGE4_MASK)

#define GPIO_INTEDG_EDGE5_MASK                   (0x20U)
#define GPIO_INTEDG_EDGE5_SHIFT                  (5U)
/*! EDGE5 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE5(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE5_SHIFT)) & GPIO_INTEDG_EDGE5_MASK)

#define GPIO_INTEDG_EDGE6_MASK                   (0x40U)
#define GPIO_INTEDG_EDGE6_SHIFT                  (6U)
/*! EDGE6 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE6(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE6_SHIFT)) & GPIO_INTEDG_EDGE6_MASK)

#define GPIO_INTEDG_EDGE7_MASK                   (0x80U)
#define GPIO_INTEDG_EDGE7_SHIFT                  (7U)
/*! EDGE7 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE7(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE7_SHIFT)) & GPIO_INTEDG_EDGE7_MASK)

#define GPIO_INTEDG_EDGE8_MASK                   (0x100U)
#define GPIO_INTEDG_EDGE8_SHIFT                  (8U)
/*! EDGE8 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE8(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE8_SHIFT)) & GPIO_INTEDG_EDGE8_MASK)

#define GPIO_INTEDG_EDGE9_MASK                   (0x200U)
#define GPIO_INTEDG_EDGE9_SHIFT                  (9U)
/*! EDGE9 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE9(x)                     (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE9_SHIFT)) & GPIO_INTEDG_EDGE9_MASK)

#define GPIO_INTEDG_EDGE10_MASK                  (0x400U)
#define GPIO_INTEDG_EDGE10_SHIFT                 (10U)
/*! EDGE10 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE10(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE10_SHIFT)) & GPIO_INTEDG_EDGE10_MASK)

#define GPIO_INTEDG_EDGE11_MASK                  (0x800U)
#define GPIO_INTEDG_EDGE11_SHIFT                 (11U)
/*! EDGE11 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE11(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE11_SHIFT)) & GPIO_INTEDG_EDGE11_MASK)

#define GPIO_INTEDG_EDGE12_MASK                  (0x1000U)
#define GPIO_INTEDG_EDGE12_SHIFT                 (12U)
/*! EDGE12 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE12(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE12_SHIFT)) & GPIO_INTEDG_EDGE12_MASK)

#define GPIO_INTEDG_EDGE13_MASK                  (0x2000U)
#define GPIO_INTEDG_EDGE13_SHIFT                 (13U)
/*! EDGE13 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE13(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE13_SHIFT)) & GPIO_INTEDG_EDGE13_MASK)

#define GPIO_INTEDG_EDGE14_MASK                  (0x4000U)
#define GPIO_INTEDG_EDGE14_SHIFT                 (14U)
/*! EDGE14 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE14(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE14_SHIFT)) & GPIO_INTEDG_EDGE14_MASK)

#define GPIO_INTEDG_EDGE15_MASK                  (0x8000U)
#define GPIO_INTEDG_EDGE15_SHIFT                 (15U)
/*! EDGE15 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE15(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE15_SHIFT)) & GPIO_INTEDG_EDGE15_MASK)

#define GPIO_INTEDG_EDGE16_MASK                  (0x10000U)
#define GPIO_INTEDG_EDGE16_SHIFT                 (16U)
/*! EDGE16 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE16(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE16_SHIFT)) & GPIO_INTEDG_EDGE16_MASK)

#define GPIO_INTEDG_EDGE17_MASK                  (0x20000U)
#define GPIO_INTEDG_EDGE17_SHIFT                 (17U)
/*! EDGE17 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE17(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE17_SHIFT)) & GPIO_INTEDG_EDGE17_MASK)

#define GPIO_INTEDG_EDGE18_MASK                  (0x40000U)
#define GPIO_INTEDG_EDGE18_SHIFT                 (18U)
/*! EDGE18 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE18(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE18_SHIFT)) & GPIO_INTEDG_EDGE18_MASK)

#define GPIO_INTEDG_EDGE19_MASK                  (0x80000U)
#define GPIO_INTEDG_EDGE19_SHIFT                 (19U)
/*! EDGE19 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE19(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE19_SHIFT)) & GPIO_INTEDG_EDGE19_MASK)

#define GPIO_INTEDG_EDGE20_MASK                  (0x100000U)
#define GPIO_INTEDG_EDGE20_SHIFT                 (20U)
/*! EDGE20 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE20(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE20_SHIFT)) & GPIO_INTEDG_EDGE20_MASK)

#define GPIO_INTEDG_EDGE21_MASK                  (0x200000U)
#define GPIO_INTEDG_EDGE21_SHIFT                 (21U)
/*! EDGE21 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE21(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE21_SHIFT)) & GPIO_INTEDG_EDGE21_MASK)

#define GPIO_INTEDG_EDGE22_MASK                  (0x400000U)
#define GPIO_INTEDG_EDGE22_SHIFT                 (22U)
/*! EDGE22 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE22(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE22_SHIFT)) & GPIO_INTEDG_EDGE22_MASK)

#define GPIO_INTEDG_EDGE23_MASK                  (0x800000U)
#define GPIO_INTEDG_EDGE23_SHIFT                 (23U)
/*! EDGE23 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE23(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE23_SHIFT)) & GPIO_INTEDG_EDGE23_MASK)

#define GPIO_INTEDG_EDGE24_MASK                  (0x1000000U)
#define GPIO_INTEDG_EDGE24_SHIFT                 (24U)
/*! EDGE24 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE24(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE24_SHIFT)) & GPIO_INTEDG_EDGE24_MASK)

#define GPIO_INTEDG_EDGE25_MASK                  (0x2000000U)
#define GPIO_INTEDG_EDGE25_SHIFT                 (25U)
/*! EDGE25 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE25(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE25_SHIFT)) & GPIO_INTEDG_EDGE25_MASK)

#define GPIO_INTEDG_EDGE26_MASK                  (0x4000000U)
#define GPIO_INTEDG_EDGE26_SHIFT                 (26U)
/*! EDGE26 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE26(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE26_SHIFT)) & GPIO_INTEDG_EDGE26_MASK)

#define GPIO_INTEDG_EDGE27_MASK                  (0x8000000U)
#define GPIO_INTEDG_EDGE27_SHIFT                 (27U)
/*! EDGE27 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE27(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE27_SHIFT)) & GPIO_INTEDG_EDGE27_MASK)

#define GPIO_INTEDG_EDGE28_MASK                  (0x10000000U)
#define GPIO_INTEDG_EDGE28_SHIFT                 (28U)
/*! EDGE28 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE28(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE28_SHIFT)) & GPIO_INTEDG_EDGE28_MASK)

#define GPIO_INTEDG_EDGE29_MASK                  (0x20000000U)
#define GPIO_INTEDG_EDGE29_SHIFT                 (29U)
/*! EDGE29 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE29(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE29_SHIFT)) & GPIO_INTEDG_EDGE29_MASK)

#define GPIO_INTEDG_EDGE30_MASK                  (0x40000000U)
#define GPIO_INTEDG_EDGE30_SHIFT                 (30U)
/*! EDGE30 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE30(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE30_SHIFT)) & GPIO_INTEDG_EDGE30_MASK)

#define GPIO_INTEDG_EDGE31_MASK                  (0x80000000U)
#define GPIO_INTEDG_EDGE31_SHIFT                 (31U)
/*! EDGE31 - Edge or level mode select bits.
 *  0b0..Level mode
 *  0b1..Edge mode
 */
#define GPIO_INTEDG_EDGE31(x)                    (((uint32_t)(((uint32_t)(x)) << GPIO_INTEDG_EDGE31_SHIFT)) & GPIO_INTEDG_EDGE31_MASK)
/*! @} */

/*! @name INTSTATA - Interrupt status for interrupt A */
/*! @{ */

#define GPIO_INTSTATA_STATUS_MASK                (0xFFFFFFFFU)
#define GPIO_INTSTATA_STATUS_SHIFT               (0U)
/*! STATUS - Interrupt status. */
#define GPIO_INTSTATA_STATUS(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTSTATA_STATUS_SHIFT)) & GPIO_INTSTATA_STATUS_MASK)
/*! @} */

/*! @name INTSTATB - Interrupt status for interrupt B */
/*! @{ */

#define GPIO_INTSTATB_STATUS_MASK                (0xFFFFFFFFU)
#define GPIO_INTSTATB_STATUS_SHIFT               (0U)
/*! STATUS - Interrupt status */
#define GPIO_INTSTATB_STATUS(x)                  (((uint32_t)(((uint32_t)(x)) << GPIO_INTSTATB_STATUS_SHIFT)) & GPIO_INTSTATB_STATUS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group GPIO_Register_Masks */


/*!
 * @}
 */ /* end of group GPIO_Peripheral_Access_Layer */


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


#endif  /* GPIO_H_ */

