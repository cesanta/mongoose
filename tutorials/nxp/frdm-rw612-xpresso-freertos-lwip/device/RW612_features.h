/*
** ###################################################################
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b241030
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
**     - rev. 1.0 (2021-03-16)
**         Initial version.
**
** ###################################################################
*/

#ifndef _RW612_FEATURES_H_
#define _RW612_FEATURES_H_

/* SOC module features */

/* @brief ACOMP availability on the SoC. */
#define FSL_FEATURE_SOC_ACOMP_COUNT (1)
/* @brief ADC availability on the SoC. */
#define FSL_FEATURE_SOC_ADC_COUNT (2)
/* @brief AON_SOC_CIU availability on the SoC. */
#define FSL_FEATURE_SOC_AON_SOC_CIU_COUNT (1)
/* @brief APU availability on the SoC. */
#define FSL_FEATURE_SOC_APU_COUNT (2)
/* @brief BG availability on the SoC. */
#define FSL_FEATURE_SOC_BG_COUNT (1)
/* @brief BLEAPU availability on the SoC. */
#define FSL_FEATURE_SOC_BLEAPU_COUNT (1)
/* @brief BUCK11 availability on the SoC. */
#define FSL_FEATURE_SOC_BUCK11_COUNT (1)
/* @brief BUCK18 availability on the SoC. */
#define FSL_FEATURE_SOC_BUCK18_COUNT (1)
/* @brief CACHE64_CTRL availability on the SoC. */
#define FSL_FEATURE_SOC_CACHE64_CTRL_COUNT (2)
/* @brief CACHE64_POLSEL availability on the SoC. */
#define FSL_FEATURE_SOC_CACHE64_POLSEL_COUNT (2)
/* @brief CAU availability on the SoC. */
#define FSL_FEATURE_SOC_CAU_COUNT (1)
/* @brief CDOG availability on the SoC. */
#define FSL_FEATURE_SOC_CDOG_COUNT (1)
/* @brief CIU1 availability on the SoC. */
#define FSL_FEATURE_SOC_CIU1_COUNT (1)
/* @brief CIU2 availability on the SoC. */
#define FSL_FEATURE_SOC_CIU2_COUNT (1)
/* @brief CLKCTL0 availability on the SoC. */
#define FSL_FEATURE_SOC_CLKCTL0_COUNT (1)
/* @brief CLKCTL1 availability on the SoC. */
#define FSL_FEATURE_SOC_CLKCTL1_COUNT (1)
/* @brief CRC availability on the SoC. */
#define FSL_FEATURE_SOC_CRC_COUNT (1)
/* @brief CTIMER availability on the SoC. */
#define FSL_FEATURE_SOC_CTIMER_COUNT (4)
/* @brief DAC availability on the SoC. */
#define FSL_FEATURE_SOC_DAC_COUNT (1)
/* @brief DMA availability on the SoC. */
#define FSL_FEATURE_SOC_DMA_COUNT (2)
/* @brief DMIC availability on the SoC. */
#define FSL_FEATURE_SOC_DMIC_COUNT (1)
/* @brief ELS availability on the SoC. */
#define FSL_FEATURE_SOC_ELS_COUNT (1)
/* @brief ENET availability on the SoC. */
#define FSL_FEATURE_SOC_ENET_COUNT (1)
/* @brief FLEXCOMM availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCOMM_COUNT (5)
/* @brief FLEXSPI availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXSPI_COUNT (1)
/* @brief FREQME availability on the SoC. */
#define FSL_FEATURE_SOC_FREQME_COUNT (1)
/* @brief GDMA availability on the SoC. */
#define FSL_FEATURE_SOC_GDMA_COUNT (1)
/* @brief GPIO availability on the SoC. */
#define FSL_FEATURE_SOC_GPIO_COUNT (2)
/* @brief I2C availability on the SoC. */
#define FSL_FEATURE_SOC_I2C_COUNT (5)
/* @brief I2S availability on the SoC. */
#define FSL_FEATURE_SOC_I2S_COUNT (5)
/* @brief INPUTMUX availability on the SoC. */
#define FSL_FEATURE_SOC_INPUTMUX_COUNT (1)
/* @brief ITRC availability on the SoC. */
#define FSL_FEATURE_SOC_ITRC_COUNT (1)
/* @brief LCDIC availability on the SoC. */
#define FSL_FEATURE_SOC_LCDIC_COUNT (1)
/* @brief MCI_IO_MUX availability on the SoC. */
#define FSL_FEATURE_SOC_MCI_IO_MUX_COUNT (1)
/* @brief MRT availability on the SoC. */
#define FSL_FEATURE_SOC_MRT_COUNT (2)
/* @brief OCOTP availability on the SoC. */
#define FSL_FEATURE_SOC_OCOTP_COUNT (1)
/* @brief OSTIMER availability on the SoC. */
#define FSL_FEATURE_SOC_OSTIMER_COUNT (1)
/* @brief PINT availability on the SoC. */
#define FSL_FEATURE_SOC_PINT_COUNT (1)
/* @brief PKC availability on the SoC. */
#define FSL_FEATURE_SOC_PKC_COUNT (1)
/* @brief POWERQUAD availability on the SoC. */
#define FSL_FEATURE_SOC_POWERQUAD_COUNT (1)
/* @brief PUF availability on the SoC. */
#define FSL_FEATURE_SOC_PUF_COUNT (1)
/* @brief RF_SYSCON availability on the SoC. */
#define FSL_FEATURE_SOC_RF_SYSCON_COUNT (1)
/* @brief ROMC availability on the SoC. */
#define FSL_FEATURE_SOC_ROMC_COUNT (1)
/* @brief RSTCTL0 availability on the SoC. */
#define FSL_FEATURE_SOC_RSTCTL0_COUNT (1)
/* @brief RSTCTL1 availability on the SoC. */
#define FSL_FEATURE_SOC_RSTCTL1_COUNT (1)
/* @brief RTC availability on the SoC. */
#define FSL_FEATURE_SOC_RTC_COUNT (1)
/* @brief SCT availability on the SoC. */
#define FSL_FEATURE_SOC_SCT_COUNT (1)
/* @brief SPI availability on the SoC. */
#define FSL_FEATURE_SOC_SPI_COUNT (5)
/* @brief SYSCTL0 availability on the SoC. */
#define FSL_FEATURE_SOC_SYSCTL0_COUNT (1)
/* @brief SYSCTL1 availability on the SoC. */
#define FSL_FEATURE_SOC_SYSCTL1_COUNT (1)
/* @brief SYSCTL2 availability on the SoC. */
#define FSL_FEATURE_SOC_SYSCTL2_COUNT (1)
/* @brief SYSPLL_T3 availability on the SoC. */
#define FSL_FEATURE_SOC_SYSPLL_T3_COUNT (1)
/* @brief SYSPLL_TCPU availability on the SoC. */
#define FSL_FEATURE_SOC_SYSPLL_TCPU_COUNT (1)
/* @brief SOC_OTP_CTRL availability on the SoC. */
#define FSL_FEATURE_SOC_SOC_OTP_CTRL_COUNT (1)
/* @brief SENSOR_CTRL availability on the SoC. */
#define FSL_FEATURE_SOC_SENSOR_CTRL_COUNT (1)
/* @brief SDU_FN0_CARD availability on the SoC. */
#define FSL_FEATURE_SOC_SDU_FN0_CARD_COUNT (1)
/* @brief SDU_FBR_CARD availability on the SoC. */
#define FSL_FEATURE_SOC_SDU_FBR_CARD_COUNT (1)
/* @brief SDU_FN_CARD availability on the SoC. */
#define FSL_FEATURE_SOC_SDU_FN_CARD_COUNT (1)
/* @brief TRNG availability on the SoC. */
#define FSL_FEATURE_SOC_TRNG_COUNT (1)
/* @brief USART availability on the SoC. */
#define FSL_FEATURE_SOC_USART_COUNT (5)
/* @brief USB availability on the SoC. */
#define FSL_FEATURE_SOC_USB_COUNT (1)
/* @brief USIM availability on the SoC. */
#define FSL_FEATURE_SOC_USIM_COUNT (1)
/* @brief UTICK availability on the SoC. */
#define FSL_FEATURE_SOC_UTICK_COUNT (1)
/* @brief WLAPU availability on the SoC. */
#define FSL_FEATURE_SOC_WLAPU_COUNT (1)
/* @brief WWDT availability on the SoC. */
#define FSL_FEATURE_SOC_WWDT_COUNT (1)

/* CACHE64_CTRL module features */

/* @brief Cache Line size in byte. */
#define FSL_FEATURE_CACHE64_CTRL_LINESIZE_BYTE (32)

/* CACHE64_POLSEL module features */

/* No feature definitions */

/* CDOG module features */

/* @brief CDOG Has No Reset */
#define FSL_FEATURE_CDOG_HAS_NO_RESET (1)
/* @brief CDOG Load default configurations during init function */
#define FSL_FEATURE_CDOG_NEED_LOAD_DEFAULT_CONF (1)

/* CRC module features */

/* @brief Has data register with name CRC */
#define FSL_FEATURE_CRC_HAS_CRC_REG (0)

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

/* DMA module features */

/* @brief Number of channels */
#define FSL_FEATURE_DMA_NUMBER_OF_CHANNELSn(x) (33)
/* @brief Number of all DMA channels */
#define FSL_FEATURE_DMA_ALL_CHANNELS (66)
/* @brief Max Number of DMA channels */
#define FSL_FEATURE_DMA_MAX_CHANNELS (33)
/* @brief Align size of DMA descriptor */
#define FSL_FEATURE_DMA_DESCRIPTOR_ALIGN_SIZE (1024)
/* @brief DMA head link descriptor table align size */
#define FSL_FEATURE_DMA_LINK_DESCRIPTOR_ALIGN_SIZE (16U)

/* DMIC module features */

/* @brief Number of channels */
#define FSL_FEATURE_DMIC_CHANNEL_NUM (4)
/* @brief DMIC channel support stereo data */
#define FSL_FEATURE_DMIC_IO_HAS_STEREO_2_4_6 (1)
/* @brief DMIC does not support bypass channel clock */
#define FSL_FEATURE_DMIC_IO_HAS_NO_BYPASS (1)
/* @brief DMIC channel FIFO register support sign extended */
#define FSL_FEATURE_DMIC_CHANNEL_HAS_SIGNEXTEND (1)
/* @brief DMIC has no IOCFG register */
#define FSL_FEATURE_DMIC_HAS_NO_IOCFG (1)
/* @brief DMIC has decimator reset function */
#define FSL_FEATURE_DMIC_HAS_DECIMATOR_RESET_FUNC (1)
/* @brief DMIC has global channel synchronization function */
#define FSL_FEATURE_DMIC_HAS_GLOBAL_SYNC_FUNC (0)

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
#define FSL_FEATURE_ENET_INSTANCE_HAS_INTERRUPT_COALESCEn(x) (0)
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
/* @brief ENET Has Extra Clock Gate.(RW610). */
#define FSL_FEATURE_ENET_HAS_EXTRA_CLOCK_GATE (1)
/* @brief ENET support reset. */
#define FSL_FEATURE_ENET_HAS_RSTCTL (1)

/* FLEXCOMM module features */

/* @brief FLEXCOMM0 USART INDEX 0 */
#define FSL_FEATURE_FLEXCOMM0_USART_INDEX (0)
/* @brief FLEXCOMM0 SPI INDEX 0 */
#define FSL_FEATURE_FLEXCOMM0_SPI_INDEX (0)
/* @brief FLEXCOMM0 I2C INDEX 0 */
#define FSL_FEATURE_FLEXCOMM0_I2C_INDEX (0)
/* @brief FLEXCOMM0 I2S INDEX 0 */
#define FSL_FEATURE_FLEXCOMM0_I2S_INDEX (0)
/* @brief FLEXCOMM1 USART INDEX 1 */
#define FSL_FEATURE_FLEXCOMM1_USART_INDEX (1)
/* @brief FLEXCOMM1 SPI INDEX 1 */
#define FSL_FEATURE_FLEXCOMM1_SPI_INDEX (1)
/* @brief FLEXCOMM1 I2C INDEX 1 */
#define FSL_FEATURE_FLEXCOMM1_I2C_INDEX (1)
/* @brief FLEXCOMM1 I2S INDEX 1 */
#define FSL_FEATURE_FLEXCOMM1_I2S_INDEX (1)
/* @brief FLEXCOMM2 USART INDEX 2 */
#define FSL_FEATURE_FLEXCOMM2_USART_INDEX (2)
/* @brief FLEXCOMM2 SPI INDEX 2 */
#define FSL_FEATURE_FLEXCOMM2_SPI_INDEX (2)
/* @brief FLEXCOMM2 I2C INDEX 2 */
#define FSL_FEATURE_FLEXCOMM2_I2C_INDEX (2)
/* @brief FLEXCOMM2 I2S INDEX 2 */
#define FSL_FEATURE_FLEXCOMM2_I2S_INDEX (2)
/* @brief FLEXCOMM3 USART INDEX 3 */
#define FSL_FEATURE_FLEXCOMM3_USART_INDEX (3)
/* @brief FLEXCOMM3 SPI INDEX 3 */
#define FSL_FEATURE_FLEXCOMM3_SPI_INDEX (3)
/* @brief FLEXCOMM3 I2C INDEX 3 */
#define FSL_FEATURE_FLEXCOMM3_I2C_INDEX (3)
/* @brief FLEXCOMM3 I2S INDEX 3 */
#define FSL_FEATURE_FLEXCOMM3_I2S_INDEX (3)
/* @brief FLEXCOMM14 USART INDEX 14 */
#define FSL_FEATURE_FLEXCOMM14_USART_INDEX (14)
/* @brief FLEXCOMM14 SPI(HS_SPI) INDEX 14 */
#define FSL_FEATURE_FLEXCOMM14_SPI_INDEX (14)
/* @brief FLEXCOMM14 I2C INDEX 14 */
#define FSL_FEATURE_FLEXCOMM14_I2C_INDEX (14)
/* @brief FLEXCOMM14 I2S INDEX 14 */
#define FSL_FEATURE_FLEXCOMM14_I2S_INDEX (14)
/* @brief I2S has DMIC interconnection */
#define FSL_FEATURE_FLEXCOMM_INSTANCE_I2S_HAS_DMIC_INTERCONNECTIONn(x) \
    (((x) == FLEXCOMM0) ? (1) : \
    (((x) == FLEXCOMM1) ? (0) : \
    (((x) == FLEXCOMM2) ? (0) : \
    (((x) == FLEXCOMM3) ? (0) : \
    (((x) == FLEXCOMM14) ? (0) : (-1))))))

/* FLEXSPI module features */

/* @brief FlexSPI AHB buffer count */
#define FSL_FEATURE_FLEXSPI_AHB_BUFFER_COUNTn(x) (8)
/* @brief FlexSPI has no MCR0 ARDFEN bit */
#define FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ARDFEN (0)
/* @brief FlexSPI has no MCR0 ATDFEN bit */
#define FSL_FEATURE_FLEXSPI_HAS_NO_MCR0_ATDFEN (0)
/* @brief FlexSPI has no IP parallel mode */
#define FSL_FEATURE_FLEXSPI_HAS_NO_IP_PARALLEL_MODE (1)
/* @brief FlexSPI has no AHB parallel mode */
#define FSL_FEATURE_FLEXSPI_HAS_NO_AHB_PARALLEL_MODE (1)
/* @brief FlexSPI support address shift */
#define FSL_FEATURE_FLEXSPI_SUPPORT_ADDRESS_SHIFT (1)
/* @brief FlexSPI support sample clock source selection */
#define FSL_FEATURE_FLEXSPI_SUPPORT_SEPERATE_RXCLKSRC_PORTB (1)
/* @brief FlexSPI support sample clock source or source_b selection */
#define FSL_FEATURE_FLEXSPI_SUPPORT_RXCLKSRC_DIFF (1)
/* @brief FlexSPI AHB RX buffer size (byte) */
#define FSL_FEATURE_FLEXSPI_AHB_RX_BUFFER_SIZEn(x) (2048)
/* @brief FlexSPI IPED REGION COUNT */
#define FSL_FEATURE_FLEXSPI_IPED_REGION_COUNT (15)
/* @brief FlexSPI Array Length */
#define FSL_FEATURE_FLEXSPI_ARRAY_LEN (1)

/* ADC module features */

/* @brief Whether ADC has the single-end mode temp channel */
#define FSL_FEATURE_ADC_HAS_NO_SINGLEEND_TEMP_CHANNEL (1)
/* @brief Whether ADC has the differential mode voice channel */
#define FSL_FEATURE_ADC_HAS_NO_DIFFERENTIAL_VOICE_CHANNEL (1)
/* @brief Whether ADC has the differential mode temp channel */
#define FSL_FEATURE_ADC_HAS_NO_DIFFERENTIAL_TEMP_CHANNEL (1)

/* GDMA module features */

/* @brief GDMA Channel Number */
#define FSL_FEATURE_GDMA_CHANNEL_NUM (4)

/* GPIO module features */

/* @brief GPIO has interrupts */
#define FSL_FEATURE_GPIO_HAS_INTERRUPT (1)

/* I2S module features */

/* @brief I2S support dual channel transfer. */
#define FSL_FEATURE_I2S_SUPPORT_SECONDARY_CHANNEL (1)
/* @brief I2S has DMIC interconnection. */
#define FSL_FEATURE_FLEXCOMM_I2S_HAS_DMIC_INTERCONNECTION (1)

/* INPUTMUX module features */

/* @brief Inputmux has DMA Request Enable */
#define FSL_FEATURE_INPUTMUX_HAS_SIGNAL_ENA (1)
/* @brief Inputmux has channel mux control */
#define FSL_FEATURE_INPUTMUX_HAS_CHANNEL_MUX (0)

/* MEMORY module features */

/* @brief Memory map has offset between subsystems. */
#define FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET (1)

/* MRT module features */

/* @brief number of channels. */
#define FSL_FEATURE_MRT_NUMBER_OF_CHANNELS (4)

/* OSTIMER module features */

/* @brief Has no OS Timer control register in PMC */
#define FSL_FEATURE_PMC_HAS_NO_OSTIMER_REG (1)

/* PINT module features */

/* @brief Number of connected outputs */
#define FSL_FEATURE_PINT_NUMBER_OF_CONNECTED_OUTPUTS (8)

/* POWERLIB module features */

/* @brief Powerlib API is different with other LPC series devices. */
#define FSL_FEATURE_POWERLIB_EXTEND (1)

/* RTC module features */

/* @brief RTC has no reset control */
#define FSL_FEATURE_RTC_HAS_NO_RESET (1)
/* @brief Has SUBSEC Register (register SUBSEC) */
#define FSL_FEATURE_RTC_HAS_SUBSEC (1)

/* SCT module features */

/* @brief Number of events */
#define FSL_FEATURE_SCT_NUMBER_OF_EVENTS (16)
/* @brief Number of states */
#define FSL_FEATURE_SCT_NUMBER_OF_STATES (32)
/* @brief Number of match capture */
#define FSL_FEATURE_SCT_NUMBER_OF_MATCH_CAPTURE (16)
/* @brief Number of outputs */
#define FSL_FEATURE_SCT_NUMBER_OF_OUTPUTS (10)

/* SPI module features */

/* @brief SSEL pin count. */
#define FSL_FEATURE_SPI_SSEL_COUNT (4)

/* TRNG module features */

/* @brief TRNG does not support SCR4L. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_SCR4L (1)
/* @brief TRNG does not support SCR5L. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_SCR5L (1)
/* @brief TRNG does not support SCR6L. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_SCR6L (1)
/* @brief TRNG does not support PKRMAX. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_PKRMAX (1)
/* @brief TRNG does not support SAMP mode. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_MCTL_SAMP_MODE (1)
/* @brief TRNG does not support ACC. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_ACC (1)
/* @brief TRNG does not support SBLIM. */
#define FSL_FEATURE_TRNG_HAS_NO_TRNG_SBLIM (1)
/* @brief TRNG supports reset control. */
#define FSL_FEATURE_TRNG_HAS_RSTCTL (1)
/* @brief TRNG supports dual oscillator mode. */
#define FSL_FEATURE_TRNG_HAS_DUAL_OSCILATORS (1)
/* @brief TRNG supports control pin. */
#define FSL_FEATURE_TRNG_HAS_CTRL_PIN (1)

/* USART module features */

/* @brief Has FIFO Receive Timeout Configuration (register FIFORXTIMEOUTCFG). */
#define FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG (1)

/* USB module features */

/* @brief USBC Atlantic Controller support on the SoC. */
#define FSL_FEATURE_USB_ATLANTIC_EHCI_SUPPORT (1)

/* USIM module features */

/* @brief USIM Tx/Rx FIFO size in byte. */
#define FSL_FEATURE_USIM_FIFO_DEPTH (16)

/* UTICK module features */

/* @brief UTICK does not support power down configure. */
#define FSL_FEATURE_UTICK_HAS_NO_PDCFG (1)

/* WWDT module features */

/* @brief WWDT does not support oscillator lock. */
#define FSL_FEATURE_WWDT_HAS_NO_OSCILLATOR_LOCK (0)
/* @brief WWDT does not support power down configure. */
#define FSL_FEATURE_WWDT_HAS_NO_PDCFG (1)

#endif /* _RW612_FEATURES_H_ */

