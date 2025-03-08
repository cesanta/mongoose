/*
 * Copyright 2020-2024 NXP
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

/* Each loop costs 3 cpu cycles */
#define CLOCK_DELAY_LOOPS(cpuFreq, delayFreq, delayCycles) \
    ((((cpuFreq) + (delayFreq)-1U) / (delayFreq)) * (delayCycles) / 3U)

#define CLOCK_KHZ(freq) ((freq)*1000UL)
#define CLOCK_MHZ(freq) (CLOCK_KHZ(freq) * 1000UL)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* External CLK_IN pin clock frequency. */
volatile uint32_t g_clkinFreq = 0U;
/* External MCLK in (mclk_in) clock frequency. If not used,
   set this to 0. Otherwise, set it to the exact rate in Hz this pin is
   being driven at.*/
volatile uint32_t g_mclkinFreq = 0U;

static const uint32_t s_avpllFreq[] = {
    0U,
    CLOCK_KHZ(2048UL),  /*!< AVPLL channel frequency 2.048MHz */
    CLOCK_KHZ(4096UL),  /*!< AVPLL channel frequency 4.096MHz */
    CLOCK_KHZ(6144UL),  /*!< AVPLL channel frequency 6.144MHz */
    CLOCK_KHZ(8192UL),  /*!< AVPLL channel frequency 8.192MHz */
    11289600UL,         /*!< AVPLL channel frequency 11.2896MHz */
    CLOCK_MHZ(12UL),    /*!< AVPLL channel frequency 12MHz */
    CLOCK_KHZ(12288UL), /*!< AVPLL channel frequency 12.288MHz */
    CLOCK_KHZ(24576UL), /*!< AVPLL channel frequency 24.576MHz */
    CLOCK_MHZ(64UL),    /*!< AVPLL channel frequency 64MHz */
    CLOCK_KHZ(98304UL), /*!< AVPLL channel frequency 98.304MHz */
};

static const uint32_t s_avpllFreqOff[] = {
    0U,       /*!< AVPLL channel unchanged */
    0x53U,    /*!< AVPLL channel frequency 2.048MHz */
    0x1050U,  /*!< AVPLL channel frequency 4.096MHz */
    0x4145DU, /*!< AVPLL channel frequency 6.144MHz */
    0x4145DU, /*!< AVPLL channel frequency 8.192MHz */
    0x38B3U,  /*!< AVPLL channel frequency 11.2896MHz */
    0U,       /*!< AVPLL channel frequency 12MHz */
    0x4145DU, /*!< AVPLL channel frequency 12.288MHz */
    0x4145DU, /*!< AVPLL channel frequency 24.576MHz */
    0xCCCDU,  /*!< AVPLL channel frequency 64MHz */
    0x4145DU, /*!< AVPLL channel frequency 98.304MHz */
};

static const uint32_t s_avpllPostDiv[] = {
    0U,   /*!< AVPLL channel unchanged */
    791U, /*!< AVPLL channel frequency 2.048MHz */
    395U, /*!< AVPLL channel frequency 4.096MHz */
    264U, /*!< AVPLL channel frequency 6.144MHz */
    198U, /*!< AVPLL channel frequency 8.192MHz */
    143U, /*!< AVPLL channel frequency 11.2896MHz */
    135U, /*!< AVPLL channel frequency 12MHz */
    132U, /*!< AVPLL channel frequency 12.288MHz */
    66U,  /*!< AVPLL channel frequency 24.576MHz */
    25U,  /*!< AVPLL channel frequency 64MHz */
    16U,  /*!< AVPLL channel frequency 98.304MHz */
};

static const uint32_t s_avpllPostDiv0p5[] = {
    0U, /*!< AVPLL channel unchanged */
    0U, /*!< AVPLL channel frequency 2.048MHz */
    0U, /*!< AVPLL channel frequency 4.096MHz */
    0U, /*!< AVPLL channel frequency 6.144MHz */
    0U, /*!< AVPLL channel frequency 8.192MHz */
    0U, /*!< AVPLL channel frequency 11.2896MHz */
    0U, /*!< AVPLL channel frequency 12MHz */
    0U, /*!< AVPLL channel frequency 12.288MHz */
    0U, /*!< AVPLL channel frequency 24.576MHz */
    0U, /*!< AVPLL channel frequency 64MHz */
    1U, /*!< AVPLL channel frequency 98.304MHz */
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Workaround iar/armgcc optimization issue */
__attribute__((__noinline__))
static void CLOCK_Delay(uint32_t loop)
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

static void CLOCK_DelayUs(uint32_t us)
{
    uint32_t instNum;

    instNum = ((SystemCoreClock + 999999UL) / 1000000UL) * us;
    CLOCK_Delay((instNum + 2U) / 3U);
}

/*! @brief  Return Frequency of t3pll_mci_48_60m_irc
 *  @return Frequency of t3pll_mci_48_60m_irc
 */
uint32_t CLOCK_GetT3PllMciIrcClkFreq(void)
{
    uint32_t freq = 0U;

    if ((SYSPLL_T3->CLKTREE_CTRL_SIX_REG & 0xFU) == 0x5U)
    {
        freq = CLOCK_MHZ(2560UL) / 43UL;
    }
    else if ((SYSPLL_T3->CLKTREE_CTRL_SIX_REG & 0xFU) == 0xAU)
    {
        freq = CLOCK_MHZ(2560UL) / 53UL;
    }
    else
    {
        /* Only 48MHz and 60MHz is allowed */
        assert(false);
    }

    return freq;
}

/*! @brief  Return Frequency of t3pll_mci_213p3m
 *  @return Frequency of t3pll_mci_213p3m
 */
uint32_t CLOCK_GetT3PllMci213mClkFreq(void)
{
    uint32_t freq = CLOCK_KHZ(213300UL);
    return freq;
}

/*! @brief  Return Frequency of t3pll_mci_256m
 *  @return Frequency of t3pll_mci_256m
 */
uint32_t CLOCK_GetT3PllMci256mClkFreq(void)
{
    uint32_t freq = CLOCK_MHZ(256UL);
    return freq;
}

/*! @brief  Return Frequency of t3pll_mci_flexspi_clk
 *  @return Frequency of t3pll_mci_flexspi_clk
 */
uint32_t CLOCK_GetT3PllMciFlexspiClkFreq(void)
{
    uint32_t freq = CLOCK_MHZ(365UL);
    ;
    return freq;
}

static uint32_t CLOCK_GetTcpuFvcoFreq(void)
{
    uint32_t freq = 0UL;
    uint32_t steps;

    steps = (SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TCPU_FBDIV_MASK) >> SYSCTL2_PLL_CTRL_TCPU_FBDIV_SHIFT;

    if ((CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_40000KHZ) && (steps >= 75UL) && (steps <= 96UL))
    {
        /* Fbdiv from 75 to 96, step 40MHz */
        steps -= 75UL;
        freq = CLOCK_MHZ(3000UL) + steps * CLOCK_MHZ(40UL);
    }
    else if ((CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_38400KHZ) && (steps >= 78UL) && (steps <= 100UL))
    {
        /* Fbdiv from 78 to 100, step 38.4MHz */
        steps -= 78UL;
        freq = CLOCK_KHZ(2995200UL) + steps * CLOCK_KHZ(38400UL);
    }
    else
    {
        assert(false);
    }

    return freq;
}

/*! @brief  Return Frequency of tcpu_mci_clk
 *  return Frequency of tcpu_mci_clk
 */
uint32_t CLOCK_GetTcpuMciClkFreq(void)
{
    uint32_t freq = CLOCK_GetTcpuFvcoFreq() / 12UL;
    return freq;
}

/*! @brief  Return Frequency of tcpu_mci_flexspi_clk
 *  @return Frequency of tcpu_mci_flexspi_clk
 */
uint32_t CLOCK_GetTcpuMciFlexspiClkFreq(void)
{
    uint32_t div =
        (SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_MASK) >> SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_SHIFT;

    return CLOCK_GetTcpuFvcoFreq() / (12UL - div);
}

/*! @brief  Return Frequency of tddr_mci_flexspi_clk
 *  @return Frequency of tddr_mci_flexspi_clk
 */
uint32_t CLOCK_GetTddrMciFlexspiClkFreq(void)
{
    uint32_t div =
        (SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_MASK) >> SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_SHIFT;
    /* TDDR FVCO fixed to 3.2GHz */
    return CLOCK_MHZ(3200UL) / (11UL - div);
}

/*! @brief  Return Frequency of tddr_mci_enet_clk
 *  @return Frequency of tddr_mci_enet_clk
 */
uint32_t CLOCK_GetTddrMciEnetClkFreq(void)
{
    uint32_t freq = CLOCK_MHZ(50UL);
    return freq;
}

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param clk  Which clock to enable, see @ref clock_ip_name_t.
 */
void CLOCK_EnableClock(clock_ip_name_t clk)
{
    uint32_t index;

    if (clk == kCLOCK_RefClkCauSlp)
    {
        PMU->CAU_SLP_CTRL &= ~PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_MASK;
        while ((PMU->CAU_SLP_CTRL & PMU_CAU_SLP_CTRL_SOC_SLP_RDY_MASK) == 0U)
        {
        }
    }
    else if (((uint32_t)clk & SYS_CLK_GATE_FLAG_MASK) != 0U)
    {
        SYSCTL2->SOURCE_CLK_GATE &= ~SYS_CLK_GATE_BIT_MASK(clk);
        /* Delay 2 40MHz cycles to make it ready. */
        CLOCK_Delay(CLOCK_DELAY_LOOPS(SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY, 40000000UL, 2U));
    }
    else
    {
        index = CLK_GATE_ABSTRACT_REG_OFFSET(clk);

        switch (index)
        {
            case CLK_CTL0_PSCCTL0:
                CLKCTL0->PSCCTL0_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL0_PSCCTL1:
                CLKCTL0->PSCCTL1_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL0_PSCCTL2:
                CLKCTL0->PSCCTL2_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL0:
                CLKCTL1->PSCCTL0_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL1:
                CLKCTL1->PSCCTL1_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL2:
                CLKCTL1->PSCCTL2_SET = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            default:
                assert(false);
                break;
        }

        if (clk == kCLOCK_Gau)
        {
            SYSCTL2->GAU_CTRL = SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN_MASK | SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN_MASK |
                                SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN_MASK | SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN_MASK |
                                SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN_MASK;
        }
    }
}

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param clk  Which clock to disable, see @ref clock_ip_name_t.
 */
void CLOCK_DisableClock(clock_ip_name_t clk)
{
    uint32_t index;

    if (clk == kCLOCK_RefClkCauSlp)
    {
        PMU->CAU_SLP_CTRL |= PMU_CAU_SLP_CTRL_CAU_SOC_SLP_CG_MASK;
    }
    else if (((uint32_t)clk & SYS_CLK_GATE_FLAG_MASK) != 0U)
    {
        SYSCTL2->SOURCE_CLK_GATE |= SYS_CLK_GATE_BIT_MASK(clk);
    }
    else
    {
        index = CLK_GATE_ABSTRACT_REG_OFFSET(clk);
        switch (index)
        {
            case CLK_CTL0_PSCCTL0:
                CLKCTL0->PSCCTL0_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL0_PSCCTL1:
                CLKCTL0->PSCCTL1_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL0_PSCCTL2:
                CLKCTL0->PSCCTL2_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL0:
                CLKCTL1->PSCCTL0_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL1:
                CLKCTL1->PSCCTL1_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            case CLK_CTL1_PSCCTL2:
                CLKCTL1->PSCCTL2_CLR = (1UL << CLK_GATE_ABSTRACT_BITS_SHIFT(clk));
                break;
            default:
                assert(false);
                break;
        }

        if (clk == kCLOCK_Gau)
        {
            SYSCTL2->GAU_CTRL = 0U;
        }
    }
}

/**
 * @brief   Configure the clock selection muxes.
 * @param   connection  : Clock to be configured.
 */
void CLOCK_AttachClk(clock_attach_id_t connection)
{
    bool final_descriptor = false;
    uint32_t i;
    volatile uint32_t *pClkSel;

    if (((uint32_t)connection & PMU_TUPLE_MUX_AVAIL) != 0U)
    {
        *PMU_TUPLE_REG(PMU, connection) = PMU_TUPLE_SEL(connection);
    }
    else if (((uint32_t)connection & CLKOUT_TUPLE_MUX_AVAIL) != 0U)
    {
        CLKCTL1->CLKOUTSEL0 = CLKCTL1_CLKOUTSEL0_SEL(((uint32_t)connection >> 4U) & 0x7U);
        CLKCTL1->CLKOUTSEL1 = CLKCTL1_CLKOUTSEL1_SEL(((uint32_t)connection >> 8U) & 0x7U);
        CLKCTL1->CLKOUTSEL2 = CLKCTL1_CLKOUTSEL2_SEL(((uint32_t)connection >> 12U) & 0x7U);
    }
    else
    {
        for (i = 0U; (i < 2U) && (!final_descriptor); i++)
        {
            connection =
                (clock_attach_id_t)(uint32_t)(((uint32_t)connection) >> (i * 16U)); /*!<  pick up next descriptor */

            if ((((uint32_t)connection) & CLKCTL1_TUPLE_FLAG_MASK) != 0UL)
            {
                pClkSel = CLKCTL_TUPLE_REG(CLKCTL1, connection);
            }
            else
            {
                pClkSel = CLKCTL_TUPLE_REG(CLKCTL0, connection);
            }

            if ((((uint32_t)connection) & 0xFFCU) != 0UL)
            {
                *pClkSel = CLKCTL_TUPLE_SEL(connection);
            }
            else
            {
                final_descriptor = true;
            }
        }
    }
}

/**
 * @brief   Setup clock dividers.
 * @param   name        : Clock divider name
 * @param   divider     : Value to be divided.
 */
void CLOCK_SetClkDiv(clock_div_name_t name, uint32_t divider)
{
    volatile uint32_t *pClkDiv;

    if ((((uint32_t)name) & CLKCTL1_TUPLE_FLAG_MASK) != 0UL)
    {
        pClkDiv = CLKCTL_TUPLE_REG(CLKCTL1, name);
    }
    else
    {
        pClkDiv = CLKCTL_TUPLE_REG(CLKCTL0, name);
    }
    /* Reset the divider counter */
    *pClkDiv |= 1UL << 29U;

    if (divider == 0U) /*!<  halt */
    {
        *pClkDiv |= 1UL << 30U;
    }
    else
    {
        *pClkDiv = divider - 1U;

        while (((*pClkDiv) & 0x80000000U) != 0UL)
        {
        }
    }
}

/*! brief  Return Frequency of selected clock
 *  return Frequency of selected clock
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName)
{
    uint32_t freq;
    switch (clockName)
    {
        case kCLOCK_CoreSysClk:
        case kCLOCK_BusClk:
            freq = CLOCK_GetCoreSysClkFreq();
            break;
        case kCLOCK_MclkClk:
            freq = CLOCK_GetMclkClkFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

/*! @brief  Return Frequency of High-Freq output of FRO
 *  @return Frequency of High-Freq output of FRO
 */
uint32_t CLOCK_GetFFroFreq(void)
{
    return CLOCK_GetT3PllMciIrcClkFreq();
}

/*! @brief  Return Frequency of SFRO
 *  @return Frequency of SFRO
 */
uint32_t CLOCK_GetSFroFreq(void)
{
    return CLOCK_GetT3PllMci256mClkFreq() / 16U;
}

/*! @brief  Return Frequency of AUDIO PLL (AVPLL CH1)
 *  @return Frequency of AUDIO PLL
 */
uint32_t CLOCK_GetAvPllCh1Freq(void)
{
    uint32_t postdiv =
        (SYSCTL2->AVPLL_CTRL5 & SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_MASK) >> SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_SHIFT;
    uint32_t i;
    uint32_t freq = 0U;

    for (i = 1U; i < ARRAY_SIZE(s_avpllPostDiv); i++)
    {
        if (s_avpllPostDiv[i] == postdiv)
        {
            freq = s_avpllFreq[i];
            break;
        }
    }

    return freq;
}

/*! @brief  Return Frequency of AVPLL CH2
 *  @return Frequency of AVPLL CH2
 */
uint32_t CLOCK_GetAvPllCh2Freq(void)
{
    uint32_t postdiv =
        (SYSCTL2->AVPLL_CTRL11 & SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_MASK) >> SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_SHIFT;
    uint32_t i;
    uint32_t freq = 0U;

    for (i = 1U; i < ARRAY_SIZE(s_avpllPostDiv); i++)
    {
        if (s_avpllPostDiv[i] == postdiv)
        {
            freq = s_avpllFreq[i];
            break;
        }
    }

    return freq;
}

static uint32_t CLOCK_GetMainPllClkFreq(void)
{
    return CLOCK_GetTcpuMciClkFreq() / ((CLKCTL0->MAINPLLCLKDIV & CLKCTL0_MAINPLLCLKDIV_DIV_MASK) + 1U);
}

static uint32_t CLOCK_GetAux0PllClkFreq(void)
{
    return CLOCK_GetTcpuMciClkFreq() / ((CLKCTL0->AUX0PLLCLKDIV & CLKCTL0_AUX0PLLCLKDIV_DIV_MASK) + 1U);
}

static uint32_t CLOCK_GetAux1PllClkFreq(void)
{
    return CLOCK_GetT3PllMci213mClkFreq() / ((CLKCTL0->AUX1PLLCLKDIV & CLKCTL0_AUX1PLLCLKDIV_DIV_MASK) + 1U);
}

static uint32_t CLOCK_GetAudioPllClkFreq(void)
{
    return CLOCK_GetAvPllCh1Freq() / ((CLKCTL1->AUDIOPLLCLKDIV & CLKCTL1_AUDIOPLLCLKDIV_DIV_MASK) + 1U);
}

/* Get MAIN Clk */
/*! @brief  Return Frequency of main clk
 *  @return Frequency of main clk
 */
uint32_t CLOCK_GetMainClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->MAINCLKSELB) & CLKCTL0_MAINCLKSELB_SEL_MASK)
    {
        case CLKCTL0_MAINCLKSELB_SEL(0):
            switch ((CLKCTL0->MAINCLKSELA) & CLKCTL0_MAINCLKSELA_SEL_MASK)
            {
                case CLKCTL0_MAINCLKSELA_SEL(0):
                    freq = CLOCK_GetSysOscFreq();
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(1):
                    freq = CLOCK_GetFFroFreq() / 4U;
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(2):
                    freq = CLOCK_GetLpOscFreq();
                    break;
                case CLKCTL0_MAINCLKSELA_SEL(3):
                    freq = CLOCK_GetFFroFreq();
                    break;
                default:
                    freq = 0U;
                    break;
            }
            break;

        case CLKCTL0_MAINCLKSELB_SEL(1):
            freq = CLOCK_GetSFroFreq();
            break;

        case CLKCTL0_MAINCLKSELB_SEL(2):
            freq = CLOCK_GetMainPllClkFreq();
            break;

        case CLKCTL0_MAINCLKSELB_SEL(3):
            freq = CLOCK_GetClk32KFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get Core/Bus Clk */
/*! @brief  Return Frequency of core/bus clk
 *  @return Frequency of core/bus clk
 */
uint32_t CLOCK_GetCoreSysClkFreq(void)
{
    return CLOCK_GetMainClkFreq() / ((CLKCTL0->SYSCPUAHBCLKDIV & CLKCTL0_SYSCPUAHBCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of systick clk
 *  @return Frequency of systick clk
 */
uint32_t CLOCK_GetSystickClkFreq(void)
{
    uint32_t freq = 0U;

    switch (CLKCTL0->SYSTICKFCLKSEL)
    {
        case CLKCTL0_SYSTICKFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq() / ((CLKCTL0->SYSTICKFCLKDIV & CLKCTL0_SYSTICKFCLKDIV_DIV_MASK) + 1U);
            break;

        case CLKCTL0_SYSTICKFCLKSEL_SEL(1):
            freq = CLOCK_GetLpOscFreq();
            break;

        case CLKCTL0_SYSTICKFCLKSEL_SEL(2):
            freq = CLOCK_GetClk32KFreq();
            break;

        case CLKCTL0_SYSTICKFCLKSEL_SEL(3):
            freq = CLOCK_GetSFroFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/* Get FRG Clk */
/*! @brief  Return Input frequency for the Fractional baud rate generator
 *  @return Input Frequency for FRG
 */
uint32_t CLOCK_GetFRGClock(uint32_t id)
{
    uint32_t freq      = 0U;
    uint32_t frgPllDiv = 1U;
    uint32_t clkSel    = 0U;
    uint32_t frgDiv    = 0U;
    uint32_t frgMul    = 0U;

    if (id <= 3UL)
    {
        clkSel = CLKCTL1->FLEXCOMM[id].FRGCLKSEL & CLKCTL1_FRGCLKSEL_SEL_MASK;
        frgMul = ((CLKCTL1->FLEXCOMM[id].FRGCTL) & CLKCTL1_FRGCTL_MULT_MASK) >> CLKCTL1_FRGCTL_MULT_SHIFT;
        frgDiv = ((CLKCTL1->FLEXCOMM[id].FRGCTL) & CLKCTL1_FRGCTL_DIV_MASK) >> CLKCTL1_FRGCTL_DIV_SHIFT;
    }
    else if (id == 14UL)
    {
        clkSel = CLKCTL1->FRG14CLKSEL & CLKCTL1_FRG14CLKSEL_SEL_MASK;
        frgMul = ((CLKCTL1->FRG14CTL) & CLKCTL1_FRGCTL_MULT_MASK) >> CLKCTL1_FRGCTL_MULT_SHIFT;
        frgDiv = ((CLKCTL1->FRG14CTL) & CLKCTL1_FRGCTL_DIV_MASK) >> CLKCTL1_FRGCTL_DIV_SHIFT;
    }
    else
    {
        assert(false);
    }

    switch (clkSel)
    {
        case CLKCTL1_FRGCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL1_FRGCLKSEL_SEL(1):
            frgPllDiv = (CLKCTL1->FRGPLLCLKDIV & CLKCTL1_FRGPLLCLKDIV_DIV_MASK) + 1U;
            freq      = CLOCK_GetMainPllClkFreq() / frgPllDiv;
            break;

        case CLKCTL1_FRGCLKSEL_SEL(2):
            freq = CLOCK_GetSFroFreq();
            break;

        case CLKCTL1_FRGCLKSEL_SEL(3):
            freq = CLOCK_GetFFroFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return (uint32_t)(((uint64_t)freq * ((uint64_t)frgDiv + 1ULL)) / (frgMul + frgDiv + 1UL));
}

/* Set FRG Clk */
/*! @brief  Set output of the Fractional baud rate generator
 * @param   config    : Configuration to set to FRGn clock.
 */
void CLOCK_SetFRGClock(const clock_frg_clk_config_t *config)
{
    uint32_t i = config->num;

    assert(i <= 14U);
    assert(config->divider == 255U); /* Always set to 0xFF to use with the fractional baudrate generator.*/

    if (i <= 3UL)
    {
        CLKCTL1->FLEXCOMM[i].FRGCLKSEL = (uint32_t)config->sfg_clock_src;
        CLKCTL1->FLEXCOMM[i].FRGCTL    = (CLKCTL1_FRGCTL_MULT(config->mult) | CLKCTL1_FRGCTL_DIV(config->divider));
    }
    else if (i == 14UL)
    {
        CLKCTL1->FRG14CLKSEL = (uint32_t)config->sfg_clock_src;
        CLKCTL1->FRG14CTL    = (CLKCTL1_FRGCTL_MULT(config->mult) | CLKCTL1_FRGCTL_DIV(config->divider));
    }
    else
    {
        assert(false);
    }
}

/*! @brief  Return Frequency of DMIC clk
 *  @return Frequency of DMIC clk
 */
uint32_t CLOCK_GetDmicClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL1->DMIC0FCLKSEL) & CLKCTL1_DMIC0FCLKSEL_SEL_MASK)
    {
        case CLKCTL1_DMIC0FCLKSEL_SEL(0):
            freq = CLOCK_GetSFroFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(1):
            freq = CLOCK_GetFFroFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(2):
            freq = CLOCK_GetAudioPllClkFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(3):
            freq = CLOCK_GetMclkInClkFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(4):
            freq = CLOCK_GetLpOscFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(5):
            freq = CLOCK_GetClk32KFreq();
            break;

        case CLKCTL1_DMIC0FCLKSEL_SEL(6):
            freq = CLOCK_GetMainClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL1->DMIC0CLKDIV & CLKCTL1_DMIC0CLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of LCD clk
 *  @return Frequency of LCD clk
 */
uint32_t CLOCK_GetLcdClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->LCDFCLKSEL) & CLKCTL0_LCDFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_LCDFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL0_LCDFCLKSEL_SEL(1):
            freq = CLOCK_GetT3PllMciFlexspiClkFreq();
            break;

        case CLKCTL0_LCDFCLKSEL_SEL(2):
            freq = CLOCK_GetTcpuMciFlexspiClkFreq();
            break;

        case CLKCTL0_LCDFCLKSEL_SEL(3):
            freq = CLOCK_GetTddrMciFlexspiClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL0->LCDFCLKDIV & CLKCTL0_LCDFCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of WDT clk
 *  @return Frequency of WDT clk
 */
uint32_t CLOCK_GetWdtClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->WDT0FCLKSEL) & CLKCTL0_WDT0FCLKSEL_SEL_MASK)
    {
        case CLKCTL0_WDT0FCLKSEL_SEL(0):
            freq = CLOCK_GetLpOscFreq();
            break;

        case CLKCTL0_WDT0FCLKSEL_SEL(1):
            freq = CLOCK_GetMainClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Return Frequency of mclk
 *  @return Frequency of mclk clk
 */
uint32_t CLOCK_GetMclkClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL1->AUDIOMCLKSEL) & CLKCTL1_AUDIOMCLKSEL_SEL_MASK)
    {
        case CLKCTL1_AUDIOMCLKSEL_SEL(0):
            freq = CLOCK_GetFFroFreq();
            break;
        case CLKCTL1_AUDIOMCLKSEL_SEL(1):
            freq = CLOCK_GetAudioPllClkFreq();
            break;
        case CLKCTL1_AUDIOMCLKSEL_SEL(2):
            freq = CLOCK_GetMainClkFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq / ((CLKCTL1->AUDIOMCLKDIV & CLKCTL1_AUDIOMCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of sct
 *  @return Frequency of sct clk
 */
uint32_t CLOCK_GetSctClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->SCTFCLKSEL) & CLKCTL0_SCTFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_SCTFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL0_SCTFCLKSEL_SEL(1):
            freq = CLOCK_GetMainPllClkFreq();
            break;

        case CLKCTL0_SCTFCLKSEL_SEL(2):
            freq = CLOCK_GetAux0PllClkFreq();
            break;

        case CLKCTL0_SCTFCLKSEL_SEL(3):
            freq = CLOCK_GetFFroFreq();
            break;

        case CLKCTL0_SCTFCLKSEL_SEL(4):
            freq = CLOCK_GetAux1PllClkFreq();
            break;

        case CLKCTL0_SCTFCLKSEL_SEL(5):
            freq = CLOCK_GetAudioPllClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL0->SCTFCLKDIV & CLKCTL0_SCTFCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of Flexcomm functional Clock
 *  @param   id    : flexcomm index to get frequency.
 *  @return Frequency of Flexcomm functional Clock
 */
uint32_t CLOCK_GetFlexCommClkFreq(uint32_t id)
{
    uint32_t freq   = 0U;
    uint32_t clkSel = 0U;

    if (id <= 3UL)
    {
        clkSel = CLKCTL1->FLEXCOMM[id].FCFCLKSEL;
    }
    else if (id == 14UL)
    {
        clkSel = CLKCTL1->FC14FCLKSEL;
    }
    else
    {
        assert(false);
    }

    switch (clkSel)
    {
        case CLKCTL1_FCFCLKSEL_SEL(0):
            freq = CLOCK_GetSFroFreq();
            break;

        case CLKCTL1_FCFCLKSEL_SEL(1):
            freq = CLOCK_GetFFroFreq();
            break;

        case CLKCTL1_FCFCLKSEL_SEL(2):
            freq = CLOCK_GetAudioPllClkFreq();
            break;

        case CLKCTL1_FCFCLKSEL_SEL(3):
            freq = CLOCK_GetMclkInClkFreq();
            break;

        case CLKCTL1_FCFCLKSEL_SEL(4):
            freq = CLOCK_GetFRGClock(id);
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Return Frequency of CTimer Clock
 *  @param   id    : ctimer index to get frequency.
 *  @return Frequency of CTimer Clock
 */
uint32_t CLOCK_GetCTimerClkFreq(uint32_t id)
{
    uint32_t freq = 0U;

    assert(id < 4U);

    switch ((CLKCTL1->CT32BITFCLKSEL[id]) & CLKCTL1_CT32BITFCLKSEL_SEL_MASK)
    {
        case CLKCTL1_CT32BITFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(1):
            freq = CLOCK_GetSFroFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(2):
            freq = CLOCK_GetFFroFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(3):
            freq = CLOCK_GetAudioPllClkFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(4):
            freq = CLOCK_GetMclkInClkFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(5):
            freq = CLOCK_GetLpOscFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Return Frequency of Utick Clock
 *  @return Frequency of Utick Clock
 */
uint32_t CLOCK_GetUtickClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->UTICKFCLKSEL) & CLKCTL0_UTICKFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_UTICKFCLKSEL_SEL(0):
            freq = CLOCK_GetLpOscFreq();
            break;

        case CLKCTL1_CT32BITFCLKSEL_SEL(1):
            freq = CLOCK_GetMainClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Return Frequency of FLEXSPI Clock
 *  @return Frequency of FLEXSPI.
 */
uint32_t CLOCK_GetFlexspiClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->FLEXSPIFCLKSEL) & CLKCTL0_FLEXSPIFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_FLEXSPIFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(1):
            freq = CLOCK_GetT3PllMciFlexspiClkFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(2):
            freq = CLOCK_GetAux0PllClkFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(3):
            freq = CLOCK_GetFFroFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(4):
            freq = CLOCK_GetAux1PllClkFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(5):
            freq = CLOCK_GetTddrMciFlexspiClkFreq();
            break;

        case CLKCTL0_FLEXSPIFCLKSEL_SEL(6):
            freq = CLOCK_GetT3PllMci256mClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL0->FLEXSPIFCLKDIV & CLKCTL0_FLEXSPIFCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of USIM Clock
 *  @return Frequency of USIM.
 */
uint32_t CLOCK_GetUsimClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->USIMFCLKSEL) & CLKCTL0_USIMFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_USIMFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL0_USIMFCLKSEL_SEL(1):
            freq = CLOCK_GetAudioPllClkFreq();
            break;

        case CLKCTL0_USIMFCLKSEL_SEL(2):
            freq = CLOCK_GetFFroFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL0->USIMFCLKDIV & CLKCTL0_USIMFCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of GAU Clock
 *  @return Frequency of GAU.
 */
uint32_t CLOCK_GetGauClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL0->GAUFCLKSEL) & CLKCTL0_GAUFCLKSEL_SEL_MASK)
    {
        case CLKCTL0_GAUFCLKSEL_SEL(0):
            freq = CLOCK_GetMainClkFreq();
            break;

        case CLKCTL0_GAUFCLKSEL_SEL(1):
            freq = CLOCK_GetT3PllMci256mClkFreq();
            break;

        case CLKCTL0_GAUFCLKSEL_SEL(2):
            freq = CLOCK_GetAvPllCh2Freq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq / ((CLKCTL0->GAUFCLKDIV & CLKCTL0_GAUFCLKDIV_DIV_MASK) + 1U);
}

/*! @brief  Return Frequency of OSTimer Clock
 *  @return Frequency of OSTimer.
 */
uint32_t CLOCK_GetOSTimerClkFreq(void)
{
    uint32_t freq = 0U;

    switch ((CLKCTL1->OSEVENTFCLKSEL) & CLKCTL1_OSEVENTFCLKSEL_SEL_MASK)
    {
        case CLKCTL1_OSEVENTFCLKSEL_SEL(0):
            freq = CLOCK_GetLpOscFreq();
            break;

        case CLKCTL1_OSEVENTFCLKSEL_SEL(1):
            freq = CLOCK_GetClk32KFreq();
            break;

        case CLKCTL1_OSEVENTFCLKSEL_SEL(2):
            freq = CLOCK_GetCoreSysClkFreq();
            break;

        case CLKCTL1_OSEVENTFCLKSEL_SEL(3):
            freq = CLOCK_GetMainClkFreq();
            break;

        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*! @brief  Enables and disables 32kHz XTAL
 *  @param  enable : true to enable 32k XTAL clock, false to disable clock
 */
void CLOCK_EnableXtal32K(bool enable)
{
    if (enable)
    {
        AON_SOC_CIU->PAD_PU_PD_EN1 &=
            ~(AON_SOC_CIU_PAD_PU_PD_EN1_GPIO22_PU_PD_EN_MASK | AON_SOC_CIU_PAD_PU_PD_EN1_GPIO23_PU_PD_EN_MASK);
        AON_SOC_CIU->MCI_IOMUX_EN0 |= (3UL << 22);
        PMU->XTAL32K_CTRL |= PMU_XTAL32K_CTRL_X32K_EN_MASK;
        while ((PMU->XTAL32K_CTRL & PMU_XTAL32K_CTRL_X32K_RDY_MASK) == 0U)
        {
        }
    }
    else
    {
        PMU->XTAL32K_CTRL &= ~PMU_XTAL32K_CTRL_X32K_EN_MASK;
    }
}

/*! @brief  Enables and disables RTC 32KHz
 *  @param  enable : true to enable 32k RTC clock, false to disable clock
 */
void CLOCK_EnableRtc32K(bool enable)
{
    if (enable)
    {
        CLKCTL0->CLK32KHZCTL0 |= CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_MASK;
    }
    else
    {
        CLKCTL0->CLK32KHZCTL0 &= ~CLKCTL0_CLK32KHZCTL0_ENA_32KHZ_MASK;
    }
}

static uint32_t CLOCK_CfgTcpuRefClk(uint32_t targetHz, clock_tcpu_flexspi_div_t div)
{
    uint32_t freq  = 0UL;
    uint32_t steps = 0UL;

    if (CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_40000KHZ)
    {
        /* Fbdiv from 75 to 96, step 40MHz */
        freq = MAX(targetHz, CLOCK_MHZ(3000UL));
        freq = MIN(freq, CLOCK_MHZ(3840UL));
        /* Find the closest freq to target */
        steps = (freq + CLOCK_MHZ(20UL) - CLOCK_MHZ(3000UL)) / CLOCK_MHZ(40UL);
        freq  = CLOCK_MHZ(3000UL) + steps * CLOCK_MHZ(40UL);
        /* Get step register value */
        steps += 75UL;
    }
    else if (CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_38400KHZ)
    {
        /* Fbdiv from 78 to 100, step 38.4MHz */
        freq = MAX(targetHz, CLOCK_KHZ(2995200UL));
        freq = MIN(freq, CLOCK_MHZ(3840UL));
        /* Find the closest freq to target */
        steps = (freq + CLOCK_KHZ(19200UL) - CLOCK_KHZ(2995200UL)) / CLOCK_KHZ(38400UL);
        freq  = CLOCK_KHZ(2995200UL) + steps * CLOCK_KHZ(38400UL);
        /* Get step register value */
        steps += 78UL;
    }
    else
    {
        assert(false);
    }

    SYSCTL2->PLL_CTRL =
        (SYSCTL2->PLL_CTRL & ~(SYSCTL2_PLL_CTRL_TCPU_FBDIV_MASK | SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_MASK)) |
        SYSCTL2_PLL_CTRL_TCPU_FBDIV(steps) | SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL(div);

    return freq;
}

/*! @brief  Initialize TCPU FVCO to target frequency.
 *          For 40MHz XTAL, FVCO ranges from 3000MHz to 3840MHz.
            For 38.4MHz XTAL, FVCO ranges from 2995.2MHz to 3840MHz
 *  @param  targetHz  : Target FVCO frequency in Hz.
 *  @param  div       : Divider for tcpu_mci_flexspi_clk.
 *  @return Actual FVCO frequency in Hz.
 */
uint32_t CLOCK_InitTcpuRefClk(uint32_t targetHz, clock_tcpu_flexspi_div_t div)
{
    uint32_t freq;

    if ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TCPU_PDB_MASK) != 0U)
    {
        CLOCK_DeinitTcpuRefClk();
    }

    SYSPLL_TCPU->TCPU_CTRL_ONE_REG = 0x74U;
    freq                           = CLOCK_CfgTcpuRefClk(targetHz, div);

    /* Set PDB */
    SYSCTL2->PLL_CTRL |= SYSCTL2_PLL_CTRL_TCPU_PDB_MASK;
    /* Wait PLL lock */
    while ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TCPU_LOCK_MASK) == 0U)
    {
    }

    return freq;
}

/*! brief  Deinit the TCPU reference clock.
 *  param  none.
 */
void CLOCK_DeinitTcpuRefClk(void)
{
    /* Gate all TCPU output clocks */
    SYSCTL2->SOURCE_CLK_GATE |=
        SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG_MASK | SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG_MASK;
    /* Clear PDB */
    SYSCTL2->PLL_CTRL &= ~SYSCTL2_PLL_CTRL_TCPU_PDB_MASK;
    /* Wait PLL lock */
    /* Ensure SystemCoreClock is up to date for accurate CLOCK_DelayUs() */
    SystemCoreClockUpdate();
    CLOCK_DelayUs(1U);

    CLOCK_DisableClock(kCLOCK_RefClkTcpu);
}

/*! @brief  Initialize the TDDR reference clock.
 *  @param  div       : Divider for tddr_mci_flexspi_clk.
 */
void CLOCK_InitTddrRefClk(clock_tddr_flexspi_div_t div)
{
    if ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TDDR_PDB_MASK) != 0U)
    {
        CLOCK_DeinitTddrRefClk();
    }

    REG_SYSPLL_TDDR->TDDR_CTRL_ONE_REG = 0x74U;
    SYSCTL2->PLL_CTRL =
        (SYSCTL2->PLL_CTRL & ~SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_MASK) | SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL(div);

    /* Set PDB */
    SYSCTL2->PLL_CTRL |= SYSCTL2_PLL_CTRL_TDDR_PDB_MASK;
    /* Wait PLL lock */
    while ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_TDDR_LOCK_MASK) == 0U)
    {
    }
}

/*! brief  Deinit the TDDR reference clock.
 *  param  none.
 */
void CLOCK_DeinitTddrRefClk(void)
{
    /* Gate all TDDR output clocks */
    SYSCTL2->SOURCE_CLK_GATE |=
        SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_MASK | SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG_MASK;
    /* Clear PDB */
    SYSCTL2->PLL_CTRL &= ~SYSCTL2_PLL_CTRL_TDDR_PDB_MASK;
    /* Wait PLL lock */
    /* Ensure SystemCoreClock is up to date for accurate CLOCK_DelayUs() */
    SystemCoreClockUpdate();
    CLOCK_DelayUs(1U);

    CLOCK_DisableClock(kCLOCK_RefClkTddr);
}

/*! @brief  Initialize the T3 reference clock.
 *  @param  cnfg       : t3pll_mci_48_60m_irc clock configuration
 */
void CLOCK_InitT3RefClk(clock_t3_mci_irc_config_t cnfg)
{
    if ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_T3_PDB_MASK) != 0U)
    {
        CLOCK_DeinitT3RefClk();
    }

    if (cnfg == kCLOCK_T3MciIrc60m)
    {
        SYSPLL_T3->CLKTREE_CTRL_SIX_REG = 0x5U;
    }
    else
    {
        SYSPLL_T3->CLKTREE_CTRL_SIX_REG = 0xAU;
    }
    /* Set PDB */
    SYSCTL2->PLL_CTRL |= SYSCTL2_PLL_CTRL_T3_PDB_MASK;
    /* Wait PLL lock */
    while ((SYSCTL2->PLL_CTRL & SYSCTL2_PLL_CTRL_T3_LOCK_MASK) == 0U)
    {
    }
}

/*! brief  Deinit the T3 reference clock.
 *  param  none.
 */
void CLOCK_DeinitT3RefClk(void)
{
    /* Ensure SystemCoreClock is up to date for accurate CLOCK_DelayUs() */
    SystemCoreClockUpdate();
    /* Gate all T3 output clocks */
    SYSCTL2->SOURCE_CLK_GATE |=
        SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG_MASK | SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_MASK |
        SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG_MASK | SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG_MASK;
    /* Clear PDB */
    SYSCTL2->PLL_CTRL &= ~SYSCTL2_PLL_CTRL_T3_PDB_MASK;

    /* Wait PLL lock */
    CLOCK_DelayUs(1U);
}

static void CLOCK_ConfigAvPll(void)
{
    uint32_t fbdiv          = 0x51U;
    uint32_t freq_offset_c8 = 0x0U;
    uint32_t pll_calclk_div = 0x14U;
    uint32_t refdiv         = 0x2U;
    uint32_t ext_speed      = 0x2U;
    uint32_t intpi          = 0x3U;
    uint32_t intpr          = 0x4U;
    uint32_t icp            = 0x5U;

    if (CLK_XTAL_OSC_CLK == CLK_XTAL_OSC_CLK_38400KHZ)
    {
        fbdiv          = 0x54U;
        freq_offset_c8 = 0x4924U;
        pll_calclk_div = 0x13U;
    }

    SYSCTL2->AVPLL_CTRL2 =
        (SYSCTL2->AVPLL_CTRL2 & ~(SYSCTL2_AVPLL_CTRL2_ICP_MASK | SYSCTL2_AVPLL_CTRL2_INTPI_MASK |
                                  SYSCTL2_AVPLL_CTRL2_INTPR_MASK | SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV_MASK)) |
        SYSCTL2_AVPLL_CTRL2_ICP(icp) | SYSCTL2_AVPLL_CTRL2_INTPI(intpi) | SYSCTL2_AVPLL_CTRL2_INTPR(intpr) |
        SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV(pll_calclk_div);

    SYSCTL2->AVPLL_CTRL3 =
        (SYSCTL2->AVPLL_CTRL3 & ~SYSCTL2_AVPLL_CTRL3_REFDIV_MASK) | SYSCTL2_AVPLL_CTRL3_REFDIV(refdiv);

    SYSCTL2->AVPLL_CTRL0 =
        (SYSCTL2->AVPLL_CTRL0 & ~SYSCTL2_AVPLL_CTRL0_EXT_SPEED_MASK) | SYSCTL2_AVPLL_CTRL0_EXT_SPEED(ext_speed);

    SYSCTL2->AVPLL_CTRL1 = (SYSCTL2->AVPLL_CTRL1 & ~SYSCTL2_AVPLL_CTRL1_FBDIV_MASK) | SYSCTL2_AVPLL_CTRL1_FBDIV(fbdiv);

    SYSCTL2->AVPLL_CTRL8 =
        (SYSCTL2->AVPLL_CTRL8 & ~(SYSCTL2_AVPLL_CTRL8_EN_LP_C8_MASK | SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8_MASK)) |
        SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8(freq_offset_c8);
}

static void CLOCK_ConfigAvPllCh1(clock_avpll_ch_freq_t ch1Freq)
{
    if (ch1Freq != kCLOCK_AvPllChUnchanged)
    {
        SYSCTL2->AVPLL_CTRL2 = (SYSCTL2->AVPLL_CTRL2 & ~SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1_MASK) |
                               SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1(s_avpllPostDiv0p5[(uint32_t)ch1Freq]);

        SYSCTL2->AVPLL_CTRL1 = (SYSCTL2->AVPLL_CTRL1 & ~SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1_MASK) |
                               SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1(s_avpllFreqOff[(uint32_t)ch1Freq]);

        SYSCTL2->AVPLL_CTRL5 = (SYSCTL2->AVPLL_CTRL5 & ~SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_MASK) |
                               SYSCTL2_AVPLL_CTRL5_POSTDIV_C1(s_avpllPostDiv[(uint32_t)ch1Freq]);
    }
}

static void CLOCK_ConfigAvPllCh2(clock_avpll_ch_freq_t ch2Freq)
{
    if (ch2Freq != kCLOCK_AvPllChUnchanged)
    {
        SYSCTL2->AVPLL_CTRL12 = (SYSCTL2->AVPLL_CTRL12 & ~SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2_MASK) |
                                SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2(s_avpllPostDiv0p5[(uint32_t)ch2Freq]);

        SYSCTL2->AVPLL_CTRL11 = SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2(s_avpllFreqOff[(uint32_t)ch2Freq]) |
                                SYSCTL2_AVPLL_CTRL11_POSTDIV_C2(s_avpllPostDiv[(uint32_t)ch2Freq]);
    }
}

static void CLOCK_PowerUpAnaGrp(void)
{
    uint32_t anaGrpPu = SYSCTL2->ANA_GRP_CTRL;

    if ((anaGrpPu & SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK) == 0U)
    {
        SYSCTL2->ANA_GRP_CTRL = anaGrpPu | SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK;
        CLOCK_DelayUs(50U); /* Delay 50us */
    }
}

static void CLOCK_PowerDownAnaGrp(void)
{
    uint32_t anaGrpPu = SYSCTL2->ANA_GRP_CTRL;

    if ((anaGrpPu & SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK) != 0U)
    {
        SYSCTL2->ANA_GRP_CTRL = anaGrpPu & ~SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK;
    }
}

/*! @brief  Initialize the AVPLL.
 *  @param  enableCh1  : Enable AVPLL channel1
 *  @param  enableCh2  : Enable AVPLL channel2
 *  @param  enableCali : Enable AVPLL calibration
 */
void CLOCK_EnableAvPllCh(bool enableCh1, bool enableCh2, bool enableCali)
{
    uint32_t calDoneDelay;

    /* Step 3: Pull up PU_C1 PU_C2 */
    if (enableCh1)
    {
        SYSCTL2->AVPLL_CTRL2 |= SYSCTL2_AVPLL_CTRL2_PU_C1_MASK;
    }
    if (enableCh2)
    {
        SYSCTL2->AVPLL_CTRL12 |= SYSCTL2_AVPLL_CTRL12_PU_C2_MASK;
    }

    /* Pull up freq_offset_c8_ready */
    SYSCTL2->AVPLL_CTRL8 |= SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_MASK;
    CLOCK_DelayUs(3U); /* Delay 516*FVCO*4, about 3us */

    /* Step 4: Pull low freq_offset_c8_ready */
    SYSCTL2->AVPLL_CTRL8 &= ~SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_MASK;
    CLOCK_DelayUs(2U); /* Delay more than 1us, recommended to delay 2us */

    /* Step 5: Pull low RESET_C8 */
    SYSCTL2->AVPLL_CTRL8 &= ~SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_MASK;
    CLOCK_DelayUs(5U); /* Delay 5us. */

    if (enableCali)
    {
        /* Step 6: Cali start */
        SYSCTL2->AVPLL_CTRL2 |= SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_MASK;
    }
    CLOCK_DelayUs(5U); /* Delay 5us */

    /* Step 7: Pull low RESET_C1 RESET_C2 */
    if (enableCh1)
    {
        SYSCTL2->AVPLL_CTRL8 &= ~SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_MASK;
    }
    if (enableCh2)
    {
        SYSCTL2->AVPLL_CTRL13 &= ~SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_MASK;
    }
    CLOCK_DelayUs(2U); /* Delay 516*FVCO , about 2us */

    /* Step 8: Pull high freq_offset_ready_c1/c2 */
    if (enableCh1)
    {
        SYSCTL2->AVPLL_CTRL1 |= SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_MASK;
    }
    if (enableCh2)
    {
        SYSCTL2->AVPLL_CTRL12 |= SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_MASK;
    }
    CLOCK_DelayUs(2U); /* Delay 516*FVCO , about 2us */

    /* Step 9: pull low freq_offset_ready_c1/2 */
    if (enableCh1)
    {
        SYSCTL2->AVPLL_CTRL1 &= ~SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_MASK;
    }
    if (enableCh2)
    {
        SYSCTL2->AVPLL_CTRL12 &= ~SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_MASK;
    }
    CLOCK_DelayUs(12U); /* Delay more than 10us, recommended 12 us */

    if (enableCali)
    {
        /* Step 10: wait cali done */
        calDoneDelay = 0U;
        while (
            ((SYSCTL2->AVPLL_CTRL7 & SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_MASK) != SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_MASK) &&
            (calDoneDelay < 600U)) /* Wait cali done or 600us */
        {
            calDoneDelay += 10U;
            CLOCK_DelayUs(10U);
        }
        CLOCK_DelayUs(10U);

        /* Step 11: clear cali start */
        SYSCTL2->AVPLL_CTRL2 &= ~SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_MASK;
    }
}

/*! @brief  Disable the AVPLL.
 *  @param  disableCh1  : Disable AVPLL channel1, channel unchanged on false.
 *  @param  disableCh2  : Disable AVPLL channel2, channel unchanged on false.
 */
void CLOCK_DisableAvPllCh(bool disableCh1, bool disableCh2)
{
    if (disableCh1)
    {
        /* Pull up RESET_C1 */
        SYSCTL2->AVPLL_CTRL8 |= SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_MASK;
        /* Pull low PU_C1 */
        SYSCTL2->AVPLL_CTRL2 &= ~SYSCTL2_AVPLL_CTRL2_PU_C1_MASK;
    }

    if (disableCh2)
    {
        /* Pull up RESET_C2 */
        SYSCTL2->AVPLL_CTRL13 |= SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_MASK;
        /* Pull low PU_C2 */
        SYSCTL2->AVPLL_CTRL12 &= ~SYSCTL2_AVPLL_CTRL12_PU_C2_MASK;
    }
}

/*! @brief  Initialize the AVPLL.
 *  @param  cnfg       : AVPLL clock configuration
 */
void CLOCK_InitAvPll(const clock_avpll_config_t *cnfg)
{
    assert(cnfg);

    if ((SYSCTL2->AVPLL_CTRL2 & SYSCTL2_AVPLL_CTRL2_PU_MASK) != 0U)
    {
        /* Pull down AVPLL power for initialization. */
        CLOCK_DeinitAvPll();
    }

    /* Ensure SystemCoreClock is up to date for accurate CLOCK_DelayUs() */
    SystemCoreClockUpdate();

    /* Ensure ANA_GRP is powered up */
    CLOCK_PowerUpAnaGrp();

    /* Configure CH1/CH2 frequency */
    CLOCK_ConfigAvPll();
    CLOCK_ConfigAvPllCh1(cnfg->ch1Freq);
    CLOCK_ConfigAvPllCh2(cnfg->ch2Freq);

    /* Step 1: Pull up PU */
    SYSCTL2->AVPLL_CTRL2 |= SYSCTL2_AVPLL_CTRL2_PU_MASK;
    CLOCK_DelayUs(15U); /* Wait more than 10us, recommended to delay 15us */

    /* Step 2: Pull low RESET */
    SYSCTL2->AVPLL_CTRL8 &= ~SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_MASK;
    CLOCK_DelayUs(15U); /* Wait more than 10us, recommended to delay 15us */

    /* Enable channels */
    CLOCK_EnableAvPllCh(true, true, cnfg->enableCali);
}

/*! @brief  Deinit the AVPLL.
 */
void CLOCK_DeinitAvPll(void)
{
    CLOCK_DisableAvPllCh(true, true);

    /* Pull up RESET */
    SYSCTL2->AVPLL_CTRL8 |= SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_MASK;
    /* Pull up RESET_C8 */
    SYSCTL2->AVPLL_CTRL8 |= SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_MASK;
    /* Pull low PU */
    SYSCTL2->AVPLL_CTRL2 &= ~SYSCTL2_AVPLL_CTRL2_PU_MASK;

    /* Disable REFCLK AUD. */
    CLOCK_DisableClock(kCLOCK_RefClkAud);

    if ((SYSCTL2->USB_CTRL & SYSCTL2_USB_CTRL_USB_PU_MASK) == 0U)
    {
        /* USB also not used, ANA_GRP can be powered down. */
        CLOCK_PowerDownAnaGrp();
    }
}

/*! @brief  Update the AVPLL channel configuration. Enable/Disable state keeps unchanged.
 *  @param  ch1Freq  : Channel 1 frequency to set.
 *  @param  ch2Freq  : Channel 2 frequency to set.
 *  @param  enableCali : Enable AVPLL calibration.
 */
void CLOCK_ConfigAvPllCh(clock_avpll_ch_freq_t ch1Freq, clock_avpll_ch_freq_t ch2Freq, bool enableCali)
{
    bool needRenableCh1, needRenableCh2;

    if ((ch1Freq == kCLOCK_AvPllChUnchanged) && (ch2Freq == kCLOCK_AvPllChUnchanged))
    {
        /* Nothing to change. */
        return;
    }

    /* If channel enabled and need to update configuration, need to reenable. */
    needRenableCh1 =
        ((SYSCTL2->AVPLL_CTRL2 & SYSCTL2_AVPLL_CTRL2_PU_C1_MASK) != 0U) && (ch1Freq != kCLOCK_AvPllChUnchanged);
    needRenableCh2 =
        ((SYSCTL2->AVPLL_CTRL12 & SYSCTL2_AVPLL_CTRL12_PU_C2_MASK) != 0U) && (ch2Freq != kCLOCK_AvPllChUnchanged);

    /* Disable channel before update configuration. */
    CLOCK_DisableAvPllCh(needRenableCh1, needRenableCh2);

    CLOCK_ConfigAvPll();

    if (ch1Freq != kCLOCK_AvPllChUnchanged)
    {
        CLOCK_ConfigAvPllCh1(ch1Freq);
    }
    if (ch2Freq != kCLOCK_AvPllChUnchanged)
    {
        CLOCK_ConfigAvPllCh2(ch2Freq);
    }

    /* Reenable channel if needed. */
    CLOCK_EnableAvPllCh(needRenableCh1, needRenableCh2, enableCali);
}

/*! @brief Enable USB HS PHY PLL clock.
 *
 * This function enables USB HS PHY PLL clock.
 */
void CLOCK_EnableUsbhsPhyClock(void)
{
    uint32_t value;
    value          = SYSCTL2->USB_CTRL;
    uint32_t delay = 100000;

    /* Ensure SystemCoreClock is up to date for accurate CLOCK_DelayUs() */
    SystemCoreClockUpdate();

    /* Ensure ANA_GRP is powered on */
    SYSCTL2->ANA_GRP_CTRL |= SYSCTL2_ANA_GRP_CTRL_PU_XTL_MASK;
    CLOCK_PowerUpAnaGrp();

    /* Reset USB PHY */
    value |=
        SYSCTL2_USB_CTRL_SOFT_PHY_RESET_MASK | SYSCTL2_USB_CTRL_PHY_RESET_SEL_MASK; /* Use soft reset to reset PHY */
    SYSCTL2->USB_CTRL =
        value & ~(SYSCTL2_USB_CTRL_USB_PU_PLL_MASK | SYSCTL2_USB_CTRL_USB_PU_MASK | SYSCTL2_USB_CTRL_USB_PU_OTG_MASK);
    __NOP();
    value &= ~(SYSCTL2_USB_CTRL_SOFT_PHY_RESET_MASK |
               SYSCTL2_USB_CTRL_PHY_RESET_SEL_MASK); /* It is an active high reset; this bit is inverted */
    SYSCTL2->USB_CTRL = value;
    /* Power up PHY OTG detection circuit */
    USBOTG->PHY_REG_OTG_CONTROL |= USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_MASK;
    /* Analog power up through pin */
    value |= SYSCTL2_USB_CTRL_USB_PU_MASK | SYSCTL2_USB_CTRL_USB_PU_OTG_MASK;
    SYSCTL2->USB_CTRL = value;
    /* Power up PLL via pin; raising edge will auto trigger calibration */
    value |= SYSCTL2_USB_CTRL_USB_PU_PLL_MASK;
    SYSCTL2->USB_CTRL = value;
    while ((delay-- > 0U) && ((USBOTG->PLL_CONTROL_0 & USBC_PLL_CONTROL_0_PLL_READY_MASK) == 0U))
    {
    }
}

/*! @brief Disable USB HS PHY PLL clock.
 *
 * This function disables USB HS PHY PLL clock.
 */
void CLOCK_DisableUsbhsPhyClock(void)
{
    uint32_t value;
    value = SYSCTL2->USB_CTRL;
    /* Power down PHY OTG detection circuit */
    USBOTG->PHY_REG_OTG_CONTROL |= USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_MASK;
    /* Analog power down through pin */
    value &= ~SYSCTL2_USB_CTRL_USB_PU_MASK & ~SYSCTL2_USB_CTRL_USB_PU_OTG_MASK;
    SYSCTL2->USB_CTRL = value;
    /* Power down PLL via pin */
    value &= ~SYSCTL2_USB_CTRL_USB_PU_PLL_MASK;
    SYSCTL2->USB_CTRL = value;
    if ((SYSCTL2->AVPLL_CTRL2 & SYSCTL2_AVPLL_CTRL2_PU_MASK) == 0U)
    {
        /* AVPLL also not used, ANA_GRP can be powered down. */
        CLOCK_PowerDownAnaGrp();
    }
    SYSCTL2->ANA_GRP_CTRL &= ~SYSCTL2_ANA_GRP_CTRL_PU_XTL_MASK;
}
