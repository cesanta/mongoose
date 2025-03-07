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
**         CMSIS Peripheral Access Layer for SPI
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
 * @file SPI.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SPI
 *
 * CMSIS Peripheral Access Layer for SPI
 */

#if !defined(SPI_H_)
#define SPI_H_                                   /**< Symbol preventing repeated inclusion */

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
   -- SPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Peripheral_Access_Layer SPI Peripheral Access Layer
 * @{
 */

/** SPI - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[1024];
  __IO uint32_t CFG;                               /**< Configuration Register, offset: 0x400 */
  __IO uint32_t DLY;                               /**< Delay Register, offset: 0x404 */
  __IO uint32_t STAT;                              /**< Status Register, offset: 0x408 */
  __IO uint32_t INTENSET;                          /**< Interrupt Enable Register, offset: 0x40C */
  __O  uint32_t INTENCLR;                          /**< Interrupt Enable Clear Register, offset: 0x410 */
       uint8_t RESERVED_1[16];
  __IO uint32_t DIV;                               /**< Clock Divider Register, offset: 0x424 */
  __I  uint32_t INTSTAT;                           /**< Interrupt Status Register, offset: 0x428 */
       uint8_t RESERVED_2[2516];
  __IO uint32_t FIFOCFG;                           /**< FIFO Configuration Register, offset: 0xE00 */
  __IO uint32_t FIFOSTAT;                          /**< FIFO Status Register, offset: 0xE04 */
  __IO uint32_t FIFOTRIG;                          /**< FIFO Trigger Register, offset: 0xE08 */
       uint8_t RESERVED_3[4];
  __IO uint32_t FIFOINTENSET;                      /**< FIFO Interrupt Enable Register, offset: 0xE10 */
  __IO uint32_t FIFOINTENCLR;                      /**< FIFO Interrupt Enable Clear Register, offset: 0xE14 */
  __I  uint32_t FIFOINTSTAT;                       /**< FIFO Interrupt Status Register, offset: 0xE18 */
       uint8_t RESERVED_4[4];
  __O  uint32_t FIFOWR;                            /**< FIFO Write Data Register, offset: 0xE20 */
       uint8_t RESERVED_5[12];
  __I  uint32_t FIFORD;                            /**< FIFO Read Data Register, offset: 0xE30 */
       uint8_t RESERVED_6[12];
  __I  uint32_t FIFORDNOPOP;                       /**< FIFO Data Read with no FIFO Pop Register, offset: 0xE40 */
       uint8_t RESERVED_7[4];
  __I  uint32_t FIFOSIZE;                          /**< FIFO Size Register, offset: 0xE48 */
  __IO uint32_t FIFORXTIMEOUTCFG;                  /**< FIFO Receive Timeout Configuration, offset: 0xE4C */
  __I  uint32_t FIFORXTIMEOUTCNT;                  /**< FIFO Receive Timeout Counter, offset: 0xE50 */
       uint8_t RESERVED_8[424];
  __I  uint32_t ID;                                /**< Peripheral Identification Register, offset: 0xFFC */
} SPI_Type;

/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/*! @name CFG - Configuration Register */
/*! @{ */

#define SPI_CFG_ENABLE_MASK                      (0x1U)
#define SPI_CFG_ENABLE_SHIFT                     (0U)
/*! ENABLE - SPI Enable
 *  0b0..Disabled. The SPI is disabled and the internal state machine and counters are reset.
 *  0b1..Enabled. The SPI is enabled for operation.
 */
#define SPI_CFG_ENABLE(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_CFG_ENABLE_SHIFT)) & SPI_CFG_ENABLE_MASK)

#define SPI_CFG_MASTER_MASK                      (0x4U)
#define SPI_CFG_MASTER_SHIFT                     (2U)
/*! MASTER - Master Mode Select
 *  0b0..Slave mode. The SPI will operate in slave mode. SCK, MOSI, and the SSEL signals are inputs; MISO is an output.
 *  0b1..Master mode. The SPI will operate in master mode. SCK, MOSI, and the SSEL signals are outputs; MISO is an input.
 */
#define SPI_CFG_MASTER(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_CFG_MASTER_SHIFT)) & SPI_CFG_MASTER_MASK)

#define SPI_CFG_LSBF_MASK                        (0x8U)
#define SPI_CFG_LSBF_SHIFT                       (3U)
/*! LSBF - LSB First Mode Enable
 *  0b0..Standard. Data is transmitted and received in standard MSB-first order.
 *  0b1..Reverse. Data is transmitted and received in reverse order (LSB first).
 */
#define SPI_CFG_LSBF(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_CFG_LSBF_SHIFT)) & SPI_CFG_LSBF_MASK)

#define SPI_CFG_CPHA_MASK                        (0x10U)
#define SPI_CFG_CPHA_SHIFT                       (4U)
/*! CPHA - Clock Phase Select
 *  0b0..Change
 *  0b1..Capture
 */
#define SPI_CFG_CPHA(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_CFG_CPHA_SHIFT)) & SPI_CFG_CPHA_MASK)

#define SPI_CFG_CPOL_MASK                        (0x20U)
#define SPI_CFG_CPOL_SHIFT                       (5U)
/*! CPOL - Clock Polarity Select
 *  0b0..Low. The rest state of the clock (between transfers) is low.
 *  0b1..High. The rest state of the clock (between transfers) is high.
 */
#define SPI_CFG_CPOL(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_CFG_CPOL_SHIFT)) & SPI_CFG_CPOL_MASK)

#define SPI_CFG_LOOP_MASK                        (0x80U)
#define SPI_CFG_LOOP_SHIFT                       (7U)
/*! LOOP - Loopback Mode Enable
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define SPI_CFG_LOOP(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_CFG_LOOP_SHIFT)) & SPI_CFG_LOOP_MASK)

#define SPI_CFG_SPOL0_MASK                       (0x100U)
#define SPI_CFG_SPOL0_SHIFT                      (8U)
/*! SPOL0 - SSEL0 Polarity Select
 *  0b0..Low. The SSEL0 pin is active low.
 *  0b1..High. The SSEL0 pin is active high.
 */
#define SPI_CFG_SPOL0(x)                         (((uint32_t)(((uint32_t)(x)) << SPI_CFG_SPOL0_SHIFT)) & SPI_CFG_SPOL0_MASK)

#define SPI_CFG_SPOL1_MASK                       (0x200U)
#define SPI_CFG_SPOL1_SHIFT                      (9U)
/*! SPOL1 - SSEL1 Polarity Select
 *  0b0..Low. The SSEL1 pin is active low.
 *  0b1..High. The SSEL1 pin is active high.
 */
#define SPI_CFG_SPOL1(x)                         (((uint32_t)(((uint32_t)(x)) << SPI_CFG_SPOL1_SHIFT)) & SPI_CFG_SPOL1_MASK)

#define SPI_CFG_SPOL2_MASK                       (0x400U)
#define SPI_CFG_SPOL2_SHIFT                      (10U)
/*! SPOL2 - SSEL2 Polarity Select
 *  0b0..Low. The SSEL2 pin is active low.
 *  0b1..High. The SSEL2 pin is active high.
 */
#define SPI_CFG_SPOL2(x)                         (((uint32_t)(((uint32_t)(x)) << SPI_CFG_SPOL2_SHIFT)) & SPI_CFG_SPOL2_MASK)

#define SPI_CFG_SPOL3_MASK                       (0x800U)
#define SPI_CFG_SPOL3_SHIFT                      (11U)
/*! SPOL3 - SSEL3 Polarity Select
 *  0b0..Low. The SSEL3 pin is active low.
 *  0b1..High. The SSEL3 pin is active high.
 */
#define SPI_CFG_SPOL3(x)                         (((uint32_t)(((uint32_t)(x)) << SPI_CFG_SPOL3_SHIFT)) & SPI_CFG_SPOL3_MASK)
/*! @} */

/*! @name DLY - Delay Register */
/*! @{ */

#define SPI_DLY_PRE_DELAY_MASK                   (0xFU)
#define SPI_DLY_PRE_DELAY_SHIFT                  (0U)
/*! PRE_DELAY - Pre-Delay
 *  0b0000..No additional time is inserted
 *  0b0001..1 SPI clock time is inserted
 *  0b0010..2 SPI clock times are inserted
 *  0b1111..15 SPI clock times are inserted
 */
#define SPI_DLY_PRE_DELAY(x)                     (((uint32_t)(((uint32_t)(x)) << SPI_DLY_PRE_DELAY_SHIFT)) & SPI_DLY_PRE_DELAY_MASK)

#define SPI_DLY_POST_DELAY_MASK                  (0xF0U)
#define SPI_DLY_POST_DELAY_SHIFT                 (4U)
/*! POST_DELAY - Post-Delay
 *  0b0000..No additional time is inserted
 *  0b0001..1 SPI clock time is inserted
 *  0b0010..2 SPI clock times are inserted
 *  0b1111..15 SPI clock times are inserted
 */
#define SPI_DLY_POST_DELAY(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_DLY_POST_DELAY_SHIFT)) & SPI_DLY_POST_DELAY_MASK)

#define SPI_DLY_FRAME_DELAY_MASK                 (0xF00U)
#define SPI_DLY_FRAME_DELAY_SHIFT                (8U)
/*! FRAME_DELAY - Frame Delay
 *  0b0000..No additional time is inserted
 *  0b0001..1 SPI clock time is inserted
 *  0b0010..2 SPI clock times are inserted
 *  0b1111..15 SPI clock times are inserted
 */
#define SPI_DLY_FRAME_DELAY(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_DLY_FRAME_DELAY_SHIFT)) & SPI_DLY_FRAME_DELAY_MASK)

#define SPI_DLY_TRANSFER_DELAY_MASK              (0xF000U)
#define SPI_DLY_TRANSFER_DELAY_SHIFT             (12U)
/*! TRANSFER_DELAY - Transfer Delay
 *  0b0000..The minimum time that SSEL is deasserted is 1 SPI clock time (zero-added time)
 *  0b0001..The minimum time that SSEL is deasserted is 2 SPI clock times
 *  0b0010..The minimum time that SSEL is deasserted is 3 SPI clock times
 *  0b1111..The minimum time that SSEL is deasserted is 16 SPI clock times
 */
#define SPI_DLY_TRANSFER_DELAY(x)                (((uint32_t)(((uint32_t)(x)) << SPI_DLY_TRANSFER_DELAY_SHIFT)) & SPI_DLY_TRANSFER_DELAY_MASK)
/*! @} */

/*! @name STAT - Status Register */
/*! @{ */

#define SPI_STAT_SSA_MASK                        (0x10U)
#define SPI_STAT_SSA_SHIFT                       (4U)
/*! SSA - Slave Select Assert */
#define SPI_STAT_SSA(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_STAT_SSA_SHIFT)) & SPI_STAT_SSA_MASK)

#define SPI_STAT_SSD_MASK                        (0x20U)
#define SPI_STAT_SSD_SHIFT                       (5U)
/*! SSD - Slave Select Deassert */
#define SPI_STAT_SSD(x)                          (((uint32_t)(((uint32_t)(x)) << SPI_STAT_SSD_SHIFT)) & SPI_STAT_SSD_MASK)

#define SPI_STAT_STALLED_MASK                    (0x40U)
#define SPI_STAT_STALLED_SHIFT                   (6U)
/*! STALLED - Stalled Status Flag */
#define SPI_STAT_STALLED(x)                      (((uint32_t)(((uint32_t)(x)) << SPI_STAT_STALLED_SHIFT)) & SPI_STAT_STALLED_MASK)

#define SPI_STAT_ENDTRANSFER_MASK                (0x80U)
#define SPI_STAT_ENDTRANSFER_SHIFT               (7U)
/*! ENDTRANSFER - End Transfer Control */
#define SPI_STAT_ENDTRANSFER(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_STAT_ENDTRANSFER_SHIFT)) & SPI_STAT_ENDTRANSFER_MASK)

#define SPI_STAT_MSTIDLE_MASK                    (0x100U)
#define SPI_STAT_MSTIDLE_SHIFT                   (8U)
/*! MSTIDLE - Master Idle Status Flag */
#define SPI_STAT_MSTIDLE(x)                      (((uint32_t)(((uint32_t)(x)) << SPI_STAT_MSTIDLE_SHIFT)) & SPI_STAT_MSTIDLE_MASK)
/*! @} */

/*! @name INTENSET - Interrupt Enable Register */
/*! @{ */

#define SPI_INTENSET_SSAEN_MASK                  (0x10U)
#define SPI_INTENSET_SSAEN_SHIFT                 (4U)
/*! SSAEN - Slave Select Assert Interrupt Enable
 *  0b0..Disabled. No interrupt will be generated when any Slave Select transitions from deasserted to asserted.
 *  0b1..Enabled. An interrupt will be generated when any Slave Select transitions from deasserted to asserted.
 */
#define SPI_INTENSET_SSAEN(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_INTENSET_SSAEN_SHIFT)) & SPI_INTENSET_SSAEN_MASK)

#define SPI_INTENSET_SSDEN_MASK                  (0x20U)
#define SPI_INTENSET_SSDEN_SHIFT                 (5U)
/*! SSDEN - Slave Select Deassert Interrupt Enable
 *  0b0..Disabled. No interrupt will be generated when all asserted Slave Selects transition to deasserted.
 *  0b1..Enabled. An interrupt will be generated when all asserted Slave Selects transition to deasserted.
 */
#define SPI_INTENSET_SSDEN(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_INTENSET_SSDEN_SHIFT)) & SPI_INTENSET_SSDEN_MASK)

#define SPI_INTENSET_MSTIDLEEN_MASK              (0x100U)
#define SPI_INTENSET_MSTIDLEEN_SHIFT             (8U)
/*! MSTIDLEEN - Master Idle Interrupt Enable
 *  0b0..No interrupt will be generated when the SPI master function is idle.
 *  0b1..An interrupt will be generated when the SPI master function is fully idle.
 */
#define SPI_INTENSET_MSTIDLEEN(x)                (((uint32_t)(((uint32_t)(x)) << SPI_INTENSET_MSTIDLEEN_SHIFT)) & SPI_INTENSET_MSTIDLEEN_MASK)
/*! @} */

/*! @name INTENCLR - Interrupt Enable Clear Register */
/*! @{ */

#define SPI_INTENCLR_SSAEN_MASK                  (0x10U)
#define SPI_INTENCLR_SSAEN_SHIFT                 (4U)
/*! SSAEN - Slave Select Assert Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Slave Select Assert Interrupt Enable bit (INTENSET[SSAEN])
 */
#define SPI_INTENCLR_SSAEN(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_INTENCLR_SSAEN_SHIFT)) & SPI_INTENCLR_SSAEN_MASK)

#define SPI_INTENCLR_SSDEN_MASK                  (0x20U)
#define SPI_INTENCLR_SSDEN_SHIFT                 (5U)
/*! SSDEN - Slave Select Deassert Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Slave Select Deassert Interrupt Enable bit (INTENSET[SSDEN])
 */
#define SPI_INTENCLR_SSDEN(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_INTENCLR_SSDEN_SHIFT)) & SPI_INTENCLR_SSDEN_MASK)

#define SPI_INTENCLR_MSTIDLE_MASK                (0x100U)
#define SPI_INTENCLR_MSTIDLE_SHIFT               (8U)
/*! MSTIDLE - Master Idle Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Master Idle Interrupt Enable bit (INTENSET[MSTIDLE])
 */
#define SPI_INTENCLR_MSTIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_INTENCLR_MSTIDLE_SHIFT)) & SPI_INTENCLR_MSTIDLE_MASK)
/*! @} */

/*! @name DIV - Clock Divider Register */
/*! @{ */

#define SPI_DIV_DIVVAL_MASK                      (0xFFFFU)
#define SPI_DIV_DIVVAL_SHIFT                     (0U)
/*! DIVVAL - Rate Divider Value */
#define SPI_DIV_DIVVAL(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_DIV_DIVVAL_SHIFT)) & SPI_DIV_DIVVAL_MASK)
/*! @} */

/*! @name INTSTAT - Interrupt Status Register */
/*! @{ */

#define SPI_INTSTAT_SSA_MASK                     (0x10U)
#define SPI_INTSTAT_SSA_SHIFT                    (4U)
/*! SSA - Slave Select Assert Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define SPI_INTSTAT_SSA(x)                       (((uint32_t)(((uint32_t)(x)) << SPI_INTSTAT_SSA_SHIFT)) & SPI_INTSTAT_SSA_MASK)

#define SPI_INTSTAT_SSD_MASK                     (0x20U)
#define SPI_INTSTAT_SSD_SHIFT                    (5U)
/*! SSD - Slave Select Deassert Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define SPI_INTSTAT_SSD(x)                       (((uint32_t)(((uint32_t)(x)) << SPI_INTSTAT_SSD_SHIFT)) & SPI_INTSTAT_SSD_MASK)

#define SPI_INTSTAT_MSTIDLE_MASK                 (0x100U)
#define SPI_INTSTAT_MSTIDLE_SHIFT                (8U)
/*! MSTIDLE - Master Idle Status Flag Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define SPI_INTSTAT_MSTIDLE(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_INTSTAT_MSTIDLE_SHIFT)) & SPI_INTSTAT_MSTIDLE_MASK)
/*! @} */

/*! @name FIFOCFG - FIFO Configuration Register */
/*! @{ */

#define SPI_FIFOCFG_ENABLETX_MASK                (0x1U)
#define SPI_FIFOCFG_ENABLETX_SHIFT               (0U)
/*! ENABLETX - Enable the Transmit FIFO
 *  0b0..The transmit FIFO is not enabled
 *  0b1..The transmit FIFO is enabled
 */
#define SPI_FIFOCFG_ENABLETX(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_ENABLETX_SHIFT)) & SPI_FIFOCFG_ENABLETX_MASK)

#define SPI_FIFOCFG_ENABLERX_MASK                (0x2U)
#define SPI_FIFOCFG_ENABLERX_SHIFT               (1U)
/*! ENABLERX - Enable the Receive FIFO
 *  0b0..The receive FIFO is not enabled
 *  0b1..The receive FIFO is enabled
 */
#define SPI_FIFOCFG_ENABLERX(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_ENABLERX_SHIFT)) & SPI_FIFOCFG_ENABLERX_MASK)

#define SPI_FIFOCFG_SIZE_MASK                    (0x30U)
#define SPI_FIFOCFG_SIZE_SHIFT                   (4U)
/*! SIZE - FIFO Size Configuration
 *  0b00..FIFO is configured as 16 entries of 8 bits.
 *  0b01..FIFO is configured as 8 entries of 16 bits.
 *  0b10..Not used
 *  0b11..Not used
 */
#define SPI_FIFOCFG_SIZE(x)                      (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_SIZE_SHIFT)) & SPI_FIFOCFG_SIZE_MASK)

#define SPI_FIFOCFG_DMATX_MASK                   (0x1000U)
#define SPI_FIFOCFG_DMATX_SHIFT                  (12U)
/*! DMATX - DMA Configuration for Transmit
 *  0b0..DMA is not used for the transmit function
 *  0b1..Issues DMA request for the transmit function if the FIFO is not full. Generally, data interrupts would be disabled if DMA is enabled.
 */
#define SPI_FIFOCFG_DMATX(x)                     (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_DMATX_SHIFT)) & SPI_FIFOCFG_DMATX_MASK)

#define SPI_FIFOCFG_DMARX_MASK                   (0x2000U)
#define SPI_FIFOCFG_DMARX_SHIFT                  (13U)
/*! DMARX - DMA Configuration for Receive
 *  0b0..DMA is not used for the receive function.
 *  0b1..Issues a DMA request for the receive function if the FIFO is not empty. Generally, data interrupts would be disabled if DMA is enabled.
 */
#define SPI_FIFOCFG_DMARX(x)                     (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_DMARX_SHIFT)) & SPI_FIFOCFG_DMARX_MASK)

#define SPI_FIFOCFG_WAKETX_MASK                  (0x4000U)
#define SPI_FIFOCFG_WAKETX_SHIFT                 (14U)
/*! WAKETX - Wake-up for Transmit FIFO Level
 *  0b0..Only enabled interrupts will wake up the device form reduced power modes
 *  0b1..A device wake-up for DMA will occur if the transmit FIFO level reaches the value specified by TXLVL in
 *       FIFOTRIG, even when the TXLVL interrupt is not enabled.
 */
#define SPI_FIFOCFG_WAKETX(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_WAKETX_SHIFT)) & SPI_FIFOCFG_WAKETX_MASK)

#define SPI_FIFOCFG_WAKERX_MASK                  (0x8000U)
#define SPI_FIFOCFG_WAKERX_SHIFT                 (15U)
/*! WAKERX - Wake-up for Receive FIFO Level
 *  0b0..Only enabled interrupts will wake up the device form reduced power modes.
 *  0b1..A device wake-up for DMA will occur if the receive FIFO level reaches the value specified by
 *       FIFOTRIG[RXLVL], even when the RXLVL interrupt is not enabled.
 */
#define SPI_FIFOCFG_WAKERX(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_WAKERX_SHIFT)) & SPI_FIFOCFG_WAKERX_MASK)

#define SPI_FIFOCFG_EMPTYTX_MASK                 (0x10000U)
#define SPI_FIFOCFG_EMPTYTX_SHIFT                (16U)
/*! EMPTYTX - Empty Command for the Transmit FIFO
 *  0b0..No effect
 *  0b1..The TX FIFO is emptied
 */
#define SPI_FIFOCFG_EMPTYTX(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_EMPTYTX_SHIFT)) & SPI_FIFOCFG_EMPTYTX_MASK)

#define SPI_FIFOCFG_EMPTYRX_MASK                 (0x20000U)
#define SPI_FIFOCFG_EMPTYRX_SHIFT                (17U)
/*! EMPTYRX - Empty Command for the Receive FIFO
 *  0b0..No effect
 *  0b1..The RX FIFO is emptied
 */
#define SPI_FIFOCFG_EMPTYRX(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_EMPTYRX_SHIFT)) & SPI_FIFOCFG_EMPTYRX_MASK)

#define SPI_FIFOCFG_POPDBG_MASK                  (0x40000U)
#define SPI_FIFOCFG_POPDBG_SHIFT                 (18U)
/*! POPDBG - Pop FIFO for Debug Reads
 *  0b0..Debug reads of the FIFO do not pop the FIFO
 *  0b1..A debug read will cause the FIFO to pop
 */
#define SPI_FIFOCFG_POPDBG(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOCFG_POPDBG_SHIFT)) & SPI_FIFOCFG_POPDBG_MASK)
/*! @} */

/*! @name FIFOSTAT - FIFO Status Register */
/*! @{ */

#define SPI_FIFOSTAT_TXERR_MASK                  (0x1U)
#define SPI_FIFOSTAT_TXERR_SHIFT                 (0U)
/*! TXERR - TX FIFO Error
 *  0b0..A transmit FIFO error has not occurred.
 *  0b1..A transmit FIFO error has occurred. This error could be an overflow caused by pushing data into a full
 *       FIFO, or by an underflow if the FIFO is empty when data is needed.
 */
#define SPI_FIFOSTAT_TXERR(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_TXERR_SHIFT)) & SPI_FIFOSTAT_TXERR_MASK)

#define SPI_FIFOSTAT_RXERR_MASK                  (0x2U)
#define SPI_FIFOSTAT_RXERR_SHIFT                 (1U)
/*! RXERR - RX FIFO Error
 *  0b0..A receive FIFO overflow has not occurred
 *  0b1..A receive FIFO overflow has occurred, caused by software or DMA not emptying the FIFO fast enough
 */
#define SPI_FIFOSTAT_RXERR(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_RXERR_SHIFT)) & SPI_FIFOSTAT_RXERR_MASK)

#define SPI_FIFOSTAT_PERINT_MASK                 (0x8U)
#define SPI_FIFOSTAT_PERINT_SHIFT                (3U)
/*! PERINT - Peripheral Interrupt
 *  0b0..The peripheral function has not asserted an interrupt
 *  0b1..Indicates that the peripheral function has asserted an interrupt. More information can be found by
 *       reading the peripheral's status register (STAT).
 */
#define SPI_FIFOSTAT_PERINT(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_PERINT_SHIFT)) & SPI_FIFOSTAT_PERINT_MASK)

#define SPI_FIFOSTAT_TXEMPTY_MASK                (0x10U)
#define SPI_FIFOSTAT_TXEMPTY_SHIFT               (4U)
/*! TXEMPTY - Transmit FIFO Empty
 *  0b0..The transmit FIFO is not empty
 *  0b1..The transmit FIFO is empty, although the peripheral may still be processing the last piece of data.
 */
#define SPI_FIFOSTAT_TXEMPTY(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_TXEMPTY_SHIFT)) & SPI_FIFOSTAT_TXEMPTY_MASK)

#define SPI_FIFOSTAT_TXNOTFULL_MASK              (0x20U)
#define SPI_FIFOSTAT_TXNOTFULL_SHIFT             (5U)
/*! TXNOTFULL - Transmit FIFO is Not Full
 *  0b0..The transmit FIFO is full and another write would cause it to overflow
 *  0b1..The transmit FIFO is not full, so more data can be written
 */
#define SPI_FIFOSTAT_TXNOTFULL(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_TXNOTFULL_SHIFT)) & SPI_FIFOSTAT_TXNOTFULL_MASK)

#define SPI_FIFOSTAT_RXNOTEMPTY_MASK             (0x40U)
#define SPI_FIFOSTAT_RXNOTEMPTY_SHIFT            (6U)
/*! RXNOTEMPTY - Receive FIFO is Not Empty
 *  0b0..When 0, the receive FIFO is empty
 *  0b1..When 1, the receive FIFO is not empty, so data can be read
 */
#define SPI_FIFOSTAT_RXNOTEMPTY(x)               (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_RXNOTEMPTY_SHIFT)) & SPI_FIFOSTAT_RXNOTEMPTY_MASK)

#define SPI_FIFOSTAT_RXFULL_MASK                 (0x80U)
#define SPI_FIFOSTAT_RXFULL_SHIFT                (7U)
/*! RXFULL - Receive FIFO is Full
 *  0b0..The receive FIFO is not full
 *  0b1..The receive FIFO is full. To prevent the peripheral from causing an overflow, data should be read out.
 */
#define SPI_FIFOSTAT_RXFULL(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_RXFULL_SHIFT)) & SPI_FIFOSTAT_RXFULL_MASK)

#define SPI_FIFOSTAT_TXLVL_MASK                  (0x1F00U)
#define SPI_FIFOSTAT_TXLVL_SHIFT                 (8U)
/*! TXLVL - Transmit FIFO Current Level */
#define SPI_FIFOSTAT_TXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_TXLVL_SHIFT)) & SPI_FIFOSTAT_TXLVL_MASK)

#define SPI_FIFOSTAT_RXLVL_MASK                  (0x1F0000U)
#define SPI_FIFOSTAT_RXLVL_SHIFT                 (16U)
/*! RXLVL - Receive FIFO Current Level */
#define SPI_FIFOSTAT_RXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_RXLVL_SHIFT)) & SPI_FIFOSTAT_RXLVL_MASK)

#define SPI_FIFOSTAT_RXTIMEOUT_MASK              (0x1000000U)
#define SPI_FIFOSTAT_RXTIMEOUT_SHIFT             (24U)
/*! RXTIMEOUT - Receive FIFO Timeout
 *  0b0..RX FIFO on
 *  0b1..RX FIFO has timed out, based on the timeout configuration in the FIFORXTIMEOUTCFG register.
 */
#define SPI_FIFOSTAT_RXTIMEOUT(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSTAT_RXTIMEOUT_SHIFT)) & SPI_FIFOSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOTRIG - FIFO Trigger Register */
/*! @{ */

#define SPI_FIFOTRIG_TXLVLENA_MASK               (0x1U)
#define SPI_FIFOTRIG_TXLVLENA_SHIFT              (0U)
/*! TXLVLENA - Transmit FIFO Level Trigger Enable
 *  0b0..Transmit FIFO level does not generate a FIFO level trigger
 *  0b1..An trigger will be generated if the transmit FIFO level reaches the value specified by the FIFOTRIG[TXLVL] field.
 */
#define SPI_FIFOTRIG_TXLVLENA(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOTRIG_TXLVLENA_SHIFT)) & SPI_FIFOTRIG_TXLVLENA_MASK)

#define SPI_FIFOTRIG_RXLVLENA_MASK               (0x2U)
#define SPI_FIFOTRIG_RXLVLENA_SHIFT              (1U)
/*! RXLVLENA - Receive FIFO Level Trigger Enable
 *  0b0..Receive FIFO level does not generate a FIFO level trigger
 *  0b1..An trigger will be generated if the receive FIFO level reaches the value specified by the FIFOTRIG[RXLVL] field.
 */
#define SPI_FIFOTRIG_RXLVLENA(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOTRIG_RXLVLENA_SHIFT)) & SPI_FIFOTRIG_RXLVLENA_MASK)

#define SPI_FIFOTRIG_TXLVL_MASK                  (0xF00U)
#define SPI_FIFOTRIG_TXLVL_SHIFT                 (8U)
/*! TXLVL - Transmit FIFO Level Trigger Point
 *  0b0000..Trigger when the TX FIFO becomes empty
 *  0b0001..Trigger when the TX FIFO level decreases to 1 entry
 *  0b1111..Trigger when the TX FIFO level decreases to 15 entries (is no longer full)
 */
#define SPI_FIFOTRIG_TXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOTRIG_TXLVL_SHIFT)) & SPI_FIFOTRIG_TXLVL_MASK)

#define SPI_FIFOTRIG_RXLVL_MASK                  (0xF0000U)
#define SPI_FIFOTRIG_RXLVL_SHIFT                 (16U)
/*! RXLVL - Receive FIFO Level Trigger Point
 *  0b0000..Trigger when the RX FIFO has received 1 entry (is no longer empty)
 *  0b0001..Trigger when the RX FIFO has received 2 entries
 *  0b1111..Trigger when the RX FIFO has received 16 entries (has become full)
 */
#define SPI_FIFOTRIG_RXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << SPI_FIFOTRIG_RXLVL_SHIFT)) & SPI_FIFOTRIG_RXLVL_MASK)
/*! @} */

/*! @name FIFOINTENSET - FIFO Interrupt Enable Register */
/*! @{ */

#define SPI_FIFOINTENSET_TXERR_MASK              (0x1U)
#define SPI_FIFOINTENSET_TXERR_SHIFT             (0U)
/*! TXERR - TX Error Interrupt Enable
 *  0b0..No interrupt will be generated for a transmit error
 *  0b1..An interrupt will be generated when a transmit error occurs
 */
#define SPI_FIFOINTENSET_TXERR(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENSET_TXERR_SHIFT)) & SPI_FIFOINTENSET_TXERR_MASK)

#define SPI_FIFOINTENSET_RXERR_MASK              (0x2U)
#define SPI_FIFOINTENSET_RXERR_SHIFT             (1U)
/*! RXERR - Receive Error Interrupt Enable
 *  0b0..No interrupt will be generated for a receive error
 *  0b1..An interrupt will be generated when a receive error occurs
 */
#define SPI_FIFOINTENSET_RXERR(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENSET_RXERR_SHIFT)) & SPI_FIFOINTENSET_RXERR_MASK)

#define SPI_FIFOINTENSET_TXLVL_MASK              (0x4U)
#define SPI_FIFOINTENSET_TXLVL_SHIFT             (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Enable
 *  0b0..No interrupt will be generated based on the TX FIFO level
 *  0b1..If FIFOTRIG[TXLVLENA]=1, then an interrupt will be generated when the TX FIFO level decreases to the level specified by FIFOTRIG[TXLVL]
 */
#define SPI_FIFOINTENSET_TXLVL(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENSET_TXLVL_SHIFT)) & SPI_FIFOINTENSET_TXLVL_MASK)

#define SPI_FIFOINTENSET_RXLVL_MASK              (0x8U)
#define SPI_FIFOINTENSET_RXLVL_SHIFT             (3U)
/*! RXLVL - Receive FIFO Level Interrupt Enable
 *  0b0..No interrupt will be generated based on the RX FIFO level
 *  0b1..If FIFOTRIG[RXLVLENA]=1, then an interrupt will be generated when the RX FIFO level increases to the level specified by FIFOTRIG[RXLVL]
 */
#define SPI_FIFOINTENSET_RXLVL(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENSET_RXLVL_SHIFT)) & SPI_FIFOINTENSET_RXLVL_MASK)

#define SPI_FIFOINTENSET_RXTIMEOUT_MASK          (0x1000000U)
#define SPI_FIFOINTENSET_RXTIMEOUT_SHIFT         (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No RX interrupt will be generated.
 *  0b1..Asserts RX interrupt if RX FIFO Timeout event occurs.
 */
#define SPI_FIFOINTENSET_RXTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENSET_RXTIMEOUT_SHIFT)) & SPI_FIFOINTENSET_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTENCLR - FIFO Interrupt Enable Clear Register */
/*! @{ */

#define SPI_FIFOINTENCLR_TXERR_MASK              (0x1U)
#define SPI_FIFOINTENCLR_TXERR_SHIFT             (0U)
/*! TXERR - TX Error Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the TX Error Interrupt Enable bit FIFOINTENSET[TXERR]
 */
#define SPI_FIFOINTENCLR_TXERR(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENCLR_TXERR_SHIFT)) & SPI_FIFOINTENCLR_TXERR_MASK)

#define SPI_FIFOINTENCLR_RXERR_MASK              (0x2U)
#define SPI_FIFOINTENCLR_RXERR_SHIFT             (1U)
/*! RXERR - Receive Error Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Receive Error Interrupt Enable bit FIFOINTENSET[RXERR]
 */
#define SPI_FIFOINTENCLR_RXERR(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENCLR_RXERR_SHIFT)) & SPI_FIFOINTENCLR_RXERR_MASK)

#define SPI_FIFOINTENCLR_TXLVL_MASK              (0x4U)
#define SPI_FIFOINTENCLR_TXLVL_SHIFT             (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Transmit FIFO Level Interrupt Enable bit FIFOINTENSET[TXLVL]
 */
#define SPI_FIFOINTENCLR_TXLVL(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENCLR_TXLVL_SHIFT)) & SPI_FIFOINTENCLR_TXLVL_MASK)

#define SPI_FIFOINTENCLR_RXLVL_MASK              (0x8U)
#define SPI_FIFOINTENCLR_RXLVL_SHIFT             (3U)
/*! RXLVL - Receive FIFO Level Interrupt Enable
 *  0b0..No effect
 *  0b1..Clear the Receive FIFO Level Interrupt Enable bit FIFOINTENSET[RXLVL]
 */
#define SPI_FIFOINTENCLR_RXLVL(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENCLR_RXLVL_SHIFT)) & SPI_FIFOINTENCLR_RXLVL_MASK)

#define SPI_FIFOINTENCLR_RXTIMEOUT_MASK          (0x1000000U)
#define SPI_FIFOINTENCLR_RXTIMEOUT_SHIFT         (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define SPI_FIFOINTENCLR_RXTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTENCLR_RXTIMEOUT_SHIFT)) & SPI_FIFOINTENCLR_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTSTAT - FIFO Interrupt Status Register */
/*! @{ */

#define SPI_FIFOINTSTAT_TXERR_MASK               (0x1U)
#define SPI_FIFOINTSTAT_TXERR_SHIFT              (0U)
/*! TXERR - TX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_TXERR(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_TXERR_SHIFT)) & SPI_FIFOINTSTAT_TXERR_MASK)

#define SPI_FIFOINTSTAT_RXERR_MASK               (0x2U)
#define SPI_FIFOINTSTAT_RXERR_SHIFT              (1U)
/*! RXERR - RX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_RXERR(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_RXERR_SHIFT)) & SPI_FIFOINTSTAT_RXERR_MASK)

#define SPI_FIFOINTSTAT_TXLVL_MASK               (0x4U)
#define SPI_FIFOINTSTAT_TXLVL_SHIFT              (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_TXLVL(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_TXLVL_SHIFT)) & SPI_FIFOINTSTAT_TXLVL_MASK)

#define SPI_FIFOINTSTAT_RXLVL_MASK               (0x8U)
#define SPI_FIFOINTSTAT_RXLVL_SHIFT              (3U)
/*! RXLVL - Receive FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_RXLVL(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_RXLVL_SHIFT)) & SPI_FIFOINTSTAT_RXLVL_MASK)

#define SPI_FIFOINTSTAT_PERINT_MASK              (0x10U)
#define SPI_FIFOINTSTAT_PERINT_SHIFT             (4U)
/*! PERINT - Peripheral Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_PERINT(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_PERINT_SHIFT)) & SPI_FIFOINTSTAT_PERINT_MASK)

#define SPI_FIFOINTSTAT_RXTIMEOUT_MASK           (0x1000000U)
#define SPI_FIFOINTSTAT_RXTIMEOUT_SHIFT          (24U)
/*! RXTIMEOUT - Receive Timeout Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define SPI_FIFOINTSTAT_RXTIMEOUT(x)             (((uint32_t)(((uint32_t)(x)) << SPI_FIFOINTSTAT_RXTIMEOUT_SHIFT)) & SPI_FIFOINTSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOWR - FIFO Write Data Register */
/*! @{ */

#define SPI_FIFOWR_TXDATA_MASK                   (0xFFFFU)
#define SPI_FIFOWR_TXDATA_SHIFT                  (0U)
/*! TXDATA - Transmit Data to the FIFO */
#define SPI_FIFOWR_TXDATA(x)                     (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXDATA_SHIFT)) & SPI_FIFOWR_TXDATA_MASK)

#define SPI_FIFOWR_TXSSEL0_N_MASK                (0x10000U)
#define SPI_FIFOWR_TXSSEL0_N_SHIFT               (16U)
/*! TXSSEL0_N - Transmit Slave Select 0
 *  0b0..SSEL0 is asserted
 *  0b1..SSEL0 is not asserted
 */
#define SPI_FIFOWR_TXSSEL0_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXSSEL0_N_SHIFT)) & SPI_FIFOWR_TXSSEL0_N_MASK)

#define SPI_FIFOWR_TXSSEL1_N_MASK                (0x20000U)
#define SPI_FIFOWR_TXSSEL1_N_SHIFT               (17U)
/*! TXSSEL1_N - Transmit Slave Select 1
 *  0b0..SSEL1 is asserted
 *  0b1..SSEL1 is not asserted
 */
#define SPI_FIFOWR_TXSSEL1_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXSSEL1_N_SHIFT)) & SPI_FIFOWR_TXSSEL1_N_MASK)

#define SPI_FIFOWR_TXSSEL2_N_MASK                (0x40000U)
#define SPI_FIFOWR_TXSSEL2_N_SHIFT               (18U)
/*! TXSSEL2_N - Transmit Slave Select 2
 *  0b0..SSEL2 is asserted
 *  0b1..SSEL2 is not asserted
 */
#define SPI_FIFOWR_TXSSEL2_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXSSEL2_N_SHIFT)) & SPI_FIFOWR_TXSSEL2_N_MASK)

#define SPI_FIFOWR_TXSSEL3_N_MASK                (0x80000U)
#define SPI_FIFOWR_TXSSEL3_N_SHIFT               (19U)
/*! TXSSEL3_N - Transmit Slave Select 3
 *  0b0..SSEL3 is asserted
 *  0b1..SSEL3 is not asserted
 */
#define SPI_FIFOWR_TXSSEL3_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXSSEL3_N_SHIFT)) & SPI_FIFOWR_TXSSEL3_N_MASK)

#define SPI_FIFOWR_EOT_MASK                      (0x100000U)
#define SPI_FIFOWR_EOT_SHIFT                     (20U)
/*! EOT - End of Transfer
 *  0b0..SSEL is not deasserted. This piece of data is not treated as the end of a transfer. SSEL will not be deasserted at the end of this data.
 *  0b1..SSEL is deasserted. This piece of data is treated as the end of a transfer. SSEL will be deasserted at the end of this piece of data.
 */
#define SPI_FIFOWR_EOT(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_EOT_SHIFT)) & SPI_FIFOWR_EOT_MASK)

#define SPI_FIFOWR_EOF_MASK                      (0x200000U)
#define SPI_FIFOWR_EOF_SHIFT                     (21U)
/*! EOF - End of Frame
 *  0b0..Data not EOF. This piece of data transmitted is not treated as the end of a frame.
 *  0b1..Data EOF. This piece of data is treated as the end of a frame, causing the Frame_delay time to be
 *       inserted before subsequent data is transmitted.
 */
#define SPI_FIFOWR_EOF(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_EOF_SHIFT)) & SPI_FIFOWR_EOF_MASK)

#define SPI_FIFOWR_RXIGNORE_MASK                 (0x400000U)
#define SPI_FIFOWR_RXIGNORE_SHIFT                (22U)
/*! RXIGNORE - Receive Ignore
 *  0b0..Read received data. Received data must be read, to allow transmission to proceed. SPI transmit will halt
 *       when the receive data FIFO is full. In slave mode, an overrun error will occur if received data is not
 *       read before new data is received.
 *  0b1..Ignore received data. Received data is ignored, allowing transmission without reading unneeded received
 *       data. No receiver flags are generated.
 */
#define SPI_FIFOWR_RXIGNORE(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_RXIGNORE_SHIFT)) & SPI_FIFOWR_RXIGNORE_MASK)

#define SPI_FIFOWR_TXIGNORE_MASK                 (0x800000U)
#define SPI_FIFOWR_TXIGNORE_SHIFT                (23U)
/*! TXIGNORE - Transmit Ignore
 *  0b0..Write transmit data
 *  0b1..Ignore transmit data
 */
#define SPI_FIFOWR_TXIGNORE(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_TXIGNORE_SHIFT)) & SPI_FIFOWR_TXIGNORE_MASK)

#define SPI_FIFOWR_LEN_MASK                      (0xF000000U)
#define SPI_FIFOWR_LEN_SHIFT                     (24U)
/*! LEN - Data Length
 *  0b0000..Reserved
 *  0b0001..Reserved
 *  0b0010..Reserved
 *  0b0011..Data transfer is 4 bits in length
 *  0b0100..Data transfer is 5 bits in length
 *  0b1111..Data transfer is 16 bits in length
 */
#define SPI_FIFOWR_LEN(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_FIFOWR_LEN_SHIFT)) & SPI_FIFOWR_LEN_MASK)
/*! @} */

/*! @name FIFORD - FIFO Read Data Register */
/*! @{ */

#define SPI_FIFORD_RXDATA_MASK                   (0xFFFFU)
#define SPI_FIFORD_RXDATA_SHIFT                  (0U)
/*! RXDATA - Received Data from the FIFO */
#define SPI_FIFORD_RXDATA(x)                     (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_RXDATA_SHIFT)) & SPI_FIFORD_RXDATA_MASK)

#define SPI_FIFORD_RXSSEL0_N_MASK                (0x10000U)
#define SPI_FIFORD_RXSSEL0_N_SHIFT               (16U)
/*! RXSSEL0_N - Slave Select 0 for Receive
 *  0b0..Slave Select 0 is active
 *  0b1..Slave Select 0 is not active
 */
#define SPI_FIFORD_RXSSEL0_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_RXSSEL0_N_SHIFT)) & SPI_FIFORD_RXSSEL0_N_MASK)

#define SPI_FIFORD_RXSSEL1_N_MASK                (0x20000U)
#define SPI_FIFORD_RXSSEL1_N_SHIFT               (17U)
/*! RXSSEL1_N - Slave Select 1 for Receive
 *  0b0..Slave Select 1 is active
 *  0b1..Slave Select 1 is not active
 */
#define SPI_FIFORD_RXSSEL1_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_RXSSEL1_N_SHIFT)) & SPI_FIFORD_RXSSEL1_N_MASK)

#define SPI_FIFORD_RXSSEL2_N_MASK                (0x40000U)
#define SPI_FIFORD_RXSSEL2_N_SHIFT               (18U)
/*! RXSSEL2_N - Slave Select 2 for Receive
 *  0b0..Slave Select 2 is active
 *  0b1..Slave Select 2 is not active
 */
#define SPI_FIFORD_RXSSEL2_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_RXSSEL2_N_SHIFT)) & SPI_FIFORD_RXSSEL2_N_MASK)

#define SPI_FIFORD_RXSSEL3_N_MASK                (0x80000U)
#define SPI_FIFORD_RXSSEL3_N_SHIFT               (19U)
/*! RXSSEL3_N - Slave Select 3 for Receive
 *  0b0..Slave Select 3 is active
 *  0b1..Slave Select 3 is not active
 */
#define SPI_FIFORD_RXSSEL3_N(x)                  (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_RXSSEL3_N_SHIFT)) & SPI_FIFORD_RXSSEL3_N_MASK)

#define SPI_FIFORD_SOT_MASK                      (0x100000U)
#define SPI_FIFORD_SOT_SHIFT                     (20U)
/*! SOT - Start of Transfer Flag
 *  0b0..This is not the 1st data after the SSELs went from deasserted to asserted
 *  0b1..This is the 1st data after the SSELs went from deasserted to asserted (i.e., any previous transfer has
 *       ended). This information can be used to identify the 1st piece of data in cases where the transfer length is
 *       greater than 16 bits.
 */
#define SPI_FIFORD_SOT(x)                        (((uint32_t)(((uint32_t)(x)) << SPI_FIFORD_SOT_SHIFT)) & SPI_FIFORD_SOT_MASK)
/*! @} */

/*! @name FIFORDNOPOP - FIFO Data Read with no FIFO Pop Register */
/*! @{ */

#define SPI_FIFORDNOPOP_RXDATA_MASK              (0xFFFFU)
#define SPI_FIFORDNOPOP_RXDATA_SHIFT             (0U)
/*! RXDATA - Received Data from the FIFO */
#define SPI_FIFORDNOPOP_RXDATA(x)                (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_RXDATA_SHIFT)) & SPI_FIFORDNOPOP_RXDATA_MASK)

#define SPI_FIFORDNOPOP_RXSSEL0_N_MASK           (0x10000U)
#define SPI_FIFORDNOPOP_RXSSEL0_N_SHIFT          (16U)
/*! RXSSEL0_N - Slave Select 0 for Receive
 *  0b0..Not selected
 *  0b1..Selected
 */
#define SPI_FIFORDNOPOP_RXSSEL0_N(x)             (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_RXSSEL0_N_SHIFT)) & SPI_FIFORDNOPOP_RXSSEL0_N_MASK)

#define SPI_FIFORDNOPOP_RXSSEL1_N_MASK           (0x20000U)
#define SPI_FIFORDNOPOP_RXSSEL1_N_SHIFT          (17U)
/*! RXSSEL1_N - Slave Select 1 for Receive
 *  0b0..Not selected
 *  0b1..Selected
 */
#define SPI_FIFORDNOPOP_RXSSEL1_N(x)             (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_RXSSEL1_N_SHIFT)) & SPI_FIFORDNOPOP_RXSSEL1_N_MASK)

#define SPI_FIFORDNOPOP_RXSSEL2_N_MASK           (0x40000U)
#define SPI_FIFORDNOPOP_RXSSEL2_N_SHIFT          (18U)
/*! RXSSEL2_N - Slave Select 2 for Receive
 *  0b0..Not selected
 *  0b1..Selected
 */
#define SPI_FIFORDNOPOP_RXSSEL2_N(x)             (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_RXSSEL2_N_SHIFT)) & SPI_FIFORDNOPOP_RXSSEL2_N_MASK)

#define SPI_FIFORDNOPOP_RXSSEL3_N_MASK           (0x80000U)
#define SPI_FIFORDNOPOP_RXSSEL3_N_SHIFT          (19U)
/*! RXSSEL3_N - Slave Select 3 for Receive
 *  0b0..Not selected
 *  0b1..Selected
 */
#define SPI_FIFORDNOPOP_RXSSEL3_N(x)             (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_RXSSEL3_N_SHIFT)) & SPI_FIFORDNOPOP_RXSSEL3_N_MASK)

#define SPI_FIFORDNOPOP_SOT_MASK                 (0x100000U)
#define SPI_FIFORDNOPOP_SOT_SHIFT                (20U)
/*! SOT - Start of Transfer Flag
 *  0b0..Not active
 *  0b1..Active
 */
#define SPI_FIFORDNOPOP_SOT(x)                   (((uint32_t)(((uint32_t)(x)) << SPI_FIFORDNOPOP_SOT_SHIFT)) & SPI_FIFORDNOPOP_SOT_MASK)
/*! @} */

/*! @name FIFOSIZE - FIFO Size Register */
/*! @{ */

#define SPI_FIFOSIZE_FIFOSIZE_MASK               (0x1FU)
#define SPI_FIFOSIZE_FIFOSIZE_SHIFT              (0U)
/*! FIFOSIZE - FIFO Size */
#define SPI_FIFOSIZE_FIFOSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << SPI_FIFOSIZE_FIFOSIZE_SHIFT)) & SPI_FIFOSIZE_FIFOSIZE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCFG - FIFO Receive Timeout Configuration */
/*! @{ */

#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK (0xFFU)
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT (0U)
/*! RXTIMEOUT_PRESCALER - Receive Timeout Counter Clock Prescaler */
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER(x) (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT)) & SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK)

#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK (0xFFFF00U)
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT (8U)
/*! RXTIMEOUT_VALUE - Receive Timeout Value */
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE(x)  (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT)) & SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK)

#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK   (0x1000000U)
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT  (24U)
/*! RXTIMEOUT_EN - Receive Timeout Enable
 *  0b0..Disable RX FIFO timeout
 *  0b1..Enable RX FIFO timeout
 */
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_EN(x)     (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT)) & SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK)

#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK  (0x2000000U)
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT (25U)
/*! RXTIMEOUT_COW - Receive Timeout Continue On Write
 *  0b0..RX FIFO timeout counter is reset every time data is transferred from the peripheral into the RX FIFO.
 *  0b1..RX FIFO timeout counter is not reset every time data is transferred from the peripheral into the RX FIFO.
 */
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COW(x)    (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT)) & SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK)

#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK  (0x4000000U)
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT (26U)
/*! RXTIMEOUT_COE - Receive Timeout Continue On Empty
 *  0b0..RX FIFO timeout counter is reset when the RX FIFO becomes empty.
 *  0b1..RX FIFO timeout counter is not reset when the RX FIFO becomes empty.
 */
#define SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COE(x)    (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT)) & SPI_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCNT - FIFO Receive Timeout Counter */
/*! @{ */

#define SPI_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK  (0xFFFFU)
#define SPI_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT (0U)
/*! RXTIMEOUT_CNT - Current RX FIFO timeout counter value */
#define SPI_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT(x)    (((uint32_t)(((uint32_t)(x)) << SPI_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT)) & SPI_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK)
/*! @} */

/*! @name ID - Peripheral Identification Register */
/*! @{ */

#define SPI_ID_APERTURE_MASK                     (0xFFU)
#define SPI_ID_APERTURE_SHIFT                    (0U)
/*! APERTURE - Aperture */
#define SPI_ID_APERTURE(x)                       (((uint32_t)(((uint32_t)(x)) << SPI_ID_APERTURE_SHIFT)) & SPI_ID_APERTURE_MASK)

#define SPI_ID_MINOR_REV_MASK                    (0xF00U)
#define SPI_ID_MINOR_REV_SHIFT                   (8U)
/*! MINOR_REV - Minor revision of module implementation */
#define SPI_ID_MINOR_REV(x)                      (((uint32_t)(((uint32_t)(x)) << SPI_ID_MINOR_REV_SHIFT)) & SPI_ID_MINOR_REV_MASK)

#define SPI_ID_MAJOR_REV_MASK                    (0xF000U)
#define SPI_ID_MAJOR_REV_SHIFT                   (12U)
/*! MAJOR_REV - Major revision of module implementation */
#define SPI_ID_MAJOR_REV(x)                      (((uint32_t)(((uint32_t)(x)) << SPI_ID_MAJOR_REV_SHIFT)) & SPI_ID_MAJOR_REV_MASK)

#define SPI_ID_ID_MASK                           (0xFFFF0000U)
#define SPI_ID_ID_SHIFT                          (16U)
/*! ID - Module identifier for the selected function */
#define SPI_ID_ID(x)                             (((uint32_t)(((uint32_t)(x)) << SPI_ID_ID_SHIFT)) & SPI_ID_ID_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SPI_Register_Masks */


/*!
 * @}
 */ /* end of group SPI_Peripheral_Access_Layer */


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


#endif  /* SPI_H_ */

