/*
 * Copyright 2020-2023 NXP
 *  
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_power.h"
#include <string.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.power"
#endif

#define IS_XIP_FLEXSPI()                                                                                \
    ((((uint32_t)POWER_EnableWakeup >= 0x08000000U) && ((uint32_t)POWER_EnableWakeup < 0x10000000U)) || \
     (((uint32_t)POWER_EnableWakeup >= 0x18000000U) && ((uint32_t)POWER_EnableWakeup < 0x20000000U)))

#define FLEXSPI_DLL_LOCK_RETRY (10U)

/* Wait some PMU cycles */
#define POWER_WAIT_PMU()              \
    do                                \
    {                                 \
        volatile uint32_t dummy;      \
        dummy = PMU->PWR_MODE_STATUS; \
        dummy = PMU->PWR_MODE_STATUS; \
        dummy = PMU->PWR_MODE_STATUS; \
        dummy = PMU->PWR_MODE_STATUS; \
        dummy = PMU->PWR_MODE_STATUS; \
        (void)dummy;                  \
    } while (false)

#define POWER_WLAN_POWER_STATUS() (SOCCTRL->WLAN_POWER_STATUS & 0x3U)
#define POWER_BLE_POWER_STATUS()  (SOCCTRL->BLE_POWER_STATUS & 0x3U)
#define POWER_WLAN_BLE_POWER_ON   (0U)
#define POWER_WLAN_BLE_POWER_SLP  (2U)
#define POWER_WLAN_BLE_POWER_OFF  (3U)

#define ITRC_OUT_SEL_MASK          (0x0C000FF0U)
#define ITRC_OUT_SEL_DISABLE       (0x08000AA0U)
#define ITRC_OUT_SEL_EVENT_MASK    (0xFU)
#define ITRC_OUT_SEL_EVENT_DISABLE (0xAU)

#define POWER_WRITE_MEM32(addr, val)            \
    do                                          \
    {                                           \
        *((volatile uint32_t *)(addr)) = (val); \
    } while (false)

typedef struct _power_nvic_context
{
    uint32_t PriorityGroup;
    uint32_t ISER[5];
    uint8_t IPR[160];
    uint8_t SHPR[12];
    uint32_t ICSR;
    uint32_t VTOR;
    uint32_t AIRCR;
    uint32_t SCR;
    uint32_t CCR;
    uint32_t SHCSR;
    uint32_t MMFAR;
    uint32_t BFAR;
    uint32_t CPACR;
    uint32_t NSACR;
} power_nvic_context_t;

typedef struct _power_systick_context
{
    uint32_t CTRL;
    uint32_t LOAD;
} power_systick_context_t;

typedef struct _power_clock_context
{
    uint32_t SOURCE_CLK_GATE;
} power_clock_context_t;

typedef struct _power_gdet_sensor_context
{
    int32_t disableCount;
    uint32_t VSEN_CTRL_1_REG_1;
    uint32_t VSEN_CTRL_2_REG_1;
    uint32_t VSEN_CTRL_3_REG_1;
    uint32_t ITRC_OUT0_SEL0;
    uint32_t ITRC_OUT1_SEL0;
    uint32_t ITRC_OUT0_SEL0_EVENT16_31;
    uint32_t ITRC_OUT1_SEL0_EVENT16_31;
    uint32_t ELS_INT_ENABLE;
    uint32_t ELS_EN;
} power_gdet_sensor_context_t;

typedef struct _power_threshold_params
{
    uint32_t param1;
    uint32_t param2;
    uint32_t margin;
} power_threshold_params_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
static power_nvic_context_t s_nvicContext;
static power_systick_context_t s_systickContext;
static power_clock_context_t s_clockContext;
static power_gdet_sensor_context_t s_gdetSensorContext;
static capt_pulse_timer_callback_t s_captPulseCb;
static void *s_captPulseCbParam;
static power_switch_callback_t s_preSwitch;
static void *s_preSwitchParam;
static power_switch_callback_t s_postSwitch;
static void *s_postSwitchParam;
static const uint8_t s_droTable[19] = {0x40U, 0x43U, 0x46U, 0x48U, 0x4CU, 0x4FU, 0x52U, 0x54U, 0x57U, 0x5AU,
                                       0x5DU, 0x5FU, 0x62U, 0x65U, 0x67U, 0x69U, 0x69U, 0x69U, 0x69U};
static power_load_gdet_cfg s_gdetCfgloadFunc;
static power_gdet_data_t s_gdetCfgData;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
AT_QUICKACCESS_SECTION_CODE(static void POWER_Delay(uint32_t loop))
{
    if (loop > 0U)
    {
        __ASM volatile(
            "1:                             \n"
            "    SUBS   %0, %0, #1          \n"
            "    CMP    %0, #0              \n"
            "    BNE    1b                  \n"
            :
            : "r"(loop));
    }
}

AT_QUICKACCESS_SECTION_CODE(static void POWER_DelayUs(uint32_t us))
{
    uint32_t instNum;

    instNum = ((SystemCoreClock + 999999UL) / 1000000UL) * us;
    POWER_Delay((instNum + 2U) / 3U);
}

static void POWER_GetThresholdParams(uint32_t pack,
                                     power_threshold_params_t *v11,
                                     power_threshold_params_t *v18,
                                     power_threshold_params_t *v33)
{
    assert(v11);
    assert(v18);
    assert(v33);

    v11->param1 = 38307U;
    v11->param2 = 514850000U;
    v11->margin = 750;
    switch (pack)
    {
        case 0: /* QFN */
            v18->param1 = 22034U;
            v18->param2 = 507580000U;
            v18->margin = 500U + 380U;
            v33->param1 = 12924U;
            v33->param2 = 503960000U;
            v33->margin = 1000U + 200U;
            break;
        case 1: /* CSP */
            v18->param1 = 21903U;
            v18->param2 = 514170000U;
            v18->margin = 500U + 305U;
            v33->param1 = 12924U;
            v33->param2 = 503960000U;
            v33->margin = 1000U + 200U;
            break;
        case 2: /* BGA */
            v18->param1 = 22156U;
            v18->param2 = 512500000U;
            v18->margin = 500U + 305U;
            v33->param1 = 12917U;
            v33->param2 = 513300000U;
            v33->margin = 1000U + 200U;
            break;
        default:
            assert(false);
            break;
    }
}

static void POWER_SaveNvicState(void)
{
    uint32_t i;
    uint32_t irqRegs;
    uint32_t irqNum;

    irqRegs = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) + 1U;
    irqNum  = irqRegs * 32U;

    assert(irqRegs <= ARRAY_SIZE(s_nvicContext.ISER));
    assert(irqNum <= ARRAY_SIZE(s_nvicContext.IPR));

    s_nvicContext.PriorityGroup = NVIC_GetPriorityGrouping();

    for (i = 0U; i < irqRegs; i++)
    {
        s_nvicContext.ISER[i] = NVIC->ISER[i];
    }

    for (i = 0U; i < irqNum; i++)
    {
        s_nvicContext.IPR[i] = NVIC->IPR[i];
    }

    /* Save SCB configuration */
    s_nvicContext.ICSR  = SCB->ICSR;
    s_nvicContext.VTOR  = SCB->VTOR;
    s_nvicContext.AIRCR = SCB->AIRCR;
    s_nvicContext.SCR   = SCB->SCR;
    s_nvicContext.CCR   = SCB->CCR;

    s_nvicContext.SHCSR = SCB->SHCSR;
    s_nvicContext.MMFAR = SCB->MMFAR;
    s_nvicContext.BFAR  = SCB->BFAR;
    s_nvicContext.CPACR = SCB->CPACR;
    s_nvicContext.NSACR = SCB->NSACR;

    for (i = 0U; i < ARRAY_SIZE(s_nvicContext.SHPR); i++)
    {
        s_nvicContext.SHPR[i] = SCB->SHPR[i];
    }
}

static void POWER_RestoreNvicState(void)
{
    uint32_t i;
    uint32_t irqRegs;
    uint32_t irqNum;

    irqRegs = (SCnSCB->ICTR & SCnSCB_ICTR_INTLINESNUM_Msk) + 1U;
    irqNum  = irqRegs * 32U;

    NVIC_SetPriorityGrouping(s_nvicContext.PriorityGroup);

    for (i = 0U; i < irqRegs; i++)
    {
        NVIC->ISER[i] = s_nvicContext.ISER[i];
    }

    for (i = 0U; i < irqNum; i++)
    {
        NVIC->IPR[i] = s_nvicContext.IPR[i];
    }

    /* Restore SCB configuration */
    SCB->ICSR  = s_nvicContext.ICSR;
    SCB->VTOR  = s_nvicContext.VTOR;
    SCB->AIRCR = s_nvicContext.AIRCR;
    SCB->SCR   = s_nvicContext.SCR;
    SCB->CCR   = s_nvicContext.CCR;

    SCB->SHCSR = s_nvicContext.SHCSR;
    SCB->MMFAR = s_nvicContext.MMFAR;
    SCB->BFAR  = s_nvicContext.BFAR;
    SCB->CPACR = s_nvicContext.CPACR;
    SCB->NSACR = s_nvicContext.NSACR;

    for (i = 0U; i < ARRAY_SIZE(s_nvicContext.SHPR); i++)
    {
        SCB->SHPR[i] = s_nvicContext.SHPR[i];
    }
}

AT_QUICKACCESS_SECTION_CODE(static void POWER_SaveGdetVSensorConfig(void))
{
    s_gdetSensorContext.VSEN_CTRL_1_REG_1 =
        SENSOR_CTRL->VSEN_CTRL_1_REG_1 & SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK;
    s_gdetSensorContext.VSEN_CTRL_2_REG_1 =
        SENSOR_CTRL->VSEN_CTRL_2_REG_1 & SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK;
    s_gdetSensorContext.VSEN_CTRL_3_REG_1 =
        SENSOR_CTRL->VSEN_CTRL_3_REG_1 & SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK;
    s_gdetSensorContext.ITRC_OUT0_SEL0            = ITRC->OUT0_SEL0;
    s_gdetSensorContext.ITRC_OUT1_SEL0            = ITRC->OUT1_SEL0;
    s_gdetSensorContext.ITRC_OUT0_SEL0_EVENT16_31 = ITRC->OUT0_SEL0_EVENT16_31;
    s_gdetSensorContext.ITRC_OUT1_SEL0_EVENT16_31 = ITRC->OUT1_SEL0_EVENT16_31;
}

AT_QUICKACCESS_SECTION_CODE(static void POWER_RestoreGdetVSensorConfig(void))
{
    SENSOR_CTRL->VSEN_CTRL_1_REG_1 =
        (SENSOR_CTRL->VSEN_CTRL_1_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK) |
        s_gdetSensorContext.VSEN_CTRL_1_REG_1;
    SENSOR_CTRL->VSEN_CTRL_2_REG_1 =
        (SENSOR_CTRL->VSEN_CTRL_2_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK) |
        s_gdetSensorContext.VSEN_CTRL_2_REG_1;
    SENSOR_CTRL->VSEN_CTRL_3_REG_1 =
        (SENSOR_CTRL->VSEN_CTRL_3_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK) |
        s_gdetSensorContext.VSEN_CTRL_3_REG_1;
    ITRC->OUT0_SEL0            = s_gdetSensorContext.ITRC_OUT0_SEL0;
    ITRC->OUT1_SEL0            = s_gdetSensorContext.ITRC_OUT1_SEL0;
    ITRC->OUT0_SEL0_EVENT16_31 = s_gdetSensorContext.ITRC_OUT0_SEL0_EVENT16_31;
    ITRC->OUT1_SEL0_EVENT16_31 = s_gdetSensorContext.ITRC_OUT1_SEL0_EVENT16_31;
}

void CAPT_PULSE_DriverIRQHandler(void);
void CAPT_PULSE_DriverIRQHandler(void)
{
    /* Clear IRQ status */
    PMU->CAPT_PULSE |= PMU_CAPT_PULSE_IRQ_CLR_MASK;
    /* Call user callback */
    if (s_captPulseCb != NULL)
    {
        s_captPulseCb(s_captPulseCbParam);
    }
}

/**
 * @brief   Check if IRQ is the wakeup source
 * @param   irq   : IRQ number
 * @return  true if IRQ is the wakeup source, false otherwise.
 */
bool POWER_GetWakeupStatus(IRQn_Type irq)
{
    uint32_t status;
    uint32_t irqNum = (uint32_t)irq;

    assert((int32_t)irq >= 0);

    if (irq <= HWVAD0_IRQn)
    {
        status = PMU->WAKEUP_PM2_STATUS0 & (1UL << irqNum);
    }
    else if (irq <= POWERQUAD_IRQn)
    {
        status = PMU->WAKEUP_PM2_STATUS1 & (1UL << (irqNum - 32U));
    }
    else if ((irq <= ITRC_IRQn) && (irq >= GAU_GPDAC_INT_FUNC11_IRQn))
    {
        status = PMU->WAKEUP_PM2_STATUS3 & (1UL << (irqNum - 96U));
    }
    else
    {
        status = 0U;
    }

    switch (irq)
    {
        case PIN0_INT_IRQn:
            status = PMU->WAKEUP_STATUS & PMU_WAKEUP_STATUS_PIN0_MASK;
            break;
        case PIN1_INT_IRQn:
            status = PMU->WAKEUP_STATUS & PMU_WAKEUP_STATUS_PIN1_MASK;
            break;
        case RTC_IRQn:
            /* PM2 wakeup status is at WAKEUP_PM2_STATUS1, PM3/PM4 wakeup status is at WAKEUP_STATUS */
            status |= PMU->WAKEUP_STATUS & PMU_WAKEUP_STATUS_RTC_MASK;
            break;
        case CAPT_PULSE_IRQn:
            status = PMU->WAKEUP_STATUS & PMU_WAKEUP_STATUS_CAPT_MASK;
            break;
        case WL_MCI_WAKEUP0_IRQn:
            status = PMU->WAKEUP_STATUS & (1UL << PMU_WAKEUP_STATUS_WL_SHIFT);
            break;
        case WL_MCI_WAKEUP1_IRQn:
            status = PMU->WAKEUP_STATUS & (2UL << PMU_WAKEUP_STATUS_WL_SHIFT);
            break;
        case BLE_MCI_WAKEUP0_IRQn:
            status = PMU->WAKEUP_STATUS & (1UL << PMU_WAKEUP_STATUS_BLE_SHIFT);
            break;
        case BLE_MCI_WAKEUP1_IRQn:
            status = PMU->WAKEUP_STATUS & (2UL << PMU_WAKEUP_STATUS_BLE_SHIFT);
            break;
        default:
            /* Do nothing */
            break;
    }

    return (status != 0U);
}

/**
 * @brief   Clear wakeup status
 * @param   irq   : IRQ number
 */
void POWER_ClearWakeupStatus(IRQn_Type irq)
{
    uint32_t irqNum = (uint32_t)irq;

    assert((int32_t)irq >= 0);

    if (irq <= HWVAD0_IRQn)
    {
        PMU->WAKEUP_PM2_SRC_CLR0 = (1UL << irqNum);
    }
    else if (irq <= POWERQUAD_IRQn)
    {
        PMU->WAKEUP_PM2_SRC_CLR1 = (1UL << (irqNum - 32U));
    }
    else if ((irq <= ITRC_IRQn) && (irq >= GAU_GPDAC_INT_FUNC11_IRQn))
    {
        PMU->WAKEUP_PM2_SRC_CLR3 = (1UL << (irqNum - 96U));
    }
    else
    {
        /* Do nothing */
    }

    switch (irq)
    {
        case PIN0_INT_IRQn:
            PMU->WAKE_SRC_CLR = PMU_WAKE_SRC_CLR_PIN0_CLR_MASK;
            break;
        case PIN1_INT_IRQn:
            PMU->WAKE_SRC_CLR = PMU_WAKE_SRC_CLR_PIN1_CLR_MASK;
            break;
        case RTC_IRQn:
            PMU->WAKE_SRC_CLR = PMU_WAKE_SRC_CLR_RTC_CLR_MASK;
            break;
        case CAPT_PULSE_IRQn:
            PMU->WAKE_SRC_CLR = PMU_WAKE_SRC_CLR_CAPT_CLR_MASK;
            break;
        case WL_MCI_WAKEUP0_IRQn:
            PMU->WAKE_SRC_CLR = (1UL << PMU_WAKE_SRC_CLR_WL_CLR_SHIFT);
            break;
        case WL_MCI_WAKEUP1_IRQn:
            PMU->WAKE_SRC_CLR = (2UL << PMU_WAKE_SRC_CLR_WL_CLR_SHIFT);
            break;
        case BLE_MCI_WAKEUP0_IRQn:
            PMU->WAKE_SRC_CLR = (1UL << PMU_WAKE_SRC_CLR_BLE_CLR_SHIFT);
            break;
        case BLE_MCI_WAKEUP1_IRQn:
            PMU->WAKE_SRC_CLR = (2UL << PMU_WAKE_SRC_CLR_BLE_CLR_SHIFT);
            break;
        default:
            /* Do nothing */
            break;
    }
}

/**
 * @brief   Enable the Wakeup interrupt.
 * @param   irq   : IRQ number
 */
void POWER_EnableWakeup(IRQn_Type irq)
{
    uint32_t irqNum = (uint32_t)irq;

    assert((int32_t)irq >= 0);

    if (irq <= HWVAD0_IRQn)
    {
        PMU->WAKEUP_PM2_MASK0 |= (1UL << irqNum);
    }
    else if (irq <= POWERQUAD_IRQn)
    {
        PMU->WAKEUP_PM2_MASK1 |= (1UL << (irqNum - 32U));
    }
    else if ((irq <= ITRC_IRQn) && (irq >= GAU_GPDAC_INT_FUNC11_IRQn))
    {
        PMU->WAKEUP_PM2_MASK3 |= (1UL << (irqNum - 96U));
    }
    else
    {
        /* Do nothing */
    }

    switch (irq)
    {
        case PIN0_INT_IRQn:
            PMU->WAKEUP_MASK |= PMU_WAKEUP_MASK_PIN0_MASK_MASK;
            break;
        case PIN1_INT_IRQn:
            PMU->WAKEUP_MASK |= PMU_WAKEUP_MASK_PIN1_MASK_MASK;
            break;
        case RTC_IRQn:
            PMU->WAKEUP_MASK |= PMU_WAKEUP_MASK_RTC_MASK_MASK;
            break;
        case CAPT_PULSE_IRQn:
            PMU->WAKEUP_MASK |= PMU_WAKEUP_MASK_CAPT_MASK_MASK;
            break;
        case WL_MCI_WAKEUP0_IRQn:
            PMU->WAKEUP_MASK |= (1UL << PMU_WAKEUP_MASK_WL_MASK_SHIFT);
            break;
        case WL_MCI_WAKEUP1_IRQn:
            PMU->WAKEUP_MASK |= (2UL << PMU_WAKEUP_MASK_WL_MASK_SHIFT);
            break;
        case BLE_MCI_WAKEUP0_IRQn:
            PMU->WAKEUP_MASK |= (1UL << PMU_WAKEUP_MASK_BLE_MASK_SHIFT);
            break;
        case BLE_MCI_WAKEUP1_IRQn:
            PMU->WAKEUP_MASK |= (2UL << PMU_WAKEUP_MASK_BLE_MASK_SHIFT);
            break;
        default:
            /* Do nothing */
            break;
    }
}

/**
 * @brief   Disable the Wakeup interrupts.
 * @param   irq   : IRQ number
 */
void POWER_DisableWakeup(IRQn_Type irq)
{
    uint32_t irqNum = (uint32_t)irq;

    assert((int32_t)irq >= 0);

    if (irq <= HWVAD0_IRQn)
    {
        PMU->WAKEUP_PM2_MASK0 &= ~(1UL << irqNum);
    }
    else if (irq <= POWERQUAD_IRQn)
    {
        PMU->WAKEUP_PM2_MASK1 &= ~(1UL << (irqNum - 32U));
    }
    else if ((irq <= ITRC_IRQn) && (irq >= GAU_GPDAC_INT_FUNC11_IRQn))
    {
        PMU->WAKEUP_PM2_MASK3 &= ~(1UL << (irqNum - 96U));
    }
    else
    {
        /* Do nothing */
    }

    switch (irq)
    {
        case PIN0_INT_IRQn:
            PMU->WAKEUP_MASK &= ~PMU_WAKEUP_MASK_PIN0_MASK_MASK;
            break;
        case PIN1_INT_IRQn:
            PMU->WAKEUP_MASK &= ~PMU_WAKEUP_MASK_PIN1_MASK_MASK;
            break;
        case RTC_IRQn:
            PMU->WAKEUP_MASK &= ~PMU_WAKEUP_MASK_RTC_MASK_MASK;
            break;
        case CAPT_PULSE_IRQn:
            PMU->WAKEUP_MASK &= ~PMU_WAKEUP_MASK_CAPT_MASK_MASK;
            break;
        case WL_MCI_WAKEUP0_IRQn:
            PMU->WAKEUP_MASK &= ~(1UL << PMU_WAKEUP_MASK_WL_MASK_SHIFT);
            break;
        case WL_MCI_WAKEUP1_IRQn:
            PMU->WAKEUP_MASK &= ~(2UL << PMU_WAKEUP_MASK_WL_MASK_SHIFT);
            break;
        case BLE_MCI_WAKEUP0_IRQn:
            PMU->WAKEUP_MASK &= ~(1UL << PMU_WAKEUP_MASK_BLE_MASK_SHIFT);
            break;
        case BLE_MCI_WAKEUP1_IRQn:
            PMU->WAKEUP_MASK &= ~(2UL << PMU_WAKEUP_MASK_BLE_MASK_SHIFT);
            break;
        default:
            /* Do nothing */
            break;
    }
}

/**
 * @brief   Set sleep mode on idle.
 * @param   mode : 0 ~ 4 stands for PM0 ~ PM4.
 */
void POWER_SetSleepMode(uint32_t mode)
{
    assert(mode <= 4U);

    if (mode == 0U)
    {
        mode = 1U; /* PM0/PM1 is same */
    }
    /* set PMU basic mode */
    PMU->PWR_MODE = PMU_PWR_MODE_PWR_MODE(mode - 1U);

    /* select deepsleep or not */
    if (mode == 1U)
    {
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    }
    else
    {
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    }
}

AT_QUICKACCESS_SECTION_CODE(static void Power_ConfigClkGate(const power_sleep_config_t *config))
{
    uint32_t pm2AnaPdCfg = (config->pm2AnaPuCfg ^ (uint32_t)kPOWER_Pm2AnaPuAll) & (uint32_t)kPOWER_Pm2AnaPuAll;
    uint32_t clkGate     = config->clkGate & (uint32_t)kPOWER_ClkGateAll;

    /* If ENET clock is enabled, TDDR power must be on. */
    if ((clkGate & SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_MASK) == 0U)
    {
        pm2AnaPdCfg &= ~SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2_MASK;
    }

    SYSCTL2->SOURCE_CLK_GATE = (SYSCTL2->SOURCE_CLK_GATE & (~((uint32_t)kPOWER_ClkGateAll))) | clkGate;
    SYSCTL2->ANA_PDWN_PM2    = pm2AnaPdCfg;
}

AT_QUICKACCESS_SECTION_CODE(static void deinitXip(void))
{
    if (IS_XIP_FLEXSPI())
    { /* FlexSPI */
        /* Wait until FLEXSPI is not busy */
        while (!(((FLEXSPI->STS0 & FLEXSPI_STS0_ARBIDLE_MASK) != 0U) &&
                 ((FLEXSPI->STS0 & FLEXSPI_STS0_SEQIDLE_MASK) != 0U)))
        {
        }
        /* Disable module. */
        FLEXSPI->MCR0 |= FLEXSPI_MCR0_MDIS_MASK;
        /* Disable clock. */
        CLKCTL0->PSCCTL0_CLR = CLKCTL0_PSCCTL0_CLR_FLEXSPI0_MASK;
    }
}

AT_QUICKACCESS_SECTION_CODE(static void initFlexSPI(FLEXSPI_Type *base))
{
    uint32_t status;
    uint32_t lastStatus;
    uint32_t retry;
    uint32_t mask = 0;

    /* Enable FLEXSPI module */
    base->MCR0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    base->MCR0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while ((base->MCR0 & FLEXSPI_MCR0_SWRESET_MASK) != 0U)
    {
    }

    /* Need to wait DLL locked if DLL enabled */
    if (0U != (base->DLLCR[0] & FLEXSPI_DLLCR_DLLEN_MASK))
    {
        lastStatus = base->STS2;
        retry      = FLEXSPI_DLL_LOCK_RETRY;
        /* Flash on port A */
        if (((base->FLSHCR0[0] & FLEXSPI_FLSHCR0_FLSHSZ_MASK) > 0U) ||
            ((base->FLSHCR0[1] & FLEXSPI_FLSHCR0_FLSHSZ_MASK) > 0U))
        {
            mask |= FLEXSPI_STS2_AREFLOCK_MASK | FLEXSPI_STS2_ASLVLOCK_MASK;
        }
        /* Flash on port B */
        if (((base->FLSHCR0[2] & FLEXSPI_FLSHCR0_FLSHSZ_MASK) > 0U) ||
            ((base->FLSHCR0[3] & FLEXSPI_FLSHCR0_FLSHSZ_MASK) > 0U))
        {
            mask |= FLEXSPI_STS2_BREFLOCK_MASK | FLEXSPI_STS2_BSLVLOCK_MASK;
        }
        /* Wait slave delay line locked and slave reference delay line locked. */
        do
        {
            status = base->STS2;
            if ((status & mask) == mask)
            {
                /* Locked */
                retry = 100;
                break;
            }
            else if (status == lastStatus)
            {
                /* Same delay cell number in calibration */
                retry--;
            }
            else
            {
                retry      = FLEXSPI_DLL_LOCK_RETRY;
                lastStatus = status;
            }
        } while (retry > 0U);
        /* According to ERR011377, need to delay at least 100 NOPs to ensure the DLL is locked. */
        for (; retry > 0U; retry--)
        {
            __NOP();
        }
    }
}

AT_QUICKACCESS_SECTION_CODE(static void initXip(void))
{
    if (IS_XIP_FLEXSPI())
    { /* FlexSPI */
        /* Enable FLEXSPI clock again */
        CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_SET_FLEXSPI0_MASK;
        /* Re-enable FLEXSPI module */
        initFlexSPI(FLEXSPI);
    }
}

void POWER_ConfigCauInSleep(bool pdCau)
{
    if (pdCau) /* xtal / cau full pd */
    {
        CAU->PD_CTRL_ONE_REG |= 0x4U;
        CAU->SLP_CTRL_ONE_REG = 0xCU;
    }
    else
    {
        CAU->PD_CTRL_ONE_REG &= 0xFBU;
        CAU->SLP_CTRL_ONE_REG = 0x9EU;
        CAU->SLP_CTRL_TWO_REG = 0x6AU;
    }
}

/* Prepare to go to low power
 *  Change clock source to RC32M
 *   Switch off PLLs, XTAL
 *  Set Deep sleep bit in SRC register
 *  Initiate state change
 */
AT_QUICKACCESS_SECTION_CODE(static void POWER_PrePowerMode(uint32_t mode, const power_sleep_config_t *config))
{
    uint32_t wlanPowerStatus, blePowerStatus;

    assert((mode >= 1U) && (mode <= 4U));
    /* Turn off Systick to avoid interrupt
     *  when entering low power state
     */
    s_systickContext.CTRL = SysTick->CTRL;
    s_systickContext.LOAD = SysTick->LOAD;
    SysTick->CTRL         = 0;
    SysTick->LOAD         = 0;

    POWER_SetSleepMode(mode);

    if ((mode == 2U) || (mode == 3U) || (mode == 4U))
    {
        /* To enter PM2/PM3/PM4, GDET sensor must be disabled */
        assert(s_gdetSensorContext.disableCount > 0);
    }

    s_clockContext.SOURCE_CLK_GATE = SYSCTL2->SOURCE_CLK_GATE;

    if (mode == 2U)
    {
        /* Deinit FlexSPI in case XIP */
        deinitXip();
        /* Keep all modules power on in SW controlled CFG */
        SYSCTL2->MEM_PD_CFG = 0U;
        /* Enable SW control for modules need be powered on, the others are powered down by HW */
        SYSCTL2->MEM_PD_CTRL = config->pm2MemPuCfg & (uint32_t)kPOWER_Pm2MemPuAll;
        Power_ConfigClkGate(config);
    }
    else if (mode >= 3U)
    {
        /* Turn off the short switch between C18/C11 and V18/V11.
           In sleep mode, V11 drops to 0.8V */
        BUCK18->BUCK_CTRL_TWENTY_REG = 0x75U;
        if (mode == 3U)
        {
            POWER_SaveNvicState();

            PMU->MEM_CFG = (PMU->MEM_CFG & ~PMU_MEM_CFG_MEM_RET_MASK) | (config->memPdCfg & PMU_MEM_CFG_MEM_RET_MASK);
            PMU->PMIP_BUCK_CTRL = (PMU->PMIP_BUCK_CTRL & ~((uint32_t)kPOWER_Pm3BuckAll)) |
                                  (config->pm3BuckCfg & (uint32_t)kPOWER_Pm3BuckAll);
            /* Clear reset status */
            PMU->SYS_RST_CLR = 0x7FU;
        }
        else if (mode == 4U)
        {
            wlanPowerStatus = POWER_WLAN_POWER_STATUS();
            blePowerStatus  = POWER_BLE_POWER_STATUS();

            PMU->MEM_CFG =
                (PMU->MEM_CFG & ~PMU_MEM_CFG_AON_MEM_RET_MASK) | (config->memPdCfg & PMU_MEM_CFG_AON_MEM_RET_MASK);
            if ((wlanPowerStatus == POWER_WLAN_BLE_POWER_OFF) && (blePowerStatus == POWER_WLAN_BLE_POWER_OFF))
            {
                /* pm422, LDO 0.8V, 1.8V */
                PMU->PMIP_LDO_LVL = PMU_PMIP_LDO_LVL_LDO18_SEL(4) | PMU_PMIP_LDO_LVL_LDO11_SEL(1);
            }
            /* Clear reset status */
            PMU->SYS_RST_CLR = 0x7FU;
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        /* PM1: Do nothing */
    }

    if (s_preSwitch != NULL)
    {
        s_preSwitch(mode, s_preSwitchParam);
    }
}

AT_QUICKACCESS_SECTION_CODE(static bool POWER_PostPowerMode(uint32_t mode))
{
    assert((mode >= 1U) && (mode <= 4U));

    if (s_postSwitch != NULL)
    {
        s_postSwitch(mode, s_postSwitchParam);
    }

    POWER_SetSleepMode(1U);

    SYSCTL2->SOURCE_CLK_GATE = s_clockContext.SOURCE_CLK_GATE;

    if (mode == 2U)
    {
        initXip();
    }
    else if (mode == 3U)
    {
        SystemInit();
        POWER_RestoreNvicState();
        initXip();
    }
    else
    {
        /* PM1: Do nothing */
    }

    if ((mode == 3U) && (PMU->PWR_MODE_STATUS == 2U))
    {
        /* Successfully resumed from PM3, GDET is enabled by ROM. */
        assert(s_gdetSensorContext.disableCount > 0);
        s_gdetSensorContext.disableCount--;
    }

    SysTick->CTRL = s_systickContext.CTRL;
    SysTick->LOAD = s_systickContext.LOAD;

    return (mode == 1U) || (PMU->PWR_MODE_STATUS == (mode - 1U)); /* PM1 doesn't update PWR_MODE_STATUS */
}

AT_QUICKACCESS_SECTION_CODE(static void POWER_EnterPm3Asm(void))
{
    uint32_t clk    = CLKCTL0->PSCCTL0;
    uint32_t rst    = RSTCTL0->PRSTCTL0;
    uint32_t rtcClk = CLKCTL1->PSCCTL2 & CLKCTL1_PSCCTL2_RTC_LITE_MASK;

    /* Enable AON MEM clock/reset. */
    CLKCTL0->PSCCTL0_SET  = CLKCTL0_PSCCTL0_SET_AON_MEM_MASK;
    RSTCTL0->PRSTCTL0_CLR = RSTCTL0_PRSTCTL0_CLR_AON_MEM_MASK;

    /* Address: 0x4015C000 is the address in NVRAM which holds address
     * where control returns after exit from PM3.
     * All general purpose registers and special registers
     * are saved by pushing them on current thread's stack
     * and finally SP is saved in NVRAM address 0x4015C004. */
    __ASM volatile(
        "push {r0-r12, lr}\n"
        "mrs r1, basepri\n"
        "push {r1}\n"
        "mrs r1, primask\n"
        "push {r1}\n"
        "mrs r1, faultmask\n"
        "push {r1}\n"
        "mrs r1, control\n"
        "bic r2, r1, #2\n"
        "msr control, r2\n" /* Use MSP */
        "push {r1}\n"       /* CONTROL */
        "mrs r1, psp\n"
        "push {r1}\n" /* PSP */
        "mrs r1, psplim\n"
        "push {r1}\n" /* PSPLIM */
        "mrs r1, msplim\n"
        "push {r1}\n" /* MSPLIM */
        "ldr r0, =0x4015C004\n"
        "str sp, [r0]\n" /* MSP */
        "ldr r0, =0x4015C000\n"
        "mov r1, pc\n"
        "add r1, r1 , #21\n"
        "str r1, [r0]\n");
    /*
     * Execute WFI to generate a state change
     * and system is in an unresponsive state
     * press wakeup key to get it out of standby
     * If time_to_standby is set to valid value
     * RTC is programmed and RTC generates
     * a wakeup signal.
     */
    __DSB();
    __WFI();
    __ISB();

    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();

    /* When system exits PM3 all registers need to be
     * restored as they are lost. */

    /*
     * When MCU enters PM3 all Core registers
     * r0-r12
     * lr
     * basepri
     * primask
     * faultmask
     * control
     * psp
     * psplim
     * msplim
     * are lost (ZERO) as MCU power is tuned off
     * On wakeup from PM3, this piece of code restores
     * these registers which were saved before entry.
     * The location of saving this register was on stack
     */
    __ASM volatile(
        "ldr r0, =0x4015C004\n"
        "ldr sp, [r0]\n"
        "pop {r4}\n"   /* MSPLIM */
        "pop {r5}\n"   /* PSPLIM */
        "pop {r1}\n"   /* PSP */
        "pop {r2}\n"   /* CONTROL */
        "mov r3, sp\n" /* MSP */
        "msr msplim, r4\n"
        "msr psplim, r5\n"
        "msr msp, r3\n"
        "msr psp, r1\n"
        "msr control, r2\n"
        "pop {r1}\n"
        "msr faultmask, r1\n"
        "pop {r1}\n"
        "msr primask, r1\n"
        "pop {r1}\n"
        "msr basepri, r1\n"
        "pop {r0-r12, lr}\n");
    /* Restore AON MEM clock/reset */
    CLKCTL0->PSCCTL0     = clk;
    RSTCTL0->PRSTCTL0    = rst;
    CLKCTL1->PSCCTL2_SET = rtcClk;
}

static void POWER_InitVSensorThreshold(uint8_t volt11, uint32_t pack)
{
    uint32_t val;
    uint32_t svcMv;
    power_threshold_params_t v11 = {0};
    power_threshold_params_t v18 = {0};
    power_threshold_params_t v33 = {0};

    POWER_GetThresholdParams(pack, &v11, &v18, &v33);

    /* Disable V11 sensor */
    val                            = SENSOR_CTRL->VSEN_CTRL_1_REG_1;
    SENSOR_CTRL->VSEN_CTRL_1_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK;
    /* Configure threshold */
    svcMv = (uint32_t)(volt11)*5U + 630U;
    val   = val & ~(SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR_MASK |
                  SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR_MASK);
    val |= SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MAX_VOLTAGE_THR(
        (v11.param1 * (svcMv * 10U + v11.margin) + v11.param2 + 999999U) / 1000000U);
    val |= SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_MIN_VOLTAGE_THR((v11.param1 * (svcMv * 10U - v11.margin) + v11.param2) /
                                                              1000000U);
    SENSOR_CTRL->VSEN_CTRL_1_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK;
    /* Restore V11 sensor */
    SENSOR_CTRL->VSEN_CTRL_1_REG_1 = val;

    /* Disable V18 sensor */
    val                            = SENSOR_CTRL->VSEN_CTRL_2_REG_1;
    SENSOR_CTRL->VSEN_CTRL_2_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK;
    /* Configure threshold */
    val = val & ~(SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR_MASK |
                  SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR_MASK);
    val |= SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MAX_VOLTAGE_THR(
        (v18.param1 * (1890U * 10U + v18.margin) + v18.param2 + 999999U) / 1000000U);
    val |= SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_MIN_VOLTAGE_THR((v18.param1 * (1710U * 10U - v18.margin) + v18.param2) /
                                                              1000000U);
    SENSOR_CTRL->VSEN_CTRL_2_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK;
    /* Restore V18 sensor */
    SENSOR_CTRL->VSEN_CTRL_2_REG_1 = val;

    /* Disable V33 sensor */
    val                            = SENSOR_CTRL->VSEN_CTRL_3_REG_1;
    SENSOR_CTRL->VSEN_CTRL_3_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK;
    /* Configure threshold */
    val = val & ~(SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR_MASK |
                  SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR_MASK);
    val |= SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MAX_VOLTAGE_THR(
        (v33.param1 * (3630U * 10U + v33.margin) + v33.param2 + 999999U) / 1000000U);
    val |= SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_MIN_VOLTAGE_THR((v33.param1 * (1850U * 10U - v33.margin) + v33.param2) /
                                                              1000000U);
    SENSOR_CTRL->VSEN_CTRL_3_REG_1 = val & ~SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK;
    /* Restore V33 sensor */
    SENSOR_CTRL->VSEN_CTRL_3_REG_1 = val;
}

void POWER_GetCurrentSleepConfig(power_sleep_config_t *config)
{
    assert(config != NULL);

    config->pm2MemPuCfg = (~SYSCTL2->MEM_PD_CFG) & (SYSCTL2->MEM_PD_CTRL);
    config->pm2AnaPuCfg = (~SYSCTL2->ANA_PDWN_PM2) & (uint32_t)kPOWER_Pm2AnaPuAll;
    config->clkGate     = SYSCTL2->SOURCE_CLK_GATE;
    config->memPdCfg    = PMU->MEM_CFG;
    config->pm3BuckCfg  = PMU->PMIP_BUCK_CTRL & (uint32_t)kPOWER_Pm3BuckAll;
}

void POWER_InitPowerConfig(const power_init_config_t *config)
{
    uint32_t reg;
    bool iBuck, gateCauRefClk;

    assert(config != NULL);

    iBuck         = config->iBuck;
    gateCauRefClk = config->gateCauRefClk;

    BUCK11->BUCK_CTRL_THREE_REG  = 0x10U;
    BUCK18->BUCK_CTRL_THREE_REG  = 0x10U;
    BUCK18->BUCK_CTRL_TWENTY_REG = 0x55U;

    SYSCTL0->AUTOCLKGATEOVERRIDE0 = 0U;
    /* Enable RAM dynamic clk gate */
    SYSCTL0->AUTOCLKGATEOVERRIDE1 = 0U;
    /* Enable ROM dynamic clk gate */
    SYSCTL2->ROM_DYN_CLK_EN = SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN_MASK;

    PMU->PMIP_LDO_LVL = 0U;
    if (iBuck)
    {
        /* No timeout with internal supply. */
        PMU->TIME_OUT_CTRL = PMU_TIME_OUT_CTRL_V11_RDY_NO_TMT_MASK | PMU_TIME_OUT_CTRL_V18_RDY_NO_TMT_MASK |
                             PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_MASK;
    }
    else
    {
        /* Use timeout mode with external supply for VCORE and AVDD18. */
        PMU->TIME_OUT_CTRL      = PMU_TIME_OUT_CTRL_PSW_MCI_RDY_NO_TMT_MASK;
        PMU->TIME_OUT_CFG_VALUE = 0x3FFFFFFFU;
    }

    PMU->SOC_MEM_PDWN &= ~(PMU_SOC_MEM_PDWN_MSC_MEM_PDWN_CTRL_MASK | PMU_SOC_MEM_PDWN_SOCTOP_OTP_PDWN_CTRL_MASK);
    PMU->CAU_SLP_CTRL = gateCauRefClk ? PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_MASK : 0U;

    /* Open usb clock and release reset */
    reg                   = CLKCTL0->PSCCTL0;
    CLKCTL0->PSCCTL0_SET  = CLKCTL0_PSCCTL0_SET_USB_MASK;
    RSTCTL0->PRSTCTL0_CLR = RSTCTL0_PRSTCTL0_CLR_USB_MASK;
    POWER_DelayUs(1U);
    /* Restore usb clk. */
    CLKCTL0->PSCCTL0 = reg;

    /* Disable G2BIST CLK */
    CLKCTL0->G2BIST_CLK_EN = 0U;
}

void POWER_SetPowerSwitchCallback(power_switch_callback_t pre,
                                  void *preParam,
                                  power_switch_callback_t post,
                                  void *postParam)
{
    s_preSwitch       = pre;
    s_preSwitchParam  = preParam;
    s_postSwitch      = post;
    s_postSwitchParam = postParam;
}

bool POWER_EnterPowerMode(uint32_t mode, const power_sleep_config_t *config)
{
    uint32_t primask;
    bool ret = true;

    assert(mode <= 4U);

    /* Needed to make POWER_DelayUs() accurate. */
    SystemCoreClockUpdate();

    if (mode >= 1U)
    {
        primask = DisableGlobalIRQ();
        POWER_PrePowerMode(mode, config);
        if (mode == 3U)
        {
            POWER_EnterPm3Asm();
        }
        else
        {
            __DSB();
            __WFI();
            __ISB();
        }
        ret = POWER_PostPowerMode(mode);
        EnableGlobalIRQ(primask);
    }

    return ret;
}

void POWER_PowerOnWlan(void)
{
    if (POWER_WLAN_POWER_STATUS() == POWER_WLAN_BLE_POWER_OFF)
    {
        /* Enable SW control */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_WL_EN_MASK;
        /* WLan request buck on, then need wait 5 fast clk_pmu cycles, do psw on, then iso release */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_WL_BUCK_ON_REQ_MASK;
        /* Wait buck on */
        POWER_WAIT_PMU();

        PMU->SW_CTRL_WL &= ~PMU_SW_CTRL_WL_PSW_WL_PD_MASK;
        /* Wait PSW ready */
        SystemCoreClockUpdate();
        POWER_DelayUs(50U);
        /* Disable ISO */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_MCI_ISO_WL_N_MASK;
        /* Wait about 125us */
        POWER_DelayUs(125U);
        /* Release WLan */
        PMU->SW_CTRL_WL &= ~PMU_SW_CTRL_WL_MCI_WL_PU_RST_MASK;
    }
}

void POWER_PowerOffWlan(void)
{
    if (POWER_WLAN_POWER_STATUS() != POWER_WLAN_BLE_POWER_OFF)
    {
        /* Enable SW control */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_WL_EN_MASK;
        /* Enable ISO before PSW off */
        PMU->SW_CTRL_WL &= ~PMU_SW_CTRL_WL_MCI_ISO_WL_N_MASK;
        POWER_WAIT_PMU();
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_PSW_WL_PD_MASK;
        /* Wait PSW off */
        while ((SOCCTRL->PSW_VD2_RDY0 & (1UL << 1)) == 0U)
        {
        }
        /* Reset WLan */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_MCI_WL_PU_RST_MASK;
        /* Request buck off */
        PMU->SW_CTRL_WL |= PMU_SW_CTRL_WL_WL_BUCK_OFF_REQ_MASK;
    }
}

void POWER_PowerOnBle(void)
{
    if (POWER_BLE_POWER_STATUS() == POWER_WLAN_BLE_POWER_OFF)
    {
        /* Enable SW control */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_BLE_EN_MASK;
        /* BLE request buck on, then need wait 5 fast clk_pmu cycles(about 96ns),do psw on, then iso release */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_BLE_BUCK_ON_REQ_MASK;
        /* Wait buck on */
        POWER_WAIT_PMU();
        PMU->SW_CTRL_BLE &= ~PMU_SW_CTRL_BLE_PSW_BLE_PD_MASK;
        /* Wait PSW ready */
        SystemCoreClockUpdate();
        POWER_DelayUs(50U);
        /* Disable ISO */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_MASK;
        /* Wait about 125us */
        POWER_DelayUs(125U);
        /* Release BLE */
        PMU->SW_CTRL_BLE &= ~PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_MASK;
    }
}

void POWER_PowerOffBle(void)
{
    if (POWER_BLE_POWER_STATUS() != POWER_WLAN_BLE_POWER_OFF)
    {
        /* Enable SW control */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_BLE_EN_MASK;
        /* Enable ISO before PSW off */
        PMU->SW_CTRL_BLE &= ~PMU_SW_CTRL_BLE_MCI_ISO_BLE_N_MASK;
        POWER_WAIT_PMU();
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_PSW_BLE_PD_MASK;
        /* Wait PSW off */
        while ((SOCCTRL->PSW_VD2_RDY0 & (1UL << 9)) == 0U)
        {
        }
        /* Reset BLE */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_MCI_BLE_PU_RST_MASK;
        /* Request buck off */
        PMU->SW_CTRL_BLE |= PMU_SW_CTRL_BLE_BLE_BUCK_OFF_REQ_MASK;
    }
}

void POWER_PowerOnGau(void)
{
    GAU_BG->CTRL &= ~BG_CTRL_PD_MASK;
    while ((GAU_BG->STATUS & BG_STATUS_RDY_MASK) == 0U)
    {
    }
}

void POWER_PowerOffGau(void)
{
    GAU_BG->CTRL |= BG_CTRL_PD_MASK;
}

void POWER_EnableCaptSlowPulseTimer(capt_slow_pulse_width_t width,
                                    capt_slow_pulse_edge_t edge,
                                    uint32_t timeout,
                                    capt_pulse_timer_callback_t cb,
                                    void *param)
{
    s_captPulseCb            = cb;
    s_captPulseCbParam       = param;
    PMU->CAPT_PULSE          = PMU_CAPT_PULSE_IRQ_CLR_MASK | PMU_CAPT_PULSE_IRQ_MSK_MASK;
    PMU->CAPT_PULSE_BASE_VAL = timeout;
    PMU->CAPT_PULSE          = PMU_CAPT_PULSE_IC_WIDTH_CLK_CNT(width) | PMU_CAPT_PULSE_IC_EDGE_CLK_CNT(edge);
    PMU->CAPT_PULSE |= PMU_CAPT_PULSE_CAPTURE_SLOW_PULSE_CNT_EN_MASK;
}

void POWER_EnableCaptFastPulseTimer(uint32_t timeout, capt_pulse_timer_callback_t cb, void *param)
{
    s_captPulseCb            = cb;
    s_captPulseCbParam       = param;
    PMU->CAPT_PULSE          = PMU_CAPT_PULSE_IRQ_CLR_MASK | PMU_CAPT_PULSE_IRQ_MSK_MASK;
    PMU->CAPT_PULSE_BASE_VAL = timeout;
    PMU->CAPT_PULSE          = PMU_CAPT_PULSE_CLK_SEL_MASK;
    PMU->CAPT_PULSE |= PMU_CAPT_PULSE_CAPTURE_FAST_PULSE_CNT_EN_MASK;
}

void POWER_DisableCaptPulseTimer(void)
{
    PMU->CAPT_PULSE = PMU_CAPT_PULSE_IRQ_CLR_MASK | PMU_CAPT_PULSE_IRQ_MSK_MASK;
}

void Power_InitLoadGdetCfg(power_load_gdet_cfg loadFunc, const power_gdet_data_t *data, uint32_t pack)
{
    assert(loadFunc != NULL);
    assert(data != NULL);
    assert(pack <= 2U);

    s_gdetCfgloadFunc = loadFunc;
    (void)memcpy(&s_gdetCfgData, data, sizeof(power_gdet_data_t));
    s_gdetCfgData.CFG[3] = POWER_TrimSvc(data->CFG[3], pack);
}

/* Configure voltage threshold */
void POWER_InitVoltage(uint32_t dro, uint32_t pack)
{
    int32_t i;
    uint8_t val;

    SystemCoreClockUpdate();

    /* LPBG trim */
    BUCK11->BUCK_CTRL_EIGHTEEN_REG = 0x6U;

    if (dro == 0U)
    { /* Boot voltage 1.11V */
        val = 0x60U;
    }
    else
    {
        /* Change buck level */
        dro /= 1000U;
        i = 36 - (int32_t)dro;
        assert((i >= 0) && ((uint32_t)i < ARRAY_SIZE(s_droTable)));
        val = s_droTable[i];
    }
    PMU->PMIP_BUCK_LVL = PMU_PMIP_BUCK_LVL_SLEEP_BUCK18_SEL(0x60U) |  /* 1.8V */
                         PMU_PMIP_BUCK_LVL_SLEEP_BUCK11_SEL(0x22U) |  /* 0.8V */
                         PMU_PMIP_BUCK_LVL_NORMAL_BUCK18_SEL(0x60U) | /* 1.8V */
                         PMU_PMIP_BUCK_LVL_NORMAL_BUCK11_SEL(val);
    /* Delay 600us */
    POWER_DelayUs(600U);

    POWER_InitVSensorThreshold(val, pack);
}

void POWER_DisableGDetVSensors(void)
{
    uint32_t val;
    uint32_t pscctl0, pscctl1, pscctl2;
    uint32_t rstctl0, rstctl1;

    if (s_gdetSensorContext.disableCount == 0)
    {
        pscctl0 = CLKCTL0->PSCCTL0;
        pscctl1 = CLKCTL0->PSCCTL1;
        pscctl2 = CLKCTL0->PSCCTL2;
        rstctl0 = RSTCTL0->PRSTCTL0;
        rstctl1 = RSTCTL0->PRSTCTL1;

        /* Enable ELS/ITRC clock */
        CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_ELS_MASK;
        CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_ELS_APB_MASK;
        CLKCTL0->PSCCTL2_SET = CLKCTL0_PSCCTL2_ITRC_MASK;
        /* Clear ELS reset */
        RSTCTL0->PRSTCTL0_CLR = RSTCTL0_PRSTCTL0_ELS_MASK;
        RSTCTL0->PRSTCTL1_CLR = RSTCTL0_PRSTCTL1_ELS_APB_MASK;

        s_gdetSensorContext.ELS_EN = ELS->ELS_CTRL & ELS_ELS_CTRL_ELS_EN_MASK;

        /* Save GDET VSEN config */
        POWER_SaveGdetVSensorConfig();
        if (s_gdetSensorContext.ELS_EN != 0U)
        {
            s_gdetSensorContext.ELS_INT_ENABLE = ELS->ELS_INT_ENABLE;
        }

        /* Disable ITRC interrupts, especially security sensors */
        /* Disable V11 sensor */
        val = SENSOR_CTRL->VSEN_CTRL_1_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_1_REG_1_VSEN_SW_ENABLE_MASK;
        SENSOR_CTRL->VSEN_CTRL_1_REG_1 = val;

        /* Disable V18 sensor */
        val = SENSOR_CTRL->VSEN_CTRL_2_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_2_REG_1_VSEN_SW_ENABLE_MASK;
        SENSOR_CTRL->VSEN_CTRL_2_REG_1 = val;

        /* Disable V33 sensor */
        val = SENSOR_CTRL->VSEN_CTRL_3_REG_1 & ~SENSOR_CTRL_VSEN_CTRL_3_REG_1_VSEN_SW_ENABLE_MASK;
        SENSOR_CTRL->VSEN_CTRL_3_REG_1 = val;

        if (s_gdetSensorContext.ELS_EN != 0U)
        {
            /* Disable CSS_INT_ENABLE[GDET_INT_EN] bit field */
            ELS->ELS_INT_ENABLE &= ~ELS_ELS_INT_ENABLE_GDET_INT_EN_MASK;
        }

        /* Disable aGDET/LVD/HVD interrupts in ITRC */
        val             = ITRC->OUT0_SEL0 & ~ITRC_OUT_SEL_MASK;
        ITRC->OUT0_SEL0 = val | ITRC_OUT_SEL_DISABLE;
        val             = ITRC->OUT1_SEL0 & ~ITRC_OUT_SEL_MASK;
        ITRC->OUT1_SEL0 = val | ITRC_OUT_SEL_DISABLE;

        val                        = ITRC->OUT0_SEL0_EVENT16_31 & ~ITRC_OUT_SEL_EVENT_MASK;
        ITRC->OUT0_SEL0_EVENT16_31 = val | ITRC_OUT_SEL_EVENT_DISABLE;
        val                        = ITRC->OUT1_SEL0_EVENT16_31 & ~ITRC_OUT_SEL_EVENT_MASK;
        ITRC->OUT1_SEL0_EVENT16_31 = val | ITRC_OUT_SEL_EVENT_DISABLE;

        /* Restore ELS/ITRC clock */
        CLKCTL0->PSCCTL0 = pscctl0;
        CLKCTL0->PSCCTL1 = pscctl1;
        CLKCTL0->PSCCTL2 = pscctl2;
        /* Restore ELS reset */
        RSTCTL0->PRSTCTL0 = rstctl0;
        RSTCTL0->PRSTCTL1 = rstctl1;
    }

    s_gdetSensorContext.disableCount++;
}

bool POWER_EnableGDetVSensors(void)
{
    uint32_t pscctl0, pscctl1, pscctl2;
    uint32_t rstctl0, rstctl1;
    bool retval = true;

    s_gdetSensorContext.disableCount--;

    if (s_gdetSensorContext.disableCount == 0)
    {
        pscctl0 = CLKCTL0->PSCCTL0;
        pscctl1 = CLKCTL0->PSCCTL1;
        pscctl2 = CLKCTL0->PSCCTL2;
        rstctl0 = RSTCTL0->PRSTCTL0;
        rstctl1 = RSTCTL0->PRSTCTL1;

        /* Enable ELS/ITRC clock */
        CLKCTL0->PSCCTL0_SET = CLKCTL0_PSCCTL0_ELS_MASK;
        CLKCTL0->PSCCTL1_SET = CLKCTL0_PSCCTL1_ELS_APB_MASK;
        CLKCTL0->PSCCTL2_SET = CLKCTL0_PSCCTL2_ITRC_MASK;
        /* Clear ELS reset */
        RSTCTL0->PRSTCTL0_CLR = RSTCTL0_PRSTCTL0_ELS_MASK;
        RSTCTL0->PRSTCTL1_CLR = RSTCTL0_PRSTCTL1_ELS_APB_MASK;

        /* Only in the normal flow that GDET_INT_EN is disabled, we will restore GDET config here.
         * An exception is that we call POWER_DisableGDetVSensors() before PM3 and then wake up
         * from ROM. In that case, ELS_INT_ENABLE will be set by ROM again and we shouldn't config
         * GDET here. */
        if (((ELS->ELS_CTRL & ELS_ELS_CTRL_ELS_EN_MASK) != 0U) &&
            (ELS->ELS_INT_ENABLE & ELS_ELS_INT_ENABLE_GDET_INT_EN_MASK) == 0U)
        {
            POWER_RestoreGdetVSensorConfig();
            if ((s_gdetSensorContext.ELS_EN != 0U) && (s_gdetSensorContext.ELS_INT_ENABLE != 0U))
            {
                /* To enable GDET, load function must be configured. */
                if (s_gdetCfgloadFunc == NULL)
                {
                    retval = false;
                }
                else
                {
                    /* GDET config must be loaded correctly for GDET working. */
                    retval = s_gdetCfgloadFunc(&s_gdetCfgData);
                }

                /* Clear GDET errors */
                ELS->ELS_INT_STATUS_CLR =
                    ELS_ELS_INT_STATUS_CLR_INT_CLR_MASK | ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR_MASK;
                ELS->ELS_ERR_STATUS_CLR  = ELS_ELS_ERR_STATUS_CLR_ERR_CLR_MASK;
                ELS->ELS_GDET_EVTCNT_CLR = ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_MASK;
                /* Clear Sensor errors */
                SENSOR_CTRL->SEN_CLR_REG = 0x3CU;
                /* Clear ITRC status */
                ITRC->STATUS0 = ITRC->STATUS0;
                ITRC->STATUS1 = ITRC->STATUS1;
                if (retval)
                {
                    ELS->ELS_INT_ENABLE = s_gdetSensorContext.ELS_INT_ENABLE;
                }
            }
        }

        /* Restore ELS/ITRC clock */
        CLKCTL0->PSCCTL0 = pscctl0;
        CLKCTL0->PSCCTL1 = pscctl1;
        CLKCTL0->PSCCTL2 = pscctl2;
        /* Restore ELS reset */
        RSTCTL0->PRSTCTL0 = rstctl0;
        RSTCTL0->PRSTCTL1 = rstctl1;
    }

    return retval;
}

uint32_t POWER_TrimSvc(uint32_t gdetTrim, uint32_t pack)
{
    int32_t x;
    int32_t y1, y3;
    uint32_t trimSvc = gdetTrim;
    uint32_t clk;
    uint32_t rst;
    uint32_t revision = SOCCTRL->CHIP_INFO & SOCCIU_CHIP_INFO_REV_NUM_MASK;

    if (revision == 2U)
    {
        /* A2 */
        /* Autotrim value at [7:0] */
        x = (int32_t)(uint32_t)(gdetTrim & 0xFFUL);
        if (pack == 0U)
        {
            /* QFN */
            y1 = (18 * x * x) + (801 * x) + 437290;
            y3 = y1 / 10000;
        }
        else if (pack == 1U)
        {
            /* CSP */
            y1 = (82 * x * x) - (5171 * x) + 559320;
            y3 = y1 / 10000;
        }
        else
        {
            /* BGA */
            assert(pack == 2U);
            y1 = (25 * x * x) + (1337 * x) + 381140;
            y3 = y1 / 10000;
        }

        trimSvc = ((uint32_t)y3) << 24;

        clk = CLKCTL0->PSCCTL0;
        rst = RSTCTL0->PRSTCTL0;
        /* Enable AON MEM clock/reset. */
        CLKCTL0->PSCCTL0_SET  = CLKCTL0_PSCCTL0_SET_AON_MEM_MASK;
        RSTCTL0->PRSTCTL0_CLR = RSTCTL0_PRSTCTL0_CLR_AON_MEM_MASK;
        POWER_WRITE_MEM32(0x4015C00CU, trimSvc);
        /* Restore AON MEM clock/reset */
        CLKCTL0->PSCCTL0  = clk;
        RSTCTL0->PRSTCTL0 = rst;
    }

    return trimSvc;
}
