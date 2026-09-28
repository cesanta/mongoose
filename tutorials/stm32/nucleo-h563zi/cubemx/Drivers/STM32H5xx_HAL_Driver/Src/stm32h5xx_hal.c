/**
  **********************************************************************************************************************
  * @file    stm32h5xx_hal.c
  * @author  MCD Application Team
  * @brief   HAL module driver.
  *          This is the common part of the HAL initialization
  *
  @verbatim
  ======================================================================================================================
                     ##### How to use this driver #####
  ======================================================================================================================
    [..]
    The common HAL driver contains a set of generic and common APIs that can be
    used by the PPP peripheral drivers and the user to start using the HAL.
    [..]
    The HAL contains two APIs' categories:
         (+) Common HAL APIs
         (+) Services HAL APIs

  @endverbatim
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup HAL HAL
  * @brief HAL module driver
  * @{
  */

#ifdef HAL_MODULE_ENABLED

/* Private typedef ---------------------------------------------------------------------------------------------------*/
/* Private define ----------------------------------------------------------------------------------------------------*/
/**
  * @brief STM32H5xx HAL Driver version number 1.6.0
   */
#define __STM32H5XX_HAL_VERSION_MAIN   (0x01UL) /*!< [31:24] main version */
#define __STM32H5XX_HAL_VERSION_SUB1   (0x06UL) /*!< [23:16] sub1 version */
#define __STM32H5XX_HAL_VERSION_SUB2   (0x00UL) /*!< [15:8]  sub2 version */
#define __STM32H5XX_HAL_VERSION_RC     (0x00UL) /*!< [7:0]  release candidate */
#define __STM32H5XX_HAL_VERSION         ((__STM32H5XX_HAL_VERSION_MAIN << 24U)\
                                         |(__STM32H5XX_HAL_VERSION_SUB1 << 16U)\
                                         |(__STM32H5XX_HAL_VERSION_SUB2 << 8U )\
                                         |(__STM32H5XX_HAL_VERSION_RC))

#if defined(VREFBUF)
#define VREFBUF_TIMEOUT_VALUE           10U   /* 10 ms */
#endif /* VREFBUF */

/* Value used to increment hide protection level */
#define SBS_HDPL_INCREMENT_VALUE  (uint8_t)0x6A

/* Value used to lock/unlock debug functionalities */
#define SBS_DEBUG_LOCK_VALUE      (uint8_t)0xC3
#define SBS_DEBUG_UNLOCK_VALUE    (uint8_t)0xB4

/* Private macro -----------------------------------------------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------------------------------------------------*/

/** @defgroup HAL_Exported_Variables HAL Exported Variables
  * @{
  */
__IO uint32_t uwTick;
uint32_t uwTickPrio   = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */
/**
  * @}
  */

/* Private function prototypes ---------------------------------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------------------------------------------------*/

/** @defgroup HAL_Exported_Functions HAL Exported Functions
  * @{
  */

/** @defgroup HAL_Exported_Functions_Group1 Initialization and de-initialization Functions
  *  @brief    Initialization and de-initialization functions
  *
@verbatim
 =======================================================================================================================
              ##### Initialization and de-initialization functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Initializes the Flash interface the NVIC allocation and initial clock
          configuration. It initializes the systick also when timeout is needed
          and the backup domain when enabled.
      (+) De-Initializes common part of the HAL.
      (+) Configure The time base source to have 1ms time base with a dedicated
          Tick interrupt priority.
        (++) SysTick timer is used by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
        (++) Time base configuration function (HAL_InitTick ()) is called automatically
             at the beginning of the program after reset by HAL_Init() or at any time
             when clock is configured, by HAL_RCC_ClockConfig().
        (++) Source of time base is configured  to generate interrupts at regular
             time intervals. Care must be taken if HAL_Delay() is called from a
             peripheral ISR process, the Tick interrupt line must have higher priority
            (numerically lower) than the peripheral interrupt. Otherwise the caller
            ISR process will be blocked.
       (++) functions affecting time base configurations are declared as __weak
             to make  override possible  in case of other  implementations in user file.
@endverbatim
  * @{
  */

/**
  * @brief  Configure the Flash prefetch, the time base source, NVIC and any required global low
  *         level hardware by calling the HAL_MspInit() callback function to be optionally defined
  *         in user file stm32h5xx_hal_msp.c.
  *
  * @note   HAL_Init() function is called at the beginning of program after reset and before
  *         the clock configuration.
  *
  * @note   In the default implementation the System Timer (SysTick) is used as source of time base.
  *         The SysTick configuration is based on HSI clock, as HSI is the clock
  *         used after a system Reset and the NVIC configuration is set to Priority group 4.
  *         Once done, time base tick starts incrementing: the tick variable counter is incremented
  *         each 1ms in the SysTick_Handler() interrupt handler.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_Init(void)
{
  /* Configure Flash prefetch */
#if (PREFETCH_ENABLE != 0U)
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR2 & RCC_CFGR2_HPRE) >> RCC_CFGR2_HPRE_Pos];

  /* Select HCLK as SysTick clock source */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Init the low level hardware */
  HAL_MspInit();

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  This function de-Initializes common part of the HAL and stops the systick.
  *         This function is optional.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_DeInit(void)
{
  /* Reset of all peripherals */
  __HAL_RCC_APB1_FORCE_RESET();
  __HAL_RCC_APB1_RELEASE_RESET();

  __HAL_RCC_APB2_FORCE_RESET();
  __HAL_RCC_APB2_RELEASE_RESET();

  __HAL_RCC_APB3_FORCE_RESET();
  __HAL_RCC_APB3_RELEASE_RESET();

  __HAL_RCC_AHB1_FORCE_RESET();
  __HAL_RCC_AHB1_RELEASE_RESET();

  __HAL_RCC_AHB2_FORCE_RESET();
  __HAL_RCC_AHB2_RELEASE_RESET();

#if defined(AHB4PERIPH_BASE)
  __HAL_RCC_AHB4_FORCE_RESET();
  __HAL_RCC_AHB4_RELEASE_RESET();
#endif /* AHB4PERIPH_BASE */

  /* De-Init the low level hardware */
  HAL_MspDeInit();

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initializes the MSP.
  * @retval None
  */
__weak void HAL_MspInit(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes the MSP.
  * @retval None
  */
__weak void HAL_MspDeInit(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief This function configures the source of the time base.
  *        The time source is configured to have 1ms time base with a dedicated
  *        Tick interrupt priority.
  * @note This function is called  automatically at the beginning of program after
  *       reset by HAL_Init() or at any time when clock is reconfigured  by HAL_RCC_ClockConfig().
  * @note In the default implementation, SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals.
  *       Care must be taken if HAL_Delay() is called from a peripheral ISR process,
  *       The SysTick interrupt must have higher priority (numerically lower)
  *       than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  *       The function is declared as __weak  to be overwritten  in case of other
  *       implementation  in user file.
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
__weak HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  uint32_t ticknumber = 0U;
  uint32_t systicksel;

  /* Check uwTickFreq for MisraC 2012 (even if uwTickFreq is a enum type that don't take the value zero)*/
  if ((uint32_t)uwTickFreq == 0UL)
  {
    return HAL_ERROR;
  }

  /* Check Clock source to calculate the tickNumber */
  if (READ_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk) == SysTick_CTRL_CLKSOURCE_Msk)
  {
    /* HCLK selected as SysTick clock source */
    ticknumber = SystemCoreClock / (1000UL / (uint32_t)uwTickFreq);
  }
  else
  {
    systicksel = HAL_SYSTICK_GetCLKSourceConfig();
    switch (systicksel)
    {
      /* HCLK_DIV8 selected as SysTick clock source */
      case SYSTICK_CLKSOURCE_HCLK_DIV8:
        /* Calculate tick value */
        ticknumber = (SystemCoreClock / (8000UL / (uint32_t)uwTickFreq));
        break;
      /* LSI selected as SysTick clock source */
      case SYSTICK_CLKSOURCE_LSI:
        /* Calculate tick value */
        ticknumber = (LSI_VALUE / (1000UL / (uint32_t)uwTickFreq));
        break;
      /* LSE selected as SysTick clock source */
      case SYSTICK_CLKSOURCE_LSE:
        /* Calculate tick value */
        ticknumber = (LSE_VALUE / (1000UL / (uint32_t)uwTickFreq));
        break;
      default:
        /* Nothing to do */
        break;
    }
  }

  /* Configure the SysTick to have interrupt in 1ms time basis*/
  if (HAL_SYSTICK_Config(ticknumber) > 0U)
  {
    return HAL_ERROR;
  }

  /* Configure the SysTick IRQ priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
  uwTickPrio = TickPriority;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HAL_Group2 HAL Control functions
  *  @brief    HAL Control functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL Control functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Provide a tick value in millisecond
      (+) Provide a blocking delay in millisecond
      (+) Suspend the time base source interrupt
      (+) Resume the time base source interrupt
      (+) Get the HAL API driver version
      (+) Get the device identifier
      (+) Get the device revision identifier

@endverbatim
  * @{
  */

/**
  * @brief This function is called to increment a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in SysTick ISR.
  * @note This function is declared as __weak to be overwritten in case of other
  *      implementations in user file.
  * @retval None
  */
__weak void HAL_IncTick(void)
{
  uwTick += (uint32_t)uwTickFreq;
}

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval tick value
  */
__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}

/**
  * @brief This function returns a tick priority.
  * @retval tick priority
  */
uint32_t HAL_GetTickPrio(void)
{
  return uwTickPrio;
}

/**
  * @brief Set new tick Freq.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{
  HAL_StatusTypeDef status  = HAL_OK;
  HAL_TickFreqTypeDef prevTickFreq;

  assert_param(IS_TICKFREQ(Freq));

  if (uwTickFreq != Freq)
  {

    /* Back up uwTickFreq frequency */
    prevTickFreq = uwTickFreq;

    /* Update uwTickFreq global variable used by HAL_InitTick() */
    uwTickFreq = Freq;

    /* Apply the new tick Freq  */
    status = HAL_InitTick(uwTickPrio);
    if (status != HAL_OK)
    {
      /* Restore previous tick frequency */
      uwTickFreq = prevTickFreq;
    }
  }

  return status;
}

/**
  * @brief Return tick frequency.
  * @retval Tick frequency.
  *         Value of @ref HAL_TickFreqTypeDef.
  */
HAL_TickFreqTypeDef HAL_GetTickFreq(void)
{
  return uwTickFreq;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay  specifies the delay time length, in milliseconds.
  * @retval None
  */
__weak void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}

/**
  * @brief Suspend Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *       used to generate interrupts at regular time intervals. Once HAL_SuspendTick()
  *       is called, the SysTick interrupt will be disabled and so Tick increment
  *       is suspended.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval None
  */
__weak void HAL_SuspendTick(void)
{
  /* Disable SysTick Interrupt */
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
  * @brief Resume Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *       used to generate interrupts at regular time intervals. Once HAL_ResumeTick()
  *       is called, the SysTick interrupt will be enabled and so Tick increment
  *       is resumed.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @retval None
  */
__weak void HAL_ResumeTick(void)
{
  /* Enable SysTick Interrupt */
  SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}

/**
  * @brief  Returns the HAL revision
  * @retval version : 0xXYZR (8bits for each decimal, R for RC)
  */
uint32_t HAL_GetHalVersion(void)
{
  return __STM32H5XX_HAL_VERSION;
}

/**
  * @brief  Returns the device revision identifier.
  * @retval Device revision identifier
  */
uint32_t HAL_GetREVID(void)
{
  return ((DBGMCU->IDCODE & DBGMCU_IDCODE_REV_ID) >> 16);
}

/**
  * @brief  Returns the device identifier.
  * @retval Device identifier
  */
uint32_t HAL_GetDEVID(void)
{
  return (DBGMCU->IDCODE & DBGMCU_IDCODE_DEV_ID);
}

/**
  * @brief  Return the first word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw0(void)
{
  return (READ_REG(*((uint32_t *)UID_BASE)));
}

/**
  * @brief  Return the second word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw1(void)
{
  return (READ_REG(*((uint32_t *)(UID_BASE + 4U))));
}

/**
  * @brief  Return the third word of the unique device identifier (UID based on 96 bits)
  * @retval Device identifier
  */
uint32_t HAL_GetUIDw2(void)
{
  return (READ_REG(*((uint32_t *)(UID_BASE + 8U))));
}

/**
  * @}
  */


/** @defgroup HAL_Exported_Functions_Group3 HAL Debug functions
  *  @brief    HAL Debug functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL Debug functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Enable/Disable Debug module during STOP mode
      (+) Enable/Disable Debug module during STANDBY mode

@endverbatim
  * @{
  */

/**
  * @brief  Enable the Debug Module during STOP mode.
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStopMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Disable the Debug Module during STOP mode.
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStopMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Enable the Debug Module during STANDBY mode.
  * @retval None
  */
void HAL_DBGMCU_EnableDBGStandbyMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @brief  Disable the Debug Module during STANDBY mode.
  * @retval None
  */
void HAL_DBGMCU_DisableDBGStandbyMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group4 HAL VREFBUF Control functions
  *  @brief    HAL VREFBUF Control functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL VREFBUF Control functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the Voltage reference buffer
      (+) Enable/Disable the Voltage reference buffer

@endverbatim
  * @{
  */

#if defined(VREFBUF)
/**
  * @brief Configure the internal voltage reference buffer voltage scale.
  * @param  VoltageScaling: specifies the output voltage to achieve
  *          This parameter can be one of the following values:
  *            @arg VREFBUF_VOLTAGE_SCALE0: VREF_OUT1 around 2.5 V.
  *                                                This requires VDDA equal to or higher than 2.8 V.
  *            @arg VREFBUF_VOLTAGE_SCALE1: VREF_OUT2 around 2.048 V.
  *                                                This requires VDDA equal to or higher than 2.4 V.
  *            @arg VREFBUF_VOLTAGE_SCALE2: VREF_OUT3 around 1.8 V.
  *                                                This requires VDDA equal to or higher than 2.1 V.
  *            @arg VREFBUF_VOLTAGE_SCALE3: VREF_OUT4 around 1.5 V.
  *                                                This requires VDDA equal to or higher than 1.8 V.
  * @retval None
  */
void HAL_VREFBUF_VoltageScalingConfig(uint32_t VoltageScaling)
{
  /* Check the parameters */
  assert_param(IS_VREFBUF_VOLTAGE_SCALE(VoltageScaling));

  MODIFY_REG(VREFBUF->CSR, VREFBUF_CSR_VRS, VoltageScaling);
}

/**
  * @brief Configure the internal voltage reference buffer high impedance mode.
  * @param  Mode: specifies the high impedance mode
  *          This parameter can be one of the following values:
  *            @arg VREFBUF_HIGH_IMPEDANCE_DISABLE: VREF+ pin is internally connect to VREFINT output.
  *            @arg VREFBUF_HIGH_IMPEDANCE_ENABLE: VREF+ pin is high impedance.
  * @retval None
  */
void HAL_VREFBUF_HighImpedanceConfig(uint32_t Mode)
{
  /* Check the parameters */
  assert_param(IS_VREFBUF_HIGH_IMPEDANCE(Mode));

  MODIFY_REG(VREFBUF->CSR, VREFBUF_CSR_HIZ, Mode);
}

/**
  * @brief  Tune the Internal Voltage Reference buffer (VREFBUF).
  * @retval None
  */
void HAL_VREFBUF_TrimmingConfig(uint32_t TrimmingValue)
{
  /* Check the parameters */
  assert_param(IS_VREFBUF_TRIMMING(TrimmingValue));

  MODIFY_REG(VREFBUF->CCR, VREFBUF_CCR_TRIM, TrimmingValue);
}

/**
  * @brief  Enable the Internal Voltage Reference buffer (VREFBUF).
  * @retval HAL_OK/HAL_TIMEOUT
  */
HAL_StatusTypeDef HAL_EnableVREFBUF(void)
{
  uint32_t  tickstart;

  SET_BIT(VREFBUF->CSR, VREFBUF_CSR_ENVR);

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait for VRR bit  */
  while (READ_BIT(VREFBUF->CSR, VREFBUF_CSR_VRR) == 0UL)
  {
    if ((HAL_GetTick() - tickstart) > VREFBUF_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Disable the Internal Voltage Reference buffer (VREFBUF).
  *
  * @retval None
  */
void HAL_DisableVREFBUF(void)
{
  CLEAR_BIT(VREFBUF->CSR, VREFBUF_CSR_ENVR);
}
#endif /* VREFBUF */

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group5 HAL SBS configuration functions
  *  @brief    HAL SBS configuration functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL SBS configuration functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Select the Ethernet PHY Interface
      (+) Enable/Disable the VDD I/Os Compensation Cell
      (+) Code selection/configuration for the VDD I/O Compensation cell
      (+) Get ready flag status of VDD I/Os Compensation cell
      (+) Get PMOS/NMOS compensation value of the I/Os supplied by VDD
      (+) Enable/Disable the NMI in case of double ECC error in FLASH Interface

@endverbatim
  * @{
  */

#if defined(SBS_PMCR_ETH_SEL_PHY)
/**
  * @brief  Ethernet PHY Interface Selection either MII or RMII
  * @param  SBS_ETHInterface: Selects the Ethernet PHY interface
  *   This parameter can be one of the following values:
  *   @arg SBS_ETH_MII : Select the Media Independent Interface
  *   @arg SBS_ETH_RMII: Select the Reduced Media Independent Interface
  * @retval None
  */
void HAL_SBS_ETHInterfaceSelect(uint32_t SBS_ETHInterface)
{
  /* Check the parameter */
  assert_param(IS_SBS_ETHERNET_CONFIG(SBS_ETHInterface));

  MODIFY_REG(SBS->PMCR, SBS_PMCR_ETH_SEL_PHY, (uint32_t)(SBS_ETHInterface));
}
#endif /* SBS_PMCR_ETH_SEL_PHY */

/**
  * @brief  Enables the VDD I/Os Compensation Cell.
  * @note   The I/O compensation cell can be used only when the device supply
  *         voltage ranges from 2.4 to 3.6 V.
  * @retval None
  */
void HAL_SBS_EnableVddIO1CompensationCell(void)
{
  SET_BIT(SBS->CCCSR, SBS_CCCSR_EN1) ;
}

/**
  * @brief  Power-down the VDD I/Os Compensation Cell.
  * @note   The I/O compensation cell can be used only when the device supply
  *         voltage ranges from 2.4 to 3.6 V.
  * @retval None
  */
void HAL_SBS_DisableVddIO1CompensationCell(void)
{
  CLEAR_BIT(SBS->CCCSR, SBS_CCCSR_EN1);
}

/**
  * @brief  Enables the VDDIO2 I/Os Compensation Cell.
  * @note   The I/O compensation cell can be used only when the device supply
  *         voltage ranges from 2.4 to 3.6 V.
  * @retval None
  */
void HAL_SBS_EnableVddIO2CompensationCell(void)
{
  SET_BIT(SBS->CCCSR, SBS_CCCSR_EN2) ;
}

/**
  * @brief  Power-down the VDDIO2 I/Os Compensation Cell.
  * @note   The I/O compensation cell can be used only when the device supply
  *         voltage ranges from 2.4 to 3.6 V.
  * @retval None
  */
void HAL_SBS_DisableVddIO2CompensationCell(void)
{
  CLEAR_BIT(SBS->CCCSR, SBS_CCCSR_EN2);
}

/**
  * @brief  Code selection for the VDD I/O Compensation cell
  * @param  SBS_CompCode: Selects the code to be applied for the I/O compensation cell
  *         This parameter can be one of the following values:
  *           @arg SBS_VDD_CELL_CODE : Select Code from the cell (available in the SBS_CCVALR)
  *           @arg SBS_VDD_REGISTER_CODE: Select Code from the SBS compensation cell code register (SBS_CCSWCR)
  * @retval None
  */
void HAL_SBS_VDDCompensationCodeSelect(uint32_t SBS_CompCode)
{
  /* Check the parameter */
  assert_param(IS_SBS_VDD_CODE_SELECT(SBS_CompCode));
  MODIFY_REG(SBS->CCCSR, SBS_CCCSR_CS1, (uint32_t)(SBS_CompCode));
}

/**
  * @brief  Code selection for the VDDIO I/O Compensation cell
  * @param  SBS_CompCode: Selects the code to be applied for the I/O compensation cell
  *         This parameter can be one of the following values:
  *           @arg SBS_VDDIO_CELL_CODE : Select Code from the cell (available in the SBS_CCVALR)
  *           @arg SBS_VDDIO_REGISTER_CODE: Select Code from the SBS compensation cell code register (SBS_CCSWCR)
  * @retval None
  */
void HAL_SBS_VDDIOCompensationCodeSelect(uint32_t SBS_CompCode)
{
  /* Check the parameter */
  assert_param(IS_SBS_VDDIO_CODE_SELECT(SBS_CompCode));
  MODIFY_REG(SBS->CCCSR, SBS_CCCSR_CS2, (uint32_t)(SBS_CompCode));
}

/**
  * @brief  VDDIO1 I/O Compensation cell get ready flag status
  * @retval State of bit (1 or 0).
  */
uint32_t HAL_SBS_GetVddIO1CompensationCellReadyFlag(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_RDY1) == SBS_CCCSR_RDY1) ? 1UL : 0UL);
}

/**
  * @brief  VDDIO2 I/O Compensation cell get ready flag status
  * @retval State of bit (1 or 0).
  */
uint32_t HAL_SBS_GetVddIO2CompensationCellReadyFlag(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_RDY2) == SBS_CCCSR_RDY2) ? 1UL : 0UL);
}

/**
  * @brief  Code configuration for the VDD I/O Compensation cell
  * @param  SBS_PMOSCode: PMOS compensation code
  *         This code is applied to the VDD I/O compensation cell when the CS1 bit of the
  *          SBS_CCSR is set
  * @param  SBS_NMOSCode: NMOS compensation code
  *         This code is applied to the VDD I/O compensation cell when the CS1 bit of the
  *          SBS_CCSR is set
  * @retval None
  */
void HAL_SBS_VDDCompensationCodeConfig(uint32_t SBS_PMOSCode, uint32_t SBS_NMOSCode)
{
  /* Check the parameter */
  assert_param(IS_SBS_CODE_CONFIG(SBS_PMOSCode));
  assert_param(IS_SBS_CODE_CONFIG(SBS_NMOSCode));
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC1 | SBS_CCSWCR_SW_APSRC1, (((uint32_t)(SBS_PMOSCode) << 4) | \
                                                                        (uint32_t)(SBS_NMOSCode)));
}

/**
  * @brief  Code configuration for the VDDIO I/O Compensation cell
  * @param  SBS_PMOSCode: PMOS compensation code
  *         This code is applied to the VDDIO I/O compensation cell when the CS2 bit of the
  *          SBS_CCSR is set
  * @param  SBS_NMOSCode: NMOS compensation code
  *         This code is applied to the VDDIO I/O compensation cell when the CS2 bit of the
  *          SBS_CCSR is set
  * @retval None
  */
void HAL_SBS_VDDIOCompensationCodeConfig(uint32_t SBS_PMOSCode, uint32_t SBS_NMOSCode)
{
  /* Check the parameter */
  assert_param(IS_SBS_CODE_CONFIG(SBS_PMOSCode));
  assert_param(IS_SBS_CODE_CONFIG(SBS_NMOSCode));
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC2 | SBS_CCSWCR_SW_APSRC2, (((uint32_t)(SBS_PMOSCode) << 12) | \
                                                                        ((uint32_t)(SBS_NMOSCode) << 8)));
}

/**
  * @brief  Get NMOS compensation value of the I/Os supplied by VDD
  * @retval None
  */
uint32_t HAL_SBS_GetNMOSVddCompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_ANSRC1));
}

/**
  * @brief  Get PMOS compensation value of the I/Os supplied by VDD
  * @retval None
  */
uint32_t HAL_SBS_GetPMOSVddCompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_APSRC1) >>  SBS_CCVALR_APSRC1_Pos);
}

/**
  * @brief  Get NMOS compensation value of the I/Os supplied by VDDIO2
  * @retval None
  */
uint32_t HAL_SBS_GetNMOSVddIO2CompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_ANSRC2) >>  SBS_CCVALR_ANSRC2_Pos);
}


/**
  * @brief  Get PMOS compensation value of the I/Os supplied by VDDIO2
  * @retval None
  */
uint32_t HAL_SBS_GetPMOSVddIO2CompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_APSRC2) >>  SBS_CCVALR_APSRC2_Pos);
}

/**
  * @brief  Disable the NMI in case of double ECC error in FLASH Interface.
  *
  * @retval None
  */
void HAL_SBS_FLASH_DisableECCNMI(void)
{
  SET_BIT(SBS->ECCNMIR, SBS_ECCNMIR_ECCNMI_MASK_EN);
}

/**
  * @brief  Enable the NMI in case of double ECC error in FLASH Interface.
  *
  * @retval None
  */
void HAL_SBS_FLASH_EnableECCNMI(void)
{
  CLEAR_BIT(SBS->ECCNMIR, SBS_ECCNMIR_ECCNMI_MASK_EN);
}

/**
  * @brief  Check if the NMI is Enabled in case of double ECC error in FLASH Interface.
  *
  * @retval State of bit (1 or 0).
  */
uint32_t HAL_SBS_FLASH_ECCNMI_IsDisabled(void)
{
  return ((READ_BIT(SBS->ECCNMIR, SBS_ECCNMIR_ECCNMI_MASK_EN) == SBS_ECCNMIR_ECCNMI_MASK_EN) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group6 HAL SBS Boot control functions
  *  @brief    HAL SBS Boot functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL SBS Boot control functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Increment the HDPL value
      (+) Get the HDPL value

@endverbatim
  * @{
  */

/**
  * @brief  Increment by 1 the HDPL value
  * @retval None
  */
void HAL_SBS_IncrementHDPLValue(void)
{
  MODIFY_REG(SBS->HDPLCR, SBS_HDPLCR_INCR_HDPL, SBS_HDPL_INCREMENT_VALUE);
}

/**
  * @brief  Get the HDPL Value.
  *
  * @retval  Returns the HDPL value
  *          This return value can be one of the following values:
  *            @arg SBS_HDPL_VALUE_0: HDPL0
  *            @arg SBS_HDPL_VALUE_1: HDPL1
  *            @arg SBS_HDPL_VALUE_2: HDPL2
  *            @arg SBS_HDPL_VALUE_3: HDPL3
  */
uint32_t HAL_SBS_GetHDPLValue(void)
{
  return (uint32_t)(READ_BIT(SBS->HDPLSR, SBS_HDPLSR_HDPL));
}

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group7 HAL SBS Hardware secure storage control functions
  *  @brief    HAL SBS Hardware secure storage functions
  *
@verbatim
 =======================================================================================================================
                                       ##### HAL SBS Hardware secure storage control functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Select EPOCH security sent to SAES IP
      (+) Set/Get EPOCH security selection
      (+) Set/Get the OBK-HDPL Value

@endverbatim
  * @{
  */

#if defined(SBS_EPOCHSELCR_EPOCH_SEL)
/**
  * @brief  Select EPOCH security sent to SAES IP to encrypt/decrypt keys
  * @param  Epoch_Selection: Select EPOCH security
  *         This parameter can be one of the following values:
  *           @arg SBS_EPOCH_SEL_SECURE    : EPOCH secure selected.
  *           @arg SBS_EPOCH_SEL_NONSECURE : EPOCH non secure selected.
  *           @arg SBS_EPOCH_SEL_PUFCHECK  : EPOCH all zeros for PUF integrity check.
  * @retval None
  */
void HAL_SBS_EPOCHSelection(uint32_t Epoch_Selection)
{
  /* Check the parameter */
  assert_param(IS_SBS_EPOCH_SELECTION(Epoch_Selection));

  MODIFY_REG(SBS->EPOCHSELCR, SBS_EPOCHSELCR_EPOCH_SEL, (uint32_t)(Epoch_Selection));
}

/**
  * @brief  Get EPOCH security selection
  * @retval Returned value can be one of the following values:
  *           @arg SBS_EPOCH_SEL_SECURE    : EPOCH secure selected.
  *           @arg SBS_EPOCH_SEL_NONSECURE : EPOCH non secure selected.
  *           @arg SBS_EPOCH_SEL_PUFCHECK  : EPOCH all zeros for PUF integrity check.
  */
uint32_t HAL_SBS_GetEPOCHSelection(void)
{
  return (uint32_t)(READ_BIT(SBS->EPOCHSELCR, SBS_EPOCHSELCR_EPOCH_SEL));
}
#endif /* SBS_EPOCHSELCR_EPOCH_SEL */

#if defined(SBS_NEXTHDPLCR_NEXTHDPL)
/**
  * @brief  Set the OBK-HDPL Value.
  * @param  OBKHDPL_Value Value of the increment to add to HDPL value to generate the OBK-HDPL.
  *         This parameter can be one of the following values:
  *           @arg SBS_OBKHDPL_INCR_0 : HDPL
  *           @arg SBS_OBKHDPL_INCR_1 : HDPL + 1
  *           @arg SBS_OBKHDPL_INCR_2 : HDPL + 2
  *           @arg SBS_OBKHDPL_INCR_3 : HDPL + 3
  * @retval None
  */
void HAL_SBS_SetOBKHDPL(uint32_t OBKHDPL_Value)
{
  /* Check the parameter */
  assert_param(IS_SBS_OBKHDPL_SELECTION(OBKHDPL_Value));

  MODIFY_REG(SBS->NEXTHDPLCR, SBS_NEXTHDPLCR_NEXTHDPL, (uint32_t)(OBKHDPL_Value));
}

/**
  * @brief  Get the OBK-HDPL Value.
  * @retval  Returns the incremement to add to HDPL value to generate OBK-HDPL
  *          This return value can be one of the following values:
  *            @arg SBS_OBKHDPL_INCR_0: HDPL
  *            @arg SBS_OBKHDPL_INCR_1: HDPL + 1
  *            @arg SBS_OBKHDPL_INCR_2: HDPL + 2
  *            @arg SBS_OBKHDPL_INCR_3: HDPL + 3
  */
uint32_t HAL_SBS_GetOBKHDPL(void)
{
  return (uint32_t)(READ_BIT(SBS->NEXTHDPLCR, SBS_NEXTHDPLCR_NEXTHDPL));
}
#endif /* SBS_NEXTHDPLCR_NEXTHDPL */

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group8 HAL SBS Debug control functions
  *  @brief    HAL SBS Debug functions
  *
@verbatim
 =======================================================================================================================
                                       ##### SBS Debug control functions #####
 =======================================================================================================================
    [..]  This section provides functions allowing to:
      (+) Open the device access port
      (+) Open the debug
      (+) Configure the authenticated debug HDPL
      (+) Get the current value of the hide protection level
      (+) Lock the access to the debug control register
      (+) Configure/Get the authenticated debug security access

@endverbatim
  * @{
  */

/**
  * @brief  Open the device access port.
  * @note   This function can be only used when device state is Closed.
  * @retval None
  */
void HAL_SBS_OpenAccessPort(void)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_AP_UNLOCK, SBS_DEBUG_UNLOCK_VALUE);
}

/**
  * @brief  Open the debug when the hide protection level is authorized.
  * @note   This function can be only used when device state is Closed.
  * @retval None
  */
void HAL_SBS_OpenDebug(void)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_UNLOCK, ((uint32_t)SBS_DEBUG_UNLOCK_VALUE << SBS_DBGCR_DBG_UNLOCK_Pos));
}

/**
  * @brief  Configure the authenticated debug hide protection level.
  * @note   This function can be only used when device state is Closed.
  * @param  Level Hide protection level where the authenticated debug opens
  *            This value is one of @ref SBS_HDPL_Value (except SBS_HDPL_VALUE_0)
  * @retval HAL_OK if parameter is correct
  *         HAL_ERROR otherwise
  */
HAL_StatusTypeDef HAL_SBS_ConfigDebugLevel(uint32_t Level)
{
  /* Check the parameter */
  assert_param(IS_SBS_HDPL(Level));

  if (Level != SBS_HDPL_VALUE_0)
  {
    MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_AUTH_HDPL, (Level << SBS_DBGCR_DBG_AUTH_HDPL_Pos));
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Get the current value of the hide protection level.
  * @note   This function can be only used when device state is Closed.
  * @retval Current hide protection level
  *            This value is one of @ref SBS_HDPL_Value
  */
uint32_t HAL_SBS_GetDebugLevel(void)
{
  return ((SBS->DBGCR & SBS_DBGCR_DBG_AUTH_HDPL) >> SBS_DBGCR_DBG_AUTH_HDPL_Pos);
}

/**
  * @brief  Lock the access to the debug control register.
  * @note   This function can be only used when device state is Closed.
  * @note   locking the current debug configuration is released only by a reset.
  * @retval None
  */
void HAL_SBS_LockDebugConfig(void)
{
  MODIFY_REG(SBS->DBGLOCKR, SBS_DBGLOCKR_DBGCFG_LOCK, SBS_DEBUG_LOCK_VALUE);
}

#if defined(SBS_DBGCR_DBG_AUTH_SEC)
/**
  * @brief  Configure the authenticated debug security access.
  * @param  Security debug opening secure/non-secure or non-secure only
  *         This parameter can be one of the following values:
  *            @arg SBS_DEBUG_SEC_NSEC: debug opening for secure and non-secure.
  *            @arg SBS_DEBUG_NSEC: debug opening for non-secure only.
  * @retval None
  */
void HAL_SBS_ConfigDebugSecurity(uint32_t Security)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_AUTH_SEC, (Security << SBS_DBGCR_DBG_AUTH_SEC_Pos));
}

/**
  * @brief  Get the current value of the hide protection level.
  * @note   This function can be only used when device state is Closed.
  * @retval Returned value can be one of the following values:
  *            @arg SBS_DEBUG_SEC_NSEC: debug opening for secure and non-secure.
  *            @arg SBS_DEBUG_NSEC: debug opening for non-secure only.
  */
uint32_t HAL_SBS_GetDebugSecurity(void)
{
  return ((SBS->DBGCR & SBS_DBGCR_DBG_AUTH_SEC) >> SBS_DBGCR_DBG_AUTH_SEC_Pos);
}
#endif /* SBS_DBGCR_DBG_AUTH_SEC */

#if defined(SBS_OTGHSPHYTUNER2_COMPDISTUNE)
/**
  * @brief  Set the OTG PHY Disconnect Threshold.
  * @param  DisconnectThreshold Defines the voltage level for the threshold used to detect a disconnect event.
  *         This parameter can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_DISCONNECT_5_9PERCENT: +5.9% (recommended value)
  *            @arg SBS_OTG_HS_PHY_DISCONNECT_0PERCENT: 0% (default value)
  * @retval None
  */

void HAL_SBS_SetOTGPHYDisconnectThreshold(uint32_t DisconnectThreshold)
{
  /* Check the parameter */
  assert_param(IS_SBS_OTGPHY_DISCONNECT(DisconnectThreshold));

  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_COMPDISTUNE, DisconnectThreshold);
}

/**
  * @brief  Get the current voltage level for the threshold used to detect a disconnect event.
  * @retval The returned value can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_DISCONNECT_5_9PERCENT: +5.9% (recommended value)
  *            @arg SBS_OTG_HS_PHY_DISCONNECT_0PERCENT: 0% (default value)
  */
uint32_t HAL_SBS_GetOTGPHYDisconnectThreshold(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_COMPDISTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_COMPDISTUNE */

#if defined(SBS_OTGHSPHYTUNER2_SQRXTUNE)
/**
  * @brief  Adjust the voltage level for the threshold used to detect valid high speed data.
  * @param  SquelchThreshold Defines the voltage level.
  *          This parameter can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_SQUELCH_15PERCENT: +15% (recommended value)
  *            @arg SBS_OTG_HS_PHY_SQUELCH_0PERCENT: 0% (default value)
  * @retval None
  */

void HAL_SBS_SetOTGPHYSquelchThreshold(uint32_t SquelchThreshold)
{
  /* Check the parameter */
  assert_param(IS_SBS_OTGPHY_SQUELCH(SquelchThreshold));

  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_SQRXTUNE, SquelchThreshold);
}

/**
  * @brief  Get the current voltage level for the threshold used to detect a squelch event.
  * @retval The returned value can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_SQUELCH_15PERCENT: +15% (recommended value)
  *            @arg SBS_OTG_HS_PHY_SQUELCH_0PERCENT: 0% (default value)
  */
uint32_t HAL_SBS_GetOTGPHYSquelchThreshold(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_SQRXTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_SQRXTUNE */

#if defined(SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE)
/**
  * @brief  Set the OTG PHY Current config.
  * @param  PreemphasisCurrent Defines the current configuration.
  *         This parameter can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_PREEMP_DISABLED: HS transmitter preemphasis circuit disabled
  *            @arg SBS_OTG_HS_PHY_PREEMP_1X: HS transmitter preemphasis circuit sources 1x preemphasis current
  *            @arg SBS_OTG_HS_PHY_PREEMP_2X: HS transmitter preemphasis circuit sources 2x preemphasis current
  *            @arg SBS_OTG_HS_PHY_PREEMP_3X: HS transmitter preemphasis circuit sources 3x preemphasis current
  * @retval None
  */

void HAL_SBS_SetOTGPHYPreemphasisCurrent(uint32_t PreemphasisCurrent)
{
  /* Check the parameter */
  assert_param(IS_SBS_OTGPHY_PREEMPHASIS(PreemphasisCurrent));

  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE, PreemphasisCurrent);
}

/**
  * @brief  Get the current level for the preemphasis.
  * @retval The returned value can be one of the following values:
  *            @arg SBS_OTG_HS_PHY_PREEMP_DISABLED: HS transmitter preemphasis circuit disabled
  *            @arg SBS_OTG_HS_PHY_PREEMP_1X: HS transmitter preemphasis circuit sources 1x preemphasis current
  *            @arg SBS_OTG_HS_PHY_PREEMP_2X: HS transmitter preemphasis circuit sources 2x preemphasis current
  *            @arg SBS_OTG_HS_PHY_PREEMP_3X: HS transmitter preemphasis circuit sources 3x preemphasis current
  */
uint32_t HAL_SBS_GetOTGPHYPreemphasisCurrent(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE */

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group9 HAL SBS lock management functions
  *  @brief SBS lock management functions.
  *
@verbatim
 =======================================================================================================================
                                        ##### SBS lock functions #####
 =======================================================================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Lock the SBS item(s).
  * @note   Setting lock(s) depends on privilege mode in secure/non-secure code
  *         Lock(s) cleared only at system reset
  * @param  Item Item(s) to set lock on.
  *         This parameter can be a combination of @ref SBS_Lock_items
  * @retval None
  */
void HAL_SBS_Lock(uint32_t Item)
{
  /* Check the parameters */
  assert_param(IS_SBS_LOCK_ITEMS(Item));

  /* Privilege secure/non-secure locks */
  SBS->CNSLCKR = (0xFFFFU & Item);  /* non-secure lock item in 16 lowest bits */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Privilege secure only locks */
  SBS->CSLCKR = ((0xFFFF0000U & Item) >> 16U);  /* Secure-only lock item in 16 highest bits */
#endif /* __ARM_FEATURE_CMSE */
}

/**
  * @brief  Get the lock state of SBS items.
  * @note   Getting lock(s) depends on privilege mode in secure/non-secure code
  * @param  pItem pointer to return locked items
  *         the return value can be a combination of @ref SBS_Lock_items
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SBS_GetLock(uint32_t *pItem)
{
  uint32_t tmp_lock;

  /* Check null pointer */
  if (pItem == NULL)
  {
    return HAL_ERROR;
  }

  /* Get the non-secure lock state */
  tmp_lock = SBS->CNSLCKR;

  /* Get the secure lock state in secure code */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  tmp_lock |= (SBS->CSLCKR << 16U);
#endif /* __ARM_FEATURE_CMSE */

  /* Return overall lock status */
  *pItem = tmp_lock;

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup HAL_Exported_Functions_Group10 HAL SBS attributes management functions
  *  @brief SBS attributes management functions.
  *
@verbatim
 =======================================================================================================================
                                        ##### SBS attributes functions #####
 =======================================================================================================================

@endverbatim
  * @{
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure the SBS item attribute(s).
  * @note   Available attributes are to secure SBS items, so this function is
  *         only available in secure.
  *         SBS_FPU item attribute is only configurable through PRIVILEGE transaction.
  * @param  Item Item(s) to set attributes on.
  *         This parameter can be a one or a combination of @ref SBS_Attributes_items
  * @param  Attributes  specifies the secure/non-secure attributes.
  * @retval None
  */
void HAL_SBS_ConfigAttributes(uint32_t Item, uint32_t Attributes)
{
  uint32_t tmp;

  /* Check the parameters */
  assert_param(IS_SBS_ITEMS_ATTRIBUTES(Item));
  assert_param(IS_SBS_ATTRIBUTES(Attributes));

  tmp = SBS->SECCFGR;

  /* Set or reset Item */
  if ((Attributes & SBS_SEC) != 0x00U)
  {
    tmp |= Item;
  }
  else
  {
    tmp &= ~Item;
  }

  /* Set secure attributes */
  SBS->SECCFGR = tmp;
}


/**
  * @brief  Get the attribute of a SBS items.
  * @note   Available attributes have read restrictions, so this function is
  *         only available in secure
  * @param  Item Single item to get secure/non-secure attribute from.
  * @param  pAttributes pointer to return the attribute.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SBS_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes)
{
  /* Check null pointer */
  if (pAttributes == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_SBS_ITEMS_ATTRIBUTES(Item));

  /* Get the secure attribute state */
  if ((SBS->SECCFGR & Item) != 0U)
  {
    *pAttributes = SBS_SEC;
  }
  else
  {
    *pAttributes = SBS_NSEC;
  }

  return HAL_OK;
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

