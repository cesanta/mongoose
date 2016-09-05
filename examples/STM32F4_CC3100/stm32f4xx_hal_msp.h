/*
 * Copyright (c) 2014-2016 Cesanta Software Limited
 * All rights reserved
 */

#ifndef CS_MONGOOSE_EXAMPLES_STM32F4_CC3100_STM32F4XX_HAL_MSP_H_
#define CS_MONGOOSE_EXAMPLES_STM32F4_CC3100_STM32F4XX_HAL_MSP_H_

/*
 * Definitions for SPI used to communicate with CC3100
 * All samples in STMCubeF4 use SPI4 for examples
 * We do the same. See README.MD for details.
 */
#define SPIx_NUMBER 4
#define SPIx SPI4
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI4_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()
#define SPIx_NSS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

#define SPIx_FORCE_RESET() __HAL_RCC_SPI4_FORCE_RESET()
#define SPIx_RELEASE_RESET() __HAL_RCC_SPI4_RELEASE_RESET()

#define SPIx_SCK_PIN GPIO_PIN_2
#define SPIx_SCK_GPIO_PORT GPIOE
#define SPIx_SCK_AF GPIO_AF5_SPI4
#define SPIx_MISO_PIN GPIO_PIN_5
#define SPIx_MISO_GPIO_PORT GPIOE
#define SPIx_MISO_AF GPIO_AF5_SPI4
#define SPIx_MOSI_PIN GPIO_PIN_6
#define SPIx_MOSI_GPIO_PORT GPIOE
#define SPIx_MOSI_AF GPIO_AF5_SPI4

#define SPIx_IRQn SPI4_IRQn
#define SPIx_IRQHandler SPI4_IRQHandler

#define SPI_CS_PIN GPIO_PIN_12
#define SPI_CS_PORT GPIOB

/*
 * CC3100 requires 2 additional pins for communication
 * See http://processors.wiki.ti.com/index.php/CC31xx_SPI_Host_Interface
 * for details
 */

#define MCU_IRQ_PIN GPIO_PIN_0
#define MCU_IRQ_PORT GPIOA
#define MCU_nHIB_PORT GPIOB
#define MCU_nHIB_PIN GPIO_PIN_0

/*
 * Definitions used for debug uart.
 * By default we use USART1, and this allows to connect serial port monitor
 * to dedicated pin (PA9) and to ST-LINK port (/dev/ttyACM0 in Linux)
 * See README.MD for details.
 */
#define USARTx USART1
#define USART_SPEED 115200
#define USARTx_CLK_ENABLE() __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET() __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET() __HAL_RCC_USART1_RELEASE_RESET()

#define USARTx_TX_PIN GPIO_PIN_9
#define USARTx_TX_GPIO_PORT GPIOA
#define USARTx_TX_AF GPIO_AF7_USART1
#define USARTx_RX_PIN GPIO_PIN_10
#define USARTx_RX_GPIO_PORT GPIOA
#define USARTx_RX_AF GPIO_AF7_USART1

#endif /* CS_MONGOOSE_EXAMPLES_STM32F4_CC3100_STM32F4XX_HAL_MSP_H_ */
