/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef FSL_GPIO_H_
#define FSL_GPIO_H_

#include "fsl_common.h"

/*!
 * @addtogroup gpio
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief GPIO driver version. */
#define FSL_GPIO_DRIVER_VERSION (MAKE_VERSION(2, 7, 3))
/*@}*/

#if defined(FSL_FEATURE_GPIO_REGISTERS_WIDTH) && (FSL_FEATURE_GPIO_REGISTERS_WIDTH == 8U)
#define GPIO_FIT_REG(value) \
    ((uint8_t)(value)) /*!< For some platforms with 8-bit register width, cast the type to uint8_t */
#else
#define GPIO_FIT_REG(value) ((uint32_t)(value))
#endif /*FSL_FEATURE_GPIO_REGISTERS_WIDTH*/

/*! @brief GPIO direction definition */
typedef enum _gpio_pin_direction
{
    kGPIO_DigitalInput  = 0U, /*!< Set current pin as digital input*/
    kGPIO_DigitalOutput = 1U, /*!< Set current pin as digital output*/
} gpio_pin_direction_t;

#if defined(FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER
/*! @brief GPIO checker attribute */
typedef enum _gpio_checker_attribute
{
    kGPIO_UsernonsecureRWUsersecureRWPrivilegedsecureRW =
        0x00U, /*!< User nonsecure:Read+Write; User Secure:Read+Write; Privileged Secure:Read+Write */
    kGPIO_UsernonsecureRUsersecureRWPrivilegedsecureRW =
        0x01U, /*!< User nonsecure:Read;       User Secure:Read+Write; Privileged Secure:Read+Write */
    kGPIO_UsernonsecureNUsersecureRWPrivilegedsecureRW =
        0x02U, /*!< User nonsecure:None;       User Secure:Read+Write; Privileged Secure:Read+Write */
    kGPIO_UsernonsecureRUsersecureRPrivilegedsecureRW =
        0x03U, /*!< User nonsecure:Read;       User Secure:Read;       Privileged Secure:Read+Write */
    kGPIO_UsernonsecureNUsersecureRPrivilegedsecureRW =
        0x04U, /*!< User nonsecure:None;       User Secure:Read;       Privileged Secure:Read+Write */
    kGPIO_UsernonsecureNUsersecureNPrivilegedsecureRW =
        0x05U, /*!< User nonsecure:None;       User Secure:None;       Privileged Secure:Read+Write */
    kGPIO_UsernonsecureNUsersecureNPrivilegedsecureR =
        0x06U, /*!< User nonsecure:None;       User Secure:None;       Privileged Secure:Read */
    kGPIO_UsernonsecureNUsersecureNPrivilegedsecureN =
        0x07U, /*!< User nonsecure:None;       User Secure:None;       Privileged Secure:None */
    kGPIO_IgnoreAttributeCheck = 0x80U, /*!< Ignores the attribute check */
} gpio_checker_attribute_t;
#endif

/*!
 * @brief The GPIO pin configuration structure.
 *
 * Each pin can only be configured as either an output pin or an input pin at a time.
 * If configured as an input pin, leave the outputConfig unused.
 * Note that in some use cases, the corresponding port property should be configured in advance
 *        with the PORT_SetPinConfig().
 */
typedef struct _gpio_pin_config
{
    gpio_pin_direction_t pinDirection; /*!< GPIO direction, input or output */
    /* Output configurations; ignore if configured as an input pin */
    uint8_t outputLogic; /*!< Set a default output logic, which has no use in input */
} gpio_pin_config_t;

#if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) || \
     !(defined(FSL_FEATURE_SOC_PORT_COUNT))
/*! @brief Configures the interrupt generation condition. */
typedef enum _gpio_interrupt_config
{
    kGPIO_InterruptStatusFlagDisabled   = 0x0U,  /*!< Interrupt status flag is disabled. */
    kGPIO_DMARisingEdge                 = 0x1U,  /*!< ISF flag and DMA request on rising edge. */
    kGPIO_DMAFallingEdge                = 0x2U,  /*!< ISF flag and DMA request on falling edge. */
    kGPIO_DMAEitherEdge                 = 0x3U,  /*!< ISF flag and DMA request on either edge. */
    kGPIO_FlagRisingEdge                = 0x05U, /*!< Flag sets on rising edge. */
    kGPIO_FlagFallingEdge               = 0x06U, /*!< Flag sets on falling edge. */
    kGPIO_FlagEitherEdge                = 0x07U, /*!< Flag sets on either edge. */
    kGPIO_InterruptLogicZero            = 0x8U,  /*!< Interrupt when logic zero. */
    kGPIO_InterruptRisingEdge           = 0x9U,  /*!< Interrupt on rising edge. */
    kGPIO_InterruptFallingEdge          = 0xAU,  /*!< Interrupt on falling edge. */
    kGPIO_InterruptEitherEdge           = 0xBU,  /*!< Interrupt on either edge. */
    kGPIO_InterruptLogicOne             = 0xCU,  /*!< Interrupt when logic one. */
    kGPIO_ActiveHighTriggerOutputEnable = 0xDU,  /*!< Enable active high-trigger output. */
    kGPIO_ActiveLowTriggerOutputEnable  = 0xEU,  /*!< Enable active low-trigger output. */
} gpio_interrupt_config_t;
#endif

#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*! @brief Configures the selection of interrupt/DMA request/trigger output. */
typedef enum _gpio_interrupt_selection
{
    kGPIO_InterruptOutput0 = 0x0U, /*!< Interrupt/DMA request/trigger output 0. */
    kGPIO_InterruptOutput1 = 0x1U, /*!< Interrupt/DMA request/trigger output 1. */
} gpio_interrupt_selection_t;
#endif /* FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT */

#if defined(FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER) && FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER
/*! @brief GPIO version information. */
typedef struct _gpio_version_info
{
    uint16_t feature; /*!< Feature Specification Number. */
    uint8_t minor;    /*!< Minor Version Number. */
    uint8_t major;    /*!< Major Version Number. */
} gpio_version_info_t;
#endif /* FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER */

#if defined(FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL) && FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL
/*! @brief GPIO pin and interrupt control. */
typedef enum
{
    kGPIO_PinControlNonSecure          = 0x01U, /*!< Pin Control Non-Secure. */
    kGPIO_InterruptControlNonSecure    = 0x02U, /*!< Interrupt Control Non-Secure. */
    kGPIO_PinControlNonPrivilege       = 0x04U, /*!< Pin Control Non-Privilege. */
    kGPIO_InterruptControlNonPrivilege = 0x08U, /*!< Interrupt Control Non-Privilege. */
} gpio_pin_interrupt_control_t;
#endif /* FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL */

/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @addtogroup gpio_driver
 * @{
 */

/*! @name GPIO Configuration */
/*@{*/

/*!
 * @brief Initializes a GPIO pin used by the board.
 *
 * To initialize the GPIO, define a pin configuration, as either input or output, in the user file.
 * Then, call the GPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration.
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
 * @param base   GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param pin    GPIO port pin number
 * @param config GPIO pin configuration pointer
 */
void GPIO_PinInit(GPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config);

#if defined(FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER) && FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER
/*!
 * @brief Get GPIO version information.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param info GPIO version information
 */
void GPIO_GetVersionInfo(GPIO_Type *base, gpio_version_info_t *info);
#endif /* FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER */

#if defined(FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL) && FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL
/*!
 * @brief lock or unlock secure privilege.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask pin or interrupt macro
 */
static inline void GPIO_SecurePrivilegeLock(GPIO_Type *base, gpio_pin_interrupt_control_t mask)
{
    base->LOCK |= GPIO_FIT_REG(mask);
}

/*!
 * @brief Enable Pin Control Non-Secure.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_EnablePinControlNonSecure(GPIO_Type *base, uint32_t mask)
{
    base->PCNS |= GPIO_FIT_REG(mask);
}

/*!
 * @brief Disable Pin Control Non-Secure.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_DisablePinControlNonSecure(GPIO_Type *base, uint32_t mask)
{
    base->PCNS &= GPIO_FIT_REG(~mask);
}

/*!
 * @brief Enable Pin Control Non-Privilege.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_EnablePinControlNonPrivilege(GPIO_Type *base, uint32_t mask)
{
    base->PCNP |= GPIO_FIT_REG(mask);
}

/*!
 * @brief Disable Pin Control Non-Privilege.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_DisablePinControlNonPrivilege(GPIO_Type *base, uint32_t mask)
{
    base->PCNP &= GPIO_FIT_REG(~mask);
}

/*!
 * @brief Enable Interrupt Control Non-Secure.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_EnableInterruptControlNonSecure(GPIO_Type *base, uint32_t mask)
{
    base->ICNS |= GPIO_FIT_REG(mask);
}

/*!
 * @brief Disable Interrupt Control Non-Secure.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_DisableInterruptControlNonSecure(GPIO_Type *base, uint32_t mask)
{
    base->ICNS &= GPIO_FIT_REG(~mask);
}

/*!
 * @brief Enable Interrupt Control Non-Privilege.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_EnableInterruptControlNonPrivilege(GPIO_Type *base, uint32_t mask)
{
    base->ICNP |= GPIO_FIT_REG(mask);
}

/*!
 * @brief Disable Interrupt Control Non-Privilege.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_DisableInterruptControlNonPrivilege(GPIO_Type *base, uint32_t mask)
{
    base->ICNP &= GPIO_FIT_REG(~mask);
}
#endif /* FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL */

#if defined(FSL_FEATURE_GPIO_HAS_PORT_INPUT_CONTROL) && FSL_FEATURE_GPIO_HAS_PORT_INPUT_CONTROL
/*!
 * @brief Enable port input.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortInputEnable(GPIO_Type *base, uint32_t mask)
{
    base->PIDR &= GPIO_FIT_REG(~mask);
}

/*!
 * @brief Disable port input.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortInputDisable(GPIO_Type *base, uint32_t mask)
{
    base->PIDR |= GPIO_FIT_REG(mask);
}
#endif /* FSL_FEATURE_GPIO_HAS_PORT_INPUT_CONTROL */

/*@}*/

/*! @name GPIO Output Operations */
/*@{*/

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 1 or 0.
 *
 * @param base    GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param pin     GPIO pin number
 * @param output  GPIO pin output logic level.
 *        - 0: corresponding pin output low-logic level.
 *        - 1: corresponding pin output high-logic level.
 */
static inline void GPIO_PinWrite(GPIO_Type *base, uint32_t pin, uint8_t output)
{
#if !(defined(FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL) && FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL)
    if (output == 0U)
    {
        base->PCOR = GPIO_FIT_REG(1UL << pin);
    }
    else
    {
        base->PSOR = GPIO_FIT_REG(1UL << pin);
    }
#else
    if (output == 0U)
    {
        base->PDOR |= GPIO_FIT_REG(1UL << pin);
    }
    else
    {
        base->PDOR &= ~GPIO_FIT_REG(1UL << pin);
    }
#endif
}

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 1.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortSet(GPIO_Type *base, uint32_t mask)
{
#if !(defined(FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL) && FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL)
    base->PSOR = GPIO_FIT_REG(mask);
#else
    base->PDOR |= GPIO_FIT_REG(mask);
#endif
}

/*!
 * @brief Sets the output level of the multiple GPIO pins to the logic 0.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortClear(GPIO_Type *base, uint32_t mask)
{
#if !(defined(FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL) && FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL)
    base->PCOR = GPIO_FIT_REG(mask);
#else
    base->PDOR &= ~GPIO_FIT_REG(mask);
#endif
}

/*!
 * @brief Reverses the current output logic of the multiple GPIO pins.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
static inline void GPIO_PortToggle(GPIO_Type *base, uint32_t mask)
{
#if !(defined(FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL) && FSL_FEATURE_GPIO_HAS_NO_INDEP_OUTPUT_CONTROL)
    base->PTOR = GPIO_FIT_REG(mask);
#else
    base->PDOR ^= GPIO_FIT_REG(mask);
#endif
}

/*@}*/

/*! @name GPIO Input Operations */
/*@{*/

/*!
 * @brief Reads the current input value of the GPIO port.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param pin     GPIO pin number
 * @retval GPIO port input value
 *        - 0: corresponding pin input low-logic level.
 *        - 1: corresponding pin input high-logic level.
 */
static inline uint32_t GPIO_PinRead(GPIO_Type *base, uint32_t pin)
{
    return (((uint32_t)(base->PDIR) >> pin) & 0x01UL);
}

/*@}*/

/*! @name GPIO Interrupt */
/*@{*/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)
/*!
 * @brief Reads the GPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @retval The current GPIO port interrupt status flag, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t GPIO_PortGetInterruptFlags(GPIO_Type *base);

/*!
 * @brief Clears multiple GPIO pin interrupt status flags.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t mask);
#else
/*!
 * @brief Configures the gpio pin interrupt/DMA request.
 *
 * @param base    GPIO peripheral base pointer.
 * @param pin     GPIO pin number.
 * @param config  GPIO pin interrupt configuration.
 *        - #kGPIO_InterruptStatusFlagDisabled: Interrupt/DMA request disabled.
 *        - #kGPIO_DMARisingEdge : DMA request on rising edge(if the DMA requests exit).
 *        - #kGPIO_DMAFallingEdge: DMA request on falling edge(if the DMA requests exit).
 *        - #kGPIO_DMAEitherEdge : DMA request on either edge(if the DMA requests exit).
 *        - #kGPIO_FlagRisingEdge : Flag sets on rising edge(if the Flag states exit).
 *        - #kGPIO_FlagFallingEdge : Flag sets on falling edge(if the Flag states exit).
 *        - #kGPIO_FlagEitherEdge : Flag sets on either edge(if the Flag states exit).
 *        - #kGPIO_InterruptLogicZero  : Interrupt when logic zero.
 *        - #kGPIO_InterruptRisingEdge : Interrupt on rising edge.
 *        - #kGPIO_InterruptFallingEdge: Interrupt on falling edge.
 *        - #kGPIO_InterruptEitherEdge : Interrupt on either edge.
 *        - #kGPIO_InterruptLogicOne   : Interrupt when logic one.
 *        - #kGPIO_ActiveHighTriggerOutputEnable : Enable active high-trigger output (if the trigger states exit).
 *        - #kGPIO_ActiveLowTriggerOutputEnable  : Enable active low-trigger output (if the trigger states exit).
 */
static inline void GPIO_SetPinInterruptConfig(GPIO_Type *base, uint32_t pin, gpio_interrupt_config_t config)
{
    assert(base);

    base->ICR[pin] = GPIO_FIT_REG((base->ICR[pin] & ~GPIO_ICR_IRQC_MASK) | GPIO_ICR_IRQC(config));
}

#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*!
 * @brief Configures the gpio pin interrupt/DMA request/trigger output channel selection.
 *
 * @param base    GPIO peripheral base pointer.
 * @param pin     GPIO pin number.
 * @param selection  GPIO pin interrupt output selection.
 *        - #kGPIO_InterruptOutput0: Interrupt/DMA request/trigger output 0.
 *        - #kGPIO_InterruptOutput1 : Interrupt/DMA request/trigger output 1.
 */
static inline void GPIO_SetPinInterruptChannel(GPIO_Type *base, uint32_t pin, gpio_interrupt_selection_t selection)
{
    assert(base);

    base->ICR[pin] = GPIO_FIT_REG((base->ICR[pin] & ~GPIO_ICR_IRQS_MASK) | GPIO_ICR_IRQS(selection));
}
#endif
/*!
 * @brief Read the GPIO interrupt status flags.
 *
 * @param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on.)
 * @return The current GPIO's interrupt status flag.
 *         '1' means the related pin's flag is set, '0' means the related pin's flag not set.
 *          For example, the return value 0x00010001 means the pin 0 and 17 have the interrupt pending.
 */
uint32_t GPIO_GpioGetInterruptFlags(GPIO_Type *base);
#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*!
 * @brief Read the GPIO interrupt status flags based on selected interrupt channel(IRQS).
 *
 * @param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on.)
 * @param channel '0' means selete interrupt channel 0, '1' means selete interrupt channel 1.
 * @return The current GPIO's interrupt status flag based on the selected interrupt channel.
 *         '1' means the related pin's flag is set, '0' means the related pin's flag not set.
 *          For example, the return value 0x00010001 means the pin 0 and 17 have the interrupt pending.
 */
uint32_t GPIO_GpioGetInterruptChannelFlags(GPIO_Type *base, uint32_t channel);
#endif
/*!
 * @brief Read individual pin's interrupt status flag.
 *
 * @param base GPIO peripheral base pointer. (GPIOA, GPIOB, GPIOC, and so on)
 * @param pin GPIO specific pin number.
 * @return The current selected pin's interrupt status flag.
 */
uint8_t GPIO_PinGetInterruptFlag(GPIO_Type *base, uint32_t pin);

/*!
 * @brief Clears GPIO pin interrupt status flags.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 */
void GPIO_GpioClearInterruptFlags(GPIO_Type *base, uint32_t mask);
#if (defined(FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT) && FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT)
/*!
 * @brief Clears GPIO pin interrupt status flags based on selected interrupt channel(IRQS).
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param mask GPIO pin number macro
 * @param channel '0' means selete interrupt channel 0, '1' means selete interrupt channel 1.
 */
void GPIO_GpioClearInterruptChannelFlags(GPIO_Type *base, uint32_t mask, uint32_t channel);
#endif
/*!
 * @brief Clear GPIO individual pin's interrupt status flag.
 *
 * @param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on).
 * @param pin GPIO specific pin number.
 */
void GPIO_PinClearInterruptFlag(GPIO_Type *base, uint32_t pin);

/*!
 * @brief Reads the GPIO DMA request flags.
 *        The corresponding flag will be cleared automatically at the completion of the requested
 *        DMA transfer
 */
static inline uint32_t GPIO_GetPinsDMARequestFlags(GPIO_Type *base)
{
    assert(base);
    return (base->ISFR[1]);
}

/*!
 * @brief Sets the GPIO interrupt configuration in PCR register for multiple pins.
 *
 * @param base   GPIO peripheral base pointer.
 * @param mask   GPIO pin number macro.
 * @param config  GPIO pin interrupt configuration.
 *        - #kGPIO_InterruptStatusFlagDisabled: Interrupt disabled.
 *        - #kGPIO_DMARisingEdge : DMA request on rising edge(if the DMA requests exit).
 *        - #kGPIO_DMAFallingEdge: DMA request on falling edge(if the DMA requests exit).
 *        - #kGPIO_DMAEitherEdge : DMA request on either edge(if the DMA requests exit).
 *        - #kGPIO_FlagRisingEdge : Flag sets on rising edge(if the Flag states exit).
 *        - #kGPIO_FlagFallingEdge : Flag sets on falling edge(if the Flag states exit).
 *        - #kGPIO_FlagEitherEdge : Flag sets on either edge(if the Flag states exit).
 *        - #kGPIO_InterruptLogicZero  : Interrupt when logic zero.
 *        - #kGPIO_InterruptRisingEdge : Interrupt on rising edge.
 *        - #kGPIO_InterruptFallingEdge: Interrupt on falling edge.
 *        - #kGPIO_InterruptEitherEdge : Interrupt on either edge.
 *        - #kGPIO_InterruptLogicOne   : Interrupt when logic one.
 *        - #kGPIO_ActiveHighTriggerOutputEnable : Enable active high-trigger output (if the trigger states exit).
 *        - #kGPIO_ActiveLowTriggerOutputEnable  : Enable active low-trigger output (if the trigger states exit)..
 */
static inline void GPIO_SetMultipleInterruptPinsConfig(GPIO_Type *base, uint32_t mask, gpio_interrupt_config_t config)
{
    assert(base);

    if (0UL != (mask & 0xffffUL))
    {
        base->GICLR = GPIO_FIT_REG((GPIO_ICR_IRQC(config)) | (mask & 0xffffU));
    }
    mask = mask >> 16U;
    if (mask != 0UL)
    {
        base->GICHR = GPIO_FIT_REG((GPIO_ICR_IRQC(config)) | (mask & 0xffffU));
    }
}
#endif

#if defined(FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * brief The GPIO module supports a device-specific number of data ports, organized as 32-bit
 * words/8-bit Bytes. Each 32-bit/8-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. If the GPIO module's GACR register
 * organized as 32-bit words, the attribute controls for the 4 data bytes in the GACR follow a standard little
 * endian data convention.
 *
 * @param base      GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * @param attribute GPIO checker attribute
 */
void GPIO_CheckAttributeBytes(GPIO_Type *base, gpio_checker_attribute_t attribute);
#endif

/*@}*/
/*! @} */

/*!
 * @addtogroup fgpio_driver
 * @{
 */

/*
 * Introduces the FGPIO feature.
 *
 * The FGPIO features are only support on some Kinetis MCUs. The FGPIO registers are aliased to the IOPORT
 * interface. Accesses via the IOPORT interface occur in parallel with any instruction fetches and
 * complete in a single cycle. This aliased Fast GPIO memory map is called FGPIO.
 */

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

/*! @name FGPIO Configuration */
/*@{*/

#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL
/*!
 * @brief Initializes the FGPIO peripheral.
 *
 * This function ungates the FGPIO clock.
 *
 * @param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 */
void FGPIO_PortInit(FGPIO_Type *base);
#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

/*!
 * @brief Initializes a FGPIO pin used by the board.
 *
 * To initialize the FGPIO driver, define a pin configuration, as either input or output, in the user file.
 * Then, call the FGPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration:
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
 * @param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param pin    FGPIO port pin number
 * @param config FGPIO pin configuration pointer
 */
void FGPIO_PinInit(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config);

/*@}*/

/*! @name FGPIO Output Operations */
/*@{*/

/*!
 * @brief Sets the output level of the multiple FGPIO pins to the logic 1 or 0.
 *
 * @param base    FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param pin     FGPIO pin number
 * @param output  FGPIOpin output logic level.
 *        - 0: corresponding pin output low-logic level.
 *        - 1: corresponding pin output high-logic level.
 */
static inline void FGPIO_PinWrite(FGPIO_Type *base, uint32_t pin, uint8_t output)
{
    if (output == 0U)
    {
        base->PCOR = 1UL << pin;
    }
    else
    {
        base->PSOR = 1UL << pin;
    }
}

/*!
 * @brief Sets the output level of the multiple FGPIO pins to the logic 1.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param mask FGPIO pin number macro
 */
static inline void FGPIO_PortSet(FGPIO_Type *base, uint32_t mask)
{
    base->PSOR = mask;
}

/*!
 * @brief Sets the output level of the multiple FGPIO pins to the logic 0.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param mask FGPIO pin number macro
 */
static inline void FGPIO_PortClear(FGPIO_Type *base, uint32_t mask)
{
    base->PCOR = mask;
}

/*!
 * @brief Reverses the current output logic of the multiple FGPIO pins.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param mask FGPIO pin number macro
 */
static inline void FGPIO_PortToggle(FGPIO_Type *base, uint32_t mask)
{
    base->PTOR = mask;
}
/*@}*/

/*! @name FGPIO Input Operations */
/*@{*/

/*!
 * @brief Reads the current input value of the FGPIO port.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param pin  FGPIO pin number
 * @retval FGPIO port input value
 *        - 0: corresponding pin input low-logic level.
 *        - 1: corresponding pin input high-logic level.
 */
static inline uint32_t FGPIO_PinRead(FGPIO_Type *base, uint32_t pin)
{
    return (((base->PDIR) >> pin) & 0x01U);
}
/*@}*/

/*! @name FGPIO Interrupt */
/*@{*/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && \
    defined(FSL_FEATURE_SOC_PORT_COUNT)

/*!
 * @brief Reads the FGPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level-sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @retval The current FGPIO port interrupt status flags, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t FGPIO_PortGetInterruptFlags(FGPIO_Type *base);

/*!
 * @brief Clears the multiple FGPIO pin interrupt status flag.
 *
 * @param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param mask FGPIO pin number macro
 */
void FGPIO_PortClearInterruptFlags(FGPIO_Type *base, uint32_t mask);
#endif
#if defined(FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * @brief The FGPIO module supports a device-specific number of data ports, organized as 32-bit
 * words. Each 32-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. The attribute controls for the 4 data
 * bytes in the GACR follow a standard little endian
 * data convention.
 *
 * @param base      FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * @param attribute FGPIO checker attribute
 */
void FGPIO_CheckAttributeBytes(FGPIO_Type *base, gpio_checker_attribute_t attribute);
#endif /* FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER */

/*@}*/

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */

#endif /* FSL_GPIO_H_*/
