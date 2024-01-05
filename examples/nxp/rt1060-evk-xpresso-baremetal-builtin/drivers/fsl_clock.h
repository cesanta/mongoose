/*
 * Copyright 2018 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CLOCK_H_
#define _FSL_CLOCK_H_

#include "fsl_common.h"

/*! @addtogroup clock */
/*! @{ */

/*! @file */

/*******************************************************************************
 * Configurations
 ******************************************************************************/

/*! @brief Configure whether driver controls clock
 *
 * When set to 0, peripheral drivers will enable clock in initialize function
 * and disable clock in de-initialize function. When set to 1, peripheral
 * driver will not control the clock, application could control the clock out of
 * the driver.
 *
 * @note All drivers share this feature switcher. If it is set to 1, application
 * should handle clock enable and disable for all drivers.
 */
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL))
#define FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL 0
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief CLOCK driver version 2.5.1. */
#define FSL_CLOCK_DRIVER_VERSION (MAKE_VERSION(2, 5, 1))

/* Definition for delay API in clock driver, users can redefine it to the real application. */
#ifndef SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY
#define SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY (600000000UL)
#endif

/* analog pll definition */
#define CCM_ANALOG_PLL_BYPASS_SHIFT         (16U)
#define CCM_ANALOG_PLL_BYPASS_CLK_SRC_MASK  (0xC000U)
#define CCM_ANALOG_PLL_BYPASS_CLK_SRC_SHIFT (14U)

/*@}*/

/*!
 * @brief CCM registers offset.
 */
#define CCSR_OFFSET   0x0C
#define CBCDR_OFFSET  0x14
#define CBCMR_OFFSET  0x18
#define CSCMR1_OFFSET 0x1C
#define CSCMR2_OFFSET 0x20
#define CSCDR1_OFFSET 0x24
#define CDCDR_OFFSET  0x30
#define CSCDR2_OFFSET 0x38
#define CSCDR3_OFFSET 0x3C
#define CACRR_OFFSET  0x10
#define CS1CDR_OFFSET 0x28
#define CS2CDR_OFFSET 0x2C

/*!
 * @brief CCM Analog registers offset.
 */
#define PLL_ARM_OFFSET   0x00
#define PLL_SYS_OFFSET   0x30
#define PLL_USB1_OFFSET  0x10
#define PLL_AUDIO_OFFSET 0x70
#define PLL_VIDEO_OFFSET 0xA0
#define PLL_ENET_OFFSET  0xE0
#define PLL_USB2_OFFSET  0x20

#define CCM_TUPLE(reg, shift, mask, busyShift) \
    (int)(((reg)&0xFFU) | ((shift) << 8U) | ((((mask) >> (shift)) & 0x1FFFU) << 13U) | ((busyShift) << 26U))
#define CCM_TUPLE_REG(base, tuple) (*((volatile uint32_t *)(((uint32_t)(base)) + ((uint32_t)(tuple)&0xFFU))))
#define CCM_TUPLE_SHIFT(tuple)     ((((uint32_t)tuple) >> 8U) & 0x1FU)
#define CCM_TUPLE_MASK(tuple) \
    ((uint32_t)((((uint32_t)(tuple) >> 13U) & 0x1FFFU) << (((((uint32_t)tuple) >> 8U) & 0x1FU))))
#define CCM_TUPLE_BUSY_SHIFT(tuple) ((((uint32_t)tuple) >> 26U) & 0x3FU)

#define CCM_NO_BUSY_WAIT (0x20U)

/*!
 * @brief CCM ANALOG tuple macros to map corresponding registers and bit fields.
 */
#define CCM_ANALOG_TUPLE(reg, shift)  ((((reg)&0xFFFU) << 16U) | (shift))
#define CCM_ANALOG_TUPLE_SHIFT(tuple) (((uint32_t)tuple) & 0x1FU)
#define CCM_ANALOG_TUPLE_REG_OFF(base, tuple, off) \
    (*((volatile uint32_t *)((uint32_t)(base) + (((uint32_t)(tuple) >> 16U) & 0xFFFU) + (off))))
#define CCM_ANALOG_TUPLE_REG(base, tuple) CCM_ANALOG_TUPLE_REG_OFF(base, tuple, 0U)

/* Definition for ERRATA 50235 check */
#if (defined(FSL_FEATURE_CCM_HAS_ERRATA_50235) && FSL_FEATURE_CCM_HAS_ERRATA_50235)
#define CAN_CLOCK_CHECK_NO_AFFECTS                                                  \
    ((CCM_CSCMR2_CAN_CLK_SEL(2U) != (CCM->CSCMR2 & CCM_CSCMR2_CAN_CLK_SEL_MASK)) || \
     (CCM_CCGR5_CG12(0) != (CCM->CCGR5 & CCM_CCGR5_CG12_MASK)))
#endif /* FSL_FEATURE_CCM_HAS_ERRATA_50235 */

/*!
 * @brief clock1PN frequency.
 */
#define CLKPN_FREQ 0U

/*! @brief External XTAL (24M OSC/SYSOSC) clock frequency.
 *
 * The XTAL (24M OSC/SYSOSC) clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetXtalFreq to set the value in to clock driver. For example,
 * if XTAL is 24MHz,
 * @code
 * CLOCK_InitExternalClk(false);
 * CLOCK_SetXtalFreq(240000000);
 * @endcode
 */
extern volatile uint32_t g_xtalFreq;

/*! @brief External RTC XTAL (32K OSC) clock frequency.
 *
 * The RTC XTAL (32K OSC) clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetRtcXtalFreq to set the value in to clock driver.
 */
extern volatile uint32_t g_rtcXtalFreq;

/* For compatible with other platforms */
#define CLOCK_SetXtal0Freq  CLOCK_SetXtalFreq
#define CLOCK_SetXtal32Freq CLOCK_SetRtcXtalFreq

/*! @brief Clock ip name array for ADC. */
#define ADC_CLOCKS                                 \
    {                                              \
        kCLOCK_IpInvalid, kCLOCK_Adc1, kCLOCK_Adc2 \
    }

/*! @brief Clock ip name array for AOI. */
#define AOI_CLOCKS                                 \
    {                                              \
        kCLOCK_IpInvalid, kCLOCK_Aoi1, kCLOCK_Aoi2 \
    }

/*! @brief Clock ip name array for BEE. */
#define BEE_CLOCKS \
    {              \
        kCLOCK_Bee \
    }

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS                                                               \
    {                                                                            \
        kCLOCK_IpInvalid, kCLOCK_Acmp1, kCLOCK_Acmp2, kCLOCK_Acmp3, kCLOCK_Acmp4 \
    }

/*! @brief Clock ip name array for CSI. */
#define CSI_CLOCKS \
    {              \
        kCLOCK_Csi \
    }

/*! @brief Clock ip name array for DCDC. */
#define DCDC_CLOCKS \
    {               \
        kCLOCK_Dcdc \
    }

/*! @brief Clock ip name array for DCP. */
#define DCP_CLOCKS \
    {              \
        kCLOCK_Dcp \
    }

/*! @brief Clock ip name array for DMAMUX_CLOCKS. */
#define DMAMUX_CLOCKS \
    {                 \
        kCLOCK_Dma    \
    }

/*! @brief Clock ip name array for DMA. */
#define EDMA_CLOCKS \
    {               \
        kCLOCK_Dma  \
    }

/*! @brief Clock ip name array for ENC. */
#define ENC_CLOCKS                                                           \
    {                                                                        \
        kCLOCK_IpInvalid, kCLOCK_Enc1, kCLOCK_Enc2, kCLOCK_Enc3, kCLOCK_Enc4 \
    }

/*! @brief Clock ip name array for ENET. */
#define ENET_CLOCKS                                 \
    {                                               \
        kCLOCK_Enet, kCLOCK_IpInvalid, kCLOCK_Enet2 \
    }

/*! @brief Clock ip name array for EWM. */
#define EWM_CLOCKS  \
    {               \
        kCLOCK_Ewm0 \
    }

/*! @brief Clock ip name array for FLEXCAN. */
#define FLEXCAN_CLOCKS                                          \
    {                                                           \
        kCLOCK_IpInvalid, kCLOCK_Can1, kCLOCK_Can2, kCLOCK_Can3 \
    }

/*! @brief Clock ip name array for FLEXCAN Peripheral clock. */
#define FLEXCAN_PERIPH_CLOCKS                                      \
    {                                                              \
        kCLOCK_IpInvalid, kCLOCK_Can1S, kCLOCK_Can2S, kCLOCK_Can3S \
    }

/*! @brief Clock ip name array for FLEXIO. */
#define FLEXIO_CLOCKS                                                    \
    {                                                                    \
        kCLOCK_IpInvalid, kCLOCK_Flexio1, kCLOCK_Flexio2, kCLOCK_Flexio3 \
    }

/*! @brief Clock ip name array for FLEXRAM. */
#define FLEXRAM_CLOCKS \
    {                  \
        kCLOCK_FlexRam \
    }

/*! @brief Clock ip name array for FLEXSPI. */
#define FLEXSPI_CLOCKS                                    \
    {                                                     \
        kCLOCK_FlexSpi, kCLOCK_IpInvalid, kCLOCK_FlexSpi2 \
    }

/*! @brief Clock ip name array for FLEXSPI EXSC. */
#define FLEXSPI_EXSC_CLOCKS \
    {                       \
        kCLOCK_FlexSpiExsc  \
    }

/*! @brief Clock ip name array for GPIO. */
#define GPIO_CLOCKS                                                                            \
    {                                                                                          \
        kCLOCK_IpInvalid, kCLOCK_Gpio1, kCLOCK_Gpio2, kCLOCK_Gpio3, kCLOCK_Gpio4, kCLOCK_Gpio5 \
    }

/*! @brief Clock ip name array for GPT. */
#define GPT_CLOCKS                                 \
    {                                              \
        kCLOCK_IpInvalid, kCLOCK_Gpt1, kCLOCK_Gpt2 \
    }

/*! @brief Clock ip name array for KPP. */
#define KPP_CLOCKS \
    {              \
        kCLOCK_Kpp \
    }

/*! @brief Clock ip name array for LCDIF. */
#define LCDIF_CLOCKS \
    {                \
        kCLOCK_Lcd   \
    }

/*! @brief Clock ip name array for LCDIF PIXEL. */
#define LCDIF_PERIPH_CLOCKS \
    {                       \
        kCLOCK_LcdPixel     \
    }

/*! @brief Clock ip name array for LPI2C. */
#define LPI2C_CLOCKS                                                                 \
    {                                                                                \
        kCLOCK_IpInvalid, kCLOCK_Lpi2c1, kCLOCK_Lpi2c2, kCLOCK_Lpi2c3, kCLOCK_Lpi2c4 \
    }

/*! @brief Clock ip name array for LPSPI. */
#define LPSPI_CLOCKS                                                                 \
    {                                                                                \
        kCLOCK_IpInvalid, kCLOCK_Lpspi1, kCLOCK_Lpspi2, kCLOCK_Lpspi3, kCLOCK_Lpspi4 \
    }

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS                                                                                     \
    {                                                                                                     \
        kCLOCK_IpInvalid, kCLOCK_Lpuart1, kCLOCK_Lpuart2, kCLOCK_Lpuart3, kCLOCK_Lpuart4, kCLOCK_Lpuart5, \
            kCLOCK_Lpuart6, kCLOCK_Lpuart7, kCLOCK_Lpuart8                                                \
    }

/*! @brief Clock ip name array for MQS. */
#define MQS_CLOCKS \
    {              \
        kCLOCK_Mqs \
    }

/*! @brief Clock ip name array for OCRAM EXSC. */
#define OCRAM_EXSC_CLOCKS \
    {                     \
        kCLOCK_OcramExsc  \
    }

/*! @brief Clock ip name array for PIT. */
#define PIT_CLOCKS \
    {              \
        kCLOCK_Pit \
    }

/*! @brief Clock ip name array for PWM. */
#define PWM_CLOCKS                                                                \
    {                                                                             \
        {kCLOCK_IpInvalid, kCLOCK_IpInvalid, kCLOCK_IpInvalid, kCLOCK_IpInvalid}, \
            {kCLOCK_Pwm1, kCLOCK_Pwm1, kCLOCK_Pwm1, kCLOCK_Pwm1},                 \
            {kCLOCK_Pwm2, kCLOCK_Pwm2, kCLOCK_Pwm2, kCLOCK_Pwm2},                 \
            {kCLOCK_Pwm3, kCLOCK_Pwm3, kCLOCK_Pwm3, kCLOCK_Pwm3},                 \
        {                                                                         \
            kCLOCK_Pwm4, kCLOCK_Pwm4, kCLOCK_Pwm4, kCLOCK_Pwm4                    \
        }                                                                         \
    }

/*! @brief Clock ip name array for PXP. */
#define PXP_CLOCKS \
    {              \
        kCLOCK_Pxp \
    }

/*! @brief Clock ip name array for RTWDOG. */
#define RTWDOG_CLOCKS \
    {                 \
        kCLOCK_Wdog3  \
    }

/*! @brief Clock ip name array for SAI. */
#define SAI_CLOCKS                                              \
    {                                                           \
        kCLOCK_IpInvalid, kCLOCK_Sai1, kCLOCK_Sai2, kCLOCK_Sai3 \
    }

/*! @brief Clock ip name array for SEMC. */
#define SEMC_CLOCKS \
    {               \
        kCLOCK_Semc \
    }

/*! @brief Clock ip name array for SEMC EXSC. */
#define SEMC_EXSC_CLOCKS \
    {                    \
        kCLOCK_SemcExsc  \
    }

/*! @brief Clock ip name array for QTIMER. */
#define TMR_CLOCKS                                                                   \
    {                                                                                \
        kCLOCK_IpInvalid, kCLOCK_Timer1, kCLOCK_Timer2, kCLOCK_Timer3, kCLOCK_Timer4 \
    }

/*! @brief Clock ip name array for TRNG. */
#define TRNG_CLOCKS \
    {               \
        kCLOCK_Trng \
    }

/*! @brief Clock ip name array for TSC. */
#define TSC_CLOCKS \
    {              \
        kCLOCK_Tsc \
    }

/*! @brief Clock ip name array for WDOG. */
#define WDOG_CLOCKS                                  \
    {                                                \
        kCLOCK_IpInvalid, kCLOCK_Wdog1, kCLOCK_Wdog2 \
    }

/*! @brief Clock ip name array for USDHC. */
#define USDHC_CLOCKS                                   \
    {                                                  \
        kCLOCK_IpInvalid, kCLOCK_Usdhc1, kCLOCK_Usdhc2 \
    }

/*! @brief Clock ip name array for SPDIF. */
#define SPDIF_CLOCKS \
    {                \
        kCLOCK_Spdif \
    }

/*! @brief Clock ip name array for XBARA. */
#define XBARA_CLOCKS                   \
    {                                  \
        kCLOCK_IpInvalid, kCLOCK_Xbar1 \
    }

/*! @brief Clock ip name array for XBARB. */
#define XBARB_CLOCKS                                                   \
    {                                                                  \
        kCLOCK_IpInvalid, kCLOCK_IpInvalid, kCLOCK_Xbar2, kCLOCK_Xbar3 \
    }

#define CLOCK_SOURCE_NONE (0xFFU)

#define CLOCK_ROOT_SOUCE                                                                                 \
    {                                                                                                    \
        {kCLOCK_SysPllPfd2Clk, kCLOCK_SysPllPfd0Clk, kCLOCK_NoneName,                                    \
         kCLOCK_NoneName,      kCLOCK_NoneName,      kCLOCK_NoneName}, /* USDHC1 Clock Root. */          \
            {kCLOCK_SysPllPfd2Clk, kCLOCK_SysPllPfd0Clk, kCLOCK_NoneName,                                \
             kCLOCK_NoneName,      kCLOCK_NoneName,      kCLOCK_NoneName}, /* USDHC2 Clock Root. */      \
            {kCLOCK_SemcClk,        kCLOCK_Usb1SwClk, kCLOCK_SysPllPfd2Clk,                              \
             kCLOCK_Usb1PllPfd0Clk, kCLOCK_NoneName,  kCLOCK_NoneName}, /* FLEXSPI Clock Root. */        \
            {kCLOCK_SysPllPfd2Clk, kCLOCK_Usb1PllPfd0Clk, kCLOCK_Usb1PllPfd1Clk,                         \
             kCLOCK_SysPllClk,     kCLOCK_NoneName,       kCLOCK_NoneName}, /* FLEXSPI2 Clock Root. */   \
            {kCLOCK_OscClk,         kCLOCK_SysPllPfd2Clk, kCLOCK_Usb1Sw120MClk,                          \
             kCLOCK_Usb1PllPfd1Clk, kCLOCK_NoneName,      kCLOCK_NoneName}, /* CSI Clock Root. */        \
            {kCLOCK_Usb1PllPfd1Clk, kCLOCK_Usb1PllPfd0Clk, kCLOCK_SysPllClk,                             \
             kCLOCK_SysPllPfd2Clk,  kCLOCK_NoneName,       kCLOCK_NoneName}, /* LPSPI Clock Root. */     \
            {kCLOCK_SysPllClk,     kCLOCK_SysPllPfd2Clk, kCLOCK_SysPllPfd0Clk,                           \
             kCLOCK_SysPllPfd1Clk, kCLOCK_NoneName,      kCLOCK_NoneName}, /* TRACE Clock Root */        \
            {kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk, kCLOCK_AudioPllClk,                              \
             kCLOCK_NoneName,       kCLOCK_NoneName,    kCLOCK_NoneName}, /* SAI1 Clock Root */          \
            {kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk, kCLOCK_AudioPllClk,                              \
             kCLOCK_NoneName,       kCLOCK_NoneName,    kCLOCK_NoneName}, /* SAI2 Clock Root */          \
            {kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk, kCLOCK_AudioPllClk,                              \
             kCLOCK_NoneName,       kCLOCK_NoneName,    kCLOCK_NoneName}, /* SAI3 Clock Root */          \
            {kCLOCK_Usb1Sw60MClk, kCLOCK_OscClk,   kCLOCK_NoneName,                                      \
             kCLOCK_NoneName,     kCLOCK_NoneName, kCLOCK_NoneName}, /* LPI2C Clock Root */              \
            {kCLOCK_Usb1Sw60MClk, kCLOCK_OscClk,   kCLOCK_Usb1Sw80MClk,                                  \
             kCLOCK_NoneName,     kCLOCK_NoneName, kCLOCK_NoneName}, /* CAN Clock Root. */               \
            {kCLOCK_Usb1Sw80MClk, kCLOCK_OscClk,   kCLOCK_NoneName,                                      \
             kCLOCK_NoneName,     kCLOCK_NoneName, kCLOCK_NoneName}, /* UART Clock Root */               \
            {kCLOCK_SysPllClk,     kCLOCK_Usb1PllPfd3Clk, kCLOCK_VideoPllClk,                            \
             kCLOCK_SysPllPfd0Clk, kCLOCK_SysPllPfd1Clk,  kCLOCK_Usb1PllPfd1Clk}, /* LCDIF Clock Root */ \
            {kCLOCK_AudioPllClk, kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk,                              \
             kCLOCK_Usb1SwClk,   kCLOCK_NoneName,       kCLOCK_NoneName}, /* SPDIF0 Clock Root */        \
            {kCLOCK_AudioPllClk, kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk,                              \
             kCLOCK_Usb1SwClk,   kCLOCK_NoneName,       kCLOCK_NoneName}, /* FLEXIO1 Clock Root */       \
            {kCLOCK_AudioPllClk, kCLOCK_Usb1PllPfd2Clk, kCLOCK_VideoPllClk,                              \
             kCLOCK_Usb1PllClk,  kCLOCK_NoneName,       kCLOCK_NoneName}, /* FLEXIO2 Clock ROOT */       \
    }

#define CLOCK_ROOT_MUX_TUPLE                                                                                       \
    {                                                                                                              \
        kCLOCK_Usdhc1Mux, kCLOCK_Usdhc2Mux, kCLOCK_FlexspiMux, kCLOCK_Flexspi2Mux, kCLOCK_CsiMux, kCLOCK_LpspiMux, \
            kCLOCK_TraceMux, kCLOCK_Sai1Mux, kCLOCK_Sai2Mux, kCLOCK_Sai3Mux, kCLOCK_Lpi2cMux, kCLOCK_CanMux,       \
            kCLOCK_UartMux, kCLOCK_LcdifPreMux, kCLOCK_SpdifMux, kCLOCK_Flexio1Mux, kCLOCK_Flexio2Mux,             \
    }

#define CLOCK_ROOT_NONE_PRE_DIV 0UL

#define CLOCK_ROOT_DIV_TUPLE                                                                    \
    {                                                                                           \
        {kCLOCK_NonePreDiv, kCLOCK_Usdhc1Div}, {kCLOCK_NonePreDiv, kCLOCK_Usdhc2Div},           \
            {kCLOCK_NonePreDiv, kCLOCK_FlexspiDiv}, {kCLOCK_NonePreDiv, kCLOCK_Flexspi2Div},    \
            {kCLOCK_NonePreDiv, kCLOCK_CsiDiv}, {kCLOCK_NonePreDiv, kCLOCK_LpspiDiv},           \
            {kCLOCK_NonePreDiv, kCLOCK_TraceDiv}, {kCLOCK_Sai1PreDiv, kCLOCK_Sai1Div},          \
            {kCLOCK_Sai2PreDiv, kCLOCK_Sai2Div}, {kCLOCK_Sai3PreDiv, kCLOCK_Sai3Div},           \
            {kCLOCK_NonePreDiv, kCLOCK_Lpi2cDiv}, {kCLOCK_NonePreDiv, kCLOCK_CanDiv},           \
            {kCLOCK_NonePreDiv, kCLOCK_UartDiv}, {kCLOCK_LcdifPreDiv, kCLOCK_LcdifDiv},         \
            {kCLOCK_Spdif0PreDiv, kCLOCK_Spdif0Div}, {kCLOCK_Flexio1PreDiv, kCLOCK_Flexio1Div}, \
            {kCLOCK_Flexio2PreDiv, kCLOCK_Flexio2Div},                                          \
    }

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    kCLOCK_CpuClk  = 0x0U, /*!< CPU clock */
    kCLOCK_AhbClk  = 0x1U, /*!< AHB clock */
    kCLOCK_SemcClk = 0x2U, /*!< SEMC clock */
    kCLOCK_IpgClk  = 0x3U, /*!< IPG clock */
    kCLOCK_PerClk  = 0x4U, /*!< PER clock */

    kCLOCK_OscClk = 0x5U, /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
    kCLOCK_RtcClk = 0x6U, /*!< RTC clock. (RTCCLK) */

    kCLOCK_ArmPllClk = 0x7U, /*!< ARMPLLCLK. */

    kCLOCK_Usb1PllClk     = 0x8U,  /*!< USB1PLLCLK. */
    kCLOCK_Usb1PllPfd0Clk = 0x9U,  /*!< USB1PLLPDF0CLK. */
    kCLOCK_Usb1PllPfd1Clk = 0xAU,  /*!< USB1PLLPFD1CLK. */
    kCLOCK_Usb1PllPfd2Clk = 0xBU,  /*!< USB1PLLPFD2CLK. */
    kCLOCK_Usb1PllPfd3Clk = 0xCU,  /*!< USB1PLLPFD3CLK. */
    kCLOCK_Usb1SwClk      = 0x18U, /*!< USB1PLLSWCLK */
    kCLOCK_Usb1Sw120MClk  = 0x19U, /*!< USB1PLLSw120MCLK */
    kCLOCK_Usb1Sw60MClk   = 0x1AU, /*!< USB1PLLSw60MCLK */
    kCLOCK_Usb1Sw80MClk   = 0x1BU, /*!< USB1PLLSw80MCLK */

    kCLOCK_Usb2PllClk = 0xDU, /*!< USB2PLLCLK. */

    kCLOCK_SysPllClk     = 0xEU,  /*!< SYSPLLCLK. */
    kCLOCK_SysPllPfd0Clk = 0xFU,  /*!< SYSPLLPDF0CLK. */
    kCLOCK_SysPllPfd1Clk = 0x10U, /*!< SYSPLLPFD1CLK. */
    kCLOCK_SysPllPfd2Clk = 0x11U, /*!< SYSPLLPFD2CLK. */
    kCLOCK_SysPllPfd3Clk = 0x12U, /*!< SYSPLLPFD3CLK. */

    kCLOCK_EnetPll0Clk = 0x13U, /*!< Enet PLLCLK ref_enetpll0. */
    kCLOCK_EnetPll1Clk = 0x14U, /*!< Enet PLLCLK ref_enetpll1. */
    kCLOCK_EnetPll2Clk = 0x15U, /*!< Enet PLLCLK ref_enetpll2. */

    kCLOCK_AudioPllClk = 0x16U, /*!< Audio PLLCLK. */
    kCLOCK_VideoPllClk = 0x17U, /*!< Video PLLCLK. */

    kCLOCK_NoneName = CLOCK_SOURCE_NONE, /*!< None Clock Name. */
} clock_name_t;

#define kCLOCK_CoreSysClk       kCLOCK_CpuClk       /*!< For compatible with other platforms without CCM. */
#define CLOCK_GetCoreSysClkFreq CLOCK_GetCpuClkFreq /*!< For compatible with other platforms without CCM. */

/*!
 * @brief CCM CCGR gate control for each module independently.
 */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = -1,

    /* CCM CCGR0 */
    kCLOCK_Aips_tz1    = (0U << 8U) | CCM_CCGR0_CG0_SHIFT,  /*!< CCGR0, CG0   */
    kCLOCK_Aips_tz2    = (0U << 8U) | CCM_CCGR0_CG1_SHIFT,  /*!< CCGR0, CG1   */
    kCLOCK_Mqs         = (0U << 8U) | CCM_CCGR0_CG2_SHIFT,  /*!< CCGR0, CG2   */
    kCLOCK_FlexSpiExsc = (0U << 8U) | CCM_CCGR0_CG3_SHIFT,  /*!< CCGR0, CG3   */
    kCLOCK_Sim_M_Main  = (0U << 8U) | CCM_CCGR0_CG4_SHIFT,  /*!< CCGR0, CG4   */
    kCLOCK_Dcp         = (0U << 8U) | CCM_CCGR0_CG5_SHIFT,  /*!< CCGR0, CG5   */
    kCLOCK_Lpuart3     = (0U << 8U) | CCM_CCGR0_CG6_SHIFT,  /*!< CCGR0, CG6   */
    kCLOCK_Can1        = (0U << 8U) | CCM_CCGR0_CG7_SHIFT,  /*!< CCGR0, CG7   */
    kCLOCK_Can1S       = (0U << 8U) | CCM_CCGR0_CG8_SHIFT,  /*!< CCGR0, CG8   */
    kCLOCK_Can2        = (0U << 8U) | CCM_CCGR0_CG9_SHIFT,  /*!< CCGR0, CG9   */
    kCLOCK_Can2S       = (0U << 8U) | CCM_CCGR0_CG10_SHIFT, /*!< CCGR0, CG10  */
    kCLOCK_Trace       = (0U << 8U) | CCM_CCGR0_CG11_SHIFT, /*!< CCGR0, CG11  */
    kCLOCK_Gpt2        = (0U << 8U) | CCM_CCGR0_CG12_SHIFT, /*!< CCGR0, CG12  */
    kCLOCK_Gpt2S       = (0U << 8U) | CCM_CCGR0_CG13_SHIFT, /*!< CCGR0, CG13  */
    kCLOCK_Lpuart2     = (0U << 8U) | CCM_CCGR0_CG14_SHIFT, /*!< CCGR0, CG14  */
    kCLOCK_Gpio2       = (0U << 8U) | CCM_CCGR0_CG15_SHIFT, /*!< CCGR0, CG15  */

    /* CCM CCGR1 */
    kCLOCK_Lpspi1   = (1U << 8U) | CCM_CCGR1_CG0_SHIFT,  /*!< CCGR1, CG0   */
    kCLOCK_Lpspi2   = (1U << 8U) | CCM_CCGR1_CG1_SHIFT,  /*!< CCGR1, CG1   */
    kCLOCK_Lpspi3   = (1U << 8U) | CCM_CCGR1_CG2_SHIFT,  /*!< CCGR1, CG2   */
    kCLOCK_Lpspi4   = (1U << 8U) | CCM_CCGR1_CG3_SHIFT,  /*!< CCGR1, CG3   */
    kCLOCK_Adc2     = (1U << 8U) | CCM_CCGR1_CG4_SHIFT,  /*!< CCGR1, CG4   */
    kCLOCK_Enet     = (1U << 8U) | CCM_CCGR1_CG5_SHIFT,  /*!< CCGR1, CG5   */
    kCLOCK_Pit      = (1U << 8U) | CCM_CCGR1_CG6_SHIFT,  /*!< CCGR1, CG6   */
    kCLOCK_Aoi2     = (1U << 8U) | CCM_CCGR1_CG7_SHIFT,  /*!< CCGR1, CG7   */
    kCLOCK_Adc1     = (1U << 8U) | CCM_CCGR1_CG8_SHIFT,  /*!< CCGR1, CG8   */
    kCLOCK_SemcExsc = (1U << 8U) | CCM_CCGR1_CG9_SHIFT,  /*!< CCGR1, CG9   */
    kCLOCK_Gpt1     = (1U << 8U) | CCM_CCGR1_CG10_SHIFT, /*!< CCGR1, CG10  */
    kCLOCK_Gpt1S    = (1U << 8U) | CCM_CCGR1_CG11_SHIFT, /*!< CCGR1, CG11  */
    kCLOCK_Lpuart4  = (1U << 8U) | CCM_CCGR1_CG12_SHIFT, /*!< CCGR1, CG12  */
    kCLOCK_Gpio1    = (1U << 8U) | CCM_CCGR1_CG13_SHIFT, /*!< CCGR1, CG13  */
    kCLOCK_Csu      = (1U << 8U) | CCM_CCGR1_CG14_SHIFT, /*!< CCGR1, CG14  */
    kCLOCK_Gpio5    = (1U << 8U) | CCM_CCGR1_CG15_SHIFT, /*!< CCGR1, CG15  */

    /* CCM CCGR2 */
    kCLOCK_OcramExsc  = (2U << 8U) | CCM_CCGR2_CG0_SHIFT,  /*!< CCGR2, CG0   */
    kCLOCK_Csi        = (2U << 8U) | CCM_CCGR2_CG1_SHIFT,  /*!< CCGR2, CG1   */
    kCLOCK_IomuxcSnvs = (2U << 8U) | CCM_CCGR2_CG2_SHIFT,  /*!< CCGR2, CG2   */
    kCLOCK_Lpi2c1     = (2U << 8U) | CCM_CCGR2_CG3_SHIFT,  /*!< CCGR2, CG3   */
    kCLOCK_Lpi2c2     = (2U << 8U) | CCM_CCGR2_CG4_SHIFT,  /*!< CCGR2, CG4   */
    kCLOCK_Lpi2c3     = (2U << 8U) | CCM_CCGR2_CG5_SHIFT,  /*!< CCGR2, CG5   */
    kCLOCK_Ocotp      = (2U << 8U) | CCM_CCGR2_CG6_SHIFT,  /*!< CCGR2, CG6   */
    kCLOCK_Xbar3      = (2U << 8U) | CCM_CCGR2_CG7_SHIFT,  /*!< CCGR2, CG7   */
    kCLOCK_Ipmux1     = (2U << 8U) | CCM_CCGR2_CG8_SHIFT,  /*!< CCGR2, CG8   */
    kCLOCK_Ipmux2     = (2U << 8U) | CCM_CCGR2_CG9_SHIFT,  /*!< CCGR2, CG9   */
    kCLOCK_Ipmux3     = (2U << 8U) | CCM_CCGR2_CG10_SHIFT, /*!< CCGR2, CG10  */
    kCLOCK_Xbar1      = (2U << 8U) | CCM_CCGR2_CG11_SHIFT, /*!< CCGR2, CG11  */
    kCLOCK_Xbar2      = (2U << 8U) | CCM_CCGR2_CG12_SHIFT, /*!< CCGR2, CG12  */
    kCLOCK_Gpio3      = (2U << 8U) | CCM_CCGR2_CG13_SHIFT, /*!< CCGR2, CG13  */
    kCLOCK_Lcd        = (2U << 8U) | CCM_CCGR2_CG14_SHIFT, /*!< CCGR2, CG14  */
    kCLOCK_Pxp        = (2U << 8U) | CCM_CCGR2_CG15_SHIFT, /*!< CCGR2, CG15  */

    /* CCM CCGR3 */
    kCLOCK_Flexio2       = (3U << 8U) | CCM_CCGR3_CG0_SHIFT,  /*!< CCGR3, CG0   */
    kCLOCK_Lpuart5       = (3U << 8U) | CCM_CCGR3_CG1_SHIFT,  /*!< CCGR3, CG1   */
    kCLOCK_Semc          = (3U << 8U) | CCM_CCGR3_CG2_SHIFT,  /*!< CCGR3, CG2   */
    kCLOCK_Lpuart6       = (3U << 8U) | CCM_CCGR3_CG3_SHIFT,  /*!< CCGR3, CG3   */
    kCLOCK_Aoi1          = (3U << 8U) | CCM_CCGR3_CG4_SHIFT,  /*!< CCGR3, CG4   */
    kCLOCK_LcdPixel      = (3U << 8U) | CCM_CCGR3_CG5_SHIFT,  /*!< CCGR3, CG5   */
    kCLOCK_Gpio4         = (3U << 8U) | CCM_CCGR3_CG6_SHIFT,  /*!< CCGR3, CG6   */
    kCLOCK_Ewm0          = (3U << 8U) | CCM_CCGR3_CG7_SHIFT,  /*!< CCGR3, CG7   */
    kCLOCK_Wdog1         = (3U << 8U) | CCM_CCGR3_CG8_SHIFT,  /*!< CCGR3, CG8   */
    kCLOCK_FlexRam       = (3U << 8U) | CCM_CCGR3_CG9_SHIFT,  /*!< CCGR3, CG9   */
    kCLOCK_Acmp1         = (3U << 8U) | CCM_CCGR3_CG10_SHIFT, /*!< CCGR3, CG10  */
    kCLOCK_Acmp2         = (3U << 8U) | CCM_CCGR3_CG11_SHIFT, /*!< CCGR3, CG11  */
    kCLOCK_Acmp3         = (3U << 8U) | CCM_CCGR3_CG12_SHIFT, /*!< CCGR3, CG12  */
    kCLOCK_Acmp4         = (3U << 8U) | CCM_CCGR3_CG13_SHIFT, /*!< CCGR3, CG13  */
    kCLOCK_Ocram         = (3U << 8U) | CCM_CCGR3_CG14_SHIFT, /*!< CCGR3, CG14  */
    kCLOCK_IomuxcSnvsGpr = (3U << 8U) | CCM_CCGR3_CG15_SHIFT, /*!< CCGR3, CG15  */

    /* CCM CCGR4 */
    kCLOCK_Sim_m7_clk_r = (4U << 8U) | CCM_CCGR4_CG0_SHIFT,  /*!< CCGR4, CG0   */
    kCLOCK_Iomuxc       = (4U << 8U) | CCM_CCGR4_CG1_SHIFT,  /*!< CCGR4, CG1   */
    kCLOCK_IomuxcGpr    = (4U << 8U) | CCM_CCGR4_CG2_SHIFT,  /*!< CCGR4, CG2   */
    kCLOCK_Bee          = (4U << 8U) | CCM_CCGR4_CG3_SHIFT,  /*!< CCGR4, CG3   */
    kCLOCK_SimM7        = (4U << 8U) | CCM_CCGR4_CG4_SHIFT,  /*!< CCGR4, CG4   */
    kCLOCK_Tsc          = (4U << 8U) | CCM_CCGR4_CG5_SHIFT,  /*!< CCGR4, CG5   */
    kCLOCK_SimM         = (4U << 8U) | CCM_CCGR4_CG6_SHIFT,  /*!< CCGR4, CG6   */
    kCLOCK_SimEms       = (4U << 8U) | CCM_CCGR4_CG7_SHIFT,  /*!< CCGR4, CG7   */
    kCLOCK_Pwm1         = (4U << 8U) | CCM_CCGR4_CG8_SHIFT,  /*!< CCGR4, CG8   */
    kCLOCK_Pwm2         = (4U << 8U) | CCM_CCGR4_CG9_SHIFT,  /*!< CCGR4, CG9   */
    kCLOCK_Pwm3         = (4U << 8U) | CCM_CCGR4_CG10_SHIFT, /*!< CCGR4, CG10  */
    kCLOCK_Pwm4         = (4U << 8U) | CCM_CCGR4_CG11_SHIFT, /*!< CCGR4, CG11  */
    kCLOCK_Enc1         = (4U << 8U) | CCM_CCGR4_CG12_SHIFT, /*!< CCGR4, CG12  */
    kCLOCK_Enc2         = (4U << 8U) | CCM_CCGR4_CG13_SHIFT, /*!< CCGR4, CG13  */
    kCLOCK_Enc3         = (4U << 8U) | CCM_CCGR4_CG14_SHIFT, /*!< CCGR4, CG14  */
    kCLOCK_Enc4         = (4U << 8U) | CCM_CCGR4_CG15_SHIFT, /*!< CCGR4, CG15  */

    /* CCM CCGR5 */
    kCLOCK_Rom      = (5U << 8U) | CCM_CCGR5_CG0_SHIFT,  /*!< CCGR5, CG0   */
    kCLOCK_Flexio1  = (5U << 8U) | CCM_CCGR5_CG1_SHIFT,  /*!< CCGR5, CG1   */
    kCLOCK_Wdog3    = (5U << 8U) | CCM_CCGR5_CG2_SHIFT,  /*!< CCGR5, CG2   */
    kCLOCK_Dma      = (5U << 8U) | CCM_CCGR5_CG3_SHIFT,  /*!< CCGR5, CG3   */
    kCLOCK_Kpp      = (5U << 8U) | CCM_CCGR5_CG4_SHIFT,  /*!< CCGR5, CG4   */
    kCLOCK_Wdog2    = (5U << 8U) | CCM_CCGR5_CG5_SHIFT,  /*!< CCGR5, CG5   */
    kCLOCK_Aips_tz4 = (5U << 8U) | CCM_CCGR5_CG6_SHIFT,  /*!< CCGR5, CG6   */
    kCLOCK_Spdif    = (5U << 8U) | CCM_CCGR5_CG7_SHIFT,  /*!< CCGR5, CG7   */
    kCLOCK_SimMain  = (5U << 8U) | CCM_CCGR5_CG8_SHIFT,  /*!< CCGR5, CG8   */
    kCLOCK_Sai1     = (5U << 8U) | CCM_CCGR5_CG9_SHIFT,  /*!< CCGR5, CG9   */
    kCLOCK_Sai2     = (5U << 8U) | CCM_CCGR5_CG10_SHIFT, /*!< CCGR5, CG10  */
    kCLOCK_Sai3     = (5U << 8U) | CCM_CCGR5_CG11_SHIFT, /*!< CCGR5, CG11  */
    kCLOCK_Lpuart1  = (5U << 8U) | CCM_CCGR5_CG12_SHIFT, /*!< CCGR5, CG12  */
    kCLOCK_Lpuart7  = (5U << 8U) | CCM_CCGR5_CG13_SHIFT, /*!< CCGR5, CG13  */
    kCLOCK_SnvsHp   = (5U << 8U) | CCM_CCGR5_CG14_SHIFT, /*!< CCGR5, CG14  */
    kCLOCK_SnvsLp   = (5U << 8U) | CCM_CCGR5_CG15_SHIFT, /*!< CCGR5, CG15  */

    /* CCM CCGR6 */
    kCLOCK_UsbOh3   = (6U << 8U) | CCM_CCGR6_CG0_SHIFT,  /*!< CCGR6, CG0   */
    kCLOCK_Usdhc1   = (6U << 8U) | CCM_CCGR6_CG1_SHIFT,  /*!< CCGR6, CG1   */
    kCLOCK_Usdhc2   = (6U << 8U) | CCM_CCGR6_CG2_SHIFT,  /*!< CCGR6, CG2   */
    kCLOCK_Dcdc     = (6U << 8U) | CCM_CCGR6_CG3_SHIFT,  /*!< CCGR6, CG3   */
    kCLOCK_Ipmux4   = (6U << 8U) | CCM_CCGR6_CG4_SHIFT,  /*!< CCGR6, CG4   */
    kCLOCK_FlexSpi  = (6U << 8U) | CCM_CCGR6_CG5_SHIFT,  /*!< CCGR6, CG5   */
    kCLOCK_Trng     = (6U << 8U) | CCM_CCGR6_CG6_SHIFT,  /*!< CCGR6, CG6   */
    kCLOCK_Lpuart8  = (6U << 8U) | CCM_CCGR6_CG7_SHIFT,  /*!< CCGR6, CG7   */
    kCLOCK_Timer4   = (6U << 8U) | CCM_CCGR6_CG8_SHIFT,  /*!< CCGR6, CG8   */
    kCLOCK_Aips_tz3 = (6U << 8U) | CCM_CCGR6_CG9_SHIFT,  /*!< CCGR6, CG9   */
    kCLOCK_SimPer   = (6U << 8U) | CCM_CCGR6_CG10_SHIFT, /*!< CCGR6, CG10  */
    kCLOCK_Anadig   = (6U << 8U) | CCM_CCGR6_CG11_SHIFT, /*!< CCGR6, CG11  */
    kCLOCK_Lpi2c4   = (6U << 8U) | CCM_CCGR6_CG12_SHIFT, /*!< CCGR6, CG12  */
    kCLOCK_Timer1   = (6U << 8U) | CCM_CCGR6_CG13_SHIFT, /*!< CCGR6, CG13  */
    kCLOCK_Timer2   = (6U << 8U) | CCM_CCGR6_CG14_SHIFT, /*!< CCGR6, CG14  */
    kCLOCK_Timer3   = (6U << 8U) | CCM_CCGR6_CG15_SHIFT, /*!< CCGR6, CG15  */

    /* CCM CCGR7 */
    kCLOCK_Enet2     = (7U << 8U) | CCM_CCGR7_CG0_SHIFT, /*!< CCGR7, CG0   */
    kCLOCK_FlexSpi2  = (7U << 8U) | CCM_CCGR7_CG1_SHIFT, /*!< CCGR7, CG1   */
    kCLOCK_Axbs_l    = (7U << 8U) | CCM_CCGR7_CG2_SHIFT, /*!< CCGR7, CG2   */
    kCLOCK_Can3      = (7U << 8U) | CCM_CCGR7_CG3_SHIFT, /*!< CCGR7, CG3   */
    kCLOCK_Can3S     = (7U << 8U) | CCM_CCGR7_CG4_SHIFT, /*!< CCGR7, CG4   */
    kCLOCK_Aips_lite = (7U << 8U) | CCM_CCGR7_CG5_SHIFT, /*!< CCGR7, CG5   */
    kCLOCK_Flexio3   = (7U << 8U) | CCM_CCGR7_CG6_SHIFT, /*!< CCGR7, CG6   */

} clock_ip_name_t;

/*! @brief OSC 24M sorce select */
typedef enum _clock_osc
{
    kCLOCK_RcOsc   = 0U, /*!< On chip OSC. */
    kCLOCK_XtalOsc = 1U, /*!< 24M Xtal OSC */
} clock_osc_t;

/*! @brief Clock gate value */
typedef enum _clock_gate_value
{
    kCLOCK_ClockNotNeeded     = 0U, /*!< Clock is off during all modes. */
    kCLOCK_ClockNeededRun     = 1U, /*!< Clock is on in run mode, but off in WAIT and STOP modes */
    kCLOCK_ClockNeededRunWait = 3U, /*!< Clock is on during all modes, except STOP mode */
} clock_gate_value_t;

/*! @brief System clock mode */
typedef enum _clock_mode_t
{
    kCLOCK_ModeRun  = 0U, /*!< Remain in run mode. */
    kCLOCK_ModeWait = 1U, /*!< Transfer to wait mode. */
    kCLOCK_ModeStop = 2U, /*!< Transfer to stop mode. */
} clock_mode_t;

/*!
 * @brief MUX control names for clock mux setting.
 *
 * These constants define the mux control names for clock mux setting.\n
 * - 0:7: REG offset to CCM_BASE in bytes.
 * - 8:15: Root clock setting bit field shift.
 * - 16:31: Root clock setting bit field width.
 */
typedef enum _clock_mux
{
    kCLOCK_Pll3SwMux = CCM_TUPLE(CCSR_OFFSET,
                                 CCM_CCSR_PLL3_SW_CLK_SEL_SHIFT,
                                 CCM_CCSR_PLL3_SW_CLK_SEL_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< pll3_sw_clk mux name */

    kCLOCK_PeriphMux  = CCM_TUPLE(CBCDR_OFFSET,
                                 CCM_CBCDR_PERIPH_CLK_SEL_SHIFT,
                                 CCM_CBCDR_PERIPH_CLK_SEL_MASK,
                                 CCM_CDHIPR_PERIPH_CLK_SEL_BUSY_SHIFT), /*!< periph mux name */
    kCLOCK_SemcAltMux = CCM_TUPLE(CBCDR_OFFSET,
                                  CCM_CBCDR_SEMC_ALT_CLK_SEL_SHIFT,
                                  CCM_CBCDR_SEMC_ALT_CLK_SEL_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< semc mux name */
    kCLOCK_SemcMux    = CCM_TUPLE(CBCDR_OFFSET,
                               CCM_CBCDR_SEMC_CLK_SEL_SHIFT,
                               CCM_CBCDR_SEMC_CLK_SEL_MASK,
                               CCM_NO_BUSY_WAIT), /*!< semc mux name */

    kCLOCK_PrePeriphMux  = CCM_TUPLE(CBCMR_OFFSET,
                                    CCM_CBCMR_PRE_PERIPH_CLK_SEL_SHIFT,
                                    CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK,
                                    CCM_NO_BUSY_WAIT), /*!< pre-periph mux name */
    kCLOCK_TraceMux      = CCM_TUPLE(CBCMR_OFFSET,
                                CCM_CBCMR_TRACE_CLK_SEL_SHIFT,
                                CCM_CBCMR_TRACE_CLK_SEL_MASK,
                                CCM_NO_BUSY_WAIT), /*!< trace mux name */
    kCLOCK_PeriphClk2Mux = CCM_TUPLE(CBCMR_OFFSET,
                                     CCM_CBCMR_PERIPH_CLK2_SEL_SHIFT,
                                     CCM_CBCMR_PERIPH_CLK2_SEL_MASK,
                                     CCM_NO_BUSY_WAIT), /*!< periph clock2 mux name */
    kCLOCK_Flexspi2Mux   = CCM_TUPLE(CBCMR_OFFSET,
                                   CCM_CBCMR_FLEXSPI2_CLK_SEL_SHIFT,
                                   CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK,
                                   CCM_NO_BUSY_WAIT), /*!< flexspi2 mux name */
    kCLOCK_LpspiMux      = CCM_TUPLE(CBCMR_OFFSET,
                                CCM_CBCMR_LPSPI_CLK_SEL_SHIFT,
                                CCM_CBCMR_LPSPI_CLK_SEL_MASK,
                                CCM_NO_BUSY_WAIT), /*!< lpspi mux name */

    kCLOCK_FlexspiMux = CCM_TUPLE(CSCMR1_OFFSET,
                                  CCM_CSCMR1_FLEXSPI_CLK_SEL_SHIFT,
                                  CCM_CSCMR1_FLEXSPI_CLK_SEL_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexspi mux name */
    kCLOCK_Usdhc2Mux  = CCM_TUPLE(CSCMR1_OFFSET,
                                 CCM_CSCMR1_USDHC2_CLK_SEL_SHIFT,
                                 CCM_CSCMR1_USDHC2_CLK_SEL_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< usdhc2 mux name */
    kCLOCK_Usdhc1Mux  = CCM_TUPLE(CSCMR1_OFFSET,
                                 CCM_CSCMR1_USDHC1_CLK_SEL_SHIFT,
                                 CCM_CSCMR1_USDHC1_CLK_SEL_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< usdhc1 mux name */
    kCLOCK_Sai3Mux    = CCM_TUPLE(CSCMR1_OFFSET,
                               CCM_CSCMR1_SAI3_CLK_SEL_SHIFT,
                               CCM_CSCMR1_SAI3_CLK_SEL_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai3 mux name */
    kCLOCK_Sai2Mux    = CCM_TUPLE(CSCMR1_OFFSET,
                               CCM_CSCMR1_SAI2_CLK_SEL_SHIFT,
                               CCM_CSCMR1_SAI2_CLK_SEL_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai2 mux name */
    kCLOCK_Sai1Mux    = CCM_TUPLE(CSCMR1_OFFSET,
                               CCM_CSCMR1_SAI1_CLK_SEL_SHIFT,
                               CCM_CSCMR1_SAI1_CLK_SEL_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai1 mux name */
    kCLOCK_PerclkMux  = CCM_TUPLE(CSCMR1_OFFSET,
                                 CCM_CSCMR1_PERCLK_CLK_SEL_SHIFT,
                                 CCM_CSCMR1_PERCLK_CLK_SEL_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< perclk mux name */

    kCLOCK_Flexio2Mux = CCM_TUPLE(CSCMR2_OFFSET,
                                  CCM_CSCMR2_FLEXIO2_CLK_SEL_SHIFT,
                                  CCM_CSCMR2_FLEXIO2_CLK_SEL_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexio2 mux name */
    kCLOCK_CanMux     = CCM_TUPLE(CSCMR2_OFFSET,
                              CCM_CSCMR2_CAN_CLK_SEL_SHIFT,
                              CCM_CSCMR2_CAN_CLK_SEL_MASK,
                              CCM_NO_BUSY_WAIT), /*!< can mux name */

    kCLOCK_UartMux = CCM_TUPLE(CSCDR1_OFFSET,
                               CCM_CSCDR1_UART_CLK_SEL_SHIFT,
                               CCM_CSCDR1_UART_CLK_SEL_MASK,
                               CCM_NO_BUSY_WAIT), /*!< uart mux name */

    kCLOCK_SpdifMux   = CCM_TUPLE(CDCDR_OFFSET,
                                CCM_CDCDR_SPDIF0_CLK_SEL_SHIFT,
                                CCM_CDCDR_SPDIF0_CLK_SEL_MASK,
                                CCM_NO_BUSY_WAIT), /*!< spdif mux name */
    kCLOCK_Flexio1Mux = CCM_TUPLE(CDCDR_OFFSET,
                                  CCM_CDCDR_FLEXIO1_CLK_SEL_SHIFT,
                                  CCM_CDCDR_FLEXIO1_CLK_SEL_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexio1 mux name */

    kCLOCK_Lpi2cMux    = CCM_TUPLE(CSCDR2_OFFSET,
                                CCM_CSCDR2_LPI2C_CLK_SEL_SHIFT,
                                CCM_CSCDR2_LPI2C_CLK_SEL_MASK,
                                CCM_NO_BUSY_WAIT), /*!< lpi2c mux name */
    kCLOCK_LcdifPreMux = CCM_TUPLE(CSCDR2_OFFSET,
                                   CCM_CSCDR2_LCDIF_PRE_CLK_SEL_SHIFT,
                                   CCM_CSCDR2_LCDIF_PRE_CLK_SEL_MASK,
                                   CCM_NO_BUSY_WAIT), /*!< lcdif pre mux name */

    kCLOCK_CsiMux = CCM_TUPLE(CSCDR3_OFFSET,
                              CCM_CSCDR3_CSI_CLK_SEL_SHIFT,
                              CCM_CSCDR3_CSI_CLK_SEL_MASK,
                              CCM_NO_BUSY_WAIT), /*!< csi mux name */
} clock_mux_t;

/*!
 * @brief Clock divider value.
 */
typedef enum _clock_div_value
{
    kCLOCK_ArmDivBy1 = 0, /*!< ARM clock divider set to divided by 1. */
    kCLOCK_ArmDivBy2 = 1, /*!< ARM clock divider set to divided by 2. */
    kCLOCK_ArmDivBy3 = 2, /*!< ARM clock divider set to divided by 3. */
    kCLOCK_ArmDivBy4 = 3, /*!< ARM clock divider set to divided by 4. */
    kCLOCK_ArmDivBy5 = 4, /*!< ARM clock divider set to divided by 5. */
    kCLOCK_ArmDivBy6 = 5, /*!< ARM clock divider set to divided by 6. */
    kCLOCK_ArmDivBy7 = 6, /*!< ARM clock divider set to divided by 7. */
    kCLOCK_ArmDivBy8 = 7, /*!< ARM clock divider set to divided by 8. */

    kCLOCK_PeriphClk2DivBy1 = 0, /*!< PeriphClk2 divider set to divided by 1. */
    kCLOCK_PeriphClk2DivBy2 = 1, /*!< PeriphClk2 divider set to divided by 2. */
    kCLOCK_PeriphClk2DivBy3 = 2, /*!< PeriphClk2 divider set to divided by 3. */
    kCLOCK_PeriphClk2DivBy4 = 3, /*!< PeriphClk2 divider set to divided by 4. */
    kCLOCK_PeriphClk2DivBy5 = 4, /*!< PeriphClk2 divider set to divided by 5. */
    kCLOCK_PeriphClk2DivBy6 = 5, /*!< PeriphClk2 divider set to divided by 6. */
    kCLOCK_PeriphClk2DivBy7 = 6, /*!< PeriphClk2 divider set to divided by 7. */
    kCLOCK_PeriphClk2DivBy8 = 7, /*!< PeriphClk2 divider set to divided by 8. */

    kCLOCK_SemcDivBy1 = 0, /*!< SEMC clock divider set to divided by 1. */
    kCLOCK_SemcDivBy2 = 1, /*!< SEMC clock divider set to divided by 2. */
    kCLOCK_SemcDivBy3 = 2, /*!< SEMC clock divider set to divided by 3. */
    kCLOCK_SemcDivBy4 = 3, /*!< SEMC clock divider set to divided by 4. */
    kCLOCK_SemcDivBy5 = 4, /*!< SEMC clock divider set to divided by 5. */
    kCLOCK_SemcDivBy6 = 5, /*!< SEMC clock divider set to divided by 6. */
    kCLOCK_SemcDivBy7 = 6, /*!< SEMC clock divider set to divided by 7. */
    kCLOCK_SemcDivBy8 = 7, /*!< SEMC clock divider set to divided by 8. */

    kCLOCK_AhbDivBy1 = 0, /*!< AHB clock divider set to divided by 1. */
    kCLOCK_AhbDivBy2 = 1, /*!< AHB clock divider set to divided by 2. */
    kCLOCK_AhbDivBy3 = 2, /*!< AHB clock divider set to divided by 3. */
    kCLOCK_AhbDivBy4 = 3, /*!< AHB clock divider set to divided by 4. */
    kCLOCK_AhbDivBy5 = 4, /*!< AHB clock divider set to divided by 5. */
    kCLOCK_AhbDivBy6 = 5, /*!< AHB clock divider set to divided by 6. */
    kCLOCK_AhbDivBy7 = 6, /*!< AHB clock divider set to divided by 7. */
    kCLOCK_AhbDivBy8 = 7, /*!< AHB clock divider set to divided by 8. */

    kCLOCK_IpgDivBy1 = 0, /*!< IPG clock divider set to divided by 1. */
    kCLOCK_IpgDivBy2 = 1, /*!< IPG clock divider set to divided by 2. */
    kCLOCK_IpgDivBy3 = 2, /*!< IPG clock divider set to divided by 3. */
    kCLOCK_IpgDivBy4 = 3, /*!< IPG clock divider set to divided by 4. */

    kCLOCK_Flexspi2DivBy1 = 0, /*!< Flexspi2 divider set to divided by 1. */
    kCLOCK_Flexspi2DivBy2 = 1, /*!< Flexspi2 divider set to divided by 2. */
    kCLOCK_Flexspi2DivBy3 = 2, /*!< Flexspi2 divider set to divided by 3. */
    kCLOCK_Flexspi2DivBy4 = 3, /*!< Flexspi2 divider set to divided by 4. */
    kCLOCK_Flexspi2DivBy5 = 4, /*!< Flexspi2 divider set to divided by 5. */
    kCLOCK_Flexspi2DivBy6 = 5, /*!< Flexspi2 divider set to divided by 6. */
    kCLOCK_Flexspi2DivBy7 = 6, /*!< Flexspi2 divider set to divided by 7. */
    kCLOCK_Flexspi2DivBy8 = 7, /*!< Flexspi2 divider set to divided by 8. */

    kCLOCK_LpspiDivBy1 = 0, /*!< Lpspi divider set to divided by 1. */
    kCLOCK_LpspiDivBy2 = 1, /*!< Lpspi divider set to divided by 2. */
    kCLOCK_LpspiDivBy3 = 2, /*!< Lpspi divider set to divided by 3. */
    kCLOCK_LpspiDivBy4 = 3, /*!< Lpspi divider set to divided by 4. */
    kCLOCK_LpspiDivBy5 = 4, /*!< Lpspi divider set to divided by 5. */
    kCLOCK_LpspiDivBy6 = 5, /*!< Lpspi divider set to divided by 6. */
    kCLOCK_LpspiDivBy7 = 6, /*!< Lpspi divider set to divided by 7. */
    kCLOCK_LpspiDivBy8 = 7, /*!< Lpspi divider set to divided by 8. */

    kCLOCK_LcdifDivBy1 = 0, /*!< Lcdif divider set to divided by 1. */
    kCLOCK_LcdifDivBy2 = 1, /*!< Lcdif divider set to divided by 2. */
    kCLOCK_LcdifDivBy3 = 2, /*!< Lcdif divider set to divided by 3. */
    kCLOCK_LcdifDivBy4 = 3, /*!< Lcdif divider set to divided by 4. */
    kCLOCK_LcdifDivBy5 = 4, /*!< Lcdif divider set to divided by 5. */
    kCLOCK_LcdifDivBy6 = 5, /*!< Lcdif divider set to divided by 6. */
    kCLOCK_LcdifDivBy7 = 6, /*!< Lcdif divider set to divided by 7. */
    kCLOCK_LcdifDivBy8 = 7, /*!< Lcdif divider set to divided by 8. */

    kCLOCK_FlexspiDivBy1 = 0, /*!< Flexspi divider set to divided by 1. */
    kCLOCK_FlexspiDivBy2 = 1, /*!< Flexspi divider set to divided by 2. */
    kCLOCK_FlexspiDivBy3 = 2, /*!< Flexspi divider set to divided by 3. */
    kCLOCK_FlexspiDivBy4 = 3, /*!< Flexspi divider set to divided by 4. */
    kCLOCK_FlexspiDivBy5 = 4, /*!< Flexspi divider set to divided by 5. */
    kCLOCK_FlexspiDivBy6 = 5, /*!< Flexspi divider set to divided by 6. */
    kCLOCK_FlexspiDivBy7 = 6, /*!< Flexspi divider set to divided by 7. */
    kCLOCK_FlexspiDivBy8 = 7, /*!< Flexspi divider set to divided by 8. */

    kCLOCK_TraceDivBy1 = 0, /*!< Trace divider set to divided by 1. */
    kCLOCK_TraceDivBy2 = 1, /*!< Trace divider set to divided by 2. */
    kCLOCK_TraceDivBy3 = 2, /*!< Trace divider set to divided by 3. */
    kCLOCK_TraceDivBy4 = 3, /*!< Trace divider set to divided by 4. */

    kCLOCK_Usdhc2DivBy1 = 0, /*!< Usdhc2 divider set to divided by 1. */
    kCLOCK_Usdhc2DivBy2 = 1, /*!< Usdhc2 divider set to divided by 2. */
    kCLOCK_Usdhc2DivBy3 = 2, /*!< Usdhc2 divider set to divided by 3. */
    kCLOCK_Usdhc2DivBy4 = 3, /*!< Usdhc2 divider set to divided by 4. */
    kCLOCK_Usdhc2DivBy5 = 4, /*!< Usdhc2 divider set to divided by 5. */
    kCLOCK_Usdhc2DivBy6 = 5, /*!< Usdhc2 divider set to divided by 6. */
    kCLOCK_Usdhc2DivBy7 = 6, /*!< Usdhc2 divider set to divided by 7. */
    kCLOCK_Usdhc2DivBy8 = 7, /*!< Usdhc2 divider set to divided by 8. */

    kCLOCK_Usdhc1DivBy1 = 0, /*!< Usdhc1 divider set to divided by 1. */
    kCLOCK_Usdhc1DivBy2 = 1, /*!< Usdhc1 divider set to divided by 2. */
    kCLOCK_Usdhc1DivBy3 = 2, /*!< Usdhc1 divider set to divided by 3. */
    kCLOCK_Usdhc1DivBy4 = 3, /*!< Usdhc1 divider set to divided by 4. */
    kCLOCK_Usdhc1DivBy5 = 4, /*!< Usdhc1 divider set to divided by 5. */
    kCLOCK_Usdhc1DivBy6 = 5, /*!< Usdhc1 divider set to divided by 6. */
    kCLOCK_Usdhc1DivBy7 = 6, /*!< Usdhc1 divider set to divided by 7. */
    kCLOCK_Usdhc1DivBy8 = 7, /*!< Usdhc1 divider set to divided by 8. */

    kCLOCK_Flexio2DivBy1 = 0, /*!< Flexio2 divider set to divided by 1. */
    kCLOCK_Flexio2DivBy2 = 1, /*!< Flexio2 divider set to divided by 2. */
    kCLOCK_Flexio2DivBy3 = 2, /*!< Flexio2 divider set to divided by 3. */
    kCLOCK_Flexio2DivBy4 = 3, /*!< Flexio2 divider set to divided by 4. */
    kCLOCK_Flexio2DivBy5 = 4, /*!< Flexio2 divider set to divided by 5. */
    kCLOCK_Flexio2DivBy6 = 5, /*!< Flexio2 divider set to divided by 6. */
    kCLOCK_Flexio2DivBy7 = 6, /*!< Flexio2 divider set to divided by 7. */
    kCLOCK_Flexio2DivBy8 = 7, /*!< Flexio2 divider set to divided by 8. */

    kCLOCK_Sai3PreDivBy1 = 0, /*!< Sai3Pre divider set to divided by 1. */
    kCLOCK_Sai3PreDivBy2 = 1, /*!< Sai3Pre divider set to divided by 2. */
    kCLOCK_Sai3PreDivBy3 = 2, /*!< Sai3Pre divider set to divided by 3. */
    kCLOCK_Sai3PreDivBy4 = 3, /*!< Sai3Pre divider set to divided by 4. */
    kCLOCK_Sai3PreDivBy5 = 4, /*!< Sai3Pre divider set to divided by 5. */
    kCLOCK_Sai3PreDivBy6 = 5, /*!< Sai3Pre divider set to divided by 6. */
    kCLOCK_Sai3PreDivBy7 = 6, /*!< Sai3Pre divider set to divided by 7. */
    kCLOCK_Sai3PreDivBy8 = 7, /*!< Sai3Pre divider set to divided by 8. */

    kCLOCK_Flexio2PreDivBy1 = 0, /*!< Flexio2Pre divider set to divided by 1. */
    kCLOCK_Flexio2PreDivBy2 = 1, /*!< Flexio2Pre divider set to divided by 2. */
    kCLOCK_Flexio2PreDivBy3 = 2, /*!< Flexio2Pre divider set to divided by 3. */
    kCLOCK_Flexio2PreDivBy4 = 3, /*!< Flexio2Pre divider set to divided by 4. */
    kCLOCK_Flexio2PreDivBy5 = 4, /*!< Flexio2Pre divider set to divided by 5. */
    kCLOCK_Flexio2PreDivBy6 = 5, /*!< Flexio2Pre divider set to divided by 6. */
    kCLOCK_Flexio2PreDivBy7 = 6, /*!< Flexio2Pre divider set to divided by 7. */
    kCLOCK_Flexio2PreDivBy8 = 7, /*!< Flexio2Pre divider set to divided by 8. */

    kCLOCK_Sai1PreDivBy1 = 0, /*!< Sai1Pre divider set to divided by 1. */
    kCLOCK_Sai1PreDivBy2 = 1, /*!< Sai1Pre divider set to divided by 2. */
    kCLOCK_Sai1PreDivBy3 = 2, /*!< Sai1Pre divider set to divided by 3. */
    kCLOCK_Sai1PreDivBy4 = 3, /*!< Sai1Pre divider set to divided by 4. */
    kCLOCK_Sai1PreDivBy5 = 4, /*!< Sai1Pre divider set to divided by 5. */
    kCLOCK_Sai1PreDivBy6 = 5, /*!< Sai1Pre divider set to divided by 6. */
    kCLOCK_Sai1PreDivBy7 = 6, /*!< Sai1Pre divider set to divided by 7. */
    kCLOCK_Sai1PreDivBy8 = 7, /*!< Sai1Pre divider set to divided by 8. */

    kCLOCK_Sai2PreDivBy1 = 0, /*!< Sai2Pre divider set to divided by 1. */
    kCLOCK_Sai2PreDivBy2 = 1, /*!< Sai2Pre divider set to divided by 2. */
    kCLOCK_Sai2PreDivBy3 = 2, /*!< Sai2Pre divider set to divided by 3. */
    kCLOCK_Sai2PreDivBy4 = 3, /*!< Sai2Pre divider set to divided by 4. */
    kCLOCK_Sai2PreDivBy5 = 4, /*!< Sai2Pre divider set to divided by 5. */
    kCLOCK_Sai2PreDivBy6 = 5, /*!< Sai2Pre divider set to divided by 6. */
    kCLOCK_Sai2PreDivBy7 = 6, /*!< Sai2Pre divider set to divided by 7. */
    kCLOCK_Sai2PreDivBy8 = 7, /*!< Sai2Pre divider set to divided by 8. */

    kCLOCK_Spdif0PreDivBy1 = 0, /*!< Spdif0Pre divider set to divided by 1. */
    kCLOCK_Spdif0PreDivBy2 = 1, /*!< Spdif0Pre divider set to divided by 2. */
    kCLOCK_Spdif0PreDivBy3 = 2, /*!< Spdif0Pre divider set to divided by 3. */
    kCLOCK_Spdif0PreDivBy4 = 3, /*!< Spdif0Pre divider set to divided by 4. */
    kCLOCK_Spdif0PreDivBy5 = 4, /*!< Spdif0Pre divider set to divided by 5. */
    kCLOCK_Spdif0PreDivBy6 = 5, /*!< Spdif0Pre divider set to divided by 6. */
    kCLOCK_Spdif0PreDivBy7 = 6, /*!< Spdif0Pre divider set to divided by 7. */
    kCLOCK_Spdif0PreDivBy8 = 7, /*!< Spdif0Pre divider set to divided by 8. */

    kCLOCK_Spdif0DivBy1 = 0, /*!< Spdif0 divider set to divided by 1. */
    kCLOCK_Spdif0DivBy2 = 1, /*!< Spdif0 divider set to divided by 2. */
    kCLOCK_Spdif0DivBy3 = 2, /*!< Spdif0 divider set to divided by 3. */
    kCLOCK_Spdif0DivBy4 = 3, /*!< Spdif0 divider set to divided by 4. */
    kCLOCK_Spdif0DivBy5 = 4, /*!< Spdif0 divider set to divided by 5. */
    kCLOCK_Spdif0DivBy6 = 5, /*!< Spdif0 divider set to divided by 6. */
    kCLOCK_Spdif0DivBy7 = 6, /*!< Spdif0 divider set to divided by 7. */
    kCLOCK_Spdif0DivBy8 = 7, /*!< Spdif0 divider set to divided by 8. */

    kCLOCK_Flexio1PreDivBy1 = 0, /*!< Flexio1Pre divider set to divided by 1. */
    kCLOCK_Flexio1PreDivBy2 = 1, /*!< Flexio1Pre divider set to divided by 2. */
    kCLOCK_Flexio1PreDivBy3 = 2, /*!< Flexio1Pre divider set to divided by 3. */
    kCLOCK_Flexio1PreDivBy8 = 7, /*!< Flexio1Pre divider set to divided by 8. */

    kCLOCK_Flexio1DivBy1 = 0, /*!< Flexio1 divider set to divided by 1. */
    kCLOCK_Flexio1DivBy2 = 1, /*!< Flexio1 divider set to divided by 2. */
    kCLOCK_Flexio1DivBy3 = 2, /*!< Flexio1 divider set to divided by 3. */
    kCLOCK_Flexio1DivBy4 = 3, /*!< Flexio1 divider set to divided by 4. */
    kCLOCK_Flexio1DivBy5 = 4, /*!< Flexio1 divider set to divided by 5. */
    kCLOCK_Flexio1DivBy6 = 5, /*!< Flexio1 divider set to divided by 6. */
    kCLOCK_Flexio1DivBy7 = 6, /*!< Flexio1 divider set to divided by 7. */
    kCLOCK_Flexio1DivBy8 = 7, /*!< Flexio1 divider set to divided by 8. */

    kCLOCK_LcdifPreDivBy1 = 0, /*!< LcdifPre divider set to divided by 1. */
    kCLOCK_LcdifPreDivBy2 = 1, /*!< LcdifPre divider set to divided by 2. */
    kCLOCK_LcdifPreDivBy3 = 2, /*!< LcdifPre divider set to divided by 3. */
    kCLOCK_LcdifPreDivBy4 = 3, /*!< LcdifPre divider set to divided by 4. */
    kCLOCK_LcdifPreDivBy5 = 4, /*!< LcdifPre divider set to divided by 5. */
    kCLOCK_LcdifPreDivBy6 = 5, /*!< LcdifPre divider set to divided by 6. */
    kCLOCK_LcdifPreDivBy7 = 6, /*!< LcdifPre divider set to divided by 7. */
    kCLOCK_LcdifPreDivBy8 = 7, /*!< LcdifPre divider set to divided by 8. */

    kCLOCK_CsiDivBy1 = 0, /*!< Csi divider set to divided by 1. */
    kCLOCK_CsiDivBy2 = 1, /*!< Csi divider set to divided by 2. */
    kCLOCK_CsiDivBy3 = 2, /*!< Csi divider set to divided by 3. */
    kCLOCK_CsiDivBy4 = 3, /*!< Csi divider set to divided by 4. */
    kCLOCK_CsiDivBy5 = 4, /*!< Csi divider set to divided by 5. */
    kCLOCK_CsiDivBy6 = 5, /*!< Csi divider set to divided by 6. */
    kCLOCK_CsiDivBy7 = 6, /*!< Csi divider set to divided by 7. */
    kCLOCK_CsiDivBy8 = 7, /*!< Csi divider set to divided by 8. */

    /* Only kCLOCK_PerClk, kCLOCK_Lpi2cDiv, kCLOCK_CanDiv, kCLOCK_UartDiv, kCLOCK_Sai1Div,
     * kCLOCK_Sai2Div, kCLOCK_Sai3Div can use these.
     */
    kCLOCK_MiscDivBy1  = 0,  /*!< Misc divider like LPI2C set to divided by 1 . */
    kCLOCK_MiscDivBy2  = 1,  /*!< Misc divider like LPI2C set to divided by 2 . */
    kCLOCK_MiscDivBy3  = 2,  /*!< Misc divider like LPI2C set to divided by 3 . */
    kCLOCK_MiscDivBy4  = 3,  /*!< Misc divider like LPI2C set to divided by 4 . */
    kCLOCK_MiscDivBy5  = 4,  /*!< Misc divider like LPI2C set to divided by 5 . */
    kCLOCK_MiscDivBy6  = 5,  /*!< Misc divider like LPI2C set to divided by 6 . */
    kCLOCK_MiscDivBy7  = 6,  /*!< Misc divider like LPI2C set to divided by 7 . */
    kCLOCK_MiscDivBy8  = 7,  /*!< Misc divider like LPI2C set to divided by 8 . */
    kCLOCK_MiscDivBy9  = 8,  /*!< Misc divider like LPI2C set to divided by 9 . */
    kCLOCK_MiscDivBy10 = 9,  /*!< Misc divider like LPI2C set to divided by 10. */
    kCLOCK_MiscDivBy11 = 10, /*!< Misc divider like LPI2C set to divided by 11. */
    kCLOCK_MiscDivBy12 = 11, /*!< Misc divider like LPI2C set to divided by 12. */
    kCLOCK_MiscDivBy13 = 12, /*!< Misc divider like LPI2C set to divided by 13. */
    kCLOCK_MiscDivBy14 = 13, /*!< Misc divider like LPI2C set to divided by 14. */
    kCLOCK_MiscDivBy15 = 14, /*!< Misc divider like LPI2C set to divided by 15. */
    kCLOCK_MiscDivBy16 = 15, /*!< Misc divider like LPI2C set to divided by 16. */
    kCLOCK_MiscDivBy17 = 16, /*!< Misc divider like LPI2C set to divided by 17. */
    kCLOCK_MiscDivBy18 = 17, /*!< Misc divider like LPI2C set to divided by 18. */
    kCLOCK_MiscDivBy19 = 18, /*!< Misc divider like LPI2C set to divided by 19. */
    kCLOCK_MiscDivBy20 = 19, /*!< Misc divider like LPI2C set to divided by 20. */
    kCLOCK_MiscDivBy21 = 20, /*!< Misc divider like LPI2C set to divided by 21. */
    kCLOCK_MiscDivBy22 = 21, /*!< Misc divider like LPI2C set to divided by 22. */
    kCLOCK_MiscDivBy23 = 22, /*!< Misc divider like LPI2C set to divided by 23. */
    kCLOCK_MiscDivBy24 = 23, /*!< Misc divider like LPI2C set to divided by 24. */
    kCLOCK_MiscDivBy25 = 24, /*!< Misc divider like LPI2C set to divided by 25. */
    kCLOCK_MiscDivBy26 = 25, /*!< Misc divider like LPI2C set to divided by 26. */
    kCLOCK_MiscDivBy27 = 26, /*!< Misc divider like LPI2C set to divided by 27. */
    kCLOCK_MiscDivBy28 = 27, /*!< Misc divider like LPI2C set to divided by 28. */
    kCLOCK_MiscDivBy29 = 28, /*!< Misc divider like LPI2C set to divided by 29. */
    kCLOCK_MiscDivBy30 = 29, /*!< Misc divider like LPI2C set to divided by 30. */
    kCLOCK_MiscDivBy31 = 30, /*!< Misc divider like LPI2C set to divided by 31. */
    kCLOCK_MiscDivBy32 = 31, /*!< Misc divider like LPI2C set to divided by 32. */
    kCLOCK_MiscDivBy33 = 32, /*!< Misc divider like LPI2C set to divided by 33. */
    kCLOCK_MiscDivBy34 = 33, /*!< Misc divider like LPI2C set to divided by 34. */
    kCLOCK_MiscDivBy35 = 34, /*!< Misc divider like LPI2C set to divided by 35. */
    kCLOCK_MiscDivBy36 = 35, /*!< Misc divider like LPI2C set to divided by 36. */
    kCLOCK_MiscDivBy37 = 36, /*!< Misc divider like LPI2C set to divided by 37. */
    kCLOCK_MiscDivBy38 = 37, /*!< Misc divider like LPI2C set to divided by 38. */
    kCLOCK_MiscDivBy39 = 38, /*!< Misc divider like LPI2C set to divided by 39. */
    kCLOCK_MiscDivBy40 = 39, /*!< Misc divider like LPI2C set to divided by 40. */
    kCLOCK_MiscDivBy41 = 40, /*!< Misc divider like LPI2C set to divided by 41. */
    kCLOCK_MiscDivBy42 = 41, /*!< Misc divider like LPI2C set to divided by 42. */
    kCLOCK_MiscDivBy43 = 42, /*!< Misc divider like LPI2C set to divided by 43. */
    kCLOCK_MiscDivBy44 = 43, /*!< Misc divider like LPI2C set to divided by 44. */
    kCLOCK_MiscDivBy45 = 44, /*!< Misc divider like LPI2C set to divided by 45. */
    kCLOCK_MiscDivBy46 = 45, /*!< Misc divider like LPI2C set to divided by 46. */
    kCLOCK_MiscDivBy47 = 46, /*!< Misc divider like LPI2C set to divided by 47. */
    kCLOCK_MiscDivBy48 = 47, /*!< Misc divider like LPI2C set to divided by 48. */
    kCLOCK_MiscDivBy49 = 48, /*!< Misc divider like LPI2C set to divided by 49. */
    kCLOCK_MiscDivBy50 = 49, /*!< Misc divider like LPI2C set to divided by 50. */
    kCLOCK_MiscDivBy51 = 50, /*!< Misc divider like LPI2C set to divided by 51. */
    kCLOCK_MiscDivBy52 = 51, /*!< Misc divider like LPI2C set to divided by 52. */
    kCLOCK_MiscDivBy53 = 52, /*!< Misc divider like LPI2C set to divided by 53. */
    kCLOCK_MiscDivBy54 = 53, /*!< Misc divider like LPI2C set to divided by 54. */
    kCLOCK_MiscDivBy55 = 54, /*!< Misc divider like LPI2C set to divided by 55. */
    kCLOCK_MiscDivBy56 = 55, /*!< Misc divider like LPI2C set to divided by 56. */
    kCLOCK_MiscDivBy57 = 56, /*!< Misc divider like LPI2C set to divided by 57. */
    kCLOCK_MiscDivBy58 = 57, /*!< Misc divider like LPI2C set to divided by 58. */
    kCLOCK_MiscDivBy59 = 58, /*!< Misc divider like LPI2C set to divided by 59. */
    kCLOCK_MiscDivBy60 = 59, /*!< Misc divider like LPI2C set to divided by 60. */
    kCLOCK_MiscDivBy61 = 60, /*!< Misc divider like LPI2C set to divided by 61. */
    kCLOCK_MiscDivBy62 = 61, /*!< Misc divider like LPI2C set to divided by 62. */
    kCLOCK_MiscDivBy63 = 62, /*!< Misc divider like LPI2C set to divided by 63. */
    kCLOCK_MiscDivBy64 = 63, /*!< Misc divider like LPI2C set to divided by 64. */
} clock_div_value_t;

/*!
 * @brief DIV control names for clock div setting.
 *
 * These constants define div control names for clock div setting.\n
 * - 0:7: REG offset to CCM_BASE in bytes.
 * - 8:15: Root clock setting bit field shift.
 * - 16:31: Root clock setting bit field width.
 */
typedef enum _clock_div
{
    kCLOCK_ArmDiv = CCM_TUPLE(CACRR_OFFSET,
                              CCM_CACRR_ARM_PODF_SHIFT,
                              CCM_CACRR_ARM_PODF_MASK,
                              CCM_CDHIPR_ARM_PODF_BUSY_SHIFT), /*!< core div name */

    kCLOCK_PeriphClk2Div = CCM_TUPLE(CBCDR_OFFSET,
                                     CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT,
                                     CCM_CBCDR_PERIPH_CLK2_PODF_MASK,
                                     CCM_NO_BUSY_WAIT), /*!< periph clock2 div name */
    kCLOCK_SemcDiv       = CCM_TUPLE(CBCDR_OFFSET,
                               CCM_CBCDR_SEMC_PODF_SHIFT,
                               CCM_CBCDR_SEMC_PODF_MASK,
                               CCM_CDHIPR_SEMC_PODF_BUSY_SHIFT), /*!< semc div name */
    kCLOCK_AhbDiv        = CCM_TUPLE(CBCDR_OFFSET,
                              CCM_CBCDR_AHB_PODF_SHIFT,
                              CCM_CBCDR_AHB_PODF_MASK,
                              CCM_CDHIPR_AHB_PODF_BUSY_SHIFT), /*!< ahb div name */
    kCLOCK_IpgDiv        = CCM_TUPLE(
        CBCDR_OFFSET, CCM_CBCDR_IPG_PODF_SHIFT, CCM_CBCDR_IPG_PODF_MASK, CCM_NO_BUSY_WAIT), /*!< ipg div name */

    kCLOCK_Flexspi2Div = CCM_TUPLE(CBCMR_OFFSET,
                                   CCM_CBCMR_FLEXSPI2_PODF_SHIFT,
                                   CCM_CBCMR_FLEXSPI2_PODF_MASK,
                                   CCM_NO_BUSY_WAIT), /*!< flexspi2 div name */
    kCLOCK_LpspiDiv    = CCM_TUPLE(
        CBCMR_OFFSET, CCM_CBCMR_LPSPI_PODF_SHIFT, CCM_CBCMR_LPSPI_PODF_MASK, CCM_NO_BUSY_WAIT), /*!< lpspi div name */
    kCLOCK_LcdifDiv = CCM_TUPLE(
        CBCMR_OFFSET, CCM_CBCMR_LCDIF_PODF_SHIFT, CCM_CBCMR_LCDIF_PODF_MASK, CCM_NO_BUSY_WAIT), /*!< lcdif div name */

    kCLOCK_FlexspiDiv = CCM_TUPLE(CSCMR1_OFFSET,
                                  CCM_CSCMR1_FLEXSPI_PODF_SHIFT,
                                  CCM_CSCMR1_FLEXSPI_PODF_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexspi div name */
    kCLOCK_PerclkDiv  = CCM_TUPLE(CSCMR1_OFFSET,
                                 CCM_CSCMR1_PERCLK_PODF_SHIFT,
                                 CCM_CSCMR1_PERCLK_PODF_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< perclk div name */

    kCLOCK_CanDiv = CCM_TUPLE(CSCMR2_OFFSET,
                              CCM_CSCMR2_CAN_CLK_PODF_SHIFT,
                              CCM_CSCMR2_CAN_CLK_PODF_MASK,
                              CCM_NO_BUSY_WAIT), /*!< can div name */

    kCLOCK_TraceDiv  = CCM_TUPLE(CSCDR1_OFFSET,
                                CCM_CSCDR1_TRACE_PODF_SHIFT,
                                CCM_CSCDR1_TRACE_PODF_MASK,
                                CCM_NO_BUSY_WAIT), /*!< trace div name */
    kCLOCK_Usdhc2Div = CCM_TUPLE(CSCDR1_OFFSET,
                                 CCM_CSCDR1_USDHC2_PODF_SHIFT,
                                 CCM_CSCDR1_USDHC2_PODF_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< usdhc2 div name */
    kCLOCK_Usdhc1Div = CCM_TUPLE(CSCDR1_OFFSET,
                                 CCM_CSCDR1_USDHC1_PODF_SHIFT,
                                 CCM_CSCDR1_USDHC1_PODF_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< usdhc1 div name */
    kCLOCK_UartDiv   = CCM_TUPLE(CSCDR1_OFFSET,
                               CCM_CSCDR1_UART_CLK_PODF_SHIFT,
                               CCM_CSCDR1_UART_CLK_PODF_MASK,
                               CCM_NO_BUSY_WAIT), /*!< uart div name */

    kCLOCK_Flexio2Div    = CCM_TUPLE(CS1CDR_OFFSET,
                                  CCM_CS1CDR_FLEXIO2_CLK_PODF_SHIFT,
                                  CCM_CS1CDR_FLEXIO2_CLK_PODF_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexio2 pre div name */
    kCLOCK_Sai3PreDiv    = CCM_TUPLE(CS1CDR_OFFSET,
                                  CCM_CS1CDR_SAI3_CLK_PRED_SHIFT,
                                  CCM_CS1CDR_SAI3_CLK_PRED_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< sai3 pre div name */
    kCLOCK_Sai3Div       = CCM_TUPLE(CS1CDR_OFFSET,
                               CCM_CS1CDR_SAI3_CLK_PODF_SHIFT,
                               CCM_CS1CDR_SAI3_CLK_PODF_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai3 div name */
    kCLOCK_Flexio2PreDiv = CCM_TUPLE(CS1CDR_OFFSET,
                                     CCM_CS1CDR_FLEXIO2_CLK_PRED_SHIFT,
                                     CCM_CS1CDR_FLEXIO2_CLK_PRED_MASK,
                                     CCM_NO_BUSY_WAIT), /*!< sai3 pre div name */
    kCLOCK_Sai1PreDiv    = CCM_TUPLE(CS1CDR_OFFSET,
                                  CCM_CS1CDR_SAI1_CLK_PRED_SHIFT,
                                  CCM_CS1CDR_SAI1_CLK_PRED_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< sai1 pre div name */
    kCLOCK_Sai1Div       = CCM_TUPLE(CS1CDR_OFFSET,
                               CCM_CS1CDR_SAI1_CLK_PODF_SHIFT,
                               CCM_CS1CDR_SAI1_CLK_PODF_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai1 div name */

    kCLOCK_Sai2PreDiv = CCM_TUPLE(CS2CDR_OFFSET,
                                  CCM_CS2CDR_SAI2_CLK_PRED_SHIFT,
                                  CCM_CS2CDR_SAI2_CLK_PRED_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< sai2 pre div name */
    kCLOCK_Sai2Div    = CCM_TUPLE(CS2CDR_OFFSET,
                               CCM_CS2CDR_SAI2_CLK_PODF_SHIFT,
                               CCM_CS2CDR_SAI2_CLK_PODF_MASK,
                               CCM_NO_BUSY_WAIT), /*!< sai2 div name */

    kCLOCK_Spdif0PreDiv  = CCM_TUPLE(CDCDR_OFFSET,
                                    CCM_CDCDR_SPDIF0_CLK_PRED_SHIFT,
                                    CCM_CDCDR_SPDIF0_CLK_PRED_MASK,
                                    CCM_NO_BUSY_WAIT), /*!< spdif pre div name */
    kCLOCK_Spdif0Div     = CCM_TUPLE(CDCDR_OFFSET,
                                 CCM_CDCDR_SPDIF0_CLK_PODF_SHIFT,
                                 CCM_CDCDR_SPDIF0_CLK_PODF_MASK,
                                 CCM_NO_BUSY_WAIT), /*!< spdif div name */
    kCLOCK_Flexio1PreDiv = CCM_TUPLE(CDCDR_OFFSET,
                                     CCM_CDCDR_FLEXIO1_CLK_PRED_SHIFT,
                                     CCM_CDCDR_FLEXIO1_CLK_PRED_MASK,
                                     CCM_NO_BUSY_WAIT), /*!< flexio1 pre div name */
    kCLOCK_Flexio1Div    = CCM_TUPLE(CDCDR_OFFSET,
                                  CCM_CDCDR_FLEXIO1_CLK_PODF_SHIFT,
                                  CCM_CDCDR_FLEXIO1_CLK_PODF_MASK,
                                  CCM_NO_BUSY_WAIT), /*!< flexio1 div name */

    kCLOCK_Lpi2cDiv    = CCM_TUPLE(CSCDR2_OFFSET,
                                CCM_CSCDR2_LPI2C_CLK_PODF_SHIFT,
                                CCM_CSCDR2_LPI2C_CLK_PODF_MASK,
                                CCM_NO_BUSY_WAIT), /*!< lpi2c div name */
    kCLOCK_LcdifPreDiv = CCM_TUPLE(CSCDR2_OFFSET,
                                   CCM_CSCDR2_LCDIF_PRED_SHIFT,
                                   CCM_CSCDR2_LCDIF_PRED_MASK,
                                   CCM_NO_BUSY_WAIT), /*!< lcdif pre div name */

    kCLOCK_CsiDiv = CCM_TUPLE(
        CSCDR3_OFFSET, CCM_CSCDR3_CSI_PODF_SHIFT, CCM_CSCDR3_CSI_PODF_MASK, CCM_NO_BUSY_WAIT), /*!< csi div name */

    kCLOCK_NonePreDiv = CLOCK_ROOT_NONE_PRE_DIV, /*!< None Pre div. */
} clock_div_t;

/*! @brief USB clock source definition. */
typedef enum _clock_usb_src
{
    kCLOCK_Usb480M      = 0,                /*!< Use 480M.      */
    kCLOCK_UsbSrcUnused = (int)0xFFFFFFFFU, /*!< Used when the function does not
                                            care the clock source. */
} clock_usb_src_t;

/*! @brief Source of the USB HS PHY. */
typedef enum _clock_usb_phy_src
{
    kCLOCK_Usbphy480M = 0, /*!< Use 480M.      */
} clock_usb_phy_src_t;

/*!@brief PLL clock source, bypass cloco source also */
enum _clock_pll_clk_src
{
    kCLOCK_PllClkSrc24M = 0U, /*!< Pll clock source 24M */
    kCLOCK_PllSrcClkPN  = 1U, /*!< Pll clock source CLK1_P and CLK1_N */
};

/*! @brief PLL configuration for ARM */
typedef struct _clock_arm_pll_config
{
    uint32_t loopDivider; /*!< PLL loop divider. Valid range for divider value: 54-108. Fout=Fin*loopDivider/2. */
    uint8_t src;          /*!< Pll clock source, reference _clock_pll_clk_src */
} clock_arm_pll_config_t;

/*! @brief PLL configuration for USB */
typedef struct _clock_usb_pll_config
{
    uint8_t loopDivider; /*!< PLL loop divider.
                              0 - Fout=Fref*20;
                              1 - Fout=Fref*22 */
    uint8_t src;         /*!< Pll clock source, reference _clock_pll_clk_src */

} clock_usb_pll_config_t;

/*! @brief PLL configuration for System */
typedef struct _clock_sys_pll_config
{
    uint8_t loopDivider;  /*!< PLL loop divider. Intended to be 1 (528M).
                               0 - Fout=Fref*20;
                               1 - Fout=Fref*22 */
    uint32_t numerator;   /*!< 30 bit numerator of fractional loop divider.*/
    uint32_t denominator; /*!< 30 bit denominator of fractional loop divider */
    uint8_t src;          /*!< Pll clock source, reference _clock_pll_clk_src */
    uint16_t ss_stop;     /*!< Stop value to get frequency change. */
    uint8_t ss_enable;    /*!< Enable spread spectrum modulation */
    uint16_t ss_step;     /*!< Step value to get frequency change step. */
} clock_sys_pll_config_t;

/*! @brief PLL configuration for AUDIO and VIDEO */
typedef struct _clock_audio_pll_config
{
    uint8_t loopDivider;  /*!< PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    uint8_t postDivider;  /*!< Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    uint32_t numerator;   /*!< 30 bit numerator of fractional loop divider.*/
    uint32_t denominator; /*!< 30 bit denominator of fractional loop divider */
    uint8_t src;          /*!< Pll clock source, reference _clock_pll_clk_src */
} clock_audio_pll_config_t;

/*! @brief PLL configuration for AUDIO and VIDEO */
typedef struct _clock_video_pll_config
{
    uint8_t loopDivider;  /*!< PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    uint8_t postDivider;  /*!< Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    uint32_t numerator;   /*!< 30 bit numerator of fractional loop divider.*/
    uint32_t denominator; /*!< 30 bit denominator of fractional loop divider */
    uint8_t src;          /*!< Pll clock source, reference _clock_pll_clk_src */

} clock_video_pll_config_t;

/*! @brief PLL configuration for ENET */
typedef struct _clock_enet_pll_config
{
    bool enableClkOutput;    /*!< Power on and enable PLL clock output for ENET0 (ref_enetpll0). */
    bool enableClkOutput25M; /*!< Power on and enable PLL clock output for ENET2 (ref_enetpll2). */
    uint8_t loopDivider;     /*!< Controls the frequency of the ENET0 reference clock.
                                  b00 25MHz
                                  b01 50MHz
                                  b10 100MHz (not 50% duty cycle)
                                  b11 125MHz */
    uint8_t src;             /*!< Pll clock source, reference _clock_pll_clk_src */
    bool enableClkOutput1;   /*!< Power on and enable PLL clock output for ENET1 (ref_enetpll1). */
    uint8_t loopDivider1;    /*!< Controls the frequency of the ENET1 reference clock.
                                  b00 25MHz
                                  b01 50MHz
                                  b10 100MHz (not 50% duty cycle)
                                  b11 125MHz */
} clock_enet_pll_config_t;

/*! @brief PLL name */
typedef enum _clock_pll
{
    kCLOCK_PllArm   = CCM_ANALOG_TUPLE(PLL_ARM_OFFSET, CCM_ANALOG_PLL_ARM_ENABLE_SHIFT),     /*!< PLL ARM */
    kCLOCK_PllSys   = CCM_ANALOG_TUPLE(PLL_SYS_OFFSET, CCM_ANALOG_PLL_SYS_ENABLE_SHIFT),     /*!< PLL SYS */
    kCLOCK_PllUsb1  = CCM_ANALOG_TUPLE(PLL_USB1_OFFSET, CCM_ANALOG_PLL_USB1_ENABLE_SHIFT),   /*!< PLL USB1 */
    kCLOCK_PllAudio = CCM_ANALOG_TUPLE(PLL_AUDIO_OFFSET, CCM_ANALOG_PLL_AUDIO_ENABLE_SHIFT), /*!< PLL Audio */
    kCLOCK_PllVideo = CCM_ANALOG_TUPLE(PLL_VIDEO_OFFSET, CCM_ANALOG_PLL_VIDEO_ENABLE_SHIFT), /*!< PLL Video */

    kCLOCK_PllEnet    = CCM_ANALOG_TUPLE(PLL_ENET_OFFSET, CCM_ANALOG_PLL_ENET_ENABLE_SHIFT),          /*!< PLL Enet0 */
    kCLOCK_PllEnet2   = CCM_ANALOG_TUPLE(PLL_ENET_OFFSET, CCM_ANALOG_PLL_ENET_ENET2_REF_EN_SHIFT),    /*!< PLL Enet1 */
    kCLOCK_PllEnet25M = CCM_ANALOG_TUPLE(PLL_ENET_OFFSET, CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_SHIFT), /*!< PLL Enet2 */

    kCLOCK_PllUsb2 = CCM_ANALOG_TUPLE(PLL_USB2_OFFSET, CCM_ANALOG_PLL_USB2_ENABLE_SHIFT), /*!< PLL USB2 */

} clock_pll_t;

/*! @brief PLL PFD name */
typedef enum _clock_pfd
{
    kCLOCK_Pfd0 = 0U, /*!< PLL PFD0 */
    kCLOCK_Pfd1 = 1U, /*!< PLL PFD1 */
    kCLOCK_Pfd2 = 2U, /*!< PLL PFD2 */
    kCLOCK_Pfd3 = 3U, /*!< PLL PFD3 */
} clock_pfd_t;

/*!
 * @brief The enumerater of clock output1's clock source, such as USB1 PLL, SYS PLL and so on.
 */
typedef enum _clock_output1_selection
{
    kCLOCK_OutputPllUsb1       = 0U,    /*!< Selects USB1 PLL clock(Divided by 2) output. */
    kCLOCK_OutputPllSys        = 1U,    /*!< Selects SYS PLL clock(Divided by 2) output. */
    kCLOCK_OutputPllVideo      = 3U,    /*!< Selects Video PLL clock(Divided by 2) output. */
    kCLOCK_OutputSemcClk       = 5U,    /*!< Selects semc clock root output. */
    kCLOCK_OutputLcdifPixClk   = 0xAU,  /*!< Selects Lcdif pix clock root output. */
    kCLOCK_OutputAhbClk        = 0xBU,  /*!< Selects AHB clock root output. */
    kCLOCK_OutputIpgClk        = 0xCU,  /*!< Selects IPG clock root output. */
    kCLOCK_OutputPerClk        = 0xDU,  /*!< Selects PERCLK clock root output. */
    kCLOCK_OutputCkilSyncClk   = 0xEU,  /*!< Selects Ckil clock root output. */
    kCLOCK_OutputPll4MainClk   = 0xFU,  /*!< Selects PLL4 main clock output. */
    kCLOCK_DisableClockOutput1 = 0x10U, /*!< Disables CLKO1. */
} clock_output1_selection_t;

/*!
 * @brief The enumerater of clock output2's clock source, such as USDHC1 clock root, LPI2C clock root and so on.
 *
 */
typedef enum _clock_output2_selection
{
    kCLOCK_OutputUsdhc1Clk     = 3U,    /*!< Selects USDHC1 clock root output. */
    kCLOCK_OutputLpi2cClk      = 6U,    /*!< Selects LPI2C clock root output. */
    kCLOCK_OutputCsiClk        = 0xBU,  /*!< Selects CSI clock root output. */
    kCLOCK_OutputOscClk        = 0xEU,  /*!< Selects OSC output. */
    kCLOCK_OutputUsdhc2Clk     = 0x11U, /*!< Selects USDHC2 clock root output. */
    kCLOCK_OutputSai1Clk       = 0x12U, /*!< Selects SAI1 clock root output. */
    kCLOCK_OutputSai2Clk       = 0x13U, /*!< Selects SAI2 clock root output. */
    kCLOCK_OutputSai3Clk       = 0x14U, /*!< Selects SAI3 clock root output. */
    kCLOCK_OutputCanClk        = 0x17U, /*!< Selects CAN clock root output. */
    kCLOCK_OutputFlexspiClk    = 0x1BU, /*!< Selects FLEXSPI clock root output. */
    kCLOCK_OutputUartClk       = 0x1CU, /*!< Selects UART clock root output. */
    kCLOCK_OutputSpdif0Clk     = 0x1DU, /*!< Selects SPDIF0 clock root output. */
    kCLOCK_DisableClockOutput2 = 0x1FU, /*!< Disables CLKO2. */
} clock_output2_selection_t;

/*!
 * @brief The enumerator of clock output's divider.
 */
typedef enum _clock_output_divider
{
    kCLOCK_DivideBy1 = 0U, /*!< Output clock divided by 1. */
    kCLOCK_DivideBy2,      /*!< Output clock divided by 2. */
    kCLOCK_DivideBy3,      /*!< Output clock divided by 3. */
    kCLOCK_DivideBy4,      /*!< Output clock divided by 4. */
    kCLOCK_DivideBy5,      /*!< Output clock divided by 5. */
    kCLOCK_DivideBy6,      /*!< Output clock divided by 6. */
    kCLOCK_DivideBy7,      /*!< Output clock divided by 7. */
    kCLOCK_DivideBy8,      /*!< Output clock divided by 8. */
} clock_output_divider_t;

/*!
 * @brief The enumerator of clock root.
 */
typedef enum _clock_root
{
    kCLOCK_Usdhc1ClkRoot = 0U, /*!< USDHC1 clock root. */
    kCLOCK_Usdhc2ClkRoot,      /*!< USDHC2 clock root. */
    kCLOCK_FlexspiClkRoot,     /*!< FLEXSPI clock root. */
    kCLOCK_Flexspi2ClkRoot,    /*!< FLEXSPI2 clock root. */
    kCLOCK_CsiClkRoot,         /*!< CSI clock root. */
    kCLOCK_LpspiClkRoot,       /*!< LPSPI clock root. */
    kCLOCK_TraceClkRoot,       /*!< Trace clock root. */
    kCLOCK_Sai1ClkRoot,        /*!< SAI1 clock root. */
    kCLOCK_Sai2ClkRoot,        /*!< SAI2 clock root. */
    kCLOCK_Sai3ClkRoot,        /*!< SAI3 clock root. */
    kCLOCK_Lpi2cClkRoot,       /*!< LPI2C clock root. */
    kCLOCK_CanClkRoot,         /*!< CAN clock root. */
    kCLOCK_UartClkRoot,        /*!< UART clock root. */
    kCLOCK_LcdifClkRoot,       /*!< LCD clock root. */
    kCLOCK_SpdifClkRoot,       /*!< SPDIF clock root. */
    kCLOCK_Flexio1ClkRoot,     /*!< FLEXIO1 clock root. */
    kCLOCK_Flexio2ClkRoot,     /*!< FLEXIO2 clock root. */
} clock_root_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Set CCM MUX node to certain value.
 *
 * @param mux   Which mux node to set, see \ref clock_mux_t.
 * @param value Clock mux value to set, different mux has different value range.
 */
static inline void CLOCK_SetMux(clock_mux_t mux, uint32_t value)
{
    uint32_t busyShift;

    busyShift               = (uint32_t)CCM_TUPLE_BUSY_SHIFT(mux);
    CCM_TUPLE_REG(CCM, mux) = (CCM_TUPLE_REG(CCM, mux) & (~CCM_TUPLE_MASK(mux))) |
                              (((uint32_t)((value) << CCM_TUPLE_SHIFT(mux))) & CCM_TUPLE_MASK(mux));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* Clock switch need Handshake? */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* Wait until CCM internal handshake finish. */
        while ((CCM->CDHIPR & ((1UL << busyShift))) != 0UL)
        {
        }
    }
}

/*!
 * @brief Get CCM MUX value.
 *
 * @param mux   Which mux node to get, see \ref clock_mux_t.
 * @return Clock mux value.
 */
static inline uint32_t CLOCK_GetMux(clock_mux_t mux)
{
    return (((uint32_t)(CCM_TUPLE_REG(CCM, mux) & CCM_TUPLE_MASK(mux))) >> CCM_TUPLE_SHIFT(mux));
}

/*!
 * @brief Set clock divider value.
 *
 * Example, set the ARM clock divider to divide by 2:
 * @code
   CLOCK_SetDiv(kCLOCK_ArmDiv, kCLOCK_ArmDivBy2);
   @endcode
 *
 * Example, set the LPI2C clock divider to divide by 5.
 * @code
   CLOCK_SetDiv(kCLOCK_Lpi2cDiv, kCLOCK_MiscDivBy5);
   @endcode
 *
 * Only @ref kCLOCK_PerClk, @ref kCLOCK_Lpi2cDiv, @ref kCLOCK_CanDiv, @ref kCLOCK_UartDiv, @ref kCLOCK_Sai1Div,
 * @ref kCLOCK_Sai2Div, @ref kCLOCK_Sai3Div can use the divider kCLOCK_MiscDivByxxx.
 *
 * @param divider Which divider node to set.
 * @param value   Clock div value to set, different divider has different value range. See @ref clock_div_value_t
 *                for details.
 *                Divided clock frequency = Undivided clock frequency / (value + 1)
 */
static inline void CLOCK_SetDiv(clock_div_t divider, uint32_t value)
{
    uint32_t busyShift;

    busyShift                   = CCM_TUPLE_BUSY_SHIFT(divider);
    CCM_TUPLE_REG(CCM, divider) = (CCM_TUPLE_REG(CCM, divider) & (~CCM_TUPLE_MASK(divider))) |
                                  (((uint32_t)((value) << CCM_TUPLE_SHIFT(divider))) & CCM_TUPLE_MASK(divider));

    assert(busyShift <= CCM_NO_BUSY_WAIT);

    /* Clock switch need Handshake? */
    if (CCM_NO_BUSY_WAIT != busyShift)
    {
        /* Wait until CCM internal handshake finish. */
        while ((CCM->CDHIPR & ((uint32_t)(1UL << busyShift))) != 0UL)
        {
        }
    }
}

/*!
 * @brief Get CCM DIV node value.
 *
 * @param divider Which div node to get, see \ref clock_div_t.
 */
static inline uint32_t CLOCK_GetDiv(clock_div_t divider)
{
    return ((uint32_t)(CCM_TUPLE_REG(CCM, divider) & CCM_TUPLE_MASK(divider)) >> CCM_TUPLE_SHIFT(divider));
}

/*!
 * @brief Control the clock gate for specific IP.
 *
 * @param name  Which clock to enable, see \ref clock_ip_name_t.
 * @param value Clock gate value to set, see \ref clock_gate_value_t.
 */
static inline void CLOCK_ControlGate(clock_ip_name_t name, clock_gate_value_t value)
{
    uint32_t index = ((uint32_t)name) >> 8UL;
    uint32_t shift = ((uint32_t)name) & 0x1FUL;
    volatile uint32_t *reg;

    assert(index <= 7UL);

    reg = (volatile uint32_t *)(&(((volatile uint32_t *)&CCM->CCGR0)[index]));
    SDK_ATOMIC_LOCAL_CLEAR_AND_SET(reg, (3UL << shift), (uint32_t)(((uint32_t)value) << (uint32_t)shift));
}

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param name  Which clock to enable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_EnableClock(clock_ip_name_t name)
{
    CLOCK_ControlGate(name, kCLOCK_ClockNeededRunWait);
}

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param name  Which clock to disable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_DisableClock(clock_ip_name_t name)
{
    CLOCK_ControlGate(name, kCLOCK_ClockNotNeeded);
}

/*!
 * @brief Setting the low power mode that system will enter on next assertion of dsm_request signal.
 *
 * @param mode  Which mode to enter, see \ref clock_mode_t.
 */
static inline void CLOCK_SetMode(clock_mode_t mode)
{
    CCM->CLPCR = (CCM->CLPCR & ~CCM_CLPCR_LPM_MASK) | CCM_CLPCR_LPM((uint32_t)mode);
}

/*!
 * @brief Gets the OSC clock frequency.
 *
 * This function will return the external XTAL OSC frequency if it is selected as the source of OSC,
 * otherwise internal 24MHz RC OSC frequency will be returned.
 *
 * @return  Clock frequency; If the clock is invalid, returns 0.
 */
static inline uint32_t CLOCK_GetOscFreq(void)
{
    return ((XTALOSC24M->LOWPWR_CTRL & XTALOSC24M_LOWPWR_CTRL_OSC_SEL_MASK) != 0UL) ? 24000000UL : g_xtalFreq;
}

/*!
 * @brief Gets the AHB clock frequency.
 *
 * @return  The AHB clock frequency value in hertz.
 */
uint32_t CLOCK_GetAhbFreq(void);

/*!
 * @brief Gets the SEMC clock frequency.
 *
 * @return  The SEMC clock frequency value in hertz.
 */
uint32_t CLOCK_GetSemcFreq(void);

/*!
 * @brief Gets the IPG clock frequency.
 *
 * @return  The IPG clock frequency value in hertz.
 */
uint32_t CLOCK_GetIpgFreq(void);

/*!
 * @brief Gets the PER clock frequency.
 *
 * @return  The PER clock frequency value in hertz.
 */
uint32_t CLOCK_GetPerClkFreq(void);

/*!
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_name_t.
 *
 * @param name Clock names defined in clock_name_t
 * @return Clock frequency value in hertz
 */
uint32_t CLOCK_GetFreq(clock_name_t name);

/*!
 * @brief Get the CCM CPU/core/system frequency.
 *
 * @return  Clock frequency; If the clock is invalid, returns 0.
 */
static inline uint32_t CLOCK_GetCpuClkFreq(void)
{
    return CLOCK_GetFreq(kCLOCK_CpuClk);
}

/*!
 * @brief Gets the frequency of selected clock root.
 *
 * @param clockRoot The clock root used to get the frequency, please refer to @ref clock_root_t.
 * @return The frequency of selected clock root.
 */
uint32_t CLOCK_GetClockRootFreq(clock_root_t clockRoot);

/*!
 * @name OSC operations
 * @{
 */

/*!
 * @brief Initialize the external 24MHz clock.
 *
 * This function supports two modes:
 * 1. Use external crystal oscillator.
 * 2. Bypass the external crystal oscillator, using input source clock directly.
 *
 * After this function, please call CLOCK_SetXtal0Freq to inform clock driver
 * the external clock frequency.
 *
 * @param bypassXtalOsc Pass in true to bypass the external crystal oscillator.
 * @note This device does not support bypass external crystal oscillator, so
 * the input parameter should always be false.
 */
void CLOCK_InitExternalClk(bool bypassXtalOsc);

/*!
 * @brief Deinitialize the external 24MHz clock.
 *
 * This function disables the external 24MHz clock.
 *
 * After this function, please call CLOCK_SetXtal0Freq to set external clock
 * frequency to 0.
 */
void CLOCK_DeinitExternalClk(void);

/*!
 * @brief Switch the OSC.
 *
 * This function switches the OSC source for SoC.
 *
 * @param osc   OSC source to switch to.
 */
void CLOCK_SwitchOsc(clock_osc_t osc);

/*!
 * @brief Gets the RTC clock frequency.
 *
 * @return  Clock frequency; If the clock is invalid, returns 0.
 */
static inline uint32_t CLOCK_GetRtcFreq(void)
{
    return 32768U;
}

/*!
 * @brief Set the XTAL (24M OSC) frequency based on board setting.
 *
 * @param freq The XTAL input clock frequency in Hz.
 */
static inline void CLOCK_SetXtalFreq(uint32_t freq)
{
    g_xtalFreq = freq;
}

/*!
 * @brief Set the RTC XTAL (32K OSC) frequency based on board setting.
 *
 * @param freq The RTC XTAL input clock frequency in Hz.
 */
static inline void CLOCK_SetRtcXtalFreq(uint32_t freq)
{
    g_rtcXtalFreq = freq;
}

/*!
 * @brief Initialize the RC oscillator 24MHz clock.
 */
void CLOCK_InitRcOsc24M(void);

/*!
 * @brief Power down the RCOSC 24M clock.
 */
void CLOCK_DeinitRcOsc24M(void);
/* @} */

/*! @brief Enable USB HS clock.
 *
 * This function only enables the access to USB HS prepheral, upper layer
 * should first call the CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 * clock to use USB HS.
 *
 * @param src  USB HS does not care about the clock source, here must be @ref kCLOCK_UsbSrcUnused.
 * @param freq USB HS does not care about the clock source, so this parameter is ignored.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0Clock(clock_usb_src_t src, uint32_t freq);

/*! @brief Enable USB HS clock.
 *
 * This function only enables the access to USB HS prepheral, upper layer
 * should first call the CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 * clock to use USB HS.
 *
 * @param src  USB HS does not care about the clock source, here must be @ref kCLOCK_UsbSrcUnused.
 * @param freq USB HS does not care about the clock source, so this parameter is ignored.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs1Clock(clock_usb_src_t src, uint32_t freq);

/* @} */

/*!
 * @name PLL/PFD operations
 * @{
 */
/*!
 * @brief PLL bypass setting
 *
 * @param base CCM_ANALOG base pointer.
 * @param pll PLL control name (see @ref ccm_analog_pll_control_t enumeration)
 * @param bypass Bypass the PLL.
 *               - true: Bypass the PLL.
 *               - false:Not bypass the PLL.
 */
static inline void CLOCK_SetPllBypass(CCM_ANALOG_Type *base, clock_pll_t pll, bool bypass)
{
    if (bypass)
    {
        CCM_ANALOG_TUPLE_REG_OFF(base, pll, 4U) = 1UL << CCM_ANALOG_PLL_BYPASS_SHIFT;
    }
    else
    {
        CCM_ANALOG_TUPLE_REG_OFF(base, pll, 8U) = 1UL << CCM_ANALOG_PLL_BYPASS_SHIFT;
    }
}

/*!
 * @brief Check if PLL is bypassed
 *
 * @param base CCM_ANALOG base pointer.
 * @param pll PLL control name (see @ref ccm_analog_pll_control_t enumeration)
 * @return PLL bypass status.
 *         - true: The PLL is bypassed.
 *         - false: The PLL is not bypassed.
 */
static inline bool CLOCK_IsPllBypassed(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return (bool)(CCM_ANALOG_TUPLE_REG(base, pll) & (1UL << CCM_ANALOG_PLL_BYPASS_SHIFT));
}

/*!
 * @brief Check if PLL is enabled
 *
 * @param base CCM_ANALOG base pointer.
 * @param pll PLL control name (see @ref ccm_analog_pll_control_t enumeration)
 * @return PLL bypass status.
 *         - true: The PLL is enabled.
 *         - false: The PLL is not enabled.
 */
static inline bool CLOCK_IsPllEnabled(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return ((CCM_ANALOG_TUPLE_REG(base, pll) & (1UL << CCM_ANALOG_TUPLE_SHIFT(pll))) != 0U);
}

/*!
 * @brief PLL bypass clock source setting.
 * Note: change the bypass clock source also change the pll reference clock source.
 *
 * @param base CCM_ANALOG base pointer.
 * @param pll PLL control name (see @ref ccm_analog_pll_control_t enumeration)
 * @param src Bypass clock source, reference _clock_pll_bypass_clk_src.
 */
static inline void CLOCK_SetPllBypassRefClkSrc(CCM_ANALOG_Type *base, clock_pll_t pll, uint32_t src)
{
    CCM_ANALOG_TUPLE_REG(base, pll) |= (CCM_ANALOG_TUPLE_REG(base, pll) & (~CCM_ANALOG_PLL_BYPASS_CLK_SRC_MASK)) | src;
}

/*!
 * @brief Get PLL bypass clock value, it is PLL reference clock actually.
 * If CLOCK1_P,CLOCK1_N is choose as the pll bypass clock source, please implement the CLKPN_FREQ define, otherwise 0
 * will be returned.
 * @param base CCM_ANALOG base pointer.
 * @param pll PLL control name (see @ref ccm_analog_pll_control_t enumeration)
 * @retval bypass reference clock frequency value.
 */
static inline uint32_t CLOCK_GetPllBypassRefClk(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return (((CCM_ANALOG_TUPLE_REG(base, pll) & CCM_ANALOG_PLL_BYPASS_CLK_SRC_MASK) >>
             CCM_ANALOG_PLL_BYPASS_CLK_SRC_SHIFT) == (uint32_t)kCLOCK_PllClkSrc24M) ?
               CLOCK_GetOscFreq() :
               CLKPN_FREQ;
}

/*!
 * @brief Initialize the ARM PLL.
 *
 * This function initialize the ARM PLL with specific settings
 *
 * @param config   configuration to set to PLL.
 */
void CLOCK_InitArmPll(const clock_arm_pll_config_t *config);

/*!
 * @brief De-initialize the ARM PLL.
 */
void CLOCK_DeinitArmPll(void);

/*!
 * @brief Initialize the System PLL.
 *
 * This function initializes the System PLL with specific settings
 *
 * @param config Configuration to set to PLL.
 */
void CLOCK_InitSysPll(const clock_sys_pll_config_t *config);

/*!
 * @brief De-initialize the System PLL.
 */
void CLOCK_DeinitSysPll(void);

/*!
 * @brief Initialize the USB1 PLL.
 *
 * This function initializes the USB1 PLL with specific settings
 *
 * @param config Configuration to set to PLL.
 */
void CLOCK_InitUsb1Pll(const clock_usb_pll_config_t *config);

/*!
 * @brief Deinitialize the USB1 PLL.
 */
void CLOCK_DeinitUsb1Pll(void);

/*!
 * @brief Initialize the USB2 PLL.
 *
 * This function initializes the USB2 PLL with specific settings
 *
 * @param config Configuration to set to PLL.
 */
void CLOCK_InitUsb2Pll(const clock_usb_pll_config_t *config);

/*!
 * @brief Deinitialize the USB2 PLL.
 */
void CLOCK_DeinitUsb2Pll(void);

/*!
 * @brief Initializes the Audio PLL.
 *
 * This function initializes the Audio PLL with specific settings
 *
 * @param config Configuration to set to PLL.
 */
void CLOCK_InitAudioPll(const clock_audio_pll_config_t *config);

/*!
 * @brief De-initialize the Audio PLL.
 */
void CLOCK_DeinitAudioPll(void);

/*!
 * @brief Initialize the video PLL.
 *
 * This function configures the Video PLL with specific settings
 *
 * @param config   configuration to set to PLL.
 */
void CLOCK_InitVideoPll(const clock_video_pll_config_t *config);

/*!
 * @brief De-initialize the Video PLL.
 */
void CLOCK_DeinitVideoPll(void);
/*!
 * @brief Initialize the ENET PLL.
 *
 * This function initializes the ENET PLL with specific settings.
 *
 * @param config Configuration to set to PLL.
 */
void CLOCK_InitEnetPll(const clock_enet_pll_config_t *config);

/*!
 * @brief Deinitialize the ENET PLL.
 *
 * This function disables the ENET PLL.
 */
void CLOCK_DeinitEnetPll(void);

/*!
 * @brief Get current PLL output frequency.
 *
 * This function get current output frequency of specific PLL
 *
 * @param pll   pll name to get frequency.
 * @return The PLL output frequency in hertz.
 */
uint32_t CLOCK_GetPllFreq(clock_pll_t pll);

/*!
 * @brief Initialize the System PLL PFD.
 *
 * This function initializes the System PLL PFD. During new value setting,
 * the clock output is disabled to prevent glitch.
 *
 * @param pfd Which PFD clock to enable.
 * @param pfdFrac The PFD FRAC value.
 * @note It is recommended that PFD settings are kept between 12-35.
 */
void CLOCK_InitSysPfd(clock_pfd_t pfd, uint8_t pfdFrac);

/*!
 * @brief De-initialize the System PLL PFD.
 *
 * This function disables the System PLL PFD.
 *
 * @param pfd Which PFD clock to disable.
 */
void CLOCK_DeinitSysPfd(clock_pfd_t pfd);

/*!
 * @brief Check if Sys PFD is enabled
 *
 * @param pfd PFD control name
 * @return PFD bypass status.
 *         - true: power on.
 *         - false: power off.
 */
bool CLOCK_IsSysPfdEnabled(clock_pfd_t pfd);

/*!
 * @brief Initialize the USB1 PLL PFD.
 *
 * This function initializes the USB1 PLL PFD. During new value setting,
 * the clock output is disabled to prevent glitch.
 *
 * @param pfd Which PFD clock to enable.
 * @param pfdFrac The PFD FRAC value.
 * @note It is recommended that PFD settings are kept between 12-35.
 */
void CLOCK_InitUsb1Pfd(clock_pfd_t pfd, uint8_t pfdFrac);

/*!
 * @brief De-initialize the USB1 PLL PFD.
 *
 * This function disables the USB1 PLL PFD.
 *
 * @param pfd Which PFD clock to disable.
 */
void CLOCK_DeinitUsb1Pfd(clock_pfd_t pfd);

/*!
 * @brief Check if Usb1 PFD is enabled
 *
 * @param pfd PFD control name.
 * @return PFD bypass status.
 *         - true: power on.
 *         - false: power off.
 */
bool CLOCK_IsUsb1PfdEnabled(clock_pfd_t pfd);

/*!
 * @brief Get current System PLL PFD output frequency.
 *
 * This function get current output frequency of specific System PLL PFD
 *
 * @param pfd   pfd name to get frequency.
 * @return The PFD output frequency in hertz.
 */
uint32_t CLOCK_GetSysPfdFreq(clock_pfd_t pfd);

/*!
 * @brief Get current USB1 PLL PFD output frequency.
 *
 * This function get current output frequency of specific USB1 PLL PFD
 *
 * @param pfd   pfd name to get frequency.
 * @return The PFD output frequency in hertz.
 */
uint32_t CLOCK_GetUsb1PfdFreq(clock_pfd_t pfd);

/*! @brief Enable USB HS PHY PLL clock.
 *
 * This function enables the internal 480MHz USB PHY PLL clock.
 *
 * @param src  USB HS PHY PLL clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0PhyPllClock(clock_usb_phy_src_t src, uint32_t freq);

/*! @brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhs0PhyPllClock(void);

/*! @brief Enable USB HS PHY PLL clock.
 *
 * This function enables the internal 480MHz USB PHY PLL clock.
 *
 * @param src  USB HS PHY PLL clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs1PhyPllClock(clock_usb_phy_src_t src, uint32_t freq);

/*! @brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhs1PhyPllClock(void);

/* @} */

/*!
 * @name Clock Output Inferfaces
 * @{
 */

/*!
 * @brief Set the clock source and the divider of the clock output1.
 *
 * @param selection The clock source to be output, please refer to @ref clock_output1_selection_t.
 * @param divider The divider of the output clock signal, please refer to @ref clock_output_divider_t.
 */
void CLOCK_SetClockOutput1(clock_output1_selection_t selection, clock_output_divider_t divider);

/*!
 * @brief Set the clock source and the divider of the clock output2.
 *
 * @param selection The clock source to be output, please refer to @ref clock_output2_selection_t.
 * @param divider The divider of the output clock signal, please refer to @ref clock_output_divider_t.
 */
void CLOCK_SetClockOutput2(clock_output2_selection_t selection, clock_output_divider_t divider);

/*!
 * @brief Get the frequency of clock output1 clock signal.
 *
 * @return The frequency of clock output1 clock signal.
 */
uint32_t CLOCK_GetClockOutCLKO1Freq(void);

/*!
 * @brief Get the frequency of clock output2 clock signal.
 *
 * @return The frequency of clock output2 clock signal.
 */
uint32_t CLOCK_GetClockOutClkO2Freq(void);

/*! @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_CLOCK_H_ */
