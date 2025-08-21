/**
  ******************************************************************************
  * @file    stm32f7xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extension RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extension peripheral:
  *           + Extended Peripheral Control functions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup RCCEx RCCEx
  * @brief RCCEx HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Defines RCCEx Private Defines
  * @{
  */
/**
  * @}
  */
/* Private macro -------------------------------------------------------------*/
/** @defgroup RCCEx_Private_Macros RCCEx Private Macros
 * @{
 */
/**
  * @}
  */

/** @defgroup RCCEx_Private_Macros RCCEx Private Macros
 * @{
 */

/**
  * @}
  */


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Functions RCCEx Exported Functions
  * @{
  */

/** @defgroup RCCEx_Exported_Functions_Group1 Extended Peripheral Control functions
 *  @brief  Extended Peripheral Control functions
 *
@verbatim
 ===============================================================================
                ##### Extended Peripheral Control functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the RCC Clocks
    frequencies.
    [..]
    (@) Important note: Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to
        select the RTC clock source; in this case the Backup domain will be reset in
        order to modify the RTC Clock source, as consequence RTC registers (including
        the backup registers) and RCC_BDCR register will be set to their reset values.

@endverbatim
  * @{
  */
#if defined (STM32F745xx) || defined (STM32F746xx) || defined (STM32F756xx) || defined (STM32F765xx) || \
    defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx) || \
    defined (STM32F750xx)
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, LTDC, RTC, TIM, UARTs, USARTs, LTPIM, SDMMC...).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0;
  uint32_t tmpreg0 = 0;
  uint32_t tmpreg1 = 0;
  uint32_t plli2sused = 0;
  uint32_t pllsaiused = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));

  /*----------------------------------- I2S configuration ----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == (RCC_PERIPHCLK_I2S))
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2SCLKSOURCE(PeriphClkInit->I2sClockSelection));

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_CONFIG(PeriphClkInit->I2sClockSelection);

    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sClockSelection == RCC_I2SCLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
  }

  /*------------------------------------ SAI1 configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == (RCC_PERIPHCLK_SAI1))
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI1CLKSOURCE(PeriphClkInit->Sai1ClockSelection));

    /* Configure SAI1 Clock source */
    __HAL_RCC_SAI1_CONFIG(PeriphClkInit->Sai1ClockSelection);
    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)
    {
      pllsaiused = 1;
    }
  }

  /*------------------------------------ SAI2 configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == (RCC_PERIPHCLK_SAI2))
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI2CLKSOURCE(PeriphClkInit->Sai2ClockSelection));

    /* Configure SAI2 Clock source */
    __HAL_RCC_SAI2_CONFIG(PeriphClkInit->Sai2ClockSelection);

    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)
    {
      pllsaiused = 1;
    }
  }

  /*-------------------------------------- SPDIF-RX Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPDIFRX) == RCC_PERIPHCLK_SPDIFRX)
  {
      plli2sused = 1;
  }

  /*------------------------------------ RTC configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
  {
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Enable write access to Backup domain */
    PWR->CR1 |= PWR_CR1_DBP;

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait for Backup domain Write protection disable */
    while((PWR->CR1 & PWR_CR1_DBP) == RESET)
    {
      if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Reset the Backup domain only if the RTC Clock source selection is modified */
    tmpreg0 = (RCC->BDCR & RCC_BDCR_RTCSEL);

    if((tmpreg0 != 0x00000000U) && (tmpreg0 != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))
    {
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg0 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));

      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();

      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg0;

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);
  }

  /*------------------------------------ TIM configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIMPRES(PeriphClkInit->TIMPresSelection));

    /* Configure Timer Prescaler */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);
  }

  /*-------------------------------------- I2C1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));

    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

  /*-------------------------------------- I2C2 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(PeriphClkInit->I2c2ClockSelection));

    /* Configure the I2C2 clock source */
    __HAL_RCC_I2C2_CONFIG(PeriphClkInit->I2c2ClockSelection);
  }

  /*-------------------------------------- I2C3 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C3) == RCC_PERIPHCLK_I2C3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C3CLKSOURCE(PeriphClkInit->I2c3ClockSelection));

    /* Configure the I2C3 clock source */
    __HAL_RCC_I2C3_CONFIG(PeriphClkInit->I2c3ClockSelection);
  }

  /*-------------------------------------- I2C4 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C4) == RCC_PERIPHCLK_I2C4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C4CLKSOURCE(PeriphClkInit->I2c4ClockSelection));

    /* Configure the I2C4 clock source */
    __HAL_RCC_I2C4_CONFIG(PeriphClkInit->I2c4ClockSelection);
  }

  /*-------------------------------------- USART1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));

    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }

  /*-------------------------------------- USART2 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));

    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }

  /*-------------------------------------- USART3 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));

    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }

  /*-------------------------------------- UART4 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART4) == RCC_PERIPHCLK_UART4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART4CLKSOURCE(PeriphClkInit->Uart4ClockSelection));

    /* Configure the UART4 clock source */
    __HAL_RCC_UART4_CONFIG(PeriphClkInit->Uart4ClockSelection);
  }

  /*-------------------------------------- UART5 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART5) == RCC_PERIPHCLK_UART5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART5CLKSOURCE(PeriphClkInit->Uart5ClockSelection));

    /* Configure the UART5 clock source */
    __HAL_RCC_UART5_CONFIG(PeriphClkInit->Uart5ClockSelection);
  }

  /*-------------------------------------- USART6 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART6) == RCC_PERIPHCLK_USART6)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART6CLKSOURCE(PeriphClkInit->Usart6ClockSelection));

    /* Configure the USART6 clock source */
    __HAL_RCC_USART6_CONFIG(PeriphClkInit->Usart6ClockSelection);
  }

  /*-------------------------------------- UART7 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART7) == RCC_PERIPHCLK_UART7)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART7CLKSOURCE(PeriphClkInit->Uart7ClockSelection));

    /* Configure the UART7 clock source */
    __HAL_RCC_UART7_CONFIG(PeriphClkInit->Uart7ClockSelection);
  }

  /*-------------------------------------- UART8 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART8) == RCC_PERIPHCLK_UART8)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART8CLKSOURCE(PeriphClkInit->Uart8ClockSelection));

    /* Configure the UART8 clock source */
    __HAL_RCC_UART8_CONFIG(PeriphClkInit->Uart8ClockSelection);
  }

  /*--------------------------------------- CEC Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(PeriphClkInit->CecClockSelection));

    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(PeriphClkInit->CecClockSelection);
  }

  /*-------------------------------------- CK48 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CLK48SOURCE(PeriphClkInit->Clk48ClockSelection));

    /* Configure the CLK48 source */
    __HAL_RCC_CLK48_CONFIG(PeriphClkInit->Clk48ClockSelection);

    /* Enable the PLLSAI when it's used as clock source for CK48 */
    if(PeriphClkInit->Clk48ClockSelection == RCC_CLK48SOURCE_PLLSAIP)
    {
      pllsaiused = 1;
    }
  }

  /*-------------------------------------- LTDC Configuration -----------------------------------*/
#if defined(STM32F746xx) || defined(STM32F756xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx) || defined (STM32F750xx)
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == RCC_PERIPHCLK_LTDC)
  {
    pllsaiused = 1;
  }
#endif /* STM32F746xx || STM32F756xx || STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx || STM32F750xx */

  /*-------------------------------------- LPTIM1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == RCC_PERIPHCLK_LPTIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1CLK(PeriphClkInit->Lptim1ClockSelection));

    /* Configure the LTPIM1 clock source */
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);
   }

  /*------------------------------------- SDMMC1 Configuration ------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC1) == RCC_PERIPHCLK_SDMMC1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC1CLKSOURCE(PeriphClkInit->Sdmmc1ClockSelection));

    /* Configure the SDMMC1 clock source */
    __HAL_RCC_SDMMC1_CONFIG(PeriphClkInit->Sdmmc1ClockSelection);
  }

#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)
  /*------------------------------------- SDMMC2 Configuration ------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC2) == RCC_PERIPHCLK_SDMMC2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC2CLKSOURCE(PeriphClkInit->Sdmmc2ClockSelection));

    /* Configure the SDMMC2 clock source */
    __HAL_RCC_SDMMC2_CONFIG(PeriphClkInit->Sdmmc2ClockSelection);
  }

  /*------------------------------------- DFSDM1 Configuration -------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM1) == RCC_PERIPHCLK_DFSDM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM1CLKSOURCE(PeriphClkInit->Dfsdm1ClockSelection));

    /* Configure the DFSDM1 interface clock source */
    __HAL_RCC_DFSDM1_CONFIG(PeriphClkInit->Dfsdm1ClockSelection);
  }

  /*------------------------------------- DFSDM AUDIO Configuration -------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DFSDM1_AUDIO) == RCC_PERIPHCLK_DFSDM1_AUDIO)
  {
    /* Check the parameters */
    assert_param(IS_RCC_DFSDM1AUDIOCLKSOURCE(PeriphClkInit->Dfsdm1AudioClockSelection));

    /* Configure the DFSDM interface clock source */
    __HAL_RCC_DFSDM1AUDIO_CONFIG(PeriphClkInit->Dfsdm1AudioClockSelection);
  }
#endif /* STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */

  /*-------------------------------------- PLLI2S Configuration ---------------------------------*/
  /* PLLI2S is configured when a peripheral will use it as source clock : SAI1, SAI2, I2S or SPDIF-RX */
  if((plli2sused == 1) || ((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))
  {
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }

    /* check for common PLLI2S Parameters */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));

    /*----------------- In Case of PLLI2S is selected as source clock for I2S -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) && (PeriphClkInit->I2sClockSelection == RCC_I2SCLKSOURCE_PLLI2S)))
    {
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));

      /* Read PLLI2SP and PLLI2SQ value from PLLI2SCFGR register (this value is not needed for I2S configuration) */
      tmpreg0 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos);
      tmpreg1 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLM) */
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , tmpreg0, tmpreg1, PeriphClkInit->PLLI2S.PLLI2SR);
    }

    /*----------------- In Case of PLLI2S is selected as source clock for SAI -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)) ||
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)))
    {
      /* Check for PLLI2S Parameters */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));
      /* Check for PLLI2S/DIVQ parameters */
      assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(PeriphClkInit->PLLI2SDivQ));

      /* Read PLLI2SP and PLLI2SR values from PLLI2SCFGR register (this value is not needed for SAI configuration) */
      tmpreg0 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos);
      tmpreg1 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN, tmpreg0, PeriphClkInit->PLLI2S.PLLI2SQ, tmpreg1);

      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLI2SDivQ);
    }

    /*----------------- In Case of PLLI2S is selected as source clock for SPDIF-RX -------------------*/
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPDIFRX) == RCC_PERIPHCLK_SPDIFRX)
    {
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SP_VALUE(PeriphClkInit->PLLI2S.PLLI2SP));

     /* Read PLLI2SR value from PLLI2SCFGR register (this value is not needed for SPDIF-RX configuration) */
      tmpreg0 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
      tmpreg1 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLM) */
      /* SPDIFCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SP */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SP, tmpreg0, tmpreg1);
    }

    /*----------------- In Case of PLLI2S is just selected  -----------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)
    {
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SP_VALUE(PeriphClkInit->PLLI2S.PLLI2SP));
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));

      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLI2SM) */
      /* SPDIFRXCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SP */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SP, PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);
    }

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }
  }

  /*-------------------------------------- PLLSAI Configuration ---------------------------------*/
  /* PLLSAI is configured when a peripheral will use it as source clock : SAI1, SAI2, LTDC or CK48 */
  if(pllsaiused == 1)
  {
    /* Disable PLLSAI Clock */
    __HAL_RCC_PLLSAI_DISABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI is disabled */
    while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }

    /* Check the PLLSAI division factors */
    assert_param(IS_RCC_PLLSAIN_VALUE(PeriphClkInit->PLLSAI.PLLSAIN));

    /*----------------- In Case of PLLSAI is selected as source clock for SAI -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)) ||\
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)))
    {
      /* check for PLLSAIQ Parameter */
      assert_param(IS_RCC_PLLSAIQ_VALUE(PeriphClkInit->PLLSAI.PLLSAIQ));
      /* check for PLLSAI/DIVQ Parameter */
      assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(PeriphClkInit->PLLSAIDivQ));

      /* Read PLLSAIP value from PLLSAICFGR register (this value is not needed for SAI configuration) */
      tmpreg0 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos);
      tmpreg1 = ((RCC->PLLSAICFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , tmpreg0, PeriphClkInit->PLLSAI.PLLSAIQ, tmpreg1);

      /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLSAIDivQ);
    }

    /*----------------- In Case of PLLSAI is selected as source clock for CLK48 -------------------*/
    /* In Case of PLLI2S is selected as source clock for CK48 */
    if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48) && (PeriphClkInit->Clk48ClockSelection == RCC_CLK48SOURCE_PLLSAIP))
    {
      /* check for Parameters */
      assert_param(IS_RCC_PLLSAIP_VALUE(PeriphClkInit->PLLSAI.PLLSAIP));
      /* Read PLLSAIQ and PLLSAIR value from PLLSAICFGR register (this value is not needed for CK48 configuration) */
      tmpreg0 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);
      tmpreg1 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);

      /* Configure the PLLSAI division factors */
      /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) x (PLLI2SN/PLLM) */
      /* 48CLK = f(PLLSAI clock output) = f(VCO clock) / PLLSAIP */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , PeriphClkInit->PLLSAI.PLLSAIP, tmpreg0, tmpreg1);
    }

#if defined(STM32F746xx) || defined(STM32F756xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx) || defined (STM32F750xx)
    /*---------------------------- LTDC configuration -------------------------------*/
    if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == (RCC_PERIPHCLK_LTDC))
    {
      assert_param(IS_RCC_PLLSAIR_VALUE(PeriphClkInit->PLLSAI.PLLSAIR));
      assert_param(IS_RCC_PLLSAI_DIVR_VALUE(PeriphClkInit->PLLSAIDivR));

      /* Read PLLSAIP and PLLSAIQ value from PLLSAICFGR register (these value are not needed for LTDC configuration) */
      tmpreg0 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);
      tmpreg1 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos);

      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* LTDC_CLK(first level) = PLLSAI_VCO Output/PLLSAIR */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , tmpreg1, tmpreg0, PeriphClkInit->PLLSAI.PLLSAIR);

      /* LTDC_CLK = LTDC_CLK(first level)/PLLSAIDIVR */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVR_CONFIG(PeriphClkInit->PLLSAIDivR);
    }
#endif /* STM32F746xx || STM32F756xx || STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx || STM32F750xx  */

    /* Enable PLLSAI Clock */
    __HAL_RCC_PLLSAI_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI is ready */
    while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  Get the RCC_PeriphCLKInitTypeDef according to the internal
  *         RCC configuration registers.
  * @param  PeriphClkInit pointer to the configured RCC_PeriphCLKInitTypeDef structure
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tempreg = 0;

  /* Set all possible values for the extended clock type parameter------------*/
#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S      | RCC_PERIPHCLK_LPTIM1   |\
                                        RCC_PERIPHCLK_SAI1     | RCC_PERIPHCLK_SAI2     |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_CEC      | RCC_PERIPHCLK_I2C4     |\
                                        RCC_PERIPHCLK_I2C1     | RCC_PERIPHCLK_I2C2     |\
                                        RCC_PERIPHCLK_I2C3     | RCC_PERIPHCLK_USART1   |\
                                        RCC_PERIPHCLK_USART2   | RCC_PERIPHCLK_USART3   |\
                                        RCC_PERIPHCLK_UART4    | RCC_PERIPHCLK_UART5    |\
                                        RCC_PERIPHCLK_USART6   | RCC_PERIPHCLK_UART7    |\
                                        RCC_PERIPHCLK_UART8    | RCC_PERIPHCLK_SDMMC1   |\
                                        RCC_PERIPHCLK_CLK48    | RCC_PERIPHCLK_SDMMC2   |\
                                        RCC_PERIPHCLK_DFSDM1   | RCC_PERIPHCLK_DFSDM1_AUDIO;
#else
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S      | RCC_PERIPHCLK_LPTIM1   |\
                                        RCC_PERIPHCLK_SAI1     | RCC_PERIPHCLK_SAI2     |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_CEC      | RCC_PERIPHCLK_I2C4     |\
                                        RCC_PERIPHCLK_I2C1     | RCC_PERIPHCLK_I2C2     |\
                                        RCC_PERIPHCLK_I2C3     | RCC_PERIPHCLK_USART1   |\
                                        RCC_PERIPHCLK_USART2   | RCC_PERIPHCLK_USART3   |\
                                        RCC_PERIPHCLK_UART4    | RCC_PERIPHCLK_UART5    |\
                                        RCC_PERIPHCLK_USART6   | RCC_PERIPHCLK_UART7    |\
                                        RCC_PERIPHCLK_UART8    | RCC_PERIPHCLK_SDMMC1   |\
                                        RCC_PERIPHCLK_CLK48;
#endif /* STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */

  /* Get the PLLI2S Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);
  PeriphClkInit->PLLI2S.PLLI2SP = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SP) >> RCC_PLLI2SCFGR_PLLI2SP_Pos);
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);

  /* Get the PLLSAI Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);
  PeriphClkInit->PLLSAI.PLLSAIP = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos);
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);
  PeriphClkInit->PLLSAI.PLLSAIR = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIR) >> RCC_PLLSAICFGR_PLLSAIR_Pos);

  /* Get the PLLSAI/PLLI2S division factors -------------------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLI2SDIVQ) >> RCC_DCKCFGR1_PLLI2SDIVQ_Pos);
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLSAIDIVQ) >> RCC_DCKCFGR1_PLLSAIDIVQ_Pos);
  PeriphClkInit->PLLSAIDivR = (uint32_t)((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLSAIDIVR) >> RCC_DCKCFGR1_PLLSAIDIVR_Pos);

  /* Get the SAI1 clock configuration ----------------------------------------------*/
  PeriphClkInit->Sai1ClockSelection = __HAL_RCC_GET_SAI1_SOURCE();

  /* Get the SAI2 clock configuration ----------------------------------------------*/
  PeriphClkInit->Sai2ClockSelection = __HAL_RCC_GET_SAI2_SOURCE();

  /* Get the I2S clock configuration ------------------------------------------*/
  PeriphClkInit->I2sClockSelection = __HAL_RCC_GET_I2SCLKSOURCE();

  /* Get the I2C1 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c1ClockSelection = __HAL_RCC_GET_I2C1_SOURCE();

  /* Get the I2C2 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c2ClockSelection = __HAL_RCC_GET_I2C2_SOURCE();

  /* Get the I2C3 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c3ClockSelection = __HAL_RCC_GET_I2C3_SOURCE();

  /* Get the I2C4 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c4ClockSelection = __HAL_RCC_GET_I2C4_SOURCE();

  /* Get the USART1 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection = __HAL_RCC_GET_USART1_SOURCE();

  /* Get the USART2 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE();

  /* Get the USART3 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart3ClockSelection = __HAL_RCC_GET_USART3_SOURCE();

  /* Get the UART4 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart4ClockSelection = __HAL_RCC_GET_UART4_SOURCE();

  /* Get the UART5 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart5ClockSelection = __HAL_RCC_GET_UART5_SOURCE();

  /* Get the USART6 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart6ClockSelection = __HAL_RCC_GET_USART6_SOURCE();

  /* Get the UART7 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart7ClockSelection = __HAL_RCC_GET_UART7_SOURCE();

  /* Get the UART8 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart8ClockSelection = __HAL_RCC_GET_UART8_SOURCE();

  /* Get the LPTIM1 clock configuration ------------------------------------------*/
  PeriphClkInit->Lptim1ClockSelection = __HAL_RCC_GET_LPTIM1_SOURCE();

  /* Get the CEC clock configuration -----------------------------------------------*/
  PeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();

  /* Get the CK48 clock configuration -----------------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();

  /* Get the SDMMC1 clock configuration -----------------------------------------------*/
  PeriphClkInit->Sdmmc1ClockSelection = __HAL_RCC_GET_SDMMC1_SOURCE();

#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)
  /* Get the SDMMC2 clock configuration -----------------------------------------------*/
  PeriphClkInit->Sdmmc2ClockSelection = __HAL_RCC_GET_SDMMC2_SOURCE();

  /* Get the DFSDM clock configuration -----------------------------------------------*/
  PeriphClkInit->Dfsdm1ClockSelection = __HAL_RCC_GET_DFSDM1_SOURCE();

  /* Get the DFSDM AUDIO clock configuration -----------------------------------------------*/
  PeriphClkInit->Dfsdm1AudioClockSelection = __HAL_RCC_GET_DFSDM1AUDIO_SOURCE();
#endif /* STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */

  /* Get the RTC Clock configuration -----------------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));

  /* Get the TIM Prescaler configuration --------------------------------------------*/
  if ((RCC->DCKCFGR1 & RCC_DCKCFGR1_TIMPRE) == RESET)
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;
  }
  else
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  }
}
#endif /* STM32F745xx || STM32F746xx || STM32F756xx || STM32F765xx || STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx || STM32F750xx */

#if defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) || defined (STM32F730xx)
/**
  * @brief  Initializes the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  PeriphClkInit pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains the configuration information for the Extended Peripherals
  *         clocks(I2S, SAI, RTC, TIM, UARTs, USARTs, LTPIM, SDMMC...).
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source; in this case the Backup domain will be reset in
  *         order to modify the RTC Clock source, as consequence RTC registers (including
  *         the backup registers) are set to their reset values.
  *
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tickstart = 0;
  uint32_t tmpreg0 = 0;
  uint32_t plli2sused = 0;
  uint32_t pllsaiused = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClkInit->PeriphClockSelection));

  /*----------------------------------- I2S configuration ----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == (RCC_PERIPHCLK_I2S))
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2SCLKSOURCE(PeriphClkInit->I2sClockSelection));

    /* Configure I2S Clock source */
    __HAL_RCC_I2S_CONFIG(PeriphClkInit->I2sClockSelection);

    /* Enable the PLLI2S when it's used as clock source for I2S */
    if(PeriphClkInit->I2sClockSelection == RCC_I2SCLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
  }

  /*------------------------------------ SAI1 configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == (RCC_PERIPHCLK_SAI1))
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI1CLKSOURCE(PeriphClkInit->Sai1ClockSelection));

    /* Configure SAI1 Clock source */
    __HAL_RCC_SAI1_CONFIG(PeriphClkInit->Sai1ClockSelection);
    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)
    {
      pllsaiused = 1;
    }
  }

  /*------------------------------------ SAI2 configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == (RCC_PERIPHCLK_SAI2))
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI2CLKSOURCE(PeriphClkInit->Sai2ClockSelection));

    /* Configure SAI2 Clock source */
    __HAL_RCC_SAI2_CONFIG(PeriphClkInit->Sai2ClockSelection);

    /* Enable the PLLI2S when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)
    {
      plli2sused = 1;
    }
    /* Enable the PLLSAI when it's used as clock source for SAI */
    if(PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)
    {
      pllsaiused = 1;
    }
  }

  /*------------------------------------ RTC configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == (RCC_PERIPHCLK_RTC))
  {
    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(PeriphClkInit->RTCClockSelection));

    /* Enable Power Clock*/
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Enable write access to Backup domain */
    PWR->CR1 |= PWR_CR1_DBP;

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait for Backup domain Write protection disable */
    while((PWR->CR1 & PWR_CR1_DBP) == RESET)
    {
      if((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
      {
        return HAL_TIMEOUT;
      }
    }

    /* Reset the Backup domain only if the RTC Clock source selection is modified */
    tmpreg0 = (RCC->BDCR & RCC_BDCR_RTCSEL);

    if((tmpreg0 != 0x00000000U) && (tmpreg0 != (PeriphClkInit->RTCClockSelection & RCC_BDCR_RTCSEL)))
    {
      /* Store the content of BDCR register before the reset of Backup Domain */
      tmpreg0 = (RCC->BDCR & ~(RCC_BDCR_RTCSEL));

      /* RTC Clock selection can be changed only if the Backup Domain is reset */
      __HAL_RCC_BACKUPRESET_FORCE();
      __HAL_RCC_BACKUPRESET_RELEASE();

      /* Restore the Content of BDCR register */
      RCC->BDCR = tmpreg0;

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSEON))
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till LSE is ready */
        while(__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET)
        {
          if((HAL_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
    __HAL_RCC_RTC_CONFIG(PeriphClkInit->RTCClockSelection);
  }

  /*------------------------------------ TIM configuration --------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == (RCC_PERIPHCLK_TIM))
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIMPRES(PeriphClkInit->TIMPresSelection));

    /* Configure Timer Prescaler */
    __HAL_RCC_TIMCLKPRESCALER(PeriphClkInit->TIMPresSelection);
  }

  /*-------------------------------------- I2C1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(PeriphClkInit->I2c1ClockSelection));

    /* Configure the I2C1 clock source */
    __HAL_RCC_I2C1_CONFIG(PeriphClkInit->I2c1ClockSelection);
  }

  /*-------------------------------------- I2C2 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(PeriphClkInit->I2c2ClockSelection));

    /* Configure the I2C2 clock source */
    __HAL_RCC_I2C2_CONFIG(PeriphClkInit->I2c2ClockSelection);
  }

  /*-------------------------------------- I2C3 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C3) == RCC_PERIPHCLK_I2C3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C3CLKSOURCE(PeriphClkInit->I2c3ClockSelection));

    /* Configure the I2C3 clock source */
    __HAL_RCC_I2C3_CONFIG(PeriphClkInit->I2c3ClockSelection);
  }

  /*-------------------------------------- USART1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(PeriphClkInit->Usart1ClockSelection));

    /* Configure the USART1 clock source */
    __HAL_RCC_USART1_CONFIG(PeriphClkInit->Usart1ClockSelection);
  }

  /*-------------------------------------- USART2 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(PeriphClkInit->Usart2ClockSelection));

    /* Configure the USART2 clock source */
    __HAL_RCC_USART2_CONFIG(PeriphClkInit->Usart2ClockSelection);
  }

  /*-------------------------------------- USART3 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(PeriphClkInit->Usart3ClockSelection));

    /* Configure the USART3 clock source */
    __HAL_RCC_USART3_CONFIG(PeriphClkInit->Usart3ClockSelection);
  }

  /*-------------------------------------- UART4 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART4) == RCC_PERIPHCLK_UART4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART4CLKSOURCE(PeriphClkInit->Uart4ClockSelection));

    /* Configure the UART4 clock source */
    __HAL_RCC_UART4_CONFIG(PeriphClkInit->Uart4ClockSelection);
  }

  /*-------------------------------------- UART5 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART5) == RCC_PERIPHCLK_UART5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART5CLKSOURCE(PeriphClkInit->Uart5ClockSelection));

    /* Configure the UART5 clock source */
    __HAL_RCC_UART5_CONFIG(PeriphClkInit->Uart5ClockSelection);
  }

  /*-------------------------------------- USART6 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART6) == RCC_PERIPHCLK_USART6)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART6CLKSOURCE(PeriphClkInit->Usart6ClockSelection));

    /* Configure the USART6 clock source */
    __HAL_RCC_USART6_CONFIG(PeriphClkInit->Usart6ClockSelection);
  }

  /*-------------------------------------- UART7 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART7) == RCC_PERIPHCLK_UART7)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART7CLKSOURCE(PeriphClkInit->Uart7ClockSelection));

    /* Configure the UART7 clock source */
    __HAL_RCC_UART7_CONFIG(PeriphClkInit->Uart7ClockSelection);
  }

  /*-------------------------------------- UART8 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART8) == RCC_PERIPHCLK_UART8)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART8CLKSOURCE(PeriphClkInit->Uart8ClockSelection));

    /* Configure the UART8 clock source */
    __HAL_RCC_UART8_CONFIG(PeriphClkInit->Uart8ClockSelection);
  }

  /*-------------------------------------- CK48 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CLK48SOURCE(PeriphClkInit->Clk48ClockSelection));

    /* Configure the CLK48 source */
    __HAL_RCC_CLK48_CONFIG(PeriphClkInit->Clk48ClockSelection);

    /* Enable the PLLSAI when it's used as clock source for CK48 */
    if(PeriphClkInit->Clk48ClockSelection == RCC_CLK48SOURCE_PLLSAIP)
    {
      pllsaiused = 1;
    }
  }

  /*-------------------------------------- LPTIM1 Configuration -----------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == RCC_PERIPHCLK_LPTIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1CLK(PeriphClkInit->Lptim1ClockSelection));

    /* Configure the LTPIM1 clock source */
    __HAL_RCC_LPTIM1_CONFIG(PeriphClkInit->Lptim1ClockSelection);
   }

  /*------------------------------------- SDMMC1 Configuration ------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC1) == RCC_PERIPHCLK_SDMMC1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC1CLKSOURCE(PeriphClkInit->Sdmmc1ClockSelection));

    /* Configure the SDMMC1 clock source */
    __HAL_RCC_SDMMC1_CONFIG(PeriphClkInit->Sdmmc1ClockSelection);
  }

  /*------------------------------------- SDMMC2 Configuration ------------------------------------*/
  if(((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC2) == RCC_PERIPHCLK_SDMMC2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SDMMC2CLKSOURCE(PeriphClkInit->Sdmmc2ClockSelection));

    /* Configure the SDMMC2 clock source */
    __HAL_RCC_SDMMC2_CONFIG(PeriphClkInit->Sdmmc2ClockSelection);
  }

  /*-------------------------------------- PLLI2S Configuration ---------------------------------*/
  /* PLLI2S is configured when a peripheral will use it as source clock : SAI1, SAI2 or I2S */
  if((plli2sused == 1) || ((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S))
  {
    /* Disable the PLLI2S */
    __HAL_RCC_PLLI2S_DISABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLI2S is disabled */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  != RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }

    /* check for common PLLI2S Parameters */
    assert_param(IS_RCC_PLLI2SN_VALUE(PeriphClkInit->PLLI2S.PLLI2SN));

    /*----------------- In Case of PLLI2S is selected as source clock for I2S -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2S) == RCC_PERIPHCLK_I2S) && (PeriphClkInit->I2sClockSelection == RCC_I2SCLKSOURCE_PLLI2S)))
    {
      /* check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));

      /* Read PLLI2SQ value from PLLI2SCFGR register (this value is not needed for I2S configuration) */
      tmpreg0 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLM) */
      /* I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , tmpreg0, PeriphClkInit->PLLI2S.PLLI2SR);
    }

    /*----------------- In Case of PLLI2S is selected as source clock for SAI -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLI2S)) ||
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLI2S)))
    {
      /* Check for PLLI2S Parameters */
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));
      /* Check for PLLI2S/DIVQ parameters */
      assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(PeriphClkInit->PLLI2SDivQ));

      /* Read PLLI2SP and PLLI2SR values from PLLI2SCFGR register (this value is not needed for SAI configuration) */
      tmpreg0 = ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);
      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
      /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN, PeriphClkInit->PLLI2S.PLLI2SQ, tmpreg0);

      /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
      __HAL_RCC_PLLI2S_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLI2SDivQ);
    }

    /*----------------- In Case of PLLI2S is just selected  -----------------*/
    if((PeriphClkInit->PeriphClockSelection & RCC_PERIPHCLK_PLLI2S) == RCC_PERIPHCLK_PLLI2S)
    {
      /* Check for Parameters */
      assert_param(IS_RCC_PLLI2SR_VALUE(PeriphClkInit->PLLI2S.PLLI2SR));
      assert_param(IS_RCC_PLLI2SQ_VALUE(PeriphClkInit->PLLI2S.PLLI2SQ));

      /* Configure the PLLI2S division factors */
      /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) x (PLLI2SN/PLLI2SM) */
      __HAL_RCC_PLLI2S_CONFIG(PeriphClkInit->PLLI2S.PLLI2SN , PeriphClkInit->PLLI2S.PLLI2SQ, PeriphClkInit->PLLI2S.PLLI2SR);
    }

    /* Enable the PLLI2S */
    __HAL_RCC_PLLI2S_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLI2S is ready */
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY)  == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }
  }

  /*-------------------------------------- PLLSAI Configuration ---------------------------------*/
  /* PLLSAI is configured when a peripheral will use it as source clock : SAI1, SAI2, LTDC or CK48 */
  if(pllsaiused == 1)
  {
    /* Disable PLLSAI Clock */
    __HAL_RCC_PLLSAI_DISABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI is disabled */
    while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }

    /* Check the PLLSAI division factors */
    assert_param(IS_RCC_PLLSAIN_VALUE(PeriphClkInit->PLLSAI.PLLSAIN));

    /*----------------- In Case of PLLSAI is selected as source clock for SAI -------------------*/
    if(((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1) && (PeriphClkInit->Sai1ClockSelection == RCC_SAI1CLKSOURCE_PLLSAI)) ||\
       ((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2) && (PeriphClkInit->Sai2ClockSelection == RCC_SAI2CLKSOURCE_PLLSAI)))
    {
      /* check for PLLSAIQ Parameter */
      assert_param(IS_RCC_PLLSAIQ_VALUE(PeriphClkInit->PLLSAI.PLLSAIQ));
      /* check for PLLSAI/DIVQ Parameter */
      assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(PeriphClkInit->PLLSAIDivQ));

      /* Read PLLSAIP value from PLLSAICFGR register (this value is not needed for SAI configuration) */
      tmpreg0 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos);
      /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
      /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
      /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , tmpreg0, PeriphClkInit->PLLSAI.PLLSAIQ);

      /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
      __HAL_RCC_PLLSAI_PLLSAICLKDIVQ_CONFIG(PeriphClkInit->PLLSAIDivQ);
    }

    /*----------------- In Case of PLLSAI is selected as source clock for CLK48 -------------------*/
    /* In Case of PLLI2S is selected as source clock for CK48 */
    if((((PeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CLK48) == RCC_PERIPHCLK_CLK48) && (PeriphClkInit->Clk48ClockSelection == RCC_CLK48SOURCE_PLLSAIP))
    {
      /* check for Parameters */
      assert_param(IS_RCC_PLLSAIP_VALUE(PeriphClkInit->PLLSAI.PLLSAIP));
      /* Read PLLSAIQ and PLLSAIR value from PLLSAICFGR register (this value is not needed for CK48 configuration) */
      tmpreg0 = ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);

      /* Configure the PLLSAI division factors */
      /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) x (PLLI2SN/PLLM) */
      /* 48CLK = f(PLLSAI clock output) = f(VCO clock) / PLLSAIP */
      __HAL_RCC_PLLSAI_CONFIG(PeriphClkInit->PLLSAI.PLLSAIN , PeriphClkInit->PLLSAI.PLLSAIP, tmpreg0);
    }

    /* Enable PLLSAI Clock */
    __HAL_RCC_PLLSAI_ENABLE();

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    /* Wait till PLLSAI is ready */
    while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)
    {
      if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)
      {
        /* return in case of Timeout detected */
        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}

/**
  * @brief  Get the RCC_PeriphCLKInitTypeDef according to the internal
  *         RCC configuration registers.
  * @param  PeriphClkInit pointer to the configured RCC_PeriphCLKInitTypeDef structure
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *PeriphClkInit)
{
  uint32_t tempreg = 0;

  /* Set all possible values for the extended clock type parameter------------*/
  PeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_I2S      | RCC_PERIPHCLK_LPTIM1   |\
                                        RCC_PERIPHCLK_SAI1     | RCC_PERIPHCLK_SAI2     |\
                                        RCC_PERIPHCLK_TIM      | RCC_PERIPHCLK_RTC      |\
                                        RCC_PERIPHCLK_I2C1     | RCC_PERIPHCLK_I2C2     |\
                                        RCC_PERIPHCLK_I2C3     | RCC_PERIPHCLK_USART1   |\
                                        RCC_PERIPHCLK_USART2   | RCC_PERIPHCLK_USART3   |\
                                        RCC_PERIPHCLK_UART4    | RCC_PERIPHCLK_UART5    |\
                                        RCC_PERIPHCLK_USART6   | RCC_PERIPHCLK_UART7    |\
                                        RCC_PERIPHCLK_UART8    | RCC_PERIPHCLK_SDMMC1   |\
                                        RCC_PERIPHCLK_CLK48    | RCC_PERIPHCLK_SDMMC2;

  /* Get the PLLI2S Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLI2S.PLLI2SN = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> RCC_PLLI2SCFGR_PLLI2SN_Pos);
  PeriphClkInit->PLLI2S.PLLI2SQ = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> RCC_PLLI2SCFGR_PLLI2SQ_Pos);
  PeriphClkInit->PLLI2S.PLLI2SR = (uint32_t)((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SR) >> RCC_PLLI2SCFGR_PLLI2SR_Pos);

  /* Get the PLLSAI Clock configuration -----------------------------------------------*/
  PeriphClkInit->PLLSAI.PLLSAIN = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> RCC_PLLSAICFGR_PLLSAIN_Pos);
  PeriphClkInit->PLLSAI.PLLSAIP = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIP) >> RCC_PLLSAICFGR_PLLSAIP_Pos);
  PeriphClkInit->PLLSAI.PLLSAIQ = (uint32_t)((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> RCC_PLLSAICFGR_PLLSAIQ_Pos);

  /* Get the PLLSAI/PLLI2S division factors -------------------------------------------*/
  PeriphClkInit->PLLI2SDivQ = (uint32_t)((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLI2SDIVQ) >> RCC_DCKCFGR1_PLLI2SDIVQ_Pos);
  PeriphClkInit->PLLSAIDivQ = (uint32_t)((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLSAIDIVQ) >> RCC_DCKCFGR1_PLLSAIDIVQ_Pos);

  /* Get the SAI1 clock configuration ----------------------------------------------*/
  PeriphClkInit->Sai1ClockSelection = __HAL_RCC_GET_SAI1_SOURCE();

  /* Get the SAI2 clock configuration ----------------------------------------------*/
  PeriphClkInit->Sai2ClockSelection = __HAL_RCC_GET_SAI2_SOURCE();

  /* Get the I2S clock configuration ------------------------------------------*/
  PeriphClkInit->I2sClockSelection = __HAL_RCC_GET_I2SCLKSOURCE();

  /* Get the I2C1 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c1ClockSelection = __HAL_RCC_GET_I2C1_SOURCE();

  /* Get the I2C2 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c2ClockSelection = __HAL_RCC_GET_I2C2_SOURCE();

  /* Get the I2C3 clock configuration ------------------------------------------*/
  PeriphClkInit->I2c3ClockSelection = __HAL_RCC_GET_I2C3_SOURCE();

  /* Get the USART1 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart1ClockSelection = __HAL_RCC_GET_USART1_SOURCE();

  /* Get the USART2 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE();

  /* Get the USART3 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart3ClockSelection = __HAL_RCC_GET_USART3_SOURCE();

  /* Get the UART4 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart4ClockSelection = __HAL_RCC_GET_UART4_SOURCE();

  /* Get the UART5 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart5ClockSelection = __HAL_RCC_GET_UART5_SOURCE();

  /* Get the USART6 clock configuration ------------------------------------------*/
  PeriphClkInit->Usart6ClockSelection = __HAL_RCC_GET_USART6_SOURCE();

  /* Get the UART7 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart7ClockSelection = __HAL_RCC_GET_UART7_SOURCE();

  /* Get the UART8 clock configuration ------------------------------------------*/
  PeriphClkInit->Uart8ClockSelection = __HAL_RCC_GET_UART8_SOURCE();

  /* Get the LPTIM1 clock configuration ------------------------------------------*/
  PeriphClkInit->Lptim1ClockSelection = __HAL_RCC_GET_LPTIM1_SOURCE();

  /* Get the CK48 clock configuration -----------------------------------------------*/
  PeriphClkInit->Clk48ClockSelection = __HAL_RCC_GET_CLK48_SOURCE();

  /* Get the SDMMC1 clock configuration -----------------------------------------------*/
  PeriphClkInit->Sdmmc1ClockSelection = __HAL_RCC_GET_SDMMC1_SOURCE();

  /* Get the SDMMC2 clock configuration -----------------------------------------------*/
  PeriphClkInit->Sdmmc2ClockSelection = __HAL_RCC_GET_SDMMC2_SOURCE();

  /* Get the RTC Clock configuration -----------------------------------------------*/
  tempreg = (RCC->CFGR & RCC_CFGR_RTCPRE);
  PeriphClkInit->RTCClockSelection = (uint32_t)((tempreg) | (RCC->BDCR & RCC_BDCR_RTCSEL));

  /* Get the TIM Prescaler configuration --------------------------------------------*/
  if ((RCC->DCKCFGR1 & RCC_DCKCFGR1_TIMPRE) == RESET)
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_DESACTIVATED;
  }
  else
  {
    PeriphClkInit->TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  }
}
#endif /* STM32F722xx || STM32F723xx || STM32F732xx || STM32F733xx || STM32F730xx */

/**
  * @brief  Return the peripheral clock frequency for a given peripheral(SAI..)
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg RCC_PERIPHCLK_SAI1: SAI1 peripheral clock
  *            @arg RCC_PERIPHCLK_SAI2: SAI2 peripheral clock
  * @retval Frequency in KHz
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint32_t PeriphClk)
{
  uint32_t tmpreg = 0;
  /* This variable is used to store the SAI clock frequency (value in Hz) */
  uint32_t frequency = 0;
  /* This variable is used to store the VCO Input (value in Hz) */
  uint32_t vcoinput = 0;
  /* This variable is used to store the SAI clock source */
  uint32_t saiclocksource = 0;

  if (PeriphClk == RCC_PERIPHCLK_SAI1)
  {
    saiclocksource = RCC->DCKCFGR1;
    saiclocksource &= RCC_DCKCFGR1_SAI1SEL;
    switch (saiclocksource)
    {
    case 0: /* PLLSAI is the clock source for SAI1 */
      {
        /* Configure the PLLSAI division factor */
        /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the PLL Source is HSI (Internal Clock) */
          vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
        }
        else
        {
          /* In Case the PLL Source is HSE (External Clock) */
          vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
        }
        /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
        /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
        tmpreg = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24;
        frequency = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6))/(tmpreg);

        /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
        tmpreg = (((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLSAIDIVQ) >> 8) + 1);
        frequency = frequency/(tmpreg);
        break;
      }
    case RCC_DCKCFGR1_SAI1SEL_0: /* PLLI2S is the clock source for SAI1 */
      {
        /* Configure the PLLI2S division factor */
        /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the PLL Source is HSI (Internal Clock) */
          vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
        }
        else
        {
          /* In Case the PLL Source is HSE (External Clock) */
          vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
        }

        /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
        /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
        tmpreg = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> 24;
        frequency = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6))/(tmpreg);

        /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
        tmpreg = ((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLI2SDIVQ) + 1);
        frequency = frequency/(tmpreg);
        break;
      }
    case RCC_DCKCFGR1_SAI1SEL_1: /* External clock is the clock source for SAI1 */
      {
        frequency = EXTERNAL_CLOCK_VALUE;
        break;
      }
#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)
    case RCC_DCKCFGR1_SAI1SEL: /* HSI or HSE is the clock source for SAI*/
      {
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the main PLL Source is HSI */
          frequency = HSI_VALUE;
        }
        else
        {
          /* In Case the main PLL Source is HSE */
          frequency = HSE_VALUE;
        }
        break;
      }
#endif /* STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */
    default :
      {
        break;
      }
    }
  }

  if (PeriphClk == RCC_PERIPHCLK_SAI2)
  {
    saiclocksource = RCC->DCKCFGR1;
    saiclocksource &= RCC_DCKCFGR1_SAI2SEL;
    switch (saiclocksource)
    {
    case 0: /* PLLSAI is the clock source for SAI*/
      {
        /* Configure the PLLSAI division factor */
        /* PLLSAI_VCO Input  = PLL_SOURCE/PLLM */
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the PLL Source is HSI (Internal Clock) */
          vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
        }
        else
        {
          /* In Case the PLL Source is HSE (External Clock) */
          vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
        }
        /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN */
        /* SAI_CLK(first level) = PLLSAI_VCO Output/PLLSAIQ */
        tmpreg = (RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIQ) >> 24;
        frequency = (vcoinput * ((RCC->PLLSAICFGR & RCC_PLLSAICFGR_PLLSAIN) >> 6))/(tmpreg);

        /* SAI_CLK_x = SAI_CLK(first level)/PLLSAIDIVQ */
        tmpreg = (((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLSAIDIVQ) >> 8) + 1);
        frequency = frequency/(tmpreg);
        break;
      }
    case RCC_DCKCFGR1_SAI2SEL_0: /* PLLI2S is the clock source for SAI2 */
      {
        /* Configure the PLLI2S division factor */
        /* PLLI2S_VCO Input  = PLL_SOURCE/PLLM */
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the PLL Source is HSI (Internal Clock) */
          vcoinput = (HSI_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM));
        }
        else
        {
          /* In Case the PLL Source is HSE (External Clock) */
          vcoinput = ((HSE_VALUE / (uint32_t)(RCC->PLLCFGR & RCC_PLLCFGR_PLLM)));
        }

        /* PLLI2S_VCO Output = PLLI2S_VCO Input * PLLI2SN */
        /* SAI_CLK(first level) = PLLI2S_VCO Output/PLLI2SQ */
        tmpreg = (RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SQ) >> 24;
        frequency = (vcoinput * ((RCC->PLLI2SCFGR & RCC_PLLI2SCFGR_PLLI2SN) >> 6))/(tmpreg);

        /* SAI_CLK_x = SAI_CLK(first level)/PLLI2SDIVQ */
        tmpreg = ((RCC->DCKCFGR1 & RCC_DCKCFGR1_PLLI2SDIVQ) + 1);
        frequency = frequency/(tmpreg);
        break;
      }
    case RCC_DCKCFGR1_SAI2SEL_1: /* External clock is the clock source for SAI2 */
      {
        frequency = EXTERNAL_CLOCK_VALUE;
        break;
      }
#if defined (STM32F765xx) || defined (STM32F767xx) || defined (STM32F769xx) || defined (STM32F777xx) || defined (STM32F779xx)
    case RCC_DCKCFGR1_SAI2SEL: /* HSI or HSE is the clock source for SAI2 */
      {
        if((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == RCC_PLLSOURCE_HSI)
        {
          /* In Case the main PLL Source is HSI */
          frequency = HSI_VALUE;
        }
        else
        {
          /* In Case the main PLL Source is HSE */
          frequency = HSE_VALUE;
        }
        break;
      }
#endif /* STM32F767xx || STM32F769xx || STM32F777xx || STM32F779xx */
    default :
      {
        break;
      }
    }
  }

  return frequency;
}

/**
  * @}
  */

/** @defgroup RCCEx_Exported_Functions_Group2 Extended Clock management functions
 *  @brief  Extended Clock management functions
 *
@verbatim
 ===============================================================================
                ##### Extended clock management functions  #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the
    activation or deactivation of PLLI2S, PLLSAI.
@endverbatim
  * @{
  */

/**
  * @brief  Enable PLLI2S.
  * @param  PLLI2SInit  pointer to an RCC_PLLI2SInitTypeDef structure that
  *         contains the configuration information for the PLLI2S
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLI2S(RCC_PLLI2SInitTypeDef  *PLLI2SInit)
{
  uint32_t tickstart;

  /* Check for parameters */
  assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SInit->PLLI2SN));
  assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SInit->PLLI2SR));
  assert_param(IS_RCC_PLLI2SQ_VALUE(PLLI2SInit->PLLI2SQ));
#if defined(RCC_PLLI2SCFGR_PLLI2SP)
  assert_param(IS_RCC_PLLI2SP_VALUE(PLLI2SInit->PLLI2SP));
#endif /* RCC_PLLI2SCFGR_PLLI2SP */

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) != RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  /* Configure the PLLI2S division factors */
#if defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) || defined (STM32F730xx)
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * PLLI2SN */
  /* I2SQCLK = PLLI2S_VCO / PLLI2SQ */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SQ, PLLI2SInit->PLLI2SR);
#else
  /* PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) * PLLI2SN */
  /* I2SPCLK = PLLI2S_VCO / PLLI2SP */
  /* I2SQCLK = PLLI2S_VCO / PLLI2SQ */
  /* I2SRCLK = PLLI2S_VCO / PLLI2SR */
  __HAL_RCC_PLLI2S_CONFIG(PLLI2SInit->PLLI2SN, PLLI2SInit->PLLI2SP, PLLI2SInit->PLLI2SQ, PLLI2SInit->PLLI2SR);
#endif /* STM32F722xx || STM32F723xx || STM32F732xx || STM32F733xx || STM32F730xx */

  /* Enable the PLLI2S */
  __HAL_RCC_PLLI2S_ENABLE();

  /* Wait till PLLI2S is ready */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLI2SRDY) == RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

 return HAL_OK;
}

/**
  * @brief  Disable PLLI2S.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLI2S(void)
{
  uint32_t tickstart;

  /* Disable the PLLI2S */
  __HAL_RCC_PLLI2S_DISABLE();

  /* Wait till PLLI2S is disabled */
  tickstart = HAL_GetTick();
  while(READ_BIT(RCC->CR, RCC_CR_PLLI2SRDY) != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLI2S_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Enable PLLSAI.
  * @param  PLLSAIInit  pointer to an RCC_PLLSAIInitTypeDef structure that
  *         contains the configuration information for the PLLSAI
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLLSAI(RCC_PLLSAIInitTypeDef  *PLLSAIInit)
{
  uint32_t tickstart;

  /* Check for parameters */
  assert_param(IS_RCC_PLLSAIN_VALUE(PLLSAIInit->PLLSAIN));
  assert_param(IS_RCC_PLLSAIQ_VALUE(PLLSAIInit->PLLSAIQ));
  assert_param(IS_RCC_PLLSAIP_VALUE(PLLSAIInit->PLLSAIP));
#if defined(RCC_PLLSAICFGR_PLLSAIR)
  assert_param(IS_RCC_PLLSAIR_VALUE(PLLSAIInit->PLLSAIR));
#endif /* RCC_PLLSAICFGR_PLLSAIR */

  /* Disable the PLLSAI */
  __HAL_RCC_PLLSAI_DISABLE();

  /* Wait till PLLSAI is disabled */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  /* Configure the PLLSAI division factors */
#if defined (STM32F722xx) || defined (STM32F723xx) || defined (STM32F732xx) || defined (STM32F733xx) || defined (STM32F730xx)
  /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * PLLSAIN */
  /* SAIPCLK = PLLSAI_VCO / PLLSAIP */
  /* SAIQCLK = PLLSAI_VCO / PLLSAIQ */
  __HAL_RCC_PLLSAI_CONFIG(PLLSAIInit->PLLSAIN, PLLSAIInit->PLLSAIP, PLLSAIInit->PLLSAIQ);
#else
  /* PLLSAI_VCO = f(VCO clock) = f(PLLSAI clock input) * PLLSAIN */
  /* SAIPCLK = PLLSAI_VCO / PLLSAIP */
  /* SAIQCLK = PLLSAI_VCO / PLLSAIQ */
  /* SAIRCLK = PLLSAI_VCO / PLLSAIR */
  __HAL_RCC_PLLSAI_CONFIG(PLLSAIInit->PLLSAIN, PLLSAIInit->PLLSAIP, \
                          PLLSAIInit->PLLSAIQ, PLLSAIInit->PLLSAIR);
#endif /* STM32F722xx || STM32F723xx || STM32F732xx || STM32F733xx || STM32F730xx */

  /* Enable the PLLSAI */
  __HAL_RCC_PLLSAI_ENABLE();

  /* Wait till PLLSAI is ready */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_PLLSAI_GET_FLAG() == RESET)
  {
    if((HAL_GetTick() - tickstart ) > PLLSAI_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

 return HAL_OK;
}

/**
  * @brief  Disable PLLSAI.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLLSAI(void)
{
  uint32_t tickstart;

  /* Disable the PLLSAI */
  __HAL_RCC_PLLSAI_DISABLE();

  /* Wait till PLLSAI is disabled */
  tickstart = HAL_GetTick();
  while(__HAL_RCC_PLLSAI_GET_FLAG() != RESET)
  {
    if((HAL_GetTick() - tickstart) > PLLSAI_TIMEOUT_VALUE)
    {
      /* return in case of Timeout detected */
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

