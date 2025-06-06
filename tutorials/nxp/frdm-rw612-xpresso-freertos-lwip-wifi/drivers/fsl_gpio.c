/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.lpc_gpio"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map FGPIO instance number to clock name. */
static const clock_ip_name_t s_gpioClockName[] = GPIO_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_GPIO_HAS_NO_RESET) && FSL_FEATURE_GPIO_HAS_NO_RESET)
/*! @brief Pointers to GPIO resets for each instance. */
static const reset_ip_name_t s_gpioResets[] = GPIO_RSTS_N;
#endif
/*******************************************************************************
 * Prototypes
 ************ ******************************************************************/
/*!
 * @brief Enable GPIO port clock.
 *
 * @param base   GPIO peripheral base pointer.
 * @param port   GPIO port number.
 */
static void GPIO_EnablePortClock(GPIO_Type *base, uint32_t port);

/*******************************************************************************
 * Code
 ******************************************************************************/
static void GPIO_EnablePortClock(GPIO_Type *base, uint32_t port)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    assert(port < ARRAY_SIZE(s_gpioClockName));

    /* Upgate the GPIO clock */
    CLOCK_EnableClock(s_gpioClockName[port]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}

/*!
 * brief Initializes the GPIO peripheral.
 *
 * This function ungates the GPIO clock.
 *
 * param base   GPIO peripheral base pointer.
 * param port   GPIO port number.
 */
void GPIO_PortInit(GPIO_Type *base, uint32_t port)
{
    GPIO_EnablePortClock(base, port);

#if !(defined(FSL_FEATURE_GPIO_HAS_NO_RESET) && FSL_FEATURE_GPIO_HAS_NO_RESET)
    /* Reset the GPIO module */
    RESET_PeripheralReset(s_gpioResets[port]);
#endif
}

/*!
 * brief Initializes a GPIO pin used by the board.
 *
 * To initialize the GPIO, define a pin configuration, either input or output, in the user file.
 * Then, call the GPIO_PinInit() function.
 *
 * This is an example to define an input pin or output pin configuration:
 * code
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
 * endcode
 *
 * param base   GPIO peripheral base pointer(Typically GPIO)
 * param port   GPIO port number
 * param pin    GPIO pin number
 * param config GPIO pin configuration pointer
 */
void GPIO_PinInit(GPIO_Type *base, uint32_t port, uint32_t pin, const gpio_pin_config_t *config)
{
    GPIO_EnablePortClock(base, port);

    if (config->pinDirection == kGPIO_DigitalInput)
    {
#if defined(FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR) && (FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR)
        base->DIRCLR[port] = 1UL << pin;
#else
        base->DIR[port] &= ~(1UL << pin);
#endif /*FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR*/
    }
    else
    {
        /* Set default output value */
        if (config->outputLogic == 0U)
        {
            base->CLR[port] = (1UL << pin);
        }
        else
        {
            base->SET[port] = (1UL << pin);
        }
/* Set pin direction */
#if defined(FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR) && (FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR)
        base->DIRSET[port] = 1UL << pin;
#else
        base->DIR[port] |= 1UL << pin;
#endif /*FSL_FEATURE_GPIO_DIRSET_AND_DIRCLR*/
    }
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
void GPIO_SetPinInterruptConfig(GPIO_Type *base, uint32_t port, uint32_t pin, gpio_interrupt_config_t *config)
{
    base->INTEDG[port] = (base->INTEDG[port] & ~(1UL << pin)) | ((uint32_t)config->mode << pin);

    base->INTPOL[port] = (base->INTPOL[port] & ~(1UL << pin)) | ((uint32_t)config->polarity << pin);
}

/*!
 * @brief Enables multiple pins interrupt.
 *
 * @param base GPIO base pointer.
 * @param port   GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortEnableInterrupts(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTENA[port] = base->INTENA[port] | mask;
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTENB[port] = base->INTENB[port] | mask;
    }
    else
    {
        /*Should not enter here*/
    }
}

/*!
 * @brief Disables multiple pins interrupt.
 *
 * @param base GPIO base pointer.
 * @param port   GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortDisableInterrupts(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTENA[port] = base->INTENA[port] & ~mask;
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTENB[port] = base->INTENB[port] & ~mask;
    }
    else
    {
        /*Should not enter here*/
    }
}

/*!
 * @brief Clears multiple pins interrupt flag. Status flags are cleared by
 *        writing a 1 to the corresponding bit position.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t port, uint32_t index, uint32_t mask)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTSTATA[port] = mask;
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTSTATB[port] = mask;
    }
    else
    {
        /*Should not enter here*/
    }
}

/*!
 * @ Read port interrupt status.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number
 * @param index GPIO interrupt number.
 * @retval masked GPIO status value
 */
uint32_t GPIO_PortGetInterruptStatus(GPIO_Type *base, uint32_t port, uint32_t index)
{
    uint32_t status = 0U;

    if ((uint32_t)kGPIO_InterruptA == index)
    {
        status = base->INTSTATA[port];
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        status = base->INTSTATB[port];
    }
    else
    {
        /*Should not enter here*/
    }
    return status;
}

/*!
 * @brief Enables the specific pin interrupt.
 *
 * @param base GPIO base pointer.
 * @param port   GPIO port number.
 * @param pin GPIO pin number.
 * @param index GPIO interrupt number.
 */
void GPIO_PinEnableInterrupt(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTENA[port] = base->INTENA[port] | (1UL << pin);
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTENB[port] = base->INTENB[port] | (1UL << pin);
    }
    else
    {
        /*Should not enter here*/
    }
}

/*!
 * @brief Disables the specific pin interrupt.
 *
 * @param base GPIO base pointer.
 * @param port   GPIO port number.
 * @param pin GPIO pin number.
 * @param index GPIO interrupt number.
 */
void GPIO_PinDisableInterrupt(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTENA[port] = base->INTENA[port] & ~(1UL << pin);
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTENB[port] = base->INTENB[port] & ~(1UL << pin);
    }
    else
    {
        /*Should not enter here*/
    }
}

/*!
 * @brief Clears the specific pin interrupt flag. Status flags are cleared by
 *        writing a 1 to the corresponding bit position.
 *
 * @param base GPIO base pointer.
 * @param port GPIO port number.
 * @param index GPIO interrupt number.
 * @param mask GPIO pin number macro.
 */
void GPIO_PinClearInterruptFlag(GPIO_Type *base, uint32_t port, uint32_t pin, uint32_t index)
{
    if ((uint32_t)kGPIO_InterruptA == index)
    {
        base->INTSTATA[port] = 1UL << pin;
    }
    else if ((uint32_t)kGPIO_InterruptB == index)
    {
        base->INTSTATB[port] = 1UL << pin;
    }
    else
    {
        /*Should not enter here*/
    }
}
#endif /* FSL_FEATURE_GPIO_HAS_INTERRUPT */
