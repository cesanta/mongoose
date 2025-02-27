#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         24000000U  /*!< Board xtal0 frequency in Hz */

#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32k frequency in Hz */
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
#define BOARD_BOOTCLOCKRUN_CORE_CLOCK             600000000U  /*!< Core clock frequency: 600000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKRUN_AHB_CLK_ROOT               600000000UL    /* Clock consumers of AHB_CLK_ROOT output : AIPSTZ1, AIPSTZ2, AIPSTZ3, AIPSTZ4, ARM, FLEXIO3, FLEXSPI, FLEXSPI2, GPIO6, GPIO7, GPIO8, GPIO9 */
#define BOARD_BOOTCLOCKRUN_CAN_CLK_ROOT               40000000UL     /* Clock consumers of CAN_CLK_ROOT output : CAN1, CAN2, CAN3 */
#define BOARD_BOOTCLOCKRUN_CKIL_SYNC_CLK_ROOT         32768UL        /* Clock consumers of CKIL_SYNC_CLK_ROOT output : CSU, EWM, GPT1, GPT2, KPP, PIT, RTWDOG, SNVS, SPDIF, TEMPMON, TSC, USB1, USB2, WDOG1, WDOG2 */
#define BOARD_BOOTCLOCKRUN_CLKO1_CLK                  0UL            /* Clock consumers of CLKO1_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_CLKO2_CLK                  0UL            /* Clock consumers of CLKO2_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_CLK_1M                     1000000UL      /* Clock consumers of CLK_1M output : EWM, RTWDOG */
#define BOARD_BOOTCLOCKRUN_CLK_24M                    24000000UL     /* Clock consumers of CLK_24M output : GPT1, GPT2 */
#define BOARD_BOOTCLOCKRUN_CSI_CLK_ROOT               12000000UL     /* Clock consumers of CSI_CLK_ROOT output : CSI */
#define BOARD_BOOTCLOCKRUN_ENET2_125M_CLK             25000000UL     /* Clock consumers of ENET2_125M_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_ENET2_REF_CLK              0UL            /* Clock consumers of ENET2_REF_CLK output : ENET2 */
#define BOARD_BOOTCLOCKRUN_ENET2_TX_CLK               0UL            /* Clock consumers of ENET2_TX_CLK output : ENET2 */
#define BOARD_BOOTCLOCKRUN_ENET_125M_CLK              50000000UL     /* Clock consumers of ENET_125M_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_ENET_25M_REF_CLK           25000000UL     /* Clock consumers of ENET_25M_REF_CLK output : ENET, ENET2 */
#define BOARD_BOOTCLOCKRUN_ENET_REF_CLK               0UL            /* Clock consumers of ENET_REF_CLK output : ENET */
#define BOARD_BOOTCLOCKRUN_ENET_TX_CLK                0UL            /* Clock consumers of ENET_TX_CLK output : ENET */
#define BOARD_BOOTCLOCKRUN_FLEXIO1_CLK_ROOT           30000000UL     /* Clock consumers of FLEXIO1_CLK_ROOT output : FLEXIO1 */
#define BOARD_BOOTCLOCKRUN_FLEXIO2_CLK_ROOT           30000000UL     /* Clock consumers of FLEXIO2_CLK_ROOT output : FLEXIO2, FLEXIO3 */
#define BOARD_BOOTCLOCKRUN_FLEXSPI2_CLK_ROOT          130909090UL    /* Clock consumers of FLEXSPI2_CLK_ROOT output : FLEXSPI2 */
#define BOARD_BOOTCLOCKRUN_FLEXSPI_CLK_ROOT           130909090UL    /* Clock consumers of FLEXSPI_CLK_ROOT output : FLEXSPI */
#define BOARD_BOOTCLOCKRUN_GPT1_IPG_CLK_HIGHFREQ      75000000UL     /* Clock consumers of GPT1_ipg_clk_highfreq output : GPT1 */
#define BOARD_BOOTCLOCKRUN_GPT2_IPG_CLK_HIGHFREQ      75000000UL     /* Clock consumers of GPT2_ipg_clk_highfreq output : GPT2 */
#define BOARD_BOOTCLOCKRUN_IPG_CLK_ROOT               150000000UL    /* Clock consumers of IPG_CLK_ROOT output : ADC1, ADC2, ADC_ETC, AOI1, AOI2, ARM, BEE, CAN1, CAN2, CAN3, CCM, CMP1, CMP2, CMP3, CMP4, CSI, CSU, DCDC, DCP, DMA0, DMAMUX, ENC1, ENC2, ENC3, ENC4, ENET, ENET2, EWM, FLEXIO1, FLEXIO2, FLEXIO3, FLEXRAM, FLEXSPI, FLEXSPI2, GPC, GPIO1, GPIO10, GPIO2, GPIO3, GPIO4, GPIO5, IOMUXC, KPP, LCDIF, LPI2C1, LPI2C2, LPI2C3, LPI2C4, LPSPI1, LPSPI2, LPSPI3, LPSPI4, LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8, NVIC, OCOTP, PMU, PWM1, PWM2, PWM3, PWM4, PXP, ROMC, RTWDOG, SAI1, SAI2, SAI3, SNVS, SPDIF, SRC, TEMPMON, TMR1, TMR2, TMR3, TMR4, TRNG, TSC, USB1, USB2, USDHC1, USDHC2, WDOG1, WDOG2, XBARA1, XBARB2, XBARB3 */
#define BOARD_BOOTCLOCKRUN_LCDIF_CLK_ROOT             67500000UL     /* Clock consumers of LCDIF_CLK_ROOT output : LCDIF */
#define BOARD_BOOTCLOCKRUN_LPI2C_CLK_ROOT             60000000UL     /* Clock consumers of LPI2C_CLK_ROOT output : LPI2C1, LPI2C2, LPI2C3, LPI2C4 */
#define BOARD_BOOTCLOCKRUN_LPSPI_CLK_ROOT             105600000UL    /* Clock consumers of LPSPI_CLK_ROOT output : LPSPI1, LPSPI2, LPSPI3, LPSPI4 */
#define BOARD_BOOTCLOCKRUN_LVDS1_CLK                  1200000000UL   /* Clock consumers of LVDS1_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_MQS_MCLK                   63529411UL     /* Clock consumers of MQS_MCLK output : N/A */
#define BOARD_BOOTCLOCKRUN_PERCLK_CLK_ROOT            75000000UL     /* Clock consumers of PERCLK_CLK_ROOT output : GPT1, GPT2, PIT */
#define BOARD_BOOTCLOCKRUN_PLL7_MAIN_CLK              24000000UL     /* Clock consumers of PLL7_MAIN_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_SAI1_CLK_ROOT              63529411UL     /* Clock consumers of SAI1_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_SAI1_MCLK1                 63529411UL     /* Clock consumers of SAI1_MCLK1 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_SAI1_MCLK2                 63529411UL     /* Clock consumers of SAI1_MCLK2 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_SAI1_MCLK3                 30000000UL     /* Clock consumers of SAI1_MCLK3 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_SAI2_CLK_ROOT              63529411UL     /* Clock consumers of SAI2_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_SAI2_MCLK1                 63529411UL     /* Clock consumers of SAI2_MCLK1 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_SAI2_MCLK2                 0UL            /* Clock consumers of SAI2_MCLK2 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_SAI2_MCLK3                 30000000UL     /* Clock consumers of SAI2_MCLK3 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_SAI3_CLK_ROOT              63529411UL     /* Clock consumers of SAI3_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_SAI3_MCLK1                 63529411UL     /* Clock consumers of SAI3_MCLK1 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_SAI3_MCLK2                 0UL            /* Clock consumers of SAI3_MCLK2 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_SAI3_MCLK3                 30000000UL     /* Clock consumers of SAI3_MCLK3 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_SEMC_CLK_ROOT              75000000UL     /* Clock consumers of SEMC_CLK_ROOT output : SEMC */
#define BOARD_BOOTCLOCKRUN_SPDIF0_CLK_ROOT            30000000UL     /* Clock consumers of SPDIF0_CLK_ROOT output : SPDIF */
#define BOARD_BOOTCLOCKRUN_SPDIF0_EXTCLK_OUT          0UL            /* Clock consumers of SPDIF0_EXTCLK_OUT output : SPDIF */
#define BOARD_BOOTCLOCKRUN_TRACE_CLK_ROOT             132000000UL    /* Clock consumers of TRACE_CLK_ROOT output : ARM */
#define BOARD_BOOTCLOCKRUN_UART_CLK_ROOT              80000000UL     /* Clock consumers of UART_CLK_ROOT output : LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8 */
#define BOARD_BOOTCLOCKRUN_USBPHY1_CLK                0UL            /* Clock consumers of USBPHY1_CLK output : TEMPMON, USB1 */
#define BOARD_BOOTCLOCKRUN_USBPHY2_CLK                0UL            /* Clock consumers of USBPHY2_CLK output : USB2 */
#define BOARD_BOOTCLOCKRUN_USDHC1_CLK_ROOT            198000000UL    /* Clock consumers of USDHC1_CLK_ROOT output : USDHC1 */
#define BOARD_BOOTCLOCKRUN_USDHC2_CLK_ROOT            198000000UL    /* Clock consumers of USDHC2_CLK_ROOT output : USDHC2 */

/*! @brief Arm PLL set for BOARD_BootClockRUN configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN;
/*! @brief Usb1 PLL set for BOARD_BootClockRUN configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_BootClockRUN;
/*! @brief Sys PLL for BOARD_BootClockRUN configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_BootClockRUN;
/*! @brief Video PLL set for BOARD_BootClockRUN configuration.
 */
extern const clock_video_pll_config_t videoPllConfig_BOARD_BootClockRUN;
/*! @brief Enet PLL set for BOARD_BootClockRUN configuration.
 */
extern const clock_enet_pll_config_t enetPllConfig_BOARD_BootClockRUN;

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
 ******************* Configuration BOARD_BootClockRUN_528M *********************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for BOARD_BootClockRUN_528M configuration
 ******************************************************************************/
#define BOARD_BOOTCLOCKRUN_528M_CORE_CLOCK        528000000U  /*!< Core clock frequency: 528000000Hz */

/* Clock outputs (values are in Hz): */
#define BOARD_BOOTCLOCKRUN_528M_AHB_CLK_ROOT          528000000UL    /* Clock consumers of AHB_CLK_ROOT output : AIPSTZ1, AIPSTZ2, AIPSTZ3, AIPSTZ4, ARM, FLEXIO3, FLEXSPI, FLEXSPI2, GPIO6, GPIO7, GPIO8, GPIO9 */
#define BOARD_BOOTCLOCKRUN_528M_CAN_CLK_ROOT          40000000UL     /* Clock consumers of CAN_CLK_ROOT output : CAN1, CAN2, CAN3 */
#define BOARD_BOOTCLOCKRUN_528M_CKIL_SYNC_CLK_ROOT    32768UL        /* Clock consumers of CKIL_SYNC_CLK_ROOT output : CSU, EWM, GPT1, GPT2, KPP, PIT, RTWDOG, SNVS, SPDIF, TEMPMON, TSC, USB1, USB2, WDOG1, WDOG2 */
#define BOARD_BOOTCLOCKRUN_528M_CLKO1_CLK             0UL            /* Clock consumers of CLKO1_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_CLKO2_CLK             0UL            /* Clock consumers of CLKO2_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_CLK_1M                1000000UL      /* Clock consumers of CLK_1M output : EWM, RTWDOG */
#define BOARD_BOOTCLOCKRUN_528M_CLK_24M               24000000UL     /* Clock consumers of CLK_24M output : GPT1, GPT2 */
#define BOARD_BOOTCLOCKRUN_528M_CSI_CLK_ROOT          12000000UL     /* Clock consumers of CSI_CLK_ROOT output : CSI */
#define BOARD_BOOTCLOCKRUN_528M_ENET2_125M_CLK        1200000UL      /* Clock consumers of ENET2_125M_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_ENET2_REF_CLK         0UL            /* Clock consumers of ENET2_REF_CLK output : ENET2 */
#define BOARD_BOOTCLOCKRUN_528M_ENET2_TX_CLK          0UL            /* Clock consumers of ENET2_TX_CLK output : ENET2 */
#define BOARD_BOOTCLOCKRUN_528M_ENET_125M_CLK         2400000UL      /* Clock consumers of ENET_125M_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_ENET_25M_REF_CLK      1200000UL      /* Clock consumers of ENET_25M_REF_CLK output : ENET, ENET2 */
#define BOARD_BOOTCLOCKRUN_528M_ENET_REF_CLK          0UL            /* Clock consumers of ENET_REF_CLK output : ENET */
#define BOARD_BOOTCLOCKRUN_528M_ENET_TX_CLK           0UL            /* Clock consumers of ENET_TX_CLK output : ENET */
#define BOARD_BOOTCLOCKRUN_528M_FLEXIO1_CLK_ROOT      30000000UL     /* Clock consumers of FLEXIO1_CLK_ROOT output : FLEXIO1 */
#define BOARD_BOOTCLOCKRUN_528M_FLEXIO2_CLK_ROOT      30000000UL     /* Clock consumers of FLEXIO2_CLK_ROOT output : FLEXIO2, FLEXIO3 */
#define BOARD_BOOTCLOCKRUN_528M_FLEXSPI2_CLK_ROOT     130909090UL    /* Clock consumers of FLEXSPI2_CLK_ROOT output : FLEXSPI2 */
#define BOARD_BOOTCLOCKRUN_528M_FLEXSPI_CLK_ROOT      130909090UL    /* Clock consumers of FLEXSPI_CLK_ROOT output : FLEXSPI */
#define BOARD_BOOTCLOCKRUN_528M_GPT1_IPG_CLK_HIGHFREQ 66000000UL     /* Clock consumers of GPT1_ipg_clk_highfreq output : GPT1 */
#define BOARD_BOOTCLOCKRUN_528M_GPT2_IPG_CLK_HIGHFREQ 66000000UL     /* Clock consumers of GPT2_ipg_clk_highfreq output : GPT2 */
#define BOARD_BOOTCLOCKRUN_528M_IPG_CLK_ROOT          132000000UL    /* Clock consumers of IPG_CLK_ROOT output : ADC1, ADC2, ADC_ETC, AOI1, AOI2, ARM, BEE, CAN1, CAN2, CAN3, CCM, CMP1, CMP2, CMP3, CMP4, CSI, CSU, DCDC, DCP, DMA0, DMAMUX, ENC1, ENC2, ENC3, ENC4, ENET, ENET2, EWM, FLEXIO1, FLEXIO2, FLEXIO3, FLEXRAM, FLEXSPI, FLEXSPI2, GPC, GPIO1, GPIO10, GPIO2, GPIO3, GPIO4, GPIO5, IOMUXC, KPP, LCDIF, LPI2C1, LPI2C2, LPI2C3, LPI2C4, LPSPI1, LPSPI2, LPSPI3, LPSPI4, LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8, NVIC, OCOTP, PMU, PWM1, PWM2, PWM3, PWM4, PXP, ROMC, RTWDOG, SAI1, SAI2, SAI3, SNVS, SPDIF, SRC, TEMPMON, TMR1, TMR2, TMR3, TMR4, TRNG, TSC, USB1, USB2, USDHC1, USDHC2, WDOG1, WDOG2, XBARA1, XBARB2, XBARB3 */
#define BOARD_BOOTCLOCKRUN_528M_LCDIF_CLK_ROOT        67500000UL     /* Clock consumers of LCDIF_CLK_ROOT output : LCDIF */
#define BOARD_BOOTCLOCKRUN_528M_LPI2C_CLK_ROOT        60000000UL     /* Clock consumers of LPI2C_CLK_ROOT output : LPI2C1, LPI2C2, LPI2C3, LPI2C4 */
#define BOARD_BOOTCLOCKRUN_528M_LPSPI_CLK_ROOT        105600000UL    /* Clock consumers of LPSPI_CLK_ROOT output : LPSPI1, LPSPI2, LPSPI3, LPSPI4 */
#define BOARD_BOOTCLOCKRUN_528M_LVDS1_CLK             1200000000UL   /* Clock consumers of LVDS1_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_MQS_MCLK              63529411UL     /* Clock consumers of MQS_MCLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_PERCLK_CLK_ROOT       66000000UL     /* Clock consumers of PERCLK_CLK_ROOT output : GPT1, GPT2, PIT */
#define BOARD_BOOTCLOCKRUN_528M_PLL7_MAIN_CLK         24000000UL     /* Clock consumers of PLL7_MAIN_CLK output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_SAI1_CLK_ROOT         63529411UL     /* Clock consumers of SAI1_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_SAI1_MCLK1            63529411UL     /* Clock consumers of SAI1_MCLK1 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_528M_SAI1_MCLK2            63529411UL     /* Clock consumers of SAI1_MCLK2 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_528M_SAI1_MCLK3            30000000UL     /* Clock consumers of SAI1_MCLK3 output : SAI1 */
#define BOARD_BOOTCLOCKRUN_528M_SAI2_CLK_ROOT         63529411UL     /* Clock consumers of SAI2_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_SAI2_MCLK1            63529411UL     /* Clock consumers of SAI2_MCLK1 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_528M_SAI2_MCLK2            0UL            /* Clock consumers of SAI2_MCLK2 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_528M_SAI2_MCLK3            30000000UL     /* Clock consumers of SAI2_MCLK3 output : SAI2 */
#define BOARD_BOOTCLOCKRUN_528M_SAI3_CLK_ROOT         63529411UL     /* Clock consumers of SAI3_CLK_ROOT output : N/A */
#define BOARD_BOOTCLOCKRUN_528M_SAI3_MCLK1            63529411UL     /* Clock consumers of SAI3_MCLK1 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_528M_SAI3_MCLK2            0UL            /* Clock consumers of SAI3_MCLK2 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_528M_SAI3_MCLK3            30000000UL     /* Clock consumers of SAI3_MCLK3 output : SAI3 */
#define BOARD_BOOTCLOCKRUN_528M_SEMC_CLK_ROOT         66000000UL     /* Clock consumers of SEMC_CLK_ROOT output : SEMC */
#define BOARD_BOOTCLOCKRUN_528M_SPDIF0_CLK_ROOT       30000000UL     /* Clock consumers of SPDIF0_CLK_ROOT output : SPDIF */
#define BOARD_BOOTCLOCKRUN_528M_SPDIF0_EXTCLK_OUT     0UL            /* Clock consumers of SPDIF0_EXTCLK_OUT output : SPDIF */
#define BOARD_BOOTCLOCKRUN_528M_TRACE_CLK_ROOT        132000000UL    /* Clock consumers of TRACE_CLK_ROOT output : ARM */
#define BOARD_BOOTCLOCKRUN_528M_UART_CLK_ROOT         80000000UL     /* Clock consumers of UART_CLK_ROOT output : LPUART1, LPUART2, LPUART3, LPUART4, LPUART5, LPUART6, LPUART7, LPUART8 */
#define BOARD_BOOTCLOCKRUN_528M_USBPHY1_CLK           0UL            /* Clock consumers of USBPHY1_CLK output : TEMPMON, USB1 */
#define BOARD_BOOTCLOCKRUN_528M_USBPHY2_CLK           0UL            /* Clock consumers of USBPHY2_CLK output : USB2 */
#define BOARD_BOOTCLOCKRUN_528M_USDHC1_CLK_ROOT       198000000UL    /* Clock consumers of USDHC1_CLK_ROOT output : USDHC1 */
#define BOARD_BOOTCLOCKRUN_528M_USDHC2_CLK_ROOT       198000000UL    /* Clock consumers of USDHC2_CLK_ROOT output : USDHC2 */

/*! @brief Arm PLL set for BOARD_BootClockRUN_528M configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_BOARD_BootClockRUN_528M;
/*! @brief Usb1 PLL set for BOARD_BootClockRUN_528M configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_BOARD_BootClockRUN_528M;
/*! @brief Sys PLL for BOARD_BootClockRUN_528M configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_BOARD_BootClockRUN_528M;
/*! @brief Video PLL set for BOARD_BootClockRUN_528M configuration.
 */
extern const clock_video_pll_config_t videoPllConfig_BOARD_BootClockRUN_528M;

/*******************************************************************************
 * API for BOARD_BootClockRUN_528M configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void BOARD_BootClockRUN_528M(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

