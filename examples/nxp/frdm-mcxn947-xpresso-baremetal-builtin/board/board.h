/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "clock_config.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-MCXN947"

/*! @brief The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE       kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR   (uint32_t) LPUART4
#define BOARD_DEBUG_UART_INSTANCE   4U
#define BOARD_DEBUG_UART_CLK_FREQ   12000000U
#define BOARD_DEBUG_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM4
#define BOARD_DEBUG_UART_RST        kFC4_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC     kCLOCK_FlexComm4
#define BOARD_UART_IRQ_HANDLER      LP_FLEXCOMM4_IRQHandler
#define BOARD_UART_IRQ              LP_FLEXCOMM4_IRQn

#define BOARD_DEBUG_UART_TYPE_CORE1       kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR_CORE1   (uint32_t) USART1
#define BOARD_DEBUG_UART_INSTANCE_CORE1   1U
#define BOARD_DEBUG_UART_CLK_FREQ_CORE1   12000000U
#define BOARD_DEBUG_UART_CLK_ATTACH_CORE1 kFRO12M_to_FLEXCOMM1
#define BOARD_DEBUG_UART_RST_CORE1        kFC1_RST_SHIFT_RSTn
#define BOARD_DEBUG_UART_CLKSRC_CORE1     kCLOCK_Flexcomm1
#define BOARD_UART_IRQ_HANDLER_CORE1      FLEXCOMM1_IRQHandler
#define BOARD_UART_IRQ_CORE1              FLEXCOMM1_IRQn

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200U
#endif /* BOARD_DEBUG_UART_BAUDRATE */

#ifndef BOARD_DEBUG_UART_BAUDRATE_CORE1
#define BOARD_DEBUG_UART_BAUDRATE_CORE1 115200U
#endif /* BOARD_DEBUG_UART_BAUDRATE_CORE1 */

/*! @brief The UART to use for Bluetooth M.2 interface. */
#define BOARD_BT_UART_INSTANCE   2
#define BOARD_BT_UART_BAUDRATE   3000000
#define BOARD_BT_UART_CLK_FREQ   12000000U
#define BOARD_BT_UART_CLK_ATTACH kFRO12M_to_FLEXCOMM2

/*! @brief The ENET PHY address. */
#define BOARD_ENET0_PHY_ADDRESS (0x00U) /* Phy address of enet port 0. */

/*! @brief Memory ranges not usable by the ENET DMA. */
#ifndef BOARD_ENET_NON_DMA_MEMORY_ARRAY
#define BOARD_ENET_NON_DMA_MEMORY_ARRAY                                                     \
    {                                                                                       \
        {0x00000000U, 0x0007FFFFU}, {0x10000000U, 0x17FFFFFFU}, {0x80000000U, 0xDFFFFFFFU}, \
            {0x00000000U, 0x00000000U},                                                     \
    }
#endif /* BOARD_ENET_NON_DMA_MEMORY_ARRAY */

#define BOARD_ACCEL_I2C_BASEADDR   LPI2C2
#define BOARD_ACCEL_I2C_CLOCK_FREQ 12000000

#define BOARD_CODEC_I2C_BASEADDR   LPI2C2
#define BOARD_CODEC_I2C_CLOCK_FREQ 12000000
#define BOARD_CODEC_I2C_INSTANCE   2

/*! @brief Indexes of the TSI channels for on-board electrodes */
#ifndef BOARD_TSI_ELECTRODE_1
#define BOARD_TSI_ELECTRODE_1 3U
#endif

/*! @brief Indexes of the TSI mutual channels for FRDM-TOUCH board */
#define BOARD_TSI_MUTUAL_TX_ELECTRODE_1 0U
#define BOARD_TSI_MUTUAL_RX_ELECTRODE_1 14U

#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIO0
#endif
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 10U
#endif

#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIO1
#endif
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 2U
#endif

#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIO0
#endif
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 27U
#endif

#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIO0
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 23U
#endif
#define BOARD_SW2_NAME        "SW2"
#define BOARD_SW2_IRQ         GPIO00_IRQn
#define BOARD_SW2_IRQ_HANDLER GPIO00_IRQHandler

#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIO0
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 6U
#endif
#define BOARD_SW3_NAME        "SW3"
#define BOARD_SW3_IRQ         GPIO00_IRQn
#define BOARD_SW3_IRQ_HANDLER GPIO00_IRQHandler

/* USB PHY condfiguration */
#define BOARD_USB_PHY_D_CAL     (0x04U)
#define BOARD_USB_PHY_TXCAL45DP (0x07U)
#define BOARD_USB_PHY_TXCAL45DM (0x07U)

#define BOARD_HAS_NO_CTIMER_OUTPUT_PIN_CONNECTED_TO_LED (1)

/* Board led color mapping */
#define LOGIC_LED_ON  0U
#define LOGIC_LED_OFF 1U

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN)                         /*!< Enable target LED_RED */
#define LED_RED_ON()     GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)  /*!< Turn on target LED_RED */
#define LED_RED_OFF()    GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)    /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_BLUE_INIT(output)                                            \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN)                       /*!< Enable target LED_BLUE */
#define LED_BLUE_ON()     GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN)  /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF()    GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN)    /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON()     GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN)  /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF()    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN)    /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

/* Display. */
#define BOARD_LCD_DC_GPIO      GPIO0
#define BOARD_LCD_DC_GPIO_PORT 0U
#define BOARD_LCD_DC_GPIO_PIN  10U

/* Camera */
#define BOARD_CAMERA_I2C_BASEADDR         LPI2C7
#define BOARD_CAMERA_I2C_INSTANCE         7
#define BOARD_CAMERA_I2C_CLOCK_FREQ       CLOCK_GetLPFlexCommClkFreq(BOARD_CAMERA_I2C_INSTANCE)

/* Serial MWM WIFI */
#define BOARD_SERIAL_MWM_PORT_CLK_FREQ CLOCK_GetFlexCommClkFreq(2)
#define BOARD_SERIAL_MWM_PORT          USART2
#define BOARD_SERIAL_MWM_PORT_IRQn     FLEXCOMM2_IRQn
#define BOARD_SERIAL_MWM_RST_WRITE(output)

/*! @brief The EMVSIM SMARTCARD PHY configuration. */
#define BOARD_SMARTCARD_MODULE                (EMVSIM0)      /*!< SMARTCARD communicational module instance */
#define BOARD_SMARTCARD_MODULE_IRQ            (EMVSIM0_IRQn) /*!< SMARTCARD communicational module IRQ handler */
#define BOARD_SMARTCARD_CLOCK_MODULE_CLK_FREQ (CLOCK_GetEmvsimClkFreq(0U))
#define BOARD_SMARTCARD_CLOCK_VALUE           (4000000U) /*!< SMARTCARD clock frequency */

/* ERPC LPSPI configuration */
#define ERPC_BOARD_LPSPI_SLAVE_READY_USE_GPIO (1)
#define ERPC_BOARD_LPSPI_BASEADDR             LPSPI3
#define ERPC_BOARD_LPSPI_BAUDRATE             500000U
#define ERPC_BOARD_LPSPI_CLKSRC               kCLOCK_Flexcomm3
#define ERPC_BOARD_LPSPI_CLK_FREQ             12000000 // CLOCK_GetFlexCommClkFreq(1)
#define ERPC_BOARD_LPSPI_INT_GPIO             GPIO0
#define ERPC_BOARD_LPSPI_INT_PIN              16U
#define ERPC_BOARD_LPSPI_INT_PIN_IRQ          PIN_INT0_IRQn
#define ERPC_BOARD_LPSPI_INT_PIN_IRQ_HANDLER  PIN_INT0_IRQHandler

/* ERPC LPI2C configuration */
#define ERPC_BOARD_LPI2C_BASEADDR            LPI2C0_BASE
#define ERPC_BOARD_LPI2C_BAUDRATE            100000U
#define ERPC_BOARD_LPI2C_CLKSRC              kCLOCK_Flexcomm0
#define ERPC_BOARD_LPI2C_CLK_FREQ            12000000 // CLOCK_GetFlexCommClkFreq(2)
#define ERPC_BOARD_LPI2C_INT_GPIO            GPIO1
#define ERPC_BOARD_LPI2C_INT_PIN             0U
#define ERPC_BOARD_LPI2C_INT_PIN_IRQ         PIN_INT1_IRQn
#define ERPC_BOARD_LPI2C_INT_PIN_IRQ_HANDLER PIN_INT1_IRQHandler

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_InitDebugConsole(void);
void BOARD_InitDebugConsole_Core1(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_LPI2C_Init(LPI2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_LPI2C_Send(LPI2C_Type *base,
                          uint8_t deviceAddress,
                          uint32_t subAddress,
                          uint8_t subaddressSize,
                          uint8_t *txBuff,
                          uint8_t txBuffSize);
status_t BOARD_LPI2C_Receive(LPI2C_Type *base,
                             uint8_t deviceAddress,
                             uint32_t subAddress,
                             uint8_t subaddressSize,
                             uint8_t *rxBuff,
                             uint8_t rxBuffSize);
status_t BOARD_LPI2C_SendSCCB(LPI2C_Type *base,
                              uint8_t deviceAddress,
                              uint32_t subAddress,
                              uint8_t subaddressSize,
                              uint8_t *txBuff,
                              uint8_t txBuffSize);
status_t BOARD_LPI2C_ReceiveSCCB(LPI2C_Type *base,
                                 uint8_t deviceAddress,
                                 uint32_t subAddress,
                                 uint8_t subaddressSize,
                                 uint8_t *rxBuff,
                                 uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Codec_I2C_Init(void);
status_t BOARD_Codec_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Codec_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
void BOARD_Camera_I2C_Init(void);
status_t BOARD_Camera_I2C_Send(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);

status_t BOARD_Camera_I2C_SendSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, const uint8_t *txBuff, uint8_t txBuffSize);
status_t BOARD_Camera_I2C_ReceiveSCCB(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subAddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

void BOARD_PowerMode_OD(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
