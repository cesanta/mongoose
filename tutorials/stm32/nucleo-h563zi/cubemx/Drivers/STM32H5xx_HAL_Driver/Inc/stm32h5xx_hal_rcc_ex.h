/**
  ******************************************************************************
  * @file    stm32h5xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL Extended  module.
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
#ifndef __STM32H5xx_HAL_RCC_EX_H
#define __STM32H5xx_HAL_RCC_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup RCCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Types RCCEx Exported Types
  * @{
  */

/**
  * @brief  PLL2 Clock structure definition
  */
typedef struct
{
  uint32_t PLL2Source;  /*!< RCC_PLL2Source: PLL2 entry clock source.
                            This parameter must be a value of @ref RCC_PLL2_Clock_Source               */

  uint32_t PLL2M;       /*!< PLL2M: Division factor for PLL2 VCO input clock.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 63    */

  uint32_t PLL2N;       /*!< PLL2N: Multiplication factor for PLL2 VCO output clock.
                             This parameter must be a number between Min_Data = 4 and Max_Data = 512   */

  uint32_t PLL2P;       /*!< PLL2P: Division factor for peripheral clock.
                             This parameter must be a number between Min_Data = 2 and Max_Data = 128   */

  uint32_t PLL2Q;        /*!< PLL2Q: Division factor for peripheral clocks.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 128   */

  uint32_t PLL2R;        /*!< PLL2R: Division factor for peripheral clocks.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 128
                             odd division factors are not allowed                                      */

  uint32_t PLL2RGE;      /*!<PLL2RGE: PLL2 clock Input range
                          This parameter must be a value of @ref RCC_PLL2_VCI_Range                    */

  uint32_t PLL2VCOSEL;   /*!<PLL2VCOSEL: PLL2 clock Output range
                          This parameter must be a value of @ref RCC_PLL2_VCO_Range                    */

  uint32_t PLL2FRACN;    /*!<PLL2FRACN: Specifies Fractional Part Of The Multiplication Factor for
                            PLL2 VCO It should be a value between 0 and 8191                           */

  uint32_t PLL2ClockOut; /*!< PLL2ClockOut: specifies PLL2 output clock to be enabled.
                                  This parameter must be a value or a combination of @ref RCC_PLL2_Clock_Output */
} RCC_PLL2InitTypeDef;


#if defined(RCC_CR_PLL3ON)
/**
  * @brief  PLL3 Clock structure definition
  */
typedef struct
{
  uint32_t PLL3Source;  /*!< RCC_PLL3Source: PLL3 entry clock source.
                            This parameter must be a value of @ref RCC_PLL3_Clock_Source               */

  uint32_t PLL3M;       /*!< PLL3M: Division factor for PLL3 VCO input clock.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 63    */

  uint32_t PLL3N;       /*!< PLL3N: Multiplication factor for PLL3 VCO output clock.
                             This parameter must be a number between Min_Data = 4 and Max_Data = 512   */

  uint32_t PLL3P;       /*!< PLL3P: Division factor for system clock.
                             This parameter must be a number between Min_Data = 2 and Max_Data = 128   */

  uint32_t PLL3Q;        /*!< PLL3Q: Division factor for peripheral clocks.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 128   */

  uint32_t PLL3R;        /*!< PLL3R: Division factor for peripheral clocks.
                             This parameter must be a number between Min_Data = 1 and Max_Data = 128
                             odd division factors are not allowed                                      */

  uint32_t PLL3RGE;      /*!<PLL3RGE: PLL3 clock Input range
                          This parameter must be a value of @ref RCC_PLL3_VCI_Range                    */

  uint32_t PLL3VCOSEL;   /*!<PLL3VCOSEL: PLL3 clock Output range
                          This parameter must be a value of @ref RCC_PLL3_VCO_Range                    */

  uint32_t PLL3FRACN;    /*!<PLL3FRACN: Specifies Fractional Part Of The Multiplication Factor for
                            PLL3 VCO It should be a value between 0 and 8191                           */

  uint32_t PLL3ClockOut; /*!< PLL3ClockOut: specifies PLL3 output clock to be enabled.
                                  This parameter must be a value or a combination of @ref RCC_PLL3_Clock_Output */
} RCC_PLL3InitTypeDef;
#endif /* RCC_CR_PLL3ON */

/**
  * @brief  RCC PLL1 Clocks structure definition
  */
typedef struct
{
  uint32_t PLL1_P_Frequency;
  uint32_t PLL1_Q_Frequency;
  uint32_t PLL1_R_Frequency;
} PLL1_ClocksTypeDef;

/**
  * @brief  RCC PLL2 Clocks structure definition
  */
typedef struct
{
  uint32_t PLL2_P_Frequency;
  uint32_t PLL2_Q_Frequency;
  uint32_t PLL2_R_Frequency;
} PLL2_ClocksTypeDef;

#if defined(RCC_CR_PLL3ON)
/**
  * @brief  RCC PLL3 Clocks structure definition
  */
typedef struct
{
  uint32_t PLL3_P_Frequency;
  uint32_t PLL3_Q_Frequency;
  uint32_t PLL3_R_Frequency;
} PLL3_ClocksTypeDef;
#endif /* RCC_CR_PLL3ON */


/**
  * @brief  RCC extended clocks structure definition
  */
typedef struct
{
  uint64_t PeriphClockSelection;   /*!< The Extended Clock to be configured.
                                        This parameter can be a value of @ref RCCEx_Periph_Clock_Selection */

  RCC_PLL2InitTypeDef PLL2;        /*!< PLL2structure parameters.
                                        This parameter will be used only when PLL2 is selected as kernel clock Source
                                        for some peripherals */

#if defined(RCC_CR_PLL3ON)
  RCC_PLL3InitTypeDef PLL3;        /*!< PLL3 structure parameters.
                                        This parameter will be used only when PLL2 is selected as kernel clock Source
                                        for some peripherals */
#endif /* RCC_CR_PLL3ON */

  uint32_t CkperClockSelection;    /*!< Specifies CKPER clock source
                                        This parameter can be a value of @ref RCCEx_CLKP_Clock_Source   */

  uint32_t Usart1ClockSelection;   /*!< Specifies USART1 clock source.
                                        This parameter can be a value of @ref RCCEx_USART1_Clock_Source */

  uint32_t Usart2ClockSelection;   /*!< Specifies USART2 clock source.
                                        This parameter can be a value of @ref RCCEx_USART2_Clock_Source */

  uint32_t Usart3ClockSelection;   /*!< Specifies USART3 clock source.
                                        This parameter can be a value of @ref RCCEx_USART3_Clock_Source */

#if defined(UART4)
  uint32_t Uart4ClockSelection;    /*!< Specifies UART4 clock source.
                                        This parameter can be a value of @ref RCCEx_UART4_Clock_Source */
#endif /* UART4 */

#if defined(UART5)
  uint32_t Uart5ClockSelection;    /*!< Specifies UART5 clock source.
                                        This parameter can be a value of @ref RCCEx_UART5_Clock_Source */
#endif /* UART5 */

#if defined(USART6)
  uint32_t Usart6ClockSelection;   /*!< Specifies USART6 clock source.
                                        This parameter can be a value of @ref RCCEx_USART6_Clock_Source */
#endif /* USART6 */

#if defined(UART7)
  uint32_t Uart7ClockSelection;    /*!< Specifies UART7 clock source.
                                        This parameter can be a value of @ref RCCEx_UART7_Clock_Source */
#endif /* UART7 */

#if defined(UART8)
  uint32_t Uart8ClockSelection;    /*!< Specifies UART8 clock source.
                                        This parameter can be a value of @ref RCCEx_UART8_Clock_Source */
#endif /* UART8 */

#if defined(UART9)
  uint32_t Uart9ClockSelection;    /*!< Specifies UART9 clock source.
                                        This parameter can be a value of @ref RCCEx_UART9_Clock_Source */
#endif /* UART9 */

#if defined(USART10)
  uint32_t Usart10ClockSelection;   /*!< Specifies USART10 clock source.
                                        This parameter can be a value of @ref RCCEx_USART10_Clock_Source */
#endif /* USART10 */

#if defined(USART11)
  uint32_t Usart11ClockSelection;   /*!< Specifies USART11 clock source.
                                        This parameter can be a value of @ref RCCEx_USART11_Clock_Source */
#endif /* USART11 */

#if defined(UART12)
  uint32_t Uart12ClockSelection;    /*!< Specifies UART12 clock source.
                                        This parameter can be a value of @ref RCCEx_UART12_Clock_Source */
#endif /* UART12 */

  uint32_t Lpuart1ClockSelection;  /*!< Specifies LPUART1 clock source.
                                        This parameter can be a value of @ref RCCEx_LPUART1_Clock_Source */

  uint32_t I2c1ClockSelection;     /*!< Specifies I2C1 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C1_Clock_Source */

  uint32_t I2c2ClockSelection;     /*!< Specifies I2C2 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C2_Clock_Source */

#if defined(I2C3)
  uint32_t I2c3ClockSelection;     /*!< Specifies I2C3 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C3_Clock_Source */
#endif /* I2C3 */

#if defined(I2C4)
  uint32_t I2c4ClockSelection;     /*!< Specifies I2C4 clock source.
                                        This parameter can be a value of @ref RCCEx_I2C4_Clock_Source */
#endif /* I2C4 */

  uint32_t I3c1ClockSelection;     /*!< Specifies I3C1 clock source.
                                        This parameter can be a value of @ref RCCEx_I3C1_Clock_Source */

#if defined(I3C2)
  uint32_t I3c2ClockSelection;     /*!< Specifies I3C2 clock source.
                                        This parameter can be a value of @ref RCCEx_I3C2_Clock_Source */
#endif /* I3C2*/

  uint32_t Lptim1ClockSelection;   /*!< Specifies LPTIM1 clock source.
                                     This parameter can be a value of @ref RCCEx_LPTIM1_Clock_Source */

  uint32_t Lptim2ClockSelection;   /*!< Specifies LPTIM2 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM2_Clock_Source */

#if defined(LPTIM3)
  uint32_t Lptim3ClockSelection;   /*!< Specifies LPTIM3 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM3_Clock_Source */
#endif /* LPTIM3 */

#if defined(LPTIM4)
  uint32_t Lptim4ClockSelection;   /*!< Specifies LPTIM4 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM4_Clock_Source */
#endif /* LPTIM4 */

#if defined(LPTIM5)
  uint32_t Lptim5ClockSelection;   /*!< Specifies LPTIM5 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM5_Clock_Source */
#endif /* LPTIM5 */

#if defined(LPTIM6)
  uint32_t Lptim6ClockSelection;   /*!< Specifies LPTIM6 clock source.
                                        This parameter can be a value of @ref RCCEx_LPTIM6_Clock_Source */
#endif /* LPTIM6 */

  uint32_t FdcanClockSelection;    /*!< Specifies FDCAN kernel clock source.
                                        This parameter can be a value of @ref RCCEx_FDCAN_Clock_Source */
#if defined(SAI1)
  uint32_t Sai1ClockSelection;     /*!< Specifies SAI1 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI1_Clock_Source */
#endif /* SAI1*/

#if defined(SAI2)
  uint32_t Sai2ClockSelection;     /*!< Specifies SAI2 clock source.
                                        This parameter can be a value of @ref RCCEx_SAI2_Clock_Source */
#endif /* SAI2*/

  uint32_t RngClockSelection;      /*!< Specifies RNG clock source.
                                        This parameter can be a value of @ref RCCEx_RNG_Clock_Source  */

#if defined(SDMMC1)
  uint32_t Sdmmc1ClockSelection;    /*!< Specifies SDMMC1 clock source.
                                       This parameter can be a value of @ref RCCEx_SDMMC1_Clock_Source */
#endif /* SDMMC1 */

#if defined(SDMMC2)
  uint32_t Sdmmc2ClockSelection;    /*!< Specifies SDMMC2 clock source.
                                       This parameter can be a value of @ref RCCEx_SDMMC2_Clock_Source */
#endif /* SDMMC2 */

  uint32_t AdcDacClockSelection;   /*!< Specifies ADC1, ADC2 and DAC interface clock source.
                                        This parameter can be a value of @ref RCCEx_ADCDAC_Clock_Source */

  uint32_t DacLowPowerClockSelection;  /*!< Specifies DAC low-power clock source used in Sample and hold mode.
                                            This parameter can be a value of @ref RCCEx_DAC_Low_Power_Clock_Source */

#if defined(OCTOSPI1)
  uint32_t OspiClockSelection;     /*!< Specifies OctoSPI clock source.
                                        This parameter can be a value of @ref RCCEx_OSPI_Clock_Source */
#endif /* OCTOSPI1 */

  uint32_t Spi1ClockSelection;     /*!< Specifies SPI1 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI1_Clock_Source    */

  uint32_t Spi2ClockSelection;     /*!< Specifies SPI2 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI2_Clock_Source    */

  uint32_t Spi3ClockSelection;     /*!< Specifies SPI3 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI3_Clock_Source    */

#if defined(SPI4)
  uint32_t Spi4ClockSelection;     /*!< Specifies SPI4 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI4_Clock_Source    */
#endif /* SPI4 */

#if defined(SPI5)
  uint32_t Spi5ClockSelection;     /*!< Specifies SPI5 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI5_Clock_Source    */
#endif /* SPI5 */

#if defined(SPI6)
  uint32_t Spi6ClockSelection;     /*!< Specifies SPI6 clock source.
                                        This parameter can be a value of @ref RCCEx_SPI6_Clock_Source    */
#endif /* SPI6 */

  uint32_t RTCClockSelection;      /*!< Specifies RTC clock source.
                                        This parameter can be a value of @ref RCC_RTC_Clock_Source */

#if defined(CEC)
  uint32_t CecClockSelection;      /*!< Specifies CEC clock source.
                                        This parameter can be a value of @ref RCCEx_CEC_Clock_Source */
#endif /* CEC */

#if defined(USB_DRD_FS)
  uint32_t UsbClockSelection;      /*!< Specifies USB clock source.
                                        This parameter can be a value of @ref RCCEx_USB_Clock_Source */
#endif /* USB_DRD_FS */

  uint32_t TimPresSelection;       /*!< Specifies TIM Clock Prescalers Selection.
                                       This parameter can be a value of @ref RCCEx_TIM_Prescaler_Selection */

#if defined(PLAY1)
  uint32_t PLAY1ClockSelection;      /*!< Specifies PLAY1 clock source.
                                        This parameter can be a value of @ref RCCEx_PLAY1_Clock_Source */
  uint32_t PLAY1ClockDivider;        /*!< Specifies PLAY1 clock divider.
                                        This parameter can be a value of @ref RCCEx_PLAY1_Clock_Divider */
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
  uint32_t OtgfsClockSelection;      /*!< Specifies USB_OTG_FS clock source.
                                        This parameter can be a value of @ref RCCEx_OTGFS_Clock_Source */
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
  uint32_t OtghsClockSelection;       /*!< Specifies USB_OTG_HS clock source.
                                        This parameter can be a value of @ref RCCEx_OTGHS_Clock_Source */
#endif /* USB_OTG_HS */

#if defined(OCTOSPI2)
  uint32_t Ospi2ClockSelection;       /*!< Specifies OCTOSPI2 clock source.
                                        This parameter can be a value of @ref RCCEx_OSPI2_Clock_Source */
#endif /* OCTOSPI2 */

#if defined(LTDC)
  uint32_t LtdcClockSelection;       /*!< Specifies LTDC clock source.
                                        This parameter can be a value of @ref RCCEx_LTDC_Clock_Source */
#endif /* LTDC */

#if defined(ADF1)
  uint32_t Adf1ClockSelection;       /*!< Specifies ADF1 clock source.
                                        This parameter can be a value of @ref RCCEx_ADF1_Clock_Source */
#endif /* ADF1 */

#if defined(MDF1)
  uint32_t Mdf1ClockSelection;       /*!< Specifies MDF1 clock source.
                                        This parameter can be a value of @ref RCCEx_MDF1_Clock_Source */
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
  uint32_t EthClockSelection;       /*!< Specifies ETH clock source.
                                        This parameter can be a value of @ref RCCEx_ETH_Clock_Source */
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined (RCC_CCIPR4_OTGPHYREFCKSEL)
  uint32_t OtgPhyClockSelection;      /*!< Specifies OTGHS PHY clock source.
                                        This parameter can be a value of @ref RCCEx_OTGPHYREFCK_Clock_Source */
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
  uint32_t EthptpClockSelection;       /*!< Specifies ETHPTP clock source.
                                        This parameter can be a value of @ref RCCEx_ETHPTP_Clock_Source */
  uint32_t EthptpClockDivider;        /*!< Specifies ETHPTP clock divider.
                                        This parameter can be a value of @ref RCCEx_ETHPTP_Clock_Divider */
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
  uint32_t EthrefClockSelection;       /*!< Specifies ETHREF clock source.
                                        This parameter can be a value of @ref RCCEx_ETHREF_Clock_Source */
#endif /* RCC_CCIPR5_ETHREFCLKSEL */
} RCC_PeriphCLKInitTypeDef;

#if defined(CRS)

/**
  * @brief RCC_CRS Init structure definition
  */
typedef struct
{
  uint32_t Prescaler;             /*!< Specifies the division factor of the SYNC signal.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroDivider  */

  uint32_t Source;                /*!< Specifies the SYNC signal source.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroSource   */

  uint32_t Polarity;              /*!< Specifies the input polarity for the SYNC signal source.
                                     This parameter can be a value of @ref RCCEx_CRS_SynchroPolarity */

  uint32_t ReloadValue;           /*!< Specifies the value to be loaded in the frequency error counter with each SYNC
                                     event. It can be calculated in using macro
                                     __HAL_RCC_CRS_RELOADVALUE_CALCULATE(__FTARGET__, __FSYNC__)
                                     This parameter must be a number between 0 and 0xFFFF or a value of
                                     @ref RCCEx_CRS_ReloadValueDefault .*/

  uint32_t ErrorLimitValue;       /*!< Specifies the value to be used to evaluate the captured frequency error value.
                                     This parameter must be a number between 0 and 0xFF or a value of
                                     @ref RCCEx_CRS_ErrorLimitDefault */

  uint32_t HSI48CalibrationValue; /*!< Specifies a user-programmable trimming value to the HSI48 oscillator.
                                     This parameter must be a number between 0 and 0x7F or a value of
                                     @ref RCCEx_CRS_HSI48CalibrationDefault */

} RCC_CRSInitTypeDef;

/**
  * @brief RCC_CRS Synchronization structure definition
  */
typedef struct
{
  uint32_t ReloadValue;           /*!< Specifies the value loaded in the Counter reload value.
                                     This parameter must be a number between 0 and 0xFFFF */

  uint32_t HSI48CalibrationValue; /*!< Specifies value loaded in HSI48 oscillator smooth trimming.
                                     This parameter must be a number between 0 and 0x3F */

  uint32_t FreqErrorCapture;      /*!< Specifies the value loaded in the .FECAP, the frequency error counter
                                     value latched in the time of the last SYNC event.
                                     This parameter must be a number between 0 and 0xFFFF */

  uint32_t FreqErrorDirection;    /*!< Specifies the value loaded in the .FEDIR, the counting direction of the
                                     frequency error counter latched in the time of the last SYNC event.
                                     It shows whether the actual frequency is below or above the target.
                                     This parameter must be a value of @ref RCCEx_CRS_FreqErrorDirection */

} RCC_CRSSynchroInfoTypeDef;

#endif /* CRS */
/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Constants RCCEx Exported Constants
  * @{
  */

/** @defgroup RCCEx_LSCO_Clock_Source Low Speed Microcontroller Clock Output Source
  * @{
  */
#define RCC_LSCOSOURCE_LSI             0x00000000U            /*!< LSI selection for low speed microcontroller clock output */
#define RCC_LSCOSOURCE_LSE             RCC_BDCR_LSCOSEL       /*!< LSE selection for low speed microcontroller clock output */
/**
  * @}
  */

#if defined(PWR_USBSCR_OTGHSEN)
/** @defgroup OTGHS_CLK48_VALUE OTG_HS output clock
  * @{
  */
#define OTGHS_CLK48_VALUE        48000000U    /*!< Value of the OTGHS_CLK48 in Hz*/
/**
  * @}
  */
#endif /* PWR_USBSCR_OTGHSEN */

/** @defgroup RCCEx_Periph_Clock_Selection  RCCEx Periph Clock Selection
  * @{
  */
#define RCC_PERIPHCLK_USART1           ((uint64_t)0x00000001U)
#define RCC_PERIPHCLK_USART2           ((uint64_t)0x00000002U)
#define RCC_PERIPHCLK_USART3           ((uint64_t)0x00000004U)
#if defined(UART4)
#define RCC_PERIPHCLK_UART4            ((uint64_t)0x00000008U)
#endif /* UART4 */
#if defined(UART5)
#define RCC_PERIPHCLK_UART5            ((uint64_t)0x00000010U)
#endif /* UART5 */
#if defined(USART6)
#define RCC_PERIPHCLK_USART6           ((uint64_t)0x00000020U)
#endif /* USART6 */
#if defined(UART7)
#define RCC_PERIPHCLK_UART7            ((uint64_t)0x00000040U)
#endif /* UART7 */
#if defined(UART8)
#define RCC_PERIPHCLK_UART8            ((uint64_t)0x00000080U)
#endif /* UART8 */
#if defined(UART9)
#define RCC_PERIPHCLK_UART9            ((uint64_t)0x00000100U)
#endif /* UART9 */
#if defined(USART10)
#define RCC_PERIPHCLK_USART10          ((uint64_t)0x00000200U)
#endif /* USART10 */
#if defined(USART11)
#define RCC_PERIPHCLK_USART11          ((uint64_t)0x00000400U)
#endif /* USART11 */
#if defined(UART12)
#define RCC_PERIPHCLK_UART12           ((uint64_t)0x00000800U)
#endif /* UART12 */
#define RCC_PERIPHCLK_LPUART1          ((uint64_t)0x00001000U)
#define RCC_PERIPHCLK_I2C1             ((uint64_t)0x00002000U)
#define RCC_PERIPHCLK_I2C2             ((uint64_t)0x00004000U)
#if defined(I2C3)
#define RCC_PERIPHCLK_I2C3             ((uint64_t)0x00008000U)
#endif /* I2C3 */
#define RCC_PERIPHCLK_I3C1             ((uint64_t)0x00010000U)
#define RCC_PERIPHCLK_LPTIM1           ((uint64_t)0x00020000U)
#define RCC_PERIPHCLK_LPTIM2           ((uint64_t)0x00040000U)
#if defined(SAI1)
#define RCC_PERIPHCLK_SAI1             ((uint64_t)0x00080000U)
#endif /* SAI1 */
#if defined(SAI2)
#define RCC_PERIPHCLK_SAI2             ((uint64_t)0x00100000U)
#endif /* SAI2 */
#define RCC_PERIPHCLK_ADCDAC           ((uint64_t)0x00200000U)
#define RCC_PERIPHCLK_ADC               RCC_PERIPHCLK_ADCDAC
#define RCC_PERIPHCLK_DAC               RCC_PERIPHCLK_ADCDAC
#if defined(SDMMC1)
#define RCC_PERIPHCLK_SDMMC1           ((uint64_t)0x00400000U)
#endif /* SDMMC1 */
#if defined(SDMMC2)
#define RCC_PERIPHCLK_SDMMC2           ((uint64_t)0x00800000U)
#endif /* SDMMC2 */
#define RCC_PERIPHCLK_CKPER            ((uint64_t)0x01000000U)
#define RCC_PERIPHCLK_RTC              ((uint64_t)0x02000000U)
#define RCC_PERIPHCLK_RNG              ((uint64_t)0x04000000U)
#define RCC_PERIPHCLK_SPI1             ((uint64_t)0x08000000U)
#define RCC_PERIPHCLK_SPI2             ((uint64_t)0x10000000U)
#define RCC_PERIPHCLK_SPI3             ((uint64_t)0x20000000U)
#if defined(SPI4)
#define RCC_PERIPHCLK_SPI4             ((uint64_t)0x40000000U)
#endif /* SPI4 */
#if defined(SPI5)
#define RCC_PERIPHCLK_SPI5             ((uint64_t)0x80000000U)
#endif /* SPI5 */
#if defined(SPI6)
#define RCC_PERIPHCLK_SPI6             ((uint64_t)0x100000000U)
#endif /* SPI6 */
#if defined(OCTOSPI1)
#define RCC_PERIPHCLK_OSPI             ((uint64_t)0x200000000U)
#endif /* OCTOSPI1 */
#define RCC_PERIPHCLK_FDCAN            ((uint64_t)0x400000000U)
#if defined(CEC)
#define RCC_PERIPHCLK_CEC              ((uint64_t)0x800000000U)
#endif /* CEC */
#if defined(USB_DRD_FS)
#define RCC_PERIPHCLK_USB              ((uint64_t)0x1000000000U)
#endif /* USB_DRD_FS */
#if defined(LPTIM3)
#define RCC_PERIPHCLK_LPTIM3           ((uint64_t)0x2000000000U)
#endif /* LPTIM3 */
#if defined(LPTIM4)
#define RCC_PERIPHCLK_LPTIM4           ((uint64_t)0x4000000000U)
#endif /* LPTIM4 */
#if defined(LPTIM5)
#define RCC_PERIPHCLK_LPTIM5           ((uint64_t)0x8000000000U)
#endif /* LPTIM5 */
#if defined(LPTIM6)
#define RCC_PERIPHCLK_LPTIM6           ((uint64_t)0x10000000000U)
#endif /* LPTIM6 */
#define RCC_PERIPHCLK_DAC_LP           ((uint64_t)0x20000000000U)
#if defined(I2C4)
#define RCC_PERIPHCLK_I2C4             ((uint64_t)0x40000000000U)
#endif /* I2C4 */
#define RCC_PERIPHCLK_TIM              ((uint64_t)0x80000000000U)
#if defined(I3C2)
#define RCC_PERIPHCLK_I3C2             ((uint64_t)0x100000000000U)
#endif /* I3C2 */
#if defined(PLAY1)
#define RCC_PERIPHCLK_PLAY1            ((uint64_t)0x200000000000U)
#endif /* PLAY1 */
#if defined(USB_OTG_FS)
#define RCC_PERIPHCLK_OTGFS            ((uint64_t)0x400000000000U)
#endif /* USB_OTG_FS */
#if defined(USB_OTG_HS)
#define RCC_PERIPHCLK_OTGHS            ((uint64_t)0x800000000000U)
#endif /* USB_OTG_HS */
#if defined(OCTOSPI2)
#define RCC_PERIPHCLK_OSPI2            ((uint64_t)0x1000000000000U)
#endif /* OCTOSPI2 */
#if defined(LTDC)
#define RCC_PERIPHCLK_LTDC             ((uint64_t)0x2000000000000U)
#endif /* LTDC */
#if defined(ADF1)
#define RCC_PERIPHCLK_ADF1             ((uint64_t)0x8000000000000U)
#endif /* ADF1 */
#if defined(MDF1)
#define RCC_PERIPHCLK_MDF1             ((uint64_t)0x10000000000000U)
#endif /* MDF1 */
#if defined(RCC_CCIPR4_ETHCLKSEL)
#define RCC_PERIPHCLK_ETH              ((uint64_t)0x20000000000000U)
#endif /* RCC_CCIPR4_ETHCLKSEL */
#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
#define RCC_PERIPHCLK_OTGPHY           ((uint64_t)0x40000000000000U)
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */
#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
#define RCC_PERIPHCLK_ETHPTP           ((uint64_t)0x80000000000000U)
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */
#if defined(RCC_CCIPR5_ETHREFCLKSEL)
#define RCC_PERIPHCLK_ETHREF           ((uint64_t)0x200000000000000U)
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

/**
  * @}
  */


/** @defgroup RCC_PLL2_Clock_Output  RCC PLL2 Clock Output
  * @{
  */
#define RCC_PLL2_DIVP                RCC_PLL2CFGR_PLL2PEN
#define RCC_PLL2_DIVQ                RCC_PLL2CFGR_PLL2QEN
#define RCC_PLL2_DIVR                RCC_PLL2CFGR_PLL2REN
/**
  * @}
  */

#if defined(RCC_CR_PLL3ON)
/** @defgroup RCC_PLL3_Clock_Output  RCC PLL3 Clock Output
  * @{
  */
#define RCC_PLL3_DIVP                RCC_PLL3CFGR_PLL3PEN
#define RCC_PLL3_DIVQ                RCC_PLL3CFGR_PLL3QEN
#define RCC_PLL3_DIVR                RCC_PLL3CFGR_PLL3REN
/**
  * @}
  */
#endif /* RCC_CR_PLL3ON */

/** @defgroup RCC_PLL2_VCI_Range  RCC PLL2 VCI Range
  * @{
  */
#define RCC_PLL2_VCIRANGE_0           (0x00000000U)                                       /*!< Clock range frequency between 1 and 2 MHz  */
#define RCC_PLL2_VCIRANGE_1           RCC_PLL2CFGR_PLL2RGE_0                              /*!< Clock range frequency between 2 and 4 MHz  */
#define RCC_PLL2_VCIRANGE_2           RCC_PLL2CFGR_PLL2RGE_1                              /*!< Clock range frequency between 4 and 8 MHz  */
#define RCC_PLL2_VCIRANGE_3          (RCC_PLL2CFGR_PLL2RGE_0 | RCC_PLL2CFGR_PLL2RGE_1)    /*!< Clock range frequency between 8 and 16 MHz */
/**
  * @}
  */

/** @defgroup RCC_PLL2_VCO_Range  RCC PLL2 VCO Range
  * @{
  */
#define RCC_PLL2_VCORANGE_WIDE       (0x00000000U)                                       /*!< Wide VCO range: 192 to 836 MHz    */
#define RCC_PLL2_VCORANGE_MEDIUM     RCC_PLL2CFGR_PLL2VCOSEL                             /*!< Medium VCO range: 150 to 420 MHz  */

/**
  * @}
  */

/** @defgroup RCC_PLL2_Clock_Source  RCC PLL2 Clock Source
  * @{
  */
#define RCC_PLL2_SOURCE_NONE          (0x00000000U)
#define RCC_PLL2_SOURCE_HSI           RCC_PLL2CFGR_PLL2SRC_0
#define RCC_PLL2_SOURCE_CSI           RCC_PLL2CFGR_PLL2SRC_1
#define RCC_PLL2_SOURCE_HSE          (RCC_PLL2CFGR_PLL2SRC_0 | RCC_PLL2CFGR_PLL2SRC_1)
/**
  * @}
  */

#if defined(RCC_CR_PLL3ON)
/** @defgroup RCC_PLL3_VCI_Range  RCC PLL3 VCI Range
  * @{
  */
#define RCC_PLL3_VCIRANGE_0           (0x00000000U)                                       /*!< Clock range frequency between 1 and 2 MHz  */
#define RCC_PLL3_VCIRANGE_1           RCC_PLL3CFGR_PLL3RGE_0                              /*!< Clock range frequency between 2 and 4 MHz  */
#define RCC_PLL3_VCIRANGE_2           RCC_PLL3CFGR_PLL3RGE_1                              /*!< Clock range frequency between 4 and 8 MHz  */
#define RCC_PLL3_VCIRANGE_3          (RCC_PLL3CFGR_PLL3RGE_0 | RCC_PLL3CFGR_PLL3RGE_1)    /*!< Clock range frequency between 8 and 16 MHz */
/**
  * @}
  */

/** @defgroup RCC_PLL3_VCO_Range  RCC PLL3 VCO Range
  * @{
  */
#define RCC_PLL3_VCORANGE_WIDE        (0x00000000U)                                       /*!< Wide VCO range: 192 to 836 MHz    */
#define RCC_PLL3_VCORANGE_MEDIUM      RCC_PLL3CFGR_PLL3VCOSEL                             /*!< Medium VCO range: 150 to 420 MHz  */
/**
  * @}
  */

/** @defgroup RCC_PLL3_Clock_Source  RCC PLL3 Clock Source
  * @{
  */
#define RCC_PLL3_SOURCE_NONE          (0x00000000U)
#define RCC_PLL3_SOURCE_HSI           RCC_PLL3CFGR_PLL3SRC_0
#define RCC_PLL3_SOURCE_CSI           RCC_PLL3CFGR_PLL3SRC_1
#define RCC_PLL3_SOURCE_HSE          (RCC_PLL3CFGR_PLL3SRC_0 | RCC_PLL3CFGR_PLL3SRC_1)
/**
  * @}
  */
#endif /* RCC_CR_PLL3ON */

/** @defgroup RCCEx_USART1_Clock_Source USART1 Clock Source
  * @{
  */
#define RCC_USART1CLKSOURCE_PCLK2      ((uint32_t)0x00000000U)
#define RCC_USART1CLKSOURCE_PLL2Q      RCC_CCIPR1_USART1SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_USART1CLKSOURCE_PLL3Q      RCC_CCIPR1_USART1SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_USART1CLKSOURCE_HSI        (RCC_CCIPR1_USART1SEL_0 | RCC_CCIPR1_USART1SEL_1)
#define RCC_USART1CLKSOURCE_CSI        RCC_CCIPR1_USART1SEL_2
#define RCC_USART1CLKSOURCE_LSE        (RCC_CCIPR1_USART1SEL_0 | RCC_CCIPR1_USART1SEL_2)
/**
  * @}
  */

/** @defgroup RCCEx_USART2_Clock_Source USART2 Clock Source
  * @{
  */
#define RCC_USART2CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_USART2CLKSOURCE_PLL2Q      RCC_CCIPR1_USART2SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_USART2CLKSOURCE_PLL3Q      RCC_CCIPR1_USART2SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_USART2CLKSOURCE_HSI        (RCC_CCIPR1_USART2SEL_0 | RCC_CCIPR1_USART2SEL_1)
#define RCC_USART2CLKSOURCE_CSI        RCC_CCIPR1_USART2SEL_2
#define RCC_USART2CLKSOURCE_LSE        (RCC_CCIPR1_USART2SEL_0 | RCC_CCIPR1_USART2SEL_2)
/**
  * @}
  */

/** @defgroup RCCEx_USART3_Clock_Source USART3 Clock Source
  * @{
  */
#define RCC_USART3CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_USART3CLKSOURCE_PLL2Q      RCC_CCIPR1_USART3SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_USART3CLKSOURCE_PLL3Q      RCC_CCIPR1_USART3SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_USART3CLKSOURCE_HSI        (RCC_CCIPR1_USART3SEL_0 | RCC_CCIPR1_USART3SEL_1)
#define RCC_USART3CLKSOURCE_CSI        RCC_CCIPR1_USART3SEL_2
#define RCC_USART3CLKSOURCE_LSE        (RCC_CCIPR1_USART3SEL_0 | RCC_CCIPR1_USART3SEL_2)
/**
  * @}
  */

#if defined(UART4)
/** @defgroup RCCEx_UART4_Clock_Source UART4 Clock Source
  * @{
  */
#define RCC_UART4CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_UART4CLKSOURCE_PLL2Q      RCC_CCIPR1_UART4SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_UART4CLKSOURCE_PLL3Q      RCC_CCIPR1_UART4SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_UART4CLKSOURCE_HSI        (RCC_CCIPR1_UART4SEL_0 | RCC_CCIPR1_UART4SEL_1)
#define RCC_UART4CLKSOURCE_CSI        RCC_CCIPR1_UART4SEL_2
#define RCC_UART4CLKSOURCE_LSE        (RCC_CCIPR1_UART4SEL_0 | RCC_CCIPR1_UART4SEL_2)
/**
  * @}
  */
#endif /* UART4 */

#if defined(UART5)
/** @defgroup RCCEx_UART5_Clock_Source UART5 Clock Source
  * @{
  */
#define RCC_UART5CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_UART5CLKSOURCE_PLL2Q      RCC_CCIPR1_UART5SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_UART5CLKSOURCE_PLL3Q      RCC_CCIPR1_UART5SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_UART5CLKSOURCE_HSI        (RCC_CCIPR1_UART5SEL_0 | RCC_CCIPR1_UART5SEL_1)
#define RCC_UART5CLKSOURCE_CSI        RCC_CCIPR1_UART5SEL_2
#define RCC_UART5CLKSOURCE_LSE        (RCC_CCIPR1_UART5SEL_0 | RCC_CCIPR1_UART5SEL_2)
/**
  * @}
  */
#endif /* UART5 */

#if defined(USART6)
/** @defgroup RCCEx_USART6_Clock_Source USART6 Clock Source
  * @{
  */
#define RCC_USART6CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_USART6CLKSOURCE_PLL2Q      RCC_CCIPR1_USART6SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_USART6CLKSOURCE_PLL3Q      RCC_CCIPR1_USART6SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_USART6CLKSOURCE_HSI        (RCC_CCIPR1_USART6SEL_0 | RCC_CCIPR1_USART6SEL_1)
#define RCC_USART6CLKSOURCE_CSI        RCC_CCIPR1_USART6SEL_2
#define RCC_USART6CLKSOURCE_LSE        (RCC_CCIPR1_USART6SEL_0 | RCC_CCIPR1_USART6SEL_2)
/**
  * @}
  */
#endif /* USART6 */

#if defined(UART7)
/** @defgroup RCCEx_UART7_Clock_Source UART7 Clock Source
  * @{
  */
#define RCC_UART7CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_UART7CLKSOURCE_PLL2Q      RCC_CCIPR1_UART7SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_UART7CLKSOURCE_PLL3Q      RCC_CCIPR1_UART7SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_UART7CLKSOURCE_HSI        (RCC_CCIPR1_UART7SEL_0 | RCC_CCIPR1_UART7SEL_1)
#define RCC_UART7CLKSOURCE_CSI        RCC_CCIPR1_UART7SEL_2
#define RCC_UART7CLKSOURCE_LSE        (RCC_CCIPR1_UART7SEL_0 | RCC_CCIPR1_UART7SEL_2)
/**
  * @}
  */
#endif /* UART7 */

#if defined(UART8)
/** @defgroup RCCEx_UART8_Clock_Source UART8 Clock Source
  * @{
  */
#define RCC_UART8CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_UART8CLKSOURCE_PLL2Q      RCC_CCIPR1_UART8SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_UART8CLKSOURCE_PLL3Q      RCC_CCIPR1_UART8SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_UART8CLKSOURCE_HSI        (RCC_CCIPR1_UART8SEL_0 | RCC_CCIPR1_UART8SEL_1)
#define RCC_UART8CLKSOURCE_CSI        RCC_CCIPR1_UART8SEL_2
#define RCC_UART8CLKSOURCE_LSE        (RCC_CCIPR1_UART8SEL_0 | RCC_CCIPR1_UART8SEL_2)
/**
  * @}
  */
#endif /* UART8 */

#if defined(UART9)
/** @defgroup RCCEx_UART9_Clock_Source UART9 Clock Source
  * @{
  */
#define RCC_UART9CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_UART9CLKSOURCE_PLL2Q      RCC_CCIPR1_UART9SEL_0
#define RCC_UART9CLKSOURCE_PLL3Q      RCC_CCIPR1_UART9SEL_1
#define RCC_UART9CLKSOURCE_HSI        (RCC_CCIPR1_UART9SEL_0 | RCC_CCIPR1_UART9SEL_1)
#define RCC_UART9CLKSOURCE_CSI        RCC_CCIPR1_UART9SEL_2
#define RCC_UART9CLKSOURCE_LSE        (RCC_CCIPR1_UART9SEL_0 | RCC_CCIPR1_UART9SEL_2)
/**
  * @}
  */
#endif /* UART9 */

#if defined(USART10)
/** @defgroup RCCEx_USART10_Clock_Source USART10 Clock Source
  * @{
  */
#define RCC_USART10CLKSOURCE_PCLK1     ((uint32_t)0x00000000U)
#define RCC_USART10CLKSOURCE_PLL2Q     RCC_CCIPR1_USART10SEL_0
#define RCC_USART10CLKSOURCE_PLL3Q     RCC_CCIPR1_USART10SEL_1
#define RCC_USART10CLKSOURCE_HSI       (RCC_CCIPR1_USART10SEL_0 | RCC_CCIPR1_USART10SEL_1)
#define RCC_USART10CLKSOURCE_CSI       RCC_CCIPR1_USART10SEL_2
#define RCC_USART10CLKSOURCE_LSE       (RCC_CCIPR1_USART10SEL_0 | RCC_CCIPR1_USART10SEL_2)
/**
  * @}
  */
#endif /* USART10 */

#if defined(USART11)
/** @defgroup RCCEx_USART11_Clock_Source USART11 Clock Source
  * @{
  */
#define RCC_USART11CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_USART11CLKSOURCE_PLL2Q      RCC_CCIPR2_USART11SEL_0
#define RCC_USART11CLKSOURCE_PLL3Q      RCC_CCIPR2_USART11SEL_1
#define RCC_USART11CLKSOURCE_HSI        (RCC_CCIPR2_USART11SEL_0 | RCC_CCIPR2_USART11SEL_1)
#define RCC_USART11CLKSOURCE_CSI        RCC_CCIPR2_USART11SEL_2
#define RCC_USART11CLKSOURCE_LSE        (RCC_CCIPR2_USART11SEL_0 | RCC_CCIPR2_USART11SEL_2)
/**
  * @}
  */
#endif /* USART11 */

#if defined(UART12)
/** @defgroup RCCEx_UART12_Clock_Source UART12 Clock Source
  * @{
  */
#define RCC_UART12CLKSOURCE_PCLK1       ((uint32_t)0x00000000U)
#define RCC_UART12CLKSOURCE_PLL2Q       RCC_CCIPR2_UART12SEL_0
#define RCC_UART12CLKSOURCE_PLL3Q       RCC_CCIPR2_UART12SEL_1
#define RCC_UART12CLKSOURCE_HSI         (RCC_CCIPR2_UART12SEL_0 | RCC_CCIPR2_UART12SEL_1)
#define RCC_UART12CLKSOURCE_CSI         RCC_CCIPR2_UART12SEL_2
#define RCC_UART12CLKSOURCE_LSE         (RCC_CCIPR2_UART12SEL_0 | RCC_CCIPR2_UART12SEL_2)
/**
  * @}
  */
#endif /* UART12 */

/** @defgroup RCCEx_LPUART1_Clock_Source LPUART1 Clock Source
  * @{
  */
#define RCC_LPUART1CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPUART1CLKSOURCE_PLL2Q       RCC_CCIPR3_LPUART1SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_LPUART1CLKSOURCE_PLL3Q       RCC_CCIPR3_LPUART1SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_LPUART1CLKSOURCE_HSI        (RCC_CCIPR3_LPUART1SEL_0 | RCC_CCIPR3_LPUART1SEL_1)
#define RCC_LPUART1CLKSOURCE_CSI         RCC_CCIPR3_LPUART1SEL_2
#define RCC_LPUART1CLKSOURCE_LSE        (RCC_CCIPR3_LPUART1SEL_0 | RCC_CCIPR3_LPUART1SEL_2)

/**
  * @}
  */

/** @defgroup RCCEx_I2C1_Clock_Source I2C1 Clock Source
  * @{
  */
#define RCC_I2C1CLKSOURCE_PCLK1        ((uint32_t)0x00000000U)
#if defined(RCC_CR_PLL3ON)
#define RCC_I2C1CLKSOURCE_PLL3R        RCC_CCIPR4_I2C1SEL_0
#else
#define RCC_I2C1CLKSOURCE_PLL2R        RCC_CCIPR4_I2C1SEL_0
#endif /* RCC_CR_PLL3ON */
#define RCC_I2C1CLKSOURCE_HSI          RCC_CCIPR4_I2C1SEL_1
#define RCC_I2C1CLKSOURCE_CSI         (RCC_CCIPR4_I2C1SEL_1 | RCC_CCIPR4_I2C1SEL_0)
/**
  * @}
  */

/** @defgroup RCCEx_I2C2_Clock_Source I2C2 Clock Source
  * @{
  */
#define RCC_I2C2CLKSOURCE_PCLK1        ((uint32_t)0x00000000U)
#if defined(RCC_CR_PLL3ON)
#define RCC_I2C2CLKSOURCE_PLL3R        RCC_CCIPR4_I2C2SEL_0
#else
#define RCC_I2C2CLKSOURCE_PLL2R        RCC_CCIPR4_I2C2SEL_0
#endif /* RCC_CR_PLL3ON */
#define RCC_I2C2CLKSOURCE_HSI          RCC_CCIPR4_I2C2SEL_1
#define RCC_I2C2CLKSOURCE_CSI         (RCC_CCIPR4_I2C2SEL_1 | RCC_CCIPR4_I2C2SEL_0)
/**
  * @}
  */

#if defined(I2C3)
/** @defgroup RCCEx_I2C3_Clock_Source I2C3 Clock Source
  * @{
  */
#define RCC_I2C3CLKSOURCE_PCLK3        ((uint32_t)0x00000000U)
#if defined(RCC_CR_PLL3ON)
#define RCC_I2C3CLKSOURCE_PLL3R        RCC_CCIPR4_I2C3SEL_0
#else
#define RCC_I2C3CLKSOURCE_PLL2R        RCC_CCIPR4_I2C3SEL_0
#endif /* RCC_CR_PLL3ON */
#define RCC_I2C3CLKSOURCE_HSI          RCC_CCIPR4_I2C3SEL_1
#define RCC_I2C3CLKSOURCE_CSI         (RCC_CCIPR4_I2C3SEL_0 | RCC_CCIPR4_I2C3SEL_1)
/**
  * @}
  */
#endif /* I2C3 */

#if defined(I2C4)
/** @defgroup RCCEx_I2C4_Clock_Source I2C4 Clock Source
  * @{
  */
#define RCC_I2C4CLKSOURCE_PCLK3        ((uint32_t)0x00000000U)
#define RCC_I2C4CLKSOURCE_PLL3R        RCC_CCIPR4_I2C4SEL_0
#define RCC_I2C4CLKSOURCE_HSI          RCC_CCIPR4_I2C4SEL_1
#define RCC_I2C4CLKSOURCE_CSI         (RCC_CCIPR4_I2C4SEL_0 | RCC_CCIPR4_I2C4SEL_1)
/**
  * @}
  */
#endif /* I2C4 */

/** @defgroup RCCEx_I3C1_Clock_Source I3C1 Clock Source
  * @{
  */
#define RCC_I3C1CLKSOURCE_PCLK1        ((uint32_t)0x00000000U)
#if defined(RCC_CR_PLL3ON)
#define RCC_I3C1CLKSOURCE_PLL3R        RCC_CCIPR4_I3C1SEL_0
#else
#define RCC_I3C1CLKSOURCE_PLL2R        RCC_CCIPR4_I3C1SEL_0
#endif /* RCC_CR_PLL3ON */
#define RCC_I3C1CLKSOURCE_HSI          RCC_CCIPR4_I3C1SEL_1
/**
  * @}
  */

#if defined(I3C2)
/** @defgroup RCCEx_I3C2_Clock_Source I3C2 Clock Source
  * @{
  */
#define RCC_I3C2CLKSOURCE_PCLK3        ((uint32_t)0x00000000U)
#if defined(RCC_CR_PLL3ON)
#define RCC_I3C2CLKSOURCE_PLL3R        RCC_CCIPR4_I3C2SEL_0
#else
#define RCC_I3C2CLKSOURCE_PLL2R        RCC_CCIPR4_I3C2SEL_0
#endif /* RCC_CR_PLL3ON */
#define RCC_I3C2CLKSOURCE_HSI          RCC_CCIPR4_I3C2SEL_1
#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
#define RCC_I3C2CLKSOURCE_CSI          (RCC_CCIPR4_I3C2SEL_0 | RCC_CCIPR4_I3C2SEL_1)
#endif /* defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx) */
/**
  * @}
  */
#endif /* I3C2 */

/** @defgroup RCCEx_RNG_Clock_Source  RCCEx RNG Clock Source
  * @{
  */
#define RCC_RNGCLKSOURCE_HSI48        ((uint32_t)0x00000000U)
#define RCC_RNGCLKSOURCE_PLL1Q         RCC_CCIPR5_RNGSEL_0
#define RCC_RNGCLKSOURCE_LSE           RCC_CCIPR5_RNGSEL_1
#define RCC_RNGCLKSOURCE_LSI          (RCC_CCIPR5_RNGSEL_0 | RCC_CCIPR5_RNGSEL_1)
/**
  * @}
  */

/** @defgroup RCCEx_SPI1_Clock_Source SPI1 Clock Source
  * @{
  */
#define RCC_SPI1CLKSOURCE_PLL1Q       ((uint32_t)0x00000000U)
#define RCC_SPI1CLKSOURCE_PLL2P        RCC_CCIPR3_SPI1SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_SPI1CLKSOURCE_PLL3P        RCC_CCIPR3_SPI1SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_SPI1CLKSOURCE_PIN         (RCC_CCIPR3_SPI1SEL_0 | RCC_CCIPR3_SPI1SEL_1)
#define RCC_SPI1CLKSOURCE_CLKP         RCC_CCIPR3_SPI1SEL_2
/**
  * @}
  */

/** @defgroup RCCEx_SPI2_Clock_Source SPI2 Clock Source
  * @{
  */
#define RCC_SPI2CLKSOURCE_PLL1Q       ((uint32_t)0x00000000U)
#define RCC_SPI2CLKSOURCE_PLL2P        RCC_CCIPR3_SPI2SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_SPI2CLKSOURCE_PLL3P        RCC_CCIPR3_SPI2SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_SPI2CLKSOURCE_PIN         (RCC_CCIPR3_SPI2SEL_0 | RCC_CCIPR3_SPI2SEL_1)
#define RCC_SPI2CLKSOURCE_CLKP         RCC_CCIPR3_SPI2SEL_2
/**
  * @}
  */

/** @defgroup RCCEx_SPI3_Clock_Source SPI3 Clock Source
  * @{
  */
#define RCC_SPI3CLKSOURCE_PLL1Q       ((uint32_t)0x00000000U)
#define RCC_SPI3CLKSOURCE_PLL2P        RCC_CCIPR3_SPI3SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_SPI3CLKSOURCE_PLL3P        RCC_CCIPR3_SPI3SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_SPI3CLKSOURCE_PIN         (RCC_CCIPR3_SPI3SEL_0 | RCC_CCIPR3_SPI3SEL_1)
#define RCC_SPI3CLKSOURCE_CLKP         RCC_CCIPR3_SPI3SEL_2
/**
  * @}
  */

#if defined(SPI4)
/** @defgroup RCCEx_SPI4_Clock_Source SPI4 Clock Source
  * @{
  */
#define RCC_SPI4CLKSOURCE_PCLK2        ((uint32_t)0x00000000U)
#define RCC_SPI4CLKSOURCE_PLL2Q         RCC_CCIPR3_SPI4SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_SPI4CLKSOURCE_PLL3Q         RCC_CCIPR3_SPI4SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_SPI4CLKSOURCE_HSI           (RCC_CCIPR3_SPI4SEL_0 | RCC_CCIPR3_SPI4SEL_1)
#define RCC_SPI4CLKSOURCE_CSI           RCC_CCIPR3_SPI4SEL_2
#define RCC_SPI4CLKSOURCE_HSE          (RCC_CCIPR3_SPI4SEL_0 | RCC_CCIPR3_SPI4SEL_2)
/**
  * @}
  */
#endif /* SPI4 */

#if defined(SPI5)
/** @defgroup RCCEx_SPI5_Clock_Source SPI5 Clock Source
  * @{
  */
#define RCC_SPI5CLKSOURCE_PCLK3        ((uint32_t)0x00000000U)
#define RCC_SPI5CLKSOURCE_PLL2Q         RCC_CCIPR3_SPI5SEL_0
#define RCC_SPI5CLKSOURCE_PLL3Q         RCC_CCIPR3_SPI5SEL_1
#define RCC_SPI5CLKSOURCE_HSI           (RCC_CCIPR3_SPI5SEL_0 | RCC_CCIPR3_SPI5SEL_1)
#define RCC_SPI5CLKSOURCE_CSI           RCC_CCIPR3_SPI5SEL_2
#define RCC_SPI5CLKSOURCE_HSE          (RCC_CCIPR3_SPI5SEL_0 | RCC_CCIPR3_SPI5SEL_2)
/**
  * @}
  */
#endif /* SPI5 */

#if defined(SPI6)
/** @defgroup RCCEx_SPI6_Clock_Source SPI6 Clock Source
  * @{
  */
#define RCC_SPI6CLKSOURCE_PCLK2        ((uint32_t)0x00000000U)
#define RCC_SPI6CLKSOURCE_PLL2Q         RCC_CCIPR3_SPI6SEL_0
#define RCC_SPI6CLKSOURCE_PLL3Q         RCC_CCIPR3_SPI6SEL_1
#define RCC_SPI6CLKSOURCE_HSI           (RCC_CCIPR3_SPI6SEL_0 | RCC_CCIPR3_SPI6SEL_1)
#define RCC_SPI6CLKSOURCE_CSI           RCC_CCIPR3_SPI6SEL_2
#define RCC_SPI6CLKSOURCE_HSE          (RCC_CCIPR3_SPI6SEL_0 | RCC_CCIPR3_SPI6SEL_2)
/**
  * @}
  */
#endif /* SPI6 */

/** @defgroup RCCEx_LPTIM1_Clock_Source LPTIM1 Clock Source
  * @{
  */
#define RCC_LPTIM1CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPTIM1CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM1SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_LPTIM1CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM1SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_LPTIM1CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM1SEL_0 | RCC_CCIPR2_LPTIM1SEL_1)
#define RCC_LPTIM1CLKSOURCE_LSI         RCC_CCIPR2_LPTIM1SEL_2
#define RCC_LPTIM1CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM1SEL_0 | RCC_CCIPR2_LPTIM1SEL_2)
/**
  * @}
  */

/** @defgroup RCCEx_LPTIM2_Clock_Source LPTIM2 Clock Source
  * @{
  */
#define RCC_LPTIM2CLKSOURCE_PCLK1      ((uint32_t)0x00000000U)
#define RCC_LPTIM2CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM2SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_LPTIM2CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM2SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_LPTIM2CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM2SEL_0 | RCC_CCIPR2_LPTIM2SEL_1)
#define RCC_LPTIM2CLKSOURCE_LSI         RCC_CCIPR2_LPTIM2SEL_2
#define RCC_LPTIM2CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM2SEL_0 | RCC_CCIPR2_LPTIM2SEL_2)
/**
  * @}
  */

#if defined(LPTIM3)
/** @defgroup RCCEx_LPTIM3_Clock_Source LPTIM3 Clock Source
  * @{
  */
#define RCC_LPTIM3CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPTIM3CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM3SEL_0
#define RCC_LPTIM3CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM3SEL_1
#define RCC_LPTIM3CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM3SEL_0 | RCC_CCIPR2_LPTIM3SEL_1)
#define RCC_LPTIM3CLKSOURCE_LSI         RCC_CCIPR2_LPTIM3SEL_2
#define RCC_LPTIM3CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM3SEL_0 | RCC_CCIPR2_LPTIM3SEL_2)
/**
  * @}
  */
#endif /* LPTIM3 */

#if defined(LPTIM4)
/** @defgroup RCCEx_LPTIM4_Clock_Source LPTIM4 Clock Source
  * @{
  */
#define RCC_LPTIM4CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPTIM4CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM4SEL_0
#define RCC_LPTIM4CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM4SEL_1
#define RCC_LPTIM4CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM4SEL_0 | RCC_CCIPR2_LPTIM4SEL_1)
#define RCC_LPTIM4CLKSOURCE_LSI         RCC_CCIPR2_LPTIM4SEL_2
#define RCC_LPTIM4CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM4SEL_0 | RCC_CCIPR2_LPTIM4SEL_2)
/**
  * @}
  */
#endif /* LPTIM4 */

#if defined(LPTIM5)
/** @defgroup RCCEx_LPTIM5_Clock_Source LPTIM5 Clock Source
  * @{
  */
#define RCC_LPTIM5CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPTIM5CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM5SEL_0
#define RCC_LPTIM5CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM5SEL_1
#define RCC_LPTIM5CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM5SEL_0 | RCC_CCIPR2_LPTIM5SEL_1)
#define RCC_LPTIM5CLKSOURCE_LSI         RCC_CCIPR2_LPTIM5SEL_2
#define RCC_LPTIM5CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM5SEL_0 | RCC_CCIPR2_LPTIM5SEL_2)
/**
  * @}
  */
#endif /* LPTIM5 */

#if defined(LPTIM6)
/** @defgroup RCCEx_LPTIM6_Clock_Source LPTIM6 Clock Source
  * @{
  */
#define RCC_LPTIM6CLKSOURCE_PCLK3      ((uint32_t)0x00000000U)
#define RCC_LPTIM6CLKSOURCE_PLL2P       RCC_CCIPR2_LPTIM6SEL_0
#define RCC_LPTIM6CLKSOURCE_PLL3R       RCC_CCIPR2_LPTIM6SEL_1
#define RCC_LPTIM6CLKSOURCE_LSE        (RCC_CCIPR2_LPTIM6SEL_0 | RCC_CCIPR2_LPTIM6SEL_1)
#define RCC_LPTIM6CLKSOURCE_LSI         RCC_CCIPR2_LPTIM6SEL_2
#define RCC_LPTIM6CLKSOURCE_CLKP       (RCC_CCIPR2_LPTIM6SEL_0 | RCC_CCIPR2_LPTIM6SEL_2)
/**
  * @}
  */
#endif /* LPTIM6 */

/** @defgroup RCCEx_FDCAN_Clock_Source FDCAN Kernel Clock Source
  * @{
  */
#define RCC_FDCANCLKSOURCE_HSE         ((uint32_t)0x00000000U)
#define RCC_FDCANCLKSOURCE_PLL1Q        RCC_CCIPR5_FDCANSEL_0
#define RCC_FDCANCLKSOURCE_PLL2Q        RCC_CCIPR5_FDCANSEL_1
/**
  * @}
  */

/** @defgroup RCCEx_ADCDAC_Clock_Source ADCDAC Kernel Clock Source
  * @{
  */
#define RCC_ADCDACCLKSOURCE_HCLK         ((uint32_t)0x00000000U)
#define RCC_ADCDACCLKSOURCE_SYSCLK       RCC_CCIPR5_ADCDACSEL_0
#define RCC_ADCDACCLKSOURCE_PLL2R        RCC_CCIPR5_ADCDACSEL_1
#define RCC_ADCDACCLKSOURCE_HSE         (RCC_CCIPR5_ADCDACSEL_1 | RCC_CCIPR5_ADCDACSEL_0)
#define RCC_ADCDACCLKSOURCE_HSI          RCC_CCIPR5_ADCDACSEL_2
#define RCC_ADCDACCLKSOURCE_CSI         (RCC_CCIPR5_ADCDACSEL_2 | RCC_CCIPR5_ADCDACSEL_0)

/**
  * @}
  */

/** @defgroup RCCEx_DAC_Low_Power_Clock_Source DAC Low Power Kernel Clock Source
  * @{
  */
#define RCC_DACLPCLKSOURCE_LSE   ((uint32_t)0x00000000U)
#define RCC_DACLPCLKSOURCE_LSI   RCC_CCIPR5_DACSEL
/**
  * @}
  */

#if defined(SAI1)
/** @defgroup RCCEx_SAI1_Clock_Source SAI1 Clock Source
  * @{
  */
#define RCC_SAI1CLKSOURCE_PLL1Q        ((uint32_t)0x00000000U)
#define RCC_SAI1CLKSOURCE_PLL2P        RCC_CCIPR5_SAI1SEL_0
#define RCC_SAI1CLKSOURCE_PLL3P        RCC_CCIPR5_SAI1SEL_1
#define RCC_SAI1CLKSOURCE_PIN          (RCC_CCIPR5_SAI1SEL_1 | RCC_CCIPR5_SAI1SEL_0)
#define RCC_SAI1CLKSOURCE_CLKP         RCC_CCIPR5_SAI1SEL_2
/**
  * @}
  */
#endif /* SAI1 */

#if defined(SAI2)
/** @defgroup RCCEx_SAI2_Clock_Source SAI2 Clock Source
  * @{
  */
#define RCC_SAI2CLKSOURCE_PLL1Q        ((uint32_t)0x00000000U)
#define RCC_SAI2CLKSOURCE_PLL2P        RCC_CCIPR5_SAI2SEL_0
#define RCC_SAI2CLKSOURCE_PLL3P        RCC_CCIPR5_SAI2SEL_1
#define RCC_SAI2CLKSOURCE_PIN          (RCC_CCIPR5_SAI2SEL_1 | RCC_CCIPR5_SAI2SEL_0)
#define RCC_SAI2CLKSOURCE_CLKP         RCC_CCIPR5_SAI2SEL_2
/**
  * @}
  */
#endif /* SAI2 */

/** @defgroup RCCEx_CLKP_Clock_Source  RCCEx CLKP Clock Source
  * @{
  */
#define RCC_CLKPSOURCE_HSI            (0x00000000U)
#define RCC_CLKPSOURCE_CSI             RCC_CCIPR5_CKERPSEL_0
#define RCC_CLKPSOURCE_HSE             RCC_CCIPR5_CKERPSEL_1
/**
  * @}
  */

#if defined(SDMMC1)
/** @defgroup RCCEx_SDMMC1_Clock_Source SDMMC1 Clock Source
  * @{
  */
#define RCC_SDMMC1CLKSOURCE_PLL1Q     ((uint32_t)0x00000000U)
#define RCC_SDMMC1CLKSOURCE_PLL2R      RCC_CCIPR4_SDMMC1SEL
/**
  * @}
  */
#endif /* SDMMC1 */

#if defined(SDMMC2)
/** @defgroup RCCEx_SDMMC2_Clock_Source SDMMC2 Clock Source
  * @{
  */
#define RCC_SDMMC2CLKSOURCE_PLL1Q     ((uint32_t)0x00000000U)
#define RCC_SDMMC2CLKSOURCE_PLL2R      RCC_CCIPR4_SDMMC2SEL
/**
  * @}
  */
#endif /* SDMMC2 */

#if defined(OCTOSPI1)
/** @defgroup RCCEx_OSPI_Clock_Source OctoSPI Clock Source
  * @{
  */
#define RCC_OSPICLKSOURCE_HCLK      ((uint32_t)0x00000000U)
#define RCC_OSPICLKSOURCE_PLL1Q      RCC_CCIPR4_OCTOSPISEL_0
#define RCC_OSPICLKSOURCE_PLL2R      RCC_CCIPR4_OCTOSPISEL_1
#define RCC_OSPICLKSOURCE_CLKP      (RCC_CCIPR4_OCTOSPISEL_0 | RCC_CCIPR4_OCTOSPISEL_1)
/**
  * @}
  */
#endif /* OCTOSPI1 */

#if defined(CEC)
/** @defgroup RCCEx_CEC_Clock_Source CEC Clock Source
  * @{
  */
#define RCC_CECCLKSOURCE_LSE              ((uint32_t)0x00000000U)
#define RCC_CECCLKSOURCE_LSI               RCC_CCIPR5_CECSEL_0
#define RCC_CECCLKSOURCE_CSI_DIV122        RCC_CCIPR5_CECSEL_1
/**
  * @}
  */
#endif /* CEC */

#if defined(USB_DRD_FS)
/** @defgroup RCCEx_USB_Clock_Source  RCCEx USB Clock Source
  * @{
  */
#define RCC_USBCLKSOURCE_PLL1Q          RCC_CCIPR4_USBSEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_USBCLKSOURCE_PLL3Q          RCC_CCIPR4_USBSEL_1
#else
#define RCC_USBCLKSOURCE_PLL2Q          RCC_CCIPR4_USBSEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_USBCLKSOURCE_HSI48          RCC_CCIPR4_USBSEL
/**
  * @}
  */
#endif /* USB_DRD_FS */

#if defined(PLAY1)
/** @defgroup RCCEx_PLAY1_Clock_Source  RCCEx PLAY1 Clock Source
  * @{
  */
#define RCC_PLAY1CLKSOURCE_PCLK3          ((uint32_t)0x00000000U)
#define RCC_PLAY1CLKSOURCE_PLL2P          RCC_CCIPR3_PLAY1SEL_0
#if defined(RCC_CR_PLL3ON)
#define RCC_PLAY1CLKSOURCE_PLL3R          RCC_CCIPR3_PLAY1SEL_1
#endif /* RCC_CR_PLL3ON */
#define RCC_PLAY1CLKSOURCE_LSE            (RCC_CCIPR3_PLAY1SEL_0 | RCC_CCIPR3_PLAY1SEL_1)
#define RCC_PLAY1CLKSOURCE_LSI            RCC_CCIPR3_PLAY1SEL_2
#define RCC_PLAY1CLKSOURCE_CLKP           (RCC_CCIPR3_PLAY1SEL_0 | RCC_CCIPR3_PLAY1SEL_2)
/**
  * @}
  */

/** @defgroup RCCEx_PLAY1_Clock_Divider RCCEx PLAY1 Clock Divider
  * @{
  */
#define RCC_PLAY1_CLK_DIV_1     ((uint32_t)0x00000000U)                                                                          /*!< PLAY1 not divided (default) */
#define RCC_PLAY1_CLK_DIV_2     RCC_CCIPR3_PLAY1DIV_0                                                                            /*!< PLAY1 divided by 2 */
#define RCC_PLAY1_CLK_DIV_4     RCC_CCIPR3_PLAY1DIV_1                                                                            /*!< PLAY1 divided by 4 */
#define RCC_PLAY1_CLK_DIV_8     (RCC_CCIPR3_PLAY1DIV_0 | RCC_CCIPR3_PLAY1DIV_1)                                                  /*!< PLAY1 divided by 8 */
#define RCC_PLAY1_CLK_DIV_16    RCC_CCIPR3_PLAY1DIV_2                                                                            /*!< PLAY1 divided by 16 */
#define RCC_PLAY1_CLK_DIV_32    (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_0)                                                  /*!< PLAY1 divided by 32 */
#define RCC_PLAY1_CLK_DIV_64    (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1)                                                  /*!< PLAY1 divided by 64 */
#define RCC_PLAY1_CLK_DIV_128   (RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1 | RCC_CCIPR3_PLAY1DIV_0)                          /*!< PLAY1 divided by 128 */
#define RCC_PLAY1_CLK_DIV_256   RCC_CCIPR3_PLAY1DIV_3                                                                            /*!< PLAY1 divided by 256 */
#define RCC_PLAY1_CLK_DIV_512   (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_0)                                                  /*!< PLAY1 divided by 512 */
#define RCC_PLAY1_CLK_DIV_1024  (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_1)                                                  /*!< PLAY1 divided by 1024 */
#define RCC_PLAY1_CLK_DIV_2048  (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_1 | RCC_CCIPR3_PLAY1DIV_0)                          /*!< PLAY1 divided by 2048 */
#define RCC_PLAY1_CLK_DIV_4096  (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2)                                                  /*!< PLAY1 divided by 4096 */
#define RCC_PLAY1_CLK_DIV_8192  (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_0)                          /*!< PLAY1 divided by 8192 */
#define RCC_PLAY1_CLK_DIV_16384 (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1)                          /*!< PLAY1 divided by 16384 */
#define RCC_PLAY1_CLK_DIV_32768 (RCC_CCIPR3_PLAY1DIV_3 | RCC_CCIPR3_PLAY1DIV_2 | RCC_CCIPR3_PLAY1DIV_1 | \
                                 RCC_CCIPR3_PLAY1DIV_0)                                                                          /*!< PLAY1 divided by 32768 */
/**
  * @}
  */
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/** @defgroup RCCEx_OTGFS_Clock_Source  RCCEx USB_OTG_FS Clock Source
  * @{
  */
#define RCC_OTGFSCLKSOURCE_HSI48          ((uint32_t)0x00000000U)
#define RCC_OTGFSCLKSOURCE_PLL1Q          RCC_CCIPR4_OTGFSSEL_0
#define RCC_OTGFSCLKSOURCE_PLL3Q          RCC_CCIPR4_OTGFSSEL_1
#if defined(PWR_USBSCR_OTGHSEN)
#define RCC_OTGFSCLKSOURCE_CLK48          (RCC_CCIPR4_OTGFSSEL_0 | RCC_CCIPR4_OTGFSSEL_1)
#endif /* PWR_USBSCR_OTGHSEN */
/**
  * @}
  */
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/** @defgroup RCCEx_OTGHS_Clock_Source  RCCEx USB_OTG_HS Clock Source
  * @{
  */
#define RCC_OTGHSCLKSOURCE_HSE            ((uint32_t)0x00000000U)
#define RCC_OTGHSCLKSOURCE_PLL3Q          RCC_CCIPR4_OTGHSSEL_0
#define RCC_OTGHSCLKSOURCE_HSE_DIV2       RCC_CCIPR4_OTGHSSEL_1
#define RCC_OTGHSCLKSOURCE_PLL1Q_DIV2     (RCC_CCIPR4_OTGHSSEL_0 | RCC_CCIPR4_OTGHSSEL_1)
/**
  * @}
  */
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/** @defgroup RCCEx_OTGPHYREFCK_Clock_Source    RCCEx OTGPHYREFCK Clock Source
  * @{
  */
#define RCC_OTGPHYREFCKCLKSOURCE_16M            (RCC_CCIPR4_OTGPHYREFCKSEL_0 | RCC_CCIPR4_OTGPHYREFCKSEL_1)
#define RCC_OTGPHYREFCKCLKSOURCE_19_2M          RCC_CCIPR4_OTGPHYREFCKSEL_3
#define RCC_OTGPHYREFCKCLKSOURCE_20M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 | RCC_CCIPR4_OTGPHYREFCKSEL_0)
#define RCC_OTGPHYREFCKCLKSOURCE_24M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 | RCC_CCIPR4_OTGPHYREFCKSEL_1)
#define RCC_OTGPHYREFCKCLKSOURCE_26M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 |\
                                                 RCC_CCIPR4_OTGPHYREFCKSEL_2 | RCC_CCIPR4_OTGPHYREFCKSEL_1)
#define RCC_OTGPHYREFCKCLKSOURCE_32M            (RCC_CCIPR4_OTGPHYREFCKSEL_3 |\
                                                 RCC_CCIPR4_OTGPHYREFCKSEL_1 | RCC_CCIPR4_OTGPHYREFCKSEL_0)

/**
  * @}
  */
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/** @defgroup RCCEx_ETHPTP_Clock_Source  RCCEx ETHPTP Clock Source
  * @{
  */
#define RCC_ETHPTPCLKSOURCE_HCLK               ((uint32_t)0x00000000U)
#define RCC_ETHPTPCLKSOURCE_PLL1R              RCC_CCIPR5_ETHPTPCLKSEL_0
#define RCC_ETHPTPCLKSOURCE_PLL1Q              RCC_CCIPR5_ETHPTPCLKSEL_1
#if defined(RCC_CR_PLL3ON)
#define RCC_ETHPTPCLKSOURCE_PLL3P              (RCC_CCIPR5_ETHPTPCLKSEL_0 | RCC_CCIPR5_ETHPTPCLKSEL_1)
#endif /* RCC_CR_PLL3ON */
/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/** @defgroup RCCEx_ETHPTP_Clock_Divider RCCEx ETHPTP Clock Divider
  * @{
  */
#define RCC_ETHPTP_CLK_DIV_1     ((uint32_t)0x00000000U)                                                     /*!< ETHPTP not divided (default) */
#define RCC_ETHPTP_CLK_DIV_2     RCC_CCIPR4_ETHPTPDIV_0                                                      /*!< ETHPTP divided by 2 */
#define RCC_ETHPTP_CLK_DIV_3     RCC_CCIPR4_ETHPTPDIV_1                                                      /*!< ETHPTP divided by 3 */
#define RCC_ETHPTP_CLK_DIV_4     (RCC_CCIPR4_ETHPTPDIV_0 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 4 */
#define RCC_ETHPTP_CLK_DIV_5     RCC_CCIPR4_ETHPTPDIV_2                                                      /*!< ETHPTP divided by 5 */
#define RCC_ETHPTP_CLK_DIV_6     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_0)                           /*!< ETHPTP divided by 6 */
#define RCC_ETHPTP_CLK_DIV_7     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 7 */
#define RCC_ETHPTP_CLK_DIV_8     (RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 8 */
#define RCC_ETHPTP_CLK_DIV_9     RCC_CCIPR4_ETHPTPDIV_3                                                      /*!< ETHPTP divided by 9 */
#define RCC_ETHPTP_CLK_DIV_10    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_0)                           /*!< ETHPTP divided by 10 */
#define RCC_ETHPTP_CLK_DIV_11    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_1)                           /*!< ETHPTP divided by 11 */
#define RCC_ETHPTP_CLK_DIV_12    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 12 */
#define RCC_ETHPTP_CLK_DIV_13    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2)                           /*!< ETHPTP divided by 13 */
#define RCC_ETHPTP_CLK_DIV_14    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_0)  /*!< ETHPTP divided by 14 */
#define RCC_ETHPTP_CLK_DIV_15    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | RCC_CCIPR4_ETHPTPDIV_1)  /*!< ETHPTP divided by 15 */
#define RCC_ETHPTP_CLK_DIV_16    (RCC_CCIPR4_ETHPTPDIV_3 | RCC_CCIPR4_ETHPTPDIV_2 | \
                                  RCC_CCIPR4_ETHPTPDIV_1 | RCC_CCIPR4_ETHPTPDIV_0)                           /*!< ETHPTP divided by 16 */


#endif /* RCC_CCIPR4_ETHPTPDIV */

#if defined(OCTOSPI2)
/** @defgroup RCCEx_OSPI2_Clock_Source OctoSPI2 Clock Source
  * @{
  */
#define RCC_OSPI2CLKSOURCE_HCLK      ((uint32_t)0x00000000U)
#define RCC_OSPI2CLKSOURCE_PLL1Q      RCC_CCIPR5_OCTOSPI2SEL_0
#define RCC_OSPI2CLKSOURCE_PLL2R      RCC_CCIPR5_OCTOSPI2SEL_1
#define RCC_OSPI2CLKSOURCE_CLKP      (RCC_CCIPR5_OCTOSPI2SEL_0 | RCC_CCIPR5_OCTOSPI2SEL_1)
/**
  * @}
  */
#endif /* OCTOSPI2 */

#if defined(LTDC)
/** @defgroup RCCEx_LTDC_Clock_Source  RCCEx LTDC Clock Source
  * @{
  */
#define RCC_LTDCCLKSOURCE_PLL3R               ((uint32_t)0x00000000U)
#define RCC_LTDCCLKSOURCE_PLL2R               RCC_CCIPR5_LTDCSEL
/**
  * @}
  */
#endif /* LTDC */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/** @defgroup RCCEx_ETHREF_Clock_Source  RCCEx ETHREF Clock Source
  * @{
  */
#define RCC_ETHREFCLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define RCC_ETHREFCLKSOURCE_PLL1R               RCC_CCIPR5_ETHREFCLKSEL
/**
  * @}
  */
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

#if defined(ADF1)
/** @defgroup RCCEx_ADF1_Clock_Source  RCCEx ADF1 Clock Source
  * @{
  */
#define RCC_ADF1CLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define RCC_ADF1CLKSOURCE_PLL2P               RCC_CCIPR5_ADF1SEL_0
#define RCC_ADF1CLKSOURCE_PLL3P               RCC_CCIPR5_ADF1SEL_1
#define RCC_ADF1CLKSOURCE_PIN                 (RCC_CCIPR5_ADF1SEL_0 | RCC_CCIPR5_ADF1SEL_1)
#define RCC_ADF1CLKSOURCE_CLKP                RCC_CCIPR5_ADF1SEL_2
/**
  * @}
  */
#endif /* ADF1 */

#if defined(MDF1)
/** @defgroup RCCEx_MDF1_Clock_Source  RCCEx MDF1 Clock Source
  * @{
  */
#define RCC_MDF1CLKSOURCE_PLL1Q               ((uint32_t)0x00000000U)
#define RCC_MDF1CLKSOURCE_PLL2P               RCC_CCIPR5_MDF1SEL_0
#define RCC_MDF1CLKSOURCE_PLL3P               RCC_CCIPR5_MDF1SEL_1
#define RCC_MDF1CLKSOURCE_PIN                 (RCC_CCIPR5_MDF1SEL_0 | RCC_CCIPR5_MDF1SEL_1)
#define RCC_MDF1CLKSOURCE_CLKP                RCC_CCIPR5_MDF1SEL_2

/**
  * @}
  */
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/** @defgroup RCCEx_ETH_Clock_Source  RCCEx ETH Clock Source
  * @{
  */
#define RCC_ETHCLKSOURCE_HSE                 ((uint32_t)0x00000000U)
#define RCC_ETHCLKSOURCE_PLL1Q               RCC_CCIPR4_ETHCLKSEL
/**
  * @}
  */
#endif /* RCC_CCIPR4_ETHCLKSEL */

/** @defgroup RCCEx_TIM_Prescaler_Selection RCCEx TIM Prescaler Selection
  * @{
  */
#define RCC_TIMPRES_DEACTIVATED         (0x00000000U)
#define RCC_TIMPRES_ACTIVATED            RCC_CFGR1_TIMPRE

/**
  * @}
  */

#if defined(CRS)

/** @defgroup RCCEx_CRS_Status RCCEx CRS Status
  * @{
  */
#define RCC_CRS_NONE                   ((uint32_t)0x00000000U)
#define RCC_CRS_TIMEOUT                ((uint32_t)0x00000001U)
#define RCC_CRS_SYNCOK                 ((uint32_t)0x00000002U)
#define RCC_CRS_SYNCWARN               ((uint32_t)0x00000004U)
#define RCC_CRS_SYNCERR                ((uint32_t)0x00000008U)
#define RCC_CRS_SYNCMISS               ((uint32_t)0x00000010U)
#define RCC_CRS_TRIMOVF                ((uint32_t)0x00000020U)
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroSource RCCEx CRS SynchroSource
  * @{
  */
#define RCC_CRS_SYNC_SOURCE_GPIO       ((uint32_t)0x00000000U)                     /*!< Synchro Signal source GPIO */
#define RCC_CRS_SYNC_SOURCE_LSE        CRS_CFGR_SYNCSRC_0                          /*!< Synchro Signal source LSE */
#if defined(USB_DRD_FS)
#define RCC_CRS_SYNC_SOURCE_USB        CRS_CFGR_SYNCSRC_1                          /*!< Synchro Signal source USB SOF (default)*/
#elif defined(USB_OTG_FS)
#define RCC_CRS_SYNC_SOURCE_OTG_FS     CRS_CFGR_SYNCSRC_1                          /*!< Synchro Signal source USB SOF (default)*/
#endif /* USB_DRD_FS */
#if defined(USB_OTG_HS)
#define RCC_CRS_SYNC_SOURCE_OTG_HS     (CRS_CFGR_SYNCSRC_0 | CRS_CFGR_SYNCSRC_1 )  /*!< Synchro Signal source USB SOF (default)*/
#endif /* USB_OTG_HS */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroDivider RCCEx CRS SynchroDivider
  * @{
  */
#define RCC_CRS_SYNC_DIV1        ((uint32_t)0x00000000U)                   /*!< Synchro Signal not divided (default) */
#define RCC_CRS_SYNC_DIV2        CRS_CFGR_SYNCDIV_0                        /*!< Synchro Signal divided by 2 */
#define RCC_CRS_SYNC_DIV4        CRS_CFGR_SYNCDIV_1                        /*!< Synchro Signal divided by 4 */
#define RCC_CRS_SYNC_DIV8        (CRS_CFGR_SYNCDIV_1 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 8 */
#define RCC_CRS_SYNC_DIV16       CRS_CFGR_SYNCDIV_2                        /*!< Synchro Signal divided by 16 */
#define RCC_CRS_SYNC_DIV32       (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_0) /*!< Synchro Signal divided by 32 */
#define RCC_CRS_SYNC_DIV64       (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_1) /*!< Synchro Signal divided by 64 */
#define RCC_CRS_SYNC_DIV128      CRS_CFGR_SYNCDIV                          /*!< Synchro Signal divided by 128 */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_SynchroPolarity RCCEx CRS SynchroPolarity
  * @{
  */
#define RCC_CRS_SYNC_POLARITY_RISING   ((uint32_t)0x00000000U) /*!< Synchro Active on rising edge (default) */
#define RCC_CRS_SYNC_POLARITY_FALLING  CRS_CFGR_SYNCPOL        /*!< Synchro Active on falling edge */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_ReloadValueDefault RCCEx CRS ReloadValueDefault
  * @{
  */
#define RCC_CRS_RELOADVALUE_DEFAULT    ((uint32_t)0x0000BB7FU) /*!< The reset value of the RELOAD field corresponds
                                                                   to a target frequency of 48 MHz and a synchronization
                                                                   signal frequency of 1 kHz (SOF signal from USB). */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_ErrorLimitDefault RCCEx CRS ErrorLimitDefault
  * @{
  */
#define RCC_CRS_ERRORLIMIT_DEFAULT     ((uint32_t)0x00000022U) /*!< Default Frequency error limit */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_HSI48CalibrationDefault RCCEx CRS HSI48CalibrationDefault
  * @{
  */
#define RCC_CRS_HSI48CALIBRATION_DEFAULT ((uint32_t)0x00000020U) /*!< The default value is 32, which corresponds to the
                                                                      middle of the trimming interval. The trimming step
                                                                      is around 67 kHz between two consecutive TRIM
                                                                      steps. A higher TRIM value corresponds to a higher
                                                                      output frequency */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_FreqErrorDirection RCCEx CRS FreqErrorDirection
  * @{
  */
#define RCC_CRS_FREQERRORDIR_UP        ((uint32_t)0x00000000U)   /*!< Upcounting direction, the actual frequency is above the target */
#define RCC_CRS_FREQERRORDIR_DOWN      ((uint32_t)CRS_ISR_FEDIR) /*!< Downcounting direction, the actual frequency is below the target */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_Interrupt_Sources RCCEx CRS Interrupt Sources
  * @{
  */
#define RCC_CRS_IT_SYNCOK              CRS_CR_SYNCOKIE       /*!< SYNC event OK */
#define RCC_CRS_IT_SYNCWARN            CRS_CR_SYNCWARNIE     /*!< SYNC warning */
#define RCC_CRS_IT_ERR                 CRS_CR_ERRIE          /*!< Error */
#define RCC_CRS_IT_ESYNC               CRS_CR_ESYNCIE        /*!< Expected SYNC */
#define RCC_CRS_IT_SYNCERR             CRS_CR_ERRIE          /*!< SYNC error */
#define RCC_CRS_IT_SYNCMISS            CRS_CR_ERRIE          /*!< SYNC missed */
#define RCC_CRS_IT_TRIMOVF             CRS_CR_ERRIE          /*!< Trimming overflow or underflow */
/**
  * @}
  */

/** @defgroup RCCEx_CRS_Flags RCCEx CRS Flags
  * @{
  */
#define RCC_CRS_FLAG_SYNCOK            CRS_ISR_SYNCOKF       /*!< SYNC event OK flag     */
#define RCC_CRS_FLAG_SYNCWARN          CRS_ISR_SYNCWARNF     /*!< SYNC warning flag      */
#define RCC_CRS_FLAG_ERR               CRS_ISR_ERRF          /*!< Error flag        */
#define RCC_CRS_FLAG_ESYNC             CRS_ISR_ESYNCF        /*!< Expected SYNC flag     */
#define RCC_CRS_FLAG_SYNCERR           CRS_ISR_SYNCERR       /*!< SYNC error */
#define RCC_CRS_FLAG_SYNCMISS          CRS_ISR_SYNCMISS      /*!< SYNC missed*/
#define RCC_CRS_FLAG_TRIMOVF           CRS_ISR_TRIMOVF       /*!< Trimming overflow or underflow */
/**
  * @}
  */

#endif /* CRS */

#if defined (RCC_CCIPR4_OTGPDCTRL)
/** @defgroup RCC_LL_EC_OTGPD_SUSPEND OTG PHY Suspend Power State
  * @{
  */
#define RCC_OTGPD_SUSPEND_POWERDOWN   ((uint32_t)0x00000000U)         /*!< Power down the PHY state machine, bias, and PLL in SUSPEND mode. */
#define RCC_OTGPD_SUSPEND_POWERON     RCC_CCIPR4_OTGPDCTRL            /*!< Keep the PHY powered in SUSPEND mode (default after reset). */
/**
  * @}
  */
#endif /* RCC_CCIPR4_OTGPDCTRL */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/** @defgroup RCCEx_Exported_Macros RCCEx Exported Macros
  * @{
  */

/** @brief  Macros to enable or disable the HSI and CSI divided clock for Timers 12, 15 and LPTimer2 input capture.
  * @note   HSI and CSI clocks without division are available when TIMIC is enabled
  * @retval None
  */
#define __HAL_RCC_TIMIC_ENABLE()  SET_BIT(RCC->CCIPR1, RCC_CCIPR1_TIMICSEL)     /*!< HSI/1024, CSI/128 and HSI/8 generation for Timers 12,15 and LPTimer2 Input capture */
#define __HAL_RCC_TIMIC_DISABLE() CLEAR_BIT(RCC->CCIPR1, RCC_CCIPR1_TIMICSEL)   /*!< No clock available for Timers Input capture */

#if defined (RCC_CCIPR4_OTGPDCTRL)
/** @brief  Set the power state of the OTG PHY in SUSPEND mode.
  * @param  _STATE_ the power state of the OTG PHY in SUSPEND mode.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_OTGPD_SUSPEND_POWERDOWN   OTG PHY is powered down when the system is in SUSPEND mode
  *            @arg @ref RCC_OTGPD_SUSPEND_POWERON     OTG PHY is powered down when the system is in SUSPEND mode
  * @retval None
  */
#define __HAL_RCC_OTGPD_CONFIG(_STATE_) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGPDCTRL, (uint32_t)(_STATE_))
#endif /* RCC_CCIPR4_OTGPDCTRL */

/** @brief  Macro to configure the PLL2 clock source.
  * @note   This function must be used only when all PLL2 is disabled.
  * @param  __PLL2SOURCE__: specifies the PLL2 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL2_SOURCE_NONE: No oscillator clock selected as PLL2 clock entry
  *            @arg RCC_PLL2_SOURCE_CSI: CSI oscillator clock selected as PLL2 clock entry
  *            @arg RCC_PLL2_SOURCE_HSI: HSI oscillator clock selected as PLL2 clock entry
  *            @arg RCC_PLL2_SOURCE_HSE: HSE oscillator clock selected as PLL2 clock entry
  *
  */
#define __HAL_RCC_PLL2_PLLSOURCE_CONFIG(__PLL2SOURCE__) MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2SRC, \
                                                                   (__PLL2SOURCE__))

/** @brief  Macro to get the oscillator used as PLL2 clock source.
  * @retval The oscillator used as PLL2 clock source. The returned value can be one
  *         of the following:
  *              - RCC_PLL2_SOURCE_NONE: No oscillator is used as PLL clock source.
  *              - RCC_PLL2_SOURCE_CSI: CSI oscillator is used as PLL clock source.
  *              - RCC_PLL2_SOURCE_HSI: HSI oscillator is used as PLL clock source.
  *              - RCC_PLL2_SOURCE_HSE: HSE oscillator is used as PLL clock source.
  */
#define __HAL_RCC_GET_PLL2_OSCSOURCE() ((uint32_t)(RCC->PLL2CFGR & RCC_PLL2CFGR_PLL2SRC))

/**
  * @brief  Macro to configures the PLL2 source, multiplication and division factors.
  * @note   This function must be used only when PLL2 is disabled.
  *
  * @param  __PLL2SOURCE__: specifies the PLL2 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLL2_SOURCE_NONE  No clock selected as PLL2 clock entry
  *            @arg @ref RCC_PLL2_SOURCE_CSI  CSI oscillator clock selected as PLL2 clock entry
  *            @arg @ref RCC_PLL2_SOURCE_HSI  HSI oscillator clock selected as PLL2 clock entry
  *            @arg @ref RCC_PLL2_SOURCE_HSE  HSE oscillator clock selected as PLL2 clock entry
  *
  * @param  __PLL2M__ specifies the division factor of PLL2 input clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 63.
  *
  * @param  __PLL2N__: specifies the multiplication factor for PLL2 VCO output clock
  *          This parameter must be a number between 4 and 512.
  * @note   You have to set the PLL2N parameter correctly to ensure that the VCO
  *         output frequency is between 192 and 836 MHz (Wide range) or 150 and 420 Mhz (Medium range).
  *         PLL2 clock frequency = f(PLL2) multiplied by PLL2N
  *
  * @param  __PLL2P__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @param  __PLL2Q__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @param  __PLL2R__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_CONFIG(__PLL2SOURCE__, __PLL2M__, __PLL2N__, __PLL2P__, __PLL2Q__, __PLL2R__) \
  do{ \
    MODIFY_REG(RCC->PLL2CFGR, (RCC_PLL2CFGR_PLL2SRC | RCC_PLL2CFGR_PLL2M), \
               ((__PLL2SOURCE__) << RCC_PLL2CFGR_PLL2SRC_Pos) | ((__PLL2M__) << RCC_PLL2CFGR_PLL2M_Pos)); \
    WRITE_REG(RCC->PLL2DIVR , ((((__PLL2N__) - 1U) & RCC_PLL2DIVR_PLL2N) | \
                               ((((__PLL2P__) - 1U) << RCC_PLL2DIVR_PLL2P_Pos) & RCC_PLL2DIVR_PLL2P) |  \
                               ((((__PLL2Q__) - 1U) << RCC_PLL2DIVR_PLL2Q_Pos) & RCC_PLL2DIVR_PLL2Q) |  \
                               ((((__PLL2R__) - 1U) << RCC_PLL2DIVR_PLL2R_Pos) & RCC_PLL2DIVR_PLL2R))); \
  } while(0)
/**
  * @brief  Macro to configure the PLL2 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLL2 is disabled.
  * @note   PLL2 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL2_CONFIG() macro.
  *
  * @param  __PLL2N__ specifies the multiplication factor for PLL2 VCO output clock.
  *          This parameter must be a number between 4 and 512.
  * @note   You have to set the PLL2N parameter correctly to ensure that the VCO
  *         output frequency is between 192 and 836 MHz (Wide range) or 150 and 420 Mhz (Medium range).
  *         PLL2 clock frequency = f(PLL2) multiplied by PLL2N
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_MULN_CONFIG(__PLL2N__) \
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_N2, ((__PLL2N__) - 1U) << RCC_PLL2DIVR_N2_Pos)

/** @brief  Macro to configure the PLL2 input clock division factor M.
  *
  * @note   This function must be used only when the PLL2 is disabled.
  * @note   PLL2 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL2_CONFIG() macro.
  *
  * @param  __PLL2M__ specifies the division factor for PLL2 clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 63.
  *         In order to save power when PLL2 is not used, the value of PLL2M must be set to 0.
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_DIVM_CONFIG(__PLL2M__) \
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_DIVM2, (__PLL2M__) << RCC_PLL2CFGR_DIVM2_Pos)

/** @brief  Macro to configure the PLL2 clock division factor P.
  *
  * @note   This function must be used only when the PLL2 is disabled.
  * @note   PLL2 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL2_CONFIG() macro.
  *
  * @param  __PLL2P__ specifies the division factor for PLL2 output P clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL2 output P clock frequency = f(PLL2) / PLL2P
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_DIVP_CONFIG(__PLL2P__) \
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_P2, ((__PLL2P__) - 1U) << RCC_PLL2DIVR_P2_Pos)

/** @brief  Macro to configure the PLL2 clock division factor Q.
  *
  * @note   This function must be used only when the PLL2 is disabled.
  * @note   PLL2 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL2_CONFIG() macro.
  *
  * @param  __PLL2Q__ specifies the division factor for PLL2 output Q clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL2 output Q clock frequency = f(PLL2) / PLL2Q
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_DIVQ_CONFIG(__PLL2Q__) \
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_Q2, ((__PLL2Q__) - 1U) << RCC_PLL2DIVR_Q2_Pos)

/** @brief  Macro to configure the PLL2 clock division factor R.
  *
  * @note   This function must be used only when the PLL2 is disabled.
  * @note   PLL2 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL2_CONFIG() macro.
  *
  * @param  __PLL2R__ specifies the division factor for PLL2 output R clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL2 output R clock frequency = f(PLL2) / PLL2R
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_DIVR_CONFIG(__PLL2R__) \
  MODIFY_REG(RCC->PLL2DIVR, RCC_PLL2DIVR_R2, ((__PLL2R__) - 1U) << RCC_PLL2DIVR_R2_Pos)

/** @brief  Macros to enable or disable the PLL2.
  * @note   After enabling PLL2, the application software should wait on
  *         PLL2RDY flag to be set indicating that PLL2 clock is stable and can
  *         be used as kernel clock source.
  * @note   The PLL2 is disabled by hardware when entering STOP and STANDBY modes.
  * @retval None
  */
#define __HAL_RCC_PLL2_ENABLE()  SET_BIT(RCC->CR, RCC_CR_PLL2ON)
#define __HAL_RCC_PLL2_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLL2ON)

/**
  * @brief  Enables or disables each clock output (PLL2_P_CLK, PLL2_Q_CLK, PLL2_R_CLK)
  * @note   Enabling/disabling those clocks can be done at any time without the need to stop the PLL2,
  *         This is mainly used to save Power.
  * @param  __PLL2_CLOCKOUT__ specifies the PLL2 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg RCC_PLL2_DIVP: This clock is used to generate an accurate kernel clock to achieve
  *                                high-quality audio performance on SAI interface, SPI/I2S and LPTIM peripherals.
  *            @arg RCC_PLL2_DIVQ: This clock is used to generate kernel clock for the random number generator RNG
  *                                (<=48 MHz), SPI, FDCAN and UART/USART peripherals.
  *            @arg RCC_PLL2_DIVR: This clock is used to generate kernel clock for ADC and DAC peripherals.
  * @retval None
  */
#define __HAL_RCC_PLL2_CLKOUT_ENABLE(__PLL2_CLOCKOUT__)   SET_BIT(RCC->PLL2CFGR, (__PLL2_CLOCKOUT__))
#define __HAL_RCC_PLL2_CLKOUT_DISABLE(__PLL2_CLOCKOUT__)  CLEAR_BIT(RCC->PLL2CFGR, (__PLL2_CLOCKOUT__))

/**
  * @brief  Macro to get the PLL2 clock output enable status.
  * @param  __PLL2_CLOCKOUT__ specifies the PLL2 clock to be output.
  *         This parameter can be one or a combination of the following values:
  *            @arg RCC_PLL2_DIVP: This clock is used to generate an accurate kernel clock to achieve
  *                                high-quality audio performance on SAI interface, SPI/I2S and LPTIM peripherals.
  *            @arg RCC_PLL2_DIVQ: This clock is used to generate kernel clock for the random number generator RNG
  *                                (<=48 MHz), SPI, FDCAN and UART/USART peripherals.
  *            @arg RCC_PLL2_DIVR: This clock is used to generate kernel clock for ADC and DAC peripherals.
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLL2_CLKOUT_CONFIG(__PLL2_CLOCKOUT__)  READ_BIT(RCC->PLL2CFGR, (__PLL2_CLOCKOUT__))

/**
  * @brief  Enables or disables Fractional Part Of The Multiplication Factor of PLL2 VCO
  * @note   Enabling/disabling  Fractional Part can be done at any time without the need to stop the PLL2
  * @retval None
  */
#define __HAL_RCC_PLL2_FRACN_ENABLE()   SET_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2FRACEN)
#define __HAL_RCC_PLL2_FRACN_DISABLE()  CLEAR_BIT(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2FRACEN)

/**
  * @brief  Macro to configures PLL2 clock Fractional Part Of The Multiplication Factor
  *
  * @note   These bits can be written at any time, allowing dynamic fine-tuning of the PLL2 VCO
  *
  * @param  __PLL2FRACN__: Specifies Fractional Part Of The Multiplication factor for PLL2 VCO
  *                           It should be a value between 0 and 8191
  * @note   Warning: the software has to set correctly these bits to insure that the VCO
  *                  output frequency is between its valid frequency range, which is:
  *                  192 to 836 MHz if PLL2VCOSEL = 0
  *                  150 to 420 MHz if PLL2VCOSEL = 1.
  *
  * @retval None
  */
#define  __HAL_RCC_PLL2_FRACN_CONFIG(__PLL2FRACN__) MODIFY_REG(RCC->PLL2FRACR, RCC_PLL2FRACR_PLL2FRACN, \
                                                               (uint32_t)(__PLL2FRACN__) << RCC_PLL2FRACR_PLL2FRACN_Pos)

/** @brief  Macro to select  the PLL2  reference frequency range.
  * @param  __PLL2VCIRange__: specifies the PLL2 input frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL2_VCIRANGE_0: Range frequency is between 1 and 2 MHz
  *            @arg RCC_PLL2_VCIRANGE_1: Range frequency is between 2 and 4 MHz
  *            @arg RCC_PLL2_VCIRANGE_2: Range frequency is between 4 and 8 MHz
  *            @arg RCC_PLL2_VCIRANGE_3: Range frequency is between 8 and 16 MHz
  * @retval None
  */
#define __HAL_RCC_PLL2_VCIRANGE(__PLL2VCIRange__) \
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2RGE, (__PLL2VCIRange__))

/** @brief  Macro to select the PLL2 reference frequency range.
  * @param  __RCC_PLL2VCORange__: specifies the PLL2 output frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL2_VCORANGE_WIDE: Range frequency is between 192 and 836 MHz
  *            @arg RCC_PLL2_VCORANGE_MEDIUM: Range frequency is between 150 and 420 MHz
  *
  * @retval None
  */
#define __HAL_RCC_PLL2_VCORANGE(__RCC_PLL2VCORange__) \
  MODIFY_REG(RCC->PLL2CFGR, RCC_PLL2CFGR_PLL2VCOSEL, (__RCC_PLL2VCORange__))

#if defined(RCC_CR_PLL3ON)
/** @brief  Macro to configure the PLL3 clock source.
  * @note   This function must be used only when all PLL3 is disabled.
  * @param  __PLL3SOURCE__: specifies the PLL3 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL3_SOURCE_NONE: No oscillator clock selected as PLL3 clock entry
  *            @arg RCC_PLL3_SOURCE_CSI: CSI oscillator clock selected as PLL3 clock entry
  *            @arg RCC_PLL3_SOURCE_HSI: HSI oscillator clock selected as PLL3 clock entry
  *            @arg RCC_PLL3_SOURCE_HSE: HSE oscillator clock selected as PLL3 clock entry
  *
  */
#define __HAL_RCC_PLL3_PLLSOURCE_CONFIG(__PLL3SOURCE__) MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3SRC, \
                                                                   (__PLL3SOURCE__))

/** @brief  Macro to get the oscillator used as PLL3 clock source.
  * @retval The oscillator used as PLL3 clock source. The returned value can be one
  *         of the following:
  *              - RCC_PLL3_SOURCE_NONE: No oscillator is used as PLL3 clock source.
  *              - RCC_PLL3_SOURCE_CSI: CSI oscillator is used as PLL3 clock source.
  *              - RCC_PLL3_SOURCE_HSI: HSI oscillator is used as PLL3 clock source.
  *              - RCC_PLL3_SOURCE_HSE: HSE oscillator is used as PLL3 clock source.
  */
#define __HAL_RCC_GET_PLL3_OSCSOURCE() ((uint32_t)(RCC->PLL3CFGR & RCC_PLL3CFGR_PLL3SRC))

/**
  * @brief  Macro to configures the PLL3  source, multiplication and division factors.
  * @note   This function must be used only when PLL3 is disabled.
  *
  * @param  __PLL3SOURCE__: specifies the PLL3 entry clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLL3_SOURCE_NONE  No clock selected as PLL3 clock entry
  *            @arg @ref RCC_PLL3_SOURCE_CSI  CSI oscillator clock selected as PLL3 clock entry
  *            @arg @ref RCC_PLL3_SOURCE_HSI  HSI oscillator clock selected as PLL3 clock entry
  *            @arg @ref RCC_PLL3_SOURCE_HSE  HSE oscillator clock selected as PLL3 clock entry
  *
  * @param  __PLL3M__ specifies the division factor of PLL3 input clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 63.
  *
  * @param  __PLL3N__: specifies the multiplication factor for PLL3 VCO output clock
  *          This parameter must be a number between 4 and 512.
  * @note   You have to set the PLL3N parameter correctly to ensure that the VCO
  *         output frequency is between 150 and 420 MHz (when in medium VCO range)
  *         or between 192 and 836 MHZ (when in wide VCO range)
  *
  * @param  __PLL3P__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @param  __PLL3Q__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @param  __PLL3R__: specifies the division factor for peripheral kernel clocks
  *          This parameter must be a number between 1 and 128
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_CONFIG(__PLL3SOURCE__, __PLL3M__, __PLL3N__, __PLL3P__, __PLL3Q__, __PLL3R__) \
  do{\
    MODIFY_REG(RCC->PLL3CFGR, (RCC_PLL3CFGR_PLL3SRC | RCC_PLL3CFGR_PLL3M), \
               ((__PLL3SOURCE__) << RCC_PLL3CFGR_PLL3SRC_Pos) | ((__PLL3M__) << RCC_PLL3CFGR_PLL3M_Pos)); \
    WRITE_REG(RCC->PLL3DIVR , ( (((__PLL3N__) - 1U) & RCC_PLL3DIVR_PLL3N)   | \
                                ((((__PLL3P__) - 1U) << RCC_PLL3DIVR_PLL3P_Pos) & RCC_PLL3DIVR_PLL3P)  | \
                                ((((__PLL3Q__) - 1U) << RCC_PLL3DIVR_PLL3Q_Pos) & RCC_PLL3DIVR_PLL3Q)  | \
                                ((((__PLL3R__) - 1U) << RCC_PLL3DIVR_PLL3R_Pos) & RCC_PLL3DIVR_PLL3R))); \
  } while(0)

/**
  * @brief  Macro to configure the PLL3 clock multiplication factor N.
  *
  * @note   This function must be used only when the PLL3 is disabled.
  * @note   PLL3 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL3_CONFIG() macro.
  *
  * @param  __PLL3N__ specifies the multiplication factor for PLL3 VCO output clock.
  *          This parameter must be a number between 8 and 86.
  * @note   You have to set the PLL3N parameter correctly to ensure that the VCO
  *         output frequency is between 192 and 836 MHz (Wide range) or 150 and 420 Mhz (Medium range).
  *         PLL3 clock frequency = f(PLL3) multiplied by PLL3N
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_MULN_CONFIG(__PLL3N__) \
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_N3, ((__PLL3N__) - 1U) << RCC_PLL3DIVR_N3_Pos)

/** @brief  Macro to configure the PLL3 input clock division factor M.
  *
  * @note   This function must be used only when the PLL3 is disabled.
  * @note   PLL3 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL3_CONFIG() macro.
  *
  * @param  __PLL3M__ specifies the division factor for PLL3 clock.
  *         This parameter must be a number between Min_Data = 1 and Max_Data = 63.
  *         In order to save power when PLL3 is not used, the value of PLL3M must be set to 0.
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_DIVM_CONFIG(__PLL3M__) \
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_DIVM3,  (__PLL3M__) << RCC_PLL3CFGR_DIVM3_Pos)

/** @brief  Macro to configure the PLL3 clock division factor P.
  *
  * @note   This function must be used only when the PLL3 is disabled.
  * @note   PLL3 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL3_CONFIG() macro.
  *
  * @param  __PLL3P__ specifies the division factor for PLL3 output P clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL3 output P clock frequency = f(PLL3) / PLL3P
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_DIVP_CONFIG(__PLL3P__) \
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_P3, ((__PLL3P__) - 1U) << RCC_PLL3DIVR_P3_Pos)

/** @brief  Macro to configure the PLL3 clock division factor Q.
  *
  * @note   This function must be used only when the PLL3 is disabled.
  * @note   PLL3 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL3_CONFIG() macro.
  *
  * @param  __PLL3Q__ specifies the division factor for PLL3 output Q clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL3 output Q clock frequency = f(PLL3) / PLL3Q
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_DIVQ_CONFIG(__PLL3Q__) \
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_Q3, ((__PLL3Q__) - 1U) << RCC_PLL3DIVR_Q3_Pos)

/** @brief  Macro to configure the PLL3 clock division factor R.
  *
  * @note   This function must be used only when the PLL3 is disabled.
  * @note   PLL3 clock source is independent from the main PLL and is configured through
  *         __HAL_RCC_PLL3_CONFIG() macro.
  *
  * @param  __PLL3R__ specifies the division factor for PLL3 output R clock.
  *         This parameter must be a number in the range (1 to 128).
  *         Use to set PLL3 output R clock frequency = f(PLL3) / PLL3R
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_DIVR_CONFIG(__PLL3R__) \
  MODIFY_REG(RCC->PLL3DIVR, RCC_PLL3DIVR_R3, ((__PLL3R__) - 1U) << RCC_PLL3DIVR_R3_Pos)

/**
  * @brief  Macro to configures  PLL3 clock Fractional Part of The Multiplication Factor
  *
  * @note   These bits can be written at any time, allowing dynamic fine-tuning of the PLL3 VCO
  *
  * @param  __PLL3FRACN__: specifies Fractional Part Of The Multiplication Factor for PLL3 VCO
  *                            It should be a value between 0 and 8191
  * @note   Warning: the software has to set correctly these bits to insure that the VCO
  *                  output frequency is between its valid frequency range, which is:
  *                  192 to 836 MHz if PLL3VCOSEL = 0
  *                  150 to 420 MHz if PLL3VCOSEL = 1.
  *
  * @retval None
  */
#define  __HAL_RCC_PLL3_FRACN_CONFIG(__PLL3FRACN__) MODIFY_REG(RCC->PLL3FRACR, RCC_PLL3FRACR_PLL3FRACN, \
                                                               (uint32_t)(__PLL3FRACN__) << RCC_PLL3FRACR_PLL3FRACN_Pos)

/** @brief  Macro to select  the PLL3  reference frequency range.
  * @param  __PLL3VCIRange__: specifies the PLL3 input frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL3_VCIRANGE_0: Range frequency is between 1 and 2 MHz
  *            @arg RCC_PLL3_VCIRANGE_1: Range frequency is between 2 and 4 MHz
  *            @arg RCC_PLL3_VCIRANGE_2: Range frequency is between 4 and 8 MHz
  *            @arg RCC_PLL3_VCIRANGE_3: Range frequency is between 8 and 16 MHz
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_VCIRANGE(__PLL3VCIRange__) \
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3RGE, (__PLL3VCIRange__))

/** @brief  Macro to select the PLL3 reference frequency range.
  * @param  __RCC_PLL3VCORange__: specifies the PLL3 input frequency range
  *         This parameter can be one of the following values:
  *            @arg RCC_PLL3VCOWIDE: Range frequency is between 192 and 836 MHz
  *            @arg RCC_PLL3VCOMEDIUM: Range frequency is between 150 and 420 MHz
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_VCORANGE(__RCC_PLL3VCORange__) \
  MODIFY_REG(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3VCOSEL, (__RCC_PLL3VCORange__))

/**
  * @brief  Macros to enable or disable the PLL3.
  * @note   The PLL3 is disabled by hardware when entering STOP and STANDBY modes.
  * @retval None
  */

/** @brief  Macros to enable or disable the main PLL3.
  * @note   After enabling  PLL3, the application software should wait on
  *         PLL3RDY flag to be set indicating that PLL3 clock is stable and can
  *         be used as kernel clock source.
  * @note   PLL3 is disabled by hardware when entering STOP and STANDBY modes.
  */
#define __HAL_RCC_PLL3_ENABLE()  SET_BIT(RCC->CR, RCC_CR_PLL3ON)
#define __HAL_RCC_PLL3_DISABLE() CLEAR_BIT(RCC->CR, RCC_CR_PLL3ON)

/**
  * @brief  Enables or disables Fractional Part Of The Multiplication Factor of PLL3 VCO
  * @note   Enabling/disabling Fractional Part can be done at any time without the need to stop the PLL3
  * @retval None
  */
#define __HAL_RCC_PLL3_FRACN_ENABLE()   SET_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3FRACEN)
#define __HAL_RCC_PLL3_FRACN_DISABLE()  CLEAR_BIT(RCC->PLL3CFGR, RCC_PLL3CFGR_PLL3FRACEN)

/**
  * @brief  Enables or disables each clock output (PLL3_P_CLK, PLL3_Q_CLK, PLL3_R_CLK)
  * @note   Enabling/disabling  Those Clocks can be any time  without the need to stop the PLL3,
  *         This is mainly used to save Power.
  * @param  __PLL3_CLOCKOUT__: specifies the PLL3 clock to be outputted
  *          This parameter can be one of the following values:
  *            @arg RCC_PLL3_DIVP: This clock is used to generate an accurate clock to achieve
  *                                high-quality audio performance on SAI and SPI/I2S interfaces.
  *            @arg RCC_PLL3_DIVQ: This clock is used to generate kernel clock for SPI, LPUART, UART/USART
  *                                and USB peripherals.
  *            @arg RCC_PLL3_DIVR: This clock is used to generate kernel clock for I2C, I3C and LPTIM peripherals.
  *
  * @retval None
  */
#define __HAL_RCC_PLL3_CLKOUT_ENABLE(__PLL3_CLOCKOUT__)  SET_BIT(RCC->PLL3CFGR, (__PLL3_CLOCKOUT__))
#define __HAL_RCC_PLL3_CLKOUT_DISABLE(__PLL3_CLOCKOUT__) CLEAR_BIT(RCC->PLL3CFGR, (__PLL3_CLOCKOUT__))

/**
  * @brief  Macro to get clock output enable status (PLL3_SAI2).
  * @param  __PLL3_CLOCKOUT__ specifies the PLL3 clock to be output.
  *          This parameter can be one or a combination of the following values:
  *            @arg RCC_PLL3_DIVP: This clock is used to generate an accurate clock to achieve
  *                                high-quality audio performance on SAI and SPI/I2S interfaces.
  *            @arg RCC_PLL3_DIVQ: This clock is used to generate kernel clock for SPI, LPUART, UART/USART
  *                                and USB peripherals.
  *            @arg RCC_PLL3_DIVR: This clock is used to generate kernel clock for I2C, I3C and LPTIM peripherals.
  *
  * @retval SET / RESET
  */
#define __HAL_RCC_GET_PLL3_CLKOUT_CONFIG(__PLL3_CLOCKOUT__)  READ_BIT(RCC->PLL3CFGR, (__PLL3_CLOCKOUT__))
#endif /* RCC_CR_PLL3ON */

/** @brief  Macro to configure the ADC and DAC kernel clock source.
  * @param  __ADCDAC_CLKSOURCE__ specifies the ADC and DAC kernel clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_ADCDACCLKSOURCE_HCLK   AHB bus clock selected as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_SYSCLK System clock selected as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_PLL2R  PLL2R clock selected as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_HSE    HSE clock selected as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_HSI    HSI clock selected as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_CSI    CSI clock selected as ADC and DAC kernel clock
  * @retval None
  */
#define __HAL_RCC_ADCDAC_CONFIG(__ADCDAC_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ADCDACSEL, (uint32_t)(__ADCDAC_CLKSOURCE__))

/** @brief  Macro to get the ADC and DAC kernel clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_ADCDACCLKSOURCE_HCLK   AHB Bus clock used as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_SYSCLK System clock used as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_PLL2R  PLL2R clock used as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_HSE    HSE oscillator used as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_HSI    HSI oscillator used as ADC and DAC kernel clock
  *            @arg @ref RCC_ADCDACCLKSOURCE_CSI    CSI oscillator used as ADC and DAC kernel clock
  */
#define __HAL_RCC_GET_ADCDAC_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_ADCDACSEL)))

/** @brief  Macro to configure the DAC kernel clock source in low-power mode.
  * @param  __DACLPCLKSOURCE__ specifies the DAC kernel clock source in low-power mode.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_DACLPCLKSOURCE_LSE LSE oscillator selected as DAC kernel clock in low-power mode
  *            @arg @ref RCC_DACLPCLKSOURCE_LSI LSI oscillator selected as DAC kernel clock in low-power mode
  * @retval None
  */
#define __HAL_RCC_DAC_LP_CONFIG(__DACLPCLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_DACSEL, (uint32_t)(__DACLPCLKSOURCE__))

/** @brief  Macro to get the DAC kernel clock source in low-power mode.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_DACLPCLKSOURCE_LSE LSE oscillator used as DAC kernel clock in low-power mode
  *            @arg @ref RCC_DACLPCLKSOURCE_LSI LSI oscillator used as DAC kernel clock in low-power mode
  */
#define __HAL_RCC_GET_DAC_LP_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_DACSEL)))

/** @brief  Macro to configure the FDCAN kernel clock (FDCANCLK).
  *
  * @param  __FDCAN_CLKSOURCE__ specifies the FDCAN kernel clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_FDCANCLKSOURCE_HSE  HSE oscillator selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL1Q  PLL1Q Clock selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL2Q  PLL2Q Clock selected as FDCAN kernel clock
  * @retval None
  */
#define __HAL_RCC_FDCAN_CONFIG(__FDCAN_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_FDCANSEL, (uint32_t)(__FDCAN_CLKSOURCE__))

/** @brief  Macro to get the FDCAN clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_FDCANCLKSOURCE_HSE  HSE oscillator selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL1Q  PLL1Q Clock selected as FDCAN kernel clock
  *            @arg @ref RCC_FDCANCLKSOURCE_PLL2Q  PLL2Q Clock selected as FDCAN kernel clock
  */
#define __HAL_RCC_GET_FDCAN_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_FDCANSEL)))

/** @brief  Macro to configure the LPTIM1 clock (LPTIM1CLK).
  *
  * @param  __LPTIM1_CLKSOURCE__ specifies the LPTIM1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PCLK3  PCLK3 selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PLL2P  PLL2P selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PLL3R  PLL3R selected as LPTIM1 clock (*)
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSE    LSE selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSI    LSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_CLKP   CLKP selected as LPTIM1 clock
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_LPTIM1_CONFIG(__LPTIM1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM1SEL, (uint32_t)(__LPTIM1_CLKSOURCE__))

/** @brief  Macro to get the LPTIM1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PCLK3  PCLK3 selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PLL2P  PLL2P selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_PLL3R  PLL3R selected as LPTIM1 clock (*)
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSE    LSE selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_LSI    LSI selected as LPTIM1 clock
  *            @arg @ref RCC_LPTIM1CLKSOURCE_CLKP   CLKP selected as LPTIM1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_LPTIM1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM1SEL)))

/** @brief  Macro to configure the LPTIM2 clock (LPTIM2CLK).
  *
  * @param  __LPTIM2_CLKSOURCE__ specifies the LPTIM2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PCLK1  PCLK1 selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PLL2P  PLL2P selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PLL3R  PLL3R selected as LPTIM2 clock (*)
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSE    LSE selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSI    LSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_CLKP   CLKP selected as LPTIM2 clock
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_LPTIM2_CONFIG(__LPTIM2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM2SEL, (uint32_t)(__LPTIM2_CLKSOURCE__))

/** @brief  Macro to get the LPTIM2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PCLK1  PCLK1 selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PLL2P  PLL2P selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_PLL3R  PLL3R selected as LPTIM2 clock (*)
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSE    LSE selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_LSI    LSI selected as LPTIM2 clock
  *            @arg @ref RCC_LPTIM2CLKSOURCE_CLKP   CLKP selected as LPTIM2 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_LPTIM2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM2SEL)))

#if defined(LPTIM3)
/** @brief  Macro to configure the LPTIM3 clock (LPTIM3CLK).
  *
  * @param  __LPTIM3_CLKSOURCE__ specifies the LPTIM3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PCLK3  PCLK3 selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PLL2P  PLL2P selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PLL3R  PLL3R selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSE    LSE selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSI    LSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_CLKP   CLKP selected as LPTIM3 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM3_CONFIG(__LPTIM3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM3SEL, (uint32_t)(__LPTIM3_CLKSOURCE__))

/** @brief  Macro to get the LPTIM3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PCLK3  PCLK3 selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PLL2P  PLL2P selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_PLL3R  PLL3R selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSE    LSE selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_LSI    LSI selected as LPTIM3 clock
  *            @arg @ref RCC_LPTIM3CLKSOURCE_CLKP   CLKP selected as LPTIM3 clock
  */
#define __HAL_RCC_GET_LPTIM3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM3SEL)))
#endif /* LPTIM3 */

#if defined(LPTIM4)
/** @brief  Macro to configure the LPTIM4 clock (LPTIM4CLK).
  *
  * @param  __LPTIM4_CLKSOURCE__ specifies the LPTIM4 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PCLK3  PCLK3 selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PLL2P  PLL2P selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PLL3R  PLL3R selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_LSE    LSE selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_LSI    LSI selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_CLKP   CLKP selected as LPTIM4 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM4_CONFIG(__LPTIM4_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM4SEL, (uint32_t)(__LPTIM4_CLKSOURCE__))

/** @brief  Macro to get the LPTIM4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PCLK3  PCLK3 selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PLL2P  PLL2P selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_PLL3R  PLL3R selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_LSE    LSE selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_LSI    LSI selected as LPTIM4 clock
  *            @arg @ref RCC_LPTIM4CLKSOURCE_CLKP   CLKP selected as LPTIM4 clock
  */
#define __HAL_RCC_GET_LPTIM4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM4SEL)))
#endif /* LPTIM4 */

#if defined(LPTIM5)
/** @brief  Macro to configure the LPTIM5 clock (LPTIM5CLK).
  *
  * @param  __LPTIM5_CLKSOURCE__ specifies the LPTIM5 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PCLK3  PCLK3 selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PLL2P  PLL2P selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PLL3R  PLL3R selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_LSE    LSE selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_LSI    LSI selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_CLKP   CLKP selected as LPTIM5 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM5_CONFIG(__LPTIM5_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM5SEL, (uint32_t)(__LPTIM5_CLKSOURCE__))

/** @brief  Macro to get the LPTIM5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PCLK3  PCLK3 selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PLL2P  PLL2P selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_PLL3R  PLL3R selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_LSE    LSE selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_LSI    LSI selected as LPTIM5 clock
  *            @arg @ref RCC_LPTIM5CLKSOURCE_CLKP   CLKP selected as LPTIM5 clock
  */
#define __HAL_RCC_GET_LPTIM5_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM5SEL)))
#endif /* LPTIM5 */

#if defined(LPTIM6)
/** @brief  Macro to configure the LPTIM6 clock (LPTIM6CLK).
  *
  * @param  __LPTIM6_CLKSOURCE__ specifies the LPTIM6 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PCLK3  PCLK3 selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PLL2P  PLL2P selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PLL3R  PLL3R selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_LSE    LSE selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_LSI    LSI selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_CLKP   CLKP selected as LPTIM6 clock
  * @retval None
  */
#define __HAL_RCC_LPTIM6_CONFIG(__LPTIM6_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_LPTIM6SEL, (uint32_t)(__LPTIM6_CLKSOURCE__))

/** @brief  Macro to get the LPTIM6 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PCLK3  PCLK3 selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PLL2P  PLL2P selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_PLL3R  PLL3R selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_LSE    LSE selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_LSI    LSI selected as LPTIM6 clock
  *            @arg @ref RCC_LPTIM6CLKSOURCE_CLKP   CLKP selected as LPTIM6 clock
  */
#define __HAL_RCC_GET_LPTIM6_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_LPTIM6SEL)))
#endif /* LPTIM6 */

/** @brief  macro to configure the SPI1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI1CLKSOURCE_PLL1Q  PLL1Q  selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_PLL2P  PLL2P  selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_PLL3P  PLL3P  selected as SPI1 clock (*)
  *            @arg RCC_SPI1CLKSOURCE_PIN    External Clock selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_CLKP   CLKP  selected as SPI1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_SPI1_CONFIG(__SPI1CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI1SEL, (uint32_t)(__SPI1CLKSource__))

/** @brief  macro to get the SPI1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI1CLKSOURCE_PLL1Q  PLL1Q  selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_PLL2P  PLL2P  selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_PLL3P  PLL3P  selected as SPI1 clock (*)
  *            @arg RCC_SPI1CLKSOURCE_PIN    External Clock selected as SPI1 clock
  *            @arg RCC_SPI1CLKSOURCE_CLKP   CLKP  selected as SPI1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_SPI1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI1SEL)))

/** @brief  macro to configure the SPI2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI2CLKSOURCE_PLL1Q  PLL1Q  selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_PLL2P  PLL2P  selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_PLL3P  PLL3P  selected as SPI2 clock (*)
  *            @arg RCC_SPI2CLKSOURCE_PIN    External Clock selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_CLKP   CLKP  selected as SPI2 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_SPI2_CONFIG(__SPI2CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI2SEL, (uint32_t)(__SPI2CLKSource__))

/** @brief  macro to get the SPI2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI2CLKSOURCE_PLL1Q  PLL1Q  selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_PLL2P  PLL2P  selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_PLL3P  PLL3P  selected as SPI2 clock (*)
  *            @arg RCC_SPI2CLKSOURCE_PIN    External Clock selected as SPI2 clock
  *            @arg RCC_SPI2CLKSOURCE_CLKP   CLKP  selected as SPI2 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_SPI2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI2SEL)))

/** @brief  macro to configure the SPI3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI3CLKSOURCE_PLL1Q  PLL1Q used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_PLL2P  PLL2P used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_PLL3P  PLL3P used as SPI3 clock (*)
  *            @arg RCC_SPI3CLKSOURCE_PIN    External Clock used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_CLKP  CLKP used as SPI3 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_SPI3_CONFIG(__SPI3CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI3SEL, (uint32_t)(__SPI3CLKSource__))

/** @brief  macro to get the SPI3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI3CLKSOURCE_PLL1Q  PLL1Q used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_PLL2P  PLL2P used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_PLL3P  PLL3P used as SPI3 clock (*)
  *            @arg RCC_SPI3CLKSOURCE_PIN    External Clock used as SPI3 clock
  *            @arg RCC_SPI3CLKSOURCE_CLKP  CLKP used as SPI3 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_SPI3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI3SEL)))

#if defined(SPI4)
/** @brief  macro to configure the SPI4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI4CLKSOURCE_PCLK2  PCLK2 used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_PLL2Q  PLL2Q used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_PLL3Q  PLL3Q used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_HSI  HSI used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_CSI  CSI Clock used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_HSE  HSE Clock used as SPI4 clock
  */
#define __HAL_RCC_SPI4_CONFIG(__SPI4CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI4SEL, (uint32_t)(__SPI4CLKSource__))

/** @brief  macro to get the SPI4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI4CLKSOURCE_PCLK2  PCLK2 used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_PLL2Q  PLL2Q used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_PLL3Q  PLL3Q used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_HSI  HSI used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_CSI  CSI used as SPI4 clock
  *            @arg RCC_SPI4CLKSOURCE_HSE  HSE used as SPI4 clock
  */
#define __HAL_RCC_GET_SPI4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI4SEL)))
#endif /* SPI4 */

#if defined(SPI5)
/** @brief  macro to configure the SPI5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI5CLKSOURCE_PCLK3  PCLK3 used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_PLL2Q  PLL2Q used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_PLL3Q  PLL3Q used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_HSI  HSI used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_CSI  CSI Clock used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_HSE  HSE Clock used as SPI5 clock
  */
#define __HAL_RCC_SPI5_CONFIG(__SPI5CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI5SEL, (uint32_t)(__SPI5CLKSource__))

/** @brief  macro to get the SPI5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI5CLKSOURCE_PCLK3  PCLK3 used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_PLL2Q  PLL2Q used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_PLL3Q  PLL3Q used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_HSI  HSI used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_CSI  CSI used as SPI5 clock
  *            @arg RCC_SPI5CLKSOURCE_HSE  HSE used as SPI5 clock
  */
#define __HAL_RCC_GET_SPI5_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI5SEL)))
#endif /* SPI5 */

#if defined(SPI6)
/** @brief  macro to configure the SPI6 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI6CLKSOURCE_PCLK2  PCLK2 used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_PLL2Q  PLL2Q used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_PLL3Q  PLL3Q used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_HSI  HSI used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_CSI  CSI used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_HSE  HSE used as SPI6 clock
  */
#define __HAL_RCC_SPI6_CONFIG(__SPI6CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_SPI6SEL, (uint32_t)(__SPI6CLKSource__))

/** @brief  macro to get the SPI6 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SPI6CLKSOURCE_PCLK2  PCLK2 used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_PLL2Q  PLL2Q used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_PLL3Q  PLL3Q used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_HSI  HSI used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_CSI  CSI used as SPI6 clock
  *            @arg RCC_SPI6CLKSOURCE_HSE  HSE used as SPI6 clock
  */
#define __HAL_RCC_GET_SPI6_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_SPI6SEL)))
#endif /* SPI6 */

/** @brief  Macro to configure the I2C1 clock (I2C1CLK).
  *
  * @param  __I2C1_CLKSOURCE__ specifies the I2C1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C1CLKSOURCE_PCLK1  PCLK1 selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_PLL3R  PLL3R selected as I2C1 clock (*)
  *            @arg @ref RCC_I2C1CLKSOURCE_PLL2R  PLL2R selected as I2C1 clock (**)
  *            @arg @ref RCC_I2C1CLKSOURCE_HSI  HSI selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_CSI  CSI selected as I2C1 clock
  * @retval None
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_I2C1_CONFIG(__I2C1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I2C1SEL, (uint32_t)(__I2C1_CLKSOURCE__))

/** @brief  Macro to get the I2C1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C1CLKSOURCE_PCLK1  PCLK1 selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_PLL3R  PLL3R selected as I2C1 clock (*)
  *            @arg @ref RCC_I2C1CLKSOURCE_PLL2R  PLL2R selected as I2C1 clock (**)
  *            @arg @ref RCC_I2C1CLKSOURCE_HSI  HSI selected as I2C1 clock
  *            @arg @ref RCC_I2C1CLKSOURCE_CSI  CSI selected as I2C1 clock
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_GET_I2C1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I2C1SEL)))

/** @brief  Macro to configure the I2C2 clock (I2C2CLK).
  *
  * @param  __I2C2_CLKSOURCE__ specifies the I2C2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C2CLKSOURCE_PCLK1  PCLK1 selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_PLL3R  PLL3R selected as I2C2 clock (*)
  *            @arg @ref RCC_I2C2CLKSOURCE_PLL2R  PLL2R selected as I2C2 clock (**)
  *            @arg @ref RCC_I2C2CLKSOURCE_HSI  HSI selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_CSI  CSI selected as I2C2 clock
  * @retval None
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_I2C2_CONFIG(__I2C2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I2C2SEL, (uint32_t)(__I2C2_CLKSOURCE__))

/** @brief  Macro to get the I2C2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C2CLKSOURCE_PCLK1  PCLK1 selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_PLL3R  PLL3R selected as I2C2 clock (*)
  *            @arg @ref RCC_I2C2CLKSOURCE_PLL2R  PLL2R selected as I2C2 clock (**)
  *            @arg @ref RCC_I2C2CLKSOURCE_HSI  HSI selected as I2C2 clock
  *            @arg @ref RCC_I2C2CLKSOURCE_CSI  CSI selected as I2C2 clock
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_GET_I2C2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I2C2SEL)))

#if defined(I2C3)
/** @brief  Macro to configure the I2C3 clock (I2C3CLK).
  *
  * @param  __I2C3_CLKSOURCE__ specifies the I2C3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C3CLKSOURCE_PCLK3  PCLK3 selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_PLL3R  PLL3R selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_HSI  HSI selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_CSI  CSI selected as I2C3 clock
  * @retval None
  */
#define __HAL_RCC_I2C3_CONFIG(__I2C3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I2C3SEL, (uint32_t)(__I2C3_CLKSOURCE__))

/** @brief  Macro to get the I2C3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C3CLKSOURCE_PCLK3  PCLK3 selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_PLL3R  PLL3R selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_HSI  HSI selected as I2C3 clock
  *            @arg @ref RCC_I2C3CLKSOURCE_CSI  CSI selected as I2C3 clock
  */
#define __HAL_RCC_GET_I2C3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I2C3SEL)))
#endif /* I2C3 */

#if defined(I2C4)
/** @brief  Macro to configure the I2C4 clock (I2C4CLK).
  *
  * @param  __I2C4_CLKSOURCE__ specifies the I2C4 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I2C4CLKSOURCE_PCLK3  PCLK3 selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_PLL3R  PLL3R selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_HSI    HSI selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_CSI    CSI selected as I2C4 clock
  * @retval None
  */
#define __HAL_RCC_I2C4_CONFIG(__I2C4_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I2C4SEL, (uint32_t)(__I2C4_CLKSOURCE__))

/** @brief  Macro to get the I2C4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I2C4CLKSOURCE_PCLK3  PCLK3 selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_PLL3R  PLL3R selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_HSI    HSI selected as I2C4 clock
  *            @arg @ref RCC_I2C4CLKSOURCE_CSI    CSI selected as I2C4 clock
  */
#define __HAL_RCC_GET_I2C4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I2C4SEL)))
#endif /* I2C4 */

/** @brief  Macro to configure the I3C1 clock (I3C1CLK).
  *
  * @param  __I3C1_CLKSOURCE__ specifies the I3C1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I3C1CLKSOURCE_PCLK1  PCLK1 selected as I3C1 clock
  *            @arg @ref RCC_I3C1CLKSOURCE_PLL3R  PLL3R selected as I3C1 clock (*)
  *            @arg @ref RCC_I3C1CLKSOURCE_PLL2R  PLL2R selected as I3C1 clock (**)
  *            @arg @ref RCC_I3C1CLKSOURCE_HSI    HSI selected as I3C1 clock
  * @retval None
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_I3C1_CONFIG(__I3C1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I3C1SEL, (uint32_t)(__I3C1_CLKSOURCE__))

/** @brief  Macro to get the I3C1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I3C1CLKSOURCE_PCLK1  PCLK1 selected as I3C1 clock
  *            @arg @ref RCC_I3C1CLKSOURCE_PLL3R  PLL3R selected as I3C1 clock (*)
  *            @arg @ref RCC_I3C1CLKSOURCE_PLL2R  PLL2R selected as I3C1 clock (**)
  *            @arg @ref RCC_I3C1CLKSOURCE_HSI    HSI selected as I3C1 clock
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_GET_I3C1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I3C1SEL)))

#if defined(I3C2)
/** @brief  Macro to configure the I3C2 clock (I3C2CLK).
  *
  * @param  __I3C2_CLKSOURCE__ specifies the I3C2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_I3C2CLKSOURCE_PCLK3  PCLK3 selected as I3C2 clock
  *            @arg @ref RCC_I3C2CLKSOURCE_PLL3R  PLL3R selected as I3C2 clock (*)
  *            @arg @ref RCC_I3C2CLKSOURCE_PLL2R  PLL2R selected as I3C2 clock (**)
  *            @arg @ref RCC_I3C2CLKSOURCE_HSI    HSI selected as I3C2 clock
  *            @arg @ref RCC_I3C2CLKSOURCE_CSI    CSI selected as I3C2 clock (*)
  *
  * @retval None
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  *  (**) : Not available for stm32h5exxx and stm32h5fxxx family lines.
  */
#define __HAL_RCC_I3C2_CONFIG(__I3C2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_I3C2SEL, (uint32_t)(__I3C2_CLKSOURCE__))

/** @brief  Macro to get the I3C2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_I3C2CLKSOURCE_PCLK3  PCLK3 selected as I3C2 clock
  *            @arg @ref RCC_I3C2CLKSOURCE_PLL3R  PLL3R selected as I3C2 clock (*)
  *            @arg @ref RCC_I3C2CLKSOURCE_PLL2R  PLL2R selected as I3C2 clock (**)
  *            @arg @ref RCC_I3C2CLKSOURCE_HSI    HSI selected as I3C2 clock
  *            @arg @ref RCC_I3C2CLKSOURCE_CSI    CSI selected as I3C2 clock (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  *  (**) : Not available for stm32h5exxx and stm32h5fxxx family lines.
  */
#define __HAL_RCC_GET_I3C2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_I3C2SEL)))
#endif /* I3C2 */

/** @brief  Macro to configure the USART1 clock (USART1CLK).
  *
  * @param  __USART1_CLKSOURCE__ specifies the USART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART1CLKSOURCE_PCLK2  PCLK2 selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_PLL2Q  PLL2Q selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_PLL3Q  PLL3Q selected as USART1 clock (*)
  *            @arg @ref RCC_USART1CLKSOURCE_HSI  HSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_CSI  CSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_LSE  LSE selected as USART1 clock
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_USART1_CONFIG(__USART1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART1SEL, (uint32_t)(__USART1_CLKSOURCE__))

/** @brief  Macro to get the USART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART1CLKSOURCE_PCLK2  PCLK2 selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_PLL2Q  PLL2Q selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_PLL3Q  PLL3Q selected as USART1 clock (*)
  *            @arg @ref RCC_USART1CLKSOURCE_HSI  HSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_CSI  CSI selected as USART1 clock
  *            @arg @ref RCC_USART1CLKSOURCE_LSE  LSE selected as USART1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_USART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART1SEL)))

/** @brief  Macro to configure the USART2 clock (USART2CLK).
  *
  * @param  __USART2_CLKSOURCE__ specifies the USART2 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART2CLKSOURCE_PCLK1  PCLK2 selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_PLL2Q  PLL2Q selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_PLL3Q  PLL3Q selected as USART2 clock (*)
  *            @arg @ref RCC_USART2CLKSOURCE_HSI  HSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_CSI  CSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_LSE  LSE selected as USART2 clock
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_USART2_CONFIG(__USART2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART2SEL, (uint32_t)(__USART2_CLKSOURCE__))

/** @brief  Macro to get the USART2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART2CLKSOURCE_PCLK1  PCLK2 selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_PLL2Q  PLL2Q selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_PLL3Q  PLL3Q selected as USART2 clock (*)
  *            @arg @ref RCC_USART2CLKSOURCE_HSI  HSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_CSI  CSI selected as USART2 clock
  *            @arg @ref RCC_USART2CLKSOURCE_LSE  LSE selected as USART2 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_USART2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART2SEL)))

/** @brief  Macro to configure the USART3 clock (USART3CLK).
  *
  * @param  __USART3_CLKSOURCE__ specifies the USART3 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART3CLKSOURCE_PCLK1  PCLK2 selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_PLL2Q  PLL2Q selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_PLL3Q  PLL3Q selected as USART3 clock (*)
  *            @arg @ref RCC_USART3CLKSOURCE_HSI  HSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_CSI  CSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_LSE  LSE selected as USART3 clock
  * @retval None
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_USART3_CONFIG(__USART3_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART3SEL, (uint32_t)(__USART3_CLKSOURCE__))

/** @brief  Macro to get the USART3 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART3CLKSOURCE_PCLK1  PCLK2 selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_PLL2Q  PLL2Q selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_PLL3Q  PLL3Q selected as USART3 clock (*)
  *            @arg @ref RCC_USART3CLKSOURCE_HSI  HSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_CSI  CSI selected as USART3 clock
  *            @arg @ref RCC_USART3CLKSOURCE_LSE  LSE selected as USART3 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_USART3_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART3SEL)))

#if defined(UART4)
/** @brief  Macro to configure the UART4 clock (UART4CLK).
  *
  * @param  __UART4_CLKSOURCE__ specifies the UART4 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART4CLKSOURCE_PCLK1  PCLK1 selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_PLL2Q  PLL2Q Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_PLL3Q  PLL3Q Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_HSI   HSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_CSI   CSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_LSE   LSE selected as UART4 clock
  * @retval None
  */
#define __HAL_RCC_UART4_CONFIG(__UART4_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART4SEL, (uint32_t)(__UART4_CLKSOURCE__))

/** @brief  Macro to get the UART4 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART4CLKSOURCE_PCLK1  PCLK1 selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_PLL2Q  PLL2Q Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_PLL3Q  PLL3Q Clock selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_HSI   HSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_CSI   CSI selected as UART4 clock
  *            @arg @ref RCC_UART4CLKSOURCE_LSE   LSE selected as UART4 clock
  */
#define __HAL_RCC_GET_UART4_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART4SEL)))
#endif /* UART4 */

#if defined(UART5)
/** @brief  Macro to configure the UART5 clock (UART5CLK).
  *
  * @param  __UART5_CLKSOURCE__ specifies the UART5 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART5CLKSOURCE_PCLK1  PCLK1 selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_PLL2Q  PLL2Q Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_PLL3Q  PLL3Q Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_HSI   HSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_CSI   CSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_LSE   LSE selected as UART5 clock
  * @retval None
  */
#define __HAL_RCC_UART5_CONFIG(__UART5_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART5SEL, (uint32_t)(__UART5_CLKSOURCE__))

/** @brief  Macro to get the UART5 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART5CLKSOURCE_PCLK1  PCLK1 selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_PLL2Q  PLL2Q Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_PLL3Q  PLL3Q Clock selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_HSI   HSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_CSI   CSI selected as UART5 clock
  *            @arg @ref RCC_UART5CLKSOURCE_LSE   LSE selected as UART5 clock
  */
#define __HAL_RCC_GET_UART5_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART5SEL)))
#endif /* UART5 */

#if defined(USART6)
/** @brief  Macro to configure the USART6 clock (USART6CLK).
  *
  * @param  __USART6_CLKSOURCE__ specifies the USART6 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART6CLKSOURCE_PCLK1  PCLK2 selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_PLL2Q  PLL2Q selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_PLL3Q  PLL3Q selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_HSI  HSI selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_CSI  CSI selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_LSE  LSE selected as USART6 clock
  * @retval None
  */
#define __HAL_RCC_USART6_CONFIG(__USART6_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART6SEL, (uint32_t)(__USART6_CLKSOURCE__))

/** @brief  Macro to get the USART6 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART6CLKSOURCE_PCLK1  PCLK1 selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_PLL2Q  PLL2Q selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_PLL3Q  PLL3Q selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_HSI  HSI selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_CSI  CSI selected as USART6 clock
  *            @arg @ref RCC_USART6CLKSOURCE_LSE  LSE selected as USART6 clock
  */
#define __HAL_RCC_GET_USART6_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART6SEL)))
#endif /* USART6 */

#if defined(UART7)
/** @brief  Macro to configure the UART7 clock (UART7CLK).
  *
  * @param  __UART7_CLKSOURCE__ specifies the UART7 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART7CLKSOURCE_PCLK1  PCLK1 selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_PLL2Q  PLL2Q selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_PLL3Q  PLL3Q selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_HSI  HSI selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_CSI  CSI selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_LSE  LSE selected as UART7 clock
  * @retval None
  */
#define __HAL_RCC_UART7_CONFIG(__UART7_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART7SEL, (uint32_t)(__UART7_CLKSOURCE__))

/** @brief  Macro to get the UART7 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART7CLKSOURCE_PCLK1  PCLK1 selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_PLL2Q  PLL2Q selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_PLL3Q  PLL3Q selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_HSI  HSI selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_CSI  CSI selected as UART7 clock
  *            @arg @ref RCC_UART7CLKSOURCE_LSE  LSE selected as UART7 clock
  */
#define __HAL_RCC_GET_UART7_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART7SEL)))
#endif /* UART5 */

#if defined(UART8)
/** @brief  Macro to configure the UART8 clock (UART8CLK).
  *
  * @param  __UART8_CLKSOURCE__ specifies the UART8 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART8CLKSOURCE_PCLK1  PCLK1 selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_PLL2Q  PLL2Q selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_PLL3Q  PLL3Q selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_HSI  HSI selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_CSI  CSI selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_LSE  LSE selected as UART8 clock
  * @retval None
  */
#define __HAL_RCC_UART8_CONFIG(__UART8_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART8SEL, (uint32_t)(__UART8_CLKSOURCE__))

/** @brief  Macro to get the UART8 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART8CLKSOURCE_PCLK1  PCLK1 selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_PLL2Q  PLL2Q selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_PLL3Q  PLL3Q selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_HSI  HSI selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_CSI  CSI selected as UART8 clock
  *            @arg @ref RCC_UART8CLKSOURCE_LSE  LSE selected as UART8 clock
  */
#define __HAL_RCC_GET_UART8_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART8SEL)))
#endif /* UART8 */

#if defined(UART9)
/** @brief  Macro to configure the UART9 clock (UART9CLK).
  *
  * @param  __UART9_CLKSOURCE__ specifies the UART9 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART9CLKSOURCE_PCLK1  PCLK1 selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_PLL2Q  PLL2Q selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_PLL3Q  PLL3Q selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_HSI  HSI selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_CSI  CSI selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_LSE  LSE selected as UART9 clock
  * @retval None
  */
#define __HAL_RCC_UART9_CONFIG(__UART9_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_UART9SEL, (uint32_t)(__UART9_CLKSOURCE__))

/** @brief  Macro to get the UART9 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART9CLKSOURCE_PCLK1  PCLK1 selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_PLL2Q  PLL2Q selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_PLL3Q  PLL3Q selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_HSI  HSI selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_CSI  CSI selected as UART9 clock
  *            @arg @ref RCC_UART9CLKSOURCE_LSE  LSE selected as UART9 clock
  */
#define __HAL_RCC_GET_UART9_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_UART9SEL)))
#endif /* UART9 */

#if defined(USART10)
/** @brief  Macro to configure the USART10 clock (USART10CLK).
  *
  * @param  __USART10_CLKSOURCE__ specifies the USART10 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART10CLKSOURCE_PCLK1  PCLK1 selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_PLL2Q  PLL2Q selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_PLL3Q  PLL3Q selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_HSI  HSI selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_CSI  CSI selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_LSE  LSE selected as USART10 clock
  * @retval None
  */
#define __HAL_RCC_USART10_CONFIG(__USART10_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR1, RCC_CCIPR1_USART10SEL, (uint32_t)(__USART10_CLKSOURCE__))

/** @brief  Macro to get the USART10 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART10CLKSOURCE_PCLK1  PCLK1 selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_PLL2Q  PLL2Q selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_PLL3Q  PLL3Q selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_HSI  HSI selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_CSI  CSI selected as USART10 clock
  *            @arg @ref RCC_USART10CLKSOURCE_LSE  LSE selected as USART10 clock
  */
#define __HAL_RCC_GET_USART10_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR1, RCC_CCIPR1_USART10SEL)))
#endif /* USART10 */

#if defined(USART11)
/** @brief  Macro to configure the USART11 clock (USART11CLK).
  *
  * @param  __USART11_CLKSOURCE__ specifies the USART11 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_USART11CLKSOURCE_PCLK1  PCLK1 selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_PLL2Q  PLL2Q selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_PLL3Q  PLL3Q selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_HSI  HSI selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_CSI  CSI selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_LSE  LSE selected as USART11 clock
  * @retval None
  */
#define __HAL_RCC_USART11_CONFIG(__USART11_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_USART11SEL, (uint32_t)(__USART11_CLKSOURCE__))

/** @brief  Macro to get the USART11 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_USART11CLKSOURCE_PCLK1  PCLK1 selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_PLL2Q  PLL2Q selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_PLL3Q  PLL3Q selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_HSI  HSI selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_CSI  CSI selected as USART11 clock
  *            @arg @ref RCC_USART11CLKSOURCE_LSE  LSE selected as USART11 clock
  */
#define __HAL_RCC_GET_USART11_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_USART11SEL)))
#endif /* USART11 */

#if defined(UART12)
/** @brief  Macro to configure the UART12 clock (UART12CLK).
  *
  * @param  __UART12_CLKSOURCE__ specifies the UART12 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_UART12CLKSOURCE_PCLK1  PCLK1 selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_PLL2Q  PLL2Q selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_PLL3Q  PLL3Q selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_HSI  HSI selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_CSI  CSI selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_LSE  LSE selected as UART12 clock
  * @retval None
  */
#define __HAL_RCC_UART12_CONFIG(__UART12_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR2, RCC_CCIPR2_UART12SEL, (uint32_t)(__UART12_CLKSOURCE__))

/** @brief  Macro to get the UART12 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_UART12CLKSOURCE_PCLK1  PCLK1 selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_PLL2Q  PLL2Q selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_PLL3Q  PLL3Q selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_HSI  HSI selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_CSI  CSI selected as UART12 clock
  *            @arg @ref RCC_UART12CLKSOURCE_LSE  LSE selected as UART12 clock
  */
#define __HAL_RCC_GET_UART12_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR2, RCC_CCIPR2_UART12SEL)))
#endif /* UART12 */

/** @brief  Macro to configure the LPUART1 clock (LPUART1CLK).
  *
  * @param  __LPUART1_CLKSOURCE__ specifies the LPUART1 clock source.
  *          This parameter can be one of the following values:
  *            @arg @ref RCC_LPUART1CLKSOURCE_PCLK3  PCLK3 selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_PLL2Q  PLL2Q selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_PLL3Q  PLL3Q selected as LPUART1 clock (*)
  *            @arg @ref RCC_LPUART1CLKSOURCE_HSI  HSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_CSI  CSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_LSE  LSE selected as LPUART1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  * @retval None
  */
#define __HAL_RCC_LPUART1_CONFIG(__LPUART1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_LPUART1SEL, (uint32_t)(__LPUART1_CLKSOURCE__))

/** @brief  Macro to get the LPUART1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_LPUART1CLKSOURCE_PCLK3  PCLK3 selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_PLL2Q  PLL2Q selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_PLL3Q  PLL3Q selected as LPUART1 clock (*)
  *            @arg @ref RCC_LPUART1CLKSOURCE_HSI  HSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_CSI  CSI selected as LPUART1 clock
  *            @arg @ref RCC_LPUART1CLKSOURCE_LSE  LSE selected as LPUART1 clock
  *
  *  (*) : For stm32h56xxx and stm32h57xxx family lines.
  */
#define __HAL_RCC_GET_LPUART1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_LPUART1SEL)))

#if defined(OCTOSPI1)
/** @brief  Macro to configure the OctoSPI clock.
  * @param  __OSPI_CLKSOURCE__ specifies the OctoSPI clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_OSPICLKSOURCE_HCLK    HCLK selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL1Q   PLL1Q divider clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL2R   PLL2R divider clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_CLKP    CLKP selected as OctoSPI clock
  * @retval None
  */
#define __HAL_RCC_OSPI_CONFIG(__OSPI_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OCTOSPISEL, (uint32_t)(__OSPI_CLKSOURCE__))

/** @brief  Macro to get the OctoSPI clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_OSPICLKSOURCE_HCLK    HCLK selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL1Q   PLL1Q divider clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_PLL2R   PLL2R divider clock selected as OctoSPI clock
  *            @arg @ref RCC_OSPICLKSOURCE_CLKP    CLKP selected as OctoSPI clock
  */
#define __HAL_RCC_GET_OSPI_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_OCTOSPISEL)))
#endif /* OCTOSPI1 */

#if defined(SDMMC1)
/** @brief  Macro to configure the SDMMC1 clock (SDMMCCLK).
  *
  * @param  __SDMMC1_CLKSOURCE__: specifies the SDMMC1 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_SDMMC1CLKSOURCE_PLL1Q  PLL1Q selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLL2R  PLL2R selected as SDMMC1 clock
  */
#define __HAL_RCC_SDMMC1_CONFIG(__SDMMC1_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_SDMMC1SEL, (uint32_t)(__SDMMC1_CLKSOURCE__))

/** @brief  macro to get the SDMMC1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SDMMC1CLKSOURCE_PLL1Q  PLL1Q selected as SDMMC1 clock
  *            @arg RCC_SDMMC1CLKSOURCE_PLL2R  PLL2R selected as SDMMC1 clock
  */
#define __HAL_RCC_GET_SDMMC1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_SDMMC1SEL)))
#endif /* SDMMC1 */

#if defined(SDMMC2)
/** @brief  Macro to configure the SDMMC2 clock (SDMMCCLK).
  *
  * @param  __SDMMC2_CLKSOURCE__: specifies the SDMMC2 clock source.
  *          This parameter can be one of the following values:
  *            @arg RCC_SDMMC2CLKSOURCE_PLL1Q  PLL1Q selected as SDMMC2 clock
  *            @arg RCC_SDMMC2CLKSOURCE_PLL2R  PLL2R selected as SDMMC2 clock
  */
#define __HAL_RCC_SDMMC2_CONFIG(__SDMMC2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_SDMMC2SEL, (uint32_t)(__SDMMC2_CLKSOURCE__))

/** @brief  macro to get the SDMMC2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_SDMMC2CLKSOURCE_PLL1Q  PLL1Q selected as SDMMC2 clock
  *            @arg RCC_SDMMC2CLKSOURCE_PLL2R  PLL2R selected as SDMMC2 clock
  */
#define __HAL_RCC_GET_SDMMC2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_SDMMC2SEL)))
#endif /*SDMMC2*/

/** @brief macro to configure the RNG clock (RNGCLK).
  *
  * @param  __RNGCLKSource__: specifies the RNG clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_HSI48: HSI48 selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLL1Q: PLL1Q selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_LSE: LSE selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_LSI: LSI selected as RNG clock
  */
#define __HAL_RCC_RNG_CONFIG(__RNGCLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_RNGSEL, (uint32_t)(__RNGCLKSource__))

/** @brief  macro to get the RNG clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_RNGCLKSOURCE_HSI48: HSI48 selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_PLL1Q: PLL1Q selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_LSE: LSE selected as RNG clock
  *            @arg RCC_RNGCLKSOURCE_LSI: LSI selected as RNG clock
  */
#define __HAL_RCC_GET_RNG_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_RNGSEL)))

#if defined(SAI1)
/**
  * @brief  Macro to configure the SAI1 clock source.
  * @param  __SAI1_CLKSOURCE__ defines the SAI1 clock source.
  *          This parameter can be one of the following values:
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL1Q  PLL1Q selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL2P  PLL2Pselected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL3P  PLL3P selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PIN    External clock selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_CLKP   CLKP selected as SAI1 clock
  * @retval None
  */
#define __HAL_RCC_SAI1_CONFIG(__SAI1_CLKSOURCE__)\
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_SAI1SEL, (uint32_t)(__SAI1_CLKSOURCE__))

/** @brief  Macro to get the SAI1 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL1Q  PLL1Q selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL2P  PLL2P selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PLL3P  PLL3P selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_PIN    External clock selected as SAI1 clock
  *             @arg @ref RCC_SAI1CLKSOURCE_CLKP   CLKP selected as SAI1 clock
  */
#define __HAL_RCC_GET_SAI1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_SAI1SEL)))
#endif /* SAI1 */

#if defined(SAI2)
/**
  * @brief  Macro to configure the SAI2 clock source.
  * @param  __SAI2_CLKSOURCE__ defines the SAI2 clock source.
  *          This parameter can be one of the following values:
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL1Q  PLL1Q selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL2P  PLL2P selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL3P  PLL3P selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PIN    External clock selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_CLKP   CLKP selected as SAI2 clock
  * @retval None
  */
#define __HAL_RCC_SAI2_CONFIG(__SAI2_CLKSOURCE__ )\
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_SAI2SEL, (uint32_t)(__SAI2_CLKSOURCE__))

/** @brief  Macro to get the SAI2 clock source.
  * @retval The clock source can be one of the following values:
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL1Q  PLL1Q selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL2P  PLL2P selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PLL3P  PLL3P selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_PIN    External clock selected as SAI2 clock
  *             @arg @ref RCC_SAI2CLKSOURCE_CLKP   CLKP selected as SAI2 clock
  */
#define __HAL_RCC_GET_SAI2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_SAI2SEL)))
#endif /* SAI2 */

/** @brief  Macro to configure the CLKP : Oscillator clock for peripheral
  * @param  __CLKPSource__ specifies Oscillator clock for peripheral
  *         This parameter can be one of the following values:
  *            @arg RCC_CLKPSOURCE_HSI  HSI oscillator selected as clock for peripheral
  *            @arg RCC_CLKPSOURCE_CSI  CSI oscillator selected as clock for peripheral
  *            @arg RCC_CLKPSOURCE_HSE  HSE oscillator selected as clock for peripheral
  */
#define __HAL_RCC_CLKP_CONFIG(__CLKPSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_CKERPSEL, (uint32_t)(__CLKPSource__))

/** @brief  Macro to get the oscillator clock for peripheral clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_CLKPSOURCE_HSI  HSI selected Oscillator clock for peripheral
  *            @arg RCC_CLKPSOURCE_CSI  CSI selected Oscillator clock for peripheral
  *            @arg RCC_CLKPSOURCE_HSE  HSE selected Oscillator clock for peripheral
  */
#define __HAL_RCC_GET_CLKP_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_CKERPSEL)))

#if defined(CEC)
/** @brief  Macro to configure the CEC clock (CECCLK)
  * @param  __CECCLKSource__ specifies the CEC clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_CECCLKSOURCE_LSE  LSE selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_LSI  LSI selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_CSI_DIV122  CSI Divided by 122 selected as CEC clock
  */
#define __HAL_RCC_CEC_CONFIG(__CECCLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_CECSEL, (uint32_t)(__CECCLKSource__))

/** @brief  Macro to get the CEC clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_CECCLKSOURCE_LSE: LSE selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_LSI: LSI selected as CEC clock
  *            @arg RCC_CECCLKSOURCE_CSI_DIV122: CSI Divided by 122 selected as CEC clock
  */
#define __HAL_RCC_GET_CEC_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_CECSEL)))
#endif /* CEC */

#if defined(USB_DRD_FS)
/** @brief  Macro to configure the USB clock (USBCLK).
  * @param  __USBCLKSource__ specifies the USB clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_PLL1Q  PLL1Q selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLL3Q  PLL3Q selected as USB clock (*)
  *            @arg RCC_USBCLKSOURCE_PLL2Q  PLL2Q selected as USB clock (**)
  *            @arg RCC_USBCLKSOURCE_HSI48  HSI48 selected as USB clock
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_USB_CONFIG(__USBCLKSource__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_USBSEL, (uint32_t)(__USBCLKSource__))

/** @brief  Macro to get the USB clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_USBCLKSOURCE_PLL1Q   PLL1Q selected as USB clock
  *            @arg RCC_USBCLKSOURCE_PLL3Q   PLL3Q selected as USB clock (*)
  *            @arg RCC_USBCLKSOURCE_PLL2Q   PLL2Q selected as USB clock (**)
  *            @arg RCC_USBCLKSOURCE_HSI48   HSI48 selected as USB clock
  *
  *  (*)  : For stm32h56xxx and stm32h57xxx family lines.
  *  (**) : For stm32h503xx family line.
  */
#define __HAL_RCC_GET_USB_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_USBSEL)))
#endif /* USB_DRD_FS */

#if defined(PLAY1)
/** @brief  Macro to configure the PLAY1 clock (PLAY1CLK).
  * @param  __PLAY1CLKSource__ specifies the PLAY1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_PLAY1CLKSOURCE_PCLK3  PCLK3 selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_PLL2P  PLL2P selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_PLL3R  PLL3R selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_LSE    LSE selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_LSI    LSI selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_CLKP   CLKP selected as PLAY1 clock
  *
  */
#define __HAL_RCC_PLAY1_CONFIG(__PLAY1CLKSource__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_PLAY1SEL, (uint32_t)(__PLAY1CLKSource__))

/** @brief  Macro to get the PLAY1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_PLAY1CLKSOURCE_PCLK3  PCLK3 selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_PLL2P  PLL2P selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_PLL3R  PLL3R selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_LSE    LSE selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_LSI    LSI selected as PLAY1 clock
  *            @arg RCC_PLAY1CLKSOURCE_CLKP   CLKP selected as PLAY1 clock
  *
  */
#define __HAL_RCC_GET_PLAY1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR3, RCC_CCIPR3_PLAY1SEL)))

/** @brief  Macro to configure the PLAY1 kernel clock divider.
  * @param  __PLAY1_CLKDIV__ specifies the PLAY1 digital interface clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_PLAY1_CLK_DIV_1       PLAY1 kernel clock divided by 1
  *            @arg @ref RCC_PLAY1_CLK_DIV_2       PLAY1 kernel clock divided by 2
  *            @arg @ref RCC_PLAY1_CLK_DIV_4       PLAY1 kernel clock divided by 4
  *            @arg @ref RCC_PLAY1_CLK_DIV_8       PLAY1 kernel clock divided by 8
  *            @arg @ref RCC_PLAY1_CLK_DIV_16      PLAY1 kernel clock divided by 16
  *            @arg @ref RCC_PLAY1_CLK_DIV_32      PLAY1 kernel clock divided by 32
  *            @arg @ref RCC_PLAY1_CLK_DIV_64      PLAY1 kernel clock divided by 64
  *            @arg @ref RCC_PLAY1_CLK_DIV_128     PLAY1 kernel clock divided by 128
  *            @arg @ref RCC_PLAY1_CLK_DIV_256     PLAY1 kernel clock divided by 256
  *            @arg @ref RCC_PLAY1_CLK_DIV_512     PLAY1 kernel clock divided by 512
  *            @arg @ref RCC_PLAY1_CLK_DIV_1024    PLAY1 kernel clock divided by 1024
  *            @arg @ref RCC_PLAY1_CLK_DIV_2048    PLAY1 kernel clock divided by 2048
  *            @arg @ref RCC_PLAY1_CLK_DIV_4096    PLAY1 kernel clock divided by 4096
  *            @arg @ref RCC_PLAY1_CLK_DIV_8192    PLAY1 kernel clock divided by 8192
  *            @arg @ref RCC_PLAY1_CLK_DIV_16384   PLAY1 kernel clock divided by 16384
  *            @arg @ref RCC_PLAY1_CLK_DIV_32768   PLAY1 kernel clock divided by 32768
  * @retval None
  */
#define __HAL_RCC_PLAY1_DIV_CONFIG(__PLAY1_CLKDIV__) \
  MODIFY_REG(RCC->CCIPR3, RCC_CCIPR3_PLAY1DIV, (__PLAY1_CLKDIV__))

/** @brief  Macro to get the PLAY1 kernel clock divider.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_PLAY1_CLK_DIV_1       PLAY1 kernel clock divided by 1
  *            @arg @ref RCC_PLAY1_CLK_DIV_2       PLAY1 kernel clock divided by 2
  *            @arg @ref RCC_PLAY1_CLK_DIV_4       PLAY1 kernel clock divided by 4
  *            @arg @ref RCC_PLAY1_CLK_DIV_8       PLAY1 kernel clock divided by 8
  *            @arg @ref RCC_PLAY1_CLK_DIV_16      PLAY1 kernel clock divided by 16
  *            @arg @ref RCC_PLAY1_CLK_DIV_32      PLAY1 kernel clock divided by 32
  *            @arg @ref RCC_PLAY1_CLK_DIV_64      PLAY1 kernel clock divided by 64
  *            @arg @ref RCC_PLAY1_CLK_DIV_128     PLAY1 kernel clock divided by 128
  *            @arg @ref RCC_PLAY1_CLK_DIV_256     PLAY1 kernel clock divided by 256
  *            @arg @ref RCC_PLAY1_CLK_DIV_512     PLAY1 kernel clock divided by 512
  *            @arg @ref RCC_PLAY1_CLK_DIV_1024    PLAY1 kernel clock divided by 1024
  *            @arg @ref RCC_PLAY1_CLK_DIV_2048    PLAY1 kernel clock divided by 2048
  *            @arg @ref RCC_PLAY1_CLK_DIV_4096    PLAY1 kernel clock divided by 4096
  *            @arg @ref RCC_PLAY1_CLK_DIV_8192    PLAY1 kernel clock divided by 8192
  *            @arg @ref RCC_PLAY1_CLK_DIV_16384   PLAY1 kernel clock divided by 16384
  *            @arg @ref RCC_PLAY1_CLK_DIV_32768   PLAY1 kernel clock divided by 32768
  */
#define __HAL_RCC_GET_PLAY1_DIV()          READ_BIT(RCC->CCIPR3, RCC_CCIPR3_PLAY1DIV)
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
/** @brief  Macro to configure the USB_OTG_FS clock (OTGFSCLK).
  * @param  __OTGFSCLKSource__ specifies the USB_OTG_FS clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_OTGFSCLKSOURCE_PCLK3       PCLK3 selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_PLL2Q       PLL2Q selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_PLL3Q      PLL3Q selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_CLK48       OTG_HS PHY CLK48 output selected as USB_OTG_FS clock (*)
  *
  *  (*) : Available only on STM32H5E5x/STM32H5F5x when OTG_HS PHY is enabled.
  */
#define __HAL_RCC_OTGFS_CONFIG(__OTGFSCLKSource__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGFSSEL, (uint32_t)(__OTGFSCLKSource__))

/** @brief  Macro to get the USB_OTG_FS clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_OTGFSCLKSOURCE_PCLK3       PCLK3 selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_PLL2Q       PLL2Q selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_PLL3Q      PLL3Q selected as USB_OTG_FS clock
  *            @arg RCC_OTGFSCLKSOURCE_CLK48       OTG_HS PHY CLK48 output selected as USB_OTG_FS clock (*)
  *
  *  (*) : Available only on STM32H5E5x/STM32H5F5x when OTG_HS PHY is enabled.
  */
#define __HAL_RCC_GET_OTGFS_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_OTGFSSEL)))
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
/** @brief  Macro to configure the USB_OTG_HS clock (OTGHSCLK).
  * @param  __OTGHSCLKSource__ specifies the USB_OTG_HS clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_OTGHSCLKSOURCE_HSE         HSE selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_PLL3Q       PLL3Q selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_HSE_DIV2    HSE_DIV2 selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_PLL1Q_DIV2  PLL1Q_DIV2 selected as USB_OTG_HS clock
  *
  */
#define __HAL_RCC_OTGHS_CONFIG(__OTGHSCLKSource__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGHSSEL, (uint32_t)(__OTGHSCLKSource__))

/** @brief  Macro to get the USB_OTG_HS clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_OTGHSCLKSOURCE_HSE         HSE selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_PLL3Q       PLL3Q selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_HSE_DIV2    HSE_DIV2 selected as USB_OTG_HS clock
  *            @arg RCC_OTGHSCLKSOURCE_PLL1Q_DIV2  PLL1Q_DIV2 selected as USB_OTG_HS clock
  *
  */
#define __HAL_RCC_GET_OTGHS_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_OTGHSSEL)))
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
/** @brief  Macro to configure the OTGPHY clock (OTGPHYCLK).
  * @param  __OTGPHYCLKSource__ specifies the OTGPHY clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_16M     16Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_19_2M   19.2Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_20M     20Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_24M     24Mhz selected as OTGPHY clockk
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_26M     26Mhz selected as OTGPHY clockk
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_32M     32Mhz selected as OTGPHY clockk
  *
  */
#define __HAL_RCC_OTGPHY_CONFIG(__OTGPHYCLKSource__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_OTGPHYREFCKSEL, (uint32_t)(__OTGPHYCLKSource__))

/** @brief  Macro to get the OTGPHY clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_16M     16Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_19_2M   19.2Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_20M     20Mhz selected as OTGPHY clock
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_24M     24Mhz selected as OTGPHY clockk
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_26M     26Mhz selected as OTGPHY clockk
  *            @arg RCC_OTGPHYREFCKCLKSOURCE_32M     32Mhz selected as OTGPHY clockk
  *
  */
#define __HAL_RCC_GET_OTGPHY_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_OTGPHYREFCKSEL)))
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(OCTOSPI2)
/** @brief  Macro to configure the OctoSPI2 clock.
  * @param  __OSPI2_CLKSOURCE__ specifies the OctoSPI2 clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_OSPI2CLKSOURCE_HCLK    HCLK selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_PLL1Q   PLL1Q divider clock selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_PLL2R   PLL2R divider clock selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_CLKP    CLKP selected as OctoSPI2 clock
  * @retval None
  */
#define __HAL_RCC_OSPI2_CONFIG(__OSPI2_CLKSOURCE__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_OCTOSPI2SEL, (uint32_t)(__OSPI2_CLKSOURCE__))

/** @brief  Macro to get the OctoSPI2 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_OSPI2CLKSOURCE_HCLK    HCLK selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_PLL1Q   PLL1Q divider clock selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_PLL2R   PLL2R divider clock selected as OctoSPI2 clock
  *            @arg @ref RCC_OSPI2CLKSOURCE_CLKP    CLKP selected as OctoSPI2 clock
  */
#define __HAL_RCC_GET_OSPI2_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_OCTOSPI2SEL)))
#endif /* OCTOSPI2 */

#if defined(LTDC)
/** @brief  Macro to configure the LTDC clock (LTDCCLK).
  * @param  __LTDCCLKSource__ specifies the LTDC clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_LTDCCLKSOURCE_PLL3R      PLL3R Clock used as LTDC clock
  *            @arg RCC_LTDCCLKSOURCE_PLL2R      PLL2R Clock used as LTDC clock
  */
#define __HAL_RCC_LTDC_CONFIG(__LTDCCLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_LTDCSEL, (uint32_t)(__LTDCCLKSource__))

/** @brief  Macro to get the LTDC clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_LTDCCLKSOURCE_PLL3R      PLL3R Clock used as LTDC clock
  *            @arg RCC_LTDCCLKSOURCE_PLL2R      PLL2R Clock used as LTDC clock
  *
  */
#define __HAL_RCC_GET_LTDC_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_LTDCSEL)))
#endif /* LTDC */

#if defined(ADF1)
/** @brief  Macro to configure the ADF1 clock (ADF1CLK).
  * @param  __ADF1CLKSource__ specifies the ADF1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_ADF1CLKSOURCE_PLL1Q      PLL1Q Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PLL2P      PLL2P Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PLL3P      PLL3P Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PIN        External Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_CLKP       CLKP Clock used as ADF1 clock
  */
#define __HAL_RCC_ADF1_CONFIG(__ADF1CLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ADF1SEL, (uint32_t)(__ADF1CLKSource__))

/** @brief  Macro to get the ADF1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_ADF1CLKSOURCE_PLL1Q      PLL1Q Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PLL2P      PLL2P Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PLL3P      PLL3P Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_PIN        External Clock used as ADF1 clock
  *            @arg RCC_ADF1CLKSOURCE_CLKP       CLKP Clock used as ADF1 clock
  *
  */
#define __HAL_RCC_GET_ADF1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_ADF1SEL)))
#endif /* ADF1 */

#if defined(MDF1)
/** @brief  Macro to configure the MDF1 clock (MDF1CLK).
  * @param  __MDF1CLKSource__ specifies the MDF1 clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_MDF1CLKSOURCE_PLL1Q      PLL1Q Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PLL2P      PLL2P Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PLL3P      PLL3P Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PIN        External Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_CLKP       CLKP Clock used as MDF1 clock
  */
#define __HAL_RCC_MDF1_CONFIG(__MDF1CLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_MDF1SEL, (uint32_t)(__MDF1CLKSource__))

/** @brief  Macro to get the MDF1 clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_MDF1CLKSOURCE_PLL1Q      PLL1Q Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PLL2P      PLL2P Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PLL3P      PLL3P Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_PIN        External Clock used as MDF1 clock
  *            @arg RCC_MDF1CLKSOURCE_CLKP       CLKP Clock used as MDF1 clock
  *
  */
#define __HAL_RCC_GET_MDF1_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_MDF1SEL)))
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
/** @brief  Macro to configure the ETH clock (ETHCLK).
  * @param  __ETHCLKSource__ specifies the ETH clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_ETHCLKSOURCE_HSE      HSE Clock used as ETH clock
  *            @arg RCC_ETHCLKSOURCE_PLL1Q    PLL1Q Clock used as ETH clock
  */
#define __HAL_RCC_ETH_CONFIG(__ETHCLKSource__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_ETHCLKSEL, (uint32_t)(__ETHCLKSource__))

/** @brief  Macro to get the ETH clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_ETHCLKSOURCE_HSE      HSE Clock used as ETH clock
  *            @arg RCC_ETHCLKSOURCE_PLL1Q    PLL1Q Clock used as ETH clock
  *
  */
#define __HAL_RCC_GET_ETH_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_ETHCLKSEL)))
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
/** @brief  Macro to configure the ETHPTP clock (ETHPTPCLK).
  * @param  __ETHPTPCLKSource__ specifies the ETHPTP clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_ETHPTPCLKSOURCE_HCLK      HCLK Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL1R     PLL1R Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL1Q     PLL1Q Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL3P     PLL3P Clock used as ETHPTP clock
  */
#define __HAL_RCC_ETHPTP_CONFIG(__ETHPTPCLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ETHPTPCLKSEL, (uint32_t)(__ETHPTPCLKSource__))

/** @brief  Macro to get the ETHPTP clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_ETHPTPCLKSOURCE_HCLK      HCLK Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL1R     PLL1R Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL1Q     PLL1Q Clock used as ETHPTP clock
  *            @arg RCC_ETHPTPCLKSOURCE_PLL3P     PLL3P Clock used as ETHPTP clock
  *
  */
#define __HAL_RCC_GET_ETHPTP_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_ETHPTPCLKSEL)))
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR4_ETHPTPDIV)
/** @brief  Macro to configure the ETHPTP kernel clock divider.
  * @param  __ETHPTP_CLKDIV__ specifies the ETHPTP digital interface clock source.
  *         This parameter can be one of the following values:
  *            @arg @ref RCC_ETHPTP_CLK_DIV_1    ETHPTP kernel clock divided by 1
  *            @arg @ref RCC_ETHPTP_CLK_DIV_2    ETHPTP kernel clock divided by 2
  *            @arg @ref RCC_ETHPTP_CLK_DIV_3    ETHPTP kernel clock divided by 3
  *            @arg @ref RCC_ETHPTP_CLK_DIV_4    ETHPTP kernel clock divided by 4
  *            @arg @ref RCC_ETHPTP_CLK_DIV_5    ETHPTP kernel clock divided by 5
  *            @arg @ref RCC_ETHPTP_CLK_DIV_6    ETHPTP kernel clock divided by 6
  *            @arg @ref RCC_ETHPTP_CLK_DIV_7    ETHPTP kernel clock divided by 7
  *            @arg @ref RCC_ETHPTP_CLK_DIV_8    ETHPTP kernel clock divided by 8
  *            @arg @ref RCC_ETHPTP_CLK_DIV_9    ETHPTP kernel clock divided by 9
  *            @arg @ref RCC_ETHPTP_CLK_DIV_10   ETHPTP kernel clock divided by 10
  *            @arg @ref RCC_ETHPTP_CLK_DIV_11   ETHPTP kernel clock divided by 11
  *            @arg @ref RCC_ETHPTP_CLK_DIV_12   ETHPTP kernel clock divided by 12
  *            @arg @ref RCC_ETHPTP_CLK_DIV_13   ETHPTP kernel clock divided by 13
  *            @arg @ref RCC_ETHPTP_CLK_DIV_14   ETHPTP kernel clock divided by 14
  *            @arg @ref RCC_ETHPTP_CLK_DIV_15   ETHPTP kernel clock divided by 15
  *            @arg @ref RCC_ETHPTP_CLK_DIV_16   ETHPTP kernel clock divided by 16
  * @retval None
  */
#define __HAL_RCC_ETHPTP_DIV_CONFIG(__ETHPTP_CLKDIV__) \
  MODIFY_REG(RCC->CCIPR4, RCC_CCIPR4_ETHPTPDIV, (__ETHPTP_CLKDIV__))

/** @brief  Macro to get the ETHPTP kernel clock divider.
  * @retval The clock source can be one of the following values:
  *            @arg @ref RCC_ETHPTP_CLK_DIV_1    ETHPTP kernel clock divided by 1
  *            @arg @ref RCC_ETHPTP_CLK_DIV_2    ETHPTP kernel clock divided by 2
  *            @arg @ref RCC_ETHPTP_CLK_DIV_3    ETHPTP kernel clock divided by 3
  *            @arg @ref RCC_ETHPTP_CLK_DIV_4    ETHPTP kernel clock divided by 4
  *            @arg @ref RCC_ETHPTP_CLK_DIV_5    ETHPTP kernel clock divided by 5
  *            @arg @ref RCC_ETHPTP_CLK_DIV_6    ETHPTP kernel clock divided by 6
  *            @arg @ref RCC_ETHPTP_CLK_DIV_7    ETHPTP kernel clock divided by 7
  *            @arg @ref RCC_ETHPTP_CLK_DIV_8    ETHPTP kernel clock divided by 8
  *            @arg @ref RCC_ETHPTP_CLK_DIV_9    ETHPTP kernel clock divided by 9
  *            @arg @ref RCC_ETHPTP_CLK_DIV_10   ETHPTP kernel clock divided by 10
  *            @arg @ref RCC_ETHPTP_CLK_DIV_11   ETHPTP kernel clock divided by 11
  *            @arg @ref RCC_ETHPTP_CLK_DIV_12   ETHPTP kernel clock divided by 12
  *            @arg @ref RCC_ETHPTP_CLK_DIV_13   ETHPTP kernel clock divided by 13
  *            @arg @ref RCC_ETHPTP_CLK_DIV_14   ETHPTP kernel clock divided by 14
  *            @arg @ref RCC_ETHPTP_CLK_DIV_15   ETHPTP kernel clock divided by 15
  *            @arg @ref RCC_ETHPTP_CLK_DIV_16   ETHPTP kernel clock divided by 16
  */
#define __HAL_RCC_GET_ETHPTP_DIV() \
  ((uint32_t)(READ_BIT(RCC->CCIPR4, RCC_CCIPR4_ETHPTPDIV) >> RCC_CCIPR4_ETHPTPDIV_Pos) + 1)
#endif /* RCC_CCIPR4_ETHPTPDIV */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
/** @brief  Macro to configure the ETHREF clock (ETHREFCLK).
  * @param  __ETHREFCLKSource__ specifies the ETHREF clock source.
  *         This parameter can be one of the following values:
  *            @arg RCC_ETHREFCLKSOURCE_PLL1Q     PLL1Q Clock used as ETHREF clock
  *            @arg RCC_ETHREFCLKSOURCE_PLL1R     PLL1R Clock used as ETHREF clock
  */
#define __HAL_RCC_ETHREF_CONFIG(__ETHREFCLKSource__) \
  MODIFY_REG(RCC->CCIPR5, RCC_CCIPR5_ETHREFCLKSEL, (uint32_t)(__ETHREFCLKSource__))

/** @brief  Macro to get the ETHREF clock source.
  * @retval The clock source can be one of the following values:
  *            @arg RCC_ETHREFCLKSOURCE_PLL1Q     PLL1Q Clock used as ETHREF clock
  *            @arg RCC_ETHREFCLKSOURCE_PLL1R     PLL1R Clock used as ETHREF clock
  *
  */
#define __HAL_RCC_GET_ETHREF_SOURCE() ((uint32_t)(READ_BIT(RCC->CCIPR5, RCC_CCIPR5_ETHREFCLKSEL)))
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

/** @brief  Macro to configure the Timers clocks prescalers
  * @param  __PRESC__  specifies the Timers clocks prescalers selection
  *         This parameter can be one of the following values:
  *            @arg RCC_TIMPRES_DEACTIVATED: The Timers kernels clocks prescaler is
  *                 equal to rcc_hclk1 if PPREx is corresponding to division by 1 or 2,
  *                 else it is equal to 2 x Frcc_pclkx (default after reset)
  *            @arg RCC_TIMPRES_ACTIVATED: The Timers kernels clocks prescaler is
  *                 equal to rcc_hclk1 if PPREx is corresponding to division by 1, 2 or 4,
  *                 else it is equal to 4 x Frcc_pclkx
  */
#define __HAL_RCC_TIMCLKPRESCALER(__PRESC__) do {RCC->CFGR1 &= ~(RCC_CFGR1_TIMPRE);\
                                                  RCC->CFGR1 |= (__PRESC__);       \
                                                }while(0)

#if defined(CRS)

/**
  * @brief  Enable the specified CRS interrupts.
  * @param  __INTERRUPT__ specifies the CRS interrupt sources to be enabled.
  *          This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval None
  */
#define __HAL_RCC_CRS_ENABLE_IT(__INTERRUPT__)   SET_BIT(CRS->CR, (__INTERRUPT__))

/**
  * @brief  Disable the specified CRS interrupts.
  * @param  __INTERRUPT__ specifies the CRS interrupt sources to be disabled.
  *          This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval None
  */
#define __HAL_RCC_CRS_DISABLE_IT(__INTERRUPT__)  CLEAR_BIT(CRS->CR, (__INTERRUPT__))

/** @brief  Check whether the CRS interrupt has occurred or not.
  * @param  __INTERRUPT__ specifies the CRS interrupt source to check.
  *         This parameter can be one of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  * @retval The new state of __INTERRUPT__ (0 or 1).
  */
#define __HAL_RCC_CRS_GET_IT_SOURCE(__INTERRUPT__)  ((READ_BIT(CRS->CR, (__INTERRUPT__)) != 0U) ? 1U : 0U)

/** @brief  Clear the CRS interrupt pending bits
  * @param  __INTERRUPT__ specifies the interrupt pending bit to clear.
  *         This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_IT_SYNCOK  SYNC event OK interrupt
  *              @arg @ref RCC_CRS_IT_SYNCWARN  SYNC warning interrupt
  *              @arg @ref RCC_CRS_IT_ERR  Synchronization or trimming error interrupt
  *              @arg @ref RCC_CRS_IT_ESYNC  Expected SYNC interrupt
  *              @arg @ref RCC_CRS_IT_TRIMOVF  Trimming overflow or underflow interrupt
  *              @arg @ref RCC_CRS_IT_SYNCERR  SYNC error interrupt
  *              @arg @ref RCC_CRS_IT_SYNCMISS  SYNC missed interrupt
  */
/* CRS IT Error Mask */
#define  RCC_CRS_IT_ERROR_MASK                 ((uint32_t)(RCC_CRS_IT_TRIMOVF |\
                                                           RCC_CRS_IT_SYNCERR | RCC_CRS_IT_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_IT(__INTERRUPT__)  do { \
                                                    if(((__INTERRUPT__) & RCC_CRS_IT_ERROR_MASK) != 0U) \
                                                    { \
                                                      WRITE_REG(CRS->ICR, CRS_ICR_ERRC | \
                                                                ((__INTERRUPT__) & ~RCC_CRS_IT_ERROR_MASK)); \
                                                    } \
                                                    else \
                                                    { \
                                                      WRITE_REG(CRS->ICR, (__INTERRUPT__)); \
                                                    } \
                                                  } while(0)

/**
  * @brief  Check whether the specified CRS flag is set or not.
  * @param  __FLAG__ specifies the flag to check.
  *          This parameter can be one of the following values:
  *              @arg @ref RCC_CRS_FLAG_SYNCOK  SYNC event OK
  *              @arg @ref RCC_CRS_FLAG_SYNCWARN  SYNC warning
  *              @arg @ref RCC_CRS_FLAG_ERR  Error
  *              @arg @ref RCC_CRS_FLAG_ESYNC  Expected SYNC
  *              @arg @ref RCC_CRS_FLAG_TRIMOVF  Trimming overflow or underflow
  *              @arg @ref RCC_CRS_FLAG_SYNCERR  SYNC error
  *              @arg @ref RCC_CRS_FLAG_SYNCMISS  SYNC missed
  * @retval The new state of _FLAG_ (TRUE or FALSE).
  */
#define __HAL_RCC_CRS_GET_FLAG(__FLAG__)  (READ_BIT(CRS->ISR, (__FLAG__)) == (__FLAG__))

/**
  * @brief  Clear the CRS specified FLAG.
  * @param __FLAG__ specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *              @arg @ref RCC_CRS_FLAG_SYNCOK  SYNC event OK
  *              @arg @ref RCC_CRS_FLAG_SYNCWARN  SYNC warning
  *              @arg @ref RCC_CRS_FLAG_ERR  Error
  *              @arg @ref RCC_CRS_FLAG_ESYNC  Expected SYNC
  *              @arg @ref RCC_CRS_FLAG_TRIMOVF  Trimming overflow or underflow
  *              @arg @ref RCC_CRS_FLAG_SYNCERR  SYNC error
  *              @arg @ref RCC_CRS_FLAG_SYNCMISS  SYNC missed
  * @note RCC_CRS_FLAG_ERR clears RCC_CRS_FLAG_TRIMOVF, RCC_CRS_FLAG_SYNCERR, RCC_CRS_FLAG_SYNCMISS
          and consequently RCC_CRS_FLAG_ERR
  * @retval None
  */

/* CRS Flag Error Mask */
#define RCC_CRS_FLAG_ERROR_MASK                ((uint32_t)(RCC_CRS_FLAG_TRIMOVF |\
                                                           RCC_CRS_FLAG_SYNCERR | RCC_CRS_FLAG_SYNCMISS))

#define __HAL_RCC_CRS_CLEAR_FLAG(__FLAG__)     do { \
                                                    if(((__FLAG__) & RCC_CRS_FLAG_ERROR_MASK) != 0U) \
                                                    { \
                                                      WRITE_REG(CRS->ICR, CRS_ICR_ERRC | \
                                                                ((__FLAG__) & ~RCC_CRS_FLAG_ERROR_MASK)); \
                                                    } \
                                                    else \
                                                    { \
                                                      WRITE_REG(CRS->ICR, (__FLAG__)); \
                                                    } \
                                                  } while(0)

/**
  * @}
  */

/** @defgroup RCCEx_CRS_Extended_Features RCCEx CRS Extended Features
  * @{
  */
/**
  * @brief  Enable the oscillator clock for frequency error counter.
  * @note   when the CEN bit is set the CRS_CFGR register becomes write-protected.
  * @retval None
  */
#define __HAL_RCC_CRS_FREQ_ERROR_COUNTER_ENABLE()  SET_BIT(CRS->CR, CRS_CR_CEN)

/**
  * @brief  Disable the oscillator clock for frequency error counter.
  * @retval None
  */
#define __HAL_RCC_CRS_FREQ_ERROR_COUNTER_DISABLE() CLEAR_BIT(CRS->CR, CRS_CR_CEN)

/**
  * @brief  Enable the automatic hardware adjustment of TRIM bits.
  * @note   When the AUTOTRIMEN bit is set the CRS_CFGR register becomes write-protected.
  * @retval None
  */
#define __HAL_RCC_CRS_AUTOMATIC_CALIB_ENABLE()     SET_BIT(CRS->CR, CRS_CR_AUTOTRIMEN)

/**
  * @brief  Enable or disable the automatic hardware adjustment of TRIM bits.
  * @retval None
  */
#define __HAL_RCC_CRS_AUTOMATIC_CALIB_DISABLE()    CLEAR_BIT(CRS->CR, CRS_CR_AUTOTRIMEN)

/**
  * @brief  Macro to calculate reload value to be set in CRS register according to target and sync frequencies
  * @note   The RELOAD value should be selected according to the ratio between the target frequency and the frequency
  *             of the synchronization source after prescaling. It is then decreased by one in order to
  *             reach the expected synchronization on the zero value. The formula is the following:
  *             RELOAD = (fTARGET / fSYNC) -1
  * @param  __FTARGET__ Target frequency (value in Hz)
  * @param  __FSYNC__ Synchronization signal frequency (value in Hz)
  * @retval None
  */
#define __HAL_RCC_CRS_RELOADVALUE_CALCULATE(__FTARGET__, __FSYNC__)  (((__FTARGET__) / (__FSYNC__)) - 1U)


/**
  * @}
  */

#endif /* CRS */

/* Private constants ---------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Constants
  * @{
  */
/* Define used for IS_RCC_* macros below */
#if defined(USB_OTG_HS)
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 | RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_UART7 | RCC_PERIPHCLK_UART8 | RCC_PERIPHCLK_UART9 | \
                                         RCC_PERIPHCLK_USART10 | RCC_PERIPHCLK_USART11 | RCC_PERIPHCLK_UART12 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I2C4 | RCC_PERIPHCLK_I3C1 | \
                                         RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | \
                                         RCC_PERIPHCLK_LPTIM3 | RCC_PERIPHCLK_LPTIM4 | RCC_PERIPHCLK_LPTIM5 | \
                                         RCC_PERIPHCLK_LPTIM6 | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI2 | \
                                         RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_SDMMC2 | \
                                         RCC_PERIPHCLK_SPI1 |  RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | \
                                         RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_SPI5 | RCC_PERIPHCLK_SPI6 | \
                                         RCC_PERIPHCLK_OSPI | RCC_PERIPHCLK_FDCAN | RCC_PERIPHCLK_CEC  | \
                                         RCC_PERIPHCLK_CKPER | RCC_PERIPHCLK_PLAY1 | RCC_PERIPHCLK_I3C2 | \
                                         RCC_PERIPHCLK_OTGFS | RCC_PERIPHCLK_OTGHS | RCC_PERIPHCLK_OSPI2 | \
                                         RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_ADF1 | RCC_PERIPHCLK_MDF1 | \
                                         RCC_PERIPHCLK_ETH |RCC_PERIPHCLK_ETHPTP | RCC_PERIPHCLK_ETHREF)
#elif defined(PLAY1)
#if defined(RCC_PERIPHCLK_UART9)
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 | RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_UART7 | RCC_PERIPHCLK_UART8 | RCC_PERIPHCLK_UART9 | \
                                         RCC_PERIPHCLK_USART10 | RCC_PERIPHCLK_USART11 | RCC_PERIPHCLK_UART12 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I2C4 | RCC_PERIPHCLK_I3C1 | \
                                         RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | \
                                         RCC_PERIPHCLK_LPTIM3 | RCC_PERIPHCLK_LPTIM4 | RCC_PERIPHCLK_LPTIM5 | \
                                         RCC_PERIPHCLK_LPTIM6 | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI2 | \
                                         RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_SDMMC2 | \
                                         RCC_PERIPHCLK_SPI1 |  RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | \
                                         RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_SPI5 | RCC_PERIPHCLK_SPI6 | \
                                         RCC_PERIPHCLK_OSPI | RCC_PERIPHCLK_FDCAN | RCC_PERIPHCLK_CEC  | \
                                         RCC_PERIPHCLK_CKPER | RCC_PERIPHCLK_PLAY1 | RCC_PERIPHCLK_I3C2 | \
                                         RCC_PERIPHCLK_OSPI2 | RCC_PERIPHCLK_OTGFS | RCC_PERIPHCLK_LTDC | \
                                         RCC_PERIPHCLK_ADF1 | RCC_PERIPHCLK_MDF1 | RCC_PERIPHCLK_ETH | \
                                         RCC_PERIPHCLK_ETHPTP | RCC_PERIPHCLK_ETHREF)
#else
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 | RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_UART7 | RCC_PERIPHCLK_UART8 | RCC_PERIPHCLK_LPUART1|\
                                         RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_I2C3 |  \
                                         RCC_PERIPHCLK_I3C1 | RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | \
                                         RCC_PERIPHCLK_LPTIM2 | RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | \
                                         RCC_PERIPHCLK_RNG | RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_SDMMC1 | \
                                         RCC_PERIPHCLK_SPI1 |  RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | \
                                         RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_OSPI | RCC_PERIPHCLK_FDCAN | \
                                         RCC_PERIPHCLK_CEC  | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_CKPER  | \
                                         RCC_PERIPHCLK_PLAY1 | RCC_PERIPHCLK_I3C2 | RCC_PERIPHCLK_ETH  | \
                                         RCC_PERIPHCLK_ETHPTP | RCC_PERIPHCLK_ETHREF)
#endif /* RCC_PERIPHCLK_UART9 */
#elif defined(SDMMC2)
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 | RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_UART7 | RCC_PERIPHCLK_UART8 | RCC_PERIPHCLK_UART9 | \
                                         RCC_PERIPHCLK_USART10 | RCC_PERIPHCLK_USART11 | RCC_PERIPHCLK_UART12 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I2C4 | RCC_PERIPHCLK_I3C1 | \
                                         RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | \
                                         RCC_PERIPHCLK_LPTIM3 | RCC_PERIPHCLK_LPTIM4 | RCC_PERIPHCLK_LPTIM5 | \
                                         RCC_PERIPHCLK_LPTIM6 | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI2 | \
                                         RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_SDMMC2 | \
                                         RCC_PERIPHCLK_SPI1 |  RCC_PERIPHCLK_SPI2 | \
                                         RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI4 | RCC_PERIPHCLK_SPI5 | \
                                         RCC_PERIPHCLK_SPI6 | RCC_PERIPHCLK_OSPI | RCC_PERIPHCLK_FDCAN | \
                                         RCC_PERIPHCLK_CEC | RCC_PERIPHCLK_USB | RCC_PERIPHCLK_CKPER)
#elif defined(UART7)
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 |  RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_UART7 | RCC_PERIPHCLK_UART8 |  RCC_PERIPHCLK_UART9 | \
                                         RCC_PERIPHCLK_USART10 | RCC_PERIPHCLK_USART11 | RCC_PERIPHCLK_UART12 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 |  RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I2C4 | RCC_PERIPHCLK_I3C1 | \
                                         RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | \
                                         RCC_PERIPHCLK_LPTIM3 | RCC_PERIPHCLK_LPTIM4 | RCC_PERIPHCLK_LPTIM5 | \
                                         RCC_PERIPHCLK_LPTIM6 | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_SAI2 | \
                                         RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_RTC |  RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_SPI1 | \
                                         RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI4 | \
                                         RCC_PERIPHCLK_SPI5 | RCC_PERIPHCLK_SPI6 | RCC_PERIPHCLK_OSPI | \
                                         RCC_PERIPHCLK_FDCAN | RCC_PERIPHCLK_CEC | RCC_PERIPHCLK_USB | \
                                         RCC_PERIPHCLK_CKPER)
#elif defined(USART6)
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_UART5 |  RCC_PERIPHCLK_USART6 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 |  RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I2C3 | RCC_PERIPHCLK_I3C1 | RCC_PERIPHCLK_I3C2 | \
                                         RCC_PERIPHCLK_TIM | RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | \
                                         RCC_PERIPHCLK_ADCDAC | RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | \
                                         RCC_PERIPHCLK_RTC |  RCC_PERIPHCLK_SDMMC1 | RCC_PERIPHCLK_SPI1 | \
                                         RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | RCC_PERIPHCLK_SPI4 | \
                                         RCC_PERIPHCLK_OSPI | RCC_PERIPHCLK_FDCAN | RCC_PERIPHCLK_CEC | \
                                         RCC_PERIPHCLK_USB | RCC_PERIPHCLK_CKPER)
#else
#define RCC_PERIPHCLOCK_ALL             (RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USART3 | \
                                         RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_I2C2 | \
                                         RCC_PERIPHCLK_I3C1 | RCC_PERIPHCLK_I3C2 | RCC_PERIPHCLK_TIM | \
                                         RCC_PERIPHCLK_LPTIM1 | RCC_PERIPHCLK_LPTIM2 | RCC_PERIPHCLK_ADCDAC | \
                                         RCC_PERIPHCLK_DAC_LP | RCC_PERIPHCLK_RNG | RCC_PERIPHCLK_RTC | \
                                         RCC_PERIPHCLK_SPI1 | RCC_PERIPHCLK_SPI2 | RCC_PERIPHCLK_SPI3 | \
                                         RCC_PERIPHCLK_FDCAN |  RCC_PERIPHCLK_USB | RCC_PERIPHCLK_CKPER)
#endif /*FDCAN2 && SDMMC2 */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup RCCEx_Private_Macros
  * @{
  */

#define IS_RCC_PLL2_SOURCE(SOURCE) (((SOURCE) == RCC_PLL2_SOURCE_CSI)  || \
                                    ((SOURCE) == RCC_PLL2_SOURCE_HSI)  || \
                                    ((SOURCE) == RCC_PLL2_SOURCE_HSE))

#define IS_RCC_PLL2_DIVM_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 63U))
#define IS_RCC_PLL2_MULN_VALUE(VALUE) ((4U <= (VALUE)) && ((VALUE) <= 512U))
#define IS_RCC_PLL2_DIVP_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL2_DIVQ_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL2_DIVR_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))

#define IS_RCC_PLL2_FRACN_VALUE(VALUE) ((VALUE) <= 8191U)

#define IS_RCC_PLL2_VCIRGE_VALUE(VALUE) (((VALUE) == RCC_PLL2_VCIRANGE_0) || \
                                         ((VALUE) == RCC_PLL2_VCIRANGE_1) || \
                                         ((VALUE) == RCC_PLL2_VCIRANGE_2) || \
                                         ((VALUE) == RCC_PLL2_VCIRANGE_3))

#define IS_RCC_PLL2_VCORGE_VALUE(VALUE) (((VALUE) == RCC_PLL2_VCORANGE_WIDE) || ((VALUE) == RCC_PLL2_VCORANGE_MEDIUM))

#define IS_RCC_PLL2_CLOCKOUT_VALUE(VALUE) ((0x00010000U <= (VALUE)) && ((VALUE) <= 0x00070000U))

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_PLL3_SOURCE(SOURCE) (((SOURCE) == RCC_PLL3_SOURCE_CSI)  || \
                                    ((SOURCE) == RCC_PLL3_SOURCE_HSI)  || \
                                    ((SOURCE) == RCC_PLL3_SOURCE_HSE))

#define IS_RCC_PLL3_VCIRGE_VALUE(VALUE) (((VALUE) == RCC_PLL3_VCIRANGE_0) || \
                                         ((VALUE) == RCC_PLL3_VCIRANGE_1) || \
                                         ((VALUE) == RCC_PLL3_VCIRANGE_2) || \
                                         ((VALUE) == RCC_PLL3_VCIRANGE_3))

#define IS_RCC_PLL3_DIVM_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 63U))
#define IS_RCC_PLL3_MULN_VALUE(VALUE) ((4U <= (VALUE)) && ((VALUE) <= 512U))
#define IS_RCC_PLL3_DIVP_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL3_DIVQ_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))
#define IS_RCC_PLL3_DIVR_VALUE(VALUE) ((1U <= (VALUE)) && ((VALUE) <= 128U))

#define IS_RCC_PLL3_FRACN_VALUE(VALUE) ((VALUE) <= 8191U)

#define IS_RCC_PLL3_VCORGE_VALUE(VALUE) (((VALUE) == RCC_PLL3_VCORANGE_WIDE) || ((VALUE) == RCC_PLL3_VCORANGE_MEDIUM))

#define IS_RCC_PLL3_CLOCKOUT_VALUE(VALUE) ((0x00010000U <= (VALUE)) && ((VALUE) <= 0x00070000U))

#endif /* RCC_CR_PLL3ON */

#define IS_RCC_LSCOSOURCE(__SOURCE__) (((__SOURCE__) == RCC_LSCOSOURCE_LSI) || \
                                       ((__SOURCE__) == RCC_LSCOSOURCE_LSE))

#define IS_RCC_CLKPSOURCE(SOURCE)   (((SOURCE) == RCC_CLKPSOURCE_HSI)  || \
                                     ((SOURCE) == RCC_CLKPSOURCE_CSI) || \
                                     ((SOURCE) == RCC_CLKPSOURCE_HSE))

#define IS_RCC_PERIPHCLOCK(__SELECTION__)  ((((__SELECTION__) & RCC_PERIPHCLOCK_ALL) != ((uint64_t)0x00)) && \
                                            (((__SELECTION__) & ~RCC_PERIPHCLOCK_ALL) == ((uint64_t)0x00)))
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_USART1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART1CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_LSE))

#define IS_RCC_USART2CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_LSE))

#define IS_RCC_USART3CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART3CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_LSE))

#else
#define IS_RCC_USART1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART1CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART1CLKSOURCE_LSE))

#define IS_RCC_USART2CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART2CLKSOURCE_LSE))

#define IS_RCC_USART3CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART3CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART3CLKSOURCE_LSE))

#endif /* RCC_CR_PLL3ON */

#if defined(UART4)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_UART4CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART4CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_LSE))
#else
#define IS_RCC_UART4CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART4CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART4CLKSOURCE_LSE))
#endif /* RCC_CR_PLL3ON */
#endif /* UART4 */

#if defined(UART5)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_UART5CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART5CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_LSE))
#else
#define IS_RCC_UART5CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART5CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART5CLKSOURCE_LSE))
#endif /* RCC_CR_PLL3ON */
#endif /* UART5 */

#if defined(USART6)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_USART6CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART6CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_LSE))
#else
#define IS_RCC_USART6CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART6CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART6CLKSOURCE_LSE))
#endif /* RCC_CR_PLL3ON */
#endif /* USART6 */

#if defined(UART7)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_UART7CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART7CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_LSE))
#else
#define IS_RCC_UART7CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART7CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART7CLKSOURCE_LSE))
#endif /* RCC_CR_PLL3ON */
#endif /* UART7 */

#if defined(UART8)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_UART8CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART8CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_LSE))
#else
#define IS_RCC_UART8CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART8CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART8CLKSOURCE_LSE))
#endif /* RCC_CR_PLL3ON */
#endif /* UART8 */

#if defined(UART9)
#define IS_RCC_UART9CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART9CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART9CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART9CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART9CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART9CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART9CLKSOURCE_LSE))
#endif /* UART9 */

#if defined(USART10)
#define IS_RCC_USART10CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART10CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART10CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART10CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART10CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART10CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART10CLKSOURCE_LSE))
#endif /* USART10 */

#if defined(USART11)
#define IS_RCC_USART11CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_USART11CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_USART11CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USART11CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USART11CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_USART11CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_USART11CLKSOURCE_LSE))
#endif /* USART11 */

#if defined(UART12)
#define IS_RCC_UART12CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_UART12CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_UART12CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_UART12CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_UART12CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_UART12CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_UART12CLKSOURCE_LSE))
#endif /* UART12 */

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_LPUART1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPUART1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_HSI) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_LSE) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_CSI))

#else
#define IS_RCC_LPUART1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPUART1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_HSI) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_LSE) || \
   ((__SOURCE__) == RCC_LPUART1CLKSOURCE_CSI))

#endif /* RCC_CR_PLL3ON */

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_I2C1CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C1CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_CSI))

#define IS_RCC_I2C2CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_CSI))

#else
#define IS_RCC_I2C1CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C1CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_I2C1CLKSOURCE_CSI))

#define IS_RCC_I2C2CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_I2C2CLKSOURCE_CSI))

#endif /* RCC_CR_PLL3ON */


#if defined(I2C3)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_I2C3CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C3CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_HSI  ) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_CSI))
#else
#define IS_RCC_I2C3CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C3CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_HSI  ) || \
   ((__SOURCE__) == RCC_I2C3CLKSOURCE_CSI))
#endif /* RCC_CR_PLL3ON */
#endif /* I2C3 */

#if defined(I2C4)
#define IS_RCC_I2C4CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I2C4CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I2C4CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I2C4CLKSOURCE_HSI  ) || \
   ((__SOURCE__) == RCC_I2C4CLKSOURCE_CSI))
#endif /* I2C4 */

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_I3C1CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I3C1CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I3C1CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I3C1CLKSOURCE_HSI))

#else
#define IS_RCC_I3C1CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I3C1CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_I3C1CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_I3C1CLKSOURCE_HSI))

#endif /* RCC_CR_PLL3ON */

#if defined(I3C2)
#if defined(STM32H5E5xx) || defined(STM32H5E4xx) || defined(STM32H5F5xx) || defined(STM32H5F4xx)
#define IS_RCC_I3C2CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I3C2CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_HSI))
#else
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_I3C2CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I3C2CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_HSI))
#else
#define IS_RCC_I3C2CLKSOURCE(__SOURCE__)   \
  (((__SOURCE__) == RCC_I3C2CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_I3C2CLKSOURCE_HSI))
#endif /* PLL3 */
#endif /* STM32H5E5xx || STM32H5E4xx || STM32H5F5xx || STM32H5F4xx */
#endif /* I3C2 */

#if defined(SAI1)
#define IS_RCC_SAI1CLK(__SOURCE__)   \
  (((__SOURCE__) == RCC_SAI1CLKSOURCE_PLL1Q)|| \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLL2P)|| \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PLL3P)|| \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_PIN)  || \
   ((__SOURCE__) == RCC_SAI1CLKSOURCE_CLKP))
#endif /* SAI1 */

#if defined(SAI2)
#define IS_RCC_SAI2CLK(__SOURCE__)   \
  (((__SOURCE__) == RCC_SAI2CLKSOURCE_PLL1Q)|| \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLL2P)|| \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PLL3P)|| \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_PIN)  || \
   ((__SOURCE__) == RCC_SAI2CLKSOURCE_CLKP))
#endif /* SAI2 */

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_LPTIM1CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_CLKP))

#define IS_RCC_LPTIM2CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_CLKP))

#else
#define IS_RCC_LPTIM1CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_PLL2P)  || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM1CLKSOURCE_CLKP))

#define IS_RCC_LPTIM2CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PCLK1) || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_PLL2P)  || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM2CLKSOURCE_CLKP))

#endif /* RCC_CR_PLL3ON */

#if defined(LPTIM3)
#define IS_RCC_LPTIM3CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM3CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_PLL2P)  || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_PLL3R)  || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM3CLKSOURCE_CLKP))
#endif /* LPTIM3 */

#if defined(LPTIM4)
#define IS_RCC_LPTIM4CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM4CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM4CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_LPTIM4CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LPTIM4CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM4CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM4CLKSOURCE_CLKP))
#endif /* LPTIM4 */

#if defined(LPTIM5)
#define IS_RCC_LPTIM5CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM5CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM5CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_LPTIM5CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LPTIM5CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM5CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM5CLKSOURCE_CLKP))
#endif /* LPTIM5 */

#if defined(LPTIM6)
#define IS_RCC_LPTIM6CLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_LPTIM6CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_LPTIM6CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_LPTIM6CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LPTIM6CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_LPTIM6CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_LPTIM6CLKSOURCE_CLKP))
#endif /* LPTIM6 */

#define IS_RCC_FDCANCLK(__SOURCE__)  \
  (((__SOURCE__) == RCC_FDCANCLKSOURCE_HSE)   || \
   ((__SOURCE__) == RCC_FDCANCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_FDCANCLKSOURCE_PLL2Q))

#if defined(SDMMC1)
#define IS_RCC_SDMMC1CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SDMMC1CLKSOURCE_PLL2R))
#endif /* SDMMC1 */

#if defined(SDMMC2)
#define IS_RCC_SDMMC2CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_SDMMC2CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SDMMC2CLKSOURCE_PLL2R))
#endif /*SDMMC2*/

#define IS_RCC_RNGCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_RNGCLKSOURCE_HSI48)  || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_PLL1Q)  || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_LSE)    || \
   ((__SOURCE__) == RCC_RNGCLKSOURCE_LSI))

#define IS_RCC_ADCDACCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_ADCDACCLKSOURCE_HCLK)   || \
   ((__SOURCE__) == RCC_ADCDACCLKSOURCE_SYSCLK) || \
   ((__SOURCE__) == RCC_ADCDACCLKSOURCE_PLL2R)  || \
   ((__SOURCE__) == RCC_ADCDACCLKSOURCE_HSE)    || \
   ((__SOURCE__) == RCC_ADCDACCLKSOURCE_HSI)    || \
   ((__SOURCE__) == RCC_ADCDACCLKSOURCE_CSI))

#define IS_RCC_DACLPCLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_DACLPCLKSOURCE_LSI)  || \
   ((__SOURCE__) == RCC_DACLPCLKSOURCE_LSE))

#if defined(OCTOSPI1)
#define IS_RCC_OSPICLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_OSPICLKSOURCE_HCLK)  || \
   ((__SOURCE__) == RCC_OSPICLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_OSPICLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_OSPICLKSOURCE_CLKP))
#endif /* OCTOSPI1 */

#if defined(RCC_CR_PLL3ON)
#define IS_RCC_SPI1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI1CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_PLL3P) || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_PIN)   || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_CLKP))

#define IS_RCC_SPI2CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI2CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_PLL3P) || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_PIN)   || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_CLKP))

#define IS_RCC_SPI3CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI3CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_PLL3P) || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_PIN)  || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_CLKP))
#else
#define IS_RCC_SPI1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI1CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_PIN)   || \
   ((__SOURCE__) == RCC_SPI1CLKSOURCE_CLKP))

#define IS_RCC_SPI2CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI2CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_PIN)   || \
   ((__SOURCE__) == RCC_SPI2CLKSOURCE_CLKP))

#define IS_RCC_SPI3CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI3CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_PIN)  || \
   ((__SOURCE__) == RCC_SPI3CLKSOURCE_CLKP))

#endif /* RCC_CR_PLL3ON */

#if defined(SPI4)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_SPI4CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI4CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_HSE))
#else
#define IS_RCC_SPI4CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI4CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_SPI4CLKSOURCE_HSE))
#endif /* RCC_CR_PLL3ON */
#endif /* SPI4 */

#if defined(SPI5)
#define IS_RCC_SPI5CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI5CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_SPI5CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_SPI5CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_SPI5CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_SPI5CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_SPI5CLKSOURCE_HSE))
#endif /* SPI5 */

#if defined(SPI6)
#define IS_RCC_SPI6CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_SPI6CLKSOURCE_PCLK2) || \
   ((__SOURCE__) == RCC_SPI6CLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_SPI6CLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_SPI6CLKSOURCE_HSI)   || \
   ((__SOURCE__) == RCC_SPI6CLKSOURCE_CSI)   || \
   ((__SOURCE__) == RCC_SPI6CLKSOURCE_HSE))
#endif /* SPI6 */

#if defined(USB_DRD_FS)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_USBCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_USBCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_HSI48))
#else
#define IS_RCC_USBCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_USBCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_PLL2Q) || \
   ((__SOURCE__) == RCC_USBCLKSOURCE_HSI48))
#endif /* RCC_CR_PLL3ON */
#endif /* USB_DRD_FS */

#if defined(CEC)
#define IS_RCC_CECCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_CECCLKSOURCE_LSE) || \
   ((__SOURCE__) == RCC_CECCLKSOURCE_LSI) || \
   ((__SOURCE__) == RCC_CECCLKSOURCE_CSI_DIV122))
#endif /*CEC*/

#if defined(PLAY1)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_PLAY1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_PLAY1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_CLKP))
#else
#define IS_RCC_PLAY1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_PLAY1CLKSOURCE_PCLK3) || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_LSE)   || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_LSI)   || \
   ((__SOURCE__) == RCC_PLAY1CLKSOURCE_CLKP))
#endif /* RCC_CR_PLL3ON */

#define IS_RCC_PLAY1_DIV(__DIVIDER__) \
  (((__DIVIDER__) == RCC_PLAY1_CLK_DIV_1 )    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_2 )    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_4 )    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_8 )    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_16)    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_32)    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_64)    || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_128)   || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_256)   || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_512)   || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_1024)  || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_2048)  || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_4096)  || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_8192)  || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_16384) || \
   ((__DIVIDER__) == RCC_PLAY1_CLK_DIV_32768))
#endif /* PLAY1 */

#if defined(USB_OTG_FS)
#if defined(PWR_USBSCR_OTGHSEN)
#define IS_RCC_OTGFSCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_OTGFSCLKSOURCE_HSI48) || \
   ((__SOURCE__) == RCC_OTGFSCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_OTGFSCLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_OTGFSCLKSOURCE_CLK48))
#else
#define IS_RCC_OTGFSCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_OTGFSCLKSOURCE_HSI48) || \
   ((__SOURCE__) == RCC_OTGFSCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_OTGFSCLKSOURCE_PLL3Q))
#endif /* PWR_USBSCR_OTGHSEN */
#endif /* USB_OTG_FS */

#if defined(USB_OTG_HS)
#define IS_RCC_OTGHSCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_OTGHSCLKSOURCE_HSE) || \
   ((__SOURCE__) == RCC_OTGHSCLKSOURCE_PLL3Q) || \
   ((__SOURCE__) == RCC_OTGHSCLKSOURCE_HSE_DIV2) || \
   ((__SOURCE__) == RCC_OTGHSCLKSOURCE_PLL1Q_DIV2))
#endif /* USB_OTG_HS */

#if defined(RCC_CCIPR4_OTGPHYREFCKSEL)
#define IS_RCC_OTGPHYCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_OTGPHYREFCKCLKSOURCE_16M) || \
   ((__SOURCE__) == RCC_OTGPHYREFCKCLKSOURCE_19_2M) || \
   ((__SOURCE__) == RCC_OTGPHYREFCKCLKSOURCE_24M) || \
   ((__SOURCE__) == RCC_OTGPHYREFCKCLKSOURCE_26M) || \
   ((__SOURCE__) == RCC_OTGPHYREFCKCLKSOURCE_32M))
#endif /* RCC_CCIPR4_OTGPHYREFCKSEL */

#if defined(LTDC)
#define IS_RCC_LTDCCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_LTDCCLKSOURCE_PLL3R) || \
   ((__SOURCE__) == RCC_LTDCCLKSOURCE_PLL2R))
#endif /* LTDC */

#if defined(OCTOSPI2)
#define IS_RCC_OSPI2CLKSOURCE(__SOURCE__)  \
  (((__SOURCE__) == RCC_OSPI2CLKSOURCE_HCLK)  || \
   ((__SOURCE__) == RCC_OSPI2CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_OSPI2CLKSOURCE_PLL2R) || \
   ((__SOURCE__) == RCC_OSPI2CLKSOURCE_CLKP))
#endif /* OCTOSPI2 */

#if defined(ADF1)
#define IS_RCC_ADF1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_ADF1CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_ADF1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_ADF1CLKSOURCE_PLL3P) || \
   ((__SOURCE__) == RCC_ADF1CLKSOURCE_PIN) || \
   ((__SOURCE__) == RCC_ADF1CLKSOURCE_CLKP))
#endif /* ADF1 */

#if defined(MDF1)
#define IS_RCC_MDF1CLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_MDF1CLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_MDF1CLKSOURCE_PLL2P) || \
   ((__SOURCE__) == RCC_MDF1CLKSOURCE_PLL3P) || \
   ((__SOURCE__) == RCC_MDF1CLKSOURCE_PIN) || \
   ((__SOURCE__) == RCC_MDF1CLKSOURCE_CLKP))
#endif /* MDF1 */

#if defined(RCC_CCIPR4_ETHCLKSEL)
#define IS_RCC_ETHCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_ETHCLKSOURCE_HSE) || \
   ((__SOURCE__) == RCC_ETHCLKSOURCE_PLL1Q))
#endif /* RCC_CCIPR4_ETHCLKSEL */

#if defined(RCC_CCIPR5_ETHPTPCLKSEL)
#if defined(RCC_CR_PLL3ON)
#define IS_RCC_ETHPTPCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_ETHPTPCLKSOURCE_HCLK) || \
   ((__SOURCE__) == RCC_ETHPTPCLKSOURCE_PLL1R) || \
   ((__SOURCE__) == RCC_ETHPTPCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_ETHPTPCLKSOURCE_PLL3P))
#else
#define IS_RCC_ETHPTPCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_ETHPTPCLKSOURCE_HCLK) || \
   ((__SOURCE__) == RCC_ETHPTPCLKSOURCE_PLL1R) || \
   ((__SOURCE__) == RCC_ETHPTPCLKSOURCE_PLL1Q))
#endif /* RCC_CR_PLL3ON */
#endif /* RCC_CCIPR5_ETHPTPCLKSEL */

#if defined(RCC_CCIPR4_ETHPTPDIV)
#define IS_RCC_ETHPTP_DIV(__DIVIDER__) \
  (((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_1)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_2)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_3)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_4)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_5)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_6)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_7)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_8)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_9)  || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_10) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_11) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_12) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_13) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_14) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_15) || \
   ((__DIVIDER__) == RCC_ETHPTP_CLK_DIV_16))
#endif /* RCC_CCIPR4_ETHPTPDIV */

#if defined(RCC_CCIPR5_ETHREFCLKSEL)
#define IS_RCC_ETHREFCLKSOURCE(__SOURCE__) \
  (((__SOURCE__) == RCC_ETHREFCLKSOURCE_PLL1Q) || \
   ((__SOURCE__) == RCC_ETHREFCLKSOURCE_PLL1R))
#endif /* RCC_CCIPR5_ETHREFCLKSEL */

#define IS_RCC_TIMPRES(VALUE)  \
  (((VALUE) == RCC_TIMPRES_DEACTIVATED) || \
   ((VALUE) == RCC_TIMPRES_ACTIVATED))

#if defined(CRS)
#if defined(USB_OTG_HS)
#define IS_RCC_CRS_SYNC_SOURCE(__SOURCE__) (((__SOURCE__) == RCC_CRS_SYNC_SOURCE_GPIO) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_LSE) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_OTG_FS) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_OTG_HS))
#elif defined(USB_OTG_FS)
#define IS_RCC_CRS_SYNC_SOURCE(__SOURCE__) (((__SOURCE__) == RCC_CRS_SYNC_SOURCE_GPIO) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_LSE) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_OTG_FS))
#elif defined(USB_DRD_FS)
#define IS_RCC_CRS_SYNC_SOURCE(__SOURCE__) (((__SOURCE__) == RCC_CRS_SYNC_SOURCE_GPIO) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_LSE) || \
                                            ((__SOURCE__) == RCC_CRS_SYNC_SOURCE_USB))
#endif /* USB_OTG_HS */

#define IS_RCC_CRS_SYNC_DIV(__DIV__)       (((__DIV__) == RCC_CRS_SYNC_DIV1) || ((__DIV__) == RCC_CRS_SYNC_DIV2) || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV4) || ((__DIV__) == RCC_CRS_SYNC_DIV8) || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV16) || ((__DIV__) == RCC_CRS_SYNC_DIV32) || \
                                            ((__DIV__) == RCC_CRS_SYNC_DIV64) || ((__DIV__) == RCC_CRS_SYNC_DIV128))

#define IS_RCC_CRS_SYNC_POLARITY(__POLARITY__) (((__POLARITY__) == RCC_CRS_SYNC_POLARITY_RISING) || \
                                                ((__POLARITY__) == RCC_CRS_SYNC_POLARITY_FALLING))

#define IS_RCC_CRS_RELOADVALUE(__VALUE__)  (((__VALUE__) <= 0xFFFFU))

#define IS_RCC_CRS_ERRORLIMIT(__VALUE__)   (((__VALUE__) <= 0xFFU))

#define IS_RCC_CRS_HSI48CALIBRATION(__VALUE__) (((__VALUE__) <= 0x7FU))

#define IS_RCC_CRS_FREQERRORDIR(__DIR__)   (((__DIR__) == RCC_CRS_FREQERRORDIR_UP) || \
                                            ((__DIR__) == RCC_CRS_FREQERRORDIR_DOWN))

#endif /* CRS */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup RCCEx_Exported_Functions
  * @{
  */

/** @addtogroup RCCEx_Exported_Functions_Group1
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_PeriphCLKConfig(const RCC_PeriphCLKInitTypeDef  *pPeriphClkInit);
void              HAL_RCCEx_GetPeriphCLKConfig(RCC_PeriphCLKInitTypeDef  *pPeriphClkInit);
uint32_t          HAL_RCCEx_GetPeriphCLKFreq(uint64_t PeriphClk);
void     HAL_RCCEx_GetPLL1ClockFreq(PLL1_ClocksTypeDef *pPLL1_Clocks);
void     HAL_RCCEx_GetPLL2ClockFreq(PLL2_ClocksTypeDef *pPLL2_Clocks);
#if defined(RCC_CR_PLL3ON)
void     HAL_RCCEx_GetPLL3ClockFreq(PLL3_ClocksTypeDef *pPLL3_Clocks);
#endif /* RCC_CR_PLL3ON */
/**
  * @}
  */

/** @addtogroup RCCEx_Exported_Functions_Group2
  * @{
  */

HAL_StatusTypeDef HAL_RCCEx_EnablePLL2(RCC_PLL2InitTypeDef  *pPLL2Init);
HAL_StatusTypeDef HAL_RCCEx_DisablePLL2(void);
#if defined(RCC_CR_PLL3ON)
HAL_StatusTypeDef HAL_RCCEx_EnablePLL3(RCC_PLL3InitTypeDef  *pPLL3Init);
HAL_StatusTypeDef HAL_RCCEx_DisablePLL3(void);
#endif /* RCC_CR_PLL3ON */

void              HAL_RCCEx_WakeUpStopCLKConfig(uint32_t WakeUpClk);
void              HAL_RCCEx_KerWakeUpStopCLKConfig(uint32_t WakeUpClk);
void              HAL_RCCEx_EnableLSECSS(void);
void              HAL_RCCEx_DisableLSECSS(void);
void              HAL_RCCEx_LSECSS_IRQHandler(void);
void              HAL_RCCEx_LSECSS_Callback(void);
void              HAL_RCCEx_EnableLSCO(uint32_t LSCOSource);
void              HAL_RCCEx_DisableLSCO(void);
/**
  * @}
  */

#if defined(CRS)

/** @addtogroup RCCEx_Exported_Functions_Group3
  * @{
  */
void              HAL_RCCEx_CRSConfig(const RCC_CRSInitTypeDef *pInit);
void              HAL_RCCEx_CRSSoftwareSynchronizationGenerate(void);
void              HAL_RCCEx_CRSGetSynchronizationInfo(RCC_CRSSynchroInfoTypeDef *pSynchroInfo);
uint32_t          HAL_RCCEx_CRSWaitSynchronization(uint32_t Timeout);
void              HAL_RCCEx_CRS_IRQHandler(void);
void              HAL_RCCEx_CRS_SyncOkCallback(void);
void              HAL_RCCEx_CRS_SyncWarnCallback(void);
void              HAL_RCCEx_CRS_ExpectedSyncCallback(void);
void              HAL_RCCEx_CRS_ErrorCallback(uint32_t Error);
/**
  * @}
  */

#endif /* CRS */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_HAL_RCC_EX_H */

