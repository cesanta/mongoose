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
**         CMSIS Peripheral Access Layer for ADC
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
 * @file ADC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for ADC
 *
 * CMSIS Peripheral Access Layer for ADC
 */

#if !defined(ADC_H_)
#define ADC_H_                                   /**< Symbol preventing repeated inclusion */

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
#if !defined(ADC_TRIGGER_SOURCE_T_)
#define ADC_TRIGGER_SOURCE_T_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief ADC trigger source, including software trigger and multiple hardware trigger sources.
 */

typedef enum _adc_trigger_source
{
    kADC_TriggerSourceCtimer        = 0U,          /**< Hardware trigger, trigger source 0:CTimer3_match1 for ADC0, CTimer3_match2 for ADC1. */
    kADC_TriggerSourceAcomp         = 1U,          /**< Hardware trigger, trigger source 1: ACOMP0 for ADC0, ACMP1 for ADC1. */
    kADC_TriggerSourceGpio50        = 2U,          /**< Hardware trigger, trigger source 2: GPIO50. */
    kADC_TriggerSourceGpio55        = 3U,          /**< Hardware trigger, trigger source 3: GPIO55. */
    kADC_TriggerSourceSoftware      = 4U,          /**< Software trigger. */
} adc_trigger_source_t;
#endif /* ADC_TRIGGER_SOURCE_T_ */


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
   -- ADC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Peripheral_Access_Layer ADC Peripheral Access Layer
 * @{
 */

/** ADC - Register Layout Typedef */
typedef struct {
  __IO uint32_t ADC_REG_CMD;                       /**< ADC Command Register, offset: 0x0 */
  __IO uint32_t ADC_REG_GENERAL;                   /**< ADC General Register, offset: 0x4 */
  __IO uint32_t ADC_REG_CONFIG;                    /**< ADC Configuration Register, offset: 0x8 */
  __IO uint32_t ADC_REG_INTERVAL;                  /**< ADC Interval Register, offset: 0xC */
  __IO uint32_t ADC_REG_ANA;                       /**< ADC ANA Register, offset: 0x10 */
       uint8_t RESERVED_0[4];
  __IO uint32_t ADC_REG_SCN1;                      /**< ADC Conversion Sequence 1 Register, offset: 0x18 */
  __IO uint32_t ADC_REG_SCN2;                      /**< ADC Conversion Sequence 2 Register, offset: 0x1C */
  __IO uint32_t ADC_REG_RESULT_BUF;                /**< ADC Result Buffer Register, offset: 0x20 */
       uint8_t RESERVED_1[4];
  __IO uint32_t ADC_REG_DMAR;                      /**< ADC DMAR Register, offset: 0x28 */
  __I  uint32_t ADC_REG_STATUS;                    /**< ADC Status Register, offset: 0x2C */
  __I  uint32_t ADC_REG_ISR;                       /**< ADC ISR Register, offset: 0x30 */
  __IO uint32_t ADC_REG_IMR;                       /**< ADC IMR Register, offset: 0x34 */
  __I  uint32_t ADC_REG_IRSR;                      /**< ADC IRSR Register, offset: 0x38 */
  __IO uint32_t ADC_REG_ICR;                       /**< ADC ICR Register, offset: 0x3C */
       uint8_t RESERVED_2[4];
  __I  uint32_t ADC_REG_RESULT;                    /**< ADC Result Register, offset: 0x44 */
  __I  uint32_t ADC_REG_RAW_RESULT;                /**< ADC Raw Result Register, offset: 0x48 */
  __IO uint32_t ADC_REG_OFFSET_CAL;                /**< ADC Offset Calibration Register, offset: 0x4C */
  __IO uint32_t ADC_REG_GAIN_CAL;                  /**< ADC Gain Calibration Register, offset: 0x50 */
  __IO uint32_t ADC_REG_TEST;                      /**< ADC Test Register, offset: 0x54 */
  __IO uint32_t ADC_REG_AUDIO;                     /**< ADC Audio Register, offset: 0x58 */
  __IO uint32_t ADC_REG_VOICE_DET;                 /**< ADC Voice Detect Register, offset: 0x5C */
  __IO uint32_t ADC_REG_RSVD;                      /**< ADC Reserved Register, offset: 0x60 */
} ADC_Type;

/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/*! @name ADC_REG_CMD - ADC Command Register */
/*! @{ */

#define ADC_ADC_REG_CMD_CONV_START_MASK          (0x1U)
#define ADC_ADC_REG_CMD_CONV_START_SHIFT         (0U)
/*! CONV_START - converaion control bit. */
#define ADC_ADC_REG_CMD_CONV_START(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CMD_CONV_START_SHIFT)) & ADC_ADC_REG_CMD_CONV_START_MASK)

#define ADC_ADC_REG_CMD_SOFT_RST_MASK            (0x2U)
#define ADC_ADC_REG_CMD_SOFT_RST_SHIFT           (1U)
/*! SOFT_RST - user reset the whole block */
#define ADC_ADC_REG_CMD_SOFT_RST(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CMD_SOFT_RST_SHIFT)) & ADC_ADC_REG_CMD_SOFT_RST_MASK)

#define ADC_ADC_REG_CMD_SOFT_CLK_RST_MASK        (0x4U)
#define ADC_ADC_REG_CMD_SOFT_CLK_RST_SHIFT       (2U)
/*! SOFT_CLK_RST - user reset clock */
#define ADC_ADC_REG_CMD_SOFT_CLK_RST(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CMD_SOFT_CLK_RST_SHIFT)) & ADC_ADC_REG_CMD_SOFT_CLK_RST_MASK)
/*! @} */

/*! @name ADC_REG_GENERAL - ADC General Register */
/*! @{ */

#define ADC_ADC_REG_GENERAL_GPADC_MASTER_MASK    (0x1U)
#define ADC_ADC_REG_GENERAL_GPADC_MASTER_SHIFT   (0U)
/*! GPADC_MASTER - play as master or slave in dual mode, master is the only controller of when slave start */
#define ADC_ADC_REG_GENERAL_GPADC_MASTER(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_GPADC_MASTER_SHIFT)) & ADC_ADC_REG_GENERAL_GPADC_MASTER_MASK)

#define ADC_ADC_REG_GENERAL_GLOBAL_EN_MASK       (0x2U)
#define ADC_ADC_REG_GENERAL_GLOBAL_EN_SHIFT      (1U)
/*! GLOBAL_EN - ADC enable/disable */
#define ADC_ADC_REG_GENERAL_GLOBAL_EN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_GLOBAL_EN_SHIFT)) & ADC_ADC_REG_GENERAL_GLOBAL_EN_MASK)

#define ADC_ADC_REG_GENERAL_FORCE_CLK_ON_MASK    (0x4U)
#define ADC_ADC_REG_GENERAL_FORCE_CLK_ON_SHIFT   (2U)
/*! FORCE_CLK_ON - override the gpadc_mclk_en from outside */
#define ADC_ADC_REG_GENERAL_FORCE_CLK_ON(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_FORCE_CLK_ON_SHIFT)) & ADC_ADC_REG_GENERAL_FORCE_CLK_ON_MASK)

#define ADC_ADC_REG_GENERAL_CLK_ANA64M_INV_MASK  (0x8U)
#define ADC_ADC_REG_GENERAL_CLK_ANA64M_INV_SHIFT (3U)
/*! CLK_ANA64M_INV - analog clock 64M inverted */
#define ADC_ADC_REG_GENERAL_CLK_ANA64M_INV(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_CLK_ANA64M_INV_SHIFT)) & ADC_ADC_REG_GENERAL_CLK_ANA64M_INV_MASK)

#define ADC_ADC_REG_GENERAL_CLK_ANA2M_INV_MASK   (0x10U)
#define ADC_ADC_REG_GENERAL_CLK_ANA2M_INV_SHIFT  (4U)
/*! CLK_ANA2M_INV - analog clock 2M inverted */
#define ADC_ADC_REG_GENERAL_CLK_ANA2M_INV(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_CLK_ANA2M_INV_SHIFT)) & ADC_ADC_REG_GENERAL_CLK_ANA2M_INV_MASK)

#define ADC_ADC_REG_GENERAL_ADC_CAL_EN_MASK      (0x20U)
#define ADC_ADC_REG_GENERAL_ADC_CAL_EN_SHIFT     (5U)
/*! ADC_CAL_EN - calibration enable, auto cleared after calibration done */
#define ADC_ADC_REG_GENERAL_ADC_CAL_EN(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_ADC_CAL_EN_SHIFT)) & ADC_ADC_REG_GENERAL_ADC_CAL_EN_MASK)

#define ADC_ADC_REG_GENERAL_CLK_DIV_RATIO_MASK   (0x3F00U)
#define ADC_ADC_REG_GENERAL_CLK_DIV_RATIO_SHIFT  (8U)
/*! CLK_DIV_RATIO - analog 64M clock division ratio */
#define ADC_ADC_REG_GENERAL_CLK_DIV_RATIO(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GENERAL_CLK_DIV_RATIO_SHIFT)) & ADC_ADC_REG_GENERAL_CLK_DIV_RATIO_MASK)
/*! @} */

/*! @name ADC_REG_CONFIG - ADC Configuration Register */
/*! @{ */

#define ADC_ADC_REG_CONFIG_TRIGGER_SEL_MASK      (0xFU)
#define ADC_ADC_REG_CONFIG_TRIGGER_SEL_SHIFT     (0U)
/*! TRIGGER_SEL - External trigger source select bits
 *  0b0000..ctimer3 match1 in ADC0 module, ctimer3 match2 in ADC1 module
 *  0b0001..acomparator 0 out in ADC0 module, acomparator 1 out in ADC1 module
 *  0b0010..GPIO50
 *  0b0011..GPIO55
 */
#define ADC_ADC_REG_CONFIG_TRIGGER_SEL(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_TRIGGER_SEL_SHIFT)) & ADC_ADC_REG_CONFIG_TRIGGER_SEL_MASK)

#define ADC_ADC_REG_CONFIG_TRIGGER_EN_MASK       (0x10U)
#define ADC_ADC_REG_CONFIG_TRIGGER_EN_SHIFT      (4U)
/*! TRIGGER_EN - External elevel trigger enable bit, support gpadc_trigger/gpadc_data_valid handshake */
#define ADC_ADC_REG_CONFIG_TRIGGER_EN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_TRIGGER_EN_SHIFT)) & ADC_ADC_REG_CONFIG_TRIGGER_EN_MASK)

#define ADC_ADC_REG_CONFIG_DUAL_MODE_MASK        (0x20U)
#define ADC_ADC_REG_CONFIG_DUAL_MODE_SHIFT       (5U)
/*! DUAL_MODE - dual mode select */
#define ADC_ADC_REG_CONFIG_DUAL_MODE(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_DUAL_MODE_SHIFT)) & ADC_ADC_REG_CONFIG_DUAL_MODE_MASK)

#define ADC_ADC_REG_CONFIG_SINGLE_DUAL_MASK      (0x40U)
#define ADC_ADC_REG_CONFIG_SINGLE_DUAL_SHIFT     (6U)
/*! SINGLE_DUAL - work mode select */
#define ADC_ADC_REG_CONFIG_SINGLE_DUAL(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_SINGLE_DUAL_SHIFT)) & ADC_ADC_REG_CONFIG_SINGLE_DUAL_MASK)

#define ADC_ADC_REG_CONFIG_CONT_CONV_EN_MASK     (0x100U)
#define ADC_ADC_REG_CONFIG_CONT_CONV_EN_SHIFT    (8U)
/*! CONT_CONV_EN - To enable continuous conversion */
#define ADC_ADC_REG_CONFIG_CONT_CONV_EN(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_CONT_CONV_EN_SHIFT)) & ADC_ADC_REG_CONFIG_CONT_CONV_EN_MASK)

#define ADC_ADC_REG_CONFIG_DATA_FORMAT_SEL_MASK  (0x200U)
#define ADC_ADC_REG_CONFIG_DATA_FORMAT_SEL_SHIFT (9U)
/*! DATA_FORMAT_SEL - set data format for the final data */
#define ADC_ADC_REG_CONFIG_DATA_FORMAT_SEL(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_DATA_FORMAT_SEL_SHIFT)) & ADC_ADC_REG_CONFIG_DATA_FORMAT_SEL_MASK)

#define ADC_ADC_REG_CONFIG_CAL_VREF_SEL_MASK     (0x400U)
#define ADC_ADC_REG_CONFIG_CAL_VREF_SEL_SHIFT    (10U)
/*! CAL_VREF_SEL - select input reference channel for gain calibration */
#define ADC_ADC_REG_CONFIG_CAL_VREF_SEL(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_CAL_VREF_SEL_SHIFT)) & ADC_ADC_REG_CONFIG_CAL_VREF_SEL_MASK)

#define ADC_ADC_REG_CONFIG_CAL_DATA_RST_MASK     (0x800U)
#define ADC_ADC_REG_CONFIG_CAL_DATA_RST_SHIFT    (11U)
/*! CAL_DATA_RST - reset the self calibration data. */
#define ADC_ADC_REG_CONFIG_CAL_DATA_RST(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_CAL_DATA_RST_SHIFT)) & ADC_ADC_REG_CONFIG_CAL_DATA_RST_MASK)

#define ADC_ADC_REG_CONFIG_CAL_DATA_SEL_MASK     (0x1000U)
#define ADC_ADC_REG_CONFIG_CAL_DATA_SEL_SHIFT    (12U)
/*! CAL_DATA_SEL - select calibration data source. */
#define ADC_ADC_REG_CONFIG_CAL_DATA_SEL(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_CAL_DATA_SEL_SHIFT)) & ADC_ADC_REG_CONFIG_CAL_DATA_SEL_MASK)

#define ADC_ADC_REG_CONFIG_AVG_SEL_MASK          (0xE000U)
#define ADC_ADC_REG_CONFIG_AVG_SEL_SHIFT         (13U)
/*! AVG_SEL - moving average length */
#define ADC_ADC_REG_CONFIG_AVG_SEL(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_AVG_SEL_SHIFT)) & ADC_ADC_REG_CONFIG_AVG_SEL_MASK)

#define ADC_ADC_REG_CONFIG_SCAN_LENGTH_MASK      (0xF0000U)
#define ADC_ADC_REG_CONFIG_SCAN_LENGTH_SHIFT     (16U)
/*! SCAN_LENGTH - scan converation length, actual length is scan_length+1. */
#define ADC_ADC_REG_CONFIG_SCAN_LENGTH(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_SCAN_LENGTH_SHIFT)) & ADC_ADC_REG_CONFIG_SCAN_LENGTH_MASK)

#define ADC_ADC_REG_CONFIG_PWR_MODE_MASK         (0x100000U)
#define ADC_ADC_REG_CONFIG_PWR_MODE_SHIFT        (20U)
/*! PWR_MODE - ADC power mode select. */
#define ADC_ADC_REG_CONFIG_PWR_MODE(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_CONFIG_PWR_MODE_SHIFT)) & ADC_ADC_REG_CONFIG_PWR_MODE_MASK)
/*! @} */

/*! @name ADC_REG_INTERVAL - ADC Interval Register */
/*! @{ */

#define ADC_ADC_REG_INTERVAL_WARMUP_TIME_MASK    (0x1FU)
#define ADC_ADC_REG_INTERVAL_WARMUP_TIME_SHIFT   (0U)
/*! WARMUP_TIME - warmup time, should be set equal to or higher than 1uS. */
#define ADC_ADC_REG_INTERVAL_WARMUP_TIME(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_INTERVAL_WARMUP_TIME_SHIFT)) & ADC_ADC_REG_INTERVAL_WARMUP_TIME_MASK)

#define ADC_ADC_REG_INTERVAL_BYPASS_WARMUP_MASK  (0x20U)
#define ADC_ADC_REG_INTERVAL_BYPASS_WARMUP_SHIFT (5U)
/*! BYPASS_WARMUP - Bypass warmup state inside adc. */
#define ADC_ADC_REG_INTERVAL_BYPASS_WARMUP(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_INTERVAL_BYPASS_WARMUP_SHIFT)) & ADC_ADC_REG_INTERVAL_BYPASS_WARMUP_MASK)
/*! @} */

/*! @name ADC_REG_ANA - ADC ANA Register */
/*! @{ */

#define ADC_ADC_REG_ANA_TSEXT_SEL_MASK           (0x1U)
#define ADC_ADC_REG_ANA_TSEXT_SEL_SHIFT          (0U)
/*! TSEXT_SEL - temperature sensor diode select */
#define ADC_ADC_REG_ANA_TSEXT_SEL(x)             (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_TSEXT_SEL_SHIFT)) & ADC_ADC_REG_ANA_TSEXT_SEL_MASK)

#define ADC_ADC_REG_ANA_TS_EN_MASK               (0x2U)
#define ADC_ADC_REG_ANA_TS_EN_SHIFT              (1U)
/*! TS_EN - temperature sensor enable, only enable when channel source is temperature sensor */
#define ADC_ADC_REG_ANA_TS_EN(x)                 (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_TS_EN_SHIFT)) & ADC_ADC_REG_ANA_TS_EN_MASK)

#define ADC_ADC_REG_ANA_VREF_SCF_BYPASS_MASK     (0x4U)
#define ADC_ADC_REG_ANA_VREF_SCF_BYPASS_SHIFT    (2U)
/*! VREF_SCF_BYPASS - adc voltage reference buffer sc-filter bypass */
#define ADC_ADC_REG_ANA_VREF_SCF_BYPASS(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_VREF_SCF_BYPASS_SHIFT)) & ADC_ADC_REG_ANA_VREF_SCF_BYPASS_MASK)

#define ADC_ADC_REG_ANA_VREF_CHOP_EN_MASK        (0x8U)
#define ADC_ADC_REG_ANA_VREF_CHOP_EN_SHIFT       (3U)
/*! VREF_CHOP_EN - adc voltage reference buffer chopper enable */
#define ADC_ADC_REG_ANA_VREF_CHOP_EN(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_VREF_CHOP_EN_SHIFT)) & ADC_ADC_REG_ANA_VREF_CHOP_EN_MASK)

#define ADC_ADC_REG_ANA_VREF_SEL_MASK            (0x30U)
#define ADC_ADC_REG_ANA_VREF_SEL_SHIFT           (4U)
/*! VREF_SEL - adc reference voltage select. */
#define ADC_ADC_REG_ANA_VREF_SEL(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_VREF_SEL_SHIFT)) & ADC_ADC_REG_ANA_VREF_SEL_MASK)

#define ADC_ADC_REG_ANA_SINGLEDIFF_MASK          (0x400U)
#define ADC_ADC_REG_ANA_SINGLEDIFF_SHIFT         (10U)
/*! SINGLEDIFF - Select single ended or differential input. */
#define ADC_ADC_REG_ANA_SINGLEDIFF(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_SINGLEDIFF_SHIFT)) & ADC_ADC_REG_ANA_SINGLEDIFF_MASK)

#define ADC_ADC_REG_ANA_INBUF_GAIN_MASK          (0x1800U)
#define ADC_ADC_REG_ANA_INBUF_GAIN_SHIFT         (11U)
/*! INBUF_GAIN - adc gain control. Also select input voltage range. */
#define ADC_ADC_REG_ANA_INBUF_GAIN(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_INBUF_GAIN_SHIFT)) & ADC_ADC_REG_ANA_INBUF_GAIN_MASK)

#define ADC_ADC_REG_ANA_INBUF_CHOP_EN_MASK       (0x2000U)
#define ADC_ADC_REG_ANA_INBUF_CHOP_EN_SHIFT      (13U)
/*! INBUF_CHOP_EN - Input buffer chopper enable */
#define ADC_ADC_REG_ANA_INBUF_CHOP_EN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_INBUF_CHOP_EN_SHIFT)) & ADC_ADC_REG_ANA_INBUF_CHOP_EN_MASK)

#define ADC_ADC_REG_ANA_INBUF_EN_MASK            (0x4000U)
#define ADC_ADC_REG_ANA_INBUF_EN_SHIFT           (14U)
/*! INBUF_EN - gpadc input gain buffer enable bit. */
#define ADC_ADC_REG_ANA_INBUF_EN(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_INBUF_EN_SHIFT)) & ADC_ADC_REG_ANA_INBUF_EN_MASK)

#define ADC_ADC_REG_ANA_CHOP_EN_MASK             (0x8000U)
#define ADC_ADC_REG_ANA_CHOP_EN_SHIFT            (15U)
/*! CHOP_EN - adc chopper/auto-zero(only in 12bit mode) enable */
#define ADC_ADC_REG_ANA_CHOP_EN(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_CHOP_EN_SHIFT)) & ADC_ADC_REG_ANA_CHOP_EN_MASK)

#define ADC_ADC_REG_ANA_BIAS_SEL_MASK            (0x10000U)
#define ADC_ADC_REG_ANA_BIAS_SEL_SHIFT           (16U)
/*! BIAS_SEL - adc analog portion low power mode select. Half the biasing current for modulator when enabled. */
#define ADC_ADC_REG_ANA_BIAS_SEL(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_BIAS_SEL_SHIFT)) & ADC_ADC_REG_ANA_BIAS_SEL_MASK)

#define ADC_ADC_REG_ANA_RES_SEL_MASK             (0x60000U)
#define ADC_ADC_REG_ANA_RES_SEL_SHIFT            (17U)
/*! RES_SEL - adc resolution/data rate select */
#define ADC_ADC_REG_ANA_RES_SEL(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ANA_RES_SEL_SHIFT)) & ADC_ADC_REG_ANA_RES_SEL_MASK)
/*! @} */

/*! @name ADC_REG_SCN1 - ADC Conversion Sequence 1 Register */
/*! @{ */

#define ADC_ADC_REG_SCN1_SCAN_CH_0_MASK          (0xFU)
#define ADC_ADC_REG_SCN1_SCAN_CH_0_SHIFT         (0U)
/*! SCAN_CH_0 - amux source 0 */
#define ADC_ADC_REG_SCN1_SCAN_CH_0(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_0_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_0_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_1_MASK          (0xF0U)
#define ADC_ADC_REG_SCN1_SCAN_CH_1_SHIFT         (4U)
/*! SCAN_CH_1 - amux source 1 */
#define ADC_ADC_REG_SCN1_SCAN_CH_1(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_1_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_1_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_2_MASK          (0xF00U)
#define ADC_ADC_REG_SCN1_SCAN_CH_2_SHIFT         (8U)
/*! SCAN_CH_2 - amux source 2 */
#define ADC_ADC_REG_SCN1_SCAN_CH_2(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_2_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_2_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_3_MASK          (0xF000U)
#define ADC_ADC_REG_SCN1_SCAN_CH_3_SHIFT         (12U)
/*! SCAN_CH_3 - amux source 3 */
#define ADC_ADC_REG_SCN1_SCAN_CH_3(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_3_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_3_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_4_MASK          (0xF0000U)
#define ADC_ADC_REG_SCN1_SCAN_CH_4_SHIFT         (16U)
/*! SCAN_CH_4 - amux source 4 */
#define ADC_ADC_REG_SCN1_SCAN_CH_4(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_4_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_4_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_5_MASK          (0xF00000U)
#define ADC_ADC_REG_SCN1_SCAN_CH_5_SHIFT         (20U)
/*! SCAN_CH_5 - amux source 5 */
#define ADC_ADC_REG_SCN1_SCAN_CH_5(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_5_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_5_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_6_MASK          (0xF000000U)
#define ADC_ADC_REG_SCN1_SCAN_CH_6_SHIFT         (24U)
/*! SCAN_CH_6 - amux source 6 */
#define ADC_ADC_REG_SCN1_SCAN_CH_6(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_6_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_6_MASK)

#define ADC_ADC_REG_SCN1_SCAN_CH_7_MASK          (0xF0000000U)
#define ADC_ADC_REG_SCN1_SCAN_CH_7_SHIFT         (28U)
/*! SCAN_CH_7 - amux source 7 */
#define ADC_ADC_REG_SCN1_SCAN_CH_7(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN1_SCAN_CH_7_SHIFT)) & ADC_ADC_REG_SCN1_SCAN_CH_7_MASK)
/*! @} */

/*! @name ADC_REG_SCN2 - ADC Conversion Sequence 2 Register */
/*! @{ */

#define ADC_ADC_REG_SCN2_SCAN_CH_8_MASK          (0xFU)
#define ADC_ADC_REG_SCN2_SCAN_CH_8_SHIFT         (0U)
/*! SCAN_CH_8 - amux source 8 */
#define ADC_ADC_REG_SCN2_SCAN_CH_8(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_8_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_8_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_9_MASK          (0xF0U)
#define ADC_ADC_REG_SCN2_SCAN_CH_9_SHIFT         (4U)
/*! SCAN_CH_9 - amux source 9 */
#define ADC_ADC_REG_SCN2_SCAN_CH_9(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_9_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_9_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_10_MASK         (0xF00U)
#define ADC_ADC_REG_SCN2_SCAN_CH_10_SHIFT        (8U)
/*! SCAN_CH_10 - amux source 10 */
#define ADC_ADC_REG_SCN2_SCAN_CH_10(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_10_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_10_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_11_MASK         (0xF000U)
#define ADC_ADC_REG_SCN2_SCAN_CH_11_SHIFT        (12U)
/*! SCAN_CH_11 - amux source 11 */
#define ADC_ADC_REG_SCN2_SCAN_CH_11(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_11_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_11_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_12_MASK         (0xF0000U)
#define ADC_ADC_REG_SCN2_SCAN_CH_12_SHIFT        (16U)
/*! SCAN_CH_12 - amux source 12 */
#define ADC_ADC_REG_SCN2_SCAN_CH_12(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_12_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_12_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_13_MASK         (0xF00000U)
#define ADC_ADC_REG_SCN2_SCAN_CH_13_SHIFT        (20U)
/*! SCAN_CH_13 - amux source 13 */
#define ADC_ADC_REG_SCN2_SCAN_CH_13(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_13_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_13_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_14_MASK         (0xF000000U)
#define ADC_ADC_REG_SCN2_SCAN_CH_14_SHIFT        (24U)
/*! SCAN_CH_14 - amux source 14 */
#define ADC_ADC_REG_SCN2_SCAN_CH_14(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_14_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_14_MASK)

#define ADC_ADC_REG_SCN2_SCAN_CH_15_MASK         (0xF0000000U)
#define ADC_ADC_REG_SCN2_SCAN_CH_15_SHIFT        (28U)
/*! SCAN_CH_15 - amux source 15 */
#define ADC_ADC_REG_SCN2_SCAN_CH_15(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_SCN2_SCAN_CH_15_SHIFT)) & ADC_ADC_REG_SCN2_SCAN_CH_15_MASK)
/*! @} */

/*! @name ADC_REG_RESULT_BUF - ADC Result Buffer Register */
/*! @{ */

#define ADC_ADC_REG_RESULT_BUF_WIDTH_SEL_MASK    (0x1U)
#define ADC_ADC_REG_RESULT_BUF_WIDTH_SEL_SHIFT   (0U)
/*! WIDTH_SEL - adc finial result fifo data packed format select, must set scan_length as even when choose 32-bits */
#define ADC_ADC_REG_RESULT_BUF_WIDTH_SEL(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_RESULT_BUF_WIDTH_SEL_SHIFT)) & ADC_ADC_REG_RESULT_BUF_WIDTH_SEL_MASK)
/*! @} */

/*! @name ADC_REG_DMAR - ADC DMAR Register */
/*! @{ */

#define ADC_ADC_REG_DMAR_DMA_EN_MASK             (0x1U)
#define ADC_ADC_REG_DMAR_DMA_EN_SHIFT            (0U)
/*! DMA_EN - dma enbale */
#define ADC_ADC_REG_DMAR_DMA_EN(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_DMAR_DMA_EN_SHIFT)) & ADC_ADC_REG_DMAR_DMA_EN_MASK)

#define ADC_ADC_REG_DMAR_FIFO_THL_MASK           (0x6U)
#define ADC_ADC_REG_DMAR_FIFO_THL_SHIFT          (1U)
/*! FIFO_THL - fifo threshold */
#define ADC_ADC_REG_DMAR_FIFO_THL(x)             (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_DMAR_FIFO_THL_SHIFT)) & ADC_ADC_REG_DMAR_FIFO_THL_MASK)
/*! @} */

/*! @name ADC_REG_STATUS - ADC Status Register */
/*! @{ */

#define ADC_ADC_REG_STATUS_ACT_MASK              (0x1U)
#define ADC_ADC_REG_STATUS_ACT_SHIFT             (0U)
/*! ACT - adc status */
#define ADC_ADC_REG_STATUS_ACT(x)                (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_STATUS_ACT_SHIFT)) & ADC_ADC_REG_STATUS_ACT_MASK)

#define ADC_ADC_REG_STATUS_FIFO_NE_MASK          (0x2U)
#define ADC_ADC_REG_STATUS_FIFO_NE_SHIFT         (1U)
/*! FIFO_NE - fifo not empty status */
#define ADC_ADC_REG_STATUS_FIFO_NE(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_STATUS_FIFO_NE_SHIFT)) & ADC_ADC_REG_STATUS_FIFO_NE_MASK)

#define ADC_ADC_REG_STATUS_FIFO_FULL_MASK        (0x4U)
#define ADC_ADC_REG_STATUS_FIFO_FULL_SHIFT       (2U)
/*! FIFO_FULL - fifo full status */
#define ADC_ADC_REG_STATUS_FIFO_FULL(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_STATUS_FIFO_FULL_SHIFT)) & ADC_ADC_REG_STATUS_FIFO_FULL_MASK)

#define ADC_ADC_REG_STATUS_FIFO_DATA_COUNT_MASK  (0x1F8U)
#define ADC_ADC_REG_STATUS_FIFO_DATA_COUNT_SHIFT (3U)
/*! FIFO_DATA_COUNT - fifo data number */
#define ADC_ADC_REG_STATUS_FIFO_DATA_COUNT(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_STATUS_FIFO_DATA_COUNT_SHIFT)) & ADC_ADC_REG_STATUS_FIFO_DATA_COUNT_MASK)
/*! @} */

/*! @name ADC_REG_ISR - ADC ISR Register */
/*! @{ */

#define ADC_ADC_REG_ISR_RDY_MASK                 (0x1U)
#define ADC_ADC_REG_ISR_RDY_SHIFT                (0U)
/*! RDY - Conversion data ready interrupt flag */
#define ADC_ADC_REG_ISR_RDY(x)                   (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_RDY_SHIFT)) & ADC_ADC_REG_ISR_RDY_MASK)

#define ADC_ADC_REG_ISR_GAINSAT_MASK             (0x2U)
#define ADC_ADC_REG_ISR_GAINSAT_SHIFT            (1U)
/*! GAINSAT - Gain correction saturation interrupt flag */
#define ADC_ADC_REG_ISR_GAINSAT(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_GAINSAT_SHIFT)) & ADC_ADC_REG_ISR_GAINSAT_MASK)

#define ADC_ADC_REG_ISR_OFFSAT_MASK              (0x4U)
#define ADC_ADC_REG_ISR_OFFSAT_SHIFT             (2U)
/*! OFFSAT - Offset correction saturation interrupt flag */
#define ADC_ADC_REG_ISR_OFFSAT(x)                (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_OFFSAT_SHIFT)) & ADC_ADC_REG_ISR_OFFSAT_MASK)

#define ADC_ADC_REG_ISR_DATASAT_NEG_MASK         (0x8U)
#define ADC_ADC_REG_ISR_DATASAT_NEG_SHIFT        (3U)
/*! DATASAT_NEG - ADC data negative side saturation interrupt flag */
#define ADC_ADC_REG_ISR_DATASAT_NEG(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_DATASAT_NEG_SHIFT)) & ADC_ADC_REG_ISR_DATASAT_NEG_MASK)

#define ADC_ADC_REG_ISR_DATASAT_POS_MASK         (0x10U)
#define ADC_ADC_REG_ISR_DATASAT_POS_SHIFT        (4U)
/*! DATASAT_POS - ADC data positive side saturation interrupt flag */
#define ADC_ADC_REG_ISR_DATASAT_POS(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_DATASAT_POS_SHIFT)) & ADC_ADC_REG_ISR_DATASAT_POS_MASK)

#define ADC_ADC_REG_ISR_FIFO_OVERRUN_MASK        (0x20U)
#define ADC_ADC_REG_ISR_FIFO_OVERRUN_SHIFT       (5U)
/*! FIFO_OVERRUN - FIFO overrun interrupt flag */
#define ADC_ADC_REG_ISR_FIFO_OVERRUN(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_FIFO_OVERRUN_SHIFT)) & ADC_ADC_REG_ISR_FIFO_OVERRUN_MASK)

#define ADC_ADC_REG_ISR_FIFO_UNDERRUN_MASK       (0x40U)
#define ADC_ADC_REG_ISR_FIFO_UNDERRUN_SHIFT      (6U)
/*! FIFO_UNDERRUN - FIFO underrun interrupt flag */
#define ADC_ADC_REG_ISR_FIFO_UNDERRUN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ISR_FIFO_UNDERRUN_SHIFT)) & ADC_ADC_REG_ISR_FIFO_UNDERRUN_MASK)
/*! @} */

/*! @name ADC_REG_IMR - ADC IMR Register */
/*! @{ */

#define ADC_ADC_REG_IMR_RDY_MASK_MASK            (0x1U)
#define ADC_ADC_REG_IMR_RDY_MASK_SHIFT           (0U)
/*! RDY_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_RDY_MASK(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_RDY_MASK_SHIFT)) & ADC_ADC_REG_IMR_RDY_MASK_MASK)

#define ADC_ADC_REG_IMR_GAINSAT_MASK_MASK        (0x2U)
#define ADC_ADC_REG_IMR_GAINSAT_MASK_SHIFT       (1U)
/*! GAINSAT_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_GAINSAT_MASK(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_GAINSAT_MASK_SHIFT)) & ADC_ADC_REG_IMR_GAINSAT_MASK_MASK)

#define ADC_ADC_REG_IMR_OFFSAT_MASK_MASK         (0x4U)
#define ADC_ADC_REG_IMR_OFFSAT_MASK_SHIFT        (2U)
/*! OFFSAT_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_OFFSAT_MASK(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_OFFSAT_MASK_SHIFT)) & ADC_ADC_REG_IMR_OFFSAT_MASK_MASK)

#define ADC_ADC_REG_IMR_DATASAT_NEG_MASK_MASK    (0x8U)
#define ADC_ADC_REG_IMR_DATASAT_NEG_MASK_SHIFT   (3U)
/*! DATASAT_NEG_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_DATASAT_NEG_MASK(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_DATASAT_NEG_MASK_SHIFT)) & ADC_ADC_REG_IMR_DATASAT_NEG_MASK_MASK)

#define ADC_ADC_REG_IMR_DATASAT_POS_MASK_MASK    (0x10U)
#define ADC_ADC_REG_IMR_DATASAT_POS_MASK_SHIFT   (4U)
/*! DATASAT_POS_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_DATASAT_POS_MASK(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_DATASAT_POS_MASK_SHIFT)) & ADC_ADC_REG_IMR_DATASAT_POS_MASK_MASK)

#define ADC_ADC_REG_IMR_FIFO_OVERRUN_MASK_MASK   (0x20U)
#define ADC_ADC_REG_IMR_FIFO_OVERRUN_MASK_SHIFT  (5U)
/*! FIFO_OVERRUN_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_FIFO_OVERRUN_MASK(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_FIFO_OVERRUN_MASK_SHIFT)) & ADC_ADC_REG_IMR_FIFO_OVERRUN_MASK_MASK)

#define ADC_ADC_REG_IMR_FIFO_UNDERRUN_MASK_MASK  (0x40U)
#define ADC_ADC_REG_IMR_FIFO_UNDERRUN_MASK_SHIFT (6U)
/*! FIFO_UNDERRUN_MASK - write 1 mask */
#define ADC_ADC_REG_IMR_FIFO_UNDERRUN_MASK(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IMR_FIFO_UNDERRUN_MASK_SHIFT)) & ADC_ADC_REG_IMR_FIFO_UNDERRUN_MASK_MASK)
/*! @} */

/*! @name ADC_REG_IRSR - ADC IRSR Register */
/*! @{ */

#define ADC_ADC_REG_IRSR_RDY_RAW_MASK            (0x1U)
#define ADC_ADC_REG_IRSR_RDY_RAW_SHIFT           (0U)
/*! RDY_RAW - The corresponding flag will be captured into this register regardless the interrupt
 *    mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_RDY_RAW(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_RDY_RAW_SHIFT)) & ADC_ADC_REG_IRSR_RDY_RAW_MASK)

#define ADC_ADC_REG_IRSR_GAINSAT_RAW_MASK        (0x2U)
#define ADC_ADC_REG_IRSR_GAINSAT_RAW_SHIFT       (1U)
/*! GAINSAT_RAW - The corresponding flag will be captured into this register regardless the
 *    interrupt mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_GAINSAT_RAW(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_GAINSAT_RAW_SHIFT)) & ADC_ADC_REG_IRSR_GAINSAT_RAW_MASK)

#define ADC_ADC_REG_IRSR_OFFSAT_RAW_MASK         (0x4U)
#define ADC_ADC_REG_IRSR_OFFSAT_RAW_SHIFT        (2U)
/*! OFFSAT_RAW - The corresponding flag will be captured into this register regardless the interrupt
 *    mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_OFFSAT_RAW(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_OFFSAT_RAW_SHIFT)) & ADC_ADC_REG_IRSR_OFFSAT_RAW_MASK)

#define ADC_ADC_REG_IRSR_DATASAT_NEG_RAW_MASK    (0x8U)
#define ADC_ADC_REG_IRSR_DATASAT_NEG_RAW_SHIFT   (3U)
/*! DATASAT_NEG_RAW - The corresponding flag will be captured into this register regardless the
 *    interrupt mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_DATASAT_NEG_RAW(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_DATASAT_NEG_RAW_SHIFT)) & ADC_ADC_REG_IRSR_DATASAT_NEG_RAW_MASK)

#define ADC_ADC_REG_IRSR_DATASAT_POS_RAW_MASK    (0x10U)
#define ADC_ADC_REG_IRSR_DATASAT_POS_RAW_SHIFT   (4U)
/*! DATASAT_POS_RAW - The corresponding flag will be captured into this register regardless the
 *    interrupt mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_DATASAT_POS_RAW(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_DATASAT_POS_RAW_SHIFT)) & ADC_ADC_REG_IRSR_DATASAT_POS_RAW_MASK)

#define ADC_ADC_REG_IRSR_FIFO_OVERRUN_RAW_MASK   (0x20U)
#define ADC_ADC_REG_IRSR_FIFO_OVERRUN_RAW_SHIFT  (5U)
/*! FIFO_OVERRUN_RAW - The corresponding flag will be captured into this register regardless the
 *    interrupt mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_FIFO_OVERRUN_RAW(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_FIFO_OVERRUN_RAW_SHIFT)) & ADC_ADC_REG_IRSR_FIFO_OVERRUN_RAW_MASK)

#define ADC_ADC_REG_IRSR_FIFO_UNDERRUN_RAW_MASK  (0x40U)
#define ADC_ADC_REG_IRSR_FIFO_UNDERRUN_RAW_SHIFT (6U)
/*! FIFO_UNDERRUN_RAW - The corresponding flag will be captured into this register regardless the
 *    interrupt mask. Will be cleared only when int_clr is asserted.
 */
#define ADC_ADC_REG_IRSR_FIFO_UNDERRUN_RAW(x)    (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_IRSR_FIFO_UNDERRUN_RAW_SHIFT)) & ADC_ADC_REG_IRSR_FIFO_UNDERRUN_RAW_MASK)
/*! @} */

/*! @name ADC_REG_ICR - ADC ICR Register */
/*! @{ */

#define ADC_ADC_REG_ICR_RDY_CLR_MASK             (0x1U)
#define ADC_ADC_REG_ICR_RDY_CLR_SHIFT            (0U)
/*! RDY_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_RDY_CLR(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_RDY_CLR_SHIFT)) & ADC_ADC_REG_ICR_RDY_CLR_MASK)

#define ADC_ADC_REG_ICR_GAINSAT_CLR_MASK         (0x2U)
#define ADC_ADC_REG_ICR_GAINSAT_CLR_SHIFT        (1U)
/*! GAINSAT_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_GAINSAT_CLR(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_GAINSAT_CLR_SHIFT)) & ADC_ADC_REG_ICR_GAINSAT_CLR_MASK)

#define ADC_ADC_REG_ICR_OFFSAT_CLR_MASK          (0x4U)
#define ADC_ADC_REG_ICR_OFFSAT_CLR_SHIFT         (2U)
/*! OFFSAT_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_OFFSAT_CLR(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_OFFSAT_CLR_SHIFT)) & ADC_ADC_REG_ICR_OFFSAT_CLR_MASK)

#define ADC_ADC_REG_ICR_DATASAT_NEG_CLR_MASK     (0x8U)
#define ADC_ADC_REG_ICR_DATASAT_NEG_CLR_SHIFT    (3U)
/*! DATASAT_NEG_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_DATASAT_NEG_CLR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_DATASAT_NEG_CLR_SHIFT)) & ADC_ADC_REG_ICR_DATASAT_NEG_CLR_MASK)

#define ADC_ADC_REG_ICR_DATASAT_POS_CLR_MASK     (0x10U)
#define ADC_ADC_REG_ICR_DATASAT_POS_CLR_SHIFT    (4U)
/*! DATASAT_POS_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_DATASAT_POS_CLR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_DATASAT_POS_CLR_SHIFT)) & ADC_ADC_REG_ICR_DATASAT_POS_CLR_MASK)

#define ADC_ADC_REG_ICR_FIFO_OVERRUN_CLR_MASK    (0x20U)
#define ADC_ADC_REG_ICR_FIFO_OVERRUN_CLR_SHIFT   (5U)
/*! FIFO_OVERRUN_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_FIFO_OVERRUN_CLR(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_FIFO_OVERRUN_CLR_SHIFT)) & ADC_ADC_REG_ICR_FIFO_OVERRUN_CLR_MASK)

#define ADC_ADC_REG_ICR_FIFO_UNDERRUN_CLR_MASK   (0x40U)
#define ADC_ADC_REG_ICR_FIFO_UNDERRUN_CLR_SHIFT  (6U)
/*! FIFO_UNDERRUN_CLR - Write 1 to clear both adc_reg_irsr and adc_reg_isr */
#define ADC_ADC_REG_ICR_FIFO_UNDERRUN_CLR(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_ICR_FIFO_UNDERRUN_CLR_SHIFT)) & ADC_ADC_REG_ICR_FIFO_UNDERRUN_CLR_MASK)
/*! @} */

/*! @name ADC_REG_RESULT - ADC Result Register */
/*! @{ */

#define ADC_ADC_REG_RESULT_DATA_MASK             (0xFFFFFFFFU)
#define ADC_ADC_REG_RESULT_DATA_SHIFT            (0U)
/*! DATA - ADC finial conversion result data, after calibratiob and signed/unsigned process */
#define ADC_ADC_REG_RESULT_DATA(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_RESULT_DATA_SHIFT)) & ADC_ADC_REG_RESULT_DATA_MASK)
/*! @} */

/*! @name ADC_REG_RAW_RESULT - ADC Raw Result Register */
/*! @{ */

#define ADC_ADC_REG_RAW_RESULT_RAW_DATA_MASK     (0x3FFFFFU)
#define ADC_ADC_REG_RAW_RESULT_RAW_DATA_SHIFT    (0U)
/*! RAW_DATA - ADC Raw data in signed 22bit format */
#define ADC_ADC_REG_RAW_RESULT_RAW_DATA(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_RAW_RESULT_RAW_DATA_SHIFT)) & ADC_ADC_REG_RAW_RESULT_RAW_DATA_MASK)
/*! @} */

/*! @name ADC_REG_OFFSET_CAL - ADC Offset Calibration Register */
/*! @{ */

#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_MASK   (0xFFFFU)
#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_SHIFT  (0U)
/*! OFFSET_CAL - ADC self offset calibration value. 16-bit signed . */
#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_SHIFT)) & ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_MASK)

#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_USR_MASK (0xFFFF0000U)
#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_USR_SHIFT (16U)
/*! OFFSET_CAL_USR - User offset calibration data. 16-bit signed. */
#define ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_USR(x) (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_USR_SHIFT)) & ADC_ADC_REG_OFFSET_CAL_OFFSET_CAL_USR_MASK)
/*! @} */

/*! @name ADC_REG_GAIN_CAL - ADC Gain Calibration Register */
/*! @{ */

#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL_MASK       (0xFFFFU)
#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL_SHIFT      (0U)
/*! GAIN_CAL - ADC self gain calibration value. 16-bit signed. */
#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GAIN_CAL_GAIN_CAL_SHIFT)) & ADC_ADC_REG_GAIN_CAL_GAIN_CAL_MASK)

#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL_USR_MASK   (0xFFFF0000U)
#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL_USR_SHIFT  (16U)
/*! GAIN_CAL_USR - ADC user gain calibration value. 16-bit signed. */
#define ADC_ADC_REG_GAIN_CAL_GAIN_CAL_USR(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_GAIN_CAL_GAIN_CAL_USR_SHIFT)) & ADC_ADC_REG_GAIN_CAL_GAIN_CAL_USR_MASK)
/*! @} */

/*! @name ADC_REG_TEST - ADC Test Register */
/*! @{ */

#define ADC_ADC_REG_TEST_TEST_EN_MASK            (0x1U)
#define ADC_ADC_REG_TEST_TEST_EN_SHIFT           (0U)
/*! TEST_EN - Analog test enable. */
#define ADC_ADC_REG_TEST_TEST_EN(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_TEST_TEST_EN_SHIFT)) & ADC_ADC_REG_TEST_TEST_EN_MASK)

#define ADC_ADC_REG_TEST_TEST_SEL_MASK           (0xEU)
#define ADC_ADC_REG_TEST_TEST_SEL_SHIFT          (1U)
/*! TEST_SEL - test_sel */
#define ADC_ADC_REG_TEST_TEST_SEL(x)             (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_TEST_TEST_SEL_SHIFT)) & ADC_ADC_REG_TEST_TEST_SEL_MASK)
/*! @} */

/*! @name ADC_REG_AUDIO - ADC Audio Register */
/*! @{ */

#define ADC_ADC_REG_AUDIO_EN_MASK                (0x1U)
#define ADC_ADC_REG_AUDIO_EN_SHIFT               (0U)
/*! EN - Audio enable. */
#define ADC_ADC_REG_AUDIO_EN(x)                  (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_AUDIO_EN_SHIFT)) & ADC_ADC_REG_AUDIO_EN_MASK)

#define ADC_ADC_REG_AUDIO_PGA_GAIN_MASK          (0x38U)
#define ADC_ADC_REG_AUDIO_PGA_GAIN_SHIFT         (3U)
/*! PGA_GAIN - Audio PGA voltage gain select */
#define ADC_ADC_REG_AUDIO_PGA_GAIN(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_AUDIO_PGA_GAIN_SHIFT)) & ADC_ADC_REG_AUDIO_PGA_GAIN_MASK)

#define ADC_ADC_REG_AUDIO_PGA_CM_MASK            (0x1C0U)
#define ADC_ADC_REG_AUDIO_PGA_CM_SHIFT           (6U)
/*! PGA_CM - Audio PGA output common mode control */
#define ADC_ADC_REG_AUDIO_PGA_CM(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_AUDIO_PGA_CM_SHIFT)) & ADC_ADC_REG_AUDIO_PGA_CM_MASK)

#define ADC_ADC_REG_AUDIO_PGA_CHOP_EN_MASK       (0x200U)
#define ADC_ADC_REG_AUDIO_PGA_CHOP_EN_SHIFT      (9U)
/*! PGA_CHOP_EN - Audio PGA chopper enable. */
#define ADC_ADC_REG_AUDIO_PGA_CHOP_EN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_AUDIO_PGA_CHOP_EN_SHIFT)) & ADC_ADC_REG_AUDIO_PGA_CHOP_EN_MASK)
/*! @} */

/*! @name ADC_REG_VOICE_DET - ADC Voice Detect Register */
/*! @{ */

#define ADC_ADC_REG_VOICE_DET_DET_EN_MASK        (0x1U)
#define ADC_ADC_REG_VOICE_DET_DET_EN_SHIFT       (0U)
/*! DET_EN - Voice level detection enable select */
#define ADC_ADC_REG_VOICE_DET_DET_EN(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_VOICE_DET_DET_EN_SHIFT)) & ADC_ADC_REG_VOICE_DET_DET_EN_MASK)

#define ADC_ADC_REG_VOICE_DET_LEVEL_SEL_MASK     (0xEU)
#define ADC_ADC_REG_VOICE_DET_LEVEL_SEL_SHIFT    (1U)
/*! LEVEL_SEL - Voice level selection. */
#define ADC_ADC_REG_VOICE_DET_LEVEL_SEL(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_VOICE_DET_LEVEL_SEL_SHIFT)) & ADC_ADC_REG_VOICE_DET_LEVEL_SEL_MASK)
/*! @} */

/*! @name ADC_REG_RSVD - ADC Reserved Register */
/*! @{ */

#define ADC_ADC_REG_RSVD_UNUSED_RESERVED_ADC_CONTROL_BITS_MASK (0xFFFFU)
#define ADC_ADC_REG_RSVD_UNUSED_RESERVED_ADC_CONTROL_BITS_SHIFT (0U)
/*! UNUSED_RESERVED_ADC_CONTROL_BITS - unused_Reserved_ADC_control_bits */
#define ADC_ADC_REG_RSVD_UNUSED_RESERVED_ADC_CONTROL_BITS(x) (((uint32_t)(((uint32_t)(x)) << ADC_ADC_REG_RSVD_UNUSED_RESERVED_ADC_CONTROL_BITS_SHIFT)) & ADC_ADC_REG_RSVD_UNUSED_RESERVED_ADC_CONTROL_BITS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group ADC_Register_Masks */

/* Backward compatibility */
#define kADC_TriggerSourceGpt                    kADC_TriggerSourceCtimer
#define kADC_TriggerSourceGpio40                 kADC_TriggerSourceGpio50
#define kADC_TriggerSourceGpio41                 kADC_TriggerSourceGpio55


/*!
 * @}
 */ /* end of group ADC_Peripheral_Access_Layer */


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


#endif  /* ADC_H_ */

