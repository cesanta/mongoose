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
**     Build:               b240715
**
**     Abstract:
**         CMSIS Peripheral Access Layer for ENET
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
 * @file ENET.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for ENET
 *
 * CMSIS Peripheral Access Layer for ENET
 */

#if !defined(ENET_H_)
#define ENET_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- ENET Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ENET_Peripheral_Access_Layer ENET Peripheral Access Layer
 * @{
 */

/** ENET - Size of Registers Arrays */
#define ENET_TXIC_COUNT                           1u
#define ENET_RXIC_COUNT                           1u
#define ENET_CHANNEL_COUNT                        4u

/** ENET - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[4];
  __IO uint32_t EIR;                               /**< Interrupt Event Register, offset: 0x4 */
  __IO uint32_t EIMR;                              /**< Interrupt Mask Register, offset: 0x8 */
       uint8_t RESERVED_1[4];
  __IO uint32_t RDAR;                              /**< Receive Descriptor Active Register - Ring 0, offset: 0x10 */
  __IO uint32_t TDAR;                              /**< Transmit Descriptor Active Register - Ring 0, offset: 0x14 */
       uint8_t RESERVED_2[12];
  __IO uint32_t ECR;                               /**< Ethernet Control Register, offset: 0x24 */
       uint8_t RESERVED_3[24];
  __IO uint32_t MMFR;                              /**< MII Management Frame Register, offset: 0x40 */
  __IO uint32_t MSCR;                              /**< MII Speed Control Register, offset: 0x44 */
       uint8_t RESERVED_4[28];
  __IO uint32_t MIBC;                              /**< MIB Control Register, offset: 0x64 */
       uint8_t RESERVED_5[28];
  __IO uint32_t RCR;                               /**< Receive Control Register, offset: 0x84 */
       uint8_t RESERVED_6[60];
  __IO uint32_t TCR;                               /**< Transmit Control Register, offset: 0xC4 */
       uint8_t RESERVED_7[28];
  __IO uint32_t PALR;                              /**< Physical Address Lower Register, offset: 0xE4 */
  __IO uint32_t PAUR;                              /**< Physical Address Upper Register, offset: 0xE8 */
  __IO uint32_t OPD;                               /**< Opcode/Pause Duration Register, offset: 0xEC */
  __IO uint32_t TXIC[ENET_TXIC_COUNT];             /**< Transmit Interrupt Coalescing Register, array offset: 0xF0, array step: 0x4 */
       uint8_t RESERVED_8[12];
  __IO uint32_t RXIC[ENET_RXIC_COUNT];             /**< Receive Interrupt Coalescing Register, array offset: 0x100, array step: 0x4 */
       uint8_t RESERVED_9[20];
  __IO uint32_t IAUR;                              /**< Descriptor Individual Upper Address Register, offset: 0x118 */
  __IO uint32_t IALR;                              /**< Descriptor Individual Lower Address Register, offset: 0x11C */
  __IO uint32_t GAUR;                              /**< Descriptor Group Upper Address Register, offset: 0x120 */
  __IO uint32_t GALR;                              /**< Descriptor Group Lower Address Register, offset: 0x124 */
       uint8_t RESERVED_10[28];
  __IO uint32_t TFWR;                              /**< Transmit FIFO Watermark Register, offset: 0x144 */
       uint8_t RESERVED_11[56];
  __IO uint32_t RDSR;                              /**< Receive Descriptor Ring 0 Start Register, offset: 0x180 */
  __IO uint32_t TDSR;                              /**< Transmit Buffer Descriptor Ring 0 Start Register, offset: 0x184 */
  __IO uint32_t MRBR;                              /**< Maximum Receive Buffer Size Register - Ring 0, offset: 0x188 */
       uint8_t RESERVED_12[4];
  __IO uint32_t RSFL;                              /**< Receive FIFO Section Full Threshold, offset: 0x190 */
  __IO uint32_t RSEM;                              /**< Receive FIFO Section Empty Threshold, offset: 0x194 */
  __IO uint32_t RAEM;                              /**< Receive FIFO Almost Empty Threshold, offset: 0x198 */
  __IO uint32_t RAFL;                              /**< Receive FIFO Almost Full Threshold, offset: 0x19C */
  __IO uint32_t TSEM;                              /**< Transmit FIFO Section Empty Threshold, offset: 0x1A0 */
  __IO uint32_t TAEM;                              /**< Transmit FIFO Almost Empty Threshold, offset: 0x1A4 */
  __IO uint32_t TAFL;                              /**< Transmit FIFO Almost Full Threshold, offset: 0x1A8 */
  __IO uint32_t TIPG;                              /**< Transmit Inter-Packet Gap, offset: 0x1AC */
  __IO uint32_t FTRL;                              /**< Frame Truncation Length, offset: 0x1B0 */
       uint8_t RESERVED_13[12];
  __IO uint32_t TACC;                              /**< Transmit Accelerator Function Configuration, offset: 0x1C0 */
  __IO uint32_t RACC;                              /**< Receive Accelerator Function Configuration, offset: 0x1C4 */
       uint8_t RESERVED_14[60];
  __I  uint32_t RMON_T_PACKETS;                    /**< Tx Packet Count Statistic Register, offset: 0x204 */
  __I  uint32_t RMON_T_BC_PKT;                     /**< Tx Broadcast Packets Statistic Register, offset: 0x208 */
  __I  uint32_t RMON_T_MC_PKT;                     /**< Tx Multicast Packets Statistic Register, offset: 0x20C */
  __I  uint32_t RMON_T_CRC_ALIGN;                  /**< Tx Packets with CRC/Align Error Statistic Register, offset: 0x210 */
  __I  uint32_t RMON_T_UNDERSIZE;                  /**< Tx Packets Less Than Bytes and Good CRC Statistic Register, offset: 0x214 */
  __I  uint32_t RMON_T_OVERSIZE;                   /**< Tx Packets GT MAX_FL bytes and Good CRC Statistic Register, offset: 0x218 */
  __I  uint32_t RMON_T_FRAG;                       /**< Tx Packets Less Than 64 Bytes and Bad CRC Statistic Register, offset: 0x21C */
  __I  uint32_t RMON_T_JAB;                        /**< Tx Packets Greater Than MAX_FL bytes and Bad CRC Statistic Register, offset: 0x220 */
  __I  uint32_t RMON_T_COL;                        /**< Tx Collision Count Statistic Register, offset: 0x224 */
  __I  uint32_t RMON_T_P64;                        /**< Tx 64-Byte Packets Statistic Register, offset: 0x228 */
  __I  uint32_t RMON_T_P65TO127;                   /**< Tx 65- to 127-byte Packets Statistic Register, offset: 0x22C */
  __I  uint32_t RMON_T_P128TO255;                  /**< Tx 128- to 255-byte Packets Statistic Register, offset: 0x230 */
  __I  uint32_t RMON_T_P256TO511;                  /**< Tx 256- to 511-byte Packets Statistic Register, offset: 0x234 */
  __I  uint32_t RMON_T_P512TO1023;                 /**< Tx 512- to 1023-byte Packets Statistic Register, offset: 0x238 */
  __I  uint32_t RMON_T_P1024TO2047;                /**< Tx 1024- to 2047-byte Packets Statistic Register, offset: 0x23C */
  __I  uint32_t RMON_T_P_GTE2048;                  /**< Tx Packets Greater Than 2048 Bytes Statistic Register, offset: 0x240 */
  __I  uint32_t RMON_T_OCTETS;                     /**< Tx Octets Statistic Register, offset: 0x244 */
       uint32_t IEEE_T_DROP;                       /**< Reserved Statistic Register, offset: 0x248 */
  __I  uint32_t IEEE_T_FRAME_OK;                   /**< Frames Transmitted OK Statistic Register, offset: 0x24C */
  __I  uint32_t IEEE_T_1COL;                       /**< Frames Transmitted with Single Collision Statistic Register, offset: 0x250 */
  __I  uint32_t IEEE_T_MCOL;                       /**< Frames Transmitted with Multiple Collisions Statistic Register, offset: 0x254 */
  __I  uint32_t IEEE_T_DEF;                        /**< Frames Transmitted after Deferral Delay Statistic Register, offset: 0x258 */
  __I  uint32_t IEEE_T_LCOL;                       /**< Frames Transmitted with Late Collision Statistic Register, offset: 0x25C */
  __I  uint32_t IEEE_T_EXCOL;                      /**< Frames Transmitted with Excessive Collisions Statistic Register, offset: 0x260 */
  __I  uint32_t IEEE_T_MACERR;                     /**< Frames Transmitted with Tx FIFO Underrun Statistic Register, offset: 0x264 */
  __I  uint32_t IEEE_T_CSERR;                      /**< Frames Transmitted with Carrier Sense Error Statistic Register, offset: 0x268 */
  __I  uint32_t IEEE_T_SQE;                        /**< Reserved Statistic Register, offset: 0x26C */
  __I  uint32_t IEEE_T_FDXFC;                      /**< Flow Control Pause Frames Transmitted Statistic Register, offset: 0x270 */
  __I  uint32_t IEEE_T_OCTETS_OK;                  /**< Octet Count for Frames Transmitted w/o Error Statistic Register, offset: 0x274 */
       uint8_t RESERVED_15[12];
  __I  uint32_t RMON_R_PACKETS;                    /**< Rx Packet Count Statistic Register, offset: 0x284 */
  __I  uint32_t RMON_R_BC_PKT;                     /**< Rx Broadcast Packets Statistic Register, offset: 0x288 */
  __I  uint32_t RMON_R_MC_PKT;                     /**< Rx Multicast Packets Statistic Register, offset: 0x28C */
  __I  uint32_t RMON_R_CRC_ALIGN;                  /**< Rx Packets with CRC/Align Error Statistic Register, offset: 0x290 */
  __I  uint32_t RMON_R_UNDERSIZE;                  /**< Rx Packets with Less Than 64 Bytes and Good CRC Statistic Register, offset: 0x294 */
  __I  uint32_t RMON_R_OVERSIZE;                   /**< Rx Packets Greater Than MAX_FL and Good CRC Statistic Register, offset: 0x298 */
  __I  uint32_t RMON_R_FRAG;                       /**< Rx Packets Less Than 64 Bytes and Bad CRC Statistic Register, offset: 0x29C */
  __I  uint32_t RMON_R_JAB;                        /**< Rx Packets Greater Than MAX_FL Bytes and Bad CRC Statistic Register, offset: 0x2A0 */
       uint8_t RESERVED_16[4];
  __I  uint32_t RMON_R_P64;                        /**< Rx 64-Byte Packets Statistic Register, offset: 0x2A8 */
  __I  uint32_t RMON_R_P65TO127;                   /**< Rx 65- to 127-Byte Packets Statistic Register, offset: 0x2AC */
  __I  uint32_t RMON_R_P128TO255;                  /**< Rx 128- to 255-Byte Packets Statistic Register, offset: 0x2B0 */
  __I  uint32_t RMON_R_P256TO511;                  /**< Rx 256- to 511-Byte Packets Statistic Register, offset: 0x2B4 */
  __I  uint32_t RMON_R_P512TO1023;                 /**< Rx 512- to 1023-Byte Packets Statistic Register, offset: 0x2B8 */
  __I  uint32_t RMON_R_P1024TO2047;                /**< Rx 1024- to 2047-Byte Packets Statistic Register, offset: 0x2BC */
  __I  uint32_t RMON_R_P_GTE2048;                  /**< Rx Packets Greater than 2048 Bytes Statistic Register, offset: 0x2C0 */
  __I  uint32_t RMON_R_OCTETS;                     /**< Rx Octets Statistic Register, offset: 0x2C4 */
  __I  uint32_t IEEE_R_DROP;                       /**< Frames not Counted Correctly Statistic Register, offset: 0x2C8 */
  __I  uint32_t IEEE_R_FRAME_OK;                   /**< Frames Received OK Statistic Register, offset: 0x2CC */
  __I  uint32_t IEEE_R_CRC;                        /**< Frames Received with CRC Error Statistic Register, offset: 0x2D0 */
  __I  uint32_t IEEE_R_ALIGN;                      /**< Frames Received with Alignment Error Statistic Register, offset: 0x2D4 */
  __I  uint32_t IEEE_R_MACERR;                     /**< Receive FIFO Overflow Count Statistic Register, offset: 0x2D8 */
  __I  uint32_t IEEE_R_FDXFC;                      /**< Flow Control Pause Frames Received Statistic Register, offset: 0x2DC */
  __I  uint32_t IEEE_R_OCTETS_OK;                  /**< Octet Count for Frames Received without Error Statistic Register, offset: 0x2E0 */
       uint8_t RESERVED_17[284];
  __IO uint32_t ATCR;                              /**< Adjustable Timer Control Register, offset: 0x400 */
  __IO uint32_t ATVR;                              /**< Timer Value Register, offset: 0x404 */
  __IO uint32_t ATOFF;                             /**< Timer Offset Register, offset: 0x408 */
  __IO uint32_t ATPER;                             /**< Timer Period Register, offset: 0x40C */
  __IO uint32_t ATCOR;                             /**< Timer Correction Register, offset: 0x410 */
  __IO uint32_t ATINC;                             /**< Time-Stamping Clock Period Register, offset: 0x414 */
  __I  uint32_t ATSTMP;                            /**< Timestamp of Last Transmitted Frame, offset: 0x418 */
       uint8_t RESERVED_18[488];
  __IO uint32_t TGSR;                              /**< Timer Global Status Register, offset: 0x604 */
  struct {                                         /* offset: 0x608, array step: 0x8 */
    __IO uint32_t TCSR;                              /**< Timer Control Status Register, array offset: 0x608, array step: 0x8 */
    __IO uint32_t TCCR;                              /**< Timer Compare Capture Register, array offset: 0x60C, array step: 0x8 */
  } CHANNEL[ENET_CHANNEL_COUNT];
} ENET_Type;

/* ----------------------------------------------------------------------------
   -- ENET Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ENET_Register_Masks ENET Register Masks
 * @{
 */

/*! @name EIR - Interrupt Event Register */
/*! @{ */

#define ENET_EIR_TS_TIMER_MASK                   (0x8000U)
#define ENET_EIR_TS_TIMER_SHIFT                  (15U)
/*! TS_TIMER - Timestamp Timer */
#define ENET_EIR_TS_TIMER(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_EIR_TS_TIMER_SHIFT)) & ENET_EIR_TS_TIMER_MASK)

#define ENET_EIR_TS_AVAIL_MASK                   (0x10000U)
#define ENET_EIR_TS_AVAIL_SHIFT                  (16U)
/*! TS_AVAIL - Transmit Timestamp Available */
#define ENET_EIR_TS_AVAIL(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_EIR_TS_AVAIL_SHIFT)) & ENET_EIR_TS_AVAIL_MASK)

#define ENET_EIR_WAKEUP_MASK                     (0x20000U)
#define ENET_EIR_WAKEUP_SHIFT                    (17U)
/*! WAKEUP - Node Wakeup Request Indication */
#define ENET_EIR_WAKEUP(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_EIR_WAKEUP_SHIFT)) & ENET_EIR_WAKEUP_MASK)

#define ENET_EIR_PLR_MASK                        (0x40000U)
#define ENET_EIR_PLR_SHIFT                       (18U)
/*! PLR - Payload Receive Error */
#define ENET_EIR_PLR(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_PLR_SHIFT)) & ENET_EIR_PLR_MASK)

#define ENET_EIR_UN_MASK                         (0x80000U)
#define ENET_EIR_UN_SHIFT                        (19U)
/*! UN - Transmit FIFO Underrun */
#define ENET_EIR_UN(x)                           (((uint32_t)(((uint32_t)(x)) << ENET_EIR_UN_SHIFT)) & ENET_EIR_UN_MASK)

#define ENET_EIR_RL_MASK                         (0x100000U)
#define ENET_EIR_RL_SHIFT                        (20U)
/*! RL - Collision Retry Limit */
#define ENET_EIR_RL(x)                           (((uint32_t)(((uint32_t)(x)) << ENET_EIR_RL_SHIFT)) & ENET_EIR_RL_MASK)

#define ENET_EIR_LC_MASK                         (0x200000U)
#define ENET_EIR_LC_SHIFT                        (21U)
/*! LC - Late Collision */
#define ENET_EIR_LC(x)                           (((uint32_t)(((uint32_t)(x)) << ENET_EIR_LC_SHIFT)) & ENET_EIR_LC_MASK)

#define ENET_EIR_EBERR_MASK                      (0x400000U)
#define ENET_EIR_EBERR_SHIFT                     (22U)
/*! EBERR - Ethernet Bus Error */
#define ENET_EIR_EBERR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_EIR_EBERR_SHIFT)) & ENET_EIR_EBERR_MASK)

#define ENET_EIR_MII_MASK                        (0x800000U)
#define ENET_EIR_MII_SHIFT                       (23U)
/*! MII - MII Interrupt. */
#define ENET_EIR_MII(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_MII_SHIFT)) & ENET_EIR_MII_MASK)

#define ENET_EIR_RXB_MASK                        (0x1000000U)
#define ENET_EIR_RXB_SHIFT                       (24U)
/*! RXB - Receive Buffer Interrupt */
#define ENET_EIR_RXB(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_RXB_SHIFT)) & ENET_EIR_RXB_MASK)

#define ENET_EIR_RXF_MASK                        (0x2000000U)
#define ENET_EIR_RXF_SHIFT                       (25U)
/*! RXF - Receive Frame Interrupt */
#define ENET_EIR_RXF(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_RXF_SHIFT)) & ENET_EIR_RXF_MASK)

#define ENET_EIR_TXB_MASK                        (0x4000000U)
#define ENET_EIR_TXB_SHIFT                       (26U)
/*! TXB - Transmit Buffer Interrupt */
#define ENET_EIR_TXB(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_TXB_SHIFT)) & ENET_EIR_TXB_MASK)

#define ENET_EIR_TXF_MASK                        (0x8000000U)
#define ENET_EIR_TXF_SHIFT                       (27U)
/*! TXF - Transmit Frame Interrupt */
#define ENET_EIR_TXF(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_TXF_SHIFT)) & ENET_EIR_TXF_MASK)

#define ENET_EIR_GRA_MASK                        (0x10000000U)
#define ENET_EIR_GRA_SHIFT                       (28U)
/*! GRA - Graceful Stop Complete */
#define ENET_EIR_GRA(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIR_GRA_SHIFT)) & ENET_EIR_GRA_MASK)

#define ENET_EIR_BABT_MASK                       (0x20000000U)
#define ENET_EIR_BABT_SHIFT                      (29U)
/*! BABT - Babbling Transmit Error */
#define ENET_EIR_BABT(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIR_BABT_SHIFT)) & ENET_EIR_BABT_MASK)

#define ENET_EIR_BABR_MASK                       (0x40000000U)
#define ENET_EIR_BABR_SHIFT                      (30U)
/*! BABR - Babbling Receive Error */
#define ENET_EIR_BABR(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIR_BABR_SHIFT)) & ENET_EIR_BABR_MASK)
/*! @} */

/*! @name EIMR - Interrupt Mask Register */
/*! @{ */

#define ENET_EIMR_TS_TIMER_MASK                  (0x8000U)
#define ENET_EIMR_TS_TIMER_SHIFT                 (15U)
/*! TS_TIMER - TS_TIMER Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_TS_TIMER(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_TS_TIMER_SHIFT)) & ENET_EIMR_TS_TIMER_MASK)

#define ENET_EIMR_TS_AVAIL_MASK                  (0x10000U)
#define ENET_EIMR_TS_AVAIL_SHIFT                 (16U)
/*! TS_AVAIL - TS_AVAIL Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_TS_AVAIL(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_TS_AVAIL_SHIFT)) & ENET_EIMR_TS_AVAIL_MASK)

#define ENET_EIMR_WAKEUP_MASK                    (0x20000U)
#define ENET_EIMR_WAKEUP_SHIFT                   (17U)
/*! WAKEUP - WAKEUP Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_WAKEUP(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_WAKEUP_SHIFT)) & ENET_EIMR_WAKEUP_MASK)

#define ENET_EIMR_PLR_MASK                       (0x40000U)
#define ENET_EIMR_PLR_SHIFT                      (18U)
/*! PLR - PLR Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_PLR(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_PLR_SHIFT)) & ENET_EIMR_PLR_MASK)

#define ENET_EIMR_UN_MASK                        (0x80000U)
#define ENET_EIMR_UN_SHIFT                       (19U)
/*! UN - UN Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_UN(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_UN_SHIFT)) & ENET_EIMR_UN_MASK)

#define ENET_EIMR_RL_MASK                        (0x100000U)
#define ENET_EIMR_RL_SHIFT                       (20U)
/*! RL - RL Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_RL(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_RL_SHIFT)) & ENET_EIMR_RL_MASK)

#define ENET_EIMR_LC_MASK                        (0x200000U)
#define ENET_EIMR_LC_SHIFT                       (21U)
/*! LC - LC Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_LC(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_LC_SHIFT)) & ENET_EIMR_LC_MASK)

#define ENET_EIMR_EBERR_MASK                     (0x400000U)
#define ENET_EIMR_EBERR_SHIFT                    (22U)
/*! EBERR - EBERR Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_EBERR(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_EBERR_SHIFT)) & ENET_EIMR_EBERR_MASK)

#define ENET_EIMR_MII_MASK                       (0x800000U)
#define ENET_EIMR_MII_SHIFT                      (23U)
/*! MII - MII Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_MII(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_MII_SHIFT)) & ENET_EIMR_MII_MASK)

#define ENET_EIMR_RXB_MASK                       (0x1000000U)
#define ENET_EIMR_RXB_SHIFT                      (24U)
/*! RXB - RXB Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_RXB(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_RXB_SHIFT)) & ENET_EIMR_RXB_MASK)

#define ENET_EIMR_RXF_MASK                       (0x2000000U)
#define ENET_EIMR_RXF_SHIFT                      (25U)
/*! RXF - RXF Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_RXF(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_RXF_SHIFT)) & ENET_EIMR_RXF_MASK)

#define ENET_EIMR_TXB_MASK                       (0x4000000U)
#define ENET_EIMR_TXB_SHIFT                      (26U)
/*! TXB - TXB Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_TXB(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_TXB_SHIFT)) & ENET_EIMR_TXB_MASK)

#define ENET_EIMR_TXF_MASK                       (0x8000000U)
#define ENET_EIMR_TXF_SHIFT                      (27U)
/*! TXF - TXF Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_TXF(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_TXF_SHIFT)) & ENET_EIMR_TXF_MASK)

#define ENET_EIMR_GRA_MASK                       (0x10000000U)
#define ENET_EIMR_GRA_SHIFT                      (28U)
/*! GRA - GRA Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_GRA(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_GRA_SHIFT)) & ENET_EIMR_GRA_MASK)

#define ENET_EIMR_BABT_MASK                      (0x20000000U)
#define ENET_EIMR_BABT_SHIFT                     (29U)
/*! BABT - BABT Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_BABT(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_BABT_SHIFT)) & ENET_EIMR_BABT_MASK)

#define ENET_EIMR_BABR_MASK                      (0x40000000U)
#define ENET_EIMR_BABR_SHIFT                     (30U)
/*! BABR - BABR Interrupt Mask
 *  0b0..The corresponding interrupt source is masked.
 *  0b1..The corresponding interrupt source is not masked.
 */
#define ENET_EIMR_BABR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_EIMR_BABR_SHIFT)) & ENET_EIMR_BABR_MASK)
/*! @} */

/*! @name RDAR - Receive Descriptor Active Register - Ring 0 */
/*! @{ */

#define ENET_RDAR_RDAR_MASK                      (0x1000000U)
#define ENET_RDAR_RDAR_SHIFT                     (24U)
/*! RDAR - Receive Descriptor Active */
#define ENET_RDAR_RDAR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RDAR_RDAR_SHIFT)) & ENET_RDAR_RDAR_MASK)
/*! @} */

/*! @name TDAR - Transmit Descriptor Active Register - Ring 0 */
/*! @{ */

#define ENET_TDAR_TDAR_MASK                      (0x1000000U)
#define ENET_TDAR_TDAR_SHIFT                     (24U)
/*! TDAR - Transmit Descriptor Active */
#define ENET_TDAR_TDAR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TDAR_TDAR_SHIFT)) & ENET_TDAR_TDAR_MASK)
/*! @} */

/*! @name ECR - Ethernet Control Register */
/*! @{ */

#define ENET_ECR_RESET_MASK                      (0x1U)
#define ENET_ECR_RESET_SHIFT                     (0U)
/*! RESET - Ethernet MAC Reset */
#define ENET_ECR_RESET(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ECR_RESET_SHIFT)) & ENET_ECR_RESET_MASK)

#define ENET_ECR_ETHEREN_MASK                    (0x2U)
#define ENET_ECR_ETHEREN_SHIFT                   (1U)
/*! ETHEREN - Ethernet Enable
 *  0b0..Reception immediately stops and transmission stops after a bad CRC is appended to any currently transmitted frame.
 *  0b1..MAC is enabled, and reception and transmission are possible.
 */
#define ENET_ECR_ETHEREN(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_ECR_ETHEREN_SHIFT)) & ENET_ECR_ETHEREN_MASK)

#define ENET_ECR_MAGICEN_MASK                    (0x4U)
#define ENET_ECR_MAGICEN_SHIFT                   (2U)
/*! MAGICEN - Magic Packet Detection Enable
 *  0b0..Magic detection logic disabled.
 *  0b1..The MAC core detects magic packets and asserts EIR[WAKEUP] when a frame is detected.
 */
#define ENET_ECR_MAGICEN(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_ECR_MAGICEN_SHIFT)) & ENET_ECR_MAGICEN_MASK)

#define ENET_ECR_SLEEP_MASK                      (0x8U)
#define ENET_ECR_SLEEP_SHIFT                     (3U)
/*! SLEEP - Sleep Mode Enable
 *  0b0..Normal operating mode.
 *  0b1..Sleep mode.
 */
#define ENET_ECR_SLEEP(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ECR_SLEEP_SHIFT)) & ENET_ECR_SLEEP_MASK)

#define ENET_ECR_EN1588_MASK                     (0x10U)
#define ENET_ECR_EN1588_SHIFT                    (4U)
/*! EN1588 - EN1588 Enable
 *  0b0..Legacy FEC buffer descriptors and functions enabled.
 *  0b1..Enhanced frame time-stamping functions enabled. Has no effect within the MAC besides controlling the DMA control bit ena_1588.
 */
#define ENET_ECR_EN1588(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_ECR_EN1588_SHIFT)) & ENET_ECR_EN1588_MASK)

#define ENET_ECR_DBGEN_MASK                      (0x40U)
#define ENET_ECR_DBGEN_SHIFT                     (6U)
/*! DBGEN - Debug Enable
 *  0b0..MAC continues operation in debug mode.
 *  0b1..MAC enters hardware freeze mode when the processor is in debug mode.
 */
#define ENET_ECR_DBGEN(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ECR_DBGEN_SHIFT)) & ENET_ECR_DBGEN_MASK)

#define ENET_ECR_DBSWP_MASK                      (0x100U)
#define ENET_ECR_DBSWP_SHIFT                     (8U)
/*! DBSWP - Descriptor Byte Swapping Enable
 *  0b0..The buffer descriptor bytes are not swapped to support big-endian devices.
 *  0b1..The buffer descriptor bytes are swapped to support little-endian devices.
 */
#define ENET_ECR_DBSWP(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ECR_DBSWP_SHIFT)) & ENET_ECR_DBSWP_MASK)
/*! @} */

/*! @name MMFR - MII Management Frame Register */
/*! @{ */

#define ENET_MMFR_DATA_MASK                      (0xFFFFU)
#define ENET_MMFR_DATA_SHIFT                     (0U)
/*! DATA - Management Frame Data */
#define ENET_MMFR_DATA(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_DATA_SHIFT)) & ENET_MMFR_DATA_MASK)

#define ENET_MMFR_TA_MASK                        (0x30000U)
#define ENET_MMFR_TA_SHIFT                       (16U)
/*! TA - Turn Around */
#define ENET_MMFR_TA(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_TA_SHIFT)) & ENET_MMFR_TA_MASK)

#define ENET_MMFR_RA_MASK                        (0x7C0000U)
#define ENET_MMFR_RA_SHIFT                       (18U)
/*! RA - Register Address */
#define ENET_MMFR_RA(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_RA_SHIFT)) & ENET_MMFR_RA_MASK)

#define ENET_MMFR_PA_MASK                        (0xF800000U)
#define ENET_MMFR_PA_SHIFT                       (23U)
/*! PA - PHY Address */
#define ENET_MMFR_PA(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_PA_SHIFT)) & ENET_MMFR_PA_MASK)

#define ENET_MMFR_OP_MASK                        (0x30000000U)
#define ENET_MMFR_OP_SHIFT                       (28U)
/*! OP - Operation Code */
#define ENET_MMFR_OP(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_OP_SHIFT)) & ENET_MMFR_OP_MASK)

#define ENET_MMFR_ST_MASK                        (0xC0000000U)
#define ENET_MMFR_ST_SHIFT                       (30U)
/*! ST - Start Of Frame Delimiter */
#define ENET_MMFR_ST(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_MMFR_ST_SHIFT)) & ENET_MMFR_ST_MASK)
/*! @} */

/*! @name MSCR - MII Speed Control Register */
/*! @{ */

#define ENET_MSCR_MII_SPEED_MASK                 (0x7EU)
#define ENET_MSCR_MII_SPEED_SHIFT                (1U)
/*! MII_SPEED - MII Speed */
#define ENET_MSCR_MII_SPEED(x)                   (((uint32_t)(((uint32_t)(x)) << ENET_MSCR_MII_SPEED_SHIFT)) & ENET_MSCR_MII_SPEED_MASK)

#define ENET_MSCR_DIS_PRE_MASK                   (0x80U)
#define ENET_MSCR_DIS_PRE_SHIFT                  (7U)
/*! DIS_PRE - Disable Preamble
 *  0b0..Preamble enabled.
 *  0b1..Preamble (32 ones) is not prepended to the MII management frame.
 */
#define ENET_MSCR_DIS_PRE(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_MSCR_DIS_PRE_SHIFT)) & ENET_MSCR_DIS_PRE_MASK)

#define ENET_MSCR_HOLDTIME_MASK                  (0x700U)
#define ENET_MSCR_HOLDTIME_SHIFT                 (8U)
/*! HOLDTIME - Hold time On MDIO Output
 *  0b000..1 internal module clock cycle
 *  0b001..2 internal module clock cycles
 *  0b010..3 internal module clock cycles
 *  0b111..8 internal module clock cycles
 */
#define ENET_MSCR_HOLDTIME(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_MSCR_HOLDTIME_SHIFT)) & ENET_MSCR_HOLDTIME_MASK)
/*! @} */

/*! @name MIBC - MIB Control Register */
/*! @{ */

#define ENET_MIBC_MIB_CLEAR_MASK                 (0x20000000U)
#define ENET_MIBC_MIB_CLEAR_SHIFT                (29U)
/*! MIB_CLEAR - MIB Clear
 *  0b0..See note above.
 *  0b1..All statistics counters are reset to 0.
 */
#define ENET_MIBC_MIB_CLEAR(x)                   (((uint32_t)(((uint32_t)(x)) << ENET_MIBC_MIB_CLEAR_SHIFT)) & ENET_MIBC_MIB_CLEAR_MASK)

#define ENET_MIBC_MIB_IDLE_MASK                  (0x40000000U)
#define ENET_MIBC_MIB_IDLE_SHIFT                 (30U)
/*! MIB_IDLE - MIB Idle
 *  0b0..The MIB block is updating MIB counters.
 *  0b1..The MIB block is not currently updating any MIB counters.
 */
#define ENET_MIBC_MIB_IDLE(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_MIBC_MIB_IDLE_SHIFT)) & ENET_MIBC_MIB_IDLE_MASK)

#define ENET_MIBC_MIB_DIS_MASK                   (0x80000000U)
#define ENET_MIBC_MIB_DIS_SHIFT                  (31U)
/*! MIB_DIS - Disable MIB Logic
 *  0b0..MIB logic is enabled.
 *  0b1..MIB logic is disabled. The MIB logic halts and does not update any MIB counters.
 */
#define ENET_MIBC_MIB_DIS(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_MIBC_MIB_DIS_SHIFT)) & ENET_MIBC_MIB_DIS_MASK)
/*! @} */

/*! @name RCR - Receive Control Register */
/*! @{ */

#define ENET_RCR_LOOP_MASK                       (0x1U)
#define ENET_RCR_LOOP_SHIFT                      (0U)
/*! LOOP - Internal Loopback
 *  0b0..Loopback disabled.
 *  0b1..Transmitted frames are looped back internal to the device and transmit MII output signals are not asserted. DRT must be cleared.
 */
#define ENET_RCR_LOOP(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_RCR_LOOP_SHIFT)) & ENET_RCR_LOOP_MASK)

#define ENET_RCR_DRT_MASK                        (0x2U)
#define ENET_RCR_DRT_SHIFT                       (1U)
/*! DRT - Disable Receive On Transmit
 *  0b0..Receive path operates independently of transmit (i.e., full-duplex mode). Can also be used to monitor transmit activity in half-duplex mode.
 *  0b1..Disable reception of frames while transmitting. (Normally used for half-duplex mode.)
 */
#define ENET_RCR_DRT(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_RCR_DRT_SHIFT)) & ENET_RCR_DRT_MASK)

#define ENET_RCR_MII_MODE_MASK                   (0x4U)
#define ENET_RCR_MII_MODE_SHIFT                  (2U)
/*! MII_MODE - Media Independent Interface Mode
 *  0b0..Reserved.
 *  0b1..MII or RMII mode, as indicated by the RMII_MODE field.
 */
#define ENET_RCR_MII_MODE(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_RCR_MII_MODE_SHIFT)) & ENET_RCR_MII_MODE_MASK)

#define ENET_RCR_PROM_MASK                       (0x8U)
#define ENET_RCR_PROM_SHIFT                      (3U)
/*! PROM - Promiscuous Mode
 *  0b0..Disabled.
 *  0b1..Enabled.
 */
#define ENET_RCR_PROM(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_RCR_PROM_SHIFT)) & ENET_RCR_PROM_MASK)

#define ENET_RCR_BC_REJ_MASK                     (0x10U)
#define ENET_RCR_BC_REJ_SHIFT                    (4U)
/*! BC_REJ - Broadcast Frame Reject
 *  0b0..Will not reject frames as described above
 *  0b1..Will reject frames as described above
 */
#define ENET_RCR_BC_REJ(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_RCR_BC_REJ_SHIFT)) & ENET_RCR_BC_REJ_MASK)

#define ENET_RCR_FCE_MASK                        (0x20U)
#define ENET_RCR_FCE_SHIFT                       (5U)
/*! FCE - Flow Control Enable
 *  0b0..Disable flow control
 *  0b1..Enable flow control
 */
#define ENET_RCR_FCE(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_RCR_FCE_SHIFT)) & ENET_RCR_FCE_MASK)

#define ENET_RCR_RMII_MODE_MASK                  (0x100U)
#define ENET_RCR_RMII_MODE_SHIFT                 (8U)
/*! RMII_MODE - RMII Mode Enable
 *  0b0..MAC configured for MII mode.
 *  0b1..MAC configured for RMII operation.
 */
#define ENET_RCR_RMII_MODE(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_RCR_RMII_MODE_SHIFT)) & ENET_RCR_RMII_MODE_MASK)

#define ENET_RCR_RMII_10T_MASK                   (0x200U)
#define ENET_RCR_RMII_10T_SHIFT                  (9U)
/*! RMII_10T
 *  0b0..100-Mbit/s operation.
 *  0b1..10-Mbit/s operation.
 */
#define ENET_RCR_RMII_10T(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_RCR_RMII_10T_SHIFT)) & ENET_RCR_RMII_10T_MASK)

#define ENET_RCR_PADEN_MASK                      (0x1000U)
#define ENET_RCR_PADEN_SHIFT                     (12U)
/*! PADEN - Enable Frame Padding Remove On Receive
 *  0b0..No padding is removed on receive by the MAC.
 *  0b1..Padding is removed from received frames.
 */
#define ENET_RCR_PADEN(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RCR_PADEN_SHIFT)) & ENET_RCR_PADEN_MASK)

#define ENET_RCR_PAUFWD_MASK                     (0x2000U)
#define ENET_RCR_PAUFWD_SHIFT                    (13U)
/*! PAUFWD - Terminate/Forward Pause Frames
 *  0b0..Pause frames are terminated and discarded in the MAC.
 *  0b1..Pause frames are forwarded to the user application.
 */
#define ENET_RCR_PAUFWD(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_RCR_PAUFWD_SHIFT)) & ENET_RCR_PAUFWD_MASK)

#define ENET_RCR_CRCFWD_MASK                     (0x4000U)
#define ENET_RCR_CRCFWD_SHIFT                    (14U)
/*! CRCFWD - Terminate/Forward Received CRC
 *  0b0..The CRC field of received frames is transmitted to the user application.
 *  0b1..The CRC field is stripped from the frame.
 */
#define ENET_RCR_CRCFWD(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_RCR_CRCFWD_SHIFT)) & ENET_RCR_CRCFWD_MASK)

#define ENET_RCR_CFEN_MASK                       (0x8000U)
#define ENET_RCR_CFEN_SHIFT                      (15U)
/*! CFEN - MAC Control Frame Enable
 *  0b0..MAC control frames with any opcode other than 0x0001 (pause frame) are accepted and forwarded to the client interface.
 *  0b1..MAC control frames with any opcode other than 0x0001 (pause frame) are silently discarded.
 */
#define ENET_RCR_CFEN(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_RCR_CFEN_SHIFT)) & ENET_RCR_CFEN_MASK)

#define ENET_RCR_MAX_FL_MASK                     (0x3FFF0000U)
#define ENET_RCR_MAX_FL_SHIFT                    (16U)
/*! MAX_FL - Maximum Frame Length */
#define ENET_RCR_MAX_FL(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_RCR_MAX_FL_SHIFT)) & ENET_RCR_MAX_FL_MASK)

#define ENET_RCR_NLC_MASK                        (0x40000000U)
#define ENET_RCR_NLC_SHIFT                       (30U)
/*! NLC - Payload Length Check Disable
 *  0b0..The payload length check is disabled.
 *  0b1..The core checks the frame's payload length with the frame length/type field. Errors are indicated in the EIR[PLR] field.
 */
#define ENET_RCR_NLC(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_RCR_NLC_SHIFT)) & ENET_RCR_NLC_MASK)

#define ENET_RCR_GRS_MASK                        (0x80000000U)
#define ENET_RCR_GRS_SHIFT                       (31U)
/*! GRS - Graceful Receive Stopped
 *  0b0..Receive not stopped
 *  0b1..Receive stopped
 */
#define ENET_RCR_GRS(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_RCR_GRS_SHIFT)) & ENET_RCR_GRS_MASK)
/*! @} */

/*! @name TCR - Transmit Control Register */
/*! @{ */

#define ENET_TCR_GTS_MASK                        (0x1U)
#define ENET_TCR_GTS_SHIFT                       (0U)
/*! GTS - Graceful Transmit Stop
 *  0b0..Disable graceful transmit stop
 *  0b1..Enable graceful transmit stop
 */
#define ENET_TCR_GTS(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_TCR_GTS_SHIFT)) & ENET_TCR_GTS_MASK)

#define ENET_TCR_FDEN_MASK                       (0x4U)
#define ENET_TCR_FDEN_SHIFT                      (2U)
/*! FDEN - Full-Duplex Enable
 *  0b0..Disable full-duplex
 *  0b1..Enable full-duplex
 */
#define ENET_TCR_FDEN(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TCR_FDEN_SHIFT)) & ENET_TCR_FDEN_MASK)

#define ENET_TCR_TFC_PAUSE_MASK                  (0x8U)
#define ENET_TCR_TFC_PAUSE_SHIFT                 (3U)
/*! TFC_PAUSE - Transmit Frame Control Pause
 *  0b0..No PAUSE frame transmitted.
 *  0b1..The MAC stops transmission of data frames after the current transmission is complete.
 */
#define ENET_TCR_TFC_PAUSE(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_TCR_TFC_PAUSE_SHIFT)) & ENET_TCR_TFC_PAUSE_MASK)

#define ENET_TCR_RFC_PAUSE_MASK                  (0x10U)
#define ENET_TCR_RFC_PAUSE_SHIFT                 (4U)
/*! RFC_PAUSE - Receive Frame Control Pause */
#define ENET_TCR_RFC_PAUSE(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_TCR_RFC_PAUSE_SHIFT)) & ENET_TCR_RFC_PAUSE_MASK)

#define ENET_TCR_ADDSEL_MASK                     (0xE0U)
#define ENET_TCR_ADDSEL_SHIFT                    (5U)
/*! ADDSEL - Source MAC Address Select On Transmit
 *  0b000..Node MAC address programmed on PADDR1/2 registers.
 *  0b100..Reserved.
 *  0b101..Reserved.
 *  0b110..Reserved.
 */
#define ENET_TCR_ADDSEL(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_TCR_ADDSEL_SHIFT)) & ENET_TCR_ADDSEL_MASK)

#define ENET_TCR_ADDINS_MASK                     (0x100U)
#define ENET_TCR_ADDINS_SHIFT                    (8U)
/*! ADDINS - Set MAC Address On Transmit
 *  0b0..The source MAC address is not modified by the MAC.
 *  0b1..The MAC overwrites the source MAC address with the programmed MAC address according to ADDSEL.
 */
#define ENET_TCR_ADDINS(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_TCR_ADDINS_SHIFT)) & ENET_TCR_ADDINS_MASK)

#define ENET_TCR_CRCFWD_MASK                     (0x200U)
#define ENET_TCR_CRCFWD_SHIFT                    (9U)
/*! CRCFWD - Forward Frame From Application With CRC
 *  0b0..TxBD[TC] controls whether the frame has a CRC from the application.
 *  0b1..The transmitter does not append any CRC to transmitted frames, as it is expecting a frame with CRC from the application.
 */
#define ENET_TCR_CRCFWD(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_TCR_CRCFWD_SHIFT)) & ENET_TCR_CRCFWD_MASK)
/*! @} */

/*! @name PALR - Physical Address Lower Register */
/*! @{ */

#define ENET_PALR_PADDR1_MASK                    (0xFFFFFFFFU)
#define ENET_PALR_PADDR1_SHIFT                   (0U)
/*! PADDR1 - Pause Address */
#define ENET_PALR_PADDR1(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_PALR_PADDR1_SHIFT)) & ENET_PALR_PADDR1_MASK)
/*! @} */

/*! @name PAUR - Physical Address Upper Register */
/*! @{ */

#define ENET_PAUR_TYPE_MASK                      (0xFFFFU)
#define ENET_PAUR_TYPE_SHIFT                     (0U)
/*! TYPE - Type Field In PAUSE Frames */
#define ENET_PAUR_TYPE(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_PAUR_TYPE_SHIFT)) & ENET_PAUR_TYPE_MASK)

#define ENET_PAUR_PADDR2_MASK                    (0xFFFF0000U)
#define ENET_PAUR_PADDR2_SHIFT                   (16U)
#define ENET_PAUR_PADDR2(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_PAUR_PADDR2_SHIFT)) & ENET_PAUR_PADDR2_MASK)
/*! @} */

/*! @name OPD - Opcode/Pause Duration Register */
/*! @{ */

#define ENET_OPD_PAUSE_DUR_MASK                  (0xFFFFU)
#define ENET_OPD_PAUSE_DUR_SHIFT                 (0U)
/*! PAUSE_DUR - Pause Duration */
#define ENET_OPD_PAUSE_DUR(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_OPD_PAUSE_DUR_SHIFT)) & ENET_OPD_PAUSE_DUR_MASK)

#define ENET_OPD_OPCODE_MASK                     (0xFFFF0000U)
#define ENET_OPD_OPCODE_SHIFT                    (16U)
/*! OPCODE - Opcode Field In PAUSE Frames */
#define ENET_OPD_OPCODE(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_OPD_OPCODE_SHIFT)) & ENET_OPD_OPCODE_MASK)
/*! @} */

/*! @name TXIC - Transmit Interrupt Coalescing Register */
/*! @{ */

#define ENET_TXIC_ICTT_MASK                      (0xFFFFU)
#define ENET_TXIC_ICTT_SHIFT                     (0U)
/*! ICTT - Interrupt coalescing timer threshold */
#define ENET_TXIC_ICTT(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TXIC_ICTT_SHIFT)) & ENET_TXIC_ICTT_MASK)

#define ENET_TXIC_ICFT_MASK                      (0xFF00000U)
#define ENET_TXIC_ICFT_SHIFT                     (20U)
/*! ICFT - Interrupt coalescing frame count threshold */
#define ENET_TXIC_ICFT(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TXIC_ICFT_SHIFT)) & ENET_TXIC_ICFT_MASK)

#define ENET_TXIC_ICCS_MASK                      (0x40000000U)
#define ENET_TXIC_ICCS_SHIFT                     (30U)
/*! ICCS - Interrupt Coalescing Timer Clock Source Select
 *  0b0..Use MII/GMII TX clocks.
 *  0b1..Use ENET system clock.
 */
#define ENET_TXIC_ICCS(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TXIC_ICCS_SHIFT)) & ENET_TXIC_ICCS_MASK)

#define ENET_TXIC_ICEN_MASK                      (0x80000000U)
#define ENET_TXIC_ICEN_SHIFT                     (31U)
/*! ICEN - Interrupt Coalescing Enable
 *  0b0..Disable Interrupt coalescing.
 *  0b1..Enable Interrupt coalescing.
 */
#define ENET_TXIC_ICEN(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TXIC_ICEN_SHIFT)) & ENET_TXIC_ICEN_MASK)
/*! @} */

/*! @name RXIC - Receive Interrupt Coalescing Register */
/*! @{ */

#define ENET_RXIC_ICTT_MASK                      (0xFFFFU)
#define ENET_RXIC_ICTT_SHIFT                     (0U)
/*! ICTT - Interrupt coalescing timer threshold */
#define ENET_RXIC_ICTT(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RXIC_ICTT_SHIFT)) & ENET_RXIC_ICTT_MASK)

#define ENET_RXIC_ICFT_MASK                      (0xFF00000U)
#define ENET_RXIC_ICFT_SHIFT                     (20U)
/*! ICFT - Interrupt coalescing frame count threshold */
#define ENET_RXIC_ICFT(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RXIC_ICFT_SHIFT)) & ENET_RXIC_ICFT_MASK)

#define ENET_RXIC_ICCS_MASK                      (0x40000000U)
#define ENET_RXIC_ICCS_SHIFT                     (30U)
/*! ICCS - Interrupt Coalescing Timer Clock Source Select
 *  0b0..Use MII/GMII TX clocks.
 *  0b1..Use ENET system clock.
 */
#define ENET_RXIC_ICCS(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RXIC_ICCS_SHIFT)) & ENET_RXIC_ICCS_MASK)

#define ENET_RXIC_ICEN_MASK                      (0x80000000U)
#define ENET_RXIC_ICEN_SHIFT                     (31U)
/*! ICEN - Interrupt Coalescing Enable
 *  0b0..Disable Interrupt coalescing.
 *  0b1..Enable Interrupt coalescing.
 */
#define ENET_RXIC_ICEN(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_RXIC_ICEN_SHIFT)) & ENET_RXIC_ICEN_MASK)
/*! @} */

/*! @name IAUR - Descriptor Individual Upper Address Register */
/*! @{ */

#define ENET_IAUR_IADDR1_MASK                    (0xFFFFFFFFU)
#define ENET_IAUR_IADDR1_SHIFT                   (0U)
#define ENET_IAUR_IADDR1(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_IAUR_IADDR1_SHIFT)) & ENET_IAUR_IADDR1_MASK)
/*! @} */

/*! @name IALR - Descriptor Individual Lower Address Register */
/*! @{ */

#define ENET_IALR_IADDR2_MASK                    (0xFFFFFFFFU)
#define ENET_IALR_IADDR2_SHIFT                   (0U)
#define ENET_IALR_IADDR2(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_IALR_IADDR2_SHIFT)) & ENET_IALR_IADDR2_MASK)
/*! @} */

/*! @name GAUR - Descriptor Group Upper Address Register */
/*! @{ */

#define ENET_GAUR_GADDR1_MASK                    (0xFFFFFFFFU)
#define ENET_GAUR_GADDR1_SHIFT                   (0U)
#define ENET_GAUR_GADDR1(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_GAUR_GADDR1_SHIFT)) & ENET_GAUR_GADDR1_MASK)
/*! @} */

/*! @name GALR - Descriptor Group Lower Address Register */
/*! @{ */

#define ENET_GALR_GADDR2_MASK                    (0xFFFFFFFFU)
#define ENET_GALR_GADDR2_SHIFT                   (0U)
#define ENET_GALR_GADDR2(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_GALR_GADDR2_SHIFT)) & ENET_GALR_GADDR2_MASK)
/*! @} */

/*! @name TFWR - Transmit FIFO Watermark Register */
/*! @{ */

#define ENET_TFWR_TFWR_MASK                      (0x3FU)
#define ENET_TFWR_TFWR_SHIFT                     (0U)
/*! TFWR - Transmit FIFO Write
 *  0b000000..64 bytes written.
 *  0b000001..64 bytes written.
 *  0b000010..128 bytes written.
 *  0b000011..192 bytes written.
 *  0b011111..1984 bytes written.
 */
#define ENET_TFWR_TFWR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TFWR_TFWR_SHIFT)) & ENET_TFWR_TFWR_MASK)

#define ENET_TFWR_STRFWD_MASK                    (0x100U)
#define ENET_TFWR_STRFWD_SHIFT                   (8U)
/*! STRFWD - Store And Forward Enable
 *  0b0..Reset. The transmission start threshold is programmed in TFWR[TFWR].
 *  0b1..Enabled.
 */
#define ENET_TFWR_STRFWD(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_TFWR_STRFWD_SHIFT)) & ENET_TFWR_STRFWD_MASK)
/*! @} */

/*! @name RDSR - Receive Descriptor Ring 0 Start Register */
/*! @{ */

#define ENET_RDSR_R_DES_START_MASK               (0xFFFFFFF8U)
#define ENET_RDSR_R_DES_START_SHIFT              (3U)
#define ENET_RDSR_R_DES_START(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_RDSR_R_DES_START_SHIFT)) & ENET_RDSR_R_DES_START_MASK)
/*! @} */

/*! @name TDSR - Transmit Buffer Descriptor Ring 0 Start Register */
/*! @{ */

#define ENET_TDSR_X_DES_START_MASK               (0xFFFFFFF8U)
#define ENET_TDSR_X_DES_START_SHIFT              (3U)
#define ENET_TDSR_X_DES_START(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_TDSR_X_DES_START_SHIFT)) & ENET_TDSR_X_DES_START_MASK)
/*! @} */

/*! @name MRBR - Maximum Receive Buffer Size Register - Ring 0 */
/*! @{ */

#define ENET_MRBR_R_BUF_SIZE_MASK                (0x3FF0U)
#define ENET_MRBR_R_BUF_SIZE_SHIFT               (4U)
#define ENET_MRBR_R_BUF_SIZE(x)                  (((uint32_t)(((uint32_t)(x)) << ENET_MRBR_R_BUF_SIZE_SHIFT)) & ENET_MRBR_R_BUF_SIZE_MASK)
/*! @} */

/*! @name RSFL - Receive FIFO Section Full Threshold */
/*! @{ */

#define ENET_RSFL_RX_SECTION_FULL_MASK           (0xFFU)
#define ENET_RSFL_RX_SECTION_FULL_SHIFT          (0U)
/*! RX_SECTION_FULL - Value Of Receive FIFO Section Full Threshold */
#define ENET_RSFL_RX_SECTION_FULL(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RSFL_RX_SECTION_FULL_SHIFT)) & ENET_RSFL_RX_SECTION_FULL_MASK)
/*! @} */

/*! @name RSEM - Receive FIFO Section Empty Threshold */
/*! @{ */

#define ENET_RSEM_RX_SECTION_EMPTY_MASK          (0xFFU)
#define ENET_RSEM_RX_SECTION_EMPTY_SHIFT         (0U)
/*! RX_SECTION_EMPTY - Value Of The Receive FIFO Section Empty Threshold */
#define ENET_RSEM_RX_SECTION_EMPTY(x)            (((uint32_t)(((uint32_t)(x)) << ENET_RSEM_RX_SECTION_EMPTY_SHIFT)) & ENET_RSEM_RX_SECTION_EMPTY_MASK)

#define ENET_RSEM_STAT_SECTION_EMPTY_MASK        (0x1F0000U)
#define ENET_RSEM_STAT_SECTION_EMPTY_SHIFT       (16U)
/*! STAT_SECTION_EMPTY - RX Status FIFO Section Empty Threshold */
#define ENET_RSEM_STAT_SECTION_EMPTY(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RSEM_STAT_SECTION_EMPTY_SHIFT)) & ENET_RSEM_STAT_SECTION_EMPTY_MASK)
/*! @} */

/*! @name RAEM - Receive FIFO Almost Empty Threshold */
/*! @{ */

#define ENET_RAEM_RX_ALMOST_EMPTY_MASK           (0xFFU)
#define ENET_RAEM_RX_ALMOST_EMPTY_SHIFT          (0U)
/*! RX_ALMOST_EMPTY - Value Of The Receive FIFO Almost Empty Threshold */
#define ENET_RAEM_RX_ALMOST_EMPTY(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RAEM_RX_ALMOST_EMPTY_SHIFT)) & ENET_RAEM_RX_ALMOST_EMPTY_MASK)
/*! @} */

/*! @name RAFL - Receive FIFO Almost Full Threshold */
/*! @{ */

#define ENET_RAFL_RX_ALMOST_FULL_MASK            (0xFFU)
#define ENET_RAFL_RX_ALMOST_FULL_SHIFT           (0U)
/*! RX_ALMOST_FULL - Value Of The Receive FIFO Almost Full Threshold */
#define ENET_RAFL_RX_ALMOST_FULL(x)              (((uint32_t)(((uint32_t)(x)) << ENET_RAFL_RX_ALMOST_FULL_SHIFT)) & ENET_RAFL_RX_ALMOST_FULL_MASK)
/*! @} */

/*! @name TSEM - Transmit FIFO Section Empty Threshold */
/*! @{ */

#define ENET_TSEM_TX_SECTION_EMPTY_MASK          (0xFFU)
#define ENET_TSEM_TX_SECTION_EMPTY_SHIFT         (0U)
/*! TX_SECTION_EMPTY - Value Of The Transmit FIFO Section Empty Threshold */
#define ENET_TSEM_TX_SECTION_EMPTY(x)            (((uint32_t)(((uint32_t)(x)) << ENET_TSEM_TX_SECTION_EMPTY_SHIFT)) & ENET_TSEM_TX_SECTION_EMPTY_MASK)
/*! @} */

/*! @name TAEM - Transmit FIFO Almost Empty Threshold */
/*! @{ */

#define ENET_TAEM_TX_ALMOST_EMPTY_MASK           (0xFFU)
#define ENET_TAEM_TX_ALMOST_EMPTY_SHIFT          (0U)
/*! TX_ALMOST_EMPTY - Value of Transmit FIFO Almost Empty Threshold */
#define ENET_TAEM_TX_ALMOST_EMPTY(x)             (((uint32_t)(((uint32_t)(x)) << ENET_TAEM_TX_ALMOST_EMPTY_SHIFT)) & ENET_TAEM_TX_ALMOST_EMPTY_MASK)
/*! @} */

/*! @name TAFL - Transmit FIFO Almost Full Threshold */
/*! @{ */

#define ENET_TAFL_TX_ALMOST_FULL_MASK            (0xFFU)
#define ENET_TAFL_TX_ALMOST_FULL_SHIFT           (0U)
/*! TX_ALMOST_FULL - Value Of The Transmit FIFO Almost Full Threshold */
#define ENET_TAFL_TX_ALMOST_FULL(x)              (((uint32_t)(((uint32_t)(x)) << ENET_TAFL_TX_ALMOST_FULL_SHIFT)) & ENET_TAFL_TX_ALMOST_FULL_MASK)
/*! @} */

/*! @name TIPG - Transmit Inter-Packet Gap */
/*! @{ */

#define ENET_TIPG_IPG_MASK                       (0x1FU)
#define ENET_TIPG_IPG_SHIFT                      (0U)
/*! IPG - Transmit Inter-Packet Gap */
#define ENET_TIPG_IPG(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TIPG_IPG_SHIFT)) & ENET_TIPG_IPG_MASK)
/*! @} */

/*! @name FTRL - Frame Truncation Length */
/*! @{ */

#define ENET_FTRL_TRUNC_FL_MASK                  (0x3FFFU)
#define ENET_FTRL_TRUNC_FL_SHIFT                 (0U)
/*! TRUNC_FL - Frame Truncation Length */
#define ENET_FTRL_TRUNC_FL(x)                    (((uint32_t)(((uint32_t)(x)) << ENET_FTRL_TRUNC_FL_SHIFT)) & ENET_FTRL_TRUNC_FL_MASK)
/*! @} */

/*! @name TACC - Transmit Accelerator Function Configuration */
/*! @{ */

#define ENET_TACC_SHIFT16_MASK                   (0x1U)
#define ENET_TACC_SHIFT16_SHIFT                  (0U)
/*! SHIFT16 - TX FIFO Shift-16
 *  0b0..Disabled.
 *  0b1..Indicates to the transmit data FIFO that the written frames contain two additional octets before the
 *       frame data. This means the actual frame begins at bit 16 of the first word written into the FIFO. This
 *       function allows putting the frame payload on a 32-bit boundary in memory, as the 14-byte Ethernet header is
 *       extended to a 16-byte header.
 */
#define ENET_TACC_SHIFT16(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_TACC_SHIFT16_SHIFT)) & ENET_TACC_SHIFT16_MASK)

#define ENET_TACC_IPCHK_MASK                     (0x8U)
#define ENET_TACC_IPCHK_SHIFT                    (3U)
/*! IPCHK
 *  0b0..Checksum is not inserted.
 *  0b1..If an IP frame is transmitted, the checksum is inserted automatically. The IP header checksum field must
 *       be cleared. If a non-IP frame is transmitted the frame is not modified.
 */
#define ENET_TACC_IPCHK(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_TACC_IPCHK_SHIFT)) & ENET_TACC_IPCHK_MASK)

#define ENET_TACC_PROCHK_MASK                    (0x10U)
#define ENET_TACC_PROCHK_SHIFT                   (4U)
/*! PROCHK
 *  0b0..Checksum not inserted.
 *  0b1..If an IP frame with a known protocol is transmitted, the checksum is inserted automatically into the
 *       frame. The checksum field must be cleared. The other frames are not modified.
 */
#define ENET_TACC_PROCHK(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_TACC_PROCHK_SHIFT)) & ENET_TACC_PROCHK_MASK)
/*! @} */

/*! @name RACC - Receive Accelerator Function Configuration */
/*! @{ */

#define ENET_RACC_PADREM_MASK                    (0x1U)
#define ENET_RACC_PADREM_SHIFT                   (0U)
/*! PADREM - Enable Padding Removal For Short IP Frames
 *  0b0..Padding not removed.
 *  0b1..Any bytes following the IP payload section of the frame are removed from the frame.
 */
#define ENET_RACC_PADREM(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_RACC_PADREM_SHIFT)) & ENET_RACC_PADREM_MASK)

#define ENET_RACC_IPDIS_MASK                     (0x2U)
#define ENET_RACC_IPDIS_SHIFT                    (1U)
/*! IPDIS - Enable Discard Of Frames With Wrong IPv4 Header Checksum
 *  0b0..Frames with wrong IPv4 header checksum are not discarded.
 *  0b1..If an IPv4 frame is received with a mismatching header checksum, the frame is discarded. IPv6 has no
 *       header checksum and is not affected by this setting. Discarding is only available when the RX FIFO operates in
 *       store and forward mode (RSFL cleared).
 */
#define ENET_RACC_IPDIS(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_RACC_IPDIS_SHIFT)) & ENET_RACC_IPDIS_MASK)

#define ENET_RACC_PRODIS_MASK                    (0x4U)
#define ENET_RACC_PRODIS_SHIFT                   (2U)
/*! PRODIS - Enable Discard Of Frames With Wrong Protocol Checksum
 *  0b0..Frames with wrong checksum are not discarded.
 *  0b1..If a TCP/IP, UDP/IP, or ICMP/IP frame is received that has a wrong TCP, UDP, or ICMP checksum, the frame
 *       is discarded. Discarding is only available when the RX FIFO operates in store and forward mode (RSFL
 *       cleared).
 */
#define ENET_RACC_PRODIS(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_RACC_PRODIS_SHIFT)) & ENET_RACC_PRODIS_MASK)

#define ENET_RACC_LINEDIS_MASK                   (0x40U)
#define ENET_RACC_LINEDIS_SHIFT                  (6U)
/*! LINEDIS - Enable Discard Of Frames With MAC Layer Errors
 *  0b0..Frames with errors are not discarded.
 *  0b1..Any frame received with a CRC, length, or PHY error is automatically discarded and not forwarded to the user application interface.
 */
#define ENET_RACC_LINEDIS(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_RACC_LINEDIS_SHIFT)) & ENET_RACC_LINEDIS_MASK)

#define ENET_RACC_SHIFT16_MASK                   (0x80U)
#define ENET_RACC_SHIFT16_SHIFT                  (7U)
/*! SHIFT16 - RX FIFO Shift-16
 *  0b0..Disabled.
 *  0b1..Instructs the MAC to write two additional bytes in front of each frame received into the RX FIFO.
 */
#define ENET_RACC_SHIFT16(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_RACC_SHIFT16_SHIFT)) & ENET_RACC_SHIFT16_MASK)
/*! @} */

/*! @name RMON_T_PACKETS - Tx Packet Count Statistic Register */
/*! @{ */

#define ENET_RMON_T_PACKETS_TXPKTS_MASK          (0xFFFFU)
#define ENET_RMON_T_PACKETS_TXPKTS_SHIFT         (0U)
/*! TXPKTS - Packet count */
#define ENET_RMON_T_PACKETS_TXPKTS(x)            (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_PACKETS_TXPKTS_SHIFT)) & ENET_RMON_T_PACKETS_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_BC_PKT - Tx Broadcast Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_BC_PKT_TXPKTS_MASK           (0xFFFFU)
#define ENET_RMON_T_BC_PKT_TXPKTS_SHIFT          (0U)
/*! TXPKTS - Broadcast packets */
#define ENET_RMON_T_BC_PKT_TXPKTS(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_BC_PKT_TXPKTS_SHIFT)) & ENET_RMON_T_BC_PKT_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_MC_PKT - Tx Multicast Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_MC_PKT_TXPKTS_MASK           (0xFFFFU)
#define ENET_RMON_T_MC_PKT_TXPKTS_SHIFT          (0U)
/*! TXPKTS - Multicast packets */
#define ENET_RMON_T_MC_PKT_TXPKTS(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_MC_PKT_TXPKTS_SHIFT)) & ENET_RMON_T_MC_PKT_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_CRC_ALIGN - Tx Packets with CRC/Align Error Statistic Register */
/*! @{ */

#define ENET_RMON_T_CRC_ALIGN_TXPKTS_MASK        (0xFFFFU)
#define ENET_RMON_T_CRC_ALIGN_TXPKTS_SHIFT       (0U)
/*! TXPKTS - Packets with CRC/align error */
#define ENET_RMON_T_CRC_ALIGN_TXPKTS(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_CRC_ALIGN_TXPKTS_SHIFT)) & ENET_RMON_T_CRC_ALIGN_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_UNDERSIZE - Tx Packets Less Than Bytes and Good CRC Statistic Register */
/*! @{ */

#define ENET_RMON_T_UNDERSIZE_TXPKTS_MASK        (0xFFFFU)
#define ENET_RMON_T_UNDERSIZE_TXPKTS_SHIFT       (0U)
/*! TXPKTS - Number of transmit packets less than 64 bytes with good CRC */
#define ENET_RMON_T_UNDERSIZE_TXPKTS(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_UNDERSIZE_TXPKTS_SHIFT)) & ENET_RMON_T_UNDERSIZE_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_OVERSIZE - Tx Packets GT MAX_FL bytes and Good CRC Statistic Register */
/*! @{ */

#define ENET_RMON_T_OVERSIZE_TXPKTS_MASK         (0xFFFFU)
#define ENET_RMON_T_OVERSIZE_TXPKTS_SHIFT        (0U)
/*! TXPKTS - Number of transmit packets greater than MAX_FL bytes with good CRC */
#define ENET_RMON_T_OVERSIZE_TXPKTS(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_OVERSIZE_TXPKTS_SHIFT)) & ENET_RMON_T_OVERSIZE_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_FRAG - Tx Packets Less Than 64 Bytes and Bad CRC Statistic Register */
/*! @{ */

#define ENET_RMON_T_FRAG_TXPKTS_MASK             (0xFFFFU)
#define ENET_RMON_T_FRAG_TXPKTS_SHIFT            (0U)
/*! TXPKTS - Number of packets less than 64 bytes with bad CRC */
#define ENET_RMON_T_FRAG_TXPKTS(x)               (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_FRAG_TXPKTS_SHIFT)) & ENET_RMON_T_FRAG_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_JAB - Tx Packets Greater Than MAX_FL bytes and Bad CRC Statistic Register */
/*! @{ */

#define ENET_RMON_T_JAB_TXPKTS_MASK              (0xFFFFU)
#define ENET_RMON_T_JAB_TXPKTS_SHIFT             (0U)
/*! TXPKTS - Number of transmit packets greater than MAX_FL bytes and bad CRC */
#define ENET_RMON_T_JAB_TXPKTS(x)                (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_JAB_TXPKTS_SHIFT)) & ENET_RMON_T_JAB_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_COL - Tx Collision Count Statistic Register */
/*! @{ */

#define ENET_RMON_T_COL_TXPKTS_MASK              (0xFFFFU)
#define ENET_RMON_T_COL_TXPKTS_SHIFT             (0U)
/*! TXPKTS - Number of transmit collisions */
#define ENET_RMON_T_COL_TXPKTS(x)                (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_COL_TXPKTS_SHIFT)) & ENET_RMON_T_COL_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P64 - Tx 64-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P64_TXPKTS_MASK              (0xFFFFU)
#define ENET_RMON_T_P64_TXPKTS_SHIFT             (0U)
/*! TXPKTS - Number of 64-byte transmit packets */
#define ENET_RMON_T_P64_TXPKTS(x)                (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P64_TXPKTS_SHIFT)) & ENET_RMON_T_P64_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P65TO127 - Tx 65- to 127-byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P65TO127_TXPKTS_MASK         (0xFFFFU)
#define ENET_RMON_T_P65TO127_TXPKTS_SHIFT        (0U)
/*! TXPKTS - Number of 65- to 127-byte transmit packets */
#define ENET_RMON_T_P65TO127_TXPKTS(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P65TO127_TXPKTS_SHIFT)) & ENET_RMON_T_P65TO127_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P128TO255 - Tx 128- to 255-byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P128TO255_TXPKTS_MASK        (0xFFFFU)
#define ENET_RMON_T_P128TO255_TXPKTS_SHIFT       (0U)
/*! TXPKTS - Number of 128- to 255-byte transmit packets */
#define ENET_RMON_T_P128TO255_TXPKTS(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P128TO255_TXPKTS_SHIFT)) & ENET_RMON_T_P128TO255_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P256TO511 - Tx 256- to 511-byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P256TO511_TXPKTS_MASK        (0xFFFFU)
#define ENET_RMON_T_P256TO511_TXPKTS_SHIFT       (0U)
/*! TXPKTS - Number of 256- to 511-byte transmit packets */
#define ENET_RMON_T_P256TO511_TXPKTS(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P256TO511_TXPKTS_SHIFT)) & ENET_RMON_T_P256TO511_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P512TO1023 - Tx 512- to 1023-byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P512TO1023_TXPKTS_MASK       (0xFFFFU)
#define ENET_RMON_T_P512TO1023_TXPKTS_SHIFT      (0U)
/*! TXPKTS - Number of 512- to 1023-byte transmit packets */
#define ENET_RMON_T_P512TO1023_TXPKTS(x)         (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P512TO1023_TXPKTS_SHIFT)) & ENET_RMON_T_P512TO1023_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P1024TO2047 - Tx 1024- to 2047-byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_T_P1024TO2047_TXPKTS_MASK      (0xFFFFU)
#define ENET_RMON_T_P1024TO2047_TXPKTS_SHIFT     (0U)
/*! TXPKTS - Number of 1024- to 2047-byte transmit packets */
#define ENET_RMON_T_P1024TO2047_TXPKTS(x)        (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P1024TO2047_TXPKTS_SHIFT)) & ENET_RMON_T_P1024TO2047_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_P_GTE2048 - Tx Packets Greater Than 2048 Bytes Statistic Register */
/*! @{ */

#define ENET_RMON_T_P_GTE2048_TXPKTS_MASK        (0xFFFFU)
#define ENET_RMON_T_P_GTE2048_TXPKTS_SHIFT       (0U)
/*! TXPKTS - Number of transmit packets greater than 2048 bytes */
#define ENET_RMON_T_P_GTE2048_TXPKTS(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_P_GTE2048_TXPKTS_SHIFT)) & ENET_RMON_T_P_GTE2048_TXPKTS_MASK)
/*! @} */

/*! @name RMON_T_OCTETS - Tx Octets Statistic Register */
/*! @{ */

#define ENET_RMON_T_OCTETS_TXOCTS_MASK           (0xFFFFFFFFU)
#define ENET_RMON_T_OCTETS_TXOCTS_SHIFT          (0U)
/*! TXOCTS - Number of transmit octets */
#define ENET_RMON_T_OCTETS_TXOCTS(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RMON_T_OCTETS_TXOCTS_SHIFT)) & ENET_RMON_T_OCTETS_TXOCTS_MASK)
/*! @} */

/*! @name IEEE_T_FRAME_OK - Frames Transmitted OK Statistic Register */
/*! @{ */

#define ENET_IEEE_T_FRAME_OK_COUNT_MASK          (0xFFFFU)
#define ENET_IEEE_T_FRAME_OK_COUNT_SHIFT         (0U)
/*! COUNT - Number of frames transmitted OK */
#define ENET_IEEE_T_FRAME_OK_COUNT(x)            (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_FRAME_OK_COUNT_SHIFT)) & ENET_IEEE_T_FRAME_OK_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_1COL - Frames Transmitted with Single Collision Statistic Register */
/*! @{ */

#define ENET_IEEE_T_1COL_COUNT_MASK              (0xFFFFU)
#define ENET_IEEE_T_1COL_COUNT_SHIFT             (0U)
/*! COUNT - Number of frames transmitted with one collision */
#define ENET_IEEE_T_1COL_COUNT(x)                (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_1COL_COUNT_SHIFT)) & ENET_IEEE_T_1COL_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_MCOL - Frames Transmitted with Multiple Collisions Statistic Register */
/*! @{ */

#define ENET_IEEE_T_MCOL_COUNT_MASK              (0xFFFFU)
#define ENET_IEEE_T_MCOL_COUNT_SHIFT             (0U)
/*! COUNT - Number of frames transmitted with multiple collisions */
#define ENET_IEEE_T_MCOL_COUNT(x)                (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_MCOL_COUNT_SHIFT)) & ENET_IEEE_T_MCOL_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_DEF - Frames Transmitted after Deferral Delay Statistic Register */
/*! @{ */

#define ENET_IEEE_T_DEF_COUNT_MASK               (0xFFFFU)
#define ENET_IEEE_T_DEF_COUNT_SHIFT              (0U)
/*! COUNT - Number of frames transmitted with deferral delay */
#define ENET_IEEE_T_DEF_COUNT(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_DEF_COUNT_SHIFT)) & ENET_IEEE_T_DEF_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_LCOL - Frames Transmitted with Late Collision Statistic Register */
/*! @{ */

#define ENET_IEEE_T_LCOL_COUNT_MASK              (0xFFFFU)
#define ENET_IEEE_T_LCOL_COUNT_SHIFT             (0U)
/*! COUNT - Number of frames transmitted with late collision */
#define ENET_IEEE_T_LCOL_COUNT(x)                (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_LCOL_COUNT_SHIFT)) & ENET_IEEE_T_LCOL_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_EXCOL - Frames Transmitted with Excessive Collisions Statistic Register */
/*! @{ */

#define ENET_IEEE_T_EXCOL_COUNT_MASK             (0xFFFFU)
#define ENET_IEEE_T_EXCOL_COUNT_SHIFT            (0U)
/*! COUNT - Number of frames transmitted with excessive collisions */
#define ENET_IEEE_T_EXCOL_COUNT(x)               (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_EXCOL_COUNT_SHIFT)) & ENET_IEEE_T_EXCOL_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_MACERR - Frames Transmitted with Tx FIFO Underrun Statistic Register */
/*! @{ */

#define ENET_IEEE_T_MACERR_COUNT_MASK            (0xFFFFU)
#define ENET_IEEE_T_MACERR_COUNT_SHIFT           (0U)
/*! COUNT - Number of frames transmitted with transmit FIFO underrun */
#define ENET_IEEE_T_MACERR_COUNT(x)              (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_MACERR_COUNT_SHIFT)) & ENET_IEEE_T_MACERR_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_CSERR - Frames Transmitted with Carrier Sense Error Statistic Register */
/*! @{ */

#define ENET_IEEE_T_CSERR_COUNT_MASK             (0xFFFFU)
#define ENET_IEEE_T_CSERR_COUNT_SHIFT            (0U)
/*! COUNT - Number of frames transmitted with carrier sense error */
#define ENET_IEEE_T_CSERR_COUNT(x)               (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_CSERR_COUNT_SHIFT)) & ENET_IEEE_T_CSERR_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_SQE - Reserved Statistic Register */
/*! @{ */

#define ENET_IEEE_T_SQE_COUNT_MASK               (0xFFFFU)
#define ENET_IEEE_T_SQE_COUNT_SHIFT              (0U)
/*! COUNT - This read-only field is reserved and always has the value 0 */
#define ENET_IEEE_T_SQE_COUNT(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_SQE_COUNT_SHIFT)) & ENET_IEEE_T_SQE_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_FDXFC - Flow Control Pause Frames Transmitted Statistic Register */
/*! @{ */

#define ENET_IEEE_T_FDXFC_COUNT_MASK             (0xFFFFU)
#define ENET_IEEE_T_FDXFC_COUNT_SHIFT            (0U)
/*! COUNT - Number of flow-control pause frames transmitted */
#define ENET_IEEE_T_FDXFC_COUNT(x)               (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_FDXFC_COUNT_SHIFT)) & ENET_IEEE_T_FDXFC_COUNT_MASK)
/*! @} */

/*! @name IEEE_T_OCTETS_OK - Octet Count for Frames Transmitted w/o Error Statistic Register */
/*! @{ */

#define ENET_IEEE_T_OCTETS_OK_COUNT_MASK         (0xFFFFFFFFU)
#define ENET_IEEE_T_OCTETS_OK_COUNT_SHIFT        (0U)
/*! COUNT - Octet count for frames transmitted without error Counts total octets (includes header and FCS fields). */
#define ENET_IEEE_T_OCTETS_OK_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_T_OCTETS_OK_COUNT_SHIFT)) & ENET_IEEE_T_OCTETS_OK_COUNT_MASK)
/*! @} */

/*! @name RMON_R_PACKETS - Rx Packet Count Statistic Register */
/*! @{ */

#define ENET_RMON_R_PACKETS_COUNT_MASK           (0xFFFFU)
#define ENET_RMON_R_PACKETS_COUNT_SHIFT          (0U)
/*! COUNT - Number of packets received */
#define ENET_RMON_R_PACKETS_COUNT(x)             (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_PACKETS_COUNT_SHIFT)) & ENET_RMON_R_PACKETS_COUNT_MASK)
/*! @} */

/*! @name RMON_R_BC_PKT - Rx Broadcast Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_BC_PKT_COUNT_MASK            (0xFFFFU)
#define ENET_RMON_R_BC_PKT_COUNT_SHIFT           (0U)
/*! COUNT - Number of receive broadcast packets */
#define ENET_RMON_R_BC_PKT_COUNT(x)              (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_BC_PKT_COUNT_SHIFT)) & ENET_RMON_R_BC_PKT_COUNT_MASK)
/*! @} */

/*! @name RMON_R_MC_PKT - Rx Multicast Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_MC_PKT_COUNT_MASK            (0xFFFFU)
#define ENET_RMON_R_MC_PKT_COUNT_SHIFT           (0U)
/*! COUNT - Number of receive multicast packets */
#define ENET_RMON_R_MC_PKT_COUNT(x)              (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_MC_PKT_COUNT_SHIFT)) & ENET_RMON_R_MC_PKT_COUNT_MASK)
/*! @} */

/*! @name RMON_R_CRC_ALIGN - Rx Packets with CRC/Align Error Statistic Register */
/*! @{ */

#define ENET_RMON_R_CRC_ALIGN_COUNT_MASK         (0xFFFFU)
#define ENET_RMON_R_CRC_ALIGN_COUNT_SHIFT        (0U)
/*! COUNT - Number of receive packets with CRC or align error */
#define ENET_RMON_R_CRC_ALIGN_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_CRC_ALIGN_COUNT_SHIFT)) & ENET_RMON_R_CRC_ALIGN_COUNT_MASK)
/*! @} */

/*! @name RMON_R_UNDERSIZE - Rx Packets with Less Than 64 Bytes and Good CRC Statistic Register */
/*! @{ */

#define ENET_RMON_R_UNDERSIZE_COUNT_MASK         (0xFFFFU)
#define ENET_RMON_R_UNDERSIZE_COUNT_SHIFT        (0U)
/*! COUNT - Number of receive packets with less than 64 bytes and good CRC */
#define ENET_RMON_R_UNDERSIZE_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_UNDERSIZE_COUNT_SHIFT)) & ENET_RMON_R_UNDERSIZE_COUNT_MASK)
/*! @} */

/*! @name RMON_R_OVERSIZE - Rx Packets Greater Than MAX_FL and Good CRC Statistic Register */
/*! @{ */

#define ENET_RMON_R_OVERSIZE_COUNT_MASK          (0xFFFFU)
#define ENET_RMON_R_OVERSIZE_COUNT_SHIFT         (0U)
/*! COUNT - Number of receive packets greater than MAX_FL and good CRC */
#define ENET_RMON_R_OVERSIZE_COUNT(x)            (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_OVERSIZE_COUNT_SHIFT)) & ENET_RMON_R_OVERSIZE_COUNT_MASK)
/*! @} */

/*! @name RMON_R_FRAG - Rx Packets Less Than 64 Bytes and Bad CRC Statistic Register */
/*! @{ */

#define ENET_RMON_R_FRAG_COUNT_MASK              (0xFFFFU)
#define ENET_RMON_R_FRAG_COUNT_SHIFT             (0U)
/*! COUNT - Number of receive packets with less than 64 bytes and bad CRC */
#define ENET_RMON_R_FRAG_COUNT(x)                (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_FRAG_COUNT_SHIFT)) & ENET_RMON_R_FRAG_COUNT_MASK)
/*! @} */

/*! @name RMON_R_JAB - Rx Packets Greater Than MAX_FL Bytes and Bad CRC Statistic Register */
/*! @{ */

#define ENET_RMON_R_JAB_COUNT_MASK               (0xFFFFU)
#define ENET_RMON_R_JAB_COUNT_SHIFT              (0U)
/*! COUNT - Number of receive packets greater than MAX_FL and bad CRC */
#define ENET_RMON_R_JAB_COUNT(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_JAB_COUNT_SHIFT)) & ENET_RMON_R_JAB_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P64 - Rx 64-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P64_COUNT_MASK               (0xFFFFU)
#define ENET_RMON_R_P64_COUNT_SHIFT              (0U)
/*! COUNT - Number of 64-byte receive packets */
#define ENET_RMON_R_P64_COUNT(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P64_COUNT_SHIFT)) & ENET_RMON_R_P64_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P65TO127 - Rx 65- to 127-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P65TO127_COUNT_MASK          (0xFFFFU)
#define ENET_RMON_R_P65TO127_COUNT_SHIFT         (0U)
/*! COUNT - Number of 65- to 127-byte recieve packets */
#define ENET_RMON_R_P65TO127_COUNT(x)            (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P65TO127_COUNT_SHIFT)) & ENET_RMON_R_P65TO127_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P128TO255 - Rx 128- to 255-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P128TO255_COUNT_MASK         (0xFFFFU)
#define ENET_RMON_R_P128TO255_COUNT_SHIFT        (0U)
/*! COUNT - Number of 128- to 255-byte recieve packets */
#define ENET_RMON_R_P128TO255_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P128TO255_COUNT_SHIFT)) & ENET_RMON_R_P128TO255_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P256TO511 - Rx 256- to 511-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P256TO511_COUNT_MASK         (0xFFFFU)
#define ENET_RMON_R_P256TO511_COUNT_SHIFT        (0U)
/*! COUNT - Number of 256- to 511-byte recieve packets */
#define ENET_RMON_R_P256TO511_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P256TO511_COUNT_SHIFT)) & ENET_RMON_R_P256TO511_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P512TO1023 - Rx 512- to 1023-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P512TO1023_COUNT_MASK        (0xFFFFU)
#define ENET_RMON_R_P512TO1023_COUNT_SHIFT       (0U)
/*! COUNT - Number of 512- to 1023-byte recieve packets */
#define ENET_RMON_R_P512TO1023_COUNT(x)          (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P512TO1023_COUNT_SHIFT)) & ENET_RMON_R_P512TO1023_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P1024TO2047 - Rx 1024- to 2047-Byte Packets Statistic Register */
/*! @{ */

#define ENET_RMON_R_P1024TO2047_COUNT_MASK       (0xFFFFU)
#define ENET_RMON_R_P1024TO2047_COUNT_SHIFT      (0U)
/*! COUNT - Number of 1024- to 2047-byte recieve packets */
#define ENET_RMON_R_P1024TO2047_COUNT(x)         (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P1024TO2047_COUNT_SHIFT)) & ENET_RMON_R_P1024TO2047_COUNT_MASK)
/*! @} */

/*! @name RMON_R_P_GTE2048 - Rx Packets Greater than 2048 Bytes Statistic Register */
/*! @{ */

#define ENET_RMON_R_P_GTE2048_COUNT_MASK         (0xFFFFU)
#define ENET_RMON_R_P_GTE2048_COUNT_SHIFT        (0U)
/*! COUNT - Number of greater-than-2048-byte recieve packets */
#define ENET_RMON_R_P_GTE2048_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_P_GTE2048_COUNT_SHIFT)) & ENET_RMON_R_P_GTE2048_COUNT_MASK)
/*! @} */

/*! @name RMON_R_OCTETS - Rx Octets Statistic Register */
/*! @{ */

#define ENET_RMON_R_OCTETS_COUNT_MASK            (0xFFFFFFFFU)
#define ENET_RMON_R_OCTETS_COUNT_SHIFT           (0U)
/*! COUNT - Number of receive octets */
#define ENET_RMON_R_OCTETS_COUNT(x)              (((uint32_t)(((uint32_t)(x)) << ENET_RMON_R_OCTETS_COUNT_SHIFT)) & ENET_RMON_R_OCTETS_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_DROP - Frames not Counted Correctly Statistic Register */
/*! @{ */

#define ENET_IEEE_R_DROP_COUNT_MASK              (0xFFFFU)
#define ENET_IEEE_R_DROP_COUNT_SHIFT             (0U)
/*! COUNT - Frame count */
#define ENET_IEEE_R_DROP_COUNT(x)                (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_DROP_COUNT_SHIFT)) & ENET_IEEE_R_DROP_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_FRAME_OK - Frames Received OK Statistic Register */
/*! @{ */

#define ENET_IEEE_R_FRAME_OK_COUNT_MASK          (0xFFFFU)
#define ENET_IEEE_R_FRAME_OK_COUNT_SHIFT         (0U)
/*! COUNT - Number of frames received OK */
#define ENET_IEEE_R_FRAME_OK_COUNT(x)            (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_FRAME_OK_COUNT_SHIFT)) & ENET_IEEE_R_FRAME_OK_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_CRC - Frames Received with CRC Error Statistic Register */
/*! @{ */

#define ENET_IEEE_R_CRC_COUNT_MASK               (0xFFFFU)
#define ENET_IEEE_R_CRC_COUNT_SHIFT              (0U)
/*! COUNT - Number of frames received with CRC error */
#define ENET_IEEE_R_CRC_COUNT(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_CRC_COUNT_SHIFT)) & ENET_IEEE_R_CRC_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_ALIGN - Frames Received with Alignment Error Statistic Register */
/*! @{ */

#define ENET_IEEE_R_ALIGN_COUNT_MASK             (0xFFFFU)
#define ENET_IEEE_R_ALIGN_COUNT_SHIFT            (0U)
/*! COUNT - Number of frames received with alignment error */
#define ENET_IEEE_R_ALIGN_COUNT(x)               (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_ALIGN_COUNT_SHIFT)) & ENET_IEEE_R_ALIGN_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_MACERR - Receive FIFO Overflow Count Statistic Register */
/*! @{ */

#define ENET_IEEE_R_MACERR_COUNT_MASK            (0xFFFFU)
#define ENET_IEEE_R_MACERR_COUNT_SHIFT           (0U)
/*! COUNT - Receive FIFO overflow count */
#define ENET_IEEE_R_MACERR_COUNT(x)              (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_MACERR_COUNT_SHIFT)) & ENET_IEEE_R_MACERR_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_FDXFC - Flow Control Pause Frames Received Statistic Register */
/*! @{ */

#define ENET_IEEE_R_FDXFC_COUNT_MASK             (0xFFFFU)
#define ENET_IEEE_R_FDXFC_COUNT_SHIFT            (0U)
/*! COUNT - Number of flow-control pause frames received */
#define ENET_IEEE_R_FDXFC_COUNT(x)               (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_FDXFC_COUNT_SHIFT)) & ENET_IEEE_R_FDXFC_COUNT_MASK)
/*! @} */

/*! @name IEEE_R_OCTETS_OK - Octet Count for Frames Received without Error Statistic Register */
/*! @{ */

#define ENET_IEEE_R_OCTETS_OK_COUNT_MASK         (0xFFFFFFFFU)
#define ENET_IEEE_R_OCTETS_OK_COUNT_SHIFT        (0U)
/*! COUNT - Number of octets for frames received without error */
#define ENET_IEEE_R_OCTETS_OK_COUNT(x)           (((uint32_t)(((uint32_t)(x)) << ENET_IEEE_R_OCTETS_OK_COUNT_SHIFT)) & ENET_IEEE_R_OCTETS_OK_COUNT_MASK)
/*! @} */

/*! @name ATCR - Adjustable Timer Control Register */
/*! @{ */

#define ENET_ATCR_EN_MASK                        (0x1U)
#define ENET_ATCR_EN_SHIFT                       (0U)
/*! EN - Enable Timer
 *  0b0..The timer stops at the current value.
 *  0b1..The timer starts incrementing.
 */
#define ENET_ATCR_EN(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_EN_SHIFT)) & ENET_ATCR_EN_MASK)

#define ENET_ATCR_OFFEN_MASK                     (0x4U)
#define ENET_ATCR_OFFEN_SHIFT                    (2U)
/*! OFFEN - Enable One-Shot Offset Event
 *  0b0..Disable.
 *  0b1..The timer can be reset to zero when the given offset time is reached (offset event). The field is cleared
 *       when the offset event is reached, so no further event occurs until the field is set again. The timer
 *       offset value must be set before setting this field.
 */
#define ENET_ATCR_OFFEN(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_OFFEN_SHIFT)) & ENET_ATCR_OFFEN_MASK)

#define ENET_ATCR_OFFRST_MASK                    (0x8U)
#define ENET_ATCR_OFFRST_SHIFT                   (3U)
/*! OFFRST - Reset Timer On Offset Event
 *  0b0..The timer is not affected and no action occurs, besides clearing OFFEN, when the offset is reached.
 *  0b1..If OFFEN is set, the timer resets to zero when the offset setting is reached. The offset event does not cause a timer interrupt.
 */
#define ENET_ATCR_OFFRST(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_OFFRST_SHIFT)) & ENET_ATCR_OFFRST_MASK)

#define ENET_ATCR_PEREN_MASK                     (0x10U)
#define ENET_ATCR_PEREN_SHIFT                    (4U)
/*! PEREN - Enable Periodical Event
 *  0b0..Disable.
 *  0b1..A period event interrupt can be generated (EIR[TS_TIMER]) and the event signal output is asserted when
 *       the timer wraps around according to the periodic setting ATPER. The timer period value must be set before
 *       setting this bit. Not all devices contain the event signal output. See the chip configuration details.
 */
#define ENET_ATCR_PEREN(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_PEREN_SHIFT)) & ENET_ATCR_PEREN_MASK)

#define ENET_ATCR_PINPER_MASK                    (0x80U)
#define ENET_ATCR_PINPER_SHIFT                   (7U)
/*! PINPER - Enables event signal output external pin frc_evt_period assertion on period event
 *  0b0..Disable.
 *  0b1..Enable.
 */
#define ENET_ATCR_PINPER(x)                      (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_PINPER_SHIFT)) & ENET_ATCR_PINPER_MASK)

#define ENET_ATCR_RESTART_MASK                   (0x200U)
#define ENET_ATCR_RESTART_SHIFT                  (9U)
/*! RESTART - Reset Timer */
#define ENET_ATCR_RESTART(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_RESTART_SHIFT)) & ENET_ATCR_RESTART_MASK)

#define ENET_ATCR_CAPTURE_MASK                   (0x800U)
#define ENET_ATCR_CAPTURE_SHIFT                  (11U)
/*! CAPTURE - Capture Timer Value
 *  0b0..No effect.
 *  0b1..The current time is captured and can be read from the ATVR register.
 */
#define ENET_ATCR_CAPTURE(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_CAPTURE_SHIFT)) & ENET_ATCR_CAPTURE_MASK)

#define ENET_ATCR_SLAVE_MASK                     (0x2000U)
#define ENET_ATCR_SLAVE_SHIFT                    (13U)
/*! SLAVE - Enable Timer Slave Mode
 *  0b0..The timer is active and all configuration fields in this register are relevant.
 *  0b1..The internal timer is disabled and the externally provided timer value is used. All other fields, except
 *       CAPTURE, in this register have no effect. CAPTURE can still be used to capture the current timer value.
 */
#define ENET_ATCR_SLAVE(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_ATCR_SLAVE_SHIFT)) & ENET_ATCR_SLAVE_MASK)
/*! @} */

/*! @name ATVR - Timer Value Register */
/*! @{ */

#define ENET_ATVR_ATIME_MASK                     (0xFFFFFFFFU)
#define ENET_ATVR_ATIME_SHIFT                    (0U)
#define ENET_ATVR_ATIME(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_ATVR_ATIME_SHIFT)) & ENET_ATVR_ATIME_MASK)
/*! @} */

/*! @name ATOFF - Timer Offset Register */
/*! @{ */

#define ENET_ATOFF_OFFSET_MASK                   (0xFFFFFFFFU)
#define ENET_ATOFF_OFFSET_SHIFT                  (0U)
#define ENET_ATOFF_OFFSET(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_ATOFF_OFFSET_SHIFT)) & ENET_ATOFF_OFFSET_MASK)
/*! @} */

/*! @name ATPER - Timer Period Register */
/*! @{ */

#define ENET_ATPER_PERIOD_MASK                   (0xFFFFFFFFU)
#define ENET_ATPER_PERIOD_SHIFT                  (0U)
/*! PERIOD - Value for generating periodic events */
#define ENET_ATPER_PERIOD(x)                     (((uint32_t)(((uint32_t)(x)) << ENET_ATPER_PERIOD_SHIFT)) & ENET_ATPER_PERIOD_MASK)
/*! @} */

/*! @name ATCOR - Timer Correction Register */
/*! @{ */

#define ENET_ATCOR_COR_MASK                      (0x7FFFFFFFU)
#define ENET_ATCOR_COR_SHIFT                     (0U)
/*! COR - Correction Counter Wrap-Around Value */
#define ENET_ATCOR_COR(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ATCOR_COR_SHIFT)) & ENET_ATCOR_COR_MASK)
/*! @} */

/*! @name ATINC - Time-Stamping Clock Period Register */
/*! @{ */

#define ENET_ATINC_INC_MASK                      (0x7FU)
#define ENET_ATINC_INC_SHIFT                     (0U)
/*! INC - Clock Period Of The Timestamping Clock (ts_clk) In Nanoseconds */
#define ENET_ATINC_INC(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_ATINC_INC_SHIFT)) & ENET_ATINC_INC_MASK)

#define ENET_ATINC_INC_CORR_MASK                 (0x7F00U)
#define ENET_ATINC_INC_CORR_SHIFT                (8U)
/*! INC_CORR - Correction Increment Value */
#define ENET_ATINC_INC_CORR(x)                   (((uint32_t)(((uint32_t)(x)) << ENET_ATINC_INC_CORR_SHIFT)) & ENET_ATINC_INC_CORR_MASK)
/*! @} */

/*! @name ATSTMP - Timestamp of Last Transmitted Frame */
/*! @{ */

#define ENET_ATSTMP_TIMESTAMP_MASK               (0xFFFFFFFFU)
#define ENET_ATSTMP_TIMESTAMP_SHIFT              (0U)
/*! TIMESTAMP - Timestamp of the last frame transmitted by the core that had TxBD[TS] set the
 *    ff_tx_ts_frm signal asserted from the user application
 */
#define ENET_ATSTMP_TIMESTAMP(x)                 (((uint32_t)(((uint32_t)(x)) << ENET_ATSTMP_TIMESTAMP_SHIFT)) & ENET_ATSTMP_TIMESTAMP_MASK)
/*! @} */

/*! @name TGSR - Timer Global Status Register */
/*! @{ */

#define ENET_TGSR_TF0_MASK                       (0x1U)
#define ENET_TGSR_TF0_SHIFT                      (0U)
/*! TF0 - Copy Of Timer Flag For Channel 0
 *  0b0..Timer Flag for Channel 0 is clear
 *  0b1..Timer Flag for Channel 0 is set
 */
#define ENET_TGSR_TF0(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TGSR_TF0_SHIFT)) & ENET_TGSR_TF0_MASK)

#define ENET_TGSR_TF1_MASK                       (0x2U)
#define ENET_TGSR_TF1_SHIFT                      (1U)
/*! TF1 - Copy Of Timer Flag For Channel 1
 *  0b0..Timer Flag for Channel 1 is clear
 *  0b1..Timer Flag for Channel 1 is set
 */
#define ENET_TGSR_TF1(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TGSR_TF1_SHIFT)) & ENET_TGSR_TF1_MASK)

#define ENET_TGSR_TF2_MASK                       (0x4U)
#define ENET_TGSR_TF2_SHIFT                      (2U)
/*! TF2 - Copy Of Timer Flag For Channel 2
 *  0b0..Timer Flag for Channel 2 is clear
 *  0b1..Timer Flag for Channel 2 is set
 */
#define ENET_TGSR_TF2(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TGSR_TF2_SHIFT)) & ENET_TGSR_TF2_MASK)

#define ENET_TGSR_TF3_MASK                       (0x8U)
#define ENET_TGSR_TF3_SHIFT                      (3U)
/*! TF3 - Copy Of Timer Flag For Channel 3
 *  0b0..Timer Flag for Channel 3 is clear
 *  0b1..Timer Flag for Channel 3 is set
 */
#define ENET_TGSR_TF3(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TGSR_TF3_SHIFT)) & ENET_TGSR_TF3_MASK)
/*! @} */

/*! @name TCSR - Timer Control Status Register */
/*! @{ */

#define ENET_TCSR_TDRE_MASK                      (0x1U)
#define ENET_TCSR_TDRE_SHIFT                     (0U)
/*! TDRE - Timer DMA Request Enable
 *  0b0..DMA request is disabled
 *  0b1..DMA request is enabled
 */
#define ENET_TCSR_TDRE(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TCSR_TDRE_SHIFT)) & ENET_TCSR_TDRE_MASK)

#define ENET_TCSR_TMODE_MASK                     (0x3CU)
#define ENET_TCSR_TMODE_SHIFT                    (2U)
/*! TMODE - Timer Mode
 *  0b0000..Timer Channel is disabled.
 *  0b0001..Timer Channel is configured for Input Capture on rising edge.
 *  0b0010..Timer Channel is configured for Input Capture on falling edge.
 *  0b0011..Timer Channel is configured for Input Capture on both edges.
 *  0b0100..Timer Channel is configured for Output Compare - software only.
 *  0b0101..Timer Channel is configured for Output Compare - toggle output on compare.
 *  0b0110..Timer Channel is configured for Output Compare - clear output on compare.
 *  0b0111..Timer Channel is configured for Output Compare - set output on compare.
 *  0b1000..Reserved
 *  0b1010..Timer Channel is configured for Output Compare - clear output on compare, set output on overflow.
 *  0b10x1..Timer Channel is configured for Output Compare - set output on compare, clear output on overflow.
 *  0b110x..Reserved
 *  0b1110..Timer Channel is configured for Output Compare - pulse output low on compare for 1 to 32 1588-clock cycles as specified by TPWC.
 *  0b1111..Timer Channel is configured for Output Compare - pulse output high on compare for 1 to 32 1588-clock cycles as specified by TPWC.
 */
#define ENET_TCSR_TMODE(x)                       (((uint32_t)(((uint32_t)(x)) << ENET_TCSR_TMODE_SHIFT)) & ENET_TCSR_TMODE_MASK)

#define ENET_TCSR_TIE_MASK                       (0x40U)
#define ENET_TCSR_TIE_SHIFT                      (6U)
/*! TIE - Timer Interrupt Enable
 *  0b0..Interrupt is disabled
 *  0b1..Interrupt is enabled
 */
#define ENET_TCSR_TIE(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TCSR_TIE_SHIFT)) & ENET_TCSR_TIE_MASK)

#define ENET_TCSR_TF_MASK                        (0x80U)
#define ENET_TCSR_TF_SHIFT                       (7U)
/*! TF - Timer Flag
 *  0b0..Input Capture or Output Compare has not occurred.
 *  0b1..Input Capture or Output Compare has occurred.
 */
#define ENET_TCSR_TF(x)                          (((uint32_t)(((uint32_t)(x)) << ENET_TCSR_TF_SHIFT)) & ENET_TCSR_TF_MASK)

#define ENET_TCSR_TPWC_MASK                      (0xF800U)
#define ENET_TCSR_TPWC_SHIFT                     (11U)
/*! TPWC - Timer PulseWidth Control
 *  0b00000..Pulse width is one 1588-clock cycle.
 *  0b00001..Pulse width is two 1588-clock cycles.
 *  0b00010..Pulse width is three 1588-clock cycles.
 *  0b00011..Pulse width is four 1588-clock cycles.
 *  0b11111..Pulse width is 32 1588-clock cycles.
 */
#define ENET_TCSR_TPWC(x)                        (((uint32_t)(((uint32_t)(x)) << ENET_TCSR_TPWC_SHIFT)) & ENET_TCSR_TPWC_MASK)
/*! @} */

/* The count of ENET_TCSR */
#define ENET_TCSR_COUNT                          (4U)

/*! @name TCCR - Timer Compare Capture Register */
/*! @{ */

#define ENET_TCCR_TCC_MASK                       (0xFFFFFFFFU)
#define ENET_TCCR_TCC_SHIFT                      (0U)
/*! TCC - Timer Capture Compare */
#define ENET_TCCR_TCC(x)                         (((uint32_t)(((uint32_t)(x)) << ENET_TCCR_TCC_SHIFT)) & ENET_TCCR_TCC_MASK)
/*! @} */

/* The count of ENET_TCCR */
#define ENET_TCCR_COUNT                          (4U)


/*!
 * @}
 */ /* end of group ENET_Register_Masks */

/* ENET Buffer Descriptor and Buffer Address Alignment. */
#define ENET_BUFF_ALIGNMENT                      (64U)


/*!
 * @}
 */ /* end of group ENET_Peripheral_Access_Layer */


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


#endif  /* ENET_H_ */

