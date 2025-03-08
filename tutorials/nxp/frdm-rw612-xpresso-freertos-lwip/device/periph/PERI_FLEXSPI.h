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
**     Build:               b241017
**
**     Abstract:
**         CMSIS Peripheral Access Layer for FLEXSPI
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
 * @file FLEXSPI.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for FLEXSPI
 *
 * CMSIS Peripheral Access Layer for FLEXSPI
 */

#if !defined(FLEXSPI_H_)
#define FLEXSPI_H_                               /**< Symbol preventing repeated inclusion */

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
   -- FLEXSPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FLEXSPI_Peripheral_Access_Layer FLEXSPI Peripheral Access Layer
 * @{
 */

/** FLEXSPI - Size of Registers Arrays */
#define FLEXSPI_AHBRXBUFXCR0_COUNT                8u
#define FLEXSPI_FLSHXCR0_COUNT                    4u
#define FLEXSPI_FLSHXCR1_COUNT                    4u
#define FLEXSPI_FLSHXCR2_COUNT                    4u
#define FLEXSPI_DLLXCR_COUNT                      2u
#define FLEXSPI_RFDR_COUNT                        32u
#define FLEXSPI_TFDR_COUNT                        32u
#define FLEXSPI_LUT_COUNT                         64u
#define FLEXSPI_IPEDCTXCTRLX_COUNT                2u

/** FLEXSPI - Register Layout Typedef */
typedef struct {
  __IO uint32_t MCR0;                              /**< Module Control Register 0, offset: 0x0 */
  __IO uint32_t MCR1;                              /**< Module Control Register 1, offset: 0x4 */
  __IO uint32_t MCR2;                              /**< Module Control Register 2, offset: 0x8 */
  __IO uint32_t AHBCR;                             /**< AHB Bus Control Register, offset: 0xC */
  __IO uint32_t INTEN;                             /**< Interrupt Enable Register, offset: 0x10 */
  __IO uint32_t INTR;                              /**< Interrupt Register, offset: 0x14 */
  __IO uint32_t LUTKEY;                            /**< LUT Key Register, offset: 0x18 */
  __IO uint32_t LUTCR;                             /**< LUT Control Register, offset: 0x1C */
  __IO uint32_t AHBRXBUFCR0[FLEXSPI_AHBRXBUFXCR0_COUNT]; /**< AHB RX Buffer 0 Control Register 0..AHB RX Buffer 7 Control Register 0, array offset: 0x20, array step: 0x4 */
       uint8_t RESERVED_0[32];
  __IO uint32_t FLSHCR0[FLEXSPI_FLSHXCR0_COUNT];   /**< Flash Control Register 0, array offset: 0x60, array step: 0x4 */
  __IO uint32_t FLSHCR1[FLEXSPI_FLSHXCR1_COUNT];   /**< Flash Control Register 1, array offset: 0x70, array step: 0x4 */
  __IO uint32_t FLSHCR2[FLEXSPI_FLSHXCR2_COUNT];   /**< Flash Control Register 2, array offset: 0x80, array step: 0x4 */
       uint8_t RESERVED_1[4];
  __IO uint32_t FLSHCR4;                           /**< Flash Control Register 4, offset: 0x94 */
       uint8_t RESERVED_2[8];
  __IO uint32_t IPCR0;                             /**< IP Control Register 0, offset: 0xA0 */
  __IO uint32_t IPCR1;                             /**< IP Control Register 1, offset: 0xA4 */
  __IO uint32_t IPCR2;                             /**< IP Control Register 2, offset: 0xA8 */
       uint8_t RESERVED_3[4];
  __IO uint32_t IPCMD;                             /**< IP Command Register, offset: 0xB0 */
  __IO uint32_t DLPR;                              /**< Data Learn Pattern Register, offset: 0xB4 */
  __IO uint32_t IPRXFCR;                           /**< IP RX FIFO Control Register, offset: 0xB8 */
  __IO uint32_t IPTXFCR;                           /**< IP TX FIFO Control Register, offset: 0xBC */
  __IO uint32_t DLLCR[FLEXSPI_DLLXCR_COUNT];       /**< DLL Control Register 0, array offset: 0xC0, array step: 0x4 */
       uint8_t RESERVED_4[24];
  __I  uint32_t STS0;                              /**< Status Register 0, offset: 0xE0 */
  __I  uint32_t STS1;                              /**< Status Register 1, offset: 0xE4 */
  __I  uint32_t STS2;                              /**< Status Register 2, offset: 0xE8 */
  __I  uint32_t AHBSPNDSTS;                        /**< AHB Suspend Status Register, offset: 0xEC */
  __I  uint32_t IPRXFSTS;                          /**< IP RX FIFO Status Register, offset: 0xF0 */
  __I  uint32_t IPTXFSTS;                          /**< IP TX FIFO Status Register, offset: 0xF4 */
       uint8_t RESERVED_5[8];
  __I  uint32_t RFDR[FLEXSPI_RFDR_COUNT];          /**< IP RX FIFO Data Register 0..IP RX FIFO Data Register 31, array offset: 0x100, array step: 0x4 */
  __O  uint32_t TFDR[FLEXSPI_TFDR_COUNT];          /**< IP TX FIFO Data Register 0..IP TX FIFO Data Register 31, array offset: 0x180, array step: 0x4 */
  __IO uint32_t LUT[FLEXSPI_LUT_COUNT];            /**< LUT 0..LUT 63, array offset: 0x200, array step: 0x4 */
       uint8_t RESERVED_6[288];
  __IO uint32_t HADDRSTART;                        /**< HADDR REMAP START ADDR, offset: 0x420 */
  __IO uint32_t HADDREND;                          /**< HADDR REMAP END ADDR, offset: 0x424 */
  __IO uint32_t HADDROFFSET;                       /**< HADDR REMAP OFFSET, offset: 0x428 */
  __IO uint32_t IPEDCTRL;                          /**< IPED function control, offset: 0x42C */
  __IO uint32_t IPSNSZSTART0;                      /**< IPS nonsecure region Start address of region 0, offset: 0x430 */
  __IO uint32_t IPSNSZEND0;                        /**< IPS nonsecure region End address of region 0, offset: 0x434 */
  __IO uint32_t IPSNSZSTART1;                      /**< IPS nonsecure region Start address of region 1, offset: 0x438 */
  __IO uint32_t IPSNSZEND1;                        /**< IPS nonsecure region End address of region 1, offset: 0x43C */
  __IO uint32_t AHBBUFREGIONSTART0;                /**< RX BUF Start address of region 0, offset: 0x440 */
  __IO uint32_t AHBBUFREGIONEND0;                  /**< RX BUF region End address of region 0, offset: 0x444 */
  __IO uint32_t AHBBUFREGIONSTART1;                /**< RX BUF Start address of region 1, offset: 0x448 */
  __IO uint32_t AHBBUFREGIONEND1;                  /**< RX BUF region End address of region 1, offset: 0x44C */
  __IO uint32_t AHBBUFREGIONSTART2;                /**< RX BUF Start address of region 2, offset: 0x450 */
  __IO uint32_t AHBBUFREGIONEND2;                  /**< RX BUF region End address of region 2, offset: 0x454 */
  __IO uint32_t AHBBUFREGIONSTART3;                /**< RX BUF Start address of region 3, offset: 0x458 */
  __IO uint32_t AHBBUFREGIONEND3;                  /**< RX BUF region End address of region 3, offset: 0x45C */
       uint8_t RESERVED_7[160];
  __IO uint32_t IPEDCTXCTRL[FLEXSPI_IPEDCTXCTRLX_COUNT]; /**< IPED context control 0..IPED context control 1, array offset: 0x500, array step: 0x4 */
       uint8_t RESERVED_8[24];
  __IO uint32_t IPEDCTX0IV0;                       /**< IPED context0 IV0, offset: 0x520 */
  __IO uint32_t IPEDCTX0IV1;                       /**< IPED context0 IV1, offset: 0x524 */
  __IO uint32_t IPEDCTX0START;                     /**< Start address of region 0, offset: 0x528 */
  __IO uint32_t IPEDCTX0END;                       /**< End address of region 0, offset: 0x52C */
  __IO uint32_t IPEDCTX0AAD0;                      /**< IPED context0 AAD0, offset: 0x530 */
  __IO uint32_t IPEDCTX0AAD1;                      /**< IPED context0 AAD1, offset: 0x534 */
       uint8_t RESERVED_9[8];
  __IO uint32_t IPEDCTX1IV0;                       /**< IPED context1 IV0, offset: 0x540 */
  __IO uint32_t IPEDCTX1IV1;                       /**< IPED context1 IV1, offset: 0x544 */
  __IO uint32_t IPEDCTX1START;                     /**< Start address of region 1, offset: 0x548 */
  __IO uint32_t IPEDCTX1END;                       /**< End address of region 1, offset: 0x54C */
  __IO uint32_t IPEDCTX1AAD0;                      /**< IPED context1 AAD0, offset: 0x550 */
  __IO uint32_t IPEDCTX1AAD1;                      /**< IPED context1 AAD1, offset: 0x554 */
       uint8_t RESERVED_10[8];
  __IO uint32_t IPEDCTX2IV0;                       /**< IPED context2 IV0, offset: 0x560 */
  __IO uint32_t IPEDCTX2IV1;                       /**< IPED context2 IV1, offset: 0x564 */
  __IO uint32_t IPEDCTX2START;                     /**< Start address of region 2, offset: 0x568 */
  __IO uint32_t IPEDCTX2END;                       /**< End address of region 2, offset: 0x56C */
  __IO uint32_t IPEDCTX2AAD0;                      /**< IPED context2 AAD0, offset: 0x570 */
  __IO uint32_t IPEDCTX2AAD1;                      /**< IPED context2 AAD1, offset: 0x574 */
       uint8_t RESERVED_11[8];
  __IO uint32_t IPEDCTX3IV0;                       /**< IPED context3 IV0, offset: 0x580 */
  __IO uint32_t IPEDCTX3IV1;                       /**< IPED context3 IV1, offset: 0x584 */
  __IO uint32_t IPEDCTX3START;                     /**< Start address of region 3, offset: 0x588 */
  __IO uint32_t IPEDCTX3END;                       /**< End address of region 3, offset: 0x58C */
  __IO uint32_t IPEDCTX3AAD0;                      /**< IPED context3 AAD0, offset: 0x590 */
  __IO uint32_t IPEDCTX3AAD1;                      /**< IPED context3 AAD1, offset: 0x594 */
       uint8_t RESERVED_12[8];
  __IO uint32_t IPEDCTX4IV0;                       /**< IPED context4 IV0, offset: 0x5A0 */
  __IO uint32_t IPEDCTX4IV1;                       /**< IPED context4 IV1, offset: 0x5A4 */
  __IO uint32_t IPEDCTX4START;                     /**< Start address of region 4, offset: 0x5A8 */
  __IO uint32_t IPEDCTX4END;                       /**< End address of region 4, offset: 0x5AC */
  __IO uint32_t IPEDCTX4AAD0;                      /**< IPED context4 AAD0, offset: 0x5B0 */
  __IO uint32_t IPEDCTX4AAD1;                      /**< IPED context4 AAD1, offset: 0x5B4 */
       uint8_t RESERVED_13[8];
  __IO uint32_t IPEDCTX5IV0;                       /**< IPED context5 IV0, offset: 0x5C0 */
  __IO uint32_t IPEDCTX5IV1;                       /**< IPED context5 IV1, offset: 0x5C4 */
  __IO uint32_t IPEDCTX5START;                     /**< Start address of region 5, offset: 0x5C8 */
  __IO uint32_t IPEDCTX5END;                       /**< End address of region 5, offset: 0x5CC */
  __IO uint32_t IPEDCTX5AAD0;                      /**< IPED context5 AAD0, offset: 0x5D0 */
  __IO uint32_t IPEDCTX5AAD1;                      /**< IPED context5 AAD1, offset: 0x5D4 */
       uint8_t RESERVED_14[8];
  __IO uint32_t IPEDCTX6IV0;                       /**< IPED context6 IV0, offset: 0x5E0 */
  __IO uint32_t IPEDCTX6IV1;                       /**< IPED context6 IV1, offset: 0x5E4 */
  __IO uint32_t IPEDCTX6START;                     /**< Start address of region 6, offset: 0x5E8 */
  __IO uint32_t IPEDCTX6END;                       /**< End address of region 6, offset: 0x5EC */
  __IO uint32_t IPEDCTX6AAD0;                      /**< IPED context6 AAD0, offset: 0x5F0 */
  __IO uint32_t IPEDCTX6AAD1;                      /**< IPED context6 AAD1, offset: 0x5F4 */
       uint8_t RESERVED_15[8];
  __IO uint32_t IPEDCTX7IV0;                       /**< IPED context7 IV0, offset: 0x600 */
  __IO uint32_t IPEDCTX7IV1;                       /**< IPED context7 IV1, offset: 0x604 */
  __IO uint32_t IPEDCTX7START;                     /**< Start address of region 7, offset: 0x608 */
  __IO uint32_t IPEDCTX7END;                       /**< End address of region 7, offset: 0x60C */
  __IO uint32_t IPEDCTX7AAD0;                      /**< IPED context7 AAD0, offset: 0x610 */
  __IO uint32_t IPEDCTX7AAD1;                      /**< IPED context7 AAD1, offset: 0x614 */
       uint8_t RESERVED_16[8];
  __IO uint32_t IPEDCTX8IV0;                       /**< IPED context8 IV0, offset: 0x620 */
  __IO uint32_t IPEDCTX8IV1;                       /**< IPED context8 IV1, offset: 0x624 */
  __IO uint32_t IPEDCTX8START;                     /**< Start address of region 8, offset: 0x628 */
  __IO uint32_t IPEDCTX8END;                       /**< End address of region 8, offset: 0x62C */
  __IO uint32_t IPEDCTX8AAD0;                      /**< IPED context8 AAD0, offset: 0x630 */
  __IO uint32_t IPEDCTX8AAD1;                      /**< IPED context8 AAD1, offset: 0x634 */
       uint8_t RESERVED_17[8];
  __IO uint32_t IPEDCTX9IV0;                       /**< IPED context9 IV0, offset: 0x640 */
  __IO uint32_t IPEDCTX9IV1;                       /**< IPED context9 IV1, offset: 0x644 */
  __IO uint32_t IPEDCTX9START;                     /**< Start address of region 9, offset: 0x648 */
  __IO uint32_t IPEDCTX9END;                       /**< End address of region 9, offset: 0x64C */
  __IO uint32_t IPEDCTX9AAD0;                      /**< IPED context9 AAD0, offset: 0x650 */
  __IO uint32_t IPEDCTX9AAD1;                      /**< IPED context9 AAD1, offset: 0x654 */
       uint8_t RESERVED_18[8];
  __IO uint32_t IPEDCTX10IV0;                      /**< IPED context10 IV0, offset: 0x660 */
  __IO uint32_t IPEDCTX10IV1;                      /**< IPED context10 IV1, offset: 0x664 */
  __IO uint32_t IPEDCTX10START;                    /**< Start address of region 10, offset: 0x668 */
  __IO uint32_t IPEDCTX10END;                      /**< End address of region 10, offset: 0x66C */
  __IO uint32_t IPEDCTX10AAD0;                     /**< IPED context10 AAD0, offset: 0x670 */
  __IO uint32_t IPEDCTX10AAD1;                     /**< IPED context10 AAD1, offset: 0x674 */
       uint8_t RESERVED_19[8];
  __IO uint32_t IPEDCTX11IV0;                      /**< IPED context11 IV0, offset: 0x680 */
  __IO uint32_t IPEDCTX11IV1;                      /**< IPED context11 IV1, offset: 0x684 */
  __IO uint32_t IPEDCTX11START;                    /**< Start address of region 11, offset: 0x688 */
  __IO uint32_t IPEDCTX11END;                      /**< End address of region 11, offset: 0x68C */
  __IO uint32_t IPEDCTX11AAD0;                     /**< IPED context11 AAD0, offset: 0x690 */
  __IO uint32_t IPEDCTX11AAD1;                     /**< IPED context11 AAD1, offset: 0x694 */
       uint8_t RESERVED_20[8];
  __IO uint32_t IPEDCTX12IV0;                      /**< IPED context12 IV0, offset: 0x6A0 */
  __IO uint32_t IPEDCTX12IV1;                      /**< IPED context12 IV1, offset: 0x6A4 */
  __IO uint32_t IPEDCTX12START;                    /**< Start address of region 12, offset: 0x6A8 */
  __IO uint32_t IPEDCTX12END;                      /**< End address of region 12, offset: 0x6AC */
  __IO uint32_t IPEDCTX12AAD0;                     /**< IPED context12 AAD0, offset: 0x6B0 */
  __IO uint32_t IPEDCTX12AAD1;                     /**< IPED context12 AAD1, offset: 0x6B4 */
       uint8_t RESERVED_21[8];
  __IO uint32_t IPEDCTX13IV0;                      /**< IPED context13 IV0, offset: 0x6C0 */
  __IO uint32_t IPEDCTX13IV1;                      /**< IPED context13 IV1, offset: 0x6C4 */
  __IO uint32_t IPEDCTX13START;                    /**< Start address of region 13, offset: 0x6C8 */
  __IO uint32_t IPEDCTX13END;                      /**< End address of region 13, offset: 0x6CC */
  __IO uint32_t IPEDCTX13AAD0;                     /**< IPED context13 AAD0, offset: 0x6D0 */
  __IO uint32_t IPEDCTX13AAD1;                     /**< IPED context13 AAD1, offset: 0x6D4 */
       uint8_t RESERVED_22[8];
  __IO uint32_t IPEDCTX14IV0;                      /**< IPED context14 IV0, offset: 0x6E0 */
  __IO uint32_t IPEDCTX14IV1;                      /**< IPED context14 IV1, offset: 0x6E4 */
  __IO uint32_t IPEDCTX14START;                    /**< Start address of region 14, offset: 0x6E8 */
  __IO uint32_t IPEDCTX14END;                      /**< End address of region 14, offset: 0x6EC */
  __IO uint32_t IPEDCTX14AAD0;                     /**< IPED context14 AAD0, offset: 0x6F0 */
  __IO uint32_t IPEDCTX14AAD1;                     /**< IPED context14 AAD1, offset: 0x6F4 */
} FLEXSPI_Type;

/* ----------------------------------------------------------------------------
   -- FLEXSPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FLEXSPI_Register_Masks FLEXSPI Register Masks
 * @{
 */

/*! @name MCR0 - Module Control Register 0 */
/*! @{ */

#define FLEXSPI_MCR0_SWRESET_MASK                (0x1U)
#define FLEXSPI_MCR0_SWRESET_SHIFT               (0U)
/*! SWRESET - Software Reset */
#define FLEXSPI_MCR0_SWRESET(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_SWRESET_SHIFT)) & FLEXSPI_MCR0_SWRESET_MASK)

#define FLEXSPI_MCR0_MDIS_MASK                   (0x2U)
#define FLEXSPI_MCR0_MDIS_SHIFT                  (1U)
/*! MDIS - Module Disable */
#define FLEXSPI_MCR0_MDIS(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_MDIS_SHIFT)) & FLEXSPI_MCR0_MDIS_MASK)

#define FLEXSPI_MCR0_RXCLKSRC_MASK               (0x30U)
#define FLEXSPI_MCR0_RXCLKSRC_SHIFT              (4U)
/*! RXCLKSRC - Sample Clock source selection for Flash Reading
 *  0b00..Dummy Read strobe generated by FlexSPI Controller and loopback internally.
 *  0b01..Dummy Read strobe generated by FlexSPI Controller and loopback from DQS pad.
 *  0b10..Reserved
 *  0b11..Flash provided Read strobe and input from DQS pad
 */
#define FLEXSPI_MCR0_RXCLKSRC(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_RXCLKSRC_SHIFT)) & FLEXSPI_MCR0_RXCLKSRC_MASK)

#define FLEXSPI_MCR0_ARDFEN_MASK                 (0x40U)
#define FLEXSPI_MCR0_ARDFEN_SHIFT                (6U)
/*! ARDFEN - Enable AHB bus Read Access to IP RX FIFO.
 *  0b0..IP RX FIFO should be read by IP Bus. AHB Bus read access to IP RX FIFO memory space will get bus error response.
 *  0b1..IP RX FIFO should be read by AHB Bus. IP Bus read access to IP RX FIFO memory space will always return data zero but no bus error response.
 */
#define FLEXSPI_MCR0_ARDFEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_ARDFEN_SHIFT)) & FLEXSPI_MCR0_ARDFEN_MASK)

#define FLEXSPI_MCR0_ATDFEN_MASK                 (0x80U)
#define FLEXSPI_MCR0_ATDFEN_SHIFT                (7U)
/*! ATDFEN - Enable AHB bus Write Access to IP TX FIFO.
 *  0b0..IP TX FIFO should be written by IP Bus. AHB Bus write access to IP TX FIFO memory space will get bus error response.
 *  0b1..IP TX FIFO should be written by AHB Bus. IP Bus write access to IP TX FIFO memory space will be ignored but no bus error response.
 */
#define FLEXSPI_MCR0_ATDFEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_ATDFEN_SHIFT)) & FLEXSPI_MCR0_ATDFEN_MASK)

#define FLEXSPI_MCR0_SERCLKDIV_MASK              (0x700U)
#define FLEXSPI_MCR0_SERCLKDIV_SHIFT             (8U)
/*! SERCLKDIV - Serial root clock
 *  0b000..Divided by 1
 *  0b001..Divided by 2
 *  0b010..Divided by 3
 *  0b011..Divided by 4
 *  0b100..Divided by 5
 *  0b101..Divided by 6
 *  0b110..Divided by 7
 *  0b111..Divided by 8
 */
#define FLEXSPI_MCR0_SERCLKDIV(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_SERCLKDIV_SHIFT)) & FLEXSPI_MCR0_SERCLKDIV_MASK)

#define FLEXSPI_MCR0_HSEN_MASK                   (0x800U)
#define FLEXSPI_MCR0_HSEN_SHIFT                  (11U)
/*! HSEN - Half Speed Serial Flash access Enable.
 *  0b0..Disable divide by 2 of serial flash clock for half speed commands.
 *  0b1..Enable divide by 2 of serial flash clock for half speed commands.
 */
#define FLEXSPI_MCR0_HSEN(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_HSEN_SHIFT)) & FLEXSPI_MCR0_HSEN_MASK)

#define FLEXSPI_MCR0_DOZEEN_MASK                 (0x1000U)
#define FLEXSPI_MCR0_DOZEEN_SHIFT                (12U)
/*! DOZEEN - Doze mode enable bit
 *  0b0..Doze mode support disabled. AHB clock and serial clock will not be gated off when there is doze mode request from system.
 *  0b1..Doze mode support enabled. AHB clock and serial clock will be gated off when there is doze mode request from system.
 */
#define FLEXSPI_MCR0_DOZEEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_DOZEEN_SHIFT)) & FLEXSPI_MCR0_DOZEEN_MASK)

#define FLEXSPI_MCR0_COMBINATIONEN_MASK          (0x2000U)
#define FLEXSPI_MCR0_COMBINATIONEN_SHIFT         (13U)
/*! COMBINATIONEN - This bit is to support Flash Octal mode access by combining Port A and B Data
 *    pins (A_DATA[3:0] and B_DATA[3:0]), when Port A and Port B are of 4 bit data width.
 *  0b0..Disable.
 *  0b1..Enable.
 */
#define FLEXSPI_MCR0_COMBINATIONEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_COMBINATIONEN_SHIFT)) & FLEXSPI_MCR0_COMBINATIONEN_MASK)

#define FLEXSPI_MCR0_SCKFREERUNEN_MASK           (0x4000U)
#define FLEXSPI_MCR0_SCKFREERUNEN_SHIFT          (14U)
/*! SCKFREERUNEN - This bit is used to force SCLK output free-running. For FPGA applications,
 *    external device may use SCLK as reference clock to its internal PLL.
 *  0b0..Disable.
 *  0b1..Enable.
 */
#define FLEXSPI_MCR0_SCKFREERUNEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_SCKFREERUNEN_SHIFT)) & FLEXSPI_MCR0_SCKFREERUNEN_MASK)

#define FLEXSPI_MCR0_LEARNEN_MASK                (0x8000U)
#define FLEXSPI_MCR0_LEARNEN_SHIFT               (15U)
/*! LEARNEN - This bit is used to enable/disable data learning feature. When data learning is
 *    disabled, the sampling clock phase 0 is always used for RX data sampling even if LEARN instruction
 *    is correctly executed.
 *  0b0..Disable.
 *  0b1..Enable.
 */
#define FLEXSPI_MCR0_LEARNEN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_LEARNEN_SHIFT)) & FLEXSPI_MCR0_LEARNEN_MASK)

#define FLEXSPI_MCR0_IPGRANTWAIT_MASK            (0xFF0000U)
#define FLEXSPI_MCR0_IPGRANTWAIT_SHIFT           (16U)
/*! IPGRANTWAIT - Timeout wait cycle for IP command grant. */
#define FLEXSPI_MCR0_IPGRANTWAIT(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_IPGRANTWAIT_SHIFT)) & FLEXSPI_MCR0_IPGRANTWAIT_MASK)

#define FLEXSPI_MCR0_AHBGRANTWAIT_MASK           (0xFF000000U)
#define FLEXSPI_MCR0_AHBGRANTWAIT_SHIFT          (24U)
/*! AHBGRANTWAIT - Timeout wait cycle for AHB command grant. */
#define FLEXSPI_MCR0_AHBGRANTWAIT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_AHBGRANTWAIT_SHIFT)) & FLEXSPI_MCR0_AHBGRANTWAIT_MASK)
/*! @} */

/*! @name MCR1 - Module Control Register 1 */
/*! @{ */

#define FLEXSPI_MCR1_AHBBUSWAIT_MASK             (0xFFFFU)
#define FLEXSPI_MCR1_AHBBUSWAIT_SHIFT            (0U)
/*! AHBBUSWAIT - AHB Bus wait */
#define FLEXSPI_MCR1_AHBBUSWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR1_AHBBUSWAIT_SHIFT)) & FLEXSPI_MCR1_AHBBUSWAIT_MASK)

#define FLEXSPI_MCR1_SEQWAIT_MASK                (0xFFFF0000U)
#define FLEXSPI_MCR1_SEQWAIT_SHIFT               (16U)
/*! SEQWAIT - Command Sequence Execution will timeout and abort after SEQWAIT * 1024 Serial Root
 *    Clock cycles. When sequence execution timeout occurs, there will be an interrupt generated
 *    (INTR[SEQTIMEOUT]) if this interrupt is enabled (INTEN[SEQTIMEOUTEN] is set 0x1) and AHB command is
 *    ignored by arbitrator.
 */
#define FLEXSPI_MCR1_SEQWAIT(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR1_SEQWAIT_SHIFT)) & FLEXSPI_MCR1_SEQWAIT_MASK)
/*! @} */

/*! @name MCR2 - Module Control Register 2 */
/*! @{ */

#define FLEXSPI_MCR2_CLRAHBBUFOPT_MASK           (0x800U)
#define FLEXSPI_MCR2_CLRAHBBUFOPT_SHIFT          (11U)
/*! CLRAHBBUFOPT - Clear AHB buffer
 *  0b0..AHB RX/TX Buffer will not be cleaned automatically when FlexSPI return Stop mode ACK.
 *  0b1..AHB RX/TX Buffer will be cleaned automatically when FlexSPI return Stop mode ACK.
 */
#define FLEXSPI_MCR2_CLRAHBBUFOPT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_CLRAHBBUFOPT_SHIFT)) & FLEXSPI_MCR2_CLRAHBBUFOPT_MASK)

#define FLEXSPI_MCR2_CLRLEARNPHASE_MASK          (0x4000U)
#define FLEXSPI_MCR2_CLRLEARNPHASE_SHIFT         (14U)
/*! CLRLEARNPHASE - The sampling clock phase selection will be reset to phase 0 when this bit is
 *    written with 0x1. This bit will be auto-cleared immediately.
 */
#define FLEXSPI_MCR2_CLRLEARNPHASE(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_CLRLEARNPHASE_SHIFT)) & FLEXSPI_MCR2_CLRLEARNPHASE_MASK)

#define FLEXSPI_MCR2_SAMEDEVICEEN_MASK           (0x8000U)
#define FLEXSPI_MCR2_SAMEDEVICEEN_SHIFT          (15U)
/*! SAMEDEVICEEN - All external devices are same devices (both in types and size) for A1/A2/B1/B2.
 *  0b0..In Individual mode, FLSHA1CRx/FLSHA2CRx/FLSHB1CRx/FLSHB2CRx register setting will be applied to Flash A1/A2/B1/B2 separately. Disabled.
 *  0b1..FLSHA1CR0/FLSHA1CR1/FLSHA1CR2 register settings will be applied to Flash A1/A2/B1/B2. FLSHA2CRx/FLSHB1CRx/FLSHB2CRx will be ignored.
 */
#define FLEXSPI_MCR2_SAMEDEVICEEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_SAMEDEVICEEN_SHIFT)) & FLEXSPI_MCR2_SAMEDEVICEEN_MASK)

#define FLEXSPI_MCR2_SCKBDIFFOPT_MASK            (0x80000U)
#define FLEXSPI_MCR2_SCKBDIFFOPT_SHIFT           (19U)
/*! SCKBDIFFOPT - B_SCLK pad can be used as A_SCLK differential clock output (inverted clock to
 *    A_SCLK). In this case, port B flash access is not available. After changing the value of this
 *    field, MCR0[SWRESET] should be set.
 *  0b0..B_SCLK pad is used as port B SCLK clock output. Port B flash access is available.
 *  0b1..B_SCLK pad is used as port A SCLK inverted clock output (Differential clock to A_SCLK). Port B flash access is not available.
 */
#define FLEXSPI_MCR2_SCKBDIFFOPT(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_SCKBDIFFOPT_SHIFT)) & FLEXSPI_MCR2_SCKBDIFFOPT_MASK)

#define FLEXSPI_MCR2_RXCLKSRC_B_MASK             (0x600000U)
#define FLEXSPI_MCR2_RXCLKSRC_B_SHIFT            (21U)
/*! RXCLKSRC_B - Sample Clock source selection for Flash Reading
 *  0b00..Dummy Read strobe generated by FlexSPI Controller and loopback internally.
 *  0b01..Dummy Read strobe generated by FlexSPI Controller and loopback from DQS pad.
 *  0b10..SCLK output clock and loopback from SCLK padReserved
 *  0b11..Flash provided Read strobe and input from DQS pad
 */
#define FLEXSPI_MCR2_RXCLKSRC_B(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_RXCLKSRC_B_SHIFT)) & FLEXSPI_MCR2_RXCLKSRC_B_MASK)

#define FLEXSPI_MCR2_RX_CLK_SRC_DIFF_MASK        (0x800000U)
#define FLEXSPI_MCR2_RX_CLK_SRC_DIFF_SHIFT       (23U)
/*! RX_CLK_SRC_DIFF - Sample Clock source or source_b selection for Flash Reading */
#define FLEXSPI_MCR2_RX_CLK_SRC_DIFF(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_RX_CLK_SRC_DIFF_SHIFT)) & FLEXSPI_MCR2_RX_CLK_SRC_DIFF_MASK)

#define FLEXSPI_MCR2_RESUMEWAIT_MASK             (0xFF000000U)
#define FLEXSPI_MCR2_RESUMEWAIT_SHIFT            (24U)
/*! RESUMEWAIT - Wait cycle (in AHB clock cycle) for idle state before suspended command sequence resumed. */
#define FLEXSPI_MCR2_RESUMEWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_RESUMEWAIT_SHIFT)) & FLEXSPI_MCR2_RESUMEWAIT_MASK)
/*! @} */

/*! @name AHBCR - AHB Bus Control Register */
/*! @{ */

#define FLEXSPI_AHBCR_CLRAHBRXBUF_MASK           (0x2U)
#define FLEXSPI_AHBCR_CLRAHBRXBUF_SHIFT          (1U)
/*! CLRAHBRXBUF - Clear the status/pointers of AHB RX Buffer. Auto-cleared. */
#define FLEXSPI_AHBCR_CLRAHBRXBUF(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_CLRAHBRXBUF_SHIFT)) & FLEXSPI_AHBCR_CLRAHBRXBUF_MASK)

#define FLEXSPI_AHBCR_CLRAHBTXBUF_MASK           (0x4U)
#define FLEXSPI_AHBCR_CLRAHBTXBUF_SHIFT          (2U)
/*! CLRAHBTXBUF - Clear the status/pointers of AHB TX Buffer. Auto-cleared. */
#define FLEXSPI_AHBCR_CLRAHBTXBUF(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_CLRAHBTXBUF_SHIFT)) & FLEXSPI_AHBCR_CLRAHBTXBUF_MASK)

#define FLEXSPI_AHBCR_CACHABLEEN_MASK            (0x8U)
#define FLEXSPI_AHBCR_CACHABLEEN_SHIFT           (3U)
/*! CACHABLEEN - Enable AHB bus cachable read access support.
 *  0b0..Disabled. When there is AHB bus cachable read access, FlexSPI will not check whether it hit AHB TX Buffer.
 *  0b1..Enabled. When there is AHB bus cachable read access, FlexSPI will check whether it hit AHB TX Buffer first.
 */
#define FLEXSPI_AHBCR_CACHABLEEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_CACHABLEEN_SHIFT)) & FLEXSPI_AHBCR_CACHABLEEN_MASK)

#define FLEXSPI_AHBCR_BUFFERABLEEN_MASK          (0x10U)
#define FLEXSPI_AHBCR_BUFFERABLEEN_SHIFT         (4U)
/*! BUFFERABLEEN - Enable AHB bus bufferable write access support.
 *  0b0..Disabled. For all AHB write access (no matter bufferable or non-bufferable ), FlexSPI will return AHB Bus
 *       ready after all data is transmitted to External device and AHB command finished.
 *  0b1..Enabled. For AHB bufferable write access, FlexSPI will return AHB Bus ready when the AHB command is
 *       granted by arbitrator and will not wait for AHB command finished.
 */
#define FLEXSPI_AHBCR_BUFFERABLEEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_BUFFERABLEEN_SHIFT)) & FLEXSPI_AHBCR_BUFFERABLEEN_MASK)

#define FLEXSPI_AHBCR_PREFETCHEN_MASK            (0x20U)
#define FLEXSPI_AHBCR_PREFETCHEN_SHIFT           (5U)
/*! PREFETCHEN - AHB Read Prefetch Enable. */
#define FLEXSPI_AHBCR_PREFETCHEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_PREFETCHEN_SHIFT)) & FLEXSPI_AHBCR_PREFETCHEN_MASK)

#define FLEXSPI_AHBCR_READADDROPT_MASK           (0x40U)
#define FLEXSPI_AHBCR_READADDROPT_SHIFT          (6U)
/*! READADDROPT - AHB Read Address option bit. This option bit is intend to remove AHB burst start address alignment limitation.
 *  0b0..There is AHB read burst start address alignment limitation when flash is accessed in flash is word-addressable.
 *  0b1..There is no AHB read burst start address alignment limitation. FlexSPI will fetch more data than AHB
 *       burst required to meet the alignment requirement.
 */
#define FLEXSPI_AHBCR_READADDROPT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_READADDROPT_SHIFT)) & FLEXSPI_AHBCR_READADDROPT_MASK)

#define FLEXSPI_AHBCR_READSZALIGN_MASK           (0x400U)
#define FLEXSPI_AHBCR_READSZALIGN_SHIFT          (10U)
/*! READSZALIGN - AHB Read Size Alignment
 *  0b0..AHB read size will be decided by other register setting like PREFETCH_EN
 *  0b1..AHB read size to up size to 8 bytes aligned, no prefetching
 */
#define FLEXSPI_AHBCR_READSZALIGN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_READSZALIGN_SHIFT)) & FLEXSPI_AHBCR_READSZALIGN_MASK)

#define FLEXSPI_AHBCR_ALIGNMENT_MASK             (0x300000U)
#define FLEXSPI_AHBCR_ALIGNMENT_SHIFT            (20U)
/*! ALIGNMENT - Decides all AHB read/write boundary. All access cross the boundary will be divided into smaller sub accesses.
 *  0b00..No limit
 *  0b01..1 KBytes
 *  0b10..512 Bytes
 *  0b11..256 Bytes
 */
#define FLEXSPI_AHBCR_ALIGNMENT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_ALIGNMENT_SHIFT)) & FLEXSPI_AHBCR_ALIGNMENT_MASK)
/*! @} */

/*! @name INTEN - Interrupt Enable Register */
/*! @{ */

#define FLEXSPI_INTEN_IPCMDDONEEN_MASK           (0x1U)
#define FLEXSPI_INTEN_IPCMDDONEEN_SHIFT          (0U)
/*! IPCMDDONEEN - IP triggered Command Sequences Execution finished interrupt enable. */
#define FLEXSPI_INTEN_IPCMDDONEEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDDONEEN_SHIFT)) & FLEXSPI_INTEN_IPCMDDONEEN_MASK)

#define FLEXSPI_INTEN_IPCMDGEEN_MASK             (0x2U)
#define FLEXSPI_INTEN_IPCMDGEEN_SHIFT            (1U)
/*! IPCMDGEEN - IP triggered Command Sequences Grant Timeout interrupt enable. */
#define FLEXSPI_INTEN_IPCMDGEEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDGEEN_SHIFT)) & FLEXSPI_INTEN_IPCMDGEEN_MASK)

#define FLEXSPI_INTEN_AHBCMDGEEN_MASK            (0x4U)
#define FLEXSPI_INTEN_AHBCMDGEEN_SHIFT           (2U)
/*! AHBCMDGEEN - AHB triggered Command Sequences Grant Timeout interrupt enable. */
#define FLEXSPI_INTEN_AHBCMDGEEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBCMDGEEN_SHIFT)) & FLEXSPI_INTEN_AHBCMDGEEN_MASK)

#define FLEXSPI_INTEN_IPCMDERREN_MASK            (0x8U)
#define FLEXSPI_INTEN_IPCMDERREN_SHIFT           (3U)
/*! IPCMDERREN - IP triggered Command Sequences Error Detected interrupt enable. */
#define FLEXSPI_INTEN_IPCMDERREN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDERREN_SHIFT)) & FLEXSPI_INTEN_IPCMDERREN_MASK)

#define FLEXSPI_INTEN_AHBCMDERREN_MASK           (0x10U)
#define FLEXSPI_INTEN_AHBCMDERREN_SHIFT          (4U)
/*! AHBCMDERREN - AHB triggered Command Sequences Error Detected interrupt enable. */
#define FLEXSPI_INTEN_AHBCMDERREN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBCMDERREN_SHIFT)) & FLEXSPI_INTEN_AHBCMDERREN_MASK)

#define FLEXSPI_INTEN_IPRXWAEN_MASK              (0x20U)
#define FLEXSPI_INTEN_IPRXWAEN_SHIFT             (5U)
/*! IPRXWAEN - IP RX FIFO WaterMark available interrupt enable. */
#define FLEXSPI_INTEN_IPRXWAEN(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPRXWAEN_SHIFT)) & FLEXSPI_INTEN_IPRXWAEN_MASK)

#define FLEXSPI_INTEN_IPTXWEEN_MASK              (0x40U)
#define FLEXSPI_INTEN_IPTXWEEN_SHIFT             (6U)
/*! IPTXWEEN - IP TX FIFO WaterMark empty interrupt enable. */
#define FLEXSPI_INTEN_IPTXWEEN(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPTXWEEN_SHIFT)) & FLEXSPI_INTEN_IPTXWEEN_MASK)

#define FLEXSPI_INTEN_DATALEARNFAILEN_MASK       (0x80U)
#define FLEXSPI_INTEN_DATALEARNFAILEN_SHIFT      (7U)
/*! DATALEARNFAILEN - Data Learning failed interrupt enable. */
#define FLEXSPI_INTEN_DATALEARNFAILEN(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_DATALEARNFAILEN_SHIFT)) & FLEXSPI_INTEN_DATALEARNFAILEN_MASK)

#define FLEXSPI_INTEN_SCKSTOPBYRDEN_MASK         (0x100U)
#define FLEXSPI_INTEN_SCKSTOPBYRDEN_SHIFT        (8U)
/*! SCKSTOPBYRDEN - SCLK is stopped during command sequence because Async RX FIFO full interrupt enable. */
#define FLEXSPI_INTEN_SCKSTOPBYRDEN(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SCKSTOPBYRDEN_SHIFT)) & FLEXSPI_INTEN_SCKSTOPBYRDEN_MASK)

#define FLEXSPI_INTEN_SCKSTOPBYWREN_MASK         (0x200U)
#define FLEXSPI_INTEN_SCKSTOPBYWREN_SHIFT        (9U)
/*! SCKSTOPBYWREN - SCLK is stopped during command sequence because Async TX FIFO empty interrupt enable. */
#define FLEXSPI_INTEN_SCKSTOPBYWREN(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SCKSTOPBYWREN_SHIFT)) & FLEXSPI_INTEN_SCKSTOPBYWREN_MASK)

#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN_MASK       (0x400U)
#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN_SHIFT      (10U)
/*! AHBBUSTIMEOUTEN - AHB Bus timeout interrupt. */
#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBBUSTIMEOUTEN_SHIFT)) & FLEXSPI_INTEN_AHBBUSTIMEOUTEN_MASK)

#define FLEXSPI_INTEN_SEQTIMEOUTEN_MASK          (0x800U)
#define FLEXSPI_INTEN_SEQTIMEOUTEN_SHIFT         (11U)
/*! SEQTIMEOUTEN - Sequence execution timeout interrupt enable. */
#define FLEXSPI_INTEN_SEQTIMEOUTEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SEQTIMEOUTEN_SHIFT)) & FLEXSPI_INTEN_SEQTIMEOUTEN_MASK)

#define FLEXSPI_INTEN_IPCMDSECUREVIOEN_MASK      (0x10000U)
#define FLEXSPI_INTEN_IPCMDSECUREVIOEN_SHIFT     (16U)
/*! IPCMDSECUREVIOEN - IP command security violation interrupt enable. */
#define FLEXSPI_INTEN_IPCMDSECUREVIOEN(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDSECUREVIOEN_SHIFT)) & FLEXSPI_INTEN_IPCMDSECUREVIOEN_MASK)

#define FLEXSPI_INTEN_AHBGCMERREN_MASK           (0x20000U)
#define FLEXSPI_INTEN_AHBGCMERREN_SHIFT          (17U)
/*! AHBGCMERREN - AHB read gcm error interrupt enable. */
#define FLEXSPI_INTEN_AHBGCMERREN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBGCMERREN_SHIFT)) & FLEXSPI_INTEN_AHBGCMERREN_MASK)
/*! @} */

/*! @name INTR - Interrupt Register */
/*! @{ */

#define FLEXSPI_INTR_IPCMDDONE_MASK              (0x1U)
#define FLEXSPI_INTR_IPCMDDONE_SHIFT             (0U)
/*! IPCMDDONE - IP triggered Command Sequences Execution finished interrupt. This interrupt is also
 *    generated when there is IPCMDGE or IPCMDERR interrupt generated.
 */
#define FLEXSPI_INTR_IPCMDDONE(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDDONE_SHIFT)) & FLEXSPI_INTR_IPCMDDONE_MASK)

#define FLEXSPI_INTR_IPCMDGE_MASK                (0x2U)
#define FLEXSPI_INTR_IPCMDGE_SHIFT               (1U)
/*! IPCMDGE - IP triggered Command Sequences Grant Timeout interrupt. */
#define FLEXSPI_INTR_IPCMDGE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDGE_SHIFT)) & FLEXSPI_INTR_IPCMDGE_MASK)

#define FLEXSPI_INTR_AHBCMDGE_MASK               (0x4U)
#define FLEXSPI_INTR_AHBCMDGE_SHIFT              (2U)
/*! AHBCMDGE - AHB triggered Command Sequences Grant Timeout interrupt. */
#define FLEXSPI_INTR_AHBCMDGE(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBCMDGE_SHIFT)) & FLEXSPI_INTR_AHBCMDGE_MASK)

#define FLEXSPI_INTR_IPCMDERR_MASK               (0x8U)
#define FLEXSPI_INTR_IPCMDERR_SHIFT              (3U)
/*! IPCMDERR - IP triggered Command Sequences Error Detected interrupt. When an error detected for
 *    IP command, this command will be ignored and not executed at all.
 */
#define FLEXSPI_INTR_IPCMDERR(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDERR_SHIFT)) & FLEXSPI_INTR_IPCMDERR_MASK)

#define FLEXSPI_INTR_AHBCMDERR_MASK              (0x10U)
#define FLEXSPI_INTR_AHBCMDERR_SHIFT             (4U)
/*! AHBCMDERR - AHB triggered Command Sequences Error Detected interrupt. When an error detected for
 *    AHB command, this command will be ignored and not executed at all.
 */
#define FLEXSPI_INTR_AHBCMDERR(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBCMDERR_SHIFT)) & FLEXSPI_INTR_AHBCMDERR_MASK)

#define FLEXSPI_INTR_IPRXWA_MASK                 (0x20U)
#define FLEXSPI_INTR_IPRXWA_SHIFT                (5U)
/*! IPRXWA - IP RX FIFO watermark available interrupt. */
#define FLEXSPI_INTR_IPRXWA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPRXWA_SHIFT)) & FLEXSPI_INTR_IPRXWA_MASK)

#define FLEXSPI_INTR_IPTXWE_MASK                 (0x40U)
#define FLEXSPI_INTR_IPTXWE_SHIFT                (6U)
/*! IPTXWE - IP TX FIFO watermark empty interrupt. */
#define FLEXSPI_INTR_IPTXWE(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPTXWE_SHIFT)) & FLEXSPI_INTR_IPTXWE_MASK)

#define FLEXSPI_INTR_DATALEARNFAIL_MASK          (0x80U)
#define FLEXSPI_INTR_DATALEARNFAIL_SHIFT         (7U)
/*! DATALEARNFAIL - Data Learning failed interrupt. */
#define FLEXSPI_INTR_DATALEARNFAIL(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_DATALEARNFAIL_SHIFT)) & FLEXSPI_INTR_DATALEARNFAIL_MASK)

#define FLEXSPI_INTR_SCKSTOPBYRD_MASK            (0x100U)
#define FLEXSPI_INTR_SCKSTOPBYRD_SHIFT           (8U)
/*! SCKSTOPBYRD - SCLK is stopped during command sequence because Async RX FIFO full interrupt. */
#define FLEXSPI_INTR_SCKSTOPBYRD(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SCKSTOPBYRD_SHIFT)) & FLEXSPI_INTR_SCKSTOPBYRD_MASK)

#define FLEXSPI_INTR_SCKSTOPBYWR_MASK            (0x200U)
#define FLEXSPI_INTR_SCKSTOPBYWR_SHIFT           (9U)
/*! SCKSTOPBYWR - SCLK is stopped during command sequence because Async TX FIFO empty interrupt. */
#define FLEXSPI_INTR_SCKSTOPBYWR(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SCKSTOPBYWR_SHIFT)) & FLEXSPI_INTR_SCKSTOPBYWR_MASK)

#define FLEXSPI_INTR_AHBBUSTIMEOUT_MASK          (0x400U)
#define FLEXSPI_INTR_AHBBUSTIMEOUT_SHIFT         (10U)
/*! AHBBUSTIMEOUT - AHB Bus timeout interrupt. */
#define FLEXSPI_INTR_AHBBUSTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBBUSTIMEOUT_SHIFT)) & FLEXSPI_INTR_AHBBUSTIMEOUT_MASK)

#define FLEXSPI_INTR_SEQTIMEOUT_MASK             (0x800U)
#define FLEXSPI_INTR_SEQTIMEOUT_SHIFT            (11U)
/*! SEQTIMEOUT - Sequence execution timeout interrupt. */
#define FLEXSPI_INTR_SEQTIMEOUT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SEQTIMEOUT_SHIFT)) & FLEXSPI_INTR_SEQTIMEOUT_MASK)

#define FLEXSPI_INTR_IPCMDSECUREVIO_MASK         (0x10000U)
#define FLEXSPI_INTR_IPCMDSECUREVIO_SHIFT        (16U)
/*! IPCMDSECUREVIO - IP command security violation interrupt. */
#define FLEXSPI_INTR_IPCMDSECUREVIO(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDSECUREVIO_SHIFT)) & FLEXSPI_INTR_IPCMDSECUREVIO_MASK)

#define FLEXSPI_INTR_AHBGCMERR_MASK              (0x20000U)
#define FLEXSPI_INTR_AHBGCMERR_SHIFT             (17U)
/*! AHBGCMERR - AHB read gcm error interrupt. */
#define FLEXSPI_INTR_AHBGCMERR(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBGCMERR_SHIFT)) & FLEXSPI_INTR_AHBGCMERR_MASK)
/*! @} */

/*! @name LUTKEY - LUT Key Register */
/*! @{ */

#define FLEXSPI_LUTKEY_KEY_MASK                  (0xFFFFFFFFU)
#define FLEXSPI_LUTKEY_KEY_SHIFT                 (0U)
/*! KEY - The Key to lock or unlock LUT. */
#define FLEXSPI_LUTKEY_KEY(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTKEY_KEY_SHIFT)) & FLEXSPI_LUTKEY_KEY_MASK)
/*! @} */

/*! @name LUTCR - LUT Control Register */
/*! @{ */

#define FLEXSPI_LUTCR_LOCK_MASK                  (0x1U)
#define FLEXSPI_LUTCR_LOCK_SHIFT                 (0U)
/*! LOCK - Lock LUT */
#define FLEXSPI_LUTCR_LOCK(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTCR_LOCK_SHIFT)) & FLEXSPI_LUTCR_LOCK_MASK)

#define FLEXSPI_LUTCR_UNLOCK_MASK                (0x2U)
#define FLEXSPI_LUTCR_UNLOCK_SHIFT               (1U)
/*! UNLOCK - Unlock LUT */
#define FLEXSPI_LUTCR_UNLOCK(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTCR_UNLOCK_SHIFT)) & FLEXSPI_LUTCR_UNLOCK_MASK)

#define FLEXSPI_LUTCR_PROTECT_MASK               (0x4U)
#define FLEXSPI_LUTCR_PROTECT_SHIFT              (2U)
/*! PROTECT - LUT protection */
#define FLEXSPI_LUTCR_PROTECT(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTCR_PROTECT_SHIFT)) & FLEXSPI_LUTCR_PROTECT_MASK)
/*! @} */

/*! @name AHBRXBUFCR0 - AHB RX Buffer 0 Control Register 0..AHB RX Buffer 7 Control Register 0 */
/*! @{ */

#define FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK           (0xFFU)
#define FLEXSPI_AHBRXBUFCR0_BUFSZ_SHIFT          (0U)
/*! BUFSZ - AHB RX Buffer Size in 64 bits. */
#define FLEXSPI_AHBRXBUFCR0_BUFSZ(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_BUFSZ_SHIFT)) & FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK)

#define FLEXSPI_AHBRXBUFCR0_MSTRID_MASK          (0xF0000U)
#define FLEXSPI_AHBRXBUFCR0_MSTRID_SHIFT         (16U)
/*! MSTRID - This AHB RX Buffer is assigned according to AHB Master with ID (MSTR_ID). */
#define FLEXSPI_AHBRXBUFCR0_MSTRID(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_MSTRID_SHIFT)) & FLEXSPI_AHBRXBUFCR0_MSTRID_MASK)

#define FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK        (0x7000000U)
#define FLEXSPI_AHBRXBUFCR0_PRIORITY_SHIFT       (24U)
/*! PRIORITY - This priority for AHB Master Read which this AHB RX Buffer is assigned. 7 is the highest priority, 0 the lowest. */
#define FLEXSPI_AHBRXBUFCR0_PRIORITY(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_PRIORITY_SHIFT)) & FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK)

#define FLEXSPI_AHBRXBUFCR0_REGIONEN_MASK        (0x40000000U)
#define FLEXSPI_AHBRXBUFCR0_REGIONEN_SHIFT       (30U)
/*! REGIONEN - AHB RX Buffer address region funciton enable */
#define FLEXSPI_AHBRXBUFCR0_REGIONEN(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_REGIONEN_SHIFT)) & FLEXSPI_AHBRXBUFCR0_REGIONEN_MASK)

#define FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK      (0x80000000U)
#define FLEXSPI_AHBRXBUFCR0_PREFETCHEN_SHIFT     (31U)
/*! PREFETCHEN - AHB Read Prefetch Enable for current AHB RX Buffer corresponding Master. */
#define FLEXSPI_AHBRXBUFCR0_PREFETCHEN(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_PREFETCHEN_SHIFT)) & FLEXSPI_AHBRXBUFCR0_PREFETCHEN_MASK)
/*! @} */

/* The count of FLEXSPI_AHBRXBUFCR0 */
#define FLEXSPI_AHBRXBUFCR0_COUNT                (8U)

/*! @name FLSHCR0 - Flash Control Register 0 */
/*! @{ */

#define FLEXSPI_FLSHCR0_FLSHSZ_MASK              (0x7FFFFFU)
#define FLEXSPI_FLSHCR0_FLSHSZ_SHIFT             (0U)
/*! FLSHSZ - Flash Size in KByte. */
#define FLEXSPI_FLSHCR0_FLSHSZ(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR0_FLSHSZ_SHIFT)) & FLEXSPI_FLSHCR0_FLSHSZ_MASK)

#define FLEXSPI_FLSHCR0_ADDRSHIFT_MASK           (0x20000000U)
#define FLEXSPI_FLSHCR0_ADDRSHIFT_SHIFT          (29U)
/*! ADDRSHIFT - AHB address shift function control.
 *  0b0..Disabled.
 *  0b1..Enabled.
 */
#define FLEXSPI_FLSHCR0_ADDRSHIFT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR0_ADDRSHIFT_SHIFT)) & FLEXSPI_FLSHCR0_ADDRSHIFT_MASK)

#define FLEXSPI_FLSHCR0_SPLITWREN_MASK           (0x40000000U)
#define FLEXSPI_FLSHCR0_SPLITWREN_SHIFT          (30U)
/*! SPLITWREN - AHB write access split function control. */
#define FLEXSPI_FLSHCR0_SPLITWREN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR0_SPLITWREN_SHIFT)) & FLEXSPI_FLSHCR0_SPLITWREN_MASK)

#define FLEXSPI_FLSHCR0_SPLITRDEN_MASK           (0x80000000U)
#define FLEXSPI_FLSHCR0_SPLITRDEN_SHIFT          (31U)
/*! SPLITRDEN - AHB read access split function control. */
#define FLEXSPI_FLSHCR0_SPLITRDEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR0_SPLITRDEN_SHIFT)) & FLEXSPI_FLSHCR0_SPLITRDEN_MASK)
/*! @} */

/* The count of FLEXSPI_FLSHCR0 */
#define FLEXSPI_FLSHCR0_COUNT                    (4U)

/*! @name FLSHCR1 - Flash Control Register 1 */
/*! @{ */

#define FLEXSPI_FLSHCR1_TCSS_MASK                (0x1FU)
#define FLEXSPI_FLSHCR1_TCSS_SHIFT               (0U)
/*! TCSS - Serial Flash CS setup time. */
#define FLEXSPI_FLSHCR1_TCSS(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_TCSS_SHIFT)) & FLEXSPI_FLSHCR1_TCSS_MASK)

#define FLEXSPI_FLSHCR1_TCSH_MASK                (0x3E0U)
#define FLEXSPI_FLSHCR1_TCSH_SHIFT               (5U)
/*! TCSH - Serial Flash CS Hold time. */
#define FLEXSPI_FLSHCR1_TCSH(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_TCSH_SHIFT)) & FLEXSPI_FLSHCR1_TCSH_MASK)

#define FLEXSPI_FLSHCR1_WA_MASK                  (0x400U)
#define FLEXSPI_FLSHCR1_WA_SHIFT                 (10U)
/*! WA - Word Addressable. */
#define FLEXSPI_FLSHCR1_WA(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_WA_SHIFT)) & FLEXSPI_FLSHCR1_WA_MASK)

#define FLEXSPI_FLSHCR1_CAS_MASK                 (0x7800U)
#define FLEXSPI_FLSHCR1_CAS_SHIFT                (11U)
/*! CAS - Column Address Size. */
#define FLEXSPI_FLSHCR1_CAS(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CAS_SHIFT)) & FLEXSPI_FLSHCR1_CAS_MASK)

#define FLEXSPI_FLSHCR1_CSINTERVALUNIT_MASK      (0x8000U)
#define FLEXSPI_FLSHCR1_CSINTERVALUNIT_SHIFT     (15U)
/*! CSINTERVALUNIT - CS interval unit
 *  0b0..The CS interval unit is 1 serial clock cycle
 *  0b1..The CS interval unit is 256 serial clock cycle
 */
#define FLEXSPI_FLSHCR1_CSINTERVALUNIT(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CSINTERVALUNIT_SHIFT)) & FLEXSPI_FLSHCR1_CSINTERVALUNIT_MASK)

#define FLEXSPI_FLSHCR1_CSINTERVAL_MASK          (0xFFFF0000U)
#define FLEXSPI_FLSHCR1_CSINTERVAL_SHIFT         (16U)
/*! CSINTERVAL - This field is used to set the minimum interval between flash device Chip selection
 *    deassertion and flash device Chip selection assertion. If external flash has a limitation on
 *    the interval between command sequences, this field should be set accordingly. If there is no
 *    limitation, set this field with value 0x0.
 */
#define FLEXSPI_FLSHCR1_CSINTERVAL(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CSINTERVAL_SHIFT)) & FLEXSPI_FLSHCR1_CSINTERVAL_MASK)
/*! @} */

/* The count of FLEXSPI_FLSHCR1 */
#define FLEXSPI_FLSHCR1_COUNT                    (4U)

/*! @name FLSHCR2 - Flash Control Register 2 */
/*! @{ */

#define FLEXSPI_FLSHCR2_ARDSEQID_MASK            (0xFU)
#define FLEXSPI_FLSHCR2_ARDSEQID_SHIFT           (0U)
/*! ARDSEQID - Sequence Index for AHB Read triggered Command in LUT. */
#define FLEXSPI_FLSHCR2_ARDSEQID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_ARDSEQID_SHIFT)) & FLEXSPI_FLSHCR2_ARDSEQID_MASK)

#define FLEXSPI_FLSHCR2_ARDSEQNUM_MASK           (0xE0U)
#define FLEXSPI_FLSHCR2_ARDSEQNUM_SHIFT          (5U)
/*! ARDSEQNUM - Sequence Number for AHB Read triggered Command in LUT. */
#define FLEXSPI_FLSHCR2_ARDSEQNUM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_ARDSEQNUM_SHIFT)) & FLEXSPI_FLSHCR2_ARDSEQNUM_MASK)

#define FLEXSPI_FLSHCR2_AWRSEQID_MASK            (0xF00U)
#define FLEXSPI_FLSHCR2_AWRSEQID_SHIFT           (8U)
/*! AWRSEQID - Sequence Index for AHB Write triggered Command. */
#define FLEXSPI_FLSHCR2_AWRSEQID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRSEQID_SHIFT)) & FLEXSPI_FLSHCR2_AWRSEQID_MASK)

#define FLEXSPI_FLSHCR2_AWRSEQNUM_MASK           (0xE000U)
#define FLEXSPI_FLSHCR2_AWRSEQNUM_SHIFT          (13U)
/*! AWRSEQNUM - Sequence Number for AHB Write triggered Command. */
#define FLEXSPI_FLSHCR2_AWRSEQNUM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRSEQNUM_SHIFT)) & FLEXSPI_FLSHCR2_AWRSEQNUM_MASK)

#define FLEXSPI_FLSHCR2_AWRWAIT_MASK             (0xFFF0000U)
#define FLEXSPI_FLSHCR2_AWRWAIT_SHIFT            (16U)
/*! AWRWAIT - For certain devices (such as FPGA), it need some time to write data into internal
 *    memory after the command sequences finished on FlexSPI interface. If another Read command sequence
 *    comes before previous programming finished internally, the read data may be wrong. This field
 *    is used to hold AHB Bus ready for AHB write access to wait the programming finished in
 *    external device. Then there will be no AHB read command triggered before the programming finished in
 *    external device. The Wait cycle between AHB triggered command sequences finished on FlexSPI
 *    interface and AHB return Bus ready: AWRWAIT * AWRWAITUNIT
 */
#define FLEXSPI_FLSHCR2_AWRWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRWAIT_SHIFT)) & FLEXSPI_FLSHCR2_AWRWAIT_MASK)

#define FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK         (0x70000000U)
#define FLEXSPI_FLSHCR2_AWRWAITUNIT_SHIFT        (28U)
/*! AWRWAITUNIT - AWRWAIT unit
 *  0b000..The AWRWAIT unit is 2 ahb clock cycle
 *  0b001..The AWRWAIT unit is 8 ahb clock cycle
 *  0b010..The AWRWAIT unit is 32 ahb clock cycle
 *  0b011..The AWRWAIT unit is 128 ahb clock cycle
 *  0b100..The AWRWAIT unit is 512 ahb clock cycle
 *  0b101..The AWRWAIT unit is 2048 ahb clock cycle
 *  0b110..The AWRWAIT unit is 8192 ahb clock cycle
 *  0b111..The AWRWAIT unit is 32768 ahb clock cycle
 */
#define FLEXSPI_FLSHCR2_AWRWAITUNIT(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRWAITUNIT_SHIFT)) & FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK)

#define FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK         (0x80000000U)
#define FLEXSPI_FLSHCR2_CLRINSTRPTR_SHIFT        (31U)
/*! CLRINSTRPTR - Clear the instruction pointer which is internally saved pointer by JMP_ON_CS. */
#define FLEXSPI_FLSHCR2_CLRINSTRPTR(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_CLRINSTRPTR_SHIFT)) & FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK)
/*! @} */

/* The count of FLEXSPI_FLSHCR2 */
#define FLEXSPI_FLSHCR2_COUNT                    (4U)

/*! @name FLSHCR4 - Flash Control Register 4 */
/*! @{ */

#define FLEXSPI_FLSHCR4_WMOPT1_MASK              (0x1U)
#define FLEXSPI_FLSHCR4_WMOPT1_SHIFT             (0U)
/*! WMOPT1 - Write mask option bit 1. This option bit could be used to remove AHB write burst start address alignment limitation.
 *  0b0..DQS pin will be used as Write Mask when writing to external device. There is no limitation on AHB write
 *       burst start address alignment when flash is accessed in individual mode.
 *  0b1..DQS pin will not be used as Write Mask when writing to external device. There is limitation on AHB write
 *       burst start address alignment when flash is accessed in individual mode.
 */
#define FLEXSPI_FLSHCR4_WMOPT1(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_WMOPT1_SHIFT)) & FLEXSPI_FLSHCR4_WMOPT1_MASK)

#define FLEXSPI_FLSHCR4_WMOPT2_MASK              (0x2U)
#define FLEXSPI_FLSHCR4_WMOPT2_SHIFT             (1U)
/*! WMOPT2 - Write mask option bit 2. When using AP memory, This option bit could be used to remove
 *    AHB write burst minimal length limitation. When using this bit, WMOPT1 should also be set.
 *  0b0..DQS pin will be used as Write Mask when writing to external device. There is no limitation on AHB write
 *       burst length when flash is accessed in individual mode.
 *  0b1..DQS pin will not be used as Write Mask when writing to external device. There is limitation on AHB write
 *       burst length when flash is accessed in individual mode, the minimal write burst length should be 4.
 */
#define FLEXSPI_FLSHCR4_WMOPT2(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_WMOPT2_SHIFT)) & FLEXSPI_FLSHCR4_WMOPT2_MASK)

#define FLEXSPI_FLSHCR4_WMENA_MASK               (0x4U)
#define FLEXSPI_FLSHCR4_WMENA_SHIFT              (2U)
/*! WMENA - Write mask enable bit for flash device on port A. When write mask function is needed for
 *    memory device on port A, this bit must be set.
 *  0b0..Write mask is disabled, DQS(RWDS) pin will not be driven when writing to external device.
 *  0b1..Write mask is enabled, DQS(RWDS) pin will be driven by FlexSPI as write mask output when writing to external device.
 */
#define FLEXSPI_FLSHCR4_WMENA(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_WMENA_SHIFT)) & FLEXSPI_FLSHCR4_WMENA_MASK)

#define FLEXSPI_FLSHCR4_WMENB_MASK               (0x8U)
#define FLEXSPI_FLSHCR4_WMENB_SHIFT              (3U)
/*! WMENB - Write mask enable bit for flash device on port B. When write mask function is needed for
 *    memory device on port B, this bit must be set.
 *  0b0..Write mask is disabled, DQS(RWDS) pin will not be driven when writing to external device.
 *  0b1..Write mask is enabled, DQS(RWDS) pin will be driven by FlexSPI as write mask output when writing to external device.
 */
#define FLEXSPI_FLSHCR4_WMENB(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_WMENB_SHIFT)) & FLEXSPI_FLSHCR4_WMENB_MASK)

#define FLEXSPI_FLSHCR4_PAR_WM_MASK              (0x600U)
#define FLEXSPI_FLSHCR4_PAR_WM_SHIFT             (9U)
/*! PAR_WM - Enable APMEM 16 bit write mask function, bit 9 for A1-B1 pair, bit 10 for A2-B2 pair. */
#define FLEXSPI_FLSHCR4_PAR_WM(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_PAR_WM_SHIFT)) & FLEXSPI_FLSHCR4_PAR_WM_MASK)

#define FLEXSPI_FLSHCR4_PAR_ADDR_ADJ_DIS_MASK    (0x800U)
#define FLEXSPI_FLSHCR4_PAR_ADDR_ADJ_DIS_SHIFT   (11U)
/*! PAR_ADDR_ADJ_DIS - Disable the address shift logic for lower density of 16 bit PSRAM. */
#define FLEXSPI_FLSHCR4_PAR_ADDR_ADJ_DIS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR4_PAR_ADDR_ADJ_DIS_SHIFT)) & FLEXSPI_FLSHCR4_PAR_ADDR_ADJ_DIS_MASK)
/*! @} */

/*! @name IPCR0 - IP Control Register 0 */
/*! @{ */

#define FLEXSPI_IPCR0_SFAR_MASK                  (0xFFFFFFFFU)
#define FLEXSPI_IPCR0_SFAR_SHIFT                 (0U)
/*! SFAR - Serial Flash Address for IP command. */
#define FLEXSPI_IPCR0_SFAR(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR0_SFAR_SHIFT)) & FLEXSPI_IPCR0_SFAR_MASK)
/*! @} */

/*! @name IPCR1 - IP Control Register 1 */
/*! @{ */

#define FLEXSPI_IPCR1_IDATSZ_MASK                (0xFFFFU)
#define FLEXSPI_IPCR1_IDATSZ_SHIFT               (0U)
/*! IDATSZ - Flash Read/Program Data Size (in Bytes) for IP command. */
#define FLEXSPI_IPCR1_IDATSZ(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_IDATSZ_SHIFT)) & FLEXSPI_IPCR1_IDATSZ_MASK)

#define FLEXSPI_IPCR1_ISEQID_MASK                (0xF0000U)
#define FLEXSPI_IPCR1_ISEQID_SHIFT               (16U)
/*! ISEQID - Sequence Index in LUT for IP command. */
#define FLEXSPI_IPCR1_ISEQID(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_ISEQID_SHIFT)) & FLEXSPI_IPCR1_ISEQID_MASK)

#define FLEXSPI_IPCR1_ISEQNUM_MASK               (0x7000000U)
#define FLEXSPI_IPCR1_ISEQNUM_SHIFT              (24U)
/*! ISEQNUM - Sequence Number for IP command: ISEQNUM+1. */
#define FLEXSPI_IPCR1_ISEQNUM(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_ISEQNUM_SHIFT)) & FLEXSPI_IPCR1_ISEQNUM_MASK)
/*! @} */

/*! @name IPCR2 - IP Control Register 2 */
/*! @{ */

#define FLEXSPI_IPCR2_IPBLKAHBREQ_MASK           (0x1U)
#define FLEXSPI_IPCR2_IPBLKAHBREQ_SHIFT          (0U)
/*! IPBLKAHBREQ - IP command block ahb request. */
#define FLEXSPI_IPCR2_IPBLKAHBREQ(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR2_IPBLKAHBREQ_SHIFT)) & FLEXSPI_IPCR2_IPBLKAHBREQ_MASK)

#define FLEXSPI_IPCR2_IPBLKAHBACK_MASK           (0x2U)
#define FLEXSPI_IPCR2_IPBLKAHBACK_SHIFT          (1U)
/*! IPBLKAHBACK - IP command block ahb ack. */
#define FLEXSPI_IPCR2_IPBLKAHBACK(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR2_IPBLKAHBACK_SHIFT)) & FLEXSPI_IPCR2_IPBLKAHBACK_MASK)

#define FLEXSPI_IPCR2_IPBLKALLAHB_MASK           (0x4U)
#define FLEXSPI_IPCR2_IPBLKALLAHB_SHIFT          (2U)
/*! IPBLKALLAHB - If this bit is 1, handshake will block all AHB. Otherwise only ahb hit gcm region will be blocked. */
#define FLEXSPI_IPCR2_IPBLKALLAHB(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR2_IPBLKALLAHB_SHIFT)) & FLEXSPI_IPCR2_IPBLKALLAHB_MASK)
/*! @} */

/*! @name IPCMD - IP Command Register */
/*! @{ */

#define FLEXSPI_IPCMD_TRG_MASK                   (0x1U)
#define FLEXSPI_IPCMD_TRG_SHIFT                  (0U)
/*! TRG - Setting this bit will trigger an IP Command. */
#define FLEXSPI_IPCMD_TRG(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCMD_TRG_SHIFT)) & FLEXSPI_IPCMD_TRG_MASK)
/*! @} */

/*! @name DLPR - Data Learn Pattern Register */
/*! @{ */

#define FLEXSPI_DLPR_DLP_MASK                    (0xFFFFFFFFU)
#define FLEXSPI_DLPR_DLP_SHIFT                   (0U)
/*! DLP - Data Learning Pattern. */
#define FLEXSPI_DLPR_DLP(x)                      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLPR_DLP_SHIFT)) & FLEXSPI_DLPR_DLP_MASK)
/*! @} */

/*! @name IPRXFCR - IP RX FIFO Control Register */
/*! @{ */

#define FLEXSPI_IPRXFCR_CLRIPRXF_MASK            (0x1U)
#define FLEXSPI_IPRXFCR_CLRIPRXF_SHIFT           (0U)
/*! CLRIPRXF - Clear all valid data entries in IP RX FIFO. */
#define FLEXSPI_IPRXFCR_CLRIPRXF(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_CLRIPRXF_SHIFT)) & FLEXSPI_IPRXFCR_CLRIPRXF_MASK)

#define FLEXSPI_IPRXFCR_RXDMAEN_MASK             (0x2U)
#define FLEXSPI_IPRXFCR_RXDMAEN_SHIFT            (1U)
/*! RXDMAEN - IP RX FIFO reading by DMA enabled.
 *  0b0..IP RX FIFO would be read by processor.
 *  0b1..IP RX FIFO would be read by DMA.
 */
#define FLEXSPI_IPRXFCR_RXDMAEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_RXDMAEN_SHIFT)) & FLEXSPI_IPRXFCR_RXDMAEN_MASK)

#define FLEXSPI_IPRXFCR_RXWMRK_MASK              (0x1FCU)
#define FLEXSPI_IPRXFCR_RXWMRK_SHIFT             (2U)
/*! RXWMRK - Watermark level is (RXWMRK+1)*64 Bits. */
#define FLEXSPI_IPRXFCR_RXWMRK(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_RXWMRK_SHIFT)) & FLEXSPI_IPRXFCR_RXWMRK_MASK)
/*! @} */

/*! @name IPTXFCR - IP TX FIFO Control Register */
/*! @{ */

#define FLEXSPI_IPTXFCR_CLRIPTXF_MASK            (0x1U)
#define FLEXSPI_IPTXFCR_CLRIPTXF_SHIFT           (0U)
/*! CLRIPTXF - Clear all valid data entries in IP TX FIFO. */
#define FLEXSPI_IPTXFCR_CLRIPTXF(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_CLRIPTXF_SHIFT)) & FLEXSPI_IPTXFCR_CLRIPTXF_MASK)

#define FLEXSPI_IPTXFCR_TXDMAEN_MASK             (0x2U)
#define FLEXSPI_IPTXFCR_TXDMAEN_SHIFT            (1U)
/*! TXDMAEN - IP TX FIFO filling by DMA enabled.
 *  0b0..IP TX FIFO would be filled by processor.
 *  0b1..IP TX FIFO would be filled by DMA.
 */
#define FLEXSPI_IPTXFCR_TXDMAEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_TXDMAEN_SHIFT)) & FLEXSPI_IPTXFCR_TXDMAEN_MASK)

#define FLEXSPI_IPTXFCR_TXWMRK_MASK              (0x1FCU)
#define FLEXSPI_IPTXFCR_TXWMRK_SHIFT             (2U)
/*! TXWMRK - Watermark level is (TXWMRK+1)*64 Bits. */
#define FLEXSPI_IPTXFCR_TXWMRK(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_TXWMRK_SHIFT)) & FLEXSPI_IPTXFCR_TXWMRK_MASK)
/*! @} */

/*! @name DLLCR - DLL Control Register 0 */
/*! @{ */

#define FLEXSPI_DLLCR_DLLEN_MASK                 (0x1U)
#define FLEXSPI_DLLCR_DLLEN_SHIFT                (0U)
/*! DLLEN - DLL calibration enable. */
#define FLEXSPI_DLLCR_DLLEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_DLLEN_SHIFT)) & FLEXSPI_DLLCR_DLLEN_MASK)

#define FLEXSPI_DLLCR_DLLRESET_MASK              (0x2U)
#define FLEXSPI_DLLCR_DLLRESET_SHIFT             (1U)
/*! DLLRESET - DLL reset */
#define FLEXSPI_DLLCR_DLLRESET(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_DLLRESET_SHIFT)) & FLEXSPI_DLLCR_DLLRESET_MASK)

#define FLEXSPI_DLLCR_SLVDLYTARGET_MASK          (0x78U)
#define FLEXSPI_DLLCR_SLVDLYTARGET_SHIFT         (3U)
/*! SLVDLYTARGET - The delay target for slave delay line is: ((SLVDLYTARGET+1) * 1/32 * clock cycle
 *    of reference clock (serial root clock). If serial root clock is >= 100 MHz, DLLEN set to 0x1,
 *    OVRDEN set to =0x0, then SLVDLYTARGET setting of 0xF is recommended.
 */
#define FLEXSPI_DLLCR_SLVDLYTARGET(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_SLVDLYTARGET_SHIFT)) & FLEXSPI_DLLCR_SLVDLYTARGET_MASK)

#define FLEXSPI_DLLCR_OVRDEN_MASK                (0x100U)
#define FLEXSPI_DLLCR_OVRDEN_SHIFT               (8U)
/*! OVRDEN - Slave clock delay line delay cell number selection override enable. */
#define FLEXSPI_DLLCR_OVRDEN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_OVRDEN_SHIFT)) & FLEXSPI_DLLCR_OVRDEN_MASK)

#define FLEXSPI_DLLCR_OVRDVAL_MASK               (0x7E00U)
#define FLEXSPI_DLLCR_OVRDVAL_SHIFT              (9U)
/*! OVRDVAL - Slave clock delay line delay cell number selection override value. */
#define FLEXSPI_DLLCR_OVRDVAL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_OVRDVAL_SHIFT)) & FLEXSPI_DLLCR_OVRDVAL_MASK)
/*! @} */

/* The count of FLEXSPI_DLLCR */
#define FLEXSPI_DLLCR_COUNT                      (2U)

/*! @name STS0 - Status Register 0 */
/*! @{ */

#define FLEXSPI_STS0_SEQIDLE_MASK                (0x1U)
#define FLEXSPI_STS0_SEQIDLE_SHIFT               (0U)
/*! SEQIDLE - This status bit indicates the state machine in SEQ_CTL is idle and there is command
 *    sequence executing on FlexSPI interface.
 */
#define FLEXSPI_STS0_SEQIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_SEQIDLE_SHIFT)) & FLEXSPI_STS0_SEQIDLE_MASK)

#define FLEXSPI_STS0_ARBIDLE_MASK                (0x2U)
#define FLEXSPI_STS0_ARBIDLE_SHIFT               (1U)
/*! ARBIDLE - This status bit indicates the state machine in ARB_CTL is busy and there is command
 *    sequence granted by arbitrator and not finished yet on FlexSPI interface. When ARB_CTL state
 *    (ARBIDLE=0x1) is idle, there will be no transaction on FlexSPI interface also (SEQIDLE=0x1). So
 *    this bit should be polled to wait for FlexSPI controller become idle instead of SEQIDLE.
 */
#define FLEXSPI_STS0_ARBIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_ARBIDLE_SHIFT)) & FLEXSPI_STS0_ARBIDLE_MASK)

#define FLEXSPI_STS0_ARBCMDSRC_MASK              (0xCU)
#define FLEXSPI_STS0_ARBCMDSRC_SHIFT             (2U)
/*! ARBCMDSRC - This status field indicates the trigger source of current command sequence granted
 *    by arbitrator. This field value is meaningless when ARB_CTL is not busy (STS0[ARBIDLE]=0x1).
 *  0b00..Triggered by AHB read command (triggered by AHB read).
 *  0b01..Triggered by AHB write command (triggered by AHB Write).
 *  0b10..Triggered by IP command (triggered by setting register bit IPCMD.TRG).
 *  0b11..Triggered by suspended command (resumed).
 */
#define FLEXSPI_STS0_ARBCMDSRC(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_ARBCMDSRC_SHIFT)) & FLEXSPI_STS0_ARBCMDSRC_MASK)

#define FLEXSPI_STS0_DATALEARNPHASEA_MASK        (0xF0U)
#define FLEXSPI_STS0_DATALEARNPHASEA_SHIFT       (4U)
/*! DATALEARNPHASEA - Indicate the sampling clock phase selection on Port A after Data Learning. */
#define FLEXSPI_STS0_DATALEARNPHASEA(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_DATALEARNPHASEA_SHIFT)) & FLEXSPI_STS0_DATALEARNPHASEA_MASK)

#define FLEXSPI_STS0_DATALEARNPHASEB_MASK        (0xF00U)
#define FLEXSPI_STS0_DATALEARNPHASEB_SHIFT       (8U)
/*! DATALEARNPHASEB - Indicate the sampling clock phase selection on Port B after Data Learning. */
#define FLEXSPI_STS0_DATALEARNPHASEB(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_DATALEARNPHASEB_SHIFT)) & FLEXSPI_STS0_DATALEARNPHASEB_MASK)
/*! @} */

/*! @name STS1 - Status Register 1 */
/*! @{ */

#define FLEXSPI_STS1_AHBCMDERRID_MASK            (0xFU)
#define FLEXSPI_STS1_AHBCMDERRID_SHIFT           (0U)
/*! AHBCMDERRID - Indicates the sequence index when an AHB command error is detected. This field
 *    will be cleared when INTR[AHBCMDERR] is write-1-clear(w1c).
 */
#define FLEXSPI_STS1_AHBCMDERRID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_AHBCMDERRID_SHIFT)) & FLEXSPI_STS1_AHBCMDERRID_MASK)

#define FLEXSPI_STS1_AHBCMDERRCODE_MASK          (0xF00U)
#define FLEXSPI_STS1_AHBCMDERRCODE_SHIFT         (8U)
/*! AHBCMDERRCODE - Indicates the Error Code when AHB command Error detected. This field will be
 *    cleared when INTR[AHBCMDERR] is write-1-clear(w1c).
 *  0b0000..No error.
 *  0b0010..AHB Write command with JMP_ON_CS instruction used in the sequence.
 *  0b0011..There is unknown instruction opcode in the sequence.
 *  0b0100..Instruction DUMMY_SDR/DUMMY_RWDS_SDR used in DDR sequence.
 *  0b0101..Instruction DUMMY_DDR/DUMMY_RWDS_DDR used in SDR sequence.
 *  0b1110..Sequence execution timeout.
 */
#define FLEXSPI_STS1_AHBCMDERRCODE(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_AHBCMDERRCODE_SHIFT)) & FLEXSPI_STS1_AHBCMDERRCODE_MASK)

#define FLEXSPI_STS1_IPCMDERRID_MASK             (0xF0000U)
#define FLEXSPI_STS1_IPCMDERRID_SHIFT            (16U)
/*! IPCMDERRID - Indicates the sequence Index when IP command error detected. */
#define FLEXSPI_STS1_IPCMDERRID(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_IPCMDERRID_SHIFT)) & FLEXSPI_STS1_IPCMDERRID_MASK)

#define FLEXSPI_STS1_IPCMDERRCODE_MASK           (0xF000000U)
#define FLEXSPI_STS1_IPCMDERRCODE_SHIFT          (24U)
/*! IPCMDERRCODE - Indicates the Error Code when IP command Error detected. This field will be
 *    cleared when INTR[IPCMDERR] is write-1-clear(w1c).
 *  0b0000..No error.
 *  0b0010..IP command with JMP_ON_CS instruction used in the sequence.
 *  0b0011..There is unknown instruction opcode in the sequence.
 *  0b0100..Instruction DUMMY_SDR/DUMMY_RWDS_SDR used in DDR sequence.
 *  0b0101..Instruction DUMMY_DDR/DUMMY_RWDS_DDR used in SDR sequence.
 *  0b0110..Flash access start address exceed the whole flash address range (A1/A2/B1/B2).
 *  0b1110..Sequence execution timeout.
 *  0b1111..Flash boundary crossed.
 */
#define FLEXSPI_STS1_IPCMDERRCODE(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_IPCMDERRCODE_SHIFT)) & FLEXSPI_STS1_IPCMDERRCODE_MASK)
/*! @} */

/*! @name STS2 - Status Register 2 */
/*! @{ */

#define FLEXSPI_STS2_ASLVLOCK_MASK               (0x1U)
#define FLEXSPI_STS2_ASLVLOCK_SHIFT              (0U)
/*! ASLVLOCK - Flash A sample clock slave delay line locked. */
#define FLEXSPI_STS2_ASLVLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_ASLVLOCK_SHIFT)) & FLEXSPI_STS2_ASLVLOCK_MASK)

#define FLEXSPI_STS2_AREFLOCK_MASK               (0x2U)
#define FLEXSPI_STS2_AREFLOCK_SHIFT              (1U)
/*! AREFLOCK - Flash A sample clock reference delay line locked. */
#define FLEXSPI_STS2_AREFLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_AREFLOCK_SHIFT)) & FLEXSPI_STS2_AREFLOCK_MASK)

#define FLEXSPI_STS2_ASLVSEL_MASK                (0xFCU)
#define FLEXSPI_STS2_ASLVSEL_SHIFT               (2U)
/*! ASLVSEL - Flash A sample clock slave delay line delay cell number selection . */
#define FLEXSPI_STS2_ASLVSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_ASLVSEL_SHIFT)) & FLEXSPI_STS2_ASLVSEL_MASK)

#define FLEXSPI_STS2_AREFSEL_MASK                (0x3F00U)
#define FLEXSPI_STS2_AREFSEL_SHIFT               (8U)
/*! AREFSEL - Flash A sample clock reference delay line delay cell number selection. */
#define FLEXSPI_STS2_AREFSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_AREFSEL_SHIFT)) & FLEXSPI_STS2_AREFSEL_MASK)

#define FLEXSPI_STS2_BSLVLOCK_MASK               (0x10000U)
#define FLEXSPI_STS2_BSLVLOCK_SHIFT              (16U)
/*! BSLVLOCK - Flash B sample clock slave delay line locked. */
#define FLEXSPI_STS2_BSLVLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BSLVLOCK_SHIFT)) & FLEXSPI_STS2_BSLVLOCK_MASK)

#define FLEXSPI_STS2_BREFLOCK_MASK               (0x20000U)
#define FLEXSPI_STS2_BREFLOCK_SHIFT              (17U)
/*! BREFLOCK - Flash B sample clock reference delay line locked. */
#define FLEXSPI_STS2_BREFLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BREFLOCK_SHIFT)) & FLEXSPI_STS2_BREFLOCK_MASK)

#define FLEXSPI_STS2_BSLVSEL_MASK                (0xFC0000U)
#define FLEXSPI_STS2_BSLVSEL_SHIFT               (18U)
/*! BSLVSEL - Flash B sample clock slave delay line delay cell number selection. */
#define FLEXSPI_STS2_BSLVSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BSLVSEL_SHIFT)) & FLEXSPI_STS2_BSLVSEL_MASK)

#define FLEXSPI_STS2_BREFSEL_MASK                (0x3F000000U)
#define FLEXSPI_STS2_BREFSEL_SHIFT               (24U)
/*! BREFSEL - Flash B sample clock reference delay line delay cell number selection. */
#define FLEXSPI_STS2_BREFSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BREFSEL_SHIFT)) & FLEXSPI_STS2_BREFSEL_MASK)
/*! @} */

/*! @name AHBSPNDSTS - AHB Suspend Status Register */
/*! @{ */

#define FLEXSPI_AHBSPNDSTS_ACTIVE_MASK           (0x1U)
#define FLEXSPI_AHBSPNDSTS_ACTIVE_SHIFT          (0U)
/*! ACTIVE - Indicates if an AHB read prefetch command sequence has been suspended. */
#define FLEXSPI_AHBSPNDSTS_ACTIVE(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_ACTIVE_SHIFT)) & FLEXSPI_AHBSPNDSTS_ACTIVE_MASK)

#define FLEXSPI_AHBSPNDSTS_BUFID_MASK            (0xEU)
#define FLEXSPI_AHBSPNDSTS_BUFID_SHIFT           (1U)
/*! BUFID - AHB RX BUF ID for suspended command sequence. */
#define FLEXSPI_AHBSPNDSTS_BUFID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_BUFID_SHIFT)) & FLEXSPI_AHBSPNDSTS_BUFID_MASK)

#define FLEXSPI_AHBSPNDSTS_DATLFT_MASK           (0xFFFF0000U)
#define FLEXSPI_AHBSPNDSTS_DATLFT_SHIFT          (16U)
/*! DATLFT - Left Data size for suspended command sequence (in byte). */
#define FLEXSPI_AHBSPNDSTS_DATLFT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_DATLFT_SHIFT)) & FLEXSPI_AHBSPNDSTS_DATLFT_MASK)
/*! @} */

/*! @name IPRXFSTS - IP RX FIFO Status Register */
/*! @{ */

#define FLEXSPI_IPRXFSTS_FILL_MASK               (0xFFU)
#define FLEXSPI_IPRXFSTS_FILL_SHIFT              (0U)
/*! FILL - Fill level of IP RX FIFO. */
#define FLEXSPI_IPRXFSTS_FILL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFSTS_FILL_SHIFT)) & FLEXSPI_IPRXFSTS_FILL_MASK)

#define FLEXSPI_IPRXFSTS_RDCNTR_MASK             (0xFFFF0000U)
#define FLEXSPI_IPRXFSTS_RDCNTR_SHIFT            (16U)
/*! RDCNTR - Total Read Data Counter: RDCNTR * 64 Bits. */
#define FLEXSPI_IPRXFSTS_RDCNTR(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFSTS_RDCNTR_SHIFT)) & FLEXSPI_IPRXFSTS_RDCNTR_MASK)
/*! @} */

/*! @name IPTXFSTS - IP TX FIFO Status Register */
/*! @{ */

#define FLEXSPI_IPTXFSTS_FILL_MASK               (0xFFU)
#define FLEXSPI_IPTXFSTS_FILL_SHIFT              (0U)
/*! FILL - Fill level of IP TX FIFO. */
#define FLEXSPI_IPTXFSTS_FILL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFSTS_FILL_SHIFT)) & FLEXSPI_IPTXFSTS_FILL_MASK)

#define FLEXSPI_IPTXFSTS_WRCNTR_MASK             (0xFFFF0000U)
#define FLEXSPI_IPTXFSTS_WRCNTR_SHIFT            (16U)
/*! WRCNTR - Total Write Data Counter: WRCNTR * 64 Bits. */
#define FLEXSPI_IPTXFSTS_WRCNTR(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFSTS_WRCNTR_SHIFT)) & FLEXSPI_IPTXFSTS_WRCNTR_MASK)
/*! @} */

/*! @name RFDR - IP RX FIFO Data Register 0..IP RX FIFO Data Register 31 */
/*! @{ */

#define FLEXSPI_RFDR_RXDATA_MASK                 (0xFFFFFFFFU)
#define FLEXSPI_RFDR_RXDATA_SHIFT                (0U)
/*! RXDATA - RX Data */
#define FLEXSPI_RFDR_RXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_RFDR_RXDATA_SHIFT)) & FLEXSPI_RFDR_RXDATA_MASK)
/*! @} */

/*! @name TFDR - IP TX FIFO Data Register 0..IP TX FIFO Data Register 31 */
/*! @{ */

#define FLEXSPI_TFDR_TXDATA_MASK                 (0xFFFFFFFFU)
#define FLEXSPI_TFDR_TXDATA_SHIFT                (0U)
/*! TXDATA - TX Data */
#define FLEXSPI_TFDR_TXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_TFDR_TXDATA_SHIFT)) & FLEXSPI_TFDR_TXDATA_MASK)
/*! @} */

/*! @name LUT - LUT 0..LUT 63 */
/*! @{ */

#define FLEXSPI_LUT_OPERAND0_MASK                (0xFFU)
#define FLEXSPI_LUT_OPERAND0_SHIFT               (0U)
/*! OPERAND0 - OPERAND0 */
#define FLEXSPI_LUT_OPERAND0(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPERAND0_SHIFT)) & FLEXSPI_LUT_OPERAND0_MASK)

#define FLEXSPI_LUT_NUM_PADS0_MASK               (0x300U)
#define FLEXSPI_LUT_NUM_PADS0_SHIFT              (8U)
/*! NUM_PADS0 - NUM_PADS0 */
#define FLEXSPI_LUT_NUM_PADS0(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_NUM_PADS0_SHIFT)) & FLEXSPI_LUT_NUM_PADS0_MASK)

#define FLEXSPI_LUT_OPCODE0_MASK                 (0xFC00U)
#define FLEXSPI_LUT_OPCODE0_SHIFT                (10U)
/*! OPCODE0 - OPCODE */
#define FLEXSPI_LUT_OPCODE0(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPCODE0_SHIFT)) & FLEXSPI_LUT_OPCODE0_MASK)

#define FLEXSPI_LUT_OPERAND1_MASK                (0xFF0000U)
#define FLEXSPI_LUT_OPERAND1_SHIFT               (16U)
/*! OPERAND1 - OPERAND1 */
#define FLEXSPI_LUT_OPERAND1(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPERAND1_SHIFT)) & FLEXSPI_LUT_OPERAND1_MASK)

#define FLEXSPI_LUT_NUM_PADS1_MASK               (0x3000000U)
#define FLEXSPI_LUT_NUM_PADS1_SHIFT              (24U)
/*! NUM_PADS1 - NUM_PADS1 */
#define FLEXSPI_LUT_NUM_PADS1(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_NUM_PADS1_SHIFT)) & FLEXSPI_LUT_NUM_PADS1_MASK)

#define FLEXSPI_LUT_OPCODE1_MASK                 (0xFC000000U)
#define FLEXSPI_LUT_OPCODE1_SHIFT                (26U)
/*! OPCODE1 - OPCODE1 */
#define FLEXSPI_LUT_OPCODE1(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPCODE1_SHIFT)) & FLEXSPI_LUT_OPCODE1_MASK)
/*! @} */

/*! @name HADDRSTART - HADDR REMAP START ADDR */
/*! @{ */

#define FLEXSPI_HADDRSTART_REMAPEN_MASK          (0x1U)
#define FLEXSPI_HADDRSTART_REMAPEN_SHIFT         (0U)
/*! REMAPEN - AHB Bus address remap function enable
 *  0b0..HADDR REMAP Disabled
 *  0b1..HADDR REMAP Enabled
 */
#define FLEXSPI_HADDRSTART_REMAPEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_HADDRSTART_REMAPEN_SHIFT)) & FLEXSPI_HADDRSTART_REMAPEN_MASK)

#define FLEXSPI_HADDRSTART_ADDRSTART_MASK        (0xFFFFF000U)
#define FLEXSPI_HADDRSTART_ADDRSTART_SHIFT       (12U)
/*! ADDRSTART - HADDR start address */
#define FLEXSPI_HADDRSTART_ADDRSTART(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_HADDRSTART_ADDRSTART_SHIFT)) & FLEXSPI_HADDRSTART_ADDRSTART_MASK)
/*! @} */

/*! @name HADDREND - HADDR REMAP END ADDR */
/*! @{ */

#define FLEXSPI_HADDREND_ENDSTART_MASK           (0xFFFFF000U)
#define FLEXSPI_HADDREND_ENDSTART_SHIFT          (12U)
/*! ENDSTART - HADDR remap range's end addr, 4K aligned */
#define FLEXSPI_HADDREND_ENDSTART(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_HADDREND_ENDSTART_SHIFT)) & FLEXSPI_HADDREND_ENDSTART_MASK)
/*! @} */

/*! @name HADDROFFSET - HADDR REMAP OFFSET */
/*! @{ */

#define FLEXSPI_HADDROFFSET_ADDROFFSET_MASK      (0xFFFFF000U)
#define FLEXSPI_HADDROFFSET_ADDROFFSET_SHIFT     (12U)
/*! ADDROFFSET - HADDR offset field, remapped address will be ADDR[31:12]=ADDR_original[31:12]+ADDROFFSET */
#define FLEXSPI_HADDROFFSET_ADDROFFSET(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_HADDROFFSET_ADDROFFSET_SHIFT)) & FLEXSPI_HADDROFFSET_ADDROFFSET_MASK)
/*! @} */

/*! @name IPEDCTRL - IPED function control */
/*! @{ */

#define FLEXSPI_IPEDCTRL_CONFIG_MASK             (0x1U)
#define FLEXSPI_IPEDCTRL_CONFIG_SHIFT            (0U)
/*! CONFIG - Drive IPED interface i_config. */
#define FLEXSPI_IPEDCTRL_CONFIG(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_CONFIG_SHIFT)) & FLEXSPI_IPEDCTRL_CONFIG_MASK)

#define FLEXSPI_IPEDCTRL_IPED_EN_MASK            (0x2U)
#define FLEXSPI_IPEDCTRL_IPED_EN_SHIFT           (1U)
/*! IPED_EN - Drive IPED interface i_enable */
#define FLEXSPI_IPEDCTRL_IPED_EN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPED_EN_SHIFT)) & FLEXSPI_IPEDCTRL_IPED_EN_MASK)

#define FLEXSPI_IPEDCTRL_IPWR_EN_MASK            (0x4U)
#define FLEXSPI_IPEDCTRL_IPWR_EN_SHIFT           (2U)
/*! IPWR_EN - IP write IPED CTR mode encryption enable */
#define FLEXSPI_IPEDCTRL_IPWR_EN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPWR_EN_SHIFT)) & FLEXSPI_IPEDCTRL_IPWR_EN_MASK)

#define FLEXSPI_IPEDCTRL_AHBWR_EN_MASK           (0x8U)
#define FLEXSPI_IPEDCTRL_AHBWR_EN_SHIFT          (3U)
/*! AHBWR_EN - AHB write IPED CTR mode encryption enable */
#define FLEXSPI_IPEDCTRL_AHBWR_EN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_AHBWR_EN_SHIFT)) & FLEXSPI_IPEDCTRL_AHBWR_EN_MASK)

#define FLEXSPI_IPEDCTRL_AHBRD_EN_MASK           (0x10U)
#define FLEXSPI_IPEDCTRL_AHBRD_EN_SHIFT          (4U)
/*! AHBRD_EN - AHB read IPED CTR mode decryption enable */
#define FLEXSPI_IPEDCTRL_AHBRD_EN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_AHBRD_EN_SHIFT)) & FLEXSPI_IPEDCTRL_AHBRD_EN_MASK)

#define FLEXSPI_IPEDCTRL_IPWROTA_MASK            (0x20U)
#define FLEXSPI_IPEDCTRL_IPWROTA_SHIFT           (5U)
/*! IPWROTA - IP GCM mode command write OTA region */
#define FLEXSPI_IPEDCTRL_IPWROTA(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPWROTA_SHIFT)) & FLEXSPI_IPEDCTRL_IPWROTA_MASK)

#define FLEXSPI_IPEDCTRL_IPGCMWR_MASK            (0x40U)
#define FLEXSPI_IPEDCTRL_IPGCMWR_SHIFT           (6U)
/*! IPGCMWR - IP write GCM mode enable */
#define FLEXSPI_IPEDCTRL_IPGCMWR(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPGCMWR_SHIFT)) & FLEXSPI_IPEDCTRL_IPGCMWR_MASK)

#define FLEXSPI_IPEDCTRL_AHGCMWR_MASK            (0x80U)
#define FLEXSPI_IPEDCTRL_AHGCMWR_SHIFT           (7U)
/*! AHGCMWR - AHB write IPED GCM mode encryption enable */
#define FLEXSPI_IPEDCTRL_AHGCMWR(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_AHGCMWR_SHIFT)) & FLEXSPI_IPEDCTRL_AHGCMWR_MASK)

#define FLEXSPI_IPEDCTRL_AHBGCMRD_MASK           (0x100U)
#define FLEXSPI_IPEDCTRL_AHBGCMRD_SHIFT          (8U)
/*! AHBGCMRD - AHB read IPED GCM mode decryption enable */
#define FLEXSPI_IPEDCTRL_AHBGCMRD(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_AHBGCMRD_SHIFT)) & FLEXSPI_IPEDCTRL_AHBGCMRD_MASK)

#define FLEXSPI_IPEDCTRL_IPED_PROTECT_MASK       (0x200U)
#define FLEXSPI_IPEDCTRL_IPED_PROTECT_SHIFT      (9U)
/*! IPED_PROTECT - when ipedctrl protect = 0 or priviledge access, no restriction when ipedctrl
 *    protect = 1, only priviledge access can write.
 */
#define FLEXSPI_IPEDCTRL_IPED_PROTECT(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPED_PROTECT_SHIFT)) & FLEXSPI_IPEDCTRL_IPED_PROTECT_MASK)

#define FLEXSPI_IPEDCTRL_IPED_SWRESET_MASK       (0x400U)
#define FLEXSPI_IPEDCTRL_IPED_SWRESET_SHIFT      (10U)
/*! IPED_SWRESET - Drive IPED interface i_abort. */
#define FLEXSPI_IPEDCTRL_IPED_SWRESET(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTRL_IPED_SWRESET_SHIFT)) & FLEXSPI_IPEDCTRL_IPED_SWRESET_MASK)
/*! @} */

/*! @name IPSNSZSTART0 - IPS nonsecure region Start address of region 0 */
/*! @{ */

#define FLEXSPI_IPSNSZSTART0_START_ADDRESS_MASK  (0xFFFFF000U)
#define FLEXSPI_IPSNSZSTART0_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 0. Minimal 4K Bytes aligned. It is flash address. */
#define FLEXSPI_IPSNSZSTART0_START_ADDRESS(x)    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPSNSZSTART0_START_ADDRESS_SHIFT)) & FLEXSPI_IPSNSZSTART0_START_ADDRESS_MASK)
/*! @} */

/*! @name IPSNSZEND0 - IPS nonsecure region End address of region 0 */
/*! @{ */

#define FLEXSPI_IPSNSZEND0_END_ADDRESS_MASK      (0xFFFFF000U)
#define FLEXSPI_IPSNSZEND0_END_ADDRESS_SHIFT     (12U)
/*! END_ADDRESS - End address of region 0. Minimal 4K Bytes aligned. It is flash address. */
#define FLEXSPI_IPSNSZEND0_END_ADDRESS(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPSNSZEND0_END_ADDRESS_SHIFT)) & FLEXSPI_IPSNSZEND0_END_ADDRESS_MASK)
/*! @} */

/*! @name IPSNSZSTART1 - IPS nonsecure region Start address of region 1 */
/*! @{ */

#define FLEXSPI_IPSNSZSTART1_START_ADDRESS_MASK  (0xFFFFF000U)
#define FLEXSPI_IPSNSZSTART1_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 1. Minimal 4K Bytes aligned. It is flash address. */
#define FLEXSPI_IPSNSZSTART1_START_ADDRESS(x)    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPSNSZSTART1_START_ADDRESS_SHIFT)) & FLEXSPI_IPSNSZSTART1_START_ADDRESS_MASK)
/*! @} */

/*! @name IPSNSZEND1 - IPS nonsecure region End address of region 1 */
/*! @{ */

#define FLEXSPI_IPSNSZEND1_END_ADDRESS_MASK      (0xFFFFF000U)
#define FLEXSPI_IPSNSZEND1_END_ADDRESS_SHIFT     (12U)
/*! END_ADDRESS - End address of region 1. Minimal 4K Bytes aligned. It is flash address. */
#define FLEXSPI_IPSNSZEND1_END_ADDRESS(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPSNSZEND1_END_ADDRESS_SHIFT)) & FLEXSPI_IPSNSZEND1_END_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONSTART0 - RX BUF Start address of region 0 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONSTART0_START_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONSTART0_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 0. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONSTART0_START_ADDRESS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONSTART0_START_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONSTART0_START_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONEND0 - RX BUF region End address of region 0 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONEND0_END_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONEND0_END_ADDRESS_SHIFT (12U)
/*! END_ADDRESS - End address of region 0. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONEND0_END_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONEND0_END_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONEND0_END_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONSTART1 - RX BUF Start address of region 1 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONSTART1_START_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONSTART1_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 1. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONSTART1_START_ADDRESS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONSTART1_START_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONSTART1_START_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONEND1 - RX BUF region End address of region 1 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONEND1_END_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONEND1_END_ADDRESS_SHIFT (12U)
/*! END_ADDRESS - End address of region 1. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONEND1_END_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONEND1_END_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONEND1_END_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONSTART2 - RX BUF Start address of region 2 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONSTART2_START_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONSTART2_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 2. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONSTART2_START_ADDRESS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONSTART2_START_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONSTART2_START_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONEND2 - RX BUF region End address of region 2 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONEND2_END_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONEND2_END_ADDRESS_SHIFT (12U)
/*! END_ADDRESS - End address of region 2. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONEND2_END_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONEND2_END_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONEND2_END_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONSTART3 - RX BUF Start address of region 3 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONSTART3_START_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONSTART3_START_ADDRESS_SHIFT (12U)
/*! START_ADDRESS - Start address of region 3. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONSTART3_START_ADDRESS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONSTART3_START_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONSTART3_START_ADDRESS_MASK)
/*! @} */

/*! @name AHBBUFREGIONEND3 - RX BUF region End address of region 3 */
/*! @{ */

#define FLEXSPI_AHBBUFREGIONEND3_END_ADDRESS_MASK (0xFFFFF000U)
#define FLEXSPI_AHBBUFREGIONEND3_END_ADDRESS_SHIFT (12U)
/*! END_ADDRESS - End address of region 3. Minimal 4K Bytes aligned. It is system address. */
#define FLEXSPI_AHBBUFREGIONEND3_END_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBBUFREGIONEND3_END_ADDRESS_SHIFT)) & FLEXSPI_AHBBUFREGIONEND3_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTXCTRL - IPED context control 0..IPED context control 1 */
/*! @{ */

#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK    (0x3U)
#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_SHIFT   (0U)
/*! CTX0_FREEZE0 - Controls the RW properties of this field and region 0 context registers (CTX0_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_MASK    (0x3U)
#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_SHIFT   (0U)
/*! CTX0_FREEZE1 - Controls the RW properties of this field and region 0 context registers (CTX0_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX0_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE0_MASK    (0xCU)
#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE0_SHIFT   (2U)
/*! CTX1_FREEZE0 - Controls the RW properties of this field and region 1 context registers (CTX1_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE1_MASK    (0xCU)
#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE1_SHIFT   (2U)
/*! CTX1_FREEZE1 - Controls the RW properties of this field and region 1 context registers (CTX1_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX1_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE0_MASK    (0x30U)
#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE0_SHIFT   (4U)
/*! CTX2_FREEZE0 - Controls the RW properties of this field and region 2 context registers (CTX2_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE1_MASK    (0x30U)
#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE1_SHIFT   (4U)
/*! CTX2_FREEZE1 - Controls the RW properties of this field and region 2 context registers (CTX2_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX2_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE0_MASK    (0xC0U)
#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE0_SHIFT   (6U)
/*! CTX3_FREEZE0 - Controls the RW properties of this field and region 3 context registers (CTX3_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE1_MASK    (0xC0U)
#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE1_SHIFT   (6U)
/*! CTX3_FREEZE1 - Controls the RW properties of this field and region 3 context registers (CTX3_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX3_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE0_MASK    (0x300U)
#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE0_SHIFT   (8U)
/*! CTX4_FREEZE0 - Controls the RW properties of this field and region 4 context registers (CTX4_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE1_MASK    (0x300U)
#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE1_SHIFT   (8U)
/*! CTX4_FREEZE1 - Controls the RW properties of this field and region 4 context registers (CTX4_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX4_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE0_MASK    (0xC00U)
#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE0_SHIFT   (10U)
/*! CTX5_FREEZE0 - Controls the RW properties of this field and region 5 context registers (CTX5_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE1_MASK    (0xC00U)
#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE1_SHIFT   (10U)
/*! CTX5_FREEZE1 - Controls the RW properties of this field and region 5 context registers (CTX5_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX5_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE0_MASK    (0x3000U)
#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE0_SHIFT   (12U)
/*! CTX6_FREEZE0 - Controls the RW properties of this field and region 6 context registers (CTX6_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE1_MASK    (0x3000U)
#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE1_SHIFT   (12U)
/*! CTX6_FREEZE1 - Controls the RW properties of this field and region 6 context registers (CTX6_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX6_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE0_MASK    (0xC000U)
#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE0_SHIFT   (14U)
/*! CTX7_FREEZE0 - Controls the RW properties of this field and region 7 context registers (CTX7_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE1_MASK    (0xC000U)
#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE1_SHIFT   (14U)
/*! CTX7_FREEZE1 - Controls the RW properties of this field and region 7 context registers (CTX7_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX7_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE0_MASK    (0x30000U)
#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE0_SHIFT   (16U)
/*! CTX8_FREEZE0 - Controls the RW properties of this field and region 8 context registers (CTX8_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE1_MASK    (0x30000U)
#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE1_SHIFT   (16U)
/*! CTX8_FREEZE1 - Controls the RW properties of this field and region 8 context registers (CTX8_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX8_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE0_MASK    (0xC0000U)
#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE0_SHIFT   (18U)
/*! CTX9_FREEZE0 - Controls the RW properties of this field and region 9 context registers (CTX9_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE1_MASK    (0xC0000U)
#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE1_SHIFT   (18U)
/*! CTX9_FREEZE1 - Controls the RW properties of this field and region 9 context registers (CTX9_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX9_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE0_MASK   (0x300000U)
#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE0_SHIFT  (20U)
/*! CTX10_FREEZE0 - Controls the RW properties of this field and region 10 context registers (CTX10_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE0(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE1_MASK   (0x300000U)
#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE1_SHIFT  (20U)
/*! CTX10_FREEZE1 - Controls the RW properties of this field and region 10 context registers (CTX10_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE1(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX10_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE0_MASK   (0xC00000U)
#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE0_SHIFT  (22U)
/*! CTX11_FREEZE0 - Controls the RW properties of this field and region 11 context registers (CTX11_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE0(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE1_MASK   (0xC00000U)
#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE1_SHIFT  (22U)
/*! CTX11_FREEZE1 - Controls the RW properties of this field and region 11 context registers (CTX11_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE1(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX11_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE0_MASK   (0x3000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE0_SHIFT  (24U)
/*! CTX12_FREEZE0 - Controls the RW properties of this field and region 12 context registers (CTX12_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE0(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE1_MASK   (0x3000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE1_SHIFT  (24U)
/*! CTX12_FREEZE1 - Controls the RW properties of this field and region 12 context registers (CTX12_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE1(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX12_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE0_MASK   (0xC000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE0_SHIFT  (26U)
/*! CTX13_FREEZE0 - Controls the RW properties of this field and region 13 context registers (CTX13_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE0(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE1_MASK   (0xC000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE1_SHIFT  (26U)
/*! CTX13_FREEZE1 - Controls the RW properties of this field and region 13 context registers (CTX13_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE1(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX13_FREEZE1_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE0_MASK   (0x30000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE0_SHIFT  (28U)
/*! CTX14_FREEZE0 - Controls the RW properties of this field and region 14 context registers (CTX14_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE0(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE0_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE0_MASK)

#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE1_MASK   (0x30000000U)
#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE1_SHIFT  (28U)
/*! CTX14_FREEZE1 - Controls the RW properties of this field and region 14 context registers (CTX14_xxxx). */
#define FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE1(x)     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE1_SHIFT)) & FLEXSPI_IPEDCTXCTRL_CTX14_FREEZE1_MASK)
/*! @} */

/* The count of FLEXSPI_IPEDCTXCTRL */
#define FLEXSPI_IPEDCTXCTRL_COUNT                (2U)

/*! @name IPEDCTX0IV0 - IPED context0 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX0IV0_CTX0_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX0IV0_CTX0_IV0_SHIFT       (0U)
/*! CTX0_IV0 - Lowest 32 bits of IV for region 0. */
#define FLEXSPI_IPEDCTX0IV0_CTX0_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0IV0_CTX0_IV0_SHIFT)) & FLEXSPI_IPEDCTX0IV0_CTX0_IV0_MASK)
/*! @} */

/*! @name IPEDCTX0IV1 - IPED context0 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX0IV1_CTX0_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX0IV1_CTX0_IV1_SHIFT       (0U)
/*! CTX0_IV1 - Highest 32 bits of IV for region 0. */
#define FLEXSPI_IPEDCTX0IV1_CTX0_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0IV1_CTX0_IV1_SHIFT)) & FLEXSPI_IPEDCTX0IV1_CTX0_IV1_MASK)
/*! @} */

/*! @name IPEDCTX0START - Start address of region 0 */
/*! @{ */

#define FLEXSPI_IPEDCTX0START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX0START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX0START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0START_GCM_SHIFT)) & FLEXSPI_IPEDCTX0START_GCM_MASK)

#define FLEXSPI_IPEDCTX0START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX0START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX0START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX0START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX0START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX0START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 0. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX0START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX0START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX0END - End address of region 0 */
/*! @{ */

#define FLEXSPI_IPEDCTX0END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX0END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 0. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX0END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX0END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX0AAD0 - IPED context0 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX0AAD0_CTX0_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX0AAD0_CTX0_AAD0_SHIFT     (0U)
/*! CTX0_AAD0 - Lowest 32 bits of AAD for region 0. */
#define FLEXSPI_IPEDCTX0AAD0_CTX0_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0AAD0_CTX0_AAD0_SHIFT)) & FLEXSPI_IPEDCTX0AAD0_CTX0_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX0AAD1 - IPED context0 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX0AAD1_CTX0_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX0AAD1_CTX0_AAD1_SHIFT     (0U)
/*! CTX0_AAD1 - Highest 32 bits of AAD for region 0. */
#define FLEXSPI_IPEDCTX0AAD1_CTX0_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX0AAD1_CTX0_AAD1_SHIFT)) & FLEXSPI_IPEDCTX0AAD1_CTX0_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX1IV0 - IPED context1 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX1IV0_CTX1_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX1IV0_CTX1_IV0_SHIFT       (0U)
/*! CTX1_IV0 - Lowest 32 bits of IV for region 1. */
#define FLEXSPI_IPEDCTX1IV0_CTX1_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1IV0_CTX1_IV0_SHIFT)) & FLEXSPI_IPEDCTX1IV0_CTX1_IV0_MASK)
/*! @} */

/*! @name IPEDCTX1IV1 - IPED context1 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX1IV1_CTX1_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX1IV1_CTX1_IV1_SHIFT       (0U)
/*! CTX1_IV1 - Highest 32 bits of IV for region 1. */
#define FLEXSPI_IPEDCTX1IV1_CTX1_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1IV1_CTX1_IV1_SHIFT)) & FLEXSPI_IPEDCTX1IV1_CTX1_IV1_MASK)
/*! @} */

/*! @name IPEDCTX1START - Start address of region 1 */
/*! @{ */

#define FLEXSPI_IPEDCTX1START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX1START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX1START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1START_GCM_SHIFT)) & FLEXSPI_IPEDCTX1START_GCM_MASK)

#define FLEXSPI_IPEDCTX1START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX1START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX1START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX1START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX1START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX1START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 1. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX1START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX1START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX1END - End address of region 1 */
/*! @{ */

#define FLEXSPI_IPEDCTX1END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX1END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 1. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX1END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX1END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX1AAD0 - IPED context1 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX1AAD0_CTX1_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX1AAD0_CTX1_AAD0_SHIFT     (0U)
/*! CTX1_AAD0 - Lowest 32 bits of AAD for region 1. */
#define FLEXSPI_IPEDCTX1AAD0_CTX1_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1AAD0_CTX1_AAD0_SHIFT)) & FLEXSPI_IPEDCTX1AAD0_CTX1_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX1AAD1 - IPED context1 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX1AAD1_CTX1_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX1AAD1_CTX1_AAD1_SHIFT     (0U)
/*! CTX1_AAD1 - Highest 32 bits of AAD for region 1. */
#define FLEXSPI_IPEDCTX1AAD1_CTX1_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX1AAD1_CTX1_AAD1_SHIFT)) & FLEXSPI_IPEDCTX1AAD1_CTX1_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX2IV0 - IPED context2 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX2IV0_CTX2_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX2IV0_CTX2_IV0_SHIFT       (0U)
/*! CTX2_IV0 - Lowest 32 bits of IV for region 2. */
#define FLEXSPI_IPEDCTX2IV0_CTX2_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2IV0_CTX2_IV0_SHIFT)) & FLEXSPI_IPEDCTX2IV0_CTX2_IV0_MASK)
/*! @} */

/*! @name IPEDCTX2IV1 - IPED context2 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX2IV1_CTX2_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX2IV1_CTX2_IV1_SHIFT       (0U)
/*! CTX2_IV1 - Highest 32 bits of IV for region 2. */
#define FLEXSPI_IPEDCTX2IV1_CTX2_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2IV1_CTX2_IV1_SHIFT)) & FLEXSPI_IPEDCTX2IV1_CTX2_IV1_MASK)
/*! @} */

/*! @name IPEDCTX2START - Start address of region 2 */
/*! @{ */

#define FLEXSPI_IPEDCTX2START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX2START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX2START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2START_GCM_SHIFT)) & FLEXSPI_IPEDCTX2START_GCM_MASK)

#define FLEXSPI_IPEDCTX2START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX2START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX2START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX2START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX2START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX2START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 2. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX2START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX2START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX2END - End address of region 2 */
/*! @{ */

#define FLEXSPI_IPEDCTX2END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX2END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 2. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX2END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX2END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX2AAD0 - IPED context2 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX2AAD0_CTX2_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX2AAD0_CTX2_AAD0_SHIFT     (0U)
/*! CTX2_AAD0 - Lowest 32 bits of AAD for region 2. */
#define FLEXSPI_IPEDCTX2AAD0_CTX2_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2AAD0_CTX2_AAD0_SHIFT)) & FLEXSPI_IPEDCTX2AAD0_CTX2_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX2AAD1 - IPED context2 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX2AAD1_CTX2_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX2AAD1_CTX2_AAD1_SHIFT     (0U)
/*! CTX2_AAD1 - Highest 32 bits of AAD for region 2. */
#define FLEXSPI_IPEDCTX2AAD1_CTX2_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX2AAD1_CTX2_AAD1_SHIFT)) & FLEXSPI_IPEDCTX2AAD1_CTX2_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX3IV0 - IPED context3 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX3IV0_CTX3_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX3IV0_CTX3_IV0_SHIFT       (0U)
/*! CTX3_IV0 - Lowest 32 bits of IV for region 3. */
#define FLEXSPI_IPEDCTX3IV0_CTX3_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3IV0_CTX3_IV0_SHIFT)) & FLEXSPI_IPEDCTX3IV0_CTX3_IV0_MASK)
/*! @} */

/*! @name IPEDCTX3IV1 - IPED context3 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX3IV1_CTX3_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX3IV1_CTX3_IV1_SHIFT       (0U)
/*! CTX3_IV1 - Highest 32 bits of IV for region 3. */
#define FLEXSPI_IPEDCTX3IV1_CTX3_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3IV1_CTX3_IV1_SHIFT)) & FLEXSPI_IPEDCTX3IV1_CTX3_IV1_MASK)
/*! @} */

/*! @name IPEDCTX3START - Start address of region 3 */
/*! @{ */

#define FLEXSPI_IPEDCTX3START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX3START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX3START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3START_GCM_SHIFT)) & FLEXSPI_IPEDCTX3START_GCM_MASK)

#define FLEXSPI_IPEDCTX3START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX3START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX3START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX3START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX3START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX3START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 3. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX3START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX3START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX3END - End address of region 3 */
/*! @{ */

#define FLEXSPI_IPEDCTX3END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX3END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 3. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX3END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX3END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX3AAD0 - IPED context3 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX3AAD0_CTX3_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX3AAD0_CTX3_AAD0_SHIFT     (0U)
/*! CTX3_AAD0 - Lowest 32 bits of AAD for region 3. */
#define FLEXSPI_IPEDCTX3AAD0_CTX3_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3AAD0_CTX3_AAD0_SHIFT)) & FLEXSPI_IPEDCTX3AAD0_CTX3_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX3AAD1 - IPED context3 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX3AAD1_CTX3_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX3AAD1_CTX3_AAD1_SHIFT     (0U)
/*! CTX3_AAD1 - Highest 32 bits of AAD for region 3. */
#define FLEXSPI_IPEDCTX3AAD1_CTX3_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX3AAD1_CTX3_AAD1_SHIFT)) & FLEXSPI_IPEDCTX3AAD1_CTX3_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX4IV0 - IPED context4 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX4IV0_CTX4_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX4IV0_CTX4_IV0_SHIFT       (0U)
/*! CTX4_IV0 - Lowest 32 bits of IV for region 4. */
#define FLEXSPI_IPEDCTX4IV0_CTX4_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4IV0_CTX4_IV0_SHIFT)) & FLEXSPI_IPEDCTX4IV0_CTX4_IV0_MASK)
/*! @} */

/*! @name IPEDCTX4IV1 - IPED context4 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX4IV1_CTX4_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX4IV1_CTX4_IV1_SHIFT       (0U)
/*! CTX4_IV1 - Highest 32 bits of IV for region 4. */
#define FLEXSPI_IPEDCTX4IV1_CTX4_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4IV1_CTX4_IV1_SHIFT)) & FLEXSPI_IPEDCTX4IV1_CTX4_IV1_MASK)
/*! @} */

/*! @name IPEDCTX4START - Start address of region 4 */
/*! @{ */

#define FLEXSPI_IPEDCTX4START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX4START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX4START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4START_GCM_SHIFT)) & FLEXSPI_IPEDCTX4START_GCM_MASK)

#define FLEXSPI_IPEDCTX4START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX4START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX4START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX4START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX4START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX4START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 4. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX4START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX4START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX4END - End address of region 4 */
/*! @{ */

#define FLEXSPI_IPEDCTX4END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX4END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 4. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX4END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX4END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX4AAD0 - IPED context4 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX4AAD0_CTX4_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX4AAD0_CTX4_AAD0_SHIFT     (0U)
/*! CTX4_AAD0 - Lowest 32 bits of AAD for region 4. */
#define FLEXSPI_IPEDCTX4AAD0_CTX4_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4AAD0_CTX4_AAD0_SHIFT)) & FLEXSPI_IPEDCTX4AAD0_CTX4_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX4AAD1 - IPED context4 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX4AAD1_CTX4_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX4AAD1_CTX4_AAD1_SHIFT     (0U)
/*! CTX4_AAD1 - Highest 32 bits of AAD for region 4. */
#define FLEXSPI_IPEDCTX4AAD1_CTX4_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX4AAD1_CTX4_AAD1_SHIFT)) & FLEXSPI_IPEDCTX4AAD1_CTX4_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX5IV0 - IPED context5 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX5IV0_CTX5_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX5IV0_CTX5_IV0_SHIFT       (0U)
/*! CTX5_IV0 - Lowest 32 bits of IV for region 5. */
#define FLEXSPI_IPEDCTX5IV0_CTX5_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5IV0_CTX5_IV0_SHIFT)) & FLEXSPI_IPEDCTX5IV0_CTX5_IV0_MASK)
/*! @} */

/*! @name IPEDCTX5IV1 - IPED context5 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX5IV1_CTX5_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX5IV1_CTX5_IV1_SHIFT       (0U)
/*! CTX5_IV1 - Highest 32 bits of IV for region 5. */
#define FLEXSPI_IPEDCTX5IV1_CTX5_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5IV1_CTX5_IV1_SHIFT)) & FLEXSPI_IPEDCTX5IV1_CTX5_IV1_MASK)
/*! @} */

/*! @name IPEDCTX5START - Start address of region 5 */
/*! @{ */

#define FLEXSPI_IPEDCTX5START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX5START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX5START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5START_GCM_SHIFT)) & FLEXSPI_IPEDCTX5START_GCM_MASK)

#define FLEXSPI_IPEDCTX5START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX5START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX5START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX5START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX5START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX5START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 5. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX5START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX5START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX5END - End address of region 5 */
/*! @{ */

#define FLEXSPI_IPEDCTX5END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX5END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 5. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX5END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX5END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX5AAD0 - IPED context5 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX5AAD0_CTX5_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX5AAD0_CTX5_AAD0_SHIFT     (0U)
/*! CTX5_AAD0 - Lowest 32 bits of AAD for region 5. */
#define FLEXSPI_IPEDCTX5AAD0_CTX5_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5AAD0_CTX5_AAD0_SHIFT)) & FLEXSPI_IPEDCTX5AAD0_CTX5_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX5AAD1 - IPED context5 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX5AAD1_CTX5_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX5AAD1_CTX5_AAD1_SHIFT     (0U)
/*! CTX5_AAD1 - Highest 32 bits of AAD for region 5. */
#define FLEXSPI_IPEDCTX5AAD1_CTX5_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX5AAD1_CTX5_AAD1_SHIFT)) & FLEXSPI_IPEDCTX5AAD1_CTX5_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX6IV0 - IPED context6 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX6IV0_CTX6_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX6IV0_CTX6_IV0_SHIFT       (0U)
/*! CTX6_IV0 - Lowest 32 bits of IV for region 6. */
#define FLEXSPI_IPEDCTX6IV0_CTX6_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6IV0_CTX6_IV0_SHIFT)) & FLEXSPI_IPEDCTX6IV0_CTX6_IV0_MASK)
/*! @} */

/*! @name IPEDCTX6IV1 - IPED context6 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX6IV1_CTX6_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX6IV1_CTX6_IV1_SHIFT       (0U)
/*! CTX6_IV1 - Highest 32 bits of IV for region 6. */
#define FLEXSPI_IPEDCTX6IV1_CTX6_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6IV1_CTX6_IV1_SHIFT)) & FLEXSPI_IPEDCTX6IV1_CTX6_IV1_MASK)
/*! @} */

/*! @name IPEDCTX6START - Start address of region 6 */
/*! @{ */

#define FLEXSPI_IPEDCTX6START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX6START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX6START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6START_GCM_SHIFT)) & FLEXSPI_IPEDCTX6START_GCM_MASK)

#define FLEXSPI_IPEDCTX6START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX6START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX6START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX6START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX6START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX6START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 6. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX6START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX6START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX6END - End address of region 6 */
/*! @{ */

#define FLEXSPI_IPEDCTX6END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX6END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 6. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX6END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX6END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX6AAD0 - IPED context6 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX6AAD0_CTX6_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX6AAD0_CTX6_AAD0_SHIFT     (0U)
/*! CTX6_AAD0 - Lowest 32 bits of AAD for region 6. */
#define FLEXSPI_IPEDCTX6AAD0_CTX6_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6AAD0_CTX6_AAD0_SHIFT)) & FLEXSPI_IPEDCTX6AAD0_CTX6_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX6AAD1 - IPED context6 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX6AAD1_CTX6_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX6AAD1_CTX6_AAD1_SHIFT     (0U)
/*! CTX6_AAD1 - Highest 32 bits of AAD for region 6. */
#define FLEXSPI_IPEDCTX6AAD1_CTX6_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX6AAD1_CTX6_AAD1_SHIFT)) & FLEXSPI_IPEDCTX6AAD1_CTX6_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX7IV0 - IPED context7 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX7IV0_CTX7_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX7IV0_CTX7_IV0_SHIFT       (0U)
/*! CTX7_IV0 - Lowest 32 bits of IV for region 7. */
#define FLEXSPI_IPEDCTX7IV0_CTX7_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7IV0_CTX7_IV0_SHIFT)) & FLEXSPI_IPEDCTX7IV0_CTX7_IV0_MASK)
/*! @} */

/*! @name IPEDCTX7IV1 - IPED context7 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX7IV1_CTX7_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX7IV1_CTX7_IV1_SHIFT       (0U)
/*! CTX7_IV1 - Highest 32 bits of IV for region 7. */
#define FLEXSPI_IPEDCTX7IV1_CTX7_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7IV1_CTX7_IV1_SHIFT)) & FLEXSPI_IPEDCTX7IV1_CTX7_IV1_MASK)
/*! @} */

/*! @name IPEDCTX7START - Start address of region 7 */
/*! @{ */

#define FLEXSPI_IPEDCTX7START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX7START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX7START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7START_GCM_SHIFT)) & FLEXSPI_IPEDCTX7START_GCM_MASK)

#define FLEXSPI_IPEDCTX7START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX7START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX7START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX7START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX7START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX7START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 7. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX7START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX7START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX7END - End address of region 7 */
/*! @{ */

#define FLEXSPI_IPEDCTX7END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX7END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 7. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX7END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX7END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX7AAD0 - IPED context7 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX7AAD0_CTX7_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX7AAD0_CTX7_AAD0_SHIFT     (0U)
/*! CTX7_AAD0 - Lowest 32 bits of AAD for region 7. */
#define FLEXSPI_IPEDCTX7AAD0_CTX7_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7AAD0_CTX7_AAD0_SHIFT)) & FLEXSPI_IPEDCTX7AAD0_CTX7_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX7AAD1 - IPED context7 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX7AAD1_CTX7_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX7AAD1_CTX7_AAD1_SHIFT     (0U)
/*! CTX7_AAD1 - Highest 32 bits of AAD for region 7. */
#define FLEXSPI_IPEDCTX7AAD1_CTX7_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX7AAD1_CTX7_AAD1_SHIFT)) & FLEXSPI_IPEDCTX7AAD1_CTX7_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX8IV0 - IPED context8 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX8IV0_CTX8_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX8IV0_CTX8_IV0_SHIFT       (0U)
/*! CTX8_IV0 - Lowest 32 bits of IV for region 8. */
#define FLEXSPI_IPEDCTX8IV0_CTX8_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8IV0_CTX8_IV0_SHIFT)) & FLEXSPI_IPEDCTX8IV0_CTX8_IV0_MASK)
/*! @} */

/*! @name IPEDCTX8IV1 - IPED context8 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX8IV1_CTX8_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX8IV1_CTX8_IV1_SHIFT       (0U)
/*! CTX8_IV1 - Highest 32 bits of IV for region 8. */
#define FLEXSPI_IPEDCTX8IV1_CTX8_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8IV1_CTX8_IV1_SHIFT)) & FLEXSPI_IPEDCTX8IV1_CTX8_IV1_MASK)
/*! @} */

/*! @name IPEDCTX8START - Start address of region 8 */
/*! @{ */

#define FLEXSPI_IPEDCTX8START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX8START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX8START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8START_GCM_SHIFT)) & FLEXSPI_IPEDCTX8START_GCM_MASK)

#define FLEXSPI_IPEDCTX8START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX8START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX8START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX8START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX8START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX8START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 8. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX8START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX8START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX8END - End address of region 8 */
/*! @{ */

#define FLEXSPI_IPEDCTX8END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX8END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 8. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX8END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX8END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX8AAD0 - IPED context8 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX8AAD0_CTX8_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX8AAD0_CTX8_AAD0_SHIFT     (0U)
/*! CTX8_AAD0 - Lowest 32 bits of AAD for region 8. */
#define FLEXSPI_IPEDCTX8AAD0_CTX8_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8AAD0_CTX8_AAD0_SHIFT)) & FLEXSPI_IPEDCTX8AAD0_CTX8_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX8AAD1 - IPED context8 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX8AAD1_CTX8_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX8AAD1_CTX8_AAD1_SHIFT     (0U)
/*! CTX8_AAD1 - Highest 32 bits of AAD for region 8. */
#define FLEXSPI_IPEDCTX8AAD1_CTX8_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX8AAD1_CTX8_AAD1_SHIFT)) & FLEXSPI_IPEDCTX8AAD1_CTX8_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX9IV0 - IPED context9 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX9IV0_CTX9_IV0_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX9IV0_CTX9_IV0_SHIFT       (0U)
/*! CTX9_IV0 - Lowest 32 bits of IV for region 9. */
#define FLEXSPI_IPEDCTX9IV0_CTX9_IV0(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9IV0_CTX9_IV0_SHIFT)) & FLEXSPI_IPEDCTX9IV0_CTX9_IV0_MASK)
/*! @} */

/*! @name IPEDCTX9IV1 - IPED context9 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX9IV1_CTX9_IV1_MASK        (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX9IV1_CTX9_IV1_SHIFT       (0U)
/*! CTX9_IV1 - Highest 32 bits of IV for region 9. */
#define FLEXSPI_IPEDCTX9IV1_CTX9_IV1(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9IV1_CTX9_IV1_SHIFT)) & FLEXSPI_IPEDCTX9IV1_CTX9_IV1_MASK)
/*! @} */

/*! @name IPEDCTX9START - Start address of region 9 */
/*! @{ */

#define FLEXSPI_IPEDCTX9START_GCM_MASK           (0x1U)
#define FLEXSPI_IPEDCTX9START_GCM_SHIFT          (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX9START_GCM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9START_GCM_SHIFT)) & FLEXSPI_IPEDCTX9START_GCM_MASK)

#define FLEXSPI_IPEDCTX9START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX9START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX9START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX9START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX9START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX9START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 9. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX9START_START_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX9START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX9END - End address of region 9 */
/*! @{ */

#define FLEXSPI_IPEDCTX9END_END_ADDRESS_MASK     (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX9END_END_ADDRESS_SHIFT    (8U)
/*! END_ADDRESS - End address of region 9. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX9END_END_ADDRESS(x)       (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX9END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX9AAD0 - IPED context9 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX9AAD0_CTX9_AAD0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX9AAD0_CTX9_AAD0_SHIFT     (0U)
/*! CTX9_AAD0 - Lowest 32 bits of AAD for region 9. */
#define FLEXSPI_IPEDCTX9AAD0_CTX9_AAD0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9AAD0_CTX9_AAD0_SHIFT)) & FLEXSPI_IPEDCTX9AAD0_CTX9_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX9AAD1 - IPED context9 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX9AAD1_CTX9_AAD1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX9AAD1_CTX9_AAD1_SHIFT     (0U)
/*! CTX9_AAD1 - Highest 32 bits of AAD for region 9. */
#define FLEXSPI_IPEDCTX9AAD1_CTX9_AAD1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX9AAD1_CTX9_AAD1_SHIFT)) & FLEXSPI_IPEDCTX9AAD1_CTX9_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX10IV0 - IPED context10 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX10IV0_CTX10_IV0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX10IV0_CTX10_IV0_SHIFT     (0U)
/*! CTX10_IV0 - Lowest 32 bits of IV for region 10. */
#define FLEXSPI_IPEDCTX10IV0_CTX10_IV0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10IV0_CTX10_IV0_SHIFT)) & FLEXSPI_IPEDCTX10IV0_CTX10_IV0_MASK)
/*! @} */

/*! @name IPEDCTX10IV1 - IPED context10 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX10IV1_CTX10_IV1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX10IV1_CTX10_IV1_SHIFT     (0U)
/*! CTX10_IV1 - Highest 32 bits of IV for region 10. */
#define FLEXSPI_IPEDCTX10IV1_CTX10_IV1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10IV1_CTX10_IV1_SHIFT)) & FLEXSPI_IPEDCTX10IV1_CTX10_IV1_MASK)
/*! @} */

/*! @name IPEDCTX10START - Start address of region 10 */
/*! @{ */

#define FLEXSPI_IPEDCTX10START_GCM_MASK          (0x1U)
#define FLEXSPI_IPEDCTX10START_GCM_SHIFT         (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX10START_GCM(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10START_GCM_SHIFT)) & FLEXSPI_IPEDCTX10START_GCM_MASK)

#define FLEXSPI_IPEDCTX10START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX10START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX10START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX10START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX10START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX10START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 10. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX10START_START_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX10START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX10END - End address of region 10 */
/*! @{ */

#define FLEXSPI_IPEDCTX10END_END_ADDRESS_MASK    (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX10END_END_ADDRESS_SHIFT   (8U)
/*! END_ADDRESS - End address of region 10. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX10END_END_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX10END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX10AAD0 - IPED context10 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX10AAD0_CTX10_AAD0_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX10AAD0_CTX10_AAD0_SHIFT   (0U)
/*! CTX10_AAD0 - Lowest 32 bits of AAD for region 10. */
#define FLEXSPI_IPEDCTX10AAD0_CTX10_AAD0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10AAD0_CTX10_AAD0_SHIFT)) & FLEXSPI_IPEDCTX10AAD0_CTX10_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX10AAD1 - IPED context10 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX10AAD1_CTX10_AAD1_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX10AAD1_CTX10_AAD1_SHIFT   (0U)
/*! CTX10_AAD1 - Highest 32 bits of AAD for region 10. */
#define FLEXSPI_IPEDCTX10AAD1_CTX10_AAD1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX10AAD1_CTX10_AAD1_SHIFT)) & FLEXSPI_IPEDCTX10AAD1_CTX10_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX11IV0 - IPED context11 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX11IV0_CTX11_IV0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX11IV0_CTX11_IV0_SHIFT     (0U)
/*! CTX11_IV0 - Lowest 32 bits of IV for region 11. */
#define FLEXSPI_IPEDCTX11IV0_CTX11_IV0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11IV0_CTX11_IV0_SHIFT)) & FLEXSPI_IPEDCTX11IV0_CTX11_IV0_MASK)
/*! @} */

/*! @name IPEDCTX11IV1 - IPED context11 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX11IV1_CTX11_IV1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX11IV1_CTX11_IV1_SHIFT     (0U)
/*! CTX11_IV1 - Highest 32 bits of IV for region 11. */
#define FLEXSPI_IPEDCTX11IV1_CTX11_IV1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11IV1_CTX11_IV1_SHIFT)) & FLEXSPI_IPEDCTX11IV1_CTX11_IV1_MASK)
/*! @} */

/*! @name IPEDCTX11START - Start address of region 11 */
/*! @{ */

#define FLEXSPI_IPEDCTX11START_GCM_MASK          (0x1U)
#define FLEXSPI_IPEDCTX11START_GCM_SHIFT         (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX11START_GCM(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11START_GCM_SHIFT)) & FLEXSPI_IPEDCTX11START_GCM_MASK)

#define FLEXSPI_IPEDCTX11START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX11START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX11START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX11START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX11START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX11START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 11. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX11START_START_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX11START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX11END - End address of region 11 */
/*! @{ */

#define FLEXSPI_IPEDCTX11END_END_ADDRESS_MASK    (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX11END_END_ADDRESS_SHIFT   (8U)
/*! END_ADDRESS - End address of region 11. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX11END_END_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX11END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX11AAD0 - IPED context11 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX11AAD0_CTX11_AAD0_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX11AAD0_CTX11_AAD0_SHIFT   (0U)
/*! CTX11_AAD0 - Lowest 32 bits of AAD for region 11. */
#define FLEXSPI_IPEDCTX11AAD0_CTX11_AAD0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11AAD0_CTX11_AAD0_SHIFT)) & FLEXSPI_IPEDCTX11AAD0_CTX11_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX11AAD1 - IPED context11 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX11AAD1_CTX11_AAD1_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX11AAD1_CTX11_AAD1_SHIFT   (0U)
/*! CTX11_AAD1 - Highest 32 bits of AAD for region 11. */
#define FLEXSPI_IPEDCTX11AAD1_CTX11_AAD1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX11AAD1_CTX11_AAD1_SHIFT)) & FLEXSPI_IPEDCTX11AAD1_CTX11_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX12IV0 - IPED context12 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX12IV0_CTX12_IV0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX12IV0_CTX12_IV0_SHIFT     (0U)
/*! CTX12_IV0 - Lowest 32 bits of IV for region 12. */
#define FLEXSPI_IPEDCTX12IV0_CTX12_IV0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12IV0_CTX12_IV0_SHIFT)) & FLEXSPI_IPEDCTX12IV0_CTX12_IV0_MASK)
/*! @} */

/*! @name IPEDCTX12IV1 - IPED context12 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX12IV1_CTX12_IV1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX12IV1_CTX12_IV1_SHIFT     (0U)
/*! CTX12_IV1 - Highest 32 bits of IV for region 12. */
#define FLEXSPI_IPEDCTX12IV1_CTX12_IV1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12IV1_CTX12_IV1_SHIFT)) & FLEXSPI_IPEDCTX12IV1_CTX12_IV1_MASK)
/*! @} */

/*! @name IPEDCTX12START - Start address of region 12 */
/*! @{ */

#define FLEXSPI_IPEDCTX12START_GCM_MASK          (0x1U)
#define FLEXSPI_IPEDCTX12START_GCM_SHIFT         (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX12START_GCM(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12START_GCM_SHIFT)) & FLEXSPI_IPEDCTX12START_GCM_MASK)

#define FLEXSPI_IPEDCTX12START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX12START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX12START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX12START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX12START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX12START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 12. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX12START_START_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX12START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX12END - End address of region 12 */
/*! @{ */

#define FLEXSPI_IPEDCTX12END_END_ADDRESS_MASK    (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX12END_END_ADDRESS_SHIFT   (8U)
/*! END_ADDRESS - End address of region 12. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX12END_END_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX12END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX12AAD0 - IPED context12 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX12AAD0_CTX12_AAD0_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX12AAD0_CTX12_AAD0_SHIFT   (0U)
/*! CTX12_AAD0 - Lowest 32 bits of AAD for region 12. */
#define FLEXSPI_IPEDCTX12AAD0_CTX12_AAD0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12AAD0_CTX12_AAD0_SHIFT)) & FLEXSPI_IPEDCTX12AAD0_CTX12_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX12AAD1 - IPED context12 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX12AAD1_CTX12_AAD1_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX12AAD1_CTX12_AAD1_SHIFT   (0U)
/*! CTX12_AAD1 - Highest 32 bits of AAD for region 12. */
#define FLEXSPI_IPEDCTX12AAD1_CTX12_AAD1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX12AAD1_CTX12_AAD1_SHIFT)) & FLEXSPI_IPEDCTX12AAD1_CTX12_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX13IV0 - IPED context13 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX13IV0_CTX13_IV0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX13IV0_CTX13_IV0_SHIFT     (0U)
/*! CTX13_IV0 - Lowest 32 bits of IV for region 13. */
#define FLEXSPI_IPEDCTX13IV0_CTX13_IV0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13IV0_CTX13_IV0_SHIFT)) & FLEXSPI_IPEDCTX13IV0_CTX13_IV0_MASK)
/*! @} */

/*! @name IPEDCTX13IV1 - IPED context13 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX13IV1_CTX13_IV1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX13IV1_CTX13_IV1_SHIFT     (0U)
/*! CTX13_IV1 - Highest 32 bits of IV for region 13. */
#define FLEXSPI_IPEDCTX13IV1_CTX13_IV1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13IV1_CTX13_IV1_SHIFT)) & FLEXSPI_IPEDCTX13IV1_CTX13_IV1_MASK)
/*! @} */

/*! @name IPEDCTX13START - Start address of region 13 */
/*! @{ */

#define FLEXSPI_IPEDCTX13START_GCM_MASK          (0x1U)
#define FLEXSPI_IPEDCTX13START_GCM_SHIFT         (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX13START_GCM(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13START_GCM_SHIFT)) & FLEXSPI_IPEDCTX13START_GCM_MASK)

#define FLEXSPI_IPEDCTX13START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX13START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX13START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX13START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX13START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX13START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 13. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX13START_START_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX13START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX13END - End address of region 13 */
/*! @{ */

#define FLEXSPI_IPEDCTX13END_END_ADDRESS_MASK    (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX13END_END_ADDRESS_SHIFT   (8U)
/*! END_ADDRESS - End address of region 13. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX13END_END_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX13END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX13AAD0 - IPED context13 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX13AAD0_CTX13_AAD0_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX13AAD0_CTX13_AAD0_SHIFT   (0U)
/*! CTX13_AAD0 - Lowest 32 bits of AAD for region 13. */
#define FLEXSPI_IPEDCTX13AAD0_CTX13_AAD0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13AAD0_CTX13_AAD0_SHIFT)) & FLEXSPI_IPEDCTX13AAD0_CTX13_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX13AAD1 - IPED context13 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX13AAD1_CTX13_AAD1_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX13AAD1_CTX13_AAD1_SHIFT   (0U)
/*! CTX13_AAD1 - Highest 32 bits of AAD for region 13. */
#define FLEXSPI_IPEDCTX13AAD1_CTX13_AAD1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX13AAD1_CTX13_AAD1_SHIFT)) & FLEXSPI_IPEDCTX13AAD1_CTX13_AAD1_MASK)
/*! @} */

/*! @name IPEDCTX14IV0 - IPED context14 IV0 */
/*! @{ */

#define FLEXSPI_IPEDCTX14IV0_CTX14_IV0_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX14IV0_CTX14_IV0_SHIFT     (0U)
/*! CTX14_IV0 - Lowest 32 bits of IV for region 14. */
#define FLEXSPI_IPEDCTX14IV0_CTX14_IV0(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14IV0_CTX14_IV0_SHIFT)) & FLEXSPI_IPEDCTX14IV0_CTX14_IV0_MASK)
/*! @} */

/*! @name IPEDCTX14IV1 - IPED context14 IV1 */
/*! @{ */

#define FLEXSPI_IPEDCTX14IV1_CTX14_IV1_MASK      (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX14IV1_CTX14_IV1_SHIFT     (0U)
/*! CTX14_IV1 - Highest 32 bits of IV for region 14. */
#define FLEXSPI_IPEDCTX14IV1_CTX14_IV1(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14IV1_CTX14_IV1_SHIFT)) & FLEXSPI_IPEDCTX14IV1_CTX14_IV1_MASK)
/*! @} */

/*! @name IPEDCTX14START - Start address of region 14 */
/*! @{ */

#define FLEXSPI_IPEDCTX14START_GCM_MASK          (0x1U)
#define FLEXSPI_IPEDCTX14START_GCM_SHIFT         (0U)
/*! GCM - If this bit is 1, current region is GCM mode region. */
#define FLEXSPI_IPEDCTX14START_GCM(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14START_GCM_SHIFT)) & FLEXSPI_IPEDCTX14START_GCM_MASK)

#define FLEXSPI_IPEDCTX14START_AHBBUSERROR_DIS_MASK (0x2U)
#define FLEXSPI_IPEDCTX14START_AHBBUSERROR_DIS_SHIFT (1U)
/*! AHBBUSERROR_DIS - If this bit is 1, ahb bus error is disable. */
#define FLEXSPI_IPEDCTX14START_AHBBUSERROR_DIS(x) (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14START_AHBBUSERROR_DIS_SHIFT)) & FLEXSPI_IPEDCTX14START_AHBBUSERROR_DIS_MASK)

#define FLEXSPI_IPEDCTX14START_START_ADDRESS_MASK (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX14START_START_ADDRESS_SHIFT (8U)
/*! START_ADDRESS - Start address of region 14. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX14START_START_ADDRESS(x)  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14START_START_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX14START_START_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX14END - End address of region 14 */
/*! @{ */

#define FLEXSPI_IPEDCTX14END_END_ADDRESS_MASK    (0xFFFFFF00U)
#define FLEXSPI_IPEDCTX14END_END_ADDRESS_SHIFT   (8U)
/*! END_ADDRESS - End address of region 14. Minimal 256 Bytes aligned. It is system address. */
#define FLEXSPI_IPEDCTX14END_END_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14END_END_ADDRESS_SHIFT)) & FLEXSPI_IPEDCTX14END_END_ADDRESS_MASK)
/*! @} */

/*! @name IPEDCTX14AAD0 - IPED context14 AAD0 */
/*! @{ */

#define FLEXSPI_IPEDCTX14AAD0_CTX14_AAD0_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX14AAD0_CTX14_AAD0_SHIFT   (0U)
/*! CTX14_AAD0 - Lowest 32 bits of AAD for region 14. */
#define FLEXSPI_IPEDCTX14AAD0_CTX14_AAD0(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14AAD0_CTX14_AAD0_SHIFT)) & FLEXSPI_IPEDCTX14AAD0_CTX14_AAD0_MASK)
/*! @} */

/*! @name IPEDCTX14AAD1 - IPED context14 AAD1 */
/*! @{ */

#define FLEXSPI_IPEDCTX14AAD1_CTX14_AAD1_MASK    (0xFFFFFFFFU)
#define FLEXSPI_IPEDCTX14AAD1_CTX14_AAD1_SHIFT   (0U)
/*! CTX14_AAD1 - Highest 32 bits of AAD for region 14. */
#define FLEXSPI_IPEDCTX14AAD1_CTX14_AAD1(x)      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPEDCTX14AAD1_CTX14_AAD1_SHIFT)) & FLEXSPI_IPEDCTX14AAD1_CTX14_AAD1_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group FLEXSPI_Register_Masks */


/*!
 * @}
 */ /* end of group FLEXSPI_Peripheral_Access_Layer */


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


#endif  /* FLEXSPI_H_ */

