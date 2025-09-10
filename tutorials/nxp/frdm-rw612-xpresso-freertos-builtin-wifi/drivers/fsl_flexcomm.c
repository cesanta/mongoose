/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_flexcomm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.flexcomm"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*! @brief Set the FLEXCOMM mode . */
static status_t FLEXCOMM_SetPeriph(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph, int lock);

/*! @brief check whether flexcomm supports peripheral type */
static bool FLEXCOMM_PeripheralIsPresent(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map FLEXCOMM instance number to base address. */
static const uint32_t s_flexcommBaseAddrs[] = FLEXCOMM_BASE_ADDRS;

/*! @brief Pointers to real IRQ handlers installed by drivers for each instance. */
static flexcomm_irq_handler_t s_flexcommIrqHandler[ARRAY_SIZE(s_flexcommBaseAddrs)];

/*! @brief Pointers to handles for each instance to provide context to interrupt routines */
static void *s_flexcommHandle[ARRAY_SIZE(s_flexcommBaseAddrs)];

/*! @brief Array to map FLEXCOMM instance number to IRQ number. */
IRQn_Type const kFlexcommIrqs[] = FLEXCOMM_IRQS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief IDs of clock for each FLEXCOMM module */
static const clock_ip_name_t s_flexcommClocks[] = FLEXCOMM_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_FLEXCOMM_HAS_NO_RESET) && FSL_FEATURE_FLEXCOMM_HAS_NO_RESET)
/*! @brief Pointers to FLEXCOMM resets for each instance. */
static const reset_ip_name_t s_flexcommResets[] = FLEXCOMM_RSTS;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/* check whether flexcomm supports peripheral type */
static bool FLEXCOMM_PeripheralIsPresent(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph)
{
    if (periph == FLEXCOMM_PERIPH_NONE)
    {
        return true;
    }
    else if (periph <= FLEXCOMM_PERIPH_I2S_TX)
    {
        return (base->PSELID & (1UL << ((uint32_t)periph + 3U))) > 0UL ? true : false;
    }
    else if (periph == FLEXCOMM_PERIPH_I2S_RX)
    {
        return (base->PSELID & (1U << 7U)) > (uint32_t)0U ? true : false;
    }
    else
    {
        return false;
    }
}

/* Get the index corresponding to the FLEXCOMM */
/*! brief Returns instance number for FLEXCOMM module with given base address. */
uint32_t FLEXCOMM_GetInstance(void *base)
{
    uint32_t i;

    for (i = 0U; i < (uint32_t)FSL_FEATURE_SOC_FLEXCOMM_COUNT; i++)
    {
        if (MSDK_REG_SECURE_ADDR((uintptr_t)(uint8_t*)base) == MSDK_REG_SECURE_ADDR(s_flexcommBaseAddrs[i]))
        {
            break;
        }
    }

    assert(i < (uint32_t)FSL_FEATURE_SOC_FLEXCOMM_COUNT);
    return i;
}

/* Changes FLEXCOMM mode */
static status_t FLEXCOMM_SetPeriph(FLEXCOMM_Type *base, FLEXCOMM_PERIPH_T periph, int lock)
{
    /* Check whether peripheral type is present */
    if (!FLEXCOMM_PeripheralIsPresent(base, periph))
    {
        return kStatus_OutOfRange;
    }

    /* Flexcomm is locked to different peripheral type than expected  */
    if (((base->PSELID & FLEXCOMM_PSELID_LOCK_MASK) != 0U) &&
        ((base->PSELID & FLEXCOMM_PSELID_PERSEL_MASK) != (uint32_t)periph))
    {
        return kStatus_Fail;
    }

    /* Check if we are asked to lock */
    if (lock != 0)
    {
        base->PSELID = (uint32_t)periph | FLEXCOMM_PSELID_LOCK_MASK;
    }
    else
    {
        base->PSELID = (uint32_t)periph;
    }

    return kStatus_Success;
}

/*! brief Initializes FLEXCOMM and selects peripheral mode according to the second parameter. */
status_t FLEXCOMM_Init(void *base, FLEXCOMM_PERIPH_T periph)
{
    uint32_t idx = FLEXCOMM_GetInstance(base);

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the peripheral clock */
    CLOCK_EnableClock(s_flexcommClocks[idx]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_FLEXCOMM_HAS_NO_RESET) && FSL_FEATURE_FLEXCOMM_HAS_NO_RESET)
    /* Reset the FLEXCOMM module */
    RESET_PeripheralReset(s_flexcommResets[idx]);
#endif

    /* Set the FLEXCOMM to given peripheral */
    return FLEXCOMM_SetPeriph((FLEXCOMM_Type *)base, periph, 0);
}

/*! brief Sets IRQ handler for given FLEXCOMM module. It is used by drivers register IRQ handler according to FLEXCOMM
 * mode */
void FLEXCOMM_SetIRQHandler(void *base, flexcomm_irq_handler_t handler, void *flexcommHandle)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(base);

    /* Clear handler first to avoid execution of the handler with wrong handle */
    s_flexcommIrqHandler[instance] = NULL;
    s_flexcommHandle[instance]     = flexcommHandle;
    s_flexcommIrqHandler[instance] = handler;
    SDK_ISR_EXIT_BARRIER;
}

/* IRQ handler functions overloading weak symbols in the startup */
#if defined(FLEXCOMM0)
void FLEXCOMM0_DriverIRQHandler(void);
void FLEXCOMM0_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM0);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM1)
void FLEXCOMM1_DriverIRQHandler(void);
void FLEXCOMM1_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM1);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM2)
void FLEXCOMM2_DriverIRQHandler(void);
void FLEXCOMM2_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM2);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM3)
void FLEXCOMM3_DriverIRQHandler(void);
void FLEXCOMM3_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM3);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM4)
void FLEXCOMM4_DriverIRQHandler(void);
void FLEXCOMM4_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM4);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}

#endif

#if defined(FLEXCOMM5)
void FLEXCOMM5_DriverIRQHandler(void);
void FLEXCOMM5_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM5);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM6)
void FLEXCOMM6_DriverIRQHandler(void);
void FLEXCOMM6_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM6);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM7)
void FLEXCOMM7_DriverIRQHandler(void);
void FLEXCOMM7_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM7);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM8)
void FLEXCOMM8_DriverIRQHandler(void);
void FLEXCOMM8_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM8);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM9)
void FLEXCOMM9_DriverIRQHandler(void);
void FLEXCOMM9_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM9);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM10)
void FLEXCOMM10_DriverIRQHandler(void);
void FLEXCOMM10_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM10);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM11)
void FLEXCOMM11_DriverIRQHandler(void);
void FLEXCOMM11_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM11);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM12)
void FLEXCOMM12_DriverIRQHandler(void);
void FLEXCOMM12_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM12);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM13)
void FLEXCOMM13_DriverIRQHandler(void);
void FLEXCOMM13_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM13);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM14)
void FLEXCOMM14_DriverIRQHandler(void);
void FLEXCOMM14_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM14);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM15)
void FLEXCOMM15_DriverIRQHandler(void);
void FLEXCOMM15_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM15);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif

#if defined(FLEXCOMM16)
void FLEXCOMM16_DriverIRQHandler(void);
void FLEXCOMM16_DriverIRQHandler(void)
{
    uint32_t instance;

    /* Look up instance number */
    instance = FLEXCOMM_GetInstance(FLEXCOMM16);
    assert(s_flexcommIrqHandler[instance] != NULL);
    s_flexcommIrqHandler[instance]((uint32_t *)s_flexcommBaseAddrs[instance], s_flexcommHandle[instance]);
    SDK_ISR_EXIT_BARRIER;
}
#endif
