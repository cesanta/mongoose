/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _LPC_GPIO_H_
#define _LPC_GPIO_H_

#include "fsl_common.h"

/*!
 * @addtogroup lpc_gpio
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*! @{ */
/*! @brief LPC GPIO driver version. */
#define FSL_GPIO_DRIVER_VERSION (MAKE_VERSION(2, 1, 7))
/*! @} */

/*! @brief LPC GPIO direction definition */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput  = 0U, /*!< Set current pin as digital input*/
    kGPIO_DigitalOutput = 1U, /*!< Set current pin as digital output*/
} gpio_pin_direction_t;

/*!
 * @brief The GPIO pin configuration structure.
 *
 * Every pin can only be configured as either output pin or input pin at a time.
 * If configured as a input pin, then leave the outputConfig unused.
 */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t pinDirection; /*!< GPIO direction, input or output */
    /* Output configurations, please ignore if configured as a input one */
    uint8_t outputLogic; /*!< Set default output logic, no use in input */
} gpio_pin_config_t;

#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT) && FSL_FEATURE_GPIO_HAS_INTERRUPT)
#define GPIO_PIN_INT_LEVEL 0x00U
#define GPIO_PIN_INT_EDGE  0x01U

#define PINT_PIN_INT_HIGH_OR_RISE_TRIGGER 0x00U
#define PINT_PIN_INT_LOW_OR_FALL_TRIGGER  0x01U

/*! @brief GPIO Pin Interrupt enable mode */
typedef enum _gpio_pin_enable_mode
{
    kGPIO_PinIntEnableLevel = GPIO_PIN_INT_LEVEL, /*!< Generate Pin Interrupt on level mode */
    kGPIO_PinIntEnableEdge  = GPIO_PIN_INT_EDGE   /*!< Generate Pin Interrupt on edge mode */
} gpio_pin_enable_mode_t;

/*! @brief GPIO Pin Interrupt enable polarity */
typedef enum _gpio_pin_enable_polarity
{
    kGPIO_PinIntEnableHighOrRise =
        PINT_PIN_INT_HIGH_OR_RISE_TRIGGER, /*!< Generate Pin Interrupt on high level or rising edge */
    kGPIO_PinIntEnableLowOrFall =
        PINT_PIN_INT_LOW_OR_FALL_TRIGGER /*!< Generate Pin Interrupt on low level or falling edge */
} gpio_pin_enable_polarity_t;

/*! @brief LPC GPIO interrupt index definition */
typedef enum _gpio_interrupt_index
{
    kGPIO_InterruptA = 0U, /*!< Set current pin as interrupt A*/
    kGPIO_InterruptB = 1U, /*!< Set current pin as interrupt B*/
} gpio_interrupt_index_t;

/*! @brief Configures the interrupt generation condition. */
typedef struct _gpio_interrupt_config
{
    uint8_t mode;     /* The trigger mode of GPIO interrupts */
    uint8_t polarity; /* The polarity of GPIO interrupts */
} gpio_interrupt_config_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*! @name GPIO Configuration */
/*! @{ */

/*!
 * @brief Initializes the GPIO peripheral.
 *
 * This function ungates the GPIO clock.
 *
 * @param base   GPIO peripheral base pointer.
 * @param port   GPIO port number.
 */
void GPIO_PortInit(GPIO_Type *base, uint32_t port);

/*!
 * @brief Initializes a GPIO pin used by the board.
 *
 * To initialize the GPIO, define a pin configuration, either input or output, in the user file.
 * Then, call the GPIO_PinInit() function.
 *
 * This is an example to define an input pin or output pin configuration:
 * @code
 * Define a digital input pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalInput,
 *   0,
 * }
 * Define a digital output pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalOutput,
 *   0,
 * }
 * @endcode
 *
 * @param base   GPIO peripheral base pointer(Typically GPIO)
 * @param port   GPIO port number
 * @param pin    GPIO pin number
 * @param config GPIO pin configuration pointer
 */
void GPIO_PinInit(GPIO_Type *base, uint32_t port, uint32_t pin, const gpio_pin_config_t *config);

/*! @} */

/*! @name GPIO Output Operations */
/*! @{ */

/*!
 * @brief Sets the output level of the one GPIO pin to the logic 1 or 0.
 *
 * @param base    GPIO peripheral base pointer(Typically GPIO)
 * @param port   GPIO port number
 * @param pin    GPIO pin number
 * @param output  GPIO pin output logic level.
 *        - 0: corresponding pin output low-logic level.
 *        - 1: corresponding pin output high-logic level.
 */
static inline void GPIO_PinWrite(GPIO_Type *base, uint32_t port, uint32_t pin, uint8_t output)
{
    base->B[port][pin] = output;
}

/*! @} */
/*! @name GPIO Input Operations */
/*! @{ */

/*!
 * @brief Reads the current input value of the GPIO PIN.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port   GPIO port number
 * @param pin    GPIO pin number
 * @retval GPIO port input value
 *        - 0: corresponding pin input low-logic level.
 *        - 1: corresponding pin input high-logic level.
 */
static inline uint32_t GPIO_PinRead(GPIO_Type *base, uint32_t port, uint32_t pin)
{
    return (uint32_t)base->B[port][pin];
}

/*! @} */

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 1.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port GPIO port number
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortSet(GPIO_Type *base, uint32_t port, uint32_t mask)
{
    base->SET[port] = mask;
}

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 0.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port GPIO port number
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortClear(GPIO_Type *base, uint32_t port, uint32_t mask)
{
    base->CLR[port] = mask;
}

/*!
 * @brief Reverses current output logic of the multiple GPIO pins.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port GPIO port number
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortToggle(GPIO_Type *base, uint32_t port, uint32_t mask)
{
    base->NOT[port] = mask;
}

/*! @} */

/*!
 * @brief Reads the current input value of the whole GPIO port.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port GPIO port number
 */
static inline uint32_t GPIO_PortRead(GPIO_Type *base, uint32_t port)
{
    return (uint32_t)base->PIN[port];
}

/*! @} */
/*! @name GPIO Mask Operations */
/*! @{ */

/*!
 * @brief Sets port mask, 0 - enable pin, 1 - disable pin.
 *
 * @param base GPIO peripheral base pointer(Typically GPIO)
 * @param port GPIO port number
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortMaskedSet(GPIO_Type *base, uint32_t port, uint32_t mask)
{
    base->MASK[port] = mask;
}

/*!
 * @brief Sets the output level of the masked GPIO port. Only pins enabled by GPIO_SetPortMask() will be affected.
 *
 * @param base    GPIO peripheral base pointer(Typically GPIO)
 * @param port   GPIO port number
 * @param output  GPIO port output value.
 */
static inline void GPIO_PortMaskedWrite(GPIO_Type *base, uint32_t port, uint32_t output)
{
    base->MPIN[port] = output;
}

/*!
 * @brief Reads the current input value of the masked GPIO port. Only pins enabled by GPIO_SetPortMask() will be
 * affected.
 *
 * @param base   GPIO peripheral base pointer(Typically GPIO)
 * @param port   GPIO port number
 * @retval       masked GPIO port value
 */
static inline uint32_t GPIO_PortMaskedRead(GPIO_Type *base, uint32_t port)
{
    return (uint32_t)base->MPIN[port];
}

#if defined(FSL_FEATURE_GPIO_HAS_INTERRUPT) && FSL_FEATURE_GPIO_HAS_INTERRUPT
/*!
 * @brief Set the configuration of pin interrupt.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number
 * @param pin GPIO pin number.
 * @param config GPIO pin interrupt configuration..
 */
void GPIO_SetPinInterruptConfig(GPIO_Type *base, uint32_t port, uint32_t pin, gpio_interrupt_config_t *config);

/*!
 * @brief Enables multiple pins interrupt.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortEnableInterrupts(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask);

/*!
 * @brief Disables multiple pins interrupt.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortDisableInterrupts(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask);

/*!
 * @brief Clears pin interrupt flag. Status flags are cleared by
 *        writing a 1 to the corresponding bit position.
 *
 * @param base GPIO base pointer.
 * @param port   GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask);

/*!
 * @ Read port interrupt status.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number
 * @param index GPIO interrupt number.
 * @retval masked GPIO status value
 */
uint32_t GPIO_PortGetInterruptStatus(GPIO_Type *base, uint32_t port, uint32_t index);

/*!
 * @brief Enables the specific pin interrupt.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param pin GPIO pin number.
 * @param index GPIO interrupt number.
 */
void GPIO_PinEnableInterrupt(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index);

/*!
 * @brief Disables the specific pin interrupt.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param pin GPIO pin number.
 * @param index GPIO interrupt number.
 */
void GPIO_PinDisableInterrupt(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index);

/*!
 * @brief Clears the specific pin interrupt flag. Status flags are cleared by
 *        writing a 1 to the corresponding bit position.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param pin GPIO pin number.
 * @param index GPIO interrupt number.
 */
void GPIO_PinClearInterruptFlag(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index);

#endif /* FSL_FEATURE_GPIO_HAS_INTERRUPT */

/*! @} */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* _LPC_GPIO_H_*/
