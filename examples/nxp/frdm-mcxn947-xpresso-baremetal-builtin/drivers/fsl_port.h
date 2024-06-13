/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FSL_PORT_H_
#define FSL_PORT_H_

#include "fsl_common.h"

/*!
 * @addtogroup port
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.port"
#endif

/*! @name Driver version */
/*@{*/
/*! @brief PORT driver version. */
#define FSL_PORT_DRIVER_VERSION (MAKE_VERSION(2, 4, 1))
/*@}*/

#if defined(FSL_FEATURE_PORT_HAS_PULL_ENABLE) && FSL_FEATURE_PORT_HAS_PULL_ENABLE
/*! @brief Internal resistor pull feature selection */
enum _port_pull
{
    kPORT_PullDisable = 0U, /*!< Internal pull-up/down resistor is disabled. */
    kPORT_PullDown    = 2U, /*!< Internal pull-down resistor is enabled. */
    kPORT_PullUp      = 3U, /*!< Internal pull-up resistor is enabled. */
};
#endif /* FSL_FEATURE_PORT_HAS_PULL_ENABLE */

#if defined(FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE) && FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE
/*! @brief Internal resistor pull value selection */
enum _port_pull_value
{
    kPORT_LowPullResistor  = 0U, /*!< Low internal pull resistor value is selected. */
    kPORT_HighPullResistor = 1U, /*!< High internal pull resistor value is selected. */
};
#endif /* FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE */

#if defined(FSL_FEATURE_PORT_HAS_SLEW_RATE) && FSL_FEATURE_PORT_HAS_SLEW_RATE
/*! @brief Slew rate selection */
enum _port_slew_rate
{
    kPORT_FastSlewRate = 0U, /*!< Fast slew rate is configured. */
    kPORT_SlowSlewRate = 1U, /*!< Slow slew rate is configured. */
};
#endif /* FSL_FEATURE_PORT_HAS_SLEW_RATE */

#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
/*! @brief Open Drain feature enable/disable */
enum _port_open_drain_enable
{
    kPORT_OpenDrainDisable = 0U, /*!< Open drain output is disabled. */
    kPORT_OpenDrainEnable  = 1U, /*!< Open drain output is enabled. */
};
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */

#if defined(FSL_FEATURE_PORT_HAS_PASSIVE_FILTER) && FSL_FEATURE_PORT_HAS_PASSIVE_FILTER
/*! @brief Passive filter feature enable/disable */
enum _port_passive_filter_enable
{
    kPORT_PassiveFilterDisable = 0U, /*!< Passive input filter is disabled. */
    kPORT_PassiveFilterEnable  = 1U, /*!< Passive input filter is enabled. */
};
#endif

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
/*! @brief Configures the drive strength. */
enum _port_drive_strength
{
    kPORT_LowDriveStrength  = 0U, /*!< Low-drive strength is configured. */
    kPORT_HighDriveStrength = 1U, /*!< High-drive strength is configured. */
};
#endif /* FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH */

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1
/*! @brief Configures the drive strength1. */
enum _port_drive_strength1
{
    kPORT_NormalDriveStrength = 0U, /*!< Normal drive strength */
    kPORT_DoubleDriveStrength = 1U, /*!< Double drive strength */
};
#endif /* FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1 */

#if defined(FSL_FEATURE_PORT_HAS_INPUT_BUFFER) && FSL_FEATURE_PORT_HAS_INPUT_BUFFER
/*! @brief input buffer disable/enable. */
enum _port_input_buffer
{
    kPORT_InputBufferDisable = 0U, /*!< Digital input is disabled */
    kPORT_InputBufferEnable  = 1U, /*!< Digital input is enabled */
};
#endif /* FSL_FEATURE_PORT_HAS_INPUT_BUFFER */

#if defined(FSL_FEATURE_PORT_HAS_INVERT_INPUT) && FSL_FEATURE_PORT_HAS_INVERT_INPUT
/*! @brief Digital input is not inverted or it is inverted. */
enum _port_invet_input
{
    kPORT_InputNormal = 0U, /*!< Digital input is not inverted */
    kPORT_InputInvert = 1U, /*!< Digital input is inverted */
};
#endif /* FSL_FEATURE_PORT_HAS_INVERT_INPUT */

#if defined(FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK) && FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK
/*! @brief Unlock/lock the pin control register field[15:0] */
enum _port_lock_register
{
    kPORT_UnlockRegister = 0U, /*!< Pin Control Register fields [15:0] are not locked. */
    kPORT_LockRegister   = 1U, /*!< Pin Control Register fields [15:0] are locked. */
};
#endif /* FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK */

#if defined(FSL_FEATURE_PORT_PCR_MUX_WIDTH) && FSL_FEATURE_PORT_PCR_MUX_WIDTH
/*! @brief Pin mux selection */
typedef enum _port_mux
{
    kPORT_PinDisabledOrAnalog = 0U,  /*!< Corresponding pin is disabled, but is used as an analog pin. */
    kPORT_MuxAsGpio           = 1U,  /*!< Corresponding pin is configured as GPIO. */
    kPORT_MuxAlt0             = 0U,  /*!< Chip-specific */
    kPORT_MuxAlt1             = 1U,  /*!< Chip-specific */
    kPORT_MuxAlt2             = 2U,  /*!< Chip-specific */
    kPORT_MuxAlt3             = 3U,  /*!< Chip-specific */
    kPORT_MuxAlt4             = 4U,  /*!< Chip-specific */
    kPORT_MuxAlt5             = 5U,  /*!< Chip-specific */
    kPORT_MuxAlt6             = 6U,  /*!< Chip-specific */
    kPORT_MuxAlt7             = 7U,  /*!< Chip-specific */
    kPORT_MuxAlt8             = 8U,  /*!< Chip-specific */
    kPORT_MuxAlt9             = 9U,  /*!< Chip-specific */
    kPORT_MuxAlt10            = 10U, /*!< Chip-specific */
    kPORT_MuxAlt11            = 11U, /*!< Chip-specific */
    kPORT_MuxAlt12            = 12U, /*!< Chip-specific */
    kPORT_MuxAlt13            = 13U, /*!< Chip-specific */
    kPORT_MuxAlt14            = 14U, /*!< Chip-specific */
    kPORT_MuxAlt15            = 15U, /*!< Chip-specific */
} port_mux_t;
#endif /* FSL_FEATURE_PORT_PCR_MUX_WIDTH */

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*! @brief Configures the interrupt generation condition. */
typedef enum _port_interrupt
{
    kPORT_InterruptOrDMADisabled = 0x0U, /*!< Interrupt/DMA request is disabled. */
#if defined(FSL_FEATURE_PORT_HAS_DMA_REQUEST) && FSL_FEATURE_PORT_HAS_DMA_REQUEST || defined(DOXYGEN_OUTPUT)
    kPORT_DMARisingEdge  = 0x1U, /*!< DMA request on rising edge. */
    kPORT_DMAFallingEdge = 0x2U, /*!< DMA request on falling edge. */
    kPORT_DMAEitherEdge  = 0x3U, /*!< DMA request on either edge. */
#endif
#if defined(FSL_FEATURE_PORT_HAS_IRQC_FLAG) && FSL_FEATURE_PORT_HAS_IRQC_FLAG || defined(DOXYGEN_OUTPUT)
    kPORT_FlagRisingEdge  = 0x05U, /*!< Flag sets on rising edge. */
    kPORT_FlagFallingEdge = 0x06U, /*!< Flag sets on falling edge. */
    kPORT_FlagEitherEdge  = 0x07U, /*!< Flag sets on either edge. */
#endif
    kPORT_InterruptLogicZero   = 0x8U, /*!< Interrupt when logic zero. */
    kPORT_InterruptRisingEdge  = 0x9U, /*!< Interrupt on rising edge. */
    kPORT_InterruptFallingEdge = 0xAU, /*!< Interrupt on falling edge. */
    kPORT_InterruptEitherEdge  = 0xBU, /*!< Interrupt on either edge. */
    kPORT_InterruptLogicOne    = 0xCU, /*!< Interrupt when logic one. */
#if defined(FSL_FEATURE_PORT_HAS_IRQC_TRIGGER) && FSL_FEATURE_PORT_HAS_IRQC_TRIGGER || defined(DOXYGEN_OUTPUT)
    kPORT_ActiveHighTriggerOutputEnable = 0xDU, /*!< Enable active high-trigger output. */
    kPORT_ActiveLowTriggerOutputEnable  = 0xEU, /*!< Enable active low-trigger output. */
#endif
} port_interrupt_t;
#endif

#if defined(FSL_FEATURE_PORT_HAS_DIGITAL_FILTER) && FSL_FEATURE_PORT_HAS_DIGITAL_FILTER
/*! @brief Digital filter clock source selection */
typedef enum _port_digital_filter_clock_source
{
    kPORT_BusClock = 0U, /*!< Digital filters are clocked by the bus clock. */
    kPORT_LpoClock = 1U, /*!< Digital filters are clocked by the 1 kHz LPO clock. */
} port_digital_filter_clock_source_t;

/*! @brief PORT digital filter feature configuration definition */
typedef struct _port_digital_filter_config
{
    uint32_t digitalFilterWidth;                    /*!< Set digital filter width */
    port_digital_filter_clock_source_t clockSource; /*!< Set digital filter clockSource */
} port_digital_filter_config_t;
#endif /* FSL_FEATURE_PORT_HAS_DIGITAL_FILTER */

#if defined(FSL_FEATURE_PORT_PCR_MUX_WIDTH) && FSL_FEATURE_PORT_PCR_MUX_WIDTH
/*! @brief PORT pin configuration structure */
typedef struct _port_pin_config
{
#if defined(FSL_FEATURE_PORT_HAS_PULL_ENABLE) && FSL_FEATURE_PORT_HAS_PULL_ENABLE
    uint16_t pullSelect : 2; /*!< No-pull/pull-down/pull-up select */
#else
    uint16_t : 2;
#endif /* FSL_FEATURE_PORT_HAS_PULL_ENABLE */

#if defined(FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE) && FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE
    uint16_t pullValueSelect : 1; /*!< Pull value select */
#endif                            /* FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE */

#if defined(FSL_FEATURE_PORT_HAS_SLEW_RATE) && FSL_FEATURE_PORT_HAS_SLEW_RATE
    uint16_t slewRate : 1; /*!< Fast/slow slew rate Configure */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_SLEW_RATE */

#if !(defined(FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE) && FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE)
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE */

#if defined(FSL_FEATURE_PORT_HAS_PASSIVE_FILTER) && FSL_FEATURE_PORT_HAS_PASSIVE_FILTER
    uint16_t passiveFilterEnable : 1; /*!< Passive filter enable/disable */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_PASSIVE_FILTER */

#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    uint16_t openDrainEnable : 1; /*!< Open drain enable/disable */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
    uint16_t driveStrength : 1; /*!< Fast/slow drive strength configure */
#else
    uint16_t : 1;
#endif

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1
    uint16_t driveStrength1 : 1; /*!< Normal/Double drive strength enable/disable */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1 */

#if defined(FSL_FEATURE_PORT_PCR_MUX_WIDTH) && (FSL_FEATURE_PORT_PCR_MUX_WIDTH == 3)
    uint16_t mux : 3; /*!< Pin mux Configure */
    uint16_t : 1;
#elif defined(FSL_FEATURE_PORT_PCR_MUX_WIDTH) && (FSL_FEATURE_PORT_PCR_MUX_WIDTH == 4)
    uint16_t mux : 4; /*!< Pin mux Configure */
#else
    uint16_t : 4;
#endif

#if defined(FSL_FEATURE_PORT_HAS_INPUT_BUFFER) && FSL_FEATURE_PORT_HAS_INPUT_BUFFER
    uint16_t inputBuffer : 1; /*!< Input Buffer Configure */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_INPUT_BUFFER */

#if defined(FSL_FEATURE_PORT_HAS_INVERT_INPUT) && FSL_FEATURE_PORT_HAS_INVERT_INPUT
    uint16_t invertInput : 1; /*!< Invert Input Configure */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_INVERT_INPUT */

    uint16_t : 1;

#if defined(FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK) && FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK
    uint16_t lockRegister : 1; /*!< Lock/unlock the PCR field[15:0] */
#else
    uint16_t : 1;
#endif /* FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK */
} port_pin_config_t;
#endif /* FSL_FEATURE_PORT_PCR_MUX_WIDTH */

#if defined(FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER) && FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER
/*! @brief PORT version information. */
typedef struct _port_version_info
{
    uint16_t feature; /*!< Feature Specification Number. */
    uint8_t minor;    /*!< Minor Version Number. */
    uint8_t major;    /*!< Major Version Number. */
} port_version_info_t;
#endif /* FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER */

#if defined(FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE) && FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE
/*! @brief PORT voltage range. */
typedef enum _port_voltage_range
{
    kPORT_VoltageRange1Dot71V_3Dot6V = 0x0U, /*!< Port voltage range is 1.71 V - 3.6 V. */
    kPORT_VoltageRange2Dot70V_3Dot6V = 0x1U, /*!< Port voltage range is 2.70 V - 3.6 V. */
} port_voltage_range_t;
#endif /* FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(FSL_FEATURE_PORT_PCR_MUX_WIDTH) && FSL_FEATURE_PORT_PCR_MUX_WIDTH
/*! @name Configuration */
/*@{*/

#if defined(FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER) && FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER
/*!
 * @brief Get PORT version information.
 *
 * @param base PORT peripheral base pointer
 * @param info PORT version information
 */
static inline void PORT_GetVersionInfo(PORT_Type *base, port_version_info_t *info)
{
    uint32_t verid = base->VERID;
    info->feature  = (uint16_t)verid;
    info->minor    = (uint8_t)(verid >> PORT_VERID_MINOR_SHIFT);
    info->major    = (uint8_t)(verid >> PORT_VERID_MAJOR_SHIFT);
}
#endif /* FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER */

#if defined(FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE) && FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE
/*!
 * @brief Get PORT version information.
 *
 * @note : PORTA_CONFIG[RANGE] controls the voltage ranges of Port A, B, and C. Read or write PORTB_CONFIG[RANGE] and
 *         PORTC_CONFIG[RANGE] does not take effect.
 *
 * @param base PORT peripheral base pointer
 * @param range port voltage range
 */
static inline void PORT_SecletPortVoltageRange(PORT_Type *base, port_voltage_range_t range)
{
    base->CONFIG = (uint32_t)range;
}
#endif /* FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE */

/*!
 * @brief Sets the port PCR register.
 *
 * This is an example to define an input pin or output pin PCR configuration.
 * @code
 * // Define a digital input pin PCR configuration
 * port_pin_config_t config = {
 *      kPORT_PullUp,
 *      kPORT_FastSlewRate,
 *      kPORT_PassiveFilterDisable,
 *      kPORT_OpenDrainDisable,
 *      kPORT_LowDriveStrength,
 *      kPORT_MuxAsGpio,
 *      kPORT_UnLockRegister,
 * };
 * @endcode
 *
 * @param base   PORT peripheral base pointer.
 * @param pin    PORT pin number.
 * @param config PORT PCR register configuration structure.
 */
static inline void PORT_SetPinConfig(PORT_Type *base, uint32_t pin, const port_pin_config_t *config)
{
    assert(config);
    uint32_t addr                = (uint32_t)&base->PCR[pin];
    *(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)config);
}

/*!
 * @brief Sets the port PCR register for multiple pins.
 *
 * This is an example to define input pins or output pins PCR configuration.
 * @code
 * Define a digital input pin PCR configuration
 * port_pin_config_t config = {
 *      kPORT_PullUp ,
 *      kPORT_PullEnable,
 *      kPORT_FastSlewRate,
 *      kPORT_PassiveFilterDisable,
 *      kPORT_OpenDrainDisable,
 *      kPORT_LowDriveStrength,
 *      kPORT_MuxAsGpio,
 *      kPORT_UnlockRegister,
 * };
 * @endcode
 *
 * @param base   PORT peripheral base pointer.
 * @param mask   PORT pin number macro.
 * @param config PORT PCR register configuration structure.
 */
static inline void PORT_SetMultiplePinsConfig(PORT_Type *base, uint32_t mask, const port_pin_config_t *config)
{
    assert(config);

    uint16_t pcrl = *((const uint16_t *)(const void *)config);

    if (0U != (mask & 0xffffU))
    {
        base->GPCLR = ((mask & 0xffffU) << 16) | pcrl;
    }
    if (0U != (mask >> 16))
    {
        base->GPCHR = (mask & 0xffff0000U) | pcrl;
    }
}

#if defined(FSL_FEATURE_PORT_HAS_MULTIPLE_IRQ_CONFIG) && FSL_FEATURE_PORT_HAS_MULTIPLE_IRQ_CONFIG
/*!
 * @brief Sets the port interrupt configuration in PCR register for multiple pins.
 *
 * @param base   PORT peripheral base pointer.
 * @param mask   PORT pin number macro.
 * @param config  PORT pin interrupt configuration.
 *        - #kPORT_InterruptOrDMADisabled: Interrupt/DMA request disabled.
 *        - #kPORT_DMARisingEdge : DMA request on rising edge(if the DMA requests exit).
 *        - #kPORT_DMAFallingEdge: DMA request on falling edge(if the DMA requests exit).
 *        - #kPORT_DMAEitherEdge : DMA request on either edge(if the DMA requests exit).
 *        - #kPORT_FlagRisingEdge : Flag sets on rising edge(if the Flag states exit).
 *        - #kPORT_FlagFallingEdge : Flag sets on falling edge(if the Flag states exit).
 *        - #kPORT_FlagEitherEdge : Flag sets on either edge(if the Flag states exit).
 *        - #kPORT_InterruptLogicZero  : Interrupt when logic zero.
 *        - #kPORT_InterruptRisingEdge : Interrupt on rising edge.
 *        - #kPORT_InterruptFallingEdge: Interrupt on falling edge.
 *        - #kPORT_InterruptEitherEdge : Interrupt on either edge.
 *        - #kPORT_InterruptLogicOne   : Interrupt when logic one.
 *        - #kPORT_ActiveHighTriggerOutputEnable : Enable active high-trigger output (if the trigger states exit).
 *        - #kPORT_ActiveLowTriggerOutputEnable  : Enable active low-trigger output (if the trigger states exit)..
 */
static inline void PORT_SetMultipleInterruptPinsConfig(PORT_Type *base, uint32_t mask, port_interrupt_t config)
{
    assert(config);

    if (0U != ((uint32_t)mask & 0xffffU))
    {
        base->GICLR = ((uint32_t)config << 16U) | ((uint32_t)mask & 0xffffU);
    }
    mask = mask >> 16;
    if (0U != mask)
    {
        base->GICHR = ((uint32_t)config << 16U) | ((uint32_t)mask & 0xffffU);
    }
}
#endif

/*!
 * @brief Configures the pin muxing.
 *
 * @param base  PORT peripheral base pointer.
 * @param pin   PORT pin number.
 * @param mux   pin muxing slot selection.
 *        - #kPORT_PinDisabledOrAnalog: Pin disabled or work in analog function.
 *        - #kPORT_MuxAsGpio          : Set as GPIO.
 *        - #kPORT_MuxAlt2            : chip-specific.
 *        - #kPORT_MuxAlt3            : chip-specific.
 *        - #kPORT_MuxAlt4            : chip-specific.
 *        - #kPORT_MuxAlt5            : chip-specific.
 *        - #kPORT_MuxAlt6            : chip-specific.
 *        - #kPORT_MuxAlt7            : chip-specific.
 * @note : This function is NOT recommended to use together with the PORT_SetPinsConfig, because
 *         the PORT_SetPinsConfig need to configure the pin mux anyway (Otherwise the pin mux is
 *         reset to zero : kPORT_PinDisabledOrAnalog).
 *        This function is recommended to use to reset the pin mux
 *
 */
static inline void PORT_SetPinMux(PORT_Type *base, uint32_t pin, port_mux_t mux)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(mux);
}
#endif /* FSL_FEATURE_PORT_PCR_MUX_WIDTH */

#if defined(FSL_FEATURE_PORT_HAS_DIGITAL_FILTER) && FSL_FEATURE_PORT_HAS_DIGITAL_FILTER

/*!
 * @brief Enables the digital filter in one port, each bit of the 32-bit register represents one pin.
 *
 * @param base    PORT peripheral base pointer.
 * @param mask    PORT pin number macro.
 * @param enable  PORT digital filter configuration.
 */
static inline void PORT_EnablePinsDigitalFilter(PORT_Type *base, uint32_t mask, bool enable)
{
    if (enable == true)
    {
        base->DFER |= mask;
    }
    else
    {
        base->DFER &= ~mask;
    }
}

/*!
 * @brief Sets the digital filter in one port, each bit of the 32-bit register represents one pin.
 *
 * @param base   PORT peripheral base pointer.
 * @param config PORT digital filter configuration structure.
 */
static inline void PORT_SetDigitalFilterConfig(PORT_Type *base, const port_digital_filter_config_t *config)
{
    assert(config);

    base->DFCR = PORT_DFCR_CS(config->clockSource);
    base->DFWR = PORT_DFWR_FILT(config->digitalFilterWidth);
}

#endif /* FSL_FEATURE_PORT_HAS_DIGITAL_FILTER */
/*@}*/

/*! @name Interrupt */
/*@{*/

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * @brief Configures the port pin interrupt/DMA request.
 *
 * @param base    PORT peripheral base pointer.
 * @param pin     PORT pin number.
 * @param config  PORT pin interrupt configuration.
 *        - #kPORT_InterruptOrDMADisabled: Interrupt/DMA request disabled.
 *        - #kPORT_DMARisingEdge : DMA request on rising edge(if the DMA requests exit).
 *        - #kPORT_DMAFallingEdge: DMA request on falling edge(if the DMA requests exit).
 *        - #kPORT_DMAEitherEdge : DMA request on either edge(if the DMA requests exit).
 *        - #kPORT_FlagRisingEdge : Flag sets on rising edge(if the Flag states exit).
 *        - #kPORT_FlagFallingEdge : Flag sets on falling edge(if the Flag states exit).
 *        - #kPORT_FlagEitherEdge : Flag sets on either edge(if the Flag states exit).
 *        - #kPORT_InterruptLogicZero  : Interrupt when logic zero.
 *        - #kPORT_InterruptRisingEdge : Interrupt on rising edge.
 *        - #kPORT_InterruptFallingEdge: Interrupt on falling edge.
 *        - #kPORT_InterruptEitherEdge : Interrupt on either edge.
 *        - #kPORT_InterruptLogicOne   : Interrupt when logic one.
 *        - #kPORT_ActiveHighTriggerOutputEnable : Enable active high-trigger output (if the trigger states exit).
 *        - #kPORT_ActiveLowTriggerOutputEnable  : Enable active low-trigger output (if the trigger states exit).
 */
static inline void PORT_SetPinInterruptConfig(PORT_Type *base, uint32_t pin, port_interrupt_t config)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(config);
}
#endif

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
/*!
 * @brief Configures the port pin drive strength.
 *
 * @param base      PORT peripheral base pointer.
 * @param pin       PORT pin number.
 * @param strength  PORT pin drive strength
 *        - #kPORT_LowDriveStrength = 0U - Low-drive strength is configured.
 *        - #kPORT_HighDriveStrength = 1U - High-drive strength is configured.
 */
static inline void PORT_SetPinDriveStrength(PORT_Type *base, uint32_t pin, uint8_t strength)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_DSE_MASK) | PORT_PCR_DSE(strength);
}
#endif

#if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1
/*!
 * @brief Enables the port pin double drive strength.
 *
 * @param base      PORT peripheral base pointer.
 * @param pin       PORT pin number.
 * @param enable  PORT pin drive strength configuration.
 */
static inline void PORT_EnablePinDoubleDriveStrength(PORT_Type *base, uint32_t pin, bool enable)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_DSE1_MASK) | PORT_PCR_DSE1(enable);
}
#endif

#if defined(FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE) && FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE
/*!
 * @brief Configures the port pin pull value.
 *
 * @param base      PORT peripheral base pointer.
 * @param pin       PORT pin number.
 * @param value  PORT pin pull value
 *        - #kPORT_LowPullResistor = 0U - Low internal pull resistor value is selected.
 *        - #kPORT_HighPullResistor = 1U - High internal pull resistor value is selected.
 */
static inline void PORT_SetPinPullValue(PORT_Type *base, uint32_t pin, uint8_t value)
{
    base->PCR[pin] = (base->PCR[pin] & ~PORT_PCR_PV_MASK) | PORT_PCR_PV(value);
}
#endif /* FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE */

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * @brief Reads the whole port status flag.
 *
 * If a pin is configured to generate the DMA request,  the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * @param base PORT peripheral base pointer.
 * @return Current port interrupt status flags, for example, 0x00010001 means the
 *         pin 0 and 16 have the interrupt.
 */
static inline uint32_t PORT_GetPinsInterruptFlags(PORT_Type *base)
{
    return base->ISFR;
}

/*!
 * @brief Clears the multiple pin interrupt status flag.
 *
 * @param base PORT peripheral base pointer.
 * @param mask PORT pin number macro.
 */
static inline void PORT_ClearPinsInterruptFlags(PORT_Type *base, uint32_t mask)
{
    base->ISFR = mask;
}
#endif

#if defined(FSL_FEATURE_PORT_SUPPORT_EFT) && FSL_FEATURE_PORT_SUPPORT_EFT
/*!
 * @brief Get EFT detect flags.
 *
 * @param base PORT peripheral base pointer
 * @return EFT detect flags
 */
static inline uint32_t PORT_GetEFTDetectFlags(PORT_Type *base)
{
    return base->EDFR;
}

/*!
 * @brief Enable EFT detect interrupts.
 *
 * @param base PORT peripheral base pointer
 * @param interrupt EFT detect interrupt
 */
static inline void PORT_EnableEFTDetectInterrupts(PORT_Type *base, uint32_t interrupt)
{
    base->EDIER |= interrupt;
}

/*!
 * @brief Disable EFT detect interrupts.
 *
 * @param base PORT peripheral base pointer
 * @param interrupt EFT detect interrupt
 */
static inline void PORT_DisableEFTDetectInterrupts(PORT_Type *base, uint32_t interrupt)
{
    base->EDIER &= ~interrupt;
}

/*!
 * @brief Clear all low EFT detector.
 *
 * @note : Port B and Port C pins share the same EFT detector clear control from PORTC_EDCR register. Any write to the
 * PORTB_EDCR does not take effect.
 * @param base PORT peripheral base pointer
 * @param interrupt EFT detect interrupt
 */
static inline void PORT_ClearAllLowEFTDetectors(PORT_Type *base)
{
    base->EDCR |= PORT_EDCR_EDLC_MASK;
    base->EDCR &= ~PORT_EDCR_EDLC_MASK;
}

/*!
 * @brief Clear all high EFT detector.
 *
 * @param base PORT peripheral base pointer
 * @param interrupt EFT detect interrupt
 */
static inline void PORT_ClearAllHighEFTDetectors(PORT_Type *base)
{
    base->EDCR |= PORT_EDCR_EDHC_MASK;
    base->EDCR &= ~PORT_EDCR_EDHC_MASK;
}
#endif /* FSL_FEATURE_PORT_SUPPORT_EFT */

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FSL_PORT_H_ */
