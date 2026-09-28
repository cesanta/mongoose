/**
  ******************************************************************************
  * @file    stm32h5xx_hal_icache.h
  * @author  MCD Application Team
  * @brief   Header file of ICACHE HAL module.
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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef STM32H5xx_HAL_ICACHE_H
#define STM32H5xx_HAL_ICACHE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

#if defined(ICACHE)
/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup ICACHE
  * @{
  */

/* Exported types -----------------------------------------------------------*/
#if defined(ICACHE_CRRx_REN)
/** @defgroup ICACHE_Exported_Types ICACHE Exported Types
  * @{
  */

/**
  * @brief  HAL ICACHE region configuration structure definition
  */
typedef struct
{
  uint32_t BaseAddress;              /*!< Configures the Base address of Region i to be remapped */

  uint32_t RemapAddress;             /*!< Configures the Remap address of Region i to be remapped */

  uint32_t Size;                     /*!< Configures the Region size.
                                          This parameter can be a value of @ref ICACHE_Region_Size */

  uint32_t TrafficRoute;             /*!< Selects the traffic route.
                                          This parameter can be a value of @ref ICACHE_Traffic_Route */

  uint32_t OutputBurstType;          /*!< Selects the output burst type.
                                          This parameter can be a value of @ref ICACHE_Output_Burst_Type */
} ICACHE_RegionConfigTypeDef;
/**
  * @}
  */
#endif /*  ICACHE_CRRx_REN */

/* Exported constants -------------------------------------------------------*/
/** @defgroup ICACHE_Exported_Constants ICACHE Exported Constants
  * @{
  */

/** @defgroup ICACHE_WaysSelection Ways selection
  * @{
  */
#define ICACHE_1WAY                    0UL               /*!< 1-way cache (direct mapped cache) */
#define ICACHE_2WAYS                   ICACHE_CR_WAYSEL  /*!< 2-ways set associative cache (default) */
/**
  * @}
  */

/** @defgroup ICACHE_Monitor_Type Monitor type
  * @{
  */
#define ICACHE_MONITOR_HIT_MISS        (ICACHE_CR_HITMEN | ICACHE_CR_MISSMEN) /*!< Hit & Miss monitoring */
#define ICACHE_MONITOR_HIT             ICACHE_CR_HITMEN                       /*!< Hit monitoring */
#define ICACHE_MONITOR_MISS            ICACHE_CR_MISSMEN                      /*!< Miss monitoring */
/**
  * @}
  */

#if defined(ICACHE_CRRx_REN)
/** @defgroup ICACHE_Region Remapped Region number
  * @{
  */
#define ICACHE_REGION_0                0UL  /*!< Region 0 */
#define ICACHE_REGION_1                1UL  /*!< Region 1 */
#define ICACHE_REGION_2                2UL  /*!< Region 2 */
#define ICACHE_REGION_3                3UL  /*!< Region 3 */
/**
  * @}
  */

/** @defgroup ICACHE_Region_Size Remapped Region size
  * @{
  */
#define ICACHE_REGIONSIZE_2MB          1UL  /*!< Region size 2MB */
#define ICACHE_REGIONSIZE_4MB          2UL  /*!< Region size 4MB */
#define ICACHE_REGIONSIZE_8MB          3UL  /*!< Region size 8MB */
#define ICACHE_REGIONSIZE_16MB         4UL  /*!< Region size 16MB */
#define ICACHE_REGIONSIZE_32MB         5UL  /*!< Region size 32MB */
#define ICACHE_REGIONSIZE_64MB         6UL  /*!< Region size 64MB */
#define ICACHE_REGIONSIZE_128MB        7UL  /*!< Region size 128MB */
/**
  * @}
  */

/** @defgroup ICACHE_Traffic_Route Remapped Traffic route
  * @{
  */
#define ICACHE_MASTER1_PORT            0UL                 /*!< Master1 port */
#define ICACHE_MASTER2_PORT            ICACHE_CRRx_MSTSEL  /*!< Master2 port */
/**
  * @}
  */

/** @defgroup ICACHE_Output_Burst_Type Remapped Output burst type
  * @{
  */
#define ICACHE_OUTPUT_BURST_WRAP       0UL                 /*!< WRAP */
#define ICACHE_OUTPUT_BURST_INCR       ICACHE_CRRx_HBURST  /*!< INCR */
/**
  * @}
  */
#endif /*  ICACHE_CRRx_REN */

/** @defgroup ICACHE_Interrupts Interrupts
  * @{
  */
#define ICACHE_IT_BUSYEND              ICACHE_IER_BSYENDIE /*!< Busy end interrupt */
#define ICACHE_IT_ERROR                ICACHE_IER_ERRIE    /*!< Cache error interrupt */
/**
  * @}
  */

/** @defgroup ICACHE_Flags Flags
  * @{
  */
#define ICACHE_FLAG_BUSY               ICACHE_SR_BUSYF     /*!< Busy flag */
#define ICACHE_FLAG_BUSYEND            ICACHE_SR_BSYENDF   /*!< Busy end flag */
#define ICACHE_FLAG_ERROR              ICACHE_SR_ERRF      /*!< Cache error flag */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros ----------------------------------------------------------*/
/** @defgroup ICACHE_Exported_Macros ICACHE Exported Macros
  * @{
  */

/** @defgroup ICACHE_Flags_Interrupts_Management Flags and Interrupts Management
  * @brief macros to manage the specified ICACHE flags and interrupts.
  * @{
  */

/** @brief  Enable ICACHE interrupts.
  * @param  __INTERRUPT__ specifies the ICACHE interrupt sources to be enabled.
  *         This parameter can be any combination of the following values:
  *            @arg @ref ICACHE_IT_BUSYEND  Busy end interrupt
  *            @arg @ref ICACHE_IT_ERROR  Cache error interrupt
  */
#define __HAL_ICACHE_ENABLE_IT(__INTERRUPT__) SET_BIT(ICACHE->IER, (__INTERRUPT__))

/** @brief  Disable ICACHE interrupts.
  * @param  __INTERRUPT__ specifies the ICACHE interrupt sources to be disabled.
  *         This parameter can be any combination of the following values:
  *            @arg @ref ICACHE_IT_BUSYEND  Busy end interrupt
  *            @arg @ref ICACHE_IT_ERROR  Cache error interrupt
  */
#define __HAL_ICACHE_DISABLE_IT(__INTERRUPT__) CLEAR_BIT(ICACHE->IER, (__INTERRUPT__))

/** @brief  Check whether the specified ICACHE interrupt source is enabled or not.
  * @param  __INTERRUPT__ specifies the ICACHE interrupt source to check.
  *         This parameter can be any combination of the following values:
  *            @arg @ref ICACHE_IT_BUSYEND  Busy end interrupt
  *            @arg @ref ICACHE_IT_ERROR  Cache error interrupt
  * @retval The state of __INTERRUPT__ (0 or 1).
  */
#define __HAL_ICACHE_GET_IT_SOURCE(__INTERRUPT__)  \
  ((READ_BIT(ICACHE->IER, (__INTERRUPT__)) == (__INTERRUPT__)) ? 1U : 0U)

/** @brief  Check whether the selected ICACHE flag is set or not.
  * @param  __FLAG__ specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg @ref ICACHE_FLAG_BUSY  Busy flag
  *            @arg @ref ICACHE_FLAG_BUSYEND  Busy end flag
  *            @arg @ref ICACHE_FLAG_ERROR  Cache error flag
  * @retval The state of __FLAG__ (0 or 1).
  */
#define __HAL_ICACHE_GET_FLAG(__FLAG__) ((READ_BIT(ICACHE->SR, (__FLAG__)) != 0U) ? 1U : 0U)

/** @brief  Clear the selected ICACHE flags.
  * @param  __FLAG__ specifies the ICACHE flags to clear.
  *         This parameter can be any combination of the following values:
  *            @arg @ref ICACHE_FLAG_BUSYEND  Busy end flag
  *            @arg @ref ICACHE_FLAG_ERROR  Cache error flag
  */
#define __HAL_ICACHE_CLEAR_FLAG(__FLAG__) WRITE_REG(ICACHE->FCR, (__FLAG__))

/**
  * @}
  */

/**
  * @}
  */

/* Exported functions -------------------------------------------------------*/
/** @addtogroup ICACHE_Exported_Functions
  * @{
  */

/** @addtogroup ICACHE_Exported_Functions_Group1
  * @brief    Initialization and control functions
  * @{
  */
/* Peripheral Control functions **********************************************/
HAL_StatusTypeDef HAL_ICACHE_Enable(void);
HAL_StatusTypeDef HAL_ICACHE_Disable(void);
uint32_t HAL_ICACHE_IsEnabled(void);
HAL_StatusTypeDef HAL_ICACHE_ConfigAssociativityMode(uint32_t AssociativityMode);
HAL_StatusTypeDef HAL_ICACHE_DeInit(void);

/******* Invalidate in blocking mode (Polling) */
HAL_StatusTypeDef HAL_ICACHE_Invalidate(void);
/******* Invalidate in non-blocking mode (Interrupt) */
HAL_StatusTypeDef HAL_ICACHE_Invalidate_IT(void);
/******* Wait for Invalidate complete in blocking mode (Polling) */
HAL_StatusTypeDef HAL_ICACHE_WaitForInvalidateComplete(void);

/******* Performance instruction cache monitoring functions */
HAL_StatusTypeDef HAL_ICACHE_Monitor_Start(uint32_t MonitorType);
HAL_StatusTypeDef HAL_ICACHE_Monitor_Stop(uint32_t MonitorType);
HAL_StatusTypeDef HAL_ICACHE_Monitor_Reset(uint32_t MonitorType);
uint32_t HAL_ICACHE_Monitor_GetHitValue(void);
uint32_t HAL_ICACHE_Monitor_GetMissValue(void);

/**
  * @}
  */

/** @addtogroup ICACHE_Exported_Functions_Group2
  * @brief    IRQ and callback functions
  * @{
  */
/******* IRQHandler and Callbacks used in non-blocking mode (Interrupt) */
void HAL_ICACHE_IRQHandler(void);
void HAL_ICACHE_InvalidateCompleteCallback(void);
void HAL_ICACHE_ErrorCallback(void);

/**
  * @}
  */

#if defined(ICACHE_CRRx_REN)
/** @addtogroup ICACHE_Exported_Functions_Group3
  * @brief    Memory remapped regions functions
  * @{
  */
/******* Memory remapped regions functions */
HAL_StatusTypeDef HAL_ICACHE_EnableRemapRegion(uint32_t Region, const ICACHE_RegionConfigTypeDef *const pRegionConfig);
HAL_StatusTypeDef HAL_ICACHE_DisableRemapRegion(uint32_t Region);

/**
  * @}
  */
#endif /*  ICACHE_CRRx_REN */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* ICACHE */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_HAL_ICACHE_H */
