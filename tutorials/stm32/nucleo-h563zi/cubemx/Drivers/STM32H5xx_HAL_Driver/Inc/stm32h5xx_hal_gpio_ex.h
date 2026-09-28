/**
  ******************************************************************************
  * @file    stm32h5xx_hal_gpio_ex.h
  * @author  MCD Application Team
  * @brief   Header file of GPIO HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H5xx_HAL_GPIO_EX_H
#define STM32H5xx_HAL_GPIO_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup GPIOEx GPIOEx
  * @brief GPIO Extended HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Constants GPIOEx Exported Constants
  * @{
  */

/** @defgroup GPIOEx_Alternate_function_selection GPIOEx Alternate function selection
  * @{
  */

/**
  * @brief   AF 0 selection
  */
#define GPIO_AF0_RTC_50HZ      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping                       */
#define GPIO_AF0_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping            */
#define GPIO_AF0_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping             */
#define GPIO_AF0_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping                          */
#define GPIO_AF0_CSLEEP        ((uint8_t)0x00)  /* CSLEEP Alternate Function mapping                         */
#define GPIO_AF0_CSTOP         ((uint8_t)0x00)  /* CSTOP Alternate Function mapping                          */
#define GPIO_AF0_CRS           ((uint8_t)0x00)  /* CRS Alternate Function mapping                            */
#if defined(DMA2D)
#define GPIO_AF0_DMA2D         ((uint8_t)0x00)  /* DMA2D Alternate Function mapping                          */
#endif /* DMA2D */
#if defined(GFXTIM)
#define GPIO_AF0_GFXTIM        ((uint8_t)0x00)  /* GFXTIM Alternate Function mapping                         */
#endif /* GFXTIM */

/**
  * @brief   AF 1 selection
  */
#define GPIO_AF1_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping                           */
#define GPIO_AF1_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping                           */
#if defined(TIM16)
#define GPIO_AF1_TIM16         ((uint8_t)0x01)  /* TIM16 Alternate Function mapping                          */
#endif /* TIM16 */
#if defined(TIM17)
#define GPIO_AF1_TIM17         ((uint8_t)0x01)  /* TIM17 Alternate Function mapping                          */
#endif /* TIM17 */
#if !defined(STM32H503xx)
#define GPIO_AF1_LPTIM1        ((uint8_t)0x01)  /* LPTIM1 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if defined(ADF1)
#define GPIO_AF1_ADF1         ((uint8_t)0x01)   /* ADF1 Alternate Function mapping                            */
#endif /* ADF1 */

/**
  * @brief   AF 2 selection
  */
#if defined(STM32H503xx)
#define GPIO_AF2_LPTIM1        ((uint8_t)0x02)  /* LPTIM1 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if defined(LPTIM3)
#define GPIO_AF2_LPTIM3        ((uint8_t)0x02)  /* LPTIM3 Alternate Function mapping                         */
#endif /* LPTIM3 */
#if defined(SAI1)
#define GPIO_AF2_SAI1          ((uint8_t)0x02)  /* SAI1 Alternate Function mapping                           */
#endif /* SAI1 */
#define GPIO_AF2_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping                           */
#if defined(TIM4)
#define GPIO_AF2_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping                           */
#endif /* TIM4 */
#if defined(TIM5)
#define GPIO_AF2_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping                           */
#endif /* TIM5 */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF2_TIM8          ((uint8_t)0x02)  /* TIM8 Alternate Function mapping                           */
#endif /* STM32H533xx || STM32H523xx */
#if defined(TIM12)
#define GPIO_AF2_TIM12         ((uint8_t)0x02)  /* TIM12 Alternate Function mapping                          */
#endif /* TIM12 */
#if defined(TIM15)
#define GPIO_AF2_TIM15         ((uint8_t)0x02)  /* TIM15 Alternate Function mapping                          */
#endif /* TIM15 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF2_TIM2          ((uint8_t)0x02)  /* TIM2 Alternate Function mapping                          */
#define GPIO_AF2_TIM13         ((uint8_t)0x02)  /* TIM13 Alternate Function mapping                          */
#define GPIO_AF2_TIM14         ((uint8_t)0x02)  /* TIM14 Alternate Function mapping                          */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */

/**
  * @brief   AF 3 selection
  */
#define GPIO_AF3_I3C1          ((uint8_t)0x03)  /* I3C1 Alternate Function mapping                           */
#if defined(I3C2)
#define GPIO_AF3_I3C2          ((uint8_t)0x03)  /* I3C2 Alternate Function mapping                           */
#endif /* I3C2 */
#define GPIO_AF3_LPTIM2        ((uint8_t)0x03)  /* LPTIM2 Alternate Function mapping                         */
#if defined(LPTIM3)
#define GPIO_AF3_LPTIM3        ((uint8_t)0x03)  /* LPTIM3 Alternate Function mapping                         */
#endif /* LPTIM3 */
#define GPIO_AF3_LPUART1       ((uint8_t)0x03)  /* LPUART1 Alternate Function mapping                        */
#if defined(OCTOSPI1)
#define GPIO_AF3_OCTOSPI1      ((uint8_t)0x03)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* OCTOSPI1 */
#if !defined(STM32H503xx)
#define GPIO_AF3_TIM1          ((uint8_t)0x03)  /* TIM1 Alternate Function mapping                           */
#endif /* STM32H503xx */
#if defined(TIM8)
#define GPIO_AF3_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping                           */
#endif /* TIM8 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF3_COMP1         ((uint8_t)0x03)  /* COMP1 Alternate Function mapping                          */
#define GPIO_AF3_COMP2         ((uint8_t)0x03)  /* COMP2 Alternate Function mapping                          */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(ADF1)
#define GPIO_AF3_ADF1          ((uint8_t)0x03)  /* ADF1 Alternate Function mapping                           */
#endif /* ADF1 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF3_I2C3          ((uint8_t)0x03)  /* I2C3 Alternate Function mapping                           */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */

/**
  * @brief   AF 4 selection
  */
#if defined(CEC)
#define GPIO_AF4_CEC           ((uint8_t)0x04)  /* CEC Alternate Function mapping                            */
#endif /* CEC */
#if !defined(STM32H5F5xx) || !defined(STM32H5F4xx) || !defined(STM32H5E5xx) || !defined(STM32H5E4xx)
#if defined(DCMI)
#define GPIO_AF4_DCMI          ((uint8_t)0x04)  /* DCMI Alternate Function mapping                           */
#endif /* DCMI */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(PSSI)
#define GPIO_AF4_PSSI          ((uint8_t)0x04)  /* PSSI Alternate Function mapping                           */
#endif /* PSSI */
#define GPIO_AF4_I2C1          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping                           */
#define GPIO_AF4_I2C2          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping                           */
#if defined(I2C3)
#define GPIO_AF4_I2C3          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping                           */
#endif /* I2C3 */
#if defined(I2C4)
#define GPIO_AF4_I2C4          ((uint8_t)0x04)  /* I2C4 Alternate Function mapping                           */
#endif /* I2C4 */
#define GPIO_AF4_LPTIM1        ((uint8_t)0x04)  /* LPTIM1 Alternate Function mapping                         */
#define GPIO_AF4_LPTIM2        ((uint8_t)0x04)  /* LPTIM2 Alternate Function mapping                         */
#define GPIO_AF4_SPI1          ((uint8_t)0x04)  /* SPI1 Alternate Function mapping                           */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF4_SPI3          ((uint8_t)0x04)  /* SPI3 Alternate Function mapping                           */
#endif /* STM32H533xx || STM32H523xx */
#if defined(TIM15)
#define GPIO_AF4_TIM15         ((uint8_t)0x04)  /* TIM15 Alternate Function mapping                          */
#endif /* TIM15 */
#define GPIO_AF4_USART1        ((uint8_t)0x04)  /* USART1 Alternate Function mapping                         */
#if defined(STM32H503xx)
#define GPIO_AF4_USART2        ((uint8_t)0x04)  /* USART2 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF4_SAI1          ((uint8_t)0x04)  /* SAI1 Alternate Function mapping                           */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(MDF1)
#define GPIO_AF4_MDF1          ((uint8_t)0x04)  /* MDF1 Alternate Function mapping                           */
#endif /* MDF1 */
#if defined(ADF1)
#define GPIO_AF4_ADF1          ((uint8_t)0x04)  /* ADF1 Alternate Function mapping                           */
#endif /* ADF1 */

/**
  * @brief   AF 5 selection
  */
#if defined(CEC)
#define GPIO_AF5_CEC           ((uint8_t)0x05)  /* CEC Alternate Function mapping                            */
#endif /* CEC */
#if !defined(STM32H503xx)
#define GPIO_AF5_I3C1          ((uint8_t)0x05)  /* I3C1 Alternate Function mapping                           */
#define GPIO_AF5_SPI3          ((uint8_t)0x05)  /* SPI3 Alternate Function mapping                           */
#endif /* STM32H503xx */
#define GPIO_AF5_LPTIM1        ((uint8_t)0x05)  /* LPTIM1 Alternate Function mapping                         */
#define GPIO_AF5_SPI1          ((uint8_t)0x05)  /* SPI1 Alternate Function mapping                           */
#define GPIO_AF5_SPI2          ((uint8_t)0x05)  /* SPI2 Alternate Function mapping                           */
#if defined(SPI4)
#define GPIO_AF5_SPI4          ((uint8_t)0x05)  /* SPI4 Alternate Function mapping                           */
#endif /* SPI4 */
#if defined(SPI5)
#define GPIO_AF5_SPI5          ((uint8_t)0x05)  /* SPI5 Alternate Function mapping                           */
#endif /* SPI5 */
#if defined(SPI6)
#define GPIO_AF5_SPI6          ((uint8_t)0x05)  /* SPI6 Alternate Function mapping                           */
#endif /* SPI6 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF5_I3C2          ((uint8_t)0x05)  /* I3C2 Alternate Function mapping                           */
#if defined(GFXTIM)
#define GPIO_AF5_GFXTIM        ((uint8_t)0x05)  /* GFXTIM Alternate Function mapping                         */
#endif /* GFXTIM */
#define GPIO_AF5_AUDIOCLK      ((uint8_t)0x05)  /* AUDIOCLK Alternate Function mapping                       */
#define GPIO_AF5_USART2        ((uint8_t)0x05)  /* USART2 Alternate Function mapping                         */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */

/**
  * @brief   AF 6 selection
  */
#if defined(I2C4)
#define GPIO_AF6_I2C4          ((uint8_t)0x06)  /* I2C4 Alternate Function mapping                           */
#endif /* I2C4 */
#if defined(OCTOSPI1)
#define GPIO_AF6_OCTOSPI1      ((uint8_t)0x06)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* OCTOSPI1 */
#if defined(SAI1)
#define GPIO_AF6_SAI1          ((uint8_t)0x06)  /* SAI1 Alternate Function mapping                           */
#endif /* SAI1 */
#if (defined(STM32H503xx) || defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF6_SPI1          ((uint8_t)0x06)  /* SPI1 Alternate Function mapping                           */
#define GPIO_AF6_SPI2          ((uint8_t)0x06)  /* SPI2 Alternate Function mapping                           */
#endif /* STM32H503xx || STM32H533xx || STM32H523xx */
#define GPIO_AF6_SPI3          ((uint8_t)0x06)  /* SPI3 Alternate Function mapping                           */
#if defined(SPI4)
#define GPIO_AF6_SPI4          ((uint8_t)0x06)  /* SPI4 Alternate Function mapping                           */
#endif /* SPI4 */
#if defined(UART4)
#define GPIO_AF6_UART4         ((uint8_t)0x06)  /* UART4 Alternate Function mapping                          */
#endif /* UART4 */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF6_USART6        ((uint8_t)0x06)  /* USART6 Alternate Function mapping                         */
#endif /* STM32H533xx || STM32H523xx */
#if defined(UART12)
#define GPIO_AF6_UART12        ((uint8_t)0x06)  /* UART12 Alternate Function mapping                         */
#endif /* UART12 */
#if defined(USART10)
#define GPIO_AF6_USART10       ((uint8_t)0x06)  /* USART10 Alternate Function mapping                        */
#endif /* USART10 */
#if defined(UCPD1)
#define GPIO_AF6_UCPD1         ((uint8_t)0x06)  /* UCPD1 Alternate Function mapping                          */
#endif /* UCPD1 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF6_I2C2          ((uint8_t)0x06)  /* I2C2 Alternate Function mapping                           */
#define GPIO_AF6_ETH           ((uint8_t)0x06)  /* ETH Alternate Function mapping                            */
#define GPIO_AF6_I2C1          ((uint8_t)0x06)  /* I2C1 Alternate Function mapping                           */
#define GPIO_AF6_USART12       ((uint8_t)0x06)  /* USART12 Alternate Function mapping                        */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */

/**
  * @brief   AF 7 selection
  */
#if defined(SDMMC1)
#define GPIO_AF7_SDMMC1        ((uint8_t)0x07)  /* SDMMC1 Alternate Function mapping                         */
#endif /* SDMMC1 */
#define GPIO_AF7_SPI2          ((uint8_t)0x07)  /* SPI2 Alternate Function mapping                           */
#define GPIO_AF7_SPI3          ((uint8_t)0x07)  /* SPI3 Alternate Function mapping                           */
#if defined(SPI6)
#define GPIO_AF7_SPI6          ((uint8_t)0x07)  /* SPI6 Alternate Function mapping                           */
#endif /* SPI6 */
#if defined(UART7)
#define GPIO_AF7_UART7         ((uint8_t)0x07)  /* UART7 Alternate Function mapping                          */
#endif /* UART7 */
#if defined(UART8)
#define GPIO_AF7_UART8         ((uint8_t)0x07)  /* UART8 Alternate Function mapping                          */
#endif /* UART8 */
#if !defined(STM32H5F5xx) || !defined(STM32H5F4xx) || !defined(STM32H5E5xx) || !defined(STM32H5E4xx)
#if defined(UART12)
#define GPIO_AF7_UART12        ((uint8_t)0x07)  /* UART12 Alternate Function mapping                         */
#endif /* UART12 */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#define GPIO_AF7_USART1        ((uint8_t)0x07)  /* USART1 Alternate Function mapping                         */
#define GPIO_AF7_USART2        ((uint8_t)0x07)  /* USART2 Alternate Function mapping                         */
#define GPIO_AF7_USART3        ((uint8_t)0x07)  /* USART3 Alternate Function mapping                         */
#if defined(USART6)
#define GPIO_AF7_USART6        ((uint8_t)0x07)  /* USART6 Alternate Function mapping                         */
#endif /* USART6 */
#if defined(USART10)
#define GPIO_AF7_USART10       ((uint8_t)0x07)  /* USART10 Alternate Function mapping                        */
#endif /* USART10 */
#if defined(USART11)
#define GPIO_AF7_USART11       ((uint8_t)0x07)  /* USART11 Alternate Function mapping                        */
#endif /* USART11 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF7_ETH           ((uint8_t)0x07)  /* ETH Alternate Function mapping                         */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
/**
  * @brief   AF 8 selection
  */
#if defined(STM32H503xx)
#define GPIO_AF8_I2C2          ((uint8_t)0x08)  /* I2C2 Alternate Function mapping                           */
#define GPIO_AF8_I3C1          ((uint8_t)0x08)  /* I3C1 Alternate Function mapping                           */
#define GPIO_AF8_USART1        ((uint8_t)0x08)  /* USART1 Alternate Function mapping                         */
#endif /* STM32H503xx */
#define GPIO_AF8_LPUART1       ((uint8_t)0x08)  /* LPUART1 Alternate Function mapping                        */
#if defined(SAI2)
#define GPIO_AF8_SAI2          ((uint8_t)0x08)  /* SAI2 Alternate Function mapping                           */
#endif /* SAI2 */
#if defined(SDMMC1)
#define GPIO_AF8_SDMMC1        ((uint8_t)0x08)  /* SDMMC1 Alternate Function mapping                         */
#endif /* SDMMC1 */
#if defined(SPI6)
#define GPIO_AF8_SPI6          ((uint8_t)0x08)  /* SPI6 Alternate Function mapping                           */
#endif /* SPI6 */
#if defined(UART4)
#define GPIO_AF8_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping                          */
#endif /* UART4 */
#if defined(UART5)
#define GPIO_AF8_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping                          */
#endif /* UART5 */
#if defined(UART8)
#define GPIO_AF8_UART8         ((uint8_t)0x08)  /* UART8 Alternate Function mapping                          */
#endif /* UART8 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF8_ETH           ((uint8_t)0x08)  /* ETH Alternate Function mapping                            */
#define GPIO_AF8_FMC           ((uint8_t)0x08)  /* FMC Alternate Function mapping                            */
#define GPIO_AF8_I3C2          ((uint8_t)0x08)  /* I3C2 Alternate Function mapping                           */
#define GPIO_AF8_OCTOSPI1      ((uint8_t)0x08)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(OCTOSPI2)
#define GPIO_AF8_OCTOSPI2      ((uint8_t)0x08)  /* OCTOSPI2 Alternate Function mapping                       */
#endif /* OCTOSPI2 */
#if defined(MDF1)
#define GPIO_AF8_MDF1          ((uint8_t)0x08)  /* MDF1 Alternate Function mapping                           */
#endif /* MDF1 */

/**
  * @brief   AF 9 selection
  */
#define GPIO_AF9_FDCAN1        ((uint8_t)0x09)  /* FDCAN1 Alternate Function mapping                         */
#if defined(FDCAN2)
#define GPIO_AF9_FDCAN2        ((uint8_t)0x09)  /* FDCAN2 Alternate Function mapping                         */
#endif /* FDCAN2 */
#if defined(FMC_BANK1)
#define GPIO_AF9_FMC           ((uint8_t)0x09)  /* FMC Alternate Function mapping                            */
#endif /* FMC_BANK1 */
#if defined(OCTOSPI1)
#define GPIO_AF9_OCTOSPI1      ((uint8_t)0x09)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* OCTOSPI1 */
#if defined(SDMMC2)
#define GPIO_AF9_SDMMC2        ((uint8_t)0x09)  /* SDMMC2 Alternate Function mapping                         */
#endif /* SDMMC2 */
#if defined(TIM13)
#define GPIO_AF9_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping                          */
#endif /* TIM13 */
#if defined(TIM14)
#define GPIO_AF9_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping                          */
#endif /* TIM14 */
#if defined(STM32H503xx)
#define GPIO_AF9_USART2        ((uint8_t)0x09)  /* USART2 Alternate Function mapping                         */
#define GPIO_AF9_USART3        ((uint8_t)0x09)  /* USART3 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF9_I2C3          ((uint8_t)0x09)  /* I2C3 Alternate Function mapping                           */
#define GPIO_AF9_I3C2          ((uint8_t)0x09)  /* I3C2 Alternate Function mapping                           */
#endif /* STM32H533xx || STM32H523xx */
#if defined(OCTOSPI2)
#define GPIO_AF9_OCTOSPI2      ((uint8_t)0x09)  /* OCTOSPI2 Alternate Function mapping                       */
#endif /* OCTOSPI2 */
#if defined(FDCAN3)
#define GPIO_AF9_FDCAN3        ((uint8_t)0x09)  /* FDCAN3 Alternate Function mapping                         */
#endif /* FDCAN3 */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF9_SDMMC1        ((uint8_t)0x09)  /* SDMMC1 Alternate Function mapping                         */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */

/**
  * @brief   AF 10 selection
  */
#define GPIO_AF10_CRS          ((uint8_t)0x0A)  /* CRS Alternate Function mapping                            */
#if defined(STM32H503xx)
#define GPIO_AF10_I3C1         ((uint8_t)0x0A)  /* I3C1 Alternate Function mapping                           */
#define GPIO_AF10_SPI3         ((uint8_t)0x0A)  /* SPI3 Alternate Function mapping                           */
#endif /* STM32H503xx */
#if (defined(STM32H503xx) || defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF10_I3C2         ((uint8_t)0x0A)  /* I3C2 Alternate Function mapping                           */
#endif /* STM32H503xx || STM32H533xx || STM32H523xx */
#if defined(FMC_BANK1)
#define GPIO_AF10_FMC          ((uint8_t)0x0A)  /* FMC Alternate Function mapping                            */
#endif /* FMC_BANK1 */
#if defined(OCTOSPI1)
#define GPIO_AF10_OCTOSPI1     ((uint8_t)0x0A)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* OCTOSPI1 */
#if defined(SAI2)
#define GPIO_AF10_SAI2         ((uint8_t)0x0A)  /* SAI2 Alternate Function mapping                           */
#endif /* SAI2 */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF10_SDMMC1       ((uint8_t)0x0A)  /* SDMMC1 Alternate Function mapping                         */
#endif /* STM32H533xx || STM32H523xx */
#if defined(SDMMC2)
#define GPIO_AF10_SDMMC2       ((uint8_t)0x0A)  /* SDMMC2 Alternate Function mapping                         */
#endif /* SDMMC2 */
#if defined(TIM8)
#define GPIO_AF10_TIM8         ((uint8_t)0x0A)  /* TIM8 Alternate Function mapping                           */
#endif /* TIM8 */
#if defined(USB_DRD_FS)
#define GPIO_AF10_USB          ((uint8_t)0x0A)  /* USB Alternate Function mapping                            */
#endif /* USB_DRD_FS */
#if defined(LTDC)
#define GPIO_AF10_LTDC         ((uint8_t)0x0A)  /* LTDC Alternate Function mapping                           */
#endif /* LTDC */
#if defined(ETH)
#define GPIO_AF10_ETH          ((uint8_t)0x0A)  /* ETH Alternate Function mapping                            */
#endif /* ETH */
#if defined(OCTOSPI2)
#define GPIO_AF10_OCTOSPI2     ((uint8_t)0x0A)  /* OCTOSPI2 Alternate Function mapping                       */
#endif /* OCTOSPI2 */
#if defined(USB_OTG_FS)
#define GPIO_AF10_OTG_FS       ((uint8_t)0x0A)  /* USB OTG FS Alternate Function mapping                     */
#endif /* USB_OTG_FS */
#if defined(USB_OTG_HS)
#define GPIO_AF10_OTG_HS       ((uint8_t)0x0A)  /* USB OTG HS Alternate Function mapping                    */
#endif /* USB_OTG_HS */

/**
  * @brief   AF 11 selection
  */
#if defined(ETH)
#define GPIO_AF11_ETH          ((uint8_t)0x0B)  /* ETH Alternate Function mapping                            */
#endif /* ETH */
#if defined(FMC_BANK1)
#define GPIO_AF11_FMC          ((uint8_t)0x0B)  /* FMC Alternate Function mapping                            */
#endif /* FMC_BANK1 */
#if defined(OCTOSPI1)
#define GPIO_AF11_OCTOSPI1     ((uint8_t)0x0B)  /* OCTOSPI1 Alternate Function mapping                       */
#endif /* OCTOSPI1 */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF11_SDMMC1       ((uint8_t)0x0B)  /* SDMMC1 Alternate Function mapping                         */
#endif /* STM32H533xx || STM32H523xx */
#if defined(SDMMC2)
#define GPIO_AF11_SDMMC2       ((uint8_t)0x0B)  /* SDMMC2 Alternate Function mapping                         */
#endif /* SDMMC2 */
#if defined(UART7)
#define GPIO_AF11_UART7        ((uint8_t)0x0B)  /* UART7 Alternate Function mapping                          */
#endif /* UART7 */
#if defined(UART9)
#define GPIO_AF11_UART9        ((uint8_t)0x0B)  /* UART9 Alternate Function mapping                          */
#endif /* UART9 */
#if defined(UCPD1)
#define GPIO_AF11_UCPD1        ((uint8_t)0x0B)  /* UCPD1 Alternate Function mapping                          */
#endif /* UCPD1 */
#if defined(STM32H503xx)
#define GPIO_AF11_I2C1         ((uint8_t)0x0B)  /* I2C1 Alternate Function mapping                           */
#define GPIO_AF11_I2C2         ((uint8_t)0x0B)  /* I2C2 Alternate Function mapping                           */
#define GPIO_AF11_SPI2         ((uint8_t)0x0B)  /* SPI2 Alternate Function mapping                           */
#define GPIO_AF11_USART2       ((uint8_t)0x0B)  /* USART2 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if defined(LTDC)
#define GPIO_AF11_LTDC         ((uint8_t)0x0B)  /* LTDC Alternate Function mapping                           */
#endif /* LTDC */

/**
  * @brief   AF 12 selection
  */
#if defined(FMC_BANK1)
#define GPIO_AF12_FMC          ((uint8_t)0x0C)  /* FMC Alternate Function mapping                            */
#endif /* FMC_BANK1 */
#if defined(SDMMC1)
#define GPIO_AF12_SDMMC1       ((uint8_t)0x0C)  /* SDMMC1 Alternate Function mapping                         */
#endif /* SDMMC1 */
#if defined(STM32H503xx)
#define GPIO_AF12_COMP1        ((uint8_t)0x0C)  /* COMP1 Alternate Function mapping                          */
#define GPIO_AF12_SPI1         ((uint8_t)0x0C)  /* SPI1 Alternate Function mapping                           */
#endif /* STM32H503xx */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF12_ETH          ((uint8_t)0x0C)  /* ETH Alternate Function mapping                            */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(LTDC)
#define GPIO_AF12_LTDC         ((uint8_t)0x0C)  /* LTDC Alternate Function mapping                           */
#endif /* LTDC */

/**
  * @brief   AF 13 selection
  */
#if defined(DCMI)
#define GPIO_AF13_DCMI         ((uint8_t)0x0D)  /* DCMI Alternate Function mapping                           */
#define GPIO_AF13_PSSI         ((uint8_t)0x0D)  /* PSSI Alternate Function mapping                           */
#endif /* DCMI */
#if defined(FMC_BANK1)
#define GPIO_AF13_FMC          ((uint8_t)0x0D)  /* FMC Alternate Function mapping                            */
#endif /* FMC_BANK1 */
#if defined(LPTIM5)
#define GPIO_AF13_LPTIM5       ((uint8_t)0x0D)  /* LPTIM5 Alternate Function mapping                         */
#endif /* LPTIM5 */
#if defined(STM32H503xx)
#define GPIO_AF13_USART2       ((uint8_t)0x0D)  /* USART2 Alternate Function mapping                         */
#define GPIO_AF13_USART3       ((uint8_t)0x0D)  /* USART3 Alternate Function mapping                         */
#endif /* STM32H503xx */
#if defined(STM32H5F5xx) || defined(STM32H5F4xx) || defined(STM32H5E5xx) || defined(STM32H5E4xx)
#define GPIO_AF13_LPTIM6       ((uint8_t)0x0D)  /* LPTIM6 Alternate Function mapping                         */
#endif /* STM32H5F5xx || STM32H5F4xx || STM32H5E5xx || STM32H5E4xx */
#if defined(LTDC)
#define GPIO_AF13_LTDC         ((uint8_t)0x0D)  /* LTDC Alternate Function mapping                           */
#endif /* LTDC */

/**
  * @brief   AF 14 selection
  */
#if defined(STM32H503xx)
#define GPIO_AF14_LPTIM1       ((uint8_t)0x0E)  /* LPTIM1 Alternate Function mapping                         */
#define GPIO_AF14_LPTIM2       ((uint8_t)0x0E)  /* LPTIM2 Alternate Function mapping                         */
#define GPIO_AF14_TIM1         ((uint8_t)0x0E)  /* TIM1 Alternate Function mapping                           */
#define GPIO_AF14_TIM2         ((uint8_t)0x0E)  /* TIM2 Alternate Function mapping                           */
#define GPIO_AF14_TIM3         ((uint8_t)0x0E)  /* TIM3 Alternate Function mapping                           */
#endif /* STM32H503xx */
#if defined(LPTIM3)
#define GPIO_AF14_LPTIM3       ((uint8_t)0x0E)  /* LPTIM3 Alternate Function mapping                         */
#endif /* LPTIM3 */
#if defined(LPTIM4)
#define GPIO_AF14_LPTIM4       ((uint8_t)0x0E)  /* LPTIM4 Alternate Function mapping                         */
#endif /* LPTIM4 */
#if defined(LPTIM5)
#define GPIO_AF14_LPTIM5       ((uint8_t)0x0E)  /* LPTIM5 Alternate Function mapping                         */
#endif /* LPTIM5 */
#if defined(LPTIM6)
#define GPIO_AF14_LPTIM6       ((uint8_t)0x0E)  /* LPTIM6 Alternate Function mapping                         */
#endif /* LPTIM6 */
#if defined(STM32H573xx) || defined(STM32H563xx) || defined(STM32H562xx) || defined(STM32H533xx) || defined(STM32H523xx)
#define GPIO_AF14_TIM2         ((uint8_t)0x0E)  /* TIM2 Alternate Function mapping                           */
#endif /* STM32H573xx || STM32H563xx || STM32H562xx || STM32H533xx || STM32H523xx */
#if defined(UART5)
#define GPIO_AF14_UART5        ((uint8_t)0x0E)  /* UART5 Alternate Function mapping                          */
#endif /* UART5 */
#if (defined(STM32H533xx) || defined(STM32H523xx))
#define GPIO_AF14_USART6       ((uint8_t)0x0E)  /* USART6 Alternate Function mapping                         */
#endif /* STM32H533xx || STM32H523xx */
#if defined(LTDC)
#define GPIO_AF14_LTDC         ((uint8_t)0x0E)  /* LTDC Alternate Function mapping                           */
#endif /* LTDC */
#if defined(PLAY1)
#define GPIO_AF14_PLAY1_IN     ((uint8_t)0x0E)  /* PLAY1_IN Alternate Function mapping                       */
#define GPIO_AF14_PLAY1_OUT    ((uint8_t)0x0E)  /* PLAY1_OUT Alternate Function mapping                      */
#endif /* PLAY1 */

/**
  * @brief   AF 15 selection
  */
#define GPIO_AF15_EVENTOUT     ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#define IS_GPIO_AF(AF)   ((AF) <= (uint8_t)0x0F)


/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup GPIOEx_Exported_Macros GPIOEx Exported Macros
  * @{
  */

/** @defgroup GPIOEx_Get_Port_Index GPIOEx_Get Port Index
  * @{
  */


/* GPIO_Peripheral_Memory_Mapping Peripheral Memory Mapping */

#if defined(STM32H573xx) || defined(STM32H563xx) || defined(STM32H562xx) || defined(STM32H533xx) || \
    defined(STM32H523xx) || defined(STM32H503xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) || \
    defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H553xx) || defined(STM32H543xx)
#define GPIO_GET_INDEX(__GPIOx__)           (((uint32_t )(__GPIOx__) & (~GPIOA_BASE)) >> 10)
#endif /* (defined(STM32H573xx) || defined(STM32H563xx) || defined(STM32H562xx) || defined(STM32H533xx) || \
          defined(STM32H523xx) || defined(STM32H503xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) || \
          defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H553xx) || defined(STM32H543xx)*/


/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_HAL_GPIO_EX_H */
