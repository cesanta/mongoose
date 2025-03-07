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
**         CMSIS Peripheral Access Layer for USBC
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
 * @file USBC.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for USBC
 *
 * CMSIS Peripheral Access Layer for USBC
 */

#if !defined(USBC_H_)
#define USBC_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- USBC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USBC_Peripheral_Access_Layer USBC Peripheral Access Layer
 * @{
 */

/** USBC - Size of Registers Arrays */
#define USBC_ENDPTCTRL_COUNT                      15u

/** USBC - Register Layout Typedef */
typedef struct {
  __I  uint32_t ID;                                /**< ID, offset: 0x0 */
  __I  uint32_t HWGENERAL;                         /**< HWGENERAL, offset: 0x4 */
  __I  uint32_t HWHOST;                            /**< HWHOST, offset: 0x8 */
  __I  uint32_t HWDEVICE;                          /**< HWDEVICE, offset: 0xC */
  __I  uint32_t HWTXBUF;                           /**< HWTXBUF, offset: 0x10 */
  __I  uint32_t HWRXBUF;                           /**< HWRXBUF, offset: 0x14 */
  __IO uint32_t HWTXBUF0;                          /**< HWTXBUF0, offset: 0x18 */
  __IO uint32_t HWTXBUF1;                          /**< HWTXBUF1, offset: 0x1C */
       uint8_t RESERVED_0[96];
  __IO uint32_t GPTIMER0LD;                        /**< GPTIMER0LD, offset: 0x80 */
  __IO uint32_t GPTIMER0CTRL;                      /**< GPTIMER0CTRL, offset: 0x84 */
  __IO uint32_t GPTIMER1LD;                        /**< GPTIMER1LD, offset: 0x88 */
  __IO uint32_t GPTIMER1CTRL;                      /**< GPTIMER1CTRL, offset: 0x8C */
  __IO uint32_t SBUSCFG;                           /**< SBUSCFG, offset: 0x90 */
       uint8_t RESERVED_1[108];
  __I  uint32_t CAPLENGTH;                         /**< CAPLENGTH, offset: 0x100 */
  __I  uint32_t HCSPARAMS;                         /**< HCSPARAMS, offset: 0x104 */
  __I  uint32_t HCCPARAMS;                         /**< HCCPARAMS, offset: 0x108 */
       uint8_t RESERVED_2[20];
  __I  uint32_t DCIVERSION;                        /**< DCIVERSION, offset: 0x120 */
  __I  uint32_t DCCPARAMS;                         /**< DCCPARAMS, offset: 0x124 */
  __IO uint32_t DEVLPMCSR;                         /**< DevLPMCSR, offset: 0x128 */
       uint8_t RESERVED_3[20];
  __IO uint32_t USBCMD;                            /**< USBCMD, offset: 0x140 */
  __IO uint32_t USBSTS;                            /**< USBSTS, offset: 0x144 */
  __IO uint32_t USBINTR;                           /**< USBINTR, offset: 0x148 */
  __IO uint32_t FRINDEX;                           /**< FRINDEX, offset: 0x14C */
       uint8_t RESERVED_4[4];
  union {                                          /* offset: 0x154 */
    __IO uint32_t DEVICEADDR;                        /**< DEVICEADDR, offset: 0x154 */
    __IO uint32_t PERIODICLISTBASE;                  /**< PERIODICLISTBASE, offset: 0x154 */
  };
  union {                                          /* offset: 0x158 */
    __IO uint32_t ASYNCLISTADDR;                     /**< ASYNCLISTADDR, offset: 0x158 */
    __IO uint32_t ENDPOINTLISTADDR;                  /**< ENDPOINTLISTADDR, offset: 0x158 */
  };
  __IO uint32_t TTCTRL;                            /**< TTCTRL, offset: 0x15C */
  __IO uint32_t BURSTSIZE;                         /**< BURSTSIZE, offset: 0x160 */
  __IO uint32_t TXFILLTUNING;                      /**< TXFILLTUNING, offset: 0x164 */
  __IO uint32_t TXTTFILLTUNING;                    /**< TXTTFILLTUNING, offset: 0x168 */
  __IO uint32_t IC_USB;                            /**< IC_USB, offset: 0x16C */
  __I  uint32_t ULPI_VIEWPORT;                     /**< ULPI_VIEWPORT, offset: 0x170 */
       uint8_t RESERVED_5[4];
  __IO uint32_t ENDPTNAK;                          /**< ENDPTNAK, offset: 0x178 */
  __IO uint32_t ENDPTNAKEN;                        /**< ENDPTNAKEN, offset: 0x17C */
       uint8_t RESERVED_6[4];
  __IO uint32_t PORTSC1;                           /**< PORTSC1, offset: 0x184 */
  __IO uint32_t PORTSC2;                           /**< PORTSC2, offset: 0x188 */
  __IO uint32_t PORTSC3;                           /**< PORTSC3, offset: 0x18C */
  __IO uint32_t PORTSC4;                           /**< PORTSC4, offset: 0x190 */
  __IO uint32_t PORTSC5;                           /**< PORTSC5, offset: 0x194 */
  __IO uint32_t PORTSC6;                           /**< PORTSC6, offset: 0x198 */
  __IO uint32_t PORTSC7;                           /**< PORTSC7, offset: 0x19C */
  __IO uint32_t PORTSC8;                           /**< PORTSC8, offset: 0x1A0 */
  __IO uint32_t OTGSC;                             /**< OTGSC, offset: 0x1A4 */
  __IO uint32_t USBMODE;                           /**< USBMODE, offset: 0x1A8 */
  __IO uint32_t ENDPTSETUPSTAT;                    /**< ENDPTSETUPSTAT, offset: 0x1AC */
  __IO uint32_t ENDPTPRIME;                        /**< ENDPTPRIME, offset: 0x1B0 */
  __IO uint32_t ENDPTFLUSH;                        /**< ENDPTFLUSH, offset: 0x1B4 */
  __I  uint32_t ENDPTSTAT;                         /**< ENDPTSTAT, offset: 0x1B8 */
  __IO uint32_t ENDPTCOMPLETE;                     /**< ENDPTCOMPLETE, offset: 0x1BC */
  __IO uint32_t ENDPTCTRL0;                        /**< ENDPTCTRL0, offset: 0x1C0 */
  __IO uint32_t ENDPTCTRL[USBC_ENDPTCTRL_COUNT];   /**< ENDPTCTRL1..ENDPTCTRL15, array offset: 0x1C4, array step: 0x4 */
  __IO uint32_t PLL_CONTROL_0;                     /**< PLL_Control_0, offset: 0x200 */
  __IO uint32_t PLL_CONTROL_1;                     /**< PLL_Control_1, offset: 0x204 */
  __IO uint32_t CALIBRATION_CONTROL;               /**< CALIBRATION_Control, offset: 0x208 */
  __IO uint32_t TX_CHANNEL_CONTRL_0;               /**< Tx_Channel_Contrl_0, offset: 0x20C */
  __I  uint32_t TX_CHANNEL_CONTRL_1;               /**< Tx_Channel_Contrl_1, offset: 0x210 */
  __IO uint32_t RX_CHANNEL_CONTRL_0;               /**< Rx_Channel_Contrl_0, offset: 0x214 */
  __IO uint32_t RX_CHANNEL_CONTRL_1;               /**< Rx_Channel_Contrl_1, offset: 0x218 */
  __IO uint32_t DIGITAL_CONTRL_0;                  /**< Digital_Contrl_0, offset: 0x21C */
  __IO uint32_t DIGITAL_CONTRL_1;                  /**< Digital_Contrl_1, offset: 0x220 */
  __IO uint32_t TEST_CONTRL_AND_STATUS_0;          /**< Test_Contrl_and_Status_0, offset: 0x224 */
  __IO uint32_t TEST_CONTRL_AND_STATUS_1;          /**< Test_Contrl_and_Status_1, offset: 0x228 */
  __IO uint32_t MONITOR;                           /**< MONITOR, offset: 0x22C */
  __IO uint32_t RESERVE_ANA;                       /**< PHY_RESERVE, offset: 0x230 */
  __IO uint32_t PHY_REG_OTG_CONTROL;               /**< PHY_REG_OTG_CONTROL, offset: 0x234 */
  __IO uint32_t PHY_REG_CHGDTC_CONTRL_1;           /**< PHY_REG_CHGDTC_CONTRL_1, offset: 0x238 */
       uint8_t RESERVED_7[16];
  __I  uint32_t RESERVED;                          /**< RESERVED, offset: 0x24C */
} USBC_Type;

/* ----------------------------------------------------------------------------
   -- USBC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USBC_Register_Masks USBC Register Masks
 * @{
 */

/*! @name ID - ID */
/*! @{ */

#define USBC_ID_ID_MASK                          (0x3FU)
#define USBC_ID_ID_SHIFT                         (0U)
/*! ID - ID */
#define USBC_ID_ID(x)                            (((uint32_t)(((uint32_t)(x)) << USBC_ID_ID_SHIFT)) & USBC_ID_ID_MASK)

#define USBC_ID_UNUSED_6_MASK                    (0xC0U)
#define USBC_ID_UNUSED_6_SHIFT                   (6U)
/*! UNUSED_6 - UNUSED_6 */
#define USBC_ID_UNUSED_6(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_ID_UNUSED_6_SHIFT)) & USBC_ID_UNUSED_6_MASK)

#define USBC_ID_NID_MASK                         (0x3F00U)
#define USBC_ID_NID_SHIFT                        (8U)
/*! NID - NID */
#define USBC_ID_NID(x)                           (((uint32_t)(((uint32_t)(x)) << USBC_ID_NID_SHIFT)) & USBC_ID_NID_MASK)

#define USBC_ID_UNUSED_14_MASK                   (0xC000U)
#define USBC_ID_UNUSED_14_SHIFT                  (14U)
/*! UNUSED_14 - UNUSED_14 */
#define USBC_ID_UNUSED_14(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_ID_UNUSED_14_SHIFT)) & USBC_ID_UNUSED_14_MASK)

#define USBC_ID_TAG_MASK                         (0x1F0000U)
#define USBC_ID_TAG_SHIFT                        (16U)
/*! TAG - TAG */
#define USBC_ID_TAG(x)                           (((uint32_t)(((uint32_t)(x)) << USBC_ID_TAG_SHIFT)) & USBC_ID_TAG_MASK)

#define USBC_ID_REVISION_MASK                    (0x1E00000U)
#define USBC_ID_REVISION_SHIFT                   (21U)
/*! REVISION - REVISION */
#define USBC_ID_REVISION(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_ID_REVISION_SHIFT)) & USBC_ID_REVISION_MASK)

#define USBC_ID_VERSION_MASK                     (0x1E000000U)
#define USBC_ID_VERSION_SHIFT                    (25U)
/*! VERSION - VERSION */
#define USBC_ID_VERSION(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_ID_VERSION_SHIFT)) & USBC_ID_VERSION_MASK)

#define USBC_ID_CIVERSION_MASK                   (0xE0000000U)
#define USBC_ID_CIVERSION_SHIFT                  (29U)
/*! CIVERSION - CIVERSION */
#define USBC_ID_CIVERSION(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_ID_CIVERSION_SHIFT)) & USBC_ID_CIVERSION_MASK)
/*! @} */

/*! @name HWGENERAL - HWGENERAL */
/*! @{ */

#define USBC_HWGENERAL_RT_MASK                   (0x1U)
#define USBC_HWGENERAL_RT_SHIFT                  (0U)
/*! RT - RT */
#define USBC_HWGENERAL_RT(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_RT_SHIFT)) & USBC_HWGENERAL_RT_MASK)

#define USBC_HWGENERAL_CLKC_MASK                 (0x6U)
#define USBC_HWGENERAL_CLKC_SHIFT                (1U)
/*! CLKC - CLKC */
#define USBC_HWGENERAL_CLKC(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_CLKC_SHIFT)) & USBC_HWGENERAL_CLKC_MASK)

#define USBC_HWGENERAL_BWT_MASK                  (0x8U)
#define USBC_HWGENERAL_BWT_SHIFT                 (3U)
/*! BWT - BWT */
#define USBC_HWGENERAL_BWT(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_BWT_SHIFT)) & USBC_HWGENERAL_BWT_MASK)

#define USBC_HWGENERAL_PHYW_MASK                 (0x30U)
#define USBC_HWGENERAL_PHYW_SHIFT                (4U)
/*! PHYW - PHYW */
#define USBC_HWGENERAL_PHYW(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_PHYW_SHIFT)) & USBC_HWGENERAL_PHYW_MASK)

#define USBC_HWGENERAL_PHYM_MASK                 (0x3C0U)
#define USBC_HWGENERAL_PHYM_SHIFT                (6U)
/*! PHYM - PHYM */
#define USBC_HWGENERAL_PHYM(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_PHYM_SHIFT)) & USBC_HWGENERAL_PHYM_MASK)

#define USBC_HWGENERAL_SM_MASK                   (0xC00U)
#define USBC_HWGENERAL_SM_SHIFT                  (10U)
/*! SM - SM */
#define USBC_HWGENERAL_SM(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_SM_SHIFT)) & USBC_HWGENERAL_SM_MASK)

#define USBC_HWGENERAL_UNUSED_12_MASK            (0xFFFFF000U)
#define USBC_HWGENERAL_UNUSED_12_SHIFT           (12U)
/*! UNUSED_12 - UNUSED_12 */
#define USBC_HWGENERAL_UNUSED_12(x)              (((uint32_t)(((uint32_t)(x)) << USBC_HWGENERAL_UNUSED_12_SHIFT)) & USBC_HWGENERAL_UNUSED_12_MASK)
/*! @} */

/*! @name HWHOST - HWHOST */
/*! @{ */

#define USBC_HWHOST_HC_MASK                      (0x1U)
#define USBC_HWHOST_HC_SHIFT                     (0U)
/*! HC - HC */
#define USBC_HWHOST_HC(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_HWHOST_HC_SHIFT)) & USBC_HWHOST_HC_MASK)

#define USBC_HWHOST_NPORT_MASK                   (0xEU)
#define USBC_HWHOST_NPORT_SHIFT                  (1U)
/*! NPORT - NPORT */
#define USBC_HWHOST_NPORT(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HWHOST_NPORT_SHIFT)) & USBC_HWHOST_NPORT_MASK)

#define USBC_HWHOST_UNUSED_4_MASK                (0xFFF0U)
#define USBC_HWHOST_UNUSED_4_SHIFT               (4U)
/*! UNUSED_4 - UNUSED_4 */
#define USBC_HWHOST_UNUSED_4(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_HWHOST_UNUSED_4_SHIFT)) & USBC_HWHOST_UNUSED_4_MASK)

#define USBC_HWHOST_TTASY_MASK                   (0xFF0000U)
#define USBC_HWHOST_TTASY_SHIFT                  (16U)
/*! TTASY - TTASY */
#define USBC_HWHOST_TTASY(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HWHOST_TTASY_SHIFT)) & USBC_HWHOST_TTASY_MASK)

#define USBC_HWHOST_TTPER_MASK                   (0xFF000000U)
#define USBC_HWHOST_TTPER_SHIFT                  (24U)
/*! TTPER - TTPER */
#define USBC_HWHOST_TTPER(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HWHOST_TTPER_SHIFT)) & USBC_HWHOST_TTPER_MASK)
/*! @} */

/*! @name HWDEVICE - HWDEVICE */
/*! @{ */

#define USBC_HWDEVICE_DC_MASK                    (0x1U)
#define USBC_HWDEVICE_DC_SHIFT                   (0U)
/*! DC - DC */
#define USBC_HWDEVICE_DC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_HWDEVICE_DC_SHIFT)) & USBC_HWDEVICE_DC_MASK)

#define USBC_HWDEVICE_DEVEP_MASK                 (0x3EU)
#define USBC_HWDEVICE_DEVEP_SHIFT                (1U)
/*! DEVEP - DEVEP */
#define USBC_HWDEVICE_DEVEP(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HWDEVICE_DEVEP_SHIFT)) & USBC_HWDEVICE_DEVEP_MASK)

#define USBC_HWDEVICE_UNUSED_6_MASK              (0xFFFFFFC0U)
#define USBC_HWDEVICE_UNUSED_6_SHIFT             (6U)
/*! UNUSED_6 - UNUSED_6 */
#define USBC_HWDEVICE_UNUSED_6(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HWDEVICE_UNUSED_6_SHIFT)) & USBC_HWDEVICE_UNUSED_6_MASK)
/*! @} */

/*! @name HWTXBUF - HWTXBUF */
/*! @{ */

#define USBC_HWTXBUF_TXBURST_MASK                (0xFFU)
#define USBC_HWTXBUF_TXBURST_SHIFT               (0U)
/*! TXBURST - TXBURST */
#define USBC_HWTXBUF_TXBURST(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF_TXBURST_SHIFT)) & USBC_HWTXBUF_TXBURST_MASK)

#define USBC_HWTXBUF_TXADD_MASK                  (0xFF00U)
#define USBC_HWTXBUF_TXADD_SHIFT                 (8U)
/*! TXADD - TXADD */
#define USBC_HWTXBUF_TXADD(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF_TXADD_SHIFT)) & USBC_HWTXBUF_TXADD_MASK)

#define USBC_HWTXBUF_TXCHANADD_MASK              (0xFF0000U)
#define USBC_HWTXBUF_TXCHANADD_SHIFT             (16U)
/*! TXCHANADD - TXCHANADD */
#define USBC_HWTXBUF_TXCHANADD(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF_TXCHANADD_SHIFT)) & USBC_HWTXBUF_TXCHANADD_MASK)

#define USBC_HWTXBUF_UNUSED_24_MASK              (0x7F000000U)
#define USBC_HWTXBUF_UNUSED_24_SHIFT             (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_HWTXBUF_UNUSED_24(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF_UNUSED_24_SHIFT)) & USBC_HWTXBUF_UNUSED_24_MASK)

#define USBC_HWTXBUF_UNUSED_31_MASK              (0x80000000U)
#define USBC_HWTXBUF_UNUSED_31_SHIFT             (31U)
/*! UNUSED_31 - UNUSED_31 */
#define USBC_HWTXBUF_UNUSED_31(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF_UNUSED_31_SHIFT)) & USBC_HWTXBUF_UNUSED_31_MASK)
/*! @} */

/*! @name HWRXBUF - HWRXBUF */
/*! @{ */

#define USBC_HWRXBUF_RXBURST_MASK                (0xFFU)
#define USBC_HWRXBUF_RXBURST_SHIFT               (0U)
/*! RXBURST - RXBURST */
#define USBC_HWRXBUF_RXBURST(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_HWRXBUF_RXBURST_SHIFT)) & USBC_HWRXBUF_RXBURST_MASK)

#define USBC_HWRXBUF_RXADD_MASK                  (0xFF00U)
#define USBC_HWRXBUF_RXADD_SHIFT                 (8U)
/*! RXADD - RXADD */
#define USBC_HWRXBUF_RXADD(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HWRXBUF_RXADD_SHIFT)) & USBC_HWRXBUF_RXADD_MASK)

#define USBC_HWRXBUF_UNUSED_16_MASK              (0xFFFF0000U)
#define USBC_HWRXBUF_UNUSED_16_SHIFT             (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_HWRXBUF_UNUSED_16(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HWRXBUF_UNUSED_16_SHIFT)) & USBC_HWRXBUF_UNUSED_16_MASK)
/*! @} */

/*! @name HWTXBUF0 - HWTXBUF0 */
/*! @{ */

#define USBC_HWTXBUF0_TXBURST_MASK               (0xFFFFFFFFU)
#define USBC_HWTXBUF0_TXBURST_SHIFT              (0U)
/*! TXBURST - TXBURST */
#define USBC_HWTXBUF0_TXBURST(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF0_TXBURST_SHIFT)) & USBC_HWTXBUF0_TXBURST_MASK)
/*! @} */

/*! @name HWTXBUF1 - HWTXBUF1 */
/*! @{ */

#define USBC_HWTXBUF1_TXBURST_MASK               (0xFFFFFFFFU)
#define USBC_HWTXBUF1_TXBURST_SHIFT              (0U)
/*! TXBURST - TXBURST */
#define USBC_HWTXBUF1_TXBURST(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_HWTXBUF1_TXBURST_SHIFT)) & USBC_HWTXBUF1_TXBURST_MASK)
/*! @} */

/*! @name GPTIMER0LD - GPTIMER0LD */
/*! @{ */

#define USBC_GPTIMER0LD_GPTLD_MASK               (0xFFFFFFU)
#define USBC_GPTIMER0LD_GPTLD_SHIFT              (0U)
/*! GPTLD - GPTLD */
#define USBC_GPTIMER0LD_GPTLD(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0LD_GPTLD_SHIFT)) & USBC_GPTIMER0LD_GPTLD_MASK)

#define USBC_GPTIMER0LD_UNUSED_24_MASK           (0xFF000000U)
#define USBC_GPTIMER0LD_UNUSED_24_SHIFT          (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_GPTIMER0LD_UNUSED_24(x)             (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0LD_UNUSED_24_SHIFT)) & USBC_GPTIMER0LD_UNUSED_24_MASK)
/*! @} */

/*! @name GPTIMER0CTRL - GPTIMER0CTRL */
/*! @{ */

#define USBC_GPTIMER0CTRL_GPTCNT_MASK            (0xFFFFFFU)
#define USBC_GPTIMER0CTRL_GPTCNT_SHIFT           (0U)
/*! GPTCNT - GPTCNT */
#define USBC_GPTIMER0CTRL_GPTCNT(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0CTRL_GPTCNT_SHIFT)) & USBC_GPTIMER0CTRL_GPTCNT_MASK)

#define USBC_GPTIMER0CTRL_GPTMODE_MASK           (0x1000000U)
#define USBC_GPTIMER0CTRL_GPTMODE_SHIFT          (24U)
/*! GPTMODE - GPTMODE */
#define USBC_GPTIMER0CTRL_GPTMODE(x)             (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0CTRL_GPTMODE_SHIFT)) & USBC_GPTIMER0CTRL_GPTMODE_MASK)

#define USBC_GPTIMER0CTRL_UNUSED_25_MASK         (0x3E000000U)
#define USBC_GPTIMER0CTRL_UNUSED_25_SHIFT        (25U)
/*! UNUSED_25 - UNUSED_25 */
#define USBC_GPTIMER0CTRL_UNUSED_25(x)           (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0CTRL_UNUSED_25_SHIFT)) & USBC_GPTIMER0CTRL_UNUSED_25_MASK)

#define USBC_GPTIMER0CTRL_GPTRST_MASK            (0x40000000U)
#define USBC_GPTIMER0CTRL_GPTRST_SHIFT           (30U)
/*! GPTRST - GPTRST */
#define USBC_GPTIMER0CTRL_GPTRST(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0CTRL_GPTRST_SHIFT)) & USBC_GPTIMER0CTRL_GPTRST_MASK)

#define USBC_GPTIMER0CTRL_GPTRUN_MASK            (0x80000000U)
#define USBC_GPTIMER0CTRL_GPTRUN_SHIFT           (31U)
/*! GPTRUN - GPTRUN */
#define USBC_GPTIMER0CTRL_GPTRUN(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER0CTRL_GPTRUN_SHIFT)) & USBC_GPTIMER0CTRL_GPTRUN_MASK)
/*! @} */

/*! @name GPTIMER1LD - GPTIMER1LD */
/*! @{ */

#define USBC_GPTIMER1LD_GPTLD_MASK               (0xFFFFFFU)
#define USBC_GPTIMER1LD_GPTLD_SHIFT              (0U)
/*! GPTLD - GPTLD */
#define USBC_GPTIMER1LD_GPTLD(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1LD_GPTLD_SHIFT)) & USBC_GPTIMER1LD_GPTLD_MASK)

#define USBC_GPTIMER1LD_UNUSED_24_MASK           (0xFF000000U)
#define USBC_GPTIMER1LD_UNUSED_24_SHIFT          (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_GPTIMER1LD_UNUSED_24(x)             (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1LD_UNUSED_24_SHIFT)) & USBC_GPTIMER1LD_UNUSED_24_MASK)
/*! @} */

/*! @name GPTIMER1CTRL - GPTIMER1CTRL */
/*! @{ */

#define USBC_GPTIMER1CTRL_GPTCNT_MASK            (0xFFFFFFU)
#define USBC_GPTIMER1CTRL_GPTCNT_SHIFT           (0U)
/*! GPTCNT - GPTCNT */
#define USBC_GPTIMER1CTRL_GPTCNT(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1CTRL_GPTCNT_SHIFT)) & USBC_GPTIMER1CTRL_GPTCNT_MASK)

#define USBC_GPTIMER1CTRL_GPTMODE_MASK           (0x1000000U)
#define USBC_GPTIMER1CTRL_GPTMODE_SHIFT          (24U)
/*! GPTMODE - GPTMODE */
#define USBC_GPTIMER1CTRL_GPTMODE(x)             (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1CTRL_GPTMODE_SHIFT)) & USBC_GPTIMER1CTRL_GPTMODE_MASK)

#define USBC_GPTIMER1CTRL_UNUSED_25_MASK         (0x3E000000U)
#define USBC_GPTIMER1CTRL_UNUSED_25_SHIFT        (25U)
/*! UNUSED_25 - UNUSED_25 */
#define USBC_GPTIMER1CTRL_UNUSED_25(x)           (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1CTRL_UNUSED_25_SHIFT)) & USBC_GPTIMER1CTRL_UNUSED_25_MASK)

#define USBC_GPTIMER1CTRL_GPTRST_MASK            (0x40000000U)
#define USBC_GPTIMER1CTRL_GPTRST_SHIFT           (30U)
/*! GPTRST - GPTRST */
#define USBC_GPTIMER1CTRL_GPTRST(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1CTRL_GPTRST_SHIFT)) & USBC_GPTIMER1CTRL_GPTRST_MASK)

#define USBC_GPTIMER1CTRL_GPTRUN_MASK            (0x80000000U)
#define USBC_GPTIMER1CTRL_GPTRUN_SHIFT           (31U)
/*! GPTRUN - GPTRUN */
#define USBC_GPTIMER1CTRL_GPTRUN(x)              (((uint32_t)(((uint32_t)(x)) << USBC_GPTIMER1CTRL_GPTRUN_SHIFT)) & USBC_GPTIMER1CTRL_GPTRUN_MASK)
/*! @} */

/*! @name SBUSCFG - SBUSCFG */
/*! @{ */

#define USBC_SBUSCFG_AHBBRST_MASK                (0x7U)
#define USBC_SBUSCFG_AHBBRST_SHIFT               (0U)
/*! AHBBRST - AHBBRST */
#define USBC_SBUSCFG_AHBBRST(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_SBUSCFG_AHBBRST_SHIFT)) & USBC_SBUSCFG_AHBBRST_MASK)

#define USBC_SBUSCFG_BARD_MASK                   (0x38U)
#define USBC_SBUSCFG_BARD_SHIFT                  (3U)
/*! BARD - BARD */
#define USBC_SBUSCFG_BARD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_SBUSCFG_BARD_SHIFT)) & USBC_SBUSCFG_BARD_MASK)

#define USBC_SBUSCFG_BAWR_MASK                   (0x1C0U)
#define USBC_SBUSCFG_BAWR_SHIFT                  (6U)
/*! BAWR - BAWR */
#define USBC_SBUSCFG_BAWR(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_SBUSCFG_BAWR_SHIFT)) & USBC_SBUSCFG_BAWR_MASK)

#define USBC_SBUSCFG_UNUSED_9_MASK               (0xFFFFFE00U)
#define USBC_SBUSCFG_UNUSED_9_SHIFT              (9U)
/*! UNUSED_9 - UNUSED_9 */
#define USBC_SBUSCFG_UNUSED_9(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_SBUSCFG_UNUSED_9_SHIFT)) & USBC_SBUSCFG_UNUSED_9_MASK)
/*! @} */

/*! @name CAPLENGTH - CAPLENGTH */
/*! @{ */

#define USBC_CAPLENGTH_CAPLENGTH_MASK            (0xFFU)
#define USBC_CAPLENGTH_CAPLENGTH_SHIFT           (0U)
/*! CAPLENGTH - CAPLENGTH */
#define USBC_CAPLENGTH_CAPLENGTH(x)              (((uint32_t)(((uint32_t)(x)) << USBC_CAPLENGTH_CAPLENGTH_SHIFT)) & USBC_CAPLENGTH_CAPLENGTH_MASK)

#define USBC_CAPLENGTH_UNUSED_8_MASK             (0xFF00U)
#define USBC_CAPLENGTH_UNUSED_8_SHIFT            (8U)
/*! UNUSED_8 - UNUSED_8 */
#define USBC_CAPLENGTH_UNUSED_8(x)               (((uint32_t)(((uint32_t)(x)) << USBC_CAPLENGTH_UNUSED_8_SHIFT)) & USBC_CAPLENGTH_UNUSED_8_MASK)

#define USBC_CAPLENGTH_HCIVERSION_MASK           (0xFFFF0000U)
#define USBC_CAPLENGTH_HCIVERSION_SHIFT          (16U)
/*! HCIVERSION - HCIVERSION */
#define USBC_CAPLENGTH_HCIVERSION(x)             (((uint32_t)(((uint32_t)(x)) << USBC_CAPLENGTH_HCIVERSION_SHIFT)) & USBC_CAPLENGTH_HCIVERSION_MASK)
/*! @} */

/*! @name HCSPARAMS - HCSPARAMS */
/*! @{ */

#define USBC_HCSPARAMS_N_PORTS_MASK              (0xFU)
#define USBC_HCSPARAMS_N_PORTS_SHIFT             (0U)
/*! N_PORTS - N_PORTS */
#define USBC_HCSPARAMS_N_PORTS(x)                (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_N_PORTS_SHIFT)) & USBC_HCSPARAMS_N_PORTS_MASK)

#define USBC_HCSPARAMS_PPC_MASK                  (0x10U)
#define USBC_HCSPARAMS_PPC_SHIFT                 (4U)
/*! PPC - PPC */
#define USBC_HCSPARAMS_PPC(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_PPC_SHIFT)) & USBC_HCSPARAMS_PPC_MASK)

#define USBC_HCSPARAMS_UNUSED_5_MASK             (0xE0U)
#define USBC_HCSPARAMS_UNUSED_5_SHIFT            (5U)
/*! UNUSED_5 - UNUSED_5 */
#define USBC_HCSPARAMS_UNUSED_5(x)               (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_UNUSED_5_SHIFT)) & USBC_HCSPARAMS_UNUSED_5_MASK)

#define USBC_HCSPARAMS_N_PCC_MASK                (0xF00U)
#define USBC_HCSPARAMS_N_PCC_SHIFT               (8U)
/*! N_PCC - N_PCC */
#define USBC_HCSPARAMS_N_PCC(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_N_PCC_SHIFT)) & USBC_HCSPARAMS_N_PCC_MASK)

#define USBC_HCSPARAMS_N_CC_MASK                 (0xF000U)
#define USBC_HCSPARAMS_N_CC_SHIFT                (12U)
/*! N_CC - N_CC */
#define USBC_HCSPARAMS_N_CC(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_N_CC_SHIFT)) & USBC_HCSPARAMS_N_CC_MASK)

#define USBC_HCSPARAMS_PI_MASK                   (0x10000U)
#define USBC_HCSPARAMS_PI_SHIFT                  (16U)
/*! PI - PI */
#define USBC_HCSPARAMS_PI(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_PI_SHIFT)) & USBC_HCSPARAMS_PI_MASK)

#define USBC_HCSPARAMS_UNUSED_17_MASK            (0xE0000U)
#define USBC_HCSPARAMS_UNUSED_17_SHIFT           (17U)
/*! UNUSED_17 - UNUSED_17 */
#define USBC_HCSPARAMS_UNUSED_17(x)              (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_UNUSED_17_SHIFT)) & USBC_HCSPARAMS_UNUSED_17_MASK)

#define USBC_HCSPARAMS_N_PTT_MASK                (0xF00000U)
#define USBC_HCSPARAMS_N_PTT_SHIFT               (20U)
/*! N_PTT - N_PTT */
#define USBC_HCSPARAMS_N_PTT(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_N_PTT_SHIFT)) & USBC_HCSPARAMS_N_PTT_MASK)

#define USBC_HCSPARAMS_N_TT_MASK                 (0xF000000U)
#define USBC_HCSPARAMS_N_TT_SHIFT                (24U)
/*! N_TT - N_TT */
#define USBC_HCSPARAMS_N_TT(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_N_TT_SHIFT)) & USBC_HCSPARAMS_N_TT_MASK)

#define USBC_HCSPARAMS_UNUSED_28_MASK            (0xF0000000U)
#define USBC_HCSPARAMS_UNUSED_28_SHIFT           (28U)
/*! UNUSED_28 - UNUSED_28 */
#define USBC_HCSPARAMS_UNUSED_28(x)              (((uint32_t)(((uint32_t)(x)) << USBC_HCSPARAMS_UNUSED_28_SHIFT)) & USBC_HCSPARAMS_UNUSED_28_MASK)
/*! @} */

/*! @name HCCPARAMS - HCCPARAMS */
/*! @{ */

#define USBC_HCCPARAMS_ADC_MASK                  (0x1U)
#define USBC_HCCPARAMS_ADC_SHIFT                 (0U)
/*! ADC - ADC */
#define USBC_HCCPARAMS_ADC(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_ADC_SHIFT)) & USBC_HCCPARAMS_ADC_MASK)

#define USBC_HCCPARAMS_PFL_MASK                  (0x2U)
#define USBC_HCCPARAMS_PFL_SHIFT                 (1U)
/*! PFL - PFL */
#define USBC_HCCPARAMS_PFL(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_PFL_SHIFT)) & USBC_HCCPARAMS_PFL_MASK)

#define USBC_HCCPARAMS_ASP_MASK                  (0x4U)
#define USBC_HCCPARAMS_ASP_SHIFT                 (2U)
/*! ASP - ASP */
#define USBC_HCCPARAMS_ASP(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_ASP_SHIFT)) & USBC_HCCPARAMS_ASP_MASK)

#define USBC_HCCPARAMS_UNUSED_3_MASK             (0x8U)
#define USBC_HCCPARAMS_UNUSED_3_SHIFT            (3U)
/*! UNUSED_3 - UNUSED_3 */
#define USBC_HCCPARAMS_UNUSED_3(x)               (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_UNUSED_3_SHIFT)) & USBC_HCCPARAMS_UNUSED_3_MASK)

#define USBC_HCCPARAMS_IST_MASK                  (0xF0U)
#define USBC_HCCPARAMS_IST_SHIFT                 (4U)
/*! IST - IST */
#define USBC_HCCPARAMS_IST(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_IST_SHIFT)) & USBC_HCCPARAMS_IST_MASK)

#define USBC_HCCPARAMS_EECP_MASK                 (0xFF00U)
#define USBC_HCCPARAMS_EECP_SHIFT                (8U)
/*! EECP - EECP */
#define USBC_HCCPARAMS_EECP(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_EECP_SHIFT)) & USBC_HCCPARAMS_EECP_MASK)

#define USBC_HCCPARAMS_UNUSED_16_MASK            (0xFFFF0000U)
#define USBC_HCCPARAMS_UNUSED_16_SHIFT           (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_HCCPARAMS_UNUSED_16(x)              (((uint32_t)(((uint32_t)(x)) << USBC_HCCPARAMS_UNUSED_16_SHIFT)) & USBC_HCCPARAMS_UNUSED_16_MASK)
/*! @} */

/*! @name DCIVERSION - DCIVERSION */
/*! @{ */

#define USBC_DCIVERSION_DCIVERSION_MASK          (0xFFFFU)
#define USBC_DCIVERSION_DCIVERSION_SHIFT         (0U)
/*! DCIVERSION - DCIVERSION */
#define USBC_DCIVERSION_DCIVERSION(x)            (((uint32_t)(((uint32_t)(x)) << USBC_DCIVERSION_DCIVERSION_SHIFT)) & USBC_DCIVERSION_DCIVERSION_MASK)

#define USBC_DCIVERSION_UNUSED_16_MASK           (0xFFFF0000U)
#define USBC_DCIVERSION_UNUSED_16_SHIFT          (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_DCIVERSION_UNUSED_16(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DCIVERSION_UNUSED_16_SHIFT)) & USBC_DCIVERSION_UNUSED_16_MASK)
/*! @} */

/*! @name DCCPARAMS - DCCPARAMS */
/*! @{ */

#define USBC_DCCPARAMS_DEN_MASK                  (0x1FU)
#define USBC_DCCPARAMS_DEN_SHIFT                 (0U)
/*! DEN - DEN */
#define USBC_DCCPARAMS_DEN(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_DEN_SHIFT)) & USBC_DCCPARAMS_DEN_MASK)

#define USBC_DCCPARAMS_UNUSED_5_MASK             (0x60U)
#define USBC_DCCPARAMS_UNUSED_5_SHIFT            (5U)
/*! UNUSED_5 - UNUSED_5 */
#define USBC_DCCPARAMS_UNUSED_5(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_UNUSED_5_SHIFT)) & USBC_DCCPARAMS_UNUSED_5_MASK)

#define USBC_DCCPARAMS_DC_MASK                   (0x80U)
#define USBC_DCCPARAMS_DC_SHIFT                  (7U)
/*! DC - DC */
#define USBC_DCCPARAMS_DC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_DC_SHIFT)) & USBC_DCCPARAMS_DC_MASK)

#define USBC_DCCPARAMS_HC_MASK                   (0x100U)
#define USBC_DCCPARAMS_HC_SHIFT                  (8U)
/*! HC - HC */
#define USBC_DCCPARAMS_HC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_HC_SHIFT)) & USBC_DCCPARAMS_HC_MASK)

#define USBC_DCCPARAMS_UNUSED_9_MASK             (0x7FFFFE00U)
#define USBC_DCCPARAMS_UNUSED_9_SHIFT            (9U)
/*! UNUSED_9 - UNUSED_9 */
#define USBC_DCCPARAMS_UNUSED_9(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_UNUSED_9_SHIFT)) & USBC_DCCPARAMS_UNUSED_9_MASK)

#define USBC_DCCPARAMS_LPM_EN_MASK               (0x80000000U)
#define USBC_DCCPARAMS_LPM_EN_SHIFT              (31U)
/*! LPM_EN - LPM_EN */
#define USBC_DCCPARAMS_LPM_EN(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_DCCPARAMS_LPM_EN_SHIFT)) & USBC_DCCPARAMS_LPM_EN_MASK)
/*! @} */

/*! @name DEVLPMCSR - DevLPMCSR */
/*! @{ */

#define USBC_DEVLPMCSR_INT_L1RSM_MASK            (0x1U)
#define USBC_DEVLPMCSR_INT_L1RSM_SHIFT           (0U)
/*! INT_L1RSM - INT_L1RSM */
#define USBC_DEVLPMCSR_INT_L1RSM(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_INT_L1RSM_SHIFT)) & USBC_DEVLPMCSR_INT_L1RSM_MASK)

#define USBC_DEVLPMCSR_INT_LPMPKT_MASK           (0x2U)
#define USBC_DEVLPMCSR_INT_LPMPKT_SHIFT          (1U)
/*! INT_LPMPKT - INT_LPMPKT */
#define USBC_DEVLPMCSR_INT_LPMPKT(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_INT_LPMPKT_SHIFT)) & USBC_DEVLPMCSR_INT_LPMPKT_MASK)

#define USBC_DEVLPMCSR_INT_LPMACK_MASK           (0x4U)
#define USBC_DEVLPMCSR_INT_LPMACK_SHIFT          (2U)
/*! INT_LPMACK - INT_LPMACK */
#define USBC_DEVLPMCSR_INT_LPMACK(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_INT_LPMACK_SHIFT)) & USBC_DEVLPMCSR_INT_LPMACK_MASK)

#define USBC_DEVLPMCSR_INT_LPMERR_MASK           (0x8U)
#define USBC_DEVLPMCSR_INT_LPMERR_SHIFT          (3U)
/*! INT_LPMERR - INT_LPMERR */
#define USBC_DEVLPMCSR_INT_LPMERR(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_INT_LPMERR_SHIFT)) & USBC_DEVLPMCSR_INT_LPMERR_MASK)

#define USBC_DEVLPMCSR_IE_L1RSM_MASK             (0x10U)
#define USBC_DEVLPMCSR_IE_L1RSM_SHIFT            (4U)
/*! IE_L1RSM - IE_L1RSM */
#define USBC_DEVLPMCSR_IE_L1RSM(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_IE_L1RSM_SHIFT)) & USBC_DEVLPMCSR_IE_L1RSM_MASK)

#define USBC_DEVLPMCSR_IE_LPMPKT_MASK            (0x20U)
#define USBC_DEVLPMCSR_IE_LPMPKT_SHIFT           (5U)
/*! IE_LPMPKT - IE_LPMPKT */
#define USBC_DEVLPMCSR_IE_LPMPKT(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_IE_LPMPKT_SHIFT)) & USBC_DEVLPMCSR_IE_LPMPKT_MASK)

#define USBC_DEVLPMCSR_IE_LPMACK_MASK            (0x40U)
#define USBC_DEVLPMCSR_IE_LPMACK_SHIFT           (6U)
/*! IE_LPMACK - IE_LPMACK */
#define USBC_DEVLPMCSR_IE_LPMACK(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_IE_LPMACK_SHIFT)) & USBC_DEVLPMCSR_IE_LPMACK_MASK)

#define USBC_DEVLPMCSR_IE_LPMERR_MASK            (0x80U)
#define USBC_DEVLPMCSR_IE_LPMERR_SHIFT           (7U)
/*! IE_LPMERR - IE_LPMERR */
#define USBC_DEVLPMCSR_IE_LPMERR(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_IE_LPMERR_SHIFT)) & USBC_DEVLPMCSR_IE_LPMERR_MASK)

#define USBC_DEVLPMCSR_RWAKE_EN_MASK             (0x100U)
#define USBC_DEVLPMCSR_RWAKE_EN_SHIFT            (8U)
/*! RWAKE_EN - RWAKE_EN */
#define USBC_DEVLPMCSR_RWAKE_EN(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_RWAKE_EN_SHIFT)) & USBC_DEVLPMCSR_RWAKE_EN_MASK)

#define USBC_DEVLPMCSR_L1STATE_MASK              (0x200U)
#define USBC_DEVLPMCSR_L1STATE_SHIFT             (9U)
/*! L1STATE - L1STATE */
#define USBC_DEVLPMCSR_L1STATE(x)                (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_L1STATE_SHIFT)) & USBC_DEVLPMCSR_L1STATE_MASK)

#define USBC_DEVLPMCSR_IE_L1STATE_MASK           (0x400U)
#define USBC_DEVLPMCSR_IE_L1STATE_SHIFT          (10U)
/*! IE_L1STATE - IE_L1STATE */
#define USBC_DEVLPMCSR_IE_L1STATE(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_IE_L1STATE_SHIFT)) & USBC_DEVLPMCSR_IE_L1STATE_MASK)

#define USBC_DEVLPMCSR_ACK_OK_MASK               (0x800U)
#define USBC_DEVLPMCSR_ACK_OK_SHIFT              (11U)
/*! ACK_OK - ACK_OK */
#define USBC_DEVLPMCSR_ACK_OK(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_ACK_OK_SHIFT)) & USBC_DEVLPMCSR_ACK_OK_MASK)

#define USBC_DEVLPMCSR_STALL_OK_MASK             (0x1000U)
#define USBC_DEVLPMCSR_STALL_OK_SHIFT            (12U)
/*! STALL_OK - STALL_OK */
#define USBC_DEVLPMCSR_STALL_OK(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_STALL_OK_SHIFT)) & USBC_DEVLPMCSR_STALL_OK_MASK)

#define USBC_DEVLPMCSR_MIN_SLP_EN_MASK           (0x2000U)
#define USBC_DEVLPMCSR_MIN_SLP_EN_SHIFT          (13U)
/*! MIN_SLP_EN - MIN_SLP_EN */
#define USBC_DEVLPMCSR_MIN_SLP_EN(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_MIN_SLP_EN_SHIFT)) & USBC_DEVLPMCSR_MIN_SLP_EN_MASK)

#define USBC_DEVLPMCSR_ALWAYS_LOG_MASK           (0x4000U)
#define USBC_DEVLPMCSR_ALWAYS_LOG_SHIFT          (14U)
/*! ALWAYS_LOG - ALWAYS_LOG */
#define USBC_DEVLPMCSR_ALWAYS_LOG(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_ALWAYS_LOG_SHIFT)) & USBC_DEVLPMCSR_ALWAYS_LOG_MASK)

#define USBC_DEVLPMCSR_LPM_ON_MASK               (0x8000U)
#define USBC_DEVLPMCSR_LPM_ON_SHIFT              (15U)
/*! LPM_ON - LPM_ON */
#define USBC_DEVLPMCSR_LPM_ON(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_LPM_ON_SHIFT)) & USBC_DEVLPMCSR_LPM_ON_MASK)

#define USBC_DEVLPMCSR_HST_RSM_EN_MASK           (0x10000U)
#define USBC_DEVLPMCSR_HST_RSM_EN_SHIFT          (16U)
/*! HST_RSM_EN - HST_RSM_EN */
#define USBC_DEVLPMCSR_HST_RSM_EN(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_HST_RSM_EN_SHIFT)) & USBC_DEVLPMCSR_HST_RSM_EN_MASK)

#define USBC_DEVLPMCSR_LPM_ANY_EP_MASK           (0x20000U)
#define USBC_DEVLPMCSR_LPM_ANY_EP_SHIFT          (17U)
/*! LPM_ANY_EP - LPM_ANY_EP */
#define USBC_DEVLPMCSR_LPM_ANY_EP(x)             (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_LPM_ANY_EP_SHIFT)) & USBC_DEVLPMCSR_LPM_ANY_EP_MASK)

#define USBC_DEVLPMCSR_UNUSED_18_MASK            (0xC0000U)
#define USBC_DEVLPMCSR_UNUSED_18_SHIFT           (18U)
/*! UNUSED_18 - UNUSED_18 */
#define USBC_DEVLPMCSR_UNUSED_18(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_UNUSED_18_SHIFT)) & USBC_DEVLPMCSR_UNUSED_18_MASK)

#define USBC_DEVLPMCSR_HIRD_MASK                 (0xF00000U)
#define USBC_DEVLPMCSR_HIRD_SHIFT                (20U)
/*! HIRD - HIRD */
#define USBC_DEVLPMCSR_HIRD(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_HIRD_SHIFT)) & USBC_DEVLPMCSR_HIRD_MASK)

#define USBC_DEVLPMCSR_LINKSTATE_MASK            (0xF000000U)
#define USBC_DEVLPMCSR_LINKSTATE_SHIFT           (24U)
/*! LINKSTATE - LINKSTATE */
#define USBC_DEVLPMCSR_LINKSTATE(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_LINKSTATE_SHIFT)) & USBC_DEVLPMCSR_LINKSTATE_MASK)

#define USBC_DEVLPMCSR_BRMTWAKE_MASK             (0x10000000U)
#define USBC_DEVLPMCSR_BRMTWAKE_SHIFT            (28U)
/*! BRMTWAKE - BRMTWAKE */
#define USBC_DEVLPMCSR_BRMTWAKE(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_BRMTWAKE_SHIFT)) & USBC_DEVLPMCSR_BRMTWAKE_MASK)

#define USBC_DEVLPMCSR_LPM_PHCD_ONLY_MASK        (0x20000000U)
#define USBC_DEVLPMCSR_LPM_PHCD_ONLY_SHIFT       (29U)
/*! LPM_PHCD_ONLY - LPM_PHCD_only */
#define USBC_DEVLPMCSR_LPM_PHCD_ONLY(x)          (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_LPM_PHCD_ONLY_SHIFT)) & USBC_DEVLPMCSR_LPM_PHCD_ONLY_MASK)

#define USBC_DEVLPMCSR_LPM_RSP_MASK              (0xC0000000U)
#define USBC_DEVLPMCSR_LPM_RSP_SHIFT             (30U)
/*! LPM_RSP - LPM_RSP */
#define USBC_DEVLPMCSR_LPM_RSP(x)                (((uint32_t)(((uint32_t)(x)) << USBC_DEVLPMCSR_LPM_RSP_SHIFT)) & USBC_DEVLPMCSR_LPM_RSP_MASK)
/*! @} */

/*! @name USBCMD - USBCMD */
/*! @{ */

#define USBC_USBCMD_RS_MASK                      (0x1U)
#define USBC_USBCMD_RS_SHIFT                     (0U)
/*! RS - RS */
#define USBC_USBCMD_RS(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_RS_SHIFT)) & USBC_USBCMD_RS_MASK)

#define USBC_USBCMD_RST_MASK                     (0x2U)
#define USBC_USBCMD_RST_SHIFT                    (1U)
/*! RST - RST */
#define USBC_USBCMD_RST(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_RST_SHIFT)) & USBC_USBCMD_RST_MASK)

#define USBC_USBCMD_FS0_MASK                     (0x4U)
#define USBC_USBCMD_FS0_SHIFT                    (2U)
/*! FS0 - HOST only */
#define USBC_USBCMD_FS0(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_FS0_SHIFT)) & USBC_USBCMD_FS0_MASK)

#define USBC_USBCMD_FS1_MASK                     (0x8U)
#define USBC_USBCMD_FS1_SHIFT                    (3U)
/*! FS1 - HOST only */
#define USBC_USBCMD_FS1(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_FS1_SHIFT)) & USBC_USBCMD_FS1_MASK)

#define USBC_USBCMD_PSE_MASK                     (0x10U)
#define USBC_USBCMD_PSE_SHIFT                    (4U)
/*! PSE - HOST only */
#define USBC_USBCMD_PSE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_PSE_SHIFT)) & USBC_USBCMD_PSE_MASK)

#define USBC_USBCMD_ASE_MASK                     (0x20U)
#define USBC_USBCMD_ASE_SHIFT                    (5U)
/*! ASE - HOST only */
#define USBC_USBCMD_ASE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ASE_SHIFT)) & USBC_USBCMD_ASE_MASK)

#define USBC_USBCMD_IAA_MASK                     (0x40U)
#define USBC_USBCMD_IAA_SHIFT                    (6U)
/*! IAA - HOST only */
#define USBC_USBCMD_IAA(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_IAA_SHIFT)) & USBC_USBCMD_IAA_MASK)

#define USBC_USBCMD_LR_MASK                      (0x80U)
#define USBC_USBCMD_LR_SHIFT                     (7U)
/*! LR - LR */
#define USBC_USBCMD_LR(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_LR_SHIFT)) & USBC_USBCMD_LR_MASK)

#define USBC_USBCMD_ASP0_MASK                    (0x100U)
#define USBC_USBCMD_ASP0_SHIFT                   (8U)
/*! ASP0 - HOST only */
#define USBC_USBCMD_ASP0(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ASP0_SHIFT)) & USBC_USBCMD_ASP0_MASK)

#define USBC_USBCMD_ASP1_MASK                    (0x200U)
#define USBC_USBCMD_ASP1_SHIFT                   (9U)
/*! ASP1 - HOST only */
#define USBC_USBCMD_ASP1(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ASP1_SHIFT)) & USBC_USBCMD_ASP1_MASK)

#define USBC_USBCMD_UNUSED_10_MASK               (0x400U)
#define USBC_USBCMD_UNUSED_10_SHIFT              (10U)
/*! UNUSED_10 - UNUSED_10 */
#define USBC_USBCMD_UNUSED_10(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_UNUSED_10_SHIFT)) & USBC_USBCMD_UNUSED_10_MASK)

#define USBC_USBCMD_ASPE_MASK                    (0x800U)
#define USBC_USBCMD_ASPE_SHIFT                   (11U)
/*! ASPE - HOST only */
#define USBC_USBCMD_ASPE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ASPE_SHIFT)) & USBC_USBCMD_ASPE_MASK)

#define USBC_USBCMD_UNUSED_12_MASK               (0x1000U)
#define USBC_USBCMD_UNUSED_12_SHIFT              (12U)
/*! UNUSED_12 - UNUSED_12 */
#define USBC_USBCMD_UNUSED_12(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_UNUSED_12_SHIFT)) & USBC_USBCMD_UNUSED_12_MASK)

#define USBC_USBCMD_SUTW_MASK                    (0x2000U)
#define USBC_USBCMD_SUTW_SHIFT                   (13U)
/*! SUTW - SUTW */
#define USBC_USBCMD_SUTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_SUTW_SHIFT)) & USBC_USBCMD_SUTW_MASK)

#define USBC_USBCMD_ATDTW_MASK                   (0x4000U)
#define USBC_USBCMD_ATDTW_SHIFT                  (14U)
/*! ATDTW - ATDTW */
#define USBC_USBCMD_ATDTW(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ATDTW_SHIFT)) & USBC_USBCMD_ATDTW_MASK)

#define USBC_USBCMD_FS2_MASK                     (0x8000U)
#define USBC_USBCMD_FS2_SHIFT                    (15U)
/*! FS2 - HOST only */
#define USBC_USBCMD_FS2(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_FS2_SHIFT)) & USBC_USBCMD_FS2_MASK)

#define USBC_USBCMD_ITC_MASK                     (0xFF0000U)
#define USBC_USBCMD_ITC_SHIFT                    (16U)
/*! ITC - ITC */
#define USBC_USBCMD_ITC(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_ITC_SHIFT)) & USBC_USBCMD_ITC_MASK)

#define USBC_USBCMD_UNUSED_24_MASK               (0xFF000000U)
#define USBC_USBCMD_UNUSED_24_SHIFT              (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_USBCMD_UNUSED_24(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBCMD_UNUSED_24_SHIFT)) & USBC_USBCMD_UNUSED_24_MASK)
/*! @} */

/*! @name USBSTS - USBSTS */
/*! @{ */

#define USBC_USBSTS_UI_MASK                      (0x1U)
#define USBC_USBSTS_UI_SHIFT                     (0U)
/*! UI - rwc */
#define USBC_USBSTS_UI(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UI_SHIFT)) & USBC_USBSTS_UI_MASK)

#define USBC_USBSTS_UEI_MASK                     (0x2U)
#define USBC_USBSTS_UEI_SHIFT                    (1U)
/*! UEI - rwc */
#define USBC_USBSTS_UEI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UEI_SHIFT)) & USBC_USBSTS_UEI_MASK)

#define USBC_USBSTS_PCI_MASK                     (0x4U)
#define USBC_USBSTS_PCI_SHIFT                    (2U)
/*! PCI - rwc */
#define USBC_USBSTS_PCI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_PCI_SHIFT)) & USBC_USBSTS_PCI_MASK)

#define USBC_USBSTS_FRI_MASK                     (0x8U)
#define USBC_USBSTS_FRI_SHIFT                    (3U)
/*! FRI - rwc */
#define USBC_USBSTS_FRI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_FRI_SHIFT)) & USBC_USBSTS_FRI_MASK)

#define USBC_USBSTS_SEI_MASK                     (0x10U)
#define USBC_USBSTS_SEI_SHIFT                    (4U)
/*! SEI - rwc */
#define USBC_USBSTS_SEI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_SEI_SHIFT)) & USBC_USBSTS_SEI_MASK)

#define USBC_USBSTS_AAI_MASK                     (0x20U)
#define USBC_USBSTS_AAI_SHIFT                    (5U)
/*! AAI - rwc */
#define USBC_USBSTS_AAI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_AAI_SHIFT)) & USBC_USBSTS_AAI_MASK)

#define USBC_USBSTS_URI_MASK                     (0x40U)
#define USBC_USBSTS_URI_SHIFT                    (6U)
/*! URI - rwc */
#define USBC_USBSTS_URI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_URI_SHIFT)) & USBC_USBSTS_URI_MASK)

#define USBC_USBSTS_SRI_MASK                     (0x80U)
#define USBC_USBSTS_SRI_SHIFT                    (7U)
/*! SRI - rwc */
#define USBC_USBSTS_SRI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_SRI_SHIFT)) & USBC_USBSTS_SRI_MASK)

#define USBC_USBSTS_SLI_MASK                     (0x100U)
#define USBC_USBSTS_SLI_SHIFT                    (8U)
/*! SLI - rwc */
#define USBC_USBSTS_SLI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_SLI_SHIFT)) & USBC_USBSTS_SLI_MASK)

#define USBC_USBSTS_UNUSED_9_MASK                (0x200U)
#define USBC_USBSTS_UNUSED_9_SHIFT               (9U)
/*! UNUSED_9 - UNUSED_9 */
#define USBC_USBSTS_UNUSED_9(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UNUSED_9_SHIFT)) & USBC_USBSTS_UNUSED_9_MASK)

#define USBC_USBSTS_ULPII_MASK                   (0x400U)
#define USBC_USBSTS_ULPII_SHIFT                  (10U)
/*! ULPII - rwc */
#define USBC_USBSTS_ULPII(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_ULPII_SHIFT)) & USBC_USBSTS_ULPII_MASK)

#define USBC_USBSTS_UNUSED_11_MASK               (0x800U)
#define USBC_USBSTS_UNUSED_11_SHIFT              (11U)
/*! UNUSED_11 - UNUSED_11 */
#define USBC_USBSTS_UNUSED_11(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UNUSED_11_SHIFT)) & USBC_USBSTS_UNUSED_11_MASK)

#define USBC_USBSTS_HCH_MASK                     (0x1000U)
#define USBC_USBSTS_HCH_SHIFT                    (12U)
/*! HCH - HOST only */
#define USBC_USBSTS_HCH(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_HCH_SHIFT)) & USBC_USBSTS_HCH_MASK)

#define USBC_USBSTS_RCL_MASK                     (0x2000U)
#define USBC_USBSTS_RCL_SHIFT                    (13U)
/*! RCL - RCL */
#define USBC_USBSTS_RCL(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_RCL_SHIFT)) & USBC_USBSTS_RCL_MASK)

#define USBC_USBSTS_PS_MASK                      (0x4000U)
#define USBC_USBSTS_PS_SHIFT                     (14U)
/*! PS - PS */
#define USBC_USBSTS_PS(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_PS_SHIFT)) & USBC_USBSTS_PS_MASK)

#define USBC_USBSTS_AS_MASK                      (0x8000U)
#define USBC_USBSTS_AS_SHIFT                     (15U)
/*! AS - AS */
#define USBC_USBSTS_AS(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_AS_SHIFT)) & USBC_USBSTS_AS_MASK)

#define USBC_USBSTS_NAKI_MASK                    (0x10000U)
#define USBC_USBSTS_NAKI_SHIFT                   (16U)
/*! NAKI - NAKI */
#define USBC_USBSTS_NAKI(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_NAKI_SHIFT)) & USBC_USBSTS_NAKI_MASK)

#define USBC_USBSTS_UNUSED_17_MASK               (0x20000U)
#define USBC_USBSTS_UNUSED_17_SHIFT              (17U)
/*! UNUSED_17 - UNUSED_17 */
#define USBC_USBSTS_UNUSED_17(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UNUSED_17_SHIFT)) & USBC_USBSTS_UNUSED_17_MASK)

#define USBC_USBSTS_UAI_MASK                     (0x40000U)
#define USBC_USBSTS_UAI_SHIFT                    (18U)
/*! UAI - rwc */
#define USBC_USBSTS_UAI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UAI_SHIFT)) & USBC_USBSTS_UAI_MASK)

#define USBC_USBSTS_UPI_MASK                     (0x80000U)
#define USBC_USBSTS_UPI_SHIFT                    (19U)
/*! UPI - rwc */
#define USBC_USBSTS_UPI(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UPI_SHIFT)) & USBC_USBSTS_UPI_MASK)

#define USBC_USBSTS_UNUSED_20_MASK               (0xF00000U)
#define USBC_USBSTS_UNUSED_20_SHIFT              (20U)
/*! UNUSED_20 - UNUSED_20 */
#define USBC_USBSTS_UNUSED_20(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UNUSED_20_SHIFT)) & USBC_USBSTS_UNUSED_20_MASK)

#define USBC_USBSTS_TI0_MASK                     (0x1000000U)
#define USBC_USBSTS_TI0_SHIFT                    (24U)
/*! TI0 - rwc */
#define USBC_USBSTS_TI0(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_TI0_SHIFT)) & USBC_USBSTS_TI0_MASK)

#define USBC_USBSTS_TI1_MASK                     (0x2000000U)
#define USBC_USBSTS_TI1_SHIFT                    (25U)
/*! TI1 - rwc */
#define USBC_USBSTS_TI1(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_TI1_SHIFT)) & USBC_USBSTS_TI1_MASK)

#define USBC_USBSTS_UNUSED_26_MASK               (0xFC000000U)
#define USBC_USBSTS_UNUSED_26_SHIFT              (26U)
/*! UNUSED_26 - UNUSED_26 */
#define USBC_USBSTS_UNUSED_26(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBSTS_UNUSED_26_SHIFT)) & USBC_USBSTS_UNUSED_26_MASK)
/*! @} */

/*! @name USBINTR - USBINTR */
/*! @{ */

#define USBC_USBINTR_UE_MASK                     (0x1U)
#define USBC_USBINTR_UE_SHIFT                    (0U)
/*! UE - UE */
#define USBC_USBINTR_UE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UE_SHIFT)) & USBC_USBINTR_UE_MASK)

#define USBC_USBINTR_UEE_MASK                    (0x2U)
#define USBC_USBINTR_UEE_SHIFT                   (1U)
/*! UEE - rwc */
#define USBC_USBINTR_UEE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UEE_SHIFT)) & USBC_USBINTR_UEE_MASK)

#define USBC_USBINTR_PCE_MASK                    (0x4U)
#define USBC_USBINTR_PCE_SHIFT                   (2U)
/*! PCE - PCE */
#define USBC_USBINTR_PCE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_PCE_SHIFT)) & USBC_USBINTR_PCE_MASK)

#define USBC_USBINTR_FRE_MASK                    (0x8U)
#define USBC_USBINTR_FRE_SHIFT                   (3U)
/*! FRE - HOST only */
#define USBC_USBINTR_FRE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_FRE_SHIFT)) & USBC_USBINTR_FRE_MASK)

#define USBC_USBINTR_SEE_MASK                    (0x10U)
#define USBC_USBINTR_SEE_SHIFT                   (4U)
/*! SEE - SEE */
#define USBC_USBINTR_SEE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_SEE_SHIFT)) & USBC_USBINTR_SEE_MASK)

#define USBC_USBINTR_AAE_MASK                    (0x20U)
#define USBC_USBINTR_AAE_SHIFT                   (5U)
/*! AAE - HOST only */
#define USBC_USBINTR_AAE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_AAE_SHIFT)) & USBC_USBINTR_AAE_MASK)

#define USBC_USBINTR_URE_MASK                    (0x40U)
#define USBC_USBINTR_URE_SHIFT                   (6U)
/*! URE - URE */
#define USBC_USBINTR_URE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_URE_SHIFT)) & USBC_USBINTR_URE_MASK)

#define USBC_USBINTR_SRE_MASK                    (0x80U)
#define USBC_USBINTR_SRE_SHIFT                   (7U)
/*! SRE - SRE */
#define USBC_USBINTR_SRE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_SRE_SHIFT)) & USBC_USBINTR_SRE_MASK)

#define USBC_USBINTR_SLE_MASK                    (0x100U)
#define USBC_USBINTR_SLE_SHIFT                   (8U)
/*! SLE - SLE */
#define USBC_USBINTR_SLE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_SLE_SHIFT)) & USBC_USBINTR_SLE_MASK)

#define USBC_USBINTR_UNUSED_9_MASK               (0x200U)
#define USBC_USBINTR_UNUSED_9_SHIFT              (9U)
/*! UNUSED_9 - UNUSED_9 */
#define USBC_USBINTR_UNUSED_9(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_9_SHIFT)) & USBC_USBINTR_UNUSED_9_MASK)

#define USBC_USBINTR_ULPE_MASK                   (0x400U)
#define USBC_USBINTR_ULPE_SHIFT                  (10U)
/*! ULPE - ONLY used VUSB_HS_PHY_ULPI =1 */
#define USBC_USBINTR_ULPE(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_ULPE_SHIFT)) & USBC_USBINTR_ULPE_MASK)

#define USBC_USBINTR_UNUSED_11_MASK              (0x800U)
#define USBC_USBINTR_UNUSED_11_SHIFT             (11U)
/*! UNUSED_11 - UNUSED_11 */
#define USBC_USBINTR_UNUSED_11(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_11_SHIFT)) & USBC_USBINTR_UNUSED_11_MASK)

#define USBC_USBINTR_UNUSED_12_MASK              (0x1000U)
#define USBC_USBINTR_UNUSED_12_SHIFT             (12U)
/*! UNUSED_12 - UNUSED_12 */
#define USBC_USBINTR_UNUSED_12(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_12_SHIFT)) & USBC_USBINTR_UNUSED_12_MASK)

#define USBC_USBINTR_UNUSED_13_MASK              (0x2000U)
#define USBC_USBINTR_UNUSED_13_SHIFT             (13U)
/*! UNUSED_13 - Not define in DUT,RCL */
#define USBC_USBINTR_UNUSED_13(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_13_SHIFT)) & USBC_USBINTR_UNUSED_13_MASK)

#define USBC_USBINTR_UNUSED_14_MASK              (0x4000U)
#define USBC_USBINTR_UNUSED_14_SHIFT             (14U)
/*! UNUSED_14 - Not define in DUT, PS */
#define USBC_USBINTR_UNUSED_14(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_14_SHIFT)) & USBC_USBINTR_UNUSED_14_MASK)

#define USBC_USBINTR_UNUSED_15_MASK              (0x8000U)
#define USBC_USBINTR_UNUSED_15_SHIFT             (15U)
/*! UNUSED_15 - Not define in DUT, AS */
#define USBC_USBINTR_UNUSED_15(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_15_SHIFT)) & USBC_USBINTR_UNUSED_15_MASK)

#define USBC_USBINTR_NAKE_MASK                   (0x10000U)
#define USBC_USBINTR_NAKE_SHIFT                  (16U)
/*! NAKE - NAKE */
#define USBC_USBINTR_NAKE(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_NAKE_SHIFT)) & USBC_USBINTR_NAKE_MASK)

#define USBC_USBINTR_UNUSED_17_MASK              (0x20000U)
#define USBC_USBINTR_UNUSED_17_SHIFT             (17U)
/*! UNUSED_17 - UNUSED_17 */
#define USBC_USBINTR_UNUSED_17(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_17_SHIFT)) & USBC_USBINTR_UNUSED_17_MASK)

#define USBC_USBINTR_UAE_MASK                    (0x40000U)
#define USBC_USBINTR_UAE_SHIFT                   (18U)
/*! UAE - Not use in Device mode */
#define USBC_USBINTR_UAE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UAE_SHIFT)) & USBC_USBINTR_UAE_MASK)

#define USBC_USBINTR_UPE_MASK                    (0x80000U)
#define USBC_USBINTR_UPE_SHIFT                   (19U)
/*! UPE - Not use in Device mode */
#define USBC_USBINTR_UPE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UPE_SHIFT)) & USBC_USBINTR_UPE_MASK)

#define USBC_USBINTR_UNUSED_20_MASK              (0xF00000U)
#define USBC_USBINTR_UNUSED_20_SHIFT             (20U)
/*! UNUSED_20 - UNUSED_20 */
#define USBC_USBINTR_UNUSED_20(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_20_SHIFT)) & USBC_USBINTR_UNUSED_20_MASK)

#define USBC_USBINTR_TIE0_MASK                   (0x1000000U)
#define USBC_USBINTR_TIE0_SHIFT                  (24U)
/*! TIE0 - TIE0 */
#define USBC_USBINTR_TIE0(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_TIE0_SHIFT)) & USBC_USBINTR_TIE0_MASK)

#define USBC_USBINTR_TIE1_MASK                   (0x2000000U)
#define USBC_USBINTR_TIE1_SHIFT                  (25U)
/*! TIE1 - TIE1 */
#define USBC_USBINTR_TIE1(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_TIE1_SHIFT)) & USBC_USBINTR_TIE1_MASK)

#define USBC_USBINTR_UNUSED_26_MASK              (0xFC000000U)
#define USBC_USBINTR_UNUSED_26_SHIFT             (26U)
/*! UNUSED_26 - UNUSED_26 */
#define USBC_USBINTR_UNUSED_26(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBINTR_UNUSED_26_SHIFT)) & USBC_USBINTR_UNUSED_26_MASK)
/*! @} */

/*! @name FRINDEX - FRINDEX */
/*! @{ */

#define USBC_FRINDEX_FRINDEX_MASK                (0x3FFFU)
#define USBC_FRINDEX_FRINDEX_SHIFT               (0U)
/*! FRINDEX - device RO, Host RW */
#define USBC_FRINDEX_FRINDEX(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_FRINDEX_FRINDEX_SHIFT)) & USBC_FRINDEX_FRINDEX_MASK)

#define USBC_FRINDEX_UNUSED_14_MASK              (0xFFFFC000U)
#define USBC_FRINDEX_UNUSED_14_SHIFT             (14U)
/*! UNUSED_14 - UNUSED_14 */
#define USBC_FRINDEX_UNUSED_14(x)                (((uint32_t)(((uint32_t)(x)) << USBC_FRINDEX_UNUSED_14_SHIFT)) & USBC_FRINDEX_UNUSED_14_MASK)
/*! @} */

/*! @name DEVICEADDR - DEVICEADDR */
/*! @{ */

#define USBC_DEVICEADDR_UNUSED_0_MASK            (0xFFFFFFU)
#define USBC_DEVICEADDR_UNUSED_0_SHIFT           (0U)
/*! UNUSED_0 - UNUSED_0 */
#define USBC_DEVICEADDR_UNUSED_0(x)              (((uint32_t)(((uint32_t)(x)) << USBC_DEVICEADDR_UNUSED_0_SHIFT)) & USBC_DEVICEADDR_UNUSED_0_MASK)

#define USBC_DEVICEADDR_USBADRA_MASK             (0x1000000U)
#define USBC_DEVICEADDR_USBADRA_SHIFT            (24U)
/*! USBADRA - USBADRA */
#define USBC_DEVICEADDR_USBADRA(x)               (((uint32_t)(((uint32_t)(x)) << USBC_DEVICEADDR_USBADRA_SHIFT)) & USBC_DEVICEADDR_USBADRA_MASK)

#define USBC_DEVICEADDR_USBADR_MASK              (0xFE000000U)
#define USBC_DEVICEADDR_USBADR_SHIFT             (25U)
/*! USBADR - USBADR */
#define USBC_DEVICEADDR_USBADR(x)                (((uint32_t)(((uint32_t)(x)) << USBC_DEVICEADDR_USBADR_SHIFT)) & USBC_DEVICEADDR_USBADR_MASK)
/*! @} */

/*! @name PERIODICLISTBASE - PERIODICLISTBASE */
/*! @{ */

#define USBC_PERIODICLISTBASE_UNUSED_0_MASK      (0xFFFU)
#define USBC_PERIODICLISTBASE_UNUSED_0_SHIFT     (0U)
/*! UNUSED_0 - UNUSED_0 */
#define USBC_PERIODICLISTBASE_UNUSED_0(x)        (((uint32_t)(((uint32_t)(x)) << USBC_PERIODICLISTBASE_UNUSED_0_SHIFT)) & USBC_PERIODICLISTBASE_UNUSED_0_MASK)

#define USBC_PERIODICLISTBASE_PERBASE_MASK       (0xFFFFF000U)
#define USBC_PERIODICLISTBASE_PERBASE_SHIFT      (12U)
/*! PERBASE - PERBASE */
#define USBC_PERIODICLISTBASE_PERBASE(x)         (((uint32_t)(((uint32_t)(x)) << USBC_PERIODICLISTBASE_PERBASE_SHIFT)) & USBC_PERIODICLISTBASE_PERBASE_MASK)
/*! @} */

/*! @name ASYNCLISTADDR - ASYNCLISTADDR */
/*! @{ */

#define USBC_ASYNCLISTADDR_UNUSED_0_MASK         (0x1FU)
#define USBC_ASYNCLISTADDR_UNUSED_0_SHIFT        (0U)
/*! UNUSED_0 - UNUSED_0 */
#define USBC_ASYNCLISTADDR_UNUSED_0(x)           (((uint32_t)(((uint32_t)(x)) << USBC_ASYNCLISTADDR_UNUSED_0_SHIFT)) & USBC_ASYNCLISTADDR_UNUSED_0_MASK)

#define USBC_ASYNCLISTADDR_ASYBASE_MASK          (0xFFFFFFE0U)
#define USBC_ASYNCLISTADDR_ASYBASE_SHIFT         (5U)
/*! ASYBASE - ASYBASE */
#define USBC_ASYNCLISTADDR_ASYBASE(x)            (((uint32_t)(((uint32_t)(x)) << USBC_ASYNCLISTADDR_ASYBASE_SHIFT)) & USBC_ASYNCLISTADDR_ASYBASE_MASK)
/*! @} */

/*! @name ENDPOINTLISTADDR - ENDPOINTLISTADDR */
/*! @{ */

#define USBC_ENDPOINTLISTADDR_UNUSED_0_MASK      (0x7FFU)
#define USBC_ENDPOINTLISTADDR_UNUSED_0_SHIFT     (0U)
/*! UNUSED_0 - UNUSED_0 */
#define USBC_ENDPOINTLISTADDR_UNUSED_0(x)        (((uint32_t)(((uint32_t)(x)) << USBC_ENDPOINTLISTADDR_UNUSED_0_SHIFT)) & USBC_ENDPOINTLISTADDR_UNUSED_0_MASK)

#define USBC_ENDPOINTLISTADDR_EPBASE_MASK        (0xFFFFF800U)
#define USBC_ENDPOINTLISTADDR_EPBASE_SHIFT       (11U)
/*! EPBASE - EPBASE */
#define USBC_ENDPOINTLISTADDR_EPBASE(x)          (((uint32_t)(((uint32_t)(x)) << USBC_ENDPOINTLISTADDR_EPBASE_SHIFT)) & USBC_ENDPOINTLISTADDR_EPBASE_MASK)
/*! @} */

/*! @name TTCTRL - TTCTRL */
/*! @{ */

#define USBC_TTCTRL_TTAS_MASK                    (0x1U)
#define USBC_TTCTRL_TTAS_SHIFT                   (0U)
/*! TTAS - TTAS */
#define USBC_TTCTRL_TTAS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_TTCTRL_TTAS_SHIFT)) & USBC_TTCTRL_TTAS_MASK)

#define USBC_TTCTRL_TTAC_MASK                    (0x2U)
#define USBC_TTCTRL_TTAC_SHIFT                   (1U)
/*! TTAC - TTAC */
#define USBC_TTCTRL_TTAC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_TTCTRL_TTAC_SHIFT)) & USBC_TTCTRL_TTAC_MASK)

#define USBC_TTCTRL_UNUSED_2_MASK                (0xFFFFFCU)
#define USBC_TTCTRL_UNUSED_2_SHIFT               (2U)
/*! UNUSED_2 - UNUSED_2 */
#define USBC_TTCTRL_UNUSED_2(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_TTCTRL_UNUSED_2_SHIFT)) & USBC_TTCTRL_UNUSED_2_MASK)

#define USBC_TTCTRL_TTHA_MASK                    (0x7F000000U)
#define USBC_TTCTRL_TTHA_SHIFT                   (24U)
/*! TTHA - TTHA */
#define USBC_TTCTRL_TTHA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_TTCTRL_TTHA_SHIFT)) & USBC_TTCTRL_TTHA_MASK)

#define USBC_TTCTRL_UNUSED_31_MASK               (0x80000000U)
#define USBC_TTCTRL_UNUSED_31_SHIFT              (31U)
/*! UNUSED_31 - UNUSED_31 */
#define USBC_TTCTRL_UNUSED_31(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_TTCTRL_UNUSED_31_SHIFT)) & USBC_TTCTRL_UNUSED_31_MASK)
/*! @} */

/*! @name BURSTSIZE - BURSTSIZE */
/*! @{ */

#define USBC_BURSTSIZE_RXPBURST_MASK             (0xFFU)
#define USBC_BURSTSIZE_RXPBURST_SHIFT            (0U)
/*! RXPBURST - RXPBURST */
#define USBC_BURSTSIZE_RXPBURST(x)               (((uint32_t)(((uint32_t)(x)) << USBC_BURSTSIZE_RXPBURST_SHIFT)) & USBC_BURSTSIZE_RXPBURST_MASK)

#define USBC_BURSTSIZE_TXPBURST_MASK             (0xFF00U)
#define USBC_BURSTSIZE_TXPBURST_SHIFT            (8U)
/*! TXPBURST - TXPBURST */
#define USBC_BURSTSIZE_TXPBURST(x)               (((uint32_t)(((uint32_t)(x)) << USBC_BURSTSIZE_TXPBURST_SHIFT)) & USBC_BURSTSIZE_TXPBURST_MASK)

#define USBC_BURSTSIZE_UNUSED_16_MASK            (0xFFFF0000U)
#define USBC_BURSTSIZE_UNUSED_16_SHIFT           (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_BURSTSIZE_UNUSED_16(x)              (((uint32_t)(((uint32_t)(x)) << USBC_BURSTSIZE_UNUSED_16_SHIFT)) & USBC_BURSTSIZE_UNUSED_16_MASK)
/*! @} */

/*! @name TXFILLTUNING - TXFILLTUNING */
/*! @{ */

#define USBC_TXFILLTUNING_TXSCHOH_MASK           (0x7FU)
#define USBC_TXFILLTUNING_TXSCHOH_SHIFT          (0U)
/*! TXSCHOH - Only use in HOST & MPH mode */
#define USBC_TXFILLTUNING_TXSCHOH(x)             (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_TXSCHOH_SHIFT)) & USBC_TXFILLTUNING_TXSCHOH_MASK)

#define USBC_TXFILLTUNING_UNUSED_7_MASK          (0x80U)
#define USBC_TXFILLTUNING_UNUSED_7_SHIFT         (7U)
/*! UNUSED_7 - UNUSED_7 */
#define USBC_TXFILLTUNING_UNUSED_7(x)            (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_UNUSED_7_SHIFT)) & USBC_TXFILLTUNING_UNUSED_7_MASK)

#define USBC_TXFILLTUNING_TXSCHHEALTH_MASK       (0x1F00U)
#define USBC_TXFILLTUNING_TXSCHHEALTH_SHIFT      (8U)
/*! TXSCHHEALTH - Only use in HOST & MPH mode, rwc */
#define USBC_TXFILLTUNING_TXSCHHEALTH(x)         (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_TXSCHHEALTH_SHIFT)) & USBC_TXFILLTUNING_TXSCHHEALTH_MASK)

#define USBC_TXFILLTUNING_UNUSED_13_MASK         (0xE000U)
#define USBC_TXFILLTUNING_UNUSED_13_SHIFT        (13U)
/*! UNUSED_13 - UNUSED_13 */
#define USBC_TXFILLTUNING_UNUSED_13(x)           (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_UNUSED_13_SHIFT)) & USBC_TXFILLTUNING_UNUSED_13_MASK)

#define USBC_TXFILLTUNING_TXFIFOTHRES_MASK       (0x3F0000U)
#define USBC_TXFILLTUNING_TXFIFOTHRES_SHIFT      (16U)
/*! TXFIFOTHRES - Only use in HOST & MPH mode */
#define USBC_TXFILLTUNING_TXFIFOTHRES(x)         (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_TXFIFOTHRES_SHIFT)) & USBC_TXFILLTUNING_TXFIFOTHRES_MASK)

#define USBC_TXFILLTUNING_UNUSED_22_MASK         (0xFFC00000U)
#define USBC_TXFILLTUNING_UNUSED_22_SHIFT        (22U)
/*! UNUSED_22 - UNUSED_22 */
#define USBC_TXFILLTUNING_UNUSED_22(x)           (((uint32_t)(((uint32_t)(x)) << USBC_TXFILLTUNING_UNUSED_22_SHIFT)) & USBC_TXFILLTUNING_UNUSED_22_MASK)
/*! @} */

/*! @name TXTTFILLTUNING - TXTTFILLTUNING */
/*! @{ */

#define USBC_TXTTFILLTUNING_TXTTSCHOH_MASK       (0x1FU)
#define USBC_TXTTFILLTUNING_TXTTSCHOH_SHIFT      (0U)
/*! TXTTSCHOH - Only use in HOST & MPH mode */
#define USBC_TXTTFILLTUNING_TXTTSCHOH(x)         (((uint32_t)(((uint32_t)(x)) << USBC_TXTTFILLTUNING_TXTTSCHOH_SHIFT)) & USBC_TXTTFILLTUNING_TXTTSCHOH_MASK)

#define USBC_TXTTFILLTUNING_UNUSED_5_MASK        (0xE0U)
#define USBC_TXTTFILLTUNING_UNUSED_5_SHIFT       (5U)
/*! UNUSED_5 - UNUSED_5 */
#define USBC_TXTTFILLTUNING_UNUSED_5(x)          (((uint32_t)(((uint32_t)(x)) << USBC_TXTTFILLTUNING_UNUSED_5_SHIFT)) & USBC_TXTTFILLTUNING_UNUSED_5_MASK)

#define USBC_TXTTFILLTUNING_TXTTSCHHEALTJ_MASK   (0x1F00U)
#define USBC_TXTTFILLTUNING_TXTTSCHHEALTJ_SHIFT  (8U)
/*! TXTTSCHHEALTJ - Only use in HOST & MPH mode, rwc */
#define USBC_TXTTFILLTUNING_TXTTSCHHEALTJ(x)     (((uint32_t)(((uint32_t)(x)) << USBC_TXTTFILLTUNING_TXTTSCHHEALTJ_SHIFT)) & USBC_TXTTFILLTUNING_TXTTSCHHEALTJ_MASK)

#define USBC_TXTTFILLTUNING_UNUSED_13_MASK       (0xFFFFE000U)
#define USBC_TXTTFILLTUNING_UNUSED_13_SHIFT      (13U)
/*! UNUSED_13 - UNUSED_13 */
#define USBC_TXTTFILLTUNING_UNUSED_13(x)         (((uint32_t)(((uint32_t)(x)) << USBC_TXTTFILLTUNING_UNUSED_13_SHIFT)) & USBC_TXTTFILLTUNING_UNUSED_13_MASK)
/*! @} */

/*! @name IC_USB - IC_USB */
/*! @{ */

#define USBC_IC_USB_IC_VDD1_MASK                 (0x7U)
#define USBC_IC_USB_IC_VDD1_SHIFT                (0U)
/*! IC_VDD1 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD1(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD1_SHIFT)) & USBC_IC_USB_IC_VDD1_MASK)

#define USBC_IC_USB_IC1_MASK                     (0x8U)
#define USBC_IC_USB_IC1_SHIFT                    (3U)
/*! IC1 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC1(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC1_SHIFT)) & USBC_IC_USB_IC1_MASK)

#define USBC_IC_USB_IC_VDD2_MASK                 (0x70U)
#define USBC_IC_USB_IC_VDD2_SHIFT                (4U)
/*! IC_VDD2 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD2(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD2_SHIFT)) & USBC_IC_USB_IC_VDD2_MASK)

#define USBC_IC_USB_IC2_MASK                     (0x80U)
#define USBC_IC_USB_IC2_SHIFT                    (7U)
/*! IC2 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC2(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC2_SHIFT)) & USBC_IC_USB_IC2_MASK)

#define USBC_IC_USB_IC_VDD3_MASK                 (0x700U)
#define USBC_IC_USB_IC_VDD3_SHIFT                (8U)
/*! IC_VDD3 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD3(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD3_SHIFT)) & USBC_IC_USB_IC_VDD3_MASK)

#define USBC_IC_USB_IC3_MASK                     (0x800U)
#define USBC_IC_USB_IC3_SHIFT                    (11U)
/*! IC3 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC3(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC3_SHIFT)) & USBC_IC_USB_IC3_MASK)

#define USBC_IC_USB_IC_VDD4_MASK                 (0x7000U)
#define USBC_IC_USB_IC_VDD4_SHIFT                (12U)
/*! IC_VDD4 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD4(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD4_SHIFT)) & USBC_IC_USB_IC_VDD4_MASK)

#define USBC_IC_USB_IC4_MASK                     (0x8000U)
#define USBC_IC_USB_IC4_SHIFT                    (15U)
/*! IC4 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC4(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC4_SHIFT)) & USBC_IC_USB_IC4_MASK)

#define USBC_IC_USB_IC_VDD5_MASK                 (0x70000U)
#define USBC_IC_USB_IC_VDD5_SHIFT                (16U)
/*! IC_VDD5 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD5(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD5_SHIFT)) & USBC_IC_USB_IC_VDD5_MASK)

#define USBC_IC_USB_IC5_MASK                     (0x80000U)
#define USBC_IC_USB_IC5_SHIFT                    (19U)
/*! IC5 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC5(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC5_SHIFT)) & USBC_IC_USB_IC5_MASK)

#define USBC_IC_USB_IC_VDD6_MASK                 (0x700000U)
#define USBC_IC_USB_IC_VDD6_SHIFT                (20U)
/*! IC_VDD6 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD6(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD6_SHIFT)) & USBC_IC_USB_IC_VDD6_MASK)

#define USBC_IC_USB_IC6_MASK                     (0x800000U)
#define USBC_IC_USB_IC6_SHIFT                    (23U)
/*! IC6 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC6(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC6_SHIFT)) & USBC_IC_USB_IC6_MASK)

#define USBC_IC_USB_IC_VDD7_MASK                 (0x7000000U)
#define USBC_IC_USB_IC_VDD7_SHIFT                (24U)
/*! IC_VDD7 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD7(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD7_SHIFT)) & USBC_IC_USB_IC_VDD7_MASK)

#define USBC_IC_USB_IC7_MASK                     (0x8000000U)
#define USBC_IC_USB_IC7_SHIFT                    (27U)
/*! IC7 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC7(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC7_SHIFT)) & USBC_IC_USB_IC7_MASK)

#define USBC_IC_USB_IC_VDD8_MASK                 (0x70000000U)
#define USBC_IC_USB_IC_VDD8_SHIFT                (28U)
/*! IC_VDD8 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC_VDD8(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC_VDD8_SHIFT)) & USBC_IC_USB_IC_VDD8_MASK)

#define USBC_IC_USB_IC8_MASK                     (0x80000000U)
#define USBC_IC_USB_IC8_SHIFT                    (31U)
/*! IC8 - available in MPH & VUSB_HS_PHY_IC_USB =1 */
#define USBC_IC_USB_IC8(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_IC_USB_IC8_SHIFT)) & USBC_IC_USB_IC8_MASK)
/*! @} */

/*! @name ULPI_VIEWPORT - ULPI_VIEWPORT */
/*! @{ */

#define USBC_ULPI_VIEWPORT_ULPIDATWR_MASK        (0xFFU)
#define USBC_ULPI_VIEWPORT_ULPIDATWR_SHIFT       (0U)
/*! ULPIDATWR - Not available */
#define USBC_ULPI_VIEWPORT_ULPIDATWR(x)          (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIDATWR_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIDATWR_MASK)

#define USBC_ULPI_VIEWPORT_ULPIDATRD_MASK        (0xFF00U)
#define USBC_ULPI_VIEWPORT_ULPIDATRD_SHIFT       (8U)
/*! ULPIDATRD - Not available */
#define USBC_ULPI_VIEWPORT_ULPIDATRD(x)          (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIDATRD_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIDATRD_MASK)

#define USBC_ULPI_VIEWPORT_ULPIADDR_MASK         (0xFF0000U)
#define USBC_ULPI_VIEWPORT_ULPIADDR_SHIFT        (16U)
/*! ULPIADDR - Not available */
#define USBC_ULPI_VIEWPORT_ULPIADDR(x)           (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIADDR_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIADDR_MASK)

#define USBC_ULPI_VIEWPORT_ULPIPORT_MASK         (0x7000000U)
#define USBC_ULPI_VIEWPORT_ULPIPORT_SHIFT        (24U)
/*! ULPIPORT - Not available */
#define USBC_ULPI_VIEWPORT_ULPIPORT(x)           (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIPORT_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIPORT_MASK)

#define USBC_ULPI_VIEWPORT_ULPISS_MASK           (0x8000000U)
#define USBC_ULPI_VIEWPORT_ULPISS_SHIFT          (27U)
/*! ULPISS - Not available */
#define USBC_ULPI_VIEWPORT_ULPISS(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPISS_SHIFT)) & USBC_ULPI_VIEWPORT_ULPISS_MASK)

#define USBC_ULPI_VIEWPORT_UNUSED_28_MASK        (0x10000000U)
#define USBC_ULPI_VIEWPORT_UNUSED_28_SHIFT       (28U)
/*! UNUSED_28 - Not available */
#define USBC_ULPI_VIEWPORT_UNUSED_28(x)          (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_UNUSED_28_SHIFT)) & USBC_ULPI_VIEWPORT_UNUSED_28_MASK)

#define USBC_ULPI_VIEWPORT_ULPIRW_MASK           (0x20000000U)
#define USBC_ULPI_VIEWPORT_ULPIRW_SHIFT          (29U)
/*! ULPIRW - Not available */
#define USBC_ULPI_VIEWPORT_ULPIRW(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIRW_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIRW_MASK)

#define USBC_ULPI_VIEWPORT_ULPIRUN_MASK          (0x40000000U)
#define USBC_ULPI_VIEWPORT_ULPIRUN_SHIFT         (30U)
/*! ULPIRUN - Not available */
#define USBC_ULPI_VIEWPORT_ULPIRUN(x)            (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIRUN_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIRUN_MASK)

#define USBC_ULPI_VIEWPORT_ULPIWU_MASK           (0x80000000U)
#define USBC_ULPI_VIEWPORT_ULPIWU_SHIFT          (31U)
/*! ULPIWU - Not available */
#define USBC_ULPI_VIEWPORT_ULPIWU(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ULPI_VIEWPORT_ULPIWU_SHIFT)) & USBC_ULPI_VIEWPORT_ULPIWU_MASK)
/*! @} */

/*! @name ENDPTNAK - ENDPTNAK */
/*! @{ */

#define USBC_ENDPTNAK_EPRN_MASK                  (0xFFFFU)
#define USBC_ENDPTNAK_EPRN_SHIFT                 (0U)
/*! EPRN - rwc */
#define USBC_ENDPTNAK_EPRN(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTNAK_EPRN_SHIFT)) & USBC_ENDPTNAK_EPRN_MASK)

#define USBC_ENDPTNAK_EPTN_MASK                  (0xFFFF0000U)
#define USBC_ENDPTNAK_EPTN_SHIFT                 (16U)
/*! EPTN - rwc */
#define USBC_ENDPTNAK_EPTN(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTNAK_EPTN_SHIFT)) & USBC_ENDPTNAK_EPTN_MASK)
/*! @} */

/*! @name ENDPTNAKEN - ENDPTNAKEN */
/*! @{ */

#define USBC_ENDPTNAKEN_EPRNE_MASK               (0xFFFFU)
#define USBC_ENDPTNAKEN_EPRNE_SHIFT              (0U)
/*! EPRNE - Only 3 PHY max */
#define USBC_ENDPTNAKEN_EPRNE(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTNAKEN_EPRNE_SHIFT)) & USBC_ENDPTNAKEN_EPRNE_MASK)

#define USBC_ENDPTNAKEN_EPTNE_MASK               (0xFFFF0000U)
#define USBC_ENDPTNAKEN_EPTNE_SHIFT              (16U)
/*! EPTNE - Only 3 PHY max */
#define USBC_ENDPTNAKEN_EPTNE(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTNAKEN_EPTNE_SHIFT)) & USBC_ENDPTNAKEN_EPTNE_MASK)
/*! @} */

/*! @name PORTSC1 - PORTSC1 */
/*! @{ */

#define USBC_PORTSC1_CCS_MASK                    (0x1U)
#define USBC_PORTSC1_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC1_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_CCS_SHIFT)) & USBC_PORTSC1_CCS_MASK)

#define USBC_PORTSC1_CSC_MASK                    (0x2U)
#define USBC_PORTSC1_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC1_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_CSC_SHIFT)) & USBC_PORTSC1_CSC_MASK)

#define USBC_PORTSC1_PE_MASK                     (0x4U)
#define USBC_PORTSC1_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC1_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PE_SHIFT)) & USBC_PORTSC1_PE_MASK)

#define USBC_PORTSC1_PEC_MASK                    (0x8U)
#define USBC_PORTSC1_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC1_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PEC_SHIFT)) & USBC_PORTSC1_PEC_MASK)

#define USBC_PORTSC1_OCA_MASK                    (0x10U)
#define USBC_PORTSC1_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC1_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_OCA_SHIFT)) & USBC_PORTSC1_OCA_MASK)

#define USBC_PORTSC1_OCC_MASK                    (0x20U)
#define USBC_PORTSC1_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC1_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_OCC_SHIFT)) & USBC_PORTSC1_OCC_MASK)

#define USBC_PORTSC1_FPR_MASK                    (0x40U)
#define USBC_PORTSC1_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC1_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_FPR_SHIFT)) & USBC_PORTSC1_FPR_MASK)

#define USBC_PORTSC1_SUSP_MASK                   (0x80U)
#define USBC_PORTSC1_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC1_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_SUSP_SHIFT)) & USBC_PORTSC1_SUSP_MASK)

#define USBC_PORTSC1_PR_MASK                     (0x100U)
#define USBC_PORTSC1_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC1_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PR_SHIFT)) & USBC_PORTSC1_PR_MASK)

#define USBC_PORTSC1_HSP_MASK                    (0x200U)
#define USBC_PORTSC1_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC1_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_HSP_SHIFT)) & USBC_PORTSC1_HSP_MASK)

#define USBC_PORTSC1_LS_MASK                     (0xC00U)
#define USBC_PORTSC1_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC1_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_LS_SHIFT)) & USBC_PORTSC1_LS_MASK)

#define USBC_PORTSC1_PP_MASK                     (0x1000U)
#define USBC_PORTSC1_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC1_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PP_SHIFT)) & USBC_PORTSC1_PP_MASK)

#define USBC_PORTSC1_PO_MASK                     (0x2000U)
#define USBC_PORTSC1_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC1_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PO_SHIFT)) & USBC_PORTSC1_PO_MASK)

#define USBC_PORTSC1_PIC_MASK                    (0xC000U)
#define USBC_PORTSC1_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC1_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PIC_SHIFT)) & USBC_PORTSC1_PIC_MASK)

#define USBC_PORTSC1_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC1_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC1_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PTC_SHIFT)) & USBC_PORTSC1_PTC_MASK)

#define USBC_PORTSC1_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC1_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC1_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_WKCN_SHIFT)) & USBC_PORTSC1_WKCN_MASK)

#define USBC_PORTSC1_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC1_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC1_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_WKDS_SHIFT)) & USBC_PORTSC1_WKDS_MASK)

#define USBC_PORTSC1_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC1_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC1_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_WKOC_SHIFT)) & USBC_PORTSC1_WKOC_MASK)

#define USBC_PORTSC1_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC1_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC1_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PHCD_SHIFT)) & USBC_PORTSC1_PHCD_MASK)

#define USBC_PORTSC1_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC1_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC1_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PFSC_SHIFT)) & USBC_PORTSC1_PFSC_MASK)

#define USBC_PORTSC1_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC1_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC1_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PTS2_SHIFT)) & USBC_PORTSC1_PTS2_MASK)

#define USBC_PORTSC1_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC1_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC1_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PSPD_SHIFT)) & USBC_PORTSC1_PSPD_MASK)

#define USBC_PORTSC1_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC1_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC1_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PTW_SHIFT)) & USBC_PORTSC1_PTW_MASK)

#define USBC_PORTSC1_STS_MASK                    (0x20000000U)
#define USBC_PORTSC1_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC1_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_STS_SHIFT)) & USBC_PORTSC1_STS_MASK)

#define USBC_PORTSC1_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC1_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC1_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC1_PTS_SHIFT)) & USBC_PORTSC1_PTS_MASK)
/*! @} */

/*! @name PORTSC2 - PORTSC2 */
/*! @{ */

#define USBC_PORTSC2_CCS_MASK                    (0x1U)
#define USBC_PORTSC2_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC2_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_CCS_SHIFT)) & USBC_PORTSC2_CCS_MASK)

#define USBC_PORTSC2_CSC_MASK                    (0x2U)
#define USBC_PORTSC2_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC2_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_CSC_SHIFT)) & USBC_PORTSC2_CSC_MASK)

#define USBC_PORTSC2_PE_MASK                     (0x4U)
#define USBC_PORTSC2_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC2_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PE_SHIFT)) & USBC_PORTSC2_PE_MASK)

#define USBC_PORTSC2_PEC_MASK                    (0x8U)
#define USBC_PORTSC2_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC2_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PEC_SHIFT)) & USBC_PORTSC2_PEC_MASK)

#define USBC_PORTSC2_OCA_MASK                    (0x10U)
#define USBC_PORTSC2_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC2_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_OCA_SHIFT)) & USBC_PORTSC2_OCA_MASK)

#define USBC_PORTSC2_OCC_MASK                    (0x20U)
#define USBC_PORTSC2_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC2_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_OCC_SHIFT)) & USBC_PORTSC2_OCC_MASK)

#define USBC_PORTSC2_FPR_MASK                    (0x40U)
#define USBC_PORTSC2_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC2_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_FPR_SHIFT)) & USBC_PORTSC2_FPR_MASK)

#define USBC_PORTSC2_SUSP_MASK                   (0x80U)
#define USBC_PORTSC2_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC2_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_SUSP_SHIFT)) & USBC_PORTSC2_SUSP_MASK)

#define USBC_PORTSC2_PR_MASK                     (0x100U)
#define USBC_PORTSC2_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC2_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PR_SHIFT)) & USBC_PORTSC2_PR_MASK)

#define USBC_PORTSC2_HSP_MASK                    (0x200U)
#define USBC_PORTSC2_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC2_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_HSP_SHIFT)) & USBC_PORTSC2_HSP_MASK)

#define USBC_PORTSC2_LS_MASK                     (0xC00U)
#define USBC_PORTSC2_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC2_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_LS_SHIFT)) & USBC_PORTSC2_LS_MASK)

#define USBC_PORTSC2_PP_MASK                     (0x1000U)
#define USBC_PORTSC2_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC2_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PP_SHIFT)) & USBC_PORTSC2_PP_MASK)

#define USBC_PORTSC2_PO_MASK                     (0x2000U)
#define USBC_PORTSC2_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC2_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PO_SHIFT)) & USBC_PORTSC2_PO_MASK)

#define USBC_PORTSC2_PIC_MASK                    (0xC000U)
#define USBC_PORTSC2_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC2_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PIC_SHIFT)) & USBC_PORTSC2_PIC_MASK)

#define USBC_PORTSC2_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC2_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC2_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PTC_SHIFT)) & USBC_PORTSC2_PTC_MASK)

#define USBC_PORTSC2_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC2_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC2_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_WKCN_SHIFT)) & USBC_PORTSC2_WKCN_MASK)

#define USBC_PORTSC2_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC2_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC2_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_WKDS_SHIFT)) & USBC_PORTSC2_WKDS_MASK)

#define USBC_PORTSC2_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC2_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC2_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_WKOC_SHIFT)) & USBC_PORTSC2_WKOC_MASK)

#define USBC_PORTSC2_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC2_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC2_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PHCD_SHIFT)) & USBC_PORTSC2_PHCD_MASK)

#define USBC_PORTSC2_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC2_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC2_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PFSC_SHIFT)) & USBC_PORTSC2_PFSC_MASK)

#define USBC_PORTSC2_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC2_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC2_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PTS2_SHIFT)) & USBC_PORTSC2_PTS2_MASK)

#define USBC_PORTSC2_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC2_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC2_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PSPD_SHIFT)) & USBC_PORTSC2_PSPD_MASK)

#define USBC_PORTSC2_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC2_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC2_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PTW_SHIFT)) & USBC_PORTSC2_PTW_MASK)

#define USBC_PORTSC2_STS_MASK                    (0x20000000U)
#define USBC_PORTSC2_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC2_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_STS_SHIFT)) & USBC_PORTSC2_STS_MASK)

#define USBC_PORTSC2_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC2_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC2_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC2_PTS_SHIFT)) & USBC_PORTSC2_PTS_MASK)
/*! @} */

/*! @name PORTSC3 - PORTSC3 */
/*! @{ */

#define USBC_PORTSC3_CCS_MASK                    (0x1U)
#define USBC_PORTSC3_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC3_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_CCS_SHIFT)) & USBC_PORTSC3_CCS_MASK)

#define USBC_PORTSC3_CSC_MASK                    (0x2U)
#define USBC_PORTSC3_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC3_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_CSC_SHIFT)) & USBC_PORTSC3_CSC_MASK)

#define USBC_PORTSC3_PE_MASK                     (0x4U)
#define USBC_PORTSC3_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC3_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PE_SHIFT)) & USBC_PORTSC3_PE_MASK)

#define USBC_PORTSC3_PEC_MASK                    (0x8U)
#define USBC_PORTSC3_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC3_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PEC_SHIFT)) & USBC_PORTSC3_PEC_MASK)

#define USBC_PORTSC3_OCA_MASK                    (0x10U)
#define USBC_PORTSC3_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC3_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_OCA_SHIFT)) & USBC_PORTSC3_OCA_MASK)

#define USBC_PORTSC3_OCC_MASK                    (0x20U)
#define USBC_PORTSC3_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC3_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_OCC_SHIFT)) & USBC_PORTSC3_OCC_MASK)

#define USBC_PORTSC3_FPR_MASK                    (0x40U)
#define USBC_PORTSC3_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC3_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_FPR_SHIFT)) & USBC_PORTSC3_FPR_MASK)

#define USBC_PORTSC3_SUSP_MASK                   (0x80U)
#define USBC_PORTSC3_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC3_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_SUSP_SHIFT)) & USBC_PORTSC3_SUSP_MASK)

#define USBC_PORTSC3_PR_MASK                     (0x100U)
#define USBC_PORTSC3_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC3_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PR_SHIFT)) & USBC_PORTSC3_PR_MASK)

#define USBC_PORTSC3_HSP_MASK                    (0x200U)
#define USBC_PORTSC3_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC3_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_HSP_SHIFT)) & USBC_PORTSC3_HSP_MASK)

#define USBC_PORTSC3_LS_MASK                     (0xC00U)
#define USBC_PORTSC3_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC3_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_LS_SHIFT)) & USBC_PORTSC3_LS_MASK)

#define USBC_PORTSC3_PP_MASK                     (0x1000U)
#define USBC_PORTSC3_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC3_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PP_SHIFT)) & USBC_PORTSC3_PP_MASK)

#define USBC_PORTSC3_PO_MASK                     (0x2000U)
#define USBC_PORTSC3_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC3_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PO_SHIFT)) & USBC_PORTSC3_PO_MASK)

#define USBC_PORTSC3_PIC_MASK                    (0xC000U)
#define USBC_PORTSC3_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC3_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PIC_SHIFT)) & USBC_PORTSC3_PIC_MASK)

#define USBC_PORTSC3_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC3_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC3_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PTC_SHIFT)) & USBC_PORTSC3_PTC_MASK)

#define USBC_PORTSC3_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC3_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC3_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_WKCN_SHIFT)) & USBC_PORTSC3_WKCN_MASK)

#define USBC_PORTSC3_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC3_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC3_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_WKDS_SHIFT)) & USBC_PORTSC3_WKDS_MASK)

#define USBC_PORTSC3_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC3_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC3_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_WKOC_SHIFT)) & USBC_PORTSC3_WKOC_MASK)

#define USBC_PORTSC3_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC3_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC3_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PHCD_SHIFT)) & USBC_PORTSC3_PHCD_MASK)

#define USBC_PORTSC3_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC3_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC3_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PFSC_SHIFT)) & USBC_PORTSC3_PFSC_MASK)

#define USBC_PORTSC3_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC3_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC3_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PTS2_SHIFT)) & USBC_PORTSC3_PTS2_MASK)

#define USBC_PORTSC3_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC3_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC3_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PSPD_SHIFT)) & USBC_PORTSC3_PSPD_MASK)

#define USBC_PORTSC3_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC3_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC3_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PTW_SHIFT)) & USBC_PORTSC3_PTW_MASK)

#define USBC_PORTSC3_STS_MASK                    (0x20000000U)
#define USBC_PORTSC3_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC3_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_STS_SHIFT)) & USBC_PORTSC3_STS_MASK)

#define USBC_PORTSC3_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC3_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC3_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC3_PTS_SHIFT)) & USBC_PORTSC3_PTS_MASK)
/*! @} */

/*! @name PORTSC4 - PORTSC4 */
/*! @{ */

#define USBC_PORTSC4_CCS_MASK                    (0x1U)
#define USBC_PORTSC4_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC4_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_CCS_SHIFT)) & USBC_PORTSC4_CCS_MASK)

#define USBC_PORTSC4_CSC_MASK                    (0x2U)
#define USBC_PORTSC4_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC4_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_CSC_SHIFT)) & USBC_PORTSC4_CSC_MASK)

#define USBC_PORTSC4_PE_MASK                     (0x4U)
#define USBC_PORTSC4_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC4_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PE_SHIFT)) & USBC_PORTSC4_PE_MASK)

#define USBC_PORTSC4_PEC_MASK                    (0x8U)
#define USBC_PORTSC4_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC4_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PEC_SHIFT)) & USBC_PORTSC4_PEC_MASK)

#define USBC_PORTSC4_OCA_MASK                    (0x10U)
#define USBC_PORTSC4_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC4_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_OCA_SHIFT)) & USBC_PORTSC4_OCA_MASK)

#define USBC_PORTSC4_OCC_MASK                    (0x20U)
#define USBC_PORTSC4_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC4_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_OCC_SHIFT)) & USBC_PORTSC4_OCC_MASK)

#define USBC_PORTSC4_FPR_MASK                    (0x40U)
#define USBC_PORTSC4_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC4_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_FPR_SHIFT)) & USBC_PORTSC4_FPR_MASK)

#define USBC_PORTSC4_SUSP_MASK                   (0x80U)
#define USBC_PORTSC4_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC4_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_SUSP_SHIFT)) & USBC_PORTSC4_SUSP_MASK)

#define USBC_PORTSC4_PR_MASK                     (0x100U)
#define USBC_PORTSC4_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC4_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PR_SHIFT)) & USBC_PORTSC4_PR_MASK)

#define USBC_PORTSC4_HSP_MASK                    (0x200U)
#define USBC_PORTSC4_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC4_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_HSP_SHIFT)) & USBC_PORTSC4_HSP_MASK)

#define USBC_PORTSC4_LS_MASK                     (0xC00U)
#define USBC_PORTSC4_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC4_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_LS_SHIFT)) & USBC_PORTSC4_LS_MASK)

#define USBC_PORTSC4_PP_MASK                     (0x1000U)
#define USBC_PORTSC4_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC4_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PP_SHIFT)) & USBC_PORTSC4_PP_MASK)

#define USBC_PORTSC4_PO_MASK                     (0x2000U)
#define USBC_PORTSC4_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC4_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PO_SHIFT)) & USBC_PORTSC4_PO_MASK)

#define USBC_PORTSC4_PIC_MASK                    (0xC000U)
#define USBC_PORTSC4_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC4_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PIC_SHIFT)) & USBC_PORTSC4_PIC_MASK)

#define USBC_PORTSC4_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC4_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC4_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PTC_SHIFT)) & USBC_PORTSC4_PTC_MASK)

#define USBC_PORTSC4_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC4_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC4_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_WKCN_SHIFT)) & USBC_PORTSC4_WKCN_MASK)

#define USBC_PORTSC4_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC4_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC4_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_WKDS_SHIFT)) & USBC_PORTSC4_WKDS_MASK)

#define USBC_PORTSC4_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC4_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC4_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_WKOC_SHIFT)) & USBC_PORTSC4_WKOC_MASK)

#define USBC_PORTSC4_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC4_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC4_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PHCD_SHIFT)) & USBC_PORTSC4_PHCD_MASK)

#define USBC_PORTSC4_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC4_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC4_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PFSC_SHIFT)) & USBC_PORTSC4_PFSC_MASK)

#define USBC_PORTSC4_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC4_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC4_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PTS2_SHIFT)) & USBC_PORTSC4_PTS2_MASK)

#define USBC_PORTSC4_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC4_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC4_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PSPD_SHIFT)) & USBC_PORTSC4_PSPD_MASK)

#define USBC_PORTSC4_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC4_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC4_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PTW_SHIFT)) & USBC_PORTSC4_PTW_MASK)

#define USBC_PORTSC4_STS_MASK                    (0x20000000U)
#define USBC_PORTSC4_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC4_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_STS_SHIFT)) & USBC_PORTSC4_STS_MASK)

#define USBC_PORTSC4_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC4_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC4_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC4_PTS_SHIFT)) & USBC_PORTSC4_PTS_MASK)
/*! @} */

/*! @name PORTSC5 - PORTSC5 */
/*! @{ */

#define USBC_PORTSC5_CCS_MASK                    (0x1U)
#define USBC_PORTSC5_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC5_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_CCS_SHIFT)) & USBC_PORTSC5_CCS_MASK)

#define USBC_PORTSC5_CSC_MASK                    (0x2U)
#define USBC_PORTSC5_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC5_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_CSC_SHIFT)) & USBC_PORTSC5_CSC_MASK)

#define USBC_PORTSC5_PE_MASK                     (0x4U)
#define USBC_PORTSC5_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC5_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PE_SHIFT)) & USBC_PORTSC5_PE_MASK)

#define USBC_PORTSC5_PEC_MASK                    (0x8U)
#define USBC_PORTSC5_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC5_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PEC_SHIFT)) & USBC_PORTSC5_PEC_MASK)

#define USBC_PORTSC5_OCA_MASK                    (0x10U)
#define USBC_PORTSC5_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC5_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_OCA_SHIFT)) & USBC_PORTSC5_OCA_MASK)

#define USBC_PORTSC5_OCC_MASK                    (0x20U)
#define USBC_PORTSC5_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC5_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_OCC_SHIFT)) & USBC_PORTSC5_OCC_MASK)

#define USBC_PORTSC5_FPR_MASK                    (0x40U)
#define USBC_PORTSC5_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC5_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_FPR_SHIFT)) & USBC_PORTSC5_FPR_MASK)

#define USBC_PORTSC5_SUSP_MASK                   (0x80U)
#define USBC_PORTSC5_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC5_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_SUSP_SHIFT)) & USBC_PORTSC5_SUSP_MASK)

#define USBC_PORTSC5_PR_MASK                     (0x100U)
#define USBC_PORTSC5_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC5_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PR_SHIFT)) & USBC_PORTSC5_PR_MASK)

#define USBC_PORTSC5_HSP_MASK                    (0x200U)
#define USBC_PORTSC5_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC5_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_HSP_SHIFT)) & USBC_PORTSC5_HSP_MASK)

#define USBC_PORTSC5_LS_MASK                     (0xC00U)
#define USBC_PORTSC5_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC5_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_LS_SHIFT)) & USBC_PORTSC5_LS_MASK)

#define USBC_PORTSC5_PP_MASK                     (0x1000U)
#define USBC_PORTSC5_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC5_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PP_SHIFT)) & USBC_PORTSC5_PP_MASK)

#define USBC_PORTSC5_PO_MASK                     (0x2000U)
#define USBC_PORTSC5_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC5_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PO_SHIFT)) & USBC_PORTSC5_PO_MASK)

#define USBC_PORTSC5_PIC_MASK                    (0xC000U)
#define USBC_PORTSC5_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC5_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PIC_SHIFT)) & USBC_PORTSC5_PIC_MASK)

#define USBC_PORTSC5_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC5_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC5_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PTC_SHIFT)) & USBC_PORTSC5_PTC_MASK)

#define USBC_PORTSC5_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC5_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC5_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_WKCN_SHIFT)) & USBC_PORTSC5_WKCN_MASK)

#define USBC_PORTSC5_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC5_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC5_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_WKDS_SHIFT)) & USBC_PORTSC5_WKDS_MASK)

#define USBC_PORTSC5_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC5_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC5_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_WKOC_SHIFT)) & USBC_PORTSC5_WKOC_MASK)

#define USBC_PORTSC5_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC5_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC5_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PHCD_SHIFT)) & USBC_PORTSC5_PHCD_MASK)

#define USBC_PORTSC5_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC5_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC5_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PFSC_SHIFT)) & USBC_PORTSC5_PFSC_MASK)

#define USBC_PORTSC5_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC5_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC5_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PTS2_SHIFT)) & USBC_PORTSC5_PTS2_MASK)

#define USBC_PORTSC5_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC5_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC5_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PSPD_SHIFT)) & USBC_PORTSC5_PSPD_MASK)

#define USBC_PORTSC5_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC5_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC5_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PTW_SHIFT)) & USBC_PORTSC5_PTW_MASK)

#define USBC_PORTSC5_STS_MASK                    (0x20000000U)
#define USBC_PORTSC5_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC5_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_STS_SHIFT)) & USBC_PORTSC5_STS_MASK)

#define USBC_PORTSC5_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC5_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC5_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC5_PTS_SHIFT)) & USBC_PORTSC5_PTS_MASK)
/*! @} */

/*! @name PORTSC6 - PORTSC6 */
/*! @{ */

#define USBC_PORTSC6_CCS_MASK                    (0x1U)
#define USBC_PORTSC6_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC6_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_CCS_SHIFT)) & USBC_PORTSC6_CCS_MASK)

#define USBC_PORTSC6_CSC_MASK                    (0x2U)
#define USBC_PORTSC6_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC6_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_CSC_SHIFT)) & USBC_PORTSC6_CSC_MASK)

#define USBC_PORTSC6_PE_MASK                     (0x4U)
#define USBC_PORTSC6_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC6_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PE_SHIFT)) & USBC_PORTSC6_PE_MASK)

#define USBC_PORTSC6_PEC_MASK                    (0x8U)
#define USBC_PORTSC6_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC6_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PEC_SHIFT)) & USBC_PORTSC6_PEC_MASK)

#define USBC_PORTSC6_OCA_MASK                    (0x10U)
#define USBC_PORTSC6_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC6_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_OCA_SHIFT)) & USBC_PORTSC6_OCA_MASK)

#define USBC_PORTSC6_OCC_MASK                    (0x20U)
#define USBC_PORTSC6_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC6_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_OCC_SHIFT)) & USBC_PORTSC6_OCC_MASK)

#define USBC_PORTSC6_FPR_MASK                    (0x40U)
#define USBC_PORTSC6_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC6_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_FPR_SHIFT)) & USBC_PORTSC6_FPR_MASK)

#define USBC_PORTSC6_SUSP_MASK                   (0x80U)
#define USBC_PORTSC6_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC6_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_SUSP_SHIFT)) & USBC_PORTSC6_SUSP_MASK)

#define USBC_PORTSC6_PR_MASK                     (0x100U)
#define USBC_PORTSC6_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC6_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PR_SHIFT)) & USBC_PORTSC6_PR_MASK)

#define USBC_PORTSC6_HSP_MASK                    (0x200U)
#define USBC_PORTSC6_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC6_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_HSP_SHIFT)) & USBC_PORTSC6_HSP_MASK)

#define USBC_PORTSC6_LS_MASK                     (0xC00U)
#define USBC_PORTSC6_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC6_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_LS_SHIFT)) & USBC_PORTSC6_LS_MASK)

#define USBC_PORTSC6_PP_MASK                     (0x1000U)
#define USBC_PORTSC6_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC6_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PP_SHIFT)) & USBC_PORTSC6_PP_MASK)

#define USBC_PORTSC6_PO_MASK                     (0x2000U)
#define USBC_PORTSC6_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC6_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PO_SHIFT)) & USBC_PORTSC6_PO_MASK)

#define USBC_PORTSC6_PIC_MASK                    (0xC000U)
#define USBC_PORTSC6_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC6_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PIC_SHIFT)) & USBC_PORTSC6_PIC_MASK)

#define USBC_PORTSC6_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC6_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC6_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PTC_SHIFT)) & USBC_PORTSC6_PTC_MASK)

#define USBC_PORTSC6_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC6_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC6_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_WKCN_SHIFT)) & USBC_PORTSC6_WKCN_MASK)

#define USBC_PORTSC6_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC6_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC6_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_WKDS_SHIFT)) & USBC_PORTSC6_WKDS_MASK)

#define USBC_PORTSC6_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC6_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC6_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_WKOC_SHIFT)) & USBC_PORTSC6_WKOC_MASK)

#define USBC_PORTSC6_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC6_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC6_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PHCD_SHIFT)) & USBC_PORTSC6_PHCD_MASK)

#define USBC_PORTSC6_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC6_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC6_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PFSC_SHIFT)) & USBC_PORTSC6_PFSC_MASK)

#define USBC_PORTSC6_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC6_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC6_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PTS2_SHIFT)) & USBC_PORTSC6_PTS2_MASK)

#define USBC_PORTSC6_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC6_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC6_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PSPD_SHIFT)) & USBC_PORTSC6_PSPD_MASK)

#define USBC_PORTSC6_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC6_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC6_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PTW_SHIFT)) & USBC_PORTSC6_PTW_MASK)

#define USBC_PORTSC6_STS_MASK                    (0x20000000U)
#define USBC_PORTSC6_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC6_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_STS_SHIFT)) & USBC_PORTSC6_STS_MASK)

#define USBC_PORTSC6_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC6_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC6_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC6_PTS_SHIFT)) & USBC_PORTSC6_PTS_MASK)
/*! @} */

/*! @name PORTSC7 - PORTSC7 */
/*! @{ */

#define USBC_PORTSC7_CCS_MASK                    (0x1U)
#define USBC_PORTSC7_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC7_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_CCS_SHIFT)) & USBC_PORTSC7_CCS_MASK)

#define USBC_PORTSC7_CSC_MASK                    (0x2U)
#define USBC_PORTSC7_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC7_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_CSC_SHIFT)) & USBC_PORTSC7_CSC_MASK)

#define USBC_PORTSC7_PE_MASK                     (0x4U)
#define USBC_PORTSC7_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC7_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PE_SHIFT)) & USBC_PORTSC7_PE_MASK)

#define USBC_PORTSC7_PEC_MASK                    (0x8U)
#define USBC_PORTSC7_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC7_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PEC_SHIFT)) & USBC_PORTSC7_PEC_MASK)

#define USBC_PORTSC7_OCA_MASK                    (0x10U)
#define USBC_PORTSC7_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC7_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_OCA_SHIFT)) & USBC_PORTSC7_OCA_MASK)

#define USBC_PORTSC7_OCC_MASK                    (0x20U)
#define USBC_PORTSC7_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC7_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_OCC_SHIFT)) & USBC_PORTSC7_OCC_MASK)

#define USBC_PORTSC7_FPR_MASK                    (0x40U)
#define USBC_PORTSC7_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC7_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_FPR_SHIFT)) & USBC_PORTSC7_FPR_MASK)

#define USBC_PORTSC7_SUSP_MASK                   (0x80U)
#define USBC_PORTSC7_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC7_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_SUSP_SHIFT)) & USBC_PORTSC7_SUSP_MASK)

#define USBC_PORTSC7_PR_MASK                     (0x100U)
#define USBC_PORTSC7_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC7_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PR_SHIFT)) & USBC_PORTSC7_PR_MASK)

#define USBC_PORTSC7_HSP_MASK                    (0x200U)
#define USBC_PORTSC7_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC7_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_HSP_SHIFT)) & USBC_PORTSC7_HSP_MASK)

#define USBC_PORTSC7_LS_MASK                     (0xC00U)
#define USBC_PORTSC7_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC7_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_LS_SHIFT)) & USBC_PORTSC7_LS_MASK)

#define USBC_PORTSC7_PP_MASK                     (0x1000U)
#define USBC_PORTSC7_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC7_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PP_SHIFT)) & USBC_PORTSC7_PP_MASK)

#define USBC_PORTSC7_PO_MASK                     (0x2000U)
#define USBC_PORTSC7_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC7_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PO_SHIFT)) & USBC_PORTSC7_PO_MASK)

#define USBC_PORTSC7_PIC_MASK                    (0xC000U)
#define USBC_PORTSC7_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC7_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PIC_SHIFT)) & USBC_PORTSC7_PIC_MASK)

#define USBC_PORTSC7_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC7_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC7_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PTC_SHIFT)) & USBC_PORTSC7_PTC_MASK)

#define USBC_PORTSC7_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC7_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC7_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_WKCN_SHIFT)) & USBC_PORTSC7_WKCN_MASK)

#define USBC_PORTSC7_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC7_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC7_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_WKDS_SHIFT)) & USBC_PORTSC7_WKDS_MASK)

#define USBC_PORTSC7_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC7_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC7_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_WKOC_SHIFT)) & USBC_PORTSC7_WKOC_MASK)

#define USBC_PORTSC7_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC7_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC7_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PHCD_SHIFT)) & USBC_PORTSC7_PHCD_MASK)

#define USBC_PORTSC7_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC7_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC7_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PFSC_SHIFT)) & USBC_PORTSC7_PFSC_MASK)

#define USBC_PORTSC7_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC7_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC7_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PTS2_SHIFT)) & USBC_PORTSC7_PTS2_MASK)

#define USBC_PORTSC7_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC7_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC7_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PSPD_SHIFT)) & USBC_PORTSC7_PSPD_MASK)

#define USBC_PORTSC7_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC7_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC7_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PTW_SHIFT)) & USBC_PORTSC7_PTW_MASK)

#define USBC_PORTSC7_STS_MASK                    (0x20000000U)
#define USBC_PORTSC7_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC7_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_STS_SHIFT)) & USBC_PORTSC7_STS_MASK)

#define USBC_PORTSC7_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC7_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC7_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC7_PTS_SHIFT)) & USBC_PORTSC7_PTS_MASK)
/*! @} */

/*! @name PORTSC8 - PORTSC8 */
/*! @{ */

#define USBC_PORTSC8_CCS_MASK                    (0x1U)
#define USBC_PORTSC8_CCS_SHIFT                   (0U)
/*! CCS - CCS */
#define USBC_PORTSC8_CCS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_CCS_SHIFT)) & USBC_PORTSC8_CCS_MASK)

#define USBC_PORTSC8_CSC_MASK                    (0x2U)
#define USBC_PORTSC8_CSC_SHIFT                   (1U)
/*! CSC - rwc */
#define USBC_PORTSC8_CSC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_CSC_SHIFT)) & USBC_PORTSC8_CSC_MASK)

#define USBC_PORTSC8_PE_MASK                     (0x4U)
#define USBC_PORTSC8_PE_SHIFT                    (2U)
/*! PE - rwc */
#define USBC_PORTSC8_PE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PE_SHIFT)) & USBC_PORTSC8_PE_MASK)

#define USBC_PORTSC8_PEC_MASK                    (0x8U)
#define USBC_PORTSC8_PEC_SHIFT                   (3U)
/*! PEC - rwc */
#define USBC_PORTSC8_PEC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PEC_SHIFT)) & USBC_PORTSC8_PEC_MASK)

#define USBC_PORTSC8_OCA_MASK                    (0x10U)
#define USBC_PORTSC8_OCA_SHIFT                   (4U)
/*! OCA - OCA */
#define USBC_PORTSC8_OCA(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_OCA_SHIFT)) & USBC_PORTSC8_OCA_MASK)

#define USBC_PORTSC8_OCC_MASK                    (0x20U)
#define USBC_PORTSC8_OCC_SHIFT                   (5U)
/*! OCC - OCC */
#define USBC_PORTSC8_OCC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_OCC_SHIFT)) & USBC_PORTSC8_OCC_MASK)

#define USBC_PORTSC8_FPR_MASK                    (0x40U)
#define USBC_PORTSC8_FPR_SHIFT                   (6U)
/*! FPR - FPR */
#define USBC_PORTSC8_FPR(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_FPR_SHIFT)) & USBC_PORTSC8_FPR_MASK)

#define USBC_PORTSC8_SUSP_MASK                   (0x80U)
#define USBC_PORTSC8_SUSP_SHIFT                  (7U)
/*! SUSP - SUSP */
#define USBC_PORTSC8_SUSP(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_SUSP_SHIFT)) & USBC_PORTSC8_SUSP_MASK)

#define USBC_PORTSC8_PR_MASK                     (0x100U)
#define USBC_PORTSC8_PR_SHIFT                    (8U)
/*! PR - PR */
#define USBC_PORTSC8_PR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PR_SHIFT)) & USBC_PORTSC8_PR_MASK)

#define USBC_PORTSC8_HSP_MASK                    (0x200U)
#define USBC_PORTSC8_HSP_SHIFT                   (9U)
/*! HSP - HSP */
#define USBC_PORTSC8_HSP(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_HSP_SHIFT)) & USBC_PORTSC8_HSP_MASK)

#define USBC_PORTSC8_LS_MASK                     (0xC00U)
#define USBC_PORTSC8_LS_SHIFT                    (10U)
/*! LS - LS */
#define USBC_PORTSC8_LS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_LS_SHIFT)) & USBC_PORTSC8_LS_MASK)

#define USBC_PORTSC8_PP_MASK                     (0x1000U)
#define USBC_PORTSC8_PP_SHIFT                    (12U)
/*! PP - PP */
#define USBC_PORTSC8_PP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PP_SHIFT)) & USBC_PORTSC8_PP_MASK)

#define USBC_PORTSC8_PO_MASK                     (0x2000U)
#define USBC_PORTSC8_PO_SHIFT                    (13U)
/*! PO - PO */
#define USBC_PORTSC8_PO(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PO_SHIFT)) & USBC_PORTSC8_PO_MASK)

#define USBC_PORTSC8_PIC_MASK                    (0xC000U)
#define USBC_PORTSC8_PIC_SHIFT                   (14U)
/*! PIC - PIC */
#define USBC_PORTSC8_PIC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PIC_SHIFT)) & USBC_PORTSC8_PIC_MASK)

#define USBC_PORTSC8_PTC_MASK                    (0xF0000U)
#define USBC_PORTSC8_PTC_SHIFT                   (16U)
/*! PTC - PTC */
#define USBC_PORTSC8_PTC(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PTC_SHIFT)) & USBC_PORTSC8_PTC_MASK)

#define USBC_PORTSC8_WKCN_MASK                   (0x100000U)
#define USBC_PORTSC8_WKCN_SHIFT                  (20U)
/*! WKCN - WKCN */
#define USBC_PORTSC8_WKCN(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_WKCN_SHIFT)) & USBC_PORTSC8_WKCN_MASK)

#define USBC_PORTSC8_WKDS_MASK                   (0x200000U)
#define USBC_PORTSC8_WKDS_SHIFT                  (21U)
/*! WKDS - WKDS */
#define USBC_PORTSC8_WKDS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_WKDS_SHIFT)) & USBC_PORTSC8_WKDS_MASK)

#define USBC_PORTSC8_WKOC_MASK                   (0x400000U)
#define USBC_PORTSC8_WKOC_SHIFT                  (22U)
/*! WKOC - WKOC */
#define USBC_PORTSC8_WKOC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_WKOC_SHIFT)) & USBC_PORTSC8_WKOC_MASK)

#define USBC_PORTSC8_PHCD_MASK                   (0x800000U)
#define USBC_PORTSC8_PHCD_SHIFT                  (23U)
/*! PHCD - PHCD */
#define USBC_PORTSC8_PHCD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PHCD_SHIFT)) & USBC_PORTSC8_PHCD_MASK)

#define USBC_PORTSC8_PFSC_MASK                   (0x1000000U)
#define USBC_PORTSC8_PFSC_SHIFT                  (24U)
/*! PFSC - PFSC */
#define USBC_PORTSC8_PFSC(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PFSC_SHIFT)) & USBC_PORTSC8_PFSC_MASK)

#define USBC_PORTSC8_PTS2_MASK                   (0x2000000U)
#define USBC_PORTSC8_PTS2_SHIFT                  (25U)
/*! PTS2 - PTS2 */
#define USBC_PORTSC8_PTS2(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PTS2_SHIFT)) & USBC_PORTSC8_PTS2_MASK)

#define USBC_PORTSC8_PSPD_MASK                   (0xC000000U)
#define USBC_PORTSC8_PSPD_SHIFT                  (26U)
/*! PSPD - PSPD */
#define USBC_PORTSC8_PSPD(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PSPD_SHIFT)) & USBC_PORTSC8_PSPD_MASK)

#define USBC_PORTSC8_PTW_MASK                    (0x10000000U)
#define USBC_PORTSC8_PTW_SHIFT                   (28U)
/*! PTW - PTW */
#define USBC_PORTSC8_PTW(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PTW_SHIFT)) & USBC_PORTSC8_PTW_MASK)

#define USBC_PORTSC8_STS_MASK                    (0x20000000U)
#define USBC_PORTSC8_STS_SHIFT                   (29U)
/*! STS - STS */
#define USBC_PORTSC8_STS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_STS_SHIFT)) & USBC_PORTSC8_STS_MASK)

#define USBC_PORTSC8_PTS_MASK                    (0xC0000000U)
#define USBC_PORTSC8_PTS_SHIFT                   (30U)
/*! PTS - PTS */
#define USBC_PORTSC8_PTS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_PORTSC8_PTS_SHIFT)) & USBC_PORTSC8_PTS_MASK)
/*! @} */

/*! @name OTGSC - OTGSC */
/*! @{ */

#define USBC_OTGSC_VD_MASK                       (0x1U)
#define USBC_OTGSC_VD_SHIFT                      (0U)
/*! VD - OTG not enable */
#define USBC_OTGSC_VD(x)                         (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_VD_SHIFT)) & USBC_OTGSC_VD_MASK)

#define USBC_OTGSC_VC_MASK                       (0x2U)
#define USBC_OTGSC_VC_SHIFT                      (1U)
/*! VC - OTG not enable */
#define USBC_OTGSC_VC(x)                         (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_VC_SHIFT)) & USBC_OTGSC_VC_MASK)

#define USBC_OTGSC_HAAR_MASK                     (0x4U)
#define USBC_OTGSC_HAAR_SHIFT                    (2U)
/*! HAAR - OTG not enable */
#define USBC_OTGSC_HAAR(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_HAAR_SHIFT)) & USBC_OTGSC_HAAR_MASK)

#define USBC_OTGSC_OT_MASK                       (0x8U)
#define USBC_OTGSC_OT_SHIFT                      (3U)
/*! OT - OTG not enable */
#define USBC_OTGSC_OT(x)                         (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_OT_SHIFT)) & USBC_OTGSC_OT_MASK)

#define USBC_OTGSC_DP_MASK                       (0x10U)
#define USBC_OTGSC_DP_SHIFT                      (4U)
/*! DP - OTG not enable */
#define USBC_OTGSC_DP(x)                         (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_DP_SHIFT)) & USBC_OTGSC_DP_MASK)

#define USBC_OTGSC_IDPU_MASK                     (0x20U)
#define USBC_OTGSC_IDPU_SHIFT                    (5U)
/*! IDPU - OTG not enable */
#define USBC_OTGSC_IDPU(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_IDPU_SHIFT)) & USBC_OTGSC_IDPU_MASK)

#define USBC_OTGSC_HADP_MASK                     (0x40U)
#define USBC_OTGSC_HADP_SHIFT                    (6U)
/*! HADP - OTG not enable */
#define USBC_OTGSC_HADP(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_HADP_SHIFT)) & USBC_OTGSC_HADP_MASK)

#define USBC_OTGSC_HABA_MASK                     (0x80U)
#define USBC_OTGSC_HABA_SHIFT                    (7U)
/*! HABA - OTG not enable */
#define USBC_OTGSC_HABA(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_HABA_SHIFT)) & USBC_OTGSC_HABA_MASK)

#define USBC_OTGSC_ID_MASK                       (0x100U)
#define USBC_OTGSC_ID_SHIFT                      (8U)
/*! ID - OTG not enable */
#define USBC_OTGSC_ID(x)                         (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_ID_SHIFT)) & USBC_OTGSC_ID_MASK)

#define USBC_OTGSC_AVV_MASK                      (0x200U)
#define USBC_OTGSC_AVV_SHIFT                     (9U)
/*! AVV - OTG not enable */
#define USBC_OTGSC_AVV(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_AVV_SHIFT)) & USBC_OTGSC_AVV_MASK)

#define USBC_OTGSC_ASV_MASK                      (0x400U)
#define USBC_OTGSC_ASV_SHIFT                     (10U)
/*! ASV - OTG not enable */
#define USBC_OTGSC_ASV(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_ASV_SHIFT)) & USBC_OTGSC_ASV_MASK)

#define USBC_OTGSC_BSV_MASK                      (0x800U)
#define USBC_OTGSC_BSV_SHIFT                     (11U)
/*! BSV - OTG not enable */
#define USBC_OTGSC_BSV(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSV_SHIFT)) & USBC_OTGSC_BSV_MASK)

#define USBC_OTGSC_BSE_MASK                      (0x1000U)
#define USBC_OTGSC_BSE_SHIFT                     (12U)
/*! BSE - OTG not enable */
#define USBC_OTGSC_BSE(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSE_SHIFT)) & USBC_OTGSC_BSE_MASK)

#define USBC_OTGSC_OTGSC_1MST_MASK               (0x2000U)
#define USBC_OTGSC_OTGSC_1MST_SHIFT              (13U)
/*! OTGSC_1MST - OTG not enable */
#define USBC_OTGSC_OTGSC_1MST(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_OTGSC_1MST_SHIFT)) & USBC_OTGSC_OTGSC_1MST_MASK)

#define USBC_OTGSC_DPS_MASK                      (0x4000U)
#define USBC_OTGSC_DPS_SHIFT                     (14U)
/*! DPS - OTG not enable */
#define USBC_OTGSC_DPS(x)                        (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_DPS_SHIFT)) & USBC_OTGSC_DPS_MASK)

#define USBC_OTGSC_UNUSED_15_MASK                (0x8000U)
#define USBC_OTGSC_UNUSED_15_SHIFT               (15U)
/*! UNUSED_15 - OTG not enable */
#define USBC_OTGSC_UNUSED_15(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_UNUSED_15_SHIFT)) & USBC_OTGSC_UNUSED_15_MASK)

#define USBC_OTGSC_IDIS_MASK                     (0x10000U)
#define USBC_OTGSC_IDIS_SHIFT                    (16U)
/*! IDIS - rwc */
#define USBC_OTGSC_IDIS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_IDIS_SHIFT)) & USBC_OTGSC_IDIS_MASK)

#define USBC_OTGSC_AVVIS_MASK                    (0x20000U)
#define USBC_OTGSC_AVVIS_SHIFT                   (17U)
/*! AVVIS - rwc */
#define USBC_OTGSC_AVVIS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_AVVIS_SHIFT)) & USBC_OTGSC_AVVIS_MASK)

#define USBC_OTGSC_ASVIS_MASK                    (0x40000U)
#define USBC_OTGSC_ASVIS_SHIFT                   (18U)
/*! ASVIS - rwc */
#define USBC_OTGSC_ASVIS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_ASVIS_SHIFT)) & USBC_OTGSC_ASVIS_MASK)

#define USBC_OTGSC_BSVIS_MASK                    (0x80000U)
#define USBC_OTGSC_BSVIS_SHIFT                   (19U)
/*! BSVIS - rwc */
#define USBC_OTGSC_BSVIS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSVIS_SHIFT)) & USBC_OTGSC_BSVIS_MASK)

#define USBC_OTGSC_BSEIS_MASK                    (0x100000U)
#define USBC_OTGSC_BSEIS_SHIFT                   (20U)
/*! BSEIS - rwc */
#define USBC_OTGSC_BSEIS(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSEIS_SHIFT)) & USBC_OTGSC_BSEIS_MASK)

#define USBC_OTGSC_OTGSC_1MSS_MASK               (0x200000U)
#define USBC_OTGSC_OTGSC_1MSS_SHIFT              (21U)
/*! OTGSC_1MSS - rwc */
#define USBC_OTGSC_OTGSC_1MSS(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_OTGSC_1MSS_SHIFT)) & USBC_OTGSC_OTGSC_1MSS_MASK)

#define USBC_OTGSC_DPIS_MASK                     (0x400000U)
#define USBC_OTGSC_DPIS_SHIFT                    (22U)
/*! DPIS - rwc */
#define USBC_OTGSC_DPIS(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_DPIS_SHIFT)) & USBC_OTGSC_DPIS_MASK)

#define USBC_OTGSC_UNUSED_23_MASK                (0x800000U)
#define USBC_OTGSC_UNUSED_23_SHIFT               (23U)
/*! UNUSED_23 - OTG not enable */
#define USBC_OTGSC_UNUSED_23(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_UNUSED_23_SHIFT)) & USBC_OTGSC_UNUSED_23_MASK)

#define USBC_OTGSC_IDIE_MASK                     (0x1000000U)
#define USBC_OTGSC_IDIE_SHIFT                    (24U)
/*! IDIE - OTG not enable */
#define USBC_OTGSC_IDIE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_IDIE_SHIFT)) & USBC_OTGSC_IDIE_MASK)

#define USBC_OTGSC_AVVIE_MASK                    (0x2000000U)
#define USBC_OTGSC_AVVIE_SHIFT                   (25U)
/*! AVVIE - OTG not enable */
#define USBC_OTGSC_AVVIE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_AVVIE_SHIFT)) & USBC_OTGSC_AVVIE_MASK)

#define USBC_OTGSC_ASVIE_MASK                    (0x4000000U)
#define USBC_OTGSC_ASVIE_SHIFT                   (26U)
/*! ASVIE - OTG not enable */
#define USBC_OTGSC_ASVIE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_ASVIE_SHIFT)) & USBC_OTGSC_ASVIE_MASK)

#define USBC_OTGSC_BSVIE_MASK                    (0x8000000U)
#define USBC_OTGSC_BSVIE_SHIFT                   (27U)
/*! BSVIE - OTG not enable */
#define USBC_OTGSC_BSVIE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSVIE_SHIFT)) & USBC_OTGSC_BSVIE_MASK)

#define USBC_OTGSC_BSEIE_MASK                    (0x10000000U)
#define USBC_OTGSC_BSEIE_SHIFT                   (28U)
/*! BSEIE - OTG not enable */
#define USBC_OTGSC_BSEIE(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_BSEIE_SHIFT)) & USBC_OTGSC_BSEIE_MASK)

#define USBC_OTGSC_OTGSC_1MSE_MASK               (0x20000000U)
#define USBC_OTGSC_OTGSC_1MSE_SHIFT              (29U)
/*! OTGSC_1MSE - OTG not enable */
#define USBC_OTGSC_OTGSC_1MSE(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_OTGSC_1MSE_SHIFT)) & USBC_OTGSC_OTGSC_1MSE_MASK)

#define USBC_OTGSC_DPIE_MASK                     (0x40000000U)
#define USBC_OTGSC_DPIE_SHIFT                    (30U)
/*! DPIE - OTG not enable */
#define USBC_OTGSC_DPIE(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_DPIE_SHIFT)) & USBC_OTGSC_DPIE_MASK)

#define USBC_OTGSC_UNUSED_31_MASK                (0x80000000U)
#define USBC_OTGSC_UNUSED_31_SHIFT               (31U)
/*! UNUSED_31 - OTG not enable */
#define USBC_OTGSC_UNUSED_31(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_OTGSC_UNUSED_31_SHIFT)) & USBC_OTGSC_UNUSED_31_MASK)
/*! @} */

/*! @name USBMODE - USBMODE */
/*! @{ */

#define USBC_USBMODE_CM_MASK                     (0x3U)
#define USBC_USBMODE_CM_SHIFT                    (0U)
/*! CM - fix device mode */
#define USBC_USBMODE_CM(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_CM_SHIFT)) & USBC_USBMODE_CM_MASK)

#define USBC_USBMODE_ES_MASK                     (0x4U)
#define USBC_USBMODE_ES_SHIFT                    (2U)
/*! ES - ES */
#define USBC_USBMODE_ES(x)                       (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_ES_SHIFT)) & USBC_USBMODE_ES_MASK)

#define USBC_USBMODE_SLOM_MASK                   (0x8U)
#define USBC_USBMODE_SLOM_SHIFT                  (3U)
/*! SLOM - SLOM */
#define USBC_USBMODE_SLOM(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_SLOM_SHIFT)) & USBC_USBMODE_SLOM_MASK)

#define USBC_USBMODE_SDIS_MASK                   (0x10U)
#define USBC_USBMODE_SDIS_SHIFT                  (4U)
/*! SDIS - SDIS */
#define USBC_USBMODE_SDIS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_SDIS_SHIFT)) & USBC_USBMODE_SDIS_MASK)

#define USBC_USBMODE_VBPS_MASK                   (0x20U)
#define USBC_USBMODE_VBPS_SHIFT                  (5U)
/*! VBPS - Only used in Host */
#define USBC_USBMODE_VBPS(x)                     (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_VBPS_SHIFT)) & USBC_USBMODE_VBPS_MASK)

#define USBC_USBMODE_UNUSED_6_MASK               (0xFC0U)
#define USBC_USBMODE_UNUSED_6_SHIFT              (6U)
/*! UNUSED_6 - UNUSED_6 */
#define USBC_USBMODE_UNUSED_6(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_UNUSED_6_SHIFT)) & USBC_USBMODE_UNUSED_6_MASK)

#define USBC_USBMODE_TXHSD_MASK                  (0x7000U)
#define USBC_USBMODE_TXHSD_SHIFT                 (12U)
/*! TXHSD - TXHSD */
#define USBC_USBMODE_TXHSD(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_TXHSD_SHIFT)) & USBC_USBMODE_TXHSD_MASK)

#define USBC_USBMODE_SRT_MASK                    (0x8000U)
#define USBC_USBMODE_SRT_SHIFT                   (15U)
/*! SRT - SRT */
#define USBC_USBMODE_SRT(x)                      (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_SRT_SHIFT)) & USBC_USBMODE_SRT_MASK)

#define USBC_USBMODE_UNUSED_16_MASK              (0xFFFF0000U)
#define USBC_USBMODE_UNUSED_16_SHIFT             (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_USBMODE_UNUSED_16(x)                (((uint32_t)(((uint32_t)(x)) << USBC_USBMODE_UNUSED_16_SHIFT)) & USBC_USBMODE_UNUSED_16_MASK)
/*! @} */

/*! @name ENDPTSETUPSTAT - ENDPTSETUPSTAT */
/*! @{ */

#define USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK  (0xFFFFU)
#define USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT (0U)
/*! ENDPTSETUPSTAT - rwc */
#define USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT(x)    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT)) & USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK)

#define USBC_ENDPTSETUPSTAT_UNUSED_16_MASK       (0xFFFF0000U)
#define USBC_ENDPTSETUPSTAT_UNUSED_16_SHIFT      (16U)
/*! UNUSED_16 - UNUSED_16 */
#define USBC_ENDPTSETUPSTAT_UNUSED_16(x)         (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTSETUPSTAT_UNUSED_16_SHIFT)) & USBC_ENDPTSETUPSTAT_UNUSED_16_MASK)
/*! @} */

/*! @name ENDPTPRIME - ENDPTPRIME */
/*! @{ */

#define USBC_ENDPTPRIME_PERB_MASK                (0xFFFFU)
#define USBC_ENDPTPRIME_PERB_SHIFT               (0U)
/*! PERB - rws */
#define USBC_ENDPTPRIME_PERB(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTPRIME_PERB_SHIFT)) & USBC_ENDPTPRIME_PERB_MASK)

#define USBC_ENDPTPRIME_PETB_MASK                (0xFFFF0000U)
#define USBC_ENDPTPRIME_PETB_SHIFT               (16U)
/*! PETB - rws */
#define USBC_ENDPTPRIME_PETB(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTPRIME_PETB_SHIFT)) & USBC_ENDPTPRIME_PETB_MASK)
/*! @} */

/*! @name ENDPTFLUSH - ENDPTFLUSH */
/*! @{ */

#define USBC_ENDPTFLUSH_FERB_MASK                (0xFFFFU)
#define USBC_ENDPTFLUSH_FERB_SHIFT               (0U)
/*! FERB - rws */
#define USBC_ENDPTFLUSH_FERB(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTFLUSH_FERB_SHIFT)) & USBC_ENDPTFLUSH_FERB_MASK)

#define USBC_ENDPTFLUSH_FETB_MASK                (0xFFFF0000U)
#define USBC_ENDPTFLUSH_FETB_SHIFT               (16U)
/*! FETB - rws */
#define USBC_ENDPTFLUSH_FETB(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTFLUSH_FETB_SHIFT)) & USBC_ENDPTFLUSH_FETB_MASK)
/*! @} */

/*! @name ENDPTSTAT - ENDPTSTAT */
/*! @{ */

#define USBC_ENDPTSTAT_ERBR_MASK                 (0xFFFFU)
#define USBC_ENDPTSTAT_ERBR_SHIFT                (0U)
/*! ERBR - ERBR */
#define USBC_ENDPTSTAT_ERBR(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTSTAT_ERBR_SHIFT)) & USBC_ENDPTSTAT_ERBR_MASK)

#define USBC_ENDPTSTAT_ETBR_MASK                 (0xFFFF0000U)
#define USBC_ENDPTSTAT_ETBR_SHIFT                (16U)
/*! ETBR - ETBR */
#define USBC_ENDPTSTAT_ETBR(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTSTAT_ETBR_SHIFT)) & USBC_ENDPTSTAT_ETBR_MASK)
/*! @} */

/*! @name ENDPTCOMPLETE - ENDPTCOMPLETE */
/*! @{ */

#define USBC_ENDPTCOMPLETE_ERCE_MASK             (0xFFFFU)
#define USBC_ENDPTCOMPLETE_ERCE_SHIFT            (0U)
/*! ERCE - rwc */
#define USBC_ENDPTCOMPLETE_ERCE(x)               (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCOMPLETE_ERCE_SHIFT)) & USBC_ENDPTCOMPLETE_ERCE_MASK)

#define USBC_ENDPTCOMPLETE_ETCE_MASK             (0xFFFF0000U)
#define USBC_ENDPTCOMPLETE_ETCE_SHIFT            (16U)
/*! ETCE - rwc */
#define USBC_ENDPTCOMPLETE_ETCE(x)               (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCOMPLETE_ETCE_SHIFT)) & USBC_ENDPTCOMPLETE_ETCE_MASK)
/*! @} */

/*! @name ENDPTCTRL0 - ENDPTCTRL0 */
/*! @{ */

#define USBC_ENDPTCTRL0_RXS_MASK                 (0x1U)
#define USBC_ENDPTCTRL0_RXS_SHIFT                (0U)
/*! RXS - RXS */
#define USBC_ENDPTCTRL0_RXS(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_RXS_SHIFT)) & USBC_ENDPTCTRL0_RXS_MASK)

#define USBC_ENDPTCTRL0_UNUSED_1_MASK            (0x2U)
#define USBC_ENDPTCTRL0_UNUSED_1_SHIFT           (1U)
/*! UNUSED_1 - UNUSED_1 */
#define USBC_ENDPTCTRL0_UNUSED_1(x)              (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_1_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_1_MASK)

#define USBC_ENDPTCTRL0_RXT_MASK                 (0xCU)
#define USBC_ENDPTCTRL0_RXT_SHIFT                (2U)
/*! RXT - RXT */
#define USBC_ENDPTCTRL0_RXT(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_RXT_SHIFT)) & USBC_ENDPTCTRL0_RXT_MASK)

#define USBC_ENDPTCTRL0_UNUSED_4_MASK            (0x70U)
#define USBC_ENDPTCTRL0_UNUSED_4_SHIFT           (4U)
/*! UNUSED_4 - UNUSED_4 */
#define USBC_ENDPTCTRL0_UNUSED_4(x)              (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_4_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_4_MASK)

#define USBC_ENDPTCTRL0_RXE_MASK                 (0x80U)
#define USBC_ENDPTCTRL0_RXE_SHIFT                (7U)
/*! RXE - RXE */
#define USBC_ENDPTCTRL0_RXE(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_RXE_SHIFT)) & USBC_ENDPTCTRL0_RXE_MASK)

#define USBC_ENDPTCTRL0_UNUSED_8_MASK            (0xFF00U)
#define USBC_ENDPTCTRL0_UNUSED_8_SHIFT           (8U)
/*! UNUSED_8 - UNUSED_8 */
#define USBC_ENDPTCTRL0_UNUSED_8(x)              (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_8_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_8_MASK)

#define USBC_ENDPTCTRL0_TXS_MASK                 (0x10000U)
#define USBC_ENDPTCTRL0_TXS_SHIFT                (16U)
/*! TXS - TXS */
#define USBC_ENDPTCTRL0_TXS(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_TXS_SHIFT)) & USBC_ENDPTCTRL0_TXS_MASK)

#define USBC_ENDPTCTRL0_UNUSED_17_MASK           (0x20000U)
#define USBC_ENDPTCTRL0_UNUSED_17_SHIFT          (17U)
/*! UNUSED_17 - UNUSED_17 */
#define USBC_ENDPTCTRL0_UNUSED_17(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_17_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_17_MASK)

#define USBC_ENDPTCTRL0_TXT_MASK                 (0xC0000U)
#define USBC_ENDPTCTRL0_TXT_SHIFT                (18U)
/*! TXT - TXT */
#define USBC_ENDPTCTRL0_TXT(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_TXT_SHIFT)) & USBC_ENDPTCTRL0_TXT_MASK)

#define USBC_ENDPTCTRL0_UNUSED_20_MASK           (0x700000U)
#define USBC_ENDPTCTRL0_UNUSED_20_SHIFT          (20U)
/*! UNUSED_20 - UNUSED_20 */
#define USBC_ENDPTCTRL0_UNUSED_20(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_20_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_20_MASK)

#define USBC_ENDPTCTRL0_TXE_MASK                 (0x800000U)
#define USBC_ENDPTCTRL0_TXE_SHIFT                (23U)
/*! TXE - TXE */
#define USBC_ENDPTCTRL0_TXE(x)                   (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_TXE_SHIFT)) & USBC_ENDPTCTRL0_TXE_MASK)

#define USBC_ENDPTCTRL0_UNUSED_24_MASK           (0xFF000000U)
#define USBC_ENDPTCTRL0_UNUSED_24_SHIFT          (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_ENDPTCTRL0_UNUSED_24(x)             (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL0_UNUSED_24_SHIFT)) & USBC_ENDPTCTRL0_UNUSED_24_MASK)
/*! @} */

/*! @name ENDPTCTRL - ENDPTCTRL1..ENDPTCTRL15 */
/*! @{ */

#define USBC_ENDPTCTRL_RXS_MASK                  (0x1U)
#define USBC_ENDPTCTRL_RXS_SHIFT                 (0U)
/*! RXS - RXS */
#define USBC_ENDPTCTRL_RXS(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXS_SHIFT)) & USBC_ENDPTCTRL_RXS_MASK)

#define USBC_ENDPTCTRL_RXD_MASK                  (0x2U)
#define USBC_ENDPTCTRL_RXD_SHIFT                 (1U)
/*! RXD - RXD */
#define USBC_ENDPTCTRL_RXD(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXD_SHIFT)) & USBC_ENDPTCTRL_RXD_MASK)

#define USBC_ENDPTCTRL_RXT_MASK                  (0xCU)
#define USBC_ENDPTCTRL_RXT_SHIFT                 (2U)
/*! RXT - RXT */
#define USBC_ENDPTCTRL_RXT(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXT_SHIFT)) & USBC_ENDPTCTRL_RXT_MASK)

#define USBC_ENDPTCTRL_UNUSED_4_MASK             (0x10U)
#define USBC_ENDPTCTRL_UNUSED_4_SHIFT            (4U)
/*! UNUSED_4 - UNUSED_4 */
#define USBC_ENDPTCTRL_UNUSED_4(x)               (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_UNUSED_4_SHIFT)) & USBC_ENDPTCTRL_UNUSED_4_MASK)

#define USBC_ENDPTCTRL_RXI_MASK                  (0x20U)
#define USBC_ENDPTCTRL_RXI_SHIFT                 (5U)
/*! RXI - RXI */
#define USBC_ENDPTCTRL_RXI(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXI_SHIFT)) & USBC_ENDPTCTRL_RXI_MASK)

#define USBC_ENDPTCTRL_RXR_MASK                  (0x40U)
#define USBC_ENDPTCTRL_RXR_SHIFT                 (6U)
/*! RXR - ws */
#define USBC_ENDPTCTRL_RXR(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXR_SHIFT)) & USBC_ENDPTCTRL_RXR_MASK)

#define USBC_ENDPTCTRL_RXE_MASK                  (0x80U)
#define USBC_ENDPTCTRL_RXE_SHIFT                 (7U)
/*! RXE - RXE */
#define USBC_ENDPTCTRL_RXE(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_RXE_SHIFT)) & USBC_ENDPTCTRL_RXE_MASK)

#define USBC_ENDPTCTRL_UNUSED_8_MASK             (0xFF00U)
#define USBC_ENDPTCTRL_UNUSED_8_SHIFT            (8U)
/*! UNUSED_8 - UNUSED_8 */
#define USBC_ENDPTCTRL_UNUSED_8(x)               (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_UNUSED_8_SHIFT)) & USBC_ENDPTCTRL_UNUSED_8_MASK)

#define USBC_ENDPTCTRL_TXS_MASK                  (0x10000U)
#define USBC_ENDPTCTRL_TXS_SHIFT                 (16U)
/*! TXS - TXS */
#define USBC_ENDPTCTRL_TXS(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXS_SHIFT)) & USBC_ENDPTCTRL_TXS_MASK)

#define USBC_ENDPTCTRL_TXD_MASK                  (0x20000U)
#define USBC_ENDPTCTRL_TXD_SHIFT                 (17U)
/*! TXD - TXD */
#define USBC_ENDPTCTRL_TXD(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXD_SHIFT)) & USBC_ENDPTCTRL_TXD_MASK)

#define USBC_ENDPTCTRL_TXT_MASK                  (0xC0000U)
#define USBC_ENDPTCTRL_TXT_SHIFT                 (18U)
/*! TXT - TXT */
#define USBC_ENDPTCTRL_TXT(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXT_SHIFT)) & USBC_ENDPTCTRL_TXT_MASK)

#define USBC_ENDPTCTRL_UNUSED_20_MASK            (0x100000U)
#define USBC_ENDPTCTRL_UNUSED_20_SHIFT           (20U)
/*! UNUSED_20 - UNUSED_20 */
#define USBC_ENDPTCTRL_UNUSED_20(x)              (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_UNUSED_20_SHIFT)) & USBC_ENDPTCTRL_UNUSED_20_MASK)

#define USBC_ENDPTCTRL_TXI_MASK                  (0x200000U)
#define USBC_ENDPTCTRL_TXI_SHIFT                 (21U)
/*! TXI - TXI */
#define USBC_ENDPTCTRL_TXI(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXI_SHIFT)) & USBC_ENDPTCTRL_TXI_MASK)

#define USBC_ENDPTCTRL_TXR_MASK                  (0x400000U)
#define USBC_ENDPTCTRL_TXR_SHIFT                 (22U)
/*! TXR - ws */
#define USBC_ENDPTCTRL_TXR(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXR_SHIFT)) & USBC_ENDPTCTRL_TXR_MASK)

#define USBC_ENDPTCTRL_TXE_MASK                  (0x800000U)
#define USBC_ENDPTCTRL_TXE_SHIFT                 (23U)
/*! TXE - TXE */
#define USBC_ENDPTCTRL_TXE(x)                    (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_TXE_SHIFT)) & USBC_ENDPTCTRL_TXE_MASK)

#define USBC_ENDPTCTRL_UNUSED_24_MASK            (0xFF000000U)
#define USBC_ENDPTCTRL_UNUSED_24_SHIFT           (24U)
/*! UNUSED_24 - UNUSED_24 */
#define USBC_ENDPTCTRL_UNUSED_24(x)              (((uint32_t)(((uint32_t)(x)) << USBC_ENDPTCTRL_UNUSED_24_SHIFT)) & USBC_ENDPTCTRL_UNUSED_24_MASK)
/*! @} */

/*! @name PLL_CONTROL_0 - PLL_Control_0 */
/*! @{ */

#define USBC_PLL_CONTROL_0_REFDIV_MASK           (0x7FU)
#define USBC_PLL_CONTROL_0_REFDIV_SHIFT          (0U)
/*! REFDIV - REFDIV */
#define USBC_PLL_CONTROL_0_REFDIV(x)             (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_REFDIV_SHIFT)) & USBC_PLL_CONTROL_0_REFDIV_MASK)

#define USBC_PLL_CONTROL_0_UNUSED_7_MASK         (0x80U)
#define USBC_PLL_CONTROL_0_UNUSED_7_SHIFT        (7U)
/*! UNUSED_7 - Reserved */
#define USBC_PLL_CONTROL_0_UNUSED_7(x)           (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_UNUSED_7_SHIFT)) & USBC_PLL_CONTROL_0_UNUSED_7_MASK)

#define USBC_PLL_CONTROL_0_ICP_MASK              (0xF00U)
#define USBC_PLL_CONTROL_0_ICP_SHIFT             (8U)
/*! ICP - ICP */
#define USBC_PLL_CONTROL_0_ICP(x)                (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_ICP_SHIFT)) & USBC_PLL_CONTROL_0_ICP_MASK)

#define USBC_PLL_CONTROL_0_VDDM_MASK             (0x3000U)
#define USBC_PLL_CONTROL_0_VDDM_SHIFT            (12U)
/*! VDDM - VDDM */
#define USBC_PLL_CONTROL_0_VDDM(x)               (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_VDDM_SHIFT)) & USBC_PLL_CONTROL_0_VDDM_MASK)

#define USBC_PLL_CONTROL_0_VDDL_MASK             (0xC000U)
#define USBC_PLL_CONTROL_0_VDDL_SHIFT            (14U)
/*! VDDL - VDDL */
#define USBC_PLL_CONTROL_0_VDDL(x)               (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_VDDL_SHIFT)) & USBC_PLL_CONTROL_0_VDDL_MASK)

#define USBC_PLL_CONTROL_0_FBDIV_MASK            (0x1FF0000U)
#define USBC_PLL_CONTROL_0_FBDIV_SHIFT           (16U)
/*! FBDIV - FBDIV */
#define USBC_PLL_CONTROL_0_FBDIV(x)              (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_FBDIV_SHIFT)) & USBC_PLL_CONTROL_0_FBDIV_MASK)

#define USBC_PLL_CONTROL_0_UNUSED_25_MASK        (0xE000000U)
#define USBC_PLL_CONTROL_0_UNUSED_25_SHIFT       (25U)
/*! UNUSED_25 - Reserved */
#define USBC_PLL_CONTROL_0_UNUSED_25(x)          (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_UNUSED_25_SHIFT)) & USBC_PLL_CONTROL_0_UNUSED_25_MASK)

#define USBC_PLL_CONTROL_0_SEL_LPFR_MASK         (0x30000000U)
#define USBC_PLL_CONTROL_0_SEL_LPFR_SHIFT        (28U)
/*! SEL_LPFR - SEL_LPFR */
#define USBC_PLL_CONTROL_0_SEL_LPFR(x)           (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_SEL_LPFR_SHIFT)) & USBC_PLL_CONTROL_0_SEL_LPFR_MASK)

#define USBC_PLL_CONTROL_0_R_ROTATE_MASK         (0x40000000U)
#define USBC_PLL_CONTROL_0_R_ROTATE_SHIFT        (30U)
/*! R_ROTATE - R_ROTATE */
#define USBC_PLL_CONTROL_0_R_ROTATE(x)           (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_R_ROTATE_SHIFT)) & USBC_PLL_CONTROL_0_R_ROTATE_MASK)

#define USBC_PLL_CONTROL_0_PLL_READY_MASK        (0x80000000U)
#define USBC_PLL_CONTROL_0_PLL_READY_SHIFT       (31U)
/*! PLL_READY - PLL_READY */
#define USBC_PLL_CONTROL_0_PLL_READY(x)          (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_0_PLL_READY_SHIFT)) & USBC_PLL_CONTROL_0_PLL_READY_MASK)
/*! @} */

/*! @name PLL_CONTROL_1 - PLL_Control_1 */
/*! @{ */

#define USBC_PLL_CONTROL_1_PU_PLL_MASK           (0x1U)
#define USBC_PLL_CONTROL_1_PU_PLL_SHIFT          (0U)
/*! PU_PLL - PU_PLL */
#define USBC_PLL_CONTROL_1_PU_PLL(x)             (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_PU_PLL_SHIFT)) & USBC_PLL_CONTROL_1_PU_PLL_MASK)

#define USBC_PLL_CONTROL_1_PU_PLL_BY_REG_MASK    (0x2U)
#define USBC_PLL_CONTROL_1_PU_PLL_BY_REG_SHIFT   (1U)
/*! PU_PLL_BY_REG - PU_PLL_BY_REG */
#define USBC_PLL_CONTROL_1_PU_PLL_BY_REG(x)      (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_PU_PLL_BY_REG_SHIFT)) & USBC_PLL_CONTROL_1_PU_PLL_BY_REG_MASK)

#define USBC_PLL_CONTROL_1_PLL_RESET_MASK        (0x4U)
#define USBC_PLL_CONTROL_1_PLL_RESET_SHIFT       (2U)
/*! PLL_RESET - PLL_RESET */
#define USBC_PLL_CONTROL_1_PLL_RESET(x)          (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_PLL_RESET_SHIFT)) & USBC_PLL_CONTROL_1_PLL_RESET_MASK)

#define USBC_PLL_CONTROL_1_PLL_SUSPEND_EN_MASK   (0x8U)
#define USBC_PLL_CONTROL_1_PLL_SUSPEND_EN_SHIFT  (3U)
/*! PLL_SUSPEND_EN - PLL_SUSPEND_EN */
#define USBC_PLL_CONTROL_1_PLL_SUSPEND_EN(x)     (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_PLL_SUSPEND_EN_SHIFT)) & USBC_PLL_CONTROL_1_PLL_SUSPEND_EN_MASK)

#define USBC_PLL_CONTROL_1_TESTMON_PLL_MASK      (0xF0U)
#define USBC_PLL_CONTROL_1_TESTMON_PLL_SHIFT     (4U)
/*! TESTMON_PLL - TESTMON_PLL */
#define USBC_PLL_CONTROL_1_TESTMON_PLL(x)        (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_TESTMON_PLL_SHIFT)) & USBC_PLL_CONTROL_1_TESTMON_PLL_MASK)

#define USBC_PLL_CONTROL_1_UNUSED_8_MASK         (0x300U)
#define USBC_PLL_CONTROL_1_UNUSED_8_SHIFT        (8U)
/*! UNUSED_8 - Reserved */
#define USBC_PLL_CONTROL_1_UNUSED_8(x)           (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_UNUSED_8_SHIFT)) & USBC_PLL_CONTROL_1_UNUSED_8_MASK)

#define USBC_PLL_CONTROL_1_TXCLK_EN_MASK         (0x400U)
#define USBC_PLL_CONTROL_1_TXCLK_EN_SHIFT        (10U)
/*! TXCLK_EN - TXCLK_EN */
#define USBC_PLL_CONTROL_1_TXCLK_EN(x)           (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_TXCLK_EN_SHIFT)) & USBC_PLL_CONTROL_1_TXCLK_EN_MASK)

#define USBC_PLL_CONTROL_1_CLK160M_EN_MASK       (0x800U)
#define USBC_PLL_CONTROL_1_CLK160M_EN_SHIFT      (11U)
/*! CLK160M_EN - CLK160M_EN */
#define USBC_PLL_CONTROL_1_CLK160M_EN(x)         (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_CLK160M_EN_SHIFT)) & USBC_PLL_CONTROL_1_CLK160M_EN_MASK)

#define USBC_PLL_CONTROL_1_REFCLK_SEL_MASK       (0x1000U)
#define USBC_PLL_CONTROL_1_REFCLK_SEL_SHIFT      (12U)
/*! REFCLK_SEL - REFCLK_SEL */
#define USBC_PLL_CONTROL_1_REFCLK_SEL(x)         (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_REFCLK_SEL_SHIFT)) & USBC_PLL_CONTROL_1_REFCLK_SEL_MASK)

#define USBC_PLL_CONTROL_1_CLK_BLK_EN_MASK       (0x2000U)
#define USBC_PLL_CONTROL_1_CLK_BLK_EN_SHIFT      (13U)
/*! CLK_BLK_EN - CLK_BLK_EN */
#define USBC_PLL_CONTROL_1_CLK_BLK_EN(x)         (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_CLK_BLK_EN_SHIFT)) & USBC_PLL_CONTROL_1_CLK_BLK_EN_MASK)

#define USBC_PLL_CONTROL_1_PLL_STRESS_TEST_MASK  (0x4000U)
#define USBC_PLL_CONTROL_1_PLL_STRESS_TEST_SHIFT (14U)
/*! PLL_STRESS_TEST - PLL_STRESS_TEST */
#define USBC_PLL_CONTROL_1_PLL_STRESS_TEST(x)    (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_PLL_STRESS_TEST_SHIFT)) & USBC_PLL_CONTROL_1_PLL_STRESS_TEST_MASK)

#define USBC_PLL_CONTROL_1_UNUSED_15_MASK        (0xFFFF8000U)
#define USBC_PLL_CONTROL_1_UNUSED_15_SHIFT       (15U)
/*! UNUSED_15 - Reserved */
#define USBC_PLL_CONTROL_1_UNUSED_15(x)          (((uint32_t)(((uint32_t)(x)) << USBC_PLL_CONTROL_1_UNUSED_15_SHIFT)) & USBC_PLL_CONTROL_1_UNUSED_15_MASK)
/*! @} */

/*! @name CALIBRATION_CONTROL - CALIBRATION_Control */
/*! @{ */

#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP_MASK (0xFU)
#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP_SHIFT (0U)
/*! EXT_FS_IMP - Tx FS Driver Impedance
 *  0b0000..Impedance (SE) : 900/12 = 75 ohms
 *  0b0001..Impedance (SE) : 900/13 = 69 ohms
 *  0b0010..Impedance (SE) : 900/14 = 64 ohms
 *  0b0011..Impedance (SE) : 900/15 = 60 ohms
 *  0b0100..Impedance (SE) : 900/16 = 56 ohms
 *  0b0101..Impedance (SE) : 900/17 = 53 ohms
 *  0b0110..Impedance (SE) : 900/18 = 50 ohms
 *  0b0111..Impedance (SE) : 900/19 = 47 ohms
 *  0b1000..Impedance (SE) : 900/20 = 45 ohms
 *  0b1001..Impedance (SE) : 900/21 = 43 ohms
 *  0b1010..Impedance (SE) : 900/22 = 41 ohms
 *  0b1011..Impedance (SE) : 900/23 = 39 ohms
 *  0b1100..Impedance (SE) : 900/24 = 38 ohms
 *  0b1101..Impedance (SE) : 900/25 = 36 ohms
 *  0b1110..Impedance (SE) : 900/26 = 35 ohms
 *  0b1111..Impedance (SE) : 900/27 = 33 ohms
 */
#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP(x)   (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_EXT_FS_IMP_SHIFT)) & USBC_CALIBRATION_CONTROL_EXT_FS_IMP_MASK)

#define USBC_CALIBRATION_CONTROL_EXT_RCAL_MASK   (0xF0U)
#define USBC_CALIBRATION_CONTROL_EXT_RCAL_SHIFT  (4U)
/*! EXT_RCAL - EXT_RCAL */
#define USBC_CALIBRATION_CONTROL_EXT_RCAL(x)     (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_EXT_RCAL_SHIFT)) & USBC_CALIBRATION_CONTROL_EXT_RCAL_MASK)

#define USBC_CALIBRATION_CONTROL_IMPCAL_VTH_MASK (0x700U)
#define USBC_CALIBRATION_CONTROL_IMPCAL_VTH_SHIFT (8U)
/*! IMPCAL_VTH - Impedance Calibration Threshold Setting
 *  0b000..Impedance : 900/16 = 56 ohms
 *  0b001..Impedance : 900/17 = 53 ohms
 *  0b010..Impedance : 900/18 = 50 ohms
 *  0b011..Impedance : 900/19 = 47 ohms
 *  0b100..Impedance : 900/20 = 45 ohms
 *  0b101..Impedance : 900/21 = 43 ohms
 *  0b110..Impedance : 900/22 = 41 ohms
 *  0b111..Impedance : 900/23 = 39 ohms
 */
#define USBC_CALIBRATION_CONTROL_IMPCAL_VTH(x)   (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_IMPCAL_VTH_SHIFT)) & USBC_CALIBRATION_CONTROL_IMPCAL_VTH_MASK)

#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP_EN_MASK (0x800U)
#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP_EN_SHIFT (11U)
/*! EXT_FS_IMP_EN - FS Impedance Calibration Enable Control
 *  0b0..Use value set by EXT_FS_IMP[3:0]
 *  0b1..Use auto calibration results
 */
#define USBC_CALIBRATION_CONTROL_EXT_FS_IMP_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_EXT_FS_IMP_EN_SHIFT)) & USBC_CALIBRATION_CONTROL_EXT_FS_IMP_EN_MASK)

#define USBC_CALIBRATION_CONTROL_EXT_RCAL_EN_MASK (0x1000U)
#define USBC_CALIBRATION_CONTROL_EXT_RCAL_EN_SHIFT (12U)
/*! EXT_RCAL_EN - EXT_RCAL_EN */
#define USBC_CALIBRATION_CONTROL_EXT_RCAL_EN(x)  (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_EXT_RCAL_EN_SHIFT)) & USBC_CALIBRATION_CONTROL_EXT_RCAL_EN_MASK)

#define USBC_CALIBRATION_CONTROL_IMPCAL_START_MASK (0x2000U)
#define USBC_CALIBRATION_CONTROL_IMPCAL_START_SHIFT (13U)
/*! IMPCAL_START - IMPCAL_START */
#define USBC_CALIBRATION_CONTROL_IMPCAL_START(x) (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_IMPCAL_START_SHIFT)) & USBC_CALIBRATION_CONTROL_IMPCAL_START_MASK)

#define USBC_CALIBRATION_CONTROL_IMPCAL_POL_MASK (0x4000U)
#define USBC_CALIBRATION_CONTROL_IMPCAL_POL_SHIFT (14U)
/*! IMPCAL_POL - IMPCAL_POL */
#define USBC_CALIBRATION_CONTROL_IMPCAL_POL(x)   (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_IMPCAL_POL_SHIFT)) & USBC_CALIBRATION_CONTROL_IMPCAL_POL_MASK)

#define USBC_CALIBRATION_CONTROL_VCOCAL_POL_MASK (0x8000U)
#define USBC_CALIBRATION_CONTROL_VCOCAL_POL_SHIFT (15U)
/*! VCOCAL_POL - VCOCAL_POL */
#define USBC_CALIBRATION_CONTROL_VCOCAL_POL(x)   (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_VCOCAL_POL_SHIFT)) & USBC_CALIBRATION_CONTROL_VCOCAL_POL_MASK)

#define USBC_CALIBRATION_CONTROL_KVCO_MASK       (0x70000U)
#define USBC_CALIBRATION_CONTROL_KVCO_SHIFT      (16U)
/*! KVCO - KVCO */
#define USBC_CALIBRATION_CONTROL_KVCO(x)         (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_KVCO_SHIFT)) & USBC_CALIBRATION_CONTROL_KVCO_MASK)

#define USBC_CALIBRATION_CONTROL_KVCO_EXT_MASK   (0x80000U)
#define USBC_CALIBRATION_CONTROL_KVCO_EXT_SHIFT  (19U)
/*! KVCO_EXT - KVCO_EXT */
#define USBC_CALIBRATION_CONTROL_KVCO_EXT(x)     (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_KVCO_EXT_SHIFT)) & USBC_CALIBRATION_CONTROL_KVCO_EXT_MASK)

#define USBC_CALIBRATION_CONTROL_PLLCAL_MASK     (0x300000U)
#define USBC_CALIBRATION_CONTROL_PLLCAL_SHIFT    (20U)
/*! PLLCAL - PLL VCO Calibration Voltage
 *  0b00..VDD
 *  0b01..1.0V (default)
 *  0b10..1.05V
 *  0b11..1.1V
 */
#define USBC_CALIBRATION_CONTROL_PLLCAL(x)       (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_PLLCAL_SHIFT)) & USBC_CALIBRATION_CONTROL_PLLCAL_MASK)

#define USBC_CALIBRATION_CONTROL_PLLCAL_START_MASK (0x400000U)
#define USBC_CALIBRATION_CONTROL_PLLCAL_START_SHIFT (22U)
/*! PLLCAL_START - PLLCAL_START */
#define USBC_CALIBRATION_CONTROL_PLLCAL_START(x) (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_PLLCAL_START_SHIFT)) & USBC_CALIBRATION_CONTROL_PLLCAL_START_MASK)

#define USBC_CALIBRATION_CONTROL_IMPCAL_DONE_MASK (0x800000U)
#define USBC_CALIBRATION_CONTROL_IMPCAL_DONE_SHIFT (23U)
/*! IMPCAL_DONE - IMPCAL_DONE */
#define USBC_CALIBRATION_CONTROL_IMPCAL_DONE(x)  (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_IMPCAL_DONE_SHIFT)) & USBC_CALIBRATION_CONTROL_IMPCAL_DONE_MASK)

#define USBC_CALIBRATION_CONTROL_CURRENT_IMP_MASK (0xF000000U)
#define USBC_CALIBRATION_CONTROL_CURRENT_IMP_SHIFT (24U)
/*! CURRENT_IMP - CURRENT_IMP */
#define USBC_CALIBRATION_CONTROL_CURRENT_IMP(x)  (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_CURRENT_IMP_SHIFT)) & USBC_CALIBRATION_CONTROL_CURRENT_IMP_MASK)

#define USBC_CALIBRATION_CONTROL_CURRENT_KVCO_MASK (0x70000000U)
#define USBC_CALIBRATION_CONTROL_CURRENT_KVCO_SHIFT (28U)
/*! CURRENT_KVCO - CURRENT_KVCO */
#define USBC_CALIBRATION_CONTROL_CURRENT_KVCO(x) (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_CURRENT_KVCO_SHIFT)) & USBC_CALIBRATION_CONTROL_CURRENT_KVCO_MASK)

#define USBC_CALIBRATION_CONTROL_PLLCAL_DONE_MASK (0x80000000U)
#define USBC_CALIBRATION_CONTROL_PLLCAL_DONE_SHIFT (31U)
/*! PLLCAL_DONE - PLLCAL_DONE */
#define USBC_CALIBRATION_CONTROL_PLLCAL_DONE(x)  (((uint32_t)(((uint32_t)(x)) << USBC_CALIBRATION_CONTROL_PLLCAL_DONE_SHIFT)) & USBC_CALIBRATION_CONTROL_PLLCAL_DONE_MASK)
/*! @} */

/*! @name TX_CHANNEL_CONTRL_0 - Tx_Channel_Contrl_0 */
/*! @{ */

#define USBC_TX_CHANNEL_CONTRL_0_HSDRV_EN_MASK   (0xFU)
#define USBC_TX_CHANNEL_CONTRL_0_HSDRV_EN_SHIFT  (0U)
/*! HSDRV_EN - HSDRV_EN */
#define USBC_TX_CHANNEL_CONTRL_0_HSDRV_EN(x)     (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_HSDRV_EN_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_HSDRV_EN_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_HS_SR_SEL_MASK  (0x30U)
#define USBC_TX_CHANNEL_CONTRL_0_HS_SR_SEL_SHIFT (4U)
/*! HS_SR_SEL - Rise/Fall Time Of HS Transmitter
 *  0b00..Shortest rise/fall
 *  0b01..Subshortest rise/fall
 *  0b10..Sublongest rise/fall
 *  0b11..Longest rise/fall
 */
#define USBC_TX_CHANNEL_CONTRL_0_HS_SR_SEL(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_HS_SR_SEL_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_HS_SR_SEL_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_LOWVDD_EN_MASK  (0x40U)
#define USBC_TX_CHANNEL_CONTRL_0_LOWVDD_EN_SHIFT (6U)
/*! LOWVDD_EN - LOWVDD_EN */
#define USBC_TX_CHANNEL_CONTRL_0_LOWVDD_EN(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_LOWVDD_EN_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_LOWVDD_EN_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_7_MASK   (0x80U)
#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_7_SHIFT  (7U)
/*! UNUSED_7 - Reserved */
#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_7(x)     (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_UNUSED_7_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_UNUSED_7_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_FSDRV_EN_MASK   (0xF00U)
#define USBC_TX_CHANNEL_CONTRL_0_FSDRV_EN_SHIFT  (8U)
/*! FSDRV_EN - FSDRV_EN */
#define USBC_TX_CHANNEL_CONTRL_0_FSDRV_EN(x)     (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_FSDRV_EN_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_FSDRV_EN_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_DRV_EN_LS_MASK  (0xF000U)
#define USBC_TX_CHANNEL_CONTRL_0_DRV_EN_LS_SHIFT (12U)
/*! DRV_EN_LS - DRV_EN_LS */
#define USBC_TX_CHANNEL_CONTRL_0_DRV_EN_LS(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_DRV_EN_LS_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_DRV_EN_LS_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_IMP_SEL_LS_MASK (0xF0000U)
#define USBC_TX_CHANNEL_CONTRL_0_IMP_SEL_LS_SHIFT (16U)
/*! IMP_SEL_LS - IMP_SEL_LS */
#define USBC_TX_CHANNEL_CONTRL_0_IMP_SEL_LS(x)   (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_IMP_SEL_LS_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_IMP_SEL_LS_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_AMP_MASK        (0x700000U)
#define USBC_TX_CHANNEL_CONTRL_0_AMP_SHIFT       (20U)
/*! AMP - AMP */
#define USBC_TX_CHANNEL_CONTRL_0_AMP(x)          (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_AMP_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_AMP_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_PU_VDDR18_MASK  (0x800000U)
#define USBC_TX_CHANNEL_CONTRL_0_PU_VDDR18_SHIFT (23U)
/*! PU_VDDR18 - PU_VDDR18 */
#define USBC_TX_CHANNEL_CONTRL_0_PU_VDDR18(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_PU_VDDR18_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_PU_VDDR18_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_PU_ANA_MASK     (0x1000000U)
#define USBC_TX_CHANNEL_CONTRL_0_PU_ANA_SHIFT    (24U)
/*! PU_ANA - PU_ANA */
#define USBC_TX_CHANNEL_CONTRL_0_PU_ANA(x)       (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_PU_ANA_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_PU_ANA_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_PU_BY_REG_MASK  (0x2000000U)
#define USBC_TX_CHANNEL_CONTRL_0_PU_BY_REG_SHIFT (25U)
/*! PU_BY_REG - PU_BY_REG */
#define USBC_TX_CHANNEL_CONTRL_0_PU_BY_REG(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_PU_BY_REG_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_PU_BY_REG_MASK)

#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_26_MASK  (0xFC000000U)
#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_26_SHIFT (26U)
/*! UNUSED_26 - Reserved */
#define USBC_TX_CHANNEL_CONTRL_0_UNUSED_26(x)    (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_0_UNUSED_26_SHIFT)) & USBC_TX_CHANNEL_CONTRL_0_UNUSED_26_MASK)
/*! @} */

/*! @name TX_CHANNEL_CONTRL_1 - Tx_Channel_Contrl_1 */
/*! @{ */

#define USBC_TX_CHANNEL_CONTRL_1_UNUSED_0_MASK   (0xFFFFFFFFU)
#define USBC_TX_CHANNEL_CONTRL_1_UNUSED_0_SHIFT  (0U)
/*! UNUSED_0 - Reserved */
#define USBC_TX_CHANNEL_CONTRL_1_UNUSED_0(x)     (((uint32_t)(((uint32_t)(x)) << USBC_TX_CHANNEL_CONTRL_1_UNUSED_0_SHIFT)) & USBC_TX_CHANNEL_CONTRL_1_UNUSED_0_MASK)
/*! @} */

/*! @name RX_CHANNEL_CONTRL_0 - Rx_Channel_Contrl_0 */
/*! @{ */

#define USBC_RX_CHANNEL_CONTRL_0_SQ_THRESH_MASK  (0xFU)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_THRESH_SHIFT (0U)
/*! SQ_THRESH - SQ_THRESH */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_THRESH(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_THRESH_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_THRESH_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_RXVDD18_MASK    (0x30U)
#define USBC_RX_CHANNEL_CONTRL_0_RXVDD18_SHIFT   (4U)
/*! RXVDD18 - RXVDD18 */
#define USBC_RX_CHANNEL_CONTRL_0_RXVDD18(x)      (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_RXVDD18_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_RXVDD18_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_RXVDDL_MASK     (0xC0U)
#define USBC_RX_CHANNEL_CONTRL_0_RXVDDL_SHIFT    (6U)
/*! RXVDDL - RXVDDL */
#define USBC_RX_CHANNEL_CONTRL_0_RXVDDL(x)       (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_RXVDDL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_RXVDDL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_DISCON_THRESH_MASK (0x300U)
#define USBC_RX_CHANNEL_CONTRL_0_DISCON_THRESH_SHIFT (8U)
/*! DISCON_THRESH - DISCON_THRESH */
#define USBC_RX_CHANNEL_CONTRL_0_DISCON_THRESH(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_DISCON_THRESH_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_DISCON_THRESH_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_LINESTATE_EN_MASK (0x400U)
#define USBC_RX_CHANNEL_CONTRL_0_LINESTATE_EN_SHIFT (10U)
/*! LINESTATE_EN - LINESTATE_EN */
#define USBC_RX_CHANNEL_CONTRL_0_LINESTATE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_LINESTATE_EN_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_LINESTATE_EN_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_11_MASK  (0x800U)
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_11_SHIFT (11U)
/*! UNUSED_11 - Reserved */
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_11(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_UNUSED_11_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_UNUSED_11_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_DLY_SEL_MASK (0x3000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_DLY_SEL_SHIFT (12U)
/*! SQ_DLY_SEL - SQ_DLY_SEL */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_DLY_SEL(x)   (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_DLY_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_DLY_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_CM_SEL_MASK  (0x4000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_CM_SEL_SHIFT (14U)
/*! SQ_CM_SEL - SQ_CM_SEL */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_CM_SEL(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_CM_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_CM_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_DET_EN_MASK  (0x8000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_DET_EN_SHIFT (15U)
/*! SQ_DET_EN - SQ_DET_EN */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_DET_EN(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_DET_EN_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_DET_EN_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_VHL_SEL_MASK (0x10000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_VHL_SEL_SHIFT (16U)
/*! SQ_VHL_SEL - SQ_VHL_SEL */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_VHL_SEL(x)   (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_VHL_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_VHL_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_17_MASK  (0x60000U)
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_17_SHIFT (17U)
/*! UNUSED_17 - Reserved */
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_17(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_UNUSED_17_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_UNUSED_17_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_STRESS_TEST_MODE_MASK (0x80000U)
#define USBC_RX_CHANNEL_CONTRL_0_STRESS_TEST_MODE_SHIFT (19U)
/*! STRESS_TEST_MODE - STRESS_TEST_MODE */
#define USBC_RX_CHANNEL_CONTRL_0_STRESS_TEST_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_STRESS_TEST_MODE_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_STRESS_TEST_MODE_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_TESTMON_MASK    (0xF00000U)
#define USBC_RX_CHANNEL_CONTRL_0_TESTMON_SHIFT   (20U)
/*! TESTMON - TESTMON */
#define USBC_RX_CHANNEL_CONTRL_0_TESTMON(x)      (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_TESTMON_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_TESTMON_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_EN_MASK (0x1000000U)
#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_EN_SHIFT (24U)
/*! RESET_EXT_EN - RESET_EXT_EN */
#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_EN_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_EN_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_MASK  (0x2000000U)
#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_SHIFT (25U)
/*! RESET_EXT - RESET_EXT */
#define USBC_RX_CHANNEL_CONTRL_0_RESET_EXT(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_RESET_EXT_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_VREF_MASK (0xC000000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_VREF_SHIFT (26U)
/*! SQ_ANA_VREF - SQ_ANA_VREF */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_VREF(x)  (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_VREF_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_VREF_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_DTC_SEL_MASK (0x10000000U)
#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_DTC_SEL_SHIFT (28U)
/*! SQ_ANA_DTC_SEL - SQ_ANA_DTC_SEL */
#define USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_DTC_SEL(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_DTC_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_SQ_ANA_DTC_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_CHIRPMODE_SEL_MASK (0x20000000U)
#define USBC_RX_CHANNEL_CONTRL_0_CHIRPMODE_SEL_SHIFT (29U)
/*! CHIRPMODE_SEL - CHIRPMODE_SEL */
#define USBC_RX_CHANNEL_CONTRL_0_CHIRPMODE_SEL(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_CHIRPMODE_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_CHIRPMODE_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_30_MASK  (0xC0000000U)
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_30_SHIFT (30U)
/*! UNUSED_30 - Reserved */
#define USBC_RX_CHANNEL_CONTRL_0_UNUSED_30(x)    (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_0_UNUSED_30_SHIFT)) & USBC_RX_CHANNEL_CONTRL_0_UNUSED_30_MASK)
/*! @} */

/*! @name RX_CHANNEL_CONTRL_1 - Rx_Channel_Contrl_1 */
/*! @{ */

#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_MASK (0x7U)
#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_SHIFT (0U)
/*! EXT_SQ_AMP_CAL - EXT_SQ_AMP_CAL */
#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_EN_MASK (0x8U)
#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_EN_SHIFT (3U)
/*! EXT_SQ_AMP_CAL_EN - EXT_SQ_AMP_CAL_EN */
#define USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_EN_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_EXT_SQ_AMP_CAL_EN_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_REQ_SQCAL_START_MASK (0x10U)
#define USBC_RX_CHANNEL_CONTRL_1_REQ_SQCAL_START_SHIFT (4U)
/*! REQ_SQCAL_START - REQ_SQCAL_START */
#define USBC_RX_CHANNEL_CONTRL_1_REQ_SQCAL_START(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_REQ_SQCAL_START_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_REQ_SQCAL_START_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_REG_SQ_UD_SWAP_MASK (0x20U)
#define USBC_RX_CHANNEL_CONTRL_1_REG_SQ_UD_SWAP_SHIFT (5U)
/*! REG_SQ_UD_SWAP - REG_SQ_UD_SWAP */
#define USBC_RX_CHANNEL_CONTRL_1_REG_SQ_UD_SWAP(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_REG_SQ_UD_SWAP_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_REG_SQ_UD_SWAP_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_SQ_DPDM_AMP_SEL_MASK (0xC0U)
#define USBC_RX_CHANNEL_CONTRL_1_SQ_DPDM_AMP_SEL_SHIFT (6U)
/*! SQ_DPDM_AMP_SEL - SQ_DPDM_AMP_SEL */
#define USBC_RX_CHANNEL_CONTRL_1_SQ_DPDM_AMP_SEL(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_SQ_DPDM_AMP_SEL_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_SQ_DPDM_AMP_SEL_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_UNUSED_8_MASK   (0x7FFFFF00U)
#define USBC_RX_CHANNEL_CONTRL_1_UNUSED_8_SHIFT  (8U)
/*! UNUSED_8 - Reserved */
#define USBC_RX_CHANNEL_CONTRL_1_UNUSED_8(x)     (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_UNUSED_8_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_UNUSED_8_MASK)

#define USBC_RX_CHANNEL_CONTRL_1_REG_SQCAL_DONE_MASK (0x80000000U)
#define USBC_RX_CHANNEL_CONTRL_1_REG_SQCAL_DONE_SHIFT (31U)
/*! REG_SQCAL_DONE - REG_SQCAL_DONE */
#define USBC_RX_CHANNEL_CONTRL_1_REG_SQCAL_DONE(x) (((uint32_t)(((uint32_t)(x)) << USBC_RX_CHANNEL_CONTRL_1_REG_SQCAL_DONE_SHIFT)) & USBC_RX_CHANNEL_CONTRL_1_REG_SQCAL_DONE_MASK)
/*! @} */

/*! @name DIGITAL_CONTRL_0 - Digital_Contrl_0 */
/*! @{ */

#define USBC_DIGITAL_CONTRL_0_SYNC_NUM_MASK      (0x3U)
#define USBC_DIGITAL_CONTRL_0_SYNC_NUM_SHIFT     (0U)
/*! SYNC_NUM - SYNC_NUM */
#define USBC_DIGITAL_CONTRL_0_SYNC_NUM(x)        (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SYNC_NUM_SHIFT)) & USBC_DIGITAL_CONTRL_0_SYNC_NUM_MASK)

#define USBC_DIGITAL_CONTRL_0_SYNCDET_WIN32_MASK (0x4U)
#define USBC_DIGITAL_CONTRL_0_SYNCDET_WIN32_SHIFT (2U)
/*! SYNCDET_WIN32 - SYNCDET_WIN32 */
#define USBC_DIGITAL_CONTRL_0_SYNCDET_WIN32(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SYNCDET_WIN32_SHIFT)) & USBC_DIGITAL_CONTRL_0_SYNCDET_WIN32_MASK)

#define USBC_DIGITAL_CONTRL_0_SYNCDET_WINDOW_EN_MASK (0x8U)
#define USBC_DIGITAL_CONTRL_0_SYNCDET_WINDOW_EN_SHIFT (3U)
/*! SYNCDET_WINDOW_EN - SYNCDET_WINDOW_EN */
#define USBC_DIGITAL_CONTRL_0_SYNCDET_WINDOW_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SYNCDET_WINDOW_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_SYNCDET_WINDOW_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL0_MASK (0x10U)
#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL0_SHIFT (4U)
/*! HOST_DISCON_SEL0 - HOST_DISCON_SEL0 */
#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL0(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL0_SHIFT)) & USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL0_MASK)

#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL1_MASK (0x20U)
#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL1_SHIFT (5U)
/*! HOST_DISCON_SEL1 - HOST_DISCON_SEL1 */
#define USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL1(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL1_SHIFT)) & USBC_DIGITAL_CONTRL_0_HOST_DISCON_SEL1_MASK)

#define USBC_DIGITAL_CONTRL_0_UNUSED_6_MASK      (0x40U)
#define USBC_DIGITAL_CONTRL_0_UNUSED_6_SHIFT     (6U)
/*! UNUSED_6 - Reserved */
#define USBC_DIGITAL_CONTRL_0_UNUSED_6(x)        (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_UNUSED_6_SHIFT)) & USBC_DIGITAL_CONTRL_0_UNUSED_6_MASK)

#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_BYPASS_MASK (0x80U)
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_BYPASS_SHIFT (7U)
/*! PLL_LOCK_BYPASS - PLL_LOCK_BYPASS */
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_PLL_LOCK_BYPASS_SHIFT)) & USBC_DIGITAL_CONTRL_0_PLL_LOCK_BYPASS_MASK)

#define USBC_DIGITAL_CONTRL_0_LONG_EOP_MASK      (0x100U)
#define USBC_DIGITAL_CONTRL_0_LONG_EOP_SHIFT     (8U)
/*! LONG_EOP - LONG_EOP */
#define USBC_DIGITAL_CONTRL_0_LONG_EOP(x)        (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_LONG_EOP_SHIFT)) & USBC_DIGITAL_CONTRL_0_LONG_EOP_MASK)

#define USBC_DIGITAL_CONTRL_0_RXFILT1_EN_MASK    (0x200U)
#define USBC_DIGITAL_CONTRL_0_RXFILT1_EN_SHIFT   (9U)
/*! RXFILT1_EN - RXFILT1_EN */
#define USBC_DIGITAL_CONTRL_0_RXFILT1_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_RXFILT1_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_RXFILT1_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_RXFILT2_EN_MASK    (0x400U)
#define USBC_DIGITAL_CONTRL_0_RXFILT2_EN_SHIFT   (10U)
/*! RXFILT2_EN - RXFILT2_EN */
#define USBC_DIGITAL_CONTRL_0_RXFILT2_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_RXFILT2_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_RXFILT2_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_EARLY_TX_ENABLE_MASK (0x800U)
#define USBC_DIGITAL_CONTRL_0_EARLY_TX_ENABLE_SHIFT (11U)
/*! EARLY_TX_ENABLE - EARLY_TX_ENABLE */
#define USBC_DIGITAL_CONTRL_0_EARLY_TX_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_EARLY_TX_ENABLE_SHIFT)) & USBC_DIGITAL_CONTRL_0_EARLY_TX_ENABLE_MASK)

#define USBC_DIGITAL_CONTRL_0_SQ_BLK_MASK        (0x7000U)
#define USBC_DIGITAL_CONTRL_0_SQ_BLK_SHIFT       (12U)
/*! SQ_BLK - SQ_BLK */
#define USBC_DIGITAL_CONTRL_0_SQ_BLK(x)          (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SQ_BLK_SHIFT)) & USBC_DIGITAL_CONTRL_0_SQ_BLK_MASK)

#define USBC_DIGITAL_CONTRL_0_RXFILT3_EN_MASK    (0x8000U)
#define USBC_DIGITAL_CONTRL_0_RXFILT3_EN_SHIFT   (15U)
/*! RXFILT3_EN - RXFILT3_EN */
#define USBC_DIGITAL_CONTRL_0_RXFILT3_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_RXFILT3_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_RXFILT3_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_SQ_FILT_MASK       (0x70000U)
#define USBC_DIGITAL_CONTRL_0_SQ_FILT_SHIFT      (16U)
/*! SQ_FILT - SQ_FILT */
#define USBC_DIGITAL_CONTRL_0_SQ_FILT(x)         (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SQ_FILT_SHIFT)) & USBC_DIGITAL_CONTRL_0_SQ_FILT_MASK)

#define USBC_DIGITAL_CONTRL_0_FIFOOVF_EN_MASK    (0x80000U)
#define USBC_DIGITAL_CONTRL_0_FIFOOVF_EN_SHIFT   (19U)
/*! FIFOOVF_EN - FIFOOVF_EN */
#define USBC_DIGITAL_CONTRL_0_FIFOOVF_EN(x)      (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_FIFOOVF_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_FIFOOVF_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_LOSSOFSYNC_EN_MASK (0x100000U)
#define USBC_DIGITAL_CONTRL_0_LOSSOFSYNC_EN_SHIFT (20U)
/*! LOSSOFSYNC_EN - LOSSOFSYNC_EN */
#define USBC_DIGITAL_CONTRL_0_LOSSOFSYNC_EN(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_LOSSOFSYNC_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_LOSSOFSYNC_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_RX_RUNAWAY_EN_MASK (0x200000U)
#define USBC_DIGITAL_CONTRL_0_RX_RUNAWAY_EN_SHIFT (21U)
/*! RX_RUNAWAY_EN - RX_RUNAWAY_EN */
#define USBC_DIGITAL_CONTRL_0_RX_RUNAWAY_EN(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_RX_RUNAWAY_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_RX_RUNAWAY_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_STOP_AT_RXERR_MASK (0x400000U)
#define USBC_DIGITAL_CONTRL_0_STOP_AT_RXERR_SHIFT (22U)
/*! STOP_AT_RXERR - STOP_AT_RXERR */
#define USBC_DIGITAL_CONTRL_0_STOP_AT_RXERR(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_STOP_AT_RXERR_SHIFT)) & USBC_DIGITAL_CONTRL_0_STOP_AT_RXERR_MASK)

#define USBC_DIGITAL_CONTRL_0_EDGE_OPT_EN_MASK   (0x800000U)
#define USBC_DIGITAL_CONTRL_0_EDGE_OPT_EN_SHIFT  (23U)
/*! EDGE_OPT_EN - EDGE_OPT_EN */
#define USBC_DIGITAL_CONTRL_0_EDGE_OPT_EN(x)     (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_EDGE_OPT_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_EDGE_OPT_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_SE0_FILT_SEL_MASK  (0x1000000U)
#define USBC_DIGITAL_CONTRL_0_SE0_FILT_SEL_SHIFT (24U)
/*! SE0_FILT_SEL - SE0_FILT_SEL */
#define USBC_DIGITAL_CONTRL_0_SE0_FILT_SEL(x)    (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SE0_FILT_SEL_SHIFT)) & USBC_DIGITAL_CONTRL_0_SE0_FILT_SEL_MASK)

#define USBC_DIGITAL_CONTRL_0_SE1_FILT_SEL_MASK  (0x2000000U)
#define USBC_DIGITAL_CONTRL_0_SE1_FILT_SEL_SHIFT (25U)
/*! SE1_FILT_SEL - SE1_FILT_SEL */
#define USBC_DIGITAL_CONTRL_0_SE1_FILT_SEL(x)    (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_SE1_FILT_SEL_SHIFT)) & USBC_DIGITAL_CONTRL_0_SE1_FILT_SEL_MASK)

#define USBC_DIGITAL_CONTRL_0_CORE_UTMI_SEL_MASK (0x4000000U)
#define USBC_DIGITAL_CONTRL_0_CORE_UTMI_SEL_SHIFT (26U)
/*! CORE_UTMI_SEL - CORE_UTMI_SEL */
#define USBC_DIGITAL_CONTRL_0_CORE_UTMI_SEL(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_CORE_UTMI_SEL_SHIFT)) & USBC_DIGITAL_CONTRL_0_CORE_UTMI_SEL_MASK)

#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_VAL_MASK (0x8000000U)
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_VAL_SHIFT (27U)
/*! PLL_LOCK_FORCE_VAL - PLL_LOCK_FORCE_VAL */
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_VAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_VAL_SHIFT)) & USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_VAL_MASK)

#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_EN_MASK (0x10000000U)
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_EN_SHIFT (28U)
/*! PLL_LOCK_FORCE_EN - PLL_LOCK_FORCE_EN */
#define USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_EN_SHIFT)) & USBC_DIGITAL_CONTRL_0_PLL_LOCK_FORCE_EN_MASK)

#define USBC_DIGITAL_CONTRL_0_FIFO_OVF_ERROR_MASK (0x20000000U)
#define USBC_DIGITAL_CONTRL_0_FIFO_OVF_ERROR_SHIFT (29U)
/*! FIFO_OVF_ERROR - FIFO_OVF_ERROR */
#define USBC_DIGITAL_CONTRL_0_FIFO_OVF_ERROR(x)  (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_FIFO_OVF_ERROR_SHIFT)) & USBC_DIGITAL_CONTRL_0_FIFO_OVF_ERROR_MASK)

#define USBC_DIGITAL_CONTRL_0_LOSS_OF_SYNC_ERROR_MASK (0x40000000U)
#define USBC_DIGITAL_CONTRL_0_LOSS_OF_SYNC_ERROR_SHIFT (30U)
/*! LOSS_OF_SYNC_ERROR - LOSS_OF_SYNC_ERROR */
#define USBC_DIGITAL_CONTRL_0_LOSS_OF_SYNC_ERROR(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_LOSS_OF_SYNC_ERROR_SHIFT)) & USBC_DIGITAL_CONTRL_0_LOSS_OF_SYNC_ERROR_MASK)

#define USBC_DIGITAL_CONTRL_0_BITSTUFFING_ERROR_MASK (0x80000000U)
#define USBC_DIGITAL_CONTRL_0_BITSTUFFING_ERROR_SHIFT (31U)
/*! BITSTUFFING_ERROR - BITSTUFFING_ERROR */
#define USBC_DIGITAL_CONTRL_0_BITSTUFFING_ERROR(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_0_BITSTUFFING_ERROR_SHIFT)) & USBC_DIGITAL_CONTRL_0_BITSTUFFING_ERROR_MASK)
/*! @} */

/*! @name DIGITAL_CONTRL_1 - Digital_Contrl_1 */
/*! @{ */

#define USBC_DIGITAL_CONTRL_1_REG_FORCE_END_EN_MASK (0x1U)
#define USBC_DIGITAL_CONTRL_1_REG_FORCE_END_EN_SHIFT (0U)
/*! REG_FORCE_END_EN - REG_FORCE_END_EN */
#define USBC_DIGITAL_CONTRL_1_REG_FORCE_END_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FORCE_END_EN_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FORCE_END_EN_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE_MASK (0x2U)
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE_SHIFT (1U)
/*! REG_FS_RX_ERROR_MODE - REG_FS_RX_ERROR_MODE */
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE1_MASK (0x4U)
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE1_SHIFT (2U)
/*! REG_FS_RX_ERROR_MODE1 - REG_FS_RX_ERROR_MODE1 */
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE1(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE1_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE1_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE2_MASK (0x8U)
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE2_SHIFT (3U)
/*! REG_FS_RX_ERROR_MODE2 - REG_FS_RX_ERROR_MODE2 */
#define USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE2(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE2_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FS_RX_ERROR_MODE2_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_ARC_DPDM_MODE_MASK (0x10U)
#define USBC_DIGITAL_CONTRL_1_REG_ARC_DPDM_MODE_SHIFT (4U)
/*! REG_ARC_DPDM_MODE - REG_ARC_DPDM_MODE */
#define USBC_DIGITAL_CONTRL_1_REG_ARC_DPDM_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_ARC_DPDM_MODE_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_ARC_DPDM_MODE_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_DM_PULLDOWN_MASK (0x20U)
#define USBC_DIGITAL_CONTRL_1_REG_DM_PULLDOWN_SHIFT (5U)
/*! REG_DM_PULLDOWN - REG_DM_PULLDOWN */
#define USBC_DIGITAL_CONTRL_1_REG_DM_PULLDOWN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_DM_PULLDOWN_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_DM_PULLDOWN_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_DP_PULLDOWN_MASK (0x40U)
#define USBC_DIGITAL_CONTRL_1_REG_DP_PULLDOWN_SHIFT (6U)
/*! REG_DP_PULLDOWN - REG_DP_PULLDOWN */
#define USBC_DIGITAL_CONTRL_1_REG_DP_PULLDOWN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_DP_PULLDOWN_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_DP_PULLDOWN_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FS_EOP_MODE_MASK (0x80U)
#define USBC_DIGITAL_CONTRL_1_REG_FS_EOP_MODE_SHIFT (7U)
/*! REG_FS_EOP_MODE - REG_FS_EOP_MODE */
#define USBC_DIGITAL_CONTRL_1_REG_FS_EOP_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FS_EOP_MODE_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FS_EOP_MODE_MASK)

#define USBC_DIGITAL_CONTRL_1_UNUSED_8_MASK      (0x300U)
#define USBC_DIGITAL_CONTRL_1_UNUSED_8_SHIFT     (8U)
/*! UNUSED_8 - Reserved */
#define USBC_DIGITAL_CONTRL_1_UNUSED_8(x)        (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_UNUSED_8_SHIFT)) & USBC_DIGITAL_CONTRL_1_UNUSED_8_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_NOVBUS_DPDM00_MASK (0x400U)
#define USBC_DIGITAL_CONTRL_1_REG_NOVBUS_DPDM00_SHIFT (10U)
/*! REG_NOVBUS_DPDM00 - REG_NOVBUS_DPDM00 */
#define USBC_DIGITAL_CONTRL_1_REG_NOVBUS_DPDM00(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_NOVBUS_DPDM00_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_NOVBUS_DPDM00_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FREERUNCLK_EN_MASK (0x800U)
#define USBC_DIGITAL_CONTRL_1_REG_FREERUNCLK_EN_SHIFT (11U)
/*! REG_FREERUNCLK_EN - REG_FREERUNCLK_EN */
#define USBC_DIGITAL_CONTRL_1_REG_FREERUNCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FREERUNCLK_EN_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FREERUNCLK_EN_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_FLSMODELP_EN_MASK (0x1000U)
#define USBC_DIGITAL_CONTRL_1_REG_FLSMODELP_EN_SHIFT (12U)
/*! REG_FLSMODELP_EN - REG_FLSMODELP_EN */
#define USBC_DIGITAL_CONTRL_1_REG_FLSMODELP_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_FLSMODELP_EN_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_FLSMODELP_EN_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_OPMODE_SEL_MASK (0x2000U)
#define USBC_DIGITAL_CONTRL_1_REG_OPMODE_SEL_SHIFT (13U)
/*! REG_OPMODE_SEL - REG_OPMODE_SEL */
#define USBC_DIGITAL_CONTRL_1_REG_OPMODE_SEL(x)  (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_OPMODE_SEL_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_OPMODE_SEL_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_LAT_UTMI_MASK  (0x4000U)
#define USBC_DIGITAL_CONTRL_1_REG_LAT_UTMI_SHIFT (14U)
/*! REG_LAT_UTMI - REG_LAT_UTMI */
#define USBC_DIGITAL_CONTRL_1_REG_LAT_UTMI(x)    (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_LAT_UTMI_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_LAT_UTMI_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_SWAP_DPDM_MASK (0x8000U)
#define USBC_DIGITAL_CONTRL_1_REG_SWAP_DPDM_SHIFT (15U)
/*! REG_SWAP_DPDM - REG_SWAP_DPDM */
#define USBC_DIGITAL_CONTRL_1_REG_SWAP_DPDM(x)   (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_SWAP_DPDM_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_SWAP_DPDM_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_SAMPLEROFF_DLY_MASK (0xF0000U)
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLEROFF_DLY_SHIFT (16U)
/*! REG_SAMPLEROFF_DLY - REG_SAMPLEROFF_DLY */
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLEROFF_DLY(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_SAMPLEROFF_DLY_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_SAMPLEROFF_DLY_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_ON_MASK (0x100000U)
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_ON_SHIFT (20U)
/*! REG_SAMPLER_ON - REG_SAMPLER_ON */
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_ON(x)  (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_SAMPLER_ON_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_SAMPLER_ON_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_PREOFF_MASK (0x200000U)
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_PREOFF_SHIFT (21U)
/*! REG_SAMPLER_PREOFF - REG_SAMPLER_PREOFF */
#define USBC_DIGITAL_CONTRL_1_REG_SAMPLER_PREOFF(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_SAMPLER_PREOFF_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_SAMPLER_PREOFF_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_CLK_OUT_DLY_MASK (0x3FC00000U)
#define USBC_DIGITAL_CONTRL_1_REG_CLK_OUT_DLY_SHIFT (22U)
/*! REG_CLK_OUT_DLY - REG_CLK_OUT_DLY */
#define USBC_DIGITAL_CONTRL_1_REG_CLK_OUT_DLY(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_CLK_OUT_DLY_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_CLK_OUT_DLY_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_LSIDLE_PRE_MASK (0x40000000U)
#define USBC_DIGITAL_CONTRL_1_REG_LSIDLE_PRE_SHIFT (30U)
/*! REG_LSIDLE_PRE - REG_LSIDLE_PRE */
#define USBC_DIGITAL_CONTRL_1_REG_LSIDLE_PRE(x)  (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_LSIDLE_PRE_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_LSIDLE_PRE_MASK)

#define USBC_DIGITAL_CONTRL_1_REG_VBUSON_SEL_HIGH_MASK (0x80000000U)
#define USBC_DIGITAL_CONTRL_1_REG_VBUSON_SEL_HIGH_SHIFT (31U)
/*! REG_VBUSON_SEL_HIGH - REG_VBUSON_SEL_HIGH */
#define USBC_DIGITAL_CONTRL_1_REG_VBUSON_SEL_HIGH(x) (((uint32_t)(((uint32_t)(x)) << USBC_DIGITAL_CONTRL_1_REG_VBUSON_SEL_HIGH_SHIFT)) & USBC_DIGITAL_CONTRL_1_REG_VBUSON_SEL_HIGH_MASK)
/*! @} */

/*! @name TEST_CONTRL_AND_STATUS_0 - Test_Contrl_and_Status_0 */
/*! @{ */

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_PATTERN_MASK (0xFFU)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_PATTERN_SHIFT (0U)
/*! REG_TEST_TX_PATTERN - REG_TEST_TX_PATTERN */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_PATTERN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_PATTERN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_PATTERN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_MODE_MASK (0x300U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_MODE_SHIFT (8U)
/*! REG_TEST_MODE - REG_TEST_MODE */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_MODE_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_MODE_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_10_MASK (0xC00U)
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_10_SHIFT (10U)
/*! UNUSED_10 - Reserved */
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_10(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_10_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_10_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SKIP_MASK (0x7000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SKIP_SHIFT (12U)
/*! REG_TEST_SKIP - REG_TEST_SKIP */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SKIP(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SKIP_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SKIP_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_15_MASK (0x8000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_15_SHIFT (15U)
/*! UNUSED_15 - Reserved */
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_15(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_15_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_15_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_OP_MODE_MASK (0x30000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_OP_MODE_SHIFT (16U)
/*! REG_TEST_OP_MODE - REG_TEST_OP_MODE */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_OP_MODE(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_OP_MODE_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_OP_MODE_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_XVCR_SELECT_MASK (0xC0000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_XVCR_SELECT_SHIFT (18U)
/*! REG_TEST_XVCR_SELECT - REG_TEST_XVCR_SELECT */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_XVCR_SELECT(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_XVCR_SELECT_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_XVCR_SELECT_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TERM_SELECT_MASK (0x100000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TERM_SELECT_SHIFT (20U)
/*! REG_TEST_TERM_SELECT - REG_TEST_TERM_SELECT */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TERM_SELECT(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TERM_SELECT_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TERM_SELECT_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_RESET_MASK (0x200000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_RESET_SHIFT (21U)
/*! REG_TEST_RESET - REG_TEST_RESET */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_RESET(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_RESET_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_RESET_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SUSPENDM_MASK (0x400000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SUSPENDM_SHIFT (22U)
/*! REG_TEST_SUSPENDM - REG_TEST_SUSPENDM */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SUSPENDM(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SUSPENDM_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_SUSPENDM_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_BITSTUFF_EN_MASK (0x800000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_BITSTUFF_EN_SHIFT (23U)
/*! REG_TEST_TX_BITSTUFF_EN - REG_TEST_TX_BITSTUFF_EN */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_BITSTUFF_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_BITSTUFF_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_TX_BITSTUFF_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_24_MASK (0x1000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_24_SHIFT (24U)
/*! UNUSED_24 - Reserved */
#define USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_24(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_24_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_UNUSED_24_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_UTMI_SEL_MASK (0x2000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_UTMI_SEL_SHIFT (25U)
/*! REG_TEST_UTMI_SEL - REG_TEST_UTMI_SEL */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_UTMI_SEL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_UTMI_SEL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_UTMI_SEL_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_BYPASS_MASK (0x4000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_BYPASS_SHIFT (26U)
/*! REG_TEST_BYPASS - REG_TEST_BYPASS */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_BYPASS_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_BYPASS_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_ANA_LPBK_MASK (0x8000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_ANA_LPBK_SHIFT (27U)
/*! REG_TEST_ANA_LPBK - REG_TEST_ANA_LPBK */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_ANA_LPBK(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_ANA_LPBK_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_ANA_LPBK_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_DIG_LPBK_MASK (0x10000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_DIG_LPBK_SHIFT (28U)
/*! REG_TEST_DIG_LPBK - REG_TEST_DIG_LPBK */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_DIG_LPBK(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_DIG_LPBK_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_DIG_LPBK_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_EN_MASK (0x20000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_EN_SHIFT (29U)
/*! REG_TEST_EN - REG_TEST_EN */
#define USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_REG_TEST_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_DONE_MASK (0x40000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_DONE_SHIFT (30U)
/*! TEST_DONE - TEST_DONE */
#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_DONE(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_TEST_DONE_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_TEST_DONE_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_FAIL_MASK (0x80000000U)
#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_FAIL_SHIFT (31U)
/*! TEST_FAIL - TEST_FAIL */
#define USBC_TEST_CONTRL_AND_STATUS_0_TEST_FAIL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_0_TEST_FAIL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_0_TEST_FAIL_MASK)
/*! @} */

/*! @name TEST_CONTRL_AND_STATUS_1 - Test_Contrl_and_Status_1 */
/*! @{ */

#define USBC_TEST_CONTRL_AND_STATUS_1_TEST_LEN_MASK (0x7FFU)
#define USBC_TEST_CONTRL_AND_STATUS_1_TEST_LEN_SHIFT (0U)
/*! TEST_LEN - TEST_LEN */
#define USBC_TEST_CONTRL_AND_STATUS_1_TEST_LEN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_TEST_LEN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_TEST_LEN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_UNUSED_11_MASK (0x800U)
#define USBC_TEST_CONTRL_AND_STATUS_1_UNUSED_11_SHIFT (11U)
/*! UNUSED_11 - Reserved */
#define USBC_TEST_CONTRL_AND_STATUS_1_UNUSED_11(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_UNUSED_11_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_UNUSED_11_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_N_MASK (0x1000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_N_SHIFT (12U)
/*! FLS_RX_N - fls_rx_n */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_N(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_N_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_N_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_P_MASK (0x2000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_P_SHIFT (13U)
/*! FLS_RX_P - fls_rx_p */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_P(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_P_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_P_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_FS_LS_MASK (0x4000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_FS_LS_SHIFT (14U)
/*! DOUT_FS_LS - dout_fs_ls */
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_FS_LS(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_DOUT_FS_LS_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_DOUT_FS_LS_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_FORCE_EN_MASK (0x8000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_FORCE_EN_SHIFT (15U)
/*! FLS_RX_FORCE_EN - fls_rx_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_RX_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_VAL_MASK (0x30000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_VAL_SHIFT (16U)
/*! LINESTATE_FORCE_VAL - linestate_force_val */
#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_VAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_VAL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_VAL_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_EN_MASK (0x40000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_EN_SHIFT (18U)
/*! LINESTATE_FORCE_EN - linestate_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_LINESTATE_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_VAL_MASK (0x80000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_VAL_SHIFT (19U)
/*! DISCON_FORCE_VAL - discon_force_val */
#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_VAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_VAL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_VAL_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_EN_MASK (0x100000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_EN_SHIFT (20U)
/*! DISCON_FORCE_EN - discon_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_DISCON_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_VAL_MASK (0x200000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_VAL_SHIFT (21U)
/*! SQ_FORCE_VAL - sq_force_val */
#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_VAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_VAL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_VAL_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_EN_MASK (0x400000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_EN_SHIFT (22U)
/*! SQ_FORCE_EN - sq_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_SQ_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_VAL_MASK (0x800000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_VAL_SHIFT (23U)
/*! DOUT_HS_FORCE_VAL - dout_hs_force_val */
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_VAL(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_VAL_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_VAL_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_EN_MASK (0x1000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_EN_SHIFT (24U)
/*! DOUT_HS_FORCE_EN - dout_hs_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_DOUT_HS_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_DATA_MASK (0x2000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_DATA_SHIFT (25U)
/*! HS_TX_DATA - hs_tx_data */
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_DATA(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_DATA_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_DATA_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_ENABLE_MASK (0x4000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_ENABLE_SHIFT (26U)
/*! HS_TX_ENABLE - hs_tx_enable */
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_ENABLE_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_ENABLE_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_FORCE_EN_MASK (0x8000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_FORCE_EN_SHIFT (27U)
/*! HS_TX_FORCE_EN - hs_tx_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_HS_TX_FORCE_EN_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_DAT_MASK (0x10000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_DAT_SHIFT (28U)
/*! FLS_TX_DAT - fls_tx_dat */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_DAT(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_DAT_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_DAT_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_SE0_MASK (0x20000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_SE0_SHIFT (29U)
/*! FLS_TX_SE0 - fls_tx_se0 */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_SE0(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_SE0_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_SE0_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_ENABLE_MASK (0x40000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_ENABLE_SHIFT (30U)
/*! FLS_TX_ENABLE - fls_tx_enable */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_ENABLE_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_ENABLE_MASK)

#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_FORCE_EN_MASK (0x80000000U)
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_FORCE_EN_SHIFT (31U)
/*! FLS_TX_FORCE_EN - fls_tx_force_en */
#define USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_FORCE_EN(x) (((uint32_t)(((uint32_t)(x)) << USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_FORCE_EN_SHIFT)) & USBC_TEST_CONTRL_AND_STATUS_1_FLS_TX_FORCE_EN_MASK)
/*! @} */

/*! @name MONITOR - MONITOR */
/*! @{ */

#define USBC_MONITOR_REG_MON_SEL_MASK            (0x3FU)
#define USBC_MONITOR_REG_MON_SEL_SHIFT           (0U)
/*! REG_MON_SEL - REG_MON_SEL */
#define USBC_MONITOR_REG_MON_SEL(x)              (((uint32_t)(((uint32_t)(x)) << USBC_MONITOR_REG_MON_SEL_SHIFT)) & USBC_MONITOR_REG_MON_SEL_MASK)

#define USBC_MONITOR_UNUSED_6_MASK               (0xFFC0U)
#define USBC_MONITOR_UNUSED_6_SHIFT              (6U)
/*! UNUSED_6 - Reserved */
#define USBC_MONITOR_UNUSED_6(x)                 (((uint32_t)(((uint32_t)(x)) << USBC_MONITOR_UNUSED_6_SHIFT)) & USBC_MONITOR_UNUSED_6_MASK)

#define USBC_MONITOR_USB_MON_MASK                (0xFFFF0000U)
#define USBC_MONITOR_USB_MON_SHIFT               (16U)
/*! USB_MON - USB_MON */
#define USBC_MONITOR_USB_MON(x)                  (((uint32_t)(((uint32_t)(x)) << USBC_MONITOR_USB_MON_SHIFT)) & USBC_MONITOR_USB_MON_MASK)
/*! @} */

/*! @name RESERVE_ANA - PHY_RESERVE */
/*! @{ */

#define USBC_RESERVE_ANA_PHY_RESERVE_MASK        (0xFFFFFFFFU)
#define USBC_RESERVE_ANA_PHY_RESERVE_SHIFT       (0U)
/*! PHY_RESERVE - PHY_RESERVE */
#define USBC_RESERVE_ANA_PHY_RESERVE(x)          (((uint32_t)(((uint32_t)(x)) << USBC_RESERVE_ANA_PHY_RESERVE_SHIFT)) & USBC_RESERVE_ANA_PHY_RESERVE_MASK)
/*! @} */

/*! @name PHY_REG_OTG_CONTROL - PHY_REG_OTG_CONTROL */
/*! @{ */

#define USBC_PHY_REG_OTG_CONTROL_TESTMON_OTG_MASK (0x7U)
#define USBC_PHY_REG_OTG_CONTROL_TESTMON_OTG_SHIFT (0U)
/*! TESTMON_OTG - TESTMON_OTG */
#define USBC_PHY_REG_OTG_CONTROL_TESTMON_OTG(x)  (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_OTG_CONTROL_TESTMON_OTG_SHIFT)) & USBC_PHY_REG_OTG_CONTROL_TESTMON_OTG_MASK)

#define USBC_PHY_REG_OTG_CONTROL_UNUSED_3_MASK   (0x8U)
#define USBC_PHY_REG_OTG_CONTROL_UNUSED_3_SHIFT  (3U)
/*! UNUSED_3 - Reserved */
#define USBC_PHY_REG_OTG_CONTROL_UNUSED_3(x)     (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_OTG_CONTROL_UNUSED_3_SHIFT)) & USBC_PHY_REG_OTG_CONTROL_UNUSED_3_MASK)

#define USBC_PHY_REG_OTG_CONTROL_PU_OTG_MASK     (0x10U)
#define USBC_PHY_REG_OTG_CONTROL_PU_OTG_SHIFT    (4U)
/*! PU_OTG - PU_OTG */
#define USBC_PHY_REG_OTG_CONTROL_PU_OTG(x)       (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_OTG_CONTROL_PU_OTG_SHIFT)) & USBC_PHY_REG_OTG_CONTROL_PU_OTG_MASK)

#define USBC_PHY_REG_OTG_CONTROL_OTG_CONTROL_BY_PIN_MASK (0x20U)
#define USBC_PHY_REG_OTG_CONTROL_OTG_CONTROL_BY_PIN_SHIFT (5U)
/*! OTG_CONTROL_BY_PIN - OTG_CONTROL_BY_PIN */
#define USBC_PHY_REG_OTG_CONTROL_OTG_CONTROL_BY_PIN(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_OTG_CONTROL_OTG_CONTROL_BY_PIN_SHIFT)) & USBC_PHY_REG_OTG_CONTROL_OTG_CONTROL_BY_PIN_MASK)

#define USBC_PHY_REG_OTG_CONTROL_UNUSED_7_MASK   (0xFFFFFF80U)
#define USBC_PHY_REG_OTG_CONTROL_UNUSED_7_SHIFT  (7U)
/*! UNUSED_7 - Reserved */
#define USBC_PHY_REG_OTG_CONTROL_UNUSED_7(x)     (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_OTG_CONTROL_UNUSED_7_SHIFT)) & USBC_PHY_REG_OTG_CONTROL_UNUSED_7_MASK)
/*! @} */

/*! @name PHY_REG_CHGDTC_CONTRL_1 - PHY_REG_CHGDTC_CONTRL_1 */
/*! @{ */

#define USBC_PHY_REG_CHGDTC_CONTRL_1_TESTMON_CHRGDTC_MASK (0x3U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_TESTMON_CHRGDTC_SHIFT (0U)
/*! TESTMON_CHRGDTC - TESTMON_CHRGDTC */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_TESTMON_CHRGDTC(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_TESTMON_CHRGDTC_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_TESTMON_CHRGDTC_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_EN_MASK (0x4U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_EN_SHIFT (2U)
/*! CDP_EN - CDP_EN */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_EN(x)   (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_EN_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_EN_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_DCP_EN_MASK (0x8U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_DCP_EN_SHIFT (3U)
/*! DCP_EN - DCP_EN */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_DCP_EN(x)   (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_DCP_EN_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_DCP_EN_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_MASK  (0x10U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_SHIFT (4U)
/*! PD_EN - PD_EN */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN(x)    (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_PD_EN_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_PU_CHRG_DTC_MASK (0x20U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_PU_CHRG_DTC_SHIFT (5U)
/*! PU_CHRG_DTC - PU_CHRG_DTC */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_PU_CHRG_DTC(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_PU_CHRG_DTC_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_PU_CHRG_DTC_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_DP_DM_SWAP_CTRL_MASK (0x40U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_DP_DM_SWAP_CTRL_SHIFT (6U)
/*! DP_DM_SWAP_CTRL - DP_DM_SWAP_CTRL */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_DP_DM_SWAP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_DP_DM_SWAP_CTRL_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_DP_DM_SWAP_CTRL_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_DM_AUTO_SWITCH_MASK (0x80U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_DM_AUTO_SWITCH_SHIFT (7U)
/*! CDP_DM_AUTO_SWITCH - CDP_DM_AUTO_SWITCH */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_DM_AUTO_SWITCH(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_DM_AUTO_SWITCH_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_CDP_DM_AUTO_SWITCH_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_VDAT_CHARGE_MASK (0x300U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_VDAT_CHARGE_SHIFT (8U)
/*! VDAT_CHARGE - VDAT_CHARGE */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_VDAT_CHARGE(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_VDAT_CHARGE_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_VDAT_CHARGE_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_VSRC_CHARGE_MASK (0xC00U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_VSRC_CHARGE_SHIFT (10U)
/*! VSRC_CHARGE - VSRC_CHARGE */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_VSRC_CHARGE(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_VSRC_CHARGE_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_VSRC_CHARGE_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DP_MASK (0x1000U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DP_SHIFT (12U)
/*! ENABLE_SWITCH_DP - ENABLE_SWITCH_DP */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DP(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DP_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DP_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DM_MASK (0x2000U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DM_SHIFT (13U)
/*! ENABLE_SWITCH_DM - ENABLE_SWITCH_DM */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DM(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DM_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_ENABLE_SWITCH_DM_MASK)

#define USBC_PHY_REG_CHGDTC_CONTRL_1_UNUSED_14_MASK (0xFFFFC000U)
#define USBC_PHY_REG_CHGDTC_CONTRL_1_UNUSED_14_SHIFT (14U)
/*! UNUSED_14 - Reserved */
#define USBC_PHY_REG_CHGDTC_CONTRL_1_UNUSED_14(x) (((uint32_t)(((uint32_t)(x)) << USBC_PHY_REG_CHGDTC_CONTRL_1_UNUSED_14_SHIFT)) & USBC_PHY_REG_CHGDTC_CONTRL_1_UNUSED_14_MASK)
/*! @} */

/*! @name RESERVED - RESERVED */
/*! @{ */

#define USBC_RESERVED_RESERVED_MASK              (0xFFFFFFFFU)
#define USBC_RESERVED_RESERVED_SHIFT             (0U)
/*! RESERVED - RESERVED */
#define USBC_RESERVED_RESERVED(x)                (((uint32_t)(((uint32_t)(x)) << USBC_RESERVED_RESERVED_SHIFT)) & USBC_RESERVED_RESERVED_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group USBC_Register_Masks */

/* Backward compatibility */
#define GPTIMER0CTL                              GPTIMER0CTRL
#define GPTIMER1CTL                              GPTIMER1CTRL
#define USBC_SBUSCFG                             SBUSCFG
#define EPLISTADDR                               ENDPOINTLISTADDR
#define EPSETUPSR                                ENDPTSETUPSTAT
#define EPPRIME                                  ENDPTPRIME
#define EPFLUSH                                  ENDPTFLUSH
#define EPSR                                     ENDPTSTAT
#define EPCOMPLETE                               ENDPTCOMPLETE
#define EPCR                                     ENDPTCTRL
#define EPCR0                                    ENDPTCTRL0
#define USBHS_ID_ID_MASK                         USBC_ID_ID_MASK
#define USBHS_ID_ID_SHIFT                        USBC_ID_ID_SHIFT
#define USBHS_ID_ID(x)                           USBC_ID_ID(x)
#define USBHS_ID_NID_MASK                        USBC_ID_NID_MASK
#define USBHS_ID_NID_SHIFT                       USBC_ID_NID_SHIFT
#define USBHS_ID_NID(x)                          USBC_ID_NID(x)
#define USBHS_ID_REVISION_MASK                   USBC_ID_REVISION_MASK
#define USBHS_ID_REVISION_SHIFT                  USBC_ID_REVISION_SHIFT
#define USBHS_ID_REVISION(x)                     USBC_ID_REVISION(x)
#define USBHS_HWGENERAL_PHYW_MASK                USBC_HWGENERAL_PHYW_MASK
#define USBHS_HWGENERAL_PHYW_SHIFT               USBC_HWGENERAL_PHYW_SHIFT
#define USBHS_HWGENERAL_PHYW(x)                  USBC_HWGENERAL_PHYW(x)
#define USBHS_HWGENERAL_PHYM_MASK                USBC_HWGENERAL_PHYM_MASK
#define USBHS_HWGENERAL_PHYM_SHIFT               USBC_HWGENERAL_PHYM_SHIFT
#define USBHS_HWGENERAL_PHYM(x)                  USBC_HWGENERAL_PHYM(x)
#define USBHS_HWGENERAL_SM_MASK                  USBC_HWGENERAL_SM_MASK
#define USBHS_HWGENERAL_SM_SHIFT                 USBC_HWGENERAL_SM_SHIFT
#define USBHS_HWGENERAL_SM(x)                    USBC_HWGENERAL_SM(x)
#define USBHS_HWHOST_HC_MASK                     USBC_HWHOST_HC_MASK
#define USBHS_HWHOST_HC_SHIFT                    USBC_HWHOST_HC_SHIFT
#define USBHS_HWHOST_HC(x)                       USBC_HWHOST_HC(x)
#define USBHS_HWHOST_NPORT_MASK                  USBC_HWHOST_NPORT_MASK
#define USBHS_HWHOST_NPORT_SHIFT                 USBC_HWHOST_NPORT_SHIFT
#define USBHS_HWHOST_NPORT(x)                    USBC_HWHOST_NPORT(x)
#define USBHS_HWDEVICE_DC_MASK                   USBC_HWDEVICE_DC_MASK
#define USBHS_HWDEVICE_DC_SHIFT                  USBC_HWDEVICE_DC_SHIFT
#define USBHS_HWDEVICE_DC(x)                     USBC_HWDEVICE_DC(x)
#define USBHS_HWDEVICE_DEVEP_MASK                USBC_HWDEVICE_DEVEP_MASK
#define USBHS_HWDEVICE_DEVEP_SHIFT               USBC_HWDEVICE_DEVEP_SHIFT
#define USBHS_HWDEVICE_DEVEP(x)                  USBC_HWDEVICE_DEVEP(x)
#define USBHS_HWTXBUF_TXBURST_MASK               USBC_HWTXBUF_TXBURST_MASK
#define USBHS_HWTXBUF_TXBURST_SHIFT              USBC_HWTXBUF_TXBURST_SHIFT
#define USBHS_HWTXBUF_TXBURST(x)                 USBC_HWTXBUF_TXBURST(x)
#define USBHS_HWTXBUF_TXCHANADD_MASK             USBC_HWTXBUF_TXCHANADD_MASK
#define USBHS_HWTXBUF_TXCHANADD_SHIFT            USBC_HWTXBUF_TXCHANADD_SHIFT
#define USBHS_HWTXBUF_TXCHANADD(x)               USBC_HWTXBUF_TXCHANADD(x)
#define USBHS_HWRXBUF_RXBURST_MASK               USBC_HWRXBUF_RXBURST_MASK
#define USBHS_HWRXBUF_RXBURST_SHIFT              USBC_HWRXBUF_RXBURST_SHIFT
#define USBHS_HWRXBUF_RXBURST(x)                 USBC_HWRXBUF_RXBURST(x)
#define USBHS_HWRXBUF_RXADD_MASK                 USBC_HWRXBUF_RXADD_MASK
#define USBHS_HWRXBUF_RXADD_SHIFT                USBC_HWRXBUF_RXADD_SHIFT
#define USBHS_HWRXBUF_RXADD(x)                   USBC_HWRXBUF_RXADD(x)
#define USBHS_GPTIMER0LD_GPTLD_MASK              USBC_GPTIMER0LD_GPTLD_MASK
#define USBHS_GPTIMER0LD_GPTLD_SHIFT             USBC_GPTIMER0LD_GPTLD_SHIFT
#define USBHS_GPTIMER0LD_GPTLD(x)                USBC_GPTIMER0LD_GPTLD(x)
#define USBHS_GPTIMER0CTL_GPTCNT_MASK            USBC_GPTIMER0CTRL_GPTCNT_MASK
#define USBHS_GPTIMER0CTL_GPTCNT_SHIFT           USBC_GPTIMER0CTRL_GPTCNT_SHIFT
#define USBHS_GPTIMER0CTL_GPTCNT(x)              USBC_GPTIMER0CTRL_GPTCNT(x)
#define USBHS_GPTIMER0CTL_MODE_MASK              USBC_GPTIMER0CTRL_GPTMODE_MASK
#define USBHS_GPTIMER0CTL_MODE_SHIFT             USBC_GPTIMER0CTRL_GPTMODE_SHIFT
#define USBHS_GPTIMER0CTL_MODE(x)                USBC_GPTIMER0CTRL_GPTMODE(x)
#define USBHS_GPTIMER0CTL_RST_MASK               USBC_GPTIMER0CTRL_GPTRST_MASK
#define USBHS_GPTIMER0CTL_RST_SHIFT              USBC_GPTIMER0CTRL_GPTRST_SHIFT
#define USBHS_GPTIMER0CTL_RST(x)                 USBC_GPTIMER0CTRL_GPTRST(x)
#define USBHS_GPTIMER0CTL_RUN_MASK               USBC_GPTIMER0CTRL_GPTRUN_MASK
#define USBHS_GPTIMER0CTL_RUN_SHIFT              USBC_GPTIMER0CTRL_GPTRUN_SHIFT
#define USBHS_GPTIMER0CTL_RUN(x)                 USBC_GPTIMER0CTRL_GPTRUN(x)
#define USBHS_GPTIMER1LD_GPTLD_MASK              USBC_GPTIMER1LD_GPTLD_MASK
#define USBHS_GPTIMER1LD_GPTLD_SHIFT             USBC_GPTIMER1LD_GPTLD_SHIFT
#define USBHS_GPTIMER1LD_GPTLD(x)                USBC_GPTIMER1LD_GPTLD(x)
#define USBHS_GPTIMER1CTL_GPTCNT_MASK            USBC_GPTIMER1CTRL_GPTCNT_MASK
#define USBHS_GPTIMER1CTL_GPTCNT_SHIFT           USBC_GPTIMER1CTRL_GPTCNT_SHIFT
#define USBHS_GPTIMER1CTL_GPTCNT(x)              USBC_GPTIMER1CTRL_GPTCNT(x)
#define USBHS_GPTIMER1CTL_MODE_MASK              USBC_GPTIMER1CTRL_GPTMODE_MASK
#define USBHS_GPTIMER1CTL_MODE_SHIFT             USBC_GPTIMER1CTRL_GPTMODE_SHIFT
#define USBHS_GPTIMER1CTL_MODE(x)                USBC_GPTIMER1CTRL_GPTMODE(x)
#define USBHS_GPTIMER1CTL_RST_MASK               USBC_GPTIMER1CTRL_GPTRST_MASK
#define USBHS_GPTIMER1CTL_RST_SHIFT              USBC_GPTIMER1CTRL_GPTRST_SHIFT
#define USBHS_GPTIMER1CTL_RST(x)                 USBC_GPTIMER1CTRL_GPTRST(x)
#define USBHS_GPTIMER1CTL_RUN_MASK               USBC_GPTIMER1CTRL_GPTRUN_MASK
#define USBHS_GPTIMER1CTL_RUN_SHIFT              USBC_GPTIMER1CTRL_GPTRUN_SHIFT
#define USBHS_GPTIMER1CTL_RUN(x)                 USBC_GPTIMER1CTRL_GPTRUN(x)
#define USBHS_USBC_SBUSCFG_BURSTMODE_MASK        USBC_SBUSCFG_AHBBRST_MASK
#define USBHS_USBC_SBUSCFG_BURSTMODE_SHIFT       USBC_SBUSCFG_AHBBRST_SHIFT
#define USBHS_USBC_SBUSCFG_BURSTMODE(x)          USBC_SBUSCFG_AHBBRST(x)
#define USBHS_CAPLENGTH_CAPLENGTH_MASK           USBC_CAPLENGTH_CAPLENGTH_MASK
#define USBHS_CAPLENGTH_CAPLENGTH_SHIFT          USBC_CAPLENGTH_CAPLENGTH_SHIFT
#define USBHS_CAPLENGTH_CAPLENGTH(x)             USBC_CAPLENGTH_CAPLENGTH(x)
#define USBHS_CAPLENGTH_HCIVERSION_MASK          USBC_CAPLENGTH_HCIVERSION_MASK
#define USBHS_CAPLENGTH_HCIVERSION_SHIFT         USBC_CAPLENGTH_HCIVERSION_SHIFT
#define USBHS_CAPLENGTH_HCIVERSION(x)            USBC_CAPLENGTH_HCIVERSION(x)
#define USBHS_HCSPARAMS_N_PORTS_MASK             USBC_HCSPARAMS_N_PORTS_MASK
#define USBHS_HCSPARAMS_N_PORTS_SHIFT            USBC_HCSPARAMS_N_PORTS_SHIFT
#define USBHS_HCSPARAMS_N_PORTS(x)               USBC_HCSPARAMS_N_PORTS(x)
#define USBHS_HCSPARAMS_PPC_MASK                 USBC_HCSPARAMS_PPC_MASK
#define USBHS_HCSPARAMS_PPC_SHIFT                USBC_HCSPARAMS_PPC_SHIFT
#define USBHS_HCSPARAMS_PPC(x)                   USBC_HCSPARAMS_PPC(x)
#define USBHS_HCSPARAMS_N_PCC_MASK               USBC_HCSPARAMS_N_PCC_MASK
#define USBHS_HCSPARAMS_N_PCC_SHIFT              USBC_HCSPARAMS_N_PCC_SHIFT
#define USBHS_HCSPARAMS_N_PCC(x)                 USBC_HCSPARAMS_N_PCC(x)
#define USBHS_HCSPARAMS_N_CC_MASK                USBC_HCSPARAMS_N_CC_MASK
#define USBHS_HCSPARAMS_N_CC_SHIFT               USBC_HCSPARAMS_N_CC_SHIFT
#define USBHS_HCSPARAMS_N_CC(x)                  USBC_HCSPARAMS_N_CC(x)
#define USBHS_HCSPARAMS_PI_MASK                  USBC_HCSPARAMS_PI_MASK
#define USBHS_HCSPARAMS_PI_SHIFT                 USBC_HCSPARAMS_PI_SHIFT
#define USBHS_HCSPARAMS_PI(x)                    USBC_HCSPARAMS_PI(x)
#define USBHS_HCSPARAMS_N_PTT_MASK               USBC_HCSPARAMS_N_PTT_MASK
#define USBHS_HCSPARAMS_N_PTT_SHIFT              USBC_HCSPARAMS_N_PTT_SHIFT
#define USBHS_HCSPARAMS_N_PTT(x)                 USBC_HCSPARAMS_N_PTT(x)
#define USBHS_HCSPARAMS_N_TT_MASK                USBC_HCSPARAMS_N_TT_MASK
#define USBHS_HCSPARAMS_N_TT_SHIFT               USBC_HCSPARAMS_N_TT_SHIFT
#define USBHS_HCSPARAMS_N_TT(x)                  USBC_HCSPARAMS_N_TT(x)
#define USBHS_HCCPARAMS_ADC_MASK                 USBC_HCCPARAMS_ADC_MASK
#define USBHS_HCCPARAMS_ADC_SHIFT                USBC_HCCPARAMS_ADC_SHIFT
#define USBHS_HCCPARAMS_ADC(x)                   USBC_HCCPARAMS_ADC(x)
#define USBHS_HCCPARAMS_PFL_MASK                 USBC_HCCPARAMS_PFL_MASK
#define USBHS_HCCPARAMS_PFL_SHIFT                USBC_HCCPARAMS_PFL_SHIFT
#define USBHS_HCCPARAMS_PFL(x)                   USBC_HCCPARAMS_PFL(x)
#define USBHS_HCCPARAMS_ASP_MASK                 USBC_HCCPARAMS_ASP_MASK
#define USBHS_HCCPARAMS_ASP_SHIFT                USBC_HCCPARAMS_ASP_SHIFT
#define USBHS_HCCPARAMS_ASP(x)                   USBC_HCCPARAMS_ASP(x)
#define USBHS_HCCPARAMS_IST_MASK                 USBC_HCCPARAMS_IST_MASK
#define USBHS_HCCPARAMS_IST_SHIFT                USBC_HCCPARAMS_IST_SHIFT
#define USBHS_HCCPARAMS_IST(x)                   USBC_HCCPARAMS_IST(x)
#define USBHS_HCCPARAMS_EECP_MASK                USBC_HCCPARAMS_EECP_MASK
#define USBHS_HCCPARAMS_EECP_SHIFT               USBC_HCCPARAMS_EECP_SHIFT
#define USBHS_HCCPARAMS_EECP(x)                  USBC_HCCPARAMS_EECP(x)
#define USBHS_DCIVERSION_DCIVERSION_MASK         USBC_DCIVERSION_DCIVERSION_MASK
#define USBHS_DCIVERSION_DCIVERSION_SHIFT        USBC_DCIVERSION_DCIVERSION_SHIFT
#define USBHS_DCIVERSION_DCIVERSION(x)           USBC_DCIVERSION_DCIVERSION(x)
#define USBHS_DCCPARAMS_DEN_MASK                 USBC_DCCPARAMS_DEN_MASK
#define USBHS_DCCPARAMS_DEN_SHIFT                USBC_DCCPARAMS_DEN_SHIFT
#define USBHS_DCCPARAMS_DEN(x)                   USBC_DCCPARAMS_DEN(x)
#define USBHS_DCCPARAMS_DC_MASK                  USBC_DCCPARAMS_DC_MASK
#define USBHS_DCCPARAMS_DC_SHIFT                 USBC_DCCPARAMS_DC_SHIFT
#define USBHS_DCCPARAMS_DC(x)                    USBC_DCCPARAMS_DC(x)
#define USBHS_DCCPARAMS_HC_MASK                  USBC_DCCPARAMS_HC_MASK
#define USBHS_DCCPARAMS_HC_SHIFT                 USBC_DCCPARAMS_HC_SHIFT
#define USBHS_DCCPARAMS_HC(x)                    USBC_DCCPARAMS_HC(x)
#define USBHS_USBCMD_RS_MASK                     USBC_USBCMD_RS_MASK
#define USBHS_USBCMD_RS_SHIFT                    USBC_USBCMD_RS_SHIFT
#define USBHS_USBCMD_RS(x)                       USBC_USBCMD_RS(x)
#define USBHS_USBCMD_RST_MASK                    USBC_USBCMD_RST_MASK
#define USBHS_USBCMD_RST_SHIFT                   USBC_USBCMD_RST_SHIFT
#define USBHS_USBCMD_RST(x)                      USBC_USBCMD_RST(x)
#define USBHS_USBCMD_FS_MASK                     USBC_USBCMD_FS1_MASK
#define USBHS_USBCMD_FS_SHIFT                    USBC_USBCMD_FS1_SHIFT
#define USBHS_USBCMD_FS(x)                       USBC_USBCMD_FS1(x)
#define USBHS_USBCMD_PSE_MASK                    USBC_USBCMD_PSE_MASK
#define USBHS_USBCMD_PSE_SHIFT                   USBC_USBCMD_PSE_SHIFT
#define USBHS_USBCMD_PSE(x)                      USBC_USBCMD_PSE(x)
#define USBHS_USBCMD_ASE_MASK                    USBC_USBCMD_ASE_MASK
#define USBHS_USBCMD_ASE_SHIFT                   USBC_USBCMD_ASE_SHIFT
#define USBHS_USBCMD_ASE(x)                      USBC_USBCMD_ASE(x)
#define USBHS_USBCMD_IAA_MASK                    USBC_USBCMD_IAA_MASK
#define USBHS_USBCMD_IAA_SHIFT                   USBC_USBCMD_IAA_SHIFT
#define USBHS_USBCMD_IAA(x)                      USBC_USBCMD_IAA(x)
#define USBHS_USBCMD_ASP_MASK                    USBC_USBCMD_ASP_MASK
#define USBHS_USBCMD_ASP_SHIFT                   USBC_USBCMD_ASP_SHIFT
#define USBHS_USBCMD_ASP(x)                      USBC_USBCMD_ASP(x)
#define USBHS_USBCMD_ASPE_MASK                   USBC_USBCMD_ASPE_MASK
#define USBHS_USBCMD_ASPE_SHIFT                  USBC_USBCMD_ASPE_SHIFT
#define USBHS_USBCMD_ASPE(x)                     USBC_USBCMD_ASPE(x)
#define USBHS_USBCMD_ATDTW_MASK                  USBC_USBCMD_ATDTW_MASK
#define USBHS_USBCMD_ATDTW_SHIFT                 USBC_USBCMD_ATDTW_SHIFT
#define USBHS_USBCMD_ATDTW(x)                    USBC_USBCMD_ATDTW(x)
#define USBHS_USBCMD_SUTW_MASK                   USBC_USBCMD_SUTW_MASK
#define USBHS_USBCMD_SUTW_SHIFT                  USBC_USBCMD_SUTW_SHIFT
#define USBHS_USBCMD_SUTW(x)                     USBC_USBCMD_SUTW(x)
#define USBHS_USBCMD_FS2_MASK                    USBC_USBCMD_FS2_MASK
#define USBHS_USBCMD_FS2_SHIFT                   USBC_USBCMD_FS2_SHIFT
#define USBHS_USBCMD_FS2(x)                      USBC_USBCMD_FS2(x)
#define USBHS_USBCMD_ITC_MASK                    USBC_USBCMD_ITC_MASK
#define USBHS_USBCMD_ITC_SHIFT                   USBC_USBCMD_ITC_SHIFT
#define USBHS_USBCMD_ITC(x)                      USBC_USBCMD_ITC(x)
#define USBHS_USBSTS_UI_MASK                     USBC_USBSTS_UI_MASK
#define USBHS_USBSTS_UI_SHIFT                    USBC_USBSTS_UI_SHIFT
#define USBHS_USBSTS_UI(x)                       USBC_USBSTS_UI(x)
#define USBHS_USBSTS_UEI_MASK                    USBC_USBSTS_UEI_MASK
#define USBHS_USBSTS_UEI_SHIFT                   USBC_USBSTS_UEI_SHIFT
#define USBHS_USBSTS_UEI(x)                      USBC_USBSTS_UEI(x)
#define USBHS_USBSTS_PCI_MASK                    USBC_USBSTS_PCI_MASK
#define USBHS_USBSTS_PCI_SHIFT                   USBC_USBSTS_PCI_SHIFT
#define USBHS_USBSTS_PCI(x)                      USBC_USBSTS_PCI(x)
#define USBHS_USBSTS_FRI_MASK                    USBC_USBSTS_FRI_MASK
#define USBHS_USBSTS_FRI_SHIFT                   USBC_USBSTS_FRI_SHIFT
#define USBHS_USBSTS_FRI(x)                      USBC_USBSTS_FRI(x)
#define USBHS_USBSTS_SEI_MASK                    USBC_USBSTS_SEI_MASK
#define USBHS_USBSTS_SEI_SHIFT                   USBC_USBSTS_SEI_SHIFT
#define USBHS_USBSTS_SEI(x)                      USBC_USBSTS_SEI(x)
#define USBHS_USBSTS_AAI_MASK                    USBC_USBSTS_AAI_MASK
#define USBHS_USBSTS_AAI_SHIFT                   USBC_USBSTS_AAI_SHIFT
#define USBHS_USBSTS_AAI(x)                      USBC_USBSTS_AAI(x)
#define USBHS_USBSTS_URI_MASK                    USBC_USBSTS_URI_MASK
#define USBHS_USBSTS_URI_SHIFT                   USBC_USBSTS_URI_SHIFT
#define USBHS_USBSTS_URI(x)                      USBC_USBSTS_URI(x)
#define USBHS_USBSTS_SRI_MASK                    USBC_USBSTS_SRI_MASK
#define USBHS_USBSTS_SRI_SHIFT                   USBC_USBSTS_SRI_SHIFT
#define USBHS_USBSTS_SRI(x)                      USBC_USBSTS_SRI(x)
#define USBHS_USBSTS_SLI_MASK                    USBC_USBSTS_SLI_MASK
#define USBHS_USBSTS_SLI_SHIFT                   USBC_USBSTS_SLI_SHIFT
#define USBHS_USBSTS_SLI(x)                      USBC_USBSTS_SLI(x)
#define USBHS_USBSTS_ULPII_MASK                  USBC_USBSTS_ULPII_MASK
#define USBHS_USBSTS_ULPII_SHIFT                 USBC_USBSTS_ULPII_SHIFT
#define USBHS_USBSTS_ULPII(x)                    USBC_USBSTS_ULPII(x)
#define USBHS_USBSTS_HCH_MASK                    USBC_USBSTS_HCH_MASK
#define USBHS_USBSTS_HCH_SHIFT                   USBC_USBSTS_HCH_SHIFT
#define USBHS_USBSTS_HCH(x)                      USBC_USBSTS_HCH(x)
#define USBHS_USBSTS_RCL_MASK                    USBC_USBSTS_RCL_MASK
#define USBHS_USBSTS_RCL_SHIFT                   USBC_USBSTS_RCL_SHIFT
#define USBHS_USBSTS_RCL(x)                      USBC_USBSTS_RCL(x)
#define USBHS_USBSTS_PS_MASK                     USBC_USBSTS_PS_MASK
#define USBHS_USBSTS_PS_SHIFT                    USBC_USBSTS_PS_SHIFT
#define USBHS_USBSTS_PS(x)                       USBC_USBSTS_PS(x)
#define USBHS_USBSTS_AS_MASK                     USBC_USBSTS_AS_MASK
#define USBHS_USBSTS_AS_SHIFT                    USBC_USBSTS_AS_SHIFT
#define USBHS_USBSTS_AS(x)                       USBC_USBSTS_AS(x)
#define USBHS_USBSTS_NAKI_MASK                   USBC_USBSTS_NAKI_MASK
#define USBHS_USBSTS_NAKI_SHIFT                  USBC_USBSTS_NAKI_SHIFT
#define USBHS_USBSTS_NAKI(x)                     USBC_USBSTS_NAKI(x)
#define USBHS_USBSTS_TI0_MASK                    USBC_USBSTS_TI0_MASK
#define USBHS_USBSTS_TI0_SHIFT                   USBC_USBSTS_TI0_SHIFT
#define USBHS_USBSTS_TI0(x)                      USBC_USBSTS_TI0(x)
#define USBHS_USBSTS_TI1_MASK                    USBC_USBSTS_TI1_MASK
#define USBHS_USBSTS_TI1_SHIFT                   USBC_USBSTS_TI1_SHIFT
#define USBHS_USBSTS_TI1(x)                      USBC_USBSTS_TI1(x)
#define USBHS_USBINTR_UE_MASK                    USBC_USBINTR_UE_MASK
#define USBHS_USBINTR_UE_SHIFT                   USBC_USBINTR_UE_SHIFT
#define USBHS_USBINTR_UE(x)                      USBC_USBINTR_UE(x)
#define USBHS_USBINTR_UEE_MASK                   USBC_USBINTR_UEE_MASK
#define USBHS_USBINTR_UEE_SHIFT                  USBC_USBINTR_UEE_SHIFT
#define USBHS_USBINTR_UEE(x)                     USBC_USBINTR_UEE(x)
#define USBHS_USBINTR_PCE_MASK                   USBC_USBINTR_PCE_MASK
#define USBHS_USBINTR_PCE_SHIFT                  USBC_USBINTR_PCE_SHIFT
#define USBHS_USBINTR_PCE(x)                     USBC_USBINTR_PCE(x)
#define USBHS_USBINTR_FRE_MASK                   USBC_USBINTR_FRE_MASK
#define USBHS_USBINTR_FRE_SHIFT                  USBC_USBINTR_FRE_SHIFT
#define USBHS_USBINTR_FRE(x)                     USBC_USBINTR_FRE(x)
#define USBHS_USBINTR_SEE_MASK                   USBC_USBINTR_SEE_MASK
#define USBHS_USBINTR_SEE_SHIFT                  USBC_USBINTR_SEE_SHIFT
#define USBHS_USBINTR_SEE(x)                     USBC_USBINTR_SEE(x)
#define USBHS_USBINTR_AAE_MASK                   USBC_USBINTR_AAE_MASK
#define USBHS_USBINTR_AAE_SHIFT                  USBC_USBINTR_AAE_SHIFT
#define USBHS_USBINTR_AAE(x)                     USBC_USBINTR_AAE(x)
#define USBHS_USBINTR_URE_MASK                   USBC_USBINTR_URE_MASK
#define USBHS_USBINTR_URE_SHIFT                  USBC_USBINTR_URE_SHIFT
#define USBHS_USBINTR_URE(x)                     USBC_USBINTR_URE(x)
#define USBHS_USBINTR_SRE_MASK                   USBC_USBINTR_SRE_MASK
#define USBHS_USBINTR_SRE_SHIFT                  USBC_USBINTR_SRE_SHIFT
#define USBHS_USBINTR_SRE(x)                     USBC_USBINTR_SRE(x)
#define USBHS_USBINTR_SLE_MASK                   USBC_USBINTR_SLE_MASK
#define USBHS_USBINTR_SLE_SHIFT                  USBC_USBINTR_SLE_SHIFT
#define USBHS_USBINTR_SLE(x)                     USBC_USBINTR_SLE(x)
#define USBHS_USBINTR_ULPIE_MASK                 USBC_USBINTR_ULPIE_MASK
#define USBHS_USBINTR_ULPIE_SHIFT                USBC_USBINTR_ULPIE_SHIFT
#define USBHS_USBINTR_ULPIE(x)                   USBC_USBINTR_ULPIE(x)
#define USBHS_USBINTR_NAKE_MASK                  USBC_USBINTR_NAKE_MASK
#define USBHS_USBINTR_NAKE_SHIFT                 USBC_USBINTR_NAKE_SHIFT
#define USBHS_USBINTR_NAKE(x)                    USBC_USBINTR_NAKE(x)
#define USBHS_USBINTR_UAIE_MASK                  USBC_USBINTR_UAIE_MASK
#define USBHS_USBINTR_UAIE_SHIFT                 USBC_USBINTR_UAIE_SHIFT
#define USBHS_USBINTR_UAIE(x)                    USBC_USBINTR_UAIE(x)
#define USBHS_USBINTR_UPIE_MASK                  USBC_USBINTR_UPIE_MASK
#define USBHS_USBINTR_UPIE_SHIFT                 USBC_USBINTR_UPIE_SHIFT
#define USBHS_USBINTR_UPIE(x)                    USBC_USBINTR_UPIE(x)
#define USBHS_USBINTR_TIE0_MASK                  USBC_USBINTR_TIE0_MASK
#define USBHS_USBINTR_TIE0_SHIFT                 USBC_USBINTR_TIE0_SHIFT
#define USBHS_USBINTR_TIE0(x)                    USBC_USBINTR_TIE0(x)
#define USBHS_USBINTR_TIE1_MASK                  USBC_USBINTR_TIE1_MASK
#define USBHS_USBINTR_TIE1_SHIFT                 USBC_USBINTR_TIE1_SHIFT
#define USBHS_USBINTR_TIE1(x)                    USBC_USBINTR_TIE1(x)
#define USBHS_FRINDEX_FRINDEX_MASK               USBC_FRINDEX_FRINDEX_MASK
#define USBHS_FRINDEX_FRINDEX_SHIFT              USBC_FRINDEX_FRINDEX_SHIFT
#define USBHS_FRINDEX_FRINDEX(x)                 USBC_FRINDEX_FRINDEX(x)
#define USBHS_DEVICEADDR_USBADRA_MASK            USBC_DEVICEADDR_USBADRA_MASK
#define USBHS_DEVICEADDR_USBADRA_SHIFT           USBC_DEVICEADDR_USBADRA_SHIFT
#define USBHS_DEVICEADDR_USBADRA(x)              USBC_DEVICEADDR_USBADRA(x)
#define USBHS_DEVICEADDR_USBADR_MASK             USBC_DEVICEADDR_USBADR_MASK
#define USBHS_DEVICEADDR_USBADR_SHIFT            USBC_DEVICEADDR_USBADR_SHIFT
#define USBHS_DEVICEADDR_USBADR(x)               USBC_DEVICEADDR_USBADR(x)
#define USBHS_PERIODICLISTBASE_PERBASE_MASK      USBC_PERIODICLISTBASE_PERBASE_MASK
#define USBHS_PERIODICLISTBASE_PERBASE_SHIFT     USBC_PERIODICLISTBASE_PERBASE_SHIFT
#define USBHS_PERIODICLISTBASE_PERBASE(x)        USBC_PERIODICLISTBASE_PERBASE(x)
#define USBHS_ASYNCLISTADDR_ASYBASE_MASK         USBC_ASYNCLISTADDR_ASYBASE_MASK
#define USBHS_ASYNCLISTADDR_ASYBASE_SHIFT        USBC_ASYNCLISTADDR_ASYBASE_SHIFT
#define USBHS_ASYNCLISTADDR_ASYBASE(x)           USBC_ASYNCLISTADDR_ASYBASE(x)
#define USBHS_EPLISTADDR_EPBASE_MASK             USBC_ENDPOINTLISTADDR_EPBASE_MASK
#define USBHS_EPLISTADDR_EPBASE_SHIFT            USBC_ENDPOINTLISTADDR_EPBASE_SHIFT
#define USBHS_EPLISTADDR_EPBASE(x)               USBC_ENDPOINTLISTADDR_EPBASE(x)
#define USBHS_BURSTSIZE_RXPBURST_MASK            USBC_BURSTSIZE_RXPBURST_MASK
#define USBHS_BURSTSIZE_RXPBURST_SHIFT           USBC_BURSTSIZE_RXPBURST_SHIFT
#define USBHS_BURSTSIZE_RXPBURST(x)              USBC_BURSTSIZE_RXPBURST(x)
#define USBHS_BURSTSIZE_TXPBURST_MASK            USBC_BURSTSIZE_TXPBURST_MASK
#define USBHS_BURSTSIZE_TXPBURST_SHIFT           USBC_BURSTSIZE_TXPBURST_SHIFT
#define USBHS_BURSTSIZE_TXPBURST(x)              USBC_BURSTSIZE_TXPBURST(x)
#define USBHS_TXFILLTUNING_TXSCHOH_MASK          USBC_TXFILLTUNING_TXSCHOH_MASK
#define USBHS_TXFILLTUNING_TXSCHOH_SHIFT         USBC_TXFILLTUNING_TXSCHOH_SHIFT
#define USBHS_TXFILLTUNING_TXSCHOH(x)            USBC_TXFILLTUNING_TXSCHOH(x)
#define USBHS_TXFILLTUNING_TXSCHHEALTH_MASK      USBC_TXFILLTUNING_TXSCHHEALTH_MASK
#define USBHS_TXFILLTUNING_TXSCHHEALTH_SHIFT     USBC_TXFILLTUNING_TXSCHHEALTH_SHIFT
#define USBHS_TXFILLTUNING_TXSCHHEALTH(x)        USBC_TXFILLTUNING_TXSCHHEALTH(x)
#define USBHS_TXFILLTUNING_TXFIFOTHRES_MASK      USBC_TXFILLTUNING_TXFIFOTHRES_MASK
#define USBHS_TXFILLTUNING_TXFIFOTHRES_SHIFT     USBC_TXFILLTUNING_TXFIFOTHRES_SHIFT
#define USBHS_TXFILLTUNING_TXFIFOTHRES(x)        USBC_TXFILLTUNING_TXFIFOTHRES(x)
#define USBHS_ENDPTNAK_EPRN_MASK                 USBC_ENDPTNAK_EPRN_MASK
#define USBHS_ENDPTNAK_EPRN_SHIFT                USBC_ENDPTNAK_EPRN_SHIFT
#define USBHS_ENDPTNAK_EPRN(x)                   USBC_ENDPTNAK_EPRN(x)
#define USBHS_ENDPTNAK_EPTN_MASK                 USBC_ENDPTNAK_EPTN_MASK
#define USBHS_ENDPTNAK_EPTN_SHIFT                USBC_ENDPTNAK_EPTN_SHIFT
#define USBHS_ENDPTNAK_EPTN(x)                   USBC_ENDPTNAK_EPTN(x)
#define USBHS_ENDPTNAKEN_EPRNE_MASK              USBC_ENDPTNAKEN_EPRNE_MASK
#define USBHS_ENDPTNAKEN_EPRNE_SHIFT             USBC_ENDPTNAKEN_EPRNE_SHIFT
#define USBHS_ENDPTNAKEN_EPRNE(x)                USBC_ENDPTNAKEN_EPRNE(x)
#define USBHS_ENDPTNAKEN_EPTNE_MASK              USBC_ENDPTNAKEN_EPTNE_MASK
#define USBHS_ENDPTNAKEN_EPTNE_SHIFT             USBC_ENDPTNAKEN_EPTNE_SHIFT
#define USBHS_ENDPTNAKEN_EPTNE(x)                USBC_ENDPTNAKEN_EPTNE(x)
#define USBHS_CONFIGFLAG_CF_MASK                 USBC_CONFIGFLAG_CF_MASK
#define USBHS_CONFIGFLAG_CF_SHIFT                USBC_CONFIGFLAG_CF_SHIFT
#define USBHS_CONFIGFLAG_CF(x)                   USBC_CONFIGFLAG_CF(x)
#define USBHS_PORTSC1_CCS_MASK                   USBC_PORTSC1_CCS_MASK
#define USBHS_PORTSC1_CCS_SHIFT                  USBC_PORTSC1_CCS_SHIFT
#define USBHS_PORTSC1_CCS(x)                     USBC_PORTSC1_CCS(x)
#define USBHS_PORTSC1_CSC_MASK                   USBC_PORTSC1_CSC_MASK
#define USBHS_PORTSC1_CSC_SHIFT                  USBC_PORTSC1_CSC_SHIFT
#define USBHS_PORTSC1_CSC(x)                     USBC_PORTSC1_CSC(x)
#define USBHS_PORTSC1_PE_MASK                    USBC_PORTSC1_PE_MASK
#define USBHS_PORTSC1_PE_SHIFT                   USBC_PORTSC1_PE_SHIFT
#define USBHS_PORTSC1_PE(x)                      USBC_PORTSC1_PE(x)
#define USBHS_PORTSC1_PEC_MASK                   USBC_PORTSC1_PEC_MASK
#define USBHS_PORTSC1_PEC_SHIFT                  USBC_PORTSC1_PEC_SHIFT
#define USBHS_PORTSC1_PEC(x)                     USBC_PORTSC1_PEC(x)
#define USBHS_PORTSC1_OCA_MASK                   USBC_PORTSC1_OCA_MASK
#define USBHS_PORTSC1_OCA_SHIFT                  USBC_PORTSC1_OCA_SHIFT
#define USBHS_PORTSC1_OCA(x)                     USBC_PORTSC1_OCA(x)
#define USBHS_PORTSC1_OCC_MASK                   USBC_PORTSC1_OCC_MASK
#define USBHS_PORTSC1_OCC_SHIFT                  USBC_PORTSC1_OCC_SHIFT
#define USBHS_PORTSC1_OCC(x)                     USBC_PORTSC1_OCC(x)
#define USBHS_PORTSC1_FPR_MASK                   USBC_PORTSC1_FPR_MASK
#define USBHS_PORTSC1_FPR_SHIFT                  USBC_PORTSC1_FPR_SHIFT
#define USBHS_PORTSC1_FPR(x)                     USBC_PORTSC1_FPR(x)
#define USBHS_PORTSC1_SUSP_MASK                  USBC_PORTSC1_SUSP_MASK
#define USBHS_PORTSC1_SUSP_SHIFT                 USBC_PORTSC1_SUSP_SHIFT
#define USBHS_PORTSC1_SUSP(x)                    USBC_PORTSC1_SUSP(x)
#define USBHS_PORTSC1_PR_MASK                    USBC_PORTSC1_PR_MASK
#define USBHS_PORTSC1_PR_SHIFT                   USBC_PORTSC1_PR_SHIFT
#define USBHS_PORTSC1_PR(x)                      USBC_PORTSC1_PR(x)
#define USBHS_PORTSC1_HSP_MASK                   USBC_PORTSC1_HSP_MASK
#define USBHS_PORTSC1_HSP_SHIFT                  USBC_PORTSC1_HSP_SHIFT
#define USBHS_PORTSC1_HSP(x)                     USBC_PORTSC1_HSP(x)
#define USBHS_PORTSC1_LS_MASK                    USBC_PORTSC1_LS_MASK
#define USBHS_PORTSC1_LS_SHIFT                   USBC_PORTSC1_LS_SHIFT
#define USBHS_PORTSC1_LS(x)                      USBC_PORTSC1_LS(x)
#define USBHS_PORTSC1_PP_MASK                    USBC_PORTSC1_PP_MASK
#define USBHS_PORTSC1_PP_SHIFT                   USBC_PORTSC1_PP_SHIFT
#define USBHS_PORTSC1_PP(x)                      USBC_PORTSC1_PP(x)
#define USBHS_PORTSC1_PO_MASK                    USBC_PORTSC1_PO_MASK
#define USBHS_PORTSC1_PO_SHIFT                   USBC_PORTSC1_PO_SHIFT
#define USBHS_PORTSC1_PO(x)                      USBC_PORTSC1_PO(x)
#define USBHS_PORTSC1_PIC_MASK                   USBC_PORTSC1_PIC_MASK
#define USBHS_PORTSC1_PIC_SHIFT                  USBC_PORTSC1_PIC_SHIFT
#define USBHS_PORTSC1_PIC(x)                     USBC_PORTSC1_PIC(x)
#define USBHS_PORTSC1_PTC_MASK                   USBC_PORTSC1_PTC_MASK
#define USBHS_PORTSC1_PTC_SHIFT                  USBC_PORTSC1_PTC_SHIFT
#define USBHS_PORTSC1_PTC(x)                     USBC_PORTSC1_PTC(x)
#define USBHS_PORTSC1_WKCN_MASK                  USBC_PORTSC1_WKCN_MASK
#define USBHS_PORTSC1_WKCN_SHIFT                 USBC_PORTSC1_WKCN_SHIFT
#define USBHS_PORTSC1_WKCN(x)                    USBC_PORTSC1_WKCN(x)
#define USBHS_PORTSC1_WKDS_MASK                  USBC_PORTSC1_WKDS_MASK
#define USBHS_PORTSC1_WKDS_SHIFT                 USBC_PORTSC1_WKDS_SHIFT
#define USBHS_PORTSC1_WKDS(x)                    USBC_PORTSC1_WKDS(x)
#define USBHS_PORTSC1_WKOC_MASK                  USBC_PORTSC1_WKOC_MASK
#define USBHS_PORTSC1_WKOC_SHIFT                 USBC_PORTSC1_WKOC_SHIFT
#define USBHS_PORTSC1_WKOC(x)                    USBC_PORTSC1_WKOC(x)
#define USBHS_PORTSC1_PHCD_MASK                  USBC_PORTSC1_PHCD_MASK
#define USBHS_PORTSC1_PHCD_SHIFT                 USBC_PORTSC1_PHCD_SHIFT
#define USBHS_PORTSC1_PHCD(x)                    USBC_PORTSC1_PHCD(x)
#define USBHS_PORTSC1_PFSC_MASK                  USBC_PORTSC1_PFSC_MASK
#define USBHS_PORTSC1_PFSC_SHIFT                 USBC_PORTSC1_PFSC_SHIFT
#define USBHS_PORTSC1_PFSC(x)                    USBC_PORTSC1_PFSC(x)
#define USBHS_PORTSC1_PTS2_MASK                  USBC_PORTSC1_PTS_2_MASK
#define USBHS_PORTSC1_PTS2_SHIFT                 USBC_PORTSC1_PTS_2_SHIFT
#define USBHS_PORTSC1_PTS2(x)                    USBC_PORTSC1_PTS_2(x)
#define USBHS_PORTSC1_PSPD_MASK                  USBC_PORTSC1_PSPD_MASK
#define USBHS_PORTSC1_PSPD_SHIFT                 USBC_PORTSC1_PSPD_SHIFT
#define USBHS_PORTSC1_PSPD(x)                    USBC_PORTSC1_PSPD(x)
#define USBHS_PORTSC1_PTW_MASK                   USBC_PORTSC1_PTW_MASK
#define USBHS_PORTSC1_PTW_SHIFT                  USBC_PORTSC1_PTW_SHIFT
#define USBHS_PORTSC1_PTW(x)                     USBC_PORTSC1_PTW(x)
#define USBHS_PORTSC1_STS_MASK                   USBC_PORTSC1_STS_MASK
#define USBHS_PORTSC1_STS_SHIFT                  USBC_PORTSC1_STS_SHIFT
#define USBHS_PORTSC1_STS(x)                     USBC_PORTSC1_STS(x)
#define USBHS_PORTSC1_PTS_MASK                   USBC_PORTSC1_PTS_1_MASK
#define USBHS_PORTSC1_PTS_SHIFT                  USBC_PORTSC1_PTS_1_SHIFT
#define USBHS_PORTSC1_PTS(x)                     USBC_PORTSC1_PTS_1(x)
#define USBHS_OTGSC_VD_MASK                      USBC_OTGSC_VD_MASK
#define USBHS_OTGSC_VD_SHIFT                     USBC_OTGSC_VD_SHIFT
#define USBHS_OTGSC_VD(x)                        USBC_OTGSC_VD(x)
#define USBHS_OTGSC_VC_MASK                      USBC_OTGSC_VC_MASK
#define USBHS_OTGSC_VC_SHIFT                     USBC_OTGSC_VC_SHIFT
#define USBHS_OTGSC_VC(x)                        USBC_OTGSC_VC(x)
#define USBHS_OTGSC_OT_MASK                      USBC_OTGSC_OT_MASK
#define USBHS_OTGSC_OT_SHIFT                     USBC_OTGSC_OT_SHIFT
#define USBHS_OTGSC_OT(x)                        USBC_OTGSC_OT(x)
#define USBHS_OTGSC_DP_MASK                      USBC_OTGSC_DP_MASK
#define USBHS_OTGSC_DP_SHIFT                     USBC_OTGSC_DP_SHIFT
#define USBHS_OTGSC_DP(x)                        USBC_OTGSC_DP(x)
#define USBHS_OTGSC_IDPU_MASK                    USBC_OTGSC_IDPU_MASK
#define USBHS_OTGSC_IDPU_SHIFT                   USBC_OTGSC_IDPU_SHIFT
#define USBHS_OTGSC_IDPU(x)                      USBC_OTGSC_IDPU(x)
#define USBHS_OTGSC_ID_MASK                      USBC_OTGSC_ID_MASK
#define USBHS_OTGSC_ID_SHIFT                     USBC_OTGSC_ID_SHIFT
#define USBHS_OTGSC_ID(x)                        USBC_OTGSC_ID(x)
#define USBHS_OTGSC_AVV_MASK                     USBC_OTGSC_AVV_MASK
#define USBHS_OTGSC_AVV_SHIFT                    USBC_OTGSC_AVV_SHIFT
#define USBHS_OTGSC_AVV(x)                       USBC_OTGSC_AVV(x)
#define USBHS_OTGSC_ASV_MASK                     USBC_OTGSC_ASV_MASK
#define USBHS_OTGSC_ASV_SHIFT                    USBC_OTGSC_ASV_SHIFT
#define USBHS_OTGSC_ASV(x)                       USBC_OTGSC_ASV(x)
#define USBHS_OTGSC_BSV_MASK                     USBC_OTGSC_BSV_MASK
#define USBHS_OTGSC_BSV_SHIFT                    USBC_OTGSC_BSV_SHIFT
#define USBHS_OTGSC_BSV(x)                       USBC_OTGSC_BSV(x)
#define USBHS_OTGSC_BSE_MASK                     USBC_OTGSC_BSE_MASK
#define USBHS_OTGSC_BSE_SHIFT                    USBC_OTGSC_BSE_SHIFT
#define USBHS_OTGSC_BSE(x)                       USBC_OTGSC_BSE(x)
#define USBHS_OTGSC_MST_MASK                     USBC_OTGSC_TOG_1MS_MASK
#define USBHS_OTGSC_MST_SHIFT                    USBC_OTGSC_TOG_1MS_SHIFT
#define USBHS_OTGSC_MST(x)                       USBC_OTGSC_TOG_1MS(x)
#define USBHS_OTGSC_DPS_MASK                     USBC_OTGSC_DPS_MASK
#define USBHS_OTGSC_DPS_SHIFT                    USBC_OTGSC_DPS_SHIFT
#define USBHS_OTGSC_DPS(x)                       USBC_OTGSC_DPS(x)
#define USBHS_OTGSC_IDIS_MASK                    USBC_OTGSC_IDIS_MASK
#define USBHS_OTGSC_IDIS_SHIFT                   USBC_OTGSC_IDIS_SHIFT
#define USBHS_OTGSC_IDIS(x)                      USBC_OTGSC_IDIS(x)
#define USBHS_OTGSC_AVVIS_MASK                   USBC_OTGSC_AVVIS_MASK
#define USBHS_OTGSC_AVVIS_SHIFT                  USBC_OTGSC_AVVIS_SHIFT
#define USBHS_OTGSC_AVVIS(x)                     USBC_OTGSC_AVVIS(x)
#define USBHS_OTGSC_ASVIS_MASK                   USBC_OTGSC_ASVIS_MASK
#define USBHS_OTGSC_ASVIS_SHIFT                  USBC_OTGSC_ASVIS_SHIFT
#define USBHS_OTGSC_ASVIS(x)                     USBC_OTGSC_ASVIS(x)
#define USBHS_OTGSC_BSVIS_MASK                   USBC_OTGSC_BSVIS_MASK
#define USBHS_OTGSC_BSVIS_SHIFT                  USBC_OTGSC_BSVIS_SHIFT
#define USBHS_OTGSC_BSVIS(x)                     USBC_OTGSC_BSVIS(x)
#define USBHS_OTGSC_BSEIS_MASK                   USBC_OTGSC_BSEIS_MASK
#define USBHS_OTGSC_BSEIS_SHIFT                  USBC_OTGSC_BSEIS_SHIFT
#define USBHS_OTGSC_BSEIS(x)                     USBC_OTGSC_BSEIS(x)
#define USBHS_OTGSC_MSS_MASK                     USBC_OTGSC_STATUS_1MS_MASK
#define USBHS_OTGSC_MSS_SHIFT                    USBC_OTGSC_STATUS_1MS_SHIFT
#define USBHS_OTGSC_MSS(x)                       USBC_OTGSC_STATUS_1MS(x)
#define USBHS_OTGSC_DPIS_MASK                    USBC_OTGSC_DPIS_MASK
#define USBHS_OTGSC_DPIS_SHIFT                   USBC_OTGSC_DPIS_SHIFT
#define USBHS_OTGSC_DPIS(x)                      USBC_OTGSC_DPIS(x)
#define USBHS_OTGSC_IDIE_MASK                    USBC_OTGSC_IDIE_MASK
#define USBHS_OTGSC_IDIE_SHIFT                   USBC_OTGSC_IDIE_SHIFT
#define USBHS_OTGSC_IDIE(x)                      USBC_OTGSC_IDIE(x)
#define USBHS_OTGSC_AVVIE_MASK                   USBC_OTGSC_AVVIE_MASK
#define USBHS_OTGSC_AVVIE_SHIFT                  USBC_OTGSC_AVVIE_SHIFT
#define USBHS_OTGSC_AVVIE(x)                     USBC_OTGSC_AVVIE(x)
#define USBHS_OTGSC_ASVIE_MASK                   USBC_OTGSC_ASVIE_MASK
#define USBHS_OTGSC_ASVIE_SHIFT                  USBC_OTGSC_ASVIE_SHIFT
#define USBHS_OTGSC_ASVIE(x)                     USBC_OTGSC_ASVIE(x)
#define USBHS_OTGSC_BSVIE_MASK                   USBC_OTGSC_BSVIE_MASK
#define USBHS_OTGSC_BSVIE_SHIFT                  USBC_OTGSC_BSVIE_SHIFT
#define USBHS_OTGSC_BSVIE(x)                     USBC_OTGSC_BSVIE(x)
#define USBHS_OTGSC_BSEIE_MASK                   USBC_OTGSC_BSEIE_MASK
#define USBHS_OTGSC_BSEIE_SHIFT                  USBC_OTGSC_BSEIE_SHIFT
#define USBHS_OTGSC_BSEIE(x)                     USBC_OTGSC_BSEIE(x)
#define USBHS_OTGSC_MSE_MASK                     USBC_OTGSC_EN_1MS_MASK
#define USBHS_OTGSC_MSE_SHIFT                    USBC_OTGSC_EN_1MS_SHIFT
#define USBHS_OTGSC_MSE(x)                       USBC_OTGSC_EN_1MS(x)
#define USBHS_OTGSC_DPIE_MASK                    USBC_OTGSC_DPIE_MASK
#define USBHS_OTGSC_DPIE_SHIFT                   USBC_OTGSC_DPIE_SHIFT
#define USBHS_OTGSC_DPIE(x)                      USBC_OTGSC_DPIE(x)
#define USBHS_USBMODE_CM_MASK                    USBC_USBMODE_CM_MASK
#define USBHS_USBMODE_CM_SHIFT                   USBC_USBMODE_CM_SHIFT
#define USBHS_USBMODE_CM(x)                      USBC_USBMODE_CM(x)
#define USBHS_USBMODE_ES_MASK                    USBC_USBMODE_ES_MASK
#define USBHS_USBMODE_ES_SHIFT                   USBC_USBMODE_ES_SHIFT
#define USBHS_USBMODE_ES(x)                      USBC_USBMODE_ES(x)
#define USBHS_USBMODE_SLOM_MASK                  USBC_USBMODE_SLOM_MASK
#define USBHS_USBMODE_SLOM_SHIFT                 USBC_USBMODE_SLOM_SHIFT
#define USBHS_USBMODE_SLOM(x)                    USBC_USBMODE_SLOM(x)
#define USBHS_USBMODE_SDIS_MASK                  USBC_USBMODE_SDIS_MASK
#define USBHS_USBMODE_SDIS_SHIFT                 USBC_USBMODE_SDIS_SHIFT
#define USBHS_USBMODE_SDIS(x)                    USBC_USBMODE_SDIS(x)
#define USBHS_EPSETUPSR_EPSETUPSTAT_MASK         USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_MASK
#define USBHS_EPSETUPSR_EPSETUPSTAT_SHIFT        USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT_SHIFT
#define USBHS_EPSETUPSR_EPSETUPSTAT(x)           USBC_ENDPTSETUPSTAT_ENDPTSETUPSTAT(x)
#define USBHS_EPPRIME_PERB_MASK                  USBC_ENDPTPRIME_PERB_MASK
#define USBHS_EPPRIME_PERB_SHIFT                 USBC_ENDPTPRIME_PERB_SHIFT
#define USBHS_EPPRIME_PERB(x)                    USBC_ENDPTPRIME_PERB(x)
#define USBHS_EPPRIME_PETB_MASK                  USBC_ENDPTPRIME_PETB_MASK
#define USBHS_EPPRIME_PETB_SHIFT                 USBC_ENDPTPRIME_PETB_SHIFT
#define USBHS_EPPRIME_PETB(x)                    USBC_ENDPTPRIME_PETB(x)
#define USBHS_EPFLUSH_FERB_MASK                  USBC_ENDPTFLUSH_FERB_MASK
#define USBHS_EPFLUSH_FERB_SHIFT                 USBC_ENDPTFLUSH_FERB_SHIFT
#define USBHS_EPFLUSH_FERB(x)                    USBC_ENDPTFLUSH_FERB(x)
#define USBHS_EPFLUSH_FETB_MASK                  USBC_ENDPTFLUSH_FETB_MASK
#define USBHS_EPFLUSH_FETB_SHIFT                 USBC_ENDPTFLUSH_FETB_SHIFT
#define USBHS_EPFLUSH_FETB(x)                    USBC_ENDPTFLUSH_FETB(x)
#define USBHS_EPSR_ERBR_MASK                     USBC_ENDPTSTAT_ERBR_MASK
#define USBHS_EPSR_ERBR_SHIFT                    USBC_ENDPTSTAT_ERBR_SHIFT
#define USBHS_EPSR_ERBR(x)                       USBC_ENDPTSTAT_ERBR(x)
#define USBHS_EPSR_ETBR_MASK                     USBC_ENDPTSTAT_ETBR_MASK
#define USBHS_EPSR_ETBR_SHIFT                    USBC_ENDPTSTAT_ETBR_SHIFT
#define USBHS_EPSR_ETBR(x)                       USBC_ENDPTSTAT_ETBR(x)
#define USBHS_EPCOMPLETE_ERCE_MASK               USBC_ENDPTCOMPLETE_ERCE_MASK
#define USBHS_EPCOMPLETE_ERCE_SHIFT              USBC_ENDPTCOMPLETE_ERCE_SHIFT
#define USBHS_EPCOMPLETE_ERCE(x)                 USBC_ENDPTCOMPLETE_ERCE(x)
#define USBHS_EPCOMPLETE_ETCE_MASK               USBC_ENDPTCOMPLETE_ETCE_MASK
#define USBHS_EPCOMPLETE_ETCE_SHIFT              USBC_ENDPTCOMPLETE_ETCE_SHIFT
#define USBHS_EPCOMPLETE_ETCE(x)                 USBC_ENDPTCOMPLETE_ETCE(x)
#define USBHS_EPCR0_RXS_MASK                     USBC_ENDPTCTRL0_RXS_MASK
#define USBHS_EPCR0_RXS_SHIFT                    USBC_ENDPTCTRL0_RXS_SHIFT
#define USBHS_EPCR0_RXS(x)                       USBC_ENDPTCTRL0_RXS(x)
#define USBHS_EPCR0_RXT_MASK                     USBC_ENDPTCTRL0_RXT_MASK
#define USBHS_EPCR0_RXT_SHIFT                    USBC_ENDPTCTRL0_RXT_SHIFT
#define USBHS_EPCR0_RXT(x)                       USBC_ENDPTCTRL0_RXT(x)
#define USBHS_EPCR0_RXE_MASK                     USBC_ENDPTCTRL0_RXE_MASK
#define USBHS_EPCR0_RXE_SHIFT                    USBC_ENDPTCTRL0_RXE_SHIFT
#define USBHS_EPCR0_RXE(x)                       USBC_ENDPTCTRL0_RXE(x)
#define USBHS_EPCR0_TXS_MASK                     USBC_ENDPTCTRL0_TXS_MASK
#define USBHS_EPCR0_TXS_SHIFT                    USBC_ENDPTCTRL0_TXS_SHIFT
#define USBHS_EPCR0_TXS(x)                       USBC_ENDPTCTRL0_TXS(x)
#define USBHS_EPCR0_TXT_MASK                     USBC_ENDPTCTRL0_TXT_MASK
#define USBHS_EPCR0_TXT_SHIFT                    USBC_ENDPTCTRL0_TXT_SHIFT
#define USBHS_EPCR0_TXT(x)                       USBC_ENDPTCTRL0_TXT(x)
#define USBHS_EPCR0_TXE_MASK                     USBC_ENDPTCTRL0_TXE_MASK
#define USBHS_EPCR0_TXE_SHIFT                    USBC_ENDPTCTRL0_TXE_SHIFT
#define USBHS_EPCR0_TXE(x)                       USBC_ENDPTCTRL0_TXE(x)
#define USBHS_EPCR_RXS_MASK                      USBC_ENDPTCTRL_RXS_MASK
#define USBHS_EPCR_RXS_SHIFT                     USBC_ENDPTCTRL_RXS_SHIFT
#define USBHS_EPCR_RXS(x)                        USBC_ENDPTCTRL_RXS(x)
#define USBHS_EPCR_RXD_MASK                      USBC_ENDPTCTRL_RXD_MASK
#define USBHS_EPCR_RXD_SHIFT                     USBC_ENDPTCTRL_RXD_SHIFT
#define USBHS_EPCR_RXD(x)                        USBC_ENDPTCTRL_RXD(x)
#define USBHS_EPCR_RXT_MASK                      USBC_ENDPTCTRL_RXT_MASK
#define USBHS_EPCR_RXT_SHIFT                     USBC_ENDPTCTRL_RXT_SHIFT
#define USBHS_EPCR_RXT(x)                        USBC_ENDPTCTRL_RXT(x)
#define USBHS_EPCR_RXI_MASK                      USBC_ENDPTCTRL_RXI_MASK
#define USBHS_EPCR_RXI_SHIFT                     USBC_ENDPTCTRL_RXI_SHIFT
#define USBHS_EPCR_RXI(x)                        USBC_ENDPTCTRL_RXI(x)
#define USBHS_EPCR_RXR_MASK                      USBC_ENDPTCTRL_RXR_MASK
#define USBHS_EPCR_RXR_SHIFT                     USBC_ENDPTCTRL_RXR_SHIFT
#define USBHS_EPCR_RXR(x)                        USBC_ENDPTCTRL_RXR(x)
#define USBHS_EPCR_RXE_MASK                      USBC_ENDPTCTRL_RXE_MASK
#define USBHS_EPCR_RXE_SHIFT                     USBC_ENDPTCTRL_RXE_SHIFT
#define USBHS_EPCR_RXE(x)                        USBC_ENDPTCTRL_RXE(x)
#define USBHS_EPCR_TXS_MASK                      USBC_ENDPTCTRL_TXS_MASK
#define USBHS_EPCR_TXS_SHIFT                     USBC_ENDPTCTRL_TXS_SHIFT
#define USBHS_EPCR_TXS(x)                        USBC_ENDPTCTRL_TXS(x)
#define USBHS_EPCR_TXD_MASK                      USBC_ENDPTCTRL_TXD_MASK
#define USBHS_EPCR_TXD_SHIFT                     USBC_ENDPTCTRL_TXD_SHIFT
#define USBHS_EPCR_TXD(x)                        USBC_ENDPTCTRL_TXD(x)
#define USBHS_EPCR_TXT_MASK                      USBC_ENDPTCTRL_TXT_MASK
#define USBHS_EPCR_TXT_SHIFT                     USBC_ENDPTCTRL_TXT_SHIFT
#define USBHS_EPCR_TXT(x)                        USBC_ENDPTCTRL_TXT(x)
#define USBHS_EPCR_TXI_MASK                      USBC_ENDPTCTRL_TXI_MASK
#define USBHS_EPCR_TXI_SHIFT                     USBC_ENDPTCTRL_TXI_SHIFT
#define USBHS_EPCR_TXI(x)                        USBC_ENDPTCTRL_TXI(x)
#define USBHS_EPCR_TXR_MASK                      USBC_ENDPTCTRL_TXR_MASK
#define USBHS_EPCR_TXR_SHIFT                     USBC_ENDPTCTRL_TXR_SHIFT
#define USBHS_EPCR_TXR(x)                        USBC_ENDPTCTRL_TXR(x)
#define USBHS_EPCR_TXE_MASK                      USBC_ENDPTCTRL_TXE_MASK
#define USBHS_EPCR_TXE_SHIFT                     USBC_ENDPTCTRL_TXE_SHIFT
#define USBHS_EPCR_TXE(x)                        USBC_ENDPTCTRL_TXE(x)
#define USBHS_EPCR_COUNT                         USBC_ENDPTCTRL_COUNT
#define USBHS_Type                               USBC_Type
#define USBHS_BASE_ADDRS                         { USBOTG_BASE }
#define USBHS_IRQS                               { USB_IRQn }
#define USBHS_IRQHandler                         USB_IRQHandler


/*!
 * @}
 */ /* end of group USBC_Peripheral_Access_Layer */


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


#endif  /* USBC_H_ */

