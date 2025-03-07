/*
** ###################################################################
**     Processors:          RW610ETA2I
**                          RW610HNA2I
**                          RW610UKA2I
**                          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b241008
**
**     Abstract:
**         CMSIS Peripheral Access Layer for USART
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
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

/*!
 * @file USART.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for USART
 *
 * CMSIS Peripheral Access Layer for USART
 */

#if !defined(USART_H_)
#define USART_H_                                 /**< Symbol preventing repeated inclusion */

#if (defined(CPU_RW610ETA2I) || defined(CPU_RW610HNA2I) || defined(CPU_RW610UKA2I))
#include "RW610_COMMON.h"
#elif (defined(CPU_RW612ETA2I) || defined(CPU_RW612HNA2I) || defined(CPU_RW612UKA2I))
#include "RW612_COMMON.h"
#else
  #error "No valid CPU defined!"
#endif

/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
  #else
    #pragma push
    #pragma anon_unions
  #endif
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- USART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USART_Peripheral_Access_Layer USART Peripheral Access Layer
 * @{
 */

/** USART - Register Layout Typedef */
typedef struct {
  __IO uint32_t CFG;                               /**< USART Configuration, offset: 0x0 */
  __IO uint32_t CTL;                               /**< USART Control, offset: 0x4 */
  __IO uint32_t STAT;                              /**< USART Status, offset: 0x8 */
  __IO uint32_t INTENSET;                          /**< Interrupt Enable Read and Set for USART (not FIFO) Status, offset: 0xC */
  __O  uint32_t INTENCLR;                          /**< Interrupt Enable Clear, offset: 0x10 */
       uint8_t RESERVED_0[12];
  __IO uint32_t BRG;                               /**< Baud Rate Generator, offset: 0x20 */
  __I  uint32_t INTSTAT;                           /**< Interrupt Status, offset: 0x24 */
  __IO uint32_t OSR;                               /**< Oversample Selection Register for Asynchronous Communication, offset: 0x28 */
  __IO uint32_t ADDR;                              /**< Address Register for Automatic Address Matching, offset: 0x2C */
       uint8_t RESERVED_1[3536];
  __IO uint32_t FIFOCFG;                           /**< FIFO Configuration, offset: 0xE00 */
  __IO uint32_t FIFOSTAT;                          /**< FIFO Status, offset: 0xE04 */
  __IO uint32_t FIFOTRIG;                          /**< FIFO Trigger Settings for Interrupt and DMA Request, offset: 0xE08 */
       uint8_t RESERVED_2[4];
  __IO uint32_t FIFOINTENSET;                      /**< FIFO Interrupt Enable, offset: 0xE10 */
  __IO uint32_t FIFOINTENCLR;                      /**< FIFO Interrupt Enable Clear, offset: 0xE14 */
  __I  uint32_t FIFOINTSTAT;                       /**< FIFO Interrupt Status, offset: 0xE18 */
       uint8_t RESERVED_3[4];
  __O  uint32_t FIFOWR;                            /**< FIFO Write Data, offset: 0xE20 */
       uint8_t RESERVED_4[12];
  __I  uint32_t FIFORD;                            /**< FIFO Read Data, offset: 0xE30 */
       uint8_t RESERVED_5[12];
  __I  uint32_t FIFORDNOPOP;                       /**< FIFO Data Read with No FIFO Pop, offset: 0xE40 */
       uint8_t RESERVED_6[4];
  __I  uint32_t FIFOSIZE;                          /**< FIFO Size, offset: 0xE48 */
  __IO uint32_t FIFORXTIMEOUTCFG;                  /**< FIFO Receive Timeout Configuration, offset: 0xE4C */
  __I  uint32_t FIFORXTIMEOUTCNT;                  /**< FIFO Receive Timeout Counter, offset: 0xE50 */
       uint8_t RESERVED_7[424];
  __I  uint32_t ID;                                /**< Peripheral Identification, offset: 0xFFC */
} USART_Type;

/* ----------------------------------------------------------------------------
   -- USART Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USART_Register_Masks USART Register Masks
 * @{
 */

/*! @name CFG - USART Configuration */
/*! @{ */

#define USART_CFG_ENABLE_MASK                    (0x1U)
#define USART_CFG_ENABLE_SHIFT                   (0U)
/*! ENABLE - USART Enable
 *  0b0..Disabled
 *  0b1..Enabled. The USART is enabled for operation.
 */
#define USART_CFG_ENABLE(x)                      (((uint32_t)(((uint32_t)(x)) << USART_CFG_ENABLE_SHIFT)) & USART_CFG_ENABLE_MASK)

#define USART_CFG_DATALEN_MASK                   (0xCU)
#define USART_CFG_DATALEN_SHIFT                  (2U)
/*! DATALEN - Data Length. Selects the data size for the USART.
 *  0b00..7 bit data length
 *  0b01..8 bit data length
 *  0b10..9 bit data length. The 9th bit is commonly used for addressing in multidrop mode. See the ADDRDET[CTL].
 *  0b11..Reserved
 */
#define USART_CFG_DATALEN(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CFG_DATALEN_SHIFT)) & USART_CFG_DATALEN_MASK)

#define USART_CFG_PARITYSEL_MASK                 (0x30U)
#define USART_CFG_PARITYSEL_SHIFT                (4U)
/*! PARITYSEL - Parity Select. Selects what type of parity is used by the USART.
 *  0b00..No parity
 *  0b01..Reserved
 *  0b10..Even parity
 *  0b11..Odd parity
 */
#define USART_CFG_PARITYSEL(x)                   (((uint32_t)(((uint32_t)(x)) << USART_CFG_PARITYSEL_SHIFT)) & USART_CFG_PARITYSEL_MASK)

#define USART_CFG_STOPLEN_MASK                   (0x40U)
#define USART_CFG_STOPLEN_SHIFT                  (6U)
/*! STOPLEN - Stop Length
 *  0b0..1 stop bit
 *  0b1..2 stop bits. This setting should be used only for asynchronous communication.
 */
#define USART_CFG_STOPLEN(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CFG_STOPLEN_SHIFT)) & USART_CFG_STOPLEN_MASK)

#define USART_CFG_MODE32K_MASK                   (0x80U)
#define USART_CFG_MODE32K_SHIFT                  (7U)
/*! MODE32K - Mode 32 kHz
 *  0b0..Disabled. USART uses standard clocking.
 *  0b1..Enabled
 */
#define USART_CFG_MODE32K(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CFG_MODE32K_SHIFT)) & USART_CFG_MODE32K_MASK)

#define USART_CFG_LINMODE_MASK                   (0x100U)
#define USART_CFG_LINMODE_SHIFT                  (8U)
/*! LINMODE - LIN Break Mode Enable
 *  0b0..Disabled. Break detect and generate is configured for normal operation.
 *  0b1..Enabled. Break detect and generate is configured for LIN bus operation.
 */
#define USART_CFG_LINMODE(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CFG_LINMODE_SHIFT)) & USART_CFG_LINMODE_MASK)

#define USART_CFG_CTSEN_MASK                     (0x200U)
#define USART_CFG_CTSEN_SHIFT                    (9U)
/*! CTSEN - CTS Enable
 *  0b0..No flow control. The transmitter does not receive any automatic flow control signal.
 *  0b1..Flow control enabled. The transmitter uses the CTS input (or RTS output in loopback mode) for flow control purposes.
 */
#define USART_CFG_CTSEN(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CFG_CTSEN_SHIFT)) & USART_CFG_CTSEN_MASK)

#define USART_CFG_SYNCEN_MASK                    (0x800U)
#define USART_CFG_SYNCEN_SHIFT                   (11U)
/*! SYNCEN - Synchronous Enable. Selects synchronous or asynchronous operation.
 *  0b0..Asynchronous mode
 *  0b1..Synchronous mode
 */
#define USART_CFG_SYNCEN(x)                      (((uint32_t)(((uint32_t)(x)) << USART_CFG_SYNCEN_SHIFT)) & USART_CFG_SYNCEN_MASK)

#define USART_CFG_CLKPOL_MASK                    (0x1000U)
#define USART_CFG_CLKPOL_SHIFT                   (12U)
/*! CLKPOL - Clock Polarity
 *  0b0..Falling edge. RXD is sampled on the falling edge of SCLK.
 *  0b1..Rising edge. RXD is sampled on the rising edge of SCLK.
 */
#define USART_CFG_CLKPOL(x)                      (((uint32_t)(((uint32_t)(x)) << USART_CFG_CLKPOL_SHIFT)) & USART_CFG_CLKPOL_MASK)

#define USART_CFG_SYNCMST_MASK                   (0x4000U)
#define USART_CFG_SYNCMST_SHIFT                  (14U)
/*! SYNCMST - Synchronous mode Master Select
 *  0b0..Slave. When synchronous mode is enabled, the USART is a slave.
 *  0b1..Master. When synchronous mode is enabled, the USART is a master.
 */
#define USART_CFG_SYNCMST(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CFG_SYNCMST_SHIFT)) & USART_CFG_SYNCMST_MASK)

#define USART_CFG_LOOP_MASK                      (0x8000U)
#define USART_CFG_LOOP_SHIFT                     (15U)
/*! LOOP - Loopback Mode
 *  0b0..Normal operation
 *  0b1..Loopback mode
 */
#define USART_CFG_LOOP(x)                        (((uint32_t)(((uint32_t)(x)) << USART_CFG_LOOP_SHIFT)) & USART_CFG_LOOP_MASK)

#define USART_CFG_OETA_MASK                      (0x40000U)
#define USART_CFG_OETA_SHIFT                     (18U)
/*! OETA - Output Enable Turnaround Time Enable for RS-485 Operation.
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define USART_CFG_OETA(x)                        (((uint32_t)(((uint32_t)(x)) << USART_CFG_OETA_SHIFT)) & USART_CFG_OETA_MASK)

#define USART_CFG_AUTOADDR_MASK                  (0x80000U)
#define USART_CFG_AUTOADDR_SHIFT                 (19U)
/*! AUTOADDR - Automatic Address Matching Enable
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define USART_CFG_AUTOADDR(x)                    (((uint32_t)(((uint32_t)(x)) << USART_CFG_AUTOADDR_SHIFT)) & USART_CFG_AUTOADDR_MASK)

#define USART_CFG_OESEL_MASK                     (0x100000U)
#define USART_CFG_OESEL_SHIFT                    (20U)
/*! OESEL - Output Enable Select
 *  0b0..Standard. The RTS signal is used as the standard flow control function.
 *  0b1..RS-485. The RTS signal is configured to provide an output enable signal to control an RS-485 transceiver.
 */
#define USART_CFG_OESEL(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CFG_OESEL_SHIFT)) & USART_CFG_OESEL_MASK)

#define USART_CFG_OEPOL_MASK                     (0x200000U)
#define USART_CFG_OEPOL_SHIFT                    (21U)
/*! OEPOL - Output Enable Polarity
 *  0b0..Low. If selected by OESEL, the output enable is active low.
 *  0b1..High. If selected by OESEL, the output enable is active high.
 */
#define USART_CFG_OEPOL(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CFG_OEPOL_SHIFT)) & USART_CFG_OEPOL_MASK)

#define USART_CFG_RXPOL_MASK                     (0x400000U)
#define USART_CFG_RXPOL_SHIFT                    (22U)
/*! RXPOL - Receive Data Polarity
 *  0b0..Standard
 *  0b1..Inverted
 */
#define USART_CFG_RXPOL(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CFG_RXPOL_SHIFT)) & USART_CFG_RXPOL_MASK)

#define USART_CFG_TXPOL_MASK                     (0x800000U)
#define USART_CFG_TXPOL_SHIFT                    (23U)
/*! TXPOL - Transmit data polarity
 *  0b0..Standard
 *  0b1..Inverted
 */
#define USART_CFG_TXPOL(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CFG_TXPOL_SHIFT)) & USART_CFG_TXPOL_MASK)
/*! @} */

/*! @name CTL - USART Control */
/*! @{ */

#define USART_CTL_TXBRKEN_MASK                   (0x2U)
#define USART_CTL_TXBRKEN_SHIFT                  (1U)
/*! TXBRKEN - Break Enable
 *  0b0..Normal operation
 *  0b1..Continuous break
 */
#define USART_CTL_TXBRKEN(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CTL_TXBRKEN_SHIFT)) & USART_CTL_TXBRKEN_MASK)

#define USART_CTL_ADDRDET_MASK                   (0x4U)
#define USART_CTL_ADDRDET_SHIFT                  (2U)
/*! ADDRDET - Enable Address Detect Mode
 *  0b0..Disabled. The USART presents all incoming data.
 *  0b1..Enabled
 */
#define USART_CTL_ADDRDET(x)                     (((uint32_t)(((uint32_t)(x)) << USART_CTL_ADDRDET_SHIFT)) & USART_CTL_ADDRDET_MASK)

#define USART_CTL_TXDIS_MASK                     (0x40U)
#define USART_CTL_TXDIS_SHIFT                    (6U)
/*! TXDIS - Transmit Disable
 *  0b0..Not disabled. USART transmitter is not disabled.
 *  0b1..Disabled. USART transmitter is disabled after any character currently being transmitted is complete. This
 *       feature can be used to facilitate software flow control.
 */
#define USART_CTL_TXDIS(x)                       (((uint32_t)(((uint32_t)(x)) << USART_CTL_TXDIS_SHIFT)) & USART_CTL_TXDIS_MASK)

#define USART_CTL_CC_MASK                        (0x100U)
#define USART_CTL_CC_SHIFT                       (8U)
/*! CC - Continuous Clock Generation
 *  0b0..Clock on character
 *  0b1..Continuous clock
 */
#define USART_CTL_CC(x)                          (((uint32_t)(((uint32_t)(x)) << USART_CTL_CC_SHIFT)) & USART_CTL_CC_MASK)

#define USART_CTL_CLRCCONRX_MASK                 (0x200U)
#define USART_CTL_CLRCCONRX_SHIFT                (9U)
/*! CLRCCONRX - Clear Continuous Clock
 *  0b0..No effect. No effect on the CC bit.
 *  0b1..Auto-clear
 */
#define USART_CTL_CLRCCONRX(x)                   (((uint32_t)(((uint32_t)(x)) << USART_CTL_CLRCCONRX_SHIFT)) & USART_CTL_CLRCCONRX_MASK)

#define USART_CTL_AUTOBAUD_MASK                  (0x10000U)
#define USART_CTL_AUTOBAUD_SHIFT                 (16U)
/*! AUTOBAUD - Autobaud Enable
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define USART_CTL_AUTOBAUD(x)                    (((uint32_t)(((uint32_t)(x)) << USART_CTL_AUTOBAUD_SHIFT)) & USART_CTL_AUTOBAUD_MASK)
/*! @} */

/*! @name STAT - USART Status */
/*! @{ */

#define USART_STAT_RXIDLE_MASK                   (0x2U)
#define USART_STAT_RXIDLE_SHIFT                  (1U)
/*! RXIDLE - Receiver Idle
 *  0b0..The receiver is currently receiving data.
 *  0b1..The receiver is not currently receiving data.
 */
#define USART_STAT_RXIDLE(x)                     (((uint32_t)(((uint32_t)(x)) << USART_STAT_RXIDLE_SHIFT)) & USART_STAT_RXIDLE_MASK)

#define USART_STAT_TXIDLE_MASK                   (0x8U)
#define USART_STAT_TXIDLE_SHIFT                  (3U)
/*! TXIDLE - Transmitter Idle
 *  0b0..The transmitter is currently sending data.
 *  0b1..The transmitter is not currently sending data.
 */
#define USART_STAT_TXIDLE(x)                     (((uint32_t)(((uint32_t)(x)) << USART_STAT_TXIDLE_SHIFT)) & USART_STAT_TXIDLE_MASK)

#define USART_STAT_CTS_MASK                      (0x10U)
#define USART_STAT_CTS_SHIFT                     (4U)
/*! CTS - CTS value */
#define USART_STAT_CTS(x)                        (((uint32_t)(((uint32_t)(x)) << USART_STAT_CTS_SHIFT)) & USART_STAT_CTS_MASK)

#define USART_STAT_DELTACTS_MASK                 (0x20U)
#define USART_STAT_DELTACTS_SHIFT                (5U)
/*! DELTACTS - Delta CTS */
#define USART_STAT_DELTACTS(x)                   (((uint32_t)(((uint32_t)(x)) << USART_STAT_DELTACTS_SHIFT)) & USART_STAT_DELTACTS_MASK)

#define USART_STAT_TXDISSTAT_MASK                (0x40U)
#define USART_STAT_TXDISSTAT_SHIFT               (6U)
/*! TXDISSTAT - Transmitter Disabled Status Flag
 *  0b0..Not Idle. Indicates that the USART transmitter is NOT fully idle after being disabled.
 *  0b1..Idle. Indicates that the USART transmitter is fully idle after being disabled (CTL[TXDIS] = 1).
 */
#define USART_STAT_TXDISSTAT(x)                  (((uint32_t)(((uint32_t)(x)) << USART_STAT_TXDISSTAT_SHIFT)) & USART_STAT_TXDISSTAT_MASK)

#define USART_STAT_RXBRK_MASK                    (0x400U)
#define USART_STAT_RXBRK_SHIFT                   (10U)
/*! RXBRK - Received Break */
#define USART_STAT_RXBRK(x)                      (((uint32_t)(((uint32_t)(x)) << USART_STAT_RXBRK_SHIFT)) & USART_STAT_RXBRK_MASK)

#define USART_STAT_DELTARXBRK_MASK               (0x800U)
#define USART_STAT_DELTARXBRK_SHIFT              (11U)
/*! DELTARXBRK - Delta Received Break */
#define USART_STAT_DELTARXBRK(x)                 (((uint32_t)(((uint32_t)(x)) << USART_STAT_DELTARXBRK_SHIFT)) & USART_STAT_DELTARXBRK_MASK)

#define USART_STAT_START_MASK                    (0x1000U)
#define USART_STAT_START_SHIFT                   (12U)
/*! START - Start */
#define USART_STAT_START(x)                      (((uint32_t)(((uint32_t)(x)) << USART_STAT_START_SHIFT)) & USART_STAT_START_MASK)

#define USART_STAT_FRAMERRINT_MASK               (0x2000U)
#define USART_STAT_FRAMERRINT_SHIFT              (13U)
/*! FRAMERRINT - Framing Error Interrupt Flag */
#define USART_STAT_FRAMERRINT(x)                 (((uint32_t)(((uint32_t)(x)) << USART_STAT_FRAMERRINT_SHIFT)) & USART_STAT_FRAMERRINT_MASK)

#define USART_STAT_PARITYERRINT_MASK             (0x4000U)
#define USART_STAT_PARITYERRINT_SHIFT            (14U)
/*! PARITYERRINT - Parity Error Interrupt Flag */
#define USART_STAT_PARITYERRINT(x)               (((uint32_t)(((uint32_t)(x)) << USART_STAT_PARITYERRINT_SHIFT)) & USART_STAT_PARITYERRINT_MASK)

#define USART_STAT_RXNOISEINT_MASK               (0x8000U)
#define USART_STAT_RXNOISEINT_SHIFT              (15U)
/*! RXNOISEINT - Received Noise Interrupt Flag */
#define USART_STAT_RXNOISEINT(x)                 (((uint32_t)(((uint32_t)(x)) << USART_STAT_RXNOISEINT_SHIFT)) & USART_STAT_RXNOISEINT_MASK)

#define USART_STAT_ABERR_MASK                    (0x10000U)
#define USART_STAT_ABERR_SHIFT                   (16U)
/*! ABERR - Auto Baud Error */
#define USART_STAT_ABERR(x)                      (((uint32_t)(((uint32_t)(x)) << USART_STAT_ABERR_SHIFT)) & USART_STAT_ABERR_MASK)
/*! @} */

/*! @name INTENSET - Interrupt Enable Read and Set for USART (not FIFO) Status */
/*! @{ */

#define USART_INTENSET_TXIDLEEN_MASK             (0x8U)
#define USART_INTENSET_TXIDLEEN_SHIFT            (3U)
/*! TXIDLEEN - Transmit Idle Flag
 *  0b1..Enables an interrupt when the transmitter becomes idle (STAT[TXIDLE] = 1).
 */
#define USART_INTENSET_TXIDLEEN(x)               (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_TXIDLEEN_SHIFT)) & USART_INTENSET_TXIDLEEN_MASK)

#define USART_INTENSET_DELTACTSEN_MASK           (0x20U)
#define USART_INTENSET_DELTACTSEN_SHIFT          (5U)
/*! DELTACTSEN - Delta CTS Input Flag
 *  0b1..Enables an interrupt when there is a change in the state of the CTS input.
 */
#define USART_INTENSET_DELTACTSEN(x)             (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_DELTACTSEN_SHIFT)) & USART_INTENSET_DELTACTSEN_MASK)

#define USART_INTENSET_TXDISEN_MASK              (0x40U)
#define USART_INTENSET_TXDISEN_SHIFT             (6U)
/*! TXDISEN - Transmit Disabled Flag
 *  0b1..Enables an interrupt when the transmitter is fully disabled as indicated by the STAT[TXDISINT] flag. See
 *       the description of the STAT[TXDISINT] flag.
 */
#define USART_INTENSET_TXDISEN(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_TXDISEN_SHIFT)) & USART_INTENSET_TXDISEN_MASK)

#define USART_INTENSET_DELTARXBRKEN_MASK         (0x800U)
#define USART_INTENSET_DELTARXBRKEN_SHIFT        (11U)
/*! DELTARXBRKEN - Delta Receive Break Enable
 *  0b1..Enable
 */
#define USART_INTENSET_DELTARXBRKEN(x)           (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_DELTARXBRKEN_SHIFT)) & USART_INTENSET_DELTARXBRKEN_MASK)

#define USART_INTENSET_STARTEN_MASK              (0x1000U)
#define USART_INTENSET_STARTEN_SHIFT             (12U)
/*! STARTEN - Start Enable
 *  0b1..Enables an interrupt when a received start bit has been detected.
 */
#define USART_INTENSET_STARTEN(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_STARTEN_SHIFT)) & USART_INTENSET_STARTEN_MASK)

#define USART_INTENSET_FRAMERREN_MASK            (0x2000U)
#define USART_INTENSET_FRAMERREN_SHIFT           (13U)
/*! FRAMERREN - Frame Error Enable
 *  0b1..Enables an interrupt when a framing error has been detected.
 */
#define USART_INTENSET_FRAMERREN(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_FRAMERREN_SHIFT)) & USART_INTENSET_FRAMERREN_MASK)

#define USART_INTENSET_PARITYERREN_MASK          (0x4000U)
#define USART_INTENSET_PARITYERREN_SHIFT         (14U)
/*! PARITYERREN - Parity Error Enble
 *  0b1..Enables an interrupt when a parity error has been detected.
 */
#define USART_INTENSET_PARITYERREN(x)            (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_PARITYERREN_SHIFT)) & USART_INTENSET_PARITYERREN_MASK)

#define USART_INTENSET_RXNOISEEN_MASK            (0x8000U)
#define USART_INTENSET_RXNOISEEN_SHIFT           (15U)
/*! RXNOISEEN - Receive Noise Enable
 *  0b1..Enables an interrupt when noise is detected. See the description of the CTL[RXNOISEINT] bit.
 */
#define USART_INTENSET_RXNOISEEN(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_RXNOISEEN_SHIFT)) & USART_INTENSET_RXNOISEEN_MASK)

#define USART_INTENSET_ABERREN_MASK              (0x10000U)
#define USART_INTENSET_ABERREN_SHIFT             (16U)
/*! ABERREN - Auto Baud Error Enable
 *  0b1..Enables an interrupt when an auto baud error occurs.
 */
#define USART_INTENSET_ABERREN(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTENSET_ABERREN_SHIFT)) & USART_INTENSET_ABERREN_MASK)
/*! @} */

/*! @name INTENCLR - Interrupt Enable Clear */
/*! @{ */

#define USART_INTENCLR_TXIDLECLR_MASK            (0x8U)
#define USART_INTENCLR_TXIDLECLR_SHIFT           (3U)
/*! TXIDLECLR - Transmit Idle Clear */
#define USART_INTENCLR_TXIDLECLR(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_TXIDLECLR_SHIFT)) & USART_INTENCLR_TXIDLECLR_MASK)

#define USART_INTENCLR_DELTACTSCLR_MASK          (0x20U)
#define USART_INTENCLR_DELTACTSCLR_SHIFT         (5U)
/*! DELTACTSCLR - Delta CTS Clear */
#define USART_INTENCLR_DELTACTSCLR(x)            (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_DELTACTSCLR_SHIFT)) & USART_INTENCLR_DELTACTSCLR_MASK)

#define USART_INTENCLR_TXDISCLR_MASK             (0x40U)
#define USART_INTENCLR_TXDISCLR_SHIFT            (6U)
/*! TXDISCLR - Transmit Disable Clear */
#define USART_INTENCLR_TXDISCLR(x)               (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_TXDISCLR_SHIFT)) & USART_INTENCLR_TXDISCLR_MASK)

#define USART_INTENCLR_DELTARXBRKCLR_MASK        (0x800U)
#define USART_INTENCLR_DELTARXBRKCLR_SHIFT       (11U)
/*! DELTARXBRKCLR - Delta Receive Break Clear */
#define USART_INTENCLR_DELTARXBRKCLR(x)          (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_DELTARXBRKCLR_SHIFT)) & USART_INTENCLR_DELTARXBRKCLR_MASK)

#define USART_INTENCLR_STARTCLR_MASK             (0x1000U)
#define USART_INTENCLR_STARTCLR_SHIFT            (12U)
/*! STARTCLR - Start Clear */
#define USART_INTENCLR_STARTCLR(x)               (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_STARTCLR_SHIFT)) & USART_INTENCLR_STARTCLR_MASK)

#define USART_INTENCLR_FRAMERRCLR_MASK           (0x2000U)
#define USART_INTENCLR_FRAMERRCLR_SHIFT          (13U)
/*! FRAMERRCLR - Frame Error Clear */
#define USART_INTENCLR_FRAMERRCLR(x)             (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_FRAMERRCLR_SHIFT)) & USART_INTENCLR_FRAMERRCLR_MASK)

#define USART_INTENCLR_PARITYERRCLR_MASK         (0x4000U)
#define USART_INTENCLR_PARITYERRCLR_SHIFT        (14U)
/*! PARITYERRCLR - Parity Error Clear */
#define USART_INTENCLR_PARITYERRCLR(x)           (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_PARITYERRCLR_SHIFT)) & USART_INTENCLR_PARITYERRCLR_MASK)

#define USART_INTENCLR_RXNOISECLR_MASK           (0x8000U)
#define USART_INTENCLR_RXNOISECLR_SHIFT          (15U)
/*! RXNOISECLR - Receive Noise Clear */
#define USART_INTENCLR_RXNOISECLR(x)             (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_RXNOISECLR_SHIFT)) & USART_INTENCLR_RXNOISECLR_MASK)

#define USART_INTENCLR_ABERRCLR_MASK             (0x10000U)
#define USART_INTENCLR_ABERRCLR_SHIFT            (16U)
/*! ABERRCLR - Auto Baud Error Clear */
#define USART_INTENCLR_ABERRCLR(x)               (((uint32_t)(((uint32_t)(x)) << USART_INTENCLR_ABERRCLR_SHIFT)) & USART_INTENCLR_ABERRCLR_MASK)
/*! @} */

/*! @name BRG - Baud Rate Generator */
/*! @{ */

#define USART_BRG_BRGVAL_MASK                    (0xFFFFU)
#define USART_BRG_BRGVAL_SHIFT                   (0U)
/*! BRGVAL - Baud Rate Generator Value
 *  0b0000000000000000..FCLK is used directly by the USART function.
 *  0b0000000000000001..FCLK is divided by 2 before use by the USART function.
 *  0b0000000000000010..FCLK is divided by 3 before use by the USART function.
 *  0b1111111111111111..FCLK is divided by 65,536 before use by the USART function.
 */
#define USART_BRG_BRGVAL(x)                      (((uint32_t)(((uint32_t)(x)) << USART_BRG_BRGVAL_SHIFT)) & USART_BRG_BRGVAL_MASK)
/*! @} */

/*! @name INTSTAT - Interrupt Status */
/*! @{ */

#define USART_INTSTAT_TXIDLE_MASK                (0x8U)
#define USART_INTSTAT_TXIDLE_SHIFT               (3U)
/*! TXIDLE - Transmitter Idle Flag */
#define USART_INTSTAT_TXIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_TXIDLE_SHIFT)) & USART_INTSTAT_TXIDLE_MASK)

#define USART_INTSTAT_DELTACTS_MASK              (0x20U)
#define USART_INTSTAT_DELTACTS_SHIFT             (5U)
/*! DELTACTS - Delta CTS Change Flag */
#define USART_INTSTAT_DELTACTS(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_DELTACTS_SHIFT)) & USART_INTSTAT_DELTACTS_MASK)

#define USART_INTSTAT_TXDISINT_MASK              (0x40U)
#define USART_INTSTAT_TXDISINT_SHIFT             (6U)
/*! TXDISINT - Transmitter Disabled Interrupt Flag */
#define USART_INTSTAT_TXDISINT(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_TXDISINT_SHIFT)) & USART_INTSTAT_TXDISINT_MASK)

#define USART_INTSTAT_DELTARXBRK_MASK            (0x800U)
#define USART_INTSTAT_DELTARXBRK_SHIFT           (11U)
/*! DELTARXBRK - Delta Receiver Break Change Flag */
#define USART_INTSTAT_DELTARXBRK(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_DELTARXBRK_SHIFT)) & USART_INTSTAT_DELTARXBRK_MASK)

#define USART_INTSTAT_START_MASK                 (0x1000U)
#define USART_INTSTAT_START_SHIFT                (12U)
/*! START - Start Detected on Receiver Flag */
#define USART_INTSTAT_START(x)                   (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_START_SHIFT)) & USART_INTSTAT_START_MASK)

#define USART_INTSTAT_FRAMERRINT_MASK            (0x2000U)
#define USART_INTSTAT_FRAMERRINT_SHIFT           (13U)
/*! FRAMERRINT - Framing Error Interrupt Flag */
#define USART_INTSTAT_FRAMERRINT(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_FRAMERRINT_SHIFT)) & USART_INTSTAT_FRAMERRINT_MASK)

#define USART_INTSTAT_PARITYERRINT_MASK          (0x4000U)
#define USART_INTSTAT_PARITYERRINT_SHIFT         (14U)
/*! PARITYERRINT - Parity Error Interrupt Flag */
#define USART_INTSTAT_PARITYERRINT(x)            (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_PARITYERRINT_SHIFT)) & USART_INTSTAT_PARITYERRINT_MASK)

#define USART_INTSTAT_RXNOISEINT_MASK            (0x8000U)
#define USART_INTSTAT_RXNOISEINT_SHIFT           (15U)
/*! RXNOISEINT - Received Noise Interrupt Flag */
#define USART_INTSTAT_RXNOISEINT(x)              (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_RXNOISEINT_SHIFT)) & USART_INTSTAT_RXNOISEINT_MASK)

#define USART_INTSTAT_ABERRINT_MASK              (0x10000U)
#define USART_INTSTAT_ABERRINT_SHIFT             (16U)
/*! ABERRINT - Auto Baud Error Interrupt Flag */
#define USART_INTSTAT_ABERRINT(x)                (((uint32_t)(((uint32_t)(x)) << USART_INTSTAT_ABERRINT_SHIFT)) & USART_INTSTAT_ABERRINT_MASK)
/*! @} */

/*! @name OSR - Oversample Selection Register for Asynchronous Communication */
/*! @{ */

#define USART_OSR_OSRVAL_MASK                    (0xFU)
#define USART_OSR_OSRVAL_SHIFT                   (0U)
/*! OSRVAL - Oversample Selection Value
 *  0b0000..Not supported
 *  0b0001..Not supported
 *  0b0010..Not supported
 *  0b0011..Not supported
 *  0b0100..5 function clocks are used to transmit and receive each data bit.
 *  0b0101..6 function clocks are used to transmit and receive each data bit.
 *  0b1111..16 function clocks are used to transmit and receive each data bit.
 */
#define USART_OSR_OSRVAL(x)                      (((uint32_t)(((uint32_t)(x)) << USART_OSR_OSRVAL_SHIFT)) & USART_OSR_OSRVAL_MASK)
/*! @} */

/*! @name ADDR - Address Register for Automatic Address Matching */
/*! @{ */

#define USART_ADDR_ADDRESS_MASK                  (0xFFU)
#define USART_ADDR_ADDRESS_SHIFT                 (0U)
/*! ADDRESS - Address */
#define USART_ADDR_ADDRESS(x)                    (((uint32_t)(((uint32_t)(x)) << USART_ADDR_ADDRESS_SHIFT)) & USART_ADDR_ADDRESS_MASK)
/*! @} */

/*! @name FIFOCFG - FIFO Configuration */
/*! @{ */

#define USART_FIFOCFG_ENABLETX_MASK              (0x1U)
#define USART_FIFOCFG_ENABLETX_SHIFT             (0U)
/*! ENABLETX - Enable the Transmit FIFO.
 *  0b0..The transmit FIFO is not enabled.
 *  0b1..The transmit FIFO is enabled.
 */
#define USART_FIFOCFG_ENABLETX(x)                (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_ENABLETX_SHIFT)) & USART_FIFOCFG_ENABLETX_MASK)

#define USART_FIFOCFG_ENABLERX_MASK              (0x2U)
#define USART_FIFOCFG_ENABLERX_SHIFT             (1U)
/*! ENABLERX - Enable the Receive FIFO
 *  0b0..The receive FIFO is not enabled.
 *  0b1..The receive FIFO is enabled.
 */
#define USART_FIFOCFG_ENABLERX(x)                (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_ENABLERX_SHIFT)) & USART_FIFOCFG_ENABLERX_MASK)

#define USART_FIFOCFG_SIZE_MASK                  (0x30U)
#define USART_FIFOCFG_SIZE_SHIFT                 (4U)
/*! SIZE - FIFO Size Configuration
 *  0b00..FIFO is configured as 16 entries of 8 bits.
 *  0b01..Not used
 *  0b10..Not used
 *  0b11..Not used
 */
#define USART_FIFOCFG_SIZE(x)                    (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_SIZE_SHIFT)) & USART_FIFOCFG_SIZE_MASK)

#define USART_FIFOCFG_DMATX_MASK                 (0x1000U)
#define USART_FIFOCFG_DMATX_SHIFT                (12U)
/*! DMATX - DMA Configuration for Transmit
 *  0b0..DMA is not used for the transmit function.
 *  0b1..Triggers DMA for the transmit function if the FIFO is not full. Generally, data interrupts would be disabled if DMA is enabled.
 */
#define USART_FIFOCFG_DMATX(x)                   (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_DMATX_SHIFT)) & USART_FIFOCFG_DMATX_MASK)

#define USART_FIFOCFG_DMARX_MASK                 (0x2000U)
#define USART_FIFOCFG_DMARX_SHIFT                (13U)
/*! DMARX - DMA Configuration for Receive
 *  0b0..DMA is not used for the receive function.
 *  0b1..Triggers DMA for the receive function if the FIFO is not empty. Generally, data interrupts would be disabled if DMA is enabled.
 */
#define USART_FIFOCFG_DMARX(x)                   (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_DMARX_SHIFT)) & USART_FIFOCFG_DMARX_MASK)

#define USART_FIFOCFG_WAKETX_MASK                (0x4000U)
#define USART_FIFOCFG_WAKETX_SHIFT               (14U)
/*! WAKETX - Wake-up for Transmit FIFO Level
 *  0b0..Only enabled interrupts will wake up the device from low power modes.
 *  0b1..A device wake-up for DMA will occur if the transmit FIFO level reaches the value specified by
 *       FIFOTRIG[TXLVL], even when the TXLVL interrupt is not enabled.
 */
#define USART_FIFOCFG_WAKETX(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_WAKETX_SHIFT)) & USART_FIFOCFG_WAKETX_MASK)

#define USART_FIFOCFG_WAKERX_MASK                (0x8000U)
#define USART_FIFOCFG_WAKERX_SHIFT               (15U)
/*! WAKERX - Wake-up for Receive FIFO Level
 *  0b0..Only enabled interrupts will wake up the device from low power modes.
 *  0b1..A device wake-up for DMA will occur if the receive FIFO level reaches the value specified by
 *       FIFOTRIG[RXLVL], even when the RXLVL interrupt is not enabled.
 */
#define USART_FIFOCFG_WAKERX(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_WAKERX_SHIFT)) & USART_FIFOCFG_WAKERX_MASK)

#define USART_FIFOCFG_EMPTYTX_MASK               (0x10000U)
#define USART_FIFOCFG_EMPTYTX_SHIFT              (16U)
/*! EMPTYTX - Empty Command for the Transmit FIFO
 *  0b0..No effect
 *  0b1..The TX FIFO is emptied.
 */
#define USART_FIFOCFG_EMPTYTX(x)                 (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_EMPTYTX_SHIFT)) & USART_FIFOCFG_EMPTYTX_MASK)

#define USART_FIFOCFG_EMPTYRX_MASK               (0x20000U)
#define USART_FIFOCFG_EMPTYRX_SHIFT              (17U)
/*! EMPTYRX - Empty Command for the Receive FIFO
 *  0b0..No effect
 *  0b1..The RX FIFO is emptied.
 */
#define USART_FIFOCFG_EMPTYRX(x)                 (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_EMPTYRX_SHIFT)) & USART_FIFOCFG_EMPTYRX_MASK)

#define USART_FIFOCFG_POPDBG_MASK                (0x40000U)
#define USART_FIFOCFG_POPDBG_SHIFT               (18U)
/*! POPDBG - Pop FIFO for Debug Reads
 *  0b0..Debug reads of the FIFO do not pop the FIFO.
 *  0b1..A debug read will cause the FIFO to pop.
 */
#define USART_FIFOCFG_POPDBG(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOCFG_POPDBG_SHIFT)) & USART_FIFOCFG_POPDBG_MASK)
/*! @} */

/*! @name FIFOSTAT - FIFO Status */
/*! @{ */

#define USART_FIFOSTAT_TXERR_MASK                (0x1U)
#define USART_FIFOSTAT_TXERR_SHIFT               (0U)
/*! TXERR - TX FIFO Error
 *  0b0..A transmit FIFO error has not occurred.
 *  0b1..A transmit FIFO error has occurred. This error could be an overflow caused by pushing data into a full
 *       FIFO, or by an underflow if the FIFO is empty when data is needed.
 */
#define USART_FIFOSTAT_TXERR(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_TXERR_SHIFT)) & USART_FIFOSTAT_TXERR_MASK)

#define USART_FIFOSTAT_RXERR_MASK                (0x2U)
#define USART_FIFOSTAT_RXERR_SHIFT               (1U)
/*! RXERR - RX FIFO Error
 *  0b0..A receive FIFO overflow has not occurred
 *  0b1..A receive FIFO overflow has occurred, caused by software or DMA not emptying the FIFO fast enough
 */
#define USART_FIFOSTAT_RXERR(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_RXERR_SHIFT)) & USART_FIFOSTAT_RXERR_MASK)

#define USART_FIFOSTAT_PERINT_MASK               (0x8U)
#define USART_FIFOSTAT_PERINT_SHIFT              (3U)
/*! PERINT - Peripheral Interrupt
 *  0b0..No Peripheral Interrupt
 *  0b1..Peripheral Interrupt
 */
#define USART_FIFOSTAT_PERINT(x)                 (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_PERINT_SHIFT)) & USART_FIFOSTAT_PERINT_MASK)

#define USART_FIFOSTAT_TXEMPTY_MASK              (0x10U)
#define USART_FIFOSTAT_TXEMPTY_SHIFT             (4U)
/*! TXEMPTY - Transmit FIFO Empty
 *  0b0..The transmit FIFO is not empty.
 *  0b1..The transmit FIFO is empty, although the peripheral may still be processing the last piece of data.
 */
#define USART_FIFOSTAT_TXEMPTY(x)                (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_TXEMPTY_SHIFT)) & USART_FIFOSTAT_TXEMPTY_MASK)

#define USART_FIFOSTAT_TXNOTFULL_MASK            (0x20U)
#define USART_FIFOSTAT_TXNOTFULL_SHIFT           (5U)
/*! TXNOTFULL - Transmit FIFO is Not Full
 *  0b0..The transmit FIFO is full and another write would cause it to overflow.
 *  0b1..The transmit FIFO is not full, so more data can be written.
 */
#define USART_FIFOSTAT_TXNOTFULL(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_TXNOTFULL_SHIFT)) & USART_FIFOSTAT_TXNOTFULL_MASK)

#define USART_FIFOSTAT_RXNOTEMPTY_MASK           (0x40U)
#define USART_FIFOSTAT_RXNOTEMPTY_SHIFT          (6U)
/*! RXNOTEMPTY - Receive FIFO is Not Empty
 *  0b0..The receive FIFO is empty.
 *  0b1..The receive FIFO is not empty, so data can be read.
 */
#define USART_FIFOSTAT_RXNOTEMPTY(x)             (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_RXNOTEMPTY_SHIFT)) & USART_FIFOSTAT_RXNOTEMPTY_MASK)

#define USART_FIFOSTAT_RXFULL_MASK               (0x80U)
#define USART_FIFOSTAT_RXFULL_SHIFT              (7U)
/*! RXFULL - Receive FIFO is Full
 *  0b0..The receive FIFO is not full.
 *  0b1..The receive FIFO is full.
 */
#define USART_FIFOSTAT_RXFULL(x)                 (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_RXFULL_SHIFT)) & USART_FIFOSTAT_RXFULL_MASK)

#define USART_FIFOSTAT_TXLVL_MASK                (0x1F00U)
#define USART_FIFOSTAT_TXLVL_SHIFT               (8U)
/*! TXLVL - Transmit FIFO Current Level */
#define USART_FIFOSTAT_TXLVL(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_TXLVL_SHIFT)) & USART_FIFOSTAT_TXLVL_MASK)

#define USART_FIFOSTAT_RXLVL_MASK                (0x1F0000U)
#define USART_FIFOSTAT_RXLVL_SHIFT               (16U)
/*! RXLVL - Receive FIFO Current Level */
#define USART_FIFOSTAT_RXLVL(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_RXLVL_SHIFT)) & USART_FIFOSTAT_RXLVL_MASK)

#define USART_FIFOSTAT_RXTIMEOUT_MASK            (0x1000000U)
#define USART_FIFOSTAT_RXTIMEOUT_SHIFT           (24U)
/*! RXTIMEOUT - Receive FIFO Timeout
 *  0b0..RX FIFO on
 *  0b1..RX FIFO has timed out, based on the timeout configuration in the FIFORXTIMEOUTCFG register.
 */
#define USART_FIFOSTAT_RXTIMEOUT(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOSTAT_RXTIMEOUT_SHIFT)) & USART_FIFOSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOTRIG - FIFO Trigger Settings for Interrupt and DMA Request */
/*! @{ */

#define USART_FIFOTRIG_TXLVLENA_MASK             (0x1U)
#define USART_FIFOTRIG_TXLVLENA_SHIFT            (0U)
/*! TXLVLENA - Transmit FIFO Level Trigger Enable.
 *  0b0..Transmit FIFO level does not generate a FIFO level trigger.
 *  0b1..A trigger will be generated if the transmit FIFO level reaches the value specified by the TXLVL field.
 */
#define USART_FIFOTRIG_TXLVLENA(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOTRIG_TXLVLENA_SHIFT)) & USART_FIFOTRIG_TXLVLENA_MASK)

#define USART_FIFOTRIG_RXLVLENA_MASK             (0x2U)
#define USART_FIFOTRIG_RXLVLENA_SHIFT            (1U)
/*! RXLVLENA - Receive FIFO Level Trigger Enable
 *  0b0..Receive FIFO level does not generate a FIFO level trigger.
 *  0b1..An trigger will be generated if the receive FIFO level reaches the value specified by the RXLVL field.
 */
#define USART_FIFOTRIG_RXLVLENA(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOTRIG_RXLVLENA_SHIFT)) & USART_FIFOTRIG_RXLVLENA_MASK)

#define USART_FIFOTRIG_TXLVL_MASK                (0xF00U)
#define USART_FIFOTRIG_TXLVL_SHIFT               (8U)
/*! TXLVL - Transmit FIFO Level Trigger Point
 *  0b0000..Trigger when the TX FIFO becomes empty
 *  0b0001..Trigger when the TX FIFO level decreases to 1 entry
 *  0b1111..Trigger when the TX FIFO level decreases to 15 entries (is no longer full)
 */
#define USART_FIFOTRIG_TXLVL(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOTRIG_TXLVL_SHIFT)) & USART_FIFOTRIG_TXLVL_MASK)

#define USART_FIFOTRIG_RXLVL_MASK                (0xF0000U)
#define USART_FIFOTRIG_RXLVL_SHIFT               (16U)
/*! RXLVL - Receive FIFO Level Trigger Point
 *  0b0000..Trigger when the RX FIFO has received 1 entry (is no longer empty)
 *  0b0001..Trigger when the RX FIFO has received 2 entries
 *  0b1111..Trigger when the RX FIFO has received 16 entries (has become full)
 */
#define USART_FIFOTRIG_RXLVL(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFOTRIG_RXLVL_SHIFT)) & USART_FIFOTRIG_RXLVL_MASK)
/*! @} */

/*! @name FIFOINTENSET - FIFO Interrupt Enable */
/*! @{ */

#define USART_FIFOINTENSET_TXERR_MASK            (0x1U)
#define USART_FIFOINTENSET_TXERR_SHIFT           (0U)
/*! TXERR - Transmit Error Interrupt Enable
 *  0b0..No interrupt will be generated for a transmit error.
 *  0b1..An interrupt will be generated when a transmit error occurs.
 */
#define USART_FIFOINTENSET_TXERR(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENSET_TXERR_SHIFT)) & USART_FIFOINTENSET_TXERR_MASK)

#define USART_FIFOINTENSET_RXERR_MASK            (0x2U)
#define USART_FIFOINTENSET_RXERR_SHIFT           (1U)
/*! RXERR - Receive Error Interrupt Enable
 *  0b0..No interrupt will be generated for a receive error.
 *  0b1..An interrupt will be generated when a receive error occurs.
 */
#define USART_FIFOINTENSET_RXERR(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENSET_RXERR_SHIFT)) & USART_FIFOINTENSET_RXERR_MASK)

#define USART_FIFOINTENSET_TXLVL_MASK            (0x4U)
#define USART_FIFOINTENSET_TXLVL_SHIFT           (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Enable
 *  0b0..No interrupt will be generated based on the TX FIFO level.
 *  0b1..If FIFOTRIG[TXLVLENA] = 1, then an interrupt will be generated when the TX FIFO level decreases to the level specified by FIFOTRIG[TXLVL]
 */
#define USART_FIFOINTENSET_TXLVL(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENSET_TXLVL_SHIFT)) & USART_FIFOINTENSET_TXLVL_MASK)

#define USART_FIFOINTENSET_RXLVL_MASK            (0x8U)
#define USART_FIFOINTENSET_RXLVL_SHIFT           (3U)
/*! RXLVL - Receive FIFO Level Interrupt Enable
 *  0b0..No interrupt will be generated based on the RX FIFO level.
 *  0b1..If FIFOTRIG[RXLVLENA] = 1, an interrupt will be generated when the when the RX FIFO level increases to
 *       the level specified by FIFOTRIG[RXLVL].
 */
#define USART_FIFOINTENSET_RXLVL(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENSET_RXLVL_SHIFT)) & USART_FIFOINTENSET_RXLVL_MASK)

#define USART_FIFOINTENSET_RXTIMEOUT_MASK        (0x1000000U)
#define USART_FIFOINTENSET_RXTIMEOUT_SHIFT       (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No RX interrupt will be generated.
 *  0b1..Asserts RX interrupt if RX FIFO Timeout event occurs.
 */
#define USART_FIFOINTENSET_RXTIMEOUT(x)          (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENSET_RXTIMEOUT_SHIFT)) & USART_FIFOINTENSET_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTENCLR - FIFO Interrupt Enable Clear */
/*! @{ */

#define USART_FIFOINTENCLR_TXERR_MASK            (0x1U)
#define USART_FIFOINTENCLR_TXERR_SHIFT           (0U)
/*! TXERR - Transmit Error Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define USART_FIFOINTENCLR_TXERR(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENCLR_TXERR_SHIFT)) & USART_FIFOINTENCLR_TXERR_MASK)

#define USART_FIFOINTENCLR_RXERR_MASK            (0x2U)
#define USART_FIFOINTENCLR_RXERR_SHIFT           (1U)
/*! RXERR - Receive Error Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define USART_FIFOINTENCLR_RXERR(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENCLR_RXERR_SHIFT)) & USART_FIFOINTENCLR_RXERR_MASK)

#define USART_FIFOINTENCLR_TXLVL_MASK            (0x4U)
#define USART_FIFOINTENCLR_TXLVL_SHIFT           (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define USART_FIFOINTENCLR_TXLVL(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENCLR_TXLVL_SHIFT)) & USART_FIFOINTENCLR_TXLVL_MASK)

#define USART_FIFOINTENCLR_RXLVL_MASK            (0x8U)
#define USART_FIFOINTENCLR_RXLVL_SHIFT           (3U)
/*! RXLVL - Receive FIFO Level Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define USART_FIFOINTENCLR_RXLVL(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENCLR_RXLVL_SHIFT)) & USART_FIFOINTENCLR_RXLVL_MASK)

#define USART_FIFOINTENCLR_RXTIMEOUT_MASK        (0x1000000U)
#define USART_FIFOINTENCLR_RXTIMEOUT_SHIFT       (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define USART_FIFOINTENCLR_RXTIMEOUT(x)          (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTENCLR_RXTIMEOUT_SHIFT)) & USART_FIFOINTENCLR_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTSTAT - FIFO Interrupt Status */
/*! @{ */

#define USART_FIFOINTSTAT_TXERR_MASK             (0x1U)
#define USART_FIFOINTSTAT_TXERR_SHIFT            (0U)
/*! TXERR - TX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_TXERR(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_TXERR_SHIFT)) & USART_FIFOINTSTAT_TXERR_MASK)

#define USART_FIFOINTSTAT_RXERR_MASK             (0x2U)
#define USART_FIFOINTSTAT_RXERR_SHIFT            (1U)
/*! RXERR - RX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_RXERR(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_RXERR_SHIFT)) & USART_FIFOINTSTAT_RXERR_MASK)

#define USART_FIFOINTSTAT_TXLVL_MASK             (0x4U)
#define USART_FIFOINTSTAT_TXLVL_SHIFT            (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_TXLVL(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_TXLVL_SHIFT)) & USART_FIFOINTSTAT_TXLVL_MASK)

#define USART_FIFOINTSTAT_RXLVL_MASK             (0x8U)
#define USART_FIFOINTSTAT_RXLVL_SHIFT            (3U)
/*! RXLVL - Receive FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_RXLVL(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_RXLVL_SHIFT)) & USART_FIFOINTSTAT_RXLVL_MASK)

#define USART_FIFOINTSTAT_PERINT_MASK            (0x10U)
#define USART_FIFOINTSTAT_PERINT_SHIFT           (4U)
/*! PERINT - Peripheral Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_PERINT(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_PERINT_SHIFT)) & USART_FIFOINTSTAT_PERINT_MASK)

#define USART_FIFOINTSTAT_RXTIMEOUT_MASK         (0x1000000U)
#define USART_FIFOINTSTAT_RXTIMEOUT_SHIFT        (24U)
/*! RXTIMEOUT - Receive Timeout Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define USART_FIFOINTSTAT_RXTIMEOUT(x)           (((uint32_t)(((uint32_t)(x)) << USART_FIFOINTSTAT_RXTIMEOUT_SHIFT)) & USART_FIFOINTSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOWR - FIFO Write Data */
/*! @{ */

#define USART_FIFOWR_TXDATA_MASK                 (0x1FFU)
#define USART_FIFOWR_TXDATA_SHIFT                (0U)
/*! TXDATA - Transmit data to the FIFO */
#define USART_FIFOWR_TXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << USART_FIFOWR_TXDATA_SHIFT)) & USART_FIFOWR_TXDATA_MASK)
/*! @} */

/*! @name FIFORD - FIFO Read Data */
/*! @{ */

#define USART_FIFORD_RXDATA_MASK                 (0x1FFU)
#define USART_FIFORD_RXDATA_SHIFT                (0U)
/*! RXDATA - Received Data from the FIFO */
#define USART_FIFORD_RXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << USART_FIFORD_RXDATA_SHIFT)) & USART_FIFORD_RXDATA_MASK)

#define USART_FIFORD_FRAMERR_MASK                (0x2000U)
#define USART_FIFORD_FRAMERR_SHIFT               (13U)
/*! FRAMERR - Framing Error Status Flag */
#define USART_FIFORD_FRAMERR(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFORD_FRAMERR_SHIFT)) & USART_FIFORD_FRAMERR_MASK)

#define USART_FIFORD_PARITYERR_MASK              (0x4000U)
#define USART_FIFORD_PARITYERR_SHIFT             (14U)
/*! PARITYERR - Parity Error Status Flag */
#define USART_FIFORD_PARITYERR(x)                (((uint32_t)(((uint32_t)(x)) << USART_FIFORD_PARITYERR_SHIFT)) & USART_FIFORD_PARITYERR_MASK)

#define USART_FIFORD_RXNOISE_MASK                (0x8000U)
#define USART_FIFORD_RXNOISE_SHIFT               (15U)
/*! RXNOISE - Received Noise Flag */
#define USART_FIFORD_RXNOISE(x)                  (((uint32_t)(((uint32_t)(x)) << USART_FIFORD_RXNOISE_SHIFT)) & USART_FIFORD_RXNOISE_MASK)
/*! @} */

/*! @name FIFORDNOPOP - FIFO Data Read with No FIFO Pop */
/*! @{ */

#define USART_FIFORDNOPOP_RXDATA_MASK            (0x1FFU)
#define USART_FIFORDNOPOP_RXDATA_SHIFT           (0U)
/*! RXDATA - Received Data from the FIFO */
#define USART_FIFORDNOPOP_RXDATA(x)              (((uint32_t)(((uint32_t)(x)) << USART_FIFORDNOPOP_RXDATA_SHIFT)) & USART_FIFORDNOPOP_RXDATA_MASK)

#define USART_FIFORDNOPOP_FRAMERR_MASK           (0x2000U)
#define USART_FIFORDNOPOP_FRAMERR_SHIFT          (13U)
/*! FRAMERR - Framing Error Status Flag */
#define USART_FIFORDNOPOP_FRAMERR(x)             (((uint32_t)(((uint32_t)(x)) << USART_FIFORDNOPOP_FRAMERR_SHIFT)) & USART_FIFORDNOPOP_FRAMERR_MASK)

#define USART_FIFORDNOPOP_PARITYERR_MASK         (0x4000U)
#define USART_FIFORDNOPOP_PARITYERR_SHIFT        (14U)
/*! PARITYERR - Parity Error Status Flag */
#define USART_FIFORDNOPOP_PARITYERR(x)           (((uint32_t)(((uint32_t)(x)) << USART_FIFORDNOPOP_PARITYERR_SHIFT)) & USART_FIFORDNOPOP_PARITYERR_MASK)

#define USART_FIFORDNOPOP_RXNOISE_MASK           (0x8000U)
#define USART_FIFORDNOPOP_RXNOISE_SHIFT          (15U)
/*! RXNOISE - Received Noise Flag */
#define USART_FIFORDNOPOP_RXNOISE(x)             (((uint32_t)(((uint32_t)(x)) << USART_FIFORDNOPOP_RXNOISE_SHIFT)) & USART_FIFORDNOPOP_RXNOISE_MASK)
/*! @} */

/*! @name FIFOSIZE - FIFO Size */
/*! @{ */

#define USART_FIFOSIZE_FIFOSIZE_MASK             (0x1FU)
#define USART_FIFOSIZE_FIFOSIZE_SHIFT            (0U)
/*! FIFOSIZE - FIFO Size */
#define USART_FIFOSIZE_FIFOSIZE(x)               (((uint32_t)(((uint32_t)(x)) << USART_FIFOSIZE_FIFOSIZE_SHIFT)) & USART_FIFOSIZE_FIFOSIZE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCFG - FIFO Receive Timeout Configuration */
/*! @{ */

#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK (0xFFU)
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT (0U)
/*! RXTIMEOUT_PRESCALER - Receive Timeout Counter Clock Prescaler */
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER(x) (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT)) & USART_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK)

#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK (0xFFFF00U)
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT (8U)
/*! RXTIMEOUT_VALUE - Receive Timeout Value */
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE(x) (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT)) & USART_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK)

#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK (0x1000000U)
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT (24U)
/*! RXTIMEOUT_EN - Receive Timeout Enable
 *  0b0..Disable RX FIFO timeout
 *  0b1..Enable RX FIFO timeout
 */
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_EN(x)   (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT)) & USART_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK)

#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK (0x2000000U)
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT (25U)
/*! RXTIMEOUT_COW - Receive Timeout Continue On Write
 *  0b0..RX FIFO timeout counter is reset every time data is transferred from the peripheral into the RX FIFO.
 *  0b1..RX FIFO timeout counter is not reset every time data is transferred from the peripheral into the RX FIFO.
 */
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COW(x)  (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT)) & USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK)

#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK (0x4000000U)
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT (26U)
/*! RXTIMEOUT_COE - Receive Timeout Continue On Empty
 *  0b0..RX FIFO timeout counter is reset when the RX FIFO becomes empty.
 *  0b1..RX FIFO timeout counter is not reset when the RX FIFO becomes empty.
 */
#define USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COE(x)  (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT)) & USART_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCNT - FIFO Receive Timeout Counter */
/*! @{ */

#define USART_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK (0xFFFFU)
#define USART_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT (0U)
/*! RXTIMEOUT_CNT - Current RX FIFO timeout counter value */
#define USART_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT(x)  (((uint32_t)(((uint32_t)(x)) << USART_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT)) & USART_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK)
/*! @} */

/*! @name ID - Peripheral Identification */
/*! @{ */

#define USART_ID_APERTURE_MASK                   (0xFFU)
#define USART_ID_APERTURE_SHIFT                  (0U)
/*! APERTURE - Aperture */
#define USART_ID_APERTURE(x)                     (((uint32_t)(((uint32_t)(x)) << USART_ID_APERTURE_SHIFT)) & USART_ID_APERTURE_MASK)

#define USART_ID_MINOR_REV_MASK                  (0xF00U)
#define USART_ID_MINOR_REV_SHIFT                 (8U)
/*! MINOR_REV - Minor revision of module implementation */
#define USART_ID_MINOR_REV(x)                    (((uint32_t)(((uint32_t)(x)) << USART_ID_MINOR_REV_SHIFT)) & USART_ID_MINOR_REV_MASK)

#define USART_ID_MAJOR_REV_MASK                  (0xF000U)
#define USART_ID_MAJOR_REV_SHIFT                 (12U)
/*! MAJOR_REV - Major revision of module implementation */
#define USART_ID_MAJOR_REV(x)                    (((uint32_t)(((uint32_t)(x)) << USART_ID_MAJOR_REV_SHIFT)) & USART_ID_MAJOR_REV_MASK)

#define USART_ID_ID_MASK                         (0xFFFF0000U)
#define USART_ID_ID_SHIFT                        (16U)
/*! ID - Module identifier for the selected function */
#define USART_ID_ID(x)                           (((uint32_t)(((uint32_t)(x)) << USART_ID_ID_SHIFT)) & USART_ID_ID_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group USART_Register_Masks */


/*!
 * @}
 */ /* end of group USART_Peripheral_Access_Layer */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
  #else
    #pragma pop
  #endif
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_access_layer */


#endif  /* USART_H_ */

