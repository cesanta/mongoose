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
**         CMSIS Peripheral Access Layer for ACOMP
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
 * @file ACOMP.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for ACOMP
 *
 * CMSIS Peripheral Access Layer for ACOMP
 */

#if !defined(ACOMP_H_)
#define ACOMP_H_                                 /**< Symbol preventing repeated inclusion */

#if (defined(CPU_RW610ETA2I) || defined(CPU_RW610HNA2I) || defined(CPU_RW610UKA2I))
#include "RW610_COMMON.h"
#elif (defined(CPU_RW612ETA2I) || defined(CPU_RW612HNA2I) || defined(CPU_RW612UKA2I))
#include "RW612_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   --
   ---------------------------------------------------------------------------- */

/* global Backward compatibility */
#define MRT0_IRQn                                MRT_IRQn
#define MRT0_IRQHandler                          MRT_IRQHandler
#define MRT0_DriverIRQHandler                    MRT_DriverIRQHandler
#define MRT1_IRQn                                GFMRT_IRQn
#define MRT1_IRQHandler                          GFMRT_IRQHandler
#define MRT1_DriverIRQHandler                    GFMRT_DriverIRQHandler
#define DMIC0_IRQn                               DMIC_IRQn
#define DMIC0_IRQHandler                         DMIC_IRQHandler
#define DMIC0_DriverIRQHandler                   DMIC_DriverIRQHandler
#define WFD_IRQn                                 WKDEEPSLEEP_IRQn
#define WFD_IRQHandler                           WKDEEPSLEEP_IRQHandler
#define WFD_DriverIRQHandler                     WKDEEPSLEEP_DriverIRQHandler
#define HWVAD0_IRQn                              HWVAD_IRQn
#define HWVAD0_IRQHandler                        HWVAD_IRQHandler
#define HWVAD0_DriverIRQHandler                  HWVAD_DriverIRQHandler
#define OS_EVENT_IRQn                            OS_EVENT_TIMER_IRQn
#define OS_EVENT_IRQHandler                      OS_EVENT_TIMER_IRQHandler
#define OS_EVENT_DriverIRQHandler                OS_EVENT_TIMER_DriverIRQHandler
#define SDU_IRQn                                 SDIO_IRQn
#define SDU_IRQHandler                           SDIO_IRQHandler
#define SDU_DriverIRQHandler                     SDIO_DriverIRQHandler
#define LCDIC_IRQn                               LCD_IRQn
#define LCDIC_IRQHandler                         LCD_IRQHandler
#define LCDIC_DriverIRQHandler                   LCD_DriverIRQHandler
#define CAPT_PULSE_IRQn                          CAPTIMER_IRQn
#define CAPT_PULSE_IRQHandler                    CAPTIMER_IRQHandler
#define CAPT_PULSE_DriverIRQHandler              CAPTIMER_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE0_IRQn                 W2MWKUP_DONE0_IRQn
#define WL_MCI_WAKEUP_DONE0_IRQHandler           W2MWKUP_DONE0_IRQHandler
#define WL_MCI_WAKEUP_DONE0_DriverIRQHandler     W2MWKUP_DONE0_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE1_IRQn                 W2MWKUP_DONE1_IRQn
#define WL_MCI_WAKEUP_DONE1_IRQHandler           W2MWKUP_DONE1_IRQHandler
#define WL_MCI_WAKEUP_DONE1_DriverIRQHandler     W2MWKUP_DONE1_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE2_IRQn                 W2MWKUP_DONE2_IRQn
#define WL_MCI_WAKEUP_DONE2_IRQHandler           W2MWKUP_DONE2_IRQHandler
#define WL_MCI_WAKEUP_DONE2_DriverIRQHandler     W2MWKUP_DONE2_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE3_IRQn                 W2MWKUP_DONE3_IRQn
#define WL_MCI_WAKEUP_DONE3_IRQHandler           W2MWKUP_DONE3_IRQHandler
#define WL_MCI_WAKEUP_DONE3_DriverIRQHandler     W2MWKUP_DONE3_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE4_IRQn                 W2MWKUP_DONE4_IRQn
#define WL_MCI_WAKEUP_DONE4_IRQHandler           W2MWKUP_DONE4_IRQHandler
#define WL_MCI_WAKEUP_DONE4_DriverIRQHandler     W2MWKUP_DONE4_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE5_IRQn                 W2MWKUP_DONE5_IRQn
#define WL_MCI_WAKEUP_DONE5_IRQHandler           W2MWKUP_DONE5_IRQHandler
#define WL_MCI_WAKEUP_DONE5_DriverIRQHandler     W2MWKUP_DONE5_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE6_IRQn                 W2MWKUP_DONE6_IRQn
#define WL_MCI_WAKEUP_DONE6_IRQHandler           W2MWKUP_DONE6_IRQHandler
#define WL_MCI_WAKEUP_DONE6_DriverIRQHandler     W2MWKUP_DONE6_DriverIRQHandler
#define WL_MCI_WAKEUP_DONE7_IRQn                 W2MWKUP_DONE7_IRQn
#define WL_MCI_WAKEUP_DONE7_IRQHandler           W2MWKUP_DONE7_IRQHandler
#define WL_MCI_WAKEUP_DONE7_DriverIRQHandler     W2MWKUP_DONE7_DriverIRQHandler
#define WL_MCI_WAKEUP0_IRQn                      W2MWKUP0_IRQn
#define WL_MCI_WAKEUP0_IRQHandler                W2MWKUP0_IRQHandler
#define WL_MCI_WAKEUP0_DriverIRQHandler          W2MWKUP0_DriverIRQHandler
#define WL_MCI_WAKEUP1_IRQn                      W2MWKUP1_IRQn
#define WL_MCI_WAKEUP1_IRQHandler                W2MWKUP1_IRQHandler
#define WL_MCI_WAKEUP1_DriverIRQHandler          W2MWKUP1_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE0_IRQn                B2MWKUP_DONE0_IRQn
#define BLE_MCI_WAKEUP_DONE0_IRQHandler          B2MWKUP_DONE0_IRQHandler
#define BLE_MCI_WAKEUP_DONE0_DriverIRQHandler    B2MWKUP_DONE0_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE1_IRQn                B2MWKUP_DONE1_IRQn
#define BLE_MCI_WAKEUP_DONE1_IRQHandler          B2MWKUP_DONE1_IRQHandler
#define BLE_MCI_WAKEUP_DONE1_DriverIRQHandler    B2MWKUP_DONE1_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE2_IRQn                B2MWKUP_DONE2_IRQn
#define BLE_MCI_WAKEUP_DONE2_IRQHandler          B2MWKUP_DONE2_IRQHandler
#define BLE_MCI_WAKEUP_DONE2_DriverIRQHandler    B2MWKUP_DONE2_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE3_IRQn                B2MWKUP_DONE3_IRQn
#define BLE_MCI_WAKEUP_DONE3_IRQHandler          B2MWKUP_DONE3_IRQHandler
#define BLE_MCI_WAKEUP_DONE3_DriverIRQHandler    B2MWKUP_DONE3_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE4_IRQn                B2MWKUP_DONE4_IRQn
#define BLE_MCI_WAKEUP_DONE4_IRQHandler          B2MWKUP_DONE4_IRQHandler
#define BLE_MCI_WAKEUP_DONE4_DriverIRQHandler    B2MWKUP_DONE4_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE5_IRQn                B2MWKUP_DONE5_IRQn
#define BLE_MCI_WAKEUP_DONE5_IRQHandler          B2MWKUP_DONE5_IRQHandler
#define BLE_MCI_WAKEUP_DONE5_DriverIRQHandler    B2MWKUP_DONE5_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE6_IRQn                B2MWKUP_DONE6_IRQn
#define BLE_MCI_WAKEUP_DONE6_IRQHandler          B2MWKUP_DONE6_IRQHandler
#define BLE_MCI_WAKEUP_DONE6_DriverIRQHandler    B2MWKUP_DONE6_DriverIRQHandler
#define BLE_MCI_WAKEUP_DONE7_IRQn                B2MWKUP_DONE7_IRQn
#define BLE_MCI_WAKEUP_DONE7_IRQHandler          B2MWKUP_DONE7_IRQHandler
#define BLE_MCI_WAKEUP_DONE7_DriverIRQHandler    B2MWKUP_DONE7_DriverIRQHandler
#define BLE_MCI_WAKEUP0_IRQn                     B2MWKUP0_IRQn
#define BLE_MCI_WAKEUP0_IRQHandler               B2MWKUP0_IRQHandler
#define BLE_MCI_WAKEUP0_DriverIRQHandler         B2MWKUP0_DriverIRQHandler
#define BLE_MCI_WAKEUP1_IRQn                     B2MWKUP1_IRQn
#define BLE_MCI_WAKEUP1_IRQHandler               B2MWKUP1_IRQHandler
#define BLE_MCI_WAKEUP1_DriverIRQHandler         B2MWKUP1_DriverIRQHandler
#define ELS_GDET_IRQ_IRQn                        ELS_GDET_IRQn
#define ELS_GDET_IRQ_IRQHandler                  ELS_GDET_IRQHandler
#define ELS_GDET_IRQ_DriverIRQHandler            ELS_GDET_DriverIRQHandler
#define ELS_GDET_ERR_IRQn                        ELS_GDET_UM_IRQn
#define ELS_GDET_ERR_IRQHandler                  ELS_GDET_UM_IRQHandler
#define ELS_GDET_ERR_DriverIRQHandler            ELS_GDET_UM_DriverIRQHandler
#define PKC_IRQn                                 PKC_INT_IRQn
#define PKC_IRQHandler                           PKC_INT_IRQHandler
#define PKC_DriverIRQHandler                     PKC_INT_DriverIRQHandler
#define CDOG_IRQn                                CDOG_INT_IRQn
#define CDOG_IRQHandler                          CDOG_INT_IRQHandler
#define CDOG_DriverIRQHandler                    CDOG_INT_DriverIRQHandler
#define GAU_GPDAC_INT_FUNC11_IRQn                GAU_DAC_IRQn
#define GAU_GPDAC_INT_FUNC11_IRQHandler          GAU_DAC_IRQHandler
#define GAU_GPDAC_INT_FUNC11_DriverIRQHandler    GAU_DAC_DriverIRQHandler
#define GAU_ACOMP_INT_WKUP11_IRQn                GAU_ACOMP_WKUP_IRQn
#define GAU_ACOMP_INT_WKUP11_IRQHandler          GAU_ACOMP_WKUP_IRQHandler
#define GAU_ACOMP_INT_WKUP11_DriverIRQHandler    GAU_ACOMP_WKUP_DriverIRQHandler
#define GAU_ACOMP_INT_FUNC11_IRQn                GAU_ACOMP_IRQn
#define GAU_ACOMP_INT_FUNC11_IRQHandler          GAU_ACOMP_IRQHandler
#define GAU_ACOMP_INT_FUNC11_DriverIRQHandler    GAU_ACOMP_DriverIRQHandler
#define GAU_GPADC1_INT_FUNC11_IRQn               GAU_ADC1_IRQn
#define GAU_GPADC1_INT_FUNC11_IRQHandler         GAU_ADC1_IRQHandler
#define GAU_GPADC1_INT_FUNC11_DriverIRQHandler   GAU_ADC1_DriverIRQHandler
#define GAU_GPADC0_INT_FUNC11_IRQn               GAU_ADC0_IRQn
#define GAU_GPADC0_INT_FUNC11_IRQHandler         GAU_ADC0_IRQHandler
#define GAU_GPADC0_INT_FUNC11_DriverIRQHandler   GAU_ADC0_DriverIRQHandler
#define OCOTP_IRQn                               OTP_IRQn
#define OCOTP_IRQHandler                         OTP_IRQHandler
#define OCOTP_DriverIRQHandler                   OTP_DriverIRQHandler
#define BOD_1_85_INT_IRQn                        PMIP_IRQn
#define BOD_1_85_INT_IRQHandler                  PMIP_IRQHandler
#define BOD_1_85_INT_DriverIRQHandler            PMIP_DriverIRQHandler
#define BOD_1_85_NEG_IRQn                        PMIP_CHANGE_IRQn
#define BOD_1_85_NEG_IRQHandler                  PMIP_CHANGE_IRQHandler
#define BOD_1_85_NEG_DriverIRQHandler            PMIP_CHANGE_DriverIRQHandler
#define AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQn               ACC_C_INT_IRQn
#define AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_IRQHandler         ACC_C_INT_IRQHandler
#define AHB_MEM_ACC_CHECKER_VIO_INT_C_OR_DriverIRQHandler   ACC_C_INT_DriverIRQHandler
#define AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQn               ACC_S_INT_IRQn
#define AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_IRQHandler         ACC_S_INT_IRQHandler
#define AHB_MEM_ACC_CHECKER_VIO_INT_S_OR_DriverIRQHandler   ACC_S_INT_DriverIRQHandler
#define WL_ACC_INT_IRQn                          WACC_IRQn
#define WL_ACC_INT_IRQHandler                    WACC_IRQHandler
#define WL_ACC_INT_DriverIRQHandler              WACC_DriverIRQHandler
#define BLE_ACC_INT_IRQn                         BACC_IRQn
#define BLE_ACC_INT_IRQHandler                   BACC_IRQHandler
#define BLE_ACC_INT_DriverIRQHandler             BACC_DriverIRQHandler


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
   -- ACOMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ACOMP_Peripheral_Access_Layer ACOMP Peripheral Access Layer
 * @{
 */

/** ACOMP - Register Layout Typedef */
typedef struct {
  __IO uint32_t CTRL0;                             /**< ACOMP0 Control Register, offset: 0x0 */
  __IO uint32_t CTRL1;                             /**< ACOMP1 Control Register, offset: 0x4 */
  __I  uint32_t STATUS0;                           /**< ACOMP0 Status Register, offset: 0x8 */
  __I  uint32_t STATUS1;                           /**< ACOMP1 Status Register, offset: 0xC */
  __IO uint32_t ROUTE0;                            /**< ACOMP0 Route Register, offset: 0x10 */
  __IO uint32_t ROUTE1;                            /**< ACOMP1 Route Register, offset: 0x14 */
  __I  uint32_t ISR0;                              /**< ACOMP0 Interrupt Status Register, offset: 0x18 */
  __I  uint32_t ISR1;                              /**< ACOMP1 Interrupt Status Register, offset: 0x1C */
  __IO uint32_t IMR0;                              /**< ACOMP0 Interrupt Mask Register, offset: 0x20 */
  __IO uint32_t IMR1;                              /**< ACOMP1 Interrupt Mask Register, offset: 0x24 */
  __I  uint32_t IRSR0;                             /**< ACOMP0 Interrupt Raw Status Register, offset: 0x28 */
  __I  uint32_t IRSR1;                             /**< ACOMP1 Interrupt Raw Status Register, offset: 0x2C */
  __IO uint32_t ICR0;                              /**< ACOMP0 Interrupt Clear Register, offset: 0x30 */
  __IO uint32_t ICR1;                              /**< ACOMP1 Interrupt Clear Register, offset: 0x34 */
  __IO uint32_t RST0;                              /**< ACOMP0 Soft Reset Register, offset: 0x38 */
  __IO uint32_t RST1;                              /**< ACOMP1 Soft Reset Register, offset: 0x3C */
       uint8_t RESERVED_0[8];
  __IO uint32_t CLK;                               /**< Clock Register, offset: 0x48 */
} ACOMP_Type;

/* ----------------------------------------------------------------------------
   -- ACOMP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ACOMP_Register_Masks ACOMP Register Masks
 * @{
 */

/*! @name CTRL0 - ACOMP0 Control Register */
/*! @{ */

#define ACOMP_CTRL0_EN_MASK                      (0x1U)
#define ACOMP_CTRL0_EN_SHIFT                     (0U)
/*! EN - ACOMP0 enable
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL0_EN(x)                        (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_EN_SHIFT)) & ACOMP_CTRL0_EN_MASK)

#define ACOMP_CTRL0_GPIOINV_MASK                 (0x2U)
#define ACOMP_CTRL0_GPIOINV_SHIFT                (1U)
/*! GPIOINV - Enable/Disable inversion of ACOMP0 output to GPIO.
 *  0b0..do not invert ACOMP0 output
 *  0b1..invert ACOMP0 output
 */
#define ACOMP_CTRL0_GPIOINV(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_GPIOINV_SHIFT)) & ACOMP_CTRL0_GPIOINV_MASK)

#define ACOMP_CTRL0_WARMTIME_MASK                (0xCU)
#define ACOMP_CTRL0_WARMTIME_SHIFT               (2U)
/*! WARMTIME - Set ACOMP0 Warm-Up time
 *  0b00..1 us
 *  0b01..2 us
 *  0b10..4 us
 *  0b11..8 us
 */
#define ACOMP_CTRL0_WARMTIME(x)                  (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_WARMTIME_SHIFT)) & ACOMP_CTRL0_WARMTIME_MASK)

#define ACOMP_CTRL0_HYST_SELN_MASK               (0x70U)
#define ACOMP_CTRL0_HYST_SELN_SHIFT              (4U)
/*! HYST_SELN - Select ACOMP0 negative hysteresis voltage level.
 *  0b000..no hysteresis
 *  0b001..-10 mV hysteresis
 *  0b010..-20 mV hysteresis
 *  0b011..-30 mV hysteresis
 *  0b100..-40 mV hysteresis
 *  0b101..-50 mV hysteresis
 *  0b110..-60 mV hysteresis
 *  0b111..-70 mV hysteresis
 */
#define ACOMP_CTRL0_HYST_SELN(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_HYST_SELN_SHIFT)) & ACOMP_CTRL0_HYST_SELN_MASK)

#define ACOMP_CTRL0_HYST_SELP_MASK               (0x380U)
#define ACOMP_CTRL0_HYST_SELP_SHIFT              (7U)
/*! HYST_SELP - Select ACOMP0 positive hysteresis voltage level.
 *  0b000..No hysteresis
 *  0b001..+10 mV hysteresis
 *  0b010..+20 mV hysteresis
 *  0b011..+30 mV hysteresis
 *  0b100..+40 mV hysteresis
 *  0b101..+50 mV hysteresis
 *  0b110..+60 mV hysteresis
 *  0b111..+70 mV hysteresis
 */
#define ACOMP_CTRL0_HYST_SELP(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_HYST_SELP_SHIFT)) & ACOMP_CTRL0_HYST_SELP_MASK)

#define ACOMP_CTRL0_BIAS_PROG_MASK               (0xC00U)
#define ACOMP_CTRL0_BIAS_PROG_SHIFT              (10U)
/*! BIAS_PROG - ACOMP0 bias current control bits or response time control bits.
 *  0b00..power mode1 (fast response mode)
 *  0b01..power mode2 (medium response mode)
 *  0b10..power mode3 (slow response mode)
 *  0b11..Reserved
 */
#define ACOMP_CTRL0_BIAS_PROG(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_BIAS_PROG_SHIFT)) & ACOMP_CTRL0_BIAS_PROG_MASK)

#define ACOMP_CTRL0_LEVEL_SEL_MASK               (0x3F000U)
#define ACOMP_CTRL0_LEVEL_SEL_SHIFT              (12U)
/*! LEVEL_SEL - Scaling factor select bits for VIO reference level.
 *  0b000000..Scaling factor=0.25
 *  0b010000..Scaling factor= 0.5
 *  0b100000..Scaling factor= 0.75
 *  0b110000..Scaling factor= 1
 */
#define ACOMP_CTRL0_LEVEL_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_LEVEL_SEL_SHIFT)) & ACOMP_CTRL0_LEVEL_SEL_MASK)

#define ACOMP_CTRL0_NEG_SEL_MASK                 (0x3C0000U)
#define ACOMP_CTRL0_NEG_SEL_SHIFT                (18U)
/*! NEG_SEL - ACOMP0 negative input select bits.
 *  0b0000..acomp_ch<0>
 *  0b0001..acomp_ch<1>
 *  0b0010..acomp_ch<2>
 *  0b0011..acomp_ch<3>
 *  0b0100..acomp_ch<4>
 *  0b0101..acomp_ch<5>
 *  0b0110..acomp_ch<6>
 *  0b0111..acomp_ch<7>
 *  0b1000..
 *  0b1001..
 *  0b1010..
 *  0b1011..
 *  0b1100..vio*scaling factor
 *  0b1101..vio*scaling factor
 *  0b1110..vio*scaling factor
 *  0b1111..vio*scaling factor
 */
#define ACOMP_CTRL0_NEG_SEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_NEG_SEL_SHIFT)) & ACOMP_CTRL0_NEG_SEL_MASK)

#define ACOMP_CTRL0_POS_SEL_MASK                 (0x3C00000U)
#define ACOMP_CTRL0_POS_SEL_SHIFT                (22U)
/*! POS_SEL - ACOMP0 positive input select bits.
 *  0b0000..acomp_ch<0>
 *  0b0001..acomp_ch<1>
 *  0b0010..acomp_ch<2>
 *  0b0011..acomp_ch<3>
 *  0b0100..acomp_ch<4>
 *  0b0101..acomp_ch<5>
 *  0b0110..acomp_ch<6>
 *  0b0111..acomp_ch<7>
 *  0b1000..
 *  0b1001..
 */
#define ACOMP_CTRL0_POS_SEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_POS_SEL_SHIFT)) & ACOMP_CTRL0_POS_SEL_MASK)

#define ACOMP_CTRL0_MUXEN_MASK                   (0x4000000U)
#define ACOMP_CTRL0_MUXEN_SHIFT                  (26U)
/*! MUXEN - ACOMP0 input MUX enable bit.
 *  0b0..disable input mux
 *  0b1..enable input mux
 */
#define ACOMP_CTRL0_MUXEN(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_MUXEN_SHIFT)) & ACOMP_CTRL0_MUXEN_MASK)

#define ACOMP_CTRL0_INACT_VAL_MASK               (0x8000000U)
#define ACOMP_CTRL0_INACT_VAL_SHIFT              (27U)
/*! INACT_VAL - Set output value when ACOMP0 is inactive.
 *  0b0..output 0 when ACOMP0 is inactive
 *  0b1..output 1 when ACOMP0 is inactive
 */
#define ACOMP_CTRL0_INACT_VAL(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_INACT_VAL_SHIFT)) & ACOMP_CTRL0_INACT_VAL_MASK)

#define ACOMP_CTRL0_RIE_MASK                     (0x10000000U)
#define ACOMP_CTRL0_RIE_SHIFT                    (28U)
/*! RIE - ACOMP0 enable/disable rising edge triggered edge pulse.
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL0_RIE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_RIE_SHIFT)) & ACOMP_CTRL0_RIE_MASK)

#define ACOMP_CTRL0_FIE_MASK                     (0x20000000U)
#define ACOMP_CTRL0_FIE_SHIFT                    (29U)
/*! FIE - ACOMP0 enable/disable falling edge triggered edge pulse.
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL0_FIE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_FIE_SHIFT)) & ACOMP_CTRL0_FIE_MASK)

#define ACOMP_CTRL0_INT_ACT_HI_MASK              (0x40000000U)
#define ACOMP_CTRL0_INT_ACT_HI_SHIFT             (30U)
/*! INT_ACT_HI - ACOMP0 interrupt active mode select.
 *  0b0..Low level or falling edge triggered interrupt
 *  0b1..High level or rising edge triggered interrupt
 */
#define ACOMP_CTRL0_INT_ACT_HI(x)                (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_INT_ACT_HI_SHIFT)) & ACOMP_CTRL0_INT_ACT_HI_MASK)

#define ACOMP_CTRL0_EDGE_LEVL_SEL_MASK           (0x80000000U)
#define ACOMP_CTRL0_EDGE_LEVL_SEL_SHIFT          (31U)
/*! EDGE_LEVL_SEL - ACOMP0 interrupt type select.
 *  0b0..level triggered interrupt
 *  0b1..edge triggered interrupt
 */
#define ACOMP_CTRL0_EDGE_LEVL_SEL(x)             (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL0_EDGE_LEVL_SEL_SHIFT)) & ACOMP_CTRL0_EDGE_LEVL_SEL_MASK)
/*! @} */

/*! @name CTRL1 - ACOMP1 Control Register */
/*! @{ */

#define ACOMP_CTRL1_EN_MASK                      (0x1U)
#define ACOMP_CTRL1_EN_SHIFT                     (0U)
/*! EN - ACOMP1 enable bit.
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL1_EN(x)                        (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_EN_SHIFT)) & ACOMP_CTRL1_EN_MASK)

#define ACOMP_CTRL1_GPIOINV_MASK                 (0x2U)
#define ACOMP_CTRL1_GPIOINV_SHIFT                (1U)
/*! GPIOINV - Enable/disable inversion of ACOMP1 output to GPIO.
 *  0b0..do not invert ACOMP1 output
 *  0b1..invert ACOMP1 output
 */
#define ACOMP_CTRL1_GPIOINV(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_GPIOINV_SHIFT)) & ACOMP_CTRL1_GPIOINV_MASK)

#define ACOMP_CTRL1_WARMTIME_MASK                (0xCU)
#define ACOMP_CTRL1_WARMTIME_SHIFT               (2U)
/*! WARMTIME - Set ACOMP1 warm-up time.
 *  0b00..1 us
 *  0b01..2 us
 *  0b10..4 us
 *  0b11..8 us
 */
#define ACOMP_CTRL1_WARMTIME(x)                  (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_WARMTIME_SHIFT)) & ACOMP_CTRL1_WARMTIME_MASK)

#define ACOMP_CTRL1_HYST_SELN_MASK               (0x70U)
#define ACOMP_CTRL1_HYST_SELN_SHIFT              (4U)
/*! HYST_SELN - Select ACOMP1 negative hysteresis voltage level.
 *  0b000..No hysteresis
 *  0b001..-10 mV hysteresis
 *  0b010..-20 mV hysteresis
 *  0b011..-30 mV hysteresis
 *  0b100..-40 mV hysteresis
 *  0b101..-50 mV hysteresis
 *  0b110..-60 mV hysteresis
 *  0b111..-70 mV hysteresis
 */
#define ACOMP_CTRL1_HYST_SELN(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_HYST_SELN_SHIFT)) & ACOMP_CTRL1_HYST_SELN_MASK)

#define ACOMP_CTRL1_HYST_SELP_MASK               (0x380U)
#define ACOMP_CTRL1_HYST_SELP_SHIFT              (7U)
/*! HYST_SELP - Select ACOMP1 positive hysteresis voltage level.
 *  0b000..No hysteresis
 *  0b001..+10 mV hysteresis
 *  0b010..+20 mV hysteresis
 *  0b011..+30 mV hysteresis
 *  0b100..+40 mV hysteresis
 *  0b101..+50 mV hysteresis
 *  0b110..+60 mV hysteresis
 *  0b111..+70 mV hysteresis
 */
#define ACOMP_CTRL1_HYST_SELP(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_HYST_SELP_SHIFT)) & ACOMP_CTRL1_HYST_SELP_MASK)

#define ACOMP_CTRL1_BIAS_PROG_MASK               (0xC00U)
#define ACOMP_CTRL1_BIAS_PROG_SHIFT              (10U)
/*! BIAS_PROG - ACOMP1 bias current control bits Or response time control bits.
 *  0b00..power mode1 (fast response mode)
 *  0b01..power mode2 (Medium response mode)
 *  0b10..power mode3 (slow response mode)
 *  0b11..Reserved
 */
#define ACOMP_CTRL1_BIAS_PROG(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_BIAS_PROG_SHIFT)) & ACOMP_CTRL1_BIAS_PROG_MASK)

#define ACOMP_CTRL1_LEVEL_SEL_MASK               (0x3F000U)
#define ACOMP_CTRL1_LEVEL_SEL_SHIFT              (12U)
/*! LEVEL_SEL - Scaling factor select bits for vio reference level.
 *  0b000000..Scaling factor=0.25
 *  0b010000..Scaling factor= 0.5
 *  0b100000..Scaling factor= 0.75
 *  0b110000..Scaling factor= 1
 */
#define ACOMP_CTRL1_LEVEL_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_LEVEL_SEL_SHIFT)) & ACOMP_CTRL1_LEVEL_SEL_MASK)

#define ACOMP_CTRL1_NEG_SEL_MASK                 (0x3C0000U)
#define ACOMP_CTRL1_NEG_SEL_SHIFT                (18U)
/*! NEG_SEL - ACOMP1 negative input select
 *  0b0000..acomp_ch<0>
 *  0b0001..acomp_ch<1>
 *  0b0010..acomp_ch<2>
 *  0b0011..acomp_ch<3>
 *  0b0100..acomp_ch<4>
 *  0b0101..acomp_ch<5>
 *  0b0110..acomp_ch<6>
 *  0b0111..acomp_ch<7>
 *  0b1000..
 *  0b1001..
 *  0b1010..
 *  0b1011..
 *  0b1100..vio*scaling factor
 *  0b1101..vio*scaling factor
 *  0b1110..vio*scaling factor
 *  0b1111..vio*scaling factor
 */
#define ACOMP_CTRL1_NEG_SEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_NEG_SEL_SHIFT)) & ACOMP_CTRL1_NEG_SEL_MASK)

#define ACOMP_CTRL1_POS_SEL_MASK                 (0x3C00000U)
#define ACOMP_CTRL1_POS_SEL_SHIFT                (22U)
/*! POS_SEL - ACOMP1 positive input select
 *  0b0000..acomp_ch<0>
 *  0b0001..acomp_ch<1>
 *  0b0010..acomp_ch<2>
 *  0b0011..acomp_ch<3>
 *  0b0100..acomp_ch<4>
 *  0b0101..acomp_ch<5>
 *  0b0110..acomp_ch<6>
 *  0b0111..acomp_ch<7>
 *  0b1000..
 *  0b1001..
 */
#define ACOMP_CTRL1_POS_SEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_POS_SEL_SHIFT)) & ACOMP_CTRL1_POS_SEL_MASK)

#define ACOMP_CTRL1_MUXEN_MASK                   (0x4000000U)
#define ACOMP_CTRL1_MUXEN_SHIFT                  (26U)
/*! MUXEN - ACOMP1 input MUX enable
 *  0b0..disable input mux
 *  0b1..enable input mux
 */
#define ACOMP_CTRL1_MUXEN(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_MUXEN_SHIFT)) & ACOMP_CTRL1_MUXEN_MASK)

#define ACOMP_CTRL1_INACT_VAL_MASK               (0x8000000U)
#define ACOMP_CTRL1_INACT_VAL_SHIFT              (27U)
/*! INACT_VAL - Set output value when ACOMP1 is inactive.
 *  0b0..output 0 when ACOMP1 is inactive
 *  0b1..output 1 when ACOMP1 is inactive
 */
#define ACOMP_CTRL1_INACT_VAL(x)                 (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_INACT_VAL_SHIFT)) & ACOMP_CTRL1_INACT_VAL_MASK)

#define ACOMP_CTRL1_RIE_MASK                     (0x10000000U)
#define ACOMP_CTRL1_RIE_SHIFT                    (28U)
/*! RIE - ACOMP1 enable/disable rising edge triggered edge pulse.
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL1_RIE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_RIE_SHIFT)) & ACOMP_CTRL1_RIE_MASK)

#define ACOMP_CTRL1_FIE_MASK                     (0x20000000U)
#define ACOMP_CTRL1_FIE_SHIFT                    (29U)
/*! FIE - ACOMP1 enable/disable falling edge triggered edge pulse.
 *  0b0..
 *  0b1..
 */
#define ACOMP_CTRL1_FIE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_FIE_SHIFT)) & ACOMP_CTRL1_FIE_MASK)

#define ACOMP_CTRL1_INT_ACT_HI_MASK              (0x40000000U)
#define ACOMP_CTRL1_INT_ACT_HI_SHIFT             (30U)
/*! INT_ACT_HI - ACOMP1 interrupt active mode select.
 *  0b0..Low level or falling edge triggered interrupt
 *  0b1..High level or rising edge triggered interrupt
 */
#define ACOMP_CTRL1_INT_ACT_HI(x)                (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_INT_ACT_HI_SHIFT)) & ACOMP_CTRL1_INT_ACT_HI_MASK)

#define ACOMP_CTRL1_EDGE_LEVL_SEL_MASK           (0x80000000U)
#define ACOMP_CTRL1_EDGE_LEVL_SEL_SHIFT          (31U)
/*! EDGE_LEVL_SEL - ACOMP1 interrupt type select.
 *  0b0..level triggered interrupt
 *  0b1..edge triggered interrupt
 */
#define ACOMP_CTRL1_EDGE_LEVL_SEL(x)             (((uint32_t)(((uint32_t)(x)) << ACOMP_CTRL1_EDGE_LEVL_SEL_SHIFT)) & ACOMP_CTRL1_EDGE_LEVL_SEL_MASK)
/*! @} */

/*! @name STATUS0 - ACOMP0 Status Register */
/*! @{ */

#define ACOMP_STATUS0_ACT_MASK                   (0x1U)
#define ACOMP_STATUS0_ACT_SHIFT                  (0U)
/*! ACT - ACOMP0 active status.
 *  0b0..ACOMP0 is inactive
 *  0b1..ACOMP0 is active
 */
#define ACOMP_STATUS0_ACT(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_STATUS0_ACT_SHIFT)) & ACOMP_STATUS0_ACT_MASK)

#define ACOMP_STATUS0_OUT_MASK                   (0x2U)
#define ACOMP_STATUS0_OUT_SHIFT                  (1U)
/*! OUT - ACOMP0 comparison output value. */
#define ACOMP_STATUS0_OUT(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_STATUS0_OUT_SHIFT)) & ACOMP_STATUS0_OUT_MASK)
/*! @} */

/*! @name STATUS1 - ACOMP1 Status Register */
/*! @{ */

#define ACOMP_STATUS1_ACT_MASK                   (0x1U)
#define ACOMP_STATUS1_ACT_SHIFT                  (0U)
/*! ACT - ACOMP1 active status.
 *  0b0..ACOMP1 is inactive
 *  0b1..ACOMP1 is active
 */
#define ACOMP_STATUS1_ACT(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_STATUS1_ACT_SHIFT)) & ACOMP_STATUS1_ACT_MASK)

#define ACOMP_STATUS1_OUT_MASK                   (0x2U)
#define ACOMP_STATUS1_OUT_SHIFT                  (1U)
/*! OUT - ACOMP1 comparison output value. */
#define ACOMP_STATUS1_OUT(x)                     (((uint32_t)(((uint32_t)(x)) << ACOMP_STATUS1_OUT_SHIFT)) & ACOMP_STATUS1_OUT_MASK)
/*! @} */

/*! @name ROUTE0 - ACOMP0 Route Register */
/*! @{ */

#define ACOMP_ROUTE0_OUTSEL_MASK                 (0x1U)
#define ACOMP_ROUTE0_OUTSEL_SHIFT                (0U)
/*! OUTSEL - Select ACOMP0 synchronous or asynchronous output to pin.
 *  0b0..Synchronous output
 *  0b1..Asynchronous output
 */
#define ACOMP_ROUTE0_OUTSEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_ROUTE0_OUTSEL_SHIFT)) & ACOMP_ROUTE0_OUTSEL_MASK)

#define ACOMP_ROUTE0_PE_MASK                     (0x2U)
#define ACOMP_ROUTE0_PE_SHIFT                    (1U)
/*! PE - Enable/disable ACOMP0 output to pin.
 *  0b0..
 *  0b1..
 */
#define ACOMP_ROUTE0_PE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_ROUTE0_PE_SHIFT)) & ACOMP_ROUTE0_PE_MASK)
/*! @} */

/*! @name ROUTE1 - ACOMP1 Route Register */
/*! @{ */

#define ACOMP_ROUTE1_OUTSEL_MASK                 (0x1U)
#define ACOMP_ROUTE1_OUTSEL_SHIFT                (0U)
/*! OUTSEL - Select ACOMP1 synchronous or asynchronous output to pin
 *  0b0..Synchronous output
 *  0b1..Asynchronous output
 */
#define ACOMP_ROUTE1_OUTSEL(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_ROUTE1_OUTSEL_SHIFT)) & ACOMP_ROUTE1_OUTSEL_MASK)

#define ACOMP_ROUTE1_PE_MASK                     (0x2U)
#define ACOMP_ROUTE1_PE_SHIFT                    (1U)
/*! PE - Enable/disable ACOMP1 output to pin
 *  0b0..
 *  0b1..
 */
#define ACOMP_ROUTE1_PE(x)                       (((uint32_t)(((uint32_t)(x)) << ACOMP_ROUTE1_PE_SHIFT)) & ACOMP_ROUTE1_PE_MASK)
/*! @} */

/*! @name ISR0 - ACOMP0 Interrupt Status Register */
/*! @{ */

#define ACOMP_ISR0_OUT_INT_MASK                  (0x1U)
#define ACOMP_ISR0_OUT_INT_SHIFT                 (0U)
/*! OUT_INT - ACOMP0 Synchronized Output Interrupt */
#define ACOMP_ISR0_OUT_INT(x)                    (((uint32_t)(((uint32_t)(x)) << ACOMP_ISR0_OUT_INT_SHIFT)) & ACOMP_ISR0_OUT_INT_MASK)

#define ACOMP_ISR0_OUTA_INT_MASK                 (0x2U)
#define ACOMP_ISR0_OUTA_INT_SHIFT                (1U)
/*! OUTA_INT - ACOMP0 Asynchronized Output Interrupt */
#define ACOMP_ISR0_OUTA_INT(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_ISR0_OUTA_INT_SHIFT)) & ACOMP_ISR0_OUTA_INT_MASK)
/*! @} */

/*! @name ISR1 - ACOMP1 Interrupt Status Register */
/*! @{ */

#define ACOMP_ISR1_OUT_INT_MASK                  (0x1U)
#define ACOMP_ISR1_OUT_INT_SHIFT                 (0U)
/*! OUT_INT - ACOMP1 Synchronized Output Interrupt */
#define ACOMP_ISR1_OUT_INT(x)                    (((uint32_t)(((uint32_t)(x)) << ACOMP_ISR1_OUT_INT_SHIFT)) & ACOMP_ISR1_OUT_INT_MASK)

#define ACOMP_ISR1_OUTA_INT_MASK                 (0x2U)
#define ACOMP_ISR1_OUTA_INT_SHIFT                (1U)
/*! OUTA_INT - ACOMP1 Asynchronized Output Interrupt */
#define ACOMP_ISR1_OUTA_INT(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_ISR1_OUTA_INT_SHIFT)) & ACOMP_ISR1_OUTA_INT_MASK)
/*! @} */

/*! @name IMR0 - ACOMP0 Interrupt Mask Register */
/*! @{ */

#define ACOMP_IMR0_OUT_INT_MASK_MASK             (0x1U)
#define ACOMP_IMR0_OUT_INT_MASK_SHIFT            (0U)
/*! OUT_INT_MASK - Mask Synchronized Interrupt */
#define ACOMP_IMR0_OUT_INT_MASK(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_IMR0_OUT_INT_MASK_SHIFT)) & ACOMP_IMR0_OUT_INT_MASK_MASK)

#define ACOMP_IMR0_OUTA_INT_MASK_MASK            (0x2U)
#define ACOMP_IMR0_OUTA_INT_MASK_SHIFT           (1U)
/*! OUTA_INT_MASK - Mask Asynchronized Interrupt */
#define ACOMP_IMR0_OUTA_INT_MASK(x)              (((uint32_t)(((uint32_t)(x)) << ACOMP_IMR0_OUTA_INT_MASK_SHIFT)) & ACOMP_IMR0_OUTA_INT_MASK_MASK)
/*! @} */

/*! @name IMR1 - ACOMP1 Interrupt Mask Register */
/*! @{ */

#define ACOMP_IMR1_OUT_INT_MASK_MASK             (0x1U)
#define ACOMP_IMR1_OUT_INT_MASK_SHIFT            (0U)
/*! OUT_INT_MASK - Mask Synchronized Interrupt */
#define ACOMP_IMR1_OUT_INT_MASK(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_IMR1_OUT_INT_MASK_SHIFT)) & ACOMP_IMR1_OUT_INT_MASK_MASK)

#define ACOMP_IMR1_OUTA_INT_MASK_MASK            (0x2U)
#define ACOMP_IMR1_OUTA_INT_MASK_SHIFT           (1U)
/*! OUTA_INT_MASK - Mask Asynchronized Interrupt */
#define ACOMP_IMR1_OUTA_INT_MASK(x)              (((uint32_t)(((uint32_t)(x)) << ACOMP_IMR1_OUTA_INT_MASK_SHIFT)) & ACOMP_IMR1_OUTA_INT_MASK_MASK)
/*! @} */

/*! @name IRSR0 - ACOMP0 Interrupt Raw Status Register */
/*! @{ */

#define ACOMP_IRSR0_OUT_INT_RAW_MASK             (0x1U)
#define ACOMP_IRSR0_OUT_INT_RAW_SHIFT            (0U)
/*! OUT_INT_RAW - Raw Mask Synchronized Interrupt */
#define ACOMP_IRSR0_OUT_INT_RAW(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_IRSR0_OUT_INT_RAW_SHIFT)) & ACOMP_IRSR0_OUT_INT_RAW_MASK)

#define ACOMP_IRSR0_OUTA_INT_RAW_MASK            (0x2U)
#define ACOMP_IRSR0_OUTA_INT_RAW_SHIFT           (1U)
/*! OUTA_INT_RAW - Raw Mask Asynchronized Interrupt */
#define ACOMP_IRSR0_OUTA_INT_RAW(x)              (((uint32_t)(((uint32_t)(x)) << ACOMP_IRSR0_OUTA_INT_RAW_SHIFT)) & ACOMP_IRSR0_OUTA_INT_RAW_MASK)
/*! @} */

/*! @name IRSR1 - ACOMP1 Interrupt Raw Status Register */
/*! @{ */

#define ACOMP_IRSR1_OUT_INT_RAW_MASK             (0x1U)
#define ACOMP_IRSR1_OUT_INT_RAW_SHIFT            (0U)
/*! OUT_INT_RAW - Raw Mask Synchronized Interrupt */
#define ACOMP_IRSR1_OUT_INT_RAW(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_IRSR1_OUT_INT_RAW_SHIFT)) & ACOMP_IRSR1_OUT_INT_RAW_MASK)

#define ACOMP_IRSR1_OUTA_INT_RAW_MASK            (0x2U)
#define ACOMP_IRSR1_OUTA_INT_RAW_SHIFT           (1U)
/*! OUTA_INT_RAW - Raw Mask Asynchronized Interrupt */
#define ACOMP_IRSR1_OUTA_INT_RAW(x)              (((uint32_t)(((uint32_t)(x)) << ACOMP_IRSR1_OUTA_INT_RAW_SHIFT)) & ACOMP_IRSR1_OUTA_INT_RAW_MASK)
/*! @} */

/*! @name ICR0 - ACOMP0 Interrupt Clear Register */
/*! @{ */

#define ACOMP_ICR0_OUT_INT_CLR_MASK              (0x1U)
#define ACOMP_ICR0_OUT_INT_CLR_SHIFT             (0U)
/*! OUT_INT_CLR - ACOMP0 syncrhonized output interrupt flag clear signal. */
#define ACOMP_ICR0_OUT_INT_CLR(x)                (((uint32_t)(((uint32_t)(x)) << ACOMP_ICR0_OUT_INT_CLR_SHIFT)) & ACOMP_ICR0_OUT_INT_CLR_MASK)

#define ACOMP_ICR0_OUTA_INT_CLR_MASK             (0x2U)
#define ACOMP_ICR0_OUTA_INT_CLR_SHIFT            (1U)
/*! OUTA_INT_CLR - ACOMP0 asyncrhonized output interrupt flag clear signal. */
#define ACOMP_ICR0_OUTA_INT_CLR(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_ICR0_OUTA_INT_CLR_SHIFT)) & ACOMP_ICR0_OUTA_INT_CLR_MASK)
/*! @} */

/*! @name ICR1 - ACOMP1 Interrupt Clear Register */
/*! @{ */

#define ACOMP_ICR1_OUT_INT_CLR_MASK              (0x1U)
#define ACOMP_ICR1_OUT_INT_CLR_SHIFT             (0U)
/*! OUT_INT_CLR - ACOMP1 syncrhonized output interrupt flag clear signal. */
#define ACOMP_ICR1_OUT_INT_CLR(x)                (((uint32_t)(((uint32_t)(x)) << ACOMP_ICR1_OUT_INT_CLR_SHIFT)) & ACOMP_ICR1_OUT_INT_CLR_MASK)

#define ACOMP_ICR1_OUTA_INT_CLR_MASK             (0x2U)
#define ACOMP_ICR1_OUTA_INT_CLR_SHIFT            (1U)
/*! OUTA_INT_CLR - ACOMP1 asyncrhonized output interrupt flag clear signal. */
#define ACOMP_ICR1_OUTA_INT_CLR(x)               (((uint32_t)(((uint32_t)(x)) << ACOMP_ICR1_OUTA_INT_CLR_SHIFT)) & ACOMP_ICR1_OUTA_INT_CLR_MASK)
/*! @} */

/*! @name RST0 - ACOMP0 Soft Reset Register */
/*! @{ */

#define ACOMP_RST0_SOFT_RST_MASK                 (0x1U)
#define ACOMP_RST0_SOFT_RST_SHIFT                (0U)
/*! SOFT_RST - Soft Reset for ACOMP0 (active high)
 *  0b0..no action
 *  0b1..
 */
#define ACOMP_RST0_SOFT_RST(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_RST0_SOFT_RST_SHIFT)) & ACOMP_RST0_SOFT_RST_MASK)
/*! @} */

/*! @name RST1 - ACOMP1 Soft Reset Register */
/*! @{ */

#define ACOMP_RST1_SOFT_RST_MASK                 (0x1U)
#define ACOMP_RST1_SOFT_RST_SHIFT                (0U)
/*! SOFT_RST - Soft Reset for ACOMP1 (active high)
 *  0b0..no action
 *  0b1..
 */
#define ACOMP_RST1_SOFT_RST(x)                   (((uint32_t)(((uint32_t)(x)) << ACOMP_RST1_SOFT_RST_SHIFT)) & ACOMP_RST1_SOFT_RST_MASK)
/*! @} */

/*! @name CLK - Clock Register */
/*! @{ */

#define ACOMP_CLK_SOFT_CLK_RST_MASK              (0x2U)
#define ACOMP_CLK_SOFT_CLK_RST_SHIFT             (1U)
/*! SOFT_CLK_RST - soft reset for clock divider
 *  0b0..no action
 *  0b1..
 */
#define ACOMP_CLK_SOFT_CLK_RST(x)                (((uint32_t)(((uint32_t)(x)) << ACOMP_CLK_SOFT_CLK_RST_SHIFT)) & ACOMP_CLK_SOFT_CLK_RST_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group ACOMP_Register_Masks */


/*!
 * @}
 */ /* end of group ACOMP_Peripheral_Access_Layer */


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


#endif  /* ACOMP_H_ */

