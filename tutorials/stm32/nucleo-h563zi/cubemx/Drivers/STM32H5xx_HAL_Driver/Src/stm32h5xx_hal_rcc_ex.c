/**
  ******************************************************************************
  * @file    stm32h5xx_hal_rcc_ex.c
  * @author  MCD Application Team
  * @brief   Extended RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities RCC extended peripheral:
  *           + Extended Peripheral Control functions
  *           + Extended Clock management functions
  *           + Extended Clock Recovery System Control functions
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

/** @defgroup RCCEx RCCEx
  * @brief RCC Extended HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/** @defgroup RCCEx_Private_Constants RCCEx Private Constants
  * @{
  */
#define PLL1_TIMEOUT_VALUE     ((uint32_t)2U)          /* 2 ms (minimum Tick + 1) */
#define PLL2_TIMEOUT_VALUE     ((uint32_t)2U)          /* 2 ms (minimum Tick + 1) */
#if defined(RCC_CR_PLL3ON)
#define PLL3_TIMEOUT_VALUE     ((uint32_t)2U)          /* 2 ms (minimum Tick + 1) */
#endif /* RCC_CR_PLL3ON */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCCEx_Private_Functions RCCEx Private Functions
  * @{
  */
static HAL_StatusTypeDef RCCEx_PLLSource_Enable(uint32_t PllSource);
static HAL_StatusTypeDef RCCEx_PLL2_Config(const RCC_PLL2InitTypeDef *Pll2);
#if defined(RCC_CR_PLL3ON)
static HAL_StatusTypeDef RCCEx_PLL3_Config(const RCC_PLL3InitTypeDef *Pll3);
#endif /* RCC_CR_PLL3ON */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

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
        the backup registers) are set to their reset values.

@endverbatim
  * @{
  */
/**
  * @brief  Initialize the RCC extended peripherals clocks according to the specified
  *         parameters in the RCC_PeriphCLKInitTypeDef.
  * @param  pPeriphClkInit  pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         contains a field PeriphClockSelection which can be a combination of the following values:
  *            @arg @ref RCC_PERIPHCLK_USART1  USART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_USART2  USART2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_USART3  USART3 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_UART4   UART4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART5   UART5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART6  USART6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART7   UART7 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART8   UART8 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART9   UART9 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART10 USART10 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART11 USART11 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART12  UART12 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPUART1 LPUART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C1    I2C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C2    I2C2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C3    I2C3 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_I2C4    I2C4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_I3C1    I3C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I3C2    I3C2 peripheral clock (***)
  *            @arg @ref RCC_PERIPHCLK_LPTIM1  LPTIM1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_LPTIM2  LPTIM2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SAI1    SAI1 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SAI2    SAI2 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_ADCDAC  ADCDAC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_ADC     ADC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SDMMC1  SDMMC1 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SDMMC2  SDMMC2 peripheral clock (**)
  *            @arg @ref RCC_PERIPHCLK_CKPER   CKPER peripheral clock
  *            @arg @ref RCC_PERIPHCLK_RTC     RTC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_RNG     RNG peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI1    SPI1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI2    SPI2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI3    SPI3 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI4    SPI4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SPI5    SPI5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SPI6    SPI6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_OSPI    OCTOSPI peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_FDCAN   FDCAN peripheral clock
  *            @arg @ref RCC_PERIPHCLK_CEC     CEC peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USB     USB peripheral clock (*****)
  *            @arg @ref RCC_PERIPHCLK_LPTIM3  LPTIM3 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM4  LPTIM4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM5  LPTIM5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM6  LPTIM6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_DAC_LP  DAC peripheral low-power clock
  *            @arg @ref RCC_PERIPHCLK_TIM     TIM peripheral clock
  *            @arg @ref RCC_PERIPHCLK_PLAY1   PLAY1 peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_OTGFS   USB_OTG_FS peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_OTGHS   USB_OTG_HS peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_OSPI2   OCTOSPI2 peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_LTDC    LTDC peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_MDF1    MDF1 peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_ADF1    ADF1 peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_ETH     ETH peripheral clock
  *            @arg @ref RCC_PERIPHCLK_ETHPTP  ETHPTP peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_ETHREF  ETHREF peripheral clock (****)
  *
  * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *         the RTC clock source: in this case the access to Backup domain is enabled.
  *
  * @retval HAL status
  *
  *  (*)     : For stm32h56xxx and stm32h57xxx family lines only.
  *  (**)    : For stm32h563xx and stm32h57xxx family lines only.
  *  (***)   : For stm32h503xx family line only.
  *  (****)  : For stm32h5exxx and stm32h5fxxx family lines only.
  *  (*****) : Not available for stm32h5exxx and stm32h5fxxx family lines.
  */
HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(const RCC_PeriphCLKInitTypeDef  *pPeriphClkInit)
{
  uint32_t tmpregister;
  uint32_t tickstart;
  HAL_StatusTypeDef ret = HAL_OK;      /* Intermediate status */
  HAL_StatusTypeDef status = HAL_OK;   /* Final status */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(pPeriphClkInit->PeriphClockSelection));

  /*------------------------------------ CKPER configuration --------------------------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CKPER) == RCC_PERIPHCLK_CKPER)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CLKPSOURCE(pPeriphClkInit->CkperClockSelection));

    /* Configure the CKPER clock source */
    __HAL_RCC_CLKP_CONFIG(pPeriphClkInit->CkperClockSelection);
  }

  /*-------------------------- USART1 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART1) == RCC_PERIPHCLK_USART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART1CLKSOURCE(pPeriphClkInit->Usart1ClockSelection));

    switch (pPeriphClkInit->Usart1ClockSelection)
    {
      case RCC_USART1CLKSOURCE_PCLK2:      /* PCLK2 is used as clock source for USART1*/

        /* USART1 clock source config set later after clock selection check */
        break;

      case RCC_USART1CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART1*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART1 clock source config set later after clock selection check */
        break;
#if defined(RCC_USART1CLKSOURCE_PLL3Q)
      case RCC_USART1CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART1*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART1 clock source config set later after clock selection check */
        break;
#endif /* RCC_USART1CLKSOURCE_PLL3Q */

      case RCC_USART1CLKSOURCE_HSI:      /* HSI clock is used as source of USART1 clock*/
        /* USART1 clock source config set later after clock selection check */
        break;

      case RCC_USART1CLKSOURCE_CSI:      /* CSI clock is used as source of USART1 clock*/
        /* USART1 clock source config set later after clock selection check */
        break;

      case RCC_USART1CLKSOURCE_LSE:      /* LSE clock is used as source of USART1 clock*/
        /* USART1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART1 clock*/
      __HAL_RCC_USART1_CONFIG(pPeriphClkInit->Usart1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- USART2 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART2) == RCC_PERIPHCLK_USART2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART2CLKSOURCE(pPeriphClkInit->Usart2ClockSelection));

    switch (pPeriphClkInit->Usart2ClockSelection)
    {
      case RCC_USART2CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for USART2*/

        /* USART2 clock source config set later after clock selection check */
        break;

      case RCC_USART2CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART2*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART2 clock source config set later after clock selection check */
        break;

#if defined(RCC_USART2CLKSOURCE_PLL3Q)
      case RCC_USART2CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART2*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART2 clock source config set later after clock selection check */
        break;
#endif /* RCC_USART2CLKSOURCE_PLL3 */

      case RCC_USART2CLKSOURCE_HSI:      /* HSI clock is used as source of USART2 clock*/
        /* USART2 clock source config set later after clock selection check */
        break;

      case RCC_USART2CLKSOURCE_CSI:      /* CSI clock is used as source of USART2 clock*/
        /* USART2 clock source config set later after clock selection check */
        break;

      case RCC_USART2CLKSOURCE_LSE:      /* LSE clock is used as source of USART2 clock*/
        /* USART2 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART2 clock*/
      __HAL_RCC_USART2_CONFIG(pPeriphClkInit->Usart2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- USART3 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART3) == RCC_PERIPHCLK_USART3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART3CLKSOURCE(pPeriphClkInit->Usart3ClockSelection));

    switch (pPeriphClkInit->Usart3ClockSelection)
    {
      case RCC_USART3CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for USART3*/

        /* USART3 clock source config set later after clock selection check */
        break;

      case RCC_USART3CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART3*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART3 clock source config set later after clock selection check */
        break;

#if defined(RCC_USART3CLKSOURCE_PLL3Q)
      case RCC_USART3CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART3*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART3 clock source config set later after clock selection check */
        break;
#endif /* RCC_USART3CLKSOURCE_PLL3Q */

      case RCC_USART3CLKSOURCE_HSI:      /* HSI clock is used as source of USART3 clock*/
        /* USART3 clock source config set later after clock selection check */
        break;

      case RCC_USART3CLKSOURCE_CSI:      /* CSI clock is used as source of USART3 clock*/
        /* USART3 clock source config set later after clock selection check */
        break;

      case RCC_USART3CLKSOURCE_LSE:      /* LSE clock is used as source of USART3 clock*/
        /* USART3 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART3 clock*/
      __HAL_RCC_USART3_CONFIG(pPeriphClkInit->Usart3ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

#if defined(UART4)
  /*-------------------------- UART4 clock source configuration --------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART4) == RCC_PERIPHCLK_UART4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART4CLKSOURCE(pPeriphClkInit->Uart4ClockSelection));

    switch (pPeriphClkInit->Uart4ClockSelection)
    {
      case RCC_UART4CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART4*/

        /* UART4 clock source config set later after clock selection check */
        break;

      case RCC_UART4CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART4*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART4 clock source config set later after clock selection check */
        break;
#if defined(RCC_UART4CLKSOURCE_PLL3Q)
      case RCC_UART4CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART4*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART4 clock source config set later after clock selection check */
        break;
#endif /* RCC_UART4CLKSOURCE_PLL3Q */
      case RCC_UART4CLKSOURCE_HSI:      /* HSI clock is used as source of UART4 clock*/
        /* UART4 clock source config set later after clock selection check */
        break;

      case RCC_UART4CLKSOURCE_CSI:      /* CSI clock is used as source of UART4 clock*/
        /* UART4 clock source config set later after clock selection check */
        break;

      case RCC_UART4CLKSOURCE_LSE:      /* LSE clock is used as source of UART4 clock*/
        /* UART4 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART4 clock*/
      __HAL_RCC_UART4_CONFIG(pPeriphClkInit->Uart4ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART4 */

#if defined(UART5)
  /*-------------------------- UART5 clock source configuration --------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART5) == RCC_PERIPHCLK_UART5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART5CLKSOURCE(pPeriphClkInit->Uart5ClockSelection));

    switch (pPeriphClkInit->Uart5ClockSelection)
    {
      case RCC_UART5CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART5*/

        /* UART5 clock source config set later after clock selection check */
        break;

      case RCC_UART5CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART5*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART5 clock source config set later after clock selection check */
        break;
#if defined(RCC_UART5CLKSOURCE_PLL3Q)
      case RCC_UART5CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART5*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART5 clock source config set later after clock selection check */
        break;
#endif /* RCC_UART5CLKSOURCE_PLL3Q */
      case RCC_UART5CLKSOURCE_HSI:      /* HSI clock is used as source of UART5 clock*/
        /* UART5 clock source config set later after clock selection check */
        break;

      case RCC_UART5CLKSOURCE_CSI:      /* CSI clock is used as source of UART5 clock*/
        /* UART5 clock source config set later after clock selection check */
        break;

      case RCC_UART5CLKSOURCE_LSE:      /* LSE clock is used as source of UART5 clock*/
        /* UART5 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART5 clock*/
      __HAL_RCC_UART5_CONFIG(pPeriphClkInit->Uart5ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART5 */

#if defined(USART6)
  /*-------------------------- USART6 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART6) == RCC_PERIPHCLK_USART6)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART6CLKSOURCE(pPeriphClkInit->Usart6ClockSelection));

    switch (pPeriphClkInit->Usart6ClockSelection)
    {
      case RCC_USART6CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for USART6*/

        /* USART6 clock source config set later after clock selection check */
        break;

      case RCC_USART6CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART6*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART6 clock source config set later after clock selection check */
        break;
#if defined(RCC_USART6CLKSOURCE_PLL3Q)
      case RCC_USART6CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART6*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART6 clock source config set later after clock selection check */
        break;
#endif /* RCC_USART6CLKSOURCE_PLL3Q */
      case RCC_USART6CLKSOURCE_HSI:      /* HSI clock is used as source of USART6 clock*/
        /* USART6 clock source config set later after clock selection check */
        break;

      case RCC_USART6CLKSOURCE_CSI:      /* CSI clock is used as source of USART6 clock*/
        /* USART6 clock source config set later after clock selection check */
        break;

      case RCC_USART6CLKSOURCE_LSE:      /* LSE clock is used as source of USART6 clock*/
        /* USART6 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART6 clock*/
      __HAL_RCC_USART6_CONFIG(pPeriphClkInit->Usart6ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* USART6 */

#if defined(UART7)
  /*-------------------------- UART7 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART7) == RCC_PERIPHCLK_UART7)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART7CLKSOURCE(pPeriphClkInit->Uart7ClockSelection));

    switch (pPeriphClkInit->Uart7ClockSelection)
    {
      case RCC_UART7CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART7*/

        /* UART7 clock source config set later after clock selection check */
        break;

      case RCC_UART7CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART7*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART7 clock source config set later after clock selection check */
        break;
#if defined(RCC_UART7CLKSOURCE_PLL3Q)
      case RCC_UART7CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART7*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART7 clock source config set later after clock selection check */
        break;
#endif /* RCC_UART7CLKSOURCE_PLL3Q */
      case RCC_UART7CLKSOURCE_HSI:      /* HSI clock is used as source of UART7 clock*/
        /* UART7 clock source config set later after clock selection check */
        break;

      case RCC_UART7CLKSOURCE_CSI:      /* CSI clock is used as source of UART7 clock*/
        /* UART7 clock source config set later after clock selection check */
        break;

      case RCC_UART7CLKSOURCE_LSE:      /* LSE clock is used as source of UART7 clock*/
        /* UART7 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART7 clock*/
      __HAL_RCC_UART7_CONFIG(pPeriphClkInit->Uart7ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART7 */

#if defined(UART8)
  /*-------------------------- UART8 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART8) == RCC_PERIPHCLK_UART8)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART8CLKSOURCE(pPeriphClkInit->Uart8ClockSelection));

    switch (pPeriphClkInit->Uart8ClockSelection)
    {
      case RCC_UART8CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART8*/

        /* UART8 clock source config set later after clock selection check */
        break;

      case RCC_UART8CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART8*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART8 clock source config set later after clock selection check */
        break;
#if defined(RCC_UART8CLKSOURCE_PLL3Q)
      case RCC_UART8CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART8*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART8 clock source config set later after clock selection check */
        break;
#endif /* RCC_UART8CLKSOURCE_PLL3Q */
      case RCC_UART8CLKSOURCE_HSI:      /* HSI clock is used as source of UART8 clock*/
        /* UART8 clock source config set later after clock selection check */
        break;

      case RCC_UART8CLKSOURCE_CSI:      /* CSI clock is used as source of UART8 clock*/
        /* UART8 clock source config set later after clock selection check */
        break;

      case RCC_UART8CLKSOURCE_LSE:      /* LSE clock is used as source of UART8 clock*/
        /* UART8 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART8 clock*/
      __HAL_RCC_UART8_CONFIG(pPeriphClkInit->Uart8ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART9 */

#if defined(UART9)
  /*-------------------------- UART9 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART9) == RCC_PERIPHCLK_UART9)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART9CLKSOURCE(pPeriphClkInit->Uart9ClockSelection));

    switch (pPeriphClkInit->Uart9ClockSelection)
    {
      case RCC_UART9CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART9*/

        /* UART9 clock source config set later after clock selection check */
        break;

      case RCC_UART9CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART9*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART9 clock source config set later after clock selection check */
        break;
      case RCC_UART9CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART9*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART9 clock source config set later after clock selection check */
        break;
      case RCC_UART9CLKSOURCE_HSI:      /* HSI clock is used as source of UART9 clock*/
        /* UART9 clock source config set later after clock selection check */
        break;

      case RCC_UART9CLKSOURCE_CSI:      /* CSI clock is used as source of UART9 clock*/
        /* UART9 clock source config set later after clock selection check */
        break;

      case RCC_UART9CLKSOURCE_LSE:      /* LSE clock is used as source of UART9 clock*/
        /* UART9 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART9 clock*/
      __HAL_RCC_UART9_CONFIG(pPeriphClkInit->Uart9ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART9 */

#if defined(USART10)
  /*-------------------------- USART10 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART10) == RCC_PERIPHCLK_USART10)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART10CLKSOURCE(pPeriphClkInit->Usart10ClockSelection));

    switch (pPeriphClkInit->Usart10ClockSelection)
    {
      case RCC_USART10CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for USART10*/

        /* USART10 clock source config set later after clock selection check */
        break;

      case RCC_USART10CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART10*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART10 clock source config set later after clock selection check */
        break;
      case RCC_USART10CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART10*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART10 clock source config set later after clock selection check */
        break;
      case RCC_USART10CLKSOURCE_HSI:      /* HSI clock is used as source of USART10 clock*/
        /* USART10 clock source config set later after clock selection check */
        break;

      case RCC_USART10CLKSOURCE_CSI:      /* CSI clock is used as source of USART10 clock*/
        /* USART10 clock source config set later after clock selection check */
        break;

      case RCC_USART10CLKSOURCE_LSE:      /* LSE clock is used as source of USART10 clock*/
        /* USART10 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART10 clock*/
      __HAL_RCC_USART10_CONFIG(pPeriphClkInit->Usart10ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* USART10 */

#if defined(USART11)
  /*-------------------------- USART11 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USART11) == RCC_PERIPHCLK_USART11)
  {
    /* Check the parameters */
    assert_param(IS_RCC_USART11CLKSOURCE(pPeriphClkInit->Usart11ClockSelection));

    switch (pPeriphClkInit->Usart11ClockSelection)
    {
      case RCC_USART11CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for USART11*/

        /* USART11 clock source config set later after clock selection check */
        break;

      case RCC_USART11CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for USART11*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* USART11 clock source config set later after clock selection check */
        break;
      case RCC_USART11CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for USART11*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USART11 clock source config set later after clock selection check */
        break;
      case RCC_USART11CLKSOURCE_HSI:      /* HSI clock is used as source of USART11 clock*/
        /* USART11 clock source config set later after clock selection check */
        break;

      case RCC_USART11CLKSOURCE_CSI:      /* CSI clock is used as source of USART11 clock*/
        /* USART11 clock source config set later after clock selection check */
        break;

      case RCC_USART11CLKSOURCE_LSE:      /* LSE clock is used as source of USART11 clock*/
        /* USART11 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USART11 clock*/
      __HAL_RCC_USART11_CONFIG(pPeriphClkInit->Usart11ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /*USART11*/

#if defined(UART12)
  /*-------------------------- UART12 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_UART12) == RCC_PERIPHCLK_UART12)
  {
    /* Check the parameters */
    assert_param(IS_RCC_UART12CLKSOURCE(pPeriphClkInit->Uart12ClockSelection));

    switch (pPeriphClkInit->Uart12ClockSelection)
    {
      case RCC_UART12CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for UART12*/

        /* UART12 clock source config set later after clock selection check */
        break;

      case RCC_UART12CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for UART12*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* UART12 clock source config set later after clock selection check */
        break;
      case RCC_UART12CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for UART12*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* UART12 clock source config set later after clock selection check */
        break;
      case RCC_UART12CLKSOURCE_HSI:      /* HSI clock is used as source of UART12 clock*/
        /* UART12 clock source config set later after clock selection check */
        break;

      case RCC_UART12CLKSOURCE_CSI:      /* CSI clock is used as source of UART12 clock*/
        /* UART12 clock source config set later after clock selection check */
        break;

      case RCC_UART12CLKSOURCE_LSE:      /* LSE clock is used as source of UART12 clock*/
        /* UART12 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of UART12 clock*/
      __HAL_RCC_UART12_CONFIG(pPeriphClkInit->Uart12ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* UART12 */

  /*-------------------------- LPUART1 clock source configuration ------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPUART1) == RCC_PERIPHCLK_LPUART1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPUART1CLKSOURCE(pPeriphClkInit->Lpuart1ClockSelection));

    switch (pPeriphClkInit->Lpuart1ClockSelection)
    {
      case RCC_LPUART1CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for LPUART1*/

        /* LPUART1 clock source config set later after clock selection check */
        break;

      case RCC_LPUART1CLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for LPUART1*/
        /* PLL2 input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPUART1 clock source config set later after clock selection check */
        break;

#if defined(RCC_LPUART1CLKSOURCE_PLL3Q)
      case RCC_LPUART1CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for LPUART1*/
        /* PLL3  input clock, parameters M, N & Q configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPUART1 clock source config set later after clock selection check */
        break;
#endif /* RCC_LPUART1CLKSOURCE_PLL3Q */

      case RCC_LPUART1CLKSOURCE_HSI:      /* HSI clock is used as source of LPUART1 clock*/
        /* LPUART1 clock source config set later after clock selection check */
        break;

      case RCC_LPUART1CLKSOURCE_CSI:      /* CSI clock is used as source of LPUART1 clock*/
        /* LPUART1 clock source config set later after clock selection check */
        break;

      case RCC_LPUART1CLKSOURCE_LSE:      /* LSE clock is used as source of LPUART1 clock*/
        /* LPUART1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPUART1 clock*/
      __HAL_RCC_LPUART1_CONFIG(pPeriphClkInit->Lpuart1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- I2C1 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C1) == RCC_PERIPHCLK_I2C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C1CLKSOURCE(pPeriphClkInit->I2c1ClockSelection));

    switch (pPeriphClkInit->I2c1ClockSelection)
    {
      case RCC_I2C1CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for I2C1*/

        /* I2C1 clock source config set later after clock selection check */
        break;

#if defined(RCC_I2C1CLKSOURCE_PLL3R)
      case RCC_I2C1CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I2C1*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
#else
      case RCC_I2C1CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for I2C1*/
        /* PLL2  input clock, parameters M, N & R configuration clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
#endif /* RCC_I2C1CLKSOURCE_PLL3R */
        /* I2C1 clock source config set later after clock selection check */
        break;


      case RCC_I2C1CLKSOURCE_HSI:      /* HSI clock is used as source of I2C1 clock*/
        /* I2C1 clock source config set later after clock selection check */
        break;

      case RCC_I2C1CLKSOURCE_CSI:      /* CSI clock is used as source of I2C1 clock*/
        /* I2C1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of I2C1 clock*/
      __HAL_RCC_I2C1_CONFIG(pPeriphClkInit->I2c1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- I2C2 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C2) == RCC_PERIPHCLK_I2C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C2CLKSOURCE(pPeriphClkInit->I2c2ClockSelection));

    switch (pPeriphClkInit->I2c2ClockSelection)
    {
      case RCC_I2C2CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for I2C2*/

        /* I2C2 clock source config set later after clock selection check */
        break;

#if defined(RCC_I2C2CLKSOURCE_PLL3R)
      case RCC_I2C2CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I2C2*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
#else
      case RCC_I2C2CLKSOURCE_PLL2R:  /* PLL32 is used as clock source for I2C2*/
        /* PLL2  input clock, parameters M, N & R configuration clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
#endif /* RCC_I2C2CLKSOURCE_PLL3R */
        /* I2C2 clock source config set later after clock selection check */
        break;

      case RCC_I2C2CLKSOURCE_HSI:      /* HSI clock is used as source of I2C2 clock*/
        /* I2C2 clock source config set later after clock selection check */
        break;

      case RCC_I2C2CLKSOURCE_CSI:      /* CSI clock is used as source of I2C2 clock*/
        /* I2C2 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of I2C2 clock*/
      __HAL_RCC_I2C2_CONFIG(pPeriphClkInit->I2c2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

#if defined(I2C3)
  /*-------------------------- I2C3 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C3) == RCC_PERIPHCLK_I2C3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C3CLKSOURCE(pPeriphClkInit->I2c3ClockSelection));

    switch (pPeriphClkInit->I2c3ClockSelection)
    {
      case RCC_I2C3CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for I2C3*/

        /* I2C3 clock source config set later after clock selection check */
        break;
#if defined(RCC_I2C3CLKSOURCE_PLL3R)
      case RCC_I2C3CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I2C3*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* I2C3 clock source config set later after clock selection check */
        break;
#else
	   case RCC_I2C3CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for I2C3*/
        /* PLL2  input clock, parameters M, N & R configuration clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* I2C3 clock source config set later after clock selection check */
        break;
#endif /* RCC_I2C3CLKSOURCE_PLL3R */
      case RCC_I2C3CLKSOURCE_HSI:      /* HSI clock is used as source of I2C3 clock*/
        /* I2C3 clock source config set later after clock selection check */
        break;

      case RCC_I2C3CLKSOURCE_CSI:      /* CSI clock is used as source of I2C3 clock*/
        /* I2C3 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of I2C3 clock*/
      __HAL_RCC_I2C3_CONFIG(pPeriphClkInit->I2c3ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* I2C3 */

#if defined(I2C4)
  /*-------------------------- I2C4 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I2C4) == RCC_PERIPHCLK_I2C4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I2C4CLKSOURCE(pPeriphClkInit->I2c4ClockSelection));

    switch (pPeriphClkInit->I2c4ClockSelection)
    {
      case RCC_I2C4CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for I2C4*/

        /* I2C4 clock source config set later after clock selection check */
        break;
      case RCC_I2C4CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I2C4*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* I2C4 clock source config set later after clock selection check */
        break;
      case RCC_I2C4CLKSOURCE_HSI:      /* HSI clock is used as source of I2C4 clock*/
        /* I2C4 clock source config set later after clock selection check */
        break;

      case RCC_I2C4CLKSOURCE_CSI:      /* CSI clock is used as source of I2C4 clock*/
        /* I2C4 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of I2C4 clock*/
      __HAL_RCC_I2C4_CONFIG(pPeriphClkInit->I2c4ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* I2C4 */

  /*-------------------------- I3C1 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I3C1) == RCC_PERIPHCLK_I3C1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I3C1CLKSOURCE(pPeriphClkInit->I3c1ClockSelection));

    switch (pPeriphClkInit->I3c1ClockSelection)
    {
      case RCC_I3C1CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for I3C1*/

        /* I3C1 clock source config set later after clock selection check */
        break;

#if defined(RCC_I3C1CLKSOURCE_PLL3R)
      case RCC_I3C1CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I3C1*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
#else
      case RCC_I3C1CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for I3C1*/
        /* PLL2  input clock, parameters M, N & R configuration clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
#endif /* RCC_I3C1CLKSOURCE_PLL3R */
        /* I3C1 clock source config set later after clock selection check */
        break;

      case RCC_I3C1CLKSOURCE_HSI:      /* HSI clock is used as source of I3C1 clock*/
        /* I3C1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of I3C1 clock*/
      __HAL_RCC_I3C1_CONFIG(pPeriphClkInit->I3c1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

#if defined (I3C2)
  /*-------------------------- I3C2 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_I3C2) == RCC_PERIPHCLK_I3C2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_I3C2CLKSOURCE(pPeriphClkInit->I3c2ClockSelection));

    switch (pPeriphClkInit->I3c2ClockSelection)
    {
      case RCC_I3C2CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for I3C2*/

        /* I3C2 clock source config set later after clock selection check */
        break;

#if defined(RCC_I3C2CLKSOURCE_PLL3R)
      case RCC_I3C2CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for I3C2*/
        /* PLL3  input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
#else
      case RCC_I3C2CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for I3C2*/
        /* PLL2  input clock, parameters M, N & R configuration clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
#endif /* RCC_I3C2CLKSOURCE_PLL3R */
        /* I3C2 clock source config set later after clock selection check */
        break;

      case RCC_I3C2CLKSOURCE_HSI:      /* HSI clock is used as source of I3C2 clock*/
        /* I3C2 clock source config set later after clock selection check */
        break;

#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
      case RCC_I3C2CLKSOURCE_CSI:      /* CSI clock is used as source of I3C2 clock*/
        /* I3C2 clock source config set later after clock selection check */
        break;
#endif /* defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) */

      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Set the source of I3C2 clock*/
      __HAL_RCC_I3C2_CONFIG(pPeriphClkInit->I3c2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* I3C2 */

  /*------------------------------------ TIM configuration --------------------------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_TIM) == RCC_PERIPHCLK_TIM)
  {
    /* Check the parameters */
    assert_param(IS_RCC_TIMPRES(pPeriphClkInit->TimPresSelection));

    /* Configure Timer Prescaler */
    __HAL_RCC_TIMCLKPRESCALER(pPeriphClkInit->TimPresSelection);
  }

  /*-------------------------- LPTIM1 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM1) == RCC_PERIPHCLK_LPTIM1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM1CLK(pPeriphClkInit->Lptim1ClockSelection));

    switch (pPeriphClkInit->Lptim1ClockSelection)
    {
      case RCC_LPTIM1CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for LPTIM1*/

        /* LPTIM1 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM1CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM1*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM1 clock source config set later after clock selection check */
        break;

#if defined(RCC_LPTIM1CLKSOURCE_PLL3R)
      case RCC_LPTIM1CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM1*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM1 clock source config set later after clock selection check */
        break;
#endif /* RCC_LPTIM1CLKSOURCE_PLL3R */

      case RCC_LPTIM1CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM1 clock*/
        /* LPTIM1 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM1CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM1 clock*/
        /* LPTIM1 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM1CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM1 clock*/
        /* LPTIM1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM1 clock*/
      __HAL_RCC_LPTIM1_CONFIG(pPeriphClkInit->Lptim1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

  /*-------------------------- LPTIM2 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM2) == RCC_PERIPHCLK_LPTIM2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM2CLK(pPeriphClkInit->Lptim2ClockSelection));

    switch (pPeriphClkInit->Lptim2ClockSelection)
    {
      case RCC_LPTIM2CLKSOURCE_PCLK1:      /* PCLK1 is used as clock source for LPTIM2*/

        /* LPTIM2 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM2CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM2*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM2 clock source config set later after clock selection check */
        break;

#if defined(RCC_LPTIM2CLKSOURCE_PLL3R)
      case RCC_LPTIM2CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM2*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM2 clock source config set later after clock selection check */
        break;
#endif /* RCC_LPTIM2CLKSOURCE_PLL3R */

      case RCC_LPTIM2CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM2 clock*/
        /* LPTIM2 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM2CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM2 clock*/
        /* LPTIM2 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM2CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM2 clock*/
        /* LPTIM2 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM2 clock*/
      __HAL_RCC_LPTIM2_CONFIG(pPeriphClkInit->Lptim2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

#if defined(LPTIM3)
  /*-------------------------- LPTIM3 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM3) == RCC_PERIPHCLK_LPTIM3)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM3CLK(pPeriphClkInit->Lptim3ClockSelection));

    switch (pPeriphClkInit->Lptim3ClockSelection)
    {
      case RCC_LPTIM3CLKSOURCE_PCLK3:  /* PCLK3 is used as clock source for LPTIM3*/

        /* LPTIM3 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM3CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM3*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM3 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM3CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM3*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM3 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM3CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM3 clock*/
        /* LPTIM3 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM3CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM3 clock*/
        /* LPTIM3 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM3CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM3 clock*/
        /* LPTIM3 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM3 clock*/
      __HAL_RCC_LPTIM3_CONFIG(pPeriphClkInit->Lptim3ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* LPTIM3 */

#if defined(LPTIM4)
  /*-------------------------- LPTIM4 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM4) == RCC_PERIPHCLK_LPTIM4)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM4CLK(pPeriphClkInit->Lptim4ClockSelection));

    switch (pPeriphClkInit->Lptim4ClockSelection)
    {
      case RCC_LPTIM4CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for LPTIM4*/

        /* LPTIM4 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM4CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM4*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM4 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM4CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM4*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM4 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM4CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM4 clock*/
        /* LPTIM4 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM4CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM4 clock*/
        /* LPTIM4 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM4CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM4 clock*/
        /* LPTIM4 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM4 clock*/
      __HAL_RCC_LPTIM4_CONFIG(pPeriphClkInit->Lptim4ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* LPTIM4 */

#if defined(LPTIM5)
  /*-------------------------- LPTIM5 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM5) == RCC_PERIPHCLK_LPTIM5)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM5CLK(pPeriphClkInit->Lptim5ClockSelection));

    switch (pPeriphClkInit->Lptim5ClockSelection)
    {
      case RCC_LPTIM5CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for LPTIM5*/

        /* LPTIM5 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM5CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM5*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM5 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM5CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM5*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM5 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM5CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM5 clock*/
        /* LPTIM5 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM5CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM5 clock*/
        /* LPTIM5 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM5CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM5 clock*/
        /* LPTIM5 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM5 clock*/
      __HAL_RCC_LPTIM5_CONFIG(pPeriphClkInit->Lptim5ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* LPTIM5 */

#if defined(LPTIM6)
  /*-------------------------- LPTIM6 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LPTIM6) == RCC_PERIPHCLK_LPTIM6)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LPTIM6CLK(pPeriphClkInit->Lptim6ClockSelection));

    switch (pPeriphClkInit->Lptim6ClockSelection)
    {
      case RCC_LPTIM6CLKSOURCE_PCLK3:      /* PCLK3 is used as clock source for LPTIM6*/

        /* LPTIM6 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM6CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for LPTIM6*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LPTIM6 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM6CLKSOURCE_PLL3R:  /* PLL3 is used as clock source for LPTIM6*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LPTIM6 clock source config set later after clock selection check */
        break;
      case RCC_LPTIM6CLKSOURCE_LSE:      /* LSE clock is used as source of LPTIM6 clock*/
        /* LPTIM6 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM6CLKSOURCE_LSI:      /* LSI clock is used as source of LPTIM6 clock*/
        /* LPTIM6 clock source config set later after clock selection check */
        break;

      case RCC_LPTIM6CLKSOURCE_CLKP:      /* CLKP is used as source of LPTIM6 clock*/
        /* LPTIM6 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of LPTIM6 clock*/
      __HAL_RCC_LPTIM6_CONFIG(pPeriphClkInit->Lptim6ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* LPTIM6 */

#if defined(SAI1)
  /*-------------------------- SAI1 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI1) == RCC_PERIPHCLK_SAI1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI1CLK(pPeriphClkInit->Sai1ClockSelection));

    switch (pPeriphClkInit->Sai1ClockSelection)
    {
      case RCC_SAI1CLKSOURCE_PLL1Q:      /* PLL is used as clock source for SAI1*/
        /* Enable SAI Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* SAI1 clock source config set later after clock selection check */
        break;

      case RCC_SAI1CLKSOURCE_PLL2P:  /* PLL2 is used as clock source for SAI1*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* SAI1 clock source config set later after clock selection check */
        break;
      case RCC_SAI1CLKSOURCE_PLL3P:  /* PLL3 is used as clock source for SAI1*/
        /* PLL3 P input clock, parameters M, N & P configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* SAI1 clock source config set later after clock selection check */
        break;
      case RCC_SAI1CLKSOURCE_PIN:      /* External clock is used as source of SAI1 clock*/
        break;

      case RCC_SAI1CLKSOURCE_CLKP:      /* CLKP is used as source of SAI1 clock*/
        /* SAI1 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of SAI1 clock*/
      __HAL_RCC_SAI1_CONFIG(pPeriphClkInit->Sai1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* SAI1*/

#if defined(SAI2)
  /*-------------------------- SAI2 clock source configuration ---------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SAI2) == RCC_PERIPHCLK_SAI2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_SAI2CLK(pPeriphClkInit->Sai2ClockSelection));

    switch (pPeriphClkInit->Sai2ClockSelection)
    {
      case RCC_SAI2CLKSOURCE_PLL1Q:      /* PLL is used as clock source for SAI2*/
        /* Enable SAI Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* SAI2 clock source config set later after clock selection check */
        break;

      case RCC_SAI2CLKSOURCE_PLL2P: /* PLL2 is used as clock source for SAI2*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* SAI2 clock source config set later after clock selection check */
        break;
      case RCC_SAI2CLKSOURCE_PLL3P:  /* PLL3 is used as clock source for SAI2*/
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* SAI2 clock source config set later after clock selection check */
        break;
      case RCC_SAI2CLKSOURCE_PIN:      /* External clock is used as source of SAI2 clock*/
      case RCC_SAI2CLKSOURCE_CLKP:      /* CLKP is used as source of SAI2 clock*/
        /* SAI2 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of SAI2 clock*/
      __HAL_RCC_SAI2_CONFIG(pPeriphClkInit->Sai2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* SAI2*/

  /*-------------------------- ADCDAC clock source configuration ----------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADCDAC) == RCC_PERIPHCLK_ADCDAC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADCDACCLKSOURCE(pPeriphClkInit->AdcDacClockSelection));

    switch (pPeriphClkInit->AdcDacClockSelection)
    {

      case RCC_ADCDACCLKSOURCE_HCLK:   /* Bus clock is used as source of ADCDAC clock*/
      case RCC_ADCDACCLKSOURCE_SYSCLK: /* System clock is used as source of ADCDAC clock*/
        /* ADCDAC clock source config set later after clock selection check */
        break;

      case RCC_ADCDACCLKSOURCE_PLL2R:
        /* PLL2 input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        break;

      case RCC_ADCDACCLKSOURCE_HSE:/* HSE clock is used as source of ADCDAC clock*/
      case RCC_ADCDACCLKSOURCE_HSI:/* HSI clock is used as source of ADCDAC clock*/
      case RCC_ADCDACCLKSOURCE_CSI:/* CSI clock is used as source of ADCDAC clock*/
        /* ADCDAC clock source configuration done later after clock selection check */
        break;


      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the ADCDAC interface clock source */
      __HAL_RCC_ADCDAC_CONFIG(pPeriphClkInit->AdcDacClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

  /*-------------------------- DAC low-power clock source configuration ----------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_DAC_LP) == RCC_PERIPHCLK_DAC_LP)
  {
    /* Check the parameters */
    assert_param(IS_RCC_DACLPCLKSOURCE(pPeriphClkInit->DacLowPowerClockSelection));

    switch (pPeriphClkInit->DacLowPowerClockSelection)
    {

      case RCC_DACLPCLKSOURCE_LSE:
        /* LSE oscillator is used as source of DAC low-power clock */
        /* DAC clock source configuration done later after clock selection check */
        break;

      case RCC_DACLPCLKSOURCE_LSI:
        /* LSI is used as clock source for DAC low-power clock */
        /* DAC clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the DAC low-power interface clock source */
      __HAL_RCC_DAC_LP_CONFIG(pPeriphClkInit->DacLowPowerClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

  /*-------------------------- RTC clock source configuration ----------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RTC) == RCC_PERIPHCLK_RTC)
  {

    /* Check for RTC Parameters used to output RTCCLK */
    assert_param(IS_RCC_RTCCLKSOURCE(pPeriphClkInit->RTCClockSelection));

    /* Enable write access to Backup domain */
    SET_BIT(PWR->DBPCR, PWR_DBPCR_DBP);

    /* Wait for Backup domain Write protection disable */
    tickstart = HAL_GetTick();

    while (HAL_IS_BIT_CLR(PWR->DBPCR, PWR_DBPCR_DBP))
    {
      if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
      {
        ret = HAL_TIMEOUT;
        break;
      }
    }

    if (ret == HAL_OK)
    {
      /* Reset the Backup domain only if the RTC Clock source selection is modified from default */
      tmpregister = READ_BIT(RCC->BDCR, RCC_BDCR_RTCSEL);

      if ((tmpregister != RCC_RTCCLKSOURCE_NO_CLK)
          && (tmpregister != ((pPeriphClkInit->RTCClockSelection) & RCC_BDCR_RTCSEL)))
      {
        /* Store the content of BDCR register before the reset of Backup Domain */
        tmpregister = READ_BIT(RCC->BDCR, ~(RCC_BDCR_RTCSEL));
        /* RTC Clock selection can be changed only if the Backup Domain is reset */
        __HAL_RCC_BACKUPRESET_FORCE();
        __HAL_RCC_BACKUPRESET_RELEASE();
        /* Restore the Content of BDCR register */
        RCC->BDCR = tmpregister;
      }

      /* Wait for LSE reactivation if LSE was enable prior to Backup Domain reset */
      if (HAL_IS_BIT_SET(tmpregister, RCC_BDCR_LSEON))
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till LSE is ready */
        while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
          {
            ret = HAL_TIMEOUT;
            break;
          }
        }
      }

      if (ret == HAL_OK)
      {
        /* Apply new RTC clock source selection */
        __HAL_RCC_RTC_CONFIG(pPeriphClkInit->RTCClockSelection);
      }
      else
      {
        /* set overall return value */
        status = ret;
      }
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

  /*------------------------------ RNG Configuration -------------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_RNG) == RCC_PERIPHCLK_RNG)
  {

    /* Check the parameters */
    assert_param(IS_RCC_RNGCLKSOURCE(pPeriphClkInit->RngClockSelection));

    switch (pPeriphClkInit->RngClockSelection)
    {

      case RCC_RNGCLKSOURCE_HSI48: /* HSI48 is used as clock source for RNG*/

        /* RNG clock source configuration done later after clock selection check */
        break;

      case RCC_RNGCLKSOURCE_PLL1Q: /* PLL1 is used as clock source for RNG*/
        /* Enable PLL1Q Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* RNG clock source configuration done later after clock selection check */
        break;
      case RCC_RNGCLKSOURCE_LSE:
        /* LSE oscillator is used as source of RNG clock */
        /* RNG clock source configuration done later after clock selection check */
        break;

      case RCC_RNGCLKSOURCE_LSI: /* HSI48 is used as clock source for RNG*/

        /* RNG clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of RNG clock*/
      __HAL_RCC_RNG_CONFIG(pPeriphClkInit->RngClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

#if defined(SDMMC1)
  /*-------------------------- SDMMC1 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC1) == RCC_PERIPHCLK_SDMMC1)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SDMMC1CLKSOURCE(pPeriphClkInit->Sdmmc1ClockSelection));

    switch (pPeriphClkInit->Sdmmc1ClockSelection)
    {
      case RCC_SDMMC1CLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for SDMMC1 kernel clock*/
        /* Enable PLL1Q Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* SDMMC1 kernel clock source config set later after clock selection check */
        break;

      case RCC_SDMMC1CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for SDMMC1 kernel clock*/
        /* PLL2R input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* SDMMC1 kernel clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SDMMC1 clock source */
      __HAL_RCC_SDMMC1_CONFIG(pPeriphClkInit->Sdmmc1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* SDMMC1 */

#if defined(SDMMC2)
  /*-------------------------- SDMMC2 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SDMMC2) == RCC_PERIPHCLK_SDMMC2)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SDMMC2CLKSOURCE(pPeriphClkInit->Sdmmc2ClockSelection));

    switch (pPeriphClkInit->Sdmmc2ClockSelection)
    {
      case RCC_SDMMC2CLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for SDMMC2 kernel clock*/
        /* Enable PLL1Q Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* SDMMC2 kernel clock source config set later after clock selection check */
        break;

      case RCC_SDMMC2CLKSOURCE_PLL2R:  /* PLL2 is used as clock source for SDMMC2 kernel clock*/
        /* PLL2R input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* SDMMC2 kernel clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SDMMC2 clock source */
      __HAL_RCC_SDMMC2_CONFIG(pPeriphClkInit->Sdmmc2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* SDMMC2 */

  /*-------------------------- SPI1 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI1) == RCC_PERIPHCLK_SPI1)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI1CLKSOURCE(pPeriphClkInit->Spi1ClockSelection));

    switch (pPeriphClkInit->Spi1ClockSelection)
    {
      case RCC_SPI1CLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for SPI1 */
        /* Enable SPI Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);

        /* SPI1 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI1CLKSOURCE_PLL2P: /* PLL2 is used as clock source for SPI1*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI1 clock source configuration done later after clock selection check */
        break;

#if defined(RCC_SPI1CLKSOURCE_PLL3P)
      case RCC_SPI1CLKSOURCE_PLL3P:  /* PLL3 is used as clock source for SPI1 */
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI1 clock source configuration done later after clock selection check */
        break;
#endif /* RCC_SPI1CLKSOURCE_PLL3P */

      case RCC_SPI1CLKSOURCE_PIN:
        /* External clock is used as source of SPI1 clock*/
        /* SPI1 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI1CLKSOURCE_CLKP:
        /* HSI, HSE, or CSI oscillator is used as source of SPI1 clock */
        /* SPI1 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI1 clock source */
      __HAL_RCC_SPI1_CONFIG(pPeriphClkInit->Spi1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

  /*-------------------------- SPI2 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI2) == RCC_PERIPHCLK_SPI2)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI2CLKSOURCE(pPeriphClkInit->Spi2ClockSelection));

    switch (pPeriphClkInit->Spi2ClockSelection)
    {
      case RCC_SPI2CLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for SPI2 */
        /* Enable SPI Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);

        /* SPI2 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI2CLKSOURCE_PLL2P: /* PLL2 is used as clock source for SPI2*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI2 clock source configuration done later after clock selection check */
        break;

#if defined(RCC_SPI2CLKSOURCE_PLL3P)
      case RCC_SPI2CLKSOURCE_PLL3P:  /* PLL3 is used as clock source for SPI2 */
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI2 clock source configuration done later after clock selection check */
        break;
#endif /* RCC_SPI2CLKSOURCE_PLL3P */

      case RCC_SPI2CLKSOURCE_PIN:
        /* External clock is used as source of SPI2 clock*/
        /* SPI2 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI2CLKSOURCE_CLKP:
        /* HSI, HSE, or CSI oscillator is used as source of SPI2 clock */
        /* SPI2 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI2 clock source */
      __HAL_RCC_SPI2_CONFIG(pPeriphClkInit->Spi2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

  /*-------------------------- SPI3 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI3) == RCC_PERIPHCLK_SPI3)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI3CLKSOURCE(pPeriphClkInit->Spi3ClockSelection));

    switch (pPeriphClkInit->Spi3ClockSelection)
    {
      case RCC_SPI3CLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for SPI3 */
        /* Enable SPI Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);

        /* SPI3 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI3CLKSOURCE_PLL2P: /* PLL2 is used as clock source for SPI3*/
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI3 clock source configuration done later after clock selection check */
        break;

#if defined(RCC_SPI3CLKSOURCE_PLL3P)
      case RCC_SPI3CLKSOURCE_PLL3P:  /* PLL3 is used as clock source for SPI3 */
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI3 clock source configuration done later after clock selection check */
        break;
#endif /* RCC_SPI3CLKSOURCE_PLL3P */

      case RCC_SPI3CLKSOURCE_PIN:
        /* External clock is used as source of SPI3 clock*/
        /* SPI3 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI3CLKSOURCE_CLKP:
        /* HSI, HSE, or CSI oscillator is used as source of SPI3 clock */
        /* SPI3 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI3 clock source */
      __HAL_RCC_SPI3_CONFIG(pPeriphClkInit->Spi3ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }

#if defined(SPI4)
  /*-------------------------- SPI4 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI4) == RCC_PERIPHCLK_SPI4)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI4CLKSOURCE(pPeriphClkInit->Spi4ClockSelection));

    switch (pPeriphClkInit->Spi4ClockSelection)
    {
      case RCC_SPI4CLKSOURCE_PCLK2:  /* PCLK2 (APB2 Clock) is used as clock source for SPI4 */
        /* SPI4 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI4CLKSOURCE_PLL2Q: /* PLL2 is used as clock source for SPI4*/
        /* PLL2 Q input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI4 clock source configuration done later after clock selection check */
        break;
#if defined(RCC_SPI4CLKSOURCE_PLL3Q)
      case RCC_SPI4CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for SPI4 */
        /* PLL3 Q input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI4 clock source configuration done later after clock selection check */
        break;
#endif /* RCC_SPI4CLKSOURCE_PLL3Q */
      case RCC_SPI4CLKSOURCE_HSI:
        /* HSI oscillator is used as source of SPI4 clock*/
        /* SPI4 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI4CLKSOURCE_CSI:
        /*  CSI oscillator is used as source of SPI4 clock */
        /* SPI4 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI4CLKSOURCE_HSE:
        /*  HSE oscillator is used as source of SPI4 clock */
        /* SPI4 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI4 clock source */
      __HAL_RCC_SPI4_CONFIG(pPeriphClkInit->Spi4ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* SPI4 */

#if defined(SPI5)
  /*-------------------------- SPI5 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI5) == RCC_PERIPHCLK_SPI5)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI5CLKSOURCE(pPeriphClkInit->Spi5ClockSelection));

    switch (pPeriphClkInit->Spi5ClockSelection)
    {
      case RCC_SPI5CLKSOURCE_PCLK3:  /* PCLK3 (APB3 Clock) is used as clock source for SPI5 */
        /* SPI5 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI5CLKSOURCE_PLL2Q: /* PLL2 is used as clock source for SPI5*/
        /* PLL2 Q input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI5 clock source configuration done later after clock selection check */
        break;
      case RCC_SPI5CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for SPI5 */
        /* PLL3 Q input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI5 clock source configuration done later after clock selection check */
        break;
      case RCC_SPI5CLKSOURCE_HSI:
        /* HSI oscillator is used as source of SPI5 clock*/
        /* SPI5 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI5CLKSOURCE_CSI:
        /*  CSI oscillator is used as source of SPI5 clock */
        /* SPI5 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI5CLKSOURCE_HSE:
        /*  HSE oscillator is used as source of SPI5 clock */
        /* SPI5 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI5 clock source */
      __HAL_RCC_SPI5_CONFIG(pPeriphClkInit->Spi5ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* SPI5 */

#if defined(SPI6)
  /*-------------------------- SPI6 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_SPI6) == RCC_PERIPHCLK_SPI6)
  {

    /* Check the parameters */
    assert_param(IS_RCC_SPI6CLKSOURCE(pPeriphClkInit->Spi6ClockSelection));

    switch (pPeriphClkInit->Spi6ClockSelection)
    {
      case RCC_SPI6CLKSOURCE_PCLK2:  /* PCLK2 (APB2 Clock) is used as clock source for SPI6 */
        /* SPI6 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI6CLKSOURCE_PLL2Q: /* PLL2 is used as clock source for SPI6*/
        /* PLL2 Q input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));

        /* SPI6 clock source configuration done later after clock selection check */
        break;
      case RCC_SPI6CLKSOURCE_PLL3Q:  /* PLL3 is used as clock source for SPI6 */
        /* PLL3 Q input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));

        /* SPI6 clock source configuration done later after clock selection check */
        break;
      case RCC_SPI6CLKSOURCE_HSI:
        /* HSI oscillator is used as source of SPI6 clock*/
        /* SPI6 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI6CLKSOURCE_CSI:
        /*  CSI oscillator is used as source of SPI6 clock */
        /* SPI6 clock source configuration done later after clock selection check */
        break;

      case RCC_SPI6CLKSOURCE_HSE:
        /*  HSE oscillator is used as source of SPI6 clock */
        /* SPI6 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the SPI6 clock source */
      __HAL_RCC_SPI6_CONFIG(pPeriphClkInit->Spi6ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* SPI6 */

#if defined(OCTOSPI1)
  /*-------------------------- OctoSPIx clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_OSPI) == RCC_PERIPHCLK_OSPI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_OSPICLKSOURCE(pPeriphClkInit->OspiClockSelection));

    switch (pPeriphClkInit->OspiClockSelection)
    {
      case RCC_OSPICLKSOURCE_HCLK:      /* HCLK is used as clock source for OCTOSPI */

        /* OCTOSPI clock source config set later after clock selection check */
        break;

      case RCC_OSPICLKSOURCE_PLL1Q:  /* PLL1 Q is used as clock source for OCTOSPI*/

        /* Enable PLL1 Q CLK output */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        break;

      case RCC_OSPICLKSOURCE_PLL2R:  /* PLL2 is used as clock source for OCTOSPI*/
        /* PLL2 R input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* OCTOSPI clock source config set later after clock selection check */
        break;

      case RCC_OSPICLKSOURCE_CLKP:  /* CLKP is used as source of OCTOSPI clock*/
        /* OCTOSPI clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the OctoSPI clock source */
      __HAL_RCC_OSPI_CONFIG(pPeriphClkInit->OspiClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* OCTOSPI1*/

  /*-------------------------- FDCAN kernel clock source configuration -------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_FDCAN) == RCC_PERIPHCLK_FDCAN)
  {
    assert_param(IS_RCC_FDCANCLK(pPeriphClkInit->FdcanClockSelection));

    switch (pPeriphClkInit->FdcanClockSelection)
    {
      case RCC_FDCANCLKSOURCE_HSE:      /* HSE is used as source of FDCAN kernel clock*/
        /* FDCAN kernel clock source config set later after clock selection check */
        break;

      case RCC_FDCANCLKSOURCE_PLL1Q:      /* PLL1 is used as clock source for FDCAN kernel clock*/
        /* Enable PLL1Q Clock output generated from System PLL . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* FDCAN kernel clock source config set later after clock selection check */
        break;

      case RCC_FDCANCLKSOURCE_PLL2Q:  /* PLL2 is used as clock source for FDCAN kernel clock*/
        /* PLL2Q input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* FDCAN kernel clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of FDCAN kernel clock*/
      __HAL_RCC_FDCAN_CONFIG(pPeriphClkInit->FdcanClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }

#if defined(USB_DRD_FS)
  /*------------------------------ USB Configuration -------------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_USB) == RCC_PERIPHCLK_USB)
  {

    /* Check the parameters */
    assert_param(IS_RCC_USBCLKSOURCE(pPeriphClkInit->UsbClockSelection));

    switch (pPeriphClkInit->UsbClockSelection)
    {
      case RCC_USBCLKSOURCE_PLL1Q:      /* PLL is used as clock source for USB*/
        /* Enable USB Clock output generated form System USB . */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);

        /* USB clock source configuration done later after clock selection check */
        break;

#if defined(RCC_USBCLKSOURCE_PLL3Q)
      case RCC_USBCLKSOURCE_PLL3Q: /* PLL3 is used as clock source for USB*/
        /* PLL3Q input clock, parameters M, N & Q configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
#else
      case RCC_USBCLKSOURCE_PLL2Q: /* PLL2 is used as clock source for USB*/
        /* PLL2Q input clock, parameters M, N & Q configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
#endif /* RCC_USBCLKSOURCE_PLL3Q */
        /* USB clock source configuration done later after clock selection check */
        break;

      case RCC_USBCLKSOURCE_HSI48:
        /* HSI48 oscillator is used as source of USB clock */
        /* USB clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USB clock*/
      __HAL_RCC_USB_CONFIG(pPeriphClkInit->UsbClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }

  }
#endif /* USB_DRD_FS */

#if defined(CEC)
  /*-------------------------- CEC clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_CEC) == RCC_PERIPHCLK_CEC)
  {

    /* Check the parameters */
    assert_param(IS_RCC_CECCLKSOURCE(pPeriphClkInit->CecClockSelection));

    /* Configure the CEC clock source */
    __HAL_RCC_CEC_CONFIG(pPeriphClkInit->CecClockSelection);

  }
#endif /* CEC */

#if defined (PLAY1)
  /*-------------------------- PLAY1 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_PLAY1) == RCC_PERIPHCLK_PLAY1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_PLAY1CLKSOURCE(pPeriphClkInit->PLAY1ClockSelection));
    assert_param(IS_RCC_PLAY1_DIV(pPeriphClkInit->PLAY1ClockDivider));

    switch (pPeriphClkInit->PLAY1ClockSelection)
    {
      case RCC_PLAY1CLKSOURCE_PCLK3:  /* PCLK3 (APB3 Clock) is used as clock source for PLAY1 */
        /* PLAY1 clock source configuration done later after clock selection check */
        break;

      case RCC_PLAY1CLKSOURCE_PLL2P:  /* PLL2 P is used as clock source for PLAY1 kernel clock*/
        /* PLL2P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* PLAY1 kernel clock source config set later after clock selection check */
        break;
#if defined(RCC_PLAY1CLKSOURCE_PLL3R)
      case RCC_PLAY1CLKSOURCE_PLL3R:  /* PLL3 R is used as clock source for PLAY1 kernel clock*/
        /* PLL3R input clock, parameters M, N & R configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* PLAY1 kernel clock source config set later after clock selection check */
        break;
#endif /* RCC_PLAY1CLKSOURCE_PLL3R */
      case RCC_PLAY1CLKSOURCE_LSE:  /* LSE is used as clock source for PLAY1 */
        /* PLAY1 clock source configuration done later after clock selection check */
        break;

      case RCC_PLAY1CLKSOURCE_LSI:  /* LSI is used as clock source for PLAY1 */
        /* PLAY1 clock source configuration done later after clock selection check */
        break;

      case RCC_PLAY1CLKSOURCE_CLKP:  /* CLKP is used as clock source for PLAY1 */
        /* PLAY1 clock source configuration done later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of PLAY1 clock*/
      __HAL_RCC_PLAY1_CONFIG(pPeriphClkInit->PLAY1ClockSelection);

      /* Set the divider for PLAY1 clock (N: 0..15, divider = 2^N) */
      __HAL_RCC_PLAY1_DIV_CONFIG(pPeriphClkInit->PLAY1ClockDivider);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /*PLAY1*/

#if defined (USB_OTG_FS)
  /*-------------------------- USB_OTG_FS clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_OTGFS) == RCC_PERIPHCLK_OTGFS)
  {

    /* Check the parameters */
    assert_param(IS_RCC_OTGFSCLKSOURCE(pPeriphClkInit->OtgfsClockSelection));

    switch (pPeriphClkInit->OtgfsClockSelection)
    {
      case RCC_OTGFSCLKSOURCE_HSI48:  /* HSI48 is used as clock source for USB_OTG_FS */
        /* USB_OTG_FS clock source configuration done later after clock selection check */
        break;

      case RCC_OTGFSCLKSOURCE_PLL1Q:  /* PLL1 Q is used as clock source for USB_OTG_FS kernel clock*/
        /* PLL1Q input clock, parameters M, N & Q configuration and clock output (PLL1ClockOut) */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* USB_OTG_FS kernel clock source config set later after clock selection check */
        break;

      case RCC_OTGFSCLKSOURCE_PLL3Q:  /* PLL3 Q is used as clock source for USB_OTG_FS kernel clock*/
        /* PLL3Q input clock, parameters M, N & Q configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USB_OTG_FS kernel clock source config set later after clock selection check */
        break;
#if defined(PWR_USBSCR_OTGHSEN)
      case RCC_OTGFSCLKSOURCE_CLK48:  /* OTG_HS PHY CLK48 output is used as clock source for USB_OTG_FS */
        /* USB_OTG_FS clock source configuration done later after clock selection check */
        break;
#endif /* PWR_USBSCR_OTGHSEN */
      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USB_OTG_FS clock*/
      __HAL_RCC_OTGFS_CONFIG(pPeriphClkInit->OtgfsClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* USB_OTG_FS */

#if defined (USB_OTG_HS)
  /*-------------------------- USB_OTG_HS clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_OTGHS) == RCC_PERIPHCLK_OTGHS)
  {

    /* Check the parameters */
    assert_param(IS_RCC_OTGHSCLKSOURCE(pPeriphClkInit->OtghsClockSelection));

    switch (pPeriphClkInit->OtghsClockSelection)
    {
      case RCC_OTGHSCLKSOURCE_HSE:  /* HSE is used as clock source for USB_OTG_HS */
        /* USB_OTG_HS clock source configuration done later after clock selection check */
        break;

      case RCC_OTGHSCLKSOURCE_PLL3Q:  /* PLL3 Q is used as clock source for USB_OTG_HS kernel clock*/
        /* PLL3Q input clock, parameters M, N & Q configuration and clock output (PLL1ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* USB_OTG_HS kernel clock source config set later after clock selection check */
        break;

      case RCC_OTGHSCLKSOURCE_HSE_DIV2:  /* HSE div 2 is used as clock source for USB_OTG_HS */
        /* USB_OTG_HS kernel clock source config set later after clock selection check */
        break;

      case RCC_OTGHSCLKSOURCE_PLL1Q_DIV2:  /* PLL1 Q div 2 is used as clock source for USB_OTG_HS */
        /* Enable PLL1 Clock output generated from System PLL . */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* USB_OTG_HS clock source configuration done later after clock selection check */
        break;
      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Set the source of USB clock*/
      __HAL_RCC_OTGHS_CONFIG(pPeriphClkInit->OtghsClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* USB_OTG_HS */

#if defined(OCTOSPI2)
  /*-------------------------- OctoSPI2 clock source configuration ----------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_OSPI2) == RCC_PERIPHCLK_OSPI2)
  {
    /* Check the parameters */
    assert_param(IS_RCC_OSPI2CLKSOURCE(pPeriphClkInit->Ospi2ClockSelection));

    switch (pPeriphClkInit->Ospi2ClockSelection)
    {
      case RCC_OSPI2CLKSOURCE_HCLK:      /* HCLK is used as clock source for OCTOSPI2 */

        /* OCTOSPI2 clock source config set later after clock selection check */
        break;

      case RCC_OSPI2CLKSOURCE_PLL1Q:  /* PLL1 Q is used as clock source for OCTOSPI2*/

        /* Enable PLL1 Q CLK output */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVQ);
        break;

      case RCC_OSPI2CLKSOURCE_PLL2R:  /* PLL2 R is used as clock source for OCTOSPI2*/
        /* PLL2 R input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* OCTOSPI clock source config set later after clock selection check */
        break;

      case RCC_OSPI2CLKSOURCE_CLKP:  /* CLKP is used as source of OCTOSPI2 clock*/
        /* OCTOSPI2 clock source config set later after clock selection check */
        break;

      default:
        ret = HAL_ERROR;
        break;
    }

    if (ret == HAL_OK)
    {
      /* Configure the OctoSPI2 clock source */
      __HAL_RCC_OSPI2_CONFIG(pPeriphClkInit->Ospi2ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* OCTOSPI2*/

#if defined (LTDC)
  /*-------------------------- LTDC clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_LTDC) == RCC_PERIPHCLK_LTDC)
  {
    /* Check the parameters */
    assert_param(IS_RCC_LTDCCLKSOURCE(pPeriphClkInit->LtdcClockSelection));

    switch (pPeriphClkInit->LtdcClockSelection)
    {
      case RCC_LTDCCLKSOURCE_PLL3R:          /* PLL3 R is used as clock source for LTDC*/
        /* PLL3 R input clock, parameters M, N & R configuration clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* LTDC clock source config set later after clock selection check */
        break;
      case RCC_LTDCCLKSOURCE_PLL2R:          /* PLL2 R is used as clock source for LTDC*/
        /* PLL2 R input clock, parameters M, N & R configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* LTDC clock source config set later after clock selection check */
        break;
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the LTDC interface clock source */
      __HAL_RCC_LTDC_CONFIG(pPeriphClkInit->LtdcClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* LTDC */

#if defined (MDF1)
  /*-------------------------- MDF1 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_MDF1) == RCC_PERIPHCLK_MDF1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_MDF1CLKSOURCE(pPeriphClkInit->Mdf1ClockSelection));

    switch (pPeriphClkInit->Mdf1ClockSelection)
    {
      case RCC_MDF1CLKSOURCE_PLL1Q:         /* PLL1 Q is used as clock source for MDF1 */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        break;
      case RCC_MDF1CLKSOURCE_PLL2P:         /* PLL2 P is used as clock source for MDF1 */
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* MDF1 clock source configuration done later after clock selection check */
        break;
      case RCC_MDF1CLKSOURCE_PLL3P:         /* PLL3 P is used as clock source for MDF1 */
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* MDF1 clock source configuration done later after clock selection check */
        break;
      case RCC_MDF1CLKSOURCE_PIN:           /* External clock is used as clock source of MDF1 */
        /* MDF1 clock source configuration done later after clock selection check */
        break;
      case RCC_MDF1CLKSOURCE_CLKP:          /* CLKP is used as clock source for MDF1 */
        /* MDF1 clock source configuration done later after clock selection check */
        break;
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the MDF1 interface clock source */
      __HAL_RCC_MDF1_CONFIG(pPeriphClkInit->Mdf1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* MDF1 */

#if defined (RCC_CCIPR4_ETHCLKSEL)
  /*-------------------------- ETH clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ETH) == RCC_PERIPHCLK_ETH)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ETHCLKSOURCE(pPeriphClkInit->EthClockSelection));

    switch (pPeriphClkInit->EthClockSelection)
    {
      case RCC_ETHCLKSOURCE_HSE:            /* HSE is used as clock source for ETH */
        /* ETH clock source configuration done later after clock selection check */
        break;
      case RCC_ETHCLKSOURCE_PLL1Q:          /* PLL1 Q is used as clock source for ETH */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* ETH clock source configuration done later after clock selection check */
        break;
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the ETH interface clock source */
      __HAL_RCC_ETH_CONFIG(pPeriphClkInit->EthClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined (ADF1)
  /*-------------------------- ADF1 clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ADF1) == RCC_PERIPHCLK_ADF1)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ADF1CLKSOURCE(pPeriphClkInit->Adf1ClockSelection));

    switch (pPeriphClkInit->Adf1ClockSelection)
    {
      case RCC_ADF1CLKSOURCE_PLL1Q:         /* PLL1 Q is used as clock source for ADF1 */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* ADF1 clock source configuration done later after clock selection check */
        break;
      case RCC_ADF1CLKSOURCE_PLL2P:         /* PLL2 P is used as clock source for ADF1 */
        /* PLL2 P input clock, parameters M, N & P configuration and clock output (PLL2ClockOut) */
        ret = RCCEx_PLL2_Config(&(pPeriphClkInit->PLL2));
        /* ADF1 clock source configuration done later after clock selection check */
        break;
      case RCC_ADF1CLKSOURCE_PLL3P:         /* PLL3 P is used as clock source for ADF1 */
        /* PLL3 P input clock, parameters M, N & P configuration and clock output (PLL3ClockOut) */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        break;
      case RCC_ADF1CLKSOURCE_PIN:           /* External clock is used as source of ADF1 clock*/
        /* ADF1 clock source configuration done later after clock selection check */
        break;
      case RCC_ADF1CLKSOURCE_CLKP:          /* CLKP is used as clock source for ADF1 */
        /* ADF1 clock source configuration done later after clock selection check */
        break;
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the ADF1 interface clock source */
      __HAL_RCC_ADF1_CONFIG(pPeriphClkInit->Adf1ClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* ADF1 */

#if defined (RCC_CCIPR5_ETHPTPCLKSEL)
  /*-------------------------- ETHPTP clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ETHPTP) == RCC_PERIPHCLK_ETHPTP)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ETHPTPCLKSOURCE(pPeriphClkInit->EthptpClockSelection));
    assert_param(IS_RCC_ETHPTP_DIV(pPeriphClkInit->EthptpClockDivider));

    switch (pPeriphClkInit->EthptpClockSelection)
    {
      case RCC_ETHPTPCLKSOURCE_HCLK:        /* HCLK is used as clock source for ETHPTP */
        /* ETHPTP clock source configuration done later after clock selection check */
        break;
      case RCC_ETHPTPCLKSOURCE_PLL1R:       /* PLL1 R is used as clock source for ETHPTP */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVR);
        /* ETHPTP clock source configuration done later after clock selection check */
        break;
      case RCC_ETHPTPCLKSOURCE_PLL1Q:       /* PLL1 Q is used as clock source for ETHPTP */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        /* ETHPTP clock source configuration done later after clock selection check */
        break;
#if defined(RCC_ETHPTPCLKSOURCE_PLL3P)
      case RCC_ETHPTPCLKSOURCE_PLL3P:       /* PLL3 P is used as clock source for ETHPTP */
        /* Enable PLL3 Clock output generated from System PLL  */
        ret = RCCEx_PLL3_Config(&(pPeriphClkInit->PLL3));
        /* ETHPTP clock source configuration done later after clock selection check */
        break;
#endif /* RCC_ETHPTPCLKSOURCE_PLL3P */
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the ETHPTP interface clock source */
      __HAL_RCC_ETHPTP_CONFIG(pPeriphClkInit->EthptpClockSelection);

      /* Set the divider for ETHPTP clock (N: 0..15, divider = N) */
      __HAL_RCC_ETHPTP_DIV_CONFIG(pPeriphClkInit->EthptpClockDivider);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined (RCC_CCIPR5_ETHREFCLKSEL)
  /*-------------------------- ETHREF clock source configuration -------------------*/
  if (((pPeriphClkInit->PeriphClockSelection) & RCC_PERIPHCLK_ETHREF) == RCC_PERIPHCLK_ETHREF)
  {
    /* Check the parameters */
    assert_param(IS_RCC_ETHREFCLKSOURCE(pPeriphClkInit->EthrefClockSelection));

    switch (pPeriphClkInit->EthrefClockSelection)
    {
      case RCC_ETHREFCLKSOURCE_PLL1Q:       /* PLL1 Q is used as clock source for ETHREF */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVQ);
        break;
      case RCC_ETHREFCLKSOURCE_PLL1R:       /* PLL1 R is used as clock source for ETHREF */
        /* Enable PLL1 Clock output generated from System PLL  */
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLL1_DIVR);
        break;
      default:
        ret = HAL_ERROR;
        break;
    }
    if (ret == HAL_OK)
    {
      /* Configure the ETHREF interface clock source */
      __HAL_RCC_ETHREF_CONFIG(pPeriphClkInit->EthrefClockSelection);
    }
    else
    {
      /* set overall return value */
      status = ret;
    }
  }
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

  return status;
}

/**
  * @brief  Get the pPeriphClkInit according to the internal RCC configuration registers.
  * @param  pPeriphClkInit  pointer to an RCC_PeriphCLKInitTypeDef structure that
  *         returns the configuration information for the Extended Peripherals
  *         clocks (ADC12, DAC, SDMMC1, SDMMC2, OCTOSPI1, TIM, LPTIM1, LPTIM2, LPTIM3, LPTIM4, LPTIM5, LPTIM6,
  *         SPI1, SPI2, SPI3, SPI4, SPI5, SPI6, USART1, USART2, USART3, UART4, UART5, USART6, UART7, UART8,
  *         UART9, USART10, USART11, UART12, LPUART1, I2C1, I2C2, I2C3, I2C4, I3C1, I3C2, CEC, FDCAN, SAI1,
  *         SAI2, USB, PLAY1), PLL2 and PLL3.
  * @retval None
  */
void HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *pPeriphClkInit)
{
  /* Set all possible values for the extended clock type parameter------------*/
  pPeriphClkInit->PeriphClockSelection = RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3  | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2     | \
                                         RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | RCC_PERIPHCLK_ADCDAC  | \
                                         RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RTC  | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_I3C1 | RCC_PERIPHCLK_SPI1 | RCC_PERIPHCLK_SPI2 | \
                                         RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_CKPER;

#if defined(USB_DRD_FS)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USB;
#endif /* USB_DRD_FS */
#if defined(UART4)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART4;
#endif /* UART4 */
#if defined(UART5)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART5;
#endif /* UART5 */
#if defined(USART6)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USART6;
#endif /* UART6 */
#if defined(UART7)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART7;
#endif /* UART7 */
#if defined(UART8)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART8;
#endif /* UART8 */
#if defined(UART9)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART9;
#endif /* UART9 */
#if defined(USART10)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USART10;
#endif /* UART10 */
#if defined(USART11)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_USART11;
#endif /* UART11 */
#if defined(UART12)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_UART12;
#endif /* UART12 */
#if defined(I2C3)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2C3;
#endif /* I2C3 */
#if defined(I2C4)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I2C4;
#endif /* I2C4 */
#if defined(I3C2)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_I3C2;
#endif /* I3C2 */
#if defined(LPTIM3)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_LPTIM3;
#endif /* LPTIM3 */
#if defined(LPTIM4)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_LPTIM4;
#endif /* LPTIM4 */
#if defined(LPTIM5)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_LPTIM5;
#endif /* LPTIM5 */
#if defined(LPTIM6)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_LPTIM6;
#endif /* LPTIM6 */
#if defined(SPI4)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SPI4;
#endif /* SPI4 */
#if defined(SPI5)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SPI5;
#endif /* SPI5 */
#if defined(SPI6)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SPI6;
#endif /* SPI6 */
#if defined(SAI1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SAI1;
#endif /* SAI1 */
#if defined(SAI2)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SAI2;
#endif /* SAI2 */
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_FDCAN;
#if defined(SDMMC1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SDMMC1;
#endif /* SDMMC1*/
#if defined(SDMMC2)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_SDMMC2;
#endif /* SDMMC2*/
#if defined(OCTOSPI1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_OSPI;
#endif /* OCTOSPI1 */
#if defined(CEC)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_CEC;
#endif /* CEC */
#if defined(PLAY1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_PLAY1;
#endif /* PLAY1 */
#if defined(USB_OTG_FS)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_OTGFS;
#endif /* USB_OTG_FS */
#if defined(USB_OTG_HS)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_OTGHS;
#endif /* USB_OTG_HS */
#if defined(OCTOSPI2)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_OSPI2;
#endif /* OCTOSPI2 */
#if defined(LTDC)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_LTDC;
#endif /* LTDC */
#if defined(ADF1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ADF1;
#endif /* ADF1 */
#if defined(MDF1)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_MDF1;
#endif /* MDF1 */
#if defined(RCC_CCIPR4_ETHCLKSEL)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ETH;
#endif /* RCC_CCIPR4_ETHCLKSEL */
#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ETHPTP;
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */
#if defined(RCC_CCIPR5_ETHREFCLKSEL)
  pPeriphClkInit->PeriphClockSelection |= RCC_PERIPHCLK_ETHREF;
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

  /* Get the PLL2 Clock configuration -----------------------------------------------*/
  pPeriphClkInit->PLL2.PLL2Source = (uint32_t)((RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2SRC) >> RCC_PLL2CFGR_PLL2SRC_Pos);
  pPeriphClkInit->PLL2.PLL2M = (uint32_t)((RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2M) >> RCC_PLL2CFGR_PLL2M_Pos);
  pPeriphClkInit->PLL2.PLL2N = (uint32_t)((RCC->PLL2DIVR & RCC_PLL2DIVR_PLL2N) >> RCC_PLL2DIVR_PLL2N_Pos) + 1U;
  pPeriphClkInit->PLL2.PLL2P = (uint32_t)((RCC->PLL2DIVR & RCC_PLL2DIVR_PLL2P) >> RCC_PLL2DIVR_PLL2P_Pos) + 1U;
  pPeriphClkInit->PLL2.PLL2Q = (uint32_t)((RCC->PLL2DIVR & RCC_PLL2DIVR_PLL2Q) >> RCC_PLL2DIVR_PLL2Q_Pos) + 1U;
  pPeriphClkInit->PLL2.PLL2R = (uint32_t)((RCC->PLL2DIVR & RCC_PLL2DIVR_PLL2R) >> RCC_PLL2DIVR_PLL2R_Pos) + 1U;
  pPeriphClkInit->PLL2.PLL2RGE = (uint32_t)((RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2RGE) >> RCC_PLL2CFGR_PLL2RGE_Pos);
  pPeriphClkInit->PLL2.PLL2FRACN = (uint32_t)((RCC->PLL2FRACR & RCC_PLL2FRACR_PLL2FRACN) >> \
                                              RCC_PLL2FRACR_PLL2FRACN_Pos);

#if defined(RCC_CR_PLL3ON)
  /* Get the PLL3 Clock configuration -----------------------------------------------*/
  pPeriphClkInit->PLL3.PLL3Source = (uint32_t)((RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3SRC) >> RCC_PLL3CFGR_PLL3SRC_Pos);
  pPeriphClkInit->PLL3.PLL3M = (uint32_t)((RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3M) >> RCC_PLL3CFGR_PLL3M_Pos);
  pPeriphClkInit->PLL3.PLL3N = (uint32_t)((RCC->PLL3DIVR & RCC_PLL3DIVR_PLL3N) >> RCC_PLL3DIVR_PLL3N_Pos) + 1U;
  pPeriphClkInit->PLL3.PLL3P = (uint32_t)((RCC->PLL3DIVR & RCC_PLL3DIVR_PLL3P) >> RCC_PLL3DIVR_PLL3P_Pos) + 1U;
  pPeriphClkInit->PLL3.PLL3Q = (uint32_t)((RCC->PLL3DIVR & RCC_PLL3DIVR_PLL3Q) >> RCC_PLL3DIVR_PLL3Q_Pos) + 1U;
  pPeriphClkInit->PLL3.PLL3R = (uint32_t)((RCC->PLL3DIVR & RCC_PLL3DIVR_PLL3R) >> RCC_PLL3DIVR_PLL3R_Pos) + 1U;
  pPeriphClkInit->PLL3.PLL3RGE = (uint32_t)((RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3RGE) >> RCC_PLL3CFGR_PLL3RGE_Pos);
  pPeriphClkInit->PLL3.PLL3FRACN = (uint32_t)((RCC->PLL3FRACR & RCC_PLL3FRACR_PLL3FRACN) >> \
                                              RCC_PLL3FRACR_PLL3FRACN_Pos);
#endif /* RCC_CR_PLL3ON */

  /* Get the USART1 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart1ClockSelection = __HAL_RCC_GET_USART1_SOURCE();

  /* Get the USART2 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart2ClockSelection = __HAL_RCC_GET_USART2_SOURCE();

  /* Get the USART3 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart3ClockSelection = __HAL_RCC_GET_USART3_SOURCE();

#if defined(UART4)
  /* Get the UART4 clock source ----------------------------------------------*/
  pPeriphClkInit->Uart4ClockSelection = __HAL_RCC_GET_UART4_SOURCE();
#endif /* UART4 */

#if defined(UART5)
  /* Get the UART5 clock source ----------------------------------------------*/
  pPeriphClkInit->Uart5ClockSelection = __HAL_RCC_GET_UART5_SOURCE();
#endif /* UART5 */

#if defined(USART6)
  /* Get the USART6 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart6ClockSelection = __HAL_RCC_GET_USART6_SOURCE();
#endif /* USART6 */

#if defined(UART7)
  /* Get the UART7 clock source ---------------------------------------------*/
  pPeriphClkInit->Uart7ClockSelection = __HAL_RCC_GET_UART7_SOURCE();
#endif /* UART7 */

#if defined(UART8)
  /* Get the UART8 clock source ---------------------------------------------*/
  pPeriphClkInit->Uart8ClockSelection = __HAL_RCC_GET_UART8_SOURCE();
#endif /* UART8 */

#if defined(UART9)
  /* Get the UART9 clock source ---------------------------------------------*/
  pPeriphClkInit->Uart9ClockSelection = __HAL_RCC_GET_UART9_SOURCE();
#endif /* UART9 */

#if defined(USART10)
  /* Get the USART10 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart10ClockSelection = __HAL_RCC_GET_USART10_SOURCE();
#endif /* USART10 */

#if defined(USART11)
  /* Get the USART11 clock source ---------------------------------------------*/
  pPeriphClkInit->Usart11ClockSelection = __HAL_RCC_GET_USART11_SOURCE();
#endif /* USART11 */

#if defined(UART12)
  /* Get the UART12 clock source ---------------------------------------------*/
  pPeriphClkInit->Uart12ClockSelection = __HAL_RCC_GET_UART12_SOURCE();
#endif /* UART12 */

  /* Get the LPUART1 clock source --------------------------------------------*/
  pPeriphClkInit->Lpuart1ClockSelection = __HAL_RCC_GET_LPUART1_SOURCE();

  /* Get the I2C1 clock source -----------------------------------------------*/
  pPeriphClkInit->I2c1ClockSelection = __HAL_RCC_GET_I2C1_SOURCE();

  /* Get the I2C2 clock source -----------------------------------------------*/
  pPeriphClkInit->I2c2ClockSelection = __HAL_RCC_GET_I2C2_SOURCE();

#if defined(I2C3)
  /* Get the I2C3 clock source -----------------------------------------------*/
  pPeriphClkInit->I2c3ClockSelection = __HAL_RCC_GET_I2C3_SOURCE();
#endif /* I2C3 */

#if defined(I2C4)
  /* Get the I2C4 clock source -----------------------------------------------*/
  pPeriphClkInit->I2c4ClockSelection = __HAL_RCC_GET_I2C4_SOURCE();
#endif /* I2C4 */

  /* Get the I3C1 clock source -----------------------------------------------*/
  pPeriphClkInit->I3c1ClockSelection = __HAL_RCC_GET_I3C1_SOURCE();

#if defined(I3C2)
  /* Get the I3C2 clock source -----------------------------------------------*/
  pPeriphClkInit->I3c2ClockSelection = __HAL_RCC_GET_I3C2_SOURCE();
#endif /* I3C2 */

  /* Get the LPTIM1 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim1ClockSelection = __HAL_RCC_GET_LPTIM1_SOURCE();

  /* Get the LPTIM2 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim2ClockSelection = __HAL_RCC_GET_LPTIM2_SOURCE();

#if defined(LPTIM3)
  /* Get the LPTIM3 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim3ClockSelection = __HAL_RCC_GET_LPTIM3_SOURCE();
#endif /* LPTIM3 */

#if defined(LPTIM4)
  /* Get the LPTIM4 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim4ClockSelection = __HAL_RCC_GET_LPTIM4_SOURCE();
#endif /* LPTIM4 */

#if defined(LPTIM5)
  /* Get the LPTIM5 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim5ClockSelection = __HAL_RCC_GET_LPTIM5_SOURCE();
#endif /* LPTIM5 */

#if defined(LPTIM6)
  /* Get the LPTIM6 clock source ---------------------------------------------*/
  pPeriphClkInit->Lptim6ClockSelection = __HAL_RCC_GET_LPTIM6_SOURCE();
#endif /* LPTIM6 */

  /* Get the FDCAN clock source ---------------------------------------------*/
  pPeriphClkInit->FdcanClockSelection = __HAL_RCC_GET_FDCAN_SOURCE();

#if defined(SAI1)
  /* Get the SAI1 clock source -----------------------------------------------*/
  pPeriphClkInit->Sai1ClockSelection = __HAL_RCC_GET_SAI1_SOURCE();
#endif /* SAI1 */

#if defined(SAI2)
  /* Get the SAI2 clock source -----------------------------------------------*/
  pPeriphClkInit->Sai2ClockSelection = __HAL_RCC_GET_SAI2_SOURCE();
#endif /* SAI2 */

#if defined(SDMMC1)
  /* Get the SDMMC1 clock source ----------------------------------------------*/
  pPeriphClkInit->Sdmmc1ClockSelection = __HAL_RCC_GET_SDMMC1_SOURCE();
#endif /* SDMMC1 */

#if defined(SDMMC2)
  /* Get the SDMMC2 clock source ----------------------------------------------*/
  pPeriphClkInit->Sdmmc2ClockSelection = __HAL_RCC_GET_SDMMC2_SOURCE();
#endif /* SDMMC2 */

  /* Get the ADCDAC clock source ---------------------------------------------*/
  pPeriphClkInit->AdcDacClockSelection = __HAL_RCC_GET_ADCDAC_SOURCE();

  /* Get the DAC low-power clock source ---------------------------------------------*/
  pPeriphClkInit->DacLowPowerClockSelection = __HAL_RCC_GET_DAC_LP_SOURCE();

#if defined(OCTOSPI1)
  /* Get the OSPI clock source -----------------------------------------------*/
  pPeriphClkInit->OspiClockSelection = __HAL_RCC_GET_OSPI_SOURCE();
#endif /* OCTOSPI1 */

  /* Get the SPI1 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi1ClockSelection = __HAL_RCC_GET_SPI1_SOURCE();

  /* Get the SPI2 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi2ClockSelection = __HAL_RCC_GET_SPI2_SOURCE();

  /* Get the SPI3 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi3ClockSelection = __HAL_RCC_GET_SPI3_SOURCE();

#if defined(SPI4)
  /* Get the SPI4 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi4ClockSelection = __HAL_RCC_GET_SPI4_SOURCE();
#endif /* SPI4 */

#if defined(SPI5)
  /* Get the SPI5 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi5ClockSelection = __HAL_RCC_GET_SPI5_SOURCE();
#endif /* SPI5 */

#if defined(SPI6)
  /* Get the SPI6 clock source -----------------------------------------------*/
  pPeriphClkInit->Spi6ClockSelection = __HAL_RCC_GET_SPI6_SOURCE();
#endif /* SPI6 */

  /* Get the RTC clock source ------------------------------------------------*/
  pPeriphClkInit->RTCClockSelection = __HAL_RCC_GET_RTC_SOURCE();

  /* Get the RNG clock source ------------------------------------------------*/
  pPeriphClkInit->RngClockSelection = __HAL_RCC_GET_RNG_SOURCE();

  /* Get the CKPER clock source ------------------------------------------------*/
  pPeriphClkInit->CkperClockSelection = __HAL_RCC_GET_CLKP_SOURCE();

#if defined(CEC)
  /* Get the CEC clock source ------------------------------------------------*/
  pPeriphClkInit->CecClockSelection = __HAL_RCC_GET_CEC_SOURCE();
#endif /* CEC */

#if defined(USB_DRD_FS)
  /* Get the USB clock source ------------------------------------------------*/
  pPeriphClkInit->UsbClockSelection = __HAL_RCC_GET_USB_SOURCE();
#endif /* USB_DRD_FS */

  /* Get the TIM Prescaler configuration -------------------------------------*/
  if ((RCC->CFGR1 & RCC_CFGR1_TIMPRE) == 0U)
  {
    pPeriphClkInit->TimPresSelection = RCC_TIMPRES_DEACTIVATED;
  }
  else
  {
    pPeriphClkInit->TimPresSelection = RCC_TIMPRES_ACTIVATED;
  }

#if defined(PLAY1)
  /* Get the PLAY1 clock source ------------------------------------------------*/
  pPeriphClkInit->PLAY1ClockSelection = __HAL_RCC_GET_PLAY1_SOURCE();
  /* Get the PLAY1 clock divider ------------------------------------------------*/
  pPeriphClkInit->PLAY1ClockDivider = __HAL_RCC_GET_PLAY1_DIV();
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
  /* Get the USB_OTG_FS clock source ------------------------------------------------*/
  pPeriphClkInit->OtgfsClockSelection = __HAL_RCC_GET_OTGFS_SOURCE();
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
  /* Get the USB_OTG_HS clock source ------------------------------------------------*/
  pPeriphClkInit->OtghsClockSelection = __HAL_RCC_GET_OTGHS_SOURCE();
#endif /* USB_OTG_HS */

#if defined(OCTOSPI2)
  /* Get the OSPI2 clock source -----------------------------------------------*/
  pPeriphClkInit->Ospi2ClockSelection = __HAL_RCC_GET_OSPI2_SOURCE();
#endif /* OCTOSPI2 */

#if defined(LTDC)
  /* Get the LTDC clock source ------------------------------------------------*/
  pPeriphClkInit->LtdcClockSelection = __HAL_RCC_GET_LTDC_SOURCE();
#endif /* LTDC */

#if defined(ADF1)
  /* Get the ADF1 clock source ------------------------------------------------*/
  pPeriphClkInit->Adf1ClockSelection = __HAL_RCC_GET_ADF1_SOURCE();
#endif /* ADF1 */

#if defined(MDF1)
  /* Get the MDF1 clock source ------------------------------------------------*/
  pPeriphClkInit->Mdf1ClockSelection = __HAL_RCC_GET_MDF1_SOURCE();
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
  /* Get the ETH clock source ------------------------------------------------*/
  pPeriphClkInit->EthClockSelection = __HAL_RCC_GET_ETH_SOURCE();
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
  /* Get the ETHPTP clock source ------------------------------------------------*/
  pPeriphClkInit->EthptpClockSelection = __HAL_RCC_GET_ETHPTP_SOURCE();
  /* Get the ETHPTP clock source ------------------------------------------------*/
  pPeriphClkInit->EthptpClockDivider = __HAL_RCC_GET_ETHPTP_DIV();
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
  /* Get the ETHREF clock source ------------------------------------------------*/
  pPeriphClkInit->EthrefClockSelection = __HAL_RCC_GET_ETHREF_SOURCE();
#endif /* RCC_CCIPR5_ETHREFCLKSEL */
}

/**
  * @brief  Returns the PLL1 clock frequencies : PLL1_P_Frequency, PLL1_R_Frequency and PLL1_Q_Frequency
  * @note   The PLL1 clock frequencies computed by this function may not be the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note   The function returns values based on HSE_VALUE, HSI_VALUE or CSI Value multiplied/divided by
            the PLL factors.
  * @note   This function can be used by the user application to compute the
  *         baud-rate for the communication peripherals or configure other parameters.
  *
  * @note   Each time PLL1CLK changes, this function must be called to update the
  *         right PLL1CLK value. Otherwise, any configuration based on this function will be incorrect.
  * @param  pPLL1_Clocks pointer to PLL1_ClocksTypeDef structure.
  * @retval None
  */
void HAL_RCCEx_GetPLL1ClockFreq(PLL1_ClocksTypeDef *pPLL1_Clocks)
{
  uint32_t pll1source;
  uint32_t pll1m;
  uint32_t pll1n;
  uint32_t pll1fracen;
  uint32_t hsivalue;
  float_t fracn1;
  float_t pll1vco;

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLL1M) * PLL1N
  PLL1xCLK = PLL1_VCO / PLL1x
  */

  pll1n = (RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1N);
  pll1source = (RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1SRC);
  pll1m = ((RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1M) >> RCC_PLL1CFGR_PLL1M_Pos);
  pll1fracen = ((RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1FRACEN) >> RCC_PLL1CFGR_PLL1FRACEN_Pos);
  fracn1 = (float_t)(uint32_t)(pll1fracen * ((RCC->PLL1FRACR & RCC_PLL1FRACR_PLL1FRACN) >> \
                                             RCC_PLL1FRACR_PLL1FRACN_Pos));

  if (pll1m != 0U)
  {
    switch (pll1source)
    {

      case RCC_PLL1_SOURCE_HSI:  /* HSI used as PLL1 clock source */
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll1vco = ((float_t)hsivalue / (float_t)pll1m) * ((float_t)(uint32_t)pll1n + (fracn1 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;

      case RCC_PLL1_SOURCE_CSI:  /* CSI used as PLL1 clock source */
        pll1vco = ((float_t)CSI_VALUE / (float_t)pll1m) * ((float_t)(uint32_t)pll1n + (fracn1 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      case RCC_PLL1_SOURCE_HSE:  /* HSE used as PLL1 clock source */
        pll1vco = ((float_t)HSE_VALUE / (float_t)pll1m) * ((float_t)(uint32_t)pll1n + (fracn1 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      default:
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll1vco = ((float_t)hsivalue / (float_t)pll1m) * ((float_t)(uint32_t)pll1n + (fracn1 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL1RDY))
    {
      if (__HAL_RCC_GET_PLL1_CLKOUT_CONFIG(RCC_PLL1_DIVP) != 0U)
      {
        pPLL1_Clocks->PLL1_P_Frequency = \
                                         (uint32_t)(float_t)(pll1vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL1DIVR & \
                                                                                   RCC_PLL1DIVR_PLL1P) >> \
                                                                                  RCC_PLL1DIVR_PLL1P_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL1_Clocks->PLL1_P_Frequency = 0U;
      }
    }
    else
    {
      pPLL1_Clocks->PLL1_P_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL1RDY))
    {
      if (__HAL_RCC_GET_PLL1_CLKOUT_CONFIG(RCC_PLL1_DIVQ) != 0U)
      {
        pPLL1_Clocks->PLL1_Q_Frequency = \
                                         (uint32_t)(float_t)(pll1vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL1DIVR & \
                                                                                   RCC_PLL1DIVR_PLL1Q) >> \
                                                                                  RCC_PLL1DIVR_PLL1Q_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL1_Clocks->PLL1_Q_Frequency = 0U;
      }
    }
    else
    {
      pPLL1_Clocks->PLL1_Q_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL1RDY))
    {
      if (__HAL_RCC_GET_PLL1_CLKOUT_CONFIG(RCC_PLL1_DIVR) != 0U)
      {
        pPLL1_Clocks->PLL1_R_Frequency = \
                                         (uint32_t)(float_t)(pll1vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL1DIVR & \
                                                                                   RCC_PLL1DIVR_PLL1R) >> \
                                                                                  RCC_PLL1DIVR_PLL1R_Pos) + \
                                                              (float_t)1)) ;
      }
      else
      {
        pPLL1_Clocks->PLL1_R_Frequency = 0U;
      }
    }
    else
    {
      pPLL1_Clocks->PLL1_R_Frequency = 0U;
    }

  }
  else
  {
    pPLL1_Clocks->PLL1_P_Frequency = 0U;
    pPLL1_Clocks->PLL1_Q_Frequency = 0U;
    pPLL1_Clocks->PLL1_R_Frequency = 0U;
  }

}

/**
  * @brief  Returns the PLL2 clock frequencies: PLL2_P_Frequency, PLL2_R_Frequency and PLL2_Q_Frequency
  * @note   The PLL2 clock frequencies computed by this function may not be the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note   The function returns values based on HSE_VALUE, HSI_VALUE or CSI Value multiplied/divided by
            the PLL factors.
  * @note   This function can be used by the user application to compute the
  *         baud-rate for the communication peripherals or configure other parameters.
  *
  * @note   Each time PLL2CLK changes, this function must be called to update the
  *         right PLL2CLK value. Otherwise, any configuration based on this function will be incorrect.
  * @param  pPLL2_Clocks pointer to PLL2_ClocksTypeDef structure.
  * @retval None
  */
void HAL_RCCEx_GetPLL2ClockFreq(PLL2_ClocksTypeDef *pPLL2_Clocks)
{
  uint32_t  pll2source;
  uint32_t  pll2m;
  uint32_t  pll2n;
  uint32_t  pll2fracen;
  uint32_t  hsivalue;
  float_t fracn2;
  float_t pll2vco;

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLL2M) * PLL2N
  PLL2xCLK = PLL2_VCO / PLL2x
  */
  pll2n = (RCC->PLL2DIVR & RCC_PLL2DIVR_PLL2N);
  pll2source = (RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2SRC);
  pll2m = ((RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2M) >> RCC_PLL2CFGR_PLL2M_Pos);
  pll2fracen = ((RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2FRACEN) >> RCC_PLL2CFGR_PLL2FRACEN_Pos);
  fracn2 = (float_t)(uint32_t)(pll2fracen * ((RCC->PLL2FRACR & RCC_PLL2FRACR_PLL2FRACN) >> \
                                             RCC_PLL2FRACR_PLL2FRACN_Pos));

  if (pll2m != 0U)
  {
    switch (pll2source)
    {
      case RCC_PLL2_SOURCE_HSI:  /* HSI used as PLL clock source */
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll2vco = ((float_t)hsivalue / (float_t)pll2m) * ((float_t)(uint32_t)pll2n + (fracn2 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;

      case RCC_PLL2_SOURCE_CSI:  /* CSI used as PLL clock source */
        pll2vco = ((float_t)CSI_VALUE / (float_t)pll2m) * ((float_t)(uint32_t)pll2n + (fracn2 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      case RCC_PLL2_SOURCE_HSE:  /* HSE used as PLL clock source */
        pll2vco = ((float_t)HSE_VALUE / (float_t)pll2m) * ((float_t)(uint32_t)pll2n + (fracn2 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      default:
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll2vco = ((float_t)hsivalue / (float_t)pll2m) * ((float_t)(uint32_t)pll2n + (fracn2 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY))
    {
      if (__HAL_RCC_GET_PLL2_CLKOUT_CONFIG(RCC_PLL2_DIVP) != 0U)
      {
        pPLL2_Clocks->PLL2_P_Frequency = \
                                         (uint32_t)(float_t)(pll2vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL2DIVR & \
                                                                                   RCC_PLL2DIVR_PLL2P) >> \
                                                                                  RCC_PLL2DIVR_PLL2P_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL2_Clocks->PLL2_P_Frequency = 0U;
      }
    }
    else
    {
      pPLL2_Clocks->PLL2_P_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY))
    {
      if (__HAL_RCC_GET_PLL2_CLKOUT_CONFIG(RCC_PLL2_DIVQ) != 0U)
      {
        pPLL2_Clocks->PLL2_Q_Frequency = \
                                         (uint32_t)(float_t)(pll2vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL2DIVR & \
                                                                                   RCC_PLL2DIVR_PLL2Q) >> \
                                                                                  RCC_PLL2DIVR_PLL2Q_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL2_Clocks->PLL2_Q_Frequency = 0U;
      }
    }
    else
    {
      pPLL2_Clocks->PLL2_Q_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY))
    {
      if (__HAL_RCC_GET_PLL2_CLKOUT_CONFIG(RCC_PLL2_DIVR) != 0U)
      {
        pPLL2_Clocks->PLL2_R_Frequency = \
                                         (uint32_t)(float_t)(pll2vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL2DIVR & \
                                                                                   RCC_PLL2DIVR_PLL2R) >> \
                                                                                  RCC_PLL2DIVR_PLL2R_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL2_Clocks->PLL2_R_Frequency = 0U;
      }
    }
    else
    {
      pPLL2_Clocks->PLL2_R_Frequency = 0U;
    }
  }
  else
  {
    pPLL2_Clocks->PLL2_P_Frequency = 0U;
    pPLL2_Clocks->PLL2_Q_Frequency = 0U;
    pPLL2_Clocks->PLL2_R_Frequency = 0U;
  }
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Returns the PLL3 clock frequencies: PLL3_P_Frequency, PLL3_R_Frequency and PLL3_Q_Frequency
  * @note   The PLL3 clock frequencies computed by this function may not be the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note   The function returns values based on HSE_VALUE, HSI_VALUE or CSI Value multiplied/divided by
            the PLL factors.
  * @note   This function can be used by the user application to compute the
  *         baud-rate for the communication peripherals or configure other parameters.
  *
  * @note   Each time PLL3CLK changes, this function must be called to update the
  *         right PLL3CLK value. Otherwise, any configuration based on this function will be incorrect.
  * @param  pPLL3_Clocks pointer to PLL3_ClocksTypeDef structure.
  * @retval None
  */
void HAL_RCCEx_GetPLL3ClockFreq(PLL3_ClocksTypeDef *pPLL3_Clocks)
{
  uint32_t  pll3source;
  uint32_t  pll3m;
  uint32_t  pll3n;
  uint32_t  pll3fracen;
  uint32_t  hsivalue;
  float_t fracn3;
  float_t pll3vco;

  /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLL3M) * PLL3N
  PLL3xCLK = PLL3_VCO / PLL3x
  */
  pll3n = (RCC->PLL3DIVR & RCC_PLL3DIVR_PLL3N);
  pll3source = (RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3SRC);
  pll3m = ((RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3M) >> RCC_PLL3CFGR_PLL3M_Pos);
  pll3fracen = ((RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3FRACEN) >> RCC_PLL3CFGR_PLL3FRACEN_Pos);
  fracn3 = (float_t)(uint32_t)(pll3fracen * ((RCC->PLL3FRACR & RCC_PLL3FRACR_PLL3FRACN) >> \
                                             RCC_PLL3FRACR_PLL3FRACN_Pos));

  if (pll3m != 0U)
  {
    switch (pll3source)
    {
      case RCC_PLL3_SOURCE_HSI:  /* HSI used as PLL clock source */
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll3vco = ((float_t)hsivalue / (float_t)pll3m) * ((float_t)(uint32_t)pll3n + (fracn3 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;

      case RCC_PLL3_SOURCE_CSI:  /* CSI used as PLL clock source */
        pll3vco = ((float_t)CSI_VALUE / (float_t)pll3m) * ((float_t)(uint32_t)pll3n + (fracn3 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      case RCC_PLL3_SOURCE_HSE:  /* HSE used as PLL clock source */
        pll3vco = ((float_t)HSE_VALUE / (float_t)pll3m) * ((float_t)(uint32_t)pll3n + (fracn3 / (float_t)0x2000) + \
                                                           (float_t)1);
        break;

      default:
        hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        pll3vco = ((float_t)hsivalue / (float_t)pll3m) * ((float_t)(uint32_t)pll3n + (fracn3 / (float_t)0x2000) + \
                                                          (float_t)1);
        break;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY))
    {
      if (__HAL_RCC_GET_PLL3_CLKOUT_CONFIG(RCC_PLL3_DIVP) != 0U)
      {
        pPLL3_Clocks->PLL3_P_Frequency = \
                                         (uint32_t)(float_t)(pll3vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL3DIVR & \
                                                                                   RCC_PLL3DIVR_PLL3P) >> \
                                                                                  RCC_PLL3DIVR_PLL3P_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL3_Clocks->PLL3_P_Frequency = 0U;
      }
    }
    else
    {
      pPLL3_Clocks->PLL3_P_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY))
    {
      if (__HAL_RCC_GET_PLL3_CLKOUT_CONFIG(RCC_PLL3_DIVQ) != 0U)
      {
        pPLL3_Clocks->PLL3_Q_Frequency = \
                                         (uint32_t)(float_t)(pll3vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL3DIVR & \
                                                                                   RCC_PLL3DIVR_PLL3Q) >> \
                                                                                  RCC_PLL3DIVR_PLL3Q_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL3_Clocks->PLL3_Q_Frequency = 0U;
      }
    }
    else
    {
      pPLL3_Clocks->PLL3_Q_Frequency = 0U;
    }

    if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY))
    {
      if (__HAL_RCC_GET_PLL3_CLKOUT_CONFIG(RCC_PLL3_DIVR) != 0U)
      {
        pPLL3_Clocks->PLL3_R_Frequency = \
                                         (uint32_t)(float_t)(pll3vco / \
                                                             ((float_t)(uint32_t)((RCC->PLL3DIVR & \
                                                                                   RCC_PLL3DIVR_PLL3R) >> \
                                                                                  RCC_PLL3DIVR_PLL3R_Pos) + \
                                                              (float_t)1));
      }
      else
      {
        pPLL3_Clocks->PLL3_R_Frequency = 0U;
      }
    }
    else
    {
      pPLL3_Clocks->PLL3_R_Frequency = 0U;
    }
  }
  else
  {
    pPLL3_Clocks->PLL3_P_Frequency = 0U;
    pPLL3_Clocks->PLL3_Q_Frequency = 0U;
    pPLL3_Clocks->PLL3_R_Frequency = 0U;
  }
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Return the peripheral clock frequency for peripherals
  * @note   Return 0 if peripheral clock identifier not managed by this API
  * @param  PeriphClk  Peripheral clock identifier
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PERIPHCLK_USART1  USART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_USART2  USART2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_USART3  USART3 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_UART4   UART4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART5   UART5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART6  USART6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART7   UART7 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART8   UART8 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART9   UART9 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART10 USART10 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USART11 USART11 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_UART12  UART12 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPUART1 LPUART1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C1    I2C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C2    I2C2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I2C3    I2C3 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_I2C4    I2C4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_I3C1    I3C1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_I3C2    I3C2 peripheral clock (***)
  *            @arg @ref RCC_PERIPHCLK_LPTIM1  LPTIM1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_LPTIM2  LPTIM2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SAI1    SAI1 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SAI2    SAI2 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_ADCDAC  ADCDAC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_ADC     ADC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SDMMC1  SDMMC1 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SDMMC2  SDMMC2 peripheral clock (**)
  *            @arg @ref RCC_PERIPHCLK_CKPER   CKPER peripheral clock
  *            @arg @ref RCC_PERIPHCLK_RTC     RTC peripheral clock
  *            @arg @ref RCC_PERIPHCLK_RNG     RNG peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI1    SPI1 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI2    SPI2 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI3    SPI3 peripheral clock
  *            @arg @ref RCC_PERIPHCLK_SPI4    SPI4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SPI5    SPI5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_SPI6    SPI6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_OSPI    OCTOSPI peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_FDCAN   FDCAN peripheral clock
  *            @arg @ref RCC_PERIPHCLK_CEC     CEC peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_USB     USB peripheral clock (*****)
  *            @arg @ref RCC_PERIPHCLK_LPTIM3  LPTIM3 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM4  LPTIM4 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM5  LPTIM5 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_LPTIM6  LPTIM6 peripheral clock (*)
  *            @arg @ref RCC_PERIPHCLK_DAC_LP  DAC low-power peripheral clock
  *            @arg @ref RCC_PERIPHCLK_PLAY1   PLAY1 peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_OTGFS   USB_OTG_FS peripheral clock (****)
  *            @arg @ref RCC_PERIPHCLK_OTGHS   USB_OTG_HS peripheral clock (****)
  *
  * @retval Frequency in Hz
  *
  *  (*)     : For stm32h56xxx and stm32h57xxx family lines only.
  *  (**)    : For stm32h563xx and stm32h57xxx family lines only.
  *  (***)   : For stm32h503xx family line only.
  *  (****)  : For stm32h5exxx and stm32h5fxxx family lines only.
  *  (*****) : Not available for stm32h5exxx and stm32h5fxxx family lines.
  */
uint32_t HAL_RCCEx_GetPeriphCLKFreq(uint64_t PeriphClk)
{
  PLL1_ClocksTypeDef pll1_clocks;
  PLL2_ClocksTypeDef pll2_clocks;
#if defined(RCC_CR_PLL3ON)
  PLL3_ClocksTypeDef pll3_clocks;
#endif /* RCC_CR_PLL3ON */

  uint32_t frequency = 0u;
  uint32_t ckpclocksource = 0u;
  uint32_t srcclk = 0u;
#if defined(PLAY1)
  uint32_t play1_divider = 0u;
#endif /* PLAY1 */
#if defined(RCC_CCIPR4_ETHPTPDIV)
  uint32_t ethptp_divider = 0u;
#endif /* RCC_CCIPR4_ETHPTPDIV */

  /* Check the parameters */
  assert_param(IS_RCC_PERIPHCLOCK(PeriphClk));

  if (PeriphClk == RCC_PERIPHCLK_RTC)
  {
    /* Get the current RTC source */
    srcclk = __HAL_RCC_GET_RTC_SOURCE();

    /* Check if LSE is ready and if RTC clock selection is LSE */
    if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_RTCCLKSOURCE_LSE))
    {
      frequency = LSE_VALUE;
    }
    /* Check if LSI is ready and if RTC clock selection is LSI */
    else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY)) && (srcclk == RCC_RTCCLKSOURCE_LSI))
    {
      frequency = LSI_VALUE;
    }
    /* Check if HSE is ready and if RTC clock selection is HSE_DIVx*/
    else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_RTCCLKSOURCE_HSE_DIVx))
    {
      if (__HAL_RCC_GET_RTC_HSE_PRESCALER() >= RCC_RTC_HSE_DIV2)
      {
        frequency = (HSE_VALUE / ((uint32_t)(__HAL_RCC_GET_RTC_HSE_PRESCALER() >> RCC_CFGR1_RTCPRE_Pos)));
      }
      else
      {
        frequency = 0U;
      }

    }
    /* Clock not enabled for RTC*/
    else
    {
      frequency = 0U;
    }
  }
  else
  {
    /* Other external peripheral clock source than RTC */
    switch (PeriphClk)
    {
#if defined (SAI1)
      case RCC_PERIPHCLK_SAI1:

        srcclk = __HAL_RCC_GET_SAI1_SOURCE();

        switch (srcclk)
        {
          case RCC_SAI1CLKSOURCE_PLL1Q: /* PLL1Q is the clock source for SAI1 */
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_SAI1CLKSOURCE_PLL2P: /* PLL2P is the clock source for SAI1 */
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_SAI1CLKSOURCE_PLL3P: /* PLLI3P is the clock source for SAI1 */
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
          case RCC_SAI1CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_SAI1CLKSOURCE_CLKP: /* CLKP is the clock source for SAI1 */
          {

            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /*SAI1*/

#if defined(SAI2)
      case RCC_PERIPHCLK_SAI2:

        srcclk = __HAL_RCC_GET_SAI2_SOURCE();

        switch (srcclk)
        {
          case RCC_SAI2CLKSOURCE_PLL1Q: /* PLL1Q is the clock source for SAI2 */
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_SAI2CLKSOURCE_PLL2P: /* PLL2P is the clock source for SAI2 */
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_SAI2CLKSOURCE_PLL3P: /* PLLI3P is the clock source for SAI2 */
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
          case RCC_SAI2CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_SAI2CLKSOURCE_CLKP: /* CLKP is the clock source for SAI2 */
          {

            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* SAI2 */

#if defined(SDMMC1)
      case RCC_PERIPHCLK_SDMMC1:
        srcclk = __HAL_RCC_GET_SDMMC1_SOURCE();
        if (srcclk == RCC_SDMMC1CLKSOURCE_PLL1Q)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
        }
        else if (srcclk == RCC_SDMMC1CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
        else
        {
          frequency = 0U;
        }
        break;
#endif /* SDMMC1 */

#if defined(SDMMC2)
      case RCC_PERIPHCLK_SDMMC2:
        srcclk = __HAL_RCC_GET_SDMMC2_SOURCE();
        if (srcclk == RCC_SDMMC2CLKSOURCE_PLL1Q)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
        }
        else if (srcclk == RCC_SDMMC2CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
        else
        {
          frequency = 0U;
        }
        break;
#endif /* SDMMC2 */

      case RCC_PERIPHCLK_USART1:
        /* Get the current USART1 source */
        srcclk = __HAL_RCC_GET_USART1_SOURCE();

        if (srcclk == RCC_USART1CLKSOURCE_PCLK2)
        {
          frequency = HAL_RCC_GetPCLK2Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART1CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_USART1CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USART1CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_USART1CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART1CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART1CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART1CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART1 */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_USART2:
        /* Get the current USART2 source */
        srcclk = __HAL_RCC_GET_USART2_SOURCE();

        if (srcclk == RCC_USART2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART2CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_USART2CLKSOURCE_PLL3Q)
        else if (srcclk == RCC_USART2CLKSOURCE_PLL3Q)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_USART2CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART2CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART2CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART2CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART2 */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_USART3:
        /* Get the current USART3 source */
        srcclk = __HAL_RCC_GET_USART3_SOURCE();

        if (srcclk == RCC_USART3CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART3CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_USART3CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USART3CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_USART3CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART3CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART3CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART3CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART3 */
        else
        {
          frequency = 0U;
        }
        break;

#if defined(UART4)
      case RCC_PERIPHCLK_UART4:
        /* Get the current UART4 source */
        srcclk = __HAL_RCC_GET_UART4_SOURCE();

        if (srcclk == RCC_UART4CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART4CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_UART4CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART4CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_UART4CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART4CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART4CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART4CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART4 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART4 */

#if defined(UART5)
      case RCC_PERIPHCLK_UART5:
        /* Get the current UART5 source */
        srcclk = __HAL_RCC_GET_UART5_SOURCE();

        if (srcclk == RCC_UART5CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART5CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_UART5CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART5CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_UART5CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART5CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART5CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART5CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART5 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART5 */

#if defined(USART6)
      case RCC_PERIPHCLK_USART6:
        /* Get the current USART6 source */
        srcclk = __HAL_RCC_GET_USART6_SOURCE();

        if (srcclk == RCC_USART6CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART6CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_USART6CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USART6CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_USART6CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART6CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART6CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART6CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART6 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* USART6 */

#if defined(UART7)
      case RCC_PERIPHCLK_UART7:
        /* Get the current UART7 source */
        srcclk = __HAL_RCC_GET_UART7_SOURCE();

        if (srcclk == RCC_UART7CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART7CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_UART7CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART7CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_UART7CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART7CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART7CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART7CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART7 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART7 */

#if defined(UART8)
      case RCC_PERIPHCLK_UART8:
        /* Get the current UART8 source */
        srcclk = __HAL_RCC_GET_UART8_SOURCE();

        if (srcclk == RCC_UART8CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART8CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_UART8CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART8CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_UART8CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART8CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART8CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART8CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART8 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART8 */

#if defined(UART9)
      case RCC_PERIPHCLK_UART9:
        /* Get the current UART9 source */
        srcclk = __HAL_RCC_GET_UART9_SOURCE();

        if (srcclk == RCC_UART9CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART9CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART9CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART9CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART9CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART9CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART9 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART9 */

#if defined(USART10)
      case RCC_PERIPHCLK_USART10:
        /* Get the current USART10 source */
        srcclk = __HAL_RCC_GET_USART10_SOURCE();

        if (srcclk == RCC_USART10CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART10CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USART10CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART10CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART10CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART10CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART10 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* USART10 */

#if defined(USART11)
      case RCC_PERIPHCLK_USART11:
        /* Get the current USART11 source */
        srcclk = __HAL_RCC_GET_USART11_SOURCE();

        if (srcclk == RCC_USART11CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USART11CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USART11CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_USART11CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_USART11CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_USART11CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for USART11 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* USART11 */

#if defined(UART12)
      case RCC_PERIPHCLK_UART12:
        /* Get the current UART12 source */
        srcclk = __HAL_RCC_GET_UART12_SOURCE();

        if (srcclk == RCC_UART12CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_UART12CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_UART12CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_UART12CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_UART12CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_UART12CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for UART12 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* UART12 */

      case RCC_PERIPHCLK_LPUART1:
        /* Get the current LPUART1 source */
        srcclk = __HAL_RCC_GET_LPUART1_SOURCE();

        if (srcclk == RCC_LPUART1CLKSOURCE_PCLK3)
        {
          frequency = HAL_RCC_GetPCLK3Freq();
        }
        else if (srcclk == RCC_LPUART1CLKSOURCE_PLL2Q)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_LPUART1CLKSOURCE_PLL3Q)
        else if (srcclk == RCC_LPUART1CLKSOURCE_PLL3Q)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_LPUART1CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_LPUART1CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_LPUART1CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_LPUART1CLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        /* Clock not enabled for LPUART1 */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_ADCDAC:
        /* Get the current ADCDAC source */
        srcclk = __HAL_RCC_GET_ADCDAC_SOURCE();

        if (srcclk == RCC_ADCDACCLKSOURCE_HCLK)
        {
          frequency = HAL_RCC_GetHCLKFreq();
        }
        else if (srcclk == RCC_ADCDACCLKSOURCE_SYSCLK)
        {
          frequency = HAL_RCC_GetSysClockFreq();
        }
        else if (srcclk == RCC_ADCDACCLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_ADCDACCLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_ADCDACCLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_ADCDACCLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        /* Clock not enabled for ADCDAC */
        else
        {
          frequency = 0U;
        }
        break;


      case RCC_PERIPHCLK_DAC_LP:
        /* Get the current DAC low-power source */
        srcclk = __HAL_RCC_GET_DAC_LP_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_DACLPCLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY)) && (srcclk == RCC_DACLPCLKSOURCE_LSI))
        {
          frequency = LSI_VALUE;
        }

        /* Clock not enabled for DAC */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_I2C1:
        /* Get the current I2C1 source */
        srcclk = __HAL_RCC_GET_I2C1_SOURCE();

        if (srcclk == RCC_I2C1CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
#if defined(RCC_I2C1CLKSOURCE_PLL3R)
        else if (srcclk ==  RCC_I2C1CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
#else
        else if (srcclk ==  RCC_I2C1CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
#endif /* RCC_I2C1CLKSOURCE_PLL3R */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C1CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk ==  RCC_I2C1CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        /* Clock not enabled for I2C1 */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_I2C2:
        /* Get the current I2C2 source */
        srcclk = __HAL_RCC_GET_I2C2_SOURCE();

        if (srcclk == RCC_I2C2CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
#if defined(RCC_I2C2CLKSOURCE_PLL3R)
        else if (srcclk ==  RCC_I2C2CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
#else
        else if (srcclk ==  RCC_I2C2CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
#endif /* RCC_I2C2CLKSOURCE_PLL3R */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C2CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk ==  RCC_I2C2CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        /* Clock not enabled for I2C2 */
        else
        {
          frequency = 0U;
        }
        break;

#if defined(I2C3)
      case RCC_PERIPHCLK_I2C3:
        /* Get the current I2C3 source */
        srcclk = __HAL_RCC_GET_I2C3_SOURCE();

        if (srcclk == RCC_I2C3CLKSOURCE_PCLK3)
        {
          frequency = HAL_RCC_GetPCLK3Freq();
        }
#if defined(RCC_I2C3CLKSOURCE_PLL3R)
        else if (srcclk ==  RCC_I2C3CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
#else
        else if (srcclk ==  RCC_I2C3CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
#endif /* RCC_I2C3CLKSOURCE_PLL3R */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C3CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk ==  RCC_I2C3CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        /* Clock not enabled for I2C3 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* I2C3 */

#if defined(I2C4)
      case RCC_PERIPHCLK_I2C4:
        /* Get the current I2C4 source */
        srcclk = __HAL_RCC_GET_I2C4_SOURCE();

        if (srcclk == RCC_I2C4CLKSOURCE_PCLK3)
        {
          frequency = HAL_RCC_GetPCLK3Freq();
        }
        else if (srcclk ==  RCC_I2C4CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I2C4CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk ==  RCC_I2C4CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        /* Clock not enabled for I2C4 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* I2C4 */

      case RCC_PERIPHCLK_I3C1:
        /* Get the current I3C1 source */
        srcclk = __HAL_RCC_GET_I3C1_SOURCE();

        if (srcclk == RCC_I3C1CLKSOURCE_PCLK1)
        {
          frequency = HAL_RCC_GetPCLK1Freq();
        }
#if defined(RCC_I3C1CLKSOURCE_PLL3R)
        else if (srcclk ==  RCC_I3C1CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
#else
        else if (srcclk ==  RCC_I3C1CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
#endif /* RCC_I3C1CLKSOURCE_PLL3R */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I3C1CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        /* Clock not enabled for I3C1 */
        else
        {
          frequency = 0U;
        }
        break;

#if defined(I3C2)
      case RCC_PERIPHCLK_I3C2:
        /* Get the current I3C2 source */
        srcclk = __HAL_RCC_GET_I3C2_SOURCE();
        if (srcclk == RCC_I3C2CLKSOURCE_PCLK3)
        {
          frequency = HAL_RCC_GetPCLK3Freq();
        }
#if defined(RCC_I3C1CLKSOURCE_PLL3R)
        else if (srcclk ==  RCC_I3C2CLKSOURCE_PLL3R)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_R_Frequency;
        }
#else
        else if (srcclk ==  RCC_I3C2CLKSOURCE_PLL2R)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_R_Frequency;
        }
#endif /* RCC_I3C1CLKSOURCE_PLL3R */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_I3C2CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk ==  RCC_I3C2CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
#endif /* defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) */
        /* Clock not enabled for I3C2 */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* I3C2*/

      case RCC_PERIPHCLK_LPTIM1:
        /* Get the current LPTIM1 source */
        srcclk = __HAL_RCC_GET_LPTIM1_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM1CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq();
            break;
          }
          case RCC_LPTIM1CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
#if defined(RCC_LPTIM1CLKSOURCE_PLL3R)
          case RCC_LPTIM1CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
#endif /* RCC_LPTIM1CLKSOURCE_PLL3R */
          case RCC_LPTIM1CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM1CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM1CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM1 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;

      case RCC_PERIPHCLK_LPTIM2:
        /* Get the current LPTIM2 source */
        srcclk = __HAL_RCC_GET_LPTIM2_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM2CLKSOURCE_PCLK1:
          {
            frequency = HAL_RCC_GetPCLK1Freq();
            break;
          }
          case RCC_LPTIM2CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
#if defined(RCC_LPTIM2CLKSOURCE_PLL3R)
          case RCC_LPTIM2CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
#endif /* RCC_LPTIM2CLKSOURCE_PLL3R */
          case RCC_LPTIM2CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM2CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM2CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM2 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;

#if defined(LPTIM3)
      case RCC_PERIPHCLK_LPTIM3:
        /* Get the current LPTIM3 source */
        srcclk = __HAL_RCC_GET_LPTIM3_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM3CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq();
            break;
          }
          case RCC_LPTIM3CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_LPTIM3CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
          case RCC_LPTIM3CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM3CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM3CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM3 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* LPTIM3 */

#if defined(LPTIM4)
      case RCC_PERIPHCLK_LPTIM4:
        /* Get the current LPTIM4 source */
        srcclk = __HAL_RCC_GET_LPTIM4_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM4CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq();
            break;
          }
          case RCC_LPTIM4CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_LPTIM4CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
          case RCC_LPTIM4CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM4CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM4CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM4 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* LPTIM4 */

#if defined(LPTIM5)
      case RCC_PERIPHCLK_LPTIM5:
        /* Get the current LPTIM5 source */
        srcclk = __HAL_RCC_GET_LPTIM5_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM5CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq();
            break;
          }
          case RCC_LPTIM5CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_LPTIM5CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
          case RCC_LPTIM5CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM5CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM5CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM5 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* LPTIM5 */

#if defined(LPTIM6)
      case RCC_PERIPHCLK_LPTIM6:
        /* Get the current LPTIM6 source */
        srcclk = __HAL_RCC_GET_LPTIM6_SOURCE();

        switch (srcclk)
        {
          case RCC_LPTIM6CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq();
            break;
          }
          case RCC_LPTIM6CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_LPTIM6CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
          case RCC_LPTIM6CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM6CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE;
            }
            else
            {
              frequency = 0U;
            }
            break;
          }
          case RCC_LPTIM6CLKSOURCE_CLKP: /* CLKP is the clock source for LPTIM6 */
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default :
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* LPTIM6 */

      case RCC_PERIPHCLK_FDCAN:
        /* Get the current FDCAN kernel source */
        srcclk = __HAL_RCC_GET_FDCAN_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_FDCANCLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        else if (srcclk == RCC_FDCANCLKSOURCE_PLL1Q)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
        }
        else if (srcclk == RCC_FDCANCLKSOURCE_PLL2Q)
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        /* Clock not enabled for FDCAN */
        else
        {
          frequency = 0U;
        }
        break;

      case RCC_PERIPHCLK_SPI1:
        /* Get the current SPI1 kernel source */
        srcclk = __HAL_RCC_GET_SPI1_SOURCE();
        switch (srcclk)
        {
          case RCC_SPI1CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_SPI1CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
#if defined(RCC_SPI1CLKSOURCE_PLL3P)
          case RCC_SPI1CLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
#endif /* RCC_SPI1CLKSOURCE_PLL3P */
          case RCC_SPI1CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_SPI1CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;

      case RCC_PERIPHCLK_SPI2:
        /* Get the current SPI2 kernel source */
        srcclk = __HAL_RCC_GET_SPI2_SOURCE();
        switch (srcclk)
        {
          case RCC_SPI2CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_SPI2CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
#if defined(RCC_SPI2CLKSOURCE_PLL3P)
          case RCC_SPI2CLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
#endif /* RCC_SPI2CLKSOURCE_PLL3P */
          case RCC_SPI2CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_SPI2CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;

      case RCC_PERIPHCLK_SPI3:
        /* Get the current SPI3 kernel source */
        srcclk = __HAL_RCC_GET_SPI3_SOURCE();
        switch (srcclk)
        {
          case RCC_SPI3CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_SPI3CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
#if defined(RCC_SPI3CLKSOURCE_PLL3P)
          case RCC_SPI3CLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
#endif /* RCC_SPI3CLKSOURCE_PLL3P */
          case RCC_SPI3CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_SPI3CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;

#if defined(SPI4)
      case RCC_PERIPHCLK_SPI4:
        /* Get the current SPI4 kernel source */
        srcclk = __HAL_RCC_GET_SPI4_SOURCE();

        if (srcclk == RCC_SPI4CLKSOURCE_PCLK2)
        {
          frequency = HAL_RCC_GetPCLK2Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_SPI4CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#if defined(RCC_SPI4CLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_SPI4CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#endif /* RCC_SPI4CLKSOURCE_PLL3Q */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_SPI4CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_SPI4CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_SPI4CLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        /* Clock not enabled for SPI4 */
        else
        {
          frequency = 0U;
        }

        break;
#endif /* SPI4 */

#if defined(SPI5)
      case RCC_PERIPHCLK_SPI5:
        /* Get the current SPI5 kernel source */
        srcclk = __HAL_RCC_GET_SPI5_SOURCE();

        if (srcclk == RCC_SPI5CLKSOURCE_PCLK3)
        {
          frequency = HAL_RCC_GetPCLK3Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_SPI5CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_SPI5CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_SPI5CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_SPI5CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_SPI5CLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        /* Clock not enabled for SPI5 */
        else
        {
          frequency = 0U;
        }

        break;
#endif /* SPI5 */

#if defined(SPI6)
      case RCC_PERIPHCLK_SPI6:
        /* Get the current SPI6 kernel source */
        srcclk = __HAL_RCC_GET_SPI6_SOURCE();

        if (srcclk == RCC_SPI6CLKSOURCE_PCLK2)
        {
          frequency = HAL_RCC_GetPCLK2Freq();
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_SPI6CLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_SPI6CLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY)) && (srcclk == RCC_SPI6CLKSOURCE_HSI))
        {
          frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_SPI6CLKSOURCE_CSI))
        {
          frequency = CSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY)) && (srcclk == RCC_SPI6CLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        /* Clock not enabled for SPI6 */
        else
        {
          frequency = 0U;
        }

        break;
#endif /* SPI6 */

#if defined(OCTOSPI1)
      case RCC_PERIPHCLK_OSPI:
        /* Get the current OSPI kernel source */
        srcclk = __HAL_RCC_GET_OSPI_SOURCE();

        switch (srcclk)
        {
          case RCC_OSPICLKSOURCE_HCLK:
          {
            frequency = HAL_RCC_GetHCLKFreq();
            break;
          }
          case RCC_OSPICLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_OSPICLKSOURCE_PLL2R:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_R_Frequency;
            break;
          }
          case RCC_OSPICLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* OCTOSPI1*/

#if defined(CEC)
      case RCC_PERIPHCLK_CEC:
        /* Get the current CEC source */
        srcclk = __HAL_RCC_GET_CEC_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_CECCLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY)) && (srcclk == RCC_CECCLKSOURCE_LSI))
        {
          frequency = LSI_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY)) && (srcclk == RCC_CECCLKSOURCE_CSI_DIV122))
        {
          frequency = CSI_VALUE / 122U;
        }

        /* Clock not enabled for CEC */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* CEC */

      case RCC_PERIPHCLK_RNG:
        /* Get the current RNG source */
        srcclk = __HAL_RCC_GET_RNG_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSI48RDY)) && (srcclk == RCC_RNGCLKSOURCE_HSI48))
        {
          frequency = HSI48_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL1RDY)) && (srcclk == RCC_RNGCLKSOURCE_PLL1Q))
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY)) && (srcclk == RCC_RNGCLKSOURCE_LSE))
        {
          frequency = LSE_VALUE;
        }
        else if ((HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY)) && (srcclk == RCC_RNGCLKSOURCE_LSI))
        {
          frequency = LSI_VALUE;
        }

        /* Clock not enabled for RNG */
        else
        {
          frequency = 0U;
        }
        break;

#if defined(USB_DRD_FS)
      case RCC_PERIPHCLK_USB:
        /* Get the current USB kernel source */
        srcclk = __HAL_RCC_GET_USB_SOURCE();

        if (srcclk == RCC_USBCLKSOURCE_PLL1Q)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
          break;
        }
#if defined(RCC_USBCLKSOURCE_PLL3Q)
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL3RDY)) && (srcclk == RCC_USBCLKSOURCE_PLL3Q))
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#else
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_PLL2RDY)) && (srcclk == RCC_USBCLKSOURCE_PLL2Q))
        {
          HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
          frequency = pll2_clocks.PLL2_Q_Frequency;
        }
#endif /* RCC_USBCLKSOURCE_PLL3 */
        else if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSI48RDY)) && (srcclk == RCC_USBCLKSOURCE_HSI48))
        {
          frequency = HSI48_VALUE;
        }
        /* Clock not enabled for USB */
        else
        {
          frequency = 0U;
        }

        break;


      default:
        frequency = 0U;
        break;
#endif /* USB_DRD_FS */

#if defined(PLAY1)
      case RCC_PERIPHCLK_PLAY1:
        /* Get the current PLAY1 kernel source */
        srcclk = __HAL_RCC_GET_PLAY1_SOURCE();

        /* Get the current PLAY1 clock divider */
        play1_divider = __HAL_RCC_GET_PLAY1_DIV();
        switch (srcclk)
        {
          case RCC_PLAY1CLKSOURCE_PCLK3:
          {
            frequency = HAL_RCC_GetPCLK3Freq() >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            break;
          }
          case RCC_PLAY1CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            break;
          }
#if defined(RCC_PLAY1CLKSOURCE_PLL3R)
          case RCC_PLAY1CLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            break;
          }
#endif /* RCC_PLAY1CLKSOURCE_PLL3R */
          case RCC_PLAY1CLKSOURCE_LSE:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSERDY))
            {
              frequency = LSE_VALUE >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            }
            break;
          }
          case  RCC_PLAY1CLKSOURCE_LSI:
          {
            if (HAL_IS_BIT_SET(RCC->BDCR, RCC_BDCR_LSIRDY))
            {
              frequency = LSI_VALUE >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            }
            break;
          }
          case RCC_PLAY1CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos)) >> 
                          (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE >> (play1_divider >> RCC_CCIPR3_PLAY1DIV_Pos);
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }
            break;
          }

          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* PLAY1*/
#if defined(USB_OTG_FS)
      case RCC_PERIPHCLK_OTGFS:
        /* Get the current USB_OTG_FS kernel source */
        srcclk = __HAL_RCC_GET_OTGFS_SOURCE();

        if ((HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSI48RDY)) && (srcclk == RCC_OTGFSCLKSOURCE_HSI48))
        {
          frequency = HSI48_VALUE;
        }
        else if (srcclk == RCC_OTGFSCLKSOURCE_PLL1Q)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = pll1_clocks.PLL1_Q_Frequency;
        }
        else if (srcclk == RCC_OTGFSCLKSOURCE_PLL3Q)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
#if defined(PWR_USBSCR_OTGHSEN)
        else if (srcclk == RCC_OTGFSCLKSOURCE_CLK48)
        {
          frequency = OTGHS_CLK48_VALUE;
        }
#endif /* PWR_USBSCR_OTGHSEN */
        /* Clock not enabled for USB_OTG_FS */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* USB_OTG_FS*/
#if defined(USB_OTG_HS)
      case RCC_PERIPHCLK_OTGHS:
        /* Get the current USB_OTG_HS kernel source */
        srcclk = __HAL_RCC_GET_OTGHS_SOURCE();

        if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (srcclk == RCC_OTGHSCLKSOURCE_HSE))
        {
          frequency = HSE_VALUE;
        }
        else if (srcclk == RCC_OTGHSCLKSOURCE_PLL3Q)
        {
          HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
          frequency = pll3_clocks.PLL3_Q_Frequency;
        }
        else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (srcclk == RCC_OTGHSCLKSOURCE_HSE_DIV2))
        {
          frequency = HSE_VALUE / 2 ;
        }
        else if (srcclk == RCC_OTGHSCLKSOURCE_PLL1Q_DIV2)
        {
          HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
          frequency = (pll1_clocks.PLL1_P_Frequency) / 2U;
        }
        /* Clock not enabled for USB_OTG_HS */
        else
        {
          frequency = 0U;
        }
        break;
#endif /* USB_OTG_HS*/

#if defined(OCTOSPI2)
      case RCC_PERIPHCLK_OSPI2:
        /* Get the current OSPI2 kernel source */
        srcclk = __HAL_RCC_GET_OSPI2_SOURCE();

        switch (srcclk)
        {
          case RCC_OSPI2CLKSOURCE_HCLK:
          {
            frequency = HAL_RCC_GetHCLKFreq();
            break;
          }
          case RCC_OSPI2CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_OSPI2CLKSOURCE_PLL2R:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_R_Frequency;
            break;
          }
          case RCC_OSPI2CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* OCTOSPI2*/

#if defined (LTDC)
      case RCC_PERIPHCLK_LTDC:

        /* Get the current LTDC kernel source */
        srcclk = __HAL_RCC_GET_LTDC_SOURCE();

        switch (srcclk)
        {
          case RCC_LTDCCLKSOURCE_PLL3R:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_R_Frequency;
            break;
          }
          case RCC_LTDCCLKSOURCE_PLL2R:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_R_Frequency;
            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* LTDC */

#if defined (MDF1)
      case RCC_PERIPHCLK_MDF1:

        /* Get the current MDF1 kernel source */
        srcclk = __HAL_RCC_GET_MDF1_SOURCE();

        switch (srcclk)
        {
          case RCC_MDF1CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_MDF1CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_MDF1CLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
          case RCC_MDF1CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_MDF1CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }

            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
      case RCC_PERIPHCLK_ETH:

        /* Get the current ETH kernel source */
        srcclk = __HAL_RCC_GET_ETH_SOURCE();
        switch (srcclk)
        {
          case RCC_ETHCLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_ETHCLKSOURCE_HSE:
          {
            frequency = HSE_VALUE;
            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
      case RCC_PERIPHCLK_ETHPTP:
        /* Get the current ETHPTP kernel source */
        srcclk = __HAL_RCC_GET_ETHPTP_SOURCE();

        /* Get the current ETHPTP clock divider */
        ethptp_divider = __HAL_RCC_GET_ETHPTP_DIV();

        switch (srcclk)
        {
          case RCC_ETHPTPCLKSOURCE_HCLK:
          {
            frequency = HAL_RCC_GetHCLKFreq() / ethptp_divider;
            break;
          }
          case RCC_ETHPTPCLKSOURCE_PLL1R:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_R_Frequency / ethptp_divider;
            break;
          }
          case RCC_ETHPTPCLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency / ethptp_divider;
            break;
          }
#if defined(RCC_ETHPTPCLKSOURCE_PLL3P)
          case RCC_ETHPTPCLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency / ethptp_divider;
            break;
          }
#endif /* RCC_ETHPTPCLKSOURCE_PLL3P */
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
      case RCC_PERIPHCLK_ETHREF:

        /* Get the current ETHREF kernel source */
        srcclk = __HAL_RCC_GET_ETHREF_SOURCE();
        switch (srcclk)
        {
          case RCC_ETHREFCLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_ETHREFCLKSOURCE_PLL1R:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_R_Frequency;
            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

#if defined (ADF1)
      case RCC_PERIPHCLK_ADF1:

        /* Get the current ADF1 kernel source */
        srcclk = __HAL_RCC_GET_ADF1_SOURCE();
        switch (srcclk)
        {
          case RCC_ADF1CLKSOURCE_PLL1Q:
          {
            HAL_RCCEx_GetPLL1ClockFreq(&pll1_clocks);
            frequency = pll1_clocks.PLL1_Q_Frequency;
            break;
          }
          case RCC_ADF1CLKSOURCE_PLL2P:
          {
            HAL_RCCEx_GetPLL2ClockFreq(&pll2_clocks);
            frequency = pll2_clocks.PLL2_P_Frequency;
            break;
          }
          case RCC_ADF1CLKSOURCE_PLL3P:
          {
            HAL_RCCEx_GetPLL3ClockFreq(&pll3_clocks);
            frequency = pll3_clocks.PLL3_P_Frequency;
            break;
          }
          case RCC_ADF1CLKSOURCE_PIN:
          {
            frequency = EXTERNAL_CLOCK_VALUE;
            break;
          }
          case RCC_ADF1CLKSOURCE_CLKP:
          {
            ckpclocksource = __HAL_RCC_GET_CLKP_SOURCE();

            if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_HSI))
            {
              /* In Case the CKPER Source is HSI */
              frequency = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_CSIRDY) && (ckpclocksource == RCC_CLKPSOURCE_CSI))
            {
              /* In Case the CKPER Source is CSI */
              frequency = CSI_VALUE;
            }

            else if (HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY) && (ckpclocksource == RCC_CLKPSOURCE_HSE))
            {
              /* In Case the CKPER Source is HSE */
              frequency = HSE_VALUE;
            }

            else
            {
              /* In Case the CKPER is disabled*/
              frequency = 0U;
            }
            break;
          }
          default:
          {
            frequency = 0U;
            break;
          }
        }
        break;
#endif /* ADF1 */
    }
  }
  return (frequency);
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
    activation or deactivation of PLL2, PLL3, LSE CSS,
    Low speed clock output and clock after wake-up from STOP mode.
@endverbatim
  * @{
  */

/**
  * @brief  Initialize and Enable the PLL2  according to the specified
  *         parameters in the RCC_PLL2InitTypeDef.
  * @param  pPLL2Init  pointer to an RCC_PLL2InitTypeDef structure that
  *         contains the configuration information for the PLL2
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLL2(RCC_PLL2InitTypeDef  *pPLL2Init)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLL2 Parameters used to output PLL2CLK */
  assert_param(IS_RCC_PLL2_SOURCE(pPLL2Init->PLL2Source));
  assert_param(IS_RCC_PLL2_DIVM_VALUE(pPLL2Init->PLL2M));
  assert_param(IS_RCC_PLL2_MULN_VALUE(pPLL2Init->PLL2N));
  assert_param(IS_RCC_PLL2_DIVP_VALUE(pPLL2Init->PLL2P));
  assert_param(IS_RCC_PLL2_DIVQ_VALUE(pPLL2Init->PLL2Q));
  assert_param(IS_RCC_PLL2_DIVR_VALUE(pPLL2Init->PLL2R));
  assert_param(IS_RCC_PLL2_CLOCKOUT_VALUE(pPLL2Init->PLL2ClockOut));
  assert_param(IS_RCC_PLL2_VCIRGE_VALUE(pPLL2Init->PLL2RGE));
  assert_param(IS_RCC_PLL2_VCORGE_VALUE(pPLL2Init->PLL2VCOSEL));
  assert_param(IS_RCC_PLL2_FRACN_VALUE(pPLL2Init->PLL2FRACN));

  /* Disable the PLL2 */
  __HAL_RCC_PLL2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL2 is ready to be updated */
  while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if (status == HAL_OK)
  {
    /* Make sure PLL2Source is ready */
    status = RCCEx_PLLSource_Enable(pPLL2Init->PLL2Source);

    if (status == HAL_OK)
    {
      /* Configure the PLL2 clock source, multiplication factor N, */
      /* and division factors M, P, Q and R */
      __HAL_RCC_PLL2_CONFIG(pPLL2Init->PLL2Source, pPLL2Init->PLL2M, pPLL2Init->PLL2N,
                            pPLL2Init->PLL2P, pPLL2Init->PLL2Q, pPLL2Init->PLL2R);

      /* Disable PLL2FRACN . */
      __HAL_RCC_PLL2_FRACN_DISABLE();

      /* Configure PLL2 FRACN */
      __HAL_RCC_PLL2_FRACN_CONFIG(pPLL2Init->PLL2FRACN);

      /* Enable PLL2FRACN */
      __HAL_RCC_PLL2_FRACN_ENABLE();

      /* Select PLL2 input reference frequency range: VCI */
      __HAL_RCC_PLL2_VCIRANGE(pPLL2Init->PLL2RGE);

      /* Select PLL2 output frequency range : VCO */
      __HAL_RCC_PLL2_VCORANGE(pPLL2Init->PLL2VCOSEL);

      /* Configure the PLL2 Clock output(s) */
      __HAL_RCC_PLL2_CLKOUT_ENABLE(pPLL2Init->PLL2ClockOut);

      /* Enable the PLL2 again by setting PLL2ON to 1*/
      __HAL_RCC_PLL2_ENABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till PLL2 is ready */
      while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) == 0U)
      {
        if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
        {
          status = HAL_TIMEOUT;
          break;
        }
      }
    }
  }

  return status;
}

/**
  * @brief  Disable PLL2.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLL2(void)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable the PLL2 */
  __HAL_RCC_PLL2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL2 is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  /* To save power, disable the PLL2 Source, FRACN and Clock outputs */
  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2PEN | RCC_PLL2CFGR_PLL2QEN | RCC_PLL2CFGR_PLL2REN | RCC_PLL2CFGR_PLL2SRC |
            RCC_PLL2CFGR_PLL2FRACEN);

  return status;
}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Initialize and Enable the PLL3  according to the specified
  *         parameters in the RCC_PLL3InitTypeDef.
  * @param  pPLL3Init  pointer to an RCC_PLL3InitTypeDef structure that
  *         contains the configuration information for the PLL3
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_RCCEx_EnablePLL3(RCC_PLL3InitTypeDef  *pPLL3Init)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status = HAL_OK;

  /* check for PLL3 Parameters used to output PLL3CLK */
  assert_param(IS_RCC_PLL3_SOURCE(pPLL3Init->PLL3Source));
  assert_param(IS_RCC_PLL3_DIVM_VALUE(pPLL3Init->PLL3M));
  assert_param(IS_RCC_PLL3_MULN_VALUE(pPLL3Init->PLL3N));
  assert_param(IS_RCC_PLL3_DIVP_VALUE(pPLL3Init->PLL3P));
  assert_param(IS_RCC_PLL3_DIVQ_VALUE(pPLL3Init->PLL3Q));
  assert_param(IS_RCC_PLL3_DIVR_VALUE(pPLL3Init->PLL3R));
  assert_param(IS_RCC_PLL3_CLOCKOUT_VALUE(pPLL3Init->PLL3ClockOut));
  assert_param(IS_RCC_PLL3_VCIRGE_VALUE(pPLL3Init->PLL3RGE));
  assert_param(IS_RCC_PLL3_VCORGE_VALUE(pPLL3Init->PLL3VCOSEL));
  assert_param(IS_RCC_PLL3_FRACN_VALUE(pPLL3Init->PLL3FRACN));

  /* Disable the PLL3 */
  __HAL_RCC_PLL3_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL3 is ready to be updated */
  while (READ_BIT(RCC->CR, RCC_CR_PLL3RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL3_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  if (status == HAL_OK)
  {
    /* Make sure PLL3Source is ready */
    status = RCCEx_PLLSource_Enable(pPLL3Init->PLL3Source);

    if (status == HAL_OK)
    {
      /* Configure the PLL3 clock source, multiplication factor N, */
      /* and division factors M and P */
      __HAL_RCC_PLL3_CONFIG(pPLL3Init->PLL3Source, pPLL3Init->PLL3M, pPLL3Init->PLL3N,  pPLL3Init->PLL3P,
                            pPLL3Init->PLL3Q, pPLL3Init->PLL3R);

      /* Disable PLL3FRACN . */
      __HAL_RCC_PLL3_FRACN_DISABLE();

      /* Configure PLL3 FRACN */
      __HAL_RCC_PLL3_FRACN_CONFIG(pPLL3Init->PLL3FRACN);

      /* Enable PLL3FRACN . */
      __HAL_RCC_PLL3_FRACN_ENABLE();

      /* Select PLL3 input reference frequency range: VCI */
      __HAL_RCC_PLL3_VCIRANGE(pPLL3Init->PLL3RGE);

      /* Select PLL3 output frequency range : VCO */
      __HAL_RCC_PLL3_VCORANGE(pPLL3Init->PLL3VCOSEL);

      /* Configure the PLL3 Clock output(s) */
      __HAL_RCC_PLL3_CLKOUT_ENABLE(pPLL3Init->PLL3ClockOut);

      /* Enable the PLL3 again by setting PLL3ON to 1*/
      __HAL_RCC_PLL3_ENABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till PLL3 is ready */
      while (READ_BIT(RCC->CR, RCC_CR_PLL3RDY) == 0U)
      {
        if ((HAL_GetTick() - tickstart) > PLL3_TIMEOUT_VALUE)
        {
          status = HAL_TIMEOUT;
          break;
        }
      }
    }
  }

  return status;
}


/**
  * @brief  Disable PLL3.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_RCCEx_DisablePLL3(void)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status = HAL_OK;

  /* Disable the PLL3 */
  __HAL_RCC_PLL3_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL3 is ready */
  while (READ_BIT(RCC->CR, RCC_CR_PLL3RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL3_TIMEOUT_VALUE)
    {
      status = HAL_TIMEOUT;
      break;
    }
  }

  /* To save power, disable the PLL3 Source and Clock outputs */
  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3PEN | RCC_PLL3CFGR_PLL3QEN | RCC_PLL3CFGR_PLL3REN | RCC_PLL3CFGR_PLL3SRC |
            RCC_PLL3CFGR_PLL3FRACEN);

  return status;
}
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  Configure the oscillator clock source for wakeup from Stop and HSE CSS backup clock.
  * @param  WakeUpClk  Wakeup clock
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_STOP_WAKEUPCLOCK_HSI  HSI oscillator selection
  *            @arg @ref RCC_STOP_WAKEUPCLOCK_CSI  CSI oscillator selection
  * @note   This function shall not be called after the Clock Security System on HSE has been
  *         enabled.
  * @retval None
  */
void HAL_RCCEx_WakeUpStopCLKConfig(uint32_t WakeUpClk)
{
  assert_param(IS_RCC_STOP_WAKEUPCLOCK(WakeUpClk));

  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(WakeUpClk);
}

/**
  * @brief  Configure the oscillator Kernel clock source for wakeup from Stop
  * @param  WakeUpClk: Kernel Wakeup clock
  *         This parameter can be one of the following values:
  *            @arg RCC_STOP_KERWAKEUPCLOCK_HSI: HSI oscillator selection
  *            @arg RCC_STOP_KERWAKEUPCLOCK_CSI: CSI oscillator selection
  * @retval None
  */
void HAL_RCCEx_KerWakeUpStopCLKConfig(uint32_t WakeUpClk)
{
  assert_param(IS_RCC_STOP_KERWAKEUPCLOCK(WakeUpClk));

  __HAL_RCC_KERWAKEUPSTOP_CLK_CONFIG(WakeUpClk);
}

/**
  * @brief  Enable the LSE Clock Security System.
  * @note   Prior to enable the LSE Clock Security System, LSE oscillator is to be enabled
  *         with HAL_RCC_OscConfig() and the LSE oscillator clock is to be selected as RTC
  *         clock with HAL_RCCEx_PeriphCLKConfig().
  * @retval None
  */
void HAL_RCCEx_EnableLSECSS(void)
{
  SET_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
}

/**
  * @brief  Disable the LSE Clock Security System.
  * @note   LSE Clock Security System can only be disabled after a LSE failure detection.
  * @retval None
  */
void HAL_RCCEx_DisableLSECSS(void)
{
  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSECSSON);
}

/**
  * @brief Handle the RCC LSE Clock Security System interrupt request.
  * @retval None
  */
void HAL_RCCEx_LSECSS_IRQHandler(void)
{
  if (READ_BIT(RCC->BDCR, RCC_BDCR_LSECSSD) != 0U)
  {
    /* RCC LSE Clock Security System interrupt user callback */
    HAL_RCCEx_LSECSS_Callback();
  }
}

/**
  * @brief  RCCEx LSE Clock Security System interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_LSECSS_Callback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_LSECSS_Callback should be implemented in the user file
   */
}

/**
  * @brief  Select the Low Speed Microcontroller Clock source to output on LSCO pin (PB2).
  * @param  LSCOSource  specifies the Low Speed clock source to output.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LSCOSOURCE_LSI  LSI clock selected as LSCO source
  *            @arg @ref RCC_LSCOSOURCE_LSE  LSE clock selected as LSCO source
  * @retval None
  */
void HAL_RCCEx_EnableLSCO(uint32_t LSCOSource)
{
  FlagStatus       backupchanged = RESET;

  /* Check the parameters */
  assert_param(IS_RCC_LSCOSOURCE(LSCOSource));

  /* Update LSCOSEL clock source in Backup Domain control register */
  if (HAL_IS_BIT_CLR(PWR->DBPCR, PWR_DBPCR_DBP))
  {
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }

  MODIFY_REG(RCC->BDCR, RCC_BDCR_LSCOSEL | RCC_BDCR_LSCOEN, LSCOSource | RCC_BDCR_LSCOEN);

  if (backupchanged == SET)
  {
    HAL_PWR_DisableBkUpAccess();
  }
}

/**
  * @brief  Disable the Low Speed Microcontroller Clock Output.
  * @retval None
  */
void HAL_RCCEx_DisableLSCO(void)
{
  FlagStatus       backupchanged = RESET;

  /* Update LSCOEN bit in Backup Domain control register */
  if (HAL_IS_BIT_CLR(PWR->DBPCR, PWR_DBPCR_DBP))
  {
    /* Enable access to the backup domain */
    HAL_PWR_EnableBkUpAccess();
    backupchanged = SET;
  }

  CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSCOEN);

  /* Restore previous configuration */
  if (backupchanged == SET)
  {
    /* Disable access to the backup domain */
    HAL_PWR_DisableBkUpAccess();
  }
}

/**
  * @}
  */

#if defined(CRS)

/** @defgroup RCCEx_Exported_Functions_Group3 Extended Clock Recovery System Control functions
  *  @brief  Extended Clock Recovery System Control functions
  *
@verbatim
 ===============================================================================
                ##### Extended Clock Recovery System Control functions  #####
 ===============================================================================
    [..]
      For devices with Clock Recovery System feature (CRS), RCC Extension HAL driver can be used as follows:

      (#) In System clock config, HSI48 needs to be enabled

      (#) Enable CRS clock in IP MSP init which will use CRS functions

      (#) Call CRS functions as follows:
          (##) Prepare synchronization configuration necessary for HSI48 calibration
              (+++) Default values can be set for frequency Error Measurement (reload and error limit)
                        and also HSI48 oscillator smooth trimming.
              (+++) Macro __HAL_RCC_CRS_RELOADVALUE_CALCULATE can be also used to calculate
                        directly reload value with target and synchronization frequencies values
          (##) Call function HAL_RCCEx_CRSConfig which
              (+++) Resets CRS registers to their default values.
              (+++) Configures CRS registers with synchronization configuration
              (+++) Enables automatic calibration and frequency error counter feature
           Note: When using USB LPM (Link Power Management) and the device is in Sleep mode, the
           periodic USB SOF will not be generated by the host. No SYNC signal will therefore be
           provided to the CRS to calibrate the HSI48 on the run. To guarantee the required clock
           precision after waking up from Sleep mode, the LSE or reference clock on the GPIOs
           should be used as SYNC signal.

          (##) A polling function is provided to wait for complete synchronization
              (+++) Call function HAL_RCCEx_CRSWaitSynchronization()
              (+++) According to CRS status, user can decide to adjust again the calibration or continue
                        application if synchronization is OK

      (#) User can retrieve information related to synchronization in calling function
            HAL_RCCEx_CRSGetSynchronizationInfo()

      (#) Regarding synchronization status and synchronization information, user can try a new calibration
           in changing synchronization configuration and call again HAL_RCCEx_CRSConfig.
           Note: When the SYNC event is detected during the downcounting phase (before reaching the zero value),
           it means that the actual frequency is lower than the target (and so, that the TRIM value should be
           incremented), while when it is detected during the upcounting phase it means that the actual frequency
           is higher (and that the TRIM value should be decremented).

      (#) In interrupt mode, user can resort to the available macros (__HAL_RCC_CRS_XXX_IT). Interrupts will go
          through CRS Handler (CRS_IRQn/CRS_IRQHandler)
              (++) Call function HAL_RCCEx_CRSConfig()
              (++) Enable CRS_IRQn (thanks to NVIC functions)
              (++) Enable CRS interrupt (__HAL_RCC_CRS_ENABLE_IT)
              (++) Implement CRS status management in the following user callbacks called from
                   HAL_RCCEx_CRS_IRQHandler():
                   (+++) HAL_RCCEx_CRS_SyncOkCallback()
                   (+++) HAL_RCCEx_CRS_SyncWarnCallback()
                   (+++) HAL_RCCEx_CRS_ExpectedSyncCallback()
                   (+++) HAL_RCCEx_CRS_ErrorCallback()

      (#) To force a SYNC EVENT, user can use the function HAL_RCCEx_CRSSoftwareSynchronizationGenerate().
          This function can be called before calling HAL_RCCEx_CRSConfig (for instance in Systick handler)

@endverbatim
  * @{
  */

/**
  * @brief  Start automatic synchronization for polling mode
  * @param  pInit Pointer on RCC_CRSInitTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSConfig(const RCC_CRSInitTypeDef *pInit)
{
  uint32_t value;

  /* Check the parameters */
  assert_param(IS_RCC_CRS_SYNC_DIV(pInit->Prescaler));
  assert_param(IS_RCC_CRS_SYNC_SOURCE(pInit->Source));
  assert_param(IS_RCC_CRS_SYNC_POLARITY(pInit->Polarity));
  assert_param(IS_RCC_CRS_RELOADVALUE(pInit->ReloadValue));
  assert_param(IS_RCC_CRS_ERRORLIMIT(pInit->ErrorLimitValue));
  assert_param(IS_RCC_CRS_HSI48CALIBRATION(pInit->HSI48CalibrationValue));

  /* CONFIGURATION */

  /* Before configuration, reset CRS registers to their default values*/
  __HAL_RCC_CRS_FORCE_RESET();
  __HAL_RCC_CRS_RELEASE_RESET();

  /* Set the SYNCDIV[2:0] bits according to Prescaler value */
  /* Set the SYNCSRC[1:0] bits according to Source value */
  /* Set the SYNCSPOL bit according to Polarity value */
  value = (pInit->Prescaler | pInit->Source | pInit->Polarity);
  /* Set the RELOAD[15:0] bits according to ReloadValue value */
  value |= pInit->ReloadValue;
  /* Set the FELIM[7:0] bits according to ErrorLimitValue value */
  value |= (pInit->ErrorLimitValue << CRS_CFGR_FELIM_Pos);
  WRITE_REG(CRS->CFGR, value);

  /* Adjust HSI48 oscillator smooth trimming */
  /* Set the TRIM[5:0] bits according to RCC_CRS_HSI48CalibrationValue value */
  MODIFY_REG(CRS->CR, CRS_CR_TRIM, (pInit->HSI48CalibrationValue << CRS_CR_TRIM_Pos));

  /* START AUTOMATIC SYNCHRONIZATION*/

  /* Enable Automatic trimming & Frequency error counter */
  SET_BIT(CRS->CR, CRS_CR_AUTOTRIMEN | CRS_CR_CEN);
}

/**
  * @brief  Generate the software synchronization event
  * @retval None
  */
void HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void)
{
  SET_BIT(CRS->CR, CRS_CR_SWSYNC);
}

/**
  * @brief  Return synchronization info
  * @param  pSynchroInfo Pointer on RCC_CRSSynchroInfoTypeDef structure
  * @retval None
  */
void HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo)
{
  /* Check the parameter */
  assert_param(pSynchroInfo != (void *)NULL);

  /* Get the reload value */
  pSynchroInfo->ReloadValue = (uint32_t)(READ_BIT(CRS->CFGR, CRS_CFGR_RELOAD));

  /* Get HSI48 oscillator smooth trimming */
  pSynchroInfo->HSI48CalibrationValue = (uint32_t)(READ_BIT(CRS->CR, CRS_CR_TRIM) >> CRS_CR_TRIM_Pos);

  /* Get Frequency error capture */
  pSynchroInfo->FreqErrorCapture = (uint32_t)(READ_BIT(CRS->ISR, CRS_ISR_FECAP) >> CRS_ISR_FECAP_Pos);

  /* Get Frequency error direction */
  pSynchroInfo->FreqErrorDirection = (uint32_t)(READ_BIT(CRS->ISR, CRS_ISR_FEDIR));
}

/**
  * @brief Wait for CRS Synchronization status.
  * @param Timeout  Duration of the timeout
  * @note  Timeout is based on the maximum time to receive a SYNC event based on synchronization
  *        frequency.
  * @note    If Timeout set to HAL_MAX_DELAY, HAL_TIMEOUT will be never returned.
  * @retval Combination of Synchronization status
  *          This parameter can be a combination of the following values:
  *            @arg @ref RCC_CRS_TIMEOUT
  *            @arg @ref RCC_CRS_SYNCOK
  *            @arg @ref RCC_CRS_SYNCWARN
  *            @arg @ref RCC_CRS_SYNCERR
  *            @arg @ref RCC_CRS_SYNCMISS
  *            @arg @ref RCC_CRS_TRIMOVF
  */
uint32_t HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout)
{
  uint32_t crsstatus = RCC_CRS_NONE;
  uint32_t tickstart;

  /* Get timeout */
  tickstart = HAL_GetTick();

  /* Wait for CRS flag or timeout detection */
  do
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
      {
        crsstatus = RCC_CRS_TIMEOUT;
      }
    }
    /* Check CRS SYNCOK flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCOK))
    {
      /* CRS SYNC event OK */
      crsstatus |= RCC_CRS_SYNCOK;

      /* Clear CRS SYNC event OK bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCOK);
    }

    /* Check CRS SYNCWARN flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCWARN))
    {
      /* CRS SYNC warning */
      crsstatus |= RCC_CRS_SYNCWARN;

      /* Clear CRS SYNCWARN bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCWARN);
    }

    /* Check CRS TRIM overflow flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_TRIMOVF))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_TRIMOVF;

      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_TRIMOVF);
    }

    /* Check CRS Error flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCERR))
    {
      /* CRS SYNC Error */
      crsstatus |= RCC_CRS_SYNCERR;

      /* Clear CRS Error bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCERR);
    }

    /* Check CRS SYNC Missed flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_SYNCMISS))
    {
      /* CRS SYNC Missed */
      crsstatus |= RCC_CRS_SYNCMISS;

      /* Clear CRS SYNC Missed bit */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_SYNCMISS);
    }

    /* Check CRS Expected SYNC flag  */
    if (__HAL_RCC_CRS_GET_FLAG(RCC_CRS_FLAG_ESYNC))
    {
      /* frequency error counter reached a zero value */
      __HAL_RCC_CRS_CLEAR_FLAG(RCC_CRS_FLAG_ESYNC);
    }
  } while (RCC_CRS_NONE == crsstatus);

  return crsstatus;
}

/**
  * @brief Handle the Clock Recovery System interrupt request.
  * @retval None
  */
void HAL_RCCEx_CRS_IRQHandler(void)
{
  uint32_t crserror = RCC_CRS_NONE;
  /* Get current IT flags and IT sources values */
  uint32_t itflags = READ_REG(CRS->ISR);
  uint32_t itsources = READ_REG(CRS->CR);

  /* Check CRS SYNCOK flag  */
  if (((itflags & RCC_CRS_FLAG_SYNCOK) != 0U) && ((itsources & RCC_CRS_IT_SYNCOK) != 0U))
  {
    /* Clear CRS SYNC event OK flag */
    WRITE_REG(CRS->ICR, CRS_ICR_SYNCOKC);

    /* user callback */
    HAL_RCCEx_CRS_SyncOkCallback();
  }
  /* Check CRS SYNCWARN flag  */
  else if (((itflags & RCC_CRS_FLAG_SYNCWARN) != 0U) && ((itsources & RCC_CRS_IT_SYNCWARN) != 0U))
  {
    /* Clear CRS SYNCWARN flag */
    WRITE_REG(CRS->ICR, CRS_ICR_SYNCWARNC);

    /* user callback */
    HAL_RCCEx_CRS_SyncWarnCallback();
  }
  /* Check CRS Expected SYNC flag  */
  else if (((itflags & RCC_CRS_FLAG_ESYNC) != 0U) && ((itsources & RCC_CRS_IT_ESYNC) != 0U))
  {
    /* frequency error counter reached a zero value */
    WRITE_REG(CRS->ICR, CRS_ICR_ESYNCC);

    /* user callback */
    HAL_RCCEx_CRS_ExpectedSyncCallback();
  }
  /* Check CRS Error flags  */
  else
  {
    if (((itflags & RCC_CRS_FLAG_ERR) != 0U) && ((itsources & RCC_CRS_IT_ERR) != 0U))
    {
      if ((itflags & RCC_CRS_FLAG_SYNCERR) != 0U)
      {
        crserror |= RCC_CRS_SYNCERR;
      }
      if ((itflags & RCC_CRS_FLAG_SYNCMISS) != 0U)
      {
        crserror |= RCC_CRS_SYNCMISS;
      }
      if ((itflags & RCC_CRS_FLAG_TRIMOVF) != 0U)
      {
        crserror |= RCC_CRS_TRIMOVF;
      }

      /* Clear CRS Error flags */
      WRITE_REG(CRS->ICR, CRS_ICR_ERRC);

      /* user error callback */
      HAL_RCCEx_CRS_ErrorCallback(crserror);
    }
  }
}

/**
  * @brief  RCCEx Clock Recovery System SYNCOK interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_SyncOkCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_SyncOkCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System SYNCWARN interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_SyncWarnCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_SyncWarnCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System Expected SYNC interrupt callback.
  * @retval none
  */
__weak void HAL_RCCEx_CRS_ExpectedSyncCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_ExpectedSyncCallback should be implemented in the user file
   */
}

/**
  * @brief  RCCEx Clock Recovery System Error interrupt callback.
  * @param  Error Combination of Error status.
  *         This parameter can be a combination of the following values:
  *           @arg @ref RCC_CRS_SYNCERR
  *           @arg @ref RCC_CRS_SYNCMISS
  *           @arg @ref RCC_CRS_TRIMOVF
  * @retval none
  */
__weak void HAL_RCCEx_CRS_ErrorCallback(uint32_t Error)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Error);

  /* NOTE : This function should not be modified, when the callback is needed,
            the @ref HAL_RCCEx_CRS_ErrorCallback should be implemented in the user file
   */
}

/**
  * @}
  */

#endif /* CRS */

/**
  * @}
  */

/** @addtogroup RCCEx_Private_Functions
  * @{
  */

/**
  * @brief  Enable PLLx source clock and check ready flag
  * @param  PllSource contains the selected PLLx source clock (HSE, HSI or CSI)
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLLSource_Enable(uint32_t PllSource)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status = HAL_OK;

  switch (PllSource)
  {
    case RCC_PLL1_SOURCE_CSI:
      /* Check whether CSI in not ready and enable it */
      if (READ_BIT(RCC->CR, RCC_CR_CSIRDY) == 0U)
      {
        /* Enable the Internal Low power oscillator (CSI). */
        __HAL_RCC_CSI_ENABLE();

        /* Get timeout */
        tickstart = HAL_GetTick();

        /* Wait till CSI is ready */
        while (READ_BIT(RCC->CR, RCC_CR_CSIRDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CSI_TIMEOUT_VALUE)
          {
            status = HAL_TIMEOUT;
            break;
          }
        }
      }
      break;

    case RCC_PLL1_SOURCE_HSI:
      /* Check whether HSI in not ready and enable it */
      if (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
      {
        /* Enable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_ENABLE();

        /* Get timeout */
        tickstart = HAL_GetTick();

        /* Wait till HSI is ready */
        while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSI_TIMEOUT_VALUE)
          {
            status = HAL_TIMEOUT;
            break;
          }
        }
      }
      break;

    case RCC_PLL1_SOURCE_HSE:
      /* Check whether HSE in not ready and enable it */
      if (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0U)
      {
        /* Enable the External High Speed oscillator (HSE). */
        SET_BIT(RCC->CR, RCC_CR_HSEON);

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSE is ready */
        while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSE_TIMEOUT_VALUE)
          {
            status = HAL_TIMEOUT;
            break;
          }
        }
      }
      break;

    default:
      status = HAL_ERROR;
      break;
  }

  return status;
}

/**
  * @brief  Configure the PLL2 VCI/VCO ranges, multiplication and division factors and its output clock(s)
  * @param  pll2  pointer to an RCC_PLL2InitTypeDef structure that
  *         contains the configuration parameters M, N, FRACN, VCI/VCO ranges as well as PLL2 output clocks dividers
  * @note   PLL2 is temporary disabled to apply new parameters
  * @retval HAL status
  */
static HAL_StatusTypeDef RCCEx_PLL2_Config(const RCC_PLL2InitTypeDef *pll2)
{

  uint32_t tickstart;
  assert_param(IS_RCC_PLL2_SOURCE(pll2->PLL2Source));
  assert_param(IS_RCC_PLL2_DIVM_VALUE(pll2->PLL2M));
  assert_param(IS_RCC_PLL2_MULN_VALUE(pll2->PLL2N));
  assert_param(IS_RCC_PLL2_DIVP_VALUE(pll2->PLL2P));
  assert_param(IS_RCC_PLL2_DIVQ_VALUE(pll2->PLL2Q));
  assert_param(IS_RCC_PLL2_DIVR_VALUE(pll2->PLL2R));
  assert_param(IS_RCC_PLL2_CLOCKOUT_VALUE(pll2->PLL2ClockOut));
  assert_param(IS_RCC_PLL2_VCIRGE_VALUE(pll2->PLL2RGE));
  assert_param(IS_RCC_PLL2_VCORGE_VALUE(pll2->PLL2VCOSEL));
  assert_param(IS_RCC_PLL2_FRACN_VALUE(pll2->PLL2FRACN));

  /* Disable  PLL2. */
  __HAL_RCC_PLL2_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL2 is disabled */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Configure PLL2 multiplication and division factors. */
  __HAL_RCC_PLL2_CONFIG(pll2->PLL2Source,
                        pll2->PLL2M,
                        pll2->PLL2N,
                        pll2->PLL2P,
                        pll2->PLL2Q,
                        pll2->PLL2R);

  /* Select PLL2 input reference frequency range: VCI */
  __HAL_RCC_PLL2_VCIRANGE(pll2->PLL2RGE);

  /* Select PLL2 output frequency range : VCO */
  __HAL_RCC_PLL2_VCORANGE(pll2->PLL2VCOSEL);

  /* Configure the PLL2 Clock output(s) */
  __HAL_RCC_PLL2_CLKOUT_ENABLE(pll2->PLL2ClockOut);

  /* Disable PLL2FRACN . */
  __HAL_RCC_PLL2_FRACN_DISABLE();

  /* Configures PLL2 clock Fractional Part Of The Multiplication Factor */
  __HAL_RCC_PLL2_FRACN_CONFIG(pll2->PLL2FRACN);

  /* Enable PLL2FRACN . */
  __HAL_RCC_PLL2_FRACN_ENABLE();

  /* Enable  PLL2. */
  __HAL_RCC_PLL2_ENABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL2 is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL2RDY) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL2_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;

}

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  Configure the PLL3 VCI/VCO ranges, multiplication and division factors and its output clock(s)
  * @param  pll3  pointer to an RCC_PLL3InitTypeDef structure that
  *         contains the configuration parameters M, N, FRACN, VCI/VCO ranges as well as PLL3 output clocks dividers
  * @note   PLL3 is temporary disabled to apply new parameters
  * @retval HAL status.
  */
static HAL_StatusTypeDef RCCEx_PLL3_Config(const RCC_PLL3InitTypeDef *pll3)
{

  uint32_t tickstart;
  assert_param(IS_RCC_PLL3_SOURCE(pll3->PLL3Source));
  assert_param(IS_RCC_PLL3_DIVM_VALUE(pll3->PLL3M));
  assert_param(IS_RCC_PLL3_MULN_VALUE(pll3->PLL3N));
  assert_param(IS_RCC_PLL3_DIVP_VALUE(pll3->PLL3P));
  assert_param(IS_RCC_PLL3_DIVQ_VALUE(pll3->PLL3Q));
  assert_param(IS_RCC_PLL3_DIVR_VALUE(pll3->PLL3R));
  assert_param(IS_RCC_PLL3_CLOCKOUT_VALUE(pll3->PLL3ClockOut));
  assert_param(IS_RCC_PLL3_VCIRGE_VALUE(pll3->PLL3RGE));
  assert_param(IS_RCC_PLL3_VCORGE_VALUE(pll3->PLL3VCOSEL));
  assert_param(IS_RCC_PLL3_FRACN_VALUE(pll3->PLL3FRACN));

  /* Disable  PLL3. */
  __HAL_RCC_PLL3_DISABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL3 is disabled */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL3_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Configure PLL3 multiplication and division factors. */
  __HAL_RCC_PLL3_CONFIG(pll3->PLL3Source,
                        pll3->PLL3M,
                        pll3->PLL3N,
                        pll3->PLL3P,
                        pll3->PLL3Q,
                        pll3->PLL3R);

  /* Select PLL3 input reference frequency range: VCI */
  __HAL_RCC_PLL3_VCIRANGE(pll3->PLL3RGE) ;

  /* Select PLL3 output frequency range : VCO */
  __HAL_RCC_PLL3_VCORANGE(pll3->PLL3VCOSEL);

  /* Configure the PLL3 Clock output(s) */
  __HAL_RCC_PLL3_CLKOUT_ENABLE(pll3->PLL3ClockOut);

  /* Disable PLL3FRACN . */
  __HAL_RCC_PLL3_FRACN_DISABLE();

  /* Configures PLL3 clock Fractional Part Of The Multiplication Factor */
  __HAL_RCC_PLL3_FRACN_CONFIG(pll3->PLL3FRACN);

  /* Enable PLL3FRACN . */
  __HAL_RCC_PLL3_FRACN_ENABLE();

  /* Enable  PLL3. */
  __HAL_RCC_PLL3_ENABLE();

  /* Get Start Tick*/
  tickstart = HAL_GetTick();

  /* Wait till PLL3 is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_PLL3RDY) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > PLL3_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}
#endif /* RCC_CR_PLL3ON */

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
