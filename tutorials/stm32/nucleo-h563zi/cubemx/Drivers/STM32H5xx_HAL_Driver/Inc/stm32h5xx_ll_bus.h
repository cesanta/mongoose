/**
  ******************************************************************************
  * @file    stm32h5xx_ll_bus.h
  * @author  MCD Application Team
  * @brief   Header file of BUS LL module.

  @verbatim
                      ##### RCC Limitations #####
  ==============================================================================
    [..]
      A delay between an RCC peripheral clock enable and the effective peripheral
      enabling should be taken into account in order to manage the peripheral read/write
      from/to registers.
      (+) This delay depends on the peripheral mapping.
        (++) AHB , APB peripherals,  1 dummy read is necessary

    [..]
      Workarounds:
      (#) For AHB , APB peripherals, a dummy read to the peripheral register has been
          inserted in each LL_{BUS}_GRP{x}_EnableClock() function.

  @endverbatim
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
#ifndef __STM32H5xx_LL_BUS_H
#define __STM32H5xx_LL_BUS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

#if defined(RCC)

/** @defgroup BUS_LL BUS
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup BUS_LL_Exported_Constants BUS Exported Constants
  * @{
  */

/** @defgroup BUS_LL_AHB_BRANCH_CLK_AHBx  BRANCH CLK AHBx
  * @{
  */
#define LL_AHB_BRANCH_CLK_AHB1          RCC_CFGR2_AHB1DIS
#define LL_AHB_BRANCH_CLK_AHB2          RCC_CFGR2_AHB2DIS
#if defined(AHB4PERIPH_BASE)
#define LL_AHB_BRANCH_CLK_AHB4          RCC_CFGR2_AHB4DIS
#endif /* AHB4PERIPH_BASE */
/**
  * @}
  */

/** @defgroup BUS_LL_APB_BRANCH_CLK_APBx  BRANCH CLK APBx
  * @{
  */
#define LL_APB_BRANCH_CLK_APB1          RCC_CFGR2_APB1DIS
#define LL_APB_BRANCH_CLK_APB2          RCC_CFGR2_APB2DIS
#define LL_APB_BRANCH_CLK_APB3          RCC_CFGR2_APB3DIS
/**
  * @}
  */

/** @defgroup BUS_LL_EC_AHB1_GRP1_PERIPH  AHB1 GRP1 PERIPH
  * @{
  */
#if defined(CORDIC)
#define LL_AHB1_GRP1_PERIPH_ALL           0xF13AD103U
#elif defined(DCACHE)
#define LL_AHB1_GRP1_PERIPH_ALL           0xF1021103U
#elif defined(JPEG)
#define LL_AHB1_GRP1_PERIPH_ALL           0xD1FFD103U
#else
#define LL_AHB1_GRP1_PERIPH_ALL           0x91021103U
#endif /* CORDIC */
#define LL_AHB1_GRP1_PERIPH_GPDMA1        RCC_AHB1ENR_GPDMA1EN
#define LL_AHB1_GRP1_PERIPH_GPDMA2        RCC_AHB1ENR_GPDMA2EN
#if defined(CORDIC)
#define LL_AHB1_GRP1_PERIPH_CORDIC        RCC_AHB1ENR_CORDICEN
#endif /* CORDIC */
#if defined(FMAC)
#define LL_AHB1_GRP1_PERIPH_FMAC          RCC_AHB1ENR_FMACEN
#endif /* FMAC */
#define LL_AHB1_GRP1_PERIPH_FLASH         RCC_AHB1ENR_FLITFEN
#if defined(ETH)
#define LL_AHB1_GRP1_PERIPH_ETH           RCC_AHB1ENR_ETHEN
#define LL_AHB1_GRP1_PERIPH_ETHTX         RCC_AHB1ENR_ETHTXEN
#define LL_AHB1_GRP1_PERIPH_ETHRX         RCC_AHB1ENR_ETHRXEN
#endif /* ETH */
#if defined(RCC_AHB1ENR_ETHCKEN)
#define LL_AHB1_GRP1_PERIPH_ETHINTERN     RCC_AHB1ENR_ETHCKEN
#endif /* RCC_AHB1ENR_ETHCKEN */
#define LL_AHB1_GRP1_PERIPH_CRC           RCC_AHB1ENR_CRCEN
#define LL_AHB1_GRP1_PERIPH_RAMCFG        RCC_AHB1ENR_RAMCFGEN
#define LL_AHB1_GRP1_PERIPH_GTZC1         RCC_AHB1ENR_TZSC1EN
#define LL_AHB1_GRP1_PERIPH_BKPSRAM       RCC_AHB1ENR_BKPRAMEN
#define LL_AHB1_GRP1_PERIPH_ICACHE        RCC_AHB1LPENR_ICACHELPEN
#if defined(DCACHE1)
#define LL_AHB1_GRP1_PERIPH_DCACHE1       RCC_AHB1ENR_DCACHE1EN
#endif /* DCACHE1 */
#define LL_AHB1_GRP1_PERIPH_SRAM1         RCC_AHB1ENR_SRAM1EN
#if defined(MDF1)
#define LL_AHB1_GRP1_PERIPH_MDF1          RCC_AHB1ENR_MDF1EN
#endif /* MDF1 */
#if defined(DMA2D)
#define LL_AHB1_GRP1_PERIPH_DMA2D         RCC_AHB1ENR_DMA2DEN
#endif /* DMA2D */
#if defined(JPEG)
#define LL_AHB1_GRP1_PERIPH_JPEG          RCC_AHB1ENR_JPEGEN
#endif /* JPEG */
/**
  * @}
  */

/** @defgroup BUS_LL_EC_AHB2_GRP1_PERIPH  AHB2 GRP1 PERIPH
  * @{
  */
#if defined(GPIOI)
#define LL_AHB2_GRP1_PERIPH_ALL            0xC01F1DFFU
#elif defined(GPIOE)
#define LL_AHB2_GRP1_PERIPH_ALL            0xC01F1CFFU
#elif defined(GPIOK)
#define LL_AHB2_GRP1_PERIPH_ALL            0xF73FFFFFU
#else
#define LL_AHB2_GRP1_PERIPH_ALL            0x40060C8FU
#endif /* GPIOE */
#define LL_AHB2_GRP1_PERIPH_GPIOA          RCC_AHB2ENR_GPIOAEN
#define LL_AHB2_GRP1_PERIPH_GPIOB          RCC_AHB2ENR_GPIOBEN
#define LL_AHB2_GRP1_PERIPH_GPIOC          RCC_AHB2ENR_GPIOCEN
#define LL_AHB2_GRP1_PERIPH_GPIOD          RCC_AHB2ENR_GPIODEN
#if defined(GPIOE)
#define LL_AHB2_GRP1_PERIPH_GPIOE          RCC_AHB2ENR_GPIOEEN
#endif /* GPIOE */
#if defined(GPIOF)
#define LL_AHB2_GRP1_PERIPH_GPIOF          RCC_AHB2ENR_GPIOFEN
#endif /* GPIOF */
#if defined(GPIOG)
#define LL_AHB2_GRP1_PERIPH_GPIOG          RCC_AHB2ENR_GPIOGEN
#endif /* GPIOG */
#define LL_AHB2_GRP1_PERIPH_GPIOH          RCC_AHB2ENR_GPIOHEN
#if defined(GPIOI)
#define LL_AHB2_GRP1_PERIPH_GPIOI          RCC_AHB2ENR_GPIOIEN
#endif /* GPIOI */
#if defined(GPIOJ)
#define LL_AHB2_GRP1_PERIPH_GPIOJ          RCC_AHB2ENR_GPIOJEN
#endif /* GPIOJ */
#if defined(GPIOK)
#define LL_AHB2_GRP1_PERIPH_GPIOK          RCC_AHB2ENR_GPIOKEN
#endif /* GPIOK */
#define LL_AHB2_GRP1_PERIPH_ADC            RCC_AHB2ENR_ADCEN
#define LL_AHB2_GRP1_PERIPH_DAC1           RCC_AHB2ENR_DAC1EN
#if defined(DCMI)
#define LL_AHB2_GRP1_PERIPH_DCMI_PSSI      RCC_AHB2ENR_DCMI_PSSIEN
#endif /* DCMI */
#if defined(AES)
#define LL_AHB2_GRP1_PERIPH_AES            RCC_AHB2ENR_AESEN
#endif /* AES */
#if defined(HASH)
#define LL_AHB2_GRP1_PERIPH_HASH           RCC_AHB2ENR_HASHEN
#endif /* HASH */
#define LL_AHB2_GRP1_PERIPH_RNG            RCC_AHB2ENR_RNGEN
#if defined(PKA)
#define LL_AHB2_GRP1_PERIPH_PKA            RCC_AHB2ENR_PKAEN
#endif /* PKA */
#if defined(SAES)
#define LL_AHB2_GRP1_PERIPH_SAES           RCC_AHB2ENR_SAESEN
#endif /* SAES */
#define LL_AHB2_GRP1_PERIPH_SRAM2          RCC_AHB2ENR_SRAM2EN
#if defined(SRAM3_BASE)
#define LL_AHB2_GRP1_PERIPH_SRAM3          RCC_AHB2ENR_SRAM3EN
#endif /* SRAM3_BASE */
#if defined(USB_OTG_FS)
#define LL_AHB2_GRP1_PERIPH_OTGFS          RCC_AHB2ENR_OTGFSEN
#endif /* USB_OTG_FS */
#if defined(USB_OTG_HS)
#define LL_AHB2_GRP1_PERIPH_OTGHS          RCC_AHB2ENR_OTGHSEN
#endif /* USB_OTG_HS */
#if defined(ADF1)
#define LL_AHB2_GRP1_PERIPH_ADF1           RCC_AHB2ENR_ADF1EN
#endif /* ADF1 */
#if defined(CCB)
#define LL_AHB2_GRP1_PERIPH_CCB           RCC_AHB2ENR_CCBEN
#endif /* CCB */
#if defined(ADC3)
#define LL_AHB2_GRP1_PERIPH_ADC3           RCC_AHB2ENR_ADC3EN
#endif /* ADC3 */
#if defined(SRAM4_BASE)
#define LL_AHB2_GRP1_PERIPH_SRAM4          RCC_AHB2ENR_SRAM4EN
#endif /* SRAM4_BASE */
#if defined(SRAM5_BASE)
#define LL_AHB2_GRP1_PERIPH_SRAM5          RCC_AHB2ENR_SRAM5EN
#endif /* SRAM5_BASE */
/**
  * @}
  */
#if defined(AHB4PERIPH_BASE)
/** @defgroup BUS_LL_EC_AHB4_GRP1_PERIPH  AHB4 GRP1 PERIPH
  * @{
  */
#if defined(OTFDEC2)
#define LL_AHB4_GRP1_PERIPH_ALL            0x00711980U
#elif defined(OCTOSPI2)
#define LL_AHB4_GRP1_PERIPH_ALL            0x00711880U
#else
#define LL_AHB4_GRP1_PERIPH_ALL            0x00111880U
#endif /* OTFDEC2 */
#if defined(OTFDEC1)
#define LL_AHB4_GRP1_PERIPH_OTFDEC         RCC_AHB4ENR_OTFDEC1EN
#endif /* OTFDEC1*/
#define LL_AHB4_GRP1_PERIPH_SDMMC1         RCC_AHB4ENR_SDMMC1EN
#if defined(SDMMC2)
#define LL_AHB4_GRP1_PERIPH_SDMMC2         RCC_AHB4ENR_SDMMC2EN
#endif /* SDMMC2*/
#define LL_AHB4_GRP1_PERIPH_FMC            RCC_AHB4ENR_FMCEN
#define LL_AHB4_GRP1_PERIPH_OSPI1          RCC_AHB4ENR_OCTOSPI1EN
#if defined(OCTOSPI2)
#define LL_AHB4_GRP1_PERIPH_OSPI2          RCC_AHB4ENR_OCTOSPI2EN
#endif /* OCTOSPI2*/
#if defined(OCTOSPIM)
#define LL_AHB4_GRP1_PERIPH_OSPIM          RCC_AHB4ENR_OCTOSPIMEN
#endif /* OCTOSPIM*/
#if defined(OTFDEC2)
#define LL_AHB4_GRP1_PERIPH_OTFDEC2        RCC_AHB4ENR_OTFDEC2EN
#endif /* OTFDEC2*/
/**
  * @}
  */
#endif /* AHB4PERIPH_BASE */

/** @defgroup BUS_LL_EC_APB1_GRP1_PERIPH  APB1 GRP1 PERIPH
  * @{
  */
#if defined(USART11)
#define LL_APB1_GRP1_PERIPH_ALL           0xDFFEC9FFU
#elif defined(USART6)
#define LL_APB1_GRP1_PERIPH_ALL           0x13FEC87FU
#elif defined(OPAMP)
#define LL_APB1_GRP1_PERIPH_ALL           0xDFFFE9FFU
#else
#define LL_APB1_GRP1_PERIPH_ALL           0x01E7E833U
#endif /* TIM4 */
#define LL_APB1_GRP1_PERIPH_TIM2          RCC_APB1LENR_TIM2EN
#define LL_APB1_GRP1_PERIPH_TIM3          RCC_APB1LENR_TIM3EN
#if defined(TIM4)
#define LL_APB1_GRP1_PERIPH_TIM4          RCC_APB1LENR_TIM4EN
#endif /* TIM4*/
#if defined(TIM5)
#define LL_APB1_GRP1_PERIPH_TIM5          RCC_APB1LENR_TIM5EN
#endif /* TIM5*/
#define LL_APB1_GRP1_PERIPH_TIM6          RCC_APB1LENR_TIM6EN
#define LL_APB1_GRP1_PERIPH_TIM7          RCC_APB1LENR_TIM7EN
#if defined(TIM12)
#define LL_APB1_GRP1_PERIPH_TIM12         RCC_APB1LENR_TIM12EN
#endif /* TIM12*/
#if defined(TIM13)
#define LL_APB1_GRP1_PERIPH_TIM13         RCC_APB1LENR_TIM13EN
#endif /* TIM13*/
#if defined(TIM14)
#define LL_APB1_GRP1_PERIPH_TIM14         RCC_APB1LENR_TIM14EN
#endif /* TIM14*/
#define LL_APB1_GRP1_PERIPH_WWDG          RCC_APB1LENR_WWDGEN
#if defined(OPAMP1)
#define LL_APB1_GRP1_PERIPH_OPAMP         RCC_APB1LENR_OPAMPEN
#endif /* OPAMP1 */
#define LL_APB1_GRP1_PERIPH_SPI2          RCC_APB1LENR_SPI2EN
#define LL_APB1_GRP1_PERIPH_SPI3          RCC_APB1LENR_SPI3EN
#if defined(COMP1)
#define LL_APB1_GRP1_PERIPH_COMP          RCC_APB1LENR_COMPEN
#endif /* COMP1 */
#define LL_APB1_GRP1_PERIPH_USART2        RCC_APB1LENR_USART2EN
#define LL_APB1_GRP1_PERIPH_USART3        RCC_APB1LENR_USART3EN
#if defined(UART4)
#define LL_APB1_GRP1_PERIPH_UART4         RCC_APB1LENR_UART4EN
#endif /* UART4*/
#if defined(UART5)
#define LL_APB1_GRP1_PERIPH_UART5         RCC_APB1LENR_UART5EN
#endif /* UART5*/
#define LL_APB1_GRP1_PERIPH_I2C1          RCC_APB1LENR_I2C1EN
#define LL_APB1_GRP1_PERIPH_I2C2          RCC_APB1LENR_I2C2EN
#define LL_APB1_GRP1_PERIPH_I3C1          RCC_APB1LENR_I3C1EN
#define LL_APB1_GRP1_PERIPH_CRS           RCC_APB1LENR_CRSEN
#if defined(USART6)
#define LL_APB1_GRP1_PERIPH_USART6        RCC_APB1LENR_USART6EN
#endif /* USART6*/
#if defined(USART10)
#define LL_APB1_GRP1_PERIPH_USART10       RCC_APB1LENR_USART10EN
#endif /* USART10*/
#if defined(USART11)
#define LL_APB1_GRP1_PERIPH_USART11       RCC_APB1LENR_USART11EN
#endif /* USART11*/
#if defined(CEC)
#define LL_APB1_GRP1_PERIPH_CEC           RCC_APB1LENR_CECEN
#endif /* CEC*/
#if defined(UART7)
#define LL_APB1_GRP1_PERIPH_UART7         RCC_APB1LENR_UART7EN
#endif /* UART7 */
#if defined(UART8)
#define LL_APB1_GRP1_PERIPH_UART8         RCC_APB1LENR_UART8EN
#endif /* UART8 */
#if defined(OPAMP)
#define LL_APB1_GRP1_PERIPH_OPAMP         RCC_APB1LENR_OPAMPEN
#endif /* OPAMP */
#if defined(COMP)
#define LL_APB1_GRP1_PERIPH_COMP          RCC_APB1LENR_COMPEN
#endif /* COMP */
/**
  * @}
  */


/** @defgroup BUS_LL_EC_APB1_GRP2_PERIPH  APB1 GRP2 PERIPH
  * @{
  */
#if defined(UART9)
#define LL_APB1_GRP2_PERIPH_ALL            0x0080022BU
#else
#define LL_APB1_GRP2_PERIPH_ALL            0x00000228U
#endif /* UART9 */
#if defined(UART9)
#define LL_APB1_GRP2_PERIPH_UART9          RCC_APB1HENR_UART9EN
#endif /* UART9 */
#if defined(UART12)
#define LL_APB1_GRP2_PERIPH_UART12         RCC_APB1HENR_UART12EN
#endif /* UART12*/
#define LL_APB1_GRP2_PERIPH_DTS            RCC_APB1HENR_DTSEN
#define LL_APB1_GRP2_PERIPH_LPTIM2         RCC_APB1HENR_LPTIM2EN
#define LL_APB1_GRP2_PERIPH_FDCAN          RCC_APB1HENR_FDCANEN
#if defined(UCPD1)
#define LL_APB1_GRP2_PERIPH_UCPD1          RCC_APB1HENR_UCPD1EN
#endif /* UCPD1 */
/**
  * @}
  */

/** @defgroup BUS_LL_EC_APB2_GRP1_PERIPH  APB2 GRP1 PERIPH
  * @{
  */
#if defined(TIM16)
#define LL_APB2_GRP1_PERIPH_ALL            0x017F7800U
#elif defined(TIM8)
#define LL_APB2_GRP1_PERIPH_ALL            0x01097800U
#elif defined(LTDC)
#define LL_APB2_GRP1_PERIPH_ALL            0x287F7800U
#else
#define LL_APB2_GRP1_PERIPH_ALL            0x01005800U
#endif /* TIM8 */
#define LL_APB2_GRP1_PERIPH_TIM1           RCC_APB2ENR_TIM1EN
#define LL_APB2_GRP1_PERIPH_SPI1           RCC_APB2ENR_SPI1EN
#if defined(TIM8)
#define LL_APB2_GRP1_PERIPH_TIM8           RCC_APB2ENR_TIM8EN
#endif /* TIM8 */
#define LL_APB2_GRP1_PERIPH_USART1         RCC_APB2ENR_USART1EN
#if defined(TIM15)
#define LL_APB2_GRP1_PERIPH_TIM15          RCC_APB2ENR_TIM15EN
#endif /* TIM15 */
#if defined(TIM16)
#define LL_APB2_GRP1_PERIPH_TIM16          RCC_APB2ENR_TIM16EN
#endif /* TIM16 */
#if defined(TIM17)
#define LL_APB2_GRP1_PERIPH_TIM17          RCC_APB2ENR_TIM17EN
#endif /* TIM17 */
#if defined(SPI4)
#define LL_APB2_GRP1_PERIPH_SPI4           RCC_APB2ENR_SPI4EN
#endif /* SPI4 */
#if defined(SPI6)
#define LL_APB2_GRP1_PERIPH_SPI6           RCC_APB2ENR_SPI6EN
#endif /* SPI6 */
#if defined(SAI1)
#define LL_APB2_GRP1_PERIPH_SAI1           RCC_APB2ENR_SAI1EN
#endif /* SAI1 */
#if defined(SAI2)
#define LL_APB2_GRP1_PERIPH_SAI2           RCC_APB2ENR_SAI2EN
#endif /* SAI2 */
#if defined(USB_DRD_FS)
#define LL_APB2_GRP1_PERIPH_USB            RCC_APB2ENR_USBEN
#endif /* USB_DRD_FS */
#if defined(LTDC)
#define LL_APB2_GRP1_PERIPH_LTDC           RCC_APB2ENR_LTDCEN
#endif /* LTDC */
#if defined(GFXTIM)
#define LL_APB2_GRP1_PERIPH_GFXTIM         RCC_APB2ENR_GFXTIMEN
#endif /* GFXTIM */
/**
  * @}
  */

/** @defgroup BUS_LL_EC_APB3_GRP1_PERIPH  APB3 GRP1 PERIPH
  * @{
  */
#if defined(SPI5)
#define LL_APB3_GRP1_PERIPH_ALL           0x0030F9E2U
#elif defined(I2C4)
#define LL_APB3_GRP1_PERIPH_ALL           0x00300AC2U
#elif defined(PLAY1)
#define LL_APB3_GRP1_PERIPH_ALL           0x00B0FBE2U
#else
#define LL_APB3_GRP1_PERIPH_ALL           0x00200A42U
#endif /* SPI5 */
#define LL_APB3_GRP1_PERIPH_SBS           RCC_APB3ENR_SBSEN
#if defined(SPI5)
#define LL_APB3_GRP1_PERIPH_SPI5          RCC_APB3ENR_SPI5EN
#endif /* SPI5 */
#define LL_APB3_GRP1_PERIPH_LPUART1       RCC_APB3ENR_LPUART1EN
#if defined(I2C3)
#define LL_APB3_GRP1_PERIPH_I2C3          RCC_APB3ENR_I2C3EN
#endif /* I2C3 */
#if defined(I2C4)
#define LL_APB3_GRP1_PERIPH_I2C4          RCC_APB3ENR_I2C4EN
#endif /* I2C4 */
#if defined(I3C2)
#define LL_APB3_GRP1_PERIPH_I3C2          RCC_APB3ENR_I3C2EN
#endif /* I3C2 */
#define LL_APB3_GRP1_PERIPH_LPTIM1        RCC_APB3ENR_LPTIM1EN
#if defined(LPTIM3)
#define LL_APB3_GRP1_PERIPH_LPTIM3        RCC_APB3ENR_LPTIM3EN
#endif /* LPTIM3 */
#if defined(LPTIM4)
#define LL_APB3_GRP1_PERIPH_LPTIM4        RCC_APB3ENR_LPTIM4EN
#endif /* LPTIM4 */
#if defined(LPTIM5)
#define LL_APB3_GRP1_PERIPH_LPTIM5        RCC_APB3ENR_LPTIM5EN
#endif /* LPTIM5 */
#if defined(LPTIM6)
#define LL_APB3_GRP1_PERIPH_LPTIM6        RCC_APB3ENR_LPTIM6EN
#endif /* LPTIM6 */
#define LL_APB3_GRP1_PERIPH_VREF          RCC_APB3ENR_VREFEN
#define LL_APB3_GRP1_PERIPH_RTCAPB        RCC_APB3ENR_RTCAPBEN
#if defined(PLAY1)
#define LL_APB3_GRP1_PERIPH_PLAY1         RCC_APB3ENR_PLAY1EN
#define LL_APB3_GRP1_PERIPH_PLAY1APB      RCC_APB3ENR_PLAY1APBEN
#endif /* PLAY1 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup BUS_LL_Exported_Functions BUS Exported Functions
  * @{
  */

/** @defgroup BUS_LL_EF_AHBx AHBx Branch
  * @{
  */
/**
  * @brief  Disable of AHBx Clock Branch
  * @rmtoll CFGR2    AHB1DIS     LL_AHB_DisableClock\n
  *         CFGR2    AHB2DIS     LL_AHB_DisableClock\n
  *         CFGR2    AHB4DIS     LL_AHB_DisableClock
  * @param  AHBx This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB1
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB2
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB4 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB_DisableClock(uint32_t AHBx)
{
  SET_BIT(RCC->CFGR2, AHBx);
}

/**
  * @brief  Enable of AHBx Clock Branch
  * @rmtoll CFGR2    AHB1DIS     LL_AHB_EnableClock\n
  *         CFGR2    AHB2DIS     LL_AHB_EnableClock\n
  *         CFGR2    AHB4DIS     LL_AHB_EnableClock
  * @param  AHBx This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB1
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB2
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB4 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB_EnableClock(uint32_t AHBx)
{
  __IO uint32_t tmpreg;
  CLEAR_BIT(RCC->CFGR2, AHBx);
  /* Delay after AHBx clock branch enabling */
  tmpreg = READ_BIT(RCC->CFGR2, AHBx);
  (void)tmpreg;
}

/**
  * @brief  Check if AHBx clock branch is disabled or not
  * @rmtoll CFGR2    AHB1DIS     LL_AHB_IsDisabledClock\n
  *         CFGR2    AHB2DIS     LL_AHB_IsDisabledClock\n
  *         CFGR2    AHB4DIS     LL_AHB_IsDisabledClock
  * @param  AHBx This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB1
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB2
  *         @arg @ref LL_AHB_BRANCH_CLK_AHB4 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of AHBx bus (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB_IsDisabledClock(uint32_t AHBx)
{
  return ((READ_BIT(RCC->CFGR2, AHBx) == AHBx) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup BUS_LL_EF_APBx APBx Branch
  * @{
  */
/**
  * @brief  Disable APBx Clock Branch
  * @rmtoll CFGR2    APB1DIS     LL_APB_DisableClock\n
  *         CFGR2    APB2DIS     LL_APB_DisableClock\n
  *         CFGR2    APB3DIS     LL_APB_DisableClock
  * @param  APBx This parameter can be a combination of the following values:
  *         @arg @ref LL_APB_BRANCH_CLK_APB1
  *         @arg @ref LL_APB_BRANCH_CLK_APB2
  *         @arg @ref LL_APB_BRANCH_CLK_APB3
  * @retval None
  */
__STATIC_INLINE void LL_APB_DisableClock(uint32_t APBx)
{
  SET_BIT(RCC->CFGR2, APBx);
}

/**
  * @brief  Enable of APBx Clock Branch
  * @rmtoll CFGR2    APB1DIS     LL_APB_EnableClock\n
  *         CFGR2    APB2DIS     LL_APB_EnableClock\n
  *         CFGR2    APB3DIS     LL_APB_EnableClock
  * @param  APBx This parameter can be a combination of the following values:
  *         @arg @ref LL_APB_BRANCH_CLK_APB1
  *         @arg @ref LL_APB_BRANCH_CLK_APB2
  *         @arg @ref LL_APB_BRANCH_CLK_APB3
  * @retval None
  */
__STATIC_INLINE void LL_APB_EnableClock(uint32_t APBx)
{
  __IO uint32_t tmpreg;
  CLEAR_BIT(RCC->CFGR2, APBx);
  /* Delay after APBx clock branch enabling */
  tmpreg = READ_BIT(RCC->CFGR2, APBx);
  (void)tmpreg;
}

/**
  * @brief  Check if APBx clock branch is disabled or not
  * @rmtoll CFGR2    APB1DIS     LL_APB_IsDisabledClock\n
  *         CFGR2    APB2DIS     LL_APB_IsDisabledClock\n
  *         CFGR2    APB3DIS     LL_APB_IsDisabledClock
  * @param  APBx This parameter can be a combination of the following values:
  *         @arg @ref LL_APB_BRANCH_CLK_APB1
  *         @arg @ref LL_APB_BRANCH_CLK_APB2
  *         @arg @ref LL_APB_BRANCH_CLK_APB3
  * @retval State of APBx bus (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB_IsDisabledClock(uint32_t APBx)
{
  return ((READ_BIT(RCC->CFGR2, APBx) == APBx) ? 1UL : 0UL);
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup BUS_LL_EF_AHB1 AHB1 Peripherals
  * @{
  */
/**
  * @brief  Enable AHB1 peripherals clock.
  * @rmtoll AHB1ENR    GPDMA1EN     LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    GPDMA2EN     LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    FLITFEN      LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    CRCEN        LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    CORDICEN     LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    FMACEN       LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    RAMCFGEN     LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    ETHEN        LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    ETHTXEN      LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    ETHRXEN      LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    TZSC1EN      LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    BKPRAMEN     LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    DCACHE1EN    LL_AHB1_GRP1_EnableClock\n
  *         AHB1ENR    SRAM1EN      LL_AHB1_GRP1_EnableClock
  *         AHB1ENR    MDF1EN       LL_AHB1_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB1ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB1 peripheral clock is enabled or not
  * @rmtoll AHB1ENR    GPDMA1EN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    GPDMA2EN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    FLITFEN      LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    CRCEN        LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    CORDICEN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    FMACEN       LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    RAMCFGEN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    ETHEN        LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    ETHTXEN      LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    ETHRXEN      LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    ETHCKEN      LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    TZSC1EN      LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    BKPRAMEN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    DCACHE1EN     LL_AHB1_GRP1_IsEnabledClock\n
  *         AHB1ENR    SRAM1EN      LL_AHB1_GRP1_IsEnabledClock
  *         AHB1ENR    MDF11EN      LL_AHB1_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB1_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB1ENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB1 peripherals clock.
  * @rmtoll AHB1ENR    GPDMA1EN    LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    GPDMA2EN    LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    FLITFEN     LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    CRCEN       LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    CORDICEN    LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    FMACEN      LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    RAMCFGEN    LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    ETHEN       LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    ETHTXEN     LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    ETHRXEN     LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    ETHCKEN     LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    TZSC1EN     LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    BKPRAMEN    LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    DCACHE1EN   LL_AHB1_GRP1_DisableClock\n
  *         AHB1ENR    SRAM1EN     LL_AHB1_GRP1_DisableClock
  *         AHB1ENR    MDF1EN      LL_AHB1_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB1ENR, Periphs);
}

/**
  * @brief  Force AHB1 peripherals reset.
  * @rmtoll AHB1RSTR    GPDMA1RST    LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    GPDMA2RST    LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    CRCRST       LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    CORDICRST    LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    FMACRST      LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    RAMCFGRST    LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    ETHRST       LL_AHB1_GRP1_ForceReset\n
  *         AHB1RSTR    TZSC1RST     LL_AHB1_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->AHB1RSTR, Periphs);
}

/**
  * @brief  Release AHB1 peripherals reset.
  * @rmtoll AHB1RSTR    GPDMA1RST     LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    GPDMA2RST     LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    CRCRST        LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    CORDICRST     LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    FMACRST       LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    RAMCFGRST     LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    ETHRST        LL_AHB1_GRP1_ReleaseReset\n
  *         AHB1RSTR    TZSC1RST      LL_AHB1_GRP1_ReleaseReset
  *         AHB1RSTR    MDF1RST      LL_AHB1_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB1RSTR, Periphs);
}

/**
  * @brief  Enable AHB1 peripheral clocks in Sleep mode
  * @rmtoll AHB1LPENR    GPDMA1LPEN    LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    GPDMA2LPEN    LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    FLITFLPEN     LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    CRCLPEN       LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    CORDICLPEN    LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    FMACLPEN      LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    RAMCFGLPEN    LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    ETHLPEN       LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    ETHTXLPEN     LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    ETHRXLPEN     LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    ETHCKLPEN     LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    TZSC1LPEN     LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    BKPRAMLPEN    LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    DCACHE1LPEN   LL_AHB1_GRP1_EnableClockSleep\n
  *         AHB1LPENR    SRAM1LPEN     LL_AHB1_GRP1_EnableClockSleep
  *         AHB1LPENR    MDF1LPEN      LL_AHB1_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB1LPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1LPENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB1 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll AHB1LPENR    GPDMA1LPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    GPDMA2LPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    FLITFLPEN     LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    CRCLPEN       LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    CORDICLPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    FMACLPEN      LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    RAMCFGLPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    ETHLPEN       LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    ETHTXLPEN     LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    ETHRXLPEN     LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    ETHCKLPEN     LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    TZSC1LPEN     LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    BKPRAMLPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    DCACHE1LPEN    LL_AHB1_GRP1_IsEnabledClockSleep\n
  *         AHB1LPENR    SRAM1LPEN     LL_AHB1_GRP1_IsEnabledClockSleep
  *         AHB1LPENR    MDF11LPEN     LL_AHB1_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB1_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB1LPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB1 peripheral clocks in Sleep mode
  * @rmtoll AHB1LPENR    GPDMA1LPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    GPDMA2LPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    FLITFLPEN     LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    CRCLPEN       LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    CORDICLPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    FMACLPEN      LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    RAMCFGLPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    ETHLPEN       LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    ETHTXLPEN     LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    ETHRXLPEN     LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    ETHCKLPEN     LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    TZSC1LPEN     LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    BKPRAMLPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    DCACHE1LPEN    LL_AHB1_GRP1_DisableClockSleep\n
  *         AHB1LPENR    SRAM1LPEN     LL_AHB1_GRP1_DisableClockSleep
  *         AHB1LPENR    MDF1LPEN     LL_AHB1_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GPDMA2
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FLASH
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CRC
  *         @arg @ref LL_AHB1_GRP1_PERIPH_CORDIC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_FMAC (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_RAMCFG
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETH (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHTX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_ETHRX (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_GTZC1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_BKPSRAM
  *         @arg @ref LL_AHB1_GRP1_PERIPH_DCACHE1 (*)
  *         @arg @ref LL_AHB1_GRP1_PERIPH_SRAM1
  *         @arg @ref LL_AHB1_GRP1_PERIPH_MDF1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB1_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB1LPENR, Periphs);
}

/**
  * @}
  */

/** @defgroup BUS_LL_EF_AHB2 AHB2 Peripherals
  * @{
  */
/**
  * @brief  Enable AHB2 peripherals clock.
  * @rmtoll AHB2ENR      GPIOAEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOBEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOCEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIODEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOEEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOFEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOGEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOHEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      GPIOIEN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      ADCEN         LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      DAC1EN        LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      DCMI_PSSIEN   LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      AESEN         LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      HASHEN        LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      RNGEN         LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      PKAEN         LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      SAESEN        LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      SRAM2EN       LL_AHB2_GRP1_EnableClock\n
  *         AHB2ENR      SRAM3EN       LL_AHB2_GRP1_EnableClock
  *         AHB2ENR      OTGFSEN       LL_AHB2_GRP1_EnableClock
  *         AHB2ENR      OTGHSEN       LL_AHB2_GRP1_EnableClock
  *         AHB2ENR      ADF1EN        LL_AHB2_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB2ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB2ENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB2 peripheral clock is enabled or not
  * @rmtoll AHB2ENR      GPIOAEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOBEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOCEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIODEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOEEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOFEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOGEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOHEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      GPIOIEN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      ADCEN         LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      DAC1EN        LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      DCMI_PSSIEN   LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      AESEN         LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      HASHEN        LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      RNGEN         LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      PKAEN         LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      SAESEN        LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      SRAM2EN       LL_AHB2_GRP1_IsEnabledClock\n
  *         AHB2ENR      SRAM3EN       LL_AHB2_GRP1_IsEnabledClock
  *         AHB2ENR      OTGFSEN       LL_AHB2_GRP1_IsEnabledClock
  *         AHB2ENR      OTGHSEN       LL_AHB2_GRP1_IsEnabledClock
  *         AHB2ENR      ADF1EN        LL_AHB2_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB2_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB2ENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB2 peripherals clock.
  * @rmtoll AHB2ENR      GPIOAEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOBEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOCEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIODEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOEEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOFEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOGEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOHEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      GPIOIEN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      ADCEN         LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      DAC1EN        LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      DCMI_PSSIEN   LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      AESEN         LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      HASHEN        LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      RNGEN         LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      PKAEN         LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      SAESEN        LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      SRAM2EN       LL_AHB2_GRP1_DisableClock\n
  *         AHB2ENR      SRAM3EN       LL_AHB2_GRP1_DisableClock
  *         AHB2ENR      OTGFSEN       LL_AHB2_GRP1_DisableClock
  *         AHB2ENR      OTGHSEN       LL_AHB2_GRP1_DisableClock
  *         AHB2ENR      ADF1EN        LL_AHB2_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB2ENR, Periphs);
}

/**
  * @brief  Force AHB2 peripherals reset.
  * @rmtoll AHB2RST     GPIOARST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOBRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOCRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIODRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOERST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOFRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOGRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOHRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     GPIOIRST      LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     ADCRST        LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     DAC1RST       LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     DCMI_PSSIRST  LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     AESRST        LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     HASHRST       LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     RNGRST        LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     PKARST        LL_AHB2_GRP1_ForceReset\n
  *         AHB2RST     SAESRST       LL_AHB2_GRP1_ForceReset
  *         AHB2RST     OTGFSRST      LL_AHB2_GRP1_ForceReset
  *         AHB2RST     OTGHSRST      LL_AHB2_GRP1_ForceReset
  *         AHB2RST     ADF1RST       LL_AHB2_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->AHB2RSTR, Periphs);
}

/**
  * @brief  Release AHB2 peripherals reset.
  * @rmtoll AHB2RST     GPIOARST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOBRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOCRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIODRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOERST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOFRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOGRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOHRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     GPIOIRST      LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     ADCRST        LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     DAC1RST       LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     DCMI_PSSIRST  LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     AESRST        LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     HASHRST       LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     RNGRST        LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     PKARST        LL_AHB2_GRP1_ReleaseReset\n
  *         AHB2RST     SAESRST       LL_AHB2_GRP1_ReleaseReset
  *         AHB2RST     OTGFSRST       LL_AHB2_GRP1_ReleaseReset
  *         AHB2RST     OTGHSRST       LL_AHB2_GRP1_ReleaseReset
  *         AHB2RST     ADF1RST       LL_AHB2_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB2RSTR, Periphs);
}

/**
  * @brief  Enable AHB2 peripheral clocks in Sleep mode
  * @rmtoll AHB2LPENR    GPIOALPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOBLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOCLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIODLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOELPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOFLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOGLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOHLPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    GPIOILPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    ADCLPEN        LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    DAC1LPEN       LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    DCMI_PSSILPEN  LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    AESLPEN        LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    HASHLPEN       LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    RNGLPEN        LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    PKALPEN        LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    SAESLPEN       LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    SRAM2LPEN      LL_AHB2_GRP1_EnableClockSleep\n
  *         AHB2LPENR    SRAM3LPEN      LL_AHB2_GRP1_EnableClockSleep
  *         AHB2LPENR    OTGFSLPEN      LL_AHB2_GRP1_EnableClockSleep
  *         AHB2LPENR    OTGHSLPEN      LL_AHB2_GRP1_EnableClockSleep
  *         AHB2LPENR    ADF1LPEN       LL_AHB2_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB2LPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB2LPENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB2 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll AHB2LPENR    GPIOALPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOBLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOCLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIODLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOELPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOFLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOGLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOHLPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    GPIOILPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    ADCLPEN        LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    DAC1LPEN       LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    DCMI_PSSILPEN  LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    AESLPEN        LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    HASHLPEN       LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    RNGLPEN        LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    PKALPEN        LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    SAESLPEN       LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    SRAM2LPEN      LL_AHB2_GRP1_IsEnabledClockSleep\n
  *         AHB2LPENR    SRAM3LPEN      LL_AHB2_GRP1_IsEnabledClockSleep
  *         AHB2LPENR    OTGFSLPEN      LL_AHB2_GRP1_IsEnabledClockSleep
  *         AHB2LPENR    OTGHSLPEN      LL_AHB2_GRP1_IsEnabledClockSleep
  *         AHB2LPENR    ADF1LPEN       LL_AHB2_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB2_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB2LPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB2 peripheral clocks in Sleep mode
  * @rmtoll AHB2LPENR    GPIOALPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOBLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOCLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIODLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOELPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOFLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOGLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOHLPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    GPIOILPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    ADCLPEN        LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    DAC1LPEN       LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    DCMI_PSSILPEN  LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    AESLPEN        LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    HASHLPEN       LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    RNGLPEN        LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    PKALPEN        LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    SAESLPEN       LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    SRAM2LPEN      LL_AHB2_GRP1_DisableClockSleep\n
  *         AHB2LPENR    SRAM3LPEN      LL_AHB2_GRP1_DisableClockSleep
  *         AHB2LPENR    OTGFSLPEN      LL_AHB2_GRP1_DisableClockSleep
  *         AHB2LPENR    OTGHSLPEN      LL_AHB2_GRP1_DisableClockSleep
  *         AHB2LPENR    ADF1LPEN       LL_AHB2_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOE (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOF (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOG (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB2_GRP1_PERIPH_GPIOI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADC
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DAC1
  *         @arg @ref LL_AHB2_GRP1_PERIPH_DCMI_PSSI (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_AES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_HASH (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_RNG
  *         @arg @ref LL_AHB2_GRP1_PERIPH_PKA (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SAES (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM2 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_SRAM3 (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGFS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_OTGHS (*)
  *         @arg @ref LL_AHB2_GRP1_PERIPH_ADF1 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB2_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB2LPENR, Periphs);
}

/**
  * @}
  */

#if defined(AHB4PERIPH_BASE)
/** @defgroup BUS_LL_EF_AHB4 AHB4 Peripherals
  * @{
  */
/**
  * @brief  Enable AHB4 peripherals clock.
  * @rmtoll AHB4ENR  OTFDEC1EN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR  SDMMC1EN        LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR  SDMMC2EN        LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR  FMCEN           LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR  OCTOSPI1EN      LL_AHB4_GRP1_EnableClock
  *         AHB4ENR  OCTOSPI2EN      LL_AHB4_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2 (*)
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB4ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB4ENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB4 peripheral clock is enabled or not
  * @rmtoll AHB4ENR  OTFDEC1EN        LL_AHB4_GRP1_IsEnabledClock\n
  *         AHB4ENR  SDMMC1EN         LL_AHB4_GRP1_IsEnabledClock\n
  *         AHB4ENR  SDMMC2EN         LL_AHB4_GRP1_IsEnabledClock\n
  *         AHB4ENR  FMCEN            LL_AHB4_GRP1_IsEnabledClock\n
  *         AHB4ENR  OCTOSPI1EN       LL_AHB4_GRP1_IsEnabledClock
  *         AHB4ENR  OCTOSPI2EN       LL_AHB4_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB4_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB4ENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB4 peripherals clock.
  * @rmtoll AHB4ENR  OTFDEC1EN       LL_AHB4_GRP1_DisableClock\n
  *         AHB4ENR  SDMMC1EN        LL_AHB4_GRP1_DisableClock\n
  *         AHB4ENR  SDMMC2EN        LL_AHB4_GRP1_DisableClock\n
  *         AHB4ENR  FMCEN           LL_AHB4_GRP1_DisableClock\n
  *         AHB4ENR  OCTOSPI1EN      LL_AHB4_GRP1_DisableClock
  *         AHB4ENR  OCTOSPI2EN      LL_AHB4_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB4ENR, Periphs);
}

/**
  * @brief  Force AHB4 peripherals reset.
  * @rmtoll AHB4RSTR  OTFDEC1RST      LL_AHB4_GRP1_ForceReset\n
  *         AHB4RSTR  SDMMC1RST       LL_AHB4_GRP1_ForceReset\n
  *         AHB4RSTR  SDMMC2RST       LL_AHB4_GRP1_ForceReset\n
  *         AHB4RSTR  FMCRST          LL_AHB4_GRP1_ForceReset\n
  *         AHB4RSTR  OCTOSPI1RST     LL_AHB4_GRP1_ForceReset
  *         AHB4RSTR  OCTOSPI2RST     LL_AHB4_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->AHB4RSTR, Periphs);
}

/**
  * @brief  Release AHB4 peripherals reset.
  * @rmtoll AHB4RSTR  OTFDEC1RST        LL_AHB4_GRP1_ReleaseReset\n
  *         AHB4RSTR  SDMMC1RST         LL_AHB4_GRP1_ReleaseReset\n
  *         AHB4RSTR  SDMMC2RST         LL_AHB4_GRP1_ReleaseReset\n
  *         AHB4RSTR  FMCRST            LL_AHB4_GRP1_ReleaseReset\n
  *         AHB4RSTR  OCTOSPI1RST       LL_AHB4_GRP1_ReleaseReset
  *         AHB4RSTR  OCTOSPI2RST       LL_AHB4_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB4RSTR, Periphs);
}

/**
  * @brief  Enable AHB4 peripheral clocks in Sleep mode
  * @rmtoll AHB4LPENR  OTFDEC1LPEN       LL_AHB4_GRP1_EnableClockSleep\n
  *         AHB4LPENR  SDMMC1LPEN        LL_AHB4_GRP1_EnableClockSleep\n
  *         AHB4LPENR  SDMMC2LPEN        LL_AHB4_GRP1_EnableClockSleep\n
  *         AHB4LPENR  FMCLPEN           LL_AHB4_GRP1_EnableClockSleep\n
  *         AHB4LPENR  OCTOSPI1LPEN      LL_AHB4_GRP1_EnableClockSleep
  *         AHB4LPENR  OCTOSPI2LPEN      LL_AHB4_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB4LPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB4LPENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if AHB4 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll AHB4LPENR  OTFDEC1LPEN       LL_AHB4_GRP1_IsEnabledClockSleep\n
  *         AHB4LPENR  SDMMC1LPEN        LL_AHB4_GRP1_IsEnabledClockSleep\n
  *         AHB4LPENR  SDMMC2LPEN        LL_AHB4_GRP1_IsEnabledClockSleep\n
  *         AHB4LPENR  FMCLPEN           LL_AHB4_GRP1_IsEnabledClockSleep\n
  *         AHB4LPENR  OCTOSPI1LPEN      LL_AHB4_GRP1_IsEnabledClockSleep
  *         AHB4LPENR  OCTOSPI2LPEN      LL_AHB4_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_AHB4_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->AHB4LPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable AHB4 peripheral clocks in Sleep and Stop modes
  * @rmtoll AHB4LPENR  OTFDEC1LPEN       LL_AHB4_GRP1_DisableClockSleep\n
  *         AHB4LPENR  SDMMC1LPEN        LL_AHB4_GRP1_DisableClockSleep\n
  *         AHB4LPENR  SDMMC2LPEN        LL_AHB4_GRP1_DisableClockSleep\n
  *         AHB4LPENR  FMCLPEN           LL_AHB4_GRP1_DisableClockSleep\n
  *         AHB4LPENR  OCTOSPI1LPEN      LL_AHB4_GRP1_DisableClockSleep
  *         AHB4LPENR  OCTOSPI2LPEN      LL_AHB4_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ALL
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OTFDEC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SDMMC2
  *         @arg @ref LL_AHB4_GRP1_PERIPH_FMC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI1
  *         @arg @ref LL_AHB4_GRP1_PERIPH_OSPI2
  * @retval None
  */
__STATIC_INLINE void LL_AHB4_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->AHB4LPENR, Periphs);
}

/**
  * @}
  */
#endif /* AHB4PERIPH_BASE */

/** @defgroup BUS_LL_EF_APB1 APB1 Peripherals
  * @{
  */

/**
  * @brief  Enable APB1 peripherals clock.
  * @rmtoll APB1LENR     TIM2EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM3EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM4EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM5EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM6EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM7EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM12EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM13EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     TIM14EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     WWDGEN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     SPI2EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     SPI3EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     USART2EN      LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     USART3EN      LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     UART4EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     UART5EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     I2C1EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     I2C2EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     I3C1EN        LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     CRSEN         LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     USART6EN      LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     USART10EN     LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     USART11EN     LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     CECEN         LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     UART7EN       LL_APB1_GRP1_EnableClock\n
  *         APB1LENR     UART8EN       LL_APB1_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1LENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1LENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Enable APB1 peripherals clock.
  * @rmtoll APB1HENR     UART9EN        LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     UART12EN       LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     DTSEN          LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     LPTIM2EN       LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     FDCANEN        LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     FDCANEN        LL_APB1_GRP2_EnableClock\n
  *         APB1HENR     UCPD1EN        LL_APB1_GRP2_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1HENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1HENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if APB1 peripheral clock is enabled or not
  * @rmtoll APB1LENR     TIM2EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     TIM3EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     TIM4EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     TIM5EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     TIM6EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     TIM7EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     WWDGEN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     SPI2EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     SPI3EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     USART2EN      LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     USART3EN      LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     UART4EN       LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     UART5EN       LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     I2C1EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     I2C2EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     I3C1EN        LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     CRSEN         LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     USART6EN      LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     USART10EN     LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     USART11EN     LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     CECEN         LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     UART7EN       LL_APB1_GRP1_IsEnabledClock\n
  *         APB1LENR     UART8EN       LL_APB1_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB1_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB1LENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Check if APB1 peripheral clock is enabled or not
  * @rmtoll APB1HENR     UART9EN       LL_APB1_GRP2_IsEnabledClock\n
  *         APB1HENR     UART12EN      LL_APB1_GRP2_IsEnabledClock\n
  *         APB1HENR     DTSEN         LL_APB1_GRP2_IsEnabledClock\n
  *         APB1HENR     LPTIM2EN      LL_APB1_GRP2_IsEnabledClock\n
  *         APB1HENR     FDCANEN       LL_APB1_GRP2_IsEnabledClock\n
  *         APB1HENR     UCPD1EN       LL_APB1_GRP2_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB1_GRP2_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB1HENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB1 peripherals clock.
  * @rmtoll APB1LENR     TIM2EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     TIM3EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     TIM4EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     TIM5EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     TIM6EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     TIM7EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     WWDGEN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     SPI2EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     SPI3EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     USART2EN       LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     USART3EN       LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     UART4EN        LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     UART5EN        LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     I2C1EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     I2C2EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     I3C1EN         LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     CRSEN          LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     USART6EN       LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     USART10EN      LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     USART11EN      LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     CECEN          LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     UART7EN        LL_APB1_GRP1_DisableClock\n
  *         APB1LENR     UART8EN        LL_APB1_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1LENR, Periphs);
}

/**
  * @brief  Disable APB1 peripherals clock.
  * @rmtoll APB1HENR     UART9EN        LL_APB1_GRP2_DisableClock\n
  *         APB1HENR     UART12EN       LL_APB1_GRP2_DisableClock\n
  *         APB1HENR     DTSEN          LL_APB1_GRP2_DisableClock\n
  *         APB1HENR     LPTIM2EN       LL_APB1_GRP2_DisableClock\n
  *         APB1HENR     FDCANEN       LL_APB1_GRP2_DisableClock\n
  *         APB1HENR     UCPD1EN        LL_APB1_GRP2_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1HENR, Periphs);
}

/**
  * @brief  Force APB1 peripherals reset.
  * @rmtoll APB1LRSTR     TIM2RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     TIM3RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     TIM4RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     TIM5RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     TIM6RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     TIM7RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     SPI2RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     SPI3RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     USART2RST      LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     USART3RST      LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     UART4RST       LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     UART5RST       LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     I2C1RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     I2C2RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     I3C1RST        LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     CRSRST         LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     USART6RST      LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     USART10RST     LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     USART11RST     LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     CECRST         LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     UART7RST       LL_APB1_GRP1_ForceReset\n
  *         APB1LRSTR     UART8RST       LL_APB1_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->APB1LRSTR, Periphs);
}

/**
  * @brief  Force APB1 peripherals reset.
  * @rmtoll APB1HRSTR     UART9RST       LL_APB1_GRP2_ForceReset\n
  *         APB1HRSTR     UART12RST      LL_APB1_GRP2_ForceReset\n
  *         APB1HRSTR     DTSRST         LL_APB1_GRP2_ForceReset\n
  *         APB1HRSTR     LPTIM2RST      LL_APB1_GRP2_ForceReset\n
  *         APB1HRSTR     FDCANRST       LL_APB1_GRP2_ForceReset\n
  *         APB1HRSTR     UCPD1RST       LL_APB1_GRP2_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->APB1HRSTR, Periphs);
}

/**
  * @brief  Release APB1 peripherals reset.
  * @rmtoll APB1LRSTR     TIM2RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     TIM3RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     TIM4RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     TIM5RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     TIM6RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     TIM7RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     SPI2RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     SPI3RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     USART2RST      LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     USART3RST      LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     UART4RST       LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     UART5RST       LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     I2C1RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     I2C2RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     I3C1RST        LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     CRSRST         LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     USART6RST      LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     USART10RST     LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     USART11RST     LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     CECRST         LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     UART7RST       LL_APB1_GRP1_ReleaseReset\n
  *         APB1LRSTR     UART8RST       LL_APB1_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1LRSTR, Periphs);
}

/**
  * @brief  Release APB1 peripherals reset.
  * @rmtoll APB1HRSTR     UART9RST       LL_APB1_GRP2_ReleaseReset\n
  *         APB1HRSTR     UART12RST      LL_APB1_GRP2_ReleaseReset\n
  *         APB1HRSTR     DTSRST         LL_APB1_GRP2_ReleaseReset\n
  *         APB1HRSTR     LPTIM2RST      LL_APB1_GRP2_ReleaseReset\n
  *         APB1HRSTR     FDCAN          LL_APB1_GRP2_ReleaseReset\n
  *         APB1HRSTR     UCPD1RST       LL_APB1_GRP2_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1HRSTR, Periphs);
}

/**
  * @brief  Enable APB1 peripheral clocks in Sleep mode
  * @rmtoll APB1LLPENR     TIM2LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     TIM3LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     TIM4LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     TIM5LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     TIM6LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     TIM7LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     WWDGLPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     SPI2LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     SPI3LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     USART2LPEN      LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     USART3LPEN      LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     UART4LPEN       LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     UART5LPEN       LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     I2C1LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     I2C2LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     I3C1LPEN        LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     CRSLPEN         LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     USART6LPEN      LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     USART10LPEN     LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     USART11LPEN     LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     CECLPEN         LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     UART7LPEN       LL_APB1_GRP1_EnableClockSleep\n
  *         APB1LLPENR     UART8LPEN       LL_APB1_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1LLPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1LLPENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if APB1 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll APB1LLPENR     TIM2LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     TIM3LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     TIM4LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     TIM5LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     TIM6LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     TIM7LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     WWDGLPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     SPI2LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     SPI3LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     USART2LPEN      LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     USART3LPEN      LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     UART4LPEN       LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     UART5LPEN       LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     I2C1LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     I2C2LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     I3C1LPEN        LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     CRSLPEN         LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     USART6LPEN      LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     USART10LPEN     LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     USART11LPEN     LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     CECLPEN         LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     UART7LPEN       LL_APB1_GRP1_IsEnabledClockSleep\n
  *         APB1LLPENR     UART8LPEN       LL_APB1_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB1_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB1LLPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB1 peripheral clocks in Sleep mode
  * @rmtoll APB1LLPENR     TIM2LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     TIM3LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     TIM4LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     TIM5LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     TIM6LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     TIM7LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     WWDGLPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     SPI2LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     SPI3LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     USART2LPEN      LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     USART3LPEN      LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     UART4LPEN       LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     UART5LPEN       LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     I2C1LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     I2C2LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     I3C1LPEN        LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     CRSLPEN         LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     USART6LPEN      LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     USART10LPEN     LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     USART11LPEN     LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     CECLPEN         LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     UART7LPEN       LL_APB1_GRP1_DisableClockSleep\n
  *         APB1LLPENR     UART8LPEN       LL_APB1_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM2
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM3
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM6
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM7
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM12 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM13 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_TIM14 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_WWDG
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI2
  *         @arg @ref LL_APB1_GRP1_PERIPH_SPI3
  *         @arg @ref LL_APB1_GRP1_PERIPH_OPAMP (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_COMP  (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART2
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART3
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART4 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART5 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_I2C2
  *         @arg @ref LL_APB1_GRP1_PERIPH_I3C1
  *         @arg @ref LL_APB1_GRP1_PERIPH_CRS
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART6 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART10 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_USART11 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_CEC (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART7 (*)
  *         @arg @ref LL_APB1_GRP1_PERIPH_UART8 (*)
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1LLPENR, Periphs);
}

/**
  * @brief  Enable APB1 peripheral clocks in Sleep mode
  * @rmtoll APB1HLPENR     UART9LPEN       LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     UART12LPEN      LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     DTSLPEN         LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     LPTIM2LPEN      LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     FDCAN12LPEN     LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     FDCAN1LPEN      LL_APB1_GRP2_EnableClockSleep\n
  *         APB1HLPENR     UCPD1LPEN       LL_APB1_GRP2_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB1HLPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1HLPENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if APB1 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll APB1HLPENR     UART9LPEN       LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     UART12LPEN      LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     DTSLPEN         LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     LPTIM2LPEN      LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     FDCAN12LPEN     LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     FDCAN1LPEN      LL_APB1_GRP2_IsEnabledClockSleep\n
  *         APB1HLPENR     UCPD1LPEN       LL_APB1_GRP2_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB1_GRP2_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB1HLPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB1 peripheral clocks in Sleep mode
  * @rmtoll APB1HLPENR     UART9LPEN       LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     UART12LPEN      LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     DTSLPEN         LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     LPTIM2LPEN      LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     FDCAN12LPEN     LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     FDCAN1LPEN      LL_APB1_GRP2_DisableClockSleep\n
  *         APB1HLPENR     UCPD1LPEN       LL_APB1_GRP2_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB1_GRP2_PERIPH_ALL
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART9 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_UART12 (*)
  *         @arg @ref LL_APB1_GRP2_PERIPH_DTS
  *         @arg @ref LL_APB1_GRP2_PERIPH_LPTIM2
  *         @arg @ref LL_APB1_GRP2_PERIPH_FDCAN
  *         @arg @ref LL_APB1_GRP2_PERIPH_UCPD1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB1_GRP2_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB1HLPENR, Periphs);
}

/**
  * @}
  */

/** @defgroup BUS_LL_EF_APB2 APB2 Peripherals
  * @{
  */

/**
  * @brief  Enable APB2 peripherals clock.
  * @rmtoll APB2ENR      TIM1EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      SPI1EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      TIM8EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      USART1EN      LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      TIM15EN       LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      TIM16EN       LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      TIM17EN       LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      SPI4EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      SPI6EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      SAI1EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      SAI2EN        LL_APB2_GRP1_EnableClock\n
  *         APB2ENR      USBEN         LL_APB2_GRP1_EnableClock
  *         APB2ENR      LTDCEN        LL_APB2_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB  (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB2ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB2ENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if APB2 peripheral clock is enabled or not
  * @rmtoll APB2ENR      TIM1EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      SPI1EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      TIM8EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      USART1EN      LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      TIM15EN       LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      TIM16EN       LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      TIM17EN       LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      SPI4EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      SPI6EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      SAI1EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      SAI2EN        LL_APB2_GRP1_IsEnabledClock\n
  *         APB2ENR      USBEN         LL_APB2_GRP1_IsEnabledClock
  *         APB2ENR      LTDCEN         LL_APB2_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB  (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB2_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB2ENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB2 peripherals clock.
  * @rmtoll APB2ENR      TIM1EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      SPI1EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      TIM8EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      USART1EN      LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      TIM15EN       LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      TIM16EN       LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      TIM17EN       LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      SPI4EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      SPI6EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      SAI1EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      SAI2EN        LL_APB2_GRP1_DisableClock\n
  *         APB2ENR      USBEN         LL_APB2_GRP1_DisableClock
  *         APB2ENR      LTDCEN         LL_APB2_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB2ENR, Periphs);
}

/**
  * @brief  Force APB2 peripherals reset.
  * @rmtoll APB2RSTR      TIM1RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      SPI1RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      TIM8RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      USART1RST      LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      TIM15RST       LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      TIM16RST       LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      TIM17RST       LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      SPI4RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      SPI6RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      SAI1RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      SAI2RST        LL_APB2_GRP1_ForceReset\n
  *         APB2RSTR      USBRST         LL_APB2_GRP1_ForceReset
  *         APB2RSTR      LTDCRST         LL_APB2_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->APB2RSTR, Periphs);
}

/**
  * @brief  Release APB2 peripherals reset.
  * @rmtoll APB2RSTR      TIM1RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      SPI1RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      TIM8RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      USART1RST      LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      TIM15RST       LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      TIM16RST       LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      TIM17RST       LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      SPI4RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      SPI6RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      SAI1RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      SAI2RST        LL_APB2_GRP1_ReleaseReset\n
  *         APB2RSTR      USBRST         LL_APB2_GRP1_ReleaseReset
  *         APB2RSTR      LTDCRST         LL_APB2_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB2RSTR, Periphs);
}

/**
  * @brief  Enable APB2 peripheral clocks in Sleep mode
  * @rmtoll APB2LPENR     TIM1LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     SPI1LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     TIM8LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     USART1LPEN     LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     TIM15LPEN      LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     TIM16LPEN      LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     TIM17LPEN      LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     SPI4LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     SPI6LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     SAI1LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     SAI2LPEN       LL_APB2_GRP1_EnableClockSleep\n
  *         APB2LPENR     USBLPEN        LL_APB2_GRP1_EnableClockSleep
  *         APB2LPENR     LTDCLPEN        LL_APB2_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB2LPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB2LPENR, Periphs);
  (void)tmpreg;
}


/**
  * @brief  Check if APB2 peripheral clocks in Sleep and Stop modes is enabled or not
  * @rmtoll APB2LPENR     TIM1LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     SPI1LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     TIM8LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     USART1LPEN     LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     TIM15LPEN      LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     TIM16LPEN      LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     TIM17LPEN      LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     SPI4LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     SPI6LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     SAI1LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     SAI2LPEN       LL_APB2_GRP1_IsEnabledClockSleep\n
  *         APB2LPENR     USBLPEN        LL_APB2_GRP1_IsEnabledClockSleep
  *         APB2LPENR     LTDCLPEN        LL_APB2_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB2_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB2LPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB2 peripheral clocks in Sleep mode
  * @rmtoll APB2LPENR     TIM1LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     SPI1LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     TIM8LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     USART1LPEN      LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     TIM15LPEN       LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     TIM16LPEN       LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     TIM17LPEN       LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     SPI4LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     SPI6LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     SAI1LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     SAI2LPEN        LL_APB2_GRP1_DisableClockSleep\n
  *         APB2LPENR     USBLPEN         LL_APB2_GRP1_DisableClockSleep
  *         APB2LPENR     LTDCLPEN         LL_APB2_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB2_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM1
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM8 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USART1
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM15 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM16 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_TIM17 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI4 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SPI6 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI1 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_SAI2 (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_USB (*)
  *         @arg @ref LL_APB2_GRP1_PERIPH_LTDC
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB2_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB2LPENR, Periphs);
}

/**
  * @}
  */


/** @defgroup BUS_LL_EF_APB3 APB3 Peripherals
  */

/**
  * @brief  Enable APB3 peripherals clock.
  * @rmtoll APB3ENR      SBSEN          LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      SPI5EN         LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPUART1EN      LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      I2C3EN         LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      I2C4EN         LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPTIM1EN       LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPTIM3EN       LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPTIM4EN       LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPTIM5EN       LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      LPTIM6EN       LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      VREFEN         LL_APB3_GRP1_EnableClock\n
  *         APB3ENR      RTCAPBEN       LL_APB3_GRP1_EnableClock
  *         APB3ENR      PLAY1EN        LL_APB3_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB3ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB3ENR, Periphs);
  (void)tmpreg;
}

/**
  * @brief  Check if APB3 peripheral clock is enabled or not
  * @rmtoll APB3ENR      SBSEN          LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      SPI5EN         LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPUART1EN      LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      I2C3EN         LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      I2C4EN         LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPTIM1EN       LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPTIM3EN       LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPTIM4EN       LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPTIM5EN       LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      LPTIM6EN       LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      VREFEN         LL_APB3_GRP1_IsEnabledClock\n
  *         APB3ENR      RTCAPBEN       LL_APB3_GRP1_IsEnabledClock
  *         APB3ENR      PLAY1EN        LL_APB3_GRP1_IsEnabledClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB3_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB3ENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB2 peripherals clock.
  * @rmtoll APB3ENR      SBSEN          LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      SPI5EN         LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPUART1EN      LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      I2C3EN         LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      I2C4EN         LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPTIM1EN       LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPTIM3EN       LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPTIM4EN       LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPTIM5EN       LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      LPTIM6EN       LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      VREFEN         LL_APB3_GRP1_DisableClock\n
  *         APB3ENR      RTCAPBEN       LL_APB3_GRP1_DisableClock
  *         APB3ENR      PLAY1EN        LL_APB3_GRP1_DisableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_DisableClock(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB3ENR, Periphs);
}

/**
  * @brief  Force APB3 peripherals reset.
  * @rmtoll APB3RSTR      SPI5RST        LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPUART1RST     LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      I2C3RST        LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      I2C4RST        LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPTIM1RST      LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPTIM3RST      LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPTIM4RST      LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPTIM5RST      LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      LPTIM6RST      LL_APB3_GRP1_ForceReset\n
  *         APB3RSTR      VREFRST        LL_APB3_GRP1_ForceReset
  *         APB3RSTR      PLAY1          LL_APB3_GRP1_ForceReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_ForceReset(uint32_t Periphs)
{
  SET_BIT(RCC->APB3RSTR, Periphs);
}

/**
  * @brief  Release APB3 peripherals reset.
  * @rmtoll APB3RSTR      SPI5RST        LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPUART1RST     LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      I2C3RST        LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      I2C4RST        LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPTIM1RST      LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPTIM3RST      LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPTIM4RST      LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPTIM5RST      LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      LPTIM6RST      LL_APB3_GRP1_ReleaseReset\n
  *         APB3RSTR      VREFRST        LL_APB3_GRP1_ReleaseReset
  *         APB3RSTR      PLAY1          LL_APB3_GRP1_ReleaseReset
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_ReleaseReset(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB3RSTR, Periphs);
}

/**
  * @brief  Enable APB3 peripheral clocks in Sleep mode
  * @rmtoll APB3LPENR      SBSLPEN       LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      SPI5LPEN      LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPUART1LPEN   LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      I2C3LPEN      LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      I2C4LPEN      LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPTIM1LPEN    LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPTIM3LPEN    LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPTIM4LPEN    LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPTIM5LPEN    LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      LPTIM6LPEN    LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      VREFLPEN      LL_APB3_GRP1_EnableClockSleep\n
  *         APB3LPENR      RTCAPBLPEN    LL_APB3_GRP1_EnableClockSleep
  *         APB3LPENR      PAYLPEN       LL_APB3_GRP1_EnableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_EnableClockSleep(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->APB3LPENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB3LPENR, Periphs);
  (void)tmpreg;
}


/**
  * @brief  Check if APB3 peripheral clocks in Sleep mode is enabled or not
  * @rmtoll APB3LPENR      SBSLPEN       LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      SPI5LPEN      LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPUART1LPEN   LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      I2C3LPEN      LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      I2C4LPEN      LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPTIM1LPEN    LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPTIM3LPEN    LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPTIM4LPEN    LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPTIM5LPEN    LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      LPTIM6LPEN    LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      VREFLPEN      LL_APB3_GRP1_IsEnabledClockSleep\n
  *         APB3LPENR      RTCAPBLPEN    LL_APB3_GRP1_IsEnabledClockSleep
  *         APB3LPENR      PLAY1LPEN     LL_APB3_GRP1_IsEnabledClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval State of Periphs (1 or 0).
  */
__STATIC_INLINE uint32_t LL_APB3_GRP1_IsEnabledClockSleep(uint32_t Periphs)
{
  return ((READ_BIT(RCC->APB3LPENR, Periphs) == Periphs) ? 1UL : 0UL);
}

/**
  * @brief  Disable APB3 peripheral clocks in Sleep mode
  * @rmtoll APB3LPENR      SBSLPEN        LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      SPI5LPEN       LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPUART1LPEN    LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      I2C3LPEN       LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      I2C4LPEN       LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPTIM1LPEN     LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPTIM3LPEN     LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPTIM4LPEN     LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPTIM5LPEN     LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      LPTIM6LPEN     LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      VREFLPEN       LL_APB3_GRP1_DisableClockSleep\n
  *         APB3LPENR      RTCAPBLPEN     LL_APB3_GRP1_DisableClockSleep
  *         APB3LPENR      PLAY1LPEN      LL_APB3_GRP1_DisableClockSleep
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_APB3_GRP1_PERIPH_ALL
  *         @arg @ref LL_APB3_GRP1_PERIPH_SBS
  *         @arg @ref LL_APB3_GRP1_PERIPH_SPI5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPUART1
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I2C4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_I3C2 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM1
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM3 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_LPTIM6 (*)
  *         @arg @ref LL_APB3_GRP1_PERIPH_VREF
  *         @arg @ref LL_APB3_GRP1_PERIPH_RTCAPB
  *         @arg @ref LL_APB3_GRP1_PERIPH_PLAY1
  *
  *  (*)  : Not available for all stm32h5xxxx family lines.
  * @retval None
  */
__STATIC_INLINE void LL_APB3_GRP1_DisableClockSleep(uint32_t Periphs)
{
  CLEAR_BIT(RCC->APB3LPENR, Periphs);
}

/**
  * @}
  */

/**
  * @}
  */
#endif /* defined(RCC) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32H5xx_LL_BUS_H */

