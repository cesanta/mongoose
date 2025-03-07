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
**     Build:               b241017
**
**     Abstract:
**         CMSIS Peripheral Access Layer for MCI_IO_MUX
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
 * @file MCI_IO_MUX.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for MCI_IO_MUX
 *
 * CMSIS Peripheral Access Layer for MCI_IO_MUX
 */

#if !defined(MCI_IO_MUX_H_)
#define MCI_IO_MUX_H_                            /**< Symbol preventing repeated inclusion */

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
   -- MCI_IO_MUX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCI_IO_MUX_Peripheral_Access_Layer MCI_IO_MUX Peripheral Access Layer
 * @{
 */

/** MCI_IO_MUX - Register Layout Typedef */
typedef struct {
  __IO uint32_t S_GPIO;                            /**< security GPIO sel, offset: 0x0 */
  __IO uint32_t FC0;                               /**< flexcomm0 function sel, offset: 0x4 */
  __IO uint32_t FC1;                               /**< flexcomm1 function sel, offset: 0x8 */
  __IO uint32_t FC2;                               /**< flexcomm2 function sel, offset: 0xC */
  __IO uint32_t FC3;                               /**< flexcomm3 function sel, offset: 0x10 */
       uint8_t RESERVED_0[8];
  __IO uint32_t FC14;                              /**< flexcomm14 function sel, offset: 0x1C */
  __IO uint32_t FSEL;                              /**< function sel, offset: 0x20 */
  __IO uint32_t C_TIMER_IN;                        /**< ctimer input function sel, offset: 0x24 */
  __IO uint32_t C_TIMER_OUT;                       /**< ctimer output function sel, offset: 0x28 */
  __IO uint32_t SC_TIMER;                          /**< sctimer function sel, offset: 0x2C */
  __IO uint32_t GPIO_GRP0;                         /**< GPIO[31:0] sel, offset: 0x30 */
  __IO uint32_t GPIO_GRP1;                         /**< GPIO[63:32] sel, offset: 0x34 */
} MCI_IO_MUX_Type;

/* ----------------------------------------------------------------------------
   -- MCI_IO_MUX Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCI_IO_MUX_Register_Masks MCI_IO_MUX Register Masks
 * @{
 */

/*! @name S_GPIO - security GPIO sel */
/*! @{ */

#define MCI_IO_MUX_S_GPIO_SEL_MASK               (0xFFFFFFFFU)
#define MCI_IO_MUX_S_GPIO_SEL_SHIFT              (0U)
/*! SEL - spio0[31:0] selection, high valid; sel[i]->spio0[i]->GPIO[i+32] */
#define MCI_IO_MUX_S_GPIO_SEL(x)                 (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_S_GPIO_SEL_SHIFT)) & MCI_IO_MUX_S_GPIO_SEL_MASK)
/*! @} */

/*! @name FC0 - flexcomm0 function sel */
/*! @{ */

#define MCI_IO_MUX_FC0_SEL_FC0_USART_SCK_MASK    (0x1U)
#define MCI_IO_MUX_FC0_SEL_FC0_USART_SCK_SHIFT   (0U)
/*! SEL_FC0_USART_SCK - flexcomm0:select GPIO-4 as usart sck */
#define MCI_IO_MUX_FC0_SEL_FC0_USART_SCK(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_USART_SCK_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_USART_SCK_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_I2C_MASK          (0x2U)
#define MCI_IO_MUX_FC0_SEL_FC0_I2C_SHIFT         (1U)
/*! SEL_FC0_I2C - flexcomm0:select GPIO-2/3 as i2c function */
#define MCI_IO_MUX_FC0_SEL_FC0_I2C(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_I2C_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_I2C_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_I2S_MASK          (0x4U)
#define MCI_IO_MUX_FC0_SEL_FC0_I2S_SHIFT         (2U)
/*! SEL_FC0_I2S - flexcomm0:select GPIO-2/3/4 as i2s function */
#define MCI_IO_MUX_FC0_SEL_FC0_I2S(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_I2S_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_I2S_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_SPI_MASK          (0x8U)
#define MCI_IO_MUX_FC0_SEL_FC0_SPI_SHIFT         (3U)
/*! SEL_FC0_SPI - flexcomm0:select GPIO-0/2/3/4 as spi function */
#define MCI_IO_MUX_FC0_SEL_FC0_SPI(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_SPI_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_SPI_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_I2S_DATA_ONLY_MASK (0x100U)
#define MCI_IO_MUX_FC0_SEL_FC0_I2S_DATA_ONLY_SHIFT (8U)
/*! SEL_FC0_I2S_DATA_ONLY - flexcomm0:select GPIO-2 as i2s data */
#define MCI_IO_MUX_FC0_SEL_FC0_I2S_DATA_ONLY(x)  (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_I2S_DATA_ONLY_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_I2S_DATA_ONLY_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_USART_DATA_MASK   (0x200U)
#define MCI_IO_MUX_FC0_SEL_FC0_USART_DATA_SHIFT  (9U)
/*! SEL_FC0_USART_DATA - flexcomm0:select GPIO-2/3 as usart rxd/txd */
#define MCI_IO_MUX_FC0_SEL_FC0_USART_DATA(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_USART_DATA_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_USART_DATA_MASK)

#define MCI_IO_MUX_FC0_SEL_FC0_USART_CMD_MASK    (0x400U)
#define MCI_IO_MUX_FC0_SEL_FC0_USART_CMD_SHIFT   (10U)
/*! SEL_FC0_USART_CMD - flexcomm0:select GPIO-0/5 as usart cts/rts */
#define MCI_IO_MUX_FC0_SEL_FC0_USART_CMD(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC0_SEL_FC0_USART_CMD_SHIFT)) & MCI_IO_MUX_FC0_SEL_FC0_USART_CMD_MASK)
/*! @} */

/*! @name FC1 - flexcomm1 function sel */
/*! @{ */

#define MCI_IO_MUX_FC1_SEL_FC1_USART_SCK_MASK    (0x1U)
#define MCI_IO_MUX_FC1_SEL_FC1_USART_SCK_SHIFT   (0U)
/*! SEL_FC1_USART_SCK - flexcomm1:select GPIO-7 as usart sck */
#define MCI_IO_MUX_FC1_SEL_FC1_USART_SCK(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_USART_SCK_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_USART_SCK_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_I2C_MASK          (0x2U)
#define MCI_IO_MUX_FC1_SEL_FC1_I2C_SHIFT         (1U)
/*! SEL_FC1_I2C - flexcomm1:select GPIO-8/9 as i2c function */
#define MCI_IO_MUX_FC1_SEL_FC1_I2C(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_I2C_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_I2C_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_I2S_MASK          (0x4U)
#define MCI_IO_MUX_FC1_SEL_FC1_I2S_SHIFT         (2U)
/*! SEL_FC1_I2S - flexcomm1:select GPIO-7/8/9 as i2s function */
#define MCI_IO_MUX_FC1_SEL_FC1_I2S(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_I2S_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_I2S_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_SPI_MASK          (0x8U)
#define MCI_IO_MUX_FC1_SEL_FC1_SPI_SHIFT         (3U)
/*! SEL_FC1_SPI - flexcomm1:select GPIO-6/7/8/9 as spi function */
#define MCI_IO_MUX_FC1_SEL_FC1_SPI(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_SPI_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_SPI_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_I2S_DATA_ONLY_MASK (0x100U)
#define MCI_IO_MUX_FC1_SEL_FC1_I2S_DATA_ONLY_SHIFT (8U)
/*! SEL_FC1_I2S_DATA_ONLY - flexcomm1:select GPIO-9 as i2s data */
#define MCI_IO_MUX_FC1_SEL_FC1_I2S_DATA_ONLY(x)  (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_I2S_DATA_ONLY_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_I2S_DATA_ONLY_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_USART_DATA_MASK   (0x200U)
#define MCI_IO_MUX_FC1_SEL_FC1_USART_DATA_SHIFT  (9U)
/*! SEL_FC1_USART_DATA - flexcomm1:select GPIO-8/9 as usart txd/rxd */
#define MCI_IO_MUX_FC1_SEL_FC1_USART_DATA(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_USART_DATA_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_USART_DATA_MASK)

#define MCI_IO_MUX_FC1_SEL_FC1_USART_CMD_MASK    (0x400U)
#define MCI_IO_MUX_FC1_SEL_FC1_USART_CMD_SHIFT   (10U)
/*! SEL_FC1_USART_CMD - flexcomm1:select GPIO-6/10 as usart cts/rts */
#define MCI_IO_MUX_FC1_SEL_FC1_USART_CMD(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC1_SEL_FC1_USART_CMD_SHIFT)) & MCI_IO_MUX_FC1_SEL_FC1_USART_CMD_MASK)
/*! @} */

/*! @name FC2 - flexcomm2 function sel */
/*! @{ */

#define MCI_IO_MUX_FC2_SEL_FC2_USART_SCK_MASK    (0x1U)
#define MCI_IO_MUX_FC2_SEL_FC2_USART_SCK_SHIFT   (0U)
/*! SEL_FC2_USART_SCK - flexcomm2:select GPIO-15 as usart sck */
#define MCI_IO_MUX_FC2_SEL_FC2_USART_SCK(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_USART_SCK_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_USART_SCK_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_I2C_MASK          (0x2U)
#define MCI_IO_MUX_FC2_SEL_FC2_I2C_SHIFT         (1U)
/*! SEL_FC2_I2C - flexcomm2:select GPIO-13/14 as i2c function */
#define MCI_IO_MUX_FC2_SEL_FC2_I2C(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_I2C_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_I2C_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_I2S_MASK          (0x4U)
#define MCI_IO_MUX_FC2_SEL_FC2_I2S_SHIFT         (2U)
/*! SEL_FC2_I2S - flexcomm2:select GPIO-13/14/15 as i2s function */
#define MCI_IO_MUX_FC2_SEL_FC2_I2S(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_I2S_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_I2S_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_SPI_MASK          (0x8U)
#define MCI_IO_MUX_FC2_SEL_FC2_SPI_SHIFT         (3U)
/*! SEL_FC2_SPI - flexcomm2:select GPIO-13/14/15/16 as spi function */
#define MCI_IO_MUX_FC2_SEL_FC2_SPI(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_SPI_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_SPI_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_I2C_COPY_PIN_MASK (0x10U)
#define MCI_IO_MUX_FC2_SEL_FC2_I2C_COPY_PIN_SHIFT (4U)
/*! SEL_FC2_I2C_COPY_PIN - flexcomm2:select GPIO-16/17 as backup-i2c function */
#define MCI_IO_MUX_FC2_SEL_FC2_I2C_COPY_PIN(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_I2C_COPY_PIN_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_I2C_COPY_PIN_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY_MASK (0x100U)
#define MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY_SHIFT (8U)
/*! SEL_FC2_I2S_DATA_ONLY - flexcomm2:select GPIO-13 as i2s data function */
#define MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY(x)  (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_I2S_DATA_ONLY_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_USART_DATA_MASK   (0x200U)
#define MCI_IO_MUX_FC2_SEL_FC2_USART_DATA_SHIFT  (9U)
/*! SEL_FC2_USART_DATA - flexcomm2:select GPIO-13/14 as usart rxd/txd */
#define MCI_IO_MUX_FC2_SEL_FC2_USART_DATA(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_USART_DATA_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_USART_DATA_MASK)

#define MCI_IO_MUX_FC2_SEL_FC2_USART_CMD_MASK    (0x400U)
#define MCI_IO_MUX_FC2_SEL_FC2_USART_CMD_SHIFT   (10U)
/*! SEL_FC2_USART_CMD - flexcomm2:select GPIO-16/17 as usart cts/rts */
#define MCI_IO_MUX_FC2_SEL_FC2_USART_CMD(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC2_SEL_FC2_USART_CMD_SHIFT)) & MCI_IO_MUX_FC2_SEL_FC2_USART_CMD_MASK)
/*! @} */

/*! @name FC3 - flexcomm3 function sel */
/*! @{ */

#define MCI_IO_MUX_FC3_SEL_FC3_USART_SCK_MASK    (0x1U)
#define MCI_IO_MUX_FC3_SEL_FC3_USART_SCK_SHIFT   (0U)
/*! SEL_FC3_USART_SCK - flexcomm3:select GPIO-25 as usart sck */
#define MCI_IO_MUX_FC3_SEL_FC3_USART_SCK(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_USART_SCK_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_USART_SCK_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_I2C_MASK          (0x2U)
#define MCI_IO_MUX_FC3_SEL_FC3_I2C_SHIFT         (1U)
/*! SEL_FC3_I2C - flexcomm3:select GPIO-24/26 as i2c function */
#define MCI_IO_MUX_FC3_SEL_FC3_I2C(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_I2C_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_I2C_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_I2S_MASK          (0x4U)
#define MCI_IO_MUX_FC3_SEL_FC3_I2S_SHIFT         (2U)
/*! SEL_FC3_I2S - flexcomm3:select GPIO-24/25/26 as i2s function */
#define MCI_IO_MUX_FC3_SEL_FC3_I2S(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_I2S_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_I2S_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_SPI_MASK          (0x8U)
#define MCI_IO_MUX_FC3_SEL_FC3_SPI_SHIFT         (3U)
/*! SEL_FC3_SPI - flexcomm3:select GPIO-20/24/25/26 as spi function */
#define MCI_IO_MUX_FC3_SEL_FC3_SPI(x)            (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_SPI_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_SPI_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_I2C_COPY_PIN_MASK (0x10U)
#define MCI_IO_MUX_FC3_SEL_FC3_I2C_COPY_PIN_SHIFT (4U)
/*! SEL_FC3_I2C_COPY_PIN - flexcomm3:select GPIO-19/20 as backup-i2c function */
#define MCI_IO_MUX_FC3_SEL_FC3_I2C_COPY_PIN(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_I2C_COPY_PIN_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_I2C_COPY_PIN_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_I2S_DATA_ONLY_MASK (0x100U)
#define MCI_IO_MUX_FC3_SEL_FC3_I2S_DATA_ONLY_SHIFT (8U)
/*! SEL_FC3_I2S_DATA_ONLY - flexcomm3:select GPIO-24 as i2s data */
#define MCI_IO_MUX_FC3_SEL_FC3_I2S_DATA_ONLY(x)  (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_I2S_DATA_ONLY_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_I2S_DATA_ONLY_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_USART_DATA_MASK   (0x200U)
#define MCI_IO_MUX_FC3_SEL_FC3_USART_DATA_SHIFT  (9U)
/*! SEL_FC3_USART_DATA - flexcomm3:select GPIO-24/26 as usart rxd/txd */
#define MCI_IO_MUX_FC3_SEL_FC3_USART_DATA(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_USART_DATA_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_USART_DATA_MASK)

#define MCI_IO_MUX_FC3_SEL_FC3_USART_CMD_MASK    (0x400U)
#define MCI_IO_MUX_FC3_SEL_FC3_USART_CMD_SHIFT   (10U)
/*! SEL_FC3_USART_CMD - flexcomm3:select GPIO-19/20 as usart rts/cts */
#define MCI_IO_MUX_FC3_SEL_FC3_USART_CMD(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC3_SEL_FC3_USART_CMD_SHIFT)) & MCI_IO_MUX_FC3_SEL_FC3_USART_CMD_MASK)
/*! @} */

/*! @name FC14 - flexcomm14 function sel */
/*! @{ */

#define MCI_IO_MUX_FC14_SEL_FC14_USART_SCK_MASK  (0x1U)
#define MCI_IO_MUX_FC14_SEL_FC14_USART_SCK_SHIFT (0U)
/*! SEL_FC14_USART_SCK - flexcomm14:select GPIO-54 as usart sck */
#define MCI_IO_MUX_FC14_SEL_FC14_USART_SCK(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_USART_SCK_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_USART_SCK_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_I2C_MASK        (0x2U)
#define MCI_IO_MUX_FC14_SEL_FC14_I2C_SHIFT       (1U)
/*! SEL_FC14_I2C - flexcomm14:select GPIO-56/57 as i2c function */
#define MCI_IO_MUX_FC14_SEL_FC14_I2C(x)          (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_I2C_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_I2C_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_I2S_MASK        (0x4U)
#define MCI_IO_MUX_FC14_SEL_FC14_I2S_SHIFT       (2U)
/*! SEL_FC14_I2S - flexcomm14:select GPIO-54/56/57 as i2s function */
#define MCI_IO_MUX_FC14_SEL_FC14_I2S(x)          (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_I2S_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_I2S_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_SPI_MASK        (0x8U)
#define MCI_IO_MUX_FC14_SEL_FC14_SPI_SHIFT       (3U)
/*! SEL_FC14_SPI - flexcomm14:select GPIO-53/54/56/57 as spi function */
#define MCI_IO_MUX_FC14_SEL_FC14_SPI(x)          (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_SPI_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_SPI_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_I2S_DATA_ONLY_MASK (0x100U)
#define MCI_IO_MUX_FC14_SEL_FC14_I2S_DATA_ONLY_SHIFT (8U)
/*! SEL_FC14_I2S_DATA_ONLY - flexcomm14:select GPIO-57 as i2s data */
#define MCI_IO_MUX_FC14_SEL_FC14_I2S_DATA_ONLY(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_I2S_DATA_ONLY_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_I2S_DATA_ONLY_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_USART_DATA_MASK (0x200U)
#define MCI_IO_MUX_FC14_SEL_FC14_USART_DATA_SHIFT (9U)
/*! SEL_FC14_USART_DATA - flexcomm14:select GPIO-56/57 as usart txd/rxd */
#define MCI_IO_MUX_FC14_SEL_FC14_USART_DATA(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_USART_DATA_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_USART_DATA_MASK)

#define MCI_IO_MUX_FC14_SEL_FC14_USART_CMD_MASK  (0x400U)
#define MCI_IO_MUX_FC14_SEL_FC14_USART_CMD_SHIFT (10U)
/*! SEL_FC14_USART_CMD - flexcomm14:select GPIO-53/55 as usart cts/rts */
#define MCI_IO_MUX_FC14_SEL_FC14_USART_CMD(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FC14_SEL_FC14_USART_CMD_SHIFT)) & MCI_IO_MUX_FC14_SEL_FC14_USART_CMD_MASK)
/*! @} */

/*! @name FSEL - function sel */
/*! @{ */

#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_FLASH_MASK  (0x2U)
#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_FLASH_SHIFT (1U)
/*! SEL_QUAD_SPI_FLASH - select GPIO-28/29/30/31/32/33/34 as quad_spi_flash function */
#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_FLASH(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_QUAD_SPI_FLASH_SHIFT)) & MCI_IO_MUX_FSEL_SEL_QUAD_SPI_FLASH_MASK)

#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_PSRAM_MASK  (0x8U)
#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_PSRAM_SHIFT (3U)
/*! SEL_QUAD_SPI_PSRAM - select GPIO-35/36/37/38/39/40/41 as quad_spi_psram function */
#define MCI_IO_MUX_FSEL_SEL_QUAD_SPI_PSRAM(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_QUAD_SPI_PSRAM_SHIFT)) & MCI_IO_MUX_FSEL_SEL_QUAD_SPI_PSRAM_MASK)

#define MCI_IO_MUX_FSEL_SEL_PDM_MASK             (0x10U)
#define MCI_IO_MUX_FSEL_SEL_PDM_SHIFT            (4U)
/*! SEL_PDM - select GPIO-51/52/53/54 as pdm function */
#define MCI_IO_MUX_FSEL_SEL_PDM(x)               (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_PDM_SHIFT)) & MCI_IO_MUX_FSEL_SEL_PDM_MASK)

#define MCI_IO_MUX_FSEL_SEL_USB_MASK             (0x20U)
#define MCI_IO_MUX_FSEL_SEL_USB_SHIFT            (5U)
/*! SEL_USB - select GPIO-12 as usb utmi_drvvbus */
#define MCI_IO_MUX_FSEL_SEL_USB(x)               (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_USB_SHIFT)) & MCI_IO_MUX_FSEL_SEL_USB_MASK)

#define MCI_IO_MUX_FSEL_SEL_MCLK_MASK            (0x40U)
#define MCI_IO_MUX_FSEL_SEL_MCLK_SHIFT           (6U)
/*! SEL_MCLK - select GPIO-5 as mclk function */
#define MCI_IO_MUX_FSEL_SEL_MCLK(x)              (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_MCLK_SHIFT)) & MCI_IO_MUX_FSEL_SEL_MCLK_MASK)

#define MCI_IO_MUX_FSEL_SEL_UTICK_MASK           (0x200U)
#define MCI_IO_MUX_FSEL_SEL_UTICK_SHIFT          (9U)
/*! SEL_UTICK - select GPIO-15/16/17/18 as utick function */
#define MCI_IO_MUX_FSEL_SEL_UTICK(x)             (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_UTICK_SHIFT)) & MCI_IO_MUX_FSEL_SEL_UTICK_MASK)

#define MCI_IO_MUX_FSEL_SEL_USIM_MASK            (0x400U)
#define MCI_IO_MUX_FSEL_SEL_USIM_SHIFT           (10U)
/*! SEL_USIM - select GPIO-15/16/17/18 as usim function */
#define MCI_IO_MUX_FSEL_SEL_USIM(x)              (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_USIM_SHIFT)) & MCI_IO_MUX_FSEL_SEL_USIM_MASK)

#define MCI_IO_MUX_FSEL_SEL_LCD_8080_MASK        (0x800U)
#define MCI_IO_MUX_FSEL_SEL_LCD_8080_SHIFT       (11U)
/*! SEL_LCD_8080 - select GPIO-42/43/44/45/46/47/48/49/51/52/53/54/56/57 as lcd_8080 function */
#define MCI_IO_MUX_FSEL_SEL_LCD_8080(x)          (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_LCD_8080_SHIFT)) & MCI_IO_MUX_FSEL_SEL_LCD_8080_MASK)

#define MCI_IO_MUX_FSEL_SEL_LCD_SPI_MASK         (0x1000U)
#define MCI_IO_MUX_FSEL_SEL_LCD_SPI_SHIFT        (12U)
/*! SEL_LCD_SPI - select GPIO-44/45/46/47/48/49 as lcd_spi function */
#define MCI_IO_MUX_FSEL_SEL_LCD_SPI(x)           (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_LCD_SPI_SHIFT)) & MCI_IO_MUX_FSEL_SEL_LCD_SPI_MASK)

#define MCI_IO_MUX_FSEL_SEL_FREQ_GPIO_CLK_MASK   (0x2000U)
#define MCI_IO_MUX_FSEL_SEL_FREQ_GPIO_CLK_SHIFT  (13U)
/*! SEL_FREQ_GPIO_CLK - select GPIO-50 as freq_gpio_clk function */
#define MCI_IO_MUX_FSEL_SEL_FREQ_GPIO_CLK(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_FREQ_GPIO_CLK_SHIFT)) & MCI_IO_MUX_FSEL_SEL_FREQ_GPIO_CLK_MASK)

#define MCI_IO_MUX_FSEL_SEL_GPIO_INT_BMATCH_MASK (0x4000U)
#define MCI_IO_MUX_FSEL_SEL_GPIO_INT_BMATCH_SHIFT (14U)
/*! SEL_GPIO_INT_BMATCH - select GPIO-18 as gpio_int_bmatch function */
#define MCI_IO_MUX_FSEL_SEL_GPIO_INT_BMATCH(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_GPIO_INT_BMATCH_SHIFT)) & MCI_IO_MUX_FSEL_SEL_GPIO_INT_BMATCH_MASK)

#define MCI_IO_MUX_FSEL_SEL_SDIO_MASK            (0x8000U)
#define MCI_IO_MUX_FSEL_SEL_SDIO_SHIFT           (15U)
/*! SEL_SDIO - select GPIO-12/15/16/17/18/19/20 as sdio function */
#define MCI_IO_MUX_FSEL_SEL_SDIO(x)              (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_SDIO_SHIFT)) & MCI_IO_MUX_FSEL_SEL_SDIO_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_CLK_MASK        (0x10000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_CLK_SHIFT       (16U)
/*! SEL_ENET_CLK - select GPIO-25 as enet clk */
#define MCI_IO_MUX_FSEL_SEL_ENET_CLK(x)          (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_CLK_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_CLK_MASK)

#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER0_MASK    (0x20000U)
#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER0_SHIFT   (17U)
/*! SEL_GAU_TRIGGER0 - select GPIO-50 as gau trigger0 */
#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER0(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER0_SHIFT)) & MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER0_MASK)

#define MCI_IO_MUX_FSEL_SEL_ACOMP0_GPIO_OUT_MASK (0x40000U)
#define MCI_IO_MUX_FSEL_SEL_ACOMP0_GPIO_OUT_SHIFT (18U)
/*! SEL_ACOMP0_GPIO_OUT - select GPIO-51 as gau acomp0_gpio_out */
#define MCI_IO_MUX_FSEL_SEL_ACOMP0_GPIO_OUT(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ACOMP0_GPIO_OUT_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ACOMP0_GPIO_OUT_MASK)

#define MCI_IO_MUX_FSEL_SEL_ACOMP0_EDGE_PULSE_MASK (0x80000U)
#define MCI_IO_MUX_FSEL_SEL_ACOMP0_EDGE_PULSE_SHIFT (19U)
/*! SEL_ACOMP0_EDGE_PULSE - select GPIO-52 as gau acomp0_edge_pulse */
#define MCI_IO_MUX_FSEL_SEL_ACOMP0_EDGE_PULSE(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ACOMP0_EDGE_PULSE_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ACOMP0_EDGE_PULSE_MASK)

#define MCI_IO_MUX_FSEL_SEL_ACOMP1_GPIO_OUT_MASK (0x100000U)
#define MCI_IO_MUX_FSEL_SEL_ACOMP1_GPIO_OUT_SHIFT (20U)
/*! SEL_ACOMP1_GPIO_OUT - select GPIO-53 as gau acomp1_gpio_out */
#define MCI_IO_MUX_FSEL_SEL_ACOMP1_GPIO_OUT(x)   (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ACOMP1_GPIO_OUT_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ACOMP1_GPIO_OUT_MASK)

#define MCI_IO_MUX_FSEL_SEL_ACOMP1_EDGE_PULSE_MASK (0x200000U)
#define MCI_IO_MUX_FSEL_SEL_ACOMP1_EDGE_PULSE_SHIFT (21U)
/*! SEL_ACOMP1_EDGE_PULSE - select GPIO-54 as gau acomp1_edge_pulse */
#define MCI_IO_MUX_FSEL_SEL_ACOMP1_EDGE_PULSE(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ACOMP1_EDGE_PULSE_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ACOMP1_EDGE_PULSE_MASK)

#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER1_MASK    (0x400000U)
#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER1_SHIFT   (22U)
/*! SEL_GAU_TRIGGER1 - select GPIO-55 as gau trigger1 */
#define MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER1(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER1_SHIFT)) & MCI_IO_MUX_FSEL_SEL_GAU_TRIGGER1_MASK)

#define MCI_IO_MUX_FSEL_SEL_CLKIN_FRM_PD_MASK    (0x800000U)
#define MCI_IO_MUX_FSEL_SEL_CLKIN_FRM_PD_SHIFT   (23U)
/*! SEL_CLKIN_FRM_PD - select GPIO-4 as clkin function */
#define MCI_IO_MUX_FSEL_SEL_CLKIN_FRM_PD(x)      (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_CLKIN_FRM_PD_SHIFT)) & MCI_IO_MUX_FSEL_SEL_CLKIN_FRM_PD_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_TX_MASK         (0x2000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_TX_SHIFT        (25U)
/*! SEL_ENET_TX - select GPIO-58/59/60 as enet tx */
#define MCI_IO_MUX_FSEL_SEL_ENET_TX(x)           (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_TX_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_TX_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_RX_MASK         (0x4000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_RX_SHIFT        (26U)
/*! SEL_ENET_RX - select GPIO-22/23/62/63 as enet rx */
#define MCI_IO_MUX_FSEL_SEL_ENET_RX(x)           (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_RX_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_RX_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_MDIO_MASK       (0x8000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_MDIO_SHIFT      (27U)
/*! SEL_ENET_MDIO - select GPIO-56/57 as enet mdio */
#define MCI_IO_MUX_FSEL_SEL_ENET_MDIO(x)         (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_MDIO_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_MDIO_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER0_MASK     (0x10000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER0_SHIFT    (28U)
/*! SEL_ENET_TIMER0 - select GPIO-27 as enet timer0 */
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER0(x)       (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_TIMER0_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_TIMER0_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER1_MASK     (0x20000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER1_SHIFT    (29U)
/*! SEL_ENET_TIMER1 - select GPIO-61 as enet timer1 */
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER1(x)       (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_TIMER1_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_TIMER1_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER2_MASK     (0x40000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER2_SHIFT    (30U)
/*! SEL_ENET_TIMER2 - select GPIO-24 as enet timer2 */
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER2(x)       (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_TIMER2_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_TIMER2_MASK)

#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER3_MASK     (0x80000000U)
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER3_SHIFT    (31U)
/*! SEL_ENET_TIMER3 - select GPIO-26 as enet timer3 */
#define MCI_IO_MUX_FSEL_SEL_ENET_TIMER3(x)       (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_FSEL_SEL_ENET_TIMER3_SHIFT)) & MCI_IO_MUX_FSEL_SEL_ENET_TIMER3_MASK)
/*! @} */

/*! @name C_TIMER_IN - ctimer input function sel */
/*! @{ */

#define MCI_IO_MUX_C_TIMER_IN_CT_INP0_SEL_MASK   (0x1U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP0_SEL_SHIFT  (0U)
/*! CT_INP0_SEL - sel GPIO-0 as ct_inp0 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP0_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP0_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP0_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP1_SEL_MASK   (0x2U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP1_SEL_SHIFT  (1U)
/*! CT_INP1_SEL - sel GPIO-1 as ct_inp1 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP1_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP1_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP1_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP2_SEL_MASK   (0x4U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP2_SEL_SHIFT  (2U)
/*! CT_INP2_SEL - sel GPIO-12 as ct_inp2 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP2_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP2_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP2_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL_MASK   (0x8U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL_SHIFT  (3U)
/*! CT_INP3_SEL - sel GPIO-13 as ct_inp3 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP3_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL_MASK   (0x10U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL_SHIFT  (4U)
/*! CT_INP4_SEL - sel GPIO-14 as ct_inp4 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP4_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP5_SEL_MASK   (0x20U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP5_SEL_SHIFT  (5U)
/*! CT_INP5_SEL - sel GPIO-21 as ct_inp5 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP5_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP5_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP5_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP6_SEL_MASK   (0x40U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP6_SEL_SHIFT  (6U)
/*! CT_INP6_SEL - sel GPIO-24 as ct_inp6 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP6_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP6_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP6_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP7_SEL_MASK   (0x80U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP7_SEL_SHIFT  (7U)
/*! CT_INP7_SEL - sel GPIO-25 as ct_inp7 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP7_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP7_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP7_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP8_SEL_MASK   (0x100U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP8_SEL_SHIFT  (8U)
/*! CT_INP8_SEL - sel GPIO-37 as ct_inp8 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP8_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP8_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP8_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP9_SEL_MASK   (0x200U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP9_SEL_SHIFT  (9U)
/*! CT_INP9_SEL - sel GPIO-38 as ct_inp9 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP9_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP9_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP9_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP10_SEL_MASK  (0x400U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP10_SEL_SHIFT (10U)
/*! CT_INP10_SEL - sel GPIO-39 as ct_inp10 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP10_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP10_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP10_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP11_SEL_MASK  (0x800U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP11_SEL_SHIFT (11U)
/*! CT_INP11_SEL - sel GPIO-51 as ct_inp11 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP11_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP11_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP11_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP12_SEL_MASK  (0x1000U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP12_SEL_SHIFT (12U)
/*! CT_INP12_SEL - sel GPIO-52 as ct_inp12 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP12_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP12_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP12_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP13_SEL_MASK  (0x2000U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP13_SEL_SHIFT (13U)
/*! CT_INP13_SEL - sel GPIO-53 as ct_inp13 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP13_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP13_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP13_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_IN_CT_INP14_SEL_MASK  (0x4000U)
#define MCI_IO_MUX_C_TIMER_IN_CT_INP14_SEL_SHIFT (14U)
/*! CT_INP14_SEL - sel GPIO-54 as ct_inp14 */
#define MCI_IO_MUX_C_TIMER_IN_CT_INP14_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_IN_CT_INP14_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_IN_CT_INP14_SEL_MASK)
/*! @} */

/*! @name C_TIMER_OUT - ctimer output function sel */
/*! @{ */

#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT0_SEL_MASK  (0x1U)
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT0_SEL_SHIFT (0U)
/*! CT0MAT0_SEL - sel GPIO-0 as ct0mat0 */
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT0_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT0MAT0_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT0MAT0_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT1_SEL_MASK  (0x2U)
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT1_SEL_SHIFT (1U)
/*! CT0MAT1_SEL - sel GPIO-1 as ct0mat1 */
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT1_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT0MAT1_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT0MAT1_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT2_SEL_MASK  (0x4U)
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT2_SEL_SHIFT (2U)
/*! CT0MAT2_SEL - sel GPIO-12 as ct0mat2 */
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT2_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT0MAT2_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT0MAT2_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL_MASK  (0x8U)
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL_SHIFT (3U)
/*! CT0MAT3_SEL - sel GPIO-13 as ct0mat3 */
#define MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT0MAT3_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL_MASK  (0x10U)
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL_SHIFT (4U)
/*! CT1MAT0_SEL - sel GPIO-14 as ct1mat0 */
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT1MAT0_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT1_SEL_MASK  (0x20U)
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT1_SEL_SHIFT (5U)
/*! CT1MAT1_SEL - sel GPIO-21 as ct1mat1 */
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT1_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT1MAT1_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT1MAT1_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT2_SEL_MASK  (0x40U)
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT2_SEL_SHIFT (6U)
/*! CT1MAT2_SEL - sel GPIO-24 as ct1mat2 */
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT2_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT1MAT2_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT1MAT2_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT3_SEL_MASK  (0x80U)
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT3_SEL_SHIFT (7U)
/*! CT1MAT3_SEL - sel GPIO-25 as ct1mat3 */
#define MCI_IO_MUX_C_TIMER_OUT_CT1MAT3_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT1MAT3_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT1MAT3_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT0_SEL_MASK  (0x100U)
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT0_SEL_SHIFT (8U)
/*! CT2MAT0_SEL - sel GPIO-37 as ct2mat0 */
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT0_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT2MAT0_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT2MAT0_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT1_SEL_MASK  (0x200U)
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT1_SEL_SHIFT (9U)
/*! CT2MAT1_SEL - sel GPIO-38 as ct2mat1 */
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT1_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT2MAT1_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT2MAT1_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT2_SEL_MASK  (0x400U)
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT2_SEL_SHIFT (10U)
/*! CT2MAT2_SEL - sel GPIO-39 as ct2mat2 */
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT2_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT2MAT2_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT2MAT2_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT3_SEL_MASK  (0x800U)
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT3_SEL_SHIFT (11U)
/*! CT2MAT3_SEL - sel GPIO-51 as ct2mat3 */
#define MCI_IO_MUX_C_TIMER_OUT_CT2MAT3_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT2MAT3_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT2MAT3_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT0_SEL_MASK  (0x1000U)
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT0_SEL_SHIFT (12U)
/*! CT3MAT0_SEL - sel GPIO-52 as ct3mat0 */
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT0_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT3MAT0_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT3MAT0_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT1_SEL_MASK  (0x2000U)
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT1_SEL_SHIFT (13U)
/*! CT3MAT1_SEL - sel GPIO-53 as ct3mat1 */
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT1_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT3MAT1_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT3MAT1_SEL_MASK)

#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT2_SEL_MASK  (0x4000U)
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT2_SEL_SHIFT (14U)
/*! CT3MAT2_SEL - sel GPIO-54 as ct3mat2 */
#define MCI_IO_MUX_C_TIMER_OUT_CT3MAT2_SEL(x)    (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_C_TIMER_OUT_CT3MAT2_SEL_SHIFT)) & MCI_IO_MUX_C_TIMER_OUT_CT3MAT2_SEL_MASK)
/*! @} */

/*! @name SC_TIMER - sctimer function sel */
/*! @{ */

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP0_SEL_MASK (0x1U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP0_SEL_SHIFT (0U)
/*! SCT0_PIN_INP0_SEL - sel GPIO-3 as sct0_pin_inp0 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP0_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP0_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP0_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP1_SEL_MASK (0x2U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP1_SEL_SHIFT (1U)
/*! SCT0_PIN_INP1_SEL - sel GPIO-4 as sct0_pin_inp1 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP1_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP1_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP1_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP2_SEL_MASK (0x4U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP2_SEL_SHIFT (2U)
/*! SCT0_PIN_INP2_SEL - sel GPIO-22 as sct0_pin_inp2 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP2_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP2_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP2_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP3_SEL_MASK (0x8U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP3_SEL_SHIFT (3U)
/*! SCT0_PIN_INP3_SEL - sel GPIO-23 as sct0_pin_inp3 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP3_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP3_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP3_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP4_SEL_MASK (0x10U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP4_SEL_SHIFT (4U)
/*! SCT0_PIN_INP4_SEL - sel GPIO-26 as sct0_pin_inp4 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP4_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP4_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP4_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP5_SEL_MASK (0x20U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP5_SEL_SHIFT (5U)
/*! SCT0_PIN_INP5_SEL - sel GPIO-27 as sct0_pin_inp5 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP5_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP5_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP5_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP6_SEL_MASK (0x40U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP6_SEL_SHIFT (6U)
/*! SCT0_PIN_INP6_SEL - sel GPIO-35 as sct0_pin_inp6 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP6_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP6_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP6_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP7_SEL_MASK (0x80U)
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP7_SEL_SHIFT (7U)
/*! SCT0_PIN_INP7_SEL - sel GPIO-36 as sct0_pin_inp7 */
#define MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP7_SEL(x) (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP7_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_PIN_INP7_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT0_SEL_MASK   (0x10000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT0_SEL_SHIFT  (16U)
/*! SCT0_OUT0_SEL - sel GPIO-3 as sct0_out0 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT0_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT0_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT0_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT1_SEL_MASK   (0x20000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT1_SEL_SHIFT  (17U)
/*! SCT0_OUT1_SEL - sel GPIO-4 as sct0_out1 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT1_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT1_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT1_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT4_SEL_MASK   (0x100000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT4_SEL_SHIFT  (20U)
/*! SCT0_OUT4_SEL - sel GPIO-26 as sct0_out4 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT4_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT4_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT4_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT5_SEL_MASK   (0x200000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT5_SEL_SHIFT  (21U)
/*! SCT0_OUT5_SEL - sel GPIO-27 as sct0_out5 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT5_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT5_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT5_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT6_SEL_MASK   (0x400000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT6_SEL_SHIFT  (22U)
/*! SCT0_OUT6_SEL - sel GPIO-35 as sct0_out6 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT6_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT6_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT6_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT7_SEL_MASK   (0x800000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT7_SEL_SHIFT  (23U)
/*! SCT0_OUT7_SEL - sel GPIO-36 as sct0_out7 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT7_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT7_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT7_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT8_SEL_MASK   (0x1000000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT8_SEL_SHIFT  (24U)
/*! SCT0_OUT8_SEL - sel GPIO-11 as sct0_out8 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT8_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT8_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT8_SEL_MASK)

#define MCI_IO_MUX_SC_TIMER_SCT0_OUT9_SEL_MASK   (0x2000000U)
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT9_SEL_SHIFT  (25U)
/*! SCT0_OUT9_SEL - sel GPIO-55 as sct0_out9 */
#define MCI_IO_MUX_SC_TIMER_SCT0_OUT9_SEL(x)     (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_SC_TIMER_SCT0_OUT9_SEL_SHIFT)) & MCI_IO_MUX_SC_TIMER_SCT0_OUT9_SEL_MASK)
/*! @} */

/*! @name GPIO_GRP0 - GPIO[31:0] sel */
/*! @{ */

#define MCI_IO_MUX_GPIO_GRP0_SEL_MASK            (0xFFFFFFFFU)
#define MCI_IO_MUX_GPIO_GRP0_SEL_SHIFT           (0U)
/*! SEL - pio0[31:0] selection, high valid; sel[i]->pio0[i]->GPIO[i] */
#define MCI_IO_MUX_GPIO_GRP0_SEL(x)              (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_GPIO_GRP0_SEL_SHIFT)) & MCI_IO_MUX_GPIO_GRP0_SEL_MASK)
/*! @} */

/*! @name GPIO_GRP1 - GPIO[63:32] sel */
/*! @{ */

#define MCI_IO_MUX_GPIO_GRP1_SEL_MASK            (0xFFFFFFFFU)
#define MCI_IO_MUX_GPIO_GRP1_SEL_SHIFT           (0U)
/*! SEL - pio0[63:32] selection, high valid; sel[i]->pio0[i+32]->GPIO[i+32] */
#define MCI_IO_MUX_GPIO_GRP1_SEL(x)              (((uint32_t)(((uint32_t)(x)) << MCI_IO_MUX_GPIO_GRP1_SEL_SHIFT)) & MCI_IO_MUX_GPIO_GRP1_SEL_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group MCI_IO_MUX_Register_Masks */


/*!
 * @}
 */ /* end of group MCI_IO_MUX_Peripheral_Access_Layer */


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


#endif  /* MCI_IO_MUX_H_ */

