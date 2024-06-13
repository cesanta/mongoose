/*
 * Copyright 2022, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_RESET_H_
#define _FSL_RESET_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "fsl_device_registers.h"

/*!
 * @addtogroup reset
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief reset driver version 2.4.0 */
#define FSL_RESET_DRIVER_VERSION (MAKE_VERSION(2, 4, 0))
/*@}*/

/*!
 * @brief Enumeration for peripheral reset control bits
 *
 * Defines the enumeration for peripheral reset control bits in PRESETCTRL/ASYNCPRESETCTRL registers
 */
typedef enum _SYSCON_RSTn
{
    kFMU_RST_SHIFT_RSTn     = 0 | 9U,  /**< Flash management unit reset control */
    kFLEXSPI_RST_SHIFT_RSTn = 0 | 11U, /**< FLEXSPI reset control */
    kMUX_RST_SHIFT_RSTn     = 0 | 12U, /**< Input mux reset control */
    kPORT0_RST_SHIFT_RSTn   = 0 | 13U, /**< PORT0 reset control */
    kPORT1_RST_SHIFT_RSTn   = 0 | 14U, /**< PORT1 reset control */
    kPORT2_RST_SHIFT_RSTn   = 0 | 15U, /**< PORT2 reset control */
    kPORT3_RST_SHIFT_RSTn   = 0 | 16U, /**< PORT3 reset control */
    kPORT4_RST_SHIFT_RSTn   = 0 | 17U, /**< PORT4 reset control */
    kGPIO0_RST_SHIFT_RSTn   = 0 | 19U, /**< GPIO0 reset control */
    kGPIO1_RST_SHIFT_RSTn   = 0 | 20U, /**< GPIO1 reset control */
    kGPIO2_RST_SHIFT_RSTn   = 0 | 21U, /**< GPIO2 reset control */
    kGPIO3_RST_SHIFT_RSTn   = 0 | 22U, /**< GPIO3 reset control */
    kGPIO4_RST_SHIFT_RSTn   = 0 | 23U, /**< GPIO4 reset control */
    kPINT_RST_SHIFT_RSTn    = 0 | 25U, /**< Pin interrupt (PINT) reset control */
    kDMA0_RST_SHIFT_RSTn    = 0 | 26U, /**< DMA0 reset control */
    kCRC_RST_SHIFT_RSTn     = 0 | 27U, /**< CRC reset control */
    kMAILBOX_RST_SHIFT_RSTn = 0 | 31U, /**< Mailbox reset control */

    kMRT_RST_SHIFT_RSTn         = 65536 | 0U,  /**< Multi-rate timer (MRT) reset control */
    kOSTIMER_RST_SHIFT_RSTn     = 65536 | 1U,  /**< OSTimer reset control */
    kSCT_RST_SHIFT_RSTn         = 65536 | 2U,  /**< SCTimer/PWM(SCT) reset control */
    kADC0_RST_SHIFT_RSTn        = 65536 | 3U,  /**< ADC0 reset control */
    kADC1_RST_SHIFT_RSTn        = 65536 | 4U,  /**< ADC1 reset control */
    kDAC0_RST_SHIFT_RSTn        = 65536 | 5U,  /**< DAC0 reset control */
    kEVSIM0_RST_SHIFT_RSTn      = 65536 | 8U,  /**< EVSIM0 reset control */
    kEVSIM1_RST_SHIFT_RSTn      = 65536 | 9U,  /**< EVSIM1 reset control */
    kUTICK_RST_SHIFT_RSTn       = 65536 | 10U, /**< Micro-tick timer reset control */
    kFC0_RST_SHIFT_RSTn         = 65536 | 11U, /**< Flexcomm Interface 0 reset control */
    kFC1_RST_SHIFT_RSTn         = 65536 | 12U, /**< Flexcomm Interface 1 reset control */
    kFC2_RST_SHIFT_RSTn         = 65536 | 13U, /**< Flexcomm Interface 2 reset control */
    kFC3_RST_SHIFT_RSTn         = 65536 | 14U, /**< Flexcomm Interface 3 reset control */
    kFC4_RST_SHIFT_RSTn         = 65536 | 15U, /**< Flexcomm Interface 4 reset control */
    kFC5_RST_SHIFT_RSTn         = 65536 | 16U, /**< Flexcomm Interface 5 reset control */
    kFC6_RST_SHIFT_RSTn         = 65536 | 17U, /**< Flexcomm Interface 6 reset control */
    kFC7_RST_SHIFT_RSTn         = 65536 | 18U, /**< Flexcomm Interface 7 reset control */
    kFC8_RST_SHIFT_RSTn         = 65536 | 19U, /**< Flexcomm Interface 8 reset control */
    kFC9_RST_SHIFT_RSTn         = 65536 | 20U, /**< MICFIL reset control */
    kMICFIL_RST_SHIFT_RSTn      = 65536 | 21U, /**< Flexcomm Interface 7 reset control */
    kCTIMER2_RST_SHIFT_RSTn     = 65536 | 22U, /**< CTimer 2 reset control */
    kUSB0_RAM_RST_SHIFT_RSTn    = 65536 | 23U, /**< USB0 RAM reset control */
    kUSB0_FS_DCD_RST_SHIFT_RSTn = 65536 | 24U, /**< USB0-FS DCD reset control */
    kUSB0_FS_RST_SHIFT_RSTn     = 65536 | 25U, /**< USB0-FS reset control */
    kCTIMER0_RST_SHIFT_RSTn     = 65536 | 26U, /**< CTimer 0 reset control */
    kCTIMER1_RST_SHIFT_RSTn     = 65536 | 27U, /**< CTimer 1 reset control */
    kSMART_DMA_RST_SHIFT_RSTn   = 65536 | 31U, /**< SmartDMA reset control */

    kDMA1_RST_SHIFT_RSTn       = 131072 | 1U,  /**< DMA1 reset control */
    kENET_RST_SHIFT_RSTn       = 131072 | 2U,  /**< Ethernet reset control */
    kUSDHC_RST_SHIFT_RSTn      = 131072 | 3U,  /**< uSDHC reset control */
    kFLEXIO_RST_SHIFT_RSTn     = 131072 | 4U,  /**< FLEXIO reset control */
    kSAI0_RST_SHIFT_RSTn       = 131072 | 5U,  /**< SAI0 reset control */
    kSAI1_RST_SHIFT_RSTn       = 131072 | 6U,  /**< SAI1 reset control */
    kTRO_RST_SHIFT_RSTn        = 131072 | 7U,  /**< TRO reset control */
    kFREQME_RST_SHIFT_RSTn     = 131072 | 8U,  /**< FREQME reset control */
    kTRNG_RST_SHIFT_RSTn       = 131072 | 13U, /**< TRNG reset control */
    kFLEXCAN0_RST_SHIFT_RSTn   = 131072 | 14U, /**< Flexcan0 reset control */
    kFLEXCAN1_RST_SHIFT_RSTn   = 131072 | 15U, /**< Flexcan1 reset control */
    kUSB_HS_RST_SHIFT_RSTn     = 131072 | 16U, /**< USB HS reset control */
    kUSB_HS_PHY_RST_SHIFT_RSTn = 131072 | 17U, /**< USB HS PHY reset control */
    kPOWERQUAD_RST_SHIFT_RSTn  = 131072 | 19U, /**< PowerQuad reset control */
    kPLU_RST_SHIFT_RSTn        = 131072 | 20U, /**< PLU reset control */
    kCTIMER3_RST_SHIFT_RSTn    = 131072 | 21U, /**< CTimer 3 reset control */
    kCTIMER4_RST_SHIFT_RSTn    = 131072 | 22U, /**< CTimer 4 reset control */
    kPUF_RST_SHIFT_RSTn        = 131072 | 23U, /**< PUF reset control */
    kPKC_RST_SHIFT_RSTn        = 131072 | 24U, /**< PKC reset control */
    kSM3_RST_SHIFT_RSTn        = 131072 | 30U, /**< SM3 reset control */

    kI3C0_RST_SHIFT_RSTn         = 196608 | 0U,  /**< I3C0 reset control */
    kI3C1_RST_SHIFT_RSTn         = 196608 | 1U,  /**< I3C1 reset control */
    kSINC_RST_SHIFT_RSTn         = 196608 | 2U,  /**< SINC reset control */
    kCOOLFLUX_RST_SHIFT_RSTn     = 196608 | 3U,  /**< CoolFlux reset control */
    kENC0_RST_SHIFT_RSTn         = 196608 | 4U,  /**< ENC0 reset control */
    kENC1_RST_SHIFT_RSTn         = 196608 | 5U,  /**< ENC1 reset control */
    kPWM0_RST_SHIFT_RSTn         = 196608 | 6U,  /**< PWM0 reset control */
    kPWM1_RST_SHIFT_RSTn         = 196608 | 7U,  /**< PWM1 reset control */
    kAOI0_RST_SHIFT_RSTn         = 196608 | 8U,  /**< AOI0 reset control */
    kDAC1_RST_SHIFT_RSTn         = 196608 | 11U, /**< DAC1 reset control */
    kDAC2_RST_SHIFT_RSTn         = 196608 | 12U, /**< DAC2 reset control */
    kOPAMP0_RST_SHIFT_RSTn       = 196608 | 13U, /**< OPAMP0 reset control */
    kOPAMP1_RST_SHIFT_RSTn       = 196608 | 14U, /**< OPAMP1 reset control */
    kOPAMP2_RST_SHIFT_RSTn       = 196608 | 15U, /**< OPAMP2 reset control */
    kCMP2_RST_SHIFT_RSTn         = 196608 | 18U, /**< CMP2 reset control */
    kVREF_RST_SHIFT_RSTn         = 196608 | 19U, /**< VREF reset control */
    kCOOLFLUX_APB_RST_SHIFT_RSTn = 196608 | 20U, /**< CoolFlux APB reset control */
    kNEUTRON_RST_SHIFT_RSTn      = 196608 | 21U, /**< Neutron mini reset control */
    kTSI_RST_SHIFT_RSTn          = 196608 | 22U, /**< TSI reset control */
    kEWM_RST_SHIFT_RSTn          = 196608 | 23U, /**< EWM reset control */
    kEIM_RST_SHIFT_RSTn          = 196608 | 24U, /**< EIM reset control */
    kSEMA42_RST_SHIFT_RSTn       = 196608 | 27U, /**< Semaphore reset control */
} SYSCON_RSTn_t;

/** Array initializers with peripheral reset bits **/
#define ADC_RSTS                                   \
    {                                              \
        kADC0_RST_SHIFT_RSTn, kADC1_RST_SHIFT_RSTn \
    } /* Reset bits for ADC peripheral */
#define CRC_RSTS            \
    {                       \
        kCRC_RST_SHIFT_RSTn \
    } /* Reset bits for CRC peripheral */
#define CTIMER_RSTS                                                                                         \
    {                                                                                                       \
        kCTIMER0_RST_SHIFT_RSTn, kCTIMER1_RST_SHIFT_RSTn, kCTIMER2_RST_SHIFT_RSTn, kCTIMER3_RST_SHIFT_RSTn, \
            kCTIMER4_RST_SHIFT_RSTn                                                                         \
    } /* Reset bits for CTIMER peripheral */
#define DMA_RSTS_N                                 \
    {                                              \
        kDMA0_RST_SHIFT_RSTn, kDMA1_RST_SHIFT_RSTn \
    } /* Reset bits for DMA peripheral */

#define LP_FLEXCOMM_RSTS                                                                                            \
    {                                                                                                               \
        kFC0_RST_SHIFT_RSTn, kFC1_RST_SHIFT_RSTn, kFC2_RST_SHIFT_RSTn, kFC3_RST_SHIFT_RSTn, kFC4_RST_SHIFT_RSTn,    \
            kFC5_RST_SHIFT_RSTn, kFC6_RST_SHIFT_RSTn, kFC7_RST_SHIFT_RSTn, kFC8_RST_SHIFT_RSTn, kFC9_RST_SHIFT_RSTn \
    } /* Reset bits for FLEXCOMM peripheral */
#define GPIO_RSTS_N                                                                                 \
    {                                                                                               \
        kGPIO0_RST_SHIFT_RSTn, kGPIO1_RST_SHIFT_RSTn, kGPIO2_RST_SHIFT_RSTn, kGPIO3_RST_SHIFT_RSTn, \
            kGPIO4_RST_SHIFT_RSTn                                                                   \
    } /* Reset bits for GPIO peripheral */
#define INPUTMUX_RSTS       \
    {                       \
        kMUX_RST_SHIFT_RSTn \
    } /* Reset bits for INPUTMUX peripheral */
#define FLASH_RSTS          \
    {                       \
        kFMC_RST_SHIFT_RSTn \
    } /* Reset bits for Flash peripheral */
#define MRT_RSTS            \
    {                       \
        kMRT_RST_SHIFT_RSTn \
    } /* Reset bits for MRT peripheral */
#define PINT_RSTS            \
    {                        \
        kPINT_RST_SHIFT_RSTn \
    } /* Reset bits for PINT peripheral */
#define TRNG_RSTS            \
    {                        \
        kTRNG_RST_SHIFT_RSTn \
    } /* Reset bits for TRNG peripheral */
#define SCT_RSTS            \
    {                       \
        kSCT_RST_SHIFT_RSTn \
    } /* Reset bits for SCT peripheral */
#define UTICK_RSTS            \
    {                         \
        kUTICK_RST_SHIFT_RSTn \
    } /* Reset bits for UTICK peripheral */
#define PLU_RSTS_N          \
    {                       \
        kPLU_RST_SHIFT_RSTn \
    } /* Reset bits for PLU peripheral */
#define OSTIMER_RSTS            \
    {                           \
        kOSTIMER_RST_SHIFT_RSTn \
    } /* Reset bits for OSTIMER peripheral */
#define POWERQUAD_RSTS            \
    {                             \
        kPOWERQUAD_RST_SHIFT_RSTn \
    } /* Reset bits for Powerquad peripheral */
#define I3C_RSTS                                   \
    {                                              \
        kI3C0_RST_SHIFT_RSTn, kI3C1_RST_SHIFT_RSTn \
    } /* Reset bits for I3C peripheral */
typedef SYSCON_RSTn_t reset_ip_name_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Assert reset to peripheral.
 *
 * Asserts reset signal to specified peripheral module.
 *
 * @param peripheral Assert reset to this peripheral. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_SetPeripheralReset(reset_ip_name_t peripheral);

/*!
 * @brief Clear reset to peripheral.
 *
 * Clears reset signal to specified peripheral module, allows it to operate.
 *
 * @param peripheral Clear reset to this peripheral. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_ClearPeripheralReset(reset_ip_name_t peripheral);

/*!
 * @brief Reset peripheral module.
 *
 * Reset peripheral module.
 *
 * @param peripheral Peripheral to reset. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
void RESET_PeripheralReset(reset_ip_name_t peripheral);

/*!
 * @brief Release peripheral module.
 *
 * Release peripheral module.
 *
 * @param peripheral Peripheral to release. The enum argument contains encoding of reset register
 *                   and reset bit position in the reset register.
 */
static inline void RESET_ReleasePeripheralReset(reset_ip_name_t peripheral)
{
    RESET_ClearPeripheralReset(peripheral);
}

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _FSL_RESET_H_ */
