/**
  ******************************************************************************
  * @file    stm32h5xx_hal_rcc.c
  * @author  MCD Application Team
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
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
  @verbatim
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]
      After reset the device is running from High Speed Internal oscillator
      (64 MHz) with Flash 3 wait states. Flash prefetch buffer, D-Cache
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.

      (+) There is no prescaler on High speed (AHBs) and Low speed (APBs) busses:
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in analog mode, except the JTAG pins which
          are assigned to be used for debug purpose.

    [..]
      Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (SAIx, RTC, ADC, USB, SDMMC, etc.)

  @endverbatim
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup RCC RCC
  * @brief RCC HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCC_Private_Constants RCC Private Constants
  * @{
  */

/** @defgroup RCC_Timeout_Value Timeout Values
  * @{
  */
#define RCC_LSI_TIMEOUT_VALUE           ((uint32_t)2U)    /* 2 ms (minimum Tick + 1) */
#define RCC_HSI48_TIMEOUT_VALUE         ((uint32_t)2U)    /* 2 ms (minimum Tick + 1) */
#define RCC_PLL_TIMEOUT_VALUE           ((uint32_t)2U)    /* 2 ms (minimum Tick + 1) */
#define RCC_CLOCKSWITCH_TIMEOUT_VALUE   ((uint32_t)5000U) /* 5 s    */
#define RCC_PLL_FRAC_WAIT_VALUE         1U        /* PLL Fractional part waiting time before new latch enable : 1 ms */
/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup RCC_Private_Macros RCC Private Macros
  * @{
  */

#define MCO1_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCO1_GPIO_PORT        GPIOA
#define MCO1_PIN              GPIO_PIN_8

#define MCO2_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define MCO2_GPIO_PORT         GPIOC
#define MCO2_PIN               GPIO_PIN_9

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup RCC_Exported_Functions RCC Exported Functions
  * @{
  */

/** @defgroup RCC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
  @verbatim
 ===============================================================================
           ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      This section provides functions allowing to configure the internal and external oscillators
      (HSE, HSI, LSE, CSI, LSI, PLL1, HSE CSS and MCOs) and the System busses clocks (SYSCLK, AHB, APB1, APB2
       and APB3).

    [..] Internal/external clock and PLL configuration
         (+) HSI (high-speed internal): 64 MHz factory-trimmed RC used directly or through
             the PLL as System clock source.

         (#) CSI is a low-power RC oscillator which can be used directly as system clock, peripheral
             clock, or PLL input. But even with frequency calibration, is less accurate than an
             external crystal oscillator or ceramic resonator.

         (+) LSI (low-speed internal): 32 KHz low consumption RC used as IWDG and/or RTC
             clock source.

         (+) HSE (high-speed external): 4 to 48 MHz crystal oscillator used directly or
             through the PLL as System clock source. Can be used also optionally as RTC clock source.

         (+) LSE (low-speed external): 32.768 KHz oscillator used optionally as RTC clock source.

         (+) PLL1 (clocked by HSI, HSE or CSI) providing up to three independent output clocks:
           (++) The first output is used to generate the high speed system clock (up to 250MHz).
           (++) The second output is used to generate the clock for the USB (48 MHz), the FDCAN1/2,
                the SPI1/2/3, the OCTOSPI, the RNG (<=48 MHz), the SDMMC1/2 and to generate an accurate
                clock to achieve high-quality audio performance on SAI1/2 interface.

         (+) PLL2 (clocked by HSI, HSE or CSI) providing up to three independent output clocks:
           (++) The first output is used to generate the clock for the LPTIMs, the SPI1/2/3 and to generate
                an accurate clock to achieve high-quality audio performance on SAI1/2 interface.
           (++) The second output is used to generate the clock for USARTs, the UARTs, the LPUART1,
                the FDCAN1/2, the SPI4/5/6 and the USB.
           (++) The third output is used to generate the clock the SDMMC1/2, the ADC/DAC, the I2C1/2,
                the I3C1/2 and the OCTOSPI.

         (+) PLL3 (clocked by HSI , HSE or CSI) providing up to three independent output clocks:
           (++) The first output is used to generate the clock for SPI1/2/3 and to generate an accurate
                clock to achieve high-quality audio performance on SAI1/2 interface.
           (++) The second  output is used to generate the clock for USARTs, the UARTs, the LPUART1,
                the SPI4/5/6 and the USB.
           (++) The third output is used to generate the clock for the I2Cs, the I3Cs and the LPTIMs.

         (+) HSE CSS (HSE Clock Security System): once enabled, if a HSE clock failure occurs
            (HSE used directly or through PLL1 as System clock source), the System clock
             is automatically switched to HSI and an interrupt is generated if enabled.
             The interrupt is linked to the Cortex-M33 NMI (Non-Maskable Interrupt)
             exception vector.

         (#) MCO1 (micro controller clock output1), used to output HSI, LSE, HSE, PLL1(PLL1_Q)
             or HSI48 clock (through a configurable pre-scaler) on PA8 pin.

         (#) MCO2 (micro controller clock output2), used to output HSE, PLL2(PLL2_P), SYSCLK,
             LSI, CSI, or PLL1(PLL1_P) clock (through a configurable pre-scaler) on PC9 pin.

    [..] System, AHB and APB busses clocks configuration
         (+) Several clock sources can be used to drive the System clock (SYSCLK): CSI, HSI, HSE and the main PLL.
             The AHB clock (HCLK) is derived from System clock through configurable
             prescaler and used to clock the CPU, memory and peripherals mapped
             on AHB bus (DMA, GPIO...). APB1 (PCLK1), APB2 (PCLK2) and APB3 (PCLK3) clocks are derived
             from AHB clock through configurable prescalers and used to clock
             the peripherals mapped on these busses. You can use
             "HAL_RCC_GetSysClockFreq()" function to retrieve the frequencies of these clocks.

         -@- All the peripheral clocks are derived from the System clock (SYSCLK) except:

           (+@) SAI: the SAI clock can be derived either from specific PLL (PLL1, PLL2 or PLL3),
                the per_ck clock (HSE, HSI or CSI) or from an external clock mapped on the SAI_CKIN pin.
                You have to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) SPI/I2S: the SPI1/2/3 clock can be derived either from specific PLL (PLL1, PLL2 or PLL3),
                the per_ck clock (HSE, HSI or CSI) or from an external clock mapped on the SPI_CKIN pin.
                You have to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) RTC: the RTC clock can be derived either from the LSI, LSE or HSE clock
                divided by 2 to 31.
                You have to use __HAL_RCC_RTC_ENABLE() and HAL_RCCEx_PeriphCLKConfig() function
                to configure this clock.
           (+@) USB: USB requires a frequency equal to 48 MHz to work correctly. This clock is derived
                of the main PLL or PLL2 through PLLQ divider. You have to use HAL_RCCEx_PeriphCLKConfig()
                function to configure this clock.
           (+@) UCPD: the UCPD clock is derived from HSI (divided by 4) clock.
           (+@) SDMMC: SDMMC1/2 peripherals require a frequency equal or lower than 48 MHz.
                This clock is derived from the PLL1 or PLL2 through PLL1Q or PLL2R divider. You have
                to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) IWDG clock which is always the LSI clock. You have to use HAL_RCCEx_PeriphCLKConfig()
                function to configure this clock.
           (+@) RNG: the RNG clock can be derived either from PLL1Q, HSI48, LSE or LSI clock. You have
                to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) DAC: the DAC clock can be derived either from LSE or LSI clock. You have
                to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) FDCAN: the FDCAN1/2 clock can be derived either from HSE, PLL1Q or PLL2Q clock. You have
                to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) CEC: the CEC clock can be derived either from LSE, LSI or CSI (divided by 122) clock.You have
                to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) ETH: the Ethernet clock is derived from PLL1Q clock.



         (+) The maximum frequency of the SYSCLK, HCLK, PCLK1, PCLK2 and PCLK3 is 250 MHz.
             The clock source frequency should be adapted depending on the device voltage range
             as listed in the Reference Manual "Clock source frequency versus voltage scaling" chapter.

  @endverbatim


           Table 1. HCLK clock frequency for STM32H5xx devices
           +-----------------------------------------------------------------------------------------------+
           | Latency         |                          HCLK clock frequency (MHz)                         |
           |                 |-----------------------------------------------------------------------------|
           |                 |  voltage range 0  |  voltage range 1 | voltage range 2  | voltage range 3   |
           |                 |    1.26 - 1.35V   |   1.15 - 1.26V   |   1.05 - 1.15V   |   0,95 - 1,05V    |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |0WS(1 CPU cycles)|   0 < HCLK <= 38  |  0 < HCLK <= 32  |  0 < HCLK <= 26  | 0 < HCLK <= 16    |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |1WS(2 CPU cycles)|  38 < HCLK <= 76  | 32 < HCLK <= 64  | 26 < HCLK <= 50  | 16 < HCLK <= 32   |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |2WS(3 CPU cycles)|  76 < HCLK <= 114 | 64 < HCLK <= 96  | 50 < HCLK <= 80  | 32 < HCLK <= 50   |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |3WS(4 CPU cycles)| 114 < HCLK <= 152 | 96 < HCLK <= 128 | 80 < HCLK <= 106 | 50 < HCLK <= 65   |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |4WS(5 CPU cycles)|  152 < HCLK <= 190| 128 < HCLK <= 160| 106 < HCLK <= 130| 65 < HCLK <= 80   |
           |-----------------|-------------------|------------------|------------------|-------------------|
           |5WS(6 CPU cycles)|  190 < HCLK <= 250| 160 < HCLK <= 180|        NA        |         NA        |
           +-----------------+-------------------+------------------+------------------+-------------------+
  * @{
  */

/**
  * @brief  Reset the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, CSI, PLL, PLL2 and PLL3 OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - HSECSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  * @retval HAL Status.
  */

HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
  uint32_t tickstart;

  /* Increasing the CPU frequency */
  if (FLASH_LATENCY_DEFAULT  > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_DEFAULT);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLASH_LATENCY_DEFAULT)
    {
      return HAL_ERROR;
    }

  }

  /* Get start tick*/
  tickstart = HAL_GetTick();

  /* Set HSION bit */
  SET_BIT(RCC->CR, RCC_CR_HSION);

  /* Wait till HSI is ready */
  while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > RCC_HSI_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Set HSIDIV Default value */
  CLEAR_BIT(RCC->CR, RCC_CR_HSIDIV);

  /* Set HSITRIM default value */
  WRITE_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM_6);


  /* Adapt Systick interrupt period */
  if (HAL_InitTick(uwTickPrio) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Get start tick*/
  tickstart = HAL_GetTick();

  /* Reset CFGR register (HSI is selected as system clock source) */
  CLEAR_REG(RCC->CFGR1);
  CLEAR_REG(RCC->CFGR2);

  /* Wait till clock switch is ready */
  while (READ_BIT(RCC->CFGR1, RCC_CFGR1_SWS) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > RCC_CLOCKSWITCH_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Reset HSECSSON, HSEON, HSIKERON, CSION, CSIKERON and HSI48ON bits */
  CLEAR_BIT(RCC->CR, RCC_CR_CSION | RCC_CR_CSIKERON | RCC_CR_HSECSSON | RCC_CR_HSIKERON | RCC_CR_HSI48ON | \
            RCC_CR_HSEON);

  /* Reset HSEEXT bit*/
  CLEAR_BIT(RCC->CR, RCC_CR_HSEEXT);

  /* Get Start Tick */
  tickstart = HAL_GetTick();

  /* Clear PLL1ON bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLL1ON);

  /* Wait till PLL1 is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Get Start Tick */
  tickstart = HAL_GetTick();

  /* Reset PLL2N bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLL2ON);

  /* Wait till PLL2 is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLL2RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

#if defined(RCC_CR_PLL3ON)

  /* Get Start Tick */
  tickstart = HAL_GetTick();

  /* Reset PLL3 bit */
  CLEAR_BIT(RCC->CR, RCC_CR_PLL3ON);

  /* Wait till PLL3 is disabled */
  while (READ_BIT(RCC->CR, RCC_CR_PLL3RDY) != 0U)
  {
    if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }
#endif /* RCC_CR_PLL3ON */

  /* Reset PLL1CFGR register */
  CLEAR_REG(RCC->PLL1CFGR);

  /* Reset PLL1DIVR register */
  WRITE_REG(RCC->PLL1DIVR, 0x01010280U);

  /* Reset PLL1FRACR register */
  CLEAR_REG(RCC->PLL1FRACR);

  /* Reset PLL2CFGR register */
  CLEAR_REG(RCC->PLL2CFGR);

  /* Reset PLL2DIVR register */
  WRITE_REG(RCC->PLL2DIVR, 0x01010280U);

  /* Reset PLL2FRACR register */
  CLEAR_REG(RCC->PLL2FRACR);

#if defined(RCC_CR_PLL3ON)
  /* Reset PLL3CFGR register */
  CLEAR_REG(RCC->PLL3CFGR);

  /* Reset PLL3DIVR register */
  WRITE_REG(RCC->PLL3DIVR, 0x01010280U);

  /* Reset PLL3FRACR register */
  CLEAR_REG(RCC->PLL3FRACR);
#endif /* RCC_CR_PLL3ON */

  /* Reset HSEBYP bit */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

  /* Disable all interrupts */
  CLEAR_REG(RCC->CIER);

  /* Clear all interrupts flags */
  WRITE_REG(RCC->CICR, 0xFFFFFFFFU);

  /* Reset all RSR flags */
  SET_BIT(RCC->RSR, RCC_RSR_RMVF);

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HSI_VALUE;

  /* Decreasing the number of wait states because of lower CPU frequency */
  if (FLASH_LATENCY_DEFAULT  < __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_DEFAULT);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLASH_LATENCY_DEFAULT)
    {
      return HAL_ERROR;
    }
  }

  /* Adapt Systick interrupt period */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  Initialize the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  pOscInitStruct pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this macro. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(const RCC_OscInitTypeDef  *pOscInitStruct)
{
  uint32_t tickstart;
  uint32_t temp_sysclksrc;
  uint32_t temp_pllckselr;
  uint32_t temp1_pllckcfg;
  uint32_t temp2_pllckcfg;

  /* Check Null pointer */
  if (pOscInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(pOscInitStruct->OscillatorType));
  temp_sysclksrc = __HAL_RCC_GET_SYSCLK_SOURCE();
  temp_pllckselr = __HAL_RCC_GET_PLL1_OSCSOURCE();

  /*----------------------------- CSI Configuration --------------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_CSI) == RCC_OSCILLATORTYPE_CSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_CSI(pOscInitStruct->CSIState));
    assert_param(IS_RCC_CSICALIBRATION_VALUE(pOscInitStruct->CSICalibrationValue));

    /* When the CSI is used as system clock it will not be disabled */
    if ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_CSI) ||
        ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (temp_pllckselr == RCC_PLL1_SOURCE_CSI)))
    {
      if (pOscInitStruct->CSIState == RCC_CSI_OFF)
      {
        return HAL_ERROR;
      }

      /* Otherwise, just the calibration and CSI is allowed */
      else
      {
        /* Adjusts the Internal Low-power oscillator (CSI) calibration value.*/
        __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(pOscInitStruct->CSICalibrationValue);
      }
    }
    else
    {
      /* Check the CSI State */
      if ((pOscInitStruct->CSIState) != RCC_CSI_OFF)
      {
        /* Enable the Internal High Speed oscillator (CSI). */
        __HAL_RCC_CSI_ENABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till CSI is ready */
        while (READ_BIT(RCC->CR, RCC_CR_CSIRDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Adjusts the Internal High Speed oscillator (CSI) calibration value.*/
        __HAL_RCC_CSI_CALIBRATIONVALUE_ADJUST(pOscInitStruct->CSICalibrationValue);
      }
      else
      {
        /* Disable the Internal High Speed oscillator (CSI). */
        __HAL_RCC_CSI_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till CSI is disabled */
        while (READ_BIT(RCC->CR, RCC_CR_CSIRDY) != 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*------------------------------- HSE Configuration ------------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSE) == RCC_OSCILLATORTYPE_HSE)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSE(pOscInitStruct->HSEState));

    /* When the HSE is used as system clock or clock source for PLL in these cases it is not allowed to be disabled */
    if ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_HSE) ||
        ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (temp_pllckselr == RCC_PLL1_SOURCE_HSE)))
    {
      if (pOscInitStruct->HSEState == RCC_HSE_OFF)
      {
        return HAL_ERROR;
      }
    }
    else
    {
      /* Set the new HSE configuration ---------------------------------------*/
      __HAL_RCC_HSE_CONFIG(pOscInitStruct->HSEState);

      /* Check the HSE State */
      if (pOscInitStruct->HSEState != RCC_HSE_OFF)
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSE is ready */
        while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSE is disabled */
        while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSE_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*----------------------------- HSI Configuration --------------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSI(pOscInitStruct->HSIState));
    assert_param(IS_RCC_HSIDIV(pOscInitStruct->HSIDiv));
    assert_param(IS_RCC_HSI_CALIBRATION_VALUE(pOscInitStruct->HSICalibrationValue));

    /* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
    if ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_HSI) ||
        ((temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_PLLCLK) && (temp_pllckselr == RCC_PLL1_SOURCE_HSI)))
    {
      /* When HSI is used as system clock it will not be disabled */
      if (pOscInitStruct->HSIState == RCC_HSI_OFF)
      {
        return HAL_ERROR;
      }
      /* Otherwise, HSI calibration and division may be allowed */
      else
      {

        /* HSI division is allowed if HSI is used as system clock */
        if (temp_sysclksrc == RCC_SYSCLKSOURCE_STATUS_HSI)
        {
          if (__HAL_RCC_GET_HSI_DIVIDER() != (pOscInitStruct->HSIDiv))
          {
            /* Adjust the HSI division factor */
            __HAL_RCC_HSI_DIVIDER_CONFIG(pOscInitStruct->HSIDiv);

            /* Update the SystemCoreClock global variable with new HSI value  */
            (void) HAL_RCC_GetHCLKFreq();

            /* Configure the source of time base considering new system clocks settings*/
            if (HAL_InitTick(uwTickPrio) != HAL_OK)
            {
              return HAL_ERROR;
            }
          }
        }

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSI is ready */
        while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
        /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(pOscInitStruct->HSICalibrationValue);
      }
    }
    else
    {
      /* Check the HSI State */
      if (pOscInitStruct->HSIState != RCC_HSI_OFF)
      {
        /* Adjust the HSI division factor */
        __HAL_RCC_HSI_DIVIDER_CONFIG(pOscInitStruct->HSIDiv);

        /* Enable the HSI oscillator */
        __HAL_RCC_HSI_ENABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSI is ready */
        while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Adjust the Internal High Speed oscillator (HSI) calibration value.*/
        __HAL_RCC_HSI_CALIBRATIONVALUE_ADJUST(pOscInitStruct->HSICalibrationValue);
      }
      else
      {
        /* Disable the Internal High Speed oscillator (HSI). */
        __HAL_RCC_HSI_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till HSI is disabled */
        while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) != 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_HSI_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }
  /*------------------------------ LSI Configuration -------------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSI) == RCC_OSCILLATORTYPE_LSI)
  {

    /* Check the parameters */
    assert_param(IS_RCC_LSI(pOscInitStruct->LSIState));

    /* Update LSI configuration in Backup Domain control register    */

    /* Check the LSI State */
    if (pOscInitStruct->LSIState != RCC_LSI_OFF)
    {
      /* Enable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_ENABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till LSI is ready */
      while (READ_BIT(RCC->BDCR, RCC_BDCR_LSIRDY) == 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Disable the Internal Low Speed oscillator (LSI). */
      __HAL_RCC_LSI_DISABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till LSI is disabled */
      while (READ_BIT(RCC->BDCR, RCC_BDCR_LSIRDY) != 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSI_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

  }
  /*------------------------------ LSE Configuration -------------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_LSE) == RCC_OSCILLATORTYPE_LSE)
  {

    /* Check the parameters */
    assert_param(IS_RCC_LSE(pOscInitStruct->LSEState));

    /* Update LSE configuration in Backup Domain control register    */
    /* Requires to enable write access to Backup Domain */
    if (HAL_IS_BIT_CLR(PWR->DBPCR, PWR_DBPCR_DBP))
    {
      /* Enable write access to Backup domain */
      SET_BIT(PWR->DBPCR, PWR_DBPCR_DBP);

      /* Wait for Backup domain Write protection disable */
      tickstart = HAL_GetTick();

      while (HAL_IS_BIT_CLR(PWR->DBPCR, PWR_DBPCR_DBP))
      {
        if ((HAL_GetTick() - tickstart) > RCC_DBP_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

    /* Set the new LSE configuration -----------------------------------------*/
    __HAL_RCC_LSE_CONFIG(pOscInitStruct->LSEState);

    /* Check the LSE State */
    if (pOscInitStruct->LSEState != RCC_LSE_OFF)
    {
      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till LSE is ready */
      while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till LSE is disabled */
      while (READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) != 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_LSE_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }

  }
  /*------------------------------ HSI48 Configuration -----------------------*/
  if (((pOscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_HSI48) == RCC_OSCILLATORTYPE_HSI48)
  {
    /* Check the parameters */
    assert_param(IS_RCC_HSI48(pOscInitStruct->HSI48State));

    /* Check the HSI48 State */
    if (pOscInitStruct->HSI48State != RCC_HSI48_OFF)
    {
      /* Enable the Internal High Speed oscillator (HSI48). */
      __HAL_RCC_HSI48_ENABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till HSI48 is ready */
      while (READ_BIT(RCC->CR, RCC_CR_HSI48RDY) == 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_HSI48_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      /* Disable the Internal High Speed oscillator (HSI48). */
      __HAL_RCC_HSI48_DISABLE();

      /* Get Start Tick*/
      tickstart = HAL_GetTick();

      /* Wait till HSI48 is disabled */
      while (READ_BIT(RCC->CR, RCC_CR_HSI48RDY) != 0U)
      {
        if ((HAL_GetTick() - tickstart) > RCC_HSI48_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }

  /*-------------------------------- PLL1 Configuration -----------------------*/
  /* Check the parameters */
  assert_param(IS_RCC_PLL(pOscInitStruct->PLL.PLLState));

  if ((pOscInitStruct->PLL.PLLState) != RCC_PLL_NONE)
  {
    /* Check if the PLL1 is used as system clock or not */
    if (temp_sysclksrc != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
    {
      if ((pOscInitStruct->PLL.PLLState) == RCC_PLL_ON)
      {
        /* Check the parameters */
        assert_param(IS_RCC_PLL1_SOURCE(pOscInitStruct->PLL.PLLSource));
        assert_param(IS_RCC_PLL1_DIVM_VALUE(pOscInitStruct->PLL.PLLM));
        assert_param(IS_RCC_PLL1_MULN_VALUE(pOscInitStruct->PLL.PLLN));
        assert_param(IS_RCC_PLL1_DIVP_VALUE(pOscInitStruct->PLL.PLLP));
        assert_param(IS_RCC_PLL1_DIVQ_VALUE(pOscInitStruct->PLL.PLLQ));
        assert_param(IS_RCC_PLL1_DIVR_VALUE(pOscInitStruct->PLL.PLLR));

        /* Disable the PLL1. */
        __HAL_RCC_PLL1_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL1 is disabled */
        while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) != 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Configure the PLL1 clock source, multiplication and division factors. */
        __HAL_RCC_PLL1_CONFIG(pOscInitStruct->PLL.PLLSource,
                              pOscInitStruct->PLL.PLLM,
                              pOscInitStruct->PLL.PLLN,
                              pOscInitStruct->PLL.PLLP,
                              pOscInitStruct->PLL.PLLQ,
                              pOscInitStruct->PLL.PLLR);

        assert_param(IS_RCC_PLL1_FRACN_VALUE(pOscInitStruct->PLL.PLLFRACN));

        /* Disable PLL1FRACN . */
        __HAL_RCC_PLL1_FRACN_DISABLE();

        /* Configure PLL  PLL1FRACN */
        __HAL_RCC_PLL1_FRACN_CONFIG(pOscInitStruct->PLL.PLLFRACN);

        /* Enable PLL1FRACN . */
        __HAL_RCC_PLL1_FRACN_ENABLE();

        assert_param(IS_RCC_PLL1_VCIRGE_VALUE(pOscInitStruct->PLL.PLLRGE));

        /* Select PLL1 input reference frequency range: VCI */
        __HAL_RCC_PLL1_VCIRANGE(pOscInitStruct->PLL.PLLRGE) ;

        assert_param(IS_RCC_PLL1_VCORGE_VALUE(pOscInitStruct->PLL.PLLVCOSEL));

        /* Select PLL1 output frequency range : VCO */
        __HAL_RCC_PLL1_VCORANGE(pOscInitStruct->PLL.PLLVCOSEL) ;

        /* Enable PLL1 System Clock output. */
        __HAL_RCC_PLL1_CLKOUT_ENABLE(RCC_PLL1_DIVP);

        /* Enable the PLL1. */
        __HAL_RCC_PLL1_ENABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL1 is ready */
        while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        /* Disable the PLL1. */
        __HAL_RCC_PLL1_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait till PLL1 is disabled */
        while (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) != 0U)
        {
          if ((HAL_GetTick() - tickstart) > RCC_PLL_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }

        /* Unselect PLL1 clock source and disable all PLL1 outputs to save power */
        RCC->PLL1CFGR &= ~(RCC_PLL1CFGR_PLL1SRC | RCC_PLL1CFGR_PLL1PEN | RCC_PLL1CFGR_PLL1QEN | RCC_PLL1CFGR_PLL1REN);

      }
    }
    else
    {
      /* Do not return HAL_ERROR if request repeats the current configuration */
      temp1_pllckcfg = RCC->PLL1CFGR;
      temp2_pllckcfg = RCC->PLL1DIVR;
      if (((pOscInitStruct->PLL.PLLState) == RCC_PLL_OFF) ||
          (READ_BIT(temp1_pllckcfg, RCC_PLL1CFGR_PLL1SRC) != pOscInitStruct->PLL.PLLSource) ||
          ((READ_BIT(temp1_pllckcfg, RCC_PLL1CFGR_PLL1M) >> \
            RCC_PLL1CFGR_PLL1M_Pos) != (pOscInitStruct->PLL.PLLM)) ||
          (READ_BIT(temp2_pllckcfg, RCC_PLL1DIVR_PLL1N) != (pOscInitStruct->PLL.PLLN - 1U)) ||
          ((READ_BIT(temp2_pllckcfg, RCC_PLL1DIVR_PLL1P) >> \
            RCC_PLL1DIVR_PLL1P_Pos) != (pOscInitStruct->PLL.PLLP - 1U)) ||
          ((READ_BIT(temp2_pllckcfg, RCC_PLL1DIVR_PLL1Q) >> \
            RCC_PLL1DIVR_PLL1Q_Pos) != (pOscInitStruct->PLL.PLLQ - 1U)) ||
          ((READ_BIT(temp2_pllckcfg, RCC_PLL1DIVR_PLL1R) >> \
            RCC_PLL1DIVR_PLL1R_Pos) != (pOscInitStruct->PLL.PLLR - 1U)))
      {
        return HAL_ERROR;
      }

      /* FRACN1 on-the-fly value update */
      if ((READ_BIT(RCC->PLL1FRACR, RCC_PLL1FRACR_PLL1FRACN) >> \
           RCC_PLL1FRACR_PLL1FRACN_Pos) != (pOscInitStruct->PLL.PLLFRACN))
      {
        assert_param(IS_RCC_PLL1_FRACN_VALUE(pOscInitStruct->PLL.PLLFRACN));

        /* Disable PLL1FRACN . */
        __HAL_RCC_PLL1_FRACN_DISABLE();

        /* Get Start Tick*/
        tickstart = HAL_GetTick();

        /* Wait at least 2 CK_REF (PLL input source divided by M) period to make sure next latched value
           will be taken into account. */
        while ((HAL_GetTick() - tickstart) < RCC_PLL_FRAC_WAIT_VALUE)
        {
        }

        /* Configure PLL PLL1FRACN */
        __HAL_RCC_PLL1_FRACN_CONFIG(pOscInitStruct->PLL.PLLFRACN);

        /* Enable PLL1FRACN to latch the new value. */
        __HAL_RCC_PLL1_FRACN_ENABLE();
      }

    }
  }
  return HAL_OK;
}

/**
  * @brief  Initialize the CPU, AHB and APB busses clocks according to the specified
  *         parameters in the pClkInitStruct.
  * @param  pClkInitStruct  pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC peripheral.
  * @param  FLatency  FLASH Latency
  *          This parameter can be one of the following values:
  *            @arg FLASH_LATENCY_0   FLASH 0 Latency cycle
  *            @arg FLASH_LATENCY_1   FLASH 1 Latency cycle
  *            @arg FLASH_LATENCY_2   FLASH 2 Latency cycles
  *            @arg FLASH_LATENCY_3   FLASH 3 Latency cycles
  *            @arg FLASH_LATENCY_4   FLASH 4 Latency cycles
  *            @arg FLASH_LATENCY_5   FLASH 5 Latency cycles
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency
  *         and updated by HAL_RCC_GetHCLKFreq() function called within this function
  *
  * @note   The HSI is used by default as system clock source after
  *         startup from Reset, wake-up from STANDBY mode. After restart from Reset,
  *         the HSI frequency is set to its default value 64 MHz.
  *
  * @note   The HSI or CSI can be selected as system clock source after wake-up
  *         from STOP modes or in case of failure of the HSE when used directly or indirectly
  *         as system clock (if the Clock Security System CSS is enabled).
  *
  * @note   A switch from one clock source to another occurs only if the target
  *         clock source is ready (clock stable after startup delay or PLL locked).
  *         If a clock source which is not yet ready is selected, the switch will
  *         occur when the clock source is ready.
  *
  * @note   You can use HAL_RCC_GetClockConfig() function to know which clock is
  *         currently used as system clock source.
  *
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_RCC_ClockConfig(const RCC_ClkInitTypeDef  *pClkInitStruct, uint32_t FLatency)
{
  HAL_StatusTypeDef halstatus;
  uint32_t tickstart;

  /* Check Null pointer */
  if (pClkInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(pClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
    must be correctly programmed according to the frequency of the CPU clock
    (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  if (FLatency > __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /* Increasing the BUS frequency divider */
  /*-------------------------- PCLK3 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK3) == RCC_CLOCKTYPE_PCLK3)
  {
    if ((pClkInitStruct->APB3CLKDivider) > ((RCC->CFGR2 & RCC_CFGR2_PPRE3) >> 8))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB3CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE3, ((pClkInitStruct->APB3CLKDivider) << 8));
    }
  }
  /*-------------------------- PCLK2 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    if ((pClkInitStruct->APB2CLKDivider) > ((RCC->CFGR2 & RCC_CFGR2_PPRE2) >> 4))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB2CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE2, ((pClkInitStruct->APB2CLKDivider) << 4));
    }
  }

  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    if ((pClkInitStruct->APB1CLKDivider) > (RCC->CFGR2 & RCC_CFGR2_PPRE1))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB1CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE1, pClkInitStruct->APB1CLKDivider);
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    if ((pClkInitStruct->AHBCLKDivider) > (RCC->CFGR2 & RCC_CFGR2_HPRE))
    {
      assert_param(IS_RCC_HCLK(pClkInitStruct->AHBCLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_HPRE, pClkInitStruct->AHBCLKDivider);
    }
  }

  /*------------------------- SYSCLK Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {
    assert_param(IS_RCC_SYSCLKSOURCE(pClkInitStruct->SYSCLKSource));

    /* PLL is selected as System Clock Source */
    if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* Check the PLL ready flag */
      if (READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == 0U)
      {
        return HAL_ERROR;
      }
    }
    else
    {
      /* HSE is selected as System Clock Source */
      if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        /* Check the HSE ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0U)
        {
          return HAL_ERROR;
        }
      }
      /* CSI is selected as System Clock Source */
      else if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_CSI)
      {
        /* Check the CSI ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_CSIRDY) == 0U)
        {
          return HAL_ERROR;
        }
      }
      /* HSI is selected as System Clock Source */
      else
      {
        /* Check the HSI ready flag */
        if (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0U)
        {
          return HAL_ERROR;
        }
      }
    }

    MODIFY_REG(RCC->CFGR1, RCC_CFGR1_SW, pClkInitStruct->SYSCLKSource);

    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
      {
        if ((HAL_GetTick() - tickstart) > RCC_CLOCKSWITCH_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSE)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else if (pClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_CSI)
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_CSI)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
      else
      {
        while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI)
        {
          if ((HAL_GetTick() - tickstart) > RCC_CLOCKSWITCH_TIMEOUT_VALUE)
          {
            return HAL_TIMEOUT;
          }
        }
      }
    }
  }

  /* Decreasing the BUS frequency divider */
  /*-------------------------- HCLK Configuration --------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    if ((pClkInitStruct->AHBCLKDivider) < (RCC->CFGR2 & RCC_CFGR2_HPRE))
    {
      assert_param(IS_RCC_HCLK(pClkInitStruct->AHBCLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_HPRE, pClkInitStruct->AHBCLKDivider);
    }
  }

  /* Decreasing the number of wait states because of lower CPU frequency */
  if (FLatency < __HAL_FLASH_GET_LATENCY())
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if (__HAL_FLASH_GET_LATENCY() != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    if ((pClkInitStruct->APB1CLKDivider) < (RCC->CFGR2 & RCC_CFGR2_PPRE1))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB1CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE1, pClkInitStruct->APB1CLKDivider);
    }
  }

  /*-------------------------- PCLK2 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    if ((pClkInitStruct->APB2CLKDivider) < ((RCC->CFGR2 & RCC_CFGR2_PPRE2) >> 4))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB2CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE2, ((pClkInitStruct->APB2CLKDivider) << 4));
    }
  }

  /*-------------------------- PCLK3 Configuration ---------------------------*/
  if (((pClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK3) == RCC_CLOCKTYPE_PCLK3)
  {
    if ((pClkInitStruct->APB3CLKDivider) < ((RCC->CFGR2 & RCC_CFGR2_PPRE3) >> 8))
    {
      assert_param(IS_RCC_PCLK(pClkInitStruct->APB3CLKDivider));
      MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PPRE3, ((pClkInitStruct->APB3CLKDivider) << 8));
    }
  }

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR2 & RCC_CFGR2_HPRE) >> RCC_CFGR2_HPRE_Pos];

  /* Configure the source of time base considering new system clocks settings*/
  halstatus = HAL_InitTick(uwTickPrio);

  return halstatus;
}

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions_Group2 Peripheral Control functions
  *  @brief   RCC clocks control functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to:

    (+) Output clock to MCO pin.
    (+) Retrieve current clock frequencies.
    (+) Enable the Clock Security System.

@endverbatim
  * @{
  */

/**
  * @brief  Select the clock source to output on MCO1 pin(PA8) or on MCO2 pin(PC9).
  * @note   PA8/PC9 should be configured in alternate function mode.
  * @param  RCC_MCOx  specifies the output direction for the clock source.
  *          For STM32H5xx family this parameter can have only one value:
  *            @arg @ref RCC_MCO1  Clock source to output on MCO1 pin(PA8).
  *            @arg @ref RCC_MCO2  Clock source to output on MCO2 pin(PC9).
  * @param  RCC_MCOSource  specifies the clock source to output.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCO1SOURCE_HSI: HSI clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_LSE: LSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_HSE: HSE clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_PLL1QCLK:  PLL1Q clock selected as MCO1 source
  *            @arg RCC_MCO1SOURCE_HSI48: HSI48 (48MHZ) selected as MCO1 source
  *            @arg RCC_MCO2SOURCE_SYSCLK: System clock (SYSCLK) selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLL2PCLK: PLL2P clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_HSE: HSE clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLL1PCLK:  PLL1P clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_CSI:  CSI clock selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_LSI:  LSI clock selected as MCO2 source
  * @param  RCC_MCODiv  specifies the MCO prescaler.
  *          This parameter can be one of the following values:
  *            @arg RCC_MCODIV_1 up to RCC_MCODIV_15  : divider applied to MCOx clock
  * @retval None
  */
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* Check the parameters */
  assert_param(IS_RCC_MCO(RCC_MCOx));
  assert_param(IS_RCC_MCODIV(RCC_MCODiv));
  /* RCC_MCO1 */
  if (RCC_MCOx == RCC_MCO1)
  {
    assert_param(IS_RCC_MCO1SOURCE(RCC_MCOSource));

    /* MCO1 Clock Enable */
    MCO1_CLK_ENABLE();

    /* Configure the MCO1 pin in alternate function mode */
    GPIO_InitStruct.Pin = MCO1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(MCO1_GPIO_PORT, &GPIO_InitStruct);

    /* Mask MCO1 and MCO1PRE[3:0] bits then Select MCO1 clock source and pre-scaler */
    MODIFY_REG(RCC->CFGR1, (RCC_CFGR1_MCO1SEL | RCC_CFGR1_MCO1PRE), (RCC_MCOSource | RCC_MCODiv));
  }
  else
  {
    assert_param(IS_RCC_MCO2SOURCE(RCC_MCOSource));

    /* MCO2 Clock Enable */
    MCO2_CLK_ENABLE();

    /* Configure the MCO2 pin in alternate function mode */
    GPIO_InitStruct.Pin = MCO2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(MCO2_GPIO_PORT, &GPIO_InitStruct);

    /* Mask MCO2 and MCO2PRE[3:0] bits then Select MCO2 clock source and pre-scaler */
    MODIFY_REG(RCC->CFGR1, (RCC_CFGR1_MCO2SEL | RCC_CFGR1_MCO2PRE), (RCC_MCOSource | (RCC_MCODiv << 7U)));
  }
}

/**
  * @brief  Return the SYSCLK frequency.
  *
  * @note   The system frequency computed by this function may not be the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constants of the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is CSI, function returns values based on CSI_VALUE(**)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(***)
  * @note     If SYSCLK source is PLL, function returns values based on HSI_VALUE(*), CSI_VALUE(**)
  *           or HSE_VALUE(***) multiplied/divided by the PLL factors.
  * @note     (*) HSI_VALUE is a constant defined in stm32h5xx_hal_conf.h file (default value
  *               64 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) CSI_VALUE is a constant defined in stm32h5xx_hal_conf.h file (default value
  *               4 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (***) HSE_VALUE is a constant defined in stm32h5xx_hal_conf.h file (default value
  *                24 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *
  * @note   This function can be used by the user application to compute the
  *         baudrate for the communication peripherals or configure other parameters.
  *
  * @note   Each time SYSCLK changes, this function must be called to update the
  *         right SYSCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  *
  * @retval SYSCLK frequency
  */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
  uint32_t pllsource;
  uint32_t pllp;
  uint32_t pllm;
  uint32_t pllfracen;
  uint32_t sysclockfreq;
  uint32_t hsivalue;
  float_t fracn1;
  float_t pllvco;

  if (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_CSI)
  {
    /* CSI used as system clock  source */
    sysclockfreq = CSI_VALUE;
  }
  else if (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSI)
  {
    /* HSI used as system clock source */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIDIVF) != 0U)
    {
      sysclockfreq = (uint32_t)(HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
    }
    else
    {
      sysclockfreq = (uint32_t) HSI_VALUE;
    }
  }
  else if (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_HSE)
  {
    /* HSE used as system clock source */
    sysclockfreq = HSE_VALUE;
  }

  else if (__HAL_RCC_GET_SYSCLK_SOURCE() == RCC_SYSCLKSOURCE_STATUS_PLLCLK)
  {
    /* PLL used as system clock  source */

    /* PLL_VCO = (HSE_VALUE or HSI_VALUE or CSI_VALUE/ PLLM) * PLLN
    SYSCLK = PLL_VCO / PLLR
    */
    pllsource = (RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1SRC);
    pllm = ((RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1M) >> RCC_PLL1CFGR_PLL1M_Pos);
    pllfracen = ((RCC->PLL1CFGR & RCC_PLL1CFGR_PLL1FRACEN) >> RCC_PLL1CFGR_PLL1FRACEN_Pos);
    fracn1 = (float_t)(uint32_t)(pllfracen * ((RCC->PLL1FRACR & \
                                               RCC_PLL1FRACR_PLL1FRACN) >> RCC_PLL1FRACR_PLL1FRACN_Pos));

    if (pllm != 0U)
    {
      switch (pllsource)
      {
        case RCC_PLL1_SOURCE_HSI:  /* HSI used as PLL1 clock source */

          if (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIDIVF) != 0U)
          {
            hsivalue = (HSI_VALUE >> (__HAL_RCC_GET_HSI_DIVIDER() >> RCC_CR_HSIDIV_Pos));
            pllvco = ((float_t)hsivalue / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1N) + \
                                                            (fracn1 / (float_t)0x2000) + (float_t)1);
          }
          else
          {
            pllvco = ((float_t)HSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1N) + \
                                                             (fracn1 / (float_t)0x2000) + (float_t)1);
          }

          break;

        case RCC_PLL1_SOURCE_HSE:  /* HSE used as PLL1 clock source */
          pllvco = ((float_t)HSE_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1N) + \
                                                           (fracn1 / (float_t)0x2000) + (float_t)1);

          break;

        case RCC_PLL1_SOURCE_CSI:  /* CSI used as PLL1 clock source */
        default:
          pllvco = ((float_t)CSI_VALUE / (float_t)pllm) * ((float_t)(uint32_t)(RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1N) + \
                                                           (fracn1 / (float_t)0x2000) + (float_t)1);
          break;
      }

      pllp = (((RCC->PLL1DIVR & RCC_PLL1DIVR_PLL1P) >> RCC_PLL1DIVR_PLL1P_Pos) + 1U) ;
      sysclockfreq = (uint32_t)(float_t)(pllvco / (float_t)pllp);
    }
    else
    {
      sysclockfreq = 0;
    }
  }

  else
  {
    /* HSI is the default system clock source */
    sysclockfreq = (uint32_t) HSI_VALUE;
  }

  return sysclockfreq;
}

/**
  * @brief  Return the HCLK frequency.
  * @note   Each time HCLK changes, this function must be called to update the
  *         right HCLK value. Otherwise, any configuration based on this function will be incorrect.
  *
  * @note   The SystemCoreClock CMSIS variable is used to store System Clock Frequency.
  * @retval HCLK frequency in Hz
  */
uint32_t HAL_RCC_GetHCLKFreq(void)
{

  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> (AHBPrescTable[(RCC->CFGR2 & RCC_CFGR2_HPRE) \
                                                                >> RCC_CFGR2_HPRE_Pos] & 0x1FU);

  return SystemCoreClock;
}

/**
  * @brief  Return the PCLK1 frequency.
  * @note   Each time PCLK1 changes, this function must be called to update the
  *         right PCLK1 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK1 frequency in Hz
  */
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
  /* Get HCLK source and Compute PCLK1 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> ((APBPrescTable[(RCC->CFGR2 & RCC_CFGR2_PPRE1) >> RCC_CFGR2_PPRE1_Pos]) & 0x1FU));
}

/**
  * @brief  Return the PCLK2 frequency.
  * @note   Each time PCLK2 changes, this function must be called to update the
  *         right PCLK2 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK2 frequency in Hz
  */
uint32_t HAL_RCC_GetPCLK2Freq(void)
{
  /* Get HCLK source and Compute PCLK2 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> ((APBPrescTable[(RCC->CFGR2 & RCC_CFGR2_PPRE2) >> RCC_CFGR2_PPRE2_Pos]) & 0x1FU));
}

/**
  * @brief  Return the PCLK3 frequency.
  * @note   Each time PCLK3 changes, this function must be called to update the
  *         right PCLK3 value. Otherwise, any configuration based on this function will be incorrect.
  * @retval PCLK3 frequency in Hz
  */
uint32_t HAL_RCC_GetPCLK3Freq(void)
{
  /* Get HCLK source and Compute PCLK3 frequency ---------------------------*/
  return (HAL_RCC_GetHCLKFreq() >> ((APBPrescTable[(RCC->CFGR2 & RCC_CFGR2_PPRE3) >> RCC_CFGR2_PPRE3_Pos]) & 0x1FU));
}
/**
  * @brief  Configure the pOscInitStruct according to the internal
  *         RCC configuration registers.
  * @param  pOscInitStruct  pointer to an RCC_OscInitTypeDef structure that
  *         will be configured.
  * @retval None
  */
void HAL_RCC_GetOscConfig(RCC_OscInitTypeDef  *pOscInitStruct)
{
  uint32_t regval;
  uint32_t reg1val;
  uint32_t reg2val;

  /* Check the parameters */
  assert_param(pOscInitStruct != (void *)NULL);

  /* Set all possible values for the Oscillator type parameter ---------------*/
  pOscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_CSI | \
                                   RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSI48;

  /* Get Control register */
  regval = RCC->CR;

  /* Get the HSE configuration -----------------------------------------------*/
  pOscInitStruct->HSEState = (regval & (RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_HSEEXT));

  /* Get the CSI configuration -----------------------------------------------*/
  pOscInitStruct->CSIState = regval & RCC_CR_CSION;

  /* Get the HSI configuration -----------------------------------------------*/
  pOscInitStruct->HSIState = regval & RCC_CR_HSION;
  pOscInitStruct->HSIDiv = regval & RCC_CR_HSIDIV;
  pOscInitStruct->HSICalibrationValue = (uint32_t)(READ_BIT(RCC->HSICFGR, \
                                                            RCC_HSICFGR_HSITRIM) >> RCC_HSICFGR_HSITRIM_Pos);

  /* Get BDCR register */
  regval = RCC->BDCR;

  /* Get the LSE configuration -----------------------------------------------*/
  pOscInitStruct->LSEState = (regval & (RCC_BDCR_LSEON | RCC_BDCR_LSEBYP | RCC_BDCR_LSEEXT));

  /* Get the LSI configuration -----------------------------------------------*/
  pOscInitStruct->LSIState = regval & RCC_BDCR_LSION;

  /* Get Control register */
  regval = RCC->CR;

  /* Get the HSI48 configuration ---------------------------------------------*/
  pOscInitStruct->HSI48State = regval & RCC_CR_HSI48ON;

  /* Get the PLL configuration -----------------------------------------------*/
  if ((regval & RCC_CR_PLL1ON) == RCC_CR_PLL1ON)
  {
    pOscInitStruct->PLL.PLLState = RCC_PLL_ON;
  }
  else
  {
    pOscInitStruct->PLL.PLLState = RCC_PLL_OFF;
  }

  /* Get PLL configuration register */
  reg1val = RCC->PLL1CFGR;
  reg2val = RCC->PLL1DIVR;

  pOscInitStruct->PLL.PLLSource = (uint32_t)(reg1val & RCC_PLL1CFGR_PLL1SRC);
  pOscInitStruct->PLL.PLLM = (uint32_t)((reg1val & RCC_PLL1CFGR_PLL1M) >> RCC_PLL1CFGR_PLL1M_Pos);
  pOscInitStruct->PLL.PLLN = (uint32_t)(((reg2val & RCC_PLL1DIVR_PLL1N) >> RCC_PLL1DIVR_PLL1N_Pos) + 1U);
  pOscInitStruct->PLL.PLLQ = (uint32_t)(((reg2val & RCC_PLL1DIVR_PLL1Q) >> RCC_PLL1DIVR_PLL1Q_Pos) + 1U);
  pOscInitStruct->PLL.PLLR = (uint32_t)(((reg2val & RCC_PLL1DIVR_PLL1R) >> RCC_PLL1DIVR_PLL1R_Pos) + 1U);
  pOscInitStruct->PLL.PLLP = (uint32_t)(((reg2val & RCC_PLL1DIVR_PLL1P) >> RCC_PLL1DIVR_PLL1P_Pos) + 1U);
  pOscInitStruct->PLL.PLLRGE = (uint32_t)((reg1val & RCC_PLL1CFGR_PLL1RGE));
  pOscInitStruct->PLL.PLLVCOSEL = (uint32_t)((reg1val & RCC_PLL1CFGR_PLL1VCOSEL) >> RCC_PLL1CFGR_PLL1VCOSEL_Pos);
  pOscInitStruct->PLL.PLLFRACN = (uint32_t)(((RCC->PLL1FRACR & RCC_PLL1FRACR_PLL1FRACN) \
                                             >> RCC_PLL1FRACR_PLL1FRACN_Pos));
}

/**
  * @brief  Configure the pClkInitStruct according to the internal
  *         RCC configuration registers.
  * @param  pClkInitStruct  pointer to an RCC_ClkInitTypeDef structure that
  *         will be configured.
  * @param  pFLatency  Pointer on the Flash Latency.
  * @retval None
  */
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef  *pClkInitStruct, uint32_t *pFLatency)
{
  uint32_t regval;

  /* Check the parameters */
  assert_param(pClkInitStruct != (void *)NULL);
  assert_param(pFLatency != (void *)NULL);

  /* Set all possible values for the Clock type parameter --------------------*/
  pClkInitStruct->ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | \
                              RCC_CLOCKTYPE_PCLK3;

  /* Get the SYSCLK configuration --------------------------------------------*/
  pClkInitStruct->SYSCLKSource = (uint32_t)(RCC->CFGR1 & RCC_CFGR1_SW);

  /* Get the HCLK configuration ----------------------------------------------*/
  regval = RCC->CFGR2;
  pClkInitStruct->AHBCLKDivider = (uint32_t)(regval & RCC_CFGR2_HPRE);

  /* Get the APB1 configuration ----------------------------------------------*/
  pClkInitStruct->APB1CLKDivider = (uint32_t)(regval & RCC_CFGR2_PPRE1);

  /* Get the APB2 configuration ----------------------------------------------*/
  pClkInitStruct->APB2CLKDivider = (uint32_t)((regval & RCC_CFGR2_PPRE2) >> 4);

  /* Get the APB3 configuration ----------------------------------------------*/
  pClkInitStruct->APB3CLKDivider = (uint32_t)((regval & RCC_CFGR2_PPRE3) >> 8);

  /* Get the Flash Wait State (Latency) configuration ------------------------*/
  *pFLatency = (uint32_t)(FLASH->ACR & FLASH_ACR_LATENCY);
}

/**
  * @brief  Get and clear reset flags
  * @note   Once reset flags are retrieved, this API is clearing them in order
  *         to isolate next reset reason.
  * @retval can be a combination of @ref RCC_Reset_Flag
  */
uint32_t HAL_RCC_GetResetSource(void)
{
  uint32_t reset;

  /* Get all reset flags */
  reset = RCC->RSR & RCC_RESET_FLAG_ALL;

  /* Clear Reset flags */
  RCC->RSR |= RCC_RSR_RMVF;

  return reset;
}

/**
  * @brief  Enable the HSE Clock Security System.
  * @note   If a failure is detected on the HSE oscillator clock, this oscillator
  *         is automatically disabled and an interrupt is generated to inform the
  *         software about the failure (Clock Security System Interrupt, CSSI),
  *         allowing the MCU to perform rescue operations. The CSSI is linked to
  *         the Cortex-M NMI (Non-Maskable Interrupt) exception vector.
  * @note   The Clock Security System can only be cleared by reset.
  * @retval None
  */
void HAL_RCC_EnableCSS(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSECSSON);
}

/**
  * @brief Handle the RCC Clock Security System interrupt request.
  * @note This API should be called under the NMI_Handler().
  * @retval None
  */
void HAL_RCC_NMI_IRQHandler(void)
{
  /* Check RCC CSSF interrupt flag  */
  if (__HAL_RCC_GET_IT(RCC_IT_HSECSS))
  {
    /* Clear RCC CSS pending bit */
    __HAL_RCC_CLEAR_IT(RCC_IT_HSECSS);

    /* RCC Clock Security System interrupt user callback */
    HAL_RCC_CSSCallback();
  }
}

/**
  * @brief  RCC HSE Clock Security System interrupt callback.
  * @retval none
  */
__weak void HAL_RCC_CSSCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_RCC_CSSCallback should be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup RCC_Exported_Functions_Group3 Attributes management functions
  *  @brief Attributes management functions.
  *
@verbatim
 ===============================================================================
                       ##### RCC attributes functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to:

    (+) Configure the RCC item(s) attributes.
    (+) Get the attribute of an RCC item.

@endverbatim
  * @{
  */
/**
  * @brief  Configure the RCC item(s) attribute(s).
  * @note   Available attributes are to secure items and set RCC as privileged (*).
  *         Default state is non-secure and unprivileged access allowed.
  * @note   Secure and non-secure attributes can only be set from the secure
  *         state when the system implements the security (TZEN=1).
  * @note   As the privileged attributes concern either all secure or all non-secure
  *         RCC resources accesses and not each RCC individual items access attribute,
  *         the application must ensure that the privilege access attribute configurations
  *         are coherent amongst the security level set on RCC individual items
  *         so not to overwrite a previous more restricted access rule (consider either
  *         all secure and/or all non-secure RCC resources accesses by privileged-only
  *         transactions or privileged and unprivileged transactions).
  * @param  Item Item(s) to set attributes on.
  *         This parameter can be a one or a combination of @ref RCC_items (**).
  * @param  Attributes specifies the RCC secure/privilege attributes.
  *         This parameter can be a value of  @ref RCC_attributes
  * @retval None
  *
  * (*)   : For stm32h503xx devices, attributes specifies the privilege attribute only (no items).
  * (**)  : For stm32h503xx devices, this parameter is unused, it can take 0 or any other numerical value.
  */
void HAL_RCC_ConfigAttributes(uint32_t Item, uint32_t Attributes)
{

  /* Check the parameters */
  assert_param(IS_RCC_ATTRIBUTES(Attributes));

#if defined(RCC_SECCFGR_HSISEC)
  assert_param(IS_RCC_ITEM_ATTRIBUTES(Item));

  switch (Attributes)
  {
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    /* Secure Privilege attribute */
    case RCC_SEC_PRIV:
      SET_BIT(RCC->SECCFGR, Item);
      SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_SPRIV);
      break;
    /* Secure Non-Privilege attribute */
    case RCC_SEC_NPRIV:
      SET_BIT(RCC->SECCFGR, Item);
      CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_SPRIV);
      break;
    /* Non-secure Privilege attribute */
    case RCC_NSEC_PRIV:
      CLEAR_BIT(RCC->SECCFGR, Item);
      SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
      break;
    /* Non-secure Non-Privilege attribute */
    case RCC_NSEC_NPRIV:
      CLEAR_BIT(RCC->SECCFGR, Item);
      CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
      break;
#else /* __ARM_FEATURE_CMSE */
    /* Non-secure Privilege attribute */
    case RCC_NSEC_PRIV:
      SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
      break;
    /* Non-secure Non-Privilege attribute */
    case RCC_NSEC_NPRIV:
      CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_NSPRIV);
      break;
#endif /* __ARM_FEATURE_CMSE */
    default:
      /* Nothing to do */
      break;
  }

#else /* RCC_SECCFGR_HSISEC */

  UNUSED(Item);

  switch (Attributes)
  {
    /* Privilege attribute */
    case RCC_PRIV:
      SET_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_PRIV);
      break;
    /* Non-secure Non-Privilege attribute */
    case RCC_NPRIV:
      CLEAR_BIT(RCC->PRIVCFGR, RCC_PRIVCFGR_PRIV);
      break;
    default:
      /* Nothing to do */
      break;
  }

#endif /* RCC_SECCFGR_HSISEC */
}

/**
  * @brief  Get the attribute of an RCC item.
  * @note   Secure and non-secure attributes are only available from secure state
  *         when the system implements the security (TZEN=1)
  * @param  Item Single item to get secure/non-secure and privilege/non-privilege attribute from.
  *         This parameter can be a one value of @ref RCC_items except RCC_ALL. (*)
  * @param  pAttributes pointer to return the attributes.
  * @retval HAL Status.
  *
  * (*)  : This parameter is unused for stm32h503xx devices, it can take 0 or any other numerical value.
  */
HAL_StatusTypeDef HAL_RCC_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes)
{
  uint32_t attributes;

  /* Check null pointer */
  if (pAttributes == NULL)
  {
    return HAL_ERROR;
  }

#if defined(RCC_SECCFGR_HSISEC)
  /* Check the parameters */
  assert_param(IS_RCC_SINGLE_ITEM_ATTRIBUTES(Item));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

  /* Check item security */
  if ((RCC->SECCFGR & Item) == Item)
  {
    /* Get Secure privileges attribute */
    attributes = ((RCC->PRIVCFGR & RCC_PRIVCFGR_SPRIV) == 0U) ? RCC_SEC_NPRIV : RCC_SEC_PRIV;
  }
  else
  {
    /* Get Non-Secure privileges attribute */
    attributes = ((RCC->PRIVCFGR & RCC_PRIVCFGR_NSPRIV) == 0U) ? RCC_NSEC_NPRIV : RCC_NSEC_PRIV;
  }
#else  /* __ARM_FEATURE_CMSE */
  attributes = ((RCC->PRIVCFGR & RCC_PRIVCFGR_NSPRIV) == 0U) ? RCC_NSEC_NPRIV : RCC_NSEC_PRIV;
#endif /* __ARM_FEATURE_CMSE */

#else /* RCC_SECCFGR_HSISEC */
  UNUSED(Item);
  /* Get privileges attribute */
  attributes = ((RCC->PRIVCFGR & RCC_PRIVCFGR_PRIV) == 0U) ? RCC_NPRIV : RCC_PRIV;
#endif /* RCC_SECCFGR_HSISEC */

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

/* Private function prototypes -----------------------------------------------*/
#endif /* HAL_RCC_MODULE_ENABLED */
/**
  * @}
  */

/**
  * @}
  */

