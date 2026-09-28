/**
  ******************************************************************************
  * @file    stm32h5xx_hal_rcc.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL  module.
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
#ifndef __STM32H5xx_HAL_RCC_H
#define __STM32H5xx_HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup RCC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup RCC_Exported_Types RCC Exported Types
  * @{
  */

/**
  * @brief  RCC PLL1 configuration structure definition
  */
typedef struct
{
  uint32_t PLLState;   /*!< PLLState: The new state of the PLL1.
                            This parameter can be a value of @ref RCC_PLL1_Config                      */

  uint32_t PLLSource;  /*!< PLLSource: PLL entry clock source.
                            This parameter must be a value of @ref RCC_PLL1_Clock_Source               */

  uint32_t PLLM;       /*!< PLLM: Division factor for PLL1 VCO input clock.
                            This parameter must be a number between Min_Data = 0 and Max_Data = 63    */

  uint32_t PLLN;       /*!< PLLN: Multiplication factor for PLL1 VCO output clock.
                            This parameter must be a number between Min_Data = 4 and Max_Data = 512   */

  uint32_t PLLP;       /*!< PLLP: Division factor for system clock.
                            This parameter must be a number between Min_Data = 2 and Max_Data = 128
                            odd division factors are not allowed                                      */

  uint32_t PLLQ;       /*!< PLLQ: Division factor for peripheral clocks.
                            This parameter must be a number between Min_Data = 1 and Max_Data = 128   */

  uint32_t PLLR;       /*!< PLLR: Division factor for peripheral clocks.
                            This parameter must be a number between Min_Data = 1 and Max_Data = 128   */

  uint32_t PLLRGE;     /*!< PLLRGE: PLL1 clock Input range
                            This parameter must be a value of @ref RCC_PLL1_VCI_Range                 */

  uint32_t PLLVCOSEL;  /*!< PLLVCOSEL: PLL1 clock Output range
                            This parameter must be a value of @ref RCC_PLL1_VCO_Range                 */

  uint32_t PLLFRACN;   /*!< PLLFRACN: Specifies Fractional Part Of The Multiplication Factor for
                            PLL1 VCO It should be a value between 0 and 8191                          */

} RCC_PLLInitTypeDef;

/**
  * @brief  RCC Internal/External Oscillator (HSE, HSI, CSI, LSE and LSI) configuration structure definition
  */
typedef struct
{
  uint32_t OscillatorType;       /*!< The oscillators to be configured.
                                      This parameter can be a value of @ref RCC_Oscillator_Type                   */

  uint32_t HSEState;             /*!< The new state of the HSE.
                                      This parameter can be a value of @ref RCC_HSE_Config                        */

  uint32_t LSEState;             /*!< The new state of the LSE.
                                      This parameter can be a value of @ref RCC_LSE_Config                        */

  uint32_t HSIState;             /*!< The new state of the HSI.
                                      This parameter can be a value of @ref RCC_HSI_Config                        */

  uint32_t HSIDiv;               /*!< The division factor of the HSI.
                                      This parameter can be a value of @ref RCC_HSI_Div                           */

  uint32_t HSICalibrationValue;  /*!< The calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x7F
                                      on the other devices */

  uint32_t LSIState;             /*!< The new state of the LSI.
                                      This parameter can be a value of @ref RCC_LSI_Config                        */

  uint32_t CSIState;             /*!< The new state of the CSI.
                                      This parameter can be a value of @ref RCC_CSI_Config */

  uint32_t CSICalibrationValue;  /*!< The calibration trimming value (default is RCC_CSICALIBRATION_DEFAULT).
                                      This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x3F */

  uint32_t HSI48State;            /*!< The new state of the HSI48.
                                        This parameter can be a value of @ref RCC_HSI48_Config                    */

  RCC_PLLInitTypeDef PLL;         /*!<  PLL1 structure parameters                                                 */

} RCC_OscInitTypeDef;

/**
  * @brief  RCC System, AHB and APB busses clock configuration structure definition
  */
typedef struct
{
  uint32_t ClockType;             /*!< The clock to be configured.
                                       This parameter can be a value of @ref RCC_System_Clock_Type      */

  uint32_t SYSCLKSource;          /*!< The clock source used as system clock (SYSCLK).
                                       This parameter can be a value of @ref RCC_System_Clock_Source    */

  uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).
                                       This parameter can be a value of @ref RCC_AHB_Clock_Source       */

  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_APB3_Clock_Source */

  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_APB3_Clock_Source */

  uint32_t APB3CLKDivider;        /*!< The APB3 clock (PCLK3) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_APB1_APB2_APB3_Clock_Source      */
} RCC_ClkInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCC_Exported_Constants RCC Exported Constants
  * @{
  */

/** @defgroup RCC_Peripheral_Memory_Mapping Peripheral Memory Mapping
  * @{
  */

/**
  * @}
  */

/** @defgroup RCC_Oscillator_Type Oscillator Type
  * @{
  */
#define RCC_OSCILLATORTYPE_NONE        (0x00000000U)   /*!< Oscillator configuration unchanged */
#define RCC_OSCILLATORTYPE_HSE         (0x00000001U)   /*!< HSE to configure */
#define RCC_OSCILLATORTYPE_HSI         (0x00000002U)   /*!< HSI to configure */
#define RCC_OSCILLATORTYPE_LSE         (0x00000004U)   /*!< LSE to configure */
#define RCC_OSCILLATORTYPE_LSI         (0x00000008U)   /*!< LSI to configure */
#define RCC_OSCILLATORTYPE_CSI         (0x00000010U)   /*!< CSI to configure */
#define RCC_OSCILLATORTYPE_HSI48       (0x00000020U)   /*!< HSI48 to configure */
/**
  * @}
  */

/** @defgroup RCC_HSE_Config HSE Config
  * @{
  */
#define RCC_HSE_OFF                    (0x00000000U)                                               /*!< HSE clock deactivation */
#define RCC_HSE_ON                     RCC_CR_HSEON                                                /*!< HSE clock activation */
#define RCC_HSE_BYPASS                 ((uint32_t)(RCC_CR_HSEBYP | RCC_CR_HSEON))                  /*!< External Analog clock source for HSE clock */
#define RCC_HSE_BYPASS_DIGITAL         ((uint32_t)(RCC_CR_HSEEXT | RCC_CR_HSEBYP | RCC_CR_HSEON))  /*!< External Digital clock source for HSE clock */
/**
  * @}
  */

/** @defgroup RCC_LSE_Config LSE Config
  * @{
  */
#define RCC_LSE_OFF                    0U                                                                 /*!< LSE clock deactivation */
#define RCC_LSE_ON                     RCC_BDCR_LSEON                                                     /*!< LSE clock activation  */
#define RCC_LSE_BYPASS                 ((uint32_t)(RCC_BDCR_LSEBYP | RCC_BDCR_LSEON))                     /*!< External Analog clock source for LSE clock Bypassed*/
#define RCC_LSE_BYPASS_DIGITAL         ((uint32_t)(RCC_BDCR_LSEEXT | RCC_BDCR_LSEBYP | RCC_BDCR_LSEON))   /*!< External Digital clock source for LSE clock Bypassed */
/**
  * @}
  */

/** @defgroup RCC_HSI_Config HSI Config
  * @{
  */
#define RCC_HSI_OFF                    0x00000000U            /*!< HSI clock deactivation */
#define RCC_HSI_ON                     RCC_CR_HSION           /*!< HSI clock activation */

#define RCC_HSICALIBRATION_DEFAULT     (0x40U)      /* Default HSI calibration trimming value */
/**
  * @}
  */

/** @defgroup RCC_HSI_Div HSI Div
  * @{
  */
#define RCC_HSI_DIV1                   0x00000000U                            /*!< HSI clock is not divided */
#define RCC_HSI_DIV2                   RCC_CR_HSIDIV_0                        /*!< HSI clock is divided by 2 */
#define RCC_HSI_DIV4                   RCC_CR_HSIDIV_1                        /*!< HSI clock is divided by 4 */
#define RCC_HSI_DIV8                   (RCC_CR_HSIDIV_1|RCC_CR_HSIDIV_0)      /*!< HSI clock is divided by 8 */
/**
  * @}
  */

/** @defgroup RCC_LSI_Config LSI Config
  * @{
  */
#define RCC_LSI_OFF                    (0x00000000U)      /*!< LSI clock deactivation */
#define RCC_LSI_ON                     RCC_BDCR_LSION     /*!< LSI clock activation */
/**
  * @}
  */

/** @defgroup RCC_CSI_Config CSI Config
  * @{
  */
#define RCC_CSI_OFF                    (0x00000000U)       /*!< CSI clock deactivation */
#define RCC_CSI_ON                     RCC_CR_CSION        /*!< CSI clock activation */

#define RCC_CSICALIBRATION_DEFAULT     (0x20U)   /*!< Default CSI calibration trimming value */
/**
  * @}
  */

/** @defgroup RCC_HSI48_Config HSI48 Config
  * @{
  */
#define RCC_HSI48_OFF                  (0x00000000U)      /*!< HSI48 clock deactivation */
#define RCC_HSI48_ON                   RCC_CR_HSI48ON     /*!< HSI48 clock activation */
/**
  * @}
  */

/** @defgroup RCC_PLL1_Config  RCC PLL1 Config
  * @{
  */
#define RCC_PLL_NONE                 (0x00000000U)
#define RCC_PLL_OFF                  (0x00000001U)
#define RCC_PLL_ON                   (0x00000002U)
/**
  * @}
  */

/** @defgroup RCC_PLL1_Clock_Output  RCC PLL1 Clock Output
  * @{
  */
#define RCC_PLL1_DIVP                RCC_PLL1CFGR_PLL1PEN
#define RCC_PLL1_DIVQ                RCC_PLL1CFGR_PLL1QEN
#define RCC_PLL1_DIVR                RCC_PLL1CFGR_PLL1REN
/**
  * @}
  */

/** @defgroup RCC_PLL1_VCI_Range  RCC PLL1 VCI Range
  * @{
  */
#define RCC_PLL1_VCIRANGE_0              (0x00000000U)                                        /*!< Clock range frequency between 1 and 2 MHz  */
#define RCC_PLL1_VCIRANGE_1              RCC_PLL1CFGR_PLL1RGE_0                               /*!< Clock range frequency between 2 and 4 MHz  */
#define RCC_PLL1_VCIRANGE_2              RCC_PLL1CFGR_PLL1RGE_1                               /*!< Clock range frequency between 4 and 8 MHz  */
#define RCC_PLL1_VCIRANGE_3              (RCC_PLL1CFGR_PLL1RGE_0 | RCC_PLL1CFGR_PLL1RGE_1)    /*!< Clock range frequency between 8 and 16 MHz */
/**
  * @}
  */

/** @defgroup RCC_PLL1_VCO_Range  RCC PLL1 VCO Range
  * @{
  */
#define RCC_PLL1_VCORANGE_WIDE           (0x00000000U)              /*!< Clock range frequency between 192 and 836 MHz  */
#define RCC_PLL1_VCORANGE_MEDIUM         RCC_PLL1CFGR_PLL1VCOSEL    /*!< Clock range frequency between 150 and 420 MHz  */

/**
  * @}
  */

/** @defgroup RCC_PLL1_Clock_Source  RCC PLL1 Clock Source
  * @{
  */
#define RCC_PLL1_SOURCE_NONE             (0x00000000U)
#define RCC_PLL1_SOURCE_HSI              RCC_PLL1CFGR_PLL1SRC_0
#define RCC_PLL1_SOURCE_CSI              RCC_PLL1CFGR_PLL1SRC_1
#define RCC_PLL1_SOURCE_HSE              (RCC_PLL1CFGR_PLL1SRC_0 | RCC_PLL1CFGR_PLL1SRC_1)
/**
  * @}
  */


/** @defgroup RCC_System_Clock_Type System Clock Type
  * @{
  */
#define RCC_CLOCKTYPE_SYSCLK           (0x00000001U)  /*!< SYSCLK to configure */
#define RCC_CLOCKTYPE_HCLK             (0x00000002U)  /*!< HCLK to configure */
#define RCC_CLOCKTYPE_PCLK1            (0x00000004U)  /*!< PCLK1 to configure */
#define RCC_CLOCKTYPE_PCLK2            (0x00000008U)  /*!< PCLK2 to configure */
#define RCC_CLOCKTYPE_PCLK3            (0x00000010U)  /*!< PCLK3 to configure */
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source System Clock Source
  * @{
  */
#define RCC_SYSCLKSOURCE_HSI             (0x00000000U)                      /*!< HSI selection as system clock */
#define RCC_SYSCLKSOURCE_CSI             RCC_CFGR1_SW_0                     /*!< CSI selection as system clock */
#define RCC_SYSCLKSOURCE_HSE             RCC_CFGR1_SW_1                     /*!< HSE selection as system clock */
#define RCC_SYSCLKSOURCE_PLLCLK          (RCC_CFGR1_SW_0 | RCC_CFGR1_SW_1)  /*!< PLL1 selection as system clock */
/**
  * @}
  */

/** @defgroup RCC_System_Clock_Source_Status System Clock Source Status
  * @{
  */
#define RCC_SYSCLKSOURCE_STATUS_HSI      (0x00000000U)                       /*!< HSI used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_CSI      RCC_CFGR1_SWS_0                     /*!< CSI used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_HSE      RCC_CFGR1_SWS_1                     /*!< HSE used as system clock */
#define RCC_SYSCLKSOURCE_STATUS_PLLCLK   (RCC_CFGR1_SWS_0 | RCC_CFGR1_SWS_1) /*!< PLL1 used as system clock */
/**
  * @}
  */

/** @defgroup RCC_AHB_Clock_Source AHB Clock Source
  * @{
  */
#define RCC_SYSCLK_DIV1                (0x00000000U)                                                                /*!< SYSCLK not divided */
#define RCC_SYSCLK_DIV2                RCC_CFGR2_HPRE_3                                                             /*!< SYSCLK divided by 2 */
#define RCC_SYSCLK_DIV4                (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_3)                                        /*!< SYSCLK divided by 4 */
#define RCC_SYSCLK_DIV8                (RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_3)                                        /*!< SYSCLK divided by 8 */
#define RCC_SYSCLK_DIV16               (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_3)                     /*!< SYSCLK divided by 16 */
#define RCC_SYSCLK_DIV64               (RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                                        /*!< SYSCLK divided by 64 */
#define RCC_SYSCLK_DIV128              (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                     /*!< SYSCLK divided by 128 */
#define RCC_SYSCLK_DIV256              (RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                     /*!< SYSCLK divided by 256 */
#define RCC_SYSCLK_DIV512              (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_1  | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3) /*!< SYSCLK divided by 512 */
/**
  * @}
  */

/** @defgroup RCC_APB1_APB2_APB3_Clock_Source APB1 APB2 APB3 Clock Source
  * @{
  */
#define RCC_HCLK_DIV1                  (0x00000000U)                                               /*!< HCLK not divided */
#define RCC_HCLK_DIV2                  RCC_CFGR2_PPRE1_2                                           /*!< HCLK divided by 2 */
#define RCC_HCLK_DIV4                  (RCC_CFGR2_PPRE1_0 | RCC_CFGR2_PPRE1_2)                     /*!< HCLK divided by 4 */
#define RCC_HCLK_DIV8                  (RCC_CFGR2_PPRE1_1 | RCC_CFGR2_PPRE1_2)                     /*!< HCLK divided by 8 */
#define RCC_HCLK_DIV16                 (RCC_CFGR2_PPRE1_0 | RCC_CFGR2_PPRE1_1 | RCC_CFGR2_PPRE1_2) /*!< HCLK divided by 16 */
/**
  * @}
  */

/** @defgroup RCC_HAL_EC_RTC_HSE_DIV RTC HSE Prescaler
  * @{
  */
#define RCC_RTC_HSE_NOCLOCK            (0x00000000U)
#define RCC_RTC_HSE_DIV2               (0x00000200U)
#define RCC_RTC_HSE_DIV3               (0x00000300U)
#define RCC_RTC_HSE_DIV4               (0x00000400U)
#define RCC_RTC_HSE_DIV5               (0x00000500U)
#define RCC_RTC_HSE_DIV6               (0x00000600U)
#define RCC_RTC_HSE_DIV7               (0x00000700U)
#define RCC_RTC_HSE_DIV8               (0x00000800U)
#define RCC_RTC_HSE_DIV9               (0x00000900U)
#define RCC_RTC_HSE_DIV10              (0x00000A00U)
#define RCC_RTC_HSE_DIV11              (0x00000B00U)
#define RCC_RTC_HSE_DIV12              (0x00000C00U)
#define RCC_RTC_HSE_DIV13              (0x00000D00U)
#define RCC_RTC_HSE_DIV14              (0x00000E00U)
#define RCC_RTC_HSE_DIV15              (0x00000F00U)
#define RCC_RTC_HSE_DIV16              (0x00001000U)
#define RCC_RTC_HSE_DIV17              (0x00001100U)
#define RCC_RTC_HSE_DIV18              (0x00001200U)
#define RCC_RTC_HSE_DIV19              (0x00001300U)
#define RCC_RTC_HSE_DIV20              (0x00001400U)
#define RCC_RTC_HSE_DIV21              (0x00001500U)
#define RCC_RTC_HSE_DIV22              (0x00001600U)
#define RCC_RTC_HSE_DIV23              (0x00001700U)
#define RCC_RTC_HSE_DIV24              (0x00001800U)
#define RCC_RTC_HSE_DIV25              (0x00001900U)
#define RCC_RTC_HSE_DIV26              (0x00001A00U)
#define RCC_RTC_HSE_DIV27              (0x00001B00U)
#define RCC_RTC_HSE_DIV28              (0x00001C00U)
#define RCC_RTC_HSE_DIV29              (0x00001D00U)
#define RCC_RTC_HSE_DIV30              (0x00001E00U)
#define RCC_RTC_HSE_DIV31              (0x00001F00U)
#define RCC_RTC_HSE_DIV32              (0x00002000U)
#define RCC_RTC_HSE_DIV33              (0x00002100U)
#define RCC_RTC_HSE_DIV34              (0x00002200U)
#define RCC_RTC_HSE_DIV35              (0x00002300U)
#define RCC_RTC_HSE_DIV36              (0x00002400U)
#define RCC_RTC_HSE_DIV37              (0x00002500U)
#define RCC_RTC_HSE_DIV38              (0x00002600U)
#define RCC_RTC_HSE_DIV39              (0x00002700U)
#define RCC_RTC_HSE_DIV40              (0x00002800U)
#define RCC_RTC_HSE_DIV41              (0x00002900U)
#define RCC_RTC_HSE_DIV42              (0x00002A00U)
#define RCC_RTC_HSE_DIV43              (0x00002B00U)
#define RCC_RTC_HSE_DIV44              (0x00002C00U)
#define RCC_RTC_HSE_DIV45              (0x00002D00U)
#define RCC_RTC_HSE_DIV46              (0x00002E00U)
#define RCC_RTC_HSE_DIV47              (0x00002F00U)
#define RCC_RTC_HSE_DIV48              (0x00003000U)
#define RCC_RTC_HSE_DIV49              (0x00003100U)
#define RCC_RTC_HSE_DIV50              (0x00003200U)
#define RCC_RTC_HSE_DIV51              (0x00003300U)
#define RCC_RTC_HSE_DIV52              (0x00003400U)
#define RCC_RTC_HSE_DIV53              (0x00003500U)
#define RCC_RTC_HSE_DIV54              (0x00003600U)
#define RCC_RTC_HSE_DIV55              (0x00003700U)
#define RCC_RTC_HSE_DIV56              (0x00003800U)
#define RCC_RTC_HSE_DIV57              (0x00003900U)
#define RCC_RTC_HSE_DIV58              (0x00003A00U)
#define RCC_RTC_HSE_DIV59              (0x00003B00U)
#define RCC_RTC_HSE_DIV60              (0x00003C00U)
#define RCC_RTC_HSE_DIV61              (0x00003D00U)
#define RCC_RTC_HSE_DIV62              (0x00003E00U)
#define RCC_RTC_HSE_DIV63              (0x00003F00U)
/**
  * @}
  */

/** @defgroup RCC_RTC_Clock_Source RTC Clock Source
  * @{
  */
#define RCC_RTCCLKSOURCE_NO_CLK        (0x00000000U)       /*!< No clock used as RTC clock source */
#define RCC_RTCCLKSOURCE_LSE           (0x00000100U)       /*!< LSE oscillator clock used as RTC clock source */
#define RCC_RTCCLKSOURCE_LSI           (0x00000200U)       /*!< LSI oscillator clock used as RTC clock source */
#define RCC_RTCCLKSOURCE_HSE_DIVx      (0x00000300U)       /*!< HSE oscillator clock divided by X used as RTC clock source */
#define RCC_RTCCLKSOURCE_HSE_DIV2      (0x00002300U)
#define RCC_RTCCLKSOURCE_HSE_DIV3      (0x00003300U)
#define RCC_RTCCLKSOURCE_HSE_DIV4      (0x00004300U)
#define RCC_RTCCLKSOURCE_HSE_DIV5      (0x00005300U)
#define RCC_RTCCLKSOURCE_HSE_DIV6      (0x00006300U)
#define RCC_RTCCLKSOURCE_HSE_DIV7      (0x00007300U)
#define RCC_RTCCLKSOURCE_HSE_DIV8      (0x00008300U)
#define RCC_RTCCLKSOURCE_HSE_DIV9      (0x00009300U)
#define RCC_RTCCLKSOURCE_HSE_DIV10     (0x0000A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV11     (0x0000B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV12     (0x0000C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV13     (0x0000D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV14     (0x0000E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV15     (0x0000F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV16     (0x00010300U)
#define RCC_RTCCLKSOURCE_HSE_DIV17     (0x00011300U)
#define RCC_RTCCLKSOURCE_HSE_DIV18     (0x00012300U)
#define RCC_RTCCLKSOURCE_HSE_DIV19     (0x00013300U)
#define RCC_RTCCLKSOURCE_HSE_DIV20     (0x00014300U)
#define RCC_RTCCLKSOURCE_HSE_DIV21     (0x00015300U)
#define RCC_RTCCLKSOURCE_HSE_DIV22     (0x00016300U)
#define RCC_RTCCLKSOURCE_HSE_DIV23     (0x00017300U)
#define RCC_RTCCLKSOURCE_HSE_DIV24     (0x00018300U)
#define RCC_RTCCLKSOURCE_HSE_DIV25     (0x00019300U)
#define RCC_RTCCLKSOURCE_HSE_DIV26     (0x0001A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV27     (0x0001B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV28     (0x0001C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV29     (0x0001D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV30     (0x0001E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV31     (0x0001F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV32     (0x00020300U)
#define RCC_RTCCLKSOURCE_HSE_DIV33     (0x00021300U)
#define RCC_RTCCLKSOURCE_HSE_DIV34     (0x00022300U)
#define RCC_RTCCLKSOURCE_HSE_DIV35     (0x00023300U)
#define RCC_RTCCLKSOURCE_HSE_DIV36     (0x00024300U)
#define RCC_RTCCLKSOURCE_HSE_DIV37     (0x00025300U)
#define RCC_RTCCLKSOURCE_HSE_DIV38     (0x00026300U)
#define RCC_RTCCLKSOURCE_HSE_DIV39     (0x00027300U)
#define RCC_RTCCLKSOURCE_HSE_DIV40     (0x00028300U)
#define RCC_RTCCLKSOURCE_HSE_DIV41     (0x00029300U)
#define RCC_RTCCLKSOURCE_HSE_DIV42     (0x0002A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV43     (0x0002B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV44     (0x0002C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV45     (0x0002D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV46     (0x0002E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV47     (0x0002F300U)
#define RCC_RTCCLKSOURCE_HSE_DIV48     (0x00030300U)
#define RCC_RTCCLKSOURCE_HSE_DIV49     (0x00031300U)
#define RCC_RTCCLKSOURCE_HSE_DIV50     (0x00032300U)
#define RCC_RTCCLKSOURCE_HSE_DIV51     (0x00033300U)
#define RCC_RTCCLKSOURCE_HSE_DIV52     (0x00034300U)
#define RCC_RTCCLKSOURCE_HSE_DIV53     (0x00035300U)
#define RCC_RTCCLKSOURCE_HSE_DIV54     (0x00036300U)
#define RCC_RTCCLKSOURCE_HSE_DIV55     (0x00037300U)
#define RCC_RTCCLKSOURCE_HSE_DIV56     (0x00038300U)
#define RCC_RTCCLKSOURCE_HSE_DIV57     (0x00039300U)
#define RCC_RTCCLKSOURCE_HSE_DIV58     (0x0003A300U)
#define RCC_RTCCLKSOURCE_HSE_DIV59     (0x0003B300U)
#define RCC_RTCCLKSOURCE_HSE_DIV60     (0x0003C300U)
#define RCC_RTCCLKSOURCE_HSE_DIV61     (0x0003D300U)
#define RCC_RTCCLKSOURCE_HSE_DIV62     (0x0003E300U)
#define RCC_RTCCLKSOURCE_HSE_DIV63     (0x0003F300U)
/**
  * @}
  */

/** @defgroup RCC_MCO_Index MCO Index
  * @{
  */
#define RCC_MCO1                       (0x00000000U)
#define RCC_MCO2                       (0x00000001U)
/**
  * @}
  */

/** @defgroup RCC_MCO1_Clock_Source  RCC MCO1 Clock Source
  * @{
  */
#define RCC_MCO1SOURCE_HSI               (0x00000000U)
#define RCC_MCO1SOURCE_LSE               RCC_CFGR1_MCO1SEL_0
#define RCC_MCO1SOURCE_HSE               RCC_CFGR1_MCO1SEL_1
#define RCC_MCO1SOURCE_PLL1Q            ((uint32_t)RCC_CFGR1_MCO1SEL_0 | RCC_CFGR1_MCO1SEL_1)
#define RCC_MCO1SOURCE_HSI48             RCC_CFGR1_MCO1SEL_2

/**
  * @}
  */

/** @defgroup RCC_MCO2_Clock_Source  RCC MCO2 Clock Source
  * @{
  */
#define RCC_MCO2SOURCE_SYSCLK           (0x00000000U)
#define RCC_MCO2SOURCE_PLL2P            RCC_CFGR1_MCO2SEL_0
#define RCC_MCO2SOURCE_HSE              RCC_CFGR1_MCO2SEL_1
#define RCC_MCO2SOURCE_PLL1P           ((uint32_t)RCC_CFGR1_MCO2SEL_0 | RCC_CFGR1_MCO2SEL_1)
#define RCC_MCO2SOURCE_CSI              RCC_CFGR1_MCO2SEL_2
#define RCC_MCO2SOURCE_LSI             ((uint32_t)RCC_CFGR1_MCO2SEL_0 | RCC_CFGR1_MCO2SEL_2)

/**
  * @}
  */

/** @defgroup RCC_MCOx_Clock_Prescaler MCOx Clock Prescaler
  * @{
  */
#define RCC_MCODIV_1                    RCC_CFGR1_MCO1PRE_0
#define RCC_MCODIV_2                    RCC_CFGR1_MCO1PRE_1
#define RCC_MCODIV_3                    ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1)
#define RCC_MCODIV_4                    RCC_CFGR1_MCO1PRE_2
#define RCC_MCODIV_5                    ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_2)
#define RCC_MCODIV_6                    ((uint32_t)RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2)
#define RCC_MCODIV_7                    ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2)
#define RCC_MCODIV_8                    RCC_CFGR1_MCO1PRE_3
#define RCC_MCODIV_9                    ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_10                   ((uint32_t)RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_11                   ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_12                   ((uint32_t)RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_13                   ((uint32_t)RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_14                   ((uint32_t)RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define RCC_MCODIV_15                   RCC_CFGR1_MCO1PRE
/**
  * @}
  */

/** @defgroup RCC_Interrupt Interrupts
  * @{
  */
#define RCC_IT_LSIRDY                  RCC_CIFR_LSIRDYF      /*!< LSI Ready Interrupt flag */
#define RCC_IT_LSERDY                  RCC_CIFR_LSERDYF      /*!< LSE Ready Interrupt flag */
#define RCC_IT_CSIRDY                  RCC_CIFR_CSIRDYF      /*!< CSI Ready Interrupt flag */
#define RCC_IT_HSIRDY                  RCC_CIFR_HSIRDYF      /*!< HSI16 Ready Interrupt flag */
#define RCC_IT_HSERDY                  RCC_CIFR_HSERDYF      /*!< HSE Ready Interrupt flag */
#define RCC_IT_HSI48RDY                RCC_CIFR_HSI48RDYF    /*!< HSI48 Ready Interrupt flag */
#define RCC_IT_PLL1RDY                 RCC_CIFR_PLL1RDYF     /*!< PLL1 Ready Interrupt flag */
#define RCC_IT_PLL2RDY                 RCC_CIFR_PLL2RDYF     /*!< PLL2 Ready Interrupt flag */
#if defined(RCC_CR_PLL3ON)
#define RCC_IT_PLL3RDY                 RCC_CIFR_PLL3RDYF     /*!< PLL3 Ready Interrupt flag */
#endif /* RCC_CR_PLL3ON */
#define RCC_IT_HSECSS                  RCC_CIFR_HSECSSF      /*!< HSE Clock Security System Interrupt flag */

/**
  * @}
  */

/** @defgroup RCC_Flag Flags
  *        Elements values convention: XXXYYYYYb
  *           - YYYYY  : Flag position in the register
  *           - XXX  : Register index
  *                 - 001: CR register
  *                 - 010: BDCR register
  *                 - 011: RSR register
  * @{
  */
/* Flags in the CR register */
#define RCC_FLAG_CSIRDY                ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_CSIRDY_Pos))   /*!< CSI Ready flag */
#define RCC_FLAG_HSIRDY                ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_HSIRDY_Pos))   /*!< HSI Ready flag */
#define RCC_FLAG_HSIDIVF               ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_HSIDIVF_Pos))  /*!< HSI divider flag */
#define RCC_FLAG_HSERDY                ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_HSERDY_Pos))   /*!< HSE Ready flag */
#define RCC_FLAG_PLL1RDY               ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_PLL1RDY_Pos))  /*!< PLL1 Ready flag */
#define RCC_FLAG_PLL2RDY               ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_PLL2RDY_Pos))  /*!< PLL2 Ready flag */
#if defined(RCC_CR_PLL3ON)
#define RCC_FLAG_PLL3RDY               ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_PLL3RDY_Pos))  /*!< PLL3 Ready flag */
#endif /* RCC_CR_PLL3ON */
#define RCC_FLAG_HSI48RDY              ((uint32_t)((RCC_CR_REG_INDEX << 5U) | RCC_CR_HSI48RDY_Pos)) /*!< HSI48 Ready flag */

/* Flags in the BDCR register */
#define RCC_FLAG_LSERDY                ((uint32_t)((RCC_BDCR_REG_INDEX << 5U) | RCC_BDCR_LSERDY_Pos))  /*!< LSE Ready flag */
#define RCC_FLAG_LSECSSD               ((uint32_t)((RCC_BDCR_REG_INDEX << 5U) | RCC_BDCR_LSECSSD_Pos)) /*!< LSE Clock Security System Interrupt flag */
#define RCC_FLAG_LSIRDY                ((uint32_t)((RCC_BDCR_REG_INDEX << 5U) | RCC_BDCR_LSIRDY_Pos))  /*!< LSI Ready flag */

/* Flags in the RSR register */
#define RCC_FLAG_RMVF                  ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_RMVF_Pos))     /*!< Remove reset flag */
#define RCC_FLAG_PINRST                ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_PINRSTF_Pos))  /*!< PIN reset flag */
#define RCC_FLAG_BORRST                ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_BORRSTF_Pos))  /*!< BOR reset flag */
#define RCC_FLAG_SFTRST                ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_SFTRSTF_Pos))  /*!< Software Reset flag */
#define RCC_FLAG_IWDGRST               ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_IWDGRSTF_Pos)) /*!< Independent Watchdog reset flag */
#define RCC_FLAG_WWDGRST               ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_WWDGRSTF_Pos)) /*!< Window watchdog reset flag */
#define RCC_FLAG_LPWRRST               ((uint32_t)((RCC_RSR_REG_INDEX << 5U) | RCC_RSR_LPWRRSTF_Pos)) /*!< Low-Power reset flag */

/**
  * @}
  */

/** @defgroup RCC_Reset_Flag Reset Flag
  * @{
  */
#define RCC_RESET_FLAG_PIN             RCC_RSR_PINRSTF    /*!< PIN reset flag */
#define RCC_RESET_FLAG_PWR             RCC_RSR_BORRSTF    /*!< BOR or POR/PDR reset flag */
#define RCC_RESET_FLAG_SW              RCC_RSR_SFTRSTF    /*!< Software Reset flag */
#define RCC_RESET_FLAG_IWDG            RCC_RSR_IWDGRSTF   /*!< Independent Watchdog reset flag */
#define RCC_RESET_FLAG_WWDG            RCC_RSR_WWDGRSTF   /*!< Window watchdog reset flag */
#define RCC_RESET_FLAG_LPWR            RCC_RSR_LPWRRSTF   /*!< Low power reset flag */
#define RCC_RESET_FLAG_ALL             (RCC_RESET_FLAG_PIN | RCC_RESET_FLAG_PWR | RCC_RESET_FLAG_SW | \
                                        RCC_RESET_FLAG_IWDG | RCC_RESET_FLAG_WWDG | RCC_RESET_FLAG_LPWR)
/**
  * @}
  */

/** @defgroup RCC_LSEDrive_Config LSE Drive Config
  * @{
  */
#if !defined(STM32H5E5xx) && !defined(STM32H5E4xx) && !defined(STM32H5F5xx) && !defined(STM32H5F4xx)
#define RCC_LSEDRIVE_LOW                 (0x00000000U)          /*!< LSE low drive capability */
#endif /* !STM32H5E5xx && !STM32H5E4xx && !STM32H5F5xx  && !STM32H5F4xx */
#define RCC_LSEDRIVE_MEDIUMLOW           RCC_BDCR_LSEDRV_0      /*!< LSE medium low drive capability */
#define RCC_LSEDRIVE_MEDIUMHIGH          RCC_BDCR_LSEDRV_1      /*!< LSE medium high drive capability */
#define RCC_LSEDRIVE_HIGH                RCC_BDCR_LSEDRV        /*!< LSE high drive capability */
/**
  * @}
  */

/** @defgroup RCC_Stop_WakeUpClock Wake-Up from STOP Clock
  * @{
  */
#define RCC_STOP_WAKEUPCLOCK_HSI         (0x00000000U)            /*!< HSI selection after wake-up from STOP */
#define RCC_STOP_WAKEUPCLOCK_CSI         RCC_CFGR1_STOPWUCK       /*!< CSI selection after wake-up from STOP */
/**
  * @}
  */

/** @defgroup RCC_Stop_KernelWakeUpClock  RCC Stop KernelWakeUpClock
  * @{
  */
#define RCC_STOP_KERWAKEUPCLOCK_HSI      (0x00000000U)            /*!< HSI kernel clock selection after wake-up from STOP */
#define RCC_STOP_KERWAKEUPCLOCK_CSI       RCC_CFGR1_STOPKERWUCK   /*!< CSI kernel clock selection after wake-up from STOP */

/**
  * @}
  */

#if defined(RCC_SECCFGR_HSISEC)
/** @defgroup RCC_items RCC items
  * @brief RCC items to configure attributes on
  * @{
  */
#define RCC_HSI                        RCC_SECCFGR_HSISEC
#define RCC_HSE                        RCC_SECCFGR_HSESEC
#define RCC_CSI                        RCC_SECCFGR_CSISEC
#define RCC_LSI                        RCC_SECCFGR_LSISEC
#define RCC_LSE                        RCC_SECCFGR_LSESEC
#define RCC_SYSCLK                     RCC_SECCFGR_SYSCLKSEC
#define RCC_PRESC                      RCC_SECCFGR_PRESCSEC
#define RCC_PLL1                       RCC_SECCFGR_PLL1SEC
#define RCC_PLL2                       RCC_SECCFGR_PLL2SEC
#if defined(RCC_CR_PLL3ON)
#define RCC_PLL3                       RCC_SECCFGR_PLL3SEC
#endif /* RCC_CR_PLL3ON */
#define RCC_HSI48                      RCC_SECCFGR_HSI48SEC
#define RCC_RMVF                       RCC_SECCFGR_RMVFSEC
#define RCC_CKPERSEL                   RCC_SECCFGR_CKPERSELSEC
#if defined(RCC_CR_PLL3ON)
#define RCC_ALL                        (RCC_HSI|RCC_HSE|RCC_CSI|RCC_LSI|RCC_LSE|RCC_HSI48| \
                                        RCC_SYSCLK|RCC_PRESC|RCC_PLL1|RCC_PLL2| \
                                        RCC_PLL3|RCC_CKPERSEL|RCC_RMVF)
#else
#define RCC_ALL                        (RCC_HSI|RCC_HSE|RCC_CSI|RCC_LSI|RCC_LSE|RCC_HSI48| \
                                        RCC_SYSCLK|RCC_PRESC|RCC_PLL1|RCC_PLL2| \
                                        RCC_CKPERSEL|RCC_RMVF)
#endif /* RCC_CR_PLL3ON */
/**
  * @}
  */
#endif /* RCC_SECCFGR_HSISEC */

/** @defgroup RCC_attributes RCC attributes
  * @brief RCC privilege/non-privilege and secure/non-secure attributes
  * @{
  */
#if defined(RCC_PRIVCFGR_NSPRIV)
#define RCC_NSEC_PRIV                  0x00000001U        /*!< Non-secure Privilege attribute item     */
#define RCC_NSEC_NPRIV                 0x00000002U        /*!< Non-secure Non-privilege attribute item */
#else
#define RCC_PRIV                       0x00000001U        /*!< Privilege attribute item     */
#define RCC_NPRIV                      0x00000002U        /*!< Non-privilege attribute item */
#endif /* RCC_PRIVCFGR_NSPRIV */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define RCC_SEC_PRIV                   0x00000010U        /*!< Secure Privilege attribute item         */
#define RCC_SEC_NPRIV                  0x00000020U        /*!< Secure Non-privilege attribute item     */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup RCC_Exported_Macros RCC Exported Macros
  * @{
  */

/** @defgroup RCC_AHB1_Peripheral_Clock_Enable_Disable AHB1 Peripheral Clock Enable Disable
  * @brief  Enable or disable the AHB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_GPDMA1_CLK_ENABLE()            do { \
                                                      __IO uint32_t tmpreg; \
                                                      SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA1EN); \
                                                      /* Delay after an RCC peripheral clock enabling */ \
                                                      tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA1EN); \
                                                      UNUSED(tmpreg); \
                                                    } while(0)

#define __HAL_RCC_GPDMA2_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CORDICEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CORDICEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FMACEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FMACEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* FMAC */

#if defined(MDF1)
#define __HAL_RCC_MDF1_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_MDF1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_MDF1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* MDF1 */

#define __HAL_RCC_CRC_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_RAMCFG_CLK_ENABLE()          do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_RAMCFGEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_RAMCFGEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_FLASH_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FLITFEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FLITFEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(ETH)
#define __HAL_RCC_ETH_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_ETHTX_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHTXEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHTXEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_ETHRX_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHRXEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHRXEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#if defined(RCC_AHB1ENR_ETHCKEN)
#define __HAL_RCC_ETHINTERN_CLK_ENABLE()       do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHCKEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHCKEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* RCC_AHB1ENR_ETHCKEN */
#endif /*ETH*/

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_JPEGEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_JPEGEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_TZSC1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_TZSC1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_BKPRAM_CLK_ENABLE()          do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_BKPRAMEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_BKPRAMEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_CLK_ENABLE()         do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DCACHE1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DCACHE1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_SRAM1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_SRAM1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_GPDMA1_CLK_DISABLE()         CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA1EN)

#define __HAL_RCC_GPDMA2_CLK_DISABLE()         CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA2EN)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_CLK_DISABLE()         CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CORDICEN)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_CLK_DISABLE()           CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FMACEN)
#endif /* FMAC */

#define __HAL_RCC_FLASH_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FLITFEN)

#define __HAL_RCC_CRC_CLK_DISABLE()            CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN)

#if defined(MDF1)
#define __HAL_RCC_MDF1_CLK_DISABLE()           CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_MDF1EN)
#endif /* MDF1 */

#define __HAL_RCC_RAMCFG_CLK_DISABLE()         CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_RAMCFGEN)

#if defined(ETH)
#define __HAL_RCC_ETH_CLK_DISABLE()            CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHEN)

#define __HAL_RCC_ETHTX_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHTXEN)

#define __HAL_RCC_ETHRX_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHRXEN)

#if defined(RCC_AHB1ENR_ETHCKEN)
#define __HAL_RCC_ETHINTERN_CLK_DISABLE()      CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHCKEN)
#endif /* RCC_AHB1ENR_ETHCKEN */
#endif /*ETH*/

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_CLK_DISABLE()           CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_JPEGEN)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_TZSC1EN)

#define __HAL_RCC_BKPRAM_CLK_DISABLE()         CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_BKPRAMEN)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_CLK_DISABLE()        CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DCACHE1EN)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_CLK_DISABLE()          CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_SRAM1EN)
/**
  * @}
  */

/** @defgroup RCC_AHB2_Peripheral_Clock_Enable_Disable AHB2 Peripheral Clock Enable Disable
  * @brief  Enable or disable the AHB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_GPIOA_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_GPIOB_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_GPIOC_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_GPIOD_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOIEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOIEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOJEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOJEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_DAC1_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DAC1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DAC1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_CLK_ENABLE()       do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMI_PSSIEN);\
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMI_PSSIEN);\
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#define __HAL_RCC_DCMI_CLK_ENABLE()           __HAL_RCC_DCMI_PSSI_CLK_ENABLE()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(RCC_AHB2ENR_OTGPHYEN)
#define __HAL_RCC_OTGPHY_CLK_ENABLE()          do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGPHYEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGPHYEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* RCC_AHB2ENR_OTGPHYEN */

#if defined(AES)
#define __HAL_RCC_AES_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_CLK_ENABLE()             do { \
                                                      __IO uint32_t tmpreg; \
                                                      SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN); \
                                                      /* Delay after an RCC peripheral clock enabling */ \
                                                      tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN); \
                                                      UNUSED(tmpreg); \
                                                    } while(0)
#endif /* HASH */

#define __HAL_RCC_RNG_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_RNGEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_RNGEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(PKA)
#define __HAL_RCC_PKA_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_PKAEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_PKAEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* PKA */

#if defined(SAES)
#define __HAL_RCC_SAES_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SAESEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SAESEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* SAES */

#if defined(CCB)
#define __HAL_RCC_CCB_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CCBEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CCBEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC3EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC3EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_CLK_ENABLE()            do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADF1EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADF1EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOKEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOKEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* GPIOK */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_CLK_ENABLE()      do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGHSEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGHSEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_CLK_ENABLE()      do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* USB_OTG_FS */

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM4EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM4EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* SRAM4_BASE */

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM5EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM5EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* SRAM5_BASE */

#define __HAL_RCC_SRAM2_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM2EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM2EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_CLK_ENABLE()           do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM3EN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM3EN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)
#endif /* SRAM3_BASE */

#define __HAL_RCC_GPIOA_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN)

#define __HAL_RCC_GPIOB_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN)

#define __HAL_RCC_GPIOC_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN)

#define __HAL_RCC_GPIOD_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOIEN)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOJEN)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN)

#define __HAL_RCC_DAC1_CLK_DISABLE()           CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DAC1EN)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_CLK_DISABLE()      CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMI_PSSIEN)
#define __HAL_RCC_DCMI_CLK_DISABLE()           __HAL_RCC_DCMI_PSSI_CLK_DISABLE()  /* for API backward compatibility*/
#endif /* DCMI */

#if defined(RCC_AHB2ENR_OTGPHYEN)
#define __HAL_RCC_OTGPHY_CLK_DISABLE()         CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGPHYEN);
#endif /* RCC_AHB2ENR_OTGPHYEN */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_CLK_DISABLE()     CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGHSEN);
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_CLK_DISABLE()     CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN);
#endif /* USB_OTG_FS */

#if defined(AES)
#define __HAL_RCC_AES_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN);
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_CLK_DISABLE()           CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN)
#endif /* HASH */

#define __HAL_RCC_RNG_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_RNGEN)

#if defined(PKA)
#define __HAL_RCC_PKA_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_PKAEN)
#endif /* PKA */

#if defined(SAES)
#define __HAL_RCC_SAES_CLK_DISABLE()           CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SAESEN)
#endif /* SAES */

#if defined(CCB)
#define __HAL_RCC_CCB_CLK_DISABLE()            CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CCBEN)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_CLK_DISABLE()           CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC3EN)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_CLK_DISABLE()           CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADF1EN)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOKEN)
#endif /* GPIOK */

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM4EN)
#endif /* SRAM4_BASE */

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM5EN)
#endif /* SRAM5_BASE */

#define __HAL_RCC_SRAM2_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM2EN)

#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_CLK_DISABLE()          CLEAR_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM3EN)
#endif /* SRAM3_BASE */
/**
  * @}
  */

/** @defgroup RCC_AHB4_Clock_Enable_Disable AHB4 Peripheral Clock Enable Disable
  * @brief  Enable or disable the AHB4 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_CLK_ENABLE()             do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC1EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC1EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_CLK_ENABLE()             do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC2EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC2EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_CLK_ENABLE()              do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC1EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC1EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_CLK_ENABLE()              do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC2EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC2EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_CLK_ENABLE()                 do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_FMCEN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_FMCEN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_CLK_ENABLE()               do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI1EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI1EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_CLK_ENABLE()               do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI2EN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI2EN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_CLK_ENABLE()               do { \
                                                        __IO uint32_t tmpreg; \
                                                        SET_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPIMEN); \
                                                        /* Delay after an RCC peripheral clock enabling */ \
                                                        tmpreg = READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPIMEN); \
                                                        UNUSED(tmpreg); \
                                                      } while(0)
#endif /* OCTOSPIM */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_CLK_DISABLE()        CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC1EN)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_CLK_DISABLE()        CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC2EN)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_CLK_DISABLE()         CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC1EN)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_CLK_DISABLE()         CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC2EN)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_CLK_DISABLE()            CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_FMCEN)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_CLK_DISABLE()          CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI1EN)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_CLK_DISABLE()          CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI2EN)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_CLK_DISABLE()          CLEAR_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPIMEN)
#endif /* OCTOSPIM */
/**
  * @}
  */


/** @defgroup RCC_APB1_Clock_Enable_Disable APB1 Peripheral Clock Enable Disable
  * @brief  Enable or disable the APB1 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_TIM2_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_TIM3_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(TIM4)
#define __HAL_RCC_TIM4_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM6EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM6EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_TIM7_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM7EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM7EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(TIM12)
#define __HAL_RCC_TIM12_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM12EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM12EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM13EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM13EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM14EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM14EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_WWDGEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_WWDGEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_OPAMPEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_OPAMPEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_SPI3_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(COMP1)
#define __HAL_RCC_COMP_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_COMPEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_COMPEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* COMP1 */

#define __HAL_RCC_USART2_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_USART3_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART3EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART3EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(UART4)
#define __HAL_RCC_UART4_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_UART4EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART4EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART5 */

#define  __HAL_RCC_I2C1_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define  __HAL_RCC_I2C2_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define  __HAL_RCC_I3C1_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_I3C1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I3C1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define   __HAL_RCC_CRS_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_CRSEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CRSEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(USART6)
#define __HAL_RCC_USART6_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART6EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART6EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_CLK_ENABLE()          do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART10EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART10EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_CLK_ENABLE()          do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_USART11EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART11EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_CLK_ENABLE()              do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_CECEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CECEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_UART7EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART7EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1LENR, RCC_APB1LENR_UART8EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART8EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART8 */

#if defined(UART9)
#define __HAL_RCC_UART9_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_UART9EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART9EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_UART12EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART12EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UART12 */

#define __HAL_RCC_DTS_CLK_ENABLE()              do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_DTSEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_DTSEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_LPTIM2_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_LPTIM2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_LPTIM2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_FDCAN_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_FDCANEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_FDCANEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB1HENR, RCC_APB1HENR_UCPD1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UCPD1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* UCPD1 */

#define __HAL_RCC_TIM2_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN)

#define __HAL_RCC_TIM3_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN)

#if defined(TIM4)
#define __HAL_RCC_TIM4_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM6EN)

#define __HAL_RCC_TIM7_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM7EN)

#if defined(TIM12)
#define __HAL_RCC_TIM12_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM12EN)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM13EN)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM14EN)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_WWDGEN)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_OPAMPEN)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI2EN)

#define __HAL_RCC_SPI3_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN)

#if defined(COMP1)
#define __HAL_RCC_COMP_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_COMPEN)
#endif /* COMP1 */

#define __HAL_RCC_USART2_CLK_DISABLE()      CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN)

#define __HAL_RCC_USART3_CLK_DISABLE()      CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_USART3EN)

#if defined(UART4)
#define __HAL_RCC_UART4_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_UART4EN)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN)
#endif /* UART5 */

#define __HAL_RCC_I2C1_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN)

#define __HAL_RCC_I2C2_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C2EN)

#define __HAL_RCC_I3C1_CLK_DISABLE()        CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_I3C1EN)

#define __HAL_RCC_CRS_CLK_DISABLE()         CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_CRSEN)

#if defined(USART6)
#define __HAL_RCC_USART6_CLK_DISABLE()      CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_USART6EN)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_CLK_DISABLE()     CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_USART10EN)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_CLK_DISABLE()     CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_USART11EN)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_CLK_DISABLE()         CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_CECEN)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_UART7EN)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_CLK_DISABLE()       CLEAR_BIT(RCC->APB1LENR, RCC_APB1LENR_UART8EN)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_CLK_DISABLE()       CLEAR_BIT(RCC->APB1HENR, RCC_APB1HENR_UART9EN)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_CLK_DISABLE()      CLEAR_BIT(RCC->APB1HENR, RCC_APB1HENR_UART12EN)
#endif /* UART12 */

#define __HAL_RCC_DTS_CLK_DISABLE()         CLEAR_BIT(RCC->APB1HENR , RCC_APB1HENR_DTSEN)

#define __HAL_RCC_LPTIM2_CLK_DISABLE()      CLEAR_BIT(RCC->APB1HENR, RCC_APB1HENR_LPTIM2EN)

#define __HAL_RCC_FDCAN_CLK_DISABLE()       CLEAR_BIT(RCC->APB1HENR, RCC_APB1HENR_FDCANEN)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_CLK_DISABLE()       CLEAR_BIT(RCC->APB1HENR, RCC_APB1HENR_UCPD1EN)
#endif /* UCPD1 */

/**
  * @}
  */

/** @defgroup RCC_APB2_Clock_Enable_Disable APB2 Peripheral Clock Enable Disable
  * @brief  Enable or disable the APB2 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */
#define __HAL_RCC_TIM1_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_SPI1_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(TIM8)
#define __HAL_RCC_TIM8_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM8 */

#define __HAL_RCC_USART1_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(TIM15)
#define __HAL_RCC_TIM15_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM17EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM17EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* SAI2 */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_CLK_ENABLE()              do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USBEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USBEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /*USB_DRD_FS*/

#if defined(LTDC)
#define __HAL_RCC_LTDC_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_LTDCEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_LTDCEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /*LTDC*/

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB2ENR, RCC_APB2ENR_GFXTIMEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_GFXTIMEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /*GFXTIM*/

#define __HAL_RCC_TIM1_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN)

#define __HAL_RCC_SPI1_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN)

#if defined(TIM8)
#define __HAL_RCC_TIM8_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN)
#endif /* TIM8 */

#define __HAL_RCC_USART1_CLK_DISABLE()         CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN)

#if defined(TIM15)
#define __HAL_RCC_TIM15_CLK_DISABLE()          CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_CLK_DISABLE()          CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_CLK_DISABLE()          CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM17EN)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN)
#endif /* SAI2 */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_CLK_DISABLE()            CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USBEN)
#endif /* USB_DRD_FS */

#if defined(LTDC)
#define __HAL_RCC_LTDC_CLK_DISABLE()           CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_LTDCEN)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_CLK_DISABLE()         CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_GFXTIMEN)
#endif /* GFXTIM */
/**
  * @}
  */

/** @defgroup RCC_APB3_Clock_Enable_Disable APB3 Peripheral Clock Enable Disable
  * @brief  Enable or disable the APB3 peripheral clock.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */
#define __HAL_RCC_SBS_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->APB3ENR, RCC_APB3ENR_SBSEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SBSEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined(SPI5)
#define __HAL_RCC_SPI5_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_SPI5EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SPI5EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_CLK_ENABLE()          do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPUART1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPUART1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(I2C3)
#define __HAL_RCC_I2C3_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C3EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C3EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C4EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C4EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_I3C2EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I3C2EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM3EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM3EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM4EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM4EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM5EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM5EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_CLK_ENABLE()           do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM6EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM6EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_CLK_ENABLE()             do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_VREFEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_VREFEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_CLK_ENABLE()             do { \
                                                    __IO uint32_t tmpreg; \
                                                    SET_BIT(RCC->APB3ENR, RCC_APB3ENR_RTCAPBEN); \
                                                    /* Delay after an RCC peripheral clock enabling */ \
                                                    tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_RTCAPBEN); \
                                                    UNUSED(tmpreg); \
                                                  } while(0)

#if defined (PLAY1)
#define __HAL_RCC_PLAY1_CLK_ENABLE()            do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1EN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1EN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)

#define __HAL_RCC_PLAY1APB_CLK_ENABLE()         do { \
                                                     __IO uint32_t tmpreg; \
                                                     SET_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1APBEN); \
                                                     /* Delay after an RCC peripheral clock enabling */ \
                                                     tmpreg = READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1APBEN); \
                                                     UNUSED(tmpreg); \
                                                   } while(0)
#endif /* PLAY1 */

#define __HAL_RCC_SBS_CLK_DISABLE()            CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_SBSEN)

#if defined(SPI5)
#define __HAL_RCC_SPI5_CLK_DISABLE()           CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_SPI5EN)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_CLK_DISABLE()        CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPUART1EN)

#if defined(I2C3)
#define __HAL_RCC_I2C3_CLK_DISABLE()           CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C3EN)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_CLK_DISABLE()           CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C4EN)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_CLK_DISABLE()           CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_I3C2EN)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_CLK_DISABLE()         CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM1EN)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_CLK_DISABLE()         CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM3EN)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_CLK_DISABLE()         CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM4EN)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_CLK_DISABLE()         CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM5EN)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_CLK_DISABLE()         CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM6EN)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_CLK_DISABLE()           CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_VREFEN)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_CLK_DISABLE()            CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_RTCAPBEN)

#if defined (PLAY1)
#define __HAL_RCC_PLAY1_CLK_DISABLE()          CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1EN)
#define __HAL_RCC_PLAY1APB_CLK_DISABLE()       CLEAR_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1APBEN)
#endif /* PLAY1 */
/**
  * @}
  */

/** @defgroup RCC_AHB_APB_Branch_Clock_Disable AHB APB Branch Clock Disable Clear Disable
  * @brief  Disable or clear Disable the AHBx/APBx branch clock for all AHBx/APBx peripherals.
  * @note   It is recommended to disable the clock of all peripherals (by writing 0 in
  *         the AHBxENR/APBxENR register) before Disabling the corresponding Bus Branch clock.
  *         Some peripheral bus clocks are not affected by branch clock disabling as IWDG (APB1),
  *         SRAM2/SRAM3 (AHB2) and FLITF/BKRAM/ICACHE/DCACHE/SRAM1 (AHB1).
  * @{
  */

#define __HAL_RCC_AHB1_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_AHB1DIS); \
                                                  /* Delay after AHB peripherals bus clocks branch disable */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB1DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)

#define __HAL_RCC_AHB2_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_AHB2DIS); \
                                                  /* Delay after AHB peripherals bus clocks branch disable */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB2DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)

#if defined(AHB4PERIPH_BASE)
#define __HAL_RCC_AHB4_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_AHB4DIS); \
                                                  /* Delay after AHB peripherals bus clocks branch disable  */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB4DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)
#endif /* AHB4PERIPH_BASE */

#define __HAL_RCC_APB1_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_APB1DIS); \
                                                  /* Delay after APB peripherals bus clocks branch disable */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_APB1DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)

#define __HAL_RCC_APB2_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_APB2DIS); \
                                                  /* Delay after APB peripherals bus clocks branch disable  */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_APB2DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)

#define __HAL_RCC_APB3_CLK_DISABLE()         do { \
                                                  __IO uint32_t tmpreg; \
                                                  SET_BIT(RCC->CFGR2, RCC_CFGR2_APB3DIS); \
                                                  /* Delay after APB peripherals bus clocks branch disable  */ \
                                                  tmpreg = READ_BIT(RCC->CFGR2, RCC_CFGR2_APB3DIS); \
                                                  UNUSED(tmpreg); \
                                                } while(0)


#define __HAL_RCC_AHB1_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_AHB1DIS)

#define __HAL_RCC_AHB2_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_AHB2DIS)

#if defined(AHB4PERIPH_BASE)
#define __HAL_RCC_AHB4_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_AHB4DIS)
#endif /* AHB4PERIPH_BASE */

#define __HAL_RCC_APB1_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_APB1DIS)

#define __HAL_RCC_APB2_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_APB2DIS)

#define __HAL_RCC_APB3_CLK_ENABLE()             CLEAR_BIT(RCC->CFGR2, RCC_CFGR2_APB3DIS)

/**
  * @}
  */

/** @defgroup RCC_AHB1_Peripheral_Clock_Enable_Disable_Status AHB1 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the AHB1 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */
#define __HAL_RCC_GPDMA1_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA1EN) != 0U)

#define __HAL_RCC_GPDMA2_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA2EN) != 0U)

#define __HAL_RCC_FLASH_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FLITFEN) != 0U)

#define __HAL_RCC_CRC_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN) != 0U)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CORDICEN) != 0U)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FMACEN) != 0U)
#endif /* FMAC */

#if defined(MDF1)
#define __HAL_RCC_MDF1_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_MDF1EN) != 0U)
#endif /* MDF1 */

#define __HAL_RCC_RAMCFG_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_RAMCFGEN) != 0U)

#if defined(ETH)
#define __HAL_RCC_ETH_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHEN) != 0U)

#define __HAL_RCC_ETHTX_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHTXEN) != 0U)

#define __HAL_RCC_ETHRX_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHRXEN) != 0U)

#if defined(RCC_AHB1ENR_ETHCKEN)
#define __HAL_RCC_ETHINTERN_IS_CLK_ENABLED()    (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHCKEN) != 0U)
#endif /* RCC_AHB1ENR_ETHCKEN */
#endif /*ETH*/

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN) != 0U)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_JPEGEN) != 0U)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_TZSC1EN) != 0U)

#define __HAL_RCC_BKPRAM_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_BKPRAMEN) != 0U)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_IS_CLK_ENABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DCACHE1EN) != 0U)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_SRAM1EN) != 0U)


#define __HAL_RCC_GPDMA1_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA1EN) == 0U)

#define __HAL_RCC_GPDMA2_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPDMA2EN) == 0U)

#define __HAL_RCC_FLASH_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FLITFEN) == 0U)

#define __HAL_RCC_CRC_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CRCEN) == 0U)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_CORDICEN) == 0U)
#endif /* CORDIC */

#if defined(MDF1)
#define __HAL_RCC_MDF1_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_MDF1EN) == 0U)
#endif /* MDF1 */

#if defined(FMAC)
#define __HAL_RCC_FMAC_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_FMACEN) == 0U)
#endif /* FMAC */

#define __HAL_RCC_RAMCFG_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_RAMCFGEN) == 0U)

#if defined(ETH)
#define __HAL_RCC_ETH_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHEN) == 0U)

#define __HAL_RCC_ETHTX_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHTXEN) == 0U)

#define __HAL_RCC_ETHRX_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHRXEN) == 0U)

#if defined(RCC_AHB1ENR_ETHCKEN)
#define __HAL_RCC_ETHINTERN_IS_CLK_DISABLED()   (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_ETHCKEN) == 0U)
#endif /* RCC_AHB1ENR_ETHCKEN */
#endif /*ETH*/

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2DEN) == 0U)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_JPEGEN) == 0U)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_TZSC1EN) == 0U)

#define __HAL_RCC_BKPRAM_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_BKPRAMEN) == 0U)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_IS_CLK_DISABLED()     (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DCACHE1EN) == 0U)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_SRAM1EN) == 0U)
/**
  * @}
  */

/** @defgroup RCC_AHB2_Peripheral_Clock_Enable_Disable_Status AHB2 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the AHB2 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_GPIOA_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN) != 0U)

#define __HAL_RCC_GPIOB_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN) != 0U)

#define __HAL_RCC_GPIOC_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN) != 0U)

#define __HAL_RCC_GPIOD_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN) != 0U)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN) != 0U)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN) != 0U)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN) != 0U)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN) != 0U)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOIEN) != 0U)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOJEN) != 0U)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN) != 0U)

#define __HAL_RCC_DAC1_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DAC1EN) != 0U)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_IS_CLK_ENABLED()    (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMI_PSSIEN) != 0U)
#define __HAL_RCC_DCMI_IS_CLK_ENABLED()      __HAL_RCC_DCMI_PSSI_IS_CLK_ENABLED()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(RCC_AHB2ENR_OTGPHYEN)
#define __HAL_RCC_OTGPHY_IS_CLK_ENABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGPHYEN) != 0U)
#endif /* RCC_AHB2ENR_OTGPHYEN */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_IS_CLK_ENABLED()   (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGHSEN) != 0U)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_IS_CLK_ENABLED()   (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN) != 0U)
#endif /* USB_OTG_FS */

#if defined(AES)
#define __HAL_RCC_AES_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN) != 0U)
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN) != 0U)
#endif /* HASH */

#define __HAL_RCC_RNG_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_RNGEN) != 0U)

#define __HAL_RCC_PKA_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_PKAEN) != 0U)

#if defined(SAES)
#define __HAL_RCC_SAES_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SAESEN) != 0U)
#endif /*SAES*/

#if defined(CCB)
#define __HAL_RCC_CCB_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CCBEN) != 0U)
#endif /*CCB*/

#if defined(ADC3)
#define __HAL_RCC_ADC3_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC3EN) != 0U)
#endif /*ADC3*/

#if defined(ADF1)
#define __HAL_RCC_ADF1_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADF1EN) != 0U)
#endif /*ADF1*/

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOKEN) != 0U)
#endif /*GPIOK*/

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM4EN) != 0U)
#endif /*SRAM4_BASE*/

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM5EN) != 0U)
#endif /*SRAM5_BASE*/

#define __HAL_RCC_SRAM2_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM2EN) != 0U)

#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM3EN) != 0U)
#endif /* SRAM3_BASE */

#define __HAL_RCC_GPIOA_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOAEN) == 0U)

#define __HAL_RCC_GPIOB_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN) == 0U)

#define __HAL_RCC_GPIOC_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOCEN) == 0U)

#define __HAL_RCC_GPIOD_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIODEN) == 0U)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN) == 0U)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOFEN) == 0U)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN) == 0U)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOHEN) == 0U)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOIEN) == 0U)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOJEN) == 0U)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADCEN) == 0U)

#define __HAL_RCC_DAC1_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DAC1EN) == 0U)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_IS_CLK_DISABLED()   (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMI_PSSIEN) == 0U)
#define __HAL_RCC_DCMI_IS_CLK_DISABLED()     __HAL_RCC_DCMI_PSSI_IS_CLK_DISABLED()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(RCC_AHB2ENR_OTGPHYEN)
#define __HAL_RCC_OTGPHY_IS_CLK_DISABLED()      (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGPHYEN) == 0U)
#endif /* RCC_AHB2ENR_OTGPHYEN */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_IS_CLK_DISABLED()  (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGHSEN) == 0U)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_IS_CLK_DISABLED()  (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_OTGFSEN) == 0U)
#endif /* USB_OTG_FS */

#if defined(AES)
#define __HAL_RCC_AES_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_AESEN) == 0U)
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_HASHEN) == 0U)
#endif /* HASH */

#define __HAL_RCC_RNG_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_RNGEN) == 0U)

#define __HAL_RCC_PKA_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_PKAEN) == 0U)

#if defined(SAES)
#define __HAL_RCC_SAES_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SAESEN) == 0U)
#endif /* SAES */

#if defined(CCB)
#define __HAL_RCC_CCB_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CCBEN) == 0U)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADC3EN) == 0U)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_ADF1EN) == 0U)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOKEN) == 0U)
#endif /* GPIOK */

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM4EN) == 0U)
#endif /* SRAM4_BASE */

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM5EN) == 0U)
#endif /* SRAM5_BASE */

#define __HAL_RCC_SRAM2_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM2EN) == 0U)

#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_SRAM3EN) == 0U)
#endif /* SRAM3_BASE */
/**
  * @}
  */

/** @defgroup RCC_AHB4_Peripheral_Clock_Enable_Disable_Status AHB4 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the AHB4 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC1EN) != 0U)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_IS_CLK_ENABLED()        (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC2EN) != 0U)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC1EN) != 0U)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_IS_CLK_ENABLED()         (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC2EN) != 0U)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_IS_CLK_ENABLED()            (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_FMCEN) != 0U)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI1EN) != 0U)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI2EN) != 0U)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_IS_CLK_ENABLED()          (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPIMEN) != 0U)
#endif /* OCTOSPIM */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC1EN) == 0U)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_IS_CLK_DISABLED()       (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OTFDEC2EN) == 0U)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC1EN) == 0U)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_IS_CLK_DISABLED()        (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_SDMMC2EN) == 0U)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_IS_CLK_DISABLED()           (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_FMCEN) == 0U)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI1EN) == 0U)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPI2EN) == 0U)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_IS_CLK_DISABLED()         (READ_BIT(RCC->AHB4ENR, RCC_AHB4ENR_OCTOSPIMEN) == 0U)
#endif /* OCTOSPIM */

/**
  * @}
  */


/** @defgroup RCC_APB1_Peripheral_Clock_Enable_Disable_Status APB1 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the APB1 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */


#define __HAL_RCC_TIM2_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN) != 0U)

#define __HAL_RCC_TIM3_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN) != 0U)

#if defined(TIM4)
#define __HAL_RCC_TIM4_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN) != 0U)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN) != 0U)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM6EN) != 0U)

#define __HAL_RCC_TIM7_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM7EN) != 0U)

#if defined(TIM12)
#define __HAL_RCC_TIM12_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM12EN) != 0U)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM13EN) != 0U)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM14EN) != 0U)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_WWDGEN) != 0U)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_OPAMPEN) != 0U)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI2EN) != 0U)

#define __HAL_RCC_SPI3_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN) != 0U)

#if defined(COMP1)
#define __HAL_RCC_COMP_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_COMPEN) != 0U)
#endif /* COMP1 */

#define __HAL_RCC_USART2_IS_CLK_ENABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN) != 0U)

#define __HAL_RCC_USART3_IS_CLK_ENABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART3EN) != 0U)

#if defined(UART4)
#define __HAL_RCC_UART4_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART4EN) != 0U)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN) != 0U)
#endif /* UART5 */

#define __HAL_RCC_I2C1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN) != 0U)

#define __HAL_RCC_I2C2_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C2EN) != 0U)

#define __HAL_RCC_I3C1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I3C1EN) != 0U)

#define __HAL_RCC_CRS_IS_CLK_ENABLED()         (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CRSEN) != 0U)

#if defined(USART6)
#define __HAL_RCC_USART6_IS_CLK_ENABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART6EN) != 0U)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_IS_CLK_ENABLED()     (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART10EN) != 0U)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_IS_CLK_ENABLED()     (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART11EN) != 0U)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_IS_CLK_ENABLED()         (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CECEN) != 0U)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART7EN) != 0U)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART8EN) != 0U)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART9EN) != 0U)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_IS_CLK_ENABLED()      (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART12EN) != 0U)
#endif /* UART12 */

#define __HAL_RCC_DTS_IS_CLK_ENABLED()         (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_DTSEN) != 0U)

#define __HAL_RCC_LPTIM2_IS_CLK_ENABLED()      (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_LPTIM2EN) != 0U)

#define __HAL_RCC_FDCAN_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_FDCANEN) != 0U)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_IS_CLK_ENABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UCPD1EN) != 0U)
#endif /* UCPD1 */


#define __HAL_RCC_TIM2_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM2EN) == 0U)

#define __HAL_RCC_TIM3_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM3EN) == 0U)

#if defined(TIM4)
#define __HAL_RCC_TIM4_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM4EN) == 0U)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM5EN) == 0U)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM6EN) == 0U)

#define __HAL_RCC_TIM7_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM7EN) == 0U)

#if defined(TIM12)
#define __HAL_RCC_TIM12_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM12EN) == 0U)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM13EN) == 0U)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_TIM14EN) == 0U)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_WWDGEN) == 0U)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_OPAMPEN) == 0U)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI2EN) == 0U)

#define __HAL_RCC_SPI3_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_SPI3EN) == 0U)

#if defined(COMP1)
#define __HAL_RCC_COMP_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_COMPEN) == 0U)
#endif /* COMP1 */

#define __HAL_RCC_USART2_IS_CLK_DISABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART2EN) == 0U)

#define __HAL_RCC_USART3_IS_CLK_DISABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART3EN) == 0U)

#if defined(UART4)
#define __HAL_RCC_UART4_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART4EN) == 0U)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART5EN) == 0U)
#endif /* UART5 */

#define __HAL_RCC_I2C1_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C1EN) == 0U)

#define __HAL_RCC_I2C2_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I2C2EN) == 0U)

#define __HAL_RCC_I3C1_IS_CLK_DISABLED()        (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_I3C1EN) == 0U)

#define __HAL_RCC_CRS_IS_CLK_DISABLED()         (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CRSEN) == 0U)

#if defined(USART6)
#define __HAL_RCC_USART6_IS_CLK_DISABLED()      (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART6EN) == 0U)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_IS_CLK_DISABLED()     (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART10EN) == 0U)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_IS_CLK_DISABLED()     (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_USART11EN) == 0U)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_IS_CLK_DISABLED()         (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_CECEN) == 0U)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART7EN) == 0U)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1LENR, RCC_APB1LENR_UART8EN) == 0U)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART9EN) == 0U)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_IS_CLK_DISABLED()      (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UART12EN) == 0U)
#endif /* UART12 */

#define __HAL_RCC_DTS_IS_CLK_DISABLED()         (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_DTSEN) == 0U)

#define __HAL_RCC_LPTIM2_IS_CLK_DISABLED()      (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_LPTIM2EN) == 0U)

#define __HAL_RCC_FDCAN_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_FDCANEN) == 0U)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_IS_CLK_DISABLED()       (READ_BIT(RCC->APB1HENR, RCC_APB1HENR_UCPD1EN) == 0U)
#endif /* UCPD1 */
/**
  * @}
  */

/** @defgroup RCC_APB2_Peripheral_Clock_Enable_Disable_Status APB2 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the APB2 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_TIM1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN) != 0U)

#define __HAL_RCC_SPI1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN) != 0U)

#if defined(TIM8)
#define __HAL_RCC_TIM8_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN) != 0U)
#endif /* TIM8 */

#define __HAL_RCC_USART1_IS_CLK_ENABLED()      (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN) != 0U)

#if defined(TIM15)
#define __HAL_RCC_TIM15_IS_CLK_ENABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN) != 0U)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_IS_CLK_ENABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN) != 0U)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_IS_CLK_ENABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM17EN) != 0U)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN) != 0U)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN) != 0U)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN) != 0U)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN) != 0U)
#endif /* SAI2 */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_IS_CLK_ENABLED()         (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USBEN) != 0U)
#endif /* USB_DRD_FS */

#if defined(LTDC)
#define __HAL_RCC_LTDC_IS_CLK_ENABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_LTDCEN) != 0U)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_IS_CLK_ENABLED()      (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_GFXTIMEN) != 0U)
#endif /* GFXTIM */

#define __HAL_RCC_TIM1_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN) == 0U)

#define __HAL_RCC_SPI1_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN) == 0U)

#if defined(TIM8)
#define __HAL_RCC_TIM8_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN) == 0U)
#endif /* TIM8 */

#define __HAL_RCC_USART1_IS_CLK_DISABLED()     (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN) == 0U)

#if defined(TIM15)
#define __HAL_RCC_TIM15_IS_CLK_DISABLED()      (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN) == 0U)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_IS_CLK_DISABLED()      (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM16EN) == 0U)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_IS_CLK_DISABLED()      (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM17EN) == 0U)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN) == 0U)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI6EN) == 0U)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI1EN) == 0U)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SAI2EN) == 0U)
#endif /* SAI2 */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_IS_CLK_DISABLED()        (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USBEN) == 0U)
#endif /* USB_DRD_FS */

#if defined(LTDC)
#define __HAL_RCC_LTDC_IS_CLK_DISABLED()       (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_LTDCEN) == 0U)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_IS_CLK_DISABLED()     (READ_BIT(RCC->APB2ENR, RCC_APB2ENR_GFXTIMEN) == 0U)
#endif /* GFXTIM */
/**
  * @}
  */

/** @defgroup RCC_APB3_Peripheral_Clock_Enable_Disable_Status APB3 Peripheral Clock Enabled or Disabled Status
  * @brief  Check whether the APB3 peripheral clock is enabled or not.
  * @note   After reset, the peripheral clock (used for registers read/write access)
  *         is disabled and the application software has to enable this clock before
  *         using it.
  * @{
  */

#define __HAL_RCC_SBS_IS_CLK_ENABLED()            (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SBSEN) != 0U)

#if defined(SPI5)
#define __HAL_RCC_SPI5_IS_CLK_ENABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SPI5EN) != 0U)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_IS_CLK_ENABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPUART1EN) != 0U)

#if defined(I2C3)
#define __HAL_RCC_I2C3_IS_CLK_ENABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C3EN) != 0U)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_IS_CLK_ENABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C4EN) != 0U)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_IS_CLK_ENABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I3C2EN) != 0U)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_IS_CLK_ENABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM1EN) != 0U)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_IS_CLK_ENABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM3EN) != 0U)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_IS_CLK_ENABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM4EN) != 0U)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_IS_CLK_ENABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM5EN) != 0U)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_IS_CLK_ENABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM6EN) != 0U)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_IS_CLK_ENABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_VREFEN) != 0U)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_IS_CLK_ENABLED()            (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_RTCAPBEN) != 0U)

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_IS_CLK_ENABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1EN) != 0U)
#define __HAL_RCC_PLAY1APB_IS_CLK_ENABLED()       (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1APBEN) != 0U)
#endif /* PLAY1 */

#define __HAL_RCC_SBS_IS_CLK_DISABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SBSEN) == 0U)

#if defined(SPI5)
#define __HAL_RCC_SPI5_IS_CLK_DISABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_SPI5EN) == 0U)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_IS_CLK_DISABLED()       (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPUART1EN) == 0U)

#if defined(I2C3)
#define __HAL_RCC_I2C3_IS_CLK_DISABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C3EN) == 0U)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_IS_CLK_DISABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I2C4EN) == 0U)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_IS_CLK_DISABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_I3C2EN) == 0U)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_IS_CLK_DISABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM1EN) == 0U)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_IS_CLK_DISABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM3EN) == 0U)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_IS_CLK_DISABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM4EN) == 0U)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_IS_CLK_DISABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM5EN) == 0U)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_IS_CLK_DISABLED()        (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_LPTIM6EN) == 0U)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_IS_CLK_DISABLED()          (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_VREFEN) == 0U)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_IS_CLK_DISABLED()           (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_RTCAPBEN) == 0U)

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_IS_CLK_DISABLED()         (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1EN) == 0U)
#define __HAL_RCC_PLAY1APB_IS_CLK_DISABLED()      (READ_BIT(RCC->APB3ENR, RCC_APB3ENR_PLAY1APBEN) == 0U)
#endif /* PLAY1 */

/**
  * @}
  */

/** @defgroup RCC_AHB_APB_Branch_Clock_Disable_Status AHB APB Branch Clock Disabled Status
  * @brief  Check whether the AHBx/APBx branch clock for all AHBx/APBx peripherals is disabled or not.
  * @note   It is recommended to disable the clock of all peripherals (by writing 0 in
  *         the AHBxENR/APBxENR register) before Disabling the corresponding Bus Branch clock.
  *         Some peripheral bus clocks are not affected by branch clock disabling as IWDG (APB1),
  *         SRAM2/SRAM3 (AHB2) and FLITF/BKRAM/ICACHE/DCACHE/SRAM1 (AHB1).
  * @{
  */

#define __HAL_RCC_AHB1_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB1DIS) != 0U)

#define __HAL_RCC_AHB2_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB2DIS) != 0U)

#if defined(AHB4PERIPH_BASE)
#define __HAL_RCC_AHB4_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_AHB4DIS) != 0U)
#endif /* AHB4PERIPH_BASE */

#define __HAL_RCC_APB1_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_APB1DIS) != 0U)

#define __HAL_RCC_APB2_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_APB2DIS) != 0U)

#define __HAL_RCC_APB3_IS_CLK_DISABLED()          (READ_BIT(RCC->CFGR2, RCC_CFGR2_APB3DIS) != 0U)

/**
  * @}
  */

/** @defgroup RCC_AHB1_Force_Release_Reset AHB1 Peripheral Force Release Reset
  * @brief  Force or release AHB1 peripheral reset.
  * @{
  */

#define __HAL_RCC_AHB1_FORCE_RESET()      WRITE_REG(RCC->AHB1RSTR, 0x010AD003U)

#define __HAL_RCC_GPDMA1_FORCE_RESET()    SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_GPDMA1RST)

#define __HAL_RCC_GPDMA2_FORCE_RESET()    SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_GPDMA2RST)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_FORCE_RESET()    SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_CORDICRST)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_FORCE_RESET()      SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_FMACRST)
#endif /* FMAC */

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_FORCE_RESET()     SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_DMA2DRST)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_FORCE_RESET()      SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_JPEGRST)
#endif /* JPEG */

#if defined(MDF1)
#define __HAL_RCC_MDF1_FORCE_RESET()      SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_MDF1RST)
#endif /* MDF1 */

#define __HAL_RCC_CRC_FORCE_RESET()       SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_CRCRST)

#define __HAL_RCC_RAMCFG_FORCE_RESET()    SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_RAMCFGRST)

#if defined(ETH)
#define __HAL_RCC_ETH_FORCE_RESET()       SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_ETHRST)
#endif /* ETH */

#define __HAL_RCC_GTZC1_FORCE_RESET()     SET_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_TZSC1RST)


#define __HAL_RCC_AHB1_RELEASE_RESET()    WRITE_REG(RCC->AHB1RSTR, 0x00000000U)

#define __HAL_RCC_GPDMA1_RELEASE_RESET()  CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_GPDMA1RST)

#define __HAL_RCC_GPDMA2_RELEASE_RESET()  CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_GPDMA2RST)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_RELEASE_RESET()  CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_CORDICRST)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_RELEASE_RESET()    CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_FMACRST)
#endif /* FMAC */

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_RELEASE_RESET()   CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_DMA2DRST)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_RELEASE_RESET()    CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_JPEGRST)
#endif /* JPEG */

#if defined(MDF1)
#define __HAL_RCC_MDF1_RELEASE_RESET()    CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_MDF1RST)
#endif /* MDF1 */

#define __HAL_RCC_CRC_RELEASE_RESET()     CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_CRCRST)

#define __HAL_RCC_RAMCFG_RELEASE_RESET()  CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_RAMCFGRST)

#if defined(ETH)
#define __HAL_RCC_ETH_RELEASE_RESET()     CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_ETHRST)
#endif /* ETH */

#define __HAL_RCC_GTZC1_RELEASE_RESET()   CLEAR_BIT(RCC->AHB1RSTR, RCC_AHB1RSTR_TZSC1RST)

/**
  * @}
  */

/** @defgroup RCC_AHB2_Force_Release_Reset AHB2 Peripheral Force Release Reset
  * @brief  Force or release AHB2 peripheral reset.
  * @{
  */

#define __HAL_RCC_AHB2_FORCE_RESET()      WRITE_REG(RCC->AHB2RSTR, 0x001F1DFFU)

#define __HAL_RCC_GPIOA_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOARST)

#define __HAL_RCC_GPIOB_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOBRST)

#define __HAL_RCC_GPIOC_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOCRST)

#define __HAL_RCC_GPIOD_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIODRST)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOERST)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOFRST)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOGRST)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOHRST)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOIRST)
#endif /* GPIOI */

#define __HAL_RCC_ADC_FORCE_RESET()       SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADCRST)

#define __HAL_RCC_DAC1_FORCE_RESET()      SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_DAC1RST)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_FORCE_RESET()  SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_DCMI_PSSIRST)
#define __HAL_RCC_DCMI_FORCE_RESET()       __HAL_RCC_DCMI_PSSI_FORCE_RESET()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(AES)
#define __HAL_RCC_AES_FORCE_RESET()        SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_AESRST)
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_FORCE_RESET()       SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_HASHRST)
#endif /* HASH */

#define __HAL_RCC_RNG_FORCE_RESET()        SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_RNGRST)

#if defined(PKA)
#define __HAL_RCC_PKA_FORCE_RESET()        SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_PKARST)
#endif /* PKA */

#if defined(SAES)
#define __HAL_RCC_SAES_FORCE_RESET()       SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_SAESRST)
#endif /* SAES*/

#if defined(CCB)
#define __HAL_RCC_CCB_FORCE_RESET()        SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_CCBRST)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_FORCE_RESET()       SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADC3RST)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_FORCE_RESET()       SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADF1RST)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_FORCE_RESET()      SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOKRST)
#endif /* GPIOK */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_FORCE_RESET()      SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOJRST)
#endif /* GPIOJ */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_FORCE_RESET() SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGHSRST)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_FORCE_RESET() SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST)
#endif /* USB_OTG_FS */

#if defined(RCC_AHB2RSTR_OTGHSPHYRST)
#define __HAL_RCC_OTGPHY_FORCE_RESET()     SET_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGHSPHYRST)
#endif /* RCC_AHB2RSTR_OTGHSPHYRST */

#define __HAL_RCC_AHB2_RELEASE_RESET()    WRITE_REG(RCC->AHB2RSTR, 0x00000000U)

#define __HAL_RCC_GPIOA_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOARST)

#define __HAL_RCC_GPIOB_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOBRST)

#define __HAL_RCC_GPIOC_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOCRST)

#define __HAL_RCC_GPIOD_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIODRST)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOERST)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOFRST)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOGRST)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOHRST)

#if defined(GPIOG)
#define __HAL_RCC_GPIOI_RELEASE_RESET()   CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOIRST)
#endif /* GPIOI */

#define __HAL_RCC_ADC_RELEASE_RESET()     CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADCRST)

#define __HAL_RCC_DAC1_RELEASE_RESET()    CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_DAC1RST)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_RELEASE_RESET()  CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_DCMI_PSSIRST)
#define __HAL_RCC_DCMI_RELEASE_RESET()     __HAL_RCC_DCMI_PSSI_RELEASE_RESET()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(AES)
#define __HAL_RCC_AES_RELEASE_RESET()        CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_AESRST)
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_RELEASE_RESET()       CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_HASHRST)
#endif /* HASH */

#define __HAL_RCC_RNG_RELEASE_RESET()        CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_RNGRST)

#if defined(PKA)
#define __HAL_RCC_PKA_RELEASE_RESET()        CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_PKARST)
#endif /* PKA */

#if defined(SAES)
#define __HAL_RCC_SAES_RELEASE_RESET()       CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_SAESRST)
#endif /* SAES*/

#if defined(CCB)
#define __HAL_RCC_CCB_RELEASE_RESET()        CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_CCBRST)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_RELEASE_RESET()       CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADC3RST)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_RELEASE_RESET()       CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_ADF1RST)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_RELEASE_RESET()      CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOKRST)
#endif /* GPIOK */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_RELEASE_RESET()      CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_GPIOJRST)
#endif /* GPIOJ */

#if defined(RCC_AHB2RSTR_OTGHSPHYRST)
#define __HAL_RCC_OTGPHY_RELEASE_RESET()     CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGHSPHYRST)
#endif /* RCC_AHB2RSTR_OTGHSPHYRST */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_RELEASE_RESET() CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGHSRST)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_RELEASE_RESET() CLEAR_BIT(RCC->AHB2RSTR, RCC_AHB2RSTR_OTGFSRST)
#endif /* USB_OTG_FS */

/**
  * @}
  */

/** @defgroup RCC_AHB4_Force_Release_Reset AHB4 Peripheral Force Release Reset
  * @brief  Force or release AHB4 peripheral reset.
  * @{
  */

#if defined(FMC_BASE)
#define __HAL_RCC_AHB4_FORCE_RESET()      WRITE_REG(RCC->AHB4RSTR, 0x00111880U)
#endif /* FMC_BASE */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_FORCE_RESET()   SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OTFDEC1RST)
#endif /* OTFDEC1 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_FORCE_RESET()    SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_SDMMC1RST)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_FORCE_RESET()    SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_SDMMC2RST)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_FORCE_RESET()       SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_FMCRST)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_FORCE_RESET()     SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPI1RST)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_FORCE_RESET()     SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPI2RST)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_FORCE_RESET()     SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPIMRST)
#endif /* OCTOSPIM */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_FORCE_RESET()   SET_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OTFDEC2RST)
#endif /* OTFDEC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_AHB4_RELEASE_RESET()    WRITE_REG(RCC->AHB4RSTR, 0x00000000U)
#endif /* FMC_BASE */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_RELEASE_RESET() CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OTFDEC1RST)
#endif /* OTFDEC1 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_RELEASE_RESET()  CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_SDMMC1RST)
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_RELEASE_RESET()  CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_SDMMC2RST)
#endif /* SDMMC2 */

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_RELEASE_RESET()     CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_FMCRST)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_RELEASE_RESET()   CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPI1RST)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_RELEASE_RESET()   CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPI2RST)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_RELEASE_RESET()   CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OCTOSPIMRST)
#endif /* OCTOSPIM */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_RELEASE_RESET() CLEAR_BIT(RCC->AHB4RSTR, RCC_AHB4RSTR_OTFDEC2RST)
#endif /* OTFDEC2 */

/**
  * @}
  */


/** @defgroup RCC_APB1_Force_Release_Reset APB1 Peripheral Force Release Reset
  * @brief  Force or release APB1 peripheral reset.
  * @{
  */

#define __HAL_RCC_APB1_FORCE_RESET()     do { \
                                                 WRITE_REG(RCC->APB1LRSTR, 0xDFFEC1FFU); \
                                                 WRITE_REG(RCC->APB1HRSTR, 0x4080062BU); \
                                               } while(0)

#define __HAL_RCC_TIM2_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM2RST)

#define __HAL_RCC_TIM3_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM3RST)

#if defined(TIM4)
#define __HAL_RCC_TIM4_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM4RST)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM5RST)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM6RST)

#define __HAL_RCC_TIM7_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM7RST)

#if defined(TIM12)
#define __HAL_RCC_TIM12_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM12RST)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM13RST)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM14RST)
#endif /* TIM14 */

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_OPAMPRST)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_SPI2RST)

#define __HAL_RCC_SPI3_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_SPI3RST)

#if defined(COMP1)
#define __HAL_RCC_COMP_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_COMPRST)
#endif /* COMP1 */

#define __HAL_RCC_USART2_FORCE_RESET()   SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART2RST)

#define __HAL_RCC_USART3_FORCE_RESET()   SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART3RST)

#if defined(UART4)
#define __HAL_RCC_UART4_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART4RST)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART5RST)
#endif /* UART5 */

#define __HAL_RCC_I2C1_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I2C1RST)

#define __HAL_RCC_I2C2_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I2C2RST)

#define __HAL_RCC_I3C1_FORCE_RESET()     SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I3C1RST)

#define __HAL_RCC_CRS_FORCE_RESET()      SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_CRSRST)

#if defined(USART6)
#define __HAL_RCC_USART6_FORCE_RESET()   SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART6RST)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_FORCE_RESET()  SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART10RST)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_FORCE_RESET()  SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART11RST)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_FORCE_RESET()      SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_CECRST)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART7RST)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_FORCE_RESET()    SET_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART8RST)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_FORCE_RESET()    SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UART9RST)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_FORCE_RESET()   SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UART12RST)
#endif /* UART12 */

#define __HAL_RCC_DTS_FORCE_RESET()      SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_DTSRST)

#define __HAL_RCC_LPTIM2_FORCE_RESET()   SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_LPTIM2RST)

#define __HAL_RCC_FDCAN_FORCE_RESET()    SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_FDCANRST)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_FORCE_RESET()    SET_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UCPD1RST)
#endif /* UCPD1 */


#define __HAL_RCC_APB1_RELEASE_RESET()   do { \
                                                 WRITE_REG(RCC->APB1LRSTR, 0x00000000U); \
                                                 WRITE_REG(RCC->APB1HRSTR, 0x00000000U); \
                                               } while(0)

#define __HAL_RCC_TIM2_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM2RST)

#define __HAL_RCC_TIM3_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM3RST)

#if defined(TIM4)
#define __HAL_RCC_TIM4_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM4RST)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM5RST)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM6RST)

#define __HAL_RCC_TIM7_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM7RST)

#if defined(TIM12)
#define __HAL_RCC_TIM12_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM12RST)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM13RST)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_TIM14RST)
#endif /* TIM14 */

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_OPAMPRST)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_SPI2RST)

#define __HAL_RCC_SPI3_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_SPI3RST)

#if defined(COMP1)
#define __HAL_RCC_COMP_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_COMPRST)
#endif /* COMP1 */

#define __HAL_RCC_USART2_RELEASE_RESET()  CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART2RST)

#define __HAL_RCC_USART3_RELEASE_RESET()  CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART3RST)

#if defined(UART4)
#define __HAL_RCC_UART4_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART4RST)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART5RST)
#endif /* UART5 */

#define __HAL_RCC_I2C1_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I2C1RST)

#define __HAL_RCC_I2C2_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I2C2RST)

#define __HAL_RCC_I3C1_RELEASE_RESET()    CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_I3C1RST)

#define __HAL_RCC_CRS_RELEASE_RESET()     CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_CRSRST)

#if defined(USART6)
#define __HAL_RCC_USART6_RELEASE_RESET()  CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART6RST)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_RELEASE_RESET() CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART10RST)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_RELEASE_RESET() CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_USART11RST)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_RELEASE_RESET()     CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_CECRST)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART7RST)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_RELEASE_RESET()   CLEAR_BIT(RCC->APB1LRSTR, RCC_APB1LRSTR_UART8RST)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_RELEASE_RESET()   CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UART9RST)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_RELEASE_RESET()  CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UART12RST)
#endif /* UART12 */

#define __HAL_RCC_DTS_RELEASE_RESET()     CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_DTSRST)

#define __HAL_RCC_LPTIM2_RELEASE_RESET()  CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_LPTIM2RST)

#define __HAL_RCC_FDCAN_RELEASE_RESET()   CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_FDCANRST)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_RELEASE_RESET()   CLEAR_BIT(RCC->APB1HRSTR, RCC_APB1HRSTR_UCPD1RST)
#endif /* UCPD1 */

/**
  * @}
  */

/** @defgroup RCC_APB2_Force_Release_Reset APB2 Peripheral Force Release Reset
  * @brief  Force or release APB2 peripheral reset.
  * @{
  */

#define __HAL_RCC_APB2_FORCE_RESET()          WRITE_REG(RCC->APB2RSTR, 0x017F7800U)

#define __HAL_RCC_TIM1_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST)

#define __HAL_RCC_SPI1_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST)

#if defined(TIM8)
#define __HAL_RCC_TIM8_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM8RST)
#endif /* TIM8 */

#define __HAL_RCC_USART1_FORCE_RESET()        SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST)

#if defined(TIM15)
#define __HAL_RCC_TIM15_FORCE_RESET()         SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM15RST)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_FORCE_RESET()         SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_FORCE_RESET()         SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM17RST)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI4RST)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI6RST)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SAI1RST)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SAI2RST)
#endif /* SAI2 */

#if defined(LTDC)
#define __HAL_RCC_LTDC_FORCE_RESET()          SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_LTDCRST)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_FORCE_RESET()        SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_GFXTIMRST)
#endif /* GFXTIM */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_FORCE_RESET()           SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USBRST)
#endif /* USB_DRD_FS */

#define __HAL_RCC_APB2_RELEASE_RESET()        WRITE_REG(RCC->APB2RSTR, 0x00000000U)

#define __HAL_RCC_TIM1_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM1RST)

#define __HAL_RCC_SPI1_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI1RST)

#if defined(TIM8)
#define __HAL_RCC_TIM8_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM8RST)
#endif /* TIM8 */

#define __HAL_RCC_USART1_RELEASE_RESET()      CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST)

#if defined(TIM15)
#define __HAL_RCC_TIM15_RELEASE_RESET()       CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM15RST)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_RELEASE_RESET()       CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_RELEASE_RESET()       CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_TIM17RST)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI4RST)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SPI6RST)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SAI1RST)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_SAI2RST)
#endif /* SAI2 */

#if defined(LTDC)
#define __HAL_RCC_LTDC_RELEASE_RESET()        CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_LTDCRST)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_RELEASE_RESET()      CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_GFXTIMRST)
#endif /* GFXTIM */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_RELEASE_RESET()         CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USBRST)
#endif /* USB_DRD_FS */

/**
  * @}
  */

/** @defgroup RCC_APB3_Force_Release_Reset APB3 Peripheral Force Release Reset
  * @brief  Force or release APB3 peripheral reset.
  * @{
  */

#define __HAL_RCC_APB3_FORCE_RESET()           WRITE_REG(RCC->APB3RSTR, 0x001008E0U)

#if defined(SPI5)
#define __HAL_RCC_SPI5_FORCE_RESET()           SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_SPI5RST)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_FORCE_RESET()        SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPUART1RST)

#if defined(I2C3)
#define __HAL_RCC_I2C3_FORCE_RESET()           SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I2C3RST)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_FORCE_RESET()           SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I2C4RST)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_FORCE_RESET()           SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I3C2RST)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_FORCE_RESET()         SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM1RST)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_FORCE_RESET()         SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM3RST)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_FORCE_RESET()         SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM4RST)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_FORCE_RESET()         SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM5RST)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_FORCE_RESET()         SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM6RST)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_FORCE_RESET()           SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_VREFRST)
#endif /* VREFBUF */

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_FORCE_RESET()          SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_PLAY1RST)
#define __HAL_RCC_PLAY1POR_FORCE_RESET()       SET_BIT(RCC->APB3RSTR, RCC_APB3RSTR_PLAY1POR)
#endif /* PLAY1 */

#define __HAL_RCC_APB3_RELEASE_RESET()         WRITE_REG(RCC->APB3RSTR, 0x00000000U)

#if defined(SPI5)
#define __HAL_RCC_SPI5_RELEASE_RESET()         CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_SPI5RST)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_RELEASE_RESET()      CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPUART1RST)

#if defined(I2C3)
#define __HAL_RCC_I2C3_RELEASE_RESET()         CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I2C3RST)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_RELEASE_RESET()         CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I2C4RST)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_RELEASE_RESET()         CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_I3C2RST)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_RELEASE_RESET()       CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM1RST)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_RELEASE_RESET()       CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM3RST)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_RELEASE_RESET()       CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM4RST)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_RELEASE_RESET()       CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM5RST)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_RELEASE_RESET()       CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_LPTIM6RST)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_RELEASE_RESET()         CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_VREFRST)
#endif /* VREFBUF */

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_RELEASE_RESET()        CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_PLAY1RST)
#define __HAL_RCC_PLAY1POR_RELEASE_RESET()     CLEAR_BIT(RCC->APB3RSTR, RCC_APB3RSTR_PLAY1POR)
#endif /* PLAY1 */
/**
  * @}
  */

/** @defgroup RCC_AHB1_Peripheral_Clock_Sleep_Enable_Disable AHB1 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the AHB1 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#define __HAL_RCC_GPDMA1_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_GPDMA1LPEN)

#define __HAL_RCC_GPDMA2_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_GPDMA2LPEN)

#define __HAL_RCC_FLASH_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_FLITFLPEN)

#define __HAL_RCC_CRC_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_CRCLPEN)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_CORDICLPEN)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_FMACLPEN)
#endif /* FMAC */

#if defined(MDF1)
#define __HAL_RCC_MDF1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_MDF1LPEN)
#endif /* MDF1 */

#define __HAL_RCC_RAMCFG_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_RAMCFGLPEN)

#if defined(ETH)
#define __HAL_RCC_ETH_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHLPEN)

#define __HAL_RCC_ETHTX_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHTXLPEN)

#define __HAL_RCC_ETHRX_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHRXLPEN)

#if defined(RCC_AHB1LPENR_ETHCKLPEN)
#define __HAL_RCC_ETHINTERN_CLK_SLEEP_ENABLE()      SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHCKLPEN)
#endif /* RCC_AHB1LPENR_ETHCKLPEN */
#endif /* ETH */

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_DMA2DLPEN)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_JPEGLPEN)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_TZSC1LPEN)

#define __HAL_RCC_BKPRAM_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_BKPRAMLPEN)

#define __HAL_RCC_ICACHE_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ICACHELPEN)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_CLK_SLEEP_ENABLE()        SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_DCACHE1LPEN)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_SRAM1LPEN)


#define __HAL_RCC_GPDMA1_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_GPDMA1LPEN)

#define __HAL_RCC_GPDMA2_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_GPDMA2LPEN)

#define __HAL_RCC_FLASH_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_FLITFLPEN)

#define __HAL_RCC_CRC_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_CRCLPEN)

#if defined(CORDIC)
#define __HAL_RCC_CORDIC_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_CORDICLPEN)
#endif /* CORDIC */

#if defined(FMAC)
#define __HAL_RCC_FMAC_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_FMACLPEN)
#endif /* FMAC */

#if defined(MDF1)
#define __HAL_RCC_MDF1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_MDF1LPEN)
#endif /* MDF1 */

#define __HAL_RCC_RAMCFG_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_RAMCFGLPEN)

#if defined(ETH)
#define __HAL_RCC_ETH_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHLPEN)

#define __HAL_RCC_ETHTX_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHTXLPEN)

#define __HAL_RCC_ETHRX_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHRXLPEN)

#if defined(RCC_AHB1LPENR_ETHCKLPEN)
#define __HAL_RCC_ETHINTERN_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ETHCKLPEN)
#endif /* RCC_AHB1LPENR_ETHCKLPEN */
#endif /* ETH */

#if defined(DMA2D)
#define __HAL_RCC_DMA2D_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_DMA2DLPEN)
#endif /* DMA2D */

#if defined(JPEG)
#define __HAL_RCC_JPEG_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_JPEGLPEN)
#endif /* JPEG */

#define __HAL_RCC_GTZC1_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_TZSC1LPEN)

#define __HAL_RCC_BKPRAM_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_BKPRAMLPEN)

#define __HAL_RCC_ICACHE_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_ICACHELPEN)

#if defined(DCACHE1)
#define __HAL_RCC_DCACHE1_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_DCACHE1LPEN)
#endif /* DCACHE1 */

#define __HAL_RCC_SRAM1_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB1LPENR, RCC_AHB1LPENR_SRAM1LPEN)

/**
  * @}
  */

/** @defgroup RCC_AHB2_Peripheral_Clock_Sleep_Enable_Disable AHB2 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the AHB2 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#define __HAL_RCC_GPIOA_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOALPEN)

#define __HAL_RCC_GPIOB_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOBLPEN)

#define __HAL_RCC_GPIOC_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOCLPEN)

#define __HAL_RCC_GPIOD_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIODLPEN)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOELPEN)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOFLPEN)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOGLPEN)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOHLPEN)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOILPEN)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOJLPEN)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADCLPEN)

#define __HAL_RCC_DAC1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_DAC1LPEN)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_CLK_SLEEP_ENABLE()      SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_DCMI_PSSILPEN)
#define __HAL_RCC_DCMI_CLK_SLEEP_ENABLE()   __HAL_RCC_DCMI_PSSI_CLK_SLEEP_ENABLE()  /* for API backward compatibility */
#endif /* DCMI */

#if defined(RCC_AHB2LPENR_OTGPHYLPEN)
#define __HAL_RCC_OTGPHY_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGPHYLPEN);
#endif /* RCC_AHB2LPENR_OTGPHYLPEN */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_ENABLE()     SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGHSLPEN);
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_CLK_SLEEP_ENABLE()     SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGFSLPEN);
#endif /* USB_OTG_FS */

#if defined(AES)
#define __HAL_RCC_AES_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_AESLPEN);
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_HASHLPEN)
#endif /* HASH */

#define __HAL_RCC_RNG_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_RNGLPEN)

#if defined(PKA)
#define __HAL_RCC_PKA_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_PKALPEN)
#endif /*PKA*/

#if defined(SAES)
#define __HAL_RCC_SAES_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SAESLPEN)
#endif /* AES */

#if defined(CCB)
#define __HAL_RCC_CCB_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_CCBLPEN);
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADC3LPEN);
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADF1LPEN);
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOKLPEN);
#endif /* GPIOK */

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM4LPEN);
#endif /* SRAM4_BASE */

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM5LPEN);
#endif /* SRAM5_BASE */

#define __HAL_RCC_SRAM2_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM2LPEN)

#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM3LPEN)
#endif /* SRAM3_BASE */

#define __HAL_RCC_GPIOA_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOALPEN)

#define __HAL_RCC_GPIOB_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOBLPEN)

#define __HAL_RCC_GPIOC_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOCLPEN)

#define __HAL_RCC_GPIOD_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIODLPEN)

#if defined(GPIOE)
#define __HAL_RCC_GPIOE_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOELPEN)
#endif /* GPIOE */

#if defined(GPIOF)
#define __HAL_RCC_GPIOF_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOFLPEN)
#endif /* GPIOF */

#if defined(GPIOG)
#define __HAL_RCC_GPIOG_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOGLPEN)
#endif /* GPIOG */

#define __HAL_RCC_GPIOH_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOHLPEN)

#if defined(GPIOI)
#define __HAL_RCC_GPIOI_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOILPEN)
#endif /* GPIOI */

#if defined(GPIOJ)
#define __HAL_RCC_GPIOJ_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOJLPEN)
#endif /* GPIOJ */

#define __HAL_RCC_ADC_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADCLPEN)

#define __HAL_RCC_DAC1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_DAC1LPEN)

#if defined(DCMI)
#define __HAL_RCC_DCMI_PSSI_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_DCMI_PSSILPEN)
#define __HAL_RCC_DCMI_CLK_SLEEP_DISABLE()  __HAL_RCC_DCMI_PSSI_CLK_SLEEP_DISABLE() /* for API backward compatibility */
#endif /* DCMI */

#if defined(RCC_AHB2LPENR_OTGPHYLPEN)
#define __HAL_RCC_OTGPHY_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGPHYLPEN)
#endif /* RCC_AHB2LPENR_OTGPHYLPEN */

#if defined(USB_OTG_HS)
#define __HAL_RCC_USB_OTG_HS_CLK_SLEEP_DISABLE()    CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGHSLPEN)
#endif /* USB_OTG_HS */

#if defined(USB_OTG_FS)
#define __HAL_RCC_USB_OTG_FS_CLK_SLEEP_DISABLE()    CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_OTGFSLPEN)
#endif /* USB_OTG_FS */

#if defined(AES)
#define __HAL_RCC_AES_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_AESLPEN);
#endif /* AES */

#if defined(HASH)
#define __HAL_RCC_HASH_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_HASHLPEN)
#endif /* HASH */

#define __HAL_RCC_RNG_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_RNGLPEN)

#if defined(PKA)
#define __HAL_RCC_PKA_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_PKALPEN)
#endif /*PKA*/

#define __HAL_RCC_SAES_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SAESLPEN)

#if defined(CCB)
#define __HAL_RCC_CCB_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_CCBLPEN)
#endif /* CCB */

#if defined(ADC3)
#define __HAL_RCC_ADC3_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADC3LPEN)
#endif /* ADC3 */

#if defined(ADF1)
#define __HAL_RCC_ADF1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_ADF1LPEN)
#endif /* ADF1 */

#if defined(GPIOK)
#define __HAL_RCC_GPIOK_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_GPIOKLPEN)
#endif /* GPIOK */

#if defined(SRAM4_BASE)
#define __HAL_RCC_SRAM4_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM4LPEN)
#endif /* SRAM4_BASE */

#if defined(SRAM5_BASE)
#define __HAL_RCC_SRAM5_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM5LPEN)
#endif /* SRAM5_BASE */

#define __HAL_RCC_SRAM2_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM2LPEN)

#if defined(SRAM3_BASE)
#define __HAL_RCC_SRAM3_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB2LPENR, RCC_AHB2LPENR_SRAM3LPEN)
#endif /* SRAM3_BASE */
/**
  * @}
  */

/** @defgroup RCC_AHB4_Clock_Sleep_Enable_Disable AHB4 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the AHB4 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_CLK_SLEEP_ENABLE()        SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OTFDEC1LPEN)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_CLK_SLEEP_ENABLE()        SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OTFDEC2LPEN)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_SDMMC1LPEN)
#endif /* SDMMC1*/

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_CLK_SLEEP_ENABLE()         SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_SDMMC2LPEN)
#endif /* SDMMC2*/

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_CLK_SLEEP_ENABLE()            SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_FMCLPEN)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPI1LPEN)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPI2LPEN)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_CLK_SLEEP_ENABLE()          SET_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPIMLPEN)
#endif /* OCTOSPIM */

#if defined(OTFDEC1)
#define __HAL_RCC_OTFDEC1_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OTFDEC1LPEN)
#endif /* OTFDEC1 */

#if defined(OTFDEC2)
#define __HAL_RCC_OTFDEC2_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OTFDEC2LPEN)
#endif /* OTFDEC2 */

#if defined(SDMMC1)
#define __HAL_RCC_SDMMC1_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_SDMMC1LPEN)
#endif /* SDMMC1*/

#if defined(SDMMC2)
#define __HAL_RCC_SDMMC2_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_SDMMC2LPEN)
#endif /* SDMMC2*/

#if defined(FMC_BASE)
#define __HAL_RCC_FMC_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_FMCLPEN)
#endif /* FMC_BASE */

#if defined(OCTOSPI1)
#define __HAL_RCC_OSPI1_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPI1LPEN)
#endif /* OCTOSPI1 */

#if defined(OCTOSPI2)
#define __HAL_RCC_OSPI2_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPI2LPEN)
#endif /* OCTOSPI2 */

#if defined(OCTOSPIM)
#define __HAL_RCC_OSPIM_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->AHB4LPENR, RCC_AHB4LPENR_OCTOSPIMLPEN)
#endif /* OCTOSPIM */

/**
  * @}
  */

/** @defgroup RCC_APB1_Clock_Sleep_Enable_Disable APB1 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the APB1 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */

#define __HAL_RCC_TIM2_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM2LPEN)

#define __HAL_RCC_TIM3_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM3LPEN)

#if defined(TIM4)
#define __HAL_RCC_TIM4_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM4LPEN)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM5LPEN)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM6LPEN)

#define __HAL_RCC_TIM7_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM7LPEN)

#if defined(TIM12)
#define __HAL_RCC_TIM12_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM12LPEN)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM13LPEN)
#endif /* TIM13 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM14LPEN)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_WWDGLPEN)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_OPAMPLPEN)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_SPI2LPEN)

#define __HAL_RCC_SPI3_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_SPI3LPEN)

#if defined(COMP1)
#define __HAL_RCC_COMP_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_COMPLPEN)
#endif /* COMP1 */

#define __HAL_RCC_USART2_CLK_SLEEP_ENABLE()      SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART2LPEN)

#define __HAL_RCC_USART3_CLK_SLEEP_ENABLE()      SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART3LPEN)

#if defined(UART4)
#define __HAL_RCC_UART4_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART4LPEN)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART5LPEN)
#endif /* UART5 */

#define __HAL_RCC_I2C1_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I2C1LPEN)

#define __HAL_RCC_I2C2_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I2C2LPEN)

#define __HAL_RCC_I3C1_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I3C1LPEN)

#define __HAL_RCC_CRS_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_CRSLPEN)

#if defined(USART6)
#define __HAL_RCC_USART6_CLK_SLEEP_ENABLE()      SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART6LPEN)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_CLK_SLEEP_ENABLE()     SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART10LPEN)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_CLK_SLEEP_ENABLE()     SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART11LPEN)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_CECLPEN)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART7LPEN)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART8LPEN)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UART9LPEN)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UART12LPEN)
#endif /* UART12 */

#define __HAL_RCC_DTS_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_DTSLPEN)

#define __HAL_RCC_LPTIM2_CLK_SLEEP_ENABLE()      SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_LPTIM2LPEN)

#define __HAL_RCC_FDCAN_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_FDCANLPEN)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_CLK_SLEEP_ENABLE()       SET_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UCPD1LPEN)
#endif /* UCPD1 */

#define __HAL_RCC_TIM2_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM2LPEN)

#define __HAL_RCC_TIM3_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM3LPEN)

#if defined(TIM4)
#define __HAL_RCC_TIM4_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM4LPEN)
#endif /* TIM4 */

#if defined(TIM5)
#define __HAL_RCC_TIM5_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM5LPEN)
#endif /* TIM5 */

#define __HAL_RCC_TIM6_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM6LPEN)

#define __HAL_RCC_TIM7_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM7LPEN)

#if defined(TIM12)
#define __HAL_RCC_TIM12_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM12LPEN)
#endif /* TIM12 */

#if defined(TIM13)
#define __HAL_RCC_TIM13_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM13LPEN)
#endif /* TIM12 */

#if defined(TIM14)
#define __HAL_RCC_TIM14_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_TIM14LPEN)
#endif /* TIM14 */

#define __HAL_RCC_WWDG_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_WWDGLPEN)

#if defined(OPAMP1)
#define __HAL_RCC_OPAMP_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_OPAMPLPEN)
#endif /* OPAMP1 */

#define __HAL_RCC_SPI2_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_SPI2LPEN)

#define __HAL_RCC_SPI3_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_SPI3LPEN)

#if defined(COMP1)
#define __HAL_RCC_COMP_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_COMPLPEN)
#endif /* COMP1 */

#define __HAL_RCC_USART2_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART2LPEN)

#define __HAL_RCC_USART3_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART3LPEN)

#if defined(UART4)
#define __HAL_RCC_UART4_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART4LPEN)
#endif /* UART4 */

#if defined(UART5)
#define __HAL_RCC_UART5_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART5LPEN)
#endif /* UART5 */

#define __HAL_RCC_I2C1_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I2C1LPEN)

#define __HAL_RCC_I2C2_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I2C2LPEN)

#define __HAL_RCC_I3C1_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_I3C1LPEN)

#define __HAL_RCC_CRS_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_CRSLPEN)

#if defined(USART6)
#define __HAL_RCC_USART6_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART6LPEN)
#endif /* USART6 */

#if defined(USART10)
#define __HAL_RCC_USART10_CLK_SLEEP_DISABLE()    CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART10LPEN)
#endif /* USART10 */

#if defined(USART11)
#define __HAL_RCC_USART11_CLK_SLEEP_DISABLE()    CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_USART11LPEN)
#endif /* USART11 */

#if defined(CEC)
#define __HAL_RCC_CEC_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_CECLPEN)
#endif /* CEC */

#if defined(UART7)
#define __HAL_RCC_UART7_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART7LPEN)
#endif /* UART7 */

#if defined(UART8)
#define __HAL_RCC_UART8_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1LLPENR, RCC_APB1LLPENR_UART8LPEN)
#endif /* UART8 */


#if defined(UART9)
#define __HAL_RCC_UART9_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UART9LPEN)
#endif /* UART9 */

#if defined(UART12)
#define __HAL_RCC_UART12_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UART12LPEN)
#endif /* UART12 */

#define __HAL_RCC_DTS_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_DTSLPEN)

#define __HAL_RCC_LPTIM2_CLK_SLEEP_DISABLE()     CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_LPTIM2LPEN)

#define __HAL_RCC_FDCAN_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_FDCANLPEN)

#if defined(UCPD1)
#define __HAL_RCC_UCPD1_CLK_SLEEP_DISABLE()      CLEAR_BIT(RCC->APB1HLPENR, RCC_APB1HLPENR_UCPD1LPEN)
#endif /* UCPD1 */

/**
  * @}
  */

/** @defgroup RCC_APB2_Clock_Sleep_Enable_Disable APB2 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the APB2 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */
#define __HAL_RCC_TIM1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM1LPEN)

#define __HAL_RCC_SPI1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI1LPEN)

#if defined(TIM8)
#define __HAL_RCC_TIM8_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM8LPEN)
#endif /* TIM8 */

#define __HAL_RCC_USART1_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_USART1LPEN)

#if defined(TIM15)
#define __HAL_RCC_TIM15_CLK_SLEEP_ENABLE()          SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM15LPEN)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_CLK_SLEEP_ENABLE()          SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM16LPEN)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_CLK_SLEEP_ENABLE()          SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM17LPEN)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI4LPEN)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI6LPEN)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SAI1LPEN)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SAI2LPEN)
#endif /* SAI2 */

#if defined(LTDC)
#define __HAL_RCC_LTDC_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_LTDCLPEN)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_GFXTIMLPEN)
#endif /* GFXTIM */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_CLK_SLEEP_ENABLE()            SET_BIT(RCC->APB2LPENR, RCC_APB2LPENR_USBLPEN)
#endif /* USB_DRD_FS */

#define __HAL_RCC_TIM1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM1LPEN)

#define __HAL_RCC_SPI1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI1LPEN)

#if defined(TIM8)
#define __HAL_RCC_TIM8_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM8LPEN)
#endif /* TIM8 */

#define __HAL_RCC_USART1_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_USART1LPEN)

#if defined(TIM15)
#define __HAL_RCC_TIM15_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM15LPEN)
#endif /* TIM15 */

#if defined(TIM16)
#define __HAL_RCC_TIM16_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM16LPEN)
#endif /* TIM16 */

#if defined(TIM17)
#define __HAL_RCC_TIM17_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_TIM17LPEN)
#endif /* TIM17 */

#if defined(SPI4)
#define __HAL_RCC_SPI4_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI4LPEN)
#endif /* SPI4 */

#if defined(SPI6)
#define __HAL_RCC_SPI6_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SPI6LPEN)
#endif /* SPI6 */

#if defined(SAI1)
#define __HAL_RCC_SAI1_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SAI1LPEN)
#endif /* SAI1 */

#if defined(SAI2)
#define __HAL_RCC_SAI2_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_SAI2LPEN)
#endif /* SAI2 */


#if defined(LTDC)
#define __HAL_RCC_LTDC_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_LTDCLPEN)
#endif /* LTDC */

#if defined(GFXTIM)
#define __HAL_RCC_GFXTIM_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_GFXTMLPEN)
#endif /* GFXTIM */

#if defined(USB_DRD_FS)
#define __HAL_RCC_USB_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->APB2LPENR, RCC_APB2LPENR_USBLPEN)
#endif /* USB_DRD_FS */

/**
  * @}
  */

/** @defgroup RCC_APB3_Clock_Sleep_Enable_Disable APB3 Peripheral Clock Sleep Enable Disable
  * @brief  Enable or disable the APB3 peripheral clock during Low Power (Sleep) mode.
  * @note   After wakeup from SLEEP mode, the peripheral clock is enabled again.
  * @note   By default, all peripheral clocks are enabled during SLEEP mode.
  * @{
  */
#define __HAL_RCC_SBS_CLK_SLEEP_ENABLE()            SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_SBSLPEN)

#if defined(SPI5)
#define __HAL_RCC_SPI5_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_SPI5LPEN)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_CLK_SLEEP_ENABLE()        SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPUART1LPEN)

#if defined(I2C3)
#define __HAL_RCC_I2C3_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I2C3LPEN)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I2C4LPEN)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I3C2LPEN)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM1LPEN)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM3LPEN)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM4LPEN)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM5LPEN)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_CLK_SLEEP_ENABLE()         SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM6LPEN)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_CLK_SLEEP_ENABLE()           SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_VREFLPEN)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_CLK_SLEEP_ENABLE()            SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_RTCAPBLPEN)

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_CLK_SLEEP_ENABLE()          SET_BIT(RCC->APB3LPENR, RCC_APB3LPENR_PLAY1LPEN)
#endif /* PLAY1 */

#define __HAL_RCC_SBS_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_SBSLPEN)

#if defined(SPI5)
#define __HAL_RCC_SPI5_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_SPI5LPEN)
#endif /* SPI5 */

#define __HAL_RCC_LPUART1_CLK_SLEEP_DISABLE()       CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPUART1LPEN)

#if defined(I2C3)
#define __HAL_RCC_I2C3_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I2C3LPEN)
#endif /* I2C3 */

#if defined(I2C4)
#define __HAL_RCC_I2C4_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I2C4LPEN)
#endif /* I2C4 */

#if defined(I3C2)
#define __HAL_RCC_I3C2_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_I3C2LPEN)
#endif /* I3C2 */

#define __HAL_RCC_LPTIM1_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM1LPEN)

#if defined(LPTIM3)
#define __HAL_RCC_LPTIM3_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM3LPEN)
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define __HAL_RCC_LPTIM4_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM4LPEN)
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define __HAL_RCC_LPTIM5_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM5LPEN)
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define __HAL_RCC_LPTIM6_CLK_SLEEP_DISABLE()        CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_LPTIM6LPEN)
#endif /* LPTIM6 */

#if defined(VREFBUF)
#define __HAL_RCC_VREF_CLK_SLEEP_DISABLE()          CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_VREFLPEN)
#endif /* VREFBUF */

#define __HAL_RCC_RTC_CLK_SLEEP_DISABLE()           CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_RTCAPBLPEN)

#if defined(PLAY1)
#define __HAL_RCC_PLAY1_CLK_SLEEP_DISABLE()         CLEAR_BIT(RCC->APB3LPENR, RCC_APB3LPENR_PLAY1LPEN)
#endif /* PLAY1 */
/**
  * @}
  */


/** @defgroup RCC_Backup_Domain_Reset RCC Backup Domain Reset
  * @{
  */

/** @brief  Macros to force or release the Backup domain reset.
  * @note   This function resets the RTC peripheral (including the backup registers)
  *         and the RTC clock source selection in RCC_BDCR register.
  * @note   The BKPSRAM is not affected by this reset.
  * @retval None
  */
#define __HAL_RCC_BACKUPRESET_FORCE()   SET_BIT(RCC->BDCR, RCC_BDCR_VSWRST)

#define __HAL_RCC_BACKUPRESET_RELEASE() CLEAR_BIT(RCC->BDCR, RCC_BDCR_VSWRST)

/**
  * @}
  */

/** @defgroup RCC_RTC_Clock_Configuration RCC RTC Clock Configuration
  * @{
  */

/** @brief  Macros to enable or disable the RTC clock.
  * @note   As the RTC is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using
  *         HAL_PWR_EnableBkUpAccess() function before to configure the RTC
  *         (to be done once after reset).
  * @note   These macros must be used after the RTC clock source was selected.
  * @retval None
  */
#define __HAL_RCC_RTC_ENABLE()         SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN)

#define __HAL_RCC_RTC_DISABLE()        CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCEN)

/**
  * @}
  */

/** @brief    Macro to configure the Internal High Speed oscillator (HSI).
  * @param    __HSIDIV__ specifies the HSI division factor.
  *           This parameter can be one of the following values:
  *            @arg RCC_HSI_DIV1 Divide the HSI oscillator clock by 1 (default after reset)
  *            @arg RCC_HSI_DIV2 Divide the HSI oscillator clock by 2
  *            @arg RCC_HSI_DIV4 Divide the HSI oscillator clock by 4
  *            @arg RCC_HSI_DIV8 Divide the HSI oscillator clock by 8
  */
#define __HAL_RCC_HSI_DIVIDER_CONFIG(__HSIDIV__) \
  MODIFY_REG(RCC->CR, RCC_CR_HSIDIV , (uint32_t)(__HSIDIV__))


/** @brief  Macro to get the HSI divider.
  * @retval The HSI divider. The returned value can be one
  *         of the following:
  *            - RCC_HSI_DIV1  HSI oscillator divided by 1
  *            - RCC_HSI_DIV2  HSI oscillator divided by 2
  *            - RCC_HSI_DIV4  HSI oscillator divided by 4
  *            - RCC_HSI_DIV8  HSI oscillator divided by 8
  */
#define __HAL_RCC_GET_HSI_DIVIDER() ((uint32_t)(READ_BIT(RCC->CR, RCC_CR_HSIDIV)))

/** @brief  Macros to enable or disable the Internal High Speed 64MHz oscillator (HSI).
  * @note   The HSI is stopped by hardware when entering STOP and STANDBY modes.
  *         It is used (enabled by hardware) as system clock source after startup
  *         from Reset, wakeup from STOP and STANDBY mode, or in case of failure
  *         of the HSE used directly or indirectly as system clock (if the HSE Clock
  *         Security System HSECSS is enabled).
  * @note   HSI can not be stopped if it is used as system clock source. In this case,
  *         you have to select another source of the system clock then stop the HSI.
  * @note   After enabling the HSI, the application software should wait on HSIRDY
  *         flag to be set indicating that HSI clock is stable and can be used as
  *         system clock source.
  *         This parameter can be: ENABLE or DISABLE.
  * @note   When the HSI is stopped, HSIRDY flag goes low after 6 HSI oscillator
  *         clock cycles.
  * @retval None
  */
#define __HAL_RCC_HSI_ENABLE()  SET_BIT(RCC->CR, RCC_CR_HSION)

#define __HAL_RCC_HSI_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_HSION)

/** @brief  Macro to adjust the Internal High Speed 64MHz oscillator (HSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal HSI RC.
  * @param  __HSICALIBRATIONVALUE__: specifies the calibration trimming value
  *         (default is RCC_HSICALIBRATION_DEFAULT).
  *         This parameter must be a number between 0 and 0x7F.
  * @retval None
  */
#define __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(__HSICALIBRATIONVALUE__) \
  MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, (uint32_t)(__HSICALIBRATIONVALUE__) << RCC_HSICFGR_HSITRIM_Pos)

/**
  * @brief    Macros to enable or disable the force of the Internal High Speed oscillator (HSI)
  *           in STOP mode to be quickly available as kernel clock for USARTs, LPUART and I2Cs.
  * @note     Keeping the HSI ON in STOP mode allows to avoid slowing down the communication
  *           speed because of the HSI startup time.
  * @note     The enable of this function has not effect on the HSION bit.
  *           This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
#define __HAL_RCC_HSISTOP_ENABLE()     SET_BIT(RCC->CR, RCC_CR_HSIKERON)

#define __HAL_RCC_HSISTOP_DISABLE()    CLEAR_BIT(RCC->CR, RCC_CR_HSIKERON)

/**
  * @brief  Macros to enable or disable the Internal Low-power oscillator (CSI).
  * @note     The CSI is stopped by hardware when entering STOP and STANDBY modes.
  *           It is used (enabled by hardware) as system clock source after
  *           startup from Reset, wakeup from STOP and STANDBY mode, or in case
  *           of failure of the HSE used directly or indirectly as system clock
  *           (if the HSE Clock Security System HSECSS is enabled and CSI is selected
  *           as system clock after wake up from system stop).
  * @note     CSI can not be stopped if it is used as system clock source.
  *           In this case, you have to select another source of the system
  *           clock then stop the CSI.
  * @note     After enabling the CSI, the application software should wait on
  *           CSIRDY flag to be set indicating that CSI clock is stable and can
  *           be used as system clock source.
  * @note   When the CSI is stopped, CSIRDY flag goes low after 6 CSI oscillator
  *         clock cycles.
  * @retval None
  */
#define __HAL_RCC_CSI_ENABLE()  SET_BIT(RCC->CR, RCC_CR_CSION)

#define __HAL_RCC_CSI_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_CSION)

/** @brief  Macro Adjusts the Internal  oscillator (CSI) calibration value.
  * @note   The calibration is used to compensate for the variations in voltage
  *         and temperature that influence the frequency of the internal CSI RC.
  * @param  __CSICalibrationValue__: specifies the calibration trimming value.
  *         This parameter must be a number between 0 and 0x3F.
  */
#define __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(__CSICalibrationValue__)                                              \
  do {                                                                                                              \
    MODIFY_REG(RCC->CSICFGR, RCC_CSICFGR_CSITRIM, (uint32_t)(__CSICalibrationValue__) << RCC_CSICFGR_CSITRIM_Pos);  \
  } while(0)

/**
  * @brief    Macros to enable or disable the force of the Low-power Internal oscillator (CSI)
  *           in STOP mode to be quickly available as kernel clock for USARTs and I2Cs.
  * @note     Keeping the CSI ON in STOP mode allows to avoid slowing down the communication
  *           speed because of the CSI start-up time.
  * @note     The enable of this function has not effect on the CSION bit.
  *           This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
#define __HAL_RCC_CSISTOP_ENABLE()     SET_BIT(RCC->CR, RCC_CR_CSIKERON)
#define __HAL_RCC_CSISTOP_DISABLE()    CLEAR_BIT(RCC->CR, RCC_CR_CSIKERON)

/** @brief  Macros to enable or disable the Internal Low Speed oscillator (LSI).
  * @note   After enabling the LSI, the application software should wait on
  *         LSIRDY flag to be set indicating that LSI clock is stable and can
  *         be used to clock the IWDG and/or the RTC.
  * @note   LSI can not be disabled if the IWDG is running.
  * @note   When the LSI is stopped, LSIRDY flag goes low after 6 LSI oscillator
  *         clock cycles.
  * @retval None
  */
#define __HAL_RCC_LSI_ENABLE()         SET_BIT(RCC->BDCR, RCC_BDCR_LSION)

#define __HAL_RCC_LSI_DISABLE()        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSION)

/**
  * @brief  Macro to configure the External High Speed oscillator (HSE).
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @note   After enabling the HSE (RCC_HSE_ON or RCC_HSE_Bypass), the application
  *         software should wait on HSERDY flag to be set indicating that HSE clock
  *         is stable and can be used to clock the PLLs and/or system clock.
  * @note   HSE state can not be changed if it is used directly or through the
  *         PLL1 as system clock. In this case, you have to select another source
  *         of the system clock then change the HSE state (ex. disable it).
  * @note   The HSE is stopped by hardware when entering STOP and STANDBY modes.
  * @param  __STATE__: specifies the new state of the HSE.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_HSE_OFF  Turn OFF the HSE oscillator, HSERDY flag goes low after
  *                              6 HSE oscillator clock cycles.
  *            @arg @ref RCC_HSE_ON  Turn ON the HSE oscillator.
  *            @arg @ref RCC_HSE_BYPASS  HSE oscillator bypassed with external clock.
  *            @arg @ref RCC_HSE_BYPASS_DIGITAL HSE oscillator bypassed with digital external clock.
  * @retval None
  */
#define __HAL_RCC_HSE_CONFIG(__STATE__)              \
  do {                                               \
    if ((__STATE__) == RCC_HSE_ON)                   \
    {                                                \
      SET_BIT(RCC->CR, RCC_CR_HSEON);                \
    }                                                \
    else if ((__STATE__) == RCC_HSE_OFF)             \
    {                                                \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEON);              \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEEXT);             \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);             \
    }                                                \
    else if ((__STATE__) == RCC_HSE_BYPASS)          \
    {                                                \
      SET_BIT(RCC->CR, RCC_CR_HSEBYP);               \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEEXT);             \
      SET_BIT(RCC->CR, RCC_CR_HSEON);                \
    }                                                \
    else if((__STATE__) == RCC_HSE_BYPASS_DIGITAL)   \
    {                                                \
      SET_BIT(RCC->CR, RCC_CR_HSEBYP);               \
      SET_BIT(RCC->CR, RCC_CR_HSEEXT);               \
      SET_BIT(RCC->CR, RCC_CR_HSEON);                \
    }                                                \
    else                                             \
    {                                                \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEON);              \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);             \
      CLEAR_BIT(RCC->CR, RCC_CR_HSEEXT);             \
    }                                                \
  } while(0)

/**
  * @brief  Macro to configure the External Low Speed oscillator (LSE).
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this macro. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable write access using
  *         HAL_PWR_EnableBkUpAccess() function before to configure the LSE
  *         (to be done once after reset).
  * @note   After enabling the LSE (RCC_LSE_ON or RCC_LSE_BYPASS), the application
  *         software should wait on LSERDY flag to be set indicating that LSE clock
  *         is stable and can be used to clock the RTC.
  * @param  __STATE__: specifies the new state of the LSE.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_LSE_OFF  Turn OFF the LSE oscillator, LSERDY flag goes low after
  *                              6 LSE oscillator clock cycles.
  *            @arg @ref RCC_LSE_ON  Turn ON the LSE oscillator.
  *            @arg @ref RCC_LSE_BYPASS  LSE oscillator bypassed with external clock.
  *            @arg @ref RCC_LSE_BYPASS_DIGITAL LSE oscillator bypassed with external digital clock.
  * @retval None
  */

#define __HAL_RCC_LSE_CONFIG(__STATE__) \
  do {                                               \
    if((__STATE__) == RCC_LSE_ON)                    \
    {                                                \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);            \
    }                                                \
    else if((__STATE__) == RCC_LSE_OFF)              \
    {                                                \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);          \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEEXT);         \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);         \
    }                                                \
    else if((__STATE__) == RCC_LSE_BYPASS)           \
    {                                                \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);           \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEEXT);         \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);            \
    }                                                \
    else if((__STATE__) == RCC_LSE_BYPASS_DIGITAL)   \
    {                                                \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);           \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEEXT);           \
      SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);            \
    }                                                \
    else                                             \
    {                                                \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);          \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);         \
      CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEEXT);         \
    }                                                \
  } while(0)

/** @brief  Macros to enable or disable the Internal High Speed 48MHz oscillator (HSI48).
  * @note   The HSI48 is stopped by hardware when entering STOP and STANDBY modes.
  * @note   After enabling the HSI48, the application software should wait on HSI48RDY
  *         flag to be set indicating that HSI48 clock is stable.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
#define __HAL_RCC_HSI48_ENABLE()  SET_BIT(RCC->CR, RCC_CR_HSI48ON)

#define __HAL_RCC_HSI48_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_HSI48ON)

/** @brief  Macros to configure the RTC clock (RTCCLK).
  * @note   As the RTC clock configuration bits are in the Backup domain and write
  *         access is denied to this domain after reset, you have to enable write
  *         access using the Power Backup Access macro before to configure
  *         the RTC clock source (to be done once after reset).
  * @note   Once the RTC clock is configured it cannot be changed unless the
  *         Backup domain is reset using __HAL_RCC_BackupReset_RELEASE() macro, or by
  *         a Power On Reset (POR).
  * @param  __RTCCLKSource__: specifies the RTC clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_RTCCLKSOURCE_LSE LSE selected as RTC clock.
  *            @arg @ref RCC_RTCCLKSOURCE_LSI LSI selected as RTC clock.
  *            @arg @ref RCC_RTCCLKSOURCE_HSE_DIVx HSE clock divided by x selected
  *                                            as RTC clock, where x can be between 2 and 63
  * @note   If the LSE or LSI is used as RTC clock source, the RTC continues to
  *         work in STOP and STANDBY modes, and can be used as wakeup source.
  *         However, when the HSE clock is used as RTC clock source, the RTC
  *         cannot be used in STOP and STANDBY modes.
  * @note   The maximum input clock frequency for RTC is 1MHz (when using HSE as
  *         RTC clock source).
  */
#define __HAL_RCC_RTC_CLKPRESCALER(__RTCCLKSource__) (((__RTCCLKSource__) & RCC_BDCR_RTCSEL) == RCC_BDCR_RTCSEL) ?    \
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_RTCPRE, \
             (((__RTCCLKSource__) & 0xFFFFCFFU) >> 4)) : CLEAR_BIT(RCC->CFGR1, RCC_CFGR1_RTCPRE)

#define __HAL_RCC_RTC_CONFIG(__RTCCLKSource__) do { __HAL_RCC_RTC_CLKPRESCALER(__RTCCLKSource__);     \
                                                    RCC->BDCR &= ~RCC_BDCR_RTCSEL;                    \
                                                    RCC->BDCR |= ((__RTCCLKSource__) & 0x00000FFFU);  \
                                                   } while (0)

/** @brief  Macro to get the RTC clock source.
  * @retval The returned value can be one of the following:
  *            @arg @ref RCC_RTCCLKSOURCE_NO_CLK  No clock selected as RTC clock.
  *            @arg @ref RCC_RTCCLKSOURCE_LSE  LSE selected as RTC clock.
  *            @arg @ref RCC_RTCCLKSOURCE_LSI  LSI selected as RTC clock.
  *            @arg @ref RCC_RTCCLKSOURCE_HSE_DIVx HSE clock divided by x selected as
  *                      RTC clock, where x can be between 2 and 63
                         (x can be retrieved with @ref __HAL_RCC_GET_RTC_HSE_PRESCALER())
  */
#define  __HAL_RCC_GET_RTC_SOURCE() ((uint32_t)(READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL)))

/** @brief  Macro to get the HSE division factor for RTC clock.
  *
  * @retval The HSE division factor for RTC clock. The returned value can be one
  *         of the following:
  *               @arg @ref RCC_RTC_HSE_NOCLOCK : No HSE Clock selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV2  : HSE Divided by 2 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV3  : HSE Divided by 3 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV4  : HSE Divided by 4 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV5  : HSE Divided by 5 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV6  : HSE Divided by 6 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV7  : HSE Divided by 7 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV8  : HSE Divided by 8 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV9  : HSE Divided by 9 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV10 : HSE Divided by 10 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV11 : HSE Divided by 11 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV12 : HSE Divided by 12 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV13 : HSE Divided by 13 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV14 : HSE Divided by 14 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV15 : HSE Divided by 15 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV16 : HSE Divided by 16 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV17 : HSE Divided by 17 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV18 : HSE Divided by 18 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV19 : HSE Divided by 19 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV20 : HSE Divided by 20 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV21 : HSE Divided by 21 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV22 : HSE Divided by 22 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV23 : HSE Divided by 23 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV24 : HSE Divided by 24 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV25 : HSE Divided by 25 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV26 : HSE Divided by 26 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV27 : HSE Divided by 27 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV28 : HSE Divided by 28 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV29 : HSE Divided by 29 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV30 : HSE Divided by 30 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV31 : HSE Divided by 31 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV32 : HSE Divided by 32 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV33 : HSE Divided by 33 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV34 : HSE Divided by 34 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV35 : HSE Divided by 35 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV36 : HSE Divided by 36 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV37 : HSE Divided by 37 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV38 : HSE Divided by 38 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV39 : HSE Divided by 39 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV40 : HSE Divided by 40 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV41 : HSE Divided by 41 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV42 : HSE Divided by 42 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV43 : HSE Divided by 43 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV44 : HSE Divided by 44 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV45 : HSE Divided by 45 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV46 : HSE Divided by 46 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV47 : HSE Divided by 47 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV48 : HSE Divided by 48 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV49 : HSE Divided by 49 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV50 : HSE Divided by 50 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV51 : HSE Divided by 51 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV52 : HSE Divided by 52 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV53 : HSE Divided by 53 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV54 : HSE Divided by 54 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV55 : HSE Divided by 55 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV56 : HSE Divided by 56 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV57 : HSE Divided by 57 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV58 : HSE Divided by 58 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV59 : HSE Divided by 59 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV60 : HSE Divided by 60 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV61 : HSE Divided by 61 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV62 : HSE Divided by 62 selected as RTC clock
  *               @arg @ref RCC_RTC_HSE_DIV63 : HSE Divided by 63 selected as RTC clock
  */
#define __HAL_RCC_GET_RTC_HSE_PRESCALER() ((uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_RTCPRE)))

/** @brief  Macros to enable or disable the main PLL.
  * @note   After enabling the main PLL, the application software should wait on
  *         PLLRDY flag to be set indicating that PLL clock is stable and can
  *         be used as system clock source.
  * @note   The main PLL can not be disabled if it is used as system clock source
  * @note   The main PLL is disabled by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCC_PLL1_ENABLE()         SET_BIT(RCC->CR, RCC_CR_PLL1ON)
#define __HAL_RCC_PLL1_DISABLE()        CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON)

/**
  * @brief  Enables or disables each clock output (PLL1P_CLK, PLL1Q_CLK, PLL1R_CLK)
  * @note   Enabling/disabling  Those Clocks can be any time  without the need to stop the PLL1,
  *        (except the ck_pll_p of the System PLL that cannot be stopped if used as System
  *         Clock. This is mainly used to save Power.
  * @param  __PLL1_CLOCKOUT__: specifies the PLL clock to be outputted
  *          This parameter can be one of the following values:
  *            @arg RCC_PLL1_DIVP: This Clock is used to generate the high speed system clock (up to 250MHz)
  *            @arg RCC_PLL1_DIVQ: This Clock is used to generate the clock for USB (48 MHz), RNG (<=48 MHz),
  *                                OCTOSPI, SPI, SAI and Ethernet
  *            @arg RCC_PLL1_DIVR: This Clock is used to generate an accurate clock
  * @retval None
  *
  */
#define __HAL_RCC_PLL1_CLKOUT_ENABLE(__PLL1_CLOCKOUT__)   SET_BIT(RCC->PLL1CFGR, (__PLL1_CLOCKOUT__))

#define __HAL_RCC_PLL1_CLKOUT_DISABLE(__PLL1_CLOCKOUT__)  CLEAR_BIT(RCC->PLL1CFGR, (__PLL1_CLOCKOUT__))

/**
  * @brief  Macro to get the PLL clock output enable status.
  * @param  __PLL1_CLOCKOUT__ specifies the PLL1 clock to be output.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL1_DIVP: This Clock is used to generate the high speed system clock (up to 250MHz)
  *            @arg RCC_PLL1_DIVQ: This Clock is used to generate the clock for USB (48 MHz), RNG (<=48 MHz),
  *                                OCTOSPI, SPI, SAI and Ethernet
  *            @arg RCC_PLL1_DIVR: This Clock is used to generate an accurate clock
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLL1_CLKOUT_CONFIG(__PLL1_CLOCKOUT__)  READ_BIT(RCC->PLL1CFGR, (__PLL1_CLOCKOUT__))

/**
  * @brief  Enables or disables Fractional Part Of The Multiplication Factor of PLL1 VCO
  * @note   Enabling/disabling  Fractional Part can be any time  without the need to stop the PLL1
  * @retval None
  */
#define __HAL_RCC_PLL1_FRACN_ENABLE()   SET_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1FRACEN)

#define __HAL_RCC_PLL1_FRACN_DISABLE()  CLEAR_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1FRACEN)

/**
  * @brief  Macro to configures the main PLL (PLL1) clock source, multiplication and division factors.
  * @note   This function must be used only when the main PLL1 is disabled.
  *
  * @param  __PLL1SOURCE__: specifies the PLL entry clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_PLL1_SOURCE_CSI: CSI oscillator clock selected as PLL1 clock entry
  *            @arg RCC_PLL1_SOURCE_HSI: HSI oscillator clock selected as PLL1 clock entry
  *            @arg RCC_PLL1_SOURCE_HSE: HSE oscillator clock selected as PLL1 clock entry
  * @note   This clock source (__PLL1SOURCE__) is the clock source for PLL1 (main PLL) and is different
            from PLL2 & PLL3 clock sources.
  *
  * @param  __PLL1M__: specifies the division factor for PLL VCO input clock
  *          This parameter must be a number between 1 and 63.
  * @note   You have to set the PLL1M parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 16 MHz.
  *
  * @param  __PLL1N__: specifies the multiplication factor for PLL VCO output clock
  *          This parameter must be a number between 4 and 512.
  * @note   You have to set the PLL1N parameter correctly to ensure that the VCO
  *         output frequency is between 150 and 420 MHz (when in medium VCO range) or
  *         between 192 and 836 MHZ (when in wide VCO range)
  *
  * @param  __PLL1P__: specifies the division factor for system  clock.
  *          This parameter must be a number between 2 and 128 (where odd numbers not allowed)
  *
  * @param  __PLL1Q__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @param  __PLL1R__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @retval None
  */
#define __HAL_RCC_PLL1_CONFIG(__PLL1SOURCE__, __PLL1M__, __PLL1N__, __PLL1P__, __PLL1Q__, __PLL1R__) \
  do{ MODIFY_REG(RCC->PLL1CFGR, (RCC_PLL1CFGR_PLL1SRC | RCC_PLL1CFGR_PLL1M), \
                   ((__PLL1SOURCE__) << RCC_PLL1CFGR_PLL1SRC_Pos) | ((__PLL1M__) << RCC_PLL1CFGR_PLL1M_Pos));\
    WRITE_REG(RCC->PLL1DIVR , ( (((__PLL1N__) - 1U ) & RCC_PLL1DIVR_PLL1N) | \
                                ((((__PLL1P__) - 1U ) << RCC_PLL1DIVR_PLL1P_Pos) & RCC_PLL1DIVR_PLL1P) | \
                                ((((__PLL1Q__) - 1U) << RCC_PLL1DIVR_PLL1Q_Pos) & RCC_PLL1DIVR_PLL1Q) | \
                                ((((__PLL1R__) - 1U) << RCC_PLL1DIVR_PLL1R_Pos) & RCC_PLL1DIVR_PLL1R))); \
  } while(0)

/** @brief  Macro to configure the PLL1 clock source.
  * @note   This function must be used only when PLL1 is disabled.
  * @param  __PLL1SOURCE__: specifies the PLL1 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL1_SOURCE_CSI: CSI oscillator clock selected as PLL1 clock entry
  *            @arg RCC_PLL1_SOURCE_HSI: HSI oscillator clock selected as PLL1 clock entry
  *            @arg RCC_PLL1_SOURCE_HSE: HSE oscillator clock selected as PLL1 clock entry
  *
  */
#define __HAL_RCC_PLL1_PLLSOURCE_CONFIG(__PLL1SOURCE__) \
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1SRC, (__PLL1SOURCE__))

/** @brief  Macro to configure the PLL1 input clock division factor M.
  *
  * @note   This function must be used only when the PLL1 is disabled.
  * @note   PLL1 clock source is common with the main PLL (configured through
  *         __HAL_RCC_PLL1_CONFIG() macro)
  *
  * @param  __PLL1M__ specifies the division factor for PLL1 clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 63.
  *         In order to save power when PLL1 is not used, the value of PLL1M must be set to 0.
  *
  * @retval None
  */
#define __HAL_RCC_PLL1_DIVM_CONFIG(__PLL1M__) \
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1M, (__PLL1M__) << RCC_PLL1CFGR_PLL1M_Pos)

/**
  * @brief  Macro to configures the main PLL clock Fractional Part Of The Multiplication Factor
  *
  * @note   These bits can be written at any time, allowing dynamic fine-tuning of the PLL1 VCO
  *
  * @param  __PLL1FRACN__: specifies Fractional Part Of The Multiplication Factor for PLL1 VCO
  *                            It should be a value between 0 and 8191
  * @note   Warning: The software has to set correctly these bits to insure that the VCO
  *                  output frequency is between its valid frequency range, which is:
  *                   192 to 836 MHz if PLL1VCOSEL = 0
  *                   150 to 420 MHz if PLL1VCOSEL = 1.
  *
  *
  * @retval None
  */
#define  __HAL_RCC_PLL1_FRACN_CONFIG(__PLL1FRACN__) WRITE_REG(RCC->PLL1FRACR, \
                                                              (uint32_t)(__PLL1FRACN__) << RCC_PLL1FRACR_PLL1FRACN_Pos)

/** @brief  Macro to select the PLL1 reference frequency range.
  * @param  __PLL1VCIRange__: specifies the PLL1 input frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL1_VCIRANGE_0: Range frequency is between 1 and 2 MHz
  *            @arg RCC_PLL1_VCIRANGE_1: Range frequency is between 2 and 4 MHz
  *            @arg RCC_PLL1_VCIRANGE_2: Range frequency is between 4 and 8 MHz
  *            @arg RCC_PLL1_VCIRANGE_3: Range frequency is between 8 and 16 MHz
  * @retval None
  */
#define __HAL_RCC_PLL1_VCIRANGE(__PLL1VCIRange__) \
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1RGE, (__PLL1VCIRange__))

/** @brief  Macro to select the PLL1 reference frequency range.
  * @param  __RCC_PLL1VCORange__: specifies the PLL1 input frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL1_VCORANGE_WIDE: Range frequency is between 192 and 836 MHz
  *            @arg RCC_PLL1_VCORANGE_MEDIUM: Range frequency is between 150 and 420 MHz
  *
  *
  * @retval None
  */
#define __HAL_RCC_PLL1_VCORANGE(__RCC_PLL1VCORange__) \
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1VCOSEL, (__RCC_PLL1VCORange__))

/** @brief  Macro to get the oscillator used as PLL1 clock source.
  * @retval The oscillator used as PLL1 clock source. The returned value can be one
  *         of the following:
  *              - RCC_PLL1_SOURCE_NONE: No oscillator is used as PLL clock source.
  *              - RCC_PLL1_SOURCE_CSI: CSI oscillator is used as PLL clock source.
  *              - RCC_PLL1_SOURCE_HSI: HSI oscillator is used as PLL clock source.
  *              - RCC_PLL1_SOURCE_HSE: HSE oscillator is used as PLL clock source.
  */
#define __HAL_RCC_GET_PLL1_OSCSOURCE() ((uint32_t)(RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1SRC))

/**
  * @brief  Macro to configure the system clock source.
  * @param  __SYSCLKSOURCE__: specifies the system clock source.
  *          This parameter can be one of the following values:
  *              - RCC_SYSCLKSOURCE_CSI: CSI oscillator is used as system clock source.
  *              - RCC_SYSCLKSOURCE_HSI: HSI oscillator is used as system clock source.
  *              - RCC_SYSCLKSOURCE_HSE: HSE oscillator is used as system clock source.
  *              - RCC_SYSCLKSOURCE_PLL1CLK: PLL1P output is used as system clock source.
  * @retval None
  */
#define __HAL_RCC_SYSCLK_CONFIG(__SYSCLKSOURCE__) \
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_SW, (__SYSCLKSOURCE__))

/** @brief  Macro to get the clock source used as system clock.
  * @retval The clock source used as system clock. The returned value can be one
  *         of the following:
  *              - RCC_SYSCLKSOURCE_STATUS_CSI: CSI used as system clock.
  *              - RCC_SYSCLKSOURCE_STATUS_HSI: HSI used as system clock.
  *              - RCC_SYSCLKSOURCE_STATUS_HSE: HSE used as system clock.
  *              - RCC_SYSCLKSOURCE_STATUS_PLL1CLK: PLL1P used as system clock.
  */
#define __HAL_RCC_GET_SYSCLK_SOURCE() ((uint32_t)(RCC->CFGR1 & RCC_CFGR1_SWS))

/**
  * @brief  Macro to configure the External Low Speed oscillator (LSE) drive capability.
  * @note   As the LSE is in the Backup domain and write access is denied to
  *         this domain after reset, you have to enable the write access using
  *         HAL_PWR_EnableBkUpAccess() function before to configure the LSE
  *         (to be done once after reset).
  * @param  __LSEDRIVE__: specifies the new state of the LSE drive capability.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LSEDRIVE_LOW  LSE oscillator low drive capability. (*)
  *            @arg @ref RCC_LSEDRIVE_MEDIUMLOW  LSE oscillator medium low drive capability.
  *            @arg @ref RCC_LSEDRIVE_MEDIUMHIGH  LSE oscillator medium high drive capability.
  *            @arg @ref RCC_LSEDRIVE_HIGH  LSE oscillator high drive capability.
  * @retval None
  *
  * (*): Not available For stm32h5exxx and stm32h5fxxx family lines.
  */
#define __HAL_RCC_LSEDRIVE_CONFIG(__LSEDRIVE__) \
  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, (uint32_t)(__LSEDRIVE__))

/**
  * @brief  Macro to configure the wake up from stop clock.
  * @note   The configured clock is also used as emergency clock for the Clock Security System on HSE (HSECSS).
  * @param  __STOPWUCLK__: specifies the clock source used after wake up from stop.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_STOP_WAKEUPCLOCK_HSI  HSI selected as system clock source
  *            @arg @ref RCC_STOP_WAKEUPCLOCK_CSI  CSI selected as system clock source
  * @retval None
  */
#define __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(__STOPWUCLK__) \
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_STOPWUCK, (__STOPWUCLK__))

#define __HAL_RCC_HSECSS_RECOVCLK_CONFIG __HAL_RCC_WAKEUPSTOP_CLK_CONFIG

/**
  * @brief  Macro to configure the Kernel wake up from stop clock.
  * @param  __RCC_STOPKERWUCLK__: specifies the Kernel clock source used after wake up from stop
  *         This parameter can be one of the following values:
  *            @arg RCC_STOP_KERWAKEUPCLOCK_CSI: CSI selected as Kernel clock source
  *            @arg RCC_STOP_KERWAKEUPCLOCK_HSI: HSI selected as Kernel clock source
  * @retval None
  */
#define __HAL_RCC_KERWAKEUPSTOP_CLK_CONFIG(__RCC_STOPKERWUCLK__) \
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_STOPKERWUCK, (__RCC_STOPKERWUCLK__))

/** @defgroup RCCEx_MCOx_Clock_Config RCC Extended MCOx Clock Config
  * @{
  */

/** @brief  Macro to configure the MCO1 clock.
  * @param  __MCOCLKSOURCE__ specifies the MCO1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCO1SOURCE_HSI: HSI clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_LSE: LSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_HSE: HSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_PLL1QCLK:  PLL1Q clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_HSI48: HSI48 (48MHZ) selected as MCO1 source
  * @param  __MCODIV__ specifies the MCO clock prescaler.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCODIV_1 up to RCC_MCODIV_15  : divider applied to MCO1 clock
  */
#define __HAL_RCC_MCO1_CONFIG(__MCOCLKSOURCE__, __MCODIV__) \
  MODIFY_REG(RCC->CFGR1, (RCC_CFGR1_MCO1SEL | RCC_CFGR1_MCO1PRE), ((__MCOCLKSOURCE__) | (__MCODIV__)))

/** @brief  Macro to configure the MCO2 clock.
  * @param  __MCOCLKSOURCE__ specifies the MCO2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCO2SOURCE_SYSCLK: System clock (SYSCLK) selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLL2PCLK: PLL2P clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_HSE: HSE clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLL1PCLK:  PLL1P clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_CSI:  CSI clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_LSI:  LSI clock selected as MCO2 source
  * @param  __MCODIV__ specifies the MCO clock prescaler.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCODIV_1 up to RCC_MCODIV_15  : divider applied to MCO2 clock
  */
#define __HAL_RCC_MCO2_CONFIG(__MCOCLKSOURCE__, __MCODIV__) \
  MODIFY_REG(RCC->CFGR1, (RCC_CFGR1_MCO2SEL | RCC_CFGR1_MCO2PRE), ((__MCOCLKSOURCE__) | ((__MCODIV__) << 7)));

/**
  * @}
  */

/** @defgroup RCC_Flags_Interrupts_Management Flags Interrupts Management
  * @brief macros to manage the specified RCC Flags and interrupts.
  * @{
  */

/** @brief  Enable RCC interrupt (Perform access to RCC_CIER[8:0] bits to enable
  *         the selected interrupts).
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be enabled.
  *         This parameter can be any combination of the following values:
  *            @arg @ref RCC_IT_LSIRDY  LSI ready interrupt
  *            @arg @ref RCC_IT_LSERDY  LSE ready interrupt
  *            @arg @ref RCC_IT_CSIRDY  CSI ready interrupt
  *            @arg @ref RCC_IT_HSIRDY  HSI ready interrupt
  *            @arg @ref RCC_IT_HSERDY  HSE ready interrupt
  *            @arg @ref RCC_IT_PLL1RDY  Main PLL ready interrupt
  *            @arg @ref RCC_IT_PLL2RDY  PLL2 ready interrupt
  *            @arg @ref RCC_IT_PLL3RDY  PLL3 ready interrupt (*)
  *            @arg @ref RCC_IT_HSECSS  HSE Clock security system interrupt
  *            @arg @ref RCC_IT_HSI48RDY  HSI48 ready interrupt
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_ENABLE_IT(__INTERRUPT__) SET_BIT(RCC->CIER, (__INTERRUPT__))

/** @brief Disable RCC interrupt (Perform access to RCC_CIER[8:0] bits to disable
  *        the selected interrupts).
  * @param  __INTERRUPT__: specifies the RCC interrupt sources to be disabled.
  *         This parameter can be any combination of the following values:
  *            @arg @ref RCC_IT_LSIRDY  LSI ready interrupt
  *            @arg @ref RCC_IT_LSERDY  LSE ready interrupt
  *            @arg @ref RCC_IT_CSIRDY  CSI ready interrupt
  *            @arg @ref RCC_IT_HSIRDY  HSI ready interrupt
  *            @arg @ref RCC_IT_HSERDY  HSE ready interrupt
  *            @arg @ref RCC_IT_PLL1RDY  Main PLL ready interrupt
  *            @arg @ref RCC_IT_PLL2RDY  PLL2 ready interrupt
  *            @arg @ref RCC_IT_PLL3RDY  PLL3 ready interrupt (*)
  *            @arg @ref RCC_IT_HSECSS  HSE Clock security system interrupt
  *            @arg @ref RCC_IT_HSI48RDY  HSI48 ready interrupt
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_DISABLE_IT(__INTERRUPT__) CLEAR_BIT(RCC->CIER, (__INTERRUPT__))

/** @brief  Clear the RCC's interrupt pending bits (Perform Byte access to RCC_CICR[10:0]
  *         bits to clear the selected interrupt pending bits.
  * @param  __INTERRUPT__: specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *            @arg @ref RCC_IT_LSIRDY  LSI ready interrupt
  *            @arg @ref RCC_IT_LSERDY  LSE ready interrupt
  *            @arg @ref RCC_IT_CSIRDY  CSI ready interrupt
  *            @arg @ref RCC_IT_HSIRDY  HSI ready interrupt
  *            @arg @ref RCC_IT_HSERDY  HSE ready interrupt
  *            @arg @ref RCC_IT_PLL1RDY  Main PLL ready interrupt
  *            @arg @ref RCC_IT_PLL2RDY  PLL2 ready interrupt
  *            @arg @ref RCC_IT_PLL3RDY  PLL3 ready interrupt (*)
  *            @arg @ref RCC_IT_HSECSS  HSE Clock security system interrupt
  *            @arg @ref RCC_IT_HSI48RDY  HSI48 ready interrupt
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_CLEAR_IT(__INTERRUPT__) WRITE_REG(RCC->CICR, (__INTERRUPT__))

/** @brief  Check whether the RCC interrupt has occurred or not.
  * @param  __INTERRUPT__: specifies the RCC interrupt source to check.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_IT_LSIRDY  LSI ready interrupt
  *            @arg @ref RCC_IT_LSERDY  LSE ready interrupt
  *            @arg @ref RCC_IT_CSIRDY  CSI ready interrupt
  *            @arg @ref RCC_IT_HSIRDY  HSI ready interrupt
  *            @arg @ref RCC_IT_HSERDY  HSE ready interrupt
  *            @arg @ref RCC_IT_PLL1RDY  Main PLL ready interrupt
  *            @arg @ref RCC_IT_PLL2RDY  PLL2 ready interrupt
  *            @arg @ref RCC_IT_PLL3RDY  PLL3 ready interrupt (*)
  *            @arg @ref RCC_IT_HSECSS  HSE Clock security system interrupt
  *            @arg @ref RCC_IT_HSI48RDY HSI48 ready interrupt
  * @retval The new state of __INTERRUPT__ (TRUE or FALSE).
   *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_IT(__INTERRUPT__)  ((RCC->CIFR & (__INTERRUPT__)) == (__INTERRUPT__))

/** @brief Set RMVF bit to clear the reset flags.
  *        The reset flags are: RCC_FLAG_SFTRST, RCC_FLAG_PINRST, RCC_FLAG_BORRST,
  *        RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST and RCC_FLAG_LPWRRST.
  * @retval None
 */
#define __HAL_RCC_CLEAR_RESET_FLAGS() (RCC->RSR |= RCC_RSR_RMVF)

/** @brief  Check whether the selected RCC flag is set or not.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_FLAG_CSIRDY  CSI oscillator clock ready
  *            @arg @ref RCC_FLAG_HSIRDY  HSI oscillator clock ready
  *            @arg @ref RCC_FLAG_HSERDY  HSE oscillator clock ready
  *            @arg @ref RCC_FLAG_PLL1RDY  Main PLL1 clock ready
  *            @arg @ref RCC_FLAG_PLL2RDY  PLL2 clock ready
  *            @arg @ref RCC_FLAG_PLL3RDY  PLL3 clock ready (*)
  *            @arg @ref RCC_FLAG_HSI48RDY  HSI48 clock ready
  *            @arg @ref RCC_FLAG_LSERDY  LSE oscillator clock ready
  *            @arg @ref RCC_FLAG_LSECSSD  Clock security system failure on LSE oscillator detection
  *            @arg @ref RCC_FLAG_LSIRDY  LSI oscillator clock ready
  *            @arg @ref RCC_FLAG_HSIDIVF  HSI Divider
  *            @arg @ref RCC_FLAG_BORRST  BOR reset
  *            @arg @ref RCC_FLAG_PINRST  Pin reset
  *            @arg @ref RCC_FLAG_RMVF  Remove reset Flag
  *            @arg @ref RCC_FLAG_SFTRST  Software reset
  *            @arg @ref RCC_FLAG_IWDGRST  Independent Watchdog reset
  *            @arg @ref RCC_FLAG_WWDGRST  Window Watchdog reset
  *            @arg @ref RCC_FLAG_LPWRRST  Low Power reset
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_FLAG(__FLAG__) (((((((__FLAG__) >> 5U) == 1U) ? RCC->CR :                    \
                                         ((((__FLAG__) >> 5U) == 2U) ? RCC->BDCR :                 \
                                          ((((__FLAG__) >> 5U) == 3U) ? RCC->RSR : RCC->CIFR))) &  \
                                        (1UL << ((__FLAG__) & RCC_FLAG_MASK))) != 0U) ? 1U : 0U)
/**
  * @}
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup RCC_Private_Constants RCC Private Constants
  * @{
  */

/** @defgroup RCC_Timeout_Value Timeout Values
  * @{
  */
#define RCC_HSE_TIMEOUT_VALUE          HSE_STARTUP_TIMEOUT
#define RCC_HSI_TIMEOUT_VALUE          ((uint32_t)2U)        /* 2 ms (minimum Tick + 1) */
#define RCC_CSI_TIMEOUT_VALUE          ((uint32_t)2U)        /* 2 ms (minimum Tick + 1) */
#define RCC_DBP_TIMEOUT_VALUE          (2U)                  /* 2 ms (minimum Tick + 1) */
#define RCC_LSE_TIMEOUT_VALUE          LSE_STARTUP_TIMEOUT
/**
  * @}
  */

/* Defines used for Flags */
#define RCC_CR_REG_INDEX              (1U)
#define RCC_BDCR_REG_INDEX            (2U)
#define RCC_RSR_REG_INDEX             (3U)

#define RCC_FLAG_MASK             (0x1FU)

/* Defines Oscillator Masks */
#define RCC_OSCILLATORTYPE_ALL    (RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSI48 | \
                                   RCC_OSCILLATORTYPE_CSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE)
/*!< All Oscillator to configure */


/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCC_Private_Macros
  * @{
  */

#define IS_RCC_OSCILLATORTYPE(__OSCILLATOR__)  (((__OSCILLATOR__) == RCC_OSCILLATORTYPE_NONE) || \
                                                (((__OSCILLATOR__) & ~RCC_OSCILLATORTYPE_ALL) == 0x00U))


#define IS_RCC_HSE(__HSE__)  (((__HSE__) == RCC_HSE_OFF)    || ((__HSE__) == RCC_HSE_ON) || \
                              ((__HSE__) == RCC_HSE_BYPASS) || ((__HSE__) == RCC_HSE_BYPASS_DIGITAL))

#define IS_RCC_LSE(__LSE__)  (((__LSE__) == RCC_LSE_OFF)    || ((__LSE__) == RCC_LSE_ON) || \
                              ((__LSE__) == RCC_LSE_BYPASS) || ((__LSE__) == RCC_LSE_BYPASS_DIGITAL))

#define IS_RCC_HSI(__HSI__)  (((__HSI__) == RCC_HSI_OFF)  || ((__HSI__) == RCC_HSI_ON))

#define IS_RCC_HSIDIV(__DIV__) (((__DIV__) == RCC_HSI_DIV1)  || ((__DIV__) == RCC_HSI_DIV2) || \
                                ((__DIV__) == RCC_HSI_DIV4)  || ((__DIV__) == RCC_HSI_DIV8))

#define IS_RCC_HSI_CALIBRATION_VALUE(__VALUE__) ((__VALUE__) \
                                                 <= (uint32_t)( RCC_HSICFGR_HSITRIM  >>  RCC_HSICFGR_HSITRIM_Pos))

#define IS_RCC_LSI(__LSI__)  (((__LSI__) == RCC_LSI_OFF) || ((__LSI__) == RCC_LSI_ON))

#define IS_RCC_CSI(__CSI__)  (((__CSI__) == RCC_CSI_OFF) || ((__CSI__) == RCC_CSI_ON))

#define IS_RCC_CSICALIBRATION_VALUE(__VALUE__) ((__VALUE__) \
                                                <= (uint32_t)( RCC_CSICFGR_CSITRIM  >>  RCC_CSICFGR_CSITRIM_Pos))

#define IS_RCC_HSI48(__HSI48__)  (((__HSI48__) == RCC_HSI48_OFF) || ((__HSI48__) == RCC_HSI48_ON))

#define IS_RCC_PLL(PLL) (((PLL) == RCC_PLL_NONE) ||((PLL) == RCC_PLL_OFF) || \
                         ((PLL) == RCC_PLL_ON))

#define IS_RCC_PLL1_SOURCE(SOURCE) (((SOURCE) == RCC_PLL1_SOURCE_CSI)  || \
                                    ((SOURCE) == RCC_PLL1_SOURCE_HSI)  || \
                                    ((SOURCE) == RCC_PLL1_SOURCE_HSE))

#define IS_RCC_PLL1_DIVM_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 63U))
#define IS_RCC_PLL1_MULN_VALUE(VALUE) ((4U <= (VALUE)) && ((VALUE) <= 512U))
#define IS_RCC_PLL1_DIVP_VALUE(VALUE) ((2U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL1_DIVQ_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL1_DIVR_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))

#define IS_RCC_PLL1_CLOCKOUT_VALUE(VALUE) (((VALUE) == RCC_PLL1_DIVP) || \
                                           ((VALUE) == RCC_PLL1_DIVQ) || \
                                           ((VALUE) == RCC_PLL1_DIVR))

#define IS_RCC_PLL1_VCIRGE_VALUE(VALUE) (((VALUE) == RCC_PLL1_VCIRANGE_0) || \
                                         ((VALUE) == RCC_PLL1_VCIRANGE_1) || \
                                         ((VALUE) == RCC_PLL1_VCIRANGE_2) || \
                                         ((VALUE) == RCC_PLL1_VCIRANGE_3))

#define IS_RCC_PLL1_VCORGE_VALUE(VALUE) (((VALUE) == RCC_PLL1_VCORANGE_WIDE) || ((VALUE) == RCC_PLL1_VCORANGE_MEDIUM))

#define IS_RCC_PLL1_FRACN_VALUE(VALUE) ((VALUE) <= 8191U)

#define IS_RCC_CLOCKTYPE(CLK) ((1U <= (CLK)) && ((CLK) <= 0x1FU))

#define IS_RCC_SYSCLKSOURCE(__SOURCE__) (((__SOURCE__) == RCC_SYSCLKSOURCE_CSI) || \
                                         ((__SOURCE__) == RCC_SYSCLKSOURCE_HSI) || \
                                         ((__SOURCE__) == RCC_SYSCLKSOURCE_HSE) || \
                                         ((__SOURCE__) == RCC_SYSCLKSOURCE_PLLCLK))

#define IS_RCC_HCLK(__HCLK__) (((__HCLK__) == RCC_SYSCLK_DIV1)   || ((__HCLK__) == RCC_SYSCLK_DIV2)   || \
                               ((__HCLK__) == RCC_SYSCLK_DIV4)   || ((__HCLK__) == RCC_SYSCLK_DIV8)   || \
                               ((__HCLK__) == RCC_SYSCLK_DIV16)  || ((__HCLK__) == RCC_SYSCLK_DIV64)  || \
                               ((__HCLK__) == RCC_SYSCLK_DIV128) || ((__HCLK__) == RCC_SYSCLK_DIV256) || \
                               ((__HCLK__) == RCC_SYSCLK_DIV512))

#define IS_RCC_PCLK(__PCLK__) (((__PCLK__) == RCC_HCLK_DIV1) || ((__PCLK__) == RCC_HCLK_DIV2) || \
                               ((__PCLK__) == RCC_HCLK_DIV4) || ((__PCLK__) == RCC_HCLK_DIV8) || \
                               ((__PCLK__) == RCC_HCLK_DIV16))

#define IS_RCC_RTCCLKSOURCE(SOURCE) \
  (((SOURCE) == RCC_RTCCLKSOURCE_LSE)      || ((SOURCE) == RCC_RTCCLKSOURCE_LSI)       || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV2)  || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV3)  || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV4)  || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV5)  || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV6)  || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV7)  || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV8)  || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV9)  || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV10) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV11) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV12) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV13) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV14) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV15) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV16) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV17) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV18) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV19) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV20) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV21) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV22) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV23) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV24) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV25) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV26) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV27) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV28) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV29) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV30) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV31) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV32) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV33) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV34) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV35) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV36) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV37) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV38) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV39) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV40) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV41) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV42) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV43) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV44) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV45) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV46) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV47) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV48) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV49) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV50) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV51) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV52) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV53) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV54) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV55) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV56) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV57) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV58) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV59) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV60) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV61) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV62) || ((SOURCE) == RCC_RTCCLKSOURCE_HSE_DIV63) || \
   ((SOURCE) == RCC_RTCCLKSOURCE_NO_CLK))

#define IS_RCC_MCO(MCOx) (((MCOx) == RCC_MCO1) || ((MCOx) == RCC_MCO2))

#define IS_RCC_MCO1SOURCE(SOURCE) (((SOURCE) == RCC_MCO1SOURCE_HSI) || ((SOURCE) == RCC_MCO1SOURCE_LSE)    || \
                                   ((SOURCE) == RCC_MCO1SOURCE_HSE) || ((SOURCE) == RCC_MCO1SOURCE_PLL1Q)  || \
                                   ((SOURCE) == RCC_MCO1SOURCE_HSI48))

#define IS_RCC_MCO2SOURCE(SOURCE) (((SOURCE) == RCC_MCO2SOURCE_SYSCLK) || ((SOURCE) == RCC_MCO2SOURCE_PLL2P) || \
                                   ((SOURCE) == RCC_MCO2SOURCE_HSE)    || ((SOURCE) == RCC_MCO2SOURCE_PLL1P) || \
                                   ((SOURCE) == RCC_MCO2SOURCE_CSI)    || ((SOURCE) == RCC_MCO2SOURCE_LSI))

#define IS_RCC_MCODIV(DIV) (((DIV) == RCC_MCODIV_1)  || ((DIV) == RCC_MCODIV_2)   || \
                            ((DIV) == RCC_MCODIV_3)  || ((DIV) == RCC_MCODIV_4)   || \
                            ((DIV) == RCC_MCODIV_5)  || ((DIV) == RCC_MCODIV_6)   || \
                            ((DIV) == RCC_MCODIV_7)  || ((DIV) == RCC_MCODIV_8)   || \
                            ((DIV) == RCC_MCODIV_9)  || ((DIV) == RCC_MCODIV_10)  || \
                            ((DIV) == RCC_MCODIV_11) || ((DIV) == RCC_MCODIV_12)  || \
                            ((DIV) == RCC_MCODIV_13) || ((DIV) == RCC_MCODIV_14)  || \
                            ((DIV) == RCC_MCODIV_15))

#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
#define IS_RCC_LSE_DRIVE(__DRIVE__) (((__DRIVE__) == RCC_LSEDRIVE_MEDIUMLOW)  || \
                                     ((__DRIVE__) == RCC_LSEDRIVE_MEDIUMHIGH) || \
                                     ((__DRIVE__) == RCC_LSEDRIVE_HIGH))
#else
#define IS_RCC_LSE_DRIVE(__DRIVE__) (((__DRIVE__) == RCC_LSEDRIVE_MEDIUMLOW)  || \
                                     ((__DRIVE__) == RCC_LSEDRIVE_LOW)        || \
                                     ((__DRIVE__) == RCC_LSEDRIVE_MEDIUMHIGH) || \
                                     ((__DRIVE__) == RCC_LSEDRIVE_HIGH))
#endif /* STM32H5E5xx || STM32H5E4xx || !STM32H5F5xx || STM32H5F4xx */

#define IS_RCC_STOP_WAKEUPCLOCK(__SOURCE__) (((__SOURCE__) == RCC_STOP_WAKEUPCLOCK_CSI) || \
                                             ((__SOURCE__) == RCC_STOP_WAKEUPCLOCK_HSI))

#define IS_RCC_STOP_KERWAKEUPCLOCK(SOURCE) (((SOURCE) == RCC_STOP_KERWAKEUPCLOCK_CSI) || \
                                            ((SOURCE) == RCC_STOP_KERWAKEUPCLOCK_HSI))

#if defined(RCC_SECCFGR_HSISEC)

#define IS_RCC_ITEM_ATTRIBUTES(ITEM)  ((((ITEM) & RCC_ALL) != 0U) && (((ITEM) & ~RCC_ALL) == 0U))

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_SINGLE_ITEM_ATTRIBUTES(ITEM) (((ITEM) == RCC_HSI)    || \
                                             ((ITEM) == RCC_HSE)    || \
                                             ((ITEM) == RCC_CSI)    || \
                                             ((ITEM) == RCC_LSI)    || \
                                             ((ITEM) == RCC_LSE)    || \
                                             ((ITEM) == RCC_SYSCLK) || \
                                             ((ITEM) == RCC_PRESC)  || \
                                             ((ITEM) == RCC_PLL1)   || \
                                             ((ITEM) == RCC_PLL2)   || \
                                             ((ITEM) == RCC_PLL3)   || \
                                             ((ITEM) == RCC_HSI48)  || \
                                             ((ITEM) == RCC_RMVF)   || \
                                             ((ITEM) == RCC_CKPERSEL))
#else
#define IS_RCC_SINGLE_ITEM_ATTRIBUTES(ITEM) (((ITEM) == RCC_HSI)    || \
                                             ((ITEM) == RCC_HSE)    || \
                                             ((ITEM) == RCC_CSI)    || \
                                             ((ITEM) == RCC_LSI)    || \
                                             ((ITEM) == RCC_LSE)    || \
                                             ((ITEM) == RCC_SYSCLK) || \
                                             ((ITEM) == RCC_PRESC)  || \
                                             ((ITEM) == RCC_PLL1)   || \
                                             ((ITEM) == RCC_PLL2)   || \
                                             ((ITEM) == RCC_HSI48)  || \
                                             ((ITEM) == RCC_RMVF)   || \
                                             ((ITEM) == RCC_CKPERSEL))	
#endif /* RCC_CR_PLL3ON */
#endif /* RCC_SECCFGR_HSISEC */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_RCC_ATTRIBUTES(ATTRIBUTES) (((ATTRIBUTES) == RCC_SEC_PRIV)  || \
                                       ((ATTRIBUTES)  == RCC_SEC_NPRIV) || \
                                       ((ATTRIBUTES)  == RCC_NSEC_PRIV) || \
                                       ((ATTRIBUTES)  == RCC_NSEC_NPRIV))
#elif defined(RCC_PRIVCFGR_NSPRIV)
#define IS_RCC_ATTRIBUTES(ATTRIBUTES) (((ATTRIBUTES) == RCC_NSEC_NPRIV) || ((ATTRIBUTES) == RCC_NSEC_PRIV))
#else
#define IS_RCC_ATTRIBUTES(ATTRIBUTES) (((ATTRIBUTES) == RCC_NPRIV) || ((ATTRIBUTES) == RCC_PRIV))
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/* Include RCC HAL Extended module */
#include "stm32h5xx_hal_rcc_ex.h"

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCC_Exported_Functions
  * @{
  */

/** @addtogroup RCC_Exported_Functions_Group1
  * @{
  */

/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef HAL_RCC_DeInit(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(const RCC_OscInitTypeDef *pOscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(const RCC_ClkInitTypeDef *pClkInitStruct, uint32_t FLatency);

/**
  * @}
  */

/** @addtogroup RCC_Exported_Functions_Group2
  * @{
  */

/* Peripheral Control functions  **********************************************/
void              HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void              HAL_RCC_EnableCSS(void);
uint32_t          HAL_RCC_GetSysClockFreq(void);
uint32_t          HAL_RCC_GetHCLKFreq(void);
uint32_t          HAL_RCC_GetPCLK1Freq(void);
uint32_t          HAL_RCC_GetPCLK2Freq(void);
uint32_t          HAL_RCC_GetPCLK3Freq(void);
void              HAL_RCC_GetOscConfig(RCC_OscInitTypeDef *pOscInitStruct);
void              HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef *pClkInitStruct, uint32_t *pFLatency);
/* CSS NMI IRQ handler */
void              HAL_RCC_NMI_IRQHandler(void);
/* User Callbacks in non blocking mode (IT mode) */
void              HAL_RCC_CSSCallback(void);
uint32_t          HAL_RCC_GetResetSource(void);

/**
  * @}
  */

/** @addtogroup RCC_Exported_Functions_Group3
  * @{
  */

/* Attributes management functions ********************************************/
void              HAL_RCC_ConfigAttributes(uint32_t Item, uint32_t Attributes);
HAL_StatusTypeDef HAL_RCC_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_HAL_RCC_H */

