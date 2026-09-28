/**
  ******************************************************************************
  * @file    stm32h5xx_ll_utils.h
  * @author  MCD Application Team
  * @brief   Header file of UTILS LL module.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The LL UTILS driver contains a set of generic APIs that can be
    used by user:
      (+) Device electronic signature
      (+) Timing functions
      (+) PLL configuration functions

  @endverbatim
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
#ifndef __STM32H5xx_LL_UTILS_H
#define __STM32H5xx_LL_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

/** @defgroup UTILS_LL UTILS
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/** @defgroup UTILS_LL_Private_Constants UTILS Private Constants
  * @{
  */

/* Max delay can be used in LL_mDelay */
#define LL_MAX_DELAY                  0xFFFFFFFFU

/**
  * @brief Unique device ID register base address
  */
#define UID_BASE_ADDRESS              UID_BASE

/**
  * @brief Flash size data register base address
  */
#define FLASHSIZE_BASE_ADDRESS        FLASHSIZE_BASE

/**
  * @brief Package data register base address
  */
#define PACKAGE_BASE_ADDRESS          PACKAGE_BASE

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup UTILS_LL_Private_Macros UTILS Private Macros
  * @{
  */
/**
  * @}
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup UTILS_LL_ES_INIT UTILS Exported structures
  * @{
  */

/**
  * @brief  UTILS PLL structure definition
  */
typedef struct
{
  uint32_t PLLM;   /*!< Division factor for PLL VCO input clock.
                        This parameter must be a number between Min_Data = 1 and Max_Data = 63

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL1_SetM(). */

  uint32_t PLLN;   /*!< Multiplication factor for PLL VCO output clock.
                        This parameter must be a number between Min_Data = 4 and Max_Data = 512

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL1_SetN(). */

  uint32_t PLLP;   /*!< Division for the main system clock.
                        This parameter must be a number between Min_Data = 2 and Max_Data = 128
                          odd division factors are not allowed

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL1_SetP(). */

  uint32_t FRACN;  /*!< Fractional part of the multiplication factor for PLL VCO.
                        This parameter can be a value between 0 and 8191

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL1_SetFRACN(). */

  uint32_t VCO_Input;  /*!< PLL clock Input range.
                        This parameter can be a value of @ref RCC_LL_EC_PLLINPUTRANGE

                        This feature can be modified afterwards using unitary function
                        @ref LL_RCC_PLL1_SetVCOInputRange(). */

  uint32_t VCO_Output;  /*!< PLL clock Output range.
                        This parameter can be a value of @ref RCC_LL_EC_PLLOUTPUTRANGE

                      This feature can be modified afterwards using unitary function
                      @ref LL_RCC_PLL1_SetVCOOutputRange(). */

} LL_UTILS_PLLInitTypeDef;

/**
  * @brief  UTILS System, AHB and APB buses clock configuration structure definition
  */
typedef struct
{
  uint32_t SYSCLKDivider;         /*!< The System clock (SYSCLK) divider. This clock is derived from the System clock.
                                       This parameter can be a value of @ref RCC_LL_EC_SYSCLK_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAHBPrescaler(). */

  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_APB1_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAPB1Prescaler(). */

  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_APB2_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAPB2Prescaler(). */

  uint32_t APB3CLKDivider;        /*!< The APB3 clock (PCLK3) divider. This clock is derived from the AHB clock (HCLK).
                                       This parameter can be a value of @ref RCC_LL_EC_APB3_DIV

                                       This feature can be modified afterwards using unitary function
                                       @ref LL_RCC_SetAPB3Prescaler(). */

} LL_UTILS_ClkInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup UTILS_LL_Exported_Constants UTILS Exported Constants
  * @{
  */

/** @defgroup UTILS_EC_HSE_BYPASS HSE Bypass activation
  * @{
  */
#define LL_UTILS_HSEBYPASS_OFF          0x00000000U       /*!< HSE Bypass is not enabled                */
#define LL_UTILS_HSEBYPASS_ON           0x00000001U       /*!< HSE Bypass Analog is enabled             */
#define LL_UTILS_HSEBYPASS_DIGITAL_ON   0x00000002U       /*!< HSE Bypass Digital is enabled            */
/**
  * @}
  */

/** @defgroup UTILS_EC_PACKAGETYPE PACKAGE TYPE
  * @{
  */
#define LL_UTILS_PACKAGETYPE_LQFP64                  0x00000000U /*!< LQFP64 package type                                 */
#define LL_UTILS_PACKAGETYPE_VFQFPN68                0x00000001U /*!< VFQFPN68 package type                               */
#define LL_UTILS_PACKAGETYPE_LQFP100                 0x00000002U /*!< LQFP100 package type                                */
#define LL_UTILS_PACKAGETYPE_UFBGA176                0x00000003U /*!< UFBGA176+25 package type                            */
#define LL_UTILS_PACKAGETYPE_LQFP144                 0x00000004U /*!< LQFP144 package type                                */
#define LL_UTILS_PACKAGETYPE_LQFP48                  0x00000005U /*!< LQFP48 package type                                 */
#define LL_UTILS_PACKAGETYPE_UFBGA169                0x00000006U /*!< UFBGA169 package type                               */
#define LL_UTILS_PACKAGETYPE_LQFP176                 0x00000007U /*!< LQFP176 package type                                */
#define LL_UTILS_PACKAGETYPE_UFQFPN32                0x00000009U /*!< UFQFPN32 package type                               */
#define LL_UTILS_PACKAGETYPE_LQFP100_SMPS            0x0000000AU /*!< LQFP100 with internal SMPS package type             */
#define LL_UTILS_PACKAGETYPE_UFBGA176_SMPS           0x0000000BU /*!< UFBGA176+25 with internal SMPS package type         */
#define LL_UTILS_PACKAGETYPE_LQFP144_SMPS            0x0000000CU /*!< LQFP144 with internal SMPS package type             */
#define LL_UTILS_PACKAGETYPE_LQFP176_SMPS            0x0000000DU /*!< LQFP176 with internal SMPS package type             */
#define LL_UTILS_PACKAGETYPE_UFBGA169_SMPS           0x0000000EU /*!< UFBGA169 with internal SMPS package type            */
#define LL_UTILS_PACKAGETYPE_WLCSP25                 0x0000000FU /*!< WLCSP25 package type                                */
#define LL_UTILS_PACKAGETYPE_UFQFPN48                0x00000010U /*!< UFQFPN48 package type                               */
#define LL_UTILS_PACKAGETYPE_WLCSP39                 0x00000011U /*!< WLCSP39 package type                                */
#define LL_UTILS_PACKAGETYPE_UFBGA100                0x00000014U /*!< UFBGA100 package type                               */
#define LL_UTILS_PACKAGETYPE_UFBGA144                0x00000015U /*!< UFBGA144 package type                               */
#define LL_UTILS_PACKAGETYPE_UFBGA144_SMPS           0x00000016U /*!< UFBGA144 with internal SMPS package type            */
#define LL_UTILS_PACKAGETYPE_WLCSP100_SMPS_USBPHY    0x00000018U /*!< WLCSP100 with internal SMPS USBPHY package type     */
#define LL_UTILS_PACKAGETYPE_UFBGA176_25_SMPS_USBPHY 0x00000019U /*!< UFBGA176+25 with internal SMPS USBPHY package type  */
#define LL_UTILS_PACKAGETYPE_LQFP176_SMPS_USBPHY     0x0000001AU /*!< LQFP176 with internal SMPS USBPHY package type      */
#define LL_UTILS_PACKAGETYPE_TFBGA255_SMPS_USBPHY    0x0000001BU /*!< TFBGA1255 with internal SMPS USBPHY package type    */
#define LL_UTILS_PACKAGETYPE_LQFP144_USBPHY          0x0000001CU /*!< LQFP144 with USBPHY package type                    */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup UTILS_LL_Exported_Functions UTILS Exported Functions
  * @{
  */

/** @defgroup UTILS_EF_DEVICE_ELECTRONIC_SIGNATURE DEVICE ELECTRONIC SIGNATURE
  * @{
  */

/**
  * @brief  Get Word0 of the unique device identifier (UID based on 96 bits)
  * @retval UID[31:0]: X and Y coordinates on the wafer expressed in BCD format
  */
__STATIC_INLINE uint32_t LL_GetUID_Word0(void)
{
  return (uint32_t)(READ_REG(*((uint32_t *)UID_BASE_ADDRESS)));
}

/**
  * @brief  Get Word1 of the unique device identifier (UID based on 96 bits)
  * @retval UID[63:32]: Wafer number (UID[39:32]) & LOT_NUM[23:0] (UID[63:40])
  */
__STATIC_INLINE uint32_t LL_GetUID_Word1(void)
{
  return (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 4U))));
}

/**
  * @brief  Get Word2 of the unique device identifier (UID based on 96 bits)
  * @retval UID[95:64]: Lot number (ASCII encoded) - LOT_NUM[55:24]
  */
__STATIC_INLINE uint32_t LL_GetUID_Word2(void)
{
  return (uint32_t)(READ_REG(*((uint32_t *)(UID_BASE_ADDRESS + 8U))));
}

/**
  * @brief  Get Flash memory size
  * @note   This bitfield indicates the size of the device Flash memory expressed in
  *         Kbytes. As an example, 0x040 corresponds to 64 Kbytes.
  * @retval FLASH_SIZE[15:0]: Flash memory size
  */
__STATIC_INLINE uint32_t LL_GetFlashSize(void)
{
  return (uint32_t)(READ_REG(*((uint32_t *)FLASHSIZE_BASE_ADDRESS)) & 0xFFFFU);
}

/**
  * @brief  Get Package type
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP64
  *         @arg @ref LL_UTILS_PACKAGETYPE_VFQFPN68
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP100
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA176
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP144
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP48
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA169
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP176
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFQFPN32
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP100_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA176_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP144_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP176_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA169_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_WLCSP25
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFQFPN48
  *         @arg @ref LL_UTILS_PACKAGETYPE_WLCSP39
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA100
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA144
  *         @arg @ref LL_UTILS_PACKAGETYPE_UFBGA144_SMPS
  *         @arg @ref LL_UTILS_PACKAGETYPE_WLCSP100_SMPS_USBPHY
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP176_SMPS_USBPHY
  *         @arg @ref LL_UTILS_PACKAGETYPE_TFBGA255_SMPS_USBPHY
  *         @arg @ref LL_UTILS_PACKAGETYPE_LQFP144_USBPHY
  * @note   Refer to product datasheet for availability of package on a specific device
  */
__STATIC_INLINE uint32_t LL_GetPackageType(void)
{
  return (uint32_t)(READ_REG(*((uint16_t *)PACKAGE_BASE_ADDRESS)));
}

/**
  * @}
  */

/** @defgroup UTILS_LL_EF_DELAY DELAY
  * @{
  */

/**
  * @brief  This function configures the Cortex-M SysTick source of the time base.
  * @param  HCLKFrequency HCLK frequency in Hz (can be calculated thanks to RCC helper macro)
  * @note   When a RTOS is used, it is recommended to avoid changing the SysTick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * @param  Ticks Number of ticks
  * @retval None
  */
__STATIC_INLINE void LL_InitTick(uint32_t HCLKFrequency, uint32_t Ticks)
{
  /* Configure the SysTick to have interrupt in 1ms time base */
  SysTick->LOAD  = (uint32_t)((HCLKFrequency / Ticks) - 1UL);  /* set reload register */
  SysTick->VAL   = 0UL;                                       /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;                   /* Enable the Systick Timer */
}

void        LL_Init1msTick(uint32_t HCLKFrequency);
void        LL_Init1msTick_HCLK_Div8(uint32_t HCLKFrequency);
void        LL_Init1msTick_LSE(void);
void        LL_Init1msTick_LSI(void);
void        LL_mDelay(uint32_t Delay);

/**
  * @}
  */

/** @defgroup UTILS_EF_SYSTEM SYSTEM
  * @{
  */

void        LL_SetSystemCoreClock(uint32_t HCLKFrequency);
ErrorStatus LL_PLL_ConfigSystemClock_CSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);
ErrorStatus LL_PLL_ConfigSystemClock_HSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);
ErrorStatus LL_PLL_ConfigSystemClock_HSE(uint32_t HSEFrequency,
                                         uint32_t HSEBypass,
                                         LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);
ErrorStatus LL_SetFlashLatency(uint32_t HCLK_Frequency);
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

#endif /* __STM32H5xx_LL_UTILS_H */

