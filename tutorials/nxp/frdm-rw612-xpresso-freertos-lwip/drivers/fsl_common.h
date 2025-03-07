/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2022,2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_COMMON_H_
#define FSL_COMMON_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if defined(__ICCARM__) || (defined(__CC_ARM) || defined(__ARMCC_VERSION)) || defined(__GNUC__)
#include <stddef.h>
#endif

#include "fsl_device_registers.h"

/*!
 * @addtogroup ksdk_common
 * @{
 */

/*******************************************************************************
 * Configurations
 ******************************************************************************/

/*! @brief Macro to use the default weak IRQ handler in drivers. */
#ifndef FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ
#define FSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ 1
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100L) + (code)))

/*! @brief Construct the version number for drivers.
 *
 * The driver version is a 32-bit number, for both 32-bit platforms(such as Cortex M)
 * and 16-bit platforms(such as DSC).
 *
 * @verbatim

   | Unused    || Major Version || Minor Version ||  Bug Fix    |
   31        25  24           17  16            9  8            0

   @endverbatim
 */
#define MAKE_VERSION(major, minor, bugfix) (((major)*65536L) + ((minor)*256L) + (bugfix))

/*! @name Driver version */
/*! @{ */
/*! @brief common driver version. */
#define FSL_COMMON_DRIVER_VERSION (MAKE_VERSION(2, 5, 0))
/*! @} */

/*! @name Debug console type definition. */
/*! @{ */
#define DEBUG_CONSOLE_DEVICE_TYPE_NONE       0U  /*!< No debug console.             */
#define DEBUG_CONSOLE_DEVICE_TYPE_UART       1U  /*!< Debug console based on UART.   */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPUART     2U  /*!< Debug console based on LPUART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPSCI      3U  /*!< Debug console based on LPSCI.  */
#define DEBUG_CONSOLE_DEVICE_TYPE_USBCDC     4U  /*!< Debug console based on USBCDC. */
#define DEBUG_CONSOLE_DEVICE_TYPE_FLEXCOMM   5U  /*!< Debug console based on FLEXCOMM. */
#define DEBUG_CONSOLE_DEVICE_TYPE_IUART      6U  /*!< Debug console based on i.MX UART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_VUSART     7U  /*!< Debug console based on LPC_VUSART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_MINI_USART 8U  /*!< Debug console based on LPC_USART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_SWO        9U  /*!< Debug console based on SWO. */
#define DEBUG_CONSOLE_DEVICE_TYPE_QSCI       10U /*!< Debug console based on QSCI. */
/*! @} */

/*! @brief Status group numbers. */
enum _status_groups
{
    kStatusGroup_Generic               = 0,   /*!< Group number for generic status codes. */
    kStatusGroup_FLASH                 = 1,   /*!< Group number for FLASH status codes. */
    kStatusGroup_LPSPI                 = 4,   /*!< Group number for LPSPI status codes. */
    kStatusGroup_FLEXIO_SPI            = 5,   /*!< Group number for FLEXIO SPI status codes. */
    kStatusGroup_DSPI                  = 6,   /*!< Group number for DSPI status codes. */
    kStatusGroup_FLEXIO_UART           = 7,   /*!< Group number for FLEXIO UART status codes. */
    kStatusGroup_FLEXIO_I2C            = 8,   /*!< Group number for FLEXIO I2C status codes. */
    kStatusGroup_LPI2C                 = 9,   /*!< Group number for LPI2C status codes. */
    kStatusGroup_UART                  = 10,  /*!< Group number for UART status codes. */
    kStatusGroup_I2C                   = 11,  /*!< Group number for UART status codes. */
    kStatusGroup_LPSCI                 = 12,  /*!< Group number for LPSCI status codes. */
    kStatusGroup_LPUART                = 13,  /*!< Group number for LPUART status codes. */
    kStatusGroup_SPI                   = 14,  /*!< Group number for SPI status code.*/
    kStatusGroup_XRDC                  = 15,  /*!< Group number for XRDC status code.*/
    kStatusGroup_SEMA42                = 16,  /*!< Group number for SEMA42 status code.*/
    kStatusGroup_SDHC                  = 17,  /*!< Group number for SDHC status code */
    kStatusGroup_SDMMC                 = 18,  /*!< Group number for SDMMC status code */
    kStatusGroup_SAI                   = 19,  /*!< Group number for SAI status code */
    kStatusGroup_MCG                   = 20,  /*!< Group number for MCG status codes. */
    kStatusGroup_SCG                   = 21,  /*!< Group number for SCG status codes. */
    kStatusGroup_SDSPI                 = 22,  /*!< Group number for SDSPI status codes. */
    kStatusGroup_FLEXIO_I2S            = 23,  /*!< Group number for FLEXIO I2S status codes */
    kStatusGroup_FLEXIO_MCULCD         = 24,  /*!< Group number for FLEXIO LCD status codes */
    kStatusGroup_FLASHIAP              = 25,  /*!< Group number for FLASHIAP status codes */
    kStatusGroup_FLEXCOMM_I2C          = 26,  /*!< Group number for FLEXCOMM I2C status codes */
    kStatusGroup_I2S                   = 27,  /*!< Group number for I2S status codes */
    kStatusGroup_IUART                 = 28,  /*!< Group number for IUART status codes */
    kStatusGroup_CSI                   = 29,  /*!< Group number for CSI status codes */
    kStatusGroup_MIPI_DSI              = 30,  /*!< Group number for MIPI DSI status codes */
    kStatusGroup_SDRAMC                = 35,  /*!< Group number for SDRAMC status codes. */
    kStatusGroup_POWER                 = 39,  /*!< Group number for POWER status codes. */
    kStatusGroup_ENET                  = 40,  /*!< Group number for ENET status codes. */
    kStatusGroup_PHY                   = 41,  /*!< Group number for PHY status codes. */
    kStatusGroup_TRGMUX                = 42,  /*!< Group number for TRGMUX status codes. */
    kStatusGroup_SMARTCARD             = 43,  /*!< Group number for SMARTCARD status codes. */
    kStatusGroup_LMEM                  = 44,  /*!< Group number for LMEM status codes. */
    kStatusGroup_QSPI                  = 45,  /*!< Group number for QSPI status codes. */
    kStatusGroup_DMA                   = 50,  /*!< Group number for DMA status codes. */
    kStatusGroup_EDMA                  = 51,  /*!< Group number for EDMA status codes. */
    kStatusGroup_DMAMGR                = 52,  /*!< Group number for DMAMGR status codes. */
    kStatusGroup_FLEXCAN               = 53,  /*!< Group number for FlexCAN status codes. */
    kStatusGroup_LTC                   = 54,  /*!< Group number for LTC status codes. */
    kStatusGroup_FLEXIO_CAMERA         = 55,  /*!< Group number for FLEXIO CAMERA status codes. */
    kStatusGroup_LPC_SPI               = 56,  /*!< Group number for LPC_SPI status codes. */
    kStatusGroup_LPC_USART             = 57,  /*!< Group number for LPC_USART status codes. */
    kStatusGroup_DMIC                  = 58,  /*!< Group number for DMIC status codes. */
    kStatusGroup_SDIF                  = 59,  /*!< Group number for SDIF status codes.*/
    kStatusGroup_SPIFI                 = 60,  /*!< Group number for SPIFI status codes. */
    kStatusGroup_OTP                   = 61,  /*!< Group number for OTP status codes. */
    kStatusGroup_MCAN                  = 62,  /*!< Group number for MCAN status codes. */
    kStatusGroup_CAAM                  = 63,  /*!< Group number for CAAM status codes. */
    kStatusGroup_ECSPI                 = 64,  /*!< Group number for ECSPI status codes. */
    kStatusGroup_USDHC                 = 65,  /*!< Group number for USDHC status codes.*/
    kStatusGroup_LPC_I2C               = 66,  /*!< Group number for LPC_I2C status codes.*/
    kStatusGroup_DCP                   = 67,  /*!< Group number for DCP status codes.*/
    kStatusGroup_MSCAN                 = 68,  /*!< Group number for MSCAN status codes.*/
    kStatusGroup_ESAI                  = 69,  /*!< Group number for ESAI status codes. */
    kStatusGroup_FLEXSPI               = 70,  /*!< Group number for FLEXSPI status codes. */
    kStatusGroup_MMDC                  = 71,  /*!< Group number for MMDC status codes. */
    kStatusGroup_PDM                   = 72,  /*!< Group number for MIC status codes. */
    kStatusGroup_SDMA                  = 73,  /*!< Group number for SDMA status codes. */
    kStatusGroup_ICS                   = 74,  /*!< Group number for ICS status codes. */
    kStatusGroup_SPDIF                 = 75,  /*!< Group number for SPDIF status codes. */
    kStatusGroup_LPC_MINISPI           = 76,  /*!< Group number for LPC_MINISPI status codes. */
    kStatusGroup_HASHCRYPT             = 77,  /*!< Group number for Hashcrypt status codes */
    kStatusGroup_LPC_SPI_SSP           = 78,  /*!< Group number for LPC_SPI_SSP status codes. */
    kStatusGroup_I3C                   = 79,  /*!< Group number for I3C status codes */
    kStatusGroup_LPC_I2C_1             = 97,  /*!< Group number for LPC_I2C_1 status codes. */
    kStatusGroup_NOTIFIER              = 98,  /*!< Group number for NOTIFIER status codes. */
    kStatusGroup_DebugConsole          = 99,  /*!< Group number for debug console status codes. */
    kStatusGroup_SEMC                  = 100, /*!< Group number for SEMC status codes. */
    kStatusGroup_ApplicationRangeStart = 101, /*!< Starting number for application groups. */
    kStatusGroup_IAP                   = 102, /*!< Group number for IAP status codes */
    kStatusGroup_SFA                   = 103, /*!< Group number for SFA status codes*/
    kStatusGroup_SPC                   = 104, /*!< Group number for SPC status codes. */
    kStatusGroup_PUF                   = 105, /*!< Group number for PUF status codes. */
    kStatusGroup_TOUCH_PANEL           = 106, /*!< Group number for touch panel status codes */
    kStatusGroup_VBAT                  = 107, /*!< Group number for VBAT status codes */
    kStatusGroup_XSPI                  = 108, /*!< Group number for XSPI status codes */
    kStatusGroup_PNGDEC                = 109, /*!< Group number for PNGDEC status codes */
    kStatusGroup_JPEGDEC               = 110, /*!< Group number for JPEGDEC status codes */

    kStatusGroup_HAL_GPIO       = 121, /*!< Group number for HAL GPIO status codes. */
    kStatusGroup_HAL_UART       = 122, /*!< Group number for HAL UART status codes. */
    kStatusGroup_HAL_TIMER      = 123, /*!< Group number for HAL TIMER status codes. */
    kStatusGroup_HAL_SPI        = 124, /*!< Group number for HAL SPI status codes. */
    kStatusGroup_HAL_I2C        = 125, /*!< Group number for HAL I2C status codes. */
    kStatusGroup_HAL_FLASH      = 126, /*!< Group number for HAL FLASH status codes. */
    kStatusGroup_HAL_PWM        = 127, /*!< Group number for HAL PWM status codes. */
    kStatusGroup_HAL_RNG        = 128, /*!< Group number for HAL RNG status codes. */
    kStatusGroup_HAL_I2S        = 129, /*!< Group number for HAL I2S status codes. */
    kStatusGroup_HAL_ADC_SENSOR = 130, /*!< Group number for HAL ADC SENSOR status codes. */
    kStatusGroup_TIMERMANAGER   = 135, /*!< Group number for TiMER MANAGER status codes. */
    kStatusGroup_SERIALMANAGER  = 136, /*!< Group number for SERIAL MANAGER status codes. */
    kStatusGroup_LED            = 137, /*!< Group number for LED status codes. */
    kStatusGroup_BUTTON         = 138, /*!< Group number for BUTTON status codes. */
    kStatusGroup_EXTERN_EEPROM  = 139, /*!< Group number for EXTERN EEPROM status codes. */
    kStatusGroup_SHELL          = 140, /*!< Group number for SHELL status codes. */
    kStatusGroup_MEM_MANAGER    = 141, /*!< Group number for MEM MANAGER status codes. */
    kStatusGroup_LIST           = 142, /*!< Group number for List status codes. */
    kStatusGroup_OSA            = 143, /*!< Group number for OSA status codes. */
    kStatusGroup_COMMON_TASK    = 144, /*!< Group number for Common task status codes. */
    kStatusGroup_MSG            = 145, /*!< Group number for messaging status codes. */
    kStatusGroup_SDK_OCOTP      = 146, /*!< Group number for OCOTP status codes. */
    kStatusGroup_SDK_FLEXSPINOR = 147, /*!< Group number for FLEXSPINOR status codes.*/
    kStatusGroup_CODEC          = 148, /*!< Group number for codec status codes. */
    kStatusGroup_ASRC           = 149, /*!< Group number for codec status ASRC. */
    kStatusGroup_OTFAD          = 150, /*!< Group number for codec status codes. */
    kStatusGroup_SDIOSLV        = 151, /*!< Group number for SDIOSLV status codes. */
    kStatusGroup_MECC           = 152, /*!< Group number for MECC status codes. */
    kStatusGroup_ENET_QOS       = 153, /*!< Group number for ENET_QOS status codes. */
    kStatusGroup_LOG            = 154, /*!< Group number for LOG status codes. */
    kStatusGroup_I3CBUS         = 155, /*!< Group number for I3CBUS status codes. */
    kStatusGroup_QSCI           = 156, /*!< Group number for QSCI status codes. */
    kStatusGroup_ELEMU          = 157, /*!< Group number for ELEMU status codes. */
    kStatusGroup_QUEUEDSPI      = 158, /*!< Group number for QSPI status codes. */
    kStatusGroup_POWER_MANAGER  = 159, /*!< Group number for POWER_MANAGER status codes. */
    kStatusGroup_IPED           = 160, /*!< Group number for IPED status codes. */
    kStatusGroup_ELS_PKC        = 161, /*!< Group number for ELS PKC status codes. */
    kStatusGroup_CSS_PKC        = 162, /*!< Group number for CSS PKC status codes. */
    kStatusGroup_HOSTIF         = 163, /*!< Group number for HOSTIF status codes. */
    kStatusGroup_CLIF           = 164, /*!< Group number for CLIF status codes. */
    kStatusGroup_BMA            = 165, /*!< Group number for BMA status codes. */
    kStatusGroup_NETC           = 166, /*!< Group number for NETC status codes. */
    kStatusGroup_ELE            = 167, /*!< Group number for ELE status codes. */
    kStatusGroup_GLIKEY         = 168, /*!< Group number for GLIKEY status codes. */
    kStatusGroup_AON_POWER      = 169, /*!< Group number for AON_POWER status codes. */
    kStatusGroup_AON_COMMON     = 170, /*!< Group number for AON_COMMON status codes. */
};

/*! \public
 * @brief Generic status return codes.
 */
enum
{
    kStatus_Success         = MAKE_STATUS(kStatusGroup_Generic, 0), /*!< Generic status for Success. */
    kStatus_Fail            = MAKE_STATUS(kStatusGroup_Generic, 1), /*!< Generic status for Fail. */
    kStatus_ReadOnly        = MAKE_STATUS(kStatusGroup_Generic, 2), /*!< Generic status for read only failure. */
    kStatus_OutOfRange      = MAKE_STATUS(kStatusGroup_Generic, 3), /*!< Generic status for out of range access. */
    kStatus_InvalidArgument = MAKE_STATUS(kStatusGroup_Generic, 4), /*!< Generic status for invalid argument check. */
    kStatus_Timeout         = MAKE_STATUS(kStatusGroup_Generic, 5), /*!< Generic status for timeout. */
    kStatus_NoTransferInProgress =
        MAKE_STATUS(kStatusGroup_Generic, 6),            /*!< Generic status for no transfer in progress. */
    kStatus_Busy = MAKE_STATUS(kStatusGroup_Generic, 7), /*!< Generic status for module is busy. */
    kStatus_NoData =
        MAKE_STATUS(kStatusGroup_Generic, 8), /*!< Generic status for no data is found for the operation. */
};

/*! @brief Type used for all status and error return values. */
typedef int32_t status_t;

#ifdef __ZEPHYR__
#include <zephyr/sys/util.h>
#else
/*!
 * @name Min/max macros
 * @{
 */
#if !defined(MIN)
/*! Computes the minimum of \a a and \a b. */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MAX)
/*! Computes the maximum of \a a and \a b. */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
/*! @} */

/*! @brief Computes the number of elements in an array. */
#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
#endif /* __ZEPHYR__ */

/*! @name UINT16_MAX/UINT32_MAX value */
/*! @{ */
#if !defined(UINT16_MAX)
/*! Max value of uint16_t type. */
#define UINT16_MAX ((uint16_t)-1)
#endif

#if !defined(UINT32_MAX)
/*! Max value of uint32_t type. */
#define UINT32_MAX ((uint32_t)-1)
#endif
/*! @} */

/*! Macro to get upper 32 bits of a 64-bit value */
#if !defined(UINT64_H)
#define UINT64_H(X)        ((uint32_t)((((uint64_t) (X)) >> 32U) & 0x0FFFFFFFFULL))
#endif

/*! Macro to get lower 32 bits of a 64-bit value */
#if !defined(UINT64_L)
#define UINT64_L(X)        ((uint32_t)(((uint64_t) (X)) & 0x0FFFFFFFFULL))
#endif

/*!
 * @def SUPPRESS_FALL_THROUGH_WARNING()
 *
 * For switch case code block, if case section ends without "break;" statement, there wil be
 * fallthrough warning with compiler flag -Wextra or -Wimplicit-fallthrough=n when using armgcc.
 * To suppress this warning, "SUPPRESS_FALL_THROUGH_WARNING();" need to be added at the end of each
 * case section which misses "break;"statement.
 */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#define SUPPRESS_FALL_THROUGH_WARNING() __attribute__((fallthrough))
#else
#define SUPPRESS_FALL_THROUGH_WARNING()
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if !((defined(__DSC__) && defined(__CW__)))
/*!
 * @brief Allocate memory with given alignment and aligned size.
 *
 * This is provided to support the dynamically allocated memory
 * used in cache-able region.
 * @param size The length required to malloc.
 * @param alignbytes The alignment size.
 * @retval The allocated memory.
 */
void *SDK_Malloc(size_t size, size_t alignbytes);

/*!
 * @brief Free memory.
 *
 * @param ptr The memory to be release.
 */
void SDK_Free(void *ptr);
#endif

/*!
 * @brief Delay at least for some time.
 *  Please note that, this API uses while loop for delay, different run-time environments make the time not precise,
 *  if precise delay count was needed, please implement a new delay function with hardware timer.
 *
 * @param delayTime_us  Delay time in unit of microsecond.
 * @param coreClock_Hz  Core clock frequency with Hz.
 */
void SDK_DelayAtLeastUs(uint32_t delayTime_us, uint32_t coreClock_Hz);

#if defined(__cplusplus)
}
#endif

/*! @} */

#if (defined(__DSC__) && defined(__CW__))
#include "fsl_common_dsc.h"
#elif defined(__XTENSA__)
#include "fsl_common_dsp.h"
#elif defined(__riscv)
#include "fsl_common_riscv.h"
#else
#include "fsl_common_arm.h"
#endif

#endif /* FSL_COMMON_H_ */
