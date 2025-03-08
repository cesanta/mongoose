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
**         CMSIS Peripheral Access Layer for LCDIC
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
 * @file LCDIC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for LCDIC
 *
 * CMSIS Peripheral Access Layer for LCDIC
 */

#if !defined(LCDIC_H_)
#define LCDIC_H_                                 /**< Symbol preventing repeated inclusion */

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
   -- LCDIC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCDIC_Peripheral_Access_Layer LCDIC Peripheral Access Layer
 * @{
 */

/** LCDIC - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[8];
  __IO uint32_t CTRL;                              /**< LCDIC Control Register, offset: 0x8 */
  __IO uint32_t FIFO_CTRL;                         /**< FIFO Control Register, offset: 0xC */
  __IO uint32_t TIMER_CTRL;                        /**< Timer Control Register, offset: 0x10 */
  __IO uint32_t RST_CTRL;                          /**< Reset Control Register, offset: 0x14 */
  __IO uint32_t I8080_CTRL0;                       /**< I8080 Control0 Register, offset: 0x18 */
  __IO uint32_t I8080_CTRL1;                       /**< I8080 Control1 Reigster, offset: 0x1C */
  __IO uint32_t SPI_CTRL;                          /**< SPI Control, offset: 0x20 */
  __IO uint32_t TE_CTRL;                           /**< Tearing Effect Control, offset: 0x24 */
  __IO uint32_t TO_CTRL;                           /**< Baseline Control Register 0, offset: 0x28 */
  __O  uint32_t TFIFO_WDATA;                       /**< Baseline Control Register 1, offset: 0x2C */
  __I  uint32_t RFIFO_RDATA;                       /**< Baseline Control Register 2, offset: 0x30 */
  __I  uint32_t ISR;                               /**< Interrupt Status Register, offset: 0x34 */
  __I  uint32_t IRSR;                              /**< Interrupt Raw Status Register, offset: 0x38 */
  __IO uint32_t ICR;                               /**< Interrupt Clear Register, offset: 0x3C */
  __IO uint32_t IMR;                               /**< Touch Detection Control Register2, offset: 0x40 */
  __I  uint32_t STATUS0;                           /**< Status Register, offset: 0x44 */
  __I  uint32_t STATUS1;                           /**< Touch Detection Control Register4, offset: 0x48 */
} LCDIC_Type;

/* ----------------------------------------------------------------------------
   -- LCDIC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCDIC_Register_Masks LCDIC Register Masks
 * @{
 */

/*! @name CTRL - LCDIC Control Register */
/*! @{ */

#define LCDIC_CTRL_LCDIC_EN_MASK                 (0x1U)
#define LCDIC_CTRL_LCDIC_EN_SHIFT                (0U)
/*! LCDIC_EN - LCDIC enable. */
#define LCDIC_CTRL_LCDIC_EN(x)                   (((uint32_t)(((uint32_t)(x)) << LCDIC_CTRL_LCDIC_EN_SHIFT)) & LCDIC_CTRL_LCDIC_EN_MASK)

#define LCDIC_CTRL_LCDIC_MD_MASK                 (0x2U)
#define LCDIC_CTRL_LCDIC_MD_SHIFT                (1U)
/*! LCDIC_MD - LCDIC mode. */
#define LCDIC_CTRL_LCDIC_MD(x)                   (((uint32_t)(((uint32_t)(x)) << LCDIC_CTRL_LCDIC_MD_SHIFT)) & LCDIC_CTRL_LCDIC_MD_MASK)

#define LCDIC_CTRL_SPI_MD_MASK                   (0x4U)
#define LCDIC_CTRL_SPI_MD_SHIFT                  (2U)
/*! SPI_MD - SPI mode. Only valid when lcdic_md = 1'd0. */
#define LCDIC_CTRL_SPI_MD(x)                     (((uint32_t)(((uint32_t)(x)) << LCDIC_CTRL_SPI_MD_SHIFT)) & LCDIC_CTRL_SPI_MD_MASK)

#define LCDIC_CTRL_DMA_EN_MASK                   (0x8U)
#define LCDIC_CTRL_DMA_EN_SHIFT                  (3U)
/*! DMA_EN - DMA enable. */
#define LCDIC_CTRL_DMA_EN(x)                     (((uint32_t)(((uint32_t)(x)) << LCDIC_CTRL_DMA_EN_SHIFT)) & LCDIC_CTRL_DMA_EN_MASK)

#define LCDIC_CTRL_DAT_ENDIAN_MASK               (0x10U)
#define LCDIC_CTRL_DAT_ENDIAN_SHIFT              (4U)
/*! DAT_ENDIAN - Byte data endian. */
#define LCDIC_CTRL_DAT_ENDIAN(x)                 (((uint32_t)(((uint32_t)(x)) << LCDIC_CTRL_DAT_ENDIAN_SHIFT)) & LCDIC_CTRL_DAT_ENDIAN_MASK)
/*! @} */

/*! @name FIFO_CTRL - FIFO Control Register */
/*! @{ */

#define LCDIC_FIFO_CTRL_TFIFO_THRES_MASK         (0x7U)
#define LCDIC_FIFO_CTRL_TFIFO_THRES_SHIFT        (0U)
/*! TFIFO_THRES - TX FIFO threshold. */
#define LCDIC_FIFO_CTRL_TFIFO_THRES(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_FIFO_CTRL_TFIFO_THRES_SHIFT)) & LCDIC_FIFO_CTRL_TFIFO_THRES_MASK)

#define LCDIC_FIFO_CTRL_RFIFO_THRES_MASK         (0x8U)
#define LCDIC_FIFO_CTRL_RFIFO_THRES_SHIFT        (3U)
/*! RFIFO_THRES - RX FIFO threshold. */
#define LCDIC_FIFO_CTRL_RFIFO_THRES(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_FIFO_CTRL_RFIFO_THRES_SHIFT)) & LCDIC_FIFO_CTRL_RFIFO_THRES_MASK)
/*! @} */

/*! @name TIMER_CTRL - Timer Control Register */
/*! @{ */

#define LCDIC_TIMER_CTRL_TIMER_RATIO0_MASK       (0xFU)
#define LCDIC_TIMER_CTRL_TIMER_RATIO0_SHIFT      (0U)
/*! TIMER_RATIO0 - Timer ratio0. */
#define LCDIC_TIMER_CTRL_TIMER_RATIO0(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_TIMER_CTRL_TIMER_RATIO0_SHIFT)) & LCDIC_TIMER_CTRL_TIMER_RATIO0_MASK)

#define LCDIC_TIMER_CTRL_TIMER_RATIO1_MASK       (0xF0U)
#define LCDIC_TIMER_CTRL_TIMER_RATIO1_SHIFT      (4U)
/*! TIMER_RATIO1 - Timer ratio1. */
#define LCDIC_TIMER_CTRL_TIMER_RATIO1(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_TIMER_CTRL_TIMER_RATIO1_SHIFT)) & LCDIC_TIMER_CTRL_TIMER_RATIO1_MASK)
/*! @} */

/*! @name RST_CTRL - Reset Control Register */
/*! @{ */

#define LCDIC_RST_CTRL_RST_START_MASK            (0x1U)
#define LCDIC_RST_CTRL_RST_START_SHIFT           (0U)
/*! RST_START - LCD reset start signal. Single pulse. */
#define LCDIC_RST_CTRL_RST_START(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_RST_CTRL_RST_START_SHIFT)) & LCDIC_RST_CTRL_RST_START_MASK)

#define LCDIC_RST_CTRL_RST_POL_MASK              (0x2U)
#define LCDIC_RST_CTRL_RST_POL_SHIFT             (1U)
/*! RST_POL - Reset signal polarity. */
#define LCDIC_RST_CTRL_RST_POL(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_RST_CTRL_RST_POL_SHIFT)) & LCDIC_RST_CTRL_RST_POL_MASK)

#define LCDIC_RST_CTRL_RST_SEQ_NUM_MASK          (0x1CU)
#define LCDIC_RST_CTRL_RST_SEQ_NUM_SHIFT         (2U)
/*! RST_SEQ_NUM - Reset sequence pulse number. 3'd0: 1 pulse; 3'd7: 8 pulse. */
#define LCDIC_RST_CTRL_RST_SEQ_NUM(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_RST_CTRL_RST_SEQ_NUM_SHIFT)) & LCDIC_RST_CTRL_RST_SEQ_NUM_MASK)

#define LCDIC_RST_CTRL_RST_SEQ_MASK              (0x1FE0U)
#define LCDIC_RST_CTRL_RST_SEQ_SHIFT             (5U)
/*! RST_SEQ - Reset sequence. LCD_RST will send rst_seq[0] onto lcd_reset first, and then followed
 *    by rst_seq[1] until rst_seq_num is reached.
 */
#define LCDIC_RST_CTRL_RST_SEQ(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_RST_CTRL_RST_SEQ_SHIFT)) & LCDIC_RST_CTRL_RST_SEQ_MASK)

#define LCDIC_RST_CTRL_RST_WIDTH_MASK            (0x7E000U)
#define LCDIC_RST_CTRL_RST_WIDTH_SHIFT           (13U)
/*! RST_WIDTH - Width of each reset pulse. Unit is T(timer_base0). T(reset pulse) = T(timer_base0)*(rst_width+1). */
#define LCDIC_RST_CTRL_RST_WIDTH(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_RST_CTRL_RST_WIDTH_SHIFT)) & LCDIC_RST_CTRL_RST_WIDTH_MASK)
/*! @} */

/*! @name I8080_CTRL0 - I8080 Control0 Register */
/*! @{ */

#define LCDIC_I8080_CTRL0_CS_POL_MASK            (0x1U)
#define LCDIC_I8080_CTRL0_CS_POL_SHIFT           (0U)
/*! CS_POL - CS polarity. */
#define LCDIC_I8080_CTRL0_CS_POL(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_CS_POL_SHIFT)) & LCDIC_I8080_CTRL0_CS_POL_MASK)

#define LCDIC_I8080_CTRL0_DC_POL_MASK            (0x2U)
#define LCDIC_I8080_CTRL0_DC_POL_SHIFT           (1U)
/*! DC_POL - DC polarity. */
#define LCDIC_I8080_CTRL0_DC_POL(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_DC_POL_SHIFT)) & LCDIC_I8080_CTRL0_DC_POL_MASK)

#define LCDIC_I8080_CTRL0_RD_POL_MASK            (0x4U)
#define LCDIC_I8080_CTRL0_RD_POL_SHIFT           (2U)
/*! RD_POL - RD polarity. */
#define LCDIC_I8080_CTRL0_RD_POL(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_RD_POL_SHIFT)) & LCDIC_I8080_CTRL0_RD_POL_MASK)

#define LCDIC_I8080_CTRL0_WR_POL_MASK            (0x8U)
#define LCDIC_I8080_CTRL0_WR_POL_SHIFT           (3U)
/*! WR_POL - WR polarity. */
#define LCDIC_I8080_CTRL0_WR_POL(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_WR_POL_SHIFT)) & LCDIC_I8080_CTRL0_WR_POL_MASK)

#define LCDIC_I8080_CTRL0_EN_DC_OFF_MASK         (0x10U)
#define LCDIC_I8080_CTRL0_EN_DC_OFF_SHIFT        (4U)
/*! EN_DC_OFF - CS off while DC switches. */
#define LCDIC_I8080_CTRL0_EN_DC_OFF(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_EN_DC_OFF_SHIFT)) & LCDIC_I8080_CTRL0_EN_DC_OFF_MASK)

#define LCDIC_I8080_CTRL0_EN_IDLE_OFF_MASK       (0x20U)
#define LCDIC_I8080_CTRL0_EN_IDLE_OFF_SHIFT      (5U)
/*! EN_IDLE_OFF - CS off while no transmission. */
#define LCDIC_I8080_CTRL0_EN_IDLE_OFF(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_EN_IDLE_OFF_SHIFT)) & LCDIC_I8080_CTRL0_EN_IDLE_OFF_MASK)

#define LCDIC_I8080_CTRL0_TCSW_MASK              (0x1C0U)
#define LCDIC_I8080_CTRL0_TCSW_SHIFT             (6U)
/*! TCSW - CS wait time. Minimum CS inactive pulse width. T(csw)=T(lcdic_clk)*tcsw. */
#define LCDIC_I8080_CTRL0_TCSW(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TCSW_SHIFT)) & LCDIC_I8080_CTRL0_TCSW_MASK)

#define LCDIC_I8080_CTRL0_TCSS_MASK              (0x1FE00U)
#define LCDIC_I8080_CTRL0_TCSS_SHIFT             (9U)
/*! TCSS - CS setup time. Minimum CS setup time before WR/RD. T(css)=T(lcdic_clk)*tcss. */
#define LCDIC_I8080_CTRL0_TCSS(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TCSS_SHIFT)) & LCDIC_I8080_CTRL0_TCSS_MASK)

#define LCDIC_I8080_CTRL0_TCSH_MASK              (0xE0000U)
#define LCDIC_I8080_CTRL0_TCSH_SHIFT             (17U)
/*! TCSH - CS hold time. Minimum CS hold time after WR/RD. T(csh)=T(lcdic_clk)*tcsh. */
#define LCDIC_I8080_CTRL0_TCSH(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TCSH_SHIFT)) & LCDIC_I8080_CTRL0_TCSH_MASK)

#define LCDIC_I8080_CTRL0_TDCS_MASK              (0x700000U)
#define LCDIC_I8080_CTRL0_TDCS_SHIFT             (20U)
/*! TDCS - DC setupt time. Minimum DC setup time before WR/RD/CS. T(dcs)=T(lcdic_clk)*tdcs. */
#define LCDIC_I8080_CTRL0_TDCS(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TDCS_SHIFT)) & LCDIC_I8080_CTRL0_TDCS_MASK)

#define LCDIC_I8080_CTRL0_TDCH_MASK              (0x3800000U)
#define LCDIC_I8080_CTRL0_TDCH_SHIFT             (23U)
/*! TDCH - DC hold time. Minimum DC hold time after WR/RD/CS. T(dch)=T(lcdic_clk)*tdch. */
#define LCDIC_I8080_CTRL0_TDCH(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TDCH_SHIFT)) & LCDIC_I8080_CTRL0_TDCH_MASK)

#define LCDIC_I8080_CTRL0_TWDS_MASK              (0x1C000000U)
#define LCDIC_I8080_CTRL0_TWDS_SHIFT             (26U)
/*! TWDS - Write data setup time. Minimum write data setup time before WR active. T(wds)=T(lcdic_clk)*twds. */
#define LCDIC_I8080_CTRL0_TWDS(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TWDS_SHIFT)) & LCDIC_I8080_CTRL0_TWDS_MASK)

#define LCDIC_I8080_CTRL0_TWDH_MASK              (0xE0000000U)
#define LCDIC_I8080_CTRL0_TWDH_SHIFT             (29U)
/*! TWDH - Write data hold time. Minimum write data setup time after WR active. T(wdh)=T(lcdic_clk)*twdh. */
#define LCDIC_I8080_CTRL0_TWDH(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL0_TWDH_SHIFT)) & LCDIC_I8080_CTRL0_TWDH_MASK)
/*! @} */

/*! @name I8080_CTRL1 - I8080 Control1 Reigster */
/*! @{ */

#define LCDIC_I8080_CTRL1_TWAW_MASK              (0x3FU)
#define LCDIC_I8080_CTRL1_TWAW_SHIFT             (0U)
/*! TWAW - Minmum write enable active pulse width. T(waw)=T(lcdic_clk)*twaw. */
#define LCDIC_I8080_CTRL1_TWAW(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL1_TWAW_SHIFT)) & LCDIC_I8080_CTRL1_TWAW_MASK)

#define LCDIC_I8080_CTRL1_TWIW_MASK              (0xFC0U)
#define LCDIC_I8080_CTRL1_TWIW_SHIFT             (6U)
/*! TWIW - Minmum write enable inactive pulse width. T(wiw)=T(lcdic_clk)*twiw. */
#define LCDIC_I8080_CTRL1_TWIW(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL1_TWIW_SHIFT)) & LCDIC_I8080_CTRL1_TWIW_MASK)

#define LCDIC_I8080_CTRL1_TRAW_MASK              (0xFF000U)
#define LCDIC_I8080_CTRL1_TRAW_SHIFT             (12U)
/*! TRAW - Minmum read enable active pulse width. T(raw)=T(lcdic_clk)*traw. */
#define LCDIC_I8080_CTRL1_TRAW(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL1_TRAW_SHIFT)) & LCDIC_I8080_CTRL1_TRAW_MASK)

#define LCDIC_I8080_CTRL1_TRIW_MASK              (0xFF00000U)
#define LCDIC_I8080_CTRL1_TRIW_SHIFT             (20U)
/*! TRIW - Minmum read enable inactive pulse width. T(riw)=T(lcdic_clk)*triw. */
#define LCDIC_I8080_CTRL1_TRIW(x)                (((uint32_t)(((uint32_t)(x)) << LCDIC_I8080_CTRL1_TRIW_SHIFT)) & LCDIC_I8080_CTRL1_TRIW_MASK)
/*! @} */

/*! @name SPI_CTRL - SPI Control */
/*! @{ */

#define LCDIC_SPI_CTRL_DC_POL_MASK               (0x1U)
#define LCDIC_SPI_CTRL_DC_POL_SHIFT              (0U)
/*! DC_POL - DC polarity. */
#define LCDIC_SPI_CTRL_DC_POL(x)                 (((uint32_t)(((uint32_t)(x)) << LCDIC_SPI_CTRL_DC_POL_SHIFT)) & LCDIC_SPI_CTRL_DC_POL_MASK)

#define LCDIC_SPI_CTRL_CPOL_MASK                 (0x2U)
#define LCDIC_SPI_CTRL_CPOL_SHIFT                (1U)
/*! CPOL - SPI CPOL. */
#define LCDIC_SPI_CTRL_CPOL(x)                   (((uint32_t)(((uint32_t)(x)) << LCDIC_SPI_CTRL_CPOL_SHIFT)) & LCDIC_SPI_CTRL_CPOL_MASK)

#define LCDIC_SPI_CTRL_CPHA_MASK                 (0x4U)
#define LCDIC_SPI_CTRL_CPHA_SHIFT                (2U)
/*! CPHA - SPI CPHA. */
#define LCDIC_SPI_CTRL_CPHA(x)                   (((uint32_t)(((uint32_t)(x)) << LCDIC_SPI_CTRL_CPHA_SHIFT)) & LCDIC_SPI_CTRL_CPHA_MASK)

#define LCDIC_SPI_CTRL_SDAT_ENDIAN_MASK          (0x8U)
#define LCDIC_SPI_CTRL_SDAT_ENDIAN_SHIFT         (3U)
/*! SDAT_ENDIAN - SPI serial data endian. */
#define LCDIC_SPI_CTRL_SDAT_ENDIAN(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_SPI_CTRL_SDAT_ENDIAN_SHIFT)) & LCDIC_SPI_CTRL_SDAT_ENDIAN_MASK)
/*! @} */

/*! @name TE_CTRL - Tearing Effect Control */
/*! @{ */

#define LCDIC_TE_CTRL_TTEW_MASK                  (0x3FU)
#define LCDIC_TE_CTRL_TTEW_SHIFT                 (0U)
/*! TTEW - Tearing effect signal synchronization wait time. Unit is T(timer_base1). T(tew)=T(timer_base1)*ttew. */
#define LCDIC_TE_CTRL_TTEW(x)                    (((uint32_t)(((uint32_t)(x)) << LCDIC_TE_CTRL_TTEW_SHIFT)) & LCDIC_TE_CTRL_TTEW_MASK)

#define LCDIC_TE_CTRL_TE_TO_MASK                 (0xFC0U)
#define LCDIC_TE_CTRL_TE_TO_SHIFT                (6U)
/*! TE_TO - Tearing effect timeout time. Unit is T(timer_base1). T(te_to)=T(timer_base1)*te_to. 6'd0 means no te_to check. */
#define LCDIC_TE_CTRL_TE_TO(x)                   (((uint32_t)(((uint32_t)(x)) << LCDIC_TE_CTRL_TE_TO_SHIFT)) & LCDIC_TE_CTRL_TE_TO_MASK)
/*! @} */

/*! @name TO_CTRL - Baseline Control Register 0 */
/*! @{ */

#define LCDIC_TO_CTRL_CMD_SHORT_TO_MASK          (0xFU)
#define LCDIC_TO_CTRL_CMD_SHORT_TO_SHIFT         (0U)
/*! CMD_SHORT_TO - Command short timeout. Unit is T(timer_base0).
 *    T(cmd_short_to)=T(timer_base0)*cmd_short_to. 4'd0 means no cmd_short_to check.
 */
#define LCDIC_TO_CTRL_CMD_SHORT_TO(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_TO_CTRL_CMD_SHORT_TO_SHIFT)) & LCDIC_TO_CTRL_CMD_SHORT_TO_MASK)

#define LCDIC_TO_CTRL_CMD_LONG_TO_MASK           (0x3F0U)
#define LCDIC_TO_CTRL_CMD_LONG_TO_SHIFT          (4U)
/*! CMD_LONG_TO - Command long timeout. Unit is T(timer_base1).
 *    T(cmd_long_to)=T(timer_base1)*cmd_long_to. 16'd0 means no cmd_long_to check.
 */
#define LCDIC_TO_CTRL_CMD_LONG_TO(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_TO_CTRL_CMD_LONG_TO_SHIFT)) & LCDIC_TO_CTRL_CMD_LONG_TO_MASK)
/*! @} */

/*! @name TFIFO_WDATA - Baseline Control Register 1 */
/*! @{ */

#define LCDIC_TFIFO_WDATA_TFIFO_WDATA_MASK       (0xFFFFFFFFU)
#define LCDIC_TFIFO_WDATA_TFIFO_WDATA_SHIFT      (0U)
/*! TFIFO_WDATA - TX FIFO write data. */
#define LCDIC_TFIFO_WDATA_TFIFO_WDATA(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_TFIFO_WDATA_TFIFO_WDATA_SHIFT)) & LCDIC_TFIFO_WDATA_TFIFO_WDATA_MASK)
/*! @} */

/*! @name RFIFO_RDATA - Baseline Control Register 2 */
/*! @{ */

#define LCDIC_RFIFO_RDATA_RFIFO_RDATA_MASK       (0xFFFFFFFFU)
#define LCDIC_RFIFO_RDATA_RFIFO_RDATA_SHIFT      (0U)
/*! RFIFO_RDATA - RX FIFO read data. */
#define LCDIC_RFIFO_RDATA_RFIFO_RDATA(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_RFIFO_RDATA_RFIFO_RDATA_SHIFT)) & LCDIC_RFIFO_RDATA_RFIFO_RDATA_MASK)
/*! @} */

/*! @name ISR - Interrupt Status Register */
/*! @{ */

#define LCDIC_ISR_RFIFO_THRES_INTR_MASK          (0x1U)
#define LCDIC_ISR_RFIFO_THRES_INTR_SHIFT         (0U)
/*! RFIFO_THRES_INTR - RX FIFO threshold interrupt. */
#define LCDIC_ISR_RFIFO_THRES_INTR(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_RFIFO_THRES_INTR_SHIFT)) & LCDIC_ISR_RFIFO_THRES_INTR_MASK)

#define LCDIC_ISR_RFIFO_UNDERFLOW_INTR_MASK      (0x2U)
#define LCDIC_ISR_RFIFO_UNDERFLOW_INTR_SHIFT     (1U)
/*! RFIFO_UNDERFLOW_INTR - RX FIFO underflow interrupt. */
#define LCDIC_ISR_RFIFO_UNDERFLOW_INTR(x)        (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_RFIFO_UNDERFLOW_INTR_SHIFT)) & LCDIC_ISR_RFIFO_UNDERFLOW_INTR_MASK)

#define LCDIC_ISR_TFIFO_THRES_INTR_MASK          (0x4U)
#define LCDIC_ISR_TFIFO_THRES_INTR_SHIFT         (2U)
/*! TFIFO_THRES_INTR - TX FIFO threshold interrupt. */
#define LCDIC_ISR_TFIFO_THRES_INTR(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_TFIFO_THRES_INTR_SHIFT)) & LCDIC_ISR_TFIFO_THRES_INTR_MASK)

#define LCDIC_ISR_TFIFO_OVERFLOW_INTR_MASK       (0x8U)
#define LCDIC_ISR_TFIFO_OVERFLOW_INTR_SHIFT      (3U)
/*! TFIFO_OVERFLOW_INTR - TX FIFO overflow interrupt. */
#define LCDIC_ISR_TFIFO_OVERFLOW_INTR(x)         (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_TFIFO_OVERFLOW_INTR_SHIFT)) & LCDIC_ISR_TFIFO_OVERFLOW_INTR_MASK)

#define LCDIC_ISR_TE_TO_INTR_MASK                (0x10U)
#define LCDIC_ISR_TE_TO_INTR_SHIFT               (4U)
/*! TE_TO_INTR - TE timeout interrupt. */
#define LCDIC_ISR_TE_TO_INTR(x)                  (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_TE_TO_INTR_SHIFT)) & LCDIC_ISR_TE_TO_INTR_MASK)

#define LCDIC_ISR_CMD_TO_INTR_MASK               (0x20U)
#define LCDIC_ISR_CMD_TO_INTR_SHIFT              (5U)
/*! CMD_TO_INTR - TRX command timeout interrupt. */
#define LCDIC_ISR_CMD_TO_INTR(x)                 (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_CMD_TO_INTR_SHIFT)) & LCDIC_ISR_CMD_TO_INTR_MASK)

#define LCDIC_ISR_CMD_DONE_INTR_MASK             (0x40U)
#define LCDIC_ISR_CMD_DONE_INTR_SHIFT            (6U)
/*! CMD_DONE_INTR - TRX command done interrupt. */
#define LCDIC_ISR_CMD_DONE_INTR(x)               (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_CMD_DONE_INTR_SHIFT)) & LCDIC_ISR_CMD_DONE_INTR_MASK)

#define LCDIC_ISR_RST_DONE_INTR_MASK             (0x80U)
#define LCDIC_ISR_RST_DONE_INTR_SHIFT            (7U)
/*! RST_DONE_INTR - Reset done interrupt. */
#define LCDIC_ISR_RST_DONE_INTR(x)               (((uint32_t)(((uint32_t)(x)) << LCDIC_ISR_RST_DONE_INTR_SHIFT)) & LCDIC_ISR_RST_DONE_INTR_MASK)
/*! @} */

/*! @name IRSR - Interrupt Raw Status Register */
/*! @{ */

#define LCDIC_IRSR_RFIFO_THRES_RAW_INTR_MASK     (0x1U)
#define LCDIC_IRSR_RFIFO_THRES_RAW_INTR_SHIFT    (0U)
/*! RFIFO_THRES_RAW_INTR - RX FIFO threshold raw interrupt. */
#define LCDIC_IRSR_RFIFO_THRES_RAW_INTR(x)       (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_RFIFO_THRES_RAW_INTR_SHIFT)) & LCDIC_IRSR_RFIFO_THRES_RAW_INTR_MASK)

#define LCDIC_IRSR_RFIFO_UNDERFLOW_RAW_INTR_MASK (0x2U)
#define LCDIC_IRSR_RFIFO_UNDERFLOW_RAW_INTR_SHIFT (1U)
/*! RFIFO_UNDERFLOW_RAW_INTR - RX FIFO underflow raw interrupt. */
#define LCDIC_IRSR_RFIFO_UNDERFLOW_RAW_INTR(x)   (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_RFIFO_UNDERFLOW_RAW_INTR_SHIFT)) & LCDIC_IRSR_RFIFO_UNDERFLOW_RAW_INTR_MASK)

#define LCDIC_IRSR_TFIFO_THRES_RAW_INTR_MASK     (0x4U)
#define LCDIC_IRSR_TFIFO_THRES_RAW_INTR_SHIFT    (2U)
/*! TFIFO_THRES_RAW_INTR - TX FIFO threshold raw interrupt. */
#define LCDIC_IRSR_TFIFO_THRES_RAW_INTR(x)       (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_TFIFO_THRES_RAW_INTR_SHIFT)) & LCDIC_IRSR_TFIFO_THRES_RAW_INTR_MASK)

#define LCDIC_IRSR_TFIFO_OVERFLOW_RAW_INTR_MASK  (0x8U)
#define LCDIC_IRSR_TFIFO_OVERFLOW_RAW_INTR_SHIFT (3U)
/*! TFIFO_OVERFLOW_RAW_INTR - TX FIFO overflow raw interrupt. */
#define LCDIC_IRSR_TFIFO_OVERFLOW_RAW_INTR(x)    (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_TFIFO_OVERFLOW_RAW_INTR_SHIFT)) & LCDIC_IRSR_TFIFO_OVERFLOW_RAW_INTR_MASK)

#define LCDIC_IRSR_TE_TO_RAW_INTR_MASK           (0x10U)
#define LCDIC_IRSR_TE_TO_RAW_INTR_SHIFT          (4U)
/*! TE_TO_RAW_INTR - TE timeout raw interrupt. */
#define LCDIC_IRSR_TE_TO_RAW_INTR(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_TE_TO_RAW_INTR_SHIFT)) & LCDIC_IRSR_TE_TO_RAW_INTR_MASK)

#define LCDIC_IRSR_CMD_TO_RAW_INTR_MASK          (0x20U)
#define LCDIC_IRSR_CMD_TO_RAW_INTR_SHIFT         (5U)
/*! CMD_TO_RAW_INTR - TRX command timeout raw interrupt. */
#define LCDIC_IRSR_CMD_TO_RAW_INTR(x)            (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_CMD_TO_RAW_INTR_SHIFT)) & LCDIC_IRSR_CMD_TO_RAW_INTR_MASK)

#define LCDIC_IRSR_CMD_DONE_RAW_INTR_MASK        (0x40U)
#define LCDIC_IRSR_CMD_DONE_RAW_INTR_SHIFT       (6U)
/*! CMD_DONE_RAW_INTR - TRX command done raw interrupt. */
#define LCDIC_IRSR_CMD_DONE_RAW_INTR(x)          (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_CMD_DONE_RAW_INTR_SHIFT)) & LCDIC_IRSR_CMD_DONE_RAW_INTR_MASK)

#define LCDIC_IRSR_RST_DONE_RAW_INTR_MASK        (0x80U)
#define LCDIC_IRSR_RST_DONE_RAW_INTR_SHIFT       (7U)
/*! RST_DONE_RAW_INTR - Reset done raw interrupt. */
#define LCDIC_IRSR_RST_DONE_RAW_INTR(x)          (((uint32_t)(((uint32_t)(x)) << LCDIC_IRSR_RST_DONE_RAW_INTR_SHIFT)) & LCDIC_IRSR_RST_DONE_RAW_INTR_MASK)
/*! @} */

/*! @name ICR - Interrupt Clear Register */
/*! @{ */

#define LCDIC_ICR_RFIFO_THRES_INTR_CLR_MASK      (0x1U)
#define LCDIC_ICR_RFIFO_THRES_INTR_CLR_SHIFT     (0U)
/*! RFIFO_THRES_INTR_CLR - RX FIFO threshold interrupt clear. */
#define LCDIC_ICR_RFIFO_THRES_INTR_CLR(x)        (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_RFIFO_THRES_INTR_CLR_SHIFT)) & LCDIC_ICR_RFIFO_THRES_INTR_CLR_MASK)

#define LCDIC_ICR_RFIFO_UNDERFLOW_INTR_CLR_MASK  (0x2U)
#define LCDIC_ICR_RFIFO_UNDERFLOW_INTR_CLR_SHIFT (1U)
/*! RFIFO_UNDERFLOW_INTR_CLR - RX FIFO underflow interrupt clear. */
#define LCDIC_ICR_RFIFO_UNDERFLOW_INTR_CLR(x)    (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_RFIFO_UNDERFLOW_INTR_CLR_SHIFT)) & LCDIC_ICR_RFIFO_UNDERFLOW_INTR_CLR_MASK)

#define LCDIC_ICR_TFIFO_THRES_INTR_CLR_MASK      (0x4U)
#define LCDIC_ICR_TFIFO_THRES_INTR_CLR_SHIFT     (2U)
/*! TFIFO_THRES_INTR_CLR - TX FIFO threshold interrupt clear. */
#define LCDIC_ICR_TFIFO_THRES_INTR_CLR(x)        (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_TFIFO_THRES_INTR_CLR_SHIFT)) & LCDIC_ICR_TFIFO_THRES_INTR_CLR_MASK)

#define LCDIC_ICR_TFIFO_OVERFLOW_INTR_CLR_MASK   (0x8U)
#define LCDIC_ICR_TFIFO_OVERFLOW_INTR_CLR_SHIFT  (3U)
/*! TFIFO_OVERFLOW_INTR_CLR - TX FIFO overflow interrupt clear. */
#define LCDIC_ICR_TFIFO_OVERFLOW_INTR_CLR(x)     (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_TFIFO_OVERFLOW_INTR_CLR_SHIFT)) & LCDIC_ICR_TFIFO_OVERFLOW_INTR_CLR_MASK)

#define LCDIC_ICR_TE_TO_INTR_CLR_MASK            (0x10U)
#define LCDIC_ICR_TE_TO_INTR_CLR_SHIFT           (4U)
/*! TE_TO_INTR_CLR - TE timeout interrupt clear. */
#define LCDIC_ICR_TE_TO_INTR_CLR(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_TE_TO_INTR_CLR_SHIFT)) & LCDIC_ICR_TE_TO_INTR_CLR_MASK)

#define LCDIC_ICR_CMD_TO_INTR_CLR_MASK           (0x20U)
#define LCDIC_ICR_CMD_TO_INTR_CLR_SHIFT          (5U)
/*! CMD_TO_INTR_CLR - TRX command timeout interrupt clear. */
#define LCDIC_ICR_CMD_TO_INTR_CLR(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_CMD_TO_INTR_CLR_SHIFT)) & LCDIC_ICR_CMD_TO_INTR_CLR_MASK)

#define LCDIC_ICR_CMD_DONE_INTR_CLR_MASK         (0x40U)
#define LCDIC_ICR_CMD_DONE_INTR_CLR_SHIFT        (6U)
/*! CMD_DONE_INTR_CLR - TRX command done interrupt clear. */
#define LCDIC_ICR_CMD_DONE_INTR_CLR(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_CMD_DONE_INTR_CLR_SHIFT)) & LCDIC_ICR_CMD_DONE_INTR_CLR_MASK)

#define LCDIC_ICR_RST_DONE_INTR_CLR_MASK         (0x80U)
#define LCDIC_ICR_RST_DONE_INTR_CLR_SHIFT        (7U)
/*! RST_DONE_INTR_CLR - Reset done interrupt clear. */
#define LCDIC_ICR_RST_DONE_INTR_CLR(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_ICR_RST_DONE_INTR_CLR_SHIFT)) & LCDIC_ICR_RST_DONE_INTR_CLR_MASK)
/*! @} */

/*! @name IMR - Touch Detection Control Register2 */
/*! @{ */

#define LCDIC_IMR_RFIFO_THRES_INTR_MSK_MASK      (0x1U)
#define LCDIC_IMR_RFIFO_THRES_INTR_MSK_SHIFT     (0U)
/*! RFIFO_THRES_INTR_MSK - RX FIFO threshold interrupt mask. */
#define LCDIC_IMR_RFIFO_THRES_INTR_MSK(x)        (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_RFIFO_THRES_INTR_MSK_SHIFT)) & LCDIC_IMR_RFIFO_THRES_INTR_MSK_MASK)

#define LCDIC_IMR_RFIFO_UNDERFLOW_INTR_MSK_MASK  (0x2U)
#define LCDIC_IMR_RFIFO_UNDERFLOW_INTR_MSK_SHIFT (1U)
/*! RFIFO_UNDERFLOW_INTR_MSK - RX FIFO underflow interrupt mask. */
#define LCDIC_IMR_RFIFO_UNDERFLOW_INTR_MSK(x)    (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_RFIFO_UNDERFLOW_INTR_MSK_SHIFT)) & LCDIC_IMR_RFIFO_UNDERFLOW_INTR_MSK_MASK)

#define LCDIC_IMR_TFIFO_THRES_INTR_MSK_MASK      (0x4U)
#define LCDIC_IMR_TFIFO_THRES_INTR_MSK_SHIFT     (2U)
/*! TFIFO_THRES_INTR_MSK - TX FIFO threshold interrupt mask. */
#define LCDIC_IMR_TFIFO_THRES_INTR_MSK(x)        (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_TFIFO_THRES_INTR_MSK_SHIFT)) & LCDIC_IMR_TFIFO_THRES_INTR_MSK_MASK)

#define LCDIC_IMR_TFIFO_OVERFLOW_INTR_MSK_MASK   (0x8U)
#define LCDIC_IMR_TFIFO_OVERFLOW_INTR_MSK_SHIFT  (3U)
/*! TFIFO_OVERFLOW_INTR_MSK - TX FIFO overflow interrupt mask. */
#define LCDIC_IMR_TFIFO_OVERFLOW_INTR_MSK(x)     (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_TFIFO_OVERFLOW_INTR_MSK_SHIFT)) & LCDIC_IMR_TFIFO_OVERFLOW_INTR_MSK_MASK)

#define LCDIC_IMR_TE_TO_INTR_MSK_MASK            (0x10U)
#define LCDIC_IMR_TE_TO_INTR_MSK_SHIFT           (4U)
/*! TE_TO_INTR_MSK - TE timeout interrupt mask. */
#define LCDIC_IMR_TE_TO_INTR_MSK(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_TE_TO_INTR_MSK_SHIFT)) & LCDIC_IMR_TE_TO_INTR_MSK_MASK)

#define LCDIC_IMR_CMD_TO_INTR_MSK_MASK           (0x20U)
#define LCDIC_IMR_CMD_TO_INTR_MSK_SHIFT          (5U)
/*! CMD_TO_INTR_MSK - TRX command timeout interrupt mask. */
#define LCDIC_IMR_CMD_TO_INTR_MSK(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_CMD_TO_INTR_MSK_SHIFT)) & LCDIC_IMR_CMD_TO_INTR_MSK_MASK)

#define LCDIC_IMR_CMD_DONE_INTR_MSK_MASK         (0x40U)
#define LCDIC_IMR_CMD_DONE_INTR_MSK_SHIFT        (6U)
/*! CMD_DONE_INTR_MSK - TRX command done interrupt mask. */
#define LCDIC_IMR_CMD_DONE_INTR_MSK(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_CMD_DONE_INTR_MSK_SHIFT)) & LCDIC_IMR_CMD_DONE_INTR_MSK_MASK)

#define LCDIC_IMR_RST_DONE_INTR_MSK_MASK         (0x80U)
#define LCDIC_IMR_RST_DONE_INTR_MSK_SHIFT        (7U)
/*! RST_DONE_INTR_MSK - Reset done interrupt mask. */
#define LCDIC_IMR_RST_DONE_INTR_MSK(x)           (((uint32_t)(((uint32_t)(x)) << LCDIC_IMR_RST_DONE_INTR_MSK_SHIFT)) & LCDIC_IMR_RST_DONE_INTR_MSK_MASK)
/*! @} */

/*! @name STATUS0 - Status Register */
/*! @{ */

#define LCDIC_STATUS0_LCDIC_IDLE_MASK            (0x1U)
#define LCDIC_STATUS0_LCDIC_IDLE_SHIFT           (0U)
/*! LCDIC_IDLE - lcdic system idle. */
#define LCDIC_STATUS0_LCDIC_IDLE(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_LCDIC_IDLE_SHIFT)) & LCDIC_STATUS0_LCDIC_IDLE_MASK)

#define LCDIC_STATUS0_TFIFO_THRES_MASK           (0x2U)
#define LCDIC_STATUS0_TFIFO_THRES_SHIFT          (1U)
/*! TFIFO_THRES - TX FIFO threshold status. */
#define LCDIC_STATUS0_TFIFO_THRES(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_TFIFO_THRES_SHIFT)) & LCDIC_STATUS0_TFIFO_THRES_MASK)

#define LCDIC_STATUS0_TFIFO_FULL_MASK            (0x4U)
#define LCDIC_STATUS0_TFIFO_FULL_SHIFT           (2U)
/*! TFIFO_FULL - TX FIFO full. */
#define LCDIC_STATUS0_TFIFO_FULL(x)              (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_TFIFO_FULL_SHIFT)) & LCDIC_STATUS0_TFIFO_FULL_MASK)

#define LCDIC_STATUS0_RFIFO_THRES_MASK           (0x8U)
#define LCDIC_STATUS0_RFIFO_THRES_SHIFT          (3U)
/*! RFIFO_THRES - RX FIFO threshold status. */
#define LCDIC_STATUS0_RFIFO_THRES(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_RFIFO_THRES_SHIFT)) & LCDIC_STATUS0_RFIFO_THRES_MASK)

#define LCDIC_STATUS0_RFIFO_EMPTY_MASK           (0x10U)
#define LCDIC_STATUS0_RFIFO_EMPTY_SHIFT          (4U)
/*! RFIFO_EMPTY - RX FIFO empty. */
#define LCDIC_STATUS0_RFIFO_EMPTY(x)             (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_RFIFO_EMPTY_SHIFT)) & LCDIC_STATUS0_RFIFO_EMPTY_MASK)

#define LCDIC_STATUS0_TB_CNT_MASK                (0x7FFFE0U)
#define LCDIC_STATUS0_TB_CNT_SHIFT               (5U)
/*! TB_CNT - Transmission byte counter which indicates how many TB has been successfully transmitted. */
#define LCDIC_STATUS0_TB_CNT(x)                  (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS0_TB_CNT_SHIFT)) & LCDIC_STATUS0_TB_CNT_MASK)
/*! @} */

/*! @name STATUS1 - Touch Detection Control Register4 */
/*! @{ */

#define LCDIC_STATUS1_TRX_CMD_MASK               (0xFFFFFFFFU)
#define LCDIC_STATUS1_TRX_CMD_SHIFT              (0U)
/*! TRX_CMD - TRX command which is under processing. */
#define LCDIC_STATUS1_TRX_CMD(x)                 (((uint32_t)(((uint32_t)(x)) << LCDIC_STATUS1_TRX_CMD_SHIFT)) & LCDIC_STATUS1_TRX_CMD_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group LCDIC_Register_Masks */


/*!
 * @}
 */ /* end of group LCDIC_Peripheral_Access_Layer */


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


#endif  /* LCDIC_H_ */

