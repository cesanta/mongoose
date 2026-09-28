/**
  ******************************************************************************
  * @file    stm32h5xx_ll_rcc.h
  * @author  MCD Application Team
  * @brief   Header file of RCC LL module.
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
#ifndef __STM32H5xx_LL_RCC_H
#define __STM32H5xx_LL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

#if defined(RCC)

/** @defgroup RCC_LL RCC
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup RCC_LL_Private_Constants RCC Private Constants
  * @{
  */
/* Defines used for security configuration extension */
#if defined(RCC_CR_PLL3ON)
#define RCC_SECURE_MASK         0x3BFFU
#else
#define RCC_SECURE_MASK         0x39FFU
#endif /* RCC_CR_PLL3ON */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
#if !defined(UNUSED)
#define UNUSED(x) ((void)(x))
#endif /* !UNUSED */

/* 32            24           16            8             0
   --------------------------------------------------------
   | Mask        | ClkSource   |  Bit       | Register    |
   |             |  Config     | Position   | Offset      |
   --------------------------------------------------------*/

/* Clock source register offset */
#define CCIPR1_OFFSET   0x00UL
#define CCIPR2_OFFSET   0x04UL
#define CCIPR3_OFFSET   0x08UL
#define CCIPR4_OFFSET   0x0CUL
#define CCIPR5_OFFSET   0x10UL

#define LL_RCC_REG_SHIFT     0U
#define LL_RCC_POS_SHIFT     8U
#define LL_RCC_CONFIG_SHIFT  16U
#define LL_RCC_MASK_SHIFT    24U

#define LL_CLKSOURCE_SHIFT(__CLKSOURCE__)   (((__CLKSOURCE__) >> LL_RCC_POS_SHIFT   ) & 0x1FUL)

#define LL_CLKSOURCE_MASK(__CLKSOURCE__)   ((((__CLKSOURCE__) >> LL_RCC_MASK_SHIFT  ) &\
                                             0xFFUL) << LL_CLKSOURCE_SHIFT(__CLKSOURCE__))

#define LL_CLKSOURCE_CONFIG(__CLKSOURCE__) ((((__CLKSOURCE__) >> LL_RCC_CONFIG_SHIFT) &\
                                             0xFFUL) << LL_CLKSOURCE_SHIFT(__CLKSOURCE__))

#define LL_CLKSOURCE_REG(__CLKSOURCE__)     (((__CLKSOURCE__) >> LL_RCC_REG_SHIFT   ) & 0xFFUL)

#define LL_CLKSOURCE(__REG__, __MSK__, __POS__, __CLK__) ((uint32_t)((((__MSK__) >> (__POS__)) << LL_RCC_MASK_SHIFT) | \
                                                                     (( __POS__              ) << LL_RCC_POS_SHIFT)  | \
                                                                     (( __REG__              ) << LL_RCC_REG_SHIFT)  | \
                                                                     (((__CLK__) >> (__POS__)) << LL_RCC_CONFIG_SHIFT)))

/* Exported types ------------------------------------------------------------*/
#if defined(USE_FULL_LL_DRIVER)
/** @defgroup RCC_LL_Exported_Types RCC Exported Types
  * @{
  */

/** @defgroup LL_ES_CLOCK_FREQ Clocks Frequency Structure
  * @{
  */

/**
  * @brief  RCC Clocks Frequency Structure
  */
typedef struct
{
  uint32_t SYSCLK_Frequency;        /*!< SYSCLK clock frequency */
  uint32_t HCLK_Frequency;          /*!< HCLK clock frequency */
  uint32_t PCLK1_Frequency;         /*!< PCLK1 clock frequency */
  uint32_t PCLK2_Frequency;         /*!< PCLK2 clock frequency */
  uint32_t PCLK3_Frequency;         /*!< PCLK3 clock frequency */
} LL_RCC_ClocksTypeDef;

/**
  * @brief  PLL Clocks Frequency Structure
  */
typedef struct
{
  uint32_t PLL_P_Frequency;
  uint32_t PLL_Q_Frequency;
  uint32_t PLL_R_Frequency;
} LL_PLL_ClocksTypeDef;

/**
  * @}
  */

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCC_LL_Exported_Constants RCC Exported Constants
  * @{
  */

/** @defgroup RCC_LL_EC_OSC_VALUES Oscillator Values adaptation
  * @brief    Defines used to adapt values of different oscillators
  * @note     These values could be modified in the user environment according to
  *           HW set-up.
  * @{
  */
#if !defined  (HSE_VALUE)
#define HSE_VALUE    25000000U  /*!< Value of the HSE oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
#define HSI_VALUE    64000000U  /*!< Value of the HSI oscillator in Hz */
#endif /* HSI_VALUE */

#if !defined  (CSI_VALUE)
#define CSI_VALUE    4000000U  /*!< Value of the CSI oscillator in Hz */
#endif /* CSI_VALUE */

#if !defined  (LSE_VALUE)
#define LSE_VALUE    32768U     /*!< Value of the LSE oscillator in Hz */
#endif /* LSE_VALUE */

#if !defined  (LSI_VALUE)
#define LSI_VALUE    32000U     /*!< Value of the LSI oscillator in Hz */
#endif /* LSI_VALUE */

#if !defined  (HSI48_VALUE)
#define HSI48_VALUE  48000000U  /*!< Value of the HSI48 oscillator in Hz */
#endif /* HSI48_VALUE */

#if !defined  (EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE    12288000U /*!< Value of the External clock in Hz*/
#endif /* EXTERNAL_CLOCK_VALUE */

/**
  * @}
  */
#if defined(PWR_USBSCR_OTGHSEN)
#define OTGHS_CLK48_VALUE       48000000U /*!< Value of the OTGHS_CLK48 in Hz */
#endif /* PWR_USBSCR_OTGHSEN */

/**
  * @}
  */

/** @defgroup RCC_LL_EC_HSIDIV  HSI oscillator divider
  * @{
  */
#define LL_RCC_HSI_DIV_1                   0x00000000U         /*!< HSI_DIV1 clock activation */
#define LL_RCC_HSI_DIV_2                   RCC_CR_HSIDIV_0     /*!< HSI_DIV2 clock activation */
#define LL_RCC_HSI_DIV_4                   RCC_CR_HSIDIV_1     /*!< HSI_DIV4 clock activation */
#define LL_RCC_HSI_DIV_8                   RCC_CR_HSIDIV       /*!< HSI_DIV8 clock activation */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LSEDRIVE  LSE oscillator drive capability
  * @{
  */
#if !defined(STM32H5E5xx) || !defined(STM32H5E4xx) || !defined(STM32H5F5xx) || !defined(STM32H5F4xx)
#define LL_RCC_LSEDRIVE_LOW                0x00000000U             /*!< Xtal mode lower driving capability */
#endif /* !defined(STM32H5E5xx) || !defined(STM32H5E4xx) || !defined(STM32H5F5xx) || !defined(STM32H5F4xx) */
#define LL_RCC_LSEDRIVE_MEDIUMLOW          RCC_BDCR_LSEDRV_0       /*!< Xtal mode medium low driving capability */
#define LL_RCC_LSEDRIVE_MEDIUMHIGH         RCC_BDCR_LSEDRV_1       /*!< Xtal mode medium high driving capability */
#define LL_RCC_LSEDRIVE_HIGH               RCC_BDCR_LSEDRV         /*!< Xtal mode higher driving capability */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SYS_CLKSOURCE_STATUS  System clock switch status
  * @{
  */
#define LL_RCC_SYS_CLKSOURCE_HSI           0x00000000U                          /*!< HSI oscillator selection as system clock */
#define LL_RCC_SYS_CLKSOURCE_CSI           RCC_CFGR1_SW_0                       /*!< CSI oscillator selection as system clock */
#define LL_RCC_SYS_CLKSOURCE_HSE           RCC_CFGR1_SW_1                       /*!< HSE oscillator selection as system clock */
#define LL_RCC_SYS_CLKSOURCE_PLL1         (RCC_CFGR1_SW_1 | RCC_CFGR1_SW_0)     /*!< PLL1 selection as system clock */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SYS_CLKSOURCE_STATUS  System clock switch status
  * @{
  */
#define LL_RCC_SYS_CLKSOURCE_STATUS_HSI    0x00000000U                          /*!< HSI oscillator used as system clock */
#define LL_RCC_SYS_CLKSOURCE_STATUS_CSI    RCC_CFGR1_SWS_0                      /*!< CSI oscillator used as system clock */
#define LL_RCC_SYS_CLKSOURCE_STATUS_HSE    RCC_CFGR1_SWS_1                      /*!< HSE oscillator used as system clock */
#define LL_RCC_SYS_CLKSOURCE_STATUS_PLL1   (RCC_CFGR1_SWS_1 | RCC_CFGR1_SWS_0)  /*!< PLL1 used as system clock */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_HSEEXT  EXTERNAL HSE clock Type
  * @{
  */
#define LL_RCC_HSE_ANALOG_TYPE               0U                    /*!< ANALOG  clock used as HSE external clock source  */
#define LL_RCC_HSE_DIGITAL_TYPE              RCC_CR_HSEEXT         /*!< DIGITAL clock used as HSE external clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LSEEXT  EXTERNAL LSE clock Type
  * @{
  */
#if !defined(STM32H5E5xx) || !defined(STM32H5E4xx) || !defined(STM32H5F5xx) || !defined(STM32H5F4xx)
#define LL_RCC_LSE_ANALOG_TYPE               0U                    /*!< ANALOG  clock used as LSE external clock source  */
#endif /* !defined(STM32H5E5xx) || !defined(STM32H5E4xx) || !defined(STM32H5F5xx) || !defined(STM32H5F4xx) */
#define LL_RCC_LSE_DIGITAL_TYPE              RCC_BDCR_LSEEXT       /*!< DIGITAL clock used as LSE external clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LSCO_CLKSOURCE  LSCO Selection
  * @{
  */
#define LL_RCC_LSCO_CLKSOURCE_LSI            0x00000000U           /*!< LSI selection for low speed clock  */
#define LL_RCC_LSCO_CLKSOURCE_LSE            RCC_BDCR_LSCOSEL      /*!< LSE selection for low speed clock  */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SYSCLK_DIV  AHB prescaler
  * @{
  */
#define LL_RCC_SYSCLK_DIV_1                   0x00000000U                                                                  /*!< SYSCLK not divided */
#define LL_RCC_SYSCLK_DIV_2                   RCC_CFGR2_HPRE_3                                                             /*!< SYSCLK divided by 2 */
#define LL_RCC_SYSCLK_DIV_4                  (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_3)                                         /*!< SYSCLK divided by 4 */
#define LL_RCC_SYSCLK_DIV_8                  (RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_3)                                         /*!< SYSCLK divided by 8 */
#define LL_RCC_SYSCLK_DIV_16                 (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_3)                      /*!< SYSCLK divided by 16 */
#define LL_RCC_SYSCLK_DIV_64                 (RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                                         /*!< SYSCLK divided by 64 */
#define LL_RCC_SYSCLK_DIV_128                (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                      /*!< SYSCLK divided by 128 */
#define LL_RCC_SYSCLK_DIV_256                (RCC_CFGR2_HPRE_1 | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)                      /*!< SYSCLK divided by 256 */
#define LL_RCC_SYSCLK_DIV_512                (RCC_CFGR2_HPRE_0 | RCC_CFGR2_HPRE_1  | RCC_CFGR2_HPRE_2 | RCC_CFGR2_HPRE_3)  /*!< SYSCLK divided by 512 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB1_DIV  APB low-speed prescaler (APB1)
  * @{
  */
#define LL_RCC_APB1_DIV_1                   (0x00000000U)                                                                 /*!< HCLK not divided */
#define LL_RCC_APB1_DIV_2                   RCC_CFGR2_PPRE1_2                                                             /*!< HCLK divided by 2 */
#define LL_RCC_APB1_DIV_4                   (RCC_CFGR2_PPRE1_0 | RCC_CFGR2_PPRE1_2)                                       /*!< HCLK divided by 4 */
#define LL_RCC_APB1_DIV_8                   (RCC_CFGR2_PPRE1_1 | RCC_CFGR2_PPRE1_2)                                       /*!< HCLK divided by 8 */
#define LL_RCC_APB1_DIV_16                  (RCC_CFGR2_PPRE1_0 | RCC_CFGR2_PPRE1_1 | RCC_CFGR2_PPRE1_2)                   /*!< HCLK divided by 16 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB2_DIV  APB high-speed prescaler (APB2)
  * @{
  */
#define LL_RCC_APB2_DIV_1                    0x00000000U                                                                  /*!< HCLK not divided */
#define LL_RCC_APB2_DIV_2                    RCC_CFGR2_PPRE2_2                                                            /*!< HCLK divided by 2 */
#define LL_RCC_APB2_DIV_4                    (RCC_CFGR2_PPRE2_2 | RCC_CFGR2_PPRE2_0)                                      /*!< HCLK divided by 4 */
#define LL_RCC_APB2_DIV_8                    (RCC_CFGR2_PPRE2_2 | RCC_CFGR2_PPRE2_1)                                      /*!< HCLK divided by 8 */
#define LL_RCC_APB2_DIV_16                   (RCC_CFGR2_PPRE2_2 | RCC_CFGR2_PPRE2_1 | RCC_CFGR2_PPRE2_0)                  /*!< HCLK divided by 16 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB3_DIV  APB high-speed prescaler (APB3)
  * @{
  */
#define LL_RCC_APB3_DIV_1                    0x00000000U                                                                  /*!< HCLK not divided */
#define LL_RCC_APB3_DIV_2                    RCC_CFGR2_PPRE3_2                                                            /*!< HCLK divided by 2 */
#define LL_RCC_APB3_DIV_4                    (RCC_CFGR2_PPRE3_2 | RCC_CFGR2_PPRE3_0)                                      /*!< HCLK divided by 4 */
#define LL_RCC_APB3_DIV_8                    (RCC_CFGR2_PPRE3_2 | RCC_CFGR2_PPRE3_1)                                      /*!< HCLK divided by 8 */
#define LL_RCC_APB3_DIV_16                   (RCC_CFGR2_PPRE3_2 | RCC_CFGR2_PPRE3_1 | RCC_CFGR2_PPRE3_0)                  /*!< HCLK divided by 16 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_AHB1_PERIPH  AHB1 peripherals clock branch disable
  * @{
  */
#define LL_RCC_AHB1_PERIPH_DIS              RCC_CFGR2_AHB1DIS                     /*!< Clock Branch disable for all AHB1 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_AHB2_PERIPH  AHB2 peripherals clock branch disable
  * @{
  */
#define LL_RCC_AHB2_PERIPH_DIS              RCC_CFGR2_AHB2DIS                     /*!< Clock Branch disable for all AHB2 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_AHB4_PERIPH  AHB4 peripherals clock branch disable
  * @{
  */
#define LL_RCC_AHB4_PERIPH_DIS              RCC_CFGR2_AHB4DIS                     /*!< Clock Branch disable for all AHB4 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB1_PERIPH  APB1 peripherals clock branch disable
  * @{
  */
#define LL_RCC_APB1_PERIPH_DIS              RCC_CFGR2_APB1DIS                     /*!< Clock Branch disable for all APB1 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB2_PERIPH  APB2 peripherals clock branch disable
  * @{
  */
#define LL_RCC_APB2_PERIPH_DIS              RCC_CFGR2_APB2DIS                     /*!< Clock Branch disable for all APB2 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_APB3_PERIPH  APB3 peripherals clock branch disable
  * @{
  */
#define LL_RCC_APB3_PERIPH_DIS              RCC_CFGR2_APB3DIS                     /*!< Clock Branch disable for all APB3 peripherals */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SYSTICK_CLKSOURCE  SYSTICK clock source selection
  * @{
  */
#define LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8    0x00000000U             /*!< HCLKDIV8  clock used as SYSTICK clock source */
#define LL_RCC_SYSTICK_CLKSOURCE_LSI         RCC_CCIPR4_SYSTICKSEL_0 /*!< LSI clock used as SYSTICK clock source */
#define LL_RCC_SYSTICK_CLKSOURCE_LSE         RCC_CCIPR4_SYSTICKSEL_1 /*!< LSE clock used as SYSTICK clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SYSWAKEUP_CLKSOURCE  System wakeup from stop and CSS backup clock selection
  * @{
  */
#define LL_RCC_SYSWAKEUP_CLKSOURCE_HSI          0x00000000U             /*!< HSI selection as system clock after wake-up from STOP */
#define LL_RCC_SYSWAKEUP_CLKSOURCE_CSI          RCC_CFGR1_STOPWUCK      /*!< CSI selection as system clock after wake-up from STOP */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_KERWAKEUP_CLKSOURCE  Kernel wakeup from stop clock source
  * @{
  */
#define LL_RCC_KERWAKEUP_CLKSOURCE_HSI          0x00000000U              /*!< HSI selection as kernel clock after wake-up from STOP */
#define LL_RCC_KERWAKEUP_CLKSOURCE_CSI          RCC_CFGR1_STOPKERWUCK    /*!< CSI selection as kernel clock after wake-up from STOP */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_RTC_HSE_DIV RTC HSE Prescaler
  * @{
  */
#define LL_RCC_RTC_HSE_NOCLOCK             (0x00000000U)
#define LL_RCC_RTC_HSE_DIV_2               (0x00000200U)
#define LL_RCC_RTC_HSE_DIV_3               (0x00000300U)
#define LL_RCC_RTC_HSE_DIV_4               (0x00000400U)
#define LL_RCC_RTC_HSE_DIV_5               (0x00000500U)
#define LL_RCC_RTC_HSE_DIV_6               (0x00000600U)
#define LL_RCC_RTC_HSE_DIV_7               (0x00000700U)
#define LL_RCC_RTC_HSE_DIV_8               (0x00000800U)
#define LL_RCC_RTC_HSE_DIV_9               (0x00000900U)
#define LL_RCC_RTC_HSE_DIV_10              (0x00000A00U)
#define LL_RCC_RTC_HSE_DIV_11              (0x00000B00U)
#define LL_RCC_RTC_HSE_DIV_12              (0x00000C00U)
#define LL_RCC_RTC_HSE_DIV_13              (0x00000D00U)
#define LL_RCC_RTC_HSE_DIV_14              (0x00000E00U)
#define LL_RCC_RTC_HSE_DIV_15              (0x00000F00U)
#define LL_RCC_RTC_HSE_DIV_16              (0x00001000U)
#define LL_RCC_RTC_HSE_DIV_17              (0x00001100U)
#define LL_RCC_RTC_HSE_DIV_18              (0x00001200U)
#define LL_RCC_RTC_HSE_DIV_19              (0x00001300U)
#define LL_RCC_RTC_HSE_DIV_20              (0x00001400U)
#define LL_RCC_RTC_HSE_DIV_21              (0x00001500U)
#define LL_RCC_RTC_HSE_DIV_22              (0x00001600U)
#define LL_RCC_RTC_HSE_DIV_23              (0x00001700U)
#define LL_RCC_RTC_HSE_DIV_24              (0x00001800U)
#define LL_RCC_RTC_HSE_DIV_25              (0x00001900U)
#define LL_RCC_RTC_HSE_DIV_26              (0x00001A00U)
#define LL_RCC_RTC_HSE_DIV_27              (0x00001B00U)
#define LL_RCC_RTC_HSE_DIV_28              (0x00001C00U)
#define LL_RCC_RTC_HSE_DIV_29              (0x00001D00U)
#define LL_RCC_RTC_HSE_DIV_30              (0x00001E00U)
#define LL_RCC_RTC_HSE_DIV_31              (0x00001F00U)
#define LL_RCC_RTC_HSE_DIV_32              (0x00002000U)
#define LL_RCC_RTC_HSE_DIV_33              (0x00002100U)
#define LL_RCC_RTC_HSE_DIV_34              (0x00002200U)
#define LL_RCC_RTC_HSE_DIV_35              (0x00002300U)
#define LL_RCC_RTC_HSE_DIV_36              (0x00002400U)
#define LL_RCC_RTC_HSE_DIV_37              (0x00002500U)
#define LL_RCC_RTC_HSE_DIV_38              (0x00002600U)
#define LL_RCC_RTC_HSE_DIV_39              (0x00002700U)
#define LL_RCC_RTC_HSE_DIV_40              (0x00002800U)
#define LL_RCC_RTC_HSE_DIV_41              (0x00002900U)
#define LL_RCC_RTC_HSE_DIV_42              (0x00002A00U)
#define LL_RCC_RTC_HSE_DIV_43              (0x00002B00U)
#define LL_RCC_RTC_HSE_DIV_44              (0x00002C00U)
#define LL_RCC_RTC_HSE_DIV_45              (0x00002D00U)
#define LL_RCC_RTC_HSE_DIV_46              (0x00002E00U)
#define LL_RCC_RTC_HSE_DIV_47              (0x00002F00U)
#define LL_RCC_RTC_HSE_DIV_48              (0x00003000U)
#define LL_RCC_RTC_HSE_DIV_49              (0x00003100U)
#define LL_RCC_RTC_HSE_DIV_50              (0x00003200U)
#define LL_RCC_RTC_HSE_DIV_51              (0x00003300U)
#define LL_RCC_RTC_HSE_DIV_52              (0x00003400U)
#define LL_RCC_RTC_HSE_DIV_53              (0x00003500U)
#define LL_RCC_RTC_HSE_DIV_54              (0x00003600U)
#define LL_RCC_RTC_HSE_DIV_55              (0x00003700U)
#define LL_RCC_RTC_HSE_DIV_56              (0x00003800U)
#define LL_RCC_RTC_HSE_DIV_57              (0x00003900U)
#define LL_RCC_RTC_HSE_DIV_58              (0x00003A00U)
#define LL_RCC_RTC_HSE_DIV_59              (0x00003B00U)
#define LL_RCC_RTC_HSE_DIV_60              (0x00003C00U)
#define LL_RCC_RTC_HSE_DIV_61              (0x00003D00U)
#define LL_RCC_RTC_HSE_DIV_62              (0x00003E00U)
#define LL_RCC_RTC_HSE_DIV_63              (0x00003F00U)
/**
  * @}
  */

/** @defgroup RCC_LL_EC_TIM_CLKPRESCALER  Timers clocks prescalers selection
  * @{
  */
#define LL_RCC_TIM_PRESCALER_TWICE          (uint32_t)(0x00000000U)
#define LL_RCC_TIM_PRESCALER_FOUR_TIMES     (uint32_t)(RCC_CFGR1_TIMPRE)
/**
  * @}
  */

/** @defgroup RCC_LL_EC_MCOxSOURCE  MCO SOURCE selection
  * @{
  */
#define LL_RCC_MCO1SOURCE_HSI              (uint32_t)((RCC_CFGR1_MCO1SEL>>16U) | 0x00000000U)
#define LL_RCC_MCO1SOURCE_LSE              (uint32_t)((RCC_CFGR1_MCO1SEL>>16U) | RCC_CFGR1_MCO1SEL_0)
#define LL_RCC_MCO1SOURCE_HSE              (uint32_t)((RCC_CFGR1_MCO1SEL>>16U) | RCC_CFGR1_MCO1SEL_1)
#define LL_RCC_MCO1SOURCE_PLL1Q            (uint32_t)((RCC_CFGR1_MCO1SEL>>16U) |\
                                                      RCC_CFGR1_MCO1SEL_1|RCC_CFGR1_MCO1SEL_0)
#define LL_RCC_MCO1SOURCE_HSI48            (uint32_t)((RCC_CFGR1_MCO1SEL>>16U) | RCC_CFGR1_MCO1SEL_2)
#define LL_RCC_MCO2SOURCE_SYSCLK           (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) | 0x00000000U)
#define LL_RCC_MCO2SOURCE_PLL2P            (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) | RCC_CFGR1_MCO2SEL_0)
#define LL_RCC_MCO2SOURCE_HSE              (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) | RCC_CFGR1_MCO2SEL_1)
#define LL_RCC_MCO2SOURCE_PLL1P            (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) |\
                                                      RCC_CFGR1_MCO2SEL_1|RCC_CFGR1_MCO2SEL_0)
#define LL_RCC_MCO2SOURCE_CSI              (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) | RCC_CFGR1_MCO2SEL_2)
#define LL_RCC_MCO2SOURCE_LSI              (uint32_t)((RCC_CFGR1_MCO2SEL>>16U) |\
                                                      RCC_CFGR1_MCO2SEL_2|RCC_CFGR1_MCO2SEL_0)
/**
  * @}
  */

/** @defgroup RCC_LL_EC_MCOx_DIV  MCO prescaler
  * @{
  */
#define LL_RCC_MCO1_DIV_1                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) | RCC_CFGR1_MCO1PRE_0)
#define LL_RCC_MCO1_DIV_2                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) | RCC_CFGR1_MCO1PRE_1)
#define LL_RCC_MCO1_DIV_3                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1)
#define LL_RCC_MCO1_DIV_4                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) | RCC_CFGR1_MCO1PRE_2)
#define LL_RCC_MCO1_DIV_5                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_2)
#define LL_RCC_MCO1_DIV_6                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2)
#define LL_RCC_MCO1_DIV_7                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2)
#define LL_RCC_MCO1_DIV_8                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_9                  (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_10                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_11                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_12                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_13                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_0 | RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_14                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) |\
                                                      RCC_CFGR1_MCO1PRE_1 | RCC_CFGR1_MCO1PRE_2 | RCC_CFGR1_MCO1PRE_3)
#define LL_RCC_MCO1_DIV_15                 (uint32_t)((RCC_CFGR1_MCO1PRE>>16U) | RCC_CFGR1_MCO1PRE)
#define LL_RCC_MCO2_DIV_1                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) | RCC_CFGR1_MCO2PRE_0)
#define LL_RCC_MCO2_DIV_2                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) | RCC_CFGR1_MCO2PRE_1)
#define LL_RCC_MCO2_DIV_3                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_1)
#define LL_RCC_MCO2_DIV_4                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) | RCC_CFGR1_MCO2PRE_2)
#define LL_RCC_MCO2_DIV_5                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_2)
#define LL_RCC_MCO2_DIV_6                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_1 | RCC_CFGR1_MCO2PRE_2)
#define LL_RCC_MCO2_DIV_7                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_1 | RCC_CFGR1_MCO2PRE_2)
#define LL_RCC_MCO2_DIV_8                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_9                  (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_10                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_1 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_11                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_1 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_12                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_2 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_13                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_0 | RCC_CFGR1_MCO2PRE_2 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_14                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) |\
                                                      RCC_CFGR1_MCO2PRE_1 | RCC_CFGR1_MCO2PRE_2 | RCC_CFGR1_MCO2PRE_3)
#define LL_RCC_MCO2_DIV_15                 (uint32_t)((RCC_CFGR1_MCO2PRE>>16U) | RCC_CFGR1_MCO2PRE)
/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup RCC_LL_EC_PERIPH_FREQUENCY Peripheral clock frequency
  * @{
  */
#define LL_RCC_PERIPH_FREQUENCY_NO           0x00000000U                 /*!< No clock enabled for the peripheral            */
#define LL_RCC_PERIPH_FREQUENCY_NA           0xFFFFFFFFU                 /*!< Frequency cannot be provided as external clock */
/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

#if defined (RCC_CCIPR4_OTGPDCTRL)
/** @defgroup RCC_LL_EC_OTGPD_SUSPEND OTG PHY Suspend Power State
  * @{
  */
#define LL_RCC_OTGPD_SUSPEND_POWERDOWN   ((uint32_t)0x00000000U)         /*!< Power down the PHY state machine, bias, and PLL in SUSPEND mode. */
#define LL_RCC_OTGPD_SUSPEND_POWERON     RCC_CCIPR4_OTGPDCTRL            /*!< Keep the PHY powered in SUSPEND mode (default after reset). */
/**
  * @}
  */
#endif /* RCC_CCIPR4_OTGPDCTRL */

/** @defgroup RCC_LL_EC_RTC_CLKSOURCE  RTC clock source selection
  * @{
  */
#define LL_RCC_RTC_CLKSOURCE_NONE            0x00000000U             /*!< No clock used as RTC clock */
#define LL_RCC_RTC_CLKSOURCE_LSE             RCC_BDCR_RTCSEL_0       /*!< LSE oscillator clock used as RTC clock */
#define LL_RCC_RTC_CLKSOURCE_LSI             RCC_BDCR_RTCSEL_1       /*!< LSI oscillator clock used as RTC clock */
#define LL_RCC_RTC_CLKSOURCE_HSE_DIV         RCC_BDCR_RTCSEL         /*!< HSE oscillator clock divided by RTCPRE used as RTC clock */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_USART_CLKSOURCE  Peripheral USARTx clock source selection
  * @{
  */
#define LL_RCC_USART1_CLKSOURCE_PCLK2       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, 0x00000000U)                                      /*!< PCLK2 clock used as USART1 clock source  */
#define LL_RCC_USART1_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, RCC_CCIPR1_USART1SEL_0)                           /*!< PLL2 Q clock used as USART1 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_USART1_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, RCC_CCIPR1_USART1SEL_1)                           /*!< PLL3 Q clock used as USART1 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_USART1_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, RCC_CCIPR1_USART1SEL_1 | RCC_CCIPR1_USART1SEL_0)  /*!< HSI   clock used as USART1 clock source  */
#define LL_RCC_USART1_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, RCC_CCIPR1_USART1SEL_2)                           /*!< CSI   clock used as USART1 clock source  */
#define LL_RCC_USART1_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, RCC_CCIPR1_USART1SEL_2 | RCC_CCIPR1_USART1SEL_0)  /*!< LSE   clock used as USART1 clock source  */

#define LL_RCC_USART2_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, 0x00000000U)                                      /*!< PCLK1 clock used as USART2 clock source  */
#define LL_RCC_USART2_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, RCC_CCIPR1_USART2SEL_0)                           /*!< PLL2 Q clock used as USART2 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_USART2_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, RCC_CCIPR1_USART2SEL_1)                           /*!< PLL3 Q clock used as USART2 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_USART2_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, RCC_CCIPR1_USART2SEL_1 | RCC_CCIPR1_USART2SEL_0)  /*!< HSI   clock used as USART2 clock source  */
#define LL_RCC_USART2_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, RCC_CCIPR1_USART2SEL_2)                           /*!< CSI   clock used as USART2 clock source  */
#define LL_RCC_USART2_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, RCC_CCIPR1_USART2SEL_2 | RCC_CCIPR1_USART2SEL_0)  /*!< LSE   clock used as USART2 clock source  */

#define LL_RCC_USART3_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, 0x00000000U)                                      /*!< PCLK1 clock used as USART3 clock source  */
#define LL_RCC_USART3_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, RCC_CCIPR1_USART3SEL_0)                           /*!< PLL2 Q clock used as USART3 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_USART3_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, RCC_CCIPR1_USART3SEL_1)                           /*!< PLL3 Q clock used as USART3 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_USART3_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, RCC_CCIPR1_USART3SEL_1 | RCC_CCIPR1_USART3SEL_0)  /*!< HSI   clock used as USART3 clock source  */
#define LL_RCC_USART3_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, RCC_CCIPR1_USART3SEL_2)                           /*!< CSI   clock used as USART3 clock source  */
#define LL_RCC_USART3_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, RCC_CCIPR1_USART3SEL_2 | RCC_CCIPR1_USART3SEL_0)  /*!< LSE   clock used as USART3 clock source  */

#if defined(USART6)
#define LL_RCC_USART6_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, 0x00000000U)                                      /*!< PCLK1 clock used as USART6 clock source  */
#define LL_RCC_USART6_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, RCC_CCIPR1_USART6SEL_0)                           /*!< PLL2 Q clock used as USART6 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_USART6_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, RCC_CCIPR1_USART6SEL_1)                           /*!< PLL3 Q clock used as USART6 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_USART6_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, RCC_CCIPR1_USART6SEL_1 | RCC_CCIPR1_USART6SEL_0)  /*!< HSI   clock used as USART6 clock source  */
#define LL_RCC_USART6_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, RCC_CCIPR1_USART6SEL_2)                           /*!< CSI   clock used as USART6 clock source  */
#define LL_RCC_USART6_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, RCC_CCIPR1_USART6SEL_2 | RCC_CCIPR1_USART6SEL_0)  /*!< LSE   clock used as USART6 clock source  */
#endif /* USART6 */

#if defined(USART10)
#define LL_RCC_USART10_CLKSOURCE_PCLK1      LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, 0x00000000U)                                       /*!< PCLK1 clock used as USART10 clock source  */
#define LL_RCC_USART10_CLKSOURCE_PLL2Q      LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, RCC_CCIPR1_USART10SEL_0)                           /*!< PLL2 Q clock used as USART10 clock source */
#define LL_RCC_USART10_CLKSOURCE_PLL3Q      LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, RCC_CCIPR1_USART10SEL_1)                           /*!< PLL3 Q clock used as USART10 clock source */
#define LL_RCC_USART10_CLKSOURCE_HSI        LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, RCC_CCIPR1_USART10SEL_1 | RCC_CCIPR1_USART10SEL_0) /*!< HSI   clock used as USART10 clock source  */
#define LL_RCC_USART10_CLKSOURCE_CSI        LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, RCC_CCIPR1_USART10SEL_2)                           /*!< CSI   clock used as USART10 clock source  */
#define LL_RCC_USART10_CLKSOURCE_LSE        LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, RCC_CCIPR1_USART10SEL_2 | RCC_CCIPR1_USART10SEL_0) /*!< LSE   clock used as USART10 clock source  */
#endif /* USART10 */

#if defined(USART11)
#define LL_RCC_USART11_CLKSOURCE_PCLK1      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, 0x00000000U)                                       /*!< PCLK1 clock used as USART11 clock source  */
#define LL_RCC_USART11_CLKSOURCE_PLL2Q      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, RCC_CCIPR2_USART11SEL_0)                           /*!< PLL2 Q clock used as USART11 clock source */
#define LL_RCC_USART11_CLKSOURCE_PLL3Q      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, RCC_CCIPR2_USART11SEL_1)                           /*!< PLL3 Q clock used as USART11 clock source */
#define LL_RCC_USART11_CLKSOURCE_HSI        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, RCC_CCIPR2_USART11SEL_1 | RCC_CCIPR2_USART11SEL_0) /*!< HSI   clock used as USART11 clock source  */
#define LL_RCC_USART11_CLKSOURCE_CSI        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, RCC_CCIPR2_USART11SEL_2)                           /*!< CSI   clock used as USART11 clock source  */
#define LL_RCC_USART11_CLKSOURCE_LSE        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, RCC_CCIPR2_USART11SEL_2 | RCC_CCIPR2_USART11SEL_0) /*!< LSE   clock used as USART11 clock source  */
#endif /* USART11 */
/**
  * @}
  */

#if defined(UART4)
/** @defgroup RCC_LL_EC_UART_CLKSOURCE  Peripheral UARTx clock source selection
  * @{
  */
#define LL_RCC_UART4_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, 0x00000000U)                                            /*!< PCLK1 clock used as UART4 clock source  */
#define LL_RCC_UART4_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, RCC_CCIPR1_UART4SEL_0)                                  /*!< PLL2 Q clock used as UART4 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_UART4_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, RCC_CCIPR1_UART4SEL_1)                                  /*!< PLL3 Q clock used as UART4 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_UART4_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, RCC_CCIPR1_UART4SEL_1 | RCC_CCIPR1_UART4SEL_0)          /*!< HSI   clock used as UART4 clock source  */
#define LL_RCC_UART4_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, RCC_CCIPR1_UART4SEL_2)                                  /*!< CSI   clock used as UART4 clock source  */
#define LL_RCC_UART4_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, RCC_CCIPR1_UART4SEL_2 | RCC_CCIPR1_UART4SEL_0)          /*!< LSE   clock used as UART4 clock source  */

#define LL_RCC_UART5_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, 0x00000000U)                                            /*!< PCLK1 clock used as UART5 clock source  */
#define LL_RCC_UART5_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, RCC_CCIPR1_UART5SEL_0)                                  /*!< PLL2 Q clock used as UART5 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_UART5_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, RCC_CCIPR1_UART5SEL_1)                                  /*!< PLL3 Q clock used as UART5 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_UART5_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, RCC_CCIPR1_UART5SEL_1 | RCC_CCIPR1_UART5SEL_0)          /*!< HSI   clock used as UART5 clock source  */
#define LL_RCC_UART5_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, RCC_CCIPR1_UART5SEL_2)                                  /*!< CSI   clock used as UART5 clock source  */
#define LL_RCC_UART5_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, RCC_CCIPR1_UART5SEL_2 | RCC_CCIPR1_UART5SEL_0)          /*!< LSE   clock used as UART5 clock source  */

#define LL_RCC_UART7_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, 0x00000000U)                                            /*!< PCLK1 clock used as UART7 clock source  */
#define LL_RCC_UART7_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, RCC_CCIPR1_UART7SEL_0)                                  /*!< PLL2 Q clock used as UART7 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_UART7_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, RCC_CCIPR1_UART7SEL_1)                                  /*!< PLL3 Q clock used as UART7 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_UART7_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, RCC_CCIPR1_UART7SEL_1 | RCC_CCIPR1_UART7SEL_0)          /*!< HSI   clock used as UART7 clock source  */
#define LL_RCC_UART7_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, RCC_CCIPR1_UART7SEL_2)                                  /*!< CSI   clock used as UART7 clock source  */
#define LL_RCC_UART7_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, RCC_CCIPR1_UART7SEL_2 | RCC_CCIPR1_UART7SEL_0)          /*!< LSE   clock used as UART7 clock source  */

#define LL_RCC_UART8_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, 0x00000000U)                                            /*!< PCLK1 clock used as UART8 clock source  */
#define LL_RCC_UART8_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, RCC_CCIPR1_UART8SEL_0)                                  /*!< PLL2 Q clock used as UART8 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_UART8_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, RCC_CCIPR1_UART8SEL_1)                                  /*!< PLL3 Q clock used as UART8 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_UART8_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, RCC_CCIPR1_UART8SEL_1 | RCC_CCIPR1_UART8SEL_0)          /*!< HSI   clock used as UART8 clock source  */
#define LL_RCC_UART8_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, RCC_CCIPR1_UART8SEL_2)                                  /*!< CSI   clock used as UART8 clock source  */
#define LL_RCC_UART8_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, RCC_CCIPR1_UART8SEL_2 | RCC_CCIPR1_UART8SEL_0)          /*!< LSE   clock used as UART8 clock source  */

#define LL_RCC_UART9_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, 0x00000000U)                                            /*!< PCLK1 clock used as UART9 clock source  */
#define LL_RCC_UART9_CLKSOURCE_PLL2Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, RCC_CCIPR1_UART9SEL_0)                                  /*!< PLL2 Q clock used as UART9 clock source */
#define LL_RCC_UART9_CLKSOURCE_PLL3Q       LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, RCC_CCIPR1_UART9SEL_1)                                  /*!< PLL3 Q clock used as UART9 clock source */
#define LL_RCC_UART9_CLKSOURCE_HSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, RCC_CCIPR1_UART9SEL_1 | RCC_CCIPR1_UART9SEL_0)          /*!< HSI   clock used as UART9 clock source  */
#define LL_RCC_UART9_CLKSOURCE_CSI         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, RCC_CCIPR1_UART9SEL_2)                                  /*!< CSI   clock used as UART9 clock source  */
#define LL_RCC_UART9_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, RCC_CCIPR1_UART9SEL_2 | RCC_CCIPR1_UART9SEL_0)          /*!< LSE   clock used as UART9 clock source  */

#define LL_RCC_UART12_CLKSOURCE_PCLK1      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, 0x00000000U)                                          /*!< PCLK1 clock used as UART12 clock source  */
#define LL_RCC_UART12_CLKSOURCE_PLL2Q      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, RCC_CCIPR2_UART12SEL_0)                               /*!< PLL2 Q clock used as UART12 clock source */
#define LL_RCC_UART12_CLKSOURCE_PLL3Q      LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, RCC_CCIPR2_UART12SEL_1)                               /*!< PLL3 Q clock used as UART12 clock source */
#define LL_RCC_UART12_CLKSOURCE_HSI        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, RCC_CCIPR2_UART12SEL_1 | RCC_CCIPR2_UART12SEL_0)      /*!< HSI   clock used as UART12 clock source  */
#define LL_RCC_UART12_CLKSOURCE_CSI        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, RCC_CCIPR2_UART12SEL_2)                               /*!< CSI   clock used as UART12 clock source  */
#define LL_RCC_UART12_CLKSOURCE_LSE        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, RCC_CCIPR2_UART12SEL_2 | RCC_CCIPR2_UART12SEL_0)      /*!< LSE   clock used as UART12 clock source  */
/**
  * @}
  */
#endif /* UART4 */

/** @defgroup RCC_LL_EC_LPUART_CLKSOURCE  Peripheral LPUARTx clock source selection
  * @{
  */
#define LL_RCC_LPUART1_CLKSOURCE_PCLK3       0x00000000U                                         /*!< PCLK3 clock used as LPUART1 clock source */
#define LL_RCC_LPUART1_CLKSOURCE_PLL2Q       RCC_CCIPR3_LPUART1SEL_0                             /*!< PLL2Q clock used as LPUART1 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_LPUART1_CLKSOURCE_PLL3Q       RCC_CCIPR3_LPUART1SEL_1                             /*!< PLL3Q clock used as LPUART1 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_LPUART1_CLKSOURCE_HSI        (RCC_CCIPR3_LPUART1SEL_0 | RCC_CCIPR3_LPUART1SEL_1)  /*!< HSI   clock used as LPUART1 clock source */
#define LL_RCC_LPUART1_CLKSOURCE_CSI         RCC_CCIPR3_LPUART1SEL_2                             /*!< CSI   clock used as LPUART1 clock source */
#define LL_RCC_LPUART1_CLKSOURCE_LSE        (RCC_CCIPR3_LPUART1SEL_0 | RCC_CCIPR3_LPUART1SEL_2)  /*!< LSE   clock used as LPUART1 clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_I2C_CLKSOURCE  Peripheral I2Cx clock source selection
  * @{
  */
#define LL_RCC_I2C1_CLKSOURCE_PCLK1         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, 0x00000000U)              /*!< PCLK1 clock used as I2C1 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_I2C1_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, RCC_CCIPR4_I2C1SEL_0)     /*!< PLL3 R clock used as I2C1 clock source */
#else
#define LL_RCC_I2C1_CLKSOURCE_PLL2R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, RCC_CCIPR4_I2C1SEL_0)     /*!< PLL2 R clock used as I2C1 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_I2C1_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, RCC_CCIPR4_I2C1SEL_1)     /*!< HSI clock used as I2C1 clock source */
#define LL_RCC_I2C1_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, RCC_CCIPR4_I2C1SEL)       /*!< CSI clock used as I2C1 clock source */

#define LL_RCC_I2C2_CLKSOURCE_PCLK1         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, 0x00000000U)              /*!< PCLK1 clock used as I2C2 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_I2C2_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, RCC_CCIPR4_I2C2SEL_0)     /*!< PLL3 R clock used as I2C2 clock source */
#else
#define LL_RCC_I2C2_CLKSOURCE_PLL2R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, RCC_CCIPR4_I2C2SEL_0)     /*!< PLL2 R clock used as I2C2 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_I2C2_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, RCC_CCIPR4_I2C2SEL_1)     /*!< HSI clock used as I2C2 clock source */
#define LL_RCC_I2C2_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, RCC_CCIPR4_I2C2SEL)       /*!< CSI clock used as I2C2 clock source */

#if defined(I2C3)
#define LL_RCC_I2C3_CLKSOURCE_PCLK3         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, 0x00000000U)              /*!< PCLK3 clock used as I2C3 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_I2C3_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, RCC_CCIPR4_I2C3SEL_0)     /*!< PLL3 R clock used as I2C3 clock source */
#else
#define LL_RCC_I2C3_CLKSOURCE_PLL2R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, RCC_CCIPR4_I2C3SEL_0)     /*!< PLL2 R clock used as I2C3 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_I2C3_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, RCC_CCIPR4_I2C3SEL_1)     /*!< HSI clock used as I2C3 clock source */
#define LL_RCC_I2C3_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, RCC_CCIPR4_I2C3SEL)       /*!< CSI clock used as I2C3 clock source */
#endif /* I2C3 */

#if defined(I2C4)
#define LL_RCC_I2C4_CLKSOURCE_PCLK3         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C4SEL, RCC_CCIPR4_I2C4SEL_Pos, 0x00000000U)              /*!< PCLK3 clock used as I2C4 clock source */
#define LL_RCC_I2C4_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C4SEL, RCC_CCIPR4_I2C4SEL_Pos, RCC_CCIPR4_I2C4SEL_0)     /*!< PLL3 R clock used as I2C4 clock source */
#define LL_RCC_I2C4_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C4SEL, RCC_CCIPR4_I2C4SEL_Pos, RCC_CCIPR4_I2C4SEL_1)     /*!< HSI clock used as I2C4 clock source */
#define LL_RCC_I2C4_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C4SEL, RCC_CCIPR4_I2C4SEL_Pos, RCC_CCIPR4_I2C4SEL)       /*!< CSI clock used as I2C4 clock source */
#endif /* I2C4 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_I3C_CLKSOURCE  Peripheral I3Cx clock source selection
  * @{
  */
#define LL_RCC_I3C1_CLKSOURCE_PCLK1         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, 0x00000000U)              /*!< PCLK1 clock used as I3C1 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_I3C1_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, RCC_CCIPR4_I3C1SEL_0)     /*!< PLL3 R clock used as I3C1 clock source */
#else
#define LL_RCC_I3C1_CLKSOURCE_PLL2R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, RCC_CCIPR4_I3C1SEL_0)     /*!< PLL2 R clock used as I3C1 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_I3C1_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, RCC_CCIPR4_I3C1SEL_1)     /*!< HSI clock used as I3C1 clock source */
#define LL_RCC_I3C1_CLKSOURCE_NONE          LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, RCC_CCIPR4_I3C1SEL)       /*!< NONE clock used as I3C1 clock source */

#if defined(I3C2)
#define LL_RCC_I3C2_CLKSOURCE_PCLK3         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, 0x00000000U)              /*!< PCLK3 clock used as I3C2 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_I3C2_CLKSOURCE_PLL3R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, RCC_CCIPR4_I3C2SEL_0)     /*!< PLL3 R clock used as I3C2 clock source */
#else
#define LL_RCC_I3C2_CLKSOURCE_PLL2R         LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, RCC_CCIPR4_I3C2SEL_0)     /*!< PLL2 R clock used as I3C2 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_I3C2_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, RCC_CCIPR4_I3C2SEL_1)     /*!< HSI clock used as I3C2 clock source */
#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
#define LL_RCC_I3C2_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, RCC_CCIPR4_I3C2SEL_0 |RCC_CCIPR4_I3C2SEL_1)  /*!< CSI clock used as I3C2 clock source */
#else
#define LL_RCC_I3C2_CLKSOURCE_NONE          LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, RCC_CCIPR4_I3C2SEL)       /*!< NONE clock used as I3C2 clock source */
#endif /* defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) */
#endif /* I3C2 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_SPI_CLKSOURCE  Peripheral SPIx clock source selection
  * @{
  */
#define LL_RCC_SPI1_CLKSOURCE_PLL1Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, 0x00000000U)                                  /*!< PLL1 Q clock used as SPI1 clock source */
#define LL_RCC_SPI1_CLKSOURCE_PLL2P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, RCC_CCIPR3_SPI1SEL_0)                         /*!< PLL2 P clock used as SPI1 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_SPI1_CLKSOURCE_PLL3P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, RCC_CCIPR3_SPI1SEL_1)                         /*!< PLL3 P clock used as SPI1 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_SPI1_CLKSOURCE_PIN           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, RCC_CCIPR3_SPI1SEL_1 | RCC_CCIPR3_SPI1SEL_0)  /*!< PIN clock used as SPI1 clock source    */
#define LL_RCC_SPI1_CLKSOURCE_CLKP          LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, RCC_CCIPR3_SPI1SEL_2)                         /*!< CLKP clock used as SPI1 clock source   */

#define LL_RCC_SPI2_CLKSOURCE_PLL1Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, 0x00000000U)                                  /*!< PLL1 Q clock used as SPI2 clock source */
#define LL_RCC_SPI2_CLKSOURCE_PLL2P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, RCC_CCIPR3_SPI2SEL_0)                         /*!< PLL2 P clock used as SPI2 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_SPI2_CLKSOURCE_PLL3P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, RCC_CCIPR3_SPI2SEL_1)                         /*!< PLL3 P clock used as SPI2 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_SPI2_CLKSOURCE_PIN           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, RCC_CCIPR3_SPI2SEL_1 | RCC_CCIPR3_SPI2SEL_0)  /*!< PIN clock used as SPI2 clock source    */
#define LL_RCC_SPI2_CLKSOURCE_CLKP          LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, RCC_CCIPR3_SPI2SEL_2)                         /*!< CLKP clock used as SPI2 clock source   */

#define LL_RCC_SPI3_CLKSOURCE_PLL1Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, 0x00000000U)                                  /*!< PLL1 Q clock used as SPI3 clock source */
#define LL_RCC_SPI3_CLKSOURCE_PLL2P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, RCC_CCIPR3_SPI3SEL_0)                         /*!< PLL2 P clock used as SPI3 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_SPI3_CLKSOURCE_PLL3P         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, RCC_CCIPR3_SPI3SEL_1)                         /*!< PLL3 P clock used as SPI3 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_SPI3_CLKSOURCE_PIN           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, RCC_CCIPR3_SPI3SEL_1 | RCC_CCIPR3_SPI3SEL_0)  /*!< PIN clock used as SPI3 clock source    */
#define LL_RCC_SPI3_CLKSOURCE_CLKP          LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, RCC_CCIPR3_SPI3SEL_2)                         /*!< CLKP clock used as SPI3 clock source   */

#if defined(SPI4)
#define LL_RCC_SPI4_CLKSOURCE_PCLK2         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, 0x00000000U)                                  /*!< PCLK2 clock used as SPI4 clock source  */
#define LL_RCC_SPI4_CLKSOURCE_PLL2Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, RCC_CCIPR3_SPI4SEL_0)                         /*!< PLL2 Q clock used as SPI4 clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_SPI4_CLKSOURCE_PLL3Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, RCC_CCIPR3_SPI4SEL_1)                         /*!< PLL3 Q clock used as SPI4 clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_SPI4_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, RCC_CCIPR3_SPI4SEL_1 | RCC_CCIPR3_SPI4SEL_0)  /*!< HSI clock used as SPI4 clock source    */
#define LL_RCC_SPI4_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, RCC_CCIPR3_SPI4SEL_2)                         /*!< CSI clock used as SPI4 clock source    */
#define LL_RCC_SPI4_CLKSOURCE_HSE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, RCC_CCIPR3_SPI4SEL_2 | RCC_CCIPR3_SPI4SEL_0)  /*!< HSE clock used as SPI4 clock source    */
#endif /* SPI4 */

#if defined(SPI5)
#define LL_RCC_SPI5_CLKSOURCE_PCLK3         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, 0x00000000U)                                  /*!< PCLK2 clock used as SPI5 clock source  */
#define LL_RCC_SPI5_CLKSOURCE_PLL2Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, RCC_CCIPR3_SPI5SEL_0)                         /*!< PLL2 Q clock used as SPI5 clock source */
#define LL_RCC_SPI5_CLKSOURCE_PLL3Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, RCC_CCIPR3_SPI5SEL_1)                         /*!< PLL3 Q clock used as SPI5 clock source */
#define LL_RCC_SPI5_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, RCC_CCIPR3_SPI5SEL_1 | RCC_CCIPR3_SPI5SEL_0)  /*!< HSI clock used as SPI5 clock source    */
#define LL_RCC_SPI5_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, RCC_CCIPR3_SPI5SEL_2)                         /*!< CSI clock used as SPI5 clock source    */
#define LL_RCC_SPI5_CLKSOURCE_HSE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, RCC_CCIPR3_SPI5SEL_2 | RCC_CCIPR3_SPI5SEL_0)  /*!< HSE clock used as SPI5 clock source    */
#endif /* SPI5 */

#if defined(SPI6)
#define LL_RCC_SPI6_CLKSOURCE_PCLK2         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, 0x00000000U)                                  /*!< PCLK2 clock used as SPI6 clock source  */
#define LL_RCC_SPI6_CLKSOURCE_PLL2Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, RCC_CCIPR3_SPI6SEL_0)                         /*!< PLL2 Q clock used as SPI6 clock source */
#define LL_RCC_SPI6_CLKSOURCE_PLL3Q         LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, RCC_CCIPR3_SPI6SEL_1)                         /*!< PLL3 Q clock used as SPI6 clock source */
#define LL_RCC_SPI6_CLKSOURCE_HSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, RCC_CCIPR3_SPI6SEL_1 | RCC_CCIPR3_SPI6SEL_0)  /*!< HSI clock used as SPI6 clock source    */
#define LL_RCC_SPI6_CLKSOURCE_CSI           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, RCC_CCIPR3_SPI6SEL_2)                         /*!< CSI clock used as SPI6 clock source    */
#define LL_RCC_SPI6_CLKSOURCE_HSE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, RCC_CCIPR3_SPI6SEL_2 | RCC_CCIPR3_SPI6SEL_0)  /*!< HSE clock used as SPI6 clock source    */
#endif /* SPI6 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LPTIM_CLKSOURCE  Peripheral LPTIMx clock source selection
  * @{
  */
#define LL_RCC_LPTIM1_CLKSOURCE_PCLK3       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, 0x00000000U)                                       /*!< PCLK3 clock used as LPTIM1 clock source */
#define LL_RCC_LPTIM1_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, RCC_CCIPR2_LPTIM1SEL_0)                            /*!< PLL2 P clock used as LPTIM1 clock source  */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_LPTIM1_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, RCC_CCIPR2_LPTIM1SEL_1)                            /*!< PLL3 R clock used as LPTIM1 clock source  */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_LPTIM1_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, RCC_CCIPR2_LPTIM1SEL_0 | RCC_CCIPR2_LPTIM1SEL_1)   /*!< LSE  clock used as LPTIM1 clock source  */
#define LL_RCC_LPTIM1_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, RCC_CCIPR2_LPTIM1SEL_2)                            /*!< LSI  clock used as LPTIM1 clock source  */
#define LL_RCC_LPTIM1_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, RCC_CCIPR2_LPTIM1SEL_0 | RCC_CCIPR2_LPTIM1SEL_2)   /*!< CLKP clock used as LPTIM1 clock source  */

#define LL_RCC_LPTIM2_CLKSOURCE_PCLK1       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, 0x00000000U)                                       /*!< PCLK1 clock used as LPTIM2 clock source */
#define LL_RCC_LPTIM2_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, RCC_CCIPR2_LPTIM2SEL_0)                            /*!< PLL2 P clock used as LPTIM2 clock source  */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_LPTIM2_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, RCC_CCIPR2_LPTIM2SEL_1)                            /*!< PLL3 R clock used as LPTIM2 clock source  */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_LPTIM2_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, RCC_CCIPR2_LPTIM2SEL_0 | RCC_CCIPR2_LPTIM2SEL_1)   /*!< LSE  clock used as LPTIM2 clock source  */
#define LL_RCC_LPTIM2_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, RCC_CCIPR2_LPTIM2SEL_2)                            /*!< LSI  clock used as LPTIM2 clock source  */
#define LL_RCC_LPTIM2_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, RCC_CCIPR2_LPTIM2SEL_0 | RCC_CCIPR2_LPTIM2SEL_2)   /*!< CLKP clock used as LPTIM2 clock source  */

#if defined(LPTIM3)
#define LL_RCC_LPTIM3_CLKSOURCE_PCLK3       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, 0x00000000U)                                       /*!< PCLK3 clock used as LPTIM3 clock source */
#define LL_RCC_LPTIM3_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, RCC_CCIPR2_LPTIM3SEL_0)                            /*!< PLL2 P clock used as LPTIM3 clock source  */
#define LL_RCC_LPTIM3_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, RCC_CCIPR2_LPTIM3SEL_1)                            /*!< PLL3 R clock used as LPTIM3 clock source  */
#define LL_RCC_LPTIM3_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, RCC_CCIPR2_LPTIM3SEL_0 | RCC_CCIPR2_LPTIM3SEL_1)   /*!< LSE  clock used as LPTIM3 clock source  */
#define LL_RCC_LPTIM3_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, RCC_CCIPR2_LPTIM3SEL_2)                            /*!< LSI  clock used as LPTIM3 clock source  */
#define LL_RCC_LPTIM3_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, RCC_CCIPR2_LPTIM3SEL_0 | RCC_CCIPR2_LPTIM3SEL_2)   /*!< CLKP clock used as LPTIM3 clock source  */
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define LL_RCC_LPTIM4_CLKSOURCE_PCLK3       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, 0x00000000U)                                       /*!< PCLK3 clock used as LPTIM4 clock source */
#define LL_RCC_LPTIM4_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, RCC_CCIPR2_LPTIM4SEL_0)                            /*!< PLL2 P clock used as LPTIM4 clock source  */
#define LL_RCC_LPTIM4_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, RCC_CCIPR2_LPTIM4SEL_1)                            /*!< PLL3 R clock used as LPTIM4 clock source  */
#define LL_RCC_LPTIM4_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, RCC_CCIPR2_LPTIM4SEL_0 | RCC_CCIPR2_LPTIM4SEL_1)   /*!< LSE  clock used as LPTIM4 clock source  */
#define LL_RCC_LPTIM4_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, RCC_CCIPR2_LPTIM4SEL_2)                            /*!< LSI  clock used as LPTIM4 clock source  */
#define LL_RCC_LPTIM4_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, RCC_CCIPR2_LPTIM4SEL_0 | RCC_CCIPR2_LPTIM4SEL_2)   /*!< CLKP clock used as LPTIM4 clock source  */
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define LL_RCC_LPTIM5_CLKSOURCE_PCLK3       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, 0x00000000U)                                       /*!< PCLK3 clock used as LPTIM5 clock source */
#define LL_RCC_LPTIM5_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, RCC_CCIPR2_LPTIM5SEL_0)                            /*!< PLL2 P clock used as LPTIM5 clock source  */
#define LL_RCC_LPTIM5_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, RCC_CCIPR2_LPTIM5SEL_1)                            /*!< PLL3 R clock used as LPTIM5 clock source  */
#define LL_RCC_LPTIM5_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, RCC_CCIPR2_LPTIM5SEL_0 | RCC_CCIPR2_LPTIM5SEL_1)   /*!< LSE  clock used as LPTIM5 clock source  */
#define LL_RCC_LPTIM5_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, RCC_CCIPR2_LPTIM5SEL_2)                            /*!< LSI  clock used as LPTIM5 clock source  */
#define LL_RCC_LPTIM5_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, RCC_CCIPR2_LPTIM5SEL_0 | RCC_CCIPR2_LPTIM5SEL_2)   /*!< CLKP clock used as LPTIM5 clock source  */
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define LL_RCC_LPTIM6_CLKSOURCE_PCLK3       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, 0x00000000U)                                       /*!< PCLK3 clock used as LPTIM6 clock source */
#define LL_RCC_LPTIM6_CLKSOURCE_PLL2P       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, RCC_CCIPR2_LPTIM6SEL_0)                            /*!< PLL2 P clock used as LPTIM6 clock source  */
#define LL_RCC_LPTIM6_CLKSOURCE_PLL3R       LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, RCC_CCIPR2_LPTIM6SEL_1)                            /*!< PLL3 R clock used as LPTIM6 clock source  */
#define LL_RCC_LPTIM6_CLKSOURCE_LSE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, RCC_CCIPR2_LPTIM6SEL_0 | RCC_CCIPR2_LPTIM6SEL_1)   /*!< LSE  clock used as LPTIM6 clock source  */
#define LL_RCC_LPTIM6_CLKSOURCE_LSI         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, RCC_CCIPR2_LPTIM6SEL_2)                            /*!< LSI  clock used as LPTIM6 clock source  */
#define LL_RCC_LPTIM6_CLKSOURCE_CLKP        LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, RCC_CCIPR2_LPTIM6SEL_0 | RCC_CCIPR2_LPTIM6SEL_2)   /*!< CLKP clock used as LPTIM6 clock source  */
#endif /* LPTIM6 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_FDCAN_CLKSOURCE  Peripheral FDCAN kernel clock source selection
  * @{
  */
#define LL_RCC_FDCAN_CLKSOURCE_HSE         0x00000000U               /*!< HSE clock used as FDCAN kernel clock source */
#define LL_RCC_FDCAN_CLKSOURCE_PLL1Q       RCC_CCIPR5_FDCANSEL_0     /*!< PLL1 Q clock used as FDCAN kernel clock source */
#define LL_RCC_FDCAN_CLKSOURCE_PLL2Q       RCC_CCIPR5_FDCANSEL_1     /*!< PLL2 Q clock used as FDCAN kernel clock source */
#define LL_RCC_FDCAN_CLKSOURCE_NONE        RCC_CCIPR5_FDCANSEL       /*!< NO clock used as FDCAN kernel clock source */
/**
  * @}
  */

#if defined(SAI1)
/** @defgroup RCC_LL_EC_SAI_CLKSOURCE  Peripheral SAIx clock source selection
  * @{
  */
#define LL_RCC_SAI1_CLKSOURCE_PLL1Q         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, 0x00000000U)                                  /*!< PLL1 Q clock used as SAI1 clock source */
#define LL_RCC_SAI1_CLKSOURCE_PLL2P         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, RCC_CCIPR5_SAI1SEL_0)                         /*!< PLL2 P clock used as SAI1 clock source */
#define LL_RCC_SAI1_CLKSOURCE_PLL3P         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, RCC_CCIPR5_SAI1SEL_1)                         /*!< PLL3 P clock used as SAI1 clock source */
#define LL_RCC_SAI1_CLKSOURCE_PIN           LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, RCC_CCIPR5_SAI1SEL_1 | RCC_CCIPR5_SAI1SEL_0)  /*!< External input clock used as SAI1 clock source */
#define LL_RCC_SAI1_CLKSOURCE_CLKP          LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, RCC_CCIPR5_SAI1SEL_2)                         /*!< CLKP clock used as SAI1 clock source */

#define LL_RCC_SAI2_CLKSOURCE_PLL1Q         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, 0x00000000U)                                  /*!< PLL1 Q clock used as SAI2 clock source */
#define LL_RCC_SAI2_CLKSOURCE_PLL2P         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, RCC_CCIPR5_SAI2SEL_0)                         /*!< PLL2 P clock used as SAI2 clock source */
#define LL_RCC_SAI2_CLKSOURCE_PLL3P         LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, RCC_CCIPR5_SAI2SEL_1)                         /*!< PLL3 P clock used as SAI2 clock source */
#define LL_RCC_SAI2_CLKSOURCE_PIN           LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, RCC_CCIPR5_SAI2SEL_1 | RCC_CCIPR5_SAI2SEL_0)  /*!< External input clock used as SAI2 clock source */
#define LL_RCC_SAI2_CLKSOURCE_CLKP          LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, RCC_CCIPR5_SAI2SEL_2)                         /*!< CLKP clock used as SAI2 clock source */
/**
  * @}
  */
#endif /* SAI1 */

#if defined(SDMMC1)
/** @defgroup RCC_LL_EC_SDMMC_CLKSOURCE  Peripheral SDMMCx kernel clock source selection
  * @{
  */
#define LL_RCC_SDMMC1_CLKSOURCE_PLL1Q       LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC1SEL, RCC_CCIPR4_SDMMC1SEL_Pos, 0x00000000U)           /*!< PLL1 Q used as SDMMC1 clock source */
#define LL_RCC_SDMMC1_CLKSOURCE_PLL2R       LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC1SEL, RCC_CCIPR4_SDMMC1SEL_Pos, RCC_CCIPR4_SDMMC1SEL)  /*!< PLL2 R used as SDMMC1 clock source */
#if defined(SDMMC2)
#define LL_RCC_SDMMC2_CLKSOURCE_PLL1Q       LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC2SEL, RCC_CCIPR4_SDMMC2SEL_Pos, 0x00000000U)           /*!< PLL1 Q used as SDMMC2 clock source */
#define LL_RCC_SDMMC2_CLKSOURCE_PLL2R       LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC2SEL, RCC_CCIPR4_SDMMC2SEL_Pos, RCC_CCIPR4_SDMMC2SEL)  /*!< PLL2 R used as SDMMC2 clock source */
#endif /*SDMMC2*/
/**
  * @}
  */
#endif /* SDMMC1 */

/** @defgroup RCC_LL_EC_RNG_CLKSOURCE  Peripheral RNG clock source selection
  * @{
  */
#define LL_RCC_RNG_CLKSOURCE_HSI48       0x00000000U               /*!< HSI48 clock used as RNG clock source */
#define LL_RCC_RNG_CLKSOURCE_PLL1Q       RCC_CCIPR5_RNGSEL_0       /*!< PLL1 Q clock used as RNG clock source */
#define LL_RCC_RNG_CLKSOURCE_LSE         RCC_CCIPR5_RNGSEL_1       /*!< LSE clock used as RNG clock source */
#define LL_RCC_RNG_CLKSOURCE_LSI         RCC_CCIPR5_RNGSEL         /*!< LSI clock used as RNG clock source */
/**
  * @}
  */

#if defined(USB_DRD_FS)
/** @defgroup RCC_LL_EC_USB_CLKSOURCE  Peripheral USB clock source selection
  * @{
  */
#define LL_RCC_USB_CLKSOURCE_NONE        0x00000000U               /*!< No clock used as USB clock source */
#define LL_RCC_USB_CLKSOURCE_PLL1Q       RCC_CCIPR4_USBSEL_0       /*!< PLL1 Q clock used as USB clock source */
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_USB_CLKSOURCE_PLL3Q       RCC_CCIPR4_USBSEL_1       /*!< PLL3 Q clock used as USB clock source */
#else
#define LL_RCC_USB_CLKSOURCE_PLL2Q       RCC_CCIPR4_USBSEL_1       /*!< PLL2 Q clock used as USB clock source */
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_USB_CLKSOURCE_HSI48       RCC_CCIPR4_USBSEL         /*!< HSI48 clock used as USB clock source */
/**
  * @}
  */
#endif /* USB_DRD_FS */

/** @defgroup RCC_LL_EC_ADCDAC_CLKSOURCE  Peripheral ADCDAC clock source selection
  * @{
  */
#define LL_RCC_ADCDAC_CLKSOURCE_HCLK       0x00000000U                                        /*!< AHB clock used as ADCDAC clock source */
#define LL_RCC_ADCDAC_CLKSOURCE_SYSCLK     RCC_CCIPR5_ADCDACSEL_0                             /*!< SYSCLK clock used as ADCDAC clock source */
#define LL_RCC_ADCDAC_CLKSOURCE_PLL2R      RCC_CCIPR5_ADCDACSEL_1                             /*!< PLL2 R clock used as ADCDAC clock source */
#define LL_RCC_ADCDAC_CLKSOURCE_HSE       (RCC_CCIPR5_ADCDACSEL_0 | RCC_CCIPR5_ADCDACSEL_1)   /*!< HSE clock used as ADCDAC clock source */
#define LL_RCC_ADCDAC_CLKSOURCE_HSI        RCC_CCIPR5_ADCDACSEL_2                             /*!< HSI clock used as ADCDAC clock source */
#define LL_RCC_ADCDAC_CLKSOURCE_CSI       (RCC_CCIPR5_ADCDACSEL_0 | RCC_CCIPR5_ADCDACSEL_2)   /*!< CSI clock used as ADCDAC clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_DAC_CLKSOURCE  Peripheral DAC low-power clock source selection
  * @{
  */
#define LL_RCC_DAC_LP_CLKSOURCE_LSE         0x00000000U                            /*!< LSE clock used as DAC low-power clock */
#define LL_RCC_DAC_LP_CLKSOURCE_LSI         RCC_CCIPR5_DACSEL                      /*!< LSI clock used as DAC low-power clock */
/**
  * @}
  */

#if defined(CEC)
/** @defgroup RCC_LL_EC_CEC_CLKSOURCE  Peripheral CEC clock source selection
  * @{
  */
#define LL_RCC_CEC_CLKSOURCE_LSE         0x00000000U                            /*!< LSE clock used as CEC clock */
#define LL_RCC_CEC_CLKSOURCE_LSI         RCC_CCIPR5_CECSEL_0                    /*!< LSI clock used as CEC clock */
#define LL_RCC_CEC_CLKSOURCE_CSI_DIV122  RCC_CCIPR5_CECSEL_1                    /*!< CSI clock divied by 122 used as CEC clock */
#define LL_RCC_CEC_CLKSOURCE_NONE        RCC_CCIPR5_CECSEL                      /*!< NO clock used as CEC clock source */
/**
  * @}
  */
#endif /* CEC */

#if defined(OCTOSPI1)
/** @defgroup RCC_LL_EC_OCTOSPI_CLKSOURCE  Peripheral OCTOSPI kernel clock source selection
  * @{
  */
#define LL_RCC_OSPI_CLKSOURCE_HCLK        0x00000000U                              /*!< AHB clock used as OctoSPI kernel clock source */
#define LL_RCC_OSPI_CLKSOURCE_PLL1Q       RCC_CCIPR4_OCTOSPISEL_0                  /*!< PLL1 Q clock used as OctoSPI kernel clock source */
#define LL_RCC_OSPI_CLKSOURCE_PLL2R       RCC_CCIPR4_OCTOSPISEL_1                  /*!< PLL2 R clock used as OctoSPI kernel clock source */
#define LL_RCC_OSPI_CLKSOURCE_CLKP        RCC_CCIPR4_OCTOSPISEL                    /*!< CLKP clock used as OctoSPI clock source */
/**
  * @}
  */
#endif /* OCTOSPI1 */

#if defined(PLAY1)
/** @defgroup RCC_LL_EC_PLAY1_CLKSOURCE  Peripheral PLAY1 clock source selection
  * @{
  */
#define LL_RCC_PLAY1_CLKSOURCE_PCLK3        ((uint32_t)0x00000000U)
#define LL_RCC_PLAY1_CLKSOURCE_PLL2P        RCC_CCIPR3_PLAY1SEL_0
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_PLAY1_CLKSOURCE_PLL3R        RCC_CCIPR3_PLAY1SEL_1
#endif /* RCC_CR_PLL3ON */
#define LL_RCC_PLAY1_CLKSOURCE_LSE          (RCC_CCIPR3_PLAY1SEL_0 | RCC_CCIPR3_PLAY1SEL_1)
#define LL_RCC_PLAY1_CLKSOURCE_LSI          RCC_CCIPR3_PLAY1SEL_2
#define LL_RCC_PLAY1_CLKSOURCE_CLKP         (RCC_CCIPR3_PLAY1SEL_0 | RCC_CCIPR3_PLAY1SEL_2)
/**
  * @}
  */

/** @defgroup RCC_LL_EC_PLAY1_CLKDIVIDER  Peripheral PLAY1 clock divider selection
  * @{
  */
#define LL_RCC_PLAY1_CLK_DIV_1              ((uint32_t)0x00000000U)                                                        /*!< PLAY1 not divided (default) */
#define LL_RCC_PLAY1_CLK_DIV_2              RCC_CCIPR3_PLAY1DIV_0                                                          /*!< PLAY1 divided by 2 */
#define LL_RCC_PLAY1_CLK_DIV_4              RCC_CCIPR3_PLAY1DIV_1                                                          /*!< PLAY1 divided by 4 */
#define LL_RCC_PLAY1_CLK_DIV_8              (RCC_CCIPR3_PLAY1DIV_0 | RCC_CCIPR3_PLAY1DIV_1)                                /*!< PLAY1 divided by 8 */
#define LL_RCC_PLAY1_CLK_DIV_16             RCC_CCIPR3_PLAY1DIV_2                                                          /*!< PLAY1 divided by 16 */
#define LL_RCC_PLAY1_CLK_DIV_32             (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_0)                                /*!< PLAY1 divided by 32 */
#define LL_RCC_PLAY1_CLK_DIV_64             (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1)                                /*!< PLAY1 divided by 64 */
#define LL_RCC_PLAY1_CLK_DIV_128            (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1 | RCC_CCIPR3_PLAY1DIV_0)        /*!< PLAY1 divided by 128 */
#define LL_RCC_PLAY1_CLK_DIV_256            RCC_CCIPR3_PLAY1DIV_3                                                          /*!< PLAY1 divided by 256 */
#define LL_RCC_PLAY1_CLK_DIV_512            (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_0)                                /*!< PLAY1 divided by 512 */
#define LL_RCC_PLAY1_CLK_DIV_1024           (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_1)                                /*!< PLAY1 divided by 1024 */
#define LL_RCC_PLAY1_CLK_DIV_2048           (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_1 | RCC_CCIPR3_PLAY1DIV_0)        /*!< PLAY1 divided by 2048 */
#define LL_RCC_PLAY1_CLK_DIV_4096           (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2)                                /*!< PLAY1 divided by 4096 */
#define LL_RCC_PLAY1_CLK_DIV_8192           (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_0)        /*!< PLAY1 divided by 8192 */
#define LL_RCC_PLAY1_CLK_DIV_16384          (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1)        /*!< PLAY1 divided by 16384 */
#define LL_RCC_PLAY1_CLK_DIV_32768          (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1 | \
                                             RCC_CCIPR3_PLAY1DIV_0)                                                        /*!< PLAY1 divided by 32768 */
/**
  * @}
  */
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/** @defgroup RCC_LL_EC_USB_OTG_FS_CLKSOURCE  Peripheral USB_OTG_FS clock source selection
  * @{
  */
#define LL_RCC_OTGFS_CLKSOURCE_HSI48          ((uint32_t)0x00000000U)
#define LL_RCC_OTGFS_CLKSOURCE_PLL1Q          RCC_CCIPR4_OTGFSSEL_0
#define LL_RCC_OTGFS_CLKSOURCE_PLL3Q          RCC_CCIPR4_OTGFSSEL_1
#if defined (PWR_USBSCR_OTGHSEN)
#define LL_RCC_OTGFS_CLKSOURCE_CLK48          (RCC_CCIPR4_OTGFSSEL_0 | RCC_CCIPR4_OTGFSSEL_1 )
#endif /* PWR_USBSCR_OTGHSEN */
/**
  * @}
  */
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/** @defgroup RCC_LL_EC_USB_OTG_HS_CLKSOURCE  Peripheral USB_OTG_HS clock source selection
  * @{
  */
#define LL_RCC_OTGHS_CLKSOURCE_HSE          ((uint32_t)0x00000000U)
#define LL_RCC_OTGHS_CLKSOURCE_PLL3Q        RCC_CCIPR4_OTGHSSEL_0
#define LL_RCC_OTGHS_CLKSOURCE_HSE_DIV2     RCC_CCIPR4_OTGHSSEL_1
#define LL_RCC_OTGHS_CLKSOURCE_PLL1Q_DIV2   (RCC_CCIPR4_OTGHSSEL_0 | RCC_CCIPR4_OTGHSSEL_1)
/**
  * @}
  */
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/** @defgroup RCC_LL_EC_OTGPHYREFCK_CLKSOURCE  Peripheral OTGPHYREFCK clock source selection
  * @{
  */
#define LL_RCC_OTGPHYREFCKCLKSOURCE_16M            (RCC_CCIPR4_OTGPHYREFCKSEL_0 | RCC_CCIPR4_OTGPHYREFCKSEL_1 )
#define LL_RCC_OTGPHYREFCKCLKSOURCE_19_2M          RCC_CCIPR4_OTGPHYREFCKSEL_3
#define LL_RCC_OTGPHYREFCKCLKSOURCE_20M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 | RCC_CCIPR4_OTGPHYREFCKSEL_0 )
#define LL_RCC_OTGPHYREFCKCLKSOURCE_24M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 | RCC_CCIPR4_OTGPHYREFCKSEL_1 )
#define LL_RCC_OTGPHYREFCKCLKSOURCE_26M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 |\
                                                    RCC_CCIPR4_OTGPHYREFCKSEL_2 | RCC_CCIPR4_OTGPHYREFCKSEL_1)
#define LL_RCC_OTGPHYREFCKCLKSOURCE_32M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 |\
                                                    RCC_CCIPR4_OTGPHYREFCKSEL_1 | RCC_CCIPR4_OTGPHYREFCKSEL_0)

/**
  * @}
  */
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(OCTOSPI2)
/** @defgroup RCC_LL_EC_OCTOSPI2_CLKSOURCE  Peripheral OCTOSPI2 kernel clock source selection
  * @{
  */
#define LL_RCC_OSPI2_CLKSOURCE_HCLK        0x00000000U                                         /*!< AHB clock used as OctoSPI2 kernel clock source */
#define LL_RCC_OSPI2_CLKSOURCE_PLL1Q       RCC_CCIPR5_OCTOSPI2SEL_0                            /*!< PLL1 Q clock used as OctoSPI2 kernel clock source */
#define LL_RCC_OSPI2_CLKSOURCE_PLL2R       RCC_CCIPR5_OCTOSPI2SEL_1                            /*!< PLL2 R clock used as OctoSPI2 kernel clock source */
#define LL_RCC_OSPI2_CLKSOURCE_CLKP        (RCC_CCIPR5_OCTOSPI2SEL_0 | RCC_CCIPR5_OCTOSPI2SEL_1) /*!< CLKP clock used as OctoSPI2 clock source */
/**
  * @}
  */
#endif /* OCTOSPI2 */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/** @defgroup RCC_LL_EC_ETHPTP_CLKSOURCE  Peripheral ETHPTP kernel clock source selection
  * @{
  */
#define LL_RCC_ETHPTP_CLKSOURCE_HCLK               ((uint32_t)0x00000000U)
#define LL_RCC_ETHPTP_CLKSOURCE_PLL1R              RCC_CCIPR5_ETHPTPCLKSEL_0
#define LL_RCC_ETHPTP_CLKSOURCE_PLL1Q              RCC_CCIPR5_ETHPTPCLKSEL_1
#if defined(RCC_CR_PLL3ON)
#define LL_RCC_ETHPTP_CLKSOURCE_PLL3P              (RCC_CCIPR5_ETHPTPCLKSEL_0 | RCC_CCIPR5_ETHPTPCLKSEL_1)
#endif /* RCC_CR_PLL3ON */

/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/** @defgroup RCC_LL_ETHPTP_CLK_Divider    Peripheral ETHPTP Clock Divider selection
  * @{
  */
#define LL_RCC_ETHPTP_CLK_DIV_1     ((uint32_t)0x00000000U)                                                     /*!< ETHPTP not divided (default) */
#define LL_RCC_ETHPTP_CLK_DIV_2     RCC_CCIPR4_ETHPTPDIV_0                                                      /*!< ETHPTP divided by 2 */
#define LL_RCC_ETHPTP_CLK_DIV_3     RCC_CCIPR4_ETHPTPDIV_1                                                      /*!< ETHPTP divided by 3 */
#define LL_RCC_ETHPTP_CLK_DIV_4     (RCC_CCIPR4_ETHPTPDIV_0 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 4 */
#define LL_RCC_ETHPTP_CLK_DIV_5     RCC_CCIPR4_ETHPTPDIV_2                                                      /*!< ETHPTP divided by 5 */
#define LL_RCC_ETHPTP_CLK_DIV_6     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_0)                           /*!< ETHPTP divided by 6 */
#define LL_RCC_ETHPTP_CLK_DIV_7     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 7 */
#define LL_RCC_ETHPTP_CLK_DIV_8     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 8 */
#define LL_RCC_ETHPTP_CLK_DIV_9     RCC_CCIPR4_ETHPTPDIV_3                                                      /*!< ETHPTP divided by 9 */
#define LL_RCC_ETHPTP_CLK_DIV_10    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_0)                           /*!< ETHPTP divided by 10 */
#define LL_RCC_ETHPTP_CLK_DIV_11    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 11 */
#define LL_RCC_ETHPTP_CLK_DIV_12    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 12 */
#define LL_RCC_ETHPTP_CLK_DIV_13    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2)                           /*!< ETHPTP divided by 13 */
#define LL_RCC_ETHPTP_CLK_DIV_14    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 14 */
#define LL_RCC_ETHPTP_CLK_DIV_15    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1)  /*!< ETHPTP divided by 15 */
#define LL_RCC_ETHPTP_CLK_DIV_16    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | \
                                     RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)                            /*!< ETHPTP divided by 16 */
#endif /* RCC_CCIPR4_ETHPTPDIV */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/** @defgroup RCC_LL_EC_ETH_CLKSOURCE  Peripheral ETH kernel clock source selection
  * @{
  */
#define LL_RCC_ETH_CLKSOURCE_HSE               ((uint32_t)0x00000000U)
#define LL_RCC_ETH_CLKSOURCE_PLL1Q             RCC_CCIPR4_ETHCLKSEL
/**
  * @}
  */
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(LTDC)
/** @defgroup RCC_LL_EC_LTDC_CLKSOURCE  Peripheral LTDC kernel clock source selection
  * @{
  */
#define LL_RCC_LTDC_CLKSOURCE_PLL3R               ((uint32_t)0x00000000U)
#define LL_RCC_LTDC_CLKSOURCE_PLL2R               RCC_CCIPR5_LTDCSEL

/**
  * @}
  */
#endif /* LTDC */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/** @defgroup RCC_LL_EC_ETHREF_CLKSOURCE  Peripheral ETHREF kernel clock source selection
  * @{
  */
#define LL_RCC_ETHREF_CLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define LL_RCC_ETHREF_CLKSOURCE_PLL1R               RCC_CCIPR5_ETHREFCLKSEL

/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

#if defined(ADF1)
/** @defgroup RCC_LL_EC_ADF1_CLKSOURCE  Peripheral ADF1 kernel clock source selection
  * @{
  */
#define LL_RCC_ADF1_CLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define LL_RCC_ADF1_CLKSOURCE_PLL2P               RCC_CCIPR5_ADF1SEL_0
#define LL_RCC_ADF1_CLKSOURCE_PLL3P               RCC_CCIPR5_ADF1SEL_1
#define LL_RCC_ADF1_CLKSOURCE_PIN                 (RCC_CCIPR5_ADF1SEL_0 | RCC_CCIPR5_ADF1SEL_1)
#define LL_RCC_ADF1_CLKSOURCE_CLKP                RCC_CCIPR5_ADF1SEL_2
/**
  * @}
  */
#endif /* ADF1 */

#if defined(MDF1)
/** @defgroup RCC_LL_EC_MDF1_CLKSOURCE  Peripheral MDF1 kernel clock source selection
  * @{
  */
#define LL_RCC_MDF1_CLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define LL_RCC_MDF1_CLKSOURCE_PLL2P               RCC_CCIPR5_MDF1SEL_0
#define LL_RCC_MDF1_CLKSOURCE_PLL3P               RCC_CCIPR5_MDF1SEL_1
#define LL_RCC_MDF1_CLKSOURCE_PIN                 (RCC_CCIPR5_MDF1SEL_0 | RCC_CCIPR5_MDF1SEL_1)
#define LL_RCC_MDF1_CLKSOURCE_CLKP                RCC_CCIPR5_MDF1SEL_2
/**
  * @}
  */
#endif /* MDF1 */

/** @defgroup RCC_LL_EC_CLKP_CLKSOURCE  Peripheral CLKP clock source selection
  * @{
  */
#define LL_RCC_CLKP_CLKSOURCE_HSI         0x00000000U                           /*!< HSI  clock used as CLKP clock source  */
#define LL_RCC_CLKP_CLKSOURCE_CSI         RCC_CCIPR5_CKERPSEL_0                 /*!< CSI  clock used as CLKP clock source  */
#define LL_RCC_CLKP_CLKSOURCE_HSE         RCC_CCIPR5_CKERPSEL_1                 /*!< HSE  clock used as CLKP clock source  */
#define LL_RCC_CLKP_CLKSOURCE_NONE        RCC_CCIPR5_CKERPSEL                   /*!< No clock selected as CLKP clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_USART Peripheral USARTx get clock source
  * @{
  */
#define LL_RCC_USART1_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART1SEL, RCC_CCIPR1_USART1SEL_Pos, 0x00000000UL)    /*!< USART1 Clock source selection */
#define LL_RCC_USART2_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART2SEL, RCC_CCIPR1_USART2SEL_Pos, 0x00000000UL)    /*!< USART2 Clock source selection */
#define LL_RCC_USART3_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART3SEL, RCC_CCIPR1_USART3SEL_Pos, 0x00000000UL)    /*!< USART3 Clock source selection */
#if defined(USART6)
#define LL_RCC_USART6_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART6SEL, RCC_CCIPR1_USART6SEL_Pos, 0x00000000UL)    /*!< USART6 Clock source selection */
#endif /* USART6 */
#if defined(USART10)
#define LL_RCC_USART10_CLKSOURCE         LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_USART10SEL, RCC_CCIPR1_USART10SEL_Pos, 0x00000000UL)  /*!< USART10 Clock source selection */
#endif /* USART10 */
#if defined(USART11)
#define LL_RCC_USART11_CLKSOURCE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_USART11SEL, RCC_CCIPR2_USART11SEL_Pos, 0x00000000UL)  /*!< USART11 Clock source selection */
#endif /* USART11 */
/**
  * @}
  */

#if defined(UART4)
/** @defgroup RCC_LL_EC_UART Peripheral UARTx get clock source
  * @{
  */
#define LL_RCC_UART4_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART4SEL, RCC_CCIPR1_UART4SEL_Pos, 0x00000000UL)    /*!< UART4 Clock source selection */
#define LL_RCC_UART5_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART5SEL, RCC_CCIPR1_UART5SEL_Pos, 0x00000000UL)    /*!< UART5 Clock source selection */
#define LL_RCC_UART7_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART7SEL, RCC_CCIPR1_UART7SEL_Pos, 0x00000000UL)    /*!< UART7 Clock source selection */
#define LL_RCC_UART8_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART8SEL, RCC_CCIPR1_UART8SEL_Pos, 0x00000000UL)    /*!< UART8 Clock source selection */
#define LL_RCC_UART9_CLKSOURCE          LL_CLKSOURCE(CCIPR1_OFFSET, RCC_CCIPR1_UART9SEL, RCC_CCIPR1_UART9SEL_Pos, 0x00000000UL)    /*!< UART9 Clock source selection */
#define LL_RCC_UART12_CLKSOURCE         LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_UART12SEL, RCC_CCIPR2_UART12SEL_Pos, 0x00000000UL)  /*!< UART12 Clock source selection */
/**
  * @}
  */
#endif /*UART4*/

/** @defgroup RCC_LL_EC_SPI Peripheral SPIx get clock source
  * @{
  */
#define LL_RCC_SPI1_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI1SEL, RCC_CCIPR3_SPI1SEL_Pos, 0x00000000UL)    /*!< SPI1 Clock source selection */
#define LL_RCC_SPI2_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI2SEL, RCC_CCIPR3_SPI2SEL_Pos, 0x00000000UL)    /*!< SPI2 Clock source selection */
#define LL_RCC_SPI3_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI3SEL, RCC_CCIPR3_SPI3SEL_Pos, 0x00000000UL)    /*!< SPI3 Clock source selection */
#if defined(SPI4)
#define LL_RCC_SPI4_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI4SEL, RCC_CCIPR3_SPI4SEL_Pos, 0x00000000UL)    /*!< SPI4 Clock source selection */
#endif /* SPI4 */
#if defined(SPI5)
#define LL_RCC_SPI5_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI5SEL, RCC_CCIPR3_SPI5SEL_Pos, 0x00000000UL)    /*!< SPI5 Clock source selection */
#endif /* SPI5 */
#if defined(SPI6)
#define LL_RCC_SPI6_CLKSOURCE           LL_CLKSOURCE(CCIPR3_OFFSET, RCC_CCIPR3_SPI6SEL, RCC_CCIPR3_SPI6SEL_Pos, 0x00000000UL)    /*!< SPI6 Clock source selection */
#endif /* SPI6 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LPUART Peripheral LPUARTx get clock source
  * @{
  */
#define LL_RCC_LPUART1_CLKSOURCE          RCC_CCIPR3_LPUART1SEL /*!< LPUART1 Clock source selection */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_I2C Peripheral I2Cx get clock source
  * @{
  */
#define LL_RCC_I2C1_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C1SEL, RCC_CCIPR4_I2C1SEL_Pos, 0x00000000UL) /*!< I2C1 Clock source selection */
#define LL_RCC_I2C2_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C2SEL, RCC_CCIPR4_I2C2SEL_Pos, 0x00000000UL) /*!< I2C2 Clock source selection */
#if defined(I2C3)
#define LL_RCC_I2C3_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C3SEL, RCC_CCIPR4_I2C3SEL_Pos, 0x00000000UL) /*!< I2C3 Clock source selection */
#endif /* I2C3 */
#if defined(I2C4)
#define LL_RCC_I2C4_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I2C4SEL, RCC_CCIPR4_I2C4SEL_Pos, 0x00000000UL) /*!< I2C4 Clock source selection */
#endif /* I2C4 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_I3C Peripheral I3Cx get clock source
  * @{
  */
#define LL_RCC_I3C1_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C1SEL, RCC_CCIPR4_I3C1SEL_Pos, 0x00000000UL) /*!< I3C1 Clock source selection */
#if defined(I3C2)
#define LL_RCC_I3C2_CLKSOURCE             LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_I3C2SEL, RCC_CCIPR4_I3C2SEL_Pos, 0x00000000UL) /*!< I3C2 Clock source selection */
#endif /* I3C2 */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_LPTIM Peripheral LPTIMx get clock source
  * @{
  */
#define LL_RCC_LPTIM1_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM1SEL, RCC_CCIPR2_LPTIM1SEL_Pos, 0x00000000UL)       /*!< LPTIM1 Clock source selection */
#define LL_RCC_LPTIM2_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM2SEL, RCC_CCIPR2_LPTIM2SEL_Pos, 0x00000000UL)       /*!< LPTIM2 Clock source selection */
#if defined(LPTIM3)
#define LL_RCC_LPTIM3_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM3SEL, RCC_CCIPR2_LPTIM3SEL_Pos, 0x00000000UL)       /*!< LPTIM3 Clock source selection */
#endif /* LPTIM3 */
#if defined(LPTIM4)
#define LL_RCC_LPTIM4_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM4SEL, RCC_CCIPR2_LPTIM4SEL_Pos, 0x00000000UL)       /*!< LPTIM4 Clock source selection */
#endif /* LPTIM4 */
#if defined(LPTIM5)
#define LL_RCC_LPTIM5_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM5SEL, RCC_CCIPR2_LPTIM5SEL_Pos, 0x00000000UL)       /*!< LPTIM5 Clock source selection */
#endif /* LPTIM5 */
#if defined(LPTIM6)
#define LL_RCC_LPTIM6_CLKSOURCE           LL_CLKSOURCE(CCIPR2_OFFSET, RCC_CCIPR2_LPTIM6SEL, RCC_CCIPR2_LPTIM6SEL_Pos, 0x00000000UL)       /*!< LPTIM6 Clock source selection */
#endif /* LPTIM6 */
/**
  * @}
  */

#if defined(SAI1)
/** @defgroup RCC_LL_EC_SAI  Peripheral SAIx get clock source
  * @{
  */
#define LL_RCC_SAI1_CLKSOURCE             LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI1SEL, RCC_CCIPR5_SAI1SEL_Pos, 0x00000000UL)  /*!< SAI1 Clock source selection */
#define LL_RCC_SAI2_CLKSOURCE             LL_CLKSOURCE(CCIPR5_OFFSET, RCC_CCIPR5_SAI2SEL, RCC_CCIPR5_SAI2SEL_Pos, 0x00000000UL)  /*!< SAI2 Clock source selection */
/**
  * @}
  */
#endif /* SAI1 */

#if defined(SDMMC1)
/** @defgroup RCC_LL_EC_SDMMC Peripheral SDMMC get clock source
  * @{
  */
#define LL_RCC_SDMMC1_CLKSOURCE            LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC1SEL, RCC_CCIPR4_SDMMC1SEL_Pos, 0x00000000UL)     /*!< SDMMC1 Kernel Clock source selection */
#if defined(SDMMC2)
#define LL_RCC_SDMMC2_CLKSOURCE            LL_CLKSOURCE(CCIPR4_OFFSET, RCC_CCIPR4_SDMMC2SEL, RCC_CCIPR4_SDMMC2SEL_Pos, 0x00000000UL)     /*!< SDMMC2 Kernel Clock source selection */
#endif /*SDMMC2*/
/**
  * @}
  */
#endif /* SDMMC1 */

/** @defgroup RCC_LL_EC_RNG  Peripheral RNG get clock source
  * @{
  */
#define LL_RCC_RNG_CLKSOURCE              RCC_CCIPR5_RNGSEL    /*!< RNG Clock source selection */
/**
  * @}
  */

#if defined(USB_DRD_FS)
/** @defgroup RCC_LL_EC_USB  Peripheral USB get clock source
  * @{
  */
#define LL_RCC_USB_CLKSOURCE              RCC_CCIPR4_USBSEL    /*!< USB Clock source selection */
/**
  * @}
  */
#endif /* USB_DRD_FS */

/** @defgroup RCC_LL_EC_ADCDAC  Peripheral ADCDAC get clock source
  * @{
  */
#define LL_RCC_ADCDAC_CLKSOURCE           RCC_CCIPR5_ADCDACSEL  /*!< ADCDACs Clock source selection */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_DAC  Peripheral DAC get low-power clock source
  * @{
  */
#define LL_RCC_DAC_LP_CLKSOURCE     RCC_CCIPR5_DACSEL   /*!< DAC low-power Clock source selection */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_CEC  Peripheral CEC get clock source
  * @{
  */
#define LL_RCC_CEC_CLKSOURCE             RCC_CCIPR5_CECSEL
/**
  * @}
  */

/** @defgroup RCC_LL_EC_FDCAN Peripheral FDCAN get kernel clock source
  * @{
  */
#define LL_RCC_FDCAN_CLKSOURCE           RCC_CCIPR5_FDCANSEL    /*!< FDCAN kernel Clock source selection */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_OCTOSPI  Peripheral OCTOSPI get clock source
  * @{
  */
#define LL_RCC_OCTOSPI_CLKSOURCE          RCC_CCIPR4_OCTOSPISEL    /*!< OctoSPI Clock source selection */
/**
  * @}
  */

#if defined(PLAY1)
/** @defgroup RCC_LL_EC_PLAY1_SRC  Peripheral PLAY1 get clock source
  * @{
  */
#define LL_RCC_PLAY1_CLKSOURCE            RCC_CCIPR3_PLAY1SEL    /*!< PLAY1 Clock source selection */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_PLAY1_DIV  Peripheral PLAY1 get clock divider
  * @{
  */
#define LL_RCC_PLAY1_CLKDIVIDER            RCC_CCIPR3_PLAY1DIV    /*!< PLAY1 Clock divider selection */
/**
  * @}
  */
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/** @defgroup RCC_LL_EC_OTGFS  Peripheral OTGFS get clock source
  * @{
  */
#define LL_RCC_OTGFS_CLKSOURCE          RCC_CCIPR4_OTGFSSEL    /*!< OTGFS Clock source selection */
/**
  * @}
  */
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/** @defgroup RCC_LL_EC_OTGHS  Peripheral OTGHS get clock source
  * @{
  */
#define LL_RCC_OTGHS_CLKSOURCE          RCC_CCIPR4_OTGHSSEL    /*!< OTGHS Clock source selection */
/**
  * @}
  */
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/** @defgroup RCC_LL_EC_OTGPHY  Peripheral OTGPHY get clock source
  * @{
  */
#define LL_RCC_OTGPHY_CLKSOURCE          RCC_CCIPR4_OTGPHYREFCKSEL    /*!< OTGPHY Clock source selection */
/**
  * @}
  */
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/** @defgroup RCC_LL_EC_ETH  Peripheral ETH get clock source
  * @{
  */
#define LL_RCC_ETH_CLKSOURCE          RCC_CCIPR4_ETHCLKSEL    /*!< ETH Clock source selection */
/**
  * @}
  */
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(OCTOSPI2)
/** @defgroup RCC_LL_EC_OCTOSPI2  Peripheral OCTOSPI2 get clock source
  * @{
  */
#define LL_RCC_OCTOSPI2_CLKSOURCE          RCC_CCIPR5_OCTOSPI2SEL    /*!< OctoSPI2 Clock source selection */
/**
  * @}
  */
#endif /* OCTOSOI2 */

#if defined(LTDC)
/** @defgroup RCC_LL_EC_LTDC  Peripheral LTDC get clock source
  * @{
  */
#define LL_RCC_LTDC_CLKSOURCE          RCC_CCIPR5_LTDCSEL    /*!< LTDC Clock source selection */
/**
  * @}
  */
#endif /* LTDC */

#if defined(ADF1)
/** @defgroup RCC_LL_EC_ADF1  Peripheral ADF1 get clock source
  * @{
  */
#define LL_RCC_ADF1_CLKSOURCE          RCC_CCIPR5_ADF1SEL    /*!< ADF1 Clock source selection */
/**
  * @}
  */
#endif /* ADF1 */

#if defined(MDF1)
/** @defgroup RCC_LL_EC_MDF1  Peripheral MDF1 get clock source
  * @{
  */
#define LL_RCC_MDF1_CLKSOURCE          RCC_CCIPR5_MDF1SEL    /*!< MDF1 Clock source selection */
/**
  * @}
  */
#endif /* MDF1 */

/** @defgroup RCC_LL_EC_CLKP Peripheral CLKP get clock source
  * @{
  */
#define LL_RCC_CLKP_CLKSOURCE            RCC_CCIPR5_CKERPSEL    /*!< CLKP Clock source selection */
/**
  * @}
  */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/** @defgroup RCC_LL_EC_ETHPTP_SRC  Peripheral ETHPTP get clock source
  * @{
  */
#define LL_RCC_ETHPTP_CLKSOURCE          RCC_CCIPR5_ETHPTPCLKSEL    /*!< ETHPTP Clock source selection */
/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/** @defgroup RCC_LL_EC_ETHPTP_DIV  Peripheral ETHPTP get clock divider
  * @{
  */
#define LL_RCC_ETHPTP_CLKDIVIDER          RCC_CCIPR4_ETHPTPDIV    /*!< ETHPTP Clock divider selection */
/**
  * @}
  */

#endif /* RCC_CCIPR4_ETHPTPDIV */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/** @defgroup RCC_LL_EC_ETHREF  Peripheral ETHREF get clock source
  * @{
  */
#define LL_RCC_ETHREF_CLKSOURCE          RCC_CCIPR5_ETHREFCLKSEL    /*!< ETHREF Clock source selection */
/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

/** @defgroup RCC_LL_EC_PLL1SOURCE  PLL1 entry clock source
  * @{
  */
#define LL_RCC_PLL1SOURCE_NONE             0x00000000U                                       /*!< No clock selected as main PLL1 entry clock source */
#define LL_RCC_PLL1SOURCE_HSI              RCC_PLL1CFGR_PLL1SRC_0                            /*!< HSI clock selected as main PLL1 entry clock source */
#define LL_RCC_PLL1SOURCE_CSI              RCC_PLL1CFGR_PLL1SRC_1                            /*!< CSI clock selected as main PLL1 entry clock source */
#define LL_RCC_PLL1SOURCE_HSE              (RCC_PLL1CFGR_PLL1SRC_0 | RCC_PLL1CFGR_PLL1SRC_1) /*!< HSE clock selected as main PLL1 entry clock source */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_PLLINPUTRANGE   All PLLs input ranges
  * @{
  */
#define LL_RCC_PLLINPUTRANGE_1_2           0x00000000U                                       /*!< VCO input range: 1 to 2 MHz  */
#define LL_RCC_PLLINPUTRANGE_2_4           0x00000001U                                       /*!< VCO input range: 2 to 4 MHz  */
#define LL_RCC_PLLINPUTRANGE_4_8           0x00000002U                                       /*!< VCO input range: 4 to 8 MHz  */
#define LL_RCC_PLLINPUTRANGE_8_16          0x00000003U                                       /*!< VCO input range: 8 to 16 MHz */
/**
  * @}
  */

/** @defgroup RCC_LL_EC_PLLOUTPUTRANGE All PLLs output ranges
  * @{
  */
#define LL_RCC_PLLVCORANGE_WIDE            0x00000000U       /*!< VCO output range: 192 to 836 MHz */
#define LL_RCC_PLLVCORANGE_MEDIUM          0x00000001U       /*!< VCO output range: 150 to 420 MHz */

/**
  * @}
  */

/** @defgroup RCC_LL_EC_PLL2SOURCE  PLL2 entry clock source
  * @{
  */
#define LL_RCC_PLL2SOURCE_NONE             0x00000000U                                       /*!< No clock selected as main PLL2 entry clock source */
#define LL_RCC_PLL2SOURCE_HSI              RCC_PLL2CFGR_PLL2SRC_0                            /*!< HSI clock selected as main PLL2 entry clock source */
#define LL_RCC_PLL2SOURCE_CSI              RCC_PLL2CFGR_PLL2SRC_1                            /*!< CSI clock selected as main PLL2 entry clock source */
#define LL_RCC_PLL2SOURCE_HSE              (RCC_PLL2CFGR_PLL2SRC_0 | RCC_PLL2CFGR_PLL2SRC_1) /*!< HSE clock selected as main PLL2 entry clock source */
/**
  * @}
  */
#if defined(RCC_CR_PLL3ON)
/** @defgroup RCC_LL_EC_PLL3SOURCE  PLL3 entry clock source
  * @{
  */
#define LL_RCC_PLL3SOURCE_NONE             0x00000000U                                       /*!< No clock selected as main PLL3 entry clock source */
#define LL_RCC_PLL3SOURCE_HSI              RCC_PLL3CFGR_PLL3SRC_0                            /*!< HSI clock selected as main PLL3 entry clock source */
#define LL_RCC_PLL3SOURCE_CSI              RCC_PLL3CFGR_PLL3SRC_1                            /*!< CSI clock selected as main PLL3 entry clock source */
#define LL_RCC_PLL3SOURCE_HSE              (RCC_PLL3CFGR_PLL3SRC_0 | RCC_PLL3CFGR_PLL3SRC_1) /*!< HSE clock selected as main PLL3 entry clock source */
/**
  * @}
  */
#endif /* RCC_CR_PLL3ON */
#if defined(RCC_SECCFGR_HSISEC)
/** @defgroup RCC_LL_EC_SECURE_ATTRIBUTES Secure attributes
  * @note Only available when system implements security (TZEN=1)
  * @{
  */
#define LL_RCC_ALL_SEC                     RCC_SECURE_MASK         /*!< Security on all RCC resources          */
#define LL_RCC_ALL_NSEC                    0U                      /*!< No security on RCC resources (default) */

#define LL_RCC_HSI_SEC                     RCC_SECCFGR_HSISEC      /*!< HSI clock configuration secure-only access */
#define LL_RCC_HSI_NSEC                    0U                      /*!< HSI clock configuration secure/non-secure access */
#define LL_RCC_HSE_SEC                     RCC_SECCFGR_HSESEC      /*!< HSE clock configuration secure-only access */
#define LL_RCC_HSE_NSEC                    0U                      /*!< HSE clock configuration secure/non-secure access */
#define LL_RCC_CSI_SEC                     RCC_SECCFGR_CSISEC      /*!< CSI clock configuration secure-only access */
#define LL_RCC_CSI_NSEC                    0U                      /*!< CSI clock configuration secure/non-secure access */
#define LL_RCC_LSI_SEC                     RCC_SECCFGR_LSISEC      /*!< LSI clock configuration secure-only access */
#define LL_RCC_LSI_NSEC                    0U                      /*!< LSI clock configuration secure/non-secure access */
#define LL_RCC_LSE_SEC                     RCC_SECCFGR_LSESEC      /*!< LSE clock configuration secure-only access */
#define LL_RCC_LSE_NSEC                    0U                      /*!< LSE clock configuration secure/non-secure access */
#define LL_RCC_SYSCLK_SEC                  RCC_SECCFGR_SYSCLKSEC   /*!< SYSCLK clock; STOPWUCK and MCO output configuration secure-only access */
#define LL_RCC_SYSCLK_NSEC                 0U                      /*!< SYSCLK clock; STOPWUCK and MCO output configuration secure/non-secure access */
#define LL_RCC_PRESCALERS_SEC              RCC_SECCFGR_PRESCSEC    /*!< AHBx/APBx prescaler configuration secure-only access */
#define LL_RCC_PRESCALERS_NSEC             0U                      /*!< AHBx/APBx prescaler configuration secure/non-secure access */
#define LL_RCC_PLL1_SEC                    RCC_SECCFGR_PLL1SEC     /*!< main PLL clock configuration secure-only access */
#define LL_RCC_PLL1_NSEC                   0U                      /*!< main PLL clock configuration secure/non-secure access */
#define LL_RCC_PLL2_SEC                    RCC_SECCFGR_PLL2SEC     /*!< PLL2 clock configuration secure-only access */
#define LL_RCC_PLL2_NSEC                   0U                      /*!< PLL2 clock configuration secure/non-secure access */
#define LL_RCC_PLL3_SEC                    RCC_SECCFGR_PLL3SEC     /*!< PLL3 clock configuration secure-only access */
#define LL_RCC_PLL3_NSEC                   0U                      /*!< PLL3 clock configuration secure/non-secure access */
#define LL_RCC_HSI48_SEC                   RCC_SECCFGR_HSI48SEC    /*!< HSI48 clock configuration secure-only access */
#define LL_RCC_HSI48_NSEC                  0U                      /*!< HSI48 clock configuration secure/non-secure access */
#define LL_RCC_RESET_FLAGS_SEC             RCC_SECCFGR_RMVFSEC     /*!< Remove reset flag secure-ony access */
#define LL_RCC_RESET_FLAGS_NSEC            0U                      /*!< Remove reset flag secure/non-secure access */
#define LL_RCC_CKPERSEL_SEC                RCC_SECCFGR_CKPERSELSEC /*!< Periph clock configuration secure-ony access */
#define LL_RCC_CKPERSEL_NSEC               0U                      /*!< Periph clock configuration secure/non-secure access */
/**
  * @}
  */
#endif /* RCC_SECCFGR_HSISEC */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup RCC_LL_Exported_Macros RCC Exported Macros
  * @{
  */

/** @defgroup RCC_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in RCC register
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_RCC_WriteReg(__REG__, __VALUE__) WRITE_REG(RCC->__REG__, (__VALUE__))

/**
  * @brief  Read a value in RCC register
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_RCC_ReadReg(__REG__) READ_REG(RCC->__REG__)
/**
  * @}
  */

/** @defgroup RCC_LL_EM_CALC_FREQ Calculate frequencies
  * @{
  */

/**
  * @brief  Helper macro to calculate the PLL1P clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL1CLK_P_FREQ (HSE_VALUE,@ref LL_RCC_PLL1_GetM (),
  *             @ref LL_RCC_PLL1_GetN (), @ref LL_RCC_PLL1_GetP ());
  * @param  __INPUTFREQ__ PLL1 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL1M__ parameter can be a value between 1 and 63
  * @param __PLL1N__ parameter can be a value between 4 and 512
  * @param __PLL1P__ parameter can be a value between 1 and 128 (odd values not allowed)
  * @retval PLL1P clock frequency (in Hz)
  */

#define __LL_RCC_CALC_PLL1CLK_P_FREQ(__INPUTFREQ__, __PLL1M__, __PLL1N__, __PLL1P__) \
  ((((__INPUTFREQ__) /(__PLL1M__)) * (__PLL1N__)) / (__PLL1P__))

/**
  * @brief  Helper macro to calculate the PLL1Q clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL1CLK_Q_FREQ (HSE_VALUE,@ref LL_RCC_PLL1_GetM (),
  *             @ref LL_RCC_PLL1_GetN (), @ref LL_RCC_PLL1_GetQ ());
  * @param  __INPUTFREQ__ PLL1 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL1M__ parameter can be a value between 1 and 63
  * @param __PLL1N__ parameter can be a value between 4 and 512
  * @param __PLL1Q__ parameter can be a value between 2 and 128
  * @retval PLL1Q clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL1CLK_Q_FREQ(__INPUTFREQ__, __PLL1M__, __PLL1N__, __PLL1Q__) \
  ((((__INPUTFREQ__) /(__PLL1M__)) * (__PLL1N__)) / (__PLL1Q__))

/**
  * @brief  Helper macro to calculate the PLL1R clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL1CLK_R_FREQ (HSE_VALUE,@ref LL_RCC_PLL1_GetM (),
  *             @ref LL_RCC_PLL1_GetN (), @ref LL_RCC_PLL1_GetN ());
  * @param  __INPUTFREQ__ PLL1 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL1M__ parameter can be a value between 1 and 63
  * @param __PLL1N__ parameter can be a value between 4 and 512
  * @param __PLL1R__ parameter can be a value between 1 and 128
  * @retval PLL1R clock frequency (in Hz)
  */

#define __LL_RCC_CALC_PLL1CLK_R_FREQ(__INPUTFREQ__, __PLL1M__, __PLL1N__, __PLL1R__) \
  ((((__INPUTFREQ__) /(__PLL1M__)) * (__PLL1N__)) / (__PLL1R__))

/**
  * @brief  Helper macro to calculate the PLL2P clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL2CLK_P_FREQ (HSE_ALUE,@ref LL_RCC_PLL2_GetM (),
  *             @ref LL_RCC_PLL2_GetN (), @ref LL_RCC_PLL2_GetP ());
  * @param  __INPUTFREQ__ PLL Input frequency (based on HSI/HSE/CSI)
  * @param __PLL2M__ parameter can be a value between 1 and 63
  * @param __PLL2N__ parameter can be a value between 4 and 512
  * @param __PLL2P__ parameter can be a value between 2 and 128
  * @retval PLL2P clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL2CLK_P_FREQ(__INPUTFREQ__, __PLL2M__, __PLL2N__, __PLL2P__) \
  ((((__INPUTFREQ__) /(__PLL2M__)) * (__PLL2N__)) / (__PLL2P__))

/**
  * @brief  Helper macro to calculate the PLL2Q clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL2CLK_Q_FREQ (HSE_VALUE,@ref LL_RCC_PLL2_GetM (),
  *             @ref LL_RCC_PLL2_GetN (), @ref LL_RCC_PLL2_GetQ ());
  * @param  __INPUTFREQ__ PLL Input frequency (based on HSI/HSE/CSI)
  * @param __PLL2M__ parameter can be a value between 1 and 63
  * @param __PLL2N__ parameter can be a value between 4 and 512
  * @param __PLL2Q__ parameter can be a value between 1 and 128
  * @retval PLL2Q clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL2CLK_Q_FREQ(__INPUTFREQ__, __PLL2M__, __PLL2N__, __PLL2Q__) \
  ((((__INPUTFREQ__) /(__PLL2M__)) * (__PLL2N__)) / (__PLL2Q__))

/**
  * @brief  Helper macro to calculate the PLL2R clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL2CLK_R_FREQ (HSE_VALUE,@ref LL_RCC_PLL2_GetM (),
  *             @ref LL_RCC_PLL2_GetN (), @ref LL_RCC_PLL2_GetR ());
  * @param  __INPUTFREQ__ PLL2 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL2M__ parameter can be a value between 1 and 63
  * @param __PLL2N__ parameter can be a value between 4 and 512
  * @param __PLL2R__ parameter can be a value between 1 and 128
  * @retval PLL2R clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL2CLK_R_FREQ(__INPUTFREQ__, __PLL2M__, __PLL2N__, __PLL2R__) \
  ((((__INPUTFREQ__) /(__PLL2M__)) * (__PLL2N__)) / (__PLL2R__))
#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Helper macro to calculate the PLL3P clock frequency
  * @note ex: @ref __LL_RCC_CALC_PLL3CLK_P_FREQ (HSE_VALUE,@ref LL_RCC_PLL3_GetM (),
  *             @ref LL_RCC_PLL3_GetN (), @ref LL_RCC_PLL3_GetP ());
  * @param  __INPUTFREQ__ PLL3 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL3M__ parameter can be a value between 1 and 63
  * @param __PLL3N__ parameter can be a value between 4 and 512
  * @param __PLL3P__ parameter can be a value between 2 and 128
  * @retval PLL3P clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL3CLK_P_FREQ(__INPUTFREQ__, __PLL3M__, __PLL3N__, __PLL3P__) \
  ((((__INPUTFREQ__) /(__PLL3M__)) * (__PLL3N__)) / (__PLL3P__))

/**
  * @brief  Helper macro to calculate the PLL3 frequency
  * @note ex: @ref __LL_RCC_CALC_PLL3CLK_Q_FREQ (HSE_VALUE,@ref LL_RCC_PLL3_GetM (),
  *             @ref LL_RCC_PLL3_GetN (), @ref LL_RCC_PLL3_GetQ ());
  * @param  __INPUTFREQ__ PLL3 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL3M__ parameter can be a value between 1 and 63
  * @param __PLL3N__ parameter can be a value between 4 and 512
  * @param __PLL3Q__ parameter can be a value between 1 and 128
  * @retval PLL3Q clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL3CLK_Q_FREQ(__INPUTFREQ__, __PLL3M__, __PLL3N__, __PLL3Q__) \
  ((((__INPUTFREQ__) /(__PLL3M__)) * (__PLL3N__)) / (__PLL3Q__))

/**
  * @brief  Helper macro to calculate the PLL3 frequency
  * @note ex: @ref __LL_RCC_CALC_PLL3CLK_R_FREQ (HSE_VALUE,@ref LL_RCC_PLL3_GetM (),
  *             @ref LL_RCC_PLL3_GetN (), @ref LL_RCC_PLL3_GetR ());
  * @param  __INPUTFREQ__ PLL3 Input frequency (based on HSI/HSE/CSI)
  * @param __PLL3M__ parameter can be a value between 1 and 63
  * @param __PLL3N__ parameter can be a value between 4 and 512
  * @param __PLL3R__ parameter can be a value between 1 and 128
  * @retval PLL3R clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PLL3CLK_R_FREQ(__INPUTFREQ__, __PLL3M__, __PLL3N__, __PLL3R__) \
  ((((__INPUTFREQ__) /(__PLL3M__)) * (__PLL3N__)) / (__PLL3R__))
#endif /* RCC_CR_PLL3ON */
/**
  * @brief  Helper macro to calculate the HCLK frequency
  * @param  __SYSCLKFREQ__ SYSCLK frequency (based on HSI/HSE/CSI/PLLCLK)
  * @param  __AHBPRESCALER__ This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SYSCLK_DIV_1
  *         @arg @ref LL_RCC_SYSCLK_DIV_2
  *         @arg @ref LL_RCC_SYSCLK_DIV_4
  *         @arg @ref LL_RCC_SYSCLK_DIV_8
  *         @arg @ref LL_RCC_SYSCLK_DIV_16
  *         @arg @ref LL_RCC_SYSCLK_DIV_64
  *         @arg @ref LL_RCC_SYSCLK_DIV_128
  *         @arg @ref LL_RCC_SYSCLK_DIV_256
  *         @arg @ref LL_RCC_SYSCLK_DIV_512
  * @retval HCLK clock frequency (in Hz)
  */
#define __LL_RCC_CALC_HCLK_FREQ(__SYSCLKFREQ__, __AHBPRESCALER__) \
  ((__SYSCLKFREQ__) >> AHBPrescTable[((__AHBPRESCALER__) & RCC_CFGR2_HPRE) >>  RCC_CFGR2_HPRE_Pos])

/**
  * @brief  Helper macro to calculate the PCLK1 frequency (APB1)
  * @param  __HCLKFREQ__ HCLK frequency
  * @param  __APB1PRESCALER__ This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB1_DIV_1
  *         @arg @ref LL_RCC_APB1_DIV_2
  *         @arg @ref LL_RCC_APB1_DIV_4
  *         @arg @ref LL_RCC_APB1_DIV_8
  *         @arg @ref LL_RCC_APB1_DIV_16
  * @retval PCLK1 clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PCLK1_FREQ(__HCLKFREQ__, __APB1PRESCALER__) \
  ((__HCLKFREQ__) >> (APBPrescTable[((__APB1PRESCALER__) & RCC_CFGR2_PPRE1) >>  RCC_CFGR2_PPRE1_Pos]))

/**
  * @brief  Helper macro to calculate the PCLK2 frequency (APB2)
  * @param  __HCLKFREQ__ HCLK frequency
  * @param  __APB2PRESCALER__ This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB2_DIV_1
  *         @arg @ref LL_RCC_APB2_DIV_2
  *         @arg @ref LL_RCC_APB2_DIV_4
  *         @arg @ref LL_RCC_APB2_DIV_8
  *         @arg @ref LL_RCC_APB2_DIV_16
  * @retval PCLK2 clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PCLK2_FREQ(__HCLKFREQ__, __APB2PRESCALER__) \
  ((__HCLKFREQ__) >> APBPrescTable[(__APB2PRESCALER__) >>  RCC_CFGR2_PPRE2_Pos])


/**
  * @brief  Helper macro to calculate the PCLK3 frequency (APB3)
  * @param  __HCLKFREQ__ HCLK frequency
  * @param  __APB3PRESCALER__ This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB3_DIV_1
  *         @arg @ref LL_RCC_APB3_DIV_2
  *         @arg @ref LL_RCC_APB3_DIV_4
  *         @arg @ref LL_RCC_APB3_DIV_8
  *         @arg @ref LL_RCC_APB3_DIV_16
  * @retval PCLK3 clock frequency (in Hz)
  */
#define __LL_RCC_CALC_PCLK3_FREQ(__HCLKFREQ__, __APB3PRESCALER__) \
  ((__HCLKFREQ__) >> APBPrescTable[(__APB3PRESCALER__) >>  RCC_CFGR2_PPRE3_Pos])

/**
  * @brief  Helper macro to calculate the HSI frequency
  * @param  __HSIDIV__ This parameter can be one of the following values:
  *         @arg @ref LL_RCC_HSI_DIV_1
  *         @arg @ref LL_RCC_HSI_DIV_2
  *         @arg @ref LL_RCC_HSI_DIV_4
  *         @arg @ref LL_RCC_HSI_DIV_8
  * @retval HSI clock frequency (in Hz)
  */
#define __LL_RCC_CALC_HSI_FREQ(__HSIDIV__) (HSI_VALUE >> ((__HSIDIV__)>> RCC_CR_HSIDIV_Pos))

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup RCC_LL_Exported_Functions RCC Exported Functions
  * @{
  */

/** @defgroup RCC_LL_EF_HSE HSE
  * @{
  */

/**
  * @brief  Enable the HSE Clock Security System.
  * @rmtoll CR           HSECSSON         LL_RCC_HSE_EnableCSS
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_EnableCSS(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSECSSON);
}

/**
  * @brief  Enable HSE external oscillator (HSE Bypass)
  * @rmtoll CR           HSEBYP        LL_RCC_HSE_EnableBypass
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_EnableBypass(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSEBYP);
}

/**
  * @brief  Disable HSE external oscillator (HSE Bypass)
  * @rmtoll CR           HSEBYP        LL_RCC_HSE_DisableBypass
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_DisableBypass(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
}

/**
  * @brief  Enable HSE crystal oscillator (HSE ON)
  * @rmtoll CR           HSEON         LL_RCC_HSE_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSEON);
}

/**
  * @brief  Disable HSE crystal oscillator (HSE ON)
  * @rmtoll CR           HSEON         LL_RCC_HSE_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_HSEON);
}

/**
  * @brief  Check if HSE oscillator Ready
  * @rmtoll CR           HSERDY        LL_RCC_HSE_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_HSE_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSERDY) == RCC_CR_HSERDY) ? 1UL : 0UL);
}

/**
  * @brief  Set external HSE clock type in Bypass mode
  * @note This bit can be written only if the HSE oscillator is disabled
  * @rmtoll CR         HSEEXT        LL_RCC_HSE_SetExternalClockType
  * @param  HSEClockMode This parameter can be one of the following values:
  *         @arg @ref LL_RCC_HSE_ANALOG_TYPE
  *         @arg @ref LL_RCC_HSE_DIGITAL_TYPE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSE_SetExternalClockType(uint32_t HSEClockMode)
{
  MODIFY_REG(RCC->CR, RCC_CR_HSEEXT, HSEClockMode);
}

/**
  * @brief  Get external HSE clock type in Bypass mode
  * @rmtoll CR         HSEEXT        LL_RCC_HSE_GetExternalClockType
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_HSE_ANALOG_TYPE
  *         @arg @ref LL_RCC_HSE_DIGITAL_TYPE
  */
__STATIC_INLINE uint32_t LL_RCC_HSE_GetExternalClockType(void)
{
  return (uint32_t)(READ_BIT(RCC->CR, RCC_CR_HSEEXT));
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_HSI HSI
  * @{
  */

/**
  * @brief  Enable HSI oscillator
  * @rmtoll CR           HSION         LL_RCC_HSI_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSION);
}

/**
  * @brief  Disable HSI oscillator
  * @rmtoll CR           HSION         LL_RCC_HSI_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_HSION);
}

/**
  * @brief  Check if HSI clock is ready
  * @rmtoll CR           HSIRDY        LL_RCC_HSI_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSIRDY) == RCC_CR_HSIRDY) ? 1UL : 0UL);
}

/**
  * @brief  Enable HSI even in stop mode for some peripherals kernel
  * @note HSI oscillator is forced ON even in Stop mode
  * @rmtoll CR           HSIKERON      LL_RCC_HSI_EnableInStopMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI_EnableInStopMode(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSIKERON);
}

/**
  * @brief  Disable HSI in stop mode for some peripherals kernel
  * @rmtoll CR           HSIKERON      LL_RCC_HSI_DisableInStopMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI_DisableInStopMode(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_HSIKERON);
}

/**
  * @brief  Check if HSI is enabled in stop mode
  * @rmtoll CR           HSIKERON        LL_RCC_HSI_IsEnabledInStopMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_IsEnabledInStopMode(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSIKERON) == RCC_CR_HSIKERON) ? 1UL : 0UL);
}

/**
  * @brief  Check if HSI new divider applied and ready
  * @rmtoll CR           HSIDIVF        LL_RCC_HSI_IsDividerReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_IsDividerReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSIDIVF) == (RCC_CR_HSIDIVF)) ? 1UL : 0UL);
}

/**
  * @brief  Set HSI divider
  * @rmtoll CR           HSIDIV        LL_RCC_HSI_SetDivider
  * @param  Divider This parameter can be one of the following values:
  *         @arg @ref LL_RCC_HSI_DIV_1
  *         @arg @ref LL_RCC_HSI_DIV_2
  *         @arg @ref LL_RCC_HSI_DIV_4
  *         @arg @ref LL_RCC_HSI_DIV_8
  * @retval None.
  */
__STATIC_INLINE void LL_RCC_HSI_SetDivider(uint32_t Divider)
{
  MODIFY_REG(RCC->CR, RCC_CR_HSIDIV, Divider);
}

/**
  * @brief  Get HSI divider
  * @rmtoll CR           HSIDIV        LL_RCC_HSI_GetDivider
  * @retval can be one of the following values:
  *         @arg @ref LL_RCC_HSI_DIV_1
  *         @arg @ref LL_RCC_HSI_DIV_2
  *         @arg @ref LL_RCC_HSI_DIV_4
  *         @arg @ref LL_RCC_HSI_DIV_8
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_GetDivider(void)
{
  return (READ_BIT(RCC->CR, RCC_CR_HSIDIV));
}

/**
  * @brief  Get HSI Calibration value
  * @note When HSITRIM is written, HSICAL is updated with the sum of
  *       HSITRIM and the factory trim value
  * @rmtoll HSICFGR       HSICAL        LL_RCC_HSI_GetCalibration
  * @retval A value between 0 and 4095 (0xFFF)
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_GetCalibration(void)
{
  return (uint32_t)(READ_BIT(RCC->HSICFGR, RCC_HSICFGR_HSICAL) >> RCC_HSICFGR_HSICAL_Pos);
}

/**
  * @brief  Set HSI Calibration trimming
  * @note user-programmable trimming value that is added to the HSICAL
  * @note Default value is 64, which, when added to the HSICAL value,
  *       should trim the HSI to 64 MHz +/- 1 %
  * @rmtoll HSICFGR        HSITRIM       LL_RCC_HSI_SetCalibTrimming
  * @param  Value can be a value between Min_Data = 0 and Max_Data = 127 (0x7F)
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI_SetCalibTrimming(uint32_t Value)
{
  MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, Value << RCC_HSICFGR_HSITRIM_Pos);
}

/**
  * @brief  Get HSI Calibration trimming
  * @rmtoll ICSC3R        HSITRIM       LL_RCC_HSI_GetCalibTrimming
  * @retval A value between Min_Data = 0 and Max_Data = 127 (0x7F)
  */
__STATIC_INLINE uint32_t LL_RCC_HSI_GetCalibTrimming(void)
{
  return (uint32_t)(READ_BIT(RCC->HSICFGR, RCC_HSICFGR_HSITRIM) >> RCC_HSICFGR_HSITRIM_Pos);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_CSI CSI
  * @{
  */

/**
  * @brief  Enable CSI oscillator
  * @rmtoll CR           CSION         LL_RCC_CSI_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_CSI_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_CSION);
}

/**
  * @brief  Disable CSI oscillator
  * @rmtoll CR           CSION         LL_RCC_CSI_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_CSI_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_CSION);
}

/**
  * @brief  Check if CSI clock is ready
  * @rmtoll CR           CSIRDY        LL_RCC_CSI_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_CSI_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_CSIRDY) == (RCC_CR_CSIRDY)) ? 1UL : 0UL);
}

/**
  * @brief  Enable CSI oscillator in Stop mode for some peripherals kernel clock
  * @rmtoll CR           CSIKERON         LL_RCC_CSI_EnableInStopMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_CSI_EnableInStopMode(void)
{
  SET_BIT(RCC->CR, RCC_CR_CSIKERON);
}

/**
  * @brief  Disable CSI oscillator in Stop mode for some peripherals kernel clock
  * @rmtoll CR           CSIKERON         LL_RCC_CSI_DisableInStopMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_CSI_DisableInStopMode(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_CSIKERON);
}

/**
  * @brief  Check if CSI is enabled in stop mode
  * @rmtoll CR           CSIKERON        LL_RCC_CSI_IsEnabledInStopMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_CSI_IsEnabledInStopMode(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_CSIKERON) == RCC_CR_CSIKERON) ? 1UL : 0UL);
}

/**
  * @brief  Get CSI Calibration value
  * @note When CSITRIM is written, CSICAL is updated with the sum of
  *       CSITRIM and the factory trim value
  * @rmtoll CSICFGR        CSICAL        LL_RCC_CSI_GetCalibration
  * @retval A value between 0 and 255 (0xFF)
  */
__STATIC_INLINE uint32_t LL_RCC_CSI_GetCalibration(void)
{
  return (uint32_t)(READ_BIT(RCC->CSICFGR, RCC_CSICFGR_CSICAL) >> RCC_CSICFGR_CSICAL_Pos);
}

/**
  * @brief  Set CSI Calibration trimming
  * @note user-programmable trimming value that is added to the CSICAL
  * @note Default value is 16, which, when added to the CSICAL value,
  *       should trim the CSI to 4 MHz +/- 1 %
  * @rmtoll CSICFGR        CSITRIM       LL_RCC_CSI_SetCalibTrimming
  * @param  Value can be a value between Min_Data = 0 and Max_Data = 63 (0x3F)
  * @retval None
  */
__STATIC_INLINE void LL_RCC_CSI_SetCalibTrimming(uint32_t Value)
{
  MODIFY_REG(RCC->CSICFGR, RCC_CSICFGR_CSITRIM, Value << RCC_CSICFGR_CSITRIM_Pos);
}

/**
  * @brief  Get CSI Calibration trimming
  * @rmtoll CSICFGR        CSITRIM       LL_RCC_CSI_GetCalibTrimming
  * @retval A value between 0 and 63 (0x3F)
  */
__STATIC_INLINE uint32_t LL_RCC_CSI_GetCalibTrimming(void)
{
  return (uint32_t)(READ_BIT(RCC->CSICFGR, RCC_CSICFGR_CSITRIM) >> RCC_CSICFGR_CSITRIM_Pos);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_HSI48 HSI48
  * @{
  */

/**
  * @brief  Enable HSI48
  * @rmtoll CR          HSI48ON       LL_RCC_HSI48_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI48_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSI48ON);
}

/**
  * @brief  Disable HSI48
  * @rmtoll CR          HSI48ON       LL_RCC_HSI48_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_HSI48_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_HSI48ON);
}

/**
  * @brief  Check if HSI48 oscillator Ready
  * @rmtoll CR          HSI48RDY      LL_RCC_HSI48_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_HSI48_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSI48RDY) == RCC_CR_HSI48RDY) ? 1UL : 0UL);
}

/**
  * @brief  Get HSI48 Calibration value
  * @rmtoll CRRCR          HSI48CAL      LL_RCC_HSI48_GetCalibration
  * @retval A value between 0 and 1023 (0x3FF)
  */
__STATIC_INLINE uint32_t LL_RCC_HSI48_GetCalibration(void)
{
  return (uint32_t)(READ_BIT(RCC->CRRCR, RCC_CRRCR_HSI48CAL) >> RCC_CRRCR_HSI48CAL_Pos);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_LSE LSE
  * @{
  */

/**
  * @brief  Enable  Low Speed External (LSE) crystal.
  * @rmtoll BDCR         LSEON         LL_RCC_LSE_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_Enable(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSEON);
}

/**
  * @brief  Disable  Low Speed External (LSE) crystal.
  * @rmtoll BDCR         LSEON         LL_RCC_LSE_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_Disable(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEON);
}

/**
  * @brief  Check if LSE oscillator Ready
  * @rmtoll BDCR         LSERDY        LL_RCC_LSE_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_LSE_IsReady(void)
{
  return ((READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == RCC_BDCR_LSERDY) ? 1UL : 0UL);
}

/**
  * @brief  Enable external clock source (LSE bypass).
  * @rmtoll BDCR         LSEBYP        LL_RCC_LSE_EnableBypass
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_EnableBypass(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
}

/**
  * @brief  Disable external clock source (LSE bypass).
  * @rmtoll BDCR         LSEBYP        LL_RCC_LSE_DisableBypass
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_DisableBypass(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEBYP);
}

/**
  * @brief  Set external LSE clock type in Bypass mode
  * @note This bit can be written only if the LSE oscillator is disabled
  * @rmtoll BDCR         LSEEXT        LL_RCC_LSE_SetExternalClockType
  * @param  LSEClockMode This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LSE_ANALOG_TYPE
  *         @arg @ref LL_RCC_LSE_DIGITAL_TYPE (*)
  * @retval None
  *
  * (*) not to be used if RTC is active
  */
__STATIC_INLINE void LL_RCC_LSE_SetExternalClockType(uint32_t LSEClockMode)
{
  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEEXT, LSEClockMode);
}

/**
  * @brief  Get external LSE clock type in Bypass mode
  * @rmtoll BDCR         LSEEXT        LL_RCC_LSE_GetExternalClockType
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LSE_ANALOG_TYPE
  *         @arg @ref LL_RCC_LSE_DIGITAL_TYPE
  */
__STATIC_INLINE uint32_t LL_RCC_LSE_GetExternalClockType(void)
{
  return (uint32_t)(READ_BIT(RCC->BDCR, RCC_BDCR_LSEEXT));
}

/**
  * @brief  Set LSE oscillator drive capability
  * @note The oscillator is in Xtal mode when it is not in bypass mode.
  * @rmtoll BDCR         LSEDRV        LL_RCC_LSE_SetDriveCapability
  * @param  LSEDrive This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LSEDRIVE_LOW
  *         @arg @ref LL_RCC_LSEDRIVE_MEDIUMLOW
  *         @arg @ref LL_RCC_LSEDRIVE_MEDIUMHIGH
  *         @arg @ref LL_RCC_LSEDRIVE_HIGH
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_SetDriveCapability(uint32_t LSEDrive)
{
  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSEDRV, LSEDrive);
}

/**
  * @brief  Get LSE oscillator drive capability
  * @rmtoll BDCR         LSEDRV        LL_RCC_LSE_GetDriveCapability
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LSEDRIVE_LOW
  *         @arg @ref LL_RCC_LSEDRIVE_MEDIUMLOW
  *         @arg @ref LL_RCC_LSEDRIVE_MEDIUMHIGH
  *         @arg @ref LL_RCC_LSEDRIVE_HIGH
  */
__STATIC_INLINE uint32_t LL_RCC_LSE_GetDriveCapability(void)
{
  return (uint32_t)(READ_BIT(RCC->BDCR, RCC_BDCR_LSEDRV));
}

/**
  * @brief  Enable Clock security system on LSE.
  * @rmtoll BDCR         LSECSSON      LL_RCC_LSE_EnableCSS
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_EnableCSS(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
}

/**
  * @brief  Disable Clock security system on LSE.
  * @note Clock security system can be disabled only after a LSE
  *       failure detection. In that case it MUST be disabled by software.
  * @rmtoll BDCR         LSECSSON      LL_RCC_LSE_DisableCSS
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSE_DisableCSS(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
}

/**
  * @brief  Check if CSS on LSE failure Detection
  * @rmtoll BDCR         LSECSSD       LL_RCC_LSE_IsCSSDetected
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_LSE_IsCSSDetected(void)
{
  return ((READ_BIT(RCC->BDCR, RCC_BDCR_LSECSSD) == RCC_BDCR_LSECSSD) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_LSI LSI
  * @{
  */

/**
  * @brief  Enable LSI Oscillator
  * @rmtoll BDCR          LSION         LL_RCC_LSI_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSI_Enable(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSION);
}

/**
  * @brief  Disable LSI Oscillator
  * @rmtoll BDCR          LSION         LL_RCC_LSI_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSI_Disable(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSION);
}

/**
  * @brief  Check if LSI is Ready
  * @rmtoll BDCR          LSIRDY        LL_RCC_LSI_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_LSI_IsReady(void)
{
  return ((READ_BIT(RCC->BDCR, RCC_BDCR_LSIRDY) == RCC_BDCR_LSIRDY) ? 1UL : 0UL);
}


/**
  * @}
  */


/** @defgroup RCC_LL_EF_LSCO LSCO
  * @{
  */

/**
  * @brief  Enable Low Speed Microcontroller Clock Output
  * @rmtoll BDCR         LSCOEN        LL_RCC_LSCO_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSCO_Enable(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSCOEN);
}

/**
  * @brief  Disable Low Speed Microcontroller Clock Output
  * @rmtoll BDCR         LSCOEN        LL_RCC_LSCO_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSCO_Disable(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSCOEN);
}

/**
  * @brief  Configure Low Speed Microcontroller Clock Output selection
  * @rmtoll BDCR         LSCOSEL       LL_RCC_LSCO_SetSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LSCO_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LSCO_CLKSOURCE_LSE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_LSCO_SetSource(uint32_t Source)
{
  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSCOSEL, Source);
}

/**
  * @brief  Get Low Speed Microcontroller Clock Output selection
  * @rmtoll BDCR         LSCOSEL       LL_RCC_LSCO_GetSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LSCO_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LSCO_CLKSOURCE_LSE
  */
__STATIC_INLINE uint32_t LL_RCC_LSCO_GetSource(void)
{
  return (uint32_t)(READ_BIT(RCC->BDCR, RCC_BDCR_LSCOSEL));
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_System System
  * @{
  */

/**
  * @brief  Configure the system clock source
  * @rmtoll CFGR1         SW           LL_RCC_SetSysClkSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_PLL1
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetSysClkSource(uint32_t Source)
{
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_SW, Source);
}

/**
  * @brief  Get the system clock source
  * @rmtoll CFGR1        SWS           LL_RCC_GetSysClkSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_HSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_CSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_HSE
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_PLL1
  */
__STATIC_INLINE uint32_t LL_RCC_GetSysClkSource(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_SWS));
}

/**
  * @brief  Set AHB prescaler
  * @rmtoll CFGR2        HPRE          LL_RCC_SetAHBPrescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SYSCLK_DIV_1
  *         @arg @ref LL_RCC_SYSCLK_DIV_2
  *         @arg @ref LL_RCC_SYSCLK_DIV_4
  *         @arg @ref LL_RCC_SYSCLK_DIV_8
  *         @arg @ref LL_RCC_SYSCLK_DIV_16
  *         @arg @ref LL_RCC_SYSCLK_DIV_64
  *         @arg @ref LL_RCC_SYSCLK_DIV_128
  *         @arg @ref LL_RCC_SYSCLK_DIV_256
  *         @arg @ref LL_RCC_SYSCLK_DIV_512
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetAHBPrescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_HPRE, Prescaler);
}

/**
  * @brief  Set Systick clock source
  * @rmtoll CCIPR4      SYSTICKSEL    LL_RCC_SetSystickClockSource
  * @param  SystickSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetSystickClockSource(uint32_t SystickSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_SYSTICKSEL, SystickSource);
}

/**
  * @brief  Set APB1 prescaler
  * @rmtoll CFGR2         PPRE1         LL_RCC_SetAPB1Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB1_DIV_1
  *         @arg @ref LL_RCC_APB1_DIV_2
  *         @arg @ref LL_RCC_APB1_DIV_4
  *         @arg @ref LL_RCC_APB1_DIV_8
  *         @arg @ref LL_RCC_APB1_DIV_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetAPB1Prescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE1, Prescaler);
}

/**
  * @brief  Set APB2 prescaler
  * @rmtoll CFGR2         PPRE2         LL_RCC_SetAPB2Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB2_DIV_1
  *         @arg @ref LL_RCC_APB2_DIV_2
  *         @arg @ref LL_RCC_APB2_DIV_4
  *         @arg @ref LL_RCC_APB2_DIV_8
  *         @arg @ref LL_RCC_APB2_DIV_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetAPB2Prescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE2, Prescaler);
}

/**
  * @brief  Set APB3 prescaler
  * @rmtoll CFGR3         PPRE3         LL_RCC_SetAPB3Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_APB3_DIV_1
  *         @arg @ref LL_RCC_APB3_DIV_2
  *         @arg @ref LL_RCC_APB3_DIV_4
  *         @arg @ref LL_RCC_APB3_DIV_8
  *         @arg @ref LL_RCC_APB3_DIV_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetAPB3Prescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE3, Prescaler);
}

/**
  * @brief  Get AHB prescaler
  * @rmtoll CFGR2         HPRE          LL_RCC_GetAHBPrescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SYSCLK_DIV_1
  *         @arg @ref LL_RCC_SYSCLK_DIV_2
  *         @arg @ref LL_RCC_SYSCLK_DIV_4
  *         @arg @ref LL_RCC_SYSCLK_DIV_8
  *         @arg @ref LL_RCC_SYSCLK_DIV_16
  *         @arg @ref LL_RCC_SYSCLK_DIV_64
  *         @arg @ref LL_RCC_SYSCLK_DIV_128
  *         @arg @ref LL_RCC_SYSCLK_DIV_256
  *         @arg @ref LL_RCC_SYSCLK_DIV_512
  */
__STATIC_INLINE uint32_t LL_RCC_GetAHBPrescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR2, RCC_CFGR2_HPRE));
}

/**
  * @brief  Get Sysctick clock source
  * @rmtoll CCIPR4       SYSTICKSEL    LL_RCC_SetSystickClockSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8
  */
__STATIC_INLINE uint32_t LL_RCC_GetSystickClockSource(void)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_SYSTICKSEL));
}

/**
  * @brief  Get APB1 prescaler
  * @rmtoll CFGR2         PPRE1         LL_RCC_GetAPB1Prescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_APB1_DIV_1
  *         @arg @ref LL_RCC_APB1_DIV_2
  *         @arg @ref LL_RCC_APB1_DIV_4
  *         @arg @ref LL_RCC_APB1_DIV_8
  *         @arg @ref LL_RCC_APB1_DIV_16
  */
__STATIC_INLINE uint32_t LL_RCC_GetAPB1Prescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR2, RCC_CFGR2_PPRE1));
}

/**
  * @brief  Get APB2 prescaler
  * @rmtoll CFGR2         PPRE2         LL_RCC_GetAPB2Prescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_APB2_DIV_1
  *         @arg @ref LL_RCC_APB2_DIV_2
  *         @arg @ref LL_RCC_APB2_DIV_4
  *         @arg @ref LL_RCC_APB2_DIV_8
  *         @arg @ref LL_RCC_APB2_DIV_16
  */
__STATIC_INLINE uint32_t LL_RCC_GetAPB2Prescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR2, RCC_CFGR2_PPRE2));
}

/**
  * @brief  Get APB3 prescaler
  * @rmtoll CFGR3         PPRE3         LL_RCC_GetAPB2Prescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_APB3_DIV_1
  *         @arg @ref LL_RCC_APB3_DIV_2
  *         @arg @ref LL_RCC_APB3_DIV_4
  *         @arg @ref LL_RCC_APB3_DIV_8
  *         @arg @ref LL_RCC_APB3_DIV_16
  */
__STATIC_INLINE uint32_t LL_RCC_GetAPB3Prescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR2, RCC_CFGR2_PPRE3));
}

/**
  * @brief  Set System Clock After Wake-Up From Stop mode
  * @rmtoll CFGR1       STOPWUCK      LL_RCC_SetClkAfterWakeFromStop
  * @param  Clock This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SYSWAKEUP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_SYSWAKEUP_CLKSOURCE_CSI
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetClkAfterWakeFromStop(uint32_t Clock)
{
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_STOPWUCK, Clock);
}

/**
  * @brief  Get System Clock After Wake-Up From Stop mode
  * @rmtoll CFGR1         STOPWUCK      LL_RCC_GetClkAfterWakeFromStop
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SYSWAKEUP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_SYSWAKEUP_CLKSOURCE_CSI
  */
__STATIC_INLINE uint32_t LL_RCC_GetClkAfterWakeFromStop(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_STOPWUCK));
}
/**
  * @}
  */

/** @defgroup RCC_LL_EF_MCO MCO
  * @{
  */

/**
  * @brief  Configure MCO1 (pin PA8) or MCO2 (pin PC9)
  * @rmtoll CFGR1         MCO1          LL_RCC_ConfigMCO\n
  *         CFGR1         MCO1PRE       LL_RCC_ConfigMCO\n
  *         CFGR1         MCO2          LL_RCC_ConfigMCO\n
  *         CFGR1         MCO2PRE       LL_RCC_ConfigMCO
  * @param  MCOxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_MCO1SOURCE_HSI
  *         @arg @ref LL_RCC_MCO1SOURCE_LSE
  *         @arg @ref LL_RCC_MCO1SOURCE_HSE
  *         @arg @ref LL_RCC_MCO1SOURCE_PLL1Q
  *         @arg @ref LL_RCC_MCO1SOURCE_HSI48
  *         @arg @ref LL_RCC_MCO2SOURCE_SYSCLK
  *         @arg @ref LL_RCC_MCO2SOURCE_PLL2P
  *         @arg @ref LL_RCC_MCO2SOURCE_HSE
  *         @arg @ref LL_RCC_MCO2SOURCE_PLL1P
  *         @arg @ref LL_RCC_MCO2SOURCE_CSI
  *         @arg @ref LL_RCC_MCO2SOURCE_LSI
  * @param  MCOxPrescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_MCO1_DIV_1
  *         @arg @ref LL_RCC_MCO1_DIV_2
  *         @arg @ref LL_RCC_MCO1_DIV_3
  *         @arg @ref LL_RCC_MCO1_DIV_4
  *         @arg @ref LL_RCC_MCO1_DIV_5
  *         @arg @ref LL_RCC_MCO1_DIV_6
  *         @arg @ref LL_RCC_MCO1_DIV_7
  *         @arg @ref LL_RCC_MCO1_DIV_8
  *         @arg @ref LL_RCC_MCO1_DIV_9
  *         @arg @ref LL_RCC_MCO1_DIV_10
  *         @arg @ref LL_RCC_MCO1_DIV_11
  *         @arg @ref LL_RCC_MCO1_DIV_12
  *         @arg @ref LL_RCC_MCO1_DIV_13
  *         @arg @ref LL_RCC_MCO1_DIV_14
  *         @arg @ref LL_RCC_MCO1_DIV_15
  *         @arg @ref LL_RCC_MCO2_DIV_1
  *         @arg @ref LL_RCC_MCO2_DIV_2
  *         @arg @ref LL_RCC_MCO2_DIV_3
  *         @arg @ref LL_RCC_MCO2_DIV_4
  *         @arg @ref LL_RCC_MCO2_DIV_5
  *         @arg @ref LL_RCC_MCO2_DIV_6
  *         @arg @ref LL_RCC_MCO2_DIV_7
  *         @arg @ref LL_RCC_MCO2_DIV_8
  *         @arg @ref LL_RCC_MCO2_DIV_9
  *         @arg @ref LL_RCC_MCO2_DIV_10
  *         @arg @ref LL_RCC_MCO2_DIV_11
  *         @arg @ref LL_RCC_MCO2_DIV_12
  *         @arg @ref LL_RCC_MCO2_DIV_13
  *         @arg @ref LL_RCC_MCO2_DIV_14
  *         @arg @ref LL_RCC_MCO2_DIV_15
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ConfigMCO(uint32_t MCOxSource, uint32_t MCOxPrescaler)
{
  MODIFY_REG(RCC->CFGR1, (MCOxSource << 16U) | (MCOxPrescaler << 16U), \
             (MCOxSource & 0xFFFF0000U) | (MCOxPrescaler & 0xFFFF0000U));
}

/**
  * @}
  */


/** @defgroup RCC_LL_EF_Peripheral_Clock_Source Peripheral Clock Source
  * @{
  */

/**
  * @brief  Configure periph clock source
  * @rmtoll CCIPR1       *     LL_RCC_SetClockSource\n
  *         CCIPR2       *     LL_RCC_SetClockSource\n
  *         CCIPR3       *     LL_RCC_SetClockSource\n
  *         CCIPR4       *     LL_RCC_SetClockSource\n
  *         CCIPR5       *     LL_RCC_SetClockSource
  * @param  ClkSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PCLK2
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_NONE (**)
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PIN (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PIN (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL2R (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL2R (*)
  *
  *         (*) value not defined in all devices.
  *         (**) : For stm32h503xx family line only.
  *
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetClockSource(uint32_t ClkSource)
{
  uint32_t *pReg = (uint32_t *)((uint32_t)&RCC->CCIPR1 + LL_CLKSOURCE_REG(ClkSource));
  MODIFY_REG(*pReg, LL_CLKSOURCE_MASK(ClkSource), LL_CLKSOURCE_CONFIG(ClkSource));
}


/**
  * @brief  Configure USARTx kernel clock source
  * @rmtoll CCIPR1       USART1SEL     LL_RCC_SetUSARTClockSource\n
  *         CCIPR1       USART2SEL     LL_RCC_SetUSARTClockSource\n
  *         CCIPR1       USART3SEL     LL_RCC_SetUSARTClockSource\n
  *         CCIPR1       USART6SEL     LL_RCC_SetUSARTClockSource\n
  *         CCIPR1       USART10SEL    LL_RCC_SetUSARTClockSource\n
  *         CCIPR2       USART11SEL    LL_RCC_SetUSARTClockSource
  * @param  USARTxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PCLK2
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_LSE (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetUSARTClockSource(uint32_t USARTxSource)
{
  LL_RCC_SetClockSource(USARTxSource);
}

#if defined(UART4)
/**
  * @brief  Configure UARTx kernel clock source
  * @rmtoll CCIPR1       UART4SEL      LL_RCC_SetUARTClockSource\n
  *         CCIPR1       UART5SEL      LL_RCC_SetUARTClockSource\n
  *         CCIPR1       UART7SEL      LL_RCC_SetUARTClockSource\n
  *         CCIPR1       UART8SEL      LL_RCC_SetUARTClockSource\n
  *         CCIPR1       UART9SEL      LL_RCC_SetUARTClockSource\n
  *         CCIPR2       UART12SEL     LL_RCC_SetUARTClockSource
  * @param  UARTxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_LSE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetUARTClockSource(uint32_t UARTxSource)
{
  LL_RCC_SetClockSource(UARTxSource);
}
#endif /* UART4 */

/**
  * @brief  Configure LPUARTx kernel clock source
  * @rmtoll CCIPR3       LPUART1SEL    LL_RCC_SetLPUARTClockSource
  * @param  LPUARTxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_LSE
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetLPUARTClockSource(uint32_t LPUARTxSource)
{
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_LPUART1SEL, LPUARTxSource);
}

/**
  * @brief  Configure I2Cx kernel clock source
  * @rmtoll CCIPR4       I2C1SEL       LL_RCC_SetI2CClockSource\n
  *         CCIPR4       I2C2SEL       LL_RCC_SetI2CClockSource\n
  *         CCIPR4       I2C3SEL       LL_RCC_SetI2CClockSource\n
  *         CCIPR4       I2C4SEL       LL_RCC_SetI2CClockSource
  * @param  I2CxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_CSI (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  *  (**) : For stm32h503xx family line only.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetI2CClockSource(uint32_t I2CxSource)
{
  LL_RCC_SetClockSource(I2CxSource);
}

/**
  * @brief  Configure I3Cx kernel clock source
  * @rmtoll CCIPR4       I3C1SEL    LL_RCC_SetI3CClockSource\n
  *         CCIPR4       I3C2SEL    LL_RCC_SetI3CClockSource
  * @param  I3CxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PCLK1 (***)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PCLK3 (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_HSI   (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_CSI   (***)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_NONE  (**)
  * @retval None
  *
  *  (*)  : For stm32h56xxx, stm32h57xxx, stm32h5exxx and stm32h5fxxx family lines.
  *  (**) : For stm32h503xx family line.
  *  (***)  : For stm32h5exxx and stm32h5fxxx family lines.
  */
__STATIC_INLINE void LL_RCC_SetI3CClockSource(uint32_t I3CxSource)
{
  LL_RCC_SetClockSource(I3CxSource);
}

/**
  * @brief  Configure SPIx kernel clock source
  * @rmtoll    CCIPR3    SPI1SEL       LL_RCC_SetSPIClockSource\n
  *            CCIPR3    SPI2SEL       LL_RCC_SetSPIClockSource\n
  *            CCIPR3    SPI3SEL       LL_RCC_SetSPIClockSource\n
  *            CCIPR3    SPI4SEL       LL_RCC_SetSPIClockSource\n
  *            CCIPR3    SPI5SEL       LL_RCC_SetSPIClockSource\n
  *            CCIPR3    SPI6SEL       LL_RCC_SetSPIClockSource
  * @param  SPIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL3P  (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSE (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetSPIClockSource(uint32_t SPIxSource)
{
  LL_RCC_SetClockSource(SPIxSource);
}

/**
  * @brief  Configure LPTIMx kernel clock source
  * @rmtoll CCIPR2       LPTIM1SEL     LL_RCC_SetLPTIMClockSource\n
  *         CCIPR2       LPTIM2SEL     LL_RCC_SetLPTIMClockSource\n
  *         CCIPR2       LPTIM3SEL     LL_RCC_SetLPTIMClockSource\n
  *         CCIPR2       LPTIM4SEL     LL_RCC_SetLPTIMClockSource\n
  *         CCIPR2       LPTIM5SEL     LL_RCC_SetLPTIMClockSource\n
  *         CCIPR2       LPTIM6SEL     LL_RCC_SetLPTIMClockSource
  * @param  LPTIMxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL3R  (*)
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_CLKP (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetLPTIMClockSource(uint32_t LPTIMxSource)
{
  LL_RCC_SetClockSource(LPTIMxSource);
}

/**
  * @brief  Configure FDCAN kernel clock source
  * @rmtoll CCIPR5       FDCANSEL      LL_RCC_SetFDCANClockSource
  * @param  FDCANxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_PLL2Q
  * @retval None
  *
  */
__STATIC_INLINE void LL_RCC_SetFDCANClockSource(uint32_t FDCANxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_FDCANSEL, FDCANxSource);
}

#if defined(SAI1)
/**
  * @brief  Configure SAIx kernel clock source
  * @rmtoll CCIPR2       SAI1SEL       LL_RCC_SetSAIClockSource\n
  *         CCIPR2       SAI2SEL       LL_RCC_SetSAIClockSource
  * @param  SAIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetSAIClockSource(uint32_t SAIxSource)
{
  LL_RCC_SetClockSource(SAIxSource);
}
#endif /* SAI1 */

#if defined(SDMMC1)
/**
  * @brief  Configure SDMMCx kernel clock source
  * @rmtoll CCIPR4       SDMMC1SEL      LL_RCC_SetSDMMCClockSource
  * @rmtoll CCIPR4       SDMMC2SEL      LL_RCC_SetSDMMCClockSource
  * @param  SDMMCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL2R
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetSDMMCClockSource(uint32_t SDMMCxSource)
{
  LL_RCC_SetClockSource(SDMMCxSource);
}
#endif /* SDMMC1 */

/**
  * @brief  Configure RNG kernel clock source
  * @rmtoll CCIPR5       RNGSEL      LL_RCC_SetRNGClockSource
  * @param  RNGxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_HSI48
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_LSI
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetRNGClockSource(uint32_t RNGxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_RNGSEL, RNGxSource);
}

#if defined(USB_DRD_FS)
/**
  * @brief  Configure USB clock source
  * @rmtoll CCIPR4       USBSEL      LL_RCC_SetUSBClockSource
  * @param  USBxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USB_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USB_CLKSOURCE_HSI48
  *
  * (*) : Available in some STM32H5 lines only.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetUSBClockSource(uint32_t USBxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_USBSEL, USBxSource);
}
#endif /* USB_DRD_FS */

/**
  * @brief  Configure ADCx kernel clock source
  * @rmtoll CCIPR5       ADCDACSEL        LL_RCC_SetADCDACClockSource
  * @param  ADCDACxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_SYSCLK
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_CSI
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetADCDACClockSource(uint32_t ADCDACxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ADCDACSEL, ADCDACxSource);
}

/**
  * @brief  Configure DAC low-power kernel clock source
  * @rmtoll CCIPR5        DACSEL        LL_RCC_SetDACLPClockSource
  * @param  DACLPxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DAC_LP_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_DAC_LP_CLKSOURCE_LSI
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetDACLPClockSource(uint32_t DACLPxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_DACSEL, DACLPxSource);
}

#if defined(CEC)
/**
  * @brief  Configure CECx kernel clock source
  * @rmtoll CCIPR5       CECSEL        LL_RCC_SetCECClockSource
  * @param  CECxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_CSI_DIV122
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetCECClockSource(uint32_t CECxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_CECSEL, CECxSource);
}
#endif /* CEC */

#if defined(OCTOSPI1)
/**
  * @brief  Configure OCTOSPIx kernel clock source
  * @rmtoll CCIPR4         OCTOSPISEL        LL_RCC_SetOCTOSPIClockSource
  * @param  OCTOSPIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetOCTOSPIClockSource(uint32_t OCTOSPIxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OCTOSPISEL, OCTOSPIxSource);
}
#endif /* OCTOSPI1 */

#if defined(PLAY1)
/**
  * @brief  Configure PLAY1 kernel clock source
  * @rmtoll CCIPR3         PLAY1SEL        LL_RCC_SetPLAY1ClockSource
  * @param  PLAYxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetPLAY1ClockSource(uint32_t PLAYxSource)
{
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_PLAY1SEL, PLAYxSource);
}
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/**
  * @brief  Configure OTGFSx kernel clock source
  * @rmtoll CCIPR4         OTGFSSEL        LL_RCC_SetOTGFSClockSource
  * @param  OTGFSxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_HSI48
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_CLK48 (*)
  * @retval None
  *
  *  (*) : Available only on STM32H5E5x/STM32H5F5x when OTG_HS PHY is enabled.
  */
__STATIC_INLINE void LL_RCC_SetOTGFSClockSource(uint32_t OTGFSxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGFSSEL, OTGFSxSource);
}
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/**
  * @brief  Configure OTGHSx kernel clock source
  * @rmtoll CCIPR4         OTGHSSEL        LL_RCC_SetOTGHSClockSource
  * @param  OTGHSxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_HSE_DIV2
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_PLL1Q_DIV2
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetOTGHSClockSource(uint32_t OTGHSxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGHSSEL, OTGHSxSource);
}
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/**
  * @brief  Configure OTGPHYx kernel clock source
  * @rmtoll CCIPR4         OTGPHYREFCKSEL      LL_RCC_SetOTGPHYClockSource
  * @param  OTGPHYxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_16M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_19_2M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_20M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_24M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_26M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_32M
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetOTGPHYClockSource(uint32_t OTGPHYxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGPHYREFCKSEL, OTGPHYxSource);
}
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(OCTOSPI2)
/**
  * @brief  Configure OCTOSPI2 kernel clock source
  * @rmtoll CCIPR5         OCTOSPI2SEL        LL_RCC_SetOCTOSPI2ClockSource
  * @param  OCTOSPIxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetOCTOSPI2ClockSource(uint32_t OCTOSPIxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_OCTOSPI2SEL, OCTOSPIxSource);
}
#endif /* OCTOSPI2 */

#if defined(LTDC)
/**
  * @brief  Configure LTDCx kernel clock source
  * @rmtoll CCIPR5         LTDCSEL        LL_RCC_SetLTDCClockSource
  * @param  LTDCxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE_PLL2R
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetLTDCClockSource(uint32_t LTDCxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_LTDCSEL, LTDCxSource);
}
#endif /* LTDC */

#if defined(ADF1)
/**
  * @brief  Configure ADF1 kernel clock source
  * @rmtoll CCIPR5         ADF1SEL        LL_RCC_SetADF1ClockSource
  * @param  ADFxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetADF1ClockSource(uint32_t ADFxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ADF1SEL, ADFxSource);
}
#endif /* ADF1 */

#if defined(MDF1)
/**
  * @brief  Configure MDF1 kernel clock source
  * @rmtoll CCIPR5         MDF1SEL        LL_RCC_SetMDF1ClockSource
  * @param  MDFxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_CLKP
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetMDF1ClockSource(uint32_t MDFxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_MDF1SEL, MDFxSource);
}
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/**
  * @brief  Configure ETH kernel clock source
  * @rmtoll CCIPR4         ETHSEL        LL_RCC_SetETHClockSource
  * @param  ETHxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETH_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_ETH_CLKSOURCE_PLL1Q
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetETHClockSource(uint32_t ETHxSource)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_ETHCLKSEL, ETHxSource);
}
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/**
  * @brief  Configure ETHPTP kernel clock source
  * @rmtoll CCIPR5         ETHPTPSEL        LL_RCC_SetETHPTPClockSource
  * @param  ETHPTPxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL1R
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL3P
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetETHPTPClockSource(uint32_t ETHPTPxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ETHPTPCLKSEL, ETHPTPxSource);
}
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/**
  * @brief  Configure ETHREF kernel clock source
  * @rmtoll CCIPR5         ETHREFSEL        LL_RCC_SetETHREFClockSource
  * @param  ETHREFxSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHREF_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ETHREF_CLKSOURCE_PLL1R
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetETHREFClockSource(uint32_t ETHREFxSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ETHREFCLKSEL, ETHREFxSource);
}
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

/**
  * @brief  Configure CLKP Kernel clock source
  * @rmtoll CCIPR5         CKPERSEL        LL_RCC_SetCLKPClockSource
  * @param  ClkSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_NONE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetCLKPClockSource(uint32_t ClkSource)
{
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_CKERPSEL, ClkSource);
}

#if defined(PLAY1)
/**
  * @brief  Configure PLAY1 kernel clock divider
  * @rmtoll CCIPR3         PLAY1DIV        LL_RCC_SetPLAY1ClockDivider
  * @param  PLAYxDivider This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_1
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_2
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_4
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_8
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_16
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_32
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_64
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_128
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_256
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_512
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_1024
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_2048
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_4096
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_8192
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_16384
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_32768
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetPLAY1ClockDivider(uint32_t PLAYxDivider)
{
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_PLAY1DIV, PLAYxDivider);
}
#endif /* PLAY1 */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/**
  * @brief  Configure ETHPTP kernel clock divider
  * @rmtoll CCIPR4         ETHPTPDIV        LL_RCC_SetETHPTPClockDivider
  * @param  ETHPTPxDivider This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_1
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_2
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_3
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_4
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_5
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_6
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_7
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_8
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_9
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_10
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_11
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_12
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_13
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_14
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_15
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetETHPTPClockDivider(uint32_t ETHPTPxDivider)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_ETHPTPDIV, ETHPTPxDivider);
}
#endif /* RCC_CCIPR4_ETHPTPDIV */

/**
  * @brief  Get periph clock source
  * @rmtoll CCIPR1      *     LL_RCC_GetClockSource\n
  *         CCIPR2      *     LL_RCC_GetClockSource\n
  *         CCIPR3      *     LL_RCC_GetClockSource\n
  *         CCIPR4      *     LL_RCC_GetClockSource\n
  *         CCIPR5      *     LL_RCC_GetClockSource
  * @param  Periph This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE (*)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PCLK2
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_NONE (**)
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PIN (*)
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PIN (*)
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL2R (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL2R (*)
  *
  *         (*) value not defined in all devices.
  *         (**) : For stm32h503xx family line only.
  * @retval None
  */
__STATIC_INLINE uint32_t LL_RCC_GetClockSource(uint32_t Periph)
{
  const uint32_t *pReg = (uint32_t *)((uint32_t)((uint32_t)(&RCC->CCIPR1) + LL_CLKSOURCE_REG(Periph)));
  return (uint32_t)(Periph | (((READ_BIT(*pReg, LL_CLKSOURCE_MASK(Periph))) >> \
                               LL_CLKSOURCE_SHIFT(Periph)) << LL_RCC_CONFIG_SHIFT));
}

/**
  * @brief  Get USARTx kernel clock source
  * @rmtoll CCIPR1       USART1SEL     LL_RCC_GetUSARTClockSource\n
  *         CCIPR1       USART2SEL     LL_RCC_GetUSARTClockSource\n
  *         CCIPR1       USART3SEL     LL_RCC_GetUSARTClockSource\n
  *         CCIPR1       USART6SEL     LL_RCC_GetUSARTClockSource\n
  *         CCIPR1       USART10SEL    LL_RCC_GetUSARTClockSource\n
  *         CCIPR2       USART11SEL    LL_RCC_GetUSARTClockSource
  * @param  USARTx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PCLK2
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_USART3_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART10_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PCLK1 (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_USART11_CLKSOURCE_LSE (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  */
__STATIC_INLINE uint32_t LL_RCC_GetUSARTClockSource(uint32_t USARTx)
{
  return LL_RCC_GetClockSource(USARTx);
}

#if defined(UART4)
/**
  * @brief  Get UARTx kernel clock source
  * @rmtoll CCIPR1       UART4SEL     LL_RCC_GetUARTClockSource\n
  *         CCIPR1       UART5SEL     LL_RCC_GetUARTClockSource\n
  *         CCIPR1       UART7SEL     LL_RCC_GetUARTClockSource\n
  *         CCIPR1       UART8SEL     LL_RCC_GetUARTClockSource\n
  *         CCIPR1       UART9SEL     LL_RCC_GetUARTClockSource\n
  *         CCIPR2       UART12SEL    LL_RCC_GetUARTClockSource
  * @param  UARTx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_UART4_CLKSOURCE
  *         @arg @ref LL_RCC_UART5_CLKSOURCE
  *         @arg @ref LL_RCC_UART7_CLKSOURCE
  *         @arg @ref LL_RCC_UART8_CLKSOURCE
  *         @arg @ref LL_RCC_UART9_CLKSOURCE
  *         @arg @ref LL_RCC_UART12_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART4_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART5_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART7_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART8_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART9_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_UART12_CLKSOURCE_LSE
  */
__STATIC_INLINE uint32_t LL_RCC_GetUARTClockSource(uint32_t UARTx)
{
  return LL_RCC_GetClockSource(UARTx);
}
#endif /* UART4 */

/**
  * @brief  Get LPUARTx kernel clock source
  * @rmtoll CCIPR3       LPUART1SEL    LL_RCC_GetLPUARTClockSource
  * @param  LPUARTx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL2Q
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_LPUART1_CLKSOURCE_LSE
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  */
__STATIC_INLINE uint32_t LL_RCC_GetLPUARTClockSource(uint32_t LPUARTx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR3, LPUARTx));
}

/**
  * @brief  Get I2Cx kernel clock source
  * @rmtoll CCIPR4       I2C1SEL       LL_RCC_GetI2CClockSource\n
  *         CCIPR4       I2C2SEL       LL_RCC_GetI2CClockSource\n
  *         CCIPR4       I2C3SEL       LL_RCC_GetI2CClockSource\n
  *         CCIPR4       I2C4SEL       LL_RCC_GetI2CClockSource
  * @param  I2Cx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C1_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I2C2_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C3_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_I2C4_CLKSOURCE_CSI (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines only.
  *  (**) : For stm32h503xx family line only.
 */
__STATIC_INLINE uint32_t LL_RCC_GetI2CClockSource(uint32_t I2Cx)
{
  return LL_RCC_GetClockSource(I2Cx);
}

/**
  * @brief  Get I3Cx kernel clock source
  * @rmtoll CCIPR4       I3C1SEL    LL_RCC_GetI3CClockSource\n
  *         CCIPR4       I3C2SEL    LL_RCC_GetI3CClockSource
  * @param  I3Cx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE (**)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_I3C1_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PCLK3 (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_PLL2R (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_HSI   (**)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_CSI   (***)
  *         @arg @ref LL_RCC_I3C2_CLKSOURCE_NONE  (**)
  *
  *  (*)   : For stm32h56xxx, stm32h57xxx, stm32h5exxx and stm32h5fxxx family lines.
  *  (**)  : For stm32h503xx family line.
  *  (***) : For stm32h5exxx and stm32h5fxxx family lines.
  */
__STATIC_INLINE uint32_t LL_RCC_GetI3CClockSource(uint32_t I3Cx)
{
  return LL_RCC_GetClockSource(I3Cx);
}

/**
  * @brief  Get SPIx kernel clock source
  * @rmtoll CCIPR3       SPI1SEL       LL_RCC_GetSPIClockSource\n
  *         CCIPR3       SPI2SEL       LL_RCC_GetSPIClockSource\n
  *         CCIPR3       SPI3SEL       LL_RCC_GetSPIClockSource\n
  *         CCIPR3       SPI4SEL       LL_RCC_GetSPIClockSource\n
  *         CCIPR3       SPI5SEL       LL_RCC_GetSPIClockSource\n
  *         CCIPR3       SPI6SEL       LL_RCC_GetSPIClockSource
  * @param  SPIx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PLL3P (*)
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PLL3P  (*)
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SPI3_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI4_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI5_CLKSOURCE_HSE (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PCLK2 (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_CSI (*)
  *         @arg @ref LL_RCC_SPI6_CLKSOURCE_HSE (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
 */
__STATIC_INLINE uint32_t LL_RCC_GetSPIClockSource(uint32_t SPIx)
{
  return LL_RCC_GetClockSource(SPIx);
}

/**
  * @brief  Get LPTIMx kernel clock source
  * @rmtoll CCIPR2       LPTIM1SEL     LL_RCC_GetLPTIMClockSource\n
  *         CCIPR2       LPTIM2SEL     LL_RCC_GetLPTIMClockSource\n
  *         CCIPR2       LPTIM3SEL     LL_RCC_GetLPTIMClockSource\n
  *         CCIPR2       LPTIM4SEL     LL_RCC_GetLPTIMClockSource\n
  *         CCIPR2       LPTIM5SEL     LL_RCC_GetLPTIMClockSource\n
  *         CCIPR2       LPTIM6SEL     LL_RCC_GetLPTIMClockSource
  * @param  LPTIMx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_PLL3R  (*)
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PCLK1
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_LPTIM2_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM3_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM4_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM5_CLKSOURCE_CLKP (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PCLK3 (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL2P (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_PLL3R (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSE (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_LSI (*)
  *         @arg @ref LL_RCC_LPTIM6_CLKSOURCE_CLKP (*)
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  */
__STATIC_INLINE uint32_t LL_RCC_GetLPTIMClockSource(uint32_t LPTIMx)
{
  return LL_RCC_GetClockSource(LPTIMx);
}

/**
  * @brief  Enable TIM2,15 and LPTIM2 Input capture clock source
  * @rmtoll CCIPR1       TIMICSEL     LL_RCC_TIMIC_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_TIMIC_Enable(void)
{
  SET_BIT(RCC->CCIPR1, RCC_CCIPR1_TIMICSEL);
}

/**
  * @brief  Disable TIM2,15 and LPTIM2 Input capture clock source
  * @rmtoll CCIPR1       TIMICSEL      LL_RCC_TIMIC_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_TIMIC_Disable(void)
{
  CLEAR_BIT(RCC->CCIPR1, RCC_CCIPR1_TIMICSEL);
}

#if defined(RCC_CCIPR4_OTGPDCTRL)

/**
  * @brief  Set the power state of the OTG PHY in SUSPEND mode.
  * @rmtoll CCIPR4       OTGPDCTRL    LL_RCC_OTGPD_Enable
  * @param  PwrState Specifies the power state to set for the OTG PHY in SUSPEND mode.
  *         This parameter can be one of the following values:
  *           @arg RCC_OTGPD_SUSPEND_POWERON: Keep the PHY powered in SUSPEND mode (default after reset).
  *           @arg RCC_OTGPD_SUSPEND_POWERDOWN: Power down the PHY state machine, bias, and PLL in SUSPEND mode.
  * @retval None
  */
__STATIC_INLINE void LL_RCC_OTGPD_SetPWRSuspend(uint32_t PwrState)
{
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGPDCTRL, PwrState);
}
#endif /* RCC_CCIPR4_OTGPDCTRL */
/**
  * @brief  Get FDCAN kernel clock source
  * @rmtoll CCIPR5         FDCANSEL        LL_RCC_GetFDCANClockSource
  * @param  FDCANx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_FDCAN_CLKSOURCE_PLL2Q
  */
__STATIC_INLINE uint32_t LL_RCC_GetFDCANClockSource(uint32_t FDCANx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, FDCANx));
}

#if defined(SAI1)
/**
  * @brief  Get SAIx kernel clock source
  * @rmtoll CCIPR2       SAI1SEL       LL_RCC_GetSAIClockSource\n
  *         CCIPR2       SAI2SEL       LL_RCC_GetSAIClockSource
  * @param  SAIx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SAI1_CLKSOURCE_CLKP
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_SAI2_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetSAIClockSource(uint32_t SAIx)
{
  return LL_RCC_GetClockSource(SAIx);
}
#endif /* SAI1 */

#if defined(SDMMC1)
/**
  * @brief  Get SDMMCx kernel clock source
  * @rmtoll CCIPR4         SDMMC1SEL        LL_RCC_GetSDMMCClockSource
  *         CCIPR4         SDMMC2SEL        LL_RCC_GetSDMMCClockSource
  * @param  SDMMCx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE (*)
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_SDMMC1_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL1Q (*)
  *         @arg @ref LL_RCC_SDMMC2_CLKSOURCE_PLL2R (*)
  *
  *  (*) value not defined in all devices.
  */
__STATIC_INLINE uint32_t LL_RCC_GetSDMMCClockSource(uint32_t SDMMCx)
{
  return LL_RCC_GetClockSource(SDMMCx);
}
#endif /* SDMMC1 */

/**
  * @brief  Get RNGx kernel clock source
  * @rmtoll CCIPR5       RNGSEL      LL_RCC_GetRNGClockSource
  * @param  RNGx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_HSI48
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_RNG_CLKSOURCE_LSI
  */
__STATIC_INLINE uint32_t LL_RCC_GetRNGClockSource(uint32_t RNGx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, RNGx));
}

#if defined(USB_DRD_FS)
/**
  * @brief  Get USB clock source
  * @rmtoll CCIPR4       USBSEL      LL_RCC_GetUSBClockSource
  * @param  USBx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_USB_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_USB_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL2Q (*)
  *         @arg @ref LL_RCC_USB_CLKSOURCE_PLL3Q (*)
  *         @arg @ref LL_RCC_USB_CLKSOURCE_HSI48
  *
  * (*) : Available in some STM32H5 lines only.
  */
__STATIC_INLINE uint32_t LL_RCC_GetUSBClockSource(uint32_t USBx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, USBx));
}
#endif /* USB_DRD_FS */

/**
  * @brief  Get ADCDACx kernel clock source
  * @rmtoll CCIPR5       ADCDACSEL        LL_RCC_GetADCDACClockSource
  * @param  ADCDACx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_SYSCLK
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_ADCDAC_CLKSOURCE_CSI
  */
__STATIC_INLINE uint32_t LL_RCC_GetADCDACClockSource(uint32_t ADCDACx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, ADCDACx));
}

/**
  * @brief  Get DAC low-power kernel Clock Source
  * @rmtoll CCIPR5        DACSEL        LL_RCC_GetDACLPClockSource
  * @param  DACLPx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_DAC_LP_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_DAC_LP_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_DAC_LP_CLKSOURCE_LSI
  */
__STATIC_INLINE uint32_t LL_RCC_GetDACLPClockSource(uint32_t DACLPx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, DACLPx));
}

/**
  * @brief  Get CECx kernel clock source
  * @rmtoll CCIPR5        CECSEL        LL_RCC_GetCECClockSource
  * @param  CECx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CEC_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_CEC_CLKSOURCE_CSI_DIV122
  */
__STATIC_INLINE uint32_t LL_RCC_GetCECClockSource(uint32_t CECx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, CECx));
}

#if defined(OCTOSPI1)
/**
  * @brief  Get OCTOSPI kernel clock source
  * @rmtoll CCIPR4         OCTOSPISEL        LL_RCC_GetOCTOSPIClockSource
  * @param  OCTOSPIx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OCTOSPI_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_OSPI_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetOCTOSPIClockSource(uint32_t OCTOSPIx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, OCTOSPIx));
}
#endif /* OCTOSPI1 */

#if defined(PLAY1)
/**
  * @brief  Get PLAY1 kernel clock source
  * @rmtoll CCIPR3         PLAY1SEL        LL_RCC_GetPLAY1ClockSource
  * @param  PLAYx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PCLK3
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_PLAY1_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetPLAY1ClockSource(uint32_t PLAYx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR3, PLAYx));
}
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/**
  * @brief  Get OTGFS kernel clock source
  * @rmtoll CCIPR4         OTGFSSEL        LL_RCC_GetOTGFSClockSource
  * @param  OTGFSx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_HSI48
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_OTGFS_CLKSOURCE_CLK48 (*)
  *
  *  (*) : Available only on STM32H5E5x/STM32H5F5x when OTG_HS PHY is enabled.
  */
__STATIC_INLINE uint32_t LL_RCC_GetOTGFSClockSource(uint32_t OTGFSx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, OTGFSx));
}
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/**
  * @brief  Get OTGHS kernel clock source
  * @rmtoll CCIPR4         OTGHSSEL        LL_RCC_GetOTGHSClockSource
  * @param  OTGHSx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_PLL3Q
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_HSE_DIV2
  *         @arg @ref LL_RCC_OTGHS_CLKSOURCE_PLL1Q_DIV2
  */
__STATIC_INLINE uint32_t LL_RCC_GetOTGHSClockSource(uint32_t OTGHSx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, OTGHSx));
}
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/**
  * @brief  Get OTGPHY kernel clock source
  * @rmtoll CCIPR4         OTGPHYREFCKSEL    LL_RCC_GetOTGPHYClockSource
  * @param  OTGPHYx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OTGPHY_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_16M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_19_2M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_20M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_24M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_26M
  *         @arg @ref LL_RCC_OTGPHYREFCKCLKSOURCE_32M
  */
__STATIC_INLINE uint32_t LL_RCC_GetOTGPHYClockSource(uint32_t OTGPHYx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, OTGPHYx));
}
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(OCTOSPI2)
/**
  * @brief  Get OCTOSPI2 kernel clock source
  * @rmtoll CCIPR5         OCTOSPI2SEL        LL_RCC_GetOCTOSPI2ClockSource
  * @param  OCTOSPIx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_OCTOSPI2_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_PLL2R
  *         @arg @ref LL_RCC_OSPI2_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetOCTOSPI2ClockSource(uint32_t OCTOSPIx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, OCTOSPIx));
}
#endif /* OCTOSPI2 */

#if defined(LTDC)
/**
  * @brief  Get LTDC kernel clock source
  * @rmtoll CCIPR5         LTDCSEL        LL_RCC_GetLTDCClockSource
  * @param  LTDCx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE_PLL3R
  *         @arg @ref LL_RCC_LTDC_CLKSOURCE_PLL2R
  */
__STATIC_INLINE uint32_t LL_RCC_GetLTDCClockSource(uint32_t LTDCx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, LTDCx));
}
#endif /* LTDC */

#if defined(ADF1)
/**
  * @brief  Get ADF1 kernel clock source
  * @rmtoll CCIPR5         ADF1SEL        LL_RCC_GetADF1ClockSource
  * @param  ADFx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_ADF1_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetADF1ClockSource(uint32_t ADFx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, ADFx));
}
#endif /* ADF1 */

#if defined(MDF1)
/**
  * @brief  Get MDF1 kernel clock source
  * @rmtoll CCIPR5         MDF1SEL        LL_RCC_GetMDF1ClockSource
  * @param  MDFx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL2P
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PLL3P
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_PIN
  *         @arg @ref LL_RCC_MDF1_CLKSOURCE_CLKP
  */
__STATIC_INLINE uint32_t LL_RCC_GetMDF1ClockSource(uint32_t MDFx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, MDFx));
}
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/*
  * @brief  Get ETH kernel clock source
  * @rmtoll CCIPR4         ETHSEL        LL_RCC_GetETHClockSource
  * @param  ETHx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETH_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ETH_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_ETH_CLKSOURCE_PLL1Q
  */
__STATIC_INLINE uint32_t LL_RCC_GetETHClockSource(uint32_t ETHx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, ETHx));
}
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/*
  * @brief  Get ETHPTP kernel clock source
  * @rmtoll CCIPR5         ETHPTPSEL        LL_RCC_GetETHPTPClockSource
  * @param  ETHPTPx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_HCLK
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL1R
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ETHPTP_CLKSOURCE_PLL3P
  */
__STATIC_INLINE uint32_t LL_RCC_GetETHPTPClockSource(uint32_t ETHPTPx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, ETHPTPx));
}
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/*
  * @brief  Get ETHREF kernel clock source
  * @rmtoll CCIPR5         ETHREFSEL        LL_RCC_GetETHREFClockSource
  * @param  ETHREFx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHREF_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ETHREF_CLKSOURCE_PLL1Q
  *         @arg @ref LL_RCC_ETHREF_CLKSOURCE_PLL1R
  */
__STATIC_INLINE uint32_t LL_RCC_GetETHREFClockSource(uint32_t ETHREFx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, ETHREFx));
}
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

/**
  * @brief  Get CLKP kernel clock source
  * @rmtoll CCIPR5       CKPERSEL        LL_RCC_GetCLKPClockSource
  * @param  CLKPx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_CSI
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_HSE
  *         @arg @ref LL_RCC_CLKP_CLKSOURCE_NONE
  */
__STATIC_INLINE uint32_t LL_RCC_GetCLKPClockSource(uint32_t CLKPx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR5, CLKPx));
}

#if defined(PLAY1)
/**
  * @brief  Get PLAY1 kernel clock divider
  * @rmtoll CCIPR3         PLAY1DIV        LL_RCC_GetPLAY1ClockDivider
  * @param  PLAYx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLKDIVIDER
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_1
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_2
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_4
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_8
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_16
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_32
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_64
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_128
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_256
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_512
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_1024
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_2048
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_4096
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_8192
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_16384
  *         @arg @ref LL_RCC_PLAY1_CLK_DIV_32768
  */
__STATIC_INLINE uint32_t LL_RCC_GetPLAY1ClockDivider(uint32_t PLAYx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR3, PLAYx));
}
#endif /* PLAY1 */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/**
  * @brief  Get ETHPTP kernel clock divider
  * @rmtoll CCIPR4         ETHPTPDIV        LL_RCC_GetETHPTPClockDivider
  * @param  ETHPTPx This parameter can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLKDIVIDER
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_1
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_2
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_3
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_4
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_5
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_6
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_7
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_8
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_9
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_10
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_11
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_12
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_13
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_14
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_15
  *         @arg @ref LL_RCC_ETHPTP_CLK_DIV_16
  * @retval None
  */
__STATIC_INLINE uint32_t LL_RCC_GetETHPTPClockDivider(uint32_t ETHPTPx)
{
  return (uint32_t)(READ_BIT(RCC->CCIPR4, ETHPTPx));
}
#endif /* RCC_CCIPR4_ETHPTPDIV */
/**
  * @brief  Configure the Kernel wakeup clock source
  * @rmtoll CFGR1         STOPKERWUCK       LL_RCC_SetKerWakeUpClkSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_KERWAKEUP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_KERWAKEUP_CLKSOURCE_CSI
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetKerWakeUpClkSource(uint32_t Source)
{
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_STOPKERWUCK, Source);
}

/**
  * @brief  Get the Kernel wakeup clock source
  * @rmtoll CFGR1         STOPKERWUCK           LL_RCC_GetKerWakeUpClkSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_KERWAKEUP_CLKSOURCE_HSI
  *         @arg @ref LL_RCC_KERWAKEUP_CLKSOURCE_CSI
  */
__STATIC_INLINE uint32_t LL_RCC_GetKerWakeUpClkSource(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_STOPKERWUCK));
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_RTC RTC
  * @{
  */

/**
  * @brief  Set RTC Clock Source
  * @note Once the RTC clock source has been selected, it cannot be changed anymore unless
  *       the Backup domain is reset, or unless a failure is detected on LSE (LSECSSD is
  *       set). The BDRST bit can be used to reset them.
  * @rmtoll BDCR         RTCSEL        LL_RCC_SetRTCClockSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_HSE_DIV
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetRTCClockSource(uint32_t Source)
{
  MODIFY_REG(RCC->BDCR, RCC_BDCR_RTCSEL, Source);
}

/**
  * @brief  Get RTC Clock Source
  * @rmtoll BDCR         RTCSEL        LL_RCC_GetRTCClockSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_NONE
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_LSE
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_LSI
  *         @arg @ref LL_RCC_RTC_CLKSOURCE_HSE_DIV
  */
__STATIC_INLINE uint32_t LL_RCC_GetRTCClockSource(void)
{
  return (uint32_t)(READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL));
}

/**
  * @brief  Enable RTC
  * @rmtoll BDCR         RTCEN         LL_RCC_EnableRTC
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableRTC(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
}

/**
  * @brief  Disable RTC
  * @rmtoll BDCR         RTCEN         LL_RCC_DisableRTC
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableRTC(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_RTCEN);
}

/**
  * @brief  Check if RTC has been enabled or not
  * @rmtoll BDCR         RTCEN         LL_RCC_IsEnabledRTC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledRTC(void)
{
  return ((READ_BIT(RCC->BDCR, RCC_BDCR_RTCEN) == RCC_BDCR_RTCEN) ? 1UL : 0UL);
}

/**
  * @brief  Force the Backup domain reset
  * @rmtoll BDCR         BDRST         LL_RCC_ForceBackupDomainReset
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ForceBackupDomainReset(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_VSWRST);
}

/**
  * @brief  Release the Backup domain reset
  * @rmtoll BDCR         BDRST         LL_RCC_ReleaseBackupDomainReset
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ReleaseBackupDomainReset(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_VSWRST);
}

/**
  * @brief  Set HSE Prescalers for RTC Clock
  * @rmtoll CFGR1         RTCPRE        LL_RCC_SetRTC_HSEPrescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_RTC_HSE_NOCLOCK
  *         @arg @ref LL_RCC_RTC_HSE_DIV_2
  *         @arg @ref LL_RCC_RTC_HSE_DIV_3
  *         @arg @ref LL_RCC_RTC_HSE_DIV_4
  *         @arg @ref LL_RCC_RTC_HSE_DIV_5
  *         @arg @ref LL_RCC_RTC_HSE_DIV_6
  *         @arg @ref LL_RCC_RTC_HSE_DIV_7
  *         @arg @ref LL_RCC_RTC_HSE_DIV_8
  *         @arg @ref LL_RCC_RTC_HSE_DIV_9
  *         @arg @ref LL_RCC_RTC_HSE_DIV_10
  *         @arg @ref LL_RCC_RTC_HSE_DIV_11
  *         @arg @ref LL_RCC_RTC_HSE_DIV_12
  *         @arg @ref LL_RCC_RTC_HSE_DIV_13
  *         @arg @ref LL_RCC_RTC_HSE_DIV_14
  *         @arg @ref LL_RCC_RTC_HSE_DIV_15
  *         @arg @ref LL_RCC_RTC_HSE_DIV_16
  *         @arg @ref LL_RCC_RTC_HSE_DIV_17
  *         @arg @ref LL_RCC_RTC_HSE_DIV_18
  *         @arg @ref LL_RCC_RTC_HSE_DIV_19
  *         @arg @ref LL_RCC_RTC_HSE_DIV_20
  *         @arg @ref LL_RCC_RTC_HSE_DIV_21
  *         @arg @ref LL_RCC_RTC_HSE_DIV_22
  *         @arg @ref LL_RCC_RTC_HSE_DIV_23
  *         @arg @ref LL_RCC_RTC_HSE_DIV_24
  *         @arg @ref LL_RCC_RTC_HSE_DIV_25
  *         @arg @ref LL_RCC_RTC_HSE_DIV_26
  *         @arg @ref LL_RCC_RTC_HSE_DIV_27
  *         @arg @ref LL_RCC_RTC_HSE_DIV_28
  *         @arg @ref LL_RCC_RTC_HSE_DIV_29
  *         @arg @ref LL_RCC_RTC_HSE_DIV_30
  *         @arg @ref LL_RCC_RTC_HSE_DIV_31
  *         @arg @ref LL_RCC_RTC_HSE_DIV_32
  *         @arg @ref LL_RCC_RTC_HSE_DIV_33
  *         @arg @ref LL_RCC_RTC_HSE_DIV_34
  *         @arg @ref LL_RCC_RTC_HSE_DIV_35
  *         @arg @ref LL_RCC_RTC_HSE_DIV_36
  *         @arg @ref LL_RCC_RTC_HSE_DIV_37
  *         @arg @ref LL_RCC_RTC_HSE_DIV_38
  *         @arg @ref LL_RCC_RTC_HSE_DIV_39
  *         @arg @ref LL_RCC_RTC_HSE_DIV_40
  *         @arg @ref LL_RCC_RTC_HSE_DIV_41
  *         @arg @ref LL_RCC_RTC_HSE_DIV_42
  *         @arg @ref LL_RCC_RTC_HSE_DIV_43
  *         @arg @ref LL_RCC_RTC_HSE_DIV_44
  *         @arg @ref LL_RCC_RTC_HSE_DIV_45
  *         @arg @ref LL_RCC_RTC_HSE_DIV_46
  *         @arg @ref LL_RCC_RTC_HSE_DIV_47
  *         @arg @ref LL_RCC_RTC_HSE_DIV_48
  *         @arg @ref LL_RCC_RTC_HSE_DIV_49
  *         @arg @ref LL_RCC_RTC_HSE_DIV_50
  *         @arg @ref LL_RCC_RTC_HSE_DIV_51
  *         @arg @ref LL_RCC_RTC_HSE_DIV_52
  *         @arg @ref LL_RCC_RTC_HSE_DIV_53
  *         @arg @ref LL_RCC_RTC_HSE_DIV_54
  *         @arg @ref LL_RCC_RTC_HSE_DIV_55
  *         @arg @ref LL_RCC_RTC_HSE_DIV_56
  *         @arg @ref LL_RCC_RTC_HSE_DIV_57
  *         @arg @ref LL_RCC_RTC_HSE_DIV_58
  *         @arg @ref LL_RCC_RTC_HSE_DIV_59
  *         @arg @ref LL_RCC_RTC_HSE_DIV_60
  *         @arg @ref LL_RCC_RTC_HSE_DIV_61
  *         @arg @ref LL_RCC_RTC_HSE_DIV_62
  *         @arg @ref LL_RCC_RTC_HSE_DIV_63
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetRTC_HSEPrescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_RTCPRE, Prescaler);
}

/**
  * @brief  Get HSE Prescalers for RTC Clock
  * @rmtoll CFGR1         RTCPRE        LL_RCC_GetRTC_HSEPrescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_RTC_HSE_NOCLOCK
  *         @arg @ref LL_RCC_RTC_HSE_DIV_2
  *         @arg @ref LL_RCC_RTC_HSE_DIV_3
  *         @arg @ref LL_RCC_RTC_HSE_DIV_4
  *         @arg @ref LL_RCC_RTC_HSE_DIV_5
  *         @arg @ref LL_RCC_RTC_HSE_DIV_6
  *         @arg @ref LL_RCC_RTC_HSE_DIV_7
  *         @arg @ref LL_RCC_RTC_HSE_DIV_8
  *         @arg @ref LL_RCC_RTC_HSE_DIV_9
  *         @arg @ref LL_RCC_RTC_HSE_DIV_10
  *         @arg @ref LL_RCC_RTC_HSE_DIV_11
  *         @arg @ref LL_RCC_RTC_HSE_DIV_12
  *         @arg @ref LL_RCC_RTC_HSE_DIV_13
  *         @arg @ref LL_RCC_RTC_HSE_DIV_14
  *         @arg @ref LL_RCC_RTC_HSE_DIV_15
  *         @arg @ref LL_RCC_RTC_HSE_DIV_16
  *         @arg @ref LL_RCC_RTC_HSE_DIV_17
  *         @arg @ref LL_RCC_RTC_HSE_DIV_18
  *         @arg @ref LL_RCC_RTC_HSE_DIV_19
  *         @arg @ref LL_RCC_RTC_HSE_DIV_20
  *         @arg @ref LL_RCC_RTC_HSE_DIV_21
  *         @arg @ref LL_RCC_RTC_HSE_DIV_22
  *         @arg @ref LL_RCC_RTC_HSE_DIV_23
  *         @arg @ref LL_RCC_RTC_HSE_DIV_24
  *         @arg @ref LL_RCC_RTC_HSE_DIV_25
  *         @arg @ref LL_RCC_RTC_HSE_DIV_26
  *         @arg @ref LL_RCC_RTC_HSE_DIV_27
  *         @arg @ref LL_RCC_RTC_HSE_DIV_28
  *         @arg @ref LL_RCC_RTC_HSE_DIV_29
  *         @arg @ref LL_RCC_RTC_HSE_DIV_30
  *         @arg @ref LL_RCC_RTC_HSE_DIV_31
  *         @arg @ref LL_RCC_RTC_HSE_DIV_32
  *         @arg @ref LL_RCC_RTC_HSE_DIV_33
  *         @arg @ref LL_RCC_RTC_HSE_DIV_34
  *         @arg @ref LL_RCC_RTC_HSE_DIV_35
  *         @arg @ref LL_RCC_RTC_HSE_DIV_36
  *         @arg @ref LL_RCC_RTC_HSE_DIV_37
  *         @arg @ref LL_RCC_RTC_HSE_DIV_38
  *         @arg @ref LL_RCC_RTC_HSE_DIV_39
  *         @arg @ref LL_RCC_RTC_HSE_DIV_40
  *         @arg @ref LL_RCC_RTC_HSE_DIV_41
  *         @arg @ref LL_RCC_RTC_HSE_DIV_42
  *         @arg @ref LL_RCC_RTC_HSE_DIV_43
  *         @arg @ref LL_RCC_RTC_HSE_DIV_44
  *         @arg @ref LL_RCC_RTC_HSE_DIV_45
  *         @arg @ref LL_RCC_RTC_HSE_DIV_46
  *         @arg @ref LL_RCC_RTC_HSE_DIV_47
  *         @arg @ref LL_RCC_RTC_HSE_DIV_48
  *         @arg @ref LL_RCC_RTC_HSE_DIV_49
  *         @arg @ref LL_RCC_RTC_HSE_DIV_50
  *         @arg @ref LL_RCC_RTC_HSE_DIV_51
  *         @arg @ref LL_RCC_RTC_HSE_DIV_52
  *         @arg @ref LL_RCC_RTC_HSE_DIV_53
  *         @arg @ref LL_RCC_RTC_HSE_DIV_54
  *         @arg @ref LL_RCC_RTC_HSE_DIV_55
  *         @arg @ref LL_RCC_RTC_HSE_DIV_56
  *         @arg @ref LL_RCC_RTC_HSE_DIV_57
  *         @arg @ref LL_RCC_RTC_HSE_DIV_58
  *         @arg @ref LL_RCC_RTC_HSE_DIV_59
  *         @arg @ref LL_RCC_RTC_HSE_DIV_60
  *         @arg @ref LL_RCC_RTC_HSE_DIV_61
  *         @arg @ref LL_RCC_RTC_HSE_DIV_62
  *         @arg @ref LL_RCC_RTC_HSE_DIV_63
  */
__STATIC_INLINE uint32_t LL_RCC_GetRTC_HSEPrescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_RTCPRE));
}


/**
  * @}
  */

/** @defgroup RCC_LL_EF_TIM_CLOCK_PRESCALER TIM
  * @{
  */

/**
  * @brief  Set Timers Clock Prescalers
  * @rmtoll CFGR1         TIMPRE        LL_RCC_SetTIMPrescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref LL_RCC_TIM_PRESCALER_TWICE
  *         @arg @ref LL_RCC_TIM_PRESCALER_FOUR_TIMES
  * @retval None
  */
__STATIC_INLINE void LL_RCC_SetTIMPrescaler(uint32_t Prescaler)
{
  MODIFY_REG(RCC->CFGR1, RCC_CFGR1_TIMPRE, Prescaler);
}

/**
  * @brief  Get Timers Clock Prescalers
  * @rmtoll CFGR1         TIMPRE        LL_RCC_GetTIMPrescaler
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_TIM_PRESCALER_TWICE
  *         @arg @ref LL_RCC_TIM_PRESCALER_FOUR_TIMES
  */
__STATIC_INLINE uint32_t LL_RCC_GetTIMPrescaler(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR1, RCC_CFGR1_TIMPRE));
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_PLL1 PLL1
  * @{
  */

/**
  * @brief  Enable PLL1
  * @rmtoll CR           PLL1ON         LL_RCC_PLL1_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_PLL1ON);
}

/**
  * @brief  Disable PLL1
  * @note Cannot be disabled if the PLL1 clock is used as the system clock
  * @rmtoll CR           PLLON         LL_RCC_PLL1_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);
}

/**
  * @brief  Check if PLL1 Ready
  * @rmtoll CR           PLL1RDY        LL_RCC_PLL1_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == RCC_CR_PLL1RDY) ? 1UL : 0UL);
}

/**
  * @brief  Enable PLL1 P output mapped to SYSCLK
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1CFGR      PLL1PEN        LL_RCC_PLL1P_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1P_Enable(void)
{
  SET_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1PEN);
}

/**
  * @brief  Disable PLL1 P output mapped to SYSCLK
  * @note Cannot be disabled if the PLL1 clock is used as the system
  *       clock
  * @rmtoll PLL1CFGR      PLL1PEN        LL_RCC_PLL1P_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1P_Disable(void)
{
  CLEAR_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1PEN);
}

/**
  * @brief  Enable PLL1 Q output
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1CFGR      PLL1QEN        LL_RCC_PLL1Q_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1Q_Enable(void)
{
  SET_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1QEN);
}

/**
  * @brief  Disable PLL1 Q output
  * @note In order to save power, when the PLL1 Q output of the PLL1 is
  *       not used,  PLL1Q  should be 0
  * @rmtoll PLL1CFGR      PLL1QEN        LL_RCC_PLL1Q_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1Q_Disable(void)
{
  CLEAR_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1QEN);
}

/**
  * @brief  Enable PLL1 R output
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1CFGR      PLL1REN        LL_RCC_PLL1R_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1R_Enable(void)
{
  SET_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1REN);
}

/**
  * @brief  Disable PLL1 R output
  * @note In order to save power, when the PLL1 R output of the PLL1 is
  *       not used,  PLL1R  should be 0
  * @rmtoll PLL1CFGR      PLL1REN        LL_RCC_PLL1R_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1R_Disable(void)
{
  CLEAR_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1REN);
}

/**
  * @brief  Check if PLL1 P is enabled
  * @rmtoll PLLCFGR           DIVP1EN         LL_RCC_PLL1P_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1P_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1PEN) == RCC_PLL1CFGR_PLL1PEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL1 Q is enabled
  * @rmtoll PLLCFGR           DIVQ1EN         LL_RCC_PLL1Q_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1Q_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1QEN) == RCC_PLL1CFGR_PLL1QEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL1 R is enabled
  * @rmtoll PLLCFGR           DIVR1EN         LL_RCC_PLL1R_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1R_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1REN) == RCC_PLL1CFGR_PLL1REN) ? 1UL : 0UL);
}

/**
  * @brief  Configure PLL1 used for SYSCLK
  * @note PLL1 Source, PLL1M, PLL1N and PLL1P can be written only when PLL1 is disabled.
  * @rmtoll PLL1CFGR      PLL1SRC        LL_RCC_PLL1_ConfigDomain_SYS\n
  *         PLL1CFGR      PLL1M          LL_RCC_PLL1_ConfigDomain_SYS\n
  *         PLL1CFGR      PLL1N          LL_RCC_PLL1_ConfigDomain_SYS\n
  *         PLL1CFGR      PLL1R          LL_RCC_PLL1_ConfigDomain_SYS
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLL1SOURCE_NONE
  *         @arg @ref LL_RCC_PLL1SOURCE_HSI
  *         @arg @ref LL_RCC_PLL1SOURCE_CSI
  *         @arg @ref LL_RCC_PLL1SOURCE_HSE
  * @param PLL1M parameter can be a value between 1 and 63
  * @param PLL1P parameter can be a value between 1 and 128 (odd values not allowed)
  * @param PLL1N parameter can be a value between 4 and 512
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_ConfigDomain_SYS(uint32_t Source, uint32_t PLL1M, uint32_t PLL1N, uint32_t PLL1P)
{
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1SRC | RCC_PLL1CFGR_PLL1M, Source | (PLL1M << RCC_PLL1CFGR_PLL1M_Pos));
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1N | RCC_PLL1DIVR_PLL1P, \
             ((PLL1N - 1UL) << RCC_PLL1DIVR_PLL1N_Pos) | ((PLL1P - 1UL) << RCC_PLL1DIVR_PLL1P_Pos));
}

/**
  * @brief  Configure PLL clock source
  * @rmtoll PLL1CFGR      PLL1SRC        LL_RCC_PLL1_SetSource
  * @param  PLL1Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLL1SOURCE_NONE
  *         @arg @ref LL_RCC_PLL1SOURCE_HSI
  *         @arg @ref LL_RCC_PLL1SOURCE_CSI
  *         @arg @ref LL_RCC_PLL1SOURCE_HSE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_SetSource(uint32_t PLL1Source)
{
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1SRC, PLL1Source);
}

/**
  * @brief  Get the oscillator used as PLL1 clock source.
  * @rmtoll PLL1CFGR      PLL1SRC        LL_RCC_PLL1_GetSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_PLL1SOURCE_NONE
  *         @arg @ref LL_RCC_PLL1SOURCE_CSI
  *         @arg @ref LL_RCC_PLL1SOURCE_HSI
  *         @arg @ref LL_RCC_PLL1SOURCE_HSE
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetSource(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1SRC));
}

/**
  * @brief  Set Main PLL1 multiplication factor for VCO
  * @rmtoll PLL1CFGR      PLL1N          LL_RCC_PLL1_SetN
  * @param PLL1N parameter can be a value between 4 and 512
  */
__STATIC_INLINE void LL_RCC_PLL1_SetN(uint32_t PLL1N)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1N, (PLL1N - 1UL) << RCC_PLL1DIVR_PLL1N_Pos);
}

/**
  * @brief  Get Main PLL1 multiplication factor for VCO
  * @rmtoll PLL1CFGR      PLL1N          LL_RCC_PLL1_GetN
  * @retval Between 4 and 512
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetN(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1N) >>  RCC_PLL1DIVR_PLL1N_Pos) + 1UL);
}

/**
  * @brief  Set Main PLL1 division factor for PLL1P
  * @note Used for System clock
  * @rmtoll PLL1CFGR      PLL1P       LL_RCC_PLL1_SetP
  * @param PLL1P parameter can be a value between 2 and 128 (odd value not allowed)
  */
__STATIC_INLINE void LL_RCC_PLL1_SetP(uint32_t PLL1P)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1P, (PLL1P - 1UL) << RCC_PLL1DIVR_PLL1P_Pos);
}

/**
  * @brief  Get PLL1 division factor for PLL1P
  * @note Used for System clock
  * @rmtoll PLL1CFGR      PLL1P      LL_RCC_PLL1_GetP
  * @retval Between 2 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetP(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1P) >>  RCC_PLL1DIVR_PLL1P_Pos) + 1UL);
}


/**
  * @brief  Set PLL1 division factor for PLL1Q
  * @note Used for peripherals clocks
  * @rmtoll PLLCFGR      PLL1Q          LL_RCC_PLL1_SetQ
  * @param PLL1Q parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL1_SetQ(uint32_t PLL1Q)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1Q, (PLL1Q - 1UL) << RCC_PLL1DIVR_PLL1Q_Pos);
}

/**
  * @brief  Get PLL1 division factor for PLL1Q
  * @note Used for peripherals clocks
  * @rmtoll PLL1CFGR      PLL1Q          LL_RCC_PLL1_GetQ
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetQ(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1Q) >>  RCC_PLL1DIVR_PLL1Q_Pos) + 1UL);
}

/**
  * @brief  Set PLL1 division factor for PLL1R
  * @note Used for trace
  * @rmtoll PLL1DIVR      PLL1R          LL_RCC_PLL1_SetR
  * @param PLL1R parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL1_SetR(uint32_t PLL1R)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1R, (PLL1R - 1UL) << RCC_PLL1DIVR_PLL1R_Pos);
}

/**
  * @brief  Get Main PLL1 division factor for PLL1R
  * @note Used for trace
  * @rmtoll PLL1DIVR      PLL1R          LL_RCC_PLL1_GetR
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetR(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL1DIVR, RCC_PLL1DIVR_PLL1R) >>  RCC_PLL1DIVR_PLL1R_Pos) + 1UL);
}

/**
  * @brief  Set Division factor for the main PLL and other PLL
  * @rmtoll PLL1CFGR      PLL1M          LL_RCC_PLL1_SetM
  * @param PLL1M parameter can be a value between 1 and 63
  */
__STATIC_INLINE void LL_RCC_PLL1_SetM(uint32_t PLL1M)
{
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1M, PLL1M  << RCC_PLL1CFGR_PLL1M_Pos);
}

/**
  * @brief  Get Division factor for the main PLL and other PLL
  * @rmtoll PLL1CFGR      PLL1M          LL_RCC_PLL1_GetM
  * @retval Between 0 and 63
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetM(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1M) >>  RCC_PLL1CFGR_PLL1M_Pos);
}

/**
  * @brief  Enable PLL1 FRACN
  * @rmtoll PLL1CFGR           PLL1FRACEN         LL_RCC_PLL1FRACN_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1FRACN_Enable(void)
{
  SET_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1FRACEN);
}

/**
  * @brief  Check if PLL1 FRACN is enabled
  * @rmtoll PLL1CFGR           PLL1FRACEN         LL_RCC_PLL1FRACN_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1FRACN_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1FRACEN) == RCC_PLL1CFGR_PLL1FRACEN) ? 1UL : 0UL);
}

/**
  * @brief  Disable PLL1 FRACN
  * @rmtoll PLL1CFGR           PLL1FRACEN         LL_RCC_PLL1FRACN_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1FRACN_Disable(void)
{
  CLEAR_BIT(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1FRACEN);
}

/**
  * @brief  Set PLL1 FRACN Coefficient
  * @rmtoll PLL1FRACR        PLL1FRACN        LL_RCC_PLL1_SetFRACN
  * @param  FRACN parameter can be a value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE void LL_RCC_PLL1_SetFRACN(uint32_t FRACN)
{
  MODIFY_REG(RCC->PLL1FRACR, RCC_PLL1FRACR_PLL1FRACN, FRACN << RCC_PLL1FRACR_PLL1FRACN_Pos);
}

/**
  * @brief  Get PLL1 FRACN Coefficient
  * @rmtoll PLL1FRACR      PLL1FRACN          LL_RCC_PLL1_GetFRACN
  * @retval A value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE uint32_t LL_RCC_PLL1_GetFRACN(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL1FRACR, RCC_PLL1FRACR_PLL1FRACN) >>  RCC_PLL1FRACR_PLL1FRACN_Pos);
}

/**
  * @brief  Set PLL1 VCO Input Range
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1CFGR        PLL1RGE       LL_RCC_PLL1_SetVCOInputRange
  * @param  InputRange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLINPUTRANGE_1_2
  *         @arg @ref LL_RCC_PLLINPUTRANGE_2_4
  *         @arg @ref LL_RCC_PLLINPUTRANGE_4_8
  *         @arg @ref LL_RCC_PLLINPUTRANGE_8_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_SetVCOInputRange(uint32_t InputRange)
{
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1RGE, InputRange << RCC_PLL1CFGR_PLL1RGE_Pos);
}

/**
  * @brief  Set PLL1 VCO OutputRange
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLLCFGR        PLL1VCOSEL       LL_RCC_PLL1_SetVCOOutputRange
  * @param  VCORange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLVCORANGE_WIDE
  *         @arg @ref LL_RCC_PLLVCORANGE_MEDIUM
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL1_SetVCOOutputRange(uint32_t VCORange)
{
  MODIFY_REG(RCC->PLL1CFGR, RCC_PLL1CFGR_PLL1VCOSEL, VCORange << RCC_PLL1CFGR_PLL1VCOSEL_Pos);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_PLL2 PLL2
  * @{
  */

/**
  * @brief  Enable PLL2
  * @rmtoll CR           PLL2ON     LL_RCC_PLL2_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_PLL2ON);
}

/**
  * @brief  Disable PLL2
  * @rmtoll CR           PLL2ON     LL_RCC_PLL2_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_PLL2ON);
}

/**
  * @brief  Check if PLL2 Ready
  * @rmtoll CR           PLL2RDY    LL_RCC_PLL2_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_PLL2RDY) == RCC_CR_PLL2RDY) ? 1UL : 0UL);
}

/**
  * @brief  Configure PLL2 clock source
  * @rmtoll PLL2CFGR      PLL2SRC        LL_RCC_PLL2_SetSource
  * @param  PLL2Source This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLL2SOURCE_NONE
  *         @arg @ref LL_RCC_PLL2SOURCE_CSI
  *         @arg @ref LL_RCC_PLL2SOURCE_HSI
  *         @arg @ref LL_RCC_PLL2SOURCE_HSE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2_SetSource(uint32_t PLL2Source)
{
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2SRC, PLL2Source);
}

/**
  * @brief  Get the oscillator used as PLL2 clock source.
  * @rmtoll PLL2CFGR      PLL2SRC        LL_RCC_PLL2_GetSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_PLL2SOURCE_NONE
  *         @arg @ref LL_RCC_PLL2SOURCE_CSI
  *         @arg @ref LL_RCC_PLL2SOURCE_HSI
  *         @arg @ref LL_RCC_PLL2SOURCE_HSE
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetSource(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2SRC));
}

/**
  * @brief  Set PLL2 Division factor M
  * @note   This API shall be called only when PLL2 is disabled.
  * @rmtoll PLL2CFGR      PLL2M          LL_RCC_PLL2_SetM
  * @param PLL2M parameter can be a value between 1 and 63
  */
__STATIC_INLINE void LL_RCC_PLL2_SetM(uint32_t PLL2M)
{
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2M, PLL2M << RCC_PLL2CFGR_PLL2M_Pos);
}

/**
  * @brief  Get PLL2 division factor M
  * @rmtoll PLL2CFGR      PLL2M          LL_RCC_PLL2_GetM
  * @retval Between 1 and 63
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetM(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2M) >>  RCC_PLL2CFGR_PLL2M_Pos);
}

/**
  * @brief  Set PLL2 multiplication factor N
  * @rmtoll PLL2CFGR      PLL2N          LL_RCC_PLL2_SetN
  * @param PLL2N parameter can be a value between 4 and 512
  */
__STATIC_INLINE void LL_RCC_PLL2_SetN(uint32_t PLL2N)
{
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2N, (PLL2N - 1UL) << RCC_PLL2DIVR_PLL2N_Pos);
}

/**
  * @brief  Get PLL2 multiplication factor N
  * @rmtoll PLL2CFGR      PLL2N          LL_RCC_PLL2_GetN
  * @retval Between 4 and 512
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetN(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2N) >>  RCC_PLL2DIVR_PLL2N_Pos) + 1UL);
}

/**
  * @brief  Set PLL2 division factor P
  * @note Used for peripherals clocks
  * @rmtoll PLL2CFGR      PLL2P       LL_RCC_PLL2_SetP
  * @param PLL2P parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL2_SetP(uint32_t PLL2P)
{
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2P, (PLL2P - 1UL) << RCC_PLL2DIVR_PLL2P_Pos);
}

/**
  * @brief  Get PLL2 division factor P
  * @note Used for peripherals clocks
  * @rmtoll PLL2CFGR      PLL2P      LL_RCC_PLL2_GetP
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetP(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2P) >>  RCC_PLL2DIVR_PLL2P_Pos) + 1UL);
}


/**
  * @brief  Set PLL2 division factor Q
  * @note Used for peripherals clocks
  * @rmtoll PLLCFGR      PLL2Q          LL_RCC_PLL2_SetQ
  * @param PLL2Q parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL2_SetQ(uint32_t PLL2Q)
{
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2Q, (PLL2Q - 1UL) << RCC_PLL2DIVR_PLL2Q_Pos);
}

/**
  * @brief  Get PLL2 division factor Q
  * @note Used for peripherals clocks
  * @rmtoll PLL2CFGR      PLL2Q          LL_RCC_PLL2_GetQ
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetQ(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2Q) >>  RCC_PLL2DIVR_PLL2Q_Pos) + 1UL);
}

/**
  * @brief  Set PLL2 division factor R
  * @note Used for PLL2CLK selected for peripherals clocks
  * @rmtoll PLL2CFGR      PLL2Q          LL_RCC_PLL2_SetR
  * @param PLL2R parameter can be a value between 1 and 128
  */
__STATIC_INLINE void  LL_RCC_PLL2_SetR(uint32_t PLL2R)
{
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2R, (PLL2R - 1UL) << RCC_PLL2DIVR_PLL2R_Pos);
}

/**
  * @brief  Get PLL2 division factor R
  * @note Used for PLL2CLK (system clock)
  * @rmtoll PLL2DIVR      PLL2R          LL_RCC_PLL2_GetR
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetR(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL2DIVR, RCC_PLL2DIVR_PLL2R) >>  RCC_PLL2DIVR_PLL2R_Pos) + 1UL);
}

/**
  * @brief  Enable PLL2 P output
  * @rmtoll PLL2CFGR  PLL2PEN    LL_RCC_PLL2P_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2P_Enable(void)
{
  SET_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2PEN);
}

/**
  * @brief  Disable PLL2 P output
  * @note In order to save power, when PLL2P output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL2CFGR  PLL2PEN    LL_RCC_PLL2P_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2P_Disable(void)
{
  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2PEN);
}

/**
  * @brief  Enable PLL2 Q output
  * @rmtoll PLL2CFGR  PLL2QEN    LL_RCC_PLL2Q_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2Q_Enable(void)
{
  SET_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2QEN);
}

/**
  * @brief  Disable PLL2 Q output
  * @note In order to save power, when PLL2Q output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL2CFGR  PLL2QEN    LL_RCC_PLL2_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2Q_Disable(void)
{
  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2QEN);
}

/**
  * @brief  Enable PLL2 R output
  * @rmtoll PLL2CFGR  PLL2REN    LL_RCC_PLL2R_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2R_Enable(void)
{
  SET_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2REN);
}

/**
  * @brief  Disable PLL2 R output
  * @note In order to save power, when PLL2R output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL2CFGR  PLL2REN    LL_RCC_PLL2R_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2R_Disable(void)
{
  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2REN);
}

/**
  * @brief  Check if PLL2 P is enabled
  * @rmtoll PLL2CFGR           PLL2PEN         LL_RCC_PLL2P_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2P_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2PEN) == RCC_PLL2CFGR_PLL2PEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL2 Q is enabled
  * @rmtoll PLL2CFGR           PLL2QEN         LL_RCC_PLL2Q_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2Q_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2QEN) == RCC_PLL2CFGR_PLL2QEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL2 R is enabled
  * @rmtoll PLL2CFGR           PLL2REN         LL_RCC_PLL2R_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2R_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2REN) == RCC_PLL2CFGR_PLL2REN) ? 1UL : 0UL);
}

/**
  * @brief  Enable PLL2 FRACN
  * @rmtoll PLL2CFGR           PLL2FRACEN         LL_RCC_PLL2FRACN_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2FRACN_Enable(void)
{
  SET_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2FRACEN);
}

/**
  * @brief  Check if PLL2 FRACN is enabled
  * @rmtoll PLL2CFGR           PLL2FRACEN         LL_RCC_PLL2FRACN_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2FRACN_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2FRACEN) == RCC_PLL2CFGR_PLL2FRACEN) ? 1UL : 0UL);
}

/**
  * @brief  Disable PLL2 FRACN
  * @rmtoll PLL2CFGR           PLL2FRACEN         LL_RCC_PLL2FRACN_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2FRACN_Disable(void)
{
  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2FRACEN);
}

/**
  * @brief  Set PLL2 FRACN Coefficient
  * @rmtoll PLL2FRACR        PLL2FRACN        LL_RCC_PLL2_SetFRACN
  * @param  FRACN parameter can be a value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE void LL_RCC_PLL2_SetFRACN(uint32_t FRACN)
{
  MODIFY_REG(RCC->PLL2FRACR, RCC_PLL2FRACR_PLL2FRACN, FRACN << RCC_PLL2FRACR_PLL2FRACN_Pos);
}

/**
  * @brief  Get PLL2 FRACN Coefficient
  * @rmtoll PLL2FRACR      PLL2FRACN          LL_RCC_PLL2_GetFRACN
  * @retval A value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE uint32_t LL_RCC_PLL2_GetFRACN(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL2FRACR, RCC_PLL2FRACR_PLL2FRACN) >>  RCC_PLL2FRACR_PLL2FRACN_Pos);
}

/**
  * @brief  Set PLL2 VCO Input Range
  * @note   This API shall be called only when PLL2 is disabled.
  * @rmtoll PLL2CFGR        PLL2RGE       LL_RCC_PLL2_SetVCOInputRange
  * @param  InputRange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLINPUTRANGE_1_2
  *         @arg @ref LL_RCC_PLLINPUTRANGE_2_4
  *         @arg @ref LL_RCC_PLLINPUTRANGE_4_8
  *         @arg @ref LL_RCC_PLLINPUTRANGE_8_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2_SetVCOInputRange(uint32_t InputRange)
{
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2RGE, InputRange << RCC_PLL2CFGR_PLL2RGE_Pos);
}

/**
  * @brief  Set PLL2 VCO OutputRange
  * @note   This API shall be called only when PLL2 is disabled.
  * @rmtoll PLL2CFGR        PLL2VCOSEL       LL_RCC_PLL2_SetVCOOutputRange
  * @param  VCORange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLVCORANGE_WIDE
  *         @arg @ref LL_RCC_PLLVCORANGE_MEDIUM
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL2_SetVCOOutputRange(uint32_t VCORange)
{
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2VCOSEL, VCORange << RCC_PLL2CFGR_PLL2VCOSEL_Pos);
}

/**
  * @}
  */

#if defined(RCC_CR_PLL3ON)
/** @defgroup RCC_LL_EF_PLL3 PLL3
  * @{
  */

/**
  * @brief  Enable PLL3
  * @rmtoll CR           PLL3ON     LL_RCC_PLL3_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_PLL3ON);
}

/**
  * @brief  Disable PLL3
  * @rmtoll CR           PLL3ON     LL_RCC_PLL3_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3_Disable(void)
{
  CLEAR_BIT(RCC->CR, RCC_CR_PLL3ON);
}

/**
  * @brief  Check if PLL3 is Ready
  * @rmtoll CR           PLL3RDY    LL_RCC_PLL3_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_PLL3RDY) == RCC_CR_PLL3RDY) ? 1UL : 0UL);
}


/**
  * @brief  Configure PLL3 clock source
  * @rmtoll PLL3CFGR      PLL3SRC        LL_RCC_PLL3_SetSource
  * @param  PLLSource This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLL3SOURCE_NONE
  *         @arg @ref LL_RCC_PLL3SOURCE_CSI
  *         @arg @ref LL_RCC_PLL3SOURCE_HSI
  *         @arg @ref LL_RCC_PLL3SOURCE_HSE
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3_SetSource(uint32_t PLLSource)
{
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3SRC, PLLSource);
}

/**
  * @brief  Get the oscillator used as PLL3 clock source.
  * @rmtoll PLL3CFGR      PLL3SRC        LL_RCC_PLL3_GetSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_PLL3SOURCE_NONE
  *         @arg @ref LL_RCC_PLL3SOURCE_CSI
  *         @arg @ref LL_RCC_PLL3SOURCE_HSI
  *         @arg @ref LL_RCC_PLL3SOURCE_HSE
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetSource(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3SRC));
}

/**
  * @brief  Set PLL3 multiplication factor N
  * @rmtoll PLL3CFGR      PLL3N          LL_RCC_PLL3_SetN
  * @param PLL3N parameter can be a value between 4 and 512
  */
__STATIC_INLINE void LL_RCC_PLL3_SetN(uint32_t PLL3N)
{
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3N, (PLL3N - 1UL) << RCC_PLL3DIVR_PLL3N_Pos);
}

/**
  * @brief  Get PLL3 multiplication factor N
  * @rmtoll PLL3CFGR      PLL3N          LL_RCC_PLL3_GetN
  * @retval Between 4 and 512
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetN(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3N) >>  RCC_PLL3DIVR_PLL3N_Pos) + 1UL);
}

/**
  * @brief  Set PLL3 division factor P
  * @note Used for peripherals clocks
  * @rmtoll PLL3CFGR      PLL3P       LL_RCC_PLL3_SetP
  * @param PLL3P parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL3_SetP(uint32_t PLL3P)
{
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3P, (PLL3P - 1UL) << RCC_PLL3DIVR_PLL3P_Pos);
}

/**
  * @brief  Get PLL3 division factor P
  * @note Used for peripherals clocks
  * @rmtoll PLL3CFGR      PLL3P      LL_RCC_PLL3_GetP
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetP(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3P) >>  RCC_PLL3DIVR_PLL3P_Pos) + 1UL);
}

/**
  * @brief  Set PLL3 division factor Q
  * @note Used for peripherals clocks
  * @rmtoll PLLCFGR      PLL3Q          LL_RCC_PLL3_SetQ
  * @param PLL3Q parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL3_SetQ(uint32_t PLL3Q)
{
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3Q, (PLL3Q - 1UL) << RCC_PLL3DIVR_PLL3Q_Pos);
}

/**
  * @brief  Get PLL3 division factor Q
  * @note Used for peripherals clocks
  * @rmtoll PLL3CFGR      PLL3Q          LL_RCC_PLL3_GetQ
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetQ(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3Q) >>  RCC_PLL3DIVR_PLL3Q_Pos) + 1UL);
}

/**
  * @brief  Set PLL3 division factor R
  * @note Used for peripherals clocks
  * @rmtoll PLL3CFGR      PLL3Q          LL_RCC_PLL3_SetR
  * @param PLL3R parameter can be a value between 1 and 128
  */
__STATIC_INLINE void LL_RCC_PLL3_SetR(uint32_t PLL3R)
{
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3R, (PLL3R - 1UL) << RCC_PLL3DIVR_PLL3R_Pos);
}

/**
  * @brief  Get PLL3 division factor R
  * @note Used for PLL3CLK (system clock)
  * @rmtoll PLL3DIVR      PLL3R          LL_RCC_PLL3_GetR
  * @retval Between 1 and 128
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetR(void)
{
  return (uint32_t)((READ_BIT(RCC->PLL3DIVR, RCC_PLL3DIVR_PLL3R) >>  RCC_PLL3DIVR_PLL3R_Pos) + 1UL);
}

/**
  * @brief  Set PLL3 Division factor M
  * @rmtoll PLL3CFGR      PLL3M          LL_RCC_PLL3_SetM
  * @param PLL3M parameter can be a value between 1 and 63
  */
__STATIC_INLINE void LL_RCC_PLL3_SetM(uint32_t PLL3M)
{
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3M, PLL3M << RCC_PLL3CFGR_PLL3M_Pos);
}

/**
  * @brief  Get PLL3 Division factor M
  * @rmtoll PLL3CFGR      PLL3M          LL_RCC_PLL3_GetM
  * @retval Between 1 and 63
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetM(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3M) >> RCC_PLL3CFGR_PLL3M_Pos);
}

/**
  * @brief  Enable PLL3 P output
  * @rmtoll PLL3CFGR  PLL3PEN    LL_RCC_PLL3P_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3P_Enable(void)
{
  SET_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3PEN);
}

/**
  * @brief  Disable PLL3 P output
  * @note In order to save power, when PLL3P output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL3CFGR  PLL3PEN    LL_RCC_PLL3P_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3P_Disable(void)
{
  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3PEN);
}

/**
  * @brief  Enable PLL3 Q output
  * @rmtoll PLL3CFGR  PLL3QEN    LL_RCC_PLL3Q_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3Q_Enable(void)
{
  SET_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3QEN);
}

/**
  * @brief  Disable PLL3 Q output
  * @note In order to save power, when PLL3Q output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL3CFGR  PLL3QEN    LL_RCC_PLL3Q_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3Q_Disable(void)
{
  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3QEN);
}

/**
  * @brief  Enable PLL3 R output
  * @rmtoll PLL3CFGR  PLL3REN    LL_RCC_PLL3R_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3R_Enable(void)
{
  SET_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3REN);
}

/**
  * @brief  Disable PLL3 R output
  * @note In order to save power, when PLL3R output is
  *       not used, it should be disabled (at any time)
  * @rmtoll PLL3CFGR  PLL3REN    LL_RCC_PLL3R_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3R_Disable(void)
{
  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3REN);
}

/**
  * @brief  Check if PLL3 P is enabled
  * @rmtoll PLL3CFGR           PLL3PEN         LL_RCC_PLL3P_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3P_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3PEN) == RCC_PLL3CFGR_PLL3PEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL3 Q is enabled
  * @rmtoll PLL3CFGR           PLL3QEN         LL_RCC_PLL3Q_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3Q_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3QEN) == RCC_PLL3CFGR_PLL3QEN) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL3 R is enabled
  * @rmtoll PLL3CFGR           PLL3REN         LL_RCC_PLL3R_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3R_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3REN) == RCC_PLL3CFGR_PLL3REN) ? 1UL : 0UL);
}

/**
  * @brief  Enable PLL3 FRACN
  * @rmtoll PLL3CFGR           PLL3FRACEN         LL_RCC_PLL3FRACN_Enable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3FRACN_Enable(void)
{
  SET_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3FRACEN);
}

/**
  * @brief  Check if PLL3 FRACN is enabled
  * @rmtoll PLL3CFGR           PLL3FRACEN         LL_RCC_PLL3FRACN_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3FRACN_IsEnabled(void)
{
  return ((READ_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3FRACEN) == RCC_PLL3CFGR_PLL3FRACEN) ? 1UL : 0UL);
}

/**
  * @brief  Disable PLL3 FRACN
  * @rmtoll PLL3CFGR           PLL3FRACEN         LL_RCC_PLL3FRACN_Disable
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3FRACN_Disable(void)
{
  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3FRACEN);
}

/**
  * @brief  Set PLL3 FRACN Coefficient
  * @rmtoll PLL3FRACR        PLL3FRACN        LL_RCC_PLL3_SetFRACN
  * @param  FRACN parameter can be a value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE void LL_RCC_PLL3_SetFRACN(uint32_t FRACN)
{
  MODIFY_REG(RCC->PLL3FRACR, RCC_PLL3FRACR_PLL3FRACN, FRACN << RCC_PLL3FRACR_PLL3FRACN_Pos);
}

/**
  * @brief  Get PLL3 FRACN Coefficient
  * @rmtoll PLL3FRACR      PLL3FRACN          LL_RCC_PLL3_GetFRACN
  * @retval A value between 0 and 8191 (0x1FFF)
  */
__STATIC_INLINE uint32_t LL_RCC_PLL3_GetFRACN(void)
{
  return (uint32_t)(READ_BIT(RCC->PLL3FRACR, RCC_PLL3FRACR_PLL3FRACN) >>  RCC_PLL3FRACR_PLL3FRACN_Pos);
}

/**
  * @brief  Set PLL3 VCO Input Range
  * @note   This API shall be called only when PLL3 is disabled.
  * @rmtoll PLL3CFGR        PLL3RGE       LL_RCC_PLL3_SetVCOInputRange
  * @param  InputRange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLINPUTRANGE_1_2
  *         @arg @ref LL_RCC_PLLINPUTRANGE_2_4
  *         @arg @ref LL_RCC_PLLINPUTRANGE_4_8
  *         @arg @ref LL_RCC_PLLINPUTRANGE_8_16
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3_SetVCOInputRange(uint32_t InputRange)
{
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3RGE, InputRange << RCC_PLL3CFGR_PLL3RGE_Pos);
}

/**
  * @brief  Set PLL3 VCO OutputRange
  * @note   This API shall be called only when PLL3 is disabled.
  * @rmtoll PLL3CFGR        PLL3VCOSEL       LL_RCC_PLL3_SetVCOOutputRange
  * @param  VCORange This parameter can be one of the following values:
  *         @arg @ref LL_RCC_PLLVCORANGE_WIDE
  *         @arg @ref LL_RCC_PLLVCORANGE_MEDIUM
  * @retval None
  */
__STATIC_INLINE void LL_RCC_PLL3_SetVCOOutputRange(uint32_t VCORange)
{
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3VCOSEL, VCORange << RCC_PLL3CFGR_PLL3VCOSEL_Pos);
}

/**
  * @}
  */
#endif /* RCC_CR_PLL3ON */

/** @defgroup RCC_LL_EF_PRIV Privileged mode
  * @{
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Enable Secure Privileged mode
  * @rmtoll PRIVCFGR       SPRIV         LL_RCC_EnableSecPrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableSecPrivilegedMode(void)
{
  SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_SPRIV);
}

/**
  * @brief  Disable Secure Privileged mode
  * @rmtoll PRIVCFGR           SPRIV          LL_RCC_DisableSecPrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableSecPrivilegedMode(void)
{
  CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_SPRIV);
}

#endif /* __ARM_FEATURE_CMSE && (__ARM_FEATURE_CMSE == 3U) */

#if defined(RCC_PRIVCFGR_NSPRIV)
/**
  * @brief  Enable Non Secure Privileged mode
  * @rmtoll PRIVCFGR       NSPRIV        LL_RCC_EnableNSecPrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableNSecPrivilegedMode(void)
{
  SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
}

/**
  * @brief  Disable Non Secure Privileged mode
  * @rmtoll PRIVCFGR           NSPRIV          LL_RCC_DisableNSecPrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableNSecPrivilegedMode(void)
{
  CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
}

/**
  * @brief  Check if Secure Privileged mode has been enabled or not
  * @rmtoll PRIVCFGR           SPRIV          LL_RCC_IsEnabledSecPrivilegedMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledSecPrivilegedMode(void)
{
  return ((READ_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_SPRIV) == RCC_PRIVCFGR_SPRIV) ? 1UL : 0UL);
}

/**
  * @brief  Check if Non Secure Privileged mode has been enabled or not
  * @rmtoll PRIVCFGR           NSPRIV          LL_RCC_IsEnabledNSecPrivilegedMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledNSecPrivilegedMode(void)
{
  return ((READ_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV) == RCC_PRIVCFGR_NSPRIV) ? 1UL : 0UL);
}

#else
/**
  * @brief  Enable Privileged mode
  * @rmtoll PRIVCFGR       PRIV        LL_RCC_EnablePrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnablePrivilegedMode(void)
{
  SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_PRIV);
}

/**
  * @brief  Disable Privileged mode
  * @rmtoll PRIVCFGR           PRIV          LL_RCC_DisablePrivilegedMode
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisablePrivilegedMode(void)
{
  CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_PRIV);
}

/**
  * @brief  Check if Privileged mode has been enabled or not
  * @rmtoll PRIVCFGR           PRIV          LL_RCC_IsEnabledPrivilegedMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledPrivilegedMode(void)
{
  return ((READ_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_PRIV) == RCC_PRIVCFGR_PRIV) ? 1UL : 0UL);
}

#endif /* RCC_PRIVCFGR_NSPRIV */

/**
  * @}
  */

/** @defgroup RCC_LL_EF_FLAG_Management FLAG Management
  * @{
  */

/**
  * @brief  Clear LSI ready interrupt flag
  * @rmtoll CICR         LSIRDYC       LL_RCC_ClearFlag_LSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_LSIRDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_LSIRDYC);
}

/**
  * @brief  Clear LSE ready interrupt flag
  * @rmtoll CICR         LSERDYC       LL_RCC_ClearFlag_LSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_LSERDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_LSERDYC);
}

/**
  * @brief  Clear CSI ready interrupt flag
  * @rmtoll CICR         CSIRDYC       LL_RCC_ClearFlag_CSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_CSIRDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_CSIRDYC);
}

/**
  * @brief  Clear HSI ready interrupt flag
  * @rmtoll CICR         HSIRDYC       LL_RCC_ClearFlag_HSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_HSIRDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_HSIRDYC);
}

/**
  * @brief  Clear HSE ready interrupt flag
  * @rmtoll CICR         HSERDYC       LL_RCC_ClearFlag_HSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_HSERDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_HSERDYC);
}


/**
  * @brief  Clear HSI48 ready interrupt flag
  * @rmtoll CICR          HSI48RDYC     LL_RCC_ClearFlag_HSI48RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_HSI48RDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_HSI48RDYC);
}

/**
  * @brief  Clear PLL1 ready interrupt flag
  * @rmtoll CICR         PLL1RDYC       LL_RCC_ClearFlag_PLL1RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_PLL1RDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_PLL1RDYC);
}

/**
  * @brief  Clear PLL2 ready interrupt flag
  * @rmtoll CICR         PLL2RDYC       LL_RCC_ClearFlag_PLL2RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_PLL2RDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_PLL2RDYC);
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Clear PLL3 ready interrupt flag
  * @rmtoll CICR         PLL3RDYC       LL_RCC_ClearFlag_PLL3RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_PLL3RDY(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_PLL3RDYC);
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Clear Clock security system interrupt flag
  * @rmtoll CICR         HSECSSC          LL_RCC_ClearFlag_HSECSS
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearFlag_HSECSS(void)
{
  SET_BIT(RCC->CICR, RCC_CICR_HSECSSC);
}

/**
  * @brief  Check if LSI ready interrupt occurred or not
  * @rmtoll CIFR         LSIRDYF       LL_RCC_IsActiveFlag_LSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_LSIRDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_LSIRDYF) == RCC_CIFR_LSIRDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if LSE ready interrupt occurred or not
  * @rmtoll CIFR         LSERDYF       LL_RCC_IsActiveFlag_LSERDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_LSERDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_LSERDYF) == RCC_CIFR_LSERDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if CSI ready interrupt occurred or not
  * @rmtoll CIFR         CSIRDYF       LL_RCC_IsActiveFlag_CSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_CSIRDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_CSIRDYF) == RCC_CIFR_CSIRDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if HSI ready interrupt occurred or not
  * @rmtoll CIFR         HSIRDYF       LL_RCC_IsActiveFlag_HSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_HSIRDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_HSIRDYF) == RCC_CIFR_HSIRDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if HSE ready interrupt occurred or not
  * @rmtoll CIFR         HSERDYF       LL_RCC_IsActiveFlag_HSERDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_HSERDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_HSERDYF) == RCC_CIFR_HSERDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if HSI48 ready interrupt occurred or not
  * @rmtoll CIFR         HSI48RDYF     LL_RCC_IsActiveFlag_HSI48RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_HSI48RDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_HSI48RDYF) == RCC_CIFR_HSI48RDYF) ? 1UL : 0UL);
}
/**
  * @brief  Check if PLL1 ready interrupt occurred or not
  * @rmtoll CIFR         PLL1RDYF       LL_RCC_IsActiveFlag_PLL1RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PLL1RDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_PLL1RDYF) == RCC_CIFR_PLL1RDYF) ? 1UL : 0UL);
}

/**
  * @brief  Check if PLL2 ready interrupt occurred or not
  * @rmtoll CIFR         PLL2RDYF       LL_RCC_IsActiveFlag_PLL2RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PLL2RDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_PLL2RDYF) == RCC_CIFR_PLL2RDYF) ? 1UL : 0UL);
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Check if PLL3 ready interrupt occurred or not
  * @rmtoll CIFR         PLL3RDYF       LL_RCC_IsActiveFlag_PLL3RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PLL3RDY(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_PLL3RDYF) == RCC_CIFR_PLL3RDYF) ? 1UL : 0UL);
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Check if Clock security system interrupt occurred or not
  * @rmtoll CIFR         HSECSSF          LL_RCC_IsActiveFlag_HSECSS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_HSECSS(void)
{
  return ((READ_BIT(RCC->CIFR, RCC_CIFR_HSECSSF) == RCC_CIFR_HSECSSF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag Independent Watchdog reset is set or not.
  * @rmtoll RSR          IWDGRSTF      LL_RCC_IsActiveFlag_IWDGRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_IWDGRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_IWDGRSTF) == RCC_RSR_IWDGRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag Low Power reset is set or not.
  * @rmtoll RSR          LPWRRSTF      LL_RCC_IsActiveFlag_LPWRRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_LPWRRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_LPWRRSTF) == RCC_RSR_LPWRRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag Pin reset is set or not.
  * @rmtoll RSR          PINRSTF       LL_RCC_IsActiveFlag_PINRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_PINRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_PINRSTF) == RCC_RSR_PINRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag Software reset is set or not.
  * @rmtoll RSR          SFTRSTF       LL_RCC_IsActiveFlag_SFTRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_SFTRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_SFTRSTF) == RCC_RSR_SFTRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag Window Watchdog reset is set or not.
  * @rmtoll RSR          WWDGRSTF      LL_RCC_IsActiveFlag_WWDGRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_WWDGRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_WWDGRSTF) == RCC_RSR_WWDGRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Check if RCC flag BOR reset is set or not.
  * @rmtoll RSR          BORRSTF       LL_RCC_IsActiveFlag_BORRST
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsActiveFlag_BORRST(void)
{
  return ((READ_BIT(RCC->RSR, RCC_RSR_BORRSTF) == RCC_RSR_BORRSTF) ? 1UL : 0UL);
}

/**
  * @brief  Set RMVF bit to clear the reset flags.
  * @rmtoll RSR          RMVF          LL_RCC_ClearResetFlags
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ClearResetFlags(void)
{
  SET_BIT(RCC->RSR, RCC_RSR_RMVF);
}

/**
  * @}
  */

/** @defgroup RCC_LL_EF_IT_Management IT Management
  * @{
  */

/**
  * @brief  Enable LSI ready interrupt
  * @rmtoll CIER         LSIRDYIE      LL_RCC_EnableIT_LSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_LSIRDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_LSIRDYIE);
}

/**
  * @brief  Enable LSE ready interrupt
  * @rmtoll CIER         LSERDYIE      LL_RCC_EnableIT_LSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_LSERDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_LSERDYIE);
}

/**
  * @brief  Enable CSI ready interrupt
  * @rmtoll CIER         CSIRDYIE      LL_RCC_EnableIT_CSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_CSIRDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_CSIRDYIE);
}

/**
  * @brief  Enable HSI ready interrupt
  * @rmtoll CIER         HSIRDYIE      LL_RCC_EnableIT_HSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_HSIRDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_HSIRDYIE);
}

/**
  * @brief  Enable HSE ready interrupt
  * @rmtoll CIER         HSERDYIE      LL_RCC_EnableIT_HSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_HSERDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_HSERDYIE);
}

/**
  * @brief  Enable HSI48 ready interrupt
  * @rmtoll CIER          HSI48RDYIE    LL_RCC_EnableIT_HSI48RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_HSI48RDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_HSI48RDYIE);
}

/**
  * @brief  Enable PLL1 ready interrupt
  * @rmtoll CIER         PLL1RDYIE      LL_RCC_EnableIT_PLL1RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_PLL1RDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_PLL1RDYIE);
}

/**
  * @brief  Enable PLL2 ready interrupt
  * @rmtoll CIER         PLL2RDYIE      LL_RCC_EnableIT_PLL2RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_PLL2RDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_PLL2RDYIE);
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Enable PLL3 ready interrupt
  * @rmtoll CIER         PLL3RDYIE      LL_RCC_EnableIT_PLL3RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_EnableIT_PLL3RDY(void)
{
  SET_BIT(RCC->CIER, RCC_CIER_PLL3RDYIE);
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Disable LSI ready interrupt
  * @rmtoll CIER         LSIRDYIE      LL_RCC_DisableIT_LSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_LSIRDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_LSIRDYIE);
}

/**
  * @brief  Disable LSE ready interrupt
  * @rmtoll CIER         LSERDYIE      LL_RCC_DisableIT_LSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_LSERDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_LSERDYIE);
}

/**
  * @brief  Disable CSI ready interrupt
  * @rmtoll CIER         CSIRDYIE      LL_RCC_DisableIT_CSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_CSIRDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_CSIRDYIE);
}

/**
  * @brief  Disable HSI ready interrupt
  * @rmtoll CIER         HSIRDYIE      LL_RCC_DisableIT_HSIRDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_HSIRDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_HSIRDYIE);
}

/**
  * @brief  Disable HSE ready interrupt
  * @rmtoll CIER         HSERDYIE      LL_RCC_DisableIT_HSERDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_HSERDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_HSERDYIE);
}

/**
  * @brief  Disable HSI48 ready interrupt
  * @rmtoll CIER          HSI48RDYIE    LL_RCC_DisableIT_HSI48RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_HSI48RDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_HSI48RDYIE);
}

/**
  * @brief  Disable PLL1 ready interrupt
  * @rmtoll CIER         PLL1RDYIE      LL_RCC_DisableIT_PLL1RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_PLL1RDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_PLL1RDYIE);
}

/**
  * @brief  Disable PLL2 ready interrupt
  * @rmtoll CIER         PLL2RDYIE      LL_RCC_DisableIT_PLL2RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_PLL2RDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_PLL2RDYIE);
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Disable PLL3 ready interrupt
  * @rmtoll CIER         PLL3RDYIE      LL_RCC_DisableIT_PLL3RDY
  * @retval None
  */
__STATIC_INLINE void LL_RCC_DisableIT_PLL3RDY(void)
{
  CLEAR_BIT(RCC->CIER, RCC_CIER_PLL3RDYIE);
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Checks if LSI ready interrupt source is enabled or disabled.
  * @rmtoll CIER         LSIRDYIE      LL_RCC_IsEnabledIT_LSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_LSIRDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_LSIRDYIE) == RCC_CIER_LSIRDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if LSE ready interrupt source is enabled or disabled.
  * @rmtoll CIER         LSERDYIE      LL_RCC_IsEnabledIT_LSERDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_LSERDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_LSERDYIE) == RCC_CIER_LSERDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if CSI ready interrupt source is enabled or disabled.
  * @rmtoll CIER         CSIRDYIE      LL_RCC_IsEnabledIT_CSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_CSIRDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_CSIRDYIE) == RCC_CIER_CSIRDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if HSI ready interrupt source is enabled or disabled.
  * @rmtoll CIER         HSIRDYIE      LL_RCC_IsEnabledIT_HSIRDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_HSIRDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_HSIRDYIE) == RCC_CIER_HSIRDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if HSE ready interrupt source is enabled or disabled.
  * @rmtoll CIER         HSERDYIE      LL_RCC_IsEnabledIT_HSERDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_HSERDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_HSERDYIE) == RCC_CIER_HSERDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if HSI48 ready interrupt source is enabled or disabled.
  * @rmtoll CIER          HSI48RDYIE    LL_RCC_IsEnabledIT_HSI48RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_HSI48RDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_HSI48RDYIE) == RCC_CIER_HSI48RDYIE) ? 1UL : 0UL);
}
/**
  * @brief  Checks if PLL1 ready interrupt source is enabled or disabled.
  * @rmtoll CIER         PLL1RDYIE      LL_RCC_IsEnabledIT_PLL1RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_PLL1RDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_PLL1RDYIE) == RCC_CIER_PLL1RDYIE) ? 1UL : 0UL);
}

/**
  * @brief  Checks if PLL2 ready interrupt source is enabled or disabled.
  * @rmtoll CIER         PLL2RDYIE      LL_RCC_IsEnabledIT_PLL2RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_PLL2RDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_PLL2RDYIE) == RCC_CIER_PLL2RDYIE) ? 1UL : 0UL);
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Checks if PLL3 ready interrupt source is enabled or disabled.
  * @rmtoll CIER         PLL3RDYIE      LL_RCC_IsEnabledIT_PLL3RDY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_RCC_IsEnabledIT_PLL3RDY(void)
{
  return ((READ_BIT(RCC->CIER, RCC_CIER_PLL3RDYIE) == RCC_CIER_PLL3RDYIE) ? 1UL : 0UL);
}
#endif /* RCC_CR_PLL3ON */

/**
  * @}
  */

/** @defgroup RCC_LL_EF_Security_Services Security Services
  * @{
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure RCC resources security
  * @note Only available from secure state when system implements security (TZEN=1)
  * @rmtoll SECCFGR     HSISEC        LL_RCC_ConfigSecure\n
  *         SECCFGR     HSESEC        LL_RCC_ConfigSecure\n
  *         SECCFGR     CSISEC        LL_RCC_ConfigSecure\n
  *         SECCFGR     LSISEC        LL_RCC_ConfigSecure\n
  *         SECCFGR     LSESEC        LL_RCC_ConfigSecure\n
  *         SECCFGR     SYSCLKSEC     LL_RCC_ConfigSecure\n
  *         SECCFGR     PRESCSEC      LL_RCC_ConfigSecure\n
  *         SECCFGR     PLL1SEC       LL_RCC_ConfigSecure\n
  *         SECCFGR     PLL2SEC       LL_RCC_ConfigSecure\n
  *         SECCFGR     PLL3SEC       LL_RCC_ConfigSecure\n
  *         SECCFGR     HSI48SEC      LL_RCC_ConfigSecure\n
  *         SECCFGR     RMVFSEC       LL_RCC_ConfigSecure\n
  *         SECCFGR     CKPERSELSEC   LL_RCC_ConfigSecure
  * @param  Configuration This parameter shall be the full combination of the following values:
  *         @arg @ref LL_RCC_ALL_SEC or LL_RCC_ALL_NSEC
  *         @arg @ref LL_RCC_HSI_SEC or LL_RCC_HSI_NSEC
  *         @arg @ref LL_RCC_HSE_SEC or LL_RCC_HSE_NSEC
  *         @arg @ref LL_RCC_CSI_SEC or LL_RCC_CSI_NSEC
  *         @arg @ref LL_RCC_LSE_SEC or LL_RCC_LSE_NSEC
  *         @arg @ref LL_RCC_LSI_SEC or LL_RCC_LSI_NSEC
  *         @arg @ref LL_RCC_SYSCLK_SEC or LL_RCC_SYSCLK_NSEC
  *         @arg @ref LL_RCC_PRESCALERS_SEC or LL_RCC_PRESCALERS_NSEC
  *         @arg @ref LL_RCC_PLL1_SEC or LL_RCC_PLL1_NSEC
  *         @arg @ref LL_RCC_PLL2_SEC or LL_RCC_PLL2_NSEC
  *         @arg @ref LL_RCC_PLL3_SEC or LL_RCC_PLL3_NSEC
  *         @arg @ref LL_RCC_HSI48_SEC or LL_RCC_HSI48_NSEC
  *         @arg @ref LL_RCC_RESET_FLAGS_SEC or LL_RCC_RESET_FLAGS_NSEC
  *         @arg @ref LL_RCC_CKPERSEL_SEC or LL_RCC_CKPERSEL_NSEC
  * @retval None
  */
__STATIC_INLINE void LL_RCC_ConfigSecure(uint32_t Configuration)
{
  WRITE_REG(RCC->SECCFGR, Configuration);
}
#endif /* __ARM_FEATURE_CMSE && (__ARM_FEATURE_CMSE == 3U) */

#if defined(RCC_SECCFGR_HSISEC)
/**
  * @brief  Get RCC resources security status
  * @note Only available from secure state when system implements security (TZEN=1)
  * @rmtoll SECCFGR     HSISEC        LL_RCC_GetConfigSecure\n
  *         SECCFGR     HSESEC        LL_RCC_GetConfigSecure\n
  *         SECCFGR     CSISEC        LL_RCC_GetConfigSecure\n
  *         SECCFGR     LSISEC        LL_RCC_GetConfigSecure\n
  *         SECCFGR     LSESEC        LL_RCC_GetConfigSecure\n
  *         SECCFGR     SYSCLKSEC     LL_RCC_GetConfigSecure\n
  *         SECCFGR     PRESCSEC      LL_RCC_GetConfigSecure\n
  *         SECCFGR     PLL1SEC       LL_RCC_GetConfigSecure\n
  *         SECCFGR     PLL2SEC       LL_RCC_GetConfigSecure\n
  *         SECCFGR     PLL3SEC       LL_RCC_GetConfigSecure\n
  *         SECCFGR     HSI48SEC      LL_RCC_GetConfigSecure\n
  *         SECCFGR     RMVFSEC       LL_RCC_GetConfigSecure\n
  *         SECCFGR     CKPERSELSEC   LL_RCC_GetConfigSecure
  * @retval Returned value is the combination of the following values:
  *         @arg @ref LL_RCC_ALL_SEC or LL_RCC_ALL_NSEC
  *         @arg @ref LL_RCC_HSI_SEC or LL_RCC_HSI_NSEC
  *         @arg @ref LL_RCC_HSE_SEC or LL_RCC_HSE_NSEC
  *         @arg @ref LL_RCC_CSI_SEC or LL_RCC_CSI_NSEC
  *         @arg @ref LL_RCC_LSE_SEC or LL_RCC_LSE_NSEC
  *         @arg @ref LL_RCC_LSI_SEC or LL_RCC_LSI_NSEC
  *         @arg @ref LL_RCC_SYSCLK_SEC or LL_RCC_SYSCLK_NSEC
  *         @arg @ref LL_RCC_PRESCALERS_SEC or LL_RCC_PRESCALERS_NSEC
  *         @arg @ref LL_RCC_PLL1_SEC or LL_RCC_PLL1_NSEC
  *         @arg @ref LL_RCC_PLL2_SEC or LL_RCC_PLL2_NSEC
  *         @arg @ref LL_RCC_PLL3_SEC or LL_RCC_PLL3_NSEC
  *         @arg @ref LL_RCC_HSI48_SEC or LL_RCC_HSI48_NSEC
  *         @arg @ref LL_RCC_RESET_FLAGS_SEC or LL_RCC_RESET_FLAGS_NSEC
  *         @arg @ref LL_RCC_CKPERSEL_SEC or LL_RCC_CKPERSEL_NSEC
  * @retval None
  */
__STATIC_INLINE uint32_t LL_RCC_GetConfigSecure(void)
{
  return (uint32_t)(READ_BIT(RCC->SECCFGR, RCC_SECURE_MASK));
}
#endif /* RCC_SECCFGR_HSISEC */

/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup RCC_LL_EF_Init De-initialization function
  * @{
  */
ErrorStatus LL_RCC_DeInit(void);
/**
  * @}
  */

/** @defgroup RCC_LL_EF_Get_Freq Get system and peripherals clocks frequency functions
  * @{
  */

uint32_t    LL_RCC_CalcPLLClockFreq(uint32_t PLLInputFreq, uint32_t M, uint32_t N, uint32_t FRACN, uint32_t PQR);

void        LL_RCC_GetPLL1ClockFreq(LL_PLL_ClocksTypeDef *pPLL_Clocks);
void        LL_RCC_GetPLL2ClockFreq(LL_PLL_ClocksTypeDef *pPLL_Clocks);
#if defined(RCC_CR_PLL3ON)
void        LL_RCC_GetPLL3ClockFreq(LL_PLL_ClocksTypeDef *pPLL_Clocks);
#endif /* PLL3 */
void        LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *pRCC_Clocks);
uint32_t    LL_RCC_GetUSARTClockFreq(uint32_t USARTxSource);
#if defined(UART4)
uint32_t    LL_RCC_GetUARTClockFreq(uint32_t UARTxSource);
#endif /* UART4 */
uint32_t    LL_RCC_GetI2CClockFreq(uint32_t I2CxSource);
uint32_t    LL_RCC_GetI3CClockFreq(uint32_t I3CxSource);
uint32_t    LL_RCC_GetLPUARTClockFreq(uint32_t LPUARTxSource);
uint32_t    LL_RCC_GetLPTIMClockFreq(uint32_t LPTIMxSource);
uint32_t    LL_RCC_GetFDCANClockFreq(uint32_t FDCANxSource);
#if defined (SAI1)
uint32_t    LL_RCC_GetSAIClockFreq(uint32_t SAIxSource);
#endif /* SAI1 */
#if defined(SDMMC1)
uint32_t    LL_RCC_GetSDMMCClockFreq(uint32_t SDMMCxSource);
#endif /* SDMMC1 */
uint32_t    LL_RCC_GetRNGClockFreq(uint32_t RNGxSource);
#if defined(USB_DRD_FS)
uint32_t    LL_RCC_GetUSBClockFreq(uint32_t USBxSource);
#endif /* USB_DRD_FS */
uint32_t    LL_RCC_GetADCDACClockFreq(uint32_t ADCDACxSource);
uint32_t    LL_RCC_GetDACLPClockFreq(uint32_t DACLPxSource);
#if defined(OCTOSPI1)
uint32_t    LL_RCC_GetOCTOSPIClockFreq(uint32_t OCTOSPIxSource);
#endif /* OCTOSPI1 */
uint32_t    LL_RCC_GetSPIClockFreq(uint32_t SPIxSource);
#if defined(CEC)
uint32_t    LL_RCC_GetCECClockFreq(uint32_t CECxSource);
#endif /* CEC */
#if defined(PLAY1)
uint32_t    LL_RCC_GetPLAY1ClockFreq(uint32_t PLAYxSource);
#endif /* PLAY1 */
#if defined(USB_OTG_FS)
uint32_t    LL_RCC_GetOTGFSClockFreq(uint32_t OTGFSxSource);
#endif /* USB_OTG_FS */
#if defined(USB_OTG_HS)
uint32_t    LL_RCC_GetOTGHSClockFreq(uint32_t OTGHSxSource);
#endif /* USB_OTG_HS */
#if defined(OCTOSPI2)
uint32_t    LL_RCC_GetOCTOSPI2ClockFreq(uint32_t OCTOSPIxSource);
#endif /* OCTOSPI2 */
#if defined(LTDC)
uint32_t    LL_RCC_GetLTDCClockFreq(uint32_t LTDCxSource);
#endif /* LTDC */
#if defined(ADF1)
uint32_t    LL_RCC_GetADF1ClockFreq(uint32_t ADFxSource);
#endif /* ADF1 */
#if defined(MDF1)
uint32_t    LL_RCC_GetMDF1ClockFreq(uint32_t MDFxSource);
#endif /* MDF1 */
#if defined(RCC_CCIPR4_ETHCLKSEL)
uint32_t    LL_RCC_GetETHClockFreq(uint32_t ETHxSource);
#endif /* RCC_CCIPR4_ETHCLKSEL */
#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
uint32_t    LL_RCC_GetETHPTPClockFreq(uint32_t ETHPTPxSource);
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */
#if defined(RCC_CCIPR5_ETHREFCLKSEL)
uint32_t    LL_RCC_GetETHREFClockFreq(uint32_t ETHREFxSource);
#endif /* RCC_CCIPR5_ETHREFCLKSEL */
uint32_t    LL_RCC_GetCLKPClockFreq(uint32_t CLKPxSource);
/**
  * @}
  */

#endif /* USE_FULL_LL_DRIVER */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined(RCC) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_LL_RCC_H */

