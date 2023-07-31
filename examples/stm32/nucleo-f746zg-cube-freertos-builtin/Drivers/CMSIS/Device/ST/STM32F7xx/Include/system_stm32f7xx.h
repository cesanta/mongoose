/**
  ******************************************************************************
  * @file    system_stm32f7xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M7 Device System Source File for STM32F7xx devices.       
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************  
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f7xx_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32F7XX_H
#define __SYSTEM_STM32F7XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup STM32F7xx_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup STM32F7xx_System_Exported_Variables
  * @{
  */
  /* The SystemCoreClock variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
    */
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

extern const uint8_t  AHBPrescTable[16];    /*!< AHB prescalers table values */
extern const uint8_t  APBPrescTable[8];     /*!< APB prescalers table values */


/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F7xx_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F7XX_H */

/**
  * @}
  */
  
/**
  * @}
  */  
