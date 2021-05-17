#pragma once

#define HAL_ETH_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED

#include "stm32f7xx.h"
#include "stm32f7xx_hal_cortex.h"
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_gpio.h"

#define UID_BASE 0x1FF0F420UL
#define __HAL_FLASH_ART_ENABLE() SET_BIT(FLASH->ACR, FLASH_ACR_ARTEN)
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE() (FLASH->ACR |= FLASH_ACR_PRFTEN)

#define assert_param(expr) ((void) 0U)

#define HAL_MODULE_ENABLED
#define HAL_ADC_MODULE_ENABLED
#define HAL_CAN_MODULE_ENABLED
#define HAL_CEC_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_CRYP_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_DCMI_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_ETH_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
//#define HAL_NAND_MODULE_ENABLED
#define HAL_NOR_MODULE_ENABLED
#define HAL_SRAM_MODULE_ENABLED
#define HAL_SDRAM_MODULE_ENABLED
#define HAL_HASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
//#define HAL_I2S_MODULE_ENABLED
//#define HAL_IWDG_MODULE_ENABLED
#define HAL_LPTIM_MODULE_ENABLED
#define HAL_LTDC_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_QSPI_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RNG_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
//#define HAL_SAI_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_IRDA_MODULE_ENABLED
#define HAL_SMARTCARD_MODULE_ENABLED
//#define HAL_WWDG_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
//#define HAL_PCD_MODULE_ENABLED
//#define HAL_HCD_MODULE_ENABLED

/* ########################## HSE/HSI Values adaptation ##################### */
/**
 * @brief Adjust the value of External High Speed oscillator (HSE) used in your
 * application. This value is used by the RCC HAL module to compute the system
 * frequency (when HSE is used as system clock source, directly or through the
 * PLL).
 */
#if !defined(HSE_VALUE)
#define HSE_VALUE \
  ((uint32_t) 25000000) /*!< Value of the External oscillator in Hz */
#endif                  /* HSE_VALUE */

#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT \
  ((uint32_t) 5000) /*!< Time out for HSE start up, in ms */
#endif              /* HSE_STARTUP_TIMEOUT */

/**
 * @brief Internal High Speed oscillator (HSI) value.
 *        This value is used by the RCC HAL module to compute the system
 * frequency (when HSI is used as system clock source, directly or through the
 * PLL).
 */
#if !defined(HSI_VALUE)
#define HSI_VALUE \
  ((uint32_t) 16000000) /*!< Value of the Internal oscillator in Hz*/
#endif                  /* HSI_VALUE */

/**
 * @brief Internal Low Speed oscillator (LSI) value.
 */
#if !defined(LSI_VALUE)
#define LSI_VALUE ((uint32_t) 40000)
#endif /* LSI_VALUE */ /*!< Value of the Internal Low Speed oscillator in Hz \
                        The real value may vary depending on the variations  \
                        in voltage and temperature.  */
/**
 * @brief External Low Speed oscillator (LSE) value.
 */
#if !defined(LSE_VALUE)
#define LSE_VALUE \
  ((uint32_t) 32768) /*!< Value of the External Low Speed oscillator in Hz */
#endif               /* LSE_VALUE */

/**
 * @brief External clock source for I2S peripheral
 *        This value is used by the I2S HAL module to compute the I2S clock
 * source frequency, this source is inserted directly through I2S_CKIN pad.
 */
#if !defined(EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE \
  ((uint32_t) 12288000) /*!< Value of the Internal oscillator in Hz*/
#endif                  /* EXTERNAL_CLOCK_VALUE */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
 * @brief This is the HAL system configuration section
 */
#define VDD_VALUE ((uint32_t) 3300)         /*!< Value of VDD in mv */
#define TICK_INT_PRIORITY ((uint32_t) 0x0F) /*!< tick interrupt priority */
#define USE_RTOS 0
#define ART_ACCLERATOR_ENABLE 1 /* To enable instruction cache and prefetch */

/* ########################## Assert Selection ############################## */
/**
 * @brief Uncomment the line below to expanse the "assert_param" macro in the
 *        HAL drivers code
 */
/* #define USE_FULL_ASSERT    1 */

/* ################## Ethernet peripheral configuration ##################### */

/* Section 1 : Ethernet peripheral configuration */

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0 2
#define MAC_ADDR1 0
#define MAC_ADDR2 0
#define MAC_ADDR3 0
#define MAC_ADDR4 0
#define MAC_ADDR5 0

/* Definition of the Ethernet driver buffers size and count */
#define ETH_RX_BUF_SIZE \
  ETH_MAX_PACKET_SIZE /* buffer size for receive               */
#define ETH_TX_BUF_SIZE \
  ETH_MAX_PACKET_SIZE              /* buffer size for transmit              */
#define ETH_RXBUFNB ((uint32_t) 8) /* 8 Rx buffers of size ETH_RX_BUF_SIZE  */
#define ETH_TXBUFNB ((uint32_t) 4) /* 4 Tx buffers of size ETH_TX_BUF_SIZE  */

/* Section 2: PHY configuration section */

/* DP83848 PHY Address*/
#define DP83848_PHY_ADDRESS 0x01
/* PHY Reset delay these values are based on a 1 ms Systick interrupt*/
#define PHY_RESET_DELAY ((uint32_t) 0x000000FF)
/* PHY Configuration delay */
#define PHY_CONFIG_DELAY ((uint32_t) 0x00000FFF)

#define PHY_READ_TO ((uint32_t) 0x0000FFFF)
#define PHY_WRITE_TO ((uint32_t) 0x0000FFFF)

/* Section 3: Common PHY Registers */

#define PHY_BCR ((uint16_t) 0x00) /*!< Transceiver Basic Control Register   */
#define PHY_BSR ((uint16_t) 0x01) /*!< Transceiver Basic Status Register    */

#define PHY_RESET ((uint16_t) 0x8000)    /*!< PHY Reset */
#define PHY_LOOPBACK ((uint16_t) 0x4000) /*!< Select loop-back mode */
#define PHY_FULLDUPLEX_100M \
  ((uint16_t) 0x2100) /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M \
  ((uint16_t) 0x2000) /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M \
  ((uint16_t) 0x0100) /*!< Set the full-duplex mode at 10 Mb/s  */
#define PHY_HALFDUPLEX_10M \
  ((uint16_t) 0x0000) /*!< Set the half-duplex mode at 10 Mb/s  */
#define PHY_AUTONEGOTIATION \
  ((uint16_t) 0x1000) /*!< Enable auto-negotiation function     */
#define PHY_RESTART_AUTONEGOTIATION \
  ((uint16_t) 0x0200) /*!< Restart auto-negotiation function    */
#define PHY_POWERDOWN \
  ((uint16_t) 0x0800) /*!< Select the power down mode           */
#define PHY_ISOLATE \
  ((uint16_t) 0x0400) /*!< Isolate PHY from MII                 */

#define PHY_AUTONEGO_COMPLETE \
  ((uint16_t) 0x0020) /*!< Auto-Negotiation process completed   */
#define PHY_LINKED_STATUS \
  ((uint16_t) 0x0004) /*!< Valid link established               */
#define PHY_JABBER_DETECTION \
  ((uint16_t) 0x0002) /*!< Jabber condition detected            */

/* Section 4: Extended PHY Registers */

#define PHY_SR \
  ((uint16_t) 0x10) /*!< PHY status register Offset                      */
#define PHY_MICR \
  ((uint16_t) 0x11) /*!< MII Interrupt Control Register                  */
#define PHY_MISR \
  ((uint16_t) 0x12) /*!< MII Interrupt Status and Misc. Control Register */

#define PHY_LINK_STATUS \
  ((uint16_t) 0x0001) /*!< PHY Link mask                                   */
#define PHY_SPEED_STATUS \
  ((uint16_t) 0x0002) /*!< PHY Speed mask                                  */
#define PHY_DUPLEX_STATUS \
  ((uint16_t) 0x0004) /*!< PHY Duplex mask                                 */

#define PHY_MICR_INT_EN \
  ((uint16_t) 0x0002) /*!< PHY Enable interrupts                           */
#define PHY_MICR_INT_OE \
  ((uint16_t) 0x0001) /*!< PHY Enable output interrupt events              */

#define PHY_MISR_LINK_INT_EN \
  ((uint16_t) 0x0020) /*!< Enable Interrupt on change of link status       */
#define PHY_LINK_INTERRUPT \
  ((uint16_t) 0x2000) /*!< PHY link status interrupt mask                  */

#if 0
#define HAL_ETH_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED

#include "stm32f7xx.h"
#include "stm32f7xx_hal_cortex.h"
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_rcc.h"

#define UID_BASE 0x1FF0F420UL
#define __HAL_FLASH_ART_ENABLE() SET_BIT(FLASH->ACR, FLASH_ACR_ARTEN)
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE() (FLASH->ACR |= FLASH_ACR_PRFTEN)

#define ETH_RXBUFNB 3
#define ETH_TXBUFNB 2
#define ETH_RX_BUF_SIZE (ipconfigNETWORK_MTU + 36)
#define ETH_TX_BUF_SIZE (ipconfigNETWORK_MTU + 36)

#define assert_param(expr) ((void) 0U)

#define LAN8742A_PHY_ADDRESS            0x00
#define PHY_RESET_DELAY                 ((uint32_t)0x00000FFF)
#define PHY_CONFIG_DELAY                ((uint32_t)0x00000FFF)
#define PHY_READ_TO                     ((uint32_t)0x0000FFFF)
#define PHY_WRITE_TO                    ((uint32_t)0x0000FFFF)
#define PHY_BCR                         ((uint16_t)0x00)    /*!< Transceiver Basic Control Register   */
#define PHY_BSR                         ((uint16_t)0x01)    /*!< Transceiver Basic Status Register    */
#define PHY_RESET                       ((uint16_t)0x8000)  /*!< PHY Reset */
#define PHY_LOOPBACK                    ((uint16_t)0x4000)  /*!< Select loop-back mode */
#define PHY_FULLDUPLEX_100M             ((uint16_t)0x2100)  /*!< Set the full-duplex mode at 100 Mb/s */
#define PHY_HALFDUPLEX_100M             ((uint16_t)0x2000)  /*!< Set the half-duplex mode at 100 Mb/s */
#define PHY_FULLDUPLEX_10M              ((uint16_t)0x0100)  /*!< Set the full-duplex mode at 10 Mb/s  */
#define PHY_HALFDUPLEX_10M              ((uint16_t)0x0000)  /*!< Set the half-duplex mode at 10 Mb/s  */
#define PHY_AUTONEGOTIATION             ((uint16_t)0x1000)  /*!< Enable auto-negotiation function     */
#define PHY_RESTART_AUTONEGOTIATION     ((uint16_t)0x0200)  /*!< Restart auto-negotiation function    */
#define PHY_POWERDOWN                   ((uint16_t)0x0800)  /*!< Select the power down mode           */
#define PHY_ISOLATE                     ((uint16_t)0x0400)  /*!< Isolate PHY from MII                 */
#define PHY_AUTONEGO_COMPLETE           ((uint16_t)0x0020)  /*!< Auto-Negotiation process completed   */
#define PHY_LINKED_STATUS               ((uint16_t)0x0004)  /*!< Valid link established               */
#define PHY_JABBER_DETECTION            ((uint16_t)0x0002)  /*!< Jabber condition detected            */
#define PHY_SR                          ((uint16_t)0x1F)    /*!< PHY special control/ status register Offset     */
#define PHY_SPEED_STATUS                ((uint16_t)0x0004)  /*!< PHY Speed mask                                  */
#define PHY_DUPLEX_STATUS               ((uint16_t)0x0010)  /*!< PHY Duplex mask                                 */
#define PHY_ISFR                        ((uint16_t)0x1D)    /*!< PHY Interrupt Source Flag register Offset       */
#define PHY_ISFR_INT4                   ((uint16_t)0x0010)  /*!< PHY Link down inturrupt                         */

#if !defined(HSE_VALUE)
#define HSE_VALUE \
  ((uint32_t) 8000000U) /*!< Value of the External oscillator in Hz */
#endif                  /* HSE_VALUE */
#if !defined(HSE_STARTUP_TIMEOUT)
#define HSE_STARTUP_TIMEOUT \
  ((uint32_t) 100U) /*!< Time out for HSE start up, in ms */
#endif              /* HSE_STARTUP_TIMEOUT */
#if !defined(HSI_VALUE)
#define HSI_VALUE \
  ((uint32_t) 16000000U) /*!< Value of the Internal oscillator in Hz*/
#endif                   /* HSI_VALUE */
#if !defined(LSI_VALUE)
#define LSI_VALUE ((uint32_t) 32000U) /*!< LSI Typical Value in Hz*/
#endif
#if !defined(LSE_VALUE)
#define LSE_VALUE \
  ((uint32_t) 32768U) /*!< Value of the External Low Speed oscillator in Hz */
#endif                /* LSE_VALUE */
#if !defined(LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT \
  ((uint32_t) 5000U) /*!< Time out for LSE start up, in ms */
#endif               /* LSE_STARTUP_TIMEOUT */
#if !defined(EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE \
  ((uint32_t) 12288000U) /*!< Value of the Internal oscillator in Hz*/
#endif                   /* EXTERNAL_CLOCK_VALUE */
#define VDD_VALUE ((uint32_t) 3300U)         /*!< Value of VDD in mv */
#define TICK_INT_PRIORITY ((uint32_t) 0x0FU) /*!< tick interrupt priority */
#define USE_RTOS 0U
#define PREFETCH_ENABLE 1U
#define ART_ACCLERATOR_ENABLE 1U /* To enable instruction cache and prefetch \
                                  */

#endif
