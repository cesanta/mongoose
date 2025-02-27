/*
** ###################################################################
**     Version:             rev. 2.0, 2022-03-25
**     Build:               b230419
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2023 NXP
**     SPDX-License-Identifier: BSD-3-Clause
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 0.1 (2017-01-10)
**         Initial version.
**     - rev. 1.0 (2018-11-16)
**         Update feature files to align with IMXRT1060RM Rev.0.
**     - rev. 2.0 (2022-03-25)
**         Add RT1060X device
**
** ###################################################################
*/

#ifndef _MIMXRT1062_FEATURES_H_
#define _MIMXRT1062_FEATURES_H_

/* SOC module features */

/* @brief ADC availability on the SoC. */
#define FSL_FEATURE_SOC_ADC_COUNT (2)
/* @brief AIPSTZ availability on the SoC. */
#define FSL_FEATURE_SOC_AIPSTZ_COUNT (4)
/* @brief AOI availability on the SoC. */
#define FSL_FEATURE_SOC_AOI_COUNT (2)
/* @brief CCM availability on the SoC. */
#define FSL_FEATURE_SOC_CCM_COUNT (1)
/* @brief CCM_ANALOG availability on the SoC. */
#define FSL_FEATURE_SOC_CCM_ANALOG_COUNT (1)
/* @brief CMP availability on the SoC. */
#define FSL_FEATURE_SOC_CMP_COUNT (4)
/* @brief CSI availability on the SoC. */
#define FSL_FEATURE_SOC_CSI_COUNT (1)
/* @brief DCDC availability on the SoC. */
#define FSL_FEATURE_SOC_DCDC_COUNT (1)
/* @brief DCP availability on the SoC. */
#define FSL_FEATURE_SOC_DCP_COUNT (1)
/* @brief DMAMUX availability on the SoC. */
#define FSL_FEATURE_SOC_DMAMUX_COUNT (1)
/* @brief EDMA availability on the SoC. */
#define FSL_FEATURE_SOC_EDMA_COUNT (1)
/* @brief ENC availability on the SoC. */
#define FSL_FEATURE_SOC_ENC_COUNT (4)
/* @brief ENET availability on the SoC. */
#define FSL_FEATURE_SOC_ENET_COUNT (2)
/* @brief EWM availability on the SoC. */
#define FSL_FEATURE_SOC_EWM_COUNT (1)
/* @brief FLEXCAN availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCAN_COUNT (3)
/* @brief FLEXIO availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXIO_COUNT (3)
/* @brief FLEXRAM availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXRAM_COUNT (1)
/* @brief FLEXSPI availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXSPI_COUNT (2)
/* @brief GPC availability on the SoC. */
#define FSL_FEATURE_SOC_GPC_COUNT (1)
/* @brief GPT availability on the SoC. */
#define FSL_FEATURE_SOC_GPT_COUNT (2)
/* @brief I2S availability on the SoC. */
#define FSL_FEATURE_SOC_I2S_COUNT (3)
/* @brief IGPIO availability on the SoC. */
#define FSL_FEATURE_SOC_IGPIO_COUNT (10)
/* @brief IOMUXC availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_COUNT (1)
/* @brief IOMUXC_GPR availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_GPR_COUNT (1)
/* @brief IOMUXC_SNVS availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_SNVS_COUNT (1)
/* @brief KPP availability on the SoC. */
#define FSL_FEATURE_SOC_KPP_COUNT (1)
/* @brief LCDIF availability on the SoC. */
#define FSL_FEATURE_SOC_LCDIF_COUNT (1)
/* @brief LPI2C availability on the SoC. */
#define FSL_FEATURE_SOC_LPI2C_COUNT (4)
/* @brief LPSPI availability on the SoC. */
#define FSL_FEATURE_SOC_LPSPI_COUNT (4)
/* @brief LPUART availability on the SoC. */
#define FSL_FEATURE_SOC_LPUART_COUNT (8)
/* @brief MPU availability on the SoC. */
#define FSL_FEATURE_SOC_MPU_COUNT (1)
/* @brief OCOTP availability on the SoC. */
#define FSL_FEATURE_SOC_OCOTP_COUNT (1)
/* @brief PIT availability on the SoC. */
#define FSL_FEATURE_SOC_PIT_COUNT (1)
/* @brief PMU availability on the SoC. */
#define FSL_FEATURE_SOC_PMU_COUNT (1)
/* @brief PWM availability on the SoC. */
#define FSL_FEATURE_SOC_PWM_COUNT (4)
/* @brief PXP availability on the SoC. */
#define FSL_FEATURE_SOC_PXP_COUNT (1)
/* @brief ROMC availability on the SoC. */
#define FSL_FEATURE_SOC_ROMC_COUNT (1)
/* @brief SEMC availability on the SoC. */
#define FSL_FEATURE_SOC_SEMC_COUNT (1)
/* @brief SNVS availability on the SoC. */
#define FSL_FEATURE_SOC_SNVS_COUNT (1)
/* @brief SPDIF availability on the SoC. */
#define FSL_FEATURE_SOC_SPDIF_COUNT (1)
/* @brief SRC availability on the SoC. */
#define FSL_FEATURE_SOC_SRC_COUNT (1)
/* @brief TEMPMON availability on the SoC. */
#define FSL_FEATURE_SOC_TEMPMON_COUNT (1)
/* @brief TMR availability on the SoC. */
#define FSL_FEATURE_SOC_TMR_COUNT (4)
/* @brief TRNG availability on the SoC. */
#define FSL_FEATURE_SOC_TRNG_COUNT (1)
/* @brief TSC availability on the SoC. */
#define FSL_FEATURE_SOC_TSC_COUNT (1)
/* @brief USBHS availability on the SoC. */
#define FSL_FEATURE_SOC_USBHS_COUNT (2)
/* @brief USBNC availability on the SoC. */
#define FSL_FEATURE_SOC_USBNC_COUNT (2)
/* @brief USBPHY availability on the SoC. */
#define FSL_FEATURE_SOC_USBPHY_COUNT (2)
/* @brief USB_ANALOG availability on the SoC. */
#define FSL_FEATURE_SOC_USB_ANALOG_COUNT (1)
/* @brief USDHC availability on the SoC. */
#define FSL_FEATURE_SOC_USDHC_COUNT (2)
/* @brief WDOG availability on the SoC. */
#define FSL_FEATURE_SOC_WDOG_COUNT (2)
/* @brief XBARA availability on the SoC. */
#define FSL_FEATURE_SOC_XBARA_COUNT (1)
/* @brief XBARB availability on the SoC. */
#define FSL_FEATURE_SOC_XBARB_COUNT (2)
/* @brief XTALOSC24M availability on the SoC. */
#define FSL_FEATURE_SOC_XTALOSC24M_COUNT (1)
/* @brief ROM API Availability */
#define FSL_FEATURE_BOOT_ROM_HAS_ROMAPI (1)

/* ADC module features */

/* @brief Remove Hardware Trigger feature. */
#define FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE (0)
/* @brief Remove ALT Clock selection feature. */
#define FSL_FEATURE_ADC_SUPPORT_ALTCLK_REMOVE (1)
/* @brief Conversion control count (related to number of registers HCn and Rn). */
#define FSL_FEATURE_ADC_CONVERSION_CONTROL_COUNT (8)

/* ADC_ETC module features */

/* @brief Has DMA model control(bit field CTRL[DMA_MODE_SEL]). */
#define FSL_FEATURE_ADC_ETC_HAS_CTRL_DMA_MODE_SEL (1)
/* @brief Has TRIGm_CHAIN_a_b IEn_EN. */
#define FSL_FEATURE_ADC_ETC_HAS_TRIGm_CHAIN_a_b_IEn_EN (0)
/* @brief Has no TSC0 trigger related bitfields (bit field CTRL[EXT0_TRIG_ENABLE], CTRL[EXT0_TRIG_PRIORITY]). */
#define FSL_FEATURE_ADC_ETC_HAS_NO_TSC0_TRIG (0)
/* @brief Has no TSC1 trigger related bitfields (bit field CTRL[EXT1_TRIG_ENABLE], CTRL[EXT1_TRIG_PRIORITY]). */
#define FSL_FEATURE_ADC_ETC_HAS_NO_TSC1_TRIG (0)

/* AOI module features */

/* @brief Maximum value of input mux. */
#define FSL_FEATURE_AOI_MODULE_INPUTS (4)
/* @brief Number of events related to number of registers AOIx_BFCRT01n/AOIx_BFCRT23n. */
#define FSL_FEATURE_AOI_EVENT_COUNT (4)

/* FLEXCAN module features */

/* @brief Message buffer size */
#define FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(x) (64)
/* @brief Has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_HAS_DOZE_MODE_SUPPORT (1)
/* @brief Insatnce has doze mode support (register bit field MCR[DOZE]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_DOZE_MODE_SUPPORTn(x) \
    (((x) == CAN1) ? (0) : \
    (((x) == CAN2) ? (0) : \
    (((x) == CAN3) ? (1) : (-1))))
/* @brief Has a glitch filter on the receive pin (register bit field MCR[WAKSRC]). */
#define FSL_FEATURE_FLEXCAN_HAS_GLITCH_FILTER (1)
/* @brief Has extended interrupt mask and flag register (register IMASK2, IFLAG2). */
#define FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER (1)
/* @brief Instance has extended bit timing register (register CBT). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_EXTENDED_TIMING_REGISTERn(x) \
    (((x) == CAN1) ? (0) : \
    (((x) == CAN2) ? (0) : \
    (((x) == CAN3) ? (1) : (-1))))
/* @brief Has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_HAS_RX_FIFO_DMA (1)
/* @brief Instance has a receive FIFO DMA feature (register bit field MCR[DMA]). */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_RX_FIFO_DMAn(x) \
    (((x) == CAN1) ? (0) : \
    (((x) == CAN2) ? (0) : \
    (((x) == CAN3) ? (1) : (-1))))
/* @brief Remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_SUPPORT_ENGINE_CLK_SEL_REMOVE (1)
/* @brief Instance remove CAN Engine Clock Source Selection from unsupported part. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_SUPPORT_ENGINE_CLK_SEL_REMOVEn(x) (1)
/* @brief Is affected by errata with ID 5641 (Module does not transmit a message that is enabled to be transmitted at a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5641 (0)
/* @brief Is affected by errata with ID 5829 (FlexCAN: FlexCAN does not transmit a message that is enabled to be transmitted in a specific moment during the arbitration process). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_5829 (1)
/* @brief Is affected by errata with ID 6032 (FlexCAN: A frame with wrong ID or payload is transmitted into the CAN bus when the Message Buffer under transmission is either aborted or deactivated while the CAN bus is in the Bus Idle state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_6032 (1)
/* @brief Is affected by errata with ID 9595 (FlexCAN: Corrupt frame possible if the Freeze Mode or the Low-Power Mode are entered during a Bus-Off state). */
#define FSL_FEATURE_FLEXCAN_HAS_ERRATA_9595 (1)
/* @brief Has CAN with Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE (1)
/* @brief CAN instance support Flexible Data rate (CAN FD) protocol. */
#define FSL_FEATURE_FLEXCAN_INSTANCE_HAS_FLEXIBLE_DATA_RATEn(x) \
    (((x) == CAN1) ? (0) : \
    (((x) == CAN2) ? (0) : \
    (((x) == CAN3) ? (1) : (-1))))
/* @brief Has memory error control (register MECR). */
#define FSL_FEATURE_FLEXCAN_HAS_MEMORY_ERROR_CONTROL (0)
/* @brief Has enhanced bit timing register (register EPRS, ENCBT, EDCBT and ETDC). */
#define FSL_FEATURE_FLEXCAN_HAS_ENHANCED_BIT_TIMING_REG (0)
/* @brief Has Pretended Networking mode support. */
#define FSL_FEATURE_FLEXCAN_HAS_PN_MODE (0)

/* CCM module features */

/* @brief Is affected by errata with ID 50235 (Incorrect clock setting for CAN affects by LPUART clock gate). */
#define FSL_FEATURE_CCM_HAS_ERRATA_50235 (1)

/* CMP module features */

/* @brief Has Trigger mode in CMP (register bit field CR1[TRIGM]). */
#define FSL_FEATURE_CMP_HAS_TRIGGER_MODE (0)
/* @brief Has Window mode in CMP (register bit field CR1[WE]). */
#define FSL_FEATURE_CMP_HAS_WINDOW_MODE (1)
/* @brief Has External sample supported in CMP (register bit field CR1[SE]). */
#define FSL_FEATURE_CMP_HAS_EXTERNAL_SAMPLE_SUPPORT (1)
/* @brief Has DMA support in CMP (register bit field SCR[DMAEN]). */
#define FSL_FEATURE_CMP_HAS_DMA (1)
/* @brief Has Pass Through mode in CMP (register bit field MUXCR[PSTM]). */
#define FSL_FEATURE_CMP_HAS_PASS_THROUGH_MODE (0)
/* @brief Has DAC Test function in CMP (register DACTEST). */
#define FSL_FEATURE_CMP_HAS_DAC_TEST (0)
/* @brief Has COUTA out of window is zero enable. */
#define FSL_FEATURE_CMP_HAS_COWZ_BIT_FIELD (0)
/* @brief Use 16 bit registers. */
#define FSL_FEATURE_CMP_USE_16BIT_REG (0)

/* DCDC module features */

/* @brief Has CTRL register (register CTRL0/1). */
#define FSL_FEATURE_DCDC_HAS_CTRL_REG (0)
/* @brief DCDC VDD output count. */
#define FSL_FEATURE_DCDC_VDD_OUTPUT_COUNT (1)
/* @brief Has no current alert function (register bit field REG0[CURRENT_ALERT_RESET]). */
#define FSL_FEATURE_DCDC_HAS_NO_CURRENT_ALERT_FUNC (0)
/* @brief Has switching converter differential mode (register bit field REG1[LOOPCTRL_EN_DF_HYST]). */
#define FSL_FEATURE_DCDC_HAS_SWITCHING_CONVERTER_DIFFERENTIAL_MODE (0)
/* @brief Has register bit field REG0[REG_DCDC_IN_DET]. */
#define FSL_FEATURE_DCDC_HAS_REG0_DCDC_IN_DET (0)
/* @brief Has no register bit field REG0[EN_LP_OVERLOAD_SNS]. */
#define FSL_FEATURE_DCDC_HAS_NO_REG0_EN_LP_OVERLOAD_SNS (0)
/* @brief Has register bit field REG3[REG_FBK_SEL]). */
#define FSL_FEATURE_DCDC_HAS_REG3_FBK_SEL (0)

/* EDMA module features */

/* @brief Number of DMA channels (related to number of registers TCD, DCHPRI, bit fields ERQ[ERQn], EEI[EEIn], INT[INTn], ERR[ERRn], HRS[HRSn] and bit field widths ES[ERRCHN], CEEI[CEEI], SEEI[SEEI], CERQ[CERQ], SERQ[SERQ], CDNE[CDNE], SSRT[SSRT], CERR[CERR], CINT[CINT], TCDn_CITER_ELINKYES[LINKCH], TCDn_CSR[MAJORLINKCH], TCDn_BITER_ELINKYES[LINKCH]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL (32)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_EDMA_DMAMUX_CHANNELS (32)
/* @brief Number of DMA channel groups (register bit fields CR[ERGA], CR[GRPnPRI], ES[GPE], DCHPRIn[GRPPRI]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT (1)
/* @brief Has DMA_Error interrupt vector. */
#define FSL_FEATURE_EDMA_HAS_ERROR_IRQ (1)
/* @brief Number of DMA channels with asynchronous request capability (register EARS). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT (32)
/* @brief Channel IRQ entry shared offset. */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL_IRQ_ENTRY_SHARED_OFFSET (16)
/* @brief If 8 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_8_BYTES_TRANSFER (1)
/* @brief If 16 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_16_BYTES_TRANSFER (0)
/* @brief If 32 bytes transfer supported. */
#define FSL_FEATURE_EDMA_SUPPORT_32_BYTES_TRANSFER (1)

/* DMAMUX module features */

/* @brief Number of DMA channels (related to number of register CHCFGn). */
#define FSL_FEATURE_DMAMUX_MODULE_CHANNEL (32)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_DMAMUX_DMAMUX_CHANNELS (32)
/* @brief Has the periodic trigger capability for the triggered DMA channel (register bit CHCFG0[TRIG]). */
#define FSL_FEATURE_DMAMUX_HAS_TRIG (1)
/* @brief Has DMA Channel Always ON function (register bit CHCFG0[A_ON]). */
#define FSL_FEATURE_DMAMUX_HAS_A_ON (1)
/* @brief Register CHCFGn width. */
#define FSL_FEATURE_DMAMUX_CHCFG_REGISTER_WIDTH (32)

/* ENC module features */

/* @brief Has no simultaneous PHASEA and PHASEB change interrupt (register bit field CTRL2[SABIE] and CTRL2[SABIRQ]). */
#define FSL_FEATURE_ENC_HAS_NO_CTRL2_SAB_INT (1)
/* @brief Has register CTRL3. */
#define FSL_FEATURE_ENC_HAS_CTRL3 (0)
/* @brief Has register LASTEDGE or LASTEDGEH. */
#define FSL_FEATURE_ENC_HAS_LASTEDGE (0)
/* @brief Has register POSDPERBFR, POSDPERH, or POSDPER. */
#define FSL_FEATURE_ENC_HAS_POSDPER (0)
/* @brief Has bitfiled FILT[FILT_PRSC]. */
#define FSL_FEATURE_ENC_HAS_FILT_PRSC (1)

/* ENET module features */

/* @brief Support Interrupt Coalesce */
#define FSL_FEATURE_ENET_HAS_INTERRUPT_COALESCE (1)
/* @brief Queue Size. */
#define FSL_FEATURE_ENET_QUEUE (1)
/* @brief Has AVB Support. */
#define FSL_FEATURE_ENET_HAS_AVB (0)
/* @brief Has Timer Pulse Width control. */
#define FSL_FEATURE_ENET_HAS_TIMER_PWCONTROL (1)
/* @brief Has Extend MDIO Support. */
#define FSL_FEATURE_ENET_HAS_EXTEND_MDIO (1)
/* @brief Has Additional 1588 Timer Channel Interrupt. */
#define FSL_FEATURE_ENET_HAS_ADD_1588_TIMER_CHN_INT (0)
/* @brief Support Interrupt Coalesce for each instance */
#define FSL_FEATURE_ENET_INSTANCE_HAS_INTERRUPT_COALESCEn(x) (1)
/* @brief Queue Size for each instance. */
#define FSL_FEATURE_ENET_INSTANCE_QUEUEn(x) (1)
/* @brief Has AVB Support for each instance. */
#define FSL_FEATURE_ENET_INSTANCE_HAS_AVBn(x) (0)
/* @brief Has Timer Pulse Width control for each instance. */
#define FSL_FEATURE_ENET_INSTANCE_HAS_TIMER_PWCONTROLn(x) (1)
/* @brief Has Extend MDIO Support for each instance. */
#define FSL_FEATURE_ENET_INSTANCE_HAS_EXTEND_MDIOn(x) (1)
/* @brief Has Additional 1588 Timer Channel Interrupt for each instance. */
#define FSL_FEATURE_ENET_INSTANCE_HAS_ADD_1588_TIMER_CHN_INTn(x) (0)
/* @brief Has threshold for the number of frames in the receive FIFO (register bit field RSEM[STAT_SECTION_EMPTY]). */
#define FSL_FEATURE_ENET_HAS_RECEIVE_STATUS_THRESHOLD (1)
/* @brief Has trasfer clock delay (register bit field ECR[TXC_DLY]). */
#define FSL_FEATURE_ENET_HAS_RGMII_TXC_DELAY (0)
/* @brief Has receive clock delay (register bit field ECR[RXC_DLY]). */
#define FSL_FEATURE_ENET_HAS_RGMII_RXC_DELAY (0)
/* @brief PTP Timestamp CAPTURE bit always returns 0 when the capture is not over. */
#define FSL_FEATURE_ENET_TIMESTAMP_CAPTURE_BIT_INVALID (0)
/* @brief ENET Has Extra Clock Gate.(RW610). */
#define FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE (0)

/* EWM module features */

/* @brief Has clock select (register CLKCTRL). */
#define FSL_FEATURE_EWM_HAS_CLOCK_SELECT (1)
/* @brief Has clock prescaler (register CLKPRESCALER). */
#define FSL_FEATURE_EWM_HAS_PRESCALER (1)

/* FLEXIO module features */

/* @brief Has Shifter Status Register (FLEXIO_SHIFTSTAT) */
#define FSL_FEATURE_FLEXIO_HAS_SHIFTER_STATUS (1)
/* @brief Has Pin Data Input Register (FLEXIO_PIN) */
#define FSL_FEATURE_FLEXIO_HAS_PIN_STATUS (1)
/* @brief Has pin input output related registers */
#define FSL_FEATURE_FLEXIO_HAS_PIN_REGISTER (0)
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
#define FSL_FEATURE_FLEXIO_VERID_RESET_VALUE (0x1010001)
/* @brief Reset value of the FLEXIO_PARAM register */
#define FSL_FEATURE_FLEXIO_PARAM_RESET_VALUE (0x2200808)
/* @brief Represent the bit width of the TIMDCE field (FLEXIO_TIMCFGLn[TIMDEC]) */
#define FSL_FEATURE_FLEXIO_TIMCFG_TIMDCE_FIELD_WIDTH (2)
/* @brief Flexio DMA request base channel */
#define FSL_FEATURE_FLEXIO_DMA_REQUEST_BASE_CHANNEL (0)

/* FLEXRAM module features */

/* @brief Bank size */
#define FSL_FEATURE_FLEXRAM_INTERNAL_RAM_BANK_SIZE (32768)
/* @brief Total Bank numbers */
#define FSL_FEATURE_FLEXRAM_INTERNAL_RAM_TOTAL_BANK_NUMBERS (16)
/* @brief Has FLEXRAM_MAGIC_ADDR. */
#define FSL_FEATURE_FLEXRAM_HAS_MAGIC_ADDR (0)
/* @brief If FLEXRAM has ECC function. */
#define FSL_FEATURE_FLEXRAM_HAS_ECC (0)
/* @brief If FLEXRAM has ECC Error Injection function. */
#define FSL_FEATURE_FLEXRAM_HAS_ECC_ERROR_INJECTION (0)

/* FLEXSPI module features */

/* @brief FlexSPI AHB buffer count */
#define FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNTn(x) (4)
/* @brief FlexSPI has no data learn. */
#define FSL_FEATURE_FLEXSPI_HAS_NO_DATA_LEARN (1)
/* @brief There is AHBBUSERROREN bit in INTEN register. */
#define FSL_FEATURE_FLEXSPI_HAS_INTEN_AHBBUSERROREN (0)
/* @brief There is CLRAHBTX_RXBUF bit in AHBCR register. */
#define FSL_FEATURE_FLEXSPI_HAS_AHBCR_CLRAHBTX_RXBUF (1)
/* @brief FLEXSPI has no IP parallel mode. */
#define FSL_FEATURE_FLEXSPI_HAS_NO_IP_PARALLEL_MODE (0)
/* @brief FLEXSPI has no AHB parallel mode. */
#define FSL_FEATURE_FLEXSPI_HAS_NO_AHB_PARALLEL_MODE (0)
/* @brief FLEXSPI support address shift. */
#define FSL_FEATURE_FLEXSPI_SUPPORT_ADDRESS_SHIFT (0)

/* GPC module features */

/* @brief Has DVFS0 Change Request. */
#define FSL_FEATURE_GPC_HAS_CNTR_DVFS0CR (0)
/* @brief Has GPC interrupt/event masking. */
#define FSL_FEATURE_GPC_HAS_CNTR_GPCIRQM (0)
/* @brief Has L2 cache power control. */
#define FSL_FEATURE_GPC_HAS_CNTR_L2PGE (0)
/* @brief Has FLEXRAM PDRAM0(bank1-7) power control. */
#define FSL_FEATURE_GPC_HAS_CNTR_PDRAM0PGE (1)
/* @brief Has VADC power control. */
#define FSL_FEATURE_GPC_HAS_CNTR_VADC (0)
/* @brief Has Display power control. */
#define FSL_FEATURE_GPC_HAS_CNTR_DISPLAY (0)
/* @brief Supports IRQ 0-31. */
#define FSL_FEATURE_GPC_HAS_IRQ_0_31 (1)

/* IGPIO module features */

/* @brief Has data register set DR_SET. */
#define FSL_FEATURE_IGPIO_HAS_DR_SET (1)
/* @brief Has data register clear DR_CLEAR. */
#define FSL_FEATURE_IGPIO_HAS_DR_CLEAR (1)
/* @brief Has data register toggle DR_TOGGLE. */
#define FSL_FEATURE_IGPIO_HAS_DR_TOGGLE (1)

/* LCDIF module features */

/* @brief LCDIF does not support alpha support. */
#define FSL_FEATURE_LCDIF_HAS_NO_AS (1)
/* @brief LCDIF does not support output reset pin to LCD panel. */
#define FSL_FEATURE_LCDIF_HAS_NO_RESET_PIN (1)
/* @brief LCDIF supports LUT. */
#define FSL_FEATURE_LCDIF_HAS_LUT (1)

/* LPI2C module features */

/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPI2C_HAS_SEPARATE_DMA_RX_TX_REQn(x) (0)
/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPI2C_FIFO_SIZEn(x) (4)

/* LPSPI module features */

/* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
#define FSL_FEATURE_LPSPI_FIFO_SIZEn(x) (16)
/* @brief Has separate DMA RX and TX requests. */
#define FSL_FEATURE_LPSPI_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
/* @brief Has CCR1 (related to existence of registers CCR1). */
#define FSL_FEATURE_LPSPI_HAS_CCR1 (0)

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
#define FSL_FEATURE_LPUART_FIFO_SIZEn(x) (4)
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
/* @brief Has register MODEM Control. */
#define FSL_FEATURE_LPUART_HAS_MCR (0)
/* @brief Has register Half Duplex Control. */
#define FSL_FEATURE_LPUART_HAS_HDCR (0)
/* @brief Has register Timeout. */
#define FSL_FEATURE_LPUART_HAS_TIMEOUT (0)

/* interrupt module features */

/* @brief Lowest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MIN (-14)
/* @brief Highest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MAX (157)

/* OCOTP module features */

/* @brief Has timing control, (register TIMING). */
#define FSL_FEATURE_OCOTP_HAS_TIMING_CTRL (1)
/* @brief Support lock eFuse word write lock, (CTRL[WORDLOCK]). */
#define FSL_FEATURE_OCOTP_HAS_WORDLOCK (0)

/* PIT module features */

/* @brief Number of channels (related to number of registers LDVALn, CVALn, TCTRLn, TFLGn). */
#define FSL_FEATURE_PIT_TIMER_COUNT (4)
/* @brief Has lifetime timer (related to existence of registers LTMR64L and LTMR64H). */
#define FSL_FEATURE_PIT_HAS_LIFETIME_TIMER (1)
/* @brief Has chain mode (related to existence of register bit field TCTRLn[CHN]). */
#define FSL_FEATURE_PIT_HAS_CHAIN_MODE (1)
/* @brief Has shared interrupt handler (has not individual interrupt handler for each channel). */
#define FSL_FEATURE_PIT_HAS_SHARED_IRQ_HANDLER (1)
/* @brief Has timer enable control. */
#define FSL_FEATURE_PIT_HAS_MDIS (1)

/* PMU module features */

/* @brief PMU supports lower power control. */
#define FSL_FEATURE_PMU_HAS_LOWPWR_CTRL (0)

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
#define FSL_FEATURE_PWM_SUBMODULE_COUNT (4U)
/* @brief Number of fault channel in each (e)FlexPWM module. */
#define FSL_FEATURE_PWM_FAULT_CH_COUNT (1)
/* @brief (e)FlexPWM has no WAITEN Bitfield In CTRL2 Register. */
#define FSL_FEATURE_PWM_HAS_NO_WAITEN (1)

/* PXP module features */

/* @brief PXP module has dither engine. */
#define FSL_FEATURE_PXP_HAS_DITHER (0)
/* @brief PXP module supports repeat run */
#define FSL_FEATURE_PXP_HAS_EN_REPEAT (1)
/* @brief PXP doesn't have CSC */
#define FSL_FEATURE_PXP_HAS_NO_CSC2 (1)
/* @brief PXP doesn't have LUT */
#define FSL_FEATURE_PXP_HAS_NO_LUT (1)

/* RTWDOG module features */

/* @brief Watchdog is available. */
#define FSL_FEATURE_RTWDOG_HAS_WATCHDOG (1)
/* @brief RTWDOG_CNT can be 32-bit written. */
#define FSL_FEATURE_RTWDOG_HAS_32BIT_ACCESS (1)

/* SAI module features */

/* @brief SAI has FIFO in this soc (register bit fields TCR1[TFW]. */
#define FSL_FEATURE_SAI_HAS_FIFO (1)
/* @brief Receive/transmit FIFO size in item count (register bit fields TCSR[FRDE], TCSR[FRIE], TCSR[FRF], TCR1[TFW], RCSR[FRDE], RCSR[FRIE], RCSR[FRF], RCR1[RFW], registers TFRn, RFRn). */
#define FSL_FEATURE_SAI_FIFO_COUNTn(x) (32)
/* @brief Receive/transmit channel number (register bit fields TCR3[TCE], RCR3[RCE], registers TDRn and RDRn). */
#define FSL_FEATURE_SAI_CHANNEL_COUNTn(x) \
    (((x) == SAI1) ? (4) : \
    (((x) == SAI2) ? (1) : \
    (((x) == SAI3) ? (1) : (-1))))
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
#define FSL_FEATURE_SAI_INT_SOURCE_NUM (2)
/* @brief Has register of MCR. */
#define FSL_FEATURE_SAI_HAS_MCR (0)
/* @brief Has bit field MICS of the MCR register. */
#define FSL_FEATURE_SAI_HAS_NO_MCR_MICS (1)
/* @brief Has register of MDR */
#define FSL_FEATURE_SAI_HAS_MDR (0)
/* @brief Has support the BCLK bypass mode when BCLK = MCLK. */
#define FSL_FEATURE_SAI_HAS_BCLK_BYPASS (0)
/* @brief Has DIV bit fields of MCR register (register bit fields MCR[DIV]. */
#define FSL_FEATURE_SAI_HAS_MCR_MCLK_POST_DIV (0)
/* @brief Support Channel Mode (register bit fields TCR4[CHMOD]). */
#define FSL_FEATURE_SAI_HAS_CHANNEL_MODE (1)

/* SEMC module features */

/* @brief Has WDH time in NOR controller (register bit field NORCR2[WDH]). */
#define FSL_FEATURE_SEMC_HAS_NOR_WDH_TIME (0)
/* @brief Has WDS time in NOR controller (register bit field NORCR2[WDS]). */
#define FSL_FEATURE_SEMC_HAS_NOR_WDS_TIME (0)
/* @brief Has LC time in NOR controller (register bit field NORCR2[LC]). */
#define FSL_FEATURE_SEMC_HAS_NOR_LC_TIME (1)
/* @brief Has RD time in NOR controller (register bit field NORCR2[RD]). */
#define FSL_FEATURE_SEMC_HAS_NOR_RD_TIME (1)
/* @brief Has WDH time in SRAM controller (register bit field SRAMCR2[WDH] or SRAMCR6[WDH]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_WDH_TIME (1)
/* @brief Has WDS time in SRAM controller (register bit field SRAMCR2[WDS] or SRAMCR6[WDS]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_WDS_TIME (1)
/* @brief Has LC time in SRAM controller (register bit field SRAMCR2[LC] or SRAMCR6[LC]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_LC_TIME (1)
/* @brief Has RD time in SRAM controller (register bit field SRAMCR2[RD] or SRAMCR6[RD]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_RD_TIME (1)
/* @brief SRAM count SEMC can support (register BRx). */
#define FSL_FEATURE_SEMC_SUPPORT_SRAM_COUNT (1)
/* @brief If SEMC support delay chain control (register DCCR). */
#define FSL_FEATURE_SEMC_HAS_DELAY_CHAIN_CONTROL (0)
/* @brief Has read hold time feature (register bit field SRAMCR2[RDH] or SRAMCR6[RDH]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_RDH_TIME (1)
/* @brief Has read hold time feature (register bit field SRAMCR0[SYNCEN] or SRAMCR4[SYNCEN]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_SYNCEN (1)
/* @brief Has read hold time feature (register bit field SRAMCR0[WAITEN] or SRAMCR4[WAITEN]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_WAITEN (0)
/* @brief Has read hold time feature (register bit field SRAMCR0[WAITSP] or SRAMCR4[WAITSP]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_WAITSP (0)
/* @brief Has read hold time feature (register bit field SRAMCR0[ADVH] or SRAMCR4[ADVH]). */
#define FSL_FEATURE_SEMC_HAS_SRAM_ADVH (1)
/* @brief Width of SDRAMCR0[PS] bitfields. */
#define FSL_FEATURE_SEMC_SUPPORT_SDRAM_PS_BITWIDTH (1)
/* @brief If SEMC has errata 050577. */
#define FSL_FEATURE_SEMC_ERRATA_050577 (0)
/* @brief If sdram support column address 8 bit (register bit field SRAMCR0[CLO8]). */
#define FSL_FEATURE_SEMC_SDRAM_SUPPORT_COLUMN_ADDRESS_8BIT (1)
/* @brief If SEMC has register DBICR2 (register DBICR2). */
#define FSL_FEATURE_SEMC_HAS_DBICR2 (0)
/* @brief SEMC supports hardware ECC on NAND flash interface. */
#define FSL_FEATURE_SEMC_HAS_NAND_HW_ECC (0)

/* SNVS module features */

/* @brief Has Secure Real Time Counter Enabled and Valid (bit field LPCR[SRTC_ENV]). */
#define FSL_FEATURE_SNVS_HAS_SRTC (1)
/* @brief Has Passive Tamper Filter (regitser LPTGFCR). */
#define FSL_FEATURE_SNVS_PASSIVE_TAMPER_FILTER (0)
/* @brief Has Active Tampers (regitser LPATCTLR, LPATCLKR, LPATRCnR). */
#define FSL_FEATURE_SNVS_HAS_ACTIVE_TAMPERS (0)
/* @brief Number of TAMPER. */
#define FSL_FEATURE_SNVS_HAS_MULTIPLE_TAMPER (0)

/* SRC module features */

/* @brief There is MASK_WDOG3_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_MASK_WDOG3_RST (1)
/* @brief There is MIX_RST_STRCH bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_MIX_RST_STRCH (0)
/* @brief There is DBG_RST_MSK_PG bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_DBG_RST_MSK_PG (1)
/* @brief There is WDOG3_RST_OPTN bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_WDOG3_RST_OPTN (0)
/* @brief There is CORES_DBG_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_CORES_DBG_RST (0)
/* @brief There is MTSR bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_MTSR (0)
/* @brief There is CORE0_DBG_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_CORE0_DBG_RST (1)
/* @brief There is CORE0_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_CORE0_RST (1)
/* @brief There is LOCKUP_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_LOCKUP_RST (0)
/* @brief There is SWRC bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_SWRC (0)
/* @brief There is EIM_RST bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_EIM_RST (0)
/* @brief There is LUEN bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_SCR_LUEN (0)
/* @brief There is no WRBC bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_NO_SCR_WRBC (1)
/* @brief There is no WRE bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_NO_SCR_WRE (1)
/* @brief There is SISR register. */
#define FSL_FEATURE_SRC_HAS_SISR (0)
/* @brief There is RESET_OUT bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_RESET_OUT (0)
/* @brief There is WDOG3_RST_B bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_WDOG3_RST_B (1)
/* @brief There is JTAG_SW_RST bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_JTAG_SW_RST (1)
/* @brief There is SW bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_SW (0)
/* @brief There is IPP_USER_RESET_B bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_IPP_USER_RESET_B (1)
/* @brief There is SNVS bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_SNVS (0)
/* @brief There is CSU_RESET_B bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_CSU_RESET_B (1)
/* @brief There is LOCKUP bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_LOCKUP (0)
/* @brief There is LOCKUP_SYSRESETREQ bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_LOCKUP_SYSRESETREQ (1)
/* @brief There is POR bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_POR (0)
/* @brief There is IPP_RESET_B bit in SRSR register. */
#define FSL_FEATURE_SRC_HAS_SRSR_IPP_RESET_B (1)
/* @brief There is no WBI bit in SCR register. */
#define FSL_FEATURE_SRC_HAS_NO_SRSR_WBI (1)

/* SCB module features */

/* @brief L1 ICACHE line size in byte. */
#define FSL_FEATURE_L1ICACHE_LINESIZE_BYTE (32)
/* @brief L1 DCACHE line size in byte. */
#define FSL_FEATURE_L1DCACHE_LINESIZE_BYTE (32)

/* TRNG module features */

/* @brief TRNG has no TRNG_ACC bitfield. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_ACC (1)

/* USBHS module features */

/* @brief EHCI module instance count */
#define FSL_FEATURE_USBHS_EHCI_COUNT (2)
/* @brief Number of endpoints supported */
#define FSL_FEATURE_USBHS_ENDPT_COUNT (8)

/* USBPHY module features */

/* @brief USBPHY contain DCD analog module */
#define FSL_FEATURE_USBPHY_HAS_DCD_ANALOG (0)
/* @brief USBPHY has register TRIM_OVERRIDE_EN */
#define FSL_FEATURE_USBPHY_HAS_TRIM_OVERRIDE_EN (0)
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
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_8_BIT_WIDTHn(x) \
    (((x) == USDHC1) ? (0) : \
    (((x) == USDHC2) ? (1) : (-1)))
/* @brief If USDHC instance support HS400 mode */
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_HS400_MODEn(x) (0)
/* @brief If USDHC instance support 1v8 signal */
#define FSL_FEATURE_USDHC_INSTANCE_SUPPORT_1V8_SIGNALn(x) (1)
/* @brief Has no retuning time counter (HOST_CTRL_CAP[TIME_COUNT_RETURNING]) */
#define FSL_FEATURE_USDHC_REGISTER_HOST_CTRL_CAP_HAS_NO_RETUNING_TIME_COUNTER (1)
/* @brief Has no VSELECT bit in VEND_SPEC register */
#define FSL_FEATURE_USDHC_HAS_NO_VOLTAGE_SELECT (0)

/* XBARA module features */

/* @brief Number of interrupt requests. */
#define FSL_FEATURE_XBARA_INTERRUPT_COUNT (4)

#endif /* _MIMXRT1062_FEATURES_H_ */

