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
**         CMSIS Peripheral Access Layer for SENSOR_CTRL
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
 * @file SENSOR_CTRL.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SENSOR_CTRL
 *
 * CMSIS Peripheral Access Layer for SENSOR_CTRL
 */

#if !defined(SENSOR_CTRL_H_)
#define SENSOR_CTRL_H_                           /**< Symbol preventing repeated inclusion */

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
   -- SENSOR_CTRL Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SENSOR_CTRL_Peripheral_Access_Layer SENSOR_CTRL Peripheral Access Layer
 * @{
 */

/** SENSOR_CTRL - Register Layout Typedef */
typedef struct {
  __IO uint32_t ADC_CTRL_REG_1;                    /**< General configuration of ADCC, offset: 0x0 */
  __I  uint32_t ADC_STATUS_REG;                    /**< status of ADC and ADCC, offset: 0x4 */
  __IO uint32_t TSEN_CTRL_1_REG_1;                 /**< TSEN Controller configuration, offset: 0x8 */
  __I  uint32_t TSEN_CTRL_1_REG_2;                 /**< TSEN Controller Output status register, offset: 0xC */
  __IO uint32_t TSEN_CTRL_2_REG_1;                 /**< TSEN Controller configuration, offset: 0x10 */
  __I  uint32_t TSEN_CTRL_2_REG_2;                 /**< TSEN Controller Output status register, offset: 0x14 */
  __IO uint32_t VSEN_CTRL_1_REG_1;                 /**< VSEN Controller configuration, offset: 0x18 */
  __I  uint32_t VSEN_CTRL_1_REG_2;                 /**< VSEN Controller Output status register, offset: 0x1C */
  __IO uint32_t VSEN_CTRL_2_REG_1;                 /**< VSEN2 Controller configuration, offset: 0x20 */
  __I  uint32_t VSEN_CTRL_2_REG_2;                 /**< VSEN2 Controller Output status register, offset: 0x24 */
  __IO uint32_t VSEN_CTRL_3_REG_1;                 /**< VSEN3 Controller configuration, offset: 0x28 */
  __I  uint32_t VSEN_CTRL_3_REG_2;                 /**< VSEN3 Controller Output status register, offset: 0x2C */
  __IO uint32_t VGLITCH_CTRL_REG_1;                /**< Voltage Glitch sensor controller configuration, offset: 0x30 */
  __IO uint32_t MISC_CTRL_REG;                     /**< Miscellaneous controls, offset: 0x34 */
  __IO uint32_t CFG_ERR_STATUS_REG;                /**< CFG ERROR Control, offset: 0x38 */
  __IO uint32_t SEN_CLR_REG;                       /**< CFG ERROR Control, offset: 0x3C */
  __IO uint32_t SEC_ECO_REG;                       /**< ECO Bits, offset: 0x40 */
} SENSOR_CTRL_Type;

/* ----------------------------------------------------------------------------
   -- SENSOR_CTRL Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SENSOR_CTRL_Register_Masks SENSOR_CTRL Register Masks
 * @{
 */

/*! @name ADC_CTRL_REG_1 - General configuration of ADCC */
/*! @{ */

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_RESET_SHIFT (0U)
/*! ADCC_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_RESET_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_RESET_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_CAL_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_CAL_ENABLE_SHIFT (1U)
/*! ADCC_SW_CAL_ENABLE - SW triggered calibration enable. */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_CAL_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_CAL_ENABLE_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_CAL_ENABLE_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_ENABLE_MASK (0x4U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_ENABLE_SHIFT (2U)
/*! ADCC_SW_ENABLE - sw enable from CPU for sensor and controller */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_ENABLE_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_SW_ENABLE_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TEST_CAL_BYPASS_MASK (0x8U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TEST_CAL_BYPASS_SHIFT (3U)
/*! ADCC_TEST_CAL_BYPASS - bypass calibration for ATE or other test */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TEST_CAL_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TEST_CAL_BYPASS_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TEST_CAL_BYPASS_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TB_SEL_MASK (0x30U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TB_SEL_SHIFT (4U)
/*! ADCC_TB_SEL - It selects one of the testbuses of ADCC. */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TB_SEL(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TB_SEL_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_TB_SEL_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_BYPASS_MASK (0x40U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_BYPASS_SHIFT (6U)
/*! ADCC_ERR_GAIN_BYPASS - Bypass error gain adjustment in ADCC data output */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_BYPASS_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_BYPASS_MASK)

#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_MASK (0x780U)
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_SHIFT (7U)
/*! ADCC_ERR_GAIN - IT is 4 bIT signed integer value of error gain of SARADC. If
 *    adcc_err_gain_bypass is set as 1, then this value is not considered for error gain adjustment of ADCC data
 *    output. Example: If value of this field is 4'b1110, then IT is -2 in decimal.
 */
#define SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_SHIFT)) & SENSOR_CTRL_ADC_CTRL_REG_1_ADCC_ERR_GAIN_MASK)
/*! @} */

/*! @name ADC_STATUS_REG - status of ADC and ADCC */
/*! @{ */

#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_OFFSET_CODE_MASK (0xFFFU)
#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_OFFSET_CODE_SHIFT (0U)
/*! ADCC_OFFSET_CODE - OFFSET of ADC computed after calibration. It is a signed number (2's
 *    complement format) with 1 fractional bit. Example, If binary value of this field is 1111_1111_0011
 *    then it represents -6.5 in decimal. Note that, this field is valid only when adcc_cal_done bit is
 *    1.
 */
#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_OFFSET_CODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_STATUS_REG_ADCC_OFFSET_CODE_SHIFT)) & SENSOR_CTRL_ADC_STATUS_REG_ADCC_OFFSET_CODE_MASK)

#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_CAL_DONE_MASK (0x1000U)
#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_CAL_DONE_SHIFT (12U)
/*! ADCC_CAL_DONE - Valid value: */
#define SENSOR_CTRL_ADC_STATUS_REG_ADCC_CAL_DONE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_STATUS_REG_ADCC_CAL_DONE_SHIFT)) & SENSOR_CTRL_ADC_STATUS_REG_ADCC_CAL_DONE_MASK)

#define SENSOR_CTRL_ADC_STATUS_REG_PU_ADC_REG_MASK (0x2000U)
#define SENSOR_CTRL_ADC_STATUS_REG_PU_ADC_REG_SHIFT (13U)
/*! PU_ADC_REG - Powerup/ power down value: */
#define SENSOR_CTRL_ADC_STATUS_REG_PU_ADC_REG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_STATUS_REG_PU_ADC_REG_SHIFT)) & SENSOR_CTRL_ADC_STATUS_REG_PU_ADC_REG_MASK)

#define SENSOR_CTRL_ADC_STATUS_REG_ADC_OUT_DFF_RSTB_MASK (0x4000U)
#define SENSOR_CTRL_ADC_STATUS_REG_ADC_OUT_DFF_RSTB_SHIFT (14U)
/*! ADC_OUT_DFF_RSTB - Reset value: */
#define SENSOR_CTRL_ADC_STATUS_REG_ADC_OUT_DFF_RSTB(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_ADC_STATUS_REG_ADC_OUT_DFF_RSTB_SHIFT)) & SENSOR_CTRL_ADC_STATUS_REG_ADC_OUT_DFF_RSTB_MASK)
/*! @} */

/*! @name TSEN_CTRL_1_REG_1 - TSEN Controller configuration */
/*! @{ */

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_RESET_SHIFT (0U)
/*! TSEN_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_RESET_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_RESET_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_ENABLE_SHIFT (1U)
/*! TSEN_SW_ENABLE - sw enable from CPU for sensor and controller */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_ENABLE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_ENABLE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TRIGGER_MODE_MASK (0xCU)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TRIGGER_MODE_SHIFT (2U)
/*! TSEN_TRIGGER_MODE - Trigger mode for sensor => DEFAULT: periodic */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TRIGGER_MODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TRIGGER_MODE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TRIGGER_MODE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TESTMODE_MASK (0x10U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TESTMODE_SHIFT (4U)
/*! TSEN_TESTMODE - This bit is used to test sensor controller. */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TESTMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TESTMODE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_TESTMODE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_TEMP_READ_EN_MASK (0x20U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_TEMP_READ_EN_SHIFT (5U)
/*! TSEN_SW_TEMP_READ_EN - SW based temperature reading enable for tsen (corresponding to tsen_trigger_mode = 1) */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_TEMP_READ_EN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_TEMP_READ_EN_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_SW_TEMP_READ_EN_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MIN_TEMP_THR_MASK (0x3FF00U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MIN_TEMP_THR_SHIFT (8U)
/*! TSEN_MIN_TEMP_THR - SW programmed minimum threshold for sensor. Default is -37C. */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MIN_TEMP_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MIN_TEMP_THR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MIN_TEMP_THR_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MAX_TEMP_THR_MASK (0x3FF00000U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MAX_TEMP_THR_SHIFT (20U)
/*! TSEN_MAX_TEMP_THR - SW programmed maximum threshold for sensor. Default is 137C. */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MAX_TEMP_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MAX_TEMP_THR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_1_TSEN_MAX_TEMP_THR_MASK)
/*! @} */

/*! @name TSEN_CTRL_1_REG_2 - TSEN Controller Output status register */
/*! @{ */

#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERR_TEMP_PVALUE_MASK (0x3FFU)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERR_TEMP_PVALUE_SHIFT (0U)
/*! TSEN_ERR_TEMP_PVALUE - The sensor reading captured at the time of *sen_error event -> this is for SW */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERR_TEMP_PVALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERR_TEMP_PVALUE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERR_TEMP_PVALUE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_TEMP_VALUE_MASK (0xFFC00U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_TEMP_VALUE_SHIFT (10U)
/*! TSEN_TEMP_VALUE - reading from last sensor sampling available for SW to read out */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_TEMP_VALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_TEMP_VALUE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_TEMP_VALUE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERROR_MASK (0x100000U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERROR_SHIFT (20U)
/*! TSEN_ERROR - Error indication from sensor */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERROR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSEN_ERROR_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_MASK (0x200000U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_SHIFT (21U)
/*! SW_ON_DEMAND_TSEN_RD_DONE_STATUS - Indicates sensor value[19:10] is valid for case of sw_on_demand trigger mode. */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_MASK)

#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSENDC_DEBUG_MASK (0xFFC00000U)
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSENDC_DEBUG_SHIFT (22U)
/*! TSENDC_DEBUG - Field description: */
#define SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSENDC_DEBUG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSENDC_DEBUG_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_1_REG_2_TSENDC_DEBUG_MASK)
/*! @} */

/*! @name TSEN_CTRL_2_REG_1 - TSEN Controller configuration */
/*! @{ */

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_RESET_SHIFT (0U)
/*! TSEN_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_RESET_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_RESET_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_ENABLE_SHIFT (1U)
/*! TSEN_SW_ENABLE - sw enable from CPU for sensor and controller(TSEN_PMIP is disabled by default) */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_ENABLE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_ENABLE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TRIGGER_MODE_MASK (0xCU)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TRIGGER_MODE_SHIFT (2U)
/*! TSEN_TRIGGER_MODE - Trigger mode for sensor => DEFAULT: periodic */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TRIGGER_MODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TRIGGER_MODE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TRIGGER_MODE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TESTMODE_MASK (0x10U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TESTMODE_SHIFT (4U)
/*! TSEN_TESTMODE - This bit is used to test sensor controller. */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TESTMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TESTMODE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_TESTMODE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_TEMP_READ_EN_MASK (0x20U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_TEMP_READ_EN_SHIFT (5U)
/*! TSEN_SW_TEMP_READ_EN - SW based temperature reading enable for tsen (corresponding to tsen_trigger_mode = 1) */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_TEMP_READ_EN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_TEMP_READ_EN_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_SW_TEMP_READ_EN_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MIN_TEMP_THR_MASK (0x3FF00U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MIN_TEMP_THR_SHIFT (8U)
/*! TSEN_MIN_TEMP_THR - SW programmed minimum threshold for sensor. Default is -37C. */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MIN_TEMP_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MIN_TEMP_THR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MIN_TEMP_THR_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MAX_TEMP_THR_MASK (0x3FF00000U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MAX_TEMP_THR_SHIFT (20U)
/*! TSEN_MAX_TEMP_THR - SW programmed maximum threshold for sensor. Default is 137C */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MAX_TEMP_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MAX_TEMP_THR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_1_TSEN_MAX_TEMP_THR_MASK)
/*! @} */

/*! @name TSEN_CTRL_2_REG_2 - TSEN Controller Output status register */
/*! @{ */

#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERR_TEMP_PVALUE_MASK (0x3FFU)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERR_TEMP_PVALUE_SHIFT (0U)
/*! TSEN_ERR_TEMP_PVALUE - The sensor reading captured at the time of *sen_error event -> this is for SW */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERR_TEMP_PVALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERR_TEMP_PVALUE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERR_TEMP_PVALUE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_TEMP_VALUE_MASK (0xFFC00U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_TEMP_VALUE_SHIFT (10U)
/*! TSEN_TEMP_VALUE - reading from last sensor sampling available for SW to read out */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_TEMP_VALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_TEMP_VALUE_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_TEMP_VALUE_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERROR_MASK (0x100000U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERROR_SHIFT (20U)
/*! TSEN_ERROR - Error indication from sensor */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERROR_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSEN_ERROR_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_MASK (0x200000U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_SHIFT (21U)
/*! SW_ON_DEMAND_TSEN_RD_DONE_STATUS - Indicates sensor value[19:10] is valid for case of sw_on_demand trigger mode. */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_2_SW_ON_DEMAND_TSEN_RD_DONE_STATUS_MASK)

#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSENDC_DEBUG_MASK (0xFFC00000U)
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSENDC_DEBUG_SHIFT (22U)
/*! TSENDC_DEBUG - Field description: */
#define SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSENDC_DEBUG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSENDC_DEBUG_SHIFT)) & SENSOR_CTRL_TSEN_CTRL_2_REG_2_TSENDC_DEBUG_MASK)
/*! @} */

/*! @name VSEN_CTRL_1_REG_1 - VSEN Controller configuration */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_RESET_SHIFT (0U)
/*! VSEN_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_RESET_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_RESET_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_SHIFT (1U)
/*! VSEN_SW_ENABLE - sw enable from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TRIGGER_MODE_MASK (0xCU)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TRIGGER_MODE_SHIFT (2U)
/*! VSEN_TRIGGER_MODE - Trigger mode for sensor => DEFAULT: periodic */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TRIGGER_MODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TRIGGER_MODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TRIGGER_MODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TESTMODE_MASK (0x10U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TESTMODE_SHIFT (4U)
/*! VSEN_TESTMODE - This bit is used to test sensor controller. */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TESTMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TESTMODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_TESTMODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK (0x20U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT (5U)
/*! VSEN_SW_VOLTAGE_READ_EN - SW based reading enable for sensor (corresponding to *sen_trigger_mode = 1) */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_VOLTAGE_READ_EN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR_MASK (0x3FF00U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT (8U)
/*! VSEN_MIN_VOLTAGE_THR - SW programmed minimum threshold for sensor. Default value is 0.925v. */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR_MASK (0x3FF00000U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT (20U)
/*! VSEN_MAX_VOLTAGE_THR - SW programmed maximum threshold for sensor. Default */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR_MASK)
/*! @} */

/*! @name VSEN_CTRL_1_REG_2 - VSEN Controller Output status register */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK (0x3FFU)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT (0U)
/*! VSEN_ERR_VOLTAGE_PVALUE - The sensor reading captured at the time of *sen_error event -> this is for SW */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERR_VOLTAGE_PVALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_VOLTAGE_VALUE_MASK (0xFFC00U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_VOLTAGE_VALUE_SHIFT (10U)
/*! VSEN_VOLTAGE_VALUE - reading from last sensor sampling available for SW to read out */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_VOLTAGE_VALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_VOLTAGE_VALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_VOLTAGE_VALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERROR_MASK (0x100000U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERROR_SHIFT (20U)
/*! VSEN_ERROR - Error indication from sensor */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERROR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSEN_ERROR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK (0x200000U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT (21U)
/*! SW_ON_DEMAND_VSEN_RD_DONE_STATUS - Indicates sensor value[19:10] is valid for case of sw_on_demand trigger mode. */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK)

#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSENDC_DEBUG_MASK (0xFFC00000U)
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSENDC_DEBUG_SHIFT (22U)
/*! VSENDC_DEBUG - Field description: */
#define SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSENDC_DEBUG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSENDC_DEBUG_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_1_REG_2_VSENDC_DEBUG_MASK)
/*! @} */

/*! @name VSEN_CTRL_2_REG_1 - VSEN2 Controller configuration */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_RESET_SHIFT (0U)
/*! VSEN_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_RESET_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_RESET_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_SHIFT (1U)
/*! VSEN_SW_ENABLE - sw enable from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TRIGGER_MODE_MASK (0xCU)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TRIGGER_MODE_SHIFT (2U)
/*! VSEN_TRIGGER_MODE - Trigger mode for sensor => DEFAULT: periodic */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TRIGGER_MODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TRIGGER_MODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TRIGGER_MODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TESTMODE_MASK (0x10U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TESTMODE_SHIFT (4U)
/*! VSEN_TESTMODE - This bit is used to test sensor controller. */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TESTMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TESTMODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_TESTMODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK (0x20U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT (5U)
/*! VSEN_SW_VOLTAGE_READ_EN - SW based reading enable for sensor (corresponding to *sen_trigger_mode = 1) */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_VOLTAGE_READ_EN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR_MASK (0x3FF00U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT (8U)
/*! VSEN_MIN_VOLTAGE_THR - SW programmed minimum threshold for sensor. Default */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR_MASK (0x3FF00000U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT (20U)
/*! VSEN_MAX_VOLTAGE_THR - SW programmed maximum threshold for sensor. Default */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR_MASK)
/*! @} */

/*! @name VSEN_CTRL_2_REG_2 - VSEN2 Controller Output status register */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK (0x3FFU)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT (0U)
/*! VSEN_ERR_VOLTAGE_PVALUE - The sensor reading captured at the time of *sen_error event -> this is for SW */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERR_VOLTAGE_PVALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_VOLTAGE_VALUE_MASK (0xFFC00U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_VOLTAGE_VALUE_SHIFT (10U)
/*! VSEN_VOLTAGE_VALUE - reading from last sensor sampling available for SW to read out */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_VOLTAGE_VALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_VOLTAGE_VALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_VOLTAGE_VALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERROR_MASK (0x100000U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERROR_SHIFT (20U)
/*! VSEN_ERROR - Error indication from sensor */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERROR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSEN_ERROR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK (0x200000U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT (21U)
/*! SW_ON_DEMAND_VSEN_RD_DONE_STATUS - Indicates sensor value[19:10] is valid for case of sw_on_demand trigger mode. */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK)

#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSENDC_DEBUG_MASK (0xFFC00000U)
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSENDC_DEBUG_SHIFT (22U)
/*! VSENDC_DEBUG - Field description: */
#define SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSENDC_DEBUG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSENDC_DEBUG_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_2_REG_2_VSENDC_DEBUG_MASK)
/*! @} */

/*! @name VSEN_CTRL_3_REG_1 - VSEN3 Controller configuration */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_RESET_MASK (0x1U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_RESET_SHIFT (0U)
/*! VSEN_SW_RESET - sw reset from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_RESET(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_RESET_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_RESET_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_SHIFT (1U)
/*! VSEN_SW_ENABLE - sw enable from CPU for sensor and controller */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TRIGGER_MODE_MASK (0xCU)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TRIGGER_MODE_SHIFT (2U)
/*! VSEN_TRIGGER_MODE - Trigger mode for sensor => DEFAULT: periodic */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TRIGGER_MODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TRIGGER_MODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TRIGGER_MODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TESTMODE_MASK (0x10U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TESTMODE_SHIFT (4U)
/*! VSEN_TESTMODE - This bit is used to test sensor controller. */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TESTMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TESTMODE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_TESTMODE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK (0x20U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT (5U)
/*! VSEN_SW_VOLTAGE_READ_EN - SW based reading enable for sensor (corresponding to *sen_trigger_mode = 1) */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_VOLTAGE_READ_EN(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_VOLTAGE_READ_EN_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_VOLTAGE_READ_EN_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR_MASK (0x3FF00U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT (8U)
/*! VSEN_MIN_VOLTAGE_THR - SW programmed minimum threshold for sensor. Default */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR_MASK (0x3FF00000U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT (20U)
/*! VSEN_MAX_VOLTAGE_THR - SW programmed maximum threshold for sensor. Default */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR_MASK)
/*! @} */

/*! @name VSEN_CTRL_3_REG_2 - VSEN3 Controller Output status register */
/*! @{ */

#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK (0x3FFU)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT (0U)
/*! VSEN_ERR_VOLTAGE_PVALUE - The sensor reading captured at the time of *sen_error event -> this is for SW */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERR_VOLTAGE_PVALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERR_VOLTAGE_PVALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERR_VOLTAGE_PVALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_VOLTAGE_VALUE_MASK (0xFFC00U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_VOLTAGE_VALUE_SHIFT (10U)
/*! VSEN_VOLTAGE_VALUE - reading from last sensor sampling available for SW to read out */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_VOLTAGE_VALUE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_VOLTAGE_VALUE_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_VOLTAGE_VALUE_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERROR_MASK (0x100000U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERROR_SHIFT (20U)
/*! VSEN_ERROR - Error indication from sensor */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERROR_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSEN_ERROR_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK (0x200000U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT (21U)
/*! SW_ON_DEMAND_VSEN_RD_DONE_STATUS - Indicates sensor value[19:10] is valid for case of sw_on_demand trigger mode. */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_2_SW_ON_DEMAND_VSEN_RD_DONE_STATUS_MASK)

#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSENDC_DEBUG_MASK (0xFFC00000U)
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSENDC_DEBUG_SHIFT (22U)
/*! VSENDC_DEBUG - Field description: */
#define SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSENDC_DEBUG(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSENDC_DEBUG_SHIFT)) & SENSOR_CTRL_VSEN_CTRL_3_REG_2_VSENDC_DEBUG_MASK)
/*! @} */

/*! @name VGLITCH_CTRL_REG_1 - Voltage Glitch sensor controller configuration */
/*! @{ */

#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_SW_ENABLE_MASK (0x2U)
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_SW_ENABLE_SHIFT (1U)
/*! VGLITCH_SW_ENABLE - sw enable from CPU for Glitch Sensor */
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_SW_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_SW_ENABLE_SHIFT)) & SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_SW_ENABLE_MASK)

#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_TESMODE_MASK (0x4U)
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_TESMODE_SHIFT (2U)
/*! VGLITCH_TESMODE - Testmode enable from CPU for Glitch Sensor */
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_TESMODE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_TESMODE_SHIFT)) & SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_TESMODE_MASK)

#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_ERROR_MASK (0x10U)
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_ERROR_SHIFT (4U)
/*! VGLITCH_ERROR - Error indication from Voltage glitch sensor */
#define SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_ERROR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_ERROR_SHIFT)) & SENSOR_CTRL_VGLITCH_CTRL_REG_1_VGLITCH_ERROR_MASK)
/*! @} */

/*! @name MISC_CTRL_REG - Miscellaneous controls */
/*! @{ */

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_ENABLE_MASK (0x1U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_ENABLE_SHIFT (0U)
/*! TIMER_1_ENABLE - enables the timer for sensor to start counting */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_ENABLE_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_ENABLE_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_PERIOD_MASK (0xEU)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_PERIOD_SHIFT (1U)
/*! TIMER_1_PERIOD - Pre defined sampling periods for this sensor */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_PERIOD(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_PERIOD_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_1_PERIOD_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_ENABLE_MASK (0x10U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_ENABLE_SHIFT (4U)
/*! TIMER_2_ENABLE - enables the timer for sensor to start counting */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_ENABLE_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_ENABLE_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_PERIOD_MASK (0xE0U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_PERIOD_SHIFT (5U)
/*! TIMER_2_PERIOD - Pre defined sampling periods for this sensor */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_PERIOD(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_PERIOD_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_2_PERIOD_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_ENABLE_MASK (0x100U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_ENABLE_SHIFT (8U)
/*! TIMER_3_ENABLE - enables the timer for sensor to start counting */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_ENABLE_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_ENABLE_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_PERIOD_MASK (0xE00U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_PERIOD_SHIFT (9U)
/*! TIMER_3_PERIOD - Pre defined sampling periods for this sensor */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_PERIOD(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_PERIOD_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_3_PERIOD_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_ENABLE_MASK (0x1000U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_ENABLE_SHIFT (12U)
/*! TIMER_4_ENABLE - enables the timer for sensor to start counting */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_ENABLE_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_ENABLE_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_PERIOD_MASK (0xE000U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_PERIOD_SHIFT (13U)
/*! TIMER_4_PERIOD - Pre defined sampling periods for this sensor */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_PERIOD(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_PERIOD_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_4_PERIOD_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_ENABLE_MASK (0x10000U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_ENABLE_SHIFT (16U)
/*! TIMER_5_ENABLE - enables the timer for sensor to start counting */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_ENABLE_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_ENABLE_MASK)

#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_PERIOD_MASK (0xE0000U)
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_PERIOD_SHIFT (17U)
/*! TIMER_5_PERIOD - Pre defined sampling periods for this sensor */
#define SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_PERIOD(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_PERIOD_SHIFT)) & SENSOR_CTRL_MISC_CTRL_REG_TIMER_5_PERIOD_MASK)
/*! @} */

/*! @name CFG_ERR_STATUS_REG - CFG ERROR Control */
/*! @{ */

#define SENSOR_CTRL_CFG_ERR_STATUS_REG_TB_SEL_MASK (0x7U)
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_TB_SEL_SHIFT (0U)
/*! TB_SEL - Select Testbus that can go to GPIO */
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_TB_SEL(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_CFG_ERR_STATUS_REG_TB_SEL_SHIFT)) & SENSOR_CTRL_CFG_ERR_STATUS_REG_TB_SEL_MASK)

#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_CFG_ERR_MASK (0x100U)
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_CFG_ERR_SHIFT (8U)
/*! SEC_CFG_ERR - Indication to SW that Sensor config Registers are has Errors. Original and shadow registers are not same */
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_CFG_ERR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_CFG_ERR_SHIFT)) & SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_CFG_ERR_MASK)

#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_SEN_ERR_MASK_MASK (0x7F0000U)
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_SEN_ERR_MASK_SHIFT (16U)
/*! SEC_SEN_ERR_MASK - SW Mask control bit for sensor Error. */
#define SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_SEN_ERR_MASK(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_SEN_ERR_MASK_SHIFT)) & SENSOR_CTRL_CFG_ERR_STATUS_REG_SEC_SEN_ERR_MASK_MASK)
/*! @} */

/*! @name SEN_CLR_REG - CFG ERROR Control */
/*! @{ */

#define SENSOR_CTRL_SEN_CLR_REG_SEN_ERR_CLR_MASK (0x7FU)
#define SENSOR_CTRL_SEN_CLR_REG_SEN_ERR_CLR_SHIFT (0U)
/*! SEN_ERR_CLR - SW control to clear error status of Sensor. */
#define SENSOR_CTRL_SEN_CLR_REG_SEN_ERR_CLR(x)   (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_SEN_CLR_REG_SEN_ERR_CLR_SHIFT)) & SENSOR_CTRL_SEN_CLR_REG_SEN_ERR_CLR_MASK)

#define SENSOR_CTRL_SEN_CLR_REG_SEN_RD_DONE_CLR_MASK (0x1F00U)
#define SENSOR_CTRL_SEN_CLR_REG_SEN_RD_DONE_CLR_SHIFT (8U)
/*! SEN_RD_DONE_CLR - SW control to clear read done status bit of Sensor in case of trigger mode set to on demand. */
#define SENSOR_CTRL_SEN_CLR_REG_SEN_RD_DONE_CLR(x) (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_SEN_CLR_REG_SEN_RD_DONE_CLR_SHIFT)) & SENSOR_CTRL_SEN_CLR_REG_SEN_RD_DONE_CLR_MASK)
/*! @} */

/*! @name SEC_ECO_REG - ECO Bits */
/*! @{ */

#define SENSOR_CTRL_SEC_ECO_REG_ECO_BITS_MASK    (0xFFFFFFFFU)
#define SENSOR_CTRL_SEC_ECO_REG_ECO_BITS_SHIFT   (0U)
/*! ECO_BITS - Reserved for ECOs */
#define SENSOR_CTRL_SEC_ECO_REG_ECO_BITS(x)      (((uint32_t)(((uint32_t)(x)) << SENSOR_CTRL_SEC_ECO_REG_ECO_BITS_SHIFT)) & SENSOR_CTRL_SEC_ECO_REG_ECO_BITS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SENSOR_CTRL_Register_Masks */


/*!
 * @}
 */ /* end of group SENSOR_CTRL_Peripheral_Access_Layer */


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


#endif  /* SENSOR_CTRL_H_ */

