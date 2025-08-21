/**
  ******************************************************************************
  * @file    stm32f7xx_hal_dma_ex.h
  * @author  MCD Application Team
  * @brief   Header file of DMA HAL extension module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F7xx_HAL_DMA_EX_H
#define __STM32F7xx_HAL_DMA_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal_def.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @addtogroup DMAEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/** @defgroup DMAEx_Exported_Types DMAEx Exported Types
  * @brief DMAEx Exported types
  * @{
  */
   
/** 
  * @brief  HAL DMA Memory definition  
  */ 
typedef enum
{
  MEMORY0      = 0x00U,    /*!< Memory 0     */
  MEMORY1      = 0x01U,    /*!< Memory 1     */

}HAL_DMA_MemoryTypeDef;

/**
  * @}
  */
  
/* Exported constants --------------------------------------------------------*/

/** @defgroup DMA_Exported_Constants DMA Exported Constants
  * @brief    DMA Exported constants 
  * @{
  */

/** @defgroup DMAEx_Channel_selection DMA Channel selection
  * @brief    DMAEx channel selection 
  * @{
  */ 
#define DMA_CHANNEL_0                     0x00000000U  /*!< DMA Channel 0 */
#define DMA_CHANNEL_1                     0x02000000U  /*!< DMA Channel 1 */
#define DMA_CHANNEL_2                     0x04000000U  /*!< DMA Channel 2 */
#define DMA_CHANNEL_3                     0x06000000U  /*!< DMA Channel 3 */
#define DMA_CHANNEL_4                     0x08000000U  /*!< DMA Channel 4 */
#define DMA_CHANNEL_5                     0x0A000000U  /*!< DMA Channel 5 */
#define DMA_CHANNEL_6                     0x0C000000U  /*!< DMA Channel 6 */
#define DMA_CHANNEL_7                     0x0E000000U  /*!< DMA Channel 7 */
#if defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) ||\
    defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) ||\
    defined (STM32F779xx) || defined (STM32F730xx)
#define DMA_CHANNEL_8                     0x10000000U  /*!< DMA Channel 8 */
#define DMA_CHANNEL_9                     0x12000000U  /*!< DMA Channel 9 */
#define DMA_CHANNEL_10                    0x14000000U  /*!< DMA Channel 10*/
#define DMA_CHANNEL_11                    0x16000000U  /*!< DMA Channel 11*/
#define DMA_CHANNEL_12                    0x18000000U  /*!< DMA Channel 12*/
#define DMA_CHANNEL_13                    0x1A000000U  /*!< DMA Channel 13*/
#define DMA_CHANNEL_14                    0x1C000000U  /*!< DMA Channel 14*/
#define DMA_CHANNEL_15                    0x1E000000U  /*!< DMA Channel 15*/
#endif /* STM32F722xx || STM32F723xx || STM32F732xx || STM32F733xx || STM32F765xx || STM32F767xx ||
          STM32F769xx || STM32F777xx || STM32F779xx || STM32F730xx */

/**
  * @}
  */
  
/**
  * @}
  */  

/* Exported functions --------------------------------------------------------*/
/** @defgroup DMAEx_Exported_Functions DMAEx Exported Functions
  * @brief   DMAEx Exported functions
  * @{
  */

/** @defgroup DMAEx_Exported_Functions_Group1 Extended features functions
  * @brief   Extended features functions
  * @{
  */

/* IO operation functions *******************************************************/
HAL_StatusTypeDef HAL_DMAEx_MultiBufferStart(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMAEx_MultiBufferStart_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMAEx_ChangeMemory(DMA_HandleTypeDef *hdma, uint32_t Address, HAL_DMA_MemoryTypeDef memory);

/**
  * @}
  */
/**
  * @}
  */
  
/* Private macros ------------------------------------------------------------*/
/** @defgroup DMAEx_Private_Macros DMA Private Macros
  * @brief    DMAEx private macros 
  * @{
  */
#if defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) ||\
    defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) ||\
    defined (STM32F779xx) || defined (STM32F730xx)
#define IS_DMA_CHANNEL(CHANNEL) (((CHANNEL) == DMA_CHANNEL_0)  || \
                                 ((CHANNEL) == DMA_CHANNEL_1)  || \
                                 ((CHANNEL) == DMA_CHANNEL_2)  || \
                                 ((CHANNEL) == DMA_CHANNEL_3)  || \
                                 ((CHANNEL) == DMA_CHANNEL_4)  || \
                                 ((CHANNEL) == DMA_CHANNEL_5)  || \
                                 ((CHANNEL) == DMA_CHANNEL_6)  || \
                                 ((CHANNEL) == DMA_CHANNEL_7)  || \
                                 ((CHANNEL) == DMA_CHANNEL_8)  || \
                                 ((CHANNEL) == DMA_CHANNEL_9)  || \
                                 ((CHANNEL) == DMA_CHANNEL_10) || \
                                 ((CHANNEL) == DMA_CHANNEL_11) || \
                                 ((CHANNEL) == DMA_CHANNEL_12) || \
                                 ((CHANNEL) == DMA_CHANNEL_13) || \
                                 ((CHANNEL) == DMA_CHANNEL_14) || \
                                 ((CHANNEL) == DMA_CHANNEL_15)) 
#else
#define IS_DMA_CHANNEL(CHANNEL) (((CHANNEL) == DMA_CHANNEL_0) || \
                                 ((CHANNEL) == DMA_CHANNEL_1) || \
                                 ((CHANNEL) == DMA_CHANNEL_2) || \
                                 ((CHANNEL) == DMA_CHANNEL_3) || \
                                 ((CHANNEL) == DMA_CHANNEL_4) || \
                                 ((CHANNEL) == DMA_CHANNEL_5) || \
                                 ((CHANNEL) == DMA_CHANNEL_6) || \
                                 ((CHANNEL) == DMA_CHANNEL_7))
#endif /* STM32F722xx || STM32F723xx || STM32F732xx || STM32F733xx || STM32F765xx || STM32F767xx ||
          STM32F769xx || STM32F777xx || STM32F779xx || STM32F730xx*/
/**
  * @}
  */  
         
/* Private functions ---------------------------------------------------------*/
/** @defgroup DMAEx_Private_Functions DMAEx Private Functions
  * @brief DMAEx Private functions
  * @{
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

#endif /* __STM32F7xx_HAL_DMA_H */

