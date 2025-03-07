/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClockRUN ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN configuration
 ******************************************************************************/

/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKRUN_AUDIO_PLL_CLK              12287999UL     /* Clock consumers of audio_pll_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_AUX0_PLL_CLK               260000000UL    /* Clock consumers of aux0_pll_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_AUX1_PLL_CLK               0UL            /* Clock consumers of aux1_pll_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_AVPLL_CH1_CLKOUT           12287999UL     /* Clock consumers of avpll_ch1_clkout output : N/A */
#define BOARD_BOOTCLOCKRUN_AVPLL_CH2_CLKOUT           63999997UL     /* Clock consumers of avpll_ch2_clkout output : N/A */
#define BOARD_BOOTCLOCKRUN_CAU_SLP_CLK                4000000UL      /* Clock consumers of cau_slp_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_CLK_32K                    32000UL        /* Clock consumers of clk_32k output : RTC */
#define BOARD_BOOTCLOCKRUN_CLK_OUT                    0UL            /* Clock consumers of clk_out output : N/A */
#define BOARD_BOOTCLOCKRUN_CLK_PMU_SYS                52000000UL     /* Clock consumers of clk_pmu_sys output : PMU */
#define BOARD_BOOTCLOCKRUN_CTIMER0_FCLK               0UL            /* Clock consumers of ctimer0_fclk output : CTIMER0 */
#define BOARD_BOOTCLOCKRUN_CTIMER1_FCLK               0UL            /* Clock consumers of ctimer1_fclk output : CTIMER1 */
#define BOARD_BOOTCLOCKRUN_CTIMER2_FCLK               0UL            /* Clock consumers of ctimer2_fclk output : CTIMER2 */
#define BOARD_BOOTCLOCKRUN_CTIMER3_FCLK               0UL            /* Clock consumers of ctimer3_fclk output : CTIMER3 */
#define BOARD_BOOTCLOCKRUN_DMIC_FCLK                  0UL            /* Clock consumers of dmic_fclk output : DMIC0 */
#define BOARD_BOOTCLOCKRUN_ELS_128M_CLK               128000000UL    /* Clock consumers of els_128m_clk output : ELS */
#define BOARD_BOOTCLOCKRUN_ELS_256M_CLK               256000000UL    /* Clock consumers of els_256m_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_ELS_64M_CLK                64000000UL     /* Clock consumers of els_64m_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_ELS_FCLK                   0UL            /* Clock consumers of els_fclk output : ELS */
#define BOARD_BOOTCLOCKRUN_FFRO_CLK_DIV4              12075471UL     /* Clock consumers of ffro_clk_div4 output : N/A */
#define BOARD_BOOTCLOCKRUN_FLEXCOMM0_FCLK             0UL            /* Clock consumers of flexcomm0_fclk output : FLEXCOMM0 */
#define BOARD_BOOTCLOCKRUN_FLEXCOMM14_FCLK            0UL            /* Clock consumers of flexcomm14_fclk output : FLEXCOMM14 */
#define BOARD_BOOTCLOCKRUN_FLEXCOMM1_FCLK             0UL            /* Clock consumers of flexcomm1_fclk output : FLEXCOMM1 */
#define BOARD_BOOTCLOCKRUN_FLEXCOMM2_FCLK             0UL            /* Clock consumers of flexcomm2_fclk output : FLEXCOMM2 */
#define BOARD_BOOTCLOCKRUN_FLEXCOMM3_FCLK             0UL            /* Clock consumers of flexcomm3_fclk output : FLEXCOMM3 */
#define BOARD_BOOTCLOCKRUN_FLEXSPI_FCLK               0UL            /* Clock consumers of flexspi_fclk output : FLEXSPI */
#define BOARD_BOOTCLOCKRUN_GAU_FCLK                   0UL            /* Clock consumers of gau_fclk output : GAU_ACOMP, GAU_BG, GAU_DAC0, GAU_GPADC0, GAU_GPADC1 */
#define BOARD_BOOTCLOCKRUN_HCLK                       260000000UL    /* Clock consumers of hclk output : AHB_SECURE_CTRL, APU0, APU1, BLEAPU, BLECTRL, BUCK11, BUCK18, CACHE64_CTRL0, CACHE64_CTRL1, CACHE64_POLSEL0, CACHE64_POLSEL1, CAU, CDOG, CLKCTL0, CLKCTL1, CRC, CTIMER0, CTIMER1, CTIMER2, CTIMER3, DMA0, DMA1, DMIC0, ELS, ENET, FLEXCOMM0, FLEXCOMM1, FLEXCOMM14, FLEXCOMM2, FLEXCOMM3, FLEXSPI, FREQME, GAU_ACOMP, GAU_BG, GAU_DAC0, GAU_GPADC0, GAU_GPADC1, GDMA, GPIO, INPUTMUX, ITRC, LCDIC, MCI_IO_MUX, MRT0, MRT1, OCOTP, OSTIMER, PINT, PKC, PMU, POWERQUAD, PUF, ROMCP, RSTCTL0, RSTCTL1, RTC, SCT0, SDU_FBR_CARD, SDU_FN0_CARD, SDU_FN_CARD, SECGPIO, SENSOR_CTRL, SOCCTRL, SOC_OTP_CTRL, SYSCTL0, SYSCTL1, SYSCTL2, SysTick, TRNG, USBOTG, USIM, UTICK, WLAPU, WLCTRL, WWDT0 */
#define BOARD_BOOTCLOCKRUN_LCD_FCLK                   0UL            /* Clock consumers of lcd_fclk output : LCDIC */
#define BOARD_BOOTCLOCKRUN_LPOSC_CLK_I                1000000UL      /* Clock consumers of lposc_clk_i output : N/A */
#define BOARD_BOOTCLOCKRUN_MAIN_CLK                   260000000UL    /* Clock consumers of main_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_MAIN_PLL_CLK               260000000UL    /* Clock consumers of main_pll_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_MCLK_OUT                   0UL            /* Clock consumers of mclk_out output : N/A */
#define BOARD_BOOTCLOCKRUN_OSEVENT_FCLK               0UL            /* Clock consumers of osevent_fclk output : OSTIMER */
#define BOARD_BOOTCLOCKRUN_OTP_FUSE_32M_CLK           32000000UL     /* Clock consumers of otp_fuse_32m_clk output : OCOTP */
#define BOARD_BOOTCLOCKRUN_REFCLK_PHY                 40000000UL     /* Clock consumers of refclk_phy output : USBOTG */
#define BOARD_BOOTCLOCKRUN_REFCLK_SYS                 0UL            /* Clock consumers of refclk_sys output : N/A */
#define BOARD_BOOTCLOCKRUN_SCT_FCLK                   0UL            /* Clock consumers of sct_fclk output : SCT0 */
#define BOARD_BOOTCLOCKRUN_SFRO_CLK_I                 16000000UL     /* Clock consumers of sfro_clk_i output : N/A */
#define BOARD_BOOTCLOCKRUN_SYSOSC_CLK_I               0UL            /* Clock consumers of sysosc_clk_i output : N/A */
#define BOARD_BOOTCLOCKRUN_SYSTICK_FCLK               260000000UL    /* Clock consumers of systick_fclk output : SysTick */
#define BOARD_BOOTCLOCKRUN_T3PLL_MCI_213P3M           0UL            /* Clock consumers of t3pll_mci_213p3m output : N/A */
#define BOARD_BOOTCLOCKRUN_T3PLL_MCI_256M             256000000UL    /* Clock consumers of t3pll_mci_256m output : N/A */
#define BOARD_BOOTCLOCKRUN_T3PLL_MCI_48_60M_IRC       48301886UL     /* Clock consumers of t3pll_mci_48_60m_irc output : N/A */
#define BOARD_BOOTCLOCKRUN_T3PLL_MCI_FLEXSPI_CLK      0UL            /* Clock consumers of t3pll_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_TCPU_MCI_CLK               260000000UL    /* Clock consumers of tcpu_mci_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_TCPU_MCI_FLEXSPI_CLK       0UL            /* Clock consumers of tcpu_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_TDDR_MCI_ENET_CLK          0UL            /* Clock consumers of tddr_mci_enet_clk output : ENET */
#define BOARD_BOOTCLOCKRUN_TDDR_MCI_FLEXSPI_CLK       320000000UL    /* Clock consumers of tddr_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKRUN_USIM_FCLK                  0UL            /* Clock consumers of usim_fclk output : USIM */
#define BOARD_BOOTCLOCKRUN_UTICK_FCLK                 0UL            /* Clock consumers of utick_fclk output : UTICK */
#define BOARD_BOOTCLOCKRUN_WDT0_FCLK                  0UL            /* Clock consumers of wdt0_fclk output : WWDT0 */

/*! @brief AVPLL set for BOARD_BootClockRUN configuration.
 */
extern const clock_avpll_config_t avpllConfig_BOARD_BootClockRUN;
/*! @brief Clock pre-initialization function.
 */
extern void BOARD_ClockPreConfig(void);
/*! @brief Clock post-initialization function.
 */
extern void BOARD_ClockPostConfig(void);
/*******************************************************************************
 * API for BOARD_BootClockRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ********************** Configuration BOARD_BootClockLPR ***********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockLPR configuration
 ******************************************************************************/

/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKLPR_AUDIO_PLL_CLK              0UL            /* Clock consumers of audio_pll_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_AUX0_PLL_CLK               260000000UL    /* Clock consumers of aux0_pll_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_AUX1_PLL_CLK               0UL            /* Clock consumers of aux1_pll_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_AVPLL_CH1_CLKOUT           0UL            /* Clock consumers of avpll_ch1_clkout output : N/A */
#define BOARD_BOOTCLOCKLPR_AVPLL_CH2_CLKOUT           0UL            /* Clock consumers of avpll_ch2_clkout output : N/A */
#define BOARD_BOOTCLOCKLPR_CAU_SLP_CLK                0UL            /* Clock consumers of cau_slp_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_CLK_32K                    32000UL        /* Clock consumers of clk_32k output : RTC */
#define BOARD_BOOTCLOCKLPR_CLK_OUT                    0UL            /* Clock consumers of clk_out output : N/A */
#define BOARD_BOOTCLOCKLPR_CLK_PMU_SYS                52000000UL     /* Clock consumers of clk_pmu_sys output : PMU */
#define BOARD_BOOTCLOCKLPR_CTIMER0_FCLK               0UL            /* Clock consumers of ctimer0_fclk output : CTIMER0 */
#define BOARD_BOOTCLOCKLPR_CTIMER1_FCLK               0UL            /* Clock consumers of ctimer1_fclk output : CTIMER1 */
#define BOARD_BOOTCLOCKLPR_CTIMER2_FCLK               0UL            /* Clock consumers of ctimer2_fclk output : CTIMER2 */
#define BOARD_BOOTCLOCKLPR_CTIMER3_FCLK               0UL            /* Clock consumers of ctimer3_fclk output : CTIMER3 */
#define BOARD_BOOTCLOCKLPR_DMIC_FCLK                  0UL            /* Clock consumers of dmic_fclk output : DMIC0 */
#define BOARD_BOOTCLOCKLPR_ELS_128M_CLK               128000000UL    /* Clock consumers of els_128m_clk output : ELS */
#define BOARD_BOOTCLOCKLPR_ELS_256M_CLK               256000000UL    /* Clock consumers of els_256m_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_ELS_64M_CLK                64000000UL     /* Clock consumers of els_64m_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_ELS_FCLK                   0UL            /* Clock consumers of els_fclk output : ELS */
#define BOARD_BOOTCLOCKLPR_FFRO_CLK_DIV4              0UL            /* Clock consumers of ffro_clk_div4 output : N/A */
#define BOARD_BOOTCLOCKLPR_FLEXCOMM0_FCLK             0UL            /* Clock consumers of flexcomm0_fclk output : FLEXCOMM0 */
#define BOARD_BOOTCLOCKLPR_FLEXCOMM14_FCLK            0UL            /* Clock consumers of flexcomm14_fclk output : FLEXCOMM14 */
#define BOARD_BOOTCLOCKLPR_FLEXCOMM1_FCLK             0UL            /* Clock consumers of flexcomm1_fclk output : FLEXCOMM1 */
#define BOARD_BOOTCLOCKLPR_FLEXCOMM2_FCLK             0UL            /* Clock consumers of flexcomm2_fclk output : FLEXCOMM2 */
#define BOARD_BOOTCLOCKLPR_FLEXCOMM3_FCLK             0UL            /* Clock consumers of flexcomm3_fclk output : FLEXCOMM3 */
#define BOARD_BOOTCLOCKLPR_FLEXSPI_FCLK               0UL            /* Clock consumers of flexspi_fclk output : FLEXSPI */
#define BOARD_BOOTCLOCKLPR_GAU_FCLK                   0UL            /* Clock consumers of gau_fclk output : GAU_ACOMP, GAU_BG, GAU_DAC0, GAU_GPADC0, GAU_GPADC1 */
#define BOARD_BOOTCLOCKLPR_HCLK                       260000000UL    /* Clock consumers of hclk output : AHB_SECURE_CTRL, APU0, APU1, BLEAPU, BLECTRL, BUCK11, BUCK18, CACHE64_CTRL0, CACHE64_CTRL1, CACHE64_POLSEL0, CACHE64_POLSEL1, CAU, CDOG, CLKCTL0, CLKCTL1, CRC, CTIMER0, CTIMER1, CTIMER2, CTIMER3, DMA0, DMA1, DMIC0, ELS, ENET, FLEXCOMM0, FLEXCOMM1, FLEXCOMM14, FLEXCOMM2, FLEXCOMM3, FLEXSPI, FREQME, GAU_ACOMP, GAU_BG, GAU_DAC0, GAU_GPADC0, GAU_GPADC1, GDMA, GPIO, INPUTMUX, ITRC, LCDIC, MCI_IO_MUX, MRT0, MRT1, OCOTP, OSTIMER, PINT, PKC, PMU, POWERQUAD, PUF, ROMCP, RSTCTL0, RSTCTL1, RTC, SCT0, SDU_FBR_CARD, SDU_FN0_CARD, SDU_FN_CARD, SECGPIO, SENSOR_CTRL, SOCCTRL, SOC_OTP_CTRL, SYSCTL0, SYSCTL1, SYSCTL2, SysTick, TRNG, USBOTG, USIM, UTICK, WLAPU, WLCTRL, WWDT0 */
#define BOARD_BOOTCLOCKLPR_LCD_FCLK                   0UL            /* Clock consumers of lcd_fclk output : LCDIC */
#define BOARD_BOOTCLOCKLPR_LPOSC_CLK_I                0UL            /* Clock consumers of lposc_clk_i output : N/A */
#define BOARD_BOOTCLOCKLPR_MAIN_CLK                   260000000UL    /* Clock consumers of main_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_MAIN_PLL_CLK               260000000UL    /* Clock consumers of main_pll_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_MCLK_OUT                   0UL            /* Clock consumers of mclk_out output : N/A */
#define BOARD_BOOTCLOCKLPR_OSEVENT_FCLK               0UL            /* Clock consumers of osevent_fclk output : OSTIMER */
#define BOARD_BOOTCLOCKLPR_OTP_FUSE_32M_CLK           32000000UL     /* Clock consumers of otp_fuse_32m_clk output : OCOTP */
#define BOARD_BOOTCLOCKLPR_REFCLK_PHY                 40000000UL     /* Clock consumers of refclk_phy output : USBOTG */
#define BOARD_BOOTCLOCKLPR_REFCLK_SYS                 40000000UL     /* Clock consumers of refclk_sys output : N/A */
#define BOARD_BOOTCLOCKLPR_SCT_FCLK                   0UL            /* Clock consumers of sct_fclk output : SCT0 */
#define BOARD_BOOTCLOCKLPR_SFRO_CLK_I                 16000000UL     /* Clock consumers of sfro_clk_i output : N/A */
#define BOARD_BOOTCLOCKLPR_SYSOSC_CLK_I               0UL            /* Clock consumers of sysosc_clk_i output : N/A */
#define BOARD_BOOTCLOCKLPR_SYSTICK_FCLK               260000000UL    /* Clock consumers of systick_fclk output : SysTick */
#define BOARD_BOOTCLOCKLPR_T3PLL_MCI_213P3M           0UL            /* Clock consumers of t3pll_mci_213p3m output : N/A */
#define BOARD_BOOTCLOCKLPR_T3PLL_MCI_256M             256000000UL    /* Clock consumers of t3pll_mci_256m output : N/A */
#define BOARD_BOOTCLOCKLPR_T3PLL_MCI_48_60M_IRC       0UL            /* Clock consumers of t3pll_mci_48_60m_irc output : N/A */
#define BOARD_BOOTCLOCKLPR_T3PLL_MCI_FLEXSPI_CLK      0UL            /* Clock consumers of t3pll_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_TCPU_MCI_CLK               260000000UL    /* Clock consumers of tcpu_mci_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_TCPU_MCI_FLEXSPI_CLK       0UL            /* Clock consumers of tcpu_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_TDDR_MCI_ENET_CLK          0UL            /* Clock consumers of tddr_mci_enet_clk output : ENET */
#define BOARD_BOOTCLOCKLPR_TDDR_MCI_FLEXSPI_CLK       0UL            /* Clock consumers of tddr_mci_flexspi_clk output : N/A */
#define BOARD_BOOTCLOCKLPR_USIM_FCLK                  0UL            /* Clock consumers of usim_fclk output : USIM */
#define BOARD_BOOTCLOCKLPR_UTICK_FCLK                 0UL            /* Clock consumers of utick_fclk output : UTICK */
#define BOARD_BOOTCLOCKLPR_WDT0_FCLK                  0UL            /* Clock consumers of wdt0_fclk output : WWDT0 */

/*******************************************************************************
 * API for BOARD_BootClockLPR configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockLPR(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

