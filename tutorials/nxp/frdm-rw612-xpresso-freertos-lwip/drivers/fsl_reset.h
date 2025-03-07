/*
 * Copyright 2021, 2023-2024 NXP
 *  
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
/*! @brief reset driver version 2.1.1. */
#define FSL_RESET_DRIVER_VERSION (MAKE_VERSION(2, 1, 1))
/*@}*/

/*!
 * @brief Reset control registers index
 */
#define RST_CTL0_PSCCTL0 0
#define RST_CTL0_PSCCTL1 1
#define RST_CTL0_PSCCTL2 2
#define RST_CTL1_PSCCTL0 3
#define RST_CTL1_PSCCTL1 4
#define RST_CTL1_PSCCTL2 5
/*!
 * @brief Enumeration for peripheral reset control bits
 *
 * Defines the enumeration for peripheral reset control bits in RSTCLTx registers
 */
typedef enum _RSTCTL_RSTn
{
    kPOWERQUAD_RST_SHIFT_RSTn = (RST_CTL0_PSCCTL0 << 8) | 8U,      /**< POWERQUAD reset control */
    kPKC_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 9U,      /**< PKC reset control */
    kELS_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 10U,     /**< ELS reset control */
    kPUF_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 11U,     /**< Physical unclonable function reset control */
    kFLEXSPI_RST_SHIFT_RSTn   = (RST_CTL0_PSCCTL0 << 8) | 16U,     /**< FLEXSPI reset control */
    kHPU_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 20U,     /**< HPU reset control */
    kUSB_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 22U,     /**< USB reset control */
    kSCT_RST_SHIFT_RSTn       = (RST_CTL0_PSCCTL0 << 8) | 24U,     /**< Standard ctimers reset control */
    kAON_MEM_RST_SHIFT_RSTn   = (RST_CTL0_PSCCTL0 << 8) | 25U,     /**< AON MEM reset control */
    kGDMA_RST_SHIFT_RSTn      = (RST_CTL0_PSCCTL0 << 8) | 28U,     /**< GDMA reset control */
    kDMA0_RST_SHIFT_RSTn      = (RST_CTL0_PSCCTL0 << 8) | 29U,     /**< DMA0 reset control */
    kDMA1_RST_SHIFT_RSTn      = (RST_CTL0_PSCCTL0 << 8) | 30U,     /**< DMA1 reset control */
    kSDIO_RST_SHIFT_RSTn      = (RST_CTL0_PSCCTL0 << 8) | 31U,     /**< SDIO reset control */

    kELS_APB_RST_SHIFT_RSTn      = (RST_CTL0_PSCCTL1 << 8) | 0U,   /**< ELS_APB reset control */
    kELS_GDET_REF_RST_SHIFT_RSTn = (RST_CTL0_PSCCTL1 << 8) | 1U,   /**< ELS_GDET_REF_RST reset control */
    kSDIO_SLV_SHIFT_RSTn         = (RST_CTL0_PSCCTL1 << 8) | 2U,   /**< SDIO_SLV reset control */
    kGAU_RST_SHIFT_RSTn          = (RST_CTL0_PSCCTL1 << 8) | 16U,  /**< GAU reset control */
    kOTP_RST_SHIFT_RSTn          = (RST_CTL0_PSCCTL1 << 8) | 17U,  /**< OTP reset control */
    kSECURE_GPIO_RST_SHIFT_RSTn  = (RST_CTL0_PSCCTL1 << 8) | 24U,  /**< Security GPIO reset control */
    kENET_IPG_RST_SHIFT_RSTn     = (RST_CTL0_PSCCTL1 << 8) | 25U,  /**< ENET_IPG reset control */
    kENET_IPG_S_RST_SHIFT_RSTn   = (RST_CTL0_PSCCTL1 << 8) | 26U,  /**< ENET_IPG_S reset control */
    kTRNG_RST_SHIFT_RSTn         = (RST_CTL0_PSCCTL1 << 8) | 27U,  /**< TRNG reset control */

    kUTICK_RST_SHIFT_RSTn   = (RST_CTL0_PSCCTL2 << 8) | 0U,        /**< Micro-tick timer reset control */
    kWWDT_RST_SHIFT_RSTn    = (RST_CTL0_PSCCTL2 << 8) | 1U,        /**< Windowed Watchdog timer reset control */
    kUSIM_RST_SHIFT_RSTn    = (RST_CTL0_PSCCTL2 << 8) | 2U,        /**< USIM reset control */
    kFREEMRT_RST_SHIFT_RSTn = (RST_CTL0_PSCCTL2 << 8) | 26U,       /**< FREEMRT reset control */
    kLCDIC_RST_SHIFT_RSTn   = (RST_CTL0_PSCCTL2 << 8) | 27U,       /**< LCDIC reset control */

    kFC0_RST_SHIFT_RSTn           = (RST_CTL1_PSCCTL0 << 8) | 8U,  /**< Flexcomm Interface 0 reset control */
    kFC1_RST_SHIFT_RSTn           = (RST_CTL1_PSCCTL0 << 8) | 9U,  /**< Flexcomm Interface 1 reset control */
    kFC2_RST_SHIFT_RSTn           = (RST_CTL1_PSCCTL0 << 8) | 10U, /**< Flexcomm Interface 2 reset control */
    kFC3_RST_SHIFT_RSTn           = (RST_CTL1_PSCCTL0 << 8) | 11U, /**< Flexcomm Interface 3 reset control */
    kFC14_RST_SHIFT_RSTn          = (RST_CTL1_PSCCTL0 << 8) | 22U, /**< Flexcomm Interface 14 reset control */
    kDMIC_RST_SHIFT_RSTn          = (RST_CTL1_PSCCTL0 << 8) | 24U, /**< Digital microphone interface reset control */
    kOSEVENT_TIMER_RST_SHIFT_RSTn = (RST_CTL1_PSCCTL0 << 8) | 27U, /**< Osevent Timer reset control */

    kHSGPIO0_RST_SHIFT_RSTn = (RST_CTL1_PSCCTL1 << 8) | 0U,        /**< HSGPIO 0 reset control */
    kHSGPIO1_RST_SHIFT_RSTn = (RST_CTL1_PSCCTL1 << 8) | 1U,        /**< HSGPIO 1 reset control */
    kCRC_RST_SHIFT_RSTn     = (RST_CTL1_PSCCTL1 << 8) | 16U,       /**< CRC reset control */
    kFREQME_RST_SHIFT_RSTn  = (RST_CTL1_PSCCTL1 << 8) | 31U,       /**< Frequency Measure reset control */

    kCT32B0_RST_SHIFT_RSTn   = (RST_CTL1_PSCCTL2 << 8) | 0U,       /**< CT32B0 reset control */
    kCT32B1_RST_SHIFT_RSTn   = (RST_CTL1_PSCCTL2 << 8) | 1U,       /**< CT32B1 reset control */
    kCT32B2_RST_SHIFT_RSTn   = (RST_CTL1_PSCCTL2 << 8) | 2U,       /**< CT32B3 reset control */
    kCT32B3_RST_SHIFT_RSTn   = (RST_CTL1_PSCCTL2 << 8) | 3U,       /**< CT32B4 reset control */
    kCT32B4_RST_SHIFT_RSTn   = (RST_CTL1_PSCCTL2 << 8) | 4U,       /**< CT32B4 reset control */
    kMRT_RST_SHIFT_RSTn      = (RST_CTL1_PSCCTL2 << 8) | 8U,       /**< Multi-rate timer (MRT) reset control */
    kPINT_RST_SHIFT_RSTn     = (RST_CTL1_PSCCTL2 << 8) | 30U,      /**< GPIO_INT reset control */
    kINPUTMUX_RST_SHIFT_RSTn = (RST_CTL1_PSCCTL2 << 8) | 31U,      /**< PMUX reset control */
} RSTCTL_RSTn_t;

/** Array initializers with peripheral reset bits **/
#define CRC_RSTS            \
    {                       \
        kCRC_RST_SHIFT_RSTn \
    } /* Reset bits for CRC peripheral */
#define DMA_RSTS_N                                 \
    {                                              \
        kDMA0_RST_SHIFT_RSTn, kDMA1_RST_SHIFT_RSTn \
    } /* Reset bits for DMA peripheral */
#define DMIC_RSTS            \
    {                        \
        kDMIC_RST_SHIFT_RSTn \
    } /* Reset bits for ADC peripheral */
#define FLEXCOMM_RSTS                                                                                            \
    {                                                                                                            \
        kFC0_RST_SHIFT_RSTn, kFC1_RST_SHIFT_RSTn, kFC2_RST_SHIFT_RSTn, kFC3_RST_SHIFT_RSTn, kFC14_RST_SHIFT_RSTn \
    } /* Reset bits for FLEXCOMM peripheral */
#define GPIO_RSTS_N                                      \
    {                                                    \
        kHSGPIO0_RST_SHIFT_RSTn, kHSGPIO1_RST_SHIFT_RSTn \
    } /* Reset bits for GPIO peripheral */
#define MRT_RSTS                                     \
    {                                                \
        kMRT_RST_SHIFT_RSTn, kFREEMRT_RST_SHIFT_RSTn \
    } /* Reset bits for MRT peripheral */
#define PINT_RSTS            \
    {                        \
        kPINT_RST_SHIFT_RSTn \
    } /* Reset bits for PINT peripheral */
#define SCT_RSTS            \
    {                       \
        kSCT_RST_SHIFT_RSTn \
    } /* Reset bits for SCT peripheral */
#define CTIMER_RSTS                                                                                     \
    {                                                                                                   \
        kCT32B0_RST_SHIFT_RSTn, kCT32B1_RST_SHIFT_RSTn, kCT32B2_RST_SHIFT_RSTn, kCT32B3_RST_SHIFT_RSTn, \
            kCT32B4_RST_SHIFT_RSTn                                                                      \
    } /* Reset bits for TIMER peripheral */
#define USB_RSTS            \
    {                       \
        kUSB_RST_SHIFT_RSTn \
    } /* Reset bits for USB peripheral */
#define UTICK_RSTS            \
    {                         \
        kUTICK_RST_SHIFT_RSTn \
    } /* Reset bits for UTICK peripheral */
#define WWDT_RSTS            \
    {                        \
        kWWDT_RST_SHIFT_RSTn \
    } /* Reset bits for WWDT peripheral */
#define OSTIMER_RSTS                  \
    {                                 \
        kOSEVENT_TIMER_RST_SHIFT_RSTn \
    } /* Reset bits for OSTIMER peripheral */
#define POWERQUAD_RSTS            \
    {                             \
        kPOWERQUAD_RST_SHIFT_RSTn \
    } /* Reset bits for Powerquad peripheral */
#define PUF_RSTS            \
    {                       \
        kPUF_RST_SHIFT_RSTn \
    } /* Reset bits for PUF peripheral */
#define TRNG_RSTS            \
    {                        \
        kTRNG_RST_SHIFT_RSTn \
    } /* Reset bits for TRNG peripheral */
#define USIM_RSTS            \
    {                        \
        kUSIM_RST_SHIFT_RSTn \
    } /* Reset bits for USIM peripheral */
#define ENET_RSTS                                            \
    {                                                        \
        kENET_IPG_RST_SHIFT_RSTn, kENET_IPG_S_RST_SHIFT_RSTn \
    } /* Reset bits for ENET peripheral */

/*!
 * @brief IP reset handle
 */
typedef RSTCTL_RSTn_t reset_ip_name_t;

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
