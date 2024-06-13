/*
 * Copyright 2022-2023 NXP
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
 * Definitions
 *****************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief CLOCK driver version 1.0.1. */
#define FSL_CLOCK_DRIVER_VERSION (MAKE_VERSION(1, 0, 1))
/*@}*/

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

/*!
 * @brief User-defined the size of cache for CLOCK_PllGetConfig() function.
 *
 * Once define this MACRO to be non-zero value, CLOCK_PllGetConfig() function
 * would cache the recent calulation and accelerate the execution to get the
 * right settings.
 */
#ifndef CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT
#define CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT 2U
#endif

/* Definition for delay API in clock driver, users can redefine it to the real application. */
#ifndef SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY
#define SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY (150000000UL)
#endif

/*! @brief Clock ip name array for ROM. */
#define ROM_CLOCKS \
    {              \
        kCLOCK_Rom \
    }
/*! @brief Clock ip name array for SRAM. */
#define SRAM_CLOCKS                                                                                      \
    {                                                                                                    \
        kCLOCK_Sram1, kCLOCK_Sram2, kCLOCK_Sram3, kCLOCK_Sram4, kCLOCK_Sram5, kCLOCK_Sram6, kCLOCK_Sram7 \
    }
/*! @brief Clock ip name array for FMC. */
#define FMC_CLOCKS \
    {              \
        kCLOCK_Fmc \
    }
/*! @brief Clock ip name array for INPUTMUX. */
#define INPUTMUX_CLOCKS  \
    {                    \
        kCLOCK_InputMux0 \
    }
/*! @brief Clock ip name array for ENET. */
#define ETH_CLOCKS  \
    {               \
        kCLOCK_Enet \
    }
/*! @brief Clock ip name array for GPIO. */
#define GPIO_CLOCKS                                                          \
    {                                                                        \
        kCLOCK_Gpio0, kCLOCK_Gpio1, kCLOCK_Gpio2, kCLOCK_Gpio3, kCLOCK_Gpio4 \
    }
/*! @brief Clock ip name array for PINT. */
#define PINT_CLOCKS \
    {               \
        kCLOCK_Pint \
    }
/*! @brief Clock ip name array for DMA. */
#define DMA_CLOCKS               \
    {                            \
        kCLOCK_Dma0, kCLOCK_Dma1 \
    }
/*! @brief Clock gate name array for EDMA. */
#define EDMA_CLOCKS              \
    {                            \
        kCLOCK_Dma0, kCLOCK_Dma1 \
    }
/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS  \
    {               \
        kCLOCK_Crc0 \
    }
/*! @brief Clock ip name array for WWDT. */
#define WWDT_CLOCKS                \
    {                              \
        kCLOCK_Wwdt0, kCLOCK_Wwdt1 \
    }
/*! @brief Clock ip name array for Mailbox. */
#define MAILBOX_CLOCKS \
    {                  \
        kCLOCK_Mailbox \
    }
/*! @brief Clock ip name array for LPADC. */
#define LPADC_CLOCKS             \
    {                            \
        kCLOCK_Adc0, kCLOCK_Adc1 \
    }
/*! @brief Clock ip name array for MRT. */
#define MRT_CLOCKS \
    {              \
        kCLOCK_Mrt \
    }
/*! @brief Clock ip name array for OSTIMER. */
#define OSTIMER_CLOCKS \
    {                  \
        kCLOCK_OsTimer \
    }
/*! @brief Clock ip name array for SCT0. */
#define SCT_CLOCKS \
    {              \
        kCLOCK_Sct \
    }
/*! @brief Clock ip name array for UTICK. */
#define UTICK_CLOCKS \
    {                \
        kCLOCK_Utick \
    }
/*! @brief Clock ip name array for LP_FLEXCOMM. */
#define LP_FLEXCOMM_CLOCKS                                                                                     \
    {                                                                                                          \
        kCLOCK_LPFlexComm0, kCLOCK_LPFlexComm1, kCLOCK_LPFlexComm2, kCLOCK_LPFlexComm3, kCLOCK_LPFlexComm4,    \
            kCLOCK_LPFlexComm5, kCLOCK_LPFlexComm6, kCLOCK_LPFlexComm7, kCLOCK_LPFlexComm8, kCLOCK_LPFlexComm9 \
    }
/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS                                                                                   \
    {                                                                                                   \
        kCLOCK_LPUart0, kCLOCK_LPUart1, kCLOCK_LPUart2, kCLOCK_LPUart3, kCLOCK_LPUart4, kCLOCK_LPUart5, \
            kCLOCK_LPUart6, kCLOCK_LPUart7, kCLOCK_LPUart8, kCLOCK_LPUart9                              \
    }
/*! @brief Clock ip name array for LPI2C. */
#define LPI2C_CLOCKS                                                                                             \
    {                                                                                                            \
        kCLOCK_LPI2c0, kCLOCK_LPI2c1, kCLOCK_LPI2c2, kCLOCK_LPI2c3, kCLOCK_LPI2c4, kCLOCK_LPI2c5, kCLOCK_LPI2c6, \
            kCLOCK_LPI2c7, kCLOCK_LPI2c8, kCLOCK_LPI2c9                                                          \
    }
/*! @brief Clock ip name array for LSPI. */
#define LPSPI_CLOCKS                                                                                             \
    {                                                                                                            \
        kCLOCK_LPSpi0, kCLOCK_LPSpi1, kCLOCK_LPSpi2, kCLOCK_LPSpi3, kCLOCK_LPSpi4, kCLOCK_LPSpi5, kCLOCK_LPSpi6, \
            kCLOCK_LPSpi7, kCLOCK_LPSpi8, kCLOCK_LPSpi9                                                          \
    }
/*! @brief Clock ip name array for CTIMER. */
#define CTIMER_CLOCKS                                                             \
    {                                                                             \
        kCLOCK_Timer0, kCLOCK_Timer1, kCLOCK_Timer2, kCLOCK_Timer3, kCLOCK_Timer4 \
    }
/*! @brief Clock ip name array for FREQME. */
#define FREQME_CLOCKS \
    {                 \
        kCLOCK_Freqme \
    }
/*! @brief Clock ip name array for PowerQuad. */
#define POWERQUAD_CLOCKS \
    {                    \
        kCLOCK_PowerQuad \
    }
/*! @brief Clock ip name array for PLU. */
#define PLU_CLOCKS    \
    {                 \
        kCLOCK_PluLut \
    }
/*! @brief Clock ip name array for PUF. */
#define PUF_CLOCKS \
    {              \
        kCLOCK_Puf \
    }
/*! @brief Clock ip name array for VREF. */
#define VREF_CLOCKS \
    {               \
        kCLOCK_Vref \
    }
/*! @brief Clock ip name array for LPDAC. */
#define LPDAC_CLOCKS             \
    {                            \
        kCLOCK_Dac0, kCLOCK_Dac1 \
    }
/*! @brief Clock ip name array for HPDAC. */
#define HPDAC_CLOCKS \
    {                \
        kCLOCK_Dac2  \
    }
/*! @brief Clock ip name array for PWM. */
#define PWM_CLOCKS                                                             \
    {                                                                          \
        {kCLOCK_Pwm0_Sm0, kCLOCK_Pwm0_Sm1, kCLOCK_Pwm0_Sm2, kCLOCK_Pwm0_Sm3},  \
        {                                                                      \
            kCLOCK_Pwm1_Sm0, kCLOCK_Pwm1_Sm1, kCLOCK_Pwm1_Sm2, kCLOCK_Pwm1_Sm3 \
        }                                                                      \
    }
/*! @brief Clock ip name array for ENC. */
#define ENC_CLOCKS               \
    {                            \
        kCLOCK_Enc0, kCLOCK_Enc1 \
    }
/*! @brief Clock ip name array for FLEXIO. */
#define FLEXIO_CLOCKS \
    {                 \
        kCLOCK_Flexio \
    }
/*! @brief Clock ip name array for FLEXCAN. */
#define FLEXCAN_CLOCKS                   \
    {                                    \
        kCLOCK_Flexcan0, kCLOCK_Flexcan1 \
    }
/*! @brief Clock ip name array for EMVSIM. */
#define EMVSIM_CLOCKS                \
    {                                \
        kCLOCK_Evsim0, kCLOCK_Evsim1 \
    }
/*! @brief Clock ip name array for I3C */
#define I3C_CLOCKS               \
    {                            \
        kCLOCK_I3c0, kCLOCK_I3c1 \
    }
/*! @brief Clock ip name array for USDHC. */
#define USDHC_CLOCKS \
    {                \
        kCLOCK_uSdhc \
    }
/*! @brief Clock ip name array for FLEXSPI */
#define FLEXSPI_CLOCKS \
    {                  \
        kCLOCK_Flexspi \
    }
/*! @brief Clock ip name array for SAI. */
#define SAI_CLOCKS               \
    {                            \
        kCLOCK_Sai0, kCLOCK_Sai1 \
    }
/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS  \
    {               \
        kCLOCK_Rtc0 \
    }
/*! @brief Clock ip name array for PDM. */
#define PDM_CLOCKS    \
    {                 \
        kCLOCK_Micfil \
    }
/*! @brief Clock ip name array for ERM. */
#define ERM_CLOCKS \
    {              \
        kCLOCK_Erm \
    }
/*! @brief Clock ip name array for EIM. */
#define EIM_CLOCKS \
    {              \
        kCLOCK_Eim \
    }
/*! @brief Clock ip name array for OPAMP. */
#define OPAMP_CLOCKS                                \
    {                                               \
        kCLOCK_Opamp0, kCLOCK_Opamp1, kCLOCK_Opamp2 \
    }
/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS \
    {              \
        kCLOCK_Tsi \
    }
/*! @brief Clock ip name array for TRNG. */
#define TRNG_CLOCKS \
    {               \
        kCLOCK_Trng \
    }
/*! @brief Clock ip name array for LPCMP. */
#define LPCMP_CLOCKS                          \
    {                                         \
        kCLOCK_None, kCLOCK_None, kCLOCK_Cmp2 \
    }
/*! @brief Clock ip name array for SINC */
#define SINC_CLOCKS \
    {               \
        kCLOCK_Sinc \
    }
/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
/*------------------------------------------------------------------------------
 clock_ip_name_t definition:
------------------------------------------------------------------------------*/

#define CLK_GATE_REG_OFFSET_SHIFT 8U
#define CLK_GATE_REG_OFFSET_MASK  0xFFFFFF00U
#define CLK_GATE_BIT_SHIFT_SHIFT  0U
#define CLK_GATE_BIT_SHIFT_MASK   0x000000FFU

#define CLK_GATE_DEFINE(reg_offset, bit_shift)                                  \
    ((((reg_offset) << CLK_GATE_REG_OFFSET_SHIFT) & CLK_GATE_REG_OFFSET_MASK) | \
     (((bit_shift) << CLK_GATE_BIT_SHIFT_SHIFT) & CLK_GATE_BIT_SHIFT_MASK))

#define CLK_GATE_ABSTRACT_REG_OFFSET(x) (((uint32_t)(x)&CLK_GATE_REG_OFFSET_MASK) >> CLK_GATE_REG_OFFSET_SHIFT)
#define CLK_GATE_ABSTRACT_BITS_SHIFT(x) (((uint32_t)(x)&CLK_GATE_BIT_SHIFT_MASK) >> CLK_GATE_BIT_SHIFT_SHIFT)

#define AHB_CLK_CTRL0  0
#define AHB_CLK_CTRL1  1
#define AHB_CLK_CTRL2  2
#define AHB_CLK_CTRL3  3
#define REG_PWM0SUBCTL 250
#define REG_PWM1SUBCTL 251

/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,                                   /*!< Invalid Ip Name. */
    kCLOCK_None      = 0U,                                   /*!< None clock gate. */

    kCLOCK_Rom       = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 1),    /*!< Clock gate name: Rom. */
    kCLOCK_Sram1     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 2),    /*!< Clock gate name: Sram1. */
    kCLOCK_Sram2     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 3),    /*!< Clock gate name: Sram2. */
    kCLOCK_Sram3     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 4),    /*!< Clock gate name: Sram3. */
    kCLOCK_Sram4     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 5),    /*!< Clock gate name: Sram4. */
    kCLOCK_Sram5     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 6),    /*!< Clock gate name: Sram5. */
    kCLOCK_Sram6     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 7),    /*!< Clock gate name: Sram6. */
    kCLOCK_Sram7     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 8),    /*!< Clock gate name: Sram7. */
    kCLOCK_Fmu       = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 9),    /*!< Clock gate name: Fmu. */
    kCLOCK_Fmc       = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 10),   /*!< Clock gate name: Fmc. */
    kCLOCK_Flexspi   = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 11),   /*!< Clock gate name: Flexspi. */
    kCLOCK_InputMux0 = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 12),   /*!< Clock gate name: InputMux0. */
    kCLOCK_InputMux  = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 12),   /*!< Clock gate name: InputMux0. */
    kCLOCK_Port0     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 13),   /*!< Clock gate name: Port0. */
    kCLOCK_Port1     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 14),   /*!< Clock gate name: Port1. */
    kCLOCK_Port2     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 15),   /*!< Clock gate name: Port2. */
    kCLOCK_Port3     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 16),   /*!< Clock gate name: Port3. */
    kCLOCK_Port4     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 17),   /*!< Clock gate name: Port4. */
    kCLOCK_Gpio0     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 19),   /*!< Clock gate name: Gpio0. */
    kCLOCK_Gpio1     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 20),   /*!< Clock gate name: Gpio1. */
    kCLOCK_Gpio2     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 21),   /*!< Clock gate name: Gpio2. */
    kCLOCK_Gpio3     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 22),   /*!< Clock gate name: Gpio3. */
    kCLOCK_Gpio4     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 23),   /*!< Clock gate name: Gpio4. */
    kCLOCK_Pint      = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 25),   /*!< Clock gate name: Pint. */
    kCLOCK_Dma0      = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 26),   /*!< Clock gate name: Dma0. */
    kCLOCK_Crc0      = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 27),   /*!< Clock gate name: Crc. */
    kCLOCK_Wwdt0     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 28),   /*!< Clock gate name: Wwdt0. */
    kCLOCK_Wwdt1     = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 29),   /*!< Clock gate name: Wwdt1. */
    kCLOCK_Mailbox   = CLK_GATE_DEFINE(AHB_CLK_CTRL0, 31),   /*!< Clock gate name: Mailbox. */

    kCLOCK_Mrt         = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 0),  /*!< Clock gate name: Mrt. */
    kCLOCK_OsTimer     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 1),  /*!< Clock gate name: OsTimer. */
    kCLOCK_Sct         = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 2),  /*!< Clock gate name: Sct. */
    kCLOCK_Adc0        = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 3),  /*!< Clock gate name: Adc0. */
    kCLOCK_Adc1        = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 4),  /*!< Clock gate name: Adc1. */
    kCLOCK_Dac0        = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 5),  /*!< Clock gate name: Dac0. */
    kCLOCK_Rtc0        = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 6),  /*!< Clock gate name: Rtc. */
    kCLOCK_Evsim0      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 8),  /*!< Clock gate name: Evsim0. */
    kCLOCK_Evsim1      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 9),  /*!< Clock gate name: Evsim1. */
    kCLOCK_Utick       = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 10), /*!< Clock gate name: Utick. */
    kCLOCK_LPFlexComm0 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 11), /*!< Clock gate name: LPFlexComm0. */
    kCLOCK_LPFlexComm1 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 12), /*!< Clock gate name: LPFlexComm1. */
    kCLOCK_LPFlexComm2 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 13), /*!< Clock gate name: LPFlexComm2. */
    kCLOCK_LPFlexComm3 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 14), /*!< Clock gate name: LPFlexComm3. */
    kCLOCK_LPFlexComm4 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 15), /*!< Clock gate name: LPFlexComm4. */
    kCLOCK_LPFlexComm5 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 16), /*!< Clock gate name: LPFlexComm5. */
    kCLOCK_LPFlexComm6 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 17), /*!< Clock gate name: LPFlexComm6. */
    kCLOCK_LPFlexComm7 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 18), /*!< Clock gate name: LPFlexComm7. */
    kCLOCK_LPFlexComm8 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 19), /*!< Clock gate name: LPFlexComm8. */
    kCLOCK_LPFlexComm9 = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 20), /*!< Clock gate name: LPFlexComm9. */
    kCLOCK_LPUart0     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 11), /*!< Clock gate name: LPUart0. */
    kCLOCK_LPUart1     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 12), /*!< Clock gate name: LPUart1. */
    kCLOCK_LPUart2     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 13), /*!< Clock gate name: LPUart2. */
    kCLOCK_LPUart3     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 14), /*!< Clock gate name: LPUart3. */
    kCLOCK_LPUart4     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 15), /*!< Clock gate name: LPUart4. */
    kCLOCK_LPUart5     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 16), /*!< Clock gate name: LPUart5. */
    kCLOCK_LPUart6     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 17), /*!< Clock gate name: LPUart6. */
    kCLOCK_LPUart7     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 18), /*!< Clock gate name: LPUart7. */
    kCLOCK_LPUart8     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 19), /*!< Clock gate name: LPUart8. */
    kCLOCK_LPUart9     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 20), /*!< Clock gate name: LPUart9. */
    kCLOCK_LPSpi0      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 11), /*!< Clock gate name: LPSpi0. */
    kCLOCK_LPSpi1      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 12), /*!< Clock gate name: LPSpi1. */
    kCLOCK_LPSpi2      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 13), /*!< Clock gate name: LPSpi2. */
    kCLOCK_LPSpi3      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 14), /*!< Clock gate name: LPSpi3. */
    kCLOCK_LPSpi4      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 15), /*!< Clock gate name: LPSpi4. */
    kCLOCK_LPSpi5      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 16), /*!< Clock gate name: LPSpi5. */
    kCLOCK_LPSpi6      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 17), /*!< Clock gate name: LPSpi6. */
    kCLOCK_LPSpi7      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 18), /*!< Clock gate name: LPSpi7. */
    kCLOCK_LPSpi8      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 19), /*!< Clock gate name: LPSpi8. */
    kCLOCK_LPSpi9      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 20), /*!< Clock gate name: LSpi9. */
    kCLOCK_LPI2c0      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 11), /*!< Clock gate name: LPI2c0. */
    kCLOCK_LPI2c1      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 12), /*!< Clock gate name: LPI2c1. */
    kCLOCK_LPI2c2      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 13), /*!< Clock gate name: LPI2c2. */
    kCLOCK_LPI2c3      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 14), /*!< Clock gate name: LPI2c3. */
    kCLOCK_LPI2c4      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 15), /*!< Clock gate name: LPI2c4. */
    kCLOCK_LPI2c5      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 16), /*!< Clock gate name: LPI2c5. */
    kCLOCK_LPI2c6      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 17), /*!< Clock gate name: LPI2c6. */
    kCLOCK_LPI2c7      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 18), /*!< Clock gate name: LPI2c7. */
    kCLOCK_LPI2c8      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 19), /*!< Clock gate name: LPI2c8. */
    kCLOCK_LPI2c9      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 20), /*!< Clock gate name: LPI2c9. */
    kCLOCK_Micfil      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 21), /*!< Clock gate name: Micfil. */
    kCLOCK_Timer2      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 22), /*!< Clock gate name: Timer2. */
    kCLOCK_Usb0Ram     = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 23), /*!< Clock gate name: Usb0Ram. */
    kCLOCK_Usb0FsDcd   = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 24), /*!< Clock gate name: Usb0FsDcd. */
    kCLOCK_Usb0Fs      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 25), /*!< Clock gate name: Usb0Fs. */
    kCLOCK_Timer0      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 26), /*!< Clock gate name: Timer0. */
    kCLOCK_Timer1      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 27), /*!< Clock gate name: Timer1. */
    kCLOCK_PkcRam      = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 29), /*!< Clock gate name: PkcRam. */
    kCLOCK_Smartdma    = CLK_GATE_DEFINE(AHB_CLK_CTRL1, 31), /*!< Clock gate name: SmartDma. */

    kCLOCK_Espi      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 0),    /*!< Clock gate name: Espi. */
    kCLOCK_Dma1      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 1),    /*!< Clock gate name: Dma1. */
    kCLOCK_Enet      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 2),    /*!< Clock gate name: Enet. */
    kCLOCK_uSdhc     = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 3),    /*!< Clock gate name: uSdhc. */
    kCLOCK_Flexio    = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 4),    /*!< Clock gate name: Flexio. */
    kCLOCK_Sai0      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 5),    /*!< Clock gate name: Sai0. */
    kCLOCK_Sai1      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 6),    /*!< Clock gate name: Sai1. */
    kCLOCK_Tro       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 7),    /*!< Clock gate name: Tro. */
    kCLOCK_Freqme    = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 8),    /*!< Clock gate name: Freqme. */
    kCLOCK_Trng      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 13),   /*!< Clock gate name: Trng. */
    kCLOCK_Flexcan0  = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 14),   /*!< Clock gate name: Flexcan0. */
    kCLOCK_Flexcan1  = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 15),   /*!< Clock gate name: Flexcan1. */
    kCLOCK_UsbHs     = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 16),   /*!< Clock gate name: UsbHs. */
    kCLOCK_UsbHsPhy  = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 17),   /*!< Clock gate name: UsbHsPhy. */
    kCLOCK_Css       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 18),   /*!< Clock gate name: Css. */
    kCLOCK_PowerQuad = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 19),   /*!< Clock gate name: PowerQuad. */
    kCLOCK_PluLut    = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 20),   /*!< Clock gate name: PluLut. */
    kCLOCK_Timer3    = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 21),   /*!< Clock gate name: Timer3. */
    kCLOCK_Timer4    = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 22),   /*!< Clock gate name: Timer4. */
    kCLOCK_Puf       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 23),   /*!< Clock gate name: Puf. */
    kCLOCK_Pkc       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 24),   /*!< Clock gate name: Pkc. */
    kCLOCK_Scg       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 26),   /*!< Clock gate name: Scg. */
    kCLOCK_Gdet      = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 29),   /*!< Clock gate name: Gdet. */
    kCLOCK_Sm3       = CLK_GATE_DEFINE(AHB_CLK_CTRL2, 30),   /*!< Clock gate name: Sm3. */

    kCLOCK_I3c0        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 0),  /*!< Clock gate name: I3c0. */
    kCLOCK_I3c1        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 1),  /*!< Clock gate name: I3c1. */
    kCLOCK_Sinc        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 2),  /*!< Clock gate name: Sinc. */
    kCLOCK_CoolFlux    = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 3),  /*!< Clock gate name: CoolFlux. */
    kCLOCK_Enc0        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 4),  /*!< Clock gate name: Enc0. */
    kCLOCK_Enc1        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 5),  /*!< Clock gate name: Enc1. */
    kCLOCK_Pwm0        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 6),  /*!< Clock gate name: Pwm0. */
    kCLOCK_Pwm1        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 7),  /*!< Clock gate name: Pwm1. */
    kCLOCK_Evtg        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 8),  /*!< Clock gate name: Evtg. */
    kCLOCK_Dac1        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 11), /*!< Clock gate name: Dac1. */
    kCLOCK_Dac2        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 12), /*!< Clock gate name: Dac2. */
    kCLOCK_Opamp0      = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 13), /*!< Clock gate name: Opamp0. */
    kCLOCK_Opamp1      = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 14), /*!< Clock gate name: Opamp1. */
    kCLOCK_Opamp2      = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 15), /*!< Clock gate name: Opamp2. */
    kCLOCK_Cmp2        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 18), /*!< Clock gate name: Cmp2. */
    kCLOCK_Vref        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 19), /*!< Clock gate name: Vref. */
    kCLOCK_CoolFluxApb = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 20), /*!< Clock gate name: CoolFluxApb. */
    kCLOCK_Neutron     = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 21), /*!< Clock gate name: Neutron. */
    kCLOCK_Tsi         = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 22), /*!< Clock gate name: Tsi. */
    kCLOCK_Ewm         = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 23), /*!< Clock gate name: Ewm. */
    kCLOCK_Ewm0        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 23), /*!< Clock gate name: Ewm. */
    kCLOCK_Eim         = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 24), /*!< Clock gate name: Eim. */
    kCLOCK_Erm         = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 25), /*!< Clock gate name: Erm. */
    kCLOCK_Intm        = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 26), /*!< Clock gate name: Intm. */
    kCLOCK_Sema42      = CLK_GATE_DEFINE(AHB_CLK_CTRL3, 27), /*!< Clock gate name: Sema42. */

    kCLOCK_Pwm0_Sm0 = CLK_GATE_DEFINE(REG_PWM0SUBCTL, 0U),   /*!< Clock gate name: PWM0 SM0. */
    kCLOCK_Pwm0_Sm1 = CLK_GATE_DEFINE(REG_PWM0SUBCTL, 1U),   /*!< Clock gate name: PWM0 SM1. */
    kCLOCK_Pwm0_Sm2 = CLK_GATE_DEFINE(REG_PWM0SUBCTL, 2U),   /*!< Clock gate name: PWM0 SM2. */
    kCLOCK_Pwm0_Sm3 = CLK_GATE_DEFINE(REG_PWM0SUBCTL, 3U),   /*!< Clock gate name: PWM0 SM3. */

    kCLOCK_Pwm1_Sm0 = CLK_GATE_DEFINE(REG_PWM1SUBCTL, 0U),   /*!< Clock gate name: PWM1 SM0. */
    kCLOCK_Pwm1_Sm1 = CLK_GATE_DEFINE(REG_PWM1SUBCTL, 1U),   /*!< Clock gate name: PWM1 SM1. */
    kCLOCK_Pwm1_Sm2 = CLK_GATE_DEFINE(REG_PWM1SUBCTL, 2U),   /*!< Clock gate name: PWM1 SM2. */
    kCLOCK_Pwm1_Sm3 = CLK_GATE_DEFINE(REG_PWM1SUBCTL, 3U)    /*!< Clock gate name: PWM1 SM3. */

} clock_ip_name_t;

/*! @brief Peripherals clock source definition. */
#define BUS_CLK kCLOCK_BusClk

#define I2C0_CLK_SRC BUS_CLK

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    kCLOCK_MainClk,     /*!< Main clock                                              */
    kCLOCK_CoreSysClk,  /*!< Core/system clock                                       */
    kCLOCK_BusClk,      /*!< Bus clock (AHB clock)                                   */
    kCLOCK_SystickClk0, /*!< Systick clock0                                          */
    kCLOCK_SystickClk1, /*!< Systick clock1                                          */
    kCLOCK_ClockOut,    /*!< CLOCKOUT                                                */
    kCLOCK_Fro12M,      /*!< FRO12M                                                  */
    kCLOCK_Clk1M,       /*!< CLK1M                                                   */
    kCLOCK_FroHf,       /*!< FRO48/144                                               */
    kCLOCK_Clk48M,      /*!< CLK48M                                                  */
    kCLOCK_Clk144M,     /*!< CLK144M                                                 */
    kCLOCK_Clk16K0,     /*!< CLK16K[0]                                               */
    kCLOCK_Clk16K1,     /*!< CLK16K[1]                                               */
    kCLOCK_Clk16K2,     /*!< CLK16K[2]                                               */
    kCLOCK_Clk16K3,     /*!< CLK16K[3]                                               */
    kCLOCK_ExtClk,      /*!< External Clock                                          */
    kCLOCK_Osc32K0,     /*!< OSC32K[0]                                               */
    kCLOCK_Osc32K1,     /*!< OSC32K[1]                                               */
    kCLOCK_Osc32K2,     /*!< OSC32K[2]                                               */
    kCLOCK_Osc32K3,     /*!< OSC32K[3]                                               */
    kCLOCK_Pll0Out,     /*!< PLL0 Output                                             */
    kCLOCK_Pll1Out,     /*!< PLL1 Output                                             */
    kCLOCK_UsbPllOut,   /*!< USB PLL Output                                          */
    kCLOCK_LpOsc,       /*!< lp_osc                                                  */
} clock_name_t;

/*! @brief Clock Mux Switches
 *  The encoding is as follows each connection identified is 32bits wide while 24bits are valuable
 *  starting from LSB upwards
 *
 *  [4 bits for choice, 0 means invalid choice] [8 bits mux ID]*
 *
 */

#define CLK_ATTACH_ID(mux, sel, pos) \
    ((((uint32_t)(mux) << 0U) | (((uint32_t)(sel) + 1U) & 0xFU) << 12U) << ((uint32_t)(pos)*16U))
#define MUX_A(mux, sel)           CLK_ATTACH_ID((mux), (sel), 0U)
#define MUX_B(mux, sel, selector) (CLK_ATTACH_ID((mux), (sel), 1U) | ((selector) << 24U))

#define GET_ID_ITEM(connection)      ((connection)&0xFFFFU)
#define GET_ID_NEXT_ITEM(connection) ((connection) >> 16U)
#define GET_ID_ITEM_MUX(connection)  (((uint16_t)connection) & 0xFFFU)
#define GET_ID_ITEM_SEL(connection)  ((uint8_t)((((uint32_t)(connection)&0xF000U) >> 12U) - 1U))
#define GET_ID_SELECTOR(connection)  ((connection)&0xF000000U)

#define CM_SYSTICKCLKSEL0   0U
#define CM_SYSTICKCLKSEL1   ((0x264 - 0x260) / 4)
#define CM_TRACECLKSEL      ((0x268 - 0x260) / 4)
#define CM_CTIMERCLKSEL0    ((0x26C - 0x260) / 4)
#define CM_CTIMERCLKSEL1    ((0x270 - 0x260) / 4)
#define CM_CTIMERCLKSEL2    ((0x274 - 0x260) / 4)
#define CM_CTIMERCLKSEL3    ((0x278 - 0x260) / 4)
#define CM_CTIMERCLKSEL4    ((0x27C - 0x260) / 4)
#define CM_CLKOUTCLKSEL     ((0x288 - 0x260) / 4)
#define CM_ADC0CLKSEL       ((0x2A4 - 0x260) / 4)
#define CM_USB0CLKSEL       ((0x2A8 - 0x260) / 4)
#define CM_FCCLKSEL0        ((0x2B0 - 0x260) / 4)
#define CM_FCCLKSEL1        ((0x2B4 - 0x260) / 4)
#define CM_FCCLKSEL2        ((0x2B8 - 0x260) / 4)
#define CM_FCCLKSEL3        ((0x2BC - 0x260) / 4)
#define CM_FCCLKSEL4        ((0x2C0 - 0x260) / 4)
#define CM_FCCLKSEL5        ((0x2C4 - 0x260) / 4)
#define CM_FCCLKSEL6        ((0x2C8 - 0x260) / 4)
#define CM_FCCLKSEL7        ((0x2CC - 0x260) / 4)
#define CM_FCCLKSEL8        ((0x2D0 - 0x260) / 4)
#define CM_FCCLKSEL9        ((0x2D4 - 0x260) / 4)
#define CM_SCTCLKSEL        ((0x2F0 - 0x260) / 4)
#define CM_TSICLKSEL        ((0x350 - 0x260) / 4)
#define CM_SINCFILTCLKSEL   ((0x360 - 0x260) / 4)
#define CM_ADC1CLKSEL       ((0x464 - 0x260) / 4)
#define CM_DAC0CLKSEL       ((0x490 - 0x260) / 4)
#define CM_DAC1CLKSEL       ((0x498 - 0x260) / 4)
#define CM_DAC2CLKSEL       ((0x4A0 - 0x260) / 4)
#define CM_FLEXSPICLKSEL    ((0x4A8 - 0x260) / 4)
#define CM_PLLCLKDIVSEL     ((0x52C - 0x260) / 4)
#define CM_I3C0FCLKSEL      ((0x530 - 0x260) / 4)
#define CM_I3C0FCLKSTCSEL   ((0x534 - 0x260) / 4)
#define CM_I3C0FCLKSSEL     ((0x544 - 0x260) / 4)
#define CM_MICFILFCLKSEL    ((0x548 - 0x260) / 4)
#define CM_ESPICLKSEL       ((0x550 - 0x260) / 4)
#define CM_USDHCCLKSEL      ((0x558 - 0x260) / 4)
#define CM_FLEXIOCLKSEL     ((0x560 - 0x260) / 4)
#define CM_FLEXCAN0CLKSEL   ((0x5A0 - 0x260) / 4)
#define CM_FLEXCAN1CLKSEL   ((0x5A8 - 0x260) / 4)
#define CM_ENETRMIICLKSEL   ((0x5B0 - 0x260) / 4)
#define CM_ENETPTPREFCLKSEL ((0x5B8 - 0x260) / 4)
#define CM_EWM0CLKSEL       ((0x5D4 - 0x260) / 4)
#define CM_WDT1CLKSEL       ((0x5D8 - 0x260) / 4)
#define CM_OSTIMERCLKSEL    ((0x5E0 - 0x260) / 4)
#define CM_CMP0FCLKSEL      ((0x5F0 - 0x260) / 4)
#define CM_CMP0RRCLKSEL     ((0x5F8 - 0x260) / 4)
#define CM_CMP1FCLKSEL      ((0x600 - 0x260) / 4)
#define CM_CMP1RRCLKSEL     ((0x608 - 0x260) / 4)
#define CM_CMP2FCLKSEL      ((0x610 - 0x260) / 4)
#define CM_CMP2RRCLKSEL     ((0x618 - 0x260) / 4)
#define CM_SAI0CLKSEL       ((0x880 - 0x260) / 4)
#define CM_SAI1CLKSEL       ((0x884 - 0x260) / 4)
#define CM_EMVSIM0CLKSEL    ((0x890 - 0x260) / 4)
#define CM_EMVSIM1CLKSEL    ((0x894 - 0x260) / 4)
#define CM_I3C1FCLKSEL      ((0xB30 - 0x260) / 4)
#define CM_I3C1FCLKSTCSEL   ((0xB34 - 0x260) / 4)
#define CM_I3C1FCLKSSEL     ((0xB44 - 0x260) / 4)

#define CM_SCGRCCRSCSCLKSEL 0x3FEU

/*!
 * @brief The enumerator of clock attach Id.
 */
typedef enum _clock_attach_id
{
    kCLK_IN_to_MAIN_CLK   = MUX_A(CM_SCGRCCRSCSCLKSEL, 1),       /*!< Attach clk_in to MAIN_CLK. */
    kFRO12M_to_MAIN_CLK   = MUX_A(CM_SCGRCCRSCSCLKSEL, 2),       /*!< Attach FRO_12M to MAIN_CLK. */
    kFRO_HF_to_MAIN_CLK   = MUX_A(CM_SCGRCCRSCSCLKSEL, 3),       /*!< Attach FRO_HF to MAIN_CLK. */
    kXTAL32K2_to_MAIN_CLK = MUX_A(CM_SCGRCCRSCSCLKSEL, 4),       /*!< Attach xtal32k[2] to MAIN_CLK. */
    kPLL0_to_MAIN_CLK     = MUX_A(CM_SCGRCCRSCSCLKSEL, 5),       /*!< Attach PLL0 to MAIN_CLK. */
    kPLL1_to_MAIN_CLK     = MUX_A(CM_SCGRCCRSCSCLKSEL, 6),       /*!< Attach PLL1 to MAIN_CLK. */
    kUSB_PLL_to_MAIN_CLK  = MUX_A(CM_SCGRCCRSCSCLKSEL, 7),       /*!< Attach USB PLL to MAIN_CLK. */
    kNONE_to_MAIN_CLK     = MUX_A(CM_SCGRCCRSCSCLKSEL, 15),      /*!< Attach NONE to MAIN_CLK. */

    kSYSTICK_DIV0_to_SYSTICK0 = MUX_A(CM_SYSTICKCLKSEL0, 0),     /*!< Attach SYSTICK_DIV0 to SYSTICK0. */
    kCLK_1M_to_SYSTICK0       = MUX_A(CM_SYSTICKCLKSEL0, 1),     /*!< Attach Clk 1 MHz to SYSTICK0. */
    kLPOSC_to_SYSTICK0        = MUX_A(CM_SYSTICKCLKSEL0, 2),     /*!< Attach LP Oscillator to SYSTICK0. */
    kNONE_to_SYSTICK0         = MUX_A(CM_SYSTICKCLKSEL0, 7),     /*!< Attach NONE to SYSTICK0. */

    kSYSTICK_DIV1_to_SYSTICK1 = MUX_A(CM_SYSTICKCLKSEL1, 0),     /*!< Attach SYSTICK_DIV1 to SYSTICK1. */
    kCLK_1M_to_SYSTICK1       = MUX_A(CM_SYSTICKCLKSEL1, 1),     /*!< Attach Clk 1 MHz to SYSTICK1. */
    kLPOSC_to_SYSTICK1        = MUX_A(CM_SYSTICKCLKSEL1, 2),     /*!< Attach LP Oscillator to SYSTICK1. */
    kNONE_to_SYSTICK1         = MUX_A(CM_SYSTICKCLKSEL1, 7),     /*!< Attach NONE to SYSTICK1. */

    kTRACE_DIV_to_TRACE = MUX_A(CM_TRACECLKSEL, 0),              /*!< Attach TRACE_DIV to TRACE. */
    kCLK_1M_to_TRACE    = MUX_A(CM_TRACECLKSEL, 1),              /*!< Attach Clk 1 MHz to TRACE. */
    kLPOSC_to_TRACE     = MUX_A(CM_TRACECLKSEL, 2),              /*!< Attach LP Oscillator to TRACE. */
    kNONE_to_TRACE      = MUX_A(CM_TRACECLKSEL, 7),              /*!< Attach NONE to TRACE. */

    kCLK_1M_to_CTIMER0       = MUX_A(CM_CTIMERCLKSEL0, 0),       /*!< Attach CLK_1M to CTIMER0. */
    kPLL0_to_CTIMER0         = MUX_A(CM_CTIMERCLKSEL0, 1),       /*!< Attach PLL0 to CTIMER0. */
    kPLL1_CLK0_to_CTIMER0    = MUX_A(CM_CTIMERCLKSEL0, 2),       /*!< Attach PLL1_clk0 to CTIMER0. */
    kFRO_HF_to_CTIMER0       = MUX_A(CM_CTIMERCLKSEL0, 3),       /*!< Attach FRO_HF to CTIMER0. */
    kFRO12M_to_CTIMER0       = MUX_A(CM_CTIMERCLKSEL0, 4),       /*!< Attach FRO 12MHz to CTIMER0. */
    kSAI0_MCLK_IN_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 5),       /*!< Attach SAI0 MCLK IN to CTIMER0. */
    kLPOSC_to_CTIMER0        = MUX_A(CM_CTIMERCLKSEL0, 6),       /*!< Attach LP Oscillator to CTIMER0. */
    kSAI1_MCLK_IN_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 8),       /*!< Attach SAI1 MCLK IN to CTIMER0. */
    kSAI0_TX_BCLK_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 9),       /*!< Attach SAI0 TX_BCLK to CTIMER0. */
    kSAI0_RX_BCLK_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 10),      /*!< Attach SAI0 RX_BCLK to CTIMER0. */
    kSAI1_TX_BCLK_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 11),      /*!< Attach SAI1 TX_BCLK to CTIMER0. */
    kSAI1_RX_BCLK_to_CTIMER0 = MUX_A(CM_CTIMERCLKSEL0, 12),      /*!< Attach SAI1 RX_BCLK to CTIMER0. */
    kNONE_to_CTIMER0         = MUX_A(CM_CTIMERCLKSEL0, 15),      /*!< Attach NONE to CTIMER0. */

    kCLK_1M_to_CTIMER1       = MUX_A(CM_CTIMERCLKSEL1, 0),       /*!< Attach CLK_1M to CTIMER1. */
    kPLL0_to_CTIMER1         = MUX_A(CM_CTIMERCLKSEL1, 1),       /*!< Attach PLL0 to CTIMER1. */
    kPLL1_CLK0_to_CTIMER1    = MUX_A(CM_CTIMERCLKSEL1, 2),       /*!< Attach PLL1_clk0 to CTIMER1. */
    kFRO_HF_to_CTIMER1       = MUX_A(CM_CTIMERCLKSEL1, 3),       /*!< Attach FRO_HF to CTIMER1. */
    kFRO12M_to_CTIMER1       = MUX_A(CM_CTIMERCLKSEL1, 4),       /*!< Attach FRO 12MHz to CTIMER1. */
    kSAI0_MCLK_IN_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 5),       /*!< Attach SAI0 MCLK IN to CTIMER1. */
    kLPOSC_to_CTIMER1        = MUX_A(CM_CTIMERCLKSEL1, 6),       /*!< Attach LP Oscillator to CTIMER1. */
    kSAI1_MCLK_IN_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 8),       /*!< Attach SAI1 MCLK IN to CTIMER1. */
    kSAI0_TX_BCLK_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 9),       /*!< Attach SAI0 TX_BCLK to CTIMER1. */
    kSAI0_RX_BCLK_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 10),      /*!< Attach SAI0 RX_BCLK to CTIMER1. */
    kSAI1_TX_BCLK_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 11),      /*!< Attach SAI1 TX_BCLK to CTIMER1. */
    kSAI1_RX_BCLK_to_CTIMER1 = MUX_A(CM_CTIMERCLKSEL1, 12),      /*!< Attach SAI1 RX_BCLK to CTIMER1. */
    kNONE_to_CTIMER1         = MUX_A(CM_CTIMERCLKSEL1, 15),      /*!< Attach NONE to CTIMER1. */

    kCLK_1M_to_CTIMER2       = MUX_A(CM_CTIMERCLKSEL2, 0),       /*!< Attach CLK_1M to CTIMER2. */
    kPLL0_to_CTIMER2         = MUX_A(CM_CTIMERCLKSEL2, 1),       /*!< Attach PLL0 to CTIMER2. */
    kPLL1_CLK0_to_CTIMER2    = MUX_A(CM_CTIMERCLKSEL2, 2),       /*!< Attach PLL1_clk0 to CTIMER2. */
    kFRO_HF_to_CTIMER2       = MUX_A(CM_CTIMERCLKSEL2, 3),       /*!< Attach FRO_HF to CTIMER2. */
    kFRO12M_to_CTIMER2       = MUX_A(CM_CTIMERCLKSEL2, 4),       /*!< Attach FRO 12MHz to CTIMER2. */
    kSAI0_MCLK_IN_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 5),       /*!< Attach SAI0 MCLK IN to CTIMER2. */
    kLPOSC_to_CTIMER2        = MUX_A(CM_CTIMERCLKSEL2, 6),       /*!< Attach LP Oscillator to CTIMER2. */
    kSAI1_MCLK_IN_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 8),       /*!< Attach SAI1 MCLK IN to CTIMER2. */
    kSAI0_TX_BCLK_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 9),       /*!< Attach SAI0 TX_BCLK to CTIMER2. */
    kSAI0_RX_BCLK_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 10),      /*!< Attach SAI0 RX_BCLK to CTIMER2. */
    kSAI1_TX_BCLK_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 11),      /*!< Attach SAI1 TX_BCLK to CTIMER2. */
    kSAI1_RX_BCLK_to_CTIMER2 = MUX_A(CM_CTIMERCLKSEL2, 12),      /*!< Attach SAI1 RX_BCLK to CTIMER2. */
    kNONE_to_CTIMER2         = MUX_A(CM_CTIMERCLKSEL2, 15),      /*!< Attach NONE to CTIMER2. */

    kCLK_1M_to_CTIMER3       = MUX_A(CM_CTIMERCLKSEL3, 0),       /*!< Attach CLK_1M to CTIMER3. */
    kPLL0_to_CTIMER3         = MUX_A(CM_CTIMERCLKSEL3, 1),       /*!< Attach PLL0 to CTIMER3. */
    kPLL1_CLK0_to_CTIMER3    = MUX_A(CM_CTIMERCLKSEL3, 2),       /*!< Attach PLL1_clk0 to CTIMER3. */
    kFRO_HF_to_CTIMER3       = MUX_A(CM_CTIMERCLKSEL3, 3),       /*!< Attach FRO_HF to CTIMER3. */
    kFRO12M_to_CTIMER3       = MUX_A(CM_CTIMERCLKSEL3, 4),       /*!< Attach FRO 12MHz to CTIMER3. */
    kSAI0_MCLK_IN_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 5),       /*!< Attach SAI0 MCLK IN to CTIMER3. */
    kLPOSC_to_CTIMER3        = MUX_A(CM_CTIMERCLKSEL3, 6),       /*!< Attach LP Oscillator to CTIMER3. */
    kSAI1_MCLK_IN_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 8),       /*!< Attach SAI1 MCLK IN to CTIMER3. */
    kSAI0_TX_BCLK_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 9),       /*!< Attach SAI0 TX_BCLK to CTIMER3. */
    kSAI0_RX_BCLK_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 10),      /*!< Attach SAI0 RX_BCLK to CTIMER3. */
    kSAI1_TX_BCLK_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 11),      /*!< Attach SAI1 TX_BCLK to CTIMER3. */
    kSAI1_RX_BCLK_to_CTIMER3 = MUX_A(CM_CTIMERCLKSEL3, 12),      /*!< Attach SAI1 RX_BCLK to CTIMER3. */
    kNONE_to_CTIMER3         = MUX_A(CM_CTIMERCLKSEL3, 15),      /*!< Attach NONE to CTIMER3. */

    kCLK_1M_to_CTIMER4       = MUX_A(CM_CTIMERCLKSEL4, 0),       /*!< Attach CLK_1M to CTIMER4. */
    kPLL0_to_CTIMER4         = MUX_A(CM_CTIMERCLKSEL4, 1),       /*!< Attach PLL0 to CTIMER4. */
    kPLL1_CLK0_to_CTIMER4    = MUX_A(CM_CTIMERCLKSEL4, 2),       /*!< Attach PLL1_clk0 to CTIMER4. */
    kFRO_HF_to_CTIMER4       = MUX_A(CM_CTIMERCLKSEL4, 3),       /*!< Attach FRO_HF to CTIMER4. */
    kFRO12M_to_CTIMER4       = MUX_A(CM_CTIMERCLKSEL4, 4),       /*!< Attach FRO 12MHz to CTIMER4. */
    kSAI0_MCLK_IN_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 5),       /*!< Attach SAI0 MCLK IN to CTIMER4. */
    kLPOSC_to_CTIMER4        = MUX_A(CM_CTIMERCLKSEL4, 6),       /*!< Attach LP Oscillator to CTIMER4. */
    kSAI1_MCLK_IN_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 8),       /*!< Attach SAI1 MCLK IN to CTIMER4. */
    kSAI0_TX_BCLK_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 9),       /*!< Attach SAI0 TX_BCLK to CTIMER4. */
    kSAI0_RX_BCLK_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 10),      /*!< Attach SAI0 RX_BCLK to CTIMER4. */
    kSAI1_TX_BCLK_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 11),      /*!< Attach SAI1 TX_BCLK to CTIMER4. */
    kSAI1_RX_BCLK_to_CTIMER4 = MUX_A(CM_CTIMERCLKSEL4, 12),      /*!< Attach SAI1 RX_BCLK to CTIMER4. */
    kNONE_to_CTIMER4         = MUX_A(CM_CTIMERCLKSEL4, 15),      /*!< Attach NONE to CTIMER4. */

    kMAIN_CLK_to_CLKOUT  = MUX_A(CM_CLKOUTCLKSEL, 0),            /*!< Attach MAIN_CLK to CLKOUT. */
    kPLL0_to_CLKOUT      = MUX_A(CM_CLKOUTCLKSEL, 1),            /*!< Attach PLL0 to CLKOUT. */
    kCLK_IN_to_CLKOUT    = MUX_A(CM_CLKOUTCLKSEL, 2),            /*!< Attach Clk_in to CLKOUT. */
    kFRO_HF_to_CLKOUT    = MUX_A(CM_CLKOUTCLKSEL, 3),            /*!< Attach FRO_HF to CLKOUT. */
    kFRO12M_to_CLKOUT    = MUX_A(CM_CLKOUTCLKSEL, 4),            /*!< Attach FRO 12 MHz to CLKOUT. */
    kPLL1_CLK0_to_CLKOUT = MUX_A(CM_CLKOUTCLKSEL, 5),            /*!< Attach PLL1_clk0 to CLKOUT. */
    kLPOSC_to_CLKOUT     = MUX_A(CM_CLKOUTCLKSEL, 6),            /*!< Attach LP Oscillator to CLKOUT. */
    kUSB_PLL_to_CLKOUT   = MUX_A(CM_CLKOUTCLKSEL, 7),            /*!< Attach USB_PLL to CLKOUT. */
    kNONE_to_CLKOUT      = MUX_A(CM_CLKOUTCLKSEL, 15),           /*!< Attach NONE to CLKOUT. */

    kPLL0_to_ADC0      = MUX_A(CM_ADC0CLKSEL, 1),                /*!< Attach PLL0 to ADC0. */
    kFRO_HF_to_ADC0    = MUX_A(CM_ADC0CLKSEL, 2),                /*!< Attach FRO_HF to ADC0. */
    kFRO12M_to_ADC0    = MUX_A(CM_ADC0CLKSEL, 3),                /*!< Attach FRO 12 MHz to ADC0. */
    kCLK_IN_to_ADC0    = MUX_A(CM_ADC0CLKSEL, 4),                /*!< Attach Clk_in to ADC0. */
    kPLL1_CLK0_to_ADC0 = MUX_A(CM_ADC0CLKSEL, 5),                /*!< Attach PLL1_clk0 to ADC0. */
    kUSB_PLL_to_ADC0   = MUX_A(CM_ADC0CLKSEL, 6),                /*!< Attach USB PLL to ADC0. */
    kNONE_to_ADC0      = MUX_A(CM_ADC0CLKSEL, 7),                /*!< Attach NONE to ADC0. */

    kPLL0_to_USB0      = MUX_A(CM_USB0CLKSEL, 1),                /*!< Attach PLL0 to USB0. */
    kCLK_48M_to_USB0   = MUX_A(CM_USB0CLKSEL, 3),                /*!< Attach Clk 48 MHz to USB0. */
    kCLK_IN_to_USB0    = MUX_A(CM_USB0CLKSEL, 4),                /*!< Attach Clk_in to USB0. */
    kPLL1_CLK0_to_USB0 = MUX_A(CM_USB0CLKSEL, 5),                /*!< Attach PLL1_clk0 to USB0. */
    kUSB_PLL_to_USB0   = MUX_A(CM_USB0CLKSEL, 6),                /*!< Attach USB PLL to USB0. */
    kNONE_to_USB0      = MUX_A(CM_USB0CLKSEL, 7),                /*!< Attach NONE to USB0. */

    kPLL_DIV_to_FLEXCOMM0    = MUX_A(CM_FCCLKSEL0, 1),           /*!< Attach PLL_DIV to FLEXCOMM0. */
    kFRO12M_to_FLEXCOMM0     = MUX_A(CM_FCCLKSEL0, 2),           /*!< Attach FRO12M to FLEXCOMM0. */
    kFRO_HF_DIV_to_FLEXCOMM0 = MUX_A(CM_FCCLKSEL0, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM0. */
    kCLK_1M_to_FLEXCOMM0     = MUX_A(CM_FCCLKSEL0, 4),           /*!< Attach CLK_1MHz to FLEXCOMM0. */
    kUSB_PLL_to_FLEXCOMM0    = MUX_A(CM_FCCLKSEL0, 5),           /*!< Attach USB_PLL to FLEXCOMM0. */
    kLPOSC_to_FLEXCOMM0      = MUX_A(CM_FCCLKSEL0, 6),           /*!< Attach LP Oscillator to FLEXCOMM0. */
    kNONE_to_FLEXCOMM0       = MUX_A(CM_FCCLKSEL0, 7),           /*!< Attach NONE to FLEXCOMM0. */

    kPLL_DIV_to_FLEXCOMM1    = MUX_A(CM_FCCLKSEL1, 1),           /*!< Attach PLL_DIV to FLEXCOMM1. */
    kFRO12M_to_FLEXCOMM1     = MUX_A(CM_FCCLKSEL1, 2),           /*!< Attach FRO12M to FLEXCOMM1. */
    kFRO_HF_DIV_to_FLEXCOMM1 = MUX_A(CM_FCCLKSEL1, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM1. */
    kCLK_1M_to_FLEXCOMM1     = MUX_A(CM_FCCLKSEL1, 4),           /*!< Attach CLK_1MHz to FLEXCOMM1. */
    kUSB_PLL_to_FLEXCOMM1    = MUX_A(CM_FCCLKSEL1, 5),           /*!< Attach USB_PLL to FLEXCOMM1. */
    kLPOSC_to_FLEXCOMM1      = MUX_A(CM_FCCLKSEL1, 6),           /*!< Attach LP Oscillator to FLEXCOMM1. */
    kNONE_to_FLEXCOMM1       = MUX_A(CM_FCCLKSEL1, 7),           /*!< Attach NONE to FLEXCOMM1. */

    kPLL_DIV_to_FLEXCOMM2    = MUX_A(CM_FCCLKSEL2, 1),           /*!< Attach PLL_DIV to FLEXCOMM2. */
    kFRO12M_to_FLEXCOMM2     = MUX_A(CM_FCCLKSEL2, 2),           /*!< Attach FRO12M to FLEXCOMM2. */
    kFRO_HF_DIV_to_FLEXCOMM2 = MUX_A(CM_FCCLKSEL2, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM2. */
    kCLK_1M_to_FLEXCOMM2     = MUX_A(CM_FCCLKSEL2, 4),           /*!< Attach CLK_1MHz to FLEXCOMM2. */
    kUSB_PLL_to_FLEXCOMM2    = MUX_A(CM_FCCLKSEL2, 5),           /*!< Attach USB_PLL to FLEXCOMM2. */
    kLPOSC_to_FLEXCOMM2      = MUX_A(CM_FCCLKSEL2, 6),           /*!< Attach LP Oscillator to FLEXCOMM2. */
    kNONE_to_FLEXCOMM2       = MUX_A(CM_FCCLKSEL2, 7),           /*!< Attach NONE to FLEXCOMM2. */

    kPLL_DIV_to_FLEXCOMM3    = MUX_A(CM_FCCLKSEL3, 1),           /*!< Attach PLL_DIV to FLEXCOMM3. */
    kFRO12M_to_FLEXCOMM3     = MUX_A(CM_FCCLKSEL3, 2),           /*!< Attach FRO12M to FLEXCOMM3. */
    kFRO_HF_DIV_to_FLEXCOMM3 = MUX_A(CM_FCCLKSEL3, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM3. */
    kCLK_1M_to_FLEXCOMM3     = MUX_A(CM_FCCLKSEL3, 4),           /*!< Attach CLK_1MHz to FLEXCOMM3. */
    kUSB_PLL_to_FLEXCOMM3    = MUX_A(CM_FCCLKSEL3, 5),           /*!< Attach USB_PLL to FLEXCOMM3. */
    kLPOSC_to_FLEXCOMM3      = MUX_A(CM_FCCLKSEL3, 6),           /*!< Attach LP Oscillator to FLEXCOMM3. */
    kNONE_to_FLEXCOMM3       = MUX_A(CM_FCCLKSEL3, 7),           /*!< Attach NONE to FLEXCOMM3. */

    kPLL_DIV_to_FLEXCOMM4    = MUX_A(CM_FCCLKSEL4, 1),           /*!< Attach PLL_DIV to FLEXCOMM4. */
    kFRO12M_to_FLEXCOMM4     = MUX_A(CM_FCCLKSEL4, 2),           /*!< Attach FRO12M to FLEXCOMM4. */
    kFRO_HF_DIV_to_FLEXCOMM4 = MUX_A(CM_FCCLKSEL4, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM4. */
    kCLK_1M_to_FLEXCOMM4     = MUX_A(CM_FCCLKSEL4, 4),           /*!< Attach CLK_1MHz to FLEXCOMM4. */
    kUSB_PLL_to_FLEXCOMM4    = MUX_A(CM_FCCLKSEL4, 5),           /*!< Attach USB_PLL to FLEXCOMM4. */
    kLPOSC_to_FLEXCOMM4      = MUX_A(CM_FCCLKSEL4, 6),           /*!< Attach LP Oscillator to FLEXCOMM4. */
    kNONE_to_FLEXCOMM4       = MUX_A(CM_FCCLKSEL4, 7),           /*!< Attach NONE to FLEXCOMM4. */

    kPLL_DIV_to_FLEXCOMM5    = MUX_A(CM_FCCLKSEL5, 1),           /*!< Attach PLL_DIV to FLEXCOMM5. */
    kFRO12M_to_FLEXCOMM5     = MUX_A(CM_FCCLKSEL5, 2),           /*!< Attach FRO12M to FLEXCOMM5. */
    kFRO_HF_DIV_to_FLEXCOMM5 = MUX_A(CM_FCCLKSEL5, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM5. */
    kCLK_1M_to_FLEXCOMM5     = MUX_A(CM_FCCLKSEL5, 4),           /*!< Attach CLK_1MHz to FLEXCOMM5. */
    kUSB_PLL_to_FLEXCOMM5    = MUX_A(CM_FCCLKSEL5, 5),           /*!< Attach USB_PLL to FLEXCOMM5. */
    kLPOSC_to_FLEXCOMM5      = MUX_A(CM_FCCLKSEL5, 6),           /*!< Attach LP Oscillator to FLEXCOMM5. */
    kNONE_to_FLEXCOMM5       = MUX_A(CM_FCCLKSEL5, 7),           /*!< Attach NONE to FLEXCOMM5. */

    kPLL_DIV_to_FLEXCOMM6    = MUX_A(CM_FCCLKSEL6, 1),           /*!< Attach PLL_DIV to FLEXCOMM6. */
    kFRO12M_to_FLEXCOMM6     = MUX_A(CM_FCCLKSEL6, 2),           /*!< Attach FRO12M to FLEXCOMM6. */
    kFRO_HF_DIV_to_FLEXCOMM6 = MUX_A(CM_FCCLKSEL6, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM6. */
    kCLK_1M_to_FLEXCOMM6     = MUX_A(CM_FCCLKSEL6, 4),           /*!< Attach CLK_1MHz to FLEXCOMM6. */
    kUSB_PLL_to_FLEXCOMM6    = MUX_A(CM_FCCLKSEL6, 5),           /*!< Attach USB_PLL to FLEXCOMM6. */
    kLPOSC_to_FLEXCOMM6      = MUX_A(CM_FCCLKSEL6, 6),           /*!< Attach LP Oscillator to FLEXCOMM6. */
    kNONE_to_FLEXCOMM6       = MUX_A(CM_FCCLKSEL6, 7),           /*!< Attach NONE to FLEXCOMM6. */

    kPLL_DIV_to_FLEXCOMM7    = MUX_A(CM_FCCLKSEL7, 1),           /*!< Attach PLL_DIV to FLEXCOMM7. */
    kFRO12M_to_FLEXCOMM7     = MUX_A(CM_FCCLKSEL7, 2),           /*!< Attach FRO12M to FLEXCOMM7. */
    kFRO_HF_DIV_to_FLEXCOMM7 = MUX_A(CM_FCCLKSEL7, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM7. */
    kCLK_1M_to_FLEXCOMM7     = MUX_A(CM_FCCLKSEL7, 4),           /*!< Attach CLK_1MHz to FLEXCOMM7. */
    kUSB_PLL_to_FLEXCOMM7    = MUX_A(CM_FCCLKSEL7, 5),           /*!< Attach USB_PLL to FLEXCOMM7. */
    kLPOSC_to_FLEXCOMM7      = MUX_A(CM_FCCLKSEL7, 6),           /*!< Attach LP Oscillator to FLEXCOMM7. */
    kNONE_to_FLEXCOMM7       = MUX_A(CM_FCCLKSEL7, 7),           /*!< Attach NONE to FLEXCOMM7. */

    kPLL_DIV_to_FLEXCOMM8    = MUX_A(CM_FCCLKSEL8, 1),           /*!< Attach PLL_DIV to FLEXCOMM8. */
    kFRO12M_to_FLEXCOMM8     = MUX_A(CM_FCCLKSEL8, 2),           /*!< Attach FRO12M to FLEXCOMM8. */
    kFRO_HF_DIV_to_FLEXCOMM8 = MUX_A(CM_FCCLKSEL8, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM8. */
    kCLK_1M_to_FLEXCOMM8     = MUX_A(CM_FCCLKSEL8, 4),           /*!< Attach CLK_1MHz to FLEXCOMM8. */
    kUSB_PLL_to_FLEXCOMM8    = MUX_A(CM_FCCLKSEL8, 5),           /*!< Attach USB_PLL to FLEXCOMM8. */
    kLPOSC_to_FLEXCOMM8      = MUX_A(CM_FCCLKSEL8, 6),           /*!< Attach LP Oscillator to FLEXCOMM8. */
    kNONE_to_FLEXCOMM8       = MUX_A(CM_FCCLKSEL8, 7),           /*!< Attach NONE to FLEXCOMM8. */

    kPLL_DIV_to_FLEXCOMM9    = MUX_A(CM_FCCLKSEL9, 1),           /*!< Attach PLL_DIV to FLEXCOMM9. */
    kFRO12M_to_FLEXCOMM9     = MUX_A(CM_FCCLKSEL9, 2),           /*!< Attach FRO12M to FLEXCOMM9. */
    kFRO_HF_DIV_to_FLEXCOMM9 = MUX_A(CM_FCCLKSEL9, 3),           /*!< Attach FRO_HF_DIV to FLEXCOMM9. */
    kCLK_1M_to_FLEXCOMM9     = MUX_A(CM_FCCLKSEL9, 4),           /*!< Attach CLK_1MHz to FLEXCOMM9. */
    kUSB_PLL_to_FLEXCOMM9    = MUX_A(CM_FCCLKSEL9, 5),           /*!< Attach USB_PLL to FLEXCOMM9. */
    kLPOSC_to_FLEXCOMM9      = MUX_A(CM_FCCLKSEL9, 6),           /*!< Attach LP Oscillator to FLEXCOMM9. */
    kNONE_to_FLEXCOMM9       = MUX_A(CM_FCCLKSEL9, 7),           /*!< Attach NONE to FLEXCOMM9. */

    kPLL0_to_SCT         = MUX_A(CM_SCTCLKSEL, 1),               /*!< Attach NONE to SCT. */
    kCLK_IN_to_SCT       = MUX_A(CM_SCTCLKSEL, 2),               /*!< Attach CLK_in to SCT. */
    kFRO_HF_to_SCT       = MUX_A(CM_SCTCLKSEL, 3),               /*!< Attach FRO_HF to SCT. */
    kPLL1_CLK0_to_SCT    = MUX_A(CM_SCTCLKSEL, 4),               /*!< Attach PLL1_clk0 to SCT. */
    kSAI0_MCLK_IN_to_SCT = MUX_A(CM_SCTCLKSEL, 5),               /*!< Attach SAI0 MCLK_IN to SCT. */
    kUSB_PLL_to_SCT      = MUX_A(CM_SCTCLKSEL, 6),               /*!< Attach USB PLL to SCT. */
    kSAI1_MCLK_IN_to_SCT = MUX_A(CM_SCTCLKSEL, 8),               /*!< Attach SAI1 MCLK_IN to SCT. */
    kNONE_to_SCT         = MUX_A(CM_SCTCLKSEL, 15),              /*!< Attach NONE to SCT. */

    kCLK_IN_to_TSI = MUX_A(CM_TSICLKSEL, 2),                     /*!< Attach clk_in to TSI. */
    kFRO12M_to_TSI = MUX_A(CM_TSICLKSEL, 4),                     /*!< Attach FRO_12Mhz to TSI. */
    kNONE_to_TSI   = MUX_A(CM_TSICLKSEL, 7),                     /*!< Attach NONE to TSI. */

    kPLL0_to_SINCFILT      = MUX_A(CM_SINCFILTCLKSEL, 1),        /*!< Attach PLL0 to SINCFILT. */
    kCLK_IN_to_SINCFILT    = MUX_A(CM_SINCFILTCLKSEL, 2),        /*!< Attach clk_in to SINCFILT. */
    kFRO_HF_to_SINCFILT    = MUX_A(CM_SINCFILTCLKSEL, 3),        /*!< Attach FRO_HF to SINCFILT. */
    kFRO12M_to_SINCFILT    = MUX_A(CM_SINCFILTCLKSEL, 4),        /*!< Attach FRO_12Mhz to SINCFILT. */
    kPLL1_CLK0_to_SINCFILT = MUX_A(CM_SINCFILTCLKSEL, 5),        /*!< Attach PLL1_clk0 to SINCFILT. */
    kUSB_PLL_to_SINCFILT   = MUX_A(CM_SINCFILTCLKSEL, 6),        /*!< Attach USB PLL to SINCFILT. */
    kNONE_to_SINCFILT      = MUX_A(CM_SINCFILTCLKSEL, 7),        /*!< Attach NONE to SINCFILT. */

    kPLL0_to_ADC1      = MUX_A(CM_ADC1CLKSEL, 1),                /*!< Attach PLL0 to ADC1. */
    kFRO_HF_to_ADC1    = MUX_A(CM_ADC1CLKSEL, 2),                /*!< Attach FRO_HF to ADC1. */
    kFRO12M_to_ADC1    = MUX_A(CM_ADC1CLKSEL, 3),                /*!< Attach FRO12M to ADC1. */
    kCLK_IN_to_ADC1    = MUX_A(CM_ADC1CLKSEL, 4),                /*!< Attach clk_in to ADC1. */
    kPLL1_CLK0_to_ADC1 = MUX_A(CM_ADC1CLKSEL, 5),                /*!< Attach PLL1_clk0 to ADC1. */
    kUSB_PLL_to_ADC1   = MUX_A(CM_ADC1CLKSEL, 6),                /*!< Attach USB PLL to ADC1. */
    kNONE_to_ADC1      = MUX_A(CM_ADC1CLKSEL, 7),                /*!< Attach NONE to ADC1. */

    kPLL0_to_DAC0      = MUX_A(CM_DAC0CLKSEL, 1),                /*!< Attach PLL0 to DAC0. */
    kCLK_IN_to_DAC0    = MUX_A(CM_DAC0CLKSEL, 2),                /*!< Attach Clk_in to DAC0. */
    kFRO_HF_to_DAC0    = MUX_A(CM_DAC0CLKSEL, 3),                /*!< Attach FRO_HF to DAC0. */
    kFRO12M_to_DAC0    = MUX_A(CM_DAC0CLKSEL, 4),                /*!< Attach FRO_12M to DAC0. */
    kPLL1_CLK0_to_DAC0 = MUX_A(CM_DAC0CLKSEL, 5),                /*!< Attach PLL1_clk0 to DAC0. */
    kNONE_to_DAC0      = MUX_A(CM_DAC0CLKSEL, 7),                /*!< Attach NONE to DAC0. */

    kPLL0_to_DAC1      = MUX_A(CM_DAC1CLKSEL, 1),                /*!< Attach PLL0 to DAC1. */
    kCLK_IN_to_DAC1    = MUX_A(CM_DAC1CLKSEL, 2),                /*!< Attach Clk_in to DAC1. */
    kFRO_HF_to_DAC1    = MUX_A(CM_DAC1CLKSEL, 3),                /*!< Attach FRO_HF to DAC1. */
    kFRO12M_to_DAC1    = MUX_A(CM_DAC1CLKSEL, 4),                /*!< Attach FRO_12M to DAC1. */
    kPLL1_CLK0_to_DAC1 = MUX_A(CM_DAC1CLKSEL, 5),                /*!< Attach PLL1_clk0 to DAC1. */
    kNONE_to_DAC1      = MUX_A(CM_DAC1CLKSEL, 7),                /*!< Attach NONE to DAC1. */

    kPLL0_to_DAC2      = MUX_A(CM_DAC2CLKSEL, 1),                /*!< Attach PLL0 to DAC2. */
    kCLK_IN_to_DAC2    = MUX_A(CM_DAC2CLKSEL, 2),                /*!< Attach Clk_in to DAC2. */
    kFRO_HF_to_DAC2    = MUX_A(CM_DAC2CLKSEL, 3),                /*!< Attach FRO_HF to DAC2. */
    kFRO12M_to_DAC2    = MUX_A(CM_DAC2CLKSEL, 4),                /*!< Attach FRO_12M to DAC2. */
    kPLL1_CLK0_to_DAC2 = MUX_A(CM_DAC2CLKSEL, 5),                /*!< Attach PLL1_clk0 to DAC2. */
    kNONE_to_DAC2      = MUX_A(CM_DAC2CLKSEL, 7),                /*!< Attach NONE to DAC2. */

    kPLL0_to_FLEXSPI    = MUX_A(CM_FLEXSPICLKSEL, 1),            /*!< Attach PLL0 to FLEXSPI. */
    kFRO_HF_to_FLEXSPI  = MUX_A(CM_FLEXSPICLKSEL, 3),            /*!< Attach FRO_HF to FLEXSPI. */
    kPLL1_to_FLEXSPI    = MUX_A(CM_FLEXSPICLKSEL, 5),            /*!< Attach PLL1 to FLEXSPI. */
    kUSB_PLL_to_FLEXSPI = MUX_A(CM_FLEXSPICLKSEL, 6),            /*!< Attach USB PLL to FLEXSPI. */
    kNONE_to_FLEXSPI    = MUX_A(CM_FLEXSPICLKSEL, 15),           /*!< Attach NONE to FLEXSPI. */

    kPLL0_to_PLLCLKDIV      = MUX_A(CM_PLLCLKDIVSEL, 0),         /*!< Attach PLL0 to PLLCLKDIV. */
    kPLL1_CLK0_to_PLLCLKDIV = MUX_A(CM_PLLCLKDIVSEL, 1),         /*!< Attach pll1_clk0 to PLLCLKDIV. */
    kNONE_to_PLLCLKDIV      = MUX_A(CM_PLLCLKDIVSEL, 1),         /*!< Attach NONE to PLLCLKDIV. */

    kPLL0_to_I3C0FCLK      = MUX_A(CM_I3C0FCLKSEL, 1),           /*!< Attach PLL0 to I3C0FCLK. */
    kCLK_IN_to_I3C0FCLK    = MUX_A(CM_I3C0FCLKSEL, 2),           /*!< Attach Clk_in to I3C0FCLK. */
    kFRO_HF_to_I3C0FCLK    = MUX_A(CM_I3C0FCLKSEL, 3),           /*!< Attach FRO_HF to I3C0FCLK. */
    kPLL1_CLK0_to_I3C0FCLK = MUX_A(CM_I3C0FCLKSEL, 5),           /*!< Attach PLL1_clk0 to I3C0FCLK. */
    kUSB_PLL_to_I3C0FCLK   = MUX_A(CM_I3C0FCLKSEL, 6),           /*!< Attach USB PLL to I3C0FCLK. */
    kNONE_to_I3C0FCLK      = MUX_A(CM_I3C0FCLKSEL, 7),           /*!< Attach NONE to I3C0FCLK. */

    kI3C0FCLK_to_I3C0FCLKSTC = MUX_A(CM_I3C0FCLKSTCSEL, 0),      /*!< Attach I3C0FCLK to I3C0FCLKSTC. */
    kCLK_1M_to_I3C0FCLKSTC   = MUX_A(CM_I3C0FCLKSTCSEL, 1),      /*!< Attach CLK_1M to I3C0FCLKSTC. */
    kNONE_to_I3C0FCLKSTC     = MUX_A(CM_I3C0FCLKSTCSEL, 7),      /*!< Attach NONE to I3C0FCLKSTC. */

    kCLK_1M_to_I3C0FCLKS = MUX_A(CM_I3C0FCLKSSEL, 0),            /*!< Attach CLK_1M to I3C0FCLKS. */
    kNONE_to_I3C0FCLKS   = MUX_A(CM_I3C0FCLKSSEL, 7),            /*!< Attach NONE to I3C0FCLKS. */

    kFRO12M_to_MICFILF       = MUX_A(CM_MICFILFCLKSEL, 0),       /*!< Attach FRO_12M to MICFILF. */
    kPLL0_to_MICFILF         = MUX_A(CM_MICFILFCLKSEL, 1),       /*!< Attach PLL0 to MICFILF. */
    kCLK_IN_to_MICFILF       = MUX_A(CM_MICFILFCLKSEL, 2),       /*!< Attach Clk_in to MICFILF. */
    kFRO_HF_to_MICFILF       = MUX_A(CM_MICFILFCLKSEL, 3),       /*!< Attach FRO_HF to MICFILF. */
    kPLL1_CLK0_to_MICFILF    = MUX_A(CM_MICFILFCLKSEL, 4),       /*!< Attach PLL1_clk0 to MICFILF. */
    kSAI0_MCLK_IN_to_MICFILF = MUX_A(CM_MICFILFCLKSEL, 5),       /*!< Attach SAI0_MCLK to MICFILF. */
    kUSB_PLL_to_MICFILF      = MUX_A(CM_MICFILFCLKSEL, 6),       /*!< Attach USB PLL to MICFILF. */
    kSAI1_MCLK_IN_to_MICFILF = MUX_A(CM_MICFILFCLKSEL, 8),       /*!< Attach SAI1_MCLK to MICFILF. */
    kNONE_to_MICFILF         = MUX_A(CM_MICFILFCLKSEL, 15),      /*!< Attach NONE to MICFILF. */

    kPLL0_to_ESPI      = MUX_A(CM_ESPICLKSEL, 1),                /*!< Attach PLL0 to ESPI. */
    kCLK_48M_to_ESPI   = MUX_A(CM_ESPICLKSEL, 3),                /*!< Attach CLK_48M to ESPI. */
    kPLL1_CLK0_to_ESPI = MUX_A(CM_ESPICLKSEL, 5),                /*!< Attach PLL1_clk0 to ESPI. */
    kUSB_PLL_to_ESPI   = MUX_A(CM_ESPICLKSEL, 6),                /*!< Attach USB PLL to ESPI. */
    kNONE_to_ESPI      = MUX_A(CM_ESPICLKSEL, 7),                /*!< Attach NONE to ESPI. */

    kPLL0_to_USDHC      = MUX_A(CM_USDHCCLKSEL, 1),              /*!< Attach PLL0 to uSDHC. */
    kCLK_IN_to_USDHC    = MUX_A(CM_USDHCCLKSEL, 2),              /*!< Attach Clk_in to uSDHC. */
    kFRO_HF_to_USDHC    = MUX_A(CM_USDHCCLKSEL, 3),              /*!< Attach FRO_HF to uSDHC. */
    kFRO12M_to_USDHC    = MUX_A(CM_USDHCCLKSEL, 4),              /*!< Attach FRO_12M to uSDHC. */
    kPLL1_CLK1_to_USDHC = MUX_A(CM_USDHCCLKSEL, 5),              /*!< Attach pll1_clk1 to uSDHC. */
    kUSB_PLL_to_USDHC   = MUX_A(CM_USDHCCLKSEL, 6),              /*!< Attach USB PLL to uSDHC. */
    kNONE_to_USDHC      = MUX_A(CM_USDHCCLKSEL, 7),              /*!< Attach NONE to uSDHC. */

    kPLL0_to_FLEXIO      = MUX_A(CM_FLEXIOCLKSEL, 1),            /*!< Attach PLL0 to FLEXIO. */
    kCLK_IN_to_FLEXIO    = MUX_A(CM_FLEXIOCLKSEL, 2),            /*!< Attach Clk_in to FLEXIO. */
    kFRO_HF_to_FLEXIO    = MUX_A(CM_FLEXIOCLKSEL, 3),            /*!< Attach FRO_HF to FLEXIO. */
    kFRO12M_to_FLEXIO    = MUX_A(CM_FLEXIOCLKSEL, 4),            /*!< Attach FRO_12M to FLEXIO. */
    kPLL1_CLK0_to_FLEXIO = MUX_A(CM_FLEXIOCLKSEL, 5),            /*!< Attach pll1_clk0 to FLEXIO. */
    kUSB_PLL_to_FLEXIO   = MUX_A(CM_FLEXIOCLKSEL, 6),            /*!< Attach USB PLL to FLEXIO. */
    kNONE_to_FLEXIO      = MUX_A(CM_FLEXIOCLKSEL, 7),            /*!< Attach NONE to FLEXIO. */

    kPLL0_to_FLEXCAN0      = MUX_A(CM_FLEXCAN0CLKSEL, 1),        /*!< Attach PLL0 to FLEXCAN0. */
    kCLK_IN_to_FLEXCAN0    = MUX_A(CM_FLEXCAN0CLKSEL, 2),        /*!< Attach Clk_in to FLEXCAN0. */
    kFRO_HF_to_FLEXCAN0    = MUX_A(CM_FLEXCAN0CLKSEL, 3),        /*!< Attach FRO_HF to FLEXCAN0. */
    kPLL1_CLK0_to_FLEXCAN0 = MUX_A(CM_FLEXCAN0CLKSEL, 5),        /*!< Attach pll1_clk0 to FLEXCAN0. */
    kUSB_PLL_to_FLEXCAN0   = MUX_A(CM_FLEXCAN0CLKSEL, 6),        /*!< Attach USB PLL to FLEXCAN0. */
    kNONE_to_FLEXCAN0      = MUX_A(CM_FLEXCAN0CLKSEL, 7),        /*!< Attach NONE to FLEXCAN0. */

    kPLL0_to_FLEXCAN1      = MUX_A(CM_FLEXCAN1CLKSEL, 1),        /*!< Attach PLL0 to FLEXCAN1. */
    kCLK_IN_to_FLEXCAN1    = MUX_A(CM_FLEXCAN1CLKSEL, 2),        /*!< Attach Clk_in to FLEXCAN1. */
    kFRO_HF_to_FLEXCAN1    = MUX_A(CM_FLEXCAN1CLKSEL, 3),        /*!< Attach FRO_HF to FLEXCAN1. */
    kPLL1_CLK0_to_FLEXCAN1 = MUX_A(CM_FLEXCAN1CLKSEL, 5),        /*!< Attach pll1_clk0 to FLEXCAN1. */
    kUSB_PLL_to_FLEXCAN1   = MUX_A(CM_FLEXCAN1CLKSEL, 6),        /*!< Attach USB PLL to FLEXCAN1. */
    kNONE_to_FLEXCAN1      = MUX_A(CM_FLEXCAN1CLKSEL, 7),        /*!< Attach NONE to FLEXCAN1. */

    kNONE_to_ENETRMII      = MUX_A(CM_ENETRMIICLKSEL, 0),        /*!< Attach NONE to ENETRMII. */
    kPLL0_to_ENETRMII      = MUX_A(CM_ENETRMIICLKSEL, 1),        /*!< Attach PLL0 to ENETRMII. */
    kCLK_IN_to_ENETRMII    = MUX_A(CM_ENETRMIICLKSEL, 2),        /*!< Attach Clk_in to ENETRMII. */
    kPLL1_CLK0_to_ENETRMII = MUX_A(CM_ENETRMIICLKSEL, 5),        /*!< Attach pll1_clk0 to ENETRMII. */

    kPLL0_to_ENETPTPREF         = MUX_A(CM_ENETPTPREFCLKSEL, 1), /*!< Attach PLL0 to ENETPTPREF. */
    kCLK_IN_to_ENETPTPREF       = MUX_A(CM_ENETPTPREFCLKSEL, 2), /*!< Attach Clk_in to ENETPTPREF. */
    kENET0_TX_CLK_to_ENETPTPREF = MUX_A(CM_ENETPTPREFCLKSEL, 4), /*!< Attach enet0_tx_clk to ENETPTPREF. */
    kPLL1_CLK1_to_ENETPTPREF    = MUX_A(CM_ENETPTPREFCLKSEL, 5), /*!< Attach pll1_clk1 to ENETPTPREF. */
    kNONE_to_ENETPTPREF         = MUX_A(CM_ENETPTPREFCLKSEL, 7), /*!< Attach NONE to ENETPTPREF. */

    kCLK_16K2_to_EWM0 = MUX_A(CM_EWM0CLKSEL, 0),                 /*!< Attach clk_16k[2] to EWM0. */
    kXTAL32K2_to_EWM0 = MUX_A(CM_EWM0CLKSEL, 1),                 /*!< Attach xtal32k[2] to EWM0. */

    kCLK_16K2_to_WDT1   = MUX_A(CM_WDT1CLKSEL, 0),               /*!< Attach FRO16K clock 2 to WDT1. */
    kFRO_HF_DIV_to_WDT1 = MUX_A(CM_WDT1CLKSEL, 1),               /*!< Attach FRO_HF_DIV to WDT1. */
    kCLK_1M_to_WDT1     = MUX_A(CM_WDT1CLKSEL, 2),               /*!< Attach clk_1m to WDT1. */
    kNONE_to_WDT1       = MUX_A(CM_WDT1CLKSEL, 3),               /*!< Attach NONE to WDT1. */

    kCLK_16K2_to_OSTIMER = MUX_A(CM_OSTIMERCLKSEL, 0),           /*!< Attach clk_16k[2] to OSTIMER. */
    kXTAL32K2_to_OSTIMER = MUX_A(CM_OSTIMERCLKSEL, 1),           /*!< Attach xtal32k[2] to OSTIMER. */
    kCLK_1M_to_OSTIMER   = MUX_A(CM_OSTIMERCLKSEL, 2),           /*!< Attach clk_1m to OSTIMER. */
    kNONE_to_OSTIMER     = MUX_A(CM_OSTIMERCLKSEL, 3),           /*!< Attach NONE to OSTIMER. */

    kPLL0_to_CMP0F      = MUX_A(CM_CMP0FCLKSEL, 1),              /*!< Attach PLL0 to CMP0F. */
    kFRO_HF_to_CMP0F    = MUX_A(CM_CMP0FCLKSEL, 2),              /*!< Attach FRO_HF to CMP0F. */
    kFRO12M_to_CMP0F    = MUX_A(CM_CMP0FCLKSEL, 3),              /*!< Attach FRO_12M to CMP0F. */
    kCLK_IN_to_CMP0F    = MUX_A(CM_CMP0FCLKSEL, 4),              /*!< Attach Clk_in to CMP0F. */
    kPLL1_CLK0_to_CMP0F = MUX_A(CM_CMP0FCLKSEL, 5),              /*!< Attach PLL1_clk0 to CMP0F. */
    kUSB_PLL_to_CMP0F   = MUX_A(CM_CMP0FCLKSEL, 6),              /*!< Attach USB PLL to CMP0F. */
    kNONE_to_CMP0F      = MUX_A(CM_CMP0FCLKSEL, 7),              /*!< Attach NONE to CMP0F. */

    kPLL0_to_CMP0RR      = MUX_A(CM_CMP0RRCLKSEL, 1),            /*!< Attach PLL0 to CMP0RR. */
    kFRO_HF_to_CMP0RR    = MUX_A(CM_CMP0RRCLKSEL, 2),            /*!< Attach FRO_HF to CMP0RR. */
    kFRO12M_to_CMP0RR    = MUX_A(CM_CMP0RRCLKSEL, 3),            /*!< Attach FRO_12M to CMP0RR. */
    kCLK_IN_to_CMP0RR    = MUX_A(CM_CMP0RRCLKSEL, 4),            /*!< Attach Clk_in to CMP0RR. */
    kPLL1_CLK0_to_CMP0RR = MUX_A(CM_CMP0RRCLKSEL, 5),            /*!< Attach PLL1_clk0 to CMP0RR. */
    kUSB_PLL_to_CMP0RR   = MUX_A(CM_CMP0RRCLKSEL, 6),            /*!< Attach USB PLL to CMP0RR. */
    kNONE_to_CMP0RR      = MUX_A(CM_CMP0RRCLKSEL, 7),            /*!< Attach NONE to CMP0RR. */

    kPLL0_to_CMP1F      = MUX_A(CM_CMP1FCLKSEL, 1),              /*!< Attach PLL0 to CMP1F. */
    kFRO_HF_to_CMP1F    = MUX_A(CM_CMP1FCLKSEL, 2),              /*!< Attach FRO_HF to CMP1F. */
    kFRO12M_to_CMP1F    = MUX_A(CM_CMP1FCLKSEL, 3),              /*!< Attach FRO_12M to CMP1F. */
    kCLK_IN_to_CMP1F    = MUX_A(CM_CMP1FCLKSEL, 4),              /*!< Attach Clk_in to CMP1F. */
    kPLL1_CLK0_to_CMP1F = MUX_A(CM_CMP1FCLKSEL, 5),              /*!< Attach PLL1_clk0 to CMP1F. */
    kUSB_PLL_to_CMP1F   = MUX_A(CM_CMP1FCLKSEL, 6),              /*!< Attach USB PLL to CMP1F. */
    kNONE_to_CMP1F      = MUX_A(CM_CMP1FCLKSEL, 7),              /*!< Attach NONE to CMP1F. */

    kPLL0_to_CMP1RR      = MUX_A(CM_CMP1RRCLKSEL, 1),            /*!< Attach PLL0 to CMP1RR. */
    kFRO_HF_to_CMP1RR    = MUX_A(CM_CMP1RRCLKSEL, 2),            /*!< Attach FRO_HF to CMP1RR. */
    kFRO12M_to_CMP1RR    = MUX_A(CM_CMP1RRCLKSEL, 3),            /*!< Attach FRO_12M to CMP1RR. */
    kCLK_IN_to_CMP1RR    = MUX_A(CM_CMP1RRCLKSEL, 4),            /*!< Attach Clk_in to CMP1RR. */
    kPLL1_CLK0_to_CMP1RR = MUX_A(CM_CMP1RRCLKSEL, 5),            /*!< Attach PLL1_clk0 to CMP1RR. */
    kUSB_PLL_to_CMP1RR   = MUX_A(CM_CMP1RRCLKSEL, 6),            /*!< Attach USB PLL to CMP1RR. */
    kNONE_to_CMP1RR      = MUX_A(CM_CMP1RRCLKSEL, 7),            /*!< Attach NONE to CMP1RR. */

    kPLL0_to_CMP2F      = MUX_A(CM_CMP2FCLKSEL, 1),              /*!< Attach PLL0 to CMP2F. */
    kFRO_HF_to_CMP2F    = MUX_A(CM_CMP2FCLKSEL, 2),              /*!< Attach FRO_HF to CMP2F. */
    kFRO12M_to_CMP2F    = MUX_A(CM_CMP2FCLKSEL, 3),              /*!< Attach FRO_12M to CMP2F. */
    kCLK_IN_to_CMP2F    = MUX_A(CM_CMP2FCLKSEL, 4),              /*!< Attach Clk_in to CMP2F. */
    kPLL1_CLK0_to_CMP2F = MUX_A(CM_CMP2FCLKSEL, 5),              /*!< Attach PLL1_clk0 to CMP2F. */
    kUSB_PLL_to_CMP2F   = MUX_A(CM_CMP2FCLKSEL, 6),              /*!< Attach USB PLL to CMP2F. */
    kNONE_to_CMP2F      = MUX_A(CM_CMP2FCLKSEL, 7),              /*!< Attach NONE to CMP2F. */

    kPLL0_to_CMP2RR      = MUX_A(CM_CMP2RRCLKSEL, 1),            /*!< Attach PLL0 to CMP2RR. */
    kFRO_HF_to_CMP2RR    = MUX_A(CM_CMP2RRCLKSEL, 2),            /*!< Attach FRO_HF to CMP2RR. */
    kFRO12M_to_CMP2RR    = MUX_A(CM_CMP2RRCLKSEL, 3),            /*!< Attach FRO_12M to CMP2RR. */
    kCLK_IN_to_CMP2RR    = MUX_A(CM_CMP2RRCLKSEL, 4),            /*!< Attach Clk_in to CMP2RR. */
    kPLL1_CLK0_to_CMP2RR = MUX_A(CM_CMP2RRCLKSEL, 5),            /*!< Attach PLL1_clk0 to CMP2RR. */
    kUSB_PLL_to_CMP2RR   = MUX_A(CM_CMP2RRCLKSEL, 6),            /*!< Attach USB PLL to CMP2RR. */
    kNONE_to_CMP2RR      = MUX_A(CM_CMP2RRCLKSEL, 7),            /*!< Attach NONE to CMP2RR. */

    kPLL0_to_SAI0      = MUX_A(CM_SAI0CLKSEL, 1),                /*!< Attach PLL0 to SAI0. */
    kCLK_IN_to_SAI0    = MUX_A(CM_SAI0CLKSEL, 2),                /*!< Attach Clk_in to SAI0. */
    kFRO_HF_to_SAI0    = MUX_A(CM_SAI0CLKSEL, 3),                /*!< Attach FRO_HF to SAI0. */
    kPLL1_CLK0_to_SAI0 = MUX_A(CM_SAI0CLKSEL, 4),                /*!< Attach PLL1_clk0 to SAI0. */
    kUSB_PLL_to_SAI0   = MUX_A(CM_SAI0CLKSEL, 6),                /*!< Attach USB PLL to SAI0. */
    kNONE_to_SAI0      = MUX_A(CM_SAI0CLKSEL, 7),                /*!< Attach NONE to SAI0. */

    kPLL0_to_SAI1      = MUX_A(CM_SAI1CLKSEL, 1),                /*!< Attach PLL0 to SAI1. */
    kCLK_IN_to_SAI1    = MUX_A(CM_SAI1CLKSEL, 2),                /*!< Attach Clk_in to SAI1. */
    kFRO_HF_to_SAI1    = MUX_A(CM_SAI1CLKSEL, 3),                /*!< Attach FRO_HF to SAI1. */
    kPLL1_CLK0_to_SAI1 = MUX_A(CM_SAI1CLKSEL, 4),                /*!< Attach PLL1_clk0 to SAI1. */
    kUSB_PLL_to_SAI1   = MUX_A(CM_SAI1CLKSEL, 6),                /*!< Attach USB PLL to SAI1. */
    kNONE_to_SAI1      = MUX_A(CM_SAI1CLKSEL, 7),                /*!< Attach NONE to SAI1. */

    kPLL0_to_EMVSIM0      = MUX_A(CM_EMVSIM0CLKSEL, 1),          /*!< Attach PLL0 to EMVSIM0. */
    kCLK_IN_to_EMVSIM0    = MUX_A(CM_EMVSIM0CLKSEL, 2),          /*!< Attach Clk_in to EMVSIM0. */
    kFRO_HF_to_EMVSIM0    = MUX_A(CM_EMVSIM0CLKSEL, 3),          /*!< Attach FRO_HF to EMVSIM0. */
    kFRO12M_to_EMVSIM0    = MUX_A(CM_EMVSIM0CLKSEL, 4),          /*!< Attach FRO_12M to EMVSIM0. */
    kPLL1_CLK0_to_EMVSIM0 = MUX_A(CM_EMVSIM0CLKSEL, 5),          /*!< Attach PLL1_clk0 to EMVSIM0. */
    kNONE_to_EMVSIM0      = MUX_A(CM_EMVSIM0CLKSEL, 7),          /*!< Attach NONE to EMVSIM0. */

    kPLL0_to_EMVSIM1      = MUX_A(CM_EMVSIM1CLKSEL, 1),          /*!< Attach PLL0 to EMVSIM1. */
    kCLK_IN_to_EMVSIM1    = MUX_A(CM_EMVSIM1CLKSEL, 2),          /*!< Attach Clk_in to EMVSIM1. */
    kFRO_HF_to_EMVSIM1    = MUX_A(CM_EMVSIM1CLKSEL, 3),          /*!< Attach FRO_HF to EMVSIM1. */
    kFRO12M_to_EMVSIM1    = MUX_A(CM_EMVSIM1CLKSEL, 4),          /*!< Attach FRO_12M to EMVSIM1. */
    kPLL1_CLK0_to_EMVSIM1 = MUX_A(CM_EMVSIM1CLKSEL, 5),          /*!< Attach PLL1_clk0 to EMVSIM1. */
    kNONE_to_EMVSIM1      = MUX_A(CM_EMVSIM1CLKSEL, 7),          /*!< Attach NONE to EMVSIM1. */

    kPLL0_to_I3C1FCLK      = MUX_A(CM_I3C1FCLKSEL, 1),           /*!< Attach PLL0 to I3C1FCLK. */
    kCLK_IN_to_I3C1FCLK    = MUX_A(CM_I3C1FCLKSEL, 2),           /*!< Attach Clk_in to I3C1FCLK. */
    kFRO_HF_to_I3C1FCLK    = MUX_A(CM_I3C1FCLKSEL, 3),           /*!< Attach FRO_HF to I3C1FCLK. */
    kPLL1_CLK0_to_I3C1FCLK = MUX_A(CM_I3C1FCLKSEL, 5),           /*!< Attach PLL1_clk0 to I3C1FCLK. */
    kUSB_PLL_to_I3C1FCLK   = MUX_A(CM_I3C1FCLKSEL, 6),           /*!< Attach USB PLL to I3C1FCLK. */
    kNONE_to_I3C1FCLK      = MUX_A(CM_I3C1FCLKSEL, 7),           /*!< Attach NONE to I3C1FCLK. */

    kI3C1FCLK_to_I3C1FCLKSTC = MUX_A(CM_I3C1FCLKSTCSEL, 0),      /*!< Attach I3C1FCLK to I3C1FCLKSTC. */
    kCLK_1M_to_I3C1FCLKSTC   = MUX_A(CM_I3C1FCLKSTCSEL, 1),      /*!< Attach CLK_1M to I3C1FCLKSTC. */
    kNONE_to_I3C1FCLKSTC     = MUX_A(CM_I3C1FCLKSTCSEL, 7),      /*!< Attach NONE to I3C1FCLKSTC. */

    kCLK_1M_to_I3C1FCLKS = MUX_A(CM_I3C1FCLKSSEL, 0),            /*!< Attach CLK_1M to I3C1FCLKS. */
    kNONE_to_I3C1FCLKS   = MUX_A(CM_I3C1FCLKSSEL, 7),            /*!< Attach NONE to I3C1FCLKS. */

    kNONE_to_NONE = (int)0x80000000U,                            /*!< Attach NONE to NONE. */

} clock_attach_id_t;

/*! @brief Clock dividers */
typedef enum _clock_div_name
{
    kCLOCK_DivSystickClk0   = 0,                     /*!< Systick Clk0 Divider. */
    kCLOCK_DivSystickClk1   = ((0x304 - 0x300) / 4), /*!< Systick Clk1 Divider. */
    kCLOCK_DivTraceClk      = ((0x308 - 0x300) / 4), /*!< Trace Clk Divider. */
    kCLOCK_DivSlowClk       = ((0x378 - 0x300) / 4), /*!< SLOW CLK Divider. */
    kCLOCK_DivTsiClk        = ((0x37C - 0x300) / 4), /*!< Tsi Clk Divider. */
    kCLOCK_DivAhbClk        = ((0x380 - 0x300) / 4), /*!< Ahb Clk Divider. */
    kCLOCK_DivClkOut        = ((0x384 - 0x300) / 4), /*!< ClkOut Clk Divider. */
    kCLOCK_DivFrohfClk      = ((0x388 - 0x300) / 4), /*!< Frohf Clk Divider. */
    kCLOCK_DivWdt0Clk       = ((0x38C - 0x300) / 4), /*!< Wdt0 Clk Divider. */
    kCLOCK_DivAdc0Clk       = ((0x394 - 0x300) / 4), /*!< Adc0 Clk Divider. */
    kCLOCK_DivUsb0Clk       = ((0x398 - 0x300) / 4), /*!< Usb0 Clk Divider. */
    kCLOCK_DivSctClk        = ((0x3B4 - 0x300) / 4), /*!< Sct Clk Divider. */
    kCLOCK_DivPllClk        = ((0x3C4 - 0x300) / 4), /*!< Pll Clk Divider. */
    kCLOCK_DivCtimer0Clk    = ((0x3D0 - 0x300) / 4), /*!< Ctimer0 Clk Divider. */
    kCLOCK_DivCtimer1Clk    = ((0x3D4 - 0x300) / 4), /*!< Ctimer1 Clk Divider. */
    kCLOCK_DivCtimer2Clk    = ((0x3D8 - 0x300) / 4), /*!< Ctimer2 Clk Divider. */
    kCLOCK_DivCtimer3Clk    = ((0x3DC - 0x300) / 4), /*!< Ctimer3 Clk Divider. */
    kCLOCK_DivCtimer4Clk    = ((0x3E0 - 0x300) / 4), /*!< Ctimer4 Clk Divider. */
    kCLOCK_DivPLL1Clk0      = ((0x3E4 - 0x300) / 4), /*!< PLL1 Clk0 Divider. */
    kCLOCK_DivPLL1Clk1      = ((0x3E8 - 0x300) / 4), /*!< Pll1 Clk1 Divider. */
    kCLOCK_DivAdc1Clk       = ((0x468 - 0x300) / 4), /*!< Adc1 Clk Divider. */
    kCLOCK_DivDac0Clk       = ((0x494 - 0x300) / 4), /*!< Dac0 Clk Divider. */
    kCLOCK_DivDac1Clk       = ((0x49C - 0x300) / 4), /*!< Dac1 Clk Divider. */
    kCLOCK_DivDac2Clk       = ((0x4A4 - 0x300) / 4), /*!< Dac2 Clk Divider. */
    kCLOCK_DivFlexspiClk    = ((0x4AC - 0x300) / 4), /*!< Flexspi Clk Divider. */
    kCLOCK_DivI3c0FClkStc   = ((0x538 - 0x300) / 4), /*!< I3C0 FCLK STC Divider. */
    kCLOCK_DivI3c0FClkS     = ((0x53C - 0x300) / 4), /*!< I3C0 FCLK S Divider. */
    kCLOCK_DivI3c0FClk      = ((0x540 - 0x300) / 4), /*!< I3C0 FClk Divider. */
    kCLOCK_DivMicfilFClk    = ((0x54C - 0x300) / 4), /*!< MICFILFCLK Divider. */
    kCLOCK_DivEspiClk       = ((0x554 - 0x300) / 4), /*!< Espi Clk Divider. */
    kCLOCK_DivUSdhcClk      = ((0x55C - 0x300) / 4), /*!< USdhc Clk Divider. */
    kCLOCK_DivFlexioClk     = ((0x564 - 0x300) / 4), /*!< Flexio Clk Divider. */
    kCLOCK_DivFlexcan0Clk   = ((0x5A4 - 0x300) / 4), /*!< Flexcan0 Clk Divider. */
    kCLOCK_DivFlexcan1Clk   = ((0x5AC - 0x300) / 4), /*!< Flexcan1 Clk Divider. */
    kCLOCK_DivEnetrmiiClk   = ((0x5B4 - 0x300) / 4), /*!< Enetrmii Clk Divider. */
    kCLOCK_DivEnetptprefClk = ((0x5BC - 0x300) / 4), /*!< Enetptpref Clk Divider. */
    kCLOCK_DivWdt1Clk       = ((0x5DC - 0x300) / 4), /*!< Wdt1 Clk Divider. */
    kCLOCK_DivCmp0FClk      = ((0x5F4 - 0x300) / 4), /*!< Cmp0 FClk Divider. */
    kCLOCK_DivCmp0rrClk     = ((0x5FC - 0x300) / 4), /*!< Cmp0rr Clk Divider. */
    kCLOCK_DivCmp1FClk      = ((0x604 - 0x300) / 4), /*!< Cmp1 FClk Divider. */
    kCLOCK_DivCmp1rrClk     = ((0x60C - 0x300) / 4), /*!< Cmp1rr Clk Divider. */
    kCLOCK_DivCmp2FClk      = ((0x614 - 0x300) / 4), /*!< Cmp2 FClk Divider. */
    kCLOCK_DivCmp2rrClk     = ((0x61C - 0x300) / 4), /*!< Cmp2rr Clk Divider. */
    kCLOCK_DivFlexcom0Clk   = ((0x850 - 0x300) / 4), /*!< Flexcom0 Clk Divider. */
    kCLOCK_DivFlexcom1Clk   = ((0x854 - 0x300) / 4), /*!< Flexcom1 Clk Divider. */
    kCLOCK_DivFlexcom2Clk   = ((0x858 - 0x300) / 4), /*!< Flexcom2 Clk Divider. */
    kCLOCK_DivFlexcom3Clk   = ((0x85C - 0x300) / 4), /*!< Flexcom3 Clk Divider. */
    kCLOCK_DivFlexcom4Clk   = ((0x860 - 0x300) / 4), /*!< Flexcom4 Clk Divider. */
    kCLOCK_DivFlexcom5Clk   = ((0x864 - 0x300) / 4), /*!< Flexcom5 Clk Divider. */
    kCLOCK_DivFlexcom6Clk   = ((0x868 - 0x300) / 4), /*!< Flexcom6 Clk Divider. */
    kCLOCK_DivFlexcom7Clk   = ((0x86C - 0x300) / 4), /*!< Flexcom7 Clk Divider. */
    kCLOCK_DivFlexcom8Clk   = ((0x870 - 0x300) / 4), /*!< Flexcom8 Clk Divider. */
    kCLOCK_DivFlexcom9Clk   = ((0x874 - 0x300) / 4), /*!< Flexcom9 Clk Divider. */
    kCLOCK_DivSai0Clk       = ((0x888 - 0x300) / 4), /*!< Sai0 Clk Divider. */
    kCLOCK_DivSai1Clk       = ((0x88C - 0x300) / 4), /*!< Sai1 Clk Divider. */
    kCLOCK_DivEmvsim0Clk    = ((0x898 - 0x300) / 4), /*!< Emvsim0 Clk Divider. */
    kCLOCK_DivEmvsim1Clk    = ((0x89C - 0x300) / 4), /*!< Emvsim1 Clk Divider. */
    kCLOCK_DivI3c1FClkStc   = ((0xB38 - 0x300) / 4), /*!< I3C1 FCLK STC Divider. */
    kCLOCK_DivI3c1FClkS     = ((0xB3C - 0x300) / 4), /*!< I3C1 FCLK S Divider. */
    kCLOCK_DivI3c1FClk      = ((0xB40 - 0x300) / 4), /*!< I3C1 FClk Divider. */
} clock_div_name_t;

/*! @brief OSC32K clock gate */
typedef enum _osc32k_clk_gate_id
{
    kCLOCK_Osc32kToVbat = 0x1, /*!< OSC32K[0] to VBAT domain. */
    kCLOCK_Osc32kToVsys = 0x2, /*!< OSC32K[1] to VSYS domain. */
    kCLOCK_Osc32kToWake = 0x4, /*!< OSC32K[2] to WAKE domain. */
    kCLOCK_Osc32kToMain = 0x8, /*!< OSC32K[3] to MAIN domain. */
    kCLOCK_Osc32kToAll  = 0xF, /*!< OSC32K to VBAT,VSYS,WAKE,MAIN domain. */
} osc32k_clk_gate_id_t;

/*! @brief CLK16K clock gate */
typedef enum _clk16k_clk_gate_id
{
    kCLOCK_Clk16KToVbat = 0x1, /*!< Clk16k[0] to VBAT domain. */
    kCLOCK_Clk16KToVsys = 0x2, /*!< Clk16k[1] to VSYS domain. */
    kCLOCK_Clk16KToWake = 0x4, /*!< Clk16k[2] to WAKE domain. */
    kCLOCK_Clk16KToMain = 0x8, /*!< Clk16k[3] to MAIN domain. */
    kCLOCK_Clk16KToAll  = 0xF, /*!< Clk16k to VBAT,VSYS,WAKE,MAIN domain. */
} clk16k_clk_gate_id_t;

/*! @brief system clocks enable controls */
typedef enum _clock_ctrl_enable
{
    kCLOCK_PLU_DEGLITCH_CLK_ENA =
        SYSCON_CLOCK_CTRL_PLU_DEGLITCH_CLK_ENA_MASK, /*!< Enables clocks FRO_1MHz and FRO_12MHz for PLU deglitching. */
    kCLOCK_FRO1MHZ_CLK_ENA =
        SYSCON_CLOCK_CTRL_FRO1MHZ_CLK_ENA_MASK,      /*!< Enables FRO_1MHz clock for clock muxing in clock gen. */
    kCLOCK_CLKIN_ENA =
        SYSCON_CLOCK_CTRL_CLKIN_ENA_MASK,  /*!< Enables clk_in clock for MICD, EMVSIM0/1, CAN0/1, I3C0/1, SAI0/1, SINC
                                              Filter (SINC), TSI, USBFS, SCT, uSDHC, clkout.. */
    kCLOCK_FRO_HF_ENA =
        SYSCON_CLOCK_CTRL_FRO_HF_ENA_MASK, /*!< Enables FRO HF clock for the Frequency Measure module. */
    kCLOCK_FRO12MHZ_ENA = SYSCON_CLOCK_CTRL_FRO12MHZ_ENA_MASK, /*!< Enables the FRO_12MHz clock for the Flash,
                                                                  LPTIMER0/1, and Frequency Measurement modules. */
    kCLOCK_FRO1MHZ_ENA =
        SYSCON_CLOCK_CTRL_FRO1MHZ_ENA_MASK,           /*!< Enables the FRO_1MHz clock for RTC module and for UTICK. */
    kCLOCK_CLKIN_ENA_FM_USBH_LPT =
        SYSCON_CLOCK_CTRL_CLKIN_ENA_FM_USBH_LPT_MASK, /*!< Enables the clk_in clock for the Frequency Measurement, USB
                                                         HS and LPTIMER0/1 modules. */
} clock_ctrl_enable_t;

/*! @brief Source of the USB HS PHY. */
typedef enum _clock_usb_phy_src
{
    kCLOCK_Usbphy480M = 0, /*!< Use 480M.      */
} clock_usb_phy_src_t;

/*!
 * @brief SCG status return codes.
 */
enum _scg_status
{
    kStatus_SCG_Busy       = MAKE_STATUS(kStatusGroup_SCG, 1), /*!< Clock is busy.  */
    kStatus_SCG_InvalidSrc = MAKE_STATUS(kStatusGroup_SCG, 2)  /*!< Invalid source. */
};

/*!
 * @brief firc trim mode.
 */
typedef enum _firc_trim_mode
{
    kSCG_FircTrimNonUpdate = SCG_FIRCCSR_FIRCTREN_MASK,
    /*!< Trim enable but not enable trim value update. In this mode, the
     trim value is fixed to the initialized value which is defined by
     trimCoar and trimFine in configure structure \ref trim_config_t.*/

    kSCG_FircTrimUpdate = SCG_FIRCCSR_FIRCTREN_MASK | SCG_FIRCCSR_FIRCTRUP_MASK
    /*!< Trim enable and trim value update enable. In this mode, the trim
     value is auto update. */

} firc_trim_mode_t;

/*!
 * @brief firc trim source.
 */
typedef enum _firc_trim_src
{
    kSCG_FircTrimSrcUsb0   = 0U, /*!< USB0 start of frame (1kHz). */
    kSCG_FircTrimSrcSysOsc = 2U, /*!< System OSC.                 */
    kSCG_FircTrimSrcRtcOsc = 3U, /*!< RTC OSC (32.768 kHz).       */
} firc_trim_src_t;

/*!
 * @brief firc trim configuration.
 */
typedef struct _firc_trim_config
{
    firc_trim_mode_t trimMode; /*!< Trim mode.                       */
    firc_trim_src_t trimSrc;   /*!< Trim source.                          */
    uint16_t trimDiv;          /*!< Divider of SOSC.             */

    uint8_t trimCoar;          /*!< Trim coarse value; Irrelevant if trimMode is kSCG_TrimUpdate. */
    uint8_t trimFine;          /*!< Trim fine value; Irrelevant if trimMode is kSCG_TrimUpdate. */
} firc_trim_config_t;

/*!
 * @brief sirc trim mode.
 */
typedef enum _sirc_trim_mode
{
    kSCG_SircTrimNonUpdate = SCG_SIRCCSR_SIRCTREN_MASK,
    /*!< Trim enable but not enable trim value update. In this mode, the
     trim value is fixed to the initialized value which is defined by
     trimCoar and trimFine in configure structure \ref trim_config_t.*/

    kSCG_SircTrimUpdate = SCG_SIRCCSR_SIRCTREN_MASK | SCG_SIRCCSR_SIRCTRUP_MASK
    /*!< Trim enable and trim value update enable. In this mode, the trim
     value is auto update. */

} sirc_trim_mode_t;

/*!
 * @brief sirc trim source.
 */
typedef enum _sirc_trim_src
{
    kSCG_SircTrimSrcSysOsc = 2U, /*!< System OSC.                 */
    kSCG_SircTrimSrcRtcOsc = 3U, /*!< RTC OSC (32.768 kHz).       */
} sirc_trim_src_t;

/*!
 * @brief sirc trim configuration.
 */
typedef struct _sirc_trim_config
{
    sirc_trim_mode_t trimMode; /*!< Trim mode.                       */
    sirc_trim_src_t trimSrc;   /*!< Trim source.                          */
    uint16_t trimDiv;          /*!< Divider of SOSC.             */

    uint8_t cltrim;            /*!< Trim coarse value; Irrelevant if trimMode is kSCG_TrimUpdate. */
    uint8_t ccotrim;           /*!< Trim fine value; Irrelevant if trimMode is kSCG_TrimUpdate. */
} sirc_trim_config_t;

/*!
 * @brief SCG system OSC monitor mode.
 */
typedef enum _scg_sosc_monitor_mode
{
    kSCG_SysOscMonitorDisable = 0U,                         /*!< Monitor disabled. */
    kSCG_SysOscMonitorInt     = SCG_SOSCCSR_SOSCCM_MASK,    /*!< Interrupt when the SOSC error is detected. */
    kSCG_SysOscMonitorReset =
        SCG_SOSCCSR_SOSCCM_MASK | SCG_SOSCCSR_SOSCCMRE_MASK /*!< Reset when the SOSC error is detected.     */
} scg_sosc_monitor_mode_t;

/*!
 * @brief SCG ROSC monitor mode.
 */
typedef enum _scg_rosc_monitor_mode
{
    kSCG_RoscMonitorDisable = 0U,                           /*!< Monitor disabled. */
    kSCG_RoscMonitorInt     = SCG_ROSCCSR_ROSCCM_MASK,      /*!< Interrupt when the RTC OSC error is detected. */
    kSCG_RoscMonitorReset =
        SCG_ROSCCSR_ROSCCM_MASK | SCG_ROSCCSR_ROSCCMRE_MASK /*!< Reset when the RTC OSC error is detected. */
} scg_rosc_monitor_mode_t;

/*!
 * @brief SCG UPLL monitor mode.
 */
typedef enum _scg_upll_monitor_mode
{
    kSCG_UpllMonitorDisable = 0U,                           /*!< Monitor disabled. */
    kSCG_UpllMonitorInt     = SCG_UPLLCSR_UPLLCM_MASK,      /*!< Interrupt when the UPLL error is detected. */
    kSCG_UpllMonitorReset =
        SCG_UPLLCSR_UPLLCM_MASK | SCG_UPLLCSR_UPLLCMRE_MASK /*!< Reset when the UPLL error is detected. */
} scg_upll_monitor_mode_t;

/*!
 * @brief SCG PLL0 monitor mode.
 */
typedef enum _scg_pll0_monitor_mode
{
    kSCG_Pll0MonitorDisable = 0U,                           /*!< Monitor disabled. */
    kSCG_Pll0MonitorInt     = SCG_APLLCSR_APLLCM_MASK,      /*!< Interrupt when the PLL0 Clock error is detected. */
    kSCG_Pll0MonitorReset =
        SCG_APLLCSR_APLLCM_MASK | SCG_APLLCSR_APLLCMRE_MASK /*!< Reset when the PLL0 Clock error is detected. */
} scg_pll0_monitor_mode_t;

/*!
 * @brief SCG PLL1 monitor mode.
 */
typedef enum _scg_pll1_monitor_mode
{
    kSCG_Pll1MonitorDisable = 0U,                           /*!< Monitor disabled. */
    kSCG_Pll1MonitorInt     = SCG_SPLLCSR_SPLLCM_MASK,      /*!< Interrupt when the PLL1 Clock error is detected. */
    kSCG_Pll1MonitorReset =
        SCG_SPLLCSR_SPLLCM_MASK | SCG_SPLLCSR_SPLLCMRE_MASK /*!< Reset when the PLL1 Clock error is detected. */
} scg_pll1_monitor_mode_t;

/*!
 * @brief The enumerator of internal capacitance of OSC's XTAL pin.
 */
typedef enum _vbat_osc_xtal_cap
{
    kVBAT_OscXtal0pFCap  = 0x0U, /*!< The internal capacitance for XTAL pin is 0pF. */
    kVBAT_OscXtal2pFCap  = 0x1U, /*!< The internal capacitance for XTAL pin is 2pF. */
    kVBAT_OscXtal4pFCap  = 0x2U, /*!< The internal capacitance for XTAL pin is 4pF. */
    kVBAT_OscXtal6pFCap  = 0x3U, /*!< The internal capacitance for XTAL pin is 6pF. */
    kVBAT_OscXtal8pFCap  = 0x4U, /*!< The internal capacitance for XTAL pin is 8pF. */
    kVBAT_OscXtal10pFCap = 0x5U, /*!< The internal capacitance for XTAL pin is 10pF. */
    kVBAT_OscXtal12pFCap = 0x6U, /*!< The internal capacitance for XTAL pin is 12pF. */
    kVBAT_OscXtal14pFCap = 0x7U, /*!< The internal capacitance for XTAL pin is 14pF. */
    kVBAT_OscXtal16pFCap = 0x8U, /*!< The internal capacitance for XTAL pin is 16pF. */
    kVBAT_OscXtal18pFCap = 0x9U, /*!< The internal capacitance for XTAL pin is 18pF. */
    kVBAT_OscXtal20pFCap = 0xAU, /*!< The internal capacitance for XTAL pin is 20pF. */
    kVBAT_OscXtal22pFCap = 0xBU, /*!< The internal capacitance for XTAL pin is 22pF. */
    kVBAT_OscXtal24pFCap = 0xCU, /*!< The internal capacitance for XTAL pin is 24pF. */
    kVBAT_OscXtal26pFCap = 0xDU, /*!< The internal capacitance for XTAL pin is 26pF. */
    kVBAT_OscXtal28pFCap = 0xEU, /*!< The internal capacitance for XTAL pin is 28pF. */
    kVBAT_OscXtal30pFCap = 0xFU, /*!< The internal capacitance for XTAL pin is 30pF. */
} vbat_osc_xtal_cap_t;

/*!
 * @brief The enumerator of internal capacitance of OSC's EXTAL pin.
 */
typedef enum _vbat_osc_extal_cap
{
    kVBAT_OscExtal0pFCap  = 0x0U, /*!< The internal capacitance for EXTAL pin is 0pF. */
    kVBAT_OscExtal2pFCap  = 0x1U, /*!< The internal capacitance for EXTAL pin is 2pF. */
    kVBAT_OscExtal4pFCap  = 0x2U, /*!< The internal capacitance for EXTAL pin is 4pF. */
    kVBAT_OscExtal6pFCap  = 0x3U, /*!< The internal capacitance for EXTAL pin is 6pF. */
    kVBAT_OscExtal8pFCap  = 0x4U, /*!< The internal capacitance for EXTAL pin is 8pF. */
    kVBAT_OscExtal10pFCap = 0x5U, /*!< The internal capacitance for EXTAL pin is 10pF. */
    kVBAT_OscExtal12pFCap = 0x6U, /*!< The internal capacitance for EXTAL pin is 12pF. */
    kVBAT_OscExtal14pFCap = 0x7U, /*!< The internal capacitance for EXTAL pin is 14pF. */
    kVBAT_OscExtal16pFCap = 0x8U, /*!< The internal capacitance for EXTAL pin is 16pF. */
    kVBAT_OscExtal18pFCap = 0x9U, /*!< The internal capacitance for EXTAL pin is 18pF. */
    kVBAT_OscExtal20pFCap = 0xAU, /*!< The internal capacitance for EXTAL pin is 20pF. */
    kVBAT_OscExtal22pFCap = 0xBU, /*!< The internal capacitance for EXTAL pin is 22pF. */
    kVBAT_OscExtal24pFCap = 0xCU, /*!< The internal capacitance for EXTAL pin is 24pF. */
    kVBAT_OscExtal26pFCap = 0xDU, /*!< The internal capacitance for EXTAL pin is 26pF. */
    kVBAT_OscExtal28pFCap = 0xEU, /*!< The internal capacitance for EXTAL pin is 28pF. */
    kVBAT_OscExtal30pFCap = 0xFU, /*!< The internal capacitance for EXTAL pin is 30pF. */
} vbat_osc_extal_cap_t;

/*!
 * @brief The enumerator of osc amplifier gain fine adjustment.
 * Changes the oscillator amplitude by modifying the automatic gain control (AGC).
 */
typedef enum _vbat_osc_fine_adjustment_value
{
    kVBAT_OscCoarseAdjustment05 = 0U,
    kVBAT_OscCoarseAdjustment10 = 1U,
    kVBAT_OscCoarseAdjustment18 = 2U,
    kVBAT_OscCoarseAdjustment33 = 3U,
} vbat_osc_coarse_adjustment_value_t;

/*!
 * @brief The structure of oscillator configuration.
 */
typedef struct _vbat_osc_config
{
    bool enableInternalCapBank;         /*!< enable/disable the internal capacitance bank. */

    bool enableCrystalOscillatorBypass; /*!< enable/disable the crystal oscillator bypass. */

    vbat_osc_xtal_cap_t xtalCap;        /*!< The internal capacitance for the OSC XTAL pin from the capacitor bank,
                                                  only useful when the internal capacitance bank is enabled. */
    vbat_osc_extal_cap_t extalCap; /*!< The internal capacitance for the OSC EXTAL pin from the capacitor bank, only
                                      useful when the internal capacitance bank is enabled. */
    vbat_osc_coarse_adjustment_value_t
        coarseAdjustment;          /*!< 32kHz crystal oscillator amplifier coarse adjustment value. */
} vbat_osc_config_t;

/*!
 * @brief The active run mode (voltage level).
 */
typedef enum _run_mode
{
    kMD_Mode, /*!< Midvoltage (1.0 V). */
    kSD_Mode, /*!< Normal voltage (1.1 V). */
    kOD_Mode, /*!< Overdrive voltage (1.2 V). */
} run_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Enable the clock for specific IP.
 * @param clk : Clock to be enabled.
 * @return  Nothing
 */
static inline void CLOCK_EnableClock(clock_ip_name_t clk)
{
    uint32_t index = CLK_GATE_ABSTRACT_REG_OFFSET(clk);
    uint32_t bit   = CLK_GATE_ABSTRACT_BITS_SHIFT(clk);

    if (clk == kCLOCK_None)
        return;

    if (index == (uint32_t)REG_PWM0SUBCTL)
    {
        SYSCON->PWM0SUBCTL |= (1UL << bit);
        SYSCON->AHBCLKCTRLSET[3] = 0x40U;
    }
    else if (index == (uint32_t)REG_PWM1SUBCTL)
    {
        SYSCON->PWM1SUBCTL |= (1UL << bit);
        SYSCON->AHBCLKCTRLSET[3] = 0x80U;
    }
    else
    {
        SYSCON->AHBCLKCTRLSET[index] = (1UL << bit);
    }
}

/**
 * @brief Disable the clock for specific IP.
 * @param clk : Clock to be Disabled.
 * @return  Nothing
 */
static inline void CLOCK_DisableClock(clock_ip_name_t clk)
{
    uint32_t index = CLK_GATE_ABSTRACT_REG_OFFSET(clk);
    uint32_t bit   = CLK_GATE_ABSTRACT_BITS_SHIFT(clk);

    if (clk == kCLOCK_None)
        return;

    if (index == (uint32_t)REG_PWM0SUBCTL)
    {
        SYSCON->PWM0SUBCTL &= ~(1UL << bit);
        if (0U == (SYSCON->PWM0SUBCTL & 0xFU))
        {
            SYSCON->AHBCLKCTRLCLR[3] = 0x20U;
        }
    }
    else if (index == (uint32_t)REG_PWM1SUBCTL)
    {
        SYSCON->PWM1SUBCTL &= ~(1UL << bit);
        if (0U == (SYSCON->PWM1SUBCTL & 0xFU))
        {
            SYSCON->AHBCLKCTRLCLR[3] = 0x40U;
        }
    }
    else
    {
        SYSCON->AHBCLKCTRLCLR[index] = (1UL << bit);
    }
}

/**
 * @brief   Initialize the Core clock to given frequency (48 or 144 MHz).
 * This function turns on FIRC and select the given frequency as the source of fro_hf
 * @param   iFreq   : Desired frequency (must be one of CLK_FRO_44MHZ or CLK_FRO_144MHZ)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupFROHFClocking(uint32_t iFreq);

/**
 * @brief   Initialize the external osc clock to given frequency.
 * @param   iFreq   : Desired frequency (must be equal to exact rate in Hz)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupExtClocking(uint32_t iFreq);

/**
 * @brief   Initialize the external reference clock to given frequency.
 * @param   iFreq   : Desired frequency (must be equal to exact rate in Hz)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupExtRefClocking(uint32_t iFreq);

/**
 * @brief   Initialize the XTAL32/EXTAL32 input clock to given frequency.
 * @param   id   : OSC 32 kHz output clock to specified modules, it should use osc32k_clk_gate_id_t value
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupOsc32KClocking(uint32_t id);

/**
 * @brief   Initialize the FRO16K input clock to given frequency.
 * @param   id   : FRO 16 kHz output clock to specified modules, it should use clk16k_clk_gate_id_t value
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupClk16KClocking(uint32_t id);

/**
 * @brief   Setup FROHF trim.
 * @param   config   : FROHF trim value
 * @return  returns success or fail status.
 */
status_t CLOCK_FROHFTrimConfig(firc_trim_config_t config);

/**
 * @brief   Setup FRO 12M trim.
 * @param   config   : FRO 12M trim value
 * @return  returns success or fail status.
 */
status_t CLOCK_FRO12MTrimConfig(sirc_trim_config_t config);

/*!
 * @brief Sets the system OSC monitor mode.
 *
 * This function sets the system OSC monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetSysOscMonitorMode(scg_sosc_monitor_mode_t mode);

/*!
 * @brief Sets the ROSC monitor mode.
 *
 * This function sets the ROSC monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetRoscMonitorMode(scg_rosc_monitor_mode_t mode);

/*!
 * @brief Sets the UPLL monitor mode.
 *
 * This function sets the UPLL monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetUpllMonitorMode(scg_upll_monitor_mode_t mode);

/*!
 * @brief Sets the PLL0 monitor mode.
 *
 * This function sets the PLL0 monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetPll0MonitorMode(scg_pll0_monitor_mode_t mode);

/*!
 * @brief Sets the PLL1 monitor mode.
 *
 * This function sets the PLL1 monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetPll1MonitorMode(scg_pll1_monitor_mode_t mode);

/*!
 * @brief Config 32k Crystal Oscillator.
 *
 * @param base VBAT peripheral base address.
 * @param config The pointer to the structure \ref vbat_osc_config_t.
 */
void VBAT_SetOscConfig(VBAT_Type *base, const vbat_osc_config_t *config);

/*!
 * @brief	Set the additional number of wait-states added to account for the ratio of system clock period to flash access
 * time during full speed power mode.
 * @param	system_freq_hz	: Input frequency
 * @param	mode	        : Active run mode (voltage level).
 * @return	success or fail status
 */
status_t CLOCK_SetFLASHAccessCyclesForFreq(uint32_t system_freq_hz, run_mode_t mode);

/**
 * @brief   Configure the clock selection muxes.
 * @param   connection  : Clock to be configured.
 * @return  Nothing
 */
void CLOCK_AttachClk(clock_attach_id_t connection);

/**
 * @brief   Get the actual clock attach id.
 * This fuction uses the offset in input attach id, then it reads the actual source value in
 * the register and combine the offset to obtain an actual attach id.
 * @param   attachId  : Clock attach id to get.
 * @return  Clock source value.
 */
clock_attach_id_t CLOCK_GetClockAttachId(clock_attach_id_t attachId);

/**
 * @brief   Setup peripheral clock dividers.
 * @param   div_name    : Clock divider name
 * @param   divided_by_value: Value to be divided
 * @return  Nothing
 */
void CLOCK_SetClkDiv(clock_div_name_t div_name, uint32_t divided_by_value);

/**
 * @brief   Get peripheral clock dividers.
 * @param   div_name    : Clock divider name
 * @return  peripheral clock dividers
 */
uint32_t CLOCK_GetClkDiv(clock_div_name_t div_name);

/**
 * @brief   Halt peripheral clock dividers.
 * @param   div_name    : Clock divider name
 * @return  Nothing
 */
void CLOCK_HaltClkDiv(clock_div_name_t div_name);

/**
 * @brief   system clocks enable controls.
 * @param   mask  : system clocks enable value, it should use clock_ctrl_enable_t value
 * @return  Nothing
 */
void CLOCK_SetupClockCtrl(uint32_t mask);

/*! @brief  Return Frequency of selected clock
 *  @return Frequency of selected clock
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName);

/*! @brief  Return Frequency of main
 *  @return Frequency of the main
 */
uint32_t CLOCK_GetMainClkFreq(void);

/*! @brief  Return Frequency of core
 *  @return Frequency of the core
 */
uint32_t CLOCK_GetCoreSysClkFreq(void);

/*! @brief  Return Frequency of CTimer functional Clock
 *  @return Frequency of CTimer functional Clock
 */
uint32_t CLOCK_GetCTimerClkFreq(uint32_t id);

/*! @brief  Return Frequency of Adc Clock
 *  @return Frequency of Adc.
 */
uint32_t CLOCK_GetAdcClkFreq(uint32_t id);

/*! @brief  Return Frequency of Usb Clock
 *  @return Frequency of Adc.
 */
uint32_t CLOCK_GetUsb0ClkFreq(void);

/*! @brief  Return Frequency of LPFlexComm Clock
 *  @return Frequency of LPFlexComm Clock
 */
uint32_t CLOCK_GetLPFlexCommClkFreq(uint32_t id);

/*! @brief  Return Frequency of SCTimer Clock
 *  @return Frequency of SCTimer Clock.
 */
uint32_t CLOCK_GetSctClkFreq(void);

/*! @brief  Return Frequency of TSI Clock
 *  @return Frequency of TSI Clock.
 */
uint32_t CLOCK_GetTsiClkFreq(void);

/*! @brief  Return Frequency of SINC FILTER Clock
 *  @return Frequency of SINC FILTER Clock.
 */
uint32_t CLOCK_GetSincFilterClkFreq(void);

/*! @brief  Return Frequency of DAC Clock
 *  @return Frequency of DAC Clock
 */
uint32_t CLOCK_GetDacClkFreq(uint32_t id);

/*! @brief  Return Frequency of FlexSPI
 *  @return Frequency of FlexSPI Clock
 */
uint32_t CLOCK_GetFlexspiClkFreq(void);

/*! @brief  Return Frequency of PLL
 *  @return Frequency of PLL
 */
uint32_t CLOCK_GetPll0OutFreq(void);
/*! @brief  Return Frequency of USB PLL
 *  @return Frequency of PLL
 */
uint32_t CLOCK_GetPll1OutFreq(void);

/*! @brief  Return Frequency of PLLCLKDIV
 *  @return Frequency of PLLCLKDIV Clock
 */
uint32_t CLOCK_GetPllClkDivFreq(void);

/*! @brief  Return Frequency of I3C function Clock
 *  @return Frequency of I3C function Clock
 */
uint32_t CLOCK_GetI3cClkFreq(uint32_t id);

/*! @brief  Return Frequency of I3C function slow TC Clock
 *  @return Frequency of I3C function slow TC Clock
 */
uint32_t CLOCK_GetI3cSTCClkFreq(uint32_t id);

/*! @brief  Return Frequency of I3C function slow Clock
 *  @return Frequency of I3C function slow Clock
 */
uint32_t CLOCK_GetI3cSClkFreq(uint32_t id);

/*! @brief  Return Frequency of MICFIL Clock
 *  @return Frequency of MICFIL.
 */
uint32_t CLOCK_GetMicfilClkFreq(void);

/*! @brief  Return Frequency of uSDHC
 *  @return Frequency of uSDHC Clock
 */
uint32_t CLOCK_GetUsdhcClkFreq(void);

/*! @brief  Return Frequency of FLEXIO
 *  @return Frequency of FLEXIO Clock
 */
uint32_t CLOCK_GetFlexioClkFreq(void);

/*! @brief  Return Frequency of FLEXCAN
 *  @return Frequency of FLEXCAN Clock
 */
uint32_t CLOCK_GetFlexcanClkFreq(uint32_t id);

/*! @brief  Return Frequency of Ethernet RMII Clock
 *  @return Frequency of Ethernet RMII.
 */
uint32_t CLOCK_GetEnetRmiiClkFreq(void);

/*! @brief  Return Frequency of Ethernet PTP REF Clock
 *  @return Frequency of Ethernet PTP REF.
 */
uint32_t CLOCK_GetEnetPtpRefClkFreq(void);

/**
 * @brief   Initialize the ENET TX CLK to given frequency.
 * @param   iFreq   : Desired frequency
 * @return  Nothing
 */
void CLOCK_SetupEnetTxClk(uint32_t iFreq);

/**
 * @brief   Return Frequency of ENET TX CLK
 * @return  Frequency of ENET TX CLK
 */
uint32_t CLOCK_GetEnetTxClkFreq(void);

/*! @brief  Return Frequency of EWM0 Clock
 *  @return Frequency of EWM0.
 */
uint32_t CLOCK_GetEwm0ClkFreq(void);

/*! @brief  Return Frequency of Watchdog
 *  @return Frequency of Watchdog
 */
uint32_t CLOCK_GetWdtClkFreq(uint32_t id);

/*! @brief  Return Frequency of OSTIMER
 *  @return Frequency of OSTIMER Clock
 */
uint32_t CLOCK_GetOstimerClkFreq(void);

/*! @brief  Return Frequency of CMP Function Clock
 *  @return Frequency of CMP Function.
 */
uint32_t CLOCK_GetCmpFClkFreq(uint32_t id);

/*! @brief  Return Frequency of CMP Round Robin Clock
 *  @return Frequency of CMP Round Robin.
 */
uint32_t CLOCK_GetCmpRRClkFreq(uint32_t id);

/*! @brief  Return Frequency of SAI Clock
 *  @return Frequency of SAI Clock.
 */
uint32_t CLOCK_GetSaiClkFreq(uint32_t id);

/**
 * @brief   Initialize the SAI MCLK to given frequency.
 * @param   iFreq   : Desired frequency
 * @return  Nothing
 */
void CLOCK_SetupSaiMclk(uint32_t id, uint32_t iFreq);

/**
 * @brief   Initialize the SAI TX BCLK to given frequency.
 * @param   iFreq   : Desired frequency
 * @return  Nothing
 */
void CLOCK_SetupSaiTxBclk(uint32_t id, uint32_t iFreq);

/**
 * @brief   Initialize the SAI RX BCLK to given frequency.
 * @param   iFreq   : Desired frequency
 * @return  Nothing
 */
void CLOCK_SetupSaiRxBclk(uint32_t id, uint32_t iFreq);

/**
 * @brief   Return Frequency of SAI MCLK
 * @return  Frequency of SAI MCLK
 */
uint32_t CLOCK_GetSaiMclkFreq(uint32_t id);

/**
 * @brief   Return Frequency of SAI TX BCLK
 * @return  Frequency of SAI TX BCLK
 */
uint32_t CLOCK_GetSaiTxBclkFreq(uint32_t id);

/**
 * @brief   Return Frequency of SAI RX BCLK
 * @return  Frequency of SAI RX BCLK
 */
uint32_t CLOCK_GetSaiRxBclkFreq(uint32_t id);

/*! @brief  Return Frequency of EMVSIM Clock
 *  @return Frequency of EMVSIM Clock.
 */
uint32_t CLOCK_GetEmvsimClkFreq(uint32_t id);

/*! @brief    Return  PLL0 input clock rate
 *  @return    PLL0 input clock rate
 */
uint32_t CLOCK_GetPLL0InClockRate(void);

/*! @brief    Return  PLL1 input clock rate
 *  @return    PLL1 input clock rate
 */
uint32_t CLOCK_GetPLL1InClockRate(void);

/*! @brief Gets the external UPLL frequency.
 *  @return The frequency of the external UPLL.
 */
uint32_t CLOCK_GetExtUpllFreq(void);

/*! @brief Sets the external UPLL frequency.
 *  @param The frequency of external UPLL.
 */
void CLOCK_SetExtUpllFreq(uint32_t freq);

/*! @brief    Check if PLL is locked or not
 *  @return   true if the PLL is locked, false if not locked
 */
__STATIC_INLINE bool CLOCK_IsPLL0Locked(void)
{
    return (bool)((SCG0->APLLCSR & SCG_APLLCSR_APLL_LOCK_MASK) != 0UL);
}

/*! @brief	Check if PLL1 is locked or not
 *  @return	true if the PLL1 is locked, false if not locked
 */
__STATIC_INLINE bool CLOCK_IsPLL1Locked(void)
{
    return (bool)((SCG0->SPLLCSR & SCG_SPLLCSR_SPLL_LOCK_MASK) != 0UL);
}

/*! @brief PLL configuration structure flags for 'flags' field
 * These flags control how the PLL configuration function sets up the PLL setup structure.<br>
 *
 * When the PLL_CONFIGFLAG_FORCENOFRACT flag is selected, the PLL hardware for the
 * automatic bandwidth selection, Spread Spectrum (SS) support, and fractional M-divider
 * are not used.<br>
 */
#define PLL_CONFIGFLAG_FORCENOFRACT (1U << 2U)
/*!< Force non-fractional output mode, PLL output will not use the fractional, automatic bandwidth, or SS hardware */

/*!
 * @brief PLL clock source.
 */
typedef enum _pll_clk_src
{
    kPll_ClkSrcSysOsc = (0 << 25), /*!< System OSC. */
    kPll_ClkSrcFirc   = (1 << 25), /*!< Fast IRC.   */
    kPll_ClkSrcRosc   = (2 << 25), /*!< RTC OSC. */
} pll_clk_src_t;

/*! @brief PLL Spread Spectrum (SS) Programmable modulation frequency
 * See (MF) field in the PLL0SSCG1 register in the UM.
 */
typedef enum _ss_progmodfm
{
    kSS_MF_512 = (0 << 2), /*!< Nss =  512 (fm ~= 3.9  - 7.8   kHz) */
    kSS_MF_384 = (1 << 2), /*!< Nss ~= 384 (fm ~= 5.2  - 10.4  kHz) */
    kSS_MF_256 = (2 << 2), /*!< Nss =  256 (fm ~= 7.8  - 15.6  kHz) */
    kSS_MF_128 = (3 << 2), /*!< Nss =  128 (fm ~= 15.6 - 31.3  kHz) */
    kSS_MF_64  = (4 << 2), /*!< Nss =  64  (fm ~= 32.3 - 64.5  kHz) */
    kSS_MF_32  = (5 << 2), /*!< Nss =  32  (fm ~= 62.5 - 125   kHz) */
    kSS_MF_24  = (6 << 2), /*!< Nss ~= 24  (fm ~= 83.3 - 166.6 kHz) */
    kSS_MF_16  = (7 << 2)  /*!< Nss =  16  (fm ~= 125  - 250   kHz) */
} ss_progmodfm_t;

/*! @brief PLL Spread Spectrum (SS) Programmable frequency modulation depth
 * See (MR) field in the PLL0SSCG1 register in the UM.
 */
typedef enum _ss_progmoddp
{
    kSS_MR_K0   = (0 << 5), /*!< k =  0 (no spread spectrum) */
    kSS_MR_K1   = (1 << 5), /*!< k ~= 1                      */
    kSS_MR_K1_5 = (2 << 5), /*!< k ~= 1.5                    */
    kSS_MR_K2   = (3 << 5), /*!< k ~= 2                      */
    kSS_MR_K3   = (4 << 5), /*!< k ~= 3                      */
    kSS_MR_K4   = (5 << 5), /*!< k ~= 4                      */
    kSS_MR_K6   = (6 << 5), /*!< k ~= 6                      */
    kSS_MR_K8   = (7 << 5)  /*!< k ~= 8                      */
} ss_progmoddp_t;

/*! @brief PLL Spread Spectrum (SS) Modulation waveform control
 * See (MC) field in the PLL0SSCG1 register in the UM.<br>
 * Compensation for low pass filtering of the PLL to get a triangular
 * modulation at the output of the PLL, giving a flat frequency spectrum.
 */
typedef enum _ss_modwvctrl
{
    kSS_MC_NOC  = (0 << 8), /*!< no compensation */
    kSS_MC_RECC = (2 << 8), /*!< recommended setting */
    kSS_MC_MAXC = (3 << 8), /*!< max. compensation */
} ss_modwvctrl_t;

/*! @brief PLL configuration structure
 *
 * This structure can be used to configure the settings for a PLL
 * setup structure. Fill in the desired configuration for the PLL
 * and call the PLL setup function to fill in a PLL setup structure.
 */
typedef struct _pll_config
{
    uint32_t desiredRate; /*!< Desired PLL rate in Hz */
    uint32_t inputSource; /*!< PLL input source */
    uint32_t flags;       /*!< PLL configuration flags, Or'ed value of PLL_CONFIGFLAG_* definitions */
    ss_progmodfm_t ss_mf; /*!< SS Programmable modulation frequency, only applicable when not using
                             PLL_CONFIGFLAG_FORCENOFRACT flag */
    ss_progmoddp_t ss_mr; /*!< SS Programmable frequency modulation depth, only applicable when not using
                             PLL_CONFIGFLAG_FORCENOFRACT flag */
    ss_modwvctrl_t
        ss_mc;     /*!< SS Modulation waveform control, only applicable when not using PLL_CONFIGFLAG_FORCENOFRACT flag
                    */
    bool mfDither; /*!< false for fixed modulation frequency or true for dithering, only applicable when not using
                      PLL_CONFIGFLAG_FORCENOFRACT flag */

} pll_config_t;

/*! @brief PLL0 setup structure
 * This structure can be used to pre-build a PLL setup configuration
 * at run-time and quickly set the PLL to the configuration. It can be
 * populated with the PLL setup function. If powering up or waiting
 * for PLL lock, the PLL input clock source should be configured prior
 * to PLL setup.
 */
typedef struct _pll_setup
{
    uint32_t pllctrl;    /*!< PLL Control register APLLCTRL */
    uint32_t pllndiv;    /*!< PLL N Divider register APLLNDIV */
    uint32_t pllpdiv;    /*!< PLL P Divider register APLLPDIV */
    uint32_t pllmdiv;    /*!< PLL M Divider register APLLMDIV */
    uint32_t pllsscg[2]; /*!< PLL Spread Spectrum Control registers APLLSSCG*/
    uint32_t pllRate;    /*!< Acutal PLL rate */
} pll_setup_t;

/*! @brief PLL status definitions
 */
typedef enum _pll_error
{
    kStatus_PLL_Success         = MAKE_STATUS(kStatusGroup_Generic, 0), /*!< PLL operation was successful */
    kStatus_PLL_OutputTooLow    = MAKE_STATUS(kStatusGroup_Generic, 1), /*!< PLL output rate request was too low */
    kStatus_PLL_OutputTooHigh   = MAKE_STATUS(kStatusGroup_Generic, 2), /*!< PLL output rate request was too high */
    kStatus_PLL_OutputError     = MAKE_STATUS(kStatusGroup_Generic, 3), /*!< PLL output rate error */
    kStatus_PLL_InputTooLow     = MAKE_STATUS(kStatusGroup_Generic, 4), /*!< PLL input rate is too low */
    kStatus_PLL_InputTooHigh    = MAKE_STATUS(kStatusGroup_Generic, 5), /*!< PLL input rate is too high */
    kStatus_PLL_OutsideIntLimit = MAKE_STATUS(kStatusGroup_Generic, 6), /*!< Requested output rate isn't possible */
    kStatus_PLL_CCOTooLow       = MAKE_STATUS(kStatusGroup_Generic, 7), /*!< Requested CCO rate isn't possible */
    kStatus_PLL_CCOTooHigh      = MAKE_STATUS(kStatusGroup_Generic, 8)  /*!< Requested CCO rate isn't possible */
} pll_error_t;

/*! @brief    Return PLL0 output clock rate from setup structure
 *  @param    pSetup : Pointer to a PLL setup structure
 *  @return   System PLL output clock rate the setup structure will generate
 */
uint32_t CLOCK_GetPLLOutFromSetup(pll_setup_t *pSetup);

/*! @brief    Set PLL output based on the passed PLL setup data
 *  @param    pControl    : Pointer to populated PLL control structure to generate setup with
 *  @param    pSetup      : Pointer to PLL setup structure to be filled
 *  @return   PLL_ERROR_SUCCESS on success, or PLL setup error code
 *  @note Actual frequency for setup may vary from the desired frequency based on the
 *  accuracy of input clocks, rounding, non-fractional PLL mode, etc.
 */
pll_error_t CLOCK_SetupPLLData(pll_config_t *pControl, pll_setup_t *pSetup);

/**
 * @brief Set PLL output from PLL setup structure (precise frequency)
 * @param pSetup  : Pointer to populated PLL setup structure
 * @return    kStatus_PLL_Success on success, or PLL setup error code
 * @note  This function will power off the PLL, setup the PLL with the
 * new setup data, and then optionally powerup the PLL, wait for PLL lock,
 * and adjust system voltages to the new PLL rate. The function will not
 * alter any source clocks (ie, main systen clock) that may use the PLL,
 * so these should be setup prior to and after exiting the function.
 */
pll_error_t CLOCK_SetPLL0Freq(const pll_setup_t *pSetup);

/**
 * @brief Set PLL output from PLL setup structure (precise frequency)
 * @param pSetup  : Pointer to populated PLL setup structure
 * @return    kStatus_PLL_Success on success, or PLL setup error code
 * @note  This function will power off the PLL, setup the PLL with the
 * new setup data, and then optionally powerup the PLL, wait for PLL lock,
 * and adjust system voltages to the new PLL rate. The function will not
 * alter any source clocks (ie, main systen clock) that may use the PLL,
 * so these should be setup prior to and after exiting the function.
 */
pll_error_t CLOCK_SetPLL1Freq(const pll_setup_t *pSetup);

/*! @brief Enable the OSTIMER 32k clock.
 *  @return  Nothing
 */
void CLOCK_EnableOstimer32kClock(void);

/*! brief Enable USB FS clock.
 * Enable USB Full Speed clock.
 */
bool CLOCK_EnableUsbfsClock(void);

/*! brief Enable USB HS PHY PLL clock.
 *
 * This function enables the internal 480MHz USB PHY PLL clock.
 *
 * param src  USB HS PHY PLL clock source.
 * param freq The frequency specified by src.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhsPhyPllClock(clock_usb_phy_src_t src, uint32_t freq);

/*! brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhsPhyPllClock(void);

/*! brief Enable USB HS clock.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhsClock(void);

/**
 * @brief   FIRC Auto Trim With SOF.
 * @return  returns success or fail status.
 */
status_t CLOCK_FIRCAutoTrimWithSOF(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_CLOCK_H_ */
