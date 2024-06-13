/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.clock"
#endif

#define NVALMAX (0x100U)
#define PVALMAX (0x20U)
#define MVALMAX (0x10000U)

#define PLL_MAX_N_DIV 0x100U

/*--------------------------------------------------------------------------
!!! If required these #defines can be moved to chip library file
----------------------------------------------------------------------------*/

#define PLL_NDIV_VAL_P (0U) /* NDIV is in bits  7:0 */
#define PLL_NDIV_VAL_M (0xFFUL << PLL_NDIV_VAL_P)
#define PLL_MDIV_VAL_P (0U) /* MDIV is in bits  15:0 */
#define PLL_MDIV_VAL_M (0xFFFFULL << PLL_MDIV_VAL_P)
#define PLL_PDIV_VAL_P (0U) /* PDIV is in bits  4:0 */
#define PLL_PDIV_VAL_M (0x1FUL << PLL_PDIV_VAL_P)

#define PLL_MIN_CCO_FREQ_MHZ (275000000U)
#define PLL_MAX_CCO_FREQ_MHZ (550000000U)
#define PLL_LOWER_IN_LIMIT   (32000U)     /*!<  Minimum PLL input rate */
#define PLL_HIGHER_IN_LIMIT  (150000000U) /*!<  Maximum PLL input rate */
#define PLL_MIN_IN_SSMODE    (3000000U)
#define PLL_MAX_IN_SSMODE \
    (100000000U) /*!<  Not find the value in UM, Just use the maximum frequency which device support */

/* PLL NDIV reg */
#define PLL_NDIV_VAL_SET(value) (((unsigned long)(value) << PLL_NDIV_VAL_P) & PLL_NDIV_VAL_M)
/* PLL MDIV reg */
#define PLL_MDIV_VAL_SET(value) (((unsigned long long)(value) << PLL_MDIV_VAL_P) & PLL_MDIV_VAL_M)
/* PLL PDIV reg */
#define PLL_PDIV_VAL_SET(value) (((unsigned long)(value) << PLL_PDIV_VAL_P) & PLL_PDIV_VAL_M)

/* PLL SSCG control1 */
#define PLL_SSCG_MD_FRACT_P 0U
#define PLL_SSCG_MD_INT_P   25U
#define PLL_SSCG_MD_FRACT_M (0x1FFFFFFUL << PLL_SSCG_MD_FRACT_P)
#define PLL_SSCG_MD_INT_M   ((uint64_t)0xFFUL << PLL_SSCG_MD_INT_P)

#define PLL_SSCG_MD_FRACT_SET(value) (((uint64_t)(value) << PLL_SSCG_MD_FRACT_P) & PLL_SSCG_MD_FRACT_M)
#define PLL_SSCG_MD_INT_SET(value)   (((uint64_t)(value) << PLL_SSCG_MD_INT_P) & PLL_SSCG_MD_INT_M)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/** External clock rate on the CLKIN pin in Hz. If not used,
    set this to 0. Otherwise, set it to the exact rate in Hz this pin is
    being driven at. */
volatile static uint32_t s_Ext_Clk_Freq = 16000000U;
/*! @brief External XTAL32K clock frequency. */
volatile static uint32_t s_Xtal32_Freq = 32768U;
/*! @brief SAI MCLK clock frequency. */
volatile static uint32_t s_Sai_Mclk_Freq[2] = {0U};
/*! @brief SAI TX BCLK clock frequency. */
volatile static uint32_t s_Sai_Tx_Bclk_Freq[2] = {0U};
/*! @brief SAI RX BCLK clock frequency. */
volatile static uint32_t s_Sai_Rx_Bclk_Freq[2] = {0U};
/*! @brief ENET TX CLK clock frequency. */
volatile static uint32_t s_Enet_Tx_Clk_Freq = 0U;

/*! @brief external UPLL clock frequency. */
static uint32_t s_extUpllFreq = 0U;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Get FRO 12M Clk */
static uint32_t CLOCK_GetFro12MFreq(void);
/* Get CLK 1M Clk */
static uint32_t CLOCK_GetClk1MFreq(void);
/* Get HF FRO Clk */
static uint32_t CLOCK_GetFroHfFreq(void);
/* Get CLK 48M Clk */
static uint32_t CLOCK_GetClk48MFreq(void);
/* Get CLK 144M Clk */
static uint32_t CLOCK_GetClk144MFreq(void);
/* Get CLK 16K Clk */
static uint32_t CLOCK_GetClk16KFreq(uint32_t id);
/* Get EXT OSC Clk */
static uint32_t CLOCK_GetExtClkFreq(void);
/* Get OSC 32K Clk */
static uint32_t CLOCK_GetOsc32KFreq(uint32_t id);
/* Get Systick Clk */
static uint32_t CLOCK_GetSystickClkFreq(uint32_t id);
/* Get CLOCK OUT Clk */
static uint32_t CLOCK_GetClockOutClkFreq(void);
/* Get LP_OSC Clk */
static uint32_t CLOCK_GetLposcFreq(void);

/* Find SELP, SELI, and SELR values for raw M value, max M = MVALMAX */
static void pllFindSel(uint32_t M, uint32_t *pSelP, uint32_t *pSelI, uint32_t *pSelR);
/* Get predivider (N) from PLL0 NDIV setting */
static uint32_t findPll0PreDiv(void);
/* Get predivider (N) from PLL1 NDIV setting */
static uint32_t findPll1PreDiv(void);
/* Get postdivider (P) from PLL0 PDIV setting */
static uint32_t findPll0PostDiv(void);
/* Get postdivider (P) from PLL1 PDIV setting */
static uint32_t findPll1PostDiv(void);
/* Get multiplier (M) from PLL0 MDIV and SSCG settings */
static float findPll0MMult(void);
/* Get multiplier (M) from PLL1 MDIV and SSCG settings */
static float findPll1MMult(void);
/* Get the greatest common divisor */
static uint32_t FindGreatestCommonDivisor(uint32_t m, uint32_t n);
/* Set PLL output based on desired output rate */
static pll_error_t CLOCK_GetPllConfig(uint32_t finHz, uint32_t foutHz, pll_setup_t *pSetup, bool useSS);
/* Set PLL0 output based on desired output rate */
static pll_error_t CLOCK_GetPllConfigInternal(uint32_t finHz, uint32_t foutHz, pll_setup_t *pSetup, bool useSS);
/* Get PLL input clock rate from setup structure */
static uint32_t CLOCK_GetPLLInClockRateFromSetup(pll_setup_t *pSetup);
/* Get predivider (N) from setup structure */
static uint32_t findPllPreDivFromSetup(pll_setup_t *pSetup);
/* Get postdivider (P) from setup structure */
static uint32_t findPllPostDivFromSetup(pll_setup_t *pSetup);
/* Get multiplier (M) from setup structure */
static float findPllMMultFromSetup(pll_setup_t *pSetup);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief   Initialize the Core clock to given frequency (48 or 144 MHz).
 * This function turns on FIRC and select the given frequency as the source of fro_hf
 * @param   iFreq   : Desired frequency (must be one of CLK_FRO_48MHZ or CLK_FRO_144MHZ)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupFROHFClocking(uint32_t iFreq)
{
    if ((iFreq != 48000000U) && (iFreq != 144000000U))
    {
        return kStatus_Fail;
    }

    /* Select 48MHz or 144MHz for FIRC clock */
    SCG0->FIRCCFG = SCG_FIRCCFG_RANGE((iFreq == 48000000U) ? 0 : 1);

    /* Unlock FIRCCSR */
    SCG0->FIRCCSR &= ~SCG_FIRCCSR_LK_MASK;

    /* Enable FIRC 48 MHz clock for peripheral use */
    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK;
    /* Enable FIRC 144 MHz clock for peripheral use */
    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK;

    /* Enable FIRC */
    SCG0->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK;

    /* Wait for FIRC clock to be valid. */
    while ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) == 0U)
    {
    }

    return kStatus_Success;
}

/**
 * @brief   Initialize the external osc clock to given frequency.
 * @param   iFreq   : Desired frequency (must be equal to exact rate in Hz)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupExtClocking(uint32_t iFreq)
{
    uint8_t range = 0U;

    if ((iFreq >= 16000000U) && (iFreq < 20000000U))
    {
        range = 0U;
    }
    else if ((iFreq >= 20000000U) && (iFreq < 30000000U))
    {
        range = 1U;
    }
    else if ((iFreq >= 30000000U) && (iFreq < 50000000U))
    {
        range = 2U;
    }
    else if ((iFreq >= 50000000U) && (iFreq < 66000000U))
    {
        range = 3U;
    }
    else
    {
        return kStatus_InvalidArgument;
    }

    /* If clock is used by system, return error. */
    if ((SCG0->SOSCCSR & SCG_SOSCCSR_SOSCSEL_MASK) != 0U)
    {
        return (status_t)kStatus_SCG_Busy;
    }

    /* If sosc is used by PLL and PLL is used by system, return error. */
    if ((((SCG0->APLLCTRL & SCG_APLLCTRL_SOURCE_MASK) == 0u) && ((SCG0->APLLCSR & SCG_APLLCSR_APLLSEL_MASK) != 0U)) ||
        (((SCG0->SPLLCTRL & SCG_SPLLCTRL_SOURCE_MASK) == 0u) && ((SCG0->SPLLCSR & SCG_SPLLCSR_SPLLSEL_MASK) != 0U)))
    {
        return (status_t)kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if ((SCG0->SOSCCSR & SCG_SOSCCSR_LK_MASK) != 0U)
    {
        return kStatus_ReadOnly;
    }

    /* De-initializes the SCG SOSC */
    SCG0->SOSCCSR = SCG_SOSCCSR_SOSCERR_MASK;

    /* Enable LDO */
    SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;

    /* Select SOSC source (internal crystal oscillator) and Configure SOSC range */
    SCG0->SOSCCFG = SCG_SOSCCFG_EREFS_MASK | SCG_SOSCCFG_RANGE(range);

    /* Unlock SOSCCSR */
    SCG0->SOSCCSR &= ~SCG_SOSCCSR_LK_MASK;

    /* Enable SOSC clock monitor and Enable SOSC */
    SCG0->SOSCCSR |= (SCG_SOSCCSR_SOSCCM_MASK | SCG_SOSCCSR_SOSCEN_MASK);

    /* Wait for SOSC clock to be valid. */
    while ((SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0U)
    {
    }

    s_Ext_Clk_Freq = iFreq;

    return kStatus_Success;
}

/**
 * @brief   Initialize the external reference clock to given frequency.
 * @param   iFreq   : Desired frequency (must be equal to exact rate in Hz)
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupExtRefClocking(uint32_t iFreq)
{
    uint8_t range = 0U;

    if ((iFreq >= 16000000U) && (iFreq < 20000000U))
    {
        range = 0U;
    }
    else if ((iFreq >= 20000000U) && (iFreq < 30000000U))
    {
        range = 1U;
    }
    else if ((iFreq >= 30000000U) && (iFreq < 50000000U))
    {
        range = 2U;
    }
    else if ((iFreq >= 50000000U) && (iFreq < 66000000U))
    {
        range = 3U;
    }
    else
    {
        return kStatus_InvalidArgument;
    }

    /* If clock is used by system, return error. */
    if ((SCG0->SOSCCSR & SCG_SOSCCSR_SOSCSEL_MASK) != 0U)
    {
        return (status_t)kStatus_SCG_Busy;
    }

    /* If sosc is used by PLL and PLL is used by system, return error. */
    if ((((SCG0->APLLCTRL & SCG_APLLCTRL_SOURCE_MASK) == 0u) && ((SCG0->APLLCSR & SCG_APLLCSR_APLLSEL_MASK) != 0U)) ||
        (((SCG0->SPLLCTRL & SCG_SPLLCTRL_SOURCE_MASK) == 0u) && ((SCG0->SPLLCSR & SCG_SPLLCSR_SPLLSEL_MASK) != 0U)))
    {
        return (status_t)kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if ((SCG0->SOSCCSR & SCG_SOSCCSR_LK_MASK) != 0U)
    {
        return kStatus_ReadOnly;
    }

    /* De-initializes the SCG SOSC */
    SCG0->SOSCCSR = SCG_SOSCCSR_SOSCERR_MASK;

    /* Enable LDO */
    SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;

    /* Select SOSC source (external reference clock)*/
    SCG0->SOSCCFG &= ~SCG_SOSCCFG_EREFS_MASK;

    /*Configure SOSC range */
    SCG0->SOSCCFG |= SCG_SOSCCFG_RANGE(range);

    /* Unlock SOSCCSR */
    SCG0->SOSCCSR &= ~SCG_SOSCCSR_LK_MASK;

    /* Enable SOSC clock monitor and Enable SOSC */
    SCG0->SOSCCSR |= (SCG_SOSCCSR_SOSCCM_MASK | SCG_SOSCCSR_SOSCEN_MASK);

    /* Wait for SOSC clock to be valid. */
    while ((SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0U)
    {
    }

    s_Ext_Clk_Freq = iFreq;

    return kStatus_Success;
}

/**
 * @brief   Initialize the OSC 32K.
 * @param   id   : OSC 32 kHz output clock to specified modules
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupOsc32KClocking(uint32_t id)
{
    /* Enable LDO */
    SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK | SCG_LDOCSR_VOUT_OK_MASK;

    VBAT0->OSCCTLA =
        (VBAT0->OSCCTLA & ~(VBAT_OSCCTLA_MODE_EN_MASK | VBAT_OSCCTLA_CAP_SEL_EN_MASK | VBAT_OSCCTLA_OSC_EN_MASK)) |
        VBAT_OSCCTLA_MODE_EN(0x2) | VBAT_OSCCTLA_OSC_EN_MASK | VBAT_OSCCTLA_OSC_EN_MASK;
    VBAT0->OSCCTLB = VBAT_OSCCTLB_INVERSE(0xDFF7E);
    /* Wait for STATUSA[OSC_RDY] to set. */
    while ((VBAT0->STATUSA & VBAT_STATUSA_OSC_RDY_MASK) == 0U)
    {
    }
    VBAT0->OSCLCKA = VBAT_OSCLCKA_LOCK_MASK;
    VBAT0->OSCLCKB &= ~VBAT_OSCLCKA_LOCK_MASK;

    VBAT0->OSCCLKE |= VBAT_OSCCLKE_CLKE(id);

    /* De-initializes the SCG ROSC */
    SCG0->ROSCCSR = SCG_ROSCCSR_ROSCERR_MASK;

    /* Unlock ROSCCSR */
    SCG0->ROSCCSR &= ~SCG_ROSCCSR_LK_MASK;

    /* Enable SOSC clock monitor and Enable ROSC */
    SCG0->ROSCCSR |= SCG_ROSCCSR_ROSCCM_MASK;

    /* Wait for ROSC clock to be valid. */
    while ((SCG0->ROSCCSR & SCG_ROSCCSR_ROSCVLD_MASK) == 0U)
    {
    }

    s_Xtal32_Freq = 32768U;

    return kStatus_Success;
}

/**
 * @brief   Initialize the CLK16K clock.
 * @param   id   : CLK 16 kHz output clock to specified modules
 * @return  returns success or fail status.
 */
status_t CLOCK_SetupClk16KClocking(uint32_t id)
{
    VBAT0->FROCTLA |= VBAT_FROCTLA_FRO_EN_MASK;
    VBAT0->FROCTLB &= ~VBAT_FROCTLB_INVERSE_MASK;

    VBAT0->FROLCKA |= VBAT_FROLCKA_LOCK_MASK;
    VBAT0->FROLCKB &= ~VBAT_FROLCKB_LOCK_MASK;

    VBAT0->FROCLKE |= VBAT_FROCLKE_CLKE(id);

    return kStatus_Success;
}

/**
 * @brief   Setup FROHF trim.
 * @param   config   : FROHF trim value
 * @return  returns success or fail status.
 */
status_t CLOCK_FROHFTrimConfig(firc_trim_config_t config)
{
    SCG0->FIRCTCFG = SCG_FIRCTCFG_TRIMDIV(config.trimDiv) | SCG_FIRCTCFG_TRIMSRC(config.trimSrc);

    if (kSCG_FircTrimNonUpdate == config.trimMode)
    {
        SCG0->FIRCSTAT = SCG_FIRCSTAT_TRIMFINE(config.trimFine);
    }

    /* Set trim mode. */
    SCG0->FIRCCSR = (uint32_t)config.trimMode;

    if ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCERR_MASK) == SCG_FIRCCSR_FIRCERR_MASK)
    {
        return (status_t)kStatus_Fail;
    }

    return (status_t)kStatus_Success;
}

/**
 * @brief   Setup FRO 12M trim.
 * @param   config   : FRO 12M trim value
 * @return  returns success or fail status.
 */
status_t CLOCK_FRO12MTrimConfig(sirc_trim_config_t config)
{
    SCG0->SIRCTCFG = SCG_SIRCTCFG_TRIMDIV(config.trimDiv) | SCG_SIRCTCFG_TRIMSRC(config.trimSrc);

    if (kSCG_SircTrimNonUpdate == config.trimMode)
    {
        SCG0->SIRCSTAT = SCG_SIRCSTAT_CCOTRIM(config.cltrim);
        SCG0->SIRCSTAT = SCG_SIRCSTAT_CCOTRIM(config.ccotrim);
    }

    /* Set trim mode. */
    SCG0->SIRCCSR = (uint32_t)config.trimMode;

    if ((SCG0->SIRCCSR & SCG_SIRCCSR_SIRCERR_MASK) == SCG_SIRCCSR_SIRCERR_MASK)
    {
        return (status_t)kStatus_Fail;
    }

    return (status_t)kStatus_Success;
}

/*!
 * @brief Sets the system OSC monitor mode.
 *
 * This function sets the system OSC monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetSysOscMonitorMode(scg_sosc_monitor_mode_t mode)
{
    uint32_t reg = SCG0->SOSCCSR;

    reg &= ~(SCG_SOSCCSR_SOSCCM_MASK | SCG_SOSCCSR_SOSCCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG0->SOSCCSR = reg;
}

/*!
 * @brief Sets the ROSC monitor mode.
 *
 * This function sets the ROSC monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetRoscMonitorMode(scg_rosc_monitor_mode_t mode)
{
    uint32_t reg = SCG0->ROSCCSR;

    reg &= ~(SCG_ROSCCSR_ROSCCM_MASK | SCG_ROSCCSR_ROSCCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG0->ROSCCSR = reg;
}

/*!
 * @brief Sets the UPLL monitor mode.
 *
 * This function sets the UPLL monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetUpllMonitorMode(scg_upll_monitor_mode_t mode)
{
    uint32_t reg = SCG0->UPLLCSR;

    reg &= ~(SCG_UPLLCSR_UPLLCM_MASK | SCG_UPLLCSR_UPLLCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG0->UPLLCSR = reg;
}

/*!
 * @brief Sets the PLL0 monitor mode.
 *
 * This function sets the PLL0 monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetPll0MonitorMode(scg_pll0_monitor_mode_t mode)
{
    uint32_t reg = SCG0->APLLCSR;

    reg &= ~(SCG_APLLCSR_APLLCM_MASK | SCG_APLLCSR_APLLCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG0->APLLCSR = reg;
}

/*!
 * @brief Sets the PLL1 monitor mode.
 *
 * This function sets the PLL1 monitor mode. The mode can be disabled,
 * it can generate an interrupt when the error is disabled, or reset when the error is detected.
 *
 * @param mode Monitor mode to set.
 */
void CLOCK_SetPll1MonitorMode(scg_pll1_monitor_mode_t mode)
{
    uint32_t reg = SCG0->SPLLCSR;

    reg &= ~(SCG_SPLLCSR_SPLLCM_MASK | SCG_SPLLCSR_SPLLCMRE_MASK);

    reg |= (uint32_t)mode;

    SCG0->SPLLCSR = reg;
}

/*!
 * @brief	Set the additional number of wait-states added to account for the ratio of system clock period to flash access
 * time during full speed power mode.
 * @param	system_freq_hz	: Input frequency
 * @param	mode	        : Active run mode (voltage level).
 * @return	success or fail status
 */
status_t CLOCK_SetFLASHAccessCyclesForFreq(uint32_t system_freq_hz, run_mode_t mode)
{
    uint32_t num_wait_states_added = 3UL; /* Default 3 additional wait states */
    switch (mode)
    {
        case kMD_Mode:
        {
            if (system_freq_hz > 50000000)
            {
                return kStatus_Fail;
            }
            if (system_freq_hz > 24000000)
            {
                num_wait_states_added = 1U;
            }
            else
            {
                num_wait_states_added = 0U;
            }
            break;
        }
        case kSD_Mode:
        {
            if (system_freq_hz > 100000000)
            {
                return kStatus_Fail;
            }
            if (system_freq_hz > 64000000)
            {
                num_wait_states_added = 2U;
            }
            else if (system_freq_hz > 36000000)
            {
                num_wait_states_added = 1U;
            }
            else
            {
                num_wait_states_added = 0U;
            }
            break;
        }
        case kOD_Mode:
        {
            if (system_freq_hz > 150000000)
            {
                return kStatus_Fail;
            }
            if (system_freq_hz > 100000000)
            {
                num_wait_states_added = 3U;
            }
            else if (system_freq_hz > 64000000)
            {
                num_wait_states_added = 2U;
            }
            else if (system_freq_hz > 36000000)
            {
                num_wait_states_added = 1U;
            }
            else
            {
                num_wait_states_added = 0U;
            }
        }
    }

    /* additional wait-states are added */
    FMU0->FCTRL = (FMU0->FCTRL & 0xFFFFFFF0UL) | (num_wait_states_added & 0xFUL);

    return kStatus_Success;
}

/*!
 * @brief Config 32k Crystal Oscillator.
 *
 * @param base VBAT peripheral base address.
 * @param config The pointer to the structure \ref vbat_osc_config_t.
 */
void VBAT_SetOscConfig(VBAT_Type *base, const vbat_osc_config_t *config)
{
    uint32_t tmp32;

    if (config->enableCrystalOscillatorBypass == true)
    {
        base->OSCCTLA |= VBAT_OSCCTLA_OSC_BYP_EN_MASK;
        while ((VBAT0->STATUSA & VBAT_STATUSA_OSC_RDY_MASK) == 0U)
        {
        }
    }
    else
    {
        tmp32 = base->OSCCTLA;

        if (config != NULL)
        {
            if (config->enableInternalCapBank)
            {
                tmp32 &= ~(VBAT_OSCCTLA_EXTAL_CAP_SEL_MASK | VBAT_OSCCTLA_XTAL_CAP_SEL_MASK);
                tmp32 |= VBAT_OSCCTLA_EXTAL_CAP_SEL(config->extalCap) | VBAT_OSCCTLA_XTAL_CAP_SEL(config->xtalCap);
                tmp32 |= VBAT_OSCCTLA_CAP_SEL_EN_MASK;
            }
            else
            {
                /* Disable the internal capacitance bank. */
                tmp32 &= ~VBAT_OSCCTLA_CAP_SEL_EN_MASK;
            }

            tmp32 &= ~(VBAT_OSCCTLA_COARSE_AMP_GAIN_MASK);
            tmp32 |= VBAT_OSCCTLA_COARSE_AMP_GAIN(config->coarseAdjustment);
        }
        base->OSCCTLA = tmp32;
        while ((VBAT0->STATUSA & VBAT_STATUSA_OSC_RDY_MASK) == 0U)
        {
        }
    }
}

/* Clock Selection for IP */
/**
 * brief   Configure the clock selection muxes.
 * param   connection  : Clock to be configured.
 * return  Nothing
 */
void CLOCK_AttachClk(clock_attach_id_t connection)
{
    uint16_t mux;
    uint8_t sel;
    uint16_t item;
    uint32_t tmp32 = (uint32_t)connection;
    uint32_t i;
    volatile uint32_t *pClkSel;

    pClkSel = &(SYSCON->SYSTICKCLKSEL0);

    if (kNONE_to_NONE != connection)
    {
        for (i = 0U; i < 2U; i++)
        {
            if (tmp32 == 0U)
            {
                break;
            }
            item = (uint16_t)GET_ID_ITEM(tmp32);
            if (item != 0U)
            {
                mux = (uint16_t)GET_ID_ITEM_MUX(item);
                sel = (uint8_t)GET_ID_ITEM_SEL(item);
                if (mux == CM_SCGRCCRSCSCLKSEL)
                {
                    SCG0->RCCR = (SCG0->RCCR & ~(SCG_RCCR_SCS_MASK)) | SCG_RCCR_SCS(sel);
                    while ((SCG0->CSR & SCG_CSR_SCS_MASK) != SCG_CSR_SCS(sel))
                    {
                    }
                }
                else
                {
                    ((volatile uint32_t *)pClkSel)[mux] = sel;
                }
            }
            tmp32 = GET_ID_NEXT_ITEM(tmp32); /* pick up next descriptor */
        }
    }
}

/* Return the actual clock attach id */
/**
 * brief   Get the actual clock attach id.
 * This fuction uses the offset in input attach id, then it reads the actual source value in
 * the register and combine the offset to obtain an actual attach id.
 * param   attachId  : Clock attach id to get.
 * return  Clock source value.
 */
clock_attach_id_t CLOCK_GetClockAttachId(clock_attach_id_t attachId)
{
    uint16_t mux;
    uint32_t actualSel;
    uint32_t tmp32 = (uint32_t)attachId;
    uint32_t i;
    uint32_t actualAttachId = 0U;
    uint32_t selector       = GET_ID_SELECTOR(tmp32);
    volatile uint32_t *pClkSel;

    pClkSel = &(SYSCON->SYSTICKCLKSEL0);

    if (kNONE_to_NONE == attachId)
    {
        return kNONE_to_NONE;
    }

    for (i = 0U; i < 2U; i++)
    {
        mux = (uint16_t)GET_ID_ITEM_MUX(tmp32);
        if (tmp32 != 0UL)
        {
            if (mux == CM_SCGRCCRSCSCLKSEL)
            {
                actualSel = (uint32_t)((SCG0->RCCR & SCG_RCCR_SCS_MASK) >> SCG_RCCR_SCS_SHIFT);
            }
            else
            {
                actualSel = (uint32_t)((volatile uint32_t *)pClkSel)[mux];
            }

            /* Consider the combination of two registers */
            actualAttachId |= CLK_ATTACH_ID(mux, actualSel, i);
        }
        tmp32 = GET_ID_NEXT_ITEM(tmp32); /*!<  pick up next descriptor */
    }

    actualAttachId |= selector;

    return (clock_attach_id_t)actualAttachId;
}

/* Set IP Clock Divider */
/**
 * brief   Setup peripheral clock dividers.
 * param   div_name    : Clock divider name
 * param divided_by_value: Value to be divided
 * return  Nothing
 */
void CLOCK_SetClkDiv(clock_div_name_t div_name, uint32_t divided_by_value)
{
    volatile uint32_t *pClkDiv;

    pClkDiv = &(SYSCON->SYSTICKCLKDIV[0]);
    /* halt and reset clock dividers */
    ((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] = 0x3UL << 29U;

    if (divided_by_value == 0U) /*!<  halt */
    {
        ((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] = 1UL << 30U;
    }
    else
    {
        ((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] = (divided_by_value - 1U);
    }
}

/* Get IP clock dividers */
/**
 * brief   Get peripheral clock dividers.
 * param   div_name    : Clock divider name
 * return  peripheral clock dividers
 */
uint32_t CLOCK_GetClkDiv(clock_div_name_t div_name)
{
    uint32_t div;
    volatile uint32_t *pClkDiv;

    pClkDiv = &(SYSCON->SYSTICKCLKDIV[0]);

    if ((uint32_t)(((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] & (0x3UL << 29U)) != 0UL)
    {
        div = 0U;
    }
    else
    {
        div = (uint32_t)((((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] & 0xFFU) + 1U);
    }

    return div;
}

/* Halt IP Clock Divider */
/**
 * brief   Setup peripheral clock dividers.
 * param   Halt    : Clock divider name
 * return  Nothing
 */
void CLOCK_HaltClkDiv(clock_div_name_t div_name)
{
    volatile uint32_t *pClkDiv;

    pClkDiv = &(SYSCON->SYSTICKCLKDIV[0]);

    /* halt clock dividers */
    ((volatile uint32_t *)pClkDiv)[(uint32_t)div_name] = 1UL << 30U;

    return;
}

/* enable system clocks */
/**
 * brief   system clocks enable controls.
 * param   mask : system clocks enable value
 * return  Nothing
 */
void CLOCK_SetupClockCtrl(uint32_t mask)
{
    SYSCON->CLOCK_CTRL |= mask;

    return;
}

/* Get IP Clk */
/*! brief  Return Frequency of selected clock
 *  return Frequency of selected clock
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName)
{
    uint32_t freq = 0U;

    switch (clockName)
    {
        case kCLOCK_MainClk:
            freq = CLOCK_GetMainClkFreq();
            break;
        case kCLOCK_CoreSysClk:
            freq = CLOCK_GetCoreSysClkFreq();
            break;
        case kCLOCK_BusClk:
            freq = CLOCK_GetCoreSysClkFreq();
            break;
        case kCLOCK_SystickClk0:
            freq = CLOCK_GetSystickClkFreq(0U);
            break;
        case kCLOCK_SystickClk1:
            freq = CLOCK_GetSystickClkFreq(1U);
            break;
        case kCLOCK_ClockOut:
            freq = CLOCK_GetClockOutClkFreq();
            break;
        case kCLOCK_Clk1M:
            freq = CLOCK_GetClk1MFreq();
            break;
        case kCLOCK_Fro12M:
            freq = CLOCK_GetFro12MFreq();
            break;
        case kCLOCK_FroHf:
            freq = CLOCK_GetFroHfFreq();
            break;
        case kCLOCK_Clk48M:
            freq = CLOCK_GetClk48MFreq();
            break;
        case kCLOCK_Clk144M:
            freq = CLOCK_GetClk144MFreq();
            break;
        case kCLOCK_Clk16K0:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToVbat);
            break;
        case kCLOCK_Clk16K1:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToVsys);
            break;
        case kCLOCK_Clk16K2:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToWake);
            break;
        case kCLOCK_Clk16K3:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToMain);
            break;
        case kCLOCK_ExtClk:
            freq = CLOCK_GetExtClkFreq();
            break;
        case kCLOCK_Osc32K0:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        case kCLOCK_Osc32K1:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVsys);
            break;
        case kCLOCK_Osc32K2:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToWake);
            break;
        case kCLOCK_Osc32K3:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToMain);
            break;
        case kCLOCK_Pll0Out:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case kCLOCK_Pll1Out:
            freq = CLOCK_GetPll1OutFreq();
            break;
        case kCLOCK_UsbPllOut:
            // freq = CLOCK_GetPll0OutFreq();
            break;
        case kCLOCK_LpOsc:
            freq = CLOCK_GetLposcFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

/* Get CTimer Clk */
/*! brief  Return Frequency of CTimer functional Clock
 *  return Frequency of CTimer functional Clock
 */
uint32_t CLOCK_GetCTimerClkFreq(uint32_t id)
{
    uint32_t freq = 0U;

    switch (SYSCON->CTIMERCLKSEL[id])
    {
        case 0U:
            freq = CLOCK_GetClk1MFreq();
            break;
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetSaiMclkFreq(0U);
            break;
        case 6U:
            freq = CLOCK_GetLposcFreq();
            break;
        case 8U:
            freq = CLOCK_GetSaiMclkFreq(1U);
            break;
        case 9U:
            freq = CLOCK_GetSaiTxBclkFreq(0U);
            break;
        case 10U:
            freq = CLOCK_GetSaiRxBclkFreq(0U);
            break;
        case 11U:
            freq = CLOCK_GetSaiTxBclkFreq(1U);
            break;
        case 12U:
            freq = CLOCK_GetSaiRxBclkFreq(1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->CTIMERCLKDIV[id] & 0xffU) + 1U);
}

/* Get ADC Clk */
/*! brief  Return Frequency of Adc Clock
 *  return Frequency of Adc.
 */
uint32_t CLOCK_GetAdcClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->ADC0CLKSEL) : (SYSCON->ADC1CLKSEL))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 3U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 4U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->ADC0CLKDIV & SYSCON_ADC0CLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->ADC1CLKDIV & SYSCON_ADC1CLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get USB0 Clk */
/*! brief  Return Frequency of Usb0 Clock
 *  return Frequency of Usb0 Clock.
 */
uint32_t CLOCK_GetUsb0ClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->USB0CLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        case 7U:
            freq = 0U;
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->USB0CLKDIV & 0xffU) + 1U);
}

/* Get LPFLEXCOMM Clk */
/*! brief  Return Frequency of LPFLEXCOMM Clock
 *  return Frequency of LPFLEXCOMM Clock.
 */
uint32_t CLOCK_GetLPFlexCommClkFreq(uint32_t id)
{
    uint32_t freq = 0U;

    switch (SYSCON->FCCLKSEL[id])
    {
        case 1U:
            freq = CLOCK_GetPllClkDivFreq();
            break;
        case 2U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq() / ((SYSCON->FROHFDIV & 0xffU) + 1U);
            break;
        case 4U:
            freq = CLOCK_GetClk1MFreq();
            break;
        case 5U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        case 6U:
            freq = CLOCK_GetLposcFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->FLEXCOMMCLKDIV[id] & 0xffU) + 1U);
}

/* Get SCTIMER Clk */
/*! brief  Return Frequency of SCTimer Clock
 *  return Frequency of SCTimer Clock.
 */
uint32_t CLOCK_GetSctClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->SCTCLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 5U:
            freq = CLOCK_GetSaiMclkFreq(0U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        case 8U:
            freq = CLOCK_GetSaiMclkFreq(1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->SCTCLKDIV & 0xffU) + 1U);
}

/* Get TSI Clk */
/*! brief  Return Frequency of TSI Clock
 *  return Frequency of TSI Clock.
 */
uint32_t CLOCK_GetTsiClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->TSICLKSEL)
    {
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->TSICLKDIV & 0xffU) + 1U);
}

/* Get SINC FILTER Clk */
/*! brief  Return Frequency of SINC FILTER Clock
 *  return Frequency of SINC FILTER Clock.
 */
uint32_t CLOCK_GetSincFilterClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->SINCFILTCLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get DAC Clk */
/*! brief  Return Frequency of DAC Clock
 *  return Frequency of DAC.
 */
uint32_t CLOCK_GetDacClkFreq(uint32_t id)
{
    uint32_t freq = 0U;

    switch (SYSCON->DAC[id].CLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->DAC[id].CLKDIV & SYSCON_DAC_CLKDIV_DIV_MASK) + 1U);
}

/* Get FlexSpi Clk */
/*! brief  Return Frequency of FlexSpi clock
 *  return Frequency of FlexSpi Clock
 */
uint32_t CLOCK_GetFlexspiClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->FLEXSPICLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq();
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->FLEXSPICLKDIV & SYSCON_FLEXSPICLKDIV_DIV_MASK) + 1U);
}

/* Get SYSTEM PLL0 Clk */
/*! brief  Return Frequency of PLL0
 *  return Frequency of PLL0
 */
uint32_t CLOCK_GetPll0OutFreq(void)
{
    uint32_t clkRate = 0;
    uint32_t prediv, postdiv;
    float workRate = 0.0F;

    /* Get the input clock frequency of PLL. */
    clkRate = CLOCK_GetPLL0InClockRate();

    /* If PLL0 is work */
    if (CLOCK_IsPLL0Locked() == true)
    {
        prediv  = findPll0PreDiv();
        postdiv = findPll0PostDiv();
        /* Adjust input clock */
        clkRate = clkRate / prediv;
        /* MDEC used for rate */
        workRate = (float)clkRate * (float)findPll0MMult();
        workRate /= (float)postdiv;
    }

    return (uint32_t)workRate;
}

/* Get SYSTEM PLL1 Clk */
/*! brief  Return Frequency of PLL1
 *  return Frequency of PLL1
 */
uint32_t CLOCK_GetPll1OutFreq(void)
{
    uint32_t clkRate = 0;
    uint32_t prediv, postdiv;
    float workRate = 0.0F;

    /* Get the input clock frequency of PLL. */
    clkRate = CLOCK_GetPLL1InClockRate();

    /* If PLL1 is work */
    if (CLOCK_IsPLL1Locked() == true)
    {
        prediv  = findPll1PreDiv();
        postdiv = findPll1PostDiv();
        /* Adjust input clock */
        clkRate = clkRate / prediv;
        /* MDEC used for rate */
        workRate = (float)clkRate * (float)findPll1MMult();
        workRate /= (float)postdiv;
    }

    return (uint32_t)workRate;
}

/* Get PLLClkDiv Clk */
/*! brief  Return Frequency of PLLClkDiv
 *  return Frequency of PLLClkDiv
 */
uint32_t CLOCK_GetPllClkDivFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->PLLCLKDIVSEL)
    {
        case 0U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 1U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->PLLCLKDIV & SYSCON_PLLCLKDIV_DIV_MASK) + 1U);
}

/*!
 * brief Gets the external UPLL frequency.
 *
 * This function gets the external UPLL frequency in Hz.
 *
 * return The frequency of the external UPLL.
 */
uint32_t CLOCK_GetExtUpllFreq(void)
{
    return s_extUpllFreq;
}

/*!
 * brief Sets the external UPLL frequency.
 *
 * This function sets the external UPLL frequency in Hz.
 * Call this function after the external PLL frequency is changed.
 * Otherwise, the APIs, which are used to get the frequency, may return an incorrect value.
 *
 * param The frequency of external UPLL.
 */
void CLOCK_SetExtUpllFreq(uint32_t freq)
{
    s_extUpllFreq = freq;
}

/* Get I3C function Clk */
/*! brief  Return Frequency of I3C function clock
 *  return Frequency of I3C function Clock
 */
uint32_t CLOCK_GetI3cClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->I3C0FCLKSEL) : (SYSCON->I3C1FCLKSEL))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->I3C0FCLKDIV & SYSCON_I3C0FCLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->I3C1FCLKDIV & SYSCON_I3C1FCLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get I3C function slow TC Clk */
/*! brief  Return Frequency of I3C function Slow TC clock
 *  return Frequency of I3C function slow TC Clock
 */
uint32_t CLOCK_GetI3cSTCClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->I3C0FCLKSTCSEL) : (SYSCON->I3C1FCLKSTCSEL))
    {
        case 0U:
            switch ((id == 0U) ? (SYSCON->I3C0FCLKSEL) : (SYSCON->I3C1FCLKSEL))
            {
                case 1U:
                    freq = CLOCK_GetPll0OutFreq();
                    break;
                case 2U:
                    freq = CLOCK_GetExtClkFreq();
                    break;
                case 3U:
                    freq = CLOCK_GetFroHfFreq();
                    break;
                case 5U:
                    freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
                    break;
                case 6U:
                    // freq = CLOCK_GetUPllOutFreq();
                default:
                    freq = 0U;
                    break;
            }
            break;
        case 1U:
            freq = CLOCK_GetClk1MFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->I3C0FCLKSTCDIV & SYSCON_I3C0FCLKSTCDIV_DIV_MASK) + 1U) :
                        ((SYSCON->I3C1FCLKSTCDIV & SYSCON_I3C1FCLKSTCDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get I3C function slow Clk */
/*! brief  Return Frequency of I3C function Slow clock
 *  return Frequency of I3C function slow Clock
 */
uint32_t CLOCK_GetI3cSClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->I3C0FCLKSSEL) : (SYSCON->I3C1FCLKSSEL))
    {
        case 0U:
            freq = CLOCK_GetClk1MFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->I3C0FCLKSDIV & SYSCON_I3C0FCLKSDIV_DIV_MASK) + 1U) :
                        ((SYSCON->I3C1FCLKSDIV & SYSCON_I3C1FCLKSDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get MICFIL Clk */
/*! brief  Return Frequency of MICFIL
 *  return Frequency of MICFIL
 */
uint32_t CLOCK_GetMicfilClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->MICFILFCLKSEL)
    {
        case 0U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 5U:
            freq = CLOCK_GetSaiMclkFreq(0U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        case 8U:
            freq = CLOCK_GetSaiMclkFreq(1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->MICFILFCLKDIV & SYSCON_MICFILFCLKDIV_DIV_MASK) + 1U);
}

/* Get uSDHC Clk */
/*! brief  Return Frequency of uSDHC
 *  return Frequency of uSDHC
 */
uint32_t CLOCK_GetUsdhcClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->USDHCCLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK1DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->USDHCCLKDIV & SYSCON_USDHCCLKDIV_DIV_MASK) + 1U);
}

/* Get FLEXIO Clk */
/*! brief  Return Frequency of FLEXIO
 *  return Frequency of FLEXIO
 */
uint32_t CLOCK_GetFlexioClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->FLEXIOCLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->FLEXIOCLKDIV & SYSCON_FLEXIOCLKDIV_DIV_MASK) + 1U);
}

/* Get FLEXCAN Clk */
/*! brief  Return Frequency of FLEXCAN
 *  return Frequency of FLEXCAN
 */
uint32_t CLOCK_GetFlexcanClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->FLEXCAN0CLKSEL) : (SYSCON->FLEXCAN1CLKSEL))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->FLEXCAN0CLKDIV & SYSCON_FLEXCAN0CLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->FLEXCAN1CLKDIV & SYSCON_FLEXCAN1CLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get Ethernet RMII Clk */
/*! brief  Return Frequency of Ethernet RMII
 *  return Frequency of Ethernet RMII
 */
uint32_t CLOCK_GetEnetRmiiClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->ENETRMIICLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->ENETRMIICLKDIV & SYSCON_ENETRMIICLKDIV_DIV_MASK) + 1U);
}

/* Get Ethernet PTP REF Clk */
/*! brief  Return Frequency of Ethernet PTP REF
 *  return Frequency of Ethernet PTP REF
 */
uint32_t CLOCK_GetEnetPtpRefClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->ENETPTPREFCLKSEL)
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 4U: // Todo enet0_tx_clk clock
            freq = 0U;
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq / ((SYSCON->ENETPTPREFCLKDIV & SYSCON_ENETPTPREFCLKDIV_DIV_MASK) + 1U);
}

/* Get ENET TX CLK */
/*! brief  Initialize the ENET TX CLK to given frequency.
 *  return Nothing
 */
void CLOCK_SetupEnetTxClk(uint32_t iFreq)
{
    s_Enet_Tx_Clk_Freq = iFreq;

    return;
}

/* Get ENET TX CLK */
/*! brief  Return Frequency of ENET TX CLK
 *  return Frequency of ENET TX CLK
 */
uint32_t CLOCK_GetEnetTxClkFreq(void)
{
    return s_Enet_Tx_Clk_Freq;
}

/* Get EWM0 Clk */
/*! brief  Return Frequency of EWM0
 *  return Frequency of EWM0
 */
uint32_t CLOCK_GetEwm0ClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->EWM0CLKSEL)
    {
        case 1U:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToWake);
            break;
        case 2U:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToWake);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get Watchdog Clk */
/*! brief  Return Frequency of Watchdog
 *  return Frequency of Watchdog
 */
uint32_t CLOCK_GetWdtClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    if (id == 0U)
    {
        freq = CLOCK_GetClk1MFreq();
    }
    else
    {
        switch (SYSCON->WDT1CLKSEL)
        {
            case 0U:
                freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToWake);
                break;
            case 1U:
                freq = CLOCK_GetFroHfFreq() / ((SYSCON->FROHFDIV & 0xffU) + 1U);
                break;
            case 2U:
                freq = CLOCK_GetClk1MFreq();
                break;
            default:
                freq = 0U;
                break;
        }
    }

    div = ((id == 0U) ? ((SYSCON->WDT0CLKDIV & SYSCON_WDT0CLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->WDT1CLKDIV & SYSCON_WDT1CLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get OSTIMER Clk */
/*! brief  Return Frequency of OSTIMER
 *  return Frequency of OSTIMER
 */
uint32_t CLOCK_GetOstimerClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->OSTIMERCLKSEL)
    {
        case 0U:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToWake);
            break;
        case 1U:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToWake);
            break;
        case 2U:
            freq = CLOCK_GetClk1MFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get CMP Function Clk */
/*! brief  Return Frequency of CMP Function
 *  return Frequency of CMP Function
 */
uint32_t CLOCK_GetCmpFClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->CMP0FCLKSEL) : ((id == 1U) ? (SYSCON->CMP1FCLKSEL) : (SYSCON->CMP2FCLKSEL)))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 3U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 4U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->CMP0FCLKDIV & SYSCON_CMP0FCLKDIV_DIV_MASK) + 1U) :
                        ((id == 1U) ? ((SYSCON->CMP1FCLKDIV & SYSCON_CMP1FCLKDIV_DIV_MASK) + 1U) :
                                      ((SYSCON->CMP2FCLKDIV & SYSCON_CMP2FCLKDIV_DIV_MASK) + 1U)));

    return freq / div;
}

/* Get CMP Round Robin Clk */
/*! brief  Return Frequency of CMP Round Robin
 *  return Frequency of CMP Round Robin
 */
uint32_t CLOCK_GetCmpRRClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->CMP0RRCLKSEL) : ((id == 1U) ? (SYSCON->CMP1RRCLKSEL) : (SYSCON->CMP2RRCLKSEL)))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 3U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 4U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->CMP0RRCLKDIV & SYSCON_CMP0RRCLKDIV_DIV_MASK) + 1U) :
                        ((id == 1U) ? ((SYSCON->CMP1RRCLKDIV & SYSCON_CMP1RRCLKDIV_DIV_MASK) + 1U) :
                                      ((SYSCON->CMP2RRCLKDIV & SYSCON_CMP2RRCLKDIV_DIV_MASK) + 1U)));

    return freq / div;
}

/* Get SAI Clk */
/*! brief  Return Frequency of SAI
 *  return Frequency of SAI
 */
uint32_t CLOCK_GetSaiClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->SAI0CLKSEL) : (SYSCON->SAI1CLKSEL))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            // freq = CLOCK_GetUPllOutFreq();
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->SAI0CLKDIV & SYSCON_SAI0CLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->SAI1CLKDIV & SYSCON_SAI1CLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Get SAI MCLK */
/*! brief  Initialize the SAI MCLK to given frequency.
 *  return Nothing
 */
void CLOCK_SetupSaiMclk(uint32_t id, uint32_t iFreq)
{
    s_Sai_Mclk_Freq[id] = iFreq;

    return;
}

/* Get SAI TX BCLK */
/*! brief  Initialize the SAI TX BCLK to given frequency.
 *  return Nothing
 */
void CLOCK_SetupSaiTxBclk(uint32_t id, uint32_t iFreq)
{
    s_Sai_Tx_Bclk_Freq[id] = iFreq;

    return;
}

/* Get SAI RX BCLK */
/*! brief  Initialize the SAI RX BCLK to given frequency.
 *  return Nothing
 */
void CLOCK_SetupSaiRxBclk(uint32_t id, uint32_t iFreq)
{
    s_Sai_Rx_Bclk_Freq[id] = iFreq;

    return;
}

/* Get SAI MCLK */
/*! brief  Return Frequency of SAI MCLK
 *  return Frequency of SAI MCLK
 */
uint32_t CLOCK_GetSaiMclkFreq(uint32_t id)
{
    return s_Sai_Mclk_Freq[id];
}

/* Get SAI TX BCLK */
/*! brief  Return Frequency of SAI TX BCLK
 *  return Frequency of SAI TX BCLK
 */
uint32_t CLOCK_GetSaiTxBclkFreq(uint32_t id)
{
    return s_Sai_Tx_Bclk_Freq[id];
}

/* Get SAI RX BCLK */
/*! brief  Return Frequency of SAI RX BCLK
 *  return Frequency of SAI RX BCLK
 */
uint32_t CLOCK_GetSaiRxBclkFreq(uint32_t id)
{
    return s_Sai_Rx_Bclk_Freq[id];
}

/* Get EMVSIM Clk */
/*! brief  Return Frequency of EMVSIM
 *  return Frequency of EMVSIM
 */
uint32_t CLOCK_GetEmvsimClkFreq(uint32_t id)
{
    uint32_t freq = 0U;
    uint32_t div  = 0U;

    switch ((id == 0U) ? (SYSCON->EMVSIM0CLKSEL) : (SYSCON->EMVSIM1CLKSEL))
    {
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    div = ((id == 0U) ? ((SYSCON->EMVSIM0CLKDIV & SYSCON_EMVSIM0CLKDIV_DIV_MASK) + 1U) :
                        ((SYSCON->EMVSIM1CLKDIV & SYSCON_EMVSIM1CLKDIV_DIV_MASK) + 1U));

    return freq / div;
}

/* Return System PLL input clock rate */
/*! brief    Return  PLL0 input clock rate
 *  return    PLL0 input clock rate
 */
uint32_t CLOCK_GetPLL0InClockRate(void)
{
    uint32_t clkRate = 0U;

    switch ((SCG0->APLLCTRL & SCG_APLLCTRL_SOURCE_MASK) >> SCG_APLLCTRL_SOURCE_SHIFT)
    {
        case 0x00U:
            clkRate = CLOCK_GetExtClkFreq();
            break;
        case 0x01U:
            clkRate = CLOCK_GetClk48MFreq();
            break;
        case 0x02U:
            clkRate = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        default:
            clkRate = 0U;
            break;
    }

    return clkRate;
}

/* Return PLL1 input clock rate */
uint32_t CLOCK_GetPLL1InClockRate(void)
{
    uint32_t clkRate = 0U;

    switch ((SCG0->SPLLCTRL & SCG_SPLLCTRL_SOURCE_MASK) >> SCG_SPLLCTRL_SOURCE_SHIFT)
    {
        case 0x00U:
            clkRate = CLOCK_GetExtClkFreq();
            break;
        case 0x01U:
            clkRate = CLOCK_GetClk48MFreq();
            break;
        case 0x02U:
            clkRate = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        default:
            clkRate = 0U;
            break;
    }

    return clkRate;
}

/* Return PLL output clock rate from setup structure */
/*! brief    Return PLL0 output clock rate from setup structure
 *  param    pSetup : Pointer to a PLL setup structure
 *  return   PLL0 output clock rate the setup structure will generate
 */
uint32_t CLOCK_GetPLLOutFromSetup(pll_setup_t *pSetup)
{
    uint32_t clkRate = 0;
    uint32_t prediv, postdiv;
    float workRate = 0.0F;

    /* Get the input clock frequency of PLL. */
    clkRate = CLOCK_GetPLLInClockRateFromSetup(pSetup);

    prediv  = findPllPreDivFromSetup(pSetup);
    postdiv = findPllPostDivFromSetup(pSetup);
    /* Adjust input clock */
    clkRate = clkRate / prediv;
    /* MDEC used for rate */
    workRate = (float)clkRate * (float)findPllMMultFromSetup(pSetup);
    workRate /= (float)postdiv;

    return (uint32_t)workRate;
}

/* Set PLL output based on the passed PLL setup data */
/*! brief    Set PLL output based on the passed PLL setup data
 *  param    pControl    : Pointer to populated PLL control structure to generate setup with
 *  param    pSetup      : Pointer to PLL setup structure to be filled
 *  return   PLL_ERROR_SUCCESS on success, or PLL setup error code
 *  note Actual frequency for setup may vary from the desired frequency based on the
 *  accuracy of input clocks, rounding, non-fractional PLL mode, etc.
 */
pll_error_t CLOCK_SetupPLLData(pll_config_t *pControl, pll_setup_t *pSetup)
{
    uint32_t inRate;
    bool useSS = (bool)((pControl->flags & PLL_CONFIGFLAG_FORCENOFRACT) == 0UL);

    pll_error_t pllError;

    /* Get PLL Input Clock Rate */
    switch (pControl->inputSource)
    {
        case (uint32_t)kPll_ClkSrcSysOsc:
            inRate = CLOCK_GetExtClkFreq();
            break;
        case (uint32_t)kPll_ClkSrcFirc:
            inRate = CLOCK_GetClk48MFreq();
            break;
        case (uint32_t)kPll_ClkSrcRosc:
            inRate = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        default:
            inRate = 0U;
            break;
    }

    /* PLL flag options */
    pllError = CLOCK_GetPllConfig(inRate, pControl->desiredRate, pSetup, useSS);
    pSetup->pllctrl |= (uint32_t)pControl->inputSource;
    if ((useSS) && (pllError == kStatus_PLL_Success))
    {
        /* If using SS mode, then some tweaks are made to the generated setup */
        pSetup->pllsscg[1] |= (uint32_t)pControl->ss_mf | (uint32_t)pControl->ss_mr | (uint32_t)pControl->ss_mc;
        if (pControl->mfDither)
        {
            pSetup->pllsscg[1] |= (1UL << SCG_APLLSSCG1_DITHER_SHIFT);
        }
    }

    return pllError;
}

/* Setup PLL Frequency from pre-calculated value */
/**
 * brief Set PLL0 output from PLL setup structure (precise frequency)
 * param pSetup  : Pointer to populated PLL setup structure
 * return    kStatus_PLL_Success on success, or PLL setup error code
 * note  This function will power off the PLL, setup the PLL with the
 * new setup data, and then optionally powerup the PLL, wait for PLL lock,
 * and adjust system voltages to the new PLL rate. The function will not
 * alter any source clocks (ie, main systen clock) that may use the PLL,
 * so these should be setup prior to and after exiting the function.
 */
pll_error_t CLOCK_SetPLL0Freq(const pll_setup_t *pSetup)
{
    uint32_t inRate, clkRate, prediv;

    /* Enable LDO */
    SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;

    /* Power off PLL0 and disable PLL0 clock during setup changes */
    SCG0->APLLCSR &= ~(SCG_APLLCSR_APLLPWREN_MASK | SCG_APLLCSR_APLLCLKEN_MASK);

    /* Write PLL setup data */
    SCG0->APLLCTRL  = pSetup->pllctrl;
    SCG0->APLLNDIV  = pSetup->pllndiv;
    SCG0->APLLNDIV  = pSetup->pllndiv | (1UL << SCG_APLLNDIV_NREQ_SHIFT); /* latch */
    SCG0->APLLPDIV  = pSetup->pllpdiv;
    SCG0->APLLPDIV  = pSetup->pllpdiv | (1UL << SCG_APLLPDIV_PREQ_SHIFT); /* latch */
    SCG0->APLLMDIV  = pSetup->pllmdiv;
    SCG0->APLLMDIV  = pSetup->pllmdiv | (1UL << SCG_APLLMDIV_MREQ_SHIFT); /* latch */
    SCG0->APLLSSCG0 = pSetup->pllsscg[0];
    SCG0->APLLSSCG1 = pSetup->pllsscg[1];

    /* Unlock APLLLOCK_CNFG register */
    SCG0->TRIM_LOCK = 0x5a5a0001;

    /* Configure lock time of APLL stable, value = 500us/x+300, where x is the period of clk_ref (clk_in/N). */
    inRate = CLOCK_GetPLL0InClockRate();
    prediv = findPll0PreDiv();
    /* Adjust input clock */
    clkRate             = inRate / prediv;
    SCG0->APLLLOCK_CNFG = SCG_APLLLOCK_CNFG_LOCK_TIME(clkRate / 2000U + 300U);

    /* Power on PLL0 and enable PLL0 clock */
    SCG0->APLLCSR |= (SCG_APLLCSR_APLLPWREN_MASK | SCG_APLLCSR_APLLCLKEN_MASK);

    /* Wait for APLL lock */
    while (CLOCK_IsPLL0Locked() == false)
    {
    }

    if (pSetup->pllRate != CLOCK_GetPll0OutFreq())
    {
        return kStatus_PLL_OutputError;
    }

    return kStatus_PLL_Success;
}

/* Setup PLL1 Frequency from pre-calculated value */
/**
 * brief Set PLL1 output from PLL setup structure (precise frequency)
 * param pSetup  : Pointer to populated PLL setup structure
 * return    kStatus_PLL_Success on success, or PLL setup error code
 * note  This function will power off the PLL, setup the PLL with the
 * new setup data, and then optionally powerup the PLL, wait for PLL lock,
 * and adjust system voltages to the new PLL rate. The function will not
 * alter any source clocks (ie, main systen clock) that may use the PLL,
 * so these should be setup prior to and after exiting the function.
 */
pll_error_t CLOCK_SetPLL1Freq(const pll_setup_t *pSetup)
{
    uint32_t inRate, clkRate, prediv;

    /* Enable LDO */
    SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;

    /* Power off PLL1 and disable PLL1 clock during setup changes */
    SCG0->SPLLCSR &= ~(SCG_SPLLCSR_SPLLPWREN_MASK | SCG_SPLLCSR_SPLLCLKEN_MASK);

    /* Write PLL setup data */
    SCG0->SPLLCTRL  = pSetup->pllctrl;
    SCG0->SPLLNDIV  = pSetup->pllndiv;
    SCG0->SPLLNDIV  = pSetup->pllndiv | (1UL << SCG_SPLLNDIV_NREQ_SHIFT); /* latch */
    SCG0->SPLLPDIV  = pSetup->pllpdiv;
    SCG0->SPLLPDIV  = pSetup->pllpdiv | (1UL << SCG_SPLLPDIV_PREQ_SHIFT); /* latch */
    SCG0->SPLLMDIV  = pSetup->pllmdiv;
    SCG0->SPLLMDIV  = pSetup->pllmdiv | (1UL << SCG_SPLLMDIV_MREQ_SHIFT); /* latch */
    SCG0->SPLLSSCG0 = pSetup->pllsscg[0];
    SCG0->SPLLSSCG1 = pSetup->pllsscg[1];

    /* Unlock SPLLLOCK_CNFG register */
    SCG0->TRIM_LOCK = 0x5a5a0001;

    /* Configure lock time of APLL stable, value = 500μs/x+300, where x is the period of clk_ref (clk_in/N). */
    inRate = CLOCK_GetPLL1InClockRate();
    prediv = findPll1PreDiv();
    /* Adjust input clock */
    clkRate             = inRate / prediv;
    SCG0->SPLLLOCK_CNFG = SCG_SPLLLOCK_CNFG_LOCK_TIME(clkRate / 2000U + 300U);

    /* Power on PLL1 and enable PLL1 clock */
    SCG0->SPLLCSR |= (SCG_SPLLCSR_SPLLPWREN_MASK | SCG_SPLLCSR_SPLLCLKEN_MASK);

    /* Wait for APLL lock */
    while (CLOCK_IsPLL1Locked() == false)
    {
    }

    if (pSetup->pllRate != CLOCK_GetPll1OutFreq())
    {
        return kStatus_PLL_OutputError;
    }

    return kStatus_PLL_Success;
}

/*! @brief Enable the OSTIMER 32k clock.
 *  @return  Nothing
 */
void CLOCK_EnableOstimer32kClock(void)
{
    // PMC->OSEVENTTIMER |= PMC_OSEVENTTIMER_CLOCKENABLE_MASK;
}

/* Get FRO 12M Clk */
/*! brief  Return Frequency of FRO 12MHz
 *  return Frequency of FRO 12MHz
 */
static uint32_t CLOCK_GetFro12MFreq(void)
{
    return ((SCG0->SIRCCSR & SCG_SIRCCSR_SIRC_CLK_PERIPH_EN_MASK) != 0UL) ? 12000000U : 0U;
}

/* Get CLK 1M Clk */
/*! brief  Return Frequency of CLK 1MHz
 *  return Frequency of CLK 1MHz
 */
static uint32_t CLOCK_GetClk1MFreq(void)
{
    return 1000000U;
}

/* Get HF FRO Clk */
/*! brief  Return Frequency of High-Freq output of FRO
 *  return Frequency of High-Freq output of FRO
 */
static uint32_t CLOCK_GetFroHfFreq(void)
{
    uint32_t freq;

    if ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRCEN_MASK) == 0UL)
    {
        freq = 0;
    }
    else if ((SCG0->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) != 0UL)
    {
        freq = 144000000U;
    }
    else
    {
        freq = 48000000U;
    }

    return freq;
}

/* Get CLK 48M Clk */
/*! brief  Return Frequency of CLK 48MHz
 *  return Frequency of CLK 48MHz
 */
static uint32_t CLOCK_GetClk48MFreq(void)
{
    return ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRC_SCLK_PERIPH_EN_MASK) != 0U) ? 48000000U : 0U;
}

/* Get CLK 144M Clk */
/*! brief  Return Frequency of CLK 144MHz
 *  return Frequency of CLK 144MHz
 */
static uint32_t CLOCK_GetClk144MFreq(void)
{
    return ((SCG0->FIRCCSR & SCG_FIRCCSR_FIRC_FCLK_PERIPH_EN_MASK) != 0U) ? 144000000U : 0U;
}

/* Get CLK 16K Clk */
/*! brief  Return Frequency of CLK 16KHz
 *  return Frequency of CLK 16KHz
 */
static uint32_t CLOCK_GetClk16KFreq(uint32_t id)
{
    return ((VBAT0->FROCTLA & VBAT_FROCTLA_FRO_EN_MASK) != 0U) ?
               (((VBAT0->FROCLKE & VBAT_FROCLKE_CLKE(id)) != 0UL) ? 16000U : 0U) :
               0U;
}

/* Get EXT OSC Clk */
/*! brief  Return Frequency of External Clock
 *  return Frequency of External Clock. If no external clock is used returns 0.
 */
static uint32_t CLOCK_GetExtClkFreq(void)
{
    return ((SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) != 0UL) ? s_Ext_Clk_Freq : 0U;
}

/* Get RTC OSC Clk */
/*! brief  Return Frequency of 32kHz osc
 *  return Frequency of 32kHz osc
 */
static uint32_t CLOCK_GetOsc32KFreq(uint32_t id)
{
    return ((SCG0->ROSCCSR & SCG_ROSCCSR_ROSCVLD_MASK) != 0UL) ?
               (((VBAT0->OSCCLKE & VBAT_OSCCLKE_CLKE(id)) != 0UL) ? s_Xtal32_Freq : 0U) :
               0U;
}

/* Get MAIN Clk */
/*! @brief  Return Frequency of main
 *  @return Frequency of the main
 */
uint32_t CLOCK_GetMainClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((SCG0->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT)
    {
        case 1U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 2U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToWake);
            break;
        case 5U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 6U:
            freq = CLOCK_GetPll1OutFreq();
            break;
        case 7U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get cpu Clk */
/*! brief  Return Frequency of Core System
 *  return Frequency of Core System
 */
uint32_t CLOCK_GetCoreSysClkFreq(void)
{
    uint32_t freq = 0U;

    freq = CLOCK_GetMainClkFreq() / ((SYSCON->AHBCLKDIV & 0xffU) + 1U);

    return freq;
}

/* Get Systick Clk */
/*! brief  Return Frequency of SystickClock
 *  return Frequency of Systick Clock
 */
static uint32_t CLOCK_GetSystickClkFreq(uint32_t id)
{
    uint32_t freq = 0U;

    switch ((id == 0U) ? SYSCON->SYSTICKCLKSEL0 : SYSCON->SYSTICKCLKSEL1)
    {
        case 0U:
            freq = CLOCK_GetMainClkFreq() / (((SYSCON->SYSTICKCLKDIV[id]) & 0xffU) + 1U);
            break;
        case 1U:
            freq = CLOCK_GetClk1MFreq();
            break;
        case 2U:
            freq = CLOCK_GetLposcFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get CLOCK OUT Clk */
/*! brief  Return Frequency of ClockOut
 *  return Frequency of ClockOut
 */
static uint32_t CLOCK_GetClockOutClkFreq(void)
{
    uint32_t freq = 0U;

    switch (SYSCON->CLKOUTSEL)
    {
        case 0U:
            freq = CLOCK_GetMainClkFreq();
            break;
        case 1U:
            freq = CLOCK_GetPll0OutFreq();
            break;
        case 2U:
            freq = CLOCK_GetExtClkFreq();
            break;
        case 3U:
            freq = CLOCK_GetFroHfFreq();
            break;
        case 4U:
            freq = CLOCK_GetFro12MFreq();
            break;
        case 5U:
            freq = CLOCK_GetPll1OutFreq() / (((SYSCON->PLL1CLK0DIV) & 0xffU) + 1U);
            break;
        case 6U:
            freq = CLOCK_GetLposcFreq();
            break;
        case 7U:
            // freq = CLOCK_GetUPllOutFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq / ((SYSCON->CLKOUTDIV & 0xffU) + 1U);
}

/* Get LP_OSC Clk */
/*! brief  Return Frequency of LP_OSC
 *  return Frequency of LP_OSC
 */
static uint32_t CLOCK_GetLposcFreq(void)
{
    uint32_t freq = 0U;

    switch ((RTC0->CTRL & RTC_CTRL_CLK_SEL_MASK) >> RTC_CTRL_CLK_SEL_SHIFT)
    {
        case 1U:
            freq = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        case 2U:
            freq = CLOCK_GetClk16KFreq((uint32_t)kCLOCK_Clk16KToVbat);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Find SELP, SELI, and SELR values for raw M value, max M = MVALMAX */
static void pllFindSel(uint32_t M, uint32_t *pSelP, uint32_t *pSelI, uint32_t *pSelR)
{
    uint32_t seli, selp;
    /* bandwidth: compute selP from Multiplier */
    if ((SCG0->APLLCTRL & SCG_APLLCTRL_LIMUPOFF_MASK) == 0UL) /* normal mode */
    {
        selp = (M >> 2U) + 1U;
        if (selp >= 31U)
        {
            selp = 31U;
        }
        *pSelP = selp;

        if (M >= 8000UL)
        {
            seli = 1UL;
        }
        else if (M >= 122UL)
        {
            seli = (uint32_t)(8000UL / M); /*floor(8000/M) */
        }
        else
        {
            seli = 2UL * ((uint32_t)(M / 4UL)) + 3UL; /* 2*floor(M/4) + 3 */
        }

        if (seli >= 63UL)
        {
            seli = 63UL;
        }
        *pSelI = seli;

        *pSelR = 0U;
    }
    else
    {
        /* Note: If the spread spectrum and fractional mode, choose N to ensure 3 MHz < Fin/N < 5 MHz */
        *pSelP = 3U;
        *pSelI = 4U;
        *pSelR = 4U;
    }
}

/* Get predivider (N) from PLL0 NDIV setting */
static uint32_t findPll0PreDiv(void)
{
    uint32_t preDiv = 1UL;

    /* Direct input is not used? */
    if ((SCG0->APLLCTRL & SCG_APLLCTRL_BYPASSPREDIV_MASK) == 0UL)
    {
        preDiv = SCG0->APLLNDIV & SCG_APLLNDIV_NDIV_MASK;
        if (preDiv == 0UL)
        {
            preDiv = 1UL;
        }
    }
    return preDiv;
}

/* Get predivider (N) from PLL1 NDIV setting */
static uint32_t findPll1PreDiv(void)
{
    uint32_t preDiv = 1UL;

    /* Direct input is not used? */
    if ((SCG0->SPLLCTRL & SCG_SPLLCTRL_BYPASSPREDIV_MASK) == 0UL)
    {
        preDiv = SCG0->SPLLNDIV & SCG_SPLLNDIV_NDIV_MASK;
        if (preDiv == 0UL)
        {
            preDiv = 1UL;
        }
    }
    return preDiv;
}

/* Get postdivider (P) from PLL0 PDIV setting */
static uint32_t findPll0PostDiv(void)
{
    uint32_t postDiv = 1UL;

    if ((SCG0->APLLCTRL & SCG_APLLCTRL_BYPASSPOSTDIV_MASK) == 0UL)
    {
        if ((SCG0->APLLCTRL & SCG_APLLCTRL_BYPASSPOSTDIV2_MASK) != 0UL)
        {
            postDiv = SCG0->APLLPDIV & SCG_APLLPDIV_PDIV_MASK;
        }
        else
        {
            postDiv = 2UL * (SCG0->APLLPDIV & SCG_APLLPDIV_PDIV_MASK);
        }
        if (postDiv == 0UL)
        {
            postDiv = 2UL;
        }
    }

    return postDiv;
}

/* Get postdivider (P) from PLL1 PDIV setting. */
static uint32_t findPll1PostDiv(void)
{
    uint32_t postDiv = 1UL;

    if ((SCG0->SPLLCTRL & SCG_SPLLCTRL_BYPASSPOSTDIV_MASK) == 0UL)
    {
        if ((SCG0->SPLLCTRL & SCG_SPLLCTRL_BYPASSPOSTDIV2_MASK) != 0UL)
        {
            postDiv = SCG0->SPLLPDIV & SCG_SPLLPDIV_PDIV_MASK;
        }
        else
        {
            postDiv = 2UL * (SCG0->SPLLPDIV & SCG_SPLLPDIV_PDIV_MASK);
        }
        if (postDiv == 0UL)
        {
            postDiv = 2UL;
        }
    }

    return postDiv;
}

/* Get multiplier (M) from PLL0 SSCG and SEL_EXT settings */
static float findPll0MMult(void)
{
    float mMult = 1.0F;
    float mMult_fract;
    uint32_t mMult_int;

    if ((SCG0->APLLSSCG1 & SCG_APLLSSCG1_SEL_SS_MDIV_MASK) == 0UL)
    {
        mMult = (float)(uint32_t)(SCG0->APLLMDIV & SCG_APLLMDIV_MDIV_MASK);
    }
    else
    {
        mMult_int = ((SCG0->APLLSSCG1 & SCG_APLLSSCG1_SS_MDIV_MSB_MASK) << 7U);
        mMult_int = mMult_int | ((SCG0->APLLSSCG0) >> PLL_SSCG_MD_INT_P);
        mMult_fract =
            ((float)(uint32_t)((SCG0->APLLSSCG0) & PLL_SSCG_MD_FRACT_M) / (float)(uint32_t)(1UL << PLL_SSCG_MD_INT_P));
        mMult = (float)mMult_int + mMult_fract;
    }
    if (0ULL == ((uint64_t)mMult))
    {
        mMult = 1.0F;
    }
    return mMult;
}

/* Get multiplier (M) from PLL1 MDEC. */
static float findPll1MMult(void)
{
    float mMult = 1.0F;
    float mMult_fract;
    uint32_t mMult_int;

    if ((SCG0->SPLLSSCG1 & SCG_SPLLSSCG1_SEL_SS_MDIV_MASK) == 0UL)
    {
        mMult = (float)(uint32_t)(SCG0->SPLLMDIV & SCG_SPLLMDIV_MDIV_MASK);
    }
    else
    {
        mMult_int = ((SCG0->SPLLSSCG1 & SCG_SPLLSSCG1_SS_MDIV_MSB_MASK) << 7U);
        mMult_int = mMult_int | ((SCG0->SPLLSSCG0) >> PLL_SSCG_MD_INT_P);
        mMult_fract =
            ((float)(uint32_t)((SCG0->SPLLSSCG0) & PLL_SSCG_MD_FRACT_M) / (float)(uint32_t)(1UL << PLL_SSCG_MD_INT_P));
        mMult = (float)mMult_int + mMult_fract;
    }
    if (0ULL == ((uint64_t)mMult))
    {
        mMult = 1.0F;
    }
    return mMult;
}

/* Find greatest common divisor between m and n */
static uint32_t FindGreatestCommonDivisor(uint32_t m, uint32_t n)
{
    uint32_t tmp;

    while (n != 0U)
    {
        tmp = n;
        n   = m % n;
        m   = tmp;
    }

    return m;
}

#if (defined(CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT) && CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT)
/* Alloct the static buffer for cache. */
static pll_setup_t s_PllSetupCacheStruct[CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT];
static uint32_t s_FinHzCache[CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT]  = {0};
static uint32_t s_FoutHzCache[CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT] = {0};
static bool s_UseSSCache[CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT]      = {false};
static uint32_t s_PllSetupCacheIdx                                  = 0U;
#endif /* CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT */

/*
 * Calculate the PLL setting values from input clock freq to output freq.
 */
static pll_error_t CLOCK_GetPllConfig(uint32_t finHz, uint32_t foutHz, pll_setup_t *pSetup, bool useSS)
{
    pll_error_t retErr;
#if (defined(CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT) && CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT)
    uint32_t i;

    for (i = 0U; i < CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT; i++)
    {
        if ((finHz == s_FinHzCache[i]) && (foutHz == s_FoutHzCache[i]) && (useSS == s_UseSSCache[i]))
        {
            /* Hit the target in cache buffer. */
            pSetup->pllctrl    = s_PllSetupCacheStruct[i].pllctrl;
            pSetup->pllndiv    = s_PllSetupCacheStruct[i].pllndiv;
            pSetup->pllmdiv    = s_PllSetupCacheStruct[i].pllmdiv;
            pSetup->pllpdiv    = s_PllSetupCacheStruct[i].pllpdiv;
            pSetup->pllsscg[0] = s_PllSetupCacheStruct[i].pllsscg[0];
            pSetup->pllsscg[1] = s_PllSetupCacheStruct[i].pllsscg[1];
            retErr             = kStatus_PLL_Success;
            break;
        }
    }

    if (i < CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT)
    {
        return retErr;
    }
#endif /* CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT */

    retErr = CLOCK_GetPllConfigInternal(finHz, foutHz, pSetup, useSS);

#if (defined(CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT) && CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT)
    /* Cache the most recent calulation result into buffer. */
    s_FinHzCache[s_PllSetupCacheIdx]  = finHz;
    s_FoutHzCache[s_PllSetupCacheIdx] = foutHz;
    s_UseSSCache[s_PllSetupCacheIdx]  = useSS;

    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllctrl    = pSetup->pllctrl;
    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllndiv    = pSetup->pllndiv;
    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllmdiv    = pSetup->pllmdiv;
    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllpdiv    = pSetup->pllpdiv;
    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllsscg[0] = pSetup->pllsscg[0];
    s_PllSetupCacheStruct[s_PllSetupCacheIdx].pllsscg[1] = pSetup->pllsscg[1];
    /* Update the index for next available buffer. */
    s_PllSetupCacheIdx = (s_PllSetupCacheIdx + 1U) % CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT;
#endif /* CLOCK_USR_CFG_PLL_CONFIG_CACHE_COUNT */

    return retErr;
}

/*
 * Set PLL output based on desired output rate.
 * In this function, the it calculates the PLL0 setting for output frequency from input clock
 * frequency. The calculation would cost a few time. So it is not recommaned to use it frequently.
 * the "pllctrl", "pllndiv", "pllpdiv", "pllmdiv" would updated in this function.
 */
static pll_error_t CLOCK_GetPllConfigInternal(uint32_t finHz, uint32_t foutHz, pll_setup_t *pSetup, bool useSS)
{
    uint32_t nDivOutHz, fccoHz;
    uint32_t pllPreDivider, pllMultiplier, pllPostDivider;
    uint32_t pllDirectInput, pllDirectOutput;
    uint32_t pllSelP, pllSelI, pllSelR, uplimoff;

    /* Baseline parameters (no input or output dividers) */
    pllPreDivider   = 1U; /* 1 implies pre-divider will be disabled */
    pllPostDivider  = 1U; /* 1 implies post-divider will be disabled */
    pllDirectOutput = 1U;

    /* Verify output rate parameter */
    if (foutHz > PLL_MAX_CCO_FREQ_MHZ)
    {
        /* Maximum PLL output with post divider=1 cannot go above this frequency */
        return kStatus_PLL_OutputTooHigh;
    }
    if (foutHz < (PLL_MIN_CCO_FREQ_MHZ / (PVALMAX << 1U)))
    {
        /* Minmum PLL output with maximum post divider cannot go below this frequency */
        return kStatus_PLL_OutputTooLow;
    }

    /* If using SS mode, input clock needs to be between 3MHz and 20MHz */
    if (useSS)
    {
        /* Verify input rate parameter */
        if (finHz < PLL_MIN_IN_SSMODE)
        {
            /* Input clock into the PLL cannot be lower than this */
            return kStatus_PLL_InputTooLow;
        }
        /* PLL input in SS mode must be under 20MHz */
        if (finHz > (PLL_MAX_IN_SSMODE * NVALMAX))
        {
            return kStatus_PLL_InputTooHigh;
        }
    }
    else
    {
        /* Verify input rate parameter */
        if (finHz < PLL_LOWER_IN_LIMIT)
        {
            /* Input clock into the PLL cannot be lower than this */
            return kStatus_PLL_InputTooLow;
        }
        if (finHz > PLL_HIGHER_IN_LIMIT)
        {
            /* Input clock into the PLL cannot be higher than this */
            return kStatus_PLL_InputTooHigh;
        }
    }

    /* Find the optimal CCO frequency for the output and input that
       will keep it inside the PLL CCO range. This may require
       tweaking the post-divider for the PLL. */
    fccoHz = foutHz;
    while (fccoHz < PLL_MIN_CCO_FREQ_MHZ)
    {
        /* CCO output is less than minimum CCO range, so the CCO output
           needs to be bumped up and the post-divider is used to bring
           the PLL output back down. */
        pllPostDivider++;
        if (pllPostDivider > PVALMAX)
        {
            return kStatus_PLL_OutsideIntLimit;
        }

        /* Target CCO goes up, PLL output goes down */
        /* divide-by-2 divider in the post-divider is always work*/
        fccoHz          = foutHz * (pllPostDivider * 2U);
        pllDirectOutput = 0U;
    }

    /* Determine if a pre-divider is needed to get the best frequency */
    if ((finHz > PLL_LOWER_IN_LIMIT) && (fccoHz >= finHz) && (useSS == false))
    {
        uint32_t a = FindGreatestCommonDivisor(fccoHz, finHz);

        if (a > PLL_LOWER_IN_LIMIT)
        {
            a = finHz / a;
            if ((a != 0U) && (a < PLL_MAX_N_DIV))
            {
                pllPreDivider = a;
            }
        }
    }

    /* Bypass pre-divider hardware if pre-divider is 1 */
    if (pllPreDivider > 1U)
    {
        pllDirectInput = 0U;
    }
    else
    {
        pllDirectInput = 1U;
    }

    /* Determine PLL multipler */
    nDivOutHz     = (finHz / pllPreDivider);
    pllMultiplier = (fccoHz / nDivOutHz);

    /* Find optimal values for filter */
    if (useSS == false)
    {
        /* Will bumping up M by 1 get us closer to the desired CCO frequency? */
        if ((nDivOutHz * ((pllMultiplier * 2U) + 1U)) < (fccoHz * 2U))
        {
            pllMultiplier++;
        }

        /* Setup filtering */
        pllFindSel(pllMultiplier, &pllSelP, &pllSelI, &pllSelR);
        uplimoff = 0U;

        /* Get encoded value for M (mult) and use manual filter, disable SS mode */
        pSetup->pllmdiv = (uint32_t)PLL_MDIV_VAL_SET(pllMultiplier);
        pSetup->pllsscg[1] &= ~SCG_APLLSSCG1_SEL_SS_MDIV_MASK;
    }
    else
    {
        uint64_t fc;

        /* Filtering will be handled by SSC */
        pllSelR  = 0UL;
        pllSelI  = 0UL;
        pllSelP  = 0UL;
        uplimoff = 1U;

        /* The PLL multiplier will get very close and slightly under the
           desired target frequency. A small fractional component can be
           added to fine tune the frequency upwards to the target. */
        fc = ((uint64_t)(uint32_t)(fccoHz % nDivOutHz) << 25UL) / nDivOutHz;

        /* Set multiplier */
        pSetup->pllsscg[0] = (uint32_t)(PLL_SSCG_MD_INT_SET(pllMultiplier) | PLL_SSCG_MD_FRACT_SET((uint32_t)fc));
        pSetup->pllsscg[1] = (uint32_t)(PLL_SSCG_MD_INT_SET(pllMultiplier) >> 32U) | SCG_APLLSSCG1_SEL_SS_MDIV_MASK;
    }

    /* Get encoded values for N (prediv) and P (postdiv) */
    pSetup->pllndiv = PLL_NDIV_VAL_SET(pllPreDivider);
    pSetup->pllpdiv = PLL_PDIV_VAL_SET(pllPostDivider);

    /* PLL control */
    pSetup->pllctrl = (pllSelR << SCG_APLLCTRL_SELR_SHIFT) |                 /* Filter coefficient */
                      (pllSelI << SCG_APLLCTRL_SELI_SHIFT) |                 /* Filter coefficient */
                      (pllSelP << SCG_APLLCTRL_SELP_SHIFT) |                 /* Filter coefficient */
                      (uplimoff << SCG_APLLCTRL_LIMUPOFF_SHIFT) |            /* SS/fractional mode disabled */
                      (pllDirectInput << SCG_APLLCTRL_BYPASSPREDIV_SHIFT) |  /* Bypass pre-divider? */
                      (pllDirectOutput << SCG_APLLCTRL_BYPASSPOSTDIV_SHIFT); /* Bypass post-divider? */

    return kStatus_PLL_Success;
}

/* Get PLL input clock rate from setup structure */
static uint32_t CLOCK_GetPLLInClockRateFromSetup(pll_setup_t *pSetup)
{
    uint32_t clkRate = 0U;

    switch ((pSetup->pllctrl & SCG_APLLCTRL_SOURCE_MASK) >> SCG_APLLCTRL_SOURCE_SHIFT)
    {
        case 0x00U:
            clkRate = CLOCK_GetExtClkFreq();
            break;
        case 0x01U:
            clkRate = CLOCK_GetClk48MFreq();
            break;
        case 0x02U:
            clkRate = CLOCK_GetOsc32KFreq((uint32_t)kCLOCK_Osc32kToVbat);
            break;
        default:
            clkRate = 0U;
            break;
    }

    return clkRate;
}

/* Get predivider (N) from from setup structure */
static uint32_t findPllPreDivFromSetup(pll_setup_t *pSetup)
{
    uint32_t preDiv = 1UL;

    /* Direct input is not used? */
    if ((pSetup->pllctrl & SCG_APLLCTRL_BYPASSPREDIV_MASK) == 0UL)
    {
        preDiv = pSetup->pllndiv & SCG_APLLNDIV_NDIV_MASK;
        if (preDiv == 0UL)
        {
            preDiv = 1UL;
        }
    }
    return preDiv;
}

/* Get postdivider (P) from from setup structure */
static uint32_t findPllPostDivFromSetup(pll_setup_t *pSetup)
{
    uint32_t postDiv = 1UL;

    if ((pSetup->pllctrl & SCG_APLLCTRL_BYPASSPOSTDIV_MASK) == 0UL)
    {
        if ((pSetup->pllctrl & SCG_APLLCTRL_BYPASSPOSTDIV2_MASK) != 0UL)
        {
            postDiv = pSetup->pllpdiv & SCG_APLLPDIV_PDIV_MASK;
        }
        else
        {
            postDiv = 2UL * (pSetup->pllpdiv & SCG_APLLPDIV_PDIV_MASK);
        }
        if (postDiv == 0UL)
        {
            postDiv = 2UL;
        }
    }

    return postDiv;
}

/* Get multiplier (M) from from setup structure */
static float findPllMMultFromSetup(pll_setup_t *pSetup)
{
    float mMult = 1.0F;
    float mMult_fract;
    uint32_t mMult_int;

    if ((pSetup->pllsscg[1] & SCG_APLLSSCG1_SEL_SS_MDIV_MASK) == 0UL)
    {
        mMult = (float)(uint32_t)(pSetup->pllmdiv & SCG_APLLMDIV_MDIV_MASK);
    }
    else
    {
        mMult_int   = ((pSetup->pllsscg[1] & SCG_APLLSSCG1_SS_MDIV_MSB_MASK) << 7U);
        mMult_int   = mMult_int | ((pSetup->pllsscg[0]) >> PLL_SSCG_MD_INT_P);
        mMult_fract = ((float)(uint32_t)((pSetup->pllsscg[0]) & PLL_SSCG_MD_FRACT_M) /
                       (float)(uint32_t)(1UL << PLL_SSCG_MD_INT_P));
        mMult       = (float)mMult_int + mMult_fract;
    }
    if (0ULL == ((uint64_t)mMult))
    {
        mMult = 1.0F;
    }
    return mMult;
}

/*! brief Enable USB FS clock.
 * Enable USB Full Speed clock.
 */
bool CLOCK_EnableUsbfsClock(void)
{
    SYSCON->USB0CLKSEL = 0x3U; /* Clk 48 MHz clock */
    CLOCK_SetClkDiv(kCLOCK_DivUsb0Clk, 1);
    SYSCON->USB0CLKDIV &= (uint32_t) ~(SYSCON_USB0CLKDIV_HALT_MASK | SYSCON_USB0CLKDIV_RESET_MASK);
    /* Wait until clock change completes */
    while ((SYSCON->USB0CLKDIV & SYSCON_SYSTICKCLKDIV_UNSTAB_MASK) != 0U)
    {
    }
    return true;
}

/*! brief Enable USB HS PHY PLL clock.
 *
 * This function enables the internal 480MHz USB PHY PLL clock.
 *
 * param src  USB HS PHY PLL clock source.
 * param freq The frequency specified by src.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhsPhyPllClock(clock_usb_phy_src_t src, uint32_t freq)
{
    uint32_t phyPllDiv  = 0U;
    uint16_t multiplier = 0U;
    bool err            = false;

    USBPHY->CTRL_CLR    = USBPHY_CTRL_SFTRST_MASK;
    USBPHY->ANACTRL_SET = USBPHY_ANACTRL_LVI_EN_MASK;
    USBPHY->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_POWER(1) | USBPHY_PLL_SIC_PLL_REG_ENABLE_MASK);
    if ((480000000UL % freq) != 0UL)
    {
        return false;
    }
    multiplier = (uint16_t)(480000000UL / freq);

    switch (multiplier)
    {
        case 15:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(0U);
            break;
        }
        case 16:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(1U);
            break;
        }
        case 20:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(2U);
            break;
        }
        case 22:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(3U);
            break;
        }
        case 24:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(4U);
            break;
        }
        case 25:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(5U);
            break;
        }
        case 30:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(6U);
            break;
        }
        case 40:
        {
            phyPllDiv = USBPHY_PLL_SIC_PLL_DIV_SEL(7U);
            break;
        }
        default:
        {
            err = true;
            break;
        }
    }

    if (err)
    {
        return false;
    }

    USBPHY->PLL_SIC = (USBPHY->PLL_SIC & ~(USBPHY_PLL_SIC_PLL_DIV_SEL_MASK)) | phyPllDiv;

    USBPHY->PLL_SIC_CLR = USBPHY_PLL_SIC_PLL_BYPASS_MASK;
    USBPHY->PLL_SIC_SET = (USBPHY_PLL_SIC_PLL_EN_USB_CLKS_MASK);

    USBPHY->CTRL_CLR = USBPHY_CTRL_CLR_CLKGATE_MASK;
    USBPHY->PWD      = 0x0U;

    while (0UL == (USBPHY->PLL_SIC & USBPHY_PLL_SIC_PLL_LOCK_MASK))
    {
    }

    return true;
}

/*! brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhsPhyPllClock(void)
{
    USBPHY->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* Set to 1U to gate clocks */
}

/*! brief Enable USB HS clock.
 * retval true The clock is set successfully.
 * retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhsClock(void)
{
    USBHS1__USBC->USBCMD |= USBHS_USBCMD_RST_MASK;
    /* Add a delay between RST and RS so make sure there is a DP pullup sequence*/
    for (uint32_t i = 0; i < 400000U; i++)
    {
        __ASM("nop");
    }
    return true;
}

/**
 * @brief   FIRC Auto Trim With SOF.
 * @return  returns success or fail status.
 */
status_t CLOCK_FIRCAutoTrimWithSOF(void)
{
    /* System OSC Clock Monitor is disabled */
    CLOCK_SetSysOscMonitorMode(kSCG_SysOscMonitorDisable);

    firc_trim_config_t fircAutoTrimConfig = {
        .trimMode = kSCG_FircTrimUpdate,        /* FIRC trim is enabled and trim value update is enabled */
        .trimSrc  = kSCG_FircTrimSrcUsb0,       /* Trim source is USB0 start of frame (1kHz) */
        .trimDiv  = 1U,                         /* Divided value */
        .trimCoar = 0U,                         /* Trim value, see Reference Manual for more information */
        .trimFine = 0U,                         /* Trim value, see Reference Manual for more information */
    };
    CLOCK_FROHFTrimConfig(fircAutoTrimConfig);

    return (status_t)kStatus_Success;
}
