/*
 * Copyright 2022-2024 NXP
 *  
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_IO_MUX_H_
#define _FSL_IO_MUX_H_

#include "fsl_common.h"

/*!
 * @addtogroup io_mux
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.cns_io_mux"
#endif

/*! @name Driver version */
/*@{*/
/*! @brief IO_MUX driver version 2.2.0. */
#define FSL_IO_MUX_DRIVER_VERSION (MAKE_VERSION(2, 2, 0))
/*@}*/

/*!
 * @name Pin function ID
 * The pin function ID is a tuple of \<GPIO_0_31_Mask GPIO_32_63_Mask GPIO_FC_SetMask GPIO_FC_ClearMask FSEL_SetMask
 * FSEL_ClearMask CTimer_SetMask CTimer_ClearMask SCTimerSetMask SCTimerClearMask\>
 *
 * GPIO_FC_xxxMask:   bit[0:10] maps to FCn[0:10];
 *                    bit[15:12] is the register offset from FC0;
 *                    bit[16] indicates GPIO should be operated;
 *                    bit[17] indicates SGPIO need to be operated.
 * CTimer_xxxMask:    bit[0:14] maps to C_TIMER_IN[0:14];
 *                    bit[16:30] maps to C_TIMER_OUT[0:14].
 * @{
 */
#define IO_MUX_GPIO_FC_MASK(gpio, fcIdx, fcMsk) \
    (((uint32_t)(gpio) << 16) | (((uint32_t)(fcIdx)&0xFUL) << 12) | ((uint32_t)(fcMsk)&0xFFFUL))
#define IO_MUX_SGPIO_FLAG(mask) (((uint32_t)(mask) >> 17) & 1UL)
#define IO_MUX_GPIO_FLAG(mask)  (((uint32_t)(mask) >> 16) & 1UL)
#define IO_MUX_FC_OFFSET(mask)  (((uint32_t)(mask) >> 12) & 0xFUL)
#define IO_MUX_FC_MASK(mask)    ((uint32_t)(mask)&0x7FFUL)

#define IO_MUX_CTIMER_MASK(inMsk, outMsk) (((uint32_t)(outMsk) << 16) | ((uint32_t)(inMsk)&0xFFFFUL))
#define IO_MUX_CTIMER_IN_MASK(mask)       ((uint32_t)(mask)&0x7FFFUL)
#define IO_MUX_CTIMER_OUT_MASK(mask)      (((uint32_t)(mask) >> 16) & 0x7FFFUL)

#define IO_MUX_SCTIMER_MASK(inMsk, outMsk) ((((uint32_t)(outMsk)&0x3FFUL) << 16) | ((uint32_t)(inMsk)&0xFFUL))

#define IO_MUX_FC0_USART_SCK                                                                   \
    0x00000010U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x001), IO_MUX_GPIO_FC_MASK(1, 0, 0x02C), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x2, 0x2)            /* SCTimer mask */

#define IO_MUX_FC0_USART_DATA                                                                  \
    0x0000000CU, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x200), IO_MUX_GPIO_FC_MASK(1, 0, 0x12E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x1, 0x1)            /* SCTimer mask */

#define IO_MUX_FC0_USART_CMD                                                                   \
    0x00000021U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x400), IO_MUX_GPIO_FC_MASK(1, 0, 0x038), /* GPIO&FC mask */ \
        0x00000000U, 0x00000040U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1, 0x1),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC0_I2C_2_3                                                                     \
    0x0000000CU, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x002), IO_MUX_GPIO_FC_MASK(1, 0, 0x32C), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x1, 0x1)            /* SCTimer mask */

#define IO_MUX_FC0_I2S                                                                         \
    0x0000001CU, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x004), IO_MUX_GPIO_FC_MASK(1, 0, 0x32B), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x3, 0x3)            /* SCTimer mask */

#define IO_MUX_FC0_I2S_DATA                                                                    \
    0x00000004U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x100), IO_MUX_GPIO_FC_MASK(1, 0, 0x22E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC0_SPI_SS0                                                                     \
    0x0000001DU, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0x008), IO_MUX_GPIO_FC_MASK(1, 0, 0x737), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1, 0x1),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x3, 0x3)            /* SCTimer mask */

#define IO_MUX_FC1_USART_SCK                                                                   \
    0x00000080U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x001), IO_MUX_GPIO_FC_MASK(1, 1, 0x0EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_USART_DATA                                                                  \
    0x00000300U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x200), IO_MUX_GPIO_FC_MASK(1, 1, 0x1EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_USART_CMD                                                                   \
    0x00000440U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x400), IO_MUX_GPIO_FC_MASK(1, 1, 0x038), /* GPIO&FC mask */ \
        0x00000000U, 0x01000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_I2C_8_9                                                                     \
    0x00000300U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x002), IO_MUX_GPIO_FC_MASK(1, 1, 0x3EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_I2S                                                                         \
    0x00000380U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x004), IO_MUX_GPIO_FC_MASK(1, 1, 0x3EB), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_I2S_DATA                                                                    \
    0x00000200U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x100), IO_MUX_GPIO_FC_MASK(1, 1, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC1_SPI_SS0                                                                     \
    0x000003C0U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 1, 0x008), IO_MUX_GPIO_FC_MASK(1, 1, 0x7F7), /* GPIO&FC mask */ \
        0x00000000U, 0x01000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_USART_SCK                                                                   \
    0x00008000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x001), IO_MUX_GPIO_FC_MASK(1, 2, 0x0EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00008400U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_USART_DATA                                                                  \
    0x00006000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x200), IO_MUX_GPIO_FC_MASK(1, 2, 0x1EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x18, 0x18),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_USART_CMD                                                                   \
    0x00030000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x400), IO_MUX_GPIO_FC_MASK(1, 2, 0x038), /* GPIO&FC mask */ \
        0x00000000U, 0x00008400U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_I2C_13_14                                                                   \
    0x00006000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x002), IO_MUX_GPIO_FC_MASK(1, 2, 0x3EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x18, 0x18),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_I2C_16_17                                                                   \
    0x00030000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x010), IO_MUX_GPIO_FC_MASK(1, 2, 0x428), /* GPIO&FC mask */ \
        0x00000000U, 0x00008400U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_I2S                                                                         \
    0x0000E000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x004), IO_MUX_GPIO_FC_MASK(1, 2, 0x3EB), /* GPIO&FC mask */ \
        0x00000000U, 0x00008400U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x18, 0x18),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_I2S_DATA                                                                    \
    0x00002000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x100), IO_MUX_GPIO_FC_MASK(1, 2, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x18, 0x18),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC2_SPI_SS0                                                                     \
    0x0001E000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 2, 0x008), IO_MUX_GPIO_FC_MASK(1, 2, 0x7F7), /* GPIO&FC mask */ \
        0x00000000U, 0x00008400U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x18, 0x18),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC3_USART_SCK                                                                   \
    0x02000000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x001), IO_MUX_GPIO_FC_MASK(1, 3, 0x0EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00010000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x80, 0x80),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC3_USART_DATA                                                                  \
    0x05000000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x200), IO_MUX_GPIO_FC_MASK(1, 3, 0x1EE), /* GPIO&FC mask */ \
        0x00000000U, 0xC0000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x40, 0x40),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)          /* SCTimer mask */

#define IO_MUX_FC3_USART_CMD                                                                   \
    0x00180000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x400), IO_MUX_GPIO_FC_MASK(1, 3, 0x038), /* GPIO&FC mask */ \
        0x00000000U, 0x00008000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC3_I2C_24_26                                                                   \
    0x05000000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x002), IO_MUX_GPIO_FC_MASK(1, 3, 0x3EC), /* GPIO&FC mask */ \
        0x00000000U, 0xC0000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x40, 0x40),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)          /* SCTimer mask */

#define IO_MUX_FC3_I2C_19_20                                                                   \
    0x00180000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x010), IO_MUX_GPIO_FC_MASK(1, 3, 0x428), /* GPIO&FC mask */ \
        0x00000000U, 0x00008000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC3_I2S                                                                         \
    0x07000000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x004), IO_MUX_GPIO_FC_MASK(1, 3, 0x3EB), /* GPIO&FC mask */ \
        0x00000000U, 0xC0010000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0xC0, 0xC0),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)          /* SCTimer mask */

#define IO_MUX_FC3_I2S_DATA                                                                    \
    0x01000000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x100), IO_MUX_GPIO_FC_MASK(1, 3, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x40000000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x40, 0x40),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC3_SPI_SS0                                                                     \
    0x07100000U, 0x00000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 3, 0x008), IO_MUX_GPIO_FC_MASK(1, 3, 0x7F7), /* GPIO&FC mask */ \
        0x00000000U, 0xC0018000U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0xC0, 0xC0),           /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)          /* SCTimer mask */

#define IO_MUX_FC14_USART_SCK                                                                  \
    0x00000000U, 0x00400000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x001), IO_MUX_GPIO_FC_MASK(3, 6, 0x0EC), /* GPIO&FC mask */ \
        0x00000000U, 0x00200810U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4000, 0x4000),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC14_USART_DATA                                                                 \
    0x00000000U, 0x03000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x200), IO_MUX_GPIO_FC_MASK(3, 6, 0x1EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC14_USART_CMD                                                                  \
    0x00000000U, 0x00A00000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x400), IO_MUX_GPIO_FC_MASK(3, 6, 0x038), /* GPIO&FC mask */ \
        0x00000000U, 0x00500810U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x2000, 0x2000),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0x200)            /* SCTimer mask */

#define IO_MUX_FC14_I2C_56_57                                                                  \
    0x00000000U, 0x03000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x002), IO_MUX_GPIO_FC_MASK(3, 6, 0x3EC), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC14_I2S                                                                        \
    0x00000000U, 0x03400000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x004), IO_MUX_GPIO_FC_MASK(3, 6, 0x3EB), /* GPIO&FC mask */ \
        0x00000000U, 0x08200810U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4000, 0x4000),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC14_I2S_DATA                                                                   \
    0x00000000U, 0x02000000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x100), IO_MUX_GPIO_FC_MASK(3, 6, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),                 /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_FC14_SPI_SS0                                                                    \
    0x00000000U, 0x03600000U,                                               /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 6, 0x008), IO_MUX_GPIO_FC_MASK(3, 6, 0x7F7), /* GPIO&FC mask */ \
        0x00000000U, 0x08300810U,                                           /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x6000, 0x6000),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)                /* SCTimer mask */

#define IO_MUX_QUAD_SPI_FLASH                                                          \
    0xF0000000U, 0x00000007U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000002U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_QUAD_SPI_PSRAM                                                          \
    0x00000000U, 0x000003F8U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000008U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x700, 0x700), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0xC0, 0xC0)  /* SCTimer mask */

#define IO_MUX_PDM                                                                         \
    0x00000000U, 0x00780000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x4FD), /* GPIO&FC mask */ \
        0x00000010U, 0x003C0800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x7800, 0x7800),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_USB                                                                         \
    0x00001000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 1, 0x080), /* GPIO&FC mask */ \
        0x00000020U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4, 0x4),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SCT_OUT_0                                                                   \
    0x00000008U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x22E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x1), IO_MUX_SCTIMER_MASK(0x1, 0)        /* SCTimer mask */

#define IO_MUX_SCT_OUT_1                                                                   \
    0x00000010U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x02D), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x2), IO_MUX_SCTIMER_MASK(0x2, 0)        /* SCTimer mask */

#define IO_MUX_SCT_OUT_8                                                                   \
    0x00000800U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 1, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x100), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SCT_OUT_4                                                                   \
    0x04000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x80000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x10), IO_MUX_SCTIMER_MASK(0x10, 0)      /* SCTimer mask */

#define IO_MUX_SCT_OUT_5                                                               \
    0x08000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x10000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x20), IO_MUX_SCTIMER_MASK(0x20, 0)  /* SCTimer mask */

#define IO_MUX_SCT_OUT_6                                                               \
    0x00000000U, 0x00000008U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x40), IO_MUX_SCTIMER_MASK(0x40, 0)  /* SCTimer mask */

#define IO_MUX_SCT_OUT_7                                                               \
    0x00000000U, 0x00000010U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x80), IO_MUX_SCTIMER_MASK(0x80, 0)  /* SCTimer mask */

#define IO_MUX_SCT_OUT_9                                                                   \
    0x00000000U, 0x00800000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00400000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0x200), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SCT_IN_0                                                                    \
    0x00000008U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x22E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x1, 0), IO_MUX_SCTIMER_MASK(0, 0x1)        /* SCTimer mask */

#define IO_MUX_SCT_IN_1                                                                    \
    0x00000010U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x02D), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x2, 0), IO_MUX_SCTIMER_MASK(0, 0x2)        /* SCTimer mask */

#define IO_MUX_SCT_IN_2                                                                    \
    0x00400000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x4, 0), IO_MUX_SCTIMER_MASK(0, 0)          /* SCTimer mask */

#define IO_MUX_SCT_IN_3                                                                    \
    0x00800000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x8, 0), IO_MUX_SCTIMER_MASK(0, 0)          /* SCTimer mask */

#define IO_MUX_SCT_IN_4                                                                    \
    0x04000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x80000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x10, 0), IO_MUX_SCTIMER_MASK(0, 0x10)      /* SCTimer mask */

#define IO_MUX_SCT_IN_5                                                                \
    0x08000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x10000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x20, 0), IO_MUX_SCTIMER_MASK(0, 0x20)  /* SCTimer mask */

#define IO_MUX_SCT_IN_6                                                                \
    0x00000000U, 0x00000008U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x40, 0), IO_MUX_SCTIMER_MASK(0, 0x40)  /* SCTimer mask */

#define IO_MUX_SCT_IN_7                                                                \
    0x00000000U, 0x00000010U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0x80, 0), IO_MUX_SCTIMER_MASK(0, 0x80)  /* SCTimer mask */

#define IO_MUX_CT0_MAT0_OUT                                                                \
    0x00000001U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x1), IO_MUX_CTIMER_MASK(0x1, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT0_MAT1_OUT                                                            \
    0x00000002U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x2), IO_MUX_CTIMER_MASK(0x2, 0),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT0_MAT2_OUT                                                                \
    0x00001000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 1, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00000020U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x4), IO_MUX_CTIMER_MASK(0x4, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT0_MAT3_OUT                                                                \
    0x00002000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x8), IO_MUX_CTIMER_MASK(0x8, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT1_MAT0_OUT                                                                \
    0x00004000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x10), IO_MUX_CTIMER_MASK(0x10, 0),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT1_MAT1_OUT                                                                \
    0x00200000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x20), IO_MUX_CTIMER_MASK(0x20, 0),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT1_MAT2_OUT                                                                \
    0x01000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x40000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x40), IO_MUX_CTIMER_MASK(0x40, 0),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT1_MAT3_OUT                                                                \
    0x02000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x0ED), /* GPIO&FC mask */ \
        0x00000000U, 0x00010000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x80), IO_MUX_CTIMER_MASK(0x80, 0),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT2_MAT0_OUT                                                            \
    0x00000000U, 0x00000020U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x100), IO_MUX_CTIMER_MASK(0x100, 0), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT2_MAT1_OUT                                                            \
    0x00000000U, 0x00000040U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x200), IO_MUX_CTIMER_MASK(0x200, 0), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT2_MAT2_OUT                                                            \
    0x00000000U, 0x00000080U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x400), IO_MUX_CTIMER_MASK(0x400, 0), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT2_MAT3_OUT                                                                \
    0x00000000U, 0x00080000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00040810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x800), IO_MUX_CTIMER_MASK(0x800, 0),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT3_MAT0_OUT                                                                \
    0x00000000U, 0x00100000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00080810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x1000), IO_MUX_CTIMER_MASK(0x1000, 0),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT3_MAT1_OUT                                                                \
    0x00000000U, 0x00200000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00100810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x2000), IO_MUX_CTIMER_MASK(0x2000, 0),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT3_MAT2_OUT                                                               \
    0x00000000U, 0x00400000U,                                          /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0xED), /* GPIO&FC mask */ \
        0x00000000U, 0x00200810U,                                      /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0x4000), IO_MUX_CTIMER_MASK(0x4000, 0),  /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)           /* SCTimer mask */

#define IO_MUX_CT_INP0                                                                     \
    0x00000001U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x1, 0), IO_MUX_CTIMER_MASK(0, 0x1),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP1                                                                 \
    0x00000002U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x2, 0), IO_MUX_CTIMER_MASK(0, 0x2),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT_INP2                                                                     \
    0x00001000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 1, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00000020U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x4, 0), IO_MUX_CTIMER_MASK(0, 0x4),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP3                                                                     \
    0x00002000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x8, 0), IO_MUX_CTIMER_MASK(0, 0x8),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP4                                                                     \
    0x00004000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x10, 0), IO_MUX_CTIMER_MASK(0, 0x10),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP5                                                                     \
    0x00200000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x20, 0), IO_MUX_CTIMER_MASK(0, 0x20),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP6                                                                     \
    0x01000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x40000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x40, 0), IO_MUX_CTIMER_MASK(0, 0x40),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP7                                                                     \
    0x02000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x0ED), /* GPIO&FC mask */ \
        0x00000000U, 0x00010000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x80, 0), IO_MUX_CTIMER_MASK(0, 0x80),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP8                                                                 \
    0x00000000U, 0x00000020U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x100, 0), IO_MUX_CTIMER_MASK(0, 0x100), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT_INP9                                                                 \
    0x00000000U, 0x00000040U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x200, 0), IO_MUX_CTIMER_MASK(0, 0x200), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT_INP10                                                                \
    0x00000000U, 0x00000080U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x400, 0), IO_MUX_CTIMER_MASK(0, 0x400), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_CT_INP11                                                                    \
    0x00000000U, 0x00080000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00040810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x800, 0), IO_MUX_CTIMER_MASK(0, 0x800),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP12                                                                    \
    0x00000000U, 0x00100000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00080810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x1000, 0), IO_MUX_CTIMER_MASK(0, 0x1000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP13                                                                    \
    0x00000000U, 0x00200000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00100810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x2000, 0), IO_MUX_CTIMER_MASK(0, 0x2000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_CT_INP14                                                                   \
    0x00000000U, 0x00400000U,                                          /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0xED), /* GPIO&FC mask */ \
        0x00000000U, 0x00200810U,                                      /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0x4000, 0), IO_MUX_CTIMER_MASK(0, 0x4000),  /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)           /* SCTimer mask */

#define IO_MUX_MCLK                                                                        \
    0x00000020U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x430), /* GPIO&FC mask */ \
        0x00000040U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_UTICK                                                                       \
    0x00078000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x4FD), /* GPIO&FC mask */ \
        0x00000200U, 0x0000C400U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_USIM                                                                        \
    0x00078000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x4FD), /* GPIO&FC mask */ \
        0x00000400U, 0x0000C200U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_LCD_8080                                                                    \
    0x00000000U, 0x037BFC00U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x7FF), /* GPIO&FC mask */ \
        0x00000800U, 0x083C1010U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x7800, 0x7800),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_LCD_SPI                                                                 \
    0x00000000U, 0x0003F000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00001000U, 0x00000800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_FREQ_GPIO_CLK                                                           \
    0x00000000U, 0x00040000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00002000U, 0x00020000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO_INT_BMATCH                                                             \
    0x00040000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 2, 0x080), /* GPIO&FC mask */ \
        0x00004000U, 0x00008600U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GAU_TRIGGER0                                                            \
    0x00000000U, 0x00040000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x00020000U, 0x00002000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_ACOMP0_GPIO_OUT                                                             \
    0x00000000U, 0x00080000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x040), /* GPIO&FC mask */ \
        0x00040000U, 0x00000810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x800, 0x800),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ACOMP0_EDGE_PULSE                                                           \
    0x00000000U, 0x00100000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x080), /* GPIO&FC mask */ \
        0x00080000U, 0x00000810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1000, 0x1000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ACOMP1_GPIO_OUT                                                             \
    0x00000000U, 0x00200000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x418), /* GPIO&FC mask */ \
        0x00100000U, 0x00000810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x2000, 0x2000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ACOMP1_EDGE_PULSE                                                           \
    0x00000000U, 0x00400000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x0ED), /* GPIO&FC mask */ \
        0x00200000U, 0x00000810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4000, 0x4000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GAU_TRIGGER1                                                                \
    0x00000000U, 0x00800000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x430), /* GPIO&FC mask */ \
        0x00400000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0x200)        /* SCTimer mask */

#define IO_MUX_SDIO                                                                      \
    0x001F9000U, 0x00000000U,                                         /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0xF, 0), /* GPIO&FC mask */ \
        0x00008000U, 0x00004620U,                                     /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4, 0x4),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)          /* SCTimer mask */

#define IO_MUX_ENET_CLK                                                                    \
    0x02000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x0ED), /* GPIO&FC mask */ \
        0x00010000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x80, 0x80),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ENET_RX                                                                     \
    0x00C00000U, 0xC0000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x0C0), /* GPIO&FC mask */ \
        0x04000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0xC, 0)          /* SCTimer mask */

#define IO_MUX_ENET_TX                                                                 \
    0x00000000U, 0x1C000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x02000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_ENET_MDIO                                                                   \
    0x00000000U, 0x03000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 6, 0x3EE), /* GPIO&FC mask */ \
        0x08000000U, 0x00000800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ENET_TIMER0                                                             \
    0x08000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x10000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x20, 0x20)  /* SCTimer mask */

#define IO_MUX_ENET_TIMER1                                                             \
    0x00000000U, 0x20000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(3, 0, 0), /* GPIO&FC mask */ \
        0x20000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_ENET_TIMER2                                                                 \
    0x01000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x3EE), /* GPIO&FC mask */ \
        0x40000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x40, 0x40),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_ENET_TIMER3                                                                 \
    0x04000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x2EE), /* GPIO&FC mask */ \
        0x80000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)      /* SCTimer mask */

#define IO_MUX_CLKIN_FRM_PD                                                                \
    0x00000010U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0x02D), /* GPIO&FC mask */ \
        0x00800000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x2, 0x2)        /* SCTimer mask */

#define IO_MUX_GPIO0                                                                       \
    0x00000001U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1, 0x1),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO1                                                                   \
    0x00000002U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x2, 0x2),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO2                                                                       \
    0x00000004U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0x32E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO3                                                                       \
    0x00000008U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0x22E), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x1, 0x1)        /* SCTimer mask */

#define IO_MUX_GPIO4                                                                       \
    0x00000010U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0x02D), /* GPIO&FC mask */ \
        0x00000000U, 0x00800000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x2, 0x2)        /* SCTimer mask */

#define IO_MUX_GPIO5                                                                       \
    0x00000020U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO6                                                                       \
    0x00000040U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x01000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO7                                                                       \
    0x00000080U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x0ED), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO8                                                                       \
    0x00000100U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO9                                                                       \
    0x00000200U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO10                                                                      \
    0x00000400U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO11                                                                      \
    0x00000800U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0x100)        /* SCTimer mask */

#define IO_MUX_GPIO12                                                                      \
    0x00001000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 1, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00008020U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4, 0x4),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO13                                                                      \
    0x00002000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x8, 0x8),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO14                                                                      \
    0x00004000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x10, 0x10),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO15                                                                      \
    0x00008000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x0ED), /* GPIO&FC mask */ \
        0x00000000U, 0x00008600U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO16                                                                      \
    0x00010000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00008600U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO17                                                                      \
    0x00020000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00008600U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO18                                                                      \
    0x00040000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x0000C600U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO19                                                                      \
    0x00080000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00008000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO20                                                                      \
    0x00100000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00008000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO21                                                                      \
    0x00200000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 2, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x20, 0x20),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO22                                                                      \
    0x00400000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x4, 0)          /* SCTimer mask */

#define IO_MUX_GPIO23                                                                      \
    0x00800000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x8, 0)          /* SCTimer mask */

#define IO_MUX_GPIO24                                                                      \
    0x01000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x40000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x40, 0x40),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO25                                                                      \
    0x02000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x0ED), /* GPIO&FC mask */ \
        0x00000000U, 0x00010000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x80, 0x80),       /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO26                                                                      \
    0x04000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 3, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x80000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)      /* SCTimer mask */

#define IO_MUX_GPIO27                                                                  \
    0x08000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x10000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x20, 0x20)  /* SCTimer mask */

#define IO_MUX_GPIO28                                                                  \
    0x10000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO29                                                                  \
    0x20000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO30                                                                  \
    0x40000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO31                                                                  \
    0x80000000U, 0x00000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(0, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO32                                                                  \
    0x00000000U, 0x00000001U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO33                                                                  \
    0x00000000U, 0x00000002U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO34                                                                  \
    0x00000000U, 0x00000004U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO35                                                                  \
    0x00000000U, 0x00000008U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x40, 0x40)  /* SCTimer mask */

#define IO_MUX_GPIO36                                                                  \
    0x00000000U, 0x00000010U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x80, 0x80)  /* SCTimer mask */

#define IO_MUX_GPIO37                                                                  \
    0x00000000U, 0x00000020U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x100, 0x100), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO38                                                                  \
    0x00000000U, 0x00000040U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x200, 0x200), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO39                                                                  \
    0x00000000U, 0x00000080U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x400, 0x400), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO40                                                                  \
    0x00000000U, 0x00000100U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO41                                                                  \
    0x00000000U, 0x00000200U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO42                                                                  \
    0x00000000U, 0x00000400U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO43                                                                  \
    0x00000000U, 0x00000800U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO44                                                                  \
    0x00000000U, 0x00001000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO45                                                                  \
    0x00000000U, 0x00002000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO46                                                                  \
    0x00000000U, 0x00004000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO47                                                                  \
    0x00000000U, 0x00008000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO48                                                                  \
    0x00000000U, 0x00010000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO49                                                                  \
    0x00000000U, 0x00020000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO50                                                                  \
    0x00000000U, 0x00040000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00022000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO51                                                                      \
    0x00000000U, 0x00080000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00040810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x800, 0x800),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO52                                                                      \
    0x00000000U, 0x00100000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00080810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1000, 0x1000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO53                                                                      \
    0x00000000U, 0x00200000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00100810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x2000, 0x2000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO54                                                                     \
    0x00000000U, 0x00400000U,                                          /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0xED), /* GPIO&FC mask */ \
        0x00000000U, 0x00200810U,                                      /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4000, 0x4000),  /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)           /* SCTimer mask */

#define IO_MUX_GPIO55                                                                      \
    0x00000000U, 0x00800000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00400000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0x200)        /* SCTimer mask */

#define IO_MUX_GPIO56                                                                      \
    0x00000000U, 0x01000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO57                                                                      \
    0x00000000U, 0x02000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 6, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_GPIO58                                                                  \
    0x00000000U, 0x04000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO59                                                                  \
    0x00000000U, 0x08000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO60                                                                  \
    0x00000000U, 0x10000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO61                                                                  \
    0x00000000U, 0x20000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x20000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO62                                                                  \
    0x00000000U, 0x40000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_GPIO63                                                                  \
    0x00000000U, 0x80000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(1, 0, 0), IO_MUX_GPIO_FC_MASK(2, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO0                                                                  \
    0x00000000U, 0x00000001U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO1                                                                  \
    0x00000000U, 0x00000002U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO2                                                                  \
    0x00000000U, 0x00000004U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000002U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO3                                                                  \
    0x00000000U, 0x00000008U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x40, 0x40)  /* SCTimer mask */

#define IO_MUX_SGPIO4                                                                  \
    0x00000000U, 0x00000010U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x80, 0x80)  /* SCTimer mask */

#define IO_MUX_SGPIO5                                                                  \
    0x00000000U, 0x00000020U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x100, 0x100), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO6                                                                  \
    0x00000000U, 0x00000040U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x200, 0x200), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO7                                                                  \
    0x00000000U, 0x00000080U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x400, 0x400), /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO8                                                                  \
    0x00000000U, 0x00000100U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO9                                                                  \
    0x00000000U, 0x00000200U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000008U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO10                                                                 \
    0x00000000U, 0x00000400U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO11                                                                 \
    0x00000000U, 0x00000800U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00000800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO12                                                                 \
    0x00000000U, 0x00001000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO13                                                                 \
    0x00000000U, 0x00002000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO14                                                                 \
    0x00000000U, 0x00004000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO15                                                                 \
    0x00000000U, 0x00008000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO16                                                                 \
    0x00000000U, 0x00010000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO17                                                                 \
    0x00000000U, 0x00020000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00001800U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO18                                                                 \
    0x00000000U, 0x00040000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x00022000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO19                                                                     \
    0x00000000U, 0x00080000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x040), /* GPIO&FC mask */ \
        0x00000000U, 0x00040810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x800, 0x800),     /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SGPIO20                                                                     \
    0x00000000U, 0x00100000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x080), /* GPIO&FC mask */ \
        0x00000000U, 0x00080810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x1000, 0x1000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SGPIO21                                                                     \
    0x00000000U, 0x00200000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x418), /* GPIO&FC mask */ \
        0x00000000U, 0x00100810U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x2000, 0x2000),   /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SGPIO22                                                                    \
    0x00000000U, 0x00400000U,                                          /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0xED), /* GPIO&FC mask */ \
        0x00000000U, 0x00200810U,                                      /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0x4000, 0x4000),  /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)           /* SCTimer mask */

#define IO_MUX_SGPIO23                                                                     \
    0x00000000U, 0x00800000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x430), /* GPIO&FC mask */ \
        0x00000000U, 0x00400000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0x200)        /* SCTimer mask */

#define IO_MUX_SGPIO24                                                                     \
    0x00000000U, 0x01000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SGPIO25                                                                     \
    0x00000000U, 0x02000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 6, 0x3EE), /* GPIO&FC mask */ \
        0x00000000U, 0x08000800U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)            /* SCTimer mask */

#define IO_MUX_SGPIO26                                                                 \
    0x00000000U, 0x04000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO27                                                                 \
    0x00000000U, 0x08000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO28                                                                 \
    0x00000000U, 0x10000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x02000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO29                                                                 \
    0x00000000U, 0x20000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x20000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO30                                                                 \
    0x00000000U, 0x40000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_SGPIO31                                                                 \
    0x00000000U, 0x80000000U,                                       /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(2, 0, 0), IO_MUX_GPIO_FC_MASK(1, 0, 0), /* GPIO&FC mask */ \
        0x00000000U, 0x04000000U,                                   /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),         /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0, 0)        /* SCTimer mask */

#define IO_MUX_AON_CAPTURE                                                                 \
    0x04000000U, 0x00000000U,                                           /* Pin mask */     \
        IO_MUX_GPIO_FC_MASK(0, 0, 0), IO_MUX_GPIO_FC_MASK(1, 3, 0x2EE), /* GPIO&FC mask */ \
        0x00000000U, 0x80000000U,                                       /* FSEL mask */    \
        IO_MUX_CTIMER_MASK(0, 0), IO_MUX_CTIMER_MASK(0, 0),             /* CTimer mask */  \
        IO_MUX_SCTIMER_MASK(0, 0), IO_MUX_SCTIMER_MASK(0x10, 0x10)      /* SCTimer mask */

/*! @brief IO MUX pin configuration.
      Bit [1:0] for pull configuration
      Bit [3:2] for drive strength configuration
 */
typedef enum
{
    IO_MUX_PinConfigNoPullDriveWeakest   = 0x0U,
    IO_MUX_PinConfigNoPullDriveWeak      = 0x4U,
    IO_MUX_PinConfigNoPullDriveStrong    = 0x8U,
    IO_MUX_PinConfigNoPullDriveStrongest = 0xCU,

    IO_MUX_PinConfigPullUpDriveWeakest   = 0x1U,
    IO_MUX_PinConfigPullUpDriveWeak      = 0x5U,
    IO_MUX_PinConfigPullUpDriveStrong    = 0x9U,
    IO_MUX_PinConfigPullUpDriveStrongest = 0xDU,

    IO_MUX_PinConfigPullDownDriveWeakest   = 0x2U,
    IO_MUX_PinConfigPullDownDriveWeak      = 0x6U,
    IO_MUX_PinConfigPullDownDriveStrong    = 0xAU,
    IO_MUX_PinConfigPullDownDriveStrongest = 0xEU,

    IO_MUX_PinConfigNoPull   = IO_MUX_PinConfigNoPullDriveStrongest,
    IO_MUX_PinConfigPullUp   = IO_MUX_PinConfigPullUpDriveStrongest,
    IO_MUX_PinConfigPullDown = IO_MUX_PinConfigPullDownDriveStrongest,
} io_mux_pin_config_t;

/*! @brief IO MUX sleep pin level */
typedef enum
{
    IO_MUX_SleepPinLevelLow       = 0U,
    IO_MUX_SleepPinLevelHigh      = 1U,
    IO_MUX_SleepPinLevelUnchanged = 2U,
} io_mux_sleep_pin_level_t;

/*@}*/
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /*__cplusplus */

/*! @name Configuration */
/*@{*/

/*!
 * @brief Sets the IO_MUX pin mux mode.
 * @note The parameters can be filled with the pin function ID macros.
 *
 * This is an example to set the GPIO2/GPIO3 as the Flexcomm0 UART RX/TX:
 * @code
 * IO_MUX_SetPinMux(IO_MUX_FC0_USART_DATA);
 * @endcode
 *
 * This is an example to set the GPIO6/GPIO10 as Flexcomm1 I2C SDA/SCL:
 * @code
 * IO_MUX_SetPinMux(IO_MUX_FC1_I2C_6_10);
 * @endcode
 *
 * @param pinLowMask     The GPIO0-31 pins mask.
 * @param pinHighMask    The GPIO32-63 pins mask.
 * @param gpioFcSetMask  The GPIO and Flexcomm registers mask to set, defined by IO_MUX_GPIO_FC_MASK()
 * @param gpioFcClrMask  The GPIO and Flexcomm registers mask to clear, defined by IO_MUX_GPIO_FC_MASK()
 * @param fselSetMask    The FSEL register mask to set
 * @param fselClrMask    The FSEL register mask to clear
 * @param ctimerSetMask  The C_TIMER_IN/C_TIMER_OUT register mask to set, defined by IO_MUX_CTIMER_MASK()
 * @param ctimerClrMask  The C_TIMER_IN/C_TIMER_OUT register mask to clear, defined by IO_MUX_CTIMER_MASK()
 * @param sctimerSetMask The SC_TIMER register mask to set
 * @param sctimerClrMask The SC_TIMER register mask to clear
 */
static inline void IO_MUX_SetPinMux(uint32_t pinLowMask,
                                    uint32_t pinHighMask,
                                    uint32_t gpioFcSetMask,
                                    uint32_t gpioFcClrMask,
                                    uint32_t fselSetMask,
                                    uint32_t fselClrMask,
                                    uint32_t ctimerSetMask,
                                    uint32_t ctimerClrMask,
                                    uint32_t sctimerSetMask,
                                    uint32_t sctimerClrMask)
{
    volatile uint32_t *fcBase = (volatile uint32_t *)&MCI_IO_MUX->FC0;
    uint32_t aonMask;

    /* Fist clear all conflicted io_mux function. */
    if (IO_MUX_SGPIO_FLAG(gpioFcClrMask) != 0U)
    {
        MCI_IO_MUX->S_GPIO &= ~pinHighMask;
    }
    if (IO_MUX_GPIO_FLAG(gpioFcClrMask) != 0U)
    {
        MCI_IO_MUX->GPIO_GRP0 &= ~pinLowMask;
        MCI_IO_MUX->GPIO_GRP1 &= ~pinHighMask;
    }

    if (IO_MUX_FC_OFFSET(gpioFcClrMask) == 0xFU) /* Special case for SDIO */
    {
        assert(fselSetMask == MCI_IO_MUX_FSEL_SEL_SDIO_MASK);
        MCI_IO_MUX->FC1 &= ~0x080UL;
        MCI_IO_MUX->FC2 &= ~0x4FDUL;
        MCI_IO_MUX->FC3 &= ~0x438UL;
    }
    else if (IO_MUX_FC_MASK(gpioFcClrMask) != 0U)
    {
        *(fcBase + IO_MUX_FC_OFFSET(gpioFcClrMask)) &= ~IO_MUX_FC_MASK(gpioFcClrMask);
    }
    else
    {
        /* FC register change not needed. */
    }

    if (fselClrMask != 0U)
    {
        MCI_IO_MUX->FSEL &= ~fselClrMask;
    }
    if (IO_MUX_CTIMER_IN_MASK(ctimerClrMask) != 0U)
    {
        MCI_IO_MUX->C_TIMER_IN &= ~IO_MUX_CTIMER_IN_MASK(ctimerClrMask);
    }
    if (IO_MUX_CTIMER_OUT_MASK(ctimerClrMask) != 0U)
    {
        MCI_IO_MUX->C_TIMER_OUT &= ~IO_MUX_CTIMER_OUT_MASK(ctimerClrMask);
    }
    if (sctimerClrMask != 0U)
    {
        MCI_IO_MUX->SC_TIMER &= ~sctimerClrMask;
    }

    /* Now set the IO_MUX for the function. */
    if (IO_MUX_SGPIO_FLAG(gpioFcSetMask) != 0U)
    {
        MCI_IO_MUX->S_GPIO |= pinHighMask;
    }
    if (IO_MUX_GPIO_FLAG(gpioFcSetMask) != 0U)
    {
        MCI_IO_MUX->GPIO_GRP0 |= pinLowMask;
        MCI_IO_MUX->GPIO_GRP1 |= pinHighMask;
    }

    if (IO_MUX_FC_MASK(gpioFcSetMask) != 0U)
    {
        *(fcBase + IO_MUX_FC_OFFSET(gpioFcSetMask)) |= IO_MUX_FC_MASK(gpioFcSetMask);
    }

    if (fselSetMask != 0U)
    {
        MCI_IO_MUX->FSEL |= fselSetMask;
    }
    if (IO_MUX_CTIMER_IN_MASK(ctimerSetMask) != 0U)
    {
        MCI_IO_MUX->C_TIMER_IN |= IO_MUX_CTIMER_IN_MASK(ctimerSetMask);
    }
    if (IO_MUX_CTIMER_OUT_MASK(ctimerSetMask) != 0U)
    {
        MCI_IO_MUX->C_TIMER_OUT |= IO_MUX_CTIMER_OUT_MASK(ctimerSetMask);
    }
    if (sctimerSetMask != 0U)
    {
        MCI_IO_MUX->SC_TIMER |= sctimerSetMask;
    }

    /* Last, enable IO function */
    aonMask = pinLowMask & AON_SOC_CIU_MCI_IOMUX_EN0_EN_27_22_MASK;
    SOCCTRL->MCI_IOMUX_EN0 |= (pinLowMask & ~aonMask);
    SOCCTRL->MCI_IOMUX_EN1 |= pinHighMask;
    AON_SOC_CIU->MCI_IOMUX_EN0 |= aonMask;
}

/*!
 * @brief Sets the IO_MUX pin mux pull up/down configuartion.
 *
 * This is an example to set the GPIO2 pin to pull down:
 * @code
 * IO_MUX_SetPinConfig(2U, IO_MUX_PinConfigPullDown);
 * @endcode
 *
 * @param pin The GPIO pin index to config.
 * @param config The pull up/down setting for the pin.
 */
static inline void IO_MUX_SetPinConfig(uint32_t pin, io_mux_pin_config_t config)
{
    __IO uint32_t *pullReg, *driveReg;
    uint32_t shift;
    uint32_t pullVal, driveVal;

    assert(pin < 64U);

    shift    = (pin % 16U) * 2U;
    pullVal  = (uint32_t)config & 0x3U;
    driveVal = ((uint32_t)config >> 2) & 0x3U;
    if (pin < 22U || pin > 27U)
    {
        pullReg  = &SOCCTRL->PAD_PU_PD_EN0 + pin / 16U;
        driveReg = &SOCCTRL->SR_CONFIG0 + pin / 16U;
    }
    else
    {
        pullReg  = &AON_SOC_CIU->PAD_PU_PD_EN1;
        driveReg = &AON_SOC_CIU->SR_CONFIG1;
    }

    *pullReg  = (*pullReg & ~(3UL << shift)) | (pullVal << shift);
    *driveReg = (*driveReg & ~(3UL << shift)) | (driveVal << shift);
}

/*!
 * @brief Sets IO output level in sleep mode. If level set to IO_MUX_SleepPinLevelUnchanged,
 *        the IO configuration is same as the active mode.
 *
 * This is an example to set the GPIO2 pin to output high during sleep:
 * @code
 * IO_MUX_SetPinOutLevelInSleep(2U, IO_MUX_SleepPinLevelHigh);
 * @endcode
 *
 * @param pin The GPIO pin index to config.
 * @param level Output level in sleep.
 */
static inline void IO_MUX_SetPinOutLevelInSleep(uint32_t pin, io_mux_sleep_pin_level_t level)
{
    __IO uint32_t *regEn, *regVal;
    uint32_t mask, shift;

    assert(pin < 64U);

    shift = pin % 32U;
    mask  = 1UL << shift;

    if (pin < 22U || pin > 27U)
    {
        if (pin < 32U)
        {
            regEn  = &SOCCTRL->PAD_SLP_EN0;
            regVal = &SOCCTRL->PAD_SLP_VAL0;
        }
        else
        {
            regEn  = &SOCCTRL->PAD_SLP_EN1;
            regVal = &SOCCTRL->PAD_SLP_VAL1;
        }
    }
    else
    {
        regEn  = &AON_SOC_CIU->PAD_SLP_EN0;
        regVal = &AON_SOC_CIU->PAD_SLP_VAL0;
    }

    if (level == IO_MUX_SleepPinLevelUnchanged)
    {
        *regEn &= ~mask;
    }
    else
    {
        *regEn |= mask;
        *regVal = ((*regVal) & ~mask) | (((uint32_t)level) << shift);
    }
}

/*!
 * @brief Sets RF Switch Pin 0-3 output level in sleep mode. If level set to
 *        IO_MUX_SleepPinLevelUnchanged, the IO configuration is same as the active mode.
 *
 * This is an example to set the RF_CNTL0 pin to output low during sleep:
 * @code
 * IO_MUX_SetRfPinOutLevelInSleep(0U, IO_MUX_SleepPinLevelLow);
 * @endcode
 *
 * @param pin The RF Switch pin index to config.
 * @param level Output level in sleep.
 */
static inline void IO_MUX_SetRfPinOutLevelInSleep(uint32_t pin, io_mux_sleep_pin_level_t level)
{
    uint32_t mask;

    assert(pin < 4U);

    mask = 0x10001UL << pin;

    if (level == IO_MUX_SleepPinLevelUnchanged)
    {
        SOCCTRL->PAD_RF_SW_SLP_CONFIG &= ~mask;
    }
    else
    {
        SOCCTRL->PAD_RF_SW_SLP_CONFIG =
            (SOCCTRL->PAD_RF_SW_SLP_CONFIG & ~mask) | (1UL << pin) | (((uint32_t)level) << (pin + 16U));
    }
}

/*@}*/

#if defined(__cplusplus)
}
#endif /*__cplusplus */

/*! @}*/

#endif /* _FSL_IO_MUX_H_ */
