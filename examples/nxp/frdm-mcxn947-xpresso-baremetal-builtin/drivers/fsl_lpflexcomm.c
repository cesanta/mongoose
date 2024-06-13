/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_lpflexcomm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.lpflexcomm"
#endif

/*!
 * @brief Used for conversion between `void*` and `uint32_t`.
 */
typedef union pvoid_to_u32
{
    void *pvoid;
    uint32_t u32;
} pvoid_to_u32_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*! @brief check whether lpflexcomm supports peripheral type */
static bool LP_FLEXCOMM_PeripheralIsPresent(LP_FLEXCOMM_Type *base, LP_FLEXCOMM_PERIPH_T periph);

/*! @brief Changes LP_FLEXCOMM mode. */
static status_t LP_FLEXCOMM_SetPeriph(uint32_t instance, LP_FLEXCOMM_PERIPH_T periph, int lock);

/*! @brief Common LPFLEXCOMM IRQhandle. */
static void LP_FLEXCOMM_CommonIRQHandler(uint32_t instance);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array to map LP_FLEXCOMM instance number to base address. */
static const uint32_t s_lpflexcommBaseAddrs[] = LP_FLEXCOMM_BASE_ADDRS;

/*! @brief Array to map LP_FLEXCOMM instance PTRS. */
static LP_FLEXCOMM_Type *const s_lpflexcommBase[] = LP_FLEXCOMM_BASE_PTRS;

/*! @brief Pointers to real IRQ handlers installed by drivers for each instance. */
static lpflexcomm_irq_handler_t s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPI2C + 1][ARRAY_SIZE(s_lpflexcommBaseAddrs)];

/*! @brief Pointers to handles for each instance to provide context to interrupt routines */
static void *s_lpflexcommHandle[LP_FLEXCOMM_PERIPH_LPI2C + 1][ARRAY_SIZE(s_lpflexcommBaseAddrs)];

/*! @brief Array to map LP_FLEXCOMM instance number to IRQ number. */
IRQn_Type const kFlexcommIrqs[] = LP_FLEXCOMM_IRQS;

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief IDs of clock for each LP_FLEXCOMM module */
static const clock_ip_name_t s_lpflexcommClocks[] = LP_FLEXCOMM_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_LP_FLEXCOMM_HAS_NO_RESET) && FSL_FEATURE_LP_FLEXCOMM_HAS_NO_RESET)
/*! @brief Pointers to LP_FLEXCOMM resets for each instance. */
static const reset_ip_name_t s_lpflexcommResets[] = LP_FLEXCOMM_RSTS;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

/* check whether lpflexcomm supports peripheral type */
static bool LP_FLEXCOMM_PeripheralIsPresent(LP_FLEXCOMM_Type *base, LP_FLEXCOMM_PERIPH_T periph)
{
    if (periph == LP_FLEXCOMM_PERIPH_NONE)
    {
        return true;
    }
    else if (periph <= LP_FLEXCOMM_PERIPH_LPI2C)
    {
        return (base->PSELID & (1UL << ((uint32_t)periph + 3U))) > 0UL ? true : false;
    }
    else if (periph == LP_FLEXCOMM_PERIPH_LPI2CAndLPUART)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*! @brief Returns for LP_FLEXCOMM base address. */
uint32_t LP_FLEXCOMM_GetBaseAddress(uint32_t instance)
{
    if(instance < (uint32_t)ARRAY_SIZE(s_lpflexcommBaseAddrs))
    {
        return s_lpflexcommBaseAddrs[instance];
    }
    return 0U;
}

/*! brief Returns for LP_FLEXCOMM interrupt source,see #_lpflexcomm_interrupt_flag. */
uint32_t LP_FLEXCOMM_GetInterruptStatus(uint32_t instance)
{
    LP_FLEXCOMM_Type *base = s_lpflexcommBase[instance];
    return base->ISTAT;
}

/* Get the index corresponding to the LP_FLEXCOMM */
/*! brief Returns instance number for LP_FLEXCOMM module with given base address. */
uint32_t LP_FLEXCOMM_GetInstance(void *base)
{
    uint32_t i;
    pvoid_to_u32_t BaseAddr;
    BaseAddr.pvoid = base;

    for (i = 0U; i < (uint32_t)ARRAY_SIZE(s_lpflexcommBaseAddrs); i++)
    {
        if (BaseAddr.u32 == s_lpflexcommBaseAddrs[i])
        {
            break;
        }
    }

    assert(i < (uint32_t)ARRAY_SIZE(s_lpflexcommBaseAddrs));
    return i;
}

/* Changes LP_FLEXCOMM mode */
static status_t LP_FLEXCOMM_SetPeriph(uint32_t instance, LP_FLEXCOMM_PERIPH_T periph, int lock)
{
    assert(periph <= LP_FLEXCOMM_PERIPH_LPI2CAndLPUART);
    LP_FLEXCOMM_Type *base = s_lpflexcommBase[instance];

    /* Check whether peripheral type is present */
    if (!LP_FLEXCOMM_PeripheralIsPresent(base, periph))
    {
        return kStatus_OutOfRange;
    }

    /* Flexcomm is locked to different peripheral type than expected  */
    if (((base->PSELID & LP_FLEXCOMM_PSELID_LOCK_MASK) != 0U) &&
        ((base->PSELID & LP_FLEXCOMM_PSELID_PERSEL_MASK) != (uint32_t)periph))
    {
        return kStatus_Fail;
    }

    /* Check if we are asked to lock */
    if (lock != 0)
    {
        base->PSELID = (uint32_t)periph | LP_FLEXCOMM_PSELID_LOCK_MASK;
    }
    else
    {
        base->PSELID = (uint32_t)periph;
    }

    return kStatus_Success;
}

/*! brief Initializes LP_FLEXCOMM and selects peripheral mode according to the second parameter. */
status_t LP_FLEXCOMM_Init(uint32_t instance, LP_FLEXCOMM_PERIPH_T periph)
{
    assert(instance < (uint32_t)ARRAY_SIZE(s_lpflexcommBase));
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Enable the peripheral clock */
    CLOCK_EnableClock(s_lpflexcommClocks[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#if !(defined(FSL_FEATURE_LP_FLEXCOMM_HAS_NO_RESET) && FSL_FEATURE_LP_FLEXCOMM_HAS_NO_RESET)
    /* Reset the LP_FLEXCOMM module before configuring it.*/
    RESET_ClearPeripheralReset(s_lpflexcommResets[instance]);
#endif
    /* Set the LP_FLEXCOMM to given peripheral */
    return LP_FLEXCOMM_SetPeriph(instance, periph, 0);
}

/*! brief Deinitializes LP_FLEXCOMM. */
void LP_FLEXCOMM_Deinit(uint32_t instance)
{
    assert(instance < (uint32_t)ARRAY_SIZE(s_lpflexcommBase));
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Disable the peripheral clock */
    CLOCK_DisableClock(s_lpflexcommClocks[instance]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
    RESET_SetPeripheralReset(s_lpflexcommResets[instance]);
}

/*! brief Sets IRQ handler for given LP_FLEXCOMM module. It is used by drivers register IRQ handler according to
 * LP_FLEXCOMM mode */
void LP_FLEXCOMM_SetIRQHandler(uint32_t instance,
                               lpflexcomm_irq_handler_t handler,
                               void *lpflexcommHandle,
                               LP_FLEXCOMM_PERIPH_T periph)
{
    assert(instance < (uint32_t)ARRAY_SIZE(s_lpflexcommBase));
    /* Clear handler first to avoid execution of the handler with wrong handle */
    s_lpflexcommIrqHandler[periph][instance] = NULL;
    s_lpflexcommHandle[periph][instance]     = lpflexcommHandle;
    s_lpflexcommIrqHandler[periph][instance] = handler;
}

static void LP_FLEXCOMM_CommonIRQHandler(uint32_t instance)
{
    uint32_t interruptStat;

    interruptStat = LP_FLEXCOMM_GetInterruptStatus(instance);
    if ((interruptStat &
         ((uint32_t)kLPFLEXCOMM_I2cSlaveInterruptFlag | (uint32_t)kLPFLEXCOMM_I2cMasterInterruptFlag)) != 0U)
    {
        if (s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPI2C][instance] != NULL)
        {
            s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPI2C][instance](
                instance, s_lpflexcommHandle[LP_FLEXCOMM_PERIPH_LPI2C][instance]);
        }
    }
    if ((interruptStat & ((uint32_t)kLPFLEXCOMM_UartRxInterruptFlag | (uint32_t)kLPFLEXCOMM_UartTxInterruptFlag)) != 0U)
    {
        if (s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPUART][instance] != NULL)
        {
            s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPUART][instance](
                instance, s_lpflexcommHandle[LP_FLEXCOMM_PERIPH_LPUART][instance]);
        }
    }
    if (((interruptStat & (uint32_t)kLPFLEXCOMM_SpiInterruptFlag)) != 0U)
    {
        if (s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPSPI][instance] != NULL)
        {
            s_lpflexcommIrqHandler[LP_FLEXCOMM_PERIPH_LPSPI][instance](
                instance, s_lpflexcommHandle[LP_FLEXCOMM_PERIPH_LPSPI][instance]);
        }
    }
    SDK_ISR_EXIT_BARRIER;
}

/* IRQ handler functions overloading weak symbols in the startup */
#if defined(LP_FLEXCOMM0)
void LP_FLEXCOMM0_DriverIRQHandler(void);
void LP_FLEXCOMM0_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(0U);
}
#endif

#if defined(LP_FLEXCOMM1)
void LP_FLEXCOMM1_DriverIRQHandler(void);
void LP_FLEXCOMM1_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(1U);
}
#endif

#if defined(LP_FLEXCOMM2)
void LP_FLEXCOMM2_DriverIRQHandler(void);
void LP_FLEXCOMM2_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(2U);
}
#endif

#if defined(LP_FLEXCOMM3)
void LP_FLEXCOMM3_DriverIRQHandler(void);
void LP_FLEXCOMM3_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(3U);
}
#endif

#if defined(LP_FLEXCOMM4)
void LP_FLEXCOMM4_DriverIRQHandler(void);
void LP_FLEXCOMM4_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(4U);
}
#endif

#if defined(LP_FLEXCOMM5)
void LP_FLEXCOMM5_DriverIRQHandler(void);
void LP_FLEXCOMM5_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(5U);
}
#endif

#if defined(LP_FLEXCOMM6)
void LP_FLEXCOMM6_DriverIRQHandler(void);
void LP_FLEXCOMM6_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(6U);
}
#endif

#if defined(LP_FLEXCOMM7)
void LP_FLEXCOMM7_DriverIRQHandler(void);
void LP_FLEXCOMM7_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(7U);
}
#endif

#if defined(LP_FLEXCOMM8)
void LP_FLEXCOMM8_DriverIRQHandler(void);
void LP_FLEXCOMM8_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(8U);
}
#endif

#if defined(LP_FLEXCOMM9)
void LP_FLEXCOMM9_DriverIRQHandler(void);
void LP_FLEXCOMM9_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(9U);
}
#endif

#if defined(LP_FLEXCOMM10)
void LP_FLEXCOMM10_DriverIRQHandler(void);
void LP_FLEXCOMM10_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(10U);
}
#endif

#if defined(LP_FLEXCOMM11)
void LP_FLEXCOMM11_DriverIRQHandler(void);
void LP_FLEXCOMM11_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(11U);
}
#endif

#if defined(LP_FLEXCOMM12)
void LP_FLEXCOMM12_DriverIRQHandler(void);
void LP_FLEXCOMM12_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(12U);
}
#endif

#if defined(LP_FLEXCOMM13)
void LP_FLEXCOMM13_DriverIRQHandler(void);
void LP_FLEXCOMM13_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(13U);
}
#endif

#if defined(LP_FLEXCOMM17)
void LP_FLEXCOMM17_DriverIRQHandler(void);
void LP_FLEXCOMM17_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(17U);
}
#endif

#if defined(LP_FLEXCOMM18)
void LP_FLEXCOMM18_DriverIRQHandler(void);
void LP_FLEXCOMM18_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(18U);
}
#endif

#if defined(LP_FLEXCOMM19)
void LP_FLEXCOMM19_DriverIRQHandler(void);
void LP_FLEXCOMM19_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(19U);
}
#endif

#if defined(LP_FLEXCOMM20)
void LP_FLEXCOMM20_DriverIRQHandler(void);
void LP_FLEXCOMM20_DriverIRQHandler(void)
{
    LP_FLEXCOMM_CommonIRQHandler(20U);
}
#endif
