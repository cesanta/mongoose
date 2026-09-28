/**
  ******************************************************************************
  * @file    stm32h5xx_hal_cortex.h
  * @author  MCD Application Team
  * @brief   Header file of CORTEX HAL module.
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
#ifndef __STM32H5xx_HAL_CORTEX_H
#define __STM32H5xx_HAL_CORTEX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup CORTEX CORTEX
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup CORTEX_Exported_Types CORTEX Exported Types
  * @{
  */

/** @defgroup CORTEX_MPU_Region_Initialization_Structure_definition MPU Region Initialization Structure Definition
  * @{
  */
typedef struct
{
  uint8_t                Enable;            /*!< Specifies the status of the region.
                                                 This parameter can be a value of @ref CORTEX_MPU_Region_Enable       */
  uint8_t                Number;            /*!< Specifies the index of the region to protect.
                                                 This parameter can be a value of @ref CORTEX_MPU_Region_Number       */
  uint32_t               BaseAddress;       /*!< Specifies the base address of the region to protect.                 */
  uint32_t               LimitAddress;      /*!< Specifies the limit address of the region to protect.                */
  uint8_t                AttributesIndex;   /*!< Specifies the memory attributes index.
                                                 This parameter can be a value of @ref CORTEX_MPU_Attributes_Number   */
  uint8_t                AccessPermission;  /*!< Specifies the region access permission type. This parameter
                                                 can be a value of @ref CORTEX_MPU_Region_Permission_Attributes       */
  uint8_t                DisableExec;       /*!< Specifies the instruction access status.
                                                 This parameter can be a value of @ref CORTEX_MPU_Instruction_Access  */
  uint8_t                IsShareable;       /*!< Specifies the shareability status of the protected region.
                                                 This parameter can be a value of @ref CORTEX_MPU_Access_Shareable    */
} MPU_Region_InitTypeDef;
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Attributes_Initialization_Structure_definition MPU Attributes
  *           Initialization Structure Definition
  * @{
  */
typedef struct
{
  uint8_t                Number;            /*!< Specifies the number of the memory attributes to configure.
                                                 This parameter can be a value of @ref CORTEX_MPU_Attributes_Number   */

  uint8_t                Attributes;        /*!< Specifies the memory attributes value. Attributes This parameter
                                                 can be a combination of @ref CORTEX_MPU_Attributes                   */

} MPU_Attributes_InitTypeDef;
/**
  * @}
  */


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup CORTEX_Exported_Constants CORTEX Exported Constants
  * @{
  */

/** @defgroup CORTEX_Preemption_Priority_Group CORTEX Preemption Priority Group
  * @{
  */
#define NVIC_PRIORITYGROUP_0            0x7U /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1            0x6U /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2            0x5U /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3            0x4U /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4            0x3U /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
/**
  * @}
  */

/** @defgroup CORTEX_SysTick_clock_source CORTEX SysTick clock source
  * @{
  */
#define SYSTICK_CLKSOURCE_HCLK_DIV8     0x0U /*!< AHB clock divided by 8 selected as SysTick clock source */
#define SYSTICK_CLKSOURCE_LSI           0x1U /*!< LSI clock selected as SysTick clock source              */
#define SYSTICK_CLKSOURCE_LSE           0x2U /*!< LSE clock selected as SysTick clock source              */
#define SYSTICK_CLKSOURCE_HCLK          0x4U /*!< AHB clock selected as SysTick clock source              */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_HFNMI_PRIVDEF_Control CORTEX MPU HFNMI and PRIVILEGED Access control
  * @{
  */
#define  MPU_HFNMI_PRIVDEF_NONE          0U /*!< Background region access not allowed, MPU disabled for Hardfaults, NMIs, and exception handlers when FAULTMASK=1 */
#define  MPU_HARDFAULT_NMI               2U /*!< Background region access not allowed, MPU enabled for Hardfaults, NMIs, and exception handlers when FAULTMASK=1 */
#define  MPU_PRIVILEGED_DEFAULT          4U /*!< Background region privileged-only access allowed, MPU disabled for Hardfaults, NMIs, and exception handlers when FAULTMASK=1 */
#define  MPU_HFNMI_PRIVDEF               6U /*!< Background region privileged-only access allowed, MPU enabled for Hardfaults, NMIs, and exception handlers when FAULTMASK=1 */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Enable CORTEX MPU Region Enable
  * @{
  */
#define  MPU_REGION_ENABLE               1U /*!< Enable region */
#define  MPU_REGION_DISABLE              0U /*!< Disable region */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Instruction_Access CORTEX MPU Instruction Access
  * @{
  */
#define  MPU_INSTRUCTION_ACCESS_ENABLE   0U /*!< Execute attribute */
#define  MPU_INSTRUCTION_ACCESS_DISABLE  1U /*!< Execute never attribute */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Shareable CORTEX MPU Instruction Access Shareable
  * @{
  */
#define  MPU_ACCESS_NOT_SHAREABLE        0U /*!< Not shareable attribute */
#define  MPU_ACCESS_OUTER_SHAREABLE      2U /*!< Outer shareable attribute */
#define  MPU_ACCESS_INNER_SHAREABLE      3U /*!< Inner shareable attribute */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Permission_Attributes CORTEX MPU Region Permission Attributes
  * @{
  */
#define  MPU_REGION_PRIV_RW              0U /*!< Read/write privileged-only attribute */
#define  MPU_REGION_ALL_RW               1U /*!< Read/write privileged/unprivileged attribute */
#define  MPU_REGION_PRIV_RO              2U /*!< Read-only privileged-only attribute */
#define  MPU_REGION_ALL_RO               3U /*!< Read-only privileged/unprivileged attribute */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Number CORTEX MPU Region Number
  * @{
  */
#define  MPU_REGION_NUMBER0              0U  /*!< MPU region number 0  */
#define  MPU_REGION_NUMBER1              1U  /*!< MPU region number 1  */
#define  MPU_REGION_NUMBER2              2U  /*!< MPU region number 2  */
#define  MPU_REGION_NUMBER3              3U  /*!< MPU region number 3  */
#define  MPU_REGION_NUMBER4              4U  /*!< MPU region number 4  */
#define  MPU_REGION_NUMBER5              5U  /*!< MPU region number 5  */
#define  MPU_REGION_NUMBER6              6U  /*!< MPU region number 6  */
#define  MPU_REGION_NUMBER7              7U  /*!< MPU region number 7  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define  MPU_REGION_NUMBER8              8U  /*!< MPU region number 8  */
#define  MPU_REGION_NUMBER9              9U  /*!< MPU region number 9  */
#define  MPU_REGION_NUMBER10             10U /*!< MPU region number 10 */
#define  MPU_REGION_NUMBER11             11U /*!< MPU region number 11 */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Attributes_Number CORTEX MPU Memory Attributes Number
  * @{
  */
#define  MPU_ATTRIBUTES_NUMBER0          0U /*!< MPU attribute number 0 */
#define  MPU_ATTRIBUTES_NUMBER1          1U /*!< MPU attribute number 1 */
#define  MPU_ATTRIBUTES_NUMBER2          2U /*!< MPU attribute number 2 */
#define  MPU_ATTRIBUTES_NUMBER3          3U /*!< MPU attribute number 3 */
#define  MPU_ATTRIBUTES_NUMBER4          4U /*!< MPU attribute number 4 */
#define  MPU_ATTRIBUTES_NUMBER5          5U /*!< MPU attribute number 5 */
#define  MPU_ATTRIBUTES_NUMBER6          6U /*!< MPU attribute number 6 */
#define  MPU_ATTRIBUTES_NUMBER7          7U /*!< MPU attribute number 7 */
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Attributes CORTEX MPU Attributes
  * @{
  */
/* Device memory attributes */
#define  MPU_DEVICE_nGnRnE          0x0U  /*!< Device non-Gathering, non-Reordering, no Early write acknowledgement */
#define  MPU_DEVICE_nGnRE           0x4U  /*!< Device non-Gathering, non-Reordering, Early write acknowledgement */
#define  MPU_DEVICE_nGRE            0x8U  /*!< Device non-Gathering, Reordering, Early write acknowledgement */
#define  MPU_DEVICE_GRE             0xCU  /*!< Device Gathering, Reordering, Early write acknowledgement */

/* Normal memory attributes */
/* To set with INNER_OUTER() macro for both inner/outer cache attributes */

/* Non-cacheable memory attribute */
#define  MPU_NOT_CACHEABLE          0x4U  /*!< Normal memory, non-cacheable. */

/* Cacheable memory attributes: combination of cache write policy, transient and allocation */
/* - cache write policy */
#define  MPU_WRITE_THROUGH          0x0U  /*!< Normal memory, write-through. */
#define  MPU_WRITE_BACK             0x4U  /*!< Normal memory, write-back.    */
/* - transient mode attribute */
#define  MPU_TRANSIENT              0x0U  /*!< Normal memory, transient.     */
#define  MPU_NON_TRANSIENT          0x8U  /*!< Normal memory, non-transient. */
/* - allocation attribute */
#define  MPU_NO_ALLOCATE            0x0U  /*!< Normal memory, no allocate.         */
#define  MPU_W_ALLOCATE             0x1U  /*!< Normal memory, write allocate.      */
#define  MPU_R_ALLOCATE             0x2U  /*!< Normal memory, read allocate.       */
#define  MPU_RW_ALLOCATE            0x3U  /*!< Normal memory, read/write allocate. */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup CORTEX_Exported_Macros CORTEX Exported Macros
  * @{
  */
#define OUTER(__ATTR__)        ((__ATTR__) << 4U)
#define INNER_OUTER(__ATTR__)  ((__ATTR__) | ((__ATTR__) << 4U))

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup CORTEX_Exported_Functions CORTEX Exported Functions
  * @{
  */

/** @defgroup CORTEX_Exported_Functions_Group1 NVIC functions
  * @brief    NVIC functions
  * @{
  */
/* NVIC functions *****************************/
void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
uint32_t HAL_NVIC_GetPriorityGrouping(void);
void HAL_NVIC_GetPriority(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t *const pPreemptPriority,
                          uint32_t *const pSubPriority);
uint32_t HAL_NVIC_GetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_SetPendingIRQ(IRQn_Type IRQn);
void HAL_NVIC_ClearPendingIRQ(IRQn_Type IRQn);
uint32_t HAL_NVIC_GetActive(IRQn_Type IRQn);
/**
  * @}
  */

/** @defgroup CORTEX_Exported_Functions_Group2 SYSTICK functions
  * @brief   SYSTICK functions
  * @{
  */
/* SYSTICK functions ***********************************************/
uint32_t HAL_SYSTICK_Config(uint32_t TicksNumb);
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);
uint32_t HAL_SYSTICK_GetCLKSourceConfig(void);
void HAL_SYSTICK_IRQHandler(void);
void HAL_SYSTICK_Callback(void);
/**
  * @}
  */

/** @defgroup CORTEX_Exported_Functions_Group3 MPU functions
  * @brief   MPU functions
  * @{
  */
/* MPU functions ***********************************************/
void HAL_MPU_Enable(uint32_t MPU_Control);
void HAL_MPU_Disable(void);
void HAL_MPU_EnableRegion(uint32_t RegionNumber);
void HAL_MPU_DisableRegion(uint32_t RegionNumber);
void HAL_MPU_ConfigRegion(const MPU_Region_InitTypeDef *const pMPU_RegionInit);
void HAL_MPU_ConfigMemoryAttributes(const MPU_Attributes_InitTypeDef *const pMPU_AttributesInit);
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* MPU_NS Control functions ***********************************************/
void HAL_MPU_Enable_NS(uint32_t MPU_Control);
void HAL_MPU_Disable_NS(void);
void HAL_MPU_EnableRegion_NS(uint32_t RegionNumber);
void HAL_MPU_DisableRegion_NS(uint32_t RegionNumber);
void HAL_MPU_ConfigRegion_NS(const MPU_Region_InitTypeDef *const pMPU_RegionInit);
void HAL_MPU_ConfigMemoryAttributes_NS(const MPU_Attributes_InitTypeDef *const pMPU_AttributesInit);
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/**
  * @}
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/** @defgroup CORTEX_Private_Macros CORTEX Private Macros
  * @{
  */
#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PRIORITYGROUP_0) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_1) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_2) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_3) || \
                                       ((GROUP) == NVIC_PRIORITYGROUP_4))

#define IS_NVIC_PREEMPTION_PRIORITY(PRIORITY)  ((PRIORITY) < (1UL<<__NVIC_PRIO_BITS))

#define IS_NVIC_SUB_PRIORITY(PRIORITY)         ((PRIORITY) < (1UL<<__NVIC_PRIO_BITS))

#define IS_NVIC_DEVICE_IRQ(IRQ)                ((IRQ) > SysTick_IRQn)

#define IS_SYSTICK_CLK_SOURCE(SOURCE) (((SOURCE) == SYSTICK_CLKSOURCE_LSI) || \
                                       ((SOURCE) == SYSTICK_CLKSOURCE_LSE) || \
                                       ((SOURCE) == SYSTICK_CLKSOURCE_HCLK)|| \
                                       ((SOURCE) == SYSTICK_CLKSOURCE_HCLK_DIV8))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_MPU_INSTANCE(INSTANCE) (((INSTANCE) == MPU) || ((INSTANCE) == MPU_NS))
#endif /* __ARM_FEATURE_CMSE */

#define IS_MPU_REGION_ENABLE(STATE) (((STATE) == MPU_REGION_ENABLE) || \
                                     ((STATE) == MPU_REGION_DISABLE))

#define IS_MPU_INSTRUCTION_ACCESS(STATE) (((STATE) == MPU_INSTRUCTION_ACCESS_ENABLE) || \
                                          ((STATE) == MPU_INSTRUCTION_ACCESS_DISABLE))

#define IS_MPU_ACCESS_SHAREABLE(STATE)   (((STATE) == MPU_ACCESS_OUTER_SHAREABLE) || \
                                          ((STATE) == MPU_ACCESS_INNER_SHAREABLE) || \
                                          ((STATE) == MPU_ACCESS_NOT_SHAREABLE))

#define IS_MPU_REGION_PERMISSION_ATTRIBUTE(TYPE) (((TYPE) == MPU_REGION_PRIV_RW) || \
                                                  ((TYPE) == MPU_REGION_ALL_RW)  || \
                                                  ((TYPE) == MPU_REGION_PRIV_RO)     || \
                                                  ((TYPE) == MPU_REGION_ALL_RO))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_MPU_REGION_NUMBER(NUMBER)    (((NUMBER) == MPU_REGION_NUMBER0) || \
                                         ((NUMBER) == MPU_REGION_NUMBER1) || \
                                         ((NUMBER) == MPU_REGION_NUMBER2) || \
                                         ((NUMBER) == MPU_REGION_NUMBER3) || \
                                         ((NUMBER) == MPU_REGION_NUMBER4) || \
                                         ((NUMBER) == MPU_REGION_NUMBER5) || \
                                         ((NUMBER) == MPU_REGION_NUMBER6) || \
                                         ((NUMBER) == MPU_REGION_NUMBER7) || \
                                         ((NUMBER) == MPU_REGION_NUMBER8) || \
                                         ((NUMBER) == MPU_REGION_NUMBER9) || \
                                         ((NUMBER) == MPU_REGION_NUMBER10)|| \
                                         ((NUMBER) == MPU_REGION_NUMBER11))

#define IS_MPU_REGION_NUMBER_NS(NUMBER) (((NUMBER) == MPU_REGION_NUMBER0) || \
                                         ((NUMBER) == MPU_REGION_NUMBER1) || \
                                         ((NUMBER) == MPU_REGION_NUMBER2) || \
                                         ((NUMBER) == MPU_REGION_NUMBER3) || \
                                         ((NUMBER) == MPU_REGION_NUMBER4) || \
                                         ((NUMBER) == MPU_REGION_NUMBER5) || \
                                         ((NUMBER) == MPU_REGION_NUMBER6) || \
                                         ((NUMBER) == MPU_REGION_NUMBER7))
#else
#define IS_MPU_REGION_NUMBER(NUMBER)    (((NUMBER) == MPU_REGION_NUMBER0) || \
                                         ((NUMBER) == MPU_REGION_NUMBER1) || \
                                         ((NUMBER) == MPU_REGION_NUMBER2) || \
                                         ((NUMBER) == MPU_REGION_NUMBER3) || \
                                         ((NUMBER) == MPU_REGION_NUMBER4) || \
                                         ((NUMBER) == MPU_REGION_NUMBER5) || \
                                         ((NUMBER) == MPU_REGION_NUMBER6) || \
                                         ((NUMBER) == MPU_REGION_NUMBER7))
#endif /*  __ARM_FEATURE_CMSE */

#define IS_MPU_ATTRIBUTES_NUMBER(NUMBER)  (((NUMBER) == MPU_ATTRIBUTES_NUMBER0) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER1) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER2) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER3) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER4) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER5) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER6) || \
                                           ((NUMBER) == MPU_ATTRIBUTES_NUMBER7))

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_HAL_CORTEX_H */


