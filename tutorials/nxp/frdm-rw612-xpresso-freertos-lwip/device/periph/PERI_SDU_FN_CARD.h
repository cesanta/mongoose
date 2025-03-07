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
**         CMSIS Peripheral Access Layer for SDU_FN_CARD
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
 * @file SDU_FN_CARD.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SDU_FN_CARD
 *
 * CMSIS Peripheral Access Layer for SDU_FN_CARD
 */

#if !defined(SDU_FN_CARD_H_)
#define SDU_FN_CARD_H_                           /**< Symbol preventing repeated inclusion */

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
   -- SDU_FN_CARD Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FN_CARD_Peripheral_Access_Layer SDU_FN_CARD Peripheral Access Layer
 * @{
 */

/** SDU_FN_CARD - Register Layout Typedef */
typedef struct {
  __I  uint8_t H2C_INTEVENT;                       /**< Host to Card Interrupt Event, offset: 0x0 */
       uint8_t RESERVED_0[3];
  __I  uint8_t HOST_INTRSR0;                       /**< Host Interrupt Reset Select 0, offset: 0x4 */
  __I  uint8_t HOST_INTRSR1;                       /**< Host Interrupt Reset Select 1, offset: 0x5 */
       uint8_t RESERVED_1[2];
  __I  uint8_t HOST_INTMASK0;                      /**< Host Interrupt Mask 0, offset: 0x8 */
  __I  uint8_t HOST_INTMASK1;                      /**< Host Interrupt Mask 1, offset: 0x9 */
       uint8_t RESERVED_2[2];
  __I  uint8_t HOST_INTSTATUS0;                    /**< Host Interrupt Status 0, offset: 0xC */
  __I  uint8_t HOST_INTSTATUS1;                    /**< Host Interrupt Status 1, offset: 0xD */
       uint8_t RESERVED_3[2];
  __IO uint8_t PKT_RD_BITMAP0;                     /**< Packet Read Bitmap 0, offset: 0x10 */
  __IO uint8_t PKT_RD_BITMAP1;                     /**< Packet Read Bitmap 1, offset: 0x11 */
  __IO uint8_t PKT_RD_BITMAP2;                     /**< Packet Read Bitmap 2, offset: 0x12 */
  __IO uint8_t PKT_RD_BITMAP3;                     /**< Packet Read Bitmap 3, offset: 0x13 */
  __IO uint8_t PKT_WR_BITMAP0;                     /**< Packet Write Bitmap 0, offset: 0x14 */
  __IO uint8_t PKT_WR_BITMAP1;                     /**< Packet Write Bitmap 1, offset: 0x15 */
  __IO uint8_t PKT_WR_BITMAP2;                     /**< Packet Write Bitmap 2, offset: 0x16 */
  __IO uint8_t PKT_WR_BITMAP3;                     /**< Packet Write Bitmap 3, offset: 0x17 */
  __IO uint8_t PORT0_RD_LEN0;                      /**< Port 0 Packet Read Length 0, offset: 0x18 */
  __IO uint8_t PORT0_RD_LEN1;                      /**< Port 0 Packet Read Length 1, offset: 0x19 */
  __IO uint8_t PORT1_RD_LEN0;                      /**< Port 1 Packet Read Length 0, offset: 0x1A */
  __IO uint8_t PORT1_RD_LEN1;                      /**< Port 1 Packet Read Length 1, offset: 0x1B */
  __IO uint8_t PORT2_RD_LEN0;                      /**< Port 2 Packet Read Length 0, offset: 0x1C */
  __IO uint8_t PORT2_RD_LEN1;                      /**< Port 2 Packet Read Length 1, offset: 0x1D */
  __IO uint8_t PORT3_RD_LEN0;                      /**< Port 3 Packet Read Length 0, offset: 0x1E */
  __IO uint8_t PORT3_RD_LEN1;                      /**< Port 3 Packet Read Length 1, offset: 0x1F */
  __IO uint8_t PORT4_RD_LEN0;                      /**< Port 4 Packet Read Length 0, offset: 0x20 */
  __IO uint8_t PORT4_RD_LEN1;                      /**< Port 4 Packet Read Length 1, offset: 0x21 */
  __IO uint8_t PORT5_RD_LEN0;                      /**< Port 5 Packet Read Length 0, offset: 0x22 */
  __IO uint8_t PORT5_RD_LEN1;                      /**< Port 5 Packet Read Length 1, offset: 0x23 */
  __IO uint8_t PORT6_RD_LEN0;                      /**< Port 6 Packet Read Length 0, offset: 0x24 */
  __IO uint8_t PORT6_RD_LEN1;                      /**< Port 6 Packet Read Length 1, offset: 0x25 */
  __IO uint8_t PORT7_RD_LEN0;                      /**< Port 7 Packet Read Length 0, offset: 0x26 */
  __IO uint8_t PORT7_RD_LEN1;                      /**< Port 7 Packet Read Length 1, offset: 0x27 */
  __IO uint8_t PORT8_RD_LEN0;                      /**< Port 8 Packet Read Length 0, offset: 0x28 */
  __IO uint8_t PORT8_RD_LEN1;                      /**< Port 8 Packet Read Length 1, offset: 0x29 */
  __IO uint8_t PORT9_RD_LEN0;                      /**< Port 9 Packet Read Length 0, offset: 0x2A */
  __IO uint8_t PORT9_RD_LEN1;                      /**< Port 9 Packet Read Length 1, offset: 0x2B */
  __IO uint8_t PORT10_RD_LEN0;                     /**< Port 10 Packet Read Length 0, offset: 0x2C */
  __IO uint8_t PORT10_RD_LEN1;                     /**< Port 10 Packet Read Length 1, offset: 0x2D */
  __IO uint8_t PORT11_RD_LEN0;                     /**< Port 11 Packet Read Length 0, offset: 0x2E */
  __IO uint8_t PORT11_RD_LEN1;                     /**< Port 11 Packet Read Length 1, offset: 0x2F */
  __IO uint8_t PORT12_RD_LEN0;                     /**< Port 12 Packet Read Length 0, offset: 0x30 */
  __IO uint8_t PORT12_RD_LEN1;                     /**< Port 12 Packet Read Length 1, offset: 0x31 */
  __IO uint8_t PORT13_RD_LEN0;                     /**< Port 13 Packet Read Length 0, offset: 0x32 */
  __IO uint8_t PORT13_RD_LEN1;                     /**< Port 13 Packet Read Length 1, offset: 0x33 */
  __IO uint8_t PORT14_RD_LEN0;                     /**< Port 14 Packet Read Length 0, offset: 0x34 */
  __IO uint8_t PORT14_RD_LEN1;                     /**< Port 14 Packet Read Length 1, offset: 0x35 */
  __IO uint8_t PORT15_RD_LEN0;                     /**< Port 15 Packet Read Length 0, offset: 0x36 */
  __IO uint8_t PORT15_RD_LEN1;                     /**< Port 15 Packet Read Length 1, offset: 0x37 */
  __IO uint8_t PORT16_RD_LEN0;                     /**< Port 16 Packet Read Length 0, offset: 0x38 */
  __IO uint8_t PORT16_RD_LEN1;                     /**< Port 16 Packet Read Length 1, offset: 0x39 */
  __IO uint8_t PORT17_RD_LEN0;                     /**< Port 17 Packet Read Length 0, offset: 0x3A */
  __IO uint8_t PORT17_RD_LEN1;                     /**< Port 17 Packet Read Length 1, offset: 0x3B */
  __IO uint8_t PORT18_RD_LEN0;                     /**< Port 18 Packet Read Length 0, offset: 0x3C */
  __IO uint8_t PORT18_RD_LEN1;                     /**< Port 18 Packet Read Length 1, offset: 0x3D */
  __IO uint8_t PORT19_RD_LEN0;                     /**< Port 19 Packet Read Length 0, offset: 0x3E */
  __IO uint8_t PORT19_RD_LEN1;                     /**< Port 19 Packet Read Length 1, offset: 0x3F */
  __IO uint8_t PORT20_RD_LEN0;                     /**< Port 20 Packet Read Length 0, offset: 0x40 */
  __IO uint8_t PORT20_RD_LEN1;                     /**< Port 20 Packet Read Length 1, offset: 0x41 */
  __IO uint8_t PORT21_RD_LEN0;                     /**< Port 21 Packet Read Length 0, offset: 0x42 */
  __IO uint8_t PORT21_RD_LEN1;                     /**< Port 21 Packet Read Length 1, offset: 0x43 */
  __IO uint8_t PORT22_RD_LEN0;                     /**< Port 22 Packet Read Length 0, offset: 0x44 */
  __IO uint8_t PORT22_RD_LEN1;                     /**< Port 22 Packet Read Length 1, offset: 0x45 */
  __IO uint8_t PORT23_RD_LEN0;                     /**< Port 23 Packet Read Length 0, offset: 0x46 */
  __IO uint8_t PORT23_RD_LEN1;                     /**< Port 23 Packet Read Length 1, offset: 0x47 */
  __IO uint8_t PORT24_RD_LEN0;                     /**< Port 24 Packet Read Length 0, offset: 0x48 */
  __IO uint8_t PORT24_RD_LEN1;                     /**< Port 24 Packet Read Length 1, offset: 0x49 */
  __IO uint8_t PORT25_RD_LEN0;                     /**< Port 25 Packet Read Length 0, offset: 0x4A */
  __IO uint8_t PORT25_RD_LEN1;                     /**< Port 25 Packet Read Length 1, offset: 0x4B */
  __IO uint8_t PORT26_RD_LEN0;                     /**< Port 26 Packet Read Length 0, offset: 0x4C */
  __IO uint8_t PORT26_RD_LEN1;                     /**< Port 26 Packet Read Length 1, offset: 0x4D */
  __IO uint8_t PORT27_RD_LEN0;                     /**< Port 27 Packet Read Length 0, offset: 0x4E */
  __IO uint8_t PORT27_RD_LEN1;                     /**< Port 27 Packet Read Length 1, offset: 0x4F */
  __IO uint8_t PORT28_RD_LEN0;                     /**< Port 28 Packet Read Length 0, offset: 0x50 */
  __IO uint8_t PORT28_RD_LEN1;                     /**< Port 28 Packet Read Length 1, offset: 0x51 */
  __IO uint8_t PORT29_RD_LEN0;                     /**< Port 29 Packet Read Length 0, offset: 0x52 */
  __IO uint8_t PORT29_RD_LEN1;                     /**< Port 29 Packet Read Length 1, offset: 0x53 */
  __IO uint8_t PORT30_RD_LEN0;                     /**< Port 30 Packet Read Length 0, offset: 0x54 */
  __IO uint8_t PORT30_RD_LEN1;                     /**< Port 30 Packet Read Length 1, offset: 0x55 */
  __IO uint8_t PORT31_RD_LEN0;                     /**< Port 31 Packet Read Length 0, offset: 0x56 */
  __IO uint8_t PORT31_RD_LEN1;                     /**< Port 31 Packet Read Length 1, offset: 0x57 */
  __I  uint8_t HOST_RESTART;                       /**< Host Transfer Status, offset: 0x58 */
  __IO uint8_t FN_CARD_INTMASK;                    /**< Function Card Interrupt Mask, offset: 0x59 */
  __IO uint8_t Q_PRT_RANGE0;                       /**< Queue Port Range 0, offset: 0x5A */
  __IO uint8_t Q_PRT_RANGE1;                       /**< Queue Port Range 1, offset: 0x5B */
  __IO uint8_t C2H_INTEVENT0;                      /**< Card to Host Event 0, offset: 0x5C */
  __IO uint8_t C2H_INTEVENT1;                      /**< Card to Host Event 1, offset: 0x5D */
       uint8_t RESERVED_4[2];
  __IO uint8_t CARD_INTMASK0;                      /**< Card Interrupt Mask 0, offset: 0x60 */
  __IO uint8_t CARD_INTMASK1;                      /**< Card Interrupt Mask 1, offset: 0x61 */
  __IO uint8_t CARD_INTMASK2;                      /**< Card Interrupt Mask 2, offset: 0x62 */
       uint8_t RESERVED_5[1];
  __I  uint8_t CARD_INTSTATUS0;                    /**< Card Interrupt Status 0, offset: 0x64 */
  __I  uint8_t CARD_INTSTATUS1;                    /**< Card Interrupt Status 1, offset: 0x65 */
  __I  uint8_t CARD_INTSTATUS2;                    /**< Card Interrupt Status 2, offset: 0x66 */
       uint8_t RESERVED_6[1];
  __IO uint8_t CARD_INTRSR0;                       /**< Card Interrupt Reset Select 0, offset: 0x68 */
  __IO uint8_t CARD_INTRSR1;                       /**< Card Interrupt Reset Select 1, offset: 0x69 */
  __IO uint8_t CARD_INTRSR2;                       /**< Card Interrupt Reset Select 2, offset: 0x6A */
       uint8_t RESERVED_7[1];
  __IO uint8_t RD_BASE0;                           /**< SQ Read Base Address 0, offset: 0x6C */
  __IO uint8_t RD_BASE1;                           /**< SQ Read Base Address 1, offset: 0x6D */
  __IO uint8_t RD_BASE2;                           /**< SQ Read Base Address 2, offset: 0x6E */
  __IO uint8_t RD_BASE3;                           /**< SQ Read Base Address 3, offset: 0x6F */
  __IO uint8_t WR_BASE0;                           /**< SQ Write Base Address 0, offset: 0x70 */
  __IO uint8_t WR_BASE1;                           /**< SQ Write Base Address 1, offset: 0x71 */
  __IO uint8_t WR_BASE2;                           /**< SQ Write Base Address 2, offset: 0x72 */
  __IO uint8_t WR_BASE3;                           /**< SQ Write Base Address 3, offset: 0x73 */
  __IO uint8_t RD_IDX;                             /**< Read Base Address Index, offset: 0x74 */
  __IO uint8_t WR_IDX;                             /**< Write Base Address Index, offset: 0x75 */
       uint8_t RESERVED_8[2];
  __IO uint8_t APU_SLP_RDY_EN;                     /**< APU Sleep Ready Enable, offset: 0x78 */
       uint8_t RESERVED_9[3];
  __IO uint8_t HOST_ERR_WKUP_EN;                   /**< Host Error Wakeup Enable, offset: 0x7C */
       uint8_t RESERVED_10[3];
  __I  uint8_t HOST_ERR_CMD0;                      /**< Host Error Command 0, offset: 0x80 */
  __I  uint8_t HOST_ERR_CMD1;                      /**< Host Error Command 1, offset: 0x81 */
  __I  uint8_t HOST_ERR_CMD2;                      /**< Host Error Command 2, offset: 0x82 */
  __I  uint8_t HOST_ERR_CMD3;                      /**< Host Error Command 3, offset: 0x83 */
  __I  uint8_t HOST_ERR_CMD4;                      /**< Host Error Command 4, offset: 0x84 */
  __I  uint8_t HOST_ERR_CMD5;                      /**< Host Error Command 5, offset: 0x85 */
       uint8_t RESERVED_11[2];
  __O  uint8_t PKT_WR_BITMAP_CLR0;                 /**< Packet Write Bitmap Clear 0, offset: 0x88 */
  __O  uint8_t PKT_WR_BITMAP_CLR1;                 /**< Packet Write Bitmap Clear 1, offset: 0x89 */
  __O  uint8_t PKT_WR_BITMAP_CLR2;                 /**< Packet Write Bitmap Clear 2, offset: 0x8A */
  __O  uint8_t PKT_WR_BITMAP_CLR3;                 /**< Packet Write Bitmap Clear 3, offset: 0x8B */
  __O  uint8_t PKT_RD_BITMAP_CLR0;                 /**< Packet Read Bitmap Clear 0, offset: 0x8C */
  __O  uint8_t PKT_RD_BITMAP_CLR1;                 /**< Packet Read Bitmap Clear 1, offset: 0x8D */
  __O  uint8_t PKT_RD_BITMAP_CLR2;                 /**< Packet Read Bitmap Clear 2, offset: 0x8E */
  __O  uint8_t PKT_RD_BITMAP_CLR3;                 /**< Packet Read Bitmap Clear 3, offset: 0x8F */
  __IO uint8_t HOST_INT_ACT_MASK_EN0;              /**< Host Interrupt Active Mask Enable 0, offset: 0x90 */
  __IO uint8_t HOST_INT_ACT_MASK_EN1;              /**< Host Interrupt Active Mask Enable 1, offset: 0x91 */
  __IO uint8_t HOST_INT_ACT_MASK_EN2;              /**< Host Interrupt Active Mask Enable 2, offset: 0x92 */
  __IO uint8_t HOST_INT_ACT_MASK_EN3;              /**< Host Interrupt Active Mask Enable 3, offset: 0x93 */
  __O  uint8_t HOST_INT_ACT_MASK_CLR0;             /**< Host Interrupt Active Mask Clear 0, offset: 0x94 */
  __O  uint8_t HOST_INT_ACT_MASK_CLR1;             /**< Host Interrupt Active Mask Clear 1, offset: 0x95 */
  __O  uint8_t HOST_INT_ACT_MASK_CLR2;             /**< Host Interrupt Active Mask Clear 2, offset: 0x96 */
  __O  uint8_t HOST_INT_ACT_MASK_CLR3;             /**< Host Interrupt Active Mask Clear 3, offset: 0x97 */
  __I  uint8_t HOST_INT_ACT_MASK_STATUS0;          /**< Host Interrupt Active Mask Status 0, offset: 0x98 */
  __I  uint8_t HOST_INT_ACT_MASK_STATUS1;          /**< Host Interrupt Active Mask Status 1, offset: 0x99 */
  __I  uint8_t HOST_INT_ACT_MASK_STATUS2;          /**< Host Interrupt Active Mask Status 2, offset: 0x9A */
  __I  uint8_t HOST_INT_ACT_MASK_STATUS3;          /**< Host Interrupt Active Mask Status 3, offset: 0x9B */
  __IO uint8_t CARD_INT_ACT_MASK_EN0;              /**< Card Interrupt Active Mask Enable 0, offset: 0x9C */
  __IO uint8_t CARD_INT_ACT_MASK_EN1;              /**< Card Interrupt Active Mask Enable 1, offset: 0x9D */
  __IO uint8_t CARD_INT_ACT_MASK_EN2;              /**< Card Interrupt Active Mask Enable 2, offset: 0x9E */
  __IO uint8_t CARD_INT_ACT_MASK_EN3;              /**< Card Interrupt Active Mask Enable 3, offset: 0x9F */
  __O  uint8_t CARD_INT_ACT_MASK_CLR0;             /**< Card Interrupt Active Mask Clear 0, offset: 0xA0 */
  __O  uint8_t CARD_INT_ACT_MASK_CLR1;             /**< Card Interrupt Active Mask Clear 1, offset: 0xA1 */
  __O  uint8_t CARD_INT_ACT_MASK_CLR2;             /**< Card Interrupt Active Mask Clear 2, offset: 0xA2 */
  __O  uint8_t CARD_INT_ACT_MASK_CLR3;             /**< Card Interrupt Active Mask Clear 3, offset: 0xA3 */
  __I  uint8_t CARD_INT_ACT_MASK_STATUS0;          /**< Card Interrupt Active Mask Status 0, offset: 0xA4 */
  __I  uint8_t CARD_INT_ACT_MASK_STATUS1;          /**< Card Interrupt Active Mask Status 1, offset: 0xA5 */
  __I  uint8_t CARD_INT_ACT_MASK_STATUS2;          /**< Card Interrupt Active Mask Status 2, offset: 0xA6 */
  __I  uint8_t CARD_INT_ACT_MASK_STATUS3;          /**< Card Interrupt Active Mask Status 3, offset: 0xA7 */
       uint8_t RESERVED_12[16];
  __IO uint8_t CMD_PORT_WR_BASE_0;                 /**< Command Port SQ Write Base Address 0, offset: 0xB8 */
  __IO uint8_t CMD_PORT_WR_BASE_1;                 /**< Command Port SQ Write Base Address 1, offset: 0xB9 */
  __IO uint8_t CMD_PORT_WR_BASE_2;                 /**< Command Port SQ Write Base Address 2, offset: 0xBA */
  __IO uint8_t CMD_PORT_WR_BASE_3;                 /**< Command Port SQ Write Base Address 3, offset: 0xBB */
  __IO uint8_t CMD_PORT_RD_BASE_0;                 /**< Command Port SQ Read Base Address 0, offset: 0xBC */
  __IO uint8_t CMD_PORT_RD_BASE_1;                 /**< Command Port SQ Read Base Address 1, offset: 0xBD */
  __IO uint8_t CMD_PORT_RD_BASE_2;                 /**< Command Port SQ Read Base Address 2, offset: 0xBE */
  __IO uint8_t CMD_PORT_RD_BASE_3;                 /**< Command Port SQ Read Base Address 3, offset: 0xBF */
  __IO uint8_t CMD_PORT_RD_LEN_0;                  /**< Command Port Read Length 0, offset: 0xC0 */
  __IO uint8_t CMD_PORT_RD_LEN_1;                  /**< Command Port Read Length 1, offset: 0xC1 */
       uint8_t RESERVED_13[2];
  __IO uint8_t CMD_PORT_CONFIG_0;                  /**< Command Port Config 0, offset: 0xC4 */
  __IO uint8_t CMD_PORT_CONFIG_1;                  /**< Command Port Config 1, offset: 0xC5 */
  __I  uint8_t CMD_PORT_CONFIG_2;                  /**< Command Port Config 2, offset: 0xC6 */
  __I  uint8_t CMD_PORT_CONFIG_3;                  /**< Command Port Config 3, offset: 0xC7 */
  __I  uint8_t CHIP_REV;                           /**< Chip Revision, offset: 0xC8 */
       uint8_t RESERVED_14[1];
  __I  uint8_t IP_REV0;                            /**< SDU Minor IP Revision, offset: 0xCA */
  __I  uint8_t IP_REV1;                            /**< SDU Major IP Revision, offset: 0xCB */
  __IO uint8_t PKT_END_RADDR0;                     /**< PKT_END_RADDR0, offset: 0xCC */
  __IO uint8_t PKT_END_RADDR1;                     /**< PKT_END_RADDR1, offset: 0xCD */
  __IO uint8_t PKT_END_RADDR2;                     /**< PKT_END_RADDR2, offset: 0xCE */
  __IO uint8_t PKT_END_RADDR3;                     /**< PKT_END_RADDR3, offset: 0xCF */
  __IO uint8_t PKT_END_WADDR0;                     /**< PKT_END_WADDR0, offset: 0xD0 */
  __IO uint8_t PKT_END_WADDR1;                     /**< PKT_END_WADDR1, offset: 0xD1 */
  __IO uint8_t PKT_END_WADDR2;                     /**< PKT_END_WADDR2, offset: 0xD2 */
  __IO uint8_t PKT_END_WADDR3;                     /**< PKT_END_WADDR3, offset: 0xD3 */
  __I  uint8_t OCR_0;                              /**< Operation Conditions 0, offset: 0xD4 */
  __I  uint8_t OCR_1;                              /**< Operation Conditions 1, offset: 0xD5 */
  __I  uint8_t OCR_2;                              /**< Operation Conditions 2, offset: 0xD6 */
  __IO uint8_t CARD_CONFIG_1;                      /**< Card Config1, offset: 0xD7 */
  __IO uint8_t CARD_CONFIG2_0;                     /**< Card Config2 0, offset: 0xD8 */
  __IO uint8_t CARD_CONFIG2_1;                     /**< Card Config2 1, offset: 0xD9 */
  __IO uint8_t CARD_CONFIG2_2;                     /**< Card Config2 2, offset: 0xDA */
  __IO uint8_t CARD_CONFIG2_3;                     /**< Card Config2 3, offset: 0xDB */
  __I  uint8_t TESTBUS0;                           /**< Testbus 0, offset: 0xDC */
  __I  uint8_t TESTBUS1;                           /**< Testbus 1, offset: 0xDD */
  __I  uint8_t RCA0;                               /**< RCA 0, offset: 0xDE */
  __I  uint8_t RCA1;                               /**< RCA 1, offset: 0xDF */
  __I  uint8_t DMA_ADDR0;                          /**< DMA Address 0, offset: 0xE0 */
  __I  uint8_t DMA_ADDR1;                          /**< DMA Address 1, offset: 0xE1 */
  __I  uint8_t DMA_ADDR2;                          /**< DMA Address 2, offset: 0xE2 */
  __I  uint8_t DMA_ADDR3;                          /**< DMA Address 3, offset: 0xE3 */
  __I  uint8_t IO_PORT0;                           /**< I/O Port 0, offset: 0xE4 */
  __I  uint8_t IO_PORT1;                           /**< I/O Port 1, offset: 0xE5 */
  __I  uint8_t IO_PORT2;                           /**< I/O Port 2, offset: 0xE6 */
       uint8_t RESERVED_15[1];
  __IO uint8_t SCRATCH2_0;                         /**< Scratch 2 0, offset: 0xE8 */
  __IO uint8_t SCRATCH2_1;                         /**< Scratch 2 1, offset: 0xE9 */
  __IO uint8_t SCRATCH2_2;                         /**< Scratch 2 2, offset: 0xEA */
  __IO uint8_t SCRATCH2_3;                         /**< Scratch 2 3, offset: 0xEB */
  __IO uint8_t SCRATCH3_0;                         /**< Scratch 3 0, offset: 0xEC */
  __IO uint8_t SCRATCH3_1;                         /**< Scratch 3 1, offset: 0xED */
  __IO uint8_t SCRATCH3_2;                         /**< Scratch 3 2, offset: 0xEE */
  __IO uint8_t SCRATCH3_3;                         /**< Scratch 3 3, offset: 0xEF */
  __IO uint8_t SCRATCH4_0;                         /**< Scratch 4 0, offset: 0xF0 */
  __IO uint8_t SCRATCH4_1;                         /**< Scratch 4 1, offset: 0xF1 */
  __IO uint8_t SCRATCH4_2;                         /**< Scratch 4 2, offset: 0xF2 */
  __IO uint8_t SCRATCH4_3;                         /**< Scratch 4 3, offset: 0xF3 */
  __IO uint8_t SCRATCH5_0;                         /**< Scratch 5 0, offset: 0xF4 */
  __IO uint8_t SCRATCH5_1;                         /**< Scratch 5 1, offset: 0xF5 */
  __IO uint8_t SCRATCH5_2;                         /**< Scratch 5 2, offset: 0xF6 */
  __IO uint8_t SCRATCH5_3;                         /**< Scratch 5 3, offset: 0xF7 */
  __IO uint8_t SCRATCH6_0;                         /**< Scratch 6 0, offset: 0xF8 */
  __IO uint8_t SCRATCH6_1;                         /**< Scratch 6 1, offset: 0xF9 */
  __IO uint8_t SCRATCH6_2;                         /**< Scratch 6 2, offset: 0xFA */
  __IO uint8_t SCRATCH6_3;                         /**< Scratch 6 3, offset: 0xFB */
  __IO uint8_t SCRATCH7_0;                         /**< Scratch 7 0, offset: 0xFC */
  __IO uint8_t SCRATCH7_1;                         /**< Scratch 7 1, offset: 0xFD */
  __IO uint8_t SCRATCH7_2;                         /**< Scratch 7 2, offset: 0xFE */
  __IO uint8_t SCRATCH7_3;                         /**< Scratch 7 3, offset: 0xFF */
} SDU_FN_CARD_Type;

/* ----------------------------------------------------------------------------
   -- SDU_FN_CARD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FN_CARD_Register_Masks SDU_FN_CARD Register Masks
 * @{
 */

/*! @name H2C_INTEVENT - Host to Card Interrupt Event */
/*! @{ */

#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_DOWN_MASK (0x1U)
#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_DOWN_SHIFT (0U)
/*! HOST_PWR_DOWN - Host power down event When host sets this bit, interrupt is generated to the CPU. */
#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_DOWN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_DOWN_SHIFT)) & SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_DOWN_MASK)

#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_UP_MASK (0x2U)
#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_UP_SHIFT (1U)
/*! HOST_PWR_UP - Host power up event When host sets this bit, interrupt is generated to the CPU. */
#define SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_UP(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_UP_SHIFT)) & SDU_FN_CARD_H2C_INTEVENT_HOST_PWR_UP_MASK)

#define SDU_FN_CARD_H2C_INTEVENT_HOST_TERMINATE_CMD53_MASK (0x4U)
#define SDU_FN_CARD_H2C_INTEVENT_HOST_TERMINATE_CMD53_SHIFT (2U)
/*! HOST_TERMINATE_CMD53 - Host terminates CMD53 When host sets this bit, current cmd53 data transfer will terminate. */
#define SDU_FN_CARD_H2C_INTEVENT_HOST_TERMINATE_CMD53(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_H2C_INTEVENT_HOST_TERMINATE_CMD53_SHIFT)) & SDU_FN_CARD_H2C_INTEVENT_HOST_TERMINATE_CMD53_MASK)

#define SDU_FN_CARD_H2C_INTEVENT_HOST_TO_CARD_EVENT_MASK (0x8U)
#define SDU_FN_CARD_H2C_INTEVENT_HOST_TO_CARD_EVENT_SHIFT (3U)
/*! HOST_TO_CARD_EVENT - Host to card event When host sets this bit, interrupt is generated to the CPU. */
#define SDU_FN_CARD_H2C_INTEVENT_HOST_TO_CARD_EVENT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_H2C_INTEVENT_HOST_TO_CARD_EVENT_SHIFT)) & SDU_FN_CARD_H2C_INTEVENT_HOST_TO_CARD_EVENT_MASK)

#define SDU_FN_CARD_H2C_INTEVENT_HOST_RST_EVENT_MASK (0x10U)
#define SDU_FN_CARD_H2C_INTEVENT_HOST_RST_EVENT_SHIFT (4U)
/*! HOST_RST_EVENT - Host reset event When host sets this bit, interrupt is generated to the CPU. */
#define SDU_FN_CARD_H2C_INTEVENT_HOST_RST_EVENT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_H2C_INTEVENT_HOST_RST_EVENT_SHIFT)) & SDU_FN_CARD_H2C_INTEVENT_HOST_RST_EVENT_MASK)
/*! @} */

/*! @name HOST_INTRSR0 - Host Interrupt Reset Select 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTRSR0_HOST_INT_RSR0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INTRSR0_HOST_INT_RSR0_SHIFT (0U)
/*! HOST_INT_RSR0 - Host Interrupt Reset Select [7:0] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN_CARD_HOST_INTRSR0_HOST_INT_RSR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTRSR0_HOST_INT_RSR0_SHIFT)) & SDU_FN_CARD_HOST_INTRSR0_HOST_INT_RSR0_MASK)
/*! @} */

/*! @name HOST_INTRSR1 - Host Interrupt Reset Select 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTRSR1_HOST_INT_RSR1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INTRSR1_HOST_INT_RSR1_SHIFT (0U)
/*! HOST_INT_RSR1 - Host Interrupt Reset Select [15:8] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN_CARD_HOST_INTRSR1_HOST_INT_RSR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTRSR1_HOST_INT_RSR1_SHIFT)) & SDU_FN_CARD_HOST_INTRSR1_HOST_INT_RSR1_MASK)
/*! @} */

/*! @name HOST_INTMASK0 - Host Interrupt Mask 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTMASK0_HOST_INT_MASK0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INTMASK0_HOST_INT_MASK0_SHIFT (0U)
/*! HOST_INT_MASK0 - Host Interrupt Mask [7:0] 0 = disable card to host interrupt 1 = enable card to host interrupt */
#define SDU_FN_CARD_HOST_INTMASK0_HOST_INT_MASK0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTMASK0_HOST_INT_MASK0_SHIFT)) & SDU_FN_CARD_HOST_INTMASK0_HOST_INT_MASK0_MASK)
/*! @} */

/*! @name HOST_INTMASK1 - Host Interrupt Mask 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTMASK1_HOST_INT_MASK1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INTMASK1_HOST_INT_MASK1_SHIFT (0U)
/*! HOST_INT_MASK1 - Host Interrupt Mask [15:8] 0 = disable card to host interrupt 1 = enable card to host interrupt */
#define SDU_FN_CARD_HOST_INTMASK1_HOST_INT_MASK1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTMASK1_HOST_INT_MASK1_SHIFT)) & SDU_FN_CARD_HOST_INTMASK1_HOST_INT_MASK1_MASK)
/*! @} */

/*! @name HOST_INTSTATUS0 - Host Interrupt Status 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_UPLD_HOST_INT_MASK (0x1U)
#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_UPLD_HOST_INT_SHIFT (0U)
/*! Q0_UPLD_HOST_INT - Queue 0 Upload Host Interrupt Status Set when card has packet ready for upload and card is in I/O ready state. */
#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_UPLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_Q0_UPLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_Q0_UPLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_DNLD_HOST_INT_MASK (0x2U)
#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_DNLD_HOST_INT_SHIFT (1U)
/*! Q0_DNLD_HOST_INT - Queue 0 Download Host Interrupt Status Set when card is ready for download from host. */
#define SDU_FN_CARD_HOST_INTSTATUS0_Q0_DNLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_Q0_DNLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_Q0_DNLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_UNDERFLOW_MASK (0x4U)
#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_UNDERFLOW_SHIFT (2U)
/*! FIFO_UNDERFLOW - Fifo Underflow Set when FIFO underflow occurs during upload. */
#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_UNDERFLOW(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_FIFO_UNDERFLOW_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_FIFO_UNDERFLOW_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_OVERFLOW_MASK (0x8U)
#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_OVERFLOW_SHIFT (3U)
/*! FIFO_OVERFLOW - Fifo Overflow Set when FIFO overflow occurs during download. */
#define SDU_FN_CARD_HOST_INTSTATUS0_FIFO_OVERFLOW(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_FIFO_OVERFLOW_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_FIFO_OVERFLOW_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_CARD_TO_HOST_INT_MASK (0x30U)
#define SDU_FN_CARD_HOST_INTSTATUS0_CARD_TO_HOST_INT_SHIFT (4U)
/*! CARD_TO_HOST_INT - 2-bit FW controlled interrupts to host. */
#define SDU_FN_CARD_HOST_INTSTATUS0_CARD_TO_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_CARD_TO_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_CARD_TO_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_UPLD_INT_MASK (0x40U)
#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_UPLD_INT_SHIFT (6U)
/*! CMD_PORT_UPLD_INT - Command Port Upload Host Interrupt Status Set when card has packet ready for
 *    command port upload and card is in I/O ready state.
 */
#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_UPLD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_UPLD_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_UPLD_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_DNLD_INT_MASK (0x80U)
#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_DNLD_INT_SHIFT (7U)
/*! CMD_PORT_DNLD_INT - Command Port Download Host Interrupt Status Set when card is ready for command port download from host. */
#define SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_DNLD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_DNLD_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS0_CMD_PORT_DNLD_INT_MASK)
/*! @} */

/*! @name HOST_INTSTATUS1 - Host Interrupt Status 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_UPLD_HOST_INT_MASK (0x1U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_UPLD_HOST_INT_SHIFT (0U)
/*! Q1_UPLD_HOST_INT - Queue 1 Upload Host Interrupt Status Set when card has packet ready for upload and card is in I/O ready state. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_UPLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q1_UPLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q1_UPLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_UPLD_HOST_INT_MASK (0x2U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_UPLD_HOST_INT_SHIFT (1U)
/*! Q2_UPLD_HOST_INT - Queue 2 Upload Host Interrupt Status Set when card has packet ready for upload and card is in I/O ready state. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_UPLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q2_UPLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q2_UPLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_UPLD_HOST_INT_MASK (0x4U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_UPLD_HOST_INT_SHIFT (2U)
/*! Q3_UPLD_HOST_INT - Queue 3 Upload Host Interrupt Status Set when card has packet ready for upload and card is in I/O ready state. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_UPLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q3_UPLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q3_UPLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_DNLD_HOST_INT_MASK (0x8U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_DNLD_HOST_INT_SHIFT (3U)
/*! Q1_DNLD_HOST_INT - Queue 1 Download Host Interrupt Status Set when card is ready for download from host. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q1_DNLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q1_DNLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q1_DNLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_DNLD_HOST_INT_MASK (0x10U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_DNLD_HOST_INT_SHIFT (4U)
/*! Q2_DNLD_HOST_INT - Queue 2 Download Host Interrupt Status Set when card is ready for download from host. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q2_DNLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q2_DNLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q2_DNLD_HOST_INT_MASK)

#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_DNLD_HOST_INT_MASK (0x20U)
#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_DNLD_HOST_INT_SHIFT (5U)
/*! Q3_DNLD_HOST_INT - Queue 3 Download Host Interrupt Status Set when card is ready for download from host. */
#define SDU_FN_CARD_HOST_INTSTATUS1_Q3_DNLD_HOST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INTSTATUS1_Q3_DNLD_HOST_INT_SHIFT)) & SDU_FN_CARD_HOST_INTSTATUS1_Q3_DNLD_HOST_INT_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP0 - Packet Read Bitmap 0 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP0_MASK (0x1U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP0_SHIFT (0U)
/*! PKT_RD_BITMAP0 - Packet read bitmap[0] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP0_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP0_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP1_MASK (0x2U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP1_SHIFT (1U)
/*! PKT_RD_BITMAP1 - Packet read bitmap[1] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP1_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP1_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP2_MASK (0x4U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP2_SHIFT (2U)
/*! PKT_RD_BITMAP2 - Packet read bitmap[2] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP2_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP2_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP3_MASK (0x8U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP3_SHIFT (3U)
/*! PKT_RD_BITMAP3 - Packet read bitmap[3] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP3_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP3_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP4_MASK (0x10U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP4_SHIFT (4U)
/*! PKT_RD_BITMAP4 - Packet read bitmap[4] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP4(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP4_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP4_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP5_MASK (0x20U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP5_SHIFT (5U)
/*! PKT_RD_BITMAP5 - Packet read bitmap[5] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP5(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP5_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP5_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP6_MASK (0x40U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP6_SHIFT (6U)
/*! PKT_RD_BITMAP6 - Packet read bitmap[6] */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP6(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP6_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP6_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP7_MASK (0x80U)
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP7_SHIFT (7U)
/*! PKT_RD_BITMAP7 - Packet read bitmap[7]. */
#define SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP7(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP7_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP0_PKT_RD_BITMAP7_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP1 - Packet Read Bitmap 1 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP8_MASK (0x1U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP8_SHIFT (0U)
/*! PKT_RD_BITMAP8 - Packet read bitmap[8] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP8(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP8_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP8_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP9_MASK (0x2U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP9_SHIFT (1U)
/*! PKT_RD_BITMAP9 - Packet read bitmap[9] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP9(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP9_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP9_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP10_MASK (0x4U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP10_SHIFT (2U)
/*! PKT_RD_BITMAP10 - Packet read bitmap[10] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP10(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP10_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP10_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP11_MASK (0x8U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP11_SHIFT (3U)
/*! PKT_RD_BITMAP11 - Packet read bitmap[11] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP11(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP11_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP11_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP12_MASK (0x10U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP12_SHIFT (4U)
/*! PKT_RD_BITMAP12 - Packet read bitmap[12] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP12(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP12_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP12_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP13_MASK (0x20U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP13_SHIFT (5U)
/*! PKT_RD_BITMAP13 - Packet read bitmap[13] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP13(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP13_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP13_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP14_MASK (0x40U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP14_SHIFT (6U)
/*! PKT_RD_BITMAP14 - Packet read bitmap[14] */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP14(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP14_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP14_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP15_MASK (0x80U)
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP15_SHIFT (7U)
/*! PKT_RD_BITMAP15 - Packet read bitmap[15]. */
#define SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP15(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP15_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP1_PKT_RD_BITMAP15_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP2 - Packet Read Bitmap 2 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP16_MASK (0x1U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP16_SHIFT (0U)
/*! PKT_RD_BITMAP16 - Packet read bitmap[16] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP16(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP16_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP16_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP17_MASK (0x2U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP17_SHIFT (1U)
/*! PKT_RD_BITMAP17 - Packet read bitmap[17] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP17(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP17_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP17_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP18_MASK (0x4U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP18_SHIFT (2U)
/*! PKT_RD_BITMAP18 - Packet read bitmap[18] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP18(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP18_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP18_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP19_MASK (0x8U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP19_SHIFT (3U)
/*! PKT_RD_BITMAP19 - Packet read bitmap[19] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP19(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP19_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP19_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP20_MASK (0x10U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP20_SHIFT (4U)
/*! PKT_RD_BITMAP20 - Packet read bitmap[20] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP20(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP20_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP20_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP21_MASK (0x20U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP21_SHIFT (5U)
/*! PKT_RD_BITMAP21 - Packet read bitmap[21] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP21(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP21_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP21_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP22_MASK (0x40U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP22_SHIFT (6U)
/*! PKT_RD_BITMAP22 - Packet read bitmap[22] */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP22(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP22_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP22_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP23_MASK (0x80U)
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP23_SHIFT (7U)
/*! PKT_RD_BITMAP23 - Packet read bitmap[23]. */
#define SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP23(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP23_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP2_PKT_RD_BITMAP23_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP3 - Packet Read Bitmap 3 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP24_MASK (0x1U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP24_SHIFT (0U)
/*! PKT_RD_BITMAP24 - Packet read bitmap[24] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP24(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP24_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP24_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP25_MASK (0x2U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP25_SHIFT (1U)
/*! PKT_RD_BITMAP25 - Packet read bitmap[25] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP25(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP25_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP25_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP26_MASK (0x4U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP26_SHIFT (2U)
/*! PKT_RD_BITMAP26 - Packet read bitmap[26] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP26(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP26_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP26_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP27_MASK (0x8U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP27_SHIFT (3U)
/*! PKT_RD_BITMAP27 - Packet read bitmap[27] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP27(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP27_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP27_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP28_MASK (0x10U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP28_SHIFT (4U)
/*! PKT_RD_BITMAP28 - Packet read bitmap[28] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP28(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP28_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP28_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP29_MASK (0x20U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP29_SHIFT (5U)
/*! PKT_RD_BITMAP29 - Packet read bitmap[29] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP29(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP29_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP29_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP30_MASK (0x40U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP30_SHIFT (6U)
/*! PKT_RD_BITMAP30 - Packet read bitmap[30] */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP30(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP30_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP30_MASK)

#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP31_MASK (0x80U)
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP31_SHIFT (7U)
/*! PKT_RD_BITMAP31 - Packet read bitmap[31]. */
#define SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP31(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP31_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP3_PKT_RD_BITMAP31_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP0 - Packet Write Bitmap 0 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP0_MASK (0x1U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP0_SHIFT (0U)
/*! PKT_WR_BITMAP0 - Packet write bitmap[0] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP0_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP0_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP1_MASK (0x2U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP1_SHIFT (1U)
/*! PKT_WR_BITMAP1 - Packet write bitmap[1] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP1_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP1_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP2_MASK (0x4U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP2_SHIFT (2U)
/*! PKT_WR_BITMAP2 - Packet write bitmap[2] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP2_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP2_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP3_MASK (0x8U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP3_SHIFT (3U)
/*! PKT_WR_BITMAP3 - Packet write bitmap[3] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP3_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP3_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP4_MASK (0x10U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP4_SHIFT (4U)
/*! PKT_WR_BITMAP4 - Packet write bitmap[4] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP4(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP4_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP4_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP5_MASK (0x20U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP5_SHIFT (5U)
/*! PKT_WR_BITMAP5 - Packet write bitmap[5] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP5(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP5_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP5_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP6_MASK (0x40U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP6_SHIFT (6U)
/*! PKT_WR_BITMAP6 - Packet write bitmap[6] */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP6(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP6_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP6_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP7_MASK (0x80U)
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP7_SHIFT (7U)
/*! PKT_WR_BITMAP7 - Packet write bitmap[7]. */
#define SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP7(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP7_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP0_PKT_WR_BITMAP7_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP1 - Packet Write Bitmap 1 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP8_MASK (0x1U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP8_SHIFT (0U)
/*! PKT_WR_BITMAP8 - Packet write bitmap[8] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP8(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP8_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP8_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP9_MASK (0x2U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP9_SHIFT (1U)
/*! PKT_WR_BITMAP9 - Packet write bitmap[9] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP9(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP9_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP9_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP10_MASK (0x4U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP10_SHIFT (2U)
/*! PKT_WR_BITMAP10 - Packet write bitmap[10] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP10(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP10_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP10_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP11_MASK (0x8U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP11_SHIFT (3U)
/*! PKT_WR_BITMAP11 - Packet write bitmap[11] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP11(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP11_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP11_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP12_MASK (0x10U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP12_SHIFT (4U)
/*! PKT_WR_BITMAP12 - Packet write bitmap[12] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP12(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP12_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP12_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP13_MASK (0x20U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP13_SHIFT (5U)
/*! PKT_WR_BITMAP13 - Packet write bitmap[13] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP13(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP13_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP13_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP14_MASK (0x40U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP14_SHIFT (6U)
/*! PKT_WR_BITMAP14 - Packet write bitmap[14] */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP14(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP14_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP14_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP15_MASK (0x80U)
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP15_SHIFT (7U)
/*! PKT_WR_BITMAP15 - Packet write bitmap[15]. */
#define SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP15(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP15_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP1_PKT_WR_BITMAP15_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP2 - Packet Write Bitmap 2 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP16_MASK (0x1U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP16_SHIFT (0U)
/*! PKT_WR_BITMAP16 - Packet write bitmap[16] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP16(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP16_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP16_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP17_MASK (0x2U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP17_SHIFT (1U)
/*! PKT_WR_BITMAP17 - Packet write bitmap[17] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP17(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP17_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP17_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP18_MASK (0x4U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP18_SHIFT (2U)
/*! PKT_WR_BITMAP18 - Packet write bitmap[18] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP18(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP18_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP18_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP19_MASK (0x8U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP19_SHIFT (3U)
/*! PKT_WR_BITMAP19 - Packet write bitmap[19] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP19(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP19_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP19_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP20_MASK (0x10U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP20_SHIFT (4U)
/*! PKT_WR_BITMAP20 - Packet write bitmap[20] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP20(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP20_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP20_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP21_MASK (0x20U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP21_SHIFT (5U)
/*! PKT_WR_BITMAP21 - Packet write bitmap[21] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP21(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP21_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP21_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP22_MASK (0x40U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP22_SHIFT (6U)
/*! PKT_WR_BITMAP22 - Packet write bitmap[22] */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP22(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP22_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP22_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP23_MASK (0x80U)
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP23_SHIFT (7U)
/*! PKT_WR_BITMAP23 - Packet write bitmap[23]. */
#define SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP23(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP23_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP2_PKT_WR_BITMAP23_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP3 - Packet Write Bitmap 3 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP24_MASK (0x1U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP24_SHIFT (0U)
/*! PKT_WR_BITMAP24 - Packet write bitmap[24] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP24(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP24_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP24_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP25_MASK (0x2U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP25_SHIFT (1U)
/*! PKT_WR_BITMAP25 - Packet write bitmap[25] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP25(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP25_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP25_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP26_MASK (0x4U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP26_SHIFT (2U)
/*! PKT_WR_BITMAP26 - Packet write bitmap[26] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP26(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP26_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP26_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP27_MASK (0x8U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP27_SHIFT (3U)
/*! PKT_WR_BITMAP27 - Packet write bitmap[27] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP27(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP27_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP27_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP28_MASK (0x10U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP28_SHIFT (4U)
/*! PKT_WR_BITMAP28 - Packet write bitmap[28] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP28(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP28_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP28_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP29_MASK (0x20U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP29_SHIFT (5U)
/*! PKT_WR_BITMAP29 - Packet write bitmap[29] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP29(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP29_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP29_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP30_MASK (0x40U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP30_SHIFT (6U)
/*! PKT_WR_BITMAP30 - Packet write bitmap[30] */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP30(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP30_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP30_MASK)

#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP31_MASK (0x80U)
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP31_SHIFT (7U)
/*! PKT_WR_BITMAP31 - Packet write bitmap[31]. */
#define SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP31(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP31_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP3_PKT_WR_BITMAP31_MASK)
/*! @} */

/*! @name PORT0_RD_LEN0 - Port 0 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT0_RD_LEN0_PORT0_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT0_RD_LEN0_PORT0_RD_LEN0_SHIFT (0U)
/*! PORT0_RD_LEN0 - Port 0 read length [7:0] */
#define SDU_FN_CARD_PORT0_RD_LEN0_PORT0_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT0_RD_LEN0_PORT0_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT0_RD_LEN0_PORT0_RD_LEN0_MASK)
/*! @} */

/*! @name PORT0_RD_LEN1 - Port 0 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT0_RD_LEN1_PORT0_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT0_RD_LEN1_PORT0_RD_LEN1_SHIFT (0U)
/*! PORT0_RD_LEN1 - Port 0 read length [15:8] */
#define SDU_FN_CARD_PORT0_RD_LEN1_PORT0_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT0_RD_LEN1_PORT0_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT0_RD_LEN1_PORT0_RD_LEN1_MASK)
/*! @} */

/*! @name PORT1_RD_LEN0 - Port 1 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT1_RD_LEN0_PORT1_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT1_RD_LEN0_PORT1_RD_LEN0_SHIFT (0U)
/*! PORT1_RD_LEN0 - Port 1 read length [7:0] */
#define SDU_FN_CARD_PORT1_RD_LEN0_PORT1_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT1_RD_LEN0_PORT1_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT1_RD_LEN0_PORT1_RD_LEN0_MASK)
/*! @} */

/*! @name PORT1_RD_LEN1 - Port 1 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT1_RD_LEN1_PORT1_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT1_RD_LEN1_PORT1_RD_LEN1_SHIFT (0U)
/*! PORT1_RD_LEN1 - Port 1 read length [15:8] */
#define SDU_FN_CARD_PORT1_RD_LEN1_PORT1_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT1_RD_LEN1_PORT1_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT1_RD_LEN1_PORT1_RD_LEN1_MASK)
/*! @} */

/*! @name PORT2_RD_LEN0 - Port 2 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT2_RD_LEN0_PORT2_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT2_RD_LEN0_PORT2_RD_LEN0_SHIFT (0U)
/*! PORT2_RD_LEN0 - Port 2 read length [7:0] */
#define SDU_FN_CARD_PORT2_RD_LEN0_PORT2_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT2_RD_LEN0_PORT2_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT2_RD_LEN0_PORT2_RD_LEN0_MASK)
/*! @} */

/*! @name PORT2_RD_LEN1 - Port 2 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT2_RD_LEN1_PORT2_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT2_RD_LEN1_PORT2_RD_LEN1_SHIFT (0U)
/*! PORT2_RD_LEN1 - Port 2 read length [15:8] */
#define SDU_FN_CARD_PORT2_RD_LEN1_PORT2_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT2_RD_LEN1_PORT2_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT2_RD_LEN1_PORT2_RD_LEN1_MASK)
/*! @} */

/*! @name PORT3_RD_LEN0 - Port 3 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT3_RD_LEN0_PORT3_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT3_RD_LEN0_PORT3_RD_LEN0_SHIFT (0U)
/*! PORT3_RD_LEN0 - Port 3 read length [7:0] */
#define SDU_FN_CARD_PORT3_RD_LEN0_PORT3_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT3_RD_LEN0_PORT3_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT3_RD_LEN0_PORT3_RD_LEN0_MASK)
/*! @} */

/*! @name PORT3_RD_LEN1 - Port 3 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT3_RD_LEN1_PORT3_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT3_RD_LEN1_PORT3_RD_LEN1_SHIFT (0U)
/*! PORT3_RD_LEN1 - Port 3 read length [15:8] */
#define SDU_FN_CARD_PORT3_RD_LEN1_PORT3_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT3_RD_LEN1_PORT3_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT3_RD_LEN1_PORT3_RD_LEN1_MASK)
/*! @} */

/*! @name PORT4_RD_LEN0 - Port 4 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT4_RD_LEN0_PORT4_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT4_RD_LEN0_PORT4_RD_LEN0_SHIFT (0U)
/*! PORT4_RD_LEN0 - Port 4 read length [7:0] */
#define SDU_FN_CARD_PORT4_RD_LEN0_PORT4_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT4_RD_LEN0_PORT4_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT4_RD_LEN0_PORT4_RD_LEN0_MASK)
/*! @} */

/*! @name PORT4_RD_LEN1 - Port 4 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT4_RD_LEN1_PORT4_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT4_RD_LEN1_PORT4_RD_LEN1_SHIFT (0U)
/*! PORT4_RD_LEN1 - Port 4 read length [15:8] */
#define SDU_FN_CARD_PORT4_RD_LEN1_PORT4_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT4_RD_LEN1_PORT4_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT4_RD_LEN1_PORT4_RD_LEN1_MASK)
/*! @} */

/*! @name PORT5_RD_LEN0 - Port 5 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT5_RD_LEN0_PORT5_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT5_RD_LEN0_PORT5_RD_LEN0_SHIFT (0U)
/*! PORT5_RD_LEN0 - Port 5 read length [7:0] */
#define SDU_FN_CARD_PORT5_RD_LEN0_PORT5_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT5_RD_LEN0_PORT5_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT5_RD_LEN0_PORT5_RD_LEN0_MASK)
/*! @} */

/*! @name PORT5_RD_LEN1 - Port 5 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT5_RD_LEN1_PORT5_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT5_RD_LEN1_PORT5_RD_LEN1_SHIFT (0U)
/*! PORT5_RD_LEN1 - Port 5 read length [15:8] */
#define SDU_FN_CARD_PORT5_RD_LEN1_PORT5_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT5_RD_LEN1_PORT5_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT5_RD_LEN1_PORT5_RD_LEN1_MASK)
/*! @} */

/*! @name PORT6_RD_LEN0 - Port 6 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT6_RD_LEN0_PORT6_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT6_RD_LEN0_PORT6_RD_LEN0_SHIFT (0U)
/*! PORT6_RD_LEN0 - Port 6 read length [7:0] */
#define SDU_FN_CARD_PORT6_RD_LEN0_PORT6_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT6_RD_LEN0_PORT6_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT6_RD_LEN0_PORT6_RD_LEN0_MASK)
/*! @} */

/*! @name PORT6_RD_LEN1 - Port 6 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT6_RD_LEN1_PORT6_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT6_RD_LEN1_PORT6_RD_LEN1_SHIFT (0U)
/*! PORT6_RD_LEN1 - Port 6 read length [15:8] */
#define SDU_FN_CARD_PORT6_RD_LEN1_PORT6_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT6_RD_LEN1_PORT6_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT6_RD_LEN1_PORT6_RD_LEN1_MASK)
/*! @} */

/*! @name PORT7_RD_LEN0 - Port 7 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT7_RD_LEN0_PORT7_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT7_RD_LEN0_PORT7_RD_LEN0_SHIFT (0U)
/*! PORT7_RD_LEN0 - Port 7 read length [7:0] */
#define SDU_FN_CARD_PORT7_RD_LEN0_PORT7_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT7_RD_LEN0_PORT7_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT7_RD_LEN0_PORT7_RD_LEN0_MASK)
/*! @} */

/*! @name PORT7_RD_LEN1 - Port 7 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT7_RD_LEN1_PORT7_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT7_RD_LEN1_PORT7_RD_LEN1_SHIFT (0U)
/*! PORT7_RD_LEN1 - Port 7 read length [15:8] */
#define SDU_FN_CARD_PORT7_RD_LEN1_PORT7_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT7_RD_LEN1_PORT7_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT7_RD_LEN1_PORT7_RD_LEN1_MASK)
/*! @} */

/*! @name PORT8_RD_LEN0 - Port 8 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT8_RD_LEN0_PORT8_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT8_RD_LEN0_PORT8_RD_LEN0_SHIFT (0U)
/*! PORT8_RD_LEN0 - Port 8 read length [7:0] */
#define SDU_FN_CARD_PORT8_RD_LEN0_PORT8_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT8_RD_LEN0_PORT8_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT8_RD_LEN0_PORT8_RD_LEN0_MASK)
/*! @} */

/*! @name PORT8_RD_LEN1 - Port 8 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT8_RD_LEN1_PORT8_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT8_RD_LEN1_PORT8_RD_LEN1_SHIFT (0U)
/*! PORT8_RD_LEN1 - Port 8 read length [15:8] */
#define SDU_FN_CARD_PORT8_RD_LEN1_PORT8_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT8_RD_LEN1_PORT8_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT8_RD_LEN1_PORT8_RD_LEN1_MASK)
/*! @} */

/*! @name PORT9_RD_LEN0 - Port 9 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT9_RD_LEN0_PORT9_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT9_RD_LEN0_PORT9_RD_LEN0_SHIFT (0U)
/*! PORT9_RD_LEN0 - Port 9 read length [7:0] */
#define SDU_FN_CARD_PORT9_RD_LEN0_PORT9_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT9_RD_LEN0_PORT9_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT9_RD_LEN0_PORT9_RD_LEN0_MASK)
/*! @} */

/*! @name PORT9_RD_LEN1 - Port 9 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT9_RD_LEN1_PORT9_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT9_RD_LEN1_PORT9_RD_LEN1_SHIFT (0U)
/*! PORT9_RD_LEN1 - Port 9 read length [15:8] */
#define SDU_FN_CARD_PORT9_RD_LEN1_PORT9_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT9_RD_LEN1_PORT9_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT9_RD_LEN1_PORT9_RD_LEN1_MASK)
/*! @} */

/*! @name PORT10_RD_LEN0 - Port 10 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT10_RD_LEN0_PORT10_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT10_RD_LEN0_PORT10_RD_LEN0_SHIFT (0U)
/*! PORT10_RD_LEN0 - Port 10 read length [7:0] */
#define SDU_FN_CARD_PORT10_RD_LEN0_PORT10_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT10_RD_LEN0_PORT10_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT10_RD_LEN0_PORT10_RD_LEN0_MASK)
/*! @} */

/*! @name PORT10_RD_LEN1 - Port 10 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT10_RD_LEN1_PORT10_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT10_RD_LEN1_PORT10_RD_LEN1_SHIFT (0U)
/*! PORT10_RD_LEN1 - Port 10 read length [15:8] */
#define SDU_FN_CARD_PORT10_RD_LEN1_PORT10_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT10_RD_LEN1_PORT10_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT10_RD_LEN1_PORT10_RD_LEN1_MASK)
/*! @} */

/*! @name PORT11_RD_LEN0 - Port 11 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT11_RD_LEN0_PORT11_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT11_RD_LEN0_PORT11_RD_LEN0_SHIFT (0U)
/*! PORT11_RD_LEN0 - Port 11 read length [7:0] */
#define SDU_FN_CARD_PORT11_RD_LEN0_PORT11_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT11_RD_LEN0_PORT11_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT11_RD_LEN0_PORT11_RD_LEN0_MASK)
/*! @} */

/*! @name PORT11_RD_LEN1 - Port 11 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT11_RD_LEN1_PORT11_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT11_RD_LEN1_PORT11_RD_LEN1_SHIFT (0U)
/*! PORT11_RD_LEN1 - Port 11 read length [15:8] */
#define SDU_FN_CARD_PORT11_RD_LEN1_PORT11_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT11_RD_LEN1_PORT11_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT11_RD_LEN1_PORT11_RD_LEN1_MASK)
/*! @} */

/*! @name PORT12_RD_LEN0 - Port 12 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT12_RD_LEN0_PORT12_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT12_RD_LEN0_PORT12_RD_LEN0_SHIFT (0U)
/*! PORT12_RD_LEN0 - Port 12 read length [7:0] */
#define SDU_FN_CARD_PORT12_RD_LEN0_PORT12_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT12_RD_LEN0_PORT12_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT12_RD_LEN0_PORT12_RD_LEN0_MASK)
/*! @} */

/*! @name PORT12_RD_LEN1 - Port 12 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT12_RD_LEN1_PORT12_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT12_RD_LEN1_PORT12_RD_LEN1_SHIFT (0U)
/*! PORT12_RD_LEN1 - Port 12 read length [15:8] */
#define SDU_FN_CARD_PORT12_RD_LEN1_PORT12_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT12_RD_LEN1_PORT12_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT12_RD_LEN1_PORT12_RD_LEN1_MASK)
/*! @} */

/*! @name PORT13_RD_LEN0 - Port 13 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT13_RD_LEN0_PORT13_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT13_RD_LEN0_PORT13_RD_LEN0_SHIFT (0U)
/*! PORT13_RD_LEN0 - Port 13 read length [7:0] */
#define SDU_FN_CARD_PORT13_RD_LEN0_PORT13_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT13_RD_LEN0_PORT13_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT13_RD_LEN0_PORT13_RD_LEN0_MASK)
/*! @} */

/*! @name PORT13_RD_LEN1 - Port 13 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT13_RD_LEN1_PORT13_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT13_RD_LEN1_PORT13_RD_LEN1_SHIFT (0U)
/*! PORT13_RD_LEN1 - Port 13 read length [15:8] */
#define SDU_FN_CARD_PORT13_RD_LEN1_PORT13_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT13_RD_LEN1_PORT13_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT13_RD_LEN1_PORT13_RD_LEN1_MASK)
/*! @} */

/*! @name PORT14_RD_LEN0 - Port 14 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT14_RD_LEN0_PORT14_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT14_RD_LEN0_PORT14_RD_LEN0_SHIFT (0U)
/*! PORT14_RD_LEN0 - Port 14 read length [7:0] */
#define SDU_FN_CARD_PORT14_RD_LEN0_PORT14_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT14_RD_LEN0_PORT14_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT14_RD_LEN0_PORT14_RD_LEN0_MASK)
/*! @} */

/*! @name PORT14_RD_LEN1 - Port 14 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT14_RD_LEN1_PORT14_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT14_RD_LEN1_PORT14_RD_LEN1_SHIFT (0U)
/*! PORT14_RD_LEN1 - Port 14 read length [15:8] */
#define SDU_FN_CARD_PORT14_RD_LEN1_PORT14_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT14_RD_LEN1_PORT14_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT14_RD_LEN1_PORT14_RD_LEN1_MASK)
/*! @} */

/*! @name PORT15_RD_LEN0 - Port 15 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT15_RD_LEN0_PORT15_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT15_RD_LEN0_PORT15_RD_LEN0_SHIFT (0U)
/*! PORT15_RD_LEN0 - Port 15 read length [7:0] */
#define SDU_FN_CARD_PORT15_RD_LEN0_PORT15_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT15_RD_LEN0_PORT15_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT15_RD_LEN0_PORT15_RD_LEN0_MASK)
/*! @} */

/*! @name PORT15_RD_LEN1 - Port 15 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT15_RD_LEN1_PORT15_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT15_RD_LEN1_PORT15_RD_LEN1_SHIFT (0U)
/*! PORT15_RD_LEN1 - Port 15 read length [15:8] */
#define SDU_FN_CARD_PORT15_RD_LEN1_PORT15_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT15_RD_LEN1_PORT15_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT15_RD_LEN1_PORT15_RD_LEN1_MASK)
/*! @} */

/*! @name PORT16_RD_LEN0 - Port 16 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT16_RD_LEN0_PORT16_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT16_RD_LEN0_PORT16_RD_LEN0_SHIFT (0U)
/*! PORT16_RD_LEN0 - Port 16 read length [7:0] */
#define SDU_FN_CARD_PORT16_RD_LEN0_PORT16_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT16_RD_LEN0_PORT16_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT16_RD_LEN0_PORT16_RD_LEN0_MASK)
/*! @} */

/*! @name PORT16_RD_LEN1 - Port 16 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT16_RD_LEN1_PORT16_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT16_RD_LEN1_PORT16_RD_LEN1_SHIFT (0U)
/*! PORT16_RD_LEN1 - Port 16 read length [15:8] */
#define SDU_FN_CARD_PORT16_RD_LEN1_PORT16_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT16_RD_LEN1_PORT16_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT16_RD_LEN1_PORT16_RD_LEN1_MASK)
/*! @} */

/*! @name PORT17_RD_LEN0 - Port 17 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT17_RD_LEN0_PORT17_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT17_RD_LEN0_PORT17_RD_LEN0_SHIFT (0U)
/*! PORT17_RD_LEN0 - Port 17 read length [7:0] */
#define SDU_FN_CARD_PORT17_RD_LEN0_PORT17_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT17_RD_LEN0_PORT17_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT17_RD_LEN0_PORT17_RD_LEN0_MASK)
/*! @} */

/*! @name PORT17_RD_LEN1 - Port 17 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT17_RD_LEN1_PORT17_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT17_RD_LEN1_PORT17_RD_LEN1_SHIFT (0U)
/*! PORT17_RD_LEN1 - Port 17 read length [15:8] */
#define SDU_FN_CARD_PORT17_RD_LEN1_PORT17_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT17_RD_LEN1_PORT17_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT17_RD_LEN1_PORT17_RD_LEN1_MASK)
/*! @} */

/*! @name PORT18_RD_LEN0 - Port 18 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT18_RD_LEN0_PORT18_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT18_RD_LEN0_PORT18_RD_LEN0_SHIFT (0U)
/*! PORT18_RD_LEN0 - Port 18 read length [7:0] */
#define SDU_FN_CARD_PORT18_RD_LEN0_PORT18_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT18_RD_LEN0_PORT18_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT18_RD_LEN0_PORT18_RD_LEN0_MASK)
/*! @} */

/*! @name PORT18_RD_LEN1 - Port 18 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT18_RD_LEN1_PORT18_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT18_RD_LEN1_PORT18_RD_LEN1_SHIFT (0U)
/*! PORT18_RD_LEN1 - Port 18 read length [15:8] */
#define SDU_FN_CARD_PORT18_RD_LEN1_PORT18_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT18_RD_LEN1_PORT18_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT18_RD_LEN1_PORT18_RD_LEN1_MASK)
/*! @} */

/*! @name PORT19_RD_LEN0 - Port 19 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT19_RD_LEN0_PORT19_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT19_RD_LEN0_PORT19_RD_LEN0_SHIFT (0U)
/*! PORT19_RD_LEN0 - Port 19 read length [7:0] */
#define SDU_FN_CARD_PORT19_RD_LEN0_PORT19_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT19_RD_LEN0_PORT19_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT19_RD_LEN0_PORT19_RD_LEN0_MASK)
/*! @} */

/*! @name PORT19_RD_LEN1 - Port 19 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT19_RD_LEN1_PORT19_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT19_RD_LEN1_PORT19_RD_LEN1_SHIFT (0U)
/*! PORT19_RD_LEN1 - Port 19 read length [15:8] */
#define SDU_FN_CARD_PORT19_RD_LEN1_PORT19_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT19_RD_LEN1_PORT19_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT19_RD_LEN1_PORT19_RD_LEN1_MASK)
/*! @} */

/*! @name PORT20_RD_LEN0 - Port 20 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT20_RD_LEN0_PORT20_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT20_RD_LEN0_PORT20_RD_LEN0_SHIFT (0U)
/*! PORT20_RD_LEN0 - Port 20 read length [7:0] */
#define SDU_FN_CARD_PORT20_RD_LEN0_PORT20_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT20_RD_LEN0_PORT20_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT20_RD_LEN0_PORT20_RD_LEN0_MASK)
/*! @} */

/*! @name PORT20_RD_LEN1 - Port 20 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT20_RD_LEN1_PORT20_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT20_RD_LEN1_PORT20_RD_LEN1_SHIFT (0U)
/*! PORT20_RD_LEN1 - Port 20 read length [15:8] */
#define SDU_FN_CARD_PORT20_RD_LEN1_PORT20_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT20_RD_LEN1_PORT20_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT20_RD_LEN1_PORT20_RD_LEN1_MASK)
/*! @} */

/*! @name PORT21_RD_LEN0 - Port 21 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT21_RD_LEN0_PORT21_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT21_RD_LEN0_PORT21_RD_LEN0_SHIFT (0U)
/*! PORT21_RD_LEN0 - Port 21 read length [7:0] */
#define SDU_FN_CARD_PORT21_RD_LEN0_PORT21_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT21_RD_LEN0_PORT21_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT21_RD_LEN0_PORT21_RD_LEN0_MASK)
/*! @} */

/*! @name PORT21_RD_LEN1 - Port 21 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT21_RD_LEN1_PORT21_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT21_RD_LEN1_PORT21_RD_LEN1_SHIFT (0U)
/*! PORT21_RD_LEN1 - Port 21 read length [15:8] */
#define SDU_FN_CARD_PORT21_RD_LEN1_PORT21_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT21_RD_LEN1_PORT21_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT21_RD_LEN1_PORT21_RD_LEN1_MASK)
/*! @} */

/*! @name PORT22_RD_LEN0 - Port 22 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT22_RD_LEN0_PORT22_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT22_RD_LEN0_PORT22_RD_LEN0_SHIFT (0U)
/*! PORT22_RD_LEN0 - Port 22 read length [7:0] */
#define SDU_FN_CARD_PORT22_RD_LEN0_PORT22_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT22_RD_LEN0_PORT22_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT22_RD_LEN0_PORT22_RD_LEN0_MASK)
/*! @} */

/*! @name PORT22_RD_LEN1 - Port 22 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT22_RD_LEN1_PORT22_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT22_RD_LEN1_PORT22_RD_LEN1_SHIFT (0U)
/*! PORT22_RD_LEN1 - Port 22 read length [15:8] */
#define SDU_FN_CARD_PORT22_RD_LEN1_PORT22_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT22_RD_LEN1_PORT22_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT22_RD_LEN1_PORT22_RD_LEN1_MASK)
/*! @} */

/*! @name PORT23_RD_LEN0 - Port 23 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT23_RD_LEN0_PORT23_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT23_RD_LEN0_PORT23_RD_LEN0_SHIFT (0U)
/*! PORT23_RD_LEN0 - Port 23 read length [7:0] */
#define SDU_FN_CARD_PORT23_RD_LEN0_PORT23_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT23_RD_LEN0_PORT23_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT23_RD_LEN0_PORT23_RD_LEN0_MASK)
/*! @} */

/*! @name PORT23_RD_LEN1 - Port 23 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT23_RD_LEN1_PORT23_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT23_RD_LEN1_PORT23_RD_LEN1_SHIFT (0U)
/*! PORT23_RD_LEN1 - Port 23 read length [15:8] */
#define SDU_FN_CARD_PORT23_RD_LEN1_PORT23_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT23_RD_LEN1_PORT23_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT23_RD_LEN1_PORT23_RD_LEN1_MASK)
/*! @} */

/*! @name PORT24_RD_LEN0 - Port 24 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT24_RD_LEN0_PORT24_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT24_RD_LEN0_PORT24_RD_LEN0_SHIFT (0U)
/*! PORT24_RD_LEN0 - Port 24 read length [7:0] */
#define SDU_FN_CARD_PORT24_RD_LEN0_PORT24_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT24_RD_LEN0_PORT24_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT24_RD_LEN0_PORT24_RD_LEN0_MASK)
/*! @} */

/*! @name PORT24_RD_LEN1 - Port 24 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT24_RD_LEN1_PORT24_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT24_RD_LEN1_PORT24_RD_LEN1_SHIFT (0U)
/*! PORT24_RD_LEN1 - Port 24 read length [15:8] */
#define SDU_FN_CARD_PORT24_RD_LEN1_PORT24_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT24_RD_LEN1_PORT24_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT24_RD_LEN1_PORT24_RD_LEN1_MASK)
/*! @} */

/*! @name PORT25_RD_LEN0 - Port 25 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT25_RD_LEN0_PORT25_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT25_RD_LEN0_PORT25_RD_LEN0_SHIFT (0U)
/*! PORT25_RD_LEN0 - Port 25 read length [7:0] */
#define SDU_FN_CARD_PORT25_RD_LEN0_PORT25_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT25_RD_LEN0_PORT25_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT25_RD_LEN0_PORT25_RD_LEN0_MASK)
/*! @} */

/*! @name PORT25_RD_LEN1 - Port 25 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT25_RD_LEN1_PORT25_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT25_RD_LEN1_PORT25_RD_LEN1_SHIFT (0U)
/*! PORT25_RD_LEN1 - Port 25 read length [15:8] */
#define SDU_FN_CARD_PORT25_RD_LEN1_PORT25_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT25_RD_LEN1_PORT25_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT25_RD_LEN1_PORT25_RD_LEN1_MASK)
/*! @} */

/*! @name PORT26_RD_LEN0 - Port 26 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT26_RD_LEN0_PORT26_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT26_RD_LEN0_PORT26_RD_LEN0_SHIFT (0U)
/*! PORT26_RD_LEN0 - Port 26 read length [7:0] */
#define SDU_FN_CARD_PORT26_RD_LEN0_PORT26_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT26_RD_LEN0_PORT26_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT26_RD_LEN0_PORT26_RD_LEN0_MASK)
/*! @} */

/*! @name PORT26_RD_LEN1 - Port 26 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT26_RD_LEN1_PORT26_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT26_RD_LEN1_PORT26_RD_LEN1_SHIFT (0U)
/*! PORT26_RD_LEN1 - Port 26 read length [15:8] */
#define SDU_FN_CARD_PORT26_RD_LEN1_PORT26_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT26_RD_LEN1_PORT26_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT26_RD_LEN1_PORT26_RD_LEN1_MASK)
/*! @} */

/*! @name PORT27_RD_LEN0 - Port 27 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT27_RD_LEN0_PORT27_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT27_RD_LEN0_PORT27_RD_LEN0_SHIFT (0U)
/*! PORT27_RD_LEN0 - Port 27 read length [7:0] */
#define SDU_FN_CARD_PORT27_RD_LEN0_PORT27_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT27_RD_LEN0_PORT27_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT27_RD_LEN0_PORT27_RD_LEN0_MASK)
/*! @} */

/*! @name PORT27_RD_LEN1 - Port 27 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT27_RD_LEN1_PORT27_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT27_RD_LEN1_PORT27_RD_LEN1_SHIFT (0U)
/*! PORT27_RD_LEN1 - Port 27 read length [15:8] */
#define SDU_FN_CARD_PORT27_RD_LEN1_PORT27_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT27_RD_LEN1_PORT27_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT27_RD_LEN1_PORT27_RD_LEN1_MASK)
/*! @} */

/*! @name PORT28_RD_LEN0 - Port 28 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT28_RD_LEN0_PORT28_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT28_RD_LEN0_PORT28_RD_LEN0_SHIFT (0U)
/*! PORT28_RD_LEN0 - Port 28 read length [7:0] */
#define SDU_FN_CARD_PORT28_RD_LEN0_PORT28_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT28_RD_LEN0_PORT28_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT28_RD_LEN0_PORT28_RD_LEN0_MASK)
/*! @} */

/*! @name PORT28_RD_LEN1 - Port 28 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT28_RD_LEN1_PORT28_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT28_RD_LEN1_PORT28_RD_LEN1_SHIFT (0U)
/*! PORT28_RD_LEN1 - Port 28 read length [15:8] */
#define SDU_FN_CARD_PORT28_RD_LEN1_PORT28_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT28_RD_LEN1_PORT28_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT28_RD_LEN1_PORT28_RD_LEN1_MASK)
/*! @} */

/*! @name PORT29_RD_LEN0 - Port 29 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT29_RD_LEN0_PORT29_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT29_RD_LEN0_PORT29_RD_LEN0_SHIFT (0U)
/*! PORT29_RD_LEN0 - Port 29 read length [7:0] */
#define SDU_FN_CARD_PORT29_RD_LEN0_PORT29_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT29_RD_LEN0_PORT29_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT29_RD_LEN0_PORT29_RD_LEN0_MASK)
/*! @} */

/*! @name PORT29_RD_LEN1 - Port 29 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT29_RD_LEN1_PORT29_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT29_RD_LEN1_PORT29_RD_LEN1_SHIFT (0U)
/*! PORT29_RD_LEN1 - Port 29 read length [15:8] */
#define SDU_FN_CARD_PORT29_RD_LEN1_PORT29_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT29_RD_LEN1_PORT29_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT29_RD_LEN1_PORT29_RD_LEN1_MASK)
/*! @} */

/*! @name PORT30_RD_LEN0 - Port 30 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT30_RD_LEN0_PORT30_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT30_RD_LEN0_PORT30_RD_LEN0_SHIFT (0U)
/*! PORT30_RD_LEN0 - Port 30 read length [7:0] */
#define SDU_FN_CARD_PORT30_RD_LEN0_PORT30_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT30_RD_LEN0_PORT30_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT30_RD_LEN0_PORT30_RD_LEN0_MASK)
/*! @} */

/*! @name PORT30_RD_LEN1 - Port 30 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT30_RD_LEN1_PORT30_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT30_RD_LEN1_PORT30_RD_LEN1_SHIFT (0U)
/*! PORT30_RD_LEN1 - Port 30 read length [15:8] */
#define SDU_FN_CARD_PORT30_RD_LEN1_PORT30_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT30_RD_LEN1_PORT30_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT30_RD_LEN1_PORT30_RD_LEN1_MASK)
/*! @} */

/*! @name PORT31_RD_LEN0 - Port 31 Packet Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_PORT31_RD_LEN0_PORT31_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_PORT31_RD_LEN0_PORT31_RD_LEN0_SHIFT (0U)
/*! PORT31_RD_LEN0 - Port 31 read length [7:0] */
#define SDU_FN_CARD_PORT31_RD_LEN0_PORT31_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT31_RD_LEN0_PORT31_RD_LEN0_SHIFT)) & SDU_FN_CARD_PORT31_RD_LEN0_PORT31_RD_LEN0_MASK)
/*! @} */

/*! @name PORT31_RD_LEN1 - Port 31 Packet Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_PORT31_RD_LEN1_PORT31_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_PORT31_RD_LEN1_PORT31_RD_LEN1_SHIFT (0U)
/*! PORT31_RD_LEN1 - Port 31 read length [15:8] */
#define SDU_FN_CARD_PORT31_RD_LEN1_PORT31_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PORT31_RD_LEN1_PORT31_RD_LEN1_SHIFT)) & SDU_FN_CARD_PORT31_RD_LEN1_PORT31_RD_LEN1_MASK)
/*! @} */

/*! @name HOST_RESTART - Host Transfer Status */
/*! @{ */

#define SDU_FN_CARD_HOST_RESTART_DNLD_RESTART_MASK (0x1U)
#define SDU_FN_CARD_HOST_RESTART_DNLD_RESTART_SHIFT (0U)
/*! DNLD_RESTART - Download Restart Host sets this bit for the card to retransmit packet. */
#define SDU_FN_CARD_HOST_RESTART_DNLD_RESTART(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_RESTART_DNLD_RESTART_SHIFT)) & SDU_FN_CARD_HOST_RESTART_DNLD_RESTART_MASK)

#define SDU_FN_CARD_HOST_RESTART_UPLD_RESTART_MASK (0x2U)
#define SDU_FN_CARD_HOST_RESTART_UPLD_RESTART_SHIFT (1U)
/*! UPLD_RESTART - Upload Restart Host sets this bit for the card to retransmit packet. */
#define SDU_FN_CARD_HOST_RESTART_UPLD_RESTART(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_RESTART_UPLD_RESTART_SHIFT)) & SDU_FN_CARD_HOST_RESTART_UPLD_RESTART_MASK)

#define SDU_FN_CARD_HOST_RESTART_DNLD_CRC_ERR_MASK (0x4U)
#define SDU_FN_CARD_HOST_RESTART_DNLD_CRC_ERR_SHIFT (2U)
/*! DNLD_CRC_ERR - Download Cyclic Redundancy Check Error This bit is set by HW if there is a data
 *    CRC error after a data block is downloaded.
 */
#define SDU_FN_CARD_HOST_RESTART_DNLD_CRC_ERR(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_RESTART_DNLD_CRC_ERR_SHIFT)) & SDU_FN_CARD_HOST_RESTART_DNLD_CRC_ERR_MASK)
/*! @} */

/*! @name FN_CARD_INTMASK - Function Card Interrupt Mask */
/*! @{ */

#define SDU_FN_CARD_FN_CARD_INTMASK_FN_CARD_INT_MASK_MASK (0x7U)
#define SDU_FN_CARD_FN_CARD_INTMASK_FN_CARD_INT_MASK_SHIFT (0U)
/*! FN_CARD_INT_MASK - Function card interrupt masks. */
#define SDU_FN_CARD_FN_CARD_INTMASK_FN_CARD_INT_MASK(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_FN_CARD_INTMASK_FN_CARD_INT_MASK_SHIFT)) & SDU_FN_CARD_FN_CARD_INTMASK_FN_CARD_INT_MASK_MASK)
/*! @} */

/*! @name Q_PRT_RANGE0 - Queue Port Range 0 */
/*! @{ */

#define SDU_FN_CARD_Q_PRT_RANGE0_Q0_PRT_RANGE_MASK (0x7U)
#define SDU_FN_CARD_Q_PRT_RANGE0_Q0_PRT_RANGE_SHIFT (0U)
/*! Q0_PRT_RANGE - Queue 0 Port Range Number of ports assigned per queue. */
#define SDU_FN_CARD_Q_PRT_RANGE0_Q0_PRT_RANGE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_Q_PRT_RANGE0_Q0_PRT_RANGE_SHIFT)) & SDU_FN_CARD_Q_PRT_RANGE0_Q0_PRT_RANGE_MASK)

#define SDU_FN_CARD_Q_PRT_RANGE0_Q1_PRT_RANGE_MASK (0x70U)
#define SDU_FN_CARD_Q_PRT_RANGE0_Q1_PRT_RANGE_SHIFT (4U)
/*! Q1_PRT_RANGE - Queue 1 Port Range Number of ports assigned per queue. */
#define SDU_FN_CARD_Q_PRT_RANGE0_Q1_PRT_RANGE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_Q_PRT_RANGE0_Q1_PRT_RANGE_SHIFT)) & SDU_FN_CARD_Q_PRT_RANGE0_Q1_PRT_RANGE_MASK)
/*! @} */

/*! @name Q_PRT_RANGE1 - Queue Port Range 1 */
/*! @{ */

#define SDU_FN_CARD_Q_PRT_RANGE1_Q2_PRT_RANGE_MASK (0x7U)
#define SDU_FN_CARD_Q_PRT_RANGE1_Q2_PRT_RANGE_SHIFT (0U)
/*! Q2_PRT_RANGE - Queue 2 Port Range Number of ports assigned per queue. */
#define SDU_FN_CARD_Q_PRT_RANGE1_Q2_PRT_RANGE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_Q_PRT_RANGE1_Q2_PRT_RANGE_SHIFT)) & SDU_FN_CARD_Q_PRT_RANGE1_Q2_PRT_RANGE_MASK)

#define SDU_FN_CARD_Q_PRT_RANGE1_Q3_PRT_RANGE_MASK (0x70U)
#define SDU_FN_CARD_Q_PRT_RANGE1_Q3_PRT_RANGE_SHIFT (4U)
/*! Q3_PRT_RANGE - Queue 3 Port Range Number of ports assigned per queue. */
#define SDU_FN_CARD_Q_PRT_RANGE1_Q3_PRT_RANGE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_Q_PRT_RANGE1_Q3_PRT_RANGE_SHIFT)) & SDU_FN_CARD_Q_PRT_RANGE1_Q3_PRT_RANGE_MASK)
/*! @} */

/*! @name C2H_INTEVENT0 - Card to Host Event 0 */
/*! @{ */

#define SDU_FN_CARD_C2H_INTEVENT0_Q0_DNLD_CARD_RDY_MASK (0x1U)
#define SDU_FN_CARD_C2H_INTEVENT0_Q0_DNLD_CARD_RDY_SHIFT (0U)
/*! Q0_DNLD_CARD_RDY - Queue 0 Download Card Ready Firmware sets this bit when one packet is ready. */
#define SDU_FN_CARD_C2H_INTEVENT0_Q0_DNLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_Q0_DNLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_Q0_DNLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_Q0_UPLD_CARD_RDY_MASK (0x2U)
#define SDU_FN_CARD_C2H_INTEVENT0_Q0_UPLD_CARD_RDY_SHIFT (1U)
/*! Q0_UPLD_CARD_RDY - Queue 0 Upload Card Ready Firmware sets this bit when one packet is ready. */
#define SDU_FN_CARD_C2H_INTEVENT0_Q0_UPLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_Q0_UPLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_Q0_UPLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_CIS_CARD_RDY_MASK (0x4U)
#define SDU_FN_CARD_C2H_INTEVENT0_CIS_CARD_RDY_SHIFT (2U)
/*! CIS_CARD_RDY - Card Information Structure Card Ready Firmware sets this bit after CIS table is initialized */
#define SDU_FN_CARD_C2H_INTEVENT0_CIS_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_CIS_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_CIS_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_IO_READY_MASK  (0x8U)
#define SDU_FN_CARD_C2H_INTEVENT0_IO_READY_SHIFT (3U)
/*! IO_READY - I/O Ready Indicator SD target device accepts CMD53 only after the previous CMD53 has finished. */
#define SDU_FN_CARD_C2H_INTEVENT0_IO_READY(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_IO_READY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_IO_READY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_CARD_TO_HOST_EVENT_MASK (0x30U)
#define SDU_FN_CARD_C2H_INTEVENT0_CARD_TO_HOST_EVENT_SHIFT (4U)
/*! CARD_TO_HOST_EVENT - Firmware controlled events to host. */
#define SDU_FN_CARD_C2H_INTEVENT0_CARD_TO_HOST_EVENT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_CARD_TO_HOST_EVENT_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_CARD_TO_HOST_EVENT_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_UPLD_RDY_MASK (0x40U)
#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_UPLD_RDY_SHIFT (6U)
/*! CMD_PORT_UPLD_RDY - Command Port Upload Ready */
#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_UPLD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_UPLD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_UPLD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_DNLD_RDY_MASK (0x80U)
#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_DNLD_RDY_SHIFT (7U)
/*! CMD_PORT_DNLD_RDY - Command Port Download Ready */
#define SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_DNLD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_DNLD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT0_CMD_PORT_DNLD_RDY_MASK)
/*! @} */

/*! @name C2H_INTEVENT1 - Card to Host Event 1 */
/*! @{ */

#define SDU_FN_CARD_C2H_INTEVENT1_Q1_DNLD_CARD_RDY_MASK (0x1U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q1_DNLD_CARD_RDY_SHIFT (0U)
/*! Q1_DNLD_CARD_RDY - Queue 1 Download Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q1_DNLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q1_DNLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q1_DNLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT1_Q2_DNLD_CARD_RDY_MASK (0x2U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q2_DNLD_CARD_RDY_SHIFT (1U)
/*! Q2_DNLD_CARD_RDY - Queue 2 Download Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q2_DNLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q2_DNLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q2_DNLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT1_Q3_DNLD_CARD_RDY_MASK (0x4U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q3_DNLD_CARD_RDY_SHIFT (2U)
/*! Q3_DNLD_CARD_RDY - Queue 3 Download Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q3_DNLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q3_DNLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q3_DNLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT1_Q1_UPLD_CARD_RDY_MASK (0x8U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q1_UPLD_CARD_RDY_SHIFT (3U)
/*! Q1_UPLD_CARD_RDY - Queue 1 Upload Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q1_UPLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q1_UPLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q1_UPLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT1_Q2_UPLD_CARD_RDY_MASK (0x10U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q2_UPLD_CARD_RDY_SHIFT (4U)
/*! Q2_UPLD_CARD_RDY - Queue 2 Upload Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q2_UPLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q2_UPLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q2_UPLD_CARD_RDY_MASK)

#define SDU_FN_CARD_C2H_INTEVENT1_Q3_UPLD_CARD_RDY_MASK (0x20U)
#define SDU_FN_CARD_C2H_INTEVENT1_Q3_UPLD_CARD_RDY_SHIFT (5U)
/*! Q3_UPLD_CARD_RDY - Queue 3 Upload Card Ready */
#define SDU_FN_CARD_C2H_INTEVENT1_Q3_UPLD_CARD_RDY(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_C2H_INTEVENT1_Q3_UPLD_CARD_RDY_SHIFT)) & SDU_FN_CARD_C2H_INTEVENT1_Q3_UPLD_CARD_RDY_MASK)
/*! @} */

/*! @name CARD_INTMASK0 - Card Interrupt Mask 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTMASK0_CARD_INT_MASK0_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTMASK0_CARD_INT_MASK0_SHIFT (0U)
/*! CARD_INT_MASK0 - Host to Card Interrupt Mask[7:0] 0 = disable host to card interrupt 1 = enable host to card interrupt */
#define SDU_FN_CARD_CARD_INTMASK0_CARD_INT_MASK0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTMASK0_CARD_INT_MASK0_SHIFT)) & SDU_FN_CARD_CARD_INTMASK0_CARD_INT_MASK0_MASK)
/*! @} */

/*! @name CARD_INTMASK1 - Card Interrupt Mask 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTMASK1_CARD_INT_MASK1_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTMASK1_CARD_INT_MASK1_SHIFT (0U)
/*! CARD_INT_MASK1 - Host to Card Interrupt Mask[15:8] 0 = disable host to card interrupt 1 = enable host to card interrupt */
#define SDU_FN_CARD_CARD_INTMASK1_CARD_INT_MASK1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTMASK1_CARD_INT_MASK1_SHIFT)) & SDU_FN_CARD_CARD_INTMASK1_CARD_INT_MASK1_MASK)
/*! @} */

/*! @name CARD_INTMASK2 - Card Interrupt Mask 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTMASK2_CARD_INT_MASK2_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTMASK2_CARD_INT_MASK2_SHIFT (0U)
/*! CARD_INT_MASK2 - Host to Card Interrupt Mask [23:16] 0 = disable host to card interrupt 1 = enable host to card interrupt */
#define SDU_FN_CARD_CARD_INTMASK2_CARD_INT_MASK2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTMASK2_CARD_INT_MASK2_SHIFT)) & SDU_FN_CARD_CARD_INTMASK2_CARD_INT_MASK2_MASK)
/*! @} */

/*! @name CARD_INTSTATUS0 - Card Interrupt Status 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_DNLD_CARD_INT_MASK (0x1U)
#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_DNLD_CARD_INT_SHIFT (0U)
/*! Q0_DNLD_CARD_INT - Queue 0 DnldCardInt event when (IO_Write) && DMA_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_DNLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_Q0_DNLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_Q0_DNLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_UPLD_CARD_INT_MASK (0x2U)
#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_UPLD_CARD_INT_SHIFT (1U)
/*! Q0_UPLD_CARD_INT - Queue 0 UpldCardInt event when (IO_Ready) && SD_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS0_Q0_UPLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_Q0_UPLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_Q0_UPLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_ABORT_CARD_INT_MASK (0x4U)
#define SDU_FN_CARD_CARD_INTSTATUS0_ABORT_CARD_INT_SHIFT (2U)
/*! ABORT_CARD_INT - Abort CardInt event when abort pulse. */
#define SDU_FN_CARD_CARD_INTSTATUS0_ABORT_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_ABORT_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_ABORT_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_DOWN_INT_MASK (0x8U)
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_DOWN_INT_SHIFT (3U)
/*! HOST_PWR_DOWN_INT - Power down interrupt */
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_DOWN_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_DOWN_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_DOWN_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_UP_INT_MASK (0x10U)
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_UP_INT_SHIFT (4U)
/*! HOST_PWR_UP_INT - Power up interrupt */
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_UP_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_UP_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_HOST_PWR_UP_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_UNDERFLOW_CARD_INT_MASK (0x20U)
#define SDU_FN_CARD_CARD_INTSTATUS0_UNDERFLOW_CARD_INT_SHIFT (5U)
/*! UNDERFLOW_CARD_INT - Fifo underflow */
#define SDU_FN_CARD_CARD_INTSTATUS0_UNDERFLOW_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_UNDERFLOW_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_UNDERFLOW_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_OVERFLOW_CARD_INT_MASK (0x40U)
#define SDU_FN_CARD_CARD_INTSTATUS0_OVERFLOW_CARD_INT_SHIFT (6U)
/*! OVERFLOW_CARD_INT - Fifo overflow */
#define SDU_FN_CARD_CARD_INTSTATUS0_OVERFLOW_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_OVERFLOW_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_OVERFLOW_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_TO_CARD_EVENT_MASK (0x80U)
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_TO_CARD_EVENT_SHIFT (7U)
/*! HOST_TO_CARD_EVENT - Host interrupt to card */
#define SDU_FN_CARD_CARD_INTSTATUS0_HOST_TO_CARD_EVENT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS0_HOST_TO_CARD_EVENT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS0_HOST_TO_CARD_EVENT_MASK)
/*! @} */

/*! @name CARD_INTSTATUS1 - Card Interrupt Status 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTSTATUS1_IO_ENABLE_INT_MASK (0x1U)
#define SDU_FN_CARD_CARD_INTSTATUS1_IO_ENABLE_INT_SHIFT (0U)
/*! IO_ENABLE_INT - This event is set when IO_ENABLE[fn] at FN0 0x02 is transitioned from 0-to-1 that is written by host */
#define SDU_FN_CARD_CARD_INTSTATUS1_IO_ENABLE_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_IO_ENABLE_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_IO_ENABLE_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_IO_DISABLE_INT_MASK (0x2U)
#define SDU_FN_CARD_CARD_INTSTATUS1_IO_DISABLE_INT_SHIFT (1U)
/*! IO_DISABLE_INT - This event is set when IO_ENABLE[fn] at FN0 0x02 is transitioned from 1-to-0 that is written by host */
#define SDU_FN_CARD_CARD_INTSTATUS1_IO_DISABLE_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_IO_DISABLE_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_IO_DISABLE_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_UPLD_CARD_INT_MASK (0x4U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_UPLD_CARD_INT_SHIFT (2U)
/*! CMD_PORT_UPLD_CARD_INT - This event is set if current cmd53 upload/rx data transfer is completed. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_UPLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_UPLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_UPLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_DNLD_CARD_INT_MASK (0x8U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_DNLD_CARD_INT_SHIFT (3U)
/*! CMD_PORT_DNLD_CARD_INT - This event is set if current cmd53 download/tx data transfer is completed. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_DNLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_DNLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD_PORT_DNLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_WR_ERR_INT_MASK (0x10U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_WR_ERR_INT_SHIFT (4U)
/*! CMD52_WR_ERR_INT - This event is set if host issues cmd52 write access to off-domain register during sleep mode. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_WR_ERR_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD52_WR_ERR_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD52_WR_ERR_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_RD_ERR_INT_MASK (0x20U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_RD_ERR_INT_SHIFT (5U)
/*! CMD52_RD_ERR_INT - This event is set if host issues cmd52 read access to off-domain register during sleep mode. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD52_RD_ERR_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD52_RD_ERR_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD52_RD_ERR_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_WR_ERR_INT_MASK (0x40U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_WR_ERR_INT_SHIFT (6U)
/*! CMD53_WR_ERR_INT - This event is set if host issues cmd53 write access to off-domain register during sleep mode. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_WR_ERR_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD53_WR_ERR_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD53_WR_ERR_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_RD_ERR_INT_MASK (0x80U)
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_RD_ERR_INT_SHIFT (7U)
/*! CMD53_RD_ERR_INT - This event is set if host issues cmd53 read access to off-domain register during sleep mode. */
#define SDU_FN_CARD_CARD_INTSTATUS1_CMD53_RD_ERR_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS1_CMD53_RD_ERR_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS1_CMD53_RD_ERR_INT_MASK)
/*! @} */

/*! @name CARD_INTSTATUS2 - Card Interrupt Status 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_DNLD_CARD_INT_MASK (0x1U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_DNLD_CARD_INT_SHIFT (0U)
/*! Q1_DNLD_CARD_INT - Queue 1 DnldCardInt event when (IO_Write) && DMA_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_DNLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q1_DNLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q1_DNLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_DNLD_CARD_INT_MASK (0x2U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_DNLD_CARD_INT_SHIFT (1U)
/*! Q2_DNLD_CARD_INT - Queue 2 DnldCardInt event when (IO_Write) && DMA_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_DNLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q2_DNLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q2_DNLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_DNLD_CARD_INT_MASK (0x4U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_DNLD_CARD_INT_SHIFT (2U)
/*! Q3_DNLD_CARD_INT - Queue 3 DnldCardInt event when (IO_Write) && DMA_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_DNLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q3_DNLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q3_DNLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_UPLD_CARD_INT_MASK (0x8U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_UPLD_CARD_INT_SHIFT (3U)
/*! Q1_UPLD_CARD_INT - Queue 1 UpldCardInt event when (IO_Ready) && SD_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q1_UPLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q1_UPLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q1_UPLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_UPLD_CARD_INT_MASK (0x10U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_UPLD_CARD_INT_SHIFT (4U)
/*! Q2_UPLD_CARD_INT - Queue 2 UpldCardInt event when (IO_Ready) && SD_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q2_UPLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q2_UPLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q2_UPLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_UPLD_CARD_INT_MASK (0x20U)
#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_UPLD_CARD_INT_SHIFT (5U)
/*! Q3_UPLD_CARD_INT - Queue 3 UpldCardInt event when (IO_Ready) && SD_Finish. */
#define SDU_FN_CARD_CARD_INTSTATUS2_Q3_UPLD_CARD_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_Q3_UPLD_CARD_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_Q3_UPLD_CARD_INT_MASK)

#define SDU_FN_CARD_CARD_INTSTATUS2_HOST_RST_INT_MASK (0x40U)
#define SDU_FN_CARD_CARD_INTSTATUS2_HOST_RST_INT_SHIFT (6U)
/*! HOST_RST_INT - Host reset event. */
#define SDU_FN_CARD_CARD_INTSTATUS2_HOST_RST_INT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTSTATUS2_HOST_RST_INT_SHIFT)) & SDU_FN_CARD_CARD_INTSTATUS2_HOST_RST_INT_MASK)
/*! @} */

/*! @name CARD_INTRSR0 - Card Interrupt Reset Select 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTRSR0_CARD_INT_RSR0_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTRSR0_CARD_INT_RSR0_SHIFT (0U)
/*! CARD_INT_RSR0 - Card Interrupt Reset Select[7:0] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN_CARD_CARD_INTRSR0_CARD_INT_RSR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTRSR0_CARD_INT_RSR0_SHIFT)) & SDU_FN_CARD_CARD_INTRSR0_CARD_INT_RSR0_MASK)
/*! @} */

/*! @name CARD_INTRSR1 - Card Interrupt Reset Select 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTRSR1_CARD_INT_RSR1_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTRSR1_CARD_INT_RSR1_SHIFT (0U)
/*! CARD_INT_RSR1 - Card Interrupt Reset Select[15:8] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN_CARD_CARD_INTRSR1_CARD_INT_RSR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTRSR1_CARD_INT_RSR1_SHIFT)) & SDU_FN_CARD_CARD_INTRSR1_CARD_INT_RSR1_MASK)
/*! @} */

/*! @name CARD_INTRSR2 - Card Interrupt Reset Select 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INTRSR2_CARD_INT_RSR2_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INTRSR2_CARD_INT_RSR2_SHIFT (0U)
/*! CARD_INT_RSR2 - Card Interrupt Reset Select[23:16] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN_CARD_CARD_INTRSR2_CARD_INT_RSR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INTRSR2_CARD_INT_RSR2_SHIFT)) & SDU_FN_CARD_CARD_INTRSR2_CARD_INT_RSR2_MASK)
/*! @} */

/*! @name RD_BASE0 - SQ Read Base Address 0 */
/*! @{ */

#define SDU_FN_CARD_RD_BASE0_SQ_READ_ADDR0_MASK  (0xFFU)
#define SDU_FN_CARD_RD_BASE0_SQ_READ_ADDR0_SHIFT (0U)
/*! SQ_READ_ADDR0 - SQ read base address bit [7:0]. */
#define SDU_FN_CARD_RD_BASE0_SQ_READ_ADDR0(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RD_BASE0_SQ_READ_ADDR0_SHIFT)) & SDU_FN_CARD_RD_BASE0_SQ_READ_ADDR0_MASK)
/*! @} */

/*! @name RD_BASE1 - SQ Read Base Address 1 */
/*! @{ */

#define SDU_FN_CARD_RD_BASE1_SQ_READ_ADDR1_MASK  (0xFFU)
#define SDU_FN_CARD_RD_BASE1_SQ_READ_ADDR1_SHIFT (0U)
/*! SQ_READ_ADDR1 - SQ read base address bit [15:8] */
#define SDU_FN_CARD_RD_BASE1_SQ_READ_ADDR1(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RD_BASE1_SQ_READ_ADDR1_SHIFT)) & SDU_FN_CARD_RD_BASE1_SQ_READ_ADDR1_MASK)
/*! @} */

/*! @name RD_BASE2 - SQ Read Base Address 2 */
/*! @{ */

#define SDU_FN_CARD_RD_BASE2_SQ_READ_ADDR2_MASK  (0xFFU)
#define SDU_FN_CARD_RD_BASE2_SQ_READ_ADDR2_SHIFT (0U)
/*! SQ_READ_ADDR2 - SQ read base address bit [23:16] */
#define SDU_FN_CARD_RD_BASE2_SQ_READ_ADDR2(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RD_BASE2_SQ_READ_ADDR2_SHIFT)) & SDU_FN_CARD_RD_BASE2_SQ_READ_ADDR2_MASK)
/*! @} */

/*! @name RD_BASE3 - SQ Read Base Address 3 */
/*! @{ */

#define SDU_FN_CARD_RD_BASE3_SQ_READ_ADDR3_MASK  (0xFFU)
#define SDU_FN_CARD_RD_BASE3_SQ_READ_ADDR3_SHIFT (0U)
/*! SQ_READ_ADDR3 - SQ read base address bit [31:24] */
#define SDU_FN_CARD_RD_BASE3_SQ_READ_ADDR3(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RD_BASE3_SQ_READ_ADDR3_SHIFT)) & SDU_FN_CARD_RD_BASE3_SQ_READ_ADDR3_MASK)
/*! @} */

/*! @name WR_BASE0 - SQ Write Base Address 0 */
/*! @{ */

#define SDU_FN_CARD_WR_BASE0_SQ_WRITE_ADDR0_MASK (0xFFU)
#define SDU_FN_CARD_WR_BASE0_SQ_WRITE_ADDR0_SHIFT (0U)
/*! SQ_WRITE_ADDR0 - SQ Write base address bit [7:0]. */
#define SDU_FN_CARD_WR_BASE0_SQ_WRITE_ADDR0(x)   (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_WR_BASE0_SQ_WRITE_ADDR0_SHIFT)) & SDU_FN_CARD_WR_BASE0_SQ_WRITE_ADDR0_MASK)
/*! @} */

/*! @name WR_BASE1 - SQ Write Base Address 1 */
/*! @{ */

#define SDU_FN_CARD_WR_BASE1_SQ_WRITE_ADDR1_MASK (0xFFU)
#define SDU_FN_CARD_WR_BASE1_SQ_WRITE_ADDR1_SHIFT (0U)
/*! SQ_WRITE_ADDR1 - SQ Write base address bit [15:8] */
#define SDU_FN_CARD_WR_BASE1_SQ_WRITE_ADDR1(x)   (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_WR_BASE1_SQ_WRITE_ADDR1_SHIFT)) & SDU_FN_CARD_WR_BASE1_SQ_WRITE_ADDR1_MASK)
/*! @} */

/*! @name WR_BASE2 - SQ Write Base Address 2 */
/*! @{ */

#define SDU_FN_CARD_WR_BASE2_SQ_WRITE_ADDR2_MASK (0xFFU)
#define SDU_FN_CARD_WR_BASE2_SQ_WRITE_ADDR2_SHIFT (0U)
/*! SQ_WRITE_ADDR2 - SQ Write base address bit [23:16] */
#define SDU_FN_CARD_WR_BASE2_SQ_WRITE_ADDR2(x)   (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_WR_BASE2_SQ_WRITE_ADDR2_SHIFT)) & SDU_FN_CARD_WR_BASE2_SQ_WRITE_ADDR2_MASK)
/*! @} */

/*! @name WR_BASE3 - SQ Write Base Address 3 */
/*! @{ */

#define SDU_FN_CARD_WR_BASE3_SQ_WRITE_ADDR3_MASK (0xFFU)
#define SDU_FN_CARD_WR_BASE3_SQ_WRITE_ADDR3_SHIFT (0U)
/*! SQ_WRITE_ADDR3 - SQ Write base address bit [31:24] */
#define SDU_FN_CARD_WR_BASE3_SQ_WRITE_ADDR3(x)   (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_WR_BASE3_SQ_WRITE_ADDR3_SHIFT)) & SDU_FN_CARD_WR_BASE3_SQ_WRITE_ADDR3_MASK)
/*! @} */

/*! @name RD_IDX - Read Base Address Index */
/*! @{ */

#define SDU_FN_CARD_RD_IDX_RD_INDEX_MASK         (0x1FU)
#define SDU_FN_CARD_RD_IDX_RD_INDEX_SHIFT        (0U)
/*! RD_INDEX - Index to current read base address [15:0] */
#define SDU_FN_CARD_RD_IDX_RD_INDEX(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RD_IDX_RD_INDEX_SHIFT)) & SDU_FN_CARD_RD_IDX_RD_INDEX_MASK)
/*! @} */

/*! @name WR_IDX - Write Base Address Index */
/*! @{ */

#define SDU_FN_CARD_WR_IDX_WR_INDEX_MASK         (0x1FU)
#define SDU_FN_CARD_WR_IDX_WR_INDEX_SHIFT        (0U)
/*! WR_INDEX - Index to current write base address [15:0] */
#define SDU_FN_CARD_WR_IDX_WR_INDEX(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_WR_IDX_WR_INDEX_SHIFT)) & SDU_FN_CARD_WR_IDX_WR_INDEX_MASK)
/*! @} */

/*! @name APU_SLP_RDY_EN - APU Sleep Ready Enable */
/*! @{ */

#define SDU_FN_CARD_APU_SLP_RDY_EN_APU_SLP_RDY_EN_MASK (0x1U)
#define SDU_FN_CARD_APU_SLP_RDY_EN_APU_SLP_RDY_EN_SHIFT (0U)
/*! APU_SLP_RDY_EN - APU Sleep Ready Enable If this bit is enabled, any pending host interrupt
 *    status will deassert sdu_apu_slp_rdy to prevent APU from going into sleep mode.
 */
#define SDU_FN_CARD_APU_SLP_RDY_EN_APU_SLP_RDY_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_APU_SLP_RDY_EN_APU_SLP_RDY_EN_SHIFT)) & SDU_FN_CARD_APU_SLP_RDY_EN_APU_SLP_RDY_EN_MASK)
/*! @} */

/*! @name HOST_ERR_WKUP_EN - Host Error Wakeup Enable */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_WR_ERR_WKUP_EN_MASK (0x1U)
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_WR_ERR_WKUP_EN_SHIFT (0U)
/*! CMD52_WR_ERR_WKUP_EN - CMD52 Write Error Wakeup Enable If host issues CMD52 write access to any
 *    off-domain register during sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_WR_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_WR_ERR_WKUP_EN_SHIFT)) & SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_WR_ERR_WKUP_EN_MASK)

#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_RD_ERR_WKUP_EN_MASK (0x2U)
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_RD_ERR_WKUP_EN_SHIFT (1U)
/*! CMD52_RD_ERR_WKUP_EN - CMD52 Read Error Wakeup Enable If host issues CMD52 read access to any
 *    off-domain register during sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_RD_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_RD_ERR_WKUP_EN_SHIFT)) & SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD52_RD_ERR_WKUP_EN_MASK)

#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_WR_ERR_WKUP_EN_MASK (0x4U)
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_WR_ERR_WKUP_EN_SHIFT (2U)
/*! CMD53_WR_ERR_WKUP_EN - CMD53 Write Error Wakeup Enable If host issues CMD53 write access during
 *    sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_WR_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_WR_ERR_WKUP_EN_SHIFT)) & SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_WR_ERR_WKUP_EN_MASK)

#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_RD_ERR_WKUP_EN_MASK (0x8U)
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_RD_ERR_WKUP_EN_SHIFT (3U)
/*! CMD53_RD_ERR_WKUP_EN - CMD53 Read Error Wakeup Enable If host issues CMD53 read access during
 *    sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_RD_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_RD_ERR_WKUP_EN_SHIFT)) & SDU_FN_CARD_HOST_ERR_WKUP_EN_CMD53_RD_ERR_WKUP_EN_MASK)

#define SDU_FN_CARD_HOST_ERR_WKUP_EN_TESTBUS_BIT_SEL_EN_MASK (0x30U)
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_TESTBUS_BIT_SEL_EN_SHIFT (4U)
/*! TESTBUS_BIT_SEL_EN - If bit 0 is 1, testbus_lo bits are individually selected by TESTBUS_BIT_SEL0 and TESTBUS_BIT_SEL1. */
#define SDU_FN_CARD_HOST_ERR_WKUP_EN_TESTBUS_BIT_SEL_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_WKUP_EN_TESTBUS_BIT_SEL_EN_SHIFT)) & SDU_FN_CARD_HOST_ERR_WKUP_EN_TESTBUS_BIT_SEL_EN_MASK)
/*! @} */

/*! @name HOST_ERR_CMD0 - Host Error Command 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD0_HOST_ERR_CMD0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD0_HOST_ERR_CMD0_SHIFT (0U)
/*! HOST_ERR_CMD0 - Host Error Command[7:0] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD0_HOST_ERR_CMD0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD0_HOST_ERR_CMD0_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD0_HOST_ERR_CMD0_MASK)
/*! @} */

/*! @name HOST_ERR_CMD1 - Host Error Command 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD1_HOST_ERR_CMD1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD1_HOST_ERR_CMD1_SHIFT (0U)
/*! HOST_ERR_CMD1 - Host Error Command[15:8] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD1_HOST_ERR_CMD1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD1_HOST_ERR_CMD1_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD1_HOST_ERR_CMD1_MASK)
/*! @} */

/*! @name HOST_ERR_CMD2 - Host Error Command 2 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD2_HOST_ERR_CMD2_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD2_HOST_ERR_CMD2_SHIFT (0U)
/*! HOST_ERR_CMD2 - Host Error Command[23:16] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD2_HOST_ERR_CMD2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD2_HOST_ERR_CMD2_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD2_HOST_ERR_CMD2_MASK)
/*! @} */

/*! @name HOST_ERR_CMD3 - Host Error Command 3 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD3_HOST_ERR_CMD3_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD3_HOST_ERR_CMD3_SHIFT (0U)
/*! HOST_ERR_CMD3 - Host Error Command[31:24] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD3_HOST_ERR_CMD3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD3_HOST_ERR_CMD3_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD3_HOST_ERR_CMD3_MASK)
/*! @} */

/*! @name HOST_ERR_CMD4 - Host Error Command 4 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD4_HOST_ERR_CMD4_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD4_HOST_ERR_CMD4_SHIFT (0U)
/*! HOST_ERR_CMD4 - Host Error Command[39:32] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD4_HOST_ERR_CMD4(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD4_HOST_ERR_CMD4_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD4_HOST_ERR_CMD4_MASK)
/*! @} */

/*! @name HOST_ERR_CMD5 - Host Error Command 5 */
/*! @{ */

#define SDU_FN_CARD_HOST_ERR_CMD5_HOST_ERR_CMD5_MASK (0xFFU)
#define SDU_FN_CARD_HOST_ERR_CMD5_HOST_ERR_CMD5_SHIFT (0U)
/*! HOST_ERR_CMD5 - Host Error Command[47:40] Capture 48-bit content of either CMD52 or CMD53
 *    depending on one of the four error conditions in CARD_INTSTATUS1[7:4].
 */
#define SDU_FN_CARD_HOST_ERR_CMD5_HOST_ERR_CMD5(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_ERR_CMD5_HOST_ERR_CMD5_SHIFT)) & SDU_FN_CARD_HOST_ERR_CMD5_HOST_ERR_CMD5_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP_CLR0 - Packet Write Bitmap Clear 0 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP_CLR0_PKT_WR_BITMAP_SW_CLR0_MASK (0xFFU)
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR0_PKT_WR_BITMAP_SW_CLR0_SHIFT (0U)
/*! PKT_WR_BITMAP_SW_CLR0 - Setting 1 to each bit will clear the corresponding pkt_wr_bitmap bit. */
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR0_PKT_WR_BITMAP_SW_CLR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP_CLR0_PKT_WR_BITMAP_SW_CLR0_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP_CLR0_PKT_WR_BITMAP_SW_CLR0_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP_CLR1 - Packet Write Bitmap Clear 1 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP_CLR1_PKT_WR_BITMAP_SW_CLR1_MASK (0xFFU)
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR1_PKT_WR_BITMAP_SW_CLR1_SHIFT (0U)
/*! PKT_WR_BITMAP_SW_CLR1 - Setting 1 to each bit will clear the corresponding pkt_wr_bitmap bit. */
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR1_PKT_WR_BITMAP_SW_CLR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP_CLR1_PKT_WR_BITMAP_SW_CLR1_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP_CLR1_PKT_WR_BITMAP_SW_CLR1_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP_CLR2 - Packet Write Bitmap Clear 2 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP_CLR2_PKT_WR_BITMAP_SW_CLR2_MASK (0xFFU)
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR2_PKT_WR_BITMAP_SW_CLR2_SHIFT (0U)
/*! PKT_WR_BITMAP_SW_CLR2 - Setting 1 to each bit will clear the corresponding pkt_wr_bitmap bit. */
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR2_PKT_WR_BITMAP_SW_CLR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP_CLR2_PKT_WR_BITMAP_SW_CLR2_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP_CLR2_PKT_WR_BITMAP_SW_CLR2_MASK)
/*! @} */

/*! @name PKT_WR_BITMAP_CLR3 - Packet Write Bitmap Clear 3 */
/*! @{ */

#define SDU_FN_CARD_PKT_WR_BITMAP_CLR3_PKT_WR_BITMAP_SW_CLR3_MASK (0xFFU)
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR3_PKT_WR_BITMAP_SW_CLR3_SHIFT (0U)
/*! PKT_WR_BITMAP_SW_CLR3 - Setting 1 to each bit will clear the corresponding pkt_wr_bitmap bit. */
#define SDU_FN_CARD_PKT_WR_BITMAP_CLR3_PKT_WR_BITMAP_SW_CLR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_WR_BITMAP_CLR3_PKT_WR_BITMAP_SW_CLR3_SHIFT)) & SDU_FN_CARD_PKT_WR_BITMAP_CLR3_PKT_WR_BITMAP_SW_CLR3_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP_CLR0 - Packet Read Bitmap Clear 0 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP_CLR0_PKT_RD_BITMAP_SW_CLR0_MASK (0xFFU)
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR0_PKT_RD_BITMAP_SW_CLR0_SHIFT (0U)
/*! PKT_RD_BITMAP_SW_CLR0 - Setting 1 to each bit will clear the corresponding pkt_rd_bitmap bit. */
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR0_PKT_RD_BITMAP_SW_CLR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP_CLR0_PKT_RD_BITMAP_SW_CLR0_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP_CLR0_PKT_RD_BITMAP_SW_CLR0_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP_CLR1 - Packet Read Bitmap Clear 1 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP_CLR1_PKT_RD_BITMAP_SW_CLR1_MASK (0xFFU)
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR1_PKT_RD_BITMAP_SW_CLR1_SHIFT (0U)
/*! PKT_RD_BITMAP_SW_CLR1 - Setting 1 to each bit will clear the corresponding pkt_rd_bitmap bit. */
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR1_PKT_RD_BITMAP_SW_CLR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP_CLR1_PKT_RD_BITMAP_SW_CLR1_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP_CLR1_PKT_RD_BITMAP_SW_CLR1_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP_CLR2 - Packet Read Bitmap Clear 2 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP_CLR2_PKT_RD_BITMAP_SW_CLR2_MASK (0xFFU)
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR2_PKT_RD_BITMAP_SW_CLR2_SHIFT (0U)
/*! PKT_RD_BITMAP_SW_CLR2 - Setting 1 to each bit will clear the corresponding pkt_rd_bitmap bit. */
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR2_PKT_RD_BITMAP_SW_CLR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP_CLR2_PKT_RD_BITMAP_SW_CLR2_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP_CLR2_PKT_RD_BITMAP_SW_CLR2_MASK)
/*! @} */

/*! @name PKT_RD_BITMAP_CLR3 - Packet Read Bitmap Clear 3 */
/*! @{ */

#define SDU_FN_CARD_PKT_RD_BITMAP_CLR3_PKT_RD_BITMAP_SW_CLR3_MASK (0xFFU)
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR3_PKT_RD_BITMAP_SW_CLR3_SHIFT (0U)
/*! PKT_RD_BITMAP_SW_CLR3 - Setting 1 to each bit will clear the corresponding pkt_rd_bitmap bit. */
#define SDU_FN_CARD_PKT_RD_BITMAP_CLR3_PKT_RD_BITMAP_SW_CLR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_RD_BITMAP_CLR3_PKT_RD_BITMAP_SW_CLR3_SHIFT)) & SDU_FN_CARD_PKT_RD_BITMAP_CLR3_PKT_RD_BITMAP_SW_CLR3_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_EN0 - Host Interrupt Active Mask Enable 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN0_HOST_INT_ACT_MASK_EN0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN0_HOST_INT_ACT_MASK_EN0_SHIFT (0U)
/*! HOST_INT_ACT_MASK_EN0 - Enable host interrupt controller active mask [7:0] */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN0_HOST_INT_ACT_MASK_EN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_EN0_HOST_INT_ACT_MASK_EN0_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_EN0_HOST_INT_ACT_MASK_EN0_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_EN1 - Host Interrupt Active Mask Enable 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN1_HOST_INT_ACT_MASK_EN1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN1_HOST_INT_ACT_MASK_EN1_SHIFT (0U)
/*! HOST_INT_ACT_MASK_EN1 - Enable host interrupt controller active mask [15:8] */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN1_HOST_INT_ACT_MASK_EN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_EN1_HOST_INT_ACT_MASK_EN1_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_EN1_HOST_INT_ACT_MASK_EN1_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_EN2 - Host Interrupt Active Mask Enable 2 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN2_HOST_INT_ACT_MASK_EN2_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN2_HOST_INT_ACT_MASK_EN2_SHIFT (0U)
/*! HOST_INT_ACT_MASK_EN2 - Enable host interrupt controller active mask [23:16] */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN2_HOST_INT_ACT_MASK_EN2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_EN2_HOST_INT_ACT_MASK_EN2_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_EN2_HOST_INT_ACT_MASK_EN2_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_EN3 - Host Interrupt Active Mask Enable 3 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN3_HOST_INT_ACT_MASK_EN3_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN3_HOST_INT_ACT_MASK_EN3_SHIFT (0U)
/*! HOST_INT_ACT_MASK_EN3 - Enable host interrupt controller active mask [31:24] */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_EN3_HOST_INT_ACT_MASK_EN3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_EN3_HOST_INT_ACT_MASK_EN3_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_EN3_HOST_INT_ACT_MASK_EN3_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_CLR0 - Host Interrupt Active Mask Clear 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR0_HOST_INT_ACT_MASK_CLR0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR0_HOST_INT_ACT_MASK_CLR0_SHIFT (0U)
/*! HOST_INT_ACT_MASK_CLR0 - Host interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding host interrupt active mask bit.
 */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR0_HOST_INT_ACT_MASK_CLR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_CLR0_HOST_INT_ACT_MASK_CLR0_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_CLR0_HOST_INT_ACT_MASK_CLR0_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_CLR1 - Host Interrupt Active Mask Clear 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR1_HOST_INT_ACT_MASK_CLR1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR1_HOST_INT_ACT_MASK_CLR1_SHIFT (0U)
/*! HOST_INT_ACT_MASK_CLR1 - Host interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding host interrupt active mask bit.
 */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR1_HOST_INT_ACT_MASK_CLR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_CLR1_HOST_INT_ACT_MASK_CLR1_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_CLR1_HOST_INT_ACT_MASK_CLR1_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_CLR2 - Host Interrupt Active Mask Clear 2 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR2_HOST_INT_ACT_MASK_CLR2_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR2_HOST_INT_ACT_MASK_CLR2_SHIFT (0U)
/*! HOST_INT_ACT_MASK_CLR2 - Host interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding host interrupt active mask bit.
 */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR2_HOST_INT_ACT_MASK_CLR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_CLR2_HOST_INT_ACT_MASK_CLR2_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_CLR2_HOST_INT_ACT_MASK_CLR2_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_CLR3 - Host Interrupt Active Mask Clear 3 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR3_HOST_INT_ACT_MASK_CLR3_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR3_HOST_INT_ACT_MASK_CLR3_SHIFT (0U)
/*! HOST_INT_ACT_MASK_CLR3 - Host interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding host interrupt active mask bit.
 */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_CLR3_HOST_INT_ACT_MASK_CLR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_CLR3_HOST_INT_ACT_MASK_CLR3_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_CLR3_HOST_INT_ACT_MASK_CLR3_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_STATUS0 - Host Interrupt Active Mask Status 0 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS0_HOST_INT_ACT_MASK_STATUS0_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS0_HOST_INT_ACT_MASK_STATUS0_SHIFT (0U)
/*! HOST_INT_ACT_MASK_STATUS0 - Host interrupt active mask status */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS0_HOST_INT_ACT_MASK_STATUS0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS0_HOST_INT_ACT_MASK_STATUS0_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS0_HOST_INT_ACT_MASK_STATUS0_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_STATUS1 - Host Interrupt Active Mask Status 1 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS1_HOST_INT_ACT_MASK_STATUS1_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS1_HOST_INT_ACT_MASK_STATUS1_SHIFT (0U)
/*! HOST_INT_ACT_MASK_STATUS1 - Host interrupt active mask status */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS1_HOST_INT_ACT_MASK_STATUS1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS1_HOST_INT_ACT_MASK_STATUS1_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS1_HOST_INT_ACT_MASK_STATUS1_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_STATUS2 - Host Interrupt Active Mask Status 2 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS2_HOST_INT_ACT_MASK_STATUS2_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS2_HOST_INT_ACT_MASK_STATUS2_SHIFT (0U)
/*! HOST_INT_ACT_MASK_STATUS2 - Host interrupt active mask status */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS2_HOST_INT_ACT_MASK_STATUS2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS2_HOST_INT_ACT_MASK_STATUS2_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS2_HOST_INT_ACT_MASK_STATUS2_MASK)
/*! @} */

/*! @name HOST_INT_ACT_MASK_STATUS3 - Host Interrupt Active Mask Status 3 */
/*! @{ */

#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS3_HOST_INT_ACT_MASK_STATUS3_MASK (0xFFU)
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS3_HOST_INT_ACT_MASK_STATUS3_SHIFT (0U)
/*! HOST_INT_ACT_MASK_STATUS3 - Host interrupt active mask status */
#define SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS3_HOST_INT_ACT_MASK_STATUS3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS3_HOST_INT_ACT_MASK_STATUS3_SHIFT)) & SDU_FN_CARD_HOST_INT_ACT_MASK_STATUS3_HOST_INT_ACT_MASK_STATUS3_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_EN0 - Card Interrupt Active Mask Enable 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN0_CARD_INT_ACT_MASK_EN0_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN0_CARD_INT_ACT_MASK_EN0_SHIFT (0U)
/*! CARD_INT_ACT_MASK_EN0 - Enable card interrupt controller active mask [7:0] */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN0_CARD_INT_ACT_MASK_EN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_EN0_CARD_INT_ACT_MASK_EN0_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_EN0_CARD_INT_ACT_MASK_EN0_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_EN1 - Card Interrupt Active Mask Enable 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN1_CARD_INT_ACT_MASK_EN1_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN1_CARD_INT_ACT_MASK_EN1_SHIFT (0U)
/*! CARD_INT_ACT_MASK_EN1 - Enable card interrupt controller active mask [15:8] */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN1_CARD_INT_ACT_MASK_EN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_EN1_CARD_INT_ACT_MASK_EN1_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_EN1_CARD_INT_ACT_MASK_EN1_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_EN2 - Card Interrupt Active Mask Enable 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN2_CARD_INT_ACT_MASK_EN2_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN2_CARD_INT_ACT_MASK_EN2_SHIFT (0U)
/*! CARD_INT_ACT_MASK_EN2 - Enable card interrupt controller active mask [23:16] */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN2_CARD_INT_ACT_MASK_EN2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_EN2_CARD_INT_ACT_MASK_EN2_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_EN2_CARD_INT_ACT_MASK_EN2_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_EN3 - Card Interrupt Active Mask Enable 3 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN3_CARD_INT_ACT_MASK_EN3_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN3_CARD_INT_ACT_MASK_EN3_SHIFT (0U)
/*! CARD_INT_ACT_MASK_EN3 - Enable card interrupt controller active mask [31:24] */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_EN3_CARD_INT_ACT_MASK_EN3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_EN3_CARD_INT_ACT_MASK_EN3_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_EN3_CARD_INT_ACT_MASK_EN3_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_CLR0 - Card Interrupt Active Mask Clear 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR0_CARD_INT_ACT_MASK_CLR0_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR0_CARD_INT_ACT_MASK_CLR0_SHIFT (0U)
/*! CARD_INT_ACT_MASK_CLR0 - Card interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding card interrupt active mask bit.
 */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR0_CARD_INT_ACT_MASK_CLR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_CLR0_CARD_INT_ACT_MASK_CLR0_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_CLR0_CARD_INT_ACT_MASK_CLR0_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_CLR1 - Card Interrupt Active Mask Clear 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR1_CARD_INT_ACT_MASK_CLR1_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR1_CARD_INT_ACT_MASK_CLR1_SHIFT (0U)
/*! CARD_INT_ACT_MASK_CLR1 - Card interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding card interrupt active mask bit.
 */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR1_CARD_INT_ACT_MASK_CLR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_CLR1_CARD_INT_ACT_MASK_CLR1_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_CLR1_CARD_INT_ACT_MASK_CLR1_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_CLR2 - Card Interrupt Active Mask Clear 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR2_CARD_INT_ACT_MASK_CLR2_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR2_CARD_INT_ACT_MASK_CLR2_SHIFT (0U)
/*! CARD_INT_ACT_MASK_CLR2 - Card interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding card interrupt active mask bit.
 */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR2_CARD_INT_ACT_MASK_CLR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_CLR2_CARD_INT_ACT_MASK_CLR2_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_CLR2_CARD_INT_ACT_MASK_CLR2_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_CLR3 - Card Interrupt Active Mask Clear 3 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR3_CARD_INT_ACT_MASK_CLR3_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR3_CARD_INT_ACT_MASK_CLR3_SHIFT (0U)
/*! CARD_INT_ACT_MASK_CLR3 - Card interrupt active mask write-1-to-clear Setting 1 to each bit will
 *    clear the corresponding card interrupt active mask bit.
 */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_CLR3_CARD_INT_ACT_MASK_CLR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_CLR3_CARD_INT_ACT_MASK_CLR3_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_CLR3_CARD_INT_ACT_MASK_CLR3_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_STATUS0 - Card Interrupt Active Mask Status 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS0_CARD_INT_ACT_MASK_STATUS0_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS0_CARD_INT_ACT_MASK_STATUS0_SHIFT (0U)
/*! CARD_INT_ACT_MASK_STATUS0 - Card interrupt active mask status */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS0_CARD_INT_ACT_MASK_STATUS0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS0_CARD_INT_ACT_MASK_STATUS0_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS0_CARD_INT_ACT_MASK_STATUS0_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_STATUS1 - Card Interrupt Active Mask Status 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS1_CARD_INT_ACT_MASK_STATUS1_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS1_CARD_INT_ACT_MASK_STATUS1_SHIFT (0U)
/*! CARD_INT_ACT_MASK_STATUS1 - Card interrupt active mask status */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS1_CARD_INT_ACT_MASK_STATUS1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS1_CARD_INT_ACT_MASK_STATUS1_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS1_CARD_INT_ACT_MASK_STATUS1_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_STATUS2 - Card Interrupt Active Mask Status 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS2_CARD_INT_ACT_MASK_STATUS2_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS2_CARD_INT_ACT_MASK_STATUS2_SHIFT (0U)
/*! CARD_INT_ACT_MASK_STATUS2 - Card interrupt active mask status */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS2_CARD_INT_ACT_MASK_STATUS2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS2_CARD_INT_ACT_MASK_STATUS2_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS2_CARD_INT_ACT_MASK_STATUS2_MASK)
/*! @} */

/*! @name CARD_INT_ACT_MASK_STATUS3 - Card Interrupt Active Mask Status 3 */
/*! @{ */

#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS3_CARD_INT_ACT_MASK_STATUS3_MASK (0xFFU)
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS3_CARD_INT_ACT_MASK_STATUS3_SHIFT (0U)
/*! CARD_INT_ACT_MASK_STATUS3 - Card interrupt active mask status */
#define SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS3_CARD_INT_ACT_MASK_STATUS3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS3_CARD_INT_ACT_MASK_STATUS3_SHIFT)) & SDU_FN_CARD_CARD_INT_ACT_MASK_STATUS3_CARD_INT_ACT_MASK_STATUS3_MASK)
/*! @} */

/*! @name CMD_PORT_WR_BASE_0 - Command Port SQ Write Base Address 0 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_WR_BASE_0_CMD_SQ_WRITE_ADDR0_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_WR_BASE_0_CMD_SQ_WRITE_ADDR0_SHIFT (0U)
/*! CMD_SQ_WRITE_ADDR0 - Command Port SQ Write base address bit [7:0]. */
#define SDU_FN_CARD_CMD_PORT_WR_BASE_0_CMD_SQ_WRITE_ADDR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_WR_BASE_0_CMD_SQ_WRITE_ADDR0_SHIFT)) & SDU_FN_CARD_CMD_PORT_WR_BASE_0_CMD_SQ_WRITE_ADDR0_MASK)
/*! @} */

/*! @name CMD_PORT_WR_BASE_1 - Command Port SQ Write Base Address 1 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_WR_BASE_1_CMD_SQ_WRITE_ADDR1_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_WR_BASE_1_CMD_SQ_WRITE_ADDR1_SHIFT (0U)
/*! CMD_SQ_WRITE_ADDR1 - Command Port SQ Write base address bit [15:8]. */
#define SDU_FN_CARD_CMD_PORT_WR_BASE_1_CMD_SQ_WRITE_ADDR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_WR_BASE_1_CMD_SQ_WRITE_ADDR1_SHIFT)) & SDU_FN_CARD_CMD_PORT_WR_BASE_1_CMD_SQ_WRITE_ADDR1_MASK)
/*! @} */

/*! @name CMD_PORT_WR_BASE_2 - Command Port SQ Write Base Address 2 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_WR_BASE_2_CMD_SQ_WRITE_ADDR2_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_WR_BASE_2_CMD_SQ_WRITE_ADDR2_SHIFT (0U)
/*! CMD_SQ_WRITE_ADDR2 - Command Port SQ Write base address bit [23:16]. */
#define SDU_FN_CARD_CMD_PORT_WR_BASE_2_CMD_SQ_WRITE_ADDR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_WR_BASE_2_CMD_SQ_WRITE_ADDR2_SHIFT)) & SDU_FN_CARD_CMD_PORT_WR_BASE_2_CMD_SQ_WRITE_ADDR2_MASK)
/*! @} */

/*! @name CMD_PORT_WR_BASE_3 - Command Port SQ Write Base Address 3 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_WR_BASE_3_CMD_SQ_WRITE_ADDR3_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_WR_BASE_3_CMD_SQ_WRITE_ADDR3_SHIFT (0U)
/*! CMD_SQ_WRITE_ADDR3 - Command Port SQ Write base address bit [31:24]. */
#define SDU_FN_CARD_CMD_PORT_WR_BASE_3_CMD_SQ_WRITE_ADDR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_WR_BASE_3_CMD_SQ_WRITE_ADDR3_SHIFT)) & SDU_FN_CARD_CMD_PORT_WR_BASE_3_CMD_SQ_WRITE_ADDR3_MASK)
/*! @} */

/*! @name CMD_PORT_RD_BASE_0 - Command Port SQ Read Base Address 0 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_BASE_0_CMD_SQ_READ_ADDR0_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_BASE_0_CMD_SQ_READ_ADDR0_SHIFT (0U)
/*! CMD_SQ_READ_ADDR0 - Command Port SQ Read base address bit [7:0]. */
#define SDU_FN_CARD_CMD_PORT_RD_BASE_0_CMD_SQ_READ_ADDR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_BASE_0_CMD_SQ_READ_ADDR0_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_BASE_0_CMD_SQ_READ_ADDR0_MASK)
/*! @} */

/*! @name CMD_PORT_RD_BASE_1 - Command Port SQ Read Base Address 1 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_BASE_1_CMD_SQ_READ_ADDR1_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_BASE_1_CMD_SQ_READ_ADDR1_SHIFT (0U)
/*! CMD_SQ_READ_ADDR1 - Command Port SQ Read base address bit [15:8]. */
#define SDU_FN_CARD_CMD_PORT_RD_BASE_1_CMD_SQ_READ_ADDR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_BASE_1_CMD_SQ_READ_ADDR1_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_BASE_1_CMD_SQ_READ_ADDR1_MASK)
/*! @} */

/*! @name CMD_PORT_RD_BASE_2 - Command Port SQ Read Base Address 2 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_BASE_2_CMD_SQ_READ_ADDR2_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_BASE_2_CMD_SQ_READ_ADDR2_SHIFT (0U)
/*! CMD_SQ_READ_ADDR2 - Command Port SQ Read base address bit [23:16]. */
#define SDU_FN_CARD_CMD_PORT_RD_BASE_2_CMD_SQ_READ_ADDR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_BASE_2_CMD_SQ_READ_ADDR2_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_BASE_2_CMD_SQ_READ_ADDR2_MASK)
/*! @} */

/*! @name CMD_PORT_RD_BASE_3 - Command Port SQ Read Base Address 3 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_BASE_3_CMD_SQ_READ_ADDR3_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_BASE_3_CMD_SQ_READ_ADDR3_SHIFT (0U)
/*! CMD_SQ_READ_ADDR3 - Command Port SQ Read base address bit [31:24]. */
#define SDU_FN_CARD_CMD_PORT_RD_BASE_3_CMD_SQ_READ_ADDR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_BASE_3_CMD_SQ_READ_ADDR3_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_BASE_3_CMD_SQ_READ_ADDR3_MASK)
/*! @} */

/*! @name CMD_PORT_RD_LEN_0 - Command Port Read Length 0 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_LEN_0_CMD_PORT_RD_LEN0_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_LEN_0_CMD_PORT_RD_LEN0_SHIFT (0U)
/*! CMD_PORT_RD_LEN0 - Command Port Read Length [7:0] */
#define SDU_FN_CARD_CMD_PORT_RD_LEN_0_CMD_PORT_RD_LEN0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_LEN_0_CMD_PORT_RD_LEN0_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_LEN_0_CMD_PORT_RD_LEN0_MASK)
/*! @} */

/*! @name CMD_PORT_RD_LEN_1 - Command Port Read Length 1 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_RD_LEN_1_CMD_PORT_RD_LEN1_MASK (0xFFU)
#define SDU_FN_CARD_CMD_PORT_RD_LEN_1_CMD_PORT_RD_LEN1_SHIFT (0U)
/*! CMD_PORT_RD_LEN1 - Command Port Read Length [15:8] */
#define SDU_FN_CARD_CMD_PORT_RD_LEN_1_CMD_PORT_RD_LEN1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_RD_LEN_1_CMD_PORT_RD_LEN1_SHIFT)) & SDU_FN_CARD_CMD_PORT_RD_LEN_1_CMD_PORT_RD_LEN1_MASK)
/*! @} */

/*! @name CMD_PORT_CONFIG_0 - Command Port Config 0 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_TX_LEN_FORMAT_MASK (0x3U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_TX_LEN_FORMAT_SHIFT (0U)
/*! CMD_PORT_TX_LEN_FORMAT - Define the cmd53 command port tx length (tx_len) header format attached
 *    in the beginning of data payload: 0 = no tx_len header info.
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_TX_LEN_FORMAT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_TX_LEN_FORMAT_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_TX_LEN_FORMAT_MASK)

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_RD_LEN_EN_MASK (0x4U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_RD_LEN_EN_SHIFT (2U)
/*! CMD_PORT_RD_LEN_EN - cmd_port_rd_len_en */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_RD_LEN_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_RD_LEN_EN_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_RD_LEN_EN_MASK)

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_UPLD_AUTO_RESET_MASK (0x10U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_UPLD_AUTO_RESET_SHIFT (4U)
/*! CMD_PORT_UPLD_AUTO_RESET - Reset control for cmd_port_upld_card_rdy event in C2H_INTEVENT 0 =
 *    upload ready event is reset to 0 after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_UPLD_AUTO_RESET(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_UPLD_AUTO_RESET_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_UPLD_AUTO_RESET_MASK)

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_DNLD_AUTO_RESET_MASK (0x20U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_DNLD_AUTO_RESET_SHIFT (5U)
/*! CMD_PORT_DNLD_AUTO_RESET - Reset control for cmd_port_dnld_card_rdy event in C2H_INTEVENT 0 =
 *    download ready event is reset to 0 after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_DNLD_AUTO_RESET(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_DNLD_AUTO_RESET_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_CMD_PORT_DNLD_AUTO_RESET_MASK)

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_UPLD_OVER_MASK (0x40U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_UPLD_OVER_SHIFT (6U)
/*! EXPLICIT_CMD_PORT_UPLD_OVER - Explicit Upload Update 0 = bitmap update is done on completion of
 *    CMD53 read 1 = bitmap update is done when host clears upload
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_UPLD_OVER(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_UPLD_OVER_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_UPLD_OVER_MASK)

#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_DNLD_OVER_MASK (0x80U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_DNLD_OVER_SHIFT (7U)
/*! EXPLICIT_CMD_PORT_DNLD_OVER - Explicit Download Over 0 = download over is generated on
 *    completion of CMD53 write 1 = download over is generated when host clears download ready interrupt
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_DNLD_OVER(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_DNLD_OVER_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_0_EXPLICIT_CMD_PORT_DNLD_OVER_MASK)
/*! @} */

/*! @name CMD_PORT_CONFIG_1 - Command Port Config 1 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_CONFIG_1_CMD_PORT_AUTO_ENABLE_MASK (0x1U)
#define SDU_FN_CARD_CMD_PORT_CONFIG_1_CMD_PORT_AUTO_ENABLE_SHIFT (0U)
/*! CMD_PORT_AUTO_ENABLE - If this is enabled, download and upload ready host interrupt is
 *    automatically cleared and re-enabled after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CMD_PORT_CONFIG_1_CMD_PORT_AUTO_ENABLE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_1_CMD_PORT_AUTO_ENABLE_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_1_CMD_PORT_AUTO_ENABLE_MASK)
/*! @} */

/*! @name CMD_PORT_CONFIG_2 - Command Port Config 2 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_CONFIG_2_RSVD_MASK  (0xFFU)
#define SDU_FN_CARD_CMD_PORT_CONFIG_2_RSVD_SHIFT (0U)
/*! RSVD - Reserved */
#define SDU_FN_CARD_CMD_PORT_CONFIG_2_RSVD(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_2_RSVD_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_2_RSVD_MASK)
/*! @} */

/*! @name CMD_PORT_CONFIG_3 - Command Port Config 3 */
/*! @{ */

#define SDU_FN_CARD_CMD_PORT_CONFIG_3_RSVD_MASK  (0xFFU)
#define SDU_FN_CARD_CMD_PORT_CONFIG_3_RSVD_SHIFT (0U)
/*! RSVD - Reserved */
#define SDU_FN_CARD_CMD_PORT_CONFIG_3_RSVD(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CMD_PORT_CONFIG_3_RSVD_SHIFT)) & SDU_FN_CARD_CMD_PORT_CONFIG_3_RSVD_MASK)
/*! @} */

/*! @name CHIP_REV - Chip Revision */
/*! @{ */

#define SDU_FN_CARD_CHIP_REV_CHIP_REV_MASK       (0xFFU)
#define SDU_FN_CARD_CHIP_REV_CHIP_REV_SHIFT      (0U)
/*! CHIP_REV - Chip Revision (same as CIU) */
#define SDU_FN_CARD_CHIP_REV_CHIP_REV(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CHIP_REV_CHIP_REV_SHIFT)) & SDU_FN_CARD_CHIP_REV_CHIP_REV_MASK)
/*! @} */

/*! @name IP_REV0 - SDU Minor IP Revision */
/*! @{ */

#define SDU_FN_CARD_IP_REV0_SDU_MINOR_IP_REV_MASK (0xFFU)
#define SDU_FN_CARD_IP_REV0_SDU_MINOR_IP_REV_SHIFT (0U)
/*! SDU_MINOR_IP_REV - SDU minor IP revision */
#define SDU_FN_CARD_IP_REV0_SDU_MINOR_IP_REV(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_IP_REV0_SDU_MINOR_IP_REV_SHIFT)) & SDU_FN_CARD_IP_REV0_SDU_MINOR_IP_REV_MASK)
/*! @} */

/*! @name IP_REV1 - SDU Major IP Revision */
/*! @{ */

#define SDU_FN_CARD_IP_REV1_SDU_MAJOR_IP_REV_MASK (0xFFU)
#define SDU_FN_CARD_IP_REV1_SDU_MAJOR_IP_REV_SHIFT (0U)
/*! SDU_MAJOR_IP_REV - SDU major IP revision */
#define SDU_FN_CARD_IP_REV1_SDU_MAJOR_IP_REV(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_IP_REV1_SDU_MAJOR_IP_REV_SHIFT)) & SDU_FN_CARD_IP_REV1_SDU_MAJOR_IP_REV_MASK)
/*! @} */

/*! @name PKT_END_RADDR0 - PKT_END_RADDR0 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_RADDR0_PACKET_END_RADDR0_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_RADDR0_PACKET_END_RADDR0_SHIFT (0U)
/*! PACKET_END_RADDR0 - For function 1, this is used as CMD53 DMA read packet end addr [7:0] For other functions, this is a scratch pad register */
#define SDU_FN_CARD_PKT_END_RADDR0_PACKET_END_RADDR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_RADDR0_PACKET_END_RADDR0_SHIFT)) & SDU_FN_CARD_PKT_END_RADDR0_PACKET_END_RADDR0_MASK)
/*! @} */

/*! @name PKT_END_RADDR1 - PKT_END_RADDR1 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_RADDR1_PACKET_END_RADDR1_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_RADDR1_PACKET_END_RADDR1_SHIFT (0U)
/*! PACKET_END_RADDR1 - For function 1, this is used as CMD53 DMA read packet end addr [15:8] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_RADDR1_PACKET_END_RADDR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_RADDR1_PACKET_END_RADDR1_SHIFT)) & SDU_FN_CARD_PKT_END_RADDR1_PACKET_END_RADDR1_MASK)
/*! @} */

/*! @name PKT_END_RADDR2 - PKT_END_RADDR2 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_RADDR2_PACKET_END_RADDR2_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_RADDR2_PACKET_END_RADDR2_SHIFT (0U)
/*! PACKET_END_RADDR2 - For function 1, this is used as CMD53 DMA read packet end addr [23:16] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_RADDR2_PACKET_END_RADDR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_RADDR2_PACKET_END_RADDR2_SHIFT)) & SDU_FN_CARD_PKT_END_RADDR2_PACKET_END_RADDR2_MASK)
/*! @} */

/*! @name PKT_END_RADDR3 - PKT_END_RADDR3 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_RADDR3_PACKET_END_RADDR3_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_RADDR3_PACKET_END_RADDR3_SHIFT (0U)
/*! PACKET_END_RADDR3 - For function 1, this is used as CMD53 DMA read packet end addr [31:24] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_RADDR3_PACKET_END_RADDR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_RADDR3_PACKET_END_RADDR3_SHIFT)) & SDU_FN_CARD_PKT_END_RADDR3_PACKET_END_RADDR3_MASK)
/*! @} */

/*! @name PKT_END_WADDR0 - PKT_END_WADDR0 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_WADDR0_PACKET_END_WADDR0_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_WADDR0_PACKET_END_WADDR0_SHIFT (0U)
/*! PACKET_END_WADDR0 - For function 1, this is used as CMD53 DMA write packet end addr [7:0] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_WADDR0_PACKET_END_WADDR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_WADDR0_PACKET_END_WADDR0_SHIFT)) & SDU_FN_CARD_PKT_END_WADDR0_PACKET_END_WADDR0_MASK)
/*! @} */

/*! @name PKT_END_WADDR1 - PKT_END_WADDR1 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_WADDR1_PACKET_END_WADDR1_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_WADDR1_PACKET_END_WADDR1_SHIFT (0U)
/*! PACKET_END_WADDR1 - For function 1, this is used as CMD53 DMA write packet end addr [15:8] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_WADDR1_PACKET_END_WADDR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_WADDR1_PACKET_END_WADDR1_SHIFT)) & SDU_FN_CARD_PKT_END_WADDR1_PACKET_END_WADDR1_MASK)
/*! @} */

/*! @name PKT_END_WADDR2 - PKT_END_WADDR2 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_WADDR2_PACKET_END_WADDR2_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_WADDR2_PACKET_END_WADDR2_SHIFT (0U)
/*! PACKET_END_WADDR2 - For function 1, this is used as CMD53 DMA write packet end addr [23:16] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_WADDR2_PACKET_END_WADDR2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_WADDR2_PACKET_END_WADDR2_SHIFT)) & SDU_FN_CARD_PKT_END_WADDR2_PACKET_END_WADDR2_MASK)
/*! @} */

/*! @name PKT_END_WADDR3 - PKT_END_WADDR3 */
/*! @{ */

#define SDU_FN_CARD_PKT_END_WADDR3_PACKET_END_WADDR3_MASK (0xFFU)
#define SDU_FN_CARD_PKT_END_WADDR3_PACKET_END_WADDR3_SHIFT (0U)
/*! PACKET_END_WADDR3 - For function 1, this is used as CMD53 DMA write packet end addr [31:24] For
 *    other functions, this is a scratch pad register
 */
#define SDU_FN_CARD_PKT_END_WADDR3_PACKET_END_WADDR3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_PKT_END_WADDR3_PACKET_END_WADDR3_SHIFT)) & SDU_FN_CARD_PKT_END_WADDR3_PACKET_END_WADDR3_MASK)
/*! @} */

/*! @name OCR_0 - Operation Conditions 0 */
/*! @{ */

#define SDU_FN_CARD_OCR_0_OCR0_MASK              (0xFFU)
#define SDU_FN_CARD_OCR_0_OCR0_SHIFT             (0U)
/*! OCR0 - Operation Conditions 0 */
#define SDU_FN_CARD_OCR_0_OCR0(x)                (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_OCR_0_OCR0_SHIFT)) & SDU_FN_CARD_OCR_0_OCR0_MASK)
/*! @} */

/*! @name OCR_1 - Operation Conditions 1 */
/*! @{ */

#define SDU_FN_CARD_OCR_1_OCR1_MASK              (0xFFU)
#define SDU_FN_CARD_OCR_1_OCR1_SHIFT             (0U)
/*! OCR1 - Operation Conditions 1 */
#define SDU_FN_CARD_OCR_1_OCR1(x)                (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_OCR_1_OCR1_SHIFT)) & SDU_FN_CARD_OCR_1_OCR1_MASK)
/*! @} */

/*! @name OCR_2 - Operation Conditions 2 */
/*! @{ */

#define SDU_FN_CARD_OCR_2_OCR2_MASK              (0xFFU)
#define SDU_FN_CARD_OCR_2_OCR2_SHIFT             (0U)
/*! OCR2 - Operation Conditions 2 */
#define SDU_FN_CARD_OCR_2_OCR2(x)                (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_OCR_2_OCR2_SHIFT)) & SDU_FN_CARD_OCR_2_OCR2_MASK)
/*! @} */

/*! @name CARD_CONFIG_1 - Card Config1 */
/*! @{ */

#define SDU_FN_CARD_CARD_CONFIG_1_SD_NEG_EDGE_MASK (0x1U)
#define SDU_FN_CARD_CARD_CONFIG_1_SD_NEG_EDGE_SHIFT (0U)
/*! SD_NEG_EDGE - Sample Data Negative Edge SD interface data sampling edge. */
#define SDU_FN_CARD_CARD_CONFIG_1_SD_NEG_EDGE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_SD_NEG_EDGE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_SD_NEG_EDGE_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_CMD53_FINISH_GBUS_MASK (0x2U)
#define SDU_FN_CARD_CARD_CONFIG_1_CMD53_FINISH_GBUS_SHIFT (1U)
/*! CMD53_FINISH_GBUS - Command 53 Finish Ahb Data transfer termination. */
#define SDU_FN_CARD_CARD_CONFIG_1_CMD53_FINISH_GBUS(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_CMD53_FINISH_GBUS_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_CMD53_FINISH_GBUS_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_OVER_MASK (0x4U)
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_OVER_SHIFT (2U)
/*! EXPLICIT_DNLD_OVER - Explicit Download Over 0 = download over is generated on completion of
 *    CMD53 write 1 = download over is generated when host clears download ready interrupt
 */
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_OVER(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_OVER_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_OVER_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_OVER_MASK (0x8U)
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_OVER_SHIFT (3U)
/*! EXPLICIT_UPLD_OVER - Explicit Upload Over 0 = upload over is generated on completion of CMD53
 *    read 1 = upload over is generated when host clears upload ready interrupt
 */
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_OVER(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_OVER_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_OVER_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_AGGR_INTR_EN_MASK (0x10U)
#define SDU_FN_CARD_CARD_CONFIG_1_AGGR_INTR_EN_SHIFT (4U)
/*! AGGR_INTR_EN - Aggregation Interrupt Enable 0 = only one download/upload over interrupt per
 *    aggregate 1 = generate interrupt for each PDU in aggregate
 */
#define SDU_FN_CARD_CARD_CONFIG_1_AGGR_INTR_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_AGGR_INTR_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_AGGR_INTR_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_UPDATE_MASK (0x20U)
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_UPDATE_SHIFT (5U)
/*! EXPLICIT_DNLD_UPDATE - Explicit Download Update 0 = bitmap update is done on completion of CMD53
 *    write 1 = bitmap update is done when host clears download ready interrupt
 */
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_UPDATE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_UPDATE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_DNLD_UPDATE_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_UPDATE_MASK (0x40U)
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_UPDATE_SHIFT (6U)
/*! EXPLICIT_UPLD_UPDATE - Explicit Upload Update 0 = bitmap update is done on completion of CMD53
 *    read 1 = bitmap update is done when host clears upload
 */
#define SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_UPDATE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_UPDATE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_EXPLICIT_UPLD_UPDATE_MASK)

#define SDU_FN_CARD_CARD_CONFIG_1_IOE_WAKEUP_EN_MASK (0x80U)
#define SDU_FN_CARD_CARD_CONFIG_1_IOE_WAKEUP_EN_SHIFT (7U)
/*! IOE_WAKEUP_EN - If this is enabled, the 0-to-1 transition of the IO_ENABLE register bit of each
 *    function that is written by host will generate a function-specific wakeup event to the APU.
 */
#define SDU_FN_CARD_CARD_CONFIG_1_IOE_WAKEUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG_1_IOE_WAKEUP_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG_1_IOE_WAKEUP_EN_MASK)
/*! @} */

/*! @name CARD_CONFIG2_0 - Card Config2 0 */
/*! @{ */

#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_FORCE_EN_MASK (0x1U)
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_FORCE_EN_SHIFT (0U)
/*! CMD53_WR_BUSY_FORCE_EN - If this is enabled, write busy signal can be forced to 1 or 0
 *    (specified by cmd53_wr_busy_force_val) and sent to host during cmd53 write operation.
 */
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_FORCE_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_FORCE_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_FORCE_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_SQ_ADDR_WRAP_EN_MASK (0x2U)
#define SDU_FN_CARD_CARD_CONFIG2_0_SQ_ADDR_WRAP_EN_SHIFT (1U)
/*! SQ_ADDR_WRAP_EN - If this is enabled, the DMA address of the current cmd53 access is wrapped
 *    back to its starting address once it reaches the end address.
 */
#define SDU_FN_CARD_CARD_CONFIG2_0_SQ_ADDR_WRAP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_SQ_ADDR_WRAP_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_SQ_ADDR_WRAP_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_SD_POSEDGE_DRV_FORCE_EN_MASK (0x4U)
#define SDU_FN_CARD_CARD_CONFIG2_0_SD_POSEDGE_DRV_FORCE_EN_SHIFT (2U)
/*! SD_POSEDGE_DRV_FORCE_EN - sd_posedge_drv_force_en */
#define SDU_FN_CARD_CARD_CONFIG2_0_SD_POSEDGE_DRV_FORCE_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_SD_POSEDGE_DRV_FORCE_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_SD_POSEDGE_DRV_FORCE_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_FORCE_ASYNC_4BIT_INT_EN_MASK (0x8U)
#define SDU_FN_CARD_CARD_CONFIG2_0_FORCE_ASYNC_4BIT_INT_EN_SHIFT (3U)
/*! FORCE_ASYNC_4BIT_INT_EN - Force asynchronous interrupt in SD 4-bit mode enable. */
#define SDU_FN_CARD_CARD_CONFIG2_0_FORCE_ASYNC_4BIT_INT_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_FORCE_ASYNC_4BIT_INT_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_FORCE_ASYNC_4BIT_INT_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_HOST_INT_AUTO_ENABLE_MASK (0x10U)
#define SDU_FN_CARD_CARD_CONFIG2_0_HOST_INT_AUTO_ENABLE_SHIFT (4U)
/*! HOST_INT_AUTO_ENABLE - If this is enabled, download and upload ready host interrupt is
 *    automatically cleared and re-enabled after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CARD_CONFIG2_0_HOST_INT_AUTO_ENABLE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_HOST_INT_AUTO_ENABLE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_HOST_INT_AUTO_ENABLE_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_ONE_BLOCK_XFRD_SLCT_MASK (0x20U)
#define SDU_FN_CARD_CARD_CONFIG2_0_ONE_BLOCK_XFRD_SLCT_SHIFT (5U)
/*! ONE_BLOCK_XFRD_SLCT - Firmware control to enable packet length issue fix for aggregation mode. */
#define SDU_FN_CARD_CARD_CONFIG2_0_ONE_BLOCK_XFRD_SLCT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_ONE_BLOCK_XFRD_SLCT_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_ONE_BLOCK_XFRD_SLCT_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_SIGNAL_MASK (0x40U)
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_SIGNAL_SHIFT (6U)
/*! CMD53_WR_BUSY_SIGNAL - Generate cmd53 write busy signal 0 = Write busy signal is not generated
 *    on SD_DAT[0] line after each block of cmd53 write.
 */
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_SIGNAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_SIGNAL_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_SIGNAL_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_WIDTH_MASK (0x80U)
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_WIDTH_SHIFT (7U)
/*! CMD53_WR_BUSY_WIDTH - cmd53_wr_busy_width */
#define SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_WIDTH(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_WIDTH_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_0_CMD53_WR_BUSY_WIDTH_MASK)
/*! @} */

/*! @name CARD_CONFIG2_1 - Card Config2 1 */
/*! @{ */

#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_NEW_MODE_MASK (0x1U)
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_NEW_MODE_SHIFT (0U)
/*! CMD53_NEW_MODE - Enable new encoding scheme of the 17b register address field in CMD53. */
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_NEW_MODE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_CMD53_NEW_MODE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_CMD53_NEW_MODE_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_1_SD_POSEDGE_DRV_FORCE_VAL_MASK (0x2U)
#define SDU_FN_CARD_CARD_CONFIG2_1_SD_POSEDGE_DRV_FORCE_VAL_SHIFT (1U)
/*! SD_POSEDGE_DRV_FORCE_VAL - If this is set to 1, SD output is driven at rising edge. */
#define SDU_FN_CARD_CARD_CONFIG2_1_SD_POSEDGE_DRV_FORCE_VAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_SD_POSEDGE_DRV_FORCE_VAL_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_SD_POSEDGE_DRV_FORCE_VAL_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_1_DNLD_RDY_AUTO_RESET_MASK (0x4U)
#define SDU_FN_CARD_CARD_CONFIG2_1_DNLD_RDY_AUTO_RESET_SHIFT (2U)
/*! DNLD_RDY_AUTO_RESET - Reset control for dnld_card_rdy event in C2H_INTEVENT 0 = download ready
 *    event is reset to 0 after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CARD_CONFIG2_1_DNLD_RDY_AUTO_RESET(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_DNLD_RDY_AUTO_RESET_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_DNLD_RDY_AUTO_RESET_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_1_UPLD_RDY_AUTO_RESET_MASK (0x8U)
#define SDU_FN_CARD_CARD_CONFIG2_1_UPLD_RDY_AUTO_RESET_SHIFT (3U)
/*! UPLD_RDY_AUTO_RESET - Reset control for upld_card_rdy event in C2H_INTEVENT 0 = upload ready
 *    event is reset to 0 after the current cmd53 is completed.
 */
#define SDU_FN_CARD_CARD_CONFIG2_1_UPLD_RDY_AUTO_RESET(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_UPLD_RDY_AUTO_RESET_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_UPLD_RDY_AUTO_RESET_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_TX_LEN_FORMAT_MASK (0x30U)
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_TX_LEN_FORMAT_SHIFT (4U)
/*! CMD53_TX_LEN_FORMAT - Define the cmd53 transmit/download length (tx_len) header format attached
 *    in the beginning of data payload: 0 = no tx_len header info 1= first 2 bytes contain tx_len
 *    info 2 = first 3 bytes contain tx_len info This field is applicable for non-aggregation mode only
 */
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_TX_LEN_FORMAT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_CMD53_TX_LEN_FORMAT_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_CMD53_TX_LEN_FORMAT_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_RD_LEN_FORMAT_MASK (0xC0U)
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_RD_LEN_FORMAT_SHIFT (6U)
/*! CMD53_RD_LEN_FORMAT - cmd53_rd_len_format */
#define SDU_FN_CARD_CARD_CONFIG2_1_CMD53_RD_LEN_FORMAT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_1_CMD53_RD_LEN_FORMAT_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_1_CMD53_RD_LEN_FORMAT_MASK)
/*! @} */

/*! @name CARD_CONFIG2_2 - Card Config2 2 */
/*! @{ */

#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_DATA_OUT_MASK (0xFU)
#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_DATA_OUT_SHIFT (0U)
/*! TEST_DATA_OUT - Test output data for SD_DAT. */
#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_DATA_OUT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_2_TEST_DATA_OUT_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_2_TEST_DATA_OUT_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_CMD_OUT_MASK (0x10U)
#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_CMD_OUT_SHIFT (4U)
/*! TEST_CMD_OUT - Test output data for SD_CMD. */
#define SDU_FN_CARD_CARD_CONFIG2_2_TEST_CMD_OUT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_2_TEST_CMD_OUT_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_2_TEST_CMD_OUT_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_BUSY_FORCE_VAL_MASK (0x20U)
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_BUSY_FORCE_VAL_SHIFT (5U)
/*! CMD53_WR_BUSY_FORCE_VAL - If cmd53_wr_busy_force_en is enabled, this register bit is used to force write busy signal to host. */
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_BUSY_FORCE_VAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_BUSY_FORCE_VAL_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_BUSY_FORCE_VAL_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_Q_EN_MASK (0x40U)
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_Q_EN_SHIFT (6U)
/*! CMD53_WR_Q_EN - If this bit is 0, multiple queue feature is disabled for CMD53 write operation. */
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_Q_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_Q_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_2_CMD53_WR_Q_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_RD_Q_EN_MASK (0x80U)
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_RD_Q_EN_SHIFT (7U)
/*! CMD53_RD_Q_EN - If this bit is 0, multiple queue feature is disabled for CMD53 read operation. */
#define SDU_FN_CARD_CARD_CONFIG2_2_CMD53_RD_Q_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_2_CMD53_RD_Q_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_2_CMD53_RD_Q_EN_MASK)
/*! @} */

/*! @name CARD_CONFIG2_3 - Card Config2 3 */
/*! @{ */

#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_DATA_EN_MASK (0xFU)
#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_DATA_EN_SHIFT (0U)
/*! TEST_DATA_EN - Test output enable for SD_DAT. */
#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_DATA_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_3_TEST_DATA_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_3_TEST_DATA_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_CMD_EN_MASK (0x10U)
#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_CMD_EN_SHIFT (4U)
/*! TEST_CMD_EN - Test output enable for SD_CMD. */
#define SDU_FN_CARD_CARD_CONFIG2_3_TEST_CMD_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_3_TEST_CMD_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_3_TEST_CMD_EN_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_3_PAD_TEST_MODE_MASK (0x20U)
#define SDU_FN_CARD_CARD_CONFIG2_3_PAD_TEST_MODE_SHIFT (5U)
/*! PAD_TEST_MODE - Enable test mode to directly drive SD_DAT and SD_CMD with test data specified by
 *    test_data_out, test_cmd_out, test_data_en, and test_cmd_en fields.
 */
#define SDU_FN_CARD_CARD_CONFIG2_3_PAD_TEST_MODE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_3_PAD_TEST_MODE_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_3_PAD_TEST_MODE_MASK)

#define SDU_FN_CARD_CARD_CONFIG2_3_CMD53_WR_BUSY_HW_CTRL_EN_MASK (0x40U)
#define SDU_FN_CARD_CARD_CONFIG2_3_CMD53_WR_BUSY_HW_CTRL_EN_SHIFT (6U)
/*! CMD53_WR_BUSY_HW_CTRL_EN - Enable cmd53_wr_busy_hw_ctrl feature for each function. */
#define SDU_FN_CARD_CARD_CONFIG2_3_CMD53_WR_BUSY_HW_CTRL_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_CARD_CONFIG2_3_CMD53_WR_BUSY_HW_CTRL_EN_SHIFT)) & SDU_FN_CARD_CARD_CONFIG2_3_CMD53_WR_BUSY_HW_CTRL_EN_MASK)
/*! @} */

/*! @name TESTBUS0 - Testbus 0 */
/*! @{ */

#define SDU_FN_CARD_TESTBUS0_TESTBUS0_MASK       (0xFFU)
#define SDU_FN_CARD_TESTBUS0_TESTBUS0_SHIFT      (0U)
/*! TESTBUS0 - SDU testbus0 */
#define SDU_FN_CARD_TESTBUS0_TESTBUS0(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_TESTBUS0_TESTBUS0_SHIFT)) & SDU_FN_CARD_TESTBUS0_TESTBUS0_MASK)
/*! @} */

/*! @name TESTBUS1 - Testbus 1 */
/*! @{ */

#define SDU_FN_CARD_TESTBUS1_TESTBUS1_MASK       (0xFFU)
#define SDU_FN_CARD_TESTBUS1_TESTBUS1_SHIFT      (0U)
/*! TESTBUS1 - SDU testbus1 */
#define SDU_FN_CARD_TESTBUS1_TESTBUS1(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_TESTBUS1_TESTBUS1_SHIFT)) & SDU_FN_CARD_TESTBUS1_TESTBUS1_MASK)
/*! @} */

/*! @name RCA0 - RCA 0 */
/*! @{ */

#define SDU_FN_CARD_RCA0_RCA0_MASK               (0xFFU)
#define SDU_FN_CARD_RCA0_RCA0_SHIFT              (0U)
/*! RCA0 - RCA[7:0] */
#define SDU_FN_CARD_RCA0_RCA0(x)                 (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RCA0_RCA0_SHIFT)) & SDU_FN_CARD_RCA0_RCA0_MASK)
/*! @} */

/*! @name RCA1 - RCA 1 */
/*! @{ */

#define SDU_FN_CARD_RCA1_RCA1_MASK               (0xFFU)
#define SDU_FN_CARD_RCA1_RCA1_SHIFT              (0U)
/*! RCA1 - RCA[15:8] */
#define SDU_FN_CARD_RCA1_RCA1(x)                 (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_RCA1_RCA1_SHIFT)) & SDU_FN_CARD_RCA1_RCA1_MASK)
/*! @} */

/*! @name DMA_ADDR0 - DMA Address 0 */
/*! @{ */

#define SDU_FN_CARD_DMA_ADDR0_DMA_ADDR0_MASK     (0xFFU)
#define SDU_FN_CARD_DMA_ADDR0_DMA_ADDR0_SHIFT    (0U)
/*! DMA_ADDR0 - DMA address [7:0] of last system bus transfer */
#define SDU_FN_CARD_DMA_ADDR0_DMA_ADDR0(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_DMA_ADDR0_DMA_ADDR0_SHIFT)) & SDU_FN_CARD_DMA_ADDR0_DMA_ADDR0_MASK)
/*! @} */

/*! @name DMA_ADDR1 - DMA Address 1 */
/*! @{ */

#define SDU_FN_CARD_DMA_ADDR1_DMA_ADDR1_MASK     (0xFFU)
#define SDU_FN_CARD_DMA_ADDR1_DMA_ADDR1_SHIFT    (0U)
/*! DMA_ADDR1 - DMA address [15:8] of last system bus transfer */
#define SDU_FN_CARD_DMA_ADDR1_DMA_ADDR1(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_DMA_ADDR1_DMA_ADDR1_SHIFT)) & SDU_FN_CARD_DMA_ADDR1_DMA_ADDR1_MASK)
/*! @} */

/*! @name DMA_ADDR2 - DMA Address 2 */
/*! @{ */

#define SDU_FN_CARD_DMA_ADDR2_DMA_ADDR2_MASK     (0xFFU)
#define SDU_FN_CARD_DMA_ADDR2_DMA_ADDR2_SHIFT    (0U)
/*! DMA_ADDR2 - DMA address [23:16] of last system bus transfer */
#define SDU_FN_CARD_DMA_ADDR2_DMA_ADDR2(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_DMA_ADDR2_DMA_ADDR2_SHIFT)) & SDU_FN_CARD_DMA_ADDR2_DMA_ADDR2_MASK)
/*! @} */

/*! @name DMA_ADDR3 - DMA Address 3 */
/*! @{ */

#define SDU_FN_CARD_DMA_ADDR3_DMA_ADDR3_MASK     (0xFFU)
#define SDU_FN_CARD_DMA_ADDR3_DMA_ADDR3_SHIFT    (0U)
/*! DMA_ADDR3 - DMA address [31:24] of last system bus transfer */
#define SDU_FN_CARD_DMA_ADDR3_DMA_ADDR3(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_DMA_ADDR3_DMA_ADDR3_SHIFT)) & SDU_FN_CARD_DMA_ADDR3_DMA_ADDR3_MASK)
/*! @} */

/*! @name IO_PORT0 - I/O Port 0 */
/*! @{ */

#define SDU_FN_CARD_IO_PORT0_IO_ADDR0_MASK       (0xFFU)
#define SDU_FN_CARD_IO_PORT0_IO_ADDR0_SHIFT      (0U)
/*! IO_ADDR0 - I/O port address [7:0] */
#define SDU_FN_CARD_IO_PORT0_IO_ADDR0(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_IO_PORT0_IO_ADDR0_SHIFT)) & SDU_FN_CARD_IO_PORT0_IO_ADDR0_MASK)
/*! @} */

/*! @name IO_PORT1 - I/O Port 1 */
/*! @{ */

#define SDU_FN_CARD_IO_PORT1_IO_ADDR1_MASK       (0xFFU)
#define SDU_FN_CARD_IO_PORT1_IO_ADDR1_SHIFT      (0U)
/*! IO_ADDR1 - I/O port address [15:8] */
#define SDU_FN_CARD_IO_PORT1_IO_ADDR1(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_IO_PORT1_IO_ADDR1_SHIFT)) & SDU_FN_CARD_IO_PORT1_IO_ADDR1_MASK)
/*! @} */

/*! @name IO_PORT2 - I/O Port 2 */
/*! @{ */

#define SDU_FN_CARD_IO_PORT2_IO_ADDR2_MASK       (0x1U)
#define SDU_FN_CARD_IO_PORT2_IO_ADDR2_SHIFT      (0U)
/*! IO_ADDR2 - I/O port address [16] */
#define SDU_FN_CARD_IO_PORT2_IO_ADDR2(x)         (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_IO_PORT2_IO_ADDR2_SHIFT)) & SDU_FN_CARD_IO_PORT2_IO_ADDR2_MASK)
/*! @} */

/*! @name SCRATCH2_0 - Scratch 2 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH2_0_SCRATCH2_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH2_0_SCRATCH2_0_SHIFT  (0U)
/*! SCRATCH2_0 - Scratch register 2 [7:0] */
#define SDU_FN_CARD_SCRATCH2_0_SCRATCH2_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH2_0_SCRATCH2_0_SHIFT)) & SDU_FN_CARD_SCRATCH2_0_SCRATCH2_0_MASK)
/*! @} */

/*! @name SCRATCH2_1 - Scratch 2 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH2_1_SCRATCH2_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH2_1_SCRATCH2_1_SHIFT  (0U)
/*! SCRATCH2_1 - Scratch register 2 [15:8] */
#define SDU_FN_CARD_SCRATCH2_1_SCRATCH2_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH2_1_SCRATCH2_1_SHIFT)) & SDU_FN_CARD_SCRATCH2_1_SCRATCH2_1_MASK)
/*! @} */

/*! @name SCRATCH2_2 - Scratch 2 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH2_2_SCRATCH2_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH2_2_SCRATCH2_2_SHIFT  (0U)
/*! SCRATCH2_2 - Scratch register 2 [23:16] */
#define SDU_FN_CARD_SCRATCH2_2_SCRATCH2_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH2_2_SCRATCH2_2_SHIFT)) & SDU_FN_CARD_SCRATCH2_2_SCRATCH2_2_MASK)
/*! @} */

/*! @name SCRATCH2_3 - Scratch 2 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH2_3_SCRATCH2_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH2_3_SCRATCH2_3_SHIFT  (0U)
/*! SCRATCH2_3 - Scratch register 2 [31:24] */
#define SDU_FN_CARD_SCRATCH2_3_SCRATCH2_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH2_3_SCRATCH2_3_SHIFT)) & SDU_FN_CARD_SCRATCH2_3_SCRATCH2_3_MASK)
/*! @} */

/*! @name SCRATCH3_0 - Scratch 3 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH3_0_SCRATCH3_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH3_0_SCRATCH3_0_SHIFT  (0U)
/*! SCRATCH3_0 - Scratch register 3 [7:0] */
#define SDU_FN_CARD_SCRATCH3_0_SCRATCH3_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH3_0_SCRATCH3_0_SHIFT)) & SDU_FN_CARD_SCRATCH3_0_SCRATCH3_0_MASK)
/*! @} */

/*! @name SCRATCH3_1 - Scratch 3 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH3_1_SCRATCH3_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH3_1_SCRATCH3_1_SHIFT  (0U)
/*! SCRATCH3_1 - Scratch register 3 [15:8] */
#define SDU_FN_CARD_SCRATCH3_1_SCRATCH3_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH3_1_SCRATCH3_1_SHIFT)) & SDU_FN_CARD_SCRATCH3_1_SCRATCH3_1_MASK)
/*! @} */

/*! @name SCRATCH3_2 - Scratch 3 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH3_2_SCRATCH3_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH3_2_SCRATCH3_2_SHIFT  (0U)
/*! SCRATCH3_2 - Scratch register 3 [23:16] */
#define SDU_FN_CARD_SCRATCH3_2_SCRATCH3_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH3_2_SCRATCH3_2_SHIFT)) & SDU_FN_CARD_SCRATCH3_2_SCRATCH3_2_MASK)
/*! @} */

/*! @name SCRATCH3_3 - Scratch 3 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH3_3_SCRATCH3_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH3_3_SCRATCH3_3_SHIFT  (0U)
/*! SCRATCH3_3 - Scratch register 3 [31:24] */
#define SDU_FN_CARD_SCRATCH3_3_SCRATCH3_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH3_3_SCRATCH3_3_SHIFT)) & SDU_FN_CARD_SCRATCH3_3_SCRATCH3_3_MASK)
/*! @} */

/*! @name SCRATCH4_0 - Scratch 4 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH4_0_SCRATCH4_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH4_0_SCRATCH4_0_SHIFT  (0U)
/*! SCRATCH4_0 - Scratch register 4 [7:0] */
#define SDU_FN_CARD_SCRATCH4_0_SCRATCH4_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH4_0_SCRATCH4_0_SHIFT)) & SDU_FN_CARD_SCRATCH4_0_SCRATCH4_0_MASK)
/*! @} */

/*! @name SCRATCH4_1 - Scratch 4 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH4_1_SCRATCH4_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH4_1_SCRATCH4_1_SHIFT  (0U)
/*! SCRATCH4_1 - Scratch register 4 [15:8] */
#define SDU_FN_CARD_SCRATCH4_1_SCRATCH4_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH4_1_SCRATCH4_1_SHIFT)) & SDU_FN_CARD_SCRATCH4_1_SCRATCH4_1_MASK)
/*! @} */

/*! @name SCRATCH4_2 - Scratch 4 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH4_2_SCRATCH4_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH4_2_SCRATCH4_2_SHIFT  (0U)
/*! SCRATCH4_2 - Scratch register 4 [23:16] */
#define SDU_FN_CARD_SCRATCH4_2_SCRATCH4_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH4_2_SCRATCH4_2_SHIFT)) & SDU_FN_CARD_SCRATCH4_2_SCRATCH4_2_MASK)
/*! @} */

/*! @name SCRATCH4_3 - Scratch 4 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH4_3_SCRATCH4_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH4_3_SCRATCH4_3_SHIFT  (0U)
/*! SCRATCH4_3 - Scratch register 4 [31:24] */
#define SDU_FN_CARD_SCRATCH4_3_SCRATCH4_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH4_3_SCRATCH4_3_SHIFT)) & SDU_FN_CARD_SCRATCH4_3_SCRATCH4_3_MASK)
/*! @} */

/*! @name SCRATCH5_0 - Scratch 5 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH5_0_SCRATCH5_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH5_0_SCRATCH5_0_SHIFT  (0U)
/*! SCRATCH5_0 - Scratch register 5 [7:0] */
#define SDU_FN_CARD_SCRATCH5_0_SCRATCH5_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH5_0_SCRATCH5_0_SHIFT)) & SDU_FN_CARD_SCRATCH5_0_SCRATCH5_0_MASK)
/*! @} */

/*! @name SCRATCH5_1 - Scratch 5 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH5_1_SCRATCH5_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH5_1_SCRATCH5_1_SHIFT  (0U)
/*! SCRATCH5_1 - Scratch register 5 [15:8] */
#define SDU_FN_CARD_SCRATCH5_1_SCRATCH5_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH5_1_SCRATCH5_1_SHIFT)) & SDU_FN_CARD_SCRATCH5_1_SCRATCH5_1_MASK)
/*! @} */

/*! @name SCRATCH5_2 - Scratch 5 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH5_2_SCRATCH5_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH5_2_SCRATCH5_2_SHIFT  (0U)
/*! SCRATCH5_2 - Scratch register 5 [23:16] */
#define SDU_FN_CARD_SCRATCH5_2_SCRATCH5_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH5_2_SCRATCH5_2_SHIFT)) & SDU_FN_CARD_SCRATCH5_2_SCRATCH5_2_MASK)
/*! @} */

/*! @name SCRATCH5_3 - Scratch 5 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH5_3_SCRATCH5_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH5_3_SCRATCH5_3_SHIFT  (0U)
/*! SCRATCH5_3 - Scratch register 5 [31:24] */
#define SDU_FN_CARD_SCRATCH5_3_SCRATCH5_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH5_3_SCRATCH5_3_SHIFT)) & SDU_FN_CARD_SCRATCH5_3_SCRATCH5_3_MASK)
/*! @} */

/*! @name SCRATCH6_0 - Scratch 6 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH6_0_SCRATCH6_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH6_0_SCRATCH6_0_SHIFT  (0U)
/*! SCRATCH6_0 - Scratch register 6 [7:0] */
#define SDU_FN_CARD_SCRATCH6_0_SCRATCH6_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH6_0_SCRATCH6_0_SHIFT)) & SDU_FN_CARD_SCRATCH6_0_SCRATCH6_0_MASK)
/*! @} */

/*! @name SCRATCH6_1 - Scratch 6 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH6_1_SCRATCH6_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH6_1_SCRATCH6_1_SHIFT  (0U)
/*! SCRATCH6_1 - Scratch register 6 [15:8] */
#define SDU_FN_CARD_SCRATCH6_1_SCRATCH6_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH6_1_SCRATCH6_1_SHIFT)) & SDU_FN_CARD_SCRATCH6_1_SCRATCH6_1_MASK)
/*! @} */

/*! @name SCRATCH6_2 - Scratch 6 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH6_2_SCRATCH6_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH6_2_SCRATCH6_2_SHIFT  (0U)
/*! SCRATCH6_2 - Scratch register 6 [23:16] */
#define SDU_FN_CARD_SCRATCH6_2_SCRATCH6_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH6_2_SCRATCH6_2_SHIFT)) & SDU_FN_CARD_SCRATCH6_2_SCRATCH6_2_MASK)
/*! @} */

/*! @name SCRATCH6_3 - Scratch 6 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH6_3_SCRATCH6_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH6_3_SCRATCH6_3_SHIFT  (0U)
/*! SCRATCH6_3 - Scratch register 6 [31:24] */
#define SDU_FN_CARD_SCRATCH6_3_SCRATCH6_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH6_3_SCRATCH6_3_SHIFT)) & SDU_FN_CARD_SCRATCH6_3_SCRATCH6_3_MASK)
/*! @} */

/*! @name SCRATCH7_0 - Scratch 7 0 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH7_0_SCRATCH7_0_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH7_0_SCRATCH7_0_SHIFT  (0U)
/*! SCRATCH7_0 - Scratch register 7 [7:0] */
#define SDU_FN_CARD_SCRATCH7_0_SCRATCH7_0(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH7_0_SCRATCH7_0_SHIFT)) & SDU_FN_CARD_SCRATCH7_0_SCRATCH7_0_MASK)
/*! @} */

/*! @name SCRATCH7_1 - Scratch 7 1 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH7_1_SCRATCH7_1_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH7_1_SCRATCH7_1_SHIFT  (0U)
/*! SCRATCH7_1 - Scratch register 7 [15:8] */
#define SDU_FN_CARD_SCRATCH7_1_SCRATCH7_1(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH7_1_SCRATCH7_1_SHIFT)) & SDU_FN_CARD_SCRATCH7_1_SCRATCH7_1_MASK)
/*! @} */

/*! @name SCRATCH7_2 - Scratch 7 2 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH7_2_SCRATCH7_2_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH7_2_SCRATCH7_2_SHIFT  (0U)
/*! SCRATCH7_2 - Scratch register 7 [23:16] */
#define SDU_FN_CARD_SCRATCH7_2_SCRATCH7_2(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH7_2_SCRATCH7_2_SHIFT)) & SDU_FN_CARD_SCRATCH7_2_SCRATCH7_2_MASK)
/*! @} */

/*! @name SCRATCH7_3 - Scratch 7 3 */
/*! @{ */

#define SDU_FN_CARD_SCRATCH7_3_SCRATCH7_3_MASK   (0xFFU)
#define SDU_FN_CARD_SCRATCH7_3_SCRATCH7_3_SHIFT  (0U)
/*! SCRATCH7_3 - Scratch register 7 [31:24] */
#define SDU_FN_CARD_SCRATCH7_3_SCRATCH7_3(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN_CARD_SCRATCH7_3_SCRATCH7_3_SHIFT)) & SDU_FN_CARD_SCRATCH7_3_SCRATCH7_3_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SDU_FN_CARD_Register_Masks */


/*!
 * @}
 */ /* end of group SDU_FN_CARD_Peripheral_Access_Layer */


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


#endif  /* SDU_FN_CARD_H_ */

