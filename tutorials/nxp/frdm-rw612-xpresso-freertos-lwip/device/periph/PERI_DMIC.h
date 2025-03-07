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
**         CMSIS Peripheral Access Layer for DMIC
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
 * @file DMIC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for DMIC
 *
 * CMSIS Peripheral Access Layer for DMIC
 */

#if !defined(DMIC_H_)
#define DMIC_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- DMIC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMIC_Peripheral_Access_Layer DMIC Peripheral Access Layer
 * @{
 */

/** DMIC - Size of Registers Arrays */
#define DMIC_CHANNEL_COUNT                        4u

/** DMIC - Register Layout Typedef */
typedef struct {
  struct {                                         /* offset: 0x0, array step: 0x100 */
    __IO uint32_t OSR;                               /**< Oversample Rate, array offset: 0x0, array step: 0x100 */
    __IO uint32_t DIVHFCLK;                          /**< DMIC Clock, array offset: 0x4, array step: 0x100 */
    __IO uint32_t PREAC2FSCOEF;                      /**< Compensation Filter for 2 FS, array offset: 0x8, array step: 0x100 */
    __IO uint32_t PREAC4FSCOEF;                      /**< Compensation Filter for 4 FS, array offset: 0xC, array step: 0x100 */
    __IO uint32_t GAINSHIFT;                         /**< Decimator Gain Shift, array offset: 0x10, array step: 0x100 */
         uint8_t RESERVED_0[108];
    __IO uint32_t FIFO_CTRL;                         /**< FIFO Control, array offset: 0x80, array step: 0x100 */
    __IO uint32_t FIFO_STATUS;                       /**< FIFO Status, array offset: 0x84, array step: 0x100 */
    __I  uint32_t FIFO_DATA;                         /**< FIFO Data, array offset: 0x88, array step: 0x100 */
    __IO uint32_t PHY_CTRL;                          /**< Physical Control, array offset: 0x8C, array step: 0x100 */
    __IO uint32_t DC_CTRL;                           /**< DC Filter Control, array offset: 0x90, array step: 0x100 */
         uint8_t RESERVED_1[108];
  } CHANNEL[DMIC_CHANNEL_COUNT];
       uint8_t RESERVED_0[2816];
  __IO uint32_t CHANEN;                            /**< Channel Enable, offset: 0xF00 */
       uint8_t RESERVED_1[12];
  __IO uint32_t USE2FS;                            /**< Use 2 FS register, offset: 0xF10 */
  __IO uint32_t GLOBAL_SYCN_EN;                    /**< Global Channel Synchronization Enable, offset: 0xF14 */
  __IO uint32_t GLOBAL_COUNT_VAL;                  /**< Global channel synchronization counter value, offset: 0xF18 */
  __IO uint32_t DECRESET;                          /**< DMIC decimator reset, offset: 0xF1C */
       uint8_t RESERVED_2[96];
  __IO uint32_t HWVADGAIN;                         /**< HWVAD Input Gain, offset: 0xF80 */
  __IO uint32_t HWVADHPFS;                         /**< HWVAD Filter Control, offset: 0xF84 */
  __IO uint32_t HWVADST10;                         /**< HWVAD Control, offset: 0xF88 */
  __IO uint32_t HWVADRSTT;                         /**< HWVAD Filter Reset, offset: 0xF8C */
  __IO uint32_t HWVADTHGN;                         /**< HWVAD Noise Estimator Gain, offset: 0xF90 */
  __IO uint32_t HWVADTHGS;                         /**< HWVAD Signal Estimator Gain, offset: 0xF94 */
  __I  uint32_t HWVADLOWZ;                         /**< HWVAD Noise Envelope Estimator, offset: 0xF98 */
} DMIC_Type;

/* ----------------------------------------------------------------------------
   -- DMIC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMIC_Register_Masks DMIC Register Masks
 * @{
 */

/*! @name CHANNEL_OSR - Oversample Rate */
/*! @{ */

#define DMIC_CHANNEL_OSR_OSR_MASK                (0xFFU)
#define DMIC_CHANNEL_OSR_OSR_SHIFT               (0U)
/*! OSR - Oversample Rate */
#define DMIC_CHANNEL_OSR_OSR(x)                  (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_OSR_OSR_SHIFT)) & DMIC_CHANNEL_OSR_OSR_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_OSR */
#define DMIC_CHANNEL_OSR_COUNT                   (4U)

/*! @name CHANNEL_DIVHFCLK - DMIC Clock */
/*! @{ */

#define DMIC_CHANNEL_DIVHFCLK_PDMDIV_MASK        (0xFU)
#define DMIC_CHANNEL_DIVHFCLK_PDMDIV_SHIFT       (0U)
/*! PDMDIV - PDM Clock Divider Value
 *  0b0000..Divide by 1
 *  0b0001..Divide by 2
 *  0b0010..Divide by 3
 *  0b0011..Divide by 4
 *  0b0100..Divide by 6
 *  0b0101..Divide by 8
 *  0b0110..Divide by 12
 *  0b0111..Divide by 16
 *  0b1000..Divide by 24
 *  0b1001..Divide by 32
 *  0b1010..Divide by 48
 *  0b1011..Divide by 64
 *  0b1100..Divide by 96
 *  0b1101..Divide by 128
 *  0b1110-0b1111..Reserved
 */
#define DMIC_CHANNEL_DIVHFCLK_PDMDIV(x)          (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_DIVHFCLK_PDMDIV_SHIFT)) & DMIC_CHANNEL_DIVHFCLK_PDMDIV_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_DIVHFCLK */
#define DMIC_CHANNEL_DIVHFCLK_COUNT              (4U)

/*! @name CHANNEL_PREAC2FSCOEF - Compensation Filter for 2 FS */
/*! @{ */

#define DMIC_CHANNEL_PREAC2FSCOEF_COMP_MASK      (0x3U)
#define DMIC_CHANNEL_PREAC2FSCOEF_COMP_SHIFT     (0U)
/*! COMP - Compensation value
 *  0b00..Compensation = 0. This is the recommended setting.
 *  0b01..Compensation = -0.16
 *  0b10..Compensation = -0.15
 *  0b11..Compensation = -0.13
 */
#define DMIC_CHANNEL_PREAC2FSCOEF_COMP(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_PREAC2FSCOEF_COMP_SHIFT)) & DMIC_CHANNEL_PREAC2FSCOEF_COMP_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_PREAC2FSCOEF */
#define DMIC_CHANNEL_PREAC2FSCOEF_COUNT          (4U)

/*! @name CHANNEL_PREAC4FSCOEF - Compensation Filter for 4 FS */
/*! @{ */

#define DMIC_CHANNEL_PREAC4FSCOEF_COMP_MASK      (0x3U)
#define DMIC_CHANNEL_PREAC4FSCOEF_COMP_SHIFT     (0U)
/*! COMP - Compensation value
 *  0b00..Compensation = 0. This is the recommended setting.
 *  0b01..Compensation = -0.16
 *  0b10..Compensation = -0.15
 *  0b11..Compensation = -0.13
 */
#define DMIC_CHANNEL_PREAC4FSCOEF_COMP(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_PREAC4FSCOEF_COMP_SHIFT)) & DMIC_CHANNEL_PREAC4FSCOEF_COMP_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_PREAC4FSCOEF */
#define DMIC_CHANNEL_PREAC4FSCOEF_COUNT          (4U)

/*! @name CHANNEL_GAINSHIFT - Decimator Gain Shift */
/*! @{ */

#define DMIC_CHANNEL_GAINSHIFT_GAIN_MASK         (0x3FU)
#define DMIC_CHANNEL_GAINSHIFT_GAIN_SHIFT        (0U)
/*! GAIN - Gain */
#define DMIC_CHANNEL_GAINSHIFT_GAIN(x)           (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_GAINSHIFT_GAIN_SHIFT)) & DMIC_CHANNEL_GAINSHIFT_GAIN_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_GAINSHIFT */
#define DMIC_CHANNEL_GAINSHIFT_COUNT             (4U)

/*! @name CHANNEL_FIFO_CTRL - FIFO Control */
/*! @{ */

#define DMIC_CHANNEL_FIFO_CTRL_ENABLE_MASK       (0x1U)
#define DMIC_CHANNEL_FIFO_CTRL_ENABLE_SHIFT      (0U)
/*! ENABLE - FIFO Enable.
 *  0b0..FIFO is not enabled. Enabling a DMIC channel with the FIFO disabled could be useful while data is being
 *       streamed to the I2S, or in order to avoid a filter settling delay when a channel is re-enabled after a
 *       period when the data was not needed.
 *  0b1..FIFO is enabled. The FIFO must be enabled in order for the CPU or DMA to read data from the DMIC via the FIFODATA register.
 */
#define DMIC_CHANNEL_FIFO_CTRL_ENABLE(x)         (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_CTRL_ENABLE_SHIFT)) & DMIC_CHANNEL_FIFO_CTRL_ENABLE_MASK)

#define DMIC_CHANNEL_FIFO_CTRL_RESETN_MASK       (0x2U)
#define DMIC_CHANNEL_FIFO_CTRL_RESETN_SHIFT      (1U)
/*! RESETN - FIFO Reset
 *  0b0..Reset the FIFO. This must be cleared before resuming operation.
 *  0b1..Normal operation
 */
#define DMIC_CHANNEL_FIFO_CTRL_RESETN(x)         (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_CTRL_RESETN_SHIFT)) & DMIC_CHANNEL_FIFO_CTRL_RESETN_MASK)

#define DMIC_CHANNEL_FIFO_CTRL_INTEN_MASK        (0x4U)
#define DMIC_CHANNEL_FIFO_CTRL_INTEN_SHIFT       (2U)
/*! INTEN - Interrupt Enable.
 *  0b0..FIFO level interrupts are not enabled.
 *  0b1..FIFO level interrupts are enabled.
 */
#define DMIC_CHANNEL_FIFO_CTRL_INTEN(x)          (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_CTRL_INTEN_SHIFT)) & DMIC_CHANNEL_FIFO_CTRL_INTEN_MASK)

#define DMIC_CHANNEL_FIFO_CTRL_DMAEN_MASK        (0x8U)
#define DMIC_CHANNEL_FIFO_CTRL_DMAEN_SHIFT       (3U)
/*! DMAEN - DMA Enable
 *  0b0..DMA requests are not enabled.
 *  0b1..DMA requests based on FIFO level are enabled.
 */
#define DMIC_CHANNEL_FIFO_CTRL_DMAEN(x)          (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_CTRL_DMAEN_SHIFT)) & DMIC_CHANNEL_FIFO_CTRL_DMAEN_MASK)

#define DMIC_CHANNEL_FIFO_CTRL_TRIGLVL_MASK      (0x1F0000U)
#define DMIC_CHANNEL_FIFO_CTRL_TRIGLVL_SHIFT     (16U)
/*! TRIGLVL - FIFO Trigger Level for Interrupt
 *  0b00000..Trigger when the FIFO has received one entry (is no longer empty).
 *  0b00001..Trigger when the FIFO has received two entries.
 *  0b01110..Trigger when the FIFO has received 15 entries.
 *  0b01111..Trigger when the FIFO has received 16 entries (has become full).
 */
#define DMIC_CHANNEL_FIFO_CTRL_TRIGLVL(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_CTRL_TRIGLVL_SHIFT)) & DMIC_CHANNEL_FIFO_CTRL_TRIGLVL_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_FIFO_CTRL */
#define DMIC_CHANNEL_FIFO_CTRL_COUNT             (4U)

/*! @name CHANNEL_FIFO_STATUS - FIFO Status */
/*! @{ */

#define DMIC_CHANNEL_FIFO_STATUS_INT_MASK        (0x1U)
#define DMIC_CHANNEL_FIFO_STATUS_INT_SHIFT       (0U)
/*! INT - Status of Interrupt (write 1 to clear) */
#define DMIC_CHANNEL_FIFO_STATUS_INT(x)          (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_STATUS_INT_SHIFT)) & DMIC_CHANNEL_FIFO_STATUS_INT_MASK)

#define DMIC_CHANNEL_FIFO_STATUS_OVERRUN_MASK    (0x2U)
#define DMIC_CHANNEL_FIFO_STATUS_OVERRUN_SHIFT   (1U)
/*! OVERRUN - Overrun Detected (write 1 to clear) */
#define DMIC_CHANNEL_FIFO_STATUS_OVERRUN(x)      (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_STATUS_OVERRUN_SHIFT)) & DMIC_CHANNEL_FIFO_STATUS_OVERRUN_MASK)

#define DMIC_CHANNEL_FIFO_STATUS_UNDERRUN_MASK   (0x4U)
#define DMIC_CHANNEL_FIFO_STATUS_UNDERRUN_SHIFT  (2U)
/*! UNDERRUN - Underrun Detected (write 1 to clear) */
#define DMIC_CHANNEL_FIFO_STATUS_UNDERRUN(x)     (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_STATUS_UNDERRUN_SHIFT)) & DMIC_CHANNEL_FIFO_STATUS_UNDERRUN_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_FIFO_STATUS */
#define DMIC_CHANNEL_FIFO_STATUS_COUNT           (4U)

/*! @name CHANNEL_FIFO_DATA - FIFO Data */
/*! @{ */

#define DMIC_CHANNEL_FIFO_DATA_DATA_MASK         (0xFFFFFFU)
#define DMIC_CHANNEL_FIFO_DATA_DATA_SHIFT        (0U)
/*! DATA - PCM Data */
#define DMIC_CHANNEL_FIFO_DATA_DATA(x)           (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_FIFO_DATA_DATA_SHIFT)) & DMIC_CHANNEL_FIFO_DATA_DATA_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_FIFO_DATA */
#define DMIC_CHANNEL_FIFO_DATA_COUNT             (4U)

/*! @name CHANNEL_PHY_CTRL - Physical Control */
/*! @{ */

#define DMIC_CHANNEL_PHY_CTRL_PHY_FALL_MASK      (0x1U)
#define DMIC_CHANNEL_PHY_CTRL_PHY_FALL_SHIFT     (0U)
/*! PHY_FALL - Capture DMIC on Falling edge (0 means on rising)
 *  0b0..Capture PDM_DATA on the rising edge of PDM_CLK.
 *  0b1..Capture PDM_DATA on the falling edge of PDM_CLK.
 */
#define DMIC_CHANNEL_PHY_CTRL_PHY_FALL(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_PHY_CTRL_PHY_FALL_SHIFT)) & DMIC_CHANNEL_PHY_CTRL_PHY_FALL_MASK)

#define DMIC_CHANNEL_PHY_CTRL_PHY_HALF_MASK      (0x2U)
#define DMIC_CHANNEL_PHY_CTRL_PHY_HALF_SHIFT     (1U)
/*! PHY_HALF - Use Half rate sampling (ie Clock to dmic is sent at half the speed than the decimator is providing)
 *  0b0..Standard half rate sampling. The clock to the DMIC is sent at the same rate as the decimator is providing.
 *  0b1..Use half rate sampling. The clock to the DMIC is sent at half the rate that the decimator is providing.
 */
#define DMIC_CHANNEL_PHY_CTRL_PHY_HALF(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_PHY_CTRL_PHY_HALF_SHIFT)) & DMIC_CHANNEL_PHY_CTRL_PHY_HALF_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_PHY_CTRL */
#define DMIC_CHANNEL_PHY_CTRL_COUNT              (4U)

/*! @name CHANNEL_DC_CTRL - DC Filter Control */
/*! @{ */

#define DMIC_CHANNEL_DC_CTRL_DCPOLE_MASK         (0x3U)
#define DMIC_CHANNEL_DC_CTRL_DCPOLE_SHIFT        (0U)
/*! DCPOLE - DC Block Filter
 *  0b00..Flat Response, no filter
 *  0b01..155 Hz
 *  0b10..78 Hz
 *  0b11..39 Hz
 */
#define DMIC_CHANNEL_DC_CTRL_DCPOLE(x)           (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_DC_CTRL_DCPOLE_SHIFT)) & DMIC_CHANNEL_DC_CTRL_DCPOLE_MASK)

#define DMIC_CHANNEL_DC_CTRL_DCGAIN_MASK         (0xF0U)
#define DMIC_CHANNEL_DC_CTRL_DCGAIN_SHIFT        (4U)
/*! DCGAIN - DC Gain */
#define DMIC_CHANNEL_DC_CTRL_DCGAIN(x)           (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_DC_CTRL_DCGAIN_SHIFT)) & DMIC_CHANNEL_DC_CTRL_DCGAIN_MASK)

#define DMIC_CHANNEL_DC_CTRL_SATURATEAT16BIT_MASK (0x100U)
#define DMIC_CHANNEL_DC_CTRL_SATURATEAT16BIT_SHIFT (8U)
/*! SATURATEAT16BIT - Saturate at 16 Bit
 *  0b0..Do not Saturate. Results roll over if out range and do not saturate.
 *  0b1..Saturate. If the result overflows, it saturates at 0xFFFF for positive overflow and 0x8000 for negative overflow.
 */
#define DMIC_CHANNEL_DC_CTRL_SATURATEAT16BIT(x)  (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_DC_CTRL_SATURATEAT16BIT_SHIFT)) & DMIC_CHANNEL_DC_CTRL_SATURATEAT16BIT_MASK)

#define DMIC_CHANNEL_DC_CTRL_SIGNEXTEND_MASK     (0x200U)
#define DMIC_CHANNEL_DC_CTRL_SIGNEXTEND_SHIFT    (9U)
/*! SIGNEXTEND - Sign Extend
 *  0b0..The top byte of the FIFODATA register is always 0.
 *  0b1..The top byte of the FIFODATA register is sign extended.
 */
#define DMIC_CHANNEL_DC_CTRL_SIGNEXTEND(x)       (((uint32_t)(((uint32_t)(x)) << DMIC_CHANNEL_DC_CTRL_SIGNEXTEND_SHIFT)) & DMIC_CHANNEL_DC_CTRL_SIGNEXTEND_MASK)
/*! @} */

/* The count of DMIC_CHANNEL_DC_CTRL */
#define DMIC_CHANNEL_DC_CTRL_COUNT               (4U)

/*! @name CHANEN - Channel Enable */
/*! @{ */

#define DMIC_CHANEN_EN_CH0_MASK                  (0x1U)
#define DMIC_CHANEN_EN_CH0_SHIFT                 (0U)
/*! EN_CH0 - Enable Channel n
 *  0b0..PDM channel n is disabled.
 *  0b1..PDM channel n is enabled.
 */
#define DMIC_CHANEN_EN_CH0(x)                    (((uint32_t)(((uint32_t)(x)) << DMIC_CHANEN_EN_CH0_SHIFT)) & DMIC_CHANEN_EN_CH0_MASK)

#define DMIC_CHANEN_EN_CH1_MASK                  (0x2U)
#define DMIC_CHANEN_EN_CH1_SHIFT                 (1U)
/*! EN_CH1 - Enable Channel n
 *  0b0..PDM channel n is disabled.
 *  0b1..PDM channel n is enabled.
 */
#define DMIC_CHANEN_EN_CH1(x)                    (((uint32_t)(((uint32_t)(x)) << DMIC_CHANEN_EN_CH1_SHIFT)) & DMIC_CHANEN_EN_CH1_MASK)

#define DMIC_CHANEN_EN_CH2_MASK                  (0x4U)
#define DMIC_CHANEN_EN_CH2_SHIFT                 (2U)
/*! EN_CH2 - Enable Channel n
 *  0b0..PDM channel n is disabled.
 *  0b1..PDM channel n is enabled.
 */
#define DMIC_CHANEN_EN_CH2(x)                    (((uint32_t)(((uint32_t)(x)) << DMIC_CHANEN_EN_CH2_SHIFT)) & DMIC_CHANEN_EN_CH2_MASK)

#define DMIC_CHANEN_EN_CH3_MASK                  (0x8U)
#define DMIC_CHANEN_EN_CH3_SHIFT                 (3U)
/*! EN_CH3 - Enable Channel n
 *  0b0..PDM channel n is disabled.
 *  0b1..PDM channel n is enabled.
 */
#define DMIC_CHANEN_EN_CH3(x)                    (((uint32_t)(((uint32_t)(x)) << DMIC_CHANEN_EN_CH3_SHIFT)) & DMIC_CHANEN_EN_CH3_MASK)
/*! @} */

/*! @name USE2FS - Use 2 FS register */
/*! @{ */

#define DMIC_USE2FS_USE2FS_MASK                  (0x1U)
#define DMIC_USE2FS_USE2FS_SHIFT                 (0U)
/*! USE2FS - Use 2FS register
 *  0b0..Use 1 FS output for PCM data.
 *  0b1..Use 2 FS output for PCM data.
 */
#define DMIC_USE2FS_USE2FS(x)                    (((uint32_t)(((uint32_t)(x)) << DMIC_USE2FS_USE2FS_SHIFT)) & DMIC_USE2FS_USE2FS_MASK)
/*! @} */

/*! @name GLOBAL_SYCN_EN - Global Channel Synchronization Enable */
/*! @{ */

#define DMIC_GLOBAL_SYCN_EN_CH_SYNC_EN_MASK      (0xFU)
#define DMIC_GLOBAL_SYCN_EN_CH_SYNC_EN_SHIFT     (0U)
/*! CH_SYNC_EN - Channel synch enable */
#define DMIC_GLOBAL_SYCN_EN_CH_SYNC_EN(x)        (((uint32_t)(((uint32_t)(x)) << DMIC_GLOBAL_SYCN_EN_CH_SYNC_EN_SHIFT)) & DMIC_GLOBAL_SYCN_EN_CH_SYNC_EN_MASK)
/*! @} */

/*! @name GLOBAL_COUNT_VAL - Global channel synchronization counter value */
/*! @{ */

#define DMIC_GLOBAL_COUNT_VAL_CCOUNTVAL_MASK     (0xFFFFFFFFU)
#define DMIC_GLOBAL_COUNT_VAL_CCOUNTVAL_SHIFT    (0U)
/*! CCOUNTVAL - Channel Counter Value */
#define DMIC_GLOBAL_COUNT_VAL_CCOUNTVAL(x)       (((uint32_t)(((uint32_t)(x)) << DMIC_GLOBAL_COUNT_VAL_CCOUNTVAL_SHIFT)) & DMIC_GLOBAL_COUNT_VAL_CCOUNTVAL_MASK)
/*! @} */

/*! @name DECRESET - DMIC decimator reset */
/*! @{ */

#define DMIC_DECRESET_DECRESET_MASK              (0xFU)
#define DMIC_DECRESET_DECRESET_SHIFT             (0U)
/*! DECRESET - Decimator reset
 *  0b0000..Release reset to decimator
 *  0b0001..Assert reset to decimator
 */
#define DMIC_DECRESET_DECRESET(x)                (((uint32_t)(((uint32_t)(x)) << DMIC_DECRESET_DECRESET_SHIFT)) & DMIC_DECRESET_DECRESET_MASK)
/*! @} */

/*! @name HWVADGAIN - HWVAD Input Gain */
/*! @{ */

#define DMIC_HWVADGAIN_INPUTGAIN_MASK            (0xFU)
#define DMIC_HWVADGAIN_INPUTGAIN_SHIFT           (0U)
/*! INPUTGAIN
 *  0b0000..-10 bits
 *  0b0001..-8 bits
 *  0b0010..-6 bits
 *  0b0011..-4 bits
 *  0b0100..-2 bits
 *  0b0101..0 bits (default)
 *  0b0110..+2 bits
 *  0b0111..+4 bits
 *  0b1000..+6 bits
 *  0b1001..+8 bits
 *  0b1010..+10 bits
 *  0b1011..+12 bits
 *  0b1100..+14 bits
 *  0b1101-0b1111..Reserved
 */
#define DMIC_HWVADGAIN_INPUTGAIN(x)              (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADGAIN_INPUTGAIN_SHIFT)) & DMIC_HWVADGAIN_INPUTGAIN_MASK)
/*! @} */

/*! @name HWVADHPFS - HWVAD Filter Control */
/*! @{ */

#define DMIC_HWVADHPFS_HPFS_MASK                 (0x3U)
#define DMIC_HWVADHPFS_HPFS_SHIFT                (0U)
/*! HPFS - The HPFS field chooses the High Pass filter in first part of HWVAD.
 *  0b00..BYPASS. First filter by-pass.
 *  0b01..HIGH_PASS_1750HZ. High pass filter with -3dB cut-off at 1750 Hz.
 *  0b10..HIGH_PASS_215HZ. High pass filter with -3dB cut-off at 215 Hz.
 *  0b11..Reserved
 */
#define DMIC_HWVADHPFS_HPFS(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADHPFS_HPFS_SHIFT)) & DMIC_HWVADHPFS_HPFS_MASK)
/*! @} */

/*! @name HWVADST10 - HWVAD Control */
/*! @{ */

#define DMIC_HWVADST10_ST10_MASK                 (0x1U)
#define DMIC_HWVADST10_ST10_SHIFT                (0U)
/*! ST10 - STAGE 1
 *  0b0..Normal operation, waiting for HWVAD trigger event (stage 0).
 *  0b1..Reset internal interrupt flag by writing a '1' (stage 1) pulse.
 */
#define DMIC_HWVADST10_ST10(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADST10_ST10_SHIFT)) & DMIC_HWVADST10_ST10_MASK)
/*! @} */

/*! @name HWVADRSTT - HWVAD Filter Reset */
/*! @{ */

#define DMIC_HWVADRSTT_RSST_MASK                 (0x1U)
#define DMIC_HWVADRSTT_RSST_SHIFT                (0U)
/*! RSST - Reset HWVAD */
#define DMIC_HWVADRSTT_RSST(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADRSTT_RSST_SHIFT)) & DMIC_HWVADRSTT_RSST_MASK)
/*! @} */

/*! @name HWVADTHGN - HWVAD Noise Estimator Gain */
/*! @{ */

#define DMIC_HWVADTHGN_THGN_MASK                 (0xFU)
#define DMIC_HWVADTHGN_THGN_SHIFT                (0U)
/*! THGN - Gain Factor for Noise Estimator */
#define DMIC_HWVADTHGN_THGN(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADTHGN_THGN_SHIFT)) & DMIC_HWVADTHGN_THGN_MASK)
/*! @} */

/*! @name HWVADTHGS - HWVAD Signal Estimator Gain */
/*! @{ */

#define DMIC_HWVADTHGS_THGS_MASK                 (0xFU)
#define DMIC_HWVADTHGS_THGS_SHIFT                (0U)
/*! THGS - Signal Gain Factor */
#define DMIC_HWVADTHGS_THGS(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADTHGS_THGS_SHIFT)) & DMIC_HWVADTHGS_THGS_MASK)
/*! @} */

/*! @name HWVADLOWZ - HWVAD Noise Envelope Estimator */
/*! @{ */

#define DMIC_HWVADLOWZ_LOWZ_MASK                 (0xFFFFU)
#define DMIC_HWVADLOWZ_LOWZ_SHIFT                (0U)
/*! LOWZ - Average Noise-floor Value */
#define DMIC_HWVADLOWZ_LOWZ(x)                   (((uint32_t)(((uint32_t)(x)) << DMIC_HWVADLOWZ_LOWZ_SHIFT)) & DMIC_HWVADLOWZ_LOWZ_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group DMIC_Register_Masks */


/*!
 * @}
 */ /* end of group DMIC_Peripheral_Access_Layer */


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


#endif  /* DMIC_H_ */

