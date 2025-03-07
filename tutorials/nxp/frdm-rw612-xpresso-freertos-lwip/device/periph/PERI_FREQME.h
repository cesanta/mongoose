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
**         CMSIS Peripheral Access Layer for FREQME
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
 * @file FREQME.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for FREQME
 *
 * CMSIS Peripheral Access Layer for FREQME
 */

#if !defined(FREQME_H_)
#define FREQME_H_                                /**< Symbol preventing repeated inclusion */

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
   -- FREQME Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FREQME_Peripheral_Access_Layer FREQME Peripheral Access Layer
 * @{
 */

/** FREQME - Register Layout Typedef */
typedef struct {
  union {                                          /* offset: 0x0 */
    __I  uint32_t FREQMECTRL_R;                      /**< Frequency Measurement (in Read mode), offset: 0x0 */
    __O  uint32_t FREQMECTRL_W;                      /**< Freqeuncy Measurement (in Write mode), offset: 0x0 */
  };
} FREQME_Type;

/* ----------------------------------------------------------------------------
   -- FREQME Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FREQME_Register_Masks FREQME Register Masks
 * @{
 */

/*! @name FREQMECTRL_R - Frequency Measurement (in Read mode) */
/*! @{ */

#define FREQME_FREQMECTRL_R_RESULT_MASK          (0x7FFFFFFFU)
#define FREQME_FREQMECTRL_R_RESULT_SHIFT         (0U)
/*! RESULT - Result */
#define FREQME_FREQMECTRL_R_RESULT(x)            (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_R_RESULT_SHIFT)) & FREQME_FREQMECTRL_R_RESULT_MASK)

#define FREQME_FREQMECTRL_R_MEASURE_IN_PROGRESS_MASK (0x80000000U)
#define FREQME_FREQMECTRL_R_MEASURE_IN_PROGRESS_SHIFT (31U)
/*! MEASURE_IN_PROGRESS - Measure in Progress
 *  0b0..Process complete. Measurement cycle is complete. The results are ready in the RESULT field.
 *  0b1..In Progress. Measurement cycle is in progress.
 */
#define FREQME_FREQMECTRL_R_MEASURE_IN_PROGRESS(x) (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_R_MEASURE_IN_PROGRESS_SHIFT)) & FREQME_FREQMECTRL_R_MEASURE_IN_PROGRESS_MASK)
/*! @} */

/*! @name FREQMECTRL_W - Freqeuncy Measurement (in Write mode) */
/*! @{ */

#define FREQME_FREQMECTRL_W_REF_SCALE_MASK       (0x1FU)
#define FREQME_FREQMECTRL_W_REF_SCALE_SHIFT      (0U)
/*! REF_SCALE - Reference Clock Scaling Factor
 *  0b00000..Count cycle = 2^0 = 1
 *  0b00001..Count cycle = 2^1 = 2
 *  0b00010..Count cycle = 2^4 = 4
 *  0b11111..Count cycle = 2^31 = 2,147,483,648
 */
#define FREQME_FREQMECTRL_W_REF_SCALE(x)         (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_W_REF_SCALE_SHIFT)) & FREQME_FREQMECTRL_W_REF_SCALE_MASK)

#define FREQME_FREQMECTRL_W_PULSE_MODE_MASK      (0x100U)
#define FREQME_FREQMECTRL_W_PULSE_MODE_SHIFT     (8U)
/*! PULSE_MODE - Pulse Width Measurement mode select
 *  0b0..Frequency Measurement Mode. FREQMECTRL works in a Frequency Measurement mode. Once the measurement starts
 *       (real count start is aligned at rising edge arrival on reference clock), the target counter increments by
 *       the target clock until the reference counter running by the reference clock reaches the count end point
 *       selected by REF_SCALE.
 *  0b1..Pulse Width Measurement mode. FREQMECTRL works in a Pulse Width Measurement mode, measuring the high or
 *       low period of reference clock input selected by PULSE_POL. The target counter starts incrementing by the
 *       target clock once a corresponding trigger edge (rising edge for high period measurement and falling edge for
 *       low period) occurs.
 */
#define FREQME_FREQMECTRL_W_PULSE_MODE(x)        (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_W_PULSE_MODE_SHIFT)) & FREQME_FREQMECTRL_W_PULSE_MODE_MASK)

#define FREQME_FREQMECTRL_W_PULSE_POL_MASK       (0x200U)
#define FREQME_FREQMECTRL_W_PULSE_POL_SHIFT      (9U)
/*! PULSE_POL - Pulse Polarity
 *  0b0..High Period. High period of reference clock is measured in Pulse Width Measurement mode triggered by the
 *       rising edge on the reference clock input.
 *  0b1..Low Period. Low period of reference clock is measured in Pulse Width Measurement mode triggered by the
 *       falling edge on the reference clock input.
 */
#define FREQME_FREQMECTRL_W_PULSE_POL(x)         (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_W_PULSE_POL_SHIFT)) & FREQME_FREQMECTRL_W_PULSE_POL_MASK)

#define FREQME_FREQMECTRL_W_MEASURE_IN_PROGRESS_MASK (0x80000000U)
#define FREQME_FREQMECTRL_W_MEASURE_IN_PROGRESS_SHIFT (31U)
/*! MEASURE_IN_PROGRESS - Measure in Progress
 *  0b0..Force Terminate. Forces the termination of any measurement cycle currently in progress and resets RESULT or just resets RESULT if in idle.
 *  0b1..Initiates Measurement Cycle. Initiates frequency or pulse width measurement process. Hardware clears the
 *       MEASURE_IN_PROGRESS bit when the measurement cycle completes. A new measurement starts if there is an
 *       active measurement in progress.
 */
#define FREQME_FREQMECTRL_W_MEASURE_IN_PROGRESS(x) (((uint32_t)(((uint32_t)(x)) << FREQME_FREQMECTRL_W_MEASURE_IN_PROGRESS_SHIFT)) & FREQME_FREQMECTRL_W_MEASURE_IN_PROGRESS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group FREQME_Register_Masks */


/*!
 * @}
 */ /* end of group FREQME_Peripheral_Access_Layer */


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


#endif  /* FREQME_H_ */

