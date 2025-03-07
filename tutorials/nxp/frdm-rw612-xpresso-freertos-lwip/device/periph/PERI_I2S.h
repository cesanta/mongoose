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
**         CMSIS Peripheral Access Layer for I2S
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
 * @file I2S.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for I2S
 *
 * CMSIS Peripheral Access Layer for I2S
 */

#if !defined(I2S_H_)
#define I2S_H_                                   /**< Symbol preventing repeated inclusion */

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
   -- I2S Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2S_Peripheral_Access_Layer I2S Peripheral Access Layer
 * @{
 */

/** I2S - Size of Registers Arrays */
#define I2S_SECCHANNEL_COUNT                      3u

/** I2S - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[3072];
  __IO uint32_t CFG1;                              /**< Configuration Register 1 for the Primary Channel Pair, offset: 0xC00 */
  __IO uint32_t CFG2;                              /**< Configuration Register 2 for the Primary Channel Pair, offset: 0xC04 */
  __IO uint32_t STAT;                              /**< Status Register for the Primary Channel Pair, offset: 0xC08 */
       uint8_t RESERVED_1[16];
  __IO uint32_t DIV;                               /**< Clock Divider, offset: 0xC1C */
  struct {                                         /* offset: 0xC20, array step: 0x20 */
    __IO uint32_t PCFG1;                             /**< Configuration Register 1 for Channel Pair 1..Configuration Register 1 for Channel Pair 3, array offset: 0xC20, array step: 0x20 */
    __IO uint32_t PCFG2;                             /**< Configuration Register 2 for Channel Pair 1..Configuration Register 2 for Channel Pair 3, array offset: 0xC24, array step: 0x20 */
    __I  uint32_t PSTAT;                             /**< Status Register for Channel Pair 1..Status Register for Channel Pair 3, array offset: 0xC28, array step: 0x20 */
         uint8_t RESERVED_0[20];
  } SECCHANNEL[I2S_SECCHANNEL_COUNT];
       uint8_t RESERVED_2[384];
  __IO uint32_t FIFOCFG;                           /**< FIFO Configuration and Enable, offset: 0xE00 */
  __IO uint32_t FIFOSTAT;                          /**< FIFO Status, offset: 0xE04 */
  __IO uint32_t FIFOTRIG;                          /**< FIFO Trigger Settings, offset: 0xE08 */
       uint8_t RESERVED_3[4];
  __IO uint32_t FIFOINTENSET;                      /**< FIFO Interrupt Enable Set and Read, offset: 0xE10 */
  __IO uint32_t FIFOINTENCLR;                      /**< FIFO Interrupt Enable Clear and Read, offset: 0xE14 */
  __I  uint32_t FIFOINTSTAT;                       /**< FIFO Interrupt Status, offset: 0xE18 */
       uint8_t RESERVED_4[4];
  __O  uint32_t FIFOWR;                            /**< FIFO Write Data, offset: 0xE20 */
  __O  uint32_t FIFOWR48H;                         /**< FIFO Write Data for Upper Data Bits, offset: 0xE24 */
       uint8_t RESERVED_5[8];
  __I  uint32_t FIFORD;                            /**< FIFO Read Data, offset: 0xE30 */
  __I  uint32_t FIFORD48H;                         /**< FIFO Read Data for Upper Data Bits, offset: 0xE34 */
       uint8_t RESERVED_6[8];
  __I  uint32_t FIFORDNOPOP;                       /**< FIFO Data Read with No FIFO Pop, offset: 0xE40 */
  __I  uint32_t FIFORD48HNOPOP;                    /**< FIFO Data Read for Upper Data Bits with No FIFO Pop, offset: 0xE44 */
  __I  uint32_t FIFOSIZE;                          /**< FIFO Size Register, offset: 0xE48 */
  __IO uint32_t FIFORXTIMEOUTCFG;                  /**< FIFO Receive Timeout Configuration, offset: 0xE4C */
  __I  uint32_t FIFORXTIMEOUTCNT;                  /**< FIFO Receive Timeout Counter, offset: 0xE50 */
       uint8_t RESERVED_7[424];
  __I  uint32_t ID;                                /**< I2S Module Identification, offset: 0xFFC */
} I2S_Type;

/* ----------------------------------------------------------------------------
   -- I2S Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2S_Register_Masks I2S Register Masks
 * @{
 */

/*! @name CFG1 - Configuration Register 1 for the Primary Channel Pair */
/*! @{ */

#define I2S_CFG1_MAINENABLE_MASK                 (0x1U)
#define I2S_CFG1_MAINENABLE_SHIFT                (0U)
/*! MAINENABLE - Main Enable
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_CFG1_MAINENABLE(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_MAINENABLE_SHIFT)) & I2S_CFG1_MAINENABLE_MASK)

#define I2S_CFG1_DATAPAUSE_MASK                  (0x2U)
#define I2S_CFG1_DATAPAUSE_SHIFT                 (1U)
/*! DATAPAUSE - Data Flow Pause
 *  0b0..Normal operation
 *  0b1..Pause
 */
#define I2S_CFG1_DATAPAUSE(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_DATAPAUSE_SHIFT)) & I2S_CFG1_DATAPAUSE_MASK)

#define I2S_CFG1_PAIRCOUNT_MASK                  (0xCU)
#define I2S_CFG1_PAIRCOUNT_SHIFT                 (2U)
/*! PAIRCOUNT - Pair Count
 *  0b00..One Pair
 *  0b01..Two Pairs
 *  0b10..Three Pairs
 *  0b11..Four Pairs
 */
#define I2S_CFG1_PAIRCOUNT(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_PAIRCOUNT_SHIFT)) & I2S_CFG1_PAIRCOUNT_MASK)

#define I2S_CFG1_MSTSLVCFG_MASK                  (0x30U)
#define I2S_CFG1_MSTSLVCFG_SHIFT                 (4U)
/*! MSTSLVCFG - Master/Slave Configuration Selection
 *  0b00..Normal Slave Mode
 *  0b01..WS Synchronized Master Mode
 *  0b10..Master Using an Existing SCK Mode
 *  0b11..Normal Master Mode
 */
#define I2S_CFG1_MSTSLVCFG(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_MSTSLVCFG_SHIFT)) & I2S_CFG1_MSTSLVCFG_MASK)

#define I2S_CFG1_MODE_MASK                       (0xC0U)
#define I2S_CFG1_MODE_SHIFT                      (6U)
/*! MODE - Mode
 *  0b00..Classic Mode
 *  0b01..DSP mode WS 50% duty cycle
 *  0b10..DSP mode WS 1 clock
 *  0b11..DSP mode WS 1 data
 */
#define I2S_CFG1_MODE(x)                         (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_MODE_SHIFT)) & I2S_CFG1_MODE_MASK)

#define I2S_CFG1_RIGHTLOW_MASK                   (0x100U)
#define I2S_CFG1_RIGHTLOW_SHIFT                  (8U)
/*! RIGHTLOW - Right Channel Low
 *  0b0..Right high
 *  0b1..Right low
 */
#define I2S_CFG1_RIGHTLOW(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_RIGHTLOW_SHIFT)) & I2S_CFG1_RIGHTLOW_MASK)

#define I2S_CFG1_LEFTJUST_MASK                   (0x200U)
#define I2S_CFG1_LEFTJUST_SHIFT                  (9U)
/*! LEFTJUST - Left-Justify Data
 *  0b0..Right-justified
 *  0b1..Left-justified
 */
#define I2S_CFG1_LEFTJUST(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_LEFTJUST_SHIFT)) & I2S_CFG1_LEFTJUST_MASK)

#define I2S_CFG1_ONECHANNEL_MASK                 (0x400U)
#define I2S_CFG1_ONECHANNEL_SHIFT                (10U)
/*! ONECHANNEL - Single Channel Mode
 *  0b0..Dual channel
 *  0b1..Single channel
 */
#define I2S_CFG1_ONECHANNEL(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_ONECHANNEL_SHIFT)) & I2S_CFG1_ONECHANNEL_MASK)

#define I2S_CFG1_PDMDATA_MASK                    (0x800U)
#define I2S_CFG1_PDMDATA_SHIFT                   (11U)
/*! PDMDATA - PDM Data Selection
 *  0b0..Normal Operation
 *  0b1..DMIC subsystem
 */
#define I2S_CFG1_PDMDATA(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_PDMDATA_SHIFT)) & I2S_CFG1_PDMDATA_MASK)

#define I2S_CFG1_SCK_POL_MASK                    (0x1000U)
#define I2S_CFG1_SCK_POL_SHIFT                   (12U)
/*! SCK_POL - SCK Polarity
 *  0b0..Falling edge
 *  0b1..Rising edge
 */
#define I2S_CFG1_SCK_POL(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_SCK_POL_SHIFT)) & I2S_CFG1_SCK_POL_MASK)

#define I2S_CFG1_WS_POL_MASK                     (0x2000U)
#define I2S_CFG1_WS_POL_SHIFT                    (13U)
/*! WS_POL - WS Polarity
 *  0b0..Not inverted
 *  0b1..Inverted. The WS signal is inverted.
 */
#define I2S_CFG1_WS_POL(x)                       (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_WS_POL_SHIFT)) & I2S_CFG1_WS_POL_MASK)

#define I2S_CFG1_DATALEN_MASK                    (0x1F0000U)
#define I2S_CFG1_DATALEN_SHIFT                   (16U)
/*! DATALEN - Data Length
 *  0b00011..Data is 4 bits in length.
 *  0b00100..Data is 5 bits in length.
 *  0b00111..Data is 8 bits in length.
 *  0b11110..Data is 31 bits in length.
 *  0b11111..Data is 32 bits in length.
 */
#define I2S_CFG1_DATALEN(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_CFG1_DATALEN_SHIFT)) & I2S_CFG1_DATALEN_MASK)
/*! @} */

/*! @name CFG2 - Configuration Register 2 for the Primary Channel Pair */
/*! @{ */

#define I2S_CFG2_FRAMELEN_MASK                   (0x7FFU)
#define I2S_CFG2_FRAMELEN_SHIFT                  (0U)
/*! FRAMELEN - Frame Length
 *  0b00000000011..Frame is 4 bits in total length
 *  0b00000000100..Frame is 5 bits in total length
 *  0b00111111111..Frame is 512 bits in total length
 *  0b11111111111..Frame is 2048 bits in total length
 */
#define I2S_CFG2_FRAMELEN(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_CFG2_FRAMELEN_SHIFT)) & I2S_CFG2_FRAMELEN_MASK)

#define I2S_CFG2_POSITION_MASK                   (0x7FF0000U)
#define I2S_CFG2_POSITION_SHIFT                  (16U)
/*! POSITION - Data Position
 *  0b00000000000..Data begins at bit position 0 (the first bit position) within the frame or WS phase
 *  0b00000000001..Data begins at bit position 1 within the frame or WS phase
 *  0b00000000010..Data begins at bit position 2 within the frame or WS phase
 */
#define I2S_CFG2_POSITION(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_CFG2_POSITION_SHIFT)) & I2S_CFG2_POSITION_MASK)
/*! @} */

/*! @name STAT - Status Register for the Primary Channel Pair */
/*! @{ */

#define I2S_STAT_BUSY_MASK                       (0x1U)
#define I2S_STAT_BUSY_SHIFT                      (0U)
/*! BUSY - Busy Status
 *  0b0..Idle
 *  0b1..Busy
 */
#define I2S_STAT_BUSY(x)                         (((uint32_t)(((uint32_t)(x)) << I2S_STAT_BUSY_SHIFT)) & I2S_STAT_BUSY_MASK)

#define I2S_STAT_SLVFRMERR_MASK                  (0x2U)
#define I2S_STAT_SLVFRMERR_SHIFT                 (1U)
/*! SLVFRMERR - Slave Frame Error
 *  0b0..No error
 *  0b1..Error
 */
#define I2S_STAT_SLVFRMERR(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_STAT_SLVFRMERR_SHIFT)) & I2S_STAT_SLVFRMERR_MASK)

#define I2S_STAT_LR_MASK                         (0x4U)
#define I2S_STAT_LR_SHIFT                        (2U)
/*! LR - Left/Right Indication
 *  0b0..Left channel
 *  0b1..Right channel
 */
#define I2S_STAT_LR(x)                           (((uint32_t)(((uint32_t)(x)) << I2S_STAT_LR_SHIFT)) & I2S_STAT_LR_MASK)

#define I2S_STAT_DATAPAUSED_MASK                 (0x8U)
#define I2S_STAT_DATAPAUSED_SHIFT                (3U)
/*! DATAPAUSED - Data Paused
 *  0b0..Not Paused
 *  0b1..Paused
 */
#define I2S_STAT_DATAPAUSED(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_STAT_DATAPAUSED_SHIFT)) & I2S_STAT_DATAPAUSED_MASK)
/*! @} */

/*! @name DIV - Clock Divider */
/*! @{ */

#define I2S_DIV_DIV_MASK                         (0xFFFU)
#define I2S_DIV_DIV_SHIFT                        (0U)
/*! DIV - Divider
 *  0b000000000000..FCLK is used directly.
 *  0b000000000001..FCLK is divided by 2.
 *  0b000000000010..FCLK is divided by 3.
 *  0b111111111111..FCLK is divided by 4,096.
 */
#define I2S_DIV_DIV(x)                           (((uint32_t)(((uint32_t)(x)) << I2S_DIV_DIV_SHIFT)) & I2S_DIV_DIV_MASK)
/*! @} */

/*! @name SECCHANNEL_PCFG1 - Configuration Register 1 for Channel Pair 1..Configuration Register 1 for Channel Pair 3 */
/*! @{ */

#define I2S_SECCHANNEL_PCFG1_PAIRENABLE_MASK     (0x1U)
#define I2S_SECCHANNEL_PCFG1_PAIRENABLE_SHIFT    (0U)
/*! PAIRENABLE - Pair Enable
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_SECCHANNEL_PCFG1_PAIRENABLE(x)       (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PCFG1_PAIRENABLE_SHIFT)) & I2S_SECCHANNEL_PCFG1_PAIRENABLE_MASK)

#define I2S_SECCHANNEL_PCFG1_ONECHANNEL_MASK     (0x400U)
#define I2S_SECCHANNEL_PCFG1_ONECHANNEL_SHIFT    (10U)
/*! ONECHANNEL - Single Channel Mode
 *  0b0..Dual Channel
 *  0b1..Single Channel
 */
#define I2S_SECCHANNEL_PCFG1_ONECHANNEL(x)       (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PCFG1_ONECHANNEL_SHIFT)) & I2S_SECCHANNEL_PCFG1_ONECHANNEL_MASK)
/*! @} */

/* The count of I2S_SECCHANNEL_PCFG1 */
#define I2S_SECCHANNEL_PCFG1_COUNT               (3U)

/*! @name SECCHANNEL_PCFG2 - Configuration Register 2 for Channel Pair 1..Configuration Register 2 for Channel Pair 3 */
/*! @{ */

#define I2S_SECCHANNEL_PCFG2_POSITION_MASK       (0x1FF0000U)
#define I2S_SECCHANNEL_PCFG2_POSITION_SHIFT      (16U)
/*! POSITION - Data Position */
#define I2S_SECCHANNEL_PCFG2_POSITION(x)         (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PCFG2_POSITION_SHIFT)) & I2S_SECCHANNEL_PCFG2_POSITION_MASK)
/*! @} */

/* The count of I2S_SECCHANNEL_PCFG2 */
#define I2S_SECCHANNEL_PCFG2_COUNT               (3U)

/*! @name SECCHANNEL_PSTAT - Status Register for Channel Pair 1..Status Register for Channel Pair 3 */
/*! @{ */

#define I2S_SECCHANNEL_PSTAT_BUSY_MASK           (0x1U)
#define I2S_SECCHANNEL_PSTAT_BUSY_SHIFT          (0U)
/*! BUSY - Busy Status for Channel Pair
 *  0b0..Idle. The transmitter/receiver for this channel pair is currently idle.
 *  0b1..Busy. The transmitter/receiver for this channel pair is currently processing data.
 */
#define I2S_SECCHANNEL_PSTAT_BUSY(x)             (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PSTAT_BUSY_SHIFT)) & I2S_SECCHANNEL_PSTAT_BUSY_MASK)

#define I2S_SECCHANNEL_PSTAT_SLVFRMERR_MASK      (0x2U)
#define I2S_SECCHANNEL_PSTAT_SLVFRMERR_SHIFT     (1U)
/*! SLVFRMERR - Save Frame Error Flag
 *  0b0..No Error
 *  0b1..Error
 */
#define I2S_SECCHANNEL_PSTAT_SLVFRMERR(x)        (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PSTAT_SLVFRMERR_SHIFT)) & I2S_SECCHANNEL_PSTAT_SLVFRMERR_MASK)

#define I2S_SECCHANNEL_PSTAT_LR_MASK             (0x4U)
#define I2S_SECCHANNEL_PSTAT_LR_SHIFT            (2U)
/*! LR - Left/Right Indication
 *  0b0..Left channel
 *  0b1..Right channel
 */
#define I2S_SECCHANNEL_PSTAT_LR(x)               (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PSTAT_LR_SHIFT)) & I2S_SECCHANNEL_PSTAT_LR_MASK)

#define I2S_SECCHANNEL_PSTAT_DATAPAUSED_MASK     (0x8U)
#define I2S_SECCHANNEL_PSTAT_DATAPAUSED_SHIFT    (3U)
/*! DATAPAUSED - Data Paused Status Flag
 *  0b0..Data Not Paused. Data is not currently paused. A data pause may have been requested but is not yet in
 *       force, waiting for an allowed pause point. Refer to the description in CFG1[DATAPAUSE].
 *  0b1..Data Paused. A data pause has been requested and is now in force.
 */
#define I2S_SECCHANNEL_PSTAT_DATAPAUSED(x)       (((uint32_t)(((uint32_t)(x)) << I2S_SECCHANNEL_PSTAT_DATAPAUSED_SHIFT)) & I2S_SECCHANNEL_PSTAT_DATAPAUSED_MASK)
/*! @} */

/* The count of I2S_SECCHANNEL_PSTAT */
#define I2S_SECCHANNEL_PSTAT_COUNT               (3U)

/*! @name FIFOCFG - FIFO Configuration and Enable */
/*! @{ */

#define I2S_FIFOCFG_ENABLETX_MASK                (0x1U)
#define I2S_FIFOCFG_ENABLETX_SHIFT               (0U)
/*! ENABLETX - Enable Transmit FIFO
 *  0b0..Disabled Transmit. The transmit FIFO is not enabled.
 *  0b1..Enabled transmit. The transmit FIFO is enabled.
 */
#define I2S_FIFOCFG_ENABLETX(x)                  (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_ENABLETX_SHIFT)) & I2S_FIFOCFG_ENABLETX_MASK)

#define I2S_FIFOCFG_ENABLERX_MASK                (0x2U)
#define I2S_FIFOCFG_ENABLERX_SHIFT               (1U)
/*! ENABLERX - Enable Receive FIFO
 *  0b0..Disabled. The receive FIFO is not enabled.
 *  0b1..Enabled. The receive FIFO is enabled.
 */
#define I2S_FIFOCFG_ENABLERX(x)                  (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_ENABLERX_SHIFT)) & I2S_FIFOCFG_ENABLERX_MASK)

#define I2S_FIFOCFG_TXI2SE0_MASK                 (0x4U)
#define I2S_FIFOCFG_TXI2SE0_SHIFT                (2U)
/*! TXI2SE0 - Transmit I2S Empty 0
 *  0b0..Last value
 *  0b1..Zero
 */
#define I2S_FIFOCFG_TXI2SE0(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_TXI2SE0_SHIFT)) & I2S_FIFOCFG_TXI2SE0_MASK)

#define I2S_FIFOCFG_PACK48_MASK                  (0x8U)
#define I2S_FIFOCFG_PACK48_SHIFT                 (3U)
/*! PACK48 - Packing Format 48-bit data
 *  0b0..Bits_24
 *  0b1..Bits_32_16
 */
#define I2S_FIFOCFG_PACK48(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_PACK48_SHIFT)) & I2S_FIFOCFG_PACK48_MASK)

#define I2S_FIFOCFG_SIZE_MASK                    (0x30U)
#define I2S_FIFOCFG_SIZE_SHIFT                   (4U)
/*! SIZE - FIFO Size Configuration
 *  0b10..Size 32 Bits
 *  0b11..Size 48 Bits
 */
#define I2S_FIFOCFG_SIZE(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_SIZE_SHIFT)) & I2S_FIFOCFG_SIZE_MASK)

#define I2S_FIFOCFG_DMATX_MASK                   (0x1000U)
#define I2S_FIFOCFG_DMATX_SHIFT                  (12U)
/*! DMATX - DMA Transmit
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOCFG_DMATX(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_DMATX_SHIFT)) & I2S_FIFOCFG_DMATX_MASK)

#define I2S_FIFOCFG_DMARX_MASK                   (0x2000U)
#define I2S_FIFOCFG_DMARX_SHIFT                  (13U)
/*! DMARX - DMA Receive
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOCFG_DMARX(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_DMARX_SHIFT)) & I2S_FIFOCFG_DMARX_MASK)

#define I2S_FIFOCFG_WAKETX_MASK                  (0x4000U)
#define I2S_FIFOCFG_WAKETX_SHIFT                 (14U)
/*! WAKETX - Wake-up for Transmit FIFO Level
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOCFG_WAKETX(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_WAKETX_SHIFT)) & I2S_FIFOCFG_WAKETX_MASK)

#define I2S_FIFOCFG_WAKERX_MASK                  (0x8000U)
#define I2S_FIFOCFG_WAKERX_SHIFT                 (15U)
/*! WAKERX - Wake-up for Receive FIFO Level
 *  0b0..Only enabled interrupts wake up the device from reduced power modes.
 *  0b1..A device wake-up for DMA occurs if the receive FIFO level reaches the value specified by FIFOTRIG[RXLVL],
 *       even when the RXLVL interrupt is not enabled.
 */
#define I2S_FIFOCFG_WAKERX(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_WAKERX_SHIFT)) & I2S_FIFOCFG_WAKERX_MASK)

#define I2S_FIFOCFG_EMPTYTX_MASK                 (0x10000U)
#define I2S_FIFOCFG_EMPTYTX_SHIFT                (16U)
/*! EMPTYTX - Empty command for the transmit FIFO. When a 1 is written to this bit, the TX FIFO is emptied. */
#define I2S_FIFOCFG_EMPTYTX(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_EMPTYTX_SHIFT)) & I2S_FIFOCFG_EMPTYTX_MASK)

#define I2S_FIFOCFG_EMPTYRX_MASK                 (0x20000U)
#define I2S_FIFOCFG_EMPTYRX_SHIFT                (17U)
/*! EMPTYRX - Empty command for the receive FIFO. When a 1 is written to this bit, the RX FIFO is emptied. */
#define I2S_FIFOCFG_EMPTYRX(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_EMPTYRX_SHIFT)) & I2S_FIFOCFG_EMPTYRX_MASK)

#define I2S_FIFOCFG_POPDBG_MASK                  (0x40000U)
#define I2S_FIFOCFG_POPDBG_SHIFT                 (18U)
/*! POPDBG - Pop FIFO for Debug Reads
 *  0b0..Debug reads of the FIFO do not pop the FIFO.
 *  0b1..A debug read causes the FIFO to pop.
 */
#define I2S_FIFOCFG_POPDBG(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOCFG_POPDBG_SHIFT)) & I2S_FIFOCFG_POPDBG_MASK)
/*! @} */

/*! @name FIFOSTAT - FIFO Status */
/*! @{ */

#define I2S_FIFOSTAT_TXERR_MASK                  (0x1U)
#define I2S_FIFOSTAT_TXERR_SHIFT                 (0U)
/*! TXERR - TX FIFO Error
 *  0b0..No transmit FIFO error occured
 *  0b1..Transmit FIFO error occured
 */
#define I2S_FIFOSTAT_TXERR(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_TXERR_SHIFT)) & I2S_FIFOSTAT_TXERR_MASK)

#define I2S_FIFOSTAT_RXERR_MASK                  (0x2U)
#define I2S_FIFOSTAT_RXERR_SHIFT                 (1U)
/*! RXERR - RX FIFO Error
 *  0b0..No receive FIFO error occured
 *  0b1..Receive FIFO error occured
 */
#define I2S_FIFOSTAT_RXERR(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_RXERR_SHIFT)) & I2S_FIFOSTAT_RXERR_MASK)

#define I2S_FIFOSTAT_PERINT_MASK                 (0x8U)
#define I2S_FIFOSTAT_PERINT_SHIFT                (3U)
/*! PERINT - Peripheral Interrupt
 *  0b0..No interrupt
 *  0b1..Interrupt
 */
#define I2S_FIFOSTAT_PERINT(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_PERINT_SHIFT)) & I2S_FIFOSTAT_PERINT_MASK)

#define I2S_FIFOSTAT_TXEMPTY_MASK                (0x10U)
#define I2S_FIFOSTAT_TXEMPTY_SHIFT               (4U)
/*! TXEMPTY - Transmit FIFO Empty
 *  0b0..Transmit FIFO is not empty
 *  0b1..Transmit FIFO is empty; however, the peripheral may still be processing the last piece of data.
 */
#define I2S_FIFOSTAT_TXEMPTY(x)                  (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_TXEMPTY_SHIFT)) & I2S_FIFOSTAT_TXEMPTY_MASK)

#define I2S_FIFOSTAT_TXNOTFULL_MASK              (0x20U)
#define I2S_FIFOSTAT_TXNOTFULL_SHIFT             (5U)
/*! TXNOTFULL - Transmit FIFO Not Full
 *  0b0..Transmit FIFO is full, and another write would cause an overflow
 *  0b1..Transmit FIFO is not full, so more data can be written
 */
#define I2S_FIFOSTAT_TXNOTFULL(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_TXNOTFULL_SHIFT)) & I2S_FIFOSTAT_TXNOTFULL_MASK)

#define I2S_FIFOSTAT_RXNOTEMPTY_MASK             (0x40U)
#define I2S_FIFOSTAT_RXNOTEMPTY_SHIFT            (6U)
/*! RXNOTEMPTY - Receive FIFO Not Empty
 *  0b0..Receive FIFO is empty
 *  0b1..Receive FIFO is not empty, so data can be read.
 */
#define I2S_FIFOSTAT_RXNOTEMPTY(x)               (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_RXNOTEMPTY_SHIFT)) & I2S_FIFOSTAT_RXNOTEMPTY_MASK)

#define I2S_FIFOSTAT_RXFULL_MASK                 (0x80U)
#define I2S_FIFOSTAT_RXFULL_SHIFT                (7U)
/*! RXFULL - Receive FIFO Full
 *  0b0..Receive FIFO is not full
 *  0b1..Receive FIFO is full
 */
#define I2S_FIFOSTAT_RXFULL(x)                   (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_RXFULL_SHIFT)) & I2S_FIFOSTAT_RXFULL_MASK)

#define I2S_FIFOSTAT_TXLVL_MASK                  (0x1F00U)
#define I2S_FIFOSTAT_TXLVL_SHIFT                 (8U)
/*! TXLVL - Transmit FIFO Current Level
 *  0b00000..TX FIFO is empty
 */
#define I2S_FIFOSTAT_TXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_TXLVL_SHIFT)) & I2S_FIFOSTAT_TXLVL_MASK)

#define I2S_FIFOSTAT_RXLVL_MASK                  (0x1F0000U)
#define I2S_FIFOSTAT_RXLVL_SHIFT                 (16U)
/*! RXLVL - Receive FIFO Current Level
 *  0b00000..RX FIFO is empty
 */
#define I2S_FIFOSTAT_RXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_RXLVL_SHIFT)) & I2S_FIFOSTAT_RXLVL_MASK)

#define I2S_FIFOSTAT_RXTIMEOUT_MASK              (0x1000000U)
#define I2S_FIFOSTAT_RXTIMEOUT_SHIFT             (24U)
/*! RXTIMEOUT - Receive FIFO Timeout
 *  0b0..RX FIFO on
 *  0b1..RX FIFO has timed out, based on the timeout configuration in the FIFORXTIMEOUTCFG register.
 */
#define I2S_FIFOSTAT_RXTIMEOUT(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSTAT_RXTIMEOUT_SHIFT)) & I2S_FIFOSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOTRIG - FIFO Trigger Settings */
/*! @{ */

#define I2S_FIFOTRIG_TXLVLENA_MASK               (0x1U)
#define I2S_FIFOTRIG_TXLVLENA_SHIFT              (0U)
/*! TXLVLENA - Transmit FIFO Level Trigger Enable
 *  0b0..Transmit FIFO level does not generate a FIFO level trigger.
 *  0b1..An trigger generates if the transmit FIFO level reaches the value specified by the TXLVL field in this register.
 */
#define I2S_FIFOTRIG_TXLVLENA(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOTRIG_TXLVLENA_SHIFT)) & I2S_FIFOTRIG_TXLVLENA_MASK)

#define I2S_FIFOTRIG_RXLVLENA_MASK               (0x2U)
#define I2S_FIFOTRIG_RXLVLENA_SHIFT              (1U)
/*! RXLVLENA - Receive FIFO Level Trigger Enable
 *  0b0..Receive FIFO level does not generate a FIFO level trigger.
 *  0b1..An trigger generates if the receive FIFO level reaches the value specified by the RXLVL.
 */
#define I2S_FIFOTRIG_RXLVLENA(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOTRIG_RXLVLENA_SHIFT)) & I2S_FIFOTRIG_RXLVLENA_MASK)

#define I2S_FIFOTRIG_TXLVL_MASK                  (0xF00U)
#define I2S_FIFOTRIG_TXLVL_SHIFT                 (8U)
/*! TXLVL - Transmit FIFO Level Trigger Point
 *  0b0000..Trigger when the TX FIFO becomes empty.
 *  0b0001..Trigger when the TX FIFO level decreases to one entry.
 *  0b1111..Trigger when the TX FIFO level decreases to 15 entries (is no longer full).
 */
#define I2S_FIFOTRIG_TXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOTRIG_TXLVL_SHIFT)) & I2S_FIFOTRIG_TXLVL_MASK)

#define I2S_FIFOTRIG_RXLVL_MASK                  (0xF0000U)
#define I2S_FIFOTRIG_RXLVL_SHIFT                 (16U)
/*! RXLVL - Receive FIFO Level Trigger Point
 *  0b0000..Trigger when the RX FIFO has received 1 entry (the FIFO is no longer empty).
 *  0b0001..Trigger when the RX FIFO has received 2 entries.
 *  0b1111..Trigger when the RX FIFO has received 16 entries (the FIFO has become full).
 */
#define I2S_FIFOTRIG_RXLVL(x)                    (((uint32_t)(((uint32_t)(x)) << I2S_FIFOTRIG_RXLVL_SHIFT)) & I2S_FIFOTRIG_RXLVL_MASK)
/*! @} */

/*! @name FIFOINTENSET - FIFO Interrupt Enable Set and Read */
/*! @{ */

#define I2S_FIFOINTENSET_TXERR_MASK              (0x1U)
#define I2S_FIFOINTENSET_TXERR_SHIFT             (0U)
/*! TXERR - Transmit Error Interrupt
 *  0b0..Disabled. No interrupt generates for a transmit error.
 *  0b1..Enabled. An interrupt generates when a transmit error occurs.
 */
#define I2S_FIFOINTENSET_TXERR(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENSET_TXERR_SHIFT)) & I2S_FIFOINTENSET_TXERR_MASK)

#define I2S_FIFOINTENSET_RXERR_MASK              (0x2U)
#define I2S_FIFOINTENSET_RXERR_SHIFT             (1U)
/*! RXERR - Receive Error Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOINTENSET_RXERR(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENSET_RXERR_SHIFT)) & I2S_FIFOINTENSET_RXERR_MASK)

#define I2S_FIFOINTENSET_TXLVL_MASK              (0x4U)
#define I2S_FIFOINTENSET_TXLVL_SHIFT             (2U)
/*! TXLVL - Transmit Level Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOINTENSET_TXLVL(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENSET_TXLVL_SHIFT)) & I2S_FIFOINTENSET_TXLVL_MASK)

#define I2S_FIFOINTENSET_RXLVL_MASK              (0x8U)
#define I2S_FIFOINTENSET_RXLVL_SHIFT             (3U)
/*! RXLVL - Receive Level Interrupt
 *  0b0..Disabled
 *  0b1..Enabled
 */
#define I2S_FIFOINTENSET_RXLVL(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENSET_RXLVL_SHIFT)) & I2S_FIFOINTENSET_RXLVL_MASK)

#define I2S_FIFOINTENSET_RXTIMEOUT_MASK          (0x1000000U)
#define I2S_FIFOINTENSET_RXTIMEOUT_SHIFT         (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No RX interrupt will be generated.
 *  0b1..Asserts RX interrupt if RX FIFO Timeout event occurs.
 */
#define I2S_FIFOINTENSET_RXTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENSET_RXTIMEOUT_SHIFT)) & I2S_FIFOINTENSET_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTENCLR - FIFO Interrupt Enable Clear and Read */
/*! @{ */

#define I2S_FIFOINTENCLR_TXERR_MASK              (0x1U)
#define I2S_FIFOINTENCLR_TXERR_SHIFT             (0U)
/*! TXERR - Transmit Error Interrupt Clear
 *  0b0..Interrupt is not cleared.
 *  0b1..Interrupt is cleared.
 */
#define I2S_FIFOINTENCLR_TXERR(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENCLR_TXERR_SHIFT)) & I2S_FIFOINTENCLR_TXERR_MASK)

#define I2S_FIFOINTENCLR_RXERR_MASK              (0x2U)
#define I2S_FIFOINTENCLR_RXERR_SHIFT             (1U)
/*! RXERR - Receive Error Interrupt Clear
 *  0b0..Interrupt is not cleared.
 *  0b1..Interrupt is cleared.
 */
#define I2S_FIFOINTENCLR_RXERR(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENCLR_RXERR_SHIFT)) & I2S_FIFOINTENCLR_RXERR_MASK)

#define I2S_FIFOINTENCLR_TXLVL_MASK              (0x4U)
#define I2S_FIFOINTENCLR_TXLVL_SHIFT             (2U)
/*! TXLVL - Transmit Level Interrupt Clear
 *  0b0..Interrupt is not cleared.
 *  0b1..Interrupt is cleared.
 */
#define I2S_FIFOINTENCLR_TXLVL(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENCLR_TXLVL_SHIFT)) & I2S_FIFOINTENCLR_TXLVL_MASK)

#define I2S_FIFOINTENCLR_RXLVL_MASK              (0x8U)
#define I2S_FIFOINTENCLR_RXLVL_SHIFT             (3U)
/*! RXLVL - Receive Level Interrupt Clear
 *  0b0..Interrupt is not cleared.
 *  0b1..Interrupt is cleared.
 */
#define I2S_FIFOINTENCLR_RXLVL(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENCLR_RXLVL_SHIFT)) & I2S_FIFOINTENCLR_RXLVL_MASK)

#define I2S_FIFOINTENCLR_RXTIMEOUT_MASK          (0x1000000U)
#define I2S_FIFOINTENCLR_RXTIMEOUT_SHIFT         (24U)
/*! RXTIMEOUT - Receive Timeout
 *  0b0..No effect
 *  0b1..Clear the interrupt
 */
#define I2S_FIFOINTENCLR_RXTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTENCLR_RXTIMEOUT_SHIFT)) & I2S_FIFOINTENCLR_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOINTSTAT - FIFO Interrupt Status */
/*! @{ */

#define I2S_FIFOINTSTAT_TXERR_MASK               (0x1U)
#define I2S_FIFOINTSTAT_TXERR_SHIFT              (0U)
/*! TXERR - TX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_TXERR(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_TXERR_SHIFT)) & I2S_FIFOINTSTAT_TXERR_MASK)

#define I2S_FIFOINTSTAT_RXERR_MASK               (0x2U)
#define I2S_FIFOINTSTAT_RXERR_SHIFT              (1U)
/*! RXERR - RX FIFO Error Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_RXERR(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_RXERR_SHIFT)) & I2S_FIFOINTSTAT_RXERR_MASK)

#define I2S_FIFOINTSTAT_TXLVL_MASK               (0x4U)
#define I2S_FIFOINTSTAT_TXLVL_SHIFT              (2U)
/*! TXLVL - Transmit FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_TXLVL(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_TXLVL_SHIFT)) & I2S_FIFOINTSTAT_TXLVL_MASK)

#define I2S_FIFOINTSTAT_RXLVL_MASK               (0x8U)
#define I2S_FIFOINTSTAT_RXLVL_SHIFT              (3U)
/*! RXLVL - Receive FIFO Level Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_RXLVL(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_RXLVL_SHIFT)) & I2S_FIFOINTSTAT_RXLVL_MASK)

#define I2S_FIFOINTSTAT_PERINT_MASK              (0x10U)
#define I2S_FIFOINTSTAT_PERINT_SHIFT             (4U)
/*! PERINT - Peripheral Interrupt Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_PERINT(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_PERINT_SHIFT)) & I2S_FIFOINTSTAT_PERINT_MASK)

#define I2S_FIFOINTSTAT_RXTIMEOUT_MASK           (0x1000000U)
#define I2S_FIFOINTSTAT_RXTIMEOUT_SHIFT          (24U)
/*! RXTIMEOUT - Receive Timeout Status
 *  0b0..Not pending
 *  0b1..Pending
 */
#define I2S_FIFOINTSTAT_RXTIMEOUT(x)             (((uint32_t)(((uint32_t)(x)) << I2S_FIFOINTSTAT_RXTIMEOUT_SHIFT)) & I2S_FIFOINTSTAT_RXTIMEOUT_MASK)
/*! @} */

/*! @name FIFOWR - FIFO Write Data */
/*! @{ */

#define I2S_FIFOWR_TXDATA_MASK                   (0xFFFFFFFFU)
#define I2S_FIFOWR_TXDATA_SHIFT                  (0U)
/*! TXDATA - Transmit Data to the FIFO */
#define I2S_FIFOWR_TXDATA(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_FIFOWR_TXDATA_SHIFT)) & I2S_FIFOWR_TXDATA_MASK)
/*! @} */

/*! @name FIFOWR48H - FIFO Write Data for Upper Data Bits */
/*! @{ */

#define I2S_FIFOWR48H_TXDATA_MASK                (0xFFFFFFU)
#define I2S_FIFOWR48H_TXDATA_SHIFT               (0U)
/*! TXDATA - Transmit Data to the FIFO */
#define I2S_FIFOWR48H_TXDATA(x)                  (((uint32_t)(((uint32_t)(x)) << I2S_FIFOWR48H_TXDATA_SHIFT)) & I2S_FIFOWR48H_TXDATA_MASK)
/*! @} */

/*! @name FIFORD - FIFO Read Data */
/*! @{ */

#define I2S_FIFORD_RXDATA_MASK                   (0xFFFFFFFFU)
#define I2S_FIFORD_RXDATA_SHIFT                  (0U)
/*! RXDATA - Received Data from the FIFO */
#define I2S_FIFORD_RXDATA(x)                     (((uint32_t)(((uint32_t)(x)) << I2S_FIFORD_RXDATA_SHIFT)) & I2S_FIFORD_RXDATA_MASK)
/*! @} */

/*! @name FIFORD48H - FIFO Read Data for Upper Data Bits */
/*! @{ */

#define I2S_FIFORD48H_RXDATA_MASK                (0xFFFFFFU)
#define I2S_FIFORD48H_RXDATA_SHIFT               (0U)
/*! RXDATA - Received Data from the FIFO */
#define I2S_FIFORD48H_RXDATA(x)                  (((uint32_t)(((uint32_t)(x)) << I2S_FIFORD48H_RXDATA_SHIFT)) & I2S_FIFORD48H_RXDATA_MASK)
/*! @} */

/*! @name FIFORDNOPOP - FIFO Data Read with No FIFO Pop */
/*! @{ */

#define I2S_FIFORDNOPOP_RXDATA_MASK              (0xFFFFFFFFU)
#define I2S_FIFORDNOPOP_RXDATA_SHIFT             (0U)
/*! RXDATA - Received Data from the FIFO */
#define I2S_FIFORDNOPOP_RXDATA(x)                (((uint32_t)(((uint32_t)(x)) << I2S_FIFORDNOPOP_RXDATA_SHIFT)) & I2S_FIFORDNOPOP_RXDATA_MASK)
/*! @} */

/*! @name FIFORD48HNOPOP - FIFO Data Read for Upper Data Bits with No FIFO Pop */
/*! @{ */

#define I2S_FIFORD48HNOPOP_RXDATA_MASK           (0xFFFFFFU)
#define I2S_FIFORD48HNOPOP_RXDATA_SHIFT          (0U)
/*! RXDATA - Received Data from the FIFO */
#define I2S_FIFORD48HNOPOP_RXDATA(x)             (((uint32_t)(((uint32_t)(x)) << I2S_FIFORD48HNOPOP_RXDATA_SHIFT)) & I2S_FIFORD48HNOPOP_RXDATA_MASK)
/*! @} */

/*! @name FIFOSIZE - FIFO Size Register */
/*! @{ */

#define I2S_FIFOSIZE_FIFOSIZE_MASK               (0x1FU)
#define I2S_FIFOSIZE_FIFOSIZE_SHIFT              (0U)
#define I2S_FIFOSIZE_FIFOSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << I2S_FIFOSIZE_FIFOSIZE_SHIFT)) & I2S_FIFOSIZE_FIFOSIZE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCFG - FIFO Receive Timeout Configuration */
/*! @{ */

#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK (0xFFU)
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT (0U)
/*! RXTIMEOUT_PRESCALER - Receive Timeout Counter Clock Prescaler */
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER(x) (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_SHIFT)) & I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_PRESCALER_MASK)

#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK (0xFFFF00U)
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT (8U)
/*! RXTIMEOUT_VALUE - Receive Timeout Value */
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE(x)  (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_SHIFT)) & I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_VALUE_MASK)

#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK   (0x1000000U)
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT  (24U)
/*! RXTIMEOUT_EN - Receive Timeout Enable
 *  0b0..Disable RX FIFO timeout
 *  0b1..Enable RX FIFO timeout
 */
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_EN(x)     (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_SHIFT)) & I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_EN_MASK)

#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK  (0x2000000U)
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT (25U)
/*! RXTIMEOUT_COW - Receive Timeout Continue On Write
 *  0b0..RX FIFO timeout counter is reset every time data is transferred from the peripheral into the RX FIFO.
 *  0b1..RX FIFO timeout counter is not reset every time data is transferred from the peripheral into the RX FIFO.
 */
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COW(x)    (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_SHIFT)) & I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COW_MASK)

#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK  (0x4000000U)
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT (26U)
/*! RXTIMEOUT_COE - Receive Timeout Continue On Empty
 *  0b0..RX FIFO timeout counter is reset when the RX FIFO becomes empty.
 *  0b1..RX FIFO timeout counter is not reset when the RX FIFO becomes empty.
 */
#define I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COE(x)    (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_SHIFT)) & I2S_FIFORXTIMEOUTCFG_RXTIMEOUT_COE_MASK)
/*! @} */

/*! @name FIFORXTIMEOUTCNT - FIFO Receive Timeout Counter */
/*! @{ */

#define I2S_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK  (0xFFFFU)
#define I2S_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT (0U)
/*! RXTIMEOUT_CNT - Current RX FIFO timeout counter value */
#define I2S_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT(x)    (((uint32_t)(((uint32_t)(x)) << I2S_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_SHIFT)) & I2S_FIFORXTIMEOUTCNT_RXTIMEOUT_CNT_MASK)
/*! @} */

/*! @name ID - I2S Module Identification */
/*! @{ */

#define I2S_ID_APERTURE_MASK                     (0xFFU)
#define I2S_ID_APERTURE_SHIFT                    (0U)
/*! APERTURE - Aperture */
#define I2S_ID_APERTURE(x)                       (((uint32_t)(((uint32_t)(x)) << I2S_ID_APERTURE_SHIFT)) & I2S_ID_APERTURE_MASK)

#define I2S_ID_MINOR_REV_MASK                    (0xF00U)
#define I2S_ID_MINOR_REV_SHIFT                   (8U)
/*! MINOR_REV - Minor Revision */
#define I2S_ID_MINOR_REV(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_ID_MINOR_REV_SHIFT)) & I2S_ID_MINOR_REV_MASK)

#define I2S_ID_MAJOR_REV_MASK                    (0xF000U)
#define I2S_ID_MAJOR_REV_SHIFT                   (12U)
/*! MAJOR_REV - Major Revision */
#define I2S_ID_MAJOR_REV(x)                      (((uint32_t)(((uint32_t)(x)) << I2S_ID_MAJOR_REV_SHIFT)) & I2S_ID_MAJOR_REV_MASK)

#define I2S_ID_ID_MASK                           (0xFFFF0000U)
#define I2S_ID_ID_SHIFT                          (16U)
/*! ID - Module Identifier */
#define I2S_ID_ID(x)                             (((uint32_t)(((uint32_t)(x)) << I2S_ID_ID_SHIFT)) & I2S_ID_ID_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group I2S_Register_Masks */


/*!
 * @}
 */ /* end of group I2S_Peripheral_Access_Layer */


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


#endif  /* I2S_H_ */

