/*
 * Copyright 2020-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CLOCK_H_
#define _FSL_CLOCK_H_

#include "fsl_common.h"
#include "fsl_reset.h"

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
/*! @brief CLOCK driver version 2.3.0. */
#define FSL_CLOCK_DRIVER_VERSION (MAKE_VERSION(2, 3, 0))
/*@}*/

/* Definition for delay API in clock driver, users can redefine it to the real application. */
#ifndef SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY
#define SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY (260000000UL)
#endif

/*! @brief Clock ip name array for GPIO. */
#define GPIO_CLOCKS                    \
    {                                  \
        kCLOCK_HsGpio0, kCLOCK_HsGpio1 \
    }

/*! @brief Clock ip name array for CACHE64. */
#define CACHE64_CLOCKS                 \
    {                                  \
        kCLOCK_Flexspi, kCLOCK_Flexspi \
    }

/*! @brief Clock ip name array for FLEXSPI. */
#define FLEXSPI_CLOCKS \
    {                  \
        kCLOCK_Flexspi \
    }

/*! @brief Clock ip name array for FLEXCOMM. */
#define FLEXCOMM_CLOCKS                                                                           \
    {                                                                                             \
        kCLOCK_Flexcomm0, kCLOCK_Flexcomm1, kCLOCK_Flexcomm2, kCLOCK_Flexcomm3, kCLOCK_Flexcomm14 \
    }

/*! @brief Clock ip name array for LPUART. */
#define USART_CLOCKS                                                           \
    {                                                                          \
        kCLOCK_Flexcomm0, kCLOCK_Flexcomm1, kCLOCK_Flexcomm2, kCLOCK_Flexcomm3 \
    }

/*! @brief Clock ip name array for I2C. */
#define I2C_CLOCKS                                                             \
    {                                                                          \
        kCLOCK_Flexcomm0, kCLOCK_Flexcomm1, kCLOCK_Flexcomm2, kCLOCK_Flexcomm3 \
    }

/*! @brief Clock ip name array for SPI. */
#define SPI_CLOCKS                                                                                \
    {                                                                                             \
        kCLOCK_Flexcomm0, kCLOCK_Flexcomm1, kCLOCK_Flexcomm2, kCLOCK_Flexcomm3, kCLOCK_Flexcomm14 \
    }

/*! @brief Clock ip name array for ACOMP. */
#define ACOMP_CLOCKS \
    {                \
        kCLOCK_Gau   \
    }

/*! @brief Clock ip name array for ADC. */
#define ADC_CLOCKS             \
    {                          \
        kCLOCK_Gau, kCLOCK_Gau \
    }

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS \
    {              \
        kCLOCK_Gau \
    }

/*! @brief Clock ip name array for LCDIC. */
#define LCDIC_CLOCKS \
    {                \
        kCLOCK_Lcdic \
    }

/*! @brief Clock ip name array for DMA. */
#define DMA_CLOCKS               \
    {                            \
        kCLOCK_Dma0, kCLOCK_Dma1 \
    }

/*! @brief Clock ip name array for DMIC. */
#define DMIC_CLOCKS  \
    {                \
        kCLOCK_Dmic0 \
    }

/*! @brief Clock ip name array for ENET. */
#define ENET_CLOCKS    \
    {                  \
        kCLOCK_EnetIpg \
    }

/*! @brief Extra clock ip name array for ENET. */
#define ENET_EXTRA_CLOCKS \
    {                     \
        kCLOCK_EnetIpgS   \
    }

/*! @brief Clock ip name array for Powerquad */
#define POWERQUAD_CLOCKS \
    {                    \
        kCLOCK_PowerQuad \
    }

/*! @brief Clock ip name array for OSTimer */
#define OSTIMER_CLOCKS      \
    {                       \
        kCLOCK_OsEventTimer \
    }

/*! @brief Clock ip name array for CT32B. */
#define CTIMER_CLOCKS                                                             \
    {                                                                             \
        kCLOCK_Ct32b0, kCLOCK_Ct32b1, kCLOCK_Ct32b2, kCLOCK_Ct32b3, kCLOCK_Ct32b4 \
    }

/*! @brief Clock ip name array for UTICK. */
#define UTICK_CLOCKS \
    {                \
        kCLOCK_Utick \
    }

/*! @brief Clock ip name array for MRT. */
#define MRT_CLOCKS                 \
    {                              \
        kCLOCK_Mrt, kCLOCK_FreeMrt \
    }

/*! @brief Clock ip name array for SCT. */
#define SCT_CLOCKS \
    {              \
        kCLOCK_Sct \
    }

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS \
    {              \
        kCLOCK_Rtc \
    }

/*! @brief Clock ip name array for WWDT. */
#define WWDT_CLOCKS  \
    {                \
        kCLOCK_Wwdt0 \
    }

/*! @brief Clock ip name array for TRNG. */
#define TRNG_CLOCKS \
    {               \
        kCLOCK_Trng \
    }

/*! @brief Clock ip name array for USIM. */
#define USIM_CLOCKS \
    {               \
        kCLOCK_Usim \
    }

/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
/*------------------------------------------------------------------------------
 clock_ip_name_t definition:
------------------------------------------------------------------------------*/

#define CLK_GATE_REG_OFFSET_SHIFT 8U
#define CLK_GATE_REG_OFFSET_MASK  0xFF00U
#define CLK_GATE_BIT_SHIFT_SHIFT  0U
#define CLK_GATE_BIT_SHIFT_MASK   0x000000FFU

#define CLK_GATE_DEFINE(reg_offset, bit_shift)                                  \
    ((((reg_offset) << CLK_GATE_REG_OFFSET_SHIFT) & CLK_GATE_REG_OFFSET_MASK) | \
     (((bit_shift) << CLK_GATE_BIT_SHIFT_SHIFT) & CLK_GATE_BIT_SHIFT_MASK))

#define CLK_GATE_ABSTRACT_REG_OFFSET(x) (((uint32_t)(x)&CLK_GATE_REG_OFFSET_MASK) >> CLK_GATE_REG_OFFSET_SHIFT)
#define CLK_GATE_ABSTRACT_BITS_SHIFT(x) (((uint32_t)(x)&CLK_GATE_BIT_SHIFT_MASK) >> CLK_GATE_BIT_SHIFT_SHIFT)

#define CLK_CTL0_PSCCTL0 0
#define CLK_CTL0_PSCCTL1 1
#define CLK_CTL0_PSCCTL2 2
#define CLK_CTL1_PSCCTL0 3
#define CLK_CTL1_PSCCTL1 4
#define CLK_CTL1_PSCCTL2 5

#define SYS_CLK_GATE_FLAG_MASK         (0x10000UL)
#define SYS_CLK_GATE_DEFINE(bit_shift) (((bit_shift)&CLK_GATE_BIT_SHIFT_MASK) | SYS_CLK_GATE_FLAG_MASK)
#define SYS_CLK_GATE_BIT_MASK(x)       (1UL << (((uint32_t)(x)&CLK_GATE_BIT_SHIFT_MASK) >> CLK_GATE_BIT_SHIFT_SHIFT))

#define CLKCTL0_TUPLE_MUXA(reg, choice) (((reg)&0xFFCU) | ((choice) << 12U))
#define CLKCTL0_TUPLE_MUXB(reg, choice) ((((reg)&0xFFCU) << 16) | ((choice) << 28U))
#define CLKCTL1_TUPLE_FLAG_MASK         (0x8000U)
#define CLKCTL1_TUPLE_MUXA(reg, choice) (((reg)&0xFFCU) | CLKCTL1_TUPLE_FLAG_MASK | ((choice) << 12U))
#define CLKCTL1_TUPLE_MUXB(reg, choice) ((((reg)&0xFFCU) | CLKCTL1_TUPLE_FLAG_MASK | ((choice) << 28U)) << 16)
#define CLKCTL_TUPLE_REG(base, tuple)   ((volatile uint32_t *)(((uint32_t)(base)) + ((uint32_t)(tuple)&0xFFCU)))
#define CLKCTL_TUPLE_SEL(tuple)         (((uint32_t)(tuple) >> 12U) & 0x7U)

#define CLKOUT_TUPLE_MUX_AVAIL          (0x2U)
#define CLKOUT_TUPLE_MUX(ch0, ch1, ch2) (CLKOUT_TUPLE_MUX_AVAIL | ((ch0) << 4U) | ((ch1) << 8) | ((ch2) << 12))

#define PMU_TUPLE_MUX_AVAIL        (0x1U)
#define PMU_TUPLE_MUX(reg, choice) (((reg)&0xFFCU) | ((choice) << 12U) | PMU_TUPLE_MUX_AVAIL)
#define PMU_TUPLE_REG(base, tuple) ((volatile uint32_t *)(((uint32_t)(base)) + ((uint32_t)(tuple)&0xFFCU)))
#define PMU_TUPLE_SEL(tuple)       (((uint32_t)(tuple) >> 12U) & 0x3U)

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    kCLOCK_CoreSysClk,       /*!< Core clock  (aka HCLK)                                 */
    kCLOCK_BusClk,           /*!< Bus clock (AHB/APB clock, aka HCLK)                    */
    kCLOCK_MclkClk,          /*!< MCLK, to MCLK pin                                      */
} clock_name_t;

/*!
 * @brief Peripheral clock name difinition used for
 * clock gate.
 */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,

    kCLOCK_TcpuMciClk         = SYS_CLK_GATE_DEFINE(0),
    kCLOCK_TcpuMciFlexspiClk  = SYS_CLK_GATE_DEFINE(1),
    kCLOCK_TddrMciEnetClk     = SYS_CLK_GATE_DEFINE(2),
    kCLOCK_TddrMciFlexspiClk  = SYS_CLK_GATE_DEFINE(3),
    kCLOCK_T3PllMciIrcClk     = SYS_CLK_GATE_DEFINE(4),
    kCLOCK_T3PllMci256mClk    = SYS_CLK_GATE_DEFINE(5),
    kCLOCK_T3PllMci213mClk    = SYS_CLK_GATE_DEFINE(6),
    kCLOCK_T3PllMciFlexspiClk = SYS_CLK_GATE_DEFINE(7),
    kCLOCK_RefClkSys          = SYS_CLK_GATE_DEFINE(9),
    kCLOCK_RefClkTcpu         = SYS_CLK_GATE_DEFINE(28),
    kCLOCK_RefClkTddr         = SYS_CLK_GATE_DEFINE(29),
    kCLOCK_RefClkAud          = SYS_CLK_GATE_DEFINE(30),
    kCLOCK_RefClkUsb          = SYS_CLK_GATE_DEFINE(31),
    kCLOCK_RefClkCauSlp       = SYS_CLK_GATE_DEFINE(32),

    kCLOCK_Cpu       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 0),
    kCLOCK_Matrix    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 1),
    kCLOCK_Romcp     = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 2),
    kCLOCK_PowerQuad = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 8),
    kCLOCK_Pkc       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 9),
    kCLOCK_Els       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 10),
    kCLOCK_Puf       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 11),
    kCLOCK_Flexspi   = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 16),
    kCLOCK_Hpu       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 20),
    kCLOCK_Usb       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 22),
    kCLOCK_Sct       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 24),
    kCLOCK_AonMem    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 25),
    kCLOCK_Gdma      = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 28),
    kCLOCK_Dma0      = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 29),
    kCLOCK_Dma1      = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 30),
    kCLOCK_Sdio      = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL0, 31),

    kCLOCK_ElsApb     = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 0),
    kCLOCK_SdioSlv    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 2),
    kCLOCK_Gau        = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 16),
    kCLOCK_Otp        = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 17),
    kCLOCK_SecureGpio = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 24),
    kCLOCK_EnetIpg    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 25),
    kCLOCK_EnetIpgS   = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 26),
    kCLOCK_Trng       = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL1, 27),

    kCLOCK_Utick   = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 0),
    kCLOCK_Wwdt0   = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 1),
    kCLOCK_Usim    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 2),
    kCLOCK_Itrc    = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 3),
    kCLOCK_FreeMrt = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 26),
    kCLOCK_Lcdic   = CLK_GATE_DEFINE(CLK_CTL0_PSCCTL2, 27),

    kCLOCK_Flexcomm0    = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 8),
    kCLOCK_Flexcomm1    = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 9),
    kCLOCK_Flexcomm2    = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 10),
    kCLOCK_Flexcomm3    = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 11),
    kCLOCK_Flexcomm14   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 22),
    kCLOCK_Dmic0        = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 24),
    kCLOCK_OsEventTimer = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL0, 27),

    kCLOCK_HsGpio0 = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL1, 0),
    kCLOCK_HsGpio1 = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL1, 1),
    kCLOCK_Crc     = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL1, 16),
    kCLOCK_Freqme  = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL1, 31),

    kCLOCK_Ct32b0   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 0),
    kCLOCK_Ct32b1   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 1),
    kCLOCK_Ct32b2   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 2),
    kCLOCK_Ct32b3   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 3),
    kCLOCK_Ct32b4   = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 4),
    kCLOCK_Pmu      = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 6),
    kCLOCK_Rtc      = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 7),
    kCLOCK_Mrt      = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 8),
    kCLOCK_Pint     = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 30),
    kCLOCK_InputMux = CLK_GATE_DEFINE(CLK_CTL1_PSCCTL2, 31),
} clock_ip_name_t;

/*!
 * @brief Peripheral clock source selection definition.
 */
typedef enum _clock_attach_id
{
    kXTAL_to_SYSOSC_CLK  = CLKCTL0_TUPLE_MUXA(0x168U, 0),
    kCLKIN_to_SYSOSC_CLK = CLKCTL0_TUPLE_MUXA(0x168U, 1),
    kNONE_to_SYSOSC_CLK  = CLKCTL0_TUPLE_MUXA(0x168U, 7),

    kSYSOSC_to_MAIN_CLK    = CLKCTL0_TUPLE_MUXA(0x430U, 0) | CLKCTL0_TUPLE_MUXB(0x434U, 0),
    kFFRO_DIV4_to_MAIN_CLK = CLKCTL0_TUPLE_MUXA(0x430U, 1) | CLKCTL0_TUPLE_MUXB(0x434U, 0),
    kLPOSC_to_MAIN_CLK     = CLKCTL0_TUPLE_MUXA(0x430U, 2) | CLKCTL0_TUPLE_MUXB(0x434U, 0),
    kFFRO_to_MAIN_CLK      = CLKCTL0_TUPLE_MUXA(0x430U, 3) | CLKCTL0_TUPLE_MUXB(0x434U, 0),
    kSFRO_to_MAIN_CLK      = CLKCTL0_TUPLE_MUXA(0x434U, 1),
    kMAIN_PLL_to_MAIN_CLK  = CLKCTL0_TUPLE_MUXA(0x434U, 2),
    kCLK32K_to_MAIN_CLK    = CLKCTL0_TUPLE_MUXA(0x434U, 3),

    kMAIN_CLK_to_FLEXSPI_CLK          = CLKCTL0_TUPLE_MUXA(0x620U, 0),
    kT3PLL_MCI_FLEXSPI_to_FLEXSPI_CLK = CLKCTL0_TUPLE_MUXA(0x620U, 1),
    kAUX0_PLL_to_FLEXSPI_CLK          = CLKCTL0_TUPLE_MUXA(0x620U, 2),
    kTCPU_MCI_FLEXSPI_to_FLEXSPI_CLK  = CLKCTL0_TUPLE_MUXA(0x620U, 3),
    kAUX1_PLL_to_FLEXSPI_CLK          = CLKCTL0_TUPLE_MUXA(0x620U, 4),
    kTDDR_MCI_FLEXSPI_to_FLEXSPI_CLK  = CLKCTL0_TUPLE_MUXA(0x620U, 5),
    kT3PLL_MCI_256M_to_FLEXSPI_CLK    = CLKCTL0_TUPLE_MUXA(0x620U, 6),
    kNONE_to_FLEXSPI_CLK              = CLKCTL0_TUPLE_MUXA(0x620U, 7),

    kMAIN_CLK_to_SCT_CLK  = CLKCTL0_TUPLE_MUXA(0x640U, 0),
    kMAIN_PLL_to_SCT_CLK  = CLKCTL0_TUPLE_MUXA(0x640U, 1),
    kAUX0_PLL_to_SCT_CLK  = CLKCTL0_TUPLE_MUXA(0x640U, 2),
    kFFRO_to_SCT_CLK      = CLKCTL0_TUPLE_MUXA(0x640U, 3),
    kAUX1_PLL_to_SCT_CLK  = CLKCTL0_TUPLE_MUXA(0x640U, 4),
    kAUDIO_PLL_to_SCT_CLK = CLKCTL0_TUPLE_MUXA(0x640U, 5),
    kNONE_to_SCT_CLK      = CLKCTL0_TUPLE_MUXA(0x640U, 7),

    kLPOSC_to_UTICK_CLK    = CLKCTL0_TUPLE_MUXA(0x700U, 0),
    kMAIN_CLK_to_UTICK_CLK = CLKCTL0_TUPLE_MUXA(0x700U, 1),
    kNONE_to_UTICK_CLK     = CLKCTL0_TUPLE_MUXA(0x700U, 3),

    kLPOSC_to_WDT0_CLK    = CLKCTL0_TUPLE_MUXA(0x720U, 0),
    kMAIN_CLK_to_WDT0_CLK = CLKCTL0_TUPLE_MUXA(0x720U, 1),
    kNONE_to_WDT0_CLK     = CLKCTL0_TUPLE_MUXA(0x720U, 3),

    kSYSTICK_DIV_to_SYSTICK_CLK = CLKCTL0_TUPLE_MUXA(0x760U, 0),
    kLPOSC_to_SYSTICK_CLK       = CLKCTL0_TUPLE_MUXA(0x760U, 1),
    kCLK32K_to_SYSTICK_CLK      = CLKCTL0_TUPLE_MUXA(0x760U, 2),
    kSFRO_to_SYSTICK_CLK        = CLKCTL0_TUPLE_MUXA(0x760U, 3),
    kNONE_to_SYSTICK_CLK        = CLKCTL0_TUPLE_MUXA(0x760U, 7),

    kMAIN_CLK_to_USIM_CLK  = CLKCTL0_TUPLE_MUXA(0x774U, 0),
    kAUDIO_PLL_to_USIM_CLK = CLKCTL0_TUPLE_MUXA(0x774U, 1),
    kFFRO_to_USIM_CLK      = CLKCTL0_TUPLE_MUXA(0x774U, 2),
    kNONE_to_USIM_CLK      = CLKCTL0_TUPLE_MUXA(0x774U, 3),

    kMAIN_CLK_to_LCD_CLK          = CLKCTL0_TUPLE_MUXA(0x778U, 0),
    kT3PLL_MCI_FLEXSPI_to_LCD_CLK = CLKCTL0_TUPLE_MUXA(0x778U, 1),
    kTCPU_MCI_FLEXSPI_to_LCD_CLK  = CLKCTL0_TUPLE_MUXA(0x778U, 2),
    kTDDR_MCI_FLEXSPI_to_LCD_CLK  = CLKCTL0_TUPLE_MUXA(0x778U, 3),
    kNONE_to_LCD_CLK              = CLKCTL0_TUPLE_MUXA(0x778U, 7),

    kMAIN_CLK_to_GAU_CLK       = CLKCTL0_TUPLE_MUXA(0x77CU, 0),
    kT3PLL_MCI_256M_to_GAU_CLK = CLKCTL0_TUPLE_MUXA(0x77CU, 1),
    kAVPLL_CH2_to_GAU_CLK      = CLKCTL0_TUPLE_MUXA(0x77CU, 2),
    kNONE_to_GAU_CLK           = CLKCTL0_TUPLE_MUXA(0x77CU, 3),

    kT3PLL_MCI_256M_to_ELS_GDET = CLKCTL0_TUPLE_MUXA(0x7A8U, 0),
    kELS_128M_to_ELS_GDET       = CLKCTL0_TUPLE_MUXA(0x7A8U, 1),
    kELS_64M_to_ELS_GDET        = CLKCTL0_TUPLE_MUXA(0x7A8U, 2),
    kOTP_FUSE_32M_to_ELS_GDET   = CLKCTL0_TUPLE_MUXA(0x7A8U, 3),
    kNONE_to_ELS_GDET           = CLKCTL0_TUPLE_MUXA(0x7A8U, 7),

    kLPOSC_to_OSTIMER_CLK    = CLKCTL1_TUPLE_MUXA(0x480U, 0),
    kCLK32K_to_OSTIMER_CLK   = CLKCTL1_TUPLE_MUXA(0x480U, 1),
    kHCLK_to_OSTIMER_CLK     = CLKCTL1_TUPLE_MUXA(0x480U, 2),
    kMAIN_CLK_to_OSTIMER_CLK = CLKCTL1_TUPLE_MUXA(0x480U, 3),
    kNONE_to_OSTIMER_CLK     = CLKCTL1_TUPLE_MUXA(0x480U, 7),

    kSFRO_to_FLEXCOMM0      = CLKCTL1_TUPLE_MUXA(0x508U, 0),
    kFFRO_to_FLEXCOMM0      = CLKCTL1_TUPLE_MUXA(0x508U, 1),
    kAUDIO_PLL_to_FLEXCOMM0 = CLKCTL1_TUPLE_MUXA(0x508U, 2),
    kMCLK_IN_to_FLEXCOMM0   = CLKCTL1_TUPLE_MUXA(0x508U, 3),
    kFRG_to_FLEXCOMM0       = CLKCTL1_TUPLE_MUXA(0x508U, 4),
    kNONE_to_FLEXCOMM0      = CLKCTL1_TUPLE_MUXA(0x508U, 7),

    kSFRO_to_FLEXCOMM1      = CLKCTL1_TUPLE_MUXA(0x528U, 0),
    kFFRO_to_FLEXCOMM1      = CLKCTL1_TUPLE_MUXA(0x528U, 1),
    kAUDIO_PLL_to_FLEXCOMM1 = CLKCTL1_TUPLE_MUXA(0x528U, 2),
    kMCLK_IN_to_FLEXCOMM1   = CLKCTL1_TUPLE_MUXA(0x528U, 3),
    kFRG_to_FLEXCOMM1       = CLKCTL1_TUPLE_MUXA(0x528U, 4),
    kNONE_to_FLEXCOMM1      = CLKCTL1_TUPLE_MUXA(0x528U, 7),

    kSFRO_to_FLEXCOMM2      = CLKCTL1_TUPLE_MUXA(0x548U, 0),
    kFFRO_to_FLEXCOMM2      = CLKCTL1_TUPLE_MUXA(0x548U, 1),
    kAUDIO_PLL_to_FLEXCOMM2 = CLKCTL1_TUPLE_MUXA(0x548U, 2),
    kMCLK_IN_to_FLEXCOMM2   = CLKCTL1_TUPLE_MUXA(0x548U, 3),
    kFRG_to_FLEXCOMM2       = CLKCTL1_TUPLE_MUXA(0x548U, 4),
    kNONE_to_FLEXCOMM2      = CLKCTL1_TUPLE_MUXA(0x548U, 7),

    kSFRO_to_FLEXCOMM3      = CLKCTL1_TUPLE_MUXA(0x568U, 0),
    kFFRO_to_FLEXCOMM3      = CLKCTL1_TUPLE_MUXA(0x568U, 1),
    kAUDIO_PLL_to_FLEXCOMM3 = CLKCTL1_TUPLE_MUXA(0x568U, 2),
    kMCLK_IN_to_FLEXCOMM3   = CLKCTL1_TUPLE_MUXA(0x568U, 3),
    kFRG_to_FLEXCOMM3       = CLKCTL1_TUPLE_MUXA(0x568U, 4),
    kNONE_to_FLEXCOMM3      = CLKCTL1_TUPLE_MUXA(0x568U, 7),

    kSFRO_to_FLEXCOMM14      = CLKCTL1_TUPLE_MUXA(0x6C8U, 0),
    kFFRO_to_FLEXCOMM14      = CLKCTL1_TUPLE_MUXA(0x6C8U, 1),
    kAUDIO_PLL_to_FLEXCOMM14 = CLKCTL1_TUPLE_MUXA(0x6C8U, 2),
    kMCLK_IN_to_FLEXCOMM14   = CLKCTL1_TUPLE_MUXA(0x6C8U, 3),
    kFRG_to_FLEXCOMM14       = CLKCTL1_TUPLE_MUXA(0x6C8U, 4),
    kNONE_to_FLEXCOMM14      = CLKCTL1_TUPLE_MUXA(0x6C8U, 7),

    kSFRO_to_DMIC_CLK      = CLKCTL1_TUPLE_MUXA(0x700U, 0),
    kFFRO_to_DMIC_CLK      = CLKCTL1_TUPLE_MUXA(0x700U, 1),
    kAUDIO_PLL_to_DMIC_CLK = CLKCTL1_TUPLE_MUXA(0x700U, 2),
    kMCLK_IN_to_DMIC_CLK   = CLKCTL1_TUPLE_MUXA(0x700U, 3),
    kLPOSC_to_DMIC_CLK     = CLKCTL1_TUPLE_MUXA(0x700U, 4),
    kCLK32K_to_DMIC_CLK    = CLKCTL1_TUPLE_MUXA(0x700U, 5),
    kMAIN_CLK_to_DMIC_CLK  = CLKCTL1_TUPLE_MUXA(0x700U, 6),
    kNONE_to_DMIC_CLK      = CLKCTL1_TUPLE_MUXA(0x700U, 7),

    kMAIN_CLK_to_CTIMER0  = CLKCTL1_TUPLE_MUXA(0x720U, 0),
    kSFRO_to_CTIMER0      = CLKCTL1_TUPLE_MUXA(0x720U, 1),
    kFFRO_to_CTIMER0      = CLKCTL1_TUPLE_MUXA(0x720U, 2),
    kAUDIO_PLL_to_CTIMER0 = CLKCTL1_TUPLE_MUXA(0x720U, 3),
    kMCLK_IN_to_CTIMER0   = CLKCTL1_TUPLE_MUXA(0x720U, 4),
    kLPOSC_to_CTIMER0     = CLKCTL1_TUPLE_MUXA(0x720U, 5),
    kNONE_to_CTIMER0      = CLKCTL1_TUPLE_MUXA(0x720U, 7),

    kMAIN_CLK_to_CTIMER1  = CLKCTL1_TUPLE_MUXA(0x724U, 0),
    kSFRO_to_CTIMER1      = CLKCTL1_TUPLE_MUXA(0x724U, 1),
    kFFRO_to_CTIMER1      = CLKCTL1_TUPLE_MUXA(0x724U, 2),
    kAUDIO_PLL_to_CTIMER1 = CLKCTL1_TUPLE_MUXA(0x724U, 3),
    kMCLK_IN_to_CTIMER1   = CLKCTL1_TUPLE_MUXA(0x724U, 4),
    kLPOSC_to_CTIMER1     = CLKCTL1_TUPLE_MUXA(0x724U, 5),
    kNONE_to_CTIMER1      = CLKCTL1_TUPLE_MUXA(0x724U, 7),

    kMAIN_CLK_to_CTIMER2  = CLKCTL1_TUPLE_MUXA(0x728U, 0),
    kSFRO_to_CTIMER2      = CLKCTL1_TUPLE_MUXA(0x728U, 1),
    kFFRO_to_CTIMER2      = CLKCTL1_TUPLE_MUXA(0x728U, 2),
    kAUDIO_PLL_to_CTIMER2 = CLKCTL1_TUPLE_MUXA(0x728U, 3),
    kMCLK_IN_to_CTIMER2   = CLKCTL1_TUPLE_MUXA(0x728U, 4),
    kLPOSC_to_CTIMER2     = CLKCTL1_TUPLE_MUXA(0x728U, 5),
    kNONE_to_CTIMER2      = CLKCTL1_TUPLE_MUXA(0x728U, 7),

    kMAIN_CLK_to_CTIMER3  = CLKCTL1_TUPLE_MUXA(0x72CU, 0),
    kSFRO_to_CTIMER3      = CLKCTL1_TUPLE_MUXA(0x72CU, 1),
    kFFRO_to_CTIMER3      = CLKCTL1_TUPLE_MUXA(0x72CU, 2),
    kAUDIO_PLL_to_CTIMER3 = CLKCTL1_TUPLE_MUXA(0x72CU, 3),
    kMCLK_IN_to_CTIMER3   = CLKCTL1_TUPLE_MUXA(0x72CU, 4),
    kLPOSC_to_CTIMER3     = CLKCTL1_TUPLE_MUXA(0x72CU, 5),
    kNONE_to_CTIMER3      = CLKCTL1_TUPLE_MUXA(0x72CU, 7),

    kFFRO_to_MCLK_CLK      = CLKCTL1_TUPLE_MUXA(0x740U, 0),
    kAUDIO_PLL_to_MCLK_CLK = CLKCTL1_TUPLE_MUXA(0x740U, 1),
    kMAIN_CLK_to_MCLK_CLK  = CLKCTL1_TUPLE_MUXA(0x740U, 2),
    kNONE_to_MCLK_CLK      = CLKCTL1_TUPLE_MUXA(0x740U, 3),

    kSFRO_to_CLKOUT              = CLKOUT_TUPLE_MUX(0U, 0U, 0U),
    kSYSOSC_to_CLKOUT            = CLKOUT_TUPLE_MUX(1U, 0U, 0U),
    kLPOSC_to_CLKOUT             = CLKOUT_TUPLE_MUX(2U, 0U, 0U),
    kFFRO_to_CLKOUT              = CLKOUT_TUPLE_MUX(3U, 0U, 0U),
    kMAIN_CLK_to_CLKOUT          = CLKOUT_TUPLE_MUX(4U, 0U, 0U),
    kREFCLK_SYS_to_CLKOUT        = CLKOUT_TUPLE_MUX(5U, 0U, 0U),
    kAVPLL_CH2_to_CLKOUT         = CLKOUT_TUPLE_MUX(6U, 0U, 0U),
    kMAIN_PLL_to_CLKOUT          = CLKOUT_TUPLE_MUX(7U, 1U, 0U),
    kAUX0_PLL_to_CLKOUT          = CLKOUT_TUPLE_MUX(7U, 2U, 0U),
    kAUX1_PLL_to_CLKOUT          = CLKOUT_TUPLE_MUX(7U, 4U, 0U),
    kAUDIO_PLL_to_CLKOUT         = CLKOUT_TUPLE_MUX(7U, 5U, 0U),
    kCLK32K_to_CLKOUT            = CLKOUT_TUPLE_MUX(7U, 6U, 0U),
    kTCPU_MCI_FLEXSPI_to_CLKOUT  = CLKOUT_TUPLE_MUX(7U, 7U, 1U),
    kTDDR_MCI_FLEXSPI_to_CLKOUT  = CLKOUT_TUPLE_MUX(7U, 7U, 2U),
    kT3PLL_MCI_FLEXSPI_to_CLKOUT = CLKOUT_TUPLE_MUX(7U, 7U, 3U),
    kT3PLL_MCI_256M_to_CLKOUT    = CLKOUT_TUPLE_MUX(7U, 7U, 4U),
    kCAU_SLP_REF_CLK_to_CLKOUT   = CLKOUT_TUPLE_MUX(7U, 7U, 5U),
    kTDDR_MCI_ENET_to_CLKOUT     = CLKOUT_TUPLE_MUX(7U, 7U, 6U),
    kNONE_to_CLKOUT              = CLKOUT_TUPLE_MUX(7U, 7U, 7U),

    kRC32K_to_CLK32K   = PMU_TUPLE_MUX(0x70U, 0),
    kXTAL32K_to_CLK32K = PMU_TUPLE_MUX(0x70U, 1),
    kNCO32K_to_CLK32K  = PMU_TUPLE_MUX(0x70U, 2),
} clock_attach_id_t;

/*!
 * @brief Clock divider definition.
 */
typedef enum _clock_div_name
{
    kCLOCK_DivMainPllClk   = CLKCTL0_TUPLE_MUXA(0x240U, 0),
    kCLOCK_DivAux0PllClk   = CLKCTL0_TUPLE_MUXA(0x248U, 0),
    kCLOCK_DivAux1PllClk   = CLKCTL0_TUPLE_MUXA(0x24CU, 0),
    kCLOCK_DivSysCpuAhbClk = CLKCTL0_TUPLE_MUXA(0x400U, 0),
    kCLOCK_DivPfc1Clk      = CLKCTL0_TUPLE_MUXA(0x504U, 0),
    kCLOCK_DivFlexspiClk   = CLKCTL0_TUPLE_MUXA(0x624U, 0),
    kCLOCK_DivSctClk       = CLKCTL0_TUPLE_MUXA(0x644U, 0),
    kCLOCK_DivUsbHsFclk    = CLKCTL0_TUPLE_MUXA(0x664U, 0),
    kCLOCK_DivSystickClk   = CLKCTL0_TUPLE_MUXA(0x764U, 0),
    kCLOCK_DivLcdClk       = CLKCTL0_TUPLE_MUXA(0x768U, 0),
    kCLOCK_DivGauClk       = CLKCTL0_TUPLE_MUXA(0x76CU, 0),
    kCLOCK_DivUsimClk      = CLKCTL0_TUPLE_MUXA(0x770U, 0),
    kCLOCK_DivPmuFclk      = CLKCTL0_TUPLE_MUXA(0x780U, 0),

    kCLOCK_DivAudioPllClk = CLKCTL1_TUPLE_MUXA(0x240U, 0),
    kCLOCK_DivPllFrgClk   = CLKCTL1_TUPLE_MUXA(0x6FCU, 0),
    kCLOCK_DivDmicClk     = CLKCTL1_TUPLE_MUXA(0x704U, 0),
    kCLOCK_DivMclkClk     = CLKCTL1_TUPLE_MUXA(0x744U, 0),
    kCLOCK_DivClockOut    = CLKCTL1_TUPLE_MUXA(0x768U, 0),
} clock_div_name_t;

/*! @brief PLL configuration for FRG */
typedef struct _clock_frg_clk_config
{
    uint8_t num; /*!< FRG clock */
    enum
    {
        kCLOCK_FrgMainClk = 0, /*!< Main System clock */
        kCLOCK_FrgPllDiv,      /*!< Main pll clock divider*/
        kCLOCK_FrgSFro,        /*!< 16MHz FRO */
        kCLOCK_FrgFFro,        /*!< FRO48/60 */
    } sfg_clock_src;
    uint8_t divider; /*!< Denominator of the fractional divider. */
    uint8_t mult;    /*!< Numerator of the fractional divider. */
} clock_frg_clk_config_t;

/*! @brief TCPU PLL divider for tcpu_mci_flexspi_clk */
typedef enum
{
    kCLOCK_TcpuFlexspiDiv12 = 0, /*!< Divided by 12 */
    kCLOCK_TcpuFlexspiDiv11,     /*!< Divided by 11 */
    kCLOCK_TcpuFlexspiDiv10,     /*!< Divided by 10 */
    kCLOCK_TcpuFlexspiDiv9,      /*!< Divided by 9  */
} clock_tcpu_flexspi_div_t;

/*! @brief TDDR PLL divider for tddr_mci_flexspi_clk */
typedef enum
{
    kCLOCK_TddrFlexspiDiv11 = 0, /*!< Divided by 11 */
    kCLOCK_TddrFlexspiDiv10,     /*!< Divided by 10 */
    kCLOCK_TddrFlexspiDiv9,      /*!< Divided by 9 */
    kCLOCK_TddrFlexspiDiv8,      /*!< Divided by 8  */
} clock_tddr_flexspi_div_t;

/*! @brief T3 PLL IRC configuration */
typedef enum
{
    kCLOCK_T3MciIrc60m = 0, /*!< T3 MCI IRC 59.53MHz */
    kCLOCK_T3MciIrc48m,     /*!< T3 MCI IRC 48.30MHz */
} clock_t3_mci_irc_config_t;

/*! @brief AVPLL channel1 frequency configuration */
typedef enum
{
    kCLOCK_AvPllChUnchanged = 0, /*!< AVPLL channel frequency unchanged. */
    kCLOCK_AvPllChFreq2p048m,    /*!< AVPLL channel frequency 2.048MHz */
    kCLOCK_AvPllChFreq4p096m,    /*!< AVPLL channel frequency 4.096MHz */
    kCLOCK_AvPllChFreq6p144m,    /*!< AVPLL channel frequency 6.144MHz */
    kCLOCK_AvPllChFreq8p192m,    /*!< AVPLL channel frequency 8.192MHz */
    kCLOCK_AvPllChFreq11p2896m,  /*!< AVPLL channel frequency 11.2896MHz */
    kCLOCK_AvPllChFreq12m,       /*!< AVPLL channel frequency 12MHz */
    kCLOCK_AvPllChFreq12p288m,   /*!< AVPLL channel frequency 12.288MHz */
    kCLOCK_AvPllChFreq24p576m,   /*!< AVPLL channel frequency 24.576MHz */
    kCLOCK_AvPllChFreq64m,       /*!< AVPLL channel frequency 64MHz */
    kCLOCK_AvPllChFreq98p304m,   /*!< AVPLL channel frequency 98.304MHz */
} clock_avpll_ch_freq_t;

/*! @brief AVPLL configuration */
typedef struct
{
    clock_avpll_ch_freq_t ch1Freq; /*!< AVPLL channel 1 frequency configuration */
    clock_avpll_ch_freq_t ch2Freq; /*!< AVPLL channel 2 frequency configuration */
    bool enableCali;               /*!< Enable calibration */
} clock_avpll_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*! @brief External CLK_IN pin clock frequency (clkin) clock frequency.
 *
 * The CLK_IN pin (clkin) clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetClkinFreq to set the value in to clock driver. For example,
 * if CLK_IN is 16MHz,
 * @code
 * CLOCK_SetClkinFreq(16000000);
 * @endcode
 */
extern volatile uint32_t g_clkinFreq;
/*! @brief External MCLK IN clock frequency.
 *
 * The MCLK in (mclk_in) PIN clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetMclkInFreq to set the value in to clock driver. For example,
 * if mclk_In is 16MHz,
 * @code
 * CLOCK_SetMclkInFreq(16000000);
 * @endcode
 */
extern volatile uint32_t g_mclkinFreq;

/*! @brief  Return Frequency of t3pll_mci_48_60m_irc
 *  @return Frequency of t3pll_mci_48_60m_irc
 */
uint32_t CLOCK_GetT3PllMciIrcClkFreq(void);

/*! @brief  Return Frequency of t3pll_mci_213p3m
 *  @return Frequency of t3pll_mci_213p3m
 */
uint32_t CLOCK_GetT3PllMci213mClkFreq(void);

/*! @brief  Return Frequency of t3pll_mci_256m
 *  @return Frequency of t3pll_mci_256m
 */
uint32_t CLOCK_GetT3PllMci256mClkFreq(void);

/*! @brief  Return Frequency of t3pll_mci_flexspi_clk
 *  @return Frequency of t3pll_mci_flexspi_clk
 */
uint32_t CLOCK_GetT3PllMciFlexspiClkFreq(void);

/*! @brief  Return Frequency of tcpu_mci_clk
 *  @return Frequency of tcpu_mci_clk
 */
uint32_t CLOCK_GetTcpuMciClkFreq(void);

/*! @brief  Return Frequency of tcpu_mci_flexspi_clk
 *  @return Frequency of tcpu_mci_flexspi_clk
 */
uint32_t CLOCK_GetTcpuMciFlexspiClkFreq(void);

/*! @brief  Return Frequency of tddr_mci_flexspi_clk
 *  @return Frequency of tddr_mci_flexspi_clk
 */
uint32_t CLOCK_GetTddrMciFlexspiClkFreq(void);

/*! @brief  Return Frequency of tddr_mci_enet_clk
 *  @return Frequency of tddr_mci_enet_clk
 */
uint32_t CLOCK_GetTddrMciEnetClkFreq(void);

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param clk  Which clock to enable, see @ref clock_ip_name_t.
 */
void CLOCK_EnableClock(clock_ip_name_t clk);

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param clk  Which clock to disable, see @ref clock_ip_name_t.
 */
void CLOCK_DisableClock(clock_ip_name_t clk);

/**
 * @brief   Configure the clock selection muxes.
 * @param   connection  : Clock to be configured.
 */
void CLOCK_AttachClk(clock_attach_id_t connection);

/**
 * @brief   Setup clock dividers.
 * @param   name        : Clock divider name
 * @param   divider     : Value to be divided.
 */
void CLOCK_SetClkDiv(clock_div_name_t name, uint32_t divider);

/*! @brief  Return Frequency of selected clock
 *  @return Frequency of selected clock
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName);

/*! @brief  Return Input frequency for the Fractional baud rate generator
 *  @return Input Frequency for FRG
 */
uint32_t CLOCK_GetFRGClock(uint32_t id);

/*! @brief  Set output of the Fractional baud rate generator
 *  @param  config    : Configuration to set to FRGn clock.
 */
void CLOCK_SetFRGClock(const clock_frg_clk_config_t *config);

/*! @brief  Return Frequency of FFRO
 *  @return Frequency of FFRO
 */
uint32_t CLOCK_GetFFroFreq(void);

/*! @brief  Return Frequency of SFRO
 *  @return Frequency of SFRO
 */
uint32_t CLOCK_GetSFroFreq(void);

/*! @brief  Return Frequency of AUDIO PLL (AVPLL CH1)
 *  @return Frequency of AUDIO PLL
 */
uint32_t CLOCK_GetAvPllCh1Freq(void);

/*! @brief  Return Frequency of AVPLL CH2
 *  @return Frequency of AVPLL CH2
 */
uint32_t CLOCK_GetAvPllCh2Freq(void);

/*! @brief  Return Frequency of main clk
 *  @return Frequency of main clk
 */
uint32_t CLOCK_GetMainClkFreq(void);

/*! @brief  Return Frequency of core/bus clk
 *  @return Frequency of core/bus clk
 */
uint32_t CLOCK_GetCoreSysClkFreq(void);

/*! @brief  Return Frequency of systick clk
 *  @return Frequency of systick clk
 */
uint32_t CLOCK_GetSystickClkFreq(void);

/*! @brief  Return Frequency of sys osc Clock
 *  @return Frequency of sys osc Clock. Or CLK_IN pin frequency.
 */
static inline uint32_t CLOCK_GetSysOscFreq(void)
{
    return (CLKCTL0->SYSOSCBYPASS == 0U) ? CLK_XTAL_OSC_CLK : ((CLKCTL0->SYSOSCBYPASS == 1U) ? g_clkinFreq : 0U);
}

/*! @brief  Return Frequency of MCLK Input Clock
 *  @return Frequency of MCLK input Clock.
 */
static inline uint32_t CLOCK_GetMclkInClkFreq(void)
{
    return g_mclkinFreq;
}

/*! @brief  Return Frequency of LPOSC
 *  @return Frequency of LPOSC
 */
static inline uint32_t CLOCK_GetLpOscFreq(void)
{
    return CLK_XTAL_OSC_CLK / 40U;
}

/*! @brief  Return Frequency of CLK_32K
 *  @return Frequency of 32KHz osc
 */
static inline uint32_t CLOCK_GetClk32KFreq(void)
{
    return CLK_RTC_32K_CLK;
}

/*! @brief  Enables and disables 32KHz XTAL
 *  @param  enable : true to enable 32k XTAL clock, false to disable clock
 */
void CLOCK_EnableXtal32K(bool enable);

/*! @brief  Enables and disables RTC 32KHz
 *  @param  enable : true to enable 32k RTC clock, false to disable clock
 */
void CLOCK_EnableRtc32K(bool enable);

/*!
 * @brief Set the CLKIN (CLKIN pin) frequency based on GPIO4 input.
 *
 * @param freq : The CLK_IN pin input clock frequency in Hz.
 */
static inline void CLOCK_SetClkinFreq(uint32_t freq)
{
    g_clkinFreq = freq;
}
/*!
 * @brief Set the MCLK in (mclk_in) clock frequency based on board setting.
 *
 * @param freq : The MCLK input clock frequency in Hz.
 */
static inline void CLOCK_SetMclkinFreq(uint32_t freq)
{
    g_mclkinFreq = freq;
}

/*! @brief  Return Frequency of DMIC clk
 *  @return Frequency of DMIC clk
 */
uint32_t CLOCK_GetDmicClkFreq(void);

/*! @brief  Return Frequency of LCD clk
 *  @return Frequency of LCD clk
 */
uint32_t CLOCK_GetLcdClkFreq(void);

/*! @brief  Return Frequency of WDT clk
 *  @return Frequency of WDT clk
 */
uint32_t CLOCK_GetWdtClkFreq(void);

/*! @brief  Return Frequency of mclk
 *  @return Frequency of mclk clk
 */
uint32_t CLOCK_GetMclkClkFreq(void);

/*! @brief  Return Frequency of sct
 *  @return Frequency of sct clk
 */
uint32_t CLOCK_GetSctClkFreq(void);

/*! @brief  Return Frequency of Flexcomm functional Clock
 *  @param   id    : flexcomm index to get frequency.
 *  @return Frequency of Flexcomm functional Clock
 */
uint32_t CLOCK_GetFlexCommClkFreq(uint32_t id);

/*! @brief  Return Frequency of CTimer Clock
 *  @param   id    : ctimer index to get frequency.
 *  @return Frequency of CTimer Clock
 */
uint32_t CLOCK_GetCTimerClkFreq(uint32_t id);

/*! @brief  Return Frequency of Utick Clock
 *  @return Frequency of Utick Clock
 */
uint32_t CLOCK_GetUtickClkFreq(void);

/*! @brief  Return Frequency of Flexspi Clock
 *  @return Frequency of Flexspi.
 */
uint32_t CLOCK_GetFlexspiClkFreq(void);

/*! @brief  Return Frequency of USIM Clock
 *  @return Frequency of USIM.
 */
uint32_t CLOCK_GetUsimClkFreq(void);

/*! @brief  Return Frequency of GAU Clock
 *  @return Frequency of GAU.
 */
uint32_t CLOCK_GetGauClkFreq(void);

/*! @brief  Return Frequency of OSTimer Clock
 *  @return Frequency of OSTimer.
 */
uint32_t CLOCK_GetOSTimerClkFreq(void);

/*! @brief  Initialize TCPU FVCO to target frequency.
 *          For 40MHz XTAL, FVCO ranges from 3000MHz to 3840MHz.
            For 38.4MHz XTAL, FVCO ranges from 2995.2MHz to 3840MHz
 *  @param  targetHz  : Target FVCO frequency in Hz.
 *  @param  div       : Divider for tcpu_mci_flexspi_clk.
 *  @return Actual FVCO frequency in Hz.
 */
uint32_t CLOCK_InitTcpuRefClk(uint32_t targetHz, clock_tcpu_flexspi_div_t div);

/*! @brief  Deinit the TCPU reference clock.
 */
void CLOCK_DeinitTcpuRefClk(void);

/*! @brief  Initialize the TDDR reference clock.
 *  @param  div       : Divider for tddr_mci_flexspi_clk.
 */
void CLOCK_InitTddrRefClk(clock_tddr_flexspi_div_t div);

/*! @brief  Deinit the TDDR reference clock.
 */
void CLOCK_DeinitTddrRefClk(void);

/*! @brief  Initialize the T3 reference clock.
 *  @param  cnfg       : t3pll_mci_48_60m_irc clock configuration
 */
void CLOCK_InitT3RefClk(clock_t3_mci_irc_config_t cnfg);

/*! @brief  Deinit the T3 reference clock. */
void CLOCK_DeinitT3RefClk(void);

/*! @brief  Initialize the AVPLL. Both channel 1 and 2 are enabled.
 *  @param  cnfg       : AVPLL clock configuration
 */
void CLOCK_InitAvPll(const clock_avpll_config_t *cnfg);

/*! @brief  Deinit the AVPLL. All channels are disabled.
 */
void CLOCK_DeinitAvPll(void);

/*! @brief  Update the AVPLL channel configuration. Enable/Disable state keeps unchanged.
 *  @param  ch1Freq  : Channel 1 frequency to set.
 *  @param  ch2Freq  : Channel 2 frequency to set.
 *  @param  enableCali : Enable AVPLL calibration.
 */
void CLOCK_ConfigAvPllCh(clock_avpll_ch_freq_t ch1Freq, clock_avpll_ch_freq_t ch2Freq, bool enableCali);

/*! @brief  Enable the AVPLL channel.
 *  @param  enableCh1  : Enable AVPLL channel1, channel unchanged on false.
 *  @param  enableCh2  : Enable AVPLL channel2, channel unchanged on false.
 *  @param  enableCali : Enable AVPLL calibration.
 */
void CLOCK_EnableAvPllCh(bool enableCh1, bool enableCh2, bool enableCali);

/*! @brief  Disable the AVPLL.
 *  @param  disableCh1  : Disable AVPLL channel1, channel unchanged on false.
 *  @param  disableCh2  : Disable AVPLL channel2, channel unchanged on false.
 */
void CLOCK_DisableAvPllCh(bool disableCh1, bool disableCh2);

/*! @brief Enable USB HS PHY PLL clock.
 *
 * This function enables USB HS PHY PLL clock.
 */
void CLOCK_EnableUsbhsPhyClock(void);

/*! @brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhsPhyClock(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_CLOCK_H_ */
