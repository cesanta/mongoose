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
**         CMSIS Peripheral Access Layer for USIM
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
 * @file USIM.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for USIM
 *
 * CMSIS Peripheral Access Layer for USIM
 */

#if !defined(USIM_H_)
#define USIM_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- USIM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USIM_Peripheral_Access_Layer USIM Peripheral Access Layer
 * @{
 */

/** USIM - Register Layout Typedef */
typedef struct {
  __I  uint32_t RBR;                               /**< Receive Buffer Register, offset: 0x0 */
  __O  uint32_t THR;                               /**< Transmit Holding Register, offset: 0x4 */
  __IO uint32_t IER;                               /**< Interrupt Enable Register, offset: 0x8 */
  __IO uint32_t IIR;                               /**< Interrupt Identification Register, offset: 0xC */
  __IO uint32_t FCR;                               /**< FIFO Control Register, offset: 0x10 */
  __I  uint32_t FSR;                               /**< FIFO Status Register, offset: 0x14 */
  __IO uint32_t ECR;                               /**< Error Control Register, offset: 0x18 */
  __IO uint32_t LCR;                               /**< Line Control Register, offset: 0x1C */
  __IO uint32_t USCCR;                             /**< Card Control Register, offset: 0x20 */
  __I  uint32_t LSR;                               /**< Line Status Register, offset: 0x24 */
  __IO uint32_t EGTR;                              /**< Extra Guard Time Register, offset: 0x28 */
  __IO uint32_t BGTR;                              /**< Block Guard Time Register, offset: 0x2C */
  __IO uint32_t TOR;                               /**< Time Out Register, offset: 0x30 */
  __IO uint32_t CLKR;                              /**< Clock Register, offset: 0x34 */
  __IO uint32_t DLR;                               /**< Divisor Latch Register, offset: 0x38 */
  __IO uint32_t FLR;                               /**< Factor Latch Register, offset: 0x3C */
  __IO uint32_t CWTR;                              /**< Character Waiting Time Register, offset: 0x40 */
  __IO uint32_t BWTR;                              /**< Block Waiting Time Register, offset: 0x44 */
} USIM_Type;

/* ----------------------------------------------------------------------------
   -- USIM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USIM_Register_Masks USIM Register Masks
 * @{
 */

/*! @name RBR - Receive Buffer Register */
/*! @{ */

#define USIM_RBR_RB_MASK                         (0xFFU)
#define USIM_RBR_RB_SHIFT                        (0U)
/*! RB - Data byte received */
#define USIM_RBR_RB(x)                           (((uint32_t)(((uint32_t)(x)) << USIM_RBR_RB_SHIFT)) & USIM_RBR_RB_MASK)

#define USIM_RBR_PERR_MASK                       (0x100U)
#define USIM_RBR_PERR_SHIFT                      (8U)
/*! PERR - parity error bit -When not masked by the PEM bit in the FCR register, the parity error
 *    indicator will appear in this bit.
 */
#define USIM_RBR_PERR(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_RBR_PERR_SHIFT)) & USIM_RBR_PERR_MASK)
/*! @} */

/*! @name THR - Transmit Holding Register */
/*! @{ */

#define USIM_THR_TB_MASK                         (0xFFU)
#define USIM_THR_TB_SHIFT                        (0U)
/*! TB - Data byte transmitted, leaset significant bit first */
#define USIM_THR_TB(x)                           (((uint32_t)(((uint32_t)(x)) << USIM_THR_TB_SHIFT)) & USIM_THR_TB_MASK)
/*! @} */

/*! @name IER - Interrupt Enable Register */
/*! @{ */

#define USIM_IER_OVRN_MASK                       (0x1U)
#define USIM_IER_OVRN_SHIFT                      (0U)
/*! OVRN - Receiver Dta Overrun Interrupt */
#define USIM_IER_OVRN(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_IER_OVRN_SHIFT)) & USIM_IER_OVRN_MASK)

#define USIM_IER_PERR_MASK                       (0x2U)
#define USIM_IER_PERR_SHIFT                      (1U)
/*! PERR - Parity Error Interrupt */
#define USIM_IER_PERR(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_IER_PERR_SHIFT)) & USIM_IER_PERR_MASK)

#define USIM_IER_T0ERR_MASK                      (0x4U)
#define USIM_IER_T0ERR_SHIFT                     (2U)
/*! T0ERR - T=0 Error Interrupt */
#define USIM_IER_T0ERR(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_IER_T0ERR_SHIFT)) & USIM_IER_T0ERR_MASK)

#define USIM_IER_FRAMERR_MASK                    (0x8U)
#define USIM_IER_FRAMERR_SHIFT                   (3U)
/*! FRAMERR - Framing Error Interrupt */
#define USIM_IER_FRAMERR(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_IER_FRAMERR_SHIFT)) & USIM_IER_FRAMERR_MASK)

#define USIM_IER_TIMEO_MASK                      (0x10U)
#define USIM_IER_TIMEO_SHIFT                     (4U)
/*! TIMEO - Receiver Time Out Interrupt */
#define USIM_IER_TIMEO(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_IER_TIMEO_SHIFT)) & USIM_IER_TIMEO_MASK)

#define USIM_IER_CWT_MASK                        (0x20U)
#define USIM_IER_CWT_SHIFT                       (5U)
/*! CWT - Character Waiting Time Interrupt */
#define USIM_IER_CWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IER_CWT_SHIFT)) & USIM_IER_CWT_MASK)

#define USIM_IER_BWT_MASK                        (0x40U)
#define USIM_IER_BWT_SHIFT                       (6U)
/*! BWT - Block Waiting Time Interrupt */
#define USIM_IER_BWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IER_BWT_SHIFT)) & USIM_IER_BWT_MASK)

#define USIM_IER_RDR_MASK                        (0x100U)
#define USIM_IER_RDR_SHIFT                       (8U)
/*! RDR - Receiver Data Ready Interrupt */
#define USIM_IER_RDR(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IER_RDR_SHIFT)) & USIM_IER_RDR_MASK)

#define USIM_IER_TDR_MASK                        (0x200U)
#define USIM_IER_TDR_SHIFT                       (9U)
/*! TDR - Transmitter Data Refill Interrupt */
#define USIM_IER_TDR(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IER_TDR_SHIFT)) & USIM_IER_TDR_MASK)

#define USIM_IER_DMA_TIME_MASK                   (0x2000U)
#define USIM_IER_DMA_TIME_SHIFT                  (13U)
/*! DMA_TIME - DMA Receive Request in event of a Time Out situation */
#define USIM_IER_DMA_TIME(x)                     (((uint32_t)(((uint32_t)(x)) << USIM_IER_DMA_TIME_SHIFT)) & USIM_IER_DMA_TIME_MASK)

#define USIM_IER_DMA_RX_MASK                     (0x4000U)
#define USIM_IER_DMA_RX_SHIFT                    (14U)
/*! DMA_RX - DMA Receive Request */
#define USIM_IER_DMA_RX(x)                       (((uint32_t)(((uint32_t)(x)) << USIM_IER_DMA_RX_SHIFT)) & USIM_IER_DMA_RX_MASK)

#define USIM_IER_DMA_TX_MASK                     (0x8000U)
#define USIM_IER_DMA_TX_SHIFT                    (15U)
/*! DMA_TX - DMA Transmit Request */
#define USIM_IER_DMA_TX(x)                       (((uint32_t)(((uint32_t)(x)) << USIM_IER_DMA_TX_SHIFT)) & USIM_IER_DMA_TX_MASK)
/*! @} */

/*! @name IIR - Interrupt Identification Register */
/*! @{ */

#define USIM_IIR_OVRN_MASK                       (0x1U)
#define USIM_IIR_OVRN_SHIFT                      (0U)
/*! OVRN - Receiver Data Overrun Interrupt */
#define USIM_IIR_OVRN(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_IIR_OVRN_SHIFT)) & USIM_IIR_OVRN_MASK)

#define USIM_IIR_PERR_MASK                       (0x2U)
#define USIM_IIR_PERR_SHIFT                      (1U)
/*! PERR - Parity Error Interrupt */
#define USIM_IIR_PERR(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_IIR_PERR_SHIFT)) & USIM_IIR_PERR_MASK)

#define USIM_IIR_T0ERR_MASK                      (0x4U)
#define USIM_IIR_T0ERR_SHIFT                     (2U)
/*! T0ERR - T=0 Error Interrupt */
#define USIM_IIR_T0ERR(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_IIR_T0ERR_SHIFT)) & USIM_IIR_T0ERR_MASK)

#define USIM_IIR_FRAMERR_MASK                    (0x8U)
#define USIM_IIR_FRAMERR_SHIFT                   (3U)
/*! FRAMERR - Framing Error Interrupt */
#define USIM_IIR_FRAMERR(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_IIR_FRAMERR_SHIFT)) & USIM_IIR_FRAMERR_MASK)

#define USIM_IIR_TIMEO_MASK                      (0x10U)
#define USIM_IIR_TIMEO_SHIFT                     (4U)
/*! TIMEO - Receiver Time Out Interrupt */
#define USIM_IIR_TIMEO(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_IIR_TIMEO_SHIFT)) & USIM_IIR_TIMEO_MASK)

#define USIM_IIR_CWT_MASK                        (0x20U)
#define USIM_IIR_CWT_SHIFT                       (5U)
/*! CWT - Character Waiting Time Interrupt */
#define USIM_IIR_CWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IIR_CWT_SHIFT)) & USIM_IIR_CWT_MASK)

#define USIM_IIR_BWT_MASK                        (0x40U)
#define USIM_IIR_BWT_SHIFT                       (6U)
/*! BWT - Block Waiting Time Interrupt */
#define USIM_IIR_BWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IIR_BWT_SHIFT)) & USIM_IIR_BWT_MASK)

#define USIM_IIR_RDR_MASK                        (0x100U)
#define USIM_IIR_RDR_SHIFT                       (8U)
/*! RDR - Receive Data Ready Interrupt */
#define USIM_IIR_RDR(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IIR_RDR_SHIFT)) & USIM_IIR_RDR_MASK)

#define USIM_IIR_TDR_MASK                        (0x200U)
#define USIM_IIR_TDR_SHIFT                       (9U)
/*! TDR - Transmitter Data Refill Interrupt */
#define USIM_IIR_TDR(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_IIR_TDR_SHIFT)) & USIM_IIR_TDR_MASK)
/*! @} */

/*! @name FCR - FIFO Control Register */
/*! @{ */

#define USIM_FCR_RESETRF_MASK                    (0x1U)
#define USIM_FCR_RESETRF_SHIFT                   (0U)
/*! RESETRF - Reset Receive FIFO */
#define USIM_FCR_RESETRF(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_FCR_RESETRF_SHIFT)) & USIM_FCR_RESETRF_MASK)

#define USIM_FCR_RESETTF_MASK                    (0x2U)
#define USIM_FCR_RESETTF_SHIFT                   (1U)
/*! RESETTF - Reset Transmit FIFO */
#define USIM_FCR_RESETTF(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_FCR_RESETTF_SHIFT)) & USIM_FCR_RESETTF_MASK)

#define USIM_FCR_TX_HOLD_MASK                    (0x4U)
#define USIM_FCR_TX_HOLD_SHIFT                   (2U)
/*! TX_HOLD - Transmission Hold */
#define USIM_FCR_TX_HOLD(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_FCR_TX_HOLD_SHIFT)) & USIM_FCR_TX_HOLD_MASK)

#define USIM_FCR_PEM_MASK                        (0x8U)
#define USIM_FCR_PEM_SHIFT                       (3U)
/*! PEM - Parity Error Mask */
#define USIM_FCR_PEM(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_FCR_PEM_SHIFT)) & USIM_FCR_PEM_MASK)

#define USIM_FCR_RX_TL_MASK                      (0xC0U)
#define USIM_FCR_RX_TL_SHIFT                     (6U)
/*! RX_TL - Receiver Trigger Level */
#define USIM_FCR_RX_TL(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_FCR_RX_TL_SHIFT)) & USIM_FCR_RX_TL_MASK)

#define USIM_FCR_TX_TL_MASK                      (0x100U)
#define USIM_FCR_TX_TL_SHIFT                     (8U)
/*! TX_TL - Transmitter Trigger Level */
#define USIM_FCR_TX_TL(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_FCR_TX_TL_SHIFT)) & USIM_FCR_TX_TL_MASK)
/*! @} */

/*! @name FSR - FIFO Status Register */
/*! @{ */

#define USIM_FSR_RX_LENGTH_MASK                  (0x1FU)
#define USIM_FSR_RX_LENGTH_SHIFT                 (0U)
/*! RX_LENGTH - Receive FIFO length */
#define USIM_FSR_RX_LENGTH(x)                    (((uint32_t)(((uint32_t)(x)) << USIM_FSR_RX_LENGTH_SHIFT)) & USIM_FSR_RX_LENGTH_MASK)

#define USIM_FSR_TX_LENGTH_MASK                  (0x3E0U)
#define USIM_FSR_TX_LENGTH_SHIFT                 (5U)
/*! TX_LENGTH - Transmit FIFO length */
#define USIM_FSR_TX_LENGTH(x)                    (((uint32_t)(((uint32_t)(x)) << USIM_FSR_TX_LENGTH_SHIFT)) & USIM_FSR_TX_LENGTH_MASK)

#define USIM_FSR_PERR_NUM_MASK                   (0x7C00U)
#define USIM_FSR_PERR_NUM_SHIFT                  (10U)
/*! PERR_NUM - Parity Error Number */
#define USIM_FSR_PERR_NUM(x)                     (((uint32_t)(((uint32_t)(x)) << USIM_FSR_PERR_NUM_SHIFT)) & USIM_FSR_PERR_NUM_MASK)
/*! @} */

/*! @name ECR - Error Control Register */
/*! @{ */

#define USIM_ECR_T0ERR_TL_MASK                   (0x3U)
#define USIM_ECR_T0ERR_TL_SHIFT                  (0U)
/*! T0ERR_TL - T=0 Error Trigger Level */
#define USIM_ECR_T0ERR_TL(x)                     (((uint32_t)(((uint32_t)(x)) << USIM_ECR_T0ERR_TL_SHIFT)) & USIM_ECR_T0ERR_TL_MASK)

#define USIM_ECR_PE_TL_MASK                      (0x18U)
#define USIM_ECR_PE_TL_SHIFT                     (3U)
/*! PE_TL - Parity Error Trigger Level */
#define USIM_ECR_PE_TL(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_ECR_PE_TL_SHIFT)) & USIM_ECR_PE_TL_MASK)

#define USIM_ECR_T0_CLR_MASK                     (0x40U)
#define USIM_ECR_T0_CLR_SHIFT                    (6U)
/*! T0_CLR - Clear T=0 Error */
#define USIM_ECR_T0_CLR(x)                       (((uint32_t)(((uint32_t)(x)) << USIM_ECR_T0_CLR_SHIFT)) & USIM_ECR_T0_CLR_MASK)

#define USIM_ECR_T0_REPEAT_MASK                  (0x80U)
#define USIM_ECR_T0_REPEAT_SHIFT                 (7U)
/*! T0_REPEAT - Repeat Character Transmission */
#define USIM_ECR_T0_REPEAT(x)                    (((uint32_t)(((uint32_t)(x)) << USIM_ECR_T0_REPEAT_SHIFT)) & USIM_ECR_T0_REPEAT_MASK)
/*! @} */

/*! @name LCR - Line Control Register */
/*! @{ */

#define USIM_LCR_INVERSE_MASK                    (0x1U)
#define USIM_LCR_INVERSE_SHIFT                   (0U)
/*! INVERSE - Bit inversion */
#define USIM_LCR_INVERSE(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_LCR_INVERSE_SHIFT)) & USIM_LCR_INVERSE_MASK)

#define USIM_LCR_ORDER_MASK                      (0x2U)
#define USIM_LCR_ORDER_SHIFT                     (1U)
/*! ORDER - Transmit/Receive Bit Order */
#define USIM_LCR_ORDER(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_LCR_ORDER_SHIFT)) & USIM_LCR_ORDER_MASK)

#define USIM_LCR_EPS_MASK                        (0x4U)
#define USIM_LCR_EPS_SHIFT                       (2U)
/*! EPS - Even Parity Select(EPS) - This bit is the even parity select bit. When EPS is a logic 0,
 *    an odd number of logic ones is transmitted or checked in the data word bits and the parity bit.
 *    When EPS is a logic 1, an even number of logic ones is transmitted or checked in the data
 *    word bits and parity bit
 */
#define USIM_LCR_EPS(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_LCR_EPS_SHIFT)) & USIM_LCR_EPS_MASK)

#define USIM_LCR_RX_T1_MASK                      (0x8U)
#define USIM_LCR_RX_T1_SHIFT                     (3U)
/*! RX_T1 - Receiver Protocol */
#define USIM_LCR_RX_T1(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_LCR_RX_T1_SHIFT)) & USIM_LCR_RX_T1_MASK)

#define USIM_LCR_TX_T1_MASK                      (0x10U)
#define USIM_LCR_TX_T1_SHIFT                     (4U)
/*! TX_T1 - Transmitter Protocol */
#define USIM_LCR_TX_T1(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_LCR_TX_T1_SHIFT)) & USIM_LCR_TX_T1_MASK)
/*! @} */

/*! @name USCCR - Card Control Register */
/*! @{ */

#define USIM_USCCR_RST_CARD_N_MASK               (0x1U)
#define USIM_USCCR_RST_CARD_N_SHIFT              (0U)
/*! RST_CARD_N - Card Reset */
#define USIM_USCCR_RST_CARD_N(x)                 (((uint32_t)(((uint32_t)(x)) << USIM_USCCR_RST_CARD_N_SHIFT)) & USIM_USCCR_RST_CARD_N_MASK)

#define USIM_USCCR_VCC_MASK                      (0x2U)
#define USIM_USCCR_VCC_SHIFT                     (1U)
/*! VCC - Card Voltage */
#define USIM_USCCR_VCC(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_USCCR_VCC_SHIFT)) & USIM_USCCR_VCC_MASK)

#define USIM_USCCR_TXD_FORCE_MASK                (0x10U)
#define USIM_USCCR_TXD_FORCE_SHIFT               (4U)
/*! TXD_FORCE - Force TXD - This bit should always remain non-active unless running a deactivation
 *    process. In deactivation the I/O must be turned low before turning down the card's voltage
 */
#define USIM_USCCR_TXD_FORCE(x)                  (((uint32_t)(((uint32_t)(x)) << USIM_USCCR_TXD_FORCE_SHIFT)) & USIM_USCCR_TXD_FORCE_MASK)
/*! @} */

/*! @name LSR - Line Status Register */
/*! @{ */

#define USIM_LSR_OVRN_MASK                       (0x1U)
#define USIM_LSR_OVRN_SHIFT                      (0U)
/*! OVRN - Receiver Data Overrun Error */
#define USIM_LSR_OVRN(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_LSR_OVRN_SHIFT)) & USIM_LSR_OVRN_MASK)

#define USIM_LSR_PERR_MASK                       (0x2U)
#define USIM_LSR_PERR_SHIFT                      (1U)
/*! PERR - Parity Error */
#define USIM_LSR_PERR(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_LSR_PERR_SHIFT)) & USIM_LSR_PERR_MASK)

#define USIM_LSR_T0ERR_MASK                      (0x4U)
#define USIM_LSR_T0ERR_SHIFT                     (2U)
/*! T0ERR - T=0 Error */
#define USIM_LSR_T0ERR(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_LSR_T0ERR_SHIFT)) & USIM_LSR_T0ERR_MASK)

#define USIM_LSR_FRAMERR_MASK                    (0x8U)
#define USIM_LSR_FRAMERR_SHIFT                   (3U)
/*! FRAMERR - Framing Error */
#define USIM_LSR_FRAMERR(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_LSR_FRAMERR_SHIFT)) & USIM_LSR_FRAMERR_MASK)

#define USIM_LSR_TIMEO_MASK                      (0x10U)
#define USIM_LSR_TIMEO_SHIFT                     (4U)
/*! TIMEO - Receiver Time Out */
#define USIM_LSR_TIMEO(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_LSR_TIMEO_SHIFT)) & USIM_LSR_TIMEO_MASK)

#define USIM_LSR_CWT_MASK                        (0x20U)
#define USIM_LSR_CWT_SHIFT                       (5U)
/*! CWT - Character Waiting Time */
#define USIM_LSR_CWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_LSR_CWT_SHIFT)) & USIM_LSR_CWT_MASK)

#define USIM_LSR_BWT_MASK                        (0x40U)
#define USIM_LSR_BWT_SHIFT                       (6U)
/*! BWT - Block Waiting Time */
#define USIM_LSR_BWT(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_LSR_BWT_SHIFT)) & USIM_LSR_BWT_MASK)

#define USIM_LSR_TDR_MASK                        (0x800U)
#define USIM_LSR_TDR_SHIFT                       (11U)
/*! TDR - Transmitter Data Refill */
#define USIM_LSR_TDR(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_LSR_TDR_SHIFT)) & USIM_LSR_TDR_MASK)

#define USIM_LSR_RX_EMPTY_N_MASK                 (0x1000U)
#define USIM_LSR_RX_EMPTY_N_SHIFT                (12U)
/*! RX_EMPTY_N - Receive FIFO Empty */
#define USIM_LSR_RX_EMPTY_N(x)                   (((uint32_t)(((uint32_t)(x)) << USIM_LSR_RX_EMPTY_N_SHIFT)) & USIM_LSR_RX_EMPTY_N_MASK)

#define USIM_LSR_TX_WORKING_MASK                 (0x2000U)
#define USIM_LSR_TX_WORKING_SHIFT                (13U)
/*! TX_WORKING - Transmitter Working */
#define USIM_LSR_TX_WORKING(x)                   (((uint32_t)(((uint32_t)(x)) << USIM_LSR_TX_WORKING_SHIFT)) & USIM_LSR_TX_WORKING_MASK)

#define USIM_LSR_RX_WORKING_MASK                 (0x4000U)
#define USIM_LSR_RX_WORKING_SHIFT                (14U)
/*! RX_WORKING - Receiver Working */
#define USIM_LSR_RX_WORKING(x)                   (((uint32_t)(((uint32_t)(x)) << USIM_LSR_RX_WORKING_SHIFT)) & USIM_LSR_RX_WORKING_MASK)

#define USIM_LSR_RXD_MASK                        (0x8000U)
#define USIM_LSR_RXD_SHIFT                       (15U)
/*! RXD - Reflects serail data from the I/O pad */
#define USIM_LSR_RXD(x)                          (((uint32_t)(((uint32_t)(x)) << USIM_LSR_RXD_SHIFT)) & USIM_LSR_RXD_MASK)
/*! @} */

/*! @name EGTR - Extra Guard Time Register */
/*! @{ */

#define USIM_EGTR_EGTM_MASK                      (0xFFU)
#define USIM_EGTR_EGTM_SHIFT                     (0U)
/*! EGTM - Extra Guard Time Moments: Number of total Guard Time moments */
#define USIM_EGTR_EGTM(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_EGTR_EGTM_SHIFT)) & USIM_EGTR_EGTM_MASK)
/*! @} */

/*! @name BGTR - Block Guard Time Register */
/*! @{ */

#define USIM_BGTR_BGT_MASK                       (0xFFU)
#define USIM_BGTR_BGT_SHIFT                      (0U)
/*! BGT - Block Guard TIME: Number of total Block Guard Time moments */
#define USIM_BGTR_BGT(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_BGTR_BGT_SHIFT)) & USIM_BGTR_BGT_MASK)
/*! @} */

/*! @name TOR - Time Out Register */
/*! @{ */

#define USIM_TOR_TO_MASK                         (0xFFU)
#define USIM_TOR_TO_SHIFT                        (0U)
/*! TO - Time Out: Number of total Time Out moments */
#define USIM_TOR_TO(x)                           (((uint32_t)(((uint32_t)(x)) << USIM_TOR_TO_SHIFT)) & USIM_TOR_TO_MASK)
/*! @} */

/*! @name CLKR - Clock Register */
/*! @{ */

#define USIM_CLKR_DIVISOR_MASK                   (0xFFU)
#define USIM_CLKR_DIVISOR_SHIFT                  (0U)
/*! DIVISOR - Clock Divisor */
#define USIM_CLKR_DIVISOR(x)                     (((uint32_t)(((uint32_t)(x)) << USIM_CLKR_DIVISOR_SHIFT)) & USIM_CLKR_DIVISOR_MASK)

#define USIM_CLKR_RQST_MASK                      (0x1000U)
#define USIM_CLKR_RQST_SHIFT                     (12U)
/*! RQST - Clock Change Request */
#define USIM_CLKR_RQST(x)                        (((uint32_t)(((uint32_t)(x)) << USIM_CLKR_RQST_SHIFT)) & USIM_CLKR_RQST_MASK)

#define USIM_CLKR_STOP_UCLK_MASK                 (0x2000U)
#define USIM_CLKR_STOP_UCLK_SHIFT                (13U)
/*! STOP_UCLK - Stop Card Clock */
#define USIM_CLKR_STOP_UCLK(x)                   (((uint32_t)(((uint32_t)(x)) << USIM_CLKR_STOP_UCLK_SHIFT)) & USIM_CLKR_STOP_UCLK_MASK)

#define USIM_CLKR_STOP_LEVEL_MASK                (0x4000U)
#define USIM_CLKR_STOP_LEVEL_SHIFT               (14U)
/*! STOP_LEVEL - Stop Level */
#define USIM_CLKR_STOP_LEVEL(x)                  (((uint32_t)(((uint32_t)(x)) << USIM_CLKR_STOP_LEVEL_SHIFT)) & USIM_CLKR_STOP_LEVEL_MASK)

#define USIM_CLKR_STOP_CLK_USIM_MASK             (0x8000U)
#define USIM_CLKR_STOP_CLK_USIM_SHIFT            (15U)
/*! STOP_CLK_USIM - Stop USIM_IF Clock */
#define USIM_CLKR_STOP_CLK_USIM(x)               (((uint32_t)(((uint32_t)(x)) << USIM_CLKR_STOP_CLK_USIM_SHIFT)) & USIM_CLKR_STOP_CLK_USIM_MASK)
/*! @} */

/*! @name DLR - Divisor Latch Register */
/*! @{ */

#define USIM_DLR_DIVISOR_MASK                    (0xFFFFU)
#define USIM_DLR_DIVISOR_SHIFT                   (0U)
/*! DIVISOR - Baud Divisor - Determines the number of USIM clock in between samples, forcing the
 *    total length of a bit to be DIVISOR*(FACTOR+1)*(USIM_CYCLE). The value zero if forbidden for
 *    DIVISOR.
 */
#define USIM_DLR_DIVISOR(x)                      (((uint32_t)(((uint32_t)(x)) << USIM_DLR_DIVISOR_SHIFT)) & USIM_DLR_DIVISOR_MASK)
/*! @} */

/*! @name FLR - Factor Latch Register */
/*! @{ */

#define USIM_FLR_FACTOR_MASK                     (0xFFU)
#define USIM_FLR_FACTOR_SHIFT                    (0U)
/*! FACTOR - Baud Factor - Determines the number of samples per bit. Number of samples would be
 *    (FACTOR+1). ISO standard demands minimum of 6 samples; therefore 5 is the minimum value of FACTOR
 *    bits
 */
#define USIM_FLR_FACTOR(x)                       (((uint32_t)(((uint32_t)(x)) << USIM_FLR_FACTOR_SHIFT)) & USIM_FLR_FACTOR_MASK)
/*! @} */

/*! @name CWTR - Character Waiting Time Register */
/*! @{ */

#define USIM_CWTR_CWT_MASK                       (0xFFFFU)
#define USIM_CWTR_CWT_SHIFT                      (0U)
/*! CWT - Character Waiting Time - Number of total Character Waiting Time moments */
#define USIM_CWTR_CWT(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_CWTR_CWT_SHIFT)) & USIM_CWTR_CWT_MASK)
/*! @} */

/*! @name BWTR - Block Waiting Time Register */
/*! @{ */

#define USIM_BWTR_BWT_MASK                       (0xFFFFU)
#define USIM_BWTR_BWT_SHIFT                      (0U)
/*! BWT - Block Waiting Time - Number of total Block Waiting Time moments */
#define USIM_BWTR_BWT(x)                         (((uint32_t)(((uint32_t)(x)) << USIM_BWTR_BWT_SHIFT)) & USIM_BWTR_BWT_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group USIM_Register_Masks */


/*!
 * @}
 */ /* end of group USIM_Peripheral_Access_Layer */


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


#endif  /* USIM_H_ */

