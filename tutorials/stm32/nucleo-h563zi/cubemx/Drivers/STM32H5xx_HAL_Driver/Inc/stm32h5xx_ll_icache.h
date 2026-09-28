/**
  ******************************************************************************
  * @file    stm32h5xx_ll_icache.h
  * @author  MCD Application Team
  * @brief   Header file of ICACHE LL module.
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
#ifndef STM32H5xx_LL_ICACHE_H
#define STM32H5xx_LL_ICACHE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

#if defined(ICACHE)

/** @defgroup ICACHE_LL ICACHE
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#if defined(ICACHE_CRRx_REN)
/** @defgroup ICACHE_LL_REGION_CONFIG ICACHE Exported Configuration structure
  * @{
  */

/**
  * @brief  LL ICACHE region configuration structure definition
  */
typedef struct
{
  uint32_t BaseAddress;              /*!< Configures the C-AHB base address to be remapped */

  uint32_t RemapAddress;             /*!< Configures the remap address to be remapped */

  uint32_t Size;                     /*!< Configures the region size.
                                          This parameter can be a value of @ref ICACHE_LL_EC_Region_Size */

  uint32_t TrafficRoute;             /*!< Selects the traffic route.
                                          This parameter can be a value of @ref ICACHE_LL_EC_Traffic_Route */

  uint32_t OutputBurstType;          /*!< Selects the output burst type.
                                          This parameter can be a value of @ref ICACHE_LL_EC_Output_Burst_Type */
} LL_ICACHE_RegionTypeDef;

/**
  * @}
  */
#endif /*  ICACHE_CRRx_REN */

/* Exported constants -------------------------------------------------------*/
/** @defgroup ICACHE_LL_Exported_Constants ICACHE Exported Constants
  * @{
  */

/** @defgroup ICACHE_LL_EC_WaysSelection Ways selection
  * @{
  */
#define LL_ICACHE_1WAY                 0U                /*!< 1-way cache (direct mapped cache) */
#define LL_ICACHE_2WAYS                ICACHE_CR_WAYSEL  /*!< 2-ways set associative cache (default) */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_Monitor_Type Monitor type
  * @{
  */
#define LL_ICACHE_MONITOR_HIT          ICACHE_CR_HITMEN                       /*!< Hit monitor counter */
#define LL_ICACHE_MONITOR_MISS         ICACHE_CR_MISSMEN                      /*!< Miss monitor counter */
#define LL_ICACHE_MONITOR_ALL          (ICACHE_CR_HITMEN | ICACHE_CR_MISSMEN) /*!< All monitors counters */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_ICACHE_ReadReg function
  * @{
  */
#define LL_ICACHE_SR_BUSYF             ICACHE_SR_BUSYF     /*!< Busy flag */
#define LL_ICACHE_SR_BSYENDF           ICACHE_SR_BSYENDF   /*!< Busy end flag */
#define LL_ICACHE_SR_ERRF              ICACHE_SR_ERRF      /*!< Cache error flag */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_CLEAR_FLAG Clear Flags Defines
  * @brief    Flags defines which can be used with LL_ICACHE_WriteReg function
  * @{
  */
#define LL_ICACHE_FCR_CBSYENDF         ICACHE_FCR_CBSYENDF /*!< Busy end flag */
#define LL_ICACHE_FCR_CERRF            ICACHE_FCR_CERRF    /*!< Cache error flag */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_IT IT Defines
  * @brief    IT defines which can be used with LL_ICACHE_ReadReg and  LL_ICACHE_WriteReg functions
  * @{
  */
#define LL_ICACHE_IER_BSYENDIE         ICACHE_IER_BSYENDIE /*!< Busy end interrupt */
#define LL_ICACHE_IER_ERRIE            ICACHE_IER_ERRIE    /*!< Cache error interrupt */
/**
  * @}
  */

#if defined(ICACHE_CRRx_REN)
/** @defgroup ICACHE_LL_EC_Region Remapped Region number
  * @{
  */
#define LL_ICACHE_REGION_0             0U  /*!< Region 0 */
#define LL_ICACHE_REGION_1             1U  /*!< Region 1 */
#define LL_ICACHE_REGION_2             2U  /*!< Region 2 */
#define LL_ICACHE_REGION_3             3U  /*!< Region 3 */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_Region_Size Remapped Region size
  * @{
  */
#define LL_ICACHE_REGIONSIZE_2MB       1U  /*!< Region size 2MB */
#define LL_ICACHE_REGIONSIZE_4MB       2U  /*!< Region size 4MB */
#define LL_ICACHE_REGIONSIZE_8MB       3U  /*!< Region size 8MB */
#define LL_ICACHE_REGIONSIZE_16MB      4U  /*!< Region size 16MB */
#define LL_ICACHE_REGIONSIZE_32MB      5U  /*!< Region size 32MB */
#define LL_ICACHE_REGIONSIZE_64MB      6U  /*!< Region size 64MB */
#define LL_ICACHE_REGIONSIZE_128MB     7U  /*!< Region size 128MB */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_Traffic_Route Remapped Traffic route
  * @{
  */
#define LL_ICACHE_MASTER1_PORT         0U                  /*!< Master1 port */
#define LL_ICACHE_MASTER2_PORT         ICACHE_CRRx_MSTSEL  /*!< Master2 port */
/**
  * @}
  */

/** @defgroup ICACHE_LL_EC_Output_Burst_Type Remapped Output burst type
  * @{
  */
#define LL_ICACHE_OUTPUT_BURST_WRAP    0U                  /*!< WRAP */
#define LL_ICACHE_OUTPUT_BURST_INCR    ICACHE_CRRx_HBURST  /*!< INCR */
/**
  * @}
  */
#endif /*  ICACHE_CRRx_REN */

/**
  * @}
  */

/* Exported macros ----------------------------------------------------------*/
/** @defgroup ICACHE_LL_Exported_Macros ICACHE Exported Macros
  * @{
  */

/** @defgroup ICACHE_LL_EM_WRITE_READ Common write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in ICACHE register
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_ICACHE_WriteReg(__REG__, __VALUE__) WRITE_REG(ICACHE->__REG__, (__VALUE__))

/**
  * @brief  Read a value in ICACHE register
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_ICACHE_ReadReg(__REG__) READ_REG(ICACHE->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup ICACHE_LL_Exported_Functions ICACHE Exported Functions
  * @{
  */

/** @defgroup ICACHE_LL_EF_Configuration Configuration
  * @{
  */

/**
  * @brief  Enable the ICACHE.
  * @rmtoll CR           EN            LL_ICACHE_Enable
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_Enable(void)
{
  SET_BIT(ICACHE->CR, ICACHE_CR_EN);
}

/**
  * @brief  Disable the ICACHE.
  * @rmtoll CR           EN            LL_ICACHE_Disable
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_Disable(void)
{
  CLEAR_BIT(ICACHE->CR, ICACHE_CR_EN);
}

/**
  * @brief  Return if ICACHE is enabled or not.
  * @rmtoll CR           EN            LL_ICACHE_IsEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsEnabled(void)
{
  return ((READ_BIT(ICACHE->CR, ICACHE_CR_EN) == (ICACHE_CR_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Select the ICACHE operating mode.
  * @rmtoll CR           WAYSEL        LL_ICACHE_SetMode
  * @param  Mode This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_1WAY
  *         @arg @ref LL_ICACHE_2WAYS
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetMode(uint32_t Mode)
{
  MODIFY_REG(ICACHE->CR, ICACHE_CR_WAYSEL, Mode);
}

/**
  * @brief  Get the selected ICACHE operating mode.
  * @rmtoll CR           WAYSEL        LL_ICACHE_GetMode
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_ICACHE_1WAY
  *         @arg @ref LL_ICACHE_2WAYS
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetMode(void)
{
  return (READ_BIT(ICACHE->CR, ICACHE_CR_WAYSEL));
}

/**
  * @brief  Invalidate the ICACHE.
  * @note   Until the BSYEND flag is set, the cache is bypassed.
  * @rmtoll CR           CACHEINV      LL_ICACHE_Invalidate
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_Invalidate(void)
{
  SET_BIT(ICACHE->CR, ICACHE_CR_CACHEINV);
}

/**
  * @}
  */

/** @defgroup ICACHE_LL_EF_Monitors Monitors
  * @{
  */

/**
  * @brief  Enable the hit/miss monitor(s).
  * @rmtoll CR           HITMEN        LL_ICACHE_EnableMonitors
  * @rmtoll CR           MISSMEN       LL_ICACHE_EnableMonitors
  * @param  Monitors This parameter can be one or a combination of the following values:
  *         @arg @ref LL_ICACHE_MONITOR_HIT
  *         @arg @ref LL_ICACHE_MONITOR_MISS
  *         @arg @ref LL_ICACHE_MONITOR_ALL
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_EnableMonitors(uint32_t Monitors)
{
  SET_BIT(ICACHE->CR, Monitors);
}

/**
  * @brief  Disable the hit/miss monitor(s).
  * @rmtoll CR           HITMEN        LL_ICACHE_DisableMonitors
  * @rmtoll CR           MISSMEN       LL_ICACHE_DisableMonitors
  * @param  Monitors This parameter can be one or a combination of the following values:
  *         @arg @ref LL_ICACHE_MONITOR_HIT
  *         @arg @ref LL_ICACHE_MONITOR_MISS
  *         @arg @ref LL_ICACHE_MONITOR_ALL
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_DisableMonitors(uint32_t Monitors)
{
  CLEAR_BIT(ICACHE->CR, Monitors);
}

/**
  * @brief  Check if the monitor(s) is(are) enabled or disabled.
  * @rmtoll CR           HITMEN        LL_ICACHE_IsEnabledMonitors
  * @rmtoll CR           MISSMEN       LL_ICACHE_IsEnabledMonitors
  * @param  Monitors This parameter can be one or a combination of the following values:
  *         @arg @ref LL_ICACHE_MONITOR_HIT
  *         @arg @ref LL_ICACHE_MONITOR_MISS
  *         @arg @ref LL_ICACHE_MONITOR_ALL
  * @retval State of parameter value (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsEnabledMonitors(uint32_t Monitors)
{
  return ((READ_BIT(ICACHE->CR, Monitors) == (Monitors)) ? 1UL : 0UL);
}

/**
  * @brief  Reset the hit/miss monitor(s).
  * @rmtoll CR           HITMRST       LL_ICACHE_ResetMonitors
  * @rmtoll CR           MISSMRST      LL_ICACHE_ResetMonitors
  * @param  Monitors This parameter can be one or a combination of the following values:
  *         @arg @ref LL_ICACHE_MONITOR_HIT
  *         @arg @ref LL_ICACHE_MONITOR_MISS
  *         @arg @ref LL_ICACHE_MONITOR_ALL
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_ResetMonitors(uint32_t Monitors)
{
  /* Reset */
  SET_BIT(ICACHE->CR, (Monitors << 2U));
  /* Release reset */
  CLEAR_BIT(ICACHE->CR, (Monitors << 2U));
}

/**
  * @brief  Get the Hit monitor.
  * @note   Upon reaching the 32-bit maximum value, hit monitor does not wrap.
  * @rmtoll HMONR        HITMON        LL_ICACHE_GetHitMonitor
  * @retval Value between Min_Data=0 and Max_Data=0xFFFFFFFF
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetHitMonitor(void)
{
  return (ICACHE->HMONR);
}

/**
  * @brief  Get the Miss monitor.
  * @note   Upon reaching the 16-bit maximum value, miss monitor does not wrap.
  * @rmtoll MMONR        MISSMON       LL_ICACHE_GetMissMonitor
  * @retval Value between Min_Data=0 and Max_Data=0xFFFF
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetMissMonitor(void)
{
  return (ICACHE->MMONR);
}

/**
  * @}
  */

/** @defgroup ICACHE_LL_EF_IT_Management IT_Management
  * @{
  */

/**
  * @brief  Enable BSYEND interrupt.
  * @rmtoll IER          BSYENDIE      LL_ICACHE_EnableIT_BSYEND
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_EnableIT_BSYEND(void)
{
  SET_BIT(ICACHE->IER, ICACHE_IER_BSYENDIE);
}

/**
  * @brief  Disable BSYEND interrupt.
  * @rmtoll IER          BSYENDIE      LL_ICACHE_DisableIT_BSYEND
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_DisableIT_BSYEND(void)
{
  CLEAR_BIT(ICACHE->IER, ICACHE_IER_BSYENDIE);
}

/**
  * @brief  Check if the BSYEND Interrupt is enabled or disabled.
  * @rmtoll IER          BSYENDIE      LL_ICACHE_IsEnabledIT_BSYEND
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsEnabledIT_BSYEND(void)
{
  return ((READ_BIT(ICACHE->IER, ICACHE_IER_BSYENDIE) == (ICACHE_IER_BSYENDIE)) ? 1UL : 0UL);
}

/**
  * @brief  Enable ERR interrupt.
  * @rmtoll IER          ERRIE         LL_ICACHE_EnableIT_ERR
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_EnableIT_ERR(void)
{
  SET_BIT(ICACHE->IER, ICACHE_IER_ERRIE);
}

/**
  * @brief  Disable ERR interrupt.
  * @rmtoll IER          ERRIE        LL_ICACHE_DisableIT_ERR
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_DisableIT_ERR(void)
{
  CLEAR_BIT(ICACHE->IER, ICACHE_IER_ERRIE);
}

/**
  * @brief  Check if the ERR Interrupt is enabled or disabled.
  * @rmtoll IER          ERRIE         LL_ICACHE_IsEnabledIT_ERR
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsEnabledIT_ERR(void)
{
  return ((READ_BIT(ICACHE->IER, ICACHE_IER_ERRIE) == (ICACHE_IER_ERRIE)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup ICACHE_LL_EF_FLAG_Management FLAG_Management
  * @{
  */

/**
  * @brief  Indicate the status of an ongoing operation flag.
  * @rmtoll SR           BUSYF         LL_ICACHE_IsActiveFlag_BUSY
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsActiveFlag_BUSY(void)
{
  return ((READ_BIT(ICACHE->SR, ICACHE_SR_BUSYF) == (ICACHE_SR_BUSYF)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate the status of an operation end flag.
  * @rmtoll SR           BSYEND       LL_ICACHE_IsActiveFlag_BSYEND
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsActiveFlag_BSYEND(void)
{
  return ((READ_BIT(ICACHE->SR, ICACHE_SR_BSYENDF) == (ICACHE_SR_BSYENDF)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate the status of an error flag.
  * @rmtoll SR           ERRF          LL_ICACHE_IsActiveFlag_ERR
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsActiveFlag_ERR(void)
{
  return ((READ_BIT(ICACHE->SR, ICACHE_SR_ERRF) == (ICACHE_SR_ERRF)) ? 1UL : 0UL);
}

/**
  * @brief  Clear busy end of operation flag.
  * @rmtoll FCR          CBSYENDF      LL_ICACHE_ClearFlag_BSYEND
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_ClearFlag_BSYEND(void)
{
  WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF);
}

/**
  * @brief  Clear error flag.
  * @rmtoll FCR          ERRF          LL_ICACHE_ClearFlag_ERR
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_ClearFlag_ERR(void)
{
  WRITE_REG(ICACHE->FCR, ICACHE_FCR_CERRF);
}

/**
  * @}
  */

#if defined(ICACHE_CRRx_REN)
/** @defgroup ICACHE_LL_EF_REGION_Management REGION_Management
  * @{
  */

/**
  * @brief  Enable the remapped memory region.
  * @note   The region must have been already configured.
  * @rmtoll CRRx         REN           LL_ICACHE_EnableRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_EnableRegion(uint32_t Region)
{
  SET_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
          ICACHE_CRRx_REN);
}

/**
  * @brief  Disable the remapped memory region.
  * @rmtoll CRRx         REN           LL_ICACHE_DisableRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_DisableRegion(uint32_t Region)
{
  CLEAR_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
            ICACHE_CRRx_REN);
}

/**
  * @brief  Return if remapped memory region is enabled or not.
  * @rmtoll CRRx         REN           LL_ICACHE_IsEnabledRegion
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_ICACHE_IsEnabledRegion(uint32_t Region)
{
  return ((READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                    ICACHE_CRRx_REN) == (ICACHE_CRRx_REN)) ? 1UL : 0UL);
}

/**
  * @brief  Select the memory remapped region base address.
  * @note   The useful bits depends on RSIZE as described in the Reference Manual.
  * @rmtoll CRRx         BASEADDR      LL_ICACHE_SetRegionBaseAddress
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @param  Address  Alias address in the Code region
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetRegionBaseAddress(uint32_t Region, uint32_t Address)
{
  MODIFY_REG(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
             ICACHE_CRRx_BASEADDR, ((Address & 0x1FFFFFFFU) >> 21U));
}

/**
  * @brief  Get the memory remapped region base address.
  * @note   The base address is the alias in the Code region.
  * @note   The useful bits depends on RSIZE as described in the Reference Manual.
  * @rmtoll CRRx         BASEADDR      LL_ICACHE_GetRegionBaseAddress
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval Address  Alias address in the Code region
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetRegionBaseAddress(uint32_t Region)
{
  return (READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                   ICACHE_CRRx_BASEADDR) << 21U);
}

/**
  * @brief  Select the memory remapped region address.
  * @note   The useful bits depends on RSIZE as described in the Reference Manual.
  * @rmtoll CRRx         REMAPADDR     LL_ICACHE_SetRegionRemapAddress
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @param  Address  Memory address to remap
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetRegionRemapAddress(uint32_t Region, uint32_t Address)
{
  MODIFY_REG(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
             ICACHE_CRRx_REMAPADDR, ((Address >> 21U) << ICACHE_CRRx_REMAPADDR_Pos));
}

/**
  * @brief  Get the memory remapped region address.
  * @note   The useful bits depends on RSIZE as described in the Reference Manual.
  * @rmtoll CRRx         REMAPADDR     LL_ICACHE_GetRegionRemapAddress
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval Address  Remapped memory address
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetRegionRemapAddress(uint32_t Region)
{
  return ((READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                    ICACHE_CRRx_REMAPADDR) >> ICACHE_CRRx_REMAPADDR_Pos) << 21U);
}

/**
  * @brief  Select the memory remapped region size.
  * @rmtoll CRRx         RSIZE         LL_ICACHE_SetRegionSize
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @param  Size This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGIONSIZE_2MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_4MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_8MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_16MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_32MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_64MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_128MB
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetRegionSize(uint32_t Region, uint32_t Size)
{
  MODIFY_REG(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
             ICACHE_CRRx_RSIZE, (Size << ICACHE_CRRx_RSIZE_Pos));
}

/**
  * @brief  Get the selected the memory remapped region size.
  * @rmtoll CRRx         RSIZE         LL_ICACHE_GetRegionSize
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_ICACHE_REGIONSIZE_2MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_4MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_8MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_16MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_32MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_64MB
  *         @arg @ref LL_ICACHE_REGIONSIZE_128MB
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetRegionSize(uint32_t Region)
{
  return (READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                   ICACHE_CRRx_RSIZE) >> ICACHE_CRRx_RSIZE_Pos);
}

/**
  * @brief  Select the memory remapped region output burst type.
  * @rmtoll CRRx         HBURST        LL_ICACHE_SetRegionOutputBurstType
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @param  Type This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_OUTPUT_BURST_WRAP
  *         @arg @ref LL_ICACHE_OUTPUT_BURST_INCR
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetRegionOutputBurstType(uint32_t Region, uint32_t Type)
{
  MODIFY_REG(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
             ICACHE_CRRx_HBURST, Type);
}

/**
  * @brief  Get the selected the memory remapped region output burst type.
  * @rmtoll CRRx         HBURST        LL_ICACHE_GetRegionOutputBurstType
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_ICACHE_OUTPUT_BURST_WRAP
  *         @arg @ref LL_ICACHE_OUTPUT_BURST_INCR
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetRegionOutputBurstType(uint32_t Region)
{
  return (READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                   ICACHE_CRRx_HBURST));
}

/**
  * @brief  Select the memory remapped region cache master port.
  * @rmtoll CRRx         MSTSEL        LL_ICACHE_SetRegionMasterPort
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @param  Port This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_MASTER1_PORT
  *         @arg @ref LL_ICACHE_MASTER2_PORT
  * @retval None
  */
__STATIC_INLINE void LL_ICACHE_SetRegionMasterPort(uint32_t Region, uint32_t Port)
{
  MODIFY_REG(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
             ICACHE_CRRx_MSTSEL, Port);
}

/**
  * @brief  Get the selected the memory remapped region cache master port.
  * @rmtoll CRRx         MSTSEL        LL_ICACHE_GetRegionMasterPort
  * @param  Region This parameter can be one of the following values:
  *         @arg @ref LL_ICACHE_REGION_0
  *         @arg @ref LL_ICACHE_REGION_1
  *         @arg @ref LL_ICACHE_REGION_2
  *         @arg @ref LL_ICACHE_REGION_3
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_ICACHE_MASTER1_PORT
  *         @arg @ref LL_ICACHE_MASTER2_PORT
  */
__STATIC_INLINE uint32_t LL_ICACHE_GetRegionMasterPort(uint32_t Region)
{
  return (READ_BIT(*((__IO uint32_t *)(&(ICACHE->CRR0) + (1U * Region))), \
                   ICACHE_CRRx_MSTSEL));
}

/**
  * @}
  */

#if defined(USE_FULL_LL_DRIVER)
/** @defgroup ICACHE_LL_EF_REGION_Init Region Initialization functions
  * @{
  */

void LL_ICACHE_ConfigRegion(uint32_t Region, const LL_ICACHE_RegionTypeDef *const pICACHE_RegionStruct);

/**
  * @}
  */
#endif /* USE_FULL_LL_DRIVER */

#endif /*  ICACHE_CRRx_REN */
/**
  * @}
  */

/**
  * @}
  */

#endif /* ICACHE */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_LL_ICACHE_H */
