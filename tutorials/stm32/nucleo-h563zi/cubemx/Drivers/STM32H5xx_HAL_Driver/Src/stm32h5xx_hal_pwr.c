/**
  ******************************************************************************
  * @file    stm32h5xx_hal_pwr.c
  * @author  MCD Application Team
  * @brief   PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Initialization/De-Initialization Functions.
  *           + Peripheral Control Functions.
  *           + PWR Attributes Functions.
  *
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup PWR PWR
  * @brief PWR HAL module driver
  * @{
  */

#if defined (HAL_PWR_MODULE_ENABLED)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup PWR_Private_Defines PWR Private Defines
  * @{
  */

/** @defgroup PWR_PVD_Mode_Mask PWR PVD Mode Mask
  * @{
  */
#define PVD_RISING_EDGE  (0x01U)  /*!< Mask for rising edge set as PVD
                                       trigger                                */
#define PVD_FALLING_EDGE (0x02U)  /*!< Mask for falling edge set as PVD
                                       trigger                                */
#define PVD_MODE_IT      (0x04U)  /*!< Mask for interruption yielded by PVD
                                       threshold crossing                     */
#define PVD_MODE_EVT     (0x08U)  /*!< Mask for event yielded by PVD threshold
                                       crossing                               */
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PWR_Exported_Functions PWR Exported Functions
  * @{
  */

/** @defgroup PWR_Exported_Functions_Group1 Initialization and De-Initialization Functions
  *  @brief   Initialization and de-Initialization functions
  *
@verbatim
 ===============================================================================
              ##### Initialization and De-Initialization Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Deinitialize the HAL PWR peripheral registers to their default reset
  *         values.
  * @note   This functionality is not available in this product.
  *         The prototype is kept just to maintain compatibility with other
  *         products.
  * @retval None.
  */
void HAL_PWR_DeInit(void)
{
}

/**
  * @brief  Enable access to the backup domain (RCC Backup domain control
  *         register RCC_BDCR, RTC registers, TAMP registers, backup registers
  *         and backup SRAM).
  * @note   After a system reset, the backup domain is protected against
  *         possible unwanted write accesses.
  * @retval None.
  */
void HAL_PWR_EnableBkUpAccess(void)
{
  SET_BIT(PWR->DBPCR, PWR_DBPCR_DBP);
}

/**
  * @brief  Disable access to the backup domain (RCC Backup domain control
  *         register RCC_BDCR, RTC registers, TAMP registers, backup registers
  *         and backup SRAM).
  * @retval None
  */
void HAL_PWR_DisableBkUpAccess(void)
{
  CLEAR_BIT(PWR->DBPCR, PWR_DBPCR_DBP);
}
/**
  * @}
  */

/** @defgroup PWR_Exported_Functions_Group2 Peripheral Control Functions
  *  @brief   Low power modes configuration functions
  *
@verbatim
 ===============================================================================
                 ##### Peripheral Control functions #####
 ===============================================================================
     [..]
@endverbatim
  * @{
  */

/**
  * @brief  Configure the voltage threshold detected by the Programmed Voltage
  *         Detector (PVD).
  * @param  sConfigPVD : Pointer to a PWR_PVDTypeDef structure that contains the
  *                      PVD configuration information (PVDLevel and EventMode).
  * @retval None.
  */
HAL_StatusTypeDef HAL_PWR_ConfigPVD(const PWR_PVDTypeDef *sConfigPVD)
{
  /* Check the parameters */
  assert_param(IS_PWR_PVD_LEVEL(sConfigPVD->PVDLevel));
  assert_param(IS_PWR_PVD_MODE(sConfigPVD->Mode));

  /* Set PLS[3:1] bits according to PVDLevel value */
  MODIFY_REG(PWR->VMCR, PWR_VMCR_PLS, sConfigPVD->PVDLevel);

  /* Disable PVD Event/Interrupt */
  __HAL_PWR_PVD_EXTI_DISABLE_EVENT();
  __HAL_PWR_PVD_EXTI_DISABLE_IT();
  __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();
  __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();

  /* Configure the PVD in interrupt mode */
  if ((sConfigPVD->Mode & PVD_MODE_IT) == PVD_MODE_IT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_IT();
  }

  /* Configure the PVD in event mode */
  if ((sConfigPVD->Mode & PVD_MODE_EVT) == PVD_MODE_EVT)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_EVENT();
  }

  /* Configure the PVD in rising edge */
  if ((sConfigPVD->Mode & PVD_RISING_EDGE) == PVD_RISING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();
  }

  /* Configure the PVD in falling edge */
  if ((sConfigPVD->Mode & PVD_FALLING_EDGE) == PVD_FALLING_EDGE)
  {
    __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();
  }

  return HAL_OK;
}

/**
  * @brief  Enable the programmable voltage detector (PVD).
  * @retval None.
  */
void HAL_PWR_EnablePVD(void)
{
  SET_BIT(PWR->VMCR, PWR_VMCR_PVDEN);
}

/**
  * @brief  Disable the programmable voltage detector (PVD).
  * @retval None.
  */
void HAL_PWR_DisablePVD(void)
{
  CLEAR_BIT(PWR->VMCR, PWR_VMCR_PVDEN);
}

/**
  * @brief  Enable the WakeUp PINx functionality.
  * @param  WakeUpPinPolarity : Specifies which Wake-Up pin to enable.
  *          This parameter can be one of the following legacy values, which
  *          sets the default (rising edge):
  *            @arg PWR_WAKEUP_PIN1, PWR_WAKEUP_PIN2, PWR_WAKEUP_PIN3,PWR_WAKEUP_PIN4,
  *                 PWR_WAKEUP_PIN5, PWR_WAKEUP_PIN6, PWR_WAKEUP_PIN7.PWR_WAKEUP_PIN8.
  *          or one of the following values where the user can explicitly states
  *          the enabled pin and the chosen polarity:
  *            @arg PWR_WAKEUP_PIN1_HIGH, PWR_WAKEUP_PIN1_LOW,
  *                 PWR_WAKEUP_PIN2_HIGH, PWR_WAKEUP_PIN2_LOW,
  *                 PWR_WAKEUP_PIN3_HIGH, PWR_WAKEUP_PIN3_LOW,
  *                 PWR_WAKEUP_PIN4_HIGH, PWR_WAKEUP_PIN4_LOW,
  *                 PWR_WAKEUP_PIN5_HIGH, PWR_WAKEUP_PIN5_LOW,
  *                 PWR_WAKEUP_PIN6_HIGH, PWR_WAKEUP_PIN6_LOW,
  *                 PWR_WAKEUP_PIN7_HIGH, PWR_WAKEUP_PIN7_LOW,
  *                 PWR_WAKEUP_PIN8_HIGH, PWR_WAKEUP_PIN8_LOW.
  * @note   PWR_WAKEUP_PINx and PWR_WAKEUP_PINx_HIGH are equivalent.
  * @note   The PWR_WAKEUP_PIN6_HIGH, PWR_WAKEUP_PIN6_LOW, PWR_WAKEUP_PIN7_HIGH, PWR_WAKEUP_PIN7_LOW,
  *         PWR_WAKEUP_PIN8_HIGH and PWR_WAKEUP_PIN8_LOW are not available for STM32H503xx devices.
  * @retval None.
  */
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinPolarity)
{
  /* Check the parameters */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinPolarity));

  /*
     Enable and Specify the Wake-Up pin polarity and the pull configuration
     for the event detection (rising or falling edge).
  */
  MODIFY_REG(PWR->WUCR, PWR_EWUP_MASK, WakeUpPinPolarity);
}

/**
  * @brief  Disable the WakeUp PINx functionality.
  * @param  WakeUpPinx : Specifies the Power Wake-Up pin to disable.
  *          This parameter can be one of the following values:
  *            @arg PWR_WAKEUP_PIN1, PWR_WAKEUP_PIN2, PWR_WAKEUP_PIN3,PWR_WAKEUP_PIN4,
  *                 PWR_WAKEUP_PIN5, PWR_WAKEUP_PIN6, PWR_WAKEUP_PIN7.PWR_WAKEUP_PIN8.
  *          or one of the following values where the user can explicitly states
  *          the enabled pin and the chosen polarity:
  *            @arg PWR_WAKEUP_PIN1_HIGH, PWR_WAKEUP_PIN1_LOW,
  *                 PWR_WAKEUP_PIN2_HIGH, PWR_WAKEUP_PIN2_LOW,
  *                 PWR_WAKEUP_PIN3_HIGH, PWR_WAKEUP_PIN3_LOW,
  *                 PWR_WAKEUP_PIN4_HIGH, PWR_WAKEUP_PIN4_LOW,
  *                 PWR_WAKEUP_PIN5_HIGH, PWR_WAKEUP_PIN5_LOW,
  *                 PWR_WAKEUP_PIN6_HIGH, PWR_WAKEUP_PIN6_LOW,
  *                 PWR_WAKEUP_PIN7_HIGH, PWR_WAKEUP_PIN7_LOW,
  *                 PWR_WAKEUP_PIN8_HIGH, PWR_WAKEUP_PIN8_LOW.
  * @note   The PWR_WAKEUP_PIN6_HIGH, PWR_WAKEUP_PIN6_LOW, PWR_WAKEUP_PIN7_HIGH, PWR_WAKEUP_PIN7_LOW,
  *         PWR_WAKEUP_PIN8_HIGH and PWR_WAKEUP_PIN8_LOW are not available for STM32H503xx devices.
  * @retval None.
  */
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx)
{
  /* Check the parameters */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));

  /* Disable the wake up pin selected */
  CLEAR_BIT(PWR->WUCR, (PWR_WUCR_WUPEN & WakeUpPinx));
}

/**
  * @brief  Enter the CPU in SLEEP mode.
  * @note   In SLEEP mode, all I/O pins keep the same state as in Run mode.
  * @note   CPU clock is off and all peripherals including Cortex-M33 core such
  *         as NVIC and SysTick can run and wake up the CPU when an interrupt
  *         or an event occurs.
  * @param  Regulator : Specifies the regulator state in Sleep mode.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_MAINREGULATOR_ON
  *                     @arg @ref PWR_LOWPOWERREGULATOR_ON
  * @note   This parameter is not available in this product.
  *         The parameter is kept just to maintain compatibility with other
  *         products.
  * @param  SLEEPEntry : Specifies if SLEEP mode is entered with WFI or WFE
  *                      instruction.
  *                      This parameter can be one of the following values :
  *                      @arg @ref PWR_SLEEPENTRY_WFI enter SLEEP mode with Wait
  *                                For Interrupt request.
  *                      @arg @ref PWR_SLEEPENTRY_WFE enter SLEEP mode with Wait
  *                                For Event request.
  * @note   When WFI entry is used, ticks interrupt must be disabled to avoid
  *         unexpected CPU wake up.
  * @retval None.
  */
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry)
{
  UNUSED(Regulator);

  /* Check the parameter */
  assert_param(IS_PWR_SLEEP_ENTRY(SLEEPEntry));

  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Select SLEEP mode entry */
  if (SLEEPEntry == PWR_SLEEPENTRY_WFI)
  {
    /* Wait For Interrupt Request */
    __WFI();
  }
  else
  {
    /* Wait For Event Request */
    __SEV();
    __WFE();
    __WFE();
  }
}

/**
  * @brief  Enter the whole system to STOP mode.
  * @note   In STOP mode, the regulator remains in main regulator mode,
  *         allowing a very fast wakeup time but with much higher consumption
  *         comparing to other STOP modes.
  * @note   STOP offers the largest number of active peripherals and wakeup
  *         sources, a smaller wakeup time but a higher consumption.
  *         STOP mode achieves the lowest power consumption while retaining
  *         the content of SRAM and registers. All clocks in the VCORE domain
  *         are stopped. The PLL, the HSI, the CSI and the HSE crystal oscillators
  *         are disabled. The LSE or LSI is still running.
  * @note   The system clock when exiting from Stop mode can be either HSI
  *         or CSI, depending on software configuration.
  * @param  Regulator : Specifies the regulator state in Sleep mode.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_MAINREGULATOR_ON
  *                     @arg @ref PWR_LOWPOWERREGULATOR_ON
  * @note   This parameter is not available in this product.
  *         The parameter is kept just to maintain compatibility with other
  *         products.
  * @param  STOPEntry : Specifies if STOP mode is entered with WFI or WFE
  *                     instruction.
  *                     This parameter can be one of the following values :
  *                     @arg @ref PWR_STOPENTRY_WFI enter STOP mode with Wait
  *                               For Interrupt request.
  *                     @arg @ref PWR_STOPENTRY_WFE enter STOP mode with Wait
  *                               For Event request.
  * @retval None.
  */
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry)
{
  UNUSED(Regulator);

  /* Check the parameter */
  assert_param(IS_PWR_STOP_ENTRY(STOPEntry));

  /* Select STOP mode */
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_LPMS);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Select STOP mode entry */
  if (STOPEntry == PWR_STOPENTRY_WFI)
  {
    /* Wait For Interrupt Request */
    __WFI();
  }
  else
  {
    /* Wait For Event Request */
    __SEV();
    __WFE();
    __WFE();
  }

  /* Reset SLEEPDEEP bit of Cortex System Control Register */
  CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
  * @brief  Enter the whole system to STANDBY mode.
  * @note   The STANDBY mode is used to achieve the lowest power consumption
  *         with BOR. The internal regulator is switched off so that the VCORE
  *         domain is powered off. The PLL, the HSI, the CSI and the HSE crystal
  *         oscillators are also switched off.
  * @note   After entering STANDBY mode, SRAMs and register contents are lost
  *         except for registers and backup SRAM in the Backup domain and
  *         STANDBY circuitry.
  * @retval None.
  */
void HAL_PWR_EnterSTANDBYMode(void)
{
  /* Select STANDBY mode */
  SET_BIT(PWR->PMCR, PWR_PMCR_LPMS);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

  /* Wait For all memory accesses to complete before continuing */
  __DSB();

  /* Ensure that the processor pipeline is flushed */
  __ISB();

  /* Wait For Interrupt Request */
  __WFI();
}

/**
  * @brief  Indicate SLEEP-ON-EXIT feature when returning from handler mode to
  *         thread mode.
  * @note   Set SLEEPONEXIT bit of SCR register. When this bit is set, the
  *         processor re-enters SLEEP mode when an interruption handling is over.
  *         Setting this bit is useful when the processor is expected to run
  *         only on interruptions handling.
  * @retval None.
  */
void HAL_PWR_EnableSleepOnExit(void)
{
  /* Set SLEEPONEXIT bit of Cortex-M33 System Control Register */
  SET_BIT(SCB->SCR, SCB_SCR_SLEEPONEXIT_Msk);
}

/**
  * @brief  Disable SLEEP-ON-EXIT feature when returning from handler mode to
  *         thread mode.
  * @note   Clears SLEEPONEXIT bit of SCR register. When this bit is set, the
  *         processor re-enters SLEEP mode when an interruption handling is over.
  * @retval None.
  */
void HAL_PWR_DisableSleepOnExit(void)
{
  /* Clear SLEEPONEXIT bit of Cortex-M33 System Control Register */
  CLEAR_BIT(SCB->SCR, SCB_SCR_SLEEPONEXIT_Msk);
}

/**
  * @brief  Enable CORTEX SEV-ON-PEND feature.
  * @note   Sets SEVONPEND bit of SCR register. When this bit is set, any
  *         pending event / interrupt even if it's disabled or has insufficient
  *         priority to cause exception entry wakes up the Cortex-M33.
  * @retval None.
  */
void HAL_PWR_EnableSEVOnPend(void)
{
  /* Set SEVONPEND bit of Cortex-M33 System Control Register */
  SET_BIT(SCB->SCR, SCB_SCR_SEVONPEND_Msk);
}

/**
  * @brief  Disable CORTEX SEVONPEND feature.
  * @note   Resets SEVONPEND bit of SCR register. When this bit is reset, only
  *         enabled pending causes exception entry wakes up the Cortex-M33.
  * @retval None.
  */
void HAL_PWR_DisableSEVOnPend(void)
{
  /* Clear SEVONPEND bit of Cortex-M33 System Control Register */
  CLEAR_BIT(SCB->SCR, SCB_SCR_SEVONPEND_Msk);
}

/**
  * @brief  This function handles the PWR PVD interrupt request.
  * @note   This API should be called under the PVD_AVD_IRQHandler().
  * @note   The use of this API is only when we activate the PVD.
  * @note   When the PVD and AVD are activated at the same time you must use this API:
  *         HAL_PWREx_PVD_AVD_IRQHandler.
  * @retval None.
  */
void HAL_PWR_PVD_IRQHandler(void)
{
  uint32_t  rising_flag;
  uint32_t  falling_flag;

  /* Get pending flags */
  rising_flag  = READ_REG(EXTI->RPR1);
  falling_flag = READ_REG(EXTI->FPR1);

  /* Check PWR EXTI flags for PVD */
  if (((rising_flag | falling_flag) & PWR_EXTI_LINE_PVD) != 0U)
  {
    /* PWR PVD interrupt user callback */
    HAL_PWR_PVDCallback();

    /* Clear PVD EXTI pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_PVD);
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_PVD);
  }
}

/**
  * @brief  PWR PVD interrupt callback.
  * @retval None.
  */
__weak void HAL_PWR_PVDCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PWR_PVDCallback can be implemented in the user file
  */
}
/**
  * @}
  */

/** @defgroup PWR_Exported_Functions_Group3 Attributes Management Functions
  *  @brief    Attributes management functions
  *
@verbatim
 ===============================================================================
                       ##### PWR Attributes Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Configure the PWR item attributes.
  * @note   Available attributes are security and privilege protection.
  * @note   Security attribute can only be set only by secure access.
  * @note   Privilege attribute for secure items can be managed only by a secure
  *         privileged access.
  * @note   Privilege attribute for nsecure items can be managed  by a secure
  *         privileged access or by a nsecure privileged access.
  * @note   As the privileged attributes concern either all secure or all non-secure
  *         PWR resources accesses and not each PWR individual items access attribute,
  *         the application must ensure that the privilege access attribute configurations
  *         are coherent amongst the security level set on PWR individual items so not to
  *         overwrite a previous more restricted access rule (consider either all secure
  *         and/or all non-secure PWR resources accesses by privileged-only transactions
  *         or privileged and unprivileged transactions).
  * @param  Item       : Specifies the item(s) to set attributes on.
  *                      This parameter can be a combination of @ref PWR_Items.
  * @param  Attributes : Specifies the available attribute(s).
  *                      This parameter can be one of @ref PWR_Attributes.
  * @retval None.
  */
void HAL_PWR_ConfigAttributes(uint32_t Item, uint32_t Attributes)
{
  /* Check the parameters */
  assert_param(IS_PWR_ATTRIBUTES(Attributes));

#if defined (PWR_SECCFGR_WUP1SEC)
  assert_param(IS_PWR_ITEMS_ATTRIBUTES(Item));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Secure item management (TZEN = 1) */
  if ((Attributes & PWR_ITEM_ATTR_SEC_PRIV_MASK) == PWR_ITEM_ATTR_SEC_PRIV_MASK)
  {
    /* Privilege item management */
    if ((Attributes & PWR_SEC_PRIV) == PWR_SEC_PRIV)
    {
      SET_BIT(PWR->SECCFGR, Item);
      SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_SPRIV);
    }
    else
    {
      SET_BIT(PWR->SECCFGR, Item);
      CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_SPRIV);
    }
  }
  /* NSecure item management */
  else
  {
    /* Privilege item management */
    if ((Attributes & PWR_NSEC_PRIV) == PWR_NSEC_PRIV)
    {
      CLEAR_BIT(PWR->SECCFGR, Item);
      SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
    }
    else
    {
      CLEAR_BIT(PWR->SECCFGR, Item);
      CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
    }
  }
#else
  /* NSecure item management (TZEN = 0) */
  if ((Attributes & PWR_ITEM_ATTR_NSEC_PRIV_MASK) == PWR_ITEM_ATTR_NSEC_PRIV_MASK)
  {
    /* Privilege item management */
    if ((Attributes & PWR_NSEC_PRIV) == PWR_NSEC_PRIV)
    {
      SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
    }
    else
    {
      CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
    }
  }
#endif /* __ARM_FEATURE_CMSE */

#else /* PWR_SECCFGR_WUP1SEC */
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Item);

  /* NSecure item management (TZEN = 0) */
  if ((Attributes & PWR_ITEM_ATTR_NSEC_PRIV_MASK) == PWR_ITEM_ATTR_NSEC_PRIV_MASK)
  {
    /* Privilege item management */
    if ((Attributes & PWR_PRIV) == PWR_PRIV)
    {
      SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_PRIV);
    }
    else
    {
      CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_PRIV);
    }
  }
#endif /* PWR_SECCFGR_WUP1SEC */
}

/**
  * @brief  Get attribute(s) of a PWR item.
  * @param  Item        : Specifies the item(s) to set attributes on.
  *                       This parameter can be one of @ref PWR_Items.
  * @param  pAttributes : Pointer to return attribute(s).
  *                       Returned value could be on of @ref PWR_Attributes.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWR_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes)
{
  uint32_t attributes;

  /* Check attribute pointer */
  if (pAttributes == NULL)
  {
    return HAL_ERROR;
  }
#if defined (PWR_SECCFGR_WUP1SEC)
  /* Check the parameter */
  assert_param(IS_PWR_ITEMS_ATTRIBUTES(Item));
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Check item security */
  if ((PWR->SECCFGR & Item) == Item)
  {
    /* Get Secure privileges attribute */
    attributes = ((PWR->PRIVCFGR & PWR_PRIVCFGR_SPRIV) == 0U) ? PWR_SEC_NPRIV : PWR_SEC_PRIV;
  }
  else
  {
    /* Get Non-Secure privileges attribute */
    attributes = ((PWR->PRIVCFGR & PWR_PRIVCFGR_NSPRIV) == 0U) ? PWR_NSEC_NPRIV : PWR_NSEC_PRIV;
  }
#else
  /* Get Non-Secure privileges attribute */
  attributes = ((PWR->PRIVCFGR & PWR_PRIVCFGR_NSPRIV) == 0U) ? PWR_NSEC_NPRIV : PWR_NSEC_PRIV;
#endif /* __ARM_FEATURE_CMSE */

#else  /* PWR_SECCFGR_WUP1SEC*/
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Item);

  /* Get Non-Secure privileges attribute */
  attributes = ((PWR->PRIVCFGR & PWR_PRIVCFGR_PRIV) == 0U) ? PWR_NPRIV : PWR_PRIV;
#endif /* PWR_SECCFGR_WUP1SEC */

  /* return value */
  *pAttributes = attributes;

  return HAL_OK;
}
/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (HAL_PWR_MODULE_ENABLED) */
/**
  * @}
  */

/**
  * @}
  */
