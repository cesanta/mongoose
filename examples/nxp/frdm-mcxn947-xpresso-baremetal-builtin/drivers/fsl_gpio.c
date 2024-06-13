/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019, 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.gpio"
#endif

#if defined(GPIO_RSTS)
#define GPIO_RESETS_ARRAY GPIO_RSTS
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
static PORT_Type *const s_portBases[] = PORT_BASE_PTRS;
static GPIO_Type *const s_gpioBases[] = GPIO_BASE_PTRS;
#endif

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map FGPIO instance number to clock name. */
static const clock_ip_name_t s_fgpioClockName[] = FGPIO_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */

#if defined(GPIO_RESETS_ARRAY)
/* Reset array */
static const reset_ip_name_t s_gpioResets[] = GPIO_RESETS_ARRAY;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * @brief Gets the GPIO instance according to the GPIO base
 *
 * @param base    GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
 * @retval GPIO instance
 */
static uint32_t GPIO_GetInstance(GPIO_Type *base);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT) || defined(GPIO_RESETS_ARRAY)
static uint32_t GPIO_GetInstance(GPIO_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_gpioBases); instance++)
    {
        if (s_gpioBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_gpioBases));

    return instance;
}
#endif
/*!
 * brief Initializes a GPIO pin used by the board.
 *
 * To initialize the GPIO, define a pin configuration, as either input or output, in the user file.
 * Then, call the GPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration.
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
 * param base   GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param pin    GPIO port pin number
 * param config GPIO pin configuration pointer
 */
void GPIO_PinInit(GPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(NULL != config);

#if defined(GPIO_RESETS_ARRAY)
    RESET_ReleasePeripheralReset(s_gpioResets[GPIO_GetInstance(base)]);
#endif

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= GPIO_FIT_REG(~(1UL << pin));
    }
    else
    {
        GPIO_PinWrite(base, pin, config->outputLogic);
        base->PDDR |= GPIO_FIT_REG((1UL << pin));
    }
}

#if defined(FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER) && FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER
void GPIO_GetVersionInfo(GPIO_Type *base, gpio_version_info_t *info)
{
    info->feature = (uint16_t)base->VERID;
    info->minor   = (uint8_t)(base->VERID >> GPIO_VERID_MINOR_SHIFT);
    info->major   = (uint8_t)(base->VERID >> GPIO_VERID_MAJOR_SHIFT);
}
#endif /* FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER */

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * brief Reads the GPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * retval The current GPIO port interrupt status flag, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t GPIO_PortGetInterruptFlags(GPIO_Type *base)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance = (uint8_t)GPIO_GetInstance(base);
    portBase = s_portBases[instance];
    return portBase->ISFR;
}
#else
/*!
 * brief Read the GPIO interrupt status flags.
 *
 * param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on.)
 * return The current GPIO's interrupt status flag.
 *         '1' means the related pin's flag is set, '0' means the related pin's flag not set.
 *          For example, the return value 0x00010001 means the pin 0 and 17 have the interrupt pending.
 */
uint32_t GPIO_GpioGetInterruptFlags(GPIO_Type *base)
{
    return base->ISFR[0];
}
#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*!
 * brief Read the GPIO interrupt status flags based on selected interrupt channel(IRQS).
 * param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on.)
 * param channel '0' means selete interrupt channel 0, '1' means selete interrupt channel 1.
 *
 * return The current GPIO's interrupt status flag based on the selected interrupt channel.
 *         '1' means the related pin's flag is set, '0' means the related pin's flag not set.
 *          For example, the return value 0x00010001 means the pin 0 and 17 have the interrupt pending.
 */
uint32_t GPIO_GpioGetInterruptChannelFlags(GPIO_Type *base, uint32_t channel)
{
    assert(channel < 2U);
    return base->ISFR[channel];
}
#endif /* FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT */
/*!
 * brief Read individual pin's interrupt status flag.
 *
 * param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on)
 * param pin GPIO specific pin number.
 * return The current selected pin's interrupt status flag.
 */
uint8_t GPIO_PinGetInterruptFlag(GPIO_Type *base, uint32_t pin)
{
    return (uint8_t)((base->ICR[pin] & GPIO_ICR_ISF_MASK) >> GPIO_ICR_ISF_SHIFT);
}
#endif /* FSL_FEATURE_PORT_HAS_NO_INTERRUPT */

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * brief Clears multiple GPIO pin interrupt status flags.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param mask GPIO pin number macro
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance       = (uint8_t)GPIO_GetInstance(base);
    portBase       = s_portBases[instance];
    portBase->ISFR = mask;
}
#else
/*!
 * brief Clears GPIO pin interrupt status flags.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param mask GPIO pin number macro
 */
void GPIO_GpioClearInterruptFlags(GPIO_Type *base, uint32_t mask)
{
    base->ISFR[0] = GPIO_FIT_REG(mask);
}
#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*!
 * brief Clears GPIO pin interrupt status flags based on selected interrupt channel(IRQS).
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param mask GPIO pin number macro
 * param channel '0' means selete interrupt channel 0, '1' means selete interrupt channel 1.
 */
void GPIO_GpioClearInterruptChannelFlags(GPIO_Type *base, uint32_t mask, uint32_t channel)
{
    assert(channel < 2U);
    base->ISFR[channel] = GPIO_FIT_REG(mask);
}
#endif /* FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT */
/*!
 * brief Clear GPIO individual pin's interrupt status flag.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on).
 * param pin GPIO specific pin number.
 */
void GPIO_PinClearInterruptFlag(GPIO_Type *base, uint32_t pin)
{
    base->ICR[pin] |= GPIO_FIT_REG(GPIO_ICR_ISF(1U));
}
#endif /* FSL_FEATURE_PORT_HAS_NO_INTERRUPT */

#if defined(FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * brief The GPIO module supports a device-specific number of data ports, organized as 32-bit
 * words/8-bit Bytes. Each 32-bit/8-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. If the GPIO module's GACR register
 * organized as 32-bit words, the attribute controls for the 4 data bytes in the GACR follow a standard little
 * endian data convention.
 *
 * param base      GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param attribute GPIO checker attribute
 */
void GPIO_CheckAttributeBytes(GPIO_Type *base, gpio_checker_attribute_t attribute)
{
#if defined(FSL_FEATURE_GPIO_REGISTERS_WIDTH) && (FSL_FEATURE_GPIO_REGISTERS_WIDTH == 8U)
    base->GACR = ((uint8_t)attribute << GPIO_GACR_ACB_SHIFT);
#else
    base->GACR = ((uint32_t)attribute << GPIO_GACR_ACB0_SHIFT) | ((uint32_t)attribute << GPIO_GACR_ACB1_SHIFT) |
                 ((uint32_t)attribute << GPIO_GACR_ACB2_SHIFT) | ((uint32_t)attribute << GPIO_GACR_ACB3_SHIFT);
#endif /* FSL_FEATURE_GPIO_REGISTERS_WIDTH */
}
#endif

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
static FGPIO_Type *const s_fgpioBases[] = FGPIO_BASE_PTRS;
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * @brief Gets the FGPIO instance according to the GPIO base
 *
 * @param base    FGPIO peripheral base pointer(PTA, PTB, PTC, etc.)
 * @retval FGPIO instance
 */
static uint32_t FGPIO_GetInstance(FGPIO_Type *base);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
static uint32_t FGPIO_GetInstance(FGPIO_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_fgpioBases); instance++)
    {
        if (s_fgpioBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_fgpioBases));

    return instance;
}
#endif
#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL
/*!
 * brief Initializes the FGPIO peripheral.
 *
 * This function ungates the FGPIO clock.
 *
 * param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 */
void FGPIO_PortInit(FGPIO_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Ungate FGPIO periphral clock */
    CLOCK_EnableClock(s_fgpioClockName[FGPIO_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

/*!
 * brief Initializes a FGPIO pin used by the board.
 *
 * To initialize the FGPIO driver, define a pin configuration, as either input or output, in the user file.
 * Then, call the FGPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration:
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
 * param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param pin    FGPIO port pin number
 * param config FGPIO pin configuration pointer
 */
void FGPIO_PinInit(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(NULL != config);

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(1UL << pin);
    }
    else
    {
        FGPIO_PinWrite(base, pin, config->outputLogic);
        base->PDDR |= (1UL << pin);
    }
}
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * brief Reads the FGPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level-sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * retval The current FGPIO port interrupt status flags, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t FGPIO_PortGetInterruptFlags(FGPIO_Type *base)
{
    uint8_t instance;
    instance = (uint8_t)FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase = s_portBases[instance];
    return portBase->ISFR;
}

/*!
 * brief Clears the multiple FGPIO pin interrupt status flag.
 *
 * param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param mask FGPIO pin number macro
 */
void FGPIO_PortClearInterruptFlags(FGPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    instance = (uint8_t)FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase       = s_portBases[instance];
    portBase->ISFR = mask;
}
#endif
#if defined(FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * brief The FGPIO module supports a device-specific number of data ports, organized as 32-bit
 * words. Each 32-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. The attribute controls for the 4 data
 * bytes in the GACR follow a standard little endian
 * data convention.
 *
 * param base      FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param attribute FGPIO checker attribute
 */
void FGPIO_CheckAttributeBytes(FGPIO_Type *base, gpio_checker_attribute_t attribute)
{
    base->GACR = ((uint32_t)attribute << FGPIO_GACR_ACB0_SHIFT) | ((uint32_t)attribute << FGPIO_GACR_ACB1_SHIFT) |
                 ((uint32_t)attribute << FGPIO_GACR_ACB2_SHIFT) | ((uint32_t)attribute << FGPIO_GACR_ACB3_SHIFT);
}
#endif

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */
