/**
  ******************************************************************************
  * @file    stm32f7xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of PWR extension peripheral:           
  *           + Peripheral Extended features functions
  *         
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief PWR HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup PWREx_Private_Constants
  * @{
  */    
#define PWR_OVERDRIVE_TIMEOUT_VALUE  1000
#define PWR_UDERDRIVE_TIMEOUT_VALUE  1000
#define PWR_BKPREG_TIMEOUT_VALUE     1000
#define PWR_VOSRDY_TIMEOUT_VALUE     1000
/**
  * @}
  */
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup PWREx_Exported_Functions PWREx Exported Functions
  *  @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Peripheral Extended features functions 
  *  @brief Peripheral Extended features functions 
  *
@verbatim   

 ===============================================================================
                 ##### Peripheral extended features functions #####
 ===============================================================================

    *** Main and Backup Regulators configuration ***
    ================================================
    [..] 
      (+) The backup domain includes 4 Kbytes of backup SRAM accessible only from 
          the CPU, and address in 32-bit, 16-bit or 8-bit mode. Its content is 
          retained even in Standby or VBAT mode when the low power backup regulator
          is enabled. It can be considered as an internal EEPROM when VBAT is 
          always present. You can use the HAL_PWREx_EnableBkUpReg() function to 
          enable the low power backup regulator. 

      (+) When the backup domain is supplied by VDD (analog switch connected to VDD) 
          the backup SRAM is powered from VDD which replaces the VBAT power supply to 
          save battery life.

      (+) The backup SRAM is not mass erased by a tamper event. It is read 
          protected to prevent confidential data, such as cryptographic private 
          key, from being accessed. The backup SRAM can be erased only through 
          the Flash interface when a protection level change from level 1 to 
          level 0 is requested. 
      -@- Refer to the description of Read protection (RDP) in the Flash 
          programming manual.

      (+) The main internal regulator can be configured to have a tradeoff between 
          performance and power consumption when the device does not operate at 
          the maximum frequency. This is done through __HAL_PWR_MAINREGULATORMODE_CONFIG() 
          macro which configure VOS bit in PWR_CR register
          
        Refer to the product datasheets for more details.

    *** FLASH Power Down configuration ****
    =======================================
    [..] 
      (+) By setting the FPDS bit in the PWR_CR register by using the 
          HAL_PWREx_EnableFlashPowerDown() function, the Flash memory also enters power 
          down mode when the device enters Stop mode. When the Flash memory 
          is in power down mode, an additional startup delay is incurred when 
          waking up from Stop mode.

    *** Over-Drive and Under-Drive configuration ****
    =================================================
    [..]         
       (+) In Run mode: the main regulator has 2 operating modes available:
        (++) Normal mode: The CPU and core logic operate at maximum frequency at a given 
             voltage scaling (scale 1, scale 2 or scale 3)
        (++) Over-drive mode: This mode allows the CPU and the core logic to operate at a 
            higher frequency than the normal mode for a given voltage scaling (scale 1,  
            scale 2 or scale 3). This mode is enabled through HAL_PWREx_EnableOverDrive() function and
            disabled by HAL_PWREx_DisableOverDrive() function, to enter or exit from Over-drive mode please follow 
            the sequence described in Reference manual.
             
       (+) In Stop mode: the main regulator or low power regulator supplies a low power 
           voltage to the 1.2V domain, thus preserving the content of registers 
           and internal SRAM. 2 operating modes are available:
         (++) Normal mode: the 1.2V domain is preserved in nominal leakage mode. This mode is only 
              available when the main regulator or the low power regulator is used in Scale 3 or 
              low voltage mode.
         (++) Under-drive mode: the 1.2V domain is preserved in reduced leakage mode. This mode is only
              available when the main regulator or the low power regulator is in low voltage mode.

@endverbatim
  * @{
  */

/**
  * @brief Enables the Backup Regulator.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_EnableBkUpReg(void)
{
  uint32_t tickstart = 0;

  /* Enable Backup regulator */
  PWR->CSR1 |= PWR_CSR1_BRE;
    
  /* Workaround for the following hardware bug: */
  /* Id 19: PWR : No STANDBY wake-up when Back-up RAM enabled (ref. Errata Sheet p23) */
  PWR->CSR1 |= PWR_CSR1_EIWUP;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait till Backup regulator ready flag is set */  
  while(__HAL_PWR_GET_FLAG(PWR_FLAG_BRR) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > PWR_BKPREG_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    } 
  }
  return HAL_OK;
}

/**
  * @brief Disables the Backup Regulator.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_DisableBkUpReg(void)
{
  uint32_t tickstart = 0;
  
  /* Disable Backup regulator */
  PWR->CSR1 &= (uint32_t)~((uint32_t)PWR_CSR1_BRE);
  
  /* Workaround for the following hardware bug: */
  /* Id 19: PWR : No STANDBY wake-up when Back-up RAM enabled (ref. Errata Sheet p23) */
  PWR->CSR1 |= PWR_CSR1_EIWUP;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait till Backup regulator ready flag is set */  
  while(__HAL_PWR_GET_FLAG(PWR_FLAG_BRR) != RESET)
  {
    if((HAL_GetTick() - tickstart ) > PWR_BKPREG_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    } 
  }
  return HAL_OK;
}

/**
  * @brief Enables the Flash Power Down in Stop mode.
  * @retval None
  */
void HAL_PWREx_EnableFlashPowerDown(void)
{
  /* Enable the Flash Power Down */
  PWR->CR1 |= PWR_CR1_FPDS;
}

/**
  * @brief Disables the Flash Power Down in Stop mode.
  * @retval None
  */
void HAL_PWREx_DisableFlashPowerDown(void)
{
  /* Disable the Flash Power Down */
  PWR->CR1 &= (uint32_t)~((uint32_t)PWR_CR1_FPDS);
}

/**
  * @brief Enables Main Regulator low voltage mode.
  * @retval None
  */
void HAL_PWREx_EnableMainRegulatorLowVoltage(void)
{
  /* Enable Main regulator low voltage */
  PWR->CR1 |= PWR_CR1_MRUDS;
}

/**
  * @brief Disables Main Regulator low voltage mode.
  * @retval None
  */
void HAL_PWREx_DisableMainRegulatorLowVoltage(void)
{  
  /* Disable Main regulator low voltage */
  PWR->CR1 &= (uint32_t)~((uint32_t)PWR_CR1_MRUDS);
}

/**
  * @brief Enables Low Power Regulator low voltage mode.
  * @retval None
  */
void HAL_PWREx_EnableLowRegulatorLowVoltage(void)
{
  /* Enable low power regulator */
  PWR->CR1 |= PWR_CR1_LPUDS;
}

/**
  * @brief Disables Low Power Regulator low voltage mode.
  * @retval None
  */
void HAL_PWREx_DisableLowRegulatorLowVoltage(void)
{
  /* Disable low power regulator */
  PWR->CR1 &= (uint32_t)~((uint32_t)PWR_CR1_LPUDS);
}

/**
  * @brief  Activates the Over-Drive mode.
  * @note   This mode allows the CPU and the core logic to operate at a higher frequency
  *         than the normal mode for a given voltage scaling (scale 1, scale 2 or scale 3).   
  * @note   It is recommended to enter or exit Over-drive mode when the application is not running 
  *         critical tasks and when the system clock source is either HSI or HSE. 
  *         During the Over-drive switch activation, no peripheral clocks should be enabled.   
  *         The peripheral clocks must be enabled once the Over-drive mode is activated.   
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_EnableOverDrive(void)
{
  uint32_t tickstart = 0;

  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* Enable the Over-drive to extend the clock frequency to 216 MHz */
  __HAL_PWR_OVERDRIVE_ENABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODRDY))
  {
    if((HAL_GetTick() - tickstart ) > PWR_OVERDRIVE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
  
  /* Enable the Over-drive switch */
  __HAL_PWR_OVERDRIVESWITCHING_ENABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_ODSWRDY))
  {
    if((HAL_GetTick() - tickstart ) > PWR_OVERDRIVE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  } 
  return HAL_OK;
}

/**
  * @brief  Deactivates the Over-Drive mode.
  * @note   This mode allows the CPU and the core logic to operate at a higher frequency
  *         than the normal mode for a given voltage scaling (scale 1, scale 2 or scale 3).    
  * @note   It is recommended to enter or exit Over-drive mode when the application is not running 
  *         critical tasks and when the system clock source is either HSI or HSE. 
  *         During the Over-drive switch activation, no peripheral clocks should be enabled.   
  *         The peripheral clocks must be enabled once the Over-drive mode is activated.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PWREx_DisableOverDrive(void)
{
  uint32_t tickstart = 0;
  
  __HAL_RCC_PWR_CLK_ENABLE();
    
  /* Disable the Over-drive switch */
  __HAL_PWR_OVERDRIVESWITCHING_DISABLE();
  
  /* Get tick */
  tickstart = HAL_GetTick();
 
  while(__HAL_PWR_GET_FLAG(PWR_FLAG_ODSWRDY))
  {
    if((HAL_GetTick() - tickstart ) > PWR_OVERDRIVE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  } 
  
  /* Disable the Over-drive */
  __HAL_PWR_OVERDRIVE_DISABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  while(__HAL_PWR_GET_FLAG(PWR_FLAG_ODRDY))
  {
    if((HAL_GetTick() - tickstart ) > PWR_OVERDRIVE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
  
  return HAL_OK;
}

/**
  * @brief  Enters in Under-Drive STOP mode.
  * 
  * @note    This mode can be selected only when the Under-Drive is already active 
  *   
  * @note    This mode is enabled only with STOP low power mode.
  *          In this mode, the 1.2V domain is preserved in reduced leakage mode. This 
  *          mode is only available when the main regulator or the low power regulator 
  *          is in low voltage mode
  *        
  * @note   If the Under-drive mode was enabled, it is automatically disabled after 
  *         exiting Stop mode. 
  *         When the voltage regulator operates in Under-drive mode, an additional  
  *         startup delay is induced when waking up from Stop mode.
  *                    
  * @note   In Stop mode, all I/O pins keep the same state as in Run mode.
  *   
  * @note   When exiting Stop mode by issuing an interrupt or a wakeup event, 
  *         the HSI RC oscillator is selected as system clock.
  *           
  * @note   When the voltage regulator operates in low power mode, an additional 
  *         startup delay is incurred when waking up from Stop mode. 
  *         By keeping the internal regulator ON during Stop mode, the consumption 
  *         is higher although the startup time is reduced.
  *     
  * @param  Regulator specifies the regulator state in STOP mode.
  *          This parameter can be one of the following values:
  *            @arg PWR_MAINREGULATOR_UNDERDRIVE_ON:  Main Regulator in under-drive mode 
  *                 and Flash memory in power-down when the device is in Stop under-drive mode
  *            @arg PWR_LOWPOWERREGULATOR_UNDERDRIVE_ON:  Low Power Regulator in under-drive mode 
  *                and Flash memory in power-down when the device is in Stop under-drive mode
  * @param  STOPEntry specifies if STOP mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PWR_SLEEPENTRY_WFI: enter STOP mode with WFI instruction
  *            @arg PWR_SLEEPENTRY_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
HAL_StatusTypeDef HAL_PWREx_EnterUnderDriveSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
  uint32_t tempreg = 0;
  uint32_t tickstart = 0;
  
  /* Check the parameters */
  assert_param(IS_PWR_REGULATOR_UNDERDRIVE(Regulator));
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));
  
  /* Enable Power ctrl clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  /* Enable the Under-drive Mode ---------------------------------------------*/
  /* Clear Under-drive flag */
  __HAL_PWR_CLEAR_ODRUDR_FLAG();
  
  /* Enable the Under-drive */ 
  __HAL_PWR_UNDERDRIVE_ENABLE();

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait for UnderDrive mode is ready */
  while(__HAL_PWR_GET_FLAG(PWR_FLAG_UDRDY))
  {
    if((HAL_GetTick() - tickstart ) > PWR_UDERDRIVE_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
  
  /* Select the regulator state in STOP mode ---------------------------------*/
  tempreg = PWR->CR1;
  /* Clear PDDS, LPDS, MRLUDS and LPLUDS bits */
  tempreg &= (uint32_t)~(PWR_CR1_PDDS | PWR_CR1_LPDS | PWR_CR1_LPUDS | PWR_CR1_MRUDS);
  
  /* Set LPDS, MRLUDS and LPLUDS bits according to PWR_Regulator value */
  tempreg |= Regulator;
  
  /* Store the new value */
  PWR->CR1 = tempreg;
  
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  
  /* Select STOP mode entry --------------------------------------------------*/
  if(STOPEntry == PWR_SLEEPENTRY_WFI)
  {   
    /* Request Wait For Interrupt */
    __WFI();
  }
  else
  {
    /* Request Wait For Event */
    __WFE();
  }
  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

  return HAL_OK;  
}

/**
  * @brief Returns Voltage Scaling Range.
  * @retval VOS bit field (PWR_REGULATOR_VOLTAGE_SCALE1, PWR_REGULATOR_VOLTAGE_SCALE2 or 
  *            PWR_REGULATOR_VOLTAGE_SCALE3)PWR_REGULATOR_VOLTAGE_SCALE1
  */  
uint32_t HAL_PWREx_GetVoltageRange(void)
{
  return  (PWR->CR1 & PWR_CR1_VOS);
}

/**
  * @brief Configures the main internal regulator output voltage.
  * @param  VoltageScaling specifies the regulator output voltage to achieve
  *         a tradeoff between performance and power consumption.
  *          This parameter can be one of the following values:
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE1: Regulator voltage output range 1 mode,
  *                                                typical output voltage at 1.4 V,  
  *                                                system frequency up to 216 MHz.
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE2: Regulator voltage output range 2 mode,
  *                                                typical output voltage at 1.2 V,                
  *                                                system frequency up to 180 MHz.
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE3: Regulator voltage output range 2 mode,
  *                                                typical output voltage at 1.00 V,                
  *                                                system frequency up to 151 MHz.
  * @note To update the system clock frequency(SYSCLK):
  *        - Set the HSI or HSE as system clock frequency using the HAL_RCC_ClockConfig().
  *        - Call the HAL_RCC_OscConfig() to configure the PLL.
  *        - Call HAL_PWREx_ConfigVoltageScaling() API to adjust the voltage scale.
  *        - Set the new system clock frequency using the HAL_RCC_ClockConfig().
  * @note The scale can be modified only when the HSI or HSE clock source is selected 
  *        as system clock source, otherwise the API returns HAL_ERROR.  
  * @note When the PLL is OFF, the voltage scale 3 is automatically selected and the VOS bits
  *       value in the PWR_CR1 register are not taken in account.
  * @note This API forces the PLL state ON to allow the possibility to configure the voltage scale 1 or 2.
  * @note The new voltage scale is active only when the PLL is ON.  
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling)
{
  uint32_t tickstart = 0;

  assert_param(IS_PWR_REGULATOR_VOLTAGE(VoltageScaling));

  /* Enable Power ctrl clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* Check if the PLL is used as system clock or not */
  if(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
  {
    /* Disable the main PLL */
    __HAL_RCC_PLL_DISABLE();
    
    /* Get Start Tick */
    tickstart = HAL_GetTick();    
    /* Wait till PLL is disabled */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) != RESET)
    {
      if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }
    
    /* Set Range */
    __HAL_PWR_VOLTAGESCALING_CONFIG(VoltageScaling);
    
    /* Enable the main PLL */
    __HAL_RCC_PLL_ENABLE();
    
    /* Get Start Tick */
    tickstart = HAL_GetTick();
    /* Wait till PLL is ready */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
    {
      if((HAL_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      } 
    }
    
    /* Get Start Tick */
    tickstart = HAL_GetTick();
    while((__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) == RESET))
    {
      if((HAL_GetTick() - tickstart ) > PWR_VOSRDY_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      } 
    }
  }
  else
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_PWR_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

