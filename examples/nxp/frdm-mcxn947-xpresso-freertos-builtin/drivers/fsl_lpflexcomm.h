/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FSL_LP_FLEXCOMM_H_
#define FSL_LP_FLEXCOMM_H_

#include "fsl_common.h"

/*!
 * @addtogroup lpflexcomm_driver
 * @{
 */

/*! @name Driver version */
/*@{*/
/*! @brief FlexCOMM driver version. */
#define FSL_LP_FLEXCOMM_DRIVER_VERSION (MAKE_VERSION(2, 2, 1))
/*@}*/

/*! @brief LP_FLEXCOMM peripheral modes. */
typedef enum
{
    LP_FLEXCOMM_PERIPH_NONE,               /*!< No peripheral */
    LP_FLEXCOMM_PERIPH_LPUART,             /*!< LPUART peripheral */
    LP_FLEXCOMM_PERIPH_LPSPI,              /*!< LPSPI Peripheral */
    LP_FLEXCOMM_PERIPH_LPI2C,              /*!< LPI2C Peripheral */
    LP_FLEXCOMM_PERIPH_LPI2CAndLPUART = 7, /*!< LPI2C and LPUART Peripheral */
} LP_FLEXCOMM_PERIPH_T;

/*! @brief LP_FLEXCOMM interrupt source flags. */
enum _lpflexcomm_interrupt_flag
{
    kLPFLEXCOMM_I2cSlaveInterruptFlag  = LP_FLEXCOMM_ISTAT_I2CS_MASK,   /* LPI2C slave interrupt. */
    kLPFLEXCOMM_I2cMasterInterruptFlag = LP_FLEXCOMM_ISTAT_I2CM_MASK,   /* LPI2C master interrupt. */
    kLPFLEXCOMM_SpiInterruptFlag       = LP_FLEXCOMM_ISTAT_SPI_MASK,    /* LPSPI interrupt. */
    kLPFLEXCOMM_UartRxInterruptFlag    = LP_FLEXCOMM_ISTAT_UARTRX_MASK, /* LPUART RX interrupt. */
    kLPFLEXCOMM_UartTxInterruptFlag    = LP_FLEXCOMM_ISTAT_UARTTX_MASK, /* LPUART TX interrupt. */

    kLPFLEXCOMM_AllInterruptFlag = kLPFLEXCOMM_I2cSlaveInterruptFlag | kLPFLEXCOMM_I2cMasterInterruptFlag |
                                   kLPFLEXCOMM_SpiInterruptFlag | kLPFLEXCOMM_UartRxInterruptFlag |
                                   kLPFLEXCOMM_UartTxInterruptFlag,
};

/*! @brief Typedef for interrupt handler. */
typedef void (*lpflexcomm_irq_handler_t)(uint32_t instance, void *handle);

/*! @brief Array with IRQ number for each LP_FLEXCOMM module. */
extern IRQn_Type const kFlexcommIrqs[];

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*! @brief Returns instance number for LP_FLEXCOMM module with given base address. */
uint32_t LP_FLEXCOMM_GetInstance(void *base);

/*! @brief Returns for LP_FLEXCOMM base address. */
uint32_t LP_FLEXCOMM_GetBaseAddress(uint32_t instance);

/*! brief Returns for LP_FLEXCOMM interrupt source,see #_lpflexcomm_interrupt_flag. */
uint32_t LP_FLEXCOMM_GetInterruptStatus(uint32_t instance);

/*! @brief Initializes LP_FLEXCOMM and selects peripheral mode according to the second parameter. */
status_t LP_FLEXCOMM_Init(uint32_t instance, LP_FLEXCOMM_PERIPH_T periph);

/*! @brief Deinitializes LP_FLEXCOMM. */
void LP_FLEXCOMM_Deinit(uint32_t instance);

/*! @brief Sets IRQ handler for given LP_FLEXCOMM module. It is used by drivers register IRQ handler according to
 * LP_FLEXCOMM mode */
void LP_FLEXCOMM_SetIRQHandler(uint32_t instance,
                               lpflexcomm_irq_handler_t handler,
                               void *lpflexcommHandle,
                               LP_FLEXCOMM_PERIPH_T periph);

#if defined(__cplusplus)
}
#endif

/*@}*/

#endif /* FSL_LP_FLEXCOMM_H_*/
