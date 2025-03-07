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
**         CMSIS Peripheral Access Layer for DAC
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
 * @file DAC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for DAC
 *
 * CMSIS Peripheral Access Layer for DAC
 */

#if !defined(DAC_H_)
#define DAC_H_                                   /**< Symbol preventing repeated inclusion */

#if (defined(CPU_RW610ETA2I) || defined(CPU_RW610HNA2I) || defined(CPU_RW610UKA2I))
#include "RW610_COMMON.h"
#elif (defined(CPU_RW612ETA2I) || defined(CPU_RW612HNA2I) || defined(CPU_RW612UKA2I))
#include "RW612_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   -- Mapping Information
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Mapping_Information Mapping Information
 * @{
 */

/** Mapping Information */
#if !defined(DAC_CHANNEL_TRIGGER_SOURCE_T_)
#define DAC_CHANNEL_TRIGGER_SOURCE_T_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief The enumeration of dac channel trigger sources.
 */

typedef enum _dac_channel_trigger_source
{
    kDAC_TriggerSourceCtimer1Match2 = 0U,          /**< Use CTimer1 match 2 as trigger source. */
    kDAC_TriggerSourceCtimer1Match1 = 1U,          /**< Use CTimer1 match 1 as trigger source. */
    kDAC_TriggerSourceGpio50        = 2U,          /**< Use GPIO50 as trigger source. */
    kDAC_TriggerSourceGpio55        = 3U,          /**< Use GPIO55 as trigger source. */
} dac_channel_trigger_source_t;
#endif /* DAC_CHANNEL_TRIGGER_SOURCE_T_ */


/*!
 * @}
 */ /* end of group Mapping_Information */


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
   -- DAC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DAC_Peripheral_Access_Layer DAC Peripheral Access Layer
 * @{
 */

/** DAC - Register Layout Typedef */
typedef struct {
  __IO uint32_t CTRL;                              /**< DAC Control Register, offset: 0x0 */
  __I  uint32_t STATUS;                            /**< DAC Status Register, offset: 0x4 */
  __IO uint32_t ACTRL;                             /**< Channel A Control Register, offset: 0x8 */
  __IO uint32_t BCTRL;                             /**< Channel B Control Register, offset: 0xC */
  __IO uint32_t ADATA;                             /**< Channel A Data Register, offset: 0x10 */
  __IO uint32_t BDATA;                             /**< Channel B Data Register, offset: 0x14 */
  __I  uint32_t ISR;                               /**< Interrupt Status Register, offset: 0x18 */
  __IO uint32_t IMR;                               /**< Interrupt Mask Register, offset: 0x1C */
  __I  uint32_t IRSR;                              /**< Interrupt Raw Status Register, offset: 0x20 */
  __IO uint32_t ICR;                               /**< Interrupt Clear Register, offset: 0x24 */
  __IO uint32_t CLK;                               /**< Clock Register, offset: 0x28 */
  __IO uint32_t RST;                               /**< Soft Reset Register, offset: 0x2C */
} DAC_Type;

/* ----------------------------------------------------------------------------
   -- DAC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DAC_Register_Masks DAC Register Masks
 * @{
 */

/*! @name CTRL - DAC Control Register */
/*! @{ */

#define DAC_CTRL_REF_SEL_MASK                    (0x1U)
#define DAC_CTRL_REF_SEL_SHIFT                   (0U)
/*! REF_SEL - Reference selector
 *  0b0..internal reference
 *  0b1..external reference
 */
#define DAC_CTRL_REF_SEL(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_CTRL_REF_SEL_SHIFT)) & DAC_CTRL_REF_SEL_MASK)
/*! @} */

/*! @name STATUS - DAC Status Register */
/*! @{ */

#define DAC_STATUS_A_DV_MASK                     (0x1U)
#define DAC_STATUS_A_DV_SHIFT                    (0U)
/*! A_DV - DACA conversion status.
 *  0b0..channel A conversion is not done
 *  0b1..channel A conversion complete
 */
#define DAC_STATUS_A_DV(x)                       (((uint32_t)(((uint32_t)(x)) << DAC_STATUS_A_DV_SHIFT)) & DAC_STATUS_A_DV_MASK)

#define DAC_STATUS_B_DV_MASK                     (0x2U)
#define DAC_STATUS_B_DV_SHIFT                    (1U)
/*! B_DV - DACB conversion status
 *  0b0..channel B conversion is not done
 *  0b1..channel B conversion complete
 */
#define DAC_STATUS_B_DV(x)                       (((uint32_t)(((uint32_t)(x)) << DAC_STATUS_B_DV_SHIFT)) & DAC_STATUS_B_DV_MASK)
/*! @} */

/*! @name ACTRL - Channel A Control Register */
/*! @{ */

#define DAC_ACTRL_A_EN_MASK                      (0x1U)
#define DAC_ACTRL_A_EN_SHIFT                     (0U)
/*! A_EN - Channel A Enable/Disable signal
 *  0b0..disable channel A conversion
 *  0b1..enable channel A conversion
 */
#define DAC_ACTRL_A_EN(x)                        (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_EN_SHIFT)) & DAC_ACTRL_A_EN_MASK)

#define DAC_ACTRL_A_IO_EN_MASK                   (0x2U)
#define DAC_ACTRL_A_IO_EN_SHIFT                  (1U)
/*! A_IO_EN - Channel A conversion output to pad enable
 *  0b0..disable channel A conversion result to GPIO
 *  0b1..enable channel A conversion result to GPIO
 */
#define DAC_ACTRL_A_IO_EN(x)                     (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_IO_EN_SHIFT)) & DAC_ACTRL_A_IO_EN_MASK)

#define DAC_ACTRL_A_TRIG_EN_MASK                 (0x4U)
#define DAC_ACTRL_A_TRIG_EN_SHIFT                (2U)
/*! A_TRIG_EN - Channel A trigger enable
 *  0b0..Channel A conversion triggered by external event disabled
 *  0b1..Channel A conversion triggered by external event enabled
 */
#define DAC_ACTRL_A_TRIG_EN(x)                   (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIG_EN_SHIFT)) & DAC_ACTRL_A_TRIG_EN_MASK)

#define DAC_ACTRL_A_TRIG_SEL_MASK                (0x18U)
#define DAC_ACTRL_A_TRIG_SEL_SHIFT               (3U)
/*! A_TRIG_SEL - Channel A trigger selector
 *  0b00..ctimer1 match2
 *  0b01..ctimer1 match1
 *  0b10..GPIO50
 *  0b11..GPIO55
 */
#define DAC_ACTRL_A_TRIG_SEL(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIG_SEL_SHIFT)) & DAC_ACTRL_A_TRIG_SEL_MASK)

#define DAC_ACTRL_A_TRIG_TYP_MASK                (0x60U)
#define DAC_ACTRL_A_TRIG_TYP_SHIFT               (5U)
/*! A_TRIG_TYP - Channel A trigger type
 *  0b00..reserved
 *  0b01..rising edge trigger
 *  0b10..falling edge trigger
 *  0b11..both rising and falling edge trigger
 */
#define DAC_ACTRL_A_TRIG_TYP(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIG_TYP_SHIFT)) & DAC_ACTRL_A_TRIG_TYP_MASK)

#define DAC_ACTRL_A_DEN_MASK                     (0x80U)
#define DAC_ACTRL_A_DEN_SHIFT                    (7U)
/*! A_DEN - Channel A DMA enable
 *  0b0..DMA data transfer disabled
 *  0b1..DMA data transfer enabled
 */
#define DAC_ACTRL_A_DEN(x)                       (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_DEN_SHIFT)) & DAC_ACTRL_A_DEN_MASK)

#define DAC_ACTRL_A_TIME_MODE_MASK               (0x100U)
#define DAC_ACTRL_A_TIME_MODE_SHIFT              (8U)
/*! A_TIME_MODE - Channel A Mode
 *  0b0..non-timing related
 *  0b1..timing related
 */
#define DAC_ACTRL_A_TIME_MODE(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TIME_MODE_SHIFT)) & DAC_ACTRL_A_TIME_MODE_MASK)

#define DAC_ACTRL_A_TRIA_HALF_MASK               (0x200U)
#define DAC_ACTRL_A_TRIA_HALF_SHIFT              (9U)
/*! A_TRIA_HALF - Channel A triangle wave type selector.
 *  0b0..full triangle
 *  0b1..half triangle
 */
#define DAC_ACTRL_A_TRIA_HALF(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIA_HALF_SHIFT)) & DAC_ACTRL_A_TRIA_HALF_MASK)

#define DAC_ACTRL_A_TRIA_MAMP_SEL_MASK           (0x3C00U)
#define DAC_ACTRL_A_TRIA_MAMP_SEL_SHIFT          (10U)
/*! A_TRIA_MAMP_SEL - Channel A triangle wave max amplitude selector.
 *  0b0000..63
 *  0b0001..127
 *  0b0010..191
 *  0b0011..255
 *  0b0100..319
 *  0b0101..383
 *  0b0110..447
 *  0b0111..511
 *  0b1000..575
 *  0b1001..639
 *  0b1010..703
 *  0b1011..767
 *  0b1100..831
 *  0b1101..895
 *  0b1110..959
 *  0b1111..1023
 */
#define DAC_ACTRL_A_TRIA_MAMP_SEL(x)             (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIA_MAMP_SEL_SHIFT)) & DAC_ACTRL_A_TRIA_MAMP_SEL_MASK)

#define DAC_ACTRL_A_TRIA_STEP_SEL_MASK           (0xC000U)
#define DAC_ACTRL_A_TRIA_STEP_SEL_SHIFT          (14U)
/*! A_TRIA_STEP_SEL - Channel A triangle wave step selector.
 *  0b00..1
 *  0b01..3
 *  0b10..15
 *  0b11..511
 */
#define DAC_ACTRL_A_TRIA_STEP_SEL(x)             (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_TRIA_STEP_SEL_SHIFT)) & DAC_ACTRL_A_TRIA_STEP_SEL_MASK)

#define DAC_ACTRL_A_WAVE_MASK                    (0x30000U)
#define DAC_ACTRL_A_WAVE_SHIFT                   (16U)
/*! A_WAVE - Channel A wave type select.
 *  0b00..
 *  0b01..triangle wave
 *  0b11..
 *  0b10..sine wave
 */
#define DAC_ACTRL_A_WAVE(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_WAVE_SHIFT)) & DAC_ACTRL_A_WAVE_MASK)

#define DAC_ACTRL_A_RANGE_MASK                   (0xC0000U)
#define DAC_ACTRL_A_RANGE_SHIFT                  (18U)
/*! A_RANGE - Output voltage range control, with Internal/External reference
 *  0b00..0.16+(0.64*input code/1023) with ref_sel=0(internal)/0.08* Vref_ext+(0.32* Vref_ext*input_code/1023) with ref_sel=1(external)
 *  0b01..0.19+(1.01*input code /1023) with ref_sel=0(internal)/0.095* Vref_ext+(0.505* Vref_ext*input_code/1023) with ref_sel=1(external)
 *  0b10..0.19+(1.01*input code /1023) with ref_sel=0(internal)/0.095* Vref_ext+(0.505* Vref_ext*input_code/1023) with ref_sel=1(external)
 *  0b11..0.18+(1.42*input code /1023) with ref_sel=0(internal)/0.09*Vref_ext+(0.71* Vref_ext*input_code/1023) with ref_sel=1(external)
 */
#define DAC_ACTRL_A_RANGE(x)                     (((uint32_t)(((uint32_t)(x)) << DAC_ACTRL_A_RANGE_SHIFT)) & DAC_ACTRL_A_RANGE_MASK)
/*! @} */

/*! @name BCTRL - Channel B Control Register */
/*! @{ */

#define DAC_BCTRL_B_EN_MASK                      (0x1U)
#define DAC_BCTRL_B_EN_SHIFT                     (0U)
/*! B_EN - Channel B Enable/Disable signal
 *  0b0..disable channel B conversion
 *  0b1..enable channel B conversion
 */
#define DAC_BCTRL_B_EN(x)                        (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_EN_SHIFT)) & DAC_BCTRL_B_EN_MASK)

#define DAC_BCTRL_B_IO_EN_MASK                   (0x2U)
#define DAC_BCTRL_B_IO_EN_SHIFT                  (1U)
/*! B_IO_EN - Channel B conversion output to pad enable
 *  0b0..disable channel B conversion result to GPIO
 *  0b1..enable channel B conversion result to GPIO
 */
#define DAC_BCTRL_B_IO_EN(x)                     (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_IO_EN_SHIFT)) & DAC_BCTRL_B_IO_EN_MASK)

#define DAC_BCTRL_B_TRIG_EN_MASK                 (0x4U)
#define DAC_BCTRL_B_TRIG_EN_SHIFT                (2U)
/*! B_TRIG_EN - Channel B trigger enable
 *  0b0..Channel B conversion triggered by external event disabled
 *  0b1..Channel B conversion triggered by external event enabled
 */
#define DAC_BCTRL_B_TRIG_EN(x)                   (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_TRIG_EN_SHIFT)) & DAC_BCTRL_B_TRIG_EN_MASK)

#define DAC_BCTRL_B_TRIG_SEL_MASK                (0x18U)
#define DAC_BCTRL_B_TRIG_SEL_SHIFT               (3U)
/*! B_TRIG_SEL - Channel B trigger selector
 *  0b00..ctimer1 match2
 *  0b01..ctimer1 match1
 *  0b10..GPIO50
 *  0b11..GPIO55
 */
#define DAC_BCTRL_B_TRIG_SEL(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_TRIG_SEL_SHIFT)) & DAC_BCTRL_B_TRIG_SEL_MASK)

#define DAC_BCTRL_B_TRIG_TYP_MASK                (0x60U)
#define DAC_BCTRL_B_TRIG_TYP_SHIFT               (5U)
/*! B_TRIG_TYP - Channel B trigger type
 *  0b00..reserved
 *  0b01..rising edge trigger
 *  0b10..falling edge trigger
 *  0b11..both rising and falling edge trigger
 */
#define DAC_BCTRL_B_TRIG_TYP(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_TRIG_TYP_SHIFT)) & DAC_BCTRL_B_TRIG_TYP_MASK)

#define DAC_BCTRL_B_DEN_MASK                     (0x80U)
#define DAC_BCTRL_B_DEN_SHIFT                    (7U)
/*! B_DEN - Channel B DMA enable
 *  0b0..DMA data transfer disabled
 *  0b1..DMA data transfer enabled
 */
#define DAC_BCTRL_B_DEN(x)                       (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_DEN_SHIFT)) & DAC_BCTRL_B_DEN_MASK)

#define DAC_BCTRL_B_TIME_MODE_MASK               (0x100U)
#define DAC_BCTRL_B_TIME_MODE_SHIFT              (8U)
/*! B_TIME_MODE - Channel B Mode
 *  0b0..non-timing related
 *  0b1..timing related
 */
#define DAC_BCTRL_B_TIME_MODE(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_TIME_MODE_SHIFT)) & DAC_BCTRL_B_TIME_MODE_MASK)

#define DAC_BCTRL_B_WAVE_MASK                    (0x600U)
#define DAC_BCTRL_B_WAVE_SHIFT                   (9U)
/*! B_WAVE - Channel B wave type select.
 *  0b00..
 *  0b01..reserved
 *  0b10..reserved
 *  0b11..differential mode with channel A
 */
#define DAC_BCTRL_B_WAVE(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_BCTRL_B_WAVE_SHIFT)) & DAC_BCTRL_B_WAVE_MASK)
/*! @} */

/*! @name ADATA - Channel A Data Register */
/*! @{ */

#define DAC_ADATA_A_DATA_MASK                    (0x3FFU)
#define DAC_ADATA_A_DATA_SHIFT                   (0U)
/*! A_DATA - Channel A Data input */
#define DAC_ADATA_A_DATA(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_ADATA_A_DATA_SHIFT)) & DAC_ADATA_A_DATA_MASK)
/*! @} */

/*! @name BDATA - Channel B Data Register */
/*! @{ */

#define DAC_BDATA_B_DATA_MASK                    (0x3FFU)
#define DAC_BDATA_B_DATA_SHIFT                   (0U)
/*! B_DATA - Channel B Data input */
#define DAC_BDATA_B_DATA(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_BDATA_B_DATA_SHIFT)) & DAC_BDATA_B_DATA_MASK)
/*! @} */

/*! @name ISR - Interrupt Status Register */
/*! @{ */

#define DAC_ISR_A_RDY_INT_MASK                   (0x1U)
#define DAC_ISR_A_RDY_INT_SHIFT                  (0U)
/*! A_RDY_INT - Channel A Data Ready */
#define DAC_ISR_A_RDY_INT(x)                     (((uint32_t)(((uint32_t)(x)) << DAC_ISR_A_RDY_INT_SHIFT)) & DAC_ISR_A_RDY_INT_MASK)

#define DAC_ISR_B_RDY_INT_MASK                   (0x2U)
#define DAC_ISR_B_RDY_INT_SHIFT                  (1U)
/*! B_RDY_INT - Channel B Data Ready */
#define DAC_ISR_B_RDY_INT(x)                     (((uint32_t)(((uint32_t)(x)) << DAC_ISR_B_RDY_INT_SHIFT)) & DAC_ISR_B_RDY_INT_MASK)

#define DAC_ISR_A_TO_INT_MASK                    (0x4U)
#define DAC_ISR_A_TO_INT_SHIFT                   (2U)
/*! A_TO_INT - Channel A Timeout */
#define DAC_ISR_A_TO_INT(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_ISR_A_TO_INT_SHIFT)) & DAC_ISR_A_TO_INT_MASK)

#define DAC_ISR_B_TO_INT_MASK                    (0x8U)
#define DAC_ISR_B_TO_INT_SHIFT                   (3U)
/*! B_TO_INT - Channel B Timeout */
#define DAC_ISR_B_TO_INT(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_ISR_B_TO_INT_SHIFT)) & DAC_ISR_B_TO_INT_MASK)

#define DAC_ISR_TRIA_OVFL_INT_MASK               (0x10U)
#define DAC_ISR_TRIA_OVFL_INT_SHIFT              (4U)
/*! TRIA_OVFL_INT - Triangle Overflow */
#define DAC_ISR_TRIA_OVFL_INT(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_ISR_TRIA_OVFL_INT_SHIFT)) & DAC_ISR_TRIA_OVFL_INT_MASK)
/*! @} */

/*! @name IMR - Interrupt Mask Register */
/*! @{ */

#define DAC_IMR_A_RDY_INT_MSK_MASK               (0x1U)
#define DAC_IMR_A_RDY_INT_MSK_SHIFT              (0U)
/*! A_RDY_INT_MSK - Channel A Data Ready Mask */
#define DAC_IMR_A_RDY_INT_MSK(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_IMR_A_RDY_INT_MSK_SHIFT)) & DAC_IMR_A_RDY_INT_MSK_MASK)

#define DAC_IMR_B_RDY_INT_MSK_MASK               (0x2U)
#define DAC_IMR_B_RDY_INT_MSK_SHIFT              (1U)
/*! B_RDY_INT_MSK - Channel B Data Ready Mask */
#define DAC_IMR_B_RDY_INT_MSK(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_IMR_B_RDY_INT_MSK_SHIFT)) & DAC_IMR_B_RDY_INT_MSK_MASK)

#define DAC_IMR_A_TO_INT_MSK_MASK                (0x4U)
#define DAC_IMR_A_TO_INT_MSK_SHIFT               (2U)
/*! A_TO_INT_MSK - Channel A Timeout Mask */
#define DAC_IMR_A_TO_INT_MSK(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_IMR_A_TO_INT_MSK_SHIFT)) & DAC_IMR_A_TO_INT_MSK_MASK)

#define DAC_IMR_B_TO_INT_MSK_MASK                (0x8U)
#define DAC_IMR_B_TO_INT_MSK_SHIFT               (3U)
/*! B_TO_INT_MSK - Channel B Timeout Mask */
#define DAC_IMR_B_TO_INT_MSK(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_IMR_B_TO_INT_MSK_SHIFT)) & DAC_IMR_B_TO_INT_MSK_MASK)

#define DAC_IMR_TRIA_OVFL_INT_MSK_MASK           (0x10U)
#define DAC_IMR_TRIA_OVFL_INT_MSK_SHIFT          (4U)
/*! TRIA_OVFL_INT_MSK - Triangle Overflow Mask */
#define DAC_IMR_TRIA_OVFL_INT_MSK(x)             (((uint32_t)(((uint32_t)(x)) << DAC_IMR_TRIA_OVFL_INT_MSK_SHIFT)) & DAC_IMR_TRIA_OVFL_INT_MSK_MASK)
/*! @} */

/*! @name IRSR - Interrupt Raw Status Register */
/*! @{ */

#define DAC_IRSR_A_RDY_INT_RAW_MASK              (0x1U)
#define DAC_IRSR_A_RDY_INT_RAW_SHIFT             (0U)
/*! A_RDY_INT_RAW - Channel A Data Ready Raw */
#define DAC_IRSR_A_RDY_INT_RAW(x)                (((uint32_t)(((uint32_t)(x)) << DAC_IRSR_A_RDY_INT_RAW_SHIFT)) & DAC_IRSR_A_RDY_INT_RAW_MASK)

#define DAC_IRSR_B_RDY_INT_RAW_MASK              (0x2U)
#define DAC_IRSR_B_RDY_INT_RAW_SHIFT             (1U)
/*! B_RDY_INT_RAW - Channel B Data Ready Raw */
#define DAC_IRSR_B_RDY_INT_RAW(x)                (((uint32_t)(((uint32_t)(x)) << DAC_IRSR_B_RDY_INT_RAW_SHIFT)) & DAC_IRSR_B_RDY_INT_RAW_MASK)

#define DAC_IRSR_A_TO_INT_RAW_MASK               (0x4U)
#define DAC_IRSR_A_TO_INT_RAW_SHIFT              (2U)
/*! A_TO_INT_RAW - Channel A Timeout Raw */
#define DAC_IRSR_A_TO_INT_RAW(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_IRSR_A_TO_INT_RAW_SHIFT)) & DAC_IRSR_A_TO_INT_RAW_MASK)

#define DAC_IRSR_B_TO_INT_RAW_MASK               (0x8U)
#define DAC_IRSR_B_TO_INT_RAW_SHIFT              (3U)
/*! B_TO_INT_RAW - Channel B Timeout Raw */
#define DAC_IRSR_B_TO_INT_RAW(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_IRSR_B_TO_INT_RAW_SHIFT)) & DAC_IRSR_B_TO_INT_RAW_MASK)

#define DAC_IRSR_TRIA_OVFL_INT_RAW_MASK          (0x10U)
#define DAC_IRSR_TRIA_OVFL_INT_RAW_SHIFT         (4U)
/*! TRIA_OVFL_INT_RAW - Triangle Overflow Raw */
#define DAC_IRSR_TRIA_OVFL_INT_RAW(x)            (((uint32_t)(((uint32_t)(x)) << DAC_IRSR_TRIA_OVFL_INT_RAW_SHIFT)) & DAC_IRSR_TRIA_OVFL_INT_RAW_MASK)
/*! @} */

/*! @name ICR - Interrupt Clear Register */
/*! @{ */

#define DAC_ICR_A_RDY_INT_CLR_MASK               (0x1U)
#define DAC_ICR_A_RDY_INT_CLR_SHIFT              (0U)
/*! A_RDY_INT_CLR - Channel A Data Ready Clear */
#define DAC_ICR_A_RDY_INT_CLR(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_ICR_A_RDY_INT_CLR_SHIFT)) & DAC_ICR_A_RDY_INT_CLR_MASK)

#define DAC_ICR_B_RDY_INT_CLR_MASK               (0x2U)
#define DAC_ICR_B_RDY_INT_CLR_SHIFT              (1U)
/*! B_RDY_INT_CLR - Channel B Data Ready Clear */
#define DAC_ICR_B_RDY_INT_CLR(x)                 (((uint32_t)(((uint32_t)(x)) << DAC_ICR_B_RDY_INT_CLR_SHIFT)) & DAC_ICR_B_RDY_INT_CLR_MASK)

#define DAC_ICR_A_TO_INT_CLR_MASK                (0x4U)
#define DAC_ICR_A_TO_INT_CLR_SHIFT               (2U)
/*! A_TO_INT_CLR - Channel A Timeout Clear */
#define DAC_ICR_A_TO_INT_CLR(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_ICR_A_TO_INT_CLR_SHIFT)) & DAC_ICR_A_TO_INT_CLR_MASK)

#define DAC_ICR_B_TO_INT_CLR_MASK                (0x8U)
#define DAC_ICR_B_TO_INT_CLR_SHIFT               (3U)
/*! B_TO_INT_CLR - Channel B Timeout Clear */
#define DAC_ICR_B_TO_INT_CLR(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_ICR_B_TO_INT_CLR_SHIFT)) & DAC_ICR_B_TO_INT_CLR_MASK)

#define DAC_ICR_TRIA_OVFL_INT_CLR_MASK           (0x10U)
#define DAC_ICR_TRIA_OVFL_INT_CLR_SHIFT          (4U)
/*! TRIA_OVFL_INT_CLR - Triangle Overflow Clear */
#define DAC_ICR_TRIA_OVFL_INT_CLR(x)             (((uint32_t)(((uint32_t)(x)) << DAC_ICR_TRIA_OVFL_INT_CLR_SHIFT)) & DAC_ICR_TRIA_OVFL_INT_CLR_MASK)
/*! @} */

/*! @name CLK - Clock Register */
/*! @{ */

#define DAC_CLK_CLK_CTRL_MASK                    (0x6U)
#define DAC_CLK_CLK_CTRL_SHIFT                   (1U)
/*! CLK_CTRL - DAC conversion rate selector.
 *  0b00..62.5K
 *  0b01..125K
 *  0b10..250K
 *  0b11..500K
 */
#define DAC_CLK_CLK_CTRL(x)                      (((uint32_t)(((uint32_t)(x)) << DAC_CLK_CLK_CTRL_SHIFT)) & DAC_CLK_CLK_CTRL_MASK)

#define DAC_CLK_SOFT_CLK_RST_MASK                (0x10U)
#define DAC_CLK_SOFT_CLK_RST_SHIFT               (4U)
/*! SOFT_CLK_RST - Soft reset for clock divider
 *  0b0..
 *  0b1..
 */
#define DAC_CLK_SOFT_CLK_RST(x)                  (((uint32_t)(((uint32_t)(x)) << DAC_CLK_SOFT_CLK_RST_SHIFT)) & DAC_CLK_SOFT_CLK_RST_MASK)
/*! @} */

/*! @name RST - Soft Reset Register */
/*! @{ */

#define DAC_RST_A_SOFT_RST_MASK                  (0x1U)
#define DAC_RST_A_SOFT_RST_SHIFT                 (0U)
/*! A_SOFT_RST - Soft reset for DAC channel A, active high
 *  0b0..no action
 *  0b1..
 */
#define DAC_RST_A_SOFT_RST(x)                    (((uint32_t)(((uint32_t)(x)) << DAC_RST_A_SOFT_RST_SHIFT)) & DAC_RST_A_SOFT_RST_MASK)

#define DAC_RST_B_SOFT_RST_MASK                  (0x2U)
#define DAC_RST_B_SOFT_RST_SHIFT                 (1U)
/*! B_SOFT_RST - Soft reset for DAC channel B, active high
 *  0b0..no action
 *  0b1..
 */
#define DAC_RST_B_SOFT_RST(x)                    (((uint32_t)(((uint32_t)(x)) << DAC_RST_B_SOFT_RST_SHIFT)) & DAC_RST_B_SOFT_RST_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group DAC_Register_Masks */

/* Backward compatibility */
#define kDAC_GPT2Trigger                         kDAC_TriggerSourceCtimer1Match2
#define kDAC_GPT3Trigger                         kDAC_TriggerSourceCtimer1Match1
#define kDAC_GPIO45Trigger                       kDAC_TriggerSourceGpio50
#define kDAC_GPIO44Trigger                       kDAC_TriggerSourceGpio55


/*!
 * @}
 */ /* end of group DAC_Peripheral_Access_Layer */


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


#endif  /* DAC_H_ */

