/*
** ###################################################################
**     Version:             rev. 1.0, 2021-08-03
**     Build:               b240116
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2024 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2021-08-03)
**         Initial version based on SPEC1.6
**
** ###################################################################
*/

#ifndef _MCXN947_cm33_core0_FEATURES_H_
#define _MCXN947_cm33_core0_FEATURES_H_

/* SOC module features */

/* @brief CACHE64_CTRL availability on the SoC. */
#define FSL_FEATURE_SOC_CACHE64_CTRL_COUNT (1)
/* @brief CACHE64_POLSEL availability on the SoC. */
#define FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT (1)
/* @brief CDOG availability on the SoC. */
#define FSL_FEATURE_SOC_CDOG_COUNT (2)
/* @brief CMC availability on the SoC. */
#define FSL_FEATURE_SOC_CMC_COUNT (1)
/* @brief CRC availability on the SoC. */
#define FSL_FEATURE_SOC_CRC_COUNT (1)
/* @brief CTIMER availability on the SoC. */
#define FSL_FEATURE_SOC_CTIMER_COUNT (5)
/* @brief EDMA availability on the SoC. */
#define FSL_FEATURE_SOC_EDMA_COUNT (2)
/* @brief EIM availability on the SoC. */
#define FSL_FEATURE_SOC_EIM_COUNT (1)
/* @brief EMVSIM availability on the SoC. */
#define FSL_FEATURE_SOC_EMVSIM_COUNT (2)
/* @brief ENC availability on the SoC. */
#define FSL_FEATURE_SOC_ENC_COUNT (2)
/* @brief EVTG availability on the SoC. */
#define FSL_FEATURE_SOC_EVTG_COUNT (1)
/* @brief EWM availability on the SoC. */
#define FSL_FEATURE_SOC_EWM_COUNT (1)
/* @brief FLEXCAN availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCAN_COUNT (2)
/* @brief FLEXIO availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXIO_COUNT (1)
/* @brief FLEXSPI availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXSPI_COUNT (1)
/* @brief FMC availability on the SoC. */
#define FSL_FEATURE_SOC_FMC_COUNT (1)
/* @brief FREQME availability on the SoC. */
#define FSL_FEATURE_SOC_FREQME_COUNT (1)
/* @brief GPIO availability on the SoC. */
#define FSL_FEATURE_SOC_GPIO_COUNT (12)
/* @brief SPC availability on the SoC. */
#define FSL_FEATURE_SOC_SPC_COUNT (1)
/* @brief HPDAC availability on the SoC. */
#define FSL_FEATURE_SOC_HPDAC_COUNT (1)
/* @brief I3C availability on the SoC. */
#define FSL_FEATURE_SOC_I3C_COUNT (2)
/* @brief I2S availability on the SoC. */
#define FSL_FEATURE_SOC_I2S_COUNT (2)
/* @brief INPUTMUX availability on the SoC. */
#define FSL_FEATURE_SOC_INPUTMUX_COUNT (1)
/* @brief ITRC availability on the SoC. */
#define FSL_FEATURE_SOC_ITRC_COUNT (1)
/* @brief LPADC availability on the SoC. */
#define FSL_FEATURE_SOC_LPADC_COUNT (2)
/* @brief LPCMP availability on the SoC. */
#define FSL_FEATURE_SOC_LPCMP_COUNT (3)
/* @brief LPDAC availability on the SoC. */
#define FSL_FEATURE_SOC_LPDAC_COUNT (2)
/* @brief LPI2C availability on the SoC. */
#define FSL_FEATURE_SOC_LPI2C_COUNT (10)
/* @brief LPSPI availability on the SoC. */
#define FSL_FEATURE_SOC_LPSPI_COUNT (10)
/* @brief LPTMR availability on the SoC. */
#define FSL_FEATURE_SOC_LPTMR_COUNT (2)
/* @brief LPUART availability on the SoC. */
#define FSL_FEATURE_SOC_LPUART_COUNT (10)
/* @brief MAILBOX availability on the SoC. */
#define FSL_FEATURE_SOC_MAILBOX_COUNT (1)
/* @brief MCX_ENET availability on the SoC. */
#define FSL_FEATURE_SOC_MCX_ENET_COUNT (1)
/* @brief MPU availability on the SoC. */
#define FSL_FEATURE_SOC_MPU_COUNT (1)
/* @brief MRT availability on the SoC. */
#define FSL_FEATURE_SOC_MRT_COUNT (1)
/* @brief OPAMP availability on the SoC. */
#define FSL_FEATURE_SOC_OPAMP_COUNT (3)
/* @brief OSTIMER availability on the SoC. */
#define FSL_FEATURE_SOC_OSTIMER_COUNT (1)
/* @brief PDM availability on the SoC. */
#define FSL_FEATURE_SOC_PDM_COUNT (1)
/* @brief PINT availability on the SoC. */
#define FSL_FEATURE_SOC_PINT_COUNT (1)
/* @brief PKC availability on the SoC. */
#define FSL_FEATURE_SOC_PKC_COUNT (1)
/* @brief POWERQUAD availability on the SoC. */
#define FSL_FEATURE_SOC_POWERQUAD_COUNT (1)
/* @brief PORT availability on the SoC. */
#define FSL_FEATURE_SOC_PORT_COUNT (6)
/* @brief PWM availability on the SoC. */
#define FSL_FEATURE_SOC_PWM_COUNT (2)
/* @brief PUF availability on the SoC. */
#define FSL_FEATURE_SOC_PUF_COUNT (4)
/* @brief RTC availability on the SoC. */
#define FSL_FEATURE_SOC_RTC_COUNT (1)
/* @brief SCG availability on the SoC. */
#define FSL_FEATURE_SOC_SCG_COUNT (1)
/* @brief SCT availability on the SoC. */
#define FSL_FEATURE_SOC_SCT_COUNT (1)
/* @brief SEMA42 availability on the SoC. */
#define FSL_FEATURE_SOC_SEMA42_COUNT (1)
/* @brief SINC availability on the SoC. */
#define FSL_FEATURE_SOC_SINC_COUNT (1)
/* @brief SMARTDMA availability on the SoC. */
#define FSL_FEATURE_SOC_SMARTDMA_COUNT (1)
/* @brief SYSCON availability on the SoC. */
#define FSL_FEATURE_SOC_SYSCON_COUNT (1)
/* @brief SYSPM availability on the SoC. */
#define FSL_FEATURE_SOC_SYSPM_COUNT (2)
/* @brief TSI availability on the SoC. */
#define FSL_FEATURE_SOC_TSI_COUNT (1)
/* @brief USB availability on the SoC. */
#define FSL_FEATURE_SOC_USB_COUNT (1)
/* @brief USBC availability on the SoC. */
#define FSL_FEATURE_SOC_USBC_COUNT (1)
/* @brief USBHSDCD availability on the SoC. */
#define FSL_FEATURE_SOC_USBHSDCD_COUNT (2)
/* @brief USBNC availability on the SoC. */
#define FSL_FEATURE_SOC_USBNC_COUNT (1)
/* @brief USBPHY availability on the SoC. */
#define FSL_FEATURE_SOC_USBPHY_COUNT (1)
/* @brief USDHC availability on the SoC. */
#define FSL_FEATURE_SOC_USDHC_COUNT (1)
/* @brief UTICK availability on the SoC. */
#define FSL_FEATURE_SOC_UTICK_COUNT (1)
/* @brief VREF availability on the SoC. */
#define FSL_FEATURE_SOC_VREF_COUNT (1)
/* @brief WWDT availability on the SoC. */
#define FSL_FEATURE_SOC_WWDT_COUNT (2)
/* @brief WUU availability on the SoC. */
#define FSL_FEATURE_SOC_WUU_COUNT (1)

/* LPADC module features */

/* @brief FIFO availability on the SoC. */
#define FSL_FEATURE_LPADC_FIFO_COUNT (2)
/* @brief Has subsequent trigger priority (bitfield CFG[TPRICTRL]). */
#define FSL_FEATURE_LPADC_HAS_CFG_SUBSEQUENT_PRIORITY (1)
/* @brief Has differential mode (bitfield CMDLn[DIFF]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_DIFF (0)
/* @brief Has channel scale (bitfield CMDLn[CSCALE]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_CSCALE (0)
/* @brief Has conversion type select (bitfield CMDLn[CTYPE]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_CTYPE (1)
/* @brief Has conversion resolution select  (bitfield CMDLn[MODE]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_MODE (1)
/* @brief Has compare function enable (bitfield CMDHn[CMPEN]). */
#define FSL_FEATURE_LPADC_HAS_CMDH_CMPEN (1)
/* @brief Has Wait for trigger assertion before execution (bitfield CMDHn[WAIT_TRIG]). */
#define FSL_FEATURE_LPADC_HAS_CMDH_WAIT_TRIG (1)
/* @brief Has offset calibration (bitfield CTRL[CALOFS]). */
#define FSL_FEATURE_LPADC_HAS_CTRL_CALOFS (1)
/* @brief Has gain calibration (bitfield CTRL[CAL_REQ]). */
#define FSL_FEATURE_LPADC_HAS_CTRL_CAL_REQ (1)
/* @brief Has calibration average (bitfield CTRL[CAL_AVGS]). */
#define FSL_FEATURE_LPADC_HAS_CTRL_CAL_AVGS (1)
/* @brief Has internal clock (bitfield CFG[ADCKEN]). */
#define FSL_FEATURE_LPADC_HAS_CFG_ADCKEN (0)
/* @brief Enable support for low voltage reference on option 1 reference (bitfield CFG[VREF1RNG]). */
#define FSL_FEATURE_LPADC_HAS_CFG_VREF1RNG (0)
/* @brief Has calibration (bitfield CFG[CALOFS]). */
#define FSL_FEATURE_LPADC_HAS_CFG_CALOFS (0)
/* @brief Has offset trim (register OFSTRIM). */
#define FSL_FEATURE_LPADC_HAS_OFSTRIM (1)
/* @brief OFSTRIM availability on the SoC. */
#define FSL_FEATURE_LPADC_OFSTRIM_COUNT (2)
/* @brief Has Trigger status register. */
#define FSL_FEATURE_LPADC_HAS_TSTAT (1)
/* @brief Has power select (bitfield CFG[PWRSEL]). */
#define FSL_FEATURE_LPADC_HAS_CFG_PWRSEL (1)
/* @brief Has alternate channel B scale (bitfield CMDLn[ALTB_CSCALE]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_ALTB_CSCALE (0)
/* @brief Has alternate channel B select enable (bitfield CMDLn[ALTBEN]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_ALTBEN (1)
/* @brief Has alternate channel input (bitfield CMDLn[ALTB_ADCH]). */
#define FSL_FEATURE_LPADC_HAS_CMDL_ALTB_ADCH (1)
/* @brief Has offset calibration mode (bitfield CTRL[CALOFSMODE]). */
#define FSL_FEATURE_LPADC_HAS_CTRL_CALOFSMODE (0)
/* @brief Conversion averaged bitfiled width. */
#define FSL_FEATURE_LPADC_CONVERSIONS_AVERAGED_BITFIELD_WIDTH (4)
/* @brief Has B side channels. */
#define FSL_FEATURE_LPADC_HAS_B_SIDE_CHANNELS (1)
/* @brief Indicate whether the LPADC STAT register has trigger exception interrupt function (bitfield STAT[TEXC_INT]). */
#define FSL_FEATURE_LPADC_HAS_STAT_TEXC_INT (1)
/* @brief Indicate whether the LPADC STAT register has trigger completion interrupt function (bitfield STAT[TCOMP_INT]). */
#define FSL_FEATURE_LPADC_HAS_STAT_TCOMP_INT (1)
/* @brief Indicate whether the LPADC STAT register has calibration ready function (bitfield STAT[CAL_RDY]). */
#define FSL_FEATURE_LPADC_HAS_STAT_CAL_RDY (1)
/* @brief Indicate whether the LPADC STAT register has ADC active function (bitfield STAT[ADC_ACTIVE]). */
#define FSL_FEATURE_LPADC_HAS_STAT_ADC_ACTIVE (1)
/* @brief Indicate whether the LPADC IE register has trigger exception interrupt enable function (bitfield IE[TEXC_IE]). */
#define FSL_FEATURE_LPADC_HAS_IE_TEXC_IE (1)
/* @brief Indicate whether the LPADC IE register has trigger completion interrupt enable function (bitfield IE[TCOMP_IE]). */
#define FSL_FEATURE_LPADC_HAS_IE_TCOMP_IE (1)
/* @brief Indicate whether the LPADC CFG register has trigger resume/restart enable function (bitfield CFG[TRES]). */
#define FSL_FEATURE_LPADC_HAS_CFG_TRES (1)
/* @brief Indicate whether the LPADC CFG register has trigger command resume/restart enable function (bitfield CFG[TCMDRES]). */
#define FSL_FEATURE_LPADC_HAS_CFG_TCMDRES (1)
/* @brief Indicate whether the LPADC CFG register has high priority trigger exception disable function (bitfield CFG[HPT_EXDI]). */
#define FSL_FEATURE_LPADC_HAS_CFG_HPT_EXDI (1)
/* @brief Indicate LPADC CFG register TPRICTRL bitfield width. */
#define FSL_FEATURE_LPADC_CFG_TPRICTRL_BITFIELD_WIDTH (2)
/* @brief Temperature sensor parameter A (slope). */
#define FSL_FEATURE_LPADC_TEMP_PARAMETER_A (783U)
/* @brief Temperature sensor parameter B (offset). */
#define FSL_FEATURE_LPADC_TEMP_PARAMETER_B (297U)
/* @brief Temperature sensor parameter Alpha. */
#define FSL_FEATURE_LPADC_TEMP_PARAMETER_ALPHA (9.63f)
/* @brief The buffer size of temperature sensor. */
#define FSL_FEATURE_LPADC_TEMP_SENS_BUFFER_SIZE (2U)

/* CACHE64_CTRL module features */

/* @brief Cache Line size in byte. */
#define FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE (32)

/* CACHE64_POLSEL module features */

/* No feature definitions */

/* FLEXCAN module features */

/* @brief Has more than 64 MBs. */
#define FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB (0)
/* @brief Message buffer size */
#define FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(x) (32)
/* @brief Has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT (0)
/* @brief Insatnce has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_DOZE_MODE_SUPPORTn(x) (0)
/* @brief Has a glitch filter on the receive pin (register bit field MCR[WAKSRC]). */
#define FSL_FEATURE_FLEXCAN_HAS_GLITCH_FILTER (1)
/* @brief Has extended interrupt mask and flag register (register IMASK2, IFLAG2). */
#define FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER (0)
/* @brief Instance has extended bit timing register (register CBT). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_EXTENDED_TIMING_REGISTERn(x) (1)
/* @brief Has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA (1)
/* @brief Instance has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_RX_FIFO_DMAn(x) (1)
/* @brief Remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE (1)
/* @brief Instance remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_SUPPORT_ENGINE_CLK_SEL_REMOVEn(x) (1)
/* @brief Is affected by errata with ID 5641 (Module does not transmit a message that is enabled to be transmitted at a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5641 (0)
/* @brief Is affected by errata with ID 5829 (FlexCAN: FlexCAN does not transmit a message that is enabled to be transmitted in a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829 (0)
/* @brief Is affected by errata with ID 6032 (FlexCAN: A frame with wrong ID or payload is transmitted into the CAN bus when the Message Buffer under transmission is either aborted or deactivated while the CAN bus is in the Bus Idle state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_6032 (0)
/* @brief Is affected by errata with ID 9595 (FlexCAN: Corrupt frame possible if the Freeze Mode or the Low-Power Mode are entered during a Bus-Off state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_9595 (0)
/* @brief Has CAN with Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE (1)
/* @brief CAN instance support Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_FLEXIBLE_DATA_RATEn(x) (1)
/* @brief Has memory error control (register MECR). */
#define FSL_FEATURE_FLEXCAN_HAS_MEMORY_ERROR_CONTROL (0)
/* @brief Has enhanced bit timing register (register EPRS, ENCBT, EDCBT and ETDC). */
#define FSL_FEATURE_FLEXCAN_HAS_ENHANCED_BIT_TIMING_REG (1)
/* @brief Has Pretended Networking mode support. */
#define FSL_FEATURE_FLEXCAN_HAS_PN_MODE (1)
/* @brief Has Enhanced Rx FIFO. */
#define FSL_FEATURE_FLEXCAN_HAS_ENHANCED_RX_FIFO (1)
/* @brief Enhanced Rx FIFO size (Indicates how many CAN FD messages can be stored). */
#define FSL_FEATURE_FLEXCAN_HAS_ENHANCED_RX_FIFO_SIZE (12)
/* @brief The number of enhanced Rx FIFO filter element registers. */
#define FSL_FEATURE_FLEXCAN_HAS_ENHANCED_RX_FIFO_FILTER_MAX_NUMBER (32)
/* @brief Does not support Supervisor Mode (bitfield MCR[SUPV]. */
#define FSL_FEATURE_FLEXCAN_HAS_NO_SUPV_SUPPORT (1)
/* @brief FlexCAN maximum data rate. */
#define FSL_FEATURE_FLEXCAN_MAX_CANFD_BITRATE (10000000)

/* CDOG module features */

/* @brief CDOG Has No Reset */
#define FSL_FEATURE_CDOG_HAS_NO_RESET (1)

/* CMC module features */

/* @brief Has SRAM_DIS register */
#define FSL_FEATURE_MCX_CMC_HAS_SRAM_DIS_REG (1)
/* @brief Has BSR register */
#define FSL_FEATURE_MCX_CMC_HAS_BSR_REG (1)
/* @brief Has RSTCNT register */
#define FSL_FEATURE_MCX_CMC_HAS_RSTCNT_REG (1)
/* @brief Has BLR register */
#define FSL_FEATURE_MCX_CMC_HAS_BLR_REG (1)
/* @brief Has no bitfield FLASHWAKE in FLASHCR register */
#define FSL_FEATURE_MCX_CMC_HAS_NO_FLASHCR_WAKE (1)

/* LPCMP module features */

/* @brief Has CCR1 FUNC_CLK_SEL bitfield. */
#define FSL_FEATURE_LPCMP_HAS_CCR1_FUNC_CLK_SEL (1)
/* @brief Has IER RRF_IE bitfield. */
#define FSL_FEATURE_LPCMP_HAS_IER_RRF_IE (1)
/* @brief Has CSR RRF bitfield. */
#define FSL_FEATURE_LPCMP_HAS_CSR_RRF (1)
/* @brief Has Round Robin mode (related to existence of registers RRCR0). */
#define FSL_FEATURE_LPCMP_HAS_ROUNDROBIN_MODE (1)
/* @brief Has window mode (related to existence of CCR1.WINDOW_CLS). */
#define FSL_FEATURE_LPCMP_HAS_WINDOW_CONTROL (1)
/* @brief Has no CCR0 CMP_STOP_EN bitfield. */
#define FSL_FEATURE_LPCMP_HAS_NO_CCR0_CMP_STOP_EN (1)

/* SYSPM module features */

/* @brief Temperature sensor parameter A (slope). */
#define FSL_FEATURE_SYSPM_HAS_PMCR_DCIFSH (0)
/* @brief Temperature sensor parameter B (offset). */
#define FSL_FEATURE_SYSPM_HAS_PMCR_RICTR (0)
/* @brief Number of PMCR registers signals number of performance monitors available in single SYSPM instance. */
#define FSL_FEATURE_SYSPM_PMCR_COUNT (1)

/* CTIMER module features */

/* @brief CTIMER has no capture channel. */
#define FSL_FEATURE_CTIMER_HAS_NO_INPUT_CAPTURE (0)
/* @brief CTIMER has no capture 2 interrupt. */
#define FSL_FEATURE_CTIMER_HAS_NO_IR_CR2INT (0)
/* @brief CTIMER capture 3 interrupt. */
#define FSL_FEATURE_CTIMER_HAS_IR_CR3INT (1)
/* @brief Has CTIMER CCR_CAP2 (register bits CCR[CAP2RE][CAP2FE][CAP2I]. */
#define FSL_FEATURE_CTIMER_HAS_NO_CCR_CAP2 (0)
/* @brief Has CTIMER CCR_CAP3 (register bits CCR[CAP3RE][CAP3FE][CAP3I]). */
#define FSL_FEATURE_CTIMER_HAS_CCR_CAP3 (1)
/* @brief CTIMER Has register MSR */
#define FSL_FEATURE_CTIMER_HAS_MSR (1)

/* LPDAC module features */

/* @brief FIFO size. */
#define FSL_FEATURE_LPDAC_FIFO_SIZE (16)
/* @brief Has OPAMP as buffer, speed control signal (bitfield GCR[BUF_SPD_CTRL]). */
#define FSL_FEATURE_LPDAC_HAS_GCR_BUF_SPD_CTRL (1)
/* @brief Buffer Enable(bitfield GCR[BUF_EN]). */
#define FSL_FEATURE_LPDAC_HAS_GCR_BUF_EN (1)
/* @brief RCLK cycles before data latch(bitfield GCR[LATCH_CYC]). */
#define FSL_FEATURE_LPDAC_HAS_GCR_LATCH_CYC (1)
/* @brief VREF source number. */
#define FSL_FEATURE_ANALOG_NUM_OF_VREF_SRC (3)
/* @brief Has internal reference current options. */
#define FSL_FEATURE_LPDAC_HAS_INTERNAL_REFERENCE_CURRENT (1)
/* @brief Support Period trigger mode DAC (bitfield IER[PTGCOCO_IE]). */
#define FSL_FEATURE_LPDAC_HAS_PERIODIC_TRIGGER_MODE (1)

/* EDMA module features */

/* @brief Number of DMA channels (related to number of registers TCD, DCHPRI, bit fields ERQ[ERQn], EEI[EEIn], INT[INTn], ERR[ERRn], HRS[HRSn] and bit field widths ES[ERRCHN], CEEI[CEEI], SEEI[SEEI], CERQ[CERQ], SERQ[SERQ], CDNE[CDNE], SSRT[SSRT], CERR[CERR], CINT[CINT], TCDn_CITER_ELINKYES[LINKCH], TCDn_CSR[MAJORLINKCH], TCDn_BITER_ELINKYES[LINKCH]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL (16)
/* @brief If 8 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_8_BYTES_TRANSFER (1)
/* @brief Number of DMA channel groups (register bit fields CR[ERGA], CR[GRPnPRI], ES[GPE], DCHPRIn[GRPPRI]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT (1)
/* @brief If 16 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_16_BYTES_TRANSFER (1)
/* @brief Has DMA_Error interrupt vector. */
#define FSL_FEATURE_EDMA_HAS_ERROR_IRQ (1)
/* @brief If 64 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_64_BYTES_TRANSFER (0)
/* @brief whether has prot register */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_PROT_REGISTERn(x) (0)
/* @brief If 128 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_128_BYTES_TRANSFER (0)
/* @brief whether has MP channel mux */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_MP_CHANNEL_MUXn(x) (0)
/* @brief If 128 bytes transfer supported. */
#define FSL_FEATURE_EDMA_INSTANCE_SUPPORT_128_BYTES_TRANSFERn(x) (0)
/* @brief If channel clock controlled independently */
#define FSL_FEATURE_EDMA_CHANNEL_HAS_OWN_CLOCK_GATE (1)
/* @brief Has register CH_CSR. */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_CONFIG (1)
/* @brief Number of channel for each EDMA instance, (only defined for soc with different channel numbers for difference instance) */
#define FSL_FEATURE_EDMA_INSTANCE_CHANNELn(x) (16)
/* @brief Has channel mux */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_MUX (1)
/* @brief Has no register bit fields MP_CSR[EBW]. */
#define FSL_FEATURE_EDMA_HAS_NO_MP_CSR_EBW (1)
/* @brief Instance has channel mux */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_CHANNEL_MUXn(x) (1)
/* @brief If dma has common clock gate */
#define FSL_FEATURE_EDMA_HAS_COMMON_CLOCK_GATE (0)
/* @brief Has register CH_SBR. */
#define FSL_FEATURE_EDMA_HAS_SBR (1)
/* @brief If dma channel IRQ support parameter */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL_IRQ_ENTRY_SUPPORT_PARAMETER (0)
/* @brief Has no register bit fields CH_SBR[ATTR]. */
#define FSL_FEATURE_EDMA_HAS_NO_CH_SBR_ATTR (1)
/* @brief NBYTES must be multiple of 8 when using scatter gather. */
#define FSL_FEATURE_EDMA_HAS_ERRATA_51327 (0)
/* @brief Has register bit field CH_CSR[SWAP]. */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SWAP_SIZE (0)
/* @brief NBYTES must be multiple of 8 when using scatter gather. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_ERRATA_51327n(x) (0)
/* @brief Instance has register bit field CH_CSR[SWAP]. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_CHANNEL_SWAP_SIZEn(x) (0)
/* @brief Has register bit fields MP_CSR[GMRC]. */
#define FSL_FEATURE_EDMA_HAS_GLOBAL_MASTER_ID_REPLICATION (1)
/* @brief Has register bit field CH_SBR[INSTR]. */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_ACCESS_TYPE (0)
/* @brief Instance has register bit field CH_SBR[INSTR]. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_CHANNEL_ACCESS_TYPEn(x) (0)
/* @brief Has register bit fields CH_MATTR[WCACHE], CH_MATTR[RCACHE]. */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_MEMORY_ATTRIBUTE  (0)
/* @brief Instance has register CH_MATTR. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_CHANNEL_MEMORY_ATTRIBUTEn(x) (0)
/* @brief Has register bit field CH_CSR[SIGNEXT]. */
#define FSL_FEATURE_EDMA_HAS_CHANNEL_SIGN_EXTENSION (0)
/* @brief Instance Has register bit field CH_CSR[SIGNEXT]. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_CHANNEL_SIGN_EXTENSIONn(x) (0)
/* @brief Has register bit field TCD_CSR[BWC]. */
#define FSL_FEATURE_EDMA_HAS_BANDWIDTH (1)
/* @brief Instance has register bit field TCD_CSR[BWC]. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_BANDWIDTHn(x) (1)
/* @brief Has register bit fields TCD_CSR[TMC]. */
#define FSL_FEATURE_EDMA_HAS_TRANSFER_MODE (0)
/* @brief Instance has register bit fields TCD_CSR[TMC]. */
#define FSL_FEATURE_EDMA_INSTANCE_HAS_TRANSFER_MODEn(x) (0)
/* @brief Has no register bit fields CH_SBR[SEC]. */
#define FSL_FEATURE_EDMA_HAS_NO_CH_SBR_SEC (0)
/* @brief Number of DMA channels with asynchronous request capability. (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT (16)

/* ENC module features */

/* @brief Has no simultaneous PHASEA and PHASEB change interrupt (register bit field CTRL2[SABIE] and CTRL2[SABIRQ]). */
#define FSL_FEATURE_ENC_HAS_NO_CTRL2_SAB_INT (0)
/* @brief Has register CTRL3. */
#define FSL_FEATURE_ENC_HAS_CTRL3 (1)
/* @brief Has register LASTEDGE or LASTEDGEH. */
#define FSL_FEATURE_ENC_HAS_LASTEDGE (1)
/* @brief Has register POSDPERBFR, POSDPERH, or POSDPER. */
#define FSL_FEATURE_ENC_HAS_POSDPER (1)
/* @brief Has bitfiled FILT[FILT_PRSC]. */
#define FSL_FEATURE_ENC_HAS_FILT_PRSC (1)

/* EVTG module features */

/* @brief OPAMP support force bypass */
#define FSL_FEATURE_EVTG_HAS_FORCE_BYPASS_FLIPFLOP (1)

/* FLEXIO module features */

/* @brief Has Shifter Status Register (FLEXIO_SHIFTSTAT) */
#define FSL_FEATURE_FLEXIO_HAS_SHIFTER_STATUS (1)
/* @brief Has Pin Data Input Register (FLEXIO_PIN) */
#define FSL_FEATURE_FLEXIO_HAS_PIN_STATUS (1)
/* @brief Has pin input output related registers */
#define FSL_FEATURE_FLEXIO_HAS_PIN_REGISTER (1)
/* @brief Has Shifter Buffer N Nibble Byte Swapped Register (FLEXIO_SHIFTBUFNBSn) */
#define FSL_FEATURE_FLEXIO_HAS_SHFT_BUFFER_NIBBLE_BYTE_SWAP (1)
/* @brief Has Shifter Buffer N Half Word Swapped Register (FLEXIO_SHIFTBUFHWSn) */
#define FSL_FEATURE_FLEXIO_HAS_SHFT_BUFFER_HALF_WORD_SWAP (1)
/* @brief Has Shifter Buffer N Nibble Swapped Register (FLEXIO_SHIFTBUFNISn) */
#define FSL_FEATURE_FLEXIO_HAS_SHFT_BUFFER_NIBBLE_SWAP (1)
/* @brief Supports Shifter State Mode (FLEXIO_SHIFTCTLn[SMOD]) */
#define FSL_FEATURE_FLEXIO_HAS_STATE_MODE (1)
/* @brief Supports Shifter Logic Mode (FLEXIO_SHIFTCTLn[SMOD]) */
#define FSL_FEATURE_FLEXIO_HAS_LOGIC_MODE (1)
/* @brief Supports paralle width (FLEXIO_SHIFTCFGn[PWIDTH]) */
#define FSL_FEATURE_FLEXIO_HAS_PARALLEL_WIDTH (1)
/* @brief Reset value of the FLEXIO_VERID register */
#define FSL_FEATURE_FLEXIO_VERID_RESET_VALUE (0x2010003)
/* @brief Reset value of the FLEXIO_PARAM register */
#define FSL_FEATURE_FLEXIO_PARAM_RESET_VALUE (0x8200808)
/* @brief Represent the bit width of the TIMDCE field (FLEXIO_TIMCFGLn[TIMDEC]) */
#define FSL_FEATURE_FLEXIO_TIMCFG_TIMDCE_FIELD_WIDTH (3)

/* FLEXSPI module features */

/* @brief FlexSPI AHB buffer count */
#define FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNTn(x) (8)
/* @brief FlexSPI0 and FlexSPI1 have shared IRQ */
#define FSL_FEATURE_FLEXSPI_HAS_SHARED_IRQ0_IRQ1 (0)
/* @brief FlexSPI has no MCR0 ARDFEN bit */
#define FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN (0)
/* @brief FlexSPI has no MCR0 ATDFEN bit */
#define FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN (0)
/* @brief FlexSPI DMA needs multiple DES to transfer */
#define FSL_FEATURE_FLEXSPI_DMA_MULTIPLE_DES (1)

/* GPIO module features */

/* @brief Has GPIO attribute checker register (GACR). */
#define FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER (0)
/* @brief Has GPIO version ID register (VERID). */
#define FSL_FEATURE_GPIO_HAS_VERSION_INFO_REGISTER (1)
/* @brief Has secure/non-secure access protection registers (LOCK, PCNS, PCNP, ICNS, ICNP). */
#define FSL_FEATURE_GPIO_HAS_SECURE_PRIVILEGE_CONTROL (1)
/* @brief Has GPIO port input disable register (PIDR). */
#define FSL_FEATURE_GPIO_HAS_PORT_INPUT_CONTROL (1)
/* @brief Has GPIO interrupt/DMA request/trigger output selection. */
#define FSL_FEATURE_GPIO_HAS_INTERRUPT_CHANNEL_SELECT (1)

/* I3C module features */

/* @brief Has TERM bitfile in MERRWARN register. */
#define FSL_FEATURE_I3C_HAS_NO_MERRWARN_TERM (1)
/* @brief SOC has no reset driver. */
#define FSL_FEATURE_I3C_HAS_NO_RESET (0)
/* @brief Use fixed BAMATCH count, do not provide editable BAMATCH. */
#define FSL_FEATURE_I3C_HAS_NO_SCONFIG_BAMATCH (0)
/* @brief Register SCONFIG do not have IDRAND bitfield. */
#define FSL_FEATURE_I3C_HAS_NO_SCONFIG_IDRAND (0)

/* INPUTMUX module features */

/* @brief Inputmux has DMA Request Enable */
#define FSL_FEATURE_INPUTMUX_HAS_SIGNAL_ENA (1)
/* @brief Inputmux has channel mux control */
#define FSL_FEATURE_INPUTMUX_HAS_CHANNEL_MUX (0)

/* INTM module features */

/* @brief Up to 4 programmable interrupt monitors */
#define FSL_FEATURE_INTM_MONITOR_COUNT (4)

/* LPI2C module features */

/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPI2C_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPI2C_FIFO_SIZEn(x) (8)

/* LPSPI module features */

/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPSPI_FIFO_SIZEn(x) (8)
/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPSPI_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
/* @brief Has CCR1 (related to existence of registers CCR1). */
#define FSL_FEATURE_LPSPI_HAS_CCR1 (1)
/* @brief Has no PCSCFG bit in CFGR1 register */
#define FSL_FEATURE_LPSPI_HAS_NO_PCSCFG (0)
/* @brief Has no WIDTH bits in TCR register */
#define FSL_FEATURE_LPSPI_HAS_NO_MULTI_WIDTH (0)

/* LPTMR module features */

/* @brief Has shared interrupt handler with another LPTMR module. */
#define FSL_FEATURE_LPTMR_HAS_SHARED_IRQ_HANDLER (0)
/* @brief Whether LPTMR counter is 32 bits width. */
#define FSL_FEATURE_LPTMR_CNR_WIDTH_IS_32B (1)
/* @brief Has timer DMA request enable (register bit CSR[TDRE]). */
#define FSL_FEATURE_LPTMR_HAS_CSR_TDRE (1)
/* @brief Do not has prescaler clock source 0. */
#define FSL_FEATURE_LPTMR_HAS_NO_PRESCALER_CLOCK_SOURCE_0_SUPPORT (0)
/* @brief Do not has prescaler clock source 1. */
#define FSL_FEATURE_LPTMR_HAS_NO_PRESCALER_CLOCK_SOURCE_1_SUPPORT (0)
/* @brief Do not has prescaler clock source 2. */
#define FSL_FEATURE_LPTMR_HAS_NO_PRESCALER_CLOCK_SOURCE_2_SUPPORT (0)
/* @brief Do not has prescaler clock source 3. */
#define FSL_FEATURE_LPTMR_HAS_NO_PRESCALER_CLOCK_SOURCE_3_SUPPORT (0)

/* LPUART module features */

/* @brief Has receive FIFO overflow detection (bit field CFIFO[RXOFE]). */
#define FSL_FEATURE_LPUART_HAS_IRQ_EXTENDED_FUNCTIONS (0)
/* @brief Has low power features (can be enabled in wait mode via register bit C1[DOZEEN] or CTRL[DOZEEN] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_LOW_POWER_UART_SUPPORT (1)
/* @brief Has extended data register ED (or extra flags in the DATA register if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_EXTENDED_DATA_REGISTER_FLAGS (1)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPUART_HAS_FIFO (1)
/* @brief Has 32-bit register MODIR */
#define FSL_FEATURE_LPUART_HAS_MODIR (1)
/* @brief Hardware flow control (RTS, CTS) is supported. */
#define FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT (1)
/* @brief Infrared (modulation) is supported. */
#define FSL_FEATURE_LPUART_HAS_IR_SUPPORT (1)
/* @brief 2 bits long stop bit is available. */
#define FSL_FEATURE_LPUART_HAS_STOP_BIT_CONFIG_SUPPORT (1)
/* @brief If 10-bit mode is supported. */
#define FSL_FEATURE_LPUART_HAS_10BIT_DATA_SUPPORT (1)
/* @brief If 7-bit mode is supported. */
#define FSL_FEATURE_LPUART_HAS_7BIT_DATA_SUPPORT (1)
/* @brief Baud rate fine adjustment is available. */
#define FSL_FEATURE_LPUART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT (0)
/* @brief Baud rate oversampling is available (has bit fields C4[OSR], C5[BOTHEDGE], C5[RESYNCDIS] or BAUD[OSR], BAUD[BOTHEDGE], BAUD[RESYNCDIS] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT (1)
/* @brief Baud rate oversampling is available. */
#define FSL_FEATURE_LPUART_HAS_RX_RESYNC_SUPPORT (1)
/* @brief Baud rate oversampling is available. */
#define FSL_FEATURE_LPUART_HAS_BOTH_EDGE_SAMPLING_SUPPORT (1)
/* @brief Peripheral type. */
#define FSL_FEATURE_LPUART_IS_SCI (1)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPUART_FIFO_SIZEn(x) (8)
/* @brief Supports two match addresses to filter incoming frames. */
#define FSL_FEATURE_LPUART_HAS_ADDRESS_MATCHING (1)
/* @brief Has transmitter/receiver DMA enable bits C5[TDMAE]/C5[RDMAE] (or BAUD[TDMAE]/BAUD[RDMAE] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_DMA_ENABLE (1)
/* @brief Has transmitter/receiver DMA select bits C4[TDMAS]/C4[RDMAS], resp. C5[TDMAS]/C5[RDMAS] if IS_SCI = 0. */
#define FSL_FEATURE_LPUART_HAS_DMA_SELECT (0)
/* @brief Data character bit order selection is supported (bit field S2[MSBF] or STAT[MSBF] if the registers are 32-bit wide). */
#define FSL_FEATURE_LPUART_HAS_BIT_ORDER_SELECT (1)
/* @brief Has smart card (ISO7816 protocol) support and no improved smart card support. */
#define FSL_FEATURE_LPUART_HAS_SMART_CARD_SUPPORT (0)
/* @brief Has improved smart card (ISO7816 protocol) support. */
#define FSL_FEATURE_LPUART_HAS_IMPROVED_SMART_CARD_SUPPORT (0)
/* @brief Has local operation network (CEA709.1-B protocol) support. */
#define FSL_FEATURE_LPUART_HAS_LOCAL_OPERATION_NETWORK_SUPPORT (0)
/* @brief Has 32-bit registers (BAUD, STAT, CTRL, DATA, MATCH, MODIR) instead of 8-bit (BDH, BDL, C1, S1, D, etc.). */
#define FSL_FEATURE_LPUART_HAS_32BIT_REGISTERS (1)
/* @brief Lin break detect available (has bit BAUD[LBKDIE]). */
#define FSL_FEATURE_LPUART_HAS_LIN_BREAK_DETECT (1)
/* @brief UART stops in Wait mode available (has bit C1[UARTSWAI]). */
#define FSL_FEATURE_LPUART_HAS_WAIT_MODE_OPERATION (0)
/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPUART_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
/* @brief Has separate RX and TX interrupts. */
#define FSL_FEATURE_LPUART_HAS_SEPARATE_RX_TX_IRQ (0)
/* @brief Has LPAURT_PARAM. */
#define FSL_FEATURE_LPUART_HAS_PARAM (1)
/* @brief Has LPUART_VERID. */
#define FSL_FEATURE_LPUART_HAS_VERID (1)
/* @brief Has LPUART_GLOBAL. */
#define FSL_FEATURE_LPUART_HAS_GLOBAL (1)
/* @brief Has LPUART_PINCFG. */
#define FSL_FEATURE_LPUART_HAS_PINCFG (1)
/* @brief Belong to LPFLEXCOMM */
#define FSL_FEATURE_LPUART_IS_LPFLEXCOMM (1)
/* @brief Has register MODEM Control. */
#define FSL_FEATURE_LPUART_HAS_MCR (1)
/* @brief Has register Half Duplex Control. */
#define FSL_FEATURE_LPUART_HAS_HDCR (1)
/* @brief Has register Timeout. */
#define FSL_FEATURE_LPUART_HAS_TIMEOUT (1)

/* LP_FLEXCOMM module features */

/* No feature definitions */

/* MAILBOX module features */

/* @brief Mailbox side for current core */
#define FSL_FEATURE_MAILBOX_SIDE_A (1)

/* MRT module features */

/* @brief number of channels. */
#define FSL_FEATURE_MRT_NUMBER_OF_CHANNELS  (4)

/* OPAMP module features */

/* @brief Opamp has OPAMP_CTR OUTSW bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_OUTSW (1)
/* @brief Opamp has OPAMP_CTR ADCSW1 bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_ADCSW1 (1)
/* @brief Opamp has OPAMP_CTR ADCSW2 bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_ADCSW2 (1)
/* @brief Opamp has OPAMP_CTR BUFEN bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_BUFEN (1)
/* @brief Opamp has OPAMP_CTR INPSEL bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_INPSEL (1)
/* @brief Opamp has OPAMP_CTR TRIGMD bit */
#define FSL_FEATURE_OPAMP_HAS_OPAMP_CTR_TRIGMD (1)
/* @brief OPAMP support reference buffer */
#define FSL_FEATURE_OPAMP_HAS_SUPPORT_REFERENCE_BUFFER (1)

/* PDM module features */

/* @brief PDM FIFO offset */
#define FSL_FEATURE_PDM_FIFO_OFFSET (4)
/* @brief PDM Channel Number */
#define FSL_FEATURE_PDM_CHANNEL_NUM (4)
/* @brief PDM FIFO WIDTH Size */
#define FSL_FEATURE_PDM_FIFO_WIDTH (4)
/* @brief PDM FIFO DEPTH Size */
#define FSL_FEATURE_PDM_FIFO_DEPTH (16)
/* @brief PDM has RANGE_CTRL register */
#define FSL_FEATURE_PDM_HAS_RANGE_CTRL (1)
/* @brief PDM Has Low Frequency */
#define FSL_FEATURE_PDM_HAS_STATUS_LOW_FREQ (0)
/* @brief PDM Has No VADEF Bitfield In PDM VAD0_STAT Register */
#define FSL_FEATURE_PDM_HAS_NO_VADEF (1)
/* @brief PDM Has no minimum clkdiv */
#define FSL_FEATURE_PDM_HAS_NO_MINIMUM_CLKDIV (1)
/* @brief PDM Has no FIR_RDY Bitfield In PDM STAT Register */
#define FSL_FEATURE_PDM_HAS_NO_FIR_RDY (1)
/* @brief PDM Has no DOZEN Bitfield In PDM CTRL_1 Register */
#define FSL_FEATURE_PDM_HAS_NO_DOZEN (1)
/* @brief PDM Has DEC_BYPASS Bitfield In PDM CTRL_2 Register */
#define FSL_FEATURE_PDM_HAS_DECIMATION_FILTER_BYPASS (0)
/* @brief PDM Has DC_OUT_CTRL */
#define FSL_FEATURE_PDM_HAS_DC_OUT_CTRL (1)
/* @brief PDM Has Fixed DC CTRL VALUE. */
#define FSL_FEATURE_PDM_DC_CTRL_VALUE_FIXED (1)
/* @brief PDM Has no independent error IRQ */
#define FSL_FEATURE_PDM_HAS_NO_INDEPENDENT_ERROR_IRQ (1)
/* @brief PDM has no hardware Voice Activity Detector */
#define FSL_FEATURE_PDM_HAS_NO_HWVAD (1)

/* PINT module features */

/* @brief Number of connected outputs */
#define FSL_FEATURE_PINT_NUMBER_OF_CONNECTED_OUTPUTS (8)
/* @brief PINT Interrupt Combine */
#define FSL_FEATURE_PINT_INTERRUPT_COMBINE (1)

/* PLU module features */

/* @brief Has WAKEINT_CTRL register. */
#define FSL_FEATURE_PLU_HAS_WAKEINT_CTRL_REG (1)

/* PORT module features */

/* @brief Has control lock (register bit PCR[LK]). */
#define FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK (1)
/* @brief Has open drain control (register bit PCR[ODE]). */
#define FSL_FEATURE_PORT_HAS_OPEN_DRAIN (1)
/* @brief Has digital filter (registers DFER, DFCR and DFWR). */
#define FSL_FEATURE_PORT_HAS_DIGITAL_FILTER (0)
/* @brief Has DMA request (register bit field PCR[IRQC] or ICR[IRQC] values). */
#define FSL_FEATURE_PORT_HAS_DMA_REQUEST (0)
/* @brief Has pull resistor selection available. */
#define FSL_FEATURE_PORT_HAS_PULL_SELECTION (1)
/* @brief Has pull resistor enable (register bit PCR[PE]). */
#define FSL_FEATURE_PORT_HAS_PULL_ENABLE (1)
/* @brief Has slew rate control (register bit PCR[SRE]). */
#define FSL_FEATURE_PORT_HAS_SLEW_RATE (1)
/* @brief Has passive filter (register bit field PCR[PFE]). */
#define FSL_FEATURE_PORT_HAS_PASSIVE_FILTER (1)
/* @brief Do not has interrupt control (register ISFR). */
#define FSL_FEATURE_PORT_HAS_NO_INTERRUPT (1)
/* @brief Has pull value (register bit field PCR[PV]). */
#define FSL_FEATURE_PORT_PCR_HAS_PULL_VALUE (1)
/* @brief Has drive strength1 control (register bit PCR[DSE1]). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH1 (0)
/* @brief Has version ID register (register VERID). */
#define FSL_FEATURE_PORT_HAS_VERSION_INFO_REGISTER (1)
/* @brief Has voltage range control (register bit CONFIG[RANGE]). */
#define FSL_FEATURE_PORT_SUPPORT_DIFFERENT_VOLTAGE_RANGE (1)
/* @brief Has EFT detect (registers EDFR, EDIER and EDCR). */
#define FSL_FEATURE_PORT_SUPPORT_EFT (1)
/* @brief Has drive strength control (register bit PCR[DSE]). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH (1)
/* @brief Defines width of PCR[MUX] field. */
#define FSL_FEATURE_PORT_PCR_MUX_WIDTH (4)
/* @brief Has dedicated interrupt vector. */
#define FSL_FEATURE_PORT_HAS_INTERRUPT_VECTOR (1)
/* @brief Has independent interrupt control(register ICR). */
#define FSL_FEATURE_PORT_HAS_INDEPENDENT_INTERRUPT_CONTROL (0)
/* @brief Has multiple pin IRQ configuration (register GICLR and GICHR). */
#define FSL_FEATURE_PORT_HAS_MULTIPLE_IRQ_CONFIG (0)
/* @brief Has Input Buffer Enable (register bit field PCR[IBE]). */
#define FSL_FEATURE_PORT_HAS_INPUT_BUFFER (1)
/* @brief Has Invert Input (register bit field PCR[IBE]). */
#define FSL_FEATURE_PORT_HAS_INVERT_INPUT (1)
/* @brief Defines whether PCR[IRQC] bit-field has flag states. */
#define FSL_FEATURE_PORT_HAS_IRQC_FLAG (0)
/* @brief Defines whether PCR[IRQC] bit-field has trigger states. */
#define FSL_FEATURE_PORT_HAS_IRQC_TRIGGER (0)

/* PUF module features */

/* @brief Puf Activation Code Address. */
#define FSL_FEATURE_PUF_ACTIVATION_CODE_ADDRESS (17826304)
/* @brief Puf Activation Code Size. */
#define FSL_FEATURE_PUF_ACTIVATION_CODE_SIZE (1000)

/* PWM module features */

/* @brief If (e)FlexPWM has module A channels (outputs). */
#define FSL_FEATURE_PWM_HAS_CHANNELA (1)
/* @brief If (e)FlexPWM has module B channels (outputs). */
#define FSL_FEATURE_PWM_HAS_CHANNELB (1)
/* @brief If (e)FlexPWM has module X channels (outputs). */
#define FSL_FEATURE_PWM_HAS_CHANNELX (1)
/* @brief If (e)FlexPWM has fractional feature. */
#define FSL_FEATURE_PWM_HAS_FRACTIONAL (1)
/* @brief If (e)FlexPWM has mux trigger source select bit field. */
#define FSL_FEATURE_PWM_HAS_MUX_TRIGGER_SOURCE_SEL (1)
/* @brief Number of submodules in each (e)FlexPWM module. */
#define FSL_FEATURE_PWM_SUBMODULE_COUNT (4)
/* @brief Number of fault channel in each (e)FlexPWM module. */
#define FSL_FEATURE_PWM_FAULT_CH_COUNT (1)
/* @brief (e)FlexPWM has no WAITEN Bitfield In CTRL2 Register. */
#define FSL_FEATURE_PWM_HAS_NO_WAITEN (1)
/* @brief If (e)FlexPWM has phase delay feature. */
#define FSL_FEATURE_PWM_HAS_PHASE_DELAY (1)
/* @brief If (e)FlexPWM has input filter capture feature. */
#define FSL_FEATURE_PWM_HAS_INPUT_FILTER_CAPTURE (1)
/* @brief If (e)FlexPWM has module capture functionality on A channels (inputs). */
#define FSL_FEATURE_PWM_HAS_CAPTURE_ON_CHANNELA (1)
/* @brief If (e)FlexPWM has module capture functionality on B channels (inputs). */
#define FSL_FEATURE_PWM_HAS_CAPTURE_ON_CHANNELB (1)
/* @brief If (e)FlexPWM has module capture functionality on X channels (inputs). */
#define FSL_FEATURE_PWM_HAS_CAPTURE_ON_CHANNELX (1)

/* RTC module features */

/* @brief Has Tamper Direction Register support. */
#define FSL_FEATURE_RTC_HAS_TAMPER_DIRECTION (0)
/* @brief Has Tamper Queue Status and Control Register support. */
#define FSL_FEATURE_RTC_HAS_TAMPER_QUEUE (0)
/* @brief Has RTC subsystem. */
#define FSL_FEATURE_RTC_HAS_SUBSYSTEM (1)
/* @brief Has RTC Tamper 23 Filter Configuration Register support. */
#define FSL_FEATURE_RTC_HAS_FILTER23_CFG (0)
/* @brief Has WAKEUP_MODE bitfile in CTRL2 register. */
#define FSL_FEATURE_RTC_HAS_NO_CTRL2_WAKEUP_MODE (1)
/* @brief Has CLK_SEL bitfile in CTRL register. */
#define FSL_FEATURE_RTC_HAS_CLOCK_SELECT (1)
/* @brief Has CLKO_DIS bitfile in CTRL register. */
#define FSL_FEATURE_RTC_HAS_CLOCK_OUTPUT_DISABLE (1)
/* @brief Has No Tamper in RTC. */
#define FSL_FEATURE_RTC_HAS_NO_TAMPER_FEATURE (1)
/* @brief Has CPU_LOW_VOLT bitfile in STATUS register. */
#define FSL_FEATURE_RTC_HAS_NO_CPU_LOW_VOLT_FLAG (1)
/* @brief Has RST_SRC bitfile in STATUS register. */
#define FSL_FEATURE_RTC_HAS_NO_RST_SRC_FLAG (1)
/* @brief Has GP_DATA_REG register. */
#define FSL_FEATURE_RTC_HAS_NO_GP_DATA_REG (1)
/* @brief Has TIMER_STB_MASK bitfile in CTRL register. */
#define FSL_FEATURE_RTC_HAS_NO_TIMER_STB_MASK (1)

/* SAI module features */

/* @brief SAI has FIFO in this soc (register bit fields TCR1[TFW]. */
#define FSL_FEATURE_SAI_HAS_FIFO (1)
/* @brief Receive/transmit FIFO size in item count (register bit fields TCSR[FRDE], TCSR[FRIE], TCSR[FRF], TCR1[TFW], RCSR[FRDE], RCSR[FRIE], RCSR[FRF], RCR1[RFW], registers TFRn, RFRn). */
#define FSL_FEATURE_SAI_FIFO_COUNTn(x) (8)
/* @brief Receive/transmit channel number (register bit fields TCR3[TCE], RCR3[RCE], registers TDRn and RDRn). */
#define FSL_FEATURE_SAI_CHANNEL_COUNTn(x) (2)
/* @brief Maximum words per frame (register bit fields TCR3[WDFL], TCR4[FRSZ], TMR[TWM], RCR3[WDFL], RCR4[FRSZ], RMR[RWM]). */
#define FSL_FEATURE_SAI_MAX_WORDS_PER_FRAME (32)
/* @brief Has support of combining multiple data channel FIFOs into single channel FIFO (register bit fields TCR3[CFR], TCR4[FCOMB], TFR0[WCP], TFR1[WCP], RCR3[CFR], RCR4[FCOMB], RFR0[RCP], RFR1[RCP]). */
#define FSL_FEATURE_SAI_HAS_FIFO_COMBINE_MODE (1)
/* @brief Has packing of 8-bit and 16-bit data into each 32-bit FIFO word (register bit fields TCR4[FPACK], RCR4[FPACK]). */
#define FSL_FEATURE_SAI_HAS_FIFO_PACKING (1)
/* @brief Configures when the SAI will continue transmitting after a FIFO error has been detected (register bit fields TCR4[FCONT], RCR4[FCONT]). */
#define FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR (1)
/* @brief Configures if the frame sync is generated internally, a frame sync is only generated when the FIFO warning flag is clear or continuously (register bit fields TCR4[ONDEM], RCR4[ONDEM]). */
#define FSL_FEATURE_SAI_HAS_ON_DEMAND_MODE (1)
/* @brief Simplified bit clock source and asynchronous/synchronous mode selection (register bit fields TCR2[CLKMODE], RCR2[CLKMODE]), in comparison with the exclusively implemented TCR2[SYNC,BCS,BCI,MSEL], RCR2[SYNC,BCS,BCI,MSEL]. */
#define FSL_FEATURE_SAI_HAS_CLOCKING_MODE (0)
/* @brief Has register for configuration of the MCLK divide ratio (register bit fields MDR[FRACT], MDR[DIVIDE]). */
#define FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER (0)
/* @brief Interrupt source number */
#define FSL_FEATURE_SAI_INT_SOURCE_NUM (1)
/* @brief Has register of MCR. */
#define FSL_FEATURE_SAI_HAS_MCR (1)
/* @brief Has bit field MICS of the MCR register. */
#define FSL_FEATURE_SAI_HAS_NO_MCR_MICS (1)
/* @brief Has register of MDR */
#define FSL_FEATURE_SAI_HAS_MDR (0)
/* @brief Has support the BCLK bypass mode when BCLK = MCLK. */
#define FSL_FEATURE_SAI_HAS_BCLK_BYPASS (1)
/* @brief Has DIV bit fields of MCR register (register bit fields MCR[DIV]. */
#define FSL_FEATURE_SAI_HAS_MCR_MCLK_POST_DIV (1)
/* @brief Support Channel Mode (register bit fields TCR4[CHMOD]). */
#define FSL_FEATURE_SAI_HAS_CHANNEL_MODE (1)
/* @brief Support synchronous with another SAI. */
#define FSL_FEATURE_SAI_HAS_SYNC_WITH_ANOTHER_SAI (1)

/* SCT module features */

/* @brief Number of events */
#define FSL_FEATURE_SCT_NUMBER_OF_EVENTS (16)
/* @brief Number of states */
#define FSL_FEATURE_SCT_NUMBER_OF_STATES (16)
/* @brief Number of match capture */
#define FSL_FEATURE_SCT_NUMBER_OF_MATCH_CAPTURE (16)
/* @brief Number of outputs */
#define FSL_FEATURE_SCT_NUMBER_OF_OUTPUTS (10)

/* SEMA42 module features */

/* @brief Gate counts */
#define FSL_FEATURE_SEMA42_GATE_COUNT (16)

/* SINC module features */

/* @brief SINC channel count. */
#define FSL_FEATURE_SINC_CHANNEL_COUNT (5)
/* @brief SINC CACFR register has bitfield ADMASEL. */
#define FSL_FEATURE_SINC_CACFR_HAS_ADMASEL (1)
/* @brief SINC CACFR register has no bitfield PTMUX. */
#define FSL_FEATURE_SINC_CACFR_HAS_NO_PTMUX (1)

/* SPC module features */

/* @brief Has DCDC */
#define FSL_FEATURE_MCX_SPC_HAS_DCDC (1)
/* @brief Has SYS LDO */
#define FSL_FEATURE_MCX_SPC_HAS_SYS_LDO (1)
/* @brief Has IOVDD_LVDF */
#define FSL_FEATURE_MCX_SPC_HAS_IOVDD_VD (1)
/* @brief Has COREVDD_HVDF */
#define FSL_FEATURE_MCX_SPC_HAS_COREVDD_HVD (1)
/* @brief Has CORELDO_VDD_DS */
#define FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS (1)
/* @brief Has LPBUFF_EN */
#define FSL_FEATURE_MCX_SPC_HAS_LPBUFF_EN_BIT (1)
/* @brief Has COREVDD_IVS_EN */
#define FSL_FEATURE_MCX_SPC_HAS_COREVDD_IVS_EN_BIT (1)
/* @brief Has SWITCH_STATE */
#define FSL_FEATURE_MCX_SPC_HAS_SWITCH_STATE_BIT (0)
/* @brief Has SRAMRETLDO */
#define FSL_FEATURE_MCX_SPC_HAS_SRAMRETLDO_REG (0)
/* @brief Has CFG register */
#define FSL_FEATURE_MCX_SPC_HAS_CFG_REG (0)
/* @brief Has SRAMLDO_DPD_ON */
#define FSL_FEATURE_MCX_SPC_HAS_SRAMLDO_DPD_ON_BIT (0)
/* @brief Has CNTRL register */
#define FSL_FEATURE_MCX_SPC_HAS_CNTRL_REG (1)
/* @brief Has DPDOWN_PULLDOWN_DISABLE */
#define FSL_FEATURE_MCX_SPC_HAS_DPDOWN_PULLDOWN_DISABLE_BIT (1)

/* SYSCON module features */

/* @brief Flash page size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES (128)
/* @brief Flash sector size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES (8192)
/* @brief Flash size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES (2097152)
/* @brief Starter register discontinuous. */
#define FSL_FEATURE_SYSCON_STARTER_DISCONTINUOUS (1)
/* @brief Support ROMAPI. */
#define FSL_FEATURE_SYSCON_ROMAPI (1)
/* @brief Powerlib API is different with other series devices.. */
#define FSL_FEATURE_POWERLIB_EXTEND (1)

/* TRDC module features */

/* @brief Process master count. */
#define FSL_FEATURE_TRDC_PROCESSOR_MASTER_COUNT (2)
/* @brief TRDC instance has PID configuration or not. */
#define FSL_FEATURE_TRDC_INSTANCE_HAS_PID_CONFIGURATIONn(x) (0)
/* @brief TRDC instance has MBC. */
#define FSL_FEATURE_TRDC_HAS_MBC (1)
/* @brief TRDC instance has MRC. */
#define FSL_FEATURE_TRDC_HAS_MRC (0)
/* @brief TRDC instance has TRDC_CR. */
#define FSL_FEATURE_TRDC_HAS_GENERAL_CONFIG (0)
/* @brief TRDC instance has MDA_Wx_y_DFMT. */
#define FSL_FEATURE_TRDC_HAS_DOMAIN_ASSIGNMENT (0)
/* @brief TRDC instance has TRDC_FDID. */
#define FSL_FEATURE_TRDC_HAS_DOMAIN_ERROR (0)
/* @brief TRDC instance has TRDC_FLW_CTL. */
#define FSL_FEATURE_TRDC_HAS_FLW (0)

/* TSI module features */

/* @brief TSI Version */
#define FSL_FEATURE_TSI_VERSION (6U)
/* @brief TSI Channel Count */
#define FSL_FEATURE_TSI_CHANNEL_COUNT (25U)

/* USBHSDCD module features */

/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USB_USB_RAM (2048)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS (1074503680)

/* USB module features */

/* @brief KHCI module instance count */
#define FSL_FEATURE_USB_KHCI_COUNT (1)
/* @brief HOST mode enabled */
#define FSL_FEATURE_USB_KHCI_HOST_ENABLED (1)
/* @brief OTG mode enabled */
#define FSL_FEATURE_USB_KHCI_OTG_ENABLED (1)
/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USB_KHCI_USB_RAM (2048)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USB_KHCI_USB_RAM_BASE_ADDRESS (1074503680)
/* @brief Has KEEP_ALIVE_CTRL register */
#define FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED (1)
/* @brief Mode control of the USB Keep Alive */
#define FSL_FEATURE_USB_KHCI_KEEP_ALIVE_MODE_CONTROL (USB_KEEP_ALIVE_CTRL_WAKE_REQ_EN_MASK)
/* @brief Has the Dynamic SOF threshold compare support */
#define FSL_FEATURE_USB_KHCI_DYNAMIC_SOF_THRESHOLD_COMPARE_ENABLED (1)
/* @brief Has the VBUS detect support */
#define FSL_FEATURE_USB_KHCI_VBUS_DETECT_ENABLED (1)
/* @brief Has the IRC48M module clock support */
#define FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED (1)
/* @brief Number of endpoints supported */
#define FSL_FEATURE_USB_ENDPT_COUNT (16)
/* @brief Has STALL_IL/OL_DIS registers */
#define FSL_FEATURE_USB_KHCI_HAS_STALL_LOW (1)
/* @brief Has STALL_IH/OH_DIS registers */
#define FSL_FEATURE_USB_KHCI_HAS_STALL_HIGH (1)

/* USBPHY module features */

/* @brief USBPHY contain DCD analog module */
#define FSL_FEATURE_USBPHY_HAS_DCD_ANALOG (0)
/* @brief USBPHY has register TRIM_OVERRIDE_EN */
#define FSL_FEATURE_USBPHY_HAS_TRIM_OVERRIDE_EN (1)
/* @brief USBPHY is 28FDSOI */
#define FSL_FEATURE_USBPHY_28FDSOI (0)

/* USDHC module features */

/* @brief Has external DMA support (VEND_SPEC[EXT_DMA_EN]) */
#define FSL_FEATURE_USDHC_HAS_EXT_DMA (0)
/* @brief Has HS400 mode (MIX_CTRL[HS400_MODE]) */
#define FSL_FEATURE_USDHC_HAS_HS400_MODE (0)
/* @brief Has SDR50 support (HOST_CTRL_CAP[SDR50_SUPPORT]) */
#define FSL_FEATURE_USDHC_HAS_SDR50_MODE (1)
/* @brief Has SDR104 support (HOST_CTRL_CAP[SDR104_SUPPORT]) */
#define FSL_FEATURE_USDHC_HAS_SDR104_MODE (1)
/* @brief USDHC has reset control */
#define FSL_FEATURE_USDHC_HAS_RESET (0)
/* @brief USDHC has no bitfield WTMK_LVL[WR_BRST_LEN] and WTMK_LVL[RD_BRST_LEN] */
#define FSL_FEATURE_USDHC_HAS_NO_RW_BURST_LEN (0)
/* @brief If USDHC instance support 8 bit width */
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_8_BIT_WIDTHn(x) (1)
/* @brief If USDHC instance support HS400 mode */
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_HS400_MODEn(x) (0)
/* @brief If USDHC instance support 1v8 signal */
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_1V8_SIGNALn(x) (1)
/* @brief Has no retuning time counter (HOST_CTRL_CAP[TIME_COUNT_RETURNING]) */
#define FSL_FEATURE_USDHC_REGISTER_HOST_CTRL_CAP_HAS_NO_RETUNING_TIME_COUNTER (1)
/* @brief Has no VSELECT bit in VEND_SPEC register */
#define FSL_FEATURE_USDHC_HAS_NO_VOLTAGE_SELECT (1)
/* @brief Has no VS18 bit in HOST_CTRL_CAP register */
#define FSL_FEATURE_USDHC_HAS_NO_VS18 (0)

/* UTICK module features */

/* @brief UTICK does not support PD configure. */
#define FSL_FEATURE_UTICK_HAS_NO_PDCFG (1)

/* VBAT module features */

/* @brief Has STATUS register */
#define FSL_FEATURE_MCX_VBAT_HAS_STATUS_REG (1)
/* @brief Has TAMPER register */
#define FSL_FEATURE_MCX_VBAT_HAS_TAMPER_REG (1)
/* @brief Has BANDGAP register */
#define FSL_FEATURE_MCX_VBAT_HAS_BANDGAP_TIMER (1)
/* @brief Has LDOCTL register */
#define FSL_FEATURE_MCX_VBAT_HAS_LDOCTL_REG (1)
/* @brief Has OSCCTL register */
#define FSL_FEATURE_MCX_VBAT_HAS_OSCCTL_REG (1)
/* @brief Has SWICTL register */
#define FSL_FEATURE_MCX_VBAT_HAS_SWICTL_REG (1)
/* @brief Has CLKMON register */
#define FSL_FEATURE_MCX_VBAT_HAS_CLKMON_REG (0)
/* @brief Has FINE_AMP_GAIN bitfield in register OSCCTLA */
#define FSL_FEATURE_MCX_VBAT_HAS_OSCCTLA_FINE_AMP_GAIN_BIT (0)

/* WWDT module features */

/* @brief Has no RESET register. */
#define FSL_FEATURE_WWDT_HAS_NO_RESET (1)
/* @brief WWDT does not support power down configure */
#define FSL_FEATURE_WWDT_HAS_NO_PDCFG (1)

#endif /* _MCXN947_cm33_core0_FEATURES_H_ */

